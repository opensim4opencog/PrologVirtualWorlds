// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// File:	D2DRMArr.cpp
// Author:	MM    (MM)

#include "stdafx.h"
#include "D2DRMArr.h"

CDirect2DRMArray::CDirect2DRMArray()
{
	D2DOBJINIT();
}

CDirect2DRMArray::~CDirect2DRMArray()
{
	D2DOBJDESTROY();

	// release all of the object stored in the array
	int cItems = m_dsData.Items();
	for (int i = 0; i < cItems; i++) {
		MMASSERT(m_dsData[i].pObject);
		m_dsData[i].pObject->Release();
	}
}

STDMETHODIMP
CDirect2DRMArray::get_Size(DWORD *pdwSize)
{
	*pdwSize = m_dsData.Items(); 
	return S_OK;
}

STDMETHODIMP
CDirect2DRMArray::GetElement(DWORD nIndex, DWORD *pnType, LPDIRECT2DRMOBJECT *ppD2DRMObject)
{
	if (nIndex < m_dsData.Items() && ppD2DRMObject && pnType) {
		*pnType = m_dsData[nIndex].nType;
		*ppD2DRMObject = m_dsData[nIndex].pObject;
		MMASSERT(*ppD2DRMObject);
		(*ppD2DRMObject)->AddRef();
		return S_OK;
	} else {
		return E_INVALIDARG;
	}
}

STDMETHODIMP
CDirect2DRMArray::Copy(LPD2DRMARRAYPRIVATE *ppArray)
{
	HRESULT hr;
	LPD2DRMARRAYPRIVATE pArray;
	if (FAILED(hr = CoCreateInstance(
		CLSID_CDirect2DRMArray,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ID2DRMArrayPrivate,
		(LPVOID *) &pArray)))
		return hr;

	pArray->Fill((LPVOID) &m_dsData);

	*ppArray = pArray;

	return S_OK;
}
