// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// ScriptModuleObj.h : Declaration of the CScriptModuleObject

#include <resource.h>

#include "Scripts.h"

#include <PropBase.h>
#include <objimpl.h>

EXTERN_C const CLSID CLSID_ScriptModule;

#define ReportScriptModuleError(hr) SUCCEEDED(hr) ? hr : ReportError("VWSYSTEM.ScriptModule.1", hr, 0x200, 0x300)

// now 8 minutes, rather than 2 (must be in msec)
#define DEFAULT_TIMEOUT -1 // (8 * 60 * 1000)

/////////////////////////////////////////////////////////////////////////////
// VWOBJECT

//REVIEW -- using pointers to ID's is necessary because some compilers don't like
//references as template arguments.

DECLARE_OBJECTPROPERTY(ScriptModule, LIBID_VWSYSTEMLib, IDS_SCRIPTMODULE_TYPE)
{
DECLARE_AGGREGATABLE(CScriptModuleObject)

public:
BEGIN_COM_MAP(CScriptModuleObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectProperty)
	COM_INTERFACE_ENTRY(IModule)
	COM_INTERFACE_ENTRY(IScriptModule)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()
		
DECLARE_REGISTRY(CLSID_ScriptModule, "VWSYSTEM.ScriptModule.1", "VWSYSTEM.ScriptModule", IDS_SCRIPTMODULE_DESC, THREADFLAGS_BOTH)

public:
	CScriptModuleObject() : m_iTimeOut(DEFAULT_TIMEOUT),
		m_lFlags(0L), m_ScriptSite(), m_bShutdown(VARIANT_FALSE)
	{ 
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CScriptModuleObject::CScriptModuleObject, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif
	};

	~CScriptModuleObject() 
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CScriptModuleObject::~CScriptModuleObject, this 0x%0x\n", this);
#endif

		Terminate();
	};

	STDMETHOD(Terminate)(void)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CScriptModuleObject::Terminate, this 0x%0x\n", this);
#endif

		CScriptModuleObjectBase::Terminate();

		// close the existing script
		m_ScriptSite.Close();

		return S_OK;
	};


// IScriptModule
public:

	// Override the base class
	STDMETHOD(get_RefCount)(DWORD *pdwRef);
	STDMETHOD(Dump)();
	STDMETHOD(Marshall)(DWORD dwOptions, IMarshallBuffer* pbuffer); 
	STDMETHOD(UnMarshall)(IUnMarshallBuffer* pbuffer); 
	STDMETHOD(MarshallToScript)(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut);
	STDMETHOD(put_World)(IWorld* pworld)
	{
		HRESULT hr = CScriptModuleObjectBase::put_World(pworld);

#if 0
		VARIANT_BOOL bServerSide = VARIANT_FALSE;
		if ( pworld )
		{
			//doesn't fail
			pworld->get_ServerSide(&bServerSide);
		}
		m_ScriptSite.EnableSiteWindow( bServerSide == VARIANT_TRUE? FALSE:TRUE );
#endif

		// never enable UI for script site
		m_ScriptSite.EnableSiteWindow(FALSE);

		return hr;
	}

	void GetMarshallingFlags(VARIANT_BOOL *bShouldExecute, VARIANT_BOOL *bShouldRemote, VARIANT_BOOL *bShouldNotify)
	{
		if (m_pWorld)
		{
			m_pWorld->get_ShouldExecute(bShouldExecute);

			m_pWorld->get_ShouldRemote(bShouldRemote);

			m_pWorld->get_ShouldNotify(bShouldNotify);
		}
	};

// IScriptModule
public:
	STDMETHOD(get_Flags)(long* plval);
	STDMETHOD(put_Flags)(long lval);

	STDMETHOD(get_ModuleName)(BSTR* pbstr)
	{
		ASSERT(pbstr);

		*pbstr = m_bstrModuleName.Copy();
		return S_OK;
	};

//	STDMETHOD(put_ModuleName)(BSTR bstr);

	STDMETHOD(get_ModuleType)(long* plval)
	{
		ASSERT(plval);

		*plval = mcScript;

		return S_OK;
	};

	STDMETHOD(get_ScriptPath)(BSTR* pbstr)
	{
		ASSERT(pbstr);

		*pbstr = m_bstrScriptPath.Copy();
		return S_OK;
	};

	STDMETHOD(put_ScriptPath)(BSTR bstr);

	STDMETHOD(Install)();

	HRESULT InvokeOnLoad();
	HRESULT InvokeOnUnload();

	STDMETHOD(InvokeByName)(BSTR bstrSubName, WORD wFlags, DISPID* pdispid, DISPPARAMS* pdispparams, VARIANT_BOOL bQuiet, LPVARIANT pvar, HRESULT* phr);
	STDMETHOD(InvokeByDispID)(DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams, VARIANT_BOOL bQuiet, LPVARIANT pvar, HRESULT* phr);

	STDMETHOD(AddScriptlet)(BSTR bstrCode);

	STDMETHOD(get_Dispatch)(IDispatch** ppdispatch)
	{
		return m_ScriptSite.GetScriptDispatch(ppdispatch);
	};

	STDMETHOD(Reload)(VARIANT_BOOL bReinstall = VARIANT_FALSE);
	STDMETHOD(Unload)(VARIANT_BOOL bShutdown = VARIANT_FALSE);
	STDMETHOD(Validate)(VARIANT_BOOL* pboolVal);

	STDMETHOD(get_IsShutdown)(VARIANT_BOOL* pboolVal)
	{
		if (pboolVal == NULL)
			return E_POINTER;

		*pboolVal = m_bShutdown;

		return S_OK;
	};

	// helper functions
	HRESULT LoadScript();

	HRESULT NotifyModuleChanged(long lHint, VARIANT varHintData);

	HRESULT SendModuleCommand(DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams)
	{
		if (m_pWorld == NULL)
			return VWOBJECT_E_INVALIDWORLD;
		else
			return m_pWorld->SendModuleCommand(this, dispid, wFlags, pdispparams);
	};

// IScriptModule
public:
	// script module reference
	CComBSTR m_bstrScriptPath;
	CComBSTR m_bstrModuleName;
	long m_lFlags;

private:
	CScriptSite m_ScriptSite;
	int m_iTimeOut;

	VARIANT_BOOL m_bShutdown;
};
