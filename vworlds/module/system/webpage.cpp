// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// WebPage.cpp : Implementation of WebPage exemplar.

#include "stdafx.h"
#include <vwsysex.h>
#include <menuitem.h>
#include <vwutils.h>
#include "propbase.h"
#include "syshelp.h"
#include "WebPage.h"
#include <vwevents.h>

const CComBSTR bstrHTMLURL("HTMLURL");
const CComBSTR bstrIsModal("IsModal");
const CComBSTR bstrWindowFeatures("PopupWindowFeatures");
const CComBSTR bstrWebPage("Web Page|Edit the web page properties|WebPageEx.htm");


/////////////////////////////////////////////////////////
// CWebPage
// The WebPageExemplar displays an HTML page in-world

/////////////////////////////////////////////////////////
// Install
// Installs this module - creates the exemplar and
// adds the properties and methods.

HRESULT CWebPageExemplarObject::Install(IModule* pModule)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CMethodInfo rgMethod[] =
	{
		{ METHOD_CLIENT, "Show",		NULL,		PSBIT_EXEMPLARDEFAULTMETHOD,	PS_ALLEXECUTEMETHOD },	
		{ METHOD_CLIENT, "OnLButtonUp",	NULL,		PSBIT_EXEMPLARDEFAULTMETHOD,	PS_ALLEXECUTEMETHOD }	

	};
	int cMethod = sizeof(rgMethod)/sizeof(CMethodInfo);

    HRESULT hr = S_OK;
	CComPtr<IThing> pWebPageEx;
	CComBSTR bstrName;
	CComBSTR bstrNameInternal;
	int iMethod;
	CComPtr<IPropertyList> pMenuEx;
	CComPtr<IPropertyList> pMenu;
	CComPtr<IMenuItem> pMenuItem;
	CComPtr<IPropertyList> pList;

	// Create the WebPage exemplar
	if (FAILED(hr = m_pWorld->CreateExemplar(CComBSTR("WebPage"), CComVariant(CComBSTR("Artifact")), &pWebPageEx)))
    	goto ERROR_ENCOUNTERED;

	// Add the methods
	for (iMethod=0; iMethod<cMethod; iMethod++)
	{
		bstrName = rgMethod[iMethod].pszName;
		
		if (rgMethod[iMethod].pszNameInternal == NULL)
			bstrNameInternal = bstrName;
		else
			bstrNameInternal = rgMethod[iMethod].pszNameInternal;
		
		hr = pWebPageEx->CreateAndAddMethodExt(rgMethod[iMethod].lFlags,
													pModule,
													bstrNameInternal,
													bstrName,
													rgMethod[iMethod].psbits,
													rgMethod[iMethod].permissions);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CWebPageExemplarObject::Install  Can't create method %s \n", CString(bstrName));
			goto ERROR_ENCOUNTERED;
		}
	}

	// Create the default properties
	if (FAILED(hr = pWebPageEx->AddPropertyExt(bstrHTMLURL,
									   CComVariant(CComBSTR("http://www.microsoft.com")),
									   PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_FRIENDLY, 
									   PS_EXEMPLARDEFAULTPROPERTY, 
									   VT_BSTR, IID_NULL, NULL)))
    	goto ERROR_ENCOUNTERED;


	if (FAILED(hr = pWebPageEx->AddPropertyExt(bstrIsModal, 
									   CComVariant((bool)VARIANT_TRUE),
									   PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_FRIENDLY, 
									   PS_EXEMPLARDEFAULTPROPERTY, 
									   VT_BOOL, IID_NULL, NULL)))
    	goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pWebPageEx->AddPropertyExt(bstrWindowFeatures, 
									   CComVariant(CComBSTR("dialogHeight:400px;dialogWidth:400px")),
									   PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_FRIENDLY, 
									   PS_EXEMPLARDEFAULTPROPERTY, 
									   VT_BSTR, IID_NULL, NULL)))
    	goto ERROR_ENCOUNTERED;

	// Get the menu item list

	// get the current menu (comes from exemplar, not COPYONGET)
	if (FAILED(hr = pWebPageEx->get_ObjectProperty(CComBSTR("Menu"), (IObjectProperty**) &pMenuEx)))
    	goto ERROR_ENCOUNTERED;

	// create the new one
	if (FAILED(hr = CreateMenu(m_pWorld, &pMenu)))
    	goto ERROR_ENCOUNTERED;

	// copy old to new
	if (FAILED(hr = pMenuEx->CopyTo(pMenu)))
    	goto ERROR_ENCOUNTERED;

	// Add in the menu items
	if (FAILED(hr = CreateMenuItemExt(m_pWorld, CComBSTR("Show..."), 0, pWebPageEx, NULL,
				CComBSTR("Show"), NULL, &pMenuItem)))
    	goto ERROR_ENCOUNTERED;
//	if (FAILED(hr = pMenuItem->put_SortOrder(MI_SORT_PRIMARY +110)))
//     	goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMenu->AddObjectProperty(pMenuItem)))
    	goto ERROR_ENCOUNTERED;

	// put it into the instance
	if (FAILED(hr = pWebPageEx->put_ObjectProperty(CComBSTR("Menu"), pMenu)))
		goto ERROR_ENCOUNTERED;

	// Add exemplar-specific edit page 

	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrWebPage)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pWebPageEx->put_ObjectProperty(CComBSTR("EndUserEditMap"), (IObjectProperty *) pList)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrWebPage)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pWebPageEx->put_ObjectProperty(CComBSTR("OwnerEditMap"), (IObjectProperty *) pList)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrWebPage)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pWebPageEx->put_ObjectProperty(CComBSTR("BuilderEditMap"), (IObjectProperty *) pList)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrWebPage)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pWebPageEx->put_ObjectProperty(CComBSTR("AdvancedEditMap"), (IObjectProperty *) pList)))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	if (hr != S_OK)
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CWebPageExemplarObject::Install   Error encountered %x\n", hr);

    return hr;
}

STDMETHODIMP CWebPageExemplarObject::Show()
{
	CComPtr<IThing>		pThis;
	IPropertyList*		pPropList = NULL;
	CComBSTR			bstrHTML;
	VARIANT_BOOL		bIsModal;
  	CComBSTR			bstrPopupWindowFeatures;
	HRESULT				hr = S_OK;

	// Get the this object
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	// Get URL to display
	if (FAILED(hr = pThis->get_String(bstrHTMLURL, &bstrHTML.m_str)))
        return hr;

	// Is this modal or modeless?
	if (FAILED(hr = pThis->get_BOOL(bstrIsModal, &bIsModal)))
	    return hr;

	// What window features are there?
	if (FAILED(hr = pThis->get_String(bstrWindowFeatures, &bstrPopupWindowFeatures.m_str)))
        return hr;

	if (bIsModal)
	{

		hr = ShowDialog(m_pWorld, VARIANT_TRUE, bstrHTML, pThis, bstrPopupWindowFeatures);
	}
	else
	{
		// If text, get the text and display it
		hr = ShowDialog(m_pWorld, VARIANT_FALSE, bstrHTML, pThis, bstrPopupWindowFeatures);

	}

    return hr;
}

STDMETHODIMP CWebPageExemplarObject::OnLButtonUp(int hwnd, int x, int y, VARIANT_BOOL *pbool)
{
	if (pbool==NULL)
    {
		VWTRACE(m_pWorld, "CORREXEMP", VWT_ERROR, "CWebPageExemplarObject::OnLButtonUp: NULL output pointer\n");
		return ReportWebPageError(E_POINTER);
    }

	// Synonym for ShowMessage
	*pbool = VARIANT_FALSE;
	return Show();
}
