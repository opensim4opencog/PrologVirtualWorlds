// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include "resource.h"
#include "vwserver.h"
#include "vwserve.h"
#include "commsite.h"

CVWCommSite::CVWCommSite(CVWServer *pServer)
{
	m_pServer = pServer;
	m_cRef = 0; 
}

HRESULT CVWCommSite::QueryInterface(REFIID iid, void **ppvObj)
{
    if (iid == IID_IUnknown || iid == IID_IVWCommSite)
    {
        *ppvObj = this;
        AddRef();
        return NOERROR;
    }
    return ResultFromScode(E_NOINTERFACE);
}

ULONG CVWCommSite::AddRef()
{ 
    return ++m_cRef;
}


ULONG CVWCommSite::Release()
{
	if (--m_cRef == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}

HRESULT CVWCommSite::OnMessageAvailable()
{
	// process messages 'til done
	IVWCommMessage *pcmm;

	while( SUCCEEDED(m_pServer->m_pcomm->GetMessage(&pcmm)) )
	{
		m_pServer->ProcessMessage(pcmm);
		pcmm->Release();
	}
	return S_OK;
}

HRESULT CVWCommSite::OnConnect_T2(IVWCommConnection *pICommConnection)
{
	return S_OK;
}

HRESULT CVWCommSite::OnDisconnect_T2(IVWCommConnection *pICommConnection)
{
	return S_OK;
}

HRESULT CVWCommSite::OnError_T2(IVWCommConnection *pICommConnection, int nErrorCode )
{
	return S_OK;
}

HRESULT CVWCommSite::OnMessageAvailable_T2()
{
	// set event
	SetEvent( m_pServer->m_heventMessage );
	return S_OK;
}


