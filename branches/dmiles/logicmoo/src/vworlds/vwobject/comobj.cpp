// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// COMObj.cpp : Implementation of CfooApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include "comobj.h"
#include "worldobj.h"
#include "cathelp.h"

IMPLEMENT_OBJECTPROPERTY(CCOMModuleObject)

//#define COM_PERF_DEBUG

#define QUIET_INVOKE VARIANT_TRUE
//#define QUIET_INVOKE VARIANT_FALSE

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CCOMModuleObject::Dump(void)
{
	CString strCLSID = VWStringFromCLSID(m_ModuleCLSID);

	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "COMModule: begin\n");

	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "COMModule: m_ModuleCLSID %s\n", strCLSID);
	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "COMModule: m_bstrModuleName %s\n", CString(m_bstrModuleName));
	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "COMModule: server-side (%s), client-side (%s)\n",
		(m_lFlags & MODULE_SERVER) ? "true" : "false",
		(m_lFlags & MODULE_CLIENT) ? "true" : "false");

	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "COMModule: end\n");

	return S_OK;
}

STDMETHODIMP CCOMModuleObject::Reload(VARIANT_BOOL bReinstall)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CCOMModuleObject::Reload: name (%s)\n", CString(m_bstrModuleName));
#endif

	hr = m_pWorld->CheckPolicyExt(NULL, CP_WORLDOWNER | CP_WIZARD, CComVariant());
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CCOMModuleObject::Reload: must be world owner to edit modules\n");
		goto ERROR_ENCOUNTERED;
	}

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (m_bShutdown == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CCOMModuleObject::Reload: module is shutdown, can't reload\n");
		hr = VWOBJECT_E_MODULESHUTDOWN;
		goto ERROR_ENCOUNTERED;
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		/* hr = */ Unload();

		hr = LoadObject();
		if (FAILED(hr))
		{
			/* hr = */ Unload(/* VARIANT_TRUE */);
			goto ERROR_ENCOUNTERED;
		}

		if (bReinstall == VARIANT_TRUE)
		{
			VARIANT_BOOL bServerSide = VARIANT_FALSE;

			/* hr = */ m_pWorld->get_ServerSide(&bServerSide);

			// only reinstall from server-side
			if (bServerSide == VARIANT_TRUE)
			{
				hr = Install();
				if (FAILED(hr))
				{
					/* hr = */ Unload(/* VARIANT_TRUE */);
					goto ERROR_ENCOUNTERED;
				}
			}
		}
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BOOL;
		dispparams.rgvarg[0].boolVal = bReinstall;

		hr = SendModuleCommand(DISPID_MODULE_RELOAD, DISPATCH_METHOD, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	return ReportCOMModuleError(hr);
}

HRESULT CCOMModuleObject::LoadObject()
{
	HRESULT hr = S_OK;
	IVWDispatch* pvwdisp = NULL;

#ifdef _DEBUG
	CString strCLSID = VWStringFromCLSID(m_ModuleCLSID);
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CCOMModuleObject::LoadObject: name (%s), clsid %s\n", CString(m_bstrModuleName), strCLSID);
#endif

	Unload();

#ifdef COM_PERF_DEBUG
	DWORD dwTime = GetTickCount();
#endif

	hr = CoCreateInstance(m_ModuleCLSID, NULL, CLSCTX_INPROC_SERVER, IID_IDispatch, (LPVOID *)&m_pdisp);
	if (FAILED(hr) || m_pdisp == NULL)
	{
		hr = VWOBJECT_E_COMPONENTNOTFOUND;

		VWREPORT(m_pWorld, VWR_ERROR, "Failed to load COM module (%s), clsid %s, hr %x\n", CString(m_bstrModuleName), VWStringFromCLSID(m_ModuleCLSID), hr);

		goto ERROR_ENCOUNTERED;
	}

	// make sure COM module is safe for scripting
	if (m_pWorld->ValidateObjectSafety(m_ModuleCLSID, m_pdisp, INTERFACESAFE_FOR_UNTRUSTED_CALLER) != S_OK)
	{
		hr = VWOBJECT_E_NOTSAFEFORSCRIPTING;

		VWREPORT(m_pWorld, VWR_ERROR, "COM module (%s) not safe for scripting, clsid %s, hr %x\n", CString(m_bstrModuleName), VWStringFromCLSID(m_ModuleCLSID), hr);

		goto ERROR_ENCOUNTERED;
	}

	// if it's a VWorld COM module, initialize it with IWorld*
	if (SUCCEEDED(m_pdisp->QueryInterface(IID_IVWDispatch, (LPVOID*)&pvwdisp)))
	{
		pvwdisp->Initialize(m_pWorld);

		SAFERELEASE(pvwdisp);
	}

#ifdef COM_PERF_DEBUG
	{
		DWORD dwNow = GetTickCount();
		CString strCLSID = VWStringFromCLSID(m_ModuleCLSID);

		VWREPORT(m_pWorld, VWR_PERFLOG, "Load COM,%d,%d,%s,%s\n",
			dwNow, dwNow - dwTime, CString(m_bstrModuleName), strCLSID);
	}
#endif

	hr = InvokeOnLoad();
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CCOMModuleObject::LoadObject: invocation of OnLoad failed, hr %x\n", hr);
		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(m_pdisp);

	return hr;
}

STDMETHODIMP CCOMModuleObject::Unload(VARIANT_BOOL bShutdown)
{
	HRESULT hr = S_OK;

	if (m_bShutdown == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CCOMModuleObject::Unload: module is shutdown, can't unload\n");
		hr = VWOBJECT_E_MODULESHUTDOWN;
		goto ERROR_ENCOUNTERED;
	}

	if (m_pdisp)
	{
		hr = InvokeOnUnload();
		if (FAILED(hr))
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CCOMModuleObject::Unload: invocation of OnUnload failed, hr %x\n", hr);

		SAFERELEASE(m_pdisp);
	}

	m_bShutdown = bShutdown;

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CCOMModuleObject::Validate(VARIANT_BOOL* pboolVal)
{
	HRESULT hr = S_OK;

	ASSERT(pboolVal);
	if (pboolVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CCOMModuleObject::Validate: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return ReportCOMModuleError(E_POINTER);
	}

	*pboolVal = VARIANT_TRUE;

	// REVIEW: what about the remoting of Reload?
//	hr = Reload();

	Unload();

	hr = LoadObject();

	if (FAILED(hr))
		*pboolVal = VARIANT_FALSE;

	Unload();

	return ReportCOMModuleError(hr);
}

#if 0
STDMETHODIMP CCOMModuleObject::put_ModuleName(BSTR bstrModuleName)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CCOMModuleObject::put_ModuleName\n"); 
#endif
		
	ASSERT(bstrModuleName);
	if (bstrModuleName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CCOMModuleObject::put_ModuleName: incoming argument (BSTR) is NULL\n");
		return ReportCOMModuleError(E_INVALIDARG);
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CCOMModuleObject::put_ModuleName: name (%s)\n", 
		CString(bstrModuleName));
#endif

	hr = m_pWorld->CheckPolicyExt(NULL, CP_WORLDOWNER | CP_WIZARD, CComVariant());
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CCOMModuleObject::put_ModuleName: must be world owner to edit modules\n");
		goto ERROR_ENCOUNTERED;
	}

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldExecute == VARIANT_TRUE)
	{
		m_bstrModuleName = bstrModuleName;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstrModuleName;

		hr = SendModuleCommand(DISPID_MODULE_MODULENAME, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of module change
		hr = NotifyModuleChanged(cmName, CComVariant(bstrModuleName));
	}

ERROR_ENCOUNTERED:
	return ReportCOMModuleError(hr);
}
#endif

#if 0
STDMETHODIMP CCOMModuleObject::put_ModuleCLSID(REFCLSID clsid)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

#ifdef _DEBUG
	CString strCLSID = VWStringFromCLSID(m_ModuleCLSID);
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CCOMModuleObject::put_ModuleCLSID: name (%s), CLSID %s\n", 
		CString(m_bstrModuleName), strCLSID);
#endif

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldExecute == VARIANT_TRUE)
	{
		m_ModuleCLSID = clsid;

		// cause reload of object
		SAFERELEASE(m_pdisp);
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		ASSERT(FALSE);

		// TODO: need to remote CLSID
//		dispparams.rgvarg[0].vt = VT_BOOL;
//		dispparams.rgvarg[0].boolVal = boolVal;

		hr = SendModuleCommand(DISPID_COMMODULE_MODULECLSID, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of module change
		hr = NotifyModuleChanged(cmCLSID, CComVariant(/* bstrModuleName */));
	}

ERROR_ENCOUNTERED:
	return ReportCOMModuleError(hr);
}
#endif

STDMETHODIMP CCOMModuleObject::put_ModuleProgID(BSTR bstrProgID)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CCOMModuleObject::put_ModuleProgID\n");
#endif

	ASSERT(bstrProgID);
	if (bstrProgID == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CCOMModuleObject::put_ModuleProgID: incoming argument (BSTR) is NULL\n");
		return ReportCOMModuleError(E_INVALIDARG);
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CCOMModuleObject::put_ModuleProgID: name (%s), progid (%s)\n", 
		CString(m_bstrModuleName), CString(bstrProgID));
#endif

	hr = m_pWorld->CheckPolicyExt(NULL, CP_WORLDOWNER | CP_WIZARD, CComVariant());
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CCOMModuleObject::put_ModuleProgID: must be world owner to edit modules\n");
		goto ERROR_ENCOUNTERED;
	}

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldExecute == VARIANT_TRUE)
	{
		CLSID clsid;

		hr = CLSIDFromProgID(bstrProgID, &clsid);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		m_ModuleCLSID = clsid;

		Unload();
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstrProgID;

		hr = SendModuleCommand(DISPID_COMMODULE_MODULEPROGID, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of module change
		hr = NotifyModuleChanged(cmProgID, CComVariant(bstrProgID));
	}

ERROR_ENCOUNTERED:
	return ReportCOMModuleError(hr);
}

STDMETHODIMP CCOMModuleObject::get_Flags(long* plVal)
{
	ASSERT(plVal);
	if (plVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CCOMModuleObject::get_Flags: outgoing pointer (long*) is NULL\n");
		return ReportCOMModuleError(E_POINTER);
	}

	*plVal = m_lFlags;

	return S_OK;
}

STDMETHODIMP CCOMModuleObject::put_Flags(long lVal)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CCOMModuleObject::put_Flags: name (%s)\n", 
		CString(m_bstrModuleName));
#endif

	// at least one bit should be set
	if (!(lVal & MODULE_CLIENT || lVal & MODULE_SERVER))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CCOMModuleObject::put_Flags: invalid flags parameter (%x)\n", lVal);
		return ReportCOMModuleError(E_INVALIDARG);
	}

	hr = m_pWorld->CheckPolicyExt(NULL, CP_WORLDOWNER | CP_WIZARD, CComVariant());
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CCOMModuleObject::put_Flags: must be world owner to edit modules\n");
		goto ERROR_ENCOUNTERED;
	}

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldExecute == VARIANT_TRUE)
	{
		m_lFlags = lVal;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_I4;
		dispparams.rgvarg[0].lVal = lVal;

		hr = SendModuleCommand(DISPID_MODULE_FLAGS, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of module change
		hr = NotifyModuleChanged(cmFlags, CComVariant(lVal));
	}

ERROR_ENCOUNTERED:
	return ReportCOMModuleError(hr);
}

STDMETHODIMP CCOMModuleObject::Install()
{
	HRESULT hr = S_OK;
	IVWDispatch *pvwdisp = NULL;

	if (m_pdisp == NULL)
		return S_OK;

	// if it's a VWorld COM module, Install it with this
	if (SUCCEEDED(m_pdisp->QueryInterface(IID_IVWDispatch, (LPVOID*)&pvwdisp)) && pvwdisp)
	{
		hr = pvwdisp->Install(this);

		SAFERELEASE(pvwdisp);
	}
	else
	{
		DISPPARAMS dispparams;
		CComVariant varRet;
		DISPID dispid;
		HRESULT hrRet = S_OK;
		static CComBSTR bstrInstall("Install");	

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			return hr;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = this;

		// invoke the "Install" method in the module
		hr = InvokeByName(bstrInstall, DISPATCH_METHOD, &dispid, &dispparams, QUIET_INVOKE, &varRet, &hrRet);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		if (SUCCEEDED(hr) || hr == DISP_E_EXCEPTION)
			hr = hrRet;
		else
		{
			// if we failed because we couldn't find the name, that's ok
			if (hr == DISP_E_MEMBERNOTFOUND)
				hr = S_OK;
		}
	}
	
	return hr;
}

HRESULT CCOMModuleObject::InvokeOnLoad()
{
	HRESULT hr = S_OK;
	IVWDispatch *pvwdisp = NULL;

	if (m_pdisp == NULL)
		return S_OK;

	// if it's a VWorld COM module, Install it with this
	if (SUCCEEDED(m_pdisp->QueryInterface(IID_IVWDispatch, (LPVOID*)&pvwdisp)) && pvwdisp)
	{
		hr = pvwdisp->OnLoad(/* this */);

		SAFERELEASE(pvwdisp);
	}
	else
	{
		DISPPARAMS dispparams;
		CComVariant varRet;
		DISPID dispid;
		HRESULT hrRet = S_OK;
		static CComBSTR bstrOnLoad("OnLoad");	

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 0);
		if (FAILED(hr))
			return hr;

//		dispparams.rgvarg[0].vt = VT_DISPATCH;
//		dispparams.rgvarg[0].pdispVal = this;

		// invoke the "OnLoad" method in the module
		hr = InvokeByName(bstrOnLoad, DISPATCH_METHOD, &dispid, &dispparams, QUIET_INVOKE, &varRet, &hrRet);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		if (SUCCEEDED(hr) || hr == DISP_E_EXCEPTION)
			hr = hrRet;
		else
		{
			// if we failed because we couldn't find the name, that's ok
			if (hr == DISP_E_MEMBERNOTFOUND)
				hr = S_OK;
		}
	}
	
	return hr;
}

HRESULT CCOMModuleObject::InvokeOnUnload()
{
	HRESULT hr = S_OK;
	IVWDispatch *pvwdisp = NULL;

	if (m_pdisp == NULL)
		return S_OK;

	// if it's a VWorld COM module, Install it with this
	if (SUCCEEDED(m_pdisp->QueryInterface(IID_IVWDispatch, (LPVOID*)&pvwdisp)) && pvwdisp)
	{
		hr = pvwdisp->OnUnload(/* this */);

		SAFERELEASE(pvwdisp);
	}
	else
	{
		DISPPARAMS dispparams;
		CComVariant varRet;
		DISPID dispid;
		HRESULT hrRet = S_OK;
		static CComBSTR bstrUnload("OnUnload");	

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 0);
		if (FAILED(hr))
			return hr;

//		dispparams.rgvarg[0].vt = VT_DISPATCH;
//		dispparams.rgvarg[0].pdispVal = this;

		// invoke the "OnUnload" method in the module
		hr = InvokeByName(bstrUnload, DISPATCH_METHOD, &dispid, &dispparams, QUIET_INVOKE, &varRet, &hrRet);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		if (SUCCEEDED(hr) || hr == DISP_E_EXCEPTION)
			hr = hrRet;
		else
		{
			// if we failed because we couldn't find the name, that's ok
			if (hr == DISP_E_MEMBERNOTFOUND)
				hr = S_OK;
		}
	}
	
	return hr;
}

STDMETHODIMP CCOMModuleObject::InvokeByName(BSTR bstrSubName, WORD wFlags, DISPID* pdispid, DISPPARAMS* pdispparams, VARIANT_BOOL bQuiet, LPVARIANT pvar, HRESULT* pinvokehr)
{
	HRESULT hr = S_OK;
	EXCEPINFO excepinfo;
	unsigned int argerr = 0;
	DISPID dispidNamedArgs = DISPID_PROPERTYPUT;
	
	memset(&excepinfo, 0x0, sizeof(EXCEPINFO));

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CCOMModuleObject::InvokeByName\n");
#endif

	ASSERT(bstrSubName);
	if (bstrSubName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CCOMModuleObject::InvokeByName: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	ASSERT(pdispid);
	if (pdispid == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CCOMModuleObject::InvokeByName: outgoing pointer (DISPID*) is NULL\n");
		return E_POINTER;
	}

	// NOTE: pvar and pinvokehr can be NULL

	*pdispid = DISPID_UNKNOWN;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CCOMModuleObject::InvokeByName: name (%s), method name %s\n", CString(m_bstrModuleName), CString(bstrSubName));
#endif

	if (m_bShutdown == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CCOMModuleObject::InvokeByName: module is shutdown, can't invoke method\n");
		return VWOBJECT_E_MODULESHUTDOWN;
	}

	if (m_pdisp == NULL)
	{
		hr = LoadObject();
		if (FAILED(hr))
		{
			/* hr = */ Unload(/* VARIANT_TRUE */);
			return hr;
		}
	}

#ifdef COM_PERF_DEBUG
	DWORD dwTime = GetTickCount();
#endif

	hr = m_pdisp->GetIDsOfNames(IID_NULL, &bstrSubName, 1, LOCALE_USER_DEFAULT, pdispid);
	if (FAILED(hr))
		return hr;
	
	if (wFlags == DISPATCH_PROPERTYPUT)
	{
		pdispparams->rgdispidNamedArgs = &dispidNamedArgs;
		pdispparams->cNamedArgs = 1;
	}

	try 
	{
		// stick result/error code back in action, or store in method?
		hr = m_pdisp->Invoke(*pdispid, IID_NULL, LOCALE_USER_DEFAULT, wFlags, 
			pdispparams, (wFlags == DISPATCH_PROPERTYPUT) ? NULL : pvar, &excepinfo, &argerr);
	}
	catch (...)
	{
		hr = VWOBJECT_E_INVOKEEXCEPTION;
	}

	if (wFlags == DISPATCH_PROPERTYPUT)
	{
		pdispparams->rgdispidNamedArgs = NULL;
		pdispparams->cNamedArgs = 0;
	}

	if (hr == DISP_E_EXCEPTION)
	{
		HRESULT excephr = ResultFromScode(excepinfo.scode);

		if (pinvokehr)
			*pinvokehr = excephr;

		if (bQuiet == VARIANT_FALSE)
			VWREPORT(m_pWorld, VWR_ERROR, "Error occurred in module (%s), method (%s), hr %x, %s\n", CString(m_bstrModuleName), CString(bstrSubName), excephr, CString(excepinfo.bstrDescription));
		else
		{
			if (excephr != DISP_E_MEMBERNOTFOUND)
				VWREPORT(m_pWorld, VWR_ERROR, "Error occurred in module (%s), method (%s), hr %x, %s\n", CString(m_bstrModuleName), CString(bstrSubName), excephr, CString(excepinfo.bstrDescription));
		}
	}

#ifdef COM_PERF_DEBUG
	{
		DWORD dwNow = GetTickCount();

		VWREPORT(m_pWorld, VWR_PERFLOG, "Invoke COM (Name),%d,%s,%s,%d\n",
			dwNow, CString(m_bstrModuleName), CString(bstrSubName), dwNow - dwTime);
	}
#endif

	return hr;
}

STDMETHODIMP CCOMModuleObject::InvokeByDispID(DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams, VARIANT_BOOL bQuiet, LPVARIANT pvar, HRESULT* pinvokehr)
{
	HRESULT hr = S_OK;
	EXCEPINFO excepinfo;
	unsigned int argerr = 0;
	DISPID dispidNamedArgs = DISPID_PROPERTYPUT;

	memset(&excepinfo, 0x0, sizeof(EXCEPINFO));

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CCOMModuleObject::InvokeByDispID\n");
#endif

	ASSERT(pdispparams);
	if (pdispparams == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CCOMModuleObject::InvokeByDispID: incoming argument (DISPPARAMS*) is NULL\n");
		return E_INVALIDARG;
	}

	if (pinvokehr)
		*pinvokehr = S_OK;

	if (pvar)
		::VariantClear(pvar);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CCOMModuleObject::InvokeByDispID: name (%s), dispid %d\n", CString(m_bstrModuleName), dispid);
#endif

	if (m_bShutdown == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CCOMModuleObject::InvokeByDispID: module is shutdown, can't invoke method\n");
		return VWOBJECT_E_MODULESHUTDOWN;
	}

	if (m_pdisp == NULL)
	{
		hr = LoadObject();
		if (FAILED(hr))
		{
			/* hr = */ Unload(/* VARIANT_TRUE */);
			return hr;
		}
	}

#ifdef COM_PERF_DEBUG
	DWORD dwTime = GetTickCount();
#endif

	if (wFlags == DISPATCH_PROPERTYPUT)
	{
		pdispparams->rgdispidNamedArgs = &dispidNamedArgs;
		pdispparams->cNamedArgs = 1;
	}

	try 
	{
		// stick result/error code back in action, or store in method?
		hr = m_pdisp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, wFlags, 
			pdispparams, (wFlags == DISPATCH_PROPERTYPUT) ? NULL : pvar, &excepinfo, &argerr);
	}
	catch (...)
	{
		hr = VWOBJECT_E_INVOKEEXCEPTION;
	}

#ifdef _DEBUG
	if (hr == VWOBJECT_E_INVOKEEXCEPTION)
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CCOMModuleObject::InvokeByDispID: caught exception in IDispatch::Invoke, name (%s), dispid %d\n", CString(m_bstrModuleName), dispid);
#endif

	if (wFlags == DISPATCH_PROPERTYPUT)
	{
		pdispparams->rgdispidNamedArgs = NULL;
		pdispparams->cNamedArgs = 0;
	}

	if (hr == DISP_E_EXCEPTION)
	{
		HRESULT excephr = ResultFromScode(excepinfo.scode);

		if (pinvokehr)
			*pinvokehr = excephr;

		if (bQuiet == VARIANT_FALSE)
			VWREPORT(m_pWorld, VWR_ERROR, "Error occurred in module (%s), dispid %d, hr %x, %s\n", CString(m_bstrModuleName), dispid, excephr, CString(excepinfo.bstrDescription));
		else
		{
			if (excephr != DISP_E_MEMBERNOTFOUND)
				VWREPORT(m_pWorld, VWR_ERROR, "Error occurred in module (%s), dispid %d, hr %x, %s\n", CString(m_bstrModuleName), dispid, excephr, CString(excepinfo.bstrDescription));
		}
	}

#ifdef COM_PERF_DEBUG
	{
		DWORD dwNow = GetTickCount();

		VWREPORT(m_pWorld, VWR_PERFLOG, "Invoke COM (DISPID),%d,%s,%d,%d\n",
			dwNow, CString(m_bstrModuleName), dispid, dwNow - dwTime);
	}
#endif

	return hr;
}

STDMETHODIMP CCOMModuleObject::Marshall(DWORD dwOptions, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = CCOMModuleObjectBase::Marshall(dwOptions, pbuffer)))
		goto ERROR_ENCOUNTERED;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CCOMModuleObject::Marshall: marshalling module CLSID\n");

	hr = pbuffer->put_ClassID(m_ModuleCLSID);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CCOMModuleObject::Marshall: marshalling module name\n");

	hr = pbuffer->put_String(m_bstrModuleName);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->put_Long(m_lFlags);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
	{
	CString strCLSID = VWStringFromCLSID(m_ModuleCLSID);
	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CCOMModuleObject::Marshall: clsid %s, name (%s)\n", strCLSID, CString(m_bstrModuleName));
	}
#endif

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CCOMModuleObject::UnMarshall(IUnMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	// free any loaded data
	Unload();

	m_bstrModuleName.Empty();
	memset(&m_ModuleCLSID, 0x0, sizeof(CLSID));

	if (FAILED(hr = CCOMModuleObjectBase::UnMarshall(pbuffer)))
		goto ERROR_ENCOUNTERED;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CCOMModuleObject::UnMarshall: unmarshalling module CLSID\n");

	hr = pbuffer->get_ClassID(&m_ModuleCLSID);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CCOMModuleObject::UnMarshall: unmarshalling module name\n");

	hr = pbuffer->get_String(&m_bstrModuleName.m_str);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->get_Long(&m_lFlags);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
	{
	CString strCLSID = VWStringFromCLSID(m_ModuleCLSID);
	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CCOMModuleObject::UnMarshall: clsid %s, name (%s)\n", strCLSID, CString(m_bstrModuleName));
	}
#endif

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CCOMModuleObject::MarshallToScript(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut)
{
	HRESULT hr = S_OK;
	BSTR bstrProgID = NULL;
	CString strOut;

	if (pbstrOut==NULL || pbAsVal==NULL)
		return E_INVALIDARG;

	// initialize 
	*pbAsVal = VARIANT_TRUE;

	if (EXTRACTREF(dwOptions) == MARSHALL_BYVALUE)
	{
		// get the module progid
		hr = ::ProgIDFromCLSID( m_ModuleCLSID, &bstrProgID);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// format the output line
		strOut.Format(	"World.CreateCOMModule(\"%s\", \"%s\", %s)",
						(LPCTSTR) CString(m_bstrModuleName),
						(LPCTSTR) CString(bstrProgID),
						(m_lFlags & MODULE_SERVER) ? ((m_lFlags & MODULE_CLIENT) ? (LPCTSTR) "World.MODULE_CLIENT + World.MODULE_SERVER" : "World.MODULE_SERVER") : (LPCTSTR) "World.MODULE_CLIENT");
	}
	else
	{
		strOut.Format("World.Module(\"%s\")", (LPCTSTR) CString(m_bstrModuleName));
	}

	// get a BSTR out of it
	*pbstrOut = strOut.AllocSysString();

ERROR_ENCOUNTERED:

	if (bstrProgID!=NULL)
		::CoTaskMemFree(bstrProgID);

	return hr;
}

HRESULT CCOMModuleObject::NotifyModuleChanged(long lHint, VARIANT varHintData)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bFreezeEvents = VARIANT_TRUE;
	CWorldObject* pWorld = (CWorldObject*)m_pWorld;

//	m_pWorld->get_FreezeEvents(&bFreezeEvents);

	if (pWorld->m_bFreezeEvents == VARIANT_FALSE)
	{
		// notify client/server and world of module change
		hr = m_pWorld->NotifyModuleChanged(m_bstrModuleName, lHint, varHintData);
	}

	return hr;
}