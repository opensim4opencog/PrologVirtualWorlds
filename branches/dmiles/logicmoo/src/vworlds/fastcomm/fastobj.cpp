// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// CommObj.cpp : Implementation of CFastCommApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include <vwcomm.h>
#include "fastobj.h"
#include "marshbuf.h"

//#define TRACE_ALLOCATION

typedef CComObject<CFastCommConnection> CComFastCommConnection;

EXTERN_C const CLSID CLSID_UnMarshallBuffer;
EXTERN_C const CLSID CLSID_VWComm;

/////////////////////////////////////////////////////////////////////////////
//

CFastCommObject::CFastCommObject()
{
#ifdef TRACE_ALLOCATION
	TRACE("CFastCommObject::CFastCommObject, sizeof %d, this 0x%0x\n", sizeof(*this), this);
#endif
	InitializeCriticalSection(&m_ConnectionsCS);
	InitializeCriticalSection(&m_MessagesCS);
	m_pMessHead = m_pMessTail = NULL;
	m_cMessagesInQueue = 0;

	// Create an instance of VWComm just to fill the static atom map.
	IVWComm *pcomm = NULL;
	if (SUCCEEDED(CoCreateInstance(CLSID_VWComm, NULL, CLSCTX_INPROC_SERVER, IID_IVWComm, (void **) &pcomm)) && pcomm)
		pcomm->Release();
}

CFastCommObject::~CFastCommObject()
{
#ifdef TRACE_ALLOCATION
	TRACE("CFastCommObject::~CFastCommObject, this 0x%0x\n", this);
#endif
	Terminate();
}

STDMETHODIMP CFastCommObject::Initialize(IVWCommSite *pCommSite)
{
#ifdef TRACE_ALLOCATION
	TRACE("CFastCommObject::Initialize, this 0x%0x\n", this);
#endif

	m_pCommSite = pCommSite;
	CConnection::GlobalInitialize();

	return S_OK;
}

STDMETHODIMP CFastCommObject::Terminate()
{
#ifdef TRACE_ALLOCATION
	TRACE("CFastCommObject::Terminate, this 0x%0x\n", this);
#endif

	// Disconnect listener.
	if (m_pListener)
	{
		m_pListener->Disconnect();
		m_pListener = NULL;
	}
	
	// Disconnect all connections.
	DisconnectAll();

	// release queued read messages
	EnterCriticalSection(&m_MessagesCS);
	while (m_pMessHead != NULL)
	{
		CFastCommMessage *pmess;

		pmess = m_pMessHead;
		m_pMessHead = m_pMessHead->GetNext();
		if (m_pMessHead==NULL)
			m_pMessTail = NULL;
		m_cMessagesInQueue--;

		pmess->Release();
	}
	LeaveCriticalSection(&m_MessagesCS);

	DeleteCriticalSection(&m_ConnectionsCS);
	DeleteCriticalSection(&m_MessagesCS);

	CConnection::GlobalCleanup();

	return S_OK;
}

STDMETHODIMP CFastCommObject::DisconnectAll()
{
	POSITION pos, oldpos;
	CFastCommConnection *pFastCommConnection = NULL;

	EnterCriticalSection(&m_ConnectionsCS);

	pos = m_pConnections.GetHeadPosition();
	while (pos)
	{
		oldpos = pos;
		pFastCommConnection = m_pConnections.GetNext(pos);
		pFastCommConnection->Disconnect();
		// Disconnect will fire OnDisconnect which will release the CFastCommConnection
		// and remove it from the list.
	}

	LeaveCriticalSection(&m_ConnectionsCS);
	
	return S_OK;
}

STDMETHODIMP CFastCommObject::KeepAlive()
{
	// Only needed client-side.
	return E_NOTIMPL;
}

STDMETHODIMP CFastCommObject::get_HostName(BSTR* pbstrVal)
{
	// Never used.
	return E_NOTIMPL;
}

STDMETHODIMP CFastCommObject::Connect(LPSTR pszRemote, int iPort, VARIANT_BOOL bTunnel, IVWCommConnection **ppconn)
{
	// Only needed client-side.
	return E_NOTIMPL;
}

STDMETHODIMP CFastCommObject::Accept(int iPort)
{
	SmartPtr<CConnection> pListener;

	// Create the CFastCommConnection wrapper.
	m_pListener = new CComFastCommConnection;
	// AddRef implicit 'cause m_pListener is a SmartPtr.

	// Initialize the CFastCommConnection wrapper.
	m_pListener->Initialize(this, m_pCommSite);

	if (CConnection::CreateListener(iPort, m_pListener, recv_Streaming, pListener) != 0)
		return ReportCommError(VWCOMM_E_WINSOCKERROR);

	// Give the CFastCommConnection wrapper the connector it wraps.
	m_pListener->SetConnector(pListener);

	return S_OK;
}

STDMETHODIMP CFastCommObject::GetMessage(IVWCommMessage **ppmess)
{
	CFastCommMessage *pmess=NULL;
	int iType;
	HRESULT hr = S_OK;

	if (ppmess==NULL)
		return E_POINTER;

	*ppmess = NULL;

	// dequeue one
	// NOTE that we don't do any refcount changes since we'd be
	// adding one (for the return value) and subtracting one
	// for the list removal

	EnterCriticalSection(&m_MessagesCS);

	if (m_pMessHead != NULL)
	{
		// see what type of message we have
		hr = m_pMessHead->GetStatus(&iType);
		//should never fail.
		if (FAILED(hr)) ASSERT(0); 

		if(iType!=CFastCommMessage::statusHaveResponse)
		{
			pmess = m_pMessHead;
			m_pMessHead = m_pMessHead->GetNext();
			if (m_pMessHead==NULL)
				m_pMessTail = NULL;
			m_cMessagesInQueue--;
		}
	}
		
	LeaveCriticalSection(&m_MessagesCS);

	if (pmess != NULL)
		*ppmess = (IVWCommMessage *) pmess;

	return S_OK;
}

HRESULT CFastCommObject::SendExt(IMarshallBuffer *pcb, int cConnection, IVWCommConnection *rgpConnection[], BYTE chSynchSerial, BYTE chMsgType)
{
	COMBLK	*pComBlk;
	DWORD	cb, cbExt;
	int		iconn, iRet = 0;
	HRESULT hr = S_OK;

	if (FAILED(hr = hr = pcb->get_Length(&cb)))
		return ReportCommError(hr);

	// adjust size (for header stuff)
	cbExt = cb + sizeof(COMBLK);

	// Possible to exceed this in recent tests of FastComm server with high load.
	// check size
//	if (cbExt > 65535)
//		return ReportCommError(E_INVALIDARG);

	// allocate
	pComBlk = (COMBLK*) new BYTE [cbExt];
	if (pComBlk==NULL)
		return ReportCommError(E_OUTOFMEMORY);

	// fill header
	pComBlk->cchMessage = (WORD) cb;
	pComBlk->chSerial = chSynchSerial;
	pComBlk->chType = chMsgType;
	// copy the data
	if (FAILED(hr = pcb->CopyFrom((BYTE *)pComBlk->rgData, cb)))
		return ReportCommError(hr);

	// iterate through the connections, sending and then
	// releasing as we go
	for (iconn = 0; iconn < cConnection; iconn++)
	{
		CFastCommConnection *pconn;

		// get the connection
		pconn = (CFastCommConnection*)rgpConnection[iconn];

		// send to ALL connections but store first error
		if (!iRet)
			iRet = pconn->m_pConnector->SendAsync((BYTE*)pComBlk, cbExt);
		else
			pconn->m_pConnector->SendAsync((BYTE*)pComBlk, cbExt);
	}
	delete [] pComBlk;

	if (iRet == WSAENOTSOCK)
		return ReportCommError(VWCOMM_E_INVALIDSOCKET);
	else if (iRet != 0)
		return ReportCommError(VWCOMM_E_WINSOCKERROR);
	
	return S_OK;
}