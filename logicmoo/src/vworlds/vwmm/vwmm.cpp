// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// vwmm.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include <resource.h>
#include "dlldatax.h"  // for proxy/stub marshalling

#include "geom2d.h"

#include "cathelp.h"

// {8A1E6645-C59C-11d0-955B-00C04FD91F4D}
const CATID CATID_VWObjectProperty		= { 0x8a1e6645, 0xc59c, 0x11d0, { 0x95, 0x5b, 0x0, 0xc0, 0x4f, 0xd9, 0x1f, 0x4d } };

#define INITGUID 
#include <objbase.h>
#include <initguid.h>
#include "vbsguids.h"

// d2d/d3d
#include <d2d.h>
#include <d3drmwin.h>

// system
interface IWorld;

#include "vwmm.h"
#include "vwsystem.h"

#include "point23.h"

// graphics
#include "VWFrameO.h"
#include "VWGCachO.h"
#include "VWRend3D.h"
#include "VWRend2D.h"
#include "VWRendRt.h"
#include "spritemo.h"

// graphics converters
#ifndef NO_VRML_CONVERTER
#include "convvrml.h"
#endif
#ifndef NO_3DS_CONVERTER
#include "conv3ds.h"
#endif

// properties
//#include "cell.h"
#include "edgecyc.h"
#include "vectorob.h"
#include "vwgeomob.h"
#include "vwgeoms.h"
#include "vwidatao.h"
#include "vwgeoma.h"
#include "jointobj.h"
#include "charobj.h"
#include "PuppetOb.h"
#include "vwadatao.h"
#include "Pickdato.h"
#include "sound.h"

// ui tools
#include "VWNavT3D.h"
#include "VWNavT2D.h"
#include "VWPkTl.h"

// tools
//#include "vwbufo.h"
//#include "vw3dbufo.h"
//#include "vwsoundo.h"

// modules
#include "vwmmexo.h"

#define IID_DEFINED

// system (vwsystem)
#include "vwsystem_i.c"

// core (vwsystem)
#include "vwprop_i.c"
#include "vwobject_i.c"

// properties (vwsystem)
#include "menuitem_i.c"
#include "ocxproxy_i.c"

// tools (vwsystem)
#include "inetfile_i.c"

// system
#include "vwmm_i.c"

// properties
#include "cellprop_i.c"
#include "vector_i.c"
#include "vwgeom_i.c"
#include "vwgeomsp_i.c"
#include "vwidata_i.c"
#include "vwtrans_i.c"
#include "vwgeomar_i.c"
#include "vwadata_i.c"
#include "avprof_i.c"
#include "Pickdata_i.c"
#include "isound_i.c"

// graphics
#include "VWFrame_i.c"
#include "VWGCache_i.c"
#include "d2d_i.c"
#include "VWRender_i.c"

// graphics converters
#include "convgeom_i.c"
#ifndef NO_3DS_CONVERTER
#include "3dstod3d_i.c"
#endif
#ifndef NO_VRML_CONVERTER
#include "vrml2d3d_i.c"
#endif

// ui tools
#include "VWNvTool_i.c"
#include "vwpktool_i.c"

// tools
//#include "vwsound_i.c"
#include "spritemn_i.c"

// modules
#include "vwmmex_i.c"

#ifdef _MERGE_PROXYSTUB
extern "C" HINSTANCE hProxyDll;
#endif

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
// properties
	OBJECT_ENTRY(CLSID_Vector, CVectorObject)                        
	OBJECT_ENTRY(CLSID_VWGeometry, CVWGeometryObject)          	
	OBJECT_ENTRY(CLSID_VWGeometrySprite, CVWGeometrySprite)      	
	OBJECT_ENTRY(CLSID_Boundary, CBoundary)                    	
//	OBJECT_ENTRY(CLSID_BoundaryBuilder, CBoundaryBuilder)      	
//	OBJECT_ENTRY(CLSID_BoundaryList, CBoundaryList)                              	
	OBJECT_ENTRY(CLSID_VWIntersectData, CVWIntersectDataObject)	
	OBJECT_ENTRY(CLSID_VWGeometryArticulated, CVWGeometryArticulated)
	OBJECT_ENTRY(CLSID_Character, CCharacter)
	OBJECT_ENTRY(CLSID_Joint, CJoint)
	OBJECT_ENTRY(CLSID_Puppet, CPuppet)
	OBJECT_ENTRY(CLSID_VWAccessoryData, CVWAccessoryDataObject)	
	OBJECT_ENTRY(CLSID_VWPickData, CVWPickDataObject)	
	OBJECT_ENTRY(CLSID_Sound, CSoundObject)	

// tools
//	OBJECT_ENTRY(CLSID_VWSound, CVWSoundObject)                           	
//	OBJECT_ENTRY(CLSID_SoundBuffer, CSoundBufferObject)                   	
//	OBJECT_ENTRY(CLSID_Sound3DBuffer, CSound3DBufferObject)               	
// graphics
	OBJECT_ENTRY(CLSID_VWFrame, CVWFrameObject)
	OBJECT_ENTRY(CLSID_VWGeometryCache, CVWGeometryCacheObject)
	OBJECT_ENTRY(CLSID_VWRenderRoot, CVWRenderRoot)
	OBJECT_ENTRY(CLSID_VWRender3D, CVWRender3D)
	OBJECT_ENTRY(CLSID_VWRender2D, CVWRender2D)
	OBJECT_ENTRY(CLSID_SpriteManagerObject, CSpriteManagerObject)
// geometry converters
#ifndef NO_3DS_CONVERTER
	OBJECT_ENTRY(CLSID_Convert3DSGeometry, CConvert3DSGeometry)
#endif
#ifndef NO_VRML_CONVERTER
	OBJECT_ENTRY(CLSID_ConvertVRMLGeometry, CConvertVRMLGeometry)
#endif
// ui tools
	OBJECT_ENTRY(CLSID_VWNavigationTool3D, CVWNavigationTool3D)
	// REVIEW: removed since they're just skeleton implementations
//	OBJECT_ENTRY(CLSID_VWNavigationTool2D, CVWNavigationTool2D)
//	OBJECT_ENTRY(CLSID_VWNavigationTool2D3D, CVWNavigationTool3D)
	OBJECT_ENTRY(CLSID_VWPickTool, CVWPickTool)
// modules
	OBJECT_ENTRY(CLSID_MultimediaExemplar, CMultimediaExemplarObject)
END_OBJECT_MAP()

class CVwmmApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
};

CVwmmApp theApp;

BOOL CVwmmApp::InitInstance()
{
#ifdef _MERGE_PROXYSTUB
	hProxyDll = m_hInstance;
#endif
	_Module.Init(ObjectMap, m_hInstance);
	AfxEnableControlContainer();
	return CWinApp::InitInstance();
}

int CVwmmApp::ExitInstance()
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
	HRESULT hr = CreateComponentCategory(CATID_VWObjectProperty, L"V-Worlds Object Properties");

	if (SUCCEEDED(hr))
	{
		// register properties
		RegisterCLSIDInCategory(CLSID_Vector, CATID_VWObjectProperty);
		RegisterCLSIDInCategory(CLSID_VWGeometry, CATID_VWObjectProperty);
		RegisterCLSIDInCategory(CLSID_VWGeometrySprite, CATID_VWObjectProperty);
		RegisterCLSIDInCategory(CLSID_Boundary, CATID_VWObjectProperty);
//		RegisterCLSIDInCategory(CLSID_BoundaryList, CATID_VWObjectProperty);
		RegisterCLSIDInCategory(CLSID_VWGeometryArticulated, CATID_VWObjectProperty);
		RegisterCLSIDInCategory(CLSID_Sound, CATID_VWObjectProperty);

	}


	// registers object, typelib and all interfaces in typelib
	hr =  _Module.RegisterServer(TRUE);
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
	UnRegisterCLSIDInCategory(CLSID_Vector, CATID_VWObjectProperty);
	UnRegisterCLSIDInCategory(CLSID_VWGeometry, CATID_VWObjectProperty);
	UnRegisterCLSIDInCategory(CLSID_VWGeometrySprite, CATID_VWObjectProperty);
	UnRegisterCLSIDInCategory(CLSID_Boundary, CATID_VWObjectProperty);
//	UnRegisterCLSIDInCategory(CLSID_BoundaryList, CATID_VWObjectProperty);
	UnRegisterCLSIDInCategory(CLSID_VWGeometryArticulated, CATID_VWObjectProperty);
	UnRegisterCLSIDInCategory(CLSID_Sound, CATID_VWObjectProperty);


#ifdef _MERGE_PROXYSTUB
	PrxDllUnregisterServer();
#endif
	_Module.UnregisterServer();
	return S_OK;
}


