// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _COMMSITE_H_
#define _COMMSITE_H_

class CVWServer;

/////////////////////////////////////////////////////////////////////////////
// CVWCommSite

class CVWCommSite: public IVWCommSite
{
public:
	CVWCommSite(CVWServer *pServer);

// IUnknown
	STDMETHOD(QueryInterface)(REFIID iid, void **ppvObject);
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();

// IVWCommSite
	STDMETHOD(OnMessageAvailable)();
	STDMETHOD(OnConnect_T2)(IVWCommConnection *pICommConnection);
	STDMETHOD(OnDisconnect_T2)(IVWCommConnection *pICommConnection);
	STDMETHOD(OnError_T2)(IVWCommConnection *pICommConnection, int nErrorCode );
	STDMETHOD(OnMessageAvailable_T2)();

protected:
	CVWServer *	m_pServer;
	ULONG		m_cRef;
};
	
#endif