// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// File:	imgcache.cpp
// Author:	MM    (MM)

#include "MMImage.h"

#define JUMPONFAIL( hr, label ) \
	if (FAILED(hr)) goto label;

ImgCacheCookie g_iccNullCookie = NULL;

CImageCache::CImageCache()
{
	m_nRef = 1;
	m_nHardLimit = 0;
	m_nCutLimit = 0;
	m_nActiveSize = 0;
	m_nInactiveSize = 0;
	m_pDD = NULL;
	m_pColMgr = NULL;
	m_pmfibBuilder = NULL;
	m_pmfiDefault = NULL;
}

HRESULT
CImageCache::Init(LPDIRECTDRAW pDD)
{
	if (m_pDD)
		return E_ALREADYINITIALIZED;

	if (pDD == NULL)
		return E_INVALIDOBJECT;

	HRESULT hr;

	pDD->AddRef();
	m_pDD = pDD;

	// create the palette manager
	MMASSERT(m_pColMgr == NULL);
	m_pColMgr = new CColorManager();
	if (m_pColMgr == NULL)
		return E_OUTOFMEMORY;

	if (FAILED(hr = m_pColMgr->Init(m_pDD)))
		return hr;

	// create an image builder
	MMASSERT(m_pmfibBuilder == NULL);
	m_pmfibBuilder = new CMFImageBuilder;
	if (m_pmfibBuilder == NULL)
		return E_OUTOFMEMORY;
	if (FAILED(hr = m_pmfibBuilder->Init(m_pDD, m_pColMgr)))
		return hr;

	return S_OK;
}

HRESULT
CImageCache::Reset()
{
//	MMTRACE("***RESETTING IMAGE CACHE***\n");
	int nLength;

//	MMTRACE("ActiveImages: %d\n", m_listActiveImages.GetCount());
//	MMTRACE("InActiveImages: %d\n", m_listInactiveImages.GetCount());

	// remove all the images left in the cache
	CMFImage *pImage;
	for (nLength = m_listActiveImages.GetCount(); nLength != 0; nLength--) {
		pImage = (CMFImage *) m_listActiveImages.RemoveHead();
		MMASSERT(pImage);
		pImage->SetImageCache(NULL);
//		TRACE("Releasing Active Image: 0x%p\n", pImage);
		pImage->Release();
	}
	for (nLength = m_listInactiveImages.GetCount(); nLength != 0; nLength--) {
		pImage = (CMFImage *) m_listInactiveImages.RemoveHead();
		MMASSERT(pImage);
		pImage->SetImageCache(NULL);
//		TRACE("Releasing Inactive Image %d: 0x%p\n", nLength, pImage);
		pImage->Release();
	}
	MMRELEASE(m_pmfiDefault);

//	MMTRACE("***RESETTING IMAGE CACHE-->		  DONE!\n");

	return S_OK;
}

CImageCache::~CImageCache()
{
	HRESULT hr;
	hr = Reset();

	MMDELETE(m_pmfibBuilder);

	MMRELEASE(m_pDD);
	MMRELEASE(m_pColMgr);

	MMASSERT(SUCCEEDED(hr));
}


HRESULT
CImageCache::SetVideoMode(const DDPIXELFORMAT &ddpf, LPDIRECTDRAWPALETTE pddp)
{
	HRESULT hr;

	if (m_pColMgr == NULL)
		return E_NOTINITIALIZED;

	BOOL bChanged;
	if (FAILED(hr = m_pColMgr->SetVideoMode(ddpf, pddp, &bChanged)))
		return hr;

	if (bChanged) {
		// TODO: fix any cached image info that might change if the video mode changes
	}
	return S_OK;
}


HRESULT
CImageCache::GetNamedImage(const char *szName, CMFImage **ppmfi)
{
	MMASSERT(ppmfi);
	if (m_pDD == NULL)
		return E_NOTINITIALIZED;
	if ((szName == NULL) || (szName[0] == 0))
		return E_INVALIDARG;

	// check if the image is in the cache
	void *pvImage;
	if (m_mapFilenameToImage.Lookup(szName, pvImage) != FALSE) {
		// use the cached image
		CMFImage *pmfi = (CMFImage *) pvImage;
		MMASSERT(pmfi);
		pmfi->AddRef();
		*ppmfi = pmfi;
		return S_OK;
	}
	return TYPE_E_ELEMENTNOTFOUND;
}


HRESULT
CImageCache::GetImageFromFile(const char *szFilename, DWORD dwFlags, CMFImage **ppmfi)
{
	MMASSERT(ppmfi);
	HRESULT hr;

	// check args and see if image is already in the cache
	hr = GetNamedImage(szFilename, ppmfi);

	if (hr == TYPE_E_ELEMENTNOTFOUND) {
		// load the image from file
		if (FAILED(hr = LoadImageFromFile(szFilename, dwFlags, ppmfi)))
			return hr;

		// no need to release image because we want to keep an extra ref for the cache
		InsImage(szFilename, *ppmfi);
	}
	return hr;
}

HRESULT
CImageCache::GetImageFromText(const char *szText, HFONT hFont, COLORREF crFontColor, BOOL bShadowed, COLORREF crShadowColor, SIZE *pSize,
							  CMFImage **ppmfi)
{
	LPDIRECTDRAWSURFACE pdds = NULL;
	LPCOLORINFO			pciText;
	SIZE				sizText;
	DDSURFACEDESC		ddsd;
	HRESULT				hr = S_OK;

	MMASSERT(ppmfi);

	// Create a surface for the text.
	if (FAILED(hr = CreateSurfaceWithText(m_pDD, m_pColMgr, szText, hFont, crFontColor, bShadowed, crShadowColor, pSize, &sizText, &pciText, &pdds)))
		goto ERROR_ENCOUNTERED;		
		
	// Create the MFImage from the surface.
	*ppmfi = new CMFImage;
	INIT_DXSTRUCT(ddsd);
	if (FAILED(hr = pdds->GetSurfaceDesc(&ddsd)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = (*ppmfi)->Init(szText, (WORD)ddsd.dwWidth, (WORD)ddsd.dwHeight, pciText)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = (*ppmfi)->AddFrame(NULL, pdds, 0)))
		goto ERROR_ENCOUNTERED;

	// Don't add this to the cache 'cause its just text created and easy to re-create.
	// We do need to give it the DD pointer though so it can create surfaces.
	(*ppmfi)->SetDD(m_pDD);

ERROR_ENCOUNTERED:
	
	MMRELEASE(pciText);
	MMRELEASE(pdds);

	return hr;
}

HRESULT
CImageCache::GetImageFromSurface(const char *szName, LPDIRECTDRAWSURFACE pdds, DWORD dwFlags,
								 CMFImage **ppmfi)
{
	MMASSERT(ppmfi);

	if ((pdds == NULL) || (szName == NULL) || (szName[0] == 0))
		return E_INVALIDARG;

	HRESULT hr;
	BOOL bVideoModeDependent;
	
		// remove image from cache if it already exists
	if (FAILED(hr = RemoveImage(szName)) ||
		// create the image in the image builder
		FAILED(hr = m_pmfibBuilder->Setup(dwFlags)) ||
		FAILED(hr = m_pmfibBuilder->Reset(szName)) ||
		FAILED(hr = m_pmfibBuilder->AddFrame(pdds, NULL, 0)) ||
		FAILED(hr = m_pmfibBuilder->Compile(ppmfi, &bVideoModeDependent)))
	{
		return hr;
	}
	
	// no need to release image because we want to keep an extra ref for the cache
	InsImage(szName, *ppmfi);

	return S_OK;
}


HRESULT
CImageCache::SetImageCacheLimits(DWORD nCutLimitInBytes, DWORD nHardLimitInBytes)
{
	if (nCutLimitInBytes > nHardLimitInBytes)
		nCutLimitInBytes = DWORD(0.9 * float(nHardLimitInBytes));

	m_nHardLimit = nHardLimitInBytes;
	m_nCutLimit = nCutLimitInBytes;
	// check if we need to delete any images from the inactive list
	CheckLimits();

	return S_OK;
}


HRESULT
CImageCache::LoadImageFromFile(const char *szFilename, DWORD dwFlags, CMFImage **ppmfi)
{
	MMASSERT(ppmfi && m_pmfibBuilder);

	HRESULT hr;
	DWORD nFileLength = 0, dwHighSize = 0;
	DWORD dwStart, dwEndDecode;
	char *pExt = g_rgchTmpBuf;

	// DEBUG: gather timing information
	dwStart = GetTickCount();

	//
	// open the file
	//
	HANDLE hFile = NULL, hMapping = NULL;
	LPBYTE pData = NULL;
	hFile = CreateFile(szFilename, GENERIC_READ, FILE_SHARE_READ, NULL, 
				OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);

	if ((hFile == NULL) || (hFile == INVALID_HANDLE_VALUE)) {
		MMTRACE("CImageCache::LoadImageFromFile: file not found - %s\n", szFilename);
		hr = STG_E_FILENOTFOUND;
		goto e_LoadImageFile;
	}

	// get the length of the file
	nFileLength = GetFileSize(hFile, &dwHighSize);
	if ((nFileLength == 0xFFFFFFFF) || dwHighSize) {
		hr = E_FAIL;
		goto e_LoadImageFile;
	}

	// create a file mapping object
	if (((hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL)) == NULL) ||
		((pData = (LPBYTE) MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0)) == NULL))
	{
		hr = E_FAIL;
		goto e_LoadImageFile;
	}

	// prepare the image builder for the new image
	// REVIEW: what are the flags for the builder!!!
	if (FAILED(hr = m_pmfibBuilder->Setup(dwFlags)) ||
		FAILED(hr = m_pmfibBuilder->Reset(szFilename)))
	{
		goto e_LoadImageFile;
	}

	// get file extension
	_splitpath(szFilename, NULL, NULL, NULL, pExt);

	// skip the '.'
	pExt += (*pExt == '.');

	// call appropriate loader 
	if (_strnicmp(pExt, "gif", 3) == 0) {
		CGIFImage gifImage;
		hr = gifImage.LoadAll(pData, nFileLength, m_pmfibBuilder);
	} 
	else if (_strnicmp(pExt, "bmp", 3) == 0) {
		CBMPImage bmpImage;
		hr = bmpImage.LoadAll(pData, nFileLength, m_pmfibBuilder);
	}
	else if (_strnicmp(pExt, "ppm", 3) == 0) {
		CPPMImage ppmImage;
		hr = ppmImage.LoadAll(pData, nFileLength, m_pmfibBuilder);
	} 
	else if ((_strnicmp(pExt, "jpg", 3) == 0) || (_strnicmp(pExt, "jpeg", 4) == 0)) {
		CJPGImage jpgImage;
		hr = jpgImage.LoadAll(pData, nFileLength, m_pmfibBuilder);
	} 
	else if (_strnicmp(pExt, "png", 3) == 0) {
		hr = E_NOTIMPL;
	}
	else {
		MMTRACE("error: unknown file extension: %s", pExt);
		hr = MK_E_INVALIDEXTENSION;
	}

e_LoadImageFile:
	if (SUCCEEDED(hr)) {
		BOOL bVideoModeDependent;

		// create an MFI from the MFIB
		hr = m_pmfibBuilder->Compile(ppmfi, &bVideoModeDependent);
		dwEndDecode = GetTickCount();

//		MMTRACE("\tTiming for %s = %d\n", szFilename, dwEndDecode - dwStart);

		// REVIEW: record that the image is dependent on the current video mode
		//   so that it can be updated if the video mode changes
	}

	// ********************************************************************
	// don't close the file before we do the compile because we might keep
	// tags to the file mapping in the builder
	// ********************************************************************
	if (pData)
		UnmapViewOfFile(pData);

	if (hMapping)
		CloseHandle(hMapping);

	if (hFile)
		CloseHandle(hFile);

	return hr;
}


void
CImageCache::InsImage(const char *szName, CMFImage *pmfi)
{
	MMASSERT(pmfi);

	pmfi->SetImageCache(this);

	// put the image in the file map
	m_mapFilenameToImage.SetAt(szName, pmfi);
	pmfi->AddRef();
}


HRESULT
CImageCache::RemoveImage(const char *szName)
{
	MMASSERT(szName);

	void *pvImage;
	if (m_mapFilenameToImage.Lookup(szName, pvImage) != FALSE)
		return RemoveImage((CMFImage *) pvImage);

	return S_OK;
}


HRESULT
CImageCache::RemoveImage(CMFImage *pmfi)
{
	MMASSERT(pmfi);

	const char *szName = pmfi->GetName();

	if (pmfi->m_nRef == 1) {
		BOOL bSuccess;
		//
		// the image is inactive
		//
		// remove the hash entry for the image
		bSuccess = m_mapFilenameToImage.RemoveKey(szName);
		MMASSERT(bSuccess);
		
		// remove the image from the inactive list
		MMASSERT(pmfi->m_icc != g_iccNullCookie);
		m_listInactiveImages.RemoveAt((POSITION) pmfi->m_icc);
		
		// adjust the cache consumption
		m_nInactiveSize -= pmfi->GetMemSize();
		
		// free the image
		pmfi->Release();
	} else {
		MMTRACE("error [imgcache]: image '%s' is currently active\n", szName);
		return E_INVALIDARG;
	}
	return S_OK;
}


void
CImageCache::CheckLimits()
{
	// the hard limit has not been set
	if (m_nHardLimit == 0)
		return;

	HRESULT hr;
	
	if ((m_nActiveSize + m_nInactiveSize) > m_nHardLimit) {
		// remove inactive images until we hit the cut limit or run out of things to delete
		MMASSERT(m_nCutLimit <= m_nHardLimit);
		for (int nLength = m_listInactiveImages.GetCount(); 
			(nLength != 0) && ((m_nActiveSize + m_nInactiveSize) > m_nCutLimit); nLength--)
		{
			// remove the image from the tail of the inactive list
			CMFImage *pmfi = (CMFImage *) m_listInactiveImages.GetTail();
			hr = RemoveImage(pmfi);
			MMASSERT(SUCCEEDED(hr));
		}
	}
}


void
CImageCache::Activate(CMFImage *pmfi)
{
//	MMTRACE("Activate image\n");
	MMASSERT(pmfi);

	DWORD nSize = pmfi->GetMemSize();

	if (pmfi->m_icc != g_iccNullCookie) {
		// remove the image from the inactive list
		m_listInactiveImages.RemoveAt(pmfi->m_icc);
		m_nInactiveSize -= nSize;
	}
	
	// put the image on the active list
	pmfi->m_icc = m_listActiveImages.AddHead((void *) pmfi);
	m_nActiveSize += nSize;

	CheckLimits();
}


void
CImageCache::InActivate(CMFImage *pmfi)
{
//	MMTRACE("InActivate image\n");

	// an image can only become inactive if it was already active
	MMASSERT(pmfi && (pmfi->m_icc != g_iccNullCookie));

	DWORD nSize = pmfi->GetMemSize();

	// remove the image from the active list
	m_listActiveImages.RemoveAt(pmfi->m_icc);
	m_nActiveSize -= nSize;

	// put the image at the front of the inactive list
	pmfi->m_icc = m_listInactiveImages.AddHead((void *) pmfi);
	m_nInactiveSize += nSize;

	CheckLimits();
}
