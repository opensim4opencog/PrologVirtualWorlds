// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// Avatar.cpp : Implementation of CAvatar
#include "stdafx.h"
#include <vwsysex.h>
#include <vwanim.h>
#include "vwutils.h"
#include "avatar.h"
#include "syshelp.h"
#include "profhelp.h"
#include "..\..\vwobject\propsecu.h"
#include <resource.h>
#include <vwevents.h>

#define ALLOWSECURITYOVERRIDE

/////////////////////////////////////////////////////////////////////////////
// CAvatar

//******************************************************
// Install
// Installs this module - creates the exemplar and
// adds the properties and methods.

STDMETHODIMP CAvatarExemplarObject::Install(IModule* pModule)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CMethodInfo rgMethod[] =
	{
		{ METHOD_CLIENT, "Gesture",					NULL,		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD },
//		{ METHOD_CLIENT, "RetrieveOwnedObject",		NULL,		PSBIT_EXEMPLARDEFAULTMETHOD,				PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER, "WieldItem",				"Wield",	PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_CLIENT, "UnWieldItem",				"UnWield",	PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER, "Teleport",				NULL,		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD },	

		{ METHOD_CLIENT, "EditProfile",				NULL,		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_CLIENT, "WhisperTo",				NULL,		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "LookAt",					NULL,		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_CLIENT, "AddToUserProfileList",	NULL,		PSBIT_EXEMPLARDEFAULTMETHOD,				PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "RemoveFromUserProfileList", NULL,		PSBIT_EXEMPLARDEFAULTMETHOD,				PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "IsInUserProfileList",		NULL,		PSBIT_EXEMPLARDEFAULTMETHOD,				PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "UpdateUserProfileList",	NULL,		PSBIT_EXEMPLARDEFAULTMETHOD,				PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "MenuUserProfileList",		NULL,		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "UpdateMenuItemName",		NULL,		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "UpdateWhisperMenuItem",	NULL,		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },

		{ METHOD_CLIENT, "Find",					NULL,		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER, "Query",					NULL,		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER, "Alert",					NULL,		PSBIT_EXEMPLARDEFAULTMETHOD,				PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER, "Confirm",					NULL,		PSBIT_EXEMPLARDEFAULTMETHOD,				PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER, "Prompt",					NULL,		PSBIT_EXEMPLARDEFAULTMETHOD,				PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "ClientQuery",				NULL,		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_CLIENT, "QueryReturn",				NULL,		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD },
		
		{ METHOD_CLIENT | METHOD_SERVER,			"GestureText", NULL,	PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_CLIENT, "DoNamedGesture", 			NULL,		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_CLIENT, "ThumbnailFileName",		NULL,		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD },

		{ METHOD_SERVER, "OnSystemCreate",			NULL,		PSBIT_SYSTEMEVENTHANDLER,							PS_EVENTHANDLER },
		{ METHOD_SERVER, "OnSystemConnect",			NULL,		PSBIT_SYSTEMEVENTHANDLER,							PS_EVENTHANDLER },
		{ METHOD_SERVER, "OnSystemDisconnect",		NULL,		PSBIT_SYSTEMEVENTHANDLER,							PS_EVENTHANDLER },
		{ METHOD_SERVER, "OnSystemPreCreate",		NULL,		PSBIT_SYSTEMEVENTHANDLER,							PS_EVENTHANDLER },
		{ METHOD_SERVER, "OnSystemPostCreate",		NULL,		PSBIT_SYSTEMEVENTHANDLER,							PS_EVENTHANDLER },
		{ METHOD_SERVER, "OnSystemPreDestroy",		NULL,		PSBIT_SYSTEMEVENTHANDLER,							PS_EVENTHANDLER },
		{ METHOD_SERVER, "OnSystemPostDestroy",		NULL,		PSBIT_SYSTEMEVENTHANDLER,							PS_EVENTHANDLER },
		{ METHOD_SERVER, "SetObjectQuota",			NULL,		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER, "AddDefaultKeys",			NULL,		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_CLIENT, "OnAction",				NULL,		PSBIT_EVENTHANDLER,							PS_EVENTHANDLER },
 		{ METHOD_CLIENT, "OnPeerAction",			"OnAction",	PSBIT_EVENTHANDLER,							PS_EVENTHANDLER },
 		{ METHOD_CLIENT, "OnContainerAction",		"OnAction",	PSBIT_EVENTHANDLER,							PS_EVENTHANDLER },
		{ METHOD_CLIENT, "OnIsAuthorChanged",		NULL,		PSBIT_EVENTHANDLER,							PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,			"OnIsWizardAccess", "OnIsWizardOrIsAuthorAccess", PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,			"OnIsAuthorAccess", "OnIsWizardOrIsAuthorAccess", PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,			"OnObjectQuotaAccess", "OnWizardAccess", PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER, "FireConnectAvatar",		NULL,		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD }
	};
	int cMethod = sizeof(rgMethod)/sizeof(CMethodInfo);

	//  OnAccess methods for world owner properties - call the system module's OnWorldOwnerAccess method
	CMethodInfo woMethod[] =
	{
		{ METHOD_CLIENT | METHOD_SERVER, "OnConnectionAccess",	"OnWorldOwnerAccess", PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER, "OnIPAddressAccess",	"OnWorldOwnerAccess", PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER, "OnIsConnectedAccess", "OnGlobalConstantAccess", PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER, "OnAuthenticationAccess",	"OnGlobalConstantAccess", PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD }
	};
	int woLength = sizeof(woMethod)/sizeof(CMethodInfo);

	CComPtr<IThing> pAvatarEx;
	CComBSTR bstrName;
	CComBSTR bstrNameInternal;
	CComBSTR bstrDesc;
	CComPtr<IPropertyList> pEditMenu;
	CComPtr<IModule> pSysModule;
	CComPtr<IPropertyList> pMenu;
	CComPtr<IPropertyList> pThingMenu;
	CComPtr<IMenuItem> pMenuItem;
    HRESULT hr = S_OK;
    int iMethod;

	if (FAILED(hr = m_pWorld->CreateExemplar(CComBSTR("Avatar"), CComVariant(CComBSTR("Thing")), &pAvatarEx)))
    	goto ERROR_ENCOUNTERED;

	// add in the methods
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
	
	// Now, add in the OnXXXAccess methods that reference OnWorldOwnerAccess in the system module
	// First, get system module
	if (FAILED(hr = m_pWorld->get_Module(CComBSTR("System"), &pSysModule)))
	    goto ERROR_ENCOUNTERED;

	for (iMethod=0; iMethod<woLength; iMethod++)
	{
		bstrName = woMethod[iMethod].pszName;

		if (woMethod[iMethod].pszNameInternal == NULL)
			bstrNameInternal = bstrName;
		else
			bstrNameInternal = woMethod[iMethod].pszNameInternal;
		
		if (FAILED(hr = pAvatarEx->CreateAndAddMethodExt(woMethod[iMethod].lFlags,
													pSysModule,
													bstrNameInternal,
													bstrName,
													woMethod[iMethod].psbits,
													woMethod[iMethod].permissions)))
    	    goto ERROR_ENCOUNTERED;
	}

	// Create the default properties
	// profile
	if (FAILED(hr = pAvatarEx->AddPropertyExt(	CComBSTR("Profile"), 
											CComVariant((IDispatch*)NULL), 
											PSBIT_SYSTEMPROPERTY | PSBIT_INTERNALLYSET | PSBIT_HIDDEN, 
											PS_SYSTEMPROPERTY,	
											VT_DISPATCH, IID_IAvatarProfile, NULL)))
	    goto ERROR_ENCOUNTERED;

	// wizardry												
	if (FAILED(hr = pAvatarEx->AddPropertyExt(	CComBSTR("IsWizard"), 
											CComVariant((bool)VARIANT_FALSE), 
											PSBIT_EXEMPLARDEFAULTPROPERTY, 
											PS_ALLACCESSPROPERTY,	
											VT_BOOL, IID_NULL, NULL)))
	    goto ERROR_ENCOUNTERED;

    // authorness
	if (FAILED(hr = pAvatarEx->AddPropertyExt(	CComBSTR("IsAuthor"), 
											CComVariant((bool)VARIANT_FALSE), 
											PSBIT_EXEMPLARDEFAULTPROPERTY, 
											PS_ALLACCESSPROPERTY,	
											VT_BOOL, IID_NULL, NULL)))
	    goto ERROR_ENCOUNTERED;

	// Wielded Item
	if (FAILED(hr = pAvatarEx->AddPropertyExt(	CComBSTR("WieldedItem"),
											CComVariant((IDispatch*) NULL),
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_DISPATCH, IID_IThing, NULL)))
	    goto ERROR_ENCOUNTERED;

	// Quota (set to zero here then bumped up later in OnCreate)
	if (FAILED(hr = pAvatarEx->AddPropertyExt(	CComBSTR("ObjectQuota"),
											CComVariant(0L),
											PSBIT_WORLDOWNERPROPERTY,
											PS_WORLDOWNERPROPERTY,
											VT_I4, IID_NULL, NULL)))
	    goto ERROR_ENCOUNTERED;

	// Queries
 	if (FAILED(hr = pAvatarEx->AddPropertyExt(	CComBSTR("QueryOK"),
											CComVariant((bool)VARIANT_FALSE),
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_BOOL, IID_NULL, NULL)))
	    goto ERROR_ENCOUNTERED;
											
	if (FAILED(hr = pAvatarEx->AddPropertyExt(	CComBSTR("QueryResult"),
											CComVariant(""),
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_BSTR, IID_NULL, NULL)))
	    goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pAvatarEx->AddPropertyExt(	CComBSTR("QueryParam"),
											CComVariant(""),
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_DISPATCH, IID_IDispatch, NULL)))
	    goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pAvatarEx->AddPropertyExt(	CComBSTR("Authentication"),
											CComVariant(""),
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
											PS_GLOBALSYSTEMCONSTANT,
											VT_BSTR, IID_NULL, NULL)))
	    goto ERROR_ENCOUNTERED;

	// Menu Items
	// Object menu items

	// Get current menu (from Thing exemplar)
	if (FAILED(hr = pAvatarEx->get_ObjectProperty(CComBSTR("Menu"), (IObjectProperty**) &pThingMenu)))
    	goto ERROR_ENCOUNTERED;

	// Make a copy (since not COPYONGET)
	if (FAILED(hr = CreateMenu(m_pWorld, &pMenu)))
	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThingMenu->CopyTo(pMenu)))
	    goto ERROR_ENCOUNTERED;
    pThingMenu.Release();

	// Create and add new items
	if (FAILED(hr = CreateMenuItemExt(m_pWorld, CComBSTR("Whisper to..."), 0, pAvatarEx, 
										CComBSTR("UpdateWhisperMenuItem"), CComBSTR("WhisperTo"), 
										NULL, &pMenuItem)))
    	goto ERROR_ENCOUNTERED;

//		pMenuItem->put_SortOrder(MI_SORT_PRIMARY+20);

	if (FAILED(hr = pMenu->AddObjectProperty(pMenuItem)))
	    goto ERROR_ENCOUNTERED;
	pMenuItem.Release();
	
    if (FAILED(hr = pAvatarEx->put_ObjectProperty(CComBSTR("Menu"), pMenu)))
    	goto ERROR_ENCOUNTERED;
	pMenu.Release();

	// Peer menu items
	if (FAILED(hr = pAvatarEx->get_ObjectProperty(CComBSTR("PeerMenu"), (IObjectProperty**) &pThingMenu)))
    	goto ERROR_ENCOUNTERED;

	// make a copy (since not COPYONGET)
	if (FAILED(hr = CreateMenu(m_pWorld, &pMenu)))
	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThingMenu->CopyTo(pMenu)))
	    goto ERROR_ENCOUNTERED;
	pThingMenu.Release();

	// Take
	if (FAILED(hr = CreateMenuItemExt(	m_pWorld, CComBSTR("Take"), 0, NULL,
									CComBSTR("UpdateIsTakeable"), CComBSTR("Take"), 
									NULL,  &pMenuItem)))
	    goto ERROR_ENCOUNTERED;

//		pMenuItem->put_SortOrder(MI_SORT_PRIMARY+100);

	if (FAILED(hr = pMenuItem->put_AddTargetParameter(VARIANT_TRUE)))
    	goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMenu->AddObjectProperty(pMenuItem)))
	    goto ERROR_ENCOUNTERED;
	pMenuItem.Release();

	if (FAILED(hr = pAvatarEx->put_ObjectProperty(CComBSTR("PeerMenu"), pMenu)))
	    goto ERROR_ENCOUNTERED;
	pMenu.Release();

	// Container menu items
	if (FAILED(hr = pAvatarEx->get_ObjectProperty(CComBSTR("ContainerMenu"), (IObjectProperty**) &pThingMenu)))
    	goto ERROR_ENCOUNTERED;
	if (FAILED(hr = CreateMenu(m_pWorld, &pMenu)))
	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThingMenu->CopyTo(pMenu)))
	    goto ERROR_ENCOUNTERED;
	pThingMenu.Release();

	// Drop
	if (FAILED(hr = CreateMenuItemExt(	m_pWorld, CComBSTR("Drop"), 0, NULL, NULL,
									CComBSTR("Drop"), NULL, &pMenuItem)))
	    goto ERROR_ENCOUNTERED;

//		pMenuItem->put_SortOrder(MI_SORT_PRIMARY+100);

	if (FAILED(hr = pMenuItem->put_AddTargetParameter(VARIANT_TRUE)))
	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMenu->AddObjectProperty(pMenuItem)))
	    goto ERROR_ENCOUNTERED;
	pMenuItem.Release();

	if (FAILED(hr = pAvatarEx->put_ObjectProperty(CComBSTR("ContainerMenu"), pMenu)))
	    goto ERROR_ENCOUNTERED;
	pMenu.Release();

	// Self menu items: More about, Edit profile, Properties..., Whisper...
	if (FAILED(hr = pAvatarEx->get_ObjectProperty(CComBSTR("SelfMenu"), (IObjectProperty**) &pThingMenu)))
    	goto ERROR_ENCOUNTERED;
	if (FAILED(hr = CreateMenu(m_pWorld, &pMenu)))
	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThingMenu->CopyTo(pMenu)))
	    goto ERROR_ENCOUNTERED;
	pThingMenu.Release();
	
	if (FAILED(hr = CreateMenuItemExt(	m_pWorld, CComBSTR("Profile"), 0, 
									NULL, CComBSTR("UpdateMenuItemName"), CComBSTR("EditProfile"), 
									NULL, &pMenuItem)))
	    goto ERROR_ENCOUNTERED;

//		pMenuItem->put_SortOrder(MI_SORT_INFO);

	if (FAILED(hr = pMenu->AddObjectProperty(pMenuItem)))
    	goto ERROR_ENCOUNTERED;
	pMenuItem.Release();

	if (FAILED(hr = CreateMenuItemExt(	m_pWorld, CComBSTR("Whisper..."), 0, pAvatarEx, 
									CComBSTR("UpdateWhisperMenuItem"), CComBSTR("WhisperTo"), 
									NULL, &pMenuItem)))
	    goto ERROR_ENCOUNTERED;

//		pMenuItem->put_SortOrder(MI_SORT_INFO+10);

	if (FAILED(hr = pMenu->AddObjectProperty(pMenuItem)))
	    goto ERROR_ENCOUNTERED;
	pMenuItem.Release();

	// NOTE:  We add the new menu AFTER we have added all of the items to it.
	if (FAILED(hr = pAvatarEx->put_ObjectProperty(CComBSTR("SelfMenu"), pMenu)))
    	goto ERROR_ENCOUNTERED;
	pMenu.Release();

	// EDIT MENUS...  Needed to have different description page (see bug #2974)
	// bstrDesc = "General|Edit the text description of the object|descedsm.htm";
	// removed the special general tab reference, but leaving in the property adding to
	// not screw up the adding of the other tabs

	// End user map
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pEditMenu)))
	    goto ERROR_ENCOUNTERED;
	//create the description page
//	if (FAILED(hr = pEditMenu->AddString(bstrDesc)))
//	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAvatarEx->put_Property(CComBSTR("EndUserEditMap"), CComVariant((IDispatch*) pEditMenu))))
	    goto ERROR_ENCOUNTERED;
	pEditMenu.Release();

	// Owner map
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pEditMenu)))
    	goto ERROR_ENCOUNTERED;
	//create the description page
//	if (FAILED(hr = pEditMenu->AddString(bstrDesc)))
//	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAvatarEx->put_Property(CComBSTR("OwnerEditMap"), CComVariant((IDispatch*) pEditMenu))))
	    goto ERROR_ENCOUNTERED;
	pEditMenu.Release();

	// Builder map
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pEditMenu)))
    	goto ERROR_ENCOUNTERED;
	//create the description page
//	if (FAILED(hr = pEditMenu->AddString(bstrDesc)))
//	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAvatarEx->put_Property(CComBSTR("BuilderEditMap"), CComVariant((IDispatch*) pEditMenu))))
	    goto ERROR_ENCOUNTERED;
	pEditMenu.Release();

	// Advanced map
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pEditMenu)))
    	goto ERROR_ENCOUNTERED;
	//create the description page
//	if (FAILED(hr = pEditMenu->AddString(bstrDesc)))
//	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAvatarEx->put_Property(CComBSTR("AdvancedEditMap"), CComVariant((IDispatch*) pEditMenu))))
	    goto ERROR_ENCOUNTERED;
	pEditMenu.Release();

    return S_OK;

ERROR_ENCOUNTERED:	
	VWTRACE(m_pWorld, "VWAVATAR", VWT_ERROR, "CAvatarExemplarObject::Install   Error encountered %x\n", hr);
    
    return hr;
}

STDMETHODIMP CAvatarExemplarObject::OnSystemCreate(IThing *pthing)
{
	HRESULT hr = S_OK;
	CComPtr<IThing>	pGlobal;
	CComPtr<IPropertyMap> pAvatars;
	CComPtr<IPropertyMap> pExemplars;
	long cAvatars;
	long lQuota;
	VARIANT_BOOL bIsExemplar = VARIANT_FALSE;

	static CComBSTR bstrDefaultQuota("DefaultObjectQuota");

	VWTRACE(m_pWorld, "VWAVATAR", VWT_METHOD, "CAvatarExemplarObject::OnSystemCreate\n");

    if (pthing == NULL)
    {
        VWTRACE(m_pWorld, "VWAVATAR", VWT_ERROR, "CAvatarExemplarObject::OnSystemCreate: NULL pointer\n");
        return ReportAvatarError(E_POINTER);
    }

	hr = pthing->get_IsExemplar(&bIsExemplar);
	if (FAILED(hr))
		return hr;

	if (bIsExemplar == VARIANT_TRUE)
	{
        VWTRACE(m_pWorld, "VWAVATAR", VWT_IMPORTANT, "CAvatarExemplarObject::OnSystemCreate: creating an avatar-based exemplar, returning\n");
		return hr;
	}

	// get the global pointer
	hr = m_pWorld->get_Global(&pGlobal);
	if (FAILED(hr))
		return hr;

	// get the default quota value from global
	hr = pGlobal->get_Long(bstrDefaultQuota, &lQuota);
	if (FAILED(hr))
		return hr;

	// check if we should make the user a wizard

	// see if this is the first avatar in the world.  
	hr = pGlobal->get_ObjectPropertyExt(CComBSTR("Avatars"), IID_IPropertyMap, (IObjectProperty **) &pAvatars);
	if (FAILED(hr))
		return hr;

    ASSERT(pAvatars);

	hr = pAvatars->get_Count(&cAvatars);
	if (FAILED(hr))
		return hr;

	if (cAvatars == 1)
	{
		{
			CSetUID SetUID(m_pWorld, pthing, pthing, VARIANT_TRUE);

			// yep - make it a wizard
			hr = pthing->put_BOOL(CComBSTR("IsWizard"), VARIANT_TRUE);
			if (FAILED(hr))
				return hr;

			// yep - make it an author
			hr = pthing->put_BOOL(CComBSTR("IsAuthor"), VARIANT_TRUE);
			if (FAILED(hr))
				return hr;
		}

		// and give it infinite quota
		lQuota = -1;

		// make it the world owner
		{
			CSetUID SetUID(m_pWorld, pGlobal, pGlobal, VARIANT_TRUE);
			
			hr = pGlobal->put_OwnerInt(pthing);

			if (FAILED(hr))
				return hr;
		}

		// fix up all exemplars installed to date with the correct owner (world.owner)
		hr = pGlobal->get_ObjectPropertyExt(CComBSTR("Exemplars"), IID_IPropertyMap, (IObjectProperty **) &pExemplars);
		if (FAILED(hr))
			return hr;

		CComBSTR bstr;
		CComVariant var;
		VARIANT_BOOL bAllDone = VARIANT_FALSE;
		IThing* pExemplar = NULL;

        ASSERT(pExemplars);

		hr = pExemplars->FirstItem(&bstr.m_str, &var, &bAllDone);

		while (bAllDone == VARIANT_FALSE && SUCCEEDED(hr)) 
		{
			ASSERT(var.vt == VT_DISPATCH);

			pExemplar = (IThing*)var.pdispVal;

			{
				CSetUID SetUID(m_pWorld, pExemplar, pExemplar, VARIANT_TRUE);

				/* hr = */ pExemplar->put_OwnerInt(pthing);
			}

			// clean up alloced memory
			var.Clear();
			bstr.Empty();
			pExemplar = NULL;

			if (FAILED(hr))
				break;

			// get next property
			hr = pExemplars->NextItem(&bstr.m_str, &var, &bAllDone);
		}

		if (FAILED(hr))
			return hr;
	}

	{
		CSetUID SetUID(m_pWorld, pthing, pthing, VARIANT_TRUE);
		
		hr = pthing->put_Long(CComBSTR("ObjectQuota"), lQuota);
	}

	return hr;
}

//******************************************************
// LookAt
// Shows avatar profile 


STDMETHODIMP CAvatarExemplarObject::LookAt()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
    HRESULT         hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	if (FAILED(hr = m_pWorld->FireUIEvent(pThis, CComBSTR(VW_SHOWPROFILE_EVENT_STR), CComVariant(NULL))))
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CAvatarExemplarObject::LookAt   FireUIEvent failed\n");
#endif
	}

	return hr;
}


//******************************************************
// EditProfile
// Brings up an editor for this avatar's profile.
// Only the avatar itself can invoke this

STDMETHODIMP CAvatarExemplarObject::EditProfile()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    HRESULT hr = S_OK;
	CComPtr<IThing> pThis;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;
	
	if (FAILED(hr = m_pWorld->FireUIEvent(pThis, CComBSTR(VW_EDITPROFILE_EVENT_STR), CComVariant(NULL))))
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CAvatarExemplarObject::EditProfile   FireUIEvent failed\n");
#endif
	}

    return hr;
}

//******************************************************
// UpdateUserProfileList
// Updates a generic UserInProfile menuItem

STDMETHODIMP CAvatarExemplarObject::UpdateUserProfileList(IMenuItem *pmi)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pmi == NULL)
    {
        VWTRACE(m_pWorld, "VWAVATAR", VWT_ERROR, "CAvatarExemplarObject::UpdateUserProfileList: NULL pointer\n");
        return ReportAvatarError(E_POINTER);
    }

	CComPtr<IThing> pUser;
	CComPtr<IThing> pThing;
	long lFlags;
	CComVariant	varOnList;
    HRESULT hr = S_OK;

	if (FAILED(hr = m_pWorld->get_User(&pUser)))
    	return hr;

    ASSERT(pUser);

	if (FAILED(hr = pmi->get_Owner(&pThing)))
	    return hr;

	if (FAILED(hr = pmi->get_Flags(&lFlags)))
	    return hr;

    DISPPARAMS *pArgs = NULL;

	if (FAILED(hr = pmi->get_ArgumentsExt(&pArgs)))
	    return hr;

	if (pArgs->cArgs!=1 || pArgs->rgvarg[0].vt!=VT_BSTR)
    {
        VWTRACE(m_pWorld, "VWAVATAR", VWT_ERROR, "CAvatarExemplarObject::UpdateUserProfileList: Invalid arguments\n");
        hr = E_INVALIDARG;
        goto ERROR_ENCOUNTERED;
    }

	if (FAILED(hr = pUser->InvokeMethodExt(CComBSTR("IsInUserProfileList"), 
                                           CComDISPPARAMS(2, CComVariant((IDispatch*) pThing),
                                                             CComVariant(pArgs->rgvarg[0].bstrVal)),
                                           &varOnList)))
	    goto ERROR_ENCOUNTERED;

	ASSERT(varOnList.vt == VT_BOOL);

	if (varOnList.boolVal == VARIANT_TRUE)
		lFlags |= MI_CHECKED;
	else
		lFlags &= ~MI_CHECKED;

	// set check status
	hr = pmi->put_Flags(lFlags);

ERROR_ENCOUNTERED:

    if (pArgs)
        TerminateDISPPARAMS(VARIANT_FALSE, pArgs);

    return ReportAvatarError(hr);
}


STDMETHODIMP CAvatarExemplarObject::MenuUserProfileList(BSTR bstrKey)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
	CComPtr<IThing> pUser;
	CComVariant		varKeyVal;
    HRESULT         hr = S_OK;

	if (FAILED(hr = m_pWorld->get_User(&pUser)))
    	return hr;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	// Get the current whisper status
    CComVariant varThis((IDispatch*) pThis);
    CComVariant varKey(bstrKey);
    CComDISPPARAMS dispparams(2, varThis, varKey);

	if (FAILED(hr = pUser->InvokeMethodExt(CComBSTR("IsInUserProfileList"), dispparams, &varKeyVal)))
	    return hr;

	ASSERT(varKeyVal.vt == VT_BOOL);

	if (varKeyVal.boolVal==VARIANT_TRUE)
		hr = pUser->InvokeMethodExt(CComBSTR("RemoveFromUserProfileList"), dispparams, NULL);
	else
		hr = pUser->InvokeMethodExt(CComBSTR("AddToUserProfileList"), dispparams, NULL);

	return hr;
}

//******************************************************
// UpdateMenuItemName
// Updates menu item to AvatarName's Profile

STDMETHODIMP CAvatarExemplarObject::UpdateMenuItemName(IMenuItem * pmi)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pTarget;
	CComBSTR bstrName;
	CComBSTR bstrMenu;
    HRESULT  hr = S_OK;

    if (pmi == NULL)
    {
        VWTRACE(m_pWorld, "VWAVATAR", VWT_ERROR, "CAvatarExemplarObject::UpdateMenuItemName: NULL pointer\n");
	    return ReportAvatarError(E_POINTER);
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

	ASSERT(pTarget);

	// REVIEW: This is not internationalizable.

	// Now get name and add to the front of the menu text
	if (FAILED(hr = pTarget->get_Name(&bstrName.m_str)))
	    return hr;

	if (FAILED(hr = pmi->get_MenuText(&bstrMenu.m_str)))
	    return hr;

	bstrName += "'s profile";
	
    hr = pmi->put_MenuText(bstrName);
    return hr;
}

//******************************************************
// UpdateWhisperMenu
// Updates Whisper menu item on avatar:
// If room doesn't allow whispers. don't display


STDMETHODIMP CAvatarExemplarObject::UpdateWhisperMenuItem(IMenuItem *pmi)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    HRESULT hr = S_OK;
	CComPtr<IThing> pThis;
	CComPtr<IThing> pContainer;
	VARIANT_BOOL boolAllowWhispers = VARIANT_FALSE;
	long lFlags;

    if (pmi == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CAvatarExemplarObject::UpdateWhisper: NULL pointer\n");
        return ReportAvatarError(E_POINTER);
    }

	// Get the this object
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	// Get container (current room) and see if it allows whispers
	if (pThis)
    {
		if (FAILED(hr = pThis->get_Container(&pContainer)))
	        return hr;

		if (pContainer)
		{
			if (FAILED(hr = pContainer->get_BOOL(CComBSTR("AllowWhispers"), &boolAllowWhispers)))
				return hr;
		}
	}

	if (FAILED(hr = pmi->get_Flags(&lFlags)))
	    return hr;
	// If allow whispers, show menu item
	if (boolAllowWhispers)
		lFlags &= ~MI_INVISIBLE;
	else  // otherwise menu item is invisible
		lFlags |= MI_INVISIBLE;

	hr = pmi->put_Flags(lFlags);
	
    return hr;
}


//******************************************************
// Gesture
// Makes the avatar perform a gesture.  This basically
// just does an emote with the appropriate gesture
// and text...

STDMETHODIMP CAvatarExemplarObject::Gesture(BSTR strGestureName)
{
	// not implemented!
    VWREPORT(m_pWorld, VWR_ERROR, "CAvatarExemplarObject::Gesture: method not implemented\n");
	return ReportAvatarError(E_NOTIMPL);

}

//******************************************************
// Wield
// Wields the object.  This adds the object to the 
// accessory plane and marks it as the wielded object.
// If an object is currently wielded, it will be
// unwielded before the new one can be wielded.
// An object must be wieldable in order to be wielded.

STDMETHODIMP CAvatarExemplarObject::Wield(IThing * pThing)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
	CComPtr<IThing> pThingCont;
	CComPtr<IThing> pCurWield;
	VARIANT_BOOL	bVal;
    HRESULT         hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

    if (pThing)
	{
		// make sure object is in inventory
		if (FAILED(hr = pThing->get_Container(&pThingCont)))
	        return hr;

		if (pThingCont != pThis)
			return S_OK;

		// make sure it's wieldable
		if (FAILED(hr = pThing->get_BOOL(CComBSTR("IsWieldable"), &bVal)))
        	return hr;

		if (bVal!=VARIANT_TRUE)
			return S_OK;
	}

	// put away the currently wielded object
	if (FAILED(hr = pThis->get_Thing(CComBSTR("WieldedItem"), &pCurWield)))
	    return hr;

	if (pCurWield)
	{
		// REVIEW: Security violations

		// unwield
		if (FAILED(hr = pCurWield->put_BOOL(CComBSTR("IsWielded"), VARIANT_FALSE)))
        	return hr;

		// not needed elsewhere anymore
		if (FAILED(hr = pCurWield->put_BOOL(CComBSTR("IsNoticeable"), VARIANT_FALSE)))
	        return hr;

		// fire event
		pCurWield->FireEventExt(CComBSTR("UnWielded"), NULL, FIREEVENT_DEFAULT);
	}

	// Now wield the new object
	if (FAILED(hr = pThis->put_Thing(CComBSTR("WieldedItem"), pThing)))
	    return hr;

	if (pThing)
	{
		if (FAILED(hr = pThing->put_BOOL(CComBSTR("IsWielded"), VARIANT_TRUE)))
	        return hr;

		// Make the object noticeable and send it to clients
		if (FAILED(hr = pThing->put_BOOL(CComBSTR("IsNoticeable"), VARIANT_TRUE)))
	        return hr;

		if (FAILED(hr = m_pWorld->SendContainer(soBystanders, pThing)))
	        return hr;

		// Fire the event
		pThing->FireEventExt(CComBSTR("Wielded"), NULL, FIREEVENT_DEFAULT); 
	}

	return hr;
}

//******************************************************
// Unwield
// Removes the wielded object by calling Wield with NULL

STDMETHODIMP CAvatarExemplarObject::UnWield()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CComPtr<IThing> pThis;
    HRESULT         hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

	// just invoke Wield(NULL)
	hr = pThis->InvokeMethodExt(CComBSTR("WieldItem"), 
                                CComDISPPARAMS(1, CComVariant((IDispatch*) NULL)),
                                NULL);

	return hr;
}

#if 0
//******************************************************
// RetrieveOwnedObject
// Given an object that you own, moves it back into your 
// contents directly....

STDMETHODIMP CAvatarExemplarObject::RetrieveOwnedObject(IThing * pThing)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
	CComPtr<IThing> pObjOwner;
    HRESULT         hr = S_OK;

    if (pThing == NULL)
    {
        VWTRACE(m_pWorld, "VWAVATAR", VWT_ERROR, "CAvatarExemplarObject::RetrieveOwnedObject: NULL pointer\n");
	    return ReportAvatarError(E_POINTER);
    }

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

	if (FAILED(hr = pThing->get_Owner(&pObjOwner)))
	    return hr;

	// retrieve, only if owner
	if (pObjOwner==pThis)
    {
		if (FAILED(hr = pThing->MoveIntoExt(pThis)))
	        return hr;
    }

	return hr;
}
#endif

//******************************************************
// Teleport
// Transports the avatar to another room - if there
// there is a portal in the room, go through it.
// Otherwise move the avatar into the room

STDMETHODIMP CAvatarExemplarObject::Teleport(IThing *pNewRoom)
{
	HRESULT hr;
	CComPtr<IThing> pThis;
	CComPtr<IPropertyList> pPortals;

	CComPtr<IPropertyList> pListArrive;
	CComBSTR bstrDescriptionArrive("You enter {object}.|{subject} enters {object}.");
	CComBSTR bstrNameArrive("TeleportArrive");

	static CComBSTR bstrPropPortals("Portals");
	static CComBSTR bstrMethodTransportTo("TransportTo");
	static CComBSTR bstrMethodMoveInto("MoveInto");

	hr = m_pWorld->get_This(&pThis);
	if (FAILED(hr))
		return hr;

	// If the destination room has a portal, go through that
	hr = pNewRoom->get_ObjectPropertyExt(bstrPropPortals, IID_IPropertyList, (IObjectProperty **) &pPortals);
	if (FAILED(hr))
		return hr;

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
		hr = pDestPortal->InvokeMethodExt(bstrMethodTransportTo, CComDISPPARAMS(1, CComVariant((IDispatch*) pThis)), NULL);
	}
	else
	{
		// Otherwise just move into room 
		hr = pThis->InvokeMethodExt(bstrMethodMoveInto, CComDISPPARAMS(1, CComVariant(pNewRoom)), NULL);
	}

	// Fire arrival event		
	// Create the Action event subject list
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pListArrive)))
    {
        // Helper function has produced a trace, but we need errorinfo here
	    return ReportAvatarError(hr);
    }

	if (FAILED(hr = pListArrive->AddThing(pThis)))
	    return hr;
	if (FAILED(hr = pListArrive->AddThing(pNewRoom)))
	    return hr;

	// Fire the Action event
	hr = pNewRoom->FireEventExt(CComBSTR("Action"),
							CComDISPPARAMS(3, CComVariant(bstrNameArrive),
										   CComVariant(bstrDescriptionArrive),
										   CComVariant((IDispatch*) pListArrive)),
							FIREEVENT_DEFAULT);
	return hr;
}

//******************************************************
// AddToUserProfileList
// Adds the object to a list key in this avatar's profile

STDMETHODIMP CAvatarExemplarObject::AddToUserProfileList(IThing *pThing, BSTR bstrKey)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
	CComPtr<IAvatarProfile> pProfile;
    HRESULT         hr = S_OK;

    if (pThing == NULL)
    {
        VWTRACE(m_pWorld, "VWAVATAR", VWT_ERROR, "CAvatarExemplarObject::AddToUserProfileList: NULL pointer\n");
	    return ReportAvatarError(E_POINTER);
    }

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

	if (FAILED(hr = pThis->get_ObjectProperty(CComBSTR("Profile"), (IObjectProperty **)&pProfile)))
	    return hr;

	if (FAILED(hr = _AddToProfileKeyList(m_pWorld, pProfile, bstrKey, pThing, pThis)))
	    return ReportAvatarError(hr);

    return S_OK;
}

//******************************************************
// RemoveFromUserProfileList
// Removes the object from a list key in this avatars profile

STDMETHODIMP CAvatarExemplarObject::RemoveFromUserProfileList(IThing *pThing, BSTR bstrKey)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
	CComPtr<IAvatarProfile> pProfile;
    HRESULT hr = S_OK;

    if (pThing == NULL)
    {
        VWTRACE(m_pWorld, "VWAVATAR", VWT_ERROR, "CAvatarExemplarObject::RemoveFromUserProfileList: NULL pointer\n");
	    return ReportAvatarError(E_POINTER);
    }

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

	if (FAILED(hr = pThis->get_ObjectProperty(CComBSTR("Profile"), (IObjectProperty **)&pProfile)))
	    return hr;

	if (FAILED(hr = _RemoveFromProfileKeyList(pProfile, bstrKey, pThing)))
	    return ReportAvatarError(hr);

    return S_OK;

}

//******************************************************
// IsInUserProfileList
// return TRUE if this object is in the specified list key
// in the avatar's profile

STDMETHODIMP CAvatarExemplarObject::IsInUserProfileList(IThing *pThing, BSTR bstrKey, VARIANT_BOOL *pBool)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CComPtr<IThing> pThis;
	CComPtr<IAvatarProfile> pProfile;
    HRESULT hr = S_OK;

    if ((pThing == NULL) || (pBool == NULL))
    {
        VWTRACE(m_pWorld, "VWAVATAR", VWT_ERROR, "CAvatarExemplarObject::IsInUserProfileList: NULL pointer\n");
	    return ReportAvatarError(E_POINTER);
    }

	*pBool = VARIANT_FALSE;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

	if (FAILED(hr = pThis->get_ObjectProperty(CComBSTR("Profile"), (IObjectProperty **)&pProfile)))
	    return hr;

	if(_IsObjInProfileKeyList(pProfile, bstrKey, pThing))
		*pBool = VARIANT_TRUE;

    return S_OK;
}

//******************************************************
// Query

STDMETHODIMP CAvatarExemplarObject::Query(BSTR bstrTitle, 
											BSTR bstrHTML,
											BSTR bstrPrompt, 
											BSTR bstrDefault,
											VARIANT varNotifyObject, 
											VARIANT varNotifyMethod,
											VARIANT varNotifyParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
    HRESULT hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

	if (varNotifyObject.vt != VT_DISPATCH)
	{
		varNotifyObject.vt = VT_DISPATCH;
		varNotifyObject.pdispVal = NULL;
	}
	if (varNotifyMethod.vt != VT_BSTR)
	{
		varNotifyMethod.vt = VT_BSTR;
		varNotifyMethod.bstrVal = NULL;
	}
	if (varNotifyParam.vt != VT_DISPATCH)
	{
		varNotifyParam.vt = VT_DISPATCH;
		varNotifyParam.pdispVal = NULL;
	}

	// Call ClientQuery
	hr = pThis->InvokeMethodExt(CComBSTR("ClientQuery"), 
                                CComDISPPARAMS (7, CComVariant(bstrTitle), 
                                 CComVariant(bstrHTML), CComVariant(bstrPrompt), 
                                 CComVariant(bstrDefault), CComVariant(varNotifyObject), 
                                 CComVariant(varNotifyMethod), CComVariant(varNotifyParam)),
	                            NULL);

	return hr;

}

//******************************************************
// Alert

STDMETHODIMP CAvatarExemplarObject::Alert(BSTR bstrTitle, BSTR bstrText, 
										  VARIANT varNotifyObject, 
										  VARIANT varNotifyMethod,
										  VARIANT varNotifyParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComBSTR bstrAlertHTML =
		"<BODY SCROLL=\"NO\" bgcolor=\"#F4F4E8\">\r\n"
		"<STYLE>\r\n"
		".bDisabled {background-color: white; border-color: #713252; border-style: solid; border-width: 1; color: #555a47;"
			"cursor: arrow; filter: alpha(opacity=60); font-family: verdana; font-size: 8pt; height: 20;"
			"position: absolute; text-align: center; top: 9; width: 60; }\r\n"
		".bEnabled {background-color: white; border-color: #713252; border-style: solid; border-width: 1; color: black;"
			"cursor: hand; font-family: verdana; font-size: 8pt; height: 20; position: absolute; text-align: center; top: 9; width: 60;}\r\n"
		".bHighlight {background-color: white; 	border-color: #713252; border-style: solid; border-width: 1; color: mediumvioletred;"
			"cursor: hand; font-family: verdana; font-size: 8pt; height: 20; position: absolute; text-align: center; top: 9; width: 60;}\r\n"
		".vH1 { color: #002f5f; cursor: default; font-family: verdana; font-size: 11pt; font-weight: bold; position: absolute;} \r\n"
		".vinput1{Font-Size: 8pt;Font-Family: Arial;Color: #000000}\r\n"
		".bStyle {position:absolute;background-color:#FFFFFF;top:12;width:60;height:22;cursor:hand;text-align:center;"
			"border-color: #713252; border-width: 1px; border-style: solid;"
			"color: #555a47;  font-family: verdana; font-size: 8pt; font-weight: bold; position: absolute;}\r\n"
		".vLabel { color: black; cursor: default; font-family: verdana; font-size: 8pt; font-weight: normal; }\r\n"
		"</STYLE>\r\n"
		"</HEAD>\r\n"

		// The helper object
		"<OBJECT ID=\"Helper\" CLASSID=\"clsid:7E3832CF-4F4E-11D0-89AB-00A0C9054129\"\r\n"
        "        BORDER=0 WIDTH=0 HEIGHT=0></OBJECT>\r\n"
		// The user text
		"<div class=\"vLabel\" style=left:18;top:7;>\r\n"
		"<SCRIPT LANGUAGE=\"VBScript\">document.write Helper.Arg.Property(2)</SCRIPT>\r\n"
		
		"<button class=bEnabled id=OKBtn onclick=OKClick style=left:250;top:50;>"
		"<span style=position:relative;top:-1;>OK</span>"
		"</button>\r\n"

		"<SCRIPT LANGUAGE=\"VBScript\">\r\n"

		"sub window_onLoad\r\n"
		"	document.all.item(\"OKBtn\").focus\r\n"
		"end sub\r\n"

		"sub document_OnMouseOver\r\n"
			"if window.event.srcElement.className = \"bEnabled\" then\r\n"
				"window.event.srcElement.className = \"bHighlight\"\r\n"
			"end if\r\n"
		"end sub\r\n"

		"sub document_OnMouseOut\r\n"
			"if window.event.srcElement.className = \"bHighlight\" then\r\n"
				"window.event.srcElement.className = \"bEnabled\"\r\n"
			"end if\r\n"
		"end sub\r\n"

		"Sub OKClick\r\n"
		"  helper.CloseWindow\r\n"
		"End sub\r\n"

		"Sub Window_OnUnload\r\n"
		"  Helper.Client.World.User.QueryReturn Helper.Arg, True, \"\"\r\n"
		"End sub\r\n"
		
		"</SCRIPT>\r\n"
		"</div>\r\n"
		"</BODY>\r\n";

	return Query(bstrTitle, bstrAlertHTML, bstrText, CComBSTR(""), varNotifyObject,
				   varNotifyMethod, varNotifyParam);
}

//******************************************************
// Confirm

STDMETHODIMP CAvatarExemplarObject::Confirm(BSTR bstrTitle, BSTR bstrText,
											VARIANT varNotifyObject, 
											VARIANT varNotifyMethod,
											VARIANT varNotifyParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComBSTR bstrConfirmHTML =
		"<BODY SCROLL=\"NO\" bgcolor=\"#F4F4E8\">\r\n"
		"<style>\r\n"
		".bDisabled {background-color: white; border-color: #713252; border-style: solid; border-width: 1; color: #555a47;"
			"cursor: arrow; filter: alpha(opacity=60); font-family: verdana; font-size: 8pt; height: 20;"
			"position: absolute; text-align: center; top: 9; width: 60; }\r\n"
		".bEnabled {background-color: white; border-color: #713252; border-style: solid; border-width: 1; color: black;"
			"cursor: hand; font-family: verdana; font-size: 8pt; height: 20; position: absolute; text-align: center; top: 9; width: 60;}\r\n"
		".bHighlight {background-color: white; 	border-color: #713252; border-style: solid; border-width: 1; color: mediumvioletred;"
			"cursor: hand; font-family: verdana; font-size: 8pt; height: 20; position: absolute; text-align: center; top: 9; width: 60;}\r\n"
		".vH1 { color: #002f5f; cursor: default; font-family: verdana; font-size: 11pt; font-weight: bold; position: absolute;} \r\n"
		".vinput1{Font-Size: 8pt;Font-Family: Arial;Color: #000000}\r\n"
		".bStyle {position:absolute;background-color:#FFFFFF;top:12;width:60;height:22;cursor:hand;text-align:center;"
			"border-color: #713252; border-width: 1px; border-style: solid;"
			"color: #555a47;  font-family: verdana; font-size: 8pt; font-weight: bold; position: absolute;}\r\n"
		".vLabel { color: black; cursor: default; font-family: verdana; font-size: 8pt; font-weight: normal; }\r\n"
		"</style>\r\n"
		
		// The helper object
		"<OBJECT ID=\"Helper\" CLASSID=\"clsid:7E3832CF-4F4E-11D0-89AB-00A0C9054129\"\r\n"
        "        BORDER=0 WIDTH=0 HEIGHT=0></OBJECT>\r\n"
		
		// The user text
		"<div class=\"vLabel\" style=left:18;top:7;>\r\n"
		"<SCRIPT LANGUAGE=\"VBScript\">document.write Helper.Arg.Property(2)</SCRIPT>\r\n"
		
		"<button class=bEnabled id=OKBtn onclick=OKClick style=left:174;top:50;>"
		"<span style=position:relative;top:-1;>OK</span>"
		"</button>\r\n"
		"<button class=bEnabled id=CancelBtn onclick=CancelClick style=left:250;top:50;>"
		"<span style=position:relative;top:-1;>Cancel</span>"
		"</button>\r\n"
		
		"<SCRIPT LANGUAGE=\"VBScript\">\r\n"
		"Dim returnValue\r\n"
		"returnValue = False\r\n"

		"sub document_OnMouseOver\r\n"
			"if window.event.srcElement.className = \"bEnabled\" then\r\n"
				"window.event.srcElement.className = \"bHighlight\"\r\n"
			"end if\r\n"
		"end sub\r\n"

		"sub document_OnMouseOut\r\n"
			"if window.event.srcElement.className = \"bHighlight\" then\r\n"
				"window.event.srcElement.className = \"bEnabled\"\r\n"
			"end if\r\n"
		"end sub\r\n"

		"Sub OKClick\r\n"
		"  returnValue = True\r\n"
		"  helper.CloseWindow\r\n"
		"End sub\r\n"

		"Sub CancelClick\r\n"
		"  helper.CloseWindow\r\n"
		"End sub\r\n"

		"Sub Window_OnUnload\r\n"
		"  Helper.Client.World.User.QueryReturn Helper.Arg, returnValue, \"\"\r\n"
		"End sub\r\n"
		"</SCRIPT>\r\n"
		"</div>\r\n"
		"</BODY>\r\n";

	return Query(bstrTitle, bstrConfirmHTML, bstrText, CComBSTR(""), varNotifyObject,
				   varNotifyMethod, varNotifyParam);
}

//******************************************************
// Prompt

STDMETHODIMP CAvatarExemplarObject::Prompt(BSTR bstrTitle, BSTR bstrPrompt, BSTR bstrDefault,
										   VARIANT varNotifyObject, 
										   VARIANT varNotifyMethod,
										   VARIANT varNotifyParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComBSTR bstrPromptHTML =
		"<BODY SCROLL=\"NO\" bgcolor=\"#F4F4E8\">\r\n"
		"<style>\r\n"
		".bDisabled {background-color: white; border-color: #713252; border-style: solid; border-width: 1; color: #555a47;"
			"cursor: arrow; filter: alpha(opacity=60); font-family: verdana; font-size: 8pt; height: 20;"
			"position: absolute; text-align: center; top: 9; width: 60; }\r\n"
		".bEnabled {background-color: white; border-color: #713252; border-style: solid; border-width: 1; color: black;"
			"cursor: hand; font-family: verdana; font-size: 8pt; height: 20; position: absolute; text-align: center; top: 9; width: 60;}\r\n"
		".bHighlight {background-color: white; 	border-color: #713252; border-style: solid; border-width: 1; color: mediumvioletred;"
			"cursor: hand; font-family: verdana; font-size: 8pt; height: 20; position: absolute; text-align: center; top: 9; width: 60;}\r\n"
		".vH1 { color: #002f5f; cursor: default; font-family: verdana; font-size: 11pt; font-weight: bold; position: absolute;} \r\n"
			".vinput1{Font-Size: 8pt;Font-Family: Arial;Color: #000000}\r\n"
		".bStyle {position:absolute;background-color:#FFFFFF;top:12;width:60;height:22;cursor:hand;text-align:center;"
			"border-color: #713252; border-width: 1px; border-style: solid;"
			"Font-Size: 8pt;Font-Family: Arial;Color: #000000}\r\n"
		".bHighlightStyle {position:absolute;background-color:#FFFFFF;top:12;width:60;height:22;cursor:hand;text-align:center;"
			"border-color: #713252; border-width: 2px; border-style: solid;"
			"Font-Size: 8pt;Font-Family: Arial;Color: #000000}\r\n"
		".vLabel { color: black; cursor: default; font-family: verdana; font-size: 8pt; font-weight: normal; }\r\n"
		"</style>\r\n"
	
		// The helper object
		"<OBJECT ID=\"Helper\" CLASSID=\"clsid:7E3832CF-4F4E-11D0-89AB-00A0C9054129\"\r\n"
        "        BORDER=0 WIDTH=0 HEIGHT=0></OBJECT>\r\n"
		
		// The user text
		"<div class=\"vLabel\" style=left:18;top:7;>\r\n"
		"<SCRIPT LANGUAGE=\"VBScript\">document.write Helper.Arg.Property(2)</SCRIPT>\r\n"
		"<INPUT TYPE=\"TEXT\" NAME=\"TextIn\">\r\n"		
		
		"<button class=bEnabled id=OKBtn onclick=OKClick style=left:174;top:50;>"
		"<span style=position:relative;top:-1;>OK</span>"
		"</button>\r\n"
		"<button class=bEnabled id=CancelBtn onclick=CancelClick style=left:250;top:50;>"
		"<span style=position:relative;top:-1;>Cancel</span>"
		"</button>\r\n"

		"<SCRIPT LANGUAGE=\"VBScript\">\r\n"
		"Dim returnValue\r\n"
		"returnValue = False\r\n"
		"TextIn.Value = Helper.Arg.Property(3)\r\n"
				
		"sub document_OnMouseOver\r\n"
			"if window.event.srcElement.className = \"bEnabled\" then\r\n"
				"window.event.srcElement.className = \"bHighlight\"\r\n"
			"end if\r\n"
		"end sub\r\n"

		"sub document_OnMouseOut\r\n"
			"if window.event.srcElement.className = \"bHighlight\" then\r\n"
				"window.event.srcElement.className = \"bEnabled\"\r\n"
			"end if\r\n"
		"end sub\r\n"

		"Sub OKClick\r\n"
		"  returnValue = True\r\n"
		"  helper.CloseWindow\r\n"
		"End sub\r\n"

		"Sub CancelClick\r\n"
		"  helper.CloseWindow\r\n"
		"End sub\r\n"

		"Sub Window_OnUnload\r\n"
		"  Helper.Client.World.User.QueryReturn Helper.Arg, returnValue, TextIn.Value\r\n"
		"End sub\r\n"
		"</SCRIPT>\r\n"
		"</div>\r\n"
		"</BODY>\r\n";

	return Query(bstrTitle, bstrPromptHTML, bstrPrompt, bstrDefault, varNotifyObject,
				   varNotifyMethod, varNotifyParam);
}

//******************************************************
// ClientQuery

STDMETHODIMP CAvatarExemplarObject::ClientQuery(BSTR bstrTitle, BSTR bstrHTML, BSTR bstrPrompt,
												BSTR bstrDefault, IThing *pNotifyObject,
												BSTR bstrNotifyMethod, IDispatch *pNotifyParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
	CComPtr<IThing> pUser;
    HRESULT         hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

	if (FAILED(hr = m_pWorld->get_User(&pUser)))
	    return hr;

	// only query if we're the right client
	// REVIEW: can't we just get this to happen in the right place?
	if (pUser==pThis)
	{
		CComPtr<IPropertyList> pArg;

		// Create the argument for the window
		if (FAILED(hr = CreatePropertyList(m_pWorld, &pArg)))
	        return ReportAvatarError(hr);
		if (FAILED(hr = pArg->AddString(bstrTitle)))
	        return hr;
		if (FAILED(hr = pArg->AddString(bstrHTML)))
	        return hr;
		if (FAILED(hr = pArg->AddString(bstrPrompt)))
	        return hr;
		if (FAILED(hr = pArg->AddString(bstrDefault)))
	        return hr;
		if (FAILED(hr = pArg->AddThing(pNotifyObject)))
	        return hr;
		if (FAILED(hr = pArg->AddString(bstrNotifyMethod)))
	        return hr;
		if (FAILED(hr = pArg->AddDispatch(pNotifyParam)))
	        return hr;

		if (FAILED(hr = m_pWorld->ShowHTMLExt(CComBSTR("_blank"), bstrHTML, bstrTitle,
			VARIANT_FALSE, pArg, 50, 50, 350, 115)))
	        return hr;
	}

    return S_OK;
}

//******************************************************
// QueryResult

STDMETHODIMP CAvatarExemplarObject::QueryReturn(IPropertyList *pArg,
												VARIANT_BOOL boolOK, BSTR bstrResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pArg==NULL)
    {
        VWTRACE(m_pWorld, "VWAVATAR", VWT_ERROR, "CAvatarExemplarObject::QueryReturn: NULL pointer\n");
	    return ReportAvatarError(E_POINTER);
    }

	HRESULT hr = S_OK;
	CComPtr<IThing> pThis;
	CComPtr<IThing> pNotifyObject;
	CComPtr<IDispatch> pNotifyParam;
	CComBSTR bstrNotifyMethod;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

	// REVIEW: use a defines instead of hard wired numbers!
	if (FAILED(hr = pArg->get_Dispatch(6, &pNotifyParam)))
	    return hr;

	// Save the result
	if (FAILED(hr = pThis->put_BOOL(CComBSTR("QueryOK"), boolOK)))
	    return hr;

	if (FAILED(hr = pThis->put_String(CComBSTR("QueryResult"), bstrResult)))
	    return hr;
	if (FAILED(hr = pThis->put_Dispatch(CComBSTR("QueryParam"), pNotifyParam)))
	    return hr;

	// Get the notify object (if any)
	hr = pArg->get_Thing(4, &pNotifyObject);
	if (FAILED(hr))
	{
		pThis->put_Dispatch(CComBSTR("QueryParam"), NULL);
        return hr;
	}

	// Get the notify method name
	hr = pArg->get_String(5, &bstrNotifyMethod.m_str);
	if (FAILED(hr))
	{
		pThis->put_Dispatch(CComBSTR("QueryParam"), NULL);
		return hr;
	}

	// Notify it
	if (pNotifyObject)
	{
		// OK if this fails
		pNotifyObject->InvokeMethodExt(bstrNotifyMethod, 
                                       CComDISPPARAMS(1, CComVariant((IDispatch*) pThis)),
                                       NULL);
	}

	return hr;
}


//******************************************************
// WhisperTo
// Checks if room allows whispers; if so, 
// brings up the seperate whisper window...

STDMETHODIMP CAvatarExemplarObject::WhisperTo()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    HRESULT hr = S_OK;
	CComPtr<IThing> pThis;
	CComPtr<IThing> pContainer;
	VARIANT_BOOL	boolAllowWhispers = VARIANT_FALSE;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

	// Get container and see if it allows whispers
	if (pThis)
    {
		if (FAILED(hr = pThis->get_Container(&pContainer)))
	        return hr;

		if (pContainer)
		{
			if (FAILED(hr = pContainer->get_BOOL(CComBSTR("AllowWhispers"), &boolAllowWhispers)))
				return hr;

			if (boolAllowWhispers)
			{

				if (FAILED(hr = m_pWorld->FireUIEvent(pThis, CComBSTR(VW_WHISPER_EVENT_STR), CComVariant(NULL))))
					return hr;
			}
			else 
				return(ReportAvatarError(VWAVATAR_E_WHISPERNOTALLOWED));
		}
	}

#ifdef _DEBUG
	// No error checking, if we fail here, we still want to keep going
	CComPtr<IThing> pCaller;
	CComPtr<IThing> pUser;
	CComBSTR bstrThisName, bstrOwnerName, bstrCallerName, bstrUserName;

	if (pThis)
		pThis->get_Name(&bstrThisName.m_str);

	m_pWorld->get_Caller(&pCaller);

	if (pCaller)
		pCaller->get_Name(&bstrCallerName.m_str);

	m_pWorld->get_User(&pUser);

	if (pUser)
		pUser->get_Name(&bstrUserName.m_str);
#endif

	return hr;
}

//******************************************************
// DoNamedGesture
// Performs a gesture by name

STDMETHODIMP CAvatarExemplarObject::DoNamedGesture(BSTR bstrGesture)
{
    HRESULT hr = S_OK;
	CComPtr<IThing>	pThis;
	CComPtr<IAvatarProfile>	pProfile;
	static CComBSTR kbstrProfile("Profile");
	static CComBSTR kbstrNamedGestures("NamedGestures");
	static CComBSTR kbstrDoGesture("DoGesture");

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;
	
	if (SUCCEEDED(pThis->get_ObjectProperty(kbstrProfile, (IObjectProperty **)&pProfile)))
	{
		// check to see if the user has a GestureText key in their profile
		CComVariant	varKeyVal;

		if (SUCCEEDED(pProfile->get_KeyValue(kbstrNamedGestures, &varKeyVal)) &&
			(varKeyVal.vt == VT_DISPATCH))
		{
			CComPtr<IPropertyMap>	pGestMap = (IPropertyMap *)varKeyVal.pdispVal;
			long	lGestNum = 0;

			if (SUCCEEDED(pGestMap->get_Long(bstrGesture, &lGestNum)))
			{
				// Do the gesture that we find there
				hr = pThis->InvokeMethodExt(kbstrDoGesture, CComDISPPARAMS(1, CComVariant(lGestNum)), NULL);
                return hr;
			}
		}
	}

	// Can't find it - invoke the super method
	hr = pThis->InvokeSuperMethod(CComVariant(bstrGesture), NULL);
    return hr;
}


//******************************************************
// GestureText
// Given a gesture name, returns the text associated with
// it.  For avatars, we attempt to find the gesture in the
// profile.  If we can't find it, we call the inherited
// method.

STDMETHODIMP CAvatarExemplarObject::GestureText(BSTR bstrGesture, BSTR *pbstrText)
{
	CComPtr<IThing>	pThis;
	CComPtr<IAvatarProfile>	pProfile;
	static CComBSTR kbstrProfile("Profile");
	static CComBSTR kbstrGestureText("GestureText");
	CComVariant varRetVal;
    HRESULT hr = S_OK;

    if (pbstrText == NULL)
    {
        VWTRACE(m_pWorld, "VWAVATAR", VWT_ERROR, "CAvatarExemplarObject::GestureText: NULL pointer\n");
	    return ReportAvatarError(E_POINTER);
    }

	*pbstrText = NULL;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;
	
	if (SUCCEEDED(pThis->get_ObjectProperty(kbstrProfile, (IObjectProperty **)&pProfile)) && pProfile)
	{
		// check to see if the user has a GestureText key in their profile
		CComVariant	varKeyVal;

		if (SUCCEEDED(pProfile->get_KeyValue(kbstrGestureText, &varKeyVal)) &&
			(varKeyVal.vt == VT_DISPATCH))
		{
			CComPtr<IPropertyMap>	pGestMap = (IPropertyMap *)varKeyVal.pdispVal;

			if (SUCCEEDED(pGestMap->get_String(bstrGesture, pbstrText)))
				return S_OK;
		}

	}

	// Can't find it - invoke the super method
	if (FAILED(hr = pThis->InvokeSuperMethod(CComVariant(bstrGesture), &varRetVal)))
	    return hr;

	if (varRetVal.vt == VT_BSTR)
		*pbstrText = CComBSTR(varRetVal.bstrVal).Copy();

    return S_OK;
}


//******************************************************
// ThumbnailFileName
// Returns the full local filename of the avatar's
// thumbnail image.  IF the avatar has a geometry,
// it will use that.  Otherwise, it will attempt to
// get it from the profile.

const char	kstrDefaultThumbNailName[] = "tdefault.gif";

#include <inetfile.h>

STDMETHODIMP CAvatarExemplarObject::ThumbnailFileName(BSTR *pbstrFileName)
{
    HRESULT hr = S_OK;
	CComBSTR bstrImageFileName;
	static CComBSTR kbstrINetFile("INetFile");
	
    if (pbstrFileName == NULL)
    {
        VWTRACE(m_pWorld, "VWAVATAR", VWT_ERROR, "CAvatarExemplarObject::ThumbnailFileName: NULL pointer\n");
	    return ReportAvatarError(E_POINTER);
    }

    bstrImageFileName = kstrDefaultThumbNailName;

	if (bstrImageFileName.Length())
	{
		CComPtr<IInternetFileManager> pINetFile;
		
        if (FAILED(hr = m_pWorld->get_Tool(kbstrINetFile, (IUnknown **)&pINetFile)))
        	return hr;

		if (pINetFile)
		{
			CComBSTR bstrFinalURL;

			if (FAILED(hr = pINetFile->GetFileEx(bstrImageFileName, VARIANT_TRUE, &bstrFinalURL.m_str, &bstrImageFileName.m_str)))
	            return hr;
		}
	}

	if (bstrImageFileName.Length())
	{
		*pbstrFileName = bstrImageFileName.Copy();
	}

    return S_OK;
}

//******************************************************
// OnContainerSystemTell
// Default does nothing

STDMETHODIMP CAvatarExemplarObject::OnContainerSystemTell(IThing* pfrom, IPropertyList* pto, BSTR bstr, long lval, BSTR *pbstrOut)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if ((pbstrOut == NULL) || (pfrom == NULL ))
    {
        VWTRACE(m_pWorld, "VWAVATAR", VWT_ERROR, "CAvatarExemplarObject::OnContainerSystemTell: NULL pointer\n");
	    return ReportAvatarError(E_POINTER);
    }

	// default
	*pbstrOut = NULL;

#ifdef _DEBUG

	CString strSay;
	CComBSTR bstrName;
	CString	strText(bstr);
	CString	strWho;

	// get user name
	ASSERT(SUCCEEDED(pfrom->get_Name(&bstrName.m_str)));
	strWho = bstrName;

	if (lval == tcSay)
		strSay = "said";
	else if (lval == tcEmote)
		strSay = "emoted";
	else if (lval == tcThink)
		strSay = "thought";
	else if (lval == tcWhisper)
		strSay = "whispered";
	else if (lval == tcShout)
		strSay = "shouted";

	VWTRACE(m_pWorld, "VWMODULE", VWT_VERBOSE, "CAvatarExemplarObject::OnContainerSystemTell: %s %s, \"%s\"\n", strWho, strSay, strText);

#endif

	return S_OK;
}

//******************************************************
// OnSystemTell
// Default does nothing

STDMETHODIMP CAvatarExemplarObject::OnSystemTell(IThing* pfrom, IPropertyList* pto, BSTR bstr, long lval, BSTR *pbstrOut)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if ((pbstrOut == NULL) || (pfrom == NULL ))
    {
        VWTRACE(m_pWorld, "VWAVATAR", VWT_ERROR, "CAvatarExemplarObject::OnSystemTell: NULL pointer\n");
	    return ReportAvatarError(E_POINTER);
    }

	*pbstrOut = NULL;

#ifdef _DEBUG
	CString strSay;
	CComBSTR bstrName;
	CString	strText(bstr);
	CString	strWho;

	// get user name
	ASSERT(SUCCEEDED(pfrom->get_Name(&bstrName.m_str)));
	strWho = bstrName;

	if (lval == tcSay)
		strSay = "said";
	else if (lval == tcEmote)
		strSay = "emoted";
	else if (lval == tcThink)
		strSay = "thought";
	else if (lval == tcWhisper)
		strSay = "whispered";
	else if (lval == tcShout)
		strSay = "shouted";

	VWTRACE(m_pWorld, "VWMODULE", VWT_VERBOSE, "CAvatarExemplarObject::OnSystemTell: %s %s, \"%s\"\n", strWho, strSay, strText);

#endif

	return S_OK;
}


//******************************************************
// OnPeerSystemTell
// Default does nothing...

STDMETHODIMP CAvatarExemplarObject::OnPeerSystemTell(IThing* pfrom, IPropertyList* pto, BSTR bstr, long lval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pfrom == NULL )
    {
        VWTRACE(m_pWorld, "VWAVATAR", VWT_ERROR, "CAvatarExemplarObject::OnPeerSystemTell: NULL pointer\n");
	    return ReportAvatarError(E_POINTER);
    }

#ifdef _DEBUG

	CString strSay;
	CComBSTR bstrName;
	CString	strText(bstr);
	CString	strWho;

	// get user name
	ASSERT(SUCCEEDED(pfrom->get_Name(&bstrName.m_str)));
	strWho = bstrName;

	if (lval == tcSay)
		strSay = "said";
	else if (lval == tcEmote)
		strSay = "emoted";
	else if (lval == tcThink)
		strSay = "thought";
	else if (lval == tcWhisper)
		strSay = "whispered";
	else if (lval == tcShout)
		strSay = "shouted";

	VWTRACE(m_pWorld, "VWMODULE", VWT_VERBOSE, "CAvatarExemplarObject::OnPeerSystemTell: %s %s, \"%s\"\n", strWho, strSay, strText);

#endif

	return S_OK;
}

//******************************************************
// OnSystemConnect
// Transport user through defaultportal if one is defined


STDMETHODIMP CAvatarExemplarObject::OnSystemConnect(IThing* pWho) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    VWTRACE(m_pWorld, "VWAVATAR", VWT_METHOD, "CAvatarExemplarObject::OnSystemConnect\n");

	if (pWho == NULL )
    {
        VWTRACE(m_pWorld, "VWAVATAR", VWT_ERROR, "CAvatarExemplarObject::OnSystemConnect: NULL pointer\n");
	    return ReportAvatarError(E_POINTER);
    }

	CComPtr<IThing> pThis;
	CComPtr<IThing> pGlobal;
	CComPtr<IThing> pDefaultPortal;
	CComPtr<IThing> pDefaultRoom;
	CComPtr<IThing> pNewContainer;
	CComPtr<IPropertyList> pAvatarList;
    HRESULT         hr = S_OK;

	// Now get default portal for the world and transport user through it
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

	if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
	    return hr;

	if (FAILED(hr = pGlobal->get_ObjectProperty(CComBSTR("DefaultPortal"), (IObjectProperty**)&pDefaultPortal)))
	    return hr;

	if (FAILED(hr = pGlobal->get_ObjectProperty(CComBSTR("DefaultRoom"), (IObjectProperty**)&pDefaultRoom)))
	    return hr;

	if (pDefaultPortal)
	{
		if (FAILED(hr = pDefaultPortal->InvokeMethodExt(CComBSTR("TransportTo"),
    										CComDISPPARAMS(1, CComVariant((IDispatch*) pWho)),
	    									NULL)))
    	    return hr;

		VWTRACE(m_pWorld, "VWAVATAR", VWT_IMPORTANT, "CAvatarExemplarObject::OnSystemConnect: Called Transport\n");

		if (FAILED(hr = pDefaultPortal->get_Container(&pNewContainer)))
			return hr;

	}
	else if (pDefaultRoom)
	{
		if (FAILED(hr = pWho->InvokeMethodExt(CComBSTR("MoveInto"),
							CComDISPPARAMS(1, CComVariant((IDispatch*)pDefaultRoom)),
							NULL)))
			return hr;

		VWTRACE(m_pWorld, "VWAVATAR", VWT_IMPORTANT, "CAvatarExemplarObject::OnSystemConnect: Called MoveIntoExt\n");
		pNewContainer = pDefaultRoom;
	}
	else
	{
		// No default room or portal: this is not an error, trace out a message
		VWTRACE(m_pWorld, "VWAVATAR", VWT_IMPORTANT, "CAvatarExemplarObject::OnSystemConnect: No default room or portal specified for world\n");
		return S_OK; 
	}

	// Fire UI event to display message in history pane that you have entered world
	static CComBSTR kbstrEvent(VW_ENTERWORLD_EVENT_STR);

	if (FAILED(hr = pNewContainer->get_ObjectProperty(CComBSTR("Avatars"), (IObjectProperty**)&pAvatarList)))
	        return hr;


	if (pGlobal)
	{
		pGlobal->InvokeMethodExt(CComBSTR("FireRemoteUIEvent"),  
								  CComDISPPARAMS(4, CComVariant((IDispatch *) pAvatarList), 
								  CComVariant((IDispatch *) pThis), 
								  CComVariant(kbstrEvent), 
								  CComVariant()), NULL);
	}

    return hr;
}

//******************************************************
// OnSystemDisconnect
// Fire UI event "OnLeaveWorld"
// Can be used to update UI when an avatar logs off


STDMETHODIMP CAvatarExemplarObject::OnSystemDisconnect(IThing* pWho) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    VWTRACE(m_pWorld, "VWAVATAR", VWT_METHOD, "CAvatarExemplarObject::OnSystemDisonnect\n");

	if (pWho == NULL )
    {
        VWTRACE(m_pWorld, "VWAVATAR", VWT_ERROR, "CAvatarExemplarObject::OnSystemDisconnect: NULL pointer\n");
	    return ReportAvatarError(E_POINTER);
    }

	CComPtr<IThing> pThis;
	CComPtr<IThing> pGlobal;
	CComPtr<IThing> pContainer;
	CComPtr<IPropertyList> pAvatarList;
	static CComBSTR kbstrEvent(VW_LEAVEWORLD_EVENT_STR);
    HRESULT         hr = S_OK;

	hr = m_pWorld->get_This(&pThis);
	if (FAILED(hr))
		return hr;

	if (pThis)
	{
		hr = pThis->get_Container(&pContainer);
		if (FAILED(hr))
			return hr;

		if (pContainer)
		{
			hr = pContainer->get_ObjectProperty(CComBSTR("Avatars"), (IObjectProperty**)&pAvatarList);
			if (FAILED(hr))
				return hr;
		}
	}

	hr = m_pWorld->get_Global(&pGlobal);
	if (FAILED(hr))
		return hr;

	// Fire UI event to display message in history pane that you are leaving the world
	if (pGlobal)
	{
		pGlobal->InvokeMethodExt(CComBSTR("FireRemoteUIEvent"),  
								  CComDISPPARAMS(4, CComVariant((IDispatch *) pAvatarList), 
								  CComVariant((IDispatch *) pThis), 
								  CComVariant(kbstrEvent), 
								  CComVariant()), NULL);
	}

    return hr;
}

STDMETHODIMP CAvatarExemplarObject::OnSystemPreCreate(IThing *pExemplar, VARIANT_BOOL *pbOkToCreate)
{
	HRESULT hr = S_OK;
	long lQuota;
	CComPtr<IThing> pThis;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pbOkToCreate==NULL)
    {
        VWTRACE(m_pWorld, "VWAVATAR", VWT_ERROR, "CAvatarExemplarObject::OnSystemPreCreate: NULL pointer\n");
	    return ReportAvatarError(E_POINTER);
    }

	*pbOkToCreate = VARIANT_FALSE;

	// get this
	hr = m_pWorld->get_This(&pThis);
	if (FAILED(hr))
		return hr;

	// get our quota
	hr = pThis->get_Long(CComBSTR("ObjectQuota"), &lQuota);
	if (FAILED(hr))
		return hr;

	// see if we have sufficient (or infinite) quota
	if (lQuota > 0 || lQuota == -1)
	{
		// yep
		*pbOkToCreate = VARIANT_TRUE;
	}
#ifdef ALLOWSECURITYOVERRIDE
	else
	{
		VARIANT_BOOL bSecure = VARIANT_FALSE;

		/* hr = */ m_pWorld->get_EnableSecurity(&bSecure);

		if (bSecure == VARIANT_FALSE)
		{
			VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CAvatarExemplarObject::OnSystemPreCreate: Insufficient object quota (%d) - ignoring because security is off\n", lQuota);

			*pbOkToCreate = VARIANT_TRUE;
		}
	}
#else
	else
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CAvatarExemplarObject::OnSystemPreCreate: Insufficient object quota (%d)\n", lQuota);

#endif

	return S_OK;
}

STDMETHODIMP CAvatarExemplarObject::OnSystemPostCreate(IThing *pThing)
{
	HRESULT hr = S_OK;
	long lQuota;
	CComPtr<IThing> pThis;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pThing==NULL)
    {
        VWTRACE(m_pWorld, "VWAVATAR", VWT_ERROR, "CAvatarExemplarObject::OnSystemPostCreate: NULL pointer\n");
	    return ReportAvatarError(E_POINTER);
    }

	// get this
	hr = m_pWorld->get_This(&pThis);
	if (FAILED(hr))
		return hr;

	// get our quota
	hr = pThis->get_Long(CComBSTR("ObjectQuota"), &lQuota);
	if (FAILED(hr))
		return hr;

	// NOTE: lQuota can be >=0 or -1. It can be zero because security
	// is disabled.  It can be -1 because the object has infinite quota.
	// Only decrement it if positive

	if (lQuota>0)
	{
		lQuota--;

		CSetUID SetUID(m_pWorld, pThis, pThis, VARIANT_TRUE);

		hr = pThis->put_Long(CComBSTR("ObjectQuota"), lQuota);
		if (FAILED(hr))
			return hr;
	}
	else if (lQuota != -1)
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CAvatarExemplarObject::OnSystemPostCreate: Invalid object quota (%d)\n", lQuota);

	return S_OK;
}

STDMETHODIMP CAvatarExemplarObject::OnSystemPreDestroy(IThing *pThing, VARIANT_BOOL *pbOkToDestroy)
{
	// REVIEW: might want to check that we're really the owner

	if (pbOkToDestroy==NULL)
    {
        VWTRACE(m_pWorld, "VWAVATAR", VWT_ERROR, "CAvatarExemplarObject::OnSystemPreDestroy: NULL pointer\n");
	    return ReportAvatarError(E_POINTER);
    }

	*pbOkToDestroy = VARIANT_TRUE;

	return S_OK;
}

STDMETHODIMP CAvatarExemplarObject::OnSystemPostDestroy(IThing *pThing)
{
	HRESULT hr = S_OK;
	long lQuota;
	CComPtr<IThing> pThis;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pThing==NULL)
    {
        VWTRACE(m_pWorld, "VWAVATAR", VWT_ERROR, "CAvatarExemplarObject::OnSystemPostDestroy: NULL pointer\n");
	    return ReportAvatarError(E_POINTER);
    }

	// get this
	hr = m_pWorld->get_This(&pThis);
	if (FAILED(hr))
		return hr;

	// get our quota
	hr = pThis->get_Long(CComBSTR("ObjectQuota"), &lQuota);
	if (FAILED(hr))
		return hr;

	if (lQuota>=0)
	{
		lQuota++;

		CSetUID SetUID(m_pWorld, pThis, pThis, VARIANT_TRUE);
		hr = pThis->put_Long(CComBSTR("ObjectQuota"), lQuota);
		if (FAILED(hr))
			return hr;
	}
	else if (lQuota<0 && lQuota!=-1)
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "Thing.OnSystemPostDestroy: Invalid object quota.\n");

	return S_OK;
}

STDMETHODIMP CAvatarExemplarObject::SetObjectQuota(long lQuota)
{
	HRESULT hr = S_OK;
	CComPtr<IThing> pThis;
	VARIANT_BOOL bOk;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// get this
	hr = m_pWorld->get_This(&pThis);
	if (FAILED(hr))
		return hr;

	// Check policy (user must be wizard)
	hr = m_pWorld->CheckPolicy(pThis, CP_WIZARD, CComVariant(0L, VT_ERROR), &bOk);
	if (FAILED(hr))
		return hr;

	// do it
	if (bOk == VARIANT_TRUE)
		hr = pThis->put_Long(CComBSTR("ObjectQuota"), lQuota);

	return hr;
}

//******************************************************
// AddDefaultKeys
// Fills in the default keys that every profile is
// supposed to have. 

STDMETHODIMP CAvatarExemplarObject::AddDefaultKeys()
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
	static CComBSTR bstrProfile("Profile");

	static CComBSTR bstrDefaultWhereFrom("");
	static CComBSTR bstrDefaultHomePage("");
	static CComBSTR bstrDefaultLastWorld("");
	static CComBSTR bstrDefaultQuote("");

	if (FAILED(hr = m_pWorld->get_This(&pAvatar)))
    	return hr;

	if (pAvatar)
	{
		if (FAILED(hr = pAvatar->get_ObjectPropertyExt(bstrProfile, IID_IAvatarProfile, (IObjectProperty **)&pProfile)))
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

	hr = pAvatar->InvokeSuperMethodExt(CComDISPPARAMS(1, CComVariant(pProfile)), NULL);
    return hr;
}

STDMETHODIMP CAvatarExemplarObject::OnAction(BSTR bstrAction, BSTR bstrDescription, IPropertyList *pParticipants)
{
	HRESULT hr = S_OK;

	// Get This
	CComPtr<IThing> pThis;
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	// Get the user
	CComPtr<IThing> pUser;
	if (FAILED(hr = m_pWorld->get_User(&pUser)))
		return hr;
	if (pUser==NULL)
		return S_OK;

	// Only pay attention to the event if it's for the user's avatar
	if (pUser == pThis)
	{
		// Get the subject
		CComPtr<IThing> pSubject;
		if (pParticipants)
		{
			if (FAILED(hr = pParticipants->get_Thing(0, &pSubject)))
				return hr;
		}
		else
		{
			// REVIEW: else what?  Do we just rely on pSubject being NULL and 
			// _IsObjInProfileKeyList failing?
		}

		// If we're ignoring the subject of this action, don't report it
		CComPtr<IAvatarProfile> pProfile;
		if (FAILED(hr = pUser->get_ObjectPropertyExt(CComBSTR("Profile"), IID_IAvatarProfile,
 												(IObjectProperty **) &pProfile)))
			return hr;

		if (pProfile==NULL)
        {
            // an error in an event handler is ignored, but verbose tracing will catch it
            VWTRACE(m_pWorld, "VWAVATAR", VWT_VERBOSE, "CAvatarExemplarObject::OnAction: couldn't find avatar profile\n");
	        return ReportAvatarError(VWAVATAR_E_NOPROFILE);
        }

		if (!_IsObjInProfileKeyList(pProfile, CComBSTR("Ignoring"), pSubject))
		{
			// Format the report
			CComBSTR bstrReport;
			if (FAILED(hr = m_pWorld->FormatSentences(bstrDescription, pParticipants, pUser,
						&bstrReport.m_str)))
				return hr;

			// Only report if we ended up with a non-empty string
			if (bstrReport && (bstrReport.Length() > 0))
			{
				// Fire OnContentSystemTell only in local container
				CComPtr<IThing> pContainer;
				if (FAILED(hr = pThis->get_Container(&pContainer)) || !pContainer)
					return hr;
				
				{
					CSetUID SetUID(m_pWorld, pContainer, pContainer);
					if (FAILED(hr = pContainer->InvokeMethodExt(CComBSTR("OnContentSystemTell"),
    									CComDISPPARAMS(4, CComVariant((IDispatch*) pSubject),
	    											   CComVariant((IDispatch*) pParticipants),
		    										   CComVariant(bstrReport),
			    									   CComVariant((long) tcAction)),
				    					NULL)))
						return hr;
				}
			}
		}
	}

    return S_OK;
}


//********************************************************
// OnIsAuthorChanged
// Fires UI event to update client's UI

STDMETHODIMP CAvatarExemplarObject::OnIsAuthorChanged(long lHint, VARIANT varHintData)
{
	HRESULT hr = S_OK;
	CComPtr<IThing> pThis;
	static CComBSTR kbstrEvent(VW_ONISAUTHORCHANGED_EVENT_STR);

	// ignore add/remove
	if (lHint != thPut)
		return S_OK;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (SUCCEEDED(hr = m_pWorld->get_This(&pThis)))
    	hr = m_pWorld->FireUIEvent(pThis, kbstrEvent, CComVariant());

	return hr;
}

//********************************************************
// OnIsWizardOrIsAuthorAccess
// Checks policy:
//  - Only wizards can make other avatars wizards
//  - No one, not even the world owner, can change these properties
//	on the world owner - don't want to take away wizardness or 
//	authorship from the world owner.
//  - An avatar cannot take away Wizardness or Authorship on him/herself

STDMETHODIMP CAvatarExemplarObject::OnIsWizardOrIsAuthorAccess(long lPerms, VARIANT_BOOL* bReturnValue)
{
	HRESULT hr = S_OK;
	CComPtr<IThing> pThis;
	CComPtr<IThing> pUser;
	CComPtr<IThing> pGlobal;
	CComPtr<IThing> pWorldOwner;
	CComPtr<IThing> pCaller;
	VARIANT_BOOL bOk = VARIANT_FALSE;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// If you have a pointer, set return value false by default
	if (bReturnValue == NULL)
    {
        VWTRACE(m_pWorld, "VWAVATAR", VWT_ERROR, "CAvatarExemplarObject::OnIsWizardAccess: NULL pointer\n");
	    return ReportAvatarError(E_POINTER);
    }

	if (lPerms != PS_WRITE)
	{
		*bReturnValue = VARIANT_TRUE;
		return S_OK;
	}
	else
		*bReturnValue = VARIANT_FALSE;

	// get this
	hr = m_pWorld->get_This(&pThis);
	if (FAILED(hr))
		return hr;

	// Get global and then world owner
	hr = m_pWorld->get_Global(&pGlobal);
	if (FAILED(hr))
		return hr;

	hr = pGlobal->get_Owner(&pWorldOwner);
	if (FAILED(hr))
		return hr;

	hr = m_pWorld->get_User(&pUser);
	if (FAILED(hr))
		return hr;

	hr = m_pWorld->get_Caller(&pCaller);
	if (FAILED(hr))
		return hr;

#ifdef _DEBUG
	// No error checking, if we fail in these traces, we still want to keep going
	CComPtr<IThing>  pOwner;
	CComBSTR bstrThisName, bstrOwnerName, bstrCallerName, bstrUserName;

	if (pThis)
	{
		pThis->get_Name(&bstrThisName.m_str);
		pThis->get_Owner(&pOwner);

		if (pOwner)
			pOwner->get_Name(&bstrOwnerName.m_str);
	}

    if (pCaller)
		pCaller->get_Name(&bstrCallerName.m_str);

	if (pUser)
		pUser->get_Name(&bstrUserName.m_str);
#endif

	// We don't allow the world owner to take away his/her wizardness
	if ((pThis) && (pWorldOwner))
	{
		if (pThis == pWorldOwner)
			return S_OK;
	}

	// We don't allow you to set these properties on yourself
	if ((pThis) && (pCaller))
	{
		if (pThis == pCaller)
			return S_OK;
	}

	// Otherwise, call check policy (caller must be wizard)
	hr = m_pWorld->CheckPolicy(pCaller, CP_WIZARD, CComVariant(0L, VT_ERROR), &bOk);
	if (FAILED(hr))
		return hr;

	// OK, return true and S_OK
	if (bOk == VARIANT_TRUE)
		*bReturnValue = VARIANT_TRUE;

	return hr;
}

//********************************************************
// OnWizardAccess
// Checks policy for properties, like ObjectQuota on an avatar,
// that only wizards can change

STDMETHODIMP CAvatarExemplarObject::OnWizardAccess(long lPerms, VARIANT_BOOL* bReturnValue)
{
	HRESULT hr = S_OK;
	CComPtr<IThing> pCaller;
	VARIANT_BOOL bOk = VARIANT_FALSE;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// If you have a pointer, set return value false by default
	if (bReturnValue == NULL)
    {
        VWTRACE(m_pWorld, "VWAVATAR", VWT_ERROR, "CAvatarExemplarObject::OnIsWizardAccess: NULL pointer\n");
	    return ReportAvatarError(E_POINTER);
    }

	// Otherwise check if caller is a wizard
	hr = m_pWorld->get_Caller(&pCaller);
	if (FAILED(hr))
		return hr;

	if (pCaller)
	{
		hr = m_pWorld->CheckPolicy(pCaller, CP_WIZARD, CComVariant(0L, VT_ERROR), &bOk);
		if (FAILED(hr))
			return hr;

		// OK, return true and S_OK
		if (bOk == VARIANT_TRUE)
			*bReturnValue = VARIANT_TRUE;
	}

	return hr;
}


STDMETHODIMP CAvatarExemplarObject::FireConnectAvatar(IThing *pthing)
{
    ASSERT(pthing);

	// package up args into DISPPARAMS struct
	CComVariant     varThing((IDispatch*)pthing);
	CComDISPPARAMS  dispparams(1, varThing);

	// fire system connect event to bystanders
	pthing->FireEventExt(CComBSTR("SystemConnect"), dispparams, FIREEVENT_DEFAULT);

	// fire connect event to bystanders
	pthing->FireEventExt(CComBSTR("Connect"), dispparams, FIREEVENT_DEFAULT);

	return S_OK;
}
