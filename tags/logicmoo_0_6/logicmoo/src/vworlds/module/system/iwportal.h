// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// Portal.h : Declaration of the CPortal

#ifndef __IWPORTAL_H_
#define __IWPORTAL_H_

#include "resource.h"       // main symbols
#include "objimpl.h"

/////////////////////////////////////////////////////////////////////////////
// CPortal

EXTERN_C const CLSID CLSID_IWPortalExemplar;

#define ReportIWPortalError(hr) SUCCEEDED(hr) ? hr : ReportError("VWSYSTEM.IWPortal.1", hr, 0xc50, 0xc60, "VWIWPORTAL")

DECLARE_VWDISPATCH( IWPortalExemplar, LIBID_VWSYSTEMLib )
{
public:
DECLARE_REGISTRY_RESOURCEID(IDR_IWPORTALEXEMPLAR)

BEGIN_COM_MAP(CIWPortalExemplarObject)
	COM_INTERFACE_ENTRY(IIWPortalExemplar)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

// IIWPortal
public:
	STDMETHOD(Install)(IModule* pModule);
	STDMETHOD(Transport)(IThing *pThing, int iIndex);
};

#endif //__IWPORTAL_H_
