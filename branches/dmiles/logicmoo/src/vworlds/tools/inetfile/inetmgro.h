// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// InetMgrO.h : Declaration of the CInternetFileManagerObject

#include <wininet.h>
#include <resource.h>
#include <propbase.h>
#include <COMDEF.H>
#include <afxtempl.h>
#include <atlctl.h>
#include "dlthread.h"

EXTERN_C const CLSID CLSID_InternetFileManager;

#define ReportInetFileError(hr) SUCCEEDED(hr) ? hr : ReportError("VWSYSTEM.InternetFileManager", IID_IInternetFileManager, hr, 0xd00, 0xe00)

/////////////////////////////////////////////////////////////////////////////
// CInternetFileManagerObject

class ATL_NO_VTABLE CInternetFileManagerObject :
	public IDispatchImpl<IInternetFileManager, &IID_IInternetFileManager, &LIBID_VWSYSTEMLib>,
	public ISupportErrorInfo,
	public IObjectSafetyImpl<CInternetFileManagerObject, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CInternetFileManagerObject, &CLSID_InternetFileManager>
{
DECLARE_AGGREGATABLE(CInternetFileManagerObject)

public:
	~CInternetFileManagerObject();

BEGIN_COM_MAP(CInternetFileManagerObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IInternetFileManager)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
//	COM_INTERFACE_ENTRY(IIFMCallback)
END_COM_MAP()

//DECLARE_NOT_AGGREGATABLE(CInternetFileManager) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

DECLARE_REGISTRY(CInternetFileManagerObject, _T("VWSYSTEM.InternetFileManager.1"), _T("VWSYSTEM.InternetFileManager"), IDS_INETFILEMANAGER_DESC, THREADFLAGS_BOTH)

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IInternetFileManager
public:
	STDMETHOD(GetFileEx)( BSTR bstrURL, VARIANT_BOOL bCheckCache, BSTR *pbstrFinalURL, BSTR *pbstrFinalPath);
	STDMETHOD(GetFile)( BSTR bstrURL, VARIANT_BOOL bCheckCache, BSTR *pbstrFinalPath);
	STDMETHOD(GetFileAsync)(BSTR bstrURL, long lCookie, IIFMCallback *pIFMCallback, VARIANT_BOOL bCheckCache = VARIANT_FALSE);
	STDMETHOD(CancelAllDownloads)();
	STDMETHOD(put_RootURL)(BSTR bstrRootURL);
	STDMETHOD(get_RootURL)(BSTR *bstrRootURL);
	STDMETHOD(FindCachedFile)(BSTR bstrURL, BSTR *pbstrFinalURL,
							  BSTR *pbstrFinalPath, VARIANT_BOOL *pbool);

	STDMETHOD(Terminate)();

	STDMETHOD(FileDLDone)(BSTR bstrFilename);

	STDMETHOD(Test)();

// IIFMCallback
//	STDMETHOD(OnProgress_T2)(ULONG ulProgress, ULONG ulProgressMax,long lCookie);
//	STDMETHOD(OnDone_T2)(HRESULT hresult, BSTR bstrFinalPath,long lCookie);

protected:
	HRESULT GetFileComplex( BSTR bstrURL, BSTR *pbstrFinalURL, BSTR *pbstrFinalPath);
	HRESULT GetFileSimplified( BSTR bstrURL, BSTR *pbstrFinalURL, BSTR *pbstrFinalPath);
	
	//tests getfile.
	void TestHelper(CString blah);

	//data
	CString m_strRootURL;

	CArray<CString, CString &> m_RootURLs;
	
	HINTERNET m_hInternet;

	CThreadMap m_ThreadMap;
};
