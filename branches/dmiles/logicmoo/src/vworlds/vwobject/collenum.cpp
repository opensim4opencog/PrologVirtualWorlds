// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include <vwobject.h>
#include <vwprop.h>
#include "collenum.h"

//////////////////////////////////////////////////////////////////
// Constuctor/destructor

CEnumCollection::CEnumCollection() :
	m_pcoll(NULL),
	m_ielt(0),
	m_celt(0),
	m_dwRef(0)
{
}

CEnumCollection::~CEnumCollection()
{
	SAFERELEASE(m_pcoll);
}

HRESULT CEnumCollection::Initialize(IPropertyCollection* pcoll)
{
	HRESULT hr;

	if (pcoll == NULL)
		return E_INVALIDARG;

	m_pcoll = pcoll;
	SAFEADDREF(m_pcoll);

	m_ielt = 0;

	// get the count
	hr = pcoll->get_Count((long *) &m_celt);
	
	return hr;
}

STDMETHODIMP CEnumCollection::QueryInterface(REFIID iid, void** ppunk)
{
    if (iid == IID_IUnknown || iid == IID_IEnumVARIANT)
    {
        *ppunk = this;
        AddRef();
        return NOERROR;
    }
    return ResultFromScode(E_NOINTERFACE);
}

ULONG CEnumCollection::AddRef(void)
{
    return ++m_dwRef;
}

ULONG CEnumCollection::Release(void)
{
	if (--m_dwRef == 0)
	{
		delete this;
		return 0;
	}

	return m_dwRef;
}

STDMETHODIMP CEnumCollection::Next(	unsigned long celt, 
								VARIANT* rgvar, 
								unsigned long* pceltFetched)
{
	HRESULT hr = S_OK;

	// initialize
	if (pceltFetched)
		*pceltFetched = 0;

	if (rgvar == NULL)
		return E_INVALIDARG;

	// get the count - it might have changed since the last call
	hr = m_pcoll->get_Count((long *) &m_celt);
	if (FAILED(hr))
		return hr;

	// fetch data
	for (unsigned long i=0; i<celt; i++)
	{
		// clear the slot
		::VariantClear(&rgvar[i]);

		// see if beyond the end
		if (m_ielt >= m_celt)
			return ResultFromScode(S_FALSE);

		// get the next element
		hr = m_pcoll->get_PropertyAtIndex(m_ielt, &rgvar[i]);
		if (FAILED(hr))
			break;

		// bump the index count 
		m_ielt++;
		if (pceltFetched)
			(*pceltFetched)++;
	}

	return hr;
}

STDMETHODIMP CEnumCollection::Skip(unsigned long celt)
{
	HRESULT hr = S_OK;

	// get the count - it might have changed since the last call
	hr = m_pcoll->get_Count((long *) &m_celt);
	if (FAILED(hr))
		return hr;

	if ((m_ielt+celt) >= m_celt)
		return S_FALSE;

	m_ielt += celt;
	return S_OK;
}

STDMETHODIMP CEnumCollection::Reset()
{
	m_ielt = 0;

	// REVIEW: get m_celt here?

	return S_OK;
}

STDMETHODIMP CEnumCollection::Clone(IEnumVARIANT** ppenum)
{
	CEnumCollection *pec;
	HRESULT hr=S_OK;

	if (ppenum == NULL)
		return E_POINTER;

	// create a new one of us
	pec = new CEnumCollection;
	if (pec==NULL)
		return E_OUTOFMEMORY;
	pec->AddRef();

	// initialize it
	hr = pec->Initialize(m_pcoll);
	if (FAILED(hr))
		return hr;

	// set the cursor
	pec->m_ielt = m_ielt;

	// return it
	*ppenum = (IEnumVARIANT *) pec;

	return S_OK;
}


