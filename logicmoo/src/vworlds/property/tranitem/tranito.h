// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// TranIto.h : Declaration of the CVWTransactionItemObject

#include <resource.h>
#include <PropBase.h>
#include <objimpl.h>

EXTERN_C const CLSID CLSID_VWTransactionItem;
EXTERN_C const IID LIBID_VWFOUNDLib;

/////////////////////////////////////////////////////////////////////////////
// VWTransactionItem

DECLARE_OBJECTPROPERTY(VWTransactionItem, LIBID_VWFOUNDLib, IDS_VWTRANSACTIONITEMOBJECT_TYPE)
{
	DECLARE_AGGREGATABLE(CVWTransactionItemObject)

public:
	CVWTransactionItemObject();
	~CVWTransactionItemObject();

BEGIN_COM_MAP(CVWTransactionItemObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectProperty)
	COM_INTERFACE_ENTRY(IVWTransactionItem)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

//DECLARE_NOT_AGGREGATABLE(CVWTransactionItemObject) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

	DECLARE_REGISTRY(CVWTransactionItemObject, _T("VWSYSTEM.TransactionItem.1"), _T("VWSYSTEM.TransactionItem"), IDS_VWTRANSACTIONITEMOBJECT_DESC, THREADFLAGS_BOTH)

	// Override the base class
	STDMETHOD(put_World)(IWorld* pworld);
	STDMETHOD(get_RefCount)(DWORD *pdwRef);
	STDMETHOD(Marshall)(DWORD dwOptions, IMarshallBuffer* pbuffer); 
	STDMETHOD(MarshallToScript)(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut);
	STDMETHOD(UnMarshall)(IUnMarshallBuffer* pbuffer); 
	STDMETHOD(Terminate)(void);


	// IVWTransactionItem methods
	STDMETHOD(get_Name)(BSTR *pbstrName);
	STDMETHOD(put_Name)(BSTR bstrName);
	STDMETHOD(get_Description)(BSTR *pbstrDescription);
	STDMETHOD(put_Description)(BSTR bstrGraphic);
	STDMETHOD(get_Graphic)(BSTR *pbstrGraphic);
	STDMETHOD(put_Graphic)(BSTR bstrGraphic);
	STDMETHOD(get_ExemplarType)(BSTR *pbstrExemplarType);
	STDMETHOD(put_ExemplarType)(BSTR bstrExemplarType);
	STDMETHOD(get_PropertyValues)(IPropertyMap **pPropertyMap);
	STDMETHOD(put_PropertyValues)(IPropertyMap **pPropertyMap);

private:
// Local helper data and funcs.

	CComBSTR		m_bstrName;
	CComBSTR		m_bstrDescription;
	CComBSTR		m_bstrGraphic;
	CComBSTR		m_bstrExemplarType;
	IPropertyMap	*m_pPropertyValues;
};

	
	
	
