// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// D2DRMImg.h : Declaration of the CDirect2DRMImage

// Notes:
//    For the sake of simplicity, all frames of a D2DImage are the same size
//  and share the following attributes: trasparency status & color, tiled 
//  status, tile transformation, and palette.

#include "resource.h"       // main symbols
#include "d2dbase.h"

/////////////////////////////////////////////////////////////////////////////
// d2d

// Bit flags associated with the image
#define flagTILED		0x100
#define flagANIMATED	0x200
#define flagVALIDBLTMAP	0x400

typedef struct ImageFrame {
	LPDIRECTDRAWSURFACE pdds;
} ImageFrame;

class ATL_NO_VTABLE CDirect2DRMImage : 
	public CComDualImpl<ID2DRMImagePrivate, &IID_IDirect2DRMImage, &LIBID_D2DLib>,
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<CDirect2DRMImage, &CLSID_CDirect2DRMImage>,
	public ID2DRMVisualPrivate
{
public:
	CDirect2DRMImage();
	~CDirect2DRMImage();

	BEGIN_COM_MAP(CDirect2DRMImage)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IDirect2DRMObject)
		COM_INTERFACE_ENTRY(IDirect2DRMVisual)
		COM_INTERFACE_ENTRY(IDirect2DRMImage)
		COM_INTERFACE_ENTRY(ID2DRMImagePrivate)
		COM_INTERFACE_ENTRY(ID2DRMVisualPrivate)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
	END_COM_MAP()

	D2DOBJECT_BASE_CLASS(Image)
	
	D2DOBJREG_DECL(Image, D2DRMIMG);
	
public:
	//
	// IDirect2DRMImage Interface
	//
	STDMETHOD(BltToSurface)(LPD2DRECT pSrcRect, LPDIRECTDRAWSURFACE pddsDst, 
				LPD2DRECT pDstRect, DWORD dwFlags);
	STDMETHOD(GetD3DTexture)(LPDIRECT3DRM pD3DRM, LPDIRECT3DRMTEXTURE *ppTexture);
	STDMETHOD(ExportD3DTexture)(LPDIRECT3DRM pD3DRM, LPDIRECTDRAWSURFACE *ppddsTexture, 
				LPDIRECT3DRMTEXTURE *ppTexture);
	STDMETHOD(ExportD3DTextureEx)(LPDIRECT3DRM pD3DRM, LPDIRECTDRAWSURFACE *ppddsTexture,long fMode, 
								   LPDIRECT3DRMTEXTURE *ppTexture);
	STDMETHOD(CreateFittedSurface)(LPDIRECTDRAWSURFACE *ppdds);

	STDMETHOD(get_NColors)(DWORD *pnColors);
	STDMETHOD(get_NFrames)(DWORD *pnFrames);
	STDMETHOD(GetFrameDelay)(DWORD nIndex, DWORD *pnDelay);

	STDMETHOD(put_VisibleImage)(DWORD nIndex);
	STDMETHOD(get_VisibleImage)(DWORD *pnIndex);

	STDMETHOD(put_Tiled)(BOOL bTiled);
	STDMETHOD(get_Tiled)(BOOL *pbTiled);
	STDMETHOD(get_Transparent)(BOOL *pbTransparent);
	STDMETHOD(get_Palettized)(BOOL *pbPaletted);
	STDMETHOD(put_Animated)(BOOL bAnimated);
	STDMETHOD(get_Animated)(BOOL *pbAnimated);

	STDMETHOD(get_Width)(DWORD *pnWidth);
	STDMETHOD(get_Height)(DWORD *pnHeight);
	STDMETHOD(get_Depth)(DWORD *pnBPP);

	STDMETHOD(SetTileOffset)(float x, float y);
	STDMETHOD(AddTileOffset)(float x, float y);
	STDMETHOD(SetTileScale)(float x, float y);
	STDMETHOD(AddTileScale)(float x, float y);

	//
	// ID2DRMImagePrivate Interface
	//
	STDMETHOD(InitFromMFImage)(LPD2DRMPRIVATE pD2DRMPriv, CMFImage *pmfi, DWORD dwFlags);
	STDMETHOD(Tick)(DWORD nElapsedTime);

	//
	// ID2DRMVisualPrivate Interface
	//
	STDMETHOD(GetDisplayList)(CDisplayList *pdlScene, D2DTransform *pt2dCurrent);
	STDMETHOD(PickVisual)(LPD2DPOINT ppt, LPD2DRMARRAYPRIVATE pAncestory, 
					LPD2DRMARRAYPRIVATE pPicks);

	friend void _D2DRMImageTextureDestroyCallback(LPDIRECT3DRMOBJECT pObject, void *pvArg);

private:
	DWORD				m_nTimeLastTicked;
	DWORD				m_nVisImage;
	DWORD				m_bvFlags;
	CMFImage *			m_pImageData;
	DWORD				m_nTimeLeftOnFrame;
	long				m_fMode;

	LPD2DRMPRIVATE		m_pD2DRMPriv;
	POSITION			m_posTickedList;

	// local texture for multiframe images
	LPDIRECTDRAWSURFACE m_pddsTexture;
	LPDIRECT3DRMTEXTURE	m_pd3drmTexture;

	// tile transformation data
	D2DTransform		m_t2dTile;
};
