// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// MethodObj.h : Declaration of the CMethodObject

#include <resource.h>
#include <PropBase.h>
#include <objimpl.h>

EXTERN_C const CLSID CLSID_Method;

/////////////////////////////////////////////////////////////////////////////
// VWOBJECT

//REVIEW -- using pointers to ID's is necessary because some compilers don't like
//references as template arguments.

DECLARE_OBJECTPROPERTY(Method, LIBID_VWSYSTEMLib, IDS_METHOD_TYPE)
{
DECLARE_AGGREGATABLE(CMethodObject)
	
public:
BEGIN_COM_MAP(CMethodObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectProperty)
	COM_INTERFACE_ENTRY(IMethod)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

DECLARE_REGISTRY(CLSID_Method, "VWSYSTEM.Method.1", "VWSYSTEM.Method", IDS_METHOD_DESC, THREADFLAGS_BOTH)

public:
	CMethodObject() : m_lFlags(0L), m_pModule(NULL), 
		m_dispid(DISPID_UNKNOWN), m_pArgList(NULL), 
		m_fParsed(VARIANT_FALSE)
	{ 
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CMethodObject::CMethodObject, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif

		InitializeDISPPARAMS(&m_dispparams, 0);
	};

	~CMethodObject() 
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CMethodObject::~CMethodObject, this 0x%0x\n", this);
#endif

		Terminate();
	};

public:
	// Override the base class
	STDMETHOD(get_RefCount)(DWORD *pdwRef);
	STDMETHOD(Dump)(); 
	STDMETHOD(Marshall)(DWORD dwOptions, IMarshallBuffer* pbuffer); 
	STDMETHOD(UnMarshall)(IUnMarshallBuffer* pbuffer); 
	STDMETHOD(MarshallToScript)(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut);

	STDMETHOD(Terminate)(void)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CMethodObject::Terminate, this 0x%0x\n", this);
#endif

		CMethodObjectBase::Terminate();

		SAFERELEASE(m_pModule);

		if (m_pArgList)
		{
			m_pArgList->RemoveAll();

			SAFERELEASE(m_pArgList);
		}

		TerminateDISPPARAMS(VARIANT_TRUE, &m_dispparams);

		return S_OK;
	}; 

// IMethod
public:
	STDMETHOD(get_Flags)(long* plVal);
	STDMETHOD(put_Flags)(long lVal);

	STDMETHOD(get_Module)(IModule** ppmodule);
	STDMETHOD(put_Module)(IModule* pmodule);

	STDMETHOD(get_DispID)(DISPID* pdispid);
	STDMETHOD(put_DispID)(DISPID dispid);

	STDMETHOD(get_MethodName)(BSTR* pbstr);
	STDMETHOD(put_MethodName)(BSTR bstr);

	STDMETHOD(get_Code)(BSTR *pbstr);
	STDMETHOD(put_Code)(BSTR bstr);

	STDMETHOD(get_IsInline)(VARIANT_BOOL *pbool);

	STDMETHOD(InvokeMethodExt)(DISPPARAMS* pdispparams, HRESULT* phr, VARIANT* pvar);
	
	// No get_Arguments, not used/implemented

	STDMETHOD(put_Arguments)(VARIANT varArgs)
	{
		HRESULT hr = S_OK;
		DISPPARAMS dispparams;
		CComVariant varTemp;

		hr = VariantCopyInd(&varTemp, &varArgs);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = ConvertSAFEARRAYToDISPPARAMS(&varTemp, &dispparams);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = put_ArgumentsExt(&dispparams);

		TerminateDISPPARAMS(VARIANT_TRUE, &dispparams);

ERROR_ENCOUNTERED:
		return hr;
	};

	STDMETHOD(get_ArgumentsExt)(DISPPARAMS** ppdispparams);
	STDMETHOD(put_ArgumentsExt)(DISPPARAMS* pdispparams);

	STDMETHOD(put_ArgumentsInt)(IPropertyList* pproplist)
	{
		HRESULT hr = S_OK;
		DISPPARAMS dispparams;

		hr = ConvertPropertyListToDISPPARAMS(pproplist, &dispparams);

		if (SUCCEEDED(hr))
			hr = put_ArgumentsExt(&dispparams);

		TerminateDISPPARAMS(VARIANT_TRUE, &dispparams);

		return hr;
	};

	HRESULT SendMethodCommand(DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams)
	{
		ASSERT(m_pWorld);

		if (m_pWorld == NULL)
			return VWOBJECT_E_INVALIDWORLD;
		else
			return m_pWorld->SendMethodCommand(this, dispid, wFlags, pdispparams);
	};

	STDMETHOD(Validate)(VARIANT_BOOL* pboolVal)
	{
		if (m_pModule)
			return m_pModule->Validate(pboolVal);

		return VWOBJECT_E_INVALIDMODULE;
	};

// helpers
protected:
	void Quotify(CString& str);


// IMethod
public:
	// associated module
	IModule* m_pModule;

	// client/server invocation flags
	long m_lFlags;

	// method reference within module
	DISPID m_dispid;
	CComBSTR m_bstrMethodName;

	// inline code stuff
	VARIANT_BOOL m_fParsed;
	CComBSTR m_bstrCode;
	CComBSTR m_bstrInternalName;

private:
	// persistent method arguments
	DISPPARAMS m_dispparams;

	IPropertyList* m_pArgList;
};
