// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// FHost.cpp : Adds host peer menu in foundation layer
#include "stdafx.h"
#include "syshelp.h"
#include "vwutils.h"
#include <foundatn.h>
#include <vwprop.h>

#define ALLOWSECURITYOVERRIDE

/////////////////////////////////////////////////////////
// This adds the LogOff menu item in the Foundation layer
// but no additional functionality

/////////////////////////////////////////////////////////
// Install
// Installs this module - creates the exemplar and
// adds the properties and methods.

HRESULT CFoundationExemplars::HostInstall(IModule* pModule)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr = S_OK;
	CComPtr<IThing> pHostEx;
	CComBSTR bstrName;
	CComBSTR bstrNameInternal;
	CComPtr<IPropertyList> pMenu;
	CComPtr<IPropertyList> pPeerMenu;
	CComPtr<IMenuItem> pMenuItem;

	hr = m_pWorld->get_Exemplar(CComBSTR("Host"), &pHostEx);

	// Delete the peer menu, the current peer menu, which won't have foundation additions (Interact >>)
	hr = pHostEx->RemoveProperty(CComBSTR("PeerMenu"));
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWMM", VWT_ERROR, "CFoundationExemplars::InstallHostExtensions  Unable to remove peer menu, hr = %x\n", hr);
		goto ERROR_ENCOUNTERED;
	}

	// Now re-get the menu (this will get the avatar foundation version) and re-add LogOff
	hr = pHostEx->get_ObjectProperty(CComBSTR("PeerMenu"), (IObjectProperty**) &pPeerMenu);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// make a copy (since not COPYONGET)
	hr = CreateMenu(m_pWorld, &pMenu);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pPeerMenu->CopyTo(pMenu);
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

	if (FAILED(hr = pMenu->AddObjectProperty(pMenuItem)))
		goto ERROR_ENCOUNTERED;

	hr = pHostEx->put_ObjectProperty(CComBSTR("PeerMenu"), pMenu);

ERROR_ENCOUNTERED:
	if (hr != S_OK)
		VWTRACE(m_pWorld, "VWFOUND", VWT_ERROR, "CFoundationExemplars::HostInstall   Error encountered\n");
	return hr;
}
