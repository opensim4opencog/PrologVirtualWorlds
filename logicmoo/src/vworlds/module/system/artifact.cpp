// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// Artifact.cpp : Implementation of CArtifact
#include "stdafx.h"
#include <vwsysex.h>
#include "vwutils.h"
#include "artifact.h"
#include "syshelp.h"

/////////////////////////////////////////////////////////////////////////////
// CArtifact

STDMETHODIMP CArtifactExemplarObject::Install(IModule* pModule)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CMethodInfo rgMethod[] =
	{
		{ METHOD_CLIENT, "WieldItem",		NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN, PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_CLIENT, "UpdateWield",		NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN, PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_CLIENT, "UpdateUnWield",	NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN, PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_CLIENT, "UnWieldItem",		NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN, PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER, "AcceptEnter",		NULL, PSBIT_EXEMPLARDEFAULTMETHOD,				  PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER, "AcceptLeave",		NULL, PSBIT_EXEMPLARDEFAULTMETHOD,				  PS_ALLEXECUTEMETHOD }
	};
	int cMethod = sizeof(rgMethod)/sizeof(rgMethod[0]);

	CComPtr<IThing> pArtEx;
	CComBSTR bstrName;
	CComBSTR bstrNameInternal;
    HRESULT  hr = S_OK;
    int iMethod;

	// create and initialize the exemplar
	hr = m_pWorld->CreateExemplar(CComBSTR("Artifact"), CComVariant(CComBSTR("Thing")), &pArtEx);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// add in the methods
	for (iMethod=0; iMethod<cMethod; iMethod++)
	{
		bstrName = rgMethod[iMethod].pszName;

		if (rgMethod[iMethod].pszNameInternal == NULL)
			bstrNameInternal = bstrName;
		else
			bstrNameInternal = rgMethod[iMethod].pszNameInternal;
		
		hr = pArtEx->CreateAndAddMethodExt(	rgMethod[iMethod].lFlags,
													pModule,
													bstrNameInternal,
													bstrName,
													rgMethod[iMethod].psbits,
													rgMethod[iMethod].permissions);
	    if (FAILED(hr))
		    goto ERROR_ENCOUNTERED;
	}

	// add in the properties
	hr = pArtEx->AddPropertyExt(	CComBSTR("IsWieldable"), 
										CComVariant((bool)VARIANT_FALSE), 
										PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN, 
										PS_EXEMPLARDEFAULTPROPERTY, 
										VT_BOOL, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

    hr = pArtEx->AddPropertyExt(	CComBSTR("IsWielded"), 
										CComVariant((bool)VARIANT_FALSE), 
										PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN, 
										PS_EXEMPLARDEFAULTPROPERTY, 
										VT_BOOL, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// do the menus now
    {
	CComPtr<IPropertyList> pMenu, pMenuThingEx;
	CComPtr<IMenuItem> pMenuItem;

	// create a blank menu
	hr = CreateMenu(m_pWorld, &pMenu);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// get the current (thing exemplar's menu)
	hr = pArtEx->get_ObjectProperty(CComBSTR("Menu"), (IObjectProperty **) &pMenuThingEx);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// copy into the new, blank one
	hr = pMenuThingEx->CopyTo(pMenu);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// add artifact menus below that
	hr = CreateMenuItemExt(	m_pWorld, 
									CComBSTR("Hold"),
									MI_INVISIBLE,
									pArtEx,
									CComBSTR("UpdateWield"),
									CComBSTR("WieldItem"),
									NULL,
								&pMenuItem );
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

//		hr = pMenuItem->put_SortOrder(MI_SORT_GENERIC);
//	    if (FAILED(hr))
//		    goto ERROR_ENCOUNTERED;

	hr = pMenu->AddObjectProperty(pMenuItem);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	pMenuItem.Release();

	hr = CreateMenuItemExt(	m_pWorld, 
									CComBSTR("Put Away"),
									MI_INVISIBLE,
									pArtEx,
									CComBSTR("UpdateUnWield"),
									CComBSTR("UnWieldItem"),
									NULL,
								&pMenuItem );
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

//		hr = pMenuItem->put_SortOrder(MI_SORT_GENERIC);
//	    if (FAILED(hr))
//		    goto ERROR_ENCOUNTERED;

	hr = pMenu->AddObjectProperty(pMenuItem);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// stow menu back
	hr = pArtEx->put_ObjectProperty(CComBSTR("Menu"), (IObjectProperty *)pMenu);
    }

ERROR_ENCOUNTERED:
	if (hr != S_OK)
		VWTRACE(m_pWorld, "VWARTIFACT", VWT_ERROR, "CArtifactExemplarObject::Install   Error encountered %x\n", hr);

	return hr;
}


//******************************************************
// AcceptLeave
// Review: should we check IsTakeable?  need to get around
// that if you're a wizard, portal, etc. [RK]

STDMETHODIMP CArtifactExemplarObject::AcceptLeave(VARIANT_BOOL * pbool)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pbool==NULL)
    {
		VWTRACE(m_pWorld, "VWARTIFACT", VWT_ERROR, "CArtifactExemplarObject::AcceptLeave: NULL output pointer\n");
		return ReportArtifactError(E_POINTER);
    }

	// default
	*pbool = VARIANT_TRUE;
	return S_OK;

}

//******************************************************
// AcceptEnter
// Used to be:An artifact will enter as long as it is going one step
// in the containment hierarchy (ie, it will enter it's container's
// container, and it will enter the contents of a peer).
// If this fails, it calls the inherited method.

STDMETHODIMP CArtifactExemplarObject::AcceptEnter(IThing * pThing, VARIANT_BOOL * pbool)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pThing==NULL || pbool==NULL)
    {
		VWTRACE(m_pWorld, "VWARTIFACT", VWT_ERROR, "CArtifactExemplarObject::AcceptEnter: NULL output pointer\n");
		return ReportArtifactError(E_POINTER);
    }

	*pbool = VARIANT_TRUE;
	return S_OK;
}


//******************************************************
// WieldItem
// Wields the item - called from the menu

STDMETHODIMP CArtifactExemplarObject::WieldItem()
{
	return S_OK;
}

//******************************************************
// UpdateWield
// Updates the wield menu

STDMETHODIMP CArtifactExemplarObject::UpdateWield(IMenuItem *pmi)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pmi==NULL)
    {
		VWTRACE(m_pWorld, "VWARTIFACT", VWT_ERROR, "CArtifactExemplarObject::UpdateWield: NULL pointer\n");
		return ReportArtifactError(E_POINTER);
    }

	CComPtr<IThing> pThing;
	CComPtr<IThing> pContainer;
	CComPtr<IThing> pUser;
	CComPtr<IThing> pWielded;
	VARIANT_BOOL bIsWieldable;
	long lFlags;
    HRESULT hr = S_OK;

	hr = pmi->get_Owner(&pThing);
	if (FAILED(hr))
		return hr;

	hr = pThing->get_Container(&pContainer);
   	if (FAILED(hr))
		return hr;

	hr = m_pWorld->get_User(&pUser);
	if (FAILED(hr))
		return hr;

	hr = pThing->get_BOOL(CComBSTR("IsWieldable"), &bIsWieldable);
	if (FAILED(hr))
		return hr;

	hr = pmi->get_Flags(&lFlags);
	if (FAILED(hr))
		return hr;

	hr = pUser->get_Thing(CComBSTR("WieldedItem"), &pWielded);
	if (FAILED(hr))
		return hr;

	// check to see if we ought to continue
	if (bIsWieldable==VARIANT_TRUE	&&
		pWielded!=pThing			&&
		pContainer==pUser )
	{
		// OK we passed!!!, enable wielding

		// Menu item is currently owned by artifact, actually want it to be owned by connected
		// user and to be called with the artifact as a parameter.

		// REVIEW: [MV] Hacking the menu is not a good approach.
		hr = pmi->put_Owner(pUser);
	    if (FAILED(hr))
		    return hr;

		hr = pmi->put_ArgumentsExt(CComDISPPARAMS(1, CComVariant((IDispatch*)pThing)));
    	if (FAILED(hr))
	    	return hr;

		lFlags &= ~MI_INVISIBLE;
	}
	else
		lFlags |= MI_INVISIBLE;

	hr = pmi->put_Flags(lFlags);

	return hr;
}


//******************************************************
// UnWieldItem
// UnWields the item - called from the menu

STDMETHODIMP CArtifactExemplarObject::UnWieldItem()
{
	return S_OK;
}

//******************************************************
// UpdateUnWield
// Updates the Unwield menu

STDMETHODIMP CArtifactExemplarObject::UpdateUnWield(IMenuItem *pmi)
{
	// UnWield should only be enabled if the artifact is already wielded

	CComPtr<IThing> pThing;
	CComPtr<IThing> pLocation;
	CComPtr<IThing> pUser;
	CComPtr<IThing> pWielded;
	long lFlags = 0;
    HRESULT hr = S_OK;

	if (pmi==NULL)
    {
		VWTRACE(m_pWorld, "VWARTIFACT", VWT_ERROR, "CArtifactExemplarObject::UpdateUnWield: NULL pointer\n");
		return ReportArtifactError(E_POINTER);
    }

	hr = pmi->get_Owner(&pThing);
	if (FAILED(hr))
		return hr;

	hr = pThing->get_Container(&pLocation);
	if (FAILED(hr))
		return hr;

	hr = m_pWorld->get_User(&pUser);
	if (FAILED(hr))
		return hr;

	hr = pmi->get_Flags(&lFlags);
	if (FAILED(hr))
		return hr;

	hr = pUser->get_Thing(CComBSTR("WieldedItem"), &pWielded);
	if (FAILED(hr))
		return hr;

	// check to see if we ought to continue

	// are we already wielded?
	if ( pWielded == pThing )
	{
		// OK we passed!!!, enable UnWielding

		// Menu item is currently owned by artifact, actually want it to be owned by connected
		// user and to be called with the artifact as a parameter.

		// REVIEW: I don't like this backpatching!! [MV]

		hr = pmi->put_Owner(pUser);
    	if (FAILED(hr))
	    	return hr;

		lFlags &= ~MI_INVISIBLE;
	}
	else
		lFlags |= MI_INVISIBLE;

	hr = pmi->put_Flags(lFlags);

    return hr;
}