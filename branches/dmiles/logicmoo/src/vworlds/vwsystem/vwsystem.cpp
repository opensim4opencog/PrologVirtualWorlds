// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// vwsystem.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include <resource.h>
#include "dlldatax.h"

#include "cathelp.h"
#include "objsafe.h"

//#include "corehelp.h"

// {8A1E6645-C59C-11d0-955B-00C04FD91F4D}
const CATID CATID_VWObjectProperty		= { 0x8a1e6645, 0xc59c, 0x11d0, { 0x95, 0x5b, 0x0, 0xc0, 0x4f, 0xd9, 0x1f, 0x4d } };

#define INITGUID 


#include <objbase.h>
#include <initguid.h>
#include "vbsguids.h"

#include <activscp.h>
#include <activeds.h>

// system
interface IWorld;

#include "vwsystem.h"

// vwobject
#include "comobj.h"
#include "scrptobj.h"
#include "methdobj.h"
#include "plistobj.h"
#include "pmapobj.h"
#include "thingobj.h"
#include "worldobj.h"
#include "propsecu.h"

// client
#include "vwcliobj.h"
#include "vwcliocx.h"

// server
#include "vwserve.h"

// admin
#include "vwadmobj.h"

// object repository
#include "vwodbobj.h"

// communications library
#include "commobj.h"
#include "fastobj.h"

// properties
#include "cmenuitm.h"
#include "vwocx.h"
#include "profob.h"

// tools
#include "vwanimob.h"
#include "vwanmtob.h"
#include "inetmgro.h"
#include "vwhtmlob.h"
#include "vwhelper.h"
//#include "statmgr.h"
#include "dserv.h"
#include "aobj.h"
#include "authobj.h"

// exemplars
#include "vwsysexo.h"
#include "vwthing.h"
#include "avatar.h"
#include "host.h"
#include "artifact.h"
#include "room.h"
#include "portal.h"
#include "IWportal.h"
#include "statemch.h"
#include "WebPage.h"

#define IID_DEFINED

// system
#include "vwsystem_i.c"

// core
#include "vwprop_i.c"
#include "vwobject_i.c"
#include "vwbuffer_i.c"
#include "vwcomm_i.c"
#include "vwodb_i.c"
#include "vwclient_i.c"
#include "vwserver_i.c"
#include "vwadmin_i.c"

// properties
#include "menuitem_i.c"
#include "ocxproxy_i.c"
#include "avprof_i.c"

// tools
#include "vwanim_i.c"
//#include "vwstats_i.c"
#include "vwhtml_i.c"
#include "inetfile_i.c"
#include "dirserv_i.c"
#include "authntic_i.c"

// exemplars
#include "vwsysex_i.c"

#ifdef _MERGE_PROXYSTUB
extern "C" HINSTANCE hProxyDll;
#endif


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
// core
	OBJECT_ENTRY(CLSID_ScriptModule, CScriptModuleObject)
	OBJECT_ENTRY(CLSID_COMModule, CCOMModuleObject)
	OBJECT_ENTRY(CLSID_Method, CMethodObject)
	OBJECT_ENTRY(CLSID_PropertyList, CPropertyListObject)
	OBJECT_ENTRY(CLSID_PropertyMap, CPropertyMapObject)
	OBJECT_ENTRY(CLSID_Thing, CThingObject)
	OBJECT_ENTRY(CLSID_World, CWorldObject)
	OBJECT_ENTRY(CLSID_MarshallBuffer, CMarshallBufferObject)
	OBJECT_ENTRY(CLSID_UnMarshallBuffer, CUnMarshallBufferObject)
	OBJECT_ENTRY(CLSID_PropertySecurity, CPropertySecurity)
// client
	OBJECT_ENTRY(CLSID_VWClient, CVWClientObject)
	OBJECT_ENTRY(CLSID_VWClientOCX, CVWClientOCX)
// server
	OBJECT_ENTRY(CLSID_VWServer, CVWServer)
// admin
	OBJECT_ENTRY(CLSID_VWAdmin, CVWAdminObject)
// communications library
	OBJECT_ENTRY(CLSID_VWComm, CVWCommObject)
	OBJECT_ENTRY(CLSID_FastComm, CFastCommObject)
// object repository
	OBJECT_ENTRY(CLSID_VWObjectRepository, CVWOdb)
// properties
	OBJECT_ENTRY(CLSID_MenuItem, CMenuItemObject)                        
	OBJECT_ENTRY(CLSID_OCXProxy, COCXProxyObject)                        
	OBJECT_ENTRY(CLSID_AvatarProfile, CAvatarProfileObject)
	OBJECT_ENTRY(CLSID_AvatarProfileData, CAvatarProfileDataObject)
// tools
	OBJECT_ENTRY(CLSID_VWHtml, CVWHtmlObject)                             
	OBJECT_ENTRY(CLSID_VWHtmlHelper, CVWHtmlHelperObject)                 	
	OBJECT_ENTRY(CLSID_VWAnimator, CVWAnimator)                     	
	OBJECT_ENTRY(CLSID_VWAnimation, CVWAnimation)                   	
//	OBJECT_ENTRY(CLSID_VWStatCol, CVWStatColObject)                      	
	OBJECT_ENTRY(CLSID_InternetFileManager, CInternetFileManagerObject) 	
	OBJECT_ENTRY(CLSID_DServ, CDServ)
	OBJECT_ENTRY(CLSID_Authenticate, CAuthenticate)
	OBJECT_ENTRY(CLSID_Authentication, Authentication)

// exemplars
	OBJECT_ENTRY(CLSID_SystemExemplar, CSystemExemplarObject)
	OBJECT_ENTRY(CLSID_ThingExemplar, CThingExemplarObject)
	OBJECT_ENTRY(CLSID_ArtifactExemplar, CArtifactExemplarObject)
	OBJECT_ENTRY(CLSID_AvatarExemplar, CAvatarExemplarObject)
	OBJECT_ENTRY(CLSID_HostExemplar, CHostExemplarObject)
	OBJECT_ENTRY(CLSID_PortalExemplar, CPortalExemplarObject)
	OBJECT_ENTRY(CLSID_IWPortalExemplar, CIWPortalExemplarObject)
	OBJECT_ENTRY(CLSID_RoomExemplar, CRoomExemplarObject)
	OBJECT_ENTRY(CLSID_StateMachineExemplar, CStateMachineExemplarObject)
	OBJECT_ENTRY(CLSID_WebPageExemplar, CWebPageExemplarObject)
END_OBJECT_MAP()

class CVwsystemApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
/*
private:
	static BOOL	m_bStringsLoaded;
*/
};

/*
// init the static strings
BOOL CVwsystemApp::m_bStringsLoaded	= FALSE;

void _InitStrings(void);
*/

CVwsystemApp theApp;

BOOL CVwsystemApp::InitInstance()
{
#ifdef _MERGE_PROXYSTUB
	hProxyDll = m_hInstance;
#endif
	_Module.Init(ObjectMap, m_hInstance);

/*
	// Loads in the strings from the resource file...
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if (!m_bStringsLoaded)
	{
		_InitStrings();
		m_bStringsLoaded = TRUE;
	}
*/

	AfxEnableControlContainer();
	return CWinApp::InitInstance();
}

int CVwsystemApp::ExitInstance()
{
	_Module.Term();
	return CWinApp::ExitInstance();
}



/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
#ifdef _MERGE_PROXYSTUB
	if (PrxDllCanUnloadNow() != S_OK)
		return S_FALSE;
#endif
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (AfxDllCanUnloadNow()==S_OK && _Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
#ifdef _MERGE_PROXYSTUB
	if (PrxDllGetClassObject(rclsid, riid, ppv) == S_OK)
		return S_OK;
#endif
	return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	HRESULT hr = S_OK;

	hr = CreateComponentCategory(CATID_VWObjectProperty, L"V-Worlds Object Properties");

	if (SUCCEEDED(hr))
	{
		// register properties
		RegisterCLSIDInCategory(CLSID_PropertyList, CATID_VWObjectProperty);
		RegisterCLSIDInCategory(CLSID_PropertyMap, CATID_VWObjectProperty);
		RegisterCLSIDInCategory(CLSID_MenuItem, CATID_VWObjectProperty);
		RegisterCLSIDInCategory(CLSID_OCXProxy, CATID_VWObjectProperty);
		RegisterCLSIDInCategory(CLSID_AvatarProfile, CATID_VWObjectProperty);
		RegisterCLSIDInCategory(CLSID_AvatarProfileData, CATID_VWObjectProperty);
	}

	// registers object, typelib and all interfaces in typelib
	hr = _Module.RegisterServer(TRUE);
	if (FAILED(hr))
		return hr;

#ifdef _MERGE_PROXYSTUB
	hr = PrxDllRegisterServer();
#endif

	return hr;
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	// unregister properties
	UnRegisterCLSIDInCategory(CLSID_PropertyList, CATID_VWObjectProperty);
	UnRegisterCLSIDInCategory(CLSID_PropertyMap, CATID_VWObjectProperty);
	UnRegisterCLSIDInCategory(CLSID_MenuItem, CATID_VWObjectProperty);
	UnRegisterCLSIDInCategory(CLSID_OCXProxy, CATID_VWObjectProperty);
	UnRegisterCLSIDInCategory(CLSID_AvatarProfile, CATID_VWObjectProperty);
	UnRegisterCLSIDInCategory(CLSID_AvatarProfileData, CATID_VWObjectProperty);

#ifdef _MERGE_PROXYSTUB
	PrxDllUnregisterServer();
#endif
	_Module.UnregisterServer();
	return S_OK;
}

/*
void	_InitStrings(void)
{
	CString strRESOURCE;

	// EXEMPLAR NAMES
	if (!strRESOURCE.LoadString(IDS_EXEMP_THING))
	{
		DWORD	dwErr;

		dwErr = GetLastError();

		TRACE1("Err:  %d\n", dwErr);
	}

	(kbstrThingEx) = strRESOURCE;

	DEFINE_STRING_RESOURCE(IDS_EXEMP_ROOM, kbstrRoomEx);
	DEFINE_STRING_RESOURCE(IDS_EXEMP_ARTIFACT, kbstrArtifactEx);
	DEFINE_STRING_RESOURCE(IDS_EXEMP_AVATAR, kbstrAvatarEx);
	DEFINE_STRING_RESOURCE(IDS_EXEMP_PORTAL, kbstrPortalEx);

	// Methods
	// Thing exemplar
	DEFINE_STRING_RESOURCE(IDS_METHOD_ALLOWENTRY, kbstrMethodAllowEntry);
	DEFINE_STRING_RESOURCE(IDS_METHOD_ALLOWEXIT, kbstrMethodAllowExit);
	DEFINE_STRING_RESOURCE(IDS_METHOD_ACCEPTENTRY, kbstrMethodAcceptEntry);
	DEFINE_STRING_RESOURCE(IDS_METHOD_ACCEPTEXIT, kbstrMethodAcceptExit);
	DEFINE_STRING_RESOURCE(IDS_METHOD_MOVEINTO, kbstrMethodMoveInto);
	DEFINE_STRING_RESOURCE(IDS_METHOD_GOHOME, kbstrMethodGoHome);

	DEFINE_STRING_RESOURCE(IDS_METHOD_CLONE, kbstrMethodClone);
	DEFINE_STRING_RESOURCE(IDS_METHOD_DESTROY, kbstrMethodDestroy);

	DEFINE_STRING_RESOURCE(IDS_METHOD_SAY, kbstrMethodSay);
	DEFINE_STRING_RESOURCE(IDS_METHOD_SHOUT, kbstrMethodShout);
	DEFINE_STRING_RESOURCE(IDS_METHOD_WHISPER, kbstrMethodWhisper);
	DEFINE_STRING_RESOURCE(IDS_METHOD_ANNOUNCE, kbstrMethodAnnounce);
	DEFINE_STRING_RESOURCE(IDS_METHOD_REPORT, kbstrMethodReport);
	DEFINE_STRING_RESOURCE(IDS_METHOD_EMOTE, kbstrMethodEmote);

	DEFINE_STRING_RESOURCE(IDS_METHOD_GETDESCRIPTION, kbstrMethodGetDescription);
	DEFINE_STRING_RESOURCE(IDS_METHOD_LOOKAT, kbstrMethodLookAt);

	DEFINE_STRING_RESOURCE(IDS_METHOD_ONTELL, kbstrMethodOnTell);
	DEFINE_STRING_RESOURCE(IDS_METHOD_ONPEERTELL, kbstrMethodOnPeerTell);
	DEFINE_STRING_RESOURCE(IDS_METHOD_ONCONTAINERTELL, kbstrMethodOnContainerTell);
	DEFINE_STRING_RESOURCE(IDS_METHOD_ONCONTENTTELL, kbstrMethodOnContentTell);

	DEFINE_STRING_RESOURCE(IDS_METHOD_ADDCLIENTPROPERTY, kbstrMethodAddClientProperty);
	DEFINE_STRING_RESOURCE(IDS_METHOD_CLIENTPROPERTY, kbstrMethodClientProperty);
	DEFINE_STRING_RESOURCE(IDS_METHOD_REMOVECLIENTPROPERTY, kbstrMethodRemoveClientProperty);

	// Artifact exemplar

	// Avatar exemplar
	DEFINE_STRING_RESOURCE(IDS_METHOD_WIELD, kbstrMethodWield);
	DEFINE_STRING_RESOURCE(IDS_METHOD_UNWIELD, kbstrMethodUnWield);

	// Portal exemplar

	// Room exemplar

	// Properties
	// Thing exemplar
	DEFINE_STRING_RESOURCE(IDS_PROP_ISMANIFEST, kbstrPropIsManifest);
	DEFINE_STRING_RESOURCE(IDS_PROP_ISANCHORED, kbstrPropIsAnchored);

	DEFINE_STRING_RESOURCE(IDS_PROP_HOME, kbstrPropHome);
	DEFINE_STRING_RESOURCE(IDS_PROP_SPECURL, kbstrPropSpecURL);

	// Avatar exemplar

	// Room exemplar
	
	// Portal exemplar
}
*/
