// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// vwclient.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"
#include "..\vwserver\wldlist.h"
#include "initguid.h"

#include "vwclient.h"
#include "VWCliObj.h"

#define IID_DEFINED
#include "vwobject_i.c"
#include "vwbuffer_i.c"
#include "vwclient_i.c"
#include "vwcomm_i.c"

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_VWClient, CVWClientObject)
END_OBJECT_MAP()

class CvwclientApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
};

CvwclientApp theApp;

BOOL CvwclientApp::InitInstance()
{
	_Module.Init(ObjectMap, m_hInstance);
	return CWinApp::InitInstance();
}

int CvwclientApp::ExitInstance()
{
	_Module.Term();
	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (AfxDllCanUnloadNow() && _Module.GetLockCount()==0) ? S_OK : S_FALSE;
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
	HRESULT hRes = S_OK;

	// registers object, typelib and all interfaces in typelib
	hRes = _Module.RegisterServer(TRUE);
	return hRes;
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Adds entries to the system registry

STDAPI DllUnregisterServer(void)
{
	HRESULT hRes = S_OK;

	_Module.UnregisterServer();
	return hRes;
}

#ifdef _DEBUG
void CrtSetBreakAlloc(long l)
{
	_CrtSetBreakAlloc(l);
}
#endif