// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

//**************************************************************************************
// Project:		FastComm
// Module:		locks.h
// Description:	defines various useful locks.
// Author:		CH
// Create:		10/28/99              
// Some locks are inspired from code by JL.
//--------------------------------------------------------------------------------------

#ifndef _LOCKS_HEADER
#define _LOCKS_HEADER
#include <windows.h>
#include <map>

//+---------------------------------------------------------------------------
//
//  Class:      CEventSem
//
//  Purpose:    Barrier services
//
//  Interface:  Wait            - wait for barrier to be lowered
//              Set             - set signalled state (lower barrier)
//              Reset           - clear signalled state (raise barrier)
//              Pulse           - set and immediately clear barrier
//
//  Notes:      Used for communication between consumers and producers.
//              Consumer threads block by calling Wait. A producer calls Set
//              waking up all the consumers who go ahead and consume until
//              there's nothing left. They call Reset, release whatever lock
//              protected the resources, and call Wait. There has to be a
//              separate lock to protect the shared resources.  Remember:
//              call Reset under lock.  don't call Wait under lock.
//
//----------------------------------------------------------------------------

class CEventSem
{
public:
    CEventSem(BOOL fStartSignaled=FALSE, 
			  const LPSECURITY_ATTRIBUTES lpsa=NULL,
			  BOOL fManualReset=TRUE) {
		m_hEvent = CreateEvent (lpsa, fManualReset, fStartSignaled, 0);
		if (m_hEvent == NULL) {
			ASSERT(0);
		}
	}
    ~CEventSem() { 
		CloseHandle (m_hEvent);
	}

    void Wait(DWORD dwMilliseconds=INFINITE, BOOL fAlertable=FALSE ) {
		if (WaitForSingleObjectEx (m_hEvent, dwMilliseconds, fAlertable) == 0xffffffff) {
			ASSERT(0);
		}
	}

    void Set(void) {
		if (!SetEvent (m_hEvent)) {
			ASSERT(0);
		}
	}
    void Reset(void) {
		if (!ResetEvent(m_hEvent)) {
			ASSERT(0);
		}
	}
    void Pulse(void) {
		if (!PulseEvent(m_hEvent)) {
			ASSERT(0);
		}
	}

private:
    HANDLE      m_hEvent;
};

//+--------------------------------------------------------------------------
//
//  Class:      CSemExclusive
//
//  Purpose:    Critical-section services
//
//  Interface:  Lock			- locks the critical section
//				Unlock			- unlocks the critical section
//
//  Note:		Used to guarantee that only one thread is accessing an
//              exclusive resource at a time. For instance, if you're
//              maintaining a shared queue, you might need to lock access to
//              the queue whenever you fiddle with some of the queue
//              pointers. 
//
//				CLock provides a nice exception-safe wrapper around a
//				CSemExclusive.
//
//---------------------------------------------------------------------------
class CSemExclusive {
public:
	CSemExclusive (unsigned long ulcSpinCount = 500)
	{
		InitializeCriticalSection(&m_csx);
		//VERIFY(InitializeCriticalSectionAndSpinCount(&m_csx, ulcSpinCount));
	}

	~CSemExclusive (void) {
		DeleteCriticalSection (&m_csx); 
	}
	
	void Lock (void) {
		EnterCriticalSection (&m_csx); 
	}
	
	BOOL TryLock (void) {
		return TryEnterCriticalSection (&m_csx); 
	}
	
	void Unlock (void) {  // to please DTC
		LeaveCriticalSection (&m_csx);
	}

private:
	CRITICAL_SECTION m_csx;
};

//Custom synchronization object.
//This object is used when you have to do N things you want done before going on.
//Ex: I have a thread pool servicing connections. I have N connections active. Before
//destroying the thread pool, I want to make sure all N connections are finished cleaning
//up.
class CWaitUntilDoneSemaphore{
public:
	CWaitUntilDoneSemaphore()
	{
		m_lCount = 0;
		//initial state = signalled.
		//not autoreset.
		m_hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
		ASSERT(m_hEvent);		
	}
 
	~CWaitUntilDoneSemaphore() 
	{
		CloseHandle(m_hEvent);
	}
	
	long Increment()
	{
		long lRet = InterlockedIncrement(&m_lCount);
		if(lRet==1)
		{
			//there are some unfinished tasks now.
			ResetEvent(m_hEvent);
		}
		return lRet;
	}

	long Decrement()
	{
		long lRet = InterlockedDecrement(&m_lCount);
		ASSERT(lRet>=0);
		if(lRet==0)
		{
			//no more unfinished tasks.
			VERIFY(SetEvent(m_hEvent));
		}
		return lRet;
	}

	DWORD WaitUntilZeroCount(DWORD dwTimeOut)
	{
		return WaitForSingleObject(m_hEvent, dwTimeOut);
	}

private:
	long m_lCount;
	HANDLE m_hEvent;
};

#endif