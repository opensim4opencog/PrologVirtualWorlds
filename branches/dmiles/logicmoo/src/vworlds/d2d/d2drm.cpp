// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// D2DRM.cpp : Implementation of Cdirect2d and DLL registration.

#include "stdafx.h"
#include "D2DRM.h"

/////////////////////////////////////////////////////////////////////////////
//

CDirect2DRM::CDirect2DRM()
{
	m_pDD = NULL;
	m_pImageCache = NULL;
	m_pDefFont = NULL;
	InitializeCriticalSection(&m_CSTickedList);
}

CDirect2DRM::~CDirect2DRM()
{
	TRACE("CDirect2DRM::~CDirect2DRM()\n");

	// It is possible for there to be textures left on the texture list (and therefore
	// images left on the ticked list) when we quit if textures are added to the list 
	// after the thread calling ReleaseTextures() exits.
	ReleaseTextures();
	MMASSERT(m_listTextures.GetCount() == 0);
	MMASSERT(m_listTicked.GetCount() == 0);

	MMRELEASE(m_pDD);
	if (m_pImageCache) {
		m_pImageCache->Reset();
		m_pImageCache->Release();
	}

	MMRELEASE(m_pDefFont);
	DeleteCriticalSection(&m_CSTickedList);
}

STDMETHODIMP
CDirect2DRM::Init(LPDIRECTDRAW pDD)
{
	if (m_pDD != NULL)
		return E_ALREADYINITIALIZED;

	if (m_pDD = pDD) {
		m_pDD->AddRef();
		m_pImageCache = new CImageCache();
		if (m_pImageCache == NULL)
			return E_OUTOFMEMORY;
		m_pImageCache->Init(pDD);
		return S_OK;
	} else {
		return E_INVALIDOBJECT;
	}
}


STDMETHODIMP 
CDirect2DRM::CreateDeviceFromSurface(LPGUID pGUID, 
									 LPDIRECTDRAWSURFACE pddsBackBuffer,
									 LPDIRECT2DRMDEVICE *ppDevice)
{
	HRESULT hr;
	LPDIRECT2DRMDEVICE pDevice = NULL;

	hr = CoCreateInstance(
		CLSID_CDirect2DRMDevice,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IDirect2DRMDevice,
		(LPVOID *) &pDevice);
	
	if (SUCCEEDED(hr)) {
		hr = pDevice->InitFromSurface(pddsBackBuffer);
		if (FAILED(hr))
			MMRELEASE(pDevice);
	}

	*ppDevice = pDevice;

	return hr;
}


STDMETHODIMP
CDirect2DRM::CreateViewport(LPDIRECT2DRMDEVICE pDevice,
							LPDIRECT2DRMFRAME pCamera,
							LPRECT prDimensions,
							LPDIRECT2DRMVIEWPORT *ppViewport)
{
	HRESULT hr;
	LPDIRECT2DRMVIEWPORT pViewport = NULL;

	hr = CoCreateInstance(
		CLSID_CDirect2DRMViewport,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IDirect2DRMViewport,
		(LPVOID *) &pViewport);

	if (SUCCEEDED(hr)) {
		hr = pViewport->Init(pDevice, pCamera, prDimensions);

		if (FAILED(hr))
			MMRELEASE(pViewport);
	}

	*ppViewport = pViewport;

	return hr;
}

STDMETHODIMP
CDirect2DRM::CreateImageFromSurface(LPDIRECTDRAWSURFACE pdds, BSTR bstrName,
									LPDIRECT2DRMIMAGE *ppImage)
{
	MMASSERT(ppImage);
	if (m_pImageCache == NULL)
		return E_NOTINITIALIZED;

	HRESULT hr;
	LPD2DRMIMAGEPRIVATE pImagePriv = NULL;
	CMFImage *pmfi = NULL;
	CString strName(bstrName);

		// get a reference to the MFImage from the image cache
	if (FAILED(hr = m_pImageCache->GetImageFromSurface(strName, pdds, 0, &pmfi)) ||
		// CoCreate the D2DRMImage object
		FAILED(hr = CoCreateInstance(
						CLSID_CDirect2DRMImage,
						NULL,
						CLSCTX_INPROC_SERVER,
						IID_ID2DRMImagePrivate,
						(LPVOID *) &pImagePriv)) ||
		// initialize the D2DRMImage with the MFImage
		FAILED(hr = pImagePriv->InitFromMFImage(this, pmfi, 0)))
		goto e_CreateImageFromImageFile;

	*ppImage = (LPDIRECT2DRMIMAGE) pImagePriv;

	// release the extra reference we had from the LoadImage
	pmfi->Release();

	return S_OK;

e_CreateImageFromImageFile:
	MMRELEASE(pImagePriv);
	MMRELEASE(pmfi);
	return hr;
}

STDMETHODIMP
CDirect2DRM::CreateImageFromImageFile(BSTR bstrFilename, DWORD dwFlags,
									  LPDIRECT2DRMIMAGE *ppImage)
{
	MMASSERT(ppImage);
	if (m_pImageCache == NULL)
		return E_NOTINITIALIZED;

	HRESULT hr;
	LPD2DRMIMAGEPRIVATE pImagePriv = NULL;
	CMFImage *pmfi = NULL;
	CString strFilename(bstrFilename);

		// get a reference to the MFImage from the image cache
	if (FAILED(hr = m_pImageCache->GetImageFromFile(strFilename, 0, &pmfi)) ||
		// CoCreate the D2DRMImage object
		FAILED(hr = CoCreateInstance(
						CLSID_CDirect2DRMImage,
						NULL,
						CLSCTX_INPROC_SERVER,
						IID_ID2DRMImagePrivate,
						(LPVOID *) &pImagePriv)) ||
		// initialize the D2DRMImage with the MFImage
		FAILED(hr = pImagePriv->InitFromMFImage(this, pmfi, dwFlags)))
		goto e_CreateImageFromImageFile;

	*ppImage = (LPDIRECT2DRMIMAGE) pImagePriv;

	// release the extra reference we had from the LoadImage
	pmfi->Release();

	return S_OK;

e_CreateImageFromImageFile:
	MMRELEASE(pImagePriv);
	MMRELEASE(pmfi);
	return hr;
}

STDMETHODIMP
CDirect2DRM::CreateImageFromImageFileExt(BSTR bstrFilename, DWORD dwFlags,
										 LPDIRECT2DRMIMAGE *ppImage)
{
	MMASSERT(ppImage);
	if (m_pImageCache == NULL)
		return E_NOTINITIALIZED;

	HRESULT hr;
	LPD2DRMIMAGEPRIVATE pImagePriv = NULL;
	CMFImage *pmfi = NULL;
	CString strFilename(bstrFilename);

	// get a reference to the MFImage from the image cache
	if (FAILED(hr = m_pImageCache->GetImageFromFile(strFilename, flagUsageMapStatically, &pmfi)) ||
		// CoCreate the D2DRMImage object
		FAILED(hr = CoCreateInstance(
						CLSID_CDirect2DRMImage,
						NULL,
						CLSCTX_INPROC_SERVER,
						IID_ID2DRMImagePrivate,
						(LPVOID *) &pImagePriv)) ||
		// initialize the D2DRMImage with the MFImage
		FAILED(hr = pImagePriv->InitFromMFImage(this, pmfi, dwFlags)))
		goto e_CreateImageFromImageFile;

	*ppImage = (LPDIRECT2DRMIMAGE) pImagePriv;

	// release the extra reference we had from the LoadImage
	pmfi->Release();

	return S_OK;

e_CreateImageFromImageFile:
	MMRELEASE(pImagePriv);
	MMRELEASE(pmfi);
	return hr;
}

#define nMINDEFPOINTHEIGHT 8
#define nDEFPOINTHEIGHT 12

#if 0
HFONT
CDirect2DRM::GetDefaultFont(DWORD nPointHeight)
{
	if (nPointHeight < nMINDEFPOINTHEIGHT)
		nPointHeight = nDEFPOINTHEIGHT;

	if (m_hDefFont == NULL) {
		HDC hDC = GetDC(NULL);
		int iLogPixelsY = GetDeviceCaps(hDC, LOGPIXELSY);
		ReleaseDC(NULL, hDC);
		
		int nFontHeight = (int) (((float(nPointHeight) * float(iLogPixelsY)) / 72.0f) + 0.5f);
		
		m_hDefFont = CreateFont(-nFontHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
						OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
						VARIABLE_PITCH | FF_DONTCARE, "Century Gothic");
	}
	return m_hDefFont;
}
#endif
IFont*
CDirect2DRM::GetDefaultFont()
{
	static FONTDESC _fontdescHeading =
	  { sizeof(FONTDESC), OLESTR("Century Gothic"), FONTSIZE( 12 ), FW_BOLD, 
     ANSI_CHARSET, FALSE, FALSE, FALSE };

	if (m_pDefFont) {
		m_pDefFont->AddRef();
		return m_pDefFont;
	} else {
		HRESULT hr = OleCreateFontIndirect( &_fontdescHeading, IID_IFont, (LPVOID*)&m_pDefFont);
		m_pDefFont->AddRef();
		//HDC hDC = GetDC(NULL);
		//hr = m_pDefFont->SetHdc(hDC);
		//ReleaseDC(NULL, hDC);
		return m_pDefFont;
	}
}

STDMETHODIMP
CDirect2DRM::CreateImageFromText(BSTR bstrText, DWORD nPointHeight, BOOL bShadowed, 
								 LPDIRECT2DRMIMAGE *ppImage)
{
	MMASSERT(ppImage);
	if (m_pImageCache == NULL)
		return E_NOTINITIALIZED;

	HRESULT hr;
	LPD2DRMIMAGEPRIVATE pImagePriv = NULL;
	CMFImage *pmfi = NULL;
	CString strText(bstrText);
	IFont *pFont = NULL;
	HFONT hFont = NULL;
	CY size = {nPointHeight*10000, 0};
	
	pFont = GetDefaultFont();
	pFont->put_Size(size);
	pFont->get_hFont(&hFont);
	
		// get a reference to the MFImage from the image cache
	if (FAILED(hr = m_pImageCache->GetImageFromText(strText, hFont, 0x00FFFFFF, bShadowed, 0, NULL, &pmfi)) ||
		// CoCreate the D2DRMImage object
		FAILED(hr = CoCreateInstance(
						CLSID_CDirect2DRMImage,
						NULL,
						CLSCTX_INPROC_SERVER,
						IID_ID2DRMImagePrivate,
						(LPVOID *) &pImagePriv)) ||
		// initialize the D2DRMImage with the MFImage
		FAILED(hr = pImagePriv->InitFromMFImage(this, pmfi, 0)))
		goto e_CreateImageFromText;

	*ppImage = (LPDIRECT2DRMIMAGE) pImagePriv;

	// release the extra reference we had from the CreateImageFromText
	pmfi->Release();

	return S_OK;

e_CreateImageFromText:
	MMRELEASE(pImagePriv);
	MMRELEASE(pmfi);
	MMRELEASE(pFont);
	return hr;
}


STDMETHODIMP
CDirect2DRM::CreateImageFromTextExt(
				BSTR bstrText,
				IFont *pFont,
				OLE_COLOR ocFontColor,
				BOOL bShadowed,
				OLE_COLOR ocShadowColor,
				LPDIRECT2DRMIMAGE *ppImage)
{
	MMASSERT(ppImage);
	if (m_pImageCache == NULL)
		return E_NOTINITIALIZED;

	HRESULT hr;
	LPD2DRMIMAGEPRIVATE pImagePriv = NULL;
	CMFImage *pmfi = NULL;
	CString strText(bstrText);
	IFont *pIFont = NULL;

	HFONT hFont = NULL;
	if ( pFont != NULL )
	{
		hr = pFont->get_hFont( &hFont);
		if (FAILED(hr))
			goto e_CreateImageFromText;
	} else {
		hr = E_INVALIDARG;
		goto e_CreateImageFromText;
	}
	
		// get a reference to the MFImage from the image cache
	if (FAILED(hr = m_pImageCache->GetImageFromText(strText, hFont, ocFontColor, bShadowed, ocShadowColor, NULL, &pmfi)) ||
		// CoCreate the D2DRMImage object
		FAILED(hr = CoCreateInstance(
						CLSID_CDirect2DRMImage,
						NULL,
						CLSCTX_INPROC_SERVER,
						IID_ID2DRMImagePrivate,
						(LPVOID *) &pImagePriv)) ||
		// initialize the D2DRMImage with the MFImage
		FAILED(hr = pImagePriv->InitFromMFImage(this, pmfi, 0)))
		goto e_CreateImageFromText;

	*ppImage = (LPDIRECT2DRMIMAGE) pImagePriv;

	// release the extra reference we had from the CreateImageFromText
	pmfi->Release();
	MMRELEASE(pIFont);
	return S_OK;

e_CreateImageFromText:
	MMRELEASE(pImagePriv);
	MMRELEASE(pmfi);
	MMRELEASE(pIFont);
	return hr;
}


STDMETHODIMP
CDirect2DRM::CreateImageFromWrappedTextExt(
				BSTR bstrText,
				IFont *pFont,
				BOOL bShadowed,
				DWORD width, 
				DWORD height,
				DWORD nPoint,
				LPDIRECT2DRMIMAGE *ppImage)
{
	MMASSERT(ppImage);
	if (m_pImageCache == NULL)
		return E_NOTINITIALIZED;

	HRESULT hr;
	LPD2DRMIMAGEPRIVATE pImagePriv = NULL;
	CMFImage *pmfi = NULL;
	SIZE size = {width, height };
	CString strText(bstrText);
	IFont *pIFont = NULL;
	CY ptsize = {nPoint*10000, 0};

	HFONT hFont = NULL;
	if ( pFont != NULL )
	{
		hr = pFont->get_hFont( &hFont);
		if (FAILED(hr))
			goto e_CreateImageFromText;
	} else {
		pIFont = GetDefaultFont();
		hr = pIFont->put_Size(ptsize);
		hr = pIFont->get_hFont( &hFont);
		
		if (FAILED(hr))
			goto e_CreateImageFromText;
	}
	
		// get a reference to the MFImage from the image cache
	if (FAILED(hr = m_pImageCache->GetImageFromText(strText, hFont, 0x00FFFFFF, bShadowed, 0, &size, &pmfi)) ||
		// CoCreate the D2DRMImage object
		FAILED(hr = CoCreateInstance(
						CLSID_CDirect2DRMImage,
						NULL,
						CLSCTX_INPROC_SERVER,
						IID_ID2DRMImagePrivate,
						(LPVOID *) &pImagePriv)) ||
		// initialize the D2DRMImage with the MFImage
		FAILED(hr = pImagePriv->InitFromMFImage(this, pmfi, 0)))
		goto e_CreateImageFromText;

	*ppImage = (LPDIRECT2DRMIMAGE) pImagePriv;

	// release the extra reference we had from the CreateImageFromText
	pmfi->Release();
	MMRELEASE(pIFont);
	return S_OK;

e_CreateImageFromText:
	MMRELEASE(pImagePriv);
	MMRELEASE(pmfi);
	MMRELEASE(pIFont);
	return hr;
}


STDMETHODIMP
CDirect2DRM::GetNamedImage(BSTR bstrName, LPDIRECT2DRMIMAGE *ppImage)
{
	MMASSERT(ppImage);
	if (m_pImageCache == NULL)
		return E_NOTINITIALIZED;

	HRESULT hr;
	LPD2DRMIMAGEPRIVATE pImagePriv = NULL;
	CMFImage *pmfi = NULL;
	CString strFilename(bstrName);

	// get a reference to the MFImage from the image cache
	if (FAILED(hr = m_pImageCache->GetNamedImage(strFilename, &pmfi)) ||
		// CoCreate the D2DRMImage object
		FAILED(hr = CoCreateInstance(
						CLSID_CDirect2DRMImage,
						NULL,
						CLSCTX_INPROC_SERVER,
						IID_ID2DRMImagePrivate,
						(LPVOID *) &pImagePriv)) ||
		// initialize the D2DRMImage with the MFImage
		FAILED(hr = pImagePriv->InitFromMFImage(this, pmfi, 0)))
		goto e_GetNamedImage;

	*ppImage = (LPDIRECT2DRMIMAGE) pImagePriv;

	// release the extra reference we had from the GetNamedImage
	pmfi->Release();

	return S_OK;

e_GetNamedImage:
	MMRELEASE(pImagePriv);
	MMRELEASE(pmfi);
	return hr;
}

STDMETHODIMP
CDirect2DRM::ReleaseImage(BSTR bstrName)
{
	CString strFilename(bstrName);

	return m_pImageCache->RemoveImage(strFilename);

}

STDMETHODIMP
CDirect2DRM::SetVideoMode(LPDDPIXELFORMAT pddpf, LPDIRECTDRAWPALETTE pddp)
{
	if (m_pImageCache == NULL)
		return E_NOTINITIALIZED;
	if (pddpf == NULL)
		return E_NULLPOINTER;

	return m_pImageCache->SetVideoMode(*pddpf, pddp);
}


STDMETHODIMP
CDirect2DRM::CreateFrame(LPDIRECT2DRMFRAME pParentFrame, LPDIRECT2DRMFRAME *ppFrame)
{
	HRESULT hr;
	LPDIRECT2DRMFRAME pFrame = NULL;

	hr = CoCreateInstance(
		CLSID_CDirect2DRMFrame,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IDirect2DRMFrame,
		(LPVOID *) &pFrame);

	if (SUCCEEDED(hr)) {
		if (pParentFrame) {
			hr = pParentFrame->AddChild(pFrame);
			if (FAILED(hr)) {
				MMRELEASE(pFrame);
			}
		}
	}
	*ppFrame = pFrame;

	return hr;
}

STDMETHODIMP 
CDirect2DRM::SetImageCacheHardLimit(DWORD dwSizeInBytes)
{
	if (m_pImageCache == NULL)
		return E_NOTINITIALIZED;

	return m_pImageCache->SetImageCacheLimits(DWORD(0.9 * float(dwSizeInBytes)), dwSizeInBytes);
}

STDMETHODIMP
CDirect2DRM::Tick(DWORD nElapsedTime)
{
	CPtrList			listTicked;
	LPD2DRMIMAGEPRIVATE pd2di;
	
	// NOTE: This code is NOT critical sectioned. This is because only images which
	// have textures will be on the ticked list and textures are ONLY released (and 
	// therefore their images removed from the ticked list) via the ReleaseTextures() 
	// method. The ReleaseTextures() method MUST be called from the same thread as 
	// this method.
	// REVIEW: The CS now added ALMOST removes the need for textures to be released in the
	// rendering thread. However it is still possible for the texture to be accessed
	// inside the image Tick after it has been destroyed.

	// Work through list (safely inside CS) plucking off images and AddRef'ing them
	// (need to AddRef 'cause could get a TextureDestroyCallback while we're in the
	// image Tick). Cannot do image Tick inside CS since can deadlock with DelTicked
	// which will be called from TextureDestroyCallback (inside D3D CS).
	EnterCriticalSection(&m_CSTickedList);

	for (POSITION pos = m_listTicked.GetHeadPosition(); pos;)
	{
		pd2di = (LPD2DRMIMAGEPRIVATE) m_listTicked.GetNext(pos);

		if (pd2di)
			pd2di->AddRef();

		listTicked.AddTail(pd2di);
	}
	LeaveCriticalSection(&m_CSTickedList);

	// Now Tick the AddRef'd images from this separate list.
	for (pos = listTicked.GetHeadPosition(); pos;)
	{
		pd2di = (LPD2DRMIMAGEPRIVATE) listTicked.GetNext(pos);
		if (pd2di)
			pd2di->Tick(nElapsedTime);
		MMRELEASE(pd2di);
	}

	return S_OK;
}


POSITION
CDirect2DRM::InsTicked(LPD2DRMIMAGEPRIVATE pd2diPriv)
{
	POSITION pos;
	MMASSERT(pd2diPriv);
	EnterCriticalSection(&m_CSTickedList);
	pos = m_listTicked.AddTail((LPVOID) pd2diPriv);
	LeaveCriticalSection(&m_CSTickedList);
	return pos;
}

void
CDirect2DRM::DelTicked(POSITION pos)
{
	MMASSERT(pos);
	EnterCriticalSection(&m_CSTickedList);
	m_listTicked.RemoveAt(pos);
	LeaveCriticalSection(&m_CSTickedList);
}

STDMETHODIMP
CDirect2DRM::AddTextureForRelease(LPDIRECT3DRMTEXTURE pTexture)
{
	MMASSERT(pTexture);
	if (!m_listTextures.Find((void*)pTexture))
		m_listTextures.AddTail((void*)pTexture);
	else
		pTexture->Release();
	return S_OK;
}

STDMETHODIMP
CDirect2DRM::ReleaseTextures()
{
	POSITION			pos, posNext;
	IDirect3DRMTexture	*pTexture;

	for (posNext = m_listTextures.GetHeadPosition(); posNext;)
	{
		pos = posNext;
		pTexture = (IDirect3DRMTexture*)m_listTextures.GetNext(posNext);
		MMASSERT(pTexture);
		pTexture->Release();
		m_listTextures.RemoveAt(pos);
	}
	return S_OK;
}
