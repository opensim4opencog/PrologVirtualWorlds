// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWAdminObj.h : Declaration of the CVWAdminObject

#include <resource.h>

#include <propbase.h>

EXTERN_C const CLSID CLSID_VWAdmin;

/////////////////////////////////////////////////////////////////////////////
// vwclient

// REVIEW -- using pointers to ID's is necessary because some compilers don't like
// references as template arguments.

class ATL_NO_VTABLE CVWAdminObject :
	public IDispatchImpl<IVWAdmin, &IID_IVWAdmin, &LIBID_VWSYSTEMLib>,
	public ISupportErrorInfo,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVWAdminObject, &CLSID_VWAdmin>
{
DECLARE_AGGREGATABLE(CVWAdminObject)

public:
	CVWAdminObject();
	~CVWAdminObject();

BEGIN_COM_MAP(CVWAdminObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWAdmin)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

DECLARE_REGISTRY(CLSID_VWAdmin, "VWSYSTEM.Admin.1", "VWSYSTEM.Admin", IDS_VWADMIN_DESC, THREADFLAGS_BOTH)

// IVWAdmin
public:
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IVWAdmin

public:
	STDMETHOD(Terminate)();	
};
