// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// vwstudio.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include <resource.h>

#include "cathelp.h"

GUID CATID_SafeForScripting		= {0x7dd95801,0x9882,0x11cf,{0x9f,0xa9,0x00,0xaa,0x00,0x6c,0x42,0xc4}};

// {8A1E6645-C59C-11d0-955B-00C04FD91F4D}
GUID CATID_VWObjectProperty		= { 0x8a1e6645, 0xc59c, 0x11d0, { 0x95, 0x5b, 0x0, 0xc0, 0x4f, 0xd9, 0x1f, 0x4d } };

#define INITGUID 
#include <objbase.h>
#include <initguid.h>
#include "vbsguids.h"

// system
#include "vwstudio.h"
#include "vwsystem.h"

// d3d
#include <d3drmobj.h>

// stdtools
//#include "CutTool.h"
//#include "CopyTool.h"
#include "VWErrTl.h"
#include "trans2d.h"
#include "select.h"
#include "undostak.h"
#include "CtrlMgr.h"
#include "Rotat2D.h"
#include "Scale2D.h"
#include "undoitem.h"
#include "PolyTool.h"
#include "ZoomTool.h"

// geomtool
#include "trans3D.h"
#include "RotTool.h"
#include "ScaleTl.h"
#include "GeomUndo.h"

// bndytool
#include "bndytool.h"

// popupmgr
//#include "PopEdMgr.h"

// webtools
#include "WWHelper.h"
#include "RWHelper.h"

// popup editors

// modules
#include "vwstuexo.h"

#define IID_DEFINED
#include <vwsystem_i.c>
#include <vwprop_i.c>
#include <vwobject_i.c>
#include <cellprop_i.c>
#include <vector_i.c>
#include <vwtrans_i.c>
#include <vwframe_i.c>

#include <vwstudio_i.c>
#include <vwuiobjs_i.c>

#include <PickData_i.c>

// stdtools
#include <stdtools_i.c>

// geomtool
#include <geomtool_i.c>

// bndytool
#include <bndytool_i.c>

// popupmgr
//#include <popup_i.c>
//#include <popupmgr_i.c>

// webtools
#include <webtools_i.c>

// popup editors

#include "FileAcTl.h"

// modules
#include "vwstuex_i.c"

// Proxy/stub marshalling
#include "dlldatax.h"

#ifdef _MERGE_PROXYSTUB
extern "C" HINSTANCE hProxyDll;
#endif

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
// stdtools
	OBJECT_ENTRY(CLSID_VWTranslate2DTool, CVWTranslate2DToolObject)
	OBJECT_ENTRY(CLSID_VWSelectTool, CVWSelectToolObject)
	OBJECT_ENTRY(CLSID_VWUndoStacks, CVWUndoStacksObject)
	OBJECT_ENTRY(CLSID_VWErrorTool, CVWErrorToolObject)
	OBJECT_ENTRY(CLSID_VWControlManager, CControlMgrObject)
	OBJECT_ENTRY(CLSID_VWUndoItem, CVWUndoItem)
	OBJECT_ENTRY(CLSID_VWRotate2DTool, CRotate2DTool)
	OBJECT_ENTRY(CLSID_VWScale2DTool, CVWScale2DTool)
	OBJECT_ENTRY(CLSID_PolyTool, CPolyTool)
	OBJECT_ENTRY(CLSID_VWZoomTool, CVWZoomTool)
// geomtool
	OBJECT_ENTRY(CLSID_VWTranslate3DTool, CVWTranslate3DTool)
	OBJECT_ENTRY(CLSID_Rotate3DTool, CRotate3DTool)
	OBJECT_ENTRY(CLSID_VWScale3DTool, CVWScale3DTool)
	OBJECT_ENTRY(CLSID_VWExecuteGeomUndo, CVWExecuteGeomUndo)
// webtools
	OBJECT_ENTRY(CLSID_WorldWizHelper, CWorldWizHelper)
	OBJECT_ENTRY(CLSID_RoomWizHelper, CRoomWizHelper)
	OBJECT_ENTRY(CLSID_VWFileAccessTool, CVWFileAccessTool)
// popup editors
// exemplars
	OBJECT_ENTRY(CLSID_StudioExemplar, CStudioExemplarObject)
END_OBJECT_MAP()

class CVwstudioApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
};

CVwstudioApp theApp;

BOOL CVwstudioApp::InitInstance()
{
#ifdef _MERGE_PROXYSTUB
	hProxyDll = m_hInstance;
#endif
	_Module.Init(ObjectMap, m_hInstance);
	AfxEnableControlContainer();
	return CWinApp::InitInstance();
}

int CVwstudioApp::ExitInstance()
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

//	hr = CreateComponentCategory(CATID_SafeForScripting, CComBSTR("Controls that are safely scriptable"));
	hr = CreateComponentCategory(CATID_VWObjectProperty, CComBSTR("V-Worlds Object Properties"));

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
	HRESULT hRes = S_OK;

#ifdef _MERGE_PROXYSTUB
	PrxDllUnregisterServer();
#endif
	_Module.UnregisterServer();
	return S_OK;
}


