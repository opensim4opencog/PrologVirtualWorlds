// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// CopyTool.h : Declaration of the CCopyTool


#include "resource.h"       // main symbols
#include "uitlimpl.h"

EXTERN_C const IID LIBID_VWSTUDIOLib;
EXTERN_C const CLSID CLSID_CopyTool;

/////////////////////////////////////////////////////////////////////////////
// Stdtools

DECLARE_VWUITOOLEX(CCopyToolObject, ICopyTool, IID_ICopyTool, CLSID_CopyTool, LIBID_VWSTUDIOLib)
{
public:
BEGIN_COM_MAP(CCopyToolObject)
	COM_INTERFACE_ENTRY2(IDispatch, ICopyTool)
	COM_INTERFACE_ENTRY(IVWUITool)
	COM_INTERFACE_ENTRY(ICopyTool)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

//DECLARE_NOT_AGGREGATABLE(CCopyToolObject) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

DECLARE_REGISTRY_RESOURCEID(IDR_CopyTool)

// ICopyTool

public:
};
