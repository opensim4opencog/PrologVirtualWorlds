// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// File:	MFImage.cpp
// Author:	MM    (MM)

#include "MMImage.h"

#define nMINFRAMEDELAY 50

//
// CMFImageFrame
//
CMFImageFrame::~CMFImageFrame()
{
	MMRELEASE(pRLE);
	MMRELEASE(pDDS);
	MMRELEASE(pTexture);
}

//
// CMFImage
//
CMFImage::CMFImage()
{
	m_pImageCache = NULL;
	m_icc = g_iccNullCookie;
	m_nSize = 0;
	m_szName = NULL;

	m_pciGlobal = NULL;
//	m_nBPP = 0;
	m_nWidth = m_nHeight = 0;
	m_nRef = 1;
	m_pDD = NULL;
}

CMFImage::~CMFImage()
{
	MMASSERT(m_nRef == 0);

	// release all of the frames
	int i, nLimit = m_dspFrames.Items();
	for (i = 0; i < nLimit; i++) {
		delete m_dspFrames[i];
	}

	//MMDELETE(m_szName);
	//created w/ strdup use FREE
	if ( m_szName ) free( m_szName );
	m_szName = NULL;

	// release the global color info
	MMRELEASE(m_pciGlobal);

	MMRELEASE(m_pDD);
}

// Function: SetImageCache
//    This function implicitly bumps the ref count on the image by one
//  to account for the image cache's reference.  This is done so that
//  the proper stuff happens when we move an image from one cache to
//  another.
void
CMFImage::SetImageCache(CImageCache *pImageCache)
{
//	if (pImageCache == m_pImageCache)
//		return;

	// REVIEW: we need to clean this up if we want to make this function
	//   useful for multiple image caches -- for instance, there is no 
	//   attempt to remove the image from the old cache
	MMASSERT(((m_pImageCache == NULL) && pImageCache) || 
		(m_pImageCache && (pImageCache == NULL)));

	if (m_pImageCache) {
		m_icc = g_iccNullCookie;
	}
	if (m_pImageCache = pImageCache) {
		// check if we already have enough AddRef's to activate the object
		if (m_nRef > 1)
			m_pImageCache->Activate(this);
	}
}

HRESULT
CMFImage::Init(const char *szName, WORD nWidth, WORD nHeight, LPCOLORINFO pci)
{
	if (m_pciGlobal)
		return E_ALREADYINITIALIZED;

	if ((nWidth == 0) || (nHeight == 0) || (pci == NULL))
		return E_INVALIDARG;

	// store a copy of the string name
	if (szName && ((m_szName = _strdup(szName)) == NULL))
		return E_OUTOFMEMORY;

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_pciGlobal = pci;
	pci->AddRef();

	return S_OK;
}

HRESULT
CMFImage::AddFrame(CRLESurface *pRLE, LPDIRECTDRAWSURFACE pDDS, DWORD nDelay)
{
	if (m_pciGlobal == NULL)
		return E_NOTINITIALIZED;

	// make sure at least one object specifies the pixels
	if (((DWORD) pRLE | (DWORD) pDDS) == 0)
		return E_INVALIDOBJECT;

	CMFImageFrame *pmfif = new CMFImageFrame();
	if (pmfif == NULL)
		return E_OUTOFMEMORY;

	if (pRLE) {
		pmfif->pRLE = pRLE;
		pRLE->AddRef();
		m_nSize += pRLE->GetSize();
	}
	if (pDDS) {
		pmfif->pDDS = pDDS;
		pDDS->AddRef();
		// REVIEW: this only records the amount of space associated with the images
		m_nSize += ((m_nWidth * m_nHeight * m_pciGlobal->GetBPP()) >> 3);
	}

	// REVIEW: realistically, a frame delay shouldn't be less than a 20th of a second
	if (nDelay < nMINFRAMEDELAY)
		nDelay = nMINFRAMEDELAY;

	pmfif->nDelay = nDelay;

	m_dspFrames.Ins(pmfif);
	
	return S_OK;
}


HRESULT
CMFImage::BltSetup(DWORD iFrame, DWORD bvFlags, const CMFImageFrame **ppFrame, 
				   DWORD *pdwBltFlags, DWORD *pdwFastBltFlags) const
{
	MMASSERT(ppFrame && pdwBltFlags && pdwFastBltFlags);
	const CMFImageFrame *pFrame;

	if ((iFrame >= GetNFrames()) || ((pFrame = m_dspFrames[iFrame]) == NULL))
		return E_INVALIDARG;

	DWORD dwBltFlags = DDBLT_ASYNC;
	DWORD dwFastBltFlags = 0;
	if ((m_pciGlobal->bvColorFlags & flagTRANSPARENT) && 
		((bvFlags & flagBLT_IGNORETRANSPARENCY) == 0)) {
		dwBltFlags |= DDBLT_KEYSRC;
		dwFastBltFlags |= DDBLTFAST_SRCCOLORKEY;
	}

	*ppFrame = pFrame;
	*pdwBltFlags = dwBltFlags;
	*pdwFastBltFlags = dwFastBltFlags;

	return S_OK;
}


HRESULT
CMFImage::BltToSurface(DWORD iFrame, LPRECT prSrc, LPDIRECTDRAWSURFACE pddsDst, LPRECT prDst,
					   DWORD bvFlags) const
{
	HRESULT hr = E_INVALIDARG;
	const CMFImageFrame *pFrame;
	DWORD dwBltFlags, dwFastBltFlags;

	if (prDst == NULL)
		return E_INVALIDARG;

	// check if we can actually do a fast blt instead
	if (((prDst->right - prDst->left) == (prSrc->right - prSrc->left)) &&
		((prDst->bottom - prDst->top) == (prSrc->bottom - prSrc->top))) {
		MMASSERT((prDst->left <= nMAXIMAGEDIM) && (prDst->top <= nMAXIMAGEDIM));
		return BltFastToSurface(iFrame, prSrc, pddsDst, 
				(WORD) prDst->left, (WORD) prDst->top, bvFlags);
	}

	if ((pddsDst == NULL) ||
		FAILED(hr = BltSetup(iFrame, bvFlags, &pFrame, &dwBltFlags, &dwFastBltFlags)))
		return hr;
		
	RECT rSrc;
	if (prSrc == NULL) {
		SetRect(&rSrc, 0, 0, m_nWidth, m_nHeight);
		prSrc = &rSrc;
	}

	// decide how to do the blt
	if (pFrame->pDDS) {
		hr = pddsDst->Blt(prDst, pFrame->pDDS, prSrc, dwBltFlags, NULL);
	} else if (pFrame->pRLE) {
		hr = pFrame->pRLE->BltToSurface(prSrc, pddsDst, prDst, bvFlags);
	} else {
		hr = E_INSUFFICIENTDATA;
	}

	return hr;
}

HRESULT
CMFImage::BltFastToSurface(DWORD iFrame, LPRECT prSrc, LPDIRECTDRAWSURFACE pddsDst, 
						   WORD nXPos, WORD nYPos, DWORD bvFlags) const
{
	HRESULT hr = E_INVALIDARG;
	const CMFImageFrame *pFrame;
	DWORD dwBltFlags, dwFastBltFlags;

	if ((pddsDst == NULL) || 
		FAILED(hr = BltSetup(iFrame, bvFlags, &pFrame, &dwBltFlags, &dwFastBltFlags)))
		return hr;

	RECT rSrc;
	if (prSrc == NULL) {
		SetRect(&rSrc, 0, 0, m_nWidth, m_nHeight);
		prSrc = &rSrc;
	}

	// decide how to do the blt
	if (pFrame->pDDS) {
		//
		// REVIEW: this is totally screwed -- i need to define a color model map class
		//
		if (bvFlags & flagBLT_USEDYNAMICMAPPING) {
			/*
			hr = m_pciGlobal->pmVideoMode.BltFast(pFrame->pDDS, prSrc, pddsDst, 
					nXPos, nYPos, bvFlags);
			// try the straight blt if we failed
			if (FAILED(hr))
				hr = pddsDst->BltFast(nXPos, nYPos, pFrame->pDDS, prSrc, dwFastBltFlags);
			*/
			//RAC lets change the order of these so the source key blts might work 
			hr = pddsDst->BltFast(nXPos, nYPos, pFrame->pDDS, prSrc, dwFastBltFlags);
			// try the straight blt if we failed
			if (FAILED(hr))
				hr = m_pciGlobal->pmVideoMode.BltFast(pFrame->pDDS, prSrc, pddsDst, nXPos, nYPos, bvFlags);

		} else {
			hr = pddsDst->BltFast(nXPos, nYPos, pFrame->pDDS, prSrc, dwFastBltFlags);
		}
	} else if (pFrame->pRLE) {
		if (bvFlags & flagBLT_USEDYNAMICMAPPING) {
			hr = m_pciGlobal->pmVideoMode.BltFast(pFrame->pRLE, prSrc, pddsDst, 
					nXPos, nYPos, bvFlags);
		} else {
			hr = pFrame->pRLE->BltFastToSurface(prSrc, pddsDst, nXPos, nYPos, bvFlags);
		}
	} else {
		hr = E_INSUFFICIENTDATA;
	}
	return hr;
}


#define shiftnINTERVALS		8
#define nINTERVALS			(1 << shiftnINTERVALS)
#define shiftnFI			16		/* fixed point shift */

typedef struct IntervalInfo {
	WORD				nFirstSrcOffset;
	WORD				rgIntervals[nINTERVALS];
} IntervalInfo;

// Function: ComputeLines
//    Use this function to compute the horizontal or vertical lines for tiling.
//  The arguments are given in fixed point.
static HRESULT
_ComputeIntervals(DWORD fiDstWidth,
				  DWORD fiSrcWidth, DWORD fiSrcOffset,
				  IntervalInfo *pii)
{
	MMASSERT(fiDstWidth && (fiSrcOffset < fiSrcWidth));

	// check if there are too many intervals
	if (((fiDstWidth >> shiftnFI)) > (fiSrcWidth >> (shiftnFI - shiftnINTERVALS)))
		return E_INVALIDARG;

	WORD *pInterval = pii->rgIntervals;
	DWORD fiCurPos;

	// set up the first interval
	if (fiSrcOffset == 0) {
		pii->nFirstSrcOffset = 0;
		*pInterval++ = (WORD) (fiSrcWidth >> shiftnFI);
		fiCurPos = fiSrcWidth;
	} else {
		pii->nFirstSrcOffset = (WORD) ((fiSrcWidth - fiSrcOffset) >> shiftnFI);
		*pInterval++ = (WORD) (fiSrcOffset >> shiftnFI);
		fiCurPos = fiSrcOffset;
	}
	// create the middle intervals
	for (; fiCurPos <= fiDstWidth; fiCurPos += fiSrcWidth) {
		*pInterval++ = (WORD) (fiSrcWidth >> shiftnFI);
	}
	// fix the last interval
	if (fiCurPos > fiDstWidth) {
		*--pInterval -= (WORD) ((fiCurPos - fiDstWidth) >> shiftnFI);
	}
	return S_OK;
}


HRESULT
CMFImage::BltTiledToSurface(DWORD iFrame, LPDIRECTDRAWSURFACE pddsDst,
							LPRECT prDst, DWORD fiXOffset, DWORD fiYOffset,
							DWORD fiXScale, DWORD fiYScale, DWORD bvFlags) const
{
	if ((fiXScale | fiYScale) == (1 << shiftnFI))
		return BltFastTiledToSurface(iFrame, pddsDst, prDst, fiXOffset, fiYOffset, bvFlags);

	HRESULT hr = E_INVALIDOBJECT;
	const CMFImageFrame *pFrame;
	DWORD dwBltFlags, dwFastBltFlags;

	if ((prDst == NULL) || (pddsDst == NULL) || 
		FAILED(hr = BltSetup(iFrame, bvFlags, &pFrame, &dwBltFlags, &dwFastBltFlags)))
		return hr;

	return E_NOTIMPL;
}

HRESULT
CMFImage::BltFastTiledToSurface(DWORD iFrame, LPDIRECTDRAWSURFACE pddsDst, LPRECT prDst, 
								DWORD fiXOffset, DWORD fiYOffset, DWORD bvFlags) const
{
	HRESULT hr = E_INVALIDOBJECT;
	const CMFImageFrame *pFrame;
	DWORD dwBltFlags, dwFastBltFlags;

	if ((prDst == NULL) || (pddsDst == NULL) || 
		FAILED(hr = BltSetup(iFrame, bvFlags, &pFrame, &dwBltFlags, &dwFastBltFlags)))
		return hr;

	MMASSERT((prDst->right > prDst->left) && (prDst->bottom > prDst->top));
	MMASSERT(((prDst->right - prDst->left) < 0x10000) && (((prDst->bottom - prDst->top) < 0x10000)));

	// get the tile interval information in each dimension
	IntervalInfo iiCol, iiRow;
	if (FAILED(hr = _ComputeIntervals((((DWORD) (prDst->right - prDst->left)) << shiftnFI),
						(((DWORD) m_nWidth) << shiftnFI), fiXOffset, &iiCol)) ||
		FAILED(hr = _ComputeIntervals((((DWORD) (prDst->bottom - prDst->top)) << shiftnFI),
						(((DWORD) m_nHeight) << shiftnFI), fiYOffset, &iiRow)))
		return hr;

	RECT rSrc;
	WORD nDstX, nDstY, nRight = (WORD) prDst->right, nBottom = (WORD) prDst->bottom;
	WORD *pnColI, *pnRowI;

	pnRowI = iiRow.rgIntervals;
	rSrc.bottom = (rSrc.top = iiRow.nFirstSrcOffset) + *pnRowI;
	nDstY = (WORD) prDst->top;
	goto BltFirstRow;
	for (; nDstY != nBottom; nDstY += *pnRowI++) {
		rSrc.top = 0;
		rSrc.bottom = *pnRowI;
BltFirstRow:
		pnColI = iiCol.rgIntervals;
		rSrc.right = (rSrc.left = iiCol.nFirstSrcOffset) + *pnColI;
		nDstX = (WORD) prDst->left;
		goto BltFirstRect;
		for (; nDstX != nRight; nDstX += *pnColI++) {
			rSrc.left = 0;
			rSrc.right = *pnColI;
BltFirstRect:
			if (pFrame->pDDS) {
				if (FAILED(hr = pddsDst->BltFast(nDstX, nDstY, pFrame->pDDS, &rSrc, dwFastBltFlags)))
					return hr;
			} else if (pFrame->pRLE) {
				return E_NOTIMPL;
			} else {
				return E_INSUFFICIENTDATA;
			}
		}
	}
	
	return hr;
}


typedef struct _D3DTextureCallbackInfo {
	CMFImage *			pImage;
	CMFImageFrame *		pFrame;
} _D3DTextureCallbackInfo;

// REVIEW: once a texture has been created on a frame, that frame can not be removed
//  until that texture is destroyed.  otherwise, there will be a dangling pointer
//  situation in the D3DTextureCallbackInfo.
static void 
_D3DTextureDestroyCallback(LPDIRECT3DRMOBJECT pObject, void *pvArg)
{
	// REVIEW: this is NOT thread safe -- to make it so, it would be necessary to
	//  critical section this entire function as well as the destroy callback
	//  w.r.t. the same CS
//	MMTRACE("***_D3DTextureDestroyCallback: %p\n", pvArg);
	_D3DTextureCallbackInfo *ptci = (_D3DTextureCallbackInfo *) pvArg;
	MMASSERT(ptci && ptci->pFrame && ptci->pImage);
	ptci->pFrame->pTexture = NULL;
	ptci->pImage->Release();
	MMDELETE(ptci);
}


HRESULT
CMFImage::GetD3DRMTexture(DWORD iFrame, LPDIRECTDRAW pDD, LPDIRECT3DRM pD3DRM, 
						  LPDIRECT3DRMTEXTURE *ppTexture)
{

	return GetD3DRMTextureEx(iFrame,flagTEXTURE_STRETCHNEAREST,pDD,pD3DRM,ppTexture);
/*
	MMASSERT(ppTexture);

	HRESULT hr;
	CMFImageFrame *pFrame;
	if ((iFrame >= GetNFrames()) ||
		((pFrame = m_dspFrames[iFrame]) == NULL))
		return E_INVALIDARG;
	
	// REVIEW: this is NOT thread safe -- to make it so, it would be necessary to
	//  critical section this entire function as well as the destroy callback
	//  w.r.t. the same CS
	LPDIRECT3DRMTEXTURE pTexture = pFrame->pTexture;
	if (pTexture == NULL) {
		if (pD3DRM == NULL)
			return E_INVALIDARG;

		if (!pFrame->pDDS) {
			if (FAILED(hr = CreateDDSOnFrame(pDD, iFrame))) {
				return hr;
			}
		}
		// by this point, we should have instantiated a DDS for the frame
		MMASSERT(pFrame->pDDS);

		// create a structure to pass as an argument to the texture's destroy callback
		_D3DTextureCallbackInfo *ptci = new _D3DTextureCallbackInfo;
		if (ptci == NULL)
			return E_OUTOFMEMORY;
		ptci->pImage = this;
		ptci->pFrame = pFrame;

		// create the texture from the DDS
		if (FAILED(hr = pD3DRM->CreateTextureFromSurface(pFrame->pDDS, &pTexture)) ||
			FAILED(hr = pTexture->AddDestroyCallback(_D3DTextureDestroyCallback, (LPVOID) ptci))) {
			MMDELETE(ptci);
			MMRELEASE(pTexture);
			return hr;
		}

		BYTE nBPP = GetDepth();
		if (nBPP <= 8) {
			pTexture->SetColors(1 << nBPP);
			pTexture->SetShades(1);
		}

		// save this texture in the frame, but don't AddRef (to avoid a circular reference)
		pFrame->pTexture = pTexture;

		// AddRef the MFImage since this frame is being used for a D3D texture
		this->AddRef();
	} else {
		pTexture->AddRef();
	}
	
	*ppTexture = pTexture;
	
	return S_OK;
	*/
}

HRESULT
CMFImage::GetD3DRMTextureEx(DWORD iFrame, long fMode, LPDIRECTDRAW pDD, LPDIRECT3DRM pD3DRM, 
							LPDIRECT3DRMTEXTURE *ppTexture)
{
	MMASSERT(ppTexture);

	HRESULT hr;
	CMFImageFrame *pFrame;
	if ((iFrame >= GetNFrames()) ||
		((pFrame = m_dspFrames[iFrame]) == NULL))
		return E_INVALIDARG;
	
	// REVIEW: this is NOT thread safe -- to make it so, it would be necessary to
	//  critical section this entire function as well as the destroy callback
	//  w.r.t. the same CS
	LPDIRECT3DRMTEXTURE pTexture = pFrame->pTexture;
	if (pTexture == NULL) {
		if (pD3DRM == NULL)
			return E_INVALIDARG;

		if (!pFrame->pDDS) {
			if (FAILED(hr = CreateDDSOnFrame(pDD, iFrame))) {
				return hr;
			}
		}
		// by this point, we should have instantiated a DDS for the frame
		MMASSERT(pFrame->pDDS);

		// create a structure to pass as an argument to the texture's destroy callback
		_D3DTextureCallbackInfo *ptci = new _D3DTextureCallbackInfo;
		if (ptci == NULL)
			return E_OUTOFMEMORY;
		ptci->pImage = this;
		ptci->pFrame = pFrame;

		// create the texture from the DDS
		if (FAILED(hr = CreateD3DRMTexture(pFrame->pDDS, fMode, pDD, pD3DRM, &pTexture)) ||
			FAILED(hr = pTexture->AddDestroyCallback(_D3DTextureDestroyCallback, (LPVOID) ptci))) {
			MMDELETE(ptci);
			MMRELEASE(pTexture);
			return hr;
		}

		BYTE nBPP = GetDepth();
		if (nBPP <= 8) {
			pTexture->SetColors(1 << nBPP);
			pTexture->SetShades(1);
		}

		// save this texture in the frame, but don't AddRef (to avoid a circular reference)
		pFrame->pTexture = pTexture;

		// AddRef the MFImage since this frame is being used for a D3D texture
		this->AddRef();
	} else {
		pTexture->AddRef();
	}
	
	*ppTexture = pTexture;
	
	return S_OK;
}

HRESULT
CMFImage::CreateFittedSurface(LPDIRECTDRAW pDD, LPDIRECTDRAWSURFACE *ppdds) const
{
	if (m_pciGlobal == NULL)
		return E_NOTINITIALIZED;
	if (pDD == NULL) {
		if (m_pImageCache == NULL && m_pDD == NULL)
			return E_NOTINITIALIZED;
		else if (m_pImageCache)
			return m_pImageCache->CreatePlainSurface(m_nWidth, m_nHeight, m_pciGlobal, ppdds);
		else
			return m_pciGlobal->CreatePlainSurface(m_pDD, m_nWidth, m_nHeight, ppdds);
	} else {
		return m_pciGlobal->CreatePlainSurface(pDD, m_nWidth, m_nHeight, ppdds);
	}
}

HRESULT
CMFImage::CreateFittedSurfaceEx(int dx, int dy, LPDIRECTDRAW pDD, LPDIRECTDRAWSURFACE *ppdds, VARIANT_BOOL bForTexture) const
{
	if (m_pciGlobal == NULL)
		return E_NOTINITIALIZED;
	if (pDD == NULL) {
		if (m_pImageCache == NULL && m_pDD == NULL)
			return E_NOTINITIALIZED;
		else if (m_pImageCache)
			return m_pImageCache->CreatePlainSurface(dx, dy, m_pciGlobal, ppdds, bForTexture);
		else
			return m_pciGlobal->CreatePlainSurface(m_pDD, dx, dy, ppdds, bForTexture);
	} else {
		return m_pciGlobal->CreatePlainSurface(pDD, dx, dy, ppdds, bForTexture);
	}
}

HRESULT
CMFImage::CreateDDSOnFrame(LPDIRECTDRAW pDD, DWORD iFrame)
{
	MMASSERT(iFrame < GetNFrames());
	CMFImageFrame *pFrame = m_dspFrames[iFrame];
	MMASSERT(pFrame);

	HRESULT hr;

	if (pFrame->pDDS)
		return S_OK;
	else {
		LPDIRECTDRAWSURFACE pdds = NULL;
		RECT rBlt;
		SetRect(&rBlt, 0, 0, m_nWidth, m_nHeight);
		if (FAILED(hr = CreateFittedSurface(pDD, &pdds)) ||
			FAILED(hr = BltFastToSurface(iFrame, &rBlt, pdds, 0, 0, flagBLT_IGNORETRANSPARENCY))) {
			MMRELEASE(pdds);
			return hr;
		}
		// set the surface on the frame after the BltToSurface, 
		// so that we get the RLE expansion
		pFrame->pDDS = pdds;
	}
	return hr;
}

DWORD
CMFImage::GetPixel(DWORD iFrame, long nX, long nY)
{
	MMASSERT(INARRAY(nX, m_nWidth) && INARRAY(nY, m_nHeight));

	// check if the frame is valid
	CMFImageFrame *pFrame;
	if ((iFrame >= GetNFrames()) || ((pFrame = m_dspFrames[iFrame]) == NULL))
		return 0;

	DWORD dwPixel = 0;

	if (pFrame->pDDS) {
		MMASSERT(m_pciGlobal);

		DDSURFACEDESC ddsd;
		INIT_DXSTRUCT(ddsd);

		RECT rPixel = {nX, nY, nX + 1, nY + 1};

		// lock the surface
		if (FAILED(pFrame->pDDS->Lock(&rPixel, &ddsd, DDLOCK_READONLY, NULL)))
		{
			ASSERT(FALSE);
			return 0;
		}
		// get the pixel based on the color model
		dwPixel = m_pciGlobal->PackPixel((BYTE *) ddsd.lpSurface);
		// unlock the surface
		pFrame->pDDS->Unlock(ddsd.lpSurface);
	} else {
		// the image is not defined as a DDS
		MMASSERT(0);
		return 0;
	}
	return dwPixel;
}

BOOL 
CMFImage::Pick(DWORD iFrame, long nX, long nY, BOOL bIgnoreTransparency)
{
	return (IsInside(nX, nY) && 
		(bIgnoreTransparency || !(m_pciGlobal->bvColorFlags & flagTRANSPARENT) || 
		(GetPixel(iFrame, nX, nY) != m_pciGlobal->dwTransColor)));
}

BOOL IsPowerOf2( int value )
{
	if ( value <= 1 ) return FALSE;
	return ( value -1 ) & value ? FALSE : TRUE;
}

HRESULT CMFImage::CalculateTextureSize( int idx, int idy, long fMode, int &tdx, int &tdy )
{
HRESULT hr = S_OK;

	tdx = tdy = 1;	
	if ( IsPowerOf2( idx ))
	{
		tdx = idx;
	}
	else
	{
		while ( tdx <= idx ) tdx *= 2;
	}

	if ( IsPowerOf2( idy ))
	{
		tdy = idy;	
	}
	else
	{
		while ( tdy <= idy ) tdy *= 2;
	}

	switch ( fMode )
	{
	case flagTEXTURE_DONOTHING:
		tdx = idx;
		tdy = idy;
	break;

	case flagTEXTURE_BLTNEXT:
	case flagTEXTURE_STRETCHNEXT:
	case flagTEXTURE_CENTERBLTNEXT:
	 // Don't do anything here...
	break;
	case flagTEXTURE_BLTNEAREST:
	case flagTEXTURE_STRETCHNEAREST:
	default: // closest power of 2
		{

			if ( tdx + (tdx/2) - ( 2 * idx ) > 0 )
				tdx = tdx /2;

			if ( tdy + (tdy/2) - ( 2 * idy ) > 0 )
				tdy = tdy /2;
		}
	break;
	}

return hr;
}

HRESULT CMFImage::CreateD3DRMTexture(LPDIRECTDRAWSURFACE pdds, long fMode, LPDIRECTDRAW pDD, LPDIRECT3DRM pD3DRM,
							LPDIRECT3DRMTEXTURE *ppTexture)
{
HRESULT hr = S_OK;
BYTE nBPP = GetDepth();
LPDIRECT3DRMTEXTURE pTexture = NULL;
LPDIRECTDRAWSURFACE pddsSized = NULL;
RECT rect, Dest;
int dx, dy;

	hr = ::GetSurfaceDimensions(pdds, &rect);
	if ( FAILED( hr ))
		goto ERROR_ENCOUNTERED;

	hr = CalculateTextureSize( rect.right, rect.bottom, fMode, dx,dy);
	if ( FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// Always create a new surface for textures.
	Dest.left = Dest.top = 0; Dest.right = dx; Dest.bottom = dy;
	hr = CreateFittedSurfaceEx(dx, dy, pDD, &pddsSized, VARIANT_TRUE);
	if ( FAILED( hr ))
		goto ERROR_ENCOUNTERED;

	if (( fMode == flagTEXTURE_BLTNEAREST ) ||
		( fMode == flagTEXTURE_BLTNEXT ))
	{
		if ( rect.right > Dest.right ) 
			rect.right = Dest.right;
		else
			Dest.right = rect.right;

		if ( rect.bottom < Dest.bottom )
			rect.bottom = Dest.bottom;
		else
			Dest.bottom = rect.bottom;
	}

	hr = pddsSized->Blt(&Dest,pdds,&rect,DDBLT_WAIT ,NULL);
	if ( FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pD3DRM->CreateTextureFromSurface(pddsSized, &pTexture);
	if ( FAILED( hr ))
		goto ERROR_ENCOUNTERED;


	if (nBPP <= 8) {
		pTexture->SetColors(1 << nBPP);
		pTexture->SetShades(1);
	}

	*ppTexture = pTexture;
	(*ppTexture)->AddRef();

ERROR_ENCOUNTERED:

	MMRELEASE( pddsSized );
	MMRELEASE( pTexture );

	return hr;
}