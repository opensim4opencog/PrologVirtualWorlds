// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include <vwfound.h>
#include <vector.h>
#include <vwframe.h>
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

HRESULT CFoundationExemplars::SignInstall(IModule* pModule)
{
	HRESULT hr = S_OK;
	IThing *pSignEx = NULL;
	IVector *pVector = NULL;
	IVWGeometry *pGeometry = NULL;
	IPropertyList *pMenu = NULL;
	IPropertyList *pMenuEx = NULL;
	IMenuItem *pMenuItem = NULL;
	static CComBSTR bstrBlank("");
	CComBSTR bstrName, bstrNameInternal;

	CMethodInfo rgMethod[] =
	{
		{ METHOD_CLIENT, "OnLoadGeometry",			"OnLoadSign",				PSBIT_EVENTHANDLER,		PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER, "SetTextAttributes",		"SetTextAttributes",		PSBIT_EXEMPLARDEFAULTMETHOD,PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "SignSetTextureFromText",	"SignSetTextureFromText",	PSBIT_EVENTHANDLER,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "OnFrameChanged",			"OnSignPropChanged",		PSBIT_EVENTHANDLER,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "OnBackgroundColorChanged","OnSignPropChanged",		PSBIT_EVENTHANDLER,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "OnTextColorChanged",		"OnSignPropChanged",		PSBIT_EVENTHANDLER,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "OnSignChanged",			"OnSignPropChanged",		PSBIT_EVENTHANDLER,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "Refresh",					"RefreshSign",				PSBIT_EVENTHANDLER,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "UpdateMenu",				"SignUpdateMenu", 			PSBIT_EVENTHANDLER,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT, "OnSelectSignCustomize",	"OnSelectSignCustomize",	PSBIT_EVENTHANDLER,		PS_ALLEXECUTEMETHOD }
	};
	int cMethod = sizeof(rgMethod)/sizeof(rgMethod[0]);
    int iMethod;

	// Create the Sign exemplar
	if (FAILED(hr = m_pWorld->CreateExemplarEx(CComBSTR("Sign Exemplar"),
												CComBSTR("Sign"),
												CComVariant(CComBSTR("Artifact")), &pSignEx)))
		goto ERROR_ENCOUNTERED;

	// Set the description string
	if (FAILED(hr = pSignEx->put_Description(CComBSTR("Sign Exemplar"))))
		goto ERROR_ENCOUNTERED;

	// Position it
	if (FAILED(hr = CreateVector(m_pWorld, 0.0, 0.0, 0.0, &pVector)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pSignEx->put_ObjectProperty(CComBSTR("Position"), pVector)))
		goto ERROR_ENCOUNTERED;
	
    SAFERELEASE(pVector);

	if (FAILED(hr = CreateVector(m_pWorld, 0.0, 0.0, -1.0, &pVector)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pSignEx->put_ObjectProperty(CComBSTR("Direction"), pVector)))
		goto ERROR_ENCOUNTERED;

    SAFERELEASE(pVector);

	if (FAILED(hr = CreateVector(m_pWorld, 0.0, 1.0, 0.0, &pVector)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pSignEx->put_ObjectProperty(CComBSTR("Up"), pVector)))
		goto ERROR_ENCOUNTERED;
	
    SAFERELEASE(pVector);

	// Create a geometry for it
	// Review: shouldn't we have some default geometry? we have a sign.3ds in the Hutch world
	if (FAILED(hr = CreateGeometry(m_pWorld, NULL, &pGeometry)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pSignEx->put_ObjectProperty(CComBSTR("Geometry"), (IObjectProperty*)pGeometry)))
		goto ERROR_ENCOUNTERED;

	// Add methods
	for (iMethod=0; iMethod<cMethod; iMethod++)
	{
		bstrName = rgMethod[iMethod].pszName;

		if (rgMethod[iMethod].pszNameInternal == NULL)
			bstrNameInternal = bstrName;
		else
			bstrNameInternal = rgMethod[iMethod].pszNameInternal;
		
		if (FAILED(hr = pSignEx->CreateAndAddMethodExt(	rgMethod[iMethod].lFlags,
														pModule,
														bstrNameInternal,
														bstrName,
														rgMethod[iMethod].psbits,
														rgMethod[iMethod].permissions)))
		goto ERROR_ENCOUNTERED;
	}


	// Add properties
	if (FAILED(hr = pSignEx->AddPropertyExt(CComBSTR("Sign"),
										CComVariant(bstrBlank),
										PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY, 
										PS_ALLACCESSPROPERTY, 
										VT_BSTR, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pSignEx->AddPropertyExt(CComBSTR("Frame"),
										CComVariant(bstrBlank),
										PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY, 
										PS_ALLACCESSPROPERTY, 
										VT_BSTR, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pSignEx->AddPropertyExt(CComBSTR("Shadow"), 
										CComVariant((bool)VARIANT_TRUE),
										PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN, 
										PS_ALLACCESSPROPERTY, 
										VT_BOOL, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pSignEx->AddPropertyExt(CComBSTR("Width"),
									   CComVariant((short) 128),
									   PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
									   PS_ALLACCESSPROPERTY,
									   VT_I2, IID_IDispatch, NULL))) 
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pSignEx->AddPropertyExt(CComBSTR("Height"),
									   CComVariant((short) 128),
									   PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
									   PS_ALLACCESSPROPERTY,
									   VT_I2, IID_IDispatch, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pSignEx->AddPropertyExt(CComBSTR("PointSize"),
									   CComVariant((short) 32),
									   PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN,
									   PS_ALLACCESSPROPERTY,
									   VT_I2, IID_IDispatch, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pSignEx->AddPropertyExt(CComBSTR("BackgroundColor"),
									   CComVariant((long) 0),
									   PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY,
									   PS_ALLACCESSPROPERTY,
									   VT_VARIANT, IID_IDispatch, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pSignEx->AddPropertyExt(CComBSTR("TextColor"),
									   CComVariant((long) 0xFFFFFF),
									   PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY,
									   PS_ALLACCESSPROPERTY,
									   VT_VARIANT, IID_IDispatch, NULL)))
		goto ERROR_ENCOUNTERED;

	// Copy and amend menu

	// Make the new menu item list
	if (FAILED(hr = CreateMenu(m_pWorld, &pMenu)))
		goto ERROR_ENCOUNTERED;

	// Get the menu for the parent
	if (FAILED(hr = pSignEx->get_ObjectProperty(CComBSTR("Menu"), (IObjectProperty**) &pMenuEx)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMenuEx->CopyTo(pMenu)))
		goto ERROR_ENCOUNTERED;

	// Add menu item to it
	if (FAILED(hr = CreateMenuItemExt(m_pWorld, CComBSTR("Customize..."), 0, pSignEx, 
				CComBSTR("UpdateMenu"), CComBSTR("OnSelectSignCustomize"), NULL, &pMenuItem)))
		goto ERROR_ENCOUNTERED;
//		if (FAILED(hr = pMenuItem->put_SortOrder(MI_SORT_PRIMARY + 110)))
//  		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMenu->AddObjectProperty(pMenuItem)))
		goto ERROR_ENCOUNTERED;

	// Stick the new menu into the exemplar
	if (FAILED(hr = pSignEx->put_ObjectProperty(CComBSTR("Menu"), (IObjectProperty*) pMenu)))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:

	// Clean up
	SAFERELEASE(pSignEx);
	SAFERELEASE(pVector);
	SAFERELEASE(pGeometry);
	SAFERELEASE(pMenu);
	SAFERELEASE(pMenuEx);
	SAFERELEASE(pMenuItem);

	return hr;
}

STDMETHODIMP CFoundationExemplars::OnLoadSign(IVWFrame *pvwf)
{
	HRESULT hr = S_OK;
	IVWFrame *pvwfTemp = NULL; 
	CComBSTR bstrCanvasFrame = "canvas";
	CComBSTR bstrBackingFrame = "backing";

	pvwf->GetFrameByName(bstrCanvasFrame, &pvwfTemp);
	if (!pvwfTemp) 
	{
		// no canvas frame so we should make one:
		if (FAILED(hr = m_pWorld->CreateObjectProperty(CComBSTR("VWSYSTEM.Frame.1"), (IObjectProperty**)&pvwfTemp)))
			goto CLEAN_UP;
		pvwf->AddChild(pvwfTemp);
		pvwfTemp->SetPositionEx(pvwf, 0.0F, 0.0F, 0.01F);
		pvwfTemp->SetName(bstrCanvasFrame);
		pvwfTemp->CreateUnitMesh();
	}
	SAFERELEASE(pvwfTemp);
	pvwf->GetFrameByName(bstrBackingFrame, &pvwfTemp);
	if (!pvwfTemp) 
	{
		// no canvas frame so we should make one:
		if (FAILED(hr = m_pWorld->CreateObjectProperty(CComBSTR("VWSYSTEM.Frame.1"), (IObjectProperty**)&pvwfTemp)))
			goto CLEAN_UP;
		pvwf->AddChild(pvwfTemp);
		pvwfTemp->SetName(bstrBackingFrame);
		pvwfTemp->CreateUnitMesh();
	}

	RefreshSign();
	
CLEAN_UP:

	SAFERELEASE(pvwfTemp);

	return hr;
}

STDMETHODIMP CFoundationExemplars::OnSignPropChanged(long lHint, VARIANT varHintData)
{
	return RefreshSign();
}

////////////////////////////////////////////////////////////
// SetTextAttributes
// Server-side method that sets properties & calls 
// client-side method to update the frame

STDMETHODIMP CFoundationExemplars::SetTextAttributes(VARIANT_BOOL bShadow, short nWidth, short nHeight, short nPointSize)
{
	HRESULT		hr				= S_OK;
	IThing		*pThis			= NULL;
	// review this should be in a resource
	CComBSTR	bstrShadow		= "Shadow";
	CComBSTR	bstrWidth		= "Width";
	CComBSTR	bstrHeight		= "Height";
	CComBSTR	bstrPointSize	= "PointSize";

	GETPROP(m_pWorld, This, &pThis);

	PUTADDEDPROP(pThis, BOOL, bstrShadow, bShadow);
	PUTADDEDPROP(pThis, Short, bstrWidth, nWidth);
	PUTADDEDPROP(pThis, Short, bstrHeight, nHeight);
	PUTADDEDPROP(pThis, Short, bstrPointSize, nPointSize);
		
	hr = pThis->InvokeMethodExt(CComBSTR("SignSetTextureFromText"), NULL, NULL);

CLEAN_UP:

	SAFERELEASE(pThis);
	return hr;
}

////////////////////////////////////////////////////////////
// SignSetTextureFromText
// Client-side method that calls frame SetTextureFromText
// Only changes text attributes - Refresh updates all

STDMETHODIMP CFoundationExemplars::SignSetTextureFromText()
{
	HRESULT		hr				= S_OK;
	IThing		*pThis			= NULL;
	IVWFrame	*pvwfTransform	= NULL;
	IVWFrame	*pvwfCanvFrame	= NULL; 
	VARIANT_BOOL bShadowed;
	short		nWidth, nHeight, nPointSize;
	// review this should be in a resource
	CComBSTR	bstrSignText;
	CComBSTR	bstrShadow		= "Shadow";
	CComBSTR	bstrWidth		= "Width";
	CComBSTR	bstrHeight		= "Height";
	CComBSTR	bstrPointSize	= "PointSize";
	CComBSTR	bstrSign		= "Sign";
	CComBSTR	bstrCanvasFrame	= "canvas";


	GETPROP(m_pWorld, This, &pThis);

	// Do set here, not in Refresh
	GETPROP(m_pWorld, This, &pThis);
	if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)))
		goto CLEAN_UP;

	if (pvwfTransform) 
	{
		GETADDEDPROP(pThis, String, bstrSign, &bstrSignText.m_str);
		GETADDEDPROP(pThis, BOOL, bstrShadow, &bShadowed);
		GETADDEDPROP(pThis, Short, bstrWidth, &nWidth);
		GETADDEDPROP(pThis, Short, bstrHeight, &nHeight);
		GETADDEDPROP(pThis, Short, bstrPointSize, &nPointSize);

		pvwfTransform->GetFrameByName(bstrCanvasFrame, &pvwfCanvFrame);
		if (pvwfCanvFrame) 
		{
			// check for no Sign name
			if (bstrSignText.Length())
				pvwfCanvFrame->SetMeshTextureFromText(VWFRAME_ALLHIERARCHY, bstrSignText, bShadowed, nWidth, nHeight, nPointSize);
		}
	}

CLEAN_UP:

	SAFERELEASE(pThis);
	SAFERELEASE(pvwfTransform);
	SAFERELEASE(pvwfCanvFrame);
	return hr;
}

/////////////////////////////////////////////////
// RefreshSign
// Updates all of the sign attributes, including
// text, background, sign
// Called when these sign properties are updated:
// sign, frame, background color, text color

STDMETHODIMP CFoundationExemplars::RefreshSign()
{
	HRESULT		hr				= S_OK;
	IVWFrame	*pvwfCanvFrame	= NULL; 
	IVWFrame	*pvwfTransform	= NULL;
	IVWFrame	*pvwfFrameFrame	= NULL; 
	IVWFrame	*pvwfBackingFrame = NULL;
	IThing		*pThis			= NULL;
	CComBSTR	bstrSignText;
	CComBSTR	bstrFrameName;
	CComVariant	varTextColor;
	CComVariant	varBackgroundColor;

	// review this should be in a resource
	CComBSTR	bstrCanvasFrame	= "canvas";
	CComBSTR	bstrBackingFrame= "backing";
	CComBSTR	bstrFrameFrame  = "frame";
	CComBSTR	bstrFrame		= "Frame";
	CComBSTR	bstrTextColor	= "TextColor";
	CComBSTR	bstrBackgroundColor = "BackgroundColor";

	GETPROP(m_pWorld, This, &pThis);
	if (FAILED(hr = _GetFrameHelper(pThis, &pvwfTransform)))
		goto CLEAN_UP;

	if (pvwfTransform) 
	{
		GETPROP(m_pWorld, This, &pThis);
		GETADDEDPROP(pThis, String, bstrFrame, &bstrFrameName.m_str);

		// get color properties
		pThis->get_Property(bstrTextColor, &varTextColor);
		pThis->get_Property(bstrBackgroundColor, &varBackgroundColor);
		if (FAILED(varTextColor.ChangeType(VT_I4)))
		{
			varTextColor.Clear();
			varTextColor.vt = VT_I4;
			varTextColor.lVal = 0;
		}
		if (FAILED(varBackgroundColor.ChangeType(VT_I4)))
		{
			varBackgroundColor.Clear();
			varBackgroundColor.vt = VT_I4;
			varBackgroundColor.lVal = 0;
		}

		// break out and normalize components
		float flRed, flGreen, flBlue;
		flRed =   (float) ((varTextColor.lVal & 0xFF0000)>>16)/255.0f;
		flGreen = (float) ((varTextColor.lVal & 0x00FF00)>>8 )/255.0f;
		flBlue =  (float) ((varTextColor.lVal & 0x0000FF)    )/255.0f;
	
		pvwfTransform->GetFrameByName(bstrCanvasFrame, &pvwfCanvFrame);
		if (pvwfCanvFrame) 
		{
			pvwfCanvFrame->SetMeshColor(VWFRAME_ALLHIERARCHY, flRed, flGreen, flBlue);
		}

		// break out and normalize components
		flRed =   (float) ((varBackgroundColor.lVal & 0xFF0000)>>16)/255.0f;
		flGreen = (float) ((varBackgroundColor.lVal & 0x00FF00)>>8 )/255.0f;
		flBlue =  (float) ((varBackgroundColor.lVal & 0x0000FF)    )/255.0f;

		pvwfTransform->GetFrameByName(bstrBackingFrame, &pvwfBackingFrame);
		if (pvwfBackingFrame) 
			pvwfBackingFrame->SetMeshColor(VWFRAME_ALLHIERARCHY, flRed, flGreen, flBlue);
		
		pvwfTransform->GetFrameByName(bstrFrameFrame, &pvwfFrameFrame);
		// If we have pointer to frame and the frame name exists, set the texture
		if ((pvwfFrameFrame)&&(bstrFrameName.Length())) 
		{
			pvwfFrameFrame->SetMeshTextureFromURL(VWFRAME_ALLHIERARCHY, bstrFrameName);
		}
		// Now call SignSetTextureFromText to update text elements
		SignSetTextureFromText();
	}
	
CLEAN_UP:

	SAFERELEASE(pvwfCanvFrame);
	SAFERELEASE(pvwfFrameFrame);
	SAFERELEASE(pvwfBackingFrame);
	SAFERELEASE(pvwfTransform);
	SAFERELEASE(pThis);
	return hr;
}

STDMETHODIMP CFoundationExemplars::SignUpdateMenu(IMenuItem *pmi)
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

	// Always enabled because signs have ALLACCESS set on their properties
	lFlags &= ~MI_INVISIBLE;

	// put the flags back
	PUTPROP(pmi, Flags, lFlags);

CLEAN_UP:
	SAFERELEASE(pTarget);

	return hr;
}

STDMETHODIMP CFoundationExemplars::OnSelectSignCustomize()
{
	HRESULT hr = S_OK;
	IThing *pThis = NULL;
	// review this should be in a resource
	CComBSTR bstrFrame = "Frame";
	CComBSTR bstrURL = "sdk/editors/exemplar/sign/signmain.htm";

	GETPROP(m_pWorld, This, &pThis);
	hr =  m_pWorld->ShowURLEx(	CComBSTR("SignWindow"), 
										CComBSTR("Sign"),
										bstrURL,
										CComVariant(pThis), 
										CComVariant(45), CComVariant(45),
										CComVariant(450), CComVariant(530) );

CLEAN_UP:

	SAFERELEASE(pThis);

	return hr;
}

