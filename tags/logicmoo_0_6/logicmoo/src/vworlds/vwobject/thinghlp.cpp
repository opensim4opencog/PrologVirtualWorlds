// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// ThingHlp.cpp : Implementation of CfooApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include "thingobj.h"

#define ALLOW_CREATEANDPUT

// Utility functions
HRESULT VariantToLong(VARIANT var, long* dest, long defvalue);
HRESULT VariantToBSTR(VARIANT var, BSTR* dest, BSTR defvalue);

// basic helper functions for all common VARIANT types

// IDispatch helper functions

STDMETHODIMP CThingObject::AddDispatch(BSTR bstrName, IDispatch* pdisp)
{
	HRESULT hr = S_OK;
	VARIANT var;

	::VariantInit(&var);

	var.vt = VT_DISPATCH;
	var.pdispVal = pdisp;

	SAFEADDREF(var.pdispVal);
	hr = AddPropertyExt(bstrName, var, PSBIT_SCRIPTDEFAULTPROPERTY, PS_SCRIPTDEFAULTPROPERTY, VT_DISPATCH, IID_IDispatch, NULL);
	SAFERELEASE(var.pdispVal);

	return hr;
}

STDMETHODIMP CThingObject::get_Dispatch(BSTR bstrName, IDispatch** ppdisp)
{
	HRESULT hr = S_OK;
	VARIANT var;
	::VariantInit(&var);

	ASSERT(ppdisp);
	if (ppdisp == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_Dispatch: outgoing pointer (IDispatch**) is NULL\n");
		return ReportThingError(E_POINTER);
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::get_Dispatch: stub object, shouldn't be accessing\n");
		return ReportThingError(VWOBJECT_E_STUBOBJECT);
	}

	*ppdisp = NULL;

	hr = get_Property(bstrName, &var);
	if (FAILED(hr))
		return hr;

	if (var.vt != VT_DISPATCH)
	{
#ifdef _DEBUG
		CString str(bstrName);
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_Dispatch: name %s, property not dispatch (%d)\n", str, var.vt);
#endif

		::VariantClear(&var);

		return ReportThingError(VWOBJECT_E_WRONGPROPERTYTYPE);
	}

	*ppdisp = var.pdispVal;
//	SAFEADDREF(*ppdisp);

	return hr;
}

STDMETHODIMP CThingObject::put_Dispatch(BSTR bstrName, IDispatch* pdisp)
{
	HRESULT hr = S_OK;
	VARIANT var;

	::VariantInit(&var);

	var.vt = VT_DISPATCH;
	var.pdispVal = pdisp;

	SAFEADDREF(var.pdispVal);
	hr = put_Property(bstrName, var);
	SAFERELEASE(var.pdispVal);

	return hr;
}

// IObjectProperty-based helper functions (takes IID to QI for)

STDMETHODIMP CThingObject::AddObjectPropertyExt(BSTR bstrName, REFIID iid, IObjectProperty* pobjectproperty)
{
	if (pobjectproperty != NULL)
	{
		IObjectProperty* pprop = NULL;

		// verify desired interface is supported
		if (SUCCEEDED(pobjectproperty->QueryInterface(iid, (LPVOID*)&pprop)))
		{
			SAFERELEASE(pprop);
		}
		else
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::AddObjectPropertyExt: QI failed for desired interface\n");
			return ReportThingError(VWOBJECT_E_INTERFACENOTSUPPORTED);
		}
	}

	HRESULT hr = S_OK;

	hr = AddPropertyExt(bstrName, CComVariant(pobjectproperty), PSBIT_SCRIPTDEFAULTPROPERTY, PS_SCRIPTDEFAULTPROPERTY, VT_DISPATCH, iid, NULL);

	return hr;
}

STDMETHODIMP CThingObject::get_ObjectPropertyExt(BSTR bstrName, REFIID iid, IObjectProperty** ppobjectproperty)
{
	HRESULT hr = S_OK;
	IDispatch* pdisp = NULL;

	ASSERT(ppobjectproperty);
	if (ppobjectproperty == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_ObjectPropertyExt: outgoing pointer (IObjectProperty**) is NULL\n");
		return ReportThingError(E_POINTER);
	}

	*ppobjectproperty = NULL;

	hr = get_Dispatch(bstrName, &pdisp);

	if (pdisp != NULL)
	{
		if (iid == IID_NULL)
		{
			// if IID_NULL, just cast and addref, don't QI
			*ppobjectproperty = (IObjectProperty*)pdisp;
			SAFEADDREF(*ppobjectproperty);
		}
		else
		{
			// verify desired interface is supported
			if (FAILED(pdisp->QueryInterface(iid, (LPVOID*)ppobjectproperty)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_ObjectPropertyExt: QI failed for desired interface\n");
				hr = ReportThingError(VWOBJECT_E_INTERFACENOTSUPPORTED);
				goto ERROR_ENCOUNTERED;
			}
		}
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pdisp);

	return hr;
}

STDMETHODIMP CThingObject::put_ObjectPropertyExt(BSTR bstrName, REFIID iid, IObjectProperty* pobjectproperty)
{
	if (pobjectproperty != NULL)
	{
		IObjectProperty* pprop = NULL;

		// verify desired interface is supported
		if (SUCCEEDED(pobjectproperty->QueryInterface(iid, (LPVOID*)&pprop)))
		{
			SAFERELEASE(pprop);
		}
		else
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::put_ObjectPropertyExt: QI failed for desired interface\n");
			return ReportThingError(VWOBJECT_E_INTERFACENOTSUPPORTED);
		}
	}

	HRESULT hr = put_Dispatch(bstrName, pobjectproperty);

	return hr;
}

// IObjectProperty helpers

STDMETHODIMP CThingObject::AddObjectProperty(BSTR bstrName, IObjectProperty* pobjectproperty)
{
	IID iid;

	memcpy(&iid, &IID_IObjectProperty, sizeof(IID));

	if (pobjectproperty)
		pobjectproperty->get_IID(&iid);

	return AddObjectPropertyExt(bstrName, iid, pobjectproperty);
}

STDMETHODIMP CThingObject::get_ObjectProperty(BSTR bstrName, IObjectProperty** ppobjectproperty)
{
	return get_ObjectPropertyExt(bstrName, IID_IObjectProperty, ppobjectproperty);
}

STDMETHODIMP CThingObject::put_ObjectProperty(BSTR bstrName, IObjectProperty* pobjectproperty)
{
	return put_ObjectPropertyExt(bstrName, IID_IObjectProperty, pobjectproperty);
}

// IThing helpers

STDMETHODIMP CThingObject::AddThing(BSTR bstrName, IThing* pthing)
{
	return AddObjectPropertyExt(bstrName, IID_IThing, pthing);
}

STDMETHODIMP CThingObject::get_Thing(BSTR bstrName, IThing** ppthing)
{
	return get_ObjectPropertyExt(bstrName, IID_IThing, (IObjectProperty**)ppthing);
}

STDMETHODIMP CThingObject::put_Thing(BSTR bstrName, IThing* pthing)
{
	return put_ObjectPropertyExt(bstrName, IID_IThing, pthing);
}

// BSTR helper functions

STDMETHODIMP CThingObject::AddString(BSTR bstrName, BSTR bstr)
{
	HRESULT hr = S_OK;
	VARIANT var;

	::VariantInit(&var);

	var.vt = VT_BSTR;
	var.bstrVal = bstr;

	hr = AddPropertyExt(bstrName, var, PSBIT_SCRIPTDEFAULTPROPERTY, PS_SCRIPTDEFAULTPROPERTY, VT_BSTR, IID_NULL, NULL);

	return hr;
}

STDMETHODIMP CThingObject::get_String(BSTR bstrName, BSTR* pbstr)
{
	HRESULT hr = S_OK;
	VARIANT var;
	::VariantInit(&var);

	ASSERT(pbstr);
	if (pbstr == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_BSTR: outgoing pointer (BSTR*) is NULL\n");
		return ReportThingError(E_POINTER);
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::get_BSTR: stub object, shouldn't be accessing\n");
		return ReportThingError(VWOBJECT_E_STUBOBJECT);
	}

	*pbstr = NULL;

	hr = get_Property(bstrName, &var);
	if (FAILED(hr))
		return hr;

	if (var.vt != VT_BSTR)
	{
#ifdef _DEBUG
		CString str(bstrName);
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_BSTR: name %s, property not BSTR (%d)\n", str, var.vt);
#endif

		::VariantClear(&var);

		return ReportThingError(VWOBJECT_E_WRONGPROPERTYTYPE);
	}

	*pbstr = var.bstrVal; // ::SysAllocStringLen(var.bstrVal, ::SysStringLen(var.bstrVal));

	return hr;
}

STDMETHODIMP CThingObject::put_String(BSTR bstrName, BSTR bstr)
{
	HRESULT hr = S_OK;
	VARIANT var;

	::VariantInit(&var);

	var.vt = VT_BSTR;
	var.bstrVal = bstr;

	hr = put_Property(bstrName, var);

	return hr;
}

// BOOL helper functions

STDMETHODIMP CThingObject::AddBOOL(BSTR bstrName, VARIANT_BOOL boolVal)
{
	HRESULT hr = S_OK;
	VARIANT var;

	::VariantInit(&var);

	var.vt = VT_BOOL;
	var.boolVal = boolVal;

	hr = AddPropertyExt(bstrName, var, PSBIT_SCRIPTDEFAULTPROPERTY, PS_SCRIPTDEFAULTPROPERTY, VT_BOOL,IID_IDispatch,NULL);

	return hr;
}

STDMETHODIMP CThingObject::get_BOOL(BSTR bstrName, VARIANT_BOOL* pboolVal)
{
	HRESULT hr = S_OK;
	VARIANT var;
	::VariantInit(&var);

	ASSERT(pboolVal);
	if (pboolVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_BOOL: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return ReportThingError(E_POINTER);
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::get_BOOL: stub object, shouldn't be accessing\n");
		return ReportThingError(VWOBJECT_E_STUBOBJECT);
	}

	*pboolVal = VARIANT_FALSE;

	hr = get_Property(bstrName, &var);
	if (FAILED(hr))
		return hr;

	if (var.vt != VT_BOOL)
	{
#ifdef _DEBUG
		CString str(bstrName);
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_BOOL: name %s, property not BOOL (%d)\n", str, var.vt);
#endif

		::VariantClear(&var);

		return ReportThingError(VWOBJECT_E_WRONGPROPERTYTYPE);
	}

	*pboolVal = var.boolVal;

	return hr;
}

STDMETHODIMP CThingObject::put_BOOL(BSTR bstrName, VARIANT_BOOL boolVal)
{
	HRESULT hr = S_OK;
	VARIANT var;

	::VariantInit(&var);

	var.vt = VT_BOOL;
	var.boolVal = boolVal;

	hr = put_Property(bstrName, var);

	return hr;
}

// long helper functions

STDMETHODIMP CThingObject::AddLong(BSTR bstrName, long lval)
{
	HRESULT hr = S_OK;
	VARIANT var;

	::VariantInit(&var);

	var.vt = VT_I4;
	var.lVal = lval;

	hr = AddPropertyExt(bstrName, var, PSBIT_SCRIPTDEFAULTPROPERTY, PS_SCRIPTDEFAULTPROPERTY, VT_I4, IID_NULL, NULL);

	return hr;
}

STDMETHODIMP CThingObject::get_Long(BSTR bstrName, long* plval)
{
	HRESULT hr = S_OK;
	VARIANT var;
	::VariantInit(&var);

	ASSERT(plval);
	if (plval == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_Long: outgoing pointer (long*) is NULL\n");
		return ReportThingError(E_POINTER);
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::get_Long: stub object, shouldn't be accessing\n");
		return ReportThingError(VWOBJECT_E_STUBOBJECT);
	}

	*plval = 0;

	hr = get_Property(bstrName, &var);
	if (FAILED(hr))
		return hr;

	if (var.vt != VT_I4)
	{
#ifdef _DEBUG
		CString str(bstrName);
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_Long: name %s, property not I4 (%d)\n", str, var.vt);
#endif

		::VariantClear(&var);

		return ReportThingError(VWOBJECT_E_WRONGPROPERTYTYPE);
	}

	*plval = var.lVal;

	return hr;
}

STDMETHODIMP CThingObject::put_Long(BSTR bstrName, long lval)
{
	HRESULT hr = S_OK;
	VARIANT var;

	::VariantInit(&var);

	var.vt = VT_I4;
	var.lVal = lval;

	hr = put_Property(bstrName, var);

	return hr;
}

// short helper functions

STDMETHODIMP CThingObject::AddShort(BSTR bstrName, short sval)
{
	HRESULT hr = S_OK;
	VARIANT var;

	::VariantInit(&var);

	var.vt = VT_I2;
	var.iVal = sval;

	hr = AddPropertyExt(bstrName, var, PSBIT_SCRIPTDEFAULTPROPERTY, PS_SCRIPTDEFAULTPROPERTY, VT_I2, IID_NULL, NULL);

	return hr;
}

STDMETHODIMP CThingObject::get_Short(BSTR bstrName, short* psval)
{
	HRESULT hr = S_OK;
	VARIANT var;
	::VariantInit(&var);

	ASSERT(psval);
	if (psval == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_Short: outgoing pointer (short*) is NULL\n");
		return ReportThingError(E_POINTER);
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::get_Short: stub object, shouldn't be accessing\n");
		return ReportThingError(VWOBJECT_E_STUBOBJECT);
	}

	*psval = 0;

	hr = get_Property(bstrName, &var);
	if (FAILED(hr))
		return hr;

	if (var.vt != VT_I2)
	{
#ifdef _DEBUG
		CString str(bstrName);
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_Short: name %s, property not I2 (%d)\n", str, var.vt);
#endif

		::VariantClear(&var);

		return ReportThingError(VWOBJECT_E_WRONGPROPERTYTYPE);
	}

	*psval = var.iVal;

	return hr;
}

STDMETHODIMP CThingObject::put_Short(BSTR bstrName, short sval)
{
	HRESULT hr = S_OK;
	VARIANT var;

	::VariantInit(&var);

	var.vt = VT_I2;
	var.iVal = sval;

	hr = put_Property(bstrName, var);

	return hr;
}

STDMETHODIMP CThingObject::CreateAndAddMethod(long lFlags, IModule *pModule, BSTR bstrMethName,
										 VARIANT varFuncName, VARIANT varPsbits, VARIANT varPermissions)
{
	HRESULT hr = S_OK;
	long psbits = PSBIT_SCRIPTDEFAULTMETHOD;
	long permissions = PS_SCRIPTDEFAULTMETHOD;
	CComBSTR bstrFuncName;

	VariantToLong(varPsbits, &psbits, psbits);
	VariantToLong(varPermissions, &permissions, permissions);
	VariantToBSTR(varFuncName, &bstrFuncName, bstrMethName);

	// deal with optional param case
	if (bstrFuncName.Length() == 0)
		bstrFuncName = bstrMethName;

	// bstrMethName: method name within thing
	// bstrFuncName: method name within module
	
	return CreateAndAddMethodExt(lFlags, pModule, bstrMethName, bstrFuncName, psbits, permissions);
}

// REVIEW: funcName comes before MethName in C++ entrypoint, backwards in script entrypoint
STDMETHODIMP CThingObject::CreateAndAddMethodExt(long lFlags, IModule *pModule,
	BSTR bstrFuncName, BSTR bstrMethName, long psbits, long permissions)
{
	HRESULT hr = S_OK;
	IMethod *pMethod = NULL;
	VARIANT_BOOL bValid = VARIANT_FALSE;

	// bstrMethName: method name within thing
	// bstrFuncName: method name within module

	// Create the new method
	if (FAILED(hr = m_pWorld->CreateMethod(lFlags, CComVariant(pModule), bstrFuncName, &pMethod)))
		goto ERROR_ENCOUNTERED;

#ifdef ALLOW_CREATEANDPUT
	if (FAILED(hr = IsValidMethod(bstrMethName, &bValid)))
		goto ERROR_ENCOUNTERED;

	// Put or Add the method depending upon whether there's one there already
	if (!bValid)
	{
#endif
		if (FAILED(hr = AddMethodExt(bstrMethName, pMethod, psbits, permissions)))
			goto ERROR_ENCOUNTERED;
#ifdef ALLOW_CREATEANDPUT
	}
	else
	{
		if (FAILED(hr = put_Method(bstrMethName, pMethod)))
			goto ERROR_ENCOUNTERED;
	}
#endif

ERROR_ENCOUNTERED:
	SAFERELEASE(pMethod);

	return hr;
}

STDMETHODIMP CThingObject::CreateAndAddProperty(BSTR bstrProgID, BSTR bstrPropName, VARIANT var)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bIsValid = VARIANT_FALSE;
	IObjectProperty* pProperty = NULL;

	// Create the new property
	if (FAILED(hr = m_pWorld->CreateObjectProperty(bstrProgID, &pProperty)))
		goto ERROR_ENCOUNTERED;

#ifdef ALLOW_CREATEANDPUT
	// Check if it already exists
	if (FAILED(IsValidProperty(bstrPropName, &bIsValid)))
		goto ERROR_ENCOUNTERED;

	if (!bIsValid)
	{
#endif
		// If not, add it
		if (FAILED(hr = AddObjectProperty(bstrPropName, pProperty)))
			goto ERROR_ENCOUNTERED;
#ifdef ALLOW_CREATEANDPUT
	}
	else
	{
		// Now set it
		if (FAILED(hr = put_ObjectProperty(bstrPropName, pProperty)))
			goto ERROR_ENCOUNTERED;
	}
#endif

ERROR_ENCOUNTERED:
	SAFERELEASE(pProperty);

	return hr;
}
/*
HRESULT VariantToLong(VARIANT var, long* dest, long defvalue)
{
	HRESULT hr = S_OK;

	if (var.vt == VT_ERROR) 
		*dest = defvalue;
	else if (var.vt == VT_I4) 
		*dest = var.lVal;
	else
	{
		CComVariant temp;
		
		hr = ::VariantChangeType(&temp, &var, 0, VT_I4);
		if (FAILED(hr))
			hr = DISP_E_TYPEMISMATCH;
		else 
			*dest = temp.lVal;
	}

	return hr;
}
*/