// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef __VWMMEXO_H_
#define __VWMMEXO_H_

#include "resource.h"       // maisymbols
#include <propbase.h>
#include "objimpl.h"
#include <vwanim.h>

#include "mmhelp.h"

EXTERN_C const IID LIBID_VWMMLib;
EXTERN_C const CLSID CLSID_MultimediaExemplar;

#define ReportMMError(hr) SUCCEEDED(hr) ? hr : ReportError("VWSYSTEM.MultimediaEx.1", hr, 0xe00, 0xe80, "MMEXEMPLAR")


DECLARE_VWDISPATCH(MultimediaExemplar, LIBID_VWMMLib)
{
public:
//	CMultimediaExemplarObject();

DECLARE_REGISTRY_RESOURCEID(IDR_VWMMEX)

BEGIN_COM_MAP(CMultimediaExemplarObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWDispatch)
	COM_INTERFACE_ENTRY(IMultimediaExemplar)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

public:
	STDMETHOD(Install)(IModule* pModule);

	STDMETHOD(CreateGeometry)(BSTR bstrGeometryName, IVWGeometry **ppgeometry);
	STDMETHOD(CreateFrame)(IVWFrame **ppframe);
	STDMETHOD(CreateSpritedGeometry)(BSTR bstrGeometryName, IVWGeometrySprite **ppgeometry);
	STDMETHOD(CreateVector)(float x, float y, float z, IVector ** ppvector);
	STDMETHOD(CreateAccessoryData)(BSTR bstrAccessoryGraphic, float fPosX, float fPosY, IVWAccessoryData **ppaccessorydata);
	STDMETHOD(CreateSound)(BSTR SoundURL, VARIANT_BOOL bIs3D, VARIANT_BOOL bIsLooping, ISound **ppSound);
	STDMETHOD(CreateBoundaryList)(IPropertyList** ppcell);
	STDMETHOD(CreatePickData)(IVWPickData** ppPickData);
	STDMETHOD(CreateBoundary)(VARIANT varInitialValue, IBoundary** ppBoundary);

	STDMETHOD(OnAutoDownloadAccess)(long lPerms, VARIANT_BOOL* bReturnValue);

	// Extended Thing methods
	STDMETHOD(ComposeScene)(IThing *pContext, IVWFrame **ppvwf);
	STDMETHOD(GetFrame)(IVWFrame **ppvwf);
	STDMETHOD(GetGeometryFrame)(IVWFrame **ppvwf);
	STDMETHOD(SetCamera)();
	STDMETHOD(Approach)(IThing *pThing);
	STDMETHOD(ApproachLocation)(float fPosX, float fPosY, float fPosZ, float fDirX, float fDirY, float fDirZ);
	STDMETHOD(UpdateIsApproachable)(IMenuItem *pmi);
	STDMETHOD(PositionOrientationHeartbeat)(IThing *pThing);
	STDMETHOD(OnScaleChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnUpChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnDirectionChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnPositionChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnBoundaryListChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnSolidChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnIsConnectedChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnVisibleChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnIsMoveableChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnRotationRateChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnRotationAxisChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnRenderQualityChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnGeometryChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnGeometryNameChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnContentsVisibleChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(FireIntersectEvent)(IVWFrame *pvwfRef, float fSrcX, float fSrcY, float fSrcZ, float fDestX, float fDestY, float fDestZ, IPropertyList *pIDataList);
//DXAnimation methods
	STDMETHOD(StartD3DInterpolation)(BSTR bstrProperty, DWORD dwTime);
	STDMETHOD(StartPositionPathAnimation)(IPropertyMap* pPathMap, float fTimeScale, float fStartRange, float fEndRange, float fIndex, CComBSTR bstrCallBack, long lOptions, IThing* pThis,long lSystemTime);
	STDMETHOD(StartDirectionPathAnimation)(IPropertyMap* pPathMap, float fTimeScale, float fStartRange, float fEndRange, float fIndex, CComBSTR bstrCallBack, long lOptions, IThing* pThis,long lSystemTime);
	STDMETHOD(StartUpPathAnimation)(IPropertyMap* pPathMap, float fTimeScale, float fStartRange, float fEndRange, float fIndex, CComBSTR bstrCallBack, long lOptions, IThing* pThis,long lSystemTime);
	STDMETHOD(StartScalePathAnimation)(IPropertyMap* pPathMap, float fTimeScale, float fStartRange, float fEndRange, float fIndex, CComBSTR bstrCallBack, long lOptions, IThing* pThis,long lSystemTime);
	STDMETHOD(StartLightPropertyPathAnimation)(long lPropertyFlag, IPropertyMap* pPathMap, float fTimeScale, float fStartRange, float fEndRange, float fIndex, CComBSTR bstrCallBack, long lOptions, IThing* pThis,long lSystemTime);
	STDMETHOD(StartFileAnimation)(float fTimeScale, float fStartRange, float fEndRange, float fIndex, CComBSTR bstrCallBack, long lOptions, IThing* pThis,long lSystemTime);
	STDMETHOD(StartD3DAnimation)(BSTR bstrProperty, DWORD dwTime);
	STDMETHOD(SetClientSystemTime)(long lSystemTime);
	STDMETHOD(GetFinalAnimationVector)(IThing* pThis, BSTR bstrPropStart, IVector** pVector);
	STDMETHOD(StartAnimation)(BSTR bstrPropertyName, VARIANT varPathMap, VARIANT varCallBack, VARIANT varOptions, VARIANT varTimeScale, VARIANT varStartRange, VARIANT varEndRange, VARIANT varIndex);
	STDMETHOD(StopAnimation)(BSTR bstrPropertyName, VARIANT varOptions);
	STDMETHOD(SetAnimationIndex)(BSTR bstrPropertyName, VARIANT varIndex, VARIANT varOptions);
	STDMETHOD(GetAnimationFileKeys)(IPropertyList** ppKeyList);
	STDMETHOD(StartAnimationServer)(BSTR bstrPropertyName, VARIANT varPathMap, VARIANT varCallBack, VARIANT varOptions, VARIANT varTimeScale, VARIANT varStartRange, VARIANT varEndRange, VARIANT varIndex);
	STDMETHOD(InvokeStaticDoneMethods)(IThing* pThing, BSTR bstrPropertyName);
	STDMETHOD(RemovePriorAnimations)(IThing* pThing, BSTR bstrPropertyName);
	STDMETHOD(ValidateAnimationPath)(VARIANT varPathMap, IVector** ppNearest, float* pfEndRange, float* pfStartRange);
	STDMETHOD(StopAnimationServer)(BSTR bstrPropertyName, VARIANT varValue, VARIANT varOptions);
	STDMETHOD(SetAnimationIndexServer)(BSTR bstrPropertyName, VARIANT varIndex, VARIANT varOptions);
	STDMETHOD(FinishAnimation)(VARIANT varSetProp, VARIANT varCompleted, BSTR bstrPropertyName, BSTR bstrCallBack, VARIANT varValue);
	STDMETHOD(SetAnimationPropertyRequest)(BSTR bstrPropertyName, VARIANT varStartTime, VARIANT varValue);
	STDMETHOD(HandleStartAnimation)(BSTR bstrPropertyName, VARIANT varPathMap, VARIANT varCallBack, VARIANT varOptions, VARIANT varTimeScale, VARIANT varStartRange, VARIANT varEndRange, VARIANT varIndex);
	STDMETHOD(HandleStopAnimation)(BSTR bstrPropertyName, VARIANT varOptions);
	STDMETHOD(HandleStopAnimationWithRequest)(BSTR bstrPropertyName, VARIANT varOptions);
	STDMETHOD(HandleSetAnimationIndex)(BSTR bstrPropertyName, VARIANT varIndex, VARIANT varOptions);
	STDMETHOD(ULongAdd)(unsigned long lNum1, unsigned long lNum2, unsigned long * lNum3);
	STDMETHOD(ULongSubtract)(unsigned long lNum1, unsigned long lNum2, unsigned long * lNum3);
	STDMETHOD(RecoverAnimation)(BSTR bstrPropertyName, IThing* pThis);
	STDMETHOD(GetAnimationSystemTime)(long* lCurrentAnimTime);
	STDMETHOD(GetAnimationStartTime)(BSTR bstrPropertyName, long* AnimStartTime);
	STDMETHOD(DoGesture)(int iGesture);
	STDMETHOD(DoNamedGesture)(BSTR bstrGesture);
	STDMETHOD(GestureText)(BSTR bstrGesture, BSTR *pbstrText);
	STDMETHOD(DoGestureClient)(IThing *pThingOriginator, int iGesture);
	STDMETHOD(GetNamedGestures)(IPropertyList **ppGestureNameList);
	STDMETHOD(FindFloorHeight)(float fPosX, float fPosY, float fPosZ, float* fFloorHeight);
	STDMETHOD(InitializeGraphics)(	BSTR bstrGeometry,
									float fXPos, float fYPos, float fZPos,
									float fXDir, float fYDir, float fZDir,
									VARIANT varXScale, VARIANT varYScale, VARIANT varZScale);
	STDMETHOD(InitializeSpriteGraphics)(BSTR bstrSprite,
										float fXPos, float fYPos, float fZPos,
										float fXDir, float fYDir, float fZDir,
										VARIANT varXScale, VARIANT varYScale, VARIANT varZScale);
	STDMETHOD(SetSound)(BSTR bstrWaveURL,
						VARIANT bIsLooping, 
						VARIANT lVolume, 
						VARIANT lFrequency,
						VARIANT lPan);

	STDMETHOD(Set3DSound)(BSTR bstrWaveURL,
						 VARIANT bIsLooping,
						 VARIANT dwInsideConeAngle,
						 VARIANT dwOutsideConeAngle,
						 VARIANT lConeOutsideVolume,
						 VARIANT flMinDistance,
						 VARIANT flMaxDistance);
	STDMETHOD(SetClientSound)(BSTR bstrWaveURL,
						VARIANT bIsLooping, 
						VARIANT lVolume, 
						VARIANT lFrequency,
						VARIANT lPan);

	STDMETHOD(SetClient3DSound)(BSTR bstrWaveURL,
						 VARIANT bIsLooping,
						 VARIANT dwInsideConeAngle,
						 VARIANT dwOutsideConeAngle,
						 VARIANT lConeOutsideVolume,
						 VARIANT flMinDistance,
						 VARIANT flMaxDistance);

	STDMETHOD(Take)(IThing *pThing);
	STDMETHOD(Drop)(IThing *pThing);
	STDMETHOD(Fall)();
	STDMETHOD(DurationReset)(BSTR bstrProperty, long nLastDuration);
	STDMETHOD(OnApproachDone)();
	STDMETHOD(OnContentSystemEnter)(IThing *pWhere, IThing *pWho);
	STDMETHOD(OnContentSystemLeave)(IThing *pWhere, IThing *pWho);
	STDMETHOD(UpdateSolidBoundary)(float fMinX, float fMinY, float fMinZ, float fMaxX, float fMaxY, float fMaxZ);
	STDMETHOD(OnLabelPropertyChanged)(long lHint, VARIANT varHintData);
	
	// named mesh functions
	STDMETHOD(GetNamedMeshes)(IPropertyList **ppMeshList);
	STDMETHOD(GetNamedMeshTexture)(BSTR bstrMeshName, BSTR *pbstrTextureURL);
	STDMETHOD(SetNamedMeshTexture)(BSTR bstrMeshName, BSTR bstrTextureURL);
	STDMETHOD(SetNamedMeshTextureClient)(BSTR bstrMeshName, BSTR bstrTextureURL);
	STDMETHOD(GetNamedMeshColor)(BSTR bstrMeshName, OLE_COLOR *pNewColor);
	STDMETHOD(SetNamedMeshColor)(BSTR bstrMeshName, OLE_COLOR newColor);
	STDMETHOD(SetNamedMeshColorClient)(BSTR bstrMeshName, OLE_COLOR newColor);
	STDMETHOD(OnLoadGeometry)(IVWFrame *pNewFrame);

	HRESULT		RestorePersistedMeshChanges();

	// Policy checking methods on mmthing
	STDMETHOD(OnPosDirScaleAccess)(long lPerms, VARIANT_BOOL* bReturnValue);

	// Extended Avatar methods
	STDMETHOD(OnCreate)(IThing *pthing);
	STDMETHOD(OnWornAccessoriesChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(LookToward)(IThing *pThing);
	STDMETHOD(ObjectInView)(IThing *pThing, VARIANT_BOOL *pbool);
	STDMETHOD(SetFieldOfView)(int newView);
	STDMETHOD(UnWear)(IThing *pThing);
	STDMETHOD(Wear)(IThing *pThing);
	STDMETHOD(OnConnect)(IThing *pAvatar);
	STDMETHOD(OnContentLeave)(IThing *pWhere, IThing *pWhat);
	STDMETHOD(ThumbnailFileName)(BSTR *pbstrFileName);
	STDMETHOD(AddDefaultKeys)();
	STDMETHOD(Teleport)(IThing *pNewRoom);
	HRESULT AddDefaultSystemKeys();

	// Extended artifact methods
	STDMETHOD(UpdateWear)(IMenuItem *pmi);
	STDMETHOD(UpdateUnWear)(IMenuItem *pmi);
	STDMETHOD(OnIsAccessoryChanged)(long lHint, VARIANT varHintData);

	// Extended room methods
	STDMETHOD(ValidatePosition)(IThing *pObject, IVector *pPosition, IVector **ppNewPos);
	STDMETHOD(OnFogChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnFogStartChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnFogEndChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnFogColorChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnRoomLoadGeometry)(IVWFrame *pNewFrame);
	STDMETHOD(OnActivate)(IThing* pThing);
	STDMETHOD(OnDeactivate)(IThing* pThing);
	STDMETHOD(RecoverAnimationCallBacks)(IThing *pThing, BSTR bstrPropertyName);


	// Extended portal methods
	STDMETHOD(TransportTo)(IThing *pThing);
	STDMETHOD(OnIntersectee) (IThing *pIntersector, float fX, float fY, float fZ, 
							 int iEdgeCycIndex, int iEdgeIndex, VARIANT_BOOL bTransition);

	//Light Exemplar methods
	STDMETHOD(OnLightTypeChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnLightColorChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnLightUmbraChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnLightPenumbraChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnLightRangeChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnLightLoadGeometry)(IVWFrame *pNewFrame);
	STDMETHOD(OnLightEnabledChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnLightAttenuationChanged)(long lHint, VARIANT varHintData);
	

protected:

	// various helpers
	HRESULT InstallThingExtensions(IModule* pModule);
	HRESULT InstallArtifactExtensions(IModule* pModule);
	HRESULT InstallAvatarExtensions(IModule* pModule);
	HRESULT InstallHostExtensions(IModule* pModule);
	HRESULT InstallRoomExtensions(IModule* pModule);
	HRESULT InstallPortalExtensions(IModule* pModule);
	HRESULT InstallLightExemplar(IModule* pModule);

	HRESULT RemoveInterpolateAnim(IThing *pThing, BSTR bstrProperty);
	HRESULT PutInterpolateAnim(IThing *pThing, BSTR bstrProperty, IVWAnimation *pAnimation);
	VARIANT_BOOL IsStateMethodThisSide(IThing *pThing, BSTR bstrMethodName);
	VARIANT_BOOL IsMethodThisSide(IThing *pThing, BSTR bstrMethodName);
	long CurrentTime();
	HRESULT OrientAndScaleTransform(IThing *pThing, IVWFrame *pvwfRef, IVWFrame *pvwfTransform);
	HRESULT ApplySolidProperty(long lHint, IThing* pThis);
	HRESULT RemoveContentsFromGeometry(IThing *pThing, IVWFrame *pVWFrame);
	HRESULT AddContentsToGeometry(IThing *pThing, IVWFrame *pVWFrame);
	STDMETHOD(UpdateLabel)(IThing *pThing);
};

#endif //__VWMMEXO_H_
