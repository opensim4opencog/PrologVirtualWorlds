// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWTraItO.cpp : Implementation of CVWTransactionItemObject 

#include "stdafx.h"
#include <vwobject.h>
#include "tranito.h"
#include "tranitem.h"
#include "syshelp.h"

IMPLEMENT_OBJECTPROPERTY(CVWTransactionItemObject)

CVWTransactionItemObject::CVWTransactionItemObject()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVWTransactionItemObject::CVWTransactionItemObject, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif
	m_bstrName = "A new object";
	m_bstrDescription = "";
	m_bstrGraphic = "";
	m_bstrExemplarType = "Artifact";
	m_pPropertyValues = NULL;
}

CVWTransactionItemObject::~CVWTransactionItemObject()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVWTransactionItemObject::~CVWTransactionItemObject, this 0x%0x\n", this);
#endif
	Terminate();
}

STDMETHODIMP CVWTransactionItemObject::Terminate(void)
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVWTransactionItemObject::Terminate, this 0x%0x\n", this);
#endif

	CVWTransactionItemObjectBase::Terminate();

	SAFERELEASE(m_pPropertyValues);
	return S_OK;
}

// Methods for accessing name of transaction item

STDMETHODIMP CVWTransactionItemObject::get_Name(BSTR *pbstrName)
{
	*pbstrName = m_bstrName.Copy();
	return S_OK;
}

STDMETHODIMP CVWTransactionItemObject::put_Name(BSTR bstrName)
{
	m_bstrName = bstrName;
	return S_OK;
}

// Methods for accessing description

STDMETHODIMP CVWTransactionItemObject::get_Description(BSTR *pbstrDescription)
{
	*pbstrDescription = m_bstrDescription.Copy();
	return S_OK;
}

STDMETHODIMP CVWTransactionItemObject::put_Description(BSTR bstrDescription)
{
	m_bstrDescription = bstrDescription;
	return S_OK;
}

// Methods for accessing graphic 

STDMETHODIMP CVWTransactionItemObject::get_Graphic(BSTR *pbstrGraphic)
{
	*pbstrGraphic = m_bstrGraphic.Copy();
	return S_OK;
}

STDMETHODIMP CVWTransactionItemObject::put_Graphic(BSTR bstrGraphic)
{
	m_bstrGraphic = bstrGraphic;
	return S_OK;
}

// Methods for accessing exemplar type of new object created

STDMETHODIMP CVWTransactionItemObject::get_ExemplarType(BSTR *pbstrExemplarType)
{
	*pbstrExemplarType = m_bstrExemplarType.Copy();
	return S_OK;
}

STDMETHODIMP CVWTransactionItemObject::put_ExemplarType(BSTR bstrExemplarType)
{
	m_bstrExemplarType = bstrExemplarType;
	return S_OK;
}

// Methods for accessing property map that contains additional information on exemplar type
// The property map can be used to set additional properties on exemplars, e.g.
// for an artifact, set IsAccessory = true

STDMETHODIMP CVWTransactionItemObject::get_PropertyValues(IPropertyMap **pPropertyMap)
{
	*pPropertyMap = m_pPropertyValues;
	SAFEADDREF(m_pPropertyValues);
	return S_OK;
}

STDMETHODIMP CVWTransactionItemObject::put_PropertyValues(IPropertyMap **pPropertyMap)
{
	SAFERELEASE(m_pPropertyValues);
	m_pPropertyValues = *pPropertyMap;
	SAFEADDREF(m_pPropertyValues);
	return S_OK;
}

STDMETHODIMP CVWTransactionItemObject::put_World(IWorld* pworld)
{
	HRESULT hr = S_OK;

	CVWTransactionItemObjectBase::put_World(pworld);

	ASSERT(m_pWorld);
	if (m_pWorld == NULL)
		return VWOBJECT_E_INVALIDWORLD;

	hr = CreatePropertyMap(m_pWorld, &m_pPropertyValues);
	if (FAILED(hr) || (!m_pPropertyValues))
		goto CLEAN_UP;

CLEAN_UP:
	return hr;
}


// Methods for marshalling & unmarshalling the object

STDMETHODIMP CVWTransactionItemObject::Marshall(DWORD dwOptions, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = CVWTransactionItemObjectBase::Marshall( dwOptions, pbuffer )))
		return hr;

	ASSERT(pbuffer);

	if (FAILED(hr = pbuffer->put_String(m_bstrName)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pbuffer->put_String(m_bstrDescription)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pbuffer->put_String(m_bstrGraphic)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pbuffer->put_String(m_bstrExemplarType)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pWorld->MarshallObjectProperty(m_pPropertyValues, clsidPropertyMap, dwOptions, pbuffer)))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	
	return hr;
}



STDMETHODIMP CVWTransactionItemObject::MarshallToScript(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut)
{
	HRESULT hr = S_OK;
	long lCount;
	CString strOut;
	CString strVal;
	CString strOne;
	CComBSTR bstrSubContext, bstrOut;
	VARIANT_BOOL bPropAsVal = VARIANT_FALSE;

	if (bstrContext==NULL || pbstrOut==NULL || pbAsVal==NULL)
		return E_INVALIDARG;
	
	*pbAsVal = VARIANT_FALSE;

	strOut.Format(	"\tSet %s = World.Global.CreateTransactionItem(\"%s\", \"%s\", \"%s\", \"%s\")\n",
					(LPCTSTR) CString(bstrContext),
					(LPCTSTR) CString(m_bstrName), (LPCTSTR) CString(m_bstrDescription), (LPCTSTR) CString(m_bstrGraphic), (LPCTSTR) CString(m_bstrExemplarType));

	// Now marshall out the property map of additional property/value pairs, if one exists
	hr = m_pPropertyValues->get_Count(&lCount);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (lCount > 0)
	{
		bstrSubContext = bstrContext;
		bstrSubContext.Append("_t");
		hr = m_pPropertyValues->MarshallToScript(MARSHALL_BYREF, bstrSubContext, &bPropAsVal, &bstrOut.m_str);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if ((BSTR) bstrOut == NULL)
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}

		// Handle value and context cases
		if (bPropAsVal == VARIANT_TRUE)
		{
			strVal = bstrOut;
		}
		else
		{
			strOut += CString(bstrOut);
			strVal = bstrSubContext;

		}

		// Now add property map to transaction item
		strOne.Format( "\t%s.PropertyValues =  %s\n",
						(LPCTSTR) CString(bstrContext),
						(LPCTSTR) strVal );
		strOut += strOne;
	}

	// allocate
	*pbstrOut = strOut.AllocSysString();

ERROR_ENCOUNTERED:
	return hr;
}


STDMETHODIMP CVWTransactionItemObject::UnMarshall(IUnMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	ASSERT(pbuffer);
	SAFERELEASE(m_pPropertyValues);

	if (FAILED(hr = CVWTransactionItemObjectBase::UnMarshall( pbuffer )))
		return hr;
	if (FAILED(hr = pbuffer->get_String(&m_bstrName.m_str)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pbuffer->get_String(&m_bstrDescription.m_str)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pbuffer->get_String(&m_bstrGraphic.m_str)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pbuffer->get_String(&m_bstrExemplarType.m_str)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pWorld->UnMarshallObjectProperty(pbuffer, (IObjectProperty**) &m_pPropertyValues)))
		goto ERROR_ENCOUNTERED;
	
ERROR_ENCOUNTERED:
	
	return hr;
}
