// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// StatMgr.cpp : Implementation of CvwstatsApp and DLL registration.

#include "stdafx.h"
#include <vwstats.h>
#include <vwobject.h>
#include "StatMgr.h"
#include "syshelp.h"

/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP CVWStatColObject::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IVWStatCol,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CVWStatColObject::Initialize(IWorld *pworld)
{
	HRESULT hr = S_OK;	

	// Initialize can only be called once. This insures that.
	if (m_pPropMap)
	{
		hr = VWSTATS_INITIALIZED_TWICE;
		goto ERROR_ENCOUNTERED;
	}

	m_pWorld = pworld;
	SAFEADDREF(m_pWorld);
	
	hr = CreatePropertyMap(m_pWorld, &m_pPropMap);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	return hr;

}

CVWStatColObject::~CVWStatColObject()
{
	SAFERELEASE(m_pPropMap);
	SAFERELEASE(m_pWorld);
}

STDMETHODIMP CVWStatColObject::get_Item(BSTR bstrCategory, BSTR bstrItem, VARIANT* pRetVar)
{
	HRESULT hr = S_OK;
	IPropertyMap *pItemMap = NULL;
	
	if (!m_pPropMap || !m_pWorld) 
	{
		hr = VWSTATS_NOT_INITIALIZED;
		goto ERROR_ENCOUNTERED;		
	} 

	//Check if the bstrCategory is there.
	hr = m_pPropMap->get_ObjectProperty(bstrCategory, (IObjectProperty**)&pItemMap);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	//get bstrItem from the existing property map.
	hr = pItemMap->get_Property(bstrItem, pRetVar);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	SAFERELEASE(pItemMap);

	return hr;
}

STDMETHODIMP CVWStatColObject::put_Item(BSTR bstrCategory, BSTR bstrItem, VARIANT var)
{
	HRESULT hr = S_OK;
	IPropertyMap *pItemMap = NULL;
	
	if (!m_pPropMap || !m_pWorld) 
	{
		hr = VWSTATS_NOT_INITIALIZED;
		goto ERROR_ENCOUNTERED;		
	} 

	//Check if the bstrCategory is there already.
	hr = m_pPropMap->get_ObjectProperty(bstrCategory, (IObjectProperty**)&pItemMap);
	if (FAILED(hr))
	{
		//create a new itemMap.
		hr = CreatePropertyMap(m_pWorld, &pItemMap);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		//insert it into bstrCategorymap.
		hr = m_pPropMap->put_ObjectProperty(bstrCategory, pItemMap);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;		
	}

	//put bstrItem in the existing property map.
	hr = pItemMap->put_Property(bstrItem, var);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	SAFERELEASE(pItemMap);

	return hr;
}
	
STDMETHODIMP CVWStatColObject::get_LongItem(BSTR bstrCategory, BSTR bstrItem, long* piRet)
{
	HRESULT hr = S_OK;
	CComVariant var;

	if (piRet == NULL)
		return E_POINTER;

	*piRet = 0;

	if (!m_pPropMap || !m_pWorld) 
	{
		hr = VWSTATS_NOT_INITIALIZED;
		goto ERROR_ENCOUNTERED;		
	} 

	hr = get_Item(bstrCategory, bstrItem, &var);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;		

	if (var.vt != VT_I4)
	{
		::VariantClear(&var);

		hr = VWSTATS_WRONG_DATATYPE;
		goto ERROR_ENCOUNTERED;		
	}

	*piRet = var.lVal;

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CVWStatColObject::put_LongItem(BSTR bstrCategory, BSTR bstrItem, long inp)
{
	VARIANT var;
	::VariantInit(&var);

	if (!m_pPropMap || !m_pWorld) 
		return VWSTATS_NOT_INITIALIZED ;

	var.vt = VT_I4;
	var.lVal = inp;

	return put_Item(bstrCategory, bstrItem, var);
}
	
STDMETHODIMP CVWStatColObject::get_BSTRItem(BSTR bstrCategory, BSTR bstrItem, BSTR * pbstrRet)
{
	HRESULT hr = S_OK;
	VARIANT var;	

	::VariantInit(&var);

	if (pbstrRet == NULL)
		return E_POINTER;

	*pbstrRet = NULL;

	if (!m_pPropMap || !m_pWorld) 
	{
		hr = VWSTATS_NOT_INITIALIZED;
		goto ERROR_ENCOUNTERED;		
	} 

	hr = get_Item(bstrCategory, bstrItem, &var);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;	

	if (var.vt != VT_BSTR)
	{
		::VariantClear(&var);

		hr = VWSTATS_WRONG_DATATYPE;
		goto ERROR_ENCOUNTERED;
	}
	
	*pbstrRet = var.bstrVal;

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CVWStatColObject::put_BSTRItem(BSTR bstrCategory, BSTR bstrItem, BSTR bstr)
{
	VARIANT var;
	::VariantInit(&var);

	if (!m_pPropMap || !m_pWorld) 
		return VWSTATS_NOT_INITIALIZED;

	var.vt = VT_BSTR;
	var.bstrVal = bstr;

	return put_Item(bstrCategory,bstrItem,var);
}

STDMETHODIMP CVWStatColObject::RemoveItem(BSTR bstrCategory, BSTR bstrVariable)
{
	HRESULT hr = S_OK;
	IPropertyMap *pItemMap;
	long size = 0;
	
	if (!m_pPropMap || !m_pWorld) 
	{
		hr = VWSTATS_NOT_INITIALIZED;
		goto ERROR_ENCOUNTERED;		
	} 

	// Check if the bstrCategory is there.
	hr = m_pPropMap->get_ObjectProperty(bstrCategory, (IObjectProperty**)&pItemMap);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;		

	// remove bstrItem from the existing property map.
	hr = pItemMap->Remove(bstrVariable);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pItemMap->get_Count(&size);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// remove bstrCategory if no items left.
	if (size == 0)
	{
		hr = m_pPropMap->Remove(bstrCategory);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pItemMap);

	return hr;

}
	
STDMETHODIMP CVWStatColObject::get_Count(long* retval)
{
	HRESULT hr = S_OK;

	if (!m_pPropMap || !m_pWorld) 
	{
		hr = VWSTATS_NOT_INITIALIZED;
		goto ERROR_ENCOUNTERED;		
	} 

	hr = m_pPropMap->get_Count(retval);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CVWStatColObject::get_PropertyMap(BSTR bstrCategory, IPropertyMap** ppRetMap)
{
	HRESULT hr = S_OK;

	if (!m_pPropMap || !m_pWorld) 
	{
		hr = VWSTATS_NOT_INITIALIZED;
		goto ERROR_ENCOUNTERED;		
	} 

	//Check if the bstrCategory is there.
	hr = m_pPropMap->get_ObjectProperty(bstrCategory, (IObjectProperty**)ppRetMap);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	return hr;
}
