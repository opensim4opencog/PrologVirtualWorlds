// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

	
// DServ.h : Declaration of the CDServ
#ifndef __DSERV_H_
#define __DSERV_H_

#include "resource.h"       // main symbols
#include "attrib.h"

EXTERN_C const CLSID CLSID_DServ;


/////////////////////////////////////////////////////////////////////////////
// CDServ
class ATL_NO_VTABLE CDServ : 
	public IDispatchImpl<IDServ, &IID_IDServ, &LIBID_VWSYSTEMLib>,
	public ISupportErrorInfo,
	public IObjectSafetyImpl<CDServ, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CDServ, &CLSID_DServ>
{
DECLARE_AGGREGATABLE(CDServ)

public:
	CDServ();
	~CDServ();

DECLARE_REGISTRY_RESOURCEID(IDR_DSERV)

BEGIN_COM_MAP(CDServ)
	COM_INTERFACE_ENTRY(IDServ)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CDServ)
END_CONNECTION_POINT_MAP()

	HRESULT FinalConstruct();

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IDServ
public:
	STDMETHOD(GetStatus)(BSTR *bstrMsg, VARIANT_BOOL *pbIsDSConnected);
	STDMETHOD(ConnectEx)(BSTR bstrLocation,BSTR bstrAdmin, BSTR bstrPasswd,HWND hNotificationWindow);
	STDMETHOD(FindUser)(BSTR inworldalias,BSTR *emailname, BSTR *computername,BSTR *comment,BSTR *worldname);
	STDMETHOD(LogUserOff)(BSTR InWorldAlias);
	STDMETHOD(Disconnect)();
	STDMETHOD(ConnectEx2)(HWND hNotificationWindow, BSTR bstrLDAP);
	STDMETHOD(LogUserOn)(BSTR InWorldAlias);
	STDMETHOD(RemoveUser)(BSTR InWorldAlias);
	STDMETHOD(AddUser)(BSTR EmailName, BSTR InWorldAlias, BSTR ComputerName, BSTR Comment,
		BSTR computername);
	STDMETHOD(TestQueue)();
	STDMETHOD(EnumerateUsers)(HWND hNotificationWindow);
	STDMETHOD(GetNextUser)(BSTR *inworldalias,BSTR *emailname, BSTR *computername,BSTR *comment,BSTR *worldname);
	STDMETHOD(IsEnumerating)(VARIANT_BOOL *pbool)
	{
//		if(m_UserAttribs.IsEnumerating()) *pbool=VARIANT_TRUE; else *pbool=VARIANT_FALSE; 
		return S_OK;
	}

	STDMETHOD(AddWorld)(BSTR bstrWorldGUID, BSTR bstrWorldURL, BSTR bstrWorldNoUsers, BSTR bstrInfoURL);
	STDMETHOD(RemoveWorld)(BSTR bstrWorldGUID);

private:
	//helper fcns.
	
	//data
	CDSAttribMap *m_pWorldAttribs;
};

CString str(long);
#endif //__DSERV_H_
