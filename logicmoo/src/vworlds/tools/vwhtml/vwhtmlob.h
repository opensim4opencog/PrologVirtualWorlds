// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWHtmlOb.h : Declaration of the CVWHtmlObject

#include <afxtempl.h>

// V-Worlds optimized templates
#include <vwtempl.h>
#include <propbase.h>

#include <atlctl.h>

#include "htmlwnd.h"

EXTERN_C const CLSID CLSID_VWHtml;

class ATL_NO_VTABLE CVWHtmlObject :
	public IDispatchImpl<IVWHtml, &IID_IVWHtml, &LIBID_VWSYSTEMLib>,
	public ISupportErrorInfo,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVWHtmlObject, &CLSID_VWHtml>,
    public IObjectSafetyImpl<CVWHtmlObject, INTERFACESAFE_FOR_UNTRUSTED_CALLER>
{
DECLARE_AGGREGATABLE(CVWHtmlObject)

friend class CHtmlWnd;

public:
	CVWHtmlObject();
	~CVWHtmlObject();

BEGIN_COM_MAP(CVWHtmlObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWHtml)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(CVWHtmlObject) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

DECLARE_REGISTRY(CVWHtmlObject, _T("VWSYSTEM.Html.1"), _T("VWSYSTEM.Html"), IDS_VWHTML_DESC, THREADFLAGS_BOTH)

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IVWHtml
public:
	STDMETHOD(Initialize)(	IVWClient *pclient);
	STDMETHOD(put_RootURL)(BSTR bstrRootURL);
	STDMETHOD(get_RootURL)(BSTR *bstrRootURL);
	STDMETHOD(CloseAll)();
	STDMETHOD(Close)(BSTR bstrName);

	STDMETHOD(ShowHTML)(	int nWnd, 
							BSTR bstrHtml, 
							BSTR bstrTitle,
							VARIANT_BOOL bSafeMode,
							IDispatch *pdispArg,
							int x,
							int y,
							int cx,
							int cy);

	STDMETHOD(ShowHTMLEx)(	BSTR bstrName,
							int nWnd, 
							BSTR bstrHtml, 
							BSTR bstrTitle,
							VARIANT_BOOL bSafeMode,
							IDispatch *pdispArg,
							int x,
							int y,
							int cx,
							int cy);

	STDMETHOD(ShowURL)(		int nWnd, 
							BSTR bstrUrl, 
							BSTR bstrTitle,
							VARIANT_BOOL bSafeMode,
							IDispatch *pdispArg,
							int x,
							int y,
							int cx,
							int cy);

	STDMETHOD(ShowURLEx)(		BSTR bstrName,
								int nWnd, 
								BSTR bstrUrl, 
								BSTR bstrTitle,
								VARIANT_BOOL bSafeMode,
								IDispatch *pdispArg,
								int x,
								int y,
								int cx,
								int cy);

protected:
	IVWClient *	m_pclient;

	CString m_strRootURL;

	// map of window names to html windows
	CVWMap<CBSTRAdapt<CComBSTR>, BSTR, CHtmlWnd*, CHtmlWnd* &> m_mapNameToWnd;

	// map of window names to arg references
	CVWMap<CBSTRAdapt<CComBSTR>, BSTR, IDispatch*, IDispatch* &> m_mapNameToArg;

	// call to bring up window
	HRESULT PrepareWindow(	BSTR bstrName,
							int nWnd,
							BSTR bstrTitle,
							IDispatch *pdispArg,
							int x,
							int y,
							int cx,
							int cy,
							CHtmlWnd **ppwnd);

	void OnHTMLClosed(BSTR bstrName);

};
/////////////////////////////////////////////////////////////////////////////
