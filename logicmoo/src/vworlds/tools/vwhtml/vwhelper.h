// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include <resource.h>

#include <propbase.h>
#include <atlcom.h>
#include <atlctl.h>

EXTERN_C const CLSID CLSID_VWHtmlHelper;

class ATL_NO_VTABLE CVWHtmlHelperObject :
 	public IDispatchImpl<IVWHtmlHelper, &IID_IVWHtmlHelper, &LIBID_VWSYSTEMLib>,
	public CComObjectRootEx<CComMultiThreadModel>,
    public CComCoClass<CVWHtmlHelperObject, &CLSID_VWHtmlHelper>,
    public CComControl<CVWHtmlHelperObject>,
    public IProvideClassInfo2Impl<&CLSID_VWHtmlHelper, NULL, &LIBID_VWSYSTEMLib>,
    public IPersistStreamInitImpl<CVWHtmlHelperObject>,
    public IPersistStorageImpl<CVWHtmlHelperObject>,
    public IQuickActivateImpl<CVWHtmlHelperObject>,
	public IOleControlImpl<CVWHtmlHelperObject>,
    public IOleObjectImpl<CVWHtmlHelperObject>,
    public IOleInPlaceActiveObjectImpl<CVWHtmlHelperObject>,
    public IViewObjectExImpl<CVWHtmlHelperObject>,
    public IOleInPlaceObjectWindowlessImpl<CVWHtmlHelperObject>,
    public IDataObjectImpl<CVWHtmlHelperObject>,
	public ISupportErrorInfoImpl<&IID_IVWHtmlHelper>,
    public ISpecifyPropertyPagesImpl<CVWHtmlHelperObject>,
    public IObjectSafetyImpl<CVWHtmlHelperObject, INTERFACESAFE_FOR_UNTRUSTED_CALLER>
{
DECLARE_AGGREGATABLE(CVWHtmlHelperObject)

public:
	CVWHtmlHelperObject();
	~CVWHtmlHelperObject();

BEGIN_COM_MAP(CVWHtmlHelperObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWHtmlHelper)
    COM_INTERFACE_ENTRY(IViewObjectEx)
    COM_INTERFACE_ENTRY_IID(IID_IViewObject2, IViewObjectEx)
    COM_INTERFACE_ENTRY_IID(IID_IViewObject, IViewObjectEx)
    COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
    COM_INTERFACE_ENTRY_IID(IID_IOleInPlaceObject, IOleInPlaceObjectWindowless)
    COM_INTERFACE_ENTRY_IID(IID_IOleWindow, IOleInPlaceObjectWindowless)
    COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
    COM_INTERFACE_ENTRY(IOleControl)
    COM_INTERFACE_ENTRY(IOleObject)
    COM_INTERFACE_ENTRY(IQuickActivate)
    COM_INTERFACE_ENTRY(IPersistStorage)
    COM_INTERFACE_ENTRY(IPersistStreamInit)
    COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
    COM_INTERFACE_ENTRY(IDataObject)
    COM_INTERFACE_ENTRY(IProvideClassInfo)
    COM_INTERFACE_ENTRY(IProvideClassInfo2)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

BEGIN_PROPERTY_MAP(CVWHtmlHelperObject)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	PROP_PAGE(CLSID_StockColorPage)
END_PROPERTY_MAP()

//BEGIN_MSG_MAP(CPubControl)
BEGIN_MSG_MAP(CVWHtmlHelperObject)
END_MSG_MAP()

//DECLARE_NOT_AGGREGATABLE(CVWHtmlHelperObject) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

DECLARE_REGISTRY(CVWHtmlHelperObject, _T("VWSYSTEM.HtmlHelper.1"), _T("VWSYSTEM.HtmlHelper"), IDS_VWHTMLHELPER_DESC, THREADFLAGS_BOTH)

// IVWHtmlHelper
public:
	STDMETHOD(get_Client)(IVWClient **ppclient);
	STDMETHOD(get_Arg)(IDispatch **ppdispArg);
	STDMETHOD(CloseWindow)();
	STDMETHOD(GetShortPathName)(BSTR bstrLongName, BSTR *pbstrShortName);

protected:
	HRESULT GetPtrsFromContainer();

	IVWClient *	m_pclient;
	IDispatch *	m_pdispArg;
};
/////////////////////////////////////////////////////////////////////////////
