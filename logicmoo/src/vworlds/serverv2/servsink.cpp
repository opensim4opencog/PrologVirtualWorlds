// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// ServerSink.cpp : Implementation of CServerSink
#include "stdafx.h"
#include "ServSink.h"
#include "serverV2.h"
	
#include "vwserver_i.c"

/////////////////////////////////////////////////////////////////////////////
// CServerSink

/////////////////////////////////////////////////////////////////////////////
// DVWServerEvents
STDMETHODIMP CServerSink::OnWorldLoaded(BSTR bstrWorld)
{
	return S_OK;
}

STDMETHODIMP CServerSink::OnWorldUnloaded(BSTR bstrWorld)
{
	return S_OK;
}

STDMETHODIMP CServerSink::OnConnect(DWORD hConnect)
{
	return S_OK;
}

STDMETHODIMP CServerSink::OnDisconnect(DWORD hDisconnect)
{
	return S_OK;
}

STDMETHODIMP CServerSink::OnEnterWorld(DWORD hConnect, BSTR bstrWorld)
{
	return S_OK;
}

STDMETHODIMP CServerSink::OnLeaveWorld(DWORD hConnect, BSTR bstrWorld)
{
	return S_OK;
}

STDMETHODIMP CServerSink::OnCommError(DWORD hConnect)
{
	return S_OK;
}

STDMETHODIMP CServerSink::OnCommMessage(DWORD hConnect, BSTR bstrMessage)
{
	return S_OK;
}

STDMETHODIMP CServerSink::OnReport(BSTR bstrMessage, long lType)
{
/*	CString str(bstrMessage);

	// log other errors too error reports
	if (lType == -1)
		Log(ID_EVENT_ERROR, str);
	// server admin heads up.
	else if (lType == 0)
	{
		Log(0, str);
	}

*/	return S_OK;
}

STDMETHODIMP CServerSink::OnTrace(BSTR bstrMessage)
{
	return S_OK;
}

STDMETHODIMP CServerSink::OnAvailableWorld(BSTR bstrName, int iNoUsers)
{
	return m_pApp->HandleOnAvailableWorld(bstrName, iNoUsers);
}