// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// StatMgr.h : Declaration of the CVWStatColObject

#include <resource.h>
#include <propbase.h>

EXTERN_C const CLSID CLSID_VWStatCol;

/////////////////////////////////////////////////////////////////////////////
// vwstats

class ATL_NO_VTABLE CVWStatColObject :
	public IDispatchImpl<IVWStatCol, &IID_IVWStatCol, &LIBID_VWSYSTEMLib>,
	public ISupportErrorInfo,
	public IObjectSafetyImpl<CVWStatColObject, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVWStatColObject, &CLSID_VWStatCol>
{
	DECLARE_AGGREGATABLE(CVWStatColObject)

public:
BEGIN_COM_MAP(CVWStatColObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWStatCol)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

//DECLARE_NOT_AGGREGATABLE(CVWStatColObject) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

DECLARE_REGISTRY(CVWStatColObject, _T("VWSYSTEM.StatMgr.1"), _T("VWSYSTEM.StatMgr"), IDS_VWSTATMGR_DESC, THREADFLAGS_BOTH)

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IVWStatMgr
public:
	CVWStatColObject(): m_pPropMap(NULL), m_pWorld(NULL) {}
	~CVWStatColObject(); 

	STDMETHOD(Initialize)(IWorld *pworld); 
	
	STDMETHOD(get_Item)( BSTR category, BSTR name, VARIANT* pRetVar);
	STDMETHOD(put_Item)( BSTR category, BSTR name, VARIANT var);
	
	STDMETHOD(get_LongItem)(BSTR category, BSTR name, long * piRet);
	STDMETHOD(put_LongItem)(BSTR category, BSTR name, long inp);
	
	STDMETHOD(get_BSTRItem)(BSTR category, BSTR name, BSTR * pbstrRet);
	STDMETHOD(put_BSTRItem)(BSTR category, BSTR name, BSTR bstr);
	
	STDMETHOD(RemoveItem)(BSTR category, BSTR variable);
	
	STDMETHOD(get_Count)(long * retval);

	STDMETHOD(get_PropertyMap)(BSTR category, IPropertyMap **ppRetMap);

private:
	IPropertyMap *m_pPropMap;
	IWorld *m_pWorld;
};
