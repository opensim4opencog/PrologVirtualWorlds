// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// COMModuleObj.h : Declaration of the CCOMModuleObject

#include <resource.h>

#include <PropBase.h>
#include <objimpl.h>

EXTERN_C const CLSID CLSID_COMModule;

#define ReportCOMModuleError(hr) SUCCEEDED(hr) ? hr : ReportError("VWSYSTEM.COMModule.1", hr, 0x200, 0x300, "VWOBJECT")

/////////////////////////////////////////////////////////////////////////////
// VWOBJECT

//REVIEW -- using pointers to ID's is necessary because some compilers don't like
//references as template arguments.

DECLARE_OBJECTPROPERTY(COMModule, LIBID_VWSYSTEMLib, IDS_COMMODULE_TYPE)
{
DECLARE_AGGREGATABLE(CCOMModuleObject)

public:
BEGIN_COM_MAP(CCOMModuleObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectProperty)
	COM_INTERFACE_ENTRY(IModule)
	COM_INTERFACE_ENTRY(ICOMModule)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()
	
DECLARE_REGISTRY(CLSID_COMModule, "VWSYSTEM.COMModule.1", "VWSYSTEM.COMModule", IDS_COMMODULE_DESC, THREADFLAGS_BOTH)

public:
	CCOMModuleObject() : m_pdisp(NULL), m_ModuleCLSID(CLSID_NULL), m_lFlags(0L), m_bShutdown(VARIANT_FALSE)
	{ 
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CCOMModuleObject::CCOMModuleObject, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif
	};

	~CCOMModuleObject() 
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CCOMModuleObject::~CCOMModuleObject, this 0x%0x\n", this);
#endif

		Terminate();
	};

	STDMETHOD(Terminate)(void)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CCOMModuleObject::Terminate, this 0x%0x\n", this);
#endif

		CCOMModuleObjectBase::Terminate();

		SAFERELEASE(m_pdisp);

		return S_OK;
	};

// ICOMModule
public:
	// Override the base class
	STDMETHOD(get_RefCount)(DWORD *pdwRef);
	STDMETHOD(Dump)();
	STDMETHOD(Marshall)(DWORD dwOptions, IMarshallBuffer* pbuffer); 
	STDMETHOD(UnMarshall)(IUnMarshallBuffer* pbuffer); 

	STDMETHOD(MarshallToScript)(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut);

	void GetMarshallingFlags(VARIANT_BOOL *bShouldExecute, VARIANT_BOOL *bShouldRemote, VARIANT_BOOL *bShouldNotify)
	{
		if (m_pWorld)
		{
			m_pWorld->get_ShouldExecute(bShouldExecute);

			m_pWorld->get_ShouldRemote(bShouldRemote);

			m_pWorld->get_ShouldNotify(bShouldNotify);
		}
	};

// ICOMModule
public:
	STDMETHOD(get_Flags)(long* plval);
	STDMETHOD(put_Flags)(long lval);

	STDMETHOD(get_ModuleName)(BSTR* pbstr)
	{
		ASSERT(pbstr);
		if (pbstr == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CCOMModuleObject::get_ModuleName: outgoing pointer (BSTR*) is NULL\n");
			return E_POINTER;
		}

		*pbstr = m_bstrModuleName.Copy();
		return S_OK;
	};

//	STDMETHOD(put_ModuleName)(BSTR bstr);		

	STDMETHOD(get_ModuleType)(long* plval)
	{
		ASSERT(plval);
		if (plval == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CCOMModuleObject::get_ModuleType: outgoing pointer (long*) is NULL\n");
			return E_POINTER;
		}

		*plval = mcCOM;

		return S_OK;
	};

	STDMETHOD(get_ModuleCLSID)(CLSID* pclsid)
	{
		ASSERT(pclsid);
		if (pclsid == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CCOMModuleObject::get_ModuleCLSID: outgoing pointer (CLSID*) is NULL\n");
			return E_POINTER;
		}

		*pclsid = m_ModuleCLSID;

		return S_OK;
	};

#if 0
	STDMETHOD(put_ModuleCLSID)(REFCLSID clsid);
#endif

	STDMETHOD(put_ModuleProgID)(BSTR bstr);

	STDMETHOD(Install)();

	HRESULT InvokeOnLoad();
	HRESULT InvokeOnUnload();

	STDMETHOD(InvokeByName)(BSTR bstrSubName, WORD wFlags, DISPID* pdispid, DISPPARAMS* pdispparams, VARIANT_BOOL bQuiet, LPVARIANT pvar, HRESULT* invokehr);
	STDMETHOD(InvokeByDispID)(DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams, VARIANT_BOOL bQuiet, LPVARIANT pvar, HRESULT* invokehr);

	STDMETHOD(get_Dispatch)(IDispatch** ppdispatch)
	{
		ASSERT(ppdispatch);
		if (ppdispatch == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CCOMModuleObject::get_Dispatch: outgoing pointer (IDispatch**) is NULL\n");
			return E_POINTER;
		}

		if (m_pdisp == NULL)
		{
			HRESULT hr = LoadObject();
			if (FAILED(hr))
				return hr;
		}

		*ppdispatch = m_pdisp;
		SAFEADDREF(*ppdispatch);

		return S_OK;
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

	HRESULT NotifyModuleChanged(long lHint, VARIANT varHintData);

	HRESULT LoadObject(void);

	HRESULT SendModuleCommand(DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams)
	{
		if (m_pWorld == NULL)
			return VWOBJECT_E_INVALIDWORLD;
		else
			return m_pWorld->SendModuleCommand(this, dispid, wFlags, pdispparams);
	};

// ICOMModule
public:
	// name of this module within world
	CComBSTR m_bstrModuleName;

	long m_lFlags;

	// class ID for automation object
	CLSID m_ModuleCLSID;

private:
	// dispatch interface for automation object
	IDispatch* m_pdisp;

	VARIANT_BOOL m_bShutdown;
};

