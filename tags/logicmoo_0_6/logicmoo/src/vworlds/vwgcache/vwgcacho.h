// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWGCachO.h : Declaration of the CVWGeometryCacheObject

#include <resource.h>       // main symbols
#include <inetfile.h>
#include <VWRender.h>
#include <propbase.h>
#include <afxtempl.h>
#ifndef NO_3DS_CONVERTER
#include <3dstod3d.h>  //For IConvert3DSGeometry
#endif
#ifndef NO_VRML_CONVERTER
#include <VRML2D3D.h>  //For IConvertVRMLGeometry
#endif

// V-Worlds optimized templates
#include <vwtempl.h>

#include <atlctl.h>

EXTERN_C const IID LIBID_VWMMLib;
EXTERN_C const CLSID CLSID_VWGeometryCache;

#ifndef NO_VRML_CONVERTER
EXTERN_C const CLSID CLSID_ConvertVRMLGeometry;
#endif

#ifndef NO_3DS_CONVERTER
EXTERN_C const CLSID CLSID_Convert3DSGeometry;
#endif

EXTERN_C const CLSID CLSID_VWFrame;

/////////////////////////////////////////////////////////////////////////////
// VWGCache

class ATL_NO_VTABLE CVWGeometryCacheObject :
 	public IDispatchImpl<IVWGeometryCache, &IID_IVWGeometryCache, &LIBID_VWMMLib>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVWGeometryCacheObject, &CLSID_VWGeometryCache>,
	public IObjectSafetyImpl<CVWGeometryCacheObject, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
	public ISupportErrorInfoImpl<&IID_IVWGeometryCache>,
	public IIFMCallback,
	public ITextureCallback
{
DECLARE_AGGREGATABLE(CVWGeometryCacheObject)

friend class CWDBReader;
friend void loadObjects(LPDIRECT3DRMOBJECT pObject, REFIID ObjectGuid, void *parg);

public:
	CVWGeometryCacheObject();
	~CVWGeometryCacheObject();

BEGIN_COM_MAP(CVWGeometryCacheObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWGeometryCache)
	COM_INTERFACE_ENTRY(IIFMCallback)
	COM_INTERFACE_ENTRY(ITextureCallback)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

DECLARE_REGISTRY(CVWGeometryCacheObject, _T("VWSYSTEM.GeometryCache.1"), _T("VWSYSTEM.GeometryCache"), IDS_VWGEOMETRYCACHEOBJECT_DESC, THREADFLAGS_BOTH)

// IVWGeometryCache
	STDMETHOD(Init)(IWorld *pWorld);
	STDMETHOD(LoadGeometry)(BSTR bstrGeometry, IVWFrame **ppvwf);
	STDMETHOD(ReleaseGeometry)(IVWFrame *pvwf);
	STDMETHOD(SetGeomCacheSize)(long iCacheSize);

// IIFMCallback
	STDMETHOD(OnProgress_T2)(ULONG ulProgress, ULONG ulProgressMax, long lCookie);
	STDMETHOD(OnDone_T2)(HRESULT hresult, BSTR bstrFinalPath, long lCookie, BSTR bstrFullURL);
// ITextureCallback
	STDMETHOD(LoadTextureCallback)(BSTR bstr, LPDIRECT3DRMTEXTURE* ppD3DTexture);

// Public helper funcs.
	STDMETHOD(LoadTextureInt)(BSTR bstrName, IDirect3DRMFace *pFace, LPDIRECT3DRMTEXTURE *ppd3dTexture);

private:

	STDMETHOD(ReadXExt)(const char* psz);
#ifndef NO_3DS_CONVERTER
	STDMETHOD(Read3DS)(BSTR bstrFile, LPDIRECT3DRMFRAME lpFrame);
#endif
#ifndef NO_VRML_CONVERTER
	STDMETHOD(ReadVRML)(BSTR bstrFile, LPDIRECT3DRMFRAME lpFrame);
#endif
	STDMETHOD(AttachTexturesToFaces)(CPtrList *plistFaces);
	STDMETHOD(RemoveTexturesFromFaces)(CPtrList *plistFaces);
	void RecursePerspTexture(LPDIRECT3DRMFRAME lpFrame);
	void RecurseFaceTextures(LPDIRECT3DRMMESHBUILDER lpMeshBuilder);
	HRESULT CreateDefaultImage();
	void DeleteFrame(IVWFrame *pvwFrame);

	IDirect2DRMImage		*m_pDefaultImage;
	IInternetFileManager	*m_pInternetFileManager;
	LPDIRECT2DRM			m_pD2DRM;
	LPDIRECT3DRM			m_pD3DRM;
#ifndef NO_3DS_CONVERTER
	IConvert3DSGeometry		*m_p3dsConverter;
#endif
#ifndef NO_VRML_CONVERTER
	IConvertVRMLGeometry	*m_pVRMLConverter;
#endif
	IDirectDraw				*m_pDD;
	IWorld					*m_pWorld;
	CMapStringToPtr			m_mapFileToVWFrame;
	CPtrList				m_listNotReferenced;
	int						m_iGeomCacheSize;
	CVWMap<IVWFrame*, IVWFrame*&, CPtrList*, CPtrList*&> m_mapVWFrameToFaces;
	CRITICAL_SECTION		m_CS;

	IVWFrame				*pvwfParentFrameForLoading;
	IDirect3DRMFrame		*pParentFrameForLoading;
	CString					strParentURLForLoading;
	D3DRMRENDERQUALITY		d3drmRenderQuality;
	bool					bLoaded;
};

