// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// CutTool.h : Declaration of the CCutTool


#include "resource.h"       // main symbols

EXTERN_C const IID LIBID_VWSTUDIOLib;
EXTERN_C const CLSID CLSID_CCutTool;

/////////////////////////////////////////////////////////////////////////////
// Stdtools

class CCutTool : 
	public CComDualImpl<ICutTool, &IID_ICutTool, &LIBID_VWSTUDIOLib>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<CCutTool,&CLSID_CCutTool>
{
public:
	CCutTool() {}
BEGIN_COM_MAP(CCutTool)
	COM_INTERFACE_ENTRY2(IDispatch, ICutTool)
	COM_INTERFACE_ENTRY(ICutTool)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(CCutTool) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

DECLARE_REGISTRY_RESOURCEID(IDR_CutTool)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// ICutTool

// ICutToolInt2
public:
};
