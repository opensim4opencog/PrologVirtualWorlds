// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// D2DRMImg.cpp : Implementation of Cd2dApp and DLL registration.

#include "stdafx.h"
#include "D2DTrans.h"
#include "DispList.h"
#include "D2DRMImg.h"
#include "D2DRMPka.h"


/////////////////////////////////////////////////////////////////////////////
//
CDirect2DRMImage::CDirect2DRMImage()
{
	D2DOBJINIT();

	m_nTimeLastTicked = 0;
	m_nVisImage = 0;
	m_bvFlags = 0;
	m_fMode = flagTEXTURE_DONOTHING;
	m_t2dTile.Init();
	m_pImageData = NULL;

	m_pD2DRMPriv = NULL;
	m_posTickedList = NULL;

	m_pddsTexture = NULL;
	m_pd3drmTexture = NULL;
}

CDirect2DRMImage::~CDirect2DRMImage()
{
	D2DOBJDESTROY();
	MMRELEASE(m_pddsTexture);
	MMRELEASE(m_pImageData);
}


STDMETHODIMP
CDirect2DRMImage::InitFromMFImage(LPD2DRMPRIVATE pD2DRMPriv, CMFImage *pmfi, DWORD dwFlags)
{
//	MMTRACE("CDirect2DRMImage::InitFromMFImage\n");
	if (m_pImageData != NULL)
		return E_ALREADYINITIALIZED;

	if (pmfi == NULL)
		return E_INVALIDOBJECT;

	m_pImageData = pmfi;
	pmfi->AddRef();

	if (dwFlags & flagCREATE_TILED)
		m_bvFlags |= flagTILED;
	if ((m_pImageData->GetNFrames() > 1) && ((dwFlags & flagCREATE_IGNOREANIMATION) == 0))
		m_bvFlags |= flagANIMATED;
	if (m_pImageData->IsPalettized())
		m_bvFlags |= flagPALETTIZED;
	if (m_pImageData->IsTransparent())
		m_bvFlags |= flagTRANSPARENT;


	// Set up weak ref back pointer to D2DRM.
	m_pD2DRMPriv = pD2DRMPriv;

	return put_VisibleImage(0);
}

inline void
_D2DRectToRECT(LPD2DRECT pr2, LPRECT pr)
{
	pr->left = (LONG) pr2->left;
	pr->top = (LONG) pr2->top;
	pr->right = (LONG) pr2->right;
	pr->bottom = (LONG) pr2->bottom;
}

#define fFIXEDPOINTSCALE ((float) 0x10000)

// Function: BltToSurface
//    This function blts the src rect of the image to the dst rect on the
//  dst dds.  If the image is tiled, we apply the tiling offset & scale and
//  then tile the image in the dst rect.
STDMETHODIMP
CDirect2DRMImage::BltToSurface(LPD2DRECT pr2Src, LPDIRECTDRAWSURFACE pddsDst, 
							   LPD2DRECT pr2Dst, DWORD dwFlags)
{
	HRESULT hr;
	if (m_pImageData == NULL)
		return E_NOTINITIALIZED;

	if (pddsDst == NULL) {
		return E_INVALIDOBJECT;
	}

	RECT rDst;
	if (pr2Dst) {
		// convert to integer
		_D2DRectToRECT(pr2Dst, &rDst);
	} else {
		// assume the whole surface
		::GetSurfaceDimensions(pddsDst, &rDst);
	}
	
	// Make sure we blt the correct image.
	if (m_bvFlags & flagANIMATED)
	{
		if (m_nTimeLastTicked != 0)
			Tick(GetTickCount() - m_nTimeLastTicked);
		m_nTimeLastTicked = GetTickCount();
	}
		
	if (m_bvFlags & flagTILED) {
		// convert to 16.16 fixed point
		DWORD fiXOffset = ((DWORD) (m_t2dTile.GetXPos() * fFIXEDPOINTSCALE));
		DWORD fiYOffset = ((DWORD) (m_t2dTile.GetYPos() * fFIXEDPOINTSCALE));
		DWORD fiXScale = ((DWORD) (m_t2dTile.GetXScale() * fFIXEDPOINTSCALE));
		DWORD fiYScale = ((DWORD) (m_t2dTile.GetYScale() * fFIXEDPOINTSCALE));
		hr = m_pImageData->BltTiledToSurface(m_nVisImage, pddsDst, &rDst, 
				fiXOffset, fiYOffset, fiXScale, fiYScale, 0);
	} else {
		RECT rSrc;
		if (pr2Src) {
			_D2DRectToRECT(pr2Src, &rSrc);
		} else {
			SetRect(&rSrc, 0, 0, m_pImageData->GetWidth(), m_pImageData->GetHeight());
		}

		hr = m_pImageData->BltToSurface(m_nVisImage, &rSrc, pddsDst, &rDst, dwFlags);
	}
	return hr;
}

STDMETHODIMP
CDirect2DRMImage::CreateFittedSurface(LPDIRECTDRAWSURFACE *ppdds)
{
	MMASSERT(ppdds);

	if (m_pImageData == NULL)
		return E_NOTINITIALIZED;

	return m_pImageData->CreateFittedSurface(NULL, ppdds);
}


STDMETHODIMP
CDirect2DRMImage::ExportD3DTexture(LPDIRECT3DRM pD3DRM, LPDIRECTDRAWSURFACE *ppddsTexture, 
								   LPDIRECT3DRMTEXTURE *ppTexture)
{
	return  ExportD3DTextureEx(pD3DRM, ppddsTexture, flagTEXTURE_BLTNEXT/*flagTEXTURE_BLTNEAREST flagTEXTURE_STRETCHNEAREST*/, ppTexture);
	/*
	MMASSERT(ppddsTexture && ppTexture);
	if (m_pImageData == NULL)
		return E_NOTINITIALIZED;

	if ((pD3DRM == NULL))
		return E_INVALIDOBJECT;

	HRESULT hr;

	// - create a DDS
	// - we don't have to pass in the DD object because a D2DI has already attached
	//   the MFI to an image cache
	LPDIRECTDRAWSURFACE pddsTexture = NULL;
	LPDIRECT3DRMTEXTURE pTexture = NULL;
	if (FAILED(hr = m_pImageData->CreateFittedSurface(NULL, &pddsTexture)) ||
		FAILED(hr = m_pImageData->BltFastToSurface(m_nVisImage, NULL, pddsTexture,
						0, 0, flagBLT_IGNORETRANSPARENCY)) ||
		FAILED(hr = pD3DRM->CreateTextureFromSurface(pddsTexture, &pTexture))) {
		MMRELEASE(pddsTexture);
		MMRELEASE(pTexture);
		return hr;
	}

	if (m_bvFlags & flagPALETTIZED) {
		DWORD nColors;
		get_NColors(&nColors);
		pTexture->SetColors(nColors);
		pTexture->SetShades(1);
	}

	*ppddsTexture = pddsTexture;
	*ppTexture = pTexture;

	return hr;
	*/
}




STDMETHODIMP
CDirect2DRMImage::ExportD3DTextureEx(LPDIRECT3DRM pD3DRM, LPDIRECTDRAWSURFACE *ppddsTexture, 
								   long fMode, LPDIRECT3DRMTEXTURE *ppTexture)
{
	MMASSERT(ppddsTexture && ppTexture);
	if (m_pImageData == NULL)
		return E_NOTINITIALIZED;

	if ((pD3DRM == NULL))
		return E_INVALIDOBJECT;

	HRESULT hr;
	int dx,dy;

	m_fMode = fMode;
	
	hr = m_pImageData->CalculateTextureSize( m_pImageData->GetWidth(),m_pImageData->GetHeight(),fMode, dx,dy);
	if ( FAILED(hr))
		return hr;

	// - create a DDS
	// - we don't have to pass in the DD object because a D2DI has already attached
	//   the MFI to an image cache
	LPDIRECTDRAWSURFACE pddsTexture = NULL;
	LPDIRECT3DRMTEXTURE pTexture = NULL;

	// default for strech
	RECT Src = {0,0, m_pImageData->GetWidth(),m_pImageData->GetHeight() };
	RECT Dest = {0,0,dx,dy};
	// No stretching in this mode..., blt the minimum area of source or destination
	if (( fMode == flagTEXTURE_BLTNEAREST ) ||
		( fMode == flagTEXTURE_BLTNEXT ) ||
		( fMode == flagTEXTURE_CENTERBLTNEXT))
	{
		int mindx = min (Src.right, Dest.right);
		Src.right = Dest.right = mindx;
		int mindy = min(Src.bottom, Dest.bottom);
		Src.bottom = Dest.bottom = mindy;
	}
	// center the destination
	if ( fMode == flagTEXTURE_CENTERBLTNEXT )
	{
		int xoffset = (dx - Dest.right ) >> 1;
		int yoffset = (dy - Dest.bottom ) >> 1;
		Dest.left += xoffset;
		Dest.right += xoffset;
		Dest.top += yoffset;
		Dest.bottom += yoffset;
	}

	if (FAILED(hr = m_pImageData->CreateFittedSurfaceEx(dx,dy,NULL, &pddsTexture, VARIANT_TRUE)) ||
		FAILED(hr = m_pImageData->BltToSurface(m_nVisImage, &Src,pddsTexture, &Dest,flagBLT_IGNORETRANSPARENCY)) ||
		FAILED(hr = pD3DRM->CreateTextureFromSurface(pddsTexture, &pTexture))) {
		MMRELEASE(pddsTexture);
		MMRELEASE(pTexture);
		return hr;
	}

	if (m_bvFlags & flagPALETTIZED) {
		DWORD nColors;
		get_NColors(&nColors);
		pTexture->SetColors(nColors);
		pTexture->SetShades(1);
	}

	*ppddsTexture = pddsTexture;
	*ppTexture = pTexture;

	return hr;
}

void 
_D2DRMImageTextureDestroyCallback(LPDIRECT3DRMOBJECT pObject, void *pvArg)
{
	CDirect2DRMImage *pImage = (CDirect2DRMImage*)pvArg;
	MMASSERT(pImage);

	// Just zero this out since we did not reference count it.
	pImage->m_pd3drmTexture = NULL;

	// Remove this image from the ticked list.
	if (pImage->m_posTickedList)
	{
		pImage->m_pD2DRMPriv->DelTicked(pImage->m_posTickedList);
		pImage->m_posTickedList = NULL;
	}

	// Release extra ref added on image when texture was created.
	((LPD2DRMIMAGEPRIVATE)pImage)->Release();
}

STDMETHODIMP
CDirect2DRMImage::GetD3DTexture(LPDIRECT3DRM pD3DRM, LPDIRECT3DRMTEXTURE *ppTexture)
{
	if (m_pImageData == NULL)
		return E_NOTINITIALIZED;

	if ((pD3DRM == NULL) || (ppTexture == NULL))
		return E_NULLPOINTER;

	HRESULT hr;

	if (m_pd3drmTexture == NULL)
	{
		// check if we are dealing with a single frame image
		if (m_pImageData->GetNFrames() == 1)
		{
			if (FAILED(hr = m_pImageData->GetD3DRMTexture(0, NULL, pD3DRM, &m_pd3drmTexture)))
				return hr;
		}
		else
		{
			if (FAILED(hr = ExportD3DTexture(pD3DRM, &m_pddsTexture, &m_pd3drmTexture)))
				return hr;
			if (m_bvFlags & flagANIMATED)
				m_posTickedList = m_pD2DRMPriv->InsTicked(this);
		}			
		if (FAILED(hr = m_pd3drmTexture->AddDestroyCallback(_D2DRMImageTextureDestroyCallback,
															(LPVOID) this)))
			return hr;
			
		// REVIEW: we want the D2DRMImage to persist as long as there is a texture on it
		((LPD2DRMIMAGEPRIVATE) this)->AddRef();
		
		// REVIEW: don't ref count the texture so that we don't get a circular ref
		//	m_pd3drmTexture->AddRef();
	}
	else
	{
		m_pd3drmTexture->AddRef();
	}

	*ppTexture = m_pd3drmTexture;

	return hr;
}


STDMETHODIMP
CDirect2DRMImage::get_NColors(DWORD *pnColors)
{
	if (m_pImageData == NULL)
		return E_NOTINITIALIZED;

	*pnColors = (1 << m_pImageData->GetDepth());
	return S_OK;
}

STDMETHODIMP
CDirect2DRMImage::get_NFrames(DWORD *pnFrames)
{
	if (m_pImageData == NULL)
		return E_NOTINITIALIZED;

	*pnFrames = m_pImageData->GetNFrames();
	return S_OK;
}

STDMETHODIMP
CDirect2DRMImage::GetFrameDelay(DWORD iIndex, DWORD *pnDelay)
{
	if (m_pImageData == NULL)
		return E_NOTINITIALIZED;

	if (iIndex < (DWORD) m_pImageData->GetNFrames())
		*pnDelay = m_pImageData->GetFrameDelay(iIndex);

	return S_OK;
}

STDMETHODIMP
CDirect2DRMImage::put_VisibleImage(DWORD iIndex)
{
	if (m_pImageData == NULL)
		return E_NOTINITIALIZED;

	HRESULT hr;

	if (iIndex < (DWORD) m_pImageData->GetNFrames()) {
		if (m_nVisImage != iIndex) {
			m_nVisImage = iIndex;
			if (m_pddsTexture) {
				RECT Src = {0,0, m_pImageData->GetWidth(),m_pImageData->GetHeight() };
				RECT Dest;
				int dx,dy;
				hr = ::GetSurfaceDimensions(m_pddsTexture, &Dest);
				if ( FAILED( hr ))
					return hr;
				// cache the surface dimensions...
				dx = Dest.right;
				dy = Dest.bottom;
				/*
				if (( m_fMode == flagTEXTURE_BLTNEAREST ) ||
					( m_fMode == flagTEXTURE_BLTNEXT ))
				{
					if ( Src.right > Dest.right ) 
						Src.right = Dest.right;
					else
						Dest.right = Src.right;

					if ( Src.bottom < Dest.bottom )
						Src.bottom = Dest.bottom;
					else
						Dest.bottom = Src.bottom;
				}
				*/
				// No stretching in this mode..., blt the minimum area of source or destination
				if (( m_fMode == flagTEXTURE_BLTNEAREST ) ||
					( m_fMode == flagTEXTURE_BLTNEXT ) ||
					( m_fMode == flagTEXTURE_CENTERBLTNEXT))
				{
					int mindx = min (Src.right, Dest.right);
					Src.right = Dest.right = mindx;
					int mindy = min(Src.bottom, Dest.bottom);
					Src.bottom = Dest.bottom = mindy;
				}
				// center the destination
				if ( m_fMode == flagTEXTURE_CENTERBLTNEXT )
				{
					int xoffset = (dx - Dest.right ) >> 1;
					int yoffset = (dy - Dest.bottom ) >> 1;
					Dest.left += xoffset;
					Dest.right += xoffset;
					Dest.top += yoffset;
					Dest.bottom += yoffset;
				}

				// The Texture may not be the same size as the image.
				if (FAILED(hr = m_pImageData->BltToSurface(iIndex, &Src,m_pddsTexture, &Dest,flagBLT_IGNORETRANSPARENCY)) ||
					(m_pd3drmTexture && FAILED(hr = m_pd3drmTexture->Changed(TRUE, FALSE))))
					return hr;
			}
		}
		m_nTimeLeftOnFrame = m_pImageData->GetFrameDelay(iIndex);
	}

	// REVIEW: just return S_OK even if the index is out of range
	return S_OK;
}

STDMETHODIMP
CDirect2DRMImage::get_VisibleImage(DWORD *piIndex)
{
	*piIndex = m_nVisImage;
	return S_OK;
}

STDMETHODIMP
CDirect2DRMImage::put_Tiled(BOOL bTiled)
{
	if (bTiled)
		m_bvFlags |= flagTILED;
	else
		m_bvFlags &= ~flagTILED;
	return S_OK;
}

STDMETHODIMP
CDirect2DRMImage::get_Tiled(BOOL *pb)
{
	*pb = ((m_bvFlags & flagTILED) != 0);
	return S_OK;
}

STDMETHODIMP
CDirect2DRMImage::get_Transparent(BOOL *pb)
{
	*pb = ((m_bvFlags & flagTRANSPARENT) != 0);
	return S_OK;
}

STDMETHODIMP
CDirect2DRMImage::get_Palettized(BOOL *pb)
{
	*pb = ((m_bvFlags & flagTRANSPARENT) != 0);
	return S_OK;
}

STDMETHODIMP
CDirect2DRMImage::put_Animated(BOOL bAnimated)
{
	if (bAnimated)
	{
		if (m_pImageData == NULL)
			return E_NOTINITIALIZED;

		if (m_pImageData->GetNFrames() > 1)
			m_bvFlags |= flagANIMATED;

		if (m_pd3drmTexture && !m_posTickedList)
			m_posTickedList = m_pD2DRMPriv->InsTicked(this);

		return S_OK;
	}
	if (m_posTickedList)
	{
		m_pD2DRMPriv->DelTicked(m_posTickedList);
		m_posTickedList = NULL;
	}

	m_bvFlags &= ~flagANIMATED;
	return S_OK;
}

STDMETHODIMP
CDirect2DRMImage::get_Animated(BOOL *pbAnimated)
{
	*pbAnimated = ((m_bvFlags & flagANIMATED) != 0);
	return S_OK;
}

STDMETHODIMP
CDirect2DRMImage::AddTileOffset(float x, float y)
{
	float xTile = m_t2dTile.GetXPos();
	float yTile = m_t2dTile.GetYPos();

	return SetTileOffset(xTile + x, yTile + y);
}

#include <math.h>

STDMETHODIMP
CDirect2DRMImage::SetTileOffset(float x, float y)
{
	if (m_pImageData == NULL)
		return E_NOTINITIALIZED;

	float fWidth = (float) m_pImageData->GetWidth();
	float fHeight = (float) m_pImageData->GetHeight();
	x = (float) fmod(x, fWidth);
	y = (float) fmod(y, fHeight);
	if (x < 0)
		x += fWidth;
	if (y < 0)
		y += fHeight;
	m_t2dTile.SetTranslation(x, y);
	return S_OK;
}

STDMETHODIMP
CDirect2DRMImage::SetTileScale(float x, float y)
{
	// REVIEW: should there even be a notion of a tile scale???
	if ((x < 0) || (y < 0))
		return E_INVALIDARG;
	m_t2dTile.SetScale(x, y);
	return S_OK;
}

STDMETHODIMP
CDirect2DRMImage::AddTileScale(float x, float y)
{
	float xTile = m_t2dTile.GetXScale();
	float yTile = m_t2dTile.GetYScale();
	m_t2dTile.SetScale(xTile * x, yTile * y);
	return S_OK;
}

STDMETHODIMP
CDirect2DRMImage::get_Width(DWORD *pdwWidth)
{
	if (m_pImageData == NULL)
		return E_NOTINITIALIZED;

	MMASSERT(pdwWidth);
	*pdwWidth = m_pImageData->GetWidth();
	return S_OK;
}

STDMETHODIMP
CDirect2DRMImage::get_Height(DWORD *pdwHeight)
{
	if (m_pImageData == NULL)
		return E_NOTINITIALIZED;

	MMASSERT(pdwHeight);
	*pdwHeight = m_pImageData->GetHeight();
	return S_OK;
}

STDMETHODIMP
CDirect2DRMImage::get_Depth(DWORD *pdwDepth)
{
	if (m_pImageData == NULL)
		return E_NOTINITIALIZED;

	MMASSERT(pdwDepth);
	*pdwDepth = (DWORD) m_pImageData->GetDepth();
	return S_OK;
}

STDMETHODIMP
CDirect2DRMImage::Tick(DWORD nElapsedTime)
{
	MMASSERT(m_pImageData && (m_bvFlags & flagANIMATED));

	if (nElapsedTime > m_nTimeLeftOnFrame) {
		// find the next visible frame
		DWORD nTmpFrame = m_nVisImage;
		DWORD cFrames = m_pImageData->GetNFrames();
		DWORD nCount = 0;
		do {
			if (++nCount == cFrames)
			{
				// We've cycled through all the frames! Stop here so we don't spend 
				// lots of time on in-frequently ticked images.
				return S_OK;
			}
			nElapsedTime -= m_nTimeLeftOnFrame;
			nTmpFrame++;
			MMASSERT(nTmpFrame <= cFrames);
			if (nTmpFrame == cFrames)
				nTmpFrame = 0;
			m_nTimeLeftOnFrame = m_pImageData->GetFrameDelay(nTmpFrame);
		} while (nElapsedTime > m_nTimeLeftOnFrame);
		put_VisibleImage(nTmpFrame);
	}
	m_nTimeLeftOnFrame -= nElapsedTime;

	return S_OK;
}

inline void
SetRect(LPD2DRECT pr2, float left, float top, float right, float bottom)
{
	pr2->left = left;
	pr2->top = top;
	pr2->right = right;
	pr2->bottom = bottom;
}


STDMETHODIMP
CDirect2DRMImage::GetDisplayList(CDisplayList *pdlScene, D2DTransform *pt2dCurrent)
{
	MMASSERT(m_pImageData && pt2dCurrent && pdlScene);

	LPDISPLAYDESC pDisplayItem = new CDisplayDesc();
	if (pDisplayItem == NULL)
		return E_OUTOFMEMORY;

	SetRect(&(pDisplayItem->m_rSrcExtent), 0, 0, m_pImageData->GetWidth(), m_pImageData->GetHeight());
	pDisplayItem->m_t2d = *pt2dCurrent;
	pdlScene->Ins(pDisplayItem);

	pDisplayItem->m_pImage = (LPDIRECT2DRMIMAGE) this;
	pDisplayItem->m_pImage->AddRef();

	return S_OK;
}

STDMETHODIMP
CDirect2DRMImage::PickVisual(LPD2DPOINT ppt2, LPD2DRMARRAYPRIVATE pAncestory, 
							 LPD2DRMARRAYPRIVATE pPicks)
{
	MMASSERT(m_pImageData && ppt2 && pAncestory && pPicks);
	HRESULT hr;
	DWORD nItems;
	pAncestory->get_Size(&nItems);

	// REVIEW: don't do the transparency pick if the image is tiled, for now
	if (m_pImageData->Pick(m_nVisImage, (long) ppt2->x, (long) ppt2->y, m_bvFlags & flagTILED)) {
		// CoCreate a new pick and add it to the pick array
		LPD2DRMPICKPRIVATE pPick = NULL;
		if (FAILED(hr = CoCreateInstance(
			CLSID_CDirect2DRMPick,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_ID2DRMPickPrivate,
			(LPVOID *) &pPick)))
			return hr;

		hr = pPick->Init(ppt2, (LPD2DRMVISUALPRIVATE) this, pAncestory);
		pPicks->InsElement(d2dotPick, pPick);

		MMRELEASE(pPick);

		return hr;
	}
	return S_OK;
}
