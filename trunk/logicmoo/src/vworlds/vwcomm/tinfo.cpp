// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include <vwobject.h>
#include <vwcomm.h>
#include "commobj.h"

/////////////////////////////////////////////////////////////////////
// Thread info

CReadThreadInfo::CReadThreadInfo() :
	m_iOperation(opUninitialized),
	m_pcomm(NULL),
	m_pthread(NULL),
	m_bW95Thread(FALSE),
	m_heventStart(NULL),
	m_heventFinished(NULL),
	m_pconn(NULL),
	m_hrReturn(S_OK),
	m_cConnection(0)
{
}

CReadThreadInfo::~CReadThreadInfo()
{
	// assert that we're stopped
	ASSERT(m_pthread==NULL);

	if (m_heventStart)
		CloseHandle(m_heventStart);

	if (m_heventFinished)
		CloseHandle(m_heventFinished);

}

HRESULT CReadThreadInfo::Initialize(CVWCommObject *pcomm)
{
	// state check
	if (m_iOperation!=opUninitialized)
		return E_FAIL;

	// NOTE: this is a weak reference
	m_pcomm = pcomm;

	// create the handles
	m_heventStart = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_heventFinished = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (m_heventStart==NULL		||
		m_heventFinished==NULL )
	{
		return E_OUTOFMEMORY;
	}

	return S_OK;
}

HRESULT CReadThreadInfo::SetThread(CWinThread *pthread, BOOL bWin95) 
{ 
	// state check
	if (m_iOperation!=opUninitialized)
		return E_FAIL;

	m_pthread = pthread; 
	m_bW95Thread = bWin95;

	return S_OK;
};

HRESULT CReadThreadInfo::ClearThread()
{
	// state check
	if (m_iOperation!=opUninitialized)
		return E_FAIL;

	// set autodelete back on so that the thread
	// nukes the winthread itself
	m_pthread->m_bAutoDelete = TRUE;
	m_pthread = NULL;

	return S_OK;
}

HRESULT CReadThreadInfo::StartReadOnConnection(CVWCommConnection *pconn)
{
	// serialize (since we use data in this to communicate with read thread
	Lock();

	// state check
	if (m_iOperation!=opUninitialized)
	{
		Unlock();
		return E_FAIL;
	}

	// stuff this
	m_pconn = pconn;

	// set the operation code
	m_iOperation = opAddRead;

	// set the read thread info on the connection
	pconn->SetReadThreadInfo(this);

	// set the start event
	SetEvent(m_heventStart);

	// wait 'til op completes
	HRESULT hr;
	if (!WaitUntilDone())
		hr = E_FAIL;
	else
		hr = m_hrReturn;

	// reset
	m_iOperation = opUninitialized;

	Unlock();

	return hr;
}

HRESULT CReadThreadInfo::StopReadOnConnection(CVWCommConnection *pconn)
{
	HRESULT hr = S_OK;

	// serialize (since uses data in this to communicate with thread)
	Lock();

	// state check
	if (m_iOperation!=opUninitialized)
	{
		Unlock();
		return E_FAIL;
	}

	// set the operation code
	m_iOperation = opStopRead;

	if (m_bW95Thread)
	{
		// on win95, need to break out of socket recv(). This might
		// have already happened, but, in case not, we close the
		// socket here, too.

		// assert have right thread
		if (m_pconn != pconn)
		{
			Unlock();
			return E_FAIL;
		}

		if (m_pconn->m_sock != INVALID_SOCKET)
		{
			closesocket(m_pconn->m_sock);

			// wait until the thread's gone
			if (m_pthread != NULL)
				if (!WaitForEvent(m_pthread->m_hThread))
				{
					Unlock();
					return E_FAIL;
				}

			// pause, to give the thread a chance to 
			// completely clean up
			// REVIEW: need to get rid of this sleep statement
			Sleep(1000);

			// zap the handle
			delete m_pthread;
			m_pthread = NULL;
		}

		// Should have 0 connections now and zapped hanndle
		//ASSERT(m_cConnection==0);
	}
	else
	{
		// if we're being called from the managed thread, do the
		// operation in-line
		if (::AfxGetThread()==m_pthread)
		{
			// cancel any pending IO
			pconn->m_pcomm->CommCancelPendingIO(pconn);

			// give it a chance to finish
			WaitForStartEx(0);

			// decrement the count
			int cCount = DecrementConnectionCount();
			ASSERT(cCount>=0);

		}
		else
		{
			// set the connection
			m_pconn = pconn;

			// set the event to start the operation
			SetEvent(m_heventStart);

			// wait 'til done
			if (!WaitUntilDone())
				hr = E_FAIL;
			else
				hr = m_hrReturn;
		}
	}

	// reset
	m_iOperation = opUninitialized;

	Unlock();

	return hr;
}

HRESULT CReadThreadInfo::Terminate()
{
	HRESULT hr = S_OK;

	// serialize
	Lock();

	// state check
	if (m_iOperation!=opUninitialized)
		return E_FAIL;

	// if no thread, nothing to do
	if (m_pthread==NULL)
		return S_OK;

	// assert that we have no connections
	// REVIEW: return error?
	// ASSERT(m_cConnection==0);

	if (m_bW95Thread)
	{
		// nothing to do
		ASSERT(m_pthread==NULL);
	}
	else
	{
		// set the operation code and conn pointer
		m_iOperation = opTerminate;
		m_pconn = NULL;

		// start the operation
		SetEvent(m_heventStart);

		if (!WaitUntilDone())
			hr = E_FAIL;
		else
			hr = m_hrReturn;

		// pause, to give the thread a chance to 
		// completely clean up
		// REVIEW: this sucks
		Sleep(100);

		// zap the handle
		delete m_pthread;
		m_pthread = NULL;
	}

	// check outgoing state
	// ASSERT(m_cConnection==0);

	// reset
	m_iOperation = opUninitialized;

	Unlock();

	return hr;
}

BOOL CReadThreadInfo::WaitForStart(DWORD cmsecWait)
{
	ASSERT(m_heventStart);
	return WaitForEvent(m_heventStart, cmsecWait);
}

DWORD CReadThreadInfo::WaitForStartEx(DWORD cmsecWait)
{
	return WaitForSingleObjectEx(m_heventStart, cmsecWait, TRUE);
}

void CReadThreadInfo::GetOperation(int& iOperation, CVWCommConnection** ppconn)
{
	iOperation = m_iOperation;
	*ppconn = m_pconn;
	SAFEADDREF(*ppconn);
}

void CReadThreadInfo::SignalDone(HRESULT hrReturn)
{
	m_hrReturn = hrReturn;
	SetEvent(m_heventFinished);
}

int CReadThreadInfo::ConnectionCount()
{
	int cCount;

	Lock();
	cCount = m_cConnection;
	Unlock();

	return cCount;
}

int CReadThreadInfo::IncrementConnectionCount()
{
	int nCount;

	// Don't need to lock and unlock since
	// this function is only called while other
	// threads wait for this one to operate (i.e.
	// there's an "external" lock.

	//Lock();
	nCount = InterlockedIncrement((long *)&m_cConnection);
	 //Unlock();

	return nCount;
}

int CReadThreadInfo::DecrementConnectionCount()
{
	int nCount;

	// Don't need to lock and unlock since
	// this function is only called while other
	// threads wait for this one to operate (i.e.
	// there's an "external" lock.

	 //Lock();
	nCount = InterlockedDecrement((long *)&m_cConnection);
	 //Unlock();

	return nCount;
}

void CReadThreadInfo::Lock()
{
	m_lock.Lock();
}

void CReadThreadInfo::Unlock()
{
	m_lock.Unlock();
}

BOOL CReadThreadInfo::WaitUntilDone(DWORD cmsecWait)
{
	return WaitForEvent(m_heventFinished, cmsecWait);
}

