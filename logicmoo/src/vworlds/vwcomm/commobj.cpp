// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// CommObj.cpp : Implementation of CvwcommApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include <vwcomm.h>
#include "commobj.h"
#include "marshbuf.h"

//#define TRACE_ALLOCATION

typedef CComObject<CUnMarshallBufferObject> CComUnMarshallBuffer;

EXTERN_C const CLSID CLSID_UnMarshallBuffer;

CVWMap<CBSTRAdapt<CComBSTR>, BSTR, int, int&> CVWCommObject::m_atomMap;
CVWMap<int, int&, CBSTRAdapt<CComBSTR>, BSTR> CVWCommObject::m_stringMap;
int CVWCommObject::m_iAtomIndex = 1;

/////////////////////////////////////////////////////////////////////////////
//

CVWCommObject::CVWCommObject() :
	m_pCommSite(NULL),
	m_state(stateUninitialized)
{
#ifdef TRACE_ALLOCATION
	TRACE("CVWCommObject::CVWCommObject, sizeof %d, this 0x%0x\n", sizeof(*this), this);
#endif

	CommConstruct();

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_iAtomIndex == 1)
	{
		CString strRESOURCE;
		int iStartAtom = IDS_START_ATOM;

		while (strRESOURCE.LoadString(iStartAtom))
		{
			/* hr = */ AddStringToAtomMap(CComBSTR(strRESOURCE));

			iStartAtom++;
		}
	}
}

CVWCommObject::~CVWCommObject()
{
#ifdef TRACE_ALLOCATION
	TRACE("CVWCommObject::~CVWCommObject, this 0x%0x\n", this);
#endif

	// clean up
	Terminate();
}

void CVWCommObject::AddStringToAtomMap(BSTR bstr)
{
	// NOTE: m_iAtomIndex starts at 1

	m_atomMap.SetAt(bstr, m_iAtomIndex);
	m_stringMap.SetAt(m_iAtomIndex, bstr);

	m_iAtomIndex++;
}

STDMETHODIMP CVWCommObject::Initialize(IVWCommSite *pCommSite)
{
	HRESULT hr;

	// check state
	if (m_state!=stateUninitialized)
		return ReportCommError(VWCOMM_E_IMPROPERSTATE);

	ASSERT(m_pCommSite==NULL);
	m_pCommSite = pCommSite;
	SAFEADDREF(m_pCommSite);

	hr = CommInit();
	if (FAILED(hr))
		return ReportCommError(hr);

	// set state
	m_state = stateInitialized;

	return S_OK;
}

STDMETHODIMP CVWCommObject::Terminate()
{
#ifdef TRACE_ALLOCATION
	TRACE("CVWCommObject::Terminate, this 0x%0x\n", this);
#endif

	HRESULT hr;

	// set state
	m_state = stateTerminating;

	// low-level termination
	hr = CommTerminate();
	if (FAILED(hr))
		return hr;

	// release the comm site
	SAFERELEASE(m_pCommSite);

	// set state
	m_state = stateUninitialized;

	return S_OK;
}

STDMETHODIMP CVWCommObject::DisconnectAll()
{
	return CommDisconnectAll();
}

STDMETHODIMP CVWCommObject::KeepAlive()
{
	return CommKeepAlive();
}

STDMETHODIMP CVWCommObject::get_HostName(BSTR* pbstrVal)
{
	return CommGetHostName(pbstrVal);
}

STDMETHODIMP CVWCommObject::Connect(LPSTR pszRemote, int iPort, VARIANT_BOOL bTunnel, IVWCommConnection **ppconn)
{
	HRESULT hr;

	// state check
	// REVIEW: This disallows connect if already accepting
	if (m_state != stateInitialized)
		return ReportCommError(VWCOMM_E_IMPROPERSTATE);

	ASSERT(pszRemote);
	if (pszRemote == NULL)
	{
		TRACE("CVWCommObject::Connect: incoming argument (LPSTR) is NULL\n");
		return ReportCommError(E_INVALIDARG);
	}

	ASSERT(ppconn);
	if (ppconn== NULL)
	{
		TRACE("CVWCommObject::Connect: outgoing pointer (IVWCommConnection**) is NULL\n");
		return ReportCommError(E_POINTER);
	}

	// initialize
	*ppconn = NULL;

	CVWCommConnection *pconn=NULL;
	hr = CommConnect(pszRemote, iPort, bTunnel, &pconn);
	if (FAILED(hr))
		return ReportCommError(hr);

	// already ref count bumped in CommConnect
	*ppconn = (IVWCommConnection *) pconn;

	m_state = stateConnected;

	return S_OK;
}

STDMETHODIMP CVWCommObject::Accept(int iPort)
{
	HRESULT hr;

	// state check
	// REVIEW: this disallows accept if already connecting
	if (m_state != stateInitialized)
		return ReportCommError(VWCOMM_E_IMPROPERSTATE);

	hr = CommAccept(iPort);
	if (FAILED(hr))
		return ReportCommError(hr);

	m_state = stateAccepting;

	return S_OK;
}

STDMETHODIMP CVWCommObject::GetMessage(IVWCommMessage **ppmess)
{
	return CommGetMessage(ppmess);
}

