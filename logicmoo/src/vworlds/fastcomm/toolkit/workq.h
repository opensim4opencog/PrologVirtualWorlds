// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

//*****************************************************************************
// Project:		WorkQAdapatable
// Module:		WorkQ.h
// Description:	declares classes: CWorkQueue and CQueueMsg
// Author:		WR
// Create:		04/01/96
//-----------------------------------------------------------------------------
// Notes:
//
//	<notes about implementation>
//
//-----------------------------------------------------------------------------
// Issues:
//
//	<open issues, todos>
//
//-----------------------------------------------------------------------------
// Architecture:
//
//	REVIEW: <TODO:architectural description, with diagrams>
//
//
//*****************************************************************************

#ifndef _WORKQ_H_
#define _WORKQ_H_
#include <winbase.h>

//#define VARIABLE_THREAD

struct SystemInfo : public SYSTEM_INFO
{
	SystemInfo()
	{
		GetSystemInfo(this);
	}
};

extern SystemInfo g_SystemInfo;

// Global includes
#include <objbase.h>	// COM et al
#include "tools.h"
// Local includes

// Local preprocessor constructs

// Global variables.

// Static variables.

// Local function prototypes

// Local type definitions

//
// Class -- CQueueMsg:
//		This is the virtual base class for all messages posted to the
//		work queue.  Other messages should be derived from this
//		base class.  The HRESULT and CEvent may be used to return status and
//		signal the requestor that the message has been completed.  This behavior
//		may be augmented by the derived class as required.
//
class CQueueMsg
{
friend class CWorkQueue;
public:
	// ctor and dtor
    CQueueMsg( BOOL fAutoDelete = FALSE );  // passing TRUE implies this object will
                                            // destroy itself when done. As such, the
                                            // WaitForCompletion() and CheckForCompletion
                                            // methods should not be used in this case.
	virtual ~CQueueMsg();

	// waits on event -- returning m_hr
	HRESULT	WaitForCompletion( DWORD dwWait = INFINITE );

	// tests eventsem and return TRUE if completed
	BOOL CheckForCompletion();

protected:
	// set return value and trigger the event
	void Completed( HRESULT hr = S_OK );

	// worker threads execute the message semantics contained in this qitem
	virtual void PerformRequest(OVERLAPPED *pOverlapped) = 0;

protected:
    HANDLE      m_hEventDone;	// event to be signaled upon completion
	HRESULT		m_hr;			// overall return code from execution of this msg
    BOOL        m_fAutoDelete;  // if TRUE, event will not be fired and object will delete itself
};


//
// Class -- CWorkQueue:
//		Worker thread semantics are encapsulated.  This is an ACTIVE object that
//		controls 1 (or more) threads that are associated with an queue.
//		Requests are posted to a queue implemented using an IO completion port.
//
class CWorkQueue
{
protected:
	// ctor and initialization function 
    CWorkQueue();	    // use static CWorkQueue::Create
	HRESULT Init();	    // Initialize and start the queue

public:
	// creation and dtor
	static HRESULT Create( CWorkQueue** ppWorkQueue );
	~CWorkQueue();

public:
	// Requests are posted to the queue
	BOOL	Post( CQueueMsg* pmsg, OVERLAPPED *pOverlapped = NULL );
	HRESULT PostAndWait( CQueueMsg* pmsg, OVERLAPPED *pOverlapped = NULL, DWORD dwTime = INFINITE );

	//Advise the queue of a socket.
	HRESULT GetIoCompletionPort(HANDLE *phPort)
	{
		*phPort = m_hCompletionPort;

		return S_OK;
	}

	CString Dump()
	{
		CString out;
		out = "Worker Threads: "+str(m_cTotalThreads)+"\r\n";    // total threads active
		out += "Total requests: "+str(m_nTotalRequestsQueued)+"\r\n";
		return out;
	}
private:
	// worker thread methods
	void	ProcessQueue();		// handle incoming requests
    static  DWORD __stdcall WorkQueueThread( LPVOID pvWorkQueue );
    BOOL    StartNewThread();

    // update the statistic based data members...
    void    UpdateStatistics( const __int64& tStart, const __int64& tStop );

private:
    HANDLE          m_hCompletionPort;  // IO Completion port used as queue and thread throttle
    ULONG           m_timeout;          // our current timeout on the port

    LONG            m_cTotalThreads;    // total threads active
    LONG            m_cThreadMinWaiting;// minimum threads we will quiesce to
    LONG            m_cThreadWaitingOrStartingUp; // number of threads either initializing or 
                                                  // blocked on the port
    __int64         m_sumT;
    __int64         m_sumTSquared;      // for info on these -- see UpdateStatistics in code body
    __int64         m_scale;
    ULONG           m_n;
	
	long			m_nTotalRequestsQueued;

    enum { m_timingUnits = 100000 };    // 1/100,000 of a sec == 10 uSec

	CWaitUntilDoneSemaphore m_DoneSemaphore; //signal to synchronize exit.
	HANDLE			m_hDoneEvent;
    BOOL            m_fExit;            // tells the worker threads to fail the current request 
                                        // and exit
};

//*****************************************************************************
// Function:	Completed
// Implements:	n/a
// Description:	Triggers the eventsem and sets the return status unless autodelete
// Inputs:		hr -- HRESULT to be used as result from processing this request
//				(Default = S_OK)
// Outputs:		m_hr set to passed in value
// Return:		none
// Exceptions:	none
// Process:		n/a
//*****************************************************************************	
inline void CQueueMsg::Completed( HRESULT hr )
{
    if( !m_fAutoDelete )
    {
	    m_hr = hr;
	    SetEvent( m_hEventDone );
    }
    else
        // if we simply destroy ourselves, then don't worry about firing any event etc.
        delete this;
}

#endif _WORKQ_H_
