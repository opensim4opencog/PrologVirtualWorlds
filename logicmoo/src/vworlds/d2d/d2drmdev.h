// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// D2DRMDev.h : Declaration of the CDirect2DRMDevice

#include "resource.h"       // main symbols
#include "d2dbase.h"
#include "DSet.h"

/////////////////////////////////////////////////////////////////////////////
// d2d

class ATL_NO_VTABLE CDirect2DRMDevice :
	public CComDualImpl<ID2DRMDevicePrivate, &IID_IDirect2DRMDevice, &LIBID_D2DLib>,
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<CDirect2DRMDevice, &CLSID_CDirect2DRMDevice>
{
public:
	CDirect2DRMDevice();
	~CDirect2DRMDevice();

	BEGIN_COM_MAP(CDirect2DRMDevice)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IDirect2DRMObject)
		COM_INTERFACE_ENTRY(IDirect2DRMDevice)
		COM_INTERFACE_ENTRY(ID2DRMDevicePrivate)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
	END_COM_MAP()

	D2DOBJECT_BASE_CLASS(Device)
	
	D2DOBJREG_DECL(Device, D2DRMDEV);

public:
	//
	// IDirect2DRMDevice Interface
	//

	// initialization
	STDMETHOD(InitFromSurface)(LPDIRECTDRAWSURFACE pdds);

	// buffer counts
	STDMETHOD(get_BufferCount)(DWORD *pnCount);
	STDMETHOD(put_BufferCount)(DWORD nCount);

	// Notifications 
	STDMETHOD(Update)(void);
	STDMETHOD(AddUpdateCallback)(void);
	STDMETHOD(DeleteUpdateCallback)(void);

	// Rendering Quality 
	STDMETHOD(get_Sampling)(WORD *pwSampling);
	STDMETHOD(put_Sampling)(WORD wSampling);

	STDMETHOD(get_Dithering)(WORD *pwDither);
	STDMETHOD(put_Dithering)(WORD wDither);
	
	// Miscellaneous Access
	STDMETHOD(get_ColorModel)(WORD *pwColorModel);
	STDMETHOD(get_Viewports)(LPDIRECT2DRMARRAY *ppViewportArray);
	STDMETHOD(GetViewport)(DWORD iViewport, LPDIRECT2DRMVIEWPORT *ppViewport);

	// Get Dimensions
	STDMETHOD(get_Height)(DWORD *pnHeight);
	STDMETHOD(get_Width)(DWORD *pnWidth);

	//
	// ID2DRMDevicePrivate Interface
	//
	STDMETHOD_(LPDIRECTDRAWSURFACE, GetBackBuffer)(void);
	STDMETHOD(AddViewport)(LPDIRECT2DRMVIEWPORT pd2drmViewport);
	STDMETHOD_(DWORD, GetDepth)(void);
	STDMETHOD_(DWORD, GetColor)(BYTE r, BYTE g, BYTE b);

private:
	LPDIRECTDRAWSURFACE				m_pddsBackBuffer;
	DDSURFACEDESC					m_ddsd;				// surface descriptor for the render surface
	CPixelInfo						m_pixi;
	DWORD							m_nBufferCount;		// number of surfaces in framebuffer
	DSet<LPD2DRMVIEWPORTPRIVATE>	m_dsViewports;
};
