// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// pmaphlp.cpp : Implementation of CfooApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include "PMapObj.h"

STDMETHODIMP CPropertyMapObject::get_String(BSTR bstrName, BSTR* pbstr)
{
	HRESULT hr = S_OK;
	VARIANT var;
	::VariantInit(&var);

	ASSERT(pbstr);
	if (pbstr == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::get_BSTR: outgoing pointer (BSTR*) is NULL\n");
		return E_POINTER;
	}

	*pbstr = NULL;

	hr = get_Property(bstrName, &var);
	if (FAILED(hr))
		return hr;

	if (var.vt != VT_BSTR)
	{
#ifdef _DEBUG
		CString str(bstrName);
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::get_BSTR: name (%s), property not BSTR (%d)\n", str, var.vt);
#endif

		::VariantClear(&var);

		return VWOBJECT_E_WRONGPROPERTYTYPE;
	}

	*pbstr = var.bstrVal; // ::SysAllocStringLen(var.bstrVal, ::SysStringLen(var.bstrVal));

	return hr;
}

STDMETHODIMP CPropertyMapObject::get_ObjectPropertyExt(BSTR bstrName, REFIID iid, IObjectProperty** ppobjectproperty)
{
	IDispatch* pdisp = NULL;
	HRESULT hr = S_OK;

	ASSERT(ppobjectproperty);
	if (ppobjectproperty == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::get_ObjectPropertyExt: outgoing pointer (IObjectProperty**) is NULL\n");
		return E_POINTER;
	}

	*ppobjectproperty = NULL;

	hr = get_Dispatch(bstrName, &pdisp);
	if (FAILED(hr))
		return hr;

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
			// verify interface supports desired interface
			if (FAILED(pdisp->QueryInterface(iid, (LPVOID*)ppobjectproperty)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::get_ObjectPropertyExt: QI failed for desired interface\n");
				hr = VWOBJECT_E_INTERFACENOTSUPPORTED;
				goto ERROR_ENCOUNTERED;
			}
		}
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pdisp);

	return hr;
}

STDMETHODIMP CPropertyMapObject::get_ObjectProperty(BSTR bstrName, IObjectProperty** ppobjectproperty)
{
	return get_ObjectPropertyExt(bstrName, IID_IObjectProperty, ppobjectproperty);
}

STDMETHODIMP CPropertyMapObject::get_Thing(BSTR bstrName, IThing** ppthing)
{
	return get_ObjectPropertyExt(bstrName, IID_IThing, (IObjectProperty**)ppthing);
}

STDMETHODIMP CPropertyMapObject::get_BOOL(BSTR bstrName, VARIANT_BOOL* pboolVal)
{
	HRESULT hr = S_OK;
	VARIANT var;
	::VariantInit(&var);

	ASSERT(pboolVal);
	if (pboolVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::get_BOOL: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return E_POINTER;
	}

	*pboolVal = VARIANT_FALSE;

	hr = get_Property(bstrName, &var);
	if (FAILED(hr))
		return hr;

	if (var.vt != VT_BOOL)
	{
#ifdef _DEBUG
		CString str(bstrName);
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::get_BOOL: name (%s), property not BOOL (%d)\n", str, var.vt);
#endif

		::VariantClear(&var);

		return VWOBJECT_E_WRONGPROPERTYTYPE;
	}

	*pboolVal = var.boolVal;

	return hr;
}

STDMETHODIMP CPropertyMapObject::get_Long(BSTR bstrName, long* plval)
{
	HRESULT hr = S_OK;
	VARIANT var;
	::VariantInit(&var);

	ASSERT(plval);
	if (plval == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::get_Long: outgoing pointer (long*) is NULL\n");
		return E_POINTER;
	}

	*plval = 0;

	hr = get_Property(bstrName, &var);
	if (FAILED(hr))
		return hr;

	if (var.vt != VT_I4)
	{
#ifdef _DEBUG
		CString str(bstrName);
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::get_Long: name (%s), property not I4 (%d)\n", str, var.vt);
#endif

		::VariantClear(&var);

		return VWOBJECT_E_WRONGPROPERTYTYPE;
	}

	*plval = var.lVal;

	return hr;
}

STDMETHODIMP CPropertyMapObject::get_Short(BSTR bstrName, short* psval)
{
	HRESULT hr = S_OK;
	VARIANT var;
	::VariantInit(&var);

	ASSERT(psval);
	if (psval == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::get_Short: outgoing pointer (short*) is NULL\n");
		return E_POINTER;
	}

	*psval = 0;

	hr = get_Property(bstrName, &var);
	if (FAILED(hr))
		return hr;

	if (var.vt != VT_I2)
	{
#ifdef _DEBUG
		CString str(bstrName);
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::get_Short: name (%s), property not I2 (%d)\n", str, var.vt);
#endif

		::VariantClear(&var);

		return VWOBJECT_E_WRONGPROPERTYTYPE;
	}

	*psval = var.iVal;

	return hr;
}

STDMETHODIMP CPropertyMapObject::get_Dispatch(BSTR bstrName, IDispatch** ppdisp)
{
	HRESULT hr = S_OK;
	VARIANT var;
	::VariantInit(&var);

	ASSERT(ppdisp);
	if (ppdisp == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::get_Dispatch: outgoing pointer (IDispatch**) is NULL\n");
		return E_POINTER;
	}

	*ppdisp = NULL;

	hr = get_Property(bstrName, &var);
	if (FAILED(hr))
		return hr;

	if (var.vt != VT_DISPATCH)
	{
#ifdef _DEBUG
		CString str(bstrName);
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::get_Dispatch: name (%s), property not dispatch (%d)\n", str, var.vt);
#endif

		::VariantClear(&var);

		return VWOBJECT_E_WRONGPROPERTYTYPE;
	}

	*ppdisp = var.pdispVal;
//	SAFEADDREF(*ppdisp);

	return hr;
}

STDMETHODIMP CPropertyMapObject::put_String(BSTR bstrName, BSTR bstr)
{
	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_BSTR;
	var.bstrVal = bstr;

	return put_Property(bstrName, var);
}

STDMETHODIMP CPropertyMapObject::put_ObjectPropertyExt(BSTR bstrName, REFIID iid, IObjectProperty* pobjectproperty)
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
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::put_ObjectPropertyExt: QI failed for desired inteface\n");
			return VWOBJECT_E_INTERFACENOTSUPPORTED;
		}
	}

	HRESULT hr = put_Dispatch(bstrName, pobjectproperty);

	return hr;
}

STDMETHODIMP CPropertyMapObject::put_ObjectProperty(BSTR bstrName, IObjectProperty* pobjectproperty)
{
	return put_ObjectPropertyExt(bstrName, IID_IObjectProperty, pobjectproperty);
}

STDMETHODIMP CPropertyMapObject::put_Thing(BSTR bstrName, IThing* pthing)
{
	return put_ObjectPropertyExt(bstrName, IID_IThing, pthing);
}

STDMETHODIMP CPropertyMapObject::put_BOOL(BSTR bstrName, VARIANT_BOOL boolVal)
{
	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_BOOL;
	var.boolVal = boolVal;

	return put_Property(bstrName, var);
}

STDMETHODIMP CPropertyMapObject::put_Long(BSTR bstrName, long lval)
{
	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_I4;
	var.lVal = lval;

	return put_Property(bstrName, var);
}

STDMETHODIMP CPropertyMapObject::put_Short(BSTR bstrName, short sval)
{
	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_I2;
	var.iVal = sval;

	return put_Property(bstrName, var);
}

STDMETHODIMP CPropertyMapObject::put_Dispatch(BSTR bstrName, IDispatch* pdisp)
{
	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_DISPATCH;
	var.pdispVal = pdisp;

	SAFEADDREF(var.pdispVal);
	HRESULT hr = put_Property(bstrName, var);
	SAFERELEASE(var.pdispVal);

	return hr;
}

