// VWFrameO.h : Declaration of the CVWFrameObject

#include <resource.h>
#include <PropBase.h>
#include <objimpl.h>
#include <vwanim.h>
#include <ddraw.h>
#include <mmsystem.h>
#include <mmstream.h>
#include <amstream.h>
#include <ddstream.h>

//#define _DXMEDIA

#ifdef _DXMEDIA
#include <dxtrans.h>
#endif

EXTERN_C const IID LIBID_VWMMLib;
EXTERN_C const CLSID CLSID_VWFrame;

#define ReportFrameError(hr) SUCCEEDED(hr) ? hr : ReportError("VWSYSTEM.Frame.1", hr, 0xe80, 0xf00, "VWFRAME")

/////////////////////////////////////////////////////////////////////////////
// VWFrame

DECLARE_OBJECTPROPERTY(VWFrame, LIBID_VWMMLib, IDS_VWFRAMEOBJECT_TYPE)
{
DECLARE_AGGREGATABLE(CVWFrameObject)

public:
	CVWFrameObject();
	~CVWFrameObject();

BEGIN_COM_MAP(CVWFrameObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectProperty)
	COM_INTERFACE_ENTRY(IVWTransform)
	COM_INTERFACE_ENTRY(IVWFrame)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

//DECLARE_NOT_AGGREGATABLE(CVWFrameObject) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

DECLARE_REGISTRY(CVWFrameObject, _T("VWSYSTEM.Frame.1"), _T("VWSYSTEM.Frame"), IDS_VWFRAMEOBJECT_DESC, THREADFLAGS_BOTH)

	// override base class
	STDMETHOD(put_World)(IWorld* pworld);
	STDMETHOD(get_RefCount)(DWORD *pdwRef);
	STDMETHOD(Terminate)(void);

// IVWTransform
	STDMETHOD(SetPosition)(float flX, float flY, float flZ);
	STDMETHOD(GetPosition)(float * pflX, float * pflY, float * pflZ);
	STDMETHOD(SetScale)(float flX, float flY, float flZ);
	STDMETHOD(GetScale)(float * pflX, float * pflY, float * pflZ);
	STDMETHOD(SetOrientation)(float flXDir, float flYDir, float flZDir,
						      float flXUp, float flYUp, float flZUp);
	STDMETHOD(GetOrientation)(float * pflXDir, float * pflYDir, float * pflZDir,
						      float * pflXUp, float * pflYUp, float * pflZUp);

// IVWFrame
	STDMETHOD(AddChild)(IVWFrame *pvwfChild);
	STDMETHOD(DeleteChild)(IVWFrame *pvwfChild);
	STDMETHOD(IsChildOf)(IVWFrame *pvwf, VARIANT_BOOL *pbIsChild);
	STDMETHOD(GetFrameByName)(BSTR bstrName, IVWFrame **ppvwfDst);
	STDMETHOD(SetName)(BSTR bstrName);
	STDMETHOD(GetName)(BSTR* bstrName);
	STDMETHOD(Copy)(IVWFrame **ppvwfDst);
	STDMETHOD (Transform)(long nRotationAxis, float *pfDstX, float *pfDstY, float *pfDstZ, float fSrcX, float fSrcY, float fSrcZ);
	STDMETHOD (InverseTransform)(long nRotationAxis, float *pfDstX, float *pfDstY, float *pfDstZ, float fSrcX, float fSrcY, float fSrcZ);

	//transform methods
	STDMETHOD(SetPosition)(IVWFrame *pvwfRef, float flX, float flY, float flZ);
	STDMETHOD(SetScale)(IVWFrame *pvwfRef, float flX, float flY, float flZ);
	STDMETHOD(SetOrientation)(IVWFrame *pvwfRef, float flXDir, float flYDir, float flZDir, float flXUp, float flYUp, float flZUp);
	STDMETHOD(SetPositionEx)(IVWFrame *pvwfRef, float fPosX, float fPosY, float fPosZ);
	STDMETHOD(GetPositionEx)(IVWFrame *pvwfRef, float *pfPosX, float *pfPosY, float *pfPosZ);
	STDMETHOD(GetPosition)(IVWFrame *pvwfRef, VARIANT *pfPosX, VARIANT *pfPosY, VARIANT *pfPosZ);
	STDMETHOD(GetOrientationEx)(IVWFrame *pvwfRef, float *pfDirX, float *pfDirY, float *pfDirZ, float *pfUpX, float *pfUpY, float *pfUpZ);
	STDMETHOD(GetOrientation)(IVWFrame *pvwfRef, VARIANT *pfDirX, VARIANT *pfDirY, VARIANT *pfDirZ, VARIANT *pfUpX, VARIANT *pfUpY, VARIANT *pfUpZ);
	STDMETHOD(SetOrientationEx)(IVWFrame *pvwfRef, float fDirX, float fDirY, float fDirZ, float fUpX, float fUpY, float fUpZ);
	STDMETHOD(SetScaleEx)(IVWFrame *pvwfRef, float fScaleX, float fScaleY, float fScaleZ);
	STDMETHOD(GetScaleEx)(IVWFrame *pvwfRef, float *pfScaleX, float *pfScaleY, float *pfScaleZ);
	STDMETHOD(GetScale)(IVWFrame *pvwfRef, VARIANT *pfScaleX, VARIANT *pfScaleY, VARIANT *pfScaleZ);
	STDMETHOD(SetRotationRate)(IVWFrame *pvwfRef,  float rvX,  float rvY,  float rvZ,  float rvTheta);
	STDMETHOD(GetRotationRate)(IVWFrame *pvwfRef,  VARIANT* prvX,  VARIANT* prvY,  VARIANT* prvZ,  VARIANT* prvTheta);
	STDMETHOD(GetRotationRateEx)(IVWFrame *pvwfRef,  float* prvX,  float* prvY,  float* prvZ,  float* prvTheta);
	STDMETHOD(SetVelocity)(IVWFrame *pvwfRef,  float vX,  float vY,  float vZ);
	STDMETHOD(GetVelocity)(IVWFrame *pvwfRef,  VARIANT* pvX,  VARIANT* pvY,  VARIANT* pvZ);
	STDMETHOD(GetVelocityEx)(IVWFrame *pvwfRef,  float* pvX,  float* pvY,  float* pvZ);
	STDMETHOD(MoveToPosition)(IVWFrame *pvwfRef, float fPosX, float fPosY, float fPosZ, VARIANT_BOOL bFireEvents, IPropertyList **ppIDataList);
	STDMETHOD(LookAt)(float fX, float fY, float fZ);
	STDMETHOD(GetBoundingBox)( VARIANT* fminX,  VARIANT* fminY,  VARIANT* fminZ, VARIANT* fmaxX,  VARIANT* fmaxY,  VARIANT* fmaxZ );

// methods for procedural geometry
	STDMETHOD(SetAppString)(BSTR bstr);
	STDMETHOD(GetAppString)(BSTR *pbstr);

	//Light methods
	STDMETHOD(CreateLight)( VARIANT bstrName );
	STDMETHOD(DeleteLight)( long nIndex);
	STDMETHOD(SetLightColor)( long nIndex,  float fRed,  float fGreen,  float fBlue);
	STDMETHOD(SetLightAttenuation)( long nIndex,  float fConst,  float fLinear,  float fQuadratic);
	STDMETHOD(SetLightType)( long nIndex,  int iType);
	STDMETHOD(SetLightRange)( long nIndex,  float fRange);
	STDMETHOD(SetSpotLightRadii)( long nIndex,  float fUmbra,  float fPenUmbra);

	STDMETHOD(SetLightName)(long nIndex, BSTR bstrName);
	STDMETHOD(GetLightName)(long nIndex, BSTR* bstrName);
	STDMETHOD(GetLightColor)(long nIndex, VARIANT* fRed, VARIANT* fGreen, VARIANT* fBlue);
	STDMETHOD(GetLightAttenuation)( long nIndex,  VARIANT* fConst, VARIANT* fLinear,  VARIANT* fQuadratic);
	STDMETHOD(GetLightType)( long nIndex, int* iType);
	STDMETHOD(GetLightRange)( long nIndex, float* fRange);
	STDMETHOD(GetSpotLightRadii)( long nIndex,  VARIANT* fUmbra,  VARIANT* fPenUmbra);
	STDMETHOD(GetLightIndexByName)(BSTR bstrName, long* nIndex);
	STDMETHOD(GetLightCount)(long* nCount);

	//New, for progressive meshes...
	STDMETHOD(GetProgressiveMeshDetail)(long nMeshIndex, float *detail);
	STDMETHOD(SetProgressiveMeshDetail)(long nMeshIndex, float detail);
	STDMETHOD(GetProgressiveMeshFaceDetail)(long nMeshIndex, DWORD *detail);
	STDMETHOD(SetProgressiveMeshFaceDetail)(long nMeshIndex, DWORD detail);
	STDMETHOD(GetProgressiveMeshVertexDetail)(long nMeshIndex, DWORD *detail);
	STDMETHOD(SetProgressiveMeshVertexDetail)(long nMeshIndex, DWORD detail);
	STDMETHOD(SetProgressiveMeshQuality)(long nMeshIndex, int nQuality);
	STDMETHOD(GetProgressiveMeshQuality)(long nMeshIndex, int *nQuality );
	STDMETHOD(GetProgressiveMeshFaceDetailRange)(long nMeshIndex, VARIANT *min, VARIANT *max);
	STDMETHOD(GetProgressiveMeshVertexDetailRange)(long nMeshIndex, VARIANT *min, VARIANT *max);
	STDMETHOD(GetProgressiveMeshIndexByName)(BSTR bstrMeshName, long *plIndex);

	
	STDMETHOD(GetMeshName)(long nMeshIndex, BSTR *pbstrMeshName);
	STDMETHOD(SetMeshName)(long nMeshIndex, BSTR bstrMeshName);
	STDMETHOD(GetMeshIndexByName)(BSTR bstrMeshName, long *plIndex);
	STDMETHOD(GetProgressiveMeshCount)(VARIANT *pvarCount);
	STDMETHOD(SetMeshPerspective)(long nMeshIndex, VARIANT_BOOL vbPersp);
	STDMETHOD(GetMeshPerspective)(long nMeshIndex, VARIANT_BOOL *vbPersp);
	STDMETHOD(SetMeshMaterial)(long nMeshIndex, float fDiffuse, float fSpecular, float fPower);
	STDMETHOD(GetMeshMaterial)(long nMeshIndex, VARIANT *fDiffuse, VARIANT *fSpecular, VARIANT *fPower);
	STDMETHOD(SetMeshTextureFromURL)(long nMeshIndex, BSTR bstrTexture);
	STDMETHOD(SetMeshTextureFromText)(long nMeshIndex, BSTR bstrText, VARIANT_BOOL bShadowed, DWORD nWidth, DWORD nHeight, DWORD nPointHeight);
	STDMETHOD(SetMeshFaceColor)(long nMeshIndex, int index, float fRed, float fGreen, float fBlue);
	STDMETHOD(GetMeshFaceColor)(long nMeshIndex, int index, VARIANT *fRed, VARIANT *fGreen, VARIANT *fBlue);
	STDMETHOD(SetMeshFaceVertex)(long nMeshIndex, int nFaceIndex, int index, float fX, float fY, float fZ);
	STDMETHOD(GetMeshFaceVertex)(long nMeshIndex, int nFaceIndex, int index, VARIANT *fX, VARIANT *fY, VARIANT *fZ);
	STDMETHOD(SetMeshFaceVertexColor)(long nMeshIndex, int nFaceIndex, int index, float fRed, float fGreen, float fBlue);
	STDMETHOD(GetMeshFaceVertexColor)(long nMeshIndex, int nFaceIndex, int index, VARIANT *fRed, VARIANT *fGreen, VARIANT *fBlue);
	STDMETHOD(SetMeshFaceUV)(long nMeshIndex, int nFaceIndex, int index, float fU, float fV);
	STDMETHOD(GetMeshFaceUV)(long nMeshIndex, int nFaceIndex, int index, VARIANT *fU, VARIANT *fV);
	STDMETHOD(WrapMesh)(long nMeshIndex, int type, float scaleu, float scalev);
	STDMETHOD(GetMeshCount)(VARIANT *pvarCount);
	STDMETHOD(SetMeshColor)(long nMeshIndex, float fRed, float fGreen, float fBlue);
	STDMETHOD(GetMeshColor)(long nMeshIndex, VARIANT *fRed, VARIANT *fGreen, VARIANT *fBlue);
	STDMETHOD(SetMeshQuality)(long nMeshIndex, int nQuality);
	STDMETHOD(GetMeshQuality)(long nMeshIndex, int *nQuality );
	STDMETHOD(CreateUnitMesh)();
	STDMETHOD(CreateMesh)();
	STDMETHOD(CreateMeshFace)(long nMeshIndex, VARIANT varArgs);
	STDMETHOD(GetMeshFaceCount)(long nMeshIndex, VARIANT* varFaceCount);
	STDMETHOD(GetMeshTextureName)(long nMeshIndex, BSTR* bstrTexture);
	STDMETHOD(GetMeshFaceVertexCount)(long nMeshIndex, long nFaceIndex, long* nCount);
	STDMETHOD(AddMeshFaceVertex)(long nMeshIndex, int nFaceIndex, float fX, float fY, float fZ);

 //Visual functions
	STDMETHOD(GetVisualCountByIID)(REFIID ref, VARIANT *pvarCount);
	STDMETHOD(GetVisualCount)(VARIANT *pvarCount);
	STDMETHOD(GetVisualIndexByName)(BSTR bstrName, long* nIndex);

	//Decal stuff
	STDMETHOD(CreateDecalFromURL)(BSTR bstrTexture);
	STDMETHOD(CreateDecalFromText)(BSTR bstrText, VARIANT bstrName);
	STDMETHOD(CreateDecalFromTextExt)(BSTR bstrText, long lPlacement, BSTR bstrFontName, long lFontSize, long lLabelStyle, IVector *pvFontColor, IVector *pvShadowColor, VARIANT bstrName);
	STDMETHOD(DeleteDecal)(long nMeshIndex);
	STDMETHOD(SetDecalSize)(long nMeshIndex, float fHeight, float fWidth, VARIANT_BOOL bScale);
	STDMETHOD(SetDecalOrigin)(long nMeshIndex, long lx, long ly);
	STDMETHOD(GetDecalSize)(long nMeshIndex, VARIANT* fHeight, VARIANT* fWidth, VARIANT* bScale);
	STDMETHOD(GetDecalOrigin)(long nMeshIndex, VARIANT* lx, VARIANT*  ly);
	STDMETHOD(GetDecalCount)(long *nCount);
	STDMETHOD(GetDecalIndexByName)(BSTR bstrDecalName, long *plIndex);

	STDMETHOD(FindIntersections)(float fStartPosX, float fStartPosY, float fStartPosZ, float fEndPosX, float fEndPosY, float fEndPosZ, IPropertyList **ppIDataList);
	STDMETHOD(GetFloorHeight)(IVWFrame *pvwfRef, float *pfFloorHeight);
	STDMETHOD(GetCeilingHeight)(IVWFrame *pvwfRef, float *pfCeilingHeight);
	STDMETHOD(GetFloorHeightAtPosition)(float fX, float fY, float fZ, float *pfFloorHeight);
	STDMETHOD(GetCeilingHeightAtPosition)(float fX, float fY, float fZ, float *pfCeilingHeight);

	STDMETHOD(get_Frame3D)(LPDIRECT3DRMFRAME *ppd3drmf);
	STDMETHOD(put_Frame3D)(LPDIRECT3DRMFRAME pd3drmf);
	STDMETHOD(get_Frame2D)(LPDIRECT2DRMFRAME *ppd2drmf);
	STDMETHOD(put_Frame2D)(LPDIRECT2DRMFRAME pd2drmf);

	STDMETHOD(get_BoundaryList)(IPropertyList **ppBoundaryList);
	STDMETHOD(put_BoundaryList)(IPropertyList *pBoundaryList);

	STDMETHOD(GetParent)(IVWFrame **ppvwfParent);
	STDMETHOD(GetChildren)(IPropertyList **ppChildren);
	STDMETHOD(GetVWFChildren)(IPropertyList **ppChildren);

	STDMETHOD(GetScene)(IVWFrame **ppvwfScene);

	STDMETHOD(get_Thing)(IThing **ppThing);
	STDMETHOD(put_Thing)(IThing *pThing);

	STDMETHOD(put_GeometryCache)(IVWGeometryCache *pVWGeometryCache);
	STDMETHOD(put_GeometryObject)(IVWGeometry *pVWGeometry);
	STDMETHOD(get_GeometryObject)(IVWGeometry **ppVWGeometry);

	// helper functions for procedural geometry
	STDMETHOD(GetVisual)(REFIID, void** visualObj, int cWhich);

	STDMETHOD(FindFrame)(BSTR bstrName, LPDIRECT3DRMFRAME pStartFrame, LPDIRECT3DRMFRAME *ppFrame);
	STDMETHOD(RecurseQuality)(LPDIRECT3DRMFRAME pStartFrame, REFIID refid, D3DRMRENDERQUALITY quality);

	//Internal functions
	ULONG InternalRelease();

	//Fog functions
	STDMETHOD(SetSceneFogColor)(float red,float green,float blue);
	STDMETHOD(SetSceneFogEnable)(VARIANT_BOOL fogEnable);
	STDMETHOD(SetSceneFogParams)(float start,float end);

	//Shadow function
	STDMETHOD(CreateShadow)(int nMeshIndex,IVWFrame *pvwfLightSrc,int nIndex,float ylevel);
	STDMETHOD(DeleteShadow)(int nMeshIndex,IVWFrame *pvwfLightSrc,int nIndex);

	//STDMETHOD(get_Animation)(LPDIRECT3DRMANIMATION2 *ppd3drmanim);
	//STDMETHOD(put_Animation)(LPDIRECT3DRMANIMATION2 ppd3drmanim);
	STDMETHOD(get_AnimationSet)(LPDIRECT3DRMANIMATIONSET2 *ppd3drmanimset);
	STDMETHOD(put_AnimationSet)(LPDIRECT3DRMANIMATIONSET2 ppd3drmanimset);

	STDMETHOD(StartAnimation)(float fTimeScale, float fRangeStart, float fRangeEnd, BSTR bstrAnimationDoneMethod);
	STDMETHOD(StopAnimation)();
	STDMETHOD(AnimationSetTime)(float fTime);

	//Multimedia streaming functions
	STDMETHOD(PlayVideo)(int nMeshIndex,BSTR  sfilename);

	static int CVWFrameObject::VideoCallBack(LPDIRECT3DRMUSERVISUAL uvis,
									  void* arg,
									  D3DRMUSERVISUALREASON reason,
									  LPDIRECT3DRMDEVICE dev,
									  LPDIRECT3DRMVIEWPORT view);
	//transform functions
	static int CVWFrameObject::ExplodeCallBack(LPDIRECT3DRMUSERVISUAL uvis,
									  void* arg,
									  D3DRMUSERVISUALREASON reason,
									  LPDIRECT3DRMDEVICE dev,
									  LPDIRECT3DRMVIEWPORT view);
	//transform functions
	STDMETHOD(Explode)(unsigned long dwTime);
	STDMETHOD(SetInternalScale)(float fScaleX,float fScaleY,float fScaleZ);

#ifdef _DXMEDIA
	IDirect3DRMTexture3*		m_pVidTexture;
	IDirectDrawSurface*			m_pVidSurface;
	IMultiMediaStream*			m_pMMStream;
 	IDirectDrawStreamSample*	m_pSample;
	HANDLE						m_hVidMutex;
	HANDLE						m_hExpMutex;
	IDXEffect*					m_pEffect;
	IDXTransform *				m_pExploder;
	IDirect3DRMMeshBuilder3*	m_pExpMeshBuilder;
	IDirect3DRMMeshBuilder3*	m_pExpMeshBuilderOrig;
	DWORD						m_time;

	//IDirect3DRMAnimation2*		m_pAnimation;
#endif
	IDirect3DRMAnimationSet2*	m_pAnimationSet;

private:
// Local helper data and funcs.
	IDirect3DRMFrame	*m_pd3drmf;
	IDirect3DRMFrame	*m_pd3drmfClone;
	IDirect2DRMFrame	*m_pd2drmf;
	IPropertyList		*m_pBoundaryList;
	CString				m_strApp;
	IPropertyList		*m_pIntersectList;
	IVWGeometryCache	*m_pVWGeometryCache;
	IVWGeometry			*m_pVWGeometryObject;
	IThing				*m_pThing;
	float				m_fScaleX, m_fScaleY, m_fScaleZ;

	//Internal functions
	BOOL Intersect(IVWFrame *pvwfRef, IVWFrame *pvwfSkip, float x1, float y1, float z1, float x2, float y2, float z2, IPropertyList **ppIDataList);
	STDMETHOD(GetFloorOrCeiling)(IVWFrame *pvwfRef, IVWFrame *pvwfSkip, float fX, float fY, float fZ, VARIANT_BOOL bFloor, float *pfHeight);
	STDMETHOD(DoIntersections)(IPropertyList* pBoundaryList,float x1, float y1, float z1, float x2, float y2, float z2, IPropertyList **ppIDataList);
	STDMETHOD(DoGetFloorHeight)(IPropertyList* pBoundaryList, float x1, float y1, float z1, float *pFloorHeight);
	STDMETHOD(DoGetCeilingHeight)(IPropertyList* pBoundaryList, float x1, float y1, float z1, float *pCeilingHeight);
	STDMETHOD(DoGetFloorOrCeiling)(IPropertyList* pBoundaryList, float x1, float y1, float z1, VARIANT_BOOL bFloor, float *pHeight);
	STDMETHOD(CloneRemoveXZRotation)();
	STDMETHOD(GetVisualIndexByNameAndIID)(BSTR bstrName, REFIID ref, long* nIndex);

	IDirect3DRM	*m_pDirect3DRM;
	IDirect2DRM	*m_pDirect2DRM;
	IVWAnimator	*m_pVWAnimator;
	CRITICAL_SECTION	m_CSChildren;
//	static IVWSound		*m_pSound;

//	static VARIANT_BOOL	m_bNoSound;
};
