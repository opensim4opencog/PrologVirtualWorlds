// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// File:	ddhelper.cpp
// Author:	MM    (MM)

#include "MMImage.h"

PALETTEENTRY g_peZero = {0, 0, 0, 0};

char g_szEOFMessage[] = "unexpected EOF\n";

DWORD g_rgdwBPPToPalFlags[9] = {
	0, DDPCAPS_1BIT, DDPCAPS_2BIT, 0, DDPCAPS_4BIT,
	0, 0, 0, DDPCAPS_8BIT};
DWORD g_rgdwBPPToPixFlags[9] = {
	0, DDPF_PALETTEINDEXED1, DDPF_PALETTEINDEXED2, 0, 
	DDPF_PALETTEINDEXED4, 0, 0, 0, DDPF_PALETTEINDEXED8};

DWORD
PaletteToPixelFlags(DWORD dwFlags)
{
	if (dwFlags & DDPCAPS_8BIT) return DDPF_PALETTEINDEXED8;
	if (dwFlags & DDPCAPS_4BIT) return DDPF_PALETTEINDEXED4;
	if (dwFlags & DDPCAPS_2BIT) return DDPF_PALETTEINDEXED2;
	if (dwFlags & DDPCAPS_1BIT) return DDPF_PALETTEINDEXED1;
	return 0;
}

DWORD
PixelToPaletteFlags(DWORD dwFlags)
{
	if (dwFlags & DDPF_PALETTEINDEXED8) return DDPCAPS_8BIT;
	if (dwFlags & DDPF_PALETTEINDEXED4) return DDPCAPS_4BIT;
	if (dwFlags & DDPF_PALETTEINDEXED2) return DDPCAPS_2BIT;
	if (dwFlags & DDPF_PALETTEINDEXED1) return DDPCAPS_1BIT;
	return 0;
}

BYTE
PixelFlagsToBPP(DWORD dwFlags)
{
	if (dwFlags & DDPF_PALETTEINDEXED8) return (BYTE) 8;
	if (dwFlags & DDPF_PALETTEINDEXED4) return (BYTE) 4;
	if (dwFlags & DDPF_PALETTEINDEXED2) return (BYTE) 2;
	if (dwFlags & DDPF_PALETTEINDEXED1) return (BYTE) 1;
	return (BYTE) 0;
}

BYTE
PaletteFlagsToBPP(DWORD dwFlags)
{
	if (dwFlags & DDPCAPS_8BIT) return (BYTE) 8;
	if (dwFlags & DDPCAPS_4BIT) return (BYTE) 4;
	if (dwFlags & DDPCAPS_2BIT) return (BYTE) 2;
	if (dwFlags & DDPCAPS_1BIT) return (BYTE) 1;
	return (BYTE) 0;
}

HRESULT
CreatePlainSurface(LPDIRECTDRAW pDD, DWORD nWidth, DWORD nHeight, DWORD nBPP,
				   const DDPIXELFORMAT *pddpf, LPDIRECTDRAWPALETTE pPalette, 
				   LPDIRECTDRAWSURFACE *ppdds, VARIANT_BOOL bForTexture)
{
	MMASSERT(pDD && ppdds);

	HRESULT hr;
	DDSURFACEDESC ddsd;
	INIT_DXSTRUCT(ddsd);
    ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS | DDSD_PIXELFORMAT;
	ddsd.dwWidth = nWidth;
	ddsd.dwHeight = nHeight;
	if (bForTexture)
	    ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY;
	else
	    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;

	if (pddpf) {
		// nBPP is ignored if a PixelFormat exists
		ddsd.ddpfPixelFormat = *pddpf;
	} else {
		if (nBPP <= 8) {
			// palettized
			if (pPalette == NULL)
				return E_INVALIDARG;
			memset(&(ddsd.ddpfPixelFormat),0,sizeof(DDPIXELFORMAT));
			ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
			ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB | BPPToPixelFlags(nBPP);
			ddsd.ddpfPixelFormat.dwRGBBitCount = nBPP;
		} else {
			return E_INVALIDARG;
		}
	}

	LPDIRECTDRAWSURFACE pdds;
	if (FAILED(hr = pDD->CreateSurface(&ddsd, &pdds, NULL)))
	{
		LPDIRECTDRAW2 lpDD2;
		DDSCAPS       ddsCaps;
		DWORD         dwTotal;
		DWORD         dwFree;
		pDD->QueryInterface(IID_IDirectDraw2, (void**)&lpDD2);
		ddsCaps.dwCaps = DDSCAPS_TEXTURE;
		lpDD2->GetAvailableVidMem(&ddsCaps, &dwTotal, &dwFree);
		TRACE("CreateSurface(%d * %d, bit depth %d) FAILED. Total video memory = %d, Free video memory = %d.\n", nWidth, nHeight, ddsd.ddpfPixelFormat.dwRGBBitCount, dwTotal, dwFree);
		lpDD2->Release();
		return hr;
	}

	// attach palette if it exists
	if (pPalette)
		if (FAILED(hr = pdds->SetPalette(pPalette))) {
			pdds->Release();
			return hr;
		}

	*ppdds = pdds;

	return hr;
}



DWORD DDColorMatch(IDirectDrawSurface *pdds, COLORREF rgb)
{
    COLORREF rgbT;
    HDC hdc;
    DWORD dw = CLR_INVALID;
    DDSURFACEDESC ddsd;
    HRESULT hres;

    //
    //  use GDI SetPixel to color match for us
    //
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
	rgbT = GetPixel(hdc, 0, 0);             // save current pixel value
	SetPixel(hdc, 0, 0, rgb);               // set our value
	pdds->ReleaseDC(hdc);
    }

    //
    // now lock the surface so we can read back the converted color
    //
    ddsd.dwSize = sizeof(ddsd);
    while ((hres = pdds->Lock(NULL, &ddsd, 0, NULL)) == DDERR_WASSTILLDRAWING)
	;

    if (hres == DD_OK)
    {
	dw  = *(DWORD *)ddsd.lpSurface;                     // get DWORD
        if(ddsd.ddpfPixelFormat.dwRGBBitCount < 32)
            dw &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount)-1;  // mask it to bpp
	pdds->Unlock(NULL);
    }

    //
    //  now put the color that was there back.
    //
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
	SetPixel(hdc, 0, 0, rgbT);
	pdds->ReleaseDC(hdc);
    }

    return dw;
}

HRESULT DDSetColorKey(IDirectDrawSurface *pdds, COLORREF rgb)
{
    DDCOLORKEY          ddck;

    ddck.dwColorSpaceLowValue  = DDColorMatch(pdds, rgb);
    ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
	ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
    return pdds->SetColorKey(DDCKEY_SRCBLT, &ddck);
}

HRESULT
CreatePlainDefaultSurface(LPDIRECTDRAW pDD, DWORD nWidth, DWORD nHeight, COLORREF clrTransp, 
						  LPDIRECTDRAWPALETTE pPalette,
						  DDSURFACEDESC *pddsd,
						  LPDIRECTDRAWSURFACE *ppdds)
{
	MMASSERT(ppdds);

	HRESULT hr;
	DDSURFACEDESC ddsd;
	INIT_DXSTRUCT(ddsd);
    ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
	ddsd.dwWidth = nWidth;
	ddsd.dwHeight = nHeight;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	

	LPDIRECTDRAWSURFACE pdds;
	if (FAILED(hr = pDD->CreateSurface(&ddsd, &pdds, NULL)))
		goto ERROR_ENCOUNTERED;

	/*
	ddPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
	hr = pdds->GetPixelFormat( &ddPixelFormat);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	
	*/

	INIT_DXSTRUCT(*pddsd);
	hr = pdds->Lock(NULL, pddsd, DDLOCK_WAIT, NULL);
	if ( FAILED(hr))
	{
		ASSERT(FALSE);
		goto ERROR_ENCOUNTERED;
	}

	pdds->Unlock(pddsd->lpSurface);

	if (( pddsd->ddpfPixelFormat.dwRGBBitCount <= 8 ) &&
		( pddsd->ddpfPixelFormat.dwFlags & (  DDPF_RGB | BPPToPixelFlags(pddsd->ddpfPixelFormat.dwRGBBitCount))) &&
		( pPalette ))
	{
		hr = pdds->SetPalette(pPalette);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}


	DDSetColorKey(pdds, clrTransp);

	*ppdds = pdds;
	//SAFEADDREF( *pdds );
	if ( pdds ) pdds->AddRef();

ERROR_ENCOUNTERED:

	MMRELEASE( pdds );

	return hr;
}


HRESULT
ClearToColor(DWORD dwColor, LPRECT prDst, LPDIRECTDRAWSURFACE pdds)
{
	MMASSERT(pdds);
	
	DDBLTFX ddbfx;
	INIT_DXSTRUCT(ddbfx);
	ddbfx.dwFillColor = dwColor;
	
	RECT rDst;
	if (prDst == NULL) {
		::GetSurfaceDimensions(pdds, &rDst);
		prDst = &rDst;
	}
	
	return pdds->Blt(prDst, NULL, NULL, DDBLT_COLORFILL | DDBLT_ASYNC, &ddbfx);
}

HRESULT
GetSurfaceDimensions(LPDIRECTDRAWSURFACE pdds, LPRECT prDim)
{
	MMASSERT(pdds && prDim);

	HRESULT hr;
	DDSURFACEDESC ddsd;
	INIT_DXSTRUCT(ddsd);
	if (FAILED(hr = pdds->GetSurfaceDesc(&ddsd))) {
		MMASSERT(SUCCEEDED(hr));
		return hr;
	}
	prDim->left = 0;
	prDim->top = 0;
	prDim->right = ddsd.dwWidth;
	prDim->bottom = ddsd.dwHeight;

	return S_OK;
}

HRESULT
CopyPixelsToDDS(const BYTE *pSrcPixels, RECT rSrc, long nSrcPitch, 
				 LPDIRECTDRAWSURFACE pddsDst, DWORD nXPos, DWORD nYPos)
{
	MMASSERT(pddsDst && pSrcPixels);
	HRESULT hr, hr2;

	DDSURFACEDESC ddsd;
	INIT_DXSTRUCT(ddsd);

	DWORD nSrcWidth = rSrc.right - rSrc.left;
	DWORD nSrcHeight = rSrc.bottom - rSrc.top;
	LPBYTE pDstPixels = NULL;

	RECT rDst = {nXPos, nYPos, nXPos + nSrcWidth, nYPos + nSrcHeight};

	// lock the surface for writing
	if (FAILED(hr = pddsDst->Lock(&rDst, &ddsd, DDLOCK_WAIT /*| DDLOCK_WRITEONLY */, NULL)))
	{
		ASSERT(FALSE);
		return hr;
	}
//	if (FAILED(hr = pddsDst->Lock(NULL, &ddsd, DDLOCK_WAIT /*| DDLOCK_WRITEONLY */, NULL)))
//		return hr;

	// check that the surface is the right size for the copy
	if (((ddsd.dwWidth - nXPos) < nSrcWidth) || ((ddsd.dwHeight - nYPos) < nSrcHeight))
	{
		hr = E_INVALIDARG;
		goto e_CopyPixelsToDDS;
	}

	//
	// copy the pixels
	//
	pDstPixels = (LPBYTE) ddsd.lpSurface;
	
	// position the source pixel pointer
	pSrcPixels += rSrc.top * nSrcPitch + rSrc.left;

	// REVIEW: (mdm) the dst position should already have been taken care of by the Lock
//	pDstPixels += nYPos * ddsd.lPitch + nXPos;

	hr = BltFast(pSrcPixels, nSrcPitch, pDstPixels, ddsd.lPitch, 
			nSrcWidth, nSrcHeight);
	
e_CopyPixelsToDDS:

	hr2 = pddsDst->Unlock(ddsd.lpSurface);

	return hr;
}

HRESULT
CreateSurfaceWithText(LPDIRECTDRAW pDD, CColorManager *pColMgr, 
					  const char *szText, 
					  HFONT hFont, COLORREF crFontColor, BOOL bShadowed, COLORREF crShadowColor, const SIZE* fittedSize, 
					  SIZE *psiz, LPCOLORINFO *ppciText, LPDIRECTDRAWSURFACE *ppdds)
{
	LPDIRECTDRAWPALETTE	pddp = NULL;
	HDC					hDC = NULL, hdcMem = NULL, hdcDDS = NULL;
	HGDIOBJ				hOldFont = NULL, hOldFontMem = NULL, hOldDIB = NULL;
	HBRUSH				hbr = NULL;
	HBITMAP				hDIB = NULL;
	LPBYTE				pDIBPixels = NULL;
	DDPIXELFORMAT		ddpf;
	PALETTEENTRY		rgpe[256];
	SIZE				sizText;
	RECT				rText, rDrawText;
	int					cTextLength = strlen(szText), px, py, nXOffset = 0, nYOffset = 0, i = 0, j = 0;
    DDCOLORKEY          ddck;
	DWORD				dwTransColorBGR, dwTransColorDD;
	UINT				fuOptions;
	BYTE				nBPPDefaultCI = 2;
	HRESULT				hr;

	// Check arguments.
	MMASSERT(ppdds && psiz);
	if ((szText == NULL) || (szText[0] == '\0') || (hFont == NULL) || (pDD == NULL))
	{
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	// Compute the size of the surface required for the text.
	if (((hDC = GetDC(NULL)) == NULL) ||
		// select the font into the DC
		((hOldFont = SelectObject(hDC, hFont)) == NULL) ||
		// compute the size of the fontified string in pixels
		(GetTextExtentPoint32(hDC, szText, cTextLength, &sizText) == 0))
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if ( fittedSize != NULL )
	{
		sizText.cx = fittedSize->cx;
		sizText.cy = fittedSize->cy;
	}	
	px = (sizText.cx +3)&~3;
	py = (sizText.cy +3)&~3;

	// Get a simple palette for the text surface.
	if (FAILED(hr = pColMgr->GetDefaultColorInfo(nBPPDefaultCI, ppciText)))
		goto ERROR_ENCOUNTERED;

	// Get the palette entries for the simple palette.
	if (FAILED(hr = (*ppciText)->pddp->GetEntries(0, 0, 256, rgpe)))
		goto ERROR_ENCOUNTERED;

	// Get the RGB value for *ppciText->dwTransColor (GetDefaultColorInfo sets 
	// *ppciText->dwTransColor as an index into the simple palette).
	dwTransColorBGR = RGB(rgpe[(*ppciText)->dwTransColor].peRed, 
						  rgpe[(*ppciText)->dwTransColor].peGreen, 
					      rgpe[(*ppciText)->dwTransColor].peBlue);

	// Use our simple palette if possible, otherwise use the video mode format.
	while (i < (1 << nBPPDefaultCI) &&
		   (crFontColor != RGB(rgpe[i].peRed, rgpe[i].peGreen, rgpe[i].peBlue))) i++;
	if (bShadowed)
	{
		while (j < (1 << nBPPDefaultCI) &&
			   (crShadowColor != RGB(rgpe[j].peRed, rgpe[j].peGreen, rgpe[j].peBlue))) j++;
	}
	if (i == (1 << nBPPDefaultCI) || j == (1 << nBPPDefaultCI))
	{
		pddp = pColMgr->GetVideoModePalette();
		if (pddp && FAILED(hr = pddp->GetEntries(0, 0, 256, rgpe)))
			goto ERROR_ENCOUNTERED;
		pColMgr->GetVideoModePixelInfo().GetDDPF(ddpf);
		dwTransColorDD = (*ppciText)->pmVideoMode.GetIndexMapping((*ppciText)->dwTransColor);
		(*ppciText)->Release();
		if (pddp)
		{
			if (FAILED(hr = pColMgr->GetColorInfo(pddp, 
												  dwTransColorDD,
												  flagTRANSPARENT | flagPALETTIZED, ppciText)))
				goto ERROR_ENCOUNTERED;
		}
		else
		{
			if (FAILED(hr = pColMgr->GetColorInfo(pColMgr->GetVideoModePixelInfo(), 
												  dwTransColorDD,
												  flagTRANSPARENT, ppciText)))
				goto ERROR_ENCOUNTERED;
		}
	}
	else
	{
		pddp = (*ppciText)->pddp;
		if (pddp)
			pddp->AddRef();
		(*ppciText)->pixi.GetDDPF(ddpf);
		dwTransColorDD = (*ppciText)->dwTransColor;
	}

	// Create the surface.
    ddck.dwColorSpaceLowValue  = dwTransColorDD;
    ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
	if ((SetRect(&rText, 0, 0, px, py) == 0) ||
		// create the DDS based on the extent
		FAILED(hr = CreatePlainSurface(pDD, rText.right, rText.bottom, 0, &ddpf, pddp, ppdds)) ||
		// clear the surface to the transparency color - need DD format RGB
		FAILED(hr = ClearToColor(dwTransColorDD, &rText, *ppdds)) ||
		// set the transparency color on the DDS
		FAILED(hr = (*ppdds)->SetColorKey(DDCKEY_SRCBLT, &ddck)))
	{
		hr = FAILED(hr) ? hr : E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	// If we have a fitted size assume that the offset should be 0.
	if (fittedSize == NULL)
	{
		// REVIEW this always centers the text in the allocated image,
		// that may not be what we want, especially as the image gets bigger than
		// the text
		nXOffset = (rText.right - sizText.cx) >> 1;
		nYOffset = (rText.bottom - sizText.cy) >> 1;
	}

	// Calculate draw rect for text.
	rDrawText.left = nXOffset;
	rDrawText.right = rDrawText.left + sizText.cx;
	rDrawText.top = nYOffset;
	rDrawText.bottom = rDrawText.top + sizText.cy;

	// Update the size
	sizText.cx = rText.right;
	sizText.cy = rText.bottom;

	// Output the font to the DDS
	// There is a known problem with GetDC on off screen surfaces which are not the same
	// pixel format as the video format, This actually leaves a lock on the surface in NT.
	// Fortunately we're now always using the video format.
	if (FAILED(hr = (*ppdds)->GetDC(&hdcDDS)))
		goto ERROR_ENCOUNTERED;

	// Select in the font.
	SetBkMode(hdcDDS, TRANSPARENT);
	if ((hOldFont = SelectObject(hdcDDS, hFont)) == NULL)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	// Background text.
	fuOptions = DT_EDITCONTROL | DT_WORDBREAK;
	if (bShadowed)
	{
		RECT rect = {rDrawText.left + 2, rDrawText.top + 2, rDrawText.right + 2,rDrawText.bottom + 2}; 
		// Set the color of the shadow text
		if ((SetTextColor(hdcDDS, crShadowColor) == CLR_INVALID) ||
			// output the shadow text
			(DrawText( hdcDDS, szText,cTextLength, &rect,fuOptions) == 0))
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
	}

	// Foreground text
	if ((SetTextColor(hdcDDS, crFontColor) == CLR_INVALID) || // specified font color
		// output the foreground text to the surface
		(DrawText( hdcDDS, szText,cTextLength, &rDrawText,fuOptions) == 0))
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	*psiz = sizText;

ERROR_ENCOUNTERED:

	MMRELEASE(pddp);
	if (hdcDDS)
	{
		(*ppdds)->ReleaseDC(hdcDDS);
		if (hOldFont)
			SelectObject(hdcDDS, hOldFont);
	}
	if (hDC)
	{
		if (hOldFont)
			SelectObject(hDC, hOldFont);
		ReleaseDC(NULL, hDC);
	}
	if (hdcMem)
	{
		if (hOldDIB)
			SelectObject(hdcMem, hOldDIB);
		if (hOldFontMem)
			SelectObject(hdcMem, hOldFontMem);
		ReleaseDC(NULL, hdcMem);
		DeleteDC( hdcMem );
	}
	if (hbr)
		DeleteObject(hbr);
	if (hDIB)
		DeleteObject(hDIB);

	return hr;
}


HRESULT
CreatePlainDIBSection(HDC hDC, DWORD nWidth, DWORD nHeight, DWORD nBPP, 
					  const PALETTEENTRY *rgpePalette, HBITMAP *phbm, LPBYTE *ppPixels)
{
	MMASSERT(rgpePalette && ppPixels && phbm);
	HRESULT hr = S_OK;
	DWORD i, cPalEntries = nBPP <= 8 ? (1 << nBPP) : 0;
	HBITMAP hbm = NULL;

	// allocate bitmap info structure
	BITMAPINFO *pbmi = NULL;
	pbmi = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFOHEADER) + cPalEntries * sizeof(RGBQUAD)];
	if (pbmi == NULL)
		return E_OUTOFMEMORY;

	// specify bitmip info
	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biPlanes = 1;
	pbmi->bmiHeader.biSizeImage = 0;
	pbmi->bmiHeader.biClrUsed = 0;
	pbmi->bmiHeader.biClrImportant = 0;
	pbmi->bmiHeader.biBitCount = (WORD) nBPP;
	pbmi->bmiHeader.biCompression = BI_RGB;
	pbmi->bmiHeader.biWidth = (LONG) nWidth;
	pbmi->bmiHeader.biHeight = -(LONG) nHeight;

	// copy palette into bmi (if palettized)
	for(i = 0; i < cPalEntries; i++) {
		pbmi->bmiColors[i].rgbRed = rgpePalette[i].peRed;
		pbmi->bmiColors[i].rgbGreen= rgpePalette[i].peGreen;
		pbmi->bmiColors[i].rgbBlue = rgpePalette[i].peBlue;
		pbmi->bmiColors[i].rgbReserved = 0;
	}

	// create bitmap
	LPVOID pvBits = NULL;
	hbm = ::CreateDIBSection(hDC, pbmi, DIB_RGB_COLORS, &pvBits, NULL, 0);
	if (hbm == NULL) {
		hr = E_FAIL;
		goto e_CreatePlainDIBSection;
	}

	*phbm = hbm;
	*ppPixels = (LPBYTE) pvBits;

e_CreatePlainDIBSection:
	MMDELETE(pbmi);

	return hr;
}

// blue is assumed to have a weight of 1.f
#define fSimpleRedWeight 2.1f
#define fSimpleGreenWeight 2.4f
#define fMaxColorDistance ((1.f + fSimpleRedWeight + fSimpleGreenWeight) * float(257 * 256))


static inline float
_ColorDistance(const PALETTEENTRY &pe, BYTE r, BYTE g, BYTE b)
{
	float fTotal, fTmpR, fTmpG, fTmpB;
	fTmpR = (float) (pe.peRed - r);
	fTotal = fSimpleRedWeight * fTmpR * fTmpR;
	fTmpG = (float) (pe.peGreen - g);
	fTotal += fSimpleGreenWeight * fTmpG * fTmpG;
	fTmpB = (float) (pe.peBlue - b);
	// blue is assumed to have a weight of 1.f
	fTotal += fTmpB * fTmpB;

	return fTotal;
}

DWORD
SimpleFindClosestIndex(const PALETTEENTRY *rgpePalette, DWORD cEntries, BYTE r, BYTE g, BYTE b)
{
	MMASSERT(rgpePalette);
	MMASSERT(cEntries <= nMAXPALETTEENTRIES);

	float fTmp, fMinDistance = fMaxColorDistance;
	DWORD nMinIndex = cEntries;

	for (DWORD i = 0; i < cEntries; i++) {
		const PALETTEENTRY &peTmp = rgpePalette[i];
		if (!(peTmp.peFlags & (PC_RESERVED | PC_EXPLICIT))) {
			if ((fTmp = _ColorDistance(peTmp, r, g, b)) < fMinDistance) {
				// check for exact match
				if (fTmp == 0.f)
					return i;
				nMinIndex = i;
				fMinDistance = fTmp;
			}
		}
	}
	MMASSERT(nMinIndex < cEntries);
	return nMinIndex;
}

/*
void
CopyRGBTRIPLEtoRGB(const RGBTRIPLE *prgbtSrc, LPRGB prgbDst, DWORD cEntries)
{
	LPRGB prgbLimit = prgbDst + cEntries;
	for (; prgbDst < prgbLimit; prgbDst++, prgbtSrc++) {
		prgbDst->r = prgbtSrc->rgbtRed;
		prgbDst->g = prgbtSrc->rgbtGreen;
		prgbDst->b = prgbtSrc->rgbtBlue;
	}
}

void
CopyRGBQUADtoRGB(const RGBQUAD *prgbqSrc, LPRGB prgbDst, DWORD cEntries)
{
	MMASSERT(prgbqSrc && prgbDst);
	LPRGB prgbLimit = prgbDst + cEntries;
	for (; prgbDst < prgbLimit; prgbDst++, prgbqSrc++) {
		prgbDst->r = prgbqSrc->rgbRed;
		prgbDst->g = prgbqSrc->rgbGreen;
		prgbDst->b = prgbqSrc->rgbBlue;
	}
}

void
CopyRGBtoPALETTEENTRY(const RGB *prgb, LPPALETTEENTRY ppe, DWORD cEntries)
{
	MMASSERT(prgb && ppe);
	LPPALETTEENTRY ppeLimit = ppe + cEntries;
	for (; ppe != ppeLimit; ppe++, prgb++) {
		ppe->peRed = prgb->r;
		ppe->peGreen = prgb->g;
		ppe->peBlue = prgb->b;
		ppe->peFlags = 0;
	}
}
*/

void
ZeroDWORDAligned(LPDWORD pdw, DWORD cEntries)
{
	// verify alignment
	MMASSERT((((DWORD) pdw) & 0x3) == 0);
	LPDWORD pdwLimit = pdw + cEntries;
	// REVIEW: use DM machine
	for (; pdw != pdwLimit; *pdw++ = 0);
}


BOOL
ClipRect(const RECT &rTarget, LPRECT prSrc)
{
	MMASSERT((rTarget.left <= rTarget.right) && (rTarget.top <= rTarget.bottom) &&
		(prSrc->left <= prSrc->right) && (prSrc->top <= prSrc->bottom));

	//REVIEW... Is this backwards?
	//The clip rect target is the actual devices
	//prSrc is desired clip
	/*
	UPDATEMIN(prSrc->left, rTarget.left);
	UPDATEMIN(prSrc->top, rTarget.top);
	UPDATEMAX(prSrc->right, rTarget.right);
	UPDATEMAX(prSrc->bottom, rTarget.bottom);
	*/
	UPDATEMAX(prSrc->left, rTarget.left);
	UPDATEMAX(prSrc->top, rTarget.top);
	UPDATEMIN(prSrc->right, rTarget.right);
	UPDATEMIN(prSrc->bottom, rTarget.bottom);

	// make sure we still have a valid rectangle
	//RAC bug, shouldn't this be an update max?
	UPDATEMAX(prSrc->right, prSrc->left);
	UPDATEMAX(prSrc->bottom, prSrc->top);

	return ((prSrc->left != prSrc->right) && (prSrc->top != prSrc->bottom));
}


DWORD
GetClosestMultipleOf4(DWORD n, BOOL bGreater)
{
	MMASSERT(((bGreater == 1) || (bGreater == 0)) && (sizeof(DWORD) == 4));
	return (n + bGreater * 3) & ~3;
}


DWORD
GetClosestPowerOf2(DWORD n, BOOL bGreater)
{
	MMASSERT(((bGreater == 1) || (bGreater == 0)) && (sizeof(DWORD) == 4));

	DWORD i = 0;
	for (n >>= 1; n != 0; i++) {
		n >>= 1;
	}
	i += (bGreater && ((n & ~(1 << i)) != 0));

	return (1 << i);
}


HRESULT
AllocatePixels(DWORD cPixels, BYTE nBPP, LPBYTE &pPixels)
{
	if (pPixels)
		return E_INVALIDARG;

	// allocate pixels on a DWORD aligned boundary
	pPixels = (LPBYTE) new DWORD[(cPixels * nBPP + 31) >> 5];
	if (pPixels == NULL)
		return E_OUTOFMEMORY;

	return S_OK;
}
