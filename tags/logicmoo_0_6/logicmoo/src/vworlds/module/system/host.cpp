// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// Host.cpp : Implementation of CHost
#include "stdafx.h"
#include <vwsysex.h>
#include <vwanim.h>
#include "vwutils.h"
#include "host.h"
#include "syshelp.h"
#include "profhelp.h"
#include "..\..\vwobject\propsecu.h"
#include <resource.h>
#include <vwevents.h>

#define ALLOWSECURITYOVERRIDE

/////////////////////////////////////////////////////////
// CHost
// The host exemplar has an additional method, LogOff,
// that disconnects avatars from the world
// Hosts are on a world-basis
// Only wizards can make an avatar a host

/////////////////////////////////////////////////////////
// Install
// Installs this module - creates the exemplar and
// adds the properties and methods.

STDMETHODIMP CHostExemplarObject::Install(IModule* pModule)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CMethodInfo rgMethod[] =
	{
		{ METHOD_CLIENT, "ShowLogOffDialog",		NULL,		PSBIT_EXEMPLARDEFAULTMETHOD,				PS_EXEMPLARDEFAULTMETHOD },	
		{ METHOD_SERVER, "LogOff",					NULL,		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD }	

	};
	int cMethod = sizeof(rgMethod)/sizeof(CMethodInfo);

	HRESULT hr = S_OK;
	CComPtr<IThing> pHostEx;
	CComBSTR bstrName;
	CComBSTR bstrNameInternal;
	CComPtr<IPropertyList> pMenu;
	CComPtr<IPropertyList> pAvatarMenu;
	CComPtr<IMenuItem> pMenuItem;
	int iMethod;

	hr = m_pWorld->CreateExemplar(CComBSTR("Host"), CComVariant(CComBSTR("Avatar")), &pHostEx);
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CHostExemplarObject::Install  Can't create host exemplar \n");
		goto ERROR_ENCOUNTERED;
	}

//	hr = pHostEx->put_Description(CComBSTR("A host avatar with the power to log off users."));
//	if (FAILED(hr))
//		goto ERROR_ENCOUNTERED;

	for (iMethod=0; iMethod<cMethod; iMethod++)
	{
		bstrName = rgMethod[iMethod].pszName;
		
		if (rgMethod[iMethod].pszNameInternal == NULL)
			bstrNameInternal = bstrName;
		else
			bstrNameInternal = rgMethod[iMethod].pszNameInternal;
		
		hr = pHostEx->CreateAndAddMethodExt(rgMethod[iMethod].lFlags,
													pModule,
													bstrNameInternal,
													bstrName,
													rgMethod[iMethod].psbits,
													rgMethod[iMethod].permissions);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CHostExemplarObject::Install  Can't create method %s \n", CString(bstrName));
			goto ERROR_ENCOUNTERED;
		}
	}

	// Create the peer menu items
	hr = pHostEx->get_ObjectProperty(CComBSTR("PeerMenu"), (IObjectProperty**) &pAvatarMenu);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// make a copy (since not COPYONGET)
	hr = CreateMenu(m_pWorld, &pMenu);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pAvatarMenu->CopyTo(pMenu);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// Add Log off... to peer menu list
	hr = CreateMenuItemExt(	m_pWorld, CComBSTR("Log off..."), 0, NULL,
									CComBSTR("UpdateIsAvatarAndNotWizard"), CComBSTR("ShowLogOffDialog"), 
									NULL,  &pMenuItem);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

//	pMenuItem->put_SortOrder(MI_SORT_PRIMARY+110);

	hr = pMenuItem->put_AddTargetParameter(VARIANT_TRUE);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pMenu->AddObjectProperty(pMenuItem);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pHostEx->put_ObjectProperty(CComBSTR("PeerMenu"), pMenu);

ERROR_ENCOUNTERED:
	if (hr != S_OK)
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CHostExemplarObject::Install   Error encountered\n");
	return hr;
}

////////////////////////////////////////////////////////
// ShowLogOffDialog
// Pops up the log off dialog, which asks the host for 
// the reason why a user is disconnecting

STDMETHODIMP CHostExemplarObject::ShowLogOffDialog(IThing* pTarget)
{
	HRESULT hr = S_OK;
	CComPtr<IThing> pCaller;
	VARIANT_BOOL bIsHost	= VARIANT_FALSE;
	VARIANT_BOOL bIsWizard	= VARIANT_FALSE;

    if (pTarget == NULL)
    {
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CHostExemplarObject::ShowLogOffDialog  NULL pointer \n");
        return ReportHostError(E_POINTER);
    }

	hr = m_pWorld->get_Caller(&pCaller);
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CHostExemplarObject::ShowLogOffDialog  Error getting caller \n");
		return hr;
	}

	if (pCaller)
	{

#ifdef _DEBUG
		CComBSTR bstrName("");
		pCaller->get_Name(&bstrName.m_str);
#endif

		hr = pCaller->IsOfType(CComBSTR("Host"), &bIsHost);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CHostExemplarObject::ShowLogOffDialog  Error getting caller host property \n");
			return hr;
		}

		if (bIsHost==VARIANT_FALSE)
		{
			VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CHostExemplarObject::ShowLogOffDialog  Caller isn't a host, can't log off \n");
			return ReportHostError(VWHOST_E_NOTAHOST);
		}
	}

	hr = pTarget->get_IsWizard(&bIsWizard);
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CHostExemplarObject::ShowLogOffDialog  Error getting Target.IsWizard");
		return hr;
	}

	if (bIsWizard)
	{
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CHostExemplarObject::ShowLogOffDialog  Target is wizard, can't log a wizard off.");
		return ReportHostError(VWHOST_E_CANTLOGOFFWIZARD);
	}

	// Pop up dialog so host can enter a reason for logging off the target avatar

	hr = m_pWorld->FireUIEvent(pCaller, CComBSTR(VW_SHOWLOGOFF_EVENT_STR), CComVariant(pTarget));
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CHostExemplarObject::ShowLogOffDialog   FireUIEvent failed\n");
		return hr;
	}

	return hr;
}

////////////////////////////////////////////////////////
// LogOff
// Does checks for validity, then calls World.DisconnectAvatar
// to log the target off, and fires UI event "OnLogoff"
// Caller must be a host and target cannot be a wizard

STDMETHODIMP CHostExemplarObject::LogOff(IThing* pTarget, BSTR bstrReason)
{
	HRESULT hr				= S_OK;
	CComPtr<IThing>			pCaller;
	CComPtr<IThing>			pContainer;
	CComPtr<IThing>			pGlobal;
	CComPtr<IPropertyList>	pListOfAvatarsInRoom;
	CComPtr<IPropertyList>	pArgList;
	VARIANT_BOOL bIsHost	= VARIANT_FALSE;
	VARIANT_BOOL bIsWizard	= VARIANT_FALSE;

    if (pTarget == NULL)
    {
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CHostExemplarObject::LogOff  NULL pointer \n");
        return ReportHostError(E_POINTER);
    }

	hr = m_pWorld->get_Caller(&pCaller);
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CHostExemplarObject::LogOff  Error getting caller \n");
		return hr;
	}

	if (pCaller)
	{

#ifdef _DEBUG
		CComBSTR bstrName("");
		pCaller->get_Name(&bstrName.m_str);
#endif

		hr = pCaller->IsOfType(CComBSTR("Host"), &bIsHost);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CHostExemplarObject::LogOff  Error getting caller host property \n");
			return hr;
		}

		if (bIsHost==VARIANT_FALSE)
		{
			VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CHostExemplarObject::LogOff  Caller isn't a host, can't log off \n");
			return ReportHostError(VWHOST_E_NOTAHOST);
		}

		hr = pTarget->get_IsWizard(&bIsWizard);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CHostExemplarObject::LogOff  Error getting Target.IsWizard");
			return hr;
		}

		if (bIsWizard)
		{
			VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CHostExemplarObject::LogOff  Target is wizard, can't log a wizard off.");
			return hr;
		}

		// Now get all the information needed to fire a remote UI event to all avatar in the room
		// First, get list of avatars to send to

		hr = pCaller->get_Container(&pContainer);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CHostExemplarObject::LogOff  Error getting container");
			return hr;
		}

		if (pContainer)
		{
			hr = pContainer->get_ObjectProperty(CComBSTR("Avatars"), (IObjectProperty **) &pListOfAvatarsInRoom);
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CHostExemplarObject::LogOff Can't get avatar list\n");
				return hr;
			}
		}


		// Now, package up arguments that client will need to display messages properly
		hr = CreatePropertyList(m_pWorld, &pArgList);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CHostExemplarObject::LogOff  Error creating property list");
			return hr;
		}
		
		if (pArgList)
		{
			hr = pArgList->AddThing(pCaller);

			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CHostExemplarObject::LogOff  Error adding to property list");
				return hr;
			}

			hr = pArgList->AddThing(pTarget);

			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CHostExemplarObject::LogOff Error adding to property list");
				return hr;
			}
			hr = pArgList->AddString(bstrReason);
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CHostExemplarObject::LogOff Error adding to property list");
				return hr;
			}

				// Fire UI event to display message in history pane that you have entered world
			static CComBSTR kbstrEvent(VW_LOGOFF_EVENT_STR);

			hr = m_pWorld->get_Global(&pGlobal);

			if (pGlobal)
			{
				hr = pGlobal->InvokeMethodExt(CComBSTR("FireRemoteUIEvent"),  
										  CComDISPPARAMS(4, CComVariant((IDispatch *) pListOfAvatarsInRoom), 
										  CComVariant((IDispatch *) pCaller), 
										  CComVariant(kbstrEvent), 
										  CComVariant((IDispatch*) pArgList)), NULL);
			}
		}

		// Set security context to be that of the avatar that is being disconnected
		{
			CSetUID SetUID(m_pWorld, pTarget, pTarget, VARIANT_TRUE);

			hr = m_pWorld->DisconnectAvatar(pTarget);
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CHostExemplarObject::LogOff  Error calling DisconnectAvatar\n");
				return hr;
			}
		}
	}
		
	return hr;
}
