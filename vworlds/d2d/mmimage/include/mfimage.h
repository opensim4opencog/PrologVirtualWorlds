// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _MFImage_h
#define _MFImage_h

// File:	MFImage.h
// Author:	MM    (MM)
//
// Description:
//    This defines the multiframe images stored in the image cache.  The MFImage
//  class is designed to be a run-time object.  The MFImageBuilder is used to
//  construct these objects at author time.
//
// Notes:
//    Once a multiframe image has been created, it should never be possible in
//  the future to change it.  This way, I can make assumptions about the way that
//  D3D texture objects will be interacting with the object.  Perhaps the only
//  reasonable change that can be made will be to change the pixels in the DDS.
//  We can do this, because we can invoke the method that indicates that the
//  texture has changed, if it exists on the frame.  Actually, shuffling the order
//  is probably okay also...we just can't allow the number of frames to go from
//  1 to 2.  Let's just make the restriction that once an object is being used 
//  by the cache, you can't add frames.
// 
//    I'm not sure I really like the MFImageBuilder structure.  It seems like
//  a nicer way to organize things would be to have the MFImage load itself.  Just
//  pass in the load method and the appropriate data, and it will get loaded.
//  That way, if we need to reload, all of the appropriate information is already
//  stored with the image.  Also, this will allow the image more control over
//  progressive and asynchronous download.  It can start by showing some simple
//  form of itself before it finishes downloading, but still provide all of the
//  appropriate services.
//
// History:
// -@- 2/21/97 (MM) created

#ifndef flagBLT_IGNORETRANSPARENCY
#define flagBLT_IGNORETRANSPARENCY 0x10
#define flagBLT_USEDYNAMICMAPPING 0x20
#endif

#ifndef flagTEXTURE_DONOTHING 
#define flagTEXTURE_DONOTHING -1
#define flagTEXTURE_STRETCHNEAREST 0
#define flagTEXTURE_STRETCHNEXT 1
#define flagTEXTURE_BLTNEAREST 2
#define flagTEXTURE_BLTNEXT 3
#define flagTEXTURE_CENTERBLTNEXT 4
#endif


// class: CMFImageFrame
class CMFImageFrame {
	friend class CImageCache;
	friend class CMFImage;
private:
						CMFImageFrame();
						~CMFImageFrame();

public:
	CRLESurface *		pRLE;			// run length encoding of frame
	LPDIRECTDRAWSURFACE pDDS;			// DDS expansion of frame
	LPDIRECT3DRMTEXTURE pTexture;		// cached texture
	DWORD				nDelay;			// frame delay to next image in milliseconds
};

inline
CMFImageFrame::CMFImageFrame()
{
	memset(this, 0, sizeof(*this));
}


class CMFImage {
	friend class CImageCache;
	friend class CMFImageBuilder;
private:
						CMFImage();
						~CMFImage();
	//
	// Construction
	//
	HRESULT				Init(const char *szName, WORD nWidth, WORD nHeight, LPCOLORINFO pci);
	HRESULT				AddFrame(CRLESurface *pRLE, LPDIRECTDRAWSURFACE pDDS, DWORD nDelay);
public:
	//
	// Blt
	//
	HRESULT				BltToSurface(DWORD iFrame, LPRECT prSrc, LPDIRECTDRAWSURFACE pddsDst, 
							LPRECT prDst, DWORD bvFlags) const;
	HRESULT				BltFastToSurface(DWORD iFrame, LPRECT prSrc, LPDIRECTDRAWSURFACE pddsDst, 
							WORD nXPos, WORD nYPos, DWORD bvFlags) const;
	HRESULT				BltTiledToSurface(DWORD iFrame, LPDIRECTDRAWSURFACE pddsDst,
							LPRECT prDst, DWORD fiXOffset, DWORD fiYOffset,
							DWORD fiXScale, DWORD fiYScale, DWORD bvFlags) const;
	HRESULT				BltFastTiledToSurface(DWORD iFrame, LPDIRECTDRAWSURFACE pddsDst,
							LPRECT prDst, DWORD fiXOffset, DWORD fiYOffset, DWORD bvFlags) const;

	HRESULT				GetD3DRMTexture(DWORD iFrame, LPDIRECTDRAW pDD, LPDIRECT3DRM pD3DRM, 
							LPDIRECT3DRMTEXTURE *ppTexture);

	HRESULT				GetD3DRMTextureEx(DWORD iFrame, long fMode, LPDIRECTDRAW pDD, LPDIRECT3DRM pD3DRM, 
							LPDIRECT3DRMTEXTURE *ppTexture);

	HRESULT				CreateFittedSurface(LPDIRECTDRAW pDD, LPDIRECTDRAWSURFACE *ppdds) const;
	HRESULT				CreateFittedSurfaceEx(int dx, int dy, LPDIRECTDRAW pDD, LPDIRECTDRAWSURFACE *ppdds, VARIANT_BOOL bForTexture = VARIANT_FALSE) const;


	HRESULT				ConvertToRLE();
	HRESULT				ConvertToDDS(LPDIRECTDRAW pDD = NULL);

	HRESULT				CalculateTextureSize( int dx, int dy, long fMode, int &tdx, int &tdy );

	//
	// Data Access
	//
	WORD				GetWidth() const	{ return m_nWidth; }
	WORD				GetHeight() const	{ return m_nHeight; }
	BYTE				GetDepth() const	{ return (m_pciGlobal ? m_pciGlobal->pixi.nBPP : 0); }
	DWORD				GetNFrames() const	{ return m_dspFrames.Items(); }
	DWORD				GetMemSize() const	{ return m_nSize; }
	const char *		GetName() const		{ return m_szName; }
	BOOL				IsTransparent() const;
	BOOL				IsPalettized() const;
	BOOL				IsInside(long nX, long nY);
	DWORD				GetFrameDelay(DWORD iFrame) const;

	BOOL				Pick(DWORD iFrame, long nX, long nY, BOOL bIgnoreTransparency);
	DWORD				GetPixel(DWORD iFrame, long nX, long nY);

public:
	DWORD				AddRef();
	DWORD				Release();

private:
	void				SetImageCache(CImageCache *pImageCache);
	void				SetDD(LPDIRECTDRAW pDD) {if (pDD) {m_pDD = pDD; pDD->AddRef();};};
	HRESULT				CreateDDSOnFrame(LPDIRECTDRAW pDD, DWORD iFrame);
	HRESULT				BltSetup(DWORD iFrame, DWORD bvFlags, const CMFImageFrame **ppFrame, 
							DWORD *pdwBltFlags, DWORD *pdwFastBltFlags) const;

	HRESULT				CreateD3DRMTexture(LPDIRECTDRAWSURFACE pDDS, long fMode, LPDIRECTDRAW pDD, LPDIRECT3DRM pD3DRM, 
							LPDIRECT3DRMTEXTURE *ppTexture);

private:
	DWORD				m_nRef;

	CImageCache *		m_pImageCache;		// back pointer
	ImgCacheCookie		m_icc;

	WORD				m_nWidth, m_nHeight;
	LPCOLORINFO			m_pciGlobal;
	DWORD				m_nSize;			// memory consumed by image related data, in bytes
	char *				m_szName;
	LPDIRECTDRAW		m_pDD;				// may need to create surfaces dynamically

	DSet<CMFImageFrame *>	m_dspFrames;	// list of defined frames
};


inline DWORD
CMFImage::AddRef()
{
	++m_nRef;

	if ((m_nRef == 2) && m_pImageCache)
		m_pImageCache->Activate(this);
	
	return m_nRef;
}

inline DWORD
CMFImage::Release()
{
	--m_nRef;

	if (m_nRef == 0) {
		delete this;
		return 0;
	} else if ((m_nRef == 1) && m_pImageCache)
		m_pImageCache->InActivate(this);

	return m_nRef;
}

inline BOOL
CMFImage::IsTransparent() const
{
	MMASSERT(m_pciGlobal);
	return (m_pciGlobal->bvColorFlags & flagTRANSPARENT);
}

inline BOOL
CMFImage::IsPalettized() const
{
	MMASSERT(m_pciGlobal);
	return (m_pciGlobal->bvColorFlags & flagPALETTIZED);
}

inline BOOL 
CMFImage::IsInside(long nX, long nY)
{
	return ((nX >= 0) && (nX < m_nWidth) && (nY >= 0) && (nY < m_nHeight));
}

inline DWORD
CMFImage::GetFrameDelay(DWORD iFrame) const
{
	const CMFImageFrame *pFrame = m_dspFrames[iFrame];
	MMASSERT(pFrame != NULL);
	return pFrame->nDelay;
}

#endif
