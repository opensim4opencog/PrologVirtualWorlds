// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include <vwfound.h>
#include <vector.h>
#include <vwutils.h>
#include <menuitem.h>
#include <syshelp.h>
#include <mmhelp.h>
#include <profhelp.h>
#include <vwevents.h>
#include "foundatn.h"

// REVIEW: remove this hard wiring when vwcore.idl is integrated into vwsystem
#define tcAction 8

// Note:  Need to clean/update ignore functionality to use CComDISPPARAMS
// Also, need to move friendship, ignore functionality to system, MM

///////////////////////////////////////////////////////////////////////////////
// Helpers
void	_FinalizeAvatarUnIgnore(IWorld *pWorld, IThing *pUser, IThing *pAvatarToTurf);
void	_FinalizeAvatarIgnore(IWorld *pWorld, IThing *pUser, IThing *pAvatarToTurf);
void	_AlertUser(IThing *pUser, CString title, CString strPrompt);
HRESULT	_PutClientGeometryIfIgnored(IWorld *pWorld, IThing *pUser, IThing *pAvatar);

///////////////////////////////////////////////////////////////////////////////
// Avatar extensions

HRESULT CFoundationExemplars::AvaInstall(IModule *pModule)
{
	CMethodInfo rgMethod[] =
	{
		{ METHOD_CLIENT, "IgnoreAvatarMenu",	"AvaIgnoreAvatarMenu",		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "UpdateIgnore",		"AvaUpdateIgnore",			PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "UnIgnoreAvatar",		"AvaUnIgnoreAvatar",		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "IgnoreAvatar",		"AvaIgnoreAvatar",			PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "BefriendAvatarMenu",	"AvaBefriendAvatarMenu",	PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "UpdateFriend",		"AvaUpdateFriend",			PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER, "BefriendAvatar",		"AvaBefriendAvatar",		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER, "UnBefriendAvatar",	"AvaUnBefriendAvatar",	 	PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "ConfirmFriend",		"AvaConfirmFriend", 		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER, "FinalizeFriend",		"AvaFinalizeFriend",		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "GetMenuItems",		"AvaGetMenuItems",			PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
	//	{ METHOD_CLIENT, "UpdateInteractions",	"AvaUpdateInteractions",	PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD},
		{ METHOD_CLIENT, "GetPeerMenuItems",	"AvaGetPeerMenuItems",		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "OnPreComposeGeometry","AvaOnPreComposeGeometry",	PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD }
	};
	int cMethod = sizeof(rgMethod)/sizeof(rgMethod[0]);

	DISPPARAMS dispparams;
	InitializeDISPPARAMS(&dispparams, 0);

	CComPtr<IThing>			pAvatarEx;
	CComPtr<IPropertyList>	pMenu;
	CComPtr<IMenuItem>		pMenuItem;
	CComPtr<IPropertyList>  pIgnoreMenu;
	CComPtr<IPropertyList>  pIgnorePeerMenu;
	CComPtr<IPropertyList>  pIgnoredByMenu;
	CComBSTR				bstrName;
	CComBSTR				bstrNameInternal;
    HRESULT                 hr;
    int                     iMethod;

	// find the avatar exemplars
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
		
		if (FAILED(hr = pAvatarEx->CreateAndAddMethodExt(	rgMethod[iMethod].lFlags,
														pModule,
														bstrNameInternal,
														bstrName,
														rgMethod[iMethod].psbits,
														rgMethod[iMethod].permissions)))
    	    goto ERROR_ENCOUNTERED;
	}

	// amend the "Menu" menu
	if (FAILED(hr = pAvatarEx->get_ObjectPropertyExt(CComBSTR("Menu"), IID_IPropertyList, (IObjectProperty **) &pMenu)))
	    goto ERROR_ENCOUNTERED;

	if (FAILED(hr = CreateMenuItemExt(m_pWorld, L"Ignore...", 0, pAvatarEx, L"UpdateIgnore", L"IgnoreAvatarMenu", NULL, &pMenuItem)))
	    goto ERROR_ENCOUNTERED;
//		if (FAILED(hr = pMenuItem->put_SortOrder(MI_SORT_PRIMARY+50)))
//	        goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMenu->AddObjectProperty(pMenuItem)))
    	goto ERROR_ENCOUNTERED;
	pMenuItem.Release();

	if (FAILED(hr = CreateMenuItemExt(m_pWorld, L"Add to Friends list...", 0, pAvatarEx, L"UpdateFriend", L"BefriendAvatarMenu", NULL, &pMenuItem)))
    	goto ERROR_ENCOUNTERED;
//		if (FAILED(hr = pMenuItem->put_SortOrder(MI_SORT_PRIMARY+40)))
//        	goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMenu->AddObjectProperty(pMenuItem)))
    	goto ERROR_ENCOUNTERED;
	pMenuItem.Release();

	pMenu.Release();

    // ignore menu

	// make a copy (since not COPYONGET)
	if (FAILED(hr = CreateMenu(m_pWorld, &pIgnoreMenu)))
    	goto ERROR_ENCOUNTERED;
	if (FAILED(hr = CreateMenu(m_pWorld, &pIgnorePeerMenu)))
	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = CreateMenu(m_pWorld, &pIgnoredByMenu)))
	    goto ERROR_ENCOUNTERED;

	if (FAILED(hr = CreateMenuItemExt(m_pWorld, L"Ignore...", 0, pAvatarEx, L"UpdateIgnore", L"IgnoreAvatarMenu", NULL, &pMenuItem)))
	    goto ERROR_ENCOUNTERED;
//		if (FAILED(hr = pMenuItem->put_SortOrder(MI_SORT_PRIMARY)))
//	        goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pIgnoreMenu->AddObjectProperty(pMenuItem)))
    	goto ERROR_ENCOUNTERED;
	pMenuItem.Release();

	if (FAILED(hr = CreateMenuItemExt(m_pWorld, L"This user is ignoring you", MI_DISABLED | MI_GRAYED, pAvatarEx,NULL, NULL, NULL, &pMenuItem)))
    	goto ERROR_ENCOUNTERED;
//		if (FAILED(hr = pMenuItem->put_SortOrder(MI_SORT_PRIMARY)))
//	        goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pIgnoredByMenu->AddObjectProperty(pMenuItem)))
    	goto ERROR_ENCOUNTERED;
	pMenuItem.Release();
			
	if (FAILED(hr = CreateMenuItemExt(m_pWorld, L"Ignore...", 0, pAvatarEx, L"UpdateIgnore", L"IgnoreAvatarMenu", NULL, &pMenuItem)))
    	goto ERROR_ENCOUNTERED;
//		if (FAILED(hr = pMenuItem->put_SortOrder(MI_SORT_PRIMARY+50)))
//	        goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pIgnoredByMenu->AddObjectProperty(pMenuItem)))
    	goto ERROR_ENCOUNTERED;
	pMenuItem.Release();

	if (FAILED(hr = pAvatarEx->AddObjectProperty(CComBSTR("IgnoreMenu"), pIgnoreMenu)))
    	goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAvatarEx->AddObjectProperty(CComBSTR("IgnorePeerMenu"), pIgnorePeerMenu)))
	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAvatarEx->AddObjectProperty(CComBSTR("IgnoredByMenu"), pIgnoredByMenu)))
	    goto ERROR_ENCOUNTERED;

	// amend the "Peer" menu
	hr = pAvatarEx->get_ObjectPropertyExt(CComBSTR("PeerMenu"), IID_IPropertyList, (IObjectProperty **) &pMenu);

	// Add an Interact sub-menu
	/*
	// REVIEW: davevr has removed interactions as per spec.
	if (FAILED(hr = CreateMenuItemExt(m_pWorld, CComBSTR("Interact"), 0, NULL, CComBSTR("UpdateInteractions"), NULL, NULL, &pMenuItem)))
	    goto ERROR_ENCOUNTERED;
//		if (FAILED(hr = pMenuItem->put_SortOrder(MI_SORT_PRIMARY+60)))
	        goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMenu->AddObjectProperty(pMenuItem)))
	    goto ERROR_ENCOUNTERED;
	pMenuItem.Release();
	*/

ERROR_ENCOUNTERED:

    TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	return hr;
}

//******************************************************
// IgnoreAvatarMenu
// Causes the user to ignore this avatar

STDMETHODIMP CFoundationExemplars::AvaIgnoreAvatarMenu()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	DISPPARAMS	dispparams;

	InitializeDISPPARAMS(&dispparams, 0);

	CComPtr<IThing> pThis;
	CComPtr<IThing> pUser;
	CComPtr<IAvatarProfile> pProfile;
	CComBSTR bstrMethod;
    HRESULT  hr;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pWorld->get_User(&pUser)))
	    goto ERROR_ENCOUNTERED;
	if (pUser==NULL)
    {
        hr = E_FAIL;
        goto ERROR_ENCOUNTERED;
    }

    if (FAILED(hr = pUser->get_ObjectPropertyExt(CComBSTR("Profile"), IID_IAvatarProfile, (IObjectProperty **)&pProfile)))
	    goto ERROR_ENCOUNTERED;
	if (pProfile==NULL)
    {
        hr = E_FAIL;
        goto ERROR_ENCOUNTERED;
    }

	// 1.  Either add or remove the friend
	if (_IsObjInProfileKeyList(pProfile, CComBSTR("Ignoring"), pThis))
		bstrMethod = "UnIgnoreAvatar";
	else
		bstrMethod = "IgnoreAvatar";

	// now, call the Ignore
	if (FAILED(hr = InitializeDISPPARAMS(&dispparams, 1)))
	    goto ERROR_ENCOUNTERED;

	dispparams.rgvarg[0].vt = VT_DISPATCH;
	dispparams.rgvarg[0].pdispVal = pThis;

	// OK if this fails
	pUser->InvokeMethodExt(bstrMethod, &dispparams, NULL);

ERROR_ENCOUNTERED:
	
    TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	return hr;
}

//******************************************************
// UpdateIgnore
// Updates the ignore menu item

STDMETHODIMP CFoundationExemplars::AvaUpdateIgnore(IMenuItem *pmi)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
	CComPtr<IThing> pUser;
	CComPtr<IAvatarProfile> pProfile;
	CComBSTR bstrMenu;
    HRESULT  hr;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	goto ERROR_ENCOUNTERED;

	if (FAILED(hr = m_pWorld->get_User(&pUser)))
	    goto ERROR_ENCOUNTERED;
	if (pUser==NULL)
    {
        hr = E_FAIL;
	    goto ERROR_ENCOUNTERED;
    }

	if (FAILED(hr = pUser->get_ObjectPropertyExt(CComBSTR("Profile"), IID_IAvatarProfile, (IObjectProperty **)&pProfile)))
    	goto ERROR_ENCOUNTERED;
	if (pProfile==NULL)
    {
        hr = E_FAIL;
	    goto ERROR_ENCOUNTERED;
    }

	// Check if user is already ignored - then you have option to un-ignore
	if (_IsObjInProfileKeyList(pProfile, CComBSTR("Ignoring"), pThis))
		bstrMenu = "Un-ignore";
	else
		bstrMenu = "Ignore";

	// Add to menu item

	// REVIEW: Don't like this way of doing things.  Better to do it
	// with two menu items [MV]
	if (FAILED(hr = pmi->put_MenuText(bstrMenu)))
    	goto ERROR_ENCOUNTERED;

    return S_OK;

ERROR_ENCOUNTERED:

    return hr;
}

//******************************************************
// IgnoreAvatar
// Causes the user to ignore this avatar

STDMETHODIMP CFoundationExemplars::AvaIgnoreAvatar(IThing *pToIgnore)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
	CComPtr<IThing> pUser;
	CComPtr<IAvatarProfile> pProfile;
    HRESULT hr;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pWorld->get_User(&pUser)))
	    goto ERROR_ENCOUNTERED;
	
#ifdef _DEBUG
    {
	    CComBSTR bstrThisName, bstrUserName;
	    if (pThis)
        {
		    if (FAILED(hr = pThis->get_Name(&bstrThisName.m_str)))
	            goto ERROR_ENCOUNTERED;
        }
	    if (pUser)
        {
		    if (FAILED(hr = pUser->get_Name(&bstrUserName.m_str)))
	            goto ERROR_ENCOUNTERED;
        }
    }
#endif

	// Only do this if pThis is pUser, don't want someone else to be able to make you ignore another avatar
	// Don't want to ignore yourself
	if ((pUser == pThis) && (pUser != pToIgnore))
	{
		if (FAILED(hr = pThis->get_ObjectPropertyExt(CComBSTR("Profile"), IID_IAvatarProfile, (IObjectProperty **)&pProfile)))
	        goto ERROR_ENCOUNTERED;
		if (pProfile==NULL)
        {
            hr = E_FAIL;
            goto ERROR_ENCOUNTERED;
        }

		// Make sure user isn't already ignored
		if (_IsObjInProfileKeyList(pProfile, CComBSTR("Ignoring"), pToIgnore))
		{
			CComBSTR	bstrName	= "<no name>";
			CString		strPrompt;

			pToIgnore->get_Name(&bstrName.m_str);
			strPrompt.Format("You are already ignoring %s!", CString(bstrName));

			_AlertUser(pThis, "Ignore", strPrompt);
		}
		else
		{
			//  1. add them to the ignore list
			_AddToProfileKeyList(m_pWorld, pProfile, CComBSTR("Ignoring"), pToIgnore, pThis);

			// 2. do the world stuff
			_FinalizeAvatarIgnore(m_pWorld, pThis, pToIgnore);
		}
	}
	else
		return ReportFoundationError(VWFOUND_E_AVATARIGNOREDENIED);


    return S_OK;

ERROR_ENCOUNTERED:

	return hr;
}

//******************************************************
// UnIgnoreAvatar
// Causes the user to un-ignore this avatar

STDMETHODIMP CFoundationExemplars::AvaUnIgnoreAvatar(IThing *pToIgnore)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
	CComPtr<IThing> pUser;
	CComPtr<IAvatarProfile> pProfile;
    HRESULT hr;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pWorld->get_User(&pUser)))
	    goto ERROR_ENCOUNTERED;

	// Only do this if pThis is pUser, don't want someone else to be able to make you un-ignore another avatar
	if ((pUser == pThis) && (pUser != pToIgnore))
	{
		if (FAILED(hr = pThis->get_ObjectPropertyExt(CComBSTR("Profile"), IID_IAvatarProfile, (IObjectProperty **)&pProfile)))
	        goto ERROR_ENCOUNTERED;
		if (pProfile==NULL)
        {
            hr = E_FAIL;
	        goto ERROR_ENCOUNTERED;
        }

		// Make sure user is already a friend
		if (!_IsObjInProfileKeyList(pProfile, CComBSTR("Ignoring"), pToIgnore))
		{
			CComBSTR	bstrName	= "<no name>";
			CString		strPrompt;

			pToIgnore->get_Name(&bstrName.m_str);
			strPrompt.Format("You are not ignoring %s!", CString(bstrName));

			_AlertUser(pThis, "Ignore", strPrompt);
		}
		else
		{
			//  1. remove them from the ignore list
			_RemoveFromProfileKeyList(pProfile, CComBSTR("Ignoring"), pToIgnore);

			// 2. do the world stuff
			_FinalizeAvatarUnIgnore(m_pWorld, pThis, pToIgnore);
		}
	}
	else
		return ReportFoundationError(VWFOUND_E_AVATARIGNOREDENIED);

    return S_OK;

ERROR_ENCOUNTERED:

	return hr;
}

//******************************************************
// BefriendAvatarMenu
// Makes this object a friend of the user....

STDMETHODIMP CFoundationExemplars::AvaBefriendAvatarMenu()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
	CComPtr<IThing> pUser;
	CComPtr<IAvatarProfile> pProfile;
	CComBSTR bstrMethod;
    HRESULT  hr;
	static CComBSTR	kbstrFriends("Friends");

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pWorld->get_User(&pUser)))
	    goto ERROR_ENCOUNTERED;
	
#ifdef _DEBUG
    {
	    CComBSTR bstrThisName, bstrUserName;
	    if (pThis)
        {
		    if (FAILED(hr = pThis->get_Name(&bstrThisName.m_str)))
	            goto ERROR_ENCOUNTERED;
        }

	    if (pUser)
        {
		    if (FAILED(hr = pUser->get_Name(&bstrUserName.m_str)))
	            goto ERROR_ENCOUNTERED;
        }
    }
#endif

	if (pUser==NULL)
    {
        hr = E_FAIL;
        goto ERROR_ENCOUNTERED;
    }

    if (FAILED(hr = pUser->get_ObjectPropertyExt(CComBSTR("Profile"), IID_IAvatarProfile, (IObjectProperty **)&pProfile)))
	    goto ERROR_ENCOUNTERED;
	if (pProfile==NULL)
    {
        hr = E_FAIL;
        goto ERROR_ENCOUNTERED;
    }

	// 1.  Either add or remove the friend
	if (_IsObjInProfileKeyList(pProfile, kbstrFriends, pThis))
		bstrMethod = "UnBefriendAvatar";
	else
		bstrMethod = "BefriendAvatar";

	// now, call the befriend, OK if this fails
	pUser->InvokeMethodExt(bstrMethod,CComDISPPARAMS(1, CComVariant(pThis)), NULL);

ERROR_ENCOUNTERED:
	return hr;
}

//******************************************************
// UpdateFriend
// Updates the Friend menu item

STDMETHODIMP CFoundationExemplars::AvaUpdateFriend(IMenuItem *pmi)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    HRESULT         hr;
	CComPtr<IThing> pThis;
	CComPtr<IThing> pUser;
	CComPtr<IAvatarProfile> pProfile;
	CComBSTR bstrMenu;
	static CComBSTR	kbstrProfile("Profile");
	static CComBSTR	kbstrFriends("Friends");

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    goto ERROR_ENCOUNTERED;
	
    if (FAILED(hr = m_pWorld->get_User(&pUser)))
	    goto ERROR_ENCOUNTERED;
	if (pUser==NULL)
    {
        hr = E_FAIL;
        goto ERROR_ENCOUNTERED;
    }

	if (FAILED(hr = pUser->get_ObjectPropertyExt(kbstrProfile, IID_IAvatarProfile, (IObjectProperty **)&pProfile)))
    	goto ERROR_ENCOUNTERED;
	if (pProfile==NULL)
    {
        hr = E_FAIL;
        goto ERROR_ENCOUNTERED;
    }

	// Check if user is a friend - if so, menu gives option to remove from friends list
	if (_IsObjInProfileKeyList(pProfile, kbstrFriends, pThis))
		bstrMenu="Remove from Friends list";
	else
		bstrMenu="Add to Friends list";

	// Add to menu item

	// REVIEW: Don't like this way of doing things.  Better to do it
	// with two menu items [MV]
	if (FAILED(hr = pmi->put_MenuText(bstrMenu)))
    	goto ERROR_ENCOUNTERED;

    return S_OK;

ERROR_ENCOUNTERED:
    return hr;
}

//******************************************************
// UnBefriendAvatar - server-side method
// Removes the specified person from your friends list

STDMETHODIMP CFoundationExemplars::AvaUnBefriendAvatar(IThing *pFriend)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
	CComPtr<IAvatarProfile> pProfile;
    HRESULT hr;
	static CComBSTR	kbstrProfile("Profile");
	static CComBSTR	kbstrFriends("Friends");

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThis->get_ObjectPropertyExt(kbstrProfile, IID_IAvatarProfile, (IObjectProperty **)&pProfile)))
	    goto ERROR_ENCOUNTERED;
	if (pProfile==NULL)
    {
        hr = E_FAIL;
        goto ERROR_ENCOUNTERED;
    }

	// Make sure user is already a friend
	if (!_IsObjInProfileKeyList(pProfile, kbstrFriends, pFriend))
	{
		if (FAILED(hr = m_pWorld->FireUIEvent(pThis, CComBSTR(VW_NOTAFRIEND_EVENT_STR), CComVariant(pFriend))))
		{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CAvatarExemplarObject::LookAt   FireUIEvent failed\n");
#endif
		goto ERROR_ENCOUNTERED;
		}

	}
	else
	{
		CComPtr<IThing>	pGlobal;

		//  remove them from the friends  list
		_RemoveFromProfileKeyList(pProfile, kbstrFriends, pFriend);
		// finally, fire the UI event

		if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
        	goto ERROR_ENCOUNTERED;

		hr = pGlobal->InvokeMethodExt(CComBSTR("FireRemoteUIEvent"),
										CComDISPPARAMS(4,CComVariant(pThis),
										CComVariant(pThis),
										CComVariant(VW_UNBEFRIEND_EVENT_STR),
										CComVariant(pFriend)), NULL);
	}

ERROR_ENCOUNTERED:
	return hr;
}

//******************************************************
// BefriendAvatar - server-side method
// Befriends the specified avatar...  This basically
// does the following:
// 1)  make sure the user is not currently your friend
// 2)  adds that avatar to a list a friends pending
// 3)  sends that user a ConfirmFriend request

STDMETHODIMP CFoundationExemplars::AvaBefriendAvatar(IThing *pFriend)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
	CComPtr<IAvatarProfile> pProfile;
    HRESULT hr;
	static CComBSTR	kbstrProfile("Profile");
	static CComBSTR	kbstrFriends("Friends");

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	goto ERROR_ENCOUNTERED;

	if (pThis == pFriend)
	{
		m_pWorld->Trace(CComBSTR("Cannot make yourself a friend."));
		goto ERROR_ENCOUNTERED;  //exit out, not an error

	}
	
	if (FAILED(hr = pThis->get_ObjectPropertyExt(kbstrProfile, IID_IAvatarProfile, (IObjectProperty **)&pProfile)))
    	goto ERROR_ENCOUNTERED;
	if (pProfile==NULL)
    {
        hr = E_FAIL;
        goto ERROR_ENCOUNTERED;
    }

	// 1.  Make sure user is not already a friend
	if (_IsObjInProfileKeyList(pProfile, kbstrFriends, pFriend))
	{
		m_pWorld->Trace(CComBSTR("This avatar is already your friend."));
		goto ERROR_ENCOUNTERED;  //exit out, not an error

	}
	else
	{
		// 2. Make sure there is no pending friendship...
		if (_IsObjInProfileKeyList(pProfile, CComBSTR("FriendsPending"), pFriend))
		{
			CComPtr<IThing>	pGlobal;

			if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
            	goto ERROR_ENCOUNTERED;

			hr = pGlobal->InvokeMethodExt(CComBSTR("FireRemoteUIEvent"),
												CComDISPPARAMS(4, CComVariant(pThis),
												CComVariant(pThis),
												CComVariant(VW_PENDINGFRIENDSHIP_EVENT_STR),
												CComVariant(pFriend)),
												NULL);

			if (FAILED(hr))
			{
#ifdef _DEBUG
				VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CAvatarExemplarObject::AvaBefriendAvatar   FireRemoteUIEvent failed\n");
#endif
				goto ERROR_ENCOUNTERED;
			}
		}
		else
		{
			//  3. add them to the friends pending list
			_AddToProfileKeyList(m_pWorld, pProfile, CComBSTR("FriendsPending"), pFriend);

			// 4.  get their confirmation, OK if this fails
			pFriend->InvokeMethodExt(CComBSTR("ConfirmFriend"),	CComDISPPARAMS(1, CComVariant(pThis)), NULL);
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

//******************************************************
// ConfirmFriend
// Confirm the friendship - if the user wants to be
// pOtherGuy's friend, this will confirm it...

STDMETHODIMP CFoundationExemplars::AvaConfirmFriend(IThing *pOtherGuy)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
	CComPtr<IThing> pUser;
    HRESULT         hr;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pWorld->get_User(&pUser)))
	    goto ERROR_ENCOUNTERED;
	if (pUser==NULL)
    {
        hr = E_FAIL;
        goto ERROR_ENCOUNTERED;
    }

	// Only query if we're in the right client
	if (pUser == pThis)
	{
		CComPtr<IPropertyList> pPropList;

		if (FAILED(hr = CreatePropertyList(m_pWorld, &pPropList)))
			goto ERROR_ENCOUNTERED;

		if (FAILED(pPropList->AddDispatch(pThis)))
			goto ERROR_ENCOUNTERED;

		if (FAILED(pPropList->AddDispatch(pOtherGuy)))
			goto ERROR_ENCOUNTERED;

		if (FAILED(hr = m_pWorld->FireUIEvent(pThis, CComBSTR(VW_ALLOWFRIENDSHIP_EVENT_STR), CComVariant(pPropList))))
		{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CAvatarExemplarObject::AvaBefriendAvatar   FireUIEvent failed\n");
#endif
		goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

//******************************************************
// FinalizeFriend
// Back on the server, actually remove this guy from 
// the waiting list and (maybe) add it to the added list...

STDMETHODIMP CFoundationExemplars::AvaFinalizeFriend(IThing *pNewFriend, VARIANT_BOOL bResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
	CComPtr<IAvatarProfile> pProfile;
    HRESULT hr;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThis->get_ObjectPropertyExt(CComBSTR("Profile"), IID_IAvatarProfile, (IObjectProperty **)&pProfile)))
	    goto ERROR_ENCOUNTERED;
	if (pProfile==NULL)
    {
        hr = E_FAIL;
        goto ERROR_ENCOUNTERED;
    }

	// 1. Make sure there is a pending friendship...
	if (_IsObjInProfileKeyList(pProfile, CComBSTR("FriendsPending"), pNewFriend))
	{
		//  2. remove them from the friends pending list
		_RemoveFromProfileKeyList(pProfile, CComBSTR("FriendsPending"), pNewFriend);

		// 3.  maybe add them to the friends list
		if (bResult == VARIANT_TRUE)
		{
			// you have a new friend!
			_AddToProfileKeyList(m_pWorld, pProfile, CComBSTR("Friends"), pNewFriend);

            // Fire the UI event to display an appropriate dialog
			CComPtr<IThing>	pGlobal;

			if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
            	goto ERROR_ENCOUNTERED;
			hr = pGlobal->InvokeMethodExt(CComBSTR("FireRemoteUIEvent"),
											CComDISPPARAMS(4, CComVariant(pThis),
											CComVariant(pThis),
											CComVariant(VW_BEFRIEND_EVENT_STR),
											CComVariant(pNewFriend)),
											NULL);
		}
		else
		{
			// Friendship request is turned down
			CComPtr<IThing>	pGlobal;

			if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
            	goto ERROR_ENCOUNTERED;
			hr = pGlobal->InvokeMethodExt(CComBSTR("FireRemoteUIEvent"),
											CComDISPPARAMS(4, CComVariant(pThis),
											CComVariant(pThis),
											CComVariant(VW_DECLINEFRIENDSHIP_EVENT_STR),
											CComVariant(pNewFriend)),
											NULL);

		}
	}

ERROR_ENCOUNTERED:
    return hr;
}

//******************************************************
// _AlertUser
// A litter wrapper for calls to Alert

void	_AlertUser(IThing *pUser, CString title, CString strPrompt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	DISPPARAMS	dispparams;

	InitializeDISPPARAMS(&dispparams, 0);

	CComBSTR	bstrTitle	= title;
	CComBSTR	bstrPrompt	= strPrompt;

	if (SUCCEEDED(InitializeDISPPARAMS(&dispparams, 2)))
    {
	    dispparams.rgvarg[1].vt = VT_BSTR;
	    dispparams.rgvarg[1].bstrVal = bstrTitle;

	    dispparams.rgvarg[0].vt = VT_BSTR;
	    dispparams.rgvarg[0].bstrVal = bstrPrompt;

	    // OK if this fails
	    pUser->InvokeMethodExt(CComBSTR("Alert"), &dispparams, NULL);
    }

    TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
}

//******************************************************
// _FinalizeAvatarIgnore
// Helper function to ignore the avatar - turfs the
// graphics and sends the message...

void	_FinalizeAvatarIgnore(IWorld *pWorld, IThing *pUser, IThing *pAvatarToTurf)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	DISPPARAMS		dispparams;
    HRESULT         hr;

	InitializeDISPPARAMS(&dispparams, 0);

	CComPtr<IThing>	pGlobal;
	CComPtr<IVWGeometrySprite> pGeom;
	CString		strMsg;
	CComBSTR	bstrMsg	= "<avatar>";

	static CComBSTR	bstrClientGeometry("ClientGeometry");
	static CComBSTR	bstrIgnoreSprite("client\\shared\\ignore.spr");
	static CComBSTR	kbstrEvent1 = VW_IGNOREDUSER_EVENT_STR;
	static CComBSTR	kbstrEvent2 = VW_IGNOREDBYUSER_EVENT_STR;

	// 1. remove avatar graphic
	if (FAILED(hr = CreateSpritedGeometry(pWorld, bstrIgnoreSprite, &pGeom)))
    	goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pAvatarToTurf->put_ObjectProperty(bstrClientGeometry, pGeom)))
	    goto ERROR_ENCOUNTERED;

	// finally, fire the UI events
	pWorld->FireUIEvent(pUser, kbstrEvent1.m_str, CComVariant((IDispatch *)pAvatarToTurf));

	if (FAILED(hr = pWorld->get_Global(&pGlobal)))
	    goto ERROR_ENCOUNTERED;

	if (FAILED(hr = InitializeDISPPARAMS(&dispparams, 4)))
    	goto ERROR_ENCOUNTERED;
	
	dispparams.rgvarg[3].vt = VT_DISPATCH;
	dispparams.rgvarg[3].pdispVal = pAvatarToTurf;

	dispparams.rgvarg[2].vt = VT_DISPATCH;
	dispparams.rgvarg[2].pdispVal = pAvatarToTurf;

	dispparams.rgvarg[1].vt = VT_BSTR;
	dispparams.rgvarg[1].bstrVal = kbstrEvent2.m_str;

	dispparams.rgvarg[0].vt = VT_DISPATCH;
	dispparams.rgvarg[0].pdispVal = pUser;

	hr = pGlobal->InvokeMethodExt(CComBSTR("FireRemoteUIEvent"), &dispparams, NULL);

ERROR_ENCOUNTERED:
	
    TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
}

//******************************************************
// _FinalizeAvatarUnIgnore
// Helper function to unignore the avatar - restores the
// graphics and sends the message...

void	_FinalizeAvatarUnIgnore(IWorld *pWorld, IThing *pUser, IThing *pAvatarToTurf)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	DISPPARAMS	dispparams;
    HRESULT     hr;

	InitializeDISPPARAMS(&dispparams, 0);

	CComPtr<IThing>	pGlobal;
	CComBSTR	bstrMsg	= "<avatar>";
	CString		strMsg;
	static      CComBSTR	bstrClientGeometry("ClientGeometry");
	static		CComBSTR	kbstrEvent1 = VW_UNIGNOREDUSER_EVENT_STR;
	static		CComBSTR	kbstrEvent2 = VW_UNIGNOREDBYUSER_EVENT_STR;

	// 1. restore avatar graphic
	// REVIEW:  Really, we should get rid of the local property
	// all-together, right..?
	if (FAILED(hr = pAvatarToTurf->put_ObjectProperty(bstrClientGeometry, NULL)))
	    goto ERROR_ENCOUNTERED;

	// finally, fire the UI event
	pWorld->FireUIEvent(pUser, kbstrEvent1.m_str, CComVariant((IDispatch *)pAvatarToTurf));

	if (FAILED(hr = pWorld->get_Global(&pGlobal)))
    	goto ERROR_ENCOUNTERED;

	if (FAILED(hr = InitializeDISPPARAMS(&dispparams, 4)))
	    goto ERROR_ENCOUNTERED;
	
	dispparams.rgvarg[3].vt = VT_DISPATCH;
	dispparams.rgvarg[3].pdispVal = pAvatarToTurf;

	dispparams.rgvarg[2].vt = VT_DISPATCH;
	dispparams.rgvarg[2].pdispVal = pAvatarToTurf;

	dispparams.rgvarg[1].vt = VT_BSTR;
	dispparams.rgvarg[1].bstrVal = kbstrEvent2.m_str;

	dispparams.rgvarg[0].vt = VT_DISPATCH;
	dispparams.rgvarg[0].pdispVal = pUser;
	
    hr = pGlobal->InvokeMethodExt(CComBSTR("FireRemoteUIEvent"), &dispparams, NULL);

ERROR_ENCOUNTERED:

    TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
}

//******************************************************
// UpdateInteractions
HRESULT CFoundationExemplars::AvaUpdateInteractions(IMenuItem *pMI)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComVariant varIgnore;
	CComPtr<IThing> pOwner, pTarget, pGlobal;
	CComPtr<IPropertyList> pMenuList, pInteractions, pTargetIgnoreList;
	CComPtr<IAvatarProfile> pTargetProfile;
	CComPtr<IMenuItem> pSubMI;
	CComBSTR bstrName;
	CComBSTR bstrGIF;
	CComBSTR bstrSoundURL;
	CComBSTR bstrDescription;
	CComBSTR bstrUpdate;
	CComDISPPARAMS dispparams;
	long lFlags, lLoc;
	HRESULT	hr = S_OK;

	if (FAILED(hr = pMI->get_Owner(&pOwner)))
		return hr;
	if (FAILED(hr = pMI->get_Target(&pTarget)))
		return hr;
	if (FAILED(hr = pMI->get_Flags(&lFlags)))
		return hr;

	// Need to figure out if target is ignoring me.
	hr = pTarget->get_ObjectProperty(CComBSTR("Profile"), (IObjectProperty **)&pTargetProfile);
	if (SUCCEEDED(hr) && pTargetProfile)
	{
		hr = pTargetProfile->get_KeyValue(CComBSTR("Ignoring"), &varIgnore);
		if (SUCCEEDED(hr) && (varIgnore.vt == VT_DISPATCH) && varIgnore.pdispVal)
		{
			pTargetIgnoreList = (IPropertyList *)varIgnore.pdispVal;
			hr = pTargetIgnoreList->Find(CComVariant(pOwner), &lLoc);
			if (SUCCEEDED(hr) && (lLoc != -1))
			{
				// The target is ignoring me, don't show the menu item.
				lFlags |= MI_INVISIBLE;
				hr = pMI->put_Flags(lFlags);
				return hr;
			}
		}
	}

	// Create the menu list
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pMenuList)))
		return hr;

	// Get Global
	if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
		return hr;

	// Get the interaction list
	hr = pGlobal->get_ObjectProperty(CComBSTR("Interactions"),
									 (IObjectProperty**) &pInteractions);
	if (FAILED(hr) && (hr != VWOBJECT_E_PROPERTYNOTEXIST))
		return hr;

	// Check that there's something to add
	if (pInteractions)
	{
		// Cycle through the list
		for (int i = 0; SUCCEEDED(pInteractions->get_String(i++, &bstrName.m_str));)
		{
			if (FAILED(pInteractions->get_String(i++, &bstrGIF.m_str)))
				break;
			if (FAILED(pInteractions->get_String(i++, &bstrSoundURL.m_str)))
				break;
			if (FAILED(pInteractions->get_String(i++, &bstrDescription.m_str)))
				break;
			if (FAILED(pInteractions->get_String(i++, &bstrUpdate.m_str)))
				break;

			CComVariant varGIF(bstrGIF);
			CComVariant varSoundURL(bstrSoundURL);
			CComVariant varDescription(bstrDescription);
			dispparams.Initialize(4, varGIF,
								  varSoundURL,
								  varDescription, 
								  CComVariant((IDispatch*)NULL));

			// Create the menu item
			CComPtr<IMenuItem> pSubMI;
			if (FAILED(hr = CreateMenuItemExt(m_pWorld, bstrName, 0, pOwner, bstrUpdate,
										  CComBSTR("InteractAt"), dispparams, &pSubMI)))
				return hr;
			if (FAILED(hr = pSubMI->put_AddTargetParameter(VARIANT_TRUE)))
				return hr;

			// Add it to the list
			if (FAILED(hr = pMenuList->AddObjectProperty(pSubMI)))
				return hr;

			// Release everything.
			pSubMI.Release();
			bstrName.Empty();
			bstrGIF.Empty();
			bstrSoundURL.Empty();
			bstrDescription.Empty();
			bstrUpdate.Empty();
			dispparams.Terminate();

			// Now add it to the menu item sub-menu
			if (FAILED(hr = pMI->put_SubMenu(pMenuList)))
				return hr;

			// We added one, show the menu item.
			if (lFlags & MI_INVISIBLE)
			{
				lFlags &= ~MI_INVISIBLE;
				hr = pMI->put_Flags(lFlags);
			}
		}
	}
	return S_OK;
}

//******************************************************
// GetMenuItems

HRESULT CFoundationExemplars::AvaGetMenuItems(IThing *pThing, IPropertyList **ppMenuList)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CComPtr<IThing>			pThis;
	CComPtr<IThing>			pUser;
	CComPtr<IAvatarProfile> pProfile;
	CComBSTR				bstrMenuProp;
	static CComBSTR			kbstrIgnoring = "Ignoring";
	CComVariant				varRetVal;
    HRESULT                 hr;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pWorld->get_User(&pUser)))
	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pUser->get_ObjectProperty(CComBSTR("Profile"), (IObjectProperty **)&pProfile)))
	    goto ERROR_ENCOUNTERED;
	
	if (_IsObjInProfileKeyList(pProfile, kbstrIgnoring, pThis))
		bstrMenuProp = "IgnoreMenu";
	else 
	{	
		VARIANT_BOOL	bAvatar;

		m_pWorld->IsAvatar(pThis, &bAvatar);
		if (bAvatar == VARIANT_TRUE)
		{
			CComPtr<IAvatarProfile> pOtherProfile;

			if (FAILED(hr = pThis->get_ObjectProperty(CComBSTR("Profile"), (IObjectProperty **)&pOtherProfile)))
	            goto ERROR_ENCOUNTERED;

			if (_IsObjInProfileKeyList(pOtherProfile, kbstrIgnoring, pUser))
				bstrMenuProp = "IgnoredByMenu";
			else
				bstrMenuProp = "Menu";
		}
		else
			bstrMenuProp = "Menu";
	}

	if (FAILED(hr = pThis->InvokeMethodExt(CComBSTR("GetMenuItemsFromProperty"),
								CComDISPPARAMS(1, CComVariant(bstrMenuProp)),
								&varRetVal)))
        goto ERROR_ENCOUNTERED;

	if (varRetVal.vt == VT_DISPATCH)
	{
		*ppMenuList = (IPropertyList *)varRetVal.pdispVal;
		SAFEADDREF(*ppMenuList);
	}

    return S_OK;

ERROR_ENCOUNTERED:

	return hr;
}

HRESULT CFoundationExemplars::AvaGetPeerMenuItems(IThing *pPeer, IPropertyList **ppMenuList)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing>			pUser;
	CComPtr<IAvatarProfile> pProfile;
	CComBSTR				bstrMenuProp;
	static CComBSTR			kbstrIgnoring = "Ignoring";
	CComVariant				varRetVal;
    HRESULT                 hr;

	if (FAILED(hr = m_pWorld->get_User(&pUser)))
	    goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pUser->get_ObjectProperty(CComBSTR("Profile"), (IObjectProperty **)&pProfile)))
	    goto ERROR_ENCOUNTERED;
	
	if (_IsObjInProfileKeyList(pProfile, kbstrIgnoring, pPeer))
		bstrMenuProp = "IgnorePeerMenu";
	else
		bstrMenuProp = "PeerMenu";

	if (FAILED(hr = pUser->InvokeMethodExt(CComBSTR("GetMenuItemsFromProperty"),
								CComDISPPARAMS(1, CComVariant(bstrMenuProp)),
								&varRetVal)))
        goto ERROR_ENCOUNTERED;

	if (varRetVal.vt == VT_DISPATCH)
	{
		*ppMenuList = (IPropertyList *)varRetVal.pdispVal;
		SAFEADDREF(*ppMenuList);
	}

    return S_OK;

ERROR_ENCOUNTERED:

	return hr;
}

STDMETHODIMP CFoundationExemplars::AvaOnPreComposeGeometry()
{
	CComPtr<IThing>				pThis, pUser;
	CComPtr<IVWGeometrySprite>	pGeom;
	CComVariant					varIgnoring;
	static CComBSTR	bstrClientGeometry("ClientGeometry");
	static CComBSTR	bstrIgnoreSprite("client\\shared\\ignore.spr");
	HRESULT hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	if (FAILED(hr = m_pWorld->get_User(&pUser)))
		return hr;

	if (pThis != pUser)
	{
		if (FAILED(hr = pUser->InvokeMethodExt(CComBSTR("IsInUserProfileList"),
											   CComDISPPARAMS(2, CComVariant((IDispatch*)pThis),
															  CComVariant(CComBSTR("Ignoring"))),
											   &varIgnoring)))
			return hr;

		if (varIgnoring.vt != VT_BOOL || varIgnoring.boolVal == VARIANT_FALSE)
			return hr;

		if (FAILED(hr = CreateSpritedGeometry(m_pWorld, bstrIgnoreSprite, &pGeom)))
			return hr;
		if (FAILED(hr = pThis->put_ObjectProperty(bstrClientGeometry, pGeom)))
			return hr;

	}
	return hr;
}

