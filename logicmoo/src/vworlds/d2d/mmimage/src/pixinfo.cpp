// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// File:	PixInfo.cpp
// Author:	MM    (MM)

#include "MMImage.h"

// define common pixi configurations
CPixelInfo g_pixiNULL(0);
CPixelInfo g_pixiPalette8(8);
CPixelInfo g_pixiRGB(24, 0xFF, 0xFF00, 0xFF0000, 0);
CPixelInfo g_pixiBGR(24, 0xFF0000, 0xFF00, 0xFF, 0);
CPixelInfo g_pixiRGBA(32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
CPixelInfo g_pixiBGRA(32, 0xFF0000, 0xFF00, 0xFF, 0xFF000000);
CPixelInfo g_pixiRGBTRIPLE = g_pixiBGR;
CPixelInfo g_pixiRGBQUAD(32, 0xFF0000, 0xFF00, 0xFF, 0);		// BGRX
CPixelInfo g_pixiPALETTEENTRY(32, 0xFF, 0xFF00, 0xFF0000, 0);	// RGBX

static void
_GetShiftMaskInfo(DWORD dwMask, BYTE *pnShiftStart, BYTE *pnLengthResidual)
{
	MMASSERT(pnShiftStart && pnLengthResidual);

	DWORD nShift = 0, nBits = 0;
	if (dwMask) {
		for (; !(dwMask & 0x1); dwMask >>= 1, nShift++);
		// DWORD fills with zero on right shift
		for (; (dwMask & 0x1); dwMask >>= 1, nBits++);
	}
	MMASSERT(nBits <= 8);
	*pnShiftStart = (BYTE) nShift;
	*pnLengthResidual = (BYTE) (8 - nBits);
}


HRESULT
CPixelInfo::Init(BYTE tnBPP, DWORD dwRedMask, DWORD dwGreenMask,
				 DWORD dwBlueMask, DWORD dwAlphaMask)
{
	MMASSERT((NUMBITS(BYTE) == 8) && (NUMBITS(WORD) == 16) && (NUMBITS(DWORD) == 32));

	if ((nBPP = tnBPP) <= 8) {
		// palettized mode
		return Init(nBPP);
	}

	_GetShiftMaskInfo(dwRedMask, &nRedShift, &nRedResidual);
	_GetShiftMaskInfo(dwGreenMask, &nGreenShift, &nGreenResidual);
	_GetShiftMaskInfo(dwBlueMask, &nBlueShift, &nBlueResidual);
	_GetShiftMaskInfo(dwAlphaMask, &nAlphaShift, &nAlphaResidual);

	if (nBPP >= 24) {
		MMASSERT((nRedShift == 0) || (nRedShift == 16));
		iRed = (nRedShift == 0 ? 0 : 2);
		iBlue = 2 - iRed;
	}

//	MMTRACE("BPP: %2d   R: %2d %2d   G: %2d %2d   B: %2d %2d   A: %2d %2d\n", nBPP,
//		8 - nRedResidual, nRedShift, 8 - nGreenResidual, nGreenShift, 
//		8 - nBlueResidual, nBlueShift, 8 - nAlphaResidual, nAlphaShift);

	return S_OK;
}


void
CPixelInfo::GetDDPF(DDPIXELFORMAT &ddpf) const
{
	memset(&ddpf,0,sizeof(DDPIXELFORMAT));
	ddpf.dwSize = sizeof(DDPIXELFORMAT);
	ddpf.dwFlags = DDPF_RGB;
	ddpf.dwRGBBitCount = nBPP;
	if (nBPP <= 8) {
		ddpf.dwFlags |= BPPToPixelFlags(nBPP);
	} else {
		ddpf.dwRBitMask = ((((DWORD) 0xFF) >> nRedResidual) << nRedShift);
		ddpf.dwGBitMask = ((((DWORD) 0xFF) >> nGreenResidual) << nGreenShift);
		ddpf.dwBBitMask = ((((DWORD) 0xFF) >> nBlueResidual) << nBlueShift);
		if (HasAlpha()) {
			ddpf.dwFlags |= DDPF_ALPHAPIXELS;
			ddpf.dwRGBAlphaBitMask = ((((DWORD) 0xFF) >> nAlphaResidual) << nAlphaShift);
		}
	}
}


BOOL
CPixelInfo::operator==(const CPixelInfo &pixi) const
{
	return ((nBPP == pixi.nBPP) && 
			((nBPP == 8) || ((nRedShift == pixi.nRedShift) && 
							 (nGreenShift == pixi.nGreenShift) &&
							 (nBlueShift == pixi.nBlueShift) &&
							 (nAlphaShift == pixi.nAlphaShift))));
}


BOOL
CPixelInfo::operator==(const DDPIXELFORMAT &ddpf) const
{
	return ((nBPP == ddpf.dwRGBBitCount) &&
			((nBPP == 8) || ((ddpf.dwRBitMask == ((DWORD(0xFF) >> nRedResidual) << nRedShift)) &&
							 (ddpf.dwGBitMask == ((DWORD(0xFF) >> nGreenResidual) << nGreenShift)) &&
							 (ddpf.dwBBitMask == ((DWORD(0xFF) >> nBlueResidual) << nBlueShift)))));
}


DWORD
CPixelInfo::Pack(const BYTE *pPixel) const
{
	MMASSERT(pPixel && (nBPP >= 8));
	if (nBPP == 8)
		return (DWORD) *pPixel;
	if (HasAlpha())
		return Pack(pPixel[0], pPixel[1], pPixel[2]);
	else
		return Pack(pPixel[0], pPixel[1], pPixel[2], pPixel[3]);
}

DWORD
CPixelInfo::Pack(BYTE r, BYTE g, BYTE b) const
{
	MMASSERT(nBPP > 8);
	// truncate the RGB values to fit in allotted bits
	return (((((DWORD) r) >> nRedResidual) << nRedShift) |
		((((DWORD) g) >> nGreenResidual) << nGreenShift) |
		((((DWORD) b) >> nBlueResidual) << nBlueShift));
}

DWORD
CPixelInfo::Pack(BYTE r, BYTE g, BYTE b, BYTE a) const
{
	MMASSERT(nBPP > 8);
	// truncate the alpha value to fit in allotted bits
	return (Pack(r, g, b) | ((((DWORD) a) >> nAlphaResidual) << nAlphaShift));
}

void
CPixelInfo::UnPack(DWORD dwPixel, BYTE *pR, BYTE *pG, BYTE *pB, BYTE *pA) const
{
	MMASSERT(nBPP > 8);
	MMASSERT(pR && pG && pB && pA);

	*pR = (BYTE) (((dwPixel >> nRedShift) & (0xFF >> nRedResidual)) << nRedResidual);
	*pG = (BYTE) (((dwPixel >> nGreenShift) & (0xFF >> nGreenResidual)) << nGreenResidual);
	*pB = (BYTE) (((dwPixel >> nBlueShift) & (0xFF >> nBlueResidual)) << nBlueResidual);
	if (HasAlpha())
		*pA = (BYTE) (((dwPixel >> nAlphaShift) & (0xFF >> nAlphaResidual)) << nAlphaResidual);
	else
		*pA = 0;
}

void
CPixelInfo::UnPack(DWORD dwPixel, BYTE *pR, BYTE *pG, BYTE *pB) const
{
	MMASSERT(nBPP > 8);
	MMASSERT(pR && pG && pB);

	*pR = (BYTE)(((dwPixel >> nRedShift) & (0xFF >> nRedResidual)) << nRedResidual);
	*pG = (BYTE)(((dwPixel >> nGreenShift) & (0xFF >> nGreenResidual)) << nGreenResidual);
	*pB = (BYTE)(((dwPixel >> nBlueShift) & (0xFF >> nBlueResidual)) << nBlueResidual);
}

DWORD
CPixelInfo::TranslatePack(DWORD dwPix, const CPixelInfo &pixiSrc) const
{
	MMASSERT((nBPP > 8) && (pixiSrc.nBPP > 8));
	// REVIEW: this could be optimized by splitting out the cases
	DWORD dwTmp;
	dwTmp = ((((((dwPix >> pixiSrc.nRedShift) & (0xFF >> pixiSrc.nRedResidual)) 
		<< pixiSrc.nRedResidual) >> nRedResidual) << nRedShift) |
	(((((dwPix >> pixiSrc.nGreenShift) & (0xFF >> pixiSrc.nGreenResidual)) 
		<< pixiSrc.nGreenResidual) >> nGreenResidual) << nGreenShift) |
	(((((dwPix >> pixiSrc.nBlueShift) & (0xFF >> pixiSrc.nBlueResidual)) 
		<< pixiSrc.nBlueResidual) >> nBlueResidual) << nBlueShift));
	if (pixiSrc.HasAlpha())
		dwTmp |= (((((dwPix >> pixiSrc.nAlphaShift) & (0xFF >> pixiSrc.nAlphaResidual)) 
					<< pixiSrc.nAlphaResidual) >> nAlphaResidual) << nAlphaShift);
	return dwTmp;
}


WORD
CPixelInfo::Pack16(BYTE r, BYTE g, BYTE b) const
{
	MMASSERT(nBPP == 16);
	return (((((WORD) r) >> nRedResidual) << nRedShift) |
		((((WORD) g) >> nGreenResidual) << nGreenShift) |
		((((WORD) b) >> nBlueResidual) << nBlueShift));
}

WORD
CPixelInfo::Pack16(BYTE r, BYTE g, BYTE b, BYTE a) const
{
	MMASSERT(nBPP == 16);
	return (Pack16(r, g, b) | ((((WORD) a) >> nAlphaResidual) << nAlphaShift));
}
