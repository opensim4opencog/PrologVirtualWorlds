// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// mmroom.cpp : Room extensions

#include "stdafx.h"
#include <d3drmwin.h>
#include <vwmmex.h>
#include <vwidata.h>
#include <vwanim.h>
#include "vwutils.h"
#include "vwmmexo.h"

#include <syshelp.h>

///////////////////////////////////////////////////////////////////////
// Parameters

extern CComBSTR bstrBoundaryList;

///////////////////////////////////////////////////////////////////////
// Helpers
extern HRESULT	_GetFrameHelper(IThing *pThing, IVWFrame **ppvwf);
extern float CastToFloat(VARIANT var);
extern long CastToLong(VARIANT var);
///////////////////////////////////////////////////////////////////////
// Install 

HRESULT CMultimediaExemplarObject::InstallRoomExtensions(IModule* pModule)
{

	CMethodInfo rgMethod[] =
	{
		{ METHOD_CLIENT,	"ValidatePosition",					NULL,			PSBIT_HIDDEN,			PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_CLIENT,	"OnFogChanged",						NULL, 			PSBIT_EVENTHANDLER,		PS_EVENTHANDLER },
		{ METHOD_CLIENT,	"OnFogStartChanged",				NULL, 			PSBIT_EVENTHANDLER,		PS_EVENTHANDLER },
		{ METHOD_CLIENT,	"OnFogColorChanged",				NULL, 			PSBIT_EVENTHANDLER,		PS_EVENTHANDLER },
		{ METHOD_CLIENT,	"OnFogEndChanged",					NULL,			PSBIT_EVENTHANDLER,		PS_EVENTHANDLER },
		{ METHOD_CLIENT,	"OnLoadGeometry",			"OnRoomLoadGeometry",	PSBIT_HIDDEN,			PS_EXEMPLARDEFAULTMETHOD  },
		{ METHOD_SERVER,	"OnActivate",						NULL, 			PSBIT_EVENTHANDLER,		PS_EVENTHANDLER },
		{ METHOD_SERVER,	"OnDeactivate",						NULL, 			PSBIT_EVENTHANDLER,		PS_EVENTHANDLER }

	};
	int cMethod = sizeof(rgMethod)/sizeof(rgMethod[0]);
    int iMethod;

	// find the Room exemplar
	CComPtr<IThing> pRoomEx;
	CComPtr<IVector> pLocDef;
	CComPtr<IPropertyList> pList;
	CComBSTR bstrName;
	CComBSTR bstrNameInternal;
	CComBSTR bstrMotion;
	CComBSTR bstrApproach;
    HRESULT  hr = S_OK;

	if (FAILED(hr = m_pWorld->get_Exemplar(CComBSTR("Room"), &pRoomEx)))
		goto ERROR_ENCOUNTERED;

	// add in methods
	for (iMethod=0; iMethod<cMethod; iMethod++)
	{
		bstrName = rgMethod[iMethod].pszName;

		if (rgMethod[iMethod].pszNameInternal == NULL)
			bstrNameInternal = bstrName;
		else
			bstrNameInternal = rgMethod[iMethod].pszNameInternal;
		
    	if (FAILED(hr = pRoomEx->CreateAndAddMethodExt(	rgMethod[iMethod].lFlags,
												pModule,
												bstrNameInternal,
												bstrName,
												rgMethod[iMethod].psbits,
												rgMethod[iMethod].permissions)))
            goto ERROR_ENCOUNTERED;
	}


	// add in properties
	if (FAILED(hr = pRoomEx->AddPropertyExt(	CComBSTR("SpeakingRange"),
											CComVariant((long) 0),
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_I4, IID_IDispatch, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
		goto ERROR_ENCOUNTERED;
 	if (FAILED(hr = pRoomEx->AddPropertyExt(	CComBSTR("InternalBoundaryList"), 
											CComVariant((IDispatch *) pList), 
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTCOLLECTION, 
											VT_DISPATCH, IID_IPropertyList, NULL)))
		goto ERROR_ENCOUNTERED;

	pList.Release();

	if (FAILED(hr = pRoomEx->AddPropertyExt(	CComBSTR("BackgroundColor"), 
											CComVariant((long)0),
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_FRIENDLY, 
											PS_EXEMPLARDEFAULTPROPERTY, 
											VT_VARIANT, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = CreateVector(0.0, 0.0, 0.0, &pLocDef)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pRoomEx->AddPropertyExt(	CComBSTR("DefaultAvatarPosition"), 
											CComVariant((IDispatch *) pLocDef), 
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_DISPATCH, IID_IVector, NULL)))
		goto ERROR_ENCOUNTERED;

	pLocDef.Release();

	if (FAILED(hr = pRoomEx->AddPropertyExt(	CComBSTR("Fog"),
											CComVariant((bool)VARIANT_FALSE),
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_FRIENDLY, 
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_BOOL, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;
	
	if (FAILED(hr = pRoomEx->AddPropertyExt(	CComBSTR("FogStart"),
											CComVariant((short) 1),
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_FRIENDLY, 
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_I2, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pRoomEx->AddPropertyExt(	CComBSTR("FogEnd"),
											CComVariant((short) 150),
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_FRIENDLY, 
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_I2, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;
	
	if (FAILED(hr = pRoomEx->AddPropertyExt(	CComBSTR("FogColor"),
											CComVariant((long) 16777215),
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_FRIENDLY, 
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_VARIANT, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	// amend edit maps to get rid of motion and approach tabs for rooms
	bstrMotion = "Motion|";
	bstrApproach = "Approach|";

	if (FAILED(hr = pRoomEx->get_ObjectPropertyExt(CComBSTR("EndUserEditMap"), IID_IPropertyList, (IObjectProperty **) &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrMotion)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrApproach)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pRoomEx->put_ObjectProperty(CComBSTR("EndUserEditMap"), (IObjectProperty *) pList)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

	if (FAILED(hr = pRoomEx->get_ObjectPropertyExt(CComBSTR("OwnerEditMap"), IID_IPropertyList, (IObjectProperty **) &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrMotion)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrApproach)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pRoomEx->put_ObjectProperty(CComBSTR("OwnerEditMap"), (IObjectProperty *) pList)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

	if (FAILED(hr = pRoomEx->get_ObjectPropertyExt(CComBSTR("BuilderEditMap"), IID_IPropertyList, (IObjectProperty **) &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrMotion)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrApproach)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pRoomEx->put_ObjectProperty(CComBSTR("BuilderEditMap"), (IObjectProperty *) pList)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

	if (FAILED(hr = pRoomEx->get_ObjectPropertyExt(CComBSTR("AdvancedEditMap"), IID_IPropertyList, (IObjectProperty **) &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrMotion)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrApproach)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pRoomEx->put_ObjectProperty(CComBSTR("AdvancedEditMap"), (IObjectProperty *) pList)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

	

    return S_OK;

ERROR_ENCOUNTERED:
	
	VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::InstallRoomExtensions: Error encountered: %x\n", hr);
    return hr;
}

STDMETHODIMP CMultimediaExemplarObject::ValidatePosition(IThing *pObject, IVector *pStartLoc, IVector **ppEndLoc)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing>		pThis;
	CComPtr<IThing>		pObjCont;
	CComPtr<IVector>	pNewLoc;
	float				x,y,z;
    HRESULT             hr = S_OK;
	
	if ((ppEndLoc==NULL) || (pObject==NULL) || (pStartLoc==NULL))
    {
    	VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::ValidatePosition: NULL pointer\n");
		return ReportMMError(E_POINTER);
    }

	*ppEndLoc = NULL;

	// 1.  make sure they are both peers in me
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;
	if (FAILED(hr = pObject->get_Container(&pObjCont)))
		return hr;

	pStartLoc->get(&x,&y,&z);

	if (FAILED(hr = CreateVector(x, y, z, &pNewLoc)))
		return hr;

	// REVIEW: finish this!
	if (pObjCont == pThis)
	{
		// 1.  stick it to the floor
		// 2.  check collisions by using a sphere of increasing size...
	}
	else
	{
		// 1.  for now, drop it into the middle of the room
	}

	*ppEndLoc = pNewLoc;

    return hr;
}

STDMETHODIMP CMultimediaExemplarObject::OnFogColorChanged(long lHint, VARIANT varHintData)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CComPtr<IThing>     pThis;	
	CComPtr<IVWFrame>	pvwfFrame;
	CComBSTR	        bstrFogColor = "FogColor";
	CComVariant         varFogColor;
	HRESULT             hr = S_OK;

	if (lHint == thPut) 
    {
		if (FAILED(hr = m_pWorld->get_This(&pThis)))
			return hr;
        ASSERT(pThis);

		if (FAILED(hr = _GetFrameHelper(pThis, &pvwfFrame)) || !pvwfFrame)
        {
    	    VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::OnFogColorChanged: Unable to retrieve geometry\n");
		    return ReportMMError(hr);
        }
		
		if (FAILED(hr = pThis->get_Property(bstrFogColor, &varFogColor)))
			return hr;

		if (FAILED(hr = varFogColor.ChangeType(VT_I4)))
        {
    	    VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::OnFogColorChanged: fog color not an integer\n");
		    return ReportMMError(hr);
        }

		// break out and normalize components
		float flRed, flGreen, flBlue;
		flRed =   (float) ((varFogColor.lVal & 0xFF0000)>>16)/255.0f;
		flGreen = (float) ((varFogColor.lVal & 0x00FF00)>>8 )/255.0f;
		flBlue =  (float) ((varFogColor.lVal & 0x0000FF)    )/255.0f;

		pvwfFrame->SetSceneFogColor(flRed,flGreen,flBlue);
	}

	return hr;
}

STDMETHODIMP CMultimediaExemplarObject::OnFogStartChanged(long lHint, VARIANT varHintData)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CComPtr<IThing>     pThis;	
	CComPtr<IVWFrame>   pvwfFrame;
	CComBSTR	bstrFogStart	= "FogStart";
	CComBSTR	bstrFogEnd		= "FogEnd";
	CComVariant varFogStart;
	CComVariant varFogEnd;
	HRESULT hr = S_OK;

	if (lHint == thPut) 
    {
		if (FAILED(hr = m_pWorld->get_This(&pThis)))
			return hr;

		if (FAILED(hr = _GetFrameHelper(pThis, &pvwfFrame)) || !pvwfFrame)
        {
    	    VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::OnFogStartChanged: Unable to retrieve geometry\n");
		    return ReportMMError(hr);
        }
		
		if (FAILED(hr = pThis->get_Property(bstrFogStart, &varFogStart)))
			return hr;
		if (FAILED(hr = pThis->get_Property(bstrFogEnd, &varFogEnd)))
			return hr;
		pvwfFrame->SetSceneFogParams(varFogStart.iVal,varFogEnd.iVal);
	}

	return hr;
}

STDMETHODIMP CMultimediaExemplarObject::OnFogEndChanged(long lHint, VARIANT varHintData)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CComPtr<IThing>     pThis;	
	CComPtr<IVWFrame>   pvwfFrame;
	CComBSTR	    bstrFogStart	= "FogStart";
	CComBSTR	bstrFogEnd		= "FogEnd";
	CComVariant varFogStart;
	CComVariant varFogEnd;
	HRESULT hr = S_OK;

	if (lHint == thPut) 
    {
		if (FAILED(hr = m_pWorld->get_This(&pThis)))
			return hr;

		if (FAILED(hr = _GetFrameHelper(pThis, &pvwfFrame)) || !pvwfFrame)
        {
    	    VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::OnFogEndChanged: Unable to retrieve geometry\n");
		    return ReportMMError(hr);
        }
		
		if (FAILED(hr = pThis->get_Property(bstrFogStart, &varFogStart)))
			return hr;
		if (FAILED(hr = pThis->get_Property(bstrFogEnd, &varFogEnd)))
			return hr;
		pvwfFrame->SetSceneFogParams(varFogStart.iVal,varFogEnd.iVal);
	}

	return hr;
}

STDMETHODIMP CMultimediaExemplarObject::OnFogChanged(long lHint, VARIANT varHintData)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	CComPtr<IThing>     pThis;	
	CComPtr<IVWFrame>   pvwfFrame;
	CComBSTR	bstrFog			= "Fog";
	CComVariant varFog;
	HRESULT hr = S_OK;

	if (lHint == thPut) 
    {
		if (FAILED(hr = m_pWorld->get_This(&pThis)))
			return hr;

		if (FAILED(hr = _GetFrameHelper(pThis, &pvwfFrame)) || !pvwfFrame)
        {
    	    VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::OnFogChanged: Unable to retrieve geometry\n");
		    return ReportMMError(hr);
        }
		
		if (SUCCEEDED(hr = pThis->get_Property(bstrFog, &varFog)))			
			pvwfFrame->SetSceneFogEnable(varFog.boolVal);
	}

	return hr;
}


STDMETHODIMP CMultimediaExemplarObject::OnRoomLoadGeometry(IVWFrame *pNewFrame)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CComPtr<IThing>		pThis;
	CComPtr<IVWFrame>   pvwfFrame;
	HRESULT		hr = S_OK;
	CComBSTR	bstrFog = "Fog";
	CComBSTR	bstrFogStart = "FogStart";
	CComBSTR	bstrFogEnd = "FogEnd";
	CComBSTR	bstrFogColor = "FogColor";
	VARIANT_BOOL	bFog = VARIANT_FALSE;
	short	sFogStart = 0;
	short	sFogEnd = 0;
	long	lFogColor = 0;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;	

	CComVariant varFrame((IDispatch *) pNewFrame);
	CComDISPPARAMS dispSuper(1, varFrame);

	if (FAILED(hr = pThis->InvokeSuperMethodExt((DISPPARAMS *) dispSuper, NULL)))
		return hr;

	//get values from the fog related props 

	if (FAILED(hr = pThis->get_BOOL(bstrFog, &bFog)))
		return hr;
	if (FAILED(hr = pThis->get_Short(bstrFogStart, &sFogStart)))	
		return hr;
	if (FAILED(hr = pThis->get_Short(bstrFogEnd, &sFogEnd)))	
		return hr;
	if (FAILED(hr = pThis->get_Long(bstrFogColor, &lFogColor)))	
		return hr;

	//get the correct geometryframe from the room and call fog methods on it

	if (FAILED(hr = _GetFrameHelper(pThis, &pvwfFrame)))
		return hr;

	if (pvwfFrame == NULL)
    {
        VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::OnRoomLoadGeometry: NULL pointer\n");
		return ReportMMError(E_POINTER);
    }

	pvwfFrame->SetSceneFogParams(sFogStart,sFogEnd);
	
	float flRed, flGreen, flBlue;

	flRed =   (float) ((lFogColor & 0xFF0000)>>16)/255.0f;
	flGreen = (float) ((lFogColor & 0x00FF00)>>8 )/255.0f;
	flBlue =  (float) ((lFogColor & 0x0000FF)    )/255.0f;

	pvwfFrame->SetSceneFogColor(flRed,flGreen,flBlue);			
	pvwfFrame->SetSceneFogEnable(bFog);

	return S_OK;
}



STDMETHODIMP CMultimediaExemplarObject::OnActivate(IThing *pRoom)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HRESULT hr = S_OK;
	CComPtr<IThing> pThing;
	CComPtr<IThing> pThing2;
	CComPtr<IThing> pGlobal;
	CComPtr<IPropertyList> pRoomList, pAvList;
	long lCountAvatars = 0L;
	long lCountRooms = 0l;
	long i,j;
	long lConnectedAvatars = 0l;
	VARIANT_BOOL bAvatarConnected;

	static CComBSTR bstrAvatars("Avatars");
	static CComBSTR bstrRooms("Rooms");
	static CComBSTR bstrIsConnected("IsConnected");

	if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pGlobal->get_ObjectProperty(bstrRooms, (IObjectProperty **) &pRoomList))|| !pRoomList)
		goto ERROR_ENCOUNTERED;

	pRoomList->get_Count(&lCountRooms);

	for (i = 0; i < lCountRooms; i++)
	{
		hr = pRoomList->get_ObjectProperty(i, (IObjectProperty**)&pThing);
		if (SUCCEEDED(hr) && pThing != NULL)
		{
			if (FAILED(hr = pThing->get_ObjectProperty(bstrAvatars, (IObjectProperty **)&pAvList))|| !pAvList)
				goto ERROR_ENCOUNTERED;

			pAvList->get_Count(&lCountAvatars);

			for (j = 0; j < lCountAvatars; j++)
			{
				hr = pAvList->get_ObjectProperty(j, (IObjectProperty**)&pThing2);
				if (SUCCEEDED(hr) && pThing2 != NULL)
				{
					pThing2->get_BOOL(bstrIsConnected, &bAvatarConnected);
					// found a connected avatar, world is inhabited
					if (bAvatarConnected == VARIANT_TRUE)
					{
						lConnectedAvatars++;
					}
				}
				pThing2.Release();
			}
			pAvList.Release();
		}
		pThing.Release();
	}

	if (lConnectedAvatars < 1)
	{
		//world has just loaded, write the start ticks time on the global
		if (FAILED(hr = pGlobal->put_Property( CComBSTR("SystemStartTicks"), CComVariant((long)GetTickCount()) )))
			goto ERROR_ENCOUNTERED;		

		long lCurrentId, k;
		CComPtr<IThing> pObject;
//		CComBSTR bstrObjectName;
//		CString strObjectName;
		VARIANT_BOOL	bExemplar;
		//wake up all the server-side animation callbacks and start them going again
		if (FAILED(hr = pGlobal->get_Long(CComBSTR("CurrentID"), &lCurrentId)))
			goto ERROR_ENCOUNTERED;

		for (k = 2; k < lCurrentId; k++)
		{
			if (FAILED(hr = m_pWorld->get_ObjectByID( k, &pObject )))
				continue;

			if (FAILED(hr = m_pWorld->IsExemplar(pObject, &bExemplar)))
					goto ERROR_ENCOUNTERED;

			if (bExemplar == VARIANT_FALSE)
			{
				//ok, this is an instance derived from Thing, so lets look at its possible animations
				hr = RecoverAnimationCallBacks(pObject, CComBSTR("File"));
				hr = RecoverAnimationCallBacks(pObject, CComBSTR("Position"));									
				hr = RecoverAnimationCallBacks(pObject, CComBSTR("Direction"));
				hr = RecoverAnimationCallBacks(pObject, CComBSTR("Up"));
				hr = RecoverAnimationCallBacks(pObject, CComBSTR("Scale"));
				hr = RecoverAnimationCallBacks(pObject, CComBSTR("LightColor"));
				hr = RecoverAnimationCallBacks(pObject, CComBSTR("LightAttenuation"));
			}

			pObject.Release();
		}

	}

	ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWMMROOM", VWT_ERROR, "CMultimediaExemplarObject::OnActivate: Error \n");
	}

	return hr;	

}


STDMETHODIMP CMultimediaExemplarObject::RecoverAnimationCallBacks(IThing *pThing, BSTR bstrPropertyName)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HRESULT			hr = S_OK;
	long			lStartTime, lOptions, lSystemTime = 0l;
	unsigned long	ulEndTime = 0;
	float			fTimeScale, fStartRange, fEndRange, fIndex;
	CComBSTR		bstrPropertyAnimProperties(bstrPropertyName);
	CComBSTR		bstrPropertyAnimOptions(bstrPropertyName);
	CComBSTR		bstrPropertyAnimStartTime(bstrPropertyName);
	CComBSTR		bstrPropertyAnimIndex(bstrPropertyName);
	CComBSTR		bstrPropertyAnimTimeScale(bstrPropertyName);
	CComBSTR		bstrPropertyAnimRangeStart(bstrPropertyName);
	CComBSTR		bstrPropertyAnimRangeEnd(bstrPropertyName);
	CComBSTR		bstrPropertyAnimCallBack(bstrPropertyName);
	CComBSTR        bstrPropertyAnimFinalValue(bstrPropertyName);
	CComBSTR		bstrPropertyAnimPath(bstrPropertyName);
	CComBSTR		bstrPropertySetPropAnimation(bstrPropertyName);
	CComBSTR		bstrCallBack;
	static CString			strFile("File");
	static CString			strPosition("Position");
	static CString			strDirection("Direction");
	static CString			strUp("Up");
	static CString			strScale("Scale");
	static CString			strLightColor("LightColor");
	static CString			strLightAttenuation("LightAttenuation");

	CString					strPropertyName(bstrPropertyName);
	CComVariant				var, varTrue;
	CComPtr<IPropertyMap>	pAnimPropertiesMap;
	CComPtr<IVector>		pFinalVector;
	float					fFinalIndex;
	CComPtr<IVWAnimator>	pVWAnimator;
	CComPtr<IVWAnimation>	pAnimation, pPropSetAnimation;
	static CComBSTR			bstrAnimator("Animator");
	CComBSTR				bstrName;
	CComBSTR				bstrEmpty("");
	unsigned long			ulDuration;
	bstrPropertyAnimProperties.Append("AnimProperties");
	bstrPropertyAnimOptions.Append("AnimOptions");
	bstrPropertyAnimStartTime.Append("AnimStartTime");
	bstrPropertyAnimIndex.Append("AnimIndex");
	bstrPropertyAnimTimeScale.Append("AnimTimeScale");
	bstrPropertyAnimRangeStart.Append("AnimRangeStart");
	bstrPropertyAnimRangeEnd.Append("AnimRangeEnd");
	bstrPropertyAnimCallBack.Append ("AnimCallBack");
	bstrPropertyAnimFinalValue.Append ("AnimFinalValue");
	bstrPropertyAnimPath.Append("AnimPath");
	bstrPropertySetPropAnimation.Append("SetPropAnimation");

	varTrue.vt = VT_BOOL;
	varTrue.boolVal = VARIANT_TRUE;

	if (FAILED(hr = pThing->get_Property(bstrPropertyAnimProperties, &var)))
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
			hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimStartTime, &var);
			
			lStartTime = CastToLong(var);
			var.Clear();
			hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimOptions, &var);
			lOptions = CastToLong(var);
			var.Clear();
			hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimTimeScale, &var);
			fTimeScale = CastToFloat(var);
			var.Clear();
			hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimRangeStart, &var);
			fStartRange = CastToFloat(var);
			var.Clear();
			hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimRangeEnd, &var);
			fEndRange = CastToFloat(var);
			var.Clear();
			hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimIndex, &var);
			fIndex = CastToFloat(var);
			var.Clear();
			hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimCallBack, &var);
			bstrCallBack = var.bstrVal;
			var.Clear();
			hr = pAnimPropertiesMap->get_Property( bstrPropertyAnimFinalValue, &var);
			if (var.vt != VT_DISPATCH)
				fFinalIndex = CastToFloat(var);
			else
			{	
				if (FAILED(hr = var.pdispVal->QueryInterface(IID_IVector,(void**)&pFinalVector)))
					pFinalVector->set(0.0f, 1.0f, 1.0f);
			}
			var.Clear();
		}
	}
	if ((lStartTime > 0) && (lOptions & 1))
	{
		if (lStartTime > CurrentTime())
		{
//				VWREPORT(m_pWorld, VWR_ERROR,  "CMultimediaExemplarObject::RecoverAnimationCallBacks: World Time has wrapped or a world time error has occurred...\n");
//				VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::RecoverAnimationCallBacks: World Time has wrapped or a world time error has occurred...\n\n");
		}
		ULongAdd((unsigned long)lStartTime, ((unsigned long)(((fIndex)>(fStartRange)?(fEndRange-fIndex):(fEndRange-fStartRange))/fTimeScale)), &ulEndTime);

		if ((unsigned long)CurrentTime() < ulEndTime)
		{
			//calculate a good index from the elapsed time and timescale				
			ULongSubtract ( ulEndTime, (unsigned long)CurrentTime(), &ulDuration);	
		}
		else 
			ulDuration = 0;

		//get the animator tool
		if (FAILED(hr = m_pWorld->get_Tool(bstrAnimator, (IUnknown**)&pVWAnimator)))
			goto ERROR_ENCOUNTERED;

		if (FAILED(hr = pVWAnimator->CreateClosedExt(	((int)(ulDuration)), 
														((int)1), 
														pThing, 
														CComBSTR("FinishAnimation"),
														CComDISPPARAMS(	5,
																		CComVariant(varTrue),
																		CComVariant(varTrue),
																		CComVariant(bstrPropertyName),
																		((bstrCallBack.Length()&&(IsMethodThisSide(pThing, bstrCallBack))) ? CComVariant(bstrCallBack) : CComVariant(CComBSTR("")) ),
																		((strPropertyName.CompareNoCase(strFile)!=0) ? CComVariant((IDispatch*)pFinalVector) : CComVariant(fFinalIndex) ),
																		NULL),
														&pPropSetAnimation )))
			goto ERROR_ENCOUNTERED;	
		
		//store the property set callback animation so we can kill it if we later interrupt this animation from code
		if (FAILED(hr = pThing->put_Property( bstrPropertySetPropAnimation, CComVariant((IDispatch*)pPropSetAnimation) )))
			goto ERROR_ENCOUNTERED;	

	}

ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWMMROOM", VWT_ERROR, "CMultimediaExemplarObject::RecoverAnimationCallBacks: Error \n");
		VWTRACE(m_pWorld, "VWMMROOM", VWT_ERROR, strPropertyName);
		VWTRACE(m_pWorld, "VWMMROOM", VWT_ERROR, "\n");
	}

	return hr;

}











STDMETHODIMP CMultimediaExemplarObject::OnDeactivate(IThing *pRoom)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HRESULT hr = S_OK;
	CComPtr<IThing> pThing, pThing2, pGlobal;
	CComPtr<IPropertyList> pRoomList, pAvList;
	long lCountAvatars = 0L;
	long lCountRooms = 0l;
	long i,j;
	VARIANT_BOOL bWorldInhabited = VARIANT_FALSE;
	VARIANT_BOOL bAvatarConnected;

	static CComBSTR bstrAvatars("Avatars");
	static CComBSTR bstrRooms("Rooms");
	static CComBSTR bstrIsConnected("IsConnected");

	if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pGlobal->get_ObjectProperty(bstrRooms, (IObjectProperty **) &pRoomList))|| !pRoomList)
		goto ERROR_ENCOUNTERED;

	pRoomList->get_Count(&lCountRooms);

	for (i = 0; i < lCountRooms; i++)
	{
		hr = pRoomList->get_ObjectProperty(i, (IObjectProperty**)&pThing);
		if (SUCCEEDED(hr) && pThing != NULL)
		{
			if (FAILED(hr = pThing->get_ObjectProperty(bstrAvatars, (IObjectProperty **)&pAvList))|| !pAvList)
				goto ERROR_ENCOUNTERED;

			pAvList->get_Count(&lCountAvatars);

			for (j = 0; j < lCountAvatars; j++)
			{
				hr = pAvList->get_ObjectProperty(j, (IObjectProperty**)&pThing2);
				if (SUCCEEDED(hr) && pThing2 != NULL)
				{
					pThing2->get_BOOL(bstrIsConnected, &bAvatarConnected);
					pThing2.Release();
					// found a connected avatar, world is inhabited
					if (bAvatarConnected == VARIANT_TRUE)
					{
						bWorldInhabited = VARIANT_TRUE;
						break;
					}
				}
			}
			pAvList.Release();
		}
		pThing.Release();
		if (bWorldInhabited == VARIANT_TRUE)
			break;
	}

	if (bWorldInhabited == VARIANT_FALSE)
	{
		//world will unload, write the current time on the global
		if (FAILED(hr = pGlobal->put_Property( CComBSTR("SystemTime"), CComVariant(CurrentTime()) )))
			goto ERROR_ENCOUNTERED;	
		if (FAILED(hr = pGlobal->put_Property( CComBSTR("SystemStartTicks"), CComVariant((long)GetTickCount()) )))
			goto ERROR_ENCOUNTERED;	
	}

	ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWMMROOM", VWT_ERROR, "CMultimediaExemplarObject::OnDeactivate: Error \n");
	}

	return hr;
}

