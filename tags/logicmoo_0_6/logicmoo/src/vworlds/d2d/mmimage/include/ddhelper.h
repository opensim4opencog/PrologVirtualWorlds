// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _DDHelper_h
#define _DDHelper_h

// File:	DDHelper.h
// Author:	MM    (MM)
//
// Description:
//    These are some useful helper functions for sanitizing interactions
//  with DirectDraw
// 
// History:
// -@- 3/6/97 (MM) created

#ifndef __DDRAW_INCLUDED__
#include <ddraw.h>
#endif
#ifndef _D2DUtils_h
#include "D2DUtils.h"
#endif

class CColorInfo;
class CColorManager;

#ifndef LPD2DRECT
typedef struct D2DRect *LPD2DRECT;
#endif

extern char g_szEOFMessage[];

//
// functions to convert between palette & pixel format flags, and bpp
//
extern DWORD g_rgdwBPPToPalFlags[9];
extern DWORD g_rgdwBPPToPixFlags[9];

inline DWORD
BPPToPaletteFlags(DWORD nBPP)
{
	return (nBPP <= 8 ? g_rgdwBPPToPalFlags[nBPP] : 0);
}

inline DWORD
BPPToPixelFlags(DWORD nBPP)
{
	return (nBPP <= 8 ? g_rgdwBPPToPixFlags[nBPP] : 0);
}

DWORD		PaletteToPixelFlags(DWORD dwPaletteFlags);
DWORD		PixelToPaletteFlags(DWORD dwPaletteFlags);
BYTE		PixelFlagsToBPP(DWORD dwFlags);
BYTE		PaletteFlagsToBPP(DWORD dwFlags);

HRESULT DDSetColorKey(IDirectDrawSurface *pdds, COLORREF rgb);
DWORD DDColorMatch(IDirectDrawSurface *pdds, COLORREF rgb);




HRESULT		CreatePlainSurface(LPDIRECTDRAW pDD, DWORD nWidth, DWORD nHeight, DWORD nBPP, 
				const DDPIXELFORMAT *pddpf, LPDIRECTDRAWPALETTE pPalette, LPDIRECTDRAWSURFACE *ppdds, VARIANT_BOOL bForTexture = VARIANT_FALSE);

HRESULT		CreatePlainDefaultSurface(LPDIRECTDRAW pDD, DWORD nWidth, DWORD nHeight, COLORREF clrTransp,
						  LPDIRECTDRAWPALETTE pPalette,
						  DDSURFACEDESC *pddsd,
						  LPDIRECTDRAWSURFACE *ppdds);

HRESULT		ClearToColor(DWORD dwColor, LPRECT prDst, LPDIRECTDRAWSURFACE pdds);
HRESULT		CreateSurfaceWithText(LPDIRECTDRAW pDD, CColorManager *pColMgr, 
				const char *szText, 
				HFONT hFont, COLORREF crFontColor, BOOL bShadowed, COLORREF crShadowColor, const SIZE* fittedSize, 
				SIZE *psiz, CColorInfo **ppciText, LPDIRECTDRAWSURFACE *ppdds);

HRESULT		CreatePlainDIBSection(HDC hDC, DWORD nWidth, DWORD nHeight, DWORD nBPP, 
				const PALETTEENTRY *rgpePalette, HBITMAP *phbm, LPBYTE *ppPixels);

HRESULT		CopyPixelsToDDS(const BYTE *pSrcPixels, RECT rSrc, long nSrcPitch,
				LPDIRECTDRAWSURFACE pddsDst, DWORD nXPos, DWORD nYPos);

HRESULT		GetSurfaceDimensions(LPDIRECTDRAWSURFACE pdds, LPRECT prDimensions);
HRESULT		GetSurfaceDimensions(LPDIRECTDRAWSURFACE pdds, LPD2DRECT prDimensions);

DWORD		SimpleFindClosestIndex(const PALETTEENTRY *rgpePalette, DWORD cEntries, 
				BYTE r, BYTE g, BYTE b);

// useful global variables
extern PALETTEENTRY g_peZero;


// Notes: luminance ~= (77r + 151g + 28b)/256
#define nREDWEIGHT 77
#define nGREENWEIGHT 151
#define nBLUEWEIGHT 28

//
// PALETTE STUFF
//
typedef struct RGB {
	BYTE r, g, b;
} RGB, *LPRGB;

#define nMAXPALETTEENTRIES 256

//void		CopyRGBTRIPLEtoRGB(const RGBTRIPLE *prgbtSrc, LPRGB prgbDst, DWORD cEntries);
//void		CopyRGBQUADtoRGB(const RGBQUAD *prgbqSrc, LPRGB prgbDst, DWORD cEntries);
//void		CopyRGBtoPALETTEENTRY(const RGB *prgb, LPPALETTEENTRY ppe, DWORD cEntries);
void		ZeroDWORDAligned(LPDWORD pdw, DWORD cEntries);


DWORD		GetClosestMultipleOf4(DWORD n, BOOL bGreater);
DWORD		GetClosestPowerOf2(DWORD n, BOOL bGreater);

HRESULT		AllocatePixels(DWORD cPixels, BYTE nBPP, LPBYTE &pPixels);


// RECT functions

// Function: ClipRect
//    Returns TRUE for a non-trivial intersection.
BOOL		ClipRect(const RECT &rTarget, LPRECT prSrc);

inline BOOL
IsInside(long nX, long nY, const RECT &r)
{
	return ((nX >= r.left) && (nX < r.right) && (nY >= r.top) && (nY < r.bottom));
}

// Function: IsValidColor
//    Return if the given color is valid given the bit depth
inline BOOL
IsValidColor(DWORD dwColor, BYTE nBPP)
{
	if ( nBPP == 32 ) return TRUE;
	return (dwColor <= ((DWORD) ((1L << nBPP) - 1)));
}

#endif
