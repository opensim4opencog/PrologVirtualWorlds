// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// FileAcTl.h : Declaration of the CVWFileAccessTool

#ifndef __VWFILEACCESSTOOL_H_
#define __VWFILEACCESSTOOL_H_

#include "resource.h"       // main symbols
#include <afxtempl.h>
#include <atlctl.h>

/////////////////////////////////////////////////////////////////////////////
// CVWFileAccessTool
class ATL_NO_VTABLE CVWFileAccessTool : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVWFileAccessTool, &CLSID_VWFileAccessTool>,
//	public IObjectSafetyImpl<CVWFileAccessTool, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
	public IDispatchImpl<IVWFileAccessTool, &IID_IVWFileAccessTool, &LIBID_VWSTUDIOLib>
{
public:
	CVWFileAccessTool()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_VWFILEACCESSTOOL)

BEGIN_COM_MAP(CVWFileAccessTool)
	COM_INTERFACE_ENTRY(IVWFileAccessTool)
	COM_INTERFACE_ENTRY(IDispatch)
//	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

// IVWFileAccessTool
public:
	STDMETHOD(ReadFile)(BSTR bstrFilename, BSTR* bstrFileText);
	STDMETHOD(WriteFile)(BSTR bstrFilename, BSTR bstrFileText);
	STDMETHOD(SelectColor)(long dwInitialColor, long* pdwPickedColor);
	STDMETHOD(OpenFileDialog)(BSTR bstrDefaultFilename, long dwFileFlags, BSTR bstrFileFilter, BSTR *pbstrFileName);
	STDMETHOD(SaveFileDialog)(BSTR bstrDefaultFilename, long dwFileFlags,  BSTR bstrFileFilter, BSTR *pbstrFileName);
	STDMETHOD(FileExists)(BSTR bstrFileName, VARIANT_BOOL *pbExists);
	STDMETHOD(GetWorldPath)(BSTR *pbstrWorldPath);
	STDMETHOD(GetHelpPath)(BSTR *pbstrHelpPath);
};

#endif //__VWFILEACCESSTOOL_H_
