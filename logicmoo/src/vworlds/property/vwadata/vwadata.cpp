// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWAData.cpp : Implementation of DLL Exports.

// Note: Proxy/Stub Information
//		To build a separate proxy/stub DLL, 
//		run nmake -f VWADataps.mak in the project directory.

#include "stdafx.h"
#include "resource.h"
#include "initguid.h"
#include "VWAData.h"
#include "VWADataO.h"

#define IID_DEFINED
#include "VWAData_i.c"
#include "vwobject_i.c"


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_VWAccessoryData, CVWAccessoryDataObject)
END_OBJECT_MAP()

class CVWADataApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
};

CVWADataApp theApp;

BOOL CVWADataApp::InitInstance()
{
	_Module.Init(ObjectMap, m_hInstance);
	return CWinApp::InitInstance();
}

int CVWADataApp::ExitInstance()
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

