// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// WorldObj.cpp : Implementation of CfooApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include <io.h>
#include <inetfile.h>
#include <vwanim.h>
//#include <vwstats.h>
#include "worldobj.h"
#ifdef VWSYSTEM
#include "plistobj.h"
#include "pmapobj.h"
#include "cmenuitm.h"
#include "vwocx.h"
#endif
#include "thingobj.h"
#include "methdobj.h"
#include "comobj.h"
#include "scrptobj.h"
#include "reghelp.h"
#include "syshelp.h"
#include "cathelp.h"
#include "authntic.h"

#define SUPPORT_SECURITY_OVERRIDE

#define SUPPORT_WORLDOWNER_RESTRICTIONS

//#define WORLD_PERF_DEBUG

//#define TRACE_CREATEOBJECT VWT_NONE
#define TRACE_CREATEOBJECT VWT_METHOD

#if 0
// force debugging of IWorld creation/destruction
#undef TRACE_ALLOCATION

#define TRACE_ALLOCATION VWT_NONE
//#define TRACE_ALLOCATION VWT_METHOD
#endif

typedef CComObject<CThingObject> CComThing;
typedef CComObject<CMethodObject> CComMethod;
typedef CComObject<CCOMModuleObject> CComCOMModule;
typedef CComObject<CScriptModuleObject> CComScriptModule;

#ifdef VWSYSTEM
typedef CComObject<CPropertyListObject> CComPropertyList;
typedef CComObject<CPropertyMapObject> CComPropertyMap;
typedef CComObject<CMenuItemObject> CComMenuItem;
typedef CComObject<COCXProxyObject> CComOCXProxy;
#endif

typedef CVWComPtr<IModule, NULL, &IID_IModule> CModulePtr;
typedef CVWComPtr<IMethod, NULL, &IID_IMethod> CMethodPtr;

EXTERN_C const CLSID CLSID_InternetFileManager;
EXTERN_C const CLSID CLSID_AvatarProfile;
EXTERN_C const CLSID CLSID_AvatarProfileData;

// NOTE! idMinDoctored must be even!
#define idMinDoctored	10000
#define idMaxDoctored	20000
#define IsProperty(id)	(!(id & 1))
#define IsMethod(id)	(id & 1)

#define VALIDATE_WORLD		{ if (m_pWorld == NULL) return ReportWorldError(VWOBJECT_E_INVALIDWORLD); }

// IMPORTANT: Here is the current logic for execution/remoting/logging
//
// [Client]:
//
// defaults:
//	 bLogging			VARIANT_TRUE
//   bFreezeEvents		VARIANT_FALSE
//   bLocalMode			VARIANT_FALSE
//
// ConnectLocal:
//	 bConnected			VARIANT_FALSE
//   bShouldExecute		VARIANT_TRUE
//   bShouldExecuteSync	VARIANT_TRUE
//   bShouldRemote		VARIANT_TRUE
//   bShouldRemoteSync	VARIANT_FALSE
//   bShouldNotify		VARIANT_TRUE
//
//   NOTE: having bConnected off prevents SendCommandExt from remoting command, 
//	 but logging still occurs
//
// Connect:
//	 bLogging			VARIANT_FALSE
//	 bConnected			VARIANT_TRUE
//   bShouldExecute		VARIANT_TRUE
//   bShouldExecuteSync	VARIANT_FALSE
//   bShouldRemote		VARIANT_TRUE
//   bShouldRemoteSync	VARIANT_TRUE
//   bShouldNotify		VARIANT_TRUE
//
//   NOTE: turning on bLocalMode overrides bShouldExecute (and causes execution)
//	 in addition to preventing SendCommandExt from remoting command, but logging still occurs
// 
//	 during ProcessMessage:
//		bShouldRemote		VARIANT_FALSE
// 		bShouldNotify		VARIANT_FALSE
//
//      NOTE: want to execute but not remote (and not log) during client-side message processing
//
// [Server]:
//
// defaults:
//	 bLogging			VARIANT_TRUE
//   bFreezeEvents		VARIANT_FALSE
//   bLocalMode			VARIANT_FALSE
//   bConnected			VARIANT_TRUE
//   bShouldExecute		VARIANT_TRUE
//   bShouldExecuteSync	VARIANT_TRUE
//   bShouldRemote		VARIANT_TRUE
//   bShouldRemoteSync	VARIANT_FALSE
//   bShouldNotify		VARIANT_TRUE
//
//   NOTE: turning on bLocalMode prevents SendCommandExt from remoting command
//
//   during ProcessLogRecord:
//		bFreezeEvents		VARIANT_TRUE
//		bShouldRemote		VARIANT_FALSE
//		bShouldNotify		VARIANT_FALSE
//
//      NOTE: want to execute but not remote, in addition to not firing OLE and in-world events
//	    during server-side log record processing

EXTERN_C const CLSID CLSID_PropertyList;
EXTERN_C const CLSID CLSID_PropertyMap;
EXTERN_C const CLSID CLSID_COMModule;
EXTERN_C const CLSID CLSID_ScriptModule;
EXTERN_C const CLSID CLSID_Method;
EXTERN_C const CLSID CLSID_Thing;
EXTERN_C const CLSID CLSID_World;
EXTERN_C const CLSID CLSID_MenuItem;
EXTERN_C const CLSID CLSID_OCXProxy;

#ifdef VWMM
EXTERN_C const CLSID CLSID_Vector;
EXTERN_C const CLSID CLSID_VWGeometry;
EXTERN_C const CLSID CLSID_VWGeometrySprite;
EXTERN_C const CLSID CLSID_VWGeometryArticulated;
//EXTERN_C const CLSID CLSID_Cell;
EXTERN_C const CLSID CLSID_Boundary;
#endif

CVWMap<CString, LPCTSTR, unsigned char, unsigned char&> CWorldObject::m_indexTable;
CVWMap<unsigned char, unsigned char&, CLSID, CLSID&> CWorldObject::m_clsidTable;
unsigned char CWorldObject::m_ucIndex = 1;

CComBSTR CWorldObject::m_bstrAuthentication;
CComBSTR CWorldObject::m_bstrMenu;
CComBSTR CWorldObject::m_bstrRoom;
CComBSTR CWorldObject::m_bstrAvatar;
CComBSTR CWorldObject::m_bstrMoveTo;
CComBSTR CWorldObject::m_bstrAddDefaultKeys;
CComBSTR CWorldObject::m_bstrConnection;
CComBSTR CWorldObject::m_bstrIsConnected;
CComBSTR CWorldObject::m_bstrPassword;
CComBSTR CWorldObject::m_bstrConnect;
CComBSTR CWorldObject::m_bstrDisconnect;
CComBSTR CWorldObject::m_bstrSystemConnect;
CComBSTR CWorldObject::m_bstrSystemDisconnect;
CComBSTR CWorldObject::m_bstrDescriptionDefault;
CComBSTR CWorldObject::m_bstrCurrentID;
CComBSTR CWorldObject::m_bstrCreate;
CComBSTR CWorldObject::m_bstrDestroy;
CComBSTR CWorldObject::m_bstrSystemCreate;
CComBSTR CWorldObject::m_bstrSystemDestroy;
CComBSTR CWorldObject::m_bstrAvatars;
CComBSTR CWorldObject::m_bstrGUIDs;
CComBSTR CWorldObject::m_bstrExemplars;
CComBSTR CWorldObject::m_bstrModules;
CComBSTR CWorldObject::m_bstrActivate;
CComBSTR CWorldObject::m_bstrDeactivate;
CComBSTR CWorldObject::m_bstrSystemActivate;
CComBSTR CWorldObject::m_bstrSystemDeactivate;
CComBSTR CWorldObject::m_bstrGUID;

// IMPORTANT: the semantic for DISPPARAMS is to pack the structure in
// reverse order (actual param 0 goes in dispparam slot n - 1).
// For remoting, the dispparams are sent already packed correctly.

IMPLEMENT_OBJECTPROPERTY(CWorldObject)

/////////////////////////////////////////////////////////////////////////////
//

CWorldObject::CWorldObject() : m_bUserConnected(VARIANT_FALSE),
	m_bServerSide(VARIANT_FALSE), m_bLocalMode(VARIANT_FALSE), m_bFreezeEvents(VARIANT_FALSE),
	m_pVWConnection(NULL), m_pTools(NULL), m_pModules(NULL), m_pModuleNameList(NULL), m_pExemplars(NULL), m_pAvatars(NULL),
	m_pGUIDs(NULL), m_pDb(NULL), m_bConnected(VARIANT_FALSE), m_pGlobal(NULL), m_pUser(NULL),
	m_bTerminated(VARIANT_FALSE), m_bLogging(VARIANT_TRUE), m_bInPropChange(VARIANT_FALSE),
	m_bShouldExecute(VARIANT_TRUE), m_bShouldExecuteSync(VARIANT_TRUE), m_bInProcessMessage(VARIANT_FALSE),
	m_bShouldRemote(VARIANT_TRUE), m_bShouldRemoteSync(VARIANT_FALSE), m_bShouldNotify(VARIANT_TRUE),
	m_pmodVBScript(NULL), m_bAbortEvent(VARIANT_FALSE), m_pMarshBuf(NULL), m_bTrace(VARIANT_TRUE),
	m_idNext(idMinDoctored), m_ucCurrentMessage(' ')

{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CWorldObject::CWorldObject, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif

	CString strRESOURCE;

	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

	if ((BSTR)m_bstrConnection == NULL)
	{
		strRESOURCE.LoadString(IDS_CONNECTION_PROP);
		m_bstrConnection = strRESOURCE;

		strRESOURCE.LoadString(IDS_ISCONNECTED_PROP);
		m_bstrIsConnected = strRESOURCE;

		strRESOURCE.LoadString(IDS_PASSWORD_PROP);
		m_bstrPassword = strRESOURCE;

		strRESOURCE.LoadString(IDS_CONNECT_EVENT);
		m_bstrConnect = strRESOURCE;

		strRESOURCE.LoadString(IDS_DISCONNECT_EVENT);
		m_bstrDisconnect = strRESOURCE;

		strRESOURCE.LoadString(IDS_SYSCONNECT_EVENT);
		m_bstrSystemConnect = strRESOURCE;

		strRESOURCE.LoadString(IDS_SYSDISCONNECT_EVENT);
		m_bstrSystemDisconnect = strRESOURCE;

		strRESOURCE.LoadString(IDS_DESCRIPTION_DEFAULT);
		m_bstrDescriptionDefault = strRESOURCE;

		strRESOURCE.LoadString(IDS_CREATE_EVENT);
		m_bstrCreate = strRESOURCE;

		strRESOURCE.LoadString(IDS_DESTROY_EVENT);
		m_bstrDestroy = strRESOURCE;

		strRESOURCE.LoadString(IDS_SYSTEMCREATE_EVENT);
		m_bstrSystemCreate = strRESOURCE;

		strRESOURCE.LoadString(IDS_SYSTEMDESTROY_EVENT);
		m_bstrSystemDestroy = strRESOURCE;

		strRESOURCE.LoadString(IDS_CURRENTID_PROP);
		m_bstrCurrentID = strRESOURCE;

		strRESOURCE.LoadString(IDS_AVATARS_PROP);
		m_bstrAvatars = strRESOURCE;
	
		strRESOURCE.LoadString(IDS_GUIDS_PROP);
		m_bstrGUIDs = strRESOURCE;
	
		strRESOURCE.LoadString(IDS_EXEMPLARS_PROP);
		m_bstrExemplars = strRESOURCE;

		strRESOURCE.LoadString(IDS_MODULES_PROP);
		m_bstrModules = strRESOURCE;

		strRESOURCE.LoadString(IDS_ROOM_TYPE);
		m_bstrRoom = strRESOURCE;

		strRESOURCE.LoadString(IDS_MENU_TYPE);
		m_bstrMenu = strRESOURCE;

		strRESOURCE.LoadString(IDS_AVATAR_TYPE);
		m_bstrAvatar = strRESOURCE;

		strRESOURCE.LoadString(IDS_MOVETO_METHOD);
		m_bstrMoveTo = strRESOURCE;

		strRESOURCE.LoadString(IDS_ADDDEFAULTKEYS_METHOD);
		m_bstrAddDefaultKeys = strRESOURCE;

		strRESOURCE.LoadString(IDS_ACTIVATE_EVENT);
		m_bstrActivate = strRESOURCE;

		strRESOURCE.LoadString(IDS_DEACTIVATE_EVENT);
		m_bstrDeactivate = strRESOURCE;

		strRESOURCE.LoadString(IDS_SYSTEMACTIVATE_EVENT);
		m_bstrSystemActivate = strRESOURCE;

		strRESOURCE.LoadString(IDS_SYSTEMDEACTIVATE_EVENT);
		m_bstrSystemDeactivate = strRESOURCE;

		strRESOURCE.LoadString(IDS_GUID_PROP);
		m_bstrGUID = strRESOURCE;

		strRESOURCE.LoadString(IDS_AUTHENTICATION_PROP);
		m_bstrAuthentication = strRESOURCE;
		
	}

	m_pThingThis = NULL;
	m_pThingCaller = NULL;
	m_pSecurityContext = NULL;
	m_pThingSuper = NULL;

	if (m_ucIndex == 1)
	{
		// setup CLSID table
		AddCLSIDToTable(CLSID_PropertyList);
		AddCLSIDToTable(CLSID_PropertyMap);
		AddCLSIDToTable(CLSID_COMModule);
		AddCLSIDToTable(CLSID_ScriptModule);
		AddCLSIDToTable(CLSID_Method);
		AddCLSIDToTable(CLSID_Thing);
		AddCLSIDToTable(CLSID_World);
		AddCLSIDToTable(CLSID_MenuItem);
		AddCLSIDToTable(CLSID_OCXProxy);
		AddCLSIDToTable(CLSID_AvatarProfile);
		AddCLSIDToTable(CLSID_AvatarProfileData);

#ifdef VWMM
		AddCLSIDToTable(CLSID_Vector);
		AddCLSIDToTable(CLSID_VWGeometry);
		AddCLSIDToTable(CLSID_VWGeometrySprite);
		AddCLSIDToTable(CLSID_VWGeometryArticulated);
//		AddCLSIDToTable(CLSID_Cell);
		AddCLSIDToTable(CLSID_Boundary);
#endif
	}

	// TODO: add other well-known CLSIDs

	InitializeCriticalSection(&m_critsec);		
}

CWorldObject::~CWorldObject()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CWorldObject::~CWorldObject, this 0x%0x\n", this);
#endif

	// cleanup module and exemplar references
	Terminate();

	SAFERELEASE(m_pMarshBuf);

	DeleteCriticalSection(&m_critsec);
}

STDMETHODIMP CWorldObject::Terminate()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CWorldObject::Terminate: this 0x%0x, refcount %d\n", this, m_dwRef);
#endif

	HRESULT hr = S_OK;

	if (m_bTerminated == VARIANT_FALSE)
	{
		Lock();

		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::Terminate\n");

		ASSERT(!m_pUser);

		// REVIEW: we'll be inside a CSetUID in processmessageext, so these won't necessarily be NULL

		// if the call stack variables aren't null then 
		// someone screwed up during InvokeMethod imp.
//		ASSERT(!m_pThingThis);
		SAFERELEASE(m_pThingThis);

//		ASSERT(!m_pThingCaller);
		SAFERELEASE(m_pThingCaller);

//		ASSERT(!m_pSecurityContext);
		SAFERELEASE(m_pSecurityContext);

//		ASSERT(!m_pThingSuper);
		SAFERELEASE(m_pThingSuper);

		//clean up tools.
		if (m_pTools)
		{
			IInternetFileManager *pInetFile = NULL;
			IVWAnimator* pAnimator = NULL;

			static CComBSTR bstrInetfile("Inetfile");
			static CComBSTR bstrAnimator("Animator");

			hr = get_ToolExt(bstrInetfile, (IUnknown**) &pInetFile);
			if (SUCCEEDED(hr) && pInetFile != NULL)
			{
				hr = pInetFile->Terminate();

				SAFERELEASE(pInetFile);
			}

			hr = get_ToolExt(bstrAnimator, (IUnknown**) &pAnimator);
			if (SUCCEEDED(hr) && pAnimator != NULL)
			{
				hr = pAnimator->Terminate();

				SAFERELEASE(pAnimator);
			}

#ifdef CHANGEDSTATSTOOLTOSUPPORTTERMINATE
			IVWStatCol *pStat;
			static CComBSTR bstrStats("Statistics");
			hr = get_ToolExt(bstrStats, (IUnknown**) &pStat);
			if (SUCCEEDED(hr) && pStat != NULL)
			{
				hr = pStat->Terminate();
				SAFERELEASE(pStat);
			}
#endif

			SAFERELEASE(m_pTools);
		}

		// release cached ptrs before releasing repository
		SAFERELEASE(m_pExemplars);
		SAFERELEASE(m_pAvatars);
		SAFERELEASE(m_pModules);
		SAFERELEASE(m_pGUIDs);
		SAFERELEASE(m_pModuleNameList);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CWorldObject::Terminate: released cached maps, this 0x%0x, refcount %d\n", this, m_dwRef);
#endif

		SAFERELEASE(m_pmodVBScript);

		SAFERELEASE(m_pGlobal);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CWorldObject::Terminate: released global object, this 0x%0x, refcount %d\n", this, m_dwRef);
#endif

		// shut down repository
		if (m_pDb)
		{
			m_pDb->CloseWorld();

			SAFERELEASE(m_pDb);
		}

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CWorldObject::Terminate: released object repository, this 0x%0x, refcount %d\n", this, m_dwRef);
#endif

		if (m_bConnected == VARIANT_TRUE)
		{
			SAFERELEASE(m_pVWConnection);

			m_bConnected = VARIANT_FALSE;
		}

		// Make sure TraceLevel stuff is cleaned up.
		m_componentMap.RemoveAll();
		
		m_bTerminated = VARIANT_TRUE;

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CWorldObject::Terminate: finished, this 0x%0x, refcount %d\n", this, m_dwRef);
#endif

		Unlock();
	}

	CWorldObjectBase::Terminate();

	return S_OK;
}

STDMETHODIMP CWorldObject::AddCLSIDToTable(REFCLSID clsid)
{
	// NOTE: m_ucIndex starts at 1

	CString strCLSID = VWStringFromCLSID(clsid);
	m_indexTable.SetAt((LPCTSTR)strCLSID, m_ucIndex);
	m_clsidTable.SetAt(m_ucIndex, (CLSID)clsid);

	m_ucIndex++;

	return S_OK;
}

STDMETHODIMP CWorldObject::put_World(IWorld* pworld)
{
	HRESULT hr = S_OK;

	CWorldObjectBase::put_World(pworld);

	// create marshall buffer
	hr = CreateMarshallBuffer(&m_pMarshBuf);
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::put_World: failed to allocate CComMarshallBuffer\n");
		goto ERROR_ENCOUNTERED;
	}

	// prime the buffer
	m_pMarshBuf->put_Byte(0x0);
	m_pMarshBuf->SeekToBegin();

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CWorldObject::InitializeConnection(IVWCommConnection* pvwcommconnection)
{
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::InitializeConnection\n");

	ASSERT(pvwcommconnection);
	if (pvwcommconnection == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::InitializeConnection: incoming argument is NULL\n");
		return E_INVALIDARG;
	}

	// NOTE: called from IVWClient::Connect

	SAFERELEASE(m_pVWConnection);
	m_pVWConnection = pvwcommconnection;
	SAFEADDREF(m_pVWConnection);

	// set client to connected state
	m_bConnected = VARIANT_TRUE;

	// when connected, don't execute sync methods on the client
	m_bShouldExecuteSync = VARIANT_FALSE;

	// when connected, allow remoting of sync methods
	m_bShouldRemoteSync = VARIANT_TRUE;

	// don't log on client-side when connected
	m_bLogging = VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CWorldObject::get_ServerSide(VARIANT_BOOL* pboolVal)
{
	VALIDATE_WORLD;

	ASSERT(pboolVal);
	if (pboolVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_ServerSide: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*pboolVal = m_bServerSide;

	return S_OK;
}

STDMETHODIMP CWorldObject::put_ServerSide(VARIANT_BOOL boolVal)
{
	// put_World called after put_ServerSide
//	VALIDATE_WORLD;

	m_bServerSide = boolVal;

	// if client-side
	if (m_bServerSide == VARIANT_FALSE)
	{
		// default to not connected
		m_bConnected = VARIANT_FALSE;
	}
	else
	{
		// default to connected state
		m_bConnected = VARIANT_TRUE;
	}

	return S_OK;
}

STDMETHODIMP CWorldObject::get_LocalMode(VARIANT_BOOL* pboolVal)
{
	VALIDATE_WORLD;

	ASSERT(pboolVal);
	if (pboolVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_LocalMode: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*pboolVal = m_bLocalMode;

	return S_OK;
}

STDMETHODIMP CWorldObject::put_LocalMode(VARIANT_BOOL boolVal)
{
	VALIDATE_WORLD;

	m_bLocalMode = boolVal;

	return S_OK;
}

STDMETHODIMP CWorldObject::get_FreezeEvents(VARIANT_BOOL* pboolVal)
{
	VALIDATE_WORLD;

	ASSERT(pboolVal);
	if (pboolVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_FreezeEvents: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*pboolVal = m_bFreezeEvents;

	return S_OK;
}

STDMETHODIMP CWorldObject::put_FreezeEvents(VARIANT_BOOL boolVal)
{
	VALIDATE_WORLD;

	m_bFreezeEvents = boolVal;

	return S_OK;
}

STDMETHODIMP CWorldObject::get_IsLoggingEnabled(VARIANT_BOOL* pboolVal)
{
	VALIDATE_WORLD;

	ASSERT(pboolVal);
	if (pboolVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_IsLoggingEnabled: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*pboolVal = m_bLogging;

	return S_OK;
}

STDMETHODIMP CWorldObject::EnableLogging(VARIANT_BOOL boolVal)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::EnableLogging: %s\n", (boolVal == VARIANT_TRUE) ? "true" : "false");

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote);

	if (bShouldExecute == VARIANT_TRUE)
	{
		// if on client and connected to server, shouldn't be turning logging on
		if (m_bServerSide == VARIANT_FALSE && m_bConnected == VARIANT_TRUE)
		{
			// should never happen
			ASSERT(FALSE);

			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}

		m_bLogging = boolVal;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		HRESULT senthr = S_OK;
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BOOL;
		dispparams.rgvarg[0].boolVal = boolVal;

		hr = SendWorldCommand(VARIANT_FALSE, DISPID_WORLD_ENABLELOGGING, DISPATCH_METHOD, &dispparams, &senthr);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
		
		if (SUCCEEDED(hr) && hr != VWOBJECT_S_NOTREMOTED) 
		{
			hr = senthr;
		}
	}

ERROR_ENCOUNTERED:
	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::IsExemplar(IThing* pthing, VARIANT_BOOL* pboolVal)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::IsExemplar\n");

	ASSERT(pthing);
	if (pthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::IsExemplar: incoming argument (IThing*) is NULL\n");
		return E_INVALIDARG;
	}

	return pthing->get_IsExemplar(pboolVal);
}

STDMETHODIMP CWorldObject::IsAvatar(IThing* pthing, VARIANT_BOOL* pboolVal)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	static CComBSTR	kbstrIsAvatar("IsAvatar");

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::IsAvatar\n");

	ASSERT(pthing);
	if (pthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::IsAvatar: incoming argument (IThing*) is NULL\n");
		return E_INVALIDARG;
	}

	ASSERT(pboolVal);
	if (pboolVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::IsAvatar: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return E_POINTER;
	}

//	*pboolVal = VARIANT_FALSE;

	hr = pthing->IsOfType(m_bstrAvatar, pboolVal);
	
	return hr;
}

STDMETHODIMP CWorldObject::IsRoom(IThing* pthing, VARIANT_BOOL* pboolVal)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::IsRoom\n");

	ASSERT(pthing);
	if (pthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::IsRoom: incoming argument (IThing*) is NULL\n");
		return E_INVALIDARG;
	}

	ASSERT(pboolVal);
	if (pboolVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::IsRoom: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return E_POINTER;
	}

	*pboolVal = VARIANT_FALSE;

	hr = pthing->IsOfType(m_bstrRoom, pboolVal);

	return hr;
}

STDMETHODIMP CWorldObject::get_User(IThing** ppthing)
{
	// OK to not have world
//	VALIDATE_WORLD;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::get_User\n");

	ASSERT(ppthing);
	if (ppthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_User: outgoing pointer (IThing**) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	// user is now being set in ProcessMessage (on server-side)

//	if (m_bServerSide == VARIANT_FALSE)
		*ppthing = m_pUser;
//	else
//		*ppthing = m_clist.FindConnection(m_pVWConnection);

	SAFEADDREF(*ppthing);

	return S_OK;
}

STDMETHODIMP CWorldObject::put_User(IThing* pthing)
{
	VALIDATE_WORLD;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::put_User\n");

	SAFERELEASE(m_pUser);
	m_pUser = pthing;
	SAFEADDREF(m_pUser);

	return S_OK;
}

STDMETHODIMP CWorldObject::get_This(IThing** ppthing)
{
	VALIDATE_WORLD;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::get_This\n");

	ASSERT(ppthing);
	if (ppthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_This: outgoing pointer (IThing**) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*ppthing = m_pThingThis;
	SAFEADDREF(*ppthing);

	return S_OK;
}

STDMETHODIMP CWorldObject::put_This(IThing* pthing)
{
	VALIDATE_WORLD;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::put_This\n");

	SAFERELEASE(m_pThingThis);
	m_pThingThis = pthing;
	SAFEADDREF(m_pThingThis);

	return S_OK;
}

STDMETHODIMP CWorldObject::get_Caller(IThing** ppthing)
{
	VALIDATE_WORLD;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::get_Caller\n");

	ASSERT(ppthing);
	if (ppthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_Caller: outgoing pointer (IThing**) is NULL\n");
		return ReportWorldError(E_POINTER);
	}
	
	*ppthing = m_pThingCaller;
	SAFEADDREF(*ppthing);

	return S_OK;
}

STDMETHODIMP CWorldObject::put_Caller(IThing* pthing)
{
	VALIDATE_WORLD;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::put_Caller\n");

	SAFERELEASE(m_pThingCaller);
	m_pThingCaller = pthing;
	SAFEADDREF(m_pThingCaller);

	return S_OK;
}

STDMETHODIMP CWorldObject::get_Super(IThing** ppthing)
{
	VALIDATE_WORLD;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::get_Super\n");

	ASSERT(ppthing);
	if (ppthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_Super: outgoing pointer (IThing**) is NULL\n");
		return ReportWorldError(E_POINTER);
	}
	
	*ppthing = m_pThingSuper;
	SAFEADDREF(*ppthing);

	return S_OK;
}

STDMETHODIMP CWorldObject::put_Super(IThing* pthing)
{
	VALIDATE_WORLD;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::put_Super\n");

	SAFERELEASE(m_pThingSuper);
	m_pThingSuper = pthing;
	SAFEADDREF(m_pThingSuper);

	return S_OK;
}

STDMETHODIMP CWorldObject::get_SecurityContext(IPropertySecurity** ppSecurity)
{
	VALIDATE_WORLD;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::get_SecurityContext\n");

	ASSERT(ppSecurity);
	if (ppSecurity == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_SecurityContext: outgoing pointer (IPropertySecurity**) is NULL\n");
		return ReportWorldError(E_POINTER);
	}
	
	*ppSecurity = m_pSecurityContext;
	SAFEADDREF(*ppSecurity);

	return S_OK;
}

STDMETHODIMP CWorldObject::put_SecurityContext(IPropertySecurity *pSecurity)
{
	VALIDATE_WORLD;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::put_SecurityContext\n");

	SAFERELEASE(m_pSecurityContext);
	m_pSecurityContext = pSecurity;
	SAFEADDREF(m_pSecurityContext);

	return S_OK;
}

STDMETHODIMP CWorldObject::get_Exemplar(BSTR bstrType, IThing** ppthing)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	IPropertyMap* pExemplars = NULL;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::get_Exemplar\n");

	ASSERT(bstrType);
	if (bstrType == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_Exemplar: incoming argument (BSTR) is NULL\n");
		return ReportWorldError(E_INVALIDARG);
	}

	ASSERT(ppthing);
	if (ppthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_Exemplar: outgoing pointer (IThing**) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*ppthing = NULL;

	hr = get_Exemplars(&pExemplars);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pExemplars->get_Thing(bstrType, ppthing);
	if (FAILED(hr))
	{
		if (hr == VWOBJECT_E_PROPERTYNOTEXIST)
			hr = VWOBJECT_E_EXEMPLARNOTEXIST;
		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pExemplars);

	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::IsValidExemplar(BSTR bstrType, VARIANT_BOOL* pboolVal)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	IPropertyMap* pExemplars = NULL;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::IsValidExemplar\n");

	ASSERT(bstrType);
	if (bstrType == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::IsValidExemplar: incoming argument (BSTR) is NULL\n");
		return ReportWorldError(E_INVALIDARG);
	}

	ASSERT(pboolVal);
	if (pboolVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::IsValidExemplar: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*pboolVal = NULL;

	hr = get_Exemplars(&pExemplars);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pExemplars->IsValid(bstrType, pboolVal);

ERROR_ENCOUNTERED:
	SAFERELEASE(pExemplars);

	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::get_Avatar(BSTR bstrName, IThing** ppthing)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	IPropertyMap* pAvatars = NULL;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::get_Avatar\n");

	ASSERT(bstrName);
	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_Avatar: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	ASSERT(ppthing);
	if (ppthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_Avatar: outgoing pointer (IThing**) is NULL\n");
		return E_POINTER;
	}

	*ppthing = NULL;

	hr = get_Avatars(&pAvatars);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pAvatars->get_Thing(bstrName, ppthing);
	if (FAILED(hr))
	{
		if (hr == VWOBJECT_E_PROPERTYNOTEXIST)
			hr = VWOBJECT_E_AVATARNOTEXIST;
		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pAvatars);

	return hr;
}

STDMETHODIMP CWorldObject::get_AvatarByGUID(BSTR bstrGUID, IThing** ppthing)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	IPropertyMap* pGUIDs = NULL;
	BSTR bstrName = NULL;
	IThing *pTempThing = NULL;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::get_AvatarByGUID\n");

	ASSERT(bstrGUID);
	if (bstrGUID == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_AvatarByGUID: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	ASSERT(ppthing);
	if (ppthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_AvatarByGUID: outgoing pointer (IThing**) is NULL\n");
		return E_POINTER;
	}

	*ppthing = NULL;

	hr = get_GUIDs(&pGUIDs);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pGUIDs->get_Thing(bstrGUID, &pTempThing);
	if (FAILED(hr))
	{
		if (hr == VWOBJECT_E_PROPERTYNOTEXIST)
			hr = VWOBJECT_E_AVATARNOTEXIST;
		goto ERROR_ENCOUNTERED;
	}

	// get the object's name
	hr = pTempThing->get_Name(&bstrName);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// does an avatar with this name exist in this world
	//cocooned object not yet converted to real avatar. can't do this 
	//hr = get_Avatar(bstrName, ppthing);
	//if (FAILED(hr))
	//	goto ERROR_ENCOUNTERED;	

	*ppthing = pTempThing;
	SAFEADDREF(*ppthing);

ERROR_ENCOUNTERED:
	SAFERELEASE(pTempThing);
	SAFERELEASE(pGUIDs);
	SAFEFREESTRING(bstrName);

	return hr;
}

STDMETHODIMP CWorldObject::get_ObjectByName(BSTR bstrName, IThing** ppthing)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::get_ObjectByName\n");

	ASSERT(bstrName);
	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_ObjectByName: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	ASSERT(ppthing);
	if (ppthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_ObjectByName: outgoing pointer (IThing**) is NULL\n");
		return E_POINTER;
	}

	*ppthing = NULL;

	hr = m_pDb->GetObjectByName(bstrName, ppthing);

	return hr;
}

STDMETHODIMP CWorldObject::get_ObjectByID(long ID, IThing** ppthing)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::get_ObjectByID\n");

	ASSERT(ppthing);
	if (ppthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_ObjectByID: outgoing pointer (IThing**) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	if (m_pDb)
		hr = m_pDb->GetObject(ID, ppthing);
	else
		hr = VWODB_E_OBJECTNOTEXIST;

	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::get_Module(BSTR bstrName, IModule** ppmodule)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	IPropertyMap* pModules = NULL;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::get_Module\n");

	ASSERT(bstrName);
	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_Module: incoming argument (BSTR) is NULL\n");
		return ReportWorldError(E_INVALIDARG);
	}

	ASSERT(ppmodule);
	if (ppmodule == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_Module: outgoing pointer (IModule**) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*ppmodule = NULL;

	// see if this is a special module
	CString strName(bstrName);
	if (strName.CompareNoCase(":VBScript") == 0)
	{
		// see if the special VBScript module has been created
		if (m_pmodVBScript == NULL)
		{
			// nope, create it
			hr = CreateLocalScriptModuleEx(bstrName, &m_pmodVBScript);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}

		// return it
		*ppmodule = m_pmodVBScript;
		SAFEADDREF(*ppmodule);
	}
	else
	{
		hr = get_Modules(&pModules);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (pModules == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_Module: invalid module map\n");
			hr = VWOBJECT_E_INVALIDMODULEMAP;
		}
		else
		{
			hr = pModules->get_ObjectProperty(bstrName, (IObjectProperty**)ppmodule);
			if (FAILED(hr))
			{
				if (hr == VWOBJECT_E_PROPERTYNOTEXIST)
					hr = VWOBJECT_E_MODULENOTEXIST;
				goto ERROR_ENCOUNTERED;
			}
		}
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pModules);

	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::IsValidModule(BSTR bstrName, VARIANT_BOOL* pboolVal)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	IPropertyMap* pModules = NULL;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::IsValidModule\n");

	ASSERT(bstrName);
	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::IsValidModule: incoming argument (BSTR) is NULL\n");
		return ReportWorldError(E_INVALIDARG);
	}

	ASSERT(pboolVal);
	if (pboolVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::IsValidModule: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*pboolVal = NULL;

	hr = get_Modules(&pModules);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pModules->IsValid(bstrName, pboolVal);

ERROR_ENCOUNTERED:
	SAFERELEASE(pModules);

	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::CreateObjectInt(long lObjectID, IThing** ppthing)
{
	HRESULT hr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateObjectInt: object ID %d\n", lObjectID);

	ASSERT(ppthing);
	if (ppthing == NULL)
	{
		hr = E_POINTER;
		goto ERROR_ENCOUNTERED;
	}

	*ppthing = new CComThing;

	if (*ppthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateObjectInt: failed to allocate CComThing\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(*ppthing);

	(*ppthing)->AddRef();

	hr = (*ppthing)->put_World((IWorld*)GetUnknown());
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppthing)->put_ID(lObjectID);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// let repository know about it
	hr = m_pDb->CreateObject(*ppthing);
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateObjectInt: IVWObjectRepository::CreateObject failed, object ID (%d)\n", lObjectID);
		goto ERROR_ENCOUNTERED;
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_CREATEOBJECT, "CWorldObject::CreateObjectInt: created object with ID (%d)\n", 
		lObjectID);
#endif

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppthing);
	return hr;
}

STDMETHODIMP CWorldObject::DeleteObjectInt(IThing* pthing)
{
	HRESULT hr = S_OK;
	long lObjectID = NULL_OBJECT;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::DeleteObjectInt\n");

	ASSERT(pthing);
	if (pthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::DeleteObjectInt: incoming argument (IThing*) is NULL\n");
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	// get object index
	hr = pthing->get_ID(&lObjectID);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_CREATEOBJECT, "CWorldObject::DeleteObjectInt: object ID (%d)\n", lObjectID);
#endif

	// remove from repository
	hr = m_pDb->DeleteObject(lObjectID);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CWorldObject::FireActivateObject(IThing* pthing)
{
	HRESULT hr = S_OK;
	DISPPARAMS dispparams;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::FireActivateObject\n");

	// package up args into DISPPARAMS struct
	hr = InitializeDISPPARAMS(&dispparams, 1);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	dispparams.rgvarg[0].vt = VT_DISPATCH;
	dispparams.rgvarg[0].pdispVal = pthing;

	// fire activate event
	/* hr = */ pthing->FireEventExt(m_bstrSystemActivate, &dispparams, FIREEVENT_DEFAULT);

	/* hr = */ pthing->FireEventExt(m_bstrActivate, &dispparams, FIREEVENT_DEFAULT);

	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CWorldObject::FireDeactivateObject(IThing* pthing)
{
	HRESULT hr = S_OK;
	DISPPARAMS dispparams;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::FireDeactivateObject\n");

	// package up args into DISPPARAMS struct
	hr = InitializeDISPPARAMS(&dispparams, 1);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	dispparams.rgvarg[0].vt = VT_DISPATCH;
	dispparams.rgvarg[0].pdispVal = pthing;

	// fire deactivate event
	/* hr = */ pthing->FireEventExt(m_bstrSystemDeactivate, &dispparams, FIREEVENT_DEFAULT);

	/* hr = */ pthing->FireEventExt(m_bstrDeactivate, &dispparams, FIREEVENT_DEFAULT);

	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CWorldObject::FireCreateObject(IThing* pthing)
{
	HRESULT hr = S_OK;
	DISPPARAMS dispparams;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::FireCreateObject\n");

	// package up args into DISPPARAMS struct
	hr = InitializeDISPPARAMS(&dispparams, 1);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	dispparams.rgvarg[0].vt = VT_DISPATCH;
	dispparams.rgvarg[0].pdispVal = pthing;

	/* hr = */ pthing->FireEventExt(m_bstrSystemCreate, &dispparams, FIREEVENT_SELF);

	/* hr = */ pthing->FireEventExt(m_bstrCreate, &dispparams, FIREEVENT_SELF);

	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CWorldObject::FireDestroyObject(IThing* pthing)
{
	HRESULT hr = S_OK;
	DISPPARAMS dispparams;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::FireDestroyObject\n");

	// package up args into DISPPARAMS struct
	hr = InitializeDISPPARAMS(&dispparams, 1);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	dispparams.rgvarg[0].vt = VT_DISPATCH;
	dispparams.rgvarg[0].pdispVal = pthing;

	/* hr = */ pthing->FireEventExt(m_bstrSystemDestroy, &dispparams, FIREEVENT_SELF);

	/* hr = */ pthing->FireEventExt(m_bstrDestroy, &dispparams, FIREEVENT_SELF);

	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CWorldObject::FireConnectAvatar(IThing* pthing)
{
	HRESULT hr = S_OK;
	CComPtr<IVWAnimator> pAnimator;
	CComPtr<IVWAnimation> pAnimation;
	DISPPARAMS dispparams;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::FireConnectAvatar\n");

	// package up args into DISPPARAMS struct
	hr = InitializeDISPPARAMS(&dispparams, 1);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	dispparams.rgvarg[0].vt = VT_DISPATCH;
	dispparams.rgvarg[0].pdispVal = pthing;

	if (m_bServerSide == VARIANT_TRUE)
	{
		// If on the server, delay firing event so client has user by the time it gets
		// the OnConnect.
		if (FAILED(hr = get_ToolExt(CComBSTR("Animator"), (IUnknown**)&pAnimator)) || !pAnimator)
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = pAnimator->CreateExt(0, pthing, CComBSTR("FireConnectAvatar"), &dispparams, &pAnimation)))
			goto ERROR_ENCOUNTERED;
	}
	else
	{
		// fire system connect event to bystanders
		/* hr = */ pthing->FireEventExt(m_bstrSystemConnect, &dispparams, FIREEVENT_DEFAULT);

		// fire connect event to bystanders
		/* hr = */ pthing->FireEventExt(m_bstrConnect, &dispparams, FIREEVENT_DEFAULT);
	}
	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CWorldObject::FireDisconnectAvatar(IThing* pthing)
{
	HRESULT hr = S_OK;
	DISPPARAMS dispparams;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::FireDisconnectAvatar\n");

	// package up args into DISPPARAMS struct
	hr = InitializeDISPPARAMS(&dispparams, 1);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	dispparams.rgvarg[0].vt = VT_DISPATCH;
	dispparams.rgvarg[0].pdispVal = pthing;

	// fire disconnect event to bystanders
	/* hr = */ pthing->FireEventExt(m_bstrDisconnect, &dispparams, FIREEVENT_DEFAULT);

	// fire system disconnect event to bystanders
	/* hr = */ pthing->FireEventExt(m_bstrSystemDisconnect, &dispparams, FIREEVENT_DEFAULT);

	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CWorldObject::InvokeMoveInto(IThing* pWho, IThing* pWhere)
{
	HRESULT hr = S_OK;
	DISPPARAMS dispparams;

	// package up args into DISPPARAMS struct
	hr = InitializeDISPPARAMS(&dispparams, 1);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	dispparams.rgvarg[0].vt = VT_DISPATCH;
	dispparams.rgvarg[0].pdispVal = pWhere;

	hr = pWho->InvokeMethodExt(m_bstrMoveTo, &dispparams, NULL);

	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CWorldObject::InvokeAddDefaultKeys(IThing* pWho, IAvatarProfile* pProfile)
{
	HRESULT hr = S_OK;
	DISPPARAMS dispparams;

	// package up args into DISPPARAMS struct
	hr = InitializeDISPPARAMS(&dispparams, 0);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	//dispparams.rgvarg[0].vt = VT_DISPATCH;
	//dispparams.rgvarg[0].pdispVal = pProfile;

	hr = pWho->InvokeMethodExt(m_bstrAddDefaultKeys, &dispparams, NULL);

	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CWorldObject::ActivateRoom(IThing* pthing)
{
	HRESULT hr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::ActivateRoom\n");

	hr = FireActivateObject(pthing);

//ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CWorldObject::DeactivateRoom(IThing* pthing)
{
	HRESULT hr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::DeactivateRoom\n");

	hr = FireDeactivateObject(pthing);

//ERROR_ENCOUNTERED:
	return hr;
}

static ULONG g_TemporaryIDCount = 100000;
// these functions are for client side operation

HRESULT CWorldObject::CreateInstanceIncomplete(BSTR bstrName, BSTR bstrGUID, IThing** ppthing)
{
	HRESULT hr = S_OK;
	long currentID = NULL_OBJECT;
	IPropertyMap *pGUIDs = NULL;
	IThing *pGlobal = NULL;
	static CComBSTR bstrIsComplete("IsComplete");

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateInstanceIncomplete: name (%s), GUID %s\n", 
		CString(bstrName), CString(bstrGUID));
#endif

	m_bLocalMode = VARIANT_TRUE;

	hr = get_Global(&pGlobal);
	if (SUCCEEDED(hr))
	{
		hr = pGlobal->get_Long(m_bstrCurrentID, &currentID);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}
	else
	{
		currentID = g_TemporaryIDCount;
		g_TemporaryIDCount++;
	}

	hr = CreateObjectInt(currentID, ppthing);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = LogCreateObject(currentID);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppthing)->put_IsStubInt(VARIANT_FALSE);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// increment current obj ID
	currentID++;

	// update persistent value
	if (pGlobal)
	{
		CSetUID SetUID(this, pGlobal, pGlobal, VARIANT_TRUE);

		hr = pGlobal->put_Long(m_bstrCurrentID, currentID);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	if (bstrGUID)
	{
		hr = get_GUIDs(&pGUIDs);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (pGUIDs)
		{
			CSetUID SetUID(this, pGlobal, pGlobal, VARIANT_TRUE);

			hr = pGUIDs->put_Thing(bstrGUID, *ppthing);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}

		hr = (*ppthing)->AddPropertyExt(m_bstrGUID, CComVariant(bstrGUID),
							PSBIT_SYSTEMPROPERTY | PSBIT_HIDDEN | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_BSTR, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	hr = ValidateInstanceNameExt(bstrName);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppthing)->AddPropertyExt(CThingObject::m_bstrName, CComVariant(bstrName), 
		PSBIT_SYSTEMPROPERTY | PSBIT_INTERNALLYSET, PS_ALLACCESSPROPERTY, VT_BSTR, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppthing)->AddPropertyExt(bstrIsComplete, CComVariant((bool)VARIANT_FALSE), 
		PSBIT_SYSTEMPROPERTY | PSBIT_INTERNALLYSET | PSBIT_HIDDEN, PS_GLOBALSYSTEMCONSTANT, VT_BOOL, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_CREATEOBJECT, "CWorldObject::CreateInstanceIncomplete: created incomplete object with ID (%d), name (%s), GUID %s\n", 
		currentID, CString(bstrName), CString(bstrGUID));
#endif

ERROR_ENCOUNTERED:
	m_bLocalMode = VARIANT_FALSE;

	SAFERELEASE(pGUIDs);
	SAFERELEASE(pGlobal);
	
	return hr;
}	

HRESULT CWorldObject::CreateInstanceIntHelper(BSTR bstrName, IThing* pthingParent, IThing** ppthing)
{
	HRESULT hr = S_OK;
	long currentID = NULL_OBJECT;
	long lObjects = 0;
	IThing* pThis = NULL;
	IThing* pOwner = NULL;
	DISPPARAMS dispparams;
	CComVariant varRet;
	VARIANT_BOOL bAvatar;
	static CComBSTR bstrOnSystemPreCreate("OnSystemPreCreate");
	static CComBSTR bstrOnSystemPostCreate("OnSystemPostCreate");
	static CComBSTR bstrAvatar("Avatar");

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateInstanceIntHelper: name (%s)\n", 
		CString(bstrName));
#endif

	// initialize (to avoid errors at terminate time)
	InitializeDISPPARAMS(&dispparams, 0);

	m_bLocalMode = VARIANT_TRUE;

	hr = ValidateInstanceNameExt(bstrName);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// assure that the would-be owner is capable of creating objects

	// determine the would-be owner
	hr = get_This(&pThis);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (!pThis)
	{
		hr = get_User(&pOwner);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}
	else
	{
		// if This is an avatar, use it as the owner
		hr = pThis->IsOfType(bstrAvatar, &bAvatar);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (bAvatar == VARIANT_TRUE)
		{
			// set owner (transfer ref)
			pOwner = pThis;
			pThis = NULL;
		}
		else
		{
			// if not, then get it's owner (which _should_ be an avatar)
			hr = pThis->get_Owner(&pOwner);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
			// OK not to have an owner
			// If there is an owner, check that it's an avatar
			if (pOwner)
			{
				hr = pOwner->IsOfType(bstrAvatar, &bAvatar);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;
				ASSERT(bAvatar == VARIANT_TRUE);
			}
#endif
		}
	}

	// ask the would-be owner if it's ok to create
	if (pOwner)
	{
		CSetUID SetUID(this, pOwner, pOwner /*, VARIANT_TRUE */);

		// package up args into DISPPARAMS struct (pass in exemplar)
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = pthingParent;

		hr = pOwner->InvokeMethodExt(bstrOnSystemPreCreate, &dispparams, &varRet);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		ASSERT(varRet.vt==VT_BOOL);
		if (varRet.boolVal != VARIANT_TRUE)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateInstanceIntHelper: Owner does not allow create\n");
#endif
			hr = VWOBJECT_E_EXCEEDEDOBJECTQUOTA;
			goto ERROR_ENCOUNTERED;
		}
	}

	hr = m_pGlobal->get_Long(m_bstrCurrentID, &currentID);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = CreateObjectInt(currentID, ppthing);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = LogCreateObject(currentID);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppthing)->put_IsStubInt(VARIANT_FALSE);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// increment current obj ID
	currentID++;

	{
		CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);

		// update persistent value
		hr = m_pGlobal->put_Long(m_bstrCurrentID, currentID);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	hr = IncrementObjectCount(&lObjects);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = CreateInstanceInPlace(bstrName, pthingParent, *ppthing);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (pOwner)
	{
		CSetUID SetUID(this, pOwner, pOwner /*, VARIANT_TRUE */);

		// tell owner we've created an object

		// free up old stuff in dispparams
		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		// package up args into DISPPARAMS struct (pass in object)
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = *ppthing;

		hr = pOwner->InvokeMethodExt(bstrOnSystemPostCreate, &dispparams, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

#ifdef WORLD_PERF_DEBUG
	VWREPORT(m_pWorld, VWR_PERFLOG, "Create Object,%d,%d\n",
		GetTickCount(), lObjects);
#endif

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_CREATEOBJECT, "CWorldObject::CreateInstanceIntHelper: created new object with ID (%d), name (%s)\n", 
		currentID, CString(bstrName));
#endif

ERROR_ENCOUNTERED:

	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	SAFERELEASE(pOwner);
	SAFERELEASE(pThis);

	m_bLocalMode = VARIANT_FALSE;

	return hr;
}

HRESULT CWorldObject::CreateInstanceInPlace(BSTR bstrName, IThing* pthingParent, IThing* pthing)
{
	HRESULT hr = S_OK;
	static CComBSTR bstrIsComplete("IsComplete");
	static CComBSTR bstrAvatar("Avatar");
	
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateInstanceInPlace: name (%s)\n", CString(bstrName));
#endif
 
	if (pthingParent != NULL)
	{
		VARIANT_BOOL fFertile = VARIANT_FALSE;

		// set security context to pthingParent to avoid security violation
		{
			CSetUID SetUID(this, pthingParent, pthingParent, VARIANT_TRUE);
		
			/* hr = */ pthingParent->get_BOOL(CThingObject::m_bstrIsFertile, &fFertile);
		}

		if (fFertile == VARIANT_FALSE)
		{
#ifdef _DEBUG
			CComBSTR bstrType;
			/* hr = */ pthingParent->get_Type(&bstrType.m_str);

			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateInstanceInPlace: exemplar not fertile (%s)\n", CString(bstrType));
#endif
			hr = VWOBJECT_E_EXEMPLARNOTFERTILE;
			goto ERROR_ENCOUNTERED;
		}
	}

	hr = ValidateInstanceNameExt(bstrName);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// don't remote prop changes, just log
	m_bLocalMode = VARIANT_TRUE;

	// set owner first, so security works
	{
		IThing* pThis = NULL;
		IThing* pOwner = NULL;
		VARIANT_BOOL bAvatar;

		hr = get_This(&pThis);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (!pThis)
		{
			hr = get_User(&pOwner);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
		else
		{
			// is this an avatar?
			hr = pThis->IsOfType(bstrAvatar, &bAvatar);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED_1;

			if (bAvatar == VARIANT_TRUE)
			{
				// yep, use it as owner (transfer ref)
				pOwner = pThis;
				pThis = NULL;
			}
			else
			{
				// get this' owner (should be an avatar)
				hr = pThis->get_Owner(&pOwner);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED_1;

#ifdef _DEBUG
				// OK not to have an owner
				// If there is an owner, check that it's an avatar
				if (pOwner)
				{
					hr = pOwner->IsOfType(bstrAvatar, &bAvatar);
					if (FAILED(hr))
						goto ERROR_ENCOUNTERED_1;
					ASSERT(bAvatar == VARIANT_TRUE);
				}
#endif
			}
		}

		// set owner prop in thing 
		if (pthingParent)
		{
			// set parent ptr in thing
			CSetUID SetUID(this, pthing, pthing, VARIANT_TRUE);

			hr = pthing->put_ExemplarInt(pthingParent);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED_1;
		}

		hr = pthing->put_OwnerInt(pOwner);

ERROR_ENCOUNTERED_1:
		SAFERELEASE(pOwner);
		SAFERELEASE(pThis);
		
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	// if root exemplar, use AddProperty
	if (pthingParent == NULL)
	{
		CSetUID SetUID(this, pthing, pthing, VARIANT_TRUE);

		hr = pthing->AddPropertyExt(CThingObject::m_bstrContainer, CComVariant((IDispatch *) NULL),
									/* PSBIT_HIDDEN | */ PSBIT_SYSTEMPROPERTY | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_DISPATCH, IID_IThing, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pthing->AddPropertyExt(CThingObject::m_bstrName, CComVariant(bstrName), 
									PSBIT_SYSTEMPROPERTY | PSBIT_INTERNALLYSET, PS_ALLACCESSPROPERTY, VT_BSTR, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// changed to have NULL description by default
		hr = pthing->AddPropertyExt(CThingObject::m_bstrDescription, CComVariant(CComBSTR("")), // m_bstrDescriptionDefault), 
									PSBIT_SYSTEMPROPERTY, PS_ALLACCESSPROPERTY, VT_BSTR, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// default: is fertile
		hr = pthing->AddPropertyExt(CThingObject::m_bstrIsFertile, CComVariant((bool)VARIANT_TRUE),
									PSBIT_SYSTEMPROPERTY | PSBIT_HIDDEN, PS_EXEMPLARDEFAULTPROPERTY, VT_BOOL, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// default: is exemplar
		hr = pthing->AddPropertyExt(CThingObject::m_bstrIsExemplar, CComVariant((bool)VARIANT_TRUE),
									PSBIT_SYSTEMPROPERTY | PSBIT_INTERNALLYSET | PSBIT_HIDDEN, PS_GLOBALSYSTEMCONSTANT, VT_BOOL, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// default: is cloneable
		hr = pthing->AddPropertyExt(CThingObject::m_bstrIsCloneable, CComVariant((bool)VARIANT_TRUE),
									PSBIT_SYSTEMPROPERTY | PSBIT_HIDDEN, PS_ALLACCESSPROPERTY, VT_BOOL, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pthing->AddPropertyExt(bstrIsComplete, CComVariant((bool)VARIANT_FALSE),
									PSBIT_SYSTEMPROPERTY | PSBIT_INTERNALLYSET | PSBIT_HIDDEN, PS_GLOBALSYSTEMCONSTANT, VT_BOOL, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pthing->AddPropertyExt(CThingObject::m_bstrIsOpen, CComVariant((bool)VARIANT_FALSE),
									PSBIT_SYSTEMPROPERTY | PSBIT_HIDDEN, PS_EXEMPLARDEFAULTPROPERTY, VT_BOOL, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pthing->AddPropertyExt(CThingObject::m_bstrIsNoticeable, CComVariant((bool)VARIANT_FALSE),
									PSBIT_SYSTEMPROPERTY | PSBIT_HIDDEN, PS_ALLACCESSPROPERTY, VT_BOOL, IID_NULL, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_CREATEOBJECT, "CWorldObject::CreateInstanceInPlace: created new complete object, name (%s)\n", 
			CString(bstrName));
#endif
	}
	// else if instance or exemplar, use put_Property
	else
	{
		// Note: this may yet fail if the properties are Exemplar write
		//  then we need to set the exemplar to the ROOT exemplar that defines these
		CSetUID SetUID(this, pthing, pthing, VARIANT_TRUE);

		// only override exemplar name if passed-in name not NULL
		if (bstrName != NULL)
		{
			// does this get the friendly bit automatically?
			hr = pthing->put_String(CThingObject::m_bstrName, bstrName);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}

		// default: is not fertile
//		hr = pthing->put_BOOL(CThingObject::m_bstrIsFertile, VARIANT_FALSE);
//		if (FAILED(hr))
//			goto ERROR_ENCOUNTERED;

		// default: is not exemplar
//		hr = pthing->put_BOOL(CThingObject::m_bstrIsExemplar, VARIANT_FALSE);
//		if (FAILED(hr))
//			goto ERROR_ENCOUNTERED;

		// default: is cloneable
//		hr = pthing->put_BOOL(CThingObject::m_bstrIsCloneable, VARIANT_TRUE);
//		if (FAILED(hr))
//			goto ERROR_ENCOUNTERED;

		hr = pthing->put_BOOL(bstrIsComplete, VARIANT_TRUE);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_CREATEOBJECT, "CWorldObject::CreateInstanceInPlace: filled in incomplete object, name (%s)\n", 
			CString(bstrName));
#endif
	}

	// set contents property, each instance has its own Contents prop
	{
		CSetUID SetUID(this, pthing, pthing, VARIANT_TRUE);

		IPropertyList* pContents = NULL;

		hr = CreatePropertyList(m_pWorld, &pContents);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// if instance, use put_Property
		// else if exemplar, use AddProperty

		if (pthingParent)
			hr = pthing->put_ObjectProperty(CThingObject::m_bstrContents, pContents);
		else
			hr = pthing->AddPropertyExt(CThingObject::m_bstrContents, CComVariant(pContents),
									PSBIT_HIDDEN | PSBIT_SYSTEMPROPERTY | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_DISPATCH, IID_IPropertyList, NULL);

		// thing propmap has ref cnt 

		SAFERELEASE(pContents);

		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	m_bLocalMode = VARIANT_FALSE;

	return hr;
}

STDMETHODIMP CWorldObject::CreateInstance(BSTR bstrName, VARIANT varExemplar, VARIANT varContainer, IThing** ppthing)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	IThing* pthingWhere = NULL;
	CComVariant varTemp;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateInstance: name %s\n", CString(bstrName));
#endif

	ASSERT(bstrName);
	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateInstance: incoming argument (BSTR) is NULL\n");
		return ReportWorldError(E_INVALIDARG);
	}

	ASSERT(ppthing);
	if (ppthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateInstance: outgoing pointer (IThing**) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*ppthing = NULL;

	hr = VariantCopyInd(&varTemp, &varContainer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (varTemp.vt == VT_DISPATCH)
	{
		CThingPtr pthing(varTemp.pdispVal);

		if (pthing != NULL)
		{
			pthingWhere = pthing;
			SAFEADDREF(pthingWhere);
		}
	}

	hr = CreateInstanceExt(bstrName, varExemplar, pthingWhere, ppthing);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	SAFERELEASE(pthingWhere);

	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::CreateInstanceExt(BSTR bstrName, VARIANT varExemplar, IThing* pthingWhere, IThing** ppthing)
{
	HRESULT hr = S_OK;
	IThing* pthingParent = NULL;
	IThing* pUser = NULL;
	static CComBSTR bstrAvatar("Avatar");
	VARIANT_BOOL bAvatar;
	CComVariant varTemp;
	VARIANT_BOOL fIsExemplar = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateInstanceExt: name %s\n", CString(bstrName));
#endif

	ASSERT(bstrName);
	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateInstanceExt: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	// NOTE: pthingWhere can be NULL

	ASSERT(ppthing);
	if (ppthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateInstanceExt: outgoing pointer (IThing**) is NULL\n");
		return E_POINTER;
	}

	*ppthing = NULL;

	hr = VariantCopyInd(&varTemp, &varExemplar);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (varTemp.vt == VT_DISPATCH)
	{
		CThingPtr pthing(varTemp.pdispVal);

		if (pthing != NULL)
		{
			pthingParent = pthing;
			SAFEADDREF(pthingParent);
		}
	}
	else if (varTemp.vt == VT_BSTR)
	{
		hr = get_Exemplar(varTemp.bstrVal, &pthingParent);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}
	else
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateInstanceExt: invalid exemplar argument (must be either BSTR or IDispatch*)\n");

		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	if (pthingParent == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateInstanceExt: incoming argument (IThing*) is NULL\n");
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	// check that parent is exemplar
	hr = pthingParent->get_BOOL(CThingObject::m_bstrIsExemplar, &fIsExemplar);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (fIsExemplar == VARIANT_FALSE)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateInstanceExt: can't create instance from an instance\n");
#endif
		hr = VWOBJECT_E_OBJECTNOTEXEMPLAR;
		goto ERROR_ENCOUNTERED;
	}

	// check for avatars and boot out
	hr = pthingParent->IsOfType(bstrAvatar, &bAvatar);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	else if (bAvatar == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateInstanceExt: illegal attempt to create an avatar by type\n");
		hr = VWOBJECT_E_CANTCREATEAVATAR;
		goto ERROR_ENCOUNTERED;
	}

	if (pthingWhere == NULL)
	{
		hr = get_User(&pUser);

		// weak reference
		pthingWhere = pUser;
	}

	hr = CreateInstanceInt(bstrName, pthingParent, pthingWhere, ppthing);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	SAFERELEASE(pthingParent);	
	SAFERELEASE(pUser);

	return hr;
}

STDMETHODIMP CWorldObject::CreateInstanceInt(BSTR bstrName, IThing* pthingParent, IThing* pthingWhere, IThing** ppthing)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateInstanceInt: name %s\n", CString(bstrName));
#endif

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote);

	if (bShouldExecute == VARIANT_TRUE)
	{
		VARIANT_BOOL bAvatar = VARIANT_FALSE;
		VARIANT_BOOL bRoom = VARIANT_FALSE;

		hr = IsAvatar(pthingParent, &bAvatar);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (bAvatar == VARIANT_TRUE)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateInstanceInt: invalid exemplar (can't create avatars with CreateInstance)\n");

			hr = VWOBJECT_E_INVALIDEXEMPLAR;
			goto ERROR_ENCOUNTERED;
		}

		if (pthingWhere != NULL)
		{
			VARIANT_BOOL bExemplar = VARIANT_FALSE;

			// can't put new instance in an exemplar
			hr = IsExemplar(pthingWhere, &bExemplar);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			if (bExemplar == VARIANT_TRUE)
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateInstanceInt: invalid container\n");

				hr = VWOBJECT_E_INVALIDCONTAINER;
				goto ERROR_ENCOUNTERED;
			}
		}

		hr = CreateInstanceIntHelper(bstrName, pthingParent, ppthing);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		m_bLocalMode = VARIANT_TRUE;

		// mark as not an exemplar
		{
			CSetUID SetUID(this, *ppthing, *ppthing, VARIANT_TRUE);

			hr = (*ppthing)->put_BOOL(CThingObject::m_bstrIsExemplar, VARIANT_FALSE);
		}

		m_bLocalMode = VARIANT_FALSE;

		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = FireCreateObject(*ppthing);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (m_bServerSide == VARIANT_TRUE)
		{
			// update client with new object
			hr = SendThing(soClient, *ppthing);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}

		if (pthingWhere != NULL)
		{
			hr = IsAvatar(*ppthing, &bAvatar);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			// if an avatar or room, don't move anywhere... leave with NULL container
			if (bAvatar == VARIANT_FALSE)
			{
				hr = IsRoom(*ppthing, &bRoom);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;

				if (bRoom == VARIANT_FALSE)
				{
					hr = InvokeMoveInto(*ppthing, pthingWhere);
					if (FAILED(hr))
						goto ERROR_ENCOUNTERED;
				}
			}
		}
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		HRESULT senthr = S_OK;
		CComVariant var;
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 3);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[2].vt = VT_BSTR;
		dispparams.rgvarg[2].bstrVal = bstrName;

		dispparams.rgvarg[1].vt = VT_DISPATCH;
		dispparams.rgvarg[1].pdispVal = pthingParent;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = pthingWhere;

		hr = SendWorldCommand(VARIANT_TRUE, DISPID_WORLD_CREATEINSTANCEINT, DISPATCH_METHOD, &dispparams, &senthr, &var);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		// if sendmessage and invoke both succeeded, pull out retval
		if (SUCCEEDED(hr) && hr != VWOBJECT_S_NOTREMOTED)
		{
			hr = senthr;

			if (SUCCEEDED(senthr))
			{
				ASSERT(var.vt == VT_DISPATCH);

				*ppthing = (IThing*)var.pdispVal;

				SAFEADDREF(*ppthing);
			}
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CWorldObject::CreateExemplar(BSTR bstrType, VARIANT varExemplar, IThing** ppthing)
{
	return CreateExemplarEx(NULL, bstrType, varExemplar, ppthing);
}

STDMETHODIMP CWorldObject::CreateExemplarEx(BSTR bstrName, BSTR bstrType, VARIANT varExemplar, IThing** ppthing)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateExemplarEx: name %s, type %s\n", CString(bstrName), CString(bstrType));
#endif

	ASSERT(bstrType);
	if (bstrType == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateExemplarEx: incoming argument (BSTR) is NULL\n");
		return ReportWorldError(E_INVALIDARG);
	}

	ASSERT(ppthing);
	if (ppthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateExemplarEx: outgoing pointer (IThing**) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*ppthing = NULL;

	hr = CreateExemplarExt(bstrName, bstrType, varExemplar, ppthing);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::CreateExemplarExt(BSTR bstrName, BSTR bstrType, VARIANT varExemplar, IThing** ppthing)
{
	HRESULT hr = S_OK;
	IThing* pthingParent = NULL;
	CComVariant varTemp;

	// NOTE: munge exemplar of type Foo to have name Foo_Exemplar
	CComBSTR bstrExemplarName = bstrType;
	bstrExemplarName += "_Exemplar";

	// NOTE: bstrName and bstrParentType can be NULL

	ASSERT(bstrType);
	if (bstrType == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateExemplarExt: incoming argument (BSTR) is NULL\n");
		return ReportWorldError(E_INVALIDARG);
	}

	ASSERT(ppthing);
	if (ppthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateExemplarExt: outgoing pointer (IThing**) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*ppthing = NULL;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateExemplarExt: name %s, type %s\n", CString(bstrName), CString(bstrType));
#endif

	hr = VariantCopyInd(&varTemp, &varExemplar);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (varTemp.vt == VT_DISPATCH)
	{
		CThingPtr pthing(varTemp.pdispVal);

		if (pthing != NULL)
		{
			pthingParent = pthing;
			SAFEADDREF(pthingParent);
		}
	}
	else if (varTemp.vt == VT_BSTR)
	{
		hr = get_Exemplar(varTemp.bstrVal, &pthingParent);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}
	else
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateExemplarExt: invalid exemplar argument (must be either BSTR or IDispatch*)\n");

		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	// if bstrName is NULL, auto-generate the name
	hr = CreateExemplarInt(bstrName ? bstrName : bstrExemplarName, bstrType, pthingParent, ppthing);

ERROR_ENCOUNTERED:
	SAFERELEASE(pthingParent);

	return hr;
}

STDMETHODIMP CWorldObject::CreateExemplarInt(BSTR bstrName, BSTR bstrType, IThing* pthingParent, IThing** ppthing)
{
	HRESULT hr = S_OK;
	IPropertyMap* pExemplars = NULL;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	IPropertyMap* pGUIDs = NULL;
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateExemplarInt: type %s\n", CString(bstrType));
#endif

	hr = ValidateExemplarTypeExt(bstrType);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote);

	if (bShouldExecute == VARIANT_TRUE)
	{
		VARIANT_BOOL fValid = VARIANT_FALSE;
		VARIANT_BOOL fIsExemplar = VARIANT_FALSE;
		static CComBSTR bstrGUID("GUID");
		CLSID clsid;

		hr = get_Exemplars(&pExemplars);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pExemplars->IsValid(bstrType, &fValid);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (fValid)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateExemplarInt: exemplar already exists (%s)\n", CString(bstrType));
#endif
			hr = VWOBJECT_E_EXEMPLAREXISTS;
			goto ERROR_ENCOUNTERED;
		}

		if (pthingParent != NULL)
		{
			hr = pthingParent->get_BOOL(CThingObject::m_bstrIsExemplar, &fIsExemplar);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			if (fIsExemplar == VARIANT_FALSE)
			{
#ifdef _DEBUG
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateExemplarInt: can't create exemplar from an instance\n");
#endif
				hr = VWOBJECT_E_OBJECTNOTEXEMPLAR;
				goto ERROR_ENCOUNTERED;
			}
		}

		hr = CreateInstanceIntHelper(bstrName, pthingParent, ppthing);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// not remoted
		hr = (*ppthing)->put_TypeInt(bstrType);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// create GUID for exemplar
		hr = CoCreateGuid(&clsid);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = get_GUIDs(&pGUIDs);
		if (SUCCEEDED(hr))
		{
			CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);

			hr = pGUIDs->put_Thing(VWBSTRFromCLSID(clsid), *ppthing);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}

		hr = FireCreateObject(*ppthing);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (m_bServerSide == VARIANT_TRUE)
		{
			// update all clients with new exemplar
			hr = SendThing(soAvatars, *ppthing);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}

		{
			CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);

			// stick pthing into exemplar map
			hr = pExemplars->put_Thing(bstrType, *ppthing);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		HRESULT senthr = S_OK;
		CComVariant var;
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 3);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[2].vt = VT_BSTR;
		dispparams.rgvarg[2].bstrVal = bstrName;

		dispparams.rgvarg[1].vt = VT_BSTR;
		dispparams.rgvarg[1].bstrVal = bstrType;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = pthingParent;

		hr = SendWorldCommand(VARIANT_TRUE, DISPID_WORLD_CREATEEXEMPLARINT, DISPATCH_METHOD, &dispparams, &senthr, &var);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		// if sendmessage and invoke both succeeded, pull out retval
		if (SUCCEEDED(hr) && hr != VWOBJECT_S_NOTREMOTED)
		{
			hr = senthr;

			if (SUCCEEDED(senthr))
			{
				ASSERT(var.vt == VT_DISPATCH);

				*ppthing = (IThing*)var.pdispVal;

				SAFEADDREF(*ppthing);
			}
		}
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pExemplars);
	SAFERELEASE(pGUIDs);

	return hr;
}

STDMETHODIMP CWorldObject::CloneInstance(BSTR bstrName, IThing* pthingSource, IThing** ppthing)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	IThing* pthingExemplar = NULL;
	IThing* pthingContainer = NULL;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CloneInstance: name %s\n", CString(bstrName));
#endif

	ASSERT(bstrName);
	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CloneInstance: incoming argument (BSTR) is NULL\n");
		return ReportWorldError(E_INVALIDARG);
	}

	ASSERT(pthingSource);
	if (pthingSource == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CloneInstance: incoming argument (IThing*) is NULL\n");
		return ReportWorldError(E_INVALIDARG);
	}

	ASSERT(ppthing);
	if (ppthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CloneInstance: outgoing pointer (IThing**) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*ppthing = NULL;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote);

	if (bShouldExecute == VARIANT_TRUE)
	{
		VARIANT_BOOL fCloneable = VARIANT_FALSE;
		VARIANT_BOOL fIsExemplar = VARIANT_FALSE;
		VARIANT_BOOL bAvatar = VARIANT_FALSE;

		/* hr = */ pthingSource->get_BOOL(CThingObject::m_bstrIsExemplar, &fIsExemplar);

		if (fIsExemplar == VARIANT_TRUE)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CloneInstance: exemplars are not cloneable\n");
#endif

			hr = VWOBJECT_E_EXEMPLARNOTCLONEABLE;
			goto ERROR_ENCOUNTERED;
		}

		/* hr = */ pthingSource->get_BOOL(CThingObject::m_bstrIsCloneable, &fCloneable);

		if (fCloneable == VARIANT_FALSE)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CloneInstance: instance not cloneable\n");
#endif

			hr = VWOBJECT_E_INSTANCENOTCLONEABLE;
			goto ERROR_ENCOUNTERED;
		}
		hr = pthingSource->get_Container(&pthingContainer);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pthingSource->get_Exemplar(&pthingExemplar);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = CreateInstanceInt(bstrName, pthingExemplar, pthingContainer, ppthing);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// Copy methods and properties (except PSBIT_INTERNALLYSET)
		{
			CSetUID SetUID(this, *ppthing, *ppthing, VARIANT_TRUE);

			IPropertyMap* pInstanceProperties = NULL;
			IPropertyMap* pInstanceMethods = NULL;
			IPropertySecurity* pSecurity = NULL;
			VARIANT_BOOL bDone = VARIANT_FALSE;
			VARIANT_BOOL bValid = VARIANT_FALSE;
			CMethodPtr methodPtr;
			CComVariant var;
			CComBSTR bstrName;

			hr = pthingSource->get_InstanceProperties(&pInstanceProperties);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			if (pInstanceProperties)
			{
				hr = pInstanceProperties->FirstItem(&bstrName.m_str, &var, &bDone);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED_1;

				while (bDone != VARIANT_TRUE)
				{
					hr = (*ppthing)->IsValidProperty(bstrName, &bValid);
					if (FAILED(hr))
						goto ERROR_ENCOUNTERED_1;

					hr = pthingSource->get_PropertySecurity(bstrName, &pSecurity);
					if (FAILED(hr))
						goto ERROR_ENCOUNTERED_1;

					CPropertySecurity* pPropSec = (CPropertySecurity*)pSecurity;

					// if property doesn't exist in clone, we must add it; otherwise use put
					if (bValid == VARIANT_FALSE)
					{
//						VWTRACE(m_pWorld, "VWOBJECT", VWT_NONE, "CWorldObject::CloneInstance: adding property (%s)\n", CString(bstrName));

						hr = (*ppthing)->AddPropertyExt(bstrName, var, pPropSec->m_ulBits, pPropSec->m_ulSecurityFlags,
							pPropSec->m_vtType, pPropSec->m_IID, pPropSec->m_bstrType);
					}
					else
					{
						if (!(pPropSec->m_ulBits & PSBIT_INTERNALLYSET))
						{
//							VWTRACE(m_pWorld, "VWOBJECT", VWT_NONE, "CWorldObject::CloneInstance: putting property (%s)\n", CString(bstrName));
	
							hr = (*ppthing)->put_Property(bstrName, var);
						}
					}


					SAFERELEASE(pSecurity);

					if (FAILED(hr))
						goto ERROR_ENCOUNTERED_1;

					// free stuff
					bstrName.Empty();
					var.Clear();

					// on to the next one
					hr = pInstanceProperties->NextItem(&bstrName.m_str, &var, &bDone);
					if (FAILED(hr))
						break;
				}

ERROR_ENCOUNTERED_1:
				SAFERELEASE(pInstanceProperties);
			}

			hr = pthingSource->get_InstanceMethods(&pInstanceMethods);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			if (pInstanceMethods)
			{
				hr = pInstanceMethods->FirstItem(&bstrName.m_str, &var, &bDone);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED_2;

				while (bDone != VARIANT_TRUE)
				{
					methodPtr = var;

					if (methodPtr == NULL)
						goto ERROR_ENCOUNTERED_2;

					hr = (*ppthing)->IsValidMethod(bstrName, &bValid);
					if (FAILED(hr))
						goto ERROR_ENCOUNTERED_1;

					hr = pthingSource->get_MethodSecurity(bstrName, &pSecurity);
					if (FAILED(hr))
						goto ERROR_ENCOUNTERED_2;

					CPropertySecurity* pPropSec = (CPropertySecurity*)pSecurity;

					// if method doesn't exist in clone, we must add it; otherwise use put
					if (bValid == VARIANT_FALSE)
					{
//						VWTRACE(m_pWorld, "VWOBJECT", VWT_NONE, "CWorldObject::CloneInstance: adding method (%s)\n", CString(bstrName));

						hr = (*ppthing)->AddMethodExt(bstrName, methodPtr, pPropSec->m_ulBits, pPropSec->m_ulSecurityFlags);
					}
					else
					{
						if (!(pPropSec->m_ulBits & PSBIT_INTERNALLYSET))
						{
//							VWTRACE(m_pWorld, "VWOBJECT", VWT_NONE, "CWorldObject::CloneInstance: putting method (%s)\n", CString(bstrName));

							hr = (*ppthing)->put_Method(bstrName, methodPtr);
						}
					}

					SAFERELEASE(pSecurity);

					if (FAILED(hr))
						goto ERROR_ENCOUNTERED_2;

					// free stuff
					bstrName.Empty();
					var.Clear();

					// on to the next one
					hr = pInstanceMethods->NextItem(&bstrName.m_str, &var, &bDone);
					if (FAILED(hr))
						break;
				}

ERROR_ENCOUNTERED_2:
				SAFERELEASE(pInstanceMethods);
			}
		}
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		HRESULT senthr = S_OK;
		CComVariant var;
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 2);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[1].vt = VT_BSTR;
		dispparams.rgvarg[1].bstrVal = bstrName;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = pthingSource;

		hr = SendWorldCommand(VARIANT_TRUE, DISPID_WORLD_CLONEINSTANCE, DISPATCH_METHOD, &dispparams, &senthr, &var);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		// if sendmessage and invoke both succeeded, pull out retval
		if (SUCCEEDED(hr) && hr != VWOBJECT_S_NOTREMOTED)
		{
			hr = senthr;

			if (SUCCEEDED(senthr))
			{
				ASSERT(var.vt == VT_DISPATCH);

				*ppthing = (IThing*)var.pdispVal;

				SAFEADDREF(*ppthing);
			}
		}
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pthingExemplar);
	SAFERELEASE(pthingContainer);
	
	return ReportWorldError(hr);
}

HRESULT CWorldObject::ConvertInstanceToExemplar(BSTR bstrType, IThing* pthing)
{
	HRESULT hr = S_OK;
	IPropertyMap* pExemplars = NULL;
	VARIANT_BOOL fValid = VARIANT_FALSE;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::ConvertInstanceToExemplar\n");

	// TODO: client/server

	// check if new type in map, if not add to map 
	// and set type into instance

	ASSERT(bstrType);
	if (bstrType == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ConvertInstanceToExemplar: incoming argument (BSTR) is NULL\n");
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(pthing);
	if (pthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ConvertInstanceToExemplar: incoming argument (IThing*) is NULL\n");
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	hr = get_Exemplars(&pExemplars);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pExemplars->IsValid(bstrType, &fValid);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (fValid)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ConvertInstanceToExemplar: exemplar already exists (%s)\n", CString(bstrType));
#endif
		hr = VWOBJECT_E_EXEMPLAREXISTS;
		goto ERROR_ENCOUNTERED;
	}

	hr = pthing->put_TypeInt(bstrType);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// make as not fertile
//	hr = pthing->put_BOOL(CThingObject::m_bstrIsFertile, VARIANT_TRUE);
//	if (FAILED(hr))
//		goto ERROR_ENCOUNTERED;

	// mark as an exemplar
//	hr = pthing->put_BOOL(CThingObject::m_bstrIsExemplar, VARIANT_TRUE);
	hr = pthing->RemoveProperty(CThingObject::m_bstrIsExemplar);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	{
		CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);
	
		// stick pthing into exemplar map
		hr = pExemplars->put_Thing(bstrType, pthing);
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pExemplars);

	return hr; // ReportWorldError(hr);
}

HRESULT CWorldObject::ConvertExemplarToInstance(IThing* pthing)
{
	HRESULT hr = S_OK;
	IPropertyMap* pExemplars = NULL;
	BSTR bstr = NULL;
	VARIANT_BOOL fValid = VARIANT_FALSE;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::ConvertExemplarToInstance\n");

	// TODO: client/server

	// check if exemplar is in map, 
	// if so null out type and remove from map

	ASSERT(pthing);
	if (pthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ConvertExemplarToInstance: incoming argument (IThing*) is NULL\n");
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	hr = get_Exemplars(&pExemplars);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pthing->get_InstanceType(&bstr);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pExemplars->IsValid(bstr, &fValid);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (fValid == VARIANT_FALSE)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ConvertExemplarToInstance: exemplar doesn't exist (%s)\n", CString(bstr));
#endif
		hr = VWOBJECT_E_EXEMPLARNOTEXIST;
		goto ERROR_ENCOUNTERED;
	}

	hr = pthing->put_TypeInt(CThingObject::m_bstrThing);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// mark as not fertile
//	hr = pthing->put_BOOL(CThingObject::m_bstrIsFertile, VARIANT_FALSE);
//	if (FAILED(hr))
//		goto ERROR_ENCOUNTERED;

	// mark as not an exemplar
	hr = pthing->put_BOOL(CThingObject::m_bstrIsExemplar, VARIANT_FALSE);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	{
		CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);

		hr = pExemplars->Remove(bstr);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	SAFEFREESTRING(bstr);
	SAFERELEASE(pExemplars);

	return hr; // ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::CreateAvatar(BSTR bstrName, BSTR bstrPassword, VARIANT varGUID, BSTR* pbstrNewName)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;	
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	CComBSTR bstrGUID;
	CLSID clsid;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateAvatar\n");

	ASSERT(bstrName);
	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateAvatar: incoming argument (BSTR) is NULL\n");
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(bstrPassword);
	if (bstrPassword == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateAvatar: incoming argument (BSTR) is NULL\n");
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(pbstrNewName);
	if (pbstrNewName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateAvatar: outgoing pointer (BSTR*) is NULL\n");
		hr = E_POINTER;
		goto ERROR_ENCOUNTERED;
	}

	if (varGUID.vt == VT_BSTR)
	{
		bstrGUID = varGUID.bstrVal;
	}
	else
	{
		// create GUID for Avatar
		hr = CoCreateGuid(&clsid);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		bstrGUID = VWBSTRFromCLSID(clsid);
	}

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote);

	if (bShouldExecute == VARIANT_TRUE)
	{
		IThing* pthing = NULL;

		hr = CreateAvatarExt(bstrName, bstrPassword, bstrGUID, &pthing, pbstrNewName);

		SAFERELEASE(pthing);

		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		HRESULT senthr = S_OK;
		CComVariant var;
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 3);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[2].vt = VT_BSTR;
		dispparams.rgvarg[2].bstrVal = bstrName;

		dispparams.rgvarg[1].vt = VT_BSTR;
		dispparams.rgvarg[1].bstrVal = bstrPassword;

		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstrGUID;

		hr = SendWorldCommand(VARIANT_TRUE, DISPID_WORLD_CREATEAVATAR, DISPATCH_METHOD, &dispparams, &senthr, &var);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		// if sendmessage and invoke both succeeded, pull out retval
		if (SUCCEEDED(hr) && hr != VWOBJECT_S_NOTREMOTED)
		{
			if (var.vt == VT_BSTR)
				*pbstrNewName = CComBSTR(var.bstrVal).Copy();
			else
			{
				ASSERT(FALSE);
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateAvatar: invalid return value from server\n");
			}
		}
	}

ERROR_ENCOUNTERED:
	return ReportWorldError(hr);
}

HRESULT CWorldObject::CreateAvatarExt(BSTR bstrName, BSTR bstrPassword, BSTR bstrGUID, 
	IThing** ppthing, BSTR *pbstrNewName)
{
	HRESULT hr = S_OK;	
	VARIANT_BOOL fValid = VARIANT_FALSE;
	IPropertyMap* pAvatars = NULL;
	IPropertyMap* pGUIDs = NULL;
	IThing* pDefaultAvatarEx = NULL;
	CComBSTR bstrNewAlias;
	VARIANT_BOOL bIsAuthenticated=VARIANT_FALSE;
	IAuthentication* pAuth = NULL;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateAvatarExt\n");

	// Look up the desired avatar exemplar in global properties	
	if (m_pGlobal)
		hr = m_pGlobal->get_ObjectProperty(CComBSTR("DefaultAvatarExemplar"), (IObjectProperty **)&pDefaultAvatarEx);

	if ((!m_pGlobal) || FAILED(hr))
	{
		hr = get_Exemplar(m_bstrAvatar, &pDefaultAvatarEx);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	hr = DecorateAlias(bstrName, &bstrNewAlias.m_str);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// ppthing is AddRef-ed
	hr = CreateInstanceIntHelper(bstrNewAlias, pDefaultAvatarEx, ppthing);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	m_bLocalMode = VARIANT_TRUE;

	// mark as not an exemplar
	hr = (*ppthing)->put_BOOL(CThingObject::m_bstrIsExemplar, VARIANT_FALSE);

	m_bLocalMode = VARIANT_FALSE;

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// REVIEW: need to make password property read-only by owner

	// don't remote type change, just log
	m_bLocalMode = VARIANT_TRUE;

	// assign password property to avatar - owner & exemplar can RW, no one else can read
	hr = (*ppthing)->AddPropertyExt(m_bstrPassword, CComVariant(bstrPassword),
		PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_INTERNALLYSET, (PS_OWNER_RW | PS_EXEMPLAR_RW), VT_BSTR, IID_NULL, NULL);
	m_bLocalMode = VARIANT_FALSE;

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// don't remote type change, just log
	hr = (*ppthing)->IsValidProperty(m_bstrGUID, &fValid);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// REVIEW: can avatar already have GUID?
	m_bLocalMode = VARIANT_TRUE;

	if (fValid == VARIANT_FALSE)
		hr = (*ppthing)->AddPropertyExt(m_bstrGUID, CComVariant(bstrGUID),
			PSBIT_SYSTEMPROPERTY | PSBIT_HIDDEN | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_BSTR, IID_NULL, NULL);
	else
		hr = (*ppthing)->put_String(m_bstrGUID, bstrGUID);

	m_bLocalMode = VARIANT_FALSE;

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// now if this is an authenticated connection lets shove in the domain/user info
	// REVIEW: do we really need to fully bail out if we fail in this block.. probably not
	if (m_pVWConnection)
	{
		hr=m_pVWConnection->get_IsAuthenticated(&bIsAuthenticated);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		if (bIsAuthenticated==VARIANT_TRUE)
		{
			// pauth Addref'd in the get.. remember to SAFERELEASE it later
			hr=m_pVWConnection->get_Authentication(&pAuth);
			if (FAILED(hr) || !pAuth)
				goto ERROR_ENCOUNTERED;

			// retrieve the User and Domain info from the Authentication object
			CComBSTR bstrAuthInfo;
			hr=pAuth->get_AuthenticationInfo(&bstrAuthInfo.m_str);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
			{
				CSetUID SetUID(this, *ppthing, *ppthing, VARIANT_TRUE);
				// shove bstrAuthInfo into the Authentication property
				hr = (*ppthing)->put_String(m_bstrAuthentication, bstrAuthInfo.Copy());
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;
			}

		}
	}

	hr = get_Avatars(&pAvatars);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	{
		CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);

		// add avatar to avatar map
		hr = pAvatars->put_Thing(bstrNewAlias, *ppthing);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	hr = get_GUIDs(&pGUIDs);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (pGUIDs)
	{
		CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);

		// add avatar to GUID map
		hr = pGUIDs->put_Thing(bstrGUID, *ppthing);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	// Fire a world event.
	hr = FireCreateObject(*ppthing);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (m_bServerSide == VARIANT_TRUE)
	{
		// update client with new avatar
		hr = SendThing(soClient, *ppthing);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	{
		long lAvatars = 0;

		hr = IncrementAvatarCount(&lAvatars);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

#ifdef WORLD_PERF_DEBUG
		VWREPORT(m_pWorld, VWR_PERFLOG, "Create Avatar,%d,%s,%d\n",
			GetTickCount(), CString(bstrNewAlias), lAvatars);
#endif
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_CREATEOBJECT, "CWorldObject::CreateAvatarExt: created avatar with name (%s), preferred name (%s), GUID %s\n", 
		CString(bstrNewAlias), CString(bstrName), CString(bstrGUID));
#endif
	
	*pbstrNewName = bstrNewAlias.Copy();

ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{
		// remove from avatar list
		if (pAvatars)
		{
			CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);

			pAvatars->Remove(bstrNewAlias);

			pGUIDs->Remove(bstrGUID);
		}

		// free instance
		SAFERELEASE(*ppthing);
	}

	SAFERELEASE(pAuth);
	SAFERELEASE(pDefaultAvatarEx);
	SAFERELEASE(pAvatars);
	SAFERELEASE(pGUIDs);

	return hr;
}

HRESULT CWorldObject::CreateAvatarExtInPlace(BSTR bstrName, BSTR bstrPassword, BSTR bstrGUID, 
	IThing* pthing, BSTR *pbstrNewName)
{
	HRESULT hr = S_OK;	
	VARIANT_BOOL fValid = VARIANT_FALSE;
	IPropertyMap* pAvatars = NULL;
	//IPropertyMap* pGUIDs = NULL;
	IThing* pDefaultAvatarEx = NULL;
	BSTR bstrAuthInfo = NULL;
	CComBSTR bstrNewAlias;
	static CComBSTR strGUID("GUID");
	static CComBSTR	kbstrDefAvatar("DefaultAvatarExemplar");
	VARIANT_BOOL bIsAuthenticated=VARIANT_FALSE;
	IAuthentication* pAuth = NULL;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateAvatarExtInPlace\n");

	// Look up the desired avatar exemplar in global properties	
	hr = m_pGlobal->get_Thing(kbstrDefAvatar, &pDefaultAvatarEx);
	if (FAILED(hr))
	{
		hr = get_Exemplar(m_bstrAvatar, &pDefaultAvatarEx);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	hr = DecorateAlias(bstrName, &bstrNewAlias.m_str);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// ppthing is AddRef-ed
	hr = CreateInstanceInPlace(bstrNewAlias, pDefaultAvatarEx, pthing);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// NOTE: bits on the password/GUID props should match those in CreateAvatarExt

	// don't remote type change, just log
	m_bLocalMode = VARIANT_TRUE;

	// Assign password property to avatar, owner/exemplar can RW, no one else can read
	hr = pthing->AddPropertyExt(m_bstrPassword, CComVariant(bstrPassword),
		PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_INTERNALLYSET, (PS_OWNER_RW | PS_EXEMPLAR_RW), VT_BSTR, IID_NULL, NULL);

	m_bLocalMode = VARIANT_FALSE;

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	m_bLocalMode = VARIANT_TRUE;

	// mark as not an exemplar
	hr = pthing->put_BOOL(CThingObject::m_bstrIsExemplar, VARIANT_FALSE);

	m_bLocalMode = VARIANT_FALSE;

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// don't remote type change, just log
	hr = pthing->IsValidProperty(strGUID, &fValid);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// REVIEW: can avatar already have GUID?
	m_bLocalMode = VARIANT_TRUE;

	if (fValid == VARIANT_FALSE)
		hr = pthing->AddPropertyExt(strGUID, CComVariant(bstrGUID),
			PSBIT_SYSTEMPROPERTY | PSBIT_HIDDEN | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_BSTR, IID_NULL, NULL);
	else
		hr = pthing->put_String(strGUID, bstrGUID);

	m_bLocalMode = VARIANT_FALSE;

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// now if this is an authenticated connection lets shove in the domain/user info
	// REVIEW: do we really need to fully bail out if we fail in this block.. probably not
	if (m_pVWConnection)
	{
		hr=m_pVWConnection->get_IsAuthenticated(&bIsAuthenticated);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		if (bIsAuthenticated==VARIANT_TRUE)
		{
			// pauth Addref'd in the get.. remember to SAFERELEASE it later
			hr=m_pVWConnection->get_Authentication(&pAuth);
			if (FAILED(hr) || !pAuth)
				goto ERROR_ENCOUNTERED;

			// retrieve the User and Domain info from the Authentication object
			hr=pAuth->get_AuthenticationInfo(&bstrAuthInfo);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
			{
				CSetUID SetUID(this, pthing, pthing, VARIANT_TRUE);
				// shove bstrAuthInfo into the Authentication property
				hr = pthing->put_String(m_bstrAuthentication, bstrAuthInfo);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;
			}
		}
	}

	hr = get_Avatars(&pAvatars);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	{
		CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);

		// add avatar to avatar map
		hr = pAvatars->put_Thing(bstrNewAlias, pthing);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	//hr = get_GUIDs(&pGUIDs);
	//if (FAILED(hr))
	//	goto ERROR_ENCOUNTERED;

	//already there.
	// add avatar to GUID map
	/*hr = pGUIDs->put_Thing(bstrGUID, *ppthing);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	*/
	
	// Fire a world event.
	hr = FireCreateObject(pthing);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (m_bServerSide == VARIANT_TRUE)
	{
		// update client with new avatar
		hr = SendThing(soClient, pthing);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	{
		long lAvatars = 0;

		hr = IncrementAvatarCount(&lAvatars);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

#ifdef WORLD_PERF_DEBUG
		VWREPORT(m_pWorld, VWR_PERFLOG, "Create Avatar,%d,%s,%d\n",
			GetTickCount(), CString(bstrNewAlias), lAvatars);
#endif
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_CREATEOBJECT, "CWorldObject::CreateAvatarExtInPlace: filled in incomplete avatar, preferred name (%s), name (%s), GUID %s\n", 
		CString(bstrName), CString(bstrNewAlias), CString(bstrGUID));
#endif
	
	*pbstrNewName = bstrNewAlias.Copy();

ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{
		// remove from avatar list
		if (pAvatars)
		{
			CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);

			pAvatars->Remove(bstrNewAlias);

	//		pGUIDs->Remove(bstrGUID); don't touch this.
		}
	}

	SAFERELEASE(pAuth);
	SAFERELEASE(pDefaultAvatarEx);
	SAFERELEASE(pAvatars);
	//SAFERELEASE(pGUIDs);
	SAFEFREESTRING(bstrAuthInfo);

	return hr;
}

STDMETHODIMP CWorldObject::DestroyObjectByID(long index)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	IThing* pthing = NULL;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::DestroyObjectByID, id %d\n", index);

	hr = get_ObjectByID(index, &pthing);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (pthing)
	{
		hr = DestroyObject(pthing);

		// release our ref
		SAFERELEASE(pthing);
	}

ERROR_ENCOUNTERED:
	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::DestroyObject(IThing* pthing)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	IThing* pContainer = NULL;
	IThing *pOwner = NULL;
	IThing* pCaller = NULL;
	VARIANT_BOOL bIsRoom = VARIANT_FALSE;
	VARIANT_BOOL bIsAvatar = VARIANT_FALSE;
	VARIANT_BOOL bStub = VARIANT_FALSE;
	VARIANT_BOOL bIsExemplar = VARIANT_FALSE;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::DestroyObject\n");

	ASSERT(pthing);
	if (pthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::DestroyObject: incoming argument (IThing*) is NULL\n");
		return ReportWorldError(E_INVALIDARG);
	}

	/* hr = */ pthing->get_IsStub(&bStub);

	// don't test for avatar/room if stub
	if (bStub == VARIANT_FALSE)
	{
		hr = IsExemplar(pthing, &bIsExemplar);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (bIsExemplar == VARIANT_TRUE)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::DestroyObject: can't destroy exemplars\n");
			hr = VWOBJECT_E_CANTDESTROYEXEMPLAR;
			goto ERROR_ENCOUNTERED;
		}

		// REVIEW: temporarily disallow deleting of avatars.  Need to resolve what to do with deleted avatars.
		hr = IsAvatar(pthing, &bIsAvatar);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (bIsAvatar == VARIANT_TRUE)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::DestroyObject: can't destroy avatars\n");
			hr = VWOBJECT_E_CANTDESTROYAVATAR;
			goto ERROR_ENCOUNTERED;
		}

		if (m_pUser != NULL && pthing == m_pUser)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::DestroyObject: can't destroy user\n");
			hr = VWOBJECT_E_CANTDESTROYUSER;
			goto ERROR_ENCOUNTERED;
		}

		// REVIEW: temporarily disallow deleting of rooms.  Need to figure out what to do with the objects within the rooms
		hr = IsRoom(pthing, &bIsRoom);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (bIsRoom == VARIANT_TRUE)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::DestroyObject: can't destroy rooms\n");
			hr = VWOBJECT_E_CANTDESTROYROOM;
			goto ERROR_ENCOUNTERED;
		}

		if (m_pUser != NULL)
		{
			// REVIEW: redundant with above code, but needed if we remove the deleting rooms restriction
			/* hr = */ m_pUser->get_Container(&pContainer);
			if (pthing == pContainer)
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::DestroyObject: can't destroy user's container\n");
				hr = VWOBJECT_E_CANTDESTROYCONTAINER;
				goto ERROR_ENCOUNTERED;
			}
		}
		// Can delete if you are owner, if you are an object with the same owner, or a wizard (includes world owner)
		hr = CheckPolicyExt(pthing, CP_OWNER | CP_CALLER | CP_WIZARD, CComVariant());
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::DestroyObject: must be owner, world owner or wizard to destroy object\n");
			goto ERROR_ENCOUNTERED;
		}
	}

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote);

	if (bShouldExecute == VARIANT_TRUE)
	{
		hr = DestroyObjectInt(pthing);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		HRESULT senthr = S_OK;
		CComVariant var;
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = pthing;

		hr = SendWorldCommand(VARIANT_TRUE, DISPID_WORLD_DESTROYOBJECT, DISPATCH_METHOD, &dispparams, &senthr, &var);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		if (SUCCEEDED(hr) && hr != VWOBJECT_S_NOTREMOTED)
		{
			hr = senthr;
		}
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pContainer);
	SAFERELEASE(pOwner);
	SAFERELEASE(pCaller);

	return ReportWorldError(hr);
}

HRESULT CWorldObject::DestroyObjectInt(IThing* pthing)
{
	HRESULT hr = S_OK;
	CComBSTR bstr;
	CComBSTR bstrGUID;
	VARIANT_BOOL fValid = VARIANT_FALSE;
	VARIANT_BOOL bExemplar = VARIANT_FALSE;
	IThing *pOwner = NULL;
	IPropertyMap* pAvatars = NULL;
	IPropertyMap* pGUIDs = NULL;
	IVWAnimator *pAnim = NULL;
	long lObjects = 0;
	DISPPARAMS dispparams;
	CComVariant varRet;
	VARIANT_BOOL bStub = VARIANT_FALSE;
	static CComBSTR bstrOnSystemPreDestroy("OnSystemPreDestroy");
	static CComBSTR bstrOnSystemPostDestroy("OnSystemPostDestroy");
	static CComBSTR bstrOnSystemDestroyClient("OnSystemDestroyClient");

	// initialize here to avoid problems if exiting early
	InitializeDISPPARAMS(&dispparams, 0);

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::DestroyObjectInt\n");

	/* hr = */ pthing->get_IsStub(&bStub);

	if (bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CWorldObject::DestroyObjectInt: attempting to destroy stub object\n");
		hr = VWOBJECT_E_ALREADYDESTROYED;
		goto ERROR_ENCOUNTERED;
	}

	// add a reference to pthing to avoid it disappearing on us (removed later)
	pthing->AddRef();

	// ask the owner if it's ok to remove the object
	hr = pthing->get_Owner(&pOwner);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// ask the owner if it's ok to destroy
	if (pOwner)
	{
		CSetUID SetUID(this, pOwner, pOwner /*, VARIANT_TRUE */);

		// package up args into DISPPARAMS struct (pass in exemplar)
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = pthing;

		hr = pOwner->InvokeMethodExt(bstrOnSystemPreDestroy, &dispparams, &varRet);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		ASSERT(varRet.vt==VT_BOOL);
		if (varRet.boolVal != VARIANT_TRUE)
		{
			hr = VWOBJECT_E_PERMISSION_DENIED;

#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::DestroyObjectInt: Owner does not allow destroy\n");
#endif
			goto ERROR_ENCOUNTERED;
		}
	}

	// remove any related animations to this thing
	if (SUCCEEDED(get_Tool(CComBSTR("Animator"), (IUnknown**)&pAnim)) && pAnim != NULL)
	{
		/* hr = */ pAnim->RemoveThingAnimations(pthing);

		SAFERELEASE(pAnim);
	}

	hr = FireDestroyObject(pthing);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = get_GUIDs(&pGUIDs);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// check if in avatarGUID map and remove it
	hr = pthing->get_String(m_bstrGUID, &bstrGUID.m_str);
	if (FAILED(hr))
	{
		// if we failed because there is no guid, then all's ok
		if (hr != VWOBJECT_E_PROPERTYNOTEXIST)
			goto ERROR_ENCOUNTERED;
	}
	else
	{

		hr = pGUIDs->IsValid(bstrGUID, &fValid);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// remove from (local) avatarGUID map, may not be found
		if (fValid)
		{
			CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);

			pGUIDs->Remove(bstrGUID);
		}
	}

	hr = get_Avatars(&pAvatars);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// check if in avatarmap and remove it		
	hr = pthing->get_Name(&bstr.m_str);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pAvatars->IsValid(bstr, &fValid);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// remove from (local) avatar map, may not be found
	if (fValid)
	{
		long lAvatars = 0;

		pAvatars->Remove(bstr);

		hr = DecrementAvatarCount(&lAvatars);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

#ifdef WORLD_PERF_DEBUG
		VWREPORT(m_pWorld, VWR_PERFLOG, "Remove Avatar,%d,%s,%d\n",
			GetTickCount(), CString(bstr), lAvatars);
#endif
	}

	hr = pthing->get_IsExemplar(&bExemplar);
	if (SUCCEEDED(hr) && bExemplar == VARIANT_TRUE)
	{
		// convert to instance, remove from exemplar map
		/* hr = */ ConvertExemplarToInstance(pthing);
	}

	hr = DecrementObjectCount(&lObjects);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

#ifdef WORLD_PERF_DEBUG
	VWREPORT(m_pWorld, VWR_PERFLOG, "Remove Object,%d,%d\n",
		GetTickCount(), lObjects);
#endif

	// only do if running on server (not in ConnectLocal mode)
	if (m_bServerSide == VARIANT_TRUE)
	{
		// free up old stuff in dispparams
		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		{
			CSetUID SetUID(this, pOwner, pOwner);

			// package up args into DISPPARAMS struct (pass in object)
			hr = InitializeDISPPARAMS(&dispparams, 1);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			dispparams.rgvarg[0].vt = VT_DISPATCH;
			dispparams.rgvarg[0].pdispVal = pthing;

			// tell thing itself (on all clients)
			hr = pthing->InvokeMethodExt(bstrOnSystemDestroyClient, &dispparams, NULL);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

	// move object to NULL destination, so there will be
	// no dangling container references

	// REVIEW: do in local mode, need to do after sending event to client (otherwise we have no bystanders)
	// but then we don't want to tell clients that we're moving or they'll get stub errors
	m_bLocalMode = VARIANT_TRUE;

	hr = pthing->MoveIntoInt(NULL);

	m_bLocalMode = VARIANT_FALSE;

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// actually delete object from VWODB
	hr = DeleteObjectInt(pthing);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// mark as stub
	/* hr = */ pthing->put_IsStubInt(VARIANT_TRUE);

	hr = LogDeleteObject(pthing);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (pOwner)
	{
		CSetUID SetUID(this, pOwner, pOwner /* , VARIANT_TRUE */);

		// tell owner we've destroyed an object

		// free up old stuff in dispparams
		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		// package up args into DISPPARAMS struct (pass in object)
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = pthing;

		// tell owner
		hr = pOwner->InvokeMethodExt(bstrOnSystemPostDestroy, &dispparams, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

	SAFERELEASE(pOwner);
	SAFERELEASE(pAvatars);
	SAFERELEASE(pGUIDs);

	// release extra reference
	SAFERELEASE(pthing);

	return hr;
}

#ifdef VWSYSTEM
STDMETHODIMP CWorldObject::CreatePropertyList(IPropertyList** pppropertylist)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreatePropertyList\n");

	ASSERT(pppropertylist);
	if (pppropertylist == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreatePropertyList: outgoing pointer (IPropertyList**) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*pppropertylist = new CComPropertyList;

	if (*pppropertylist == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreatePropertyList: failed to allocate CComPropertyList\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(*pppropertylist);

	(*pppropertylist)->AddRef();

	hr = (*pppropertylist)->put_World((IWorld*)GetUnknown());

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*pppropertylist);
	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::CreatePropertyMap(IPropertyMap** pppropertymap)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreatePropertyMap\n");

	ASSERT(pppropertymap);
	if (pppropertymap == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreatePropertyMap: outgoing pointer (IPropertyMap**) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*pppropertymap = new CComPropertyMap;

	if (*pppropertymap == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreatePropertyMap: failed to allocate CComPropertyMap\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(*pppropertymap);

	(*pppropertymap)->AddRef();

	hr = (*pppropertymap)->put_World((IWorld*)GetUnknown());

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*pppropertymap);
	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::CreateMenu(IPropertyList** ppmenu)
{
	VALIDATE_WORLD;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateMenu\n");

	HRESULT hr = S_OK;

	hr = CreatePropertyList(ppmenu);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmenu)->put_Type(m_bstrMenu);

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppmenu);
	return hr;
}

STDMETHODIMP CWorldObject::CreateMenuItem(BSTR bstrMenuText, long lFlags, IThing* pOwner, BSTR bstrUpdateMethod, BSTR bstrMethod, VARIANT varArgs, IMenuItem** ppMenuItem)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	DISPPARAMS dispparams;
	CComVariant varTemp;

	// VT_ERROR means optional arg not passed in
	if (varArgs.vt != VT_ERROR)
	{
		hr = VariantCopyInd(&varTemp, &varArgs);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = ConvertSAFEARRAYToDISPPARAMS(&varTemp, &dispparams);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = CreateMenuItemExt(bstrMenuText, lFlags, pOwner, bstrUpdateMethod, bstrMethod, &dispparams, ppMenuItem);

		TerminateDISPPARAMS(VARIANT_TRUE, &dispparams);
	}
	else
		hr = CreateMenuItemExt(bstrMenuText, lFlags, pOwner, bstrUpdateMethod, bstrMethod, NULL, ppMenuItem);

ERROR_ENCOUNTERED:
	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::CreateMenuItemExt(BSTR bstrMenuText, long lFlags, IThing* pOwner, BSTR bstrUpdateMethod, BSTR bstrMethod, DISPPARAMS* params, IMenuItem** ppmenuitem)
{
	VALIDATE_WORLD;

	HRESULT	hr = S_OK;

	ASSERT(ppmenuitem);
	if (ppmenuitem == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateMenuItemExt: outgoing pointer (IObjectProperty**) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*ppmenuitem = new CComMenuItem;

	if (*ppmenuitem == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateMenuItemExt: failed to allocate CComMenuItem\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(*ppmenuitem);

	(*ppmenuitem)->AddRef();

	hr = (*ppmenuitem)->put_World((IWorld*)GetUnknown());
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmenuitem)->put_MenuText(bstrMenuText);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmenuitem)->put_Method(bstrMethod);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmenuitem)->put_ArgumentsExt(params);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmenuitem)->put_OnUpdateMethod(bstrUpdateMethod);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmenuitem)->put_Flags(lFlags);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmenuitem)->put_Owner(pOwner);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppmenuitem);
	return hr;
}

STDMETHODIMP CWorldObject::CreateOCXProxy(IDispatch *pOCX, IOCXProxy** ppOCXProxy)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;

	ASSERT(ppOCXProxy);
	if (ppOCXProxy == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateOCXProxy: outgoing pointer (IObjectProperty**) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*ppOCXProxy = new CComOCXProxy;

	if (*ppOCXProxy == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateOCXProxy: failed to allocate CComOCXProxy\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(*ppOCXProxy);

	(*ppOCXProxy)->AddRef();

	hr = (*ppOCXProxy)->put_World((IWorld*)GetUnknown());
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppOCXProxy)->put_OCX(pOCX);
   	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppOCXProxy);
	return hr;
}
#endif

STDMETHODIMP CWorldObject::CreateObjectPropertyExt(REFCLSID clsid, IUnknown* punk, IObjectProperty** ppproperty)
{
	IUnknown* punkProp = NULL;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateObjectPropertyExt\n");

	// NOTE: punk can be NULL

	ASSERT(ppproperty);
	if (ppproperty == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateObjectPropertyExt: outgoing pointer (IObjectProperty**) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*ppproperty = NULL;

	HRESULT hr = CoCreateInstance(clsid, punk, CLSCTX_INPROC_SERVER, IID_IObjectProperty, (LPVOID *)ppproperty);
	if (FAILED(hr) || *ppproperty == NULL)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateObjectPropertyExt: failed to create component\n");
#endif
		hr = VWOBJECT_E_COMPONENTNOTFOUND;
		goto ERROR_ENCOUNTERED;
	}

	// make sure property is safe for scripting
	if (ValidateObjectSafety(clsid, *ppproperty, INTERFACESAFE_FOR_UNTRUSTED_CALLER) != S_OK)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateObjectPropertyExt: component is not safe for scripting\n");
#endif
		hr = VWOBJECT_E_NOTSAFEFORSCRIPTING;
		goto ERROR_ENCOUNTERED;
	}

	hr = (*ppproperty)->put_World((IWorld*)GetUnknown());

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppproperty);

	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::CreateObjectProperty(BSTR bstrProgID, IObjectProperty** ppproperty)
{
	VALIDATE_WORLD;

	CLSID clsid;
	HRESULT hr = S_OK;

	ASSERT(bstrProgID);
	if (bstrProgID == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateObjectProperty: incoming argument (BSTR) is NULL\n");
		return ReportWorldError(E_INVALIDARG);
	}

	hr = CLSIDFromProgID(bstrProgID, &clsid);
	if (FAILED(hr))
		return ReportWorldError(hr);

	hr = CreateObjectPropertyExt(clsid, NULL, ppproperty);

	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::CreateMethodByDispID(long lFlags, VARIANT varModule, DISPID dispid, IMethod** ppmethod)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	IModule* pmodule = NULL;
	CComVariant varTemp;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateMethodByDispID: dispid %d\n", dispid);

	ASSERT(ppmethod);
	if (ppmethod == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateMethodByDispID: outgoing pointer (IMethod**) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*ppmethod = NULL;

	hr = VariantCopyInd(&varTemp, &varModule);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (varTemp.vt == VT_DISPATCH)
	{
		CModulePtr pMod(varTemp.pdispVal);

		if (pMod != NULL)
		{
			pmodule = pMod;
			SAFEADDREF(pmodule);
		}
	}
	else if (varTemp.vt == VT_BSTR)
	{
		hr = get_Module(varTemp.bstrVal, &pmodule);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}
	else
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateMethodByDispID: invalid module argument (must be either BSTR or IDispatch*)\n");

		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	if (pmodule == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateMethodByDispID: incoming argument (IModule*) is NULL\n");

		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	*ppmethod = new CComMethod;

	if (*ppmethod == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateMethodByDispID: failed to allocate CComMethod\n");

		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(*ppmethod);

	(*ppmethod)->AddRef();

	hr = (*ppmethod)->put_World((IWorld*)GetUnknown());
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmethod)->put_Flags(lFlags);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmethod)->put_Module(pmodule);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmethod)->put_DispID(dispid);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppmethod);

	SAFERELEASE(pmodule);

	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::CreateMethod(long lFlags, VARIANT varModule, BSTR bstrName, IMethod** ppmethod)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	IModule* pmodule = NULL;
	CComVariant varTemp;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateMethod: name %s\n", CString(bstrName));
#endif

	ASSERT(ppmethod);
	if (ppmethod == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateMethod: outgoing pointer (IMethod**) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*ppmethod = NULL;

	hr = VariantCopyInd(&varTemp, &varModule);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (varTemp.vt == VT_DISPATCH)
	{
		CModulePtr pMod(varTemp.pdispVal);

		if (pMod != NULL)
		{
			pmodule = pMod;
			SAFEADDREF(pmodule);
		}
	}
	else if (varTemp.vt == VT_BSTR)
	{
		hr = get_Module(varTemp.bstrVal, &pmodule);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}
	else
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateMethod: invalid module argument (must be either BSTR or IDispatch*)\n");

		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	if (pmodule == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateMethod: incoming argument (IModule*) is NULL\n");

		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	*ppmethod = new CComMethod;

	if (*ppmethod == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateMethod: failed to allocate CComMethod\n");

		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(*ppmethod);

	(*ppmethod)->AddRef();

	hr = (*ppmethod)->put_World((IWorld*)GetUnknown());
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmethod)->put_Flags(lFlags);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmethod)->put_Module(pmodule);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmethod)->put_MethodName(bstrName);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppmethod);

	SAFERELEASE(pmodule);

	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::CreateInlineMethod(long lFlags, BSTR bstrLanguage, BSTR bstrName, BSTR bstrCode, IMethod** ppmethod)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	CString strLanguage(bstrLanguage);
	IModule *pmod = NULL;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateInlineMethod");
#endif

	ASSERT(bstrLanguage);
	if (bstrLanguage == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateInlineMethod: incoming argument (BSTR) is NULL\n");
		return ReportWorldError(E_INVALIDARG);
	}

	ASSERT(bstrName);
	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateInlineMethod: incoming argument (BSTR) is NULL\n");
		return ReportWorldError(E_INVALIDARG);
	}

	ASSERT(bstrCode);
	if (bstrCode == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateInlineMethod: incoming argument (BSTR) is NULL\n");
		return ReportWorldError(E_INVALIDARG);
	}

	// see what module to use.  
	if (strLanguage.CompareNoCase("VBScript") == 0)
	{
		// VBScript - synthesize the special module name
		hr = get_Module(CComBSTR(":VBScript"), &pmod);
	}
	else
	{
		// we don't support any other modules for inline methods
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateInlineMethod: unknown scripting language (%s)\n", strLanguage);

		// get the module 
//		hr = get_Module(bstrLanguage, &pmod);
		hr = VWOBJECT_E_MODULENOTEXIST;
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	ASSERT(ppmethod);
	if (ppmethod == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateInlineMethod: outgoing pointer (IMethod**) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*ppmethod = new CComMethod;

	if (*ppmethod == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateInlineMethod: failed to allocate CComMethod\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(*ppmethod);

	(*ppmethod)->AddRef();

	hr = (*ppmethod)->put_World((IWorld*)GetUnknown());
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmethod)->put_Flags(lFlags);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmethod)->put_Module(pmod);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmethod)->put_MethodName(bstrName);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmethod)->put_Code(bstrCode);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	SAFERELEASE(pmod);

	if (FAILED(hr))
		SAFERELEASE(*ppmethod);

	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::CreateCOMModule(BSTR bstrName, BSTR bstrProgID, long lFlags, IModule** ppmodule)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	IPropertyMap* pModules = NULL;
	IPropertyList* pModuleNames = NULL;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateCOMModule: progid %s\n", CString(bstrProgID));
#endif

	ASSERT(bstrName);
	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateCOMModule: incoming argument (BSTR) is NULL\n");
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(bstrProgID);
	if (bstrProgID == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateCOMModule: incoming argument (BSTR) is NULL\n");
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(ppmodule);
	if (ppmodule == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateCOMModule: outgoing pointer (IModule**) is NULL\n");
		hr = E_POINTER;
		goto ERROR_ENCOUNTERED;
	}

	*ppmodule = NULL;

#ifdef SUPPORT_WORLDOWNER_RESTRICTIONS
	hr = CheckPolicyExt(NULL, CP_WORLDOWNER | CP_WIZARD, CComVariant());
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateCOMModule: must be world owner to create modules\n");
		goto ERROR_ENCOUNTERED;
	}
#endif

	hr = get_Modules(&pModules);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = get_ModuleNameList(&pModuleNames);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote);

	if (bShouldExecute == VARIANT_TRUE)
	{
#ifdef _DEBUG
		CString str(bstrName);
#endif
		VARIANT_BOOL fValid = VARIANT_FALSE;

		hr = pModules->IsValid(bstrName, &fValid);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (fValid)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateCOMModule: module already exists (%s)\n", str);
#endif
			hr = VWOBJECT_E_MODULEEXISTS;
			goto ERROR_ENCOUNTERED;
		}

		hr = CreateCOMModuleEx(bstrName, bstrProgID, lFlags, ppmodule);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// attempt to load COM module (tests object safety)
		m_bLocalMode = VARIANT_TRUE;

		hr = (*ppmodule)->Reload(VARIANT_FALSE);

		m_bLocalMode = VARIANT_FALSE;

		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (m_bServerSide == VARIANT_TRUE)
		{
			// update all clients with new module
			hr = SendModule(soAvatars, *ppmodule, bstrName);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}

		hr = LogModule(*ppmodule, bstrName);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		{
			CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);

			// don't remote or log (module is remoted by ref, and this errors on the client)
			m_bShouldRemote = VARIANT_FALSE;

			hr = pModules->put_ObjectProperty(bstrName, *ppmodule);

			m_bShouldRemote = VARIANT_TRUE;

			{
				CString strMungedName(bstrName);
				strMungedName.MakeLower();
				strMungedName += "#";

				hr = pModuleNames->AddString(CComBSTR(strMungedName));
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;
			}
		}

		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateCOMModule: failed to add module to module map or name list\n");
			goto ERROR_ENCOUNTERED;
		}

		// Call the Install method
		hr = (*ppmodule)->Install();
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		NotifyModuleChanged(bstrName, moCreate, CComVariant());
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		CComVariant var;
		HRESULT senthr = S_OK;
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 3);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[2].vt = VT_BSTR;
		dispparams.rgvarg[2].bstrVal = bstrName;

		dispparams.rgvarg[1].vt = VT_BSTR;
		dispparams.rgvarg[1].bstrVal = bstrProgID;

		dispparams.rgvarg[0].vt = VT_I4;
		dispparams.rgvarg[0].lVal = lFlags;

		hr = SendWorldCommand(VARIANT_TRUE, DISPID_WORLD_CREATECOMMODULE, DISPATCH_METHOD, &dispparams, &senthr, &var);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		// if sendmessage and invoke both succeeded, pull out retval
		if (SUCCEEDED(hr) && hr != VWOBJECT_S_NOTREMOTED)
		{
			hr = senthr;

			if (SUCCEEDED(senthr))
			{
				ASSERT(var.vt == VT_DISPATCH);

				*ppmodule = (IModule*)var.pdispVal;

				SAFEADDREF(*ppmodule);

				NotifyModuleChanged(bstrName, moCreate, CComVariant());
			}
		}
	}

ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{
		if (*ppmodule)
		{
			// unload module before removing (mark as shutdown)
			/* hr = */ (*ppmodule)->Unload(VARIANT_TRUE);
		}

		{
			CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);

			if (pModules)
			{
				// don't remote or log (module is remoted by ref, and this errors on the client)
//				m_bShouldRemote = VARIANT_FALSE;

				/* hr = */ pModules->Remove(bstrName);

//				m_bShouldRemote = VARIANT_TRUE;
			}

			if (pModuleNames)
			{
				CString strMungedName(bstrName);
				strMungedName.MakeLower();
				strMungedName += "#";

				/* hr = */ pModuleNames->RemoveString(CComBSTR(strMungedName));
			}
		}

		SAFERELEASE(*ppmodule);
	}

	SAFERELEASE(pModules);
	SAFERELEASE(pModuleNames);

	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::CreateScriptModule(BSTR bstrName, BSTR bstrScriptPath, long lFlags, IModule** ppmodule)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	IPropertyMap* pModules = NULL;
	IPropertyList* pModuleNames = NULL;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateScriptModule\n");

	ASSERT(bstrName);
	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateScriptModule: incoming argument (BSTR) is NULL\n");
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(bstrScriptPath);
	if (bstrScriptPath == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateScriptModule: incoming argument (BSTR) is NULL\n");
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(ppmodule);
	if (ppmodule == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateScriptModule: outgoing pointer (IModule**) is NULL\n");
		hr = E_POINTER;
		goto ERROR_ENCOUNTERED;
	}

	*ppmodule = NULL;

#ifdef SUPPORT_WORLDOWNER_RESTRICTIONS
	hr = CheckPolicyExt(NULL, CP_WORLDOWNER | CP_WIZARD, CComVariant());
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateScriptModule: must be world owner to create modules\n");
		goto ERROR_ENCOUNTERED;
	}
#endif

	hr = get_Modules(&pModules);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = get_ModuleNameList(&pModuleNames);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote);

	if (bShouldExecute == VARIANT_TRUE)
	{
#ifdef _DEBUG
		CString str(bstrName);
#endif
		VARIANT_BOOL fValid = VARIANT_FALSE;

		hr = pModules->IsValid(bstrName, &fValid);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (fValid)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateScriptModule: module already exists (%s)\n", str);
#endif
			hr = VWOBJECT_E_MODULEEXISTS;
			goto ERROR_ENCOUNTERED;
		}

		hr = CreateScriptModuleEx(bstrName, bstrScriptPath, lFlags, ppmodule);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// attempt to load script module (attempts to d/l file)

		m_bLocalMode = VARIANT_TRUE;

		hr = (*ppmodule)->Reload(VARIANT_FALSE);

		m_bLocalMode = VARIANT_FALSE;

		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (m_bServerSide == VARIANT_TRUE)
		{
			// update all clients with new module
			hr = SendModule(soAvatars, *ppmodule, bstrName);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}

		hr = LogModule(*ppmodule, bstrName);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		{
			CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);

			// don't remote or log (module is remoted by ref, and this errors on the client)
			m_bShouldRemote = VARIANT_FALSE;

			hr = pModules->put_ObjectProperty(bstrName, *ppmodule);

			m_bShouldRemote = VARIANT_TRUE;

			{
				CString strMungedName(bstrName);
				strMungedName.MakeLower();
				strMungedName += "#";

				hr = pModuleNames->AddString(CComBSTR(strMungedName));
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;
			}
		}

		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateScriptModule: failed to add module to module map or name list\n");
			goto ERROR_ENCOUNTERED;
		}

		// Call the Install method
		hr = (*ppmodule)->Install();
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		NotifyModuleChanged(bstrName, moCreate, CComVariant());
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		CComVariant var;
		HRESULT senthr = S_OK;
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 3);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[2].vt = VT_BSTR;
		dispparams.rgvarg[2].bstrVal = bstrName;

		dispparams.rgvarg[1].vt = VT_BSTR;
		dispparams.rgvarg[1].bstrVal = bstrScriptPath;

		dispparams.rgvarg[0].vt = VT_I4;
		dispparams.rgvarg[0].lVal = lFlags;

		hr = SendWorldCommand(VARIANT_TRUE, DISPID_WORLD_CREATESCRIPTMODULE, DISPATCH_METHOD, &dispparams, &senthr, &var);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		// if sendmessage and invoke both succeeded, pull out retval
		if (SUCCEEDED(hr) && hr != VWOBJECT_S_NOTREMOTED)
		{
			hr = senthr;

			if (SUCCEEDED(senthr))
			{
				ASSERT(var.vt == VT_DISPATCH);

				*ppmodule = (IModule*)var.pdispVal;

				SAFEADDREF(*ppmodule);

				NotifyModuleChanged(bstrName, moCreate, CComVariant());
			}
		}
	}

ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{
		if (*ppmodule)
		{
			// unload module before removing (mark as shutdown)
			/* hr = */ (*ppmodule)->Unload(VARIANT_TRUE);
		}

		{
			CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);

			if (pModules)
			{
				// don't remote or log (module is remoted by ref, and this errors on the client)
//				m_bShouldRemote = VARIANT_FALSE;

				/* hr = */ pModules->Remove(bstrName);

//				m_bShouldRemote = VARIANT_TRUE;
			}

			if (pModuleNames)
			{
				CString strMungedName(bstrName);
				strMungedName.MakeLower();
				strMungedName += "#";

				/* hr = */ pModuleNames->RemoveString(CComBSTR(strMungedName));
			}
		}

		SAFERELEASE(*ppmodule);
	}

	SAFERELEASE(pModules);
	SAFERELEASE(pModuleNames);

	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::CreateCOMModuleEx(BSTR bstrName, BSTR bstrProgID, long lFlags, IModule** ppmodule)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	ICOMModule* pcommodule = NULL;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateCOMModuleEx\n");

	ASSERT(bstrName);
	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateCOMModuleEx: incoming argument (BSTR) is NULL\n");
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(bstrProgID);
	if (bstrProgID == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateCOMModuleEx: incoming argument (BSTR) is NULL\n");
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(ppmodule);
	if (ppmodule == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateCOMModuleEx: outgoing pointer (IModule**) is NULL\n");
		hr = E_POINTER;
		goto ERROR_ENCOUNTERED;
	}

	*ppmodule = new CComCOMModule;

	if (*ppmodule == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateCOMModuleEx: failed to allocate CComCOMModule\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(*ppmodule);

	(*ppmodule)->AddRef();

	hr = (*ppmodule)->put_World((IWorld*)GetUnknown());
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (SUCCEEDED((*ppmodule)->QueryInterface(IID_ICOMModule, (LPVOID *)&pcommodule)))
	{
		ASSERT(pcommodule);

		CCOMModuleObject* pcom = (CCOMModuleObject*)pcommodule;
		CLSID clsid;

		pcom->m_bstrModuleName = bstrName;
		pcom->m_lFlags = lFlags;

		hr = CLSIDFromProgID(bstrProgID, &clsid);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED_1;

		pcom->m_ModuleCLSID = clsid;

ERROR_ENCOUNTERED_1:
		SAFERELEASE(pcommodule);
	}
	else
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateCOMModuleEx: not a COM module\n");
		hr = VWOBJECT_E_INVALIDMODULETYPE;
		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppmodule);

	return hr;
}

STDMETHODIMP CWorldObject::CreateScriptModuleEx(BSTR bstrName, BSTR bstrScriptPath, long lFlags, IModule** ppmodule)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	IScriptModule* pscriptmodule = NULL;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateScriptModuleEx\n");

	ASSERT(bstrName);
	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateScriptModuleEx: incoming argument (BSTR) is NULL\n");
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(bstrScriptPath);
	if (bstrScriptPath == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateScriptModuleEx: incoming argument (BSTR) is NULL\n");
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(ppmodule);
	if (ppmodule == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateScriptModuleEx: outgoing pointer (IModule**) is NULL\n");
		hr = E_POINTER;
		goto ERROR_ENCOUNTERED;
	}

	*ppmodule = new CComScriptModule;

	if (*ppmodule == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateScriptModuleEx: failed to allocate CComScriptModule\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(*ppmodule);

	(*ppmodule)->AddRef();

	hr = (*ppmodule)->put_World((IWorld*)GetUnknown());
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (SUCCEEDED((*ppmodule)->QueryInterface(IID_IScriptModule, (LPVOID *)&pscriptmodule)))
	{
		ASSERT(pscriptmodule);

		CScriptModuleObject* pscript = (CScriptModuleObject*)pscriptmodule;

		pscript->m_lFlags = lFlags;
		pscript->m_bstrModuleName = bstrName;
		pscript->m_bstrScriptPath = bstrScriptPath;

		SAFERELEASE(pscriptmodule);
	}
	else
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateScriptModuleEx: not a script module\n");
		hr = VWOBJECT_E_INVALIDMODULETYPE;
	}

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppmodule);

	return hr;
}

STDMETHODIMP CWorldObject::CreateLocalScriptModuleEx(BSTR bstrName, IModule **ppmodule)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	IScriptModule* pscriptmodule = NULL;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateLocalScriptModuleEx\n");

	ASSERT(bstrName);
	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateLocalScriptModuleEx: incoming argument (BSTR) is NULL\n");
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(ppmodule);
	if (ppmodule == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateLocalScriptModuleEx: outgoing pointer (IModule**) is NULL\n");
		hr = E_POINTER;
		goto ERROR_ENCOUNTERED;
	}

	*ppmodule = new CComScriptModule;

	if (*ppmodule == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateLocalScriptModuleEx: failed to allocate CComScriptModule\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(*ppmodule);

	(*ppmodule)->AddRef();

	hr = (*ppmodule)->put_World((IWorld*)GetUnknown());
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (SUCCEEDED((*ppmodule)->QueryInterface(IID_IScriptModule, (LPVOID *)&pscriptmodule)))
	{
		ASSERT(pscriptmodule);

		// need to directly set member data of local script module, in order to 
		// bypass world owner policy checking

		CScriptModuleObject* pscript = (CScriptModuleObject*)pscriptmodule;

		pscript->m_lFlags = MODULE_CLIENT | MODULE_SERVER | MODULE_LOCAL;
		pscript->m_bstrModuleName = bstrName;
		pscript->m_bstrScriptPath = bstrName;

		SAFERELEASE(pscriptmodule);
	}
	else
	{
		// not a script module
//			ASSERT(FALSE);

		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateLocalScriptModuleEx: not a script module\n");
		hr = VWOBJECT_E_INVALIDMODULETYPE;
	}

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppmodule);

	return hr;
}

STDMETHODIMP CWorldObject::RemoveModule(BSTR bstrName)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	IPropertyMap* pModules = NULL;
	IPropertyList* pModuleNames = NULL;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	IModule *pModule = NULL;
	VARIANT_BOOL fValid = VARIANT_FALSE;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::RemoveModule\n");

	ASSERT(bstrName);
	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::RemoveModule: incoming argument (BSTR) is NULL\n");
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	hr = get_Modules(&pModules);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pModules->IsValid(bstrName, &fValid);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (fValid == VARIANT_FALSE)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::RemoveModule: module doesn't exist (%s)\n", CString(bstrName));
#endif
		hr = VWOBJECT_E_MODULENOTEXIST;
		goto ERROR_ENCOUNTERED;
	}

#ifdef SUPPORT_WORLDOWNER_RESTRICTIONS
	hr = CheckPolicyExt(NULL, CP_WORLDOWNER | CP_WIZARD, CComVariant());
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::RemoveModule: must be world owner to remove modules\n");
		goto ERROR_ENCOUNTERED;
	}
#endif

	hr = get_Module(bstrName, &pModule);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote);

	if (bShouldExecute == VARIANT_TRUE)
	{
		// unload module before removing (mark as shutdown)
		hr = pModule->Unload(VARIANT_TRUE);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = get_ModuleNameList(&pModuleNames);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		{
			CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);

			// REVIEW: ignore errors to make sure both map and list get cleared (IsValid should have already made sure this will work)

			/* hr = */ pModules->Remove(bstrName);

			// NOTE: property list not case-insensitive

			CString strMungedName(bstrName);
			strMungedName.MakeLower();
			strMungedName += "#";

			/* hr = */ pModuleNames->RemoveString(CComBSTR(strMungedName));
		}

		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::RemoveModule: failed to remove module from module map or name list\n");
			goto ERROR_ENCOUNTERED;
		}

		NotifyModuleChanged(bstrName, moRemove, CComVariant());
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldRemote == VARIANT_TRUE)
	{
		HRESULT senthr = S_OK;
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstrName;

		hr = SendWorldCommand(VARIANT_TRUE, DISPID_WORLD_REMOVEMODULE, DISPATCH_METHOD, &dispparams, &senthr, NULL);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		// if sendmessage and invoke both succeeded
		if (SUCCEEDED(hr) && hr != VWOBJECT_S_NOTREMOTED)
		{
			hr = senthr;

			if (SUCCEEDED(senthr))
			{
				// unload module on client-side (mark as shutdown)
				/* hr = */ pModule->Unload(VARIANT_TRUE);

				NotifyModuleChanged(bstrName, moRemove, CComVariant());
			}
		}
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pModule);
	SAFERELEASE(pModules);
	SAFERELEASE(pModuleNames);

	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::CreateToolExt(BSTR bstrName, REFCLSID clsid, IUnknown** ppunk)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL fValid = VARIANT_FALSE;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateToolExt\n");

	ASSERT(bstrName);
	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateToolExt: incoming argument (BSTR) is NULL\n");
		return ReportWorldError(E_INVALIDARG);
	}

	ASSERT(ppunk);
	if (ppunk == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateToolExt: outgoing pointer (IUnknown**) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*ppunk = NULL;

	if (m_pTools == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateToolExt: invalid tool map\n");
		return ReportWorldError(VWOBJECT_E_INVALIDTOOLMAP);
	}

	hr = m_pTools->IsValid(bstrName, &fValid);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (fValid)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateToolExt: tool already exists (%s)\n", CString(bstrName));
#endif
		hr = VWOBJECT_E_TOOLEXISTS;
		goto ERROR_ENCOUNTERED;
	}

	hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (LPVOID *)ppunk);
	if (FAILED(hr) || *ppunk == NULL)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateToolExt: tool (%s), failed to create component\n", CString(bstrName));
#endif
		hr = VWOBJECT_E_COMPONENTNOTFOUND;
		goto ERROR_ENCOUNTERED;
	}

	// make sure tool is safe for scripting
	if (ValidateObjectSafety(clsid, *ppunk, INTERFACESAFE_FOR_UNTRUSTED_CALLER) != S_OK)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateToolExt: tool (%s), component is not safe for scripting\n", CString(bstrName));
#endif
		hr = VWOBJECT_E_NOTSAFEFORSCRIPTING;
		goto ERROR_ENCOUNTERED;
	}

	hr = m_pTools->put_Property(bstrName, CComVariant(*ppunk));

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppunk);

	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::CreateTool(BSTR bstrName, BSTR bstrProgID, IUnknown** ppunk)
{
	VALIDATE_WORLD;

	CLSID clsid;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateTool\n");

	if (SUCCEEDED(CLSIDFromProgID(bstrProgID, &clsid)))
		return CreateToolExt(bstrName, clsid, ppunk);
	else
	{
#ifdef _DEBUG
		CString str(bstrProgID);
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateTool: unknown progID (%s)\n", str);
#endif
		return ReportWorldError(VWOBJECT_E_UNKNOWNPROGID);
	}
}

STDMETHODIMP CWorldObject::CreateToolFromInterface(BSTR bstrName, IUnknown* punk)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;
	VARIANT_BOOL fValid = VARIANT_FALSE;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::CreateToolFromInterface\n");

	ASSERT(bstrName);
	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateToolFromInterface: incoming argument (BSTR) is NULL\n");
		return ReportWorldError(E_INVALIDARG);
	}

	if (m_pTools == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateToolFromInterface: invalid tool map\n");
		return ReportWorldError(VWOBJECT_E_INVALIDTOOLMAP);
	}

	hr = m_pTools->IsValid(bstrName, &fValid);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (fValid)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CreateToolFromInterface: tool already exists (%s)\n", CString(bstrName));
#endif
		hr = VWOBJECT_E_TOOLEXISTS;
		goto ERROR_ENCOUNTERED;
	}

	hr = m_pTools->put_Property(bstrName, CComVariant(punk));

ERROR_ENCOUNTERED:
	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::get_Tool(BSTR bstrName, IUnknown** ppunk)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;

	hr = get_ToolExt(bstrName, ppunk);

	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::get_ToolExt(BSTR bstrName, IUnknown** ppunk)
{
	HRESULT hr = S_OK;
	CComVariant var;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::get_ToolExt\n");

	ASSERT(bstrName);
	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_ToolExt: incoming argument (BSTR) is NULL\n");
		return ReportWorldError(E_INVALIDARG);
	}

	ASSERT(ppunk);
	if (ppunk == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_ToolExt: outgoing pointer (IUnknown**) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*ppunk = NULL;

	if (m_pTools == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_ToolExt: invalid tool map\n");
		return ReportWorldError(VWOBJECT_E_INVALIDTOOLMAP);
	}

	hr = m_pTools->get_Property(bstrName, &var);
	if (FAILED(hr))
	{
		if (hr == VWOBJECT_E_PROPERTYNOTEXIST)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_ToolExt: name (%s), tool does not exist\n", CString(bstrName));
#endif
			hr = VWOBJECT_E_TOOLNOTEXIST;
		}
		goto ERROR_ENCOUNTERED;
	}

	if (var.vt != VT_UNKNOWN)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_ToolExt: name (%s), property not IUnknown (%d)\n", CString(bstrName), var.vt);
#endif

		return VWOBJECT_E_WRONGPROPERTYTYPE;
	}

	*ppunk = var.punkVal;
	SAFEADDREF(*ppunk);

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CWorldObject::IsValidTool(BSTR bstrName, VARIANT_BOOL* pboolVal)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::IsValidTool\n");

	ASSERT(bstrName);
	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::IsValidTool: incoming argument (BSTR) is NULL\n");
		return ReportWorldError(E_INVALIDARG);
	}

	ASSERT(pboolVal);
	if (pboolVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::IsValidTool: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*pboolVal = NULL;

	if (m_pTools == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::IsValidTool: invalid tool map\n");
		return ReportWorldError(VWOBJECT_E_INVALIDTOOLMAP);
	}

	hr = m_pTools->IsValid(bstrName, pboolVal);

	return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::get_Tools(IPropertyMap** pppropertymap)
{			  
	VALIDATE_WORLD;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::get_Tools\n");

	ASSERT(pppropertymap);
	if (pppropertymap == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_Tools: outgoing pointer (IPropertyMap**) is NULL\n");
		return ReportWorldError(E_POINTER);
	}

	*pppropertymap = m_pTools;
	SAFEADDREF(*pppropertymap);

	return S_OK;
}

STDMETHODIMP CWorldObject::InitializeTools(IPropertyMap* ppropertymap)
{
	HRESULT hr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::InitializeTools\n");

	if ( !m_pTools )
	{
		hr = CreatePropertyMap(m_pWorld, &m_pTools);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}
	
	// incoming pmap is optional
	if (ppropertymap)
		hr = ppropertymap->CopyTo(m_pTools);

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CWorldObject::RemoveTool(BSTR bstrName)
{
	VALIDATE_WORLD;

	HRESULT hr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::RemoveTool\n");

	ASSERT(bstrName);
	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::RemoveTool: incoming argument (BSTR) is NULL\n");
		return ReportWorldError(E_INVALIDARG);
	}

	ASSERT(m_pTools);
	if (m_pTools == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::RemoveTool: invalid tool map\n");
		return ReportWorldError(VWOBJECT_E_INVALIDTOOLMAP);
	}

	hr = m_pTools->Remove(bstrName);

	return ReportWorldError(hr);
}

// IDispatchImpl overrides

STDMETHODIMP CWorldObject::GetIDsOfNames(	REFIID riid, 
											LPOLESTR* rgszNames, 
											UINT cNames,
											LCID lcid, 
											DISPID* rgdispid)
{
	HRESULT hr;

	// first, try default thing
	hr = IDispatchImpl<IWorld, &IID_IWorld, &LIBID_VWSYSTEMLib>::GetIDsOfNames(	riid, 
																				rgszNames, 
																				cNames, 
																				lcid, 
																				rgdispid);

#ifndef DISABLEDISPATCHHACK

	// if that failed, try checking property and method lists
	if (FAILED(hr))
	{
		if (cNames == 1)
		{
			// peg
			LPOLESTR psz = rgszNames[0];

			// check if the provided name is a property or method.  If so, get
			// "doctored" dispid for it
			DISPID id;

			// is name already in map?  return id
			if (m_nameMap.Lookup(psz, id))
			{
#ifdef _DEBUG
//				TRACE("CWorldObject::GetIDsOfNames: found DISPID (%d) with name (%s)\n", id, CString(psz));
#endif

				rgdispid[0] = id;
				return S_OK;
			}
			else
			{
				// create new id
				hr = IdFromName(psz, &id);
				if (SUCCEEDED(hr))
				{
					// add to maps
					m_nameMap.SetAt(psz, id);
					m_DISPIDMap.SetAt(id, psz);

					rgdispid[0] = id;

#ifdef _DEBUG
//					TRACE("CWorldObject::GetIDsOfNames: adding new DISPID (%d) with name (%s)\n", id, CString(psz));
#endif

					return S_OK;
				}
			}
		}
	}

#endif

	return hr;
}

STDMETHODIMP CWorldObject::Invoke(	DISPID dispidMember, 
									REFIID riid,
									LCID lcid, 
									WORD wFlags, 
									DISPPARAMS* pdispparams, 
									VARIANT* pvarResult,
									EXCEPINFO* pexcepinfo, 
									UINT* puArgErr)
{

#ifndef DISABLEDISPATCHHACK

	VARIANT_BOOL bProp;

	// if the dispid is a "doctored one", handle this through the specific
	// method or property
	if (m_pGlobal != NULL && IsDoctoredId(dispidMember, &bProp))
	{
		HRESULT hr = S_OK;
		CComBSTR bstr;

		// get the name associated with the doctored id
		hr = NameFromId(dispidMember, &bstr.m_str);
		if (FAILED(hr))
			return hr;

#ifdef _DEBUG
//		TRACE("CWorldObject::Invoke: found name (%s) with DISPID (%d)\n", CString(bstr), dispidMember);
#endif

		// if it's a property, see whether we're setting or getting it
		if (bProp == VARIANT_TRUE)
		{
			// if property get, then return value
			if (wFlags & DISPATCH_PROPERTYGET)
			{
				hr = m_pGlobal->get_Property(bstr, pvarResult);
			}
			else if (wFlags & (DISPATCH_PROPERTYPUT | DISPATCH_PROPERTYPUTREF))
			{
				// should only have one argument (the value)
				if (pdispparams->cArgs != 1)
				{
					hr = DISP_E_BADPARAMCOUNT;
				}
				else
				{
					// should have exactly one named arg
					if (pdispparams->cNamedArgs != 1 ||
						pdispparams->rgdispidNamedArgs[0] != DISPID_PROPERTYPUT)
					{
						hr = DISP_E_PARAMNOTOPTIONAL;
					}
					else
					{
						// set the property
						hr = m_pGlobal->put_Property(bstr, pdispparams->rgvarg[0]);
					}
				}
			}
			else
			{
				// trying to do a method invoke on a property
				hr = DISP_E_TYPEMISMATCH;
			}
		}
		else
		{
			// it's a method, should be an invoke
			if (wFlags & DISPATCH_METHOD) 
			{
				hr = m_pGlobal->InvokeMethodExt(bstr, pdispparams, pvarResult);
			}
			else
			{
				// trying to do property get/put on a method
				hr = DISP_E_TYPEMISMATCH;
			}
		}

		return ReportThingError(hr);
	}
	else
	{

#endif

		// call base class version of this
		return IDispatchImpl<IWorld, &IID_IWorld, &LIBID_VWSYSTEMLib>::Invoke(	dispidMember, 
																				riid, 
																				lcid,
																				wFlags, 
																				pdispparams, 
																				pvarResult, 
																				pexcepinfo, 
																				puArgErr);
#ifndef DISABLEDISPATCHHACK
	}
#endif

}

// helpers for doctored dispids
BOOL CWorldObject::IsDoctoredId(DISPID id, VARIANT_BOOL* pbProp)
{
	ASSERT(pbProp);

	if (pbProp != NULL && id >= idMinDoctored && id < idMaxDoctored)
	{
		// set prop/method flag
		*pbProp = IsProperty(id) ? VARIANT_TRUE : VARIANT_FALSE;

		return TRUE;
	}

	return FALSE;
}

HRESULT CWorldObject::IdFromName(LPOLESTR psz, DISPID* pid)
{
	HRESULT hr;
	VARIANT_BOOL bValid;
	CComBSTR bstr(psz);

	ASSERT(psz);
	ASSERT(pid);

	if (psz == NULL || pid == NULL)
		return E_POINTER;

	if (m_pGlobal == NULL)
		goto ERROR_ENCOUNTERED;

	// check if a property or method
	hr = m_pGlobal->IsValidProperty(bstr, &bValid);
	if (SUCCEEDED(hr) && bValid == VARIANT_TRUE)
	{
		// set id to next available (even) doctored one
		*pid = m_idNext;

		// increment (wrap around).
		// NOTE: we don't have to be unique here.  Mostly, we're
		// doing this so that we can check to assure that OLE Scripting
		// is always calling GetIDsOfNames before calling Invoke
		m_idNext += 2;
		if ((m_idNext+1) >= idMaxDoctored)
			m_idNext = idMinDoctored;

		return S_OK;
	}
	else
	{
		hr = m_pGlobal->IsValidMethod(bstr, &bValid);
		if (SUCCEEDED(hr) && bValid == VARIANT_TRUE)
		{
			// set id to next available (odd) doctored one
			*pid = m_idNext + 1;

			// increment (wrap around).
			// NOTE: we don't have to be unique here.  Mostly, we're
			// doing this so that we can check to assure that OLE Scripting
			// is always calling GetIDsOfNames before calling Invoke
			m_idNext += 2;
			if ((m_idNext+1) >= idMaxDoctored)
				m_idNext = idMinDoctored;

			return S_OK;
		}
	}

ERROR_ENCOUNTERED:
	// didn't find it
	return DISP_E_UNKNOWNNAME;
}

HRESULT CWorldObject::NameFromId(DISPID id, BSTR* pbstr)
{
	ASSERT(pbstr);

	if (pbstr == NULL)
		return E_POINTER;

	*pbstr = NULL;

	CComBSTR bstr;

	if (!m_DISPIDMap.Lookup(id, (CBSTRAdapt<CComBSTR>&)bstr))
		return DISP_E_UNKNOWNNAME;

	*pbstr = bstr.Detach();

	return S_OK;
}

STDMETHODIMP CWorldObject::ValidateObjectSafety(REFCLSID clsid, IUnknown* punk, DWORD dwOptions)
{
	HRESULT hr = E_FAIL;
	IObjectSafety* pobjsafe = NULL;
	VARIANT_BOOL bSecurityEnabled = VARIANT_TRUE;

	VALIDATE_WORLD;

	if (punk == NULL)
		return E_INVALIDARG;

#ifdef SUPPORT_SECURITY_OVERRIDE
	/* hr = */ m_pWorld->get_EnableSecurity(&bSecurityEnabled);
#endif

	if (bSecurityEnabled == VARIANT_TRUE)
	{
		if (SUCCEEDED(punk->QueryInterface(IID_IObjectSafety, (void**)&pobjsafe)) && pobjsafe != NULL)
		{
			hr = pobjsafe->SetInterfaceSafetyOptions(IID_IDispatch, INTERFACESAFE_FOR_UNTRUSTED_CALLER, dwOptions);
				
			pobjsafe->Release();
		}

		// if IObjectSafety isn't supported, try CATID
		if (FAILED(hr))
			hr = IsCLSIDInImplCategory(clsid, CATID_SafeForScripting);
	}
	else
		hr = S_OK;

	return hr;
}
