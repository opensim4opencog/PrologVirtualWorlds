// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// File:	PalMap.cpp
// Author:	MM    (MM)

#include "MMImage.h"

char gs_szPMPrefix[] = "palette map error";


CPaletteMap::CPaletteMap()
{
	m_rgIndexMap = NULL;
	m_nConvertCode = cvcInvalid;
}

CPaletteMap::~CPaletteMap()
{
	MMDELETE(m_rgIndexMap);
}

// Function: CreateMap
//    This function creates a new mapping from a src palette to a destination color model.
HRESULT
CPaletteMap::CreateMap(BYTE nBPPSrcPixels, BYTE nBPPSrcPalette, LPPALETTEENTRY rgpeSrc, 
					   const CPixelInfo &pixiDst, LPDIRECTDRAWPALETTE pddpDst)
{
//	MMTRACE("CPaletteMap::CreateMap\n");
	HRESULT hr;
	PALETTEENTRY rgpeDst[256];
	DWORD dwDstCaps;

	// verify arguments
	if (rgpeSrc == NULL)
		return E_INVALIDARG;

	// delete the old index map, if it exists
	MMDELETE(m_rgIndexMap);

	// store the bit depths for mapping verification
	// REVIEW: perhaps the maps should be created with at least 256 entries always
	m_cSrcBPP = nBPPSrcPixels;
	m_cDstBPP = pixiDst.nBPP;

	// figure out what kind of conversion we are doing
	if ((m_nConvertCode = GetConvertCode(m_cSrcBPP, m_cDstBPP)) == cvcInvalid) {
		MMTRACE("%s: can't convert from %d bit to %d bit\n", 
			gs_szPMPrefix, (int) m_cSrcBPP, (int) m_cDstBPP);
		return E_INVALIDARG;
	}

	if (pddpDst == NULL) {
		// destination is RGB
		switch (m_cDstBPP) {
		case 16: return DoPalTo16BitMap(nBPPSrcPalette, pixiDst, rgpeSrc); break;
		case 24: return DoPalTo24BitMap(nBPPSrcPalette, pixiDst, rgpeSrc); break;
		case 32: return DoPalTo32BitMap(nBPPSrcPalette, pixiDst, rgpeSrc); break;
		default:
			return E_INVALIDARG;
			break;
		}
	} else {
		// destination is 8 bit palettized
		hr = E_INVALIDARG;
		if ((m_cDstBPP != 8) ||
			// get the caps
			FAILED(pddpDst->GetCaps(&dwDstCaps)) ||
			// verify we have True Color entries
			(dwDstCaps & DDPCAPS_8BITENTRIES) ||
			// make sure the number of palette entries from the caps is 8 bits
			(!(dwDstCaps & DDPCAPS_8BIT)) ||
			// get the palette entries
			FAILED(hr = pddpDst->GetEntries(0, 0, 1 << m_cDstBPP, rgpeDst)))
		{
			MMTRACE("%s: invalid dst palette for map\n", gs_szPMPrefix);
			return hr;
		}
		// create map for palette to palette
		return DoPalToPalMap(nBPPSrcPalette, m_cDstBPP, rgpeSrc, rgpeDst);
	}
}


HRESULT
CPaletteMap::CreateMap(LPDIRECTDRAWPALETTE pddpSrc, const CPixelInfo &pixiDst, 
					   LPDIRECTDRAWPALETTE pddpDst)
{
//	MMTRACE("CPaletteMap::CreateMap\n");
	PALETTEENTRY rgpeSrc[256];
	BYTE nBPPSrc;
	DWORD dwSrcCaps;

	// sanitize the src palette and get the srcBPP
	HRESULT hr = E_INVALIDARG;
	if ((pddpSrc == NULL) ||
		// get the caps
		FAILED(pddpSrc->GetCaps(&dwSrcCaps)) ||
		// verify we have True Color entries
		(dwSrcCaps & DDPCAPS_8BITENTRIES) ||
		// get the number of palette entries from the caps
		((nBPPSrc = PaletteFlagsToBPP(dwSrcCaps)) == 0) ||
		// get the palette entries
		FAILED(hr = pddpSrc->GetEntries(0, 0, (1 << nBPPSrc), rgpeSrc)))
	{
		MMTRACE("%s: invalid src palette for map\n", gs_szPMPrefix);
		return hr;
	}

	return CreateMap(nBPPSrc, nBPPSrc, rgpeSrc, pixiDst, pddpDst);
}


HRESULT
CPaletteMap::CreateSortedMap(BYTE nBPP, const RGB *rgrgbSrc, BYTE nBPPUsed, DWORD iTransColor, 
							 DWORD dwFlags, LPPALETTEENTRY rgpeDst)
{
	MMTRACE("CPaletteMap::CreateSortedMap\n");
	MMASSERT(nBPP <= nBPPUsed);
	DWORD i, j, imin;

	if ((rgrgbSrc == NULL) || (nBPPUsed > 8))
		return E_INVALIDARG;

	struct {
		DWORD	nPos;
		int		nLuminance;
	} rgSortMap[nMAXPALETTEENTRIES], minLuminance;

	// allocate the index map
	MMDELETE(m_rgIndexMap);

	m_rgIndexMap = (BYTE *) new BYTE[1 << nBPPUsed];
	if (m_rgIndexMap == NULL)
		return E_OUTOFMEMORY;

	m_nConvertCode = GetConvertCode(nBPPUsed, nBPPUsed);
	MMASSERT(m_nConvertCode == cvc8To8);
	m_cSrcBPP = nBPPUsed;
	m_cDstBPP = nBPPUsed;
//	m_pixiDst.Init(nBPPUsed);

	// initialize the sort map (compute luminance values)
	DWORD cMapLength = (1 << nBPP), cTotalEntries = (1 << nBPPUsed);
	for (i = 0; i < cMapLength; i++) {
		const RGB &rgbTmp = rgrgbSrc[i];
		rgSortMap[i].nPos = i;
		rgSortMap[i].nLuminance = nREDWEIGHT * rgbTmp.r + nGREENWEIGHT * rgbTmp.g + 
			nBLUEWEIGHT * rgbTmp.b;
	}

	// if transparency exists, change its luminance to -1 so it will
	// become the zeroth index
	if (dwFlags & flagTRANSPARENT) {
		if (iTransColor > cMapLength)
			return E_INVALIDARG;
		rgSortMap[iTransColor].nLuminance = -1;
	}

	// sort the entries by luminance
	// REVIEW: use naive insertion sort for now
	for (i = 0; i < cMapLength; i++) {
		imin = i;
		minLuminance = rgSortMap[imin];

		for (j = i + 1; j < cMapLength; j++) {
			if (minLuminance.nLuminance > rgSortMap[j].nLuminance) {
				imin = j;
				minLuminance = rgSortMap[imin];
			}
		}
		rgSortMap[imin] = rgSortMap[i];
		rgSortMap[i] = minLuminance;
	}

	// fill in the index map (sorting generates an "inverse" map)
	for (i = 0; i < cMapLength; i++) {
		m_rgIndexMap[rgSortMap[i].nPos] = (BYTE) i;
	}
	for (; i < cTotalEntries; i++)
		m_rgIndexMap[i] = (BYTE) i;


	// sort to a palette entry array based on this mapping
	if (rgpeDst) {
		for (i = 0; i < cMapLength; i++) {
			PALETTEENTRY &pe = rgpeDst[i];
			const RGB &rgb = rgrgbSrc[rgSortMap[i].nPos];
			pe.peRed = rgb.r; pe.peGreen = rgb.g; pe.peBlue = rgb.b; pe.peFlags = 0;
		}
		PALETTEENTRY peZero = {0, 0, 0, 0};
		for (; i < cTotalEntries; i++)
			rgpeDst[i] = peZero;
	}

	return S_OK;
}


HRESULT
CPaletteMap::DoPalTo16BitMap(BYTE cSrcBPP, const CPixelInfo &pixiDst, const PALETTEENTRY *ppeSrc)
{
	MMASSERT(ppeSrc);

	DWORD cEntries = (1 << cSrcBPP);
	MapEntry16 *pIndexMap = new MapEntry16[cEntries];
	if (pIndexMap == NULL)
		return E_OUTOFMEMORY;

	for (DWORD i = 0; i < cEntries; i++) {
		pIndexMap[i] = pixiDst.Pack16(ppeSrc[i]);
	}

	m_rgIndexMap = (BYTE *) pIndexMap;
	return S_OK;
}


HRESULT
CPaletteMap::DoPalTo24BitMap(BYTE cSrcBPP, const CPixelInfo &pixiDst, const PALETTEENTRY *ppeSrc)
{
	MMASSERT(ppeSrc);

	if ((pixiDst.nRedResidual | pixiDst.nGreenResidual | pixiDst.nBlueResidual) != 0)
		return DDERR_INVALIDPIXELFORMAT;

	DWORD cEntries = (1 << cSrcBPP);
	MapEntry24 *pIndexMap = new MapEntry24[cEntries];
	if (pIndexMap == NULL)
		return E_OUTOFMEMORY;

	for (DWORD i = 0; i < cEntries; i++) {
		pIndexMap[i] = pixiDst.Pack(ppeSrc[i]);
	}

	m_rgIndexMap = (BYTE *) pIndexMap;
	return S_OK;
}


HRESULT
CPaletteMap::DoPalTo32BitMap(BYTE cSrcBPP, const CPixelInfo &pixiDst, const PALETTEENTRY *ppeSrc)
{
	// REVIEW: since PALETTEENTRY does not have an alpha field, 
	//  this should be the same as 24 bit
	return DoPalTo24BitMap(cSrcBPP, pixiDst, ppeSrc);
}


// Function: DoPalToPalMap
//    Compute a mapping from one palette to another and store in the palette map.
HRESULT
CPaletteMap::DoPalToPalMap(BYTE cSrcBPP, BYTE cDstBPP, const PALETTEENTRY *ppeSrc, 
						   const PALETTEENTRY *ppeDst)
{
	MMASSERT(ppeSrc && ppeDst);

	DWORD cSrcEntries = (1 << cSrcBPP), cDstEntries = (1 << cDstBPP);
	m_rgIndexMap = new BYTE[cSrcEntries];
	if (m_rgIndexMap == NULL)
		return E_OUTOFMEMORY;
	for (DWORD i = 0; i < cSrcEntries; i++) {
		const PALETTEENTRY &pe = ppeSrc[i];
		m_rgIndexMap[i] = (BYTE) SimpleFindClosestIndex(ppeDst, cDstEntries, 
									pe.peRed, pe.peGreen, pe.peBlue);
	}

	return S_OK;
}


// Function: GetConvertCode
//    This function computes the index into the function arrays for
//  mapping and color conversion.
int
CPaletteMap::GetConvertCode(DWORD nSrcBPP, DWORD nDstBPP)
{
	int nCode;

	if ((nDstBPP < 8) || (nSrcBPP > 8) || (nSrcBPP < 4)) {
		nCode = cvcInvalid;
	} else {
		nCode = (((nSrcBPP >> 2) - 1) << 2) | ((nDstBPP >> 3) - 1);
	}
	return nCode;	
}


static DWORD
GetColor8To8(DWORD dwSrcColor, const BYTE *pIndexMap)
{
	MMASSERT(dwSrcColor < 256);
	return (DWORD) pIndexMap[dwSrcColor];
}

static DWORD
GetColor8To16(DWORD dwSrcColor, const BYTE *pIndexMap)
{
	MMASSERT(dwSrcColor < 256);
	MapEntry16 *pIndexMap16 = (MapEntry16 *) pIndexMap;
	return (DWORD) pIndexMap16[dwSrcColor];
}

static DWORD
GetColor8To24(DWORD dwSrcColor, const BYTE *pIndexMap)
{
	MMASSERT(dwSrcColor < 256);
	MapEntry24 *pIndexMap24 = (MapEntry24 *) pIndexMap;
	return (DWORD) pIndexMap24[dwSrcColor];
}

static DWORD
GetColor8To32(DWORD dwSrcColor, const BYTE *pIndexMap)
{
	MMASSERT(dwSrcColor < 256);
	MapEntry32 *pIndexMap32 = (MapEntry32 *) pIndexMap;
	return (DWORD) pIndexMap32[dwSrcColor];
}

static GetColorFunction gs_rgGetColorFunctions[cvcNumCodes] = {
	NULL, NULL, NULL, NULL,
	GetColor8To8, GetColor8To16, 
	GetColor8To24, GetColor8To32
};

DWORD
CPaletteMap::GetIndexMapping(DWORD iSrcColor) const
{
	MMASSERT((m_nConvertCode < cvcInvalid) && (gs_rgGetColorFunctions[m_nConvertCode] != NULL));
	return gs_rgGetColorFunctions[m_nConvertCode](iSrcColor, m_rgIndexMap);
}
