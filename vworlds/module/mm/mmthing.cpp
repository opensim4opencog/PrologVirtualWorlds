// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// mmthing.cpp : Thing extensions

#include "stdafx.h"
#include <d3drmwin.h>
#include <vwmmex.h>
#include <vwidata.h>
#include "vwutils.h"
#include "vwmmexo.h"
#include <vwnvtool.h>
#include <math.h>
#include <vwrnedit.h>
#include <vwsevent.h>
#include <syshelp.h>
#include <mmsystem.h>

extern const CLSID CLSID_Boundary;

///////////////////////////////////////////////////////////////////////
// Parameters

#define iInterpolatePeriod		50
#define iApproachTimePerUnit	125
#define PI						3.1415927F
#define GRAVITY_CONST			9.8F
#define fFallOffset				0.001F
#define	iDurationResetLag		1000
#define FLOAT_EQUAL(x,y) ( fabs(x - y) < 0.001)
#define LABELDECALNAME "vwLabelDecal"


CComBSTR bstrPositionDoneDefault("OnPositionDone");
CComBSTR bstrDirectionDoneDefault("OnDirectionDone");
CComBSTR bstrUpDoneDefault("OnUpDone");
CComBSTR bstrScaleDoneDefault("OnScaleDone");
CComBSTR bstrOnApproachDone("OnApproachDone");
CComBSTR bstrBoundaryList("BoundaryList");
CComBSTR bstrGetFrame("GetFrame");
CComBSTR bstrSolid("IsSolid");
CComBSTR bstrBoundary("Boundary");
CComBSTR bstrBoundaryUpdate("BoundaryUpdate");
CComBSTR bstrBoundingBox("BoundingBox");
CComBSTR bstrUpdateSolidBoundary("UpdateSolidBoundary");

///////////////////////////////////////////////////////////////////////
// Helpers

HRESULT	_GetFrameHelper(IThing *pThing, IVWFrame **ppvwf);
float CastToFloat(VARIANT var);
long CastToLong(VARIANT var);
VARIANT_BOOL IsNumeric (VARIANT var);

void ConvertDirToRotation(float fDirX, float fDirY, float fDirZ, float &fRotation)
{
	D3DVECTOR rlvDir;
	D3DVECTOR rlvPreDef = {0.0F, 0.0F, 1.0F};
	D3DVALUE  valCosTheta;

	rlvDir.x = fDirX;
	rlvDir.y = fDirY;
	rlvDir.z = fDirZ;

	D3DRMVectorNormalize(&rlvDir);

	// First need angle between the pre-defined angle and rlvDir.
	valCosTheta = D3DRMVectorDotProduct(&rlvPreDef, &rlvDir);
	fRotation = (D3DVALUE)acos(valCosTheta);

	// This will always be the acute angle. Since rotation will always be in positive direction we must
	// give correct angle (possibly obtuse) for that direction.
	if (rlvDir.x < 0.0F)
	{
		// Acute angle will not work, need obtuse angle.
		fRotation = (2*PI) - fRotation;
	}
}

HRESULT FindZeroIntersectPoint(IVWFrame *pvwfRef, float *pfSrcX, float *pfSrcY, float *pfSrcZ, float *pfDstX, float *pfDstY, float *pfDstZ)
{
	CComPtr<IPropertyList>		pIDataList;
	CComPtr<IVWIntersectData>	pVWIntersectData;
	CComPtr<IPropertyList>		pCell;
	CComPtr<IBoundary>			pBoundary;
	long						lCount;
	int							iBoundaryIndex;
	VARIANT_BOOL				bPassable;
	HRESULT hr = S_OK;

	if (FAILED(hr = pvwfRef->FindIntersections(*pfSrcX, *pfSrcY, *pfSrcZ, *pfDstX, *pfDstY, *pfDstZ, &pIDataList)))
		return hr;

    if (pIDataList)
	{
		pIDataList->get_Count(&lCount);
		if (FAILED(hr = pIDataList->get_ObjectProperty(lCount-1, (IObjectProperty**)&pVWIntersectData)) || !pVWIntersectData)
			return hr;
		if (FAILED(hr = pVWIntersectData->get_BoundaryList(&pCell)) || !pCell)
			return hr;
		pVWIntersectData->get_BoundaryIndex(&iBoundaryIndex);
		if (FAILED(hr = pCell->get_ObjectProperty(iBoundaryIndex, (IObjectProperty**) &pBoundary)) || !pBoundary)
			return hr;
		pBoundary->get_IsPassable(&bPassable);
		if (!bPassable)
		{
			*pfDstX += (*pfSrcX-*pfDstX)/2;
			*pfDstY += (*pfSrcY-*pfDstY)/2;
			*pfDstZ += (*pfSrcZ-*pfDstZ)/2;
			if (FAILED(hr = FindZeroIntersectPoint(pvwfRef, pfSrcX, pfSrcY, pfSrcZ, pfDstX, pfDstY, pfDstZ)))
				return hr;
		}
	}

	return hr;
}

///////////////////////////////////////////////////////////////////////
// Install 

HRESULT CMultimediaExemplarObject::InstallThingExtensions(IModule* pModule)
{
	CMethodInfo rgMethod[] =
	{
		{ METHOD_CLIENT,	"SetCamera",								NULL, PSBIT_HIDDEN,			PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_CLIENT | METHOD_SERVER,	"Approach",					NULL, PSBIT_NONE,			PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_CLIENT | METHOD_SERVER,	"ApproachLocation",			NULL, PSBIT_NONE,			PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_CLIENT,	"UpdateIsApproachable",						NULL, PSBIT_HIDDEN,			PS_ALLEXECUTEMETHOD  },
//		{ METHOD_CLIENT,	"MovetoPosition",							NULL, PSBIT_NONE,			PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_CLIENT,	"PositionOrientationHeartbeat",				NULL, PSBIT_HIDDEN,			PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT | METHOD_SERVER,	"StartAnimation",			NULL,		PSBIT_NONE,		PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_CLIENT | METHOD_SERVER,	"StopAnimation",			NULL,		PSBIT_NONE,		PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_CLIENT | METHOD_SERVER,	"SetAnimationIndex",		NULL,		PSBIT_NONE,		PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_CLIENT | METHOD_SERVER,	"GetAnimationStartTime",	NULL,		PSBIT_NONE,		PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_CLIENT,	"SetClientSystemTime",						NULL,		PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD  },	
		{ METHOD_SERVER,	"StartAnimationServer",						NULL,		PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_SERVER,	"StopAnimationServer",						NULL,		PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_SERVER,	"SetAnimationIndexServer",					NULL,		PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_SERVER,	"FinishAnimation",							NULL,		PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD | PS_ALLEXECUTEMETHOD  },	
		{ METHOD_SERVER,	"StopAnimationWithRequest",					NULL,		PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_SERVER,	"SetAnimationPropertyRequest",				NULL,		PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_CLIENT,	"HandleStopAnimationWithRequest",			NULL,		PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_CLIENT,	"HandleStartAnimation",						NULL,		PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_CLIENT,	"HandleStopAnimation",						NULL,		PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_CLIENT,	"HandleSetAnimationIndex",					NULL,		PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_CLIENT,	"GetAnimationFileKeys",						NULL,		PSBIT_NONE,		PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_SERVER,	"OnScaleChanged",							NULL, PSBIT_EVENTHANDLER,	PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER,	"OnUpChanged",								NULL, PSBIT_EVENTHANDLER,	PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER,	"OnDirectionChanged",						NULL, PSBIT_EVENTHANDLER,	PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER,	"OnPositionChanged",						NULL, PSBIT_EVENTHANDLER,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"OnClientScaleChanged",		"OnScaleChanged",	  PSBIT_EVENTHANDLER,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"OnClientUpChanged",		"OnUpChanged",		  PSBIT_EVENTHANDLER,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"OnClientDirectionChanged",	"OnDirectionChanged", PSBIT_EVENTHANDLER,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"OnClientPositionChanged",	"OnPositionChanged",  PSBIT_EVENTHANDLER,	PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT,	"OnBoundaryListChanged",					NULL, PSBIT_EVENTHANDLER,	PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT,	"OnIsConnectedChanged",						NULL, PSBIT_EVENTHANDLER,	PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT,	"OnIsVisibleChanged",  "OnVisibleChanged",		  PSBIT_EVENTHANDLER,	PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT,	"OnIsMoveableChanged",						NULL, PSBIT_EVENTHANDLER,	PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT,	"OnGeometryChanged",						NULL, PSBIT_EVENTHANDLER,	PS_ALLEXECUTEMETHOD  },
		{ METHOD_SERVER,	"OnGeometryNameChanged",	NULL, PSBIT_EVENTHANDLER,	PS_ALLEXECUTEMETHOD  },	
		{ METHOD_CLIENT,	"OnClientGeometryChanged",		"OnGeometryChanged", PSBIT_EVENTHANDLER,PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT,	"OnIsContentsVisibleChanged",				"OnContentsVisibleChanged", PSBIT_EVENTHANDLER,	PS_ALLEXECUTEMETHOD  },
//		{ METHOD_CLIENT,	"UpdateScene",								NULL, PSBIT_NONE,			PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_CLIENT,	"ComposeScene",								NULL, PSBIT_HIDDEN,			PS_ALLEXECUTEMETHOD   },
		{ METHOD_CLIENT | METHOD_SERVER,	"GetFrame",					NULL, PSBIT_NONE,			PS_ALLEXECUTEMETHOD  },	
		{ METHOD_CLIENT | METHOD_SERVER,	"GetGeometryFrame",			NULL, PSBIT_NONE,			PS_ALLEXECUTEMETHOD  },	
		{ METHOD_CLIENT,	"FireIntersectEvent",						NULL, PSBIT_NONE,			PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT | METHOD_SERVER,	"GestureText",				NULL, PSBIT_HIDDEN,			PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_CLIENT,	"DoGesture",								NULL, PSBIT_HIDDEN,			PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_CLIENT,	"DoNamedGesture",							NULL, PSBIT_HIDDEN,			PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_CLIENT,	"DoGestureClient",							NULL, PSBIT_HIDDEN,			PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_CLIENT,	"GetNamedGestures",							NULL, PSBIT_NONE,			PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_SERVER,	"InitializeSpriteGraphics",		NULL, PSBIT_HIDDEN,			PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_SERVER,	"InitializeGraphics",			NULL, PSBIT_HIDDEN,			PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_SERVER,	"SetSound",									NULL, PSBIT_HIDDEN,			PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_SERVER,	"Set3DSound",								NULL, PSBIT_HIDDEN,			PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_CLIENT | METHOD_SERVER,	"SetClientSound",			NULL, PSBIT_HIDDEN,			PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_CLIENT | METHOD_SERVER,	"SetClient3DSound",			NULL, PSBIT_HIDDEN,			PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_SERVER | METHOD_CLIENT,	"Take",						NULL, PSBIT_NONE,			PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_SERVER | METHOD_CLIENT,	"Drop",						NULL, PSBIT_NONE,			PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_CLIENT,	"Fall",										NULL, PSBIT_HIDDEN,			PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT,	"DurationReset",							NULL, PSBIT_HIDDEN,			PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT,	"OnRotationRateChanged",					NULL, PSBIT_EVENTHANDLER,	PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT,	"OnRotationAxisChanged",					NULL, PSBIT_EVENTHANDLER,	PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT,	"OnRenderQualityChanged",					NULL, PSBIT_EVENTHANDLER,	PS_ALLEXECUTEMETHOD  },
		{ METHOD_SERVER,	"OnIsSolidChanged",	 "OnSolidChanged",			  PSBIT_EVENTHANDLER,	PS_ALLEXECUTEMETHOD  },
		{ METHOD_SERVER,	"UpdateSolidBoundary",						NULL, PSBIT_HIDDEN,			PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT,	"GetNamedMeshes",							NULL, PSBIT_NONE,			PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT,	"GetNamedMeshTexture",						NULL, PSBIT_NONE,			PS_ALLEXECUTEMETHOD  },
		{ METHOD_SERVER,	"SetNamedMeshTexture",						NULL, PSBIT_NONE,			PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT,	"SetNamedMeshTextureClient",				NULL, PSBIT_NONE,			PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT,	"GetNamedMeshColor",						NULL, PSBIT_NONE,			PS_ALLEXECUTEMETHOD  },
		{ METHOD_SERVER,	"SetNamedMeshColor",						NULL, PSBIT_NONE,			PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT,	"SetNamedMeshColorClient",					NULL, PSBIT_NONE,			PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT,	"OnLoadGeometry",							NULL, PSBIT_NONE,			PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPositionAccess",	"OnPosDirScaleAccess", PSBIT_NONE,	PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnDirectionAccess","OnPosDirScaleAccess", PSBIT_NONE,	PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnUpAccess",		"OnPosDirScaleAccess", PSBIT_NONE,	PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnScaleAccess",	"OnPosDirScaleAccess", PSBIT_NONE,	PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPositionDurationAccess",	"OnPosDirScaleAccess", PSBIT_NONE,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnDirectionDurationAccess","OnPosDirScaleAccess", PSBIT_NONE,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnUpDurationAccess",		"OnPosDirScaleAccess", PSBIT_NONE,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnScaleDurationAccess",	"OnPosDirScaleAccess", PSBIT_NONE,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnApproachDone",			NULL, PSBIT_HIDDEN,			PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT,	"FindFloorHeight",							NULL,	PSBIT_NONE,			PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT,	"OnContentSystemEnter",		NULL,	PSBIT_SYSTEMEVENTHANDLER,		PS_EVENTHANDLER  },
		{ METHOD_CLIENT,	"OnContentSystemLeave",		NULL,	PSBIT_SYSTEMEVENTHANDLER,		PS_EVENTHANDLER  },
		{ METHOD_CLIENT,	"OnLabelChanged",				"OnLabelPropertyChanged",	PSBIT_EVENTHANDLER,		PS_EVENTHANDLER  },
		{ METHOD_CLIENT,	"OnLabelPlacementChanged",		"OnLabelPropertyChanged",	PSBIT_EVENTHANDLER,		PS_EVENTHANDLER  },
		{ METHOD_CLIENT,	"OnLabelFontNameChanged",		"OnLabelPropertyChanged",	PSBIT_EVENTHANDLER,		PS_EVENTHANDLER  },
		{ METHOD_CLIENT,	"OnLabelFontSizeChanged",		"OnLabelPropertyChanged",	PSBIT_EVENTHANDLER,		PS_EVENTHANDLER  },
		{ METHOD_CLIENT,	"OnLabelStyleChanged",			"OnLabelPropertyChanged",	PSBIT_EVENTHANDLER,		PS_EVENTHANDLER  },
		{ METHOD_CLIENT,	"OnLabelShadowColorChanged",	"OnLabelPropertyChanged",	PSBIT_EVENTHANDLER,		PS_EVENTHANDLER  },
		{ METHOD_CLIENT,	"OnLabelFontColorChanged",		"OnLabelPropertyChanged",	PSBIT_EVENTHANDLER,		PS_EVENTHANDLER  }
	};
	int cMethod = sizeof(rgMethod)/sizeof(rgMethod[0]);
    int iMethod;
    HRESULT hr = S_OK;

	// find the Thing exemplar
	CComPtr<IThing>			pThingEx;
	CComPtr<IPropertyList>	pList;
	CComPtr<IPropertyMap>	pPropMap;
	CComPtr<IPropertyList>	pMap;
	CComPtr<IVector>		pVector;
	CComBSTR bstrGeometry;
	CComBSTR bstrMotion;
	CComBSTR bstrApproach;
	CComBSTR bstrName;
	CComBSTR bstrNameInternal;

	if (FAILED(hr = m_pWorld->get_Exemplar(CComBSTR("Thing"), &pThingEx)))
		goto ERROR_ENCOUNTERED;

	// add in methods
	for (iMethod=0; iMethod<cMethod; iMethod++)
	{
		bstrName = rgMethod[iMethod].pszName;

		if (rgMethod[iMethod].pszNameInternal == NULL)
			bstrNameInternal = bstrName;
		else
			bstrNameInternal = rgMethod[iMethod].pszNameInternal;
		
		if (FAILED(hr = pThingEx->CreateAndAddMethodExt(rgMethod[iMethod].lFlags,
													pModule,
													bstrNameInternal,
													bstrName,
													rgMethod[iMethod].psbits,
													rgMethod[iMethod].permissions)))
		    goto ERROR_ENCOUNTERED;
	}


	{
		CComPtr<IThing> pGlobal;

		//global properties for persisting the world time in ticks for animations

		if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
			goto ERROR_ENCOUNTERED;

		if (FAILED(hr = pGlobal->AddPropertyExt(	CComBSTR("SystemTime"),
													CComVariant((long) 0),
													PSBIT_NOTREMOTED | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
													PS_EXEMPLARDEFAULTPROPERTY,
													VT_I4, IID_NULL, NULL)))
			goto ERROR_ENCOUNTERED;

		if (FAILED(hr = pGlobal->AddPropertyExt(	CComBSTR("SystemStartTicks"),
													CComVariant((long) 0),
													PSBIT_LOCAL | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
													PS_EXEMPLARDEFAULTPROPERTY,
													VT_I4, IID_NULL, NULL)))
			goto ERROR_ENCOUNTERED;

		if (FAILED(hr = pGlobal->AddPropertyExt(	CComBSTR("A_RELATIVE"),
													CComVariant((long) 64),
													PSBIT_LOCAL | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
													PS_EXEMPLARDEFAULTPROPERTY,
													VT_I4, IID_NULL, NULL)))
			goto ERROR_ENCOUNTERED;

		if (FAILED(hr = pGlobal->AddPropertyExt(	CComBSTR("A_LOCAL"),
													CComVariant((long) 32),
													PSBIT_LOCAL | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
													PS_EXEMPLARDEFAULTPROPERTY,
													VT_I4, IID_NULL, NULL)))
			goto ERROR_ENCOUNTERED;

		if (FAILED(hr = pGlobal->AddPropertyExt(	CComBSTR("A_NEAREST"),
													CComVariant((long) 16),
													PSBIT_LOCAL | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
													PS_EXEMPLARDEFAULTPROPERTY,
													VT_I4, IID_NULL, NULL)))
			goto ERROR_ENCOUNTERED;

		if (FAILED(hr = pGlobal->AddPropertyExt(	CComBSTR("A_LINEAR"),
													CComVariant((long) 4),
													PSBIT_LOCAL | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
													PS_EXEMPLARDEFAULTPROPERTY,
													VT_I4, IID_NULL, NULL)))
			goto ERROR_ENCOUNTERED;

		if (FAILED(hr = pGlobal->AddPropertyExt(	CComBSTR("A_SPLINE"),
													CComVariant((long) 8),
													PSBIT_LOCAL | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
													PS_EXEMPLARDEFAULTPROPERTY,
													VT_I4, IID_NULL, NULL)))
			goto ERROR_ENCOUNTERED;

		if (FAILED(hr = pGlobal->AddPropertyExt(	CComBSTR("A_LOOP"),
													CComVariant((long) 2),
													PSBIT_LOCAL | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
													PS_EXEMPLARDEFAULTPROPERTY,
													VT_I4, IID_NULL, NULL)))
			goto ERROR_ENCOUNTERED;

		if (FAILED(hr = pGlobal->AddPropertyExt(	CComBSTR("A_ONCE"),
													CComVariant((long) 1),
													PSBIT_LOCAL | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
													PS_EXEMPLARDEFAULTPROPERTY,
													VT_I4, IID_NULL, NULL)))
			goto ERROR_ENCOUNTERED;

		CMethodInfo rgGlobalAnimMethod = {METHOD_CLIENT | METHOD_SERVER,"GetAnimationSystemTime",NULL,PSBIT_INTERNALLYADDED,PS_EXEMPLARDEFAULTMETHOD};
		bstrName = rgGlobalAnimMethod.pszName;
		bstrNameInternal = bstrName;
		if (FAILED(hr = pGlobal->CreateAndAddMethodExt(rgGlobalAnimMethod.lFlags,
													pModule,
													bstrNameInternal,
													bstrName,
													rgGlobalAnimMethod.psbits,
													rgGlobalAnimMethod.permissions)))
		    goto ERROR_ENCOUNTERED;
	}

	// Add in properties
#if 0
	// KM: removed, extinct?
	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("IsMouseVisible"), 
											CComVariant((bool)VARIANT_TRUE), 
											PSBIT_LOCAL | PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_BOOL, IID_IDispatch, NULL)))
		goto ERROR_ENCOUNTERED;
#endif

	//Create BoundingBox with invalid default value.  pdateSolidBoundary will fill it in with a good value.
	hr = CreatePropertyList(m_pWorld, &pList);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	pList->Add(CComVariant(1.0f));
	pList->Add(CComVariant(1.0f));
	pList->Add(CComVariant(1.0f));
	pList->Add(CComVariant(-1.0f));
	pList->Add(CComVariant(-1.0f));
	pList->Add(CComVariant(-1.0f));
	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("BoundingBox"), 
											CComVariant((IDispatch *) pList), 
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_FRIENDLY,
											PS_EXEMPLARDEFAULTCOLLECTION, 
											VT_DISPATCH, IID_IPropertyList, NULL)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("IsMoveable"), 
											CComVariant((bool)VARIANT_TRUE), 
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_BOOL, IID_IDispatch, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("Geometry"), 
											CComVariant((IDispatch *)NULL), 
											PSBIT_HIDDEN | PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_DISPATCH, IID_IVWGeometry, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("GeometryName"), 
											CComVariant(""), 
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_BSTR, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("InnerGeometry"), 
											CComVariant((IDispatch *)NULL), 
											PSBIT_HIDDEN | PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_DISPATCH, IID_IDispatch, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(CComBSTR("ClientGeometry"),
										 CComVariant((IDispatch*)NULL),
										 PSBIT_LOCAL|PSBIT_CLIENTPROPERTY|PSBIT_HIDDEN, 
										 PS_ALLACCESSPROPERTY, 
										 VT_VARIANT, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("Position"), 
											CComVariant((IDispatch *)NULL), 
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_ALLACCESSPROPERTY,
											VT_DISPATCH, IID_IVector, NULL)))
		goto ERROR_ENCOUNTERED;

 	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("Direction"), 
											CComVariant((IDispatch *)NULL), 
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_ALLACCESSPROPERTY,
											VT_DISPATCH, IID_IVector, NULL)))
		goto ERROR_ENCOUNTERED;

 	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("Up"), 
											CComVariant((IDispatch *)NULL), 
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_ALLACCESSPROPERTY,
											VT_DISPATCH, IID_IVector, NULL)))
		goto ERROR_ENCOUNTERED;

 	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("Scale"), 
											CComVariant((IDispatch *)NULL), 
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_ALLACCESSPROPERTY,
											VT_DISPATCH, IID_IVector, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = CreatePropertyMap(m_pWorld, &pPropMap)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("FileAnimOptions"),CComVariant((short)0) )))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("FileAnimStartTime"),CComVariant((long)0) )))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("FileAnimIndex"),CComVariant((float)0.0f) )))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("FileAnimIndex"),CComVariant((float)0.0f) )))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("FileAnimCallBack"),CComVariant(CComBSTR("")) )))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("FileAnimFinalValue"),CComVariant((long)0) )))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("FileAnimTimeScale"),CComVariant((float)1.0f) )))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("FileAnimRangeStart"),CComVariant((float)0.0f) )))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("FileAnimRangeEnd"),CComVariant((float)0.0f) )))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("FileAnimProperties"),
											CComVariant((IDispatch*)pPropMap),
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_RWA,
											VT_DISPATCH, IID_IPropertyMap, NULL)))
		goto ERROR_ENCOUNTERED;

	pPropMap.Release();

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("FileSetPropAnimation"),
											CComVariant((IDispatch*)NULL),
											PSBIT_NOTREMOTED | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_RWA,
											VT_DISPATCH, IID_IVWAnimation, NULL)))
		goto ERROR_ENCOUNTERED;

//animation properties for position

	if (FAILED(hr = CreatePropertyMap(m_pWorld, &pPropMap)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("PositionAnimPath"),
											CComVariant((IDispatch*)pPropMap),
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_RWA,
											VT_DISPATCH, IID_IPropertyMap, NULL)))
		goto ERROR_ENCOUNTERED;

	pPropMap.Release();

	if (FAILED(hr = CreatePropertyMap(m_pWorld, &pPropMap)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("PositionAnimOptions"),CComVariant((short)0))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("PositionAnimStartTime"),CComVariant((long)0))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("PositionAnimIndex"),CComVariant((float)0.0f))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("PositionAnimCallBack"),CComVariant(    CComBSTR("")    ))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("PositionAnimFinalValue"),CComVariant((long)0) )))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("PositionAnimTimeScale"),CComVariant((float)1.0f))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("PositionAnimRangeStart"),CComVariant((float)0.0f))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("PositionAnimRangeEnd"),CComVariant((float)0.0f))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("PositionAnimProperties"),
											CComVariant((IDispatch*)pPropMap),
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_RWA,
											VT_DISPATCH, IID_IPropertyMap, NULL)))
		goto ERROR_ENCOUNTERED;

	pPropMap.Release();

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("PositionSetPropAnimation"),
											CComVariant((IDispatch*)NULL),
											PSBIT_NOTREMOTED | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_RWA,
											VT_DISPATCH, IID_IVWAnimation, NULL)))
		goto ERROR_ENCOUNTERED;

//animation properties for lightcolor
	
	if (FAILED(hr = CreatePropertyMap(m_pWorld, &pPropMap)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("LightColorAnimPath"),
											CComVariant((IDispatch*)pPropMap),
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_RWA,
											VT_DISPATCH, IID_IPropertyMap, NULL)))
		goto ERROR_ENCOUNTERED;

	pPropMap.Release();

	if (FAILED(hr = CreatePropertyMap(m_pWorld, &pPropMap)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("LightColorAnimOptions"),CComVariant((short)0))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("LightColorAnimStartTime"),CComVariant((long)0))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("LightColorAnimIndex"),CComVariant((float)0.0f))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("LightColorAnimCallBack"),CComVariant(    CComBSTR("")    ))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("LightColorAnimFinalValue"),CComVariant((long)0) )))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("LightColorAnimTimeScale"),CComVariant((float)1.0f))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("LightColorAnimRangeStart"),CComVariant((float)0.0f))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("LightColorAnimRangeEnd"),CComVariant((float)0.0f))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("LightColorAnimProperties"),
											CComVariant((IDispatch*)pPropMap),
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_RWA,
											VT_DISPATCH, IID_IPropertyMap, NULL)))
		goto ERROR_ENCOUNTERED;

	pPropMap.Release();

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("LightColorSetPropAnimation"),
											CComVariant((IDispatch*)NULL),
											PSBIT_NOTREMOTED | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_RWA,
											VT_DISPATCH, IID_IVWAnimation, NULL)))
		goto ERROR_ENCOUNTERED;

//animation properties for lightattenuation

	if (FAILED(hr = CreatePropertyMap(m_pWorld, &pPropMap)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("LightAttenuationAnimPath"),
											CComVariant((IDispatch*)pPropMap),
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_RWA,
											VT_DISPATCH, IID_IPropertyMap, NULL)))
		goto ERROR_ENCOUNTERED;

	pPropMap.Release();

	if (FAILED(hr = CreatePropertyMap(m_pWorld, &pPropMap)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("LightAttenuationAnimOptions"),CComVariant((short)0))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("LightAttenuationAnimStartTime"),CComVariant((long)0))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("LightAttenuationAnimIndex"),CComVariant((float)0.0f))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("LightAttenuationAnimCallBack"),CComVariant(    CComBSTR("")    ))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("LightAttenuationAnimFinalValue"),CComVariant((long)0) )))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("LightAttenuationAnimTimeScale"),CComVariant((float)1.0f))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("LightAttenuationAnimRangeStart"),CComVariant((float)0.0f))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("LightAttenuationAnimRangeEnd"),CComVariant((float)0.0f))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("LightAttenuationAnimProperties"),
											CComVariant((IDispatch*)pPropMap),
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_RWA,
											VT_DISPATCH, IID_IPropertyMap, NULL)))
		goto ERROR_ENCOUNTERED;

	pPropMap.Release();

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("LightAttenuationSetPropAnimation"),
											CComVariant((IDispatch*)NULL),
											PSBIT_NOTREMOTED | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_RWA,
											VT_DISPATCH, IID_IVWAnimation, NULL)))
		goto ERROR_ENCOUNTERED;

//animation properties for scale

	if (FAILED(hr = CreatePropertyMap(m_pWorld, &pPropMap)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("ScaleAnimPath"),
											CComVariant((IDispatch*)pPropMap),
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_RWA,
											VT_DISPATCH, IID_IPropertyMap, NULL)))
		goto ERROR_ENCOUNTERED;

	pPropMap.Release();

	if (FAILED(hr = CreatePropertyMap(m_pWorld, &pPropMap)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("ScaleAnimOptions"),CComVariant((short)0))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("ScaleAnimStartTime"),CComVariant((long)0))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("ScaleAnimIndex"),CComVariant((float)0.0f))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("ScaleAnimCallBack"),CComVariant(    CComBSTR("")    ))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("ScaleAnimFinalValue"),CComVariant((long)0) )))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("ScaleAnimTimeScale"),CComVariant((float)1.0f))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("ScaleAnimRangeStart"),CComVariant((float)0.0f))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("ScaleAnimRangeEnd"),CComVariant((float)0.0f))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("ScaleAnimProperties"),
											CComVariant((IDispatch*)pPropMap),
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_RWA,
											VT_DISPATCH, IID_IPropertyMap, NULL)))
		goto ERROR_ENCOUNTERED;

	pPropMap.Release();

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("ScaleSetPropAnimation"),
											CComVariant((IDispatch*)NULL),
											PSBIT_NOTREMOTED | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_RWA,
											VT_DISPATCH, IID_IVWAnimation, NULL)))
		goto ERROR_ENCOUNTERED;

	//we need a second scale path for problems with animating scale and direction simultaneously
	if (FAILED(hr = CreatePropertyMap(m_pWorld, &pPropMap)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("InternalScalePath"),
											CComVariant((IDispatch*)pPropMap),
											PSBIT_LOCAL | PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_RWA,
											VT_DISPATCH, IID_IPropertyMap, NULL)))
		goto ERROR_ENCOUNTERED;

	pPropMap.Release();



	//animation properties for direction

	if (FAILED(hr = CreatePropertyMap(m_pWorld, &pPropMap)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("DirectionAnimPath"),
											CComVariant((IDispatch*)pPropMap),
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_RWA,
											VT_DISPATCH, IID_IPropertyMap, NULL)))
		goto ERROR_ENCOUNTERED;

	pPropMap.Release();

	if (FAILED(hr = CreatePropertyMap(m_pWorld, &pPropMap)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("DirectionAnimOptions"),CComVariant((short)0))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("DirectionAnimStartTime"),CComVariant((long)0))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("DirectionAnimIndex"),CComVariant((float)0.0f))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("DirectionAnimCallBack"),CComVariant(    CComBSTR("")    ))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("DirectionAnimFinalValue"),CComVariant((long)0) )))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("DirectionAnimTimeScale"),CComVariant((float)1.0f))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("DirectionAnimRangeStart"),CComVariant((float)0.0f))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("DirectionAnimRangeEnd"),CComVariant((float)0.0f))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("DirectionAnimProperties"),
											CComVariant((IDispatch*)pPropMap),
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_RWA,
											VT_DISPATCH, IID_IPropertyMap, NULL)))
		goto ERROR_ENCOUNTERED;

	pPropMap.Release();

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("DirectionSetPropAnimation"),
											CComVariant((IDispatch*)NULL),
											PSBIT_NOTREMOTED | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_RWA,
											VT_DISPATCH, IID_IVWAnimation, NULL)))
		goto ERROR_ENCOUNTERED;

//animation properties for Up
	if (FAILED(hr = CreatePropertyMap(m_pWorld, &pPropMap)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("UpAnimPath"),
											CComVariant((IDispatch*)pPropMap),
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_RWA,
											VT_DISPATCH, IID_IPropertyMap, NULL)))
		goto ERROR_ENCOUNTERED;

	pPropMap.Release();

	if (FAILED(hr = CreatePropertyMap(m_pWorld, &pPropMap)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("UpAnimOptions"),CComVariant((short)0))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("UpAnimStartTime"),CComVariant((long)0))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("UpAnimIndex"),CComVariant((float)0.0f))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("UpAnimCallBack"),CComVariant(    CComBSTR("")    ))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("UpAnimFinalValue"),CComVariant((long)0) )))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("UpAnimTimeScale"),CComVariant((float)1.0f))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("UpAnimRangeStart"),CComVariant((float)0.0f))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPropMap->put_Property(  CComBSTR("UpAnimRangeEnd"),CComVariant((float)0.0f))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("UpAnimProperties"),
											CComVariant((IDispatch*)pPropMap),
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_RWA,
											VT_DISPATCH, IID_IPropertyMap, NULL)))
		goto ERROR_ENCOUNTERED;

	pPropMap.Release();

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("UpSetPropAnimation"),
											CComVariant((IDispatch*)NULL),
											PSBIT_NOTREMOTED | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_RWA,
											VT_DISPATCH, IID_IVWAnimation, NULL)))
		goto ERROR_ENCOUNTERED;

//property durations for 1 shot interpolation
	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("PositionDuration"),
											CComVariant(0),
											PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_ALLACCESSPROPERTY,
											VT_I4, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("DirectionDuration"),
											CComVariant(0),
											PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_ALLACCESSPROPERTY,
											VT_I4, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("UpDuration"),
											CComVariant(0),
											PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_ALLACCESSPROPERTY,
											VT_I4, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("ScaleDuration"),
											CComVariant(0),
											PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_ALLACCESSPROPERTY,
											VT_I4, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("PositionDone"),
											CComVariant(bstrPositionDoneDefault),
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_BSTR, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("DirectionDone"),
											CComVariant(bstrDirectionDoneDefault),
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_BSTR, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("UpDone"),
											CComVariant(bstrUpDoneDefault),
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_BSTR, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("ScaleDone"),
											CComVariant(bstrScaleDoneDefault),
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_BSTR, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;
//


 	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("PositionAnim"), 
											CComVariant((IDispatch *)NULL), 
											PSBIT_LOCAL | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_DISPATCH, IID_IDispatch, NULL)))
		goto ERROR_ENCOUNTERED;

 	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("DirectionAnim"), 
											CComVariant((IDispatch *)NULL), 
											PSBIT_LOCAL | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_DISPATCH, IID_IDispatch, NULL)))
		goto ERROR_ENCOUNTERED;

 	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("UpAnim"), 
											CComVariant((IDispatch *)NULL), 
											PSBIT_LOCAL | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_DISPATCH, IID_IDispatch, NULL)))
		goto ERROR_ENCOUNTERED;

 	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("ScaleAnim"), 
											CComVariant((IDispatch *)NULL), 
											PSBIT_LOCAL | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_DISPATCH, IID_IDispatch, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("DurationResetAnim"), 
											CComVariant((IDispatch *)NULL), 
											PSBIT_LOCAL | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_DISPATCH, IID_IDispatch, NULL)))
		goto ERROR_ENCOUNTERED;
//
 	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("NavigationTarget"), 
											CComVariant((bool)VARIANT_FALSE), 
											PSBIT_LOCAL | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_BOOL, IID_IDispatch, NULL)))
		goto ERROR_ENCOUNTERED;

 	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("IsVisible"), 
											CComVariant((bool)VARIANT_TRUE), 
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY, 
											PS_EXEMPLARDEFAULTPROPERTY, 
											VT_BOOL, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

 	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("IsContentsVisible"), 
											CComVariant((bool)VARIANT_TRUE), 
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY, 
											PS_EXEMPLARDEFAULTPROPERTY, 
											VT_BOOL, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = CreateBoundaryList(&pList)))
		goto ERROR_ENCOUNTERED;
 	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("ExternalBoundaryList"), 
											CComVariant((IDispatch *) pList), 
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTCOLLECTION, 
											VT_DISPATCH, IID_IPropertyList, NULL)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

	if (FAILED(hr = CreateBoundaryList(&pList)))
		goto ERROR_ENCOUNTERED;
 	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("BoundaryList"), 
											CComVariant((IDispatch *) pList), 
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_FRIENDLY,
											PS_EXEMPLARDEFAULTCOLLECTION, 
											VT_DISPATCH, IID_IPropertyList, NULL)))
		goto ERROR_ENCOUNTERED;
	pList.Release();
	
	// color & texture persisting helpers
	if (FAILED(hr = CreatePropertyMap(m_pWorld, &pPropMap)))
		goto ERROR_ENCOUNTERED;
 	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("NamedMeshColors"), 
											CComVariant((IDispatch *) pPropMap), 
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTCOLLECTION, 
											VT_DISPATCH, IID_IPropertyMap, NULL)))
		goto ERROR_ENCOUNTERED;

	pPropMap.Release();

	if (FAILED(hr = CreatePropertyMap(m_pWorld, &pPropMap)))
		goto ERROR_ENCOUNTERED;
 	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("NamedMeshTextures"), 
											CComVariant((IDispatch *) pPropMap), 
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTCOLLECTION, 
											VT_DISPATCH, IID_IPropertyMap, NULL)))
		goto ERROR_ENCOUNTERED;

	pPropMap.Release();

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("RotationRate"),
											CComVariant((float) 0.0f),
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_R4, IID_IDispatch, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = CreateVector(0.0f, 1.0f, 0.0f, &pVector)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("RotationAxis"), 
											CComVariant((IDispatch *)pVector), 
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_DISPATCH, IID_IVector, NULL)))
		goto ERROR_ENCOUNTERED;

	pVector.Release();

	// KM: changed to 3 as per content team request
	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("RenderQuality"),
											CComVariant((short) 1),
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_I2, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("ApproachToFront"), 
											CComVariant((bool)VARIANT_TRUE), 
											PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_BOOL, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("ApproachDistance"),
											CComVariant((float) 4.0f),
											PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_R4, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("ApproachRadius"),
											CComVariant((float) 1.0f),
											PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_R4, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

    if (FAILED(hr = pThingEx->AddPropertyExt(CComBSTR("InteractVerb"), 
											CComVariant(CComBSTR("")), 
											PSBIT_LOCAL | PSBIT_COPYONGET | PSBIT_HIDDEN, 
											PS_EXEMPLARDEFAULTPROPERTY, 
											VT_BSTR, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(CComBSTR("Interactee"), 
											CComVariant((IThing*)NULL), 
											PSBIT_LOCAL | PSBIT_COPYONGET | PSBIT_HIDDEN, 
											PS_EXEMPLARDEFAULTPROPERTY, 
											VT_DISPATCH, IID_IThing, NULL)))
		goto ERROR_ENCOUNTERED;

	// sound support props
	/* use the sound property instead */
	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("Sound"), 
											CComVariant((IDispatch *)NULL), 
											PSBIT_COPYONGET | PSBIT_FRIENDLY, 
											PS_EXEMPLARDEFAULTPROPERTY, 
											VT_DISPATCH, IID_ISound, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("ClientSound"), 
											CComVariant((IDispatch *)NULL), 
											PSBIT_NOTREMOTED|PSBIT_COPYONGET |  PSBIT_HIDDEN, 
											PS_EXEMPLARDEFAULTPROPERTY, 
											VT_DISPATCH, IID_ISound, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("IsSolid"), 
											CComVariant((bool)VARIANT_FALSE), 
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_FRIENDLY,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_BOOL, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	// Thing label support properties
	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("Label"),
											CComVariant(CComBSTR("")),
											PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_BSTR, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("LabelPlacement"),
											CComVariant((long) LABEL_ABOVE),
											PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_I4, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("LabelFontName"),
											CComVariant(CComBSTR("Arial")),
											PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_BSTR, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("LabelFontSize"),
											CComVariant((long) 24),
											PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_I4, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("LabelStyle"),
											CComVariant((long) 0),
											PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_I4, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = CreateVector(1.0f, 1.0f, 1.0f, &pVector)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("LabelFontColor"), 
											CComVariant((IDispatch *)pVector), 
											PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_DISPATCH, IID_IVector, NULL)))
		goto ERROR_ENCOUNTERED;
	pVector.Release();
	if (FAILED(hr = CreateVector(0.0f, 0.0f, 0.0f, &pVector)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("LabelShadowColor"), 
											CComVariant((IDispatch *)pVector), 
											PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_DISPATCH, IID_IVector, NULL)))
		goto ERROR_ENCOUNTERED;
	pVector.Release();

	// invoke InitializeGraphics to set default geometry and other props on ourself
    {
	    CComBSTR bstrGeomFile("client\\shared\\default.x");
	    CComVariant varFile(bstrGeomFile);
	    CComVariant varOne(1.0F);
	    CComVariant varZero(0.0F);
	    CComDISPPARAMS dispparams(	7, 
								    varFile, 
								    varZero, varOne, varZero, 
								    varZero, varZero, varOne );
        pThingEx->InvokeMethodExt(CComBSTR("InitializeGraphics"), (DISPPARAMS *) dispparams, NULL);
    }

	// amend edit maps
	bstrGeometry = "Geometry|Edit the geometry of the object|geometry.htm";
	bstrMotion = "Motion|Edit the spinning motion of the object|motion.htm";
	bstrApproach = "Approach|Edit how the object is approached|approach.htm";

	if (FAILED(hr = pThingEx->get_ObjectPropertyExt(CComBSTR("EndUserEditMap"), IID_IPropertyList, (IObjectProperty **) &pMap)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMap->AddString(bstrGeometry)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMap->AddString(bstrMotion)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMap->AddString(bstrApproach)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThingEx->put_ObjectProperty(CComBSTR("EndUserEditMap"), (IObjectProperty *) pMap)))
		goto ERROR_ENCOUNTERED;
	pMap.Release();

	if (FAILED(hr = pThingEx->get_ObjectPropertyExt(CComBSTR("OwnerEditMap"), IID_IPropertyList, (IObjectProperty **) &pMap)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMap->AddString(bstrGeometry)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMap->AddString(bstrMotion)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMap->AddString(bstrApproach)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThingEx->put_ObjectProperty(CComBSTR("OwnerEditMap"), (IObjectProperty *) pMap)))
		goto ERROR_ENCOUNTERED;
	pMap.Release();

	if (FAILED(hr = pThingEx->get_ObjectPropertyExt(CComBSTR("BuilderEditMap"), IID_IPropertyList, (IObjectProperty **) &pMap)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMap->AddString(bstrGeometry)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMap->AddString(bstrMotion)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMap->AddString(bstrApproach)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThingEx->put_ObjectProperty(CComBSTR("BuilderEditMap"), (IObjectProperty *) pMap)))
		goto ERROR_ENCOUNTERED;
	pMap.Release();

	if (FAILED(hr = pThingEx->get_ObjectPropertyExt(CComBSTR("AdvancedEditMap"), IID_IPropertyList, (IObjectProperty **) &pMap)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMap->AddString(bstrGeometry)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMap->AddString(bstrMotion)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMap->AddString(bstrApproach)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThingEx->put_ObjectProperty(CComBSTR("AdvancedEditMap"), (IObjectProperty *) pMap)))
		goto ERROR_ENCOUNTERED;
	pMap.Release();

    return S_OK;

ERROR_ENCOUNTERED:
	
	VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::InstallThingExtensions: Error encountered: %x\n", hr);
    return hr;
}

///////////////////////////////////////////////////////////////////////
// Methods

//******************************************************
// SetCamera
// Sets the camera to this object.

STDMETHODIMP CMultimediaExemplarObject::SetCamera()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// for now, do nothing.  Need to do this with new notification mechanism

	return S_OK;
}

//******************************************************
// Approach
// Moves the thing up to the object, using the navigation tool if possible.

STDMETHODIMP CMultimediaExemplarObject::Approach(IThing *pThing)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    HRESULT                 hr = S_OK;
	CComPtr<IThing>			pThis, pGlobal;
	CComPtr<IVector>		pvPos, pvDir, pvTemp, pvEndPos, pRadiusVector, 
							pvDistance;
	CComPtr<IVWFrame>		pvwfTransform, pvwfParent;
	float					fPosX, fPosY, fPosZ, fDirX, fDirY, fDirZ,
							fEndPosX, fEndPosY, fEndPosZ;
	VARIANT_BOOL			bApproachToFront, bIsAvatar;
	CComVariant				varApproachDistance, varApproachRadius;
	long					nEditingMode;

	static CComBSTR	bstrPosition("Position");
	static CComBSTR	bstrDirection("Direction");
	static CComBSTR	bstrCenterCameraOnObject(VWSTUDIO_CENTERCAMERAONOBJECT_EVENT_STR);
	static CComBSTR	bstrApproach("You approach {object}.|{subject} approaches you.|");
	static CComBSTR	bstrEditingMode("EditingMode");

    if (pThing == NULL)
    {
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::Approach: NULL pointer\n");
        return ReportMMError(E_POINTER);
    }

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;
	if (FAILED(hr = pThis->put_String(CComBSTR("InteractVerb"), bstrApproach)))
		return hr;
	if (FAILED(hr = pThis->put_Thing(CComBSTR("Interactee"), pThing)))
		return hr;

	if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
		return hr;
	
	//If editing, just move the camera directly there.
	if (FAILED(hr = pGlobal->get_Long(bstrEditingMode, &nEditingMode)))
		return hr;

	if (nEditingMode != 0)
	{
		hr = m_pWorld->FireUIEvent(pThis, bstrCenterCameraOnObject, CComVariant(pThing));
		return hr;
	}

	// Get the properties which define the approach.
	if (FAILED(hr = pThing->get_BOOL(CComBSTR("ApproachToFront"), &bApproachToFront)))
		return hr;
	if (FAILED(hr = pThing->get_Property(CComBSTR("ApproachDistance"),
									&varApproachDistance)))
		return hr;

	if (FAILED(hr = varApproachDistance.ChangeType(VT_R4)))
    {
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::Approach: Unable to coerce approach distance to VT_R4\n");
        return ReportMMError(hr);
    }

	if (FAILED(hr = pThing->get_Property(CComBSTR("ApproachRadius"), &varApproachRadius)))
		return hr;
	if (FAILED(hr = varApproachRadius.ChangeType(VT_R4)))
    {
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::Approach: Unable to coerce approach radius to VT_R4\n");
        return ReportMMError(hr);
    }

	// Get the thing to find's position and direction.
	if (FAILED(hr = pThing->get_ObjectPropertyExt(bstrPosition, IID_IVector, (IObjectProperty**)&pvPos)))
		return hr;
    if (pvPos==NULL)
        return S_OK;
	pvPos->get(&fPosX, &fPosY, &fPosZ);

	if (FAILED(hr = pThing->get_ObjectPropertyExt(bstrDirection, IID_IVector, (IObjectProperty**)&pvDir)))
		return hr;
    if (pvDir==NULL)
        return S_OK;
	pvDir->get(&fDirX, &fDirY, &fDirZ);

	// Calculate final position on find spline path.
	if (!bApproachToFront)
	{
		// Just randomize direction from target for now.
		if (FAILED(hr = CreateVector(fDirX, fDirY, fDirZ, &pvTemp)))
    		return hr;

		if (FAILED(hr = pvTemp->Rotate((float) rand(), pvTemp)))
	    	return hr;

		pvTemp->get(&fDirX, &fDirY, &fDirZ);
	}

	fEndPosX = fPosX + (fDirX*varApproachDistance.fltVal);
	fEndPosY = fPosY;
	fEndPosZ = fPosZ + (fDirZ*varApproachDistance.fltVal);
	if (FAILED(hr = CreateVector(fEndPosX, fEndPosY, fEndPosZ, &pvEndPos)))
		return hr;
	
	// Calculate the radius scatter
	varApproachRadius.fltVal *= ((float) (rand() % 1000)) / 1000.0f;
	if (FAILED(hr = CreateVector(varApproachRadius.fltVal, 0.0f, 0.0f, &pRadiusVector)))
		return hr;
	if (FAILED(hr = pRadiusVector->Rotate((float) rand(), pRadiusVector)))
		return hr;

	// Combine the position and radius
	if (FAILED(hr = pvEndPos->Add(pRadiusVector, pvEndPos)))
		return hr;
	if (FAILED(hr = pvEndPos->get(&fEndPosX, &fEndPosY, &fEndPosZ)))
		return hr;

	// If the thing to find is an avatar, we must arrive within its containing
	// boundaries.
	if (FAILED(hr = m_pWorld->IsAvatar(pThing, &bIsAvatar)))
		return hr;
	if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)))
    {
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::Approach: Unable to get the frame\n");
        return ReportMMError(hr);
    }

	if (pvwfTransform)
    {
		if (FAILED(hr = pvwfTransform->GetParent(&pvwfParent)))
	    	return hr;
    }

	if (bIsAvatar && pvwfParent)
	{
		if (FAILED(hr = FindZeroIntersectPoint(pvwfParent, &fPosX, &fPosY, &fPosZ, &fEndPosX, &fEndPosY, &fEndPosZ)))
        {
    		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::Approach: FindZeroIntersectPoint failed\n");
	    	return ReportMMError(hr);
        }
		if (FAILED(hr = pvEndPos->set(fEndPosX, fEndPosY, fEndPosZ)))
		    return hr;
	}
	
	if (fEndPosX == fPosX && fEndPosZ == fPosZ)
		hr = ApproachLocation(fEndPosX, fEndPosY, fEndPosZ, -fDirX, 0.0F, -fDirZ);
	else
		hr = ApproachLocation(fEndPosX, fEndPosY, fEndPosZ, fPosX - fEndPosX, 0.0F, fPosZ - fEndPosZ);

    return hr;
}

//******************************************************
// Approach
// Moves the thing to the position/direction, using the navigation tool if possible.

STDMETHODIMP CMultimediaExemplarObject::ApproachLocation(float fPosX, float fPosY, float fPosZ, float fDirX, float fDirY, float fDirZ)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IPropertyMap>	pmapPositions, pmapDirections;
	CComPtr<IThing>			pThis;
	CComPtr<IVector>		pvPos, pvStartPos, pvStartDir, pvDistance;
	CComPtr<IVWFrame>		pvwfTransform, pvwfParent;
	CComPtr<IVWNavigationTool> pNavTool;
	float					fStartPosX, fStartPosY, fStartPosZ, fDistance;
	char					buff[30];
	COleVariant				var;
	CComBSTR				strFindSplineTime1;
	VARIANT_BOOL			bNavTarget;
	int						iTime;
	D3DVECTOR				rlvDir = {fDirX, fDirY, fDirZ};
    HRESULT                 hr = S_OK;

	static CComBSTR	bstrPosition("Position");
	static CComBSTR	bstrDirection("Direction");
	static CComBSTR	bstrPositionDuration("PositionDuration");
	static CComBSTR	bstrDirectionDuration("DirectionDuration");
	static CComBSTR	bstrDirectionDone("DirectionDone");
	static CComBSTR	bstrNavigationTarget("NavigationTarget");

    if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	// Get this thing's position.
	if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)))
    {
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::ApproachLocation: Unable to get the frame\n");
        return ReportMMError(hr);
    }

	if (pvwfTransform)
	{
		if (FAILED(hr = pvwfTransform->GetParent(&pvwfParent)))
		    return hr;
		if (pvwfParent==NULL)
            return S_OK;

		if (FAILED(hr = pvwfTransform->GetPositionEx(pvwfParent, &fStartPosX, &fStartPosY, &fStartPosZ)))
    		return hr;
		if (FAILED(hr = CreateVector(fStartPosX, fStartPosY, fStartPosZ, &pvStartPos)))
	    	return hr;
	}
	else
	{
		if (FAILED(hr = pThis->get_ObjectPropertyExt(bstrPosition, IID_IVector, (IObjectProperty**)&pvStartPos)) || !pvStartPos)
		    return hr;

		if (FAILED(hr = pvStartPos->get(&fStartPosX, &fStartPosY, &fStartPosZ)))
			return hr;
	}
			
	// How far are we moving?
	if (FAILED(hr = CreateVector(0.0, 0.0, 0.0, &pvDistance)))
		return hr;
	if (FAILED(hr = CreateVector(fPosX, fPosY, fPosZ, &pvPos)))
		return hr;
	if (FAILED(hr = pvPos->Subtract(pvStartPos, pvDistance)))
		return hr;
	if (FAILED(hr = pvDistance->Length(&fDistance)))
		return hr;
	iTime = (int)(fDistance*iApproachTimePerUnit);

	// If not navigation target just do an interpolated move.
	if (FAILED(hr = pThis->get_BOOL(bstrNavigationTarget, &bNavTarget)))
		return hr;
	if (!bNavTarget)
	{
		if (pvwfTransform)
		{
			pvStartPos.Release();
			if (FAILED(hr = pThis->get_ObjectPropertyExt(bstrPosition, IID_IVector, (IObjectProperty**)&pvStartPos)) || !pvStartPos)
        		return hr;
		}

        if (FAILED(hr = pThis->get_ObjectPropertyExt(bstrDirection, IID_IVector, (IObjectProperty**)&pvStartDir)) || !pvStartDir)
		    return hr;
		if (FAILED(hr = pThis->put_Long(bstrPositionDuration, iTime)))
		    return hr;
		if (FAILED(hr = pThis->put_Long(bstrDirectionDuration, iTime)))
		    return hr;
		if (FAILED(hr = pThis->put_String(bstrDirectionDone, bstrOnApproachDone)))
		    return hr;
		if (FAILED(hr = pvStartPos->set(fPosX, fPosY, fPosZ)))
		    return hr;
        D3DRMVectorNormalize(&rlvDir);

        if (FAILED(hr = pvStartDir->set(rlvDir.x, rlvDir.y, rlvDir.z)))
		    return hr;
	
        return S_OK;
	}

	// Property maps to pass to nav tool.
	if (FAILED(hr = CreatePropertyMap(m_pWorld, &pmapPositions)))
        return ReportMMError(hr);
	if (FAILED(hr = CreatePropertyMap(m_pWorld, &pmapDirections)))
		return ReportMMError(hr);

	// Put calcs into property maps.
	strFindSplineTime1 = itoa(iTime, buff, 10);
	if (FAILED(hr = pmapPositions->put_ObjectProperty(strFindSplineTime1, pvPos)))
		return hr;
	var.vt = VT_R4;
	ConvertDirToRotation(fDirX, fDirY, fDirZ, var.fltVal);
	if (FAILED(hr = pmapDirections->put_Property(strFindSplineTime1, var)))
		return hr;

	// Tell navigation tool to follow the defined spline path.
	if (FAILED(hr = m_pWorld->get_Tool(CComBSTR("Navigate"), (IUnknown**)&pNavTool)))
		return hr;
	if (pNavTool)
		pNavTool->SetNavigationPath(pmapPositions, pmapDirections, bstrOnApproachDone);

	return hr;
}

//******************************************************
// UpdateIsApproachable - used to be IsArtifactOrAvatar,
// now includes portals

STDMETHODIMP CMultimediaExemplarObject::UpdateIsApproachable(IMenuItem *pmi)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pTarget;
	VARIANT_BOOL boolIsArtifact = VARIANT_FALSE;
	VARIANT_BOOL boolIsAvatar = VARIANT_FALSE;
	VARIANT_BOOL boolIsPortal = VARIANT_FALSE;
	VARIANT_BOOL boolIsLight = VARIANT_FALSE;

	long lFlags;
    HRESULT hr = S_OK;

    if (pmi == NULL)
    {
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::UpdateIsApproachable: NULL pointer\n");
        return ReportMMError(E_POINTER);
    }

	// Get the this object
	if (FAILED(hr = pmi->get_Target(&pTarget)))
		return hr;

	// If there's no target, use the owner
	if (!pTarget)
    {
		if (FAILED(hr = pmi->get_Owner(&pTarget)))
	    	return hr;
    }

	// Check if it's an Artifact
	if (FAILED(hr = pTarget->IsOfType(CComBSTR("Artifact"), &boolIsArtifact)))
		return hr;

	// If not, check for Avatar and Portal and Light
	if (boolIsArtifact!=VARIANT_TRUE)
	{
		if (FAILED(hr = m_pWorld->IsAvatar(pTarget, &boolIsAvatar)))
    		return hr;

		if (boolIsAvatar!=VARIANT_TRUE)
        {
			if (FAILED(hr = pTarget->IsOfType(CComBSTR("Portal"), &boolIsPortal)))
		        return hr;

			if (boolIsPortal!=VARIANT_TRUE)
			{
				if (FAILED(hr = pTarget->IsOfType(CComBSTR("Light"), &boolIsLight)))
		        return hr;
			}

		}


	}

	// Get and set the invisible flag as indicated
	if (FAILED(hr = pmi->get_Flags(&lFlags)))
		return hr;

	if (boolIsArtifact==VARIANT_TRUE || boolIsAvatar==VARIANT_TRUE || boolIsPortal==VARIANT_TRUE || boolIsLight == VARIANT_TRUE)
		lFlags &= ~MI_INVISIBLE;
	else
		lFlags |= MI_INVISIBLE;

	hr = pmi->put_Flags(lFlags);

	return hr;
}


//******************************************************
// PositionOrientationHeartbeat
// REVIEW: Hmm - not sure what is happening here - I guess this
// is used for continuous movement

STDMETHODIMP CMultimediaExemplarObject::PositionOrientationHeartbeat(IThing *pThing)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    HRESULT             hr = S_OK;
	CComPtr<IVector>	pvPos, pvDir;
	float				fPosx, fPosy, fPosz, fDirx, fDiry, fDirz;
	float				fTPosx, fTPosy, fTPosz, fTDirx, fTDiry, fTDirz, fTUpx, fTUpy, fTUpz;
	CComPtr<IVWFrame>	pvwfTransform, pvwfParent;
	static CComBSTR		bstrPosition("Position");
	static CComBSTR		bstrDirection("Direction");

    if (pThing == NULL)
    {
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::PositionOrientationHeartbeat: NULL pointer\n");
        return ReportMMError(E_POINTER);
    }

	// Get our current position from our loaded geometry since this is being updated
	// separately from our world position.
	if (FAILED(hr = _GetFrameHelper(pThing, &pvwfTransform)))
    {
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::PositionOrientationHeartbeat: Error getting the frame\n");
        return ReportMMError(hr);
    }

	if (pvwfTransform==NULL)
		return S_OK;

	if (FAILED(hr = pvwfTransform->GetParent(&pvwfParent)))
		return hr;
	if (pvwfParent==NULL)
		return S_OK;

	if (FAILED(hr = pvwfTransform->GetPositionEx(pvwfParent, &fTPosx, &fTPosy, &fTPosz)))
		return hr;

	// Get our world position.
	if (FAILED(hr = pThing->get_ObjectProperty(bstrPosition, (IObjectProperty**)&pvPos)) || !pvPos)
		return hr;
	pvPos->get(&fPosx, &fPosy, &fPosz);

	// Is it out of sync?
	if (fPosx != fTPosx ||
		fPosy != fTPosy ||
		fPosz != fTPosz)
	{
		// Sync up with the geometry position.
		pvPos->set(fTPosx, fTPosy, fTPosz);
	}

	// Get our current direction from our loaded geometry since this is being updated
	// separately from our world direction.
	if (FAILED(hr = pvwfTransform->GetOrientationEx(pvwfParent, &fTDirx, &fTDiry, &fTDirz, &fTUpx, &fTUpy, &fTUpz)))
		return hr;

	// Get our world direction.
	if (FAILED(hr = pThing->get_ObjectProperty(bstrDirection, (IObjectProperty**)&pvDir)) || !pvDir)
		return hr;
	pvDir->get(&fDirx, &fDiry, &fDirz);

	// Is it out of sync?
	if (fDirx != fTDirx ||
		fDiry != fTDiry ||
		fDirz != fTDirz)
	{
		// Sync up with the geometry direction.
		pvDir->set(fTDirx, fTDiry, fTDirz);
	}

    return hr;
}


//******************************************************
// OnPositionChanged
// Updates geo for new position property

STDMETHODIMP CMultimediaExemplarObject::OnPositionChanged(long lHint, VARIANT varHintData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IVWFrame>			pvwfTransform, pvwfParent;
	CComPtr<IThing>				pThis;
	CComPtr<IVector>			pvPos;
 	long						lPositionDuration;
	VARIANT_BOOL				bNavTarget, bServerSide;
	float						fPosX, fPosY, fPosZ;
    HRESULT                     hr = S_OK;

	static CComBSTR	bstrPosition("Position");
	static CComBSTR	bstrNavigationTarget("NavigationTarget");
 	static CComBSTR	bstrPositionDuration("PositionDuration");
 	static CComBSTR	bstrOnClientPositionChanged("OnClientPositionChanged");

	if (lHint == thAdd)
		// Just adding property to thing - ignore.
		return S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;


	// Is this thing navigation target?
	if (FAILED(hr = pThis->get_BOOL(bstrNavigationTarget, &bNavTarget)))
		return hr;

	// Ignore changed event if thing's geometry frame is being navigated.
	if (bNavTarget!=VARIANT_TRUE)
	{

		if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)))
        {
		    VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::PositionOrientationHeartbeat: Error getting the frame\n");
            return ReportMMError(hr);
        }
 		if (FAILED(hr = pThis->get_Long(bstrPositionDuration, &lPositionDuration)))
		    return hr;
 		if (lPositionDuration > 0)
 		{
 			// An interpolation is required.
			if (FAILED(hr = StartD3DInterpolation(bstrPosition, lPositionDuration)))
		        return hr;
 		}
 		else if (pvwfTransform)
 		{
 			// An explicit set position is required.
			if (FAILED(hr = pvwfTransform->GetParent(&pvwfParent)))
		        return hr;
 			if (FAILED(hr = pThis->get_ObjectProperty(bstrPosition, (IObjectProperty**)&pvPos)) || !pvPos)
		        return hr;
            pvPos->get(&fPosX, &fPosY, &fPosZ);

            if (FAILED(hr = pvwfTransform->SetPositionEx(pvwfParent, fPosX, fPosY, fPosZ)))
		        return hr;
		}

		if (FAILED(hr = m_pWorld->get_ServerSide(&bServerSide)))
			return hr;

		if (bServerSide)
		{
			hr = pThis->InvokeMethodExt(bstrOnClientPositionChanged, 
										   CComDISPPARAMS(2, CComVariant(lHint), varHintData), 
										   NULL);
			return hr;
		}
	}
	

    return hr;
}

//******************************************************
// OnDirectionChanged
// Updates for new Direction

STDMETHODIMP CMultimediaExemplarObject::OnDirectionChanged(long lHint, VARIANT varHintData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IVWFrame>			pvwfTransform, pvwfParent;
	CComPtr<IThing>				pThis;
	CComPtr<IVector>			pvDir;
 	long						lDirectionDuration;
	VARIANT_BOOL				bNavTarget, bServerSide;
	float						fDirX, fDirY, fDirZ, fOldDirX, fOldDirY, fOldDirZ, fUpX, fUpY, fUpZ;
    HRESULT                     hr = S_OK;

	static CComBSTR	bstrDirection("Direction");
	static CComBSTR	bstrNavigationTarget("NavigationTarget");
 	static CComBSTR	bstrDirectionDuration("DirectionDuration");
 	static CComBSTR	bstrOnClientDirectionChanged("OnClientDirectionChanged");

	if (lHint == thAdd)
		// Just adding property to thing - ignore.
		return S_OK;


	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	if (FAILED(hr = m_pWorld->get_ServerSide(&bServerSide)))
		return hr;

    if (bServerSide)
	{
		hr = pThis->InvokeMethodExt(bstrOnClientDirectionChanged, 
									   CComDISPPARAMS(2, CComVariant(lHint), varHintData), 
									   NULL);
		return hr;
	}

	// Is this thing navigation target?
	if (FAILED(hr = pThis->get_BOOL(bstrNavigationTarget, &bNavTarget)))
		return hr;

	// Ignore changed event if thing's geometry frame is being navigated.
	if (bNavTarget!=VARIANT_TRUE)
	{
		if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)))
        {
		    VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::OnDirectionChanged: Error getting the frame\n");
            return ReportMMError(hr);
        }
 		if (FAILED(hr = pThis->get_Long(bstrDirectionDuration, &lDirectionDuration)))
		    return hr;
 		if (lDirectionDuration > 0)
 		{
 			// An interpolation is required.
			if (FAILED(hr = StartD3DAnimation(bstrDirection, lDirectionDuration)))
		        return hr;
 		}
 		else if (pvwfTransform)
 		{
 			// An explicit set orientation is required.
 			if (FAILED(hr = pvwfTransform->GetParent(&pvwfParent)))
		        return hr;
 			if (FAILED(hr = pThis->get_ObjectProperty(bstrDirection, (IObjectProperty**)&pvDir)) || !pvDir)
		        return hr;
			pvDir->get(&fDirX, &fDirY, &fDirZ);

			if (FAILED(hr = pvwfTransform->GetOrientationEx(pvwfParent, &fOldDirX, &fOldDirY, &fOldDirZ, &fUpX, &fUpY, &fUpZ)))
		        return hr;
			if (FAILED(hr = pvwfTransform->SetOrientationEx(pvwfParent, fDirX, fDirY, fDirZ, fUpX, fUpY, fUpZ)))
		        return hr;
		}
	}

    return hr;
}

//******************************************************
// OnUpChanged
// Updates for new up vector

STDMETHODIMP CMultimediaExemplarObject::OnUpChanged(long lHint, VARIANT varHintData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IVWFrame>			pvwfTransform, pvwfParent;
	CComPtr<IThing>				pThis;
	CComPtr<IVector>			pvUp;
	VARIANT_BOOL				bServerSide;
	float						fOldUpX, fOldUpY, fOldUpZ, fDirX, fDirY, fDirZ, fUpX, fUpY, fUpZ;
 	long						lUpDuration;
    HRESULT                     hr = S_OK;

	static CComBSTR	bstrUp("Up");
 	static CComBSTR	bstrOnClientUpChanged("OnClientUpChanged");
 	static CComBSTR	bstrUpDuration("UpDuration");

	if (lHint == thAdd)
		// Just adding property to thing - ignore.
		return S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	if (FAILED(hr = m_pWorld->get_ServerSide(&bServerSide)))
		return hr;

    if (bServerSide)
	{
		hr = pThis->InvokeMethodExt(bstrOnClientUpChanged, 
									   CComDISPPARAMS(2, CComVariant(lHint), varHintData), 
									   NULL);
		return hr;
	}

	if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)))
    {
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::OnUpChanged: Error getting the frame\n");
        return ReportMMError(hr);
    }
 	if (FAILED(hr = pThis->get_Long(bstrUpDuration, &lUpDuration)))
		return hr;

 	if (lUpDuration > 0)
 	{
 		// A DXAnimation is required.
		if (FAILED(hr = StartD3DAnimation(bstrUp, lUpDuration)))
	    	return hr;
 	}
 	else if (pvwfTransform)
 	{
 		// An explicit set orientation is required.
 		if (FAILED(hr = pvwfTransform->GetParent(&pvwfParent)))
    		return hr;
 		if (FAILED(hr = pThis->get_ObjectProperty(bstrUp, (IObjectProperty**)&pvUp))|| !pvUp)
	    	return hr;

		pvUp->get(&fUpX, &fUpY, &fUpZ);

        if (FAILED(hr = pvwfTransform->GetOrientationEx(pvwfParent, &fDirX, &fDirY, &fDirZ, &fOldUpX, &fOldUpY, &fOldUpZ)))
		    return hr;
		if (FAILED(hr = pvwfTransform->SetOrientationEx(pvwfParent, fDirX, fDirY, fDirZ, fUpX, fUpY, fUpZ)))
		    return hr;
	}

	if (FAILED(hr = m_pWorld->get_ServerSide(&bServerSide)))
		return hr;

	if (bServerSide)
		hr = pThis->InvokeMethodExt(bstrOnClientUpChanged, 
									   CComDISPPARAMS(2, CComVariant(lHint), varHintData), 
									   NULL);
    return hr;
}

//******************************************************
// OnScaleChanged
// Updates for new scale

STDMETHODIMP CMultimediaExemplarObject::OnScaleChanged(long lHint, VARIANT varHintData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IVWFrame>			pvwfTransform, pvwfParent;
	CComPtr<IThing>				pThis;
	CComPtr<IVector>			pvScale;
	VARIANT_BOOL				bServerSide;
	float						fScaleX, fScaleY, fScaleZ;
 	long						lScaleDuration;
    HRESULT                     hr = S_OK;

	static CComBSTR	bstrScale("Scale");
 	static CComBSTR	bstrOnClientScaleChanged("OnClientScaleChanged");
 	static CComBSTR	bstrScaleDuration("ScaleDuration");

	if (lHint == thAdd)
		// Just adding property to thing - ignore.
		return S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	if (FAILED(hr = m_pWorld->get_ServerSide(&bServerSide)))
		return hr;

    if (bServerSide)
	{
		hr = pThis->InvokeMethodExt(bstrOnClientScaleChanged, 
									   CComDISPPARAMS(2, CComVariant(lHint), varHintData), 
									   NULL);
		return hr;
	}

	if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)))
    {
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::OnScaleChanged: Error getting the frame\n");
        return ReportMMError(hr);
    }
 	if (FAILED(hr = pThis->get_Long(bstrScaleDuration, &lScaleDuration)))
		return hr;
 	if (lScaleDuration > 0)
 	{
 		// An interpolation is required.
		if (FAILED(hr = StartD3DAnimation(bstrScale, lScaleDuration)))
		    return hr;
 	}
 	else if (pvwfTransform)
 	{
		// An explicit SetScale is required.
		if (FAILED(hr = pvwfTransform->GetParent(&pvwfParent)))
		    return hr;
 		if (FAILED(hr = pThis->get_ObjectProperty(bstrScale, (IObjectProperty**)&pvScale)) || !pvScale)
		    return hr;

		pvScale->get(&fScaleX, &fScaleY, &fScaleZ);
        
        if (FAILED(hr = pvwfTransform->SetScaleEx(pvwfParent, fScaleX, fScaleY, fScaleZ)))
		    return hr;
	}
	UpdateLabel(pThis);

	return hr;
}

//******************************************************
// OnBoundaryListChanged
// 

STDMETHODIMP CMultimediaExemplarObject::OnBoundaryListChanged(long lHint, VARIANT varHintData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing>			pThis;
	CComPtr<IPropertyList>	pCell;
	CComPtr<IVWGeometry>	pVWGeometry;
	CComPtr<IVWFrame>		pVWFrame;
	CComPtr<IBoundary>		pBoundary;
    HRESULT                 hr = S_OK;
	VARIANT_BOOL			vbSolid, vbIsAutoSolidBoundary;
	long					nCount, i;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	if (FAILED(hr = pThis->get_ObjectProperty(bstrBoundaryList,(IObjectProperty**) &pCell)))
		return hr;

	if (pCell == NULL)
		return S_OK;

	if (lHint == thAdd || lHint == thPut )
	{
        // REVIEW: why do we retrieve this property?  We don't use it in this method.
		if (FAILED(hr = pThis->get_ObjectProperty(CComBSTR("Geometry"),(IObjectProperty**) &pVWGeometry)))
		    return hr;
		if (FAILED(hr = _GetFrameHelper(pThis, &pVWFrame)))
        {
		    VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::OnScaleChanged: Error getting the frame\n");
            return ReportMMError(hr);
        }

		if (pVWFrame==NULL)
			return S_OK;

		if (FAILED(hr = pVWFrame->put_BoundaryList(pCell)))
		    return hr;
	}
	else if (lHint == plPut)
	{
		long nArg = ( ((unsigned long) varHintData.lVal) << 8) + BOUNDARYUPDATE_UPDATEINFO;
		if (FAILED(hr = m_pWorld->FireUIEvent(pThis, bstrBoundaryUpdate, CComVariant( nArg ))))
		    return hr;
	}
	else if (lHint == plAdd || lHint == plInsertAt) // || lHint == plRemove || lHint == plRemoveAt)
	{
		//Fire UIEvent to notify Boundary editor that a boundary has changed.
		long nArg = ( ((unsigned long) varHintData.lVal) << 8) + BOUNDARYUPDATE_ADD;
		hr = m_pWorld->FireUIEvent(pThis, bstrBoundaryUpdate, CComVariant( nArg ));

//			if (FAILED(hr = m_pWorld->FireUIEvent(pThis, bstrBoundaryUpdate, CComVariant(pBoundary) )))
//      		goto ERROR_ENCOUNTERED;
	}
	else  //Doing a remove boundary
	{
		//Check if there's any solid boundaries.  If there isn't, set IsSolid property to false.
		if (FAILED(hr = pThis->get_BOOL(bstrSolid, &vbSolid)))
			return hr;

		vbIsAutoSolidBoundary = VARIANT_FALSE;
		if (vbSolid == VARIANT_TRUE)
		{
			if (FAILED(hr = pCell->get_Count(&nCount)))
				return hr;

			for (i = 0; i < nCount; i++)
			{
				CComPtr<IBoundary>		pTmpBoundary;

				if (FAILED(hr = pCell->get_ObjectProperty(i, (IObjectProperty**) &pTmpBoundary.p)))
					return hr;

				if (pTmpBoundary == NULL)
					continue;

				if (FAILED(hr = pTmpBoundary->get_IsAutoSolidBoundary(&vbIsAutoSolidBoundary)))
					return hr;

				if (vbIsAutoSolidBoundary)
					break;
			}

			if (vbIsAutoSolidBoundary == VARIANT_FALSE)
			{
				if (FAILED(hr = pThis->put_BOOL(bstrSolid, VARIANT_FALSE)))
					return hr;
			}
		}

		//Finally set update event for boundary editor.
		long nArg = ( ((unsigned long) varHintData.lVal) << 8) + BOUNDARYUPDATE_REMOVE;
		hr = m_pWorld->FireUIEvent(pThis, bstrBoundaryUpdate, CComVariant( nArg ));
	}

#ifdef _DEBUG
	//This code ASSERT if there's more than one IsSolid boundary.
	CComPtr<IPropertyList>	pBoundaryList;
	long					nNumSolid;

	vbIsAutoSolidBoundary = VARIANT_FALSE;
	nNumSolid = 0;

	if (FAILED(hr = pThis->get_ObjectProperty(bstrBoundaryList, (IObjectProperty**) &pBoundaryList.p)))
		return hr;

	if (FAILED(hr = pBoundaryList->get_Count(&nCount)))
		return hr;

	for (i = 0; i < nCount; i++)
	{
		CComPtr<IBoundary> pBoundary;

		if (FAILED(hr = pBoundaryList->get_ObjectProperty(i, (IObjectProperty**) &pBoundary.p)))
			return hr;

		if (pBoundary == NULL)
			continue;

		if (FAILED(hr = pBoundary->get_IsAutoSolidBoundary(&vbIsAutoSolidBoundary)))
			return hr;

		if (vbIsAutoSolidBoundary)
			nNumSolid++;
	}

	ASSERT(nNumSolid <= 1);
#endif

	return hr;
}


STDMETHODIMP CMultimediaExemplarObject::OnIsConnectedChanged(long lHint, VARIANT varHintData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT					hr;
	CComPtr<IThing>			pThis;
	CComPtr<IVWGeometry>	pVWGeometry;
	VARIANT_BOOL			bVisible;
	VARIANT_BOOL			bIsConnected = VARIANT_FALSE;

	static CComBSTR bstrVisible("IsVisible");
	static CComBSTR	bstrGeometry("Geometry");
	static CComBSTR	bstrClientGeometry("ClientGeometry");
	static CComBSTR bstrIsConnected("IsConnected");

	if (lHint == thRemove || lHint == thPut || lHint == thAdd)
	{
		if (FAILED(hr = m_pWorld->get_This(&pThis)))
    		return hr;
		
		// Is the thing's geometry occluded?
		if (FAILED(hr = pThis->get_BOOL(bstrVisible, &bVisible)))
	    	return hr;

		// if so, we don't need to muck with it
		if (bVisible==VARIANT_FALSE)
			return S_OK;

		// Is the thing connected?
		hr = pThis->get_BOOL(bstrIsConnected, &bIsConnected);
		if (FAILED(hr))
		{
			// ignore property not found error, will happen on thRemove
			if (hr != VWOBJECT_E_PROPERTYNOTEXIST)
				return hr;
		}

		// Some things may just not have a geometry property, or there may be something
		// wrong with this thing's geomerty property. Either way we should not fail here.
		// Get the thing's ClientGeometry property (if it has one).
		if (FAILED(pThis->get_ObjectPropertyExt(bstrClientGeometry, IID_IVWGeometry, (IObjectProperty**)&pVWGeometry)) || !pVWGeometry)
		{
			// Get the thing's geometry property (if it has one).
			if (FAILED(pThis->get_ObjectPropertyExt(bstrGeometry, IID_IVWGeometry, (IObjectProperty**)&pVWGeometry)) || !pVWGeometry)
				return S_OK;
		}

		if (bIsConnected==VARIANT_TRUE)
			bVisible = VARIANT_TRUE;
		else
			bVisible = VARIANT_FALSE;
		
        hr = pVWGeometry->put_Occluded(!bVisible);
	}

	return hr;
}

//******************************************************
// OnIsMoveableChanged
// Handles the geometry being movealb changing

STDMETHODIMP CMultimediaExemplarObject::OnIsMoveableChanged(long lHint, VARIANT varHintData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing>			pThis;
    HRESULT                 hr = S_OK;

	if (lHint != thPut)
		return S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;
		
	hr = m_pWorld->FireUIEvent(pThis, CComBSTR(VWSTUDIO_ISMOVEABLECHANGED_EVENT_STR), CComVariant());

	return hr;
}


//******************************************************
// OnVisibleChanged
// Handles the geometry being occluded..

STDMETHODIMP CMultimediaExemplarObject::OnVisibleChanged(long lHint, VARIANT varHintData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing>			pThis;
	CComPtr<IVWGeometry>	pVWGeometry;
	CComPtr<IVWFrame>		pvwfGeometry, pvwfTransform;
	VARIANT_BOOL			bVisible;
    HRESULT                 hr = S_OK;

	static CComBSTR bstrVisible("IsVisible");
	static CComBSTR	bstrGeometry("Geometry");
	static CComBSTR bstrOnLoadGeometry("OnLoadGeometry");
	static CComBSTR bstrRenderQuality("RenderQuality");

	// only listen to put notifications
	if (lHint == thPut)
	{
		if (FAILED(hr = m_pWorld->get_This(&pThis)))
		    return hr;
		
		// Is the thing's geometry occluded?
		if (FAILED(hr = pThis->get_BOOL(bstrVisible, &bVisible)))
		    return hr;

		// Some things may just not have a geometry property, or there may be something
		// wrong with this thing's geomerty property. Either way we should not fail here.
		// Get the thing's geometry property (if it has one).
		if (FAILED(hr = pThis->get_ObjectPropertyExt(bstrGeometry, IID_IVWGeometry, (IObjectProperty**)&pVWGeometry)))
		    return hr;

		if (pVWGeometry==NULL)
			return S_OK;

		if (FAILED(hr = pVWGeometry->put_Occluded(!bVisible)))
		    return hr;

		if (bVisible)
		{
			// Invoke OnLoadGeometry.
			if (FAILED(hr = pVWGeometry->get_GeometryFrame(&pvwfGeometry)))
		        return hr;

			pThis->InvokeMethodExt(bstrOnLoadGeometry,
			    				   CComDISPPARAMS(1, CComVariant((IDispatch*)pvwfGeometry)),
				    			   NULL);

			UpdateLabel(pThis);

			//Now set the mesh quality
			if (pvwfGeometry)
			{
				short sRenderQuality;

				if (FAILED(hr = pThis->get_Short(bstrRenderQuality, &sRenderQuality)))
					return hr;

				//Ingore HRESULT on SetMeshQuality because it might be OCCLUDED.
				pvwfGeometry->SetMeshQuality(VWFRAME_ALLHIERARCHY, sRenderQuality);
			}
		}

		hr = m_pWorld->FireUIEvent(pThis, CComBSTR(VWSTUDIO_VISIBILITYCHANGED_EVENT_STR), CComVariant((bool)bVisible));
	}

    return hr;
}

//******************************************************
// OnGeometryChanged
// Handles the geometry property being changed.

STDMETHODIMP CMultimediaExemplarObject::OnGeometryChanged(long lHint, VARIANT varHintData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IVWGeometry>	pNewVWGeometry, pVWGeometry;
	CComPtr<IThing>			pThis, pThing, pContainer;
	CComPtr<IVWFrame>		pvwfTransform, pvwfNewTransform, pvwfGeometry, pvwfNewGeometry, pvwFrame;
	CComPtr<IPropertyList>	pList;
	long					lChildren, lContents, l;
	D3DRMMATRIX4D			d3drmmtx;
	CComPtr<IDirect3DRMFrame> pd3drmf;
	VARIANT_BOOL			bVisible, bContentsVisible, bClientGeometry = VARIANT_FALSE;
    HRESULT                 hr = S_OK;
	short						sRenderQuality;
	static CComBSTR bstrRenderQuality("RenderQuality");
	static CComBSTR	bstrGeometry("Geometry");
	static CComBSTR	bstrClientGeometry("ClientGeometry");
	static CComBSTR bstrVisible("IsVisible");
	static CComBSTR bstrContentsVisible("IsContentsVisible");
	static CComBSTR bstrOnLoadGeometry("OnLoadGeometry");

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

	// Get the thing's ClientGeometry property (if it has one).
	if (FAILED(pThis->get_ObjectProperty(bstrClientGeometry, (IObjectProperty**)&pNewVWGeometry)) || !pNewVWGeometry)
	{
		// Get the thing's geometry property (if it has one).
		if (FAILED(hr = pThis->get_ObjectProperty(bstrGeometry, (IObjectProperty**)&pNewVWGeometry)))
		    return hr;
		if (pNewVWGeometry==NULL)
			return S_OK;
	}
	else
		bClientGeometry = VARIANT_TRUE;

	// mostly only listen to put notifications
	if (lHint == thPut || lHint == thAdd || lHint == thRemove)
	{
		// Get my container thing.
		if (FAILED(hr = pThis->get_Container(&pContainer)))
	    	return hr;
    
		if (!pContainer)
		{
			// This is the root level thing, we will have to use it's contents.
			if (FAILED(hr = pThis->get_Contents(&pList)))
		        return hr;
		
            pList->get_Count(&lContents);

			for (l = 0; l < lContents; l++)
			{
				if (FAILED(hr = pList->get_ObjectProperty(l, (IObjectProperty**)&pThing)))
		            return hr;
				if (FAILED(hr = _GetFrameHelper(pThing, &pvwFrame)))
                {
		            VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::OnGeometryChanged: Error getting the frame\n");
                    return ReportMMError(hr);
                }
				if (pvwFrame)
				{
					// First content we find with loaded geometry will do just fine.
					if (FAILED(hr = pvwFrame->GetParent(&pvwfTransform)))
		                return hr;
					break;
				}
				pvwFrame.Release();
				pThing.Release();
			}
		}
		else
		{
			// If the container's contents is occluded, we assume this geometry was never
			// loaded and do nothing.
			if (FAILED(hr = pContainer->get_BOOL(bstrContentsVisible, &bContentsVisible)))
		        return hr;
			if (!bContentsVisible)
				return S_OK;
			
			// Get the container's geometry property (if it has one).
			if (FAILED(hr = _GetFrameHelper(pContainer, &pvwFrame)))
            {
		        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::OnGeometryChanged: Error getting the frame\n");
                return ReportMMError(hr);
            }
			if (pvwFrame==NULL)
				return S_OK;

			// Cycle through this frame's children looking for a frame whose thing == this.
			if (FAILED(hr = pvwFrame->GetVWFChildren(&pList)))
		        return hr;
            
            pList->get_Count(&lChildren);

            for (l = 0; l < lChildren; l++)
			{
				if (FAILED(hr = pList->get_ObjectProperty(l, (IObjectProperty**)&pvwfTransform)) || !pvwfTransform)
		            return hr;
				if (FAILED(hr = pvwfTransform->get_Thing(&pThing)))
		            return hr;
				if (pThing == pThis)
					break;
				pvwfTransform.Release();
				pThing.Release();
			}
		}

		// Is the thing's geometry occluded?
		if (FAILED(hr = pThis->get_BOOL(bstrVisible, &bVisible)))
		    return hr;

		// Set occluded state before we load the geometry.
		if (FAILED(hr = pNewVWGeometry->put_Occluded(!bVisible)))
		    return hr;

		if (!pvwfTransform)
		{
			// This cannot be the root level thing if it has no transform.
			// Also, don't create new geometry for client geometry properties.
			if (pvwFrame && !bClientGeometry)
			{
				// Hmmm, no transform. Most likely this is the first geometry this object 
				// has had. We'll create it.
				if (FAILED(hr = pNewVWGeometry->ComposeGeometry(&pvwfTransform)))
		            return hr;
				
				if (FAILED(hr = pvwFrame->AddChild(pvwfTransform)))
		            return hr;
				if (FAILED(hr = OrientAndScaleTransform(pThis, pvwFrame, pvwfTransform)))
		            return hr;
			}
			// OK, we're done.
			return S_OK;
		}

		// Get the GeometryObject which created this transform, if it was created by 
		// a geometry object.
		if (FAILED(hr = pvwfTransform->get_GeometryObject(&pVWGeometry)))
		    return hr;
		if (pVWGeometry)
		{
			// Easier to use last geometry object to get the geometry frame.
			pVWGeometry->get_GeometryFrame(&pvwfGeometry);
			// Allow the geometry object to clean itself up.
			pVWGeometry->ReleaseGeometry();
		}
		else
		{
			// Cycle through the Transform frame's children looking for a frame whose 
			// thing == NULL, this will be it's geometry frame.
			pList.Release();
			pThing.Release();
			if (FAILED(hr = pvwfTransform->GetVWFChildren(&pList)))
		        return hr;
			pList->get_Count(&lChildren);

			for (l = 0; l < lChildren; l++)
			{
				if (FAILED(hr = pList->get_ObjectProperty(l, (IObjectProperty**)&pvwfGeometry)) || !pvwfGeometry)
		            return hr;
				if (FAILED(hr = pvwfGeometry->get_Thing(&pThing)))
		            return hr;
				if (!pThing)
					break;
				pvwfGeometry.Release();
				pThing.Release();
			}
		}

		// Want to replace last geometry frames with these ones.
        if (pvwfGeometry) {
			if (FAILED(hr = pvwfTransform->DeleteChild(pvwfGeometry)))
		        return hr;
        }
		if (FAILED(hr = pNewVWGeometry->ComposeGeometry(&pvwfNewTransform)))
		    return hr;
		if (FAILED(hr = pNewVWGeometry->get_GeometryFrame(&pvwfNewGeometry)))
		    return hr;
		if (pvwfNewGeometry)
		{
			if (FAILED(hr = pvwfNewGeometry->get_Frame3D(&pd3drmf)))
		        return hr;
			if (FAILED(hr = pd3drmf->GetTransform(d3drmmtx)))
		        return hr;
			if (FAILED(hr = pvwfNewTransform->DeleteChild(pvwfNewGeometry)))
		        return hr;
			if (FAILED(hr = pvwfTransform->AddChild(pvwfNewGeometry)))
		        return hr;
			if (FAILED(hr = pd3drmf->AddTransform(D3DRMCOMBINE_REPLACE, d3drmmtx)))
		        return hr;
		}
		if (FAILED(hr = pNewVWGeometry->put_Frame(pvwfTransform)))
		    return hr;
	}

	// Always invoke OnLoadGeometry.
	pvwfNewGeometry.Release();
	if (FAILED(hr = pNewVWGeometry->get_GeometryFrame(&pvwfNewGeometry)))
		return hr;

	//Now set the mesh quality
	if (pvwfNewGeometry)
	{
		if (FAILED(hr = pThis->get_Short(bstrRenderQuality, &sRenderQuality)))
			return hr;

		//Ingore HRESULT on SetMeshQuality because it might be OCCLUDED.
		pvwfNewGeometry->SetMeshQuality(VWFRAME_ALLHIERARCHY, sRenderQuality);
	}

	pThis->InvokeMethodExt(bstrOnLoadGeometry,
						   CComDISPPARAMS(1, CComVariant((IDispatch*)pvwfNewGeometry)),
						   NULL);

	UpdateLabel(pThis);

	return hr;
}

//******************************************************
// OnGeometryNameChanged

STDMETHODIMP CMultimediaExemplarObject::OnGeometryNameChanged(long lHint, VARIANT varHintData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IVWGeometry>		pGeometry;
	CComPtr<IVWGeometrySprite>	pGeometrySprite;
	CComPtr<IThing>				pThis;
	CComBSTR					bstrGeometryName;
	static CComBSTR	bstrGeometry("Geometry");
	static CComBSTR	bstrGeometryNameProp("GeometryName");
	char						szExt[_MAX_EXT];
	static CComBSTR	bstrNamedMeshColors("NamedMeshColors");
	static CComBSTR	bstrNamedMeshTextures("NamedMeshTextures");
	static CComBSTR bstrFall("Fall");
	CComPtr<IPropertyMap>	    pMap;
	VARIANT_BOOL				bShouldFall = VARIANT_TRUE;
	HRESULT						hr = S_OK;

	if (lHint == thAdd)
		return hr;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;
	
	// clear out the named mesh colors
 	if (SUCCEEDED(hr = pThis->get_ObjectProperty(bstrNamedMeshColors, (IObjectProperty **)&pMap)) && pMap)
	{
		pMap->RemoveAll();
		pThis->put_ObjectProperty(bstrNamedMeshColors, pMap);
		pMap.Release();
	}

	// clear out the named mesh textures
 	if (SUCCEEDED(hr = pThis->get_ObjectProperty(bstrNamedMeshTextures, (IObjectProperty **)&pMap)) && pMap)
	{
		pMap->RemoveAll();
		pThis->put_ObjectProperty(bstrNamedMeshTextures, pMap);
		pMap.Release();
	}

	if (FAILED(hr = pThis->get_String(bstrGeometryNameProp, &bstrGeometryName.m_str)))
		return hr;
	if (FAILED(hr = pThis->get_ObjectProperty(bstrGeometry, (IObjectProperty**)&pGeometry)))
		return hr;

	// See what type of geometry we need.
	_splitpath(CString(bstrGeometryName), NULL, NULL, NULL, szExt);
	if ((stricmp(szExt, ".X") == 0 ||
		 stricmp(szExt, ".3DS") == 0 ||
		 stricmp(szExt, ".WRL") == 0) &&
		(!pGeometry ||
		 (SUCCEEDED(pGeometry->QueryInterface(IID_IVWGeometrySprite, (void**)&pGeometrySprite)) && pGeometrySprite)))
	{
		// Need to create a 3D geometry.
		pGeometry.Release();
		if (FAILED(hr = CreateGeometry(bstrGeometryName, &pGeometry)))
			return hr;

		if (FAILED(hr = pThis->put_ObjectProperty(bstrGeometry, pGeometry)))
			return hr;
	}
	else if ((stricmp(szExt, ".SPR") == 0 ||
			  stricmp(szExt, ".BMP") == 0 ||
			  stricmp(szExt, ".GIF") == 0 ||
			  stricmp(szExt, ".JPG") == 0) &&
			 (!pGeometry ||
			  (FAILED(pGeometry->QueryInterface(IID_IVWGeometrySprite, (void**)&pGeometrySprite)) || !pGeometrySprite)))
	{
		// Need to create a sprited geometry.
		if (FAILED(hr = CreateSpritedGeometry(bstrGeometryName, &pGeometrySprite)))
			return hr;

		if (FAILED(hr = pThis->put_ObjectProperty(bstrGeometry, pGeometrySprite)))
			return hr;
	}
	else
	{
		// Must have right type of geometry (or name must be empty), just set it in whatever 
		// geomerty type we currently have.
		if (FAILED(hr = pGeometry->put_GeometryName(bstrGeometryName)))
			return hr;

		// If just changing the graphics of a sprite, don't bother with overhead of fall.
		if (pGeometrySprite)
			bShouldFall = VARIANT_FALSE;
	}

	VARIANT_BOOL bExemplar = VARIANT_FALSE;

	m_pWorld->IsExemplar(pThis, &bExemplar);

	if (bShouldFall && (bExemplar == VARIANT_FALSE))
	{
		// Make sure this object is on the ground.
		// REVIEW: If OnGeometryNameChanged gets called server side this will cause ALL
		// clients to make the object fall which will lead to a good degree of spamming.
		if (FAILED(hr = pThis->InvokeMethodExt(bstrFall, NULL, NULL)))
			return hr;
	}

    return hr;
}

//******************************************************
// OnContentsVisibleChanged
// Handles propagating changes to the contents being 
// occluded

STDMETHODIMP CMultimediaExemplarObject::OnContentsVisibleChanged(long lHint, VARIANT varHintData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing>		pThis;
	CComPtr<IVWFrame>	pvwf;
	VARIANT_BOOL		bContentsVisible;
	static CComBSTR bstrContentsVisible("IsContentsVisible");
	HRESULT hr = S_OK;

	// only listen to put notifications
	if (lHint == thPut)
	{
		if (FAILED(hr = m_pWorld->get_This(&pThis)))
			return hr;
		
		if (FAILED(hr = _GetFrameHelper(pThis, &pvwf)) || !pvwf)
			return hr;
	
		// Is the thing's contents occluded?
		if (FAILED(pThis->get_BOOL(bstrContentsVisible, &bContentsVisible)))
			return hr;

		if (!bContentsVisible)
			RemoveContentsFromGeometry(pThis, pvwf);
		else
			AddContentsToGeometry(pThis, pvwf);
	}
	return hr;
}

HRESULT CMultimediaExemplarObject::ComposeScene(IThing *pContext, IVWFrame **ppvwf)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	ASSERT(ppvwf);
	*ppvwf = NULL;

	HRESULT hr = S_OK;
	COleVariant				var;
	CComPtr<IVWFrame>		pvwfParent, pvwfGeometry;
	CComPtr<IThing>			pThis, pContainer, pRoot;
	CComPtr<IVWGeometry>	pVWGeometry;
	CComPtr<IPropertyList>	pCell;
	CComPtr<IVector>		pVector;

	static CComBSTR	bstrClientGeometry("ClientGeometry");
	static CComBSTR	bstrGeometry("Geometry");
	static CComBSTR	bstrInnerGeometry("InnerGeometry");
	static CComBSTR bstrCell("BoundaryList");
	static CComBSTR bstrVisible("IsVisible");
	static CComBSTR bstrContentsVisible("IsContentsVisible");
	static CComBSTR bstrOnPreComposeGeometry("OnPreComposeGeometry");
	static CComBSTR bstrOnLoadGeometry("OnLoadGeometry");
	static CComBSTR bstrRotationRate("RotationRate");
	static CComBSTR bstrRotationAxis("RotationAxis");
	static CComBSTR bstrRenderQuality("RenderQuality");
	
	VARIANT_BOOL	bGetInner = VARIANT_FALSE;
	VARIANT_BOOL	bVisible, bContentsVisible, bIsAvatar, 
					bIsConnected, bLastItem = VARIANT_FALSE;
	short			sRenderQuality;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;
	
	// Is the thing's geometry occluded? Should it's contents be occluded?
	if (FAILED(hr = pThis->get_BOOL(bstrVisible, &bVisible)))
		return hr;
	if (FAILED(hr = pThis->get_BOOL(bstrContentsVisible, &bContentsVisible)))
		return hr;
	
	// Need to decide whether we need the inner or outer geometry of this 
	// object. We need the inner geometry if this object is above the context 
	// in the hierarchy.
	// NOTE: This will only work 1 level deep cause we don't pass context to 
	// AddContentsToGeometry().
	pContainer = pContext;
	while (SUCCEEDED(pContainer->get_Container(&pRoot)) && pRoot != NULL)
	{
		if (pThis == pRoot)
		{
			bGetInner = VARIANT_TRUE;
			pRoot.Release();
			break;
		}
		pContainer.Release();
		pContainer = pRoot;
		pRoot.Release();
	}
	pContainer.Release();

	// Invoke OnPreComposeGeometry.
	pThis->InvokeMethodExt(bstrOnPreComposeGeometry, NULL, NULL);

	// Get the thing's geometry property (if it has one).
	if (bGetInner != VARIANT_TRUE || FAILED(pThis->get_ObjectProperty(bstrInnerGeometry, (IObjectProperty**)&pVWGeometry)) || pVWGeometry == NULL)
	{
		if (FAILED(pThis->get_ObjectProperty(bstrClientGeometry, (IObjectProperty**)&pVWGeometry)) || !pVWGeometry)
		{
			if (FAILED(hr = pThis->get_ObjectProperty(bstrGeometry, (IObjectProperty**)&pVWGeometry)))
        		return hr;
			if (pVWGeometry==NULL)
				return S_OK;
		}
	}

	// See what kind of object is 
	if (FAILED(hr = pThis->IsOfType(CComBSTR("Avatar"), &bIsAvatar)))
		return hr;

	if (bIsAvatar == VARIANT_TRUE)
	{
		hr = pThis->get_BOOL(CComBSTR("IsConnected"), &bIsConnected);
		if (FAILED(hr))
		{
			if (hr != VWOBJECT_E_PROPERTYNOTEXIST)
				return hr;

			bIsConnected = VARIANT_FALSE;
		}
		if (bIsConnected != VARIANT_TRUE && bVisible == VARIANT_TRUE)
			bVisible = VARIANT_FALSE;
	}
		
	// Set occluded state before we load the geometry.
	if (FAILED(hr = pVWGeometry->put_Occluded(!bVisible)))
		return hr;

	// Now get the thing's geometry.
	if (FAILED(hr = pVWGeometry->ComposeGeometry(ppvwf)))
		return hr;

	// Now set it rotating and set the mesh quality if necessary
	if (*ppvwf)
	{
		if (FAILED(hr = pThis->get_Short(bstrRenderQuality, &sRenderQuality)))
		    return hr;
		//Ingore HRESULT on SetMeshQuality because it might be OCCLUDED.
		(*ppvwf)->SetMeshQuality(VWFRAME_ALLHIERARCHY, sRenderQuality);
	}

	// And add the thing's cell property to the VWFrame.
	hr = pThis->get_ObjectProperty(bstrCell, (IObjectProperty**)&pCell);
	if (FAILED(hr))
	{
		if (hr != VWOBJECT_E_PROPERTYNOTEXIST)
			return hr;
	}
	if (pCell && hr != VWOBJECT_E_PROPERTYNOTEXIST)
	{
		// This is the thing's cell - add it to the VWFrame.
		(*ppvwf)->put_BoundaryList(pCell);
	}

	if (bContentsVisible == VARIANT_TRUE)
		AddContentsToGeometry(pThis, *ppvwf);

	// Invoke OnLoadGeometry.
	if (FAILED(hr = pVWGeometry->get_GeometryFrame(&pvwfGeometry)))
		return hr;

	pThis->InvokeMethodExt(bstrOnLoadGeometry, 
                           CComDISPPARAMS(1, CComVariant((IDispatch*)pvwfGeometry)), 
                           &var);

	return hr;
}

//******************************************************
// GetFrame

STDMETHODIMP CMultimediaExemplarObject::GetFrame(IVWFrame **ppvwf)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr = S_OK;

	*ppvwf = NULL;

	CComPtr<IThing> pThis;
	CComPtr<IVWGeometry> pVWGeometry;
	static CComBSTR bstrGeometry("Geometry");
	static CComBSTR bstrClientGeometry("ClientGeometry");

	if (ppvwf==NULL)
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::GetFrame: NULL output pointer\n");
		return ReportMMError(E_POINTER);
    }

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	// Some things may just not have a geometry property, or there may be something
	// wrong with this thing's geomerty property. Either way we should not fail here.
	// Get the thing's ClientGeometry property (if it has one).
	if (FAILED(pThis->get_ObjectPropertyExt(bstrClientGeometry, IID_IVWGeometry, (IObjectProperty**)&pVWGeometry)) || !pVWGeometry)
	{
		// Get the thing's geometry property (if it has one).
		if (FAILED(pThis->get_ObjectPropertyExt(bstrGeometry, IID_IVWGeometry, (IObjectProperty**)&pVWGeometry)))
			return S_OK;
	}

	if (pVWGeometry)
		hr = pVWGeometry->get_Frame(ppvwf);

	return hr;
}

//******************************************************
// GetFrame

STDMETHODIMP CMultimediaExemplarObject::GetGeometryFrame(IVWFrame **ppvwf)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr = S_OK;

	CComPtr<IThing> pThis;
	CComPtr<IVWGeometry> pVWGeometry;
	static CComBSTR bstrGeometry("Geometry");
	static CComBSTR bstrClientGeometry("ClientGeometry");

	if (ppvwf==NULL)
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::GetGeometryFrame: NULL output pointer\n");
		return ReportMMError(E_POINTER);
    }

	*ppvwf = NULL;
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	// Some things may just not have a geometry property, or there may be something
	// wrong with this thing's geomerty property. Either way we should not fail here.
	// Get the thing's ClientGeometry property (if it has one).
	if (FAILED(pThis->get_ObjectPropertyExt(bstrClientGeometry, IID_IVWGeometry, (IObjectProperty**)&pVWGeometry)) || !pVWGeometry)
	{
		// Get the thing's geometry property (if it has one).
		if (FAILED(pThis->get_ObjectPropertyExt(bstrGeometry, IID_IVWGeometry, (IObjectProperty**)&pVWGeometry)))
			return S_OK;
	}

	if (pVWGeometry)
		hr = pVWGeometry->get_GeometryFrame(ppvwf);

	return hr;
}

//******************************************************
// FireIntersectEvent
// Fires an intersection event...??

STDMETHODIMP CMultimediaExemplarObject::FireIntersectEvent(IVWFrame *pvwfRef, float fSrcX, float fSrcY, float fSrcZ, float fDestX, float fDestY, float fDestZ, IPropertyList *pIDataList)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IVWIntersectData>	pVWID;
	CComPtr<IPropertyList>		pCell;
	CComPtr<IThing>				pIntersecter;
	CComPtr<IThing>				pIntersectee;
	CComPtr<IVWFrame>			pvwfIntersectee;
	long						lCount;
	int							i, iBoundaryIndex, iEdgeIndex;
	BYTE						bPassed, bTransition;
	float						fPosX, fPosY, fPosZ, fDirX, fDirY, fDirZ, fUpX, fUpY, fUpZ;
	float						fDist, fIntX, fIntY, fIntZ, fIntXOld, fIntZOld;
	COleVariant					var;
    HRESULT                     hr = S_OK;

	static CComBSTR		bstrIntersecterMethod("OnIntersecter");
	static CComBSTR		bstrIntersecteeMethod("OnIntersectee");
	static CComBSTR		bstrGetFrame("GetFrame");

    if ((pvwfRef==NULL) || (pIDataList==NULL))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::FireIntersectEvent: NULL pointer\n");
		return ReportMMError(E_POINTER);
    }

	// We are the intersecter.
	if (FAILED(hr = m_pWorld->get_This(&pIntersecter)))
		return hr;

	pIDataList->get_Count(&lCount);

	// Now extract the intersect data from the list, one by one.
	for (i = 0; i < lCount; i++)
	{
		// Extract the intersect data.
		if (FAILED(hr = pIDataList->get_ObjectProperty(i, (IObjectProperty**)&pVWID)) || !pVWID)
		    return hr;

		// Make sure we didn't pass over/under this edge.
		pVWID->get_Passed(&bPassed);
		if (bPassed == ePassedOver || bPassed == ePassedUnder)
		{
			pVWID.Release();
			continue;
		}

		// Need to figure out which thing the cell we have intersected with is associated 
		// with, that thing is the intersectee.
		if (FAILED(hr = pVWID->get_BoundaryList(&pCell)))
		    return hr;
		if (FAILED(hr = pCell->get_Parent(&pIntersectee)))
		    return hr;

		// Intersectee needs to be passed intersection coordinates which are relative to 
		// it's frame, not the reference frame used to move the intersecter.
		// Intersecter needs same coordinates but relative to the reference frame used to 
		// move it.

		// First get the intersectee's loaded geometry.
		if (FAILED(hr = pIntersectee->InvokeMethodExt(bstrGetFrame, NULL, &var)))
		    return hr;
		if (var.vt != VT_DISPATCH || !var.pdispVal)
        {
            VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::FireIntersectEvent: non-dispatch returned from GetFrame\n");
            return ReportMMError(VWMM_E_UNKNOWN);
        }
		pvwfIntersectee = (IVWFrame*)var.pdispVal;
		pVWID->get_BoundaryIndex(&iBoundaryIndex);
		pVWID->get_EdgeIndex(&iEdgeIndex);
		pVWID->get_DistToIntersect(&fDist);
		pVWID->get_Transition(&bTransition);
		// Find the true point of intersection, relative to the intersecter's reference frame.
		fIntX = fSrcX + ((fDestX-fSrcX)*fDist);
		fIntY = fSrcY + ((fDestY-fSrcY)*fDist);
		fIntZ = fSrcZ + ((fDestZ-fSrcZ)*fDist);

		// Invoke the event handlers on intersecter.
        pIntersecter->InvokeMethodExt(bstrIntersecterMethod, CComDISPPARAMS(7, CComVariant((IDispatch*)pIntersectee),
			CComVariant(fIntX), CComVariant(fIntY), CComVariant(fIntZ), CComVariant(iBoundaryIndex),
			CComVariant(iEdgeIndex), CComVariant(bTransition)), NULL);

		// The intersectee's frame may be the reference frame used to move the intersecter.
		if (pvwfIntersectee != pvwfRef)
		{
			// Also need the point relative to the intersectee's frame. Apply the 
			// intersectee's transformation relative to the intersecter's reference 
			// frame inversely to the intersect point.
			if (FAILED(hr = pvwfIntersectee->GetPositionEx(pvwfRef, &fPosX, &fPosY, &fPosZ)))
        		return hr;
			if (FAILED(hr = pvwfIntersectee->GetOrientationEx(pvwfRef, &fDirX, &fDirY, &fDirZ, &fUpX, &fUpY, &fUpZ)))
		        return hr;
			fIntX -= fPosX;
			fIntY -= fPosY;
			fIntZ -= fPosZ;
			fIntXOld = fIntX;
			fIntZOld = fIntZ;
			fIntX = fIntXOld*fDirZ + fIntZOld*fDirX;
			fIntZ = fIntZOld*fDirZ - fIntXOld*fDirX;
		}	
        // Invoke the event handlers on intersectee.
        pIntersectee->InvokeMethodExt(bstrIntersecteeMethod, CComDISPPARAMS(7, CComVariant((IDispatch*)pIntersecter),
                                      CComVariant(fIntX), CComVariant(fIntY),
                                      CComVariant(fIntZ), CComVariant(iBoundaryIndex),
                                      CComVariant(iEdgeIndex), CComVariant(bTransition)), NULL);
		pIntersectee.Release();
		pVWID.Release();
		pCell.Release();
		pvwfIntersectee.Release();
	}
	return hr;
}


STDMETHODIMP CMultimediaExemplarObject::GetFinalAnimationVector(IThing* pThis, BSTR bstrProp, IVector** ppVector) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HRESULT				hr = S_OK;
	CComPtr<IVector>	pVector;
	CComPtr<IVWFrame>	pvwfTransform,pvwfParent;
	float				fX,fY,fZ,fTempX,fTempY,fTempZ;
	CComVariant			vX, vY, vZ;
	CString				strProp(bstrProp);
	static CString		strPosition("Position");
	static CString		strDirection("Direction");
	static CString		strScale("Scale");
	static CString		strUp("Up");
	static CString		strFile("File");
	static CString		strLightColor("LightColor");
	static CString		strLightAttenuation("LightAttenuation");
	CComBSTR			bstrPropertyAnimStartTime(bstrProp);
	CComBSTR			bstrPropertyAnimProperties(bstrProp);
	CComBSTR			bstrPropertyAnimOptions(bstrProp);
	CComBSTR			bstrPropertyAnimTimeScale(bstrProp);
	CComBSTR			bstrPropertyAnimRangeStart(bstrProp);
	CComBSTR			bstrPropertyAnimRangeEnd(bstrProp);
	CComBSTR			bstrPropertyAnimIndex(bstrProp);
	CComVariant				var, var2;
	CComPtr<IPropertyMap>	pAnimPropertiesMap;
	CComPtr<IPropertyList>	pFileKeyList;
	long					lOptions = 1l;
	long					lStartTime = 0l;
	unsigned long			ulEndTime;
	bstrPropertyAnimProperties.Append("AnimProperties");
	bstrPropertyAnimOptions.Append("AnimOptions");
	bstrPropertyAnimStartTime.Append("AnimStartTime");
	bstrPropertyAnimTimeScale.Append("AnimTimeScale");
	bstrPropertyAnimRangeStart.Append("AnimRangeStart");
	bstrPropertyAnimRangeEnd.Append("AnimRangeEnd");
	bstrPropertyAnimIndex.Append("AnimIndex");

	float fTimeScale, fStartRange, fEndRange, fIndex;
	long lCount=0l;

	if (!pThis)
		return E_FAIL;
	//get the start time and the options from the animation to calculate time for file / scale indexes
	if (FAILED(hr = pThis->get_Property(bstrPropertyAnimProperties, &var)))
	   return hr;
	if ((var.vt != VT_DISPATCH) || (!(var.pdispVal)))
	{ 
		hr = E_FAIL;
		return hr;
	}	
	if (FAILED(hr = var.pdispVal->QueryInterface(IID_IPropertyMap,(void**)&pAnimPropertiesMap)))
		return hr;
	
	if (FAILED(hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimOptions , &var2) ))
		return hr;
	lOptions = CastToLong(var2);
	var2.Clear();

	if (FAILED(hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimStartTime, &var2) ))
		return hr;
	lStartTime = CastToLong(var2);
	var2.Clear();

	if (FAILED(hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimTimeScale, &var2) ))
		return hr;
	fTimeScale = CastToFloat(var2);
	var2.Clear();

	if (FAILED(hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimRangeStart, &var2) ))
		return hr;
	fStartRange = CastToFloat(var2);
	var2.Clear();

	if (FAILED(hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimRangeEnd, &var2) ))
		return hr;
	fEndRange = CastToFloat(var2);
	var2.Clear();

	if (FAILED(hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimIndex, &var2) ))
		return hr;
	fIndex = CastToFloat(var2);
	var2.Clear();

	if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)) || (!pvwfTransform))
		return hr;
	if ((FAILED(hr = pvwfTransform->GetParent(&pvwfParent))) || (!pvwfParent))
		return hr;
	

	if (lStartTime > 0)
	{		
		ULongAdd((unsigned long)lStartTime, ((unsigned long)(((fIndex)>(fStartRange)?(fEndRange-fIndex):(fEndRange-fStartRange))/fTimeScale)), &ulEndTime);

		if (((unsigned long)CurrentTime() < ulEndTime) || (lOptions & 2))
		{
			//calculate a good index from the elapsed time and timescale
			unsigned long ulDuration;

			if ((unsigned long)CurrentTime() < ulEndTime)
			{
				ULongSubtract ( ulEndTime, (unsigned long)CurrentTime(), &ulDuration);	
				fIndex = ( (   ((fEndRange - fStartRange)/fTimeScale) - (float)ulDuration ) * (fEndRange - fStartRange) ) / ((fEndRange - fStartRange)/fTimeScale);
				if (strProp.CompareNoCase(strFile)==0)
					fIndex = fStartRange + fIndex;
			}
			else
			{
				ULongSubtract ( (unsigned long)CurrentTime(), ulEndTime, &ulDuration);
				ulDuration = (ulDuration)%((unsigned long)((fEndRange - fStartRange)/fTimeScale)); 
				fIndex = ((float)ulDuration * (fEndRange - fStartRange)) / ((fEndRange - fStartRange)/fTimeScale);
				if (strProp.CompareNoCase(strFile)==0)
					fIndex = fStartRange + fIndex; //SK to do: check if this is accurate!
			}
		}
		else
		{
			fIndex = fEndRange;
		}
	}
	//for position, direction, and up + light animations we can get the final vector directly from the frame.
	//for file and scale animations we must base it on the elapsed time, since we cant query the current state from
	//the frame.

	if (strProp.CompareNoCase(strFile)==0) 
	{
		if (FAILED(hr = CreateVector(fIndex, 0.0f, 0.0f, &pVector)))
			return hr;
		*ppVector = pVector;
		SAFEADDREF(*ppVector);
	}
	else if (strProp.CompareNoCase(strPosition)==0) 
	{
		if (FAILED(hr = pvwfTransform->GetPositionEx(pvwfParent,&fX,&fY,&fZ)))
			return hr;
		if (FAILED(hr = CreateVector(fX, fY, fZ, &pVector)))
			return hr;
		*ppVector = pVector;
		SAFEADDREF(*ppVector);
	}
	else if (strProp.CompareNoCase(strDirection)==0)
	{
		if (FAILED(hr = pvwfTransform->GetOrientationEx(pvwfParent,&fX,&fY,&fZ,&fTempX,&fTempY,&fTempZ)))
			return hr;
		if (FAILED(hr = CreateVector(fX, fY, fZ, &pVector)))
			return hr;
		*ppVector = pVector;
		SAFEADDREF(*ppVector);
	}
	else if (strProp.CompareNoCase(strUp)==0)
	{
		if (FAILED(hr = pvwfTransform->GetOrientationEx(pvwfParent,&fTempX,&fTempY,&fTempZ,&fX,&fY,&fZ)))
			return hr;
		if (FAILED(hr = CreateVector(fX, fY, fZ, &pVector)))
			return hr;
		*ppVector = pVector;
		SAFEADDREF(*ppVector);
	}
	else if (strProp.CompareNoCase(strLightColor)==0)
	{
		if (FAILED(hr = pvwfTransform->GetLightColor(0,&vX,&vY,&vZ)))
			return hr;
		if (FAILED(hr = CreateVector(vX.fltVal, vY.fltVal, vZ.fltVal, &pVector)))
			return hr;
		*ppVector = pVector;
		SAFEADDREF(*ppVector);
	}
	else if (strProp.CompareNoCase(strLightAttenuation)==0)
	{
		if (FAILED(hr = pvwfTransform->GetLightAttenuation(0,&vX,&vY,&vZ)))
			return hr;
		if (FAILED(hr = CreateVector(vX.fltVal, vY.fltVal, vZ.fltVal, &pVector)))
			return hr;
		*ppVector = pVector;
		SAFEADDREF(*ppVector);
	}
	//for scale animations we must get the end value from the internal scale 
	//path we created when the animation was started.

	else if (strProp.CompareNoCase(strScale)==0)
	{
	
		long dwTime = (long)fIndex;
		long lTime,lAbove = 0,lBelow = 0,lDur = 0;
		float fAX,fAY,fAZ,fBX,fBY,fBZ,fPerc,fDX,fDY,fDZ;
		BSTR bstrName;
		CComVariant var;
		VARIANT_BOOL bDone,bIsEmpty;
		CComPtr<IPropertyMap>	pPropPath;
		
		//get the internal path and find the scale values at the keys directly before
		//and after the current animation time
		if (FAILED(hr = pThis->get_ObjectPropertyExt(CComBSTR("ScaleAnimPath"), IID_IPropertyMap, (IObjectProperty**)&pPropPath)))
			return hr;
		if (!pPropPath) return E_FAIL;
		if (FAILED(hr = pPropPath->get_IsEmpty(&bIsEmpty)))
			return hr;
		if (bIsEmpty == TRUE) return S_OK;

		hr = pPropPath->FirstItem(&bstrName, &var, &bDone);
		if (FAILED(hr) && hr != VWOBJECT_E_PERMISSION_DENIED)
			return hr;

		while (bDone != VARIANT_TRUE)
		{
			if (var.vt != VT_DISPATCH) {
				hr = E_FAIL;
				return hr;
			}
			if (FAILED(hr = var.pdispVal->QueryInterface(IID_IVector,(void**)&pVector)))
				return hr;

			if (hr != VWOBJECT_E_PERMISSION_DENIED)
			{
				lTime = atoi(CString(bstrName));
				pVector->get(&fX, &fY, &fZ);
				if ( (lTime >= (long)dwTime) && ((lTime < lAbove) || (!lAbove)) ) {
					lAbove = lTime;
					fAX = fX;
					fAY = fY;
					fAZ = fZ;
				}
				if ((lTime <= (long)dwTime) && (lTime >= lBelow)) {
					lBelow = lTime;
					fBX = fX;
					fBY = fY;
					fBZ = fZ;
				}
				if (lTime > lDur) {
					lDur = lTime;
					fDX = fX;
					fDY = fY;
					fDZ = fZ;
				}
			}
			// free stuff
			SAFEFREESTRING(bstrName);
			pVector.Release();
			// go on to the next one
			hr = pPropPath->NextItem(&bstrName, &var, &bDone);
			if (FAILED(hr) && hr != VWOBJECT_E_PERMISSION_DENIED)
				return hr;
		}//end while bdone!=true

		//if the animation is done use the values from the last key
		if ((long)dwTime > lDur) {
			if (FAILED(hr = CreateVector(fDX, fDY, fDZ, &pVector)))
				return hr;
		}
		//otherwise interpolate from the keys before and after the current time
		else {
			fPerc = (float)(dwTime - lBelow) / (float)(lAbove - lBelow);
			fX = (1-fPerc)*fBX + fPerc*fAX;
			fY = (1-fPerc)*fBY + fPerc*fAY;
			fZ = (1-fPerc)*fBZ + fPerc*fAZ;
			if (FAILED(hr = CreateVector(fX, fY, fZ, &pVector)))
				return hr;
		}
		//insert the end values into the vector**
		*ppVector = pVector;
		SAFEADDREF(*ppVector);

	}

	return hr;
}


STDMETHODIMP CMultimediaExemplarObject::StartAnimation(BSTR bstrPropertyName, VARIANT varPathMap, VARIANT varCallBack, VARIANT varOptions, VARIANT varTimeScale, VARIANT varStartRange, VARIANT varEndRange, VARIANT varIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	static CComBSTR			bstrStartAnimationServer("StartAnimationServer");
	static CComBSTR			bstrHandleStartAnimation("HandleStartAnimation");
	static CString			strFile("File");
	CString					strPropertyName(bstrPropertyName);
	CComBSTR				bstrPropertyAnimOptions(bstrPropertyName);
	CComPtr<IThing>			pThis;
	CComPtr<IPropertyMap>	pPathMap;
	HRESULT					hr = S_OK;
	CComPtr<IPropertyMap>	pPropPath;
	VARIANT_BOOL			bIsEmpty, bPropertyExists, bIsValidMethod, bIsServerSide;
	long					lOptions = 0l;
	long					lFlag = 0l;
	CComVariant				varValidOptions;
	VARIANT_BOOL			bOption = VARIANT_TRUE;
	const long				lOpen = 1l;
	const long				lClosed = 2l;
	const long				lLinear = 4l;
	const long				lSpline = 8l;
	const long				lNearest = 16l;
	const long				lLocal = 32l;
	const long				lRelative = 64l;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;
	//check the optional args against VT_ERROR to see if they were passed in and verify the values are of a usable type
	if (varOptions.vt != VT_ERROR) 
	{
		if (!IsNumeric(varOptions))	
			goto VARIANT_ERROR_ENCOUNTERED;
		else
			lOptions = CastToLong(varOptions);

		if ((varOptions.vt == VT_R4) || (varOptions.vt == VT_R8))
			goto VARIANT_ERROR_ENCOUNTERED;
	}
	if ((lOptions < 0) || (lOptions > 114))
	{
		VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::StartAnimation: Invalid animation options.\n");
		return ReportMMError(E_FAIL);
	}
	if (lOptions >= (lLocal + lRelative))
	{
		lFlag = (lLocal + lRelative);
		lOptions -= (lLocal + lRelative);
	}
	else if (lOptions >= lRelative)
	{
		lFlag = (lLocal + lRelative);
		lOptions -= lRelative;
	}
	else if (lOptions >= lLocal)
	{
		lFlag = lLocal;
		lOptions -= lLocal;
	}
	switch (lOptions)
	{
		case (0):
			bOption = VARIANT_TRUE;
			lOptions = (lOpen + lSpline);
			break;
		case (lOpen):
			bOption = VARIANT_TRUE;
			lOptions = (lOpen + lSpline);
			break;
		case (lClosed):
			bOption = VARIANT_TRUE;
			lOptions = (lClosed + lSpline);
			break;
		case (lLinear):
			bOption = VARIANT_TRUE;
			lOptions = (lOpen + lLinear);
			break;
		case (lLinear + lOpen): case (lLinear + lClosed):
			bOption = VARIANT_TRUE;
			break;
		case (lSpline):
			bOption = VARIANT_TRUE;
			lOptions = (lOpen + lSpline);
			break;
		case (lSpline + lOpen): case (lSpline + lClosed):
			bOption = VARIANT_TRUE;
			break;
		case (lNearest):
			bOption = VARIANT_TRUE;
			lOptions = (lOpen + lNearest);
			break;
		case (lNearest + lOpen): case (lNearest + lClosed):
			bOption = VARIANT_TRUE;
			break;
		default:
			 bOption = VARIANT_FALSE;
	}
	lOptions += lFlag;
	varValidOptions = lOptions;

	if (!bOption)
	{
		VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::StartAnimation: Invalid animation options.\n");
		return ReportMMError(E_FAIL);
	}

	if (varTimeScale.vt != VT_ERROR) 
		if (!IsNumeric(varTimeScale))	
			goto VARIANT_ERROR_ENCOUNTERED;
	if (varStartRange.vt != VT_ERROR) 
		if (!IsNumeric(varStartRange))	
			goto VARIANT_ERROR_ENCOUNTERED;
	if (varEndRange.vt != VT_ERROR) 
		if (!IsNumeric(varEndRange))	
		{
VARIANT_ERROR_ENCOUNTERED:
			VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::StartAnimation: Found an invalid optional argument\n");
			return ReportMMError(E_FAIL);
		}	
	//make sure our property is one of the ones we support for animation by checking if we have a good anim options map
	bstrPropertyAnimOptions.Append("AnimProperties");
	if (FAILED(hr = pThis->IsValidProperty(bstrPropertyAnimOptions, &bPropertyExists)))
		   return hr;
	if (!bPropertyExists)
	{
		VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::StartAnimation: Unsupported Animation type passed in 1st argument\n");
		return ReportMMError(E_FAIL);
	}
	if (strPropertyName.CompareNoCase(strFile)!=0) 
	{
		//make sure the property we are trying to animate is actually on the object
		if (FAILED(hr = pThis->IsValidProperty(bstrPropertyName, &bPropertyExists)))
			   return hr;
		if (!bPropertyExists)
		{
			VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::StartAnimation: The object doesnt support this animation property\n");
			return ReportMMError(E_FAIL);
		}
		//check to see if we have a good property animation path map, otherwise fail here
		if (varPathMap.vt == VT_ERROR) 
		{
			//fail here, since we passed in a property animation but no map
			VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::StartAnimation: Cant animate properties other than file without a property map passed in argument 2\n");
			return ReportMMError(E_FAIL);
		}	
		if (varPathMap.vt != VT_DISPATCH) 
		{
			VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::StartAnimation: 2nd argument must be a property map containing only vectors\n");
			return ReportMMError(E_FAIL);
		}
		//make sure the value in the argument is a pointer to a good property map
		if (FAILED(hr = varPathMap.pdispVal->QueryInterface(IID_IPropertyMap,(void**)&pPathMap)))
		{
			VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::StartAnimation: 2nd argument must be a property map containing only vectors\n");
			return ReportMMError(E_FAIL);
		}
		if (!pPathMap)
			goto MAP_ERROR_ENCOUNTERED;
		if (FAILED(hr = pPathMap->get_IsEmpty(&bIsEmpty)))
			goto MAP_ERROR_ENCOUNTERED;
		if (bIsEmpty == TRUE)
		{
MAP_ERROR_ENCOUNTERED:
				VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::StartAnimation: Animation Path Map has no keys\n");
				return ReportMMError(E_FAIL);
		}
		pPathMap.Release();
	}
	else
		if ((varStartRange.vt == VT_ERROR) || (varEndRange.vt == VT_ERROR))
		{
			VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::StartAnimation: File animations require start and end ranges as parameters.\n");
			return ReportMMError(E_FAIL);
		}
	//if we got a callback method, verify that it is valid for the object
	if (varCallBack.vt != VT_ERROR)
	{
		//verify the the variant has a string in it
		if (varCallBack.vt != VT_BSTR)
		{
				VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::StartAnimation: Argument 3 (Method Callback) must be empty or the string name of a valid method\n");
				return ReportMMError(E_FAIL);
		}
		if (FAILED(hr = pThis->IsValidMethod(varCallBack.bstrVal, &bIsValidMethod)))
			return hr;
		if (!bIsValidMethod)
		{
			VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::StartAnimation: An invalid method was specified for the animation callback\n");
			return ReportMMError(E_FAIL);
		}
	}
	//check if we are client-only in the options passed in, and we are not server-side right now
	//if so, just call the client-side animation handler directly, 
	 if ((SUCCEEDED(m_pWorld->get_ServerSide( &bIsServerSide )) && (bIsServerSide == VARIANT_FALSE))&&(lOptions & 32))
		hr = pThis->InvokeMethodExt(bstrHandleStartAnimation, 
									CComDISPPARAMS(	8,
													CComVariant(bstrPropertyName),
													CComVariant(varPathMap),
													CComVariant(varCallBack),
													CComVariant(varValidOptions),
													CComVariant(varTimeScale),
													CComVariant(varStartRange),
													CComVariant(varEndRange),
													CComVariant(varIndex)), 
									NULL);	
	else
	{
//		if (bIsServerSide == VARIANT_TRUE)
//			if (lOptions & 64)
//			{
				

//			}
		//kick off the server method to remote/persist this animation to everyone
		hr = pThis->InvokeMethodExt(bstrStartAnimationServer, 
									CComDISPPARAMS(	8,
													CComVariant(bstrPropertyName),
													CComVariant(varPathMap),
													CComVariant(varCallBack),
													CComVariant(varValidOptions),
													CComVariant(varTimeScale),
													CComVariant(varStartRange),
													CComVariant(varEndRange),
													CComVariant(varIndex)), 
									NULL);
	}
	return hr;
}



STDMETHODIMP CMultimediaExemplarObject::StopAnimation(BSTR bstrPropertyName, VARIANT varOptions)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	CComBSTR		bstrStopAnimationServer("StopAnimationServer");
	CComBSTR		bstrHandleStopAnimationWithRequest("HandleStopAnimationWithRequest");
	CComBSTR		bstrHandleStopAnimation("HandleStopAnimation");
	CComPtr<IThing>	pThis;
	CComPtr<IVector>pVector;
	HRESULT			hr = S_OK;
	VARIANT_BOOL	bIsServerSide;
	long lOptions = 9l;
	static CString	strFile("File");
	static CString	strPosition("Position");
	static CString	strDirection("Direction");
	static CString	strUp("Up");
	static CString	strScale("Scale");
	static CString	strLightColor("LightColor");
	static CString	strLightAttenuation("LightAttenuation");

	CString strPropertyName = bstrPropertyName;

	//error out on non-supported anim types..
	if ((strPropertyName.CompareNoCase(strFile)!=0) && 
		(strPropertyName.CompareNoCase(strPosition)!=0) &&
		(strPropertyName.CompareNoCase(strDirection)!=0) &&
		(strPropertyName.CompareNoCase(strUp)!=0) &&
		(strPropertyName.CompareNoCase(strScale)!=0) &&
		(strPropertyName.CompareNoCase(strLightColor)!=0) &&
		(strPropertyName.CompareNoCase(strLightAttenuation)!=0))
	{	
		VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::StopAnimation: invalid animation property\n");
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::StopAnimation: invalid animation property\n");
		return E_FAIL;
	}

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		goto ERROR_ENCOUNTERED;

	if (varOptions.vt != VT_ERROR) 
	{
		if (IsNumeric(varOptions))	
			lOptions = CastToLong(varOptions);
		else
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
	}
	if (FAILED(hr = m_pWorld->get_ServerSide( &bIsServerSide )))
		goto ERROR_ENCOUNTERED;
	if ((bIsServerSide == VARIANT_FALSE)&&(lOptions & 32))
	{
		if (FAILED(hr = pThis->InvokeMethodExt(bstrHandleStopAnimation, 
									CComDISPPARAMS(	2,
													CComVariant(bstrPropertyName),
													CComVariant(lOptions)), 
									NULL)))
			goto ERROR_ENCOUNTERED;
	}
	else
	{	
		if (bIsServerSide == VARIANT_FALSE)
		{
			//get the value of this prop on this client...
			if (FAILED(hr = GetFinalAnimationVector(pThis, bstrPropertyName, &pVector)))
				goto ERROR_ENCOUNTERED;
			//kick of the server-side method to set the prop and handlestopanimation on other clients
			if (FAILED(hr = pThis->InvokeMethodExt(bstrStopAnimationServer, 
										CComDISPPARAMS(	3,
														CComVariant(bstrPropertyName),
														CComVariant(pVector),
														CComVariant(lOptions)), 
										NULL)))
				goto ERROR_ENCOUNTERED;
		}
		else
			if (FAILED(hr = pThis->InvokeMethodExt(bstrHandleStopAnimationWithRequest, 
											CComDISPPARAMS(	2,
															CComVariant(bstrPropertyName),
															CComVariant(lOptions)), 
											NULL)))
					goto ERROR_ENCOUNTERED;
	}
ERROR_ENCOUNTERED:

	if (FAILED(hr))
	{	
		VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::StopAnimation: An error occurred\n");
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::StopAnimation: Error dealing with an animation property Map\n");
	}
	return hr;
}



STDMETHODIMP CMultimediaExemplarObject::SetAnimationPropertyRequest(BSTR bstrPropertyName, VARIANT varStartTime, VARIANT varValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	
	//this sets the animation's start time property in the map back to zero 
	//after the first call with a good start time

	CComBSTR				bstrPropertyAnimStartTime(bstrPropertyName);
	CComBSTR				bstrPropertyAnimProperties(bstrPropertyName);
	CComPtr<IPropertyMap>	pAnimPropertiesMap;
	CComVariant				var;
	CComPtr<IThing>	pThis;
	CComPtr<IVector>pVector;
	HRESULT			hr = S_OK;
	long lStoredTime, lStartTimeClient;
	static CString		strFile("File");
	CString				strPropertyName(bstrPropertyName);
	CComBSTR				bstrFileAnimIndex("FileAnimIndex");

	lStartTimeClient = CastToLong(varStartTime);

	if (lStartTimeClient != 0l)
	{
		bstrPropertyAnimStartTime.Append("AnimStartTime");
		bstrPropertyAnimProperties.Append("AnimProperties");

		if (FAILED(hr = m_pWorld->get_This(&pThis)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = pThis->get_Property(bstrPropertyAnimProperties, &var)))
		   goto ERROR_ENCOUNTERED;
		else
		{
			//get the relevant starttime info from the animation properties map
			if (var.vt != VT_DISPATCH)
			{ 
				hr = E_FAIL;
				goto ERROR_ENCOUNTERED;
			}	
			if(var.pdispVal)
			{
				if (SUCCEEDED(hr = var.pdispVal->QueryInterface(IID_IPropertyMap,(void**)&pAnimPropertiesMap)))
				{
					//stamp the starttime as zero in the correct animproperties map for this animation
					if (FAILED(hr = pAnimPropertiesMap->get_Long( bstrPropertyAnimStartTime, &lStoredTime )))
						goto ERROR_ENCOUNTERED;	
				}
				if ((lStoredTime == lStartTimeClient) && (lStoredTime != 0l))
				{
					if (FAILED(hr = pAnimPropertiesMap->put_Property( bstrPropertyAnimStartTime, CComVariant((long)0l) )))
						goto ERROR_ENCOUNTERED;		
					if (FAILED(hr = varValue.pdispVal->QueryInterface(IID_IVector,(void**)&pVector)))
						goto ERROR_ENCOUNTERED;
					if (strPropertyName.CompareNoCase(strFile)==0) 
					{
						float fX, fY, fZ;
						pVector->get(&fX, &fY, &fZ);
						if (FAILED(hr = pAnimPropertiesMap->put_Property( bstrFileAnimIndex, CComVariant((float)fX) )))
							goto ERROR_ENCOUNTERED;	
					}
					else
						if (FAILED(hr = pThis->put_ObjectProperty(bstrPropertyName, pVector)))
							goto ERROR_ENCOUNTERED;	
					if (FAILED(hr = RemovePriorAnimations(pThis, bstrPropertyName)))
						goto ERROR_ENCOUNTERED;
					if (FAILED(hr = InvokeStaticDoneMethods(pThis, bstrPropertyName)))
						goto ERROR_ENCOUNTERED;
				}
			}
		}
	}
ERROR_ENCOUNTERED:

	if (FAILED(hr))
	{	
		VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::SetAnimationPropertyRequest: An error occurred\n");
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::SetAnimationPropertyRequest: Error \n");
	}
	return hr;
}



STDMETHODIMP CMultimediaExemplarObject::HandleStopAnimationWithRequest(BSTR bstrPropertyName, VARIANT varOptions)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	char					buff[200];
	CComBSTR				bstrInterpName;
	static CComBSTR			bstrSetAnimationPropertyRequest("SetAnimationPropertyRequest");
	static CComBSTR			bstrDeleteInterp("DeleteAnimation");
	CComBSTR				bstrPropertyAnimProperties(bstrPropertyName);
	CComBSTR				bstrPropertyAnimStartTime(bstrPropertyName);
	CComPtr<IThing>			pThis;
	CComPtr<IPropertyMap>	pAnimPropertiesMap;
	CComVariant				var;
	HRESULT					hr = S_OK;
	static CString			strFile("File");
	CString					strPropertyName(bstrPropertyName);
	CComPtr<IVWFrame>		pvwfGeometry, pvwfTransform;
	CComPtr<IVWGeometry>	pVWGeometry;
	CComPtr<IVector>		pVector;
	static CComBSTR			bstrGeometry("Geometry");
	CComPtr<IDirect3DRMAnimationSet2> pAnimationSet;
	DWORD					dwSize = 256;
	char					szAnimName[256];
	long					lOptions;
	long					lStartTime;

	lOptions = CastToLong(varOptions);

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	if (strPropertyName.CompareNoCase(strFile)!=0) 
	{
		_ltoa((long)(void*)pThis,buff,16);
		bstrInterpName = buff;
		bstrInterpName.Append(bstrPropertyName);

		if (FAILED(hr = m_pWorld->FireUIEvent(NULL,bstrDeleteInterp,CComVariant(bstrInterpName))))
			goto ERROR_ENCOUNTERED;
	}
	else
	{	
  		if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)))
			goto ERROR_ENCOUNTERED;
		if (!pvwfTransform)
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		if (FAILED(hr = pThis->get_ObjectPropertyExt(bstrGeometry, IID_IVWGeometry, (IObjectProperty**)&pVWGeometry)))
			goto ERROR_ENCOUNTERED;
		if (pVWGeometry==NULL)
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		if (FAILED(hr = pVWGeometry->get_GeometryFrame(&pvwfGeometry)))
			goto ERROR_ENCOUNTERED;
		if (pvwfGeometry==NULL)
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		if (FAILED(hr = pvwfGeometry->get_AnimationSet(&pAnimationSet)))
			goto ERROR_ENCOUNTERED;
		if (pAnimationSet == NULL)
		{
			return S_OK;
		}
		pAnimationSet->GetName(&dwSize, szAnimName);
		_ltoa((long)(void*)pThis,buff,16);
		bstrInterpName = buff;
		bstrInterpName.Append(szAnimName);
		if (FAILED(hr = m_pWorld->FireUIEvent(NULL,bstrDeleteInterp,CComVariant(bstrInterpName))))
			goto ERROR_ENCOUNTERED;		
	}
	if (lOptions & 32) //we are supposed to only be client-side- dont ask the server to touch the property
		return hr;
	else
	{	//SK: this is a hack but it's the only way to inform the server of the correct prop value
		//without setting the prop explicitly from every client...

		//clients get the vector to set the property with from their current graphics, 
		//then they pass the starttime, propname + the vector back to the server
		//the server-side method invoked will only accept one client call per unique thing+property+starttime.
		if (FAILED(hr = GetFinalAnimationVector(pThis, bstrPropertyName, &pVector)))
			goto ERROR_ENCOUNTERED;

		bstrPropertyAnimProperties.Append("AnimProperties");
		bstrPropertyAnimStartTime.Append("AnimStartTime");

		//get the map of anim properties, get the correct start time out for server verification.
		if (FAILED(hr = pThis->get_Property(bstrPropertyAnimProperties, &var)))
		   goto ERROR_ENCOUNTERED;

		if (var.vt != VT_DISPATCH)
		{ 
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}	
		if(var.pdispVal)
		{
			if (SUCCEEDED(hr = var.pdispVal->QueryInterface(IID_IPropertyMap,(void**)&pAnimPropertiesMap)))
			{	
				var.Clear();
				if (FAILED(hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimStartTime, &var) ))
					goto ERROR_ENCOUNTERED;
				lStartTime = CastToLong(var);
			}
		}
		if (FAILED(hr = pThis->InvokeMethodExt(bstrSetAnimationPropertyRequest, 
												CComDISPPARAMS(	3,
																CComVariant(bstrPropertyName),
																CComVariant(lStartTime), 
																CComVariant(pVector)),
												NULL)))
			goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{
		VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::HandleStopAnimationWithRequest: an error occurred\n");
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::HandleStopAnimationWithRequest: Failed getting correct geometryframe to stop a file animation\n");
		return ReportMMError(hr);
	}	
	return hr;
}

	
	


STDMETHODIMP CMultimediaExemplarObject::SetAnimationIndex(BSTR bstrPropertyName, VARIANT varIndex, VARIANT varOptions)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	static CComBSTR			bstrSetAnimationIndexServer("SetAnimationIndexServer");
	static CComBSTR			bstrHandleSetAnimationIndex("HandleSetAnimationIndex");
	static CString			strFile("File");
	CString					strPropertyName(bstrPropertyName);
	CComBSTR				bstrPropertyAnimOptions(bstrPropertyName);
	CComPtr<IThing>			pThis;
	HRESULT					hr = S_OK;
	VARIANT_BOOL			bPropertyExists, bIsServerSide;
	long					lOptions = 0l;
	float					fIndex = 0.0f;

	//validate the index passed in
	if (varIndex.vt != VT_ERROR) 
	{
		if (!IsNumeric(varIndex))	
			{
				VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::SetAnimationIndex: Found an invalid or non-numeric argument\n");
				hr = E_FAIL;
				goto ERROR_ENCOUNTERED;
			}
		else
			fIndex = CastToFloat(varIndex);
	}
	VariantClear(&varIndex);
	varIndex.vt = VT_R4;
	varIndex.fltVal = fIndex;

	if (varOptions.vt != VT_ERROR) 
	//check the optional args against VT_ERROR to see if they were passed in 
	{
		if (!IsNumeric(varOptions))	
			{
				VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::SetAnimationIndex: Found an invalid or non-numeric argument\n");
				hr = E_FAIL;
				goto ERROR_ENCOUNTERED;
			}
		else
			lOptions = CastToLong(varOptions);
	}
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		goto ERROR_ENCOUNTERED;

	//make sure our property is one of the ones we support for animation by checking if we have a good anim options map
	bstrPropertyAnimOptions.Append("AnimProperties");
	if (FAILED(hr = pThis->IsValidProperty(bstrPropertyAnimOptions, &bPropertyExists)))
		   goto ERROR_ENCOUNTERED;
	if (!bPropertyExists)
	{
		VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::SetAnimationIndex: Unsupported Animation type passed in 1st argument\n");
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	//check if we are client-only in the options passed in, and we are not server-side right now
	//if so, just call the client-side animation handler directly, 
	if ((SUCCEEDED(m_pWorld->get_ServerSide( &bIsServerSide )) && (bIsServerSide == VARIANT_FALSE))&&(lOptions & 32))
	{
		 if (FAILED(hr = pThis->InvokeMethodExt(bstrHandleSetAnimationIndex, 
									CComDISPPARAMS(	3,
													CComVariant(bstrPropertyName),
													CComVariant(varIndex),
													CComVariant(varOptions)),
													NULL)))	
			goto ERROR_ENCOUNTERED;
	}
	else
	{
		//kick off the server method to remote/persist this animation to everyone
		if (FAILED(hr = pThis->InvokeMethodExt(bstrSetAnimationIndexServer, 
								CComDISPPARAMS(	3,
													CComVariant(bstrPropertyName),
													CComVariant(varIndex),
													CComVariant(varOptions)),
													NULL)))
			goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	pThis.Release();

	if (FAILED(hr))
	{	
		VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::SetAnimationIndex: an error occurred\n");
	}
	return hr;
}





STDMETHODIMP CMultimediaExemplarObject::InvokeStaticDoneMethods (IThing* pThing, BSTR bstrPropertyName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HRESULT					hr = S_OK;
	CComBSTR				bstrPropertyAnimProperties(bstrPropertyName);
	CComBSTR				bstrPropertyAnimOptions(bstrPropertyName);
	CComBSTR				bstrPropertyAnimStartTime(bstrPropertyName);
	CComBSTR				bstrPropertyAnimCallBack(bstrPropertyName);
	CComBSTR				bstrDoneMethod;
	CComVariant				var, var2, varFalse;
	CComPtr<IPropertyMap>	pAnimPropertiesMap;
	long					lOptions;
	long					lStartTime;

	varFalse.vt = VT_BOOL;
	varFalse.boolVal = VARIANT_FALSE;

	bstrPropertyAnimProperties.Append("AnimProperties");
	bstrPropertyAnimOptions.Append("AnimOptions");
	bstrPropertyAnimStartTime.Append("AnimStartTime");
	bstrPropertyAnimCallBack.Append("AnimCallBack");

	if (FAILED(hr = pThing->get_Property(bstrPropertyAnimProperties, &var)))
	   goto ERROR_ENCOUNTERED;

	//get any previous good values from it to indicate if a previous Done method invocation is required by the server
	if (var.vt != VT_DISPATCH)
	{ 
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}	
	if(var.pdispVal)
	{
		if (SUCCEEDED(hr = var.pdispVal->QueryInterface(IID_IPropertyMap,(void**)&pAnimPropertiesMap)))
		{	
			//get the start time and the options from any previous animation here,
			//if there is a closed animation indicated to be running (i.e. if its closed and has a nonzero start time),
			//it may have a server-side callback you need to fire off now
			//client side ones will get fired by the renderer on the client when this animation supercedes the old one.
			
			if (FAILED(hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimOptions , &var2) ))
				goto ERROR_ENCOUNTERED;
			lOptions = CastToLong(var2);

			var2.Clear();
			if (FAILED(hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimStartTime, &var2) ))
				goto ERROR_ENCOUNTERED;
			lStartTime = CastToLong(var2);

			var2.Clear();
			if (FAILED(hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimCallBack, &var2) ))
				goto ERROR_ENCOUNTERED;

			if (var2.vt == VT_BSTR)
			{
				bstrDoneMethod = var2.bstrVal;
				if ( ((lOptions & 2) && ( lStartTime > 0l )) && (IsMethodThisSide(pThing, bstrDoneMethod)) )
				{
					//ok, we need to invoke the old callback now, if the current time indicates it hasnt expired...
					hr = pThing->InvokeMethodExt(CComBSTR("FinishAnimation"),
												CComDISPPARAMS(	5,
												CComVariant(varFalse),
												CComVariant(varFalse),
												CComVariant(bstrPropertyName),
												CComVariant(bstrDoneMethod),
												CComVariant((IDispatch*)NULL)),
												NULL);
				}
			}
		}
		else
			goto ERROR_ENCOUNTERED;	
	}

ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{	
		VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::InvokeStaticDoneMethods: an error occurred\n");
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::InvokeStaticDoneMethods: Error dealing with an animation property Map\n");
	}
	return hr;
}



STDMETHODIMP CMultimediaExemplarObject::RemovePriorAnimations (IThing* pThing, BSTR bstrPropertyName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	HRESULT					hr = S_OK;
	CComBSTR				bstrPropertySetPropAnimation(bstrPropertyName) , bstrCallBack("");
	CComVariant				var, varValue, varTrue, varFalse;
	CComPtr<IVWAnimator>	pVWAnimator;
	CComPtr<IVWAnimation>	pAnimation, pPropSetAnimation;
	static CComBSTR			bstrAnimator("Animator");
	CComBSTR				bstrMethod;
	DISPPARAMS*				pDispparams;
	
	varTrue.vt = VT_BOOL;
	varTrue.boolVal = VARIANT_TRUE;
	varFalse.vt = VT_BOOL;
	varFalse.boolVal = VARIANT_FALSE;

	bstrPropertySetPropAnimation.Append ("SetPropAnimation");

	//if a property setter animation is already running on this property
	//kill the animation managing the callback now, and call finish animation with completed = false

	if (SUCCEEDED(hr = pThing->get_Property( bstrPropertySetPropAnimation, &var)))
	{
		if(var.pdispVal)
		{
			if (SUCCEEDED(hr = var.pdispVal->QueryInterface(IID_IVWAnimation,(void**)&pPropSetAnimation)))
			{
				if (FAILED(hr = m_pWorld->get_Tool(bstrAnimator, (IUnknown**)&pVWAnimator)))
					goto ERROR_ENCOUNTERED;
				if (SUCCEEDED(hr = pVWAnimator->Remove(pPropSetAnimation)))
				{	
					if (SUCCEEDED(hr = pPropSetAnimation->get_Method(&bstrMethod.m_str)))
					{
						//if we got here, the animation was not scheduled to finish yet
						if (SUCCEEDED(hr = pPropSetAnimation->get_ArgumentsExt(&pDispparams)))
						{
							pPropSetAnimation.Release();

							//unpack the dispparam args 
							ASSERT ((pDispparams->cArgs)>4);
							if ( ((pDispparams->cArgs)<5) || (pDispparams->rgvarg[3].vt != VT_BOOL) ||
								(pDispparams->rgvarg[2].vt != VT_BSTR) || (pDispparams->rgvarg[1].vt != VT_BSTR) ||
								( (pDispparams->rgvarg[0].vt != VT_DISPATCH) && (pDispparams->rgvarg[0].vt != VT_R4) )  )
							{
								VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::RemovePriorAnimations: bad dispparams in the stored callback animation\n");
								VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::RemovePriorAnimations: bad dispparams in the stored callback animation\n");
								return E_FAIL;
							}
							//bstrPropertyName = pDispparams->rgvarg[1].bstrVal;	
							bstrCallBack = pDispparams->rgvarg[1].bstrVal;
							varValue = pDispparams->rgvarg[0];
							TerminateDISPPARAMS(VARIANT_TRUE, pDispparams);
							hr = pThing->InvokeMethodExt(bstrMethod,
												CComDISPPARAMS(	5, 
																CComVariant(varTrue),
																CComVariant(varFalse),
																CComVariant(bstrPropertyName),
																CComVariant(bstrCallBack),
																CComVariant(varValue)),
																NULL);
						}
					}
				}			
	
			}
		}
	}	


	//erase the pointer to the prop setter animation 
	if (FAILED(hr = pThing->put_Property( bstrPropertySetPropAnimation, CComVariant((IDispatch*)NULL)) ))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{	
		VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::RemovePriorAnimations: an error occurred\n");
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::RemovePriorAnimations: Error dealing with an animation property Map\n");
	}

	return hr;
}





STDMETHODIMP CMultimediaExemplarObject::ValidateAnimationPath (VARIANT varPathMap, IVector** ppNearest, float* pfEndRange, float* pfStartRange)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	CComPtr<IVector>		pVector, pHighest, pNearest;
	CComPtr<IPropertyMap>	pPathMap;
	float					fDuration = 0.0f;
	float					fLastTime, fTime;
	HRESULT					hr = S_OK;
	CComVariant				var;
	VARIANT_BOOL			bDone, bIsEmpty;
	CComBSTR				bstrKeyName;

	if (ppNearest == NULL || pfEndRange == NULL || pfStartRange == NULL)
	{
		hr = E_POINTER;
		goto ERROR_ENCOUNTERED;
	}
	//make sure our path map is valid and has all good numeric string keys and vector values in it
	//also set a good default range/duration for the animation if it wasn't specified
	if (varPathMap.vt == VT_ERROR) 
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (varPathMap.vt != VT_DISPATCH) 
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = varPathMap.pdispVal->QueryInterface(IID_IPropertyMap,(void**)&pPathMap)))
		goto ERROR_ENCOUNTERED;
	if (!pPathMap)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pPathMap->get_IsEmpty(&bIsEmpty)))
		goto ERROR_ENCOUNTERED;
	if (bIsEmpty)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	hr = pPathMap->FirstItem(&bstrKeyName.m_str, &var, &bDone);

	if (FAILED(hr) && hr != VWOBJECT_E_PERMISSION_DENIED)
		goto ERROR_ENCOUNTERED;
	else 
		if (hr == VWOBJECT_E_PERMISSION_DENIED)
		{
			VWREPORT(m_pWorld, VWR_ERROR,  "CMultimediaExemplarObject::ValidateAnimationPath: No permissions to read the specified property map\n");
			goto ERROR_ENCOUNTERED;
		}
	//store the first vector now in case it turns out to be the value of the key closest to the end range.
	if (var.vt != VT_DISPATCH) 
		goto ERROR_ENCOUNTERED;	
	if (FAILED(hr = var.pdispVal->QueryInterface(IID_IVector,(void**)&pNearest)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = var.pdispVal->QueryInterface(IID_IVector,(void**)&pHighest)))
		goto ERROR_ENCOUNTERED;

	fLastTime = *pfEndRange;

	//loop through the map and examine all the values...
	while (bDone != VARIANT_TRUE)
	{
		//fail on finding anything that cant be a vector
		if (var.vt != VT_DISPATCH) 
		{
			VWREPORT(m_pWorld, VWR_ERROR,  "CMultimediaExemplarObject::ValidateAnimationPath: invalid contents in the animation property map \n");
			goto ERROR_ENCOUNTERED;
		}
		//make sure the values in the map are vectors
		pVector.Release();
		if (FAILED(hr = var.pdispVal->QueryInterface(IID_IVector,(void**)&pVector)))
		{
			VWREPORT(m_pWorld, VWR_ERROR,  "CMultimediaExemplarObject::ValidateAnimationPath: the animation path map is invalid or contains non-vector types\n");
			goto ERROR_ENCOUNTERED;
		}
		//keep track of the ending time for the maximum duration, nearest prop key for property set
		fTime = (float)(atoi(CString(bstrKeyName)));
		if (fDuration < fTime)  
		{
			fDuration = fTime;
			pHighest.Release();
			pHighest = pVector;
		}
		if (*pfEndRange > 0.0f)
            if (  (fabs(*pfEndRange - fTime))	< fLastTime	 )
			{
				pNearest.Release();
				pNearest = pVector;
				fLastTime = (float)(fabs(*pfEndRange - fTime));
			}
		bstrKeyName.Empty();
		var.Clear();
		if (FAILED(hr = pPathMap->NextItem(&bstrKeyName.m_str, &var, &bDone)))
			goto ERROR_ENCOUNTERED;
	}
	//validate our ranges
	if ((*pfEndRange < 0.0f) || (*pfEndRange > fDuration))
	{	
		*pfEndRange = fDuration;
		pNearest.Release();
		pNearest = pHighest;
	}
	if (*pfStartRange > *pfEndRange)
		*pfStartRange = *pfEndRange;
	else if (*pfStartRange < 0.0f)
		*pfStartRange = 0.0f;

	*ppNearest = pNearest;
	SAFEADDREF(*ppNearest);

ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{	
		VWREPORT(m_pWorld, VWR_ERROR,  "CMultimediaExemplarObject::ValidateAnimationPath: An error occurred\n");
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::ValidateAnimationPath: Error dealing with an animation property Map\n");
	}

	return hr;

}


STDMETHODIMP CMultimediaExemplarObject::StartAnimationServer(BSTR bstrPropertyName, VARIANT varPathMap, VARIANT varCallBack, VARIANT varOptions, VARIANT varTimeScale, VARIANT varStartRange, VARIANT varEndRange , VARIANT varIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CComPtr<IThing>			pThis;
	CComPtr<IVector>		pNearest;
	HRESULT					hr = S_OK;
	static CComBSTR			bstrHandleStartAnimation("HandleStartAnimation");
	CComBSTR				bstrPropertyAnimOptions(bstrPropertyName);
	CComBSTR				bstrPropertyAnimStartTime(bstrPropertyName);
	CComBSTR				bstrPropertyAnimIndex(bstrPropertyName);
	CComBSTR				bstrPropertyAnimTimeScale(bstrPropertyName);
	CComBSTR				bstrPropertyAnimRangeStart(bstrPropertyName);
	CComBSTR				bstrPropertyAnimRangeEnd(bstrPropertyName);
	CComBSTR				bstrPropertyAnimPath(bstrPropertyName);	
	CComBSTR				bstrPropertyAnimProperties(bstrPropertyName);
	CComBSTR				bstrPropertyAnimCallBack(bstrPropertyName);
	CComBSTR				bstrPropertySetPropAnimation(bstrPropertyName);
	CComBSTR				bstrPropertyAnimFinalValue(bstrPropertyName);
	CComBSTR				bstrCallBack, bstrPreviousCallBack;
	static CString			strFile("File");
	CString					strPropertyName(bstrPropertyName);
	float					fTimeScale = 1.0f;
	float					fStartRange = -1.0f;
	float					fEndRange = -1.0f;
	float					fIndex = 0.0f;
	long					lOptions = 5l;  //default should be closed, linear?
	long					lSystemTime = 0l;
	CComPtr<IPropertyMap>	pPathMap;
	CComPtr<IPropertyMap>	pAnimPropertiesMap;
	CComPtr<IVWAnimator>	pVWAnimator;
	CComPtr<IVWAnimation>	pAnimation, pPropSetAnimation;
	static CComBSTR			bstrAnimator("Animator");
	CComBSTR				bstrMethod;
	CComVariant				var, varTrue;
	
	varTrue.vt = VT_BOOL;
	varTrue.boolVal = VARIANT_TRUE;

	//if args were not optional, retrieve them (we validated them for good values already in the initial call)
	if (varOptions.vt != VT_ERROR) 
		lOptions = CastToLong(varOptions);
	if (varTimeScale.vt != VT_ERROR) 
	{
		fTimeScale = CastToFloat(varTimeScale);
		fTimeScale = ((fTimeScale) < 0 ? -(fTimeScale) : (fTimeScale));
	}
	if (varStartRange.vt != VT_ERROR) 
		fStartRange = CastToFloat(varStartRange);
	if (varIndex.vt != VT_ERROR) 
	{
		fIndex = CastToFloat(varIndex);
		fIndex = ((fIndex) < 0 ? -(fIndex) : (fIndex));
	}
	if (varEndRange.vt != VT_ERROR) 
		fEndRange = CastToFloat(varEndRange);
	if (varCallBack.vt != VT_ERROR) 
		bstrCallBack = varCallBack.bstrVal;
	else
		bstrCallBack = ("");

	//get the right property + key names to use for storing this animation's persisted values
	bstrPropertyAnimOptions.Append("AnimOptions");
	bstrPropertyAnimStartTime.Append("AnimStartTime");
	bstrPropertyAnimIndex.Append("AnimIndex");
	bstrPropertyAnimTimeScale.Append("AnimTimeScale");
	bstrPropertyAnimRangeStart.Append("AnimRangeStart");
	bstrPropertyAnimRangeEnd.Append("AnimRangeEnd");
	bstrPropertyAnimPath.Append("AnimPath");
	bstrPropertyAnimCallBack.Append ("AnimCallBack");
	bstrPropertySetPropAnimation.Append("SetPropAnimation");
	bstrPropertyAnimFinalValue.Append ("AnimFinalValue");

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	if (FAILED(hr = RemovePriorAnimations(pThis, bstrPropertyName)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = InvokeStaticDoneMethods(pThis, bstrPropertyName)))
		goto ERROR_ENCOUNTERED;

	if (SUCCEEDED(hr = pThis->get_Property( bstrPropertySetPropAnimation, &var)))
	{
		if(var.pdispVal)
		{
			if (SUCCEEDED(hr = var.pdispVal->QueryInterface(IID_IVWAnimation,(void**)&pPropSetAnimation)))
			{
				//if there is still an animation available after removal, 
				//it means the done method that was invoked has started a new animation, so this one should fail
				VWREPORT(m_pWorld, VWR_ERROR,  "CMultimediaExemplarObject::StartAnimationServer: Can't start an animation if a previous OnDone method has already initiated the same type...\n");
				hr = E_FAIL;
				goto ERROR_ENCOUNTERED;
				
			}
		}
	}
	else
		goto ERROR_ENCOUNTERED;

	var.Clear();



	if (strPropertyName.CompareNoCase(strFile)!=0) 
	{
		if (FAILED(hr = ValidateAnimationPath(varPathMap, &pNearest, &fEndRange, &fStartRange)))
			goto ERROR_ENCOUNTERED;

		//store the path
		if (FAILED(hr = pThis->put_Property(bstrPropertyAnimPath, CComVariant(varPathMap))))
			goto ERROR_ENCOUNTERED;
	}
	//make sure our index is in the range
	if (fIndex < fStartRange) 
		fIndex = fStartRange;
	if (fIndex > fEndRange)
		fIndex = fEndRange;
	
	//write the good props to the animproperties map	
	if (FAILED(hr = CreatePropertyMap(m_pWorld, &pAnimPropertiesMap)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAnimPropertiesMap->put_Property( bstrPropertyAnimOptions , CComVariant((short)lOptions) )))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAnimPropertiesMap->put_Property( bstrPropertyAnimIndex, CComVariant(fIndex) )))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAnimPropertiesMap->put_Property( bstrPropertyAnimCallBack, CComVariant(bstrCallBack) )))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAnimPropertiesMap->put_Property( bstrPropertyAnimTimeScale, CComVariant(fTimeScale) )))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAnimPropertiesMap->put_Property( bstrPropertyAnimRangeStart, CComVariant(fStartRange) )))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAnimPropertiesMap->put_Property( bstrPropertyAnimRangeEnd, CComVariant(fEndRange) )))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAnimPropertiesMap->put_Property( bstrPropertyAnimStartTime, CComVariant(CurrentTime()) )))
		goto ERROR_ENCOUNTERED;		

	//if the callback method is on the server, set up a server-side animator tool to do the callback
	//also dont send a callback param to the clients at all
	if (lOptions & 1) //if this animation is scheduled to end in the future, set up callbacks	
	{
		if (FAILED(hr = m_pWorld->get_Tool(bstrAnimator, (IUnknown**)&pVWAnimator)))
			goto ERROR_ENCOUNTERED;	

		if (FAILED(hr = pVWAnimator->CreateClosedExt(	((int)(((fIndex)>(fStartRange)?(fEndRange-fIndex):(fEndRange-fStartRange))/fTimeScale)), 
														((int)1), 
														pThis, 
														CComBSTR("FinishAnimation"),
														CComDISPPARAMS(	5,
																		CComVariant(varTrue),
																		CComVariant(varTrue),
																		CComVariant(bstrPropertyName),
																		((bstrCallBack.Length()&&(IsMethodThisSide(pThis, bstrCallBack))) ? CComVariant(bstrCallBack) : CComVariant(CComBSTR("")) ),
																		((strPropertyName.CompareNoCase(strFile)!=0) ? CComVariant((IDispatch*)pNearest) : CComVariant(fEndRange) ),
																		NULL),
														&pPropSetAnimation )))
			goto ERROR_ENCOUNTERED;	
		pVWAnimator.Release();
	}


	//store the property set callback animation so we can kill it if we later interrupt this animation from code
	if (FAILED(hr = pThis->put_Property( bstrPropertySetPropAnimation, CComVariant((IDispatch*)pPropSetAnimation) )))
		goto ERROR_ENCOUNTERED;	
	//also store the vector as the final value in the animproperties map
	if (FAILED(hr = pAnimPropertiesMap->put_Property( bstrPropertyAnimFinalValue, ((strPropertyName.CompareNoCase(strFile)!=0) ? CComVariant((IDispatch*)pNearest) : CComVariant(fEndRange) ) )))
		goto ERROR_ENCOUNTERED;		

	
	//put the whole map of animation properties
	bstrPropertyAnimProperties.Append("AnimProperties");

	if (FAILED(hr = pThis->put_Property(bstrPropertyAnimProperties, CComVariant(pAnimPropertiesMap))))
		goto ERROR_ENCOUNTERED;

	//now fire off the animation on the clients 
	//make sure a server-side OnDone method isn't passed to all the clients...
	hr = pThis->InvokeMethodExt(			bstrHandleStartAnimation, 
											CComDISPPARAMS(	8,
															CComVariant(bstrPropertyName),
															CComVariant(varPathMap),
															((bstrCallBack.Length()&&(IsMethodThisSide(pThis, bstrCallBack))) ? CComVariant(CComBSTR("")) : CComVariant(bstrCallBack)),
															CComVariant(lOptions),
															CComVariant(fTimeScale),
															CComVariant(fStartRange),
															CComVariant(fEndRange),
															CComVariant(fIndex)), 
															NULL);
ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{	
		VWREPORT(m_pWorld, VWR_ERROR,  "CMultimediaExemplarObject::StartAnimationServer: An error occurred\n");
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::StartAnimationServer: Error dealing with an animation property Map\n");
	}
	return hr;
}



STDMETHODIMP CMultimediaExemplarObject::FinishAnimation(VARIANT varSetProp, VARIANT varCompleted, BSTR bstrPropertyName, BSTR bstrCallBack, VARIANT varValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	CComPtr<IThing>			pThis;
	CComBSTR				bstrPropertyAnimStartTime(bstrPropertyName);
	CComBSTR				bstrPropertyAnimProperties(bstrPropertyName);
	CComVariant				var;
	HRESULT					hr = S_OK;
	CComPtr<IPropertyMap>	pAnimPropertiesMap;
	static CString			strFile("File");
	CString					strPropertyName(bstrPropertyName);
	CComBSTR				bstrFileAnimIndex("FileAnimIndex");
	CComBSTR				bstrPropName(bstrPropertyName), bstrCall(bstrCallBack);
	CComBSTR	bstrPropertySetPropAnimation(bstrPropertyName);
	bstrPropertySetPropAnimation.Append ("SetPropAnimation");

	if (  (!(bstrPropName.Length()) || (varCompleted.vt != VT_BOOL)) || (varSetProp.vt != VT_BOOL) )
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		goto ERROR_ENCOUNTERED;

	//erase the pointer to the prop setter animation that fired this.
	if (FAILED(hr = pThis->put_Property( bstrPropertySetPropAnimation, CComVariant((IDispatch*)NULL)) ))
		goto ERROR_ENCOUNTERED;

	if (varCompleted.boolVal) 
		if (strPropertyName.CompareNoCase(strFile)!=0)
			if (FAILED(hr = pThis->put_Property(bstrPropertyName, CComVariant(varValue))))
				goto ERROR_ENCOUNTERED;
	
	bstrPropertyAnimStartTime.Append("AnimStartTime");
	bstrPropertyAnimProperties.Append("AnimProperties");

	if (FAILED(hr = pThis->get_Property(bstrPropertyAnimProperties, &var)))
	   goto ERROR_ENCOUNTERED;
	else
	{
		//write a zero into the starttime in the animation properties map
		if (var.vt != VT_DISPATCH)
		{ 
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}	
		if(var.pdispVal)
		{
			if (SUCCEEDED(hr = var.pdispVal->QueryInterface(IID_IPropertyMap,(void**)&pAnimPropertiesMap)))
			{
				//stamp the starttime as zero in the correct animproperties map for this animation
				if (varSetProp.boolVal)
					if (FAILED(hr = pAnimPropertiesMap->put_Property(bstrPropertyAnimStartTime, CComVariant((long)0l) )))
						goto ERROR_ENCOUNTERED;	
				//if this was a file animation, also write the index
				if (strPropertyName.CompareNoCase(strFile)==0)
					if ((varCompleted.boolVal) && (varSetProp.boolVal))
						if (FAILED(hr = pAnimPropertiesMap->put_Property(bstrFileAnimIndex, CComVariant(varValue) )))
							goto ERROR_ENCOUNTERED;	
			}
		}
	}
	//now, if there is a server-side done method, invoke it with the completed flag and the current time
	if (bstrCall.Length())
	{
		hr = pThis->InvokeMethodExt(CComBSTR(bstrCallBack),
											CComDISPPARAMS(2, 
											CComVariant(varCompleted.boolVal),
											CComVariant(CurrentTime())),
											NULL);
	}




ERROR_ENCOUNTERED:
	
	if (FAILED(hr))
	{	
		VWREPORT(m_pWorld, VWR_ERROR,  "CMultimediaExemplarObject::FinishAnimation: An error occurred\n");
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::FinishAnimation: an error occurred\n");
	}

	return hr;
}


STDMETHODIMP CMultimediaExemplarObject::StopAnimationServer(BSTR bstrPropertyName, VARIANT varValue, VARIANT varOptions)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	CComPtr<IThing>			pThis;
	CComBSTR				bstrPropertyAnimStartTime(bstrPropertyName);
	CComBSTR				bstrPropertyAnimProperties(bstrPropertyName);
	CComBSTR				bstrPropertySetPropAnimation(bstrPropertyName);
	CComPtr<IPropertyMap>	pAnimPropertiesMap;
	CComPtr<IVWAnimator>	pVWAnimator;
	CComPtr<IVWAnimation>	pAnimation, pPropSetAnimation;
	CComPtr<IVector>		pVector;
	HRESULT					hr = S_OK;
	CComVariant				var;
	static CComBSTR			bstrHandleStopAnimation("HandleStopAnimation");
	static CComBSTR			bstrFileAnimIndex("FileAnimIndex");
	long					lOptions;
	static CComBSTR			bstrAnimator("Animator");
	static CString			strFile("File");
	CString					strPropertyName(bstrPropertyName);

	lOptions = CastToLong(varOptions);
	bstrPropertyAnimStartTime.Append("AnimStartTime");
	bstrPropertyAnimProperties.Append("AnimProperties");
	bstrPropertySetPropAnimation.Append("SetPropAnimation");

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;
	if (FAILED(hr = pThis->get_Property(bstrPropertyAnimProperties, &var)))
	   goto ERROR_ENCOUNTERED;
	else
	{
		//write a zero into the starttime in the animation properties map
		if (var.vt != VT_DISPATCH)
		{ 
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}	
		if(var.pdispVal)
		{
			if (SUCCEEDED(hr = var.pdispVal->QueryInterface(IID_IPropertyMap,(void**)&pAnimPropertiesMap)))
			{
				//stamp the starttime as zero in the correct animproperties map for this animation
				if (FAILED(hr = pAnimPropertiesMap->put_Property( bstrPropertyAnimStartTime, CComVariant((long)0l) )))
					goto ERROR_ENCOUNTERED;		
			}
			else
				goto ERROR_ENCOUNTERED;	
		}
	}

	//fire off handlestopanimation on the clients
	hr = pThis->InvokeMethodExt(		bstrHandleStopAnimation, 
										CComDISPPARAMS(	2,
														CComVariant(bstrPropertyName),
														CComVariant(lOptions)),
										NULL);
	//now that we have stopped on the clients, stop on the server too (could restart a new animation)
	if (FAILED(hr = RemovePriorAnimations(pThis, bstrPropertyName)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = InvokeStaticDoneMethods(pThis, bstrPropertyName)))
		goto ERROR_ENCOUNTERED;
	else
	{
		if (varValue.pdispVal)
		{
			if (FAILED(hr = varValue.pdispVal->QueryInterface(IID_IVector,(void**)&pVector)))
					goto ERROR_ENCOUNTERED;
			//now set the final property value here...
			if (strPropertyName.CompareNoCase(strFile)==0) 
			{
				float fX, fY, fZ;
				pVector->get(&fX, &fY, &fZ);
				if (pAnimPropertiesMap)
					if (FAILED(hr = pAnimPropertiesMap->put_Property( bstrFileAnimIndex, CComVariant(fX) )))
						goto ERROR_ENCOUNTERED;	
			}
			else if (FAILED(hr = pThis->put_ObjectProperty(bstrPropertyName, pVector)))
					goto ERROR_ENCOUNTERED;	
		}
	}


ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{
		VWREPORT(m_pWorld, VWR_ERROR,  "CMultimediaExemplarObject::StopAnimationServer: An error occurred\n");
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::StopAnimationServer: Error\n");
		return ReportMMError(hr);
	}

	return hr;
}





STDMETHODIMP CMultimediaExemplarObject::SetAnimationIndexServer(BSTR bstrPropertyName, VARIANT varIndex, VARIANT varOptions)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CComPtr<IThing>			pThis;
	HRESULT					hr = S_OK;
	static CComBSTR			bstrHandleSetAnimationIndex("HandleSetAnimationIndex");
	CComBSTR				bstrPropertyAnimOptions(bstrPropertyName);
	CComBSTR				bstrPropertyAnimStartTime(bstrPropertyName);
	CComBSTR				bstrPropertyAnimIndex(bstrPropertyName);
	CComBSTR				bstrPropertyAnimTimeScale(bstrPropertyName);
	CComBSTR				bstrPropertyAnimRangeStart(bstrPropertyName);
	CComBSTR				bstrPropertyAnimRangeEnd(bstrPropertyName);
	CComBSTR				bstrPropertyAnimPath(bstrPropertyName);	
	CComBSTR				bstrPropertyAnimProperties(bstrPropertyName);
	CComBSTR				bstrPropertyAnimCallBack(bstrPropertyName);
	CComBSTR				bstrPropertySetPropAnimation(bstrPropertyName);
	CComBSTR				bstrCallBack;
	static CString			strFile("File");
	CString					strPropertyName(bstrPropertyName);
	float					fTimeScale = 1.0f;
	float					fStartRange = 0.0f;
	float					fEndRange = 0.0f;
	long					lOptions = 5l;  //default should remoted
	long					lPreviousOptions = 0l;
	long					lSystemTime = 0l;
	long					lStartTime = 0l;
	CComPtr<IPropertyMap>	pAnimPropertiesMap;
	CComPtr<IVWAnimator>	pVWAnimator;
	CComPtr<IVWAnimation>	pAnimation, pPropSetAnimation;
	static CComBSTR			bstrAnimator("Animator");
	CComBSTR				bstrName;
	BSTR					bstrMethod;
	CComVariant				var;
	DISPPARAMS*				pDispparams;
	float					fIndex = 0.0f;

	//if args were not optional, retrieve them (we validated them for good values already in the initial call)
	if (varOptions.vt != VT_ERROR) 
		lOptions = CastToLong(varOptions);

	fIndex = CastToFloat(varIndex);

	//check to make sure SetanimationIndexServer wasnt called from a server-side StartAnimation with a client-only flag, before we set any properties
	if (lOptions & 32) 
	{
		VWREPORT(m_pWorld, VWR_ERROR,  "CMultimediaExemplarObject::SetAnimationIndexServer: Cannot set the index from the server if client-side flag is specified\n");
		goto ERROR_ENCOUNTERED;
	}
	//get the right property + key names to use for retrieving this animation's persisted values
	bstrPropertyAnimOptions.Append("AnimOptions");
	bstrPropertyAnimStartTime.Append("AnimStartTime");
	bstrPropertyAnimIndex.Append("AnimIndex");
	bstrPropertyAnimTimeScale.Append("AnimTimeScale");
	bstrPropertyAnimRangeStart.Append("AnimRangeStart");
	bstrPropertyAnimRangeEnd.Append("AnimRangeEnd");
	bstrPropertyAnimPath.Append("AnimPath");
	bstrPropertyAnimCallBack.Append ("AnimCallBack");
	bstrPropertyAnimProperties.Append("AnimProperties");
	bstrPropertySetPropAnimation.Append("SetPropAnimation");
	
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		goto ERROR_ENCOUNTERED;
	//make sure an animation properties map exists for this property
	if (FAILED(hr = pThis->get_Property(bstrPropertyAnimProperties, &var)))
	   goto ERROR_ENCOUNTERED;
	else
	{
		//get the relevant range/etc. and options info from the animation properties map
		if (var.vt != VT_DISPATCH)
		{ 
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}	
		if(var.pdispVal)
		{
			if (SUCCEEDED(hr = var.pdispVal->QueryInterface(IID_IPropertyMap,(void**)&pAnimPropertiesMap)))
			{
				//retrieve the values
					var.Clear();
					if (FAILED(hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimRangeStart , &var) ))
						goto ERROR_ENCOUNTERED;
					fStartRange = CastToFloat(var);

					var.Clear();
					if (FAILED(hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimRangeEnd, &var) ))
						goto ERROR_ENCOUNTERED;
					fEndRange = CastToFloat(var);

					var.Clear();
					if (FAILED(hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimTimeScale, &var) ))
						goto ERROR_ENCOUNTERED;
					fTimeScale = CastToFloat(var);

					var.Clear();
					if (FAILED(hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimOptions, &var) ))
						goto ERROR_ENCOUNTERED;
					lPreviousOptions = CastToLong(var);
					var.Clear();
					if (FAILED(hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimStartTime, &var) ))
						goto ERROR_ENCOUNTERED;
					lStartTime = CastToLong(var);
					var.Clear();
			}
			else
				goto ERROR_ENCOUNTERED;	
		}
	}
	var.Clear();

	//make sure this animation was scheduled to end
	if ((lPreviousOptions & 1)&&(lStartTime != 0)) 
	{
		//if an property setter animation is already running on this property
		//kill the animation managing the property set callback now, and start a new callback with the new duration.
		if (SUCCEEDED(hr = pThis->get_Property( bstrPropertySetPropAnimation, &var)))
		{
			if(var.pdispVal)
			{
				if (SUCCEEDED(hr = var.pdispVal->QueryInterface(IID_IVWAnimation,(void**)&pPropSetAnimation)))
				{
					if (FAILED(hr = m_pWorld->get_Tool(bstrAnimator, (IUnknown**)&pVWAnimator)))
						goto ERROR_ENCOUNTERED;

					if (SUCCEEDED(hr = pVWAnimator->Remove(pPropSetAnimation)))
					{	
						if (SUCCEEDED(hr = pPropSetAnimation->get_Method(&bstrMethod)))
						{
							//if we got here, the animation was not scheduled to finish yet, so start a new prop set callback
							//use the same params as before, we have them in the old animation
							if (SUCCEEDED(hr = pPropSetAnimation->get_ArgumentsExt(&pDispparams)))
							{
								pPropSetAnimation.Release();
								if (FAILED(hr = pVWAnimator->CreateClosedExt(	((int)((fEndRange-fStartRange+fIndex)/fTimeScale)), 
																				((int)1), 
																				pThis, 
																				CComBSTR(bstrMethod),
																				pDispparams,
																				&pPropSetAnimation )))
									goto ERROR_ENCOUNTERED;	
								//store this callback in a property in case its interrupted again.
								if (FAILED(hr = pThis->put_Property( bstrPropertySetPropAnimation, CComVariant((IDispatch*)pPropSetAnimation) )))
									goto ERROR_ENCOUNTERED;	
							}
						}
					}			
				}
			}
		}
	}

	pAnimation.Release();
	pPropSetAnimation.Release();
	pVWAnimator.Release();
	var.Clear();

	//write the new index of the animation and a new start time into its properties map for future clients.
	if (FAILED(hr = pAnimPropertiesMap->put_Property( bstrPropertyAnimIndex, CComVariant(fIndex) )))
		goto ERROR_ENCOUNTERED;
	if (lStartTime > 0l)
	{
		if (FAILED(hr = pAnimPropertiesMap->put_Property( bstrPropertyAnimStartTime, CComVariant((long)CurrentTime()) )))
			goto ERROR_ENCOUNTERED;	
	}
	//fire off the index change to all the clients 
	hr = pThis->InvokeMethodExt(			bstrHandleSetAnimationIndex, 
											CComDISPPARAMS(	3,
															CComVariant(bstrPropertyName), 
															CComVariant(varIndex), 
															CComVariant(varOptions)),
															NULL);
ERROR_ENCOUNTERED:
	pAnimPropertiesMap.Release();
	bstrName.Empty();
	var.Clear();

	if (FAILED(hr))
	{	
		VWREPORT(m_pWorld, VWR_ERROR,  "CMultimediaExemplarObject::SetAnimationIndexServer: An Error Occurred\n");
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::SetAnimationindexServer: An Error Occurred\n");
	}
	return hr;

}






STDMETHODIMP CMultimediaExemplarObject::HandleStartAnimation(BSTR bstrPropertyName, VARIANT varPathMap, VARIANT varCallBack, VARIANT varOptions, VARIANT varTimeScale, VARIANT varStartRange, VARIANT varEndRange, VARIANT varIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	char					buff[200];
	CComBSTR				bstrInterpName;
	CComBSTR				bstrCallBack;
	CComBSTR				bstrDeleteInterp("DeleteAnimation");
	CComBSTR				bstrStartProp(bstrPropertyName);
	static CString			strFile("File");
	static CString			strPosition("Position");
	static CString			strDirection("Direction");
	static CString			strUp("Up");
	static CString			strScale("Scale");
	static CString			strLightColor("LightColor");
	static CString			strLightAttenuation("LightAttenuation");
	CString					strPropertyName(bstrPropertyName);
	CComPtr<IThing>			pThis;
	CComPtr<IPropertyMap>	pPathMap;
	HRESULT					hr = S_OK;
	float					fTimeScale = 1.0f;
	long					lOptions = 5l;
	long					lSystemTime;
	float					fStartRange = -1.0f;
	float					fEndRange = -1.0f;
	float					fIndex = 0.0f;
	float					fDuration = 1000.0f;
	VARIANT_BOOL			bIsValidMethod, bIsEmpty;

	//get the this pointer
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		goto ERROR_ENCOUNTERED;
	//get the options and other anim vals
	if (varCallBack.vt == VT_BSTR && CComBSTR(varCallBack.bstrVal).Length() != 0)
	{
		if (FAILED(hr = pThis->IsValidMethod(varCallBack.bstrVal, &bIsValidMethod)))
			goto ERROR_ENCOUNTERED;
		if (!bIsValidMethod)
		{
			VWREPORT(m_pWorld, VWR_ERROR,  "CMultimediaExemplarObject::HandleStartAnimation: Invalid Method specified for animation callback\n");
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		bstrCallBack = varCallBack.bstrVal;
	}
	//verify the the variant has a string in it
	else if (varCallBack.vt != VT_BSTR && varCallBack.vt != VT_ERROR)
	{
		VWREPORT(m_pWorld, VWR_ERROR,  "CMultimediaExemplarObject::HandleStartAnimation: Argument 3 (Method Callback) must be empty or a string\n");
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	
	if (varOptions.vt != VT_ERROR) 
		lOptions = CastToLong(varOptions);

	if (varTimeScale.vt != VT_ERROR) 
	{
		fTimeScale = CastToFloat(varTimeScale);
		fTimeScale = ((fTimeScale) < 0 ? -(fTimeScale) : (fTimeScale));
	}
	if (varIndex.vt != VT_ERROR) 
	{
		fIndex = CastToFloat(varIndex);
		fIndex = ((fIndex) < 0 ? -(fIndex) : (fIndex));
	}

	if (varStartRange.vt != VT_ERROR) 
		fStartRange = CastToFloat(varStartRange);
	if (varEndRange.vt != VT_ERROR) 
		fEndRange = CastToFloat(varEndRange);
	
	if (strPropertyName.CompareNoCase(strFile)!=0) 
	{	
		//we are starting a property animation, get the property path for keyframes
		if (varPathMap.vt == VT_ERROR) 
		{	hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		} 
		if (varPathMap.vt != VT_DISPATCH) 
		{	hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		} 
		if (FAILED(hr = varPathMap.pdispVal->QueryInterface(IID_IPropertyMap,(void**)&pPathMap)))
			goto ERROR_ENCOUNTERED;
		if (!pPathMap)
		{	hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		} 
		if (FAILED(hr = pPathMap->get_IsEmpty(&bIsEmpty)))
			goto ERROR_ENCOUNTERED;

		_ltoa((long)(void*)pThis,buff,16);
		bstrInterpName = buff;
		bstrInterpName.Append(bstrPropertyName);

		lSystemTime = GetTickCount();

		//call the correct helper for this animation
		if (strPropertyName.CompareNoCase(strPosition)==0)
			hr = StartPositionPathAnimation(pPathMap, fTimeScale, fStartRange, fEndRange, fIndex, bstrCallBack ,lOptions, pThis,lSystemTime);
		else if (strPropertyName.CompareNoCase(strDirection)==0)
			hr = StartDirectionPathAnimation(pPathMap, fTimeScale, fStartRange, fEndRange, fIndex, bstrCallBack ,lOptions, pThis,lSystemTime);
		else if (strPropertyName.CompareNoCase(strUp)==0)
			hr = StartUpPathAnimation(pPathMap,fTimeScale, fStartRange, fEndRange, fIndex, bstrCallBack ,lOptions, pThis,lSystemTime);
		else if (strPropertyName.CompareNoCase(strScale)==0)
			hr = StartScalePathAnimation(pPathMap,fTimeScale, fStartRange, fEndRange, fIndex, bstrCallBack ,lOptions, pThis,lSystemTime);
		else if (strPropertyName.CompareNoCase(strLightColor)==0)
			hr = StartLightPropertyPathAnimation((long)0, pPathMap,  fTimeScale, fStartRange, fEndRange, fIndex, bstrCallBack ,lOptions, pThis,lSystemTime);
		else if (strPropertyName.CompareNoCase(strLightAttenuation)==0)
			hr = StartLightPropertyPathAnimation((long)1, pPathMap,  fTimeScale, fStartRange, fEndRange, fIndex, bstrCallBack ,lOptions, pThis,lSystemTime);
	}
	else
	{
		lSystemTime = GetTickCount();
		hr = StartFileAnimation(fTimeScale, fStartRange, fEndRange, fIndex, bstrCallBack ,lOptions, pThis,lSystemTime);
	}

ERROR_ENCOUNTERED:
	bstrCallBack.Empty();
	pPathMap.Release();
	pThis.Release();

	if (FAILED(hr))
	{
		VWREPORT(m_pWorld, VWR_ERROR,  "CMultimediaExemplarObject::HandleStartAnimation: An error occurred\n");
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::HandleStartAnimation: Error dealing with an animation property Map\n");
		return ReportMMError(hr);
	}
	
	return hr;
}


STDMETHODIMP CMultimediaExemplarObject::HandleStopAnimation(BSTR bstrPropertyName, VARIANT varOptions)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	char					buff[200];
	CComBSTR				bstrInterpName;
	CComBSTR				bstrDeleteInterp("DeleteAnimation");
	CComPtr<IThing>			pThis;
	HRESULT					hr = S_OK;
	static CString			strFile("File");
	CString					strPropertyName(bstrPropertyName);
	CComPtr<IVWFrame>		pvwfGeometry, pvwfTransform;
	CComPtr<IVWGeometry>	pVWGeometry;
	static CComBSTR			bstrGeometry("Geometry");
	CComPtr<IDirect3DRMAnimationSet2> pAnimationSet;
	char					szAnimName[256];
	DWORD					dwSize = 256;
	long					lOptions;

	lOptions = CastToLong(varOptions);

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	if (strPropertyName.CompareNoCase(strFile)!=0) 
	{
		_ltoa((long)(void*)pThis,buff,16);
		bstrInterpName = buff;
		bstrInterpName.Append(bstrPropertyName);

		if (FAILED(hr = m_pWorld->FireUIEvent(NULL,bstrDeleteInterp,CComVariant(bstrInterpName))))
			goto ERROR_ENCOUNTERED;
	
		if (lOptions & 32) 
			return hr;
	}
	else
	{
  		if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)))
			goto ERROR_ENCOUNTERED;
		if (!pvwfTransform)
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		if (FAILED(hr = pThis->get_ObjectPropertyExt(bstrGeometry, IID_IVWGeometry, (IObjectProperty**)&pVWGeometry)))
			goto ERROR_ENCOUNTERED;
		if (pVWGeometry==NULL)
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		if (FAILED(hr = pVWGeometry->get_GeometryFrame(&pvwfGeometry)))
			goto ERROR_ENCOUNTERED;
		if (pvwfGeometry==NULL)
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		if (FAILED(hr = pvwfGeometry->get_AnimationSet(&pAnimationSet)))
			goto ERROR_ENCOUNTERED;
		if (pAnimationSet == NULL)
		{
			return S_OK;
		}
		pAnimationSet->GetName(&dwSize, szAnimName);
		_ltoa((long)(void*)pThis,buff,16);
		bstrInterpName = buff;
		bstrInterpName.Append(szAnimName);
		if (FAILED(hr = m_pWorld->FireUIEvent(NULL,bstrDeleteInterp,CComVariant(bstrInterpName))))
			goto ERROR_ENCOUNTERED;		
	}
ERROR_ENCOUNTERED:
	pvwfGeometry.Release();
	pvwfTransform.Release();
	pVWGeometry.Release();
	pAnimationSet.Release();
	pThis.Release();

	if (FAILED(hr))
	{
		VWREPORT(m_pWorld, VWR_ERROR,  "CMultimediaExemplarObject::HandleStopAnimation: An error occurred\n");
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::HandleStopAnimation: Failed getting correct geometryframe to stop a file animation\n");
		return ReportMMError(hr);
	}

	
	return hr;
}





STDMETHODIMP CMultimediaExemplarObject::HandleSetAnimationIndex(BSTR bstrPropertyName, VARIANT varIndex, VARIANT varOptions)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	char					buff[200];
	CComBSTR				bstrInterpName;
	CComPtr<IThing>			pThis;
	CComPtr<IPropertyList>	pArgList;
	HRESULT					hr = S_OK;
	static CString			strFile("File");
	CString					strPropertyName(bstrPropertyName);
	CComPtr<IVWFrame>		pvwfGeometry, pvwfTransform;
	CComPtr<IVWGeometry>	pVWGeometry;
	static CComBSTR			bstrGeometry("Geometry");
	static CComBSTR			bstrSetTimeOfAnimation("SetTimeOfAnimation");
	float					fIndex;
	CComVariant				var;
	char					szAnimName[256];
	DWORD					dwSize = 256;
	CComPtr<IDirect3DRMAnimationSet2> pAnimationSet;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		goto ERROR_ENCOUNTERED;
	if (varIndex.vt != VT_ERROR) 
	{
		fIndex = CastToFloat(varIndex);
		fIndex = ((fIndex) < 0 ? -(fIndex) : (fIndex));
	}
	else
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (strPropertyName.CompareNoCase(strFile)!=0) 
	{
		_ltoa((long)(void*)pThis,buff,16);
		bstrInterpName = buff;
		bstrInterpName.Append(bstrPropertyName);

		//pack up the args into a prop list, put bstrInterpName in slot 0 and index in slot 1
		//fire the UI event
		if (FAILED(hr = CreatePropertyList(m_pWorld, &pArgList)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = pArgList->AddString(bstrInterpName)))
			goto ERROR_ENCOUNTERED;

		var.vt = VT_R4;
		var.fltVal = fIndex;
		if (FAILED(hr = pArgList->Add(var)))
			goto ERROR_ENCOUNTERED;

		if (FAILED(hr = m_pWorld->FireUIEvent(NULL,bstrSetTimeOfAnimation,CComVariant((IUnknown*)pArgList))))
			goto ERROR_ENCOUNTERED;
	}
	else
	{
  		if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)))
			goto ERROR_ENCOUNTERED;
		if (!pvwfTransform)
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		if (FAILED(hr = pThis->get_ObjectPropertyExt(bstrGeometry, IID_IVWGeometry, (IObjectProperty**)&pVWGeometry)))
			goto ERROR_ENCOUNTERED;
		if (pVWGeometry==NULL)
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		if (FAILED(hr = pVWGeometry->get_GeometryFrame(&pvwfGeometry)))
			goto ERROR_ENCOUNTERED;
		if (pvwfGeometry==NULL)
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		if (FAILED(hr = pvwfGeometry->get_AnimationSet(&pAnimationSet)))
			goto ERROR_ENCOUNTERED;
		if (pAnimationSet == NULL)
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		if (FAILED(hr = CreatePropertyList(m_pWorld, &pArgList)))
			goto ERROR_ENCOUNTERED;

		pAnimationSet->GetName(&dwSize, szAnimName);
		
		//if the object isnt animating set the time on the animationset anyway...
		//this will allow the file to pesist its current position for clients
		pAnimationSet->SetTime(fIndex);

		_ltoa((long)(void*)pThis,buff,16);
		bstrInterpName = buff;
		bstrInterpName.Append(szAnimName);

		pArgList->AddString( bstrInterpName );
		var.Clear();
		var.vt = VT_R4;
		var.fltVal = fIndex;
		if (FAILED(hr = pArgList->Add(var)))
			goto ERROR_ENCOUNTERED;

		if (FAILED(hr = m_pWorld->FireUIEvent(NULL,bstrSetTimeOfAnimation,CComVariant((IUnknown*)pArgList))))
			goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	pvwfGeometry.Release();
	pvwfTransform.Release();
	pVWGeometry.Release();
	pThis.Release();
	pAnimationSet.Release();

	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::HandleSetAnimationIndex: an error occurred\n");
		return ReportMMError(hr);
	}

	return hr;
}




// Grabs the first light it can find either in the frame passed in or in one
// of its children frames
HRESULT GetLightFromFrame(IDirect3DRMFrame* pFrame,IDirect3DRMLight** ppLight) 
{
	HRESULT hr = S_OK;
	CComPtr<IDirect3DRMLightArray>	pLightArray;
	CComPtr<IDirect3DRMFrameArray>	pFrameArray;	
	CComPtr<IDirect3DRMFrame>		pChildFrame;
	int nCount;

	if (FAILED(hr = pFrame->GetLights(&pLightArray)))
		return hr;

	if (pLightArray->GetSize()) {
		if (FAILED(hr = pLightArray->GetElement(0, ppLight)))
			return hr;
	}
	else {
		if (SUCCEEDED(hr = pFrame->GetChildren(&pFrameArray)))
		{																				
			nCount = pFrameArray->GetSize();											
			for (int i = 0; i < nCount; i++)
			{
				if (SUCCEEDED(pFrameArray->GetElement(i, &pChildFrame)))
				{
					if (SUCCEEDED(hr = GetLightFromFrame(pChildFrame,ppLight)))
						return hr;
				}
			}
		}
		hr = E_FAIL;
	}
	return hr;
}




STDMETHODIMP CMultimediaExemplarObject::StartFileAnimation(float fTimeScale, float fStartRange, float fEndRange, float fIndex, CComBSTR bstrCallBack, long lOptions, IThing* pThis,long lSystemTime)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HRESULT					hr = S_OK;
	char					szAnimName[256];
	DWORD					dwSize = 256;
	long					nOptions;
	static CComBSTR			bstrAddAnimation("AddAnimation");
	static CComBSTR			bstrDeleteInterp("DeleteAnimation");
	CComBSTR				bstrInterpName;
	CComPtr <IPropertyList> pPropList;
	CComPtr <IVWFrame>		pParentFrame;
	CComPtr<IVWFrame>		pvwfGeometry, pvwfTransform;
	CComPtr<IVWGeometry>	pVWGeometry;
	CComPtr<IDirect3DRMAnimationSet2> pAnimationSet;
	CComPtr <IDirect3DRMAnimationArray> pD3DAnimArray;
	CComPtr <IDirect3DRMAnimation2> pD3DAnim;
	static CString			strFile("File");
	static CComBSTR			bstrGeometry("Geometry");
	CComVariant				var;
	float					fEndTime = 0.0f;
	DWORD					dwNumKeys = 0;
	unsigned				int j;
	D3DRMANIMATIONKEY*		pKeys = NULL;
	int						nAnimCount, i;
	char								buff[50];

	//Create the unique name for the animation
	_ltoa((long)(void*)pThis,buff,16);
	bstrInterpName = buff;

	//validate duration and range values by searching for highest key value throughout the set
	if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)))
		goto ERROR_ENCOUNTERED;
	if (!pvwfTransform)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pThis->get_ObjectPropertyExt(bstrGeometry, IID_IVWGeometry, (IObjectProperty**)&pVWGeometry)))
		goto ERROR_ENCOUNTERED;
	if (pVWGeometry==NULL)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pVWGeometry->get_GeometryFrame(&pvwfGeometry)))
		goto ERROR_ENCOUNTERED;
	if (pvwfGeometry==NULL)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pvwfGeometry->get_AnimationSet(&pAnimationSet)))
		goto ERROR_ENCOUNTERED;
	if (pAnimationSet == NULL)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	pAnimationSet->GetAnimations(&pD3DAnimArray);
	nAnimCount = pD3DAnimArray->GetSize();

	//iterate through each animation in the set
	for (i = 0; i < nAnimCount; i++)
	{
		//get an animation from the set
		if (FAILED(hr = pD3DAnimArray->GetElement(i, &pD3DAnim)))
			goto ERROR_ENCOUNTERED;
		//get the number of keys we have in this animation in the set
		pD3DAnim->GetKeys(0, 9999999, &dwNumKeys, NULL);
		//declare a key array of the right size to iterate through
		pKeys = new D3DRMANIMATIONKEY[dwNumKeys];
		//put the keys into the key array
		pD3DAnim->GetKeys(0, 9999999, &dwNumKeys, pKeys);
		//iterate the array, looking at the value of each key to find highest
		for (j = 0; j < dwNumKeys; j++)
		{
			if (fEndTime < (float)(pKeys[j].dvTime))
				fEndTime = (float)(pKeys[j].dvTime);
		}
		delete [] pKeys;
		pD3DAnim.Release();
	}
	//check that we didnt have an overly huge time value in one of the keys
	if (fEndTime > 0xFFFFFFFF) 
	{
		hr = E_FAIL; 
		goto ERROR_ENCOUNTERED;
	}

	//if no start/end ranges were provided (i.e. they were passed in as -1), set them to good defaults
	if (fEndRange < 0.0f)
		fEndRange = fEndTime;
	if (fStartRange > 0.0f)
	{
		if (fStartRange > fEndRange)
			fStartRange = fEndRange;
	}
	else
		fStartRange = 0.0f;
	//we cant delimit the range of file animations since we cant check their duration on the server...

	::CreatePropertyList(m_pWorld, &pPropList);
	pAnimationSet->GetName(&dwSize, szAnimName);

	nOptions = D3DRMANIMATION_LINEARPOSITION | D3DRMANIMATION_SCALEANDROTATION;
	if (lOptions & 1) nOptions |= D3DRMANIMATION_OPEN;
	if (lOptions & 2) nOptions |= D3DRMANIMATION_CLOSED;

	pPropList->AddLong((long)(void*)(pAnimationSet));
	pPropList->AddLong(lSystemTime);
	pPropList->AddLong(nOptions);

	var = (fEndRange - fStartRange);
	pPropList->Add(var);

	bstrInterpName.Append(szAnimName);

	pPropList->AddString( bstrInterpName );
	pPropList->AddThing( pThis );
	
	var = fTimeScale;
	pPropList->Add(var);

	var = fIndex;
	pPropList->Add(var);

	if (IsMethodThisSide(pThis, bstrCallBack))
		pPropList->AddString(bstrCallBack); 
	else
		pPropList->AddString(CComBSTR(""));

	var = fStartRange;
	pPropList->Add(var);

	var = fEndRange;
	pPropList->Add(var);

	hr = m_pWorld->FireUIEvent(NULL, bstrAddAnimation, CComVariant((IUnknown*)pPropList));


ERROR_ENCOUNTERED:
	pPropList.Release();
	pParentFrame.Release();
	pvwfGeometry.Release();
	pvwfTransform.Release();
	pVWGeometry.Release();
	pAnimationSet.Release();
	pD3DAnimArray.Release();
	pD3DAnim.Release();

	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::StartFileAnimation Failed\n");
		return ReportMMError(hr);
	}
	return hr;
}



STDMETHODIMP CMultimediaExemplarObject::StartPositionPathAnimation(IPropertyMap* pPathMap, float fTimeScale, float fStartRange, float fEndRange, float fIndex, CComBSTR bstrCallBack, long lOptions, IThing* pThis,long lSystemTime)
{		
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CComPtr<IDirect3DRMInterpolator>	pInterpolator;
	CComPtr<IDirect3DRMFrame>			pFrameInterpolator, pFrame, pFrameParent;
	CComPtr<IPropertyList>				pPropList;
	CComPtr<IVector>					pVector;
	CComPtr<IDirect3DRM>				pd3drm;
	CComPtr<IVWFrame>					pvwfTransform, pvwfRelativeFrame, pvwfParentFrame;					
	CComBSTR							bstrInterpName;
	CComVariant							var, var2;
	VARIANT_BOOL						bIsEmpty,bDone;
    HRESULT								hr = S_OK;
	BSTR								bstrName = NULL;
	char								buff[50];
	float								fEndTime = 0.0f;
	float								fX,fY,fZ;
	float								fTime;
	DWORD								dwOptions;
	static CComBSTR						bstrAddInterp("AddAnimation");

	//Create the unique name for the animation
	_ltoa((long)(void*)pThis,buff,16);
	bstrInterpName = buff;
	bstrInterpName.Append("Position");
	
	//validate we have a good frame + a good path
	if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)))
		goto ERROR_ENCOUNTERED;
	if (!pvwfTransform)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pvwfTransform->get_Frame3D(&pFrame)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pWorld->get_Tool(CComBSTR("Direct3DRM"), (IUnknown**)&pd3drm)))
		goto ERROR_ENCOUNTERED;
	if (!pPathMap) 
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pPathMap->get_IsEmpty(&bIsEmpty)))
		goto ERROR_ENCOUNTERED;
	if (bIsEmpty == TRUE)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	//create the position interpolator
	if (FAILED(hr = pd3drm->CreateObject(CLSID_CDirect3DRMFrameInterpolator, 0, IID_IDirect3DRMInterpolator, (void**)&pInterpolator)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pInterpolator->QueryInterface(IID_IDirect3DRMFrame, (void**)&pFrameInterpolator)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pFrame->GetParent(&pFrameParent)))
		goto ERROR_ENCOUNTERED;

	hr = pPathMap->FirstItem(&bstrName, &var, &bDone);
	if (FAILED(hr) && hr != VWOBJECT_E_PERMISSION_DENIED)
		goto ERROR_ENCOUNTERED;
	
	if (lOptions & 64)
	{
		::CreateFrame(m_pWorld, &pvwfRelativeFrame);
		if (!pvwfRelativeFrame)
		{
			VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::StartPositionPathAnimation relative path frame operation Failed\n");
			goto ERROR_ENCOUNTERED;
		}
		if (FAILED(hr = pvwfTransform->AddChild(pvwfRelativeFrame)))
		{
			VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::StartPositionPathAnimation relative path frame operation Failed\n");
			goto ERROR_ENCOUNTERED;
		}
		if (FAILED(hr = pvwfTransform->GetParent(&pvwfParentFrame)))
		{
			VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::StartPositionPathAnimation relative path frame operation Failed\n");
			goto ERROR_ENCOUNTERED;
		}
	}
	while (bDone != VARIANT_TRUE)
	{
		//ignore anything thats not a vector or a dispatch type in the map
		if ((var.vt != VT_DISPATCH)) 
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		if (FAILED(hr = var.pdispVal->QueryInterface(IID_IVector,(void**)&pVector)))
			goto ERROR_ENCOUNTERED;
		if (hr != VWOBJECT_E_PERMISSION_DENIED)
		{	
			pVector->get(&fX, &fY, &fZ);
			fTime = (float)(atoi(CString(bstrName)));
			//keep track of the ending time and value
			if (fEndTime<fTime)  
				fEndTime = fTime;
			pInterpolator->SetIndex(fTime);
			if (lOptions & 64)
			{
				//get a relative position to its current pos/orientation with the relative frame
				pvwfRelativeFrame->SetPositionEx(pvwfTransform, fX, fY, fZ);
				pvwfRelativeFrame->GetPositionEx(pvwfParentFrame, &fX, &fY, &fZ);
				pFrameInterpolator->SetPosition(pFrameParent,fX,fY,fZ);
			}
			else
				pFrameInterpolator->SetPosition(pFrameParent,fX,fY,fZ);
		}
		SAFEFREESTRING(bstrName);
		pVector.Release();

		// go on to the next one
		hr = pPathMap->NextItem(&bstrName, &var, &bDone);
		if (FAILED(hr) && hr != VWOBJECT_E_PERMISSION_DENIED)
			goto ERROR_ENCOUNTERED;
	}
	if (lOptions & 64)
	{
			pvwfTransform->DeleteChild(pvwfRelativeFrame);
			pvwfRelativeFrame.Release();
			pvwfParentFrame.Release();
	}
	//check that we didnt have an overly huge time value in the map of vectors
	if (fEndTime > 0xFFFFFFFF) 
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	//if no start/end ranges were provided (i.e. they were passed in as -1), set them to good defaults
	if (fEndRange < 0.0f)
		fEndRange = fEndTime;
	if (fStartRange > 0.0f)
	{
		if (fStartRange > fEndRange)
			fStartRange = fEndRange;
	}
	else
		fStartRange = 0.0f;
	if (fEndRange > fEndTime)
		fEndRange = fEndTime;
	//validate that the index falls in the range
	if ((fStartRange + fIndex) > fEndRange)
		fIndex = fEndRange - fStartRange;
	//calculate the options for interpolations, and attach the proper object

	if (lOptions & 1) dwOptions = D3DRMINTERPOLATION_OPEN;
	if (lOptions & 2) dwOptions = D3DRMINTERPOLATION_CLOSED;
	if (lOptions & 4) dwOptions |= D3DRMINTERPOLATION_LINEAR;
	if (lOptions & 8) dwOptions |= D3DRMINTERPOLATION_SPLINE;
	if (lOptions & 16) dwOptions |= D3DRMINTERPOLATION_NEAREST;

	//attach the interpolation to the frame
	if (FAILED(hr = pInterpolator->AttachObject((IDirect3DRMObject*)pFrame)))
		goto ERROR_ENCOUNTERED;

	//create property list to pass to renderer
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pPropList)))
		goto ERROR_ENCOUNTERED;

	pPropList->AddLong((long)(void*)pInterpolator);
	pPropList->AddLong(lSystemTime);
	pPropList->AddLong(dwOptions);

	var2 = (fEndRange - fStartRange);
	pPropList->Add(var2);

	pPropList->AddString(bstrInterpName);
	pPropList->AddThing( pThis );
	
	var2 = fTimeScale;
	pPropList->Add(var2);

	var2 = fIndex;
	pPropList->Add(var2);

	if (IsMethodThisSide(pThis, bstrCallBack))
		pPropList->AddString(bstrCallBack); 
	else
		pPropList->AddString(CComBSTR(""));

	var2 = fStartRange;
	pPropList->Add(var2);

	var2 = fEndRange;
	pPropList->Add(var2);

	//pass list to the renderer
	if(FAILED(hr = m_pWorld->FireUIEvent(NULL,bstrAddInterp,CComVariant((IUnknown*)pPropList))))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	pInterpolator.Release();
	pFrameInterpolator.Release();
	pFrame.Release();
	pFrameParent.Release();
	pPropList.Release();
	pVector.Release();
	pd3drm.Release();
	pvwfTransform.Release();

	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::StartPositionPathAnimation Failed\n");
		return ReportMMError(hr);
	}

	return hr;
}


//StartDirectionPathAnimation 
STDMETHODIMP CMultimediaExemplarObject::StartDirectionPathAnimation(IPropertyMap* pPathMap, float fTimeScale, float fStartRange, float fEndRange, float fIndex, CComBSTR bstrCallBack, long lOptions, IThing* pThis,long lSystemTime)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CComPtr<IVWFrame>					pvwfTransform;
	CComPtr<IDirect3DRMInterpolator>	pInterpolator;
	CComPtr<IDirect3DRMFrame>			pFrame;
	CComPtr<IDirect3DRMFrame3>			pFrame3;
	CComPtr<IDirect3DRMAnimation2>		pAnimation2;
	CComPtr<IPropertyList>				pPropList;
	CComPtr<IVector>					pVector;
	CComPtr<IDirect3DRM>				pd3drm;
	CComPtr<IDirect3DRM3>				pd3drm3;
	CComPtr<IPropertyMap>				pPropPath,pIntScale;
	CComBSTR							bstrInterpName,bstrPathName,bstrStartName;
	CComVariant							var, var2;
	D3DRMQUATERNION						quatDir,quatFinal,quatUp;
	D3DVECTOR							vSrc, vDest, vRotate;
	VARIANT_BOOL						bIsEmpty,bDone;
    HRESULT								hr = S_OK;
	BSTR								bstrName = NULL;
	char								buff[50];
	float								fEndTime = 0,fUpX,fUpY,fUpZ,fDirX,fDirY,fDirZ,fAngle,
										fX,fY,fZ,fScaleX,fScaleY,fScaleZ,fEndX,fEndY,fEndZ;
	float								fTime;
	DWORD								dwOptions;
	static CComBSTR						bstrAddInterp("AddAnimation");

	//Create the unique name for the animation
	_ltoa((long)(void*)pThis,buff,16);
	bstrInterpName = buff;
	bstrInterpName.Append("Direction");
	
	if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)))
		goto ERROR_ENCOUNTERED;
	if (!pvwfTransform)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pvwfTransform->get_Frame3D(&pFrame)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pWorld->get_Tool(CComBSTR("Direct3DRM"), (IUnknown**)&pd3drm)))
		goto ERROR_ENCOUNTERED;
	if (!pPathMap) 
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pPathMap->get_IsEmpty(&bIsEmpty)))
		goto ERROR_ENCOUNTERED;
	if (bIsEmpty == TRUE) 
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pd3drm->QueryInterface(IID_IDirect3DRM3,(void**)&pd3drm3)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pd3drm3->CreateAnimation(&pAnimation2)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pvwfTransform->GetOrientationEx(NULL, &fDirX, &fDirY, &fDirZ, &fUpX, &fUpY, &fUpZ)))
		goto ERROR_ENCOUNTERED;
	
	//for rotation animations we need the current scale, otherwise the
	//scale would get blown away
	if (FAILED(hr = pvwfTransform->GetScaleEx(NULL, &fScaleX, &fScaleY, &fScaleZ)))
		goto ERROR_ENCOUNTERED;

	//SK to do : if its a relative path, set the zero index to be the same as the current frame pos.
	//Loop through all the animation points and add them to the animation

	hr = pPathMap->FirstItem(&bstrName, &var, &bDone);
	if (FAILED(hr) && hr != VWOBJECT_E_PERMISSION_DENIED)
		goto ERROR_ENCOUNTERED;

	while (bDone != VARIANT_TRUE)
	{
		//ignore anything thats not a vector or a dispatch type in the map
		if ((var.vt != VT_DISPATCH)) 
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		if (FAILED(hr = var.pdispVal->QueryInterface(IID_IVector,(void**)&pVector)))
			goto ERROR_ENCOUNTERED;
		if (hr != VWOBJECT_E_PERMISSION_DENIED)
		{
			pVector->get(&fX, &fY, &fZ);
			fTime = (float)(atoi(CString(bstrName)));
			//keep track of the ending time and value
			if (fEndTime < fTime) 
			{ 
				fEndTime = fTime;
				fEndX = fX;
				fEndY = fY;
				fEndZ = fZ;
			}
			pAnimation2->AddScaleKey(fTime,fScaleX,fScaleY,fScaleZ);
			//calculate quaternions for the direction animation 	
			fDirX = fX;
			fDirY = fY;
			fDirZ = fZ;	
			vSrc.x = 0;
			vSrc.y = 1;
			vSrc.z = 0;
			vDest.x = fUpX;
			vDest.y = fUpY;
			vDest.z = fUpZ;
			fAngle = D3DRMVectorDotProduct(&vSrc,&vDest) / D3DRMVectorModulus(&vDest);
			D3DRMVectorCrossProduct(&vRotate, &vSrc, &vDest);
			D3DRMQuaternionFromRotation(&quatUp, &vRotate, (float)(-acos(fAngle)));
			//this is a temporary storage of the z-axis, not the real destination
			vDest.x = 0;
			vDest.y = 0;
			vDest.z = 1;
			D3DRMVectorRotate(&vSrc,&vDest,&vRotate,(float)(acos(fAngle)));
			//this is the real destination
			vDest.x = fDirX;
			vDest.y = fDirY;
			vDest.z = fDirZ;
			D3DRMVectorCrossProduct(&vRotate, &vSrc, &vDest);
			fAngle = D3DRMVectorDotProduct(&vSrc,&vDest) / (D3DRMVectorModulus(&vSrc)*D3DRMVectorModulus(&vDest));
			D3DRMQuaternionFromRotation(&quatDir, &vRotate, (float)(-acos(fAngle)));
			D3DRMQuaternionMultiply(&quatFinal,&quatUp,&quatDir);
			pAnimation2->AddRotateKey(fTime,&quatFinal);
		}
		SAFEFREESTRING(bstrName);
		pVector.Release();
		// go on to the next one
		hr = pPathMap->NextItem(&bstrName, &var, &bDone);
		if (FAILED(hr) && hr != VWOBJECT_E_PERMISSION_DENIED)
			goto ERROR_ENCOUNTERED;
	}
	if (fEndTime > 0xFFFFFFFF)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	//if no start/end ranges were provided (i.e. they were passed in as -1), set them to good defaults
	if (fEndRange < 0.0f)
		fEndRange = fEndTime;
	if (fStartRange > 0.0f)
	{
		if (fStartRange > fEndRange)
			fStartRange = fEndRange;
	}
	else
		fStartRange = 0.0f;
	if (fEndRange > fEndTime)
		fEndRange = fEndTime;
	//validate that the index falls in the range
	if ((fStartRange + fIndex) > fEndRange)
		fIndex = fEndRange - fStartRange;
	//calculate options for the animation and attach frame
	if (FAILED(hr = pFrame->QueryInterface(IID_IDirect3DRMFrame3,(void**)&pFrame3)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAnimation2->SetFrame(pFrame3)))
		goto ERROR_ENCOUNTERED;
	if (lOptions & 1) dwOptions = D3DRMANIMATION_OPEN;
	if (lOptions & 2) dwOptions = D3DRMANIMATION_CLOSED;
	dwOptions |= D3DRMANIMATION_SCALEANDROTATION;

	//create property list to pass to renderer
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pPropList)))
		goto ERROR_ENCOUNTERED;
	pPropList->AddLong((long)(void*)pAnimation2);
	pPropList->AddLong(lSystemTime);
	pPropList->AddLong(dwOptions);

	var2 = (fEndRange - fStartRange);
	pPropList->Add(var2);

	pPropList->AddString(bstrInterpName);
	pPropList->AddThing( pThis );
	
	var2 = fTimeScale;
	pPropList->Add(var2);

	var2 = fIndex;
	pPropList->Add(var2);

	if (IsMethodThisSide(pThis, bstrCallBack))
		pPropList->AddString(bstrCallBack); 
	else
		pPropList->AddString(CComBSTR(""));

	var2 = fStartRange;
	pPropList->Add(var2);

	var2 = fEndRange;
	pPropList->Add(var2);

	//pass list to the renderer
	if(FAILED(hr = m_pWorld->FireUIEvent(NULL,bstrAddInterp,CComVariant((IUnknown*)pPropList))))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	pvwfTransform.Release();
	pInterpolator.Release();
	pFrame.Release();
	pFrame3.Release();
	pAnimation2.Release();
	pPropList.Release();
	pVector.Release();
	pd3drm.Release();
	pd3drm3.Release();
	pPropPath.Release();
	pIntScale.Release();

	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::StartDirectionPathAnimation Failed\n");
		return ReportMMError(hr);
	}


	return hr;
}

 
STDMETHODIMP CMultimediaExemplarObject::StartUpPathAnimation(IPropertyMap* pPathMap, float fTimeScale, float fStartRange, float fEndRange, float fIndex, CComBSTR bstrCallBack, long lOptions, IThing* pThis,long lSystemTime)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CComPtr<IVWFrame>					pvwfTransform;
	CComPtr<IDirect3DRMInterpolator>	pInterpolator;
	CComPtr<IDirect3DRMFrame>			pFrameInterpolator, pFrame;
	CComPtr<IDirect3DRMFrame3>			pFrame3;
	CComPtr<IDirect3DRMLight>			pLightInterpolator, pLight;
	CComPtr<IDirect3DRMAnimation2>		pAnimation2;
	CComPtr<IPropertyList>				pPropList;
	CComPtr<IVector>					pVector;
	CComPtr<IDirect3DRM>				pd3drm;
	CComPtr<IDirect3DRM3>				pd3drm3;
	CComPtr<IPropertyMap>				pPropPath,pIntScale;
	CComBSTR							bstrInterpName,bstrPathName,bstrStartName;
	CComVariant							var, var2;
	D3DRMQUATERNION						quatDir,quatFinal,quatUp;
	D3DVECTOR							vSrc, vDest, vRotate;
	VARIANT_BOOL						bIsEmpty,bDone;
    HRESULT								hr = S_OK;
	BSTR								bstrName = NULL;
	char								buff[50];
	float								fEndTime = 0,fUpX,fUpY,fUpZ,fDirX,fDirY,fDirZ,fAngle,
										fX,fY,fZ,fScaleX,fScaleY,fScaleZ,fDot,fEndX,fEndY,fEndZ;
	float								fTime;
	DWORD								dwOptions;
	static CComBSTR						bstrAddInterp("AddAnimation");

	static CComBSTR						bstrSystemTime("SystemTime");

	//Create the unique name for the animation
	_ltoa((long)(void*)pThis,buff,16);
	bstrInterpName = buff;
	bstrInterpName.Append("Up");
	
	if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)))
		goto ERROR_ENCOUNTERED;
	if (!pvwfTransform)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pvwfTransform->get_Frame3D(&pFrame)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pWorld->get_Tool(CComBSTR("Direct3DRM"), (IUnknown**)&pd3drm)))
		goto ERROR_ENCOUNTERED;
	if (!pPathMap) 
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pPathMap->get_IsEmpty(&bIsEmpty)))
		goto ERROR_ENCOUNTERED;
	if (bIsEmpty == TRUE) 
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pd3drm->QueryInterface(IID_IDirect3DRM3,(void**)&pd3drm3)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pd3drm3->CreateAnimation(&pAnimation2)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pvwfTransform->GetOrientationEx(NULL, &fDirX, &fDirY, &fDirZ, &fUpX, &fUpY, &fUpZ)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pvwfTransform->GetScaleEx(NULL, &fScaleX, &fScaleY, &fScaleZ)))
		goto ERROR_ENCOUNTERED;	

	//SK to do : if its a relative path, set the zero index to be the same as the current frame pos.
	//Loop through all the animation points and add them to the animation

	hr = pPathMap->FirstItem(&bstrName, &var, &bDone);
	if (FAILED(hr) && hr != VWOBJECT_E_PERMISSION_DENIED)
		goto ERROR_ENCOUNTERED;

	while (bDone != VARIANT_TRUE)
	{
		//ignore anything thats not a vector or a dispatch type in the map
		if ((var.vt != VT_DISPATCH)) 
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		if (FAILED(hr = var.pdispVal->QueryInterface(IID_IVector,(void**)&pVector)))
			goto ERROR_ENCOUNTERED;
		if (hr != VWOBJECT_E_PERMISSION_DENIED)
		{	
			pVector->get(&fX, &fY, &fZ);
			fTime = (float)(atoi(CString(bstrName)));
			if (fEndTime<fTime)  
			{ 
				fEndTime = fTime;
				fEndX = fX;
				fEndY = fY;
				fEndZ = fZ;
			}
			pAnimation2->AddScaleKey(fTime,fScaleX,fScaleY,fScaleZ);
			//calculate quaternions for the  up animation 		
			vSrc.x = fX;
			vSrc.y = fY;
			vSrc.z = fZ;
			vDest.x = fDirX;
			vDest.y = fDirY;
			vDest.z = fDirZ;		
			fDot = D3DRMVectorDotProduct(&vSrc,&vDest);
			vRotate.x = vDest.x * fDot;
			vRotate.y = vDest.y * fDot;
			vRotate.z = vDest.z * fDot;
			fUpX = vSrc.x - vRotate.x;
			fUpY = vSrc.y - vRotate.y;
			fUpZ = vSrc.z - vRotate.z;							
			//fUp now equals the up vector projected onto the plane that the direction vector
			//is the normal of.  If the up and direction vectors were parallel vRotate will be (0,0,0)
			if (!fUpX  && !fUpY && !fUpZ) 
			{
				hr = E_FAIL;
				goto ERROR_ENCOUNTERED;
			}
			vSrc.x = 0;
			vSrc.y = 1;
			vSrc.z = 0;
			vDest.x = fUpX;
			vDest.y = fUpY;
			vDest.z = fUpZ;
			fAngle = D3DRMVectorDotProduct(&vSrc,&vDest) / D3DRMVectorModulus(&vDest);
			D3DRMVectorCrossProduct(&vRotate, &vSrc, &vDest);
			D3DRMQuaternionFromRotation(&quatUp, &vRotate, (float)(-acos(fAngle)));
			//this is a temporary storage of the z-axis, not the real destination
			vDest.x = 0;
			vDest.y = 0;
			vDest.z = 1;
			D3DRMVectorRotate(&vSrc,&vDest,&vRotate,(float)(acos(fAngle)));
			//this is the real destination
			vDest.x = fDirX;
			vDest.y = fDirY;
			vDest.z = fDirZ;
			D3DRMVectorCrossProduct(&vRotate, &vSrc, &vDest);
			fAngle = D3DRMVectorDotProduct(&vSrc,&vDest) / (D3DRMVectorModulus(&vSrc)*D3DRMVectorModulus(&vDest));
			D3DRMQuaternionFromRotation(&quatDir, &vRotate, (float)(-acos(fAngle)));
			D3DRMQuaternionMultiply(&quatFinal,&quatUp,&quatDir);
			pAnimation2->AddRotateKey(fTime,&quatFinal);
		}
		SAFEFREESTRING(bstrName);
		pVector.Release();
		// go on to the next one
		hr = pPathMap->NextItem(&bstrName, &var, &bDone);
		if (FAILED(hr) && hr != VWOBJECT_E_PERMISSION_DENIED)
			goto ERROR_ENCOUNTERED;
	}	

	//if no start/end ranges were provided (i.e. they were passed in as -1), set them to good defaults
	if (fEndRange < 0.0f)
		fEndRange = fEndTime;
	if (fStartRange > 0.0f)
	{
		if (fStartRange > fEndRange)
			fStartRange = fEndRange;
	}
	else
		fStartRange = 0.0f;
	if (fEndRange > fEndTime)
		fEndRange = fEndTime;
	//validate that the index falls in the range
	if ((fStartRange + fIndex) > fEndRange)
		fIndex = fEndRange - fStartRange;
	//calculate options for animations and attach frame
	if (FAILED(hr = pFrame->QueryInterface(IID_IDirect3DRMFrame3,(void**)&pFrame3)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAnimation2->SetFrame(pFrame3)))
		goto ERROR_ENCOUNTERED;
	if (lOptions & 1) dwOptions = D3DRMANIMATION_OPEN;
	if (lOptions & 2) dwOptions = D3DRMANIMATION_CLOSED;
	dwOptions |= D3DRMANIMATION_SCALEANDROTATION;
	
	//create property list to pass to renderer
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pPropList)))
		goto ERROR_ENCOUNTERED;
	pPropList->AddLong((long)(void*)pAnimation2);
	pPropList->AddLong(lSystemTime);
	pPropList->AddLong(dwOptions);

	var2 = (fEndRange - fStartRange);
	pPropList->Add(var2);

	pPropList->AddString(bstrInterpName);
	pPropList->AddThing( pThis );
	
	var2 = fTimeScale;
	pPropList->Add(var2);

	var2 = fIndex;
	pPropList->Add(var2);

	if (IsMethodThisSide(pThis, bstrCallBack))
		pPropList->AddString(bstrCallBack); 
	else
		pPropList->AddString(CComBSTR(""));

	var2 = fStartRange;
	pPropList->Add(var2);

	var2 = fEndRange;
	pPropList->Add(var2);

	//pass list to the renderer
	if(FAILED(hr = m_pWorld->FireUIEvent(NULL,bstrAddInterp,CComVariant((IUnknown*)pPropList))))
		goto ERROR_ENCOUNTERED;

	
ERROR_ENCOUNTERED:
	pvwfTransform.Release();
	pInterpolator.Release();
	pFrameInterpolator.Release();
	pFrame.Release();
	pFrame3.Release();
	pLightInterpolator.Release();
	pLight.Release();
	pAnimation2.Release();
	pPropList.Release();
	pVector.Release();
	pd3drm.Release();
	pd3drm3.Release();
	pPropPath.Release();
	pIntScale.Release();

	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::StartUpPathAnimation Failed\n");
		return ReportMMError(hr);
	}

	return hr;
}

STDMETHODIMP CMultimediaExemplarObject::StartScalePathAnimation(IPropertyMap* pPathMap, float fTimeScale, float fStartRange, float fEndRange, float fIndex, CComBSTR bstrCallBack, long lOptions, IThing* pThis,long lSystemTime)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CComPtr<IVWFrame>					pvwfTransform;
	CComPtr<IDirect3DRMFrame>			pFrameInterpolator, pFrame;
	CComPtr<IDirect3DRMFrame3>			pFrame3;
	CComPtr<IDirect3DRMAnimation2>		pAnimation2;
	CComPtr<IPropertyList>				pPropList;
	CComPtr<IVector>					pVector;
	CComPtr<IDirect3DRM>				pd3drm;
	CComPtr<IDirect3DRM3>				pd3drm3;
	CComPtr<IPropertyMap>				pPropPath,pIntScale;
	CComBSTR							bstrInterpName,bstrPathName,bstrStartName;
	CComVariant							var,varClone, var2;
	D3DRMQUATERNION						quatDir,quatFinal,quatUp;
	D3DVECTOR							vSrc, vDest, vRotate;
	VARIANT_BOOL						bIsEmpty,bDone;
    HRESULT								hr = S_OK;
	BSTR								bstrName = NULL;
	char								buff[50];
	float								fEndTime = 0.0f,fUpX,fUpY,fUpZ,fDirX,fDirY,fDirZ,fAngle,
										fX,fY,fZ,fEndX,fEndY,fEndZ, fTime;
	DWORD								dwOptions;
	static CComBSTR						bstrInternalScalePath("InternalScalePath"); 
	static CComBSTR						bstrAddInterp("AddAnimation");
	static CComBSTR						bstrSystemTime("SystemTime");
	CString								strMethodName(bstrName);

	//Create the unique name for the animation
	_ltoa((long)(void*)pThis,buff,16);
	bstrInterpName = buff;
	bstrInterpName.Append("Scale");
	
	if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)))
		goto ERROR_ENCOUNTERED;
	if (!pvwfTransform)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pvwfTransform->get_Frame3D(&pFrame)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pWorld->get_Tool(CComBSTR("Direct3DRM"), (IUnknown**)&pd3drm)))
		goto ERROR_ENCOUNTERED;
	if (!pPathMap) 
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pPathMap->get_IsEmpty(&bIsEmpty)))
		goto ERROR_ENCOUNTERED;
	if (bIsEmpty == TRUE) 
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pd3drm->QueryInterface(IID_IDirect3DRM3,(void**)&pd3drm3)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pd3drm3->CreateAnimation(&pAnimation2)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pvwfTransform->GetOrientationEx(NULL, &fDirX, &fDirY, &fDirZ, &fUpX, &fUpY, &fUpZ)))
		goto ERROR_ENCOUNTERED;
	//for scale animations we need to create a quaternion with the current
	//rotation of the object to put in the animation. Otherwise the rotation
	//gets blown away
	if (FAILED(hr = pThis->get_ObjectPropertyExt(bstrInternalScalePath, IID_IPropertyMap, (IObjectProperty**)&pIntScale)))
		goto ERROR_ENCOUNTERED;
	if (!pIntScale) 
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pIntScale->RemoveAll()))
		goto ERROR_ENCOUNTERED;
	vSrc.x = 0;
	vSrc.y = 1;
	vSrc.z = 0;
	vDest.x = fUpX;
	vDest.y = fUpY;
	vDest.z = fUpZ;
	fAngle = D3DRMVectorDotProduct(&vSrc,&vDest) / D3DRMVectorModulus(&vDest);
	D3DRMVectorCrossProduct(&vRotate, &vSrc, &vDest);
	D3DRMQuaternionFromRotation(&quatUp, &vRotate, (float)(-acos(fAngle)));
	//this is a temporary storage of the z-axis, not the real destination
	vDest.x = 0;
	vDest.y = 0;
	vDest.z = 1;
	D3DRMVectorRotate(&vSrc,&vDest,&vRotate,(float)(acos(fAngle)));
	//this is the real destination
	vDest.x = fDirX;
	vDest.y = fDirY;
	vDest.z = fDirZ;
	D3DRMVectorCrossProduct(&vRotate, &vSrc, &vDest);
	fAngle = D3DRMVectorDotProduct(&vSrc,&vDest) / (D3DRMVectorModulus(&vSrc)*D3DRMVectorModulus(&vDest));
	D3DRMQuaternionFromRotation(&quatDir, &vRotate, (float)(-acos(fAngle)));
	D3DRMQuaternionMultiply(&quatFinal,&quatUp,&quatDir);

	//Loop through all the animation points and add them to the animation

	hr = pPathMap->FirstItem(&bstrName, &var, &bDone);
	if (FAILED(hr) && hr != VWOBJECT_E_PERMISSION_DENIED)
		goto ERROR_ENCOUNTERED;

	while (bDone != VARIANT_TRUE)
	{
		//ignore anything thats not a vector or a dispatch type in the map
		if (var.vt != VT_DISPATCH)
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		if (FAILED(hr = var.pdispVal->QueryInterface(IID_IVector,(void**)&pVector)))
			goto ERROR_ENCOUNTERED;
		if (hr != VWOBJECT_E_PERMISSION_DENIED)
		{		
			pVector->get(&fX, &fY, &fZ);
			fTime = (float)(atoi(CString(bstrName)));
			if (fEndTime<fTime)  
			{ 
				fEndTime = fTime;
				fEndX = fX;
				fEndY = fY;
				fEndZ = fZ;
			}
			if (FAILED(hr = pAnimation2->AddScaleKey(fTime,fX,fY,fZ)))
				goto ERROR_ENCOUNTERED;
			if (FAILED(hr = m_pWorld->CloneProperty(var,&varClone)))
				goto ERROR_ENCOUNTERED;
			if (FAILED(hr = pIntScale->put_Property(bstrName, varClone)))
				goto ERROR_ENCOUNTERED;
			//This will keep the direction from getting screwed up by the scale, 
			//but also prevents direction+scale from animating concurrently
			pAnimation2->AddRotateKey(fTime,&quatFinal);
		}
		// free stuff
		SAFEFREESTRING(bstrName);
		pVector.Release();
		// go on to the next one
		hr = pPathMap->NextItem(&bstrName, &var, &bDone);
		if (FAILED(hr) && hr != VWOBJECT_E_PERMISSION_DENIED)
			goto ERROR_ENCOUNTERED;
	}
	if (fEndTime > 0xFFFFFFFF) 
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	//if no start/end ranges were provided (i.e. they were passed in as -1), set them to good defaults
	if (fEndRange < 0.0f)
		fEndRange = fEndTime;
	if (fStartRange > 0.0f)
	{
		if (fStartRange > fEndRange)
			fStartRange = fEndRange;
	}
	else
		fStartRange = 0.0f;
	if (fEndRange > fEndTime)
		fEndRange = fEndTime;
	//validate that the index falls in the range
	if ((fStartRange + fIndex) > fEndRange)
		fIndex = fEndRange - fStartRange;
	//calculate options for animations and attach frame
	if (FAILED(hr = pFrame->QueryInterface(IID_IDirect3DRMFrame3,(void**)&pFrame3)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAnimation2->SetFrame(pFrame3)))
		goto ERROR_ENCOUNTERED;
	if (lOptions & 1) dwOptions = D3DRMANIMATION_OPEN;
	if (lOptions & 2) dwOptions = D3DRMANIMATION_CLOSED;
	dwOptions |= D3DRMANIMATION_SCALEANDROTATION;

	//create property list to pass to renderer
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pPropList)))
		goto ERROR_ENCOUNTERED;
	pPropList->AddLong((long)(void*)pAnimation2);
	pPropList->AddLong(lSystemTime);
	pPropList->AddLong(dwOptions);

	var2 = (fEndRange - fStartRange);
	pPropList->Add(var2);

	pPropList->AddString(bstrInterpName);
	pPropList->AddThing( pThis );
	
	var2 = fTimeScale;
	pPropList->Add(var2);

	var2 = fIndex;
	pPropList->Add(var2);

	if (IsMethodThisSide(pThis, bstrCallBack))
		pPropList->AddString(bstrCallBack); 
	else
		pPropList->AddString(CComBSTR(""));

	var2 = fStartRange;
	pPropList->Add(var2);

	var2 = fEndRange;
	pPropList->Add(var2);

	//pass list to the renderer
	if(FAILED(hr = m_pWorld->FireUIEvent(NULL,bstrAddInterp,CComVariant((IUnknown*)pPropList))))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	pvwfTransform.Release();
	pFrameInterpolator.Release();
	pFrame.Release();
	pFrame3.Release();
	pAnimation2.Release();
	pPropList.Release();
	pVector.Release();
	pd3drm.Release();
	pd3drm3.Release();
	pPropPath.Release();
	pIntScale.Release();

	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::StartScalePathAnimation Failed\n");
		return ReportMMError(hr);
	}


	return hr;
}
	
//StartLightPropertyPathAnimation
STDMETHODIMP CMultimediaExemplarObject::StartLightPropertyPathAnimation(long lPropertyFlag, IPropertyMap* pPathMap, float fTimeScale, float fStartRange, float fEndRange, float fIndex, CComBSTR bstrCallBack, long lOptions, IThing* pThis,long lSystemTime)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CComPtr<IVWFrame>					pvwfTransform;
	CComPtr<IDirect3DRMInterpolator>	pInterpolator;
	CComPtr<IDirect3DRMFrame>			pFrameInterpolator, pFrame;
	CComPtr<IDirect3DRMLight>			pLightInterpolator, pLight;
	CComPtr<IPropertyList>				pPropList;
	CComPtr<IVector>					pVector;
	CComPtr<IDirect3DRM>				pd3drm;
	CComPtr<IDirect3DRM3>				pd3drm3;
	CComPtr<IPropertyMap>				pPropPath;
	CComBSTR							bstrInterpName,bstrPathName,bstrStartName;
	CComVariant							var, var2;
	VARIANT_BOOL						bIsEmpty,bDone;
    HRESULT								hr = S_OK;
	BSTR								bstrName = NULL;
	char								buff[50];
	float								fEndTime = 0.0f,fX,fY,fZ,fEndX,fEndY,fEndZ, fTime;
	DWORD								dwOptions;
	static CComBSTR						bstrAddInterp("AddAnimation");

	//Create the unique name for the animation
	_ltoa((long)(void*)pThis,buff,16);
	bstrInterpName = buff;
	bstrInterpName.Append("LightColor");
	
	if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)))
		goto ERROR_ENCOUNTERED;
	if (!pvwfTransform)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if ((FAILED(hr = pvwfTransform->get_Frame3D(&pFrame))) || (!pFrame) )
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pWorld->get_Tool(CComBSTR("Direct3DRM"), (IUnknown**)&pd3drm)))
		goto ERROR_ENCOUNTERED;
	if (!pPathMap) 
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pPathMap->get_IsEmpty(&bIsEmpty)))
		goto ERROR_ENCOUNTERED;
	if (bIsEmpty == TRUE) 
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pd3drm->CreateObject(CLSID_CDirect3DRMLightInterpolator, 0, IID_IDirect3DRMInterpolator, (void**)&pInterpolator)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pInterpolator->QueryInterface(IID_IDirect3DRMLight, (void**)&pLightInterpolator)))
		goto ERROR_ENCOUNTERED;

	hr = pPathMap->FirstItem(&bstrName, &var, &bDone);
	if (FAILED(hr) && hr != VWOBJECT_E_PERMISSION_DENIED)
		goto ERROR_ENCOUNTERED;

	while (bDone != VARIANT_TRUE)
	{
		//ignore anything thats not a vector or a dispatch type in the map
		if (var.vt != VT_DISPATCH)
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		if (FAILED(hr = var.pdispVal->QueryInterface(IID_IVector,(void**)&pVector)))
			goto ERROR_ENCOUNTERED;
	
		if (hr != VWOBJECT_E_PERMISSION_DENIED)
		{		
			pVector->get(&fX, &fY, &fZ);
			fTime = (float)(atoi(CString(bstrName)));
			//keep track of the ending time and value
			if (fEndTime<fTime)  
			{ 
				fEndTime = fTime;
				fEndX = fX;
				fEndY = fY;
				fEndZ = fZ;
			} 
			pInterpolator->SetIndex(fTime);
			//0 for LightAttenuation, 1 for LightColor
			if (lPropertyFlag)
			{
				pLightInterpolator->SetConstantAttenuation(fX);
				pLightInterpolator->SetLinearAttenuation(fY);
				pLightInterpolator->SetQuadraticAttenuation(fZ);
			}
			else
			{
				pLightInterpolator->SetColorRGB(fX,fY,fZ);
			}
		}
		// free stuff
		SAFEFREESTRING(bstrName);
		pVector.Release();
		// go on to the next one
		hr = pPathMap->NextItem(&bstrName, &var, &bDone);
		if (FAILED(hr) && hr != VWOBJECT_E_PERMISSION_DENIED)
			goto ERROR_ENCOUNTERED;
	}

	//if no start/end ranges were provided (i.e. they were passed in as -1), set them to good defaults
	if (fEndRange < 0.0f)
		fEndRange = fEndTime;
	if (fStartRange > 0.0f)
	{
		if (fStartRange > fEndRange)
			fStartRange = fEndRange;
	}
	else
		fStartRange = 0.0f;
	if (fEndRange > fEndTime)
		fEndRange = fEndTime;
	//validate that the index falls in the range
	if ((fStartRange + fIndex) > fEndRange)
		fIndex = fEndRange - fStartRange;

	if (lOptions & 1) dwOptions = D3DRMINTERPOLATION_OPEN;
	if (lOptions & 2) dwOptions = D3DRMINTERPOLATION_CLOSED;
	if (lOptions & 4) dwOptions |= D3DRMINTERPOLATION_LINEAR;
	if (lOptions & 8) dwOptions |= D3DRMINTERPOLATION_SPLINE;
	if (lOptions & 16) dwOptions |= D3DRMINTERPOLATION_NEAREST;

	if (FAILED(hr = GetLightFromFrame(pFrame,&pLight)))
	{
		VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::StartLightPropertyPathAnimation: no light found on the frame and/or lightenabled property is false.\n");
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pInterpolator->AttachObject((IDirect3DRMObject*)pLight)))
		goto ERROR_ENCOUNTERED;
	
	//create property list to pass to renderer
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pPropList)))
		goto ERROR_ENCOUNTERED;

	pPropList->AddLong((long)(void*)pInterpolator);
	pPropList->AddLong(lSystemTime);
	pPropList->AddLong(dwOptions);

	var2 = (fEndRange - fStartRange);
	pPropList->Add(var2);

	pPropList->AddString(bstrInterpName);
	pPropList->AddThing( pThis );
	
	var2 = fTimeScale;
	pPropList->Add(var2);

	var2 = fIndex;
	pPropList->Add(var2);

	if (IsMethodThisSide(pThis, bstrCallBack))
		pPropList->AddString(bstrCallBack); 
	else
		pPropList->AddString(CComBSTR(""));

	var2 = fStartRange;
	pPropList->Add(var2);

	var2 = fEndRange;
	pPropList->Add(var2);

	//pass list to the renderer
	if(FAILED(hr = m_pWorld->FireUIEvent(NULL,bstrAddInterp,CComVariant((IUnknown*)pPropList))))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	pvwfTransform.Release();
	pInterpolator.Release();
	pFrameInterpolator.Release();
	pFrame.Release();
	pLightInterpolator.Release();
	pLight.Release();
	pPropList.Release();
	pVector.Release();
	pd3drm.Release();
	pd3drm3.Release();
	pPropPath.Release();

	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::StartLightPropertyPathAnimation Failed\n");
		return ReportMMError(hr);
	}
	return hr;
}


// Sets up the proper interpolation and adds it to the global list.
STDMETHODIMP CMultimediaExemplarObject::StartD3DInterpolation(BSTR bstrProperty, DWORD dwTime)
{
	CComPtr<IVWAnimator>				pVWAnimator;
	CComPtr<IVWAnimation>				pAnimation;
	CComPtr<IDirect3DRMInterpolator>	pInterpolator;
	CComPtr<IPropertyList>				pPropList;
	CComPtr<IDirect3DRMFrame>			pFrameInterpolator, pFrame;
	CComPtr<IThing>						pThing;
	VARIANT_BOOL						bIsStateMachine = VARIANT_FALSE, bServerSide;
	CComPtr<IVWFrame>					pvwfTransform, pvwfParent;
	CComPtr<IVector>					pvStart, pv, pvEnd;
	float								fStartX, fStartY, fStartZ, fEndX, fEndY, fEndZ;
	CComBSTR							bstrDone, bstrDoneProperty;
	CString								strProperty(bstrProperty);
	CComPtr<IDirect3DRM>				pd3drm;
	CComVariant							var;
	HRESULT hr = S_OK;

	static CComBSTR	bstrAnimator("Animator");
	static CComBSTR	bstrAddInterp("AddAnimation");
	
	if (dwTime > 0xFFFFFFFF) 
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	//position is the only single point interpolation supported
	if (strProperty != "Position") 
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	if (FAILED(hr = m_pWorld->get_This(&pThing)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = _GetFrameHelper(pThing, &pvwfTransform)))
		goto ERROR_ENCOUNTERED;

	// Get the global animator tool we need from the world.
	if (FAILED(hr = m_pWorld->get_Tool(bstrAnimator, (IUnknown**)&pVWAnimator)))
		goto ERROR_ENCOUNTERED;
	
	if (FAILED(hr = m_pWorld->get_ServerSide(&bServerSide)))
		return hr;

    if (bServerSide)
	{
	
		//If there is an OnDone method (and we're on the server),
		//set up a Vworlds animation to call it when the
		//animation finishes, otherwise it will get handled by the renderer
		bstrDoneProperty = (strProperty + CString("Done"));
		pThing->get_String(bstrDoneProperty, &bstrDone.m_str);

		pThing->IsOfType(CComBSTR("StateMachine"), &bIsStateMachine);
		if ((bIsStateMachine && IsStateMethodThisSide(pThing, bstrDone)) ||
			IsMethodThisSide(pThing, bstrDone))
		{
			if (FAILED(hr = pVWAnimator->CreateClosedExt(((int)dwTime), ((int)1), pThing, bstrDone, NULL, &pAnimation )))
				goto ERROR_ENCOUNTERED;
		}

		//we no longer need to be in here, since on the server we would have a null frame anyway
		return hr;
	}
	if (!pvwfTransform) goto ERROR_ENCOUNTERED;

	if (FAILED(hr = m_pWorld->get_Tool(CComBSTR("Direct3DRM"), (IUnknown**)&pd3drm)))
		goto ERROR_ENCOUNTERED;

	//Get the ending and starting points
	if (FAILED(hr = pThing->get_ObjectProperty(bstrProperty, (IObjectProperty**)&pv)))
		goto ERROR_ENCOUNTERED;
 	if (FAILED(hr = pv->get(&fEndX, &fEndY, &fEndZ)))
		goto ERROR_ENCOUNTERED;

	if ((FAILED(hr = pvwfTransform->GetParent(&pvwfParent))) || (!pvwfParent))
		goto ERROR_ENCOUNTERED;
	
	if (strProperty == "Position") {
		if (FAILED(hr = pvwfTransform->GetPositionEx(pvwfParent, &fStartX, &fStartY, &fStartZ)))
			goto ERROR_ENCOUNTERED;
	}

	//If we're animating between the same two points just leave
	if (FLOAT_EQUAL(fStartX,fEndX) && FLOAT_EQUAL(fStartY,fEndY) && FLOAT_EQUAL(fStartZ,fEndZ))
		return S_OK;
	
	//Create the interpolator
	if (FAILED(hr = pd3drm->CreateObject(CLSID_CDirect3DRMFrameInterpolator, 0, IID_IDirect3DRMInterpolator, (void**)&pInterpolator)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pInterpolator->QueryInterface(IID_IDirect3DRMFrame, (void**)&pFrameInterpolator)))
		goto ERROR_ENCOUNTERED;

	//Set the start and end points in the interpolator
	if (FAILED(hr = pInterpolator->SetIndex(0)))
		goto ERROR_ENCOUNTERED;
	if (strProperty == "Position") {
		if (FAILED(hr = pFrameInterpolator->SetPosition(NULL,fStartX,fStartY,fStartZ)))
			goto ERROR_ENCOUNTERED;
	}

	if (FAILED(hr = pInterpolator->SetIndex((float)dwTime)))
		goto ERROR_ENCOUNTERED;
	if (strProperty == "Position") {
		if (FAILED(hr = pFrameInterpolator->SetPosition(NULL,fEndX,fEndY,fEndZ)))
			goto ERROR_ENCOUNTERED;
	}

	//Attach the proper object
	if (FAILED(hr = pvwfTransform->get_Frame3D(&pFrame)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pInterpolator->AttachObject((IDirect3DRMObject*)pFrame)))
		goto ERROR_ENCOUNTERED;

	//Create a property list for the render thread
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pPropList)))
		goto ERROR_ENCOUNTERED;

	pPropList->AddLong((long)(void*)pInterpolator);
	pPropList->AddLong(GetTickCount());
	pPropList->AddLong(D3DRMINTERPOLATION_OPEN | D3DRMINTERPOLATION_LINEAR);

	var.Clear();
	var.vt = VT_R4;
	var.fltVal = ((float)dwTime);
	pPropList->Add(var);
	pPropList->AddString(CComBSTR("PositionDuration"));


	pPropList->AddThing( pThing );
	
	var.Clear();
	var.vt = VT_R4;
	var.fltVal = 1.0f;
	pPropList->Add(var);

	var.Clear();
	var.vt = VT_R4;
	var.fltVal = 0.0f;
	pPropList->Add(var);

	if (IsMethodThisSide(pThing, CComBSTR("OnPositionDone")))
		pPropList->AddString(CComBSTR("OnPositionDone")); 
	else
		pPropList->AddString(CComBSTR(""));

	var.Clear();
	var.vt = VT_R4;
	var.fltVal = 0.0f;
	pPropList->Add(var);

	var.Clear();
	var.vt = VT_R4;
	var.fltVal = ((float)dwTime);
	pPropList->Add(var);


	//Send everything off to the render thread
	if(FAILED(hr = m_pWorld->FireUIEvent(pThing,bstrAddInterp,CComVariant((IUnknown*)pPropList))))
		goto ERROR_ENCOUNTERED;
	
ERROR_ENCOUNTERED:
	pVWAnimator.Release();
	pAnimation.Release();
	pInterpolator.Release();
	pPropList.Release();
	pFrameInterpolator.Release();
	pFrame.Release();
	pThing.Release();
	pvwfTransform.Release();
	pvwfParent.Release();
	pvStart.Release();
	pv.Release();
	pvEnd.Release();
	pd3drm.Release();
	var.Clear();

	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::StartD3DInterpolation Failed\n");
		return ReportMMError(hr);
	}

	return hr;

}

//******************************************************
// StartD3DAnimation
// This replaces the propertyDuration animations
// Sets up the proper animation and adds it to the global list.
STDMETHODIMP CMultimediaExemplarObject::StartD3DAnimation(BSTR bstrProperty, DWORD dwTime)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CComPtr<IVWAnimator>				pVWAnimator;
	CComPtr<IVWAnimation>				pAnimation;
	CComPtr<IDirect3DRMAnimation2>		pAnimation2;
	CComPtr<IDirect3DRMFrame>			pFrame;
	CComPtr<IDirect3DRMFrame3>			pFrame3;
	CComPtr<IThing>						pThing;
	VARIANT_BOOL						bIsStateMachine = VARIANT_FALSE;
	CComPtr<IVWFrame>					pvwfTransform, pvwfParent;
	CComPtr<IVector>					pvStart, pv, pvEnd;
	float								fAngle,fStartX, fStartY, fStartZ, fEndX, fEndY, 
										fEndZ,fDirX,fDirY,fDirZ,fUpX,fUpY,fUpZ,fDot;
	CComBSTR							bstrDone, bstrDoneProperty,bstrAnimName;
	CString								strProperty(bstrProperty);
	CComPtr<IDirect3DRM>				pd3drm;
	CComPtr<IDirect3DRM3>				pd3drm3;
	CComPtr<IPropertyList>				pPropList;
	HRESULT hr = S_OK;
	D3DRMQUATERNION						quatDir,quatfinalend,quatfinalstart,quatUp;
	D3DVECTOR							vSrc, vDest, vRotate;
	int									ctr;
	CComVariant							var;
	static CComBSTR	bstrAnimator("Animator");
	static CComBSTR	AddAnimation("AddAnimation");
	static CComBSTR bstrScaleD("ScaleDuration");
	static CComBSTR bstrDirD("DirectionDuration");
	static CComBSTR bstrUpD("UpDuration");

	if (dwTime > 0xFFFFFFFF) 
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	//the position animation is handled by an interpolation
	if (strProperty == "Scale") bstrAnimName = bstrScaleD;
	else if (strProperty == "Direction") bstrAnimName = bstrDirD;
	else if (strProperty == "Up") bstrAnimName = bstrUpD;
	else {
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	if (FAILED(hr = m_pWorld->get_This(&pThing)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = _GetFrameHelper(pThing, &pvwfTransform)))
		goto ERROR_ENCOUNTERED;

	// Get the global animator tool we need from the world.
	if (FAILED(hr = m_pWorld->get_Tool(bstrAnimator, (IUnknown**)&pVWAnimator)))
		goto ERROR_ENCOUNTERED;
	
	//If there is an OnDone method set up a Vworlds animation to call
	//it when the animation finishes
	bstrDoneProperty = (strProperty + CString("Done"));
	pThing->get_String(bstrDoneProperty, &bstrDone.m_str);

	pThing->IsOfType(CComBSTR("StateMachine"), &bIsStateMachine);
	if ((bIsStateMachine && IsStateMethodThisSide(pThing, bstrDone)) ||
		IsMethodThisSide(pThing, bstrDone))
	{
		if (FAILED(hr = pVWAnimator->CreateClosedExt(((int)dwTime), ((int)1), pThing, bstrDone, NULL, &pAnimation )))
			goto ERROR_ENCOUNTERED;
	}
	if (!pvwfTransform) //we are on the server, so bail out
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = m_pWorld->get_Tool(CComBSTR("Direct3DRM"), (IUnknown**)&pd3drm)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pd3drm->QueryInterface(IID_IDirect3DRM3, (void**)&pd3drm3)))
		goto ERROR_ENCOUNTERED;
	//Get the start and end values
	if (FAILED(hr = pThing->get_ObjectProperty(bstrProperty, (IObjectProperty**)&pv)))
		goto ERROR_ENCOUNTERED;
 	if (FAILED(hr = pv->get(&fEndX, &fEndY, &fEndZ)))
		goto ERROR_ENCOUNTERED;
	if ((FAILED(hr = pvwfTransform->GetParent(&pvwfParent))) || (!pvwfParent))
		goto ERROR_ENCOUNTERED;
	if (strProperty == "Scale") 
	{
		if (FAILED(hr = pvwfTransform->GetScaleEx(pvwfParent, &fStartX, &fStartY, &fStartZ)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = pvwfTransform->SetInternalScale(fEndX, fEndY, fEndZ)))
			goto ERROR_ENCOUNTERED;		
	}
	if (FAILED(hr = pvwfTransform->get_Frame3D(&pFrame)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pFrame->QueryInterface(IID_IDirect3DRMFrame3,(void**)&pFrame3)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pvwfTransform->GetOrientationEx(NULL, &fDirX, &fDirY, &fDirZ, &fUpX, &fUpY, &fUpZ)))
		goto ERROR_ENCOUNTERED;
	if (strProperty == "Up") 
	{
		vSrc.x = fEndX;
		vSrc.y = fEndY;
		vSrc.z = fEndZ;
		vDest.x = fDirX;
		vDest.y = fDirY;
		vDest.z = fDirZ;
		
		fDot = D3DRMVectorDotProduct(&vSrc,&vDest);
		vRotate.x = vDest.x * fDot;
		vRotate.y = vDest.y * fDot;
		vRotate.z = vDest.z * fDot;

		fEndX = vSrc.x - vRotate.x;
		fEndY = vSrc.y - vRotate.y;
		fEndZ = vSrc.z - vRotate.z;
		
		//fEnd now equals the up vector projected onto the plane that the direction vector
		//is the normal of.  If the up and direction vectors were parallel vRotate will be (0,0,0)
		if (!fEndX && !fEndY && !fEndZ) 
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
	}
	//Turn the starting value for the orientation into a quaternion
	//If this is an orientation animation then the start and end values
	//need to be turned into quaternions.
	for(ctr=0;ctr<2;ctr++) {
		//Only perform twice if this is an orientation animation
		if ( (!ctr) || (strProperty!="Scale") ) {
			if (ctr) {
				//if this is the second calculation store the first result in quatfinalstart
				quatfinalstart = quatfinalend;
				if (strProperty=="Direction") {
					fDirX = fEndX;
					fDirY = fEndY;
					fDirZ = fEndZ;
				}
				else {
					fUpX = fEndX;
					fUpY = fEndY;
					fUpZ = fEndZ;
				}
			}
			vSrc.x = 0;
			vSrc.y = 1;
			vSrc.z = 0;
			vDest.x = fUpX;
			vDest.y = fUpY;
			vDest.z = fUpZ;

			fAngle = D3DRMVectorDotProduct(&vSrc,&vDest) / D3DRMVectorModulus(&vDest);
			D3DRMVectorCrossProduct(&vRotate, &vSrc, &vDest);
			D3DRMQuaternionFromRotation(&quatUp, &vRotate, (float)(-acos(fAngle)));

			//this is a temporary storage of the z-axis, not the real destination
			vDest.x = 0;
			vDest.y = 0;
			vDest.z = 1;
			D3DRMVectorRotate(&vSrc,&vDest,&vRotate,(float)(acos(fAngle)));

			//this is the real destination
			vDest.x = fDirX;
			vDest.y = fDirY;
			vDest.z = fDirZ;

			D3DRMVectorCrossProduct(&vRotate, &vSrc, &vDest);
			fAngle = D3DRMVectorDotProduct(&vSrc,&vDest) / (D3DRMVectorModulus(&vSrc)*D3DRMVectorModulus(&vDest));
			D3DRMQuaternionFromRotation(&quatDir, &vRotate, (float)(-acos(fAngle)));

			D3DRMQuaternionMultiply(&quatfinalend,&quatUp,&quatDir);
		}
	}

	//If this is a scale animation the orientation can't change.
	if (strProperty == "Scale") {
		quatfinalstart = quatfinalend;
		//If we're animating between the same two points just leave
		if (FLOAT_EQUAL(fStartX,fEndX) && FLOAT_EQUAL(fStartY,fEndY) && FLOAT_EQUAL(fStartZ,fEndZ))
			return S_OK;
	}
	else {
		if (FAILED(hr = pvwfTransform->GetScaleEx(pvwfParent, &fStartX, &fStartY, &fStartZ)))
			goto ERROR_ENCOUNTERED;
		fEndX = fStartX;
		fEndY = fStartY;
		fEndZ = fStartZ;

		//If we're animating between the same two points just leave
		if (FLOAT_EQUAL(quatfinalstart.s,quatfinalend.s) && FLOAT_EQUAL(quatfinalstart.v.x,quatfinalend.v.x) && FLOAT_EQUAL(quatfinalstart.v.y,quatfinalend.v.y) && FLOAT_EQUAL(quatfinalstart.v.z,quatfinalend.v.z))
			return S_OK;
	}

	//Create the animation and add the keys
	if (FAILED(pd3drm3->CreateAnimation(&pAnimation2)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pAnimation2->AddRotateKey(0,&quatfinalstart)))
		goto ERROR_ENCOUNTERED;	
	if (FAILED(hr = pAnimation2->AddRotateKey((float)dwTime,&quatfinalend)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAnimation2->AddScaleKey(0,fStartX,fStartY,fStartZ)))
		goto ERROR_ENCOUNTERED;	
	if (FAILED(hr = pAnimation2->AddScaleKey((float)dwTime,fEndX,fEndY,fEndZ)))
		goto ERROR_ENCOUNTERED;

	//Attach the frame, and create the property list
	if (FAILED(hr = pAnimation2->SetFrame(pFrame3)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = CreatePropertyList(m_pWorld, &pPropList)))
		goto ERROR_ENCOUNTERED;

	pPropList->AddLong((long)(void*)pAnimation2);
	pPropList->AddLong(GetTickCount());
	pPropList->AddLong(D3DRMANIMATION_OPEN | D3DRMANIMATION_LINEARPOSITION | D3DRMANIMATION_SCALEANDROTATION);
	var.Clear();
	var.vt = VT_R4;
	var.fltVal = ((float)dwTime);
	pPropList->Add(var);
	pPropList->AddString(bstrAnimName);

	pPropList->AddThing( pThing );
	
	var.Clear();
	var.vt = VT_R4;
	var.fltVal = 1.0f;
	pPropList->Add(var);

	var.Clear();
	var.vt = VT_R4;
	var.fltVal = 0.0f;
	pPropList->Add(var);

	if (IsMethodThisSide(pThing,bstrDone))
		pPropList->AddString(bstrDone); 
	else
		pPropList->AddString(CComBSTR(""));

	var.Clear();
	var.vt = VT_R4;
	var.fltVal = 0.0f;
	pPropList->Add(var);

	var.Clear();
	var.vt = VT_R4;
	var.fltVal = ((float)dwTime);
	pPropList->Add(var);

	//Send it all off to the render thread.
	if(FAILED(hr = m_pWorld->FireUIEvent(NULL,AddAnimation,CComVariant((IUnknown*)pPropList))))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	pVWAnimator.Release();
	pAnimation.Release();
	pAnimation2.Release();
	pFrame.Release();
	pFrame3.Release();
	pThing.Release();
	pvwfTransform.Release();
	pvwfParent.Release();
	pvStart.Release();
	pv.Release();
	pvEnd.Release();
	pd3drm.Release();
	pd3drm3.Release();
	pPropList.Release();
	var.Clear();

	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::StartD3DAnimation Failed\n");
		return ReportMMError(hr);
	}

	return hr;
}


STDMETHODIMP CMultimediaExemplarObject::GetAnimationStartTime(BSTR bstrPropertyName, long* AnimStartTime) 
{		
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HRESULT		hr = S_OK;
	static CString	strFile("File");
	static CString	strPosition("Position");
	static CString	strDirection("Direction");
	static CString	strUp("Up");
	static CString	strScale("Scale");
	static CString	strLightColor("LightColor");
	static CString	strLightAttenuation("LightAttenuation");
	CComBSTR		bstrPropertyAnimStartTime(bstrPropertyName);
	CComBSTR		bstrPropertyAnimProperties(bstrPropertyName);
	CString strPropertyName = bstrPropertyName;
	CComPtr<IThing>		pThis;
	CComPtr<IPropertyMap>	pAnimPropertiesMap;
	CComVariant		var, var2;
	bstrPropertyAnimStartTime.Append("AnimStartTime");
	bstrPropertyAnimProperties.Append("AnimProperties");

	//error out on non-supported anim types..
	if ((strPropertyName.CompareNoCase(strFile)!=0) && 
		(strPropertyName.CompareNoCase(strPosition)!=0) &&
		(strPropertyName.CompareNoCase(strDirection)!=0) &&
		(strPropertyName.CompareNoCase(strUp)!=0) &&
		(strPropertyName.CompareNoCase(strScale)!=0) &&
		(strPropertyName.CompareNoCase(strLightColor)!=0) &&
		(strPropertyName.CompareNoCase(strLightAttenuation)!=0))
	{	
		VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::GetAnimationStartTime: invalid animation property name\n");
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::GetAnimationStartTime: invalid animation property name\n");
		return E_FAIL;
	}

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThis->get_Property(bstrPropertyAnimProperties, &var)))
	   goto ERROR_ENCOUNTERED;

	//get any previous good values from it to indicate if a previous Done method invocation is required by the server
	if (var.vt != VT_DISPATCH)
		goto ERROR_ENCOUNTERED;

	if(var.pdispVal)
		if (SUCCEEDED(hr = var.pdispVal->QueryInterface(IID_IPropertyMap,(void**)&pAnimPropertiesMap)))
		{
			if (FAILED(hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimStartTime, &var2) ))
				goto ERROR_ENCOUNTERED;
			*AnimStartTime = CastToLong(var2);
		}

ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{	
		VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::GetAnimationStartTime: An internal error occurred\n");
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::GetAnimationStartTime: An internal error occurred\n");
		return E_FAIL;
	}
	return S_OK;
}



STDMETHODIMP CMultimediaExemplarObject::GetAnimationFileKeys(IPropertyList** ppKeyList) 
{		
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	HRESULT					hr = S_OK;
	CComPtr<IThing>			pThis;
	CComPtr <IPropertyList> pPropList;
	CComPtr <IVWFrame>		pParentFrame;
	CComPtr<IVWFrame>		pvwfGeometry, pvwfTransform;
	CComPtr<IVWGeometry>	pVWGeometry;
	CComPtr<IDirect3DRMAnimationSet2> pAnimationSet;
	CComPtr <IDirect3DRMAnimationArray> pD3DAnimArray;
	CComPtr <IDirect3DRMAnimation2> pD3DAnim;
	CComVariant				var;
	float					fEndTime = 0.0f;
	DWORD					dwNumKeys = 0;
	unsigned				int j;
	D3DRMANIMATIONKEY*		pKeys = NULL;
	int						nAnimCount, i;
	static CComBSTR			bstrGeometry("Geometry");

	hr = CreatePropertyList(m_pWorld, ppKeyList);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)))
		goto ERROR_ENCOUNTERED;
	if (!pvwfTransform)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pThis->get_ObjectPropertyExt(bstrGeometry, IID_IVWGeometry, (IObjectProperty**)&pVWGeometry)))
		goto ERROR_ENCOUNTERED;
	if (pVWGeometry==NULL)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pVWGeometry->get_GeometryFrame(&pvwfGeometry)))
		goto ERROR_ENCOUNTERED;
	if (pvwfGeometry==NULL)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pvwfGeometry->get_AnimationSet(&pAnimationSet)))
		goto ERROR_ENCOUNTERED;
	if (pAnimationSet == NULL)
	{
		hr = S_OK;
		goto ERROR_ENCOUNTERED;
	}
	pAnimationSet->GetAnimations(&pD3DAnimArray);
	nAnimCount = pD3DAnimArray->GetSize();

	//iterate through each animation in the set
	for (i = 0; i < nAnimCount; i++)
	{
		//get an animation from the set
		if (FAILED(hr = pD3DAnimArray->GetElement(i, &pD3DAnim)))
			goto ERROR_ENCOUNTERED;
		//get the number of keys we have in this animation in the set
		pD3DAnim->GetKeys(0, 9999999, &dwNumKeys, NULL);
		//declare a key array of the right size to iterate through
		pKeys = new D3DRMANIMATIONKEY[dwNumKeys];
		//put the keys into the key array
		pD3DAnim->GetKeys(0, 9999999, &dwNumKeys, pKeys);
		//iterate the array, looking at the value of each key to find highest
		for (j = 0; j < dwNumKeys; j++)
		{
			if (fEndTime < (float)(pKeys[j].dvTime))
			{
				//compare to the current highest key in the list, if there is a higher one, add it to the list
				fEndTime = (float)(pKeys[j].dvTime);
				(*ppKeyList)->Add(CComVariant(fEndTime));
			}			
		}
		delete [] pKeys;
		pD3DAnim.Release();
	}
	//check that we didnt have an overly huge time value in one of the keys
	if (fEndTime > 0xFFFFFFFF) 
	{
		hr = E_FAIL; 
		goto ERROR_ENCOUNTERED;
	}
ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{
		VWREPORT(m_pWorld, VWR_ERROR,  "CMultimediaExemplarObject::GetAnimationFileKeys: An error occurred\n");
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::GetAnimationFileKeys Error\n");
		return ReportMMError(hr);
	}

	return hr;
	
}

STDMETHODIMP CMultimediaExemplarObject::ULongAdd(unsigned long lNum1, unsigned long lNum2, unsigned long * lNum3) 
{
	const unsigned long ulHalfMax = (unsigned long)ULONG_MAX/2;
		//2147483647l;
	if (lNum2 >= ulHalfMax)
	{
		//you might wrap
		if (lNum1 >= ulHalfMax)
			//you will wrap
			*lNum3 = (lNum1-ulHalfMax)+(lNum2-ulHalfMax);
		else
		{
			//you might wrap
			if (lNum1 >= (ULONG_MAX - lNum2))
				//you will wrap 
				*lNum3 = lNum1 - (ULONG_MAX - lNum2);
			else
				*lNum3 = lNum1 + lNum2;
		}
	}
	else 
	{
		if (lNum1 >= ulHalfMax)
		{
				//you might wrap
				if (lNum2 >= (ULONG_MAX - lNum1))
					//you will wrap 
					*lNum3 = lNum2 - (ULONG_MAX - lNum1);
				else
					*lNum3 = lNum2 + lNum1;
		}
		else
			*lNum3 = lNum2 + lNum1;
	}
	return S_OK;
}

STDMETHODIMP CMultimediaExemplarObject::ULongSubtract(unsigned long lNum1, unsigned long lNum2, unsigned long * lNum3) 
{
	HRESULT hr = S_OK;
	if (lNum2 > lNum1)
		hr = ULongAdd(lNum1, (ULONG_MAX - lNum2), lNum3);
	else
		*lNum3 = lNum1 - lNum2;

	return hr;
}


STDMETHODIMP CMultimediaExemplarObject::SetClientSystemTime(long lSystemTime) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CComPtr <IThing>	pThis;
	CComPtr <IThing>	pUser;
	CComPtr <IThing>	pGlobal;
	HRESULT				hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	if (FAILED(hr = m_pWorld->get_User(&pUser)))
		return hr;

	if (pUser == pThis)
	{
		if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
			return hr;

		if (FAILED(hr = pGlobal->put_Long(CComBSTR("SystemStartTicks"), (long)GetTickCount())))
			return hr;

		if (FAILED(hr = pGlobal->put_Long(CComBSTR("SystemTime"), lSystemTime)))
			return hr;
	}
	return hr;
}


//******************************************************
// GestureText
// Given a gesture name, returns the text associated with
// it.  This one is just a place-holder, and always fails.

STDMETHODIMP CMultimediaExemplarObject::GestureText(BSTR bstrGesture, BSTR *pbstrText)
{
    HRESULT                 hr = S_OK;
	CComPtr<IThing>			pThis;
	CComPtr<IVWGeometry>	pVWGeometry;
	static CComBSTR			bstrGeometry("Geometry");

    if (pbstrText==NULL)
	{
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::GestureText: NULL pointer\n");
		return ReportMMError(E_POINTER);
	}
    
    *pbstrText = NULL;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;
	if (FAILED(hr = pThis->get_ObjectProperty(bstrGeometry, (IObjectProperty**)&pVWGeometry)))
		return hr;

    if (pVWGeometry == NULL)
        return S_OK;

	// parse the gesture from the geometry....
	hr = pVWGeometry->get_GestureText(bstrGesture, pbstrText);

	return hr;
}

//******************************************************
// DoNamedGesture
// Performs a gesture by name

STDMETHODIMP CMultimediaExemplarObject::DoNamedGesture(BSTR bstrGesture)
{
	int						iGestureNum	= 0;
	CComPtr<IThing>			pThis;
	CComPtr<IVWGeometry>	pVWGeometry;
	static CComBSTR			bstrGeometry("Geometry");
	static CComBSTR			bstrDoGesture("DoGesture");
    HRESULT                 hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;
	if (FAILED(hr = pThis->get_ObjectProperty(bstrGeometry, (IObjectProperty**)&pVWGeometry)))
		return hr;

    if (pVWGeometry == NULL)
        return S_OK;

	// parse the gesture from the geometry....
	if (FAILED(hr = pVWGeometry->get_NamedGesture(bstrGesture, &iGestureNum)))
		return hr;
	ASSERT(iGestureNum >= 0);

    if (iGestureNum < 0)
		iGestureNum = 0;

	// now, invoke the int-based method
	TRACE1("DOGESTURE: %d\n", iGestureNum);
	
    hr = pThis->InvokeMethodExt(bstrDoGesture, CComDISPPARAMS(1, CComVariant(iGestureNum)), NULL);

	return hr;
}

//******************************************************
// DoGesture
// Performs a gesture

STDMETHODIMP CMultimediaExemplarObject::DoGesture(int iGesture)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

//	DISPPARAMS			dispparams;

	// Initialize dispparams so TerminateDispparams won't fail if not used.
//	InitializeDISPPARAMS(&dispparams, 0);

	CComPtr<IThing>				pThis;
	CComPtr<IVWGeometry>		pVWGeometry;
	CComPtr<IThing>				pConnectedUser;
    HRESULT                     hr = S_OK;

	static CComBSTR		bstrGeometry("Geometry");
	static CComBSTR		bstrDoGestureServer("DoGestureServer");

	// Who's doing the gesture?
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;
	if (FAILED(hr = pThis->get_ObjectProperty(bstrGeometry, (IObjectProperty**)&pVWGeometry)))
		return hr;

    if (pVWGeometry == NULL)
        return S_OK;

	// THIS METHOD NOW WORKS FOR ALL IVWGOEMETRY OBJECTS -- whether it works
	// or not, remote the intention ----- disla
	pVWGeometry->put_CurrentGesture(iGesture);

	if (FAILED(hr = m_pWorld->get_User(&pConnectedUser)))
		return hr;

	// Call the server side method which will deal with doing the gesture on all other clients.
	// package up args into DISPPARAMS struct

    hr = pThis->InvokeMethodExt(bstrDoGestureServer, 
                                CComDISPPARAMS(2, CComVariant((IDispatch*)pConnectedUser),
                                                  CComVariant(iGesture)),
                                NULL);

	return hr;
}

//******************************************************
// DoGestureClient
// Performs the gesture on the client

STDMETHODIMP CMultimediaExemplarObject::DoGestureClient(IThing *pThingOriginator, int iGesture)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing>				pConnectedUser;
	CComPtr<IThing>				pThis;
	CComPtr<IVWGeometry>		pVWGeometry;

	static CComBSTR		bstrGeometry("Geometry");
	HRESULT				hr = S_OK;

	// This method is only to update clients that are not me!
	if (FAILED(hr = m_pWorld->get_User(&pConnectedUser)))
		return hr;

	if (pThingOriginator != pConnectedUser)
	{
		// Who's doing the gesture?
		if (FAILED(hr = m_pWorld->get_This(&pThis)))
    		return hr;
		if (FAILED(hr = pThis->get_ObjectProperty(bstrGeometry, (IObjectProperty**)&pVWGeometry)))
	    	return hr;

        if (pVWGeometry == NULL)
            return S_OK;

		hr = pVWGeometry->put_CurrentGesture(iGesture);
	}	

	return hr;
}

//******************************************************
// GetNamedGestures
// Returns an IPropertyList of all the named gestures

STDMETHODIMP CMultimediaExemplarObject::GetNamedGestures(IPropertyList **ppGestureNameList)
{
	CComPtr<IThing>				pThis;
	CComPtr<IVWGeometry>		pVWGeometry;
	static CComBSTR				bstrGeometry("Geometry");
	HRESULT hr = S_OK;

    if (ppGestureNameList==NULL)
	{
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::GetNamedGestures: NULL pointer\n");
		return ReportMMError(E_POINTER);
	}
    *ppGestureNameList = NULL;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;
	if (FAILED(hr = pThis->get_ObjectProperty(bstrGeometry, (IObjectProperty**)&pVWGeometry)))
		return hr;
	if (pVWGeometry == NULL)
        return S_OK;

    hr = pVWGeometry->GetNamedGestures(ppGestureNameList);
	return hr;
}

//******************************************************
// InitializeGraphics
HRESULT CMultimediaExemplarObject::InitializeGraphics(BSTR bstrGeometryName,
													float fXPos, float fYPos, float fZPos,
													float fXDir, float fYDir, float fZDir,
													VARIANT varXScale, VARIANT varYScale, VARIANT varZScale)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
	CComPtr<IVector> pVector;
    HRESULT hr = S_OK;

	static CComBSTR	bstrGeometry("Geometry");
	static CComBSTR	bstrGeometryNameProp("GeometryName");

	// Get this
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	// Set position
	if (FAILED(hr = CreateVector(fXPos, fYPos, fZPos, &pVector)))
		return hr;
	if (FAILED(hr = pThis->put_ObjectProperty(CComBSTR("Position"), pVector)))
		return hr;
	pVector.Release();

	// Set direction
	if (FAILED(hr = CreateVector(fXDir, fYDir, fZDir, &pVector)))
		return hr;
	if (FAILED(hr = pThis->put_ObjectProperty(CComBSTR("Direction"), pVector)))
		return hr;
	pVector.Release();

	// Set "up"
	if (FAILED(hr = CreateVector(0.0, 1.0, 0.0, &pVector)))
		return hr;
	if (FAILED(hr = pThis->put_ObjectProperty(CComBSTR("Up"), pVector)))
		return hr;
	pVector.Release();

	// Set scale
	if (FAILED(hr = CreateVector(CastToFloat(varXScale), CastToFloat(varYScale), CastToFloat(varZScale), &pVector)))
		return hr;
	if (FAILED(hr = pThis->put_ObjectProperty(CComBSTR("Scale"), pVector)))
		return hr;
	pVector.Release();

	hr = pThis->put_String(bstrGeometryNameProp, bstrGeometryName);

    return hr;
}

//******************************************************
// InitializeSpriteGraphics
HRESULT CMultimediaExemplarObject::InitializeSpriteGraphics(BSTR bstrSprite,
														float fXPos, float fYPos, float fZPos,
														float fXDir, float fYDir, float fZDir,
														VARIANT varXScale, VARIANT varYScale, VARIANT varZScale)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
	CComPtr<IVector> pVector;
    HRESULT hr = S_OK;

	static CComBSTR	bstrGeometry("Geometry");
	static CComBSTR	bstrGeometryNameProp("GeometryName");

	// Get this
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	// Set position
	if (FAILED(hr = CreateVector(fXPos, fYPos, fZPos, &pVector)))
		return hr;
	if (FAILED(hr = pThis->put_ObjectProperty(CComBSTR("Position"), pVector)))
		return hr;
	pVector.Release();

	// Set direction
	if (FAILED(hr = CreateVector(fXDir, fYDir, fZDir, &pVector)))
		return hr;
	if (FAILED(hr = pThis->put_ObjectProperty(CComBSTR("Direction"), pVector)))
		return hr;
	pVector.Release();

	// Set "up"
	if (FAILED(hr = CreateVector(0.0, 1.0, 0.0, &pVector)))
		return hr;
	if (FAILED(hr = pThis->put_ObjectProperty(CComBSTR("Up"), pVector)))
		return hr;
	pVector.Release();

	// Set scale
	if (FAILED(hr = CreateVector(CastToFloat(varXScale), CastToFloat(varYScale), CastToFloat(varZScale), &pVector)))
		return hr;
	if (FAILED(hr = pThis->put_ObjectProperty(CComBSTR("Scale"), pVector)))
		return hr;
	pVector.Release();

	hr = pThis->put_String(bstrGeometryNameProp, bstrSprite);

	return hr;
}

HRESULT CMultimediaExemplarObject::Set3DSound(BSTR bstrWaveURL,
											 VARIANT bIsLooping,
											 VARIANT dwInsideConeAngle,
											 VARIANT dwOutsideConeAngle,
											 VARIANT lConeOutsideVolume,
											 VARIANT flMinDistance,
											 VARIANT flMaxDistance)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CVWComPtr<IThing, NULL, &IID_IThing> pThis;
	CVWComPtr<ISound, NULL, &IID_ISound> pSound;
	CComVariant var;
    HRESULT     hr = S_OK;

	static CComBSTR bstrSound("Sound");
	
	// Convert to proper types
	if (FAILED(hr = ::VariantChangeType( &bIsLooping,&bIsLooping,0,VT_BOOL)))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::Set3DSound: bad argument\n");
        return ReportMMError(E_INVALIDARG);
    }
	if (FAILED(hr = ::VariantChangeType( &dwInsideConeAngle,&dwInsideConeAngle,0,VT_I4)))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::Set3DSound: bad argument\n");
        return ReportMMError(E_INVALIDARG);
    }
	if (FAILED(hr = ::VariantChangeType( &dwOutsideConeAngle,&dwOutsideConeAngle,0,VT_I4)))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::Set3DSound: bad argument\n");
        return ReportMMError(E_INVALIDARG);
    }
	if (FAILED(hr = ::VariantChangeType( &lConeOutsideVolume,&lConeOutsideVolume,0,VT_I4)))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::Set3DSound: bad argument\n");
        return ReportMMError(E_INVALIDARG);
    }
	if (FAILED(hr = ::VariantChangeType( &flMinDistance,&flMinDistance,0,VT_R4)))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::Set3DSound: bad argument\n");
        return ReportMMError(E_INVALIDARG);
    }
	if (FAILED(hr = ::VariantChangeType( &flMaxDistance,&flMaxDistance,0,VT_R4)))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::Set3DSound: bad argument\n");
        return ReportMMError(E_INVALIDARG);
    }

	// Get this
	if (FAILED(hr = m_pWorld->get_This(&pThis.p)))
		return hr;
	//Get the sound property if it exists
	if (SUCCEEDED( pThis->get_Property( bstrSound, &var )))
	{
		pSound = var;
	}

	if (pSound == NULL )
	{
		if (FAILED(hr = CreateSound(bstrWaveURL, VARIANT_TRUE, bIsLooping.boolVal, &pSound.p)))
		    return hr;
		if (FAILED(hr =  pSound->Set3DData(dwInsideConeAngle.lVal,
									   dwOutsideConeAngle.lVal,
									   lConeOutsideVolume.lVal,
									   flMinDistance.fltVal,
									   flMaxDistance.fltVal)))
		    return hr;

		hr = pThis->put_ObjectProperty(bstrSound, pSound );
	}
	else
	{
		if (FAILED(hr =  pSound->put_Is3D( VARIANT_TRUE )))
	    	return hr;
		if (FAILED(hr =  pSound->put_IsLooping( bIsLooping.boolVal )))
		    return hr;
		if (FAILED(hr =  pSound->Set3DData(dwInsideConeAngle.lVal,
									   dwOutsideConeAngle.lVal,
									   lConeOutsideVolume.lVal,
									   flMinDistance.fltVal,
									   flMaxDistance.fltVal)))
		    return hr;

		hr =  pSound->put_SoundURL( bstrWaveURL );
	}

	return hr;
}


HRESULT CMultimediaExemplarObject::SetSound(BSTR bstrWaveURL,VARIANT bIsLooping, VARIANT lVolume, VARIANT lFrequency,VARIANT lPan)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CVWComPtr<IThing, NULL, &IID_IThing> pThis;
	CVWComPtr<ISound, NULL, &IID_ISound> pSound;
	CComVariant var;
    HRESULT hr = S_OK;

	static CComBSTR bstrSound("Sound");

	// Convert to proper types
	if (FAILED(hr = ::VariantChangeType( &bIsLooping,&bIsLooping,0,VT_BOOL)))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::SetSound: bad argument\n");
        return ReportMMError(E_INVALIDARG);
    }
	if (FAILED(hr = ::VariantChangeType( &lVolume,&lVolume,0,VT_I4)))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::SetSound: bad argument\n");
        return ReportMMError(E_INVALIDARG);
    }
	if (FAILED(hr = ::VariantChangeType( &lFrequency,&lFrequency,0,VT_I4)))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::SetSound: bad argument\n");
        return ReportMMError(E_INVALIDARG);
    }
	if (FAILED(hr = ::VariantChangeType( &lPan,&lPan,0,VT_I4)))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::SetSound: bad argument\n");
        return ReportMMError(E_INVALIDARG);
    }

	// Get this
	if (FAILED(hr = m_pWorld->get_This(&pThis.p)))
		return hr;
	//Get the sound property if it exists
	if (SUCCEEDED( pThis->get_Property( bstrSound, &var )))
	{
		pSound = var;
	}

	if (pSound == NULL )
	{
		if (FAILED(hr =  CreateSound(bstrWaveURL, VARIANT_FALSE, bIsLooping.boolVal, &pSound.p )))
		    return hr;
		if (FAILED(hr =  pSound->Set2DData(lVolume.lVal,lFrequency.lVal,lPan.lVal)))
		    return hr;
		hr = pThis->put_ObjectProperty(bstrSound, pSound );
	}
	else
	{
		if (FAILED(hr =  pSound->put_Is3D( VARIANT_FALSE )))
		    return hr;
		if (FAILED(hr =  pSound->put_IsLooping( bIsLooping.boolVal )))
		    return hr;
		if (FAILED(hr =  pSound->Set2DData(lVolume.lVal,lFrequency.lVal,lPan.lVal)))
		    return hr;
		hr =  pSound->put_SoundURL( bstrWaveURL );
	}
	
    return hr;
}

HRESULT CMultimediaExemplarObject::SetClient3DSound(BSTR bstrWaveURL,
											 VARIANT bIsLooping,
											 VARIANT dwInsideConeAngle,
											 VARIANT dwOutsideConeAngle,
											 VARIANT lConeOutsideVolume,
											 VARIANT flMinDistance,
											 VARIANT flMaxDistance)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	VARIANT_BOOL bServer = VARIANT_FALSE;

    if (SUCCEEDED(m_pWorld->get_ServerSide( &bServer )) &&
		(bServer == VARIANT_TRUE))
		return S_OK;

	CVWComPtr<IThing, NULL, &IID_IThing> pThis;
	CVWComPtr<ISound, NULL, &IID_ISound> pSound;
	CComVariant var;
    HRESULT hr = S_OK;

	static CComBSTR bstrSound("ClientSound");
	
	// Convert to proper types
	if (FAILED(hr = ::VariantChangeType( &bIsLooping,&bIsLooping,0,VT_BOOL)))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::SetClient3DSound: bad argument\n");
        return ReportMMError(E_INVALIDARG);
    }
	if (FAILED(hr = ::VariantChangeType( &dwInsideConeAngle,&dwInsideConeAngle,0,VT_I4)))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::SetClient3DSound: bad argument\n");
        return ReportMMError(E_INVALIDARG);
    }
	if (FAILED(hr = ::VariantChangeType( &dwOutsideConeAngle,&dwOutsideConeAngle,0,VT_I4)))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::SetClient3DSound: bad argument\n");
        return ReportMMError(E_INVALIDARG);
    }
	if (FAILED(hr = ::VariantChangeType( &lConeOutsideVolume,&lConeOutsideVolume,0,VT_I4)))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::SetClient3DSound: bad argument\n");
        return ReportMMError(E_INVALIDARG);
    }
	if (FAILED(hr = ::VariantChangeType( &flMinDistance,&flMinDistance,0,VT_R4)))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::SetClient3DSound: bad argument\n");
        return ReportMMError(E_INVALIDARG);
    }
	if (FAILED(hr = ::VariantChangeType( &flMaxDistance,&flMaxDistance,0,VT_R4)))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::SetClient3DSound: bad argument\n");
        return ReportMMError(E_INVALIDARG);
    }

	// Get this
	if (FAILED(hr = m_pWorld->get_This(&pThis.p)))
		return hr;
	//Get the sound property if it exists
	if (SUCCEEDED( pThis->get_Property( bstrSound, &var )))
	{
		pSound = var;
	}

	if (pSound == NULL )
	{
		if (FAILED(hr = CreateSound(bstrWaveURL, VARIANT_TRUE, bIsLooping.boolVal, &pSound.p)))
		    return hr;
		if (FAILED(hr =  pSound->Set3DData(dwInsideConeAngle.lVal,
									   dwOutsideConeAngle.lVal,
									   lConeOutsideVolume.lVal,
									   flMinDistance.fltVal,
									   flMaxDistance.fltVal)))
		    return hr;

		hr = pThis->put_ObjectProperty(bstrSound, pSound );
	}
	else
	{
		if (FAILED(hr =  pSound->put_Is3D( VARIANT_TRUE )))
		    return hr;
		if (FAILED(hr =  pSound->put_IsLooping( bIsLooping.boolVal )))
		    return hr;
		if (FAILED(hr =  pSound->Set3DData(dwInsideConeAngle.lVal,
									   dwOutsideConeAngle.lVal,
									   lConeOutsideVolume.lVal,
									   flMinDistance.fltVal,
									   flMaxDistance.fltVal)))
		    return hr;

		hr =  pSound->put_SoundURL( bstrWaveURL );
	}
	
    return hr;
}


HRESULT CMultimediaExemplarObject::SetClientSound(BSTR bstrWaveURL,VARIANT bIsLooping, VARIANT lVolume, VARIANT lFrequency,VARIANT lPan)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	VARIANT_BOOL bServer = VARIANT_FALSE;

    if (SUCCEEDED(m_pWorld->get_ServerSide( &bServer )) &&
		(bServer == VARIANT_TRUE))
		return S_OK;
	
	CVWComPtr<IThing, NULL, &IID_IThing> pThis;
	CVWComPtr<ISound, NULL, &IID_ISound> pSound;
	CComVariant var;
    HRESULT hr = S_OK;

	static CComBSTR bstrSound("ClientSound");

	// Convert to proper types
	if (FAILED(hr = ::VariantChangeType( &bIsLooping,&bIsLooping,0,VT_BOOL)))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::SetClientSound: bad argument\n");
        return ReportMMError(E_INVALIDARG);
    }
	if (FAILED(hr = ::VariantChangeType( &lVolume,&lVolume,0,VT_I4)))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::SetClientSound: bad argument\n");
        return ReportMMError(E_INVALIDARG);
    }
	if (FAILED(hr = ::VariantChangeType( &lFrequency,&lFrequency,0,VT_I4)))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::SetClientSound: bad argument\n");
        return ReportMMError(E_INVALIDARG);
    }
	if (FAILED(hr = ::VariantChangeType( &lPan,&lPan,0,VT_I4)))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::SetClientSound: bad argument\n");
        return ReportMMError(E_INVALIDARG);
    }

	// Get this
	if (FAILED(hr = m_pWorld->get_This(&pThis.p)))
		return hr;
	//Get the sound property if it exists
	if (SUCCEEDED( pThis->get_Property( bstrSound, &var )))
	{
		pSound = var;
	}

	if (pSound == NULL )
	{
		if (FAILED(hr =  CreateSound(bstrWaveURL, VARIANT_FALSE, bIsLooping.boolVal, &pSound.p )))
		    return hr;
		if (FAILED(hr =  pSound->Set2DData(lVolume.lVal,lFrequency.lVal,lPan.lVal)))
		    return hr;
		hr = pThis->put_ObjectProperty(bstrSound, pSound );
	}
	else
	{
		if (FAILED(hr =  pSound->put_Is3D( VARIANT_FALSE )))
		    return hr;
		if (FAILED(hr =  pSound->put_IsLooping( bIsLooping.boolVal )))
		    return hr;
		if (FAILED(hr =  pSound->Set2DData(lVolume.lVal,lFrequency.lVal,lPan.lVal)))
		    return hr;
		hr =  pSound->put_SoundURL( bstrWaveURL );
	}

	return hr;
}

//******************************************************
// Take
// Take should be allowed if:
//	object to be taken has the same container as object 
//	object is takeable - has IsTakeable == true OR
//  taker is object's owner OR 
//  taker is wizard
// If Take is allowed, position and orient appropriately

HRESULT CMultimediaExemplarObject::Take(IThing *pThing)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr = S_OK;
	CComPtr<IThing>		pThis;
	CComPtr<IThing>		pCaller;
	CComPtr<IThing>		pContainer;
	CComPtr<IThing>		pThingsContainer;
	CComPtr<IThing>		pOwner;
	CComPtr<IVector>	pvPos, pvDir;
	VARIANT_BOOL		bCheckIsTakeable = VARIANT_TRUE;  // assume you will have to do the check
	VARIANT_BOOL		bIsTakeable = VARIANT_FALSE;      // default is not takeable
	static CComBSTR		bstrPosition("Position");
	static CComBSTR		bstrDirection("Direction");

	if (pThing == NULL)
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::Take: NULL pointer\n");
        return ReportMMError(E_POINTER);
    }
    
	// Get the this
	hr = m_pWorld->get_This(&pThis);
	if (FAILED(hr))
		return hr;

	// Get caller
	if (FAILED(hr = m_pWorld->get_Caller(&pCaller)))
	    return hr;

	// Get the container
	hr = pThis->get_Container(&pContainer);
	if (FAILED(hr))
		return hr;

	// Get the container of the thing
	hr = pThing->get_Container(&pThingsContainer);
	if (FAILED(hr))
		return hr;

	// Get owner of the thing
	if (FAILED(hr = pThing->get_Owner(&pOwner)))
	    return hr;

#ifdef _DEBUG
	// No error checking here, these are for debug and we don't care if they fail
	CComBSTR bstrCallerName, bstrOwnerName;
	if (pCaller)

		pCaller->get_Name(&bstrCallerName.m_str);

	if (pOwner)
		pOwner->get_Name(&bstrOwnerName.m_str);

#endif

	// Can only take things in my container.
	if (pThingsContainer != pContainer)
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::Take: not in user's container\n");
        return ReportMMError(VWMM_E_NOTINCONTAINER);
    }

	// Get IsTakeable (shouldn't do the move if IsTakeable is false)
	if (FAILED(hr = pThing->get_BOOL(CComBSTR("IsTakeable"), &bIsTakeable)))
    	return hr;

	// If the object isn't takeable, check for special cases in which we 
	// want to override this property

	if ((bIsTakeable == VARIANT_FALSE) && (bCheckIsTakeable == VARIANT_TRUE))
	{	
		{
			// Is the caller the owner of the object?
			if (pOwner)
			{
				if (pCaller==pOwner)
					bCheckIsTakeable = VARIANT_FALSE;
			}
				
			// Now if caller != owner, check if wizard
			if (bCheckIsTakeable == VARIANT_TRUE)
			{
				VARIANT_BOOL bIsAvatar;
				VARIANT_BOOL bIsWizard = VARIANT_FALSE;

				if (FAILED(hr = pCaller->IsOfType(CComBSTR("Avatar"), &bIsAvatar)))
	                return hr;

				if (bIsAvatar)
				{
					if (FAILED(hr = pCaller->get_IsWizard(&bIsWizard)))
	                    return hr;

					if (bIsWizard == VARIANT_TRUE)
						bCheckIsTakeable = VARIANT_FALSE;
				}
			}
		}
	}

	// Call MoveInto if either the object is takeable or one of the other conditions is met
    if ((bIsTakeable == VARIANT_TRUE) || (bCheckIsTakeable == VARIANT_FALSE))
    {

		hr = pThing->InvokeMethodExt(CComBSTR("MoveIntoSync"),
            					CComDISPPARAMS(1, CComVariant((IDispatch*)pThis)),
			        			NULL);

		if (SUCCEEDED(hr))
		{
			// Position/orient the object appropriately.
			hr = pThing->get_ObjectProperty(bstrPosition, (IObjectProperty**)&pvPos);	
			if (FAILED(hr))
				return hr;
		
			hr = pThing->get_ObjectProperty(bstrDirection, (IObjectProperty**)&pvDir);
			if (FAILED(hr))
				return hr;

			if (pvPos==NULL || pvDir==NULL)
				return S_OK;
			
			pvPos->set(0.0, 0.0, 0.0);
			pvDir->set(0.0, 0.0, 1.0);

			// Create the Action event subject list and fire the Action event.
			CComPtr<IPropertyList> pList;
			
			hr = CreatePropertyList(m_pWorld, &pList);
			if (FAILED(hr))
				return ReportMMError(hr);

			hr = pList->AddThing(pThis);
			if (FAILED(hr))
				return hr;
			
			hr = pList->AddThing(pThing);
			if (FAILED(hr))
				return hr;
			
			CComBSTR bstrName("Take");
			CComBSTR bstrDescription("You take {object}.|{subject} takes {object}.");
			pThis->FireEventExt(CComBSTR("Action"),
								CComDISPPARAMS(3, CComVariant(bstrName),
												  CComVariant(bstrDescription),
												  CComVariant((IDispatch*) pList)),
								FIREEVENT_DEFAULT);
		}
		return hr;
	}

	else
		return ReportMMError(VWMM_E_TAKEPERMISSIONDENIED);
}

HRESULT CMultimediaExemplarObject::Drop(IThing *pThing)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IPropertyList> pList;
	CComPtr<IThing>		pThis, pContainer, pThingsContainer;
	CComPtr<IVWFrame>	pvwfThis, pvwfContainer;
	CComPtr<IVector>	pvThisPos, pvThisDir, pvPos, pvDir;
	float				fThisPosX, fThisPosY, fThisPosZ, fThisDirX, fThisDirY, fThisDirZ, fThisUpX, fThisUpY, fThisUpZ;
	static CComBSTR		bstrPosition("Position");
	static CComBSTR		bstrDirection("Direction");
    HRESULT             hr = S_OK;

	if (pThing == NULL)
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::Drop: NULL pointer\n");
        return ReportMMError(E_POINTER);
    }

	// Get the this
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	// Get the room
	if (FAILED(hr = pThis->get_Container(&pContainer)))
		return hr;

	// Get the container of the thing
	if (FAILED(hr = pThing->get_Container(&pThingsContainer)))
		return hr;

	// Can only drop things in my contents.
	if (pThingsContainer != pThis)
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::Drop: not in user's contents\n");
        return ReportMMError(VWMM_E_NOTINCONTENTS);
    }

	// Position/orient the object appropriately - relative to this.
	// Do this BEFORE dropping so things don't look bad immediately after drop.
	// Get this's position from frame if possible 'cause most up to date.
	if (FAILED(hr = _GetFrameHelper(pThis, &pvwfThis)) || !pvwfThis ||
		FAILED(hr = _GetFrameHelper(pContainer, &pvwfContainer)) || !pvwfContainer)
	{
		if (FAILED(hr = pThis->get_ObjectProperty(bstrPosition, (IObjectProperty**)&pvThisPos)))
			return hr;
		if (FAILED(hr = pvThisPos->get(&fThisPosX, &fThisPosY, &fThisPosZ)))
			return hr;
		if (FAILED(hr = pThis->get_ObjectProperty(bstrDirection, (IObjectProperty**)&pvThisDir)))
			return hr;
		if (FAILED(hr = pvThisDir->get(&fThisDirX, &fThisDirY, &fThisDirZ)))
			return hr;
	}
	else
	{
		if (FAILED(hr = pvwfThis->GetPositionEx(pvwfContainer, &fThisPosX, &fThisPosY, &fThisPosZ)))
			return hr;
		if (FAILED(hr = pvwfThis->GetOrientationEx(pvwfContainer, &fThisDirX, &fThisDirY, &fThisDirZ, &fThisUpX, &fThisUpY, &fThisUpZ)))
			return hr;
	}
	if (FAILED(hr = pThing->get_ObjectProperty(bstrPosition, (IObjectProperty**)&pvPos)) || !pvPos)
		return hr;
	if (FAILED(hr = pThing->get_ObjectProperty(bstrDirection, (IObjectProperty**)&pvDir)) || !pvDir)
		return hr;

    pvPos->set(fThisPosX+(4*fThisDirX), fThisPosY, fThisPosZ+(4*fThisDirZ));
	pvDir->set(-fThisDirX, 0.0, -fThisDirZ);

	// Do it - relying on MoveInto for security.
	if (FAILED(hr = pThing->InvokeMethodExt(CComBSTR("MoveIntoSync"),
                    						CComDISPPARAMS(1, CComVariant((IDispatch*)pContainer)),
					                    	NULL)))
        return hr;

	// Create the Action event subject list and fire the Action event.
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
		return hr;
	if (FAILED(hr = pList->AddThing(pThis)))
		return hr;
	if (FAILED(hr = pList->AddThing(pThing)))
		return hr;

    CComBSTR bstrName("Drop");
	CComBSTR bstrDescription("You drop {object}.|{subject} drops {object}.");
	pThis->FireEventExt(CComBSTR("Action"),
						CComDISPPARAMS(3, CComVariant(bstrName),
										  CComVariant(bstrDescription),
										  CComVariant((IDispatch*) pList)),
						FIREEVENT_DEFAULT);

	if (pvwfThis)
	{
		// Invoke Fall method for client-side graphics stuff.
		if (FAILED(hr = pThing->InvokeMethodExt(CComBSTR("Fall"), NULL, NULL)))
			return hr;
	}

    return hr;
}

HRESULT CMultimediaExemplarObject::Fall()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IVWAnimator>	pVWAnimator;
	CComPtr<IVWAnimation>	pAnimation;
	CComPtr<IThing>		pThis, pContainer, pGlobal;
	CComPtr<IVWFrame>	pvwf, pvwfContainer;
	CComPtr<IVector>	pvPos;
	CComVariant			varMinX, varMinY(0.0F), varMinZ, varMaxX, varMaxY, varMaxZ;
	float				fPosX, fNewPosY, fPosY, fPosZ, fFloorHeight, fScaleX, fScaleY, fScaleZ;
	long				nDuration, nLastDuration;
	VARIANT_BOOL		bGravity;
	static CComBSTR		bstrPosition("Position");
	static CComBSTR		bstrPositionDuration("PositionDuration");
	static CComBSTR		bstrGravity("Gravity");
	static CComBSTR		bstrAnimator("Animator");
	static CComBSTR		bstrDurationReset("DurationReset");
	static CComBSTR		bstrDurationResetAnim("DurationResetAnim");
    HRESULT             hr = S_OK;

	// Don't do anything unless gravity is on.
	if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
		return hr;
	if (FAILED(hr = pGlobal->get_BOOL(bstrGravity, &bGravity)) || !bGravity)
		return hr;

	// Get the this.
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	// Get the room
	if (FAILED(hr = pThis->get_Container(&pContainer)) || !pContainer)
		return hr;

	if (FAILED(hr = _GetFrameHelper(pThis, &pvwf)))
		return hr;
	if (FAILED(hr = _GetFrameHelper(pContainer, &pvwfContainer)))
		return hr;

	// If no graphics, do nothing. Thing::Drop has already done all it can.
	if (!pvwfContainer || !pvwf)
		return hr;

	// Get this's position.
	if (FAILED(hr = pvwf->GetPositionEx(pvwfContainer, &fPosX, &fPosY, &fPosZ)))
	    return hr;

	// Adjust the final position according to the gravity.
	if (FAILED(hr = pvwf->GetFloorHeight(pvwfContainer, &fFloorHeight)))
        return hr;
	
	// Only do anything if there's a floor.
	if (fFloorHeight == fPosY)
		return hr;

	// Correct for the coordinate offset of the bottom of the geometry
	if (FAILED(hr = pvwf->GetBoundingBox(&varMinX, &varMinY, &varMinZ,
											  &varMaxX, &varMaxY, &varMaxZ)))
		return hr;
	if (FAILED(hr = pvwf->GetScaleEx(NULL, &fScaleX, &fScaleY, &fScaleZ)))
		return hr;
	// Don't let the object fall quite to the ground 'cause then we won't know
	// the difference between being on the ground and there not being a floor.
	fNewPosY = fFloorHeight + (float)fabs(varMinY.fltVal)*fScaleY + fFallOffset;

	// Don't fall if we're already on the ground.
	if (FLOAT_EQUAL(fNewPosY, fPosY))
		return hr;

	// Store the current duration.
	if (FAILED(hr = pThis->get_Long(bstrPositionDuration, &nLastDuration)))
		return hr;

	// Calculate the proper time required to fall.
	nDuration = (long) (sqrt(2*fabs(fPosY-fNewPosY)/GRAVITY_CONST) * 1000.0f);
	if (FAILED(hr = pThis->put_Long(bstrPositionDuration, nDuration)))
		return hr;

	if (FAILED(hr = pThis->get_ObjectProperty(bstrPosition, (IObjectProperty**)&pvPos)) || !pvPos)
		return hr;
	pvPos->set(fPosX, fNewPosY, fPosZ);

	// Get the global animator tool we need from the world 
	if (FAILED(hr = m_pWorld->get_Tool(bstrAnimator, (IUnknown**)&pVWAnimator)) || !pVWAnimator)
		return hr;

	// Remove the old animation (if there is one).
 	if (FAILED(hr = pThis->get_Dispatch(bstrDurationResetAnim, (IDispatch**)&pAnimation)))
		return hr;
	if (pAnimation)
	{
		DISPPARAMS *pArgs = NULL;

		// Need to get the actual last duration from this animation.
		if (FAILED(hr = pAnimation->get_ArgumentsExt(&pArgs)))
			return hr;

		if (pArgs->cArgs != 2 || pArgs->rgvarg[0].vt != VT_I4)
			return E_INVALIDARG;
		nLastDuration = pArgs->rgvarg[0].lVal;

 		if (FAILED(hr = pVWAnimator->Remove(pAnimation)))
			return hr;
		pAnimation.Release();
        TerminateDISPPARAMS(VARIANT_FALSE, pArgs);
	}

	// Set up the animation to reset the PositionDuration.
	if (FAILED(hr = pVWAnimator->CreateExt(iDurationResetLag, pThis, bstrDurationReset, CComDISPPARAMS(2, CComVariant(bstrPosition), CComVariant(nLastDuration)), &pAnimation)))
		return hr;
	if (FAILED(hr = pThis->put_Dispatch(bstrDurationResetAnim, pAnimation)))
		return hr;

    return hr;
}

HRESULT CMultimediaExemplarObject::DurationReset(BSTR bstrProperty, long nLastDuration)
{
	HRESULT hr = S_OK;
	CComPtr<IThing>	pThis;
	CComPtr<IVWAnimation>	pAnimation;
	CComPtr<IVWAnimator>	pVWAnimator;
	static CComBSTR	bstrAnimator("Animator");
	static CComBSTR	bstrDurationResetAnim("DurationResetAnim");

	// Get the this.
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	// Reset the Duration property.
	if (FAILED(hr = pThis->put_Long(CComBSTR(CString(bstrProperty)+CString("Duration")), nLastDuration)))
		return hr;

	// Get the global animator tool we need from the world 
	if (FAILED(hr = m_pWorld->get_Tool(bstrAnimator, (IUnknown**)&pVWAnimator)) || !pVWAnimator)
		return hr;

	// Remove the animation.
 	if (FAILED(hr = pThis->get_Dispatch(bstrDurationResetAnim, (IDispatch**)&pAnimation)) || !pAnimation)
		return hr;
 	if (FAILED(hr = pVWAnimator->Remove(pAnimation)))
		return hr;
	hr = pThis->put_Dispatch(bstrDurationResetAnim, NULL);

	return hr;
}
	
///////////////////////////////////////////////////////////////////////
// Helpers

//******************************************************
// OrientAndScaleTransform
// Changes the actual orientation and scale of the object


HRESULT CMultimediaExemplarObject::OrientAndScaleTransform(IThing *pThing, IVWFrame *pvwfRef, IVWFrame *pvwfTransform)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    HRESULT					hr = S_OK;
	CComVariant				varRate;
	CComPtr<IPropertyList>	pBoundaryList;
	CComPtr<IVector>		pvPos, pvDir, pvUp, pvScale, pRotAxis;
	CComVariant				varMinX, varMinY, varMinZ, varMaxX, varMaxY, varMaxZ;
	float					fPosX, fPosY, fPosZ, fDirX, fDirY, fDirZ, fUpX, fUpY, fUpZ,
							fScaleX, fScaleY, fScaleZ, fRate;

	CComPtr<IThing>			pThis;

	static CComBSTR	bstrPosition("Position");
	static CComBSTR	bstrDirection("Direction");
	static CComBSTR	bstrUp("Up");
	static CComBSTR	bstrScale("Scale");
	static CComBSTR bstrRotationRate("RotationRate");
	static CComBSTR bstrRotationAxis("RotationAxis");
	static CComBSTR bstrPositionStart("PositionStart");
	static CComBSTR bstrDirectionStart("DirectionStart");
	static CComBSTR bstrUpStart("UpStart");
	static CComBSTR bstrScaleStart("ScaleStart");

    if ((pThing==NULL) || (pvwfTransform==NULL))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::OrientAndScaleTransform: NULL pointer\n");
        return ReportMMError(E_POINTER);
    }

	long lSystemTime = CurrentTime();
	COleVariant v;

	if (FAILED(hr = pThing->get_ObjectProperty(bstrPosition, (IObjectProperty**)&pvPos)) || !pvPos)
		return hr;
	if (FAILED(hr = pThing->get_ObjectProperty(bstrDirection, (IObjectProperty**)&pvDir)) || !pvDir)
		return hr;
	if (FAILED(hr = pThing->get_ObjectProperty(bstrUp, (IObjectProperty**)&pvUp)) || !pvUp)
		return hr;
	if (FAILED(hr = pThing->get_ObjectProperty(bstrScale, (IObjectProperty**)&pvScale)) || !pvScale)
		return hr;
	pvPos->get(&fPosX, &fPosY, &fPosZ);
	pvDir->get(&fDirX, &fDirY, &fDirZ);
	pvUp->get(&fUpX, &fUpY, &fUpZ);
	pvScale->get(&fScaleX, &fScaleY, &fScaleZ);

	if (FAILED(hr = pvwfTransform->SetOrientationEx(pvwfRef, fDirX, fDirY, fDirZ, fUpX, fUpY, fUpZ)))
		return hr;
	if (FAILED(hr = pvwfTransform->SetScaleEx(pvwfRef, fScaleX, fScaleY, fScaleZ)))
		return hr;
	if (FAILED(hr = pvwfTransform->SetPositionEx(pvwfRef, fPosX, fPosY, fPosZ)))
		return hr;

	if (FAILED(hr = pThing->get_Property(bstrRotationRate, &varRate)))
		return hr;
	
	if (varRate.vt == VT_R4 || varRate.vt == VT_R8 || varRate.fltVal != 0.0f)
	{
		if (varRate.vt == VT_R8)
			fRate = (float) varRate.dblVal;
		else if (varRate.vt == VT_R4)
			fRate = varRate.fltVal;

		if (FAILED(hr = pThing->get_ObjectProperty(bstrRotationAxis, (IObjectProperty**) &pRotAxis)))
    		return hr;
		if (pRotAxis != NULL)
		{
			if (FAILED(hr = pRotAxis->get(&fUpX, &fUpY, &fUpZ)))
	        	return hr;
			if (FAILED(hr = pvwfTransform->SetRotationRate(pvwfRef, fUpX, fUpY, fUpZ, fRate)))
		        return hr;
		}   
	}				

	if (FAILED(hr = pvwfTransform->GetBoundingBox( &varMinX, &varMinY, &varMinZ, &varMaxX, &varMaxY, &varMaxZ )))
		return hr;

	if (FAILED(hr = pThing->InvokeMethodExt(bstrUpdateSolidBoundary, CComDISPPARAMS(6, varMinX, varMinY, varMinZ, varMaxX, varMaxY, varMaxZ), NULL)))
		return hr;

	RecoverAnimation(CComBSTR("Position"),pThing);
	RecoverAnimation(CComBSTR("Direction"),pThing);
	RecoverAnimation(CComBSTR("Up"),pThing);
	RecoverAnimation(CComBSTR("Scale"),pThing);
	RecoverAnimation(CComBSTR("LightColor"),pThing);
	RecoverAnimation(CComBSTR("LightAttenuation"),pThing);
	RecoverAnimation(CComBSTR("File"),pThing);

	return hr;
}


STDMETHODIMP CMultimediaExemplarObject::RecoverAnimation(BSTR bstrPropertyName, IThing* pThis)
{
	//check position, up, direction, scale, file, lightcolor, lightattenuation on the thing

	HRESULT			hr = S_OK;
	long			lStartTime, lOptions, lSystemTime = 0l;
	unsigned long	ulEndTime;
	float			fTimeScale, fStartRange, fEndRange, fIndex;
	CComBSTR		bstrPropertyAnimProperties(bstrPropertyName);
	CComBSTR		bstrPropertyAnimOptions(bstrPropertyName);
	CComBSTR		bstrPropertyAnimStartTime(bstrPropertyName);
	CComBSTR		bstrPropertyAnimIndex(bstrPropertyName);
	CComBSTR		bstrPropertyAnimTimeScale(bstrPropertyName);
	CComBSTR		bstrPropertyAnimRangeStart(bstrPropertyName);
	CComBSTR		bstrPropertyAnimRangeEnd(bstrPropertyName);
	CComBSTR		bstrPropertyAnimCallBack(bstrPropertyName);
	CComBSTR		bstrPropertyAnimPath(bstrPropertyName);
	CComBSTR		bstrCallBack;
	static CString			strFile("File");
	static CString			strPosition("Position");
	static CString			strDirection("Direction");
	static CString			strUp("Up");
	static CString			strScale("Scale");
	static CString			strLightColor("LightColor");
	static CString			strLightAttenuation("LightAttenuation");

	CString			strPropertyName(bstrPropertyName);
	CComVariant		var, var2, varPathMap;
	CComPtr<IPropertyMap>	pAnimPropertiesMap, pPathMap;
	CComPtr<IPropertyList> pFileKeyList;
	long lCount=0l;

	bstrPropertyAnimProperties.Append("AnimProperties");
	bstrPropertyAnimOptions.Append("AnimOptions");
	bstrPropertyAnimStartTime.Append("AnimStartTime");
	bstrPropertyAnimIndex.Append("AnimIndex");
	bstrPropertyAnimTimeScale.Append("AnimTimeScale");
	bstrPropertyAnimRangeStart.Append("AnimRangeStart");
	bstrPropertyAnimRangeEnd.Append("AnimRangeEnd");
	bstrPropertyAnimCallBack.Append ("AnimCallBack");
	bstrPropertyAnimPath.Append("AnimPath");

  	if (FAILED(hr = pThis->get_Property(bstrPropertyAnimProperties, &var)))
		return S_OK;

	if (var.vt != VT_DISPATCH)
	{ 
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}	
	if(var.pdispVal)
	{
		if (SUCCEEDED(hr = var.pdispVal->QueryInterface(IID_IPropertyMap,(void**)&pAnimPropertiesMap)))
		{	
			var.Clear();
			if (FAILED(hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimStartTime, &var) ))
				goto ERROR_ENCOUNTERED;
			lStartTime = CastToLong(var);
			var.Clear();
			if (FAILED(hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimOptions, &var) ))
				goto ERROR_ENCOUNTERED;
			lOptions = CastToLong(var);
			var.Clear();
			if (FAILED(hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimTimeScale, &var) ))
				goto ERROR_ENCOUNTERED;
			fTimeScale = CastToFloat(var);
			var.Clear();
			if (FAILED(hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimRangeStart, &var) ))
				goto ERROR_ENCOUNTERED;
			fStartRange = CastToFloat(var);
			var.Clear();
			if (FAILED(hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimRangeEnd, &var) ))
				goto ERROR_ENCOUNTERED;
			fEndRange = CastToFloat(var);
			var.Clear();
			if (FAILED(hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimIndex, &var) ))
				goto ERROR_ENCOUNTERED;
			fIndex = CastToFloat(var);
			var.Clear();
			if (FAILED(hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimCallBack, &var) ))
				goto ERROR_ENCOUNTERED;
			bstrCallBack = var.bstrVal;
			var.Clear();
		}
	}
	//if this is not a file animation, get the path from the property of the corresponding name
	if (strPropertyName.CompareNoCase(strFile)!=0)
	{
		if (FAILED(hr = pThis->get_Property(bstrPropertyAnimPath, &varPathMap)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = varPathMap.pdispVal->QueryInterface(IID_IPropertyMap,(void**)&pPathMap)))
			goto ERROR_ENCOUNTERED;
	}
	if (lStartTime > 0)
	{
		
		ULongAdd((unsigned long)lStartTime, ((unsigned long)(((fIndex)>(fStartRange)?(fEndRange-fIndex):(fEndRange-fStartRange))/fTimeScale)), &ulEndTime);

		if (((unsigned long)CurrentTime() < ulEndTime) || (lOptions & 2))
		{
			//calculate a good index from the elapsed time and timescale
	 		
			unsigned long ulDuration;

			if ((unsigned long)CurrentTime() < ulEndTime)
			{
				ULongSubtract ( ulEndTime, (unsigned long)CurrentTime(), &ulDuration);	
				fIndex = ( (   ((fEndRange - fStartRange)/fTimeScale) - (float)ulDuration ) * (fEndRange - fStartRange) ) / ((fEndRange - fStartRange)/fTimeScale);
			}
			else 
			{   
				if (fEndRange == fStartRange) //range values were never extrapolated-- 
					//it is likely this is a file animation with bad range settings
				{   
					if ((strPropertyName.CompareNoCase(strFile)==0) && (fEndRange < 0))
					{
						//a file animation was started with no ranges,
						//so we dont know what the end range is, we need to create it.
						fEndRange = 0.0f;
						fStartRange = 0.0f;
						if (FAILED(hr = GetAnimationFileKeys(&pFileKeyList)))
							return hr;
						pFileKeyList->get_Count(&lCount);
						if (lCount > 0)
						{
							if (FAILED(hr = pFileKeyList->get_Property((lCount - 1), &var2)))
								return hr;
							fEndRange = CastToFloat(var2);
						}
					}
				}
				ULongSubtract ( (unsigned long)CurrentTime(), ulEndTime, &ulDuration);
				ulDuration = (ulDuration)%((unsigned long)((fEndRange - fStartRange)/fTimeScale)); 
				fIndex = ((float)ulDuration * (fEndRange - fStartRange)) / ((fEndRange - fStartRange)/fTimeScale);
			}
#ifdef _DEBUG 
			int iIndex = (int)fIndex;
			char bufferr[30];
			CString strReport = itoa(iIndex, bufferr, 10);
	        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, strReport);
			VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "\n");
#endif
			if (!(IsMethodThisSide(pThis, bstrCallBack)))
				bstrCallBack = ("");

	
			char					buff[200];
			CComBSTR				bstrInterpName;
			CComPtr<IPropertyList>	pArgList;			
//			char					szAnimName[256];
			DWORD					dwSize = 256;
			//CComPtr<IDirect3DRMAnimationSet2> pAnimationSet;

			_ltoa((long)(void*)pThis,buff,16);
			bstrInterpName = buff;
			bstrInterpName.Append(bstrPropertyName);

			//pack up the args into a prop list, put bstrInterpName in slot 0 and index in slot 1
			//fire the UI event
			if (FAILED(hr = CreatePropertyList(m_pWorld, &pArgList)))
				goto ERROR_ENCOUNTERED;
			if (FAILED(hr = pArgList->AddString(bstrInterpName)))
				goto ERROR_ENCOUNTERED;

			var.Clear();
			var.vt = VT_R4;
			var.fltVal = fIndex;
			if (FAILED(hr = pArgList->Add(var)))
				goto ERROR_ENCOUNTERED;

			lSystemTime = GetTickCount();

			if (strPropertyName.CompareNoCase(strPosition)==0)
			{
				hr = StartPositionPathAnimation(pPathMap, fTimeScale, fStartRange, fEndRange, fIndex, bstrCallBack ,lOptions, pThis,lSystemTime);
			}
			else if (strPropertyName.CompareNoCase(strDirection)==0)
			{
				hr = StartDirectionPathAnimation(pPathMap, fTimeScale, fStartRange, fEndRange, fIndex, bstrCallBack ,lOptions, pThis,lSystemTime);
			}
			else if (strPropertyName.CompareNoCase(strUp)==0)
			{
				hr = StartUpPathAnimation(pPathMap,fTimeScale, fStartRange, fEndRange, fIndex, bstrCallBack ,lOptions, pThis,lSystemTime);
			}
			else if (strPropertyName.CompareNoCase(strScale)==0)
			{
				hr = StartScalePathAnimation(pPathMap,fTimeScale, fStartRange, fEndRange, fIndex, bstrCallBack ,lOptions, pThis,lSystemTime);
			}
			else if (strPropertyName.CompareNoCase(strLightColor)==0)
			{
				hr = StartLightPropertyPathAnimation((long)0, pPathMap,  fTimeScale, fStartRange, fEndRange, fIndex, bstrCallBack ,lOptions, pThis,lSystemTime);
			}
			else if (strPropertyName.CompareNoCase(strLightAttenuation)==0)
			{
				hr = StartLightPropertyPathAnimation((long)1, pPathMap,  fTimeScale, fStartRange, fEndRange, fIndex, bstrCallBack ,lOptions, pThis,lSystemTime);	
			}
			else if (strPropertyName.CompareNoCase(strFile)==0)
			{
				hr = StartFileAnimation(fTimeScale, fStartRange, fEndRange, fIndex, bstrCallBack ,lOptions, pThis,lSystemTime);	
			}

		}
	}
	else if (strPropertyName.CompareNoCase(strFile)==0)
	{

		if (fIndex >= 0.0f)
		{
			//should set the file index on the frame of this object even if it has stopped

			CComPtr<IVWFrame>		pvwfGeometry, pvwfTransform;
			CComPtr<IVWGeometry>	pVWGeometry;
			static CComBSTR			bstrGeometry("Geometry");
			CComPtr<IDirect3DRMAnimationSet2> pAnimationSet;

  			if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)))
				goto ERROR_ENCOUNTERED;
			if (!pvwfTransform)
			{
				hr = E_FAIL;
				goto ERROR_ENCOUNTERED;
			}
			if (FAILED(hr = pThis->get_ObjectPropertyExt(bstrGeometry, IID_IVWGeometry, (IObjectProperty**)&pVWGeometry)))
				goto ERROR_ENCOUNTERED;
			if (pVWGeometry==NULL)
			{
				hr = E_FAIL;
				goto ERROR_ENCOUNTERED;
			}
			if (FAILED(hr = pVWGeometry->get_GeometryFrame(&pvwfGeometry)))
				goto ERROR_ENCOUNTERED;
			if (pvwfGeometry==NULL)
			{
				hr = E_FAIL;
				goto ERROR_ENCOUNTERED;
			}
			if (FAILED(hr = pvwfGeometry->get_AnimationSet(&pAnimationSet)))
				goto ERROR_ENCOUNTERED;
			if (pAnimationSet)
				pAnimationSet->SetTime(fIndex);
		}

	}

ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::RecoverAnimation Failed\n");
		return ReportMMError(hr);
	}
	return hr;
}


HRESULT CMultimediaExemplarObject::RemoveContentsFromGeometry(IThing *pThing, IVWFrame *pVWFrame)
{
	COleVariant				var;
	CComPtr<IVWFrame>		pvwfContent;
	CComPtr<IThing>			pContent;
	CComPtr<IPropertyList>	pContents;
	VARIANT_BOOL			bStub;
	long					lContents;
	int						i;
	BSTR			bstrName;

    if ((pThing==NULL) || (pVWFrame==NULL))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::RemoveContentsFromGeometry: NULL pointer\n");
        return ReportMMError(E_POINTER);
    }

	// Get the thing's contents.
	if (FAILED(pThing->get_Contents(&pContents)))
		return S_OK;
	pContents->get_Count(&lContents);

	// Cycle through all contents getting their frame's and deleting them from the 
	// thing's frame.
	for (i = 0; i < lContents; i++)
	{
		if (FAILED(pContents->get_Property(i, &var)) ||
			var.vt != VT_DISPATCH || var.pdispVal == NULL)
		{
			pThing->get_Name(&bstrName);
			m_pWorld->Report(CComBSTR(CString("ThingExemplar: Failed to obtain some contents of thing: ") + CString(bstrName)), -1);
			SAFEFREESTRING(bstrName);
			var.Clear();
			continue;
		}

		pContent = (IThing*)var.pdispVal;

		if (FAILED(pContent->get_IsStub(&bStub)) || bStub == VARIANT_TRUE)
		{
			pThing->get_Name(&bstrName);
			m_pWorld->Report(CComBSTR(CString("ThingExemplar: Failed to obtain some contents of thing: ") + CString(bstrName)), -1);
			SAFEFREESTRING(bstrName);
			var.Clear();
			continue;
		}

		if (FAILED(_GetFrameHelper(pContent, &pvwfContent)) || !pvwfContent)
		{
			pContent->get_Name(&bstrName);
			SAFEFREESTRING(bstrName);
			var.Clear();
			continue;
		}

		if (FAILED(pVWFrame->DeleteChild(pvwfContent)))
		{
			pContent->get_Name(&bstrName);
			m_pWorld->Report(CComBSTR(CString("ThingExemplar: Failed to break link in geometry hierarchy with thing: ") + CString(bstrName)), -1);
			SAFEFREESTRING(bstrName);
		}

		pContent.Release();
		pvwfContent.Release();

		var.Clear();
	}

    return S_OK;
}

HRESULT CMultimediaExemplarObject::AddContentsToGeometry(IThing *pThing, IVWFrame *pVWFrame)
{
	COleVariant				var, var2;
	CComPtr<IVWFrame>		pvwfContent;
	CComPtr<IThing>			pContent;
	CComPtr<IPropertyList>	pContents;
	VARIANT_BOOL			bStub;
	long					lContents;
	int						i;
	BSTR			bstrName;
	static CComBSTR	bstrComposeScene("ComposeScene");
	CComDISPPARAMS		paramsCompose;

    if ((pThing==NULL) || (pVWFrame==NULL))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::AddContentsToGeometry: NULL pointer\n");
        return ReportMMError(E_POINTER);
    }

	// Get the thing's contents.
	if (FAILED(pThing->get_Contents(&pContents)))
		return S_OK;
	pContents->get_Count(&lContents);

	// Cycle through all contents calling their ComposeScene methods and adding each
	// returned VWFrame to this thing's VWFrame, scaling positioning and orientating
	// it appropriately.
	// Any failures in this loop should not bring everything to a halt, we should
	// continue getting the geometry for all other children so we get as much geometry
	// loaded as possible.

	// Prepare to package up args for ComposeScene into DISPPARAMS struct.
	CComVariant varThing((IDispatch*)pThing);
	paramsCompose.Initialize(1, varThing);

	for (i = 0; i < lContents; i++)
	{
		if (FAILED(pContents->get_Property(i, &var)) ||
			var.vt != VT_DISPATCH || var.pdispVal == NULL)
		{
			pThing->get_Name(&bstrName);
			m_pWorld->Report(CComBSTR(CString("ThingExemplar: Failed to obtain some contents of thing: ") + CString(bstrName)), -1);
			SAFEFREESTRING(bstrName);
			var.Clear();
			continue;
		}

		pContent = (IThing*)var.pdispVal;

		if (FAILED(pContent->get_IsStub(&bStub)) || bStub == VARIANT_TRUE)
		{
			pThing->get_Name(&bstrName);
			m_pWorld->Report(CComBSTR(CString("ThingExemplar: Failed to obtain some contents of thing: ") + CString(bstrName)), -1);
			SAFEFREESTRING(bstrName);
			var.Clear();
			continue;
		}

		if (FAILED(pContent->InvokeMethodExt(bstrComposeScene, paramsCompose, &var2)) ||
			var2.vt != VT_DISPATCH || var2.pdispVal == NULL)
		{
			pContent->get_Name(&bstrName);
			m_pWorld->Report(CComBSTR(CString("ThingExemplar: Failed invoking ComposeScene method on thing: ") + CString(bstrName)), -1);
			SAFEFREESTRING(bstrName);
			var.Clear();
			var2.Clear();
			continue;
		}

		pvwfContent = (IVWFrame*)var2.pdispVal;

		pContent->get_Name(&bstrName);
		SAFEFREESTRING(bstrName);
		if (FAILED(pVWFrame->AddChild(pvwfContent)) ||
			FAILED(OrientAndScaleTransform(pContent, pVWFrame, pvwfContent)))
		{
			pContent->get_Name(&bstrName);
			m_pWorld->Report(CComBSTR(CString("ThingExemplar: Failed to make link in geometry hierarchy with thing: ") + CString(bstrName)), -1);
			SAFEFREESTRING(bstrName);
		}
		UpdateLabel(pContent);

		pContent.Release();
		pvwfContent.Release();

		var.Clear();
		var2.Clear();
	}

    return S_OK;
}

HRESULT CMultimediaExemplarObject::RemoveInterpolateAnim(IThing *pThing, BSTR bstrProperty)
{
	CComPtr<IVWAnimation>	pAnimation;
	CComPtr<IVWAnimator>	pVWAnimator;
	HRESULT					hr = S_OK;

	static CComBSTR	bstrAnimator("Animator");
	CComBSTR bstrAnim(CString(bstrProperty) + CString("Anim"));

    if (pThing==NULL)
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::RemoveInterpolateAnim: NULL pointer\n");
        return ReportMMError(E_POINTER);
    }

	if (FAILED(hr = m_pWorld->get_Tool(bstrAnimator, (IUnknown**)&pVWAnimator)) || !pVWAnimator)
		return hr;
 	if (FAILED(hr = pThing->get_Dispatch(bstrAnim, (IDispatch**)&pAnimation)) || !pAnimation)
	{
		if (hr == VWOBJECT_E_PROPERTYNOTEXIST)
		{
			hr = S_OK;
 			if (FAILED(hr = pThing->AddPropertyExt(	bstrAnim, 
													CComVariant((IDispatch *)NULL), 
													PSBIT_NOTLOGGED | PSBIT_NOTREMOTED | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
													PS_EXEMPLARDEFAULTPROPERTY,
													VT_DISPATCH, IID_IDispatch, NULL)))
				return hr;
		}
		return hr;
	}
 	if (FAILED(hr = pVWAnimator->Remove(pAnimation)))
		return hr;
	hr = pThing->put_Dispatch(bstrAnim, NULL);

	return hr;
}

HRESULT CMultimediaExemplarObject::PutInterpolateAnim(IThing *pThing, BSTR bstrProperty, IVWAnimation *pAnimation)
{
	HRESULT hr = S_OK;

	CComBSTR bstrAnim(CString(bstrProperty) + CString("Anim"));

    if (pThing==NULL)
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::PutInterpolateAnim: NULL pointer\n");
        return ReportMMError(E_POINTER);
    }

	hr = pThing->put_Dispatch(bstrAnim, pAnimation);

	return hr;
}

VARIANT_BOOL CMultimediaExemplarObject::IsStateMethodThisSide(IThing *pThing, BSTR bstrMethodName)
{
	CComPtr<IPropertyList>	pStateMachines;
	long					lStateMachines, l;
	CComBSTR				bstrStateMachine;
	CComVariant				varReturnVal;

	static CComBSTR bstrGetCurrentStateConcatName("GetCurrentStateConcatName");

    if (pThing==NULL)
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::IsStateMethodThisSide: NULL pointer\n");
        return VARIANT_FALSE;
    }

	if (SUCCEEDED(pThing->get_ObjectProperty(CComBSTR("StateMachines"), (IObjectProperty**)&pStateMachines)))
	{
		pStateMachines->get_Count(&lStateMachines);
		for (l = 0; l < lStateMachines; l++)
		{
			if (FAILED(pStateMachines->get_String(l, &bstrStateMachine.m_str)) ||
				FAILED(pThing->InvokeMethodExt(bstrGetCurrentStateConcatName,
					   CComDISPPARAMS(2, CComVariant(bstrStateMachine), CComVariant(bstrMethodName)),
					   &varReturnVal)) ||
				varReturnVal.vt != VT_BSTR)
				continue;
			if (IsMethodThisSide(pThing, CComBSTR(varReturnVal.bstrVal)))
				return VARIANT_TRUE;
			bstrStateMachine.Empty();
			varReturnVal.Clear();
		}
	}

	return VARIANT_FALSE;
}

VARIANT_BOOL CMultimediaExemplarObject::IsMethodThisSide(IThing *pThing, BSTR bstrMethodName)
{
	CComPtr<IMethod>		pMethod;
	VARIANT_BOOL			bMethodValid, bServerSide;
	long					lFlags = 0;
    HRESULT                 hr = S_OK;

    if (pThing==NULL)
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::IsMethodThisSide: NULL pointer\n");
        return VARIANT_FALSE;
    }

	if (FAILED(hr = pThing->IsValidMethod(bstrMethodName, &bMethodValid)))
		return VARIANT_FALSE;
	if (bMethodValid)
    {
		if (FAILED(hr = pThing->get_Method(bstrMethodName, &pMethod)))
			return VARIANT_FALSE;
    }
	if (pMethod)
    {
		if (FAILED(hr = pMethod->get_Flags(&lFlags)))
			return VARIANT_FALSE;
    }
	if (FAILED(hr = m_pWorld->get_ServerSide(&bServerSide)))
		return VARIANT_FALSE;
	if ((lFlags & METHOD_CLIENT && !bServerSide) || 
		(lFlags & METHOD_SERVER && bServerSide))
		return VARIANT_TRUE;

	return VARIANT_FALSE;
}

HRESULT	_GetFrameHelper(IThing *pThing, IVWFrame **ppvwf)
{
	CComVariant	varReturnVal;
	HRESULT hr = S_OK;

	if ((pThing == NULL) || (ppvwf==NULL))
		return E_POINTER;

	*ppvwf = NULL;
	
	if (FAILED(hr = pThing->InvokeMethodExt(CComBSTR("GetFrame"), NULL, &varReturnVal)))
		return hr;

	if (varReturnVal.vt != VT_DISPATCH || !varReturnVal.pdispVal)
		return S_OK;

	*ppvwf = (IVWFrame*)varReturnVal.pdispVal;
	SAFEADDREF(varReturnVal.pdispVal);

	return hr;
}

float CastToFloat(VARIANT var)
{
	switch (var.vt)
	{
		case VT_I2:
			return (float)var.iVal;
		case VT_I4:
			return (float)var.lVal;
		case VT_R4:
			return var.fltVal;
		case VT_R8:
			return (float)var.dblVal;
		default:
			return 1.0F;
	}
}

long CastToLong(VARIANT var)
{
	switch (var.vt)
		{
			case VT_I2:
				return(long)var.iVal;
			case VT_I4:
				return var.lVal;
			case VT_R4:
				return (long)var.fltVal;
			case VT_R8:
				return (long)var.dblVal;
			default:
				return 1l;
	}
}

VARIANT_BOOL IsNumeric(VARIANT var)
{
	switch (var.vt)
		{
			case VT_I2:
				return VARIANT_TRUE;
			case VT_I4:
				return VARIANT_TRUE;
			case VT_R4:
				return VARIANT_TRUE;
			case VT_R8:
				return VARIANT_TRUE;
			default:
				return VARIANT_FALSE;
		}
}
//******************************************************
// OnRotationRateChanged

STDMETHODIMP CMultimediaExemplarObject::OnRotationRateChanged(long lHint, VARIANT varHintData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	static CComBSTR bstrRotationRate("RotationRate");
	static CComBSTR bstrRotationAxis("RotationAxis");

    HRESULT hr = S_OK;

	// only listen to put notifications
	if (lHint == thPut || lHint == thAdd)
	{
		CComPtr<IThing>			pThis;
		CComPtr<IVWFrame>		pvwfTransform, pvwfParent;
		CComPtr<IVector>		pVector;
		float					fX, fY, fZ, fRate;
		CComVariant				varRate;

		if (FAILED(hr = m_pWorld->get_This(&pThis)))
    		return hr;
		if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)))
        {
		    VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::OnRotationRateChanged: Unable to get the frame\n");
            return ReportMMError(hr);
        }
		
		if (FAILED(hr = pThis->get_Property(bstrRotationRate, &varRate)))
		    return hr;

		if (pvwfTransform.p && (varRate.vt == VT_R4 || varRate.vt == VT_R8) )
		{
			if (varRate.vt == VT_R8)
				fRate = (float) varRate.dblVal;
			else if (varRate.vt == VT_R4)
				fRate = varRate.fltVal;

			if (FAILED(hr = pThis->get_ObjectProperty(bstrRotationAxis, (IObjectProperty**) &pVector)))
		        return hr;
			if (pVector == NULL)
			{
				if (FAILED(hr = CreateVector(0.0f,1.0f,0.0f, &pVector)))
		            return hr;
				if (FAILED(hr = pThis->put_ObjectProperty(bstrRotationAxis, pVector)))
		            return hr;
			}

			if (FAILED(hr = pVector->get(&fX, &fY, &fZ)))
		        return hr;
			if (FAILED(hr = pvwfTransform->GetParent(&pvwfParent)))
		        return hr;
			if (FAILED(hr = pvwfTransform->SetRotationRate(pvwfParent, fX, fY, fZ, fRate)))
		        return hr;
		}
	}

    return hr;
}

//******************************************************
// OnRotationRateChanged

STDMETHODIMP CMultimediaExemplarObject::OnRotationAxisChanged(long lHint, VARIANT varHintData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	static CComBSTR bstrRotationRate("RotationRate");
	static CComBSTR bstrRotationAxis("RotationAxis");

    HRESULT hr = S_OK;

	// only listen to put notifications
	if (lHint == thPut || lHint == thAdd)
	{
		CComPtr<IThing>			pThis;
		CComPtr<IVWFrame>		pvwfTransform, pvwfParent;
		CComPtr<IVector>		pVector;
		float					fX, fY, fZ, fRate;
		CComVariant				varRate;

		if (FAILED(hr = m_pWorld->get_This(&pThis)))
		    return hr;
		if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)))
        {
		    VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::OnRotationAxisChanged: Unable to get the frame\n");
            return ReportMMError(hr);
        }
		
		if (FAILED(hr = pThis->get_Property(bstrRotationRate, &varRate)))
		    return hr;

		if (pvwfTransform.p && (varRate.vt == VT_R4 || varRate.vt == VT_R8) )
		{
			if (varRate.vt == VT_R8)
				fRate = (float) varRate.dblVal;
			else if (varRate.vt == VT_R4)
				fRate = varRate.fltVal;

			if (FAILED(hr = pThis->get_ObjectProperty(bstrRotationAxis, (IObjectProperty**) &pVector)))
		        return hr;
			if (pVector == NULL)
				return S_OK;

			if (FAILED(hr = pVector->get(&fX, &fY, &fZ)))
		        return hr;
			
			if (FAILED(hr = pvwfTransform->GetParent(&pvwfParent)))
		        return hr;
			if (FAILED(hr = pvwfTransform->SetRotationRate(pvwfParent, fX, fY, fZ, fRate)))
		        return hr;
		}
	}

	return hr;
}

//******************************************************
// OnRenderQualityChanged

STDMETHODIMP CMultimediaExemplarObject::OnRenderQualityChanged(long lHint, VARIANT varHintData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	static CComBSTR bstrRenderQuality("RenderQuality");

    HRESULT hr = S_OK;

	// only listen to put notifications
	if (lHint == thPut || lHint == thAdd)
	{
		CComPtr<IThing>			pThis;
		CComPtr<IVWFrame>		pvwfTransform;
		short					sRenderQuality;

		if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    	return hr;
		if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)))
        {
		    VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::OnRenderQualityChanged: Unable to get the frame\n");
            return ReportMMError(hr);
        }
		
		if (pvwfTransform.p)
		{
			if (FAILED(hr = pThis->get_Short(bstrRenderQuality, &sRenderQuality)))
			    return hr;

			if (FAILED(hr = pvwfTransform->SetMeshQuality(VWFRAME_ALLHIERARCHY, sRenderQuality)))
			    return hr;
		}
	}

	return hr;
}

//********************************************************
// OnPosDirScaleAccess
// Checks policy before setting of position
// Setting is allowed if:
//  the caller is a wizard
//  the caller is the owner of the object
//	the caller is the container an object 
//	the caller is a portal (we're assuming users aren't malicious for the core drop)
//  the object is moveable - has IsMoveable set to true

STDMETHODIMP CMultimediaExemplarObject::OnPosDirScaleAccess(long lPerms, VARIANT_BOOL* bReturnValue)
{
	HRESULT hr =	S_OK;
	CComPtr<IThing> pThis;
	CComPtr<IThing> pCaller;
	CComPtr<IThing> pUser;
	CComPtr<IThing> pContainer;
	VARIANT_BOOL bOk = VARIANT_FALSE;
	VARIANT_BOOL bIsMoveable = VARIANT_FALSE;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// If one of the checks later succeeds, it will then be set to true
	if (bReturnValue == NULL)
    {
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::OnPosDirScaleAccess: Unable to get the frame\n");
        return ReportMMError(hr);
    }

	// If you have a pointer, set return value false by default
	if (lPerms != PS_WRITE)
	{
		*bReturnValue = VARIANT_TRUE;
		return S_OK;
	}
	else
		*bReturnValue = VARIANT_FALSE;

	// Get this
	hr = m_pWorld->get_This(&pThis);
	if (FAILED(hr))
		return hr;
	
	// Get the caller
	hr = m_pWorld->get_Caller(&pCaller);
	if (FAILED(hr))
		return hr;

	// Get the container
	hr = pThis->get_Container(&pContainer);
	if (FAILED(hr))
		return hr;

#ifdef _DEBUG
	CComBSTR bstrThis("<none>"), bstrCaller("<none>"), bstrContainer("<none>");

	if (pThis)
		pThis->get_Name(&bstrThis.m_str);
	if (pCaller)
		pCaller->get_Name(&bstrCaller.m_str);
	if (pContainer)
		pContainer->get_Name(&bstrContainer.m_str);
#endif

	// We want wizards (and thus world owner) to always succeed, even if IsMoveable is false
	hr = m_pWorld->CheckPolicy(pThis, CP_WIZARD, CComVariant(0L, VT_ERROR), &bOk);
	if (FAILED(hr))
		return hr;

	// If CheckPolicy succeedes, set return value to TRUE 
	if (bOk == VARIANT_TRUE)
	{
		*bReturnValue = VARIANT_TRUE;
		goto ERROR_ENCOUNTERED;
	}
	// Otherwise, continue checking other cases 
	else 
	{
		// If object can be moved, check other owner/caller permissions
		// (return bool value has already been defaulted to false)
		hr = pThis->get_BOOL(CComBSTR("IsMoveable"), &bIsMoveable);
		if (FAILED(hr))
			return hr;

		if (bIsMoveable == VARIANT_TRUE)
		{
			// Check if owner or if owner and caller's owner are the same
			hr = m_pWorld->CheckPolicy(pThis, CP_OWNER | CP_CALLER, CComVariant(0L, VT_ERROR), &bOk);
			if (FAILED(hr))
				return hr;

			if (bOk == VARIANT_TRUE)
			{
				*bReturnValue = VARIANT_TRUE;
				goto ERROR_ENCOUNTERED;
			}

			// If we aren't installing (a caller exists)
			if (pCaller)
			{
				// Next, check that container & caller are the same
				// For case of user taking/dropping
				if (pContainer)
				{
					if (pContainer == pCaller)
					{
						*bReturnValue = VARIANT_TRUE;
						goto ERROR_ENCOUNTERED;
					}
				}

				// Check if this == caller - case of objects setting own properties in vending machine, 
				// or client-side portal transport
				if (pThis == pCaller)
				{
					*bReturnValue = VARIANT_TRUE;
					goto ERROR_ENCOUNTERED;
				}

				// Checks for portal Transport: caller will be portal in destination room
				// Look for stub on client
				VARIANT_BOOL bIsStub = VARIANT_FALSE;
				hr = pCaller->get_IsStub(&bIsStub);
				if (FAILED(hr))
					return hr;

				if (bIsStub == VARIANT_TRUE)
					*bReturnValue = VARIANT_TRUE;

				// Or portal on server
				else
				{
					VARIANT_BOOL bIsPortal = VARIANT_FALSE;
					hr = pCaller->IsOfType(CComBSTR("Portal"), &bIsPortal);
					if (FAILED(hr))
						return(hr);
					if (bIsPortal == VARIANT_TRUE)
						*bReturnValue = VARIANT_TRUE;
				}
			}
		}
	}

ERROR_ENCOUNTERED:
#ifdef _DEBUG
	if (*bReturnValue == VARIANT_FALSE)
	{
		VWTRACE(m_pWorld, "VWMM", VWT_ERROR, "CMultimediaExemplarObject::OnPosDirScaleAccess: permission denied, this (%s), caller (%s), container (%s)\n",
			CString(bstrThis),CString(bstrCaller), CString(bstrContainer));
	}
#endif
	return hr;
}

// named mesh handling functions



//******************************************************
// _PushNew
// Adds a BSTR to the prop list only if it isn't already
// in the proplist

HRESULT	_PushNew(IPropertyList *pList, BSTR bstrNameVal)
{
    HRESULT     hr = S_OK;
	long		lCount, i;
	CComBSTR	bstrCurStr;

    if (pList==NULL)
        return E_POINTER;

	if (FAILED(hr = pList->get_Count(&lCount)))
		return hr;

	for (i = 0; i < lCount; i++)
	{
		if (FAILED(hr = pList->get_String(i, &bstrCurStr.m_str)))
		    return hr;
		if (::CompareElements(&bstrNameVal, &bstrCurStr.m_str))
			return S_OK;
	}
	
	// must not be there yet...
	hr = pList->AddString(bstrNameVal);
    
    return hr;
}

//******************************************************
// _AddNamedMeshesToPropList
// Given a prop list, adds all of the named meshes in the
// given frame and its descendants to the list.

HRESULT	_AddNamedMeshesToPropList(IVWFrame *pFrame, IPropertyList *pList)
{
	CComPtr<IPropertyList>	pChildList;
	CComVariant	varRetVal;
	CComBSTR	bstrMeshName;
	long		i;
    HRESULT     hr = S_OK;
	static CComBSTR			bstrVWSelectionBox(LEAVEMESHQUALITY);

    if (pFrame==NULL)
        return E_POINTER;

	// first, add in this frame's meshes
	if (FAILED(hr = pFrame->GetMeshCount(&varRetVal)))
		return hr;
	if (FAILED(hr = varRetVal.ChangeType(VT_I4)))
        return hr;

	for (i = 0; i < varRetVal.lVal; i++)
	{
		if (FAILED(hr = pFrame->GetMeshName(i, &bstrMeshName.m_str)))
    		return hr;
		if (bstrMeshName.Length() && !(::CompareElements(&bstrMeshName.m_str, &bstrVWSelectionBox.m_str)))
			_PushNew(pList, bstrMeshName.m_str);
	}
	
	// next, do the same thing for the children
	if (FAILED(hr = pFrame->GetChildren(&pChildList)))
		return hr;

	if (pChildList != NULL)
	{
		CComPtr<IVWFrame>	pCurChild;
		long	lNumChildren;

		if (FAILED(hr = pChildList->get_Count(&lNumChildren)))
		    return hr;

		for (i = 0; i < lNumChildren; i++)
		{
			pChildList->get_ObjectProperty(i, (IObjectProperty **)&pCurChild);
			_AddNamedMeshesToPropList(pCurChild, pList);
			pCurChild = NULL;
			pCurChild.Release();
		}
	}

    return hr;
}

//******************************************************
// _SetNamedMeshesToTexture
// For a given frame, sets the texture of every mesh of
// the specified name to the specified texture

static CComBSTR	kbstrWildcard = "*";

HRESULT	_SetNamedMeshesToTexture(IVWFrame *pFrame, BSTR bstrMeshName, BSTR bstrTextureName)
{
	CComPtr<IPropertyList>	pChildList;
	CComVariant	varRetVal;
	CComBSTR	bstrCurMeshName;
	long		i;
    HRESULT     hr = S_OK;

    if (pFrame==NULL)
        return E_POINTER;

	// first, add in this frame's meshes
	if (FAILED(hr = pFrame->GetMeshCount(&varRetVal)))
		return hr;
	if (FAILED(hr = varRetVal.ChangeType(VT_I4)))
        return hr;

	for (i = 0; i < varRetVal.lVal; i++)
	{
		if (FAILED(hr = pFrame->GetMeshName(i, &bstrCurMeshName.m_str)))
    		return hr;
		if (::CompareElements(&bstrCurMeshName.m_str, &bstrMeshName))
			pFrame->SetMeshTextureFromURL(i, bstrTextureName);
		else if (::CompareElements(&kbstrWildcard.m_str, &bstrMeshName))
			pFrame->SetMeshTextureFromURL(i, bstrTextureName);
	}
	
	// next, do the same thing for the children
	if (FAILED(hr = pFrame->GetChildren(&pChildList)))
		return hr;

	if (pChildList != NULL)
	{
		CComPtr<IVWFrame>	pCurChild;
		long	lNumChildren;

		if (FAILED(hr = pChildList->get_Count(&lNumChildren)))
	    	return hr;

		for (i = 0; i < lNumChildren; i++)
		{
			pChildList->get_ObjectProperty(i, (IObjectProperty **)&pCurChild);
			_SetNamedMeshesToTexture(pCurChild, bstrMeshName, bstrTextureName);
			pCurChild.Release();
		}
	}

    return hr;
}

//******************************************************
// _SetNamedMeshesToColor
// For a given frame, sets the RGB Color of every mesh of
// the specified name to the specified color

HRESULT	_SetNamedMeshesToColor(IVWFrame *pFrame, BSTR bstrMeshName, float fRed, float fGreen, float fBlue)
{
	CComPtr<IPropertyList>	pChildList;
	CComVariant	varRetVal;
	CComBSTR	bstrCurMeshName;
	long		i;
    HRESULT     hr = S_OK;

    if (pFrame==NULL)
        return E_POINTER;

    // first, add in this frame's meshes
	if (FAILED(hr = pFrame->GetMeshCount(&varRetVal)))
		return hr;
	if (FAILED(hr = varRetVal.ChangeType(VT_I4)))
        return hr;

	for (i = 0; i < varRetVal.lVal; i++)
	{
		if (FAILED(hr = pFrame->GetMeshName(i, &bstrCurMeshName.m_str)))
    		return hr;
		if (::CompareElements(&bstrCurMeshName.m_str, &bstrMeshName))
			pFrame->SetMeshColor(i, fRed, fGreen, fBlue);
		else if (::CompareElements(&kbstrWildcard.m_str, &bstrMeshName))
			pFrame->SetMeshColor(i, fRed, fGreen, fBlue);
	}
	
	// next, do the same thing for the children
	if (FAILED(hr = pFrame->GetChildren(&pChildList)))
		return hr;

	if (pChildList != NULL)
	{
		CComPtr<IVWFrame>	pCurChild;
		long	lNumChildren;

		if (FAILED(hr = pChildList->get_Count(&lNumChildren)))
		    return hr;

		for (i = 0; i < lNumChildren; i++)
		{
			pChildList->get_ObjectProperty(i, (IObjectProperty **)&pCurChild);
			_SetNamedMeshesToColor(pCurChild, bstrMeshName, fRed, fGreen, fBlue);
			pCurChild.Release();
		}
	}

    return hr;
}


//******************************************************
// _GetFirstNamedMeshTexture
// For a given frame, returns the texture of the first
// mesh of the given name

HRESULT	_GetFirstNamedMeshTexture(IVWFrame *pFrame, BSTR bstrMeshName, BSTR *pbstrTextureName, BOOL *pbFoundIt)
{
	CComPtr<IPropertyList>	pChildList;
	CComVariant	varRetVal;
	CComBSTR	bstrCurMeshName;
	long		i;
    HRESULT     hr = S_OK;

    if ((pFrame==NULL) || (pbstrTextureName==NULL) || (pbFoundIt==NULL))
        return E_POINTER;

	// first, add in this frame's meshes
	if (FAILED(hr = pFrame->GetMeshCount(&varRetVal)))
		return hr;
	if (FAILED(hr = varRetVal.ChangeType(VT_I4)))
        return hr;

	for (i = 0; i < varRetVal.lVal; i++)
	{
		if (FAILED(hr = pFrame->GetMeshName(i, &bstrCurMeshName.m_str)))
    		return hr;
		if (::CompareElements(&bstrCurMeshName.m_str, &bstrMeshName))
		{
			pFrame->GetMeshTextureName(i, pbstrTextureName);
			*pbFoundIt = TRUE;
			break;
		}
	}
	
	// next, do the same thing for the children
	if (!(*pbFoundIt))
	{
		if (FAILED(hr = pFrame->GetChildren(&pChildList)))
	    	return hr;

		if (pChildList != NULL)
		{
			CComPtr<IVWFrame>	pCurChild;
			long	lNumChildren;

			if (FAILED(hr = pChildList->get_Count(&lNumChildren)))
		        return hr;

			for (i = 0; i < lNumChildren; i++)
			{
				pChildList->get_ObjectProperty(i, (IObjectProperty **)&pCurChild);
				_GetFirstNamedMeshTexture(pCurChild, bstrMeshName, pbstrTextureName, pbFoundIt);
				pCurChild.Release();
				if (*pbFoundIt)
					break;
			}
		}
	}

    return hr;
}

//******************************************************
// _GetFirstNamedMeshColor
// For a given frame, returns the color of the first
// mesh of the given name

HRESULT	_GetFirstNamedMeshColor(IVWFrame *pFrame, BSTR bstrMeshName, float *pfRed, float *pfGreen, float *pfBlue, BOOL *pbFoundIt)
{
	CComPtr<IPropertyList>	pChildList;
	CComVariant	varRetVal;
	CComBSTR	bstrCurMeshName;
	long		i;
    HRESULT     hr = S_OK;

    if ((pFrame==NULL) || (pfRed==NULL) || (pfGreen==NULL) || (pfBlue==NULL) || (pbFoundIt==NULL))
        return E_POINTER;

	// first, add in this frame's meshes
	if (FAILED(hr = pFrame->GetMeshCount(&varRetVal)))
		return hr;
	if (FAILED(hr = varRetVal.ChangeType(VT_I4)))
        return hr;

	for (i = 0; i < varRetVal.lVal; i++)
	{
		if (FAILED(hr = pFrame->GetMeshName(i, &bstrCurMeshName.m_str)))
    		return hr;
		if (::CompareElements(&bstrCurMeshName.m_str, &bstrMeshName))
		{
			// REVIEW: Why does this function use variants????
			CComVariant varRed, varGreen, varBlue;

			pFrame->GetMeshColor(i, &varRed, &varGreen, &varBlue);

			::VariantChangeType(&varRed, &varRed, 0, VT_R4);
			::VariantChangeType(&varGreen, &varGreen, 0, VT_R4);
			::VariantChangeType(&varBlue, &varBlue, 0, VT_R4);

			*pfRed = varRed.fltVal; 
			*pfGreen = varGreen.fltVal; 
			*pfBlue = varBlue.fltVal; 
			*pbFoundIt = TRUE;
			break;
		}
	}
	
	// next, do the same thing for the children
	if (!(*pbFoundIt))
	{
		if (FAILED(hr = pFrame->GetChildren(&pChildList)))
	    	return hr;

		if (pChildList != NULL)
		{
			CComPtr<IVWFrame>	pCurChild;
			long	lNumChildren;

			if (FAILED(hr = pChildList->get_Count(&lNumChildren)))
		        return hr;

			for (i = 0; i < lNumChildren; i++)
			{
				pChildList->get_ObjectProperty(i, (IObjectProperty **)&pCurChild);
				_GetFirstNamedMeshColor(pCurChild, bstrMeshName, pfRed, pfGreen, pfBlue, pbFoundIt);
				pCurChild.Release();
				if (*pbFoundIt)
					break;
			}
		}
	}
    
    return hr;
}


//******************************************************
// GetNamedMeshes
// Returns a proplist fill with the unique names of
// all of the named meshes, if any.  If there are none,
// or if there is no frame, etc., it returns an 
// empty proplist.

STDMETHODIMP CMultimediaExemplarObject::GetNamedMeshes(IPropertyList **ppMeshList)
{
    HRESULT                 hr = S_OK;
	CComPtr<IThing>			pThis;
	CComPtr<IVWFrame>		pFrame;
	CComVariant				varRetVal;
	static CComBSTR			bstrGetFrame("GetFrame");

    if (ppMeshList==NULL)
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::GetNamedMeshes: NULL output pointer\n");
        return ReportMMError(E_POINTER);
    }

	if (FAILED(hr = ::CreatePropertyList(m_pWorld, ppMeshList)))
		return ReportMMError(hr);
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;
	if (FAILED(hr = pThis->InvokeMethodExt(bstrGetFrame, NULL, &varRetVal)))
		return hr;
	if (varRetVal.vt != VT_DISPATCH)
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_IMPORTANT, "CMultimediaExemplarObject::GetNamedMeshes: GetFrame returned non-dispatch VARIANT\n");
		return S_OK;
    }

	pFrame = (IVWFrame *)varRetVal.pdispVal;
	if (FAILED(hr = _AddNamedMeshesToPropList(pFrame, *ppMeshList)))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::GetNamedMeshes: unable to add meshes to property list\n");
		return ReportMMError(hr);
    }

    return hr;
}

//******************************************************
// GetNamedMeshTexture
// Given the name of a named mesh, returns that mesh's
// texture

STDMETHODIMP CMultimediaExemplarObject::GetNamedMeshTexture(BSTR bstrMeshName, BSTR *pbstrTextureURL)
{
    HRESULT                 hr = S_OK;
	CComPtr<IThing>			pThis;
	CComPtr<IVWFrame>		pFrame;
	CComVariant				varRetVal;
	static CComBSTR			bstrGetFrame("GetFrame");
	BOOL					bFoundIt = FALSE;

    if (pbstrTextureURL==NULL)
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::GetNamedMeshTexture: NULL output pointer\n");
        return ReportMMError(E_POINTER);
    }

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;
	if (FAILED(hr = pThis->InvokeMethodExt(bstrGetFrame, NULL, &varRetVal)))
		return hr;
	if (varRetVal.vt != VT_DISPATCH)
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_IMPORTANT, "CMultimediaExemplarObject::GetNamedMeshes: GetFrame returned non-dispatch VARIANT\n");
//		return S_OK;
		return E_FAIL;
    }
	pFrame = (IVWFrame *)varRetVal.pdispVal;
	if (FAILED(hr = _GetFirstNamedMeshTexture(pFrame, bstrMeshName, pbstrTextureURL, &bFoundIt)))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::GetNamedMeshTexture: unable to retrieve mesh texture from property list\n");
		return ReportMMError(hr);
    }
	if (!bFoundIt)
	{
		VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::GetNamedMeshTexture: the specified mesh was not found.\n");
	}
    return hr;
}

//******************************************************
// SetNamedMeshTexture
// Given the name of a named mesh, sets the texture of
// every mesh of that name to the specified texture
// The server-side method simply invokes
// the client-side method to actually do its work.

STDMETHODIMP CMultimediaExemplarObject::SetNamedMeshTexture(BSTR bstrMeshName, BSTR bstrTextureURL)
{
    HRESULT                 hr = S_OK;
	CComPtr<IThing>			pThis;
	CComPtr<IPropertyMap>	pMap;
	static CComBSTR			kbstrClientMethod("SetNamedMeshTextureClient");
	static CComBSTR			kbstrProp("NamedMeshTextures");

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;
	if (FAILED(hr = pThis->InvokeMethodExt(kbstrClientMethod,
										CComDISPPARAMS(2, CComVariant(bstrMeshName),
														  CComVariant(bstrTextureURL)),

										NULL)))
		return hr;

	// now, persist the change
	if (FAILED(hr = pThis->get_ObjectProperty(kbstrProp, (IObjectProperty **)&pMap))|| !pMap)
		return hr;
	if (FAILED(hr = pMap->put_String(bstrMeshName, bstrTextureURL)))
		return hr;
	hr = pThis->put_ObjectProperty(kbstrProp, (IObjectProperty *)pMap);
    
    return hr;
}


//******************************************************
// SetNamedMeshTextureClient
// Client-side handler for the above.

STDMETHODIMP CMultimediaExemplarObject::SetNamedMeshTextureClient(BSTR bstrMeshName, BSTR bstrTextureURL)
{
    HRESULT                 hr = S_OK;
	CComPtr<IThing>			pThis;
	CComPtr<IVWFrame>		pFrame;
	CComVariant				varRetVal;
	static CComBSTR			bstrGetFrame("GetFrame");

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;
	if (FAILED(hr = pThis->InvokeMethodExt(bstrGetFrame, NULL, &varRetVal)))
		return hr;
	if (varRetVal.vt != VT_DISPATCH)
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_IMPORTANT, "CMultimediaExemplarObject::SetNamedMeshTextureClient: GetFrame returned non-dispatch VARIANT\n");
		return S_OK;
    }

	pFrame = (IVWFrame *)varRetVal.pdispVal;
	if (FAILED(hr = _SetNamedMeshesToTexture(pFrame, bstrMeshName, bstrTextureURL)))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::SetNamedMeshTextureClient: unable to set named meshes to texturer URL\n");
		return ReportMMError(hr);
    }
    
    return hr;
}


//********************************************
// _OLEfromRGB
// _RGBfromOLE
// Ole to RGB color conversion helpers

OLE_COLOR	_OLEfromRGB(float fRed, float fGreen, float fBlue)
{
	const static float	fMax = 255.0f;

	return (OLE_COLOR)RGB((short)(fMax * fRed), (short)(fMax * fGreen), (short)(fMax * fBlue));
}

void	_RGBfromOLE(OLE_COLOR theColor, float *pfRed, float *pfGreen, float *pfBlue)
{
	COLORREF	newColor;
	const static float	fMax = 255.0f;

    ASSERT(pfRed!=NULL);
    ASSERT(pfGreen!=NULL);
    ASSERT(pfBlue!=NULL);
	::OleTranslateColor(theColor, NULL, &newColor);
	*pfRed = (float)GetRValue(newColor) / fMax;
	*pfGreen = (float)GetGValue(newColor) / fMax;
	*pfBlue = (float)GetBValue(newColor) / fMax;
}

//******************************************************
// GetNamedMeshColor
// Given the name of a named mesh, returns that mesh's
// color


STDMETHODIMP CMultimediaExemplarObject::GetNamedMeshColor(BSTR bstrMeshName, OLE_COLOR *pColor)
{
	CComPtr<IThing>			pThis;
	CComPtr<IVWFrame>		pFrame;
	CComVariant				varRetVal;
	static CComBSTR			bstrGetFrame("GetFrame");
	BOOL					bFoundIt = FALSE;
	float					fRed, fGreen, fBlue;
    HRESULT                 hr = S_OK;

    if (pColor==NULL)
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::GetNamedMeshColor: NULL output pointer\n");
        return ReportMMError(E_POINTER);
    }

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;
	if (FAILED(hr = pThis->InvokeMethodExt(bstrGetFrame, NULL, &varRetVal)))
		return hr;
	if (varRetVal.vt != VT_DISPATCH)
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_IMPORTANT, "CMultimediaExemplarObject::GetNamedMeshColor: GetFrame returned non-dispatch VARIANT\n");
		return E_FAIL;
    }

    pFrame = (IVWFrame *)varRetVal.pdispVal;
	if (FAILED(hr = _GetFirstNamedMeshColor(pFrame, bstrMeshName, &fRed, &fGreen, &fBlue, &bFoundIt)))
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::GetNamedMeshColor: unable to retrieve named mesh color\n");
		return ReportMMError(hr);
    }

    if (bFoundIt)
		*pColor = _OLEfromRGB(fRed, fGreen, fBlue);
	else
	{
		*pColor = (OLE_COLOR)RGB(0xFF,0xFF,0xFF);	// default color is white
		VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::GetNamedMeshColor: the specified mesh was not found, returning the default color value\n");
	}
    return hr;
}

//******************************************************
// SetNamedMeshColor
// Given the name of a named mesh, sets the color of
// every mesh of that name to the specified RGB color
// The server-side method simply invokes
// the client-side method to actually do its work.

STDMETHODIMP CMultimediaExemplarObject::SetNamedMeshColor(BSTR bstrMeshName, OLE_COLOR newColor)
{
	CComPtr<IThing>			pThis;
	CComPtr<IPropertyMap>	pMap;
	static CComBSTR			kbstrClientMethod("SetNamedMeshColorClient");
	static CComBSTR			kbstrProp("NamedMeshColors");
	VARIANT					varColor;
    HRESULT                 hr = S_OK;

	::VariantInit(&varColor);

	varColor.vt = VT_COLOR;
	varColor.lVal = (long)newColor;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	if (FAILED(hr = pThis->InvokeMethodExt(kbstrClientMethod,
										CComDISPPARAMS(2, CComVariant(bstrMeshName),
														  varColor),
										NULL)))
		return hr;

	// now, persist the change
	if (FAILED(hr = pThis->get_ObjectProperty(kbstrProp, (IObjectProperty **)&pMap)) || !pMap)
		return hr;
	if (FAILED(hr = pMap->put_Property(bstrMeshName, varColor)))
		return hr;
	hr = pThis->put_ObjectProperty(kbstrProp, (IObjectProperty *)pMap);
   
    return hr;
}


//******************************************************
// SetNamedMeshColorClient
// Client-side handler for the above.

STDMETHODIMP CMultimediaExemplarObject::SetNamedMeshColorClient(BSTR bstrMeshName, OLE_COLOR newColor)
{
	CComPtr<IThing>			pThis;
	CComPtr<IVWFrame>		pFrame;
	CComVariant				varRetVal;
	static CComBSTR			bstrGetFrame("GetFrame");
	float					fRed, fGreen, fBlue;
    HRESULT                 hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;
	if (FAILED(hr = pThis->InvokeMethodExt(bstrGetFrame, NULL, &varRetVal)))
		return hr;
	if (varRetVal.vt != VT_DISPATCH)
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_IMPORTANT, "CMultimediaExemplarObject::SetNamedMeshColorClient: GetFrame returned non-dispatch VARIANT\n");
		return S_OK;
    }

    pFrame = (IVWFrame *)varRetVal.pdispVal;
	_RGBfromOLE(newColor, &fRed, &fGreen, &fBlue);
	hr = _SetNamedMeshesToColor(pFrame, bstrMeshName, fRed, fGreen, fBlue);
    
    return hr;
}

//******************************************************
// RestorePersistedMeshChanges
// Restores the mesh changes from the persisted data.

HRESULT CMultimediaExemplarObject::RestorePersistedMeshChanges()
{
    HRESULT                 hr = S_OK;
	CComPtr<IThing>			pThis;
	CComPtr<IVWFrame>		pFrame;
	CComPtr<IPropertyMap>	pMap;
	CComVariant				varRetVal;
	long					lCount;
	static CComBSTR			kbstrColorProp("NamedMeshColors");
	static CComBSTR			kbstrTextureProp("NamedMeshTextures");
	static CComBSTR			bstrGetFrame("GetFrame");
	static CComBSTR			bstrAll("*");
	
	// 1. Get the frame
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;
	if (FAILED(hr = pThis->InvokeMethodExt(bstrGetFrame, NULL, &varRetVal)))
		return hr;
	if (varRetVal.vt != VT_DISPATCH)
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_IMPORTANT, "CMultimediaExemplarObject::RestorePersistedMeshChanges: GetFrame returned non-dispatch VARIANT\n");
		return S_OK;
    }

	pFrame = (IVWFrame *)varRetVal.pdispVal;
	
	if (FAILED(hr = pThis->get_ObjectProperty(kbstrColorProp, (IObjectProperty **)&pMap)) || !pMap)
		return hr;
	if (FAILED(hr = pMap->get_Count(&lCount)))
		return hr;
	if (lCount)
	{
		CComBSTR		bstrMeshName;
		CComVariant		varColor;
		VARIANT_BOOL	bDone;
		float			fRed, fGreen, fBlue;
		
		pMap->FirstItem(&bstrMeshName.m_str, &varColor, &bDone);
		_RGBfromOLE((OLE_COLOR)varColor.lVal, &fRed, &fGreen, &fBlue);
		if (FAILED(hr = _SetNamedMeshesToColor(pFrame, bstrMeshName, fRed, fGreen, fBlue)))
        {
            VWTRACE(m_pWorld, "VWMMTHING", VWT_IMPORTANT, "CMultimediaExemplarObject::RestorePersistedMeshChanges: unable to set named mesh to color\n");
		    return ReportMMError(hr);
        }

		while(bDone == VARIANT_FALSE)
		{
			pMap->NextItem(&bstrMeshName.m_str, &varColor, &bDone);
			_RGBfromOLE((OLE_COLOR)varColor.lVal, &fRed, &fGreen, &fBlue);
			if (FAILED(hr = _SetNamedMeshesToColor(pFrame, bstrMeshName, fRed, fGreen, fBlue)))
            {
                VWTRACE(m_pWorld, "VWMMTHING", VWT_IMPORTANT, "CMultimediaExemplarObject::RestorePersistedMeshChanges: unable to set named mesh to color\n");
		        return ReportMMError(hr);
            }
		}
	}
	pMap.Release();

	if (FAILED(hr = pThis->get_ObjectProperty(kbstrTextureProp, (IObjectProperty **)&pMap)) || !pMap)
		return hr;
	if (FAILED(hr = pMap->get_Count(&lCount)))
		return hr;
	if (lCount)
	{
		CComBSTR		bstrMeshName, bstrAllMeshTextureName;
		CComVariant		varTexture;
		VARIANT_BOOL	bDone, bAllMeshTexture = VARIANT_FALSE;

		//First determine if "*" is one of the named meshes.
		hr = pMap->get_String(bstrAll, &bstrAllMeshTextureName.m_str);
		if (SUCCEEDED(hr))
		{
			bAllMeshTexture = VARIANT_TRUE;
		}

		//If there's an "*", do it first.
		if (bAllMeshTexture && FAILED(hr = _SetNamedMeshesToTexture(pFrame, bstrAll, bstrAllMeshTextureName)))
		{
			VWTRACE(m_pWorld, "VWMMTHING", VWT_IMPORTANT, "CMultimediaExemplarObject::RestorePersistedMeshChanges: unable to set named mesh to color\n");
			return ReportMMError(hr);
		}


		pMap->FirstItem(&bstrMeshName.m_str, &varTexture, &bDone);
		if (!bAllMeshTexture || !::CompareElements(&bstrMeshName.m_str, &bstrAll.m_str ))
		{
			if (FAILED(hr = _SetNamedMeshesToTexture(pFrame, bstrMeshName, varTexture.bstrVal)))
			{
				VWTRACE(m_pWorld, "VWMMTHING", VWT_IMPORTANT, "CMultimediaExemplarObject::RestorePersistedMeshChanges: unable to set named mesh to color\n");
				return ReportMMError(hr);
			}
		}
		varTexture.Clear();

		while(bDone == VARIANT_FALSE)
		{
			pMap->NextItem(&bstrMeshName.m_str, &varTexture, &bDone);

			if (!bAllMeshTexture || !::CompareElements(&bstrMeshName.m_str, &bstrAll.m_str ))
			{
				if (FAILED(hr = _SetNamedMeshesToTexture(pFrame, bstrMeshName, varTexture.bstrVal)))
				{
					VWTRACE(m_pWorld, "VWMMTHING", VWT_IMPORTANT, "CMultimediaExemplarObject::RestorePersistedMeshChanges: unable to set named mesh to color\n");
					return ReportMMError(hr);
				}
			}
			varTexture.Clear();
		}
	}
	pMap.Release();
    
    return hr;
}

//******************************************************
// OnApproachDone
// Called to send the message for approach.


HRESULT CMultimediaExemplarObject::OnApproachDone()
{
	CComPtr<IThing>			pThis, pInteractee;
	CComPtr<IPropertyList>	pList;
	CComBSTR				bstrVerb;
	HRESULT					hr = S_OK;

	static CComBSTR bstrPosition("Position");

	// Get the relevant interaction related properties from this object.
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;
	if (FAILED(hr = pThis->get_String(CComBSTR("InteractVerb"), &bstrVerb.m_str)))
		return hr;
	if (FAILED(hr = pThis->get_Thing(CComBSTR("Interactee"), &pInteractee)))
		return hr;

	// Need all the properties to do interaction.
	if (bstrVerb.Length() == 0 || !pInteractee)
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::OnApproachDone: failed interaction\n");
		return ReportMMError(VWMM_E_UNKNOWN);
    }
	
	// Make sure these properties are made empty.
	if (FAILED(hr = pThis->put_String(CComBSTR("InteractVerb"), CComBSTR(""))))
		return hr;
	if (FAILED(hr = pThis->put_Thing(CComBSTR("Interactee"), NULL)))
		return hr;

	// Create the Action event subject list and fire the Action event.
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
		return ReportMMError(hr);
	if (FAILED(hr = pList->AddThing(pThis)))
		return hr;
	if (FAILED(hr = pList->AddThing(pInteractee)))
		return hr;
	pThis->FireEventExt(CComBSTR("Action"),
						CComDISPPARAMS(3, CComVariant(CComBSTR("Interaction")),
										  CComVariant(bstrVerb),
										  CComVariant((IDispatch*) pList)),
						FIREEVENT_DEFAULT);

    return hr;
}

//******************************************************
// UpdateSolidBoundary - called to update the bounding box and rebuild solid boundary if necessary.
//						 Should only be called if IsSolid is true to save the server call.
STDMETHODIMP CMultimediaExemplarObject::UpdateSolidBoundary(float fMinX, float fMinY, float fMinZ, float fMaxX, float fMaxY, float fMaxZ)
{
	HRESULT					hr = S_OK;
	CComPtr<IThing>			pThis;
	CComVariant				var;
	CComPtr<IPropertyList>	pList;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	if (FAILED(hr = pThis->get_ObjectProperty(bstrBoundingBox, (IObjectProperty**) &pList.p)))
		return hr;

	if (FAILED(hr = pList->get_Property(0, &var)) || var.vt != VT_R4)
		return hr;
	if (!FLOAT_EQUAL(var.fltVal, fMinX))
		goto APPLY_NEW_BOUNDARY;
	
	if (FAILED(hr = pList->get_Property(1, &var)) || var.vt != VT_R4)
		return hr;
	if (!FLOAT_EQUAL(var.fltVal, fMinY))
		goto APPLY_NEW_BOUNDARY;
	
	if (FAILED(hr = pList->get_Property(2, &var)) || var.vt != VT_R4)
		return hr;
	if (!FLOAT_EQUAL(var.fltVal, fMinZ))
		goto APPLY_NEW_BOUNDARY;
	
	if (FAILED(hr = pList->get_Property(3, &var)) || var.vt != VT_R4)
		return hr;
	if (!FLOAT_EQUAL(var.fltVal, fMaxX))
		goto APPLY_NEW_BOUNDARY;
	
	if (FAILED(hr = pList->get_Property(4, &var)) || var.vt != VT_R4)
		return hr;
	if (!FLOAT_EQUAL(var.fltVal, fMaxY))
		goto APPLY_NEW_BOUNDARY;
	
	if (FAILED(hr = pList->get_Property(5, &var)) || var.vt != VT_R4)
		return hr;
	if (!FLOAT_EQUAL(var.fltVal, fMaxZ))
		goto APPLY_NEW_BOUNDARY;

	goto EXIT_FAIL;

APPLY_NEW_BOUNDARY:
	if (FAILED(hr = pList->put_Property(0, CComVariant(fMinX))))
		return hr;
	if (FAILED(hr = pList->put_Property(1, CComVariant(fMinY))))
		return hr;
	if (FAILED(hr = pList->put_Property(2, CComVariant(fMinZ))))
		return hr;
	if (FAILED(hr = pList->put_Property(3, CComVariant(fMaxX))))
		return hr;
	if (FAILED(hr = pList->put_Property(4, CComVariant(fMaxY))))
		return hr;
	if (FAILED(hr = pList->put_Property(5, CComVariant(fMaxZ))))
		return hr;

	if (FAILED(hr = ApplySolidProperty(thPut, pThis)))
		return hr;

EXIT_FAIL:
    return hr;
}

//******************************************************
// OnSolidChanged
STDMETHODIMP CMultimediaExemplarObject::OnSolidChanged(long lHint, VARIANT varHintData)
{
	CComPtr<IThing>			pThis;
	long nCount, i;
	CComPtr<IBoundary>		pBoundary;
	CComPtr<IPropertyList>	pBoundaryList;
	VARIANT_BOOL			vbIsAutoSolidBoundary;
	HRESULT					hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	if (FAILED(hr = pThis->get_ObjectProperty(bstrBoundaryList, (IObjectProperty**) &pBoundaryList.p)))
	{
		return hr = ApplySolidProperty(lHint, pThis);
	}

	if (FAILED(hr = pBoundaryList->get_Count(&nCount)))
		return hr;

	for (i = 0; i < nCount; i++)
	{
		CComPtr<IBoundary>		pTmpBoundary;

		if (FAILED(hr = pBoundaryList->get_ObjectProperty(i, (IObjectProperty**) &pTmpBoundary.p)))
		    return hr;

        if (pTmpBoundary == NULL)
            continue;

		if (FAILED(hr = pTmpBoundary->get_IsAutoSolidBoundary(&vbIsAutoSolidBoundary)))
		    return hr;

		if (vbIsAutoSolidBoundary)
		{
			pBoundary = pTmpBoundary;
			break;
		}
	}

	//If we've found a boundary and the property is now false then remove it.
	if (pBoundary.p && !varHintData.boolVal)
	{
		if (FAILED(hr = ApplySolidProperty(lHint, pThis)))
			return hr;
	}
	else if (!pBoundary.p && varHintData.boolVal)
	{
		if (FAILED(hr = ApplySolidProperty(lHint, pThis)))
			return hr;
	}

	return hr;
}

//******************************************************
// ApplySolidProperty
//    Does the dirty work of rebuilting the solid boundary.

HRESULT CMultimediaExemplarObject::ApplySolidProperty(long lHint, IThing* pThis)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	long nCount, i;
	CComPtr<IBoundary>		pBoundary;
	CComPtr<IPropertyList>	pBoundaryList;
	CComPtr<IWorld>			pWorld;
	VARIANT_BOOL			vbSolid, vbIsAutoSolidBoundary;
    HRESULT                 hr = S_OK;
	
	vbIsAutoSolidBoundary = VARIANT_FALSE;

	if (FAILED(hr = pThis->get_World(&pWorld)))
		return hr;

	if (FAILED(hr = pThis->get_BOOL(bstrSolid, &vbSolid)))
		return hr;

	if (FAILED(hr = pThis->get_ObjectProperty(bstrBoundaryList, (IObjectProperty**) &pBoundaryList.p)))
		return hr;
	
	if (!pBoundaryList)
	{
		ASSERT(0);

		VARIANT_BOOL fBoundaryListPropertyExists;

		fBoundaryListPropertyExists = FALSE;

		if (FAILED(hr = ::CreateBoundaryList(m_pWorld, &pBoundaryList)))
		    return ReportMMError(hr);

		if (FAILED(hr = pThis->IsValidProperty(CComBSTR("BoundaryList"), &fBoundaryListPropertyExists )))
		    return hr;

		if(VARIANT_TRUE == fBoundaryListPropertyExists)
		{
			if (FAILED(hr = pThis->put_ObjectProperty(bstrBoundaryList, (IObjectProperty*) pBoundaryList.p)))
		        return hr;
		}
		else // Doesn't exit, Add it,
		{
			if (FAILED(hr = pThis->AddObjectProperty(bstrBoundaryList, (IObjectProperty*) pBoundaryList.p)))
		        return hr;
		}
	}

	if (FAILED(hr = pBoundaryList->get_Count(&nCount)))
		return hr;

	for (i = 0; i < nCount; i++)
	{
		CComPtr<IBoundary>		pTmpBoundary;

		if (FAILED(hr = pBoundaryList->get_ObjectProperty(i, (IObjectProperty**) &pTmpBoundary.p)))
		    return hr;

        if (pTmpBoundary == NULL)
            continue;

		if (FAILED(hr = pTmpBoundary->get_IsAutoSolidBoundary(&vbIsAutoSolidBoundary)))
		    return hr;

		if (vbIsAutoSolidBoundary)
		{
			pBoundary = pTmpBoundary;
			break;
		}
	}

	if (vbIsAutoSolidBoundary && !vbSolid)  //Remove it, possible add later to remote change
	{
		if (FAILED(hr = pBoundaryList->RemoveAt(i)))
		    return hr;

		pBoundary.Release();
	}

	// only listen to put notifications
	if (lHint != thRemove)
	{
		if (vbSolid)
		{
			CComVariant varMinX, varMinY, varMinZ, varMaxX, varMaxY, varMaxZ;
			CComPtr<IPropertyList>	pList;

			if (!vbIsAutoSolidBoundary)
			{
				//Create new boundary 
				if (FAILED(hr = ::CreateBoundary(m_pWorld, &pBoundary)))
					return ReportMMError(hr);
			}

			if (FAILED(hr = pBoundary->put_IsAutoSolidBoundary(VARIANT_TRUE)))
		        return hr;

			if (FAILED(hr = pThis->get_ObjectProperty(bstrBoundingBox, (IObjectProperty**) &pList.p)))
				return hr;

			if (FAILED(hr = pList->get_Property(0, &varMinX)) || varMinX.vt != VT_R4)
				return hr;
			if (FAILED(hr = pList->get_Property(1, &varMinY)) || varMinY.vt != VT_R4)
				return hr;
			if (FAILED(hr = pList->get_Property(2, &varMinZ)) || varMinZ.vt != VT_R4)
				return hr;
			if (FAILED(hr = pList->get_Property(3, &varMaxX)) || varMaxX.vt != VT_R4)
				return hr;
			if (FAILED(hr = pList->get_Property(4, &varMaxY)) || varMaxY.vt != VT_R4)
				return hr;
			if (FAILED(hr = pList->get_Property(5, &varMaxZ)) || varMaxZ.vt != VT_R4)
				return hr;

			hr = pBoundary->AutoGenerateFromBox(varMinX.fltVal, varMinY.fltVal, varMinZ.fltVal, varMaxX.fltVal, varMaxY.fltVal, varMaxZ.fltVal, 6, 0.0f);
			if (hr == VWMM_E_INVALIDBOUNDARY)
			{	//Object has no geometry
		        hr = S_OK;
				goto EXIT_FAIL;
			}
			if (FAILED(hr))
				return hr;

			if (vbIsAutoSolidBoundary)
			{
				CComVariant pvarBoundary;

				if (FAILED(hr = pWorld->CloneProperty( CComVariant(pBoundary.p), &pvarBoundary )))
					return hr;

				if (FAILED(hr = pBoundaryList->put_Property(i, pvarBoundary)))
					return hr;
			}
			else
			{
				if (FAILED(hr = pBoundaryList->AddObjectProperty( pBoundary.p )))
					return hr;
			}
		}
	}

EXIT_FAIL:
	return hr;
}

//********************************************
// OnLoadGeometry
// This is called whenever the geometry changes
// If you override this, you'd better call the
// inherited method or your object will not work.

STDMETHODIMP CMultimediaExemplarObject::OnLoadGeometry(IVWFrame *pNewFrame)
{
	CComPtr<IThing> pThis;
	VARIANT_BOOL	bIsSolid;
	HRESULT hr = S_OK;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pNewFrame)
	{
		if (FAILED(hr = m_pWorld->get_This(&pThis)))
			return hr;
		
		if (FAILED(hr = pThis->get_BOOL(bstrSolid, &bIsSolid)))
			return hr;

		if (bIsSolid) 
		{
			CComVariant varMinX, varMinY, varMinZ, varMaxX, varMaxY, varMaxZ;

			if (FAILED(hr = pNewFrame->GetBoundingBox( &varMinX, &varMinY, &varMinZ, &varMaxX, &varMaxY, &varMaxZ )))
				return hr;

			if (FAILED(hr = pThis->InvokeMethodExt(bstrUpdateSolidBoundary, CComDISPPARAMS(6, varMinX, varMinY, varMinZ, varMaxX, varMaxY, varMaxZ), NULL)))
				return hr;
		}

		RestorePersistedMeshChanges();
	}
	else
	{
		VWTRACE(m_pWorld, "VWMMTHING", VWT_IMPORTANT, "CMultimediaExemplarObject::OnLoadGeometry: Received NULL Frame pointer\n");
	}

	return S_OK;
}


//******************************************************
// FindFloorHeight
// Used to find the first floor or ceiling under the given point.
STDMETHODIMP CMultimediaExemplarObject::FindFloorHeight(float fPosX, float fPosY, float fPosZ, float* fFloorHeight)
{
	CComPtr<IThing>		pThis;
	CComPtr<IVWFrame>	pvwfTransform;
    HRESULT             hr = S_OK;

    if (fFloorHeight == NULL)
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::FindFloorHeight: NULL output pointer\n");
        return ReportMMError(E_POINTER);
    }

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;
	if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)) || !pvwfTransform)
    {
		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::FindFloorHeight: Unable to get the frame\n");
        return ReportMMError(hr);
    }
    hr = pvwfTransform->GetFloorHeightAtPosition(fPosX, fPosY, fPosZ, fFloorHeight);

    return hr;
}

//******************************************************
// OnContentSystemEnter
// Called by the geometry code when some geometry enters
// the contents...

STDMETHODIMP CMultimediaExemplarObject::OnContentSystemEnter(IThing *pContainer, IThing *pThing)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IVWFrame>			pVWFrame, pvwfThing;
	CComPtr<IPropertyList>		pCell;
	COleVariant					var;
	VARIANT_BOOL				bIsChild;
	VARIANT_BOOL				bContentsVisible;
    HRESULT                     hr = S_OK;

	static CComBSTR				bstrComposeScene("ComposeScene");
	static CComBSTR				bstrContentsVisible("IsContentsVisible");

	if ((pContainer==NULL) || (pThing==NULL))
    {
    	VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::OnContentSystemEnter: NULL pointer\n");
		return ReportMMError(E_POINTER);
    }

	// A thing is entering a location. We must add the geometry for the thing
	// to the location.

	// Get the location's loaded geometry.
	if (FAILED(hr = _GetFrameHelper(pContainer, &pVWFrame)))
    {
    	VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::OnContentSystemEnter: Unable to retrieve geometry\n");
		return ReportMMError(hr);
    }

	if (!pVWFrame)
		return S_OK;

	// This container's contents might be occluded.
	if (FAILED(hr = pContainer->get_BOOL(bstrContentsVisible, &bContentsVisible)))
		return hr;
	if (bContentsVisible==VARIANT_FALSE)
		return S_OK;

	// Get the thing's geometry.
	if (FAILED(hr = pThing->InvokeMethodExt(bstrComposeScene, CComDISPPARAMS(1, CComVariant(pThing), NULL), &var)))
		return hr;
	if (var.vt != VT_DISPATCH || var.pdispVal == NULL)
    {
    	VWREPORT(m_pWorld, VWR_ERROR, "CMultimediaExemplarObject::OnContentSystemEnter: not dispatch\n");
        return ReportMMError(VWMM_E_UNKNOWN);
    }

	pvwfThing = (IVWFrame*)var.pdispVal;

	// It's possible that another thing is also updating this scene and 
	// has already done this.
	if (FAILED(hr = pvwfThing->IsChildOf(pVWFrame, &bIsChild)))
		return hr;
	if (bIsChild!=VARIANT_TRUE)
	{
		// Add the entering thing's geometry to this thing.
		if (FAILED(hr = pVWFrame->AddChild(pvwfThing)))
    		return hr;

		// And set it's scale, position and direction appropriately.
		if (FAILED(hr = OrientAndScaleTransform(pThing, pVWFrame, pvwfThing)))
    		return hr;

		UpdateLabel(pThing);
	}

    return hr;
}

//******************************************************
// OnContentSystemLeave
// Called by the geometry code when something exits
// the contents...

STDMETHODIMP CMultimediaExemplarObject::OnContentSystemLeave(IThing *pContainer, IThing *pThing)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IVWFrame>		pVWFrame, pvwfThing;
	CComPtr<IPropertyList>	pCell;
	VARIANT_BOOL			bIsChild = VARIANT_FALSE;
	VARIANT_BOOL			bIsStub = VARIANT_FALSE;
	HRESULT                 hr = S_OK;

	// A thing is leaving a location. We must remove the geometry for the thing
	// from the location.
	
	// Get the location's loaded geometry.
	if (FAILED(hr = _GetFrameHelper(pContainer, &pVWFrame)))
    {
    	VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::OnContentSystemLeave: Unable to retrieve geometry\n");
		return ReportMMError(hr);
    }

	// Just check if the location has any geometry loaded!
	if (!pVWFrame)
		// Moving a thing out of a location with no loaded geometry, don't do anything.
		return S_OK;

	// Get the thing's loaded geometry.
	// Only do this if Thing is not a stub
	// (If a stub, you are on a remote client when object is first being created and dropped within another user)
	if (FAILED(hr = pThing->get_IsStub(&bIsStub)))
		return hr;

	if (bIsStub == VARIANT_FALSE)
	{
		if (FAILED(hr = _GetFrameHelper(pThing, &pvwfThing)))
		{
    		VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::OnContentSystemLeave: Unable to retrieve geometry\n");
			return ReportMMError(hr);
		}

		// Just check if the thing has any geometry loaded!
		if (!pvwfThing)
			// Moving a thing with no loaded geometry out of a location with loaded geometry, 
			// don't do anything.
			return S_OK;

		// It's possible that another thing is also updating this scene and 
		// has already done this.
		if (FAILED(hr = pvwfThing->IsChildOf(pVWFrame, &bIsChild)))
			return hr;
		if (bIsChild==VARIANT_TRUE)
		{
			// And delete the leaving thing's geometry from this thing.
			if (FAILED(hr = pVWFrame->DeleteChild(pvwfThing)))
    			return hr;
		}
	}

    return hr;
}

STDMETHODIMP CMultimediaExemplarObject::OnLabelPropertyChanged(long lHint, VARIANT varHintData)
{
	CComPtr<IThing> pThis;
	HRESULT	hr = S_OK;

	// Just adding property to thing - ignore.
	if (lHint == thAdd)
		return S_OK;
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;
	return UpdateLabel(pThis);
}

STDMETHODIMP CMultimediaExemplarObject::UpdateLabel(IThing *pThing)
{
	CComPtr<IVWFrame>	pvwfGeometry;
	CComBSTR			bstrLabelValue, bstrLabelFontNameValue;
	long				lLabelFontSize, lLabelStyle, lDecalIndex, lLabelPlacement;
	CComPtr<IVector>	pvLabelFontColor, pvLabelShadowColor;
	CComPtr<IVWGeometry>	pVWGeometry;
	VARIANT_BOOL		bIsAvatar;
	HRESULT hr = S_OK;

	static CComBSTR bstrLabel("Label");
	static CComBSTR bstrLabelPlacement("LabelPlacement");
	static CComBSTR bstrLabelFontName("LabelFontName");
	static CComBSTR bstrLabelFontSize("LabelFontSize");
	static CComBSTR bstrLabelStyle("LabelStyle");
	static CComBSTR bstrLabelFontColor("LabelFontColor");
	static CComBSTR bstrLabelShadowColor("LabelShadowColor");
	static CComBSTR	bstrGeometry("Geometry");


	if (FAILED(hr = pThing->get_ObjectPropertyExt(bstrGeometry, IID_IVWGeometry, (IObjectProperty**)&pVWGeometry)) || !pVWGeometry)
		return hr;

	if (FAILED(hr = pVWGeometry->get_GeometryFrame(&pvwfGeometry)) || !pvwfGeometry)
		return hr;

	// Remove label if already exists.
	if (SUCCEEDED(hr = pvwfGeometry->GetDecalIndexByName(CComBSTR(LABELDECALNAME), &lDecalIndex)))
	{
		if (FAILED(hr = pvwfGeometry->DeleteDecal(lDecalIndex)))
			return hr;
	}

	// If label is empty and not an avatar (all avatars have labels) never do anything.
	if (FAILED(hr = pThing->get_String(bstrLabel, &bstrLabelValue.m_str)))
		return hr;
	if (FAILED(hr = m_pWorld->IsAvatar(pThing, &bIsAvatar)))
		return hr;
	if (bstrLabelValue.Length() == 0)
	{
		if (bIsAvatar)
		{
			// Use avatar's name for label.
			bstrLabelValue.Empty();
			if (FAILED(hr = pThing->get_Name(&bstrLabelValue.m_str)))
				return hr;
		}
		else
			return hr;
	}

	if (FAILED(hr = pThing->get_Long(bstrLabelPlacement, &lLabelPlacement)))
		return hr;
	if (FAILED(hr = pThing->get_String(bstrLabelFontName, &bstrLabelFontNameValue.m_str)))
		return hr;
	if (FAILED(hr = pThing->get_Long(bstrLabelFontSize, &lLabelFontSize)))
		return hr;
	if (lLabelFontSize < 1)
	{
		pThing->put_Long(bstrLabelFontSize, 1);
		return ReportMMError(VWMM_E_LABELFONTSIZE);
	}
	if (lLabelFontSize > 100)
	{
		pThing->put_Long(bstrLabelFontSize, 100);
		return ReportMMError(VWMM_E_LABELFONTSIZE);
	}
	if (FAILED(hr = pThing->get_Long(bstrLabelStyle, &lLabelStyle)))
		return hr;
	if (FAILED(hr = pThing->get_ObjectPropertyExt(bstrLabelFontColor, IID_IVector, (IObjectProperty**)&pvLabelFontColor)))
		return hr;
	if (FAILED(hr = pThing->get_ObjectPropertyExt(bstrLabelShadowColor, IID_IVector, (IObjectProperty**)&pvLabelShadowColor)))
		return hr;

	if (FAILED(hr = pvwfGeometry->CreateDecalFromTextExt(bstrLabelValue, lLabelPlacement, 
														  bstrLabelFontNameValue, lLabelFontSize,
														  lLabelStyle, pvLabelFontColor, pvLabelShadowColor,
														  CComVariant(LABELDECALNAME))))
		return hr;

	return hr;
}



long CMultimediaExemplarObject::CurrentTime()
{
	long lSystemTime = 0;
	long lSystemStartTicks = 0;
	unsigned long	ulSystemTicks = 0;
	unsigned long	ulSystemTickTime = 0;
	HRESULT hr = S_OK;

	CComPtr <IThing> pGlobal;

	hr = m_pWorld->get_Global(&pGlobal);

	hr = pGlobal->get_Long(CComBSTR("SystemTime"), &lSystemTime);

	hr = pGlobal->get_Long(CComBSTR("SystemStartTicks"), &lSystemStartTicks);

	hr = ULongSubtract((unsigned long)GetTickCount(),(unsigned long)lSystemStartTicks, &ulSystemTicks);

	hr = ULongAdd((unsigned long)lSystemTime , ulSystemTicks, &ulSystemTickTime);

	return ((long)ulSystemTickTime);
}

STDMETHODIMP CMultimediaExemplarObject::GetAnimationSystemTime(long* CurrentAnimTime)
{
	HRESULT	hr = S_OK;
	*(CurrentAnimTime) = CurrentTime();
	return hr;
}
