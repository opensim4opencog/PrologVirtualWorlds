// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// Room.cpp : Implementation of CRoom
#include "stdafx.h"
#include <vwsysex.h>
#include <vwanim.h>
#include "vwutils.h"
#include "room.h"
#include "syshelp.h"
#include <vwevents.h>
#include "..\..\vwobject\propsecu.h"


/////////////////////////////////////////////////////////////////////////////
// CRoom

//******************************************************
// Install
// Installs this module - creates the exemplar and
// adds the properties and methods.

STDMETHODIMP CRoomExemplarObject::Install(IModule* pModule)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CMethodInfo rgMethod[] =
	{
		{ METHOD_SERVER,		"Expel",						NULL,		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER,		"TransmitWhisper",				NULL,		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,		"ReceiveWhisper",				NULL,		PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER,		"AllowEnter",					NULL,		PSBIT_EXEMPLARDEFAULTMETHOD,				PS_ALLEXECUTEMETHOD},
		{ METHOD_SERVER,		"AllowLeave",					NULL,		PSBIT_EXEMPLARDEFAULTMETHOD,				PS_ALLEXECUTEMETHOD},
		{ METHOD_SERVER,		"OnContentEnter",				NULL,		PSBIT_EVENTHANDLER /* | PSBIT_HIDDEN */,	PS_EVENTHANDLER },
		{ METHOD_SERVER,		"OnContentLeave",				NULL,		PSBIT_EVENTHANDLER /* | PSBIT_HIDDEN */,	PS_EVENTHANDLER },
		{ METHOD_SERVER,		"OnSystemCreate",				NULL,		PSBIT_SYSTEMEVENTHANDLER,	PS_EVENTHANDLER },
		{ METHOD_SERVER,		"OnSystemDestroy",				NULL,		PSBIT_SYSTEMEVENTHANDLER,	PS_EVENTHANDLER },	
		{ METHOD_CLIENT,		"OnContentSystemConnect",		NULL, 		PSBIT_SYSTEMEVENTHANDLER,	PS_EVENTHANDLER },
		{ METHOD_CLIENT,		"OnContentSystemDisconnect",	NULL, 		PSBIT_SYSTEMEVENTHANDLER,	PS_EVENTHANDLER },
		{ METHOD_CLIENT,		"OnContentSystemTell",			NULL,		PSBIT_SYSTEMEVENTHANDLER,	PS_EVENTHANDLER }
	};
	int cMethod = sizeof(rgMethod)/sizeof(rgMethod[0]);

	CComPtr<IThing> pRoomEx;
	CComPtr<IPropertyList> pList;
	CComPtr<IThing> pGlobal;
	CComPtr<IModule> pSysModule;
	CComPtr<IPropertyList> pRoomList;
	CComBSTR bstrName;
	CComBSTR bstrNameInternal;
	CComPtr<IPropertyList> pEditMenu;
	CComBSTR bstrDesc;
    int      iMethod;
    HRESULT  hr = S_OK;

	// Create the Room exemplar
	if (FAILED(hr = m_pWorld->CreateExemplar(CComBSTR("Room"), CComVariant(CComBSTR("Thing")), &pRoomEx)))
    	goto ERROR_ENCOUNTERED;

//	if (FAILED(hr = pRoomEx->put_Description(CComBSTR("This is a standard room."))))
//	    goto ERROR_ENCOUNTERED;

	// set room to be open by default
	if (FAILED(hr = pRoomEx->put_BOOL(CComBSTR("IsOpen"), VARIANT_TRUE)))
	    goto ERROR_ENCOUNTERED;

	// Add in the methods
	for (iMethod=0; iMethod<cMethod; iMethod++)
	{
		bstrName = rgMethod[iMethod].pszName;

		if (rgMethod[iMethod].pszNameInternal == NULL)
			bstrNameInternal = bstrName;
		else
			bstrNameInternal = rgMethod[iMethod].pszNameInternal;
		
		if (FAILED(hr = pRoomEx->CreateAndAddMethodExt(	rgMethod[iMethod].lFlags,
													pModule,
													bstrNameInternal,
													bstrName,
													rgMethod[iMethod].psbits,
													rgMethod[iMethod].permissions)))
    	    goto ERROR_ENCOUNTERED;
	}

	// Add in OnRooms/Portals/AvatarsAccess - calls the system module's OnWorldOwnerAccess method
	// Need to get system module first
	if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
	    goto ERROR_ENCOUNTERED;

	if (FAILED(hr = m_pWorld->get_Module(CComBSTR("System"), &pSysModule)))
	    goto ERROR_ENCOUNTERED;

	if ((pRoomEx) && (pGlobal) && (pSysModule))
	{
		if (FAILED(hr = pRoomEx->CreateAndAddMethodExt( METHOD_CLIENT | METHOD_SERVER,
													pSysModule,
													CComBSTR("OnGlobalConstantAccess"),
													CComBSTR("OnAvatarsAccess"),
													PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, 
													PS_ALLEXECUTEMETHOD )))
            goto ERROR_ENCOUNTERED;

		if (FAILED(hr = pRoomEx->CreateAndAddMethodExt( METHOD_CLIENT | METHOD_SERVER,
													pSysModule,
													CComBSTR("OnGlobalConstantAccess"),
													CComBSTR("OnPortalsAccess"),
													PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, 
													PS_ALLEXECUTEMETHOD )))
            goto ERROR_ENCOUNTERED;

		if (FAILED(hr = pGlobal->CreateAndAddMethodExt( METHOD_CLIENT | METHOD_SERVER,
													pSysModule,
													CComBSTR("OnGlobalConstantAccess"),
													CComBSTR("OnRoomsAccess"),
													PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, 
													PS_ALLEXECUTEMETHOD )))
            goto ERROR_ENCOUNTERED;
	}

	// Create the default properties - add Avatars list to Room
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
	    goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pRoomEx->AddPropertyExt(CComBSTR("Avatars"), 
										CComVariant(pList), 
										PSBIT_WORLDOWNERPROPERTY | PSBIT_INTERNALLYSET,
										PS_GLOBALSYSTEMCONSTANT, 
										VT_DISPATCH, IID_IPropertyList, NULL)))
	    goto ERROR_ENCOUNTERED;

	pList.Release();

	// Add Portals list to Room
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
	    goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pRoomEx->AddPropertyExt(CComBSTR("Portals"), 
										CComVariant(pList), 
										PSBIT_WORLDOWNERPROPERTY | PSBIT_INTERNALLYSET,
										PS_GLOBALSYSTEMCONSTANT, 
										VT_DISPATCH, IID_IPropertyList, NULL)))
	    goto ERROR_ENCOUNTERED;

	// Add the Rooms list to Global
    if (FAILED(hr = CreatePropertyList(m_pWorld, &pRoomList)))
	    goto ERROR_ENCOUNTERED;

	if (pGlobal)
	{
		if (FAILED(hr = pGlobal->AddPropertyExt(CComBSTR("Rooms"), 
										CComVariant((IDispatch *)pRoomList),
										PSBIT_WORLDOWNERPROPERTY | PSBIT_INTERNALLYSET | PSBIT_HIDDEN,
										PS_GLOBALSYSTEMCONSTANT,
										VT_DISPATCH, IID_IPropertyList, NULL)))
	    goto ERROR_ENCOUNTERED;
	}

	// Set room to allow whispers by default
	if (FAILED(hr = pRoomEx->AddPropertyExt(CComBSTR("AllowWhispers"),
										CComVariant((bool)VARIANT_TRUE),
										PSBIT_EXEMPLARDEFAULTPROPERTY,
										PS_EXEMPLARDEFAULTPROPERTY, 
										VT_BOOL, IID_NULL, NULL)))
	    goto ERROR_ENCOUNTERED;

	// EDIT MENUS...  Needed to have different description page (see bug #2974)
//	bstrDesc = "General|Edit the text description of the object|descedsm.htm";
	// removed the special general tab reference, but leaving in the property adding to
	// not screw up the adding of the other tabs

	// End user map
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pEditMenu)))
	    goto ERROR_ENCOUNTERED;
	//create the description page
//	if (FAILED(hr = pEditMenu->AddString(bstrDesc)))
//	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pRoomEx->put_Property(CComBSTR("EndUserEditMap"), CComVariant((IDispatch*) pEditMenu))))
	    goto ERROR_ENCOUNTERED;
	pEditMenu.Release();

	// Owner map
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pEditMenu)))
    	goto ERROR_ENCOUNTERED;
	//create the description page
//	if (FAILED(hr = pEditMenu->AddString(bstrDesc)))
//	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pRoomEx->put_Property(CComBSTR("OwnerEditMap"), CComVariant((IDispatch*) pEditMenu))))
	    goto ERROR_ENCOUNTERED;
	pEditMenu.Release();

	// Builder map
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pEditMenu)))
    	goto ERROR_ENCOUNTERED;
	//create the description page
//	if (FAILED(hr = pEditMenu->AddString(bstrDesc)))
//	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pRoomEx->put_Property(CComBSTR("BuilderEditMap"), CComVariant((IDispatch*) pEditMenu))))
	    goto ERROR_ENCOUNTERED;
	pEditMenu.Release();

	// Advanced map
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pEditMenu)))
    	goto ERROR_ENCOUNTERED;
	//create the description page
//	if (FAILED(hr = pEditMenu->AddString(bstrDesc)))
//	    goto ERROR_ENCOUNTERED;
	hr = pRoomEx->put_Property(CComBSTR("AdvancedEditMap"), CComVariant((IDispatch*) pEditMenu));
	pEditMenu.Release();

ERROR_ENCOUNTERED:

    if (FAILED(hr))
    	VWTRACE(m_pWorld, "VWROOM", VWT_ERROR, "CRoomExemplarObject::Install: Error encountered:%x \n", hr);

    return hr;
}


//******************************************************
// Expel
// Boots the person out of the room.  This does not ask
// any permissions - it just does it.  If a door is
// specified, the person is booted out that door.  Otherwise,
// the person is forced out of the first door found.
// If no door is found or if the target room doesn't want
// the expelee, they are sent home.

STDMETHODIMP CRoomExemplarObject::Expel(IThing * pThing, IThing * pDoor, VARIANT_BOOL * pbool)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr = S_OK;

	if ((pbool==NULL) || (pThing==NULL))
    {
        VWTRACE(m_pWorld, "VWROOM", VWT_ERROR, "CRoomExemplarObject::Expel: NULL pointer\n");
		return ReportRoomError(E_POINTER);
    }

	*pbool = VARIANT_FALSE;

	CComPtr<IThing> pThis;
	CComPtr<IThing> pOldContainer;
	CComPtr<IThing> pCurDoor;
	CComPtr<IThing> pCurDoorRoom;
	CComPtr<IPropertyList> pDoorList;
	long lCount;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

    CComVariant     varThing((IDispatch*) pThing);
    CComDISPPARAMS  dispparams(1, varThing);

	//  Try to force them out the specified door, if any
	if (pDoor)
	{
		if (SUCCEEDED(pDoor->InvokeMethodExt(CComBSTR("PrimaryTransport"), dispparams, NULL)))
			return S_OK;
	}

	// Try to force them out any door
	if (FAILED(hr = pThis->get_ObjectProperty(CComBSTR("Portals"), (IObjectProperty **)&pDoorList))|| !pDoorList)
	    return hr;

	if (FAILED(hr = pDoorList->get_Count(&lCount)))
	    return hr;

	// for every door in turn, try to force the thing out
	while (lCount--)
	{
		if (FAILED(hr = pDoorList->get_Thing(lCount, &pCurDoor)))
	        break;

		ASSERT(pCurDoor);

		// ok, try to force them out
		if (SUCCEEDED(pCurDoor->InvokeMethodExt(CComBSTR("PrimaryTransport"), dispparams, NULL)))
    		break;

		pCurDoor.Release();
	}

	return hr;
}



//******************************************************
// AllowEnter
// Used to be:Rooms do not allow anyone to enter unless 
// their container is already in the room.

STDMETHODIMP CRoomExemplarObject::AllowEnter(IThing * pThing, VARIANT_BOOL * pbool)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if ((pbool==NULL) || (pThing==NULL))
    {
        VWTRACE(m_pWorld, "VWROOM", VWT_ERROR, "CRoomExemplarObject::AllowEnter: NULL pointer\n");
		return ReportRoomError(E_POINTER);
    }

	*pbool = VARIANT_TRUE;
	return S_OK;
}


//******************************************************
// AllowLeave
// Rooms allow ANYTHING to leave.  No problem leaving a room!

STDMETHODIMP CRoomExemplarObject::AllowLeave(IThing * pThing, VARIANT_BOOL * pbool)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if ((pbool==NULL) || (pThing==NULL))
    {
        VWTRACE(m_pWorld, "VWROOM", VWT_ERROR, "CRoomExemplarObject::AllowLeave: NULL pointer\n");
		return ReportRoomError(E_POINTER);
    }

	*pbool = VARIANT_TRUE;
	return S_OK;
}

//******************************************************
// OnContentEnter
// Add the item to the appropriate list - Portals, Avatar,
// Object

STDMETHODIMP CRoomExemplarObject::OnContentEnter(IThing *pWhere, IThing *pWhat)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pWhat==NULL)
    {
        VWTRACE(m_pWorld, "VWROOM", VWT_ERROR, "CRoomExemplarObject::OnContentEnter: NULL pointer\n");
		return ReportRoomError(E_POINTER);
    }

	CComPtr<IThing>         pThis;
	CComPtr<IThing>         pGlobal;
	CComPtr<IPropertyList>  pList;
	VARIANT_BOOL            bIs;
    HRESULT                 hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
		return hr;

	// see if entering thing is an avatar
	if (FAILED(hr = pWhat->IsOfType(CComBSTR("Avatar"), &bIs)))
	    return hr;

	if (bIs == VARIANT_TRUE)
    {
		if (FAILED(hr = pThis->get_ObjectProperty(CComBSTR("Avatars"), (IObjectProperty**)&pList)))
	        return hr;
    }
	else
	{
		if (FAILED(hr = pWhat->IsOfType(CComBSTR("Portal"), &bIs)))
	        return hr;

		if (bIs == VARIANT_TRUE)
        {
			if (FAILED(hr = pThis->get_ObjectProperty(CComBSTR("Portals"), (IObjectProperty**)&pList)))
	            return hr;
        }
	}

	// add thing to appropriate list
	if (pList)
    {
		CSetUID SetUID(m_pWorld, pGlobal, pGlobal, VARIANT_TRUE);

		if (FAILED(hr = pList->AddThing(pWhat)))
	        return hr;
    }

	// need to invoke base class method
	hr = pThis->InvokeSuperMethodExt(CComDISPPARAMS(2, CComVariant((IDispatch*)pWhere), 
                                                       CComVariant((IDispatch*)pWhat)), NULL);

	return hr;
}

//******************************************************
// OnContentLeave
// Remove the object form the appropriate list - Portals,
// Avatar, Object

STDMETHODIMP CRoomExemplarObject::OnContentLeave(IThing *pWhere, IThing *pWhat)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	if (pWhat==NULL)
    {
        VWTRACE(m_pWorld, "VWROOM", VWT_ERROR, "CRoomExemplarObject::OnContentLeave: NULL pointer\n");
		return ReportRoomError(E_POINTER);
    }

	CComPtr<IThing>         pThis;
	CComPtr<IThing>         pGlobal;
	CComPtr<IPropertyList>  pList;
	VARIANT_BOOL            bIs;   
    HRESULT                 hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
		return hr;

	// see if entering thing is an avatar
	if (FAILED(hr = pWhat->IsOfType(CComBSTR("Avatar"), &bIs)))
	    return hr;

	if (bIs == VARIANT_TRUE)
    {
		if (FAILED(hr = pThis->get_ObjectProperty(CComBSTR("Avatars"), (IObjectProperty**)&pList)))
	        return hr;
    }
	else
	{
		if (FAILED(hr = pWhat->IsOfType(CComBSTR("Portal"), &bIs)))
	        return hr;

		if (bIs == VARIANT_TRUE)
        {
			if (FAILED(hr = pThis->get_ObjectProperty(CComBSTR("Portals"), (IObjectProperty**)&pList)))
	            return hr;
        }
	}

	// remove thing to appropriate list
	if (pList)
    {
		CSetUID SetUID(m_pWorld, pGlobal, pGlobal, VARIANT_TRUE);

		if (FAILED(hr = pList->RemoveThing(pWhat)))
	        return hr;
    }

	// need to invoke base class method
	hr = pThis->InvokeSuperMethodExt(CComDISPPARAMS(2, CComVariant((IDispatch*)pWhere), 
                                                       CComVariant((IDispatch*)pWhat)), NULL);

    return hr;
}


STDMETHODIMP CRoomExemplarObject::OnSystemCreate(IThing *pThing)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pThing==NULL)
    {
        VWTRACE(m_pWorld, "VWROOM", VWT_ERROR, "CRoomExemplarObject::OnSystemCreate: NULL pointer\n");
		return ReportRoomError(E_POINTER);
    }

	CComPtr<IThing>         pThis;
	CComPtr<IThing>         pGlobal;
	CComPtr<IPropertyList>  pRoomList;
	CComPtr<IThing>			pUser;
	CComPtr<IThing>			pUserRoom;
	CComBSTR                bstrRoomName;
	VARIANT_BOOL	        bIsExemplar;
	static CComBSTR	        bstrIsExemplar("IsExemplar");
	HRESULT                 hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	if (FAILED(hr = pThis->get_BOOL(bstrIsExemplar, &bIsExemplar)) || bIsExemplar)
		return hr;
	
	if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
		return hr;

	if (FAILED(hr = pThing->get_Name(&bstrRoomName.m_str)))
		return hr;

	// REVIEW: do we have to worry about Rooms list not being present
	if (FAILED(hr = pGlobal->get_ObjectProperty(CComBSTR("Rooms"), (IObjectProperty **) &pRoomList)) || !pRoomList)
		return hr;

	{
		CSetUID SetUID(m_pWorld, pGlobal, pGlobal, VARIANT_TRUE);

		if (FAILED(hr = pRoomList->AddThing(pThing)))
			return hr;
	}

	// Invoke client size method to fire UI event "OnCreateRoom"
	if (pGlobal)
	{
		if (FAILED(hr = pGlobal->InvokeMethodExt(CComBSTR("OnCreateRoomClient"), CComDISPPARAMS(1, CComVariant((IDispatch *) pThing), NULL), NULL)))
			return hr;
	}

	return hr;
}

STDMETHODIMP CRoomExemplarObject::OnSystemDestroy(IThing *pThing)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pThing==NULL)
    {
        VWTRACE(m_pWorld, "VWROOM", VWT_ERROR, "CRoomExemplarObject::OnSystemDestroy: NULL pointer\n");
		return ReportRoomError(E_POINTER);
    }

	CComPtr<IThing>         pThis;
	CComPtr<IThing>         pGlobal;
	CComPtr<IPropertyList>  pRoomList;
	CComBSTR                bstrRoomName;
	VARIANT_BOOL	        bIsExemplar;
	static CComBSTR	        bstrIsExemplar("IsExemplar");
	HRESULT                 hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	if (FAILED(hr = pThis->get_BOOL(bstrIsExemplar, &bIsExemplar)) || bIsExemplar)
		return hr;
	
	if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
		return hr;
	if (FAILED(hr = pThing->get_Name(&bstrRoomName.m_str)))
		return hr;

	// REVIEW: do we have to worry about Rooms list not being present
	if (FAILED(hr = pGlobal->get_ObjectProperty(CComBSTR("Rooms"), (IObjectProperty **) &pRoomList))|| !pRoomList)
		return hr;

	{
		CSetUID SetUID(m_pWorld, pGlobal, pGlobal, VARIANT_TRUE);
	
		if (FAILED(hr = pRoomList->RemoveThing(pThing)))
			return hr;
	}

	return hr;
}


//******************************************************
// TransmitWhisper
// Transmits a whisper from the caller to a list of avatars
// 1.  convert the varNameList to a collection of IThings
// 2.  Removing illegal recipients
// 3.  Generate list of target rooms by checking containers
// 4.  Remove illegal rooms (and their targets)
// 5.  Generate final formatted text message with names of all parties
// 6.  Invoke various receivewhisper commands
#include "profhelp.h"

STDMETHODIMP CRoomExemplarObject::TransmitWhisper(BSTR bstrMsg, VARIANT varNameListIn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT			hr = S_OK;
	IPropertyList	*pTargetStrList = NULL;
	IPropertyList	*pTargetPtrList = NULL;
	IPropertyList	*pRoomList		= NULL;
	long			lNumTargets;
	BOOL			bNeedConversion = TRUE;
	VARIANT_BOOL	bAllowsWhisper;
	IThing			*pCaller = NULL;
	IThing			*pCont			= NULL;
	CComBSTR		bstrFinalMsg;
    CComVariant     varNameList;
	static CComBSTR	kbstrAllowWhispers("AllowWhispers");
	static CComBSTR	kbstrPropProfile("Profile");

	hr = m_pWorld->get_Caller(&pCaller);
	if (FAILED(hr) || (!pCaller))
		goto CLEAN_UP;
	
	hr = pCaller->get_Container(&pCont);
	if (FAILED(hr))
		goto CLEAN_UP;
	else if (pCont)
	{
		hr = pCont->get_BOOL(kbstrAllowWhispers, &bAllowsWhisper);
		if (SUCCEEDED(hr) && (bAllowsWhisper == VARIANT_FALSE))
			goto CLEAN_UP;
	}

	// 1.  convert the varNameList to a collection of IThings
	hr = CreatePropertyList(m_pWorld, &pTargetPtrList);
	if (FAILED(hr))
		goto CLEAN_UP;
	
	hr = CreatePropertyList(m_pWorld, &pTargetStrList);
	if (FAILED(hr))
		goto CLEAN_UP;
	
	hr = CreatePropertyList(m_pWorld, &pRoomList);
	if (FAILED(hr))
		goto CLEAN_UP;
	
	// check for variant to a variant
	// if so, need to crack it and continue...
    if (FAILED(hr = VariantCopyInd(&varNameList, &varNameListIn)))
    {
    	VWTRACE(m_pWorld, "VWROOM", VWT_ERROR, "CRoomExemplarObject::TransmitWhisper: Error copying input VARIANT\n");
        goto CLEAN_UP;
    }

	switch (varNameList.vt)
	{
	case VT_SAFEARRAY:
		hr = ConvertSAFEARRAYToPropertyList(&varNameList, pTargetStrList);
		if (FAILED(hr))
        {
            VWTRACE(m_pWorld, "VWROOM", VWT_ERROR, "CRoomExemplarObject::OnSystemDestroy: Cannot convert SAFEARRAY\n");
			goto CLEAN_UP;
        }
		break;
	
	case VT_BSTR:
		hr = pTargetStrList->AddString(varNameList.bstrVal);
		if (FAILED(hr))
			goto CLEAN_UP;
		break;

	case VT_DISPATCH:
		if (varNameList.pdispVal)
		{
			// could be either an IThing or an IPropertyList...
			IPropertyList *	ptProp	= NULL;
			IThing *		ptThing	= NULL;

			if (SUCCEEDED(varNameList.pdispVal->QueryInterface(IID_IThing, (LPVOID*)&ptThing)))
			{
				// we have a thing - add it directly
				hr = pTargetPtrList->AddThing((IThing *)varNameList.pdispVal);
				if (FAILED(hr))
					goto CLEAN_UP;
				bNeedConversion = FALSE;
				SAFERELEASE(ptThing);
			}
			else if (SUCCEEDED(varNameList.pdispVal->QueryInterface(IID_IPropertyList, (LPVOID*)&ptProp)))
			{
				// we have a list - copy it
				CComVariant	testVar;

				hr = ptProp->get_Property(0, &testVar);
				if (SUCCEEDED(hr))
				{
					if (testVar.vt == VT_DISPATCH)
					{
						ptProp->CopyTo(pTargetPtrList);
						bNeedConversion = FALSE;
					}
					else if (testVar.vt == VT_BSTR)
						ptProp->CopyTo(pTargetStrList);
				}

				SAFERELEASE(ptProp);
			}
		}
		break;

	default:
		// invalid args!!
        VWTRACE(m_pWorld, "VWROOM", VWT_ERROR, "CRoomExemplarObject::OnSystemDestroy: Invalid arguments\n");
		hr = DISP_E_BADVARTYPE; // ??? is this the correct error???
		goto CLEAN_UP;
		break;
	}

	// now convert the list to a list of avatars
	if (bNeedConversion)
	{
		BSTR	bstrCurName;
		IThing	*pCurThing;
		
		hr = pTargetStrList->get_Count(&lNumTargets);
		if (FAILED(hr))
			goto CLEAN_UP;
		
		for (long i = 0; i < lNumTargets; i++)
		{
			hr = pTargetStrList->get_String(i, &bstrCurName);
			if (SUCCEEDED(hr))
			{
				hr = m_pWorld->get_Avatar(bstrCurName, &pCurThing);
				if (SUCCEEDED(hr) && pCurThing)
				{
					hr = pTargetPtrList->AddThing(pCurThing);
					SAFERELEASE(pCurThing);
				}
			}
		}

	}

	// 2.  Removing illegal recipients
	hr = pTargetPtrList->get_Count(&lNumTargets);
	if (FAILED(hr) || (lNumTargets == 0))
		goto CLEAN_UP;
	
	{
		IThing			*pCurThing;
		IAvatarProfile	*pProfile	= NULL;
		static CComBSTR	kbstrIgnore("Ignoring");

		for (long i = lNumTargets-1; i >= 0; i--)
		{
			hr = pTargetPtrList->get_Thing(i, &pCurThing);
			if (SUCCEEDED(hr) && pCurThing)
			{
				hr = pCurThing->get_ObjectProperty(kbstrPropProfile, (IObjectProperty **)&pProfile);
				if (SUCCEEDED(hr) && pProfile)
				{
					if (_IsObjInProfileKeyList(pProfile, kbstrIgnore.m_str, pCaller))
						hr = pTargetPtrList->RemoveAt(i);
					SAFERELEASE(pProfile);
				}
	
				SAFERELEASE(pCurThing);
			}
		}
	}

	// 3.  Generate list of target rooms by checking containers
	// 4.  Remove illegal rooms (and their targets)
	hr = pTargetPtrList->get_Count(&lNumTargets);
	if (FAILED(hr) || (lNumTargets == 0))
		goto CLEAN_UP;
	
	{
		IThing	*pContainer;
		IThing	*pCurThing;

		for (long i = lNumTargets - 1; i >= 0; i--)
		{
			hr = pTargetPtrList->get_Thing(i, &pCurThing);
			if (SUCCEEDED(hr) && pCurThing)
			{
				hr = pCurThing->get_Container(&pContainer);
				if (SUCCEEDED(hr) && pContainer)
				{
					hr = pContainer->get_BOOL(kbstrAllowWhispers, &bAllowsWhisper);
					if (FAILED(hr) || (bAllowsWhisper == VARIANT_TRUE))
					{
						// Add this room if it isn't there already
						VARIANT	varRoom;
						long		lCurLoc;

						::VariantInit(&varRoom);

						varRoom.vt = VT_DISPATCH;
						varRoom.pdispVal = pContainer;

						hr = pRoomList->Find(varRoom, &lCurLoc);
						if (FAILED(hr) || (lCurLoc == -1))
						{
							hr = pRoomList->AddThing(pContainer);
						}
					}
					else
					{
						// remove that object - it is in a blocked room
						hr = pTargetPtrList->RemoveAt(i);
					}
					SAFERELEASE(pContainer);
				}
				SAFERELEASE(pCurThing);
			}
		}
	}

	// 5.  Generate final formatted text message with names of all parties
	hr = pTargetPtrList->get_Count(&lNumTargets);
	if (FAILED(hr) || (lNumTargets == 0))
		goto CLEAN_UP;

	{
		// REVIEW:  CString use inefficient
		CComBSTR	bstrName;
		CString		strName;
		CString		strMsg;
		IThing		*pCurThing;

		pCaller->get_Name(&bstrName.m_str);
		strName = bstrName;
		strMsg = strName + " whispers to ";

		for (long i = 0; i < lNumTargets; i++)
		{
			pTargetPtrList->get_Thing(i, &pCurThing);
			pCurThing->get_Name(&bstrName.m_str);
			strName = bstrName;
			strMsg = strMsg + strName;
			if (i < (lNumTargets - 2))
				strMsg = strMsg + ", ";
			else if (i == (lNumTargets - 2))
				strMsg = strMsg + " and ";
			SAFERELEASE(pCurThing);
		}
		strMsg = strMsg + ", \"" + CString(bstrMsg) + "\"";
		bstrFinalMsg = strMsg;
	}

	// 6.  Invoke various receivewhisper commands
	hr = pRoomList->get_Count(&lNumTargets);
	if (FAILED(hr) || (lNumTargets == 0))
		goto CLEAN_UP;

	// add the caller so they get a compy of the message
	{
		long	lRoomPos;
		VARIANT	varCallerRoom;

		::VariantInit(&varCallerRoom);

		hr = pTargetPtrList->AddThing(pCaller);
		if (FAILED(hr))
			goto CLEAN_UP;

		varCallerRoom.vt = VT_DISPATCH;
		varCallerRoom.pdispVal = pCont;

		hr = pRoomList->Find(varCallerRoom, &lRoomPos);
		if (SUCCEEDED(hr) && (lRoomPos == -1))
		{
			pRoomList->AddThing(pCont);
			lNumTargets++;
		}
	}

	{
		IThing		*pCurRoom;
		IThing		*pCurObj;
		IThing		*pCurCont;
		long		lAvCount, j;
		static	CComBSTR	kbstrReceiveWhisper = "ReceiveWhisper";
		
		pTargetPtrList->get_Count(&lAvCount);

		for (long i = 0; i < lNumTargets; i++)
		{
			pTargetStrList->RemoveAll();	// we will reuse this list for now
			pRoomList->get_Thing(i, &pCurRoom);

			// get the list of avatars in this room
			pTargetPtrList->get_Count(&lAvCount);
			for (j = lAvCount-1; j >= 0; j--)
			{
				pTargetPtrList->get_Thing(j, &pCurObj);
				pCurObj->get_Container(&pCurCont);
				if (pCurCont == pCurRoom)
				{
					// add it to this room list
					pTargetStrList->AddThing(pCurObj);
					pTargetPtrList->RemoveAt(j);
				}
				SAFERELEASE(pCurCont);
				SAFERELEASE(pCurObj);
			}
			
    		hr = pCurRoom->InvokeMethodExt(kbstrReceiveWhisper, 
                                           CComDISPPARAMS(2, CComVariant(bstrFinalMsg.m_str), 
                                                             CComVariant((IDispatch*) pTargetStrList)), 
                                           NULL);
		}
	}

CLEAN_UP:
	SAFERELEASE(pCont);
	SAFERELEASE(pTargetStrList);
	SAFERELEASE(pTargetPtrList);
	SAFERELEASE(pRoomList);

    return ReportRoomError(hr);
}


//******************************************************
// ReceiveWhisper
// Receives a whisper from a transmit whisper command

STDMETHODIMP CRoomExemplarObject::ReceiveWhisper(BSTR bstrMsg, IPropertyList *pTargetList)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT			hr = S_OK;
	CComPtr<IThing>	pUser;

    if (pTargetList == NULL)
    {
        VWTRACE(m_pWorld, "VWROOM", VWT_ERROR, "CRoomExemplarObject::ReceiveWhisper: NULL pointer\n");
		return ReportRoomError(E_POINTER);
    }

	hr = m_pWorld->get_User(&pUser);
	if (SUCCEEDED(hr))
	{
		long			lCurLoc;

		hr = pTargetList->Find(CComVariant((IDispatch*) pUser), &lCurLoc);
		if (SUCCEEDED(hr) && (lCurLoc != -1))
			m_pWorld->Report(bstrMsg, VWR_PAGE);
	}

	return hr;
}

STDMETHODIMP CRoomExemplarObject::OnContentSystemConnect(IThing* pWho)  
{ 
	// This is a dummy method, but needed by VWCLIOCX
	return S_OK; 
};

STDMETHODIMP CRoomExemplarObject::OnContentSystemDisconnect(IThing* pWho)
{ 
	// This is a dummy method, but needed by VWCLIOCX
	return S_OK; 
};

//******************************************************
// OnContentSystemTell
// Default does nothing

STDMETHODIMP CRoomExemplarObject::OnContentSystemTell(IThing* pfrom, IPropertyList* pto, BSTR bstr, long lval, BSTR *pbstrOut)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if ((pbstrOut==NULL) || (pfrom==NULL))
    {
        VWTRACE(m_pWorld, "VWROOM", VWT_ERROR, "CRoomExemplarObject::OnContentSystemTell: NULL pointer\n");
		return ReportRoomError(E_POINTER);
    }

	*pbstrOut = NULL;

#ifdef _DEBUG

	CString strSay;
	CComBSTR bstrName("Something");
	CString	strText(bstr);
	CString	strWho;

	// get user name
	if (SUCCEEDED(pfrom->get_Name(&bstrName.m_str)))
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

	VWTRACE(m_pWorld, "VWMODULE", VWT_VERBOSE, "CRoomExemplarObject::OnContentSystemTell: %s %s, \"%s\"\n", strWho, strSay, strText);
#endif

	return S_OK;
}
