// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWFrameO.cpp : Implementation of CVWFrameApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include <VWAData.h>
#include "VWADataO.h"

IMPLEMENT_OBJECTPROPERTY(CVWAccessoryDataObject)

/////////////////////////////////////////////////////////////////////////////
//

CVWAccessoryDataObject::CVWAccessoryDataObject()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVWAccessoryDataObject::CVWAccessoryDataObject, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif
}

CVWAccessoryDataObject::~CVWAccessoryDataObject()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVWAccessoryDataObject::~CVWAccessoryDataObject, this 0x%0x\n", this);
#endif
}

STDMETHODIMP CVWAccessoryDataObject::get_AccessoryGraphic(BSTR *pbstrAccessoryGraphic)
{
	*pbstrAccessoryGraphic = m_bstrAccessoryGraphic.Copy();
	return S_OK;
}

STDMETHODIMP CVWAccessoryDataObject::put_AccessoryGraphic(BSTR bstrAccessoryGraphic)
{
	m_bstrAccessoryGraphic = bstrAccessoryGraphic;
	return S_OK;
}

STDMETHODIMP CVWAccessoryDataObject::GetPos(float *pfPosX, float *pfPosY)
{
	*pfPosX = m_fPosX;
	*pfPosY = m_fPosY;
	return S_OK;
}

STDMETHODIMP CVWAccessoryDataObject::PutPos(float fPosX, float fPosY)
{
	m_fPosX = fPosX;
	m_fPosY = fPosY;
	return S_OK;
}

STDMETHODIMP CVWAccessoryDataObject::Marshall(DWORD dwOptions, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = CVWAccessoryDataObjectBase::Marshall( dwOptions, pbuffer )))
		return hr;

	ASSERT(pbuffer);

	if (FAILED(hr = pbuffer->put_String(m_bstrAccessoryGraphic)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pbuffer->put_Float(m_fPosX)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pbuffer->put_Float(m_fPosY)))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	
	return hr;
}

STDMETHODIMP CVWAccessoryDataObject::UnMarshall(IUnMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = CVWAccessoryDataObjectBase::UnMarshall( pbuffer )))
		return hr;

	ASSERT(pbuffer);

	if (FAILED(hr = pbuffer->get_String(&m_bstrAccessoryGraphic.m_str)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pbuffer->get_Float(&m_fPosX)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pbuffer->get_Float(&m_fPosY)))
		goto ERROR_ENCOUNTERED;
	
ERROR_ENCOUNTERED:
	
	return hr;
}
