// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _PalMap_h
#define _PalMap_h

// File:	PalMap.h
// Author:	MM    (MM)
//
// Description:
//    This class helps perform on-the-fly pixel conversions.
// 
// History:
// -@- 11/18/96 (MM) created
// -@- 12/05/96 (MM) modified
//     added code for translating all palettes to 8, 16, 24, and 32 bit;
//     transparency/alpha stuff is not yet implemented
// -@- 06/24/97 (MM) modified
//     removed PixelInfo
//
// Notes:
// REVIEW:
//    I probably will never support a 4 bit mode, since 4 bit stuff might
//  as well be stored as 8 bit since the space savings is small.  However,
//  2 and 1 bit stuff should still be considered, since the space savings
//  could be substantial.  Further, 1 and 2 bit surfaces represent a large
//  class of content - line art, text, FAX, etc.  (2 bits buys BGW +
//  transparency for example).  This type of content tends to be much larger
//  in dimension -- so we need an efficient representation.

typedef WORD MapEntry16;
typedef DWORD MapEntry24;
typedef DWORD MapEntry32;

#define flagTRANSPARENT 0x1
#define flagPALETTIZED  0x2

typedef enum ConvertCode {
	cvc4To8 = 0, cvc4To16, cvc4To24, cvc4To32,
	cvc8To8, cvc8To16, cvc8To24, cvc8To32,
	cvcInvalid, cvcNumCodes
} ConvertCode;

typedef HRESULT		(*ConvertFunction)(const BYTE *pSrcPixels, long nSrcPitch,
						BYTE *pDstPixels, long nDstPitch,
						DWORD nWidth, DWORD nHeight,
						const BYTE *pIndexMap, const BOOL bTransparent, const DWORD dwTransSrc);
typedef DWORD		(*GetColorFunction)(DWORD dwSrcColor, const BYTE *pIndexMap);

extern ConvertFunction g_rgConvertFunctions[cvcNumCodes];

class CPaletteMap {
public:
					CPaletteMap();
					~CPaletteMap();
	
	HRESULT			CreateMap(BYTE nBPPSrcPixels, BYTE nBPPSrcPalette, LPPALETTEENTRY rgpeSrc, 
					   const CPixelInfo &pixiDst, LPDIRECTDRAWPALETTE pddpDst);
	HRESULT			CreateMap(LPDIRECTDRAWPALETTE pddpSrc, const CPixelInfo &pixiDst,
						LPDIRECTDRAWPALETTE pddpDst);

	HRESULT			CreateSortedMap(BYTE nBPP, const RGB *rgrgbSrc, BYTE nBPPUsed, DWORD iTransColor, 
						DWORD dwFlags, LPPALETTEENTRY rgpeDst);

	// REVIEW: eventually these should be moved to Blt.cpp
	HRESULT			BltFast(LPDIRECTDRAWSURFACE pddsSrc, LPRECT prSrc, LPDIRECTDRAWSURFACE pddsDst,
						DWORD nXPos, DWORD nYPos, DWORD dwFlags) const;
	HRESULT			BltFast(LPRLESURFACE prlesSrc, LPRECT prSrc, LPDIRECTDRAWSURFACE pddsDst,
						DWORD nXPos, DWORD nYPos, DWORD dwFlags) const;

	// REVIEW: this is not "clip-safe"
	HRESULT			BltFast(const BYTE *pSrcPixels, long nSrcPitch, BYTE *pDstPixels, long nDstPitch,
						DWORD nWidth, DWORD nHeight, const BOOL bTransparent, const DWORD dwTransSrc);
	
	DWORD			GetIndexMapping(DWORD iSrcColor) const;

	DWORD			GetSrcBPP() const	{ return m_cSrcBPP; }
	DWORD			GetDstBPP() const	{ return m_cDstBPP; }
//	BOOL			IsIdentity() const	{ return m_bIdentity; }
	
private:	
	HRESULT			DoPalTo16BitMap(BYTE nSrcBPP, const CPixelInfo &pixiDst, 
						const PALETTEENTRY *ppeSrc);
	HRESULT			DoPalTo24BitMap(BYTE nSrcBPP, const CPixelInfo &pixiDst, 
						const PALETTEENTRY *ppeSrc);
	HRESULT			DoPalTo32BitMap(BYTE nSrcBPP, const CPixelInfo &pixiDst, 
						const PALETTEENTRY *ppeSrc);
	HRESULT			DoPalToPalMap(BYTE nSrcBPP, BYTE nDstBPP, const PALETTEENTRY *ppeSrc, 
						const PALETTEENTRY *ppeDst);

	static int		GetConvertCode(DWORD nSrcBPP, DWORD nDstBPP);

private:
	BYTE *			m_rgIndexMap;
	BYTE			m_nConvertCode;
	// REVIEW: we don't need to store the src and dst info ==> implicit in ConvertCode
	BYTE			m_cSrcBPP, m_cDstBPP;
	BYTE			m_bIdentity;
};


inline
HRESULT
CPaletteMap::BltFast(const BYTE *pSrcPixels, long nSrcPitch, BYTE *pDstPixels, long nDstPitch,
					 DWORD nWidth, DWORD nHeight, const BOOL bTransparent, const DWORD dwTransSrc)
{
	ConvertFunction pfnConvertFunction = g_rgConvertFunctions[m_nConvertCode];
	if (pfnConvertFunction)
		return pfnConvertFunction(pSrcPixels, nSrcPitch, pDstPixels, nDstPitch, 
					nWidth, nHeight, m_rgIndexMap, bTransparent, dwTransSrc);
	return E_NOTIMPL;
}

#endif
