// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// RWHelper.h : Declaration of the CRoomWizHelper

#ifndef __ROOMWIZHELPER_H_
#define __ROOMWIZHELPER_H_

#include "resource.h"       // main symbols
#include <afxtempl.h>
#include <atlctl.h>

EXTERN_C const IID LIBID_VWSTUDIOLib;
EXTERN_C const CLSID CLSID_RoomWizHelper;

/////////////////////////////////////////////////////////////////////////////
// CRoomWizHelper
class ATL_NO_VTABLE CRoomWizHelper : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CRoomWizHelper, &CLSID_RoomWizHelper>,
	public ISupportErrorInfo,
	public IDispatchImpl<IRoomWizHelper, &IID_IRoomWizHelper, &LIBID_VWSTUDIOLib>,
	public IObjectSafetyImpl<CRoomWizHelper, INTERFACESAFE_FOR_UNTRUSTED_CALLER>
{
public:
	CRoomWizHelper()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ROOMWIZHELPER)

BEGIN_COM_MAP(CRoomWizHelper)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IRoomWizHelper)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IRoomWizHelper
public:
	STDMETHOD(initialize)(/* [in] */ BSTR bstrInitialDirectory);
	STDMETHOD(get_TypeNames)(/*[out, retval]*/ VARIANT *pVal);

protected:
	CList<CString, CString>	m_typeNames;
};

#endif //__RoomWizHELPER_H_
