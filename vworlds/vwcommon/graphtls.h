// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// GraphTls.h : Helper file to create graphics tools.

#include <d3drmwin.h>
#include "vwobject.h"
#include <vwgcache.h>

// Function: CreateDDPalette
//    This function creates a Palette that will be used by DirectDraw if we are
//  not in RGB mode.
HRESULT CreateDDPalette(IDirectDraw *pDD, LPDIRECTDRAWPALETTE *ppPalette)
{
	HDC		hDC;
	HWND	hWnd;
	PALETTEENTRY rgPE[256];

	hWnd = GetDesktopWindow();
	hDC = GetDC(hWnd);
	GetSystemPaletteEntries(hDC, 0, 256, rgPE);
	ReleaseDC(hWnd, hDC);

	// fill palette entry flags
    for (int i = 0; i < 10; i++) rgPE[i].peFlags = D3DPAL_READONLY;
    for (i = 10; i < 256 - 10; i++) rgPE[i].peFlags = D3DPAL_FREE | PC_RESERVED;
    for (i = 256 - 10; i < 256; i++) rgPE[i].peFlags = D3DPAL_READONLY;
	
	return pDD->CreatePalette(DDPCAPS_8BIT | DDPCAPS_INITIALIZE, rgPE, ppPalette, NULL);
}

HRESULT CreateGraphicsTools(IWorld *pWorld, 
							IDirectDraw **ppDD, 
							IDirect2DRM **ppD2DRM,
							IDirect3DRM **ppD3DRM, 
							IVWGeometryCache **ppVWGCache)
{
	CComPtr<IPropertyMap> pTools;
	VARIANT_BOOL bValid;
	HRESULT hr = S_OK;

	// Right,have the DirectDraw, Direct3DRM, Direct2DRM and VWGeometryCache objects been 
	// created by another OCX yet?

	// NOTE: If the in parameter is NULL, the tool is assumed not to be needed. If the
	// object pointed to be the in parameter is NULL the object will be created and 
	// placed as a tool in the world. If the object pointed to by the in parameter 
	// is NULL the object will (if necessary) just be placed as a tool in the world.

	if (ppDD)
	{
		if (!*ppDD)
		{
			if (SUCCEEDED(hr = ((IWorld*)pWorld)->IsValidTool(CComBSTR("DirectDraw"), &bValid)) &&
				bValid)
			{
				((IWorld*)pWorld)->get_Tools(&pTools);
				pTools->get_Long(CComBSTR("DirectDrawPointer"), (long*)ppDD);
				(*ppDD)->AddRef();
				pTools.Release();
			}
			else
			{
				if(FAILED(hr = ::DirectDrawCreate(NULL, ppDD, NULL))) goto ERROR_ENCOUNTERED;
				if (FAILED(hr = (*ppDD)->SetCooperativeLevel(NULL, DDSCL_NORMAL))) goto ERROR_ENCOUNTERED;
				((IWorld*)pWorld)->CreateToolFromInterface(CComBSTR("DirectDraw"), *ppDD);
				((IWorld*)pWorld)->get_Tools(&pTools);
				pTools->put_Long(CComBSTR("DirectDrawPointer"), (long)*ppDD);
				pTools.Release();
			}
		}
		else
		{
			if (FAILED(hr = ((IWorld*)pWorld)->IsValidTool(CComBSTR("DirectDraw"), &bValid)) ||
				!bValid)
			{
				((IWorld*)pWorld)->CreateToolFromInterface(CComBSTR("DirectDraw"), *ppDD);
				((IWorld*)pWorld)->get_Tools(&pTools);
				pTools->put_Long(CComBSTR("DirectDrawPointer"), (long)*ppDD);
				pTools.Release();
			}
		}
	}
	if (ppD3DRM)
	{
		if (!*ppD3DRM)
		{
			if (SUCCEEDED(hr = ((IWorld*)pWorld)->IsValidTool(CComBSTR("Direct3DRM"), &bValid)) && 
				bValid)
				((IWorld*)pWorld)->get_Tool(CComBSTR("Direct3DRM"), (IUnknown**)ppD3DRM);
			else
			{
				if(FAILED(hr = ::Direct3DRMCreate(ppD3DRM))) goto ERROR_ENCOUNTERED;
				((IWorld*)pWorld)->CreateToolFromInterface(CComBSTR("Direct3DRM"), *ppD3DRM);
			}
		}
		else
		{
			if (FAILED(hr = ((IWorld*)pWorld)->IsValidTool(CComBSTR("Direct3DRM"), &bValid)) ||
				!bValid)
				((IWorld*)pWorld)->CreateToolFromInterface(CComBSTR("Direct3DRM"), *ppD3DRM);
		}
	}
	if (ppD2DRM)
	{
		if (!ppDD)
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		if (!*ppD2DRM)
		{
			if (SUCCEEDED(hr = ((IWorld*)pWorld)->IsValidTool(CComBSTR("Direct2DRM"), &bValid)) && 
				bValid)
			{
				((IWorld*)pWorld)->get_Tools(&pTools);
				pTools->get_Long(CComBSTR("Direct2DRMPointer"), (long*)ppD2DRM);
				(*ppD2DRM)->AddRef();
				pTools.Release();
			}
			else
			{
				if (FAILED(hr = ((IWorld*)pWorld)->CreateTool(CComBSTR("Direct2DRM"), CComBSTR("D2D.Direct2DRM.1"), (IUnknown**)ppD2DRM)))
					goto ERROR_ENCOUNTERED;
				if (FAILED(hr = (*ppD2DRM)->Init(*ppDD)))
					goto ERROR_ENCOUNTERED;
				((IWorld*)pWorld)->get_Tools(&pTools);
				pTools->put_Long(CComBSTR("Direct2DRMPointer"), (long)*ppD2DRM);
				pTools.Release();
			}
		}
		else
		{
			if (FAILED(hr = ((IWorld*)pWorld)->IsValidTool(CComBSTR("Direct2DRM"), &bValid)) ||
				!bValid)
			{
				((IWorld*)pWorld)->CreateToolFromInterface(CComBSTR("Direct2DRM"), *ppD2DRM);
				((IWorld*)pWorld)->get_Tools(&pTools);
				pTools->put_Long(CComBSTR("Direct2DRMPointer"), (long)*ppD2DRM);
				pTools.Release();
			}
		}
	}
	if (ppVWGCache)
	{
		if (!*ppVWGCache)
		{
			if (SUCCEEDED(hr = ((IWorld*)pWorld)->IsValidTool(CComBSTR("VWGeometryCache"), &bValid)) && 
				bValid)
				((IWorld*)pWorld)->get_Tool(CComBSTR("VWGeometryCache"), (IUnknown**)ppVWGCache);
			else
			{
				if (FAILED(hr = ((IWorld*)pWorld)->CreateTool(CComBSTR("VWGeometryCache"), CComBSTR("VWSYSTEM.GeometryCache.1"), (IUnknown**)ppVWGCache)))
					goto ERROR_ENCOUNTERED;
				if (FAILED(hr = (*ppVWGCache)->Init((IWorld *) pWorld)))
					goto ERROR_ENCOUNTERED;
				if (FAILED(hr = (*ppVWGCache)->SetGeomCacheSize(200)))
					goto ERROR_ENCOUNTERED;
			}
		}
		else
		{
			if (FAILED(hr = ((IWorld*)pWorld)->IsValidTool(CComBSTR("VWGeometryCache"), &bValid)) ||
				!bValid)
				((IWorld*)pWorld)->CreateToolFromInterface(CComBSTR("VWGeometryCache"), *ppVWGCache);
			if (FAILED(hr = (*ppVWGCache)->Init((IWorld *) pWorld)))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:

	return hr;	
}

HRESULT CreateFrontBuffer(IDirectDraw *pDD, 
						  IDirect2DRM *pD2DRM,
						  IDirectDrawSurface **ppFrontBuffer) 
{
	DDPIXELFORMAT ddpfTarget;
	LPDIRECTDRAWPALETTE pPalette = NULL;
	HRESULT hr = S_OK;
	
	if (!pDD || !ppFrontBuffer)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	
	// Get the primary surface or create it if necessary.
	if (FAILED(hr = pDD->GetGDISurface(ppFrontBuffer)))
	{
		if (hr == DDERR_NOTFOUND)
		{
			DDSURFACEDESC ddsd;
			memset(&ddsd,0,sizeof(DDSURFACEDESC));
			ddsd.dwSize = sizeof( ddsd );
			ddsd.dwFlags = DDSD_CAPS;
			ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_3DDEVICE;
			if (FAILED(hr = pDD->CreateSurface(&ddsd, ppFrontBuffer, NULL)))
				return hr;
		}
		else
			goto ERROR_ENCOUNTERED;
	}

	// palettize if we are not in RGB mode
	memset(&ddpfTarget,0,sizeof(DDPIXELFORMAT));
    ddpfTarget.dwSize = sizeof(ddpfTarget);
	(*ppFrontBuffer)->GetPixelFormat(&ddpfTarget);
	if (ddpfTarget.dwRGBBitCount <= 8)
	{
		if (FAILED(hr = CreateDDPalette(pDD, &pPalette)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = (*ppFrontBuffer)->SetPalette(pPalette)))
			goto ERROR_ENCOUNTERED;
	}

	if (pD2DRM)
	{
		if (FAILED(hr = pD2DRM->SetVideoMode(&ddpfTarget, pPalette)))
			goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:

	SAFERELEASE(pPalette);
	return hr;
}


HRESULT CreateClipper(IDirectDraw *pDD, HWND hWnd, IDirectDrawClipper **ppClipper)
{
	HRESULT hr = S_OK;
	
	if (!hWnd || !ppClipper)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	
	// set up the window clipper
	if (FAILED(hr = pDD->CreateClipper(0, ppClipper, NULL)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = (*ppClipper)->SetHWnd(0, hWnd)))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:

	return hr;
}