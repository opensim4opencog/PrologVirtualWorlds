// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include <vwobject.h>
#include <vwcomm.h>
#include "commobj.h"
#include "marshbuf.h"

//#define TRACE_ALLOCATION

typedef CComObject<CUnMarshallBufferObject> CComUnMarshallBuffer;

EXTERN_C const CLSID CLSID_UnMarshallBuffer;

CVWCommMessage::CVWCommMessage() :
	m_nStatus(statusUninitialized),
	m_pMessNext(NULL),
	m_pcomm(NULL),
	m_pconn(NULL),
	m_pUbuffer(NULL),
	m_chSynchSerial(0)
{
#ifdef TRACE_ALLOCATION
	TRACE("CVWCommMessage::CVWCommMessage, sizeof %d, this 0x%0x\n", sizeof(*this), this);
#endif
}

CVWCommMessage::~CVWCommMessage()
{
#ifdef TRACE_ALLOCATION
	TRACE("CVWCommMessage::~CVWCommMessage, this 0x%0x, m_pcomm 0x%0x, m_pconn 0x%0x\n", this, m_pcomm, m_pconn);
#endif

	SAFERELEASE(m_pcomm);
	SAFERELEASE(m_pconn);
	SAFERELEASE(m_pUbuffer);
}

void CVWCommMessage::Initialize(CVWCommObject *pcomm, CVWCommConnection *pconn)
{ 
#ifdef TRACE_ALLOCATION
	TRACE("CVWCommMessage::Initialize, this 0x%0x, m_pcomm 0x%0x, m_pconn 0x%0x\n", this, pcomm, pconn);
#endif

	SAFERELEASE(m_pcomm);
	m_pcomm = pcomm;
	SAFEADDREF(pcomm);

	SAFERELEASE(m_pconn);
	m_pconn = pconn; 
	SAFEADDREF(pconn);
}

STDMETHODIMP CVWCommMessage::GetConnection(IVWCommConnection **ppIVWCommConnection)
{
	ASSERT(ppIVWCommConnection);
	if (ppIVWCommConnection == NULL)
	{
		TRACE("CVWCommMessage::GetConnection: outgoing pointer (IVWCommConnection**) is NULL\n");
		return ReportMessageError(E_POINTER);
	}

	*ppIVWCommConnection = m_pconn;
	SAFEADDREF(*ppIVWCommConnection);

	return S_OK;
}

STDMETHODIMP CVWCommMessage::GetData(IUnMarshallBuffer **ppbuffer)
{
	ASSERT(ppbuffer);
	if (ppbuffer == NULL)
	{
		TRACE("CVWCommMessage::GetData: outgoing pointer (IUnMarshallBuffer**) is NULL\n");
		return ReportMessageError(E_POINTER);
	}

	*ppbuffer = m_pUbuffer;
	SAFEADDREF(*ppbuffer);

	return S_OK;
}

STDMETHODIMP CVWCommMessage::GetSynchronous(VARIANT_BOOL *pbSync)
{
	ASSERT(pbSync);
	if (pbSync == NULL)
	{
		TRACE("CVWCommMessage::GetSynchronous: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return ReportMessageError(E_POINTER);
	}

	*pbSync = (m_chSynchSerial != 0) ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CVWCommMessage::Reply(IMarshallBuffer *pcbReply)
{
	HRESULT hr = S_OK;
	PWRITEBUFFER pwb;

	ASSERT(pcbReply);
	if (pcbReply == NULL)
	{
		TRACE("CVWCommMessage::Reply: incoming argument (IMarshallBuffer*) is NULL\n");
		return ReportMessageError(E_INVALIDARG);
	}

	// get a buffer
	hr = m_pcomm->CommAllocWriteBuffer(pcbReply, &pwb);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pcomm->CommReply(m_pconn, pwb, m_chSynchSerial);

ERROR_ENCOUNTERED:

	return ReportMessageError(hr);
}

HRESULT CVWCommMessage::SetData(BYTE *pb, DWORD cb, char chSynch)
{
	HRESULT hr = S_OK;

	ASSERT(pb);
	if (pb == NULL)
	{
		TRACE("CVWCommMessage::SetData: incoming argument (BYTE*) is NULL\n");
		return ReportMessageError(E_INVALIDARG);
	}

	// create marshall buffer
	m_pUbuffer = new CComUnMarshallBuffer();
	if (m_pUbuffer == NULL)
	{
		TRACE("CVWCommMessage::SetData: failed to allocate CComUnMarshallBuffer\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	m_pUbuffer->AddRef();

	hr = m_pUbuffer->CopyTo(pb, cb);

	// set serial #
	m_chSynchSerial = chSynch;

ERROR_ENCOUNTERED:
	return ReportMessageError(hr);
}

