// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _imgcache_h
#define _imgcache_h

// File:	imgcache.h
// Author:	MM    (MM)
//
// Description:
//    This class is the D2D image chaching mechanism.

// Notes:
//    Later on, if i decide to add the softlimit managment stuff,
//  we can give each individual frame of an image a cookie.  The imagecache
//  can then do its own thing with mipmap levels, assuming the imagecache
//  is consulted for each access.
//    If this thing kept track of the D3D meshes that are using the 2D images
//  as textures, we could change the textures on the fly.  It would be nice to
//  know more about how the texture is currently being used tho...
// 
// History:
// -@- 1/17/97 (MM) created

#define nMAXSURFACEDIM 0xffff

typedef POSITION ImgCacheCookie;

extern ImgCacheCookie g_iccNullCookie;

class CMFImage;
class CMFImageBuilder;
class CColorInfo;

class CImageCache {
	friend class CMFImage;
public:
						CImageCache();
						~CImageCache();

	HRESULT				Init(LPDIRECTDRAW pDD);
	HRESULT				Reset();

	DWORD				AddRef();
	DWORD				Release();

	HRESULT				SetVideoMode(const DDPIXELFORMAT &ddpf, LPDIRECTDRAWPALETTE pddp);

	HRESULT				GetNamedImage(const char *szName, CMFImage **ppmfi);
	HRESULT				GetImageFromFile(const char *szFilename, DWORD dwFlags, CMFImage **ppmfi);
	HRESULT				GetImageFromText(const char *szText, HFONT hFont, COLORREF crFontColor, BOOL bShadowed, COLORREF crShadowColor, SIZE *pSize,
							CMFImage **ppmfi);
	HRESULT				GetImageFromSurface(const char *szName, LPDIRECTDRAWSURFACE pdds, DWORD dwFlags,
							CMFImage **ppmfi);

//	HRESULT				SetDefaultImage(CMFImage *pmfi);
//	HRESULT				GetDefaultImage(CMFImage **ppmfi);

	HRESULT				RemoveImage(const char *szName);
	HRESULT				RemoveImage(CMFImage *pmfi);

	HRESULT				SetImageCacheLimits(DWORD nCutLimitInBytes, DWORD nHardLimitInBytes);

public:
	HRESULT				CreatePlainSurface(DWORD nWidth, DWORD nHeight, const CColorInfo *pci,
							LPDIRECTDRAWSURFACE *ppdds, VARIANT_BOOL bForTexture = VARIANT_FALSE) const;

private:
	HRESULT				LoadImageFromFile(const char *szFilename, DWORD dwFlags, CMFImage **ppmfi);

private:
	void				InsImage(const char *szName, CMFImage *pmfiImage);
	void				CheckLimits();
private:
	// these should ONLY be called by CMFImage
	void				Activate(CMFImage *pmfi);
	void				InActivate(CMFImage *pmfi);

private:
	DWORD				m_nRef;

	DWORD				m_nHardLimit, m_nCutLimit;
	DWORD				m_nActiveSize, m_nInactiveSize;

	CMapStringToPtr		m_mapFilenameToImage;
	CPtrList			m_listActiveImages;
	CPtrList			m_listInactiveImages;

	LPDIRECTDRAW		m_pDD;					// may need to create surfaces dynamically
	CColorManager *		m_pColMgr;

	// image file loading
	CMFImageBuilder *	m_pmfibBuilder;
	CMFImage *			m_pmfiDefault;
};


inline HRESULT
CImageCache::CreatePlainSurface(DWORD nWidth, DWORD nHeight, const CColorInfo *pci,
								LPDIRECTDRAWSURFACE *ppdds, VARIANT_BOOL bForTexture) const
{
	MMASSERT((nWidth <= nMAXSURFACEDIM) && (nHeight <= nMAXSURFACEDIM));
	if (pci)
		return pci->CreatePlainSurface(m_pDD, nWidth, nHeight, ppdds, bForTexture);
	return E_INVALIDARG;
}

inline DWORD
CImageCache::AddRef()
{
	return ++m_nRef;
}

inline DWORD
CImageCache::Release()
{
	if (--m_nRef == 0) {
		delete this;
		return 0;
	}

	return m_nRef;
}


// Macro for memory mapped file stuff
#define CHECKEOF(__pFilePos, __pFileLimit) MACSTART \
	if ((__pFilePos) >= (__pFileLimit)) { \
		MMTRACE(g_szEOFMessage); \
		return E_UNEXPECTED; \
	} MACEND

#endif
