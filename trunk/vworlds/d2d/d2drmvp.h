// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// D2DRMVP.h : Declaration of the CDirect2DRMViewPort


#include "resource.h"       // main symbols
#include "d2dbase.h"

/////////////////////////////////////////////////////////////////////////////
// d2d

class ATL_NO_VTABLE CDirect2DRMViewport :
	public CComDualImpl<ID2DRMViewportPrivate, &IID_IDirect2DRMViewport, &LIBID_D2DLib>,
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<CDirect2DRMViewport, &CLSID_CDirect2DRMViewport>
{
public:
	CDirect2DRMViewport();
	~CDirect2DRMViewport();

	BEGIN_COM_MAP(CDirect2DRMViewport)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IDirect2DRMObject)
		COM_INTERFACE_ENTRY(IDirect2DRMViewport)
		COM_INTERFACE_ENTRY(ID2DRMViewportPrivate)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
	END_COM_MAP()
	
	D2DOBJECT_BASE_CLASS(Viewport)

	D2DOBJREG_DECL(Viewport, D2DRMVP);
	
public:
	//
	// IDirect2DRMViewport Interface
	//
	STDMETHOD(Init)(
		LPDIRECT2DRMDEVICE pDevice,
		LPDIRECT2DRMFRAME pCamera,
		LPRECT prDimensions);
//		DWORD dwXPos,
//		DWORD dwYPos, 
//		DWORD dwWidth,
//		DWORD dwHeight);

	STDMETHOD(Configure)(
		LPRECT prDimensions);
//		DWORD dwXPos,
//		DWORD dwYPos, 
//		DWORD dwWidth,
//		DWORD dwHeight);

	STDMETHOD(SetBackgroundImage)(LPDIRECT2DRMIMAGE pd2diBackground);
	STDMETHOD(SetBackgroundColor)(BYTE r, BYTE g, BYTE b);

	STDMETHOD(get_Camera)(LPDIRECT2DRMFRAME *ppCamera);
	STDMETHOD(put_Camera)(LPDIRECT2DRMFRAME pCamera);
	STDMETHOD(get_Device)(LPDIRECT2DRMDEVICE *ppDevice);

	// Scene to Screen mapping
	STDMETHOD(get_XOffset)(DWORD *pnXOffset);
	STDMETHOD(get_YOffset)(DWORD *pnYOffset);
	STDMETHOD(get_Width)(DWORD *pnWidth);
	STDMETHOD(get_Height)(DWORD *pnHeight);

	STDMETHOD(Render)(LPDIRECT2DRMFRAME pScene, DWORD dwFlags);
	STDMETHOD(Pick)(LPDIRECT2DRMFRAME pScene, DWORD nX, DWORD nY, LPDIRECT2DRMARRAY *prgPicks);
	STDMETHOD(MovePicked)(float dX, float dY, LPDIRECT2DRMPICK pPick);

	STDMETHOD(Clear)(DWORD dwFlags);
//	STDMETHOD(Clear)(LPDIRECT2DRMIMAGE pd2diBackground, DWORD dwFillColor, DWORD dwFlags);
//	STDMETHOD(ClearToColor)(DWORD dwFillColor);
//	STDMETHOD(ClearToImage)(LPDIRECT2DRMIMAGE pd2diBackground, DWORD dwFlags);

	//
	// ID2DRMViewportPrivate Interface
	//
	STDMETHOD(UnsetParentDevice)();
	STDMETHOD(DeviceResized)();

private:
	LPD2DRMDEVICEPRIVATE	m_pParentDevice;
	LPD2DRMFRAMEPRIVATE		m_pCameraFrame;
	RECT					m_rDimensions;			// size in pixels
	BOOL					m_bAutoSize;			// automatically size to full device

	// background for clear
	LPDIRECT2DRMIMAGE		m_pd2diBackground;
	DWORD					m_dwBackgroundColor;
};
