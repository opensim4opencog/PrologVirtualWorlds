// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// WebPage.h : Declaration of the CHost

#ifndef __WEBPAGE_H_
#define __WEBPAGE_H_

#include "resource.h"       // main symbols
#include "objimpl.h"

/////////////////////////////////////////////////////////////////////////////
// CWebPage

EXTERN_C const CLSID CLSID_WebPageExemplar;

#define ReportWebPageError(hr) SUCCEEDED(hr) ? hr : ReportError("VWSYSTEM.WebPage.1", hr, 0xc90, 0xca0, "VWWEBPAGE")

DECLARE_VWDISPATCH(WebPageExemplar, LIBID_VWSYSTEMLib )
{
public:

DECLARE_REGISTRY_RESOURCEID(IDR_WEBPAGEEXEMPLAR)

BEGIN_COM_MAP(CWebPageExemplarObject)
	COM_INTERFACE_ENTRY(IWebPageExemplar)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

// IHost
public:
	STDMETHOD(Install)(IModule* pModule);
	STDMETHOD(Show)();
	STDMETHOD(OnLButtonUp)(int hwnd, int x, int y, VARIANT_BOOL *pbool);
};

#endif //__WEBPAGE_H_
