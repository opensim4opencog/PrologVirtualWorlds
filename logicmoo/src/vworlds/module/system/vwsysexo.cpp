// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// vwsysexo.cpp : Implementation of CSystemExemplarObject
#include "stdafx.h"
#include <mmsystem.h> // for timeGetTime
#include <vwsysex.h>
#include "vwsysexo.h"
#include "vwutils.h"
#include "syshelp.h"
#include <wpapi.h>
#include <reghelp.h>
#include <vwevents.h>
#include <vwanim.h>
#include <inetfile.h>
#include <worldobj.h>

//#define TRACE_TIME VWT_METHOD
#define TRACE_TIME VWT_NONE

// KM: bumped to 50 to allow rooms to be built by default
#define lDefaultObjectQuota	50L // 10L

#define SUPPORT_SECURITY_OVERRIDE

#define TIME_GETPROPERTY
#define TIME_GETPROPERTYSECURITY
#define TIME_PUTPROPERTY
#define TIME_GETMETHOD
#define TIME_INVOKEMETHOD
#define TIME_CHECKPROPERTYSECURITY
#define TIME_CHECKMETHODSECURITY
#define TIME_CHECKPROPERTIESSECURITY
#define TIME_CHECKMETHODSSECURITY
#define TIME_NOTIFYCLIENT
#define TIME_SENDCOMMAND
#define TIME_PROCESSMESSAGE

//******************************************************
// Install
// Installs this module - creates the exemplar and
// adds the properties and methods.

STDMETHODIMP CSystemExemplarObject::Install(IModule* pModule)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr = S_OK;
	CComPtr<IThing> pGlobal;
	CComBSTR bstrName;
	CComBSTR bstrNameInternal;

	CMethodInfo rgMethod[] =
	{
		{ METHOD_CLIENT | METHOD_SERVER,	"CreatePropertyList",		NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"CreatePropertyMap",		NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"CreateMenuItem",			NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"CreateMenu",				NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"CreateMenuSeparator",		NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"CreateOCXProxy",			NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_SERVER | METHOD_SYNC,		"QueryAvatarNames",			NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_SERVER,					"InvokeRemoteClientMethod",	NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_CLIENT,					"HandleRemoteClientMethod",	NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_SERVER,					"FireRemoteUIEvent",		NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_CLIENT,					"FireUIEvent",				NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_SERVER,					"OnRootURLChanged",			NULL,	PSBIT_SYSTEMEVENTHANDLER, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,					"OnRootURLChangedClient",	"OnRootURLChanged", PSBIT_SYSTEMEVENTHANDLER, PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER,					"OnWorldNameChanged",		"OnRootURLChanged",	PSBIT_SYSTEMEVENTHANDLER, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,					"UploadFile",				NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_SERVER,					"MakeHost",					NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER,					"UnMakeHost",				NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_ALLEXECUTEMETHOD },	
		{ METHOD_CLIENT | METHOD_SERVER,	"CreateTimer",				NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_GLOBALDEFAULTMETHOD },	
		{ METHOD_CLIENT | METHOD_SERVER,	"DestroyTimer",				NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_CLIENT,					"OnCreateRoomClient",		NULL,	PSBIT_SYSTEMEVENTHANDLER,	PS_EVENTHANDLER },
		{ METHOD_CLIENT,					"ShowWelcomeMessage",		NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_GLOBALDEFAULTMETHOD },
		// World Owner Access methods
		{ METHOD_CLIENT | METHOD_SERVER,	"OnWorldOwnerAccess",		NULL,	PSBIT_GLOBALDEFAULTMETHOD| PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD  },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnWorldDescriptionAccess", "OnWorldOwnerAccess",	PSBIT_GLOBALDEFAULTMETHOD, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnRootURLAccess",			"OnWorldOwnerAccess",	PSBIT_GLOBALDEFAULTMETHOD, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnEnableSecurityAccess",	"OnWorldOwnerAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },	
		{ METHOD_CLIENT | METHOD_SERVER,	"OnNumberOfUsersAccess",	"OnWorldOwnerAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnNumberOfObjectsAccess",	"OnWorldOwnerAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },	
		{ METHOD_CLIENT | METHOD_SERVER,	"OnNumberOfAvatarsAccess",	"OnWorldOwnerAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnDefaultObjectQuotaAccess",	"OnWorldOwnerAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnDefaultRoomAccess",		"OnWorldOwnerAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnDefaultPortalAccess",	"OnWorldOwnerAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnEnableRoamingProfilesAccess",	"OnWorldOwnerAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnDefaultAvatarExemplarAccess", "OnWorldOwnerAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },	
		// Global Constant Paths
		{ METHOD_CLIENT | METHOD_SERVER,	"OnGlobalConstantAccess",	NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnWorldNameAccess",		"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnContentPathAccess",		"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnWorldContentPathAccess",	"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnAvatarGraphicsPathAccess",	"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnWorldAvatarGraphicsPathAccess",	"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },	
		{ METHOD_CLIENT | METHOD_SERVER,	"OnWorldPathAccess",		"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },		
		{ METHOD_CLIENT | METHOD_SERVER,	"OnHelpPathAccess",			"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnDirectoryServicePageAccess", "OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnIsSDKInstalledAccess",	"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnServerNameAccess",		"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnOwnerAccess",			"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnContainerAccess",		"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnNameAccess",				"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnAvatarsAccess",			"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnGUIDsAccess",			"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnExemplarsAccess",		"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnModulesAccess",			"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnModuleNameListAccess",	"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnDescriptionAccess",		"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },	
		{ METHOD_CLIENT | METHOD_SERVER,	"OnGUIDAccess",				"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPSBIT_NONEAccess",		"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPSBIT_NOTLOGGEDAccess",	"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPSBIT_NOTREMOTEDAccess",	"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPSBIT_HIDDENAccess",		"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPSBIT_COPYONGETAccess",	"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPSBIT_CONSTANTAccess",	"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPSBIT_SCRIPTDEFAULTPROPERTYAccess",	"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPSBIT_SCRIPTDEFAULTMETHODAccess",	"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPS_NONEAccess",			"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPS_READAccess",			"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPS_WRITEAccess",			"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPS_EXECUTEAccess",		"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPS_ADDAccess",			"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPS_OWNER_RWAccess",		"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPS_OWNER_RWAAccess",		"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPS_EXEMPLAR_RWAccess",	"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPS_EXEMPLAR_RWAAccess",	"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPS_ALL_RAccess",			"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPS_ALL_RWAccess",		"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPS_ALL_RWAAccess",		"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPS_ALL_EAccess",			"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPS_ALLACCESSPROPERTYAccess",	"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPS_SCRIPTDEFAULTPROPERTYAccess",	"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPS_SCRIPTDEFAULTCOLLECTIONAccess",	"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPS_SCRIPTDEFAULTMETHODAccess",	"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnMETHOD_CLIENTAccess",	"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnMETHOD_SERVERAccess",	"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnMETHOD_SYNCAccess",		"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnMODULE_CLIENTAccess",	"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnMODULE_SERVERAccess",	"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnCP_WIZARDAccess",		"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnCP_OWNERAccess",			"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnCP_WORLDOWNERAccess",	"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnCP_CALLERAccess",		"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnMI_DEFAULTAccess",		"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnMI_GRAYEDAccess",		"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnMI_DISABLEDAccess",		"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnMI_INVISIBLEAccess",		"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnMI_CHECKEDAccess",		"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnMI_POPUPAccess",			"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnNothingAccess",			"OnGlobalConstantAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		// utility methods
		{ METHOD_CLIENT | METHOD_SERVER,	"TraceExemplarInfo",		NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_GLOBALDEFAULTMETHOD },
		// user-accessible perf testing methods
		{ METHOD_CLIENT | METHOD_SERVER,	"TimeGetProperty",			NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"TimePutProperty",			NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"TimeInvokeMethod",			NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"TimeGetMethod",			NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"TimeGetPropertySecurity",	NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"TimeCheckPropertySecurity",	NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"TimeCheckMethodSecurity",		NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"TimeCheckPropertiesSecurity",	NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"TimeCheckMethodsSecurity",		NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"TimeGetPropertyAll",		NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"TimePutPropertyAll",		NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"TimeInvokeMethodAll",		NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"TimeGetMethodAll",			NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"TimeGetPropertySecurityAll",		NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"TimeCheckPropertySecurityAll",		NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"TimeCheckMethodSecurityAll",		NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"TimeCheckPropertiesSecurityAll",		NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"TimeCheckMethodsSecurityAll",		NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"TimeAll",					NULL,	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
	};

	int cMethod = sizeof(rgMethod)/sizeof(rgMethod[0]);
	int iMethod;

	hr = m_pWorld->get_Global(&pGlobal);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	for (iMethod=0; iMethod<cMethod; iMethod++)
	{
		bstrName = rgMethod[iMethod].pszName;

		if (rgMethod[iMethod].pszNameInternal == NULL)
			bstrNameInternal = bstrName;
		else
			bstrNameInternal = rgMethod[iMethod].pszNameInternal;
			
		hr = pGlobal->CreateAndAddMethodExt(rgMethod[iMethod].lFlags,
										   pModule, bstrNameInternal, bstrName,
										   rgMethod[iMethod].psbits,
										   rgMethod[iMethod].permissions);
        if (FAILED(hr))
            goto ERROR_ENCOUNTERED;
	}

	if (pGlobal)
	{
		CComPtr<IInternetFileManager> pInetFile;
		CComPtr<IPropertySecurity> pSec;
		CComPtr<IMenuItem> pMenuItem;
		CLSID clsid;
		CComBSTR bstrRootURL;
		static CComBSTR	bstrRoaming("EnableRoamingProfiles");

		// Add ServerName and WorldName
		hr = pGlobal->AddPropertyExt(CComBSTR("ServerName"), CComVariant((LPCTSTR)NULL),
										PSBIT_SYSTEMPROPERTY | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_BSTR, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("WorldName"), CComVariant((LPCTSTR)NULL),
										PSBIT_SYSTEMPROPERTY | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_BSTR, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("WorldDescription"), CComVariant((LPCTSTR)NULL),
										PSBIT_WORLDOWNERPROPERTY, PS_WORLDOWNERPROPERTY, VT_BSTR, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// Create GUID for World
		hr = CoCreateGuid(&clsid);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("GUID"), CComVariant(VWBSTRFromCLSID(clsid)),
										PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_BSTR, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// Per-session count of users
		hr = pGlobal->AddPropertyExt(CComBSTR("NumberOfUsers"), CComVariant(0L),
										PSBIT_HIDDEN | PSBIT_WORLDOWNERPROPERTY | PSBIT_NOTREMOTED | PSBIT_INTERNALLYSET, PS_WORLDOWNERPROPERTY, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// Persistent count of objects
		hr = pGlobal->AddPropertyExt(CComBSTR("NumberOfObjects"), CComVariant(0L),
										PSBIT_HIDDEN | PSBIT_WORLDOWNERPROPERTY | PSBIT_NOTREMOTED, PS_WORLDOWNERPROPERTY, VT_I4, IID_NULL, NULL);	
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// Persistent count of avatars
		hr = pGlobal->AddPropertyExt(CComBSTR("NumberOfAvatars"), CComVariant(0L),
										PSBIT_HIDDEN | PSBIT_WORLDOWNERPROPERTY | PSBIT_NOTREMOTED, PS_WORLDOWNERPROPERTY, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// Default object quota
		hr = pGlobal->AddPropertyExt(CComBSTR("DefaultObjectQuota"), CComVariant((long)lDefaultObjectQuota),
										PSBIT_WORLDOWNERPROPERTY, PS_WORLDOWNERPROPERTY, VT_I4, IID_IDispatch, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("DefaultRoom"), CComVariant((IDispatch*)NULL),
										PSBIT_WORLDOWNERPROPERTY, PS_WORLDOWNERPROPERTY, VT_DISPATCH, IID_IThing, CComBSTR("Room"));
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("DefaultPortal"), CComVariant((IDispatch*)NULL),
										PSBIT_WORLDOWNERPROPERTY, PS_WORLDOWNERPROPERTY, VT_DISPATCH, IID_IThing, CComBSTR("Portal"));
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(bstrRoaming, CComVariant((bool)VARIANT_TRUE),
										PSBIT_WORLDOWNERPROPERTY | PSBIT_HIDDEN, PS_WORLDOWNERPROPERTY, VT_BOOL, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

 
		hr = m_pWorld->get_Tool(CComBSTR("Inetfile"), (IUnknown**)&pInetFile);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pInetFile->get_RootURL(&bstrRootURL.m_str);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("RootURL"), CComVariant(bstrRootURL), 
				PSBIT_WORLDOWNERPROPERTY /* | PSBIT_INTERNALLYSET */, PS_WORLDOWNERPROPERTY, VT_BSTR, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("WelcomeURL"), 
								 CComVariant("Welcome.htm"),
								 PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_HIDDEN, 
								 PS_EXEMPLARDEFAULTPROPERTY, 
								 VT_BSTR, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;


#ifdef SUPPORT_SECURITY_OVERRIDE
		hr = pGlobal->AddPropertyExt(CComBSTR("EnableSecurity"), CComVariant((bool)VARIANT_TRUE),
										PSBIT_WORLDOWNERPROPERTY | PSBIT_HIDDEN, PS_WORLDOWNERPROPERTY, VT_BOOL, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
#endif

		// install the core exemplars
		hr = InstallCoreExemplars(pModule);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		CComPtr<IThing> pAvatarEx;

		hr = m_pWorld->get_Exemplar(CComBSTR("Avatar"), &pAvatarEx);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("DefaultAvatarExemplar"), CComVariant(pAvatarEx), 
			PSBIT_WORLDOWNERPROPERTY, PS_WORLDOWNERPROPERTY, VT_DISPATCH, IID_IThing, CComBSTR("Avatar"));
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// PSBITS for scripting use
		hr = pGlobal->AddPropertyExt(CComBSTR("PSBIT_NONE"), CComVariant((long) PSBIT_NONE), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		hr = pGlobal->AddPropertyExt(CComBSTR("PSBIT_NOTLOGGED"), CComVariant((long) PSBIT_NOTLOGGED), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		hr = pGlobal->AddPropertyExt(CComBSTR("PSBIT_NOTREMOTED"), CComVariant((long) PSBIT_NOTREMOTED), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
#if 0
		hr = pGlobal->AddPropertyExt(CComBSTR("PSBIT_FRIENDLY"), CComVariant((long) PSBIT_FRIENDLY), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
#endif
		
		hr = pGlobal->AddPropertyExt(CComBSTR("PSBIT_HIDDEN"), CComVariant((long) PSBIT_HIDDEN), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		hr = pGlobal->AddPropertyExt(CComBSTR("PSBIT_COPYONGET"), CComVariant((long) PSBIT_COPYONGET), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		hr = pGlobal->AddPropertyExt(CComBSTR("PSBIT_CONSTANT"), CComVariant((long) PSBIT_CONSTANT), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("PSBIT_SCRIPTDEFAULTPROPERTY"), CComVariant((long) PSBIT_SCRIPTDEFAULTPROPERTY), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("PSBIT_SCRIPTDEFAULTMETHOD"), CComVariant((long) PSBIT_SCRIPTDEFAULTMETHOD), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// Basic permissions for scripting use
		hr = pGlobal->AddPropertyExt(CComBSTR("PS_NONE"), CComVariant((long )PS_NONE), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("PS_READ"), CComVariant((long) PS_READ), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("PS_WRITE"), CComVariant((long) PS_WRITE), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("PS_EXECUTE"), CComVariant((long) PS_EXECUTE), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		hr = pGlobal->AddPropertyExt(CComBSTR("PS_ADD"), CComVariant((long) PS_ADD), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// Frequently used combinations of permissions
		hr = pGlobal->AddPropertyExt(CComBSTR("PS_OWNER_RW"), CComVariant((long) PS_OWNER_RW), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("PS_OWNER_RWA"), CComVariant((long) PS_OWNER_RWA), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("PS_EXEMPLAR_RW"), CComVariant((long) PS_EXEMPLAR_RW), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("PS_EXEMPLAR_RWA"), CComVariant((long) PS_EXEMPLAR_RWA), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("PS_ALL_R"), CComVariant((long) PS_ALL_R), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("PS_ALL_RW"), CComVariant((long) PS_ALL_RW), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("PS_ALL_RWA"), CComVariant((long) PS_ALL_RWA), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("PS_ALL_E"), CComVariant((long) PS_ALL_E), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("PS_ALLACCESSPROPERTY"), CComVariant((long) PS_ALLACCESSPROPERTY), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// Exemplar permissions & bits, modified for scripting (no internally added bits set)
		hr = pGlobal->AddPropertyExt(CComBSTR("PS_SCRIPTDEFAULTPROPERTY"), CComVariant((long) PS_SCRIPTDEFAULTPROPERTY), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("PS_SCRIPTDEFAULTCOLLECTION"), CComVariant((long) PS_SCRIPTDEFAULTCOLLECTION), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("PS_SCRIPTDEFAULTMETHOD"), CComVariant((long) PS_SCRIPTDEFAULTMETHOD), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// Client/server, etc. flags for scripting use
		hr = pGlobal->AddPropertyExt(CComBSTR("METHOD_CLIENT"), CComVariant((long) METHOD_CLIENT), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		hr = pGlobal->AddPropertyExt(CComBSTR("METHOD_SERVER"), CComVariant((long) METHOD_SERVER), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		hr = pGlobal->AddPropertyExt(CComBSTR("METHOD_SYNC"), CComVariant((long) METHOD_SYNC), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("MODULE_CLIENT"), CComVariant((long) MODULE_CLIENT), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("MODULE_SERVER"), CComVariant((long) MODULE_SERVER), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("CP_WIZARD"), CComVariant((long) CP_WIZARD), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("CP_OWNER"), CComVariant((long) CP_OWNER), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("CP_WORLDOWNER"), CComVariant((long) CP_WORLDOWNER), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("CP_CALLER"), CComVariant((long) CP_CALLER), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("MI_DEFAULT"), CComVariant((long) MI_DEFAULT), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("MI_GRAYED"), CComVariant((long) MI_GRAYED), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("MI_DISABLED"), CComVariant((long) MI_DISABLED), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("MI_INVISIBLE"), CComVariant((long) MI_INVISIBLE), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("MI_CHECKED"), CComVariant((long) MI_CHECKED), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("MI_POPUP"), CComVariant((long) MI_POPUP), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->AddPropertyExt(CComBSTR("Nothing"), CComVariant((IDispatch*)NULL), 
			PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_DISPATCH, IID_NULL, NULL);
	}
	
	// Uncomment this to see a list of core properties & methods w/ bits in the debugger window
//	TraceExemplarInfo();

ERROR_ENCOUNTERED:
	if (hr != S_OK)
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::Install: Error encountered %x\n", hr);
	return hr;
}

//********************************************
// CreatePropertyList
// Creates a property list.  Takes an optional
// boolean parameter to indicate that the list
// should be created without a world.

STDMETHODIMP CSystemExemplarObject::CreatePropertyList(VARIANT varInitialValue, VARIANT varOrphan, IPropertyList** pppropertylist)
{
	HRESULT hr = S_OK;
	CComVariant varTemp;
	
	// check incoming param
	if (varOrphan.vt != VT_ERROR && varOrphan.vt != VT_BOOL)
		return E_INVALIDARG;

	// need to send m_pWorld as param because it might need to call CreateObjectPropertyExt
	hr = ::CreatePropertyList(m_pWorld, pppropertylist);
	if (FAILED(hr) || *pppropertylist == NULL)
        return ReportCoreError(hr);
    
	// VT_ERROR means optional arg not passed in
	if (varInitialValue.vt != VT_ERROR)
	{
		hr = VariantCopyInd(&varTemp, &varInitialValue);
		if (FAILED(hr))
        {
            VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::CreatePropertyList: error copy VARIANT\n");
			return ReportCoreError(hr);
        }

		hr = ConvertJSArrayToPropertyList(&varTemp, *pppropertylist);
		if (FAILED(hr))
		{
			hr = ConvertSAFEARRAYToPropertyList(&varTemp, *pppropertylist);
			if (FAILED(hr))
            {
                VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::CreatePropertyList: unable to convert input variant to a property list\n");
			    return ReportCoreError(hr);
            }
		}
	}

	// if we want an orphan, null out the IWorld*
	if ((varOrphan.vt == VT_BOOL) && (varOrphan.boolVal == VARIANT_TRUE))
		(*pppropertylist)->put_World((IWorld*)NULL);

	return hr;
}


//********************************************
// CreatePropertyMap
// Creates a property map.  Takes an optional
// boolean parameter to indicate that the map
// should be created without a world.

STDMETHODIMP CSystemExemplarObject::CreatePropertyMap(VARIANT varOrphan, IPropertyMap** pppropertymap)
{
	HRESULT		hr = S_OK;
	
	// check incoming param
	if (varOrphan.vt != VT_ERROR && varOrphan.vt != VT_BOOL)
		return E_INVALIDARG;

	// need to send m_pWorld as param because it might need to call CreateObjectPropertyExt
	hr = ::CreatePropertyMap(m_pWorld, pppropertymap);
	if (FAILED(hr) || *pppropertymap == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::CreatePropertyMap: failed: %x\n", hr);
		return ReportCoreError(hr);
    }
	
	// if we want an orphan, null out the IWorld*
	if ((varOrphan.vt == VT_BOOL) && (varOrphan.boolVal == VARIANT_TRUE))
		(*pppropertymap)->put_World((IWorld*)NULL);

	return S_OK;
}

STDMETHODIMP CSystemExemplarObject::CreateMenu(IPropertyList** ppmenu)
{
	HRESULT hr = ::CreateMenu(m_pWorld, ppmenu);

	return ReportCoreError(hr);
}

STDMETHODIMP CSystemExemplarObject::CreateMenuItem(BSTR bstrMenuText, long lFlags, IThing* pOwner, BSTR bstrUpdateMethod, BSTR bstrMethod, VARIANT varArgs, IMenuItem** ppmenuitem)
{
	HRESULT hr = ::CreateMenuItem(m_pWorld, bstrMenuText, lFlags, pOwner, bstrUpdateMethod, bstrMethod, 
		                    varArgs, ppmenuitem);

	return ReportCoreError(hr);
}

STDMETHODIMP CSystemExemplarObject::CreateOCXProxy(IDispatch *pOCX, IOCXProxy** ppOCXProxy)
{
	HRESULT hr = ::CreateOCXProxy(m_pWorld, pOCX, ppOCXProxy);

	return ReportCoreError(hr);
}

HRESULT CSystemExemplarObject::InstallCoreExemplars(IModule *pModule)
{
	HRESULT hr = S_OK;
	CComPtr<IModule> pNewModule;

	hr = m_pWorld->CreateCOMModule(CComBSTR("Thing"), CComBSTR("VWSYSTEM.Thing.1"), MODULE_CLIENT | MODULE_SERVER, &pNewModule);
	if (FAILED(hr))
		return hr;
	pNewModule.Release();

	hr = m_pWorld->CreateCOMModule(CComBSTR("Artifact"), CComBSTR("VWSYSTEM.Artifact.1"), MODULE_CLIENT | MODULE_SERVER, &pNewModule);
	if (FAILED(hr))
		return hr;
	pNewModule.Release();

	hr = m_pWorld->CreateCOMModule(CComBSTR("Avatar"), CComBSTR("VWSYSTEM.Avatar.1"), MODULE_CLIENT | MODULE_SERVER, &pNewModule);
	if (FAILED(hr))
		return hr;
	pNewModule.Release();

	hr = m_pWorld->CreateCOMModule(CComBSTR("Host"), CComBSTR("VWSYSTEM.Host.1"), MODULE_CLIENT | MODULE_SERVER, &pNewModule);
	if (FAILED(hr))
		return hr;
	pNewModule.Release();

	hr = m_pWorld->CreateCOMModule(CComBSTR("Room"), CComBSTR("VWSYSTEM.Room.1"), MODULE_CLIENT | MODULE_SERVER, &pNewModule);
	if (FAILED(hr))
		return hr;
	pNewModule.Release();

	hr = m_pWorld->CreateCOMModule(CComBSTR("Portal"), CComBSTR("VWSYSTEM.Portal.1"), MODULE_CLIENT | MODULE_SERVER, &pNewModule);
	if (FAILED(hr))
		return hr;
	pNewModule.Release();

	hr = m_pWorld->CreateCOMModule(CComBSTR("IWPortal"), CComBSTR("VWSYSTEM.IWPortal.1"), MODULE_CLIENT | MODULE_SERVER, &pNewModule);
	if (FAILED(hr))
		return hr;
	pNewModule.Release();

	hr = m_pWorld->CreateCOMModule(CComBSTR("StateMachine"), CComBSTR("VWSYSTEM.StateMachine.1"), MODULE_CLIENT | MODULE_SERVER, &pNewModule);
	if (FAILED(hr))
		return hr;
	pNewModule.Release();

	hr = m_pWorld->CreateCOMModule(CComBSTR("WebPage"), CComBSTR("VWSYSTEM.WebPage.1"), MODULE_CLIENT | MODULE_SERVER, &pNewModule);
	return hr;
}

//******************************************************
// QueryAvatarNames
// Returns a list of every avatar on the sytem.

STDMETHODIMP CSystemExemplarObject::QueryAvatarNames(long lFlags, IPropertyList **ppNameList)
{
	if (ppNameList == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::QueryAvatarNames: NULL ouput pointer\n");
		return ReportCoreError(E_POINTER);
    }

	*ppNameList = NULL;

	HRESULT			        hr = S_OK;
	CComPtr<IPropertyMap>   pAvMap;
	CComPtr<IThing>         pGlobal;
	CComPtr<IThing>         pUser;
	CComPtr<IPropertyList>  pFriendList;
	CComPtr<IAvatarProfile> pProfile;
	CComPtr<IPropertyList>  pIgnoreList;
	long			        lAvCount, i;
	CComVariant		        varCurName;
	BOOL			        bAddPerson;
	static CComBSTR	        kbstrAvatars("Avatars");
	static CComBSTR	        kbstrFriends("Friends");
	static CComBSTR	        kbstrIgnoring("Ignoring");
	static CComBSTR	        kbstrProfile("Profile");
	CComBSTR				bstrActualName;

	hr = m_pWorld->get_User(&pUser);
	if (FAILED(hr) || (!pUser))
		return hr;

	if ((lFlags & qanFlagIgnored) || (lFlags & qanFlagFriend))
	{
		hr = pUser->get_ObjectProperty(kbstrProfile, (IObjectProperty **)&pProfile);
		if (FAILED(hr) || (!pProfile))
			return hr;
		
		if (lFlags & qanFlagIgnored)
		{
			CComVariant	varIgnore;

			hr = pProfile->get_KeyValue(kbstrIgnoring, &varIgnore);
			if (SUCCEEDED(hr) && (varIgnore.vt == VT_DISPATCH) && varIgnore.pdispVal)
				pIgnoreList = (IPropertyList *)varIgnore.pdispVal;
		}
		
		if (lFlags & qanFlagFriend)
		{
			CComVariant varFriends;

			hr = pProfile->get_KeyValue(kbstrFriends, &varFriends);
			if (SUCCEEDED(hr) && (varFriends.vt == VT_DISPATCH) && varFriends.pdispVal)
				pFriendList = (IPropertyList *)varFriends.pdispVal;
		}
	}

	hr = m_pWorld->get_Global(&pGlobal);
	if (FAILED(hr) || (!pGlobal))
		return hr;

	hr = pGlobal->get_ObjectProperty(kbstrAvatars, (IObjectProperty**)&pAvMap);
	if (FAILED(hr) || (!pAvMap))
		return hr;

	hr = pAvMap->get_Count(&lAvCount);
	if (FAILED(hr) || (!lAvCount))
		return hr;

	hr = ::CreatePropertyList(m_pWorld, ppNameList);
	if (FAILED(hr) || (!*ppNameList))
		return hr;
	
	// now, add the avatars
	for (i = 0; i < lAvCount; i++)
	{
		hr = pAvMap->get_PropertyAtIndex(i, &varCurName);
		if (SUCCEEDED(hr) && (varCurName.vt == VT_BSTR) && varCurName.bstrVal)
		{
			IThing	*pCurThing	= NULL;

			hr = pAvMap->get_Thing(varCurName.bstrVal, &pCurThing);
			if (SUCCEEDED(hr) && pCurThing)
			{
				// we have to get the name because there is a chance
				// that the name might be different in the avatar from
				// the avatar map.
				pCurThing->get_Name(&bstrActualName.m_str);
				bAddPerson = TRUE;

				if (lFlags != qanFlagAll)
				{
					// 1.  Check Flags
					if (lFlags & qanFlagConnected)
					{
						// remove users who are not connected
						VARIANT_BOOL	bConnected;
						static CComBSTR	kbstrIsConnected	= "IsConnected";

						hr = pCurThing->get_BOOL(kbstrIsConnected, &bConnected);
						if (FAILED(hr) || (bConnected == VARIANT_FALSE))
							bAddPerson = FALSE;
						hr = S_OK;
					}

					if (bAddPerson && (lFlags & qanFlagIgnored) && pIgnoreList)
					{
						// remove users who are ignored by me
						long	lLoc;

						hr = pIgnoreList->Find(CComVariant(pCurThing), &lLoc);
						if (SUCCEEDED(hr) && (lLoc != -1))
							bAddPerson = FALSE;
						hr = S_OK;
					}

					if (bAddPerson && lFlags & qanFlagIgnoring)
					{
						// remove users who are ignoring me
						CComPtr<IAvatarProfile> pCurProfile;

						hr = pCurThing->get_ObjectProperty(kbstrProfile, (IObjectProperty **)&pCurProfile);
						if (SUCCEEDED(hr) && pCurProfile)
						{
							CComVariant varIgnore;

							hr = pCurProfile->get_KeyValue(kbstrIgnoring, &varIgnore);
							if (SUCCEEDED(hr) && (varIgnore.vt == VT_DISPATCH) && varIgnore.pdispVal)
							{
								long			        lLoc;
								CComPtr<IPropertyList>  pCurIgnoreList = (IPropertyList *)varIgnore.pdispVal;

								hr = pCurIgnoreList->Find(CComVariant(pUser), &lLoc);
								if (SUCCEEDED(hr) && (lLoc != -1))
									bAddPerson = FALSE;
								hr = S_OK;
							}
						}
					}

					if (bAddPerson && (lFlags & qanFlagFriend) && pFriendList)
					{
						// remove users who are not my friend
						long	lLoc;

						hr = pFriendList->Find(CComVariant(pCurThing), &lLoc);
						if (FAILED(hr) || (lLoc == -1))
							bAddPerson = FALSE;
						hr = S_OK;
					}
				}

				// valid thing - add it
				if (bAddPerson)
						(*ppNameList)->AddString(bstrActualName);

				varCurName.Clear();
				bstrActualName.Empty();
			}
		}
	}

	return hr;
}

//******************************************************
// InvokeRemoteClientMethod
// Invokes a method on a number of named clients

STDMETHODIMP CSystemExemplarObject::InvokeRemoteClientMethod(IThing *pTarget, BSTR bstrMethodName, VARIANT varUserOrList, VARIANT varParams)
{
    HRESULT         hr = S_OK;
	CComPtr<IThing>	pThis;
	static CComBSTR	kbstrHandleMethod = "HandleRemoteClientMethod";

    if (pTarget == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::InvokeRemoteClientMethod: NULL pointer\n");
        return ReportCoreError(E_POINTER);
    }

	hr = m_pWorld->get_This(&pThis);
    if (FAILED(hr))
        return hr;
	
	hr = pThis->InvokeMethodExt(kbstrHandleMethod, CComDISPPARAMS(4, CComVariant((IDispatch *)pTarget), CComVariant(bstrMethodName), varUserOrList, varParams), NULL);
	return hr;
}


//******************************************************
// HandleRemoteClientMethod
// Handles a method on a number of named clients

STDMETHODIMP CSystemExemplarObject::HandleRemoteClientMethod(IThing *pTarget, BSTR bstrMethodName, VARIANT userOrListIn, VARIANT varParams)
{
    HRESULT                 hr = S_OK;
	CComPtr<IThing>			pUser;
	CComPtr<IPropertyList>	pTargetList;

    if (pTarget == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::HandleRemoteClientMethod: NULL pointer\n");
        return ReportCoreError(E_POINTER);
    }

	hr = m_pWorld->get_User(&pUser);
    if (FAILED(hr))
        return hr;

    CComVariant userOrList;

    if (FAILED(hr = VariantCopyInd(&userOrList, &userOrListIn)))
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::HandleRemoteClientMethod: Unable to convert VARIANT\n");
        return ReportCoreError(hr);
    }

	switch (userOrList.vt)
	{
	case VT_SAFEARRAY:
		hr = ::CreatePropertyList(m_pWorld, &pTargetList);
        if (FAILED(hr))
            return ReportCoreError(hr);

		if (FAILED(hr = ConvertSAFEARRAYToPropertyList(&userOrList, pTargetList)))
        {
            VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::HandleRemoteClientMethod: Unable to convert input parm to property list\n");
            return ReportCoreError(hr);
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
				hr = ::CreatePropertyList(m_pWorld, &pTargetList);
                if (FAILED(hr))
                    return ReportCoreError(hr);

				if (FAILED(hr = pTargetList->AddDispatch(pthing)))
                    return hr;
			}
			else
			{
				// see if we have a list
				plist = userOrList.pdispVal;

				// we have a list - use it instead
				if (plist)
				{
					pTargetList = plist;
					// WGR: let scope exit handle Release: plist.Release();
				}
			}
		}
		break;

	default:
		// invalid args!!
		// REVIEW: ??? is this the correct error???
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::HandleRemoteClientMethod: Invalid argument\n");
		return ReportCoreError(DISP_E_BADVARTYPE); 
	}
	
	if (pTargetList)
	{
		long	lLoc;

		hr = pTargetList->Find(CComVariant((IDispatch *)pUser), &lLoc);
		if ((hr == S_OK ) && lLoc >= 0)
			return pTarget->InvokeMethod(bstrMethodName, varParams, NULL);
	}

	return hr;
}


//******************************************************
// FireUIEvent
// Fires a UI event on this machine

STDMETHODIMP CSystemExemplarObject::FireUIEvent(IPropertyList *pTargetList, IThing *pSource, BSTR bstrMethodName, VARIANT varParams)
{
	CComPtr<IThing>	pUser;
	long	lLoc;
    HRESULT hr = S_OK;
	
    if (pTargetList == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::FireUIEvent: NULL pointer\n");
        return ReportCoreError(E_POINTER);
    }

	if (FAILED(hr = m_pWorld->get_User(&pUser)))
        return hr;

	if (FAILED(hr = pTargetList->Find(CComVariant((IDispatch *)pUser), &lLoc)))
        return hr;
	
	if (lLoc >= 0)
		return m_pWorld->FireUIEvent(pSource, bstrMethodName, varParams);

    return hr;	
}


//******************************************************
// FireRemoteUIEvent
// Given a list of other clients, fires a UI event
// on all of those machines.

STDMETHODIMP CSystemExemplarObject::FireRemoteUIEvent(VARIANT userOrListIn, IThing *pSource, BSTR bstrEventName, VARIANT varParams)
{
    HRESULT                 hr = S_OK;
	CComPtr<IPropertyList>	pTargetList;
    CComVariant             userOrList;

    if (FAILED(hr = VariantCopyInd(&userOrList, &userOrListIn)))
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::FireRemoteUIEvent: Unable to convert VARIANT\n");
        return ReportCoreError(hr);
    }

	switch (userOrList.vt)
	{
	case VT_SAFEARRAY:
		if (FAILED(hr = ::CreatePropertyList(m_pWorld, &pTargetList)))
            return ReportCoreError(hr);

		if (FAILED(hr = ConvertSAFEARRAYToPropertyList(&userOrList, pTargetList)))
        {
            VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::FireRemoteUIEvent: Unable to convert input parm to property list\n");
            return ReportCoreError(hr);
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
				if (FAILED(hr = ::CreatePropertyList(m_pWorld, &pTargetList)))
                    return ReportCoreError(hr);

				if (FAILED(hr = pTargetList->AddThing(pthing))) // Dispatch(pthing)))
				    return hr;
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

	default:
		// invalid args!!
		// REVIEW: ??? is this the correct error???
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::FireRemoteUIEvent: Invalid argument\n");
		return ReportCoreError(DISP_E_BADVARTYPE); 
		break;
	}
	
	if (pTargetList)
	{
		static CComBSTR	kbstrFireUIEvent("FireUIEvent");
		CComPtr<IThing> pThis;

		if (SUCCEEDED(hr = m_pWorld->get_This(&pThis)))
		    return pThis->InvokeMethodExt(kbstrFireUIEvent, CComDISPPARAMS(4, CComVariant((IDispatch *)pTargetList), CComVariant((IDispatch *)pSource), CComVariant(bstrEventName), varParams), NULL);
	}

    return hr;
}

STDMETHODIMP CSystemExemplarObject::OnRootURLChanged(long lHint, VARIANT varHintData)
{
	VARIANT_BOOL bServerSide = VARIANT_FALSE;

	// ignore property add
	if (lHint == thAdd)
		return S_OK;

    HRESULT hr = S_OK;

	hr = m_pWorld->UpdateGlobalPaths();
	if (FAILED(hr))
		return hr;

	m_pWorld->get_ServerSide(&bServerSide);
	
	if (bServerSide == VARIANT_TRUE)
	{
		CComPtr<IThing> pThis;
		DISPPARAMS dispparams;

		InitializeDISPPARAMS(&dispparams, 0);

		// write out log record, so we fixup paths on server when reading RootURL log record
		/* hr = */ m_pWorld->LogWorldCommand(DISPID_WORLD_UPDATEGLOBALPATHS, DISPATCH_METHOD, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		hr = m_pWorld->get_This(&pThis);
		if (FAILED(hr))
			return hr;

		hr = pThis->InvokeMethodExt(CComBSTR("OnRootURLChangedClient"), CComDISPPARAMS(2, CComVariant(lHint), varHintData), NULL);
	}

	return hr;
}

STDMETHODIMP CSystemExemplarObject::UploadFile( BSTR bstrLocalName, BSTR bstrRemoteName, BSTR *pbstrURL)
{
	HRESULT hr = S_OK;
	CString strLocalName(bstrLocalName);
	CString strRemoteName(bstrRemoteName);
	CComPtr<IThing>	pGlobal;
	CComBSTR bstrPath;
	CComBSTR bstrURL;

    if (pbstrURL == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::UploadFile: NULL pointer\n");
        return ReportCoreError(E_POINTER);
    }

	*pbstrURL = NULL;

	// get the global pointer
	hr = m_pWorld->get_Global(&pGlobal);
	if (FAILED(hr))
		return hr;

	// see if we can just copy with the file system
	if (SUCCEEDED(pGlobal->get_String(CComBSTR("NetworkPublishingPath"), &bstrPath.m_str)) &&
		SUCCEEDED(pGlobal->get_String(CComBSTR("NetworkPublishingURL"), &bstrURL.m_str)) )
	{
		// yep - do it

		// calculate destination. Decorate it with the user
		// name to avoid file name collisions.
		CComPtr<IThing> pUser;
		CComBSTR bstrUserName;

		hr = m_pWorld->get_User(&pUser);
		if (FAILED(hr))
			return hr;

		hr = pUser->get_Name(&bstrUserName.m_str);
		if (FAILED(hr))
			return hr;

		CString strDest = bstrPath;
		strDest += "\\";
		strDest += CString(bstrUserName);
		strDest += "_";
		strDest += strRemoteName;

		if (!::CopyFile(strLocalName, strDest, FALSE))
		{
            VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::UploadFile: unable to copy file: %x\n", HRESULT_FROM_WIN32(GetLastError()));
            return ReportCoreError(VWCORE_E_FILEUPLOADERROR);
		}

		// calculate destination url
		bstrURL.Append("/");
		bstrURL.Append(bstrUserName);
		bstrURL.Append("_");
		bstrURL.Append(bstrRemoteName);

		*pbstrURL = bstrURL.Copy();

		return S_OK;
	}

	// couldn't do file system copy - do web post
	LPTSTR psz = (char *) (LPCTSTR) strLocalName;
	DWORD cbSite = 0;
	char szURL[_MAX_PATH] = "";
	DWORD cbURL = sizeof(szURL);

	typedef HRESULT (WINAPI *WPPOSTPFN)(HWND hwnd, 
										DWORD cLocalPaths, LPSTR *lppszLocalPaths, 
										LPDWORD lpcbSiteName, LPSTR lpszSiteName,
										LPDWORD lpcbURL, LPSTR lpszURL, 
										DWORD dwFlag);
	WPPOSTPFN	pfnWpPost;
	HINSTANCE	hinstWebPost = NULL;
	if ((hinstWebPost = LoadLibrary("WEBPOST.DLL")) == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::UploadFile: unable to post file: %d\n", GetLastError());
        return ReportCoreError(VWCORE_E_FILEUPLOADERROR);
    }

    if (((pfnWpPost = (WPPOSTPFN)GetProcAddress(hinstWebPost, "WpPost")) != NULL) &&
	    ((*pfnWpPost)(NULL, 1, &psz, &cbSite, NULL, &cbURL, szURL, 
                                    WPF_NO_RECURSIVE_POST) == ERROR_SUCCESS))
	{
		CString strURL(szURL);

		// compose return URL
		// REVIEW: is there any way to name the remote file anything
		// other than the same as the source?

		char szFName[_MAX_FNAME];
		char szExt[_MAX_EXT];

		_splitpath(strLocalName, NULL, NULL, szFName, szExt);
		strURL += "/";
		strURL += szFName;
		strURL += szExt;

		*pbstrURL = strURL.AllocSysString();
	}
	else
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::UploadFile: unable to post file: %d\n", GetLastError());
        return ReportCoreError(VWCORE_E_FILEUPLOADERROR);
    }

    // if we get this far we have up'd the refcount on webpost.dll...
    FreeLibrary(hinstWebPost);
    return S_OK;
}

////////////////////////////////////////////////////////////////////
// MakeHost
// Change exemplar type of avatar to host 
// Only wizards can call
// Currently only supported for VWorlds Avatars, not custom avatars

STDMETHODIMP CSystemExemplarObject::MakeHost(IThing* pTarget)
{
	HRESULT hr				= S_OK;
	CComPtr<IThing>			pAvatarExemplar;
	CComPtr<IThing>			pHostExemplar;
	CComPtr<IThing>			pCurrentExemplar;
	CComPtr<IThing>			pNextExemplar;
	CComPtr<IThing>			pCaller;
	CComPtr<IThing>			pGlobal;
	CComPtr<IThing>			pRoom;
	CComPtr<IPropertyList>	pAvatars;
	static CComBSTR kbstrEvent = VW_HOSTSTATUSCHANGED_EVENT_STR;
	VARIANT_BOOL			bTargetIsAvatar = VARIANT_FALSE;
	VARIANT_BOOL			bIsAvatar = VARIANT_FALSE;
	VARIANT_BOOL			bIsWizard = VARIANT_FALSE;

#ifdef _DEBUG
	TRACE("CSystemExemplarObject::MakeHost\n");
#endif

    if (pTarget == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::MakeHost: NULL pointer\n");
        return ReportCoreError(E_POINTER);
    }

	// Check that target is of type Avatar 
	hr = pTarget->IsOfType(CComBSTR("Avatar"), &bTargetIsAvatar);

	if (bTargetIsAvatar == VARIANT_FALSE)
	{
		VWTRACE(m_pWorld, "COREEXEMP", VWT_NONE, "CSystemExemplarObject::MakeHost: Target is not an avatar\n");
		return ReportCoreError(VWCORE_E_NOTANAVATAR);
	}

	// Get the avatar exemplar
	hr = m_pWorld->get_Exemplar(CComBSTR("Avatar"), &pAvatarExemplar);
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::MakeHost: Can't get avatar exemplar\n");
		return hr;
	}


	// Get the host exemplar
	hr = m_pWorld->get_Exemplar(CComBSTR("Host"), &pHostExemplar);
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::MakeHost: Can't get host exemplar\n");
		return hr;
	}

	// Security policy check - make sure caller is a wizard

	hr = m_pWorld->get_Caller(&pCaller);
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::MakeHost: Can't get caller \n");
		return hr;
	}
	if (pCaller)
	{
		hr = pCaller->IsOfType(CComBSTR("Avatar"), &bIsAvatar);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::MakeHost: Can't get if avatar\n");
			return hr;
		}

		if (bIsAvatar)
		{
			hr = pCaller->get_IsWizard(&bIsWizard);
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::MakeHost: Can't get if wizard \n");
				return hr;
			}
			// Only change the exemplar if a wizard is calling the method
			if (bIsWizard == VARIANT_TRUE)
			{
				// Get the current exemplar
				hr = pTarget->get_Exemplar(&pCurrentExemplar);
				if (FAILED(hr))
				{
					VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::MakeHost: Can't get current exemplar\n");
					return hr;
				}
	
				// Only change if exemplar is a VWorlds Avatar
				if ((pCurrentExemplar == NULL) || (pCurrentExemplar != pAvatarExemplar))
				{
					VWTRACE(m_pWorld, "COREEXEMP", VWT_NONE, "CSystemExemplarObject::MakeHost: Target is not a VWorlds Avatar\n");
					return ReportCoreError(VWCORE_E_NOTANAVATAR);
				}

				
				// Set the current exemplar's exemplar to be host
				hr = pTarget->put_ExemplarInt(pHostExemplar);
				if (FAILED(hr))
				{
					VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::MakeHost: Can't put host exemplar\n");
					return hr;
				}
				
				// Now, get all the information to 
				// fire a UI event so the avatar list will update with correct host icon

				// Get the current room
				hr = pTarget->get_Container(&pRoom);
				if (FAILED(hr))
				{
					VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::MakeHost: Can't get container\n");
					return hr;
				}

				// Get the avatars in the room
				if (pRoom)
				{
					hr = pRoom->get_ObjectProperty(CComBSTR("Avatars"), (IObjectProperty **) &pAvatars);
					if (FAILED(hr))
					{
						VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::MakeHost: Can't get avatar list\n");
						return hr;
					}
				}

				hr = m_pWorld->get_Global(&pGlobal);
				if (FAILED(hr))
				{
					VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::MakeHost: Can't get global\n");
					return hr;
				}
				if (pGlobal)
				{
					hr = pGlobal->InvokeMethodExt(CComBSTR("FireRemoteUIEvent"),  
												  CComDISPPARAMS(4, CComVariant((IDispatch *) pAvatars), 
												  CComVariant((IDispatch *) pCaller), 
												  CComVariant(kbstrEvent), 
												  CComVariant((IDispatch *) pTarget)), NULL);
					if (FAILED(hr))
					{
						VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::MakeHost: Failed to fire remote UI event\n");
						return hr;
					}
				}
			}
			else
            {
				VWTRACE(m_pWorld, "COREEXEMP", VWT_NONE, "CSystemExemplarObject::MakeHost: Security failure: caller not a wizard\n");
                return ReportCoreError(VWCORE_E_NOTAWIZARD);
            }
		}
	}

	return hr;
}

//////////////////////////////////////////////////////////
// UnMakeHost
// Change exemplar type from host to avatar
// Only wizards can call
// Currently only supported for VWorlds Avatars, not custom avatars

STDMETHODIMP CSystemExemplarObject::UnMakeHost(IThing* pTarget)
{
	HRESULT hr				= S_OK;
	CComPtr<IThing>			pAvatarExemplar;
	CComPtr<IThing>			pHostExemplar;
	CComPtr<IThing>			pCurrentExemplar;
	CComPtr<IThing>			pNextExemplar;
	CComPtr<IThing>			pCaller;
	CComPtr<IThing>			pGlobal;
	CComPtr<IThing>			pRoom;
	CComPtr<IPropertyList>	pAvatars;
	static CComBSTR kbstrEvent = VW_HOSTSTATUSCHANGED_EVENT_STR;	
	VARIANT_BOOL			bTargetIsAvatar = VARIANT_FALSE;
	VARIANT_BOOL			bIsAvatar = VARIANT_FALSE;
	VARIANT_BOOL			bIsWizard = VARIANT_FALSE;

#ifdef _DEBUG
	TRACE("CSystemExemplarObject::UnMakeHost\n");
#endif 

    if (pTarget == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::UnMakeHost: NULL pointer\n");
        return ReportCoreError(E_POINTER);
    }

	// Check that target is of type Avatar 
	hr = pTarget->IsOfType(CComBSTR("Avatar"), &bTargetIsAvatar);

	if (bTargetIsAvatar == VARIANT_FALSE)
	{
		VWTRACE(m_pWorld, "COREEXEMP", VWT_NONE, "CSystemExemplarObject::MakeHost: Target is not an avatar\n");
		return ReportCoreError(VWCORE_E_NOTANAVATAR);
	}

	// Get the avatar exemplar
	hr = m_pWorld->get_Exemplar(CComBSTR("Avatar"), &pAvatarExemplar);
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::UnMakeHost: Can't get avatar exemplar\n");
		return hr;
	}

	// Get the host exemplar
	hr = m_pWorld->get_Exemplar(CComBSTR("Host"), &pHostExemplar);
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::UnMakeHost: Can't get host exemplar\n");
		return hr;
	}
	// Security policy check - make sure caller is a wizard

	hr = m_pWorld->get_Caller(&pCaller);
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::MakeHost: Can't get caller \n");
		return hr;
	}

	if (pCaller)
	{
		hr = pCaller->IsOfType(CComBSTR("Avatar"), &bIsAvatar);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::MakeHost: Can't get if avatar\n");
			return hr;
		}

		if (bIsAvatar)
		{
			hr = pCaller->get_IsWizard(&bIsWizard);
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::MakeHost: Can't get if wizard \n");
				return hr;
			}
			// Only change the exemplar if a wizard is calling the method
			if (bIsWizard == VARIANT_TRUE)
			{
				// Get the current exemplar
				hr = pTarget->get_Exemplar(&pCurrentExemplar);
				if (FAILED(hr))
				{
					VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::UnMakeHost: Can't get current exemplar\n");
					return hr;
				}

				// Only change if target is a VWorlds host
				if ((pCurrentExemplar == NULL) || (pCurrentExemplar != pHostExemplar))
				{
					VWTRACE(m_pWorld, "COREEXEMP", VWT_VERBOSE, "CSystemExemplarObject::UnMakeHost: Not a VWorlds Host\n");
					return ReportCoreError(VWCORE_E_NOTAHOST);
				}


				// Set the current exemplar's exemplar to be avatar
				hr = pTarget->put_ExemplarInt(pAvatarExemplar);
				if (FAILED(hr))
					VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::UnMakeHost: Can't put avatar exemplar\n");

				// Now, get all the information to 
				// fire a UI event so the avatar list will update with correct host icon

				// Get the current room
				hr = pTarget->get_Container(&pRoom);
				if (FAILED(hr))
				{
					VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::MakeHost: Can't get container\n");
					return hr;
				}

				// Get the avatars in the room
				if (pRoom)
				{
					hr = pRoom->get_ObjectProperty(CComBSTR("Avatars"), (IObjectProperty **) &pAvatars);
					if (FAILED(hr))
					{
						VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::UnMakeHost: Can't get avatar list\n");
						return hr;
					}
				}
				
				hr = m_pWorld->get_Global(&pGlobal);
				if (FAILED(hr))
				{
					VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::UnMakeHost: Can't get global\n");
					return hr;
				}
				if (pGlobal)
				{
					hr = pGlobal->InvokeMethodExt(CComBSTR("FireRemoteUIEvent"), 
												  CComDISPPARAMS(4, CComVariant((IDispatch *) pAvatars), 
												  CComVariant((IDispatch *) pCaller), 
												  CComVariant(kbstrEvent), CComVariant((IDispatch *) pTarget)), NULL);
					if (FAILED(hr))
					{
						VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::UnMakeHost: Failed to fire remote UI event\n");
						return hr;
					}
				}
			}
			else
            {
				VWTRACE(m_pWorld, "COREEXEMP", VWT_NONE, "CSystemExemplarObject::UnMakeHost: Security failure: caller not a wizard\n");
                return ReportCoreError(VWCORE_E_NOTAWIZARD);
            }
		}
	}

	return hr;
}

//////////////////////////////////////////////////////////
// TraceExemplarInfo
// Traces all exemplars and their properties and methods

STDMETHODIMP CSystemExemplarObject::TraceExemplarInfo()
{
	typedef CVWComPtr<IThing, NULL, &IID_IThing> CThingPtr;
	typedef CVWComPtr<IPropertyMap, NULL, &IID_IPropertyMap> CMapPtr;

	CThingPtr  pGlobal, pExemplar, pProperty;
	CMapPtr pExemplarMap, pMethodsMap, pPropertiesMap;
	CComVariant varExemplar;
	CComVariant varProperty;
	CComVariant varMethod;
	BSTR bstrExemplar;
	BSTR bstrPropertyName;
	BSTR bstrMethodName;
	VARIANT_BOOL bLastExemValue;
	VARIANT_BOOL bLastPropValue;
	VARIANT_BOOL bLastMethValue;
	VARIANT_BOOL bIsExemEmpty;
	VARIANT_BOOL bIsPropEmpty;
	VARIANT_BOOL bIsMethEmpty;
	bool bGlobalDone = false;
	long lPermissions;
	long lBits;
	long lCount;
    HRESULT hr = S_OK;
	
	if (FAILED(hr = m_pWorld->get_Exemplars(&pExemplarMap.p)))
        return hr;

	if (FAILED(hr = pExemplarMap->get_IsEmpty(&bIsExemEmpty)))
        return hr;

	if (FAILED(hr = pExemplarMap->get_Count(&lCount)))
        return hr;

	TRACE("Exemplar Count: %ld \n", lCount);
	
	if (bIsExemEmpty==VARIANT_FALSE)
	{
		// Get first pair of values 
		if (FAILED(hr = pExemplarMap->FirstItem(&bstrExemplar, &varExemplar, &bLastExemValue)))
            return hr;
		
		// Iternate through exemplar property map
		while (bLastExemValue==VARIANT_FALSE)
		{
			// First look for Global properties & methods
			if (!bGlobalDone)
			{
				if (FAILED(hr = m_pWorld->get_Global(&pGlobal.p)))
                    return hr;

				pExemplar = pGlobal;  // want to test for properties & methods on Global
				TRACE("Global \n");
			}
			else
			{
				pExemplar = varExemplar;
				ASSERT (pExemplar != NULL);
				TRACE("%s \n", CString(bstrExemplar));
			}
		
			// Now get properties for this exemplar
		
			if (FAILED(hr = pExemplar->get_InstanceProperties(&pPropertiesMap.p)))
                return hr;

			if (FAILED(hr = pPropertiesMap->get_IsEmpty(&bIsPropEmpty)))
                return hr;

			if (FAILED(hr = pPropertiesMap->get_Count(&lCount)))
                return hr;

			TRACE("Properties Count %ld \n", lCount);

			// Iterate through property list
			if (bIsPropEmpty==VARIANT_FALSE)
			{
				// Get first item
				if (FAILED(hr = pPropertiesMap->FirstItem(&bstrPropertyName, &varProperty, &bLastPropValue)))
                    return hr;
				
				while (bLastPropValue!=VARIANT_TRUE)
				{
					CComPtr<IPropertySecurity> pPropSecurity;
					TRACE("%s \t", CString(bstrPropertyName));

					// Get security on the property						
					if (FAILED(hr = pPropertiesMap->get_PropertySecurity(bstrPropertyName, &pPropSecurity)))
                        return hr;

					if (FAILED(hr = pPropSecurity->get_Permissions(&lPermissions)))
                        return hr;

					TRACE("0X%X \t", lPermissions);

					// Check for defined macros
					if (lPermissions == PS_GLOBALSYSTEMCONSTANT)
						TRACE("Global System Constant \t");
					else if (lPermissions == PS_WORLDOWNERPROPERTY)
						TRACE("World Owner Property \t");
					else if (lPermissions == PS_WORLDOWNERCOLLECTION)
						TRACE("World Owner Collection \t");
					else if (lPermissions == PS_EXEMPLARDEFAULTPROPERTY)
						TRACE("Exemplar-System Default Property \t");
					else if (lPermissions == PS_EXEMPLARDEFAULTCOLLECTION)
						TRACE("Exemplar-System Default Collection \t");	
					else if (lPermissions == PS_ALLACCESSPROPERTY)
						TRACE("All Access Property \t");
					else
						TRACE("\t");

				
					// Test for various permissions
					TRACE("%s \t", "Exemplar");
					TRACE("%s",  ((lPermissions & (PS_READ << 8))? "R ": "-"));
					TRACE("%s",  ((lPermissions & (PS_WRITE << 8))? "W ": "-"));
					TRACE("%s",  ((lPermissions & (PS_EXECUTE << 8))? "E ": "-"));
					TRACE("%s \t",  ((lPermissions & (PS_ADD << 8))? "A ": "-"));

					TRACE("%s \t", "Owner");
					TRACE("%s",  ((lPermissions & PS_READ)? "R ": "-"));
					TRACE("%s",  ((lPermissions & PS_WRITE)? "W ": "-"));
					TRACE("%s",  ((lPermissions & PS_EXECUTE)? "E ": "-"));
					TRACE("%s \t",  ((lPermissions & PS_ADD)? "A ": "-"));

					TRACE("%s \t", "All");
					TRACE("%s",  ((lPermissions & (PS_READ << 16))? "R ": "-"));
					TRACE("%s",  ((lPermissions & (PS_WRITE << 16))? "W ": "-"));
					TRACE("%s",  ((lPermissions & (PS_EXECUTE << 16))? "E ": "-"));
					TRACE("%s \t",  ((lPermissions & (PS_ADD << 16))? "A ": "-"));
			
					// Get bits
					if (FAILED(hr = pPropSecurity->get_Bits(&lBits)))
                        return hr;

					TRACE(" \t 0X%X \t", lBits);

					// Test for various bits
					TRACE("%s \t", ((lBits & PSBIT_COPYONGET)? "Copy ": " "));
					TRACE("%s \t", ((lBits & PSBIT_NOTLOGGED)? "NotLog" : " "));
					TRACE("%s \t", ((lBits & PSBIT_NOTREMOTED)? "NotRem" : " "));
					TRACE("%s \t", ((lBits & PSBIT_BYVAL)? "ByVal" : " "));
					TRACE("%s \t", ((lBits & PSBIT_OWNERONLY)? "Own" : " "));
					TRACE("%s \t", ((lBits & PSBIT_FRIENDLY)? "Frd" : " "));
					TRACE("%s \t", ((lBits & PSBIT_HIDDEN)? "Hide" : " "));
//						TRACE("%s \t", ((lBits & PSBIT_CLIENTPROPERTY)? "Clnt" : " "));
					TRACE("%s \t", ((lBits & PSBIT_INTERNALLYADDED) ? "IntAdd" :" "));
					TRACE("%s \t", ((lBits & PSBIT_INTERNALLYSET)? "IntSet" : " "));
//						TRACE("%s \t", ((lBits & PSBIT_FROMSERVER)? "FrSrv" : " "));

					// Free up property values
					SAFEFREESTRING(bstrPropertyName);
					varProperty.Clear();

					// Get the next property
					if (FAILED(hr = pPropertiesMap->NextItem(&bstrPropertyName, &varProperty, &bLastPropValue)))
                        return hr;

					TRACE("\n");
				}

			}

			pPropertiesMap.Release();

			// Now get methods for this exemplar 
			if (FAILED(hr = pExemplar->get_InstanceMethods(&pMethodsMap.p)))
                return hr;

			if (FAILED(hr = pMethodsMap->get_IsEmpty(&bIsMethEmpty)))
                return hr;

			if (FAILED(hr = pMethodsMap->get_Count(&lCount)))
                return hr;

			TRACE("Methods Count %ld \n", lCount);

			if (bIsMethEmpty==VARIANT_FALSE)
			{
				if (FAILED(hr = pMethodsMap->FirstItem(&bstrMethodName, &varMethod, &bLastMethValue)))
                    return hr;

				while (bLastMethValue==VARIANT_FALSE)
				{
					CComPtr<IPropertySecurity> pMethSecurity;
					TRACE("%s \t", CString(bstrMethodName));

					// Get security flags on the method
					if (FAILED(hr = pMethodsMap->get_PropertySecurity(bstrMethodName, &pMethSecurity)))
                        return hr;

					if (FAILED(hr = pMethSecurity->get_Permissions(&lPermissions)))
                        return hr;

					TRACE("0X%X \t", lPermissions);

					// Test for defined macros
#if 0
					if (lPermissions == PS_SYSTEMMETHOD)
						TRACE("System Method \t");
					else 
#endif
					if (lPermissions == PS_GLOBALDEFAULTMETHOD)
						TRACE("Global Default Method \t");
					else if (lPermissions == PS_EXEMPLARDEFAULTMETHOD)
						TRACE("Exemplar Default Method \t");
					else
						TRACE(" \t");

					// Test for various permissions
					// Test for various permissions
					TRACE("%s \t", "Exemplar");
					TRACE("%s",  ((lPermissions & (PS_READ << 8))? "R ": "-"));
					TRACE("%s",  ((lPermissions & (PS_WRITE << 8))? "W ": "-"));
					TRACE("%s",  ((lPermissions & (PS_EXECUTE << 8))? "E ": "-"));
					TRACE("%s \t",  ((lPermissions & (PS_ADD << 8))? "A ": "-"));

					TRACE("%s \t", "Owner");
					TRACE("%s",  ((lPermissions & PS_READ)? "R ": "-"));
					TRACE("%s",  ((lPermissions & PS_WRITE)? "W ": "-"));
					TRACE("%s",  ((lPermissions & PS_EXECUTE)? "E ": "-"));
					TRACE("%s \t",  ((lPermissions & PS_ADD)? "A ": "-"));

					TRACE("%s \t", "All");
					TRACE("%s",  ((lPermissions & (PS_READ << 16))? "R ": "-"));
					TRACE("%s",  ((lPermissions & (PS_WRITE << 16))? "W ": "-"));
					TRACE("%s",  ((lPermissions & (PS_EXECUTE << 16))? "E ": "-"));
					TRACE("%s \t",  ((lPermissions & (PS_ADD << 16))? "A ": "-"));

					// Get bits
					if (FAILED(hr = pMethSecurity->get_Bits(&lBits)))
                        return hr;

					TRACE(" \t 0X%X ", lBits);

					// Test for various bits
					TRACE("%s \t", ((lBits & PSBIT_COPYONGET)? "Copy ": " "));
					TRACE("%s \t", ((lBits & PSBIT_NOTLOGGED)? "NotLog" : " "));
					TRACE("%s \t", ((lBits & PSBIT_NOTREMOTED)? "NotRem" : " "));
					TRACE("%s \t", ((lBits & PSBIT_BYVAL)? "ByVal" : " "));
					TRACE("%s \t", ((lBits & PSBIT_OWNERONLY)? "Own" : " "));
					TRACE("%s \t", ((lBits & PSBIT_FRIENDLY)? "Frd" : " "));
					TRACE("%s \t", ((lBits & PSBIT_HIDDEN)? "Hide" : " "));
//						TRACE("%s \t", ((lBits & PSBIT_CLIENTPROPERTY)? "Clnt" : " "));
					TRACE("%s \t", ((lBits & PSBIT_INTERNALLYADDED) ? "IntAdd" :" "));
					TRACE("%s \t", ((lBits & PSBIT_INTERNALLYSET)? "IntSet" : " "));
//						TRACE("%s \t", ((lBits & PSBIT_FROMSERVER)? "FrSrv" : " "));

					// Free up property values
					SAFEFREESTRING(bstrMethodName);
					varMethod.Clear();

					// Get next property
					if (FAILED(hr = pMethodsMap->NextItem(&bstrMethodName, &varMethod, &bLastMethValue)))
                        return hr;

					TRACE("\n");
				}
			}

			pMethodsMap.Release();

			// Get next exemplar only if we're not on global (the first one)
			// Otherwise keep variables in memory
			if (!bGlobalDone)
				bGlobalDone = true;
			else
			{
				// Free values for the next exemplar
				SAFEFREESTRING(bstrExemplar);
				varExemplar.Clear();
				// Get next exemplar
				if (FAILED(hr = pExemplarMap->NextItem(&bstrExemplar, &varExemplar, &bLastExemValue)))
                    return hr;
			}
		}
	}

    return hr;
}

//////////////////////////////////////////////////////////
// CreateTimer
// Creates a timer which repeatedly calls a method on a thing at a specified period.

STDMETHODIMP CSystemExemplarObject::CreateTimer(int msecPeriod,
												IThing *pThing,
												BSTR bstrMethod,
												VARIANT varArgs,
												IDispatch **ppTimer)
{
	CComPtr<IVWAnimator>	pVWAnimator;
	CComPtr<IPropertyList>	pArgs;
	DISPPARAMS				dispparams;
	HRESULT					hr = S_OK;

	if (!ppTimer)
		return E_POINTER;
	*ppTimer = NULL;

    if ((pThing == NULL) || (ppTimer == NULL))
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::CreateTimer: NULL pointer\n");
        return ReportCoreError(E_POINTER);
    }

	if (varArgs.vt == VT_ERROR ||
		(varArgs.vt == VT_DISPATCH && !varArgs.pdispVal))
	{
		// Optional param not passed in.
		if (FAILED(hr = ::CreatePropertyList(m_pWorld, &pArgs)))
			return ReportCoreError(hr);
	}
	else if (varArgs.vt != VT_DISPATCH || 
			 FAILED(varArgs.pdispVal->QueryInterface(IID_IPropertyList, (void**)&pArgs)) ||
			 !pArgs)
	{
		return ReportCoreError(DISP_E_TYPEMISMATCH);
	}

	if (FAILED(hr = m_pWorld->get_Tool(CComBSTR("Animator"), (IUnknown**) &pVWAnimator)))
		return hr;

	if (FAILED(hr = ConvertPropertyListToDISPPARAMS(pArgs, &dispparams)))
		return ReportCoreError(hr);

	if (FAILED(hr = pVWAnimator->CreateExt(msecPeriod, pThing,
										   bstrMethod,
									       &dispparams,
										   (IVWAnimation**)ppTimer)))
	{
		TerminateDISPPARAMS(VARIANT_TRUE, &dispparams);
		return hr;
	}

	TerminateDISPPARAMS(VARIANT_TRUE, &dispparams);

	// Insert timer as first arg for method.
	if (FAILED(hr = pArgs->InsertDispatchAt(0, *ppTimer)))
		return hr;
	
	if (FAILED(hr = ConvertPropertyListToDISPPARAMS(pArgs, &dispparams)))
		return ReportCoreError(hr);

	if (FAILED(hr = ((IVWAnimation*)*ppTimer)->put_ArgumentsExt(&dispparams)))
	{
		TerminateDISPPARAMS(VARIANT_TRUE, &dispparams);
		return hr;
	}

	TerminateDISPPARAMS(VARIANT_TRUE, &dispparams);
	return hr;
}

//////////////////////////////////////////////////////////
// DestroyTimer
// Destroys a timer previously created with CreateTimer.

STDMETHODIMP CSystemExemplarObject::DestroyTimer(IDispatch *pTimer)
{
	CComPtr<IVWAnimator>	pVWAnimator;
	HRESULT					hr = S_OK;

    if (pTimer == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::DestroyTimer: NULL pointer\n");
        return ReportCoreError(E_POINTER);
    }

	if (FAILED(hr = m_pWorld->get_Tool(CComBSTR("Animator"), (IUnknown**) &pVWAnimator)))
		return hr;

	hr = pVWAnimator->Remove((IVWAnimation*)pTimer);

	return hr;
}
//////////////////////////////////////////////////////////
// OnWorldOwnerAccess
// Security access method - called by individual OnXXXAccess
// methods when a world owner property (DefaultRoom, 
// WorldName, etc.) is accessed 
// Checks that the caller is the world owner

STDMETHODIMP CSystemExemplarObject::OnWorldOwnerAccess(long lPerms, VARIANT_BOOL *bReturnValue)
{
	HRESULT hr = S_OK;
	CComPtr<IThing> pThis;
	VARIANT_BOOL bOk;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// If you have a pointer, set return value false by default
	if (bReturnValue == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::OnWorldOwnerAccess: NULL output pointer\n");
        return ReportCoreError(E_POINTER);
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

	// Check policy (user must be world owner)
	hr = m_pWorld->CheckPolicy(pThis, CP_WORLDOWNER, CComVariant(0L, VT_ERROR), &bOk);
	if (FAILED(hr))
		return hr;

	// OK, return true and S_OK
	if (bOk==VARIANT_TRUE)
		*bReturnValue = VARIANT_TRUE;

	return hr;
}


//////////////////////////////////////////////////////////
// OnGlobalConstantAccess
// Security access method - doesn't allow anyone to change
// a global constant, such as PSBIT_FRIENDLY or ServerName

STDMETHODIMP CSystemExemplarObject::OnGlobalConstantAccess(long lPerms, VARIANT_BOOL *bReturnValue)
{
	HRESULT hr = S_OK;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

#ifdef _DEBUG
	// No error checking, if we fail in these traces, we still want to keep going
	CComPtr<IThing> pThis, pOwner, pCaller, pUser;
	CComBSTR bstrThisName, bstrOwnerName, bstrCallerName, bstrUserName;

	m_pWorld->get_This(&pThis);
	if (pThis)
	{
		pThis->get_Name(&bstrThisName.m_str);
		pThis->get_Owner(&pOwner);

		if (pOwner)
			pOwner->get_Name(&bstrOwnerName.m_str);
	}

	m_pWorld->get_Caller(&pCaller);
    if (pCaller)
		pCaller->get_Name(&bstrCallerName.m_str);

	m_pWorld->get_User(&pUser);
	if (pUser)
		pUser->get_Name(&bstrUserName.m_str);
#endif

	*bReturnValue = VARIANT_FALSE;

	return hr;
}

// performance testing methods
// get property:
//		normal (on instance)
//		non-existent
//		(not impl) on exemplar (with copy-on-get)
//		(not impl) on exemplar (without copy-on-get)
// put property:
//		normal (on instance)
//		non-existent
//		with OnXXXAccess method (COM and inline)
//		with OnXXXChanged method (COM and inline)
// invoke method:
//		normal (on instance) (COM)
//		normal (on instance) (inline)
//		non-existent
//		(not impl) on exemplar
// 
// other ideas:
//		native property types (BSTR) vs. object properties
//		policy checking
//		OLE events overhead (freeze events?)
//		OnInvokeMethodFailed overhead
//		various argument lengths
//		put_Method
//		propertymap: put_Property
//		propertylist: Add, put_Property

STDMETHODIMP CSystemExemplarObject::TimeGetProperty(IThing* pthing, BSTR bstrPropertyName, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;
	CComVariant var;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	long lStart = timeGetTime();

	for (long i = 0; i < lRepeat && SUCCEEDED(hr); i++)
	{
		hr = pthing->get_PropertyExt(bstrPropertyName, HashKey<BSTR>(bstrPropertyName), NULL, &var);

		if (hr == VWOBJECT_E_PROPERTYNOTEXIST)
			hr = S_OK;

		var.Clear();
	}

	*plElapsed = timeGetTime() - lStart;

	return hr;
}

STDMETHODIMP CSystemExemplarObject::TimePutProperty(IThing* pthing, BSTR bstrPropertyName, VARIANT varData, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	long lStart = timeGetTime();

	for (long i = 0; i < lRepeat && SUCCEEDED(hr); i++)
	{
		// put_PropertyExt not exposed in IDL, call it directly
		hr = ((CThingObject*)pthing)->put_PropertyExt(bstrPropertyName, HashKey<BSTR>(bstrPropertyName), varData);

		if (hr == VWOBJECT_E_PROPERTYNOTEXIST)
			hr = S_OK;
	}

	*plElapsed = timeGetTime() - lStart;

	return hr;
}

STDMETHODIMP CSystemExemplarObject::TimeCloneProperty(VARIANT varData, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	CComVariant var;
	long lStart = timeGetTime();

	for (long i = 0; i < lRepeat && SUCCEEDED(hr); i++)
	{
		hr = m_pWorld->CloneProperty(varData, &var);

		var.Clear();
	}

	*plElapsed = timeGetTime() - lStart;

	return hr;
}
STDMETHODIMP CSystemExemplarObject::TimeCopyProperty(VARIANT varData, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	CComVariant var;
	long lStart = timeGetTime();

	for (long i = 0; i < lRepeat && SUCCEEDED(hr); i++)
	{
		::VariantCopyInd(&var, &varData);

		var.Clear();
	}

	*plElapsed = timeGetTime() - lStart;

	return hr;
}

STDMETHODIMP CSystemExemplarObject::TimePutPropertySecurity(IPropertySecurity* pSecurity, VARIANT varData, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	long lStart = timeGetTime();

	for (long i = 0; i < lRepeat && SUCCEEDED(hr); i++)
	{
		hr = pSecurity->put_Property(varData);
	}

	*plElapsed = timeGetTime() - lStart;

	return hr;
}

STDMETHODIMP CSystemExemplarObject::TimeInvokeMethod(IThing* pthing, BSTR bstrMethodName, VARIANT varArgs, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	DISPPARAMS dispparams;
	DISPPARAMS* pdispparams = NULL;
	CComVariant varTemp;
	CComVariant var;
	long i = 0;
	long lStart = 0;

	InitializeDISPPARAMS(&dispparams, 0);

	// VT_ERROR means optional arg not passed in
	if (varArgs.vt != VT_ERROR)
	{
		hr = VariantCopyInd(&varTemp, &varArgs);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = ConvertJSArrayToDISPPARAMS(&varTemp, &dispparams);
		if (FAILED(hr))
		{
			hr = ConvertSAFEARRAYToDISPPARAMS(&varTemp, &dispparams);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}

		pdispparams = &dispparams;
	}
	else
		pdispparams = NULL;

	lStart = timeGetTime();

	for (; i < lRepeat && SUCCEEDED(hr); i++)
	{
		hr = pthing->InvokeMethodExt(bstrMethodName, pdispparams, &var);

		var.Clear();

		if (hr == VWOBJECT_E_METHODNOTEXIST)
			hr = S_OK;
	}

	*plElapsed = timeGetTime() - lStart;

ERROR_ENCOUNTERED:	
	TerminateDISPPARAMS(VARIANT_TRUE, &dispparams);

	return hr;
}

STDMETHODIMP CSystemExemplarObject::TimeInterfaceInvokeMethod(IThing* pthing, BSTR bstrMethodName, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;
	CComPtr<IMethod> pMethod;
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	hr = pthing->get_MethodExt(bstrMethodName, HashKey<BSTR>(bstrMethodName), NULL, &pMethod);
	if (FAILED(hr))
		return hr;

	CComVariant var;
	HRESULT invokehr = S_OK;
	long lStart = timeGetTime();

	for (long i = 0; i < lRepeat && SUCCEEDED(hr); i++)
	{
		hr = pMethod->InvokeMethodExt(&dispparamsNoArgs, &invokehr, &var);

		var.Clear();
	}

	*plElapsed = timeGetTime() - lStart;

	return hr;
}

STDMETHODIMP CSystemExemplarObject::TimeVTableInvokeMethod(long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	long lStart = timeGetTime();

	for (long i = 0; i < lRepeat && SUCCEEDED(hr); i++)
	{
		hr = YYY();
	}

	*plElapsed = timeGetTime() - lStart;

	return hr;
}

STDMETHODIMP CSystemExemplarObject::TimeAutomationInvokeMethod(long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;
	static CComBSTR bstrName("YYY");

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	DISPID dispid = DISPID_UNKNOWN;
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
	UINT uArgErr = 0;
	CComVariant var;
	EXCEPINFO ei;

	memset(&ei, 0x0, sizeof(EXCEPINFO));

	long lStart = timeGetTime();
	long i = 0;

	hr = GetIDsOfNames(IID_NULL, &bstrName.m_str, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(hr) || dispid == DISPID_UNKNOWN)
		goto ERROR_ENCOUNTERED;

	for ( ; i < lRepeat && SUCCEEDED(hr); i++)
	{
		hr = Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparamsNoArgs, 
			&var, &ei, &uArgErr);

		var.Clear();
	}

ERROR_ENCOUNTERED:
	*plElapsed = timeGetTime() - lStart;

	return hr;
}

// includes <scripts.h> and defines DEFAULT_TIMEOUT
#include <scrptobj.h>

STDMETHODIMP CSystemExemplarObject::TimeScriptInvokeMethod(long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;
	static CComBSTR bstrName("YYY");
	static CComBSTR bstrMethodCode("sub YYY:end sub");

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	CScriptSite m_ScriptSite;
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
	static CComBSTR bstrWorld("World");
	CComVariant var;
	HRESULT invokehr = S_OK;
	long i = 0;
	long lStart = 0;

	hr = m_ScriptSite.AddNamedItem(CWorldObject::g_strIDSType, m_pWorld, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// initialize the script engine
	hr = m_ScriptSite.LoadScript(CString(":VBScript"), CWorldObject::g_strIDSType);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// stuff inline script into engine
	hr = m_ScriptSite.InvokeScriptlet(CString(bstrMethodCode), CWorldObject::g_strIDSType, DEFAULT_TIMEOUT);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	lStart = timeGetTime();

	for ( ; i < lRepeat && SUCCEEDED(hr); i++)
	{
		hr = m_ScriptSite.InvokeByName(bstrName, bstrWorld, DISPATCH_METHOD, &dispparamsNoArgs, &var, &invokehr, DEFAULT_TIMEOUT);

		var.Clear();
	}

ERROR_ENCOUNTERED:
	*plElapsed = timeGetTime() - lStart;

	return hr;
}

STDMETHODIMP CSystemExemplarObject::TimeGetMethod(IThing* pthing, BSTR bstrMethodName, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;
	CComPtr<IMethod> pMethod;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	long lStart = timeGetTime();

	for (long i = 0; i < lRepeat && SUCCEEDED(hr); i++)
	{
		hr = pthing->get_MethodExt(bstrMethodName, HashKey<BSTR>(bstrMethodName), NULL, &pMethod);

		if (hr == VWOBJECT_E_METHODNOTEXIST)
			hr = S_OK;

		pMethod.Release();
	}

	*plElapsed = timeGetTime() - lStart;

	return hr;
}

STDMETHODIMP CSystemExemplarObject::TimeGetPropertySecurity(IThing* pthing, BSTR bstrPropertyName, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;
	IPropertySecurity* pSec = NULL;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	long lStart = timeGetTime();

	for (long i = 0; i < lRepeat && SUCCEEDED(hr); i++)
	{
		hr = pthing->get_PropertySecurityExt(bstrPropertyName, HashKey<BSTR>(bstrPropertyName), NULL, NULL, &pSec);

		if (hr == VWOBJECT_E_PROPERTYNOTEXIST)
			hr = S_OK;

		SAFERELEASE(pSec);
	}

	*plElapsed = timeGetTime() - lStart;

	return hr;
}

STDMETHODIMP CSystemExemplarObject::TimeCheckPropertySecurity(IThing* pthing, BSTR bstrPropertyName, unsigned long ulFlags, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	long lStart = timeGetTime();

	for (long i = 0; i < lRepeat && SUCCEEDED(hr); i++)
	{
		hr = pthing->CheckPropertySecurityExt(bstrPropertyName, ulFlags);

		if (hr == VWOBJECT_E_PERMISSION_DENIED)
			hr = S_OK;
	}

	*plElapsed = timeGetTime() - lStart;

	return hr;
}

STDMETHODIMP CSystemExemplarObject::TimeCheckMethodSecurity(IThing* pthing, BSTR bstrMethodName, unsigned long ulFlags, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	long lStart = timeGetTime();

	for (long i = 0; i < lRepeat && SUCCEEDED(hr); i++)
	{
		hr = pthing->CheckMethodSecurityExt(bstrMethodName, ulFlags);

		if (hr == VWOBJECT_E_PERMISSION_DENIED)
			hr = S_OK;
	}

	*plElapsed = timeGetTime() - lStart;

	return hr;
}

STDMETHODIMP CSystemExemplarObject::TimeCheckPropertiesSecurity(IThing* pthing, unsigned long ulFlags, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	long lStart = timeGetTime();

	for (long i = 0; i < lRepeat && SUCCEEDED(hr); i++)
	{
		hr = pthing->CheckPropertiesSecurityExt(ulFlags);

		if (hr == VWOBJECT_E_PERMISSION_DENIED)
			hr = S_OK;
	}

	*plElapsed = timeGetTime() - lStart;

	return hr;
}

STDMETHODIMP CSystemExemplarObject::TimeCheckMethodsSecurity(IThing* pthing, unsigned long ulFlags, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	long lStart = timeGetTime();

	for (long i = 0; i < lRepeat && SUCCEEDED(hr); i++)
	{
		hr = pthing->CheckMethodsSecurityExt(ulFlags);

		if (hr == VWOBJECT_E_PERMISSION_DENIED)
			hr = S_OK;
	}

	*plElapsed = timeGetTime() - lStart;

	return hr;
}

STDMETHODIMP CSystemExemplarObject::TimeNotifyInvokeMethod(IThing* pthing, BSTR bstrMethodName, DISPPARAMS* pdispparams, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	long lStart = timeGetTime();

	for (long i = 0; i < lRepeat && SUCCEEDED(hr); i++)
	{
		hr = m_pWorld->NotifyInvokeMethod(pthing, bstrMethodName, pdispparams);
	}

	*plElapsed = timeGetTime() - lStart;

	return hr;
}

STDMETHODIMP CSystemExemplarObject::TimeNotifyPropertyChanged(IThing* pthing, BSTR bstrPropertyName, long lHint, VARIANT varHintData, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	long lStart = timeGetTime();

	for (long i = 0; i < lRepeat && SUCCEEDED(hr); i++)
	{
		hr = m_pWorld->NotifyPropertyChanged(pthing, bstrPropertyName, lHint, varHintData);
	}

	*plElapsed = timeGetTime() - lStart;

	return hr;
}

STDMETHODIMP CSystemExemplarObject::TimeSendThingCommand(long lFlags, IThing* pthing, DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	long lStart = timeGetTime();

	// bReply = no reply, bSync = async
	for (long i = 0; i < lRepeat && SUCCEEDED(hr); i++)
	{
		hr = m_pWorld->SendThingCommandExt(VARIANT_FALSE, VARIANT_FALSE, lFlags, pthing, dispid, wFlags, pdispparams, NULL, NULL); 
	}

	*plElapsed = timeGetTime() - lStart;

	return hr;
}

STDMETHODIMP CSystemExemplarObject::TimeSendObjectPropertyCommand(long lFlags, IObjectProperty* pprop, DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	CComVariant var;
	HRESULT recvhr = S_OK;
	long lStart = timeGetTime();

	// bReply = no reply, bSync = async
	for (long i = 0; i < lRepeat && SUCCEEDED(hr); i++)
	{
		hr = m_pWorld->SendObjectPropertyCommandExt(VARIANT_FALSE, VARIANT_FALSE, lFlags, pprop, dispid, wFlags, pdispparams, &recvhr, &var); 

		var.Clear();
	}

	*plElapsed = timeGetTime() - lStart;

	return hr;
}

STDMETHODIMP CSystemExemplarObject::TimeSendWorldCommand(DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	CComVariant var;
	HRESULT recvhr = S_OK;
	long lStart = timeGetTime();

	// bReply = no reply, bSync = async
	for (long i = 0; i < lRepeat && SUCCEEDED(hr); i++)
	{
		hr = m_pWorld->SendWorldCommandExt(VARIANT_FALSE, VARIANT_FALSE, dispid, wFlags, pdispparams, &recvhr, &var); 

		var.Clear();
	}

	*plElapsed = timeGetTime() - lStart;

	return hr;
}

STDMETHODIMP CSystemExemplarObject::TimeProcessMessage(unsigned char ch, IUnMarshallBuffer* pUBuffer, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	CComVariant var;
	HRESULT processhr = S_OK;
	long lStart = timeGetTime();

	for (long i = 0; i < lRepeat && SUCCEEDED(hr); i++)
	{
		hr = m_pWorld->ProcessMessageExt(ch, pUBuffer, &processhr, &var);

		var.Clear();

		// reset for another go
		pUBuffer->SeekToBegin();
	}

	*plElapsed = timeGetTime() - lStart;

	return hr;
}

CString CStringFromBits(unsigned long ulFlags)
{
	if (ulFlags == PS_READ)
		return CString("PS_READ");
	else if (ulFlags == PS_WRITE)
		return CString("PS_WRITE");
	else if (ulFlags == PS_EXECUTE)
		return CString("PS_EXECUTE");
	else
		return CString("PS_???");
}

CString CStringFromVT(unsigned int vt)
{
	if (vt == VT_BSTR)
		return CString("VT_BSTR");
	else if (vt == VT_I4)
		return CString("VT_I4");
	else if (vt == VT_I2)
		return CString("VT_I2");
	else if (vt == VT_BOOL)
		return CString("VT_BOOL");
	else if (vt == VT_DISPATCH)
		return CString("VT_DISPATCH");
	else
		return CString("VT_???");
}

STDMETHODIMP CSystemExemplarObject::TimeGetPropertyAll(VARIANT varData, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;
	CComPtr<IThing> pGlobal;
	static CComBSTR bstrProperty("XXX");
	static CComBSTR bstrPropertyNotExist("XXXNotExist");
	long lElapsed = 0L;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	hr = m_pWorld->get_Global(&pGlobal);
	if (FAILED(hr))
		return hr;

	hr = pGlobal->AddPropertyExt(bstrProperty, varData, PSBIT_SCRIPTDEFAULTPROPERTY | PSBIT_NOTREMOTED, 
		PS_SCRIPTDEFAULTPROPERTY, varData.vt, IID_NULL, NULL);
	if (FAILED(hr))
		return hr;

	// time normal get property
	hr = TimeGetProperty(pGlobal, bstrProperty, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeGetPropertyAll: %d times, %s, normal, %6.4f msec avg\n", lRepeat, CStringFromVT(varData.vt), ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	hr = pGlobal->RemoveProperty(bstrProperty);
	if (FAILED(hr))
		return hr;
	
	// time non-existent get property
	hr = TimeGetProperty(pGlobal, bstrPropertyNotExist, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeGetPropertyAll: %d times, %s, not exist, %6.4f msec avg\n", lRepeat, CStringFromVT(varData.vt), ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	return S_OK;
}

STDMETHODIMP CSystemExemplarObject::TimePutPropertyAll(VARIANT varData, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;
	CComPtr<IThing> pGlobal;
	CComPtr<IModule> pModule;
	CComPtr<IMethod> pMethodAccess;
	CComPtr<IMethod> pMethodChanged;
	static CComBSTR bstrProperty("XXX");
	static CComBSTR bstrMethodAccess("OnXXXAccess");
	static CComBSTR bstrMethodChanged("OnXXXChanged");
	static CComBSTR bstrPropertyNotExist("XXXNotExist");
	static CComBSTR bstrMethodAccessCode("function OnXXXAccess(perms):OnXXXAccess = True:end function");
	static CComBSTR bstrMethodChangedCode("sub OnXXXChanged(hint, hintdata):end sub");
	long lElapsed = 0L;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	// time clone property
	hr = TimeCloneProperty(CComVariant(varData), lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimePutPropertyAll: %d times, %s, clone, %6.4f msec avg\n", lRepeat, CStringFromVT(varData.vt), ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	// time VariantCopyInd
	hr = TimeCopyProperty(CComVariant(varData), lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimePutPropertyAll: %d times, %s, copy, %6.4f msec avg\n", lRepeat, CStringFromVT(varData.vt), ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	hr = m_pWorld->get_Global(&pGlobal);
	if (FAILED(hr))
		return hr;

	{
		IPropertySecurity* pSecurity = new CComPropertySecurity;
		if (pSecurity == NULL)
		{
			hr = E_OUTOFMEMORY;
			return hr;
		}

		SAFEADDREF(pSecurity);

		pSecurity->put_World(m_pWorld);
		
		// time put property into security slot
		hr = TimePutPropertySecurity(pSecurity, CComVariant(varData), lRepeat, &lElapsed);

		SAFERELEASE(pSecurity);

		if (FAILED(hr))
			return hr;

		VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimePutPropertyAll: %d times, %s, put into security, %6.4f msec avg\n", lRepeat, CStringFromVT(varData.vt), ((float)lElapsed/(float)lRepeat));

		*plElapsed += lElapsed;
	}

	hr = m_pWorld->get_Module(CComBSTR("System"), &pModule);
	if (FAILED(hr))
		return hr;

	hr = pGlobal->AddPropertyExt(bstrProperty, varData, PSBIT_SCRIPTDEFAULTPROPERTY /* | PSBIT_NOTREMOTED */, 
		PS_SCRIPTDEFAULTPROPERTY, varData.vt, IID_NULL, NULL);
	if (FAILED(hr))
		return hr;

	// time normal put property
	hr = TimePutProperty(pGlobal, bstrProperty, CComVariant(varData), lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimePutPropertyAll: %d times, %s, normal, %6.4f msec avg\n", lRepeat, CStringFromVT(varData.vt), ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	// time non-existent put property
	hr = TimePutProperty(pGlobal, bstrPropertyNotExist, CComVariant(varData), lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimePutPropertyAll: %d times, %s, not exist, %6.4f msec avg\n", lRepeat, CStringFromVT(varData.vt), ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	hr = pGlobal->CreateAndAddMethodExt(METHOD_CLIENT | METHOD_SERVER, pModule, 
		bstrMethodAccess, bstrMethodAccess, PSBIT_SCRIPTDEFAULTMETHOD /* | PSBIT_NOTREMOTED */, PS_ALLEXECUTEMETHOD);
	if (FAILED(hr))
		return hr;

	// time put property with OnXXXAccess method (found in COM module)
	hr = TimePutProperty(pGlobal, bstrProperty, CComVariant(varData), lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimePutPropertyAll: %d times, %s, OnXXXAccess (COM), %6.4f msec avg\n", lRepeat, CStringFromVT(varData.vt), ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	hr = pGlobal->RemoveMethod(bstrMethodAccess);
	if (FAILED(hr))
		return hr;

	hr = pGlobal->CreateAndAddMethodExt(METHOD_CLIENT | METHOD_SERVER, pModule, 
		bstrMethodChanged, bstrMethodChanged, PSBIT_SCRIPTDEFAULTMETHOD /* | PSBIT_NOTREMOTED */, PS_ALLEXECUTEMETHOD);
	if (FAILED(hr))
		return hr;

	// time put property with OnXXXChanged method (found in COM module)
	hr = TimePutProperty(pGlobal, bstrProperty, CComVariant(varData), lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimePutPropertyAll: %d times, %s, OnXXXChanged (COM), %6.4f msec avg\n", lRepeat, CStringFromVT(varData.vt), ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	hr = pGlobal->RemoveMethod(bstrMethodChanged);
	if (FAILED(hr))
		return hr;

	hr = m_pWorld->CreateInlineMethod(METHOD_CLIENT | METHOD_SERVER, CComBSTR("VBScript"), 
		bstrMethodAccess, bstrMethodAccessCode, &pMethodAccess);
	if (FAILED(hr))
		return hr;

	hr = pGlobal->AddMethodExt(bstrMethodAccess, pMethodAccess, PSBIT_SCRIPTDEFAULTMETHOD /* | PSBIT_NOTREMOTED */, PS_ALLEXECUTEMETHOD);
	if (FAILED(hr))
		return hr;

	// time put property with OnXXXAccess inline method
	hr = TimePutProperty(pGlobal, bstrProperty, CComVariant(varData), lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimePutPropertyAll: %d times, %s, OnXXXAccess (inline VBS), %6.4f msec avg\n", lRepeat, CStringFromVT(varData.vt), ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	hr = pGlobal->RemoveMethod(bstrMethodAccess);
	if (FAILED(hr))
		return hr;

	hr = m_pWorld->CreateInlineMethod(METHOD_CLIENT | METHOD_SERVER, CComBSTR("VBScript"), 
		bstrMethodChanged, bstrMethodChangedCode, &pMethodChanged);
	if (FAILED(hr))
		return hr;

	hr = pGlobal->AddMethodExt(bstrMethodChanged, pMethodChanged, PSBIT_SCRIPTDEFAULTMETHOD /* | PSBIT_NOTREMOTED */, PS_ALLEXECUTEMETHOD);
	if (FAILED(hr))
		return hr;

	// time put property with OnXXXChanged inline method
	hr = TimePutProperty(pGlobal, bstrProperty, CComVariant(varData), lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimePutPropertyAll: %d times, %s, OnXXXChanged (inline VBS), %6.4f msec avg\n", lRepeat, CStringFromVT(varData.vt), ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	hr = pGlobal->RemoveMethod(bstrMethodChanged);
	if (FAILED(hr))
		return hr;

	hr = pGlobal->RemoveProperty(bstrProperty);
	if (FAILED(hr))
		return hr;

	return S_OK;
}

STDMETHODIMP CSystemExemplarObject::TimeInvokeMethodAll(long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;
	CComPtr<IThing> pGlobal;
	CComPtr<IModule> pModule;
	CComPtr<IMethod> pMethod;
	static CComBSTR bstrMethod("YYY");
	static CComBSTR bstrMethodNotExist("YYYNotExist");
	static CComBSTR bstrMethodCode("sub YYY:end sub");
	long lElapsed = 0L;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	hr = m_pWorld->get_Module(CComBSTR("System"), &pModule);
	if (FAILED(hr))
		return hr;

	hr = m_pWorld->get_Global(&pGlobal);
	if (FAILED(hr))
		return hr;

	hr = pGlobal->CreateAndAddMethodExt(METHOD_CLIENT | METHOD_SERVER, pModule, 
		bstrMethod, bstrMethod, PSBIT_SCRIPTDEFAULTMETHOD /* | PSBIT_NOTREMOTED */, PS_ALLEXECUTEMETHOD);
	if (FAILED(hr))
		return hr;

	// time normal method invocation (found in COM module)
	hr = TimeInvokeMethod(pGlobal, bstrMethod, CComVariant(0L, VT_ERROR), lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeInvokeMethodAll: %d times, normal (COM), %6.4f msec avg\n", lRepeat, ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	// time direct-to-interface method invocation (found in COM module)
	hr = TimeInterfaceInvokeMethod(pGlobal, bstrMethod, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeInvokeMethodAll: %d times, interface (COM), %6.4f msec avg\n", lRepeat, ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	hr = pGlobal->RemoveMethod(bstrMethod);
	if (FAILED(hr))
		return hr;
	
	hr = m_pWorld->CreateInlineMethod(METHOD_CLIENT | METHOD_SERVER, CComBSTR("VBScript"), 
		bstrMethod, bstrMethodCode, &pMethod);
	if (FAILED(hr))
		return hr;

	hr = pGlobal->AddMethodExt(bstrMethod, pMethod, PSBIT_SCRIPTDEFAULTMETHOD /* | PSBIT_NOTREMOTED */, PS_ALLEXECUTEMETHOD);
	if (FAILED(hr))
		return hr;

	// time normal inline method invocation
	hr = TimeInvokeMethod(pGlobal, bstrMethod, CComVariant(0L, VT_ERROR), lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeInvokeMethodAll: %d times, normal (inline), %6.4f msec avg\n", lRepeat, ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	// time direct-to-interface inline method invocation
	hr = TimeInterfaceInvokeMethod(pGlobal, bstrMethod, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeInvokeMethodAll: %d times, interface (inline), %6.4f msec avg\n", lRepeat, ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	hr = pGlobal->RemoveMethod(bstrMethod);
	if (FAILED(hr))
		return hr;

	// time non-existent method invocation
	hr = TimeInvokeMethod(pGlobal, bstrMethodNotExist, CComVariant(0L, VT_ERROR), lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeInvokeMethodAll: %d times, not exist, %6.4f msec avg\n", lRepeat, ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	// time vtable COM method invocation
	hr = TimeVTableInvokeMethod(lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeInvokeMethodAll: %d times, direct, vtable (COM), %6.4f msec avg\n", lRepeat, ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;
	
	// time automation COM method invocation
	hr = TimeAutomationInvokeMethod(lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeInvokeMethodAll: %d times, direct, automation (COM), %6.4f msec avg\n", lRepeat, ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	// time script method invocation
	hr = TimeScriptInvokeMethod(lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeInvokeMethodAll: %d times, direct, automation (inline), %6.4f msec avg\n", lRepeat, ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	return S_OK;
}

STDMETHODIMP CSystemExemplarObject::TimeGetMethodAll(long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;
	CComPtr<IThing> pGlobal;
	CComPtr<IModule> pModule;
	CComPtr<IMethod> pMethod;
	static CComBSTR bstrMethod("YYY");
	static CComBSTR bstrMethodNotExist("YYYNotExist");
	long lElapsed = 0L;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	hr = m_pWorld->get_Module(CComBSTR("System"), &pModule);
	if (FAILED(hr))
		return hr;

	hr = m_pWorld->get_Global(&pGlobal);
	if (FAILED(hr))
		return hr;

	hr = pGlobal->CreateAndAddMethodExt(METHOD_CLIENT | METHOD_SERVER, pModule, 
		bstrMethod, bstrMethod, PSBIT_SCRIPTDEFAULTMETHOD | PSBIT_NOTREMOTED, PS_ALLEXECUTEMETHOD);
	if (FAILED(hr))
		return hr;

	// time normal get method
	hr = TimeGetMethod(pGlobal, bstrMethod, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeGetMethodAll: %d times, normal, %6.4f msec avg\n", lRepeat, ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	hr = pGlobal->RemoveMethod(bstrMethod);
	if (FAILED(hr))
		return hr;
	
	// time non-existent get method
	hr = TimeGetMethod(pGlobal, bstrMethodNotExist, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeGetMethodAll: %d times, not exist, %6.4f msec avg\n", lRepeat, ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	return S_OK;
}

STDMETHODIMP CSystemExemplarObject::TimeGetPropertySecurityAll(long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;
	CComPtr<IThing> pGlobal;
	static CComBSTR bstrProperty("XXX");
	static CComBSTR bstrPropertyNotExist("XXXNotExist");
	long lElapsed = 0L;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	hr = m_pWorld->get_Global(&pGlobal);
	if (FAILED(hr))
		return hr;

	hr = pGlobal->AddPropertyExt(bstrProperty, CComVariant(0L), PSBIT_SCRIPTDEFAULTPROPERTY | PSBIT_NOTREMOTED, 
		PS_SCRIPTDEFAULTPROPERTY, VT_I4, IID_NULL, NULL);
	if (FAILED(hr))
		return hr;

	// time normal get property security
	hr = TimeGetPropertySecurity(pGlobal, bstrProperty, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeGetPropertySecurityAll: %d times, normal, %6.4f msec avg\n", lRepeat, ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	hr = pGlobal->RemoveProperty(bstrProperty);
	if (FAILED(hr))
		return hr;
	
	// time non-existent get property security
	hr = TimeGetPropertySecurity(pGlobal, bstrPropertyNotExist, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeGetPropertySecurityAll: %d times, not exist, %6.4f msec avg\n", lRepeat, ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	return S_OK;
}

STDMETHODIMP CSystemExemplarObject::TimeCheckPropertySecurityAll(unsigned long ulFlags, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;
	CComPtr<IThing> pGlobal;
	static CComBSTR bstrProperty("XXX");
	long lElapsed = 0L;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	hr = m_pWorld->get_Global(&pGlobal);
	if (FAILED(hr))
		return hr;

	// make sure bit exists
	hr = pGlobal->AddPropertyExt(bstrProperty, CComVariant(0L), PSBIT_SCRIPTDEFAULTPROPERTY | PSBIT_NOTREMOTED, 
		PS_SCRIPTDEFAULTPROPERTY, VT_I4, IID_NULL, NULL);
	if (FAILED(hr))
		return hr;

	// time normal check property security
	hr = TimeCheckPropertySecurity(pGlobal, bstrProperty, ulFlags, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeCheckPropertySecurityAll: %d times, %s, normal, %6.4f msec avg\n", lRepeat, CStringFromBits(ulFlags), ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	hr = pGlobal->RemoveProperty(bstrProperty);
	if (FAILED(hr))
		return hr;

	// make sure bit doesn't exist
	hr = pGlobal->AddPropertyExt(bstrProperty, CComVariant(0L), PSBIT_SCRIPTDEFAULTPROPERTY | PSBIT_NOTREMOTED, 
		PS_ADD, VT_I4, IID_NULL, NULL);
	if (FAILED(hr))
		return hr;

	// time normal check property security
	hr = TimeCheckPropertySecurity(pGlobal, bstrProperty, ulFlags, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeCheckPropertySecurityAll: %d times, %s, permission denied, %6.4f msec avg\n", lRepeat, CStringFromBits(ulFlags), ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	hr = pGlobal->RemoveProperty(bstrProperty);
	if (FAILED(hr))
		return hr;

	return S_OK;
}

STDMETHODIMP CSystemExemplarObject::TimeCheckMethodSecurityAll(unsigned long ulFlags, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;
	CComPtr<IThing> pGlobal;
	CComPtr<IModule> pModule;
	CComPtr<IMethod> pMethod;
	static CComBSTR bstrMethod("YYY");
	long lElapsed = 0L;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	hr = m_pWorld->get_Module(CComBSTR("System"), &pModule);
	if (FAILED(hr))
		return hr;

	hr = m_pWorld->get_Global(&pGlobal);
	if (FAILED(hr))
		return hr;

	// make sure bit exists
	hr = pGlobal->CreateAndAddMethodExt(METHOD_CLIENT | METHOD_SERVER, pModule, 
		bstrMethod, bstrMethod, PSBIT_SCRIPTDEFAULTMETHOD | PSBIT_NOTREMOTED, PS_ALLEXECUTEMETHOD);
	if (FAILED(hr))
		return hr;

	// time normal check method security
	hr = TimeCheckMethodSecurity(pGlobal, bstrMethod, ulFlags, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeCheckMethodSecurityAll: %d times, %s, normal, %6.4f msec avg\n", lRepeat, CStringFromBits(ulFlags), ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	hr = pGlobal->RemoveMethod(bstrMethod);
	if (FAILED(hr))
		return hr;

	// make sure bit doesn't exist
	hr = pGlobal->CreateAndAddMethodExt(METHOD_CLIENT | METHOD_SERVER, pModule, 
		bstrMethod, bstrMethod, PSBIT_SCRIPTDEFAULTMETHOD | PSBIT_NOTREMOTED, PS_ADD);
	if (FAILED(hr))
		return hr;

	// time normal check Method security
	hr = TimeCheckMethodSecurity(pGlobal, bstrMethod, ulFlags, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeCheckMethodSecurityAll: %d times, %s, permission denied, %6.4f msec avg\n", lRepeat, CStringFromBits(ulFlags), ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	hr = pGlobal->RemoveMethod(bstrMethod);
	if (FAILED(hr))
		return hr;

	return S_OK;
}

STDMETHODIMP CSystemExemplarObject::TimeCheckPropertiesSecurityAll(unsigned long ulFlags, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;
	CComPtr<IThing> pGlobal;
	long lElapsed = 0L;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	hr = m_pWorld->get_Global(&pGlobal);
	if (FAILED(hr))
		return hr;

	// time normal check properties security
	hr = TimeCheckPropertiesSecurity(pGlobal, ulFlags, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeCheckPropertiesSecurityAll: %d times, %s, normal, %6.4f msec avg\n", lRepeat, CStringFromBits(ulFlags), ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	return S_OK;
}

STDMETHODIMP CSystemExemplarObject::TimeCheckMethodsSecurityAll(unsigned long ulFlags, long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;
	CComPtr<IThing> pGlobal;
	long lElapsed = 0L;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	hr = m_pWorld->get_Global(&pGlobal);
	if (FAILED(hr))
		return hr;

	// time normal check methods security
	hr = TimeCheckMethodsSecurity(pGlobal, ulFlags, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeCheckMethodsSecurityAll: %d times, %s, normal, %6.4f msec avg\n", lRepeat, CStringFromBits(ulFlags), ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	return S_OK;
}

STDMETHODIMP CSystemExemplarObject::TimeNotifyAll(long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;
	CComPtr<IThing> pGlobal;
	static CComBSTR bstrProperty("XXX");
	static CComBSTR bstrMethod("YYY");
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
	CComPtr<IPropertyList> pPropList;

	long lElapsed = 0L;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	hr = ::CreatePropertyList(m_pWorld, &pPropList);
	if (FAILED(hr) || pPropList == NULL)
		return hr;

	hr = m_pWorld->get_Global(&pGlobal);
	if (FAILED(hr))
		return hr;

	// time notify property changed (VT_I4)
	hr = TimeNotifyPropertyChanged(pGlobal, bstrProperty, thPut, CComVariant(0L), lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeNotifyAll: OnPropertyChanged(\"%s\", thPut, VT_I4), %d times, %6.4f msec avg\n", CString(bstrProperty), lRepeat, ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;
	
	// time notify property changed (VT_BSTR)
	hr = TimeNotifyPropertyChanged(pGlobal, bstrProperty, thPut, CComVariant(CComBSTR("0")), lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeNotifyAll: OnPropertyChanged(\"%s\", thPut, VT_BSTR), %d times, %6.4f msec avg\n", CString(bstrProperty), lRepeat, ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	// time notify property changed (VT_DISPATCH)
	hr = TimeNotifyPropertyChanged(pGlobal, bstrProperty, thPut, CComVariant(pPropList), lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeNotifyAll: OnPropertyChanged(\"%s\", thPut, VT_DISPATCH), %d times, %6.4f msec avg\n", CString(bstrProperty), lRepeat, ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	// time notify method invocation
	hr = TimeNotifyInvokeMethod(pGlobal, bstrMethod, &dispparamsNoArgs, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeNotifyAll: OnInvokeMethod(\"%s\"), %d times, %6.4f msec avg\n", CString(bstrMethod), lRepeat, ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;
	
	return S_OK;
}

STDMETHODIMP CSystemExemplarObject::TimeSendCommandAll(long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;
	CComPtr<IThing> pGlobal;
	CComPtr<IModule> pModule;
	long lElapsed = 0L;
	DISPPARAMS dispparams;
	CComVariant varData(0L);
	static CComBSTR bstrProperty("XXX");
	static CComBSTR bstrMethod("YYY");
	long lStart = 0L;
	long i = 0;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	hr = m_pWorld->get_Global(&pGlobal);
	if (FAILED(hr))
		return hr;

	// time initialize/terminate DISPPARAMS for propput
	lStart = timeGetTime();

	for (i = 0; i < lRepeat && SUCCEEDED(hr); i++)
	{
		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 2);
		if (FAILED(hr))
			return hr;

		dispparams.rgvarg[1].vt = VT_BSTR;
		dispparams.rgvarg[1].bstrVal = bstrProperty;

		dispparams.rgvarg[0] = varData;

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		return hr;

	lElapsed = timeGetTime() - lStart;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeSendCommandAll: thing, dispparams, DISPID_THING_PROPERTY [propput] VT_I4, %d times, %6.4f msec avg\n", lRepeat, ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	hr = pGlobal->AddPropertyExt(bstrProperty, varData, PSBIT_SCRIPTDEFAULTPROPERTY /* | PSBIT_NOTREMOTED */, 
		PS_SCRIPTDEFAULTPROPERTY, varData.vt, IID_NULL, NULL);
	if (FAILED(hr))
		return hr;

	// package up args into DISPPARAMS struct
	hr = InitializeDISPPARAMS(&dispparams, 2);
	if (FAILED(hr))
		return hr;

	dispparams.rgvarg[1].vt = VT_BSTR;
	dispparams.rgvarg[1].bstrVal = bstrProperty;

	dispparams.rgvarg[0] = varData;

	// time send thing command
	hr = TimeSendThingCommand(PSBIT_NONE, pGlobal, DISPID_THING_PROPERTY, DISPATCH_PROPERTYPUT, &dispparams, lRepeat, &lElapsed);

	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeSendCommandAll: thing, DISPID_THING_PROPERTY [propput] VT_I4, %d times, %6.4f msec avg\n", lRepeat, ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	hr = pGlobal->RemoveProperty(bstrProperty);
	if (FAILED(hr))
		return hr;

	hr = m_pWorld->get_Module(CComBSTR("System"), &pModule);
	if (FAILED(hr))
		return hr;

	// time initialize/terminate DISPPARAMS for invokemethod
	lStart = timeGetTime();

	for (i = 0; i < lRepeat && SUCCEEDED(hr); i++)
	{
		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 3);
		if (FAILED(hr))
			return hr;

		dispparams.rgvarg[2].vt = VT_DISPATCH;
		dispparams.rgvarg[2].pdispVal = pGlobal;

		dispparams.rgvarg[1].vt = VT_BSTR;
		dispparams.rgvarg[1].bstrVal = bstrMethod;

		// NOTE: if NULL dispparams passed in, we need to make sure
		// it's treated as such on other side - not as 0 parameters
		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = NULL;

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		return hr;

	lElapsed = timeGetTime() - lStart;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeSendCommandAll: thing, dispparams, DISPID_THING_INVOKEMETHOD, %d times, %6.4f msec avg\n", lRepeat, ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	hr = pGlobal->CreateAndAddMethodExt(METHOD_CLIENT | METHOD_SERVER, pModule, 
		bstrMethod, bstrMethod, PSBIT_SCRIPTDEFAULTMETHOD /* | PSBIT_NOTREMOTED */, PS_ALLEXECUTEMETHOD);
	if (FAILED(hr))
		return hr;

	// package up args into DISPPARAMS struct
	hr = InitializeDISPPARAMS(&dispparams, 3);
	if (FAILED(hr))
		return hr;

	dispparams.rgvarg[2].vt = VT_DISPATCH;
	dispparams.rgvarg[2].pdispVal = pGlobal;

	dispparams.rgvarg[1].vt = VT_BSTR;
	dispparams.rgvarg[1].bstrVal = bstrMethod;

	// NOTE: if NULL dispparams passed in, we need to make sure
	// it's treated as such on other side - not as 0 parameters
	dispparams.rgvarg[0].vt = VT_DISPATCH;
	dispparams.rgvarg[0].pdispVal = NULL;

	// time send thing command
	hr = TimeSendThingCommand(PSBIT_NONE, pGlobal, DISPID_THING_INVOKEMETHODINT, DISPATCH_METHOD, &dispparams, lRepeat, &lElapsed);

	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeSendCommandAll: thing, DISPID_THING_INVOKEMETHOD, %d times, %6.4f msec avg\n", lRepeat, ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	hr = pGlobal->RemoveMethod(bstrMethod);
	if (FAILED(hr))
		return hr;
	
	return hr;
}

STDMETHODIMP CSystemExemplarObject::TimeProcessMessageAll(long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;
	CComPtr<IThing> pGlobal;
	CComPtr<IModule> pModule;
	CComPtr<IUnMarshallBuffer> pUBuffer;
	static CComBSTR bstrProperty("XXX");
	static CComBSTR bstrMethod("YYY");
	static unsigned char ucThing = 'o';
	static unsigned char ucObjectProperty = 'p';
	static unsigned char ucWorld = 'w';
	DISPPARAMS dispparams;
	long lElapsed = 0L;
	CComVariant varData(0L);

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	hr = m_pWorld->get_Global(&pGlobal);
	if (FAILED(hr))
		return hr;

	hr = pGlobal->AddPropertyExt(bstrProperty, varData, PSBIT_SCRIPTDEFAULTPROPERTY /* | PSBIT_NOTREMOTED */, 
		PS_SCRIPTDEFAULTPROPERTY, varData.vt, IID_NULL, NULL);
	if (FAILED(hr))
		return hr;

	// package up args into DISPPARAMS struct
	hr = InitializeDISPPARAMS(&dispparams, 2);
	if (FAILED(hr))
		return hr;

	dispparams.rgvarg[1].vt = VT_BSTR;
	dispparams.rgvarg[1].bstrVal = bstrProperty;

	dispparams.rgvarg[0] = varData;

	hr = SendCommandHelper(&pUBuffer, ucThing, pGlobal, DISPID_THING_PROPERTY, DISPATCH_PROPERTYPUT, &dispparams);

	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

	if (FAILED(hr))
		return hr;

	// time process thing command (put property)
	hr = TimeProcessMessage(ucThing, pUBuffer, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeProcessMessageAll: thing, DISPID_THING_PROPERTY [propput] VT_I4, %d times, %6.4f msec avg\n", lRepeat, ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	pUBuffer.Release();

	hr = pGlobal->RemoveProperty(bstrProperty);
	if (FAILED(hr))
		return hr;

	hr = m_pWorld->get_Module(CComBSTR("System"), &pModule);
	if (FAILED(hr))
		return hr;

	hr = pGlobal->CreateAndAddMethodExt(METHOD_CLIENT | METHOD_SERVER, pModule, 
		bstrMethod, bstrMethod, PSBIT_SCRIPTDEFAULTMETHOD /* | PSBIT_NOTREMOTED */, PS_ALLEXECUTEMETHOD);
	if (FAILED(hr))
		return hr;

	// package up args into DISPPARAMS struct
	hr = InitializeDISPPARAMS(&dispparams, 3);
	if (FAILED(hr))
		return hr;

	dispparams.rgvarg[2].vt = VT_DISPATCH;
	dispparams.rgvarg[2].pdispVal = pGlobal;

	dispparams.rgvarg[1].vt = VT_BSTR;
	dispparams.rgvarg[1].bstrVal = bstrMethod;

	// NOTE: if NULL dispparams passed in, we need to make sure
	// it's treated as such on other side - not as 0 parameters
	dispparams.rgvarg[0].vt = VT_DISPATCH;
	dispparams.rgvarg[0].pdispVal = NULL;

	hr = SendCommandHelper(&pUBuffer, ucThing, pGlobal, DISPID_THING_INVOKEMETHODINT, DISPATCH_METHOD, &dispparams);

	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

	if (FAILED(hr))
		return hr;

	// time process thing command (invoke method)
	hr = TimeProcessMessage(ucThing, pUBuffer, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	VWREPORT(m_pWorld, VWR_PRINT, "CSystemExemplarObject::TimeProcessMessageAll: thing, DISPID_THING_INVOKEMETHODINT, %d times, %6.4f msec avg\n", lRepeat, ((float)lElapsed/(float)lRepeat));

	*plElapsed += lElapsed;

	pUBuffer.Release();

	hr = pGlobal->RemoveMethod(bstrMethod);
	if (FAILED(hr))
		return hr;

	return S_OK;
}

HRESULT CSystemExemplarObject::SendCommandHelper(IUnMarshallBuffer** ppUBuffer, unsigned char ch, IThing* pthing, DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams)
{
	HRESULT hr = S_OK;
	CComPtr<IMarshallBuffer> pBuffer;
	unsigned char* pb = NULL;
	unsigned long cb = 0;

	*ppUBuffer = NULL;

	hr = ::CreateMarshallBuffer(&pBuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// not needed for ProcessMessageExt
#if 0
	hr = m_pWorld->WriteCommandHeader(pBuffer, 'N', ch);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
#endif

#ifdef SUPPORT_SERVER_CALLSTACK
	hr = m_pWorld->WriteCommandCallstack(pBuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
#endif

	// marshal IThing (by ref)
	hr = m_pWorld->MarshallThing(pthing, MARSHALL_BYID | MARSHALL_BYREF | MARSHALL_TONETWORK, pBuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal DISPID
	hr = pBuffer->put_Long(dispid);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal wFlags
	hr = pBuffer->put_UShort(wFlags);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshal DISPPARAMS
	hr = m_pWorld->MarshallDISPPARAMS(pdispparams, pBuffer, MARSHALL_BYID | MARSHALL_BYREF | MARSHALL_TONETWORK);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = ::CreateUnMarshallBuffer(ppUBuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pBuffer->Detach(&pb, &cb);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppUBuffer)->CopyTo(pb, cb);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	return hr;
}

// no-op YYY method
STDMETHODIMP CSystemExemplarObject::YYY()
{
	return S_OK;
}

// no-op OnXXXAccess method
STDMETHODIMP CSystemExemplarObject::OnXXXAccess(long lPerms, VARIANT_BOOL *bReturnValue)
{
	if (bReturnValue == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CSystemExemplarObject::OnXXXAccess: NULL output pointer\n");
        return ReportCoreError(E_POINTER);
    }

	*bReturnValue = VARIANT_TRUE;

	return S_OK;
}

// no-op OnXXXChanged method
STDMETHODIMP CSystemExemplarObject::OnXXXChanged(long lHint, VARIANT varHintData)
{
	return S_OK;
}

STDMETHODIMP CSystemExemplarObject::TimeAll(long lRepeat, long* plElapsed)
{
	HRESULT hr = S_OK;
	long lElapsed = 0L;
	CComPtr<IPropertyList> pPropList;

	if (plElapsed == NULL)
		return E_POINTER;

	*plElapsed = 0L;

	hr = ::CreatePropertyList(m_pWorld, &pPropList);
	if (FAILED(hr) || pPropList == NULL)
		return hr;

#ifdef TIME_GETPROPERTY
	hr = TimeGetPropertyAll(CComVariant(0L), lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	*plElapsed += lElapsed;

	hr = TimeGetPropertyAll(CComVariant(CComBSTR("0")), lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	*plElapsed += lElapsed;

	hr = TimeGetPropertyAll(CComVariant(pPropList), lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	*plElapsed += lElapsed;
#endif

#ifdef TIME_GETPROPERTYSECURITY
	hr = TimeGetPropertySecurityAll(lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	*plElapsed += lElapsed;
#endif

#ifdef TIME_PUTPROPERTY
	hr = TimePutPropertyAll(CComVariant(0L), lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	*plElapsed += lElapsed;

	hr = TimePutPropertyAll(CComVariant(CComBSTR("0")), lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	*plElapsed += lElapsed;

	hr = TimePutPropertyAll(CComVariant(pPropList), lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	*plElapsed += lElapsed;
#endif

#ifdef TIME_CHECKPROPERTYSECURITY
	hr = TimeCheckPropertySecurityAll(PS_READ, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	*plElapsed += lElapsed;

	hr = TimeCheckPropertySecurityAll(PS_WRITE, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	*plElapsed += lElapsed;
#endif

#ifdef TIME_CHECKPROPERTIESSECURITY
	hr = TimeCheckPropertiesSecurityAll(PS_READ, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	*plElapsed += lElapsed;

	hr = TimeCheckPropertiesSecurityAll(PS_WRITE, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	*plElapsed += lElapsed;
#endif

#ifdef TIME_GETMETHOD
	hr = TimeGetMethodAll(lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	*plElapsed += lElapsed;
#endif

#ifdef TIME_CHECKMETHODSECURITY
	hr = TimeCheckMethodSecurityAll(PS_READ, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	*plElapsed += lElapsed;

	hr = TimeCheckMethodSecurityAll(PS_WRITE, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	*plElapsed += lElapsed;

	hr = TimeCheckMethodSecurityAll(PS_EXECUTE, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	*plElapsed += lElapsed;
#endif

#ifdef TIME_CHECKMETHODSSECURITY
	hr = TimeCheckMethodsSecurityAll(PS_READ, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	*plElapsed += lElapsed;

	hr = TimeCheckMethodsSecurityAll(PS_WRITE, lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	*plElapsed += lElapsed;
#endif

#ifdef TIME_INVOKEMETHOD
	hr = TimeInvokeMethodAll(lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	*plElapsed += lElapsed;
#endif

#ifdef TIME_NOTIFYCLIENT
	hr = TimeNotifyAll(lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	*plElapsed += lElapsed;
#endif

#ifdef TIME_SENDCOMMAND
	hr = TimeSendCommandAll(lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	*plElapsed += lElapsed;
#endif

#ifdef TIME_PROCESSMESSAGE
	hr = TimeProcessMessageAll(lRepeat, &lElapsed);
	if (FAILED(hr))
		return hr;

	*plElapsed += lElapsed;
#endif

	return S_OK;
}


//------------------------------------------------------
// OnCreateRoomClient
// We need this method to delay firing the "OnCreateRoom" event so that we aren't 
// in the middle of a synchronous call when we handle this event (RK)
// (This was a problem when the teleport list handles OnCreateRoom 
// and tries to call GetNamesFromIDs while CreateInstance calls are occurring)

STDMETHODIMP CSystemExemplarObject::OnCreateRoomClient(IThing *pThing)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing>         pGlobal;
	CComPtr<IThing>			pCurrentAvatar;
	CComPtr<IPropertyMap>	pAvatarMap;
	CComPtr<IPropertyList>	pAvatarList;
	CComPtr<IVWAnimator>	pAnimator;
	CComPtr<IVWAnimation>	pAnimation;
	CComVariant				var;
	VARIANT_BOOL			bAllDone;
	DISPPARAMS				dispparams;
	HRESULT                 hr = S_OK;

	if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
    	return hr;

	// Get the avatars in the world
	if (pGlobal)
	{
		hr = pGlobal->get_ObjectProperty(CComBSTR("Avatars"), (IObjectProperty **) &pAvatarMap);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CRoomObject::OnCreateRoomClient  Can't get avatar list\n");
			return hr;
		}
	}

	// Construct a property list from the avatar map
	if (pAvatarMap)
	{
		if (FAILED(hr = ::CreatePropertyList(m_pWorld, &pAvatarList)))
			return hr;

		// Get first property
		hr = pAvatarMap->FirstItem(NULL, &var, &bAllDone);

		while (bAllDone == VARIANT_FALSE && SUCCEEDED(hr)) 
		{
			ASSERT(var.vt == VT_DISPATCH);

			hr = pAvatarList->Add(var);
			if (FAILED(hr))
				break;

			// Clean up alloced memory
			var.Clear();

			// Get next property
			hr = pAvatarMap->NextItem(NULL, &var, &bAllDone);
		}

		if (FAILED(hr = InitializeDISPPARAMS(&dispparams, 4)))
    		return hr;
		
		dispparams.rgvarg[3].vt = VT_DISPATCH;
		dispparams.rgvarg[3].pdispVal = pAvatarList;

		dispparams.rgvarg[2].vt = VT_DISPATCH;
		dispparams.rgvarg[2].pdispVal = pThing;

		dispparams.rgvarg[1].vt = VT_BSTR;
		dispparams.rgvarg[1].bstrVal = CComBSTR(VW_CREATEROOM_EVENT_STR).Copy();

		dispparams.rgvarg[0] = CComVariant(); 

		if (FAILED(hr = m_pWorld->get_ToolExt(CComBSTR("Animator"), (IUnknown**)&pAnimator)) || !pAnimator)
			return hr;
		
		if (pGlobal)
		{
			if (FAILED(hr = pAnimator->CreateExt(0, pGlobal, CComBSTR("FireUIEvent"), &dispparams, &pAnimation)))
				return hr;
		}
	}

	return hr;
}



// ======================================
// ShowWelcomeMessage
// shows a welcome message - called first time
// the user connects to a world.

STDMETHODIMP CSystemExemplarObject::ShowWelcomeMessage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    HRESULT         hr;
	CComPtr<IThing>	pGlobal;
	CComBSTR		bstrWelcomeURL;

	if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
	    goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pGlobal->get_String(CComBSTR("WelcomeURL"), &bstrWelcomeURL.m_str)))
	    goto ERROR_ENCOUNTERED;

	if (FAILED(hr = m_pWorld->FireUIEvent(pGlobal, CComBSTR(VW_SHOWWELCOME_EVENT_STR), CComVariant(NULL))))
		return hr;

ERROR_ENCOUNTERED:
    return hr;
}