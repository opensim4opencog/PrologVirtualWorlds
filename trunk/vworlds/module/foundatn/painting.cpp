// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include <vwfound.h>
#include <vector.h>
#include <menuitem.h>
#include <vwutils.h>
#include "foundatn.h"
#include "propbase.h"
#include "syshelp.h"
#include "mmhelp.h"

extern HRESULT _GetFrameHelper(IThing *pThing, IVWFrame **ppvwf);
#define GETPROP(obj,name,val) hr = ##obj->get_##name(##val); if (FAILED(hr)) goto CLEAN_UP
#define GETADDEDPROP(obj,type,name,val) hr = ##obj->get_##type(##name, ##val); if (FAILED(hr)) goto CLEAN_UP

#define PUTPROP(obj,name,val) hr = ##obj->put_##name(##val); if (FAILED(hr)) goto CLEAN_UP
#define PUTADDEDPROP(obj,type,name,val) hr = ##obj->put_##type(##name, ##val); if (FAILED(hr)) goto CLEAN_UP
HRESULT	_GetFrameHelper(IThing *pThing, IVWFrame **ppvwf);

HRESULT	_GetFrameHelper(IThing *pThing, IVWFrame **ppvwf)
{
	HRESULT		hr;
	CComVariant	varReturnVal;
	
	if (ppvwf==NULL)
		return E_POINTER;
	*ppvwf = NULL;

	if (FAILED(hr = pThing->InvokeMethodExt(CComBSTR("GetFrame"), NULL, &varReturnVal)))
		goto CLEAN_UP;

	if (varReturnVal.vt != VT_DISPATCH || !varReturnVal.pdispVal)
		goto CLEAN_UP;

	*ppvwf = (IVWFrame*)varReturnVal.pdispVal;
	SAFEADDREF(varReturnVal.pdispVal);

CLEAN_UP:

	return hr;
}

HRESULT CFoundationExemplars::PaintInstall(IModule* pModule)
{
	HRESULT hr = S_OK;
	IThing *pPaintingEx = NULL;
	IVector *pVector = NULL;
	IVWGeometry *pGeometry = NULL;
	IPropertyList *pMenu = NULL;
	IPropertyList *pMenuEx= NULL;
	IMenuItem *pMenuItem = NULL;
	IThing *pExemplar = NULL;
	CComBSTR bstrBlank("");
	CComBSTR bstrName, bstrNameInternal;

	CMethodInfo rgMethod[] =
	{
		{ METHOD_CLIENT, "OnLoadGeometry",			"OnLoadPainting",			PSBIT_EVENTHANDLER, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "OnPaintingChanged",		"OnPaintingPropChanged",	PSBIT_EVENTHANDLER,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "OnFrameChanged",			"OnPaintingPropChanged",	PSBIT_EVENTHANDLER,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "Refresh",					"RefreshPainting",			PSBIT_EVENTHANDLER,	PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_CLIENT, "UpdateMenu",				"PaintingUpdateMenu",		PSBIT_EVENTHANDLER,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "OnSelectPaintingCustomize","OnSelectPaintingCustomize", PSBIT_EVENTHANDLER, PS_ALLEXECUTEMETHOD }
	};
	
	int cMethod = sizeof(rgMethod)/sizeof(rgMethod[0]);
    int iMethod;

	// Create the Message exemplar
	if (FAILED(hr = m_pWorld->CreateExemplarEx(CComBSTR("Painting Exemplar"),
												CComBSTR("Painting"),
												CComVariant(CComBSTR("Artifact")), &pPaintingEx)))
        goto ERROR_ENCOUNTERED;

	// Set the description string
	if (FAILED(hr = pPaintingEx->put_Description(CComBSTR("Painting Exemplar"))))
	    goto ERROR_ENCOUNTERED;

	// Position it
	if (FAILED(hr = CreateVector(m_pWorld, 0.0, 0.0, 0.0, &pVector)))
	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pPaintingEx->put_ObjectProperty(CComBSTR("Position"), pVector)))
	    goto ERROR_ENCOUNTERED;
	SAFERELEASE(pVector);

	if (FAILED(hr = CreateVector(m_pWorld, 0.0, 0.0, -1.0, &pVector)))
    	goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pPaintingEx->put_ObjectProperty(CComBSTR("Direction"), pVector)))
	    goto ERROR_ENCOUNTERED;
	SAFERELEASE(pVector);

	if (FAILED(hr = CreateVector(m_pWorld, 0.0, 1.0, 0.0, &pVector)))
    	goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pPaintingEx->put_ObjectProperty(CComBSTR("Up"), pVector)))
	    goto ERROR_ENCOUNTERED;
	SAFERELEASE(pVector);
	
	// Create a geometry for it - review: shouldn't we have a default file?
	if (FAILED(hr = CreateGeometry(m_pWorld, NULL, &pGeometry)))
    	goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pPaintingEx->put_ObjectProperty(CComBSTR("Geometry"), (IObjectProperty*)pGeometry)))
	    goto ERROR_ENCOUNTERED;

	// Add methods
	for (iMethod=0; iMethod<cMethod; iMethod++)
	{
		bstrName = rgMethod[iMethod].pszName;

		if (rgMethod[iMethod].pszNameInternal == NULL)
			bstrNameInternal = bstrName;
		else
			bstrNameInternal = rgMethod[iMethod].pszNameInternal;
		
		if (FAILED(hr = pPaintingEx->CreateAndAddMethodExt(	rgMethod[iMethod].lFlags,
														pModule,
														bstrNameInternal,
														bstrName,
														rgMethod[iMethod].psbits,
														rgMethod[iMethod].permissions)))
            goto ERROR_ENCOUNTERED;
	}

	// Add properties
	if (FAILED(hr = pPaintingEx->AddPropertyExt(CComBSTR("Painting"),
											CComVariant(bstrBlank),
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_ALLACCESSPROPERTY,
											VT_BSTR, IID_NULL, NULL)))
        goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPaintingEx->AddPropertyExt(CComBSTR("Frame"),
											CComVariant(bstrBlank),
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_ALLACCESSPROPERTY,
											VT_BSTR, IID_NULL, NULL)))
        goto ERROR_ENCOUNTERED;

	// Copy and amend menu

	// Make the new menu item list
	if (FAILED(hr = CreateMenu(m_pWorld, &pMenu)))
	    goto ERROR_ENCOUNTERED;

	// Get the menu for the parent
	if (FAILED(hr = pPaintingEx->get_ObjectProperty(CComBSTR("Menu"), (IObjectProperty**) &pMenuEx)))
	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMenuEx->CopyTo(pMenu)))
    	goto ERROR_ENCOUNTERED;

	// Add menu item to it
	if (FAILED(hr = CreateMenuItemExt(m_pWorld, CComBSTR("Customize..."), 0, pPaintingEx, 
				CComBSTR("UpdateMenu"), CComBSTR("OnSelectPaintingCustomize"), NULL, &pMenuItem)))
        goto ERROR_ENCOUNTERED;

//		if (FAILED(hr = pMenuItem->put_SortOrder(MI_SORT_PRIMARY + 110)))
//      	goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMenu->AddObjectProperty(pMenuItem)))
	    goto ERROR_ENCOUNTERED;

	// Stick the new menu into the exemplar
	hr = pPaintingEx->put_ObjectProperty(CComBSTR("Menu"), (IObjectProperty*) pMenu);

ERROR_ENCOUNTERED:

    // Clean up
	SAFERELEASE(pPaintingEx);
	SAFERELEASE(pVector);
	SAFERELEASE(pGeometry);
	SAFERELEASE(pMenu);
	SAFERELEASE(pMenuEx);
	SAFERELEASE(pMenuItem);
	SAFERELEASE(pExemplar);

	return hr;
}

STDMETHODIMP CFoundationExemplars::OnLoadPainting(IVWFrame *pvwf)
{
	HRESULT hr = S_OK;
	IVWFrame *pvwfTemp = NULL;
	static CComBSTR bstrCanvas("canvas");

	if (pvwf == NULL)
		return E_INVALIDARG;

	pvwf->GetFrameByName(bstrCanvas, &pvwfTemp);
	if (!pvwfTemp) 
	{
		// no canvas frame so we should make one:
		if (FAILED(hr = m_pWorld->CreateObjectProperty(CComBSTR("VWSYSTEM.Frame.1"), (IObjectProperty**)&pvwfTemp)))
			goto CLEAN_UP;
		pvwf->AddChild(pvwfTemp);
		pvwfTemp->SetName(bstrCanvas);
		pvwfTemp->CreateUnitMesh();
	}
	RefreshPainting();
	
CLEAN_UP:

	SAFERELEASE(pvwfTemp);
	return hr;
}

STDMETHODIMP CFoundationExemplars::OnPaintingPropChanged(long lHint, VARIANT varHintData)
{
	return RefreshPainting();
}

STDMETHODIMP CFoundationExemplars::RefreshPainting()
{
	HRESULT		hr				= S_OK;
	IThing		*pThis			= NULL;
	IVWFrame	*pvwfCanvFrame	= NULL; 
	IVWFrame	*pvwfFrameFrame	= NULL; 
	IVWFrame	*pvwfTransform	= NULL;
	CComBSTR	bstrPaintingName;
	CComBSTR	bstrFrameName;
	// review these should be in resources
	CComBSTR	bstrPainting	= "Painting";
	CComBSTR	bstrFrame		= "Frame";
	CComBSTR	bstrCanvasFrame	= "canvas";
	CComBSTR	bstrFrameFrame  = "frame";


	GETPROP(m_pWorld, This, &pThis);
	if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)))
		goto CLEAN_UP;

	if (pvwfTransform) {
		GETADDEDPROP(pThis, String, bstrPainting, &bstrPaintingName.m_str);
	
		pvwfTransform->GetFrameByName(bstrCanvasFrame, &pvwfCanvFrame);
		if (pvwfCanvFrame) 
		{
			// check for no painting name
			pvwfCanvFrame->SetMeshTextureFromURL(VWFRAME_ALLHIERARCHY, bstrPaintingName);
		}
		GETADDEDPROP(pThis, String, bstrFrame, &bstrFrameName.m_str);
	
		pvwfTransform->GetFrameByName(bstrFrameFrame, &pvwfFrameFrame);
		if (pvwfFrameFrame) {
			// check for no painting name
			pvwfFrameFrame->SetMeshTextureFromURL(VWFRAME_ALLHIERARCHY, bstrFrameName);
		}
	}
	
CLEAN_UP:

	SAFERELEASE(pvwfCanvFrame);
	SAFERELEASE(pvwfFrameFrame);
	SAFERELEASE(pvwfTransform);
	SAFERELEASE(pThis);

	return hr;
}

STDMETHODIMP CFoundationExemplars::PaintingUpdateMenu(IMenuItem *pmi)
{
	// disable unless owner or public (or wizard)
	HRESULT hr = S_OK;
	IThing *pTarget = NULL;
	long lFlags;

	GETPROP(pmi, Target, &pTarget);

	// if there's no target, use the owner
	if (!pTarget)
	{
		GETPROP(pmi, Owner, &pTarget);
		ASSERT(pTarget);
	}

	// get the existing menu flags
	if (FAILED(hr = pmi->get_Flags(&lFlags)))
		goto CLEAN_UP;

    lFlags &= ~MI_INVISIBLE;

	// put the flags back
	PUTPROP(pmi, Flags, lFlags);

CLEAN_UP:
	SAFERELEASE(pTarget);

	return hr;
}

STDMETHODIMP CFoundationExemplars::OnSelectPaintingCustomize()
{
	HRESULT hr = S_OK;
	IThing *pThis = NULL;
	// review this should be in a resource
	CComBSTR bstrFrame = "Frame";
	CComBSTR bstrURL = "sdk/editors/exemplar/painting/pntmain.htm";

	GETPROP(m_pWorld, This, &pThis);
	hr =  m_pWorld->ShowURLEx(	CComBSTR("PaintingWindow"), 
										CComBSTR("Painting"),
										bstrURL,
										CComVariant(pThis), 
										CComVariant(45), CComVariant(45),
										CComVariant(350), CComVariant(345) );

CLEAN_UP:

	SAFERELEASE(pThis);

	return hr;
}

