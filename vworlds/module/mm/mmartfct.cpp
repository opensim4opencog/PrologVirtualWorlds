// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// mmartfct.cpp : Artifact extensions

#include "stdafx.h"
#include <d3drmwin.h>
#include <d2d.h>
#include <syshelp.h>
#include <vwmmex.h>
#include <vwidata.h>
#include <vwanim.h>
#include "vwutils.h"
#include "vwmmexo.h"
#include <vwevents.h>

///////////////////////////////////////////////////////////////////////
// Parameters


///////////////////////////////////////////////////////////////////////
// Helpers

///////////////////////////////////////////////////////////////////////
// Install 

HRESULT CMultimediaExemplarObject::InstallArtifactExtensions(IModule* pModule)
{
	CMethodInfo rgMethod[] =
	{
		{ METHOD_CLIENT,	"UpdateWear",						NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"UpdateUnWear",						NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"OnIsAccessoryChanged",				NULL, PSBIT_EVENTHANDLER,					PS_ALLEXECUTEMETHOD }
	};
	int cMethod = sizeof(rgMethod)/sizeof(rgMethod[0]);
    int iMethod;

	// find the Artifact exemplar
	CComPtr<IThing> pArtifactEx;
	CComPtr<IPropertyList> pMenu;
	CComPtr<IPropertyList> pList;
	CComPtr<IMenuItem> pMenuItem;
	CComBSTR bstrName;
	CComBSTR bstrNameInternal;
	CComBSTR bstrTexture;
	CComBSTR bstrModel;
    HRESULT  hr = S_OK;

	if (FAILED(hr = m_pWorld->get_Exemplar(CComBSTR("Artifact"), &pArtifactEx)))
		goto ERROR_ENCOUNTERED;

	// add in methods
	for (iMethod=0; iMethod<cMethod; iMethod++)
	{
		bstrName = rgMethod[iMethod].pszName;

		if (rgMethod[iMethod].pszNameInternal == NULL)
			bstrNameInternal = bstrName;
		else
			bstrNameInternal = rgMethod[iMethod].pszNameInternal;
		
		if (FAILED(hr = pArtifactEx->CreateAndAddMethodExt(	rgMethod[iMethod].lFlags,
														pModule,
														bstrNameInternal,
														bstrName,	
														rgMethod[iMethod].psbits,
														rgMethod[iMethod].permissions )))
            goto ERROR_ENCOUNTERED;
	}

	// add in properties
	if (FAILED(hr = pArtifactEx->AddPropertyExt(	CComBSTR("AccessoryGraphic"),
												CComVariant(CComBSTR("")),
												PSBIT_EXEMPLARDEFAULTPROPERTY,
												PS_EXEMPLARDEFAULTPROPERTY,
												VT_BSTR, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pArtifactEx->AddPropertyExt(	CComBSTR("IsAccessory"),
												CComVariant((bool)VARIANT_FALSE),
												PSBIT_EXEMPLARDEFAULTPROPERTY,
												PS_EXEMPLARDEFAULTPROPERTY,
												VT_BOOL, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pArtifactEx->AddPropertyExt(CComBSTR("IsWorn"),
												CComVariant((bool)VARIANT_FALSE),
												PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
												PS_ALLACCESSPROPERTY,
												VT_BOOL, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	// amend the "Menu" menu
	if (FAILED(hr = pArtifactEx->get_ObjectPropertyExt(CComBSTR("Menu"), IID_IPropertyList, (IObjectProperty **) &pMenu)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = CreateMenuItemExt(m_pWorld, CComBSTR("Wear"), 0, NULL, CComBSTR("UpdateWear"), CComBSTR("Wear"), NULL, &pMenuItem)))
		goto ERROR_ENCOUNTERED;
//		if (FAILED(hr = pMenuItem->put_SortOrder(MI_SORT_GENERIC)))
//  		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMenu->AddObjectProperty(pMenuItem)))
		goto ERROR_ENCOUNTERED;
	pMenuItem.Release();

	if (FAILED(hr = CreateMenuItemExt(m_pWorld, CComBSTR("Take Off"), 0, NULL, CComBSTR("UpdateUnWear"), CComBSTR("UnWear"), NULL, &pMenuItem)))
		goto ERROR_ENCOUNTERED;
//		if (FAILED(hr = pMenuItem->put_SortOrder(MI_SORT_GENERIC)))
//  		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMenu->AddObjectProperty(pMenuItem)))
		goto ERROR_ENCOUNTERED;
	pMenuItem.Release();

	// Add items to edit maps
	// Need to actually create the lists, because they aren't created in
	// the Avatar base exemplar.  If that changes, these will break
	bstrTexture = "Texture|Edit an object's color or texture|texture.htm";
	// Owner edit map
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrTexture)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pArtifactEx->put_ObjectProperty(CComBSTR("OwnerEditMap"), (IObjectProperty *) pList)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

	// Builder (author) edit map
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrTexture)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pArtifactEx->put_ObjectProperty(CComBSTR("BuilderEditMap"), (IObjectProperty *) pList)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

	// Advanced (wizard) Edit Map
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrTexture)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pArtifactEx->put_ObjectProperty(CComBSTR("AdvancedEditMap"), (IObjectProperty *) pList)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

    return S_OK;

ERROR_ENCOUNTERED:

	VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::InstallArtifactExtensions: Error encountered: %x\n", hr);
	return hr;
}

//******************************************************
// UpdateWear
// Updates the Wear menuitem

STDMETHODIMP CMultimediaExemplarObject::UpdateWear(IMenuItem *pmi)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing>				pUser, pThing, pLocation;
	VARIANT_BOOL				bAccessory, bWorn;
	long						lFlags = 0;
    HRESULT                     hr = S_OK;

	static CComBSTR		bstrGeometry("Geometry");

    if (pmi==NULL)
    {
        // we can exit with an error here since there is no menu...
    	VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::UpdateWear: NULL pointer\n");
        return ReportMMError(E_POINTER);
    }

	// Wear should only be enabled if the artifact is being held by the user, is an accessory
	// and is not already worn.
	if (FAILED(hr = pmi->get_Owner(&pThing)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThing->get_Container(&pLocation)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pWorld->get_User(&pUser)))
		goto ERROR_ENCOUNTERED;
    if (pLocation != pUser)
        goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThing->get_BOOL(CComBSTR("isWorn"), &bWorn)))
		goto ERROR_ENCOUNTERED;
	if (bWorn==VARIANT_TRUE)
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThing->get_BOOL(CComBSTR("IsAccessory"), &bAccessory)))
		goto ERROR_ENCOUNTERED;
    if (bAccessory != VARIANT_TRUE)
        goto ERROR_ENCOUNTERED;

	// Menu item is currently owned by artifact, actually want it to be owned by connected
	// user and to be called with the artifact as a parameter.
	if (FAILED(hr = pmi->put_Owner(pUser)))
		goto ERROR_ENCOUNTERED;

    if (FAILED(hr = pmi->put_ArgumentsExt(CComDISPPARAMS(1, CComVariant((IDispatch*)pThing)))))
		goto ERROR_ENCOUNTERED;

	pmi->get_Flags(&lFlags);
	lFlags &= ~MI_INVISIBLE;
	pmi->put_Flags(lFlags);

    return S_OK; // normal case

ERROR_ENCOUNTERED:
    // NOTE: we may get here even if hr == S_OK ...see above..
	pmi->get_Flags(&lFlags);
	lFlags |= MI_INVISIBLE;
	pmi->put_Flags(lFlags);

	// REVIEW: Return S_OK to ensure other menu items are given a chance to update.
	return S_OK;
}

//******************************************************
// UpdateUnWear
// Updates the UnWear menuitem

STDMETHODIMP CMultimediaExemplarObject::UpdateUnWear(IMenuItem *pmi)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing>				pUser, pThing, pLocation;
	VARIANT_BOOL				bAccessory, bWorn;
	long						lFlags = 0;
    HRESULT                     hr = S_OK;

	static CComBSTR		bstrGeometry("Geometry");

    if (pmi==NULL)
    {
        // we can exit with an error here since there is no menu...
    	VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::UpdateUnWear: NULL pointer\n");
        return ReportMMError(E_POINTER);
    }

	// TakeOff should only be enabled if the artifact is being held by the user, is an accessory
	// and is being worn.
	if (FAILED(hr = pmi->get_Owner(&pThing)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThing->get_Container(&pLocation)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pWorld->get_User(&pUser)))
		goto ERROR_ENCOUNTERED;
    if (pLocation != pUser)
        goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThing->get_BOOL(CComBSTR("isWorn"), &bWorn)))
		goto ERROR_ENCOUNTERED;
	if (bWorn!=VARIANT_TRUE)
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThing->get_BOOL(CComBSTR("IsAccessory"), &bAccessory)))
		goto ERROR_ENCOUNTERED;
    if (bAccessory != VARIANT_TRUE)
        goto ERROR_ENCOUNTERED;

	// Menu item is currently owned by artifact, actually want it to be owned by connected
	// user and to be called with the artifact as a parameter.
	if (FAILED(hr = pmi->put_Owner(pUser)))
		goto ERROR_ENCOUNTERED;

    if (FAILED(hr = pmi->put_ArgumentsExt(CComDISPPARAMS(1, CComVariant((IDispatch*)pThing)))))
		goto ERROR_ENCOUNTERED;

	pmi->get_Flags(&lFlags);
	lFlags &= ~MI_INVISIBLE;
	pmi->put_Flags(lFlags);

	return S_OK;

ERROR_ENCOUNTERED:		
	pmi->get_Flags(&lFlags);
	lFlags |= MI_INVISIBLE;
	pmi->put_Flags(lFlags);

	// REVIEW: Return S_OK to ensure other menu items are given a chance to update.
	return S_OK;
}

//********************************************************
// OnIsWornAccessoryChanged
//

STDMETHODIMP CMultimediaExemplarObject::OnIsAccessoryChanged(long lHint, VARIANT varHintData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr = S_OK;
	CComPtr<IThing> pThis;
	static CComBSTR	kbstrUIChanged(VW_UIELEMENTCHANGED_EVENT_STR);

	if (lHint != thPut)
		return S_OK;

	// get this
	hr = m_pWorld->get_This(&pThis);
	if (FAILED(hr))
		return hr;

	hr = m_pWorld->FireUIEvent(pThis, kbstrUIChanged, CComVariant());
	return hr;
}