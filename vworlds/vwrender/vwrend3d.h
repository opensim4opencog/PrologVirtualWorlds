// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWRend3D.h : Declaration of the CVWRender3D

#include <resource.h>       // main symbols
#include <propbase.h>

#include <atlctl.h>

EXTERN_C const IID LIBID_VWMMLib;
EXTERN_C const CLSID CLSID_VWRender3D;

typedef struct _D3DDeviceInfo
{
	BOOL		  bFindHWDevice;
	D3DCOLORMODEL cm;
	LPGUID        lpHWGuid;
	D3DDEVICEDESC HWDeviceDesc;
	LPGUID        lpSWGuid;
	D3DDEVICEDESC SWDeviceDesc;
} D3DDeviceInfo;

/////////////////////////////////////////////////////////////////////////////
// VWRender

class ATL_NO_VTABLE CVWRender3D :
	public IDispatchImpl<IVWRender3D, &IID_IVWRender3D, &LIBID_VWMMLib>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVWRender3D, &CLSID_VWRender3D>,
	public ISupportErrorInfoImpl<&IID_IVWRender3D>,
	public IObjectSafetyImpl<CVWRender3D, INTERFACESAFE_FOR_UNTRUSTED_CALLER>
{
	DECLARE_AGGREGATABLE(CVWRender3D)

public:
	CVWRender3D();
	~CVWRender3D();

BEGIN_COM_MAP(CVWRender3D)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWRender)
	COM_INTERFACE_ENTRY(IVWRender3D)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(CVWRender3D) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

	DECLARE_REGISTRY(CVWRender3D, _T("VWSYSTEM.Render3D.1"), _T("VWSYSTEM.Render3D"), IDS_VWRENDER3D_DESC, THREADFLAGS_BOTH)

// IVWRender
	STDMETHOD(HandleActivate)(WPARAM wParam);
	STDMETHOD(HandleSize)(LPDIRECTDRAW pdd, LPDIRECTDRAWSURFACE pddsBackBuffer, LPDIRECTDRAWCLIPPER pddClipper, int cx, int cy, DWORD dwDeviceType);
	STDMETHOD(HandlePaint)(HDC hDC);
	STDMETHOD(Render)();
	STDMETHOD(HitTest)(VARIANT varHitParams, VARIANT_BOOL vbReturnAllPicked, VARIANT *pvarHitResult);

// IVWRender3D
	STDMETHOD(SetFastMode)(VARIANT_BOOL bFastMode);
//	STDMETHOD(Init)(HWND hWnd, LPDIRECTDRAW pdd, LPDIRECT3DRM pd3drm, LPDIRECTDRAWSURFACE pddsBackBuffer);
	STDMETHOD(Init)(HWND hWnd, LPDIRECTDRAW pdd, LPDIRECT3DRM pd3drm);
	STDMETHOD(get_Scene)(LPDIRECT3DRMFRAME *ppd3drmf);
	STDMETHOD(put_Scene)(LPDIRECT3DRMFRAME pd3drmf);
	STDMETHOD(get_Direct3DRM)(LPDIRECT3DRM *ppd3drm);
	STDMETHOD(get_Camera)(LPDIRECT3DRMFRAME *ppd3drm);
	STDMETHOD(get_Viewport)(LPDIRECT3DRMVIEWPORT *ppd3drm);
	STDMETHOD(SetBackgroundColor)(BYTE r, BYTE g, BYTE b);
	STDMETHOD(Clear());
	STDMETHOD(Update)();
	STDMETHOD(ReleaseDevice)();
	STDMETHOD(put_ColorModel)(int d3drmColorModel);
	STDMETHOD(get_ColorModel)(int *pd3drmColorModel);
	STDMETHOD(get_DeviceWidth)(DWORD *pdwDeviceWidth);
	STDMETHOD(get_DeviceHeight)(DWORD *pdwDeviceHeight);

	void CalculateFPS(DWORD dw2DClearTime, DWORD dw3DRenderTime, DWORD dw2DRenderTime, DWORD dwBltTime);

private:
	HRESULT BuildDevice(LPDIRECTDRAW lpdd, LPDIRECTDRAWSURFACE pddsBackBuffer, LPDIRECTDRAWCLIPPER pddClipper, int cx, int cy, DWORD dwDeviceType);
	HRESULT CreateViewport(int cx, int cy);
	LPGUID	FindDevice(LPDIRECTDRAW lpdd);
	HRESULT CreateDevice(LPDIRECTDRAW lpdd, LPDIRECTDRAWSURFACE lpDDSurfaceBack, LPDIRECTDRAWCLIPPER pddClipper, int cx, int cy, DWORD dwDeviceType, IDirect3DRMDevice **pprlDevice);
	HRESULT InitializeScene(LPDIRECTDRAW pdd);
	HRESULT ReleaseAll();
	HRESULT CreateLogoVisual(LPDIRECTDRAW pdd, LPDIRECT3DRMVISUAL *ppVisual);
	HRESULT ApplyBitmapResourceToFace(LPDIRECTDRAW pdd, UINT nIDResource, LPDIRECT3DRMFACE pFace);
	HRESULT CreateTextureFromBitmap(LPDIRECTDRAW pdd, CBitmap &bmp,  LPDIRECT3DRMTEXTURE *ppTexture);
	HRESULT CreateOffScreenSurface(LPDIRECTDRAW pdd, LPDIRECTDRAWSURFACE *ppddsOffScreen, DWORD dwWidth, DWORD dwHeight);
	HRESULT CreatePalette(LPDIRECTDRAW pdd);
	IDispatch* GetVWFrame(IDirect3DRMFrameArray	*pfa);

private:

	D3DDeviceInfo   m_DeviceInfo;

	HWND					m_hWnd;
	IDirect3DRM				*m_prlapi;
	IDirect3DRMDevice		*m_prlDevice;
	IDirect3DRMWinDevice	*m_prlWinDevice;
	IDirect3DRMViewport		*m_prlViewport;
	IDirect3DRMFrame		*m_prlfScene;
	IDirect3DRMFrame		*m_prlfCamera;
	IDirect3DRMFrame		*m_prlfEmptyScene;
	IDirect3DRMFrame		*m_prlfEmptySceneCamera;
	BOOL					m_bMoveScene;
	D3DCOLORMODEL			m_d3dColorModel;
	LPDIRECTDRAWPALETTE		m_pPalette;
	float					m_fField;

	static CPtrList			m_list3DRenderers;
};
