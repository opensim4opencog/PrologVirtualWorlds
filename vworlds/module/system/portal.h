// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// Portal.h : Declaration of the CPortal

#ifndef __PORTAL_H_
#define __PORTAL_H_

#include "resource.h"       // main symbols
#include "objimpl.h"

/////////////////////////////////////////////////////////////////////////////
// CPortal

EXTERN_C const CLSID CLSID_PortalExemplar;

#define ReportPortalError(hr) SUCCEEDED(hr) ? hr : ReportError("VWSYSTEM.Portal.1", hr, 0xc00, 0xc10, "VWPORTAL")

DECLARE_VWDISPATCH( PortalExemplar, LIBID_VWSYSTEMLib )
{
public:
DECLARE_REGISTRY_RESOURCEID(IDR_PORTALEXEMPLAR)

BEGIN_COM_MAP(CPortalExemplarObject)
	COM_INTERFACE_ENTRY(IPortalExemplar)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

// IPortal
public:
	STDMETHOD(Install)(IModule* pModule);
	STDMETHOD(ShowTransportDialog)();
	STDMETHOD(TransportMenu)();
	STDMETHOD(Transport)(IThing *pThing, int iIndex);
	STDMETHOD(TransportTo)(IThing *pThing);
	STDMETHOD(AllowTransport)(IThing* pThing, VARIANT_BOOL* pbool);
	STDMETHOD(AcceptTransport)(IThing* pThing, VARIANT_BOOL* pbool);
	STDMETHOD(SetDestinationByID)(long lNewID);		
};

#endif //__PORTAL_H_
