// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWRend2D.h : Declaration of the CVWRender2D

#include <resource.h>       // main symbols
#include <propbase.h>

#include <atlctl.h>

EXTERN_C const IID LIBID_VWMMLib;
EXTERN_C const CLSID CLSID_VWRender2D;

/////////////////////////////////////////////////////////////////////////////
// VWRender

class ATL_NO_VTABLE CVWRender2D :
	public IDispatchImpl<IVWRender2D, &IID_IVWRender2D, &LIBID_VWMMLib>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVWRender2D, &CLSID_VWRender2D>,
	public ISupportErrorInfoImpl<&IID_IVWRender2D>,
	public IObjectSafetyImpl<CVWRender2D, INTERFACESAFE_FOR_UNTRUSTED_CALLER>
{
	DECLARE_AGGREGATABLE(CVWRender2D)

public:
	CVWRender2D();
	~CVWRender2D();

BEGIN_COM_MAP(CVWRender2D)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWRender)
	COM_INTERFACE_ENTRY(IVWRender2D)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

//DECLARE_NOT_AGGREGATABLE(CVWRender2D) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

	DECLARE_REGISTRY(CVWRender2D, _T("VWSYSTEM.Render2D.1"), _T("VWSYSTEM.Render2D"), IDS_VWRENDER2D_DESC, THREADFLAGS_BOTH)

// IVWRender
	STDMETHOD(HandleActivate)(WPARAM wParam);
	STDMETHOD(HandleSize)(LPDIRECTDRAWSURFACE pddsBackBuffer);
	STDMETHOD(HandlePaint)(HDC hDC);
	STDMETHOD(Render)();
	STDMETHOD(HitTest)(VARIANT varHitParams, VARIANT_BOOL vbReturnAllPicked, VARIANT *pvarHitResult);

// IVWRender2D
	STDMETHOD(SetFastMode)(VARIANT_BOOL bFastMode);
	STDMETHOD(Init)(HWND hWnd, LPDIRECT2DRM pd2drm);
	STDMETHOD(get_Scene)(LPDIRECT2DRMFRAME *ppd2drmf);
	STDMETHOD(put_Scene)(LPDIRECT2DRMFRAME pd2drmf);
	STDMETHOD(get_Camera)(LPDIRECT2DRMFRAME *ppd2drm);
	STDMETHOD(get_Direct2DRM)(LPDIRECT2DRM *ppd2drm);
	STDMETHOD(SetBackgroundColor)(BYTE r, BYTE g, BYTE b);
	STDMETHOD(put_BackgroundImage)(LPDIRECT2DRMIMAGE pImage);
	STDMETHOD(Clear());
	STDMETHOD(ReleaseDevice)();

private:
	HRESULT				ReleaseAll();
//	HRESULT				ReleaseSizeBasedObjects();
	HRESULT				InitSizeBasedObjects(LPDIRECTDRAWSURFACE pddsBackBuffer);
	HRESULT				InitScene();

private:
	HWND					m_hWnd;
	LPDIRECT2DRM			m_pd2drm;
	LPDIRECT2DRMDEVICE		m_pd2drmDevice;
//	LPDIRECT2DRMWINDEVICE	m_pd2drmWinDevice;
	LPDIRECT2DRMDEVICE		m_pd2drmWinDevice;
	LPDIRECT2DRMVIEWPORT	m_pd2drmViewport;
	LPDIRECT2DRMFRAME		m_pfCamera;
	LPDIRECT2DRMFRAME		m_pfScene;
	LPDIRECT2DRMFRAME		m_pfEmptyScene;
	LPDIRECT2DRMFRAME		m_pfEmptySceneCamera;
	BOOL					m_bMoveScene;

	static CPtrList			m_list2DRenderers;
};
