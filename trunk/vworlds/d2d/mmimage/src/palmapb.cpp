// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// File:	PalMapb.cpp
// Author:	MM    (MM)
// Description:
//    This is the PalMap blt code.

#include "MMImage.h"


// Notes:
//    The convert functions also fix the transparency on the destination objects.
//  A better way to do this stuff might be to have Blt functions and then separate
//  convert functions that cleanup the rest of the image after the Blt.
ConvertFunction g_rgConvertFunctions[cvcNumCodes] = {
	NULL, NULL, NULL, NULL,
	BltFast8To8T, BltFast8To16T, 
	BltFast8To24T, BltFast8To32T
};

// Function: BltFast
//    This function takes a src dds and writes a dst dds using the
//  mapping defined by the PaletteMap.  The src and dst can be the
//  same surface.
HRESULT 
CPaletteMap::BltFast(LPDIRECTDRAWSURFACE pddsSrc, LPRECT prSrc, LPDIRECTDRAWSURFACE pddsDst,
					 DWORD nXPos, DWORD nYPos, DWORD dwFlags) const
{
	if (m_rgIndexMap == NULL)
		return E_NOTINITIALIZED;

	// make sure the surfaces are valid
	if ((pddsSrc == NULL) || (pddsDst == NULL) || (prSrc == NULL)) {
		MMASSERT(0);
		return E_INVALIDOBJECT;
	}

	ConvertFunction pfnConvertFunction;
	HRESULT hr = DDERR_INVALIDOBJECT;
	BOOL bSrcLocked = FALSE, bDstLocked = FALSE;
	DDSURFACEDESC ddsdSrc, ddsdDst;
	INIT_DXSTRUCT(ddsdSrc);
	INIT_DXSTRUCT(ddsdDst);
	long nWidth, nHeight;

	BOOL bTransparent = FALSE;
	DDCOLORKEY	ddck;
	if (!(dwFlags & flagBLT_IGNORETRANSPARENCY) &&
		SUCCEEDED(pddsSrc->GetColorKey(DDCKEY_SRCBLT, &ddck)))
		bTransparent = TRUE;

	//
	// Lock the surfaces
	//
	if (pddsSrc == pddsDst) {
		// REVIEW: this lock could just lock the minimum rectangle...
		if (FAILED(hr = pddsDst->Lock(NULL, &ddsdDst, DDLOCK_WAIT, NULL)))
		{
			ASSERT(FALSE);
			goto e_Convert;
		}
		bSrcLocked = bDstLocked = TRUE;
		// copy the dst info into the src info
		ddsdSrc = ddsdDst;
	} else {

		// REVIEW: this lock could just lock the minimum rectangle...
		if (FAILED(hr = pddsSrc->Lock(NULL, &ddsdSrc, DDLOCK_WAIT, NULL)))
		{
			ASSERT(FALSE);
			goto e_Convert;
		}
		bSrcLocked = TRUE;
		if (FAILED(hr = pddsDst->Lock(NULL, &ddsdDst, DDLOCK_WAIT, NULL)))
		{
			ASSERT(FALSE);
			goto e_Convert;
		}
		bDstLocked = TRUE;
	}

	// verify the image information
	if ((ddsdSrc.ddpfPixelFormat.dwRGBBitCount != m_cSrcBPP) ||
		(ddsdDst.ddpfPixelFormat.dwRGBBitCount != m_cDstBPP)) {
		hr = E_INVALIDOBJECT;
		goto e_Convert;
	}

	//
	// clip 
	//
	long nClipWidth, nClipHeight, nLeft, nTop;
	nWidth = prSrc->right - prSrc->left;
	nHeight = prSrc->bottom - prSrc->top;
	nClipWidth = long(ddsdDst.dwWidth - nXPos);
	nClipHeight = long(ddsdDst.dwHeight - nYPos);
	nLeft = prSrc->left;
	nTop = prSrc->top;
	UPDATEMAX(nClipWidth, 0);
	UPDATEMAX(nClipHeight, 0);
	UPDATEMAX(nWidth, 0);
	UPDATEMAX(nHeight, 0);
	UPDATEMAX(nLeft, 0);
	UPDATEMAX(nTop, 0);
	UPDATEMIN(nClipWidth, nWidth);
	UPDATEMIN(nClipHeight, nHeight);
	if (((nLeft + nClipWidth) > long(ddsdSrc.dwWidth)) ||
		((nTop + nClipHeight) > long(ddsdSrc.dwHeight))) {
		hr = E_INVALIDOBJECT;
		goto e_Convert;
	}

	// REVIEW: for now, fail if we are not dealing with at least 8BPP
	if ((ddsdSrc.ddpfPixelFormat.dwRGBBitCount < 8) || (ddsdDst.ddpfPixelFormat.dwRGBBitCount < 8)) {
		hr = E_FAIL;
		goto e_Convert;
	}
		
	nLeft *= (ddsdSrc.ddpfPixelFormat.dwRGBBitCount >> 3);
	nXPos *= (ddsdDst.ddpfPixelFormat.dwRGBBitCount >> 3);

	pfnConvertFunction = g_rgConvertFunctions[m_nConvertCode];
	if (pfnConvertFunction) {
		hr = pfnConvertFunction(
			LPBYTE(ddsdSrc.lpSurface) + nLeft + (nTop * ddsdSrc.lPitch),
			ddsdSrc.lPitch,
			LPBYTE(ddsdDst.lpSurface) + nXPos + (nYPos * ddsdDst.lPitch),
			ddsdDst.lPitch,
			nClipWidth,
			nClipHeight,
			m_rgIndexMap,
			bTransparent,
			ddck.dwColorSpaceLowValue);
	} else {
		hr = E_NOTIMPL;
		goto e_Convert;
	}

e_Convert:
	// unlock the surfaces
	if (pddsSrc == pddsDst) {
		if (bSrcLocked)
			pddsDst->Unlock(ddsdDst.lpSurface);
	} else {
		if (bDstLocked)
			pddsDst->Unlock(ddsdDst.lpSurface);
		if (bSrcLocked)
			pddsSrc->Unlock(ddsdSrc.lpSurface);
	}

	MMASSERT(SUCCEEDED(hr));
	return hr;
}


HRESULT 
CPaletteMap::BltFast(LPRLESURFACE prlesSrc, LPRECT prSrc, LPDIRECTDRAWSURFACE pddsDst,
					 DWORD nXPos, DWORD nYPos, DWORD dwFlags) const
{
	if (m_rgIndexMap == NULL)
		return E_NOTINITIALIZED;

	// make sure the surfaces are valid
	if ((prlesSrc == NULL) || (pddsDst == NULL) || (prSrc == NULL)) {
		return E_INVALIDOBJECT;
	}

	return E_NOTIMPL;
}
