// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

//*****************************************************************************
// Project:		WorkQAdapatable
// Module:		WorkQ.CPP
// Description:	defines classes: CWorkQueue and CQueueMsg
// Author:		WR
// Create:		04/02/96
//              01/18/99
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
//	<architectural description, with diagrams>
//
//
//*****************************************************************************

// Global includes
//#include "stdafx.h"
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

// Local includes
#include "workq.h"

// Local preprocessor constructs
// Local type definitions

// Global variables.
SystemInfo g_SystemInfo;

// Static variables.

// Local function prototypes

// Class CWorkQueue


//*****************************************************************************
// Function:	constructor
// Implements:	n/a
// Description:	initializes members that don't return error results
// Inputs:		none
// Outputs:		none
// Return:		none
// Exceptions:	none
// Process:		n/a
//*****************************************************************************	
CWorkQueue::CWorkQueue()
{
    m_hCompletionPort = NULL;
    m_cTotalThreads = 0;
    m_cThreadWaitingOrStartingUp = 0;
    m_fExit = FALSE;
	m_hDoneEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

    // initial timeout is something big, but not INFINITE so all threads can 
    // eventually prune themselves out
    m_timeout = 3 * 60 * 1000;         
    m_cThreadMinWaiting = 1;

    // Init the statistics
    m_sumT = m_sumTSquared = 0;
    m_n = 0;
    QueryPerformanceFrequency( (LARGE_INTEGER*) &m_scale ); 
    // change from counts per second to counts per 10 uSec.
    m_scale = m_scale / (ULONG)m_timingUnits;
    if( m_scale == 0 )
        m_scale = 1;

	m_nTotalRequestsQueued = 0;
}


//*****************************************************************************
// Function:	destructor
// Implements:	n/a
// Description:	clears Q and waits for all threads to exit
// Inputs:		none
// Outputs:		none
// Return:		none
// Exceptions:	none
// Process:		n/a
//*****************************************************************************	
CWorkQueue::~CWorkQueue()
{
//    TRACE( "Terminating the work queue\n" );

    // set the flag to exit. This will force the threads that retrieve a valid
    // queued request at this point to fail it and return.
    m_fExit = TRUE;
	m_timeout = 0;

    // Try to shut down somewhat more gracefully so as not to, for example,
    // leak memory from the items that they contain.
    while( m_cTotalThreads > 0 )
    {
//		TRACE( "waiting for worker thread termination...\n" );
	    if(PostQueuedCompletionStatus( m_hCompletionPort, 0, NULL, NULL))
		{
			WaitForSingleObject(m_hDoneEvent, INFINITE);
		}
		else
		{
			ASSERT(0);
			break;
		}
//      TRACE( "...worker thread terminated\n" );
    }

	//should all be cleaned up already.
    VERIFY(WAIT_TIMEOUT != m_DoneSemaphore.WaitUntilZeroCount(1000));

    // close the port and event
    if( m_hCompletionPort )
        CloseHandle( m_hCompletionPort );

    if( m_hDoneEvent )
        CloseHandle( m_hDoneEvent );

}					


//*****************************************************************************
// Function:	Create (static)
// Implements:	n/a
// Description:	creates a heap based CWorkQueue object and initializes it.
// Inputs:		none
// Outputs:		returns a pointer to the object.  NULL upon failure
// Return:		
//				S_OK			Success
//				E_INVALIDARG	Bad output pointer
//              E_OUTOFMEMORY   ctor failed -- possibly memory exhaustion
// Process:		n/a
//*****************************************************************************	
HRESULT CWorkQueue::Create( CWorkQueue** ppWorkQueue )
{
	CWorkQueue*	pWorkQueue;
    HRESULT     hr;

//    TRACE( "Creating the work queue\n" );

	// do the output pointer dance
	if( ppWorkQueue == NULL )
		return E_INVALIDARG;

	*ppWorkQueue = NULL;

	// allocate a new object and initialize it
	pWorkQueue = new CWorkQueue();
    if (pWorkQueue == NULL)
        return E_OUTOFMEMORY;

	if (FAILED(hr = pWorkQueue->Init() ) )
        return hr;

	*ppWorkQueue = pWorkQueue;
	return S_OK;
}


//*****************************************************************************
// Function:	Init
// Implements:	n/a
// Description:	initializes the queue (including starting a thread) and returns any failures
// Inputs:		none
// Outputs:		none 
// Return:		
//				S_OK			Success
//				E_*	            Win32 error from completion port create failure etc.
// Process:		n/a
//*****************************************************************************	
HRESULT CWorkQueue::Init()
{
    // create the IO Completion Port (using 0 to indicated concurrent threads = #processors)
    m_hCompletionPort = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );

    if( m_hCompletionPort == NULL )
        return HRESULT_FROM_WIN32( GetLastError() );

	#ifdef VARIABLE_THREAD
	    return (StartNewThread() == TRUE);
	#else
		int iInitialNoThreads = g_SystemInfo.dwNumberOfProcessors;
		for(DWORD i = 0; i < iInitialNoThreads; i++)
		{
			//go with same number of threads as processors.
			VERIFY(StartNewThread());
		}
		return 1;
	#endif
}


//*****************************************************************************
// Function:	Post
// Implements:	n/a
// Description:	Adds the msg to the queue
// Inputs:		pmsg -- pointer the the queue msg request
// Outputs:		none
// Return:		TRUE if successful; FALSE otherwise
// Exceptions:	none
// Process:		n/a
//*****************************************************************************	
BOOL CWorkQueue::Post( CQueueMsg* pmsg, OVERLAPPED *pOverlapped )
{
    if( m_fExit )
        return FALSE;

    // post it to the completion port using the number of bytes parm to pass the pointer
    BOOL fRet = PostQueuedCompletionStatus( m_hCompletionPort, 0, (DWORD) pmsg, pOverlapped );

//    TRACE( "Post returned %s for request: %x\n", fRet ? "TRUE" : "FALSE", (DWORD) pmsg );
    return fRet;
}


//*****************************************************************************
// Function:	PostAndWait
// Implements:	n/a
// Description:	Adds the msg to the queue and waits for the result
// Inputs:		pmsg -- pointer the the queue msg request
//				dwTime -- duration to wait for event (default = INFINITE)
// Outputs:		none
// Return:		
//				S_OK			Success
//				E_*				(values returned by called functions)
// Exceptions:	none
// Process:		n/a
//*****************************************************************************	
HRESULT CWorkQueue::PostAndWait( CQueueMsg* pmsg, OVERLAPPED *pOverlapped, DWORD dwTime )
{
	// add it to the msg queue
	if( !Post( pmsg, pOverlapped ) )
		return HRESULT_FROM_WIN32( GetLastError() );

	// and wait for the response..
	return pmsg->WaitForCompletion( dwTime );
}


//*****************************************************************************
// Function:	ProcessQueue
// Implements:	n/a
// Description:	Waits for msgs on the queue and executes their contents.  Main
//				processing loop for the worker threads
// Inputs:		none
// Outputs:		none
// Return:		none
// Exceptions:	none
// Process:		n/a
//*****************************************************************************	
void CWorkQueue::ProcessQueue()
{
	m_DoneSemaphore.Increment();

    // balance the increment in StartNewThread
    InterlockedDecrement( &m_cThreadWaitingOrStartingUp ); 

	// until we are asked to exit....
	for (;;) {
		CQueueMsg*	    pmsg = NULL;
        DWORD           dwBytes;
        LPOVERLAPPED    poverlapped;
        ULONG           timeoutUsed = m_timeout; // capture the timeout each iteration

        // Wait for more work. Note that the wait
        // discipline for a kernel queue / completion port is LIFO.
        InterlockedIncrement( &m_cThreadWaitingOrStartingUp );

		// wait until the completion port returns completion
        BOOL ioSuccess = GetQueuedCompletionStatus( m_hCompletionPort, &dwBytes, (DWORD*) &pmsg, 
                                                    &poverlapped, timeoutUsed );

		InterlockedIncrement( &m_nTotalRequestsQueued);
		
        ULONG cThreadsNowWaitingOrStartingUp = InterlockedDecrement( &m_cThreadWaitingOrStartingUp );
        
        if( pmsg != NULL )
        {
            // Something was dequeued. Implicitly, this must have been done successfully.
            //TRACE( "Thread id: %d received request: %x\n", GetCurrentThreadId(), (DWORD) pmsg );

            if( !m_fExit )
            {
                // Implement our simple little policy as to when to kick off a new thread.
                #ifdef VARIABLE_THREAD
					if( 0 == cThreadsNowWaitingOrStartingUp )
						StartNewThread();   // REVIEW: Handle failure case?

					// Do it! Time it if we can.
					__int64 timeStart, timeStop;
					BOOL    fTime;
					fTime = QueryPerformanceCounter( (LARGE_INTEGER*) &timeStart );
				#endif

    		    // get the message to perform the work (virtual function invocation)
				pmsg->PerformRequest(poverlapped);

                #ifdef VARIABLE_THREAD
					QueryPerformanceCounter( (LARGE_INTEGER*) &timeStop ) ;

					if( fTime )
						UpdateStatistics( timeStart, timeStop );
				#endif
            }
            else
                // we dequeued a request but we are trying to terminate -- fail it and allow
                // thread to return to drain the queue
                pmsg->Completed( E_FAIL );
        }
        else
        {
            // Nothing was dequeued
            if( !ioSuccess && ( GetLastError() == WAIT_TIMEOUT) )
            {
                if( m_fExit )
                    // Timed out during shutdown because our queue is now fully drained
                    break;
                else // !m_fExit
                {
					#ifdef VARIABLE_THREAD
						// If we time out during the normal course of events, and if we're not the
						// last thread in the pool, then we commit suicide. This is what brings the
						// size of the thread pool back down during a period of quiet that follows 
						// a period of intense activity.
						if( m_cThreadMinWaiting <= m_cThreadWaitingOrStartingUp )
						{
							//TRACE( "no work for %d ms: committing suicide...\n", timeoutUsed );
							break;
						}
					#endif
                }
            }
            else
            {
                // Either a NULL item was posted, or we got some unexpected error.

                // In practice, conditions where ioSuccess was FALSE but GetLastError() was zero 
                // have been observed during process termination sequences. In any case, this thread 
                // is going down.
                if( ioSuccess )
                {
                    // A NULL item posted. Try to get out gracefully, being careful to drain
                    // our queue.
                    ASSERT(m_fExit);
                    m_timeout = 0;
                }
                else
                {
                    // We got one of the strange errors. Be sure that the thread actually exits,
                    // at the possible expense of not maybe somehow fully draining the queue.
					ASSERT(0);
//                    TRACE("weird error 0x%08x...\n", GetLastError());
                    break;
                }
            }
        }
    }

    InterlockedDecrement( &m_cTotalThreads );
	SetEvent(m_hDoneEvent);
	m_DoneSemaphore.Decrement();

//    TRACE( "worker thread exiting\n" );
}


//*****************************************************************************
// Function:	UpdateStatistics
// Implements:	n/a
// Description:	collates timing statistics and adjusts minThreads and timeouts 
//				accordingly (see comments below
// Inputs:		start and stop time
// Outputs:		statistical data members are updated as are minThreads and timeout
// Return:		none
// Exceptions:	none
// Process:		n/a
//*****************************************************************************	
void CWorkQueue::UpdateStatistics( const __int64& tStart, const __int64& tStop )
{
    // Update our timings statistics that will tell us when to kill off excess threads.
    // 
    // What we do, having gathered sufficient data to be of some significance, is to adjust 
    // the timeout period to be way up there in the tail of the distribution of service times.
    // Having hit such a timeout, our threads will commit suicide, modulo keeping a few extra
    // threads around to handle fluctuations.
    //
    ULONG n = InterlockedIncrement( (LONG*) &m_n);
    //
    // Scale the elapsed time to get reasonable units. Mainly so that m_sumTSquared doesn't
    // overflow and so that most timings don't round down to zero.
    //
    __int64 t = (tStop - tStart) / m_scale;
    __int64 tSquared = t * t;
    
    m_sumT += t;
    m_sumTSquared += tSquared;

    // only start calculating after 1000 samples (minimum sample size is somewhat arbitrary..)
    const ULONG significanceThreshold = 1000;

    if( ( n > significanceThreshold ) && ( n % 32 == 0 ) ) // Don't adjust really very often: 
                                                           // just not worth it. 32 is arbitrary.
    {
        // When we read m_sumT and m_sumTSquared here, we might get slightly incorrect answers
        // due to how we add increments to them (non-atomic add? REVIEW: compiler __in64 addition).
        // However, since we're only seeking an approximate timeout hint in the first place, 
        // we live with it.
        double sumT        = (double) m_sumT;
        double sumTSquared = (double) m_sumTSquared;

        //
        // Paranoia: We might be maybe just a chance going to overflow the counters. If we're
        // close, then reset the stats gathering machinery. REVIEW: We might consider doing
        // it more often just to make sure that the statistics reflect recent data and not
        // something from ages and ages and ages ago.
        //
        // 2^62 = 4.61169E+18
        //
        if( sumTSquared > 4.61169E+18 )
        {
            // We don't do this thread safe, 'cause we can't (REVIEW: see __int64 comment above). 
            // However, again, since we're only interested in approximations, it doesn't really much matter.
            m_n = 0;
            m_sumT = 0;
            m_sumTSquared = 0;
        }
        else
        {
            // Actually adjust the timeout period
            double S2           = (n * sumTSquared - sumT * sumT) / ( (double)n * (n-1) );
            double S            = sqrt(S2);
            double msS          = S * 1000.0 / m_timingUnits;           // in units of milliseconds
            double msMean       = sumT * 1000.0 / m_timingUnits / n;    // in units of milliseconds

            // timeout = the average time it takes each request to complete + 30 std deviations
            LONG  msNewTimeout  = (LONG)(msMean + 30.0 * msS);          // thirty std devs is pretty arbitrary

            // here we keep more threads as the standard deviation grows in proportion to the average time
            LONG  cThreadMin    = (LONG)(1.0 * msS / msMean + 1.0);     // #threads to keep on standby to handle fluctuation
            ASSERT(cThreadMin >= 1);

            //TRACE( "n=%d msS=%f, msMean=%f, new timeout=%d, cThreadMin=%d\n", n, msS, msMean, msNewTimeout, cThreadMin );

            m_timeout           = msNewTimeout;
            m_cThreadMinWaiting = cThreadMin;
        }
    }
}


//*****************************************************************************
// Function:	StartNewThread
// Implements:	n/a
// Description:	starts a thread used to feed the work queue thread pool
// Inputs:		none
// Outputs:		thread counts updated
// Return:		TRUE        success
//              FAIL        see below
// Exceptions:	none
// Process:		n/a
//*****************************************************************************	
BOOL CWorkQueue::StartNewThread()
{
    InterlockedIncrement( &m_cTotalThreads );

    DWORD   threadId;
    HANDLE  threadHandle = CreateThread( NULL, 0, WorkQueueThread, this, 0, &threadId );

    if( threadHandle != NULL )
    {
        InterlockedIncrement( &m_cThreadWaitingOrStartingUp );
        CloseHandle( threadHandle );
        return TRUE;
    }

    InterlockedDecrement( &m_cTotalThreads ); // counter our function entry on failure
    return FALSE;
}
    
//*****************************************************************************
// Function:	WorkQueueThread
// Implements:	n/a
// Description:	Pulls msgs from the CWorkQueue and processes them.
// Inputs:		pvWorkQueue - contains the CWorkQueue* associated with this q.
// Outputs:		none
// Return:		0
// Exceptions:	none
// Process:		n/a
//*****************************************************************************	
DWORD __stdcall CWorkQueue::WorkQueueThread( LPVOID pvWorkQueue )
{
//    TRACE( "Thread id: %d created...\n", GetCurrentThreadId() );

    // get the CWorkQueue* out of the passed parm.
	CWorkQueue* pQ = (CWorkQueue*) pvWorkQueue;

	// process msgs until we are asked to quit
	pQ->ProcessQueue();

	return 0;
}


// Class -- CQueueMsg

//*****************************************************************************
// Function:	constructor
// Implements:	n/a
// Description:	constructs the base object members
// Inputs:		fAutoDelete -- see data member in header
// Outputs:		none
// Return:		none
// Exceptions:	none
// Process:		n/a
//*****************************************************************************	
CQueueMsg::CQueueMsg( BOOL fAutoDelete ) : 
                    m_fAutoDelete( fAutoDelete ), m_hEventDone( NULL )
{
    // only concern ourselves with returning an HRESULT or firing an event if the
    // posting code wants to know....
    if( !m_fAutoDelete )
    {
    	m_hr = S_OK;

        // REVIEW: how do we catch a failure here? -- should have an Init() method?
        m_hEventDone = CreateEvent( NULL, FALSE, FALSE, NULL );
		ASSERT(m_hEventDone);
    }
}

//*****************************************************************************
// Function:	destructor
// Implements:	n/a
// Outputs:		none
// Return:		none
// Exceptions:	none
// Process:		n/a
//*****************************************************************************	
CQueueMsg::~CQueueMsg()
{
    // close the event if there is one
    ASSERT(m_hEventDone);
	if( m_hEventDone )
	{
        CloseHandle( m_hEventDone );
		m_hEventDone = NULL;
	}
}


//*****************************************************************************
// Function:	WaitForCompletion
// Implements:	n/a
// Description:	Waits until the request has been processed or a timeout was reached
// Inputs:		dwTime -- time to wait (or default = INFINITE) in milliseconds.
// Outputs:		none
// Return:		
//				S_OK	Success
//				E_FAIL	request timeout
//				E_*		value of m_hr as returned by the processing of this request
// Exceptions:	none
// Process:		n/a
//*****************************************************************************	
HRESULT	CQueueMsg::WaitForCompletion( DWORD dwWait )
{
    ASSERT( m_fAutoDelete == FALSE );

	// lock and return the value upon completion
	if( WaitForSingleObject( m_hEventDone, dwWait ) == WAIT_OBJECT_0 )
		return m_hr;
	
	return E_FAIL;	// we timed out on the event sem or it was abandoned
}

//*****************************************************************************
// Function:	CheckForCompletion
// Implements:	n/a
// Description:	checks to see if operation has completed yet...
// Inputs:		none
// Outputs:		none
// Return:		TRUE    operation complete (or thread may have died...)
//              FALSE   not completed
// Exceptions:	none
// Process:		n/a
//*****************************************************************************	
BOOL CQueueMsg::CheckForCompletion() 
{ 
    ASSERT( !m_fAutoDelete ); 
    return WaitForSingleObject( m_hEventDone, 0 ) != WAIT_TIMEOUT;
}

