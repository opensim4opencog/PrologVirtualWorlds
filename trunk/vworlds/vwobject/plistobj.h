// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// PropListObj.h : Declaration of the CPropListObject

#include <resource.h>

#include <afxtempl.h>

#include <PropBase.h>
#include <objimpl.h>

#if 0
// force debugging of IPropertyList creation/destruction
#undef TRACE_ALLOCATION

#define TRACE_ALLOCATION VWT_NONE
//#define TRACE_ALLOCATION VWT_METHOD
#endif

EXTERN_C const CLSID CLSID_PropertyList;
EXTERN_C const CLSID CLSID_PropertySecurity;

/////////////////////////////////////////////////////////////////////////////
// VWOBJECT

//REVIEW -- using pointers to ID's is necessary because some compilers don't like
//references as template arguments.

typedef CVWComPtr< IPropertySecurity, &CLSID_PropertySecurity, &IID_IPropertySecurity > CComSecurityPtr;

DECLARE_OBJECTPROPERTY(PropertyList, LIBID_VWSYSTEMLib, IDS_PROPLIST_TYPE)
{
DECLARE_AGGREGATABLE(CPropertyListObject)

public:
BEGIN_COM_MAP(CPropertyListObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectProperty)
	COM_INTERFACE_ENTRY(IPropertyCollection)
	COM_INTERFACE_ENTRY(IPropertyList)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

DECLARE_REGISTRY(CLSID_PropertyList, "VWSYSTEM.PropertyList.1", "VWSYSTEM.PropertyList", IDS_PROPLIST_DESC, THREADFLAGS_BOTH)

public:
	CPropertyListObject() : m_curpos(0)
	{ 
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CPropertyListObject::CPropertyListObject, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif
	};

	~CPropertyListObject() 
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CPropertyListObject::~CPropertyListObject, this 0x%0x\n", this);
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

			VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CPropertyListObject::Terminate, %s.%s, this 0x%0x\n", CString(bstrName), CString(m_bstrPropertyName), this);
		}
#endif

		EmptyList();

		CPropertyListObjectBase::Terminate();

		return S_OK;
	}


// IPropertyList
public:
	STDMETHOD(Add)(VARIANT var);
	STDMETHOD(AddString)(BSTR str);
	STDMETHOD(AddObjectPropertyExt)(REFIID iid, IObjectProperty* pobjectproperty);
	STDMETHOD(AddObjectProperty)(IObjectProperty* pobjectproperty);
	STDMETHOD(AddThing)(IThing* pthing);
	STDMETHOD(AddBOOL)(VARIANT_BOOL boolVal);
	STDMETHOD(AddLong)(long lval);
	STDMETHOD(AddShort)(short sval);
	STDMETHOD(AddDispatch)(IDispatch* pdisp);

	STDMETHOD(InsertAt)(long index, VARIANT var);
	STDMETHOD(InsertStringAt)(long index, BSTR bstr);
	STDMETHOD(InsertObjectPropertyExtAt)(long index, REFIID iid, IObjectProperty* pobjectproperty);
	STDMETHOD(InsertObjectPropertyAt)(long index, IObjectProperty* pobjectproperty);
	STDMETHOD(InsertThingAt)(long index, IThing* pthing);
	STDMETHOD(InsertBOOLAt)(long index, VARIANT_BOOL boolVal);
	STDMETHOD(InsertLongAt)(long index, long lval);
	STDMETHOD(InsertShortAt)(long index, short sval);
	STDMETHOD(InsertDispatchAt)(long index, IDispatch* pdisp);

	STDMETHOD(Remove)(VARIANT var);
	STDMETHOD(RemoveString)(BSTR bstr);
	STDMETHOD(RemoveObjectPropertyExt)(REFIID iid, IObjectProperty* pobjectproperty);
	STDMETHOD(RemoveObjectProperty)(IObjectProperty* pobjectproperty);
	STDMETHOD(RemoveThing)(IThing* pthing);
	STDMETHOD(RemoveBOOL)(VARIANT_BOOL boolVal);
	STDMETHOD(RemoveLong)(long lval);
	STDMETHOD(RemoveShort)(short sval);
	STDMETHOD(RemoveDispatch)(IDispatch* pdisp);

	STDMETHOD(RemoveAt)(long index);
	STDMETHOD(RemoveAll)();

	STDMETHOD(Find)(VARIANT var, long* plval);

	STDMETHOD(get_IsEmpty)(VARIANT_BOOL* pboolVal)
	{
		ASSERT(pboolVal);
		if (pboolVal == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::get_IsEmpty: outgoing pointer (VARIANT_BOOL*) is NULL\n");
			return E_POINTER;
		}

		*pboolVal = (m_varList.GetSize() == 0) ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}

	STDMETHOD(get_Count)(long* plval)
	{
		ASSERT(plval);
		if (plval == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::get_Count: outgoing pointer (long*) is NULL\n");
			return E_POINTER;
		}

		*plval = m_varList.GetSize();

		return S_OK;
	}

	STDMETHOD(get_Property)(long index, VARIANT* pvar);
	STDMETHOD(get_String)(long index, BSTR* pbstr);
	STDMETHOD(get_ObjectPropertyExt)(long index, REFIID iid, IObjectProperty** ppobjectproperty);
	STDMETHOD(get_ObjectProperty)(long index, IObjectProperty** ppobjectproperty);
	STDMETHOD(get_Thing)(long index, IThing** ppthing);
	STDMETHOD(get_BOOL)(long index, VARIANT_BOOL* pboolVal);
	STDMETHOD(get_Long)(long index, long* plval);
	STDMETHOD(get_Short)(long index, short* psval);
	STDMETHOD(get_Dispatch)(long index, IDispatch** ppdisp);

	STDMETHOD(put_Property)(long index, VARIANT var);
	STDMETHOD(put_String)(long index, BSTR bstr);
	STDMETHOD(put_ObjectPropertyExt)(long index, REFIID iid, IObjectProperty* pobjectproperty);
	STDMETHOD(put_ObjectProperty)(long index, IObjectProperty* pobjectproperty);
	STDMETHOD(put_Thing)(long index, IThing* pthing);
	STDMETHOD(put_BOOL)(long index, VARIANT_BOOL boolVal);
	STDMETHOD(put_Long)(long index, long lval);
	STDMETHOD(put_Short)(long index, short sval);
	STDMETHOD(put_Dispatch)(long index, IDispatch* pdisp);

	STDMETHOD(FirstItem)(BSTR* name, VARIANT* pvar, VARIANT_BOOL* pbool);
	STDMETHOD(NextItem)(BSTR* name, VARIANT* pvar, VARIANT_BOOL* pbool);

	STDMETHOD(get_PropertySecurity)(long index,IPropertySecurity** ppSecurity);

	STDMETHOD(CopyTo)(IPropertyList* ppropertylist);

	STDMETHOD(get_PropertyAtIndex)(long index, VARIANT *pvar);
	STDMETHOD(get__NewEnum)(IUnknown **punk);

private:
	void EmptyList();

	CArray<CComSecurityPtr, const CComSecurityPtr&> m_varList;

	int m_curpos;

};
