// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include <vwobject.h>
#include <vwcomm.h>
#include <vwbuffer.h>
#include "commobj.h"

//#define TRACE_ALLOCATION

// typedef CComObject<CUnMarshallBufferObject> CComUnMarshallBuffer;
// EXTERN_C const CLSID CLSID_UnMarshallBuffer;

//
// CVWCommConnection
//
CVWCommConnection::CVWCommConnection() :
	m_sock(INVALID_SOCKET),
	m_ct(eUnknown),
	m_bClientSide(TRUE),
	m_bAsciiMode(TRUE),
	m_bReplyReceived(FALSE),
	m_iPort(0),
	m_pcomm(NULL),
	m_pconnPartner(NULL),
	m_pauth(NULL),
	m_pvData(NULL),
	m_pti(NULL),
	m_pbRead(NULL),
	m_cbRead(0),
	m_cbReadMax(0),
	m_bInDisconnect(FALSE),
	m_bInWrite(FALSE),
	m_bInSendSynch(FALSE),
	m_bHaveReply(FALSE),
	m_bReplyFailed(FALSE),
	m_heventHaveData(NULL),
	m_pBufferReply(NULL),
	m_chSynchSerial(0),
	m_timeSampleStart(0),
	m_cPacketsSent(0),
	m_cPacketsSentPerSecond(0),
	m_cBytesSent(0),
	m_cBytesSentPerSecond(0),
	m_cPacketsReceived(0),
	m_cPacketsReceivedPerSecond(0),
	m_cBytesReceived(0),
	m_cBytesReceivedPerSecond(0)
{
#ifdef TRACE_ALLOCATION
	TRACE("CVWCommConnection::CVWCommConnection, sizeof %d, this 0x%0x\n", sizeof(*this), this);
#endif
}

CVWCommConnection::~CVWCommConnection()
{
#ifdef TRACE_ALLOCATION
	TRACE("CVWCommConnection::~CVWCommConnection, this 0x%0x\n", this);
#endif

	// assert that it's disconnected already
	ASSERT(m_sock == INVALID_SOCKET);

	SAFERELEASE(m_pcomm);
	SAFERELEASE(m_pauth);
	SAFERELEASE(m_pconnPartner);

	if (m_pbRead!=NULL)
		delete [] m_pbRead;

	CloseHandle(m_heventHaveData);
}

HRESULT CVWCommConnection::Initialize(CVWCommObject *pcomm, SOCKET sock, PSOCKADDR psa, LPCTSTR pszServer, int iPort, BOOL bClientSide)
{
#ifdef TRACE_ALLOCATION
	TRACE("CVWCommConnection::Initialize, this 0x%0x\n", this);
#endif

	// set backpointers, etc.
	ASSERT(m_pcomm==NULL);
	ASSERT(pcomm!=NULL);
	m_pcomm = pcomm;
	pcomm->AddRef();

	m_sock = sock;
	m_bClientSide = bClientSide;
	memcpy(&m_sockaddr, psa, sizeof(SOCKADDR_IN));
	if (pszServer!=NULL)
		m_strServer = pszServer;
	m_iPort = iPort;

	// create buffer
	m_pbRead = new BYTE[cbReadMaxDefault];
	if (m_pbRead==NULL)
		return E_OUTOFMEMORY;
	m_cbReadMax = cbReadMaxDefault;

	// create event for handling synch sends
	m_heventHaveData = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (m_heventHaveData==NULL)
		return E_OUTOFMEMORY;

	// Initialize OLS -  poke self pointer into overlapped structure
	// NOTE: as per Win32 docs, can do this hack
	memset(&m_ols, 0, sizeof(m_ols));
	m_ols.hEvent = (HANDLE) (void *) this;

	// set time stamp for statistics
	m_timeSampleStart = GetTickCount();

	return S_OK;
}


STDMETHODIMP CVWCommConnection::SendAsync(IMarshallBuffer *pcb)
{
	HRESULT hr = S_OK;
	PWRITEBUFFER pwb;

	ASSERT(pcb);
	if (pcb == NULL)
	{
		TRACE("CVWCommConnection::SendAsync: incoming argument (IMarshallBuffer*) is NULL\n");
		return ReportConnectionError(E_INVALIDARG);
	}

	// get a buffer
	hr = m_pcomm->CommAllocWriteBuffer(pcb, &pwb);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pcomm->CommWrite(this, pwb);

ERROR_ENCOUNTERED:
	return ReportConnectionError(hr);
}

STDMETHODIMP CVWCommConnection::SendAsyncExt(IMarshallBuffer *pcb, int cConnection, IVWCommConnection *rgpConnection[])
{
	HRESULT hr = S_OK;
	PWRITEBUFFER pwb;

	ASSERT(pcb);
	if (pcb == NULL)
	{
		TRACE("CVWCommConnection::SendAsyncExt: incoming argument (IMarshallBuffer*) is NULL\n");
		return ReportConnectionError(E_INVALIDARG);
	}

	// get a buffer
	hr = m_pcomm->CommAllocWriteBuffer(pcb, &pwb);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pcomm->CommWriteExt(cConnection, rgpConnection, pwb);

ERROR_ENCOUNTERED:

	return ReportConnectionError(hr);
}

STDMETHODIMP CVWCommConnection::SendSync(IMarshallBuffer *pcb, IUnMarshallBuffer **ppcbReply)
{
	HRESULT hr = S_OK;
	PWRITEBUFFER pwb;
	
	ASSERT(pcb);
	if (pcb == NULL)
	{
		TRACE("CVWCommConnection::SendSync: incoming argument (IMarshallBuffer*) is NULL\n");
		return ReportConnectionError(E_INVALIDARG);
	}

	ASSERT(ppcbReply);
	if (ppcbReply == NULL)
	{
		TRACE("CVWCommConnection::SendSync: outgoing pointer (IUnMarshallBuffer**) is NULL\n");
		return ReportConnectionError(E_POINTER);
	}

	*ppcbReply = NULL;

	// get a buffer
	hr = m_pcomm->CommAllocWriteBuffer(pcb, &pwb);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// bump serial number (wrap to 1 - disallow 0)
	if (++m_chSynchSerial==0)
		++m_chSynchSerial;

	hr = m_pcomm->CommWriteSynch(this, pwb, m_chSynchSerial, ppcbReply);

ERROR_ENCOUNTERED:

	return ReportConnectionError(hr);
}

STDMETHODIMP CVWCommConnection::Disconnect()
{
	return m_pcomm->CommDisconnect(this, TRUE);
}

STDMETHODIMP CVWCommConnection::get_SocketAddress(BSTR* pbstrVal)
{
	CComBSTR bstrSockAddr;

	if (pbstrVal == NULL)
		return ReportConnectionError(E_POINTER);

	*pbstrVal = NULL;

	// verify all's ok
	if (m_sock==INVALID_SOCKET || m_pcomm==NULL)
		return ReportConnectionError(VWCOMM_E_NOTINITIALIZED);

	bstrSockAddr = inet_ntoa(m_sockaddr.sin_addr);
	*pbstrVal = bstrSockAddr.Copy();

	return S_OK;
}

HRESULT CVWCommConnection::SetConnectionType(ConnectionType ct)
{
	m_ct = ct;

	// also set ascii mode flag (false unless unknown type)
	SetAsciiMode(ct==eUnknown);

	return S_OK;
}

