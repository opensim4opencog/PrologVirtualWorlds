// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// D2DRMDev.cpp : Implementation of Cd2dApp and DLL registration.

#include "stdafx.h"
#include "D2DRMDev.h"

/////////////////////////////////////////////////////////////////////////////
//

CDirect2DRMDevice::CDirect2DRMDevice()
{
	D2DOBJINIT();
	m_pddsBackBuffer = NULL;
	INIT_DXSTRUCT(m_ddsd);
	m_nBufferCount = 1;
}

CDirect2DRMDevice::~CDirect2DRMDevice()
{
	D2DOBJDESTROY();

	for(DWORD i = 0, cLimit = m_dsViewports.Items(); i < cLimit; i++) {
		// make sure there are no dangling pointers to this device
		m_dsViewports[i]->UnsetParentDevice();
		MMRELEASE(m_dsViewports[i]);
	}

	MMRELEASE(m_pddsBackBuffer);
}

// initialization
STDMETHODIMP 
CDirect2DRMDevice::InitFromSurface(LPDIRECTDRAWSURFACE pdds)
{
	HRESULT hr;
	if (pdds == NULL)
		return E_NULLPOINTER;

//	if (m_pddsBackBuffer)
//		return E_ALREADYINITIALIZED;

	// release the old surface
	MMRELEASE(m_pddsBackBuffer);

	pdds->AddRef();
	m_pddsBackBuffer = pdds;

	if (FAILED(hr = pdds->GetSurfaceDesc(&m_ddsd)) ||
		FAILED(hr = m_pixi.Init(m_ddsd.ddpfPixelFormat)))
		return hr;

	// notify the viewports that the device has been resized
	for(DWORD i = 0, cLimit = m_dsViewports.Items(); i < cLimit; i++) {
		if (FAILED(hr = m_dsViewports[i]->DeviceResized()))
			return hr;
	}
	return S_OK;
}

// buffer counts
STDMETHODIMP 
CDirect2DRMDevice::get_BufferCount(DWORD *pnCount)
{
	if (pnCount == NULL)
		return E_NULLPOINTER;
	*pnCount = m_nBufferCount;
	return S_OK;
}

STDMETHODIMP 
CDirect2DRMDevice::put_BufferCount(DWORD nCount)
{
	m_nBufferCount = nCount;
	return S_OK;
}


// Notifications 
STDMETHODIMP 
CDirect2DRMDevice::Update(void)
{
	return E_NOTIMPL;
}

STDMETHODIMP 
CDirect2DRMDevice::AddUpdateCallback(void)
{
	return E_NOTIMPL;
}

STDMETHODIMP 
CDirect2DRMDevice::DeleteUpdateCallback(void)
{
	return E_NOTIMPL;
}


// Rendering Quality 
STDMETHODIMP 
CDirect2DRMDevice::get_Sampling(WORD *pwSampling)
{
	if (pwSampling == NULL)
		return E_NULLPOINTER;

	return E_NOTIMPL;
}

STDMETHODIMP 
CDirect2DRMDevice::put_Sampling(WORD wSampling)
{
	return E_NOTIMPL;
}

STDMETHODIMP 
CDirect2DRMDevice::get_Dithering(WORD *pwDither)
{
	if (pwDither == NULL)
		return E_NULLPOINTER;

	return E_NOTIMPL;
}

STDMETHODIMP 
CDirect2DRMDevice::put_Dithering(WORD wDither)
{
	return E_NOTIMPL;
}


// Miscellaneous Access
STDMETHODIMP 
CDirect2DRMDevice::get_ColorModel(WORD *pwColorModel)
{
	if (pwColorModel == NULL)
		return E_NULLPOINTER;

	return E_NOTIMPL;
}


STDMETHODIMP 
CDirect2DRMDevice::get_Viewports(LPDIRECT2DRMARRAY *ppViewports)
{
	if (ppViewports == NULL)
		return E_NULLPOINTER;

	return E_NOTIMPL;
}

STDMETHODIMP
CDirect2DRMDevice::GetViewport(DWORD iViewport, LPDIRECT2DRMVIEWPORT *ppViewport)
{
	if (ppViewport == NULL)
		return E_NULLPOINTER;

	if (iViewport < m_dsViewports.Items()) {
		LPDIRECT2DRMVIEWPORT pViewport;
		pViewport = (LPDIRECT2DRMVIEWPORT) m_dsViewports[iViewport];
		pViewport->AddRef();
		*ppViewport = pViewport;
		return S_OK;
	}
	return E_INVALIDARG;
}


STDMETHODIMP 
CDirect2DRMDevice::get_Width(DWORD *pnWidth)
{
	if (pnWidth == NULL)
		return E_NULLPOINTER;

	*pnWidth = m_ddsd.dwWidth;
	return S_OK;
}

STDMETHODIMP 
CDirect2DRMDevice::get_Height(DWORD *pnHeight)
{
	if (pnHeight == NULL)
		return E_NULLPOINTER;

	*pnHeight = m_ddsd.dwHeight;
	return S_OK;
}

//
// ID2DRMDevicePrivate Interface
//
LPDIRECTDRAWSURFACE
CDirect2DRMDevice::GetBackBuffer(void)
{
	return m_pddsBackBuffer;
}

STDMETHODIMP
CDirect2DRMDevice::AddViewport(LPDIRECT2DRMVIEWPORT pd2drmViewport)
{
	LPD2DRMVIEWPORTPRIVATE pPrivateViewport;
	HRESULT hr;

	if ((pd2drmViewport == NULL) ||
		FAILED(hr = pd2drmViewport->QueryInterface(IID_ID2DRMViewportPrivate, (void **) &pPrivateViewport)))
		return E_INVALIDOBJECT;

	m_dsViewports.Ins(pPrivateViewport);
	return S_OK;
}


DWORD
CDirect2DRMDevice::GetDepth(void)
{
	return m_ddsd.ddpfPixelFormat.dwRGBBitCount;
}

DWORD
CDirect2DRMDevice::GetColor(BYTE r, BYTE g, BYTE b)
{
	HRESULT hr;
	if (m_pddsBackBuffer == NULL)
		return E_NOTINITIALIZED;

	// REVIEW: this function is heavy weight when the device is palettized
	DWORD nBPP = m_ddsd.ddpfPixelFormat.dwRGBBitCount;
	if (nBPP <= 8) {
		// palettized
		PALETTEENTRY rgpe[nMAXPALETTEENTRIES];
		LPDIRECTDRAWPALETTE pddp = NULL;
		if (FAILED(hr = m_pddsBackBuffer->GetPalette(&pddp)) ||
			FAILED(hr = pddp->GetEntries(0, 0, nMAXPALETTEENTRIES, rgpe))) {
			MMRELEASE(pddp);
			return hr;
		}
		MMRELEASE(pddp);
		return SimpleFindClosestIndex(rgpe, 1 << nBPP, r, g, b);
	} else {
		// RGB
		return m_pixi.Pack(r, g, b);
	}
	return 0;
}
