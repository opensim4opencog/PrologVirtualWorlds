// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWHtml.cpp : Implementation of DLL Exports.

// You will need the NT SUR Beta 2 SDK or VC 4.2 in order to build this 
// project.  This is because you will need MIDL 3.00.15 or higher and new
// headers and libs.  If you have VC 4.2 installed, then everything should
// already be configured correctly.

// Note: Proxy/Stub Information
//		To build a separate proxy/stub DLL, 
//		run nmake -f VWHtmlps.mak in the project directory.

#include "stdafx.h"
#include "resource.h"
#include "initguid.h"
#include "VWHtml.h"
#include "VWHtmlOb.h"
#include "VWHelper.h"

#include "cathelp.h"

#define IID_DEFINED
#include "VWHtml_i.c"

const CATID CATID_SafeForScripting		= {0x7dd95801,0x9882,0x11cf,{0x9f,0xa9,0x00,0xaa,0x00,0x6c,0x42,0xc4}};

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_VWHtml, CVWHtmlObject)
	OBJECT_ENTRY(CLSID_VWHtmlHelper, CVWHtmlHelperObject)
END_OBJECT_MAP()

class CVWHtmlApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
};

CVWHtmlApp theApp;

BOOL CVWHtmlApp::InitInstance()
{
	_Module.Init(ObjectMap, m_hInstance);
	AfxEnableControlContainer();
	return CWinApp::InitInstance();
}

int CVWHtmlApp::ExitInstance()
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
	// register CLSID's as being safe for scripting
	if (SUCCEEDED(CreateComponentCategory(CATID_SafeForScripting, CComBSTR("Controls that are safely scriptable"))))
	{
		RegisterCLSIDInCategory(CLSID_VWHtml, CATID_SafeForScripting);
		RegisterCLSIDInCategory(CLSID_VWHtmlHelper, CATID_SafeForScripting);
	}

	// registers object, typelib and all interfaces in typelib
	return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	UnRegisterCLSIDInCategory(CLSID_VWHtml, CATID_SafeForScripting);
	UnRegisterCLSIDInCategory(CLSID_VWHtmlHelper, CATID_SafeForScripting);
	_Module.UnregisterServer();
	return S_OK;
}

