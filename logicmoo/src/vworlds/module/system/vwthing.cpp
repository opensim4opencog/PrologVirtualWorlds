// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWThing.cpp : Implementation of CVWThing
#include "stdafx.h"
#include <vwsysex.h>
#include "vwutils.h"
#include "vwthing.h"
#include "syshelp.h"
#include "vwevents.h"

//#define REPORT_PERF
#define ALLOWSECURITYOVERRIDE

//******************************************************
// Local helper functions...

HRESULT _FireWhisperEvent(IWorld *pWorld, IThing *pTarget, IPropertyList *pTalkList, BSTR bstrText);
HRESULT _FireWhisperEventHelper(IWorld *pWorld, IThing *pTarget, IPropertyList *pHearList, DISPPARAMS *pDispParams);
HRESULT	_FireTellEvent(IThing *pSpeaker, IPropertyList *pTargetList, BSTR bstrText, long talkType);
float	CastToFloat(VARIANT var);

typedef CVWComPtr<IMenuItem, NULL, &IID_IMenuItem> CMenuItemPtr;
typedef CVWComPtr<IPropertyList, NULL, &IID_IPropertyList> CPropListPtr;


//******************************************************
// Install
// Installs this module - creates the exemplar and
// adds the properties and methods.

STDMETHODIMP CThingExemplarObject::Install(IModule* pModule)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CMethodInfo rgMethod[] =
	{
		{ METHOD_SERVER,	"AcceptEnter",					NULL, PSBIT_EXEMPLARDEFAULTMETHOD,						PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER,	"AcceptLeave",					NULL, PSBIT_EXEMPLARDEFAULTMETHOD,						PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER,	"AllowEnter",					NULL, PSBIT_EXEMPLARDEFAULTMETHOD,						PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER,	"AllowLeave",					NULL, PSBIT_EXEMPLARDEFAULTMETHOD,						PS_ALLEXECUTEMETHOD },
//		{ METHOD_SERVER,	"Destroy", 						NULL, PSBIT_EXEMPLARDEFAULTMETHOD,						PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER,	"Emote", 						NULL, PSBIT_EXEMPLARDEFAULTMETHOD,						PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"GestureText", 	NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_CLIENT,	"LookAt", 						NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER,	"MoveInto", 					NULL, PSBIT_EXEMPLARDEFAULTMETHOD,						PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER | METHOD_SYNC,	"MoveIntoSync", 	"MoveInto", PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD},
		{ METHOD_SERVER,	"Say",							NULL, PSBIT_EXEMPLARDEFAULTMETHOD,						PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER,	"Shout",						NULL, PSBIT_EXEMPLARDEFAULTMETHOD,						PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER,	"SayGesture",					NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_CLIENT,	"GetMenuList",					NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"GetMenuItems",					NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"GetPeerMenuItems",				NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"GetContentMenuItems",			NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"GetContainerMenuItems",		NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"GetSelfMenuItems",				NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"GetInventoryMenuItems",		NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"GetEditMenuItems",				NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"GetMenuItemsFromProperty",		NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"UpdateIsArtifact",				NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"UpdateIsAvatar",				NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"UpdateIsAvatarAndNotWizard",	NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"UpdateIsArtifactOrAvatar",		NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"UpdateIsTakeable",				NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"UpdateMenuItemName",			NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"UpdatePropertiesMenuItem",		NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"UpdateContentsMenu",			NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"UpdateRoomAvatarsMenu",		NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"UpdateInventoryMenu",			NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"UpdatePropertyListMenu",		NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER,	"Whisper",						NULL, PSBIT_EXEMPLARDEFAULTMETHOD,						PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_CLIENT,	"DoGesture",					NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_CLIENT,	"DoNamedGesture",				NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_CLIENT,	"DoGestureClient",				NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER,	"DoGestureServer",				NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER,	"Announce",						NULL, PSBIT_EXEMPLARDEFAULTMETHOD,						PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER,	"Report",						NULL, PSBIT_EXEMPLARDEFAULTMETHOD,						PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER,	"Take",							NULL, PSBIT_EXEMPLARDEFAULTMETHOD,						PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER,	"Drop",							NULL, PSBIT_EXEMPLARDEFAULTMETHOD,						PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_CLIENT,	"EditObject",					NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"GetEditTabMap",				NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"GetSortedEditTabMap",			NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"AddClientProperty",			NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_CLIENT,	"RemoveClientProperty",			NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_CLIENT,	"OnContentSystemEnter",			NULL, PSBIT_SYSTEMEVENTHANDLER,						PS_EVENTHANDLER },
		{ METHOD_CLIENT,	"OnContentSystemLeave",			NULL, PSBIT_SYSTEMEVENTHANDLER,						PS_EVENTHANDLER },
		{ METHOD_CLIENT,	"OnSystemEnter",				"OnContentSystemEnter", PSBIT_SYSTEMEVENTHANDLER,		PS_EVENTHANDLER },
		{ METHOD_CLIENT,	"OnSystemLeave",				"OnContentSystemLeave", PSBIT_SYSTEMEVENTHANDLER,		PS_EVENTHANDLER },
		{ METHOD_CLIENT | METHOD_SERVER, "OnIsNoticeableAccess", NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD},
		{ METHOD_CLIENT | METHOD_SERVER, "OnNameAccess",	NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_ALLEXECUTEMETHOD},
		{ METHOD_CLIENT | METHOD_SERVER, "OnDescriptionAccess",	"OnOwnerOrWizardAccess", PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER, "OnIsTakeableAccess",	"OnOwnerOrWizardAccess", PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER, "OnIsCloneableAccess",	"OnOwnerOrWizardAccess", PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER, "OnCanChangeContainerAccess",	"OnOwnerOrWizardAccess", PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"OnSystemDestroyClient",		NULL,	PSBIT_SYSTEMEVENTHANDLER,						PS_EVENTHANDLER },
		{ METHOD_SERVER, "OnIsOpenChanged",		NULL,		PSBIT_EVENTHANDLER,							PS_ALLEXECUTEMETHOD }
	};
	int cMethod = sizeof(rgMethod)/sizeof(rgMethod[0]);

	//  OnAccess methods for GlobalConstant properties - call the system module's OnGlobalConstantAccess method
	CMethodInfo woMethod[] =
	{
		{ METHOD_CLIENT | METHOD_SERVER, "OnGUIDAccess",		"OnWorldOwnerAccess",	PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER, "OnIsExemplarAccess",	"OnWorldOwnerAccess",	PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER, "OnContainerAccess",	"OnGlobalConstantAccess", PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER, "OnOwnerAccess",		"OnGlobalConstantAccess", PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER, "OnContentsAccess",	"OnGlobalConstantAccess", PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER, "OnCurrentIDAccess",	"OnGlobalConstantAccess", PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER, "OnGUIDStaticAccess",	"OnGlobalConstantAccess", PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER, "OnIsCompleteAccess",	"OnGlobalConstantAccess", PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD }
	};
	int woLength = sizeof(woMethod)/sizeof(CMethodInfo);

	CComPtr<IThing> pThingEx;
	CComPtr<IPropertyList> pMenu;
	CComPtr<IMenuItem> pMenuItem;
	CComPtr<IPropertyList> pEditMenu;
	CComPtr<IModule> pSysModule;
	CComBSTR bstrName;
	CComBSTR bstrNameInternal;
	CComBSTR bstrDesc;
//	CComBSTR bstrMethods;
	CComVariant var;
    int         iMethod;
    HRESULT     hr = S_OK;
	

	// Create the Thing exemplar
	if (FAILED(hr = m_pWorld->CreateRootExemplar(CComBSTR("Thing"), &pThingEx)))
        goto ERROR_ENCOUNTERED;

//	if (FAILED(hr = pThingEx->put_Description(CComBSTR("This is a thing."))))
//        goto ERROR_ENCOUNTERED;

	// add in the methods
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


	// Now, add in the OnXXXAccess methods that reference OnGlobalConstantAccess in the system module
	// First, get system module
	if (FAILED(hr = m_pWorld->get_Module(CComBSTR("System"), &pSysModule)))
	    goto ERROR_ENCOUNTERED;

	for (iMethod=0; iMethod<woLength; iMethod++)
	{
		bstrName = woMethod[iMethod].pszName;

		if (woMethod[iMethod].pszNameInternal == NULL)
			bstrNameInternal = bstrName;
		else
			bstrNameInternal = woMethod[iMethod].pszNameInternal;
		
		if (FAILED(hr = pThingEx->CreateAndAddMethodExt(woMethod[iMethod].lFlags,
													pSysModule,
													bstrNameInternal,
													bstrName,
													woMethod[iMethod].psbits,
													woMethod[iMethod].permissions)))
    	    goto ERROR_ENCOUNTERED;
	}

	// Properties
	
	// CanChangeContainer used to check whether an object can be moved into another container
	if (FAILED(hr = pThingEx->AddPropertyExt(CComBSTR("CanChangeContainer"), CComVariant((bool)VARIANT_TRUE),
				PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY, PS_ALLACCESSPROPERTY, VT_BOOL, IID_NULL, NULL)))
        goto ERROR_ENCOUNTERED;

	// IsTakeable used to check whether an object can be taken (affects whether Take appears in UI)
	if (FAILED(hr = pThingEx->AddPropertyExt(CComBSTR("IsTakeable"), CComVariant((bool)VARIANT_FALSE),
				PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY, PS_ALLACCESSPROPERTY, VT_BOOL, IID_NULL, NULL)))
        goto ERROR_ENCOUNTERED;

	// IsUIVisible used to determine whether thing appears in room inventory list of UI
	if (FAILED(hr = pThingEx->AddPropertyExt(CComBSTR("IsUIVisible"), CComVariant((bool)VARIANT_TRUE), 
				PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_FRIENDLY, PS_EXEMPLARDEFAULTPROPERTY, VT_BOOL, IID_NULL, NULL)))
        goto ERROR_ENCOUNTERED;

	// Edit maps

	// End user map
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pEditMenu)))
        goto ERROR_ENCOUNTERED;

	bstrDesc = "General|Edit the text description of the object|descedit.htm";
//	bstrMethods = "Methods|Edit the script methods of the object|method.htm";
	
    //create the description page
	if (FAILED(hr = pEditMenu->AddString(bstrDesc)))
        goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(CComBSTR("EndUserEditMap"), CComVariant((IDispatch*) pEditMenu),
				PSBIT_INTERNALLYADDED | PSBIT_CONSTANT, PS_GLOBALSYSTEMCONSTANT, VT_DISPATCH, IID_IPropertyList, NULL)))
        goto ERROR_ENCOUNTERED;

	pEditMenu.Release();

	// Owner map
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pEditMenu)))
        goto ERROR_ENCOUNTERED;

	//create the description page
	if (FAILED(hr = pEditMenu->AddString(bstrDesc)))
        goto ERROR_ENCOUNTERED;

	// create the method tab
//	if (FAILED(hr = pEditMenu->AddString(bstrMethods)))
//        goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(CComBSTR("OwnerEditMap"), CComVariant((IDispatch*) pEditMenu),
				PSBIT_INTERNALLYADDED | PSBIT_CONSTANT, PS_GLOBALSYSTEMCONSTANT, VT_DISPATCH, IID_IPropertyList, NULL)))
        goto ERROR_ENCOUNTERED;

	pEditMenu.Release();


//		CComBSTR bstrAdvProps("Advanced Properties|Advanced Editing of the properties of the object|advprop.htm");
//		CComBSTR bstrAdvMeths("Advanced Methods|Advanced Editing of the methods of the object|advmeth.htm");

    // Builder map
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pEditMenu)))
        goto ERROR_ENCOUNTERED;

	//create the description page
	if (FAILED(hr = pEditMenu->AddString(bstrDesc)))
        goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(CComBSTR("BuilderEditMap"), CComVariant((IDispatch*) pEditMenu),
				PSBIT_INTERNALLYADDED | PSBIT_CONSTANT, PS_GLOBALSYSTEMCONSTANT, VT_DISPATCH, IID_IPropertyList, NULL)))
        goto ERROR_ENCOUNTERED;

	pEditMenu.Release();

	// Advanced map
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pEditMenu)))
        goto ERROR_ENCOUNTERED;

	//create the description page
	if (FAILED(hr = pEditMenu->AddString(bstrDesc)))
        goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(CComBSTR("AdvancedEditMap"), CComVariant((IDispatch*) pEditMenu),
				PSBIT_INTERNALLYADDED | PSBIT_CONSTANT, PS_GLOBALSYSTEMCONSTANT, VT_DISPATCH, IID_IPropertyList, NULL)))
        goto ERROR_ENCOUNTERED;

	pEditMenu.Release();

	// Menu support

	// Peer menu
	if (FAILED(hr = CreateMenu(m_pWorld, &pMenu)))
        goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(CComBSTR("PeerMenu"), CComVariant((IDispatch*) pMenu),
				PSBIT_INTERNALLYADDED, PS_EXEMPLARDEFAULTCOLLECTION, VT_DISPATCH, IID_IPropertyList, CComBSTR("Menu"))))
        goto ERROR_ENCOUNTERED;

	pMenu.Release();

	// Container menu
	if (FAILED(hr = CreateMenu(m_pWorld, &pMenu)))
        goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(CComBSTR("ContainerMenu"), CComVariant((IDispatch*) pMenu),
				PSBIT_INTERNALLYADDED, PS_EXEMPLARDEFAULTCOLLECTION, VT_DISPATCH, IID_IPropertyList, CComBSTR("Menu"))))
        goto ERROR_ENCOUNTERED;

	pMenu.Release();

	// Content menu
	if (FAILED(hr = CreateMenu(m_pWorld, &pMenu)))
        goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(CComBSTR("ContentMenu"), CComVariant((IDispatch*) pMenu),
				PSBIT_INTERNALLYADDED, PS_EXEMPLARDEFAULTCOLLECTION, VT_DISPATCH, IID_IPropertyList, CComBSTR("Menu"))))
        goto ERROR_ENCOUNTERED;

	pMenu.Release();

	// Self menu
	if (FAILED(hr = CreateMenu(m_pWorld, &pMenu)))
        goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(CComBSTR("SelfMenu"), CComVariant((IDispatch*) pMenu),
				PSBIT_INTERNALLYADDED, PS_EXEMPLARDEFAULTCOLLECTION, VT_DISPATCH, IID_IPropertyList, CComBSTR("Menu"))))
        goto ERROR_ENCOUNTERED;

	pMenu.Release();

	// Inventory menu
	if (FAILED(hr = CreateMenu(m_pWorld, &pMenu)))
        goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pThingEx->AddPropertyExt(CComBSTR("InventoryMenu"), CComVariant((IDispatch*) pMenu),
				PSBIT_INTERNALLYADDED, PS_EXEMPLARDEFAULTCOLLECTION, VT_DISPATCH, IID_IPropertyList, CComBSTR("Menu"))))
        goto ERROR_ENCOUNTERED;

	pMenu.Release();														 

	// Plain old "Menu"
	if (FAILED(hr = CreateMenu(m_pWorld, &pMenu)))
        goto ERROR_ENCOUNTERED;

	if (FAILED(hr = CreateMenuItemExt(m_pWorld, CComBSTR("More about "), 0, pThingEx, CComBSTR("UpdateMenuItemName"), CComBSTR("LookAt"), NULL, &pMenuItem)))
        goto ERROR_ENCOUNTERED;

//		pMenuItem->put_SortOrder(MI_SORT_INFO);
	if (FAILED(hr = pMenu->AddObjectProperty(pMenuItem)))
        goto ERROR_ENCOUNTERED;

	pMenuItem.Release();

	if (FAILED(hr = pThingEx->AddPropertyExt(CComBSTR("Menu"), CComVariant((IDispatch*) pMenu),
				PSBIT_INTERNALLYADDED, PS_EXEMPLARDEFAULTCOLLECTION, VT_DISPATCH, IID_IPropertyList, CComBSTR("Menu"))))
        goto ERROR_ENCOUNTERED;

	pMenu.Release();														 

	// REVIEW: What's the difference between "Self Menu" and "Menu"?  Do we need
	// all these menus?
	if (FAILED(hr = CreateMenu(m_pWorld, &pMenu)))
        goto ERROR_ENCOUNTERED;

	if (FAILED(hr = CreateMenuItemExt(m_pWorld, CComBSTR("Properties..."), 0, pThingEx, CComBSTR("UpdatePropertiesMenuItem"), CComBSTR("EditObject"), NULL, &pMenuItem)))
        goto ERROR_ENCOUNTERED;

//		pMenuItem->put_SortOrder(MI_SORT_EDIT);
	if (FAILED(hr = pMenu->AddObjectProperty(pMenuItem)))
        goto ERROR_ENCOUNTERED;

	pMenuItem.Release();

	if (FAILED(hr = pThingEx->AddPropertyExt(CComBSTR("EditMenu"), CComVariant((IDispatch*) pMenu),
				PSBIT_INTERNALLYADDED, PS_EXEMPLARDEFAULTCOLLECTION, VT_DISPATCH, IID_IPropertyList, CComBSTR("Menu"))))
        goto ERROR_ENCOUNTERED;

    return S_OK;

ERROR_ENCOUNTERED:

	VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::Install: Error encountered %x\n", hr);
	return hr;
}


//******************************************************
// _FireWhisperEvent
// This is an internal helper function for firing the whisper
// event.  

HRESULT  _FireWhisperEvent(IWorld *pWorld, IThing *pTarget, IPropertyList *pTalkList, BSTR bstrText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (bstrText==NULL || pTarget==NULL)
    {
		VWTRACE(pWorld, "COREEXEMP", VWT_ERROR, "_FireWhisperEvent: NULL pointer(s)\n");
		return E_POINTER;
    }

	// REVIEW: we use a "Tell" event here?
	static CComBSTR	bstrSystemTellEventName("SystemTell");
	static CComBSTR	bstrTellEventName("Tell");
	VARIANT_BOOL	bShouldExecute	= VARIANT_TRUE;
	VARIANT_BOOL	bShouldRemote	= VARIANT_FALSE;
	VARIANT_BOOL	bIsStub;
	long			lSpeakRange		= 0;
    HRESULT         hr = S_OK;

	// package up args into DISPPARAMS struct
	CComVariant     varTarget((IDispatch*) pTarget);
	CComVariant     varTalkList((IDispatch*) pTalkList);
	CComVariant     varText(bstrText);
	CComVariant     varWhisper(tcWhisper);
	CComDISPPARAMS	dispparams(4, varTarget, varTalkList, varText, varWhisper);

	if (FAILED(hr = pTarget->get_IsStub(&bIsStub)))
        return hr;

	if (bIsStub == VARIANT_TRUE)
	{
		VWTRACE(pWorld, "COREEXEMP", VWT_IMPORTANT, "_FireWhisperEvent: stub object, shouldn't be accessing\n");
        return VWOBJECT_E_STUBOBJECT;
	}
    else
    {

#ifdef _DEBUG
	    VWTRACE(pWorld, "COREEXEMP", VWT_METHOD, "_FireWhisperEvent\n");
#endif

	    pWorld->get_ShouldExecuteSync(&bShouldExecute);

	    // using this call for the proper semantics, not that FireEvent is a sync command
	    pWorld->get_ShouldRemoteSync(&bShouldRemote);

	    if (bShouldExecute == VARIANT_TRUE)
        {
		    if (FAILED(hr = _FireWhisperEventHelper(pWorld, pTarget, pTalkList, dispparams)))
                return hr;
        }

	    if (bShouldRemote == VARIANT_TRUE)
	    {
		    CComPtr<IPropertyList> pArgList;

		    if (FAILED(hr = CreatePropertyList(pWorld, &pArgList)))
                return hr;

		    if (FAILED(hr = ConvertDISPPARAMSToPropertyList(dispparams, pArgList)))
            {
        		VWTRACE(pWorld, "COREEXEMP", VWT_IMPORTANT, "_FireWhisperEvent: unable to convert input DISPPARAMS to propertyList\n");
                return hr;
            }

		    // package up args into DISPPARAMS struct and send the command
            hr = pWorld->SendThingCommand(PSBIT_NONE, pTarget, DISPID_THING_FIREEVENTINT, DISPATCH_METHOD, 
        	                              CComDISPPARAMS(3, CComVariant(bstrTellEventName), 
                                                            CComVariant((IDispatch*) pArgList),
                                                            CComVariant((long) FIREEVENT_DEFAULT)));
	    }
    }

	return hr;
}

//******************************************************
// _FireWhisperEventHelper
//
// Helper function that actually dispatches the various
// OnFooTell methods...

HRESULT  _FireWhisperEventHelper(IWorld *pWorld, IThing *pTarget, IPropertyList *pHearList, DISPPARAMS *pDispParams)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

	ASSERT(pWorld);
	ASSERT(pTarget);

	CComPtr<IThing>	pContainer;
	CComPtr<IPropertyList> pContents;
	CComVariant		varReturnVal;
	CComBSTR		bstrSaid(pDispParams->rgvarg[1].bstrVal);
	int				i;
	VARIANT_BOOL	bAbortEvent;
	HRESULT			hr = S_OK;

	pWorld->get_IsEventAborted(&bAbortEvent);
	ASSERT(bAbortEvent == VARIANT_FALSE);

	// get the container and containers
	if (FAILED(hr = pTarget->get_Container(&pContainer)))
    	return hr;

	if (FAILED(hr = pTarget->get_Contents(&pContents)))
	    return hr;

	// 1. Fire tell to my contents
	if (pContents)
	{
		static CComBSTR bstrParentSystemEvent("OnContainerSystemTell");
		static CComBSTR bstrParentEvent("OnContainerTell");
		CComPtr<IThing> pthing;
		long propcnt = 0;

		if (FAILED(hr = pContents->get_Count(&propcnt)))
        	return hr;

		for (i = 0; i < propcnt; i++)
		{
			if (FAILED(hr = pContents->get_Thing(i, &pthing)))
	            return hr;

			if (pthing)
			{
				hr = pthing->InvokeMethodExt(bstrParentEvent, pDispParams, &varReturnVal);

				if (SUCCEEDED(hr) &&
					(varReturnVal.vt == VT_BSTR) && 
					(varReturnVal.bstrVal != NULL))
				{
					bstrSaid = varReturnVal.bstrVal;
					pDispParams->rgvarg[1].bstrVal = bstrSaid;
				}
				varReturnVal.Clear();
				pthing.Release();

				// check for aborted event
				pWorld->get_IsEventAborted(&bAbortEvent);

				if (bAbortEvent == VARIANT_TRUE)
                {

            		VWTRACE(pWorld, "COREEXEMP", VWT_VERBOSE, "_FireWhisperEvent: Event aborted\n");
                    return VWOBJECT_E_EVENTABORTED;
                }
			}
		}
	}

	// 2:  Fire OnContent to the container.
	if (pContainer)
	{
		// for parent, fire OnContent
		static CComBSTR bstrChildEvent("OnContentTell");
		static CComBSTR bstrChildSystemEvent("OnContentSystemTell");

		hr = pContainer->InvokeMethodExt(bstrChildEvent, pDispParams, &varReturnVal);

		if (SUCCEEDED(hr) &&
			(varReturnVal.vt == VT_BSTR) && 
			(varReturnVal.bstrVal != NULL))
		{
			bstrSaid = varReturnVal.bstrVal;
			pDispParams->rgvarg[1].bstrVal = bstrSaid;
		}

		varReturnVal.Clear();

		// check for aborted event
		pWorld->get_IsEventAborted(&bAbortEvent);

		if (bAbortEvent == VARIANT_TRUE)
        {
    		VWTRACE(pWorld, "COREEXEMP", VWT_VERBOSE, "_FireWhisperEvent: Event aborted\n");
            return VWOBJECT_E_EVENTABORTED;
        }
	}

	// 3:  Fire event to self
	static CComBSTR bstrSelfEvent("OnTell");
	static CComBSTR bstrSelfSystemEvent("OnSystemTell");
	
	hr = pTarget->InvokeMethodExt(bstrSelfEvent, pDispParams, &varReturnVal);
	
	if (SUCCEEDED(hr) &&
		(varReturnVal.vt == VT_BSTR) && 
		(varReturnVal.bstrVal != NULL))
	{
		bstrSaid = varReturnVal.bstrVal;
		pDispParams->rgvarg[1].bstrVal = bstrSaid;
	}
	varReturnVal.Clear();

	// check for aborted event
	pWorld->get_IsEventAborted(&bAbortEvent);

	if (bAbortEvent == VARIANT_TRUE)
    {
    	VWTRACE(pWorld, "COREEXEMP", VWT_VERBOSE, "_FireWhisperEvent: Event aborted\n");
        return VWOBJECT_E_EVENTABORTED;
    }

	// 4.  Fire OnPeerTell to selected peers
	if (pContainer)
	{
		// compute the distance
		static CComBSTR bstrPeerEvent("OnPeerTell");
		static CComBSTR bstrPeerSystemEvent("OnPeerSystemTell");

		if (pHearList)
		{
			CComPtr<IThing> pCurPeer;
			CComPtr<IPropertyList> pTargetList = (IPropertyList *)pDispParams->rgvarg[2].pdispVal;
			long lPropCnt	= 0;

			if (FAILED(hr = pHearList->get_Count(&lPropCnt)))
            	return hr;

			for (i = 0; i < lPropCnt; i++)
			{
				if (FAILED(hr = pHearList->get_Thing(i, &pCurPeer)))
	                break;

				if (pCurPeer &&  (pCurPeer != pTarget))
				{
					pCurPeer->InvokeMethodExt(bstrPeerEvent, pDispParams, NULL);
					pCurPeer.Release();

					// check for abort?
					// pWorld->get_IsEventAborted(&bAbortEvent);
					//
					// if (bAbortEvent == VARIANT_TRUE)
					//	throw CHRESULT(VWOBJECT_E_EVENTABORTED);
				}
			}
		}
	}

    return hr;
}


//******************************************************
// CastToFloat
// Converts a variant into a float.

float CastToFloat(VARIANT var)
{
	switch (var.vt)
	{
		case VT_I2:
			return (float)var.iVal;
		case VT_I4:
			return (float)var.lVal;
		case VT_R4:
			return var.fltVal;
		case VT_R8:
			return (float)var.dblVal;
		default:
			return 1.0F;
	}
}

//******************************************************
// AcceptExit/AcceptEntry called on object being moved
// AllowExit called on current container
// AllowEntry called on new container
// These functions are called from MoveInto

//******************************************************
// AllowEnter
//
// Checks true if pThing is allowed to move into this object.
// Used to be: default returns false unless the owner 
// of both objects is the same.

STDMETHODIMP CThingExemplarObject::AllowEnter(IThing* pThing, VARIANT_BOOL* pbool)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pThing==NULL || pbool==NULL)
    {
    	VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::AllowEnter: NULL pointer\n");
		return ReportVWThingError(E_POINTER);
    }

	// default
	*pbool = VARIANT_TRUE;
	 return S_OK;
}


//******************************************************
// AllowLeave
// Returns TRUE if the object allows the specifed object to
// exit from its contents. 
// Used to be: The default returns FALSE  unless
// the owners of the two objects are the same.

STDMETHODIMP CThingExemplarObject::AllowLeave(IThing * pThing, VARIANT_BOOL * pbool)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pThing==NULL || pbool==NULL)
    {
    	VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::AllowLeave: NULL pointer\n");
		return ReportVWThingError(E_POINTER);
    }

	// default
	*pbool = VARIANT_TRUE;
	return S_OK;
}


//******************************************************
// AcceptEnter
// Returns TRUE if the object wants to be placed into
// a new container.  (Used to check owners.)

STDMETHODIMP CThingExemplarObject::AcceptEnter(IThing * pThing, VARIANT_BOOL * pbool)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pThing==NULL || pbool==NULL)
    {
    	VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::AcceptEnter: NULL pointer\n");
		return ReportVWThingError(E_POINTER);
    }

	// default
	*pbool = VARIANT_TRUE;
	return S_OK;
}


//******************************************************
// AcceptLeave
// Returns true if this object is willing to be moved
// out of its current container.  
// Used to be: return false unless the owners are the same 
// and the object is not IsTakeable.

STDMETHODIMP CThingExemplarObject::AcceptLeave(VARIANT_BOOL* pbool)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pbool==NULL)
    {
    	VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::AcceptLeave: NULL pointer\n");
		return ReportVWThingError(E_POINTER);
    }

	// default
	*pbool = VARIANT_TRUE;
	return S_OK;
}

#if 0
//******************************************************
// Clone
// Clone checks to see if this object is NOT an exemplar and
// IS clonable.  If both are true, it makes a new child, and
// then calls OnCloneInit on this object, passing the new
// clone as a parameter.

STDMETHODIMP CThingExemplarObject::Clone(IThing** ppNewThing)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (ppNewThing==NULL)
    {
    	VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::Clone: NULL output pointer\n");
		return ReportVWThingError(E_POINTER);
    }

	// default
	*ppNewThing	= NULL;

	CComPtr<IThing>	pThis;
	CComPtr<IThing> pThisParent;
	VARIANT_BOOL	bStatus;
	CComBSTR		strCloneName;
	CComVariant		varReturnVal;
    HRESULT         hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	if (FAILED(hr = pThis->get_BOOL(CComBSTR("IsClonable"), &bStatus)))
	    return hr;

	// if not clonable, stop now
	if (bStatus!=VARIANT_TRUE)
		return S_OK;

	// OK, now actually make the object by calling the CreateChild method...
	if (FAILED(hr = pThis->InvokeMethodExt(CComBSTR("CreateChild"), NULL, &varReturnVal)))
	    return hr;

	if (varReturnVal.vt != VT_DISPATCH || varReturnVal.pdispVal == NULL)
    {
    	VWREPORT(m_pWorld, VWR_ERROR, "CThingExemplarObject::Clone: CreateChild returned bad IDispatch\n");
		return ReportVWThingError(VWTHING_E_UNKNOWN);
    }
	
	*ppNewThing = (IThing *)(varReturnVal.pdispVal);

	(*ppNewThing)->AddRef();
	varReturnVal.Clear();

	// rename it to have the name of the cloned object
	if (FAILED(hr = pThis->get_Name(&(strCloneName.m_str))))
    	return hr;

	strCloneName.Append(" clone");
	(*ppNewThing)->put_Name(strCloneName);

	// OK, we have a new guy - let's init the clone!!

	dispparams.rgvarg[0].vt = VT_DISPATCH;
	dispparams.rgvarg[0].pdispVal = *ppNewThing;

	hr = pThis->InvokeMethodExt(CComBSTR("CloneInit"), 
                                CComDISPPARAMS(1, CComVariant((IDispatch*) *ppNewThing)),
                                NULL);

	return hr;
}
#endif

//******************************************************
// Clone
// Clone checks to see if this object is NOT an exemplar and
// IS clonable.  If both are true, it makes a new child, and
// then calls OnCloneInit on this object, passing the new
// clone as a parameter.

STDMETHODIMP CThingExemplarObject::Clone(IThing** ppNewThing)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (ppNewThing==NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::Clone: NULL output pointer\n");
		return ReportVWThingError(E_POINTER);
    }

	CComPtr<IThing> pThis;
	CComBSTR	bstrCloneName;
    HRESULT     hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

	// generate the name of the cloned object
	if (FAILED(hr = pThis->get_Name(&bstrCloneName.m_str)))
	    return hr;

	bstrCloneName.Append(" clone");

	hr = m_pWorld->CloneInstance(bstrCloneName, pThis, ppNewThing);

    return hr;
}

#if 0
//******************************************************
// Destroy
// Sends the OnDestroy event, then does the actual
// destruction

STDMETHODIMP CThingExemplarObject::Destroy()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
    HRESULT         hr = S_OK;

	if (SUCCEEDED(hr = m_pWorld->get_This(&pThis)))
    	hr = m_pWorld->DestroyObject(pThis);

	return hr;
}
#endif

//******************************************************
// GestureText
// Given a gesture name, returns the text associated with
// it.  This one is just a place-holder, and always fails.

STDMETHODIMP CThingExemplarObject::GestureText(BSTR bstrText, BSTR *pbstrText)
{
    if (pbstrText == NULL)
    {
    	VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::GestureText: NULL output pointer\n");
		return ReportVWThingError(E_POINTER);
    }

	*pbstrText = NULL;

    // REVIEW: should this return a more specific error code and errorinfo?
   	VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::GestureText: Method not implemented\n");
	return ReportVWThingError(E_NOTIMPL);
}

//******************************************************
// Emote
// The object emotes.  Emoting calls the low-level Tell
// method with our new syntax, and also optionally
// performs a state.

STDMETHODIMP CThingExemplarObject::Emote(VARIANT varGestureIn, VARIANT varTextIn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	BOOL			bUseString = FALSE;
	int				iGestNum = 0;
	CComBSTR		bstrText;
	CComPtr<IThing> pThis;
    CComVariant     varGesture;
    CComVariant     varText;
	HRESULT         hr = S_OK;

	static CComBSTR	bstrSayGesture("SayGesture");
	static CComBSTR	kbstrGestureText("GestureText");

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

	// 1.  Get rid of any BYREF indirections
    if ((FAILED(hr = ::VariantCopyInd(&varGesture, &varGestureIn))) ||
        (FAILED(hr = ::VariantCopyInd(&varText, &varTextIn))))
    {
    	VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::Emote: Error copying input VARIANTs\n");
        return ReportVWThingError(hr);
    }

	// 2.  Crack the gesture variant
    if (varGesture.vt == VT_BSTR)
    {
		// could be a numeric string - try to parse it
		BSTR	endStr;

		iGestNum = wcstol(varGesture.bstrVal, &endStr, 10);
		if (varGesture.bstrVal == endStr)
            // wasn't numeric, so we will assume it is a gesture name
			bUseString = TRUE;
	}
    else
    {
        CComVariant varGestureI4;

        // try and coerce the type to an integer if necessary
        if ((varGesture.vt == VT_I4) || SUCCEEDED(hr = varGesture.ChangeType(VT_I4, &varGestureI4)))
    		iGestNum = varGestureI4.lVal;

        // REVIEW: (WR) if this failed then iGestNum=0 -- is that a valid gesture?
	}

	// 	3. coerce or default the text as needed
	if (varText.vt == VT_BSTR)
	{
		// have what we need
		bstrText = varText.bstrVal;
	}
	else
	{
		// missing or wrong type
		CComVariant varTextBSTR;

		if (varText.vt == VT_ERROR)
            // get the gesture text based on the gesture type (missing text)
			pThis->InvokeMethodExt(kbstrGestureText, CComDISPPARAMS(1, varGesture), &varTextBSTR);
		else if (varText.vt != VT_BSTR)
            // try and coerce the variant to the right type (wrong type)
            varText.ChangeType(VT_BSTR, &varTextBSTR);

		// if still don't have text, use gesture text (i.e. neither of the above worked)
		if (varTextBSTR.vt == VT_BSTR)
			bstrText = varTextBSTR.bstrVal;
		else if (varGesture.vt == VT_BSTR)
			bstrText = varGesture.bstrVal;
        // else ... null text okay..we just don't perform the Tell below...
	}

	// 5.  Start the gesture
	if (bUseString)
	{
        // a named gesture was provided
		hr = pThis->InvokeMethodExt(CComBSTR("DoNamedGesture"), 
                                    CComDISPPARAMS(1, varGesture), NULL);
	}
	else
	{
        // an indexed gesture was provided
		hr = pThis->InvokeMethodExt(CComBSTR("DoGesture"), 
                                    CComDISPPARAMS(1, CComVariant(iGestNum)), NULL);
	}

	// 6. now, do the actual tell if we have a string	
	if ((hr == S_OK) && bstrText)
	{
		hr = pThis->InvokeMethodExt(bstrSayGesture, 
                                    CComDISPPARAMS(1, CComVariant(bstrText)), NULL);
	}

	return hr;
}

#if 0
	if (varGesture.vt == (VT_VARIANT | VT_BYREF))
		varGesture = *varGesture.pvarVal;
	if (varText.vt == (VT_VARIANT | VT_BYREF))
		varText = *varText.pvarVal;

	// 2.  Crack the gesture variant
	switch (varGesture.vt)
	{
	case VT_BSTR:
		{
			// could be a numeric string - try to parse it
			BSTR	endStr;

			iGestNum = wcstol(varGesture.bstrVal, &endStr, 10);
			if (varGesture.bstrVal == endStr)
				bUseString = TRUE;
		}
		break;

	default:
		{
			HRESULT	hr = S_OK;
			CComVariant varI4;
			
			hr = ::VariantChangeType(&varI4, &varGesture, 0, VT_I4);
			if (SUCCEEDED(hr))
				iGestNum = varI4.lVal;
            // REVIEW: (WR) shouldn't we fail in an else clause here? - iGestNum = 0...
		}
	}

	// 	3. coerce or default the text as needed
	if (varText.vt == VT_BSTR)
	{
		// have what we need
		bstrText = varText.bstrVal;
	}
	else
	{
		// missing or wrong type
		CComVariant varTextBSTR;

		if (varText.vt == VT_ERROR)
			pThis->InvokeMethodExt(kbstrGestureText, CComDISPPARAMS(1, varGesture), &varTextBSTR);
		else if (varText.vt != VT_BSTR)
			::VariantChangeType(&varTextBSTR, &varText, 0, VT_BSTR);

		// if still don't have text, use gesture text
		if (varTextBSTR.vt == VT_BSTR)
			bstrText = varTextBSTR.bstrVal;
		else if (varGesture.vt == VT_BSTR)
			bstrText = varGesture.bstrVal;
	}

	// 5.  Start the gesture
	if (FAILED(hr = InitializeDISPPARAMS(&dispparams, 1)))
    {
	    VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::Emote: Error allocating DISPPARAMS: %x\n", hr);
	    goto ERROR_ENCOUNTERED;
    }
	
	if (bUseString)
	{
		dispparams.rgvarg[0] = varGesture;

		hr = pThis->InvokeMethodExt(CComBSTR("DoNamedGesture"), &dispparams, NULL);
	}
	else
	{
		dispparams.rgvarg[0].vt = VT_I4;
		dispparams.rgvarg[0].lVal = iGestNum;

		hr = pThis->InvokeMethodExt(CComBSTR("DoGesture"), &dispparams, NULL);
	}

	// 6. now, do the actual tell if we have a string	
	if ((hr == S_OK) && bstrText)
	{
		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstrText;
		
		hr = pThis->InvokeMethodExt(bstrSayGesture, &dispparams, NULL);
	}

ERROR_ENCOUNTERED:

	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	return ReportVWThingError(hr);
}
#endif


//******************************************************
// MoveInto
// MoveInto moves something from its current container into
// a new container.  
// MoveInto is allowed if:
//	Doing a CreateInstance
//  Object is first entering world
//	Caller is taking/dropping object (object must be takeable)
//	Caller is owner of object
//	Caller is wizard
//	Caller is portal 
// If MoveInto is allowed, we check Accept/Allow policy:
//	item->AcceptLeave
//	item->oldContainer->AllowLeave
//	item->AcceptEnter
//	item->newContainer->AllowEnter

STDMETHODIMP CThingExemplarObject::MoveInto(IThing *pNewContainer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT			hr	= S_OK;
	CComPtr<IThing> pThis;
	CComPtr<IThing> pCaller;
	CComPtr<IThing> pCallersContainer;
	CComPtr<IThing> pOldContainer;
	CComPtr<IThing> pOwner;
	CComVariant		varReturn;
	VARIANT_BOOL bIsAbleToDoMove = VARIANT_FALSE;		// assume you can't move the object by default
	VARIANT_BOOL bCanChangeContainer = VARIANT_FALSE;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

	if (pThis)
	{
		if (FAILED(hr = pThis->get_Owner(&pOwner)))
			return hr;
        
		if (FAILED(hr = pThis->get_Container(&pOldContainer)))
			return hr;
		
		if (FAILED(hr = pThis->get_BOOL(CComBSTR("CanChangeContainer"), &bCanChangeContainer)))
			return hr;

	}

	if (FAILED(hr = m_pWorld->get_Caller(&pCaller)))
	    return hr;

	if (pCaller)
	{
		if (FAILED(hr = pCaller->get_Container(&pCallersContainer)))
			return hr;
	}


#ifdef _DEBUG
	CComBSTR bstrCallerName, bstrOwnerName;
	if (pCaller)
    {
		if (FAILED(hr = pCaller->get_Name(&bstrCallerName.m_str)))
	        return hr;
    }

	if (pOwner)
    {
		if (FAILED(hr = pOwner->get_Name(&bstrOwnerName.m_str)))
	        return hr;
    }
#endif
	
	if (pOldContainer == pNewContainer)
	{
		// Don't need to do anything
		// Return a success HRESULT (S_OK)
        return S_OK;
	}

	// Simple cases of CreateInstance and first entering world
	// First, check for case of CreateInstance, when your previous container is NULL
	if (pOldContainer == NULL)
		bIsAbleToDoMove = VARIANT_TRUE;

	// Check if we are first entering world, when there is no caller/user yet
	if (pCaller == NULL)
		bIsAbleToDoMove = VARIANT_TRUE;
	
	if (bIsAbleToDoMove == VARIANT_FALSE)
	{	
		// Check if CanChangeContainer is set to true
		if (bCanChangeContainer == VARIANT_TRUE)
			bIsAbleToDoMove = VARIANT_TRUE;
		
		// Now check special cases: caller is owner of object, caller is wizard, 
		// caller's owner is a wizard, or caller is portal:
		else
		{
			// Is the caller the owner of the object?
			if (pOwner)
			{
				if (pCaller==pOwner)
					bIsAbleToDoMove = VARIANT_TRUE;
			}
				
			// Now if caller != owner, check if wizard
			if (bIsAbleToDoMove == VARIANT_FALSE)
			{
				VARIANT_BOOL bIsAvatar;
				VARIANT_BOOL bIsWizard = VARIANT_FALSE;

				if (FAILED(hr = pCaller->IsOfType(CComBSTR("Avatar"), &bIsAvatar)))
	                return hr;

				if (bIsAvatar)
				{
					if (FAILED(hr = pCaller->get_IsWizard(&bIsWizard)))
	                    return hr;

					if (bIsWizard == VARIANT_TRUE)
						bIsAbleToDoMove = VARIANT_TRUE;
				}

				// If caller isn't an avatar, check if caller's owner is a wizard (like CP_CALLER policy check)
				// (case of object being created by TransactionMachine)
				else
				{
					CComPtr<IThing>	pCallerOwner;
					pCaller->get_Owner(&pCallerOwner);
					if (pCallerOwner)
					{
						if (FAILED(hr = pCallerOwner->IsOfType(CComBSTR("Avatar"), &bIsAvatar)))
	                        return hr;

						if (bIsAvatar)
						{
							if (FAILED(hr = pCallerOwner->get_IsWizard(&bIsWizard)))
	                            return hr;

							if (bIsWizard == VARIANT_TRUE)
								bIsAbleToDoMove = VARIANT_TRUE;
						}
					}
				}
			}

			// If none of these are true,
			// Test if portal (we'll worry about it being on correct room list later)
			if (bIsAbleToDoMove == VARIANT_FALSE)
			{
				VARIANT_BOOL bIsPortal;
				if (FAILED(hr = pCaller->IsOfType(CComBSTR("Portal"), &bIsPortal)))
	                return hr;

				if (bIsPortal)
					bIsAbleToDoMove = VARIANT_TRUE;;
			}
		}
	}

	// Check Accepts & Allows if you are allowed move it 
	if (bIsAbleToDoMove == VARIANT_TRUE)
	{
		// 1. item->AcceptLeave
		if (pThis)
        {
			if (FAILED(hr = pThis->InvokeMethodExt(CComBSTR("AcceptLeave"), NULL, &varReturn)))
                return hr;
        }

		if (varReturn.vt!=VT_BOOL)
        {
    		VWREPORT(m_pWorld, VWR_ERROR, "CThingExemplarObject::MoveInto AcceptLeave returned variant type != VT_BOOL\n");
            return ReportVWThingError(VWTHING_E_UNKNOWN);
        }

		// if doesn't allow, exit with appropriate HR
		if (varReturn.boolVal != VARIANT_TRUE)
        {
    		VWTRACE(m_pWorld, "COREEXEMP", VWT_VERBOSE, "CThingExemplarObject::MoveInto AcceptLeave not allowed\n");
            return ReportVWThingError(VWTHING_E_ACCEPTLEAVEDENIED);
        }

		// 2.  item->oldContainer->AllowLeave
		// ask if it's ok to leave where we are
		if ((pThis) && (pOldContainer))
		{	
			if (FAILED(hr = pOldContainer->InvokeMethodExt(CComBSTR("AllowLeave"), 
                                                           CComDISPPARAMS(1, CComVariant((IDispatch*) pThis)),
                                                           &varReturn)))
                return hr;

			if (varReturn.vt!=VT_BOOL)
            {
        		VWREPORT(m_pWorld, VWR_ERROR, "CThingExemplarObject::MoveInto AllowLeave returned variant type != VT_BOOL\n");
                return ReportVWThingError(VWTHING_E_UNKNOWN);
            }

			// if doesn't allow, exit with appropriate HR
			if (varReturn.boolVal != VARIANT_TRUE)
            {
        		VWTRACE(m_pWorld, "COREEXEMP", VWT_VERBOSE, "CThingExemplarObject::MoveInto AllowLeave not allowed\n");
                return ReportVWThingError(VWTHING_E_ALLOWLEAVEDENIED);
            }
		}

		// 3.  item->AcceptEnter
		if ((pThis) && (pNewContainer))
		{
			if (FAILED(hr = pThis->InvokeMethodExt(CComBSTR("AcceptEnter"), 
                                                   CComDISPPARAMS(1, CComVariant((IDispatch*) pNewContainer)),
                                                   &varReturn)))
                return hr;

			if (varReturn.vt!=VT_BOOL)
            {
        		VWREPORT(m_pWorld, VWR_ERROR, "CThingExemplarObject::MoveInto AcceptEnter returned variant type != VT_BOOL\n");
                return ReportVWThingError(VWTHING_E_UNKNOWN);
            }

			// if doesn't allow, exit with appropriate HR
			if (varReturn.boolVal != VARIANT_TRUE)
            {
        		VWTRACE(m_pWorld, "COREEXEMP", VWT_VERBOSE, "CThingExemplarObject::MoveInto AcceptEnter not allowed\n");
                return ReportVWThingError(VWTHING_E_ACCEPTENTERDENIED);
            }
		}
	
		// 4.  newContainer->AllowEnter
		if ((pThis) && (pNewContainer))	
		{
			if (FAILED(hr = pNewContainer->InvokeMethodExt(CComBSTR("AllowEnter"), 
                                                           CComDISPPARAMS(1, CComVariant((IDispatch*) pThis)),
                                                           &varReturn)))
                return hr;

			if (varReturn.vt!=VT_BOOL)
            {
        		VWREPORT(m_pWorld, VWR_ERROR, "CThingExemplarObject::MoveInto AllowEnter returned variant type != VT_BOOL\n");
                return ReportVWThingError(VWTHING_E_UNKNOWN);
            }

			// if doesn't allow, exit 
			if (varReturn.boolVal != VARIANT_TRUE)
            {
        		VWTRACE(m_pWorld, "COREEXEMP", VWT_VERBOSE, "CThingExemplarObject::MoveInto AllowEnter not allowed\n");
                return ReportVWThingError(VWTHING_E_ALLOWENTERDENIED);
            }
		}

		// 5.  Actually perform the low-level move
		hr = pThis->MoveIntoExt(pNewContainer);
	}

	// Don't have permission to do MoveInto - policy checks have failed
	else
		return ReportVWThingError(VWTHING_E_MOVEINTOPOLICYDENIED);

    return hr;
}

//******************************************************
// _FireTellEvent
//
// Fires a tell event with the specified parameters...

HRESULT _FireTellEvent(IThing *pSpeaker, IPropertyList *pTargetList, BSTR bstrText, long talkType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	ASSERT(pSpeaker);

	static CComBSTR	kbstrSystemTell("SystemTell");	
	static CComBSTR	kbstrTell("Tell");	
    HRESULT hr = S_OK;

	// now, do the actual whisper
	CComVariant    varSpeaker((IDispatch*) pSpeaker);
	CComVariant    varTargetList((IDispatch*)pTargetList);
	CComVariant    varText(bstrText);
	CComVariant    varType(talkType);
    CComDISPPARAMS dispparams(4, varSpeaker, varTargetList, varText, varType);
    
	// Now, fire the method
	if (FAILED(hr = pSpeaker->FireEventExt(kbstrSystemTell, dispparams, FIREEVENT_DEFAULT)))
	    return hr;

	hr = pSpeaker->FireEventExt(kbstrTell, dispparams, FIREEVENT_DEFAULT);

    return hr;
}

//******************************************************
// Say
// Says the line

STDMETHODIMP CThingExemplarObject::Say(BSTR bstrText, VARIANT userOrListIn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    HRESULT         hr = S_OK;
	CComPtr<IThing> pThis;
	CComPtr<IPropertyList> pTargetList;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;
    
#ifdef REPORT_PERF
	CComBSTR bstrUser;

	pThis->get_Name(&bstrUser.m_str);

	VWREPORT(m_pWorld, VWR_PERFLOG, "User Say,%d,%s\n", GetTickCount(), CString(bstrUser));
#endif

	// now, parse the variant into a property list

	// check for variant to a variant
	// if so, need to crack it and continue...
    CComVariant userOrList;
    if (FAILED(hr = VariantCopyInd(&userOrList, &userOrListIn)))
    {
        VWTRACE(m_pWorld, "VWTHING", VWT_ERROR, "CThingExemplarObject::Say: cannot convert input VARIANT\n");
        return ReportVWThingError(hr);
    }

	switch (userOrList.vt)
	{
	case VT_SAFEARRAY:
		if (FAILED(hr = CreatePropertyList(m_pWorld, &pTargetList)))
	        return ReportVWThingError(hr);

		if (FAILED(hr = ConvertSAFEARRAYToPropertyList(&userOrList, pTargetList)))
        {
            VWTRACE(m_pWorld, "VWTHING", VWT_ERROR, "CThingExemplarObject::Say: cannot convert input VARIANT to property list\n");
            return ReportVWThingError(hr);
        }
		break;
	
	case VT_DISPATCH:
		if (userOrList.pdispVal)
		{
			// could be either an IThing or an IPropertyList...
			CComQIPtr<IThing, &IID_IThing> pthing;
			CComQIPtr<IPropertyList, &IID_IPropertyList> plist;

			pthing = userOrList.pdispVal;
			if (pthing)
			{
				// we have a thing - create a list and add to it
				if (FAILED(hr = CreatePropertyList(m_pWorld, &pTargetList)))
                	return ReportVWThingError(hr);

				if (FAILED(hr = pTargetList->AddDispatch(pthing)))
    				return ReportVWThingError(hr);
			}
			else
			{
				// see if we have a list
				plist = userOrList.pdispVal;

				// we have a list - use it instead
				if (plist)
				{
					pTargetList = plist;
					plist.Release();
				}
			}
		}
		break;

	case VT_ERROR:
		if (userOrList.lVal == DISP_E_PARAMNOTFOUND)
		{
			// if no target, say to no one!
			pTargetList = NULL;
		}
		break;

	default:
		// invalid args!!
		// REVIEW: ??? is this the correct error???
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::Say: Bad vartype\n");
		return ReportVWThingError(DISP_E_BADVARTYPE); 
	}

	// Now, do the tell
    hr = _FireTellEvent(pThis, pTargetList, bstrText, tcSay);

    return hr;
}


//******************************************************
// Shout
// Shouts the text into the room.  When you shout, it
// is heard by everyone in the room.

STDMETHODIMP CThingExemplarObject::Shout(BSTR bstrText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
    HRESULT         hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

#ifdef REPORT_PERF
	CComBSTR bstrUser;

	pThis->get_Name(&bstrUser.m_str);

	VWREPORT(m_pWorld, VWR_PERFLOG, "User Shout,%d,%s\n", GetTickCount(), CString(bstrUser));
#endif

	// now, do the actual shout	
	hr = _FireTellEvent(pThis, NULL, bstrText, tcSay);

    return ReportVWThingError(hr);
}

//******************************************************
// SayGesture
// Says a gesture.  This is basically the same.

STDMETHODIMP CThingExemplarObject::SayGesture(BSTR bstrText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
    HRESULT         hr = S_OK;    

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

#ifdef REPORT_PERF
	CComBSTR bstrUser;

	pThis->get_Name(&bstrUser.m_str);

	VWREPORT(m_pWorld, VWR_PERFLOG, "User Gesture/Emote,%d,%s\n", GetTickCount(), CString(bstrUser));
#endif

	// now, do the actual shout	
	hr = _FireTellEvent(pThis, NULL, bstrText, tcEmote);

	return ReportVWThingError(hr);
}

//******************************************************
// Whisper
// Whispers the text.  Other people can tell you are
// whispering, but can't hear you.

STDMETHODIMP CThingExemplarObject::Whisper(BSTR bstrText, VARIANT userOrList)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    HRESULT         hr = S_OK;
	CComPtr<IThing> pThis;
	CComPtr<IPropertyList> pTargetList;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

#ifdef REPORT_PERF
	CComBSTR bstrUser;

	pThis->get_Name(&bstrUser.m_str);

	VWREPORT(m_pWorld, VWR_PERFLOG, "User Whisper,%d,%s\n", GetTickCount(), CString(bstrUser));
#endif
	// We are going to use the new page-based whisper to do the work...
	CComPtr<IThing>	pContainer;
	
	if (SUCCEEDED(hr = pThis->get_Container(&pContainer)))
    {
	    if (pContainer)
	    {
		    static CComBSTR	kbstrTransmitWhisper = "TransmitWhisper";
		    
		    hr = pContainer->InvokeMethodExt(kbstrTransmitWhisper, CComDISPPARAMS(2, CComVariant(bstrText), userOrList), NULL);
	    }
    }
	/*
	// now, parse the variant into a property list

	// check for variant to a variant
	// if so, need to crack it and continue...
	if (userOrList.vt == (VT_VARIANT | VT_BYREF))
		userOrList = *userOrList.pvarVal;

	switch (userOrList.vt)
	{
	case VT_SAFEARRAY:
		if (FAILED(hr = CreatePropertyList(m_pWorld, &pTargetList)))
	goto ERROR_ENCOUNTERED;
		if (FAILED(hr = ConvertSAFEARRAYToPropertyList(&userOrList, pTargetList)))
	goto ERROR_ENCOUNTERED;
		break;
	
	case VT_DISPATCH:
		if (userOrList.pdispVal)
		{
			// could be either an IThing or an IPropertyList...
			CComQIPtr<IThing, &IID_IThing> pthing;
			CComQIPtr<IPropertyList, &IID_IPropertyList> plist;

			pthing = userOrList.pdispVal;
			if (pthing)
			{
				// we have a thing - create a list and add to it
				if (FAILED(hr = CreatePropertyList(m_pWorld, &pTargetList)))
	goto ERROR_ENCOUNTERED;
				if (FAILED(hr = pTargetList->AddDispatch(pthing)))
	goto ERROR_ENCOUNTERED;
				pthing.Release())
	goto ERROR_ENCOUNTERED;
			}
			else
			{
				// see if we have a list
				plist = userOrList.pdispVal;

				// we have a list - use it instead
				if (plist)
				{
					pTargetList = plist;
					plist.Release();
				}
			}
		}
		break;

	case VT_ERROR:
		if (userOrList.lVal == DISP_E_PARAMNOTFOUND)
		{
			// if no target, say to no one!
			pTargetList = NULL;
		}
		break;

	default:
		// invalid args!!
		// REVIEW: ??? is this the correct error???
		throw CHRESULTException(DISP_E_BADVARTYPE); 
		break;
	}

	// Now, do the tell
	_FireWhisperEvent(m_pWorld, pThis, pTargetList, bstrText);
	*/

	return hr;
}


//******************************************************
// Announce
// Announce is used to tell something to everyone in the thing's
// that is very important

STDMETHODIMP CThingExemplarObject::Announce(BSTR bstrText)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
    HRESULT         hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

#ifdef REPORT_PERF
	CComBSTR bstrUser;

	pThis->get_Name(&bstrUser.m_str);

	VWREPORT(m_pWorld, VWR_PERFLOG, "User Announce,%d,%s\n", GetTickCount(), CString(bstrUser));
#endif

	// now, do the actual shout	
	hr = _FireTellEvent(pThis, NULL, bstrText, tcAnnounce);

	return ReportVWThingError(hr);
}

//******************************************************
// Report
// Report is used to tell something to everyone in the room that
// is of little importance

STDMETHODIMP CThingExemplarObject::Report(BSTR bstrText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
    HRESULT         hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

#ifdef REPORT_PERF
	CComBSTR bstrUser;

	pThis->get_Name(&bstrUser.m_str);

	VWREPORT(m_pWorld, VWR_PERFLOG, "User Report,%d,%s\n", GetTickCount(), CString(bstrUser));
#endif

	// now, do the actual shout	
	hr = _FireTellEvent(pThis, NULL, bstrText, tcReport);

	return ReportVWThingError(hr);
}


//******************************************************
// LookAt
// Looks at the object
// Fire "OnMoreAbout" UI event for things
// Overwritten in avatar to fire "OnShowProfile" 

STDMETHODIMP CThingExemplarObject::LookAt()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
    HRESULT         hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	hr = m_pWorld->FireUIEvent(pThis, CComBSTR(VW_MOREABOUT_EVENT_STR), CComVariant(pThis));

	return hr;
}

//******************************************************
// GetMenuList
// Returns a copy of the menu property

STDMETHODIMP CThingExemplarObject::GetMenuList(IPropertyList **pplistMenu)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pplistMenu==NULL)
    {
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::GetMenuList: NULL ouput pointer\n");
		return ReportVWThingError(E_POINTER);
    }

	*pplistMenu = NULL;

	CComPtr<IPropertyList> pMenuList;
    HRESULT hr = S_OK;

	// Create an empty list
	if (SUCCEEDED(hr = CreatePropertyList(m_pWorld, &pMenuList)))
    {

	    // Build the menu list
	    hr = BuildMenuList(pMenuList);

	// And sort it
//		if (FAILED(hr = SortMenuList(pMenuList)))
//	        goto ERROR_ENCOUNTERED;
    }

	// Set
	*pplistMenu = pMenuList;
	(*pplistMenu)->AddRef();

	return hr;
}

//******************************************************
// BuildMenuList

HRESULT CThingExemplarObject::BuildMenuList(IPropertyList *pMenuList)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
	CComPtr<IThing> pContainer;
	CComPtr<IThing> pUserContainer;
	CComPtr<IThing> pUser;
	CComPtr<IPropertyList> pContentList;
	CComPtr<IThing> pContent;
	CComPtr<IThing> pInvItem;
	long i;
    HRESULT hr = S_OK;
	VARIANT_BOOL bStub = VARIANT_FALSE;
    VARIANT_BOOL vbAuthor = VARIANT_FALSE;

    ASSERT(pMenuList);

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	// Get the container
	if (FAILED(hr = pThis->get_Container(&pContainer)))
	    return hr;

	// Get the user
	if (FAILED(hr = m_pWorld->get_User(&pUser)))
	    return hr;

	// Collect menu items from various sources; first the thing itself
	if (pThis != pUser)
    {
		// If it's not self, get it from GetMenuItems
		if (FAILED(hr = AddMenuSource(pThis, pThis, CComBSTR("GetMenuItems"), pMenuList)))
	       return hr;
    }
	else
    {
		// Or if it's self, then GetSelfMenuItems
		if (FAILED(hr = AddMenuSource(pThis, pThis, CComBSTR("GetSelfMenuItems"), pMenuList)))
	        return hr;
    }

	// Second, the container of the thing (if this is a room, there is no container)
	if (pContainer)
	{
		if (FAILED(hr = AddMenuSource(pThis, pContainer, CComBSTR("GetContainerMenuItems"), pMenuList)))
	        return hr;
	}

	// Third, the peers (user only, and if the target is not self)
	if (pThis != pUser)
	{
		// Check if the user and object are in the same container
		if (FAILED(hr = pUser->get_Container(&pUserContainer)))
	        return hr;

		if (pContainer == pUserContainer)
        {
			if (FAILED(hr = AddMenuSource(pThis, pUser, CComBSTR("GetPeerMenuItems"), pMenuList)))
	            return hr;
        }
	}

	// Fourth, the contents of the object itself
	if (FAILED(hr = pThis->get_Contents(&pContentList)))
	    return hr;

	// Make sure we got a valid contents
	if (pContentList)
	{
		for (i = 0; ; i++)
		{
			if (FAILED(pContentList->get_Thing(i, &pContent)))
				break;

			ASSERT(pContent);

			// this object may be a stub if its container isn't marked as open,
			// and/or it isn't marked as noticeable
			/* hr = */ pContent->get_IsStub(&bStub);

			if (bStub == VARIANT_FALSE)
            {
				if (FAILED(hr = AddMenuSource(pThis, pContent, CComBSTR("GetContentMenuItems"), pMenuList)))
	                return hr;
            }
			pContent.Release();
		}
        pContentList.Release();
	}

	// Finally, the contents of the user's inventory
	if (FAILED(hr = pUser->get_Contents(&pContentList)))
	    return hr;

	for (i = 0; ; i++)
	{
		if (FAILED(pContentList->get_Thing(i, &pContent)))
			break;

		ASSERT(pContent);

		if (FAILED(hr = AddMenuSource(pThis, pContent, CComBSTR("GetInventoryMenuItems"), pMenuList)))
	        return hr;

		pContent.Release();
	}

	// only add the edit menu if we are an author
	if (FAILED(hr = pUser->get_BOOL(CComBSTR("IsAuthor"), &vbAuthor)))
        return hr;
	
    if (vbAuthor == VARIANT_TRUE)
	    hr = AddMenuSource(pThis, pThis, CComBSTR("GetEditMenuItems"), pMenuList);

	return hr;
}

//******************************************************
// AddMenuSource

HRESULT CThingExemplarObject::AddMenuSource(IThing *pTarget, IThing *pThing, BSTR bstrMethod,
											IPropertyList *pMenuList)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComVariant varResult;
	CComQIPtr<IPropertyList, &IID_IPropertyList> pSourceMenuList;
	CComPtr<IMenuItem> pMIReal;
	CComPtr<IMenuItem> pMICopy;
	CComPtr<IMenuItem> pMI;
	long cnt = 0, incomingcnt = 0;
	HRESULT hr = S_OK;

    ASSERT(pThing);

	// Invoke the method to return the list
	if (FAILED(hr = pThing->InvokeMethod(bstrMethod, CComVariant(pTarget), &varResult)))
	    return hr;

	if (varResult.vt!=VT_DISPATCH)
    {
		VWREPORT(m_pWorld, VWR_ERROR, "CThingExemplarObject::AddMenuSource varResult.vt!=VT_DISPATCH\n");
        return ReportVWThingError(VWTHING_E_UNKNOWN);
    }
	
	// If we got it, process the results
	if (varResult.pdispVal)
	{
		pSourceMenuList = varResult.pdispVal;
		if (pSourceMenuList==NULL)
        {
    		VWREPORT(m_pWorld, VWR_ERROR, "CThingExemplarObject::AddMenuSource Source menu list NULL\n");
            return ReportVWThingError(VWTHING_E_UNKNOWN);
        }

		// Now update the menu items
		if (FAILED(hr = UpdateMenuList(pSourceMenuList, pTarget, pThing)))
        	return hr;

		if (FAILED(hr = pMenuList->get_Count(&incomingcnt)))
	        return hr;

		if (FAILED(hr = pSourceMenuList->get_Count(&cnt)))
	        return hr;

		// if we're going to add one or more menu items, prepend a separator (but only if there are already items in the menu)
		if (incomingcnt > 0 && cnt > 0)
		{
			if (FAILED(hr = CreateMenuSeparator(m_pWorld, NULL, &pMI)))
	            return ReportVWThingError(hr);

			pMenuList->AddObjectProperty(pMI);

			pMI.Release();
		}

		// Copy the results into the menu list
		for (int i = 0; i < cnt; i++)
		{
			hr = pSourceMenuList->get_ObjectPropertyExt(i, IID_IMenuItem, (IObjectProperty**) &pMIReal);
			if (SUCCEEDED(hr) && pMIReal)
			{
				// Deep copy the item
				if (FAILED(hr = pMIReal->Duplicate(&pMICopy)))
	                return hr;

				ASSERT(pMICopy);

				// Add it into the list
				if (FAILED(hr = pMenuList->AddObjectProperty(pMICopy)))
                	return hr;

				// release loop stuff
				pMICopy.Release();
				pMIReal.Release();
			}
		}
	}

	return hr;
}

//******************************************************
// SetMenuListTarget
// This is a helper function to generate a menu list...

HRESULT CThingExemplarObject::SetMenuListTarget(IPropertyList* pMenuList, IThing *pTarget,
												IThing *pOwner)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    HRESULT hr = S_OK;
	CComPtr<IMenuItem> pMI;
	CComPtr<IPropertyList> pSubMenu;

    ASSERT(pMenuList);

	// Set each item in the list
	for (int i = 0; SUCCEEDED(pMenuList->get_ObjectPropertyExt(i, IID_IMenuItem,
													(IObjectProperty**) &pMI)); i++)
	{
		if (pMI)
		{
			// Set the target
			if (FAILED(hr = pMI->put_Target(pTarget)))
            	break;

			// Set the owner
			if (FAILED(hr = pMI->put_Owner(pOwner)))
	            break;

			pMI.Release();
		}
	}

	return hr;
}

//******************************************************
// UpdateMenuList
// This is a helper function to generate a menu list...

HRESULT CThingExemplarObject::UpdateMenuList(IPropertyList* pMenuList, IThing *pTarget,
											 IThing *pThing)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    HRESULT hr = S_OK;
	CComPtr<IMenuItem> pMI;
	CComPtr<IPropertyList> pSubMenu;

	// Patch in the owners and targets
	if (FAILED(hr = SetMenuListTarget(pMenuList, pTarget, pThing)))
	    return hr;
    
	for (int i=0; SUCCEEDED(pMenuList->get_ObjectPropertyExt(i, IID_IMenuItem,
													(IObjectProperty**) &pMI)); i++)
	{
		if (pMI)
		{
			// now call the update function to give the menu item a chance to 
			// modify itself
			// NOTE: Ignore error. Don't exit from loop or other
			// menu items won't get updated!
			pMI->OnUpdate();

			// If there's a sub-menu, update it too (and set its pop-up menu flag)
			pMI->get_SubMenu(&pSubMenu);
			if (pSubMenu)
			{
				long lFlags;

				// Set the pop-up flag
				if (FAILED(hr = pMI->get_Flags(&lFlags)))
                	break;

				lFlags |= MI_POPUP;
				if (FAILED(hr = pMI->put_Flags(lFlags)))
	                break;

				// And update the sub-menu items
				if (FAILED(hr = UpdateMenuList(pSubMenu, pTarget, pThing)))
	                break;
			}

			// release loop stuff
			pMI.Release();
			pSubMenu.Release();
		}
	}

	return hr;
}

#if 0
//******************************************************
// SortMenuList

HRESULT CThingExemplarObject::SortMenuList(IPropertyList *pMenuList)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    HRESULT hr = S_OK;
	long cnt, i, j, k;
	long lSortOrder1, lSortOrder2;
	CComPtr<IMenuItem> pMI1;
	CComPtr<IMenuItem> pMI2;;
	CComPtr<IPropertyList> pSublist1;
	CComPtr<IPropertyList> pSublist2;
	VARIANT_BOOL boolNewLow;
	VARIANT_BOOL boolGotMenuText;
	CComBSTR bstrMenuText1, bstrMenuText2;
	CString cstringMenuText1, cstringMenuText2;
	int iOff;
	long lFlags;

    ASSERT(pMenuList);

	// Find out how many we're dealing with
	if (FAILED(hr = pMenuList->get_Count(&cnt)))
    	return hr;

	// if no items, nothing to do
	if (cnt==0)
		return S_OK;

	// Sort the top-level list
	for (i = 0; i < (cnt-1); i++)
	{
		// Find the lowest sort order item
		lSortOrder1 = LONG_MAX;
		boolGotMenuText = VARIANT_FALSE;
		k = -1;
		for (j = i; j < cnt; j++)
		{
			// Get the item
			if (FAILED(hr = pMenuList->get_ObjectPropertyExt(j, IID_IMenuItem, (IObjectProperty**) &pMI2)) || !pMI2)
	            return hr;

			// Get the sort order
			pMI2->get_SortOrder(&lSortOrder2);

			// Check if it's a new low
			boolNewLow = VARIANT_FALSE;
			if (lSortOrder2 == lSortOrder1)
			{
				// If it's the same sort order, then sort by menu text
				if (!boolGotMenuText)
				{
					// If we don't have the menu text for the previous item, get it
					if (FAILED(hr = pMI1->get_MenuText(&bstrMenuText1.m_str)))
	                    return hr;

					cstringMenuText1 = bstrMenuText1;
					bstrMenuText1.Empty();
					// But eliminate "&"s
					while ((iOff = cstringMenuText1.Find('&')) >= 0)
					{
						cstringMenuText1 = cstringMenuText1.Left(iOff) +
									cstringMenuText1.Right(cstringMenuText1.GetLength()-
															iOff-1);
					}
					boolGotMenuText = VARIANT_TRUE;
				}
				// Get the menu text for the new item
				if (FAILED(hr = pMI2->get_MenuText(&bstrMenuText2.m_str)))
	                return hr;

				cstringMenuText2 = bstrMenuText2;
				bstrMenuText2.Empty();
				// But first eliminate "&"s
				while ((iOff = cstringMenuText2.Find('&')) >= 0)
				{
					cstringMenuText2 = cstringMenuText2.Left(iOff) +
								cstringMenuText2.Right(cstringMenuText2.GetLength()-iOff-1);
				}
				boolNewLow = cstringMenuText2.Collate(cstringMenuText1) < 0;
			}
			else boolNewLow = lSortOrder2 < lSortOrder1;
			if (boolNewLow)
			{
				// New low; remember the specifics
				lSortOrder1 = lSortOrder2;
				k = j;
				pMI1.Release();
				pMI1 = pMI2;
				pMI2 = NULL;
				cstringMenuText1 = cstringMenuText2;
			}
			else
			{
				pMI2.Release();
			}
		}
		// If we got something, swap it into the next slot
		if (k >= 0 && i != k)
		{
			// Get the two items
			if (FAILED(hr = pMenuList->get_ObjectPropertyExt(i, IID_IMenuItem, (IObjectProperty**) &pMI2)))
	            return hr;

			// Null out the slots (to clear the back-pointers
			if (FAILED(hr = pMenuList->put_ObjectProperty(i, NULL)))
	            return hr;

			if (FAILED(hr = pMenuList->put_ObjectProperty(k, NULL)))
	            return hr;

			// Put them back reversed
			if (FAILED(hr = pMenuList->put_ObjectProperty(i, pMI1)))
	            return hr;

			if (FAILED(hr = pMenuList->put_ObjectProperty(k, pMI2)))
	            return hr;
		}
		pMI1.Release();
		pMI2.Release();
	}

	// Eliminate invisible items (simplifies subsequent processing)
	for (i = 0; i < cnt; i++)
	{
		// Get the next visibility
		if (FAILED(hr = pMenuList->get_ObjectPropertyExt(i, IID_IMenuItem, (IObjectProperty**) &pMI1)) || !pMI1)
	        return hr;

		pMI1->get_Flags(&lFlags))
	        return hr;

		pMI1.Release();

		// If it's invisible, remove it
		if (lFlags & MI_INVISIBLE)
		{
			if (FAILED(hr = pMenuList->RemoveAt(i)))
            	return hr;

			cnt--;
			i--;
		}
	}

	// Combine sub-menus with the same name and sort order
	if (FAILED(hr = pMenuList->get_ObjectPropertyExt(0, IID_IMenuItem, (IObjectProperty**) &pMI1)) || !pMI1)
	    return hr;

	if (FAILED(hr = pMI1->get_MenuText(&bstrMenuText1.m_str)))
	    return hr;

	if (FAILED(hr = pMI1->get_SubMenu(&pSublist1)))
	    return hr;

	pMI1.Release();

    for (i = 1; i < cnt; i++)
	{
		// Get the next sub-menu list & name
		if (FAILED(hr = pMenuList->get_ObjectPropertyExt(i, IID_IMenuItem, (IObjectProperty**) &pMI2)) || !pMI2)
	        return hr;

		if (FAILED(hr = pMI2->get_MenuText(&bstrMenuText2.m_str)))
	        return hr;

		if (FAILED(hr = pMI2->get_SubMenu(&pSublist2)))
	        return hr;

		pMI2.Release();

		// Sublist with same name?
		if (pSublist1 && pSublist2 && (CString(bstrMenuText1) == CString(bstrMenuText2)))
		{
			// Combine the two sublists
			pSublist2->CopyTo(pSublist1);
			pSublist2.Release();

			// And eliminate the second entry
			pMenuList->RemoveAt(i);
			cnt--;
			i--;
		}
		else
		{
			bstrMenuText1 = bstrMenuText2;
			bstrMenuText2.Empty();
			pSublist1.Release();
			pSublist1 = pSublist2;
			pSublist2 = NULL;
		}
	}
	pMI1.Release();
	bstrMenuText1.Empty();
	pSublist1.Release();

	// Sort the sub-menus
	for (i = 0; i < cnt; i++)
	{
		// Sort one
		if (FAILED(hr = pMenuList->get_ObjectPropertyExt(i, IID_IMenuItem, (IObjectProperty**) &pMI1)) || !pMI1)
        	return hr;

		if (FAILED(hr = pMI1->get_SubMenu(&pSublist1)))
	        return hr;

		if (pSublist1)
		{
			SortMenuList(pSublist1);
			pSublist1.Release();
		}
		pMI1.Release();
	}

	// Insert separators
	if (FAILED(hr = pMenuList->get_ObjectPropertyExt(0, IID_IMenuItem, (IObjectProperty**) &pMI1)) || !pMI1)
	    return hr;

	pMI1->get_SortOrder(&lSortOrder1))
	    return hr;
	
    pMI1.Release();

    for (i = 1; i < cnt; i++)
	{
		// Get the next sort order & visibility
		if (FAILED(hr = pMenuList->get_ObjectPropertyExt(i, IID_IMenuItem, (IObjectProperty**) &pMI2)) || !pMI2)
	        return hr;

		pMI2->get_SortOrder(&lSortOrder2))
	        return hr;

		pMI2.Release();

		// Visible & crossed a 10,000s boundry?
		if ((lSortOrder1/10000) != (lSortOrder2/10000))
		{
			// Update the sort order
			lSortOrder1 = lSortOrder2;
			// Create and insert a separator
			if (FAILED(hr = CreateMenuSeparator(m_pWorld, NULL, &pMI2)))
        	    return ReportVWThingError(hr);

			if (FAILED(hr = pMenuList->InsertObjectPropertyAt(i, pMI2)))
	            return hr;

			cnt++;
			pMI2.Release();
			// Skip the separator we just added
			i++;
		}
	}

	return hr;
}
#endif

//******************************************************
// GetMenuItems

STDMETHODIMP CThingExemplarObject::GetMenuItems(IThing *pTarget, IPropertyList **ppMenuList)
{
	return GetMenuItemsFromProperty(CComBSTR("Menu"), ppMenuList);
}

//******************************************************
// GetPeerMenuItems

STDMETHODIMP CThingExemplarObject::GetPeerMenuItems(IThing *pPeer, IPropertyList **ppMenuList)
{
	return GetMenuItemsFromProperty(CComBSTR("PeerMenu"), ppMenuList);
}

//******************************************************
// GetContainerMenuItems

STDMETHODIMP CThingExemplarObject::GetContainerMenuItems(IThing *pTarget, IPropertyList **ppMenuList)
{
	return GetMenuItemsFromProperty(CComBSTR("ContainerMenu"), ppMenuList);
}

//******************************************************
// GetContentMenuItems

STDMETHODIMP CThingExemplarObject::GetContentMenuItems(IThing *pTarget, IPropertyList **ppMenuList)
{
	return GetMenuItemsFromProperty(CComBSTR("ContentMenu"), ppMenuList);
}

//******************************************************
// GetSelfMenuItems

STDMETHODIMP CThingExemplarObject::GetSelfMenuItems(IThing *pTarget, IPropertyList **ppMenuList)
{
	return GetMenuItemsFromProperty(CComBSTR("SelfMenu"), ppMenuList);
}

//******************************************************
// GetInventoryMenuItems

STDMETHODIMP CThingExemplarObject::GetInventoryMenuItems(IThing *pTarget, IPropertyList **ppMenuList)
{
	return GetMenuItemsFromProperty(CComBSTR("InventoryMenu"), ppMenuList);
}

//******************************************************
// GetContentMenuItems

STDMETHODIMP CThingExemplarObject::GetEditMenuItems(IThing *pTarget, IPropertyList **ppMenuList)
{
	return GetMenuItemsFromProperty(CComBSTR("EditMenu"), ppMenuList);
}

//******************************************************
// GetMenuItemsFromProperty

HRESULT CThingExemplarObject::GetMenuItemsFromProperty(BSTR bstrMenuListProperty,
													   IPropertyList **ppMenuList)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (ppMenuList==NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::GetMenuItemsFromPropery: NULL pointer\n");
        return ReportVWThingError(E_POINTER);
    }

	// Initialize
	*ppMenuList = NULL;

	CComPtr<IThing> pThis;
	CComPtr<IPropertyList> pListReal;
	CComPtr<IPropertyList> pListCopy;
	CComPtr<IMenuItem> pMIReal;
	CComPtr<IMenuItem> pMICopy;
	long lFlags = 0;
	long iItem;
	long cItem;
    HRESULT hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	// Lookup the MenuList property (OK if fails -- just means there's nothing to hand back)
	pThis->get_ObjectPropertyExt(bstrMenuListProperty, IID_IPropertyList,
												(IObjectProperty**) &pListReal);

	if (pListReal == NULL)
		return S_OK;

	// Create an empty list
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pListCopy)))
	    return ReportVWThingError(hr);

	// Clone the old list into it (note that IMenuItem->Duplicate does a deep copy)

	// Get count of items
	if (FAILED(hr = pListReal->get_Count(&cItem)))
    	return hr;

	for (iItem=0; iItem<cItem; iItem++)
	{
		// Get an item
		if (FAILED(hr = pListReal->get_ObjectPropertyExt(iItem, IID_IMenuItem,
													(IObjectProperty**) &pMIReal)))
            break;

		ASSERT(pMIReal);

		// Deep copy the item
		if (FAILED(hr = pMIReal->Duplicate(&pMICopy)))
	        break;

		ASSERT(pMICopy);

		// Add it into the list
		if (FAILED(hr = pListCopy->AddObjectProperty(pMICopy)))
            break;

		// release loop stuff
		pMIReal.Release();
		pMICopy.Release();
	}

	// Set & addref (extra since it will be released later)
	*ppMenuList = pListCopy;
	(*ppMenuList)->AddRef();

	return hr;
}

//******************************************************
// UpdateIsArtifact

STDMETHODIMP CThingExemplarObject::UpdateIsArtifact(IMenuItem *pmi)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pTarget;
	VARIANT_BOOL boolIsArtifact;
	long lFlags;
    HRESULT hr = S_OK;

    if (pmi == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::UpdateIsArtifact: NULL pointer\n");
        return ReportVWThingError(E_POINTER);
    }

	// Get the this object
	if (FAILED(hr = pmi->get_Target(&pTarget)))
	    return hr;

	// If there's no target, use the owner
	if (!pTarget)
    {
		if (FAILED(hr = pmi->get_Owner(&pTarget)))
	        return hr;
    }

	// Check if it's an Artifact
	if (FAILED(hr = pTarget->IsOfType(CComBSTR("Artifact"), &boolIsArtifact)))
	    return hr;

	// Get and set the invisible flag as indicated
	if (FAILED(hr = pmi->get_Flags(&lFlags)))
	    return hr;

	if (boolIsArtifact)
		lFlags &= ~MI_INVISIBLE;
	else
		lFlags |= MI_INVISIBLE;

	hr = pmi->put_Flags(lFlags);

	return hr;
}

//******************************************************
// UpdateIsAvatar

STDMETHODIMP CThingExemplarObject::UpdateIsAvatar(IMenuItem *pmi)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pTarget;
	VARIANT_BOOL boolIsAvatar;
	long lFlags;
    HRESULT hr = S_OK;

    if (pmi == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::UpdateIsAvatar: NULL pointer\n");
        return ReportVWThingError(E_POINTER);
    }

	// Get the this object
	if (FAILED(hr = pmi->get_Target(&pTarget)))
	    return hr;

	// If there's no target, use the owner
    if (!pTarget)
    {
		if (FAILED(hr = pmi->get_Owner(&pTarget)))
	        return hr;
    }

	// Check if it's an Avatar
	if (FAILED(hr = m_pWorld->IsAvatar(pTarget, &boolIsAvatar)))
	    return hr;

	// Get and set the invisible flag as indicated
	if (FAILED(hr = pmi->get_Flags(&lFlags)))
	    return hr;

	if (boolIsAvatar)
		lFlags &= ~MI_INVISIBLE;
	else
		lFlags |= MI_INVISIBLE;

	hr = pmi->put_Flags(lFlags);

	return hr;
}

//******************************************************
// UpdateIsAvatarAndNotWizard

STDMETHODIMP CThingExemplarObject::UpdateIsAvatarAndNotWizard(IMenuItem *pmi)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pTarget;
	VARIANT_BOOL boolIsAvatar;
	VARIANT_BOOL boolIsWizard;
	long lFlags;
    HRESULT hr = S_OK;

    if (pmi == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::UpdateIsAvatarAndNotWizard: NULL pointer\n");
        return ReportVWThingError(E_POINTER);
    }

    // Get the this object
	if (FAILED(hr = pmi->get_Target(&pTarget)))
	    return hr;

	// If there's no target, use the owner
	if (!pTarget)
    {
		if (FAILED(hr = pmi->get_Owner(&pTarget)))
	        return hr;
    }

	// Check if it's an Avatar
	if (FAILED(hr = pTarget->IsOfType(CComBSTR("Avatar"), &boolIsAvatar)))
	    return hr;

	// Check that it's not a Wizard
	if (boolIsAvatar)
    {
		if (FAILED(hr = pTarget->get_IsWizard(&boolIsWizard)))
	        return hr;
    }

	// Get and set the invisible flag as indicated
	if (FAILED(hr = pmi->get_Flags(&lFlags)))
	    return hr;

	if ((boolIsAvatar == VARIANT_TRUE) && (boolIsWizard == VARIANT_FALSE))
		lFlags &= ~MI_INVISIBLE;
	else
		lFlags |= MI_INVISIBLE;

	hr = pmi->put_Flags(lFlags);

	return hr;
}

//******************************************************
// UpdateIsArtifactOrAvatar


STDMETHODIMP CThingExemplarObject::UpdateIsArtifactOrAvatar(IMenuItem *pmi)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pTarget;
	VARIANT_BOOL boolIsArtifact;
	VARIANT_BOOL boolIsAvatar;
	long lFlags;
    HRESULT hr = S_OK;

    if (pmi == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::UpdateIsArtifactOrAvatar: NULL pointer\n");
        return ReportVWThingError(E_POINTER);
    }

	// Get the this object
	if (FAILED(hr = pmi->get_Target(&pTarget)))
	    return hr;

	// If there's no target, use the owner
	if (!pTarget)
    {
		if (FAILED(hr = pmi->get_Owner(&pTarget)))
	        return hr;
    }

	// Check if it's an Artifact
	if (FAILED(hr = pTarget->IsOfType(CComBSTR("Artifact"), &boolIsArtifact)))
	    return hr;

	// If not, check for Avatar 
	if (!boolIsArtifact)
    {
		if (FAILED(hr = m_pWorld->IsAvatar(pTarget, &boolIsAvatar)))
	        return hr;
    }

	// Get and set the invisible flag as indicated
	if (FAILED(hr = pmi->get_Flags(&lFlags)))
	    return hr;

	if (boolIsArtifact || boolIsAvatar)
		lFlags &= ~MI_INVISIBLE;
	else
		lFlags |= MI_INVISIBLE;

	hr = pmi->put_Flags(lFlags);

    return hr;
}

//******************************************************
// UpdateIsTakeable
// Take is displayed if:
//	Object is not avatar
//	Object has IsTakeable set to true
//  Object has CanChangeContainer set to true
// Note:
//	IsMovable is not checked, since it refers to position properties

STDMETHODIMP CThingExemplarObject::UpdateIsTakeable(IMenuItem *pmi)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pTarget;
	VARIANT_BOOL boolIsAvatar;
	VARIANT_BOOL boolIsTakeable;
	VARIANT_BOOL boolCanChangeContainer;
	long lFlags;
    HRESULT hr = S_OK;

    if (pmi == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::UpdateIsTakeable: NULL pointer\n");
        return ReportVWThingError(E_POINTER);
    }

	// Get the this object
	if (FAILED(hr = pmi->get_Target(&pTarget)))
	    return hr;

	// If there's no target, use the owner
	if (!pTarget)
    {
		if (FAILED(hr = pmi->get_Owner(&pTarget)))
	        return hr;
    }

	// Check if object is an Avatar 
	if (FAILED(hr = m_pWorld->IsAvatar(pTarget, &boolIsAvatar)))
	    return hr;

	// If not an avatar, check if the object is takeable
	if (!boolIsAvatar)
    {
		if (FAILED(hr = pTarget->get_BOOL(CComBSTR("IsTakeable"), &boolIsTakeable)))
	        return hr;

		if (FAILED(hr = pTarget->get_BOOL(CComBSTR("CanChangeContainer"), &boolCanChangeContainer)))
	        return hr;
    }
	
	// Get and set the invisible flag as indicated
	if (FAILED(hr = pmi->get_Flags(&lFlags)))
	    return hr;

	if ((!boolIsAvatar)&&(boolIsTakeable)&&(boolCanChangeContainer))
		lFlags &= ~MI_INVISIBLE;
	else
		lFlags |= MI_INVISIBLE;

	hr = pmi->put_Flags(lFlags);
	
    return hr;
}


//******************************************************
// UpdatePropertiesMenuItem
// Updates menu item "Properties..." to remove it if the user isn't an author

STDMETHODIMP CThingExemplarObject::UpdatePropertiesMenuItem(IMenuItem * pmi)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pUser;
	CComPtr<IThing> pGlobal;
	long lFlags;
    HRESULT hr = S_OK;
	VARIANT_BOOL vbAuthor = VARIANT_FALSE;
	VARIANT_BOOL vbSDKInstalled = VARIANT_FALSE;

    if (pmi == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::UpdatePropertiesMenuItem: NULL pointer\n");
        return ReportVWThingError(E_POINTER);
    }

	// Get the user
	if (FAILED(hr = m_pWorld->get_User(&pUser)))
        return hr;

	// get the global
	if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
		return hr;

	// get the existing flags
	if (FAILED(hr = pmi->get_Flags(&lFlags)))
        return hr;

	if (pUser)
    {
		if (FAILED(hr = pUser->get_BOOL(CComBSTR("IsAuthor"), &vbAuthor)))
            return hr;
    }

	if (pGlobal)
	{
		if (FAILED(hr = pGlobal->get_BOOL(CComBSTR("IsSDKInstalled"), &vbSDKInstalled)))
			return hr;
	}

	if (vbAuthor == VARIANT_FALSE || vbSDKInstalled == VARIANT_FALSE)
		lFlags |= MI_INVISIBLE;
	else
		lFlags &= ~MI_INVISIBLE;

	hr = pmi->put_Flags(lFlags);

	return hr;
}

//******************************************************
// UpdateMenuItemName
// Updates menu item "More about" to add name

STDMETHODIMP CThingExemplarObject::UpdateMenuItemName(IMenuItem * pmi)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pTarget;
	CComBSTR bstrName;
	CComBSTR bstrMenu;
    HRESULT  hr = S_OK;

    if (pmi == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::UpdateMenuItemName: NULL pointer\n");
        return ReportVWThingError(E_POINTER);
    }

	// Get the this object
	if (FAILED(hr = pmi->get_Target(&pTarget)))
	    return hr;

	// If there's no target, use the owner
	if (!pTarget)
    {
		if (FAILED(hr = pmi->get_Owner(&pTarget)))
	        return hr;
    }

	ASSERT(pTarget);

	// REVIEW: This is not internationalizable.

	// Now get name and add to menu
	if (FAILED(hr = pTarget->get_Name(&bstrName.m_str)))
	    return hr;

	if (FAILED(hr = pmi->get_MenuText(&bstrMenu.m_str)))
	    return hr;

	bstrMenu += bstrName;

	// Now add ... to the end
	bstrMenu+="...";
	
	hr = pmi->put_MenuText(bstrMenu);

    return hr;
}


//******************************************************
// UpdatePropertyListMenu

STDMETHODIMP CThingExemplarObject::UpdatePropertyListMenu(IMenuItem *pMI, IPropertyList *pList)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pOwner;
	BSTR bstrMethod = NULL;
	BSTR bstrItemName = NULL;
	CComPtr<IThing> pUser;
	CComPtr<IPropertyList> pMenuList;
	long iList, cList;
	CComPtr<IThing> pChild;
	CComPtr<IMenuItem> pSubMI;
    HRESULT hr = S_OK;

    if (pMI == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::UpdatePropertyListMenu: NULL pointer\n");
        return ReportVWThingError(E_POINTER);
    }

	// Get the owner
	if (FAILED(hr = pMI->get_Owner(&pOwner)))
	    return hr;

	// Get the user
	if (FAILED(hr = m_pWorld->get_User(&pUser)))
	    return hr;

	ASSERT(pUser);

	// Create the menu list
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pMenuList)))
    	return ReportVWThingError(hr);

	// Check that there's something to add
	if (pList)
    {
		// Add an entry for each item
		if (FAILED(hr = pList->get_Count(&cList)))
	        return hr;
    }
	else
		cList = 0;

	// Cycle through the list
	for (iList = 0; iList < cList; iList++)
	{
		if (FAILED(hr = pList->get_Thing(iList, &pChild)))
	        return hr;

		ASSERT(pChild);

		// Fill in the name
		if (FAILED(hr = pChild->get_Name(&bstrItemName)))
        	return hr;

		// Set the method
		if (FAILED(hr = pMI->get_Method(&bstrMethod)))
        	return hr;

		// Create the menu item
		if (FAILED(hr = CreateMenuItemExt(m_pWorld, bstrItemName, 0, pOwner, NULL, bstrMethod, 
                                          CComDISPPARAMS(1, CComVariant((IDispatch*)pChild)), 
                                          &pSubMI)))
	        return hr;

		SAFEFREESTRING(bstrItemName);
		SAFEFREESTRING(bstrMethod);
		pChild.Release();

		// Add it to the list
		if (FAILED(hr = pMenuList->AddObjectProperty(pSubMI)))
        	return hr;

		// Release the sub-menu
		pSubMI.Release();
	}

	// Now add it to the menu item sub-menu
	hr = pMI->put_SubMenu(pMenuList);

	return hr;
}

//******************************************************
// UpdateContentsMenu
// Updates the contents menu

STDMETHODIMP CThingExemplarObject::UpdateContentsMenu(IMenuItem *pMI)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThing;
	CComPtr<IPropertyList> pContents;
    HRESULT hr = S_OK;

    if (pMI == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::UpdateContentsMenu: NULL pointer\n");
        return ReportVWThingError(E_POINTER);
    }

	// Get the thing
	if (FAILED(hr = pMI->get_Target(&pThing)))
	    return hr;

	if (!pThing)
    {
		if (FAILED(hr = pMI->get_Owner(&pThing)))
	        return hr;
    }

	ASSERT(pThing);

	// Get the thing's contents
	if (FAILED(hr = pThing->get_Contents(&pContents)))
    	return hr;

	// Fill in the menu
	hr = UpdatePropertyListMenu(pMI, pContents);

    return hr;
}

//******************************************************
// UpdateRoomAvatarsMenu
// Updates the room avatars menu

STDMETHODIMP CThingExemplarObject::UpdateRoomAvatarsMenu(IMenuItem *pMI)
{
	CComPtr<IThing> pThing;
	CComPtr<IThing> pRoom;
	CComPtr<IPropertyList> pAvatars;
    HRESULT hr = S_OK;

    if (pMI == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::UpdateRoomAvatarsMenu: NULL pointer\n");
        return ReportVWThingError(E_POINTER);
    }

	// Get the thing
	if (FAILED(hr = pMI->get_Target(&pThing)))
		return hr;

	if (!pThing)
    {
		if (FAILED(hr = pMI->get_Owner(&pThing)))
		    return hr;
    }

	ASSERT(pThing);

	// Get the thing's room
	if (FAILED(hr = pThing->get_Container(&pRoom)))
    	return hr;

	// Get the avatars in the room
	if (FAILED(hr = pThing->get_ObjectProperty(CComBSTR("Avatars"),
											   (IObjectProperty **) &pAvatars)))
        return hr;

	// Fill in the menu
	hr = UpdatePropertyListMenu(pMI, pAvatars);

	return hr;
}

//******************************************************
// UpdateInventoryMenu
// Updates the inventory menu with the name of the
// object's contents.

STDMETHODIMP CThingExemplarObject::UpdateInventoryMenu(IMenuItem *pMI)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pUser;
	CComPtr<IPropertyList> pContents;
    HRESULT hr = S_OK;

    if (pMI == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::UpdateInventoryMenu: NULL pointer\n");
        return ReportVWThingError(E_POINTER);
    }

	// Get the user
	if (FAILED(hr = m_pWorld->get_User(&pUser)))
	    return hr;

	// Get the user's inventory
	if (FAILED(hr = pUser->get_Contents(&pContents)))
	    return hr;

	// Fill in the menu
	hr = UpdatePropertyListMenu(pMI, pContents);

	return hr;
}


//******************************************************
// DoNamedGesture
// Performs a gesture by name.  This default method
// does nothing, and always invokes gesture 0.

STDMETHODIMP CThingExemplarObject::DoNamedGesture(BSTR bstrGesture)
{
    HRESULT                 hr = S_OK;
	int						iGestureNum	= 0;
	CComPtr<IThing>			pThis;
	static CComBSTR			bstrDoGesture("DoGesture");

	if (SUCCEEDED(hr = m_pWorld->get_This(&pThis)))
	    // now, invoke the int-based method
	    hr = pThis->InvokeMethodExt(bstrDoGesture, CComDISPPARAMS(1, CComVariant(iGestureNum)), NULL);

	return hr;
}


//******************************************************
// DoGesture
// Performs a gesture

STDMETHODIMP CThingExemplarObject::DoGesture(int iGesture)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    HRESULT         hr = S_OK;
	CComPtr<IThing> pThis;
	CComPtr<IThing> pConnectedUser;
	static CComBSTR	bstrDoGestureServer("DoGestureServer");

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	if (FAILED(hr = m_pWorld->get_User(&pConnectedUser)))
	    return hr;

	// Call the server side method which will deal with doing the gesture on all other clients.
	// package up args into DISPPARAMS struct
	hr = pThis->InvokeMethodExt(bstrDoGestureServer, 
                                CComDISPPARAMS(2, CComVariant((IDispatch*) pConnectedUser),
                                                  CComVariant(iGesture)),
                                NULL);

	return hr;
}

//******************************************************
// DoGestureServer
// Performs the gesture on the server, which
// posts events and dispatches to the clients..

STDMETHODIMP CThingExemplarObject::DoGestureServer(IThing *pThingOriginator, int iGesture)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (pThingOriginator == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::DoGestureServer: NULL pointer\n");
        return ReportVWThingError(E_POINTER);
    }

	CComPtr<IThing> pThis;
	static CComBSTR	bstrDoGestureClient("DoGestureClient");
    HRESULT         hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

	// Call the client side method which will deal with doing the gesture on all other clients.
	// package up args into DISPPARAMS struct
	hr = pThis->InvokeMethodExt(bstrDoGestureClient, 
                                CComDISPPARAMS(2, CComVariant((IDispatch*) pThingOriginator),
                                                  CComVariant(iGesture)),
                                NULL);
	
	return hr;
}

//******************************************************
// DoGestureClient
// Performs the gesture on the client

STDMETHODIMP CThingExemplarObject::DoGestureClient(IThing *pThingOriginator, int iGesture)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// does nothing for now (should it generate some text?)

	return S_OK;
}


//******************************************************
// Take
// Take should succeed if:
//	object to be taken has the same container as object 
//	object is takeable - has IsTakeable == true OR
//  taker is object's owner OR
//  taker is wizard

HRESULT CThingExemplarObject::Take(IThing *pThing)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
	CComPtr<IThing> pCaller;
	CComPtr<IThing> pContainer;
	CComPtr<IThing> pThingsContainer;
	CComPtr<IThing> pOwner;
	VARIANT_BOOL bCheckIsTakeable = VARIANT_TRUE;  // assume you will have to do the check
	VARIANT_BOOL bIsTakeable = VARIANT_FALSE;      // default is not takeable
    HRESULT         hr = S_OK;

    if (pThing == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::Take: NULL pointer\n");
        return ReportVWThingError(E_POINTER);
    }

	// Get the this
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	// Get caller
	if (FAILED(hr = m_pWorld->get_Caller(&pCaller)))
	    return hr;

	// Get the container
	if (FAILED(hr = pThis->get_Container(&pContainer)))
	    return hr;

	// Get the container of the thing
	if (FAILED(hr = pThing->get_Container(&pThingsContainer)))
	    return hr;

	// Get owner of the thing
	if (FAILED(hr = pThing->get_Owner(&pOwner)))
	    return hr;


#ifdef _DEBUG
	CComBSTR bstrCallerName, bstrOwnerName;
	if (pCaller)
    {
		if (FAILED(hr = pCaller->get_Name(&bstrCallerName.m_str)))
	        return hr;
    }

	if (pOwner)
    {
		if (FAILED(hr = pOwner->get_Name(&bstrOwnerName.m_str)))
	        return hr;
    }
#endif

	// Can only take things in my container.
	if (pThingsContainer != pContainer)
    {
    
        VWTRACE(m_pWorld, "COREEXEMP", VWT_IMPORTANT, "CThingExemplarObject::Take: Cannot take object -- not in container\n");
        return ReportVWThingError(VWTHING_E_TAKENOTINCONTAINER);
    }

	// Get IsTakeable (shouldn't do the move if IsTakeable is false)
	if (FAILED(hr = pThing->get_BOOL(CComBSTR("IsTakeable"), &bIsTakeable)))
    	return hr;

	// If the object isn't takeable, check for special cases in which we 
	// want to override this property

	if ((bIsTakeable == VARIANT_FALSE) && (bCheckIsTakeable == VARIANT_TRUE))
	{	
		{
			// Is the caller the owner of the object?
			if (pOwner)
			{
				if (pCaller==pOwner)
					bCheckIsTakeable = VARIANT_FALSE;
			}
				
			// Now if caller != owner, check if wizard
			if (bCheckIsTakeable == VARIANT_TRUE)
			{
				VARIANT_BOOL bIsAvatar;
				VARIANT_BOOL bIsWizard = VARIANT_FALSE;

				if (FAILED(hr = pCaller->IsOfType(CComBSTR("Avatar"), &bIsAvatar)))
	                return hr;

				if (bIsAvatar)
				{
					if (FAILED(hr = pCaller->get_IsWizard(&bIsWizard)))
	                    return hr;

					if (bIsWizard == VARIANT_TRUE)
						bCheckIsTakeable = VARIANT_FALSE;
				}
			}
		}
	}

	// Call MoveInto if either the object is takeable or one of the other conditions is met
    if ((bIsTakeable == VARIANT_TRUE) || (bCheckIsTakeable == VARIANT_FALSE))
    {
	    // Do it - relying on MoveInto for security.
	    hr = pThing->InvokeMethodExt(CComBSTR("MoveInto"),
						            CComDISPPARAMS(1, CComVariant((IDispatch*)pThis)),
						            NULL);
		return hr;
    }
	else
        return ReportVWThingError(VWTHING_E_TAKENOTINCONTAINER);


}

//******************************************************
// Drop
HRESULT CThingExemplarObject::Drop(IThing *pThing)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
	CComPtr<IThing>	pContainer, pThingsContainer;
    HRESULT         hr = S_OK;

    if (pThing == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::Drop: NULL pointer\n");
        return ReportVWThingError(E_POINTER);
    }

	// Get the this
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	// Get the room
	if (FAILED(hr = pThis->get_Container(&pContainer)))
	    return hr;

	// Get the container of the thing
	if (FAILED(hr = pThing->get_Container(&pThingsContainer)))
	    return hr;

	// Can only drop things in my contents.
	if (pThingsContainer != pThis)
    {
    
        VWTRACE(m_pWorld, "COREEXEMP", VWT_IMPORTANT, "CThingExemplarObject::Drop: Cannot drop object -- not in contents\n");
        return ReportVWThingError(VWTHING_E_DROPNOTINCONTENTS);
    }
    else
    {
	    // Do it - relying on MoveInto for security.
	    hr = pThing->InvokeMethodExt(CComBSTR("MoveInto"),
						        CComDISPPARAMS(1, CComVariant((IDispatch*)pContainer)),
						        NULL);
    }

	return hr;
}

//******************************************************
// EditObject
HRESULT CThingExemplarObject::EditObject()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
    HRESULT  hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	// Bring up the editor
	hr = m_pWorld->FireUIEvent(pThis, CComBSTR(VW_EDITOBJECT_EVENT_STR), CComVariant(pThis));
    return hr;
}

HRESULT CThingExemplarObject::MergePropertyMaps(IPropertyMap *pFrom, IPropertyMap *pTo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr = S_OK;

	// adds the elements in pFrom to pTo, ignoring conflicts
	BSTR bstrReturnValue = NULL;
	VARIANT_BOOL bLastItem;
	VARIANT_BOOL bValid;
	COleVariant varProperty;

	if (FAILED(hr = pFrom->get_IsEmpty(&bLastItem)))
    	return hr;
	
	if (bLastItem!=VARIANT_TRUE)
	{
		if (SUCCEEDED(hr = pFrom->FirstItem(&bstrReturnValue, &varProperty, &bLastItem)))
        {
		    while (bLastItem!=VARIANT_TRUE)
		    {
			    // check to see if we can add the prop to the pTo
			    if (FAILED(hr = pTo->IsValid(bstrReturnValue, &bValid)))
	                break;

			    if (bValid == VARIANT_FALSE)
                {
				    // add it to the pToList
				    if (FAILED(hr = pTo->put_Property(bstrReturnValue, varProperty)))
	                    break;
                }
		    
                SAFEFREESTRING(bstrReturnValue);

			    if (FAILED(hr = pFrom->NextItem(&bstrReturnValue, &varProperty, &bLastItem)))
	                break;
		    }
		    SAFEFREESTRING(bstrReturnValue);
        }
	}

	return hr;
}

HRESULT CThingExemplarObject::MergePropertyLists(IPropertyList *pFrom, IPropertyList *pTo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr = S_OK;

	// adds the elements in pFrom to pTo, ignoring conflicts
	BSTR bstrReturnValue = NULL;
	VARIANT_BOOL bLastItem;
	VARIANT_BOOL bLastToItem;
	CComVariant varFromProperty;
	CComVariant varToProperty;
	CComBSTR	bstrFrom;
	CComBSTR	bstrTo;
	CString		strFrom;
	CString		strTo;
	BOOL bCanAdd = TRUE;

	if (FAILED(hr = pFrom->get_IsEmpty(&bLastItem)))
    	return hr;
	
	if ( bLastItem!=VARIANT_TRUE )
	{
		if (FAILED(hr = pFrom->FirstItem(&bstrReturnValue, &varFromProperty, &bLastItem)))
	        return hr;
		
		while (bLastItem!=VARIANT_TRUE)
		{
			// check to see if we can add the item to the pTo
			if (varFromProperty.vt == VT_BSTR)
			{
				bstrFrom = varFromProperty.bstrVal;
				strFrom = bstrFrom;
				
				if (!strFrom.IsEmpty())
				{
					// walk the to list and make sure that there are no other menu items with the same text
					bCanAdd = TRUE;
					if (FAILED(hr = pTo->get_IsEmpty(&bLastToItem)))
	                    return hr;

					if ( bLastToItem!=VARIANT_TRUE )
					{
						if (FAILED(hr = pTo->FirstItem(&bstrReturnValue, &varToProperty, &bLastToItem)))
	                        return hr;
						
						while ((bLastToItem!=VARIANT_TRUE) && bCanAdd)
						{
							if (varToProperty.vt == VT_BSTR)
							{
								bstrTo = varToProperty.bstrVal;
								strTo = bstrTo;

								if (!(strTo.Left(strTo.Find('|'))).CompareNoCase(strFrom.Left(strFrom.Find('|'))))
									bCanAdd = FALSE;
							}
							if (FAILED(hr = pTo->NextItem(&bstrReturnValue, &varToProperty, &bLastToItem)))
	                            return hr;
						}
					}
				}
			}
			
			if (bCanAdd)
			{
				// add it to the pToList
				if (FAILED(hr = pTo->Add(varFromProperty)))
	                return hr;
			}

			SAFEFREESTRING(bstrReturnValue);

			if (FAILED(hr = pFrom->NextItem(&bstrReturnValue, &varFromProperty, &bLastItem)))
	            return hr;
		}
		SAFEFREESTRING(bstrReturnValue);
	}

	return hr;
}

HRESULT CThingExemplarObject::AppendPropertyMap(IThing *pThis, BSTR bstrPropName, IPropertyMap *pMap)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// walks up the exemplar chain, getting a property map out of each exemplar
	// and appending it to the end of the current property map
	CComPtr<IThing> pExemplar;
	CComPtr<IPropertyMap> pToMerge;
	CComPtr<IPropertyMap> pProperties;
	HRESULT hr = S_OK;

    ASSERT(pThis);

	if (FAILED(hr = pThis->get_Properties(&pProperties)))
    	return hr;

	if (pProperties==NULL)
        return VWTHING_E_UNKNOWN;
    
	hr = pProperties->get_ObjectProperty(bstrPropName, (IObjectProperty**) &pToMerge);
	if (FAILED(hr) && hr!=VWOBJECT_E_PROPERTYNOTEXIST)
        return hr;

	if (pToMerge)
    {
		if (FAILED(hr = MergePropertyMaps(pToMerge, pMap)))
	        return hr;
    }

	if (FAILED(hr = pThis->get_Exemplar(&pExemplar)))
	    return hr;

	pToMerge.Release();
	pProperties.Release();

	while (pExemplar != NULL)
	{
		CComPtr<IThing> pExemplar2;
	
		// should have a way to shortcut this in case we are inheriting
		if (FAILED(hr = pExemplar->get_Properties(&pProperties)))
	        return hr;
	
		if (pProperties)
		{
			hr = pProperties->get_ObjectProperty(bstrPropName, (IObjectProperty**) &pToMerge);
			if (FAILED(hr) && hr!=VWOBJECT_E_PROPERTYNOTEXIST)
				return hr;

			if (pToMerge)
            {
				if (FAILED(hr = MergePropertyMaps(pToMerge, pMap)))
	                return hr;
            }
		}

		if (FAILED(hr = pExemplar->get_Exemplar(&pExemplar2)))
	        return hr;

		pExemplar = pExemplar2;

		pToMerge.Release();
		pProperties.Release();;
	}

	return hr;
}

HRESULT CThingExemplarObject::AppendPropertyList(IThing *pThis, BSTR bstrPropName, IPropertyList *pMenu)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// walks up the exemplar chain, getting a property map out of each exemplar
	// and appending it to the end of the current property map

	CComPtr<IThing> pExemplar;
	CComPtr<IPropertyList> pToMerge;
	CComPtr<IPropertyMap> pProperties;
	HRESULT hr = S_OK;

    ASSERT(pThis);

	if (FAILED(hr = pThis->get_Properties(&pProperties)))
    	return hr;
	
    if (pProperties==NULL)
        return VWTHING_E_UNKNOWN;

	hr = pProperties->get_ObjectProperty(bstrPropName, (IObjectProperty**) &pToMerge);
	if (FAILED(hr) && hr!=VWOBJECT_E_PROPERTYNOTEXIST)
		return hr;

	if (pToMerge)
    {
		if (FAILED(hr = MergePropertyLists(pToMerge, pMenu)))
	        return hr;
    }

	if (FAILED(hr = pThis->get_Exemplar(&pExemplar)))
	    return hr;

	pToMerge.Release();
	pProperties.Release();

	while (pExemplar != NULL)
	{
		CComPtr<IThing> pExemplar2;
	
		// should have a way to shortcut this in case we are inheriting
		if (FAILED(hr = pExemplar->get_Properties(&pProperties)))
	        return hr;
	
		if (pProperties)
		{
			hr = pProperties->get_ObjectProperty(bstrPropName, (IObjectProperty**) &pToMerge);
			if (FAILED(hr) && hr!=VWOBJECT_E_PROPERTYNOTEXIST)
				break;

			if (pToMerge)
            {
				if (FAILED(hr = MergePropertyLists(pToMerge, pMenu)))
	                break;
            }
		}

		if (FAILED(hr = pExemplar->get_Exemplar(&pExemplar2)))
	        break;

		pExemplar = pExemplar2;

		pToMerge.Release();
		pProperties.Release();;
	}

	return hr;
}

HRESULT CThingExemplarObject::GetSortedEditTabMap(IPropertyList **ppMap)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (ppMap==NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_VERBOSE, "CThingExemplarObject::GetSortedEditTabMap: NULL output pointer\n");
        return ReportVWThingError(E_POINTER);
    }

	CComPtr<IThing> pThis;
	CComVariant varReturn;
	CPropListPtr pPropList;
    HRESULT hr = S_OK;

	// Get This
	if (SUCCEEDED(hr = m_pWorld->get_This(&pThis)))
    {
	    if (SUCCEEDED(hr = pThis->InvokeMethodExt(CComBSTR("GetEditTabMap"), NULL, &varReturn)))
        {
	        // get the IPropList out of the Var
	        pPropList = varReturn;
	        if (pPropList)
	        {
		        // walk the proplist and remove any items that have empty values

		        *ppMap = (IPropertyList *) pPropList;
		        SAFEADDREF(*ppMap);
        //			if (FAILED(hr = SortMenuList(*ppMap)))
        //	            goto ERROR_ENCOUNTERED;
	        }
	        else
		        *ppMap = NULL;
        }
    }

	return hr;
}

HRESULT CThingExemplarObject::GetEditTabMap(IPropertyList **ppMap)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (ppMap==NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_VERBOSE, "CThingExemplarObject::GetEditTabMap: NULL output pointer\n");
        return ReportVWThingError(E_POINTER);
    }

	CComPtr<IThing> pThis;
	CComBSTR bstrEditMapName;
	CComPtr<IThing> pUser;
	VARIANT_BOOL vbOwner = VARIANT_FALSE;
	VARIANT_BOOL vbAuthor = VARIANT_FALSE;
	VARIANT_BOOL vbWizard = VARIANT_FALSE;
	CComPtr<IThing> pOwner;
	CComPtr<IPropertyList> pMenu;
    HRESULT hr = S_OK;

	bstrEditMapName = "EndUserEditMap";

	// Get This
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	// get the user
	if (FAILED(hr = m_pWorld->get_User(&pUser)))
	    return hr;

	// is the user a wizard?
	if (FAILED(hr = pUser->get_IsWizard(&vbWizard)))
	    return hr;

	if (vbWizard == VARIANT_TRUE)
		bstrEditMapName = "AdvancedEditMap";
	else
	{
		// is the user an author?
		if (FAILED(hr = pUser->get_BOOL(CComBSTR("IsAuthor"), &vbAuthor)))
	        return hr;

		if (vbAuthor == VARIANT_TRUE)
			bstrEditMapName = "BuilderEditMap";
		else
		{
			// is the user the owner of the object?
			if (FAILED(hr = pThis->get_Owner(&pOwner)))
	            return hr;
			
			if (pOwner == pUser)
				bstrEditMapName = "OwnerEditMap";
		}
	}

	if (FAILED(hr = CreatePropertyList(m_pWorld, ppMap)))
	    return ReportVWThingError(hr);

	hr = AppendPropertyList(pThis, bstrEditMapName, *ppMap);

	hr = SortEditMap(*ppMap);
    return hr;
}

//REVIEW: I am a hack.  This function exists to get the order edit map inline with
//the spec.  In V2 a better mechanism should be used. (MM)

HRESULT CThingExemplarObject::SortEditMap(IPropertyList* pEditMap)
{
	BSTR bstrReturnValue = NULL;
	VARIANT_BOOL bLastItem;
	CComVariant varProperty;
	CComBSTR	bstrName;
	CString		strName, strFull;
	int i, nCount, nProp;
	HRESULT hr = S_OK;

	CString szPreferedEditMapOrder[] = { "General", "Geometry", "Texture", "Approach", "Motion" };
	nCount = sizeof(szPreferedEditMapOrder) / sizeof(szPreferedEditMapOrder[0]);

	if (FAILED(hr = pEditMap->get_IsEmpty(&bLastItem)))
	    return hr;

	if ( bLastItem != VARIANT_TRUE )
	{
		for (i = nCount - 1; i >= 0; i--)
		{ 
			nProp = 0;
			if (FAILED(hr = pEditMap->FirstItem(&bstrReturnValue, &varProperty, &bLastItem)))
				return hr;
			
			while ( (bLastItem != VARIANT_TRUE) )
			{
				if (varProperty.vt == VT_BSTR)
				{
					bstrName = varProperty.bstrVal;
					strFull = bstrName;

					strName = strFull.Left(strFull.Find('|'));

					if (strName.Compare(szPreferedEditMapOrder[i]) == 0)
					{
						pEditMap->RemoveAt(nProp);
						pEditMap->InsertStringAt(0, bstrName);
						break; //Kick out of while loop.
					}
				}
				if (FAILED(hr = pEditMap->NextItem(&bstrReturnValue, &varProperty, &bLastItem)))
					return hr;

				nProp++;
			}
		}
	}

	return hr;
}


// REVIEW: What's this method doing? Seems to check just a subset of things [MV]
HRESULT CThingExemplarObject::CheckSecurity( )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pUser;
	CComPtr<IThing> pCaller;
	VARIANT_BOOL bServerSide;
    HRESULT hr = S_OK;

	if (FAILED(hr = m_pWorld->get_ServerSide(&bServerSide)))
        return hr;

	if (bServerSide==VARIANT_TRUE )
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_VERBOSE, "CThingExemplarObject::CheckSecurity: Don't check security on server side\n");
		return ReportVWThingError(VWTHING_E_NOSECURITYONSERVER);
    }

	// Get Caller
	if (SUCCEEDED(hr = m_pWorld->get_Caller(&pCaller)))
    {
	    // Get User
	    if (SUCCEEDED(hr = m_pWorld->get_User(&pUser)))
        {
	        // test to see if caller == user or caller == NULL
	        if (pCaller!=pUser)
	        {
		        if (pCaller)
                {
                    VWTRACE(m_pWorld, "COREEXEMP", VWT_VERBOSE, "CThingExemplarObject::CheckSecurity: Caller is not user\n");
		            return ReportVWThingError(VWTHING_E_NOSECURITYFORUNKNOWNUSER);
                }
	        }
        }
    }

	return hr;
}

// REVIEW: seems like this is just checking for the client bit [MV]
HRESULT CThingExemplarObject::CheckPropertyBits(IThing *pThis, BSTR bstrName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IPropertySecurity> pSecurity;
	VARIANT_BOOL bClientProp;
    HRESULT hr = S_OK;

    if (pThis == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_VERBOSE, "CThingExemplarObject::CheckPropertyBits: NULL output pointer\n");
        return ReportVWThingError(E_POINTER);
    }

	// Test to see if it is a client property
	if (SUCCEEDED(hr = pThis->get_PropertySecurity( bstrName, &pSecurity)))
    {
	    if (SUCCEEDED(hr = pSecurity->get_BitState(PSBIT_CLIENTPROPERTY, &bClientProp)))
        {
	        if (bClientProp!=VARIANT_TRUE )
	        {
                VWTRACE(m_pWorld, "COREEXEMP", VWT_VERBOSE, "CThingExemplarObject::CheckPropertyBits: not a client property\n");
		        return ReportVWThingError(VWTHING_E_NOTACLIENTPROPERTY);
            }
	    }
    }

	return hr;
}


// Client Only property support
HRESULT CThingExemplarObject::AddClientProperty(BSTR bstrName,VARIANT var)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    HRESULT         hr = S_OK;
	CComPtr<IThing> pThis;
	CComPtr<IThing> pExemplar;
	CComPtr<IPropertySecurity> pSecurity;
	CComPtr<IPropertySecurity> pContext;

	if (FAILED(hr = CheckSecurity()))
	    return hr;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	if (FAILED(hr = pThis->AddPropertyExt(bstrName, var, PSBIT_LOCAL | PSBIT_HIDDEN | PSBIT_CLIENTPROPERTY, PS_EXEMPLARDEFAULTPROPERTY, VT_VARIANT, IID_NULL, NULL)))
	    return hr;

	// Now lets patch up security a bit, so that we set the property's security to
	// the exemplar we are defined on and set the permissions to exemplar access only
	if (FAILED(hr = m_pWorld->get_SecurityContext(&pContext)))
	    return hr;
	
	// Get my Exemplar
	if (FAILED(hr = pContext->get_Exemplar( &pExemplar )))
	    return hr;

	if (FAILED(hr = pThis->get_PropertySecurity( bstrName, &pSecurity )))
	    return hr;

	if (FAILED(hr = pSecurity->put_Exemplar( pExemplar )))
	    return hr;

	hr = pSecurity->put_Permissions(0x00010b01 );

	return hr;
}

HRESULT CThingExemplarObject::RemoveClientProperty(BSTR bstrName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
    HRESULT         hr = S_OK;

	if (FAILED(hr = CheckSecurity()))
	    return hr;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	// Test to see if it is a client property
	if (FAILED(hr = CheckPropertyBits(pThis,bstrName)))
	    return hr;

	hr = pThis->RemoveProperty( bstrName );

    return hr;
}

STDMETHODIMP CThingExemplarObject::OnSystemDestroyClient(IThing* pthing)
{
	VARIANT_BOOL bLocalMode = VARIANT_FALSE;

	if (pthing == NULL)
		return E_POINTER;

	m_pWorld->get_LocalMode(&bLocalMode);

	// REVIEW: do in local mode, since server will have already done it
	m_pWorld->put_LocalMode(VARIANT_TRUE);

	/* hr = */ pthing->MoveIntoInt(NULL);

	m_pWorld->put_LocalMode(bLocalMode);

	// NOTE: this is a client-side method to remove objects from clients after they've been destroyed
	// otherwise they'll sit around in a NULL container, but are still accessible by reference

	// actually delete object from VWODB (eat error, since we aren't returning it to any one)
	/* hr = */ m_pWorld->DeleteObjectInt(pthing);

	// mark as stub
	/* hr = */ pthing->put_IsStubInt(VARIANT_TRUE);

	return S_OK;
}

STDMETHODIMP CThingExemplarObject::OnContentSystemEnter(IThing *pWhere, IThing *pWhat)
{
	// empty method: needed for VWCLIOCX
	return S_OK;
}

STDMETHODIMP CThingExemplarObject::OnContentSystemLeave(IThing *pWhere, IThing *pWhat)
{
	// empty method: needed for VWCLIOCX
	return S_OK;
}


//********************************************************
// Policy accessor methods

//********************************************************
// OnIsNoticeableAccess
// Checks policy:
// User is owner, user is wizard, or  caller & container are same

STDMETHODIMP CThingExemplarObject::OnIsNoticeableAccess(long lPerms, VARIANT_BOOL* bReturnValue)
{
	HRESULT hr = S_OK;
	CComPtr<IThing> pThis;
	CComPtr<IThing> pCaller;
	CComPtr<IThing> pContainer;
	VARIANT_BOOL bOk;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// If you have a pointer, set return value false by default
	if (bReturnValue == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::OnIsNoticeableAccess: NULL output pointer\n");
		return ReportVWThingError(E_POINTER);
    }

	if (lPerms != PS_WRITE)
	{
		*bReturnValue = VARIANT_TRUE;
		return S_OK;
	}
	else
		*bReturnValue = VARIANT_FALSE;

	// get this
	hr = m_pWorld->get_This(&pThis);
	if (FAILED(hr))
		return hr;

	// Check policy (user must be wizard or caller must be container of object)
	hr = m_pWorld->CheckPolicy(pThis, CP_OWNER | CP_WIZARD, CComVariant(0L, VT_ERROR), &bOk);
	if (FAILED(hr))
		return hr;

	if (bOk!=VARIANT_TRUE)
	{
		// get the container
		hr = pThis->get_Container(&pContainer);
		if (FAILED(hr))
			return hr;

		// get the caller
		hr = m_pWorld->get_Caller(&pCaller);
		if (FAILED(hr))
			return hr;

		if (pContainer==pCaller)
			bOk = VARIANT_TRUE;
	}

	// It's OK, return true and S_OK
	if (bOk==VARIANT_TRUE)
		*bReturnValue = VARIANT_TRUE;

	return hr;
}


//********************************************************
// OnNameAccess
// Checks policy before allowing name changes
// Avatars aren't allowed to change their names
// If not an avatar, user must be owner or wizard

STDMETHODIMP CThingExemplarObject::OnNameAccess(long lPerms, VARIANT_BOOL* bReturnValue)
{
	HRESULT hr = S_OK;
	CComPtr<IThing> pThis;
	CComPtr<IThing> pUser;
	VARIANT_BOOL bOk;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// If you have a pointer, set return value false by default
	if (bReturnValue == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::OnNameAccess: NULL output pointer\n");
		return ReportVWThingError(E_POINTER);
    }

	if (lPerms != PS_WRITE)
	{
		*bReturnValue = VARIANT_TRUE;
		return S_OK;
	}
	else
		*bReturnValue = VARIANT_FALSE;

	// get this
	hr = m_pWorld->get_This(&pThis);
	if (FAILED(hr))
		return hr;

	hr = m_pWorld->get_User(&pUser);
	if (FAILED(hr))
		return hr;

	// If This is an avatar, fail
	if ((pThis) && (pUser))
	{
		VARIANT_BOOL bIsAvatar;
		if (FAILED(hr = pThis->IsOfType(CComBSTR("Avatar"), &bIsAvatar)))
    		return hr;
		if (bIsAvatar)
        {
            VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::OnNameAccess: avatar object cannot access\n");
			// KM: don't fail, just return VARIANT_FALSE
		    return S_OK;
        }
	}

	// Check policy - User must be owner, wizard,
	// or caller's owner must be the same as the owner - 
	// case of vending machine or another object that creates objects)
	hr = m_pWorld->CheckPolicy(pThis, CP_OWNER | CP_WIZARD | CP_CALLER, CComVariant(0L, VT_ERROR), &bOk);
	if (FAILED(hr))
		return hr;

	// OK, return true and S_OK
	if (bOk==VARIANT_TRUE)
		*bReturnValue = VARIANT_TRUE;

	return hr;
}



//********************************************************
// OnOwnerOrWizardAccess
// Checks policy before setting of a property 
// (Description, IsTakeable, IsAccessory)
// User must be owner or wizard

STDMETHODIMP CThingExemplarObject::OnOwnerOrWizardAccess(long lPerms, VARIANT_BOOL* bReturnValue)
{
	HRESULT hr = S_OK;
	CComPtr<IThing> pThis;
	CComPtr<IThing> pCaller;
	VARIANT_BOOL bOk;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// If you have a pointer, set return value false by default
	if (bReturnValue == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CThingExemplarObject::OnOwnerOrWizardAccess: NULL output pointer\n");
		return ReportVWThingError(E_POINTER);
    }

	if (lPerms != PS_WRITE)
	{
		*bReturnValue = VARIANT_TRUE;
		return S_OK;
	}
	else
		*bReturnValue = VARIANT_FALSE;

	// get this
	hr = m_pWorld->get_This(&pThis);
	if (FAILED(hr))
		return hr;

	hr = m_pWorld->get_Caller(&pCaller);
	if (FAILED(hr))
		return hr;

#ifdef _DEBUG
	// No error checking, if we fail in these traces, we still want to keep going
	CComPtr<IThing> pOwner, pUser;
	CComBSTR bstrThisName, bstrOwnerName, bstrCallerName, bstrUserName;
	if (pThis)
	{
		pThis->get_Name(&bstrThisName.m_str);
		pThis->get_Owner(&pOwner);

		if (pOwner)
			pOwner->get_Name(&bstrOwnerName.m_str);
	}
 	
    if (pCaller)
		pCaller->get_Name(&bstrCallerName.m_str);

	m_pWorld->get_User(&pUser);
	if (pUser)
		pUser->get_Name(&bstrUserName.m_str);
#endif

	// Check policy (user must be owner or wizard)
	if (FAILED(hr = m_pWorld->CheckPolicy(pThis, CP_OWNER | CP_CALLER | CP_WIZARD, CComVariant(0L, VT_ERROR), &bOk)))
        return hr;

	// OK, return true and S_OK
	if (bOk==VARIANT_TRUE)
		*bReturnValue = VARIANT_TRUE;

	// Else check for case of user setting name, description on self
	if (pCaller == pThis)
		*bReturnValue = VARIANT_TRUE;

    return hr;
}

STDMETHODIMP CThingExemplarObject::OnIsOpenChanged(long lHint, VARIANT varHintData)
{
	CComPtr<IThing>	pThis;
	VARIANT_BOOL	bIsOpen;
	static CComBSTR	bstrIsOpen("IsOpen");
	HRESULT hr = S_OK;
	VARIANT_BOOL bConnected = VARIANT_FALSE;

	// ignore add/remove
	if (lHint != thPut)
		return S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	if (FAILED(hr = pThis->get_BOOL(bstrIsOpen, &bIsOpen)))
		return hr;

	// If setting IsOpen to TRUE (and therefore adding bystanders), must make sure all 
	// the new bystanders have this.
	//TO: add check for m_bConnected..  this is a ConnectLocal case.. we already have the 
	// data in this case and SendContents will error with "shouldn't be called from client"
	hr = m_pWorld->get_IsConnected(&bConnected);
	if (bIsOpen && (bConnected == VARIANT_TRUE))
	{
		if (FAILED(hr = m_pWorld->SendContents(soBystanders, pThis, VARIANT_FALSE)))
			return hr;
	}
	return hr;
}

