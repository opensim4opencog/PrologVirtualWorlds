// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// D2DRMVP.cpp : Implementation of Cd2dApp and DLL registration.

#include "stdafx.h"
#include "D2DTrans.h"
#include "DispList.h"
#include "MMImage.h"
#include "D2DRMVP.h"

/////////////////////////////////////////////////////////////////////////////
//

CDirect2DRMViewport::CDirect2DRMViewport()
{
	D2DOBJINIT();

	SetRect(&m_rDimensions, 0, 0, 0, 0);
	m_pParentDevice = NULL;
	m_pCameraFrame = NULL;
	m_bAutoSize = TRUE;

	m_pd2diBackground = NULL;
	m_dwBackgroundColor = 0;
}


CDirect2DRMViewport::~CDirect2DRMViewport()
{
	D2DOBJDESTROY();

	MMRELEASE(m_pCameraFrame);
	MMRELEASE(m_pd2diBackground);
}

STDMETHODIMP 
CDirect2DRMViewport::Init(LPDIRECT2DRMDEVICE pDevice,
						  LPDIRECT2DRMFRAME pCamera,
						  LPRECT prDimensions)
{
	if (m_pParentDevice)
		return E_ALREADYINITIALIZED;

	HRESULT hr = E_INVALIDOBJECT;
	if ((pDevice == NULL) ||
		FAILED(pDevice->QueryInterface(IID_ID2DRMDevicePrivate, (void **) &m_pParentDevice)) ||
		FAILED(hr = put_Camera(pCamera)) ||
		FAILED(hr = m_pParentDevice->AddViewport(this))) {
		// failed to initialize properly -- cleanup
		MMRELEASE(m_pParentDevice);
		MMRELEASE(m_pCameraFrame);
		return hr;
	}

	// the viewport should not AddRef the parent device 
	// ==> that would create a circular reference
	m_pParentDevice->Release();

	// configure the viewport
	return Configure(prDimensions);
}

// Function: Configure
//    This function sets the size of the viewport in screen pixels.  The
//  new viewport must be completely contained within the viewport's device
//  or the viewport does not change size and an error message is returned.
//  It is assumed that viewport autosizing will be used if all of the
//  parameters are 0.
STDMETHODIMP 
CDirect2DRMViewport::Configure(LPRECT prDimensions)
{
	m_bAutoSize = (prDimensions == NULL);

	if (!m_bAutoSize)
		m_rDimensions = *prDimensions;

	return DeviceResized();
}


STDMETHODIMP
CDirect2DRMViewport::SetBackgroundImage(LPDIRECT2DRMIMAGE pd2diBackground)
{
	MMRELEASE(m_pd2diBackground);

	if (m_pd2diBackground = pd2diBackground)
		pd2diBackground->AddRef();

	return S_OK;
}

STDMETHODIMP
CDirect2DRMViewport::SetBackgroundColor(BYTE r, BYTE g, BYTE b)
{
	if (m_pParentDevice) {
		m_dwBackgroundColor = m_pParentDevice->GetColor(r, g, b);
		return S_OK;
	}
	return E_NOTINITIALIZED;
}


STDMETHODIMP 
CDirect2DRMViewport::get_Camera(LPDIRECT2DRMFRAME *ppCamera)
{
	if (m_pCameraFrame == NULL)
		return E_NOTINITIALIZED;

	return m_pCameraFrame->QueryInterface(IID_IDirect2DRMFrame, (void **) ppCamera);
}

STDMETHODIMP 
CDirect2DRMViewport::put_Camera(LPDIRECT2DRMFRAME pCamera)
{
	if (pCamera == NULL)
		return E_INVALIDOBJECT;

	// release the old camera, if it exists
	MMRELEASE(m_pCameraFrame);

	if (FAILED(pCamera->QueryInterface(IID_ID2DRMFramePrivate, (void **) &m_pCameraFrame)))
		return E_INVALIDOBJECT;

	return S_OK;
}

STDMETHODIMP
CDirect2DRMViewport::get_Device(LPDIRECT2DRMDEVICE *ppDevice)
{
	if (m_pParentDevice == NULL)
		return E_NOTINITIALIZED;

	return m_pParentDevice->QueryInterface(IID_IDirect2DRMDevice, (void **) ppDevice);
}


STDMETHODIMP
CDirect2DRMViewport::get_XOffset(DWORD *pnXOffset)
{
	*pnXOffset = m_rDimensions.left; 
	return S_OK;
}

STDMETHODIMP
CDirect2DRMViewport::get_YOffset(DWORD *pnYOffset)
{
	*pnYOffset = m_rDimensions.top;
	return S_OK;
}

STDMETHODIMP
CDirect2DRMViewport::get_Width(DWORD *pnWidth)
{
	*pnWidth = m_rDimensions.right - m_rDimensions.left; 
	return S_OK;
}

STDMETHODIMP
CDirect2DRMViewport::get_Height(DWORD *pnHeight)
{
	*pnHeight = m_rDimensions.bottom - m_rDimensions.top;
	return S_OK;
}

/*
STDMETHODIMP
CDirect2DRMViewport::ClearToColor(DWORD dwFillColor)
{
	return CDirect2DRMViewport::Clear(NULL, dwFillColor, 0);
}

STDMETHODIMP
CDirect2DRMViewport::ClearToImage(LPDIRECT2DRMIMAGE pd2diBackground, DWORD dwFlags)
{
	return CDirect2DRMViewport::Clear(pd2diBackground, 0, dwFlags);
}
*/

void
_RECTToD2DRect(const RECT &r, LPD2DRECT pr2)
{
	pr2->left = (float) r.left;
	pr2->top = (float) r.top;
	pr2->right = (float) r.right;
	pr2->bottom = (float) r.bottom;
}


// Function: Clear
//    Clear the background.  If the background image is NULL or partially
//  transparent, the background is first cleared to the FillColor, which is
//  a palette index in paletted modes, or packed RGB triple in RGB modes.  
//  The default is clear to black.
STDMETHODIMP
CDirect2DRMViewport::Clear(DWORD dwFlags)
{
	HRESULT hr = S_OK;  // null clears are okay
	LPDIRECTDRAWSURFACE pddsTarget;

	if ((m_pParentDevice == NULL) ||
		((pddsTarget = m_pParentDevice->GetBackBuffer()) == NULL))
		return E_NOTINITIALIZED;
	
	BOOL bTransparent;

	// if the background color is visible, clear to it
	if ((m_pd2diBackground == NULL) || (!(flagBLT_IGNORETRANSPARENCY & dwFlags) &&
		(m_pd2diBackground->get_Transparent(&bTransparent), bTransparent))) {

		if (FAILED(hr = ::ClearToColor(m_dwBackgroundColor, &m_rDimensions, pddsTarget)))
			return hr;
	}
		
	// if a background image exists, use it
	if (m_pd2diBackground) {
		D2DRect r2;
		_RECTToD2DRect(m_rDimensions, &r2);
		if (FAILED(hr = m_pd2diBackground->BltToSurface(NULL, pddsTarget, &r2, dwFlags)))
			return hr;
	}

	return hr;
}


STDMETHODIMP
CDirect2DRMViewport::Render(LPDIRECT2DRMFRAME pScene, DWORD dwFlags)
{
	HRESULT hr = S_OK;
	LPDIRECTDRAWSURFACE pddsTarget = NULL;

	// make sure the viewport is initialized properly
	if ((m_pParentDevice == NULL) ||
		((pddsTarget = m_pParentDevice->GetBackBuffer()) == NULL))
		return E_NOTINITIALIZED;

	// get the private interface for the scene frame
	LPD2DRMFRAMEPRIVATE pScenePrivate = NULL;
	if ((pScene == NULL) ||
		FAILED(pScene->QueryInterface(IID_ID2DRMFramePrivate, (void **) &pScenePrivate)))
		return E_INVALIDOBJECT;

	// create a transformation in the camera's frame
	D2DTransform t2dTemp;
	if (m_pCameraFrame)
		m_pCameraFrame->GetInverseTransform(&t2dTemp);
	else
		t2dTemp.Init();

	// render the frame hierarchy to the given surface
	D2DRect r2;
	_RECTToD2DRect(m_rDimensions, &r2);
	hr = pScenePrivate->RenderToSurface(pddsTarget, &r2, &t2dTemp, dwFlags);

	pScenePrivate->Release();

	return hr;
}

// Function: Pick
//    This function returns a list of the visuals that contain the pick point.
//  Viewports are considered to have the top and left edges inclusive, which
//  also applies to the Pick operation.  When executing the Pick on the frame,
//  notice that we use the center of the pixel as the pick point.
STDMETHODIMP
CDirect2DRMViewport::Pick(LPDIRECT2DRMFRAME pScene, DWORD nX, DWORD nY, 
						  LPDIRECT2DRMARRAY *prgPicks)
{
	HRESULT hr;

	// get the private interface for the scene frame
	LPD2DRMFRAMEPRIVATE pScenePrivate = NULL;
	LPD2DRMARRAYPRIVATE pAncestory = NULL;
	LPD2DRMARRAYPRIVATE pPicks = NULL;
	D2DTransform t2dCam;
	D2DPoint pt2Pick;
	DWORD nItems = 0;

	if ((pScene == NULL) || (prgPicks == NULL) ||
		FAILED(pScene->QueryInterface(IID_ID2DRMFramePrivate, (void **) &pScenePrivate)))
		return E_INVALIDOBJECT;

	if (IsInside(nX, nY, m_rDimensions)) {

			// CoCreate a private Array for the ancestory
		if (FAILED(hr = CoCreateInstance(
				CLSID_CDirect2DRMArray,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_ID2DRMArrayPrivate,
				(LPVOID *) &pAncestory)) ||
			// CoCreate a private array to store picks
			FAILED(hr = CoCreateInstance(
				CLSID_CDirect2DRMArray,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_ID2DRMArrayPrivate,
				(LPVOID *) &pPicks)))
			goto ePick;

		// use the center of the pixel as the pick in floating point
		pt2Pick.x = ((float) nX) + 0.5f;
		pt2Pick.y = ((float) nY) + 0.5f;

		// transform into the camera's frame
		D2DPoint pt2New;
		if (m_pCameraFrame)
			m_pCameraFrame->TransformPt(&pt2Pick, &pt2New);
		else
			pt2New = pt2Pick;

		if (FAILED(hr = pScenePrivate->PickFrame(&pt2New, pAncestory, pPicks)))
			goto ePick;

		// if there was a pick, QI for the public interface to the array
		pPicks->get_Size(&nItems);
		if (nItems) {
			hr = pPicks->QueryInterface(IID_IDirect2DRMArray, (void **) prgPicks);
		} else {
			*prgPicks = NULL;
		}
	}

ePick:
	MMRELEASE(pScenePrivate);
	MMRELEASE(pAncestory);
	MMRELEASE(pPicks);

	return hr;
}

STDMETHODIMP
CDirect2DRMViewport::MovePicked(float dX, float dY, LPDIRECT2DRMPICK pPick)
{
	if (pPick == NULL)
		return E_INVALIDOBJECT;

	// transform the viewport vector into a world vector
	// REVIEW: for now, assume a null camera ==> identity matrix
	D2DVector vWorld;
	vWorld.x = dX;
	vWorld.y = dY;

	if (m_pCameraFrame)
		m_pCameraFrame->Transform(&vWorld, &vWorld);

	// move the visual w.r.t. world vector
	return pPick->MoveVisual(&vWorld);
}

STDMETHODIMP
CDirect2DRMViewport::UnsetParentDevice()
{
	m_pParentDevice = NULL;
	return S_OK;
}

STDMETHODIMP
CDirect2DRMViewport::DeviceResized()
{
	if (m_pParentDevice == NULL)
		return E_NOTINITIALIZED;

	// get the current device dimensions
	DWORD nWidth, nHeight;
	m_pParentDevice->get_Width(&nWidth);
	m_pParentDevice->get_Height(&nHeight);

	if (m_bAutoSize == FALSE) {
		RECT rDevice;
		SetRect(&rDevice, 0, 0, nWidth, nHeight);
		// clip the current viewport dimensions against the device
		if (ClipRect(rDevice, &m_rDimensions) == FALSE)
			m_bAutoSize = TRUE;
	}
	if (m_bAutoSize) {
		// make the viewport fill the device
		SetRect(&m_rDimensions, 0, 0, nWidth, nHeight);
	}
	return S_OK;
}
