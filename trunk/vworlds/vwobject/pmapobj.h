// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// PropMapObj.h : Declaration of the CPropMapObject

#include <resource.h>

#include <afxtempl.h>

// V-Worlds optimized templates
#include <vwtempl.h>

#include <PropBase.h>
#include <objimpl.h>

EXTERN_C const CLSID CLSID_PropertyMap;
EXTERN_C const CLSID CLSID_PropertySecurity;

typedef CVWComPtr<IPropertySecurity, &CLSID_PropertySecurity, &IID_IPropertySecurity> CComSecurityPtr;

#if 0
// force debugging of IPropertyMap creation/destruction
#undef TRACE_ALLOCATION

#define TRACE_ALLOCATION VWT_NONE
//#define TRACE_ALLOCATION VWT_METHOD
#endif

#ifdef _DEBUG
//#define TRACE_MAP
#endif

/////////////////////////////////////////////////////////////////////////////
// VWOBJECT

//REVIEW -- using pointers to ID's is necessary because some compilers don't like
//references as template arguments.

DECLARE_OBJECTPROPERTY(PropertyMap, LIBID_VWSYSTEMLib, IDS_PROPMAP_TYPE)
{
	DECLARE_AGGREGATABLE(CPropertyMapObject)

public:
BEGIN_COM_MAP(CPropertyMapObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectProperty)
	COM_INTERFACE_ENTRY(IPropertyCollection)
	COM_INTERFACE_ENTRY(IPropertyMap)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

	DECLARE_REGISTRY(CLSID_PropertyMap,  "VWSYSTEM.PropertyMap.1", "VWSYSTEM.PropertyMap", IDS_PROPMAP_DESC, THREADFLAGS_BOTH)

public:
	CPropertyMapObject() : m_curpos(NULL)
	{ 
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CPropertyMapObject::CPropertyMapObject, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif
	};

	~CPropertyMapObject() 
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CPropertyMapObject::~CPropertyMapObject, this 0x%0x\n", this);
#endif

		Terminate();		
	};

	// Base class overrides
	STDMETHOD(get_RefCount)(DWORD *pdwRef);
	STDMETHOD(ToAscii)(BSTR* pbstr);
	STDMETHOD(Dump)(void);
	STDMETHOD(Marshall)(DWORD dwOptions, IMarshallBuffer* pbuffer);
	STDMETHOD(UnMarshall)(IUnMarshallBuffer* pbuffer);
	STDMETHOD(MarshallToScript)(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut);

	STDMETHOD(Terminate)(void)
	{
#ifdef _DEBUG
		CComBSTR bstrName("n/a");

		if (m_pParent)
		{
			m_pParent->get_Name(&bstrName.m_str);

			VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CPropertyMapObject::Terminate, %s.%s, this 0x%0x\n", CString(bstrName), CString(m_bstrPropertyName), this);
		}
#endif

#ifdef TRACE_MAP
		m_varMap.TraceMap();
#endif

		EmptyMap();

		CPropertyMapObjectBase::Terminate();

		return S_OK;
	};

// IPropertyMap
public:
	STDMETHOD(IsValid)(BSTR name, VARIANT_BOOL* pbool);

	STDMETHOD(get_Property)(BSTR name, VARIANT* pvar);
	STDMETHOD(get_PropertyExt)(BSTR name, UINT nHashOrig, VARIANT* pvar);

	STDMETHOD(put_Property)(BSTR name, VARIANT var);
	STDMETHOD(put_PropertyInt)(BSTR name, VARIANT var, IPropertySecurity* pSecurity);

	STDMETHOD(get_PropertySecurity)(BSTR name, IPropertySecurity **pSecurity);
	STDMETHOD(get_PropertySecurityExt)(BSTR name, UINT nHashOrig, IPropertySecurity **pSecurity);

	STDMETHOD(get_String)(BSTR name, BSTR* bstr);
	STDMETHOD(put_String)(BSTR name, BSTR bstr);

	STDMETHOD(get_ObjectPropertyExt)(BSTR name, REFIID iid, IObjectProperty** ppobjectproperty);
	STDMETHOD(put_ObjectPropertyExt)(BSTR name, REFIID iid, IObjectProperty* pobjectproperty);

	STDMETHOD(get_ObjectProperty)(BSTR name, IObjectProperty** ppobjectproperty);
	STDMETHOD(put_ObjectProperty)(BSTR name, IObjectProperty* pobjectproperty);

	STDMETHOD(get_Thing)(BSTR name, IThing** ppthing);
	STDMETHOD(put_Thing)(BSTR name, IThing* pthing);

	STDMETHOD(get_BOOL)(BSTR name, VARIANT_BOOL* pboolVal);
	STDMETHOD(put_BOOL)(BSTR name, VARIANT_BOOL boolVal);

	STDMETHOD(get_Long)(BSTR name, long* plval);
	STDMETHOD(put_Long)(BSTR name, long lval);

	STDMETHOD(get_Short)(BSTR name, short* psval);
	STDMETHOD(put_Short)(BSTR name, short sval);

	STDMETHOD(get_Dispatch)(BSTR name, IDispatch** ppdisp);
	STDMETHOD(put_Dispatch)(BSTR name, IDispatch* pdisp);

	STDMETHOD(Remove)(BSTR bstr);
	STDMETHOD(RemoveInt)(BSTR bstr);

	STDMETHOD(RemoveAll)();

	STDMETHOD(get_IsEmpty)(VARIANT_BOOL* pboolVal)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CPropertyMapObject::get_IsEmpty\n");

		ASSERT(pboolVal);
		if (pboolVal == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::get_IsEmpty: outgoing pointer (VARIANT_BOOL*) is NULL\n");
			return E_POINTER;
		}

		*pboolVal = m_varMap.IsEmpty() ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}

	STDMETHOD(get_Count)(long* plval)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CPropertyMapObject::get_IsEmpty\n");

		ASSERT(plval);
		if (plval == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::get_IsEmpty: outgoing pointer (long*) is NULL\n");
			return E_POINTER;
		}

		*plval = m_varMap.GetCount();

		return S_OK;
	}

	STDMETHOD(FirstItem)(BSTR* name, VARIANT* pvar, VARIANT_BOOL* pbool);
	STDMETHOD(NextItem)(BSTR* name, VARIANT* pvar, VARIANT_BOOL* pbool);

	STDMETHOD(CopyTo)(IPropertyMap* ppropertymap);

	STDMETHOD(get_PropertyAtIndex)(long index, VARIANT *pvar);
	STDMETHOD(get__NewEnum)(IUnknown **punk);

private:
	void EmptyMap();

	CVWMap<CBSTRAdapt<CComBSTR>, BSTR, CComSecurityPtr, CComSecurityPtr&> m_varMap;

	POSITION m_curpos;

	// helpers for IDispatch overriding
	BOOL IsDoctoredId( DISPID id, VARIANT_BOOL *pbProp );
	HRESULT IdFromName( LPOLESTR psz, DISPID* pid);
	HRESULT NameFromId( DISPID id, BSTR *pbstr);
	CComBSTR m_bstrCached;
	DISPID m_idCached;
	DISPID m_idNext;
};
