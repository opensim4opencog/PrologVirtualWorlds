// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// d2d.cpp : Implementation of DLL Exports.

// You will need the NT SUR Beta 2 SDK or VC 4.2 in order to build this 
// project.  This is because you will need MIDL 3.00.15 or higher and new
// headers and libs.  If you have VC 4.2 installed, then everything should
// already be configured correctly.

// Note: Proxy/Stub Information
//		To build a separate proxy/stub DLL, 
//		run nmake -f d2dps.mak in the project directory.


#include <afxwin.h>
#include <afxdisp.h>

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

#include "resource.h"
#include "initguid.h"
#define ERRMGR_NAME g_emD2D;
#include "MMImage.h"
#include "D2DTrans.h"
#include <d3drm.h>
#include "d2d.h"
#include "DispList.h"
#include "d2dprv.h"
#include "D2DRM.h"
#include "D2DRMArr.h"
#include "D2DRMDev.h"
#include "D2DRMVP.h"
#include "D2DRMFrm.h"
#include "D2DRMImg.h"
#include "D2DRMHSp.h"
#include "D2DRMPkA.h"
#include "D2DRMAnm.h"

#define IID_DEFINED
#include "d2dprv_i.c"
#include "d2d_i.c"

ErrMgr g_emD2D;

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_CDirect2DRM, CDirect2DRM)
	OBJECT_ENTRY(CLSID_CDirect2DRMArray, CDirect2DRMArray)
	OBJECT_ENTRY(CLSID_CDirect2DRMDevice, CDirect2DRMDevice)
	OBJECT_ENTRY(CLSID_CDirect2DRMViewport, CDirect2DRMViewport)
	OBJECT_ENTRY(CLSID_CDirect2DRMFrame, CDirect2DRMFrame)
	OBJECT_ENTRY(CLSID_CDirect2DRMImage, CDirect2DRMImage)
	OBJECT_ENTRY(CLSID_CDirect2DRMHotSpot, CDirect2DRMHotSpot)
	OBJECT_ENTRY(CLSID_CDirect2DRMPick, CDirect2DRMPick)
	OBJECT_ENTRY(CLSID_CDirect2DRMAnimation, CDirect2DRMAnimation)
END_OBJECT_MAP()

class Cd2dApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
};

Cd2dApp theApp;

BOOL Cd2dApp::InitInstance()
{
	_Module.Init(ObjectMap, m_hInstance);
	return CWinApp::InitInstance();
}

int Cd2dApp::ExitInstance()
{
	_Module.Term();
	int nTmp = CWinApp::ExitInstance();
	return nTmp;
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (AfxDllCanUnloadNow() == S_OK) {
		if (_Module.GetLockCount()==0) {
			return S_OK;
		}
	}
	return S_FALSE;
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
