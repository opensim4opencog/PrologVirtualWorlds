// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _SERVSINKHEADER
#define _SERVSINKHEADER
#include "..\vwcommon\vwserver.h"

class CServerV2App;

class CServerSink:	
	public CComObjectRootEx<CComMultiThreadModel>,
	public DVWServerEvents
{
public:
	CServerSink() {}

BEGIN_COM_MAP(CServerSink)
	COM_INTERFACE_ENTRY(DVWServerEvents)
END_COM_MAP()

	void SetApp(CServerV2App *papp) { m_pApp = papp;}

// DVWServerEvents
	STDMETHOD(OnWorldLoaded)(BSTR bstrWorld);
	STDMETHOD(OnWorldUnloaded)(BSTR bstrWorld);

	STDMETHOD(OnConnect)(DWORD hConnect);
	STDMETHOD(OnDisconnect)(DWORD hDisconnect);

	STDMETHOD(OnEnterWorld)(DWORD hConnect, BSTR bstrWorld);
	STDMETHOD(OnLeaveWorld)(DWORD hConnect, BSTR bstrWorld);

	STDMETHOD(OnCommError)(DWORD hConnect);
	STDMETHOD(OnCommMessage)(DWORD hConnect, BSTR bstrMessage);

	STDMETHOD(OnReport)(BSTR bstr, long lType);
	STDMETHOD(OnTrace)(BSTR bstr);

	STDMETHOD(OnAvailableWorld)(BSTR bstrName, int iNoUsers); 

protected:
	CServerV2App *m_pApp;
	//void Log( int idEvent, LPCSTR pszMessage);
};

#endif