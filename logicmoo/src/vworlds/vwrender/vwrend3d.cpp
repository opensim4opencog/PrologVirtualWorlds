// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWRend3D.cpp : Implementation of CVWRenderApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
// d3drmwin.h needs to go above vwrender.h
#include <d3drmwin.h>
#include <vwrender.h>
#include "VWRend3D.h"
#include "syshelp.h"
#include <vwutils.h>

#ifndef WEIRDDRAWLIBBUGFIXED
typedef HRESULT (WINAPI *DDCREATECLIPPERPFN)(DWORD dwFlags, LPDIRECTDRAWCLIPPER FAR *lplpDDClipper, IUnknown FAR *pUnkOuter );
#endif

#ifdef _DEBUG
void DumpGraphics(IDirect3DRM *prlapi);
void RMObjEnumFunc( LPDIRECT3DRMOBJECT obj, LPVOID arg);
#endif

#define dwRefreshFPS 2000		/* refresh FPS every 2 seconds or so */

//------------------------------------------------------------------
// 
// Function     : D3DEnumDriverCallBack()
//
// Purpose      : Enumeration Function
//
//------------------------------------------------------------------

HRESULT WINAPI D3DEnumDeviceCallBack(LPGUID lpGuid,     
                                     LPSTR lpDeviceDescription,
                                     LPSTR lpDeviceName,
                                     LPD3DDEVICEDESC lpHWDesc, 
                                     LPD3DDEVICEDESC lpHELDesc, 
                                     LPVOID lpContext)
{
    D3DDeviceInfo* pInfo = (D3DDeviceInfo *)lpContext;
    
	if (pInfo->bFindHWDevice)
	{
   		// Is this a hardware device?
		if (lpHWDesc->dcmColorModel & pInfo->cm)
		{
			// Make sure the driver has Z-Buffering capabilities and 
			// can support textures.
			if (((lpHWDesc->dwDeviceZBufferBitDepth & DDBD_16) || 
				 (lpHWDesc->dwDeviceZBufferBitDepth & DDBD_24) ||
				 (lpHWDesc->dwDeviceZBufferBitDepth & DDBD_32)) &&
				(lpHWDesc->dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_PERSPECTIVE))
			{                                       
				// Record the HAL description for later use
				memcpy(&pInfo->HWDeviceDesc, lpHWDesc, sizeof(D3DDEVICEDESC));

				// Record the guid for later use
				pInfo->lpHWGuid = lpGuid;
            
				// No need to keep looking for any more devices
				return D3DENUMRET_CANCEL;
			}
		}
	}
	else
	{
		if (lpHELDesc->dcmColorModel & pInfo->cm)
		{
			// Record the HEL description for later use
			memcpy(&pInfo->SWDeviceDesc, lpHELDesc, sizeof(D3DDEVICEDESC));
            
			// Record the guid for later use
			pInfo->lpSWGuid = lpGuid;

			// No need to keep looking for any more devices
			return D3DENUMRET_CANCEL;
		}
	}
	// Keep looking.
    return D3DENUMRET_OK;
}

/////////////////////////////////////////////////////////////////////////////
//

CPtrList CVWRender3D::m_list3DRenderers;

CVWRender3D::CVWRender3D()
{
	m_prlDevice = NULL;
	m_prlWinDevice = NULL;
	m_prlViewport = NULL;
	m_prlfScene = NULL;
	m_prlfEmptyScene = NULL;
	m_prlfCamera = NULL;
	m_bMoveScene = TRUE;
	m_hWnd = NULL;
	m_pPalette = NULL;
	m_d3dColorModel = D3DCOLORMODEL(0);
	m_fField = D3DVALUE(fDefaultField);

	// Add this renderer to the list of 3D renderers.
	m_list3DRenderers.AddTail(this);
}

CVWRender3D::~CVWRender3D()
{
	POSITION			pos;
	CVWRender3D			*p3DRenderer;
	IDirect3DRMFrame	*p3DScene;

	// Remove this renderer to the list of 3D renderers.
	pos = m_list3DRenderers.Find(this);
	ASSERT(pos);
	m_list3DRenderers.RemoveAt(pos);

	if (m_bMoveScene)
	{
		// OK, lets see if this scene is (still) being rendered by another renderer.
		// If it is then that other renderer will have to take over moving the scene.
		pos = m_list3DRenderers.GetHeadPosition();
		while (pos)
		{
			p3DRenderer = (CVWRender3D*)m_list3DRenderers.GetNext(pos);
			p3DRenderer->get_Scene(&p3DScene);
			if (p3DScene == m_prlfScene)
			{
				p3DRenderer->m_bMoveScene = TRUE;
			}
			SAFERELEASE(p3DScene);
		}
	}

	ReleaseAll();
}

HRESULT 
CVWRender3D::ReleaseAll() 
{
	SAFERELEASE(m_prlfEmptyScene);
	SAFERELEASE(m_prlfCamera);
	SAFERELEASE(m_prlfScene);
	SAFERELEASE(m_prlViewport);
	SAFERELEASE(m_prlDevice);
	SAFERELEASE(m_prlWinDevice);
	SAFERELEASE(m_prlapi);
	SAFERELEASE(m_pPalette);
	return S_OK;
}

STDMETHODIMP CVWRender3D::HandleActivate(WPARAM wParam)
{
	if (m_prlWinDevice)
		return m_prlWinDevice->HandleActivate(wParam);
	return S_OK;
}

STDMETHODIMP CVWRender3D::HandleSize(LPDIRECTDRAW pdd, LPDIRECTDRAWSURFACE pddsBackBuffer, LPDIRECTDRAWCLIPPER pddClipper, int cx, int cy, DWORD dwDeviceType)
{
	if (dwDeviceType)
		return BuildDevice(pdd, pddsBackBuffer, pddClipper, cx, cy, dwDeviceType); 
	SAFERELEASE(m_prlViewport);
	return CreateViewport(cx, cy);
}

STDMETHODIMP CVWRender3D::HandlePaint(HDC hDC)
{
	if (m_prlWinDevice)
		return m_prlWinDevice->HandlePaint(hDC);
	return S_OK;
}

STDMETHODIMP CVWRender3D::Init(HWND hWnd, LPDIRECTDRAW pdd, LPDIRECT3DRM pd3drm)
{
	HRESULT hr = S_OK;

	m_prlapi = pd3drm;
	m_prlapi->AddRef();
	m_hWnd = hWnd;

	// do any global scene stuff (creates camera, too) && finish initializing DD objects
    if (FAILED(hr = InitializeScene(pdd)))
	{
		return hr;
	}

	return hr;
}

STDMETHODIMP
CVWRender3D::Clear()
{
	if (m_prlViewport)
		return m_prlViewport->Clear();
	return S_OK;
}

STDMETHODIMP CVWRender3D::Render()
{
	static DWORD dwFrameTime = 0;
	DWORD dwTime = 0;
	try 
	{
		// Only move the scene if we're responsible for it.
		dwTime = GetTickCount();
		if (m_bMoveScene)
		{	
			if (dwFrameTime > 0 && dwFrameTime < dwTime)
				m_prlfScene->Move((float)(dwTime-dwFrameTime)/100);
			else
				m_prlfScene->Move(1.0f);
		}
		dwFrameTime = dwTime;
		if (m_prlViewport)
			m_prlViewport->Render( m_prlfScene );
	} 
	catch(...) 
	{
		// If anything fails, just return with error.
		TRACE("Caught exception during D3D Render call.\n");
		return E_FAIL;
	}
	return S_OK;
}

STDMETHODIMP CVWRender3D::Update()
{
	if (m_prlDevice)
		return m_prlDevice->Update();
	return S_OK;
}
	
STDMETHODIMP CVWRender3D::HitTest(VARIANT varHitParams, VARIANT_BOOL vbReturnAllPicked, VARIANT *pvarHitResult)
{
	IDirect3DRMPickedArray	*ppa = NULL;
	IDirect3DRMFrameArray	*pfa = NULL;
	IDirect3DRMVisual		*pvisual = NULL;
	IDirect3DRMFrame		*pframe = NULL;
	IThing					*pThing = NULL;
	CPoint pt;
	HRESULT hr = S_OK;
	IPropertyList*			pPropList = NULL;
	IWorld* pWorld = NULL;
	unsigned long			nNameSize = 64;  //64 is enough because we only really care about comparing the LEAVEMESHQUALITY string
	char					szName[64];
	IDispatch*				idisp = NULL;
	IDispatch*				idispFirst = NULL;
	int						nCount;

	// initialize
	pvarHitResult->vt = VT_EMPTY;

	if (!m_prlViewport)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	// unbundle the point
	if (varHitParams.vt != VT_I4)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	pt.x = (long) LOWORD(varHitParams.lVal);
	pt.y = (long) HIWORD(varHitParams.lVal);

	// see what's under the point
	if (FAILED(hr = m_prlViewport->Pick(pt.x, pt.y, &ppa)))
		goto ERROR_ENCOUNTERED;

	nCount = ppa->GetSize();

	// if nothing, return success, but emptiness
	if (nCount == 0) 
	{
		pvarHitResult->vt = VT_EMPTY;
		goto ERROR_ENCOUNTERED;
	}

	if (vbReturnAllPicked == VARIANT_TRUE)
	{	//Will return a property list of multiple items

		if (nCount > 0)
		{
			//Iterate on the items possibly picked
			for (int i = 0; i < nCount; i++)
			{
				// get the frames and visuals
				if (FAILED(hr = ppa->GetPick(i, &pvisual, &pfa, NULL)))
					goto ERROR_ENCOUNTERED;

				if (pvisual && SUCCEEDED(pvisual->GetName(&nNameSize, (char*) &szName)))
				{
					//If this is the select box it's not a valid selection so skip it.
					if (!strcmp(szName, LEAVEMESHQUALITY))
					{
						SAFERELEASE(pvisual);
						SAFERELEASE(pfa);
						continue;
					}
				}

				idisp = GetVWFrame(pfa);

				//So if we find something the user picked on
				if (idisp != NULL)
				{
					if (pPropList == NULL)
					{	//Create the property list if we haven't already
						hr = ((IObjectProperty*)idisp)->get_World(&pWorld);
						if (FAILED(hr)) goto ERROR_ENCOUNTERED;

						hr = CreatePropertyList(pWorld, &pPropList);
						if (FAILED(hr)) goto ERROR_ENCOUNTERED;

						pvarHitResult->vt = VT_DISPATCH;
						pvarHitResult->pdispVal = pPropList;
						pPropList->AddRef();

						SAFERELEASE(pWorld);
					}

					hr = pPropList->AddObjectProperty((IObjectProperty*)idisp);
					if (FAILED(hr)) goto ERROR_ENCOUNTERED;
				}
				SAFERELEASE(pvisual);
				SAFERELEASE(pfa);
			}
		}
	}
	else //Only pick the first item, the usual case.
	{
		//Iterate on the items possibly picked
		for (int i = 0; i < nCount; i++)
		{
			// get the frames and visuals
			if (FAILED(hr = ppa->GetPick(i, &pvisual, &pfa, NULL)))
				goto ERROR_ENCOUNTERED;

			if (pvisual && SUCCEEDED(pvisual->GetName(&nNameSize, (char*) &szName)))
			{
				//If this is the select box it's not a valid selection so skip it.
				if (!strcmp(szName, LEAVEMESHQUALITY))
				{
					SAFERELEASE(pvisual);
					SAFERELEASE(pfa);
					continue;
				}
			}

			idisp = GetVWFrame(pfa);

			if (idisp)
			{
				pvarHitResult->vt = VT_DISPATCH;
				pvarHitResult->pdispVal = idisp;

				// bump the reference count (extra bump)
				idisp->AddRef();
				break;
			}
			SAFERELEASE(pvisual);
			SAFERELEASE(pfa);
		}
	}


ERROR_ENCOUNTERED:
	
	// release stuff
	SAFERELEASE(pPropList);
	SAFERELEASE(pWorld);
	SAFERELEASE(pframe);
	SAFERELEASE(pvisual);
	SAFERELEASE(pfa);
	SAFERELEASE(ppa);
	SAFERELEASE(pThing);

	return hr;
}

IDispatch* CVWRender3D::GetVWFrame(IDirect3DRMFrameArray	*pfa)
{
	IDirect3DRMFrame		*pframe = NULL;
	IThing					*pThing = NULL;
	int						iframe;
	DWORD					dw = 0;

	// iterate from leaf-most to top-most, looking for a frame with app data on it
	for(iframe = pfa->GetSize()-1; iframe >= 0; iframe--)
	{
		pfa->GetElement(iframe, &pframe);
		ASSERT( pframe );

		if ((dw = pframe->GetAppData()) != 0)
		{
			((IVWFrame*)dw)->get_Thing(&pThing);
			if (pThing)
				goto ERROR_DONE;
		}

		dw = 0;

		// release
		SAFERELEASE(pframe);
		SAFERELEASE(pThing);
	}

ERROR_DONE:
	SAFERELEASE(pframe);
	SAFERELEASE(pThing);

	return (IDispatch*) dw;
}

STDMETHODIMP CVWRender3D::SetFastMode(VARIANT_BOOL bFastMode)
{
	if (bFastMode)
	{
		// Set size of viewport to be 1/2 window size.
		if (m_prlViewport)
			return m_prlViewport->Configure(0, 0, m_prlViewport->GetWidth()/2, m_prlViewport->GetHeight()/2);
	}
	else
	{
		// Set size of viewport to be window size.
		if (m_prlViewport)
			return m_prlViewport->Configure(0, 0, m_prlViewport->GetWidth()*2, m_prlViewport->GetHeight()*2);
	}
	return E_FAIL;
}

STDMETHODIMP CVWRender3D::get_Scene(LPDIRECT3DRMFRAME *ppd3drmf)
{
	if (m_prlfScene)
	{
		*ppd3drmf = m_prlfScene;
		m_prlfScene->AddRef();
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CVWRender3D::put_Scene(LPDIRECT3DRMFRAME pd3drmf)
{
	HRESULT				hr = S_OK;
	POSITION			pos;
	CVWRender3D			*p3DRenderer;
	IDirect3DRMFrame	*p3DScene;
	int					iViewportWidth, iViewportHeight;

	IDirect3DRMFrame *prlfOldScene = m_prlfScene;
	IDirect3DRMFrame *prlfOldCamera = m_prlfCamera;;

	if (pd3drmf)
	{
		// OK, lets see if this scene is being rendered by another renderer.
		// If it is then that other renderer will be responsible for moving the scene.
		m_bMoveScene = TRUE;
		pos = m_list3DRenderers.GetHeadPosition();
		while (pos)
		{
			p3DRenderer = (CVWRender3D*)m_list3DRenderers.GetNext(pos);
			p3DRenderer->get_Scene(&p3DScene);
			if (p3DScene == pd3drmf)
			{
				m_bMoveScene = FALSE;
			}
		}

		m_prlfScene = pd3drmf;
		pd3drmf->AddRef();
		// Create a camera for this scene.
		if (FAILED(hr = m_prlapi->CreateFrame(m_prlfScene, &m_prlfCamera)))
		{
			return hr;
		}
	}
	else
	{
		m_prlfScene = m_prlfEmptyScene;
		m_prlfEmptyScene->AddRef();
		// Make the camera the empty scene camera.
		m_prlfCamera = m_prlfEmptySceneCamera;
		m_prlfEmptySceneCamera->AddRef();

		if (m_bMoveScene)
		{
			// OK, lets see if this scene is (still) being rendered by another renderer.
			// If it is then that other renderer will have to take over moving the scene.
			pos = m_list3DRenderers.GetHeadPosition();
			while (pos)
			{
				p3DRenderer = (CVWRender3D*)m_list3DRenderers.GetNext(pos);
				p3DRenderer->get_Scene(&p3DScene);
				if (p3DScene == prlfOldScene)
				{
					p3DRenderer->m_bMoveScene = TRUE;
				}
			}
		}
		// OK, lets see if the empty scene is being rendered by another renderer.
		// If it is then that other renderer will be responsible for moving the scene.
		m_bMoveScene = TRUE;
		pos = m_list3DRenderers.GetHeadPosition();
		while (pos)
		{
			p3DRenderer = (CVWRender3D*)m_list3DRenderers.GetNext(pos);
			p3DRenderer->get_Scene(&p3DScene);
			if (p3DScene == m_prlfEmptyScene &&
				p3DRenderer != this)
			{
				m_bMoveScene = FALSE;
			}
		}
	}
	// Re-create viewport now we have a different camera.
	if (m_prlViewport)
	{
		m_fField = m_prlViewport->GetField();
		iViewportWidth = m_prlViewport->GetWidth();
		iViewportHeight = m_prlViewport->GetHeight();
		SAFERELEASE(m_prlViewport);
		CreateViewport(iViewportWidth, iViewportHeight);
	}

	// Finally release old scene and camera (wait until here so never in a bad state
	// for render thread).
	prlfOldScene->Release();
	prlfOldCamera->Release();

	return S_OK;
}

STDMETHODIMP CVWRender3D::SetBackgroundColor(BYTE r, BYTE g, BYTE b)
{
	HRESULT hr = S_OK;

	if (m_prlfScene)
		hr = m_prlfScene->SetSceneBackground(D3DRMCreateColorRGB(r / 255.0F, g / 255.0F, b / 255.0F));

	return hr;
}


STDMETHODIMP CVWRender3D::get_Direct3DRM (LPDIRECT3DRM *ppd3drm)
{
	if (m_prlapi)
	{
		*ppd3drm = m_prlapi;
		m_prlapi->AddRef();
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CVWRender3D::get_Camera (LPDIRECT3DRMFRAME *ppd3drm)
{
	if (m_prlfCamera)
	{
		*ppd3drm = m_prlfCamera;
		m_prlfCamera->AddRef();
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CVWRender3D::get_Viewport (LPDIRECT3DRMVIEWPORT *ppd3drmVP)
{
	if(!ppd3drmVP)
		return E_POINTER;

	if (m_prlViewport)
	{
		*ppd3drmVP = m_prlViewport;
		m_prlViewport->AddRef();
		return S_OK;
	}
	return E_FAIL;
}

HRESULT
CVWRender3D::BuildDevice(LPDIRECTDRAW lpdd, LPDIRECTDRAWSURFACE pddsBackBuffer, LPDIRECTDRAWCLIPPER pddClipper, int cx, int cy, DWORD dwDeviceType)
{
	IDirect3DRMDevice *prlDevice = NULL;
	HRESULT hr;

	// Has a device been created yet?
	if (m_prlDevice != NULL)
	{
		if (m_prlViewport)
			m_fField = m_prlViewport->GetField();
		SAFERELEASE(m_prlViewport);
		SAFERELEASE(m_prlDevice);
		SAFERELEASE(m_prlWinDevice);
	}

	// Create a new device and set parms.
	if (FAILED(hr = CreateDevice(lpdd, pddsBackBuffer, pddClipper, cx, cy, dwDeviceType, &m_prlDevice)))
		return hr;

	if (FAILED(hr = m_prlDevice->QueryInterface(IID_IDirect3DRMWinDevice, (void**)&m_prlWinDevice)))
		return hr;

	return CreateViewport(cx, cy);
}

HRESULT
CVWRender3D::ReleaseDevice()
{
	if (m_prlViewport)
		m_fField = m_prlViewport->GetField();
	SAFERELEASE(m_prlViewport);
	SAFERELEASE(m_prlDevice);
	SAFERELEASE(m_prlWinDevice);
	return S_OK;
}

HRESULT CVWRender3D::CreateViewport(int cx, int cy)
{
	HRESULT hRes = S_OK;

	if (!m_prlDevice || !m_prlfCamera)
	{
		return E_FAIL;
	}

    if (FAILED(hRes = m_prlapi->CreateViewport(m_prlDevice, m_prlfCamera,
											   0, 0, cx, cy, &m_prlViewport)))
	{
		return hRes;
	}

	if (!m_prlViewport)
		return E_FAIL;

	if (FAILED(hRes = m_prlViewport->SetBack(D3DVALUE(fBackClippingPlane))))
	{
		return hRes;
	}

	if (FAILED(hRes = m_prlViewport->SetFront(D3DVALUE(fFrontClippingPlane))))
	{
		return hRes;
	}

	hRes = m_prlViewport->SetField(D3DVALUE(m_fField));
	if (FAILED(hRes))
		return hRes;

	return S_OK;
}

LPGUID CVWRender3D::FindDevice(LPDIRECTDRAW lpdd)
{
    LPDIRECT3D			lpD3D;
	LPGUID				lpGUIDRet = NULL;

	// Get the Direct3D object from the direct draw.
    if (lpdd->QueryInterface(IID_IDirect3D, (void**) &lpD3D))
	{
		ASSERT(FALSE);
        return NULL;
	}

    // Enumerate the drivers.
    if (FAILED(lpD3D->EnumDevices(D3DEnumDeviceCallBack, &m_DeviceInfo)))
        return NULL;

	// If we don't find the kind of device requested, we'll just get the 
	// default device (NULL).
	if (m_DeviceInfo.bFindHWDevice && m_DeviceInfo.lpHWGuid)
        lpGUIDRet = m_DeviceInfo.lpHWGuid;
	else if (!m_DeviceInfo.bFindHWDevice)
		lpGUIDRet = m_DeviceInfo.lpSWGuid;

	// Don't need this for anything else.
	lpD3D->Release();

	return lpGUIDRet;
}

HRESULT
CVWRender3D::CreateDevice(LPDIRECTDRAW lpdd, LPDIRECTDRAWSURFACE lpDDSurfaceBack, LPDIRECTDRAWCLIPPER pddClipper, int cx, int cy, DWORD dwDeviceType, IDirect3DRMDevice **pprlDevice)
{
	LPGUID	lpGUID;
	HRESULT hRes = S_OK;
	
	try
	{
		// Reset the device info used to enumerate the devices.
		memset(&m_DeviceInfo, 0, sizeof(D3DDeviceInfo));
		if (dwDeviceType == 2)
		{
			// We want a HW device - select RGB color model.
			m_DeviceInfo.bFindHWDevice = TRUE;
			m_DeviceInfo.cm = D3DCOLOR_RGB;
		}
		else
		{
			// We want a SW device of the required color model.
			m_DeviceInfo.bFindHWDevice = FALSE;
  			if (!m_d3dColorModel)
				m_DeviceInfo.cm = D3DCOLOR_MONO;
			else
				m_DeviceInfo.cm = m_d3dColorModel;
		}
		if (!(lpGUID = FindDevice(lpdd)))
			return E_FAIL;
		if (lpDDSurfaceBack)
		{
			//query to see if it has a IDirect3DRM2 interface. If so, create the
			//device with that.
#ifdef IDirect3D2_CreateDevice 
			IDirect3DRM2 *rm2;
			hRes = m_prlapi->QueryInterface(IID_IDirect3DRM2, (void**)&rm2);
			if (hRes == D3DRM_OK) {
				TestHRESULT(rm2->CreateDeviceFromSurface(lpGUID,
															lpdd,
															lpDDSurfaceBack, 
															(IDirect3DRMDevice2**)pprlDevice));
				rm2->Release();
			}
			else 
#endif
				TestHRESULT(m_prlapi->CreateDeviceFromSurface(lpGUID,
																lpdd,
																lpDDSurfaceBack, 
																pprlDevice));
		}
		else
		{
			TestHRESULT(m_prlapi->CreateDeviceFromClipper(pddClipper,
														  lpGUID,
														  cx,
														  cy,
														  pprlDevice));
		}
	}
	catch(CHRESULTException chr)
	{
		return chr.GetHRESULT();
	}

	if (m_DeviceInfo.lpHWGuid)
	{
		if (m_DeviceInfo.HWDeviceDesc.dpcTriCaps.dwShadeCaps & D3DPSHADECAPS_COLORPHONGRGB)
			(*pprlDevice)->SetQuality(D3DRMRENDER_PHONG);
		else if (m_DeviceInfo.HWDeviceDesc.dpcTriCaps.dwShadeCaps & D3DPSHADECAPS_COLORGOURAUDRGB)
			(*pprlDevice)->SetQuality(D3DRMRENDER_GOURAUD);
		else if (m_DeviceInfo.HWDeviceDesc.dpcTriCaps.dwShadeCaps & D3DPSHADECAPS_COLORFLATRGB)
			(*pprlDevice)->SetQuality(D3DRMRENDER_FLAT);
		else
			(*pprlDevice)->SetQuality(D3DRMRENDER_UNLITFLAT);
		if (m_DeviceInfo.HWDeviceDesc.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_DITHER)
		    (*pprlDevice)->SetDither(TRUE);
		else
		    (*pprlDevice)->SetDither(FALSE);
// Having problems with Voodoo driver doing bilinear filter w transparency.
//		if (m_DeviceInfo.HWDeviceDesc.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_LINEAR)
//			(*pprlDevice)->SetTextureQuality(D3DRMTEXTURE_LINEAR);
//		else
			(*pprlDevice)->SetTextureQuality(D3DRMTEXTURE_NEAREST);
	} 
	else
	{
		// max out at phong shading
		(*pprlDevice)->SetQuality(D3DRMRENDER_PHONG);
	    (*pprlDevice)->SetDither(FALSE);
		(*pprlDevice)->SetTextureQuality(D3DRMTEXTURE_NEAREST);
	}
	m_d3dColorModel = m_DeviceInfo.cm;

	return S_OK;
}

HRESULT CVWRender3D::InitializeScene(LPDIRECTDRAW pdd)
{
	IDirect3DRMFrame		*prlfLogo = NULL;
	POSITION				pos;
	CVWRender3D				*p3DRenderer;
	IDirect3DRMFrame		*p3DScene;
	IDirect3DRMVisual		*pVisual = NULL;
	HRESULT					hr = S_OK;

	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

	// create empty scene frame
	if (FAILED(hr = m_prlapi->CreateFrame( NULL, &m_prlfEmptyScene )))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_prlfEmptyScene->SetSceneBackground(D3DRMCreateColorRGB(0.5F, 0.5F, 0.5F))))
		goto ERROR_ENCOUNTERED;

	// create a frame for the logo
	if (FAILED(hr = m_prlapi->CreateFrame(m_prlfEmptyScene, &prlfLogo)))
		goto ERROR_ENCOUNTERED;

	// set rotation and position
	if (FAILED(hr = prlfLogo->SetRotation(m_prlfEmptyScene, D3DVALUE(-1.0), D3DVALUE(1.0), D3DVALUE(0.0), D3DVALUE(-0.02))))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = prlfLogo->SetPosition(m_prlfEmptyScene, D3DVALUE(0.0), D3DVALUE(0.0), D3DVALUE(10.0))))
		goto ERROR_ENCOUNTERED;

	// Create the V-Worlds cube.
	if (SUCCEEDED(CreateLogoVisual(pdd, &pVisual)))
	{
		// Add visual to frame
		prlfLogo->AddVisual(pVisual);
	}

	// create camera
	if (FAILED(hr = m_prlapi->CreateFrame(m_prlfScene, &m_prlfEmptySceneCamera)))
		goto ERROR_ENCOUNTERED;
	// set its position
	if (FAILED(hr = m_prlfEmptySceneCamera->SetPosition(m_prlfScene, D3DVALUE(0.0), D3DVALUE(0.0), D3DVALUE(0.0))))
		goto ERROR_ENCOUNTERED;

	// OK, lets see if the empty scene is being rendered by another renderer.
	// If it is then that other renderer will be responsible for moving the scene.
	m_bMoveScene = TRUE;
	pos = m_list3DRenderers.GetHeadPosition();
	while (pos)
	{
		p3DRenderer = (CVWRender3D*)m_list3DRenderers.GetNext(pos);
		p3DRenderer->get_Scene(&p3DScene);
		if (p3DScene == m_prlfEmptyScene)
		{
			m_bMoveScene = FALSE;
		}
	}

	// Make the rendered scene the empty scene initially.
	m_prlfScene = m_prlfEmptyScene;
	m_prlfEmptyScene->AddRef();
	m_prlfCamera = m_prlfEmptySceneCamera;
	m_prlfEmptySceneCamera->AddRef();

ERROR_ENCOUNTERED:

	SAFERELEASE(prlfLogo);
	SAFERELEASE(pVisual);

	return hr;
}

HRESULT CVWRender3D::CreateLogoVisual(LPDIRECTDRAW pdd, LPDIRECT3DRMVISUAL *ppVisual)
{
	IDirect3DRMMeshBuilder	*pbuilder = NULL;
	IDirect3DRMFaceArray	*pFaceArray = NULL;
	IDirect3DRMFace			*pFace = NULL;
	D3DVECTOR				vVertices[] = {{-1.0, -1.0, -1.0}, {-1.0, 1.0, -1.0},
											{1.0, 1.0, -1.0}, {1.0, -1.0, -1.0},
											{1.0, -1.0, -1.0}, {1.0, 1.0, -1.0},
											{1.0, 1.0, 1.0}, {1.0, -1.0, 1.0},
											{1.0, -1.0, 1.0}, {1.0, 1.0, 1.0},
											{-1.0, 1.0, 1.0}, {-1.0, -1.0, 1.0},
											{-1.0, -1.0, 1.0}, {-1.0, 1.0, 1.0},
											{-1.0, 1.0, -1.0}, {-1.0, -1.0, -1.0},
											{-1.0, 1.0, -1.0}, {-1.0, 1.0, 1.0},
											{1.0, 1.0, 1.0}, {1.0, 1.0, -1.0},
											{1.0, -1.0, -1.0}, {1.0, -1.0, 1.0},
											{-1.0, -1.0, 1.0}, {-1.0, -1.0, -1.0}};
	D3DVECTOR				vNormals[] = {{0.0, 0.0, -1.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0},
											{-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, -1.0, 0.0}};
	DWORD					dwFaceData[] = {4, 0, 0, 1, 0, 2, 0, 3, 0,
											4, 4, 1, 5, 1, 6, 1, 7, 1,
											4, 8, 2, 9, 2, 10, 2, 11, 2,
											4, 12, 3, 13, 3, 14, 3, 15, 3,
											4, 16, 4, 17, 4, 18, 4, 19, 4,
											4, 20, 5, 21, 5, 22, 5, 23, 5, 0};
	DWORD					dwFaceResourceIDs[] = {IDB_VWGLOGO, 
		                                           IDB_VWGLOGO2, 
												   IDB_VWGLOGO,
												   IDB_VWGLOGO2,
												   IDB_VWGLOGO3,
												   IDB_VWGLOGO3};
	int						i;
	HRESULT					hr = S_OK;

	// load visuals for logo
	if (FAILED(hr = m_prlapi->CreateMeshBuilder(&pbuilder)))
		goto ERROR_ENCOUNTERED;

	// Create vertices and faces for the cube.
	if (FAILED(pbuilder->AddFaces(24, (D3DVECTOR*)&vVertices, 6, (D3DVECTOR*)&vNormals, (DWORD*)&dwFaceData, &pFaceArray)))
		goto ERROR_ENCOUNTERED;
	pbuilder->SetQuality(D3DRMRENDER_UNLITFLAT);
	for (i = 0; i < 6; i++)
	{
		if (FAILED(hr = pFaceArray->GetElement(i, &pFace)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = pFace->SetTextureCoordinates(0,  0.0, 0.0)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = pFace->SetTextureCoordinates(1,  0.0, 1.0)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = pFace->SetTextureCoordinates(2,  1.0, 1.0)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = pFace->SetTextureCoordinates(3,  1.0, 0.0)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = ApplyBitmapResourceToFace(pdd, dwFaceResourceIDs[i], pFace)))
			goto ERROR_ENCOUNTERED;
		SAFERELEASE(pFace);
	}

	*ppVisual = pbuilder;
	pbuilder->AddRef();

ERROR_ENCOUNTERED:

	SAFERELEASE(pbuilder);
	SAFERELEASE(pFaceArray);
	SAFERELEASE(pFace);

	return hr;
}

HRESULT CVWRender3D::ApplyBitmapResourceToFace(LPDIRECTDRAW pdd, UINT nIDResource, LPDIRECT3DRMFACE pFace)
{
	IDirect3DRMTexture	*pd3drmTexture = NULL;
	CBitmap				bmpFace;
	HRESULT				hr = S_OK;

	if (!bmpFace.LoadBitmap(nIDResource))
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = CreateTextureFromBitmap(pdd, bmpFace, &pd3drmTexture)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pFace->SetColor(0xffffffff)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pFace->SetTexture(pd3drmTexture)))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:

	SAFERELEASE(pd3drmTexture);

	return hr;
}

HRESULT CVWRender3D::CreateTextureFromBitmap(LPDIRECTDRAW pdd, CBitmap &bmp, LPDIRECT3DRMTEXTURE *ppTexture)
{
	LPDIRECTDRAWSURFACE	pddsBuffer = NULL;
	HDC					hDCDDS, hDC;
	BITMAP				bitmap;
	char				*pbits = NULL;
	BITMAPINFO			*pbmi = NULL;
	PALETTEENTRY		pPalEntries[256];
	int					i;
	HRESULT				hr = S_OK;

	if (!bmp.GetBitmap(&bitmap))
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	pbits = (char*)malloc(bitmap.bmWidthBytes*bitmap.bmHeight);
	if (bmp.GetBitmapBits(bitmap.bmWidthBytes*bitmap.bmHeight, pbits) != (DWORD)(bitmap.bmWidthBytes*bitmap.bmHeight))
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	if (FAILED(hr = CreateOffScreenSurface(pdd, &pddsBuffer, bitmap.bmWidth, bitmap.bmHeight)))
		goto ERROR_ENCOUNTERED;

	pddsBuffer->GetDC(&hDCDDS);
	if (bitmap.bmBitsPixel == 8)
		pbmi = (BITMAPINFO*) malloc(sizeof(BITMAPINFOHEADER) + (256 * sizeof(RGBQUAD)));
	else
		pbmi = (BITMAPINFO*) malloc(sizeof(BITMAPINFOHEADER));
	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = bitmap.bmWidth;
	pbmi->bmiHeader.biHeight = bitmap.bmHeight;
	pbmi->bmiHeader.biPlanes = 1;
	pbmi->bmiHeader.biBitCount = bitmap.bmBitsPixel;
	pbmi->bmiHeader.biCompression = BI_RGB;
	pbmi->bmiHeader.biSizeImage = 0;
	pbmi->bmiHeader.biXPelsPerMeter = 0;
	pbmi->bmiHeader.biYPelsPerMeter = 0;
	pbmi->bmiHeader.biClrUsed = 0;
	pbmi->bmiHeader.biClrImportant = 0;
	if (bitmap.bmBitsPixel == 8)
	{
		// We're in palettized mode.
		hDC = GetDC(NULL);
		GetSystemPaletteEntries(hDC, 0, 256, pPalEntries);
		ReleaseDC(NULL, hDC);
		for (i = 0; i < 256; i++)
		{
			pbmi->bmiColors[i]. rgbBlue = pPalEntries[i].peBlue;
			pbmi->bmiColors[i]. rgbGreen = pPalEntries[i].peGreen;
			pbmi->bmiColors[i]. rgbRed = pPalEntries[i].peRed;
			pbmi->bmiColors[i]. rgbReserved = 0;
		}
		StretchDIBits(hDCDDS,
					  0, 0,
					  bitmap.bmWidth, bitmap.bmHeight,
					  0, 0,
					  bitmap.bmWidth, bitmap.bmHeight,
					  pbits,
					  pbmi,
					  DIB_RGB_COLORS,
					  SRCCOPY);
	}
	else
	{
		StretchDIBits(hDCDDS,
					  0, 0,
					  bitmap.bmWidth, bitmap.bmHeight,
					  0, 0,
					  bitmap.bmWidth, bitmap.bmHeight,
					  pbits,
					  pbmi,
					  0,
					  SRCCOPY);
	}
	pddsBuffer->ReleaseDC(hDCDDS);

	// Create a texture from that surface.
	if (FAILED(hr = m_prlapi->CreateTextureFromSurface(pddsBuffer, ppTexture)))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:

	if (pbits)
		free(pbits);
	if (pbmi)
		free(pbmi);
	SAFERELEASE(pddsBuffer);

	return hr;
}

HRESULT CVWRender3D::CreateOffScreenSurface(LPDIRECTDRAW pdd, LPDIRECTDRAWSURFACE *ppddsOffScreen, DWORD dwWidth, DWORD dwHeight)
{
	IDirectDrawSurface	*pddsGDI = NULL;
	DDPIXELFORMAT		ddpf;
	DDSURFACEDESC		ddsd;
	HRESULT hr;

	// Are we in palettized mode?
	memset(&ddpf,0,sizeof(DDPIXELFORMAT));
    ddpf.dwSize = sizeof(ddpf);
	if (SUCCEEDED(pdd->GetGDISurface(&pddsGDI)))
	{
		pddsGDI->GetPixelFormat(&ddpf);
		pddsGDI->Release();
		if (ddpf.dwRGBBitCount == 8 && !m_pPalette)
		{
			// We're palettized.
			if (FAILED(hr = CreatePalette(pdd)))
			{
				ASSERT(FALSE);
				return hr;
			}
		}
	}

	// Create an offscreen surface.
    memset(&ddsd,0,sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.dwWidth = dwWidth;
	ddsd.dwHeight = dwHeight;
	ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY;
	if (FAILED(hr = pdd->CreateSurface( &ddsd, ppddsOffScreen, NULL )))
	{
		ASSERT(FALSE);
		return hr;
	}

	// Attach the palette if one exists
	if (m_pPalette && FAILED(hr = (*ppddsOffScreen)->SetPalette(m_pPalette)))
		return hr;

	return S_OK;
}

HRESULT CVWRender3D::CreatePalette(LPDIRECTDRAW pdd)
{
	PALETTEENTRY rgPE[256];
	ASSERT(m_pPalette == NULL);
	HDC hDC = GetDC(NULL);
	GetSystemPaletteEntries(hDC, 0, 256, rgPE);
	ReleaseDC(NULL, hDC);

	// lock down the palette entries so that D3D does not try to modify them...
	for (int i = 0; i < 256; i++) rgPE[i].peFlags = D3DPAL_READONLY;

	return 	pdd->CreatePalette(DDPCAPS_8BIT | DDPCAPS_INITIALIZE, rgPE, &m_pPalette, NULL);
}

STDMETHODIMP CVWRender3D::put_ColorModel(int d3drmColorModel)
{
	m_d3dColorModel = (D3DCOLORMODEL)d3drmColorModel;
	return S_OK;
}

STDMETHODIMP CVWRender3D::get_ColorModel(int *pd3drmColorModel)
{
	if (!m_d3dColorModel)
		return E_PENDING;
	*pd3drmColorModel = (int)m_d3dColorModel;
	return S_OK;
}

STDMETHODIMP CVWRender3D::get_DeviceWidth(DWORD *pdwDeviceWidth)
{
	if (!m_prlDevice)
		*pdwDeviceWidth = 0;
	else
		*pdwDeviceWidth = m_prlDevice->GetWidth();
	return S_OK;
}

STDMETHODIMP CVWRender3D::get_DeviceHeight(DWORD *pdwDeviceHeight)
{
	if (!m_prlDevice)
		*pdwDeviceHeight = 0;
	else
		*pdwDeviceHeight = m_prlDevice->GetHeight();
	return S_OK;
}

#ifdef _DEBUG

void DumpGraphics(IDirect3DRM *prlapi)
{
	// show all the objects that are left
	OutputDebugString("\r\nList of graphical objects:\r\n");
	prlapi->EnumerateObjects( RMObjEnumFunc, NULL);
}

void RMObjEnumFunc( LPDIRECT3DRMOBJECT obj, LPVOID arg)
{
	DWORD dw;
	char szClass[64];
	char szName[64];
	char sz[256];


	// get class name and name
	dw = 64;
	obj->GetClassName( &dw, szClass );
	szClass[dw] = '\0';

#ifdef NAMEBUGFIXED
	dw = 64;
	obj->GetName( &dw, szName );
	szName[dw] = '\0';
#else
	szName[0] = '\0';
#endif

	// display
	wsprintf(sz, "Object: 0x%0X, Type: %s, Name: %s\r\n", obj, szClass, szName );
	OutputDebugString( sz );

	// see if it's a frame
	if (strcmp(szClass, "Frame") == 0) {

		// yep, turn it into one
		IDirect3DRMFrame *prlf;
		VERIFY( SUCCEEDED(obj->QueryInterface(IID_IDirect3DRMFrame, (void **) &prlf)) );

		// get parent frame
		IDirect3DRMFrame *prlfParent;
		VERIFY( SUCCEEDED(prlf->GetParent(&prlfParent)) );

		// display
		wsprintf(sz, "     Frame parent: 0x%0X\r\n", prlfParent );
		OutputDebugString( sz );

		// Release
		if (prlfParent != NULL )
			prlfParent->Release();
		prlf->Release();
	}

}

#endif
 
void CVWRender3D::CalculateFPS(DWORD dw2DClearTime, DWORD dw3DRenderTime, DWORD dw2DRenderTime, DWORD dwBltTime)
{
	static DWORD dwTimeMark = 0;
	static UINT cFrames = 0;
	static char sz[200];
	static DWORD dwTotal2DClearTime = 0;
	static DWORD dwTotal3DRenderTime = 0;
	static DWORD dwTotal2DRenderTime = 0;
	static DWORD dwTotalBltTime = 0;
	DWORD dwDelta = GetTickCount() - dwTimeMark;
	dwTotal2DClearTime += dw2DClearTime;
	dwTotal3DRenderTime += dw3DRenderTime;
	dwTotal2DRenderTime += dw2DRenderTime;
	dwTotalBltTime += dwBltTime;
	float fFrames, fTotal2DClearTime, fTotal3DRenderTime, fTotal2DRenderTime, fTotalBltTime;

	cFrames++;

	if (dwDelta > dwRefreshFPS) {
		// update the FPS string
		fFrames = 1000.f * float(cFrames);
		fTotal2DClearTime = float(dwTotal2DClearTime)/fFrames;
		fTotal3DRenderTime = float(dwTotal3DRenderTime)/fFrames;
		fTotal2DRenderTime = float(dwTotal2DRenderTime)/fFrames;
		fTotalBltTime = float(dwTotalBltTime)/fFrames;
		sprintf(sz, "FPS: %5.2f, 2DClear: %5.5f, 3DRender: %5.5f, 2DRender: %5.5f, Blt/Update: %5.5f", fFrames/float(dwDelta), fTotal2DClearTime, fTotal3DRenderTime, fTotal2DRenderTime, fTotalBltTime);
		dwTimeMark += dwDelta;
		dwTotal2DClearTime = 0;
		dwTotal3DRenderTime = 0;
		dwTotal2DRenderTime = 0;
		dwTotalBltTime = 0;
		cFrames = 0;
	}

	if (m_hWnd)
	{
		// write the string to the viewing window
		HDC hDC = GetDC(m_hWnd);
		CString	str(sz);
		int iCount = str.GetLength();
		TextOut(hDC, 2, 2, sz, iCount);
		ReleaseDC(m_hWnd, hDC);
	}
}
