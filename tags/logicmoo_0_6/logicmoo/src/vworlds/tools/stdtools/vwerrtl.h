// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// CopyTool.h : Declaration of the CCopyTool

#include "resource.h"       // main symbols

EXTERN_C const IID LIBID_VWSTUDIOLib;
EXTERN_C const CLSID CLSID_VWErrorTool;

/////////////////////////////////////////////////////////////////////////////
// Stdtools

class CVWErrorToolObject : 
	public CComDualImpl<IVWErrorTool, &IID_IVWErrorTool, &LIBID_VWSTUDIOLib>, 
	public CComObjectRoot,
	public CComCoClass<CVWErrorToolObject,&CLSID_VWErrorTool>
{
public:
	CVWErrorToolObject() {		 }
	~CVWErrorToolObject() {		 }
BEGIN_COM_MAP(CVWErrorToolObject)
	COM_INTERFACE_ENTRY2(IDispatch, IVWErrorTool)
	COM_INTERFACE_ENTRY(IVWErrorTool)
END_COM_MAP()

DECLARE_REGISTRY_RESOURCEID(IDR_VWErrorTool)

// IVWErrorTool
STDMETHOD (DisplayError)(HRESULT hr, VARIANT_BOOL *pbHandled);

void ShowError(UINT nID);

// IVWErrorToolInt2
public:
};
