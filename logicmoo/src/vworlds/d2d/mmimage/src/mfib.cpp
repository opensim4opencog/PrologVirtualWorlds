// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// File:	MFIB.cpp
// Author:	MM    (MM)
//
// Notes:
// - accumulate data for image from various source types
// - Once a DDS is created for a frame, it can not be replaced or removed.
//   This is because the frame might have been created from a surface that
//   the system is using elsewhere.
// - Images can only have one global color model -- if frames use a different
//   color model, they are mapped at load time to the global color model.  If
//   a global color model is not defined explicitly, the color model on the
//   first frame is used as the global color model.  Believe me, this is the
//   only reasonable thing to do - if you try to do anything else, you will 
//   regret it.
// - Images are constructed such that the pixel and palette formats are common
//   to all frames.  The only way for different frames to have different color
//   models is for them to have different palettes.  The content can have a
//   color model which is different from the final color model.

#include "MMImage.h"

static char gs_szMFIBPrefix[] = "image builder error";

static HasPixelFn gs_rgHasPixelFn[5] = {
	NULL, HasPixel8, HasPixel16, HasPixel24, HasPixel32
};

//
// CGlobalBuilderInfo
//
void
CGlobalBuilderInfo::Reset()
{
	if (pPalette && (dwFlags & flagPalDealloc))
		delete pPalette;
	MMRELEASE(pciFinal);

	memset(this, 0, sizeof(*this));
}


//
// CFrameBuilderInfo
//
void
CFrameBuilderInfo::Reset()
{
	if (pPalette && (dwFlags & flagPalDealloc)) {
		MMDELETE(pPalette);
	}
	if (bDDSLocked) {
		MMASSERT(pdds);
		pdds->Unlock(pSurfacePixels);
	}
	if (pPixels && (dwFlags & flagPixDealloc)) {
		MMDELETE(pPixels);
	}
	MMRELEASE(pdds);
	MMRELEASE(prles);

	memset(this, 0, sizeof(*this));
}


//
// CMFImageBuilderInfo
//
CMFImageBuilder::CMFImageBuilder()
{
	m_pDD = NULL;
	m_pColMgr = NULL;
	m_szName = NULL;
	m_pfbiCur = NULL;
	m_conm = conmNone;
}

CMFImageBuilder::~CMFImageBuilder()
{
	Reset(NULL);

	MMRELEASE(m_pDD);
	MMRELEASE(m_pColMgr);
}


HRESULT
CMFImageBuilder::Init(LPDIRECTDRAW pDD, CColorManager *pColMgr)
{
	if ((pDD == NULL) || (pColMgr == NULL)) 
		return E_INVALIDOBJECT;

	if (m_pDD || m_pColMgr)
		return E_ALREADYINITIALIZED;

	pDD->AddRef();
	m_pDD = pDD;

	pColMgr->AddRef();
	m_pColMgr = pColMgr;

	return S_OK;
}


// Function: Setup
//    Setup the compile preferences for the builder.
HRESULT
CMFImageBuilder::Setup(DWORD dwUsageFlags)
{
	m_dwUsageFlags = dwUsageFlags;

	// REVIEW: RLE needs more work before it can be added permanently...
	if (m_dwUsageFlags & flagUsageRLE) {
		MMTRACE("%s: turning off RLE\n", gs_szMFIBPrefix);
		m_dwUsageFlags ^= flagUsageRLE;
	}

	return S_OK;
}


HRESULT
CMFImageBuilder::Reset(const char *szName)
{
	m_gbi.Reset();

	MMDELETE(m_pfbiCur);

	// delete all of the frame information
	DWORD cItems = m_dspfbiFrames.Items();
	for (DWORD i = 0; i < cItems; i++) {
		delete m_dspfbiFrames[i];
	}
	m_dspfbiFrames.Reset();

	// store the new image name
//	MMDELETE(m_szName);
//  allocated w/ strdup... use free
	if ( m_szName ) free(m_szName );
	m_szName = NULL;

	if (szName) {
		// duplicate the string
		if ((m_szName = _strdup(szName)) == NULL)
			return E_OUTOFMEMORY;
	}

	m_conm = conmNone;

	return S_OK;
}


HRESULT
CMFImageBuilder::StartImage(DWORD nWidth, DWORD nHeight, 
							const CPixelInfo &pixiPalFmt, const CPixelInfo &pixiPixFmt)
{
	MMASSERT((pixiPalFmt.nBPP <= 32) && (pixiPixFmt.nBPP <= 32));
	HRESULT hr;

	if (m_pDD == NULL)
		return E_NOTINITIALIZED;
	if ((m_conm != conmNone) || (m_dspfbiFrames.Items() > 0))
		return E_ALREADYINITIALIZED;

	// verify image dimensions
	if ((nWidth > nMAXIMAGEDIM) || (nHeight > nMAXIMAGEDIM))
		return E_INVALIDARG;

	// verify bit depths
	if (pixiPixFmt.nBPP < 8) {
		MMTRACE("%s: pixel formats less than 8 bits not implemented\n", gs_szMFIBPrefix);
		return E_NOTIMPL;
	}

	m_conm = conmInit;

	m_gbi.nWidth = (WORD) nWidth;
	m_gbi.nHeight = (WORD) nHeight;
	m_gbi.pixiPalFmt = pixiPalFmt;
	m_gbi.pixiPixFmt = pixiPixFmt;
	m_gbi.cBytesPerPixel = (m_gbi.pixiPixFmt.nBPP + 7) >> 3;

	// determine final pixel format based on usage flags and content pixel format
	if (FAILED(hr = ComputeFinalPixelInfo()))
		return hr;

	return S_OK;
}


// Notes on Method 1:
// - Pixel and palette space can be allocated/deallocated by the builder.
//   This means that the code calling the builder can either allocate P&|P
//   and have the builder deallocate, or it can request space for P&|P from
//   the builder and write the data in.  If space is allocated by the
//   builder, it will be deallocated by the builder.
// - The palette format is defined by the pixiPalFmt.
// - The pixel format is defined by the pixiPixFmt.


HRESULT
CMFImageBuilder::StartGlobal(BYTE nBPPPalette, LPBYTE &pPalette,
							 DWORD dwTransColor, DWORD dwBackgroundColor, DWORD dwFlags)
{
//	MMTRACE("CMFImageBuilder::StartGlobal\n");
	HRESULT hr;

	if ((m_pDD == NULL) || (m_conm != conmInit))
		return E_NOTINITIALIZED;

	m_conm = conmInteractive;

	// verify color model
	BYTE nBPPColor;
	if (pPalette || (dwFlags & flagPalAlloc)) {
		nBPPColor = nBPPPalette;
		if ((m_gbi.pixiPixFmt.nBPP != 8) || (nBPPPalette > 8) || (nBPPPalette == 0))
			return E_INVALIDARG;
	} else {
		nBPPColor = m_gbi.pixiPixFmt.nBPP;
		if (m_gbi.pixiPixFmt.nBPP <= 8)
			return E_INVALIDARG;
	}

	// verify transparenecy and background colors
	if (!(IsValidColor(dwTransColor, nBPPColor) && IsValidColor(dwBackgroundColor, nBPPColor))) {
		MMTRACE("warning: invalid background or transparency color -- ignoring\n", gs_szMFIBPrefix);
	}
	m_gbi.dwFlags = dwFlags;
	m_gbi.dwTransColor = dwTransColor;
	m_gbi.dwBackgroundColor = dwBackgroundColor;
	m_gbi.nBPPPalette = nBPPPalette;

	// allocate the palette, if necessary
	MMASSERT(m_gbi.pPalette == NULL);
	if ((dwFlags & flagPalAlloc) && 
		FAILED(hr = AllocatePixels(1 << nBPPPalette, m_gbi.pixiPalFmt.nBPP, pPalette)))
		return hr;
	if (m_gbi.pPalette = pPalette)
		m_gbi.dwFlags |= flagPALETTIZED;

	m_gbi.bGlobalColorModel = ((m_gbi.pixiPixFmt.nBPP > 8) || (pPalette != NULL));

	return S_OK;
}


HRESULT
CMFImageBuilder::StartFrame(const RECT &rDstRect, DWORD nFrameDelay, BYTE nBPPPalette, 
							LPBYTE &pPalette, LPBYTE &pPixels, DWORD dwTransColor, DWORD dwFlags)
{
//	MMTRACE("CMFImageBuilder::StartFrame\n");
	HRESULT hr;

	if (m_conm != conmInteractive)
		return E_NOTINITIALIZED;

	long nWidth = rDstRect.right - rDstRect.left;
	long nHeight = rDstRect.bottom - rDstRect.top;

	// verify frame dimensions
	if ((nWidth <= 0) || (nHeight <= 0) || 
		(((DWORD) rDstRect.right) > ((DWORD) m_gbi.nWidth)) || 
		(((DWORD) rDstRect.bottom) > ((DWORD) m_gbi.nHeight)))
	{
		MMTRACE("%s: invalid frame dimensions\n", gs_szMFIBPrefix);
		return E_INVALIDARG;
	}

	// verify color model
	if (pPalette || (dwFlags & flagPalAlloc)) {
		if ((m_gbi.pixiPixFmt.nBPP != 8) || (nBPPPalette > 8) || (nBPPPalette == 0) ||
			!IsValidColor(dwTransColor, nBPPPalette))
			return E_INVALIDARG;
	}

	// allocate a new frame
	MMASSERT(m_pfbiCur == NULL);
	m_pfbiCur = new CFrameBuilderInfo;
	if (m_pfbiCur == NULL) 
		return E_OUTOFMEMORY;

	// allocate the palette, if necessary
	if ((dwFlags & flagPalAlloc) &&
		FAILED(hr = AllocatePixels(1 << nBPPPalette, m_gbi.pixiPalFmt.nBPP, pPalette)))
		return hr;

	// if the global color model has not been defined, retroactively define it
	// from the color model of the first frame
	if (m_gbi.bGlobalColorModel == FALSE) {
		// define as the global color model
		MMASSERT((m_gbi.pPalette == NULL) && pPalette || (dwFlags & flagPalAlloc));
		// N.B: transparency is dealt with below
		m_gbi.pPalette = pPalette;
		m_gbi.dwFlags |= (flagPalAlloc | flagPALETTIZED);
		m_gbi.nBPPPalette = nBPPPalette;
		m_gbi.bGlobalColorModel = TRUE;
		// don't mark the local frame as having allocated palette
		dwFlags ^= flagPalAlloc;
	} else {
		// define as a local color model
		if (m_pfbiCur->pPalette = pPalette)
			m_pfbiCur->dwFlags |= flagPALETTIZED;
		m_pfbiCur->nBPPPalette = nBPPPalette;
	}

	// verify transparency when using the global color model
	if ((dwFlags & flagTRANSPARENT) && (m_pfbiCur->pPalette == NULL)) {
		if (m_gbi.dwFlags & flagTRANSPARENT) {
			// REVIEW: we can probably remap on a mismatch, but for now...
			// verify the transparency color keys match
			if (dwTransColor != m_gbi.dwTransColor) {
				MMTRACE("%s: multiple transparency color keys defined on global color model\n", gs_szMFIBPrefix);
				return E_INVALIDARG;
			}
		} else {
			// define a global transparency color key since one doesn't exist yet
			// NB: it is necessary to do this here because GIF doesn't let us
			//     know about transparency until we try to load the first frame
			m_gbi.dwFlags |= flagTRANSPARENT;
			m_gbi.dwTransColor = dwTransColor;
		}
	}

	m_pfbiCur->rDim = rDstRect;
	m_pfbiCur->nWidth = (WORD) nWidth;
	m_pfbiCur->nHeight = (WORD) nHeight;
	UPDATEMAX(nFrameDelay, 12);
	m_pfbiCur->nDelay = nFrameDelay;
	m_pfbiCur->dwFlags = dwFlags;
	m_pfbiCur->dwTransColor = dwTransColor;

	// allocate the pixels, if necessary
	if ((dwFlags & flagPixAlloc) && 
		FAILED(hr = AllocatePixels(nWidth * nHeight, m_gbi.pixiPixFmt.nBPP, pPixels)))
		return hr;

	m_pfbiCur->nPitch = nWidth * m_gbi.cBytesPerPixel;
	m_pfbiCur->pPixels = pPixels;

	return S_OK;
}


HRESULT
CMFImageBuilder::EndFrame()
{
	if ((m_pfbiCur == NULL) || (m_conm != conmInteractive))
		return E_NOTINITIALIZED;

	HRESULT hr;

	//
	// verify transparency, if necessary
	//
	if ((m_pfbiCur->dwFlags & flagTRANSPARENT) && 
		(m_dwUsageFlags & flagUsageVerifyTransparency)) 
	{
		BOOL b;
		MMASSERT(INRANGE(m_gbi.cBytesPerPixel, 1, 4));
		// check if the transparency pixel can be found in data
		hr = gs_rgHasPixelFn[m_gbi.cBytesPerPixel](m_pfbiCur->pPixels, m_pfbiCur->nPitch, 
				m_pfbiCur->dwTransColor, m_pfbiCur->nWidth, m_pfbiCur->nHeight, &b);
		
		if (SUCCEEDED(hr) && (b == FALSE)) {
			// unmark this frame as having transparency
			m_pfbiCur->dwFlags ^= flagTRANSPARENT;
		}
	}

	// compute the MFI target color model, if necessary
	if ((m_gbi.pciFinal == NULL) && FAILED(hr = ComputeFinalColorModel()))
		return hr;

	// create the fully expanded image using the target color model
	if (FAILED(hr = CreateFinalPixels(m_pfbiCur)))
		return hr;

	// store the frame info
	m_dspfbiFrames.Ins(m_pfbiCur);
	m_pfbiCur = NULL;

	return S_OK;
}


HRESULT
CMFImageBuilder::ComputeFinalColorModel()
{
	MMASSERT((m_gbi.pciFinal == NULL) && m_gbi.bGlobalColorModel);
	HRESULT hr;

	if (m_gbi.pixiFinalFmt.nBPP <= 8) {
		// palettized
		MMASSERT(m_gbi.bFinalFmtSame);
		m_gbi.dwFinalTransColor = m_gbi.dwTransColor;
		m_gbi.dwFinalBackgroundColor = m_gbi.dwBackgroundColor;
		// N.B.: the palette mapping from the global model to the final is the identity
		//   when the model is palettized
		if (FAILED(hr = m_pColMgr->GetColorInfo(m_gbi.pixiPalFmt, m_gbi.nBPPPalette,
							m_gbi.pPalette, m_gbi.pixiPixFmt.nBPP, 
							m_gbi.dwFinalTransColor, m_gbi.dwFlags, &(m_gbi.pciFinal))))
			return hr;
	} else {
		// RGB
		DWORD dwFlags = (m_gbi.dwFlags & flagTRANSPARENT);
		if (m_gbi.bFinalFmtSame) {
			m_gbi.dwFinalTransColor = m_gbi.dwTransColor;
			m_gbi.dwFinalBackgroundColor = m_gbi.dwBackgroundColor;
		} else {
			// if the global color model is palettized, create a color map
			if (m_gbi.pPalette) {
				MMASSERT(!m_gbi.bFinalFmtSame);
				PALETTEENTRY rgpe[nMAXPALETTEENTRIES];
				// copy the palette to a PALETTEENTRY table
				if (FAILED(hr = BltFastRGBToRGB(m_gbi.pPalette, 0, (LPBYTE) rgpe, 0, 
									1 << m_gbi.nBPPPalette, 1, m_gbi.pixiPalFmt, g_pixiPALETTEENTRY)))
					return hr;
				// create a palette map
				if (FAILED(hr = m_pmFinal.CreateMap(m_gbi.pixiPixFmt.nBPP, m_gbi.nBPPPalette, rgpe, 
									m_gbi.pixiFinalFmt, NULL)))
					return hr;
				m_gbi.dwFinalTransColor = m_pmFinal.GetIndexMapping(m_gbi.dwTransColor);
				m_gbi.dwFinalBackgroundColor = m_pmFinal.GetIndexMapping(m_gbi.dwBackgroundColor);
			} else {
				m_gbi.dwFinalTransColor = m_gbi.pixiFinalFmt.TranslatePack(m_gbi.dwTransColor, m_gbi.pixiPixFmt);
				m_gbi.dwFinalBackgroundColor = m_gbi.pixiFinalFmt.TranslatePack(m_gbi.dwBackgroundColor, m_gbi.pixiPixFmt);
			}				
		}
		if (FAILED(hr = m_pColMgr->GetColorInfo(m_gbi.pixiFinalFmt, m_gbi.dwFinalTransColor, 
							dwFlags, &(m_gbi.pciFinal))))
			return hr;
	}
	return S_OK;
}


HRESULT
CMFImageBuilder::ComputeFinalPixelInfo()
{
	MMASSERT(m_pColMgr);
	HRESULT hr;
	DWORD nFinalWidth = m_gbi.nWidth;
	DWORD nFinalHeight = m_gbi.nHeight;

	// for D3D textures, we need the image to have power of 2 dimensions
	if (m_dwUsageFlags & flagUsageD3DTextureEnabled) {
		BOOL bGreater = (m_dwUsageFlags & flagUsageResizeTrunc ? FALSE : TRUE);
		nFinalWidth = GetClosestPowerOf2(nFinalWidth, bGreater);
		nFinalHeight = GetClosestPowerOf2(nFinalHeight, bGreater);
	}

	if ((nFinalWidth > nMAXIMAGEDIM) || (nFinalHeight > nMAXIMAGEDIM))
		return E_INVALIDARG;

	m_gbi.nFinalWidth = (WORD) nFinalWidth;
	m_gbi.nFinalHeight = (WORD) nFinalHeight;

	// REVIEW: fix this...
	if ((m_gbi.nFinalWidth != m_gbi.nWidth) || (m_gbi.nFinalHeight != m_gbi.nHeight)) {
		MMTRACE("%s: auto-resizing is not fully implemented\n", gs_szMFIBPrefix);
		return E_NOTIMPL;
	}

	BOOL bRGBContent = (m_gbi.pixiPixFmt.nBPP > 8);

	if ((m_dwUsageFlags & flagUsageMapStatically) || 
		(bRGBContent && !(m_dwUsageFlags & flagUsageD3DTextureOnly)))
	{
		// use the current video mode format
		m_gbi.pixiFinalFmt = m_pColMgr->GetVideoModePixelInfo();
		m_gbi.bVideoModeDependent = TRUE;
		// color quantization is not yet implemented
//		if (m_gbi.pixiFinalFmt.nBPP <= 8)
//			return E_NOTIMPL;
	} else if (bRGBContent && (m_dwUsageFlags & flagUsageD3DTextureOnly)) {
		// use 16 bit for D3D textures
		if (FAILED(hr = m_pColMgr->GetPreferredPixelFormat(16, &(m_gbi.pixiFinalFmt))))
			return hr;
	} else {
		// otherwise, use the same format as the pixel content
		m_gbi.pixiFinalFmt = m_gbi.pixiPixFmt;
	}
	// store if the target pixi is the same as the content pixi
	m_gbi.bFinalFmtSame = (m_gbi.pixiFinalFmt == m_gbi.pixiPixFmt);

	return S_OK;
}


inline BOOL
IsFullSize(WORD nWidth, WORD nHeight, const RECT &rDim)
{
	return ((rDim.left == 0) && (rDim.top == 0) && 
			(rDim.right == (long) nWidth) && (rDim.bottom == (long) nHeight));
}


HRESULT
FillWithPrevious(LPFBI pfbi, LPFBI pfbiPrev, const RECT &rFullSize, const RECT &rHole)
{
	MMASSERT(pfbi && pfbiPrev);
	if ((pfbi->pdds == NULL) || (pfbiPrev->pdds == NULL))
		return E_NOTIMPL;
	return pfbi->pdds->BltFast(0, 0, pfbiPrev->pdds, (LPRECT) &rFullSize, DDBLTFAST_WAIT);
}


HRESULT
FillWithColor(LPFBI pfbi, DWORD dwColor, const RECT &rFullSize, const RECT &rHole)
{
	MMASSERT(pfbi);
	if (pfbi->pdds == NULL)
		return E_NOTIMPL;

	return ClearToColor(dwColor, (LPRECT) &rFullSize, pfbi->pdds);
}


HRESULT
CMFImageBuilder::CreateFinalPixels(LPFBI pfbi)
{
	MMASSERT(pfbi && pfbi->pPixels && m_gbi.pciFinal);

	HRESULT hr;
	DWORD cItems = m_dspfbiFrames.Items();

	// create a DDS
	LPDIRECTDRAWSURFACE pdds = NULL;
	MMASSERT(pfbi->pdds == NULL);
	if (FAILED(hr = m_gbi.pciFinal->CreatePlainSurface(m_pDD, m_gbi.nWidth, m_gbi.nHeight, &pdds)))
		return hr;
	pfbi->pdds = pdds;

	// fill the background, if necessary
	// REVIEW: we need to be able to deal with transparency fills (13.gif)
	if (!IsFullSize(m_gbi.nWidth, m_gbi.nHeight, pfbi->rDim)) {
		RECT rFullSize = {0, 0, m_gbi.nWidth, m_gbi.nHeight};
		if ((cItems != 0) && (pfbi->dwFlags & flagFrameFillWithPrevious)) {
			// fill with the previous image
			hr = FillWithPrevious(pfbi, m_dspfbiFrames[cItems - 1], rFullSize, pfbi->rDim);
		} else {
			// clear to the background color (as default)
			hr = FillWithColor(pfbi, m_gbi.dwFinalBackgroundColor, rFullSize, pfbi->rDim);
		}
		if (FAILED(hr))
			return hr;
	}

	// open the surface for writing
	DDSURFACEDESC ddsd;
	INIT_DXSTRUCT(ddsd);
	if (FAILED(hr = pdds->Lock(&(pfbi->rDim), &ddsd, DDLOCK_WAIT, NULL)))
	{
		ASSERT(FALSE);
		return hr;
	}
	pfbi->bDDSLocked = TRUE;
	pfbi->pSurfacePixels = (LPBYTE) ddsd.lpSurface;

	//
	// copy the frame pixels into the image
	//
	if (m_gbi.bFinalFmtSame && (pfbi->pPalette == NULL)) {
		//
		// straight copy
		//
		hr = BltFast(pfbi->pPixels, pfbi->nPitch, pfbi->pSurfacePixels, ddsd.lPitch, 
				pfbi->nWidth * m_gbi.cBytesPerPixel, pfbi->nHeight);
	} else if (pfbi->pPalette) {
		//
		// local palette translation copy
		//
		PALETTEENTRY rgpe[nMAXPALETTEENTRIES];
		// copy the palette to a PALETTEENTRY table
		if (FAILED(hr = BltFastRGBToRGB(pfbi->pPalette, 0, (LPBYTE) rgpe, 0, 
							1 << pfbi->nBPPPalette, 1, m_gbi.pixiPalFmt, g_pixiPALETTEENTRY)))
			return hr;
		// create a palette map
		CPaletteMap pmFrame;
		if (FAILED(hr = pmFrame.CreateMap(m_gbi.pixiPixFmt.nBPP, pfbi->nBPPPalette, rgpe, 
							m_gbi.pixiFinalFmt, m_gbi.pciFinal->pddp)))
			return hr;
		
		// do the translation Blt
		hr = pmFrame.BltFast(pfbi->pPixels, pfbi->nPitch, pfbi->pSurfacePixels, ddsd.lPitch, 
				pfbi->nWidth, pfbi->nHeight, m_gbi.dwFlags & flagTRANSPARENT, m_gbi.dwFinalTransColor);
	} else if (m_gbi.pPalette) {
		// global palette to RGB conversion
		hr = m_pmFinal.BltFast(pfbi->pPixels, pfbi->nPitch, pfbi->pSurfacePixels, ddsd.lPitch, 
				pfbi->nWidth, pfbi->nHeight, m_gbi.dwFlags & flagTRANSPARENT, m_gbi.dwFinalTransColor);
	} else {
		// RGB translation
		hr = BltFastRGBToRGB(pfbi->pPixels, pfbi->nPitch, pfbi->pSurfacePixels, ddsd.lPitch,
				pfbi->nWidth, pfbi->nHeight, m_gbi.pixiPixFmt, m_gbi.pixiFinalFmt);
	}

	pdds->Unlock(pfbi->pSurfacePixels);
	pfbi->pSurfacePixels = NULL;
	pfbi->bDDSLocked = FALSE;

	if (pfbi->dwFlags & flagPixDealloc) {
		MMDELETE(pfbi->pPixels);
	}

	return hr;
}


HRESULT
CMFImageBuilder::AddFrame(LPDIRECTDRAWSURFACE pdds, LPCOLORINFO pci, DWORD nFrameDelay)
{
	// verify arguments
	if (pdds == NULL)
		return E_INVALIDOBJECT;

	HRESULT hr;

	// get the surface information
	DDSURFACEDESC ddsd;
	INIT_DXSTRUCT(ddsd);
	if (FAILED(hr = pdds->GetSurfaceDesc(&ddsd)))
		return hr;

	DWORD cItems = m_dspfbiFrames.Items();
	if (cItems == 0) {
		// check if we are adding the first frame without having initialized the builder
		if (m_conm != conmNone)
			return E_ALREADYINITIALIZED;
		CPixelInfo pixiPixFmt(ddsd.ddpfPixelFormat);
		if (FAILED(hr = StartImage(ddsd.dwWidth, ddsd.dwHeight, g_pixiPALETTEENTRY, pixiPixFmt)))
			return hr;
	} else if (m_conm != conmExisting)
		return E_ALREADYINITIALIZED;

	// verify image dimensions
	if ((WORD(ddsd.dwWidth) != m_gbi.nWidth) || (WORD(ddsd.dwHeight) != m_gbi.nHeight) ||
		(m_gbi.pixiPixFmt != ddsd.ddpfPixelFormat))
	{
		MMTRACE("%s: invalid image dimensions on frame\n", gs_szMFIBPrefix);
		return E_INVALIDARG;
	}

	// allocate a new info frame
	MMASSERT(m_pfbiCur == NULL);
	m_pfbiCur = new CFrameBuilderInfo;
	if (m_pfbiCur == NULL) 
		return E_OUTOFMEMORY;

	UPDATEMAX(nFrameDelay, 12);
	m_pfbiCur->nDelay = nFrameDelay;

	LPDIRECTDRAWPALETTE pddp = NULL;
	if (pci == NULL) {
		//Review: dwFlags 
		DWORD dwFlags = 0, dwTransColor = 0;
		
		// check for palette
		if (ddsd.ddpfPixelFormat.dwRGBBitCount <= 8) {
			dwFlags |= flagPALETTIZED;
			if (FAILED(hr = pdds->GetPalette(&pddp)) || (pddp == NULL))
				goto e_AddFrame;
		}
		// check for a color key for the src surface
		if (ddsd.dwFlags & DDSD_CKSRCBLT) {
			dwFlags |= flagTRANSPARENT;
			dwTransColor = ddsd.ddckCKSrcBlt.dwColorSpaceLowValue;
		}
		
		// create the color info object
		// REVIEW: this should be easier -- i should be able to make it either way...
		if (pddp) {
			// palettized
			if (FAILED(hr = m_pColMgr->GetColorInfo(pddp, dwTransColor, dwFlags, &(m_gbi.pciFinal))))
				goto e_AddFrame;
		} else {
			// RGB
			if (FAILED(hr = m_pColMgr->GetColorInfo(m_gbi.pixiFinalFmt, dwTransColor, 
							dwFlags, &(m_gbi.pciFinal))))
				goto e_AddFrame;
		}
		MMRELEASE(pddp);
	} else {
		m_gbi.pciFinal = pci;
		pci->AddRef();
	}

	m_pfbiCur->pdds = pdds;
	pdds->AddRef();

	// store the frame info
	m_dspfbiFrames.Ins(m_pfbiCur);
	m_pfbiCur = NULL;

	return S_OK;
	
e_AddFrame:
	MMRELEASE(pddp);
	return hr;
}


// Function: Compile
// Notes:
//  Once an image is compiled, it can not be further modified.
//
//  The image compiler goes thru several stages:
//  - Mappings from one color model to the target model are generated.
//  - Pixels are mapped to the target color model, as necessary.
//  - A single frame size is computed for the image, based on usage info.
//  - An RLE is computed for each frame, if appropriate.
//  - Pixels are "mapped" to the target size based on usage flags, as
//    necessary.
HRESULT
CMFImageBuilder::Compile(CMFImage **ppmfi, BOOL *pbVideoModeDependent)
{
	MMASSERT(ppmfi && pbVideoModeDependent);
	HRESULT hr;
	DWORD i, cItems = m_dspfbiFrames.Items();

	if (cItems == 0) {
		MMTRACE("%s: no frames defined for image\n", gs_szMFIBPrefix);
		return E_NOTINITIALIZED;
	}

	// allocate the MFI
	CMFImage *pmfi = new CMFImage;
	if (pmfi == NULL)
		return E_OUTOFMEMORY;

	// insert the global info
	if (FAILED(hr = pmfi->Init(m_szName, m_gbi.nWidth, m_gbi.nHeight, m_gbi.pciFinal)))
		goto e_Compile;

	// insert all the frames
	for (i = 0; i < cItems; i++) {
		LPFBI pfbi = m_dspfbiFrames[i];
		MMASSERT(pfbi);
		if (FAILED(hr = pmfi->AddFrame(pfbi->prles, pfbi->pdds, pfbi->nDelay)))
			goto e_Compile;
	}

	*ppmfi = pmfi;
	*pbVideoModeDependent = m_gbi.bVideoModeDependent;

	return S_OK;

e_Compile:
	MMRELEASE(pmfi);

	return hr;
}


HRESULT
CMFImageBuilder::ComputeRLE(LPFBI pfbi)
{
	MMASSERT(pfbi && (pfbi->prles == NULL) && (m_gbi.pixiPixFmt.nBPP <= 8));
/*	HRESULT hr;
	LPRLESURFACE prles = NULL;

	// create the RLE
	prles = new CRLESurface;
	if (prles == NULL)
		return E_OUTOFMEMORY;
	
	if (FAILED(hr = prles->Init(pfbi->pPixels, pfbi->nWidth, pfbi->nWidth, 
						pfbi->nHeight, pfbi->nBPPUsed, 
						(pfbi->dwFlags & flagTRANSPARENT ? pfbi->dwTransColor : 257)))
		return hr;
*/

	// check if the RLE represents a reasonable level of compression
	MMTRACE("%s: RLE encoding is not fully implemented\n", gs_szMFIBPrefix);
	return E_NOTIMPL;
}
