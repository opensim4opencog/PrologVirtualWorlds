// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWRendRt.h : Declaration of the CVWRenderRoot

#include <d3drmwin.h>
#include <resource.h>       // main symbols
#include <propbase.h>

#include <atlctl.h>

EXTERN_C const IID LIBID_VWMMLib;
EXTERN_C const CLSID CLSID_VWRenderRoot;

/////////////////////////////////////////////////////////////////////////////
// VWRender

class ATL_NO_VTABLE CVWRenderRoot :
	public IDispatchImpl<IVWRenderRoot, &IID_IVWRenderRoot, &LIBID_VWMMLib>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVWRenderRoot, &CLSID_VWRenderRoot>,
	public ISupportErrorInfoImpl<&IID_IVWRender3D>,
	public IObjectSafetyImpl<CVWRenderRoot, INTERFACESAFE_FOR_UNTRUSTED_CALLER>
{
	DECLARE_AGGREGATABLE(CVWRenderRoot)

public:
	CVWRenderRoot();
	~CVWRenderRoot();
BEGIN_COM_MAP(CVWRenderRoot)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWRender)
	COM_INTERFACE_ENTRY(IVWRenderRoot)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(CVWRenderRoot) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

	DECLARE_REGISTRY(CVWRenderRoot, _T("VWSYSTEM.RenderRoot.1"), _T("VWSYSTEM.RenderRoot"), IDS_VWRENDERROOT_DESC, THREADFLAGS_BOTH)

// IVWRender
	STDMETHOD(HandleActivate)(WPARAM wParam);
	STDMETHOD(HandleSize)(int cx, int cy);
	STDMETHOD(HandlePaint)(HDC hDC);
	STDMETHOD(Render)();
	STDMETHOD(Update)();
	STDMETHOD(HitTest)(VARIANT varHitParams, VARIANT_BOOL vbReturnAllPicked, VARIANT *pvarHitResult);
	STDMETHOD(get_FastMode)(VARIANT_BOOL* bFastMode);
	STDMETHOD(put_FastMode)(VARIANT_BOOL bFastMode);
	STDMETHOD(put_FastModeEnabled)(VARIANT_BOOL bEnabled);
	STDMETHOD(get_FastModeEnabled)(VARIANT_BOOL *pbEnabled);
	STDMETHOD(put_UseHWAcceleration)(VARIANT_BOOL bUseHWAcceleration);
	STDMETHOD(get_UseHWAcceleration)(VARIANT_BOOL *pbUseHWAcceleration);
	STDMETHOD(put_UseBackBuffer)(VARIANT_BOOL bUseBackBuffer);
	STDMETHOD(get_UseBackBuffer)(VARIANT_BOOL *pbBackBuffer);
	STDMETHOD(put_ForceBackBuffer)(VARIANT_BOOL bForceBackBuffer);
	STDMETHOD(get_ForceBackBuffer)(VARIANT_BOOL *pbForceBuffer);

// IVWRenderRoot
	STDMETHOD(Init)(HWND hWnd, LPDIRECTDRAW pdd, LPDIRECT3DRM pd3drm, LPDIRECT2DRM pd2drm);
	STDMETHOD(get_Scene)(IVWFrame **ppvwfScene);
	STDMETHOD(put_Scene)(IVWFrame *pvwfScene);
	STDMETHOD(SetBackgroundColor)(BYTE r, BYTE g, BYTE b);
	STDMETHOD(get_VWRender3D)(IVWRender3D **ppVWRender3D);
	STDMETHOD(get_VWRender2D)(IVWRender2D **ppVWRender2D);
	STDMETHOD(get_Camera)(IVWFrame **ppvwfCamera);
	STDMETHOD(put_ColorModel)(int d3drmColorModel);

	STDMETHOD(get_OverlayEnabled)(VARIANT_BOOL* pbEnabled)
	{
		ASSERT( pbEnabled );
		*pbEnabled = m_bOverlayEnabled;
		return S_OK;
	}
	STDMETHOD(put_OverlayEnabled)(VARIANT_BOOL bEnabled)
	{
		m_bOverlayEnabled = bEnabled;
		return S_OK;
	}

	STDMETHOD(get_FPSecond)(VARIANT_BOOL* pbEnabled)
	{
		*pbEnabled = m_bFPSEnabled;
		return S_OK;
	}
	STDMETHOD(put_FPSecond)(VARIANT_BOOL bEnabled)
	{
		m_bFPSEnabled = bEnabled;
		return S_OK;
	}
	STDMETHOD(GetBackBuffer)(LPDIRECTDRAWSURFACE* ppddsBackBuffer)
	{
		ASSERT(ppddsBackBuffer != NULL);
		*ppddsBackBuffer = m_pddsBackBuffer;
		SAFEADDREF(*ppddsBackBuffer);
		return S_OK;
	}
	STDMETHOD(RebuildDevice)();


private:
	HRESULT HandleSizeInternal(int cx, int cy, VARIANT_BOOL bUseBackBuffer, VARIANT_BOOL bRebuild3DDevice);
	HRESULT CreateBackBuffer(LPDIRECTDRAW pdd, int cx, int cy, VARIANT_BOOL bUseHWAcceleration, IDirectDrawSurface **ppddsBackBuffer);
	HRESULT	ReleaseAll();
//	HRESULT	Blt();

private:	
	// Local helper data.
	IVWRender3D			*m_pVWRender3D;
	IVWRender2D			*m_pVWRender2D;
	IVWFrame			*m_pvwfScene;
	IVWFrame			*m_pvwfCamera;
	IDirect3DRMFrame	*m_pd3drmfCamera;
	IDirect2DRMFrame	*m_pd2drmfCamera;
	HWND				m_hWnd;
	LPDIRECTDRAW		m_pDD;
	IDirect2DRM			*m_pD2DRM;
	LPDIRECTDRAWPALETTE m_pPalette;
	LPDIRECTDRAWSURFACE m_pddsFrontBuffer;
	LPDIRECTDRAWSURFACE m_pddsBackBuffer;
	VARIANT_BOOL		m_bUseHWAcceleration;
	VARIANT_BOOL		m_bFastMode;
	VARIANT_BOOL		m_bFastModeEnabled;
	VARIANT_BOOL		m_bUseBackBuffer;
	VARIANT_BOOL		m_bForceBackBuffer;
	unsigned long		m_d3dColorModel;
	int					m_nRGB;
	LPDIRECTDRAWCLIPPER m_lpDDClipper;
	CRITICAL_SECTION	m_CS;
	VARIANT_BOOL		m_bOverlayEnabled;
	VARIANT_BOOL		m_bFPSEnabled;
	int					m_iCurrentCX;
	int					m_iCurrentCY;
	COLORREF			m_crBackgroundColor;
};
