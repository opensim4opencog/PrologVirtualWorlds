// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include <vwobject.h>
#include <vwcomm.h>
#include "fastobj.h"
#include "marshbuf.h"

//#define TRACE_ALLOCATION

typedef CComObject<CUnMarshallBufferObject> CComUnMarshallBuffer;

EXTERN_C const CLSID CLSID_UnMarshallBuffer;

CFastCommMessage::CFastCommMessage() : m_pMessNext(NULL), m_pUbuffer(NULL), m_chSynchSerial(0), m_nStatus(statusUninitialized), m_pcomm(NULL), m_pconn(NULL)
{
#ifdef TRACE_ALLOCATION
	TRACE("CFastCommMessage::CFastCommMessage, sizeof %d, this 0x%0x\n", sizeof(*this), this);
#endif
}

CFastCommMessage::~CFastCommMessage()
{
#ifdef TRACE_ALLOCATION
	TRACE("CFastCommMessage::~CFastCommMessage, this 0x%0x, m_pcomm 0x%0x, m_pconn 0x%0x\n", this, m_pcomm, m_pconn);
#endif

	SAFERELEASE(m_pconn);
	SAFERELEASE(m_pUbuffer);
}

HRESULT CFastCommMessage::Initialize(CFastCommObject *pcomm, CFastCommConnection *pconn)
{ 
#ifdef TRACE_ALLOCATION
	TRACE("CFastCommMessage::Initialize, this 0x%0x, m_pcomm 0x%0x, m_pconn 0x%0x\n", this, pcomm, pconn);
#endif

	if (m_pcomm || m_pconn)
		return ReportMessageError(VWCOMM_E_IMPROPERSTATE);

	m_pconn = pconn;
	m_pconn->AddRef();

	// Weak reference to avoid cycles.
	m_pcomm = pcomm;

	return S_OK;
}

STDMETHODIMP CFastCommMessage::GetConnection(IVWCommConnection **ppIVWCommConnection)
{
	if (ppIVWCommConnection == NULL)
	{
		TRACE("CFastCommMessage::GetConnection: outgoing pointer (IVWCommConnection**) is NULL\n");
		return ReportMessageError(E_POINTER);
	}

	*ppIVWCommConnection = m_pconn;
	SAFEADDREF(*ppIVWCommConnection);

	return S_OK;
}

STDMETHODIMP CFastCommMessage::GetData(IUnMarshallBuffer **ppbuffer)
{
	ASSERT(ppbuffer);
	if (ppbuffer == NULL)
	{
		TRACE("CFastCommMessage::GetData: outgoing pointer (IUnMarshallBuffer**) is NULL\n");
		return ReportMessageError(E_POINTER);
	}

	*ppbuffer = m_pUbuffer;
	SAFEADDREF(*ppbuffer);

	return S_OK;
}

STDMETHODIMP CFastCommMessage::GetSynchronous(VARIANT_BOOL *pbSync)
{
	ASSERT(pbSync);
	if (pbSync == NULL)
	{
		TRACE("CFastCommMessage::GetSynchronous: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return ReportMessageError(E_POINTER);
	}

	*pbSync = (m_chSynchSerial != 0) ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CFastCommMessage::Reply(IMarshallBuffer *pcbReply)
{
	IVWCommConnection *pIVWCommConnection = m_pconn;
	return m_pcomm->SendExt(pcbReply, 1, &pIVWCommConnection, m_chSynchSerial, typeReply);
}

HRESULT CFastCommMessage::SetData(BYTE *pb, DWORD cb, char chSynch)
{
	HRESULT hr = S_OK;

	ASSERT(pb);
	if (pb == NULL)
	{
		TRACE("CFastCommMessage::SetData: incoming argument (BYTE*) is NULL\n");
		return ReportMessageError(E_INVALIDARG);
	}

	// create marshall buffer
	m_pUbuffer = new CComUnMarshallBuffer();
	if (m_pUbuffer == NULL)
	{
		TRACE("CFastCommMessage::SetData: failed to allocate CComUnMarshallBuffer\n");
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