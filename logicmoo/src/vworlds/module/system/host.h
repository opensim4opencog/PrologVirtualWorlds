// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// Host.h : Declaration of the CHost

#ifndef __HOST_H_
#define __HOST_H_

#include "resource.h"       // main symbols
#include "objimpl.h"

/////////////////////////////////////////////////////////////////////////////
// CHost

EXTERN_C const CLSID CLSID_HostExemplar;

#define ReportHostError(hr) SUCCEEDED(hr) ? hr : ReportError("VWSYSTEM.Host.1", hr, 0xc20, 0xc30, "VWHOST")

DECLARE_VWDISPATCH(HostExemplar, LIBID_VWSYSTEMLib )
{
public:

DECLARE_REGISTRY_RESOURCEID(IDR_HOSTEXEMPLAR)

BEGIN_COM_MAP(CHostExemplarObject)
	COM_INTERFACE_ENTRY(IHostExemplar)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

// IHost
public:
	STDMETHOD(Install)(IModule* pModule);
	STDMETHOD(ShowLogOffDialog)(IThing * pTarget);
	STDMETHOD(LogOff)(IThing * pTarget, BSTR bstrReason);
};

#endif //__HOST_H_
