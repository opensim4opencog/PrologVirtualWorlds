// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include <vwfound.h>
#include <vector.h>
#include <menuitem.h>
#include <syshelp.h>
#include "foundatn.h"
#include "mmhelp.h"
#include <vwutils.h>
#include <vwanim.h>

extern HRESULT _GetFrameHelper(IThing *pThing, IVWFrame **ppvwf);

#define iMonitorPeriod 1000
#define dwMonitorDuration 5000

HRESULT CFoundationExemplars::ThngInstall(IModule *pModule)
{
	CMethodInfo rgMethod[] =
	{
		{ METHOD_CLIENT | METHOD_SERVER,	"InteractAt",			"ThngInteractAt",			PSBIT_EXEMPLARDEFAULTMETHOD,					PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnApproachDone",		"ThngOnApproachDone",		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER,					"StartInteractSprite",	"ThngStartInteractSprite",	PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER,					"MonitorInteractSprite","ThngMonitorInteractSprite", PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_CLIENT,					"OnDropFile",			"ThngOnDropFile",			PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD }
	};
	int cMethod = sizeof(rgMethod)/sizeof(rgMethod[0]);

	// find the Thing exemplar
	CComPtr<IThing> pThingEx;
	CComPtr<IThing> pGlobal;
	CComPtr<IPropertyList> pPropertyList;
	CComPtr<IPropertyList> pEditMenu;
	CComBSTR bstrApproach;
	CComBSTR bstrName;
	CComBSTR bstrNameInternal;
    HRESULT  hr;
    int      iMethod;

	if (FAILED(hr = m_pWorld->get_Exemplar(CComBSTR("Thing"), &pThingEx)))
	    goto ERROR_ENCOUNTERED;

	// add in methods
	for (iMethod=0; iMethod<cMethod; iMethod++)
	{
		bstrName = rgMethod[iMethod].pszName;

		if (rgMethod[iMethod].pszNameInternal == NULL)
			bstrNameInternal = bstrName;
		else
			bstrNameInternal = rgMethod[iMethod].pszNameInternal;
		
		if (FAILED(hr = pThingEx->CreateAndAddMethodExt(rgMethod[iMethod].lFlags,
													pModule,
													bstrNameInternal,
													bstrName,
													rgMethod[iMethod].psbits,
													rgMethod[iMethod].permissions)))
    	    goto ERROR_ENCOUNTERED;
	}

	// add in properties
	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("IsCameraAnchor"), 
											CComVariant((bool)VARIANT_FALSE), 
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_BOOL, IID_NULL, NULL)))
	    goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(	CComBSTR("InteractSpriteFile"), 
											CComVariant(CComBSTR("")),
											PSBIT_LOCAL | PSBIT_COPYONGET | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_BSTR, IID_NULL, NULL)))
	    goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(CComBSTR("InteractSoundURL"), 
											CComVariant(CComBSTR("")), 
											PSBIT_LOCAL | PSBIT_COPYONGET | PSBIT_HIDDEN, 
											PS_EXEMPLARDEFAULTPROPERTY, 
											VT_BSTR, IID_NULL, NULL)))
	    goto ERROR_ENCOUNTERED;

 	if (FAILED(hr = pThingEx->AddPropertyExt(CComBSTR("InteractSpriteAnim"), 
											CComVariant((IDispatch*)NULL), 
											PSBIT_LOCAL | PSBIT_COPYONGET | PSBIT_HIDDEN, 
											PS_EXEMPLARDEFAULTPROPERTY, 
											VT_DISPATCH, IID_IDispatch, NULL)))
	    goto ERROR_ENCOUNTERED;

	// Add global properties
	if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
    	goto ERROR_ENCOUNTERED;

	// REVIEW: Interactions are out for V1.0
#if 0
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pPropertyList)))
	    goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pGlobal->AddPropertyExt(	CComBSTR("Interactions"), 
											CComVariant(pPropertyList),
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN, 
											PS_EXEMPLARDEFAULTPROPERTY, 
											VT_DISPATCH, IID_IPropertyList, NULL)))
	    goto ERROR_ENCOUNTERED;
#endif

    return S_OK;

ERROR_ENCOUNTERED:

    return hr;
}

//******************************************************
// InteractAt
HRESULT CFoundationExemplars::ThngInteractAt(IThing *pThing, BSTR bstrSprFile, BSTR bstrSoundURL, BSTR bstrVerb)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// Get the this
	CComPtr<IThing> pThis;
    HRESULT         hr;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    goto ERROR_ENCOUNTERED;

	// First approach the interactee.
	if (FAILED(hr = pThis->InvokeMethodExt(CComBSTR("Approach"),
									   CComDISPPARAMS(1, CComVariant(pThing)), NULL)))
	    goto ERROR_ENCOUNTERED;

	// Store away the parameters for when the approach is done.
	// we do this later because it has to override the approach...
	if (FAILED(hr = pThis->put_String(CComBSTR("InteractSpriteFile"), bstrSprFile)))
	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThis->put_String(CComBSTR("InteractSoundURL"), bstrSoundURL)))
	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThis->put_String(CComBSTR("InteractVerb"), bstrVerb)))
	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThis->put_Thing(CComBSTR("Interactee"), pThing)))
	    goto ERROR_ENCOUNTERED;

    return S_OK;

ERROR_ENCOUNTERED:

    return hr;
}

HRESULT CFoundationExemplars::ThngOnApproachDone()
{
	CComPtr<IThing>		pThis, pInteractee;
	CComPtr<IVWFrame>	pvwfThis, pvwfInteractee, pvwfParent;
	CComPtr<IPropertyList> pList;
	CComPtr<IVector>	pvPos;
	float				fThisX, fThisY, fThisZ, fInteeX, fInteeY, fInteeZ;
	CComBSTR			bstrSprFile, bstrSoundURL, bstrVerb;
	HRESULT hr = S_OK;

	static CComBSTR bstrPosition("Position");

	// Get the relevant interaction related properties from this object.
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThis->get_String(CComBSTR("InteractSpriteFile"), &bstrSprFile.m_str)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThis->get_String(CComBSTR("InteractSoundURL"), &bstrSoundURL.m_str)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThis->get_String(CComBSTR("InteractVerb"), &bstrVerb.m_str)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThis->get_Thing(CComBSTR("Interactee"), &pInteractee)))
		goto ERROR_ENCOUNTERED;

	// Need all the properties to do interaction.
	if (bstrVerb.Length() == 0 || !pInteractee)
		goto ERROR_ENCOUNTERED;
	
	// Make sure these properties are made empty.
	if (FAILED(hr = pThis->put_String(CComBSTR("InteractSpriteFile"), CComBSTR(""))))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThis->put_String(CComBSTR("InteractSoundURL"), CComBSTR(""))))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThis->put_String(CComBSTR("InteractVerb"), CComBSTR(""))))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThis->put_Thing(CComBSTR("Interactee"), NULL)))
		goto ERROR_ENCOUNTERED;

	// Find out where to put the sprite animation.
	if (bstrSprFile.Length() > 0 && bstrSoundURL.Length() > 0)
	{
		if (FAILED(hr = _GetFrameHelper(pThis, &pvwfThis)) || !pvwfThis)
		{
 			if (FAILED(hr = pThis->get_ObjectProperty(bstrPosition, (IObjectProperty**)&pvPos)))
				goto ERROR_ENCOUNTERED;
			pvPos->get(&fThisX, &fThisY, &fThisZ);
			pvPos.Release();
		}
		else
		{
			if (FAILED(hr = pvwfThis->GetParent(&pvwfParent)) || !pvwfParent)
				goto ERROR_ENCOUNTERED;
			if (FAILED(hr = pvwfThis->GetPositionEx(pvwfParent, &fThisX, &fThisY, &fThisZ)))
				goto ERROR_ENCOUNTERED;
			pvwfParent.Release();
		}
		if (FAILED(hr = _GetFrameHelper(pInteractee, &pvwfInteractee)) || !pvwfInteractee)
		{
 			if (FAILED(hr = pInteractee->get_ObjectProperty(bstrPosition, (IObjectProperty**)&pvPos)))
				goto ERROR_ENCOUNTERED;
			pvPos->get(&fInteeX, &fInteeY, &fInteeZ);
		}
		else
		{
			if (FAILED(hr = pvwfThis->GetParent(&pvwfParent)) || !pvwfParent)
				goto ERROR_ENCOUNTERED;
			if (FAILED(hr = pvwfInteractee->GetPositionEx(pvwfParent, &fInteeX, &fInteeY, &fInteeZ)))
				goto ERROR_ENCOUNTERED;
		}
		fThisX += (fInteeX-fThisX)/2;
		fThisY += (fInteeY-fThisY)/2;
		fThisZ += (fInteeZ-fThisZ)/2;

		// And let the server deal with how long it stays around.
		if (FAILED(hr = pThis->InvokeMethodExt(CComBSTR("StartInteractSprite"),
										   CComDISPPARAMS(5, CComVariant(bstrSprFile),
										   CComVariant(bstrSoundURL),
										   CComVariant(fThisX), CComVariant(fThisY), CComVariant(fThisZ)),
										   NULL)))
			goto ERROR_ENCOUNTERED;
	}

	// Create the Action event subject list and fire the Action event.
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddThing(pThis)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddThing(pInteractee)))
		goto ERROR_ENCOUNTERED;
	pThis->FireEventExt(CComBSTR("Action"),
						CComDISPPARAMS(3, CComVariant(CComBSTR("Interaction")),
										  CComVariant(bstrVerb),
										  CComVariant((IDispatch*) pList)),
						FIREEVENT_DEFAULT);

    return S_OK;

ERROR_ENCOUNTERED:

	return hr;
}

HRESULT CFoundationExemplars::ThngStartInteractSprite(BSTR bstrSprFile, BSTR bstrSoundURL, float fPosX, float fPosY, float fPosZ)
{
	CComPtr<IThing>			pContainer, pInteractSprite;
	CComPtr<IVWAnimator>	pVWAnimator;
	CComPtr<IVWAnimation>	pAnimation;
	CComPtr<IThing>			pThis;
	DISPPARAMS*				pDispparams = NULL;
	static CComBSTR	bstrAnimator("Animator");
	static CComBSTR	bstrAnim("InteractSpriteAnim");
	static CComBSTR	bstrMonitor("MonitorInteractSprite");
	static CComBSTR	bstrPosition("Position");
	static CComBSTR	bstrGeometry("Geometry");
	HRESULT	hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThis->get_Container(&pContainer)))
		goto ERROR_ENCOUNTERED;

	// Get the global animator tool we need from the world.
	if (FAILED(hr = m_pWorld->get_Tool(bstrAnimator, (IUnknown**)&pVWAnimator)))
		goto ERROR_ENCOUNTERED;
	if (!pVWAnimator)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

 	// Get any old animations lying around and delete them (and move associated 
	// sprite back into our inventory).
 	hr = pThis->get_Dispatch(bstrAnim, (IDispatch**)&pAnimation);
 	if (SUCCEEDED(hr) && pAnimation)
	{
 		if (FAILED(hr = pVWAnimator->Remove(pAnimation)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = pAnimation->get_ArgumentsExt(&pDispparams)))
			goto ERROR_ENCOUNTERED;
		if (!pDispparams || pDispparams->cArgs != 2 || pDispparams->rgvarg[1].vt != VT_DISPATCH)
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		if (FAILED(hr = ((IThing*)pDispparams->rgvarg[1].pdispVal)->InvokeMethodExt(
						CComBSTR("MoveInto"),
						CComDISPPARAMS(1, CComVariant((IDispatch*)pThis)),
						NULL)))
			goto ERROR_ENCOUNTERED;
		pAnimation.Release();
 	}
	
	// Create the sprite in our inventory if we haven't already.
	if (FAILED(pThis->FindWithin(CComBSTR("InteractSprite"), CComVariant((bool)VARIANT_TRUE), &pInteractSprite)) || !pInteractSprite)
	{
		if (FAILED(hr = m_pWorld->CreateInstanceExt(CComBSTR("InteractSprite"),
															 CComVariant(CComBSTR("Artifact")), pThis,
															 &pInteractSprite)) || !pInteractSprite)
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = pInteractSprite->put_BOOL(CComBSTR("IsUIVisible"), VARIANT_FALSE)))
			goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pInteractSprite->InvokeMethodExt(CComBSTR("InitializeSpriteGraphics"),
									   CComDISPPARAMS(7, CComVariant(bstrSprFile),
									   CComVariant(fPosX), CComVariant(fPosY), CComVariant(fPosZ),
									   CComVariant(0.0f), CComVariant(0.0f), CComVariant(1.0f)),
									   NULL)))
		goto ERROR_ENCOUNTERED;

	// Move it into the room.
	if (FAILED(hr = pInteractSprite->InvokeMethodExt(CComBSTR("MoveInto"),
					CComDISPPARAMS(1, CComVariant((IDispatch*)pContainer)),
					NULL)))
		goto ERROR_ENCOUNTERED;

	// Create the new animation.
	if (FAILED(hr = pVWAnimator->CreateExt(iMonitorPeriod, pThis, bstrMonitor, CComDISPPARAMS(2, CComVariant(pInteractSprite), CComVariant((long)GetTickCount())), &pAnimation)) || !pAnimation)
	{
		// MUST have an animation set up to move this sprite back into our inventory, 
		// otherwise it will be left around forever.
		hr = pInteractSprite->InvokeMethodExt(CComBSTR("MoveInto"),
			 CComDISPPARAMS(1, CComVariant((IDispatch*)pThis)),
			 NULL);
		goto ERROR_ENCOUNTERED;
	}

	hr = pThis->put_Dispatch(bstrAnim, pAnimation);

	// Start playing the sound.
	pThis->InvokeMethodExt(CComBSTR("Set3DSound"),
						   CComDISPPARAMS(2, CComVariant(bstrSoundURL),
						   CComVariant((bool)VARIANT_TRUE)), NULL);

ERROR_ENCOUNTERED:
	
	if (pDispparams)
	{
		TerminateDISPPARAMS(VARIANT_TRUE, pDispparams);
		delete pDispparams;
	}
	return hr;
}

HRESULT CFoundationExemplars::ThngMonitorInteractSprite(IThing *pInteractSprite, DWORD dwTimeStarted)
{
	CComPtr<IVWAnimator>	pVWAnimator;
	CComPtr<IVWAnimation>	pAnimation;
	CComPtr<IThing>			pThis, pContainer;
	static CComBSTR	bstrAnimator("Animator");
	static CComBSTR	bstrAnim("InteractSpriteAnim");
	HRESULT	hr = S_OK;

	if (GetTickCount()-dwTimeStarted < dwMonitorDuration)
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThis->get_Container(&pContainer)))
		goto ERROR_ENCOUNTERED;

	// Stop playing the sound.
	pThis->InvokeMethodExt(CComBSTR("Set3DSound"),
						   CComDISPPARAMS(2, CComVariant(""),
						   CComVariant((bool)VARIANT_FALSE)), NULL);

	// Move the interact sprite back into our inventory.
	if (FAILED(hr = pInteractSprite->InvokeMethodExt(CComBSTR("MoveInto"),
					CComDISPPARAMS(1, CComVariant((IDispatch*)pThis)),
					NULL)))
		goto ERROR_ENCOUNTERED;

	// Finished with this animation.
	if (FAILED(hr = m_pWorld->get_Tool(bstrAnimator, (IUnknown**)&pVWAnimator)) || !pVWAnimator)
		goto ERROR_ENCOUNTERED;
 	if (FAILED(hr = pThis->get_Dispatch(bstrAnim, (IDispatch**)&pAnimation)) || !pAnimation)
		goto ERROR_ENCOUNTERED;
 	if (FAILED(hr = pVWAnimator->Remove(pAnimation)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThis->put_Dispatch(bstrAnim, NULL)))
		goto ERROR_ENCOUNTERED;

    return S_OK;

ERROR_ENCOUNTERED:
	
	return hr;
}

//
// [MV] I'm intentionally writing this "new style" (with CComDISPPARAMS, etc.) to
// see what that's like. 
//
STDMETHODIMP CFoundationExemplars::ThngOnDropFile(long x, long y, BSTR bstrFilename, VARIANT_BOOL *pbHandled)
{
	// Algorithm: copy the file somewhere that it's reachable.  Then, create an HTML object in-world
	// that refers to it.

	HRESULT hr;
	CComPtr<IThing> pThis;
	CComPtr<IThing> pGlobal;
	CComPtr<IThing> pThingNew;
	CComPtr<IThing> pUser;
	CComPtr<IThing> pUserContainer;
	CComBSTR bstrPath;
	CComBSTR bstrURL;
	CString strFilename(bstrFilename);
	BOOL bDropInRoom;

	if (pbHandled==NULL)
		return E_POINTER;

	*pbHandled = VARIANT_FALSE;

	// get this
	hr = m_pWorld->get_This(&pThis);
	if (FAILED(hr))
		return hr;

	// get the user
	hr = m_pWorld->get_User(&pUser);
	if (FAILED(hr))
		return hr;

	// now, get the user's container
	hr = pUser->get_Container(&pUserContainer);
	if (FAILED(hr))
		return hr;

	// see if we're dropping in the user's container (room)
	bDropInRoom = (pThis==pUserContainer);

	// [MV] for now (3-3-98), only allow dropping in the
	// user's container.

	if (!bDropInRoom)
		return S_OK;

	// get the global pointer
	hr = m_pWorld->get_Global(&pGlobal);
	if (FAILED(hr))
		return hr;

	// call Global.UploadFile to upload the file
	
	// calculate the destination argument
	CComBSTR bstrDestname;
	char szFName[_MAX_FNAME];
	char szExt[_MAX_EXT];
	_splitpath(strFilename, NULL, NULL, szFName, szExt);
	bstrDestname = szFName;
	bstrDestname.Append(szExt);

	// prepare method arguments
	CComVariant varFilename(bstrFilename), varDestname(bstrDestname);
	CComDISPPARAMS dp(2, varFilename, varDestname);

	// invoke UploadFile
	CComVariant varResult;
	hr = pGlobal->InvokeMethodExt( CComBSTR("UploadFile"), dp, &varResult);
	if (FAILED(hr))
		return hr;

	ASSERT(varResult.vt==VT_BSTR);
	ASSERT(varResult.bstrVal!=NULL);

	// get the result string
	bstrURL = varResult.bstrVal;
	varResult.Clear();

	// REVIEW: verify that we can drop in this container

	// if we're dropping in the user's room, put the object in the user and then "drop" it
	if (bDropInRoom)
	{
		// Create the HTML object
		hr = m_pWorld->CreateInstanceExt(CComBSTR(szFName), CComVariant(CComBSTR("HTML")), pUser, &pThingNew);
		if (FAILED(hr))
			return hr;
	}
	else
	{
		// [MV] - disabled; only allow drop in room
#if 0
		// Create the HTML object
		hr = m_pWorld->CreateInstanceExt(CComBSTR(szFName), CComVariant(CComBSTR("HTML")), pThis, &pThingNew);
		if (FAILED(hr))
			return hr;
#else
		ASSERT(FALSE);
#endif
	}

	// set some graphics for it
	CString strExt = szExt;
	CString strGeomFile;

	if (strExt.CompareNoCase(".txt")==0)
		strGeomFile = "shared\\TxtDoc.gif";
	else if (strExt.CompareNoCase(".doc")==0)
		strGeomFile = "shared\\DocDoc.gif";
	else if (strExt.CompareNoCase(".xls")==0)
		strGeomFile = "shared\\XlsDoc.gif";
	else if (strExt.CompareNoCase(".htm")==0)
		strGeomFile = "shared\\HtmDoc.gif";
	else
		strGeomFile = "shared\\GenDoc.gif";

	CComVariant varGeomFile(strGeomFile);
	dp.Terminate();
	dp.Initialize(	7,
					varGeomFile,
					CComVariant(0.0f), CComVariant(1.0f), CComVariant(0.0f),
					CComVariant(0.0f), CComVariant(0.0f), CComVariant(1.0f));

	hr = pThingNew->InvokeMethodExt(CComBSTR("InitializeSpriteGraphics"), dp, &varResult);
	if (FAILED(hr))
		return hr;

	// scale it
	CComPtr<IVector> pScale;
	hr = pThingNew->get_ObjectPropertyExt(CComBSTR("scale"), IID_IVector, (IObjectProperty **) &pScale);
	if (FAILED(hr))
		return hr;

	hr = pScale->set(0.5, 0.5, 0.5);
	if (FAILED(hr))
		return hr;

	// indicate that it's not anchored (so it can be dropped)
	hr = pThingNew->put_BOOL(CComBSTR("IsTakeable"), VARIANT_FALSE);
	if (FAILED(hr))
		return hr;

	// set its URL
	hr = pThingNew->put_BOOL(CComBSTR("ShowURL"), VARIANT_TRUE);
	if (FAILED(hr))
		return hr;

	hr = pThingNew->put_String(CComBSTR("HTMLURL"), bstrURL);
	if (FAILED(hr))
		return hr;

	// set popup
	hr = pThingNew->put_BOOL(CComBSTR("PopupWindow"), VARIANT_FALSE);
	if (FAILED(hr))
		return hr;

	// if dropping in room, call pThingNew.Drop to drop it
	if (bDropInRoom)
	{
		CComVariant varNotify((IDispatch *) NULL);
		dp.Terminate();
		varResult.Clear();
		dp.Initialize(	2,
						CComVariant((IDispatch *)pThingNew),
						varNotify );
		hr = pUser->InvokeMethodExt(CComBSTR("Drop"), dp, &varResult);
		if (FAILED(hr))
			return hr;
	}

	*pbHandled = VARIANT_TRUE;

	return S_OK;
}
