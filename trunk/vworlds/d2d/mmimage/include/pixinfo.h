// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _PixInfo_h
#define _PixInfo_h

// File:	PixInfo.h
// Author:	MM    (MM)
//
// Description:
//    Store the PixelFormat information in a form that is actually useful
//  to an application.
//
// ***Hungarian: pixi
// 
// History:
// -@- 6/24/97 (MM) created -- snarfed from PalMap.h

class CPixelInfo {
public:
	HRESULT			Init(BYTE tnBPP = 0) { nBPP = tnBPP; nAlphaResidual = 8; return S_OK; }
	HRESULT			Init(BYTE nBPP, DWORD dwRedMask, DWORD dwGreenMask,
						DWORD dwBlueMask, DWORD dwAlphaMask);
	HRESULT			Init(const DDPIXELFORMAT &ddpf)
					{
						HRESULT hr;
						hr = Init((BYTE) ddpf.dwRGBBitCount, ddpf.dwRBitMask, ddpf.dwGBitMask, 
								  ddpf.dwBBitMask, ddpf.dwRGBAlphaBitMask);
						if (!(ddpf.dwFlags & DDPF_ALPHAPIXELS))
							nAlphaResidual = 8;
						return hr;
					}

					CPixelInfo(BYTE nBPP = 0) { Init(nBPP); }
					CPixelInfo(BYTE nBPP, DWORD dwRedMask, DWORD dwGreenMask,
						DWORD dwBlueMask, DWORD dwAlphaMask){
							Init(nBPP, dwRedMask, dwGreenMask, dwBlueMask, dwAlphaMask); }
					CPixelInfo(const DDPIXELFORMAT &ddpf) { Init(ddpf); }
	

	void			GetDDPF(DDPIXELFORMAT &ddpf) const;
	BOOL			HasAlpha() const				{ return (nAlphaResidual != 8); }

	BOOL			operator==(const CPixelInfo &pixi) const;
	BOOL			operator==(const DDPIXELFORMAT &ddpf) const;
	BOOL			operator!=(const DDPIXELFORMAT &ddpf) const { return !(*this == ddpf); }

	// generic
	DWORD			Pack(const BYTE *pPixels) const;
	DWORD			Pack(BYTE r, BYTE g, BYTE b) const;
	DWORD			Pack(BYTE r, BYTE g, BYTE b, BYTE a) const;
	DWORD			Pack(const PALETTEENTRY &pe) const	{ return Pack(pe.peRed, pe.peGreen, pe.peBlue); }
	void			UnPack(DWORD dwPixel, BYTE *pR, BYTE *pG, BYTE *pB, BYTE *pA) const;
	void			UnPack(DWORD dwPixel, BYTE *pR, BYTE *pG, BYTE *pB) const;
	DWORD			TranslatePack(DWORD dwSrcPixel, const CPixelInfo &pixiSrcFmt) const;

	// explicit
	WORD			Pack16(BYTE r, BYTE g, BYTE b) const;
	WORD			Pack16(BYTE r, BYTE g, BYTE b, BYTE a) const;
	WORD			Pack16(const PALETTEENTRY &pe) const	{ return Pack16(pe.peRed, pe.peGreen, pe.peBlue); }

public:
	BYTE			nBPP;
	BYTE			nRedShift, nRedResidual;
	BYTE			nGreenShift, nGreenResidual;
	BYTE			nBlueShift, nBlueResidual;
	BYTE			nAlphaShift, nAlphaResidual;
	BYTE			iRed, iBlue;
};

// define common pixi configurations
extern CPixelInfo g_pixiNULL;
extern CPixelInfo g_pixiPalette8;
extern CPixelInfo g_pixiRGB;
extern CPixelInfo g_pixiBGR;
extern CPixelInfo g_pixiRGBA;
extern CPixelInfo g_pixiBGRA;
extern CPixelInfo g_pixiRGBTRIPLE;
extern CPixelInfo g_pixiRGBQUAD;
extern CPixelInfo g_pixiPALETTEENTRY;

#endif
