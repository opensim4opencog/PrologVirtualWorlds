// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWRendRt.cpp : Implementation of CVWRenderApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
// d3drmwin.h and d2d.h need to go above vwrender.h
#include <d3drmwin.h>
#include <d2d.h>
#include <vwrender.h>
#include "VWRendRt.h"
#include "VWRend3D.h"
#include "D2DUtils.h"
#include <graphtls.h>
#include <vwsevent.h>

#ifndef MMRELEASE
#define MMRELEASE(_p) do { if ((_p) != NULL) {(_p)->Release(); (_p) = NULL; }} while (0)
#endif

// Statics.
DWORD dw2DClearTime = 0;
DWORD dw3DRenderTime = 0;
DWORD dw2DRenderTime = 0;
DWORD dwBltTime = 0;	

char*
D3DRMErrorToString(HRESULT error);
HRESULT DDSetColorKey(IDirectDrawSurface *pdds, COLORREF rgb);
DWORD DDColorMatch(IDirectDrawSurface *pdds, COLORREF rgb);

/////////////////////////////////////////////////////////////////////////////
//

CVWRenderRoot::CVWRenderRoot()
{
	m_pVWRender3D = NULL;
	m_pVWRender2D = NULL;
	m_pvwfScene = NULL;
	m_pvwfCamera = NULL;
	m_pd3drmfCamera = NULL;
	m_pd2drmfCamera = NULL;
	m_pPalette = NULL;
	m_pddsFrontBuffer = NULL;
	m_pddsBackBuffer = NULL;
	m_hWnd = NULL;
	m_pDD = NULL;
	m_pD2DRM = NULL;
	m_bFastMode = VARIANT_FALSE;
	m_bFastModeEnabled = VARIANT_TRUE;
	m_bUseBackBuffer = VARIANT_FALSE;
	m_bForceBackBuffer = VARIANT_FALSE;
	m_lpDDClipper = NULL;
	m_bOverlayEnabled = VARIANT_TRUE;
	m_bFPSEnabled = VARIANT_FALSE;
	m_iCurrentCX = 0; m_iCurrentCY = 0;
	m_bUseHWAcceleration = VARIANT_TRUE;
	m_d3dColorModel = 0;
	m_crBackgroundColor = RGB(0,0,0);
	InitializeCriticalSection(&m_CS);
}

CVWRenderRoot::~CVWRenderRoot()
{
	ReleaseAll();
	DeleteCriticalSection(&m_CS);
}

HRESULT
CVWRenderRoot::ReleaseAll()
{
	MMRELEASE(m_pVWRender3D);
	MMRELEASE(m_pVWRender2D);
	MMRELEASE(m_pvwfCamera);
	MMRELEASE(m_pd3drmfCamera);
	MMRELEASE(m_pd2drmfCamera);
	MMRELEASE(m_pvwfScene);
	MMRELEASE(m_pPalette);
	MMRELEASE(m_pddsFrontBuffer);
	MMRELEASE(m_lpDDClipper);
	MMRELEASE(m_pddsBackBuffer);
	MMRELEASE(m_pDD);
	MMRELEASE(m_pD2DRM);
	return S_OK;
}

STDMETHODIMP CVWRenderRoot::HandleActivate(WPARAM wParam)
{
	HRESULT hRes = S_OK;

	if (!m_pVWRender3D)
		return E_NOTINITIALIZED;

	EnterCriticalSection(&m_CS);

	if (FAILED(hRes = m_pVWRender2D->HandleActivate(wParam)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hRes = m_pVWRender3D->HandleActivate(wParam)))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	LeaveCriticalSection(&m_CS);
	return hRes;
}

// Function: HandlePaint
//    HandlePaint should get called when the OCX container gets an OnDraw call.
//  This code should NOT be wrapped in a critical section because it can be 
//  triggered by the D3DViewport->Clear call, which is already in a critical 
//  section.  The Viewport clear internally makes a SendMessage call and blocks
//  until the Paint message is processed, effectively deadlocking the renderer.
//    In any case, this function will only interact badly in a multithreaded
//  situation when a HandleSize occurs at the same time. This shouldn't happen 
//  because the HandlePaint can only get called from the thread handling the 
//  message pump, on an OnDraw call.  In other words, ONLY CALL THIS FUNCTION
//  INDIRECTLY THRU THE MESSAGE PUMP.
STDMETHODIMP CVWRenderRoot::HandlePaint(HDC hDC)
{
	CRect	rectWnd;
	HRESULT hr = S_OK;

	if (!m_pVWRender3D)
		return E_NOTINITIALIZED;

	// If device is smaller than window, better fill in the gaps.
	GetClientRect(m_hWnd, &rectWnd);
	if (m_iCurrentCX < rectWnd.Width()  || m_iCurrentCY < rectWnd.Height())
		CDC::FromHandle(hDC)->FillSolidRect(rectWnd,m_crBackgroundColor);

	if (m_bUseBackBuffer || !m_bUseHWAcceleration)
	{
		if (FAILED(hr = m_pVWRender2D->HandlePaint(hDC)))
			goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = m_pVWRender3D->HandlePaint(hDC)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = Update()))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:

	return hr;
}

STDMETHODIMP CVWRenderRoot::Init(HWND hWnd, LPDIRECTDRAW pdd, LPDIRECT3DRM pd3drm, LPDIRECT2DRM pd2drm)
{
	EnterCriticalSection(&m_CS);

	CLSID				clsidVWRender3D;
	CLSID				clsidVWRender2D;
	DDPIXELFORMAT		ddpf;
	HRESULT hr = S_OK;

	m_pDD = pdd;
	m_pDD->AddRef();
	m_pD2DRM = pd2drm;
	m_pD2DRM->AddRef();

	m_hWnd = hWnd;

	//
	// create front buffer (primary surface shared with GDI)
	//
	if (FAILED(hr = CreateFrontBuffer(pdd, pd2drm, &m_pddsFrontBuffer)))
		goto e_Init;
	memset(&ddpf,0,sizeof(DDPIXELFORMAT));
	ddpf.dwSize = sizeof(DDPIXELFORMAT);
	m_pddsFrontBuffer->GetPixelFormat(&ddpf);
	if (ddpf.dwRGBBitCount <= 8)
		m_pddsFrontBuffer->GetPalette(&m_pPalette);

	//
	// create clipper
	//
	if (FAILED(hr = CreateClipper(pdd, m_hWnd, &m_lpDDClipper)))
		goto e_Init;

	//
	// create the 2d renderer
	//
	if (FAILED(hr = CLSIDFromProgID(CComBSTR("VWSYSTEM.Render2D.1"), &clsidVWRender2D)) ||
		FAILED(hr = CoCreateInstance(clsidVWRender2D, 
								NULL,
								CLSCTX_INPROC_SERVER,
								IID_IVWRender2D,
								(void **)&m_pVWRender2D)) ||
		FAILED(hr = m_pVWRender2D->Init(hWnd, pd2drm)))
	{
		ASSERT(FALSE);
		goto e_Init;
	}

	//
	// create the 3d renderer
	//
	if (FAILED(hr = CLSIDFromProgID(CComBSTR("VWSYSTEM.Render3D.1"), &clsidVWRender3D)) ||
		FAILED(hr = CoCreateInstance(clsidVWRender3D, 
								NULL,
								CLSCTX_INPROC_SERVER,
								IID_IVWRender3D,
								(void **)&m_pVWRender3D)))
	{
		ASSERT(FALSE);
		goto e_Init;
	}

	// Pass in the color model if already received.
	if (m_d3dColorModel)
		m_pVWRender3D->put_ColorModel(m_d3dColorModel);

	//
	// initialize the 3d renderer
	//
	if (FAILED(hr = m_pVWRender3D->Init(hWnd, pdd, pd3drm)))
	{
		ASSERT(FALSE);
		goto e_Init;
	}

	// Note: The backbuffer will get created on the first OnSize message

	LeaveCriticalSection(&m_CS);
	return hr;

e_Init:
	ReleaseAll();
	LeaveCriticalSection(&m_CS);
	return hr;
}

HRESULT
CVWRenderRoot::CreateBackBuffer(LPDIRECTDRAW pdd, int cx, int cy, VARIANT_BOOL bUseHWAcceleration, IDirectDrawSurface **ppddsBackBuffer)
{
	DDSURFACEDESC	ddsd;

	// it would be bad to try and create a zero-sized backbuffer
	if (cx <= 0)
		cx = 1;
	if (cy <= 0)
		cy = 1;
	
	// Create a back buffer.
	HRESULT hRes;
    memset(&ddsd,0,sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.dwWidth = cx;
	ddsd.dwHeight = cy;
	if (bUseHWAcceleration)
		ddsd.ddsCaps.dwCaps = DDSCAPS_3DDEVICE | DDSCAPS_VIDEOMEMORY;
	else
		ddsd.ddsCaps.dwCaps = DDSCAPS_3DDEVICE | DDSCAPS_SYSTEMMEMORY;
	hRes = pdd->CreateSurface( &ddsd, ppddsBackBuffer, NULL );

	if (FAILED(hRes))
		return hRes;

	//
	// attach the palette if one exists
	//
	if (m_pPalette && FAILED(hRes = (*ppddsBackBuffer)->SetPalette(m_pPalette)))
		return hRes;

	return S_OK;
}

STDMETHODIMP CVWRenderRoot::Render()
{
	HRESULT hr = S_OK;

	if (!m_pVWRender3D)
		return E_NOTINITIALIZED;

	EnterCriticalSection(&m_CS);
	
	if (FAILED(hr = m_pVWRender3D->Clear()))
		goto e_Render;

	if (m_bUseBackBuffer || !m_bUseHWAcceleration)
	{
		if (m_bFPSEnabled)
			dw2DClearTime = GetTickCount();	

		if (FAILED(hr = m_pVWRender2D->Clear()))
			goto e_Render;

		if (m_bFPSEnabled)
			dw2DClearTime = GetTickCount() - dw2DClearTime;	
	}

	if (m_bFPSEnabled)
		dw3DRenderTime = GetTickCount();	

	if (FAILED(hr = m_pVWRender3D->Render()))
		goto e_Render;

	if (m_bFPSEnabled)
		dw3DRenderTime = GetTickCount() - dw3DRenderTime;	

	if ((m_bUseBackBuffer || !m_bUseHWAcceleration) && m_bOverlayEnabled)
	{
		if (m_bFPSEnabled)
			dw2DRenderTime = GetTickCount();	

		if (FAILED(hr = m_pVWRender2D->Render()))
				goto e_Render;

		if (m_bFPSEnabled)
			dw2DRenderTime = GetTickCount() - dw2DRenderTime;	
	}

e_Render:
	LeaveCriticalSection(&m_CS);

	return hr;
}

STDMETHODIMP CVWRenderRoot::Update()
{
	HRESULT hr = S_OK;

	if (!m_pVWRender3D)
		return E_NOTINITIALIZED;

	EnterCriticalSection(&m_CS);

	CRect rectBack, rectWnd;
	static iBlt = 0;

	if (m_bFPSEnabled)
		dwBltTime = GetTickCount();

	if (GetWindowRect(m_hWnd, &rectWnd) == NULL)
	{
		ASSERT(FALSE);
		goto e_Update;
	}

	if (m_pddsFrontBuffer && m_pddsFrontBuffer->IsLost() == DDERR_SURFACELOST)
	{
		m_pddsFrontBuffer->Restore();
		// Should NOT call HandleSizeInternal from rendering thread. Seems OK to remove
		// this now after testing with new drop of Win2K.
		// It seems that just restoring the back buffer (if it exists) don't quite cut it.
//		if (FAILED(hr = HandleSizeInternal(rectWnd.Width(), rectWnd.Height(), m_bUseHWAcceleration, VARIANT_TRUE)))
//			goto e_Update;
	}

	if (m_bUseBackBuffer || !m_bUseHWAcceleration)
	{
		if (!m_pddsBackBuffer)
		{
			// Must have failed to create the back buffer.
			hr = E_FAIL;
			goto e_Update;
		}

		// Is device is smaller than window?
		if (m_iCurrentCX < rectWnd.Width())
			rectWnd.right = rectWnd.left + m_iCurrentCX;
		if (m_iCurrentCY < rectWnd.Height())
			rectWnd.bottom = rectWnd.top + m_iCurrentCY;

		hr = m_pddsFrontBuffer->SetClipper(m_lpDDClipper);
 		if (FAILED(hr))
		{
 			TRACE(CString(D3DRMErrorToString(hr))+ CString("\n"));
			goto e_Update;
 		}
		
 		// Now strech blt from back buffer to front.
 		rectBack.left = 0;
 		rectBack.top = 0;
 		if (m_bFastMode)
		{
 			rectBack.right = rectWnd.Width() >> 1;
			rectBack.bottom = rectWnd.Height() >> 1;
 		}
		else
 		{
 			rectBack.right = rectWnd.Width();
 			rectBack.bottom = rectWnd.Height();
 		}
		
		// Now strech blt from back buffer to front.
		if ((rectWnd.left != rectWnd.right) && 
			(rectWnd.top != rectWnd.bottom)) 
		{
			hr = m_pddsFrontBuffer->Blt(&rectWnd,m_pddsBackBuffer,&rectBack,DDBLT_WAIT,NULL); 
			if (FAILED(hr))
			{
				TRACE(CString(D3DRMErrorToString(hr))+ CString("\n"));
				goto e_Update;
			}
		}
	}
	else
	{
		m_pVWRender3D->Update();
	}

	if (m_bFPSEnabled)
		dwBltTime = GetTickCount() - dwBltTime;	

	if (m_bFPSEnabled)
		((CVWRender3D*)m_pVWRender3D)->CalculateFPS(dw2DClearTime, dw3DRenderTime, dw2DRenderTime, dwBltTime);

e_Update:

	LeaveCriticalSection(&m_CS);

	return hr;
}

STDMETHODIMP CVWRenderRoot::HitTest(VARIANT varHitParams, VARIANT_BOOL vbReturnAllPicked, VARIANT *pvarHitResult)
{
	HRESULT hr;

	if (!m_pVWRender3D)
		return E_NOTINITIALIZED;

	if (m_bUseBackBuffer || !m_bUseHWAcceleration)
	{
		if (FAILED(hr = m_pVWRender2D->HitTest(varHitParams, vbReturnAllPicked, pvarHitResult)))
			return hr;
	}

	if (FAILED(hr = m_pVWRender3D->HitTest(varHitParams, vbReturnAllPicked, pvarHitResult)))
		return hr;

	return S_OK;
}

STDMETHODIMP CVWRenderRoot::get_FastMode(VARIANT_BOOL* pbFastMode)
{
	*pbFastMode = m_bFastMode;
	return S_OK;
}

STDMETHODIMP CVWRenderRoot::put_FastMode(VARIANT_BOOL bFastMode)
{
	HRESULT hr;

	if (!m_bUseHWAcceleration && m_bFastModeEnabled && m_bFastMode != bFastMode)
	{
		// This should be wrapped with critical section so that we don't
		// render in non-fast mode, and then Blt as fastmode
		EnterCriticalSection(&m_CS);
		m_bFastMode = bFastMode;
		if (FAILED(hr = m_pVWRender2D->SetFastMode(bFastMode)) ||
			FAILED(hr = m_pVWRender3D->SetFastMode(bFastMode)))
			TRACE(CString(D3DRMErrorToString(hr))+ CString("\n"));
		LeaveCriticalSection(&m_CS);
		return hr;
	}
	return S_OK;
}

STDMETHODIMP CVWRenderRoot::put_FastModeEnabled(VARIANT_BOOL bEnabled)
{
	if (!bEnabled)
		put_FastMode(VARIANT_FALSE);
	m_bFastModeEnabled = bEnabled;
	return S_OK;
}

STDMETHODIMP CVWRenderRoot::get_FastModeEnabled(VARIANT_BOOL *pbEnabled)
{
	*pbEnabled = m_bFastModeEnabled;
	return S_OK;
}

STDMETHODIMP CVWRenderRoot::HandleSize(int cx, int cy)
{
	DWORD			dwDeviceWidth, dwDeviceHeight;
	VARIANT_BOOL	bRebuild3DDevice;

	if (!m_pVWRender3D)
		return E_NOTINITIALIZED;

	// If the 3D device is too small we're gonna need a new 3D device.
	m_pVWRender3D->get_DeviceWidth(&dwDeviceWidth);
	m_pVWRender3D->get_DeviceHeight(&dwDeviceHeight);
	bRebuild3DDevice = (int)dwDeviceWidth < cx || (int)dwDeviceHeight < cy;

	return HandleSizeInternal(cx, cy, m_bUseHWAcceleration, bRebuild3DDevice);
}

STDMETHODIMP CVWRenderRoot::put_ColorModel(int d3drmColorModel)
{
	CRect	rectWnd;

	m_d3dColorModel = (D3DCOLORMODEL)d3drmColorModel;

	if (m_pVWRender3D)
	{
		m_pVWRender3D->put_ColorModel(d3drmColorModel);
		if (!m_bUseHWAcceleration)
		{
			GetWindowRect(m_hWnd, &rectWnd);
			return HandleSizeInternal(rectWnd.Width(), rectWnd.Height(), m_bUseHWAcceleration, VARIANT_TRUE);
		}
	}
	return S_OK;
}

STDMETHODIMP CVWRenderRoot::put_UseHWAcceleration(VARIANT_BOOL bUseHWAcceleration)
{
	CRect	rectWnd;
	HRESULT hr = S_OK;

	if (m_bUseHWAcceleration == bUseHWAcceleration)
		return S_OK;

	// This really just sets up a back buffer if necessary.
	if (m_pVWRender3D)
	{
		GetWindowRect(m_hWnd, &rectWnd);
		if (FAILED(hr = HandleSizeInternal(rectWnd.Width(), rectWnd.Height(), bUseHWAcceleration, VARIANT_TRUE)))
			return hr;
	}
	else
		m_bUseHWAcceleration = bUseHWAcceleration;
	
	return S_OK;
}

STDMETHODIMP CVWRenderRoot::get_UseHWAcceleration(VARIANT_BOOL *pbUseHWAcceleration)
{
	*pbUseHWAcceleration = m_bUseHWAcceleration;
	return S_OK;
}

STDMETHODIMP CVWRenderRoot::put_UseBackBuffer(VARIANT_BOOL bUseBackBuffer)
{
	CRect	rectWnd;
	HRESULT hr = S_OK;

	if (m_bForceBackBuffer || (m_bUseBackBuffer == bUseBackBuffer))
		return S_OK;

	m_bUseBackBuffer = bUseBackBuffer;

	// This really just sets up a back buffer if necessary.
	if (m_pVWRender3D)
	{
		GetWindowRect(m_hWnd, &rectWnd);
		if (FAILED(hr = HandleSizeInternal(rectWnd.Width(), rectWnd.Height(), m_bUseHWAcceleration, VARIANT_TRUE)))
			return hr;
	}
	
	return S_OK;
}

STDMETHODIMP CVWRenderRoot::get_UseBackBuffer(VARIANT_BOOL *pbUseBackBuffer)
{
	*pbUseBackBuffer = m_bUseBackBuffer;
	return S_OK;
}

STDMETHODIMP CVWRenderRoot::put_ForceBackBuffer(VARIANT_BOOL bForceBackBuffer)
{
	CRect	rectWnd;
	HRESULT hr = S_OK;

	if (bForceBackBuffer)
	{
		if (FAILED(hr = put_UseBackBuffer(VARIANT_TRUE)))
			return hr;
	}
	m_bForceBackBuffer = bForceBackBuffer;

	if (!bForceBackBuffer)
	{
		if (FAILED(hr = put_UseBackBuffer(VARIANT_FALSE)))
			return hr;
	}
	return hr;
}

STDMETHODIMP CVWRenderRoot::get_ForceBackBuffer(VARIANT_BOOL *pbForceBackBuffer)
{
	*pbForceBackBuffer = m_bForceBackBuffer;
	return S_OK;
}

STDMETHODIMP CVWRenderRoot::RebuildDevice()
{
	CRect	rectWnd;
	HRESULT hr = S_OK;

	if (m_pVWRender3D)
	{
		GetWindowRect(m_hWnd, &rectWnd);
		hr = HandleSizeInternal(rectWnd.Width(), rectWnd.Height(), m_bUseHWAcceleration, VARIANT_TRUE);
	}
	return hr;
}

HRESULT CVWRenderRoot::HandleSizeInternal(int cx, int cy, VARIANT_BOOL bUseHWAcceleration, VARIANT_BOOL bRebuild3DDevice)
{
	EnterCriticalSection(&m_CS);

	static			iLevel = 0;
	HRESULT			hRes = S_OK;
	IWorld*			pWorld = NULL;
	DWORD			dwDeviceType = 0;

	iLevel++;

	if (bRebuild3DDevice)
	{
		m_pVWRender3D->ReleaseDevice();
		if (m_pddsBackBuffer)
			m_pVWRender2D->ReleaseDevice();

		SAFERELEASE(m_pddsBackBuffer);
	}
	else if (m_pddsBackBuffer)
		m_pVWRender2D->ReleaseDevice();

	if (bRebuild3DDevice)
	{
		if (bUseHWAcceleration)
			dwDeviceType = 2;
		else
			dwDeviceType = 1;
	}

	if ((bRebuild3DDevice && (m_bUseBackBuffer || !bUseHWAcceleration) && FAILED(hRes = CreateBackBuffer(m_pDD, cx, cy, bUseHWAcceleration, &m_pddsBackBuffer))) ||
		 FAILED(hRes = m_pVWRender3D->HandleSize(m_pDD, m_pddsBackBuffer, m_lpDDClipper, cx, cy, dwDeviceType)))
	{
		TRACE("Error %x creating 3D device.\n", hRes);
		if (hRes == DDERR_OUTOFVIDEOMEMORY && iLevel < 4)
		{
			// Try to recreate at at least 3/4 size. Try at least once at old size (if appropriate).
			if (iLevel == 1 && int(cx*0.75) < m_iCurrentCX) cx = m_iCurrentCX; else cx = (int)(cx*0.75);
			if (iLevel == 1 && int(cy*0.75) < m_iCurrentCY) cy = m_iCurrentCY; else cy = (int)(cy*0.75);
			hRes = HandleSizeInternal(cx, cy, bUseHWAcceleration, VARIANT_TRUE);
		}
		if (iLevel == 1 && FAILED(hRes))
		{
			// We completely failed to create the device. If we were trying for HW,
			// now try for SW.
			if (bUseHWAcceleration)
				hRes = HandleSizeInternal(cx, cy, VARIANT_FALSE, VARIANT_TRUE);
		}
		goto ERROR_ENCOUNTERED;
	}
	
	if (m_bUseBackBuffer || !bUseHWAcceleration)
	{
		if (FAILED(hRes = m_pVWRender2D->HandleSize(m_pddsBackBuffer)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hRes = m_pVWRender2D->SetBackgroundColor(GetRValue(m_crBackgroundColor),
															GetGValue(m_crBackgroundColor),
															GetBValue(m_crBackgroundColor))))
			goto ERROR_ENCOUNTERED;
	}

	m_bUseHWAcceleration = bUseHWAcceleration;
	m_iCurrentCX = cx; m_iCurrentCY = cy;

	if (m_pvwfScene)
	{
		hRes = m_pvwfScene->get_World(&pWorld);
		if (FAILED(hRes) || !pWorld) goto ERROR_ENCOUNTERED;

		pWorld->FireUIEvent(NULL, CComBSTR(VWSTUDIO_DEVICECHANGED_EVENT_STR), CComVariant(NULL));
	}

ERROR_ENCOUNTERED:

	SAFERELEASE(pWorld);

	iLevel--;

	LeaveCriticalSection(&m_CS);

	return hRes;
}

STDMETHODIMP CVWRenderRoot::get_Scene(IVWFrame **ppvwfScene)
{
	if (m_pvwfScene)
	{
		m_pvwfScene->AddRef();
	}
	*ppvwfScene = m_pvwfScene;
	return S_OK;
}

STDMETHODIMP CVWRenderRoot::put_Scene(IVWFrame *pvwfScene)
{
	if (!m_pVWRender3D)
		return E_NOTINITIALIZED;

	EnterCriticalSection(&m_CS);
	
	LPDIRECT2DRMFRAME pd2drmf = NULL;
	IDirect3DRMFrame *pd3drmf = NULL;
	HRESULT hr = S_OK;

	if (m_pvwfScene)
	{
		m_pvwfScene->Release();
	}
	m_pvwfScene = pvwfScene;

	if (m_pvwfScene)
	{
		m_pvwfScene->AddRef();

		if (m_bUseBackBuffer || !m_bUseHWAcceleration)
		{
			if (FAILED(hr = m_pvwfScene->get_Frame2D(&pd2drmf)))
			{
				ASSERT(SUCCEEDED(hr));
				goto e_put_Scene;
			}

			if (FAILED(hr = m_pVWRender2D->put_Scene(pd2drmf)))
			{
				ASSERT(SUCCEEDED(hr));
				goto e_put_Scene;
			}
		}

		if (FAILED(hr = m_pvwfScene->get_Frame3D(&pd3drmf)))
		{
			ASSERT(SUCCEEDED(hr));
			goto e_put_Scene;
		}
		if (FAILED(hr = m_pVWRender3D->put_Scene(pd3drmf)))
		{
			ASSERT(SUCCEEDED(hr));
			goto e_put_Scene;
		}
	}
	else
	{
		if (m_bUseBackBuffer || !m_bUseHWAcceleration)
		{
			if (FAILED(hr = m_pVWRender2D->put_Scene(NULL)))
			{
				ASSERT(SUCCEEDED(hr));
				goto e_put_Scene;
			}
		}

		if (FAILED(hr = m_pVWRender3D->put_Scene(NULL)))
		{
			ASSERT(SUCCEEDED(hr));
			goto e_put_Scene;
		}
	}
e_put_Scene:
	MMRELEASE(pd2drmf);
	MMRELEASE(pd3drmf);

	ASSERT(SUCCEEDED(hr));

	LeaveCriticalSection(&m_CS);

	return hr;
}

/*
STDMETHODIMP CVWRenderRoot::get_BackgroundColor(int* pnRGB)
{
	*pnRGB = m_nRGB;
	return S_OK;
}
*/

STDMETHODIMP CVWRenderRoot::SetBackgroundColor(BYTE r, BYTE g, BYTE b)
{
	HDC		hDC;
	HRESULT hr = S_OK;

	if (!m_pVWRender3D)
		return E_NOTINITIALIZED;

	EnterCriticalSection(&m_CS);

	if (m_bUseBackBuffer || !m_bUseHWAcceleration)
	{
		if (FAILED(hr = m_pVWRender2D->SetBackgroundColor(r, g, b)))
			goto ERROR_ENCOUNTERED;
	}

	if (FAILED(hr = m_pVWRender3D->SetBackgroundColor(r, g, b)))
		goto ERROR_ENCOUNTERED;

	m_crBackgroundColor = RGB(r,g,b);
	hDC = GetDC(m_hWnd);
	if (hDC)
		HandlePaint(hDC);

ERROR_ENCOUNTERED:
	
	LeaveCriticalSection(&m_CS);
	return hr;
}

STDMETHODIMP CVWRenderRoot::get_VWRender3D(IVWRender3D **ppVWRender3D)
{
	*ppVWRender3D = m_pVWRender3D;
	SAFEADDREF(m_pVWRender3D);
	return S_OK;
}

STDMETHODIMP CVWRenderRoot::get_VWRender2D(IVWRender2D **ppVWRender2D)
{
	*ppVWRender2D = m_pVWRender2D;
	SAFEADDREF(m_pVWRender2D);
	return S_OK;
}

STDMETHODIMP CVWRenderRoot::get_Camera(IVWFrame **ppvwfCamera)
{
	HRESULT hr;
	IDirect3DRMFrame *pd3drmf = NULL;
	IDirect2DRMFrame *pd2drmf = NULL;

	if (!m_pVWRender3D)
		return E_NOTINITIALIZED;

	if (m_bUseBackBuffer || !m_bUseHWAcceleration)
	{
		if (FAILED(hr = m_pVWRender2D->get_Camera(&pd2drmf)))
			goto e_get_Camera;
	}
	if (FAILED(hr = m_pVWRender3D->get_Camera(&pd3drmf)))
		goto e_get_Camera;

	if (pd2drmf != m_pd2drmfCamera ||
		pd3drmf != m_pd3drmfCamera)
	{
		// OK, the camera has changed, create a new VWFrame to represent the camera.
		CLSID clsidVWFrame;
		if (hr = FAILED(CLSIDFromProgID(CComBSTR("VWSYSTEM.Frame.1"), &clsidVWFrame)))
		{
			ASSERT(FALSE);
			goto e_get_Camera;
		}
		if (FAILED(hr = CoCreateInstance(clsidVWFrame, 
									NULL,
									CLSCTX_INPROC_SERVER,
									IID_IVWFrame,
									(void **)&(*ppvwfCamera))))
		{
			ASSERT(FALSE);
			goto e_get_Camera;
		}
		if (FAILED(hr = (*ppvwfCamera)->put_Frame2D(pd2drmf)) ||
			FAILED(hr = (*ppvwfCamera)->put_Frame3D(pd3drmf))) {
			(*ppvwfCamera)->Release();
			(*ppvwfCamera) = NULL;
			goto e_get_Camera;
		}

		SAFERELEASE(m_pvwfCamera);
		SAFERELEASE(m_pd2drmfCamera);
		SAFERELEASE(m_pd3drmfCamera);
		m_pvwfCamera = *ppvwfCamera;
		SAFEADDREF(m_pvwfCamera);
		m_pd2drmfCamera = pd2drmf;
		SAFEADDREF(m_pd2drmfCamera);
		m_pd3drmfCamera = pd3drmf;
		SAFEADDREF(m_pd3drmfCamera);
	}
	else
	{
		// Same camera, another reference.
		*ppvwfCamera = m_pvwfCamera;
		SAFEADDREF(m_pvwfCamera);
	}

e_get_Camera:
	MMRELEASE(pd2drmf);
	MMRELEASE(pd3drmf);

	return hr;
}

char*
D3DRMErrorToString(HRESULT error)
{
    switch(error) {
        case DD_OK:
	    /* Also includes D3D_OK and D3DRM_OK */
            return "No error.\0";
        case DDERR_ALREADYINITIALIZED:
            return "This object is already initialized.\0";
        case DDERR_BLTFASTCANTCLIP:
            return "Return if a clipper object is attached to the source surface passed into a BltFast call.\0";
        case DDERR_CANNOTATTACHSURFACE:
            return "This surface can not be attached to the requested surface.\0";
        case DDERR_CANNOTDETACHSURFACE:
            return "This surface can not be detached from the requested surface.\0";
        case DDERR_CANTCREATEDC:
            return "Windows can not create any more DCs.\0";
        case DDERR_CANTDUPLICATE:
            return "Can't duplicate primary & 3D surfaces, or surfaces that are implicitly created.\0";
        case DDERR_CLIPPERISUSINGHWND:
            return "An attempt was made to set a cliplist for a clipper object that is already monitoring an hwnd.\0";
        case DDERR_COLORKEYNOTSET:
            return "No src color key specified for this operation.\0";
        case DDERR_CURRENTLYNOTAVAIL:
            return "Support is currently not available.\0";
        case DDERR_DIRECTDRAWALREADYCREATED:
            return "A DirectDraw object representing this driver has already been created for this process.\0";
        case DDERR_EXCEPTION:
            return "An exception was encountered while performing the requested operation.\0";
        case DDERR_EXCLUSIVEMODEALREADYSET:
            return "An attempt was made to set the cooperative level when it was already set to exclusive.\0";
        case DDERR_GENERIC:
            return "Generic failure.\0";
        case DDERR_HEIGHTALIGN:
            return "Height of rectangle provided is not a multiple of reqd alignment.\0";
        case DDERR_HWNDALREADYSET:
            return "The CooperativeLevel HWND has already been set. It can not be reset while the process has surfaces or palettes created.\0";
        case DDERR_HWNDSUBCLASSED:
            return "HWND used by DirectDraw CooperativeLevel has been subclassed, this prevents DirectDraw from restoring state.\0";
        case DDERR_IMPLICITLYCREATED:
            return "This surface can not be restored because it is an implicitly created surface.\0";
        case DDERR_INCOMPATIBLEPRIMARY:
            return "Unable to match primary surface creation request with existing primary surface.\0";
        case DDERR_INVALIDCAPS:
            return "One or more of the caps bits passed to the callback are incorrect.\0";
        case DDERR_INVALIDCLIPLIST:
            return "DirectDraw does not support the provided cliplist.\0";
        case DDERR_INVALIDDIRECTDRAWGUID:
            return "The GUID passed to DirectDrawCreate is not a valid DirectDraw driver identifier.\0";
        case DDERR_INVALIDMODE:
            return "DirectDraw does not support the requested mode.\0";
        case DDERR_INVALIDOBJECT:
            return "DirectDraw received a pointer that was an invalid DIRECTDRAW object.\0";
        case DDERR_INVALIDPARAMS:
            return "One or more of the parameters passed to the function are incorrect.\0";
        case DDERR_INVALIDPIXELFORMAT:
            return "The pixel format was invalid as specified.\0";
        case DDERR_INVALIDPOSITION:
            return "Returned when the position of the overlay on the destination is no longer legal for that destination.\0";
        case DDERR_INVALIDRECT:
            return "Rectangle provided was invalid.\0";
        case DDERR_LOCKEDSURFACES:
            return "Operation could not be carried out because one or more surfaces are locked.\0";
        case DDERR_NO3D:
            return "There is no 3D present.\0";
        case DDERR_NOALPHAHW:
            return "Operation could not be carried out because there is no alpha accleration hardware present or available.\0";
        case DDERR_NOBLTHW:
            return "No blitter hardware present.\0";
        case DDERR_NOCLIPLIST:
            return "No cliplist available.\0";
        case DDERR_NOCLIPPERATTACHED:
            return "No clipper object attached to surface object.\0";
        case DDERR_NOCOLORCONVHW:
            return "Operation could not be carried out because there is no color conversion hardware present or available.\0";
        case DDERR_NOCOLORKEY:
            return "Surface doesn't currently have a color key\0";
        case DDERR_NOCOLORKEYHW:
            return "Operation could not be carried out because there is no hardware support of the destination color key.\0";
        case DDERR_NOCOOPERATIVELEVELSET:
            return "Create function called without DirectDraw object method SetCooperativeLevel being called.\0";
        case DDERR_NODC:
            return "No DC was ever created for this surface.\0";
        case DDERR_NODDROPSHW:
            return "No DirectDraw ROP hardware.\0";
        case DDERR_NODIRECTDRAWHW:
            return "A hardware-only DirectDraw object creation was attempted but the driver did not support any hardware.\0";
        case DDERR_NOEMULATION:
            return "Software emulation not available.\0";
        case DDERR_NOEXCLUSIVEMODE:
            return "Operation requires the application to have exclusive mode but the application does not have exclusive mode.\0";
        case DDERR_NOFLIPHW:
            return "Flipping visible surfaces is not supported.\0";
        case DDERR_NOGDI:
            return "There is no GDI present.\0";
        case DDERR_NOHWND:
            return "Clipper notification requires an HWND or no HWND has previously been set as the CooperativeLevel HWND.\0";
        case DDERR_NOMIRRORHW:
            return "Operation could not be carried out because there is no hardware present or available.\0";
        case DDERR_NOOVERLAYDEST:
            return "Returned when GetOverlayPosition is called on an overlay that UpdateOverlay has never been called on to establish a destination.\0";
        case DDERR_NOOVERLAYHW:
            return "Operation could not be carried out because there is no overlay hardware present or available.\0";
        case DDERR_NOPALETTEATTACHED:
            return "No palette object attached to this surface.\0";
        case DDERR_NOPALETTEHW:
            return "No hardware support for 16 or 256 color palettes.\0";
        case DDERR_NORASTEROPHW:
            return "Operation could not be carried out because there is no appropriate raster op hardware present or available.\0";
        case DDERR_NOROTATIONHW:
            return "Operation could not be carried out because there is no rotation hardware present or available.\0";
        case DDERR_NOSTRETCHHW:
            return "Operation could not be carried out because there is no hardware support for stretching.\0";
        case DDERR_NOT4BITCOLOR:
            return "DirectDrawSurface is not in 4 bit color palette and the requested operation requires 4 bit color palette.\0";
        case DDERR_NOT4BITCOLORINDEX:
            return "DirectDrawSurface is not in 4 bit color index palette and the requested operation requires 4 bit color index palette.\0";
        case DDERR_NOT8BITCOLOR:
            return "DirectDrawSurface is not in 8 bit color mode and the requested operation requires 8 bit color.\0";
        case DDERR_NOTAOVERLAYSURFACE:
            return "Returned when an overlay member is called for a non-overlay surface.\0";
        case DDERR_NOTEXTUREHW:
            return "Operation could not be carried out because there is no texture mapping hardware present or available.\0";
        case DDERR_NOTFLIPPABLE:
            return "An attempt has been made to flip a surface that is not flippable.\0";
        case DDERR_NOTFOUND:
            return "Requested item was not found.\0";
        case DDERR_NOTLOCKED:
            return "Surface was not locked.  An attempt to unlock a surface that was not locked at all, or by this process, has been attempted.\0";
        case DDERR_NOTPALETTIZED:
            return "The surface being used is not a palette-based surface.\0";
        case DDERR_NOVSYNCHW:
            return "Operation could not be carried out because there is no hardware support for vertical blank synchronized operations.\0";
        case DDERR_NOZBUFFERHW:
            return "Operation could not be carried out because there is no hardware support for zbuffer blitting.\0";
        case DDERR_NOZOVERLAYHW:
            return "Overlay surfaces could not be z layered based on their BltOrder because the hardware does not support z layering of overlays.\0";
        case DDERR_OUTOFCAPS:
            return "The hardware needed for the requested operation has already been allocated.\0";
        case DDERR_OUTOFMEMORY:
            return "DirectDraw does not have enough memory to perform the operation.\0";
        case DDERR_OUTOFVIDEOMEMORY:
            return "DirectDraw does not have enough memory to perform the operation.\0";
        case DDERR_OVERLAYCANTCLIP:
            return "The hardware does not support clipped overlays.\0";
        case DDERR_OVERLAYCOLORKEYONLYONEACTIVE:
            return "Can only have ony color key active at one time for overlays.\0";
        case DDERR_OVERLAYNOTVISIBLE:
            return "Returned when GetOverlayPosition is called on a hidden overlay.\0";
        case DDERR_PALETTEBUSY:
            return "Access to this palette is being refused because the palette is already locked by another thread.\0";
        case DDERR_PRIMARYSURFACEALREADYEXISTS:
            return "This process already has created a primary surface.\0";
        case DDERR_REGIONTOOSMALL:
            return "Region passed to Clipper::GetClipList is too small.\0";
        case DDERR_SURFACEALREADYATTACHED:
            return "This surface is already attached to the surface it is being attached to.\0";
        case DDERR_SURFACEALREADYDEPENDENT:
            return "This surface is already a dependency of the surface it is being made a dependency of.\0";
        case DDERR_SURFACEBUSY:
            return "Access to this surface is being refused because the surface is already locked by another thread.\0";
        case DDERR_SURFACEISOBSCURED:
            return "Access to surface refused because the surface is obscured.\0";
        case DDERR_SURFACELOST:
            return "Access to this surface is being refused because the surface memory is gone. The DirectDrawSurface object representing this surface should have Restore called on it.\0";
        case DDERR_SURFACENOTATTACHED:
            return "The requested surface is not attached.\0";
        case DDERR_TOOBIGHEIGHT:
            return "Height requested by DirectDraw is too large.\0";
        case DDERR_TOOBIGSIZE:
            return "Size requested by DirectDraw is too large, but the individual height and width are OK.\0";
        case DDERR_TOOBIGWIDTH:
            return "Width requested by DirectDraw is too large.\0";
        case DDERR_UNSUPPORTED:
            return "Action not supported.\0";
        case DDERR_UNSUPPORTEDFORMAT:
            return "FOURCC format requested is unsupported by DirectDraw.\0";
        case DDERR_UNSUPPORTEDMASK:
            return "Bitmask in the pixel format requested is unsupported by DirectDraw.\0";
        case DDERR_VERTICALBLANKINPROGRESS:
            return "Vertical blank is in progress.\0";
        case DDERR_WASSTILLDRAWING:
            return "Informs DirectDraw that the previous Blt which is transfering information to or from this Surface is incomplete.\0";
        case DDERR_WRONGMODE:
            return "This surface can not be restored because it was created in a different mode.\0";
        case DDERR_XALIGN:
            return "Rectangle provided was not horizontally aligned on required boundary.\0";
        case D3DERR_BADMAJORVERSION:
            return "D3DERR_BADMAJORVERSION\0";
        case D3DERR_BADMINORVERSION:
            return "D3DERR_BADMINORVERSION\0";
        case D3DERR_EXECUTE_LOCKED:
            return "D3DERR_EXECUTE_LOCKED\0";
        case D3DERR_EXECUTE_NOT_LOCKED:
            return "D3DERR_EXECUTE_NOT_LOCKED\0";
        case D3DERR_EXECUTE_CREATE_FAILED:
            return "D3DERR_EXECUTE_CREATE_FAILED\0";
        case D3DERR_EXECUTE_DESTROY_FAILED:
            return "D3DERR_EXECUTE_DESTROY_FAILED\0";
        case D3DERR_EXECUTE_LOCK_FAILED:
            return "D3DERR_EXECUTE_LOCK_FAILED\0";
        case D3DERR_EXECUTE_UNLOCK_FAILED:
            return "D3DERR_EXECUTE_UNLOCK_FAILED\0";
        case D3DERR_EXECUTE_FAILED:
            return "D3DERR_EXECUTE_FAILED\0";
        case D3DERR_EXECUTE_CLIPPED_FAILED:
            return "D3DERR_EXECUTE_CLIPPED_FAILED\0";
        case D3DERR_TEXTURE_NO_SUPPORT:
            return "D3DERR_TEXTURE_NO_SUPPORT\0";
        case D3DERR_TEXTURE_NOT_LOCKED:
            return "D3DERR_TEXTURE_NOT_LOCKED\0";
        case D3DERR_TEXTURE_LOCKED:
            return "D3DERR_TEXTURELOCKED\0";
        case D3DERR_TEXTURE_CREATE_FAILED:
            return "D3DERR_TEXTURE_CREATE_FAILED\0";
        case D3DERR_TEXTURE_DESTROY_FAILED:
            return "D3DERR_TEXTURE_DESTROY_FAILED\0";
        case D3DERR_TEXTURE_LOCK_FAILED:
            return "D3DERR_TEXTURE_LOCK_FAILED\0";
        case D3DERR_TEXTURE_UNLOCK_FAILED:
            return "D3DERR_TEXTURE_UNLOCK_FAILED\0";
        case D3DERR_TEXTURE_LOAD_FAILED:
            return "D3DERR_TEXTURE_LOAD_FAILED\0";
        case D3DERR_MATRIX_CREATE_FAILED:
            return "D3DERR_MATRIX_CREATE_FAILED\0";
        case D3DERR_MATRIX_DESTROY_FAILED:
            return "D3DERR_MATRIX_DESTROY_FAILED\0";
        case D3DERR_MATRIX_SETDATA_FAILED:
            return "D3DERR_MATRIX_SETDATA_FAILED\0";
        case D3DERR_SETVIEWPORTDATA_FAILED:
            return "D3DERR_SETVIEWPORTDATA_FAILED\0";
        case D3DERR_MATERIAL_CREATE_FAILED:
            return "D3DERR_MATERIAL_CREATE_FAILED\0";
        case D3DERR_MATERIAL_DESTROY_FAILED:
            return "D3DERR_MATERIAL_DESTROY_FAILED\0";
        case D3DERR_MATERIAL_SETDATA_FAILED:
            return "D3DERR_MATERIAL_SETDATA_FAILED\0";
        case D3DERR_LIGHT_SET_FAILED:
            return "D3DERR_LIGHT_SET_FAILED\0";
	case D3DRMERR_BADOBJECT:
	    return "D3DRMERR_BADOBJECT\0";
	case D3DRMERR_BADTYPE:
	    return "D3DRMERR_BADTYPE\0";
	case D3DRMERR_BADALLOC:
	    return "D3DRMERR_BADALLOC\0";
	case D3DRMERR_FACEUSED:
	    return "D3DRMERR_FACEUSED\0";
	case D3DRMERR_NOTFOUND:
	    return "D3DRMERR_NOTFOUND\0";
	case D3DRMERR_NOTDONEYET:
	    return "D3DRMERR_NOTDONEYET\0";
	case D3DRMERR_FILENOTFOUND:
	    return "The file was not found.\0";
	case D3DRMERR_BADFILE:
	    return "D3DRMERR_BADFILE\0";
	case D3DRMERR_BADDEVICE:
	    return "D3DRMERR_BADDEVICE\0";
	case D3DRMERR_BADVALUE:
	    return "D3DRMERR_BADVALUE\0";
	case D3DRMERR_BADMAJORVERSION:
	    return "D3DRMERR_BADMAJORVERSION\0";
	case D3DRMERR_BADMINORVERSION:
	    return "D3DRMERR_BADMINORVERSION\0";
	case D3DRMERR_UNABLETOEXECUTE:
	    return "D3DRMERR_UNABLETOEXECUTE\0";
        default:
            return "Unrecognized error value.\0";
    }
}

HRESULT DDSetColorKey(IDirectDrawSurface *pdds, COLORREF rgb)
{
    DDCOLORKEY          ddck;

    ddck.dwColorSpaceLowValue  = DDColorMatch(pdds, rgb);
    ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
    return pdds->SetColorKey(DDCKEY_SRCBLT, &ddck);
}

DWORD DDColorMatch(IDirectDrawSurface *pdds, COLORREF rgb)
{
    COLORREF rgbT;
    HDC hdc;
    DWORD dw = CLR_INVALID;
    DDSURFACEDESC ddsd;
    HRESULT hres;

    //
    //  use GDI SetPixel to color match for us
    //
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
	rgbT = GetPixel(hdc, 0, 0);             // save current pixel value
	SetPixel(hdc, 0, 0, rgb);               // set our value
	pdds->ReleaseDC(hdc);
    }

    //
    // now lock the surface so we can read back the converted color
    //
    ddsd.dwSize = sizeof(ddsd);
    while ((hres = pdds->Lock(NULL, &ddsd, 0, NULL)) == DDERR_WASSTILLDRAWING)
	;

    if (hres == DD_OK)
    {
	dw  = *(DWORD *)ddsd.lpSurface;                     // get DWORD
        if(ddsd.ddpfPixelFormat.dwRGBBitCount < 32)
            dw &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount)-1;  // mask it to bpp
	pdds->Unlock(NULL);
    }

    //
    //  now put the color that was there back.
    //
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
	SetPixel(hdc, 0, 0, rgbT);
	pdds->ReleaseDC(hdc);
    }

    return dw;
}
