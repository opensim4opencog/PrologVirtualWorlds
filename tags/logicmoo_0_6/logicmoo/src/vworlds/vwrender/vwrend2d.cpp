// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWRend2D.cpp : Implementation of CVWRenderApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
// d2d.h needs to go above vwrender.h
#include <d2d.h>
#include <vwrender.h>
#include "VWRend2D.h"
#include "D2DUtils.h"

/////////////////////////////////////////////////////////////////////////////
//

CPtrList CVWRender2D::m_list2DRenderers;

CVWRender2D::CVWRender2D()
{
	m_hWnd = 0;
	m_pd2drm = NULL;
	m_pd2drmDevice = NULL;
	m_pd2drmWinDevice = NULL;
	m_pd2drmViewport = NULL;

	m_pfScene = NULL;
//	m_pd2drmiBackground = NULL;
//	m_dwBackgroundColor = 0;
	m_pfCamera = NULL;

	m_pfEmptyScene = NULL;
	m_pfEmptySceneCamera = NULL;
	m_bMoveScene = TRUE;

	// add this renderer to the list of 2D renderers
	m_list2DRenderers.AddTail(this);
}

CVWRender2D::~CVWRender2D()
{
	POSITION			pos;
	CVWRender2D			*p2DRenderer;
	IDirect2DRMFrame	*p2DScene;

	// Remove this renderer to the list of 2D renderers.
	pos = m_list2DRenderers.Find(this);
	ASSERT(pos);
	m_list2DRenderers.RemoveAt(pos);

	if (m_bMoveScene)
	{
		// OK, lets see if this scene is (still) being rendered by another renderer.
		// If it is then that other renderer will have to take over moving the scene.
		pos = m_list2DRenderers.GetHeadPosition();
		while (pos)
		{
			p2DRenderer = (CVWRender2D*)m_list2DRenderers.GetNext(pos);
			p2DRenderer->get_Scene(&p2DScene);
			if (p2DScene == m_pfScene)
			{
				p2DRenderer->m_bMoveScene = TRUE;
			}
			SAFERELEASE(p2DScene);
		}
	}

	ReleaseAll();
}

HRESULT 
CVWRender2D::ReleaseAll()
{
	MMRELEASE(m_pfEmptyScene);
	MMRELEASE(m_pfEmptySceneCamera);
	MMRELEASE(m_pfCamera);
	MMRELEASE(m_pfScene);
	MMRELEASE(m_pd2drmDevice);
	MMRELEASE(m_pd2drmWinDevice);
	MMRELEASE(m_pd2drmViewport);
	MMRELEASE(m_pd2drm);
	return S_OK;
}

/*
HRESULT
CVWRender2D::ReleaseSizeBasedObjects()
{
	MMRELEASE(m_pd2drmDevice);
	MMRELEASE(m_pd2drmWinDevice);
	MMRELEASE(m_pd2drmViewport);

	return S_OK;
}
*/

STDMETHODIMP 
CVWRender2D::HandleActivate(WPARAM wParam)
{
	if (m_pd2drmWinDevice) {
//		return m_pd2drmWinDevice->HandleActivate(wParam);
	}
	return S_OK;
}

STDMETHODIMP 
CVWRender2D::HandleSize(LPDIRECTDRAWSURFACE pddsBackBuffer)
{
	HRESULT hr;

	if (m_pd2drmDevice == NULL) {
		if (FAILED(hr = m_pd2drm->CreateDeviceFromSurface(NULL, pddsBackBuffer, &m_pd2drmDevice)) ||
			FAILED(hr = m_pd2drm->CreateViewport(m_pd2drmDevice, m_pfCamera, NULL, &m_pd2drmViewport))) {
			return hr;
		}
	} else {
		MMASSERT(m_pd2drmViewport);
		if (FAILED(hr = m_pd2drmDevice->InitFromSurface(pddsBackBuffer)))
			return hr;
	}

//	IDirect2DRMImage *piBackground;
//	m_pd2drm->CreateImageFromTextExt(CComBSTR("Background Scene"), 20, TRUE, &piBackground );
//	m_pd2drm->CreateImageFromImageFile(CComBSTR("c:\\cache\\trever24.gif"), flagCREATE_IGNOREANIMATION, &piBackground);
//	m_pd2drmViewport->SetBackgroundImage(piBackground);
//	SAFERELEASE(piBackground);

	return S_OK;
}

STDMETHODIMP 
CVWRender2D::ReleaseDevice()
{
	MMRELEASE(m_pd2drmDevice);
	MMRELEASE(m_pd2drmWinDevice);
	MMRELEASE(m_pd2drmViewport);
	return S_OK;
}

STDMETHODIMP 
CVWRender2D::HandlePaint(HDC hDC)
{
	if (m_pd2drmWinDevice) {
//		return m_pd2drmWinDevice->HandlePaint(hDC);
	}
	return S_OK;
}

/*
HRESULT
CVWRender2D::InitSizeBasedObjects(LPDIRECTDRAWSURFACE pddsBackBuffer)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = m_pd2drm->CreateDeviceFromSurface(NULL, pddsBackBuffer, &m_pd2drmDevice)) ||
		FAILED(hr = m_pd2drm->CreateViewport(m_pd2drmDevice, m_pfCamera, 0, 0, 0, 0, &m_pd2drmViewport))) {
		ASSERT(FALSE);
	}

	return hr;
}
*/

HRESULT
CVWRender2D::InitScene()
{
	ASSERT(m_pfEmptyScene == NULL);
	IDirect2DRMFrame	*pFrame = NULL;
	IDirect2DRMImage	*pImage = NULL;

	HRESULT hr = S_OK;

	// create the empty scene
	m_pd2drm->CreateFrame(NULL, &m_pfEmptyScene);
	// make the camera anchored in world space
	m_pd2drm->CreateFrame(NULL, &m_pfEmptySceneCamera);
//	m_pd2drm->CreateFrame(m_pfScene, &m_pfCamera);

	m_pfScene = m_pfEmptyScene;
	m_pfScene->AddRef();

	m_pfCamera = m_pfEmptySceneCamera;
	m_pfCamera->AddRef();

//#define __ADD_SOME_DEBUGGING_TEXT__
#ifdef __ADD_SOME_DEBUGGING_TEXT__
	hr = m_pd2drm->CreateFrame(m_pfEmptyScene, &pFrame);
	if ( FAILED( hr ))
		goto ERROR_ENCOUNTERED;

	hr = pFrame->SetTranslation(5, 5);
	if ( FAILED( hr ))
		goto ERROR_ENCOUNTERED;
	
	hr = m_pd2drm->CreateImageFromTextExt(CComBSTR("Empty Scene"), 20, TRUE, &pImage );
	if ( FAILED( hr ))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pFrame->AddVisual(pImage)))
		goto ERROR_ENCOUNTERED;

	SAFERELEASE( pFrame );
	SAFERELEASE( pImage );

	hr = m_pd2drm->CreateFrame(m_pfEmptyScene, &pFrame);
	if ( FAILED( hr ))
		goto ERROR_ENCOUNTERED;

	hr = pFrame->SetTranslation(0, 10);
	if ( FAILED( hr ))
		goto ERROR_ENCOUNTERED;
	
	hr = m_pd2drm->CreateImageFromImageFileExt(CComBSTR("c:\\temp\\thintree.gif"), 0, &pImage );
	if ( FAILED( hr ))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pFrame->AddVisual(pImage)))
		goto ERROR_ENCOUNTERED;

	SAFERELEASE( pImage );
	SAFERELEASE( pFrame );

ERROR_ENCOUNTERED:
#endif

	// each renderer owns its own empty scene
	m_bMoveScene = TRUE;

	
	SAFERELEASE( pImage );
	SAFERELEASE( pFrame );

	return hr;
}

STDMETHODIMP 
CVWRender2D::Init(HWND hWnd, LPDIRECT2DRM pd2drm)
{
//	if (!(pddsBackBuffer && pd2drm && hWnd))
//		return E_INVALIDOBJECT;
	if (!(pd2drm && hWnd))
		return E_INVALIDOBJECT;
	
	if (m_pfEmptyScene)
		return E_ALREADYINITIALIZED;
	
	HRESULT hr;
	
	pd2drm->AddRef();
	m_pd2drm = pd2drm;
	
	m_hWnd = hWnd;
	
	if (FAILED(hr = InitScene())) {
		return hr;
	}

	// N.B.: wait for first OnSize message to initialize the size-based objects

	return S_OK;
}


STDMETHODIMP
CVWRender2D::SetFastMode(VARIANT_BOOL bFastMode)
{
	if (m_pd2drmViewport == NULL)
		return E_NOTINITIALIZED;

	RECT rDevice;
	rDevice.left = rDevice.top = 0;
	m_pd2drmDevice->get_Width((unsigned long*)&rDevice.right);
	m_pd2drmDevice->get_Height((unsigned long*)&rDevice.bottom);

	if (bFastMode) {
		// set viewport to half window size
		rDevice.right >>= 1;
		rDevice.bottom >>= 1;
		m_pfCamera->AddScale(2.0, 2.0);
		return m_pd2drmViewport->Configure(&rDevice);
	} else {
		// set viewport to autosize with the device
		m_pfCamera->AddScale(0.5, 0.5);
		return m_pd2drmViewport->Configure(NULL);
	}
}


STDMETHODIMP 
CVWRender2D::get_Direct2DRM(LPDIRECT2DRM *ppd2drm)
{
	if (*ppd2drm = m_pd2drm) {
		m_pd2drm->AddRef();
		return S_OK;
	}
	return E_NOTINITIALIZED;
}

STDMETHODIMP
CVWRender2D::Clear()
{
	if (m_pd2drmViewport)
		return m_pd2drmViewport->Clear(flagBLT_USEDYNAMICMAPPING);
	return E_NOTINITIALIZED;
}


STDMETHODIMP 
CVWRender2D::Render()
{
	HRESULT hr = S_OK;

	if (!m_pd2drmViewport)
		return E_NOTINITIALIZED;

	if (m_pfScene) {
		if (m_bMoveScene) {
			// REVIEW: we still need to move the scene at the right time
//			m_pfScene->Move(1.f);
		}
		return m_pd2drmViewport->Render(m_pfScene, flagBLT_USEDYNAMICMAPPING);
	}
	return hr;
}

STDMETHODIMP 
CVWRender2D::HitTest(VARIANT varHitParams, VARIANT_BOOL vbReturnAllPicked, VARIANT *pvarHitResult)
{
	HRESULT hr = S_OK;

	return hr;
}


STDMETHODIMP 
CVWRender2D::get_Scene (LPDIRECT2DRMFRAME *ppd2drmf)
{
	if (*ppd2drmf = m_pfScene) {
		m_pfScene->AddRef();
		return S_OK;
	}
	return E_NOTINITIALIZED;
}

STDMETHODIMP 
CVWRender2D::put_Scene (LPDIRECT2DRMFRAME pd2drmf)
{
	HRESULT hr = S_OK;
	SAFERELEASE( m_pfScene );
	m_pfScene = pd2drmf;
	if ( m_pfScene == NULL)
	{
		m_pfScene = m_pfEmptyScene;
	}
	SAFEADDREF( m_pfScene );
	return hr;
}

STDMETHODIMP 
CVWRender2D::get_Camera(LPDIRECT2DRMFRAME *ppd2drm)
{
	if (*ppd2drm = m_pfCamera) {
		m_pfCamera->AddRef();
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP
CVWRender2D::SetBackgroundColor(BYTE r, BYTE g, BYTE b)
{
	if (m_pd2drmViewport) {
		return m_pd2drmViewport->SetBackgroundColor(r, g, b);
	}
	return E_NOTINITIALIZED;
}

STDMETHODIMP 
CVWRender2D::put_BackgroundImage(LPDIRECT2DRMIMAGE piBackground)
{
	if (m_pd2drmViewport) {
		return m_pd2drmViewport->SetBackgroundImage(piBackground);
	}
	return E_NOTINITIALIZED;
}
