// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// vwobject.cpp : Implementation of DLL Exports.

// You will need the NT SUR Beta 2 SDK or VC 4.2 in order to build this 
// project.  This is because you will need MIDL 3.00.15 or higher and new
// headers and libs.  If you have VC 4.2 installed, then everything should
// already be configured correctly.

// Note: Proxy/Stub Information
//		To build a separate proxy/stub DLL, 
//		run nmake -f vwobjectps.mak in the project directory.

#include "stdafx.h"
#include "resource.h"
#include "initguid.h"
#include "vwobject.h"

#include <activscp.h>
#include "vbsguids.h"

#include "COMObj.h"
#include "ScrptObj.h"
#include "MethdObj.h"
#include "PListObj.h"
#include "PMapObj.h"
#include "ThingObj.h"
#include "WorldObj.h"
#include "MarshBuf.h"
#include "propsecu.h"

#define IID_DEFINED
#include "vwobject_i.c"
#include "vwbuffer_i.c"

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
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
END_OBJECT_MAP()

class CvwobjectApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
};

CvwobjectApp theApp;

BOOL CvwobjectApp::InitInstance()
{
	_Module.Init(ObjectMap, m_hInstance);
	return CWinApp::InitInstance();
}

int CvwobjectApp::ExitInstance()
{
	_Module.Term();
	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (AfxDllCanUnloadNow()==S_OK && _Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	// registers object, typelib and all interfaces in typelib
	return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	_Module.UnregisterServer();
	return S_OK;
}

