// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// uictrl.cpp : Implementation of CUictrlApp and DLL registration.

#include "stdafx.h"
#include "uictrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CComModule _Module;

CUictrlApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0xdc9ca0bd, 0x336f, 0x11d1, { 0xbb, 0x22, 0, 0xc0, 0x4f, 0xc2, 0xca, 0xb1 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

#define INITGUID                   // define all the guids.
#include <initguid.h>
#include <activscp.h>
#include <vbsguids.h>

#include <vwobject.h>

#include <inetfile.h>
#include <inetfile_i.c>

#include <menuitem.h>
#include <menuitem_i.c>

#include <vector.h>
#include <vector_i.c>

////////////////////////////////////////////////////////////////////////////
// CUictrlApp::InitInstance - DLL initialization

BOOL CUictrlApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}


////////////////////////////////////////////////////////////////////////////
// CUictrlApp::ExitInstance - DLL termination

int CUictrlApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}


/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
