// VWGeomS.h : Declaration of the CVWGeometrySprite

#include <resource.h>

#include <d3drm.h>
//#include <d3d.h>
#include <inetfile.h>

#include <propbase.h>
#include <objimpl.h>
#include <spritemn.h>

EXTERN_C const IID LIBID_VWMMLib;
EXTERN_C const CLSID CLSID_VWGeometrySprite;

EXTERN_C const CLSID CLSID_VWFrame;

#define CVIEWS 20
/////////////////////////////////////////////////////////////////////////////
// VWGeomSp

DECLARE_OBJECTPROPERTYEX(CVWGeometrySprite, IVWGeometrySprite, IID_IVWGeometrySprite, CLSID_VWGeometrySprite, LIBID_VWMMLib, IDS_VWGEOMETRYSP_TYPE),
	public IIFMCallback
{
	DECLARE_AGGREGATABLE(CVWGeometrySprite)

public:
	CVWGeometrySprite();
	~CVWGeometrySprite();

	STDMETHOD(Marshall)(DWORD dwOptions, IMarshallBuffer* pbuffer);
	STDMETHOD(UnMarshall)(IUnMarshallBuffer* pbuffer);
	STDMETHOD(MarshallToScript)(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut);
	STDMETHOD(Terminate)(void);
	STDMETHOD(get_RefCount)(DWORD *pdwRef);

BEGIN_COM_MAP(CVWGeometrySprite)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectProperty)
	COM_INTERFACE_ENTRY(IVWGeometry)
	COM_INTERFACE_ENTRY(IVWGeometrySprite)
	COM_INTERFACE_ENTRY(IIFMCallback)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

//DECLARE_NOT_AGGREGATABLE(CVWGeometrySprite) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

DECLARE_REGISTRY(CVWGeometrySprite, _T("VWSYSTEM.GeometrySprite.1"), _T("VWSYSTEM.GeometrySprite"), IDS_VWGEOMETRYSPRITE_DESC, THREADFLAGS_BOTH)

// IVWGeometry
	STDMETHOD(ComposeGeometry)(IVWFrame **ppvwf);
	STDMETHOD(ReleaseGeometry)();
	STDMETHOD(put_GeometryName)(BSTR bstr);
	STDMETHOD(get_GeometryName)(BSTR* pbstr);
	STDMETHOD(get_Frame)(IVWFrame **ppVWFrame);
	STDMETHOD(put_Frame)(IVWFrame *pVWFrame);
	STDMETHOD(get_GeometryFrame)(IVWFrame **ppVWFrame);
 	STDMETHOD(put_Occluded)(VARIANT_BOOL bOccluded);
	STDMETHOD(get_NamedGesture)(BSTR bstrGesture, int *pGestureNum);
	STDMETHOD(get_GestureText)(BSTR bstrGesture, BSTR *pbstrText);
	STDMETHOD(GetNamedGestures)(IPropertyList **ppGestureNameList);
	STDMETHOD(put_CurrentGesture)(int iGesture);
	STDMETHOD(get_CurrentGesture)(int* piGesture);
// IVWGeometrySprite
	STDMETHOD(get_ImageFileName)(BSTR* pbstrImageFileName);
	STDMETHOD(get_ThumbnailName)(BSTR* pbstrThumbnailName);
	STDMETHOD(DownloadFullImage)();
	STDMETHOD(ReloadImage)();
	STDMETHOD(get_AccessoryScene)(IDirect2DRMFrame** pd2drmFrame);
	STDMETHOD(AddAccessory)(BSTR bstrImageFileName, long lID, float fPosX, float fPosY);
	STDMETHOD(RemoveAccessory)(long lID);
	STDMETHOD(OnAccessoriesChanged)();
	STDMETHOD(AccessoryIDFromFrame)(IDirect2DRMFrame *pFrame, long *plID);
	STDMETHOD(AccessoryFrameFromID)(long lID, IDirect2DRMFrame **ppFrame);
	STDMETHOD(put_RotationAxis)(int iAxis);
	STDMETHOD(get_RotationAxis)(int* piAxis);

// IIFMCallback
	STDMETHOD(OnProgress_T2)(ULONG ulProgress, ULONG ulProgressMax, long lCookie){return S_OK;};
	STDMETHOD(OnDone_T2)(HRESULT hresult, BSTR bstrFinalPath, long lCookie, BSTR bstrFullURL);

private:

	CComBSTR	m_bstrGeometryName;
	CComBSTR	m_bstrThumbnailName;
	CComBSTR	m_bstrImageFilePath;
	CComBSTR	m_bstrImageFileURL;
	CComBSTR	m_bstrDefaultSpriteFile;
	IVWFrame	*m_pvwfTransform;
	IVWFrame	*m_pvwfGeometry;
	LPDIRECT3DRMFRAME	m_pSpriteFrame;
	LPDIRECT2DRMIMAGE	m_pSpriteImage;
	LPDIRECT2DRMIMAGE	m_pThumbnailImage;
	VARIANT_BOOL	m_bThumbnailMode;
	D2DRect		m_rectThumbnailSrc;
	D2DRect		m_rectThumbnailDst;
	IDirect3DRMTexture	*m_pd3drmTexture;
	IDirectDrawSurface	*m_pddsTexture;
	LPDIRECT3DRMMESHBUILDER m_lpd3drmMeshBuilder;
	VARIANT_BOOL	m_bOccluded;
	int				m_iAxis;
	VARIANT_BOOL	m_bAccessoriesShown;

	int					m_iSpriteImageFrame;

	IDirect2DRMFrame	*m_pd2drmfAccessoryScene;
	VARIANT_BOOL		m_bAccessoriesDirty;

	IDirect3DRM		*m_pDirect3DRM;
	IDirect2DRM		*m_pDirect2DRM;
	IInternetFileManager	*m_pINetFileManager;

	ISpriteManager		*m_pSpriteManager;

	STDMETHOD(InitializeSprite)(VARIANT_BOOL bForceDownload = VARIANT_FALSE);
	STDMETHOD(ReadSpriteFile)(BSTR bstrSpriteFile, VARIANT_BOOL bGetImageFile);
	STDMETHOD(InitializeThumbnailImage)();
	HRESULT CreateSprite();
	STDMETHOD(ReleaseSprite)();

public:
	static int CVWGeometrySprite::UpdateCallBack(LPDIRECT3DRMUSERVISUAL obj, LPVOID arg,
                                    D3DRMUSERVISUALREASON reason,
                                    LPDIRECT3DRMDEVICE dev,
                                    LPDIRECT3DRMVIEWPORT view);
	static void CVWGeometrySprite::DestroyCallBack(LPDIRECT3DRMOBJECT obj, LPVOID arg);
};
