// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _PalMgr_h
#define _PalMgr_h

// File:	PalMgr.h
// Author:	MM    (MM)
//
// Description:
//    
// 
// History:
// -@- 4/17/97 (MM) created
//
// REVIEW: 7/14/97 there needs to be some code that removes ColorInfo objects from the cache
// REVIEW: 7/14/97 if we store the cacheID's with the ColorInfo objects, then it should be
//   possible to remove ColorInfo object from the cache ==> we don't need a separate
//   list of the objects in the hashtable since we can iterate thru all of them
//   with the hash table services

typedef POSITION ColMgrCookie;
extern ColMgrCookie g_cmcNullCookie;

// default palette tables
extern PALETTEENTRY g_rgpeDefPal2[2], g_rgpeDefPal4[4], g_rgpeDefPal16[16], g_rgpeDefPal256[256];

class CColorManager;

#define nCIHASHIDLENGTH 8

class CColorInfo {
	friend class CColorManager;
private:
						CColorInfo();
						~CColorInfo();
public:
	DWORD				PackPixel(const LPBYTE pPixels)	{ return pixi.Pack(pPixels); }
	HRESULT				CreatePlainSurface(LPDIRECTDRAW pDD, DWORD nWidth, DWORD nHeight, 
							LPDIRECTDRAWSURFACE *ppdds, VARIANT_BOOL bForTexture = VARIANT_FALSE) const;
	
	// reference counting
	DWORD				AddRef();
	DWORD				Release();

	BYTE				GetBPP() const					{ return pixi.nBPP; }

public:
	// REVIEW: do i need the pixi in here???
	CPixelInfo			pixi;			// original image's pixel info
	LPDIRECTDRAWPALETTE pddp;			// original image's palette
	DWORD				dwTransColor;	// transparent color on original image
	DWORD				bvColorFlags;
	CPaletteMap			pmVideoMode;	// map from image's palette to the video mode

private:
	static void			GetHashID(const CPixelInfo &pixiRGB, DWORD dwTransColor, 
							DWORD dwFlags, LPBYTE pID);
	static void			GetHashID(BYTE nBPP, const PALETTEENTRY *rgpePalette, 
							DWORD dwTransColor, DWORD dwFlags, LPBYTE pID);

private:
	void				SetColMgr(CColorManager *pColMgr);

	DWORD				m_nRef;
	CColorManager *		m_pColMgr;
//	ColMgrCookie		m_cmc;
};

typedef CColorInfo *LPCOLORINFO;


class CColorManager {
	friend class CColorInfo;
public:
						CColorManager();
						~CColorManager();

	HRESULT				Init(LPDIRECTDRAW pDD);
	HRESULT				Reset();

	// reference counting
	DWORD				AddRef();
	DWORD				Release();

	HRESULT				SetVideoMode(const DDPIXELFORMAT &ddpf, LPDIRECTDRAWPALETTE pddp, 
							LPBOOL pbChanged);
	const CPixelInfo &	GetVideoModePixelInfo() const { return m_pixiTarget; }
	LPDIRECTDRAWPALETTE	GetVideoModePalette() const;

	// ColorInfo for RGB
	HRESULT				GetColorInfo(const CPixelInfo &pixiRGBPixFmt, DWORD dwTransColor, 
							DWORD dwFlags, LPCOLORINFO *ppci);
	// ColorInfo for palette that does not exist
	HRESULT				GetColorInfo(const CPixelInfo &pixiPalFmt, BYTE nBPPPalette, 
							LPBYTE pPalette, BYTE nBPPPixels, DWORD dwTransColor, 
							DWORD dwFlags, LPCOLORINFO *ppci);
	// ColorInfo for palette that already exists
	HRESULT				GetColorInfo(LPDIRECTDRAWPALETTE pddp, DWORD dwTransColor, 
							DWORD dwFlags, LPCOLORINFO *ppci);
	// ColorInfo for bit depth
	HRESULT				GetDefaultColorInfo(BYTE nBPP, LPCOLORINFO *ppci);

	HRESULT				GetPreferredPixelFormat(BYTE nBPP, CPixelInfo *ppixi);

private:
	HRESULT				GetColorInfo(const CPixelInfo &pixiPixFmt, const PALETTEENTRY *rgpe, 
							LPDIRECTDRAWPALETTE pddp, const BYTE *pID,
							DWORD dwTransColor, DWORD dwFlags, LPCOLORINFO *ppci);

	HRESULT				CreateColorInfo(const CPixelInfo &pixi, LPDIRECTDRAWPALETTE pddp, 
							DWORD dwTransColor, DWORD dwFlags, LPCOLORINFO *ppci);

	HRESULT				InsColorInfo(const char *szID, LPCOLORINFO pci);

private:
	DWORD				m_nRef;
	LPDIRECTDRAW		m_pDD;			// need to create palettes dynamically

	CMapStringToPtr		m_mapColorInfo;
	// REVIEW: i don't need a list of the colorinfo objects -- use hash::removeall
	CPtrList			m_listColorInfo;

	BOOL				m_bActiveTarget;
	LPDIRECTDRAWPALETTE	m_pddpTarget;
	CPixelInfo			m_pixiTarget;
//	DDPIXELFORMAT		m_ddpfTarget;
};


inline DWORD
CColorInfo::AddRef()
{
	return ++m_nRef;
}

inline DWORD
CColorInfo::Release()
{
	--m_nRef;

	if (m_nRef == 0) {
		// REVIEW: this could leave a dangling reference if something other
		//  than the ColorManager does the final release!!!
//		MMASSERT(0);
//		if (m_pColMgr) {
//			m_pColMgr->Remove();
//		}
		delete this;
		return 0;
	}

	return m_nRef;
}

inline DWORD
CColorManager::AddRef()
{
	return ++m_nRef;
}

inline DWORD
CColorManager::Release()
{
	if (--m_nRef == 0) {
		delete this;
		return 0;
	}

	return m_nRef;
}

inline LPDIRECTDRAWPALETTE
CColorManager::GetVideoModePalette() const
{
	if (m_pddpTarget) 
		m_pddpTarget->AddRef(); 
	return m_pddpTarget;
}


#endif
