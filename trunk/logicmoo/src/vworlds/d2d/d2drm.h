// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// D2DRM.h : Declaration of the CDirect2DRM
#include "resource.h"       // main symbols
#include "d2dbase.h"
#include "imgcache.h"

#include <atlctl.h>

/////////////////////////////////////////////////////////////////////////////
// d2d


class ATL_NO_VTABLE CDirect2DRM :
	public CComDualImpl<ID2DRMPrivate, &IID_IDirect2DRM, &LIBID_D2DLib>,
	public ISupportErrorInfo,
	public IObjectSafetyImpl<CDirect2DRM, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
	public CComObjectRoot,
	public CComCoClass<CDirect2DRM, &CLSID_CDirect2DRM>
{
public:
	CDirect2DRM();
	~CDirect2DRM();

	BEGIN_COM_MAP(CDirect2DRM)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IDirect2DRM)
		COM_INTERFACE_ENTRY(ID2DRMPrivate)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IObjectSafety)
	END_COM_MAP()

	// ISupportsErrorInfo
	D2DIERRORINFO_DECL(Direct2DRM);

	D2DREG_DECL(Direct2DRM, D2DRM);

public:
	//
	// IDirect2DRM Interface
	//
	STDMETHOD(Init)(
		LPDIRECTDRAW pDD);

	STDMETHOD(CreateDeviceFromSurface)(
		LPGUID pGUID, 
		LPDIRECTDRAWSURFACE pddsBackBuffer,
		LPDIRECT2DRMDEVICE *ppDevice);

	STDMETHOD(CreateViewport)(
		LPDIRECT2DRMDEVICE pDevice,
		LPDIRECT2DRMFRAME pCamera,
		LPRECT prDimensions,
//		DWORD dwXPos,
//		DWORD dwYPos, 
//		DWORD dwWidth,
//		DWORD dwHeight,
		LPDIRECT2DRMVIEWPORT *ppViewport);

	STDMETHOD(CreateImageFromSurface)(
		LPDIRECTDRAWSURFACE pdds,
 		BSTR bstrFilename,
		LPDIRECT2DRMIMAGE *ppImage);

	STDMETHOD(CreateImageFromImageFile)(
 		BSTR bstrFilename,
		DWORD dwFlags,
		LPDIRECT2DRMIMAGE *ppImage);

	STDMETHOD(CreateImageFromImageFileExt)(
 		BSTR bstrFilename,
		DWORD dwFlags,
		LPDIRECT2DRMIMAGE *ppImage);

	STDMETHOD(GetNamedImage)(
 		BSTR bstrName,
		LPDIRECT2DRMIMAGE *ppImage);

	STDMETHOD(ReleaseImage)(
		BSTR bstrName);

	STDMETHOD(CreateImageFromText)(
 		BSTR bstrText,
		DWORD nPointHeight,
		BOOL bShadowed,
		LPDIRECT2DRMIMAGE *ppImage);

	STDMETHOD(CreateImageFromTextExt)(
		BSTR bstrText,
		IFont *pFont,
		OLE_COLOR ocFontColor,
		BOOL bShadowed,
		OLE_COLOR ocShadowColor,
		LPDIRECT2DRMIMAGE *ppImage);

	STDMETHOD(CreateImageFromWrappedTextExt)(
		BSTR bstrText,
		IFont *pFont,
		BOOL bShadowed,
		DWORD width, 
		DWORD height,
		DWORD nPoint,
		LPDIRECT2DRMIMAGE *ppImage);

	STDMETHOD(CreateFrame)(
		LPDIRECT2DRMFRAME pParentFrame,
		LPDIRECT2DRMFRAME *ppFrame);

	STDMETHOD(SetVideoMode)(
		LPDDPIXELFORMAT pddpf,
		LPDIRECTDRAWPALETTE pddp);

	STDMETHOD(Tick)(
		DWORD nElapsedTime);

	// REVIEW: image manager stuff - for now
	STDMETHOD(SetImageCacheHardLimit)(
		DWORD nSizeInBytes);

	STDMETHOD(AddTextureForRelease)(LPDIRECT3DRMTEXTURE pTexture);

	STDMETHOD(ReleaseTextures)();

	//
	// ID2DRMPrivate Interface
	//
	STDMETHOD_(POSITION, InsTicked)(LPD2DRMIMAGEPRIVATE pd2diPriv);
	STDMETHOD_(void, DelTicked)(POSITION pos);

private:
	IFont				*GetDefaultFont();
	void				PurgeDeleteList();

	LPDIRECTDRAW		m_pDD;
	CImageCache *		m_pImageCache;
	CPtrList			m_listTicked;
	CPtrList			m_listTextures;

	// HFONT				m_hDefFont;
	IFont				*m_pDefFont;

	CRITICAL_SECTION	m_CSTickedList;
};
