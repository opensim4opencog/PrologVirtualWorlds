// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// plisthlp.cpp : Implementation of CfooApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include "PListObj.h"

STDMETHODIMP CPropertyListObject::AddString(BSTR bstr)
{
	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_BSTR;
	var.bstrVal = bstr;

	return Add(var);
}

STDMETHODIMP CPropertyListObject::AddObjectPropertyExt(REFIID iid, IObjectProperty* pobjectproperty)
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
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::AddObjectPropertyExt: QI failed for desired interface\n");
			return VWOBJECT_E_INTERFACENOTSUPPORTED;
		}
	}

	HRESULT hr = AddDispatch(pobjectproperty);

	return hr;
}

STDMETHODIMP CPropertyListObject::AddObjectProperty(IObjectProperty* pobjectproperty)
{
	return AddObjectPropertyExt(IID_IObjectProperty, pobjectproperty);
}

STDMETHODIMP CPropertyListObject::AddThing(IThing* pthing)
{
	return AddObjectPropertyExt(IID_IThing, pthing);
}

STDMETHODIMP CPropertyListObject::AddBOOL(VARIANT_BOOL boolVal)
{
	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_BOOL;
	var.boolVal = boolVal;

	return Add(var);
}

STDMETHODIMP CPropertyListObject::AddLong(long lval)
{
	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_I4;
	var.lVal = lval;

	return Add(var);
}

STDMETHODIMP CPropertyListObject::AddShort(short sval)
{
	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_I2;
	var.iVal = sval;

	return Add(var);
}

STDMETHODIMP CPropertyListObject::AddDispatch(IDispatch* pdisp)
{
	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_DISPATCH;
	var.pdispVal = pdisp;

	SAFEADDREF(var.pdispVal);
	HRESULT hr = Add(var);
	SAFERELEASE(var.pdispVal);

	return hr;
}

STDMETHODIMP CPropertyListObject::InsertStringAt(long index, BSTR bstr)
{
	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_BSTR;
	var.bstrVal = bstr;

	return InsertAt(index, var);
}

STDMETHODIMP CPropertyListObject::InsertObjectPropertyExtAt(long index, REFIID iid, IObjectProperty* pobjectproperty)
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
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::InsertObjectPropertyExtAt: QI failed for desired interface\n");
			return VWOBJECT_E_INTERFACENOTSUPPORTED;
		}
	}

	HRESULT hr = InsertDispatchAt(index, pobjectproperty);

	return hr;
}

STDMETHODIMP CPropertyListObject::InsertObjectPropertyAt(long index, IObjectProperty* pobjectproperty)
{
	return InsertObjectPropertyExtAt(index, IID_IObjectProperty, pobjectproperty);
}

STDMETHODIMP CPropertyListObject::InsertThingAt(long index, IThing* pthing)
{
	return InsertObjectPropertyExtAt(index, IID_IThing, pthing);
}

STDMETHODIMP CPropertyListObject::InsertBOOLAt(long index, VARIANT_BOOL boolVal)
{
	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_BOOL;
	var.boolVal = boolVal;

	return InsertAt(index, var);
}

STDMETHODIMP CPropertyListObject::InsertLongAt(long index, long lval)
{
	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_I4;
	var.lVal = lval;

	return InsertAt(index, var);
}

STDMETHODIMP CPropertyListObject::InsertShortAt(long index, short sval)
{
	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_I2;
	var.iVal = sval;

	return InsertAt(index, var);
}

STDMETHODIMP CPropertyListObject::InsertDispatchAt(long index, IDispatch* pdisp)
{
	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_DISPATCH;
	var.pdispVal = pdisp;

	SAFEADDREF(var.pdispVal);
	HRESULT hr = InsertAt(index, var);
	SAFERELEASE(var.pdispVal);

	return hr;
}

STDMETHODIMP CPropertyListObject::RemoveString(BSTR bstr)
{
	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_BSTR;
	var.bstrVal = bstr;

	return Remove(var);
}

STDMETHODIMP CPropertyListObject::RemoveObjectPropertyExt(REFIID iid, IObjectProperty* pobjectproperty)
{
	if (pobjectproperty != NULL)
	{
		IObjectProperty* pprop = NULL;

		// verify it's actually an IObjectProperty ptr
		if (SUCCEEDED(pobjectproperty->QueryInterface(iid, (LPVOID*)&pprop)))
		{
			SAFERELEASE(pprop);
		}
		else
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::RemoveObjectPropertyExt: QI failed for desired interface\n");
			return VWOBJECT_E_INTERFACENOTSUPPORTED;
		}
	}

	HRESULT hr = RemoveDispatch(pobjectproperty);

	return hr;
}

STDMETHODIMP CPropertyListObject::RemoveObjectProperty(IObjectProperty* pobjectproperty)
{
	return RemoveObjectPropertyExt(IID_IObjectProperty, pobjectproperty);
}

STDMETHODIMP CPropertyListObject::RemoveThing(IThing* pthing)
{
	return RemoveObjectPropertyExt(IID_IThing, pthing);
}

STDMETHODIMP CPropertyListObject::RemoveBOOL(VARIANT_BOOL boolVal)
{
	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_BOOL;
	var.boolVal = boolVal;

	return Remove(var);
}

STDMETHODIMP CPropertyListObject::RemoveLong(long lval)
{
	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_I4;
	var.lVal = lval;

	return Remove(var);
}

STDMETHODIMP CPropertyListObject::RemoveShort(short sval)
{
	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_I2;
	var.iVal = sval;

	return Remove(var);
}

STDMETHODIMP CPropertyListObject::RemoveDispatch(IDispatch* pdisp)
{
	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_DISPATCH;
	var.pdispVal = pdisp;

	SAFEADDREF(var.pdispVal);
	HRESULT hr = Remove(var);
	SAFERELEASE(var.pdispVal);

	return hr;
}

STDMETHODIMP CPropertyListObject::get_String(long Index, BSTR* pbstr)
{
	HRESULT hr = S_OK;
	VARIANT var;
	::VariantInit(&var);
	
	ASSERT(pbstr);
	if (pbstr == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::get_BSTR: outgoing pointer (BSTR*) is NULL\n");
		return E_POINTER;
	}

	*pbstr = NULL;

	hr = get_Property(Index, &var);
	if (FAILED(hr))
		return hr;

	if (var.vt != VT_BSTR)
	{
		::VariantClear(&var);

		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::get_BSTR: property not BSTR (%d)\n", var.vt);
		return VWOBJECT_E_WRONGPROPERTYTYPE;
	}

	*pbstr = var.bstrVal; //::SysAllocStringLen(var.bstrVal, ::SysStringLen(var.bstrVal));

	return hr;
}

STDMETHODIMP CPropertyListObject::get_ObjectPropertyExt(long Index, REFIID iid, IObjectProperty** ppobjectproperty)
{
	HRESULT hr = S_OK;
	IDispatch* pdisp = NULL;

	ASSERT(ppobjectproperty);
	if (ppobjectproperty == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::get_ObjectPropertyExt: outgoing pointer (IObjectProperty**) is NULL\n");
		return E_POINTER;
	}

	*ppobjectproperty = NULL;

	hr = get_Dispatch(Index, &pdisp);
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
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::get_ObjectPropertyExt: QI failed for desired interface\n");
				hr = VWOBJECT_E_INTERFACENOTSUPPORTED;
				goto ERROR_ENCOUNTERED;
			}
		}
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pdisp);

	return hr;
}

STDMETHODIMP CPropertyListObject::get_ObjectProperty(long Index, IObjectProperty** ppobjectproperty)
{
	return get_ObjectPropertyExt(Index, IID_IObjectProperty, ppobjectproperty);
}

STDMETHODIMP CPropertyListObject::get_Thing(long Index, IThing** ppthing)
{
	return get_ObjectPropertyExt(Index, IID_IThing, (IObjectProperty**)ppthing);
}

STDMETHODIMP CPropertyListObject::get_BOOL(long Index, VARIANT_BOOL* pboolVal)
{
	HRESULT hr = S_OK;
	VARIANT var;
	::VariantInit(&var);

	ASSERT(pboolVal);
	if (pboolVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::get_BOOL: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return E_POINTER;
	}

	*pboolVal = VARIANT_FALSE;

	hr = get_Property(Index, &var);
	if (FAILED(hr))
		return hr;

	if (var.vt != VT_BOOL)
	{
		::VariantClear(&var);

		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::get_BOOL: property not BOOL (%d)\n", var.vt);
		return VWOBJECT_E_WRONGPROPERTYTYPE;
	}

	*pboolVal = var.boolVal;

	return hr;
}

STDMETHODIMP CPropertyListObject::get_Long(long Index, long* plval)
{
	HRESULT hr = S_OK;
	VARIANT var;
	::VariantInit(&var);

	ASSERT(plval);
	if (plval == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::get_Long: outgoing pointer (long*) is NULL\n");
		return E_POINTER;
	}
	
	*plval = 0;

	hr = get_Property(Index, &var);
	if (FAILED(hr))
		return hr;

	if (var.vt != VT_I4)
	{
		::VariantClear(&var);

		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::get_Long: property not I4 (%d)\n", var.vt);
		return VWOBJECT_E_WRONGPROPERTYTYPE;
	}

	*plval = var.lVal;

	return hr;
}

STDMETHODIMP CPropertyListObject::get_Short(long Index, short* psval)
{
	HRESULT hr = S_OK;
	VARIANT var;
	::VariantInit(&var);
	
	ASSERT(psval);
	if (psval == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::get_Short: outgoing pointer (short*) is NULL\n");
		return E_POINTER;
	}

	*psval = 0;

	hr = get_Property(Index, &var);
	if (FAILED(hr))
		return hr;

	if (var.vt != VT_I2)
	{
		::VariantClear(&var);

		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::get_Short: property not I2 (%d)\n", var.vt);
		return VWOBJECT_E_WRONGPROPERTYTYPE;
	}

	*psval = var.iVal;

	return hr;
}

STDMETHODIMP CPropertyListObject::get_Dispatch(long Index, IDispatch** ppdisp)
{
	HRESULT hr = S_OK;
	VARIANT var;
	::VariantInit(&var);
	
	ASSERT(ppdisp);
	if (ppdisp == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::get_Dispatch: outgoing pointer (IDispatch**) is NULL\n");
		return E_POINTER;
	}

	*ppdisp = 0;

	hr = get_Property(Index, &var);
	if (FAILED(hr))
		return hr;

	if (var.vt != VT_DISPATCH)
	{
		::VariantClear(&var);

		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::get_Dispatch: property not dispatch (%d)\n", var.vt);
		return VWOBJECT_E_WRONGPROPERTYTYPE;
	}

	*ppdisp = var.pdispVal;
//	SAFEADDREF(*ppdisp);

	return hr;
}

STDMETHODIMP CPropertyListObject::put_String(long Index, BSTR bstr)
{
	HRESULT hr = S_OK;

	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_BSTR;
	var.bstrVal = bstr;

	hr = put_Property(Index, var);

	return hr;
}

STDMETHODIMP CPropertyListObject::put_ObjectPropertyExt(long Index, REFIID iid, IObjectProperty* pobjectproperty)
{
	if (pobjectproperty != NULL)
	{
		IObjectProperty* pprop = NULL;

		// verify it's actually an IObjectProperty ptr
		if (SUCCEEDED(pobjectproperty->QueryInterface(iid, (LPVOID*)&pprop)))
		{
			SAFERELEASE(pprop);
		}
		else
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::put_ObjectPropertyExt: QI failed for desired interface\n");
			return VWOBJECT_E_INTERFACENOTSUPPORTED;
		}
	}

	HRESULT hr = put_Dispatch(Index, pobjectproperty);

	return hr;
}

STDMETHODIMP CPropertyListObject::put_ObjectProperty(long Index, IObjectProperty* pobjectproperty)
{
	return put_ObjectPropertyExt(Index, IID_IObjectProperty, pobjectproperty);
}

STDMETHODIMP CPropertyListObject::put_Thing(long Index, IThing* pthing)
{
	return put_ObjectPropertyExt(Index, IID_IThing, pthing);
}

STDMETHODIMP CPropertyListObject::put_BOOL(long Index, VARIANT_BOOL boolVal)
{
	HRESULT hr = S_OK;
	
	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_BOOL;
	var.boolVal = boolVal;

	hr = put_Property(Index, var);

	return hr;
}

STDMETHODIMP CPropertyListObject::put_Long(long Index, long lval)
{
	HRESULT hr = S_OK;
	
	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_I4;
	var.lVal = lval;

	hr = put_Property(Index, var);

	return hr;
}

STDMETHODIMP CPropertyListObject::put_Short(long Index, short sval)
{
	HRESULT hr = S_OK;
	
	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_I2;
	var.iVal = sval;

	hr = put_Property(Index, var);

	return hr;
}

STDMETHODIMP CPropertyListObject::put_Dispatch(long Index, IDispatch* pdisp)
{
	HRESULT hr = S_OK;

	VARIANT var;
	::VariantInit(&var);

	var.vt = VT_DISPATCH;
	var.pdispVal = pdisp;

	SAFEADDREF(var.pdispVal);
	hr = put_Property(Index, var);
	SAFERELEASE(var.pdispVal);

	return hr;
}

