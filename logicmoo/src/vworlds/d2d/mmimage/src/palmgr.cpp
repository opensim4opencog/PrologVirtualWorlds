// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// File:	PalMgr.cpp
// Author:	MM    (MM)

#include "MMImage.h"

ColMgrCookie g_cmcNullCookie = NULL;

//
// CColorInfo
//
CColorInfo::CColorInfo()
{
	pixi.Init();
	pddp = NULL;
	dwTransColor = 0;
	bvColorFlags = 0;

	m_nRef = 1;
	m_pColMgr = NULL;
//	m_cmc = g_cmcNullCookie;
}

CColorInfo::~CColorInfo()
{
	MMASSERT(m_nRef == 0);
	MMDEBUG_ONLY(m_pColMgr = NULL);
	MMRELEASE(pddp);
}

void
CColorInfo::SetColMgr(CColorManager *pColMgr)
{
	// Notes:
	// - do not AddRef -- avoids circular reference problem
	// - the color manager can be NULL
	m_pColMgr = pColMgr;
}

HRESULT
CColorInfo::CreatePlainSurface(LPDIRECTDRAW pDD, DWORD nWidth, DWORD nHeight, 
							   LPDIRECTDRAWSURFACE *ppdds, VARIANT_BOOL bForTexture) const
{
	MMASSERT(pDD && ppdds);
	HRESULT hr;
	LPDIRECTDRAWSURFACE pdds = NULL;

	// compute a pixel format that matches the color model
	DDPIXELFORMAT ddpf;
	pixi.GetDDPF(ddpf);

	if (FAILED(hr = ::CreatePlainSurface(pDD, nWidth, nHeight, 0, &ddpf, pddp, &pdds, bForTexture)))
		return hr;

	// set the transparency color key on the surface and clear to it
	if (bvColorFlags & flagTRANSPARENT)
	{
		DDCOLORKEY ddck;
		DDBLTFX ddbfx;
		INIT_DXSTRUCT(ddbfx);
		ddbfx.dwFillColor = dwTransColor;
		ddck.dwColorSpaceLowValue = ddck.dwColorSpaceHighValue = dwTransColor;
		if (FAILED(hr = pdds->SetColorKey(DDCKEY_SRCBLT, &ddck)) ||
			FAILED(hr = pdds->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_ASYNC, &ddbfx)))
		{
			MMRELEASE(pdds);
			return hr;
		}
	}
	*ppdds = pdds;
	return S_OK;
}


// Function: GetHashID
//    These functions compute an ID for a ColorInfo object constructed from
//  the given arguments.  The ID must be a string with no NULL chars so that
//  the MFC hashing mechanism does the right thing.
void
CColorInfo::GetHashID(const CPixelInfo &pixi, DWORD dwTransColor, DWORD dwFlags, LPBYTE pID)
{
	MMASSERT((pixi.nBPP > 8) && (pixi.nBPP <= 32) && pID);
	// encode the pixi -- this should be unique
	//RAC added encoding of dwFlags, because the default would have no transparency
	//pID[0] = (BYTE) (((8 - pixi.nAlphaResidual) << 5) | pixi.nBPP); 

	pID[0] = (BYTE) (((8 - pixi.nAlphaResidual ) << 5) | (pixi.nBPP & 0x03) | dwFlags);
	pID[1] = (BYTE) (((8 - pixi.nRedResidual) << 5) | pixi.nRedShift);
	pID[2] = (BYTE) (((8 - pixi.nGreenResidual) << 5) | pixi.nGreenShift);
	pID[3] = (BYTE) (((8 - pixi.nBlueResidual) << 5) | pixi.nBlueShift);
	// encode the transparency
	pID[4] = (BYTE) (dwTransColor & 0xFF);
	pID[5] = (BYTE) ((dwTransColor & 0xFF00) >> 8);
	pID[6] = (BYTE) ((dwTransColor & 0xFF0000) >> 16);
	pID[7] = 0;
}

void
CColorInfo::GetHashID(BYTE nBPP, const PALETTEENTRY *rgpePalette, DWORD dwTransColor,
					  DWORD dwFlags, LPBYTE pID)
{
	// REVIEW: this might not be unique, which is another reason to redo the hash stuff right
	MMASSERT(rgpePalette && pID && (sizeof(PALETTEENTRY) == sizeof(DWORD)));

	// compute a hash value for the palette
	DWORD nHash = 0;
	const DWORD *pdwPalette = (const DWORD *) rgpePalette;
	// assume that the PALETTEENTRY array has unused entries zeroed out
	const DWORD *pdwLimit = pdwPalette + nMAXPALETTEENTRIES;
	do {
		nHash += (nHash >> 11) + pdwPalette[0];
		nHash += (nHash >> 11) + pdwPalette[1];
		nHash += (nHash >> 11) + pdwPalette[2];
		nHash += (nHash >> 11) + pdwPalette[3];
		pdwPalette += 4;
	} while (pdwPalette != pdwLimit);

	pID[0] = (((dwFlags & flagTRANSPARENT) != NULL) << 7) | nBPP;
	// encode the hash as a string -- make sure all the entries are greater than zero
	pID[1] = (BYTE) ((nHash | 0x1) & 0x7F);
	pID[2] = (BYTE) (((nHash >> 8) | 0x1) & 0x7F);
	pID[3] = (BYTE) (((nHash >> 16) | 0x1) & 0x7F);
	pID[4] = (BYTE) (((nHash >> 24) | 0x1) & 0x7F);
	pID[5] = (BYTE) ((nHash & 0x81) | (((nHash >> 8) & 0x81) << 1) | 
					 (((nHash >> 16) & 0x81) << 2) | (((nHash >> 24) & 0x81) << 3));
	// encode the transparency
	pID[6] = (BYTE) (dwTransColor & 0xFF);
	pID[7] = 0;
}


//
// CColorManager
//

HRESULT
CColorManager::Reset()
{
	MMRELEASE(m_pddpTarget);
//	INIT_DXSTRUCT(m_ddpfTarget);
	m_pixiTarget.Init();
	m_bActiveTarget = FALSE;

	// delete all of the CColorInfo objects
	LPCOLORINFO pci;
	for (int cLength = m_listColorInfo.GetCount(); cLength != 0; cLength--) {
		pci = (LPCOLORINFO) m_listColorInfo.RemoveHead();
		MMASSERT(pci);
		pci->SetColMgr(NULL);
		pci->Release();
	}
	return S_OK;
}

CColorManager::CColorManager()
{
	m_nRef = 1;
	m_pDD = NULL;
	m_bActiveTarget = FALSE;
	m_pddpTarget = NULL;
//	INIT_DXSTRUCT(m_ddpfTarget);
}

CColorManager::~CColorManager()
{
	HRESULT hr;
	hr = Reset();
	MMRELEASE(m_pDD);
	MMASSERT(SUCCEEDED(hr));
}

HRESULT
CColorManager::Init(LPDIRECTDRAW pDD)
{
	if (m_pDD)
		return E_ALREADYINITIALIZED;

	if ((m_pDD = pDD) == NULL)
		return E_INVALIDOBJECT;

	pDD->AddRef();
	return S_OK;
}

HRESULT
CColorManager::SetVideoMode(const DDPIXELFORMAT &ddpf, LPDIRECTDRAWPALETTE pddp, 
							LPBOOL pbChanged)
{
	MMTRACE("CColorManager::SetVideoMode: \tBPP = %d  Palette = 0x%p\n", ddpf.dwRGBBitCount, pddp);
	MMASSERT(pbChanged);
	HRESULT hr;

	// verify arguments
	if ((pddp && (ddpf.dwRGBBitCount > 8)) || (!pddp && (ddpf.dwRGBBitCount <= 8))) {
		*pbChanged = FALSE;
		return E_INVALIDARG;
	}

	// check if this is the same video mode as last time
	if ((pddp == m_pddpTarget) && (m_pixiTarget == ddpf)) {
		*pbChanged = FALSE;
		return S_OK;
	}

	// REVIEW: fix this later...
	if (m_pixiTarget.nBPP > 8)
	{
		// technically, this will work for the color manager, 
		// but other code needs to be fixed if this happens -- like reloading RGB...
		MMTRACE("error: changing video mode once set to RGB is not fully implemented\n");
		return E_NOTIMPL;
	}

	// store the new video mode
	m_pixiTarget.Init(ddpf);
	MMRELEASE(m_pddpTarget);
	if (m_pddpTarget = pddp)
		pddp->AddRef();
	m_bActiveTarget = TRUE;

	// remap all of the currently active ColorInfo objects to the new color model
	LPCOLORINFO pci;
	POSITION pos = m_listColorInfo.GetHeadPosition();
	if (pos) {
		pci = (LPCOLORINFO) m_listColorInfo.GetAt(pos);
		do {
			MMASSERT(pci);
			if (pci->pddp && 
				FAILED(hr = pci->pmVideoMode.CreateMap(pci->pddp, m_pixiTarget, m_pddpTarget)))
			{
				return hr;
			}
			pci = (LPCOLORINFO) m_listColorInfo.GetNext(pos);
		} while (pos);
	}

	*pbChanged = TRUE;
	return S_OK;
}


HRESULT
CColorManager::GetColorInfo(const CPixelInfo &pixiRGBPixFmt, DWORD dwTransColor, DWORD dwFlags, 
							LPCOLORINFO *ppci)
{
	// verify arguments
	if (pixiRGBPixFmt.nBPP <= 8)
		return E_INVALIDARG;

	return GetColorInfo(pixiRGBPixFmt, NULL, NULL, (const BYTE *) NULL, dwTransColor, dwFlags, ppci);
}


// Function: GetColorInfo
HRESULT
CColorManager::GetColorInfo(const CPixelInfo &pixiPalFmt, BYTE nBPPPalette, LPBYTE pPalette, 
							BYTE nBPPPixels, DWORD dwTransColor, DWORD dwFlags, LPCOLORINFO *ppci)
{
	MMASSERT(sizeof(PALETTEENTRY) == sizeof(DWORD));
	PALETTEENTRY rgpe[nMAXPALETTEENTRIES];

	// verify arguments
	if ((pPalette == NULL) || (nBPPPalette > 8) || (nBPPPalette == 0) || 
		(nBPPPixels < nBPPPalette))
	{
		return E_INVALIDARG;
	}

	DWORD cEntries = (1 << nBPPPalette);

	BltFastRGBToRGB(pPalette, 0, (LPBYTE) rgpe, 0, cEntries, 1, pixiPalFmt, g_pixiPALETTEENTRY);
	ZeroDWORDAligned((LPDWORD) rgpe + cEntries, nMAXPALETTEENTRIES - cEntries);

	// All this is to fix a bug in some card's driver's. They seem to need the transparency
	// color to be black, so we set it to black. We also must make sure any other colors
	// in the palette that were black are no longer quite black or else they will sometimes
	// turn out transparent (since black is now our transparent color, even though its a
	// different palette index).
	if (dwFlags & flagTRANSPARENT)
	{
		for (DWORD i = 0; i < nMAXPALETTEENTRIES; i++)
		{
			if (rgpe[i].peRed == 0 && rgpe[i].peGreen == 0 && rgpe[i].peBlue == 0)
			{
				rgpe[i].peRed = 4;
				rgpe[i].peGreen = 4;
				rgpe[i].peBlue = 4;
			}
		}
		int nIndex = (dwTransColor);// * sizeof(PALETTEENTRY);
		rgpe[nIndex].peRed = 0;
		rgpe[nIndex].peGreen = 0;
		rgpe[nIndex].peBlue = 0;
	}

	CPixelInfo pixiPixFmt(nBPPPixels);
	return GetColorInfo(pixiPixFmt, rgpe, NULL, NULL, dwTransColor, dwFlags, ppci);
}


HRESULT
CColorManager::GetColorInfo(LPDIRECTDRAWPALETTE pddp, DWORD dwTransColor, 
							DWORD dwFlags, LPCOLORINFO *ppci)
{
	PALETTEENTRY rgpe[nMAXPALETTEENTRIES];
	HRESULT hr = E_INVALIDOBJECT;

	if ((pddp == NULL) ||
		FAILED(hr = pddp->GetEntries(0, 0, nMAXPALETTEENTRIES, rgpe)))
		return hr;

	DWORD dwCaps;
	pddp->GetCaps(&dwCaps);
	BYTE nBPPDst = PaletteFlagsToBPP(dwCaps);

	DWORD cEntries = (1 << nBPPDst);
	ZeroDWORDAligned((LPDWORD) rgpe + cEntries, nMAXPALETTEENTRIES - cEntries);

	CPixelInfo pixiPixFmt(nBPPDst);
	return GetColorInfo(pixiPixFmt, rgpe, pddp, NULL, dwTransColor, dwFlags, ppci);
}


HRESULT
CColorManager::GetDefaultColorInfo(BYTE nBPP, LPCOLORINFO *ppci)
{
	static BYTE rgID[nCIHASHIDLENGTH] = "DEF:8";
	rgID[4] = nBPP;
	PALETTEENTRY rgpe[nMAXPALETTEENTRIES];

	LPPALETTEENTRY ppe;
	DWORD dwTransColor = 3, dwFlags = flagPALETTIZED;
	switch (nBPP) {
	case 1: ppe = g_rgpeDefPal2; break;
	case 2: ppe = g_rgpeDefPal4; dwFlags |= flagTRANSPARENT; break;
	case 4: ppe = g_rgpeDefPal16; break;
	case 8: ppe = g_rgpeDefPal256; break;
	default:
		return E_INVALIDARG;
	}
	// REVIEW: for now, assume that the pixel format is always 8 bit

	// copy the paletteentries over to the temporary table
	DWORD cEntries = 1 << nBPP;
	for (DWORD i = 0; i < cEntries; i++)
		rgpe[i] = ppe[i];
	// zero out the rest
	ZeroDWORDAligned((LPDWORD) rgpe + cEntries, nMAXPALETTEENTRIES - cEntries);

	CPixelInfo pixi(8);
	return GetColorInfo(pixi, rgpe, NULL, rgID, dwTransColor, dwFlags, ppci);
}


HRESULT
CColorManager::GetPreferredPixelFormat(BYTE nBPP, CPixelInfo *ppixi)
{
	MMASSERT(ppixi);

	// if the requested bit depth is the same as the current video mode,
	// use that as the preferred pixel format
	if (nBPP == m_pixiTarget.nBPP) {
		*ppixi = m_pixiTarget;
		return S_OK;
	}
	MMTRACE("CColorManager error: pixel format enumeration not implemented\n");
	return E_NOTIMPL;
}


HRESULT
CColorManager::GetColorInfo(const CPixelInfo &pixiPixFmt, const PALETTEENTRY *rgpe, 
							LPDIRECTDRAWPALETTE pddp, const BYTE *pID,
							DWORD dwTransColor, DWORD dwFlags, LPCOLORINFO *ppci)
{
	if (m_pDD == NULL)
		return E_NOTINITIALIZED;

	if (!IsValidColor(dwTransColor, pixiPixFmt.nBPP))
		return E_INVALIDARG;

	MMASSERT(ppci);

	BYTE rgID[nCIHASHIDLENGTH];
	LPCOLORINFO pci = NULL;
	HRESULT hr;

	// create a hash ID if necessary
	if (pID == NULL) {
		if (rgpe)
			CColorInfo::GetHashID(pixiPixFmt.nBPP, rgpe, dwTransColor, dwFlags, rgID);
		else
			CColorInfo::GetHashID(pixiPixFmt, dwTransColor, dwFlags, rgID);
		pID = rgID;
	}

	// check if the ColorInfo object is in the cache
	LPVOID pvci;
	if (m_mapColorInfo.Lookup((const char *) pID, pvci) == FALSE) {
		//
		// the ColorInfo object does not exist in the cache yet
		//
		if (pixiPixFmt.nBPP <= 8) {
			if (pddp) {
				pddp->AddRef();
			} else {
				MMASSERT(rgpe);
				// create a new palette
				DWORD dwPalFlags = BPPToPaletteFlags(pixiPixFmt.nBPP) | DDPCAPS_ALLOW256;
				if (FAILED(hr = m_pDD->CreatePalette(dwPalFlags, (PALETTEENTRY *) rgpe, &pddp, NULL)))
					return hr;
			}
		}

		if (FAILED(hr = CreateColorInfo(pixiPixFmt, pddp, dwTransColor, dwFlags, &pci)) ||
			FAILED(hr = InsColorInfo((const char *) pID, pci)))
		{
			MMRELEASE(pddp);
			MMRELEASE(pci);
			return hr;
		}
		MMASSERT(pci);
		MMRELEASE(pddp);
	} else {
		//
		// use the cached ColorInfo object
		//
		pci = (LPCOLORINFO) pvci;
		MMASSERT(pci);
		pci->AddRef();
	}
	*ppci = pci;
	return S_OK;
}


HRESULT
CColorManager::CreateColorInfo(const CPixelInfo &pixi, LPDIRECTDRAWPALETTE pddp, DWORD dwTransColor, 
							   DWORD dwFlags, LPCOLORINFO *ppci)
{
	MMASSERT(ppci);
	LPCOLORINFO pci = new CColorInfo;
	if (pci == NULL)
		return E_OUTOFMEMORY;

	pci->pixi = pixi;
	pci->bvColorFlags = 0;
	pci->dwTransColor = dwTransColor;
	pci->bvColorFlags |= (dwFlags & flagTRANSPARENT);
	if (pci->pddp = pddp) {
		pddp->AddRef();
		pci->bvColorFlags |= flagPALETTIZED;
	}
	*ppci = pci;
	return S_OK;
}


HRESULT
CColorManager::InsColorInfo(const char *szID, LPCOLORINFO pci)
{
	MMASSERT(szID && pci);
	HRESULT hr;

	pci->SetColMgr(this);

	// put the image in the file map
	m_mapColorInfo.SetAt(szID, (LPVOID) pci);

	// put the image into the list
	m_listColorInfo.AddHead((LPVOID) pci);

	// REVIEW: these things should never go away until the ColMgr is deleted -- fix
	//  this later when we create a better "map" class
	// keep a reference in the CColorManager
	pci->AddRef();

	// map this CColorInfo object to the current video mode
	if (m_bActiveTarget && pci->pddp) {
		if (FAILED(hr = pci->pmVideoMode.CreateMap(pci->pddp, m_pixiTarget, m_pddpTarget)))
			return hr;
	}
	return S_OK;
}
