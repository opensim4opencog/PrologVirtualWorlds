// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// mmavatar.cpp : Avatar extensions

#include "stdafx.h"
#include <d3drmwin.h>
#include <d2d.h>
#include <syshelp.h>
#include <mmhelp.h>
#include "profhelp.h"
#include <vwmmex.h>
#include <vwidata.h>
#include <vwanim.h>
#include "vwutils.h"
#include "vwmmexo.h"

///////////////////////////////////////////////////////////////////////
// Parameters

///////////////////////////////////////////////////////////////////////
// Helpers

extern HRESULT	_GetFrameHelper(IThing *pThing, IVWFrame **ppvwf);

///////////////////////////////////////////////////////////////////////
// Install 

HRESULT CMultimediaExemplarObject::InstallAvatarExtensions(IModule* pModule)
{
	CMethodInfo rgMethod[] =
	{
		{	METHOD_CLIENT,	"OnWornAccessoriesChanged",		NULL,	PSBIT_EVENTHANDLER,				PS_ALLEXECUTEMETHOD },
		{	METHOD_CLIENT,	"LookToward",					NULL,	PSBIT_HIDDEN,					PS_EXEMPLARDEFAULTMETHOD },
		{	METHOD_CLIENT,	"ObjectInView",					NULL,	PSBIT_HIDDEN,					PS_EXEMPLARDEFAULTMETHOD },
		{	METHOD_CLIENT,	"SetFieldOfView",				NULL,	PSBIT_HIDDEN,					PS_EXEMPLARDEFAULTMETHOD },
		{	METHOD_CLIENT,	"UnWear",						NULL,	PSBIT_NONE,						PS_EXEMPLARDEFAULTMETHOD },
		{	METHOD_CLIENT,	"Wear",							NULL,	PSBIT_NONE,						PS_EXEMPLARDEFAULTMETHOD },
		{	METHOD_CLIENT,	"OnContentSystemLeave",			"OnContentLeave", PSBIT_SYSTEMEVENTHANDLER,	PS_EVENTHANDLER },
		{	METHOD_CLIENT,	"ThumbnailFileName",			NULL,	PSBIT_HIDDEN,					PS_EXEMPLARDEFAULTMETHOD },
		{	METHOD_CLIENT,	"Find",							NULL,	PSBIT_HIDDEN,					PS_EXEMPLARDEFAULTMETHOD },
		{	METHOD_SERVER,	"OnCreate",						NULL,	PSBIT_EVENTHANDLER,				PS_EXEMPLARDEFAULTMETHOD },
		{	METHOD_SERVER,	"OnConnect",					NULL,	PSBIT_EVENTHANDLER,				PS_EXEMPLARDEFAULTMETHOD },
		{	METHOD_SERVER,	"AddDefaultKeys",				NULL,	PSBIT_HIDDEN,					PS_EXEMPLARDEFAULTMETHOD },
		{   METHOD_SERVER,	"Teleport",						NULL,	PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,PS_EXEMPLARDEFAULTMETHOD }	

	};
	int cMethod = sizeof(rgMethod)/sizeof(rgMethod[0]);
    int iMethod;

	// find the Avatar exemplar
	CComPtr<IThing>			pAvatarEx;
	CComPtr<IPropertyList>	pList;
	CComPtr<IPropertyMap>	pMap;
	CComVariant				var((double) .9);
	CComPtr<IPropertyList>	pMenu;
	CComPtr<IMenuItem>		pMenuItem;
	CComBSTR				bstrName;
	CComBSTR				bstrNameInternal;
	CComBSTR                bstrGeometry;
	CComBSTR				bstrMotion;
	CComBSTR				bstrApproach;
    HRESULT                 hr = S_OK;

	if (FAILED(hr = m_pWorld->get_Exemplar(CComBSTR("Avatar"), &pAvatarEx)))
		goto ERROR_ENCOUNTERED;

	// add in methods
	for (iMethod=0; iMethod<cMethod; iMethod++)
	{
		bstrName = rgMethod[iMethod].pszName;

		if (rgMethod[iMethod].pszNameInternal == NULL)
			bstrNameInternal = bstrName;
		else
			bstrNameInternal = rgMethod[iMethod].pszNameInternal;
		
		if (FAILED(hr = pAvatarEx->CreateAndAddMethodExt(rgMethod[iMethod].lFlags,
													pModule,
													bstrNameInternal,
													bstrName,
													rgMethod[iMethod].psbits,
													rgMethod[iMethod].permissions)))
            goto ERROR_ENCOUNTERED;
	}

	// add in properties
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAvatarEx->AddPropertyExt(	CComBSTR("CurrentEmblem"), 
											CComVariant((IDispatch *) pList), 
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTCOLLECTION, 
											VT_DISPATCH, IID_IPropertyList, NULL)))
		goto ERROR_ENCOUNTERED;
	
    pList.Release();
	
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAvatarEx->AddPropertyExt(	CComBSTR("WornItems"), 
											CComVariant((IDispatch *) pList), 
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTCOLLECTION, 
											VT_DISPATCH, IID_IPropertyList, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pAvatarEx->AddPropertyExt(	CComBSTR("FieldOfView"),
											CComVariant((long) 120),
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_I4, IID_IDispatch, NULL)))
		goto ERROR_ENCOUNTERED;


	if (FAILED(hr = pAvatarEx->AddPropertyExt(	CComBSTR("EyeLevel"), 
											var, 
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN, 
											PS_EXEMPLARDEFAULTPROPERTY, 
											VT_R8, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = CreatePropertyMap(m_pWorld, &pMap)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAvatarEx->AddPropertyExt(	CComBSTR("WornAccessories"), 
											CComVariant((IDispatch*) pMap),
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN, 
											PS_EXEMPLARDEFAULTCOLLECTION, 
											VT_DISPATCH, IID_IPropertyMap, NULL)))
		goto ERROR_ENCOUNTERED;

	// Set Contents occluded to be true by default for avatars
	if (FAILED(hr = pAvatarEx->put_BOOL(CComBSTR("IsContentsVisible"), VARIANT_FALSE)))
		goto ERROR_ENCOUNTERED;

	// amend the "PeerMenu" menu
	if (FAILED(hr = pAvatarEx->get_ObjectPropertyExt(CComBSTR("PeerMenu"), IID_IPropertyList, (IObjectProperty **) &pMenu)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = CreateMenuItemExt(m_pWorld, CComBSTR("Approach ->>"), 0, NULL, CComBSTR("UpdateIsApproachable"), CComBSTR("Approach"), NULL, &pMenuItem)))
		goto ERROR_ENCOUNTERED;
//		if (FAILED(hr = pMenuItem->put_SortOrder(MI_SORT_PRIMARY+10)))
//  		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMenuItem->put_AddTargetParameter(VARIANT_TRUE)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMenu->AddObjectProperty(pMenuItem)))
		goto ERROR_ENCOUNTERED;
	pMenuItem.Release();
	pList.Release();

//	REVIEW [TO]:	Changing the avatar geometry editmap entry per spec
//	Leaving nothing after pipe.. so we override mmthing map entry

	// amend edit maps
//	bstrGeometry = "Geometry|Edit the geometry of the object|avageom.htm";
	bstrGeometry = "Geometry|";
	bstrMotion = "Motion|";
	bstrApproach = "Approach|";

	// create the avatar geometry page
	if (FAILED(hr = pAvatarEx->get_ObjectProperty(CComBSTR("EndUserEditMap"), (IObjectProperty **) &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrGeometry)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrMotion)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrApproach)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAvatarEx->put_ObjectProperty(CComBSTR("EndUserEditMap"), (IObjectProperty *) pList)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

	if (FAILED(hr = pAvatarEx->get_ObjectProperty(CComBSTR("OwnerEditMap"), (IObjectProperty **) &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrGeometry)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrMotion)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrApproach)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAvatarEx->put_ObjectProperty(CComBSTR("OwnerEditMap"), (IObjectProperty *) pList)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

	if (FAILED(hr = pAvatarEx->get_ObjectProperty(CComBSTR("BuilderEditMap"), (IObjectProperty **) &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrGeometry)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrMotion)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrApproach)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAvatarEx->put_ObjectProperty(CComBSTR("BuilderEditMap"), (IObjectProperty *) pList)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

	if (FAILED(hr = pAvatarEx->get_ObjectProperty(CComBSTR("AdvancedEditMap"), (IObjectProperty **) &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrGeometry)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrMotion)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrApproach)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(CComBSTR("Wizard|Edit a user's privileges|wizard.htm"))))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAvatarEx->put_ObjectProperty(CComBSTR("AdvancedEditMap"), (IObjectProperty *) pList)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

    return S_OK;

ERROR_ENCOUNTERED:
	
	VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::InstallAvatarExtensions: Error encountered: %x\n", hr);
    return hr;
}


//******************************************************
// OnContentLeave
// Called when this avatar or a peer drops something

STDMETHODIMP CMultimediaExemplarObject::OnContentLeave(IThing *pWhere, IThing *pWhat)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	VARIANT_BOOL    bWorn = VARIANT_FALSE;
	CComPtr<IThing> pThis;
    HRESULT         hr = S_OK;

    if (pWhere == NULL)
    {
        VWTRACE(m_pWorld, "VWMMAVATAR", VWT_ERROR, "CMultimediaExemplarObject::OnContentLeave: NULL pointer\n");
        return ReportMMError(E_POINTER);
    }

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;
	
	// See if object is stub before checking if artifact
	// (If a stub, you are on a remote client when object is first being created and dropped within another user)
	if (pWhat)
	{
		VARIANT_BOOL bIsStub = VARIANT_FALSE;

		if (FAILED(hr = pWhat->get_IsStub(&bIsStub)))
			return hr;

		// if object is being worn, must first remove it
		if (bIsStub == VARIANT_FALSE)
		{
			VARIANT_BOOL boolIsArtifact = VARIANT_FALSE;

			if (FAILED(hr = pWhat->IsOfType(CComBSTR("Artifact"), &boolIsArtifact)))
    			return hr;

			if (boolIsArtifact)
			{
				if (FAILED(hr = pWhat->get_BOOL(CComBSTR("IsWorn"), &bWorn)))
					return hr;

				if (bWorn==VARIANT_TRUE)
				{
					if (FAILED(hr = pWhere->InvokeMethodExt(CComBSTR("UnWear"), 
															CComDISPPARAMS(1, CComVariant((IDispatch*)pWhat)),
															NULL)))
						return hr;
				}
			}
		}
	}

	// need to invoke base class method
	hr = pThis->InvokeSuperMethodExt(CComDISPPARAMS(2, CComVariant((IDispatch*)pWhere), 
                                     CComVariant((IDispatch*)pWhat)), NULL);

	return hr;
}

//******************************************************
// LookToward
// Turns the avatar to look at the object.  This typically
// does not move the avatar, but it might.

STDMETHODIMP CMultimediaExemplarObject::LookToward(IThing * pThing)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// REVIEW: need simple implementation
	return S_OK;
}

//******************************************************
// ObjectInView
// Returns true if the avatar can see the object.  This
// basically takes into account containment and fieldofview
// and cell-level blocking.  It is NOT pixel-accurate.

STDMETHODIMP CMultimediaExemplarObject::ObjectInView(IThing * pThing, VARIANT_BOOL * pbool)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// REVIEW: need simple implementation
	return S_OK;
}

//******************************************************
// SetFieldOfView
// Sets the field of view of the avatar.  It is up to
// the client to use this information....

STDMETHODIMP CMultimediaExemplarObject::SetFieldOfView(int iNewView)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
    HRESULT         hr = S_OK;

	if (SUCCEEDED(hr = m_pWorld->get_This(&pThis)))
    {
	    // REVIEW: check security
	    // check security

        // set the prop
        hr = pThis->put_Long(CComBSTR("FieldOfView"), iNewView);

	    // REVIEW: fire a UI event to tell the renderer
    }

	return hr;
}

//******************************************************
// Wear
// Wears the object.  The object must be an unwear
// wearable artifact in your inventory....

STDMETHODIMP CMultimediaExemplarObject::Wear(IThing * pThing)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing>				pThis;
	CComPtr<IThing>				pThingCont;
	CComPtr<IPropertyList>		pWornList;
	CComPtr<IPropertyMap>		pWornAccessories;
	CComPtr<IVWAccessoryData>	pVWAccessoryData;

	VARIANT_BOOL				bVal;
	CComBSTR					bstrName;
	long						lID;
	char						buff[10];
    HRESULT                     hr = S_OK;

    if (pThing == NULL)
    {
        VWTRACE(m_pWorld, "VWMMAVATAR", VWT_ERROR, "CMultimediaExemplarObject::Wear: NULL pointer\n");
        return ReportMMError(E_POINTER);
    }

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	// 1.  make sure object is in my inventory
	if (FAILED(hr = pThing->get_Container(&pThingCont)))
		return hr;
	if (pThingCont != pThis)
    {
        VWTRACE(m_pWorld, "VWMMAVATAR", VWT_IMPORTANT, "CMultimediaExemplarObject::Wear: not in contents\n");
        return ReportMMError(VWMM_E_NOTINCONTENTS);
    }
	
	// 2.  make sure it is wearable
	if (FAILED(hr = pThing->get_BOOL(CComBSTR("IsAccessory"), &bVal)))
		return hr;
	if (bVal != VARIANT_TRUE)
    {
        VWTRACE(m_pWorld, "VWMMAVATAR", VWT_ERROR, "CMultimediaExemplarObject::Wear: not wearable\n");
        return ReportMMError(VWMM_E_NOTWEARABLE);
    }

	// 3.  make sure it is not worn
	if (FAILED(hr = pThing->get_BOOL(CComBSTR("IsWorn"), &bVal)))
		return hr;
	if (bVal == VARIANT_TRUE)
		// REVIEW: should this return an error
        return S_OK;

	// 4.  Prepare for wearing
	if (FAILED(hr = pThing->get_String(CComBSTR("AccessoryGraphic"), &bstrName.m_str)))
		return hr;
	if (FAILED(hr = pThing->get_ID(&lID)))
		return hr;

	// 5.  Wear the accessory
	if (FAILED(hr = pThis->get_ObjectPropertyExt(CComBSTR("WornAccessories"), IID_IPropertyMap, (IObjectProperty**)&pWornAccessories)))
		return hr;
	if (FAILED(hr = CreateAccessoryData(bstrName, 0, 0, &pVWAccessoryData)))
		return hr;
	if (FAILED(hr = pWornAccessories->put_ObjectProperty(CComBSTR(ltoa(lID, buff, 10)), pVWAccessoryData)))
		return hr;

	// 6. Tell the item it's being worn
	if (FAILED(hr = pThing->put_BOOL(CComBSTR("IsWorn"), VARIANT_TRUE)))
		return hr;
	
	// 7. Add it to the list of worn object
	hr = pThis->get_ObjectPropertyExt(CComBSTR("WornItems"), IID_IPropertyList, (IObjectProperty **)&pWornList);
    pWornList->Add(CComVariant((IDispatch *) pThing));
	
	// 8. Fire the event
	pThing->FireEventExt(CComBSTR("Worn"), NULL, FIREEVENT_DEFAULT);

	return hr;
}


//******************************************************
// UnWear
// Wears the object, if it is being worn..

STDMETHODIMP CMultimediaExemplarObject::UnWear(IThing * pThing)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing>				pThis;
	CComPtr<IThing>				pThingCont;
	CComPtr<IPropertyList>		pWornList;
	CComPtr<IPropertyMap>		pWornAccessories;
	VARIANT_BOOL				bVal;
	long						lID;
	char						buff[10];
    HRESULT                     hr = S_OK;

    if (pThing == NULL)
    {
        VWTRACE(m_pWorld, "VWMMAVATAR", VWT_ERROR, "CMultimediaExemplarObject::UnWear: NULL pointer\n");
        return ReportMMError(E_POINTER);
    }

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	// 1.  make sure object is in my inventory
	if (FAILED(hr = pThing->get_Container(&pThingCont)))
		return hr;
	if (pThingCont != pThis)
    {
        VWTRACE(m_pWorld, "VWMMAVATAR", VWT_IMPORTANT, "CMultimediaExemplarObject::UnWear: not in contents\n");
        return ReportMMError(VWMM_E_NOTINCONTENTS);
    }
	
	// 2.  make sure it is being worn
	if (FAILED(hr = pThing->get_BOOL(CComBSTR("IsWorn"), &bVal)))
		return hr;
	if (bVal != VARIANT_TRUE)
    {
        VWTRACE(m_pWorld, "VWMMAVATAR", VWT_ERROR, "CMultimediaExemplarObject::UnWear: not worn\n");
        return ReportMMError(VWMM_E_NOTWORN);
    }

	// 3.  Prepare for unwearing
	if (FAILED(hr = pThing->get_ID(&lID)))
		return hr;

	// 4.  Remove the worn accessory
	if (FAILED(hr = pThis->get_ObjectPropertyExt(CComBSTR("WornAccessories"), IID_IPropertyMap, (IObjectProperty**)&pWornAccessories)))
		return hr;
	if (FAILED(hr = pWornAccessories->Remove(CComBSTR(ltoa(lID, buff, 10)))))
		return hr;

	// 5. Tell the item it's being unworn
	if (FAILED(hr = pThing->put_BOOL(CComBSTR("IsWorn"), VARIANT_FALSE)))
		return hr;
	
	// 6. Remove it from the list of worn objects
	hr = pThis->get_ObjectPropertyExt(CComBSTR("WornItems"), IID_IPropertyList, (IObjectProperty **)&pWornList);
	pWornList->Remove(CComVariant((IDispatch *)pThing));
	
	// 7. Fire the event
	pThing->FireEventExt(CComBSTR("UnWorn"), NULL, FIREEVENT_DEFAULT);

	return hr;

}

//******************************************************
// OnWornAccessoriesChanged
// Called whenever the worn accessories change

STDMETHODIMP CMultimediaExemplarObject::OnWornAccessoriesChanged(long lHint, VARIANT varHintData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing>				pThis;
	CComPtr<IVWGeometrySprite>	pVWGeometrySprite;
	CComPtr<IDirect2DRMFrame>	pFrame, pScene;
	CComPtr<IPropertyMap>		pWornAccessories;
	CComPtr<IVWAccessoryData>	pVWAccessoryData;
	CComBSTR					bstrAccessoryGraphic;
	float						fPosX, fPosY;
	static CComBSTR				bstrGeometry("Geometry");
    HRESULT                     hr = S_OK;

	// skip add/remove notifications
	if (lHint != thAdd && lHint != thRemove)
	{
		if (FAILED(hr = m_pWorld->get_This(&pThis)))
    		return hr;

		// Get the thing's geometry property (make sure its a sprite).
		if (FAILED(hr = pThis->get_ObjectPropertyExt(bstrGeometry, IID_IVWGeometrySprite, (IObjectProperty**)&pVWGeometrySprite)))
		    return hr;

        if (pVWGeometrySprite == NULL)
            return S_OK;

		// Get the property list.
		if (FAILED(hr = pThis->get_ObjectPropertyExt(CComBSTR("WornAccessories"), IID_IPropertyMap, (IObjectProperty**)&pWornAccessories)))
		    return hr;

		if (lHint == pmPut)
		{
            if (pWornAccessories == NULL)
                return S_OK;

			// Get the string from the WornAccessory property list.
			if (FAILED(hr = pWornAccessories->get_ObjectPropertyExt(varHintData.bstrVal, IID_IVWAccessoryData, (IObjectProperty**)&pVWAccessoryData)))
		        return hr;

            if (pVWAccessoryData == NULL)
                return S_OK;

			if (FAILED(hr = pVWAccessoryData->get_AccessoryGraphic(&bstrAccessoryGraphic.m_str)))
		        return hr;
			if (FAILED(hr = pVWAccessoryData->GetPos(&fPosX, &fPosY)))
		        return hr;
			// Has this accessory been added or changed?
			if (FAILED(hr = pVWGeometrySprite->AccessoryFrameFromID(atol(CString(varHintData.bstrVal)), &pFrame)))
		        return hr;
			// Update the sprited geometry accordingly.
			if (!pFrame)
            {
				if (FAILED(hr = pVWGeometrySprite->AddAccessory(bstrAccessoryGraphic, atol(CString(varHintData.bstrVal)), fPosX, fPosY)))
		            return hr;
            }
			else
			{
				pFrame->SetTranslation(fPosX, fPosY);
				pVWGeometrySprite->get_AccessoryScene(&pScene);
				if (pScene)
					pScene->MoveChildToFront(pFrame);
				pVWGeometrySprite->OnAccessoriesChanged();
			}
		}
		else if (lHint == pmRemove)
		{
			if (FAILED(hr = pVWGeometrySprite->RemoveAccessory(atol(CString(varHintData.bstrVal)))))
	        	return hr;
		}
	}

    return hr;
}

//******************************************************
// OnConnect
// Called when this avatar is connected to the world

STDMETHODIMP CMultimediaExemplarObject::OnConnect(IThing *pAvatar)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr <IAvatarProfile> pProfile;
	CComPtr <IVWGeometrySprite> pGeom;
	CComVariant varReturnVal;
	CComPtr <IThing> pThis;
    HRESULT hr = S_OK;

	static CComBSTR bstrProfile("Profile");
	static CComBSTR bstrSpriteGeometryKey("SpriteGeometry");
	static CComBSTR	bstrGeometryName("GeometryName");

    if (pAvatar==NULL)
    {
        VWTRACE(m_pWorld, "VWMMAVATAR", VWT_ERROR, "CMultimediaExemplarObject::OnConnect: NULL pointer\n");
        return ReportMMError(E_POINTER);
    }

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	// We are good to go - install the profile prefs...
	
	if (FAILED(hr = pAvatar->get_ObjectPropertyExt(bstrProfile, IID_IAvatarProfile, (IObjectProperty **)&pProfile)))
		return hr;
	if (pProfile==NULL)
        return S_OK;

	// get the profile's sprite file and insert it into the avatar....
	if (FAILED(hr = pProfile->get_KeyValue(bstrSpriteGeometryKey.m_str, &varReturnVal)))
		return hr;
	if (varReturnVal.vt != VT_BSTR)
		return S_OK;
	pAvatar->put_String(bstrGeometryName, varReturnVal.bstrVal);

	// need to invoke base class method
	if (FAILED(hr = pThis->InvokeSuperMethodExt(CComDISPPARAMS(1, CComVariant((IDispatch*)pAvatar)), NULL)))
		return hr;

	//push the server time down to the client by calling SetClientSystemTime
	hr = pThis->InvokeMethodExt(CComBSTR("SetClientSystemTime"), 
								CComDISPPARAMS(1, CComVariant(CurrentTime())),
								NULL);

	return hr;
}

//******************************************************
// ThumbnailFileName
// Returns the full local filename of the avatar's
// thumbnail image.  IF the avatar has a geometry,
// it will use that.  Otherwise, it will attempt to
// get it from the profile.

#include <inetfile.h>
#include <spritemn.h>

STDMETHODIMP CMultimediaExemplarObject::ThumbnailFileName(BSTR *pbstrFileName)
{
	CComPtr<IVWGeometry>		pVWGeometry;
	CComPtr<IVWGeometrySprite>	pVWGeometrySprite;
	CComPtr<IThing>				pAvatar;
	CComBSTR					bstrFileName;
	char						szExt[_MAX_EXT];
	static CComBSTR		kbstrGeometry("Geometry");
	static CComBSTR		kbstrINetFile("INetFile");
	static CComBSTR		kbstrProfile("Profile");
	static CComBSTR		kbstrSpriteKey("SpriteGeometry");
    HRESULT             hr = S_OK;
	
    if (pbstrFileName==NULL)
    {
        VWTRACE(m_pWorld, "VWMMAVATAR", VWT_ERROR, "CMultimediaExemplarObject::ThumbnailFileName: NULL output pointer\n");
        return ReportMMError(E_POINTER);
    }

	if (FAILED(hr = m_pWorld->get_This(&pAvatar)))
		return hr;

	if (FAILED(hr = pAvatar->get_ObjectProperty(kbstrGeometry, (IObjectProperty**)&pVWGeometry)))
		return hr;

	if (pVWGeometry)
	{
		if (SUCCEEDED(pVWGeometry->QueryInterface(IID_IVWGeometrySprite, (void**)&pVWGeometrySprite)))
			pVWGeometry->get_GeometryName(&bstrFileName.m_str);
	}

	if (!bstrFileName.Length())
	{
		CComPtr<IAvatarProfile>	pProfile;

		if (FAILED(hr = pAvatar->get_ObjectProperty(kbstrProfile, (IObjectProperty **)&pProfile)))
		    return hr;

		if (pProfile)
		{
			CComBSTR	bstrFinalURL;
			CComVariant	varSprKey;

			if (FAILED(hr = pProfile->get_KeyValue(kbstrSpriteKey, &varSprKey)))
		        return hr;
			if (FAILED(hr = ::VariantChangeType(&varSprKey, &varSprKey, 0, VT_BSTR)))
            {
                VWTRACE(m_pWorld, "VWMMAVATAR", VWT_ERROR, "CMultimediaExemplarObject::ThumbnailFileName: sprite geometry filename is not BSTR\n");
                return ReportMMError(hr);
            }
			bstrFileName = varSprKey.bstrVal;
		}
	}

	// If the file is a sprite file we'll need to crack it to get the thumbnail.
	_splitpath(CString(bstrFileName), NULL, NULL, NULL, szExt );
	if (bstrFileName.Length() && stricmp(szExt, ".SPR") == 0)
	{
		CComPtr<IInternetFileManager>	pINetFile;
		CComPtr<ISpriteManager>		pSpriteManager;
		CComBSTR	bstrThumbNail;
		CComBSTR	bstrError;
		char	    pathName[_MAX_DIR], driveName[_MAX_DRIVE];
		CString	    fullName;

		if (FAILED(hr = m_pWorld->get_Tool(kbstrINetFile, (IUnknown **)&pINetFile)))
		    return hr;

		if (pINetFile)
		{
			CComBSTR	bstrFinalURL;
			CComBSTR	bstrInitialFile = bstrFileName;

			if (FAILED(hr = pINetFile->GetFileEx(bstrFileName, VARIANT_TRUE, &bstrFinalURL.m_str, &bstrFileName.m_str)))
		        return hr;
		}

		// OK, we have the image file - crack it with the SPR manager.
		if (FAILED(hr = CoCreateInstance(CLSID_SpriteManagerObject, NULL, CLSCTX_INPROC_SERVER,
						IID_ISpriteManager, (LPVOID*)&pSpriteManager)))
        {
	        VWTRACE(m_pWorld, "VWMMAVATAR", VWT_ERROR, "CMultimediaExemplarObject::ThumbnailFileName: unable to create sprite mgr\n");
            return ReportMMError(hr);
        }

		// get the thumbnail name
		if (FAILED(hr = pSpriteManager->ReadSpriteFile(bstrFileName, &bstrError.m_str)))
        {
	        VWTRACE(m_pWorld, "VWMMAVATAR", VWT_ERROR, "CMultimediaExemplarObject::ThumbnailFileName: failed to read the sprite file\n");
            return ReportMMError(hr);
        }
		if (FAILED(hr = pSpriteManager->get_ThumbNailImage(&bstrThumbNail.m_str)))
        {
	        VWTRACE(m_pWorld, "VWMMAVATAR", VWT_ERROR, "CMultimediaExemplarObject::ThumbnailFileName: unable to retrieve the thumbnail image\n");
            return ReportMMError(hr);
        }

		// now stick it onto the path name
		_splitpath(CString(bstrFileName), driveName, pathName, NULL, NULL );
		fullName = CString(driveName) + CString(pathName) + CString(bstrThumbNail);
		bstrThumbNail = fullName;
		*pbstrFileName = bstrThumbNail.Copy();
	}

	return hr;
}

STDMETHODIMP CMultimediaExemplarObject::OnCreate(IThing *pthing)
{
	// NOTE: This is basically a no-op.  Leaving the code
	// in here in case we need this hook at some point.

    if (pthing==NULL)
    {
        VWTRACE(m_pWorld, "VWMMAVATAR", VWT_ERROR, "CMultimediaExemplarObject::OnCreate: NULL pointer\n");
        return ReportMMError(E_POINTER);
    }

	// invoke super method
	CComVariant varThing((IDispatch *) pthing);
	CComDISPPARAMS dispSuper(1, varThing);
								
	return pthing->InvokeSuperMethodExt((DISPPARAMS *) dispSuper, NULL);
}


//******************************************************
// AddDefaultKeys
// Fills in the default keys that every profile is
// supposed to have. 

STDMETHODIMP CMultimediaExemplarObject::AddDefaultKeys()
{
	HRESULT hr = S_OK;
	CComPtr<IThing> pAvatar;
	CComPtr<IVector> pvec;
	CComPtr<IAvatarProfile> pProfile;
	CComPtr<IThing> pGlobal;

	const VARIANT kVarNone = {VT_ERROR, 0, 0, 0, DISP_E_MEMBERNOTFOUND};

	static CComBSTR bstrNA("n/a");
	static CComBSTR bstrEmpty("");
	static CComBSTR bstrSpriteGeometry("SpriteGeometry");
	static CComBSTR bstrGestureText("GestureText");
	static CComBSTR bstrNamedGestures("NamedGestures");
	static CComBSTR bstrCamera("CameraPreference");
	static CComBSTR bstrAvatarCameraPosition("AvatarCameraPosition");
	static CComBSTR bstrProfile("Profile");

	CComBSTR bstrDefaultSpriteGeometry;
	static CComBSTR bstrDefaultCamera("*FIRSTPERSON*");

	if (FAILED(hr = m_pWorld->get_This(&pAvatar)))
		return hr;
	if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
		return hr;

    // REVIEW: is this right?  we dereference pAvatar later in this method anyway...
	if (pAvatar)
	{
		if (FAILED(hr = pAvatar->get_ObjectPropertyExt(CComBSTR("Profile"), IID_IAvatarProfile, (IObjectProperty **)&pProfile)))
		    return hr;
		if (pProfile==NULL)
			return S_OK;
		if (FAILED(hr = pGlobal->get_String(CComBSTR("DefaultSpriteFile"), &bstrDefaultSpriteGeometry.m_str)))
		    return hr;

		_AddStringKey(pProfile, bstrSpriteGeometry, bstrDefaultSpriteGeometry, pAvatar);
		_AddStringKey(pProfile, bstrCamera, bstrDefaultCamera, pAvatar);

		_AddMapKey(m_pWorld, pProfile, bstrGestureText, pAvatar);
		_AddMapKey(m_pWorld, pProfile, bstrNamedGestures, pAvatar);

		/* hr = */ ::CreateVector(m_pWorld, 0.0f, 0.7f, 0.0f, &pvec);

		if (pvec)
			pProfile->AddKey(bstrAvatarCameraPosition, CComVariant(pvec), kVarNone, CComVariant(kKeyPublic), CComVariant(pAvatar));
	}

	// hack to call system layer method since 'super' method won't call it
	if (FAILED(hr = AddDefaultSystemKeys()))
		return hr;

	hr = pAvatar->InvokeSuperMethodExt(CComDISPPARAMS(0), NULL);

	return hr;
}

HRESULT CMultimediaExemplarObject::AddDefaultSystemKeys()
{
	HRESULT hr = S_OK;	 
	CComPtr<IThing> pAvatar;
	CComPtr<IAvatarProfile> pProfile;

	static CComBSTR bstrRealName("Real name");
	static CComBSTR bstrEMail("E-Mail alias");
	static CComBSTR bstrGender("Gender");
	static CComBSTR bstrAge("Age");
	static CComBSTR bstrWhereFrom("Where from");
	static CComBSTR bstrHomePage("Home page");
	static CComBSTR bstrLastWorld("LastWorld");
	static CComBSTR bstrQuote("Quote");
	static CComBSTR bstrRecentWorlds("RecentWorlds");
	static CComBSTR bstrFriends("Friends");
	static CComBSTR bstrIgnoring("Ignoring");
	static CComBSTR bstrChat("Chat?");
	static CComBSTR bstrNA("n/a");
	static CComBSTR bstrEmpty("");

	static CComBSTR bstrDefaultWhereFrom("");
	static CComBSTR bstrDefaultHomePage("");
	static CComBSTR bstrDefaultLastWorld("");
	static CComBSTR bstrDefaultQuote("");
	static CComBSTR bstrProfile("Profile");

	if (FAILED(hr = m_pWorld->get_This(&pAvatar)))
		return hr;

	if (pAvatar)
	{
		if (FAILED(hr = pAvatar->get_ObjectPropertyExt(CComBSTR("Profile"), IID_IAvatarProfile, (IObjectProperty **)&pProfile)))
    		return hr;
		if (pProfile==NULL)
			return S_OK;

		// REVIEW:: We require an avatar to set permissions properly
		_AddStringKey(pProfile, bstrRealName, bstrNA, pAvatar);
		_AddStringKey(pProfile, bstrEMail, bstrNA, pAvatar);
		_AddStringKey(pProfile, bstrGender, bstrNA, pAvatar);
		_AddStringKey(pProfile, bstrAge, bstrNA, pAvatar);
		_AddStringKey(pProfile, bstrWhereFrom, bstrDefaultWhereFrom, pAvatar);
		_AddStringKey(pProfile, bstrHomePage, bstrDefaultHomePage, pAvatar);
		_AddStringKey(pProfile, bstrLastWorld, bstrDefaultLastWorld, pAvatar);
		_AddStringKey(pProfile, bstrQuote, bstrDefaultQuote, pAvatar);

		_AddListKey(m_pWorld, pProfile, bstrRecentWorlds, pAvatar);
		_AddListKey(m_pWorld, pProfile, bstrFriends, NULL);
		_AddListKey(m_pWorld, pProfile, bstrIgnoring, pAvatar);

		_AddBOOLKey(pProfile, bstrChat, VARIANT_TRUE, pAvatar);
	}

//		if (FAILED(hr = pAvatar->InvokeSuperMethodExt(CComDISPPARAMS(1, CComVariant(pProfile)), NULL)))
//  		goto ERROR_ENCOUNTERED;
    
    return hr;
}


//******************************************************
// Teleport
// Transports the avatar to another room - if 
// there is a portal in the room, go through it.
// Otherwise get the room's DefaultAvatarPosition and 
// move the avatar there

STDMETHODIMP CMultimediaExemplarObject::Teleport(IThing *pNewRoom)
{
	HRESULT hr;
	CComPtr<IThing> pThis;
	CComPtr<IPropertyList> pPortals;

	CComPtr<IPropertyList> pListArrive;
	CComBSTR bstrDescriptionArrive("You enter {object}.|{subject} enters {object}.");
	CComBSTR bstrNameArrive("TeleportArrive");


	static CComBSTR bstrPropPortals("Portals");
	static CComBSTR bstrMethodTransportTo("TransportTo");
	static CComBSTR bstrPropDefPos("DefaultAvatarPosition");
	static CComBSTR bstrPropPosition("Position");
	static CComBSTR bstrMethodMoveInto("MoveInto");

	hr = m_pWorld->get_This(&pThis);
	if (FAILED(hr))
		return hr;

	// If the destination room has a portal, go through that
	hr = pNewRoom->get_ObjectPropertyExt(bstrPropPortals, IID_IPropertyList, (IObjectProperty **) &pPortals);
	if (FAILED(hr))
		return hr;

	
#ifdef _DEBUG
	CComBSTR bstrNewRoomName("n/a");
	pNewRoom->get_Name(&bstrNewRoomName.m_str);
#endif

	long cPortals;
	hr = pPortals->get_Count(&cPortals);
	if (FAILED(hr))
		return hr;

	if (cPortals>0)
	{
		CComPtr<IThing> pDestPortal;

		// Get the first portal
		hr = pPortals->get_Thing(0, &pDestPortal);
		if (FAILED(hr))
			return hr;

		// Go through it
		hr = pDestPortal->InvokeMethodExt(bstrMethodTransportTo, CComDISPPARAMS(1, CComVariant((IDispatch*) pThis)),
									NULL);

		
#ifdef _DEBUG
	CComBSTR bstrDestPortalName("n/a");
	pNewRoom->get_Name(&bstrDestPortalName.m_str);
	TRACE("Teleporting to room %s through portal %s \n", CString(bstrNewRoomName), CString(bstrDestPortalName));
#endif

	}
	else
	{
		// Otherwise move to room's default avatar position (unless set otherwise, default is 0, 0, 0)
		// Get the new room's default position
		CComPtr<IVector> pPos, pDefPos;

		hr = pNewRoom->get_ObjectPropertyExt(bstrPropDefPos, IID_IVector, (IObjectProperty **) &pDefPos);
		if (FAILED(hr))
			return hr;

		// Get the avatar's position and set it to be the default position
		hr = pThis->get_ObjectPropertyExt(bstrPropPosition, IID_IVector, (IObjectProperty **) &pPos);
		if (FAILED(hr))
			return hr;

		float fx, fy, fz;
		hr = pDefPos->get(&fx, &fy, &fz);
		if (FAILED(hr))
			return hr;
		hr = pPos->set(fx, fy, fz);
		if (FAILED(hr))
			return hr;

		// Now move into the new room
		hr = pThis->InvokeMethodExt(bstrMethodMoveInto, CComDISPPARAMS(1, CComVariant(pNewRoom)), NULL);
	#ifdef _DEBUG
		TRACE("Teleporting to room %s using MoveInto \n", CString(bstrNewRoomName));
	#endif
	}
	// Fire arrival event		
	// Create the Action event subject list
	hr = CreatePropertyList(m_pWorld, &pListArrive);
  	if (FAILED(hr))
		return hr;

	hr = pListArrive->AddThing(pThis);
  	if (FAILED(hr))
		return hr;
	
	hr = pListArrive->AddThing(pNewRoom);
	if (FAILED(hr))
		return hr;

	// Fire the Action event
	hr = pNewRoom->FireEventExt(CComBSTR("Action"),
							CComDISPPARAMS(3, CComVariant(bstrNameArrive),
										   CComVariant(bstrDescriptionArrive),
										   CComVariant((IDispatch*) pListArrive)),
							FIREEVENT_DEFAULT);


	return hr;
}