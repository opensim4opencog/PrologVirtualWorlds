// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// ScriptObj.cpp : Implementation of CfooApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include <inetfile.h>
#include "worldobj.h"
#include "scrptobj.h"

IMPLEMENT_OBJECTPROPERTY(CScriptModuleObject)

//#define SCRIPT_PERF_DEBUG

#define QUIET_INVOKE VARIANT_TRUE
//#define QUIET_INVOKE VARIANT_FALSE

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CScriptModuleObject::Dump(void)
{
	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "ScriptModule: begin\n");

	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "ScriptModule: m_bstrScriptPath %s\n", CString(m_bstrScriptPath));
	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "ScriptModule: m_bstrModuleName %s\n", CString(m_bstrModuleName));
	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "ScriptModule: server-side (%s), client-side (%s)\n",
		(m_lFlags & MODULE_SERVER) ? "true" : "false",
		(m_lFlags & MODULE_CLIENT) ? "true" : "false");
	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "ScriptModule: local (%s)\n", 
		(m_lFlags & MODULE_LOCAL) ? "true" : "false");

	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "ScriptModule: end\n");

	return S_OK;
}

#if 0
STDMETHODIMP CScriptModuleObject::put_ModuleName(BSTR bstrModuleName)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CScriptModuleObject::put_ModuleName\n");

	ASSERT(bstrModuleName);
	if (bstrModuleName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptModuleObject::put_ModuleName: incoming argument (BSTR) is NULL\n");
		return ReportScriptModuleError(E_INVALIDARG);
	}

	hr = m_pWorld->CheckPolicyExt(NULL, CP_WORLDOWNER | CP_WIZARD, CComVariant());
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptModuleObject::put_ModuleName: must be world owner to edit modules\n");
		goto ERROR_ENCOUNTERED;
	}

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	// if this is a "local" module - don't remote
	if (m_lFlags & MODULE_LOCAL)
		bShouldRemote = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CScriptModuleObject::put_ModuleName: name (%s)\n", 
		CString(bstrModuleName));
#endif

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
		hr = NotifyModuleChanged(smName, CComVariant(bstrModuleName));
	}

ERROR_ENCOUNTERED:
	return ReportScriptModuleError(hr);
}
#endif

STDMETHODIMP CScriptModuleObject::put_ScriptPath(BSTR bstrScriptPath)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CScriptModuleObject::put_ScriptPath\n");
#endif

	ASSERT(bstrScriptPath);
	if (bstrScriptPath == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptModuleObject::put_ScriptPath: incoming argument (BSTR) is NULL\n");
		return ReportScriptModuleError(E_INVALIDARG);
	}

	hr = m_pWorld->CheckPolicyExt(NULL, CP_WORLDOWNER | CP_WIZARD, CComVariant());
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptModuleObject::put_ScriptPath: must be world owner to edit modules\n");
		goto ERROR_ENCOUNTERED;
	}

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	// if this is a "local" module - don't remote
	if (m_lFlags & MODULE_LOCAL)
		bShouldRemote = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CScriptModuleObject::put_ScriptPath: name (%s), path (%s)\n", 
		CString(m_bstrModuleName), CString(bstrScriptPath));
#endif

	if (bShouldExecute == VARIANT_TRUE)
	{
		m_bstrScriptPath = bstrScriptPath;

		// close the existing script
		hr = Unload();
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}
	
	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstrScriptPath;

		hr = SendModuleCommand(DISPID_SCRIPTMODULE_SCRIPTPATH, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of module change
		hr = NotifyModuleChanged(smScriptPath, CComVariant(bstrScriptPath));
	}

ERROR_ENCOUNTERED:
	return ReportScriptModuleError(hr);
}

STDMETHODIMP CScriptModuleObject::get_Flags(long* plVal)
{
	ASSERT(plVal);
	if (plVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptModuleObject::get_Flags: outgoing pointer (long*) is NULL\n");
		return ReportScriptModuleError(E_POINTER);
	}

	*plVal = m_lFlags;
	return S_OK;
}

STDMETHODIMP CScriptModuleObject::put_Flags(long lVal)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CScriptModuleObject::put_Flags: name (%s)\n", 
		CString(m_bstrModuleName));
#endif

	// at least one bit should be set
	if (!(lVal & MODULE_CLIENT || lVal & MODULE_SERVER))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptModuleObject::put_Flags: invalid flags parameter (%x)\n", lVal);
		return ReportScriptModuleError(E_INVALIDARG);
	}

	hr = m_pWorld->CheckPolicyExt(NULL, CP_WORLDOWNER | CP_WIZARD, CComVariant());
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptModuleObject::put_Flags: must be world owner to edit modules\n");
		goto ERROR_ENCOUNTERED;
	}

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldExecute == VARIANT_TRUE)
	{
		m_lFlags = lVal;
	}

	// NOTE: put after ShouldExecute so m_lFlags would be set
	// if this is a "local" module - don't remote
	if (m_lFlags & MODULE_LOCAL)
		bShouldRemote = VARIANT_FALSE;

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
		hr = NotifyModuleChanged(smFlags, CComVariant(lVal));
	}

ERROR_ENCOUNTERED:
	return ReportScriptModuleError(hr);
}

STDMETHODIMP CScriptModuleObject::Install()
{
	HRESULT hr, hrRet;
	DISPID id;
	DISPPARAMS dispparams;
	CComVariant varRet;
	static CComBSTR bstrInstall("Install");

	// package up args into DISPPARAMS struct
	hr = InitializeDISPPARAMS(&dispparams, 1);
	if (FAILED(hr))
		return hr;

	dispparams.rgvarg[0].vt = VT_DISPATCH;
	dispparams.rgvarg[0].pdispVal = this;

	// invoke the "Install" method in the module
	hr = InvokeByName(bstrInstall, DISPATCH_METHOD, &id, &dispparams, QUIET_INVOKE, &varRet, &hrRet);

	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

	if (SUCCEEDED(hr) || hr == DISP_E_EXCEPTION)
		hr = hrRet;
	else
	{
		// if we failed because we couldn't find the name, that's ok
		if (hr == DISP_E_UNKNOWNNAME)
			hr = S_OK;
	}

	return hr;
}

HRESULT CScriptModuleObject::InvokeOnLoad()
{
	HRESULT hr = S_OK;
	DISPPARAMS dispparams;
	CComVariant varRet;
	DISPID dispid;
	HRESULT hrRet = S_OK;
	static CComBSTR bstrOnLoad("OnLoad");	

	// package up args into DISPPARAMS struct
	hr = InitializeDISPPARAMS(&dispparams, 0);
	if (FAILED(hr))
		return hr;

//	dispparams.rgvarg[0].vt = VT_DISPATCH;
//	dispparams.rgvarg[0].pdispVal = this;

	// invoke the "OnLoad" method in the module
	hr = InvokeByName(bstrOnLoad, DISPATCH_METHOD, &dispid, &dispparams, QUIET_INVOKE, &varRet, &hrRet);

	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

	if (SUCCEEDED(hr) || hr == DISP_E_EXCEPTION)
		hr = hrRet;
	else
	{
		// if we failed because we couldn't find the name, that's ok
		if (hr == DISP_E_UNKNOWNNAME)
			hr = S_OK;
	}
	
	return hr;
}

HRESULT CScriptModuleObject::InvokeOnUnload()
{
	HRESULT hr = S_OK;
	DISPPARAMS dispparams;
	CComVariant varRet;
	DISPID dispid;
	HRESULT hrRet = S_OK;
	static CComBSTR bstrUnload("OnUnload");	

	// package up args into DISPPARAMS struct
	hr = InitializeDISPPARAMS(&dispparams, 0);
	if (FAILED(hr))
		return hr;

//	dispparams.rgvarg[0].vt = VT_DISPATCH;
//	dispparams.rgvarg[0].pdispVal = this;

	// invoke the "OnUnload" method in the module
	hr = InvokeByName(bstrUnload, DISPATCH_METHOD, &dispid, &dispparams, QUIET_INVOKE, &varRet, &hrRet);

	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

	if (SUCCEEDED(hr) || hr == DISP_E_EXCEPTION)
		hr = hrRet;
	else
	{
		// if we failed because we couldn't find the name, that's ok
		if (hr == DISP_E_UNKNOWNNAME)
			hr = S_OK;
	}
	
	return hr;
}

STDMETHODIMP CScriptModuleObject::Reload(VARIANT_BOOL bReinstall)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CScriptModuleObject::Reload: name (%s)\n", CString(m_bstrModuleName));
#endif

	hr = m_pWorld->CheckPolicyExt(NULL, CP_WORLDOWNER | CP_WIZARD, CComVariant());
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptModuleObject::Reload: must be world owner to edit modules\n");
		goto ERROR_ENCOUNTERED;
	}

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (m_bShutdown == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptModuleObject::Reload: module is shutdown, can't reload\n");
		hr = VWOBJECT_E_MODULESHUTDOWN;
		goto ERROR_ENCOUNTERED;
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		Unload();

		if (!m_ScriptSite.IsLoaded())
		{
			hr = LoadScript();
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
	return ReportScriptModuleError(hr);
}

HRESULT CScriptModuleObject::LoadScript()
{
	HRESULT hr = NULL;
//	ITypeInfo* pTypeInfo = NULL;
	IInternetFileManager *pInetfile = NULL;
	CString strPath;
	static CComBSTR bstrInetfile("Inetfile");
	CComBSTR bstrFinalURL;
	CComBSTR bstrFile;

#ifdef SCRIPT_PERF_DEBUG
	DWORD dwTime = GetTickCount();
#endif

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CScriptModuleObject::LoadScript: name (%s), path %s\n", CString(m_bstrModuleName), CString(m_bstrScriptPath));

	Unload();

//	hr = m_pWorld->GetTypeInfo(0, LOCALE_SYSTEM_DEFAULT, &pTypeInfo);
//	if (FAILED(hr))
//		goto ERROR_ENCOUNTERED;

	hr = m_ScriptSite.AddNamedItem(CWorldObject::g_strIDSType, m_pWorld, NULL); //pTypeInfo);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// Load the file
	// REVIEW: check if this is the VBScript "special" module
	strPath = m_bstrScriptPath;
	if (strPath != ":VBScript")
	{
		hr = m_pWorld->get_ToolExt(bstrInetfile, (IUnknown**)&pInetfile);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (FAILED(hr = pInetfile->GetFileEx(m_bstrScriptPath, VARIANT_FALSE, &bstrFinalURL.m_str, &bstrFile.m_str))) 
		{
			m_pWorld->Report(CComBSTR(CString("Failed to download file: ") + CString(m_bstrScriptPath)), -1);

			hr = VWOBJECT_E_SCRIPTNOTFOUND;
			goto ERROR_ENCOUNTERED;
		}

		// NOTE: m_ScriptSite doesn't return VWOBJECT HRESULTs
		hr = m_ScriptSite.LoadScript(CString(bstrFile), CWorldObject::g_strIDSType);
		if (FAILED(hr))
		{
			int err = m_ScriptSite.GetLastError();
		
			if (hr == E_FAIL)
			{
				if (err == E_NO_ENGINE)
					hr = VWOBJECT_E_INVALIDSCRIPTENGINE;
				else if (err == E_RUNTIME)
					hr = VWOBJECT_E_RUNTIMESCRIPTERROR;
				else 
					hr = VWOBJECT_E_UNKNOWNSCRIPTERROR;
			}
			else
			{
				hr = VWOBJECT_E_PARSESCRIPTERROR;
			}

			VWREPORT(m_pWorld, VWR_ERROR, "Failed to load script module (%s), path (%s), hr %x, %s\n", CString(m_bstrModuleName), CString(m_bstrScriptPath), hr, CString(m_ScriptSite.strerror(err)));

			goto ERROR_ENCOUNTERED;
		}

		// only invoke OnLoad on non-local modules
		hr = InvokeOnLoad();
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CCOMModuleObject::LoadScript: invocation of OnLoad failed, hr %x\n", hr);
			goto ERROR_ENCOUNTERED;
		}
	}
	else
	{
		// NOTE: m_ScriptSite doesn't return VWOBJECT HRESULTs

		// REVIEW: Tell the script site to load the special VBScript module.
		// This doesn't do a load - it just initializes the script engine.
		hr = m_ScriptSite.LoadScript(strPath, CWorldObject::g_strIDSType);
		if (FAILED(hr))
		{
			int err = m_ScriptSite.GetLastError();
		
			if (hr == E_FAIL)
			{
				if (err == E_NO_ENGINE)
					hr = VWOBJECT_E_INVALIDSCRIPTENGINE;
				else if (err == E_RUNTIME)
					hr = VWOBJECT_E_RUNTIMESCRIPTERROR;
				else 
					hr = VWOBJECT_E_UNKNOWNSCRIPTERROR;
			}
			else
			{
				hr = VWOBJECT_E_PARSESCRIPTERROR;
			}

			VWREPORT(m_pWorld, VWR_ERROR, "Failed to load script module (%s), path (%s), hr %x, %s\n", CString(m_bstrModuleName), CString(m_bstrScriptPath), hr, CString(m_ScriptSite.strerror(err)));

			goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:	
#ifdef SCRIPT_PERF_DEBUG
	{
		DWORD dwNow = GetTickCount();

		VWREPORT(m_pWorld, VWR_PERFLOG, "Load Script,%d,%d,%s,%s\n",
			dwNow, dwNow - dwTime, CString(m_bstrModuleName), CString(m_bstrScriptPath));
	}
#endif

//	SAFERELEASE(pTypeInfo);
	SAFERELEASE(pInetfile);

	return hr;
}

STDMETHODIMP CScriptModuleObject::Unload(VARIANT_BOOL bShutdown)
{
	HRESULT hr = S_OK;

	if (m_bShutdown == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptModuleObject::Unload: module is shutdown, can't unload\n");
		hr = VWOBJECT_E_MODULESHUTDOWN;
		goto ERROR_ENCOUNTERED;
	}

	if (m_ScriptSite.IsLoaded())
	{
		// don't invoke OnUnload on local modules
		if (!(m_lFlags & MODULE_LOCAL))
		{
			hr = InvokeOnUnload();
			if (FAILED(hr))
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptModuleObject::Unload: invocation of OnUnload failed, hr %x\n", hr);
		}

		m_ScriptSite.Close();
	}

	m_bShutdown = bShutdown;

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CScriptModuleObject::Validate(VARIANT_BOOL* pboolVal)
{
	HRESULT hr = S_OK;

	ASSERT(pboolVal);
	if (pboolVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptModuleObject::Validate: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return ReportScriptModuleError(E_POINTER);
	}

	*pboolVal = VARIANT_TRUE;

	// REVIEW: what about the remoting of Reload
//	hr = Reload();

	Unload();

	if (!m_ScriptSite.IsLoaded())
		hr = LoadScript();

	if (FAILED(hr))
		*pboolVal = VARIANT_FALSE;

	Unload();

	return ReportScriptModuleError(hr);
}

STDMETHODIMP CScriptModuleObject::InvokeByName(BSTR bstrName, WORD wFlags, DISPID* pdispid, DISPPARAMS* pdispparams, VARIANT_BOOL bQuiet, LPVARIANT pvar, HRESULT* phr)
{
	HRESULT hr = S_OK;
	static CComBSTR bstrWorld("World");

	ASSERT(pdispid);
	if (pdispid == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptModuleObject::InvokeByName: outgoing pointer (DISPID*) is NULL\n");
		return E_POINTER;
	}

	ASSERT(pdispparams);
	if (pdispparams == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptModuleObject::InvokeByName: incoming argument (DISPPARAMS*) is NULL\n");
		return E_INVALIDARG;
	}

	// NOTE: phr and pvar can be NULL

	if (phr)
		*phr = S_OK;

	if (pvar)
		::VariantClear(pvar);

	*pdispid = DISPID_UNKNOWN;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CScriptModuleObject::InvokeByName: name (%s), method name (%s)\n", CString(m_bstrModuleName), CString(bstrName));
#endif

	if (m_bShutdown == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptModuleObject::InvokeByName: module is shutdown, can't invoke method\n");
		return VWOBJECT_E_MODULESHUTDOWN;
	}

	if (!m_ScriptSite.IsLoaded())
	{
		hr = LoadScript();
		if (FAILED(hr))
		{
			/* hr = */ Unload(/* VARIANT_TRUE */);
			return hr;
		}
	}

#ifdef SCRIPT_PERF_DEBUG
	DWORD dwTime = GetTickCount();
#endif

	try 
	{
		hr = m_ScriptSite.InvokeByName(bstrName, bstrWorld, wFlags, pdispparams, pvar, phr, m_iTimeOut);
	}
	catch (...)
	{
		hr = VWOBJECT_E_INVOKEEXCEPTION;
	}
	
	if (FAILED(hr))
	{
#ifdef _DEBUG
		if (hr == VWOBJECT_E_INVOKEEXCEPTION)
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptModuleObject::InvokeByName: caught exception in IDispatch::Invoke, name (%s), method (%s)\n", CString(m_bstrModuleName), CString(bstrName));
#endif

		int err = m_ScriptSite.GetLastError();

		if (hr == E_FAIL)
		{
			if (err == E_NO_ENGINE)
				hr = VWOBJECT_E_INVALIDSCRIPTENGINE;
			else if (err == E_RUNTIME)
				hr = VWOBJECT_E_RUNTIMESCRIPTERROR;
			else 
				hr = VWOBJECT_E_UNKNOWNSCRIPTERROR;
		}

		if (bQuiet == VARIANT_FALSE)
		{
			if (err != -1)
				VWREPORT(m_pWorld, VWR_ERROR, "Error occurred in module (%s), method (%s), (%s), %s\n", CString(m_bstrModuleName), CString(bstrName), CScriptSite::error_strings[err], CString(m_ScriptSite.strerror(err)));
			else
				VWREPORT(m_pWorld, VWR_ERROR, "Error occurred in module (%s), method (%s), no error information\n", CString(m_bstrModuleName), CString(bstrName));
		}
		else
		{
			if (hr != DISP_E_UNKNOWNNAME && err != E_NO_IDOFNAME)
			{
				if (err != -1)
					VWREPORT(m_pWorld, VWR_ERROR, "Error occurred in module (%s), method (%s), (%s), %s\n", CString(m_bstrModuleName), CString(bstrName), CScriptSite::error_strings[err], CString(m_ScriptSite.strerror(err)));
				else
					VWREPORT(m_pWorld, VWR_ERROR, "Error occurred in module (%s), method (%s), no error information\n", CString(m_bstrModuleName), CString(bstrName));
			}
		}
	}

#ifdef SCRIPT_PERF_DEBUG
	{
		DWORD dwNow = GetTickCount();

		VWREPORT(m_pWorld, VWR_PERFLOG, "Invoke Script (Name),%d,%s,%s,%d\n",
			dwNow, CString(m_bstrModuleName), CString(bstrName), dwNow - dwTime);
	}
#endif

	return hr;
}

STDMETHODIMP CScriptModuleObject::InvokeByDispID(DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams, VARIANT_BOOL bQuiet, LPVARIANT pvar, HRESULT* phr)
{
	HRESULT hr = S_OK;
	static CComBSTR bstrWorld("World");

	ASSERT(pdispparams);
	if (pdispparams == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptModuleObject::InvokeByDispID: incoming argument (DISPPARAMS*) is NULL\n");
		return E_INVALIDARG;
	}

	// NOTE: phr and pvar can be NULL

	if (phr)
		*phr = S_OK;

	if (pvar)
		::VariantClear(pvar);

	if (m_bShutdown == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptModuleObject::InvokeByDispID: module is shutdown, can't invoke method\n");
		return VWOBJECT_E_MODULESHUTDOWN;
	}

	if (!m_ScriptSite.IsLoaded())
	{
		hr = LoadScript();
		if (FAILED(hr))
		{
			/* hr = */ Unload(/* VARIANT_TRUE */);
			return hr;
		}
	}

#ifdef SCRIPT_PERF_DEBUG
	DWORD dwTime = GetTickCount();
#endif

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CScriptModuleObject::InvokeByDispID: name (%s), dispid %d\n", CString(m_bstrModuleName), dispid);

	try 
	{
		hr = m_ScriptSite.InvokeByDispID(dispid, bstrWorld, wFlags, pdispparams, pvar, phr, m_iTimeOut);
	}
	catch (...)
	{
		hr = VWOBJECT_E_INVOKEEXCEPTION;
	}
	
	if (FAILED(hr))
	{
#ifdef _DEBUG
		if (hr == VWOBJECT_E_INVOKEEXCEPTION)
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptModuleObject::InvokeByDispID: caught exception in IDispatch::Invoke, name (%s), dispid %d\n", CString(m_bstrModuleName), dispid);
#endif

		int err = m_ScriptSite.GetLastError();

		if (hr == E_FAIL)
		{
			if (err == E_NO_ENGINE)
				hr = VWOBJECT_E_INVALIDSCRIPTENGINE;
			else if (err == E_RUNTIME)
				hr = VWOBJECT_E_RUNTIMESCRIPTERROR;
			else 
				hr = VWOBJECT_E_UNKNOWNSCRIPTERROR;
		}

		if (bQuiet == VARIANT_FALSE)
		{
			if (err != -1)
				VWREPORT(m_pWorld, VWR_ERROR, "Error occurred in module (%s), dispid %d, (%s), %s\n", CString(m_bstrModuleName), dispid, CScriptSite::error_strings[err], CString(m_ScriptSite.strerror(err)));
			else
				VWREPORT(m_pWorld, VWR_ERROR, "Error occurred in module (%s), dispid %d, no error information\n", CString(m_bstrModuleName), dispid);
		}
		else
		{
			if (hr != DISP_E_UNKNOWNNAME && err != E_NO_IDOFNAME)
			{
				if (err != -1)
					VWREPORT(m_pWorld, VWR_ERROR, "Error occurred in module (%s), dispid %d, (%s), %s\n", CString(m_bstrModuleName), dispid, CScriptSite::error_strings[err], CString(m_ScriptSite.strerror(err)));
				else
					VWREPORT(m_pWorld, VWR_ERROR, "Error occurred in module (%s), dispid %d, no error information\n", CString(m_bstrModuleName), dispid);
			}
		}
	}

#ifdef SCRIPT_PERF_DEBUG
	{
		DWORD dwNow = GetTickCount();

		VWREPORT(m_pWorld, VWR_PERFLOG, "Invoke Script (DISPID),%d,%s,%d,%d\n",
			dwNow, CString(m_bstrModuleName), dispid, dwNow - dwTime);
	}
#endif

	return hr;
}

STDMETHODIMP CScriptModuleObject::AddScriptlet(BSTR bstrCode)
{
	HRESULT hr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CScriptModuleObject::AddScriptlet\n");

	ASSERT(bstrCode);
	if (bstrCode == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptModuleObject::AddScriptlet: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	if (m_bShutdown == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptModuleObject::AddScriptlet: module is shutdown, can't add scriptlet\n");
		return VWOBJECT_E_MODULESHUTDOWN;
	}

	if (!m_ScriptSite.IsLoaded())
	{
		hr = LoadScript();
		if (FAILED(hr))
		{
			/* hr = */ Unload(/* VARIANT_TRUE */);
			return hr;
		}
	}

	// tell the script site to do this
	// Note: InvokeScriptlet will execute any executable code in 
	// bstrCode - the caller should assure we've got a SUB or FUNCTION

	hr = m_ScriptSite.InvokeScriptlet(CString(bstrCode), CWorldObject::g_strIDSType, m_iTimeOut);

	if (FAILED(hr))
	{
#ifdef _DEBUG
		if (hr == VWOBJECT_E_INVOKEEXCEPTION)
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptModuleObject::InvokeScriptlet: caught exception in IDispatch::Invoke, name (%s)\n", CString(m_bstrModuleName));
#endif

		int err = m_ScriptSite.GetLastError();

		if (hr == E_FAIL)
		{
			if (err == E_NO_ENGINE)
				hr = VWOBJECT_E_INVALIDSCRIPTENGINE;
			else if (err == E_RUNTIME)
				hr = VWOBJECT_E_RUNTIMESCRIPTERROR;
			else 
				hr = VWOBJECT_E_UNKNOWNSCRIPTERROR;
		}

#if 0
		if (bQuiet == VARIANT_FALSE)
		{
#endif
			if (err != -1)
				VWREPORT(m_pWorld, VWR_ERROR, "Error occurred in inline module (%s), (%s), %s\n", CString(m_bstrModuleName), CScriptSite::error_strings[err], CString(m_ScriptSite.strerror(err)));
			else
				VWREPORT(m_pWorld, VWR_ERROR, "Error occurred in inline module (%s), no error information\n", CString(m_bstrModuleName));
#if 0
		}
		else
		{
			if (hr != DISP_E_UNKNOWNNAME && err != E_NO_IDOFNAME)
			{
				if (err != -1)
					VWREPORT(m_pWorld, VWR_ERROR, "Error occurred in inline module (%s), (%s), %s\n", CString(m_bstrModuleName), CScriptSite::error_strings[err], CString(m_ScriptSite.strerror(err)));
				else
					VWREPORT(m_pWorld, VWR_ERROR, "Error occurred in inline module (%s), no error information\n", CString(m_bstrModuleName));
			}
		}
#endif
	}

	return hr;
}

STDMETHODIMP CScriptModuleObject::Marshall(DWORD dwOptions, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CScriptModuleObject::Marshall: marshalling script path\n");

	// check to make sure we're not marshaling a local module
	if (m_lFlags & MODULE_LOCAL)
	{
		ASSERT(FALSE);
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	if (FAILED(hr = CScriptModuleObjectBase::Marshall(dwOptions, pbuffer)))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->put_String(m_bstrScriptPath, VARIANT_FALSE);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CScriptModuleObject::Marshall: marshalling module name\n");

	hr = pbuffer->put_String(m_bstrModuleName);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->put_Long(m_lFlags);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CScriptModuleObject::Marshall: script path %s, module name %s\n", CString(m_bstrScriptPath), CString(m_bstrModuleName));

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CScriptModuleObject::UnMarshall(IUnMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	// free any loaded data
	Unload();

	m_bstrScriptPath.Empty();
	m_bstrModuleName.Empty();

	if (FAILED(hr = CScriptModuleObjectBase::UnMarshall(pbuffer)))
		goto ERROR_ENCOUNTERED;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CScriptModuleObject::UnMarshall: unmarshalling script path\n");

	hr = pbuffer->get_String(&m_bstrScriptPath.m_str);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CScriptModuleObject::UnMarshall: unmarshalling module name\n");

	hr = pbuffer->get_String(&m_bstrModuleName.m_str);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->get_Long(&m_lFlags);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CScriptModuleObject::UnMarshall: script path %s, module name %s\n", CString(m_bstrScriptPath), CString(m_bstrModuleName));

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CScriptModuleObject::MarshallToScript(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut)
{
	HRESULT hr = S_OK;
	CString strOut;

	if (pbstrOut==NULL || pbAsVal==NULL)
		return E_INVALIDARG;

	// initialize 
	*pbAsVal = VARIANT_TRUE;

	if (EXTRACTREF(dwOptions) == MARSHALL_BYVALUE)
	{
		// format the output line
		strOut.Format(	"World.CreateScriptModule(\"%s\", \"%s\", %s)",
						(LPCTSTR) CString(m_bstrModuleName),
						(LPCTSTR) CString(m_bstrScriptPath),
						(m_lFlags & MODULE_SERVER) ? ((m_lFlags & MODULE_CLIENT) ? (LPCTSTR) "World.MODULE_CLIENT + World.MODULE_SERVER" : "World.MODULE_SERVER") : (LPCTSTR) "World.MODULE_CLIENT");
	}
	else
	{
		strOut.Format("World.Module(\"%s\")", (LPCTSTR) CString(m_bstrModuleName));
	}

	// get a BSTR out of it
	*pbstrOut = strOut.AllocSysString();

	return hr;
}

HRESULT CScriptModuleObject::NotifyModuleChanged(long lHint, VARIANT varHintData)
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
