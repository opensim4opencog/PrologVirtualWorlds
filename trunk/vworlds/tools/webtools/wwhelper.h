// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// WWHelper.h : Declaration of the CWorldWizHelper

#ifndef __WORLDWIZHELPER_H_
#define __WORLDWIZHELPER_H_

#include "resource.h"       // main symbols
#include <afxtempl.h>
#include <atlctl.h>

EXTERN_C const IID LIBID_VWSTUDIOLib;
EXTERN_C const CLSID CLSID_WorldWizHelper;

/////////////////////////////////////////////////////////////////////////////
// CWorldWizHelper
class ATL_NO_VTABLE CWorldWizHelper : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CWorldWizHelper, &CLSID_WorldWizHelper>,
	public ISupportErrorInfo,
	public IDispatchImpl<IWorldWizHelper, &IID_IWorldWizHelper, &LIBID_VWSTUDIOLib>,
	public IObjectSafetyImpl<CWorldWizHelper, INTERFACESAFE_FOR_UNTRUSTED_CALLER>
{
public:
	CWorldWizHelper()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_WORLDWIZHELPER)

BEGIN_COM_MAP(CWorldWizHelper)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IWorldWizHelper)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IWorldWizHelper
public:
	STDMETHOD(initialize)();
	STDMETHOD(get_TypeNames)(/*[out, retval]*/ VARIANT *pVal);

protected:
	CList<CString, CString>	m_typeNames;
};

#endif //__WORLDWIZHELPER_H_
