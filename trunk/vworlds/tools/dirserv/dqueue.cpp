// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include "dqueue.h"

//DSPriorityQ stuff.
BOOL DSPriorityQ::find(MSG inp)
{
	//do a linear search through the list. starting from the lowest priority.
	POSITION pos = m_list.GetHeadPosition();
	
	DSRequest * pcurr;
	
	while( pos != NULL )
	{
		pcurr = &m_list.GetNext(pos);
	
		if(pcurr->getEvent()==DSWindowsMessage)
		{
			MSG msg=(*pcurr).getMSG();
			if(msg.message==WM_TIMER && 
				msg.hwnd==inp.hwnd &&
				msg.wParam==inp.wParam &&
				msg.lParam==inp.lParam)
			{
				return TRUE;
			}
		}
	} 
	return FALSE;
}

DSRequest DSPriorityQ::get()
{		
	return m_list.RemoveTail();
}

CString DSPriorityQ::dump()
{
	CString outstr;

	POSITION pos = m_list.GetTailPosition();
	
	DSRequest * pcurr;
	long cIndex=1;
	while( pos != NULL )
	{
		pcurr = &m_list.GetPrev(pos);
	
		outstr+=str(cIndex)+") "+pcurr->dump()+".\r\n";

		cIndex++;
	} 
	
	if(m_list.GetCount()==0) outstr+="  (Queue is empty right now)\r\n";
	return outstr;
}

void DSPriorityQ::put(DSRequest input)
{
	//do a linear search through the list. starting from the front.
	POSITION pos = m_list.GetHeadPosition();
	
	DSRequest * curr=NULL;
	POSITION prevpos;
	while( pos != NULL )
	{
		prevpos=pos;
		curr = &m_list.GetNext(pos);
		if((*curr)<input)
		{
			m_list.InsertBefore(prevpos,input);		
			return;
		}
	} 
	//it reached all the way to the end. So it's the last element.
	m_list.AddTail(input);
}

//DSRequest stuff.--------------
inline BYTE DSRequest::ComputePriority()
{
	//We offer 256 levels of priority here.
	//lowest number is the highest priority. 0 is the highest, 255 is the lowest.

	//REVIEW: These are sensitive. Ex: if I deal with ShutDownCommThread first,
	//then currently vwcomm doesn't like shutdown before all messages are purged.
	switch(m_event)
	{
		case DSEMPTY:
			return 100;
		default:
			//normal priority.
			return 50;
	}
}

inline BOOL DSRequest::operator<(DSRequest & right)
{
	//Priority treatment of events.
	//We service all highest priority events first, then next lower priority, etc.
	//Within the same priority, we do first come, first serve.
	BYTE lpri=ComputePriority();
	BYTE rpri=right.ComputePriority();

	if(lpri==rpri)
	{
		//they're equal priority. Then use first come, first serve.
		return m_lPriority<right.m_lPriority;
	}
	else if(lpri>rpri)
		return FALSE;
	else 
		//lpri<rpri
		return TRUE;
}

CString DSRequest::dump()
{
	CString outstr;
	switch(m_event)
	{
		case DSEMPTY:
			outstr="Idle. (Empty)";
			break;
		case DSShutDownCommThread:
			outstr="ShutDownCommThread";
			break;
		case DSSyncInitCommThread:
			outstr="DSSyncInitCommThread";
			break;
		case DSTimerMsg: 
		case DSWindowsMessage:
			outstr="Winmsg";
			break;
		case RequestConnect:
			outstr="RequestConnect";
			break;
		case RequestRemoveAll:
			outstr="RequestRemoveAll";
			break;
		case RequestAddAttrib:
			outstr="RequestAddAttrib";
			break;
		case RequestDisconnect:
			outstr="RequestDisconnect";
			break;
		case RequestUploadChanges:
			outstr="RequestUploadChanges";
			break;
		case RequestUpload:
			outstr="RequestUpload";
			break;		
		case RequestDownload:
			outstr="RequestDownload";
			break;
		case RequestFindAttrib:
			outstr="RequestFindAttrib";
			break;
		case RequestDeleteObject:
			outstr="RequestDeleteObject";
			break;
		case RequestTestQueue:
			outstr="RequestTestQueue";
			break;
		case RequestEnumerate:
			outstr="RequestEnumerate";
			break;
		default:
			ASSERT(0);
			break;
	}
	outstr+=", Priority: "+str(ComputePriority())+", Entry #: "+str(m_lPriority);
	return outstr;
}

// DSRequestQueue stuff///////////////////////////////////////////////////////////
DSRequestQueue::DSRequestQueue() 
{
	InitializeCriticalSection(&m_critsec);		
	EnterCriticalSection(&m_critsec);
	
	VERIFY(m_hEmptyEvent = CreateEvent(NULL,FALSE,FALSE,NULL));
	m_PriQueue.RemoveAll();
	m_dwTotalRequests = 0;		
	m_dwThreadWorker = NULL;
	m_dwWindowsMsgRequests = 0;	
	m_dwCurrentMsgs = 0;	
	m_dwCurrentTimers = 0;	
	m_dwThreadUI = NULL;

	m_dwThreadWorker = NULL;
	m_hThreadWorker = NULL;

	m_pSetRequests = new DSTSSetOfRequests;

	m_pSetRequests->setQueueAlive(TRUE);

	LeaveCriticalSection(&m_critsec);
}

void DSRequestQueue::Terminate(void)
{
	EnterCriticalSection(&m_critsec);
	
	m_PriQueue.RemoveAll();
	
	if (m_hEmptyEvent != NULL)
	{
		CloseHandle(m_hEmptyEvent);
		m_hEmptyEvent = NULL;
	}

	if (m_hThreadWorker != NULL)
	{
		CloseHandle(m_hThreadWorker);
		m_hThreadWorker = NULL;
	}

	if (m_pSetRequests)
	{
		m_pSetRequests->setQueueAlive(FALSE);
		m_pSetRequests->Release();
		m_pSetRequests = NULL;
	}

	LeaveCriticalSection(&m_critsec);	
}

DSRequestQueue::~DSRequestQueue() 
{ 
	Terminate();

	DeleteCriticalSection(&m_critsec);
}

DSRequest DSRequestQueue::get() 
{
	EnterCriticalSection(&m_critsec);
	
	FlushWindowsQueue();
	
	DSRequest temp;
	if (m_PriQueue.IsEmpty()) 
	{
		temp.setEvent(DSEMPTY);
		SetEvent(m_hEmptyEvent);
	}
	else 
	{
		temp=m_PriQueue.get();

		//set stats
		if(temp.getEvent()==DSWindowsMessage) 
		{
			m_dwCurrentTimers--;
		}
		if (m_PriQueue.IsEmpty()) SetEvent(m_hEmptyEvent);			
	}
	m_strCurrentRequest=temp.dump();

	LeaveCriticalSection(&m_critsec);
	return temp;
}

void DSRequestQueue::testQueue()
{
/*	DSRequest req,req2;
	HRESULT hr=S_OK;

	req.testQueue(3);
	req2=putSync(req);
	ASSERT(req2.getHR()==3);
	MessageBeep(0xFFFFFFFF);

	#ifdef _DEBUG
		afxDump<<"TestQueue: DSRequest # "<<(long)req2.getPutSync()<<".\r\n";
	#endif

	req.testQueue(4);
	req2=putSync(req);
	ASSERT(req2.getHR()==4);
	MessageBeep(0xFFFFFFFF);

	#ifdef _DEBUG
		afxDump<<"TestQueue: DSRequest # "<<(long)req2.getPutSync()<<".\r\n";
	#endif
	*/
}


CString DSRequestQueue::dump()
{
	EnterCriticalSection(&m_critsec);
		
		CString output;
		
		output="Current request: "+m_strCurrentRequest+".\r\n";

		// Print Current statistics of queue.
		output+="Current statistics of queue:\r\n";
		output+="   Total reqs: "+str(size())+".\r\n";

		// Print Cumulative statistics.
		output+="Cumulative statistics:\r\n";
		output+="   Total reqs: "+str(m_dwTotalRequests)+".\r\n";

		output+="Queue contents:\r\n"+m_PriQueue.dump();
		
	LeaveCriticalSection(&m_critsec);	
	return output;
}

void DSRequestQueue::put(DSRequest inp)
{
	if(!m_dwThreadWorker)
	{
		return;
	}

	EnterCriticalSection(&m_critsec);		

	//if it's a windows msg, check if it's a timer already in queue.
	if(inp.getEvent()==DSWindowsMessage && m_PriQueue.find(inp.getMSG()))
	{
		LeaveCriticalSection(&m_critsec);	
		return;
	}

	//Set statistics
	if(inp.getEvent()==DSTimerMsg) 
	{
		m_dwTotalTimerRequests++;
	}
	else if(inp.getEvent()==DSWindowsMessage) 
	{
		m_dwWindowsMsgRequests++;
		//mostly animator timers.
		m_dwCurrentTimers++;
	}
	
	m_dwTotalRequests++;

	//enter into queue.
	inp.setPriority(m_dwTotalRequests);

	m_PriQueue.put(inp);
	
	// If shutdown, add another one to freeze primary thread until this is done.
	if (inp.getEvent() == DSShutDownCommThread) 
	{
		m_dwTotalRequests++;
		inp.setPriority(m_dwTotalRequests);
		m_PriQueue.put(inp);		
	}

	// Don't bother checking return value of PostThreadMessage - it will fail
	// if the queue is full (something that is ok)
	
	ASSERT(m_dwThreadWorker);
	
	::PostThreadMessage(m_dwThreadWorker,WM_NULL, NULL, NULL);

	ResetEvent(m_hEmptyEvent); 
	
	LeaveCriticalSection(&m_critsec);	
}

UINT DSRequestQueue::size() 
{
	EnterCriticalSection(&m_critsec);

	long temp=m_PriQueue.size();
	
	LeaveCriticalSection(&m_critsec);	
	return temp;
}

BOOL DSRequestQueue::isEmpty() 
{ 
	EnterCriticalSection(&m_critsec);
	
	BOOL temp=m_PriQueue.IsEmpty();
	
	LeaveCriticalSection(&m_critsec);
	return temp;
}

void PostMessage(DSRequest req)
{
	DWORD dwEntryNumber=0, dwThreadID=0;
	req.getPutSync(dwEntryNumber, dwThreadID);
	ASSERT(dwThreadID);

	TRACE("FiringSignalToPutSync to thread: %i, Entry: %i.\n", dwThreadID,dwEntryNumber);

	BOOL bSuccess = PostThreadMessage(dwThreadID,WM_SYNC_CALL_FINISHED,dwEntryNumber,NULL);
	
	if (bSuccess == FALSE)
	{
		if (::GetLastError() == ERROR_INVALID_THREAD_ID)
		{
			TRACE("RequestQueue::FireSignalToPutSync: invalid thread, PostThreadMessage failed\n");
			ASSERT(0);
		}
		else
		{
			TRACE("RequestQueue::FireSignalToPutSync: unknown error (%x), PostThreadMessage failed\n", ::GetLastError());
			ASSERT(0);
		}
	}
}

//pass the return value, /w the correct message # receipt.
void DSRequestQueue::FireSignalToPutSync(DSRequest req)
{
	EnterCriticalSection(&m_critsec);

	m_pSetRequests->insert(req);
	PostMessage(req);

	LeaveCriticalSection(&m_critsec);		
}


//create a stack of msgs for below's use.
class CStackOMsgs : public CList<MSG, MSG>
{
public:
    // Add element to top of stack
    void Push( MSG msg)
        { AddHead( msg); }

    // Peek at top element of stack
    BOOL Pop(MSG &msg)
        { if(IsEmpty()) return FALSE;
			else msg=RemoveHead(); return TRUE;}

    // Pop top element off stack
    MSG Pop()
        { return RemoveHead(); }
};

void DSRequestQueue::SetWorkerThread()
{
	if(NULL==m_dwThreadWorker) 
		m_dwThreadWorker=::GetCurrentThreadId();
	else
	{
		ASSERT(m_dwThreadWorker==::GetCurrentThreadId());
	}

	if(NULL==m_hThreadWorker)
	{
		VERIFY(DuplicateHandle(
		GetCurrentProcess(),  // handle to the source process
		GetCurrentThread(),         // handle to duplicate
		GetCurrentProcess(),  // handle to process to duplicate to
		&m_hThreadWorker,  // pointer to duplicate handle
		NULL,    // access for duplicate handle
		TRUE,      // handle inheritance flag
		DUPLICATE_SAME_ACCESS));

		ASSERT(m_hThreadWorker);
	}
};

void DSRequestQueue::SetWorkerThread(DWORD dwThreadID, HANDLE hThread)
{
	if(NULL==m_dwThreadWorker) 
		m_dwThreadWorker=dwThreadID;
	else
	{
		ASSERT(m_dwThreadWorker==dwThreadID);
	}

	if(NULL==m_hThreadWorker)
	{
		VERIFY(DuplicateHandle(
		GetCurrentProcess(),  // handle to the source process
		hThread,         // handle to duplicate
		GetCurrentProcess(),  // handle to process to duplicate to
		&m_hThreadWorker,  // pointer to duplicate handle
		NULL,    // access for duplicate handle
		TRUE,      // handle inheritance flag
		DUPLICATE_SAME_ACCESS));

		ASSERT(m_hThreadWorker);
	}
};

//on thread can only call putSync once on the callstack.
DSRequest DSRequestQueue::putSync(DSRequest req, BOOL bKeepUIActive)
{	
	HANDLE hEvents[1]; 
	DWORD dwWaitResult;
	int iDebug=0;
	HRESULT hr = S_OK;

	ASSERT(m_pSetRequests);
	if(!m_pSetRequests || !m_dwThreadWorker)
	{
		DSRequest req;
		req.setHR(E_FAIL);
		return req;
	}

	m_pSetRequests->AddRef();
	DSTSSetOfRequests *pSetRequests = m_pSetRequests;

	//not important what this handle is.
	hEvents[0]=m_hThreadWorker;
	ASSERT(hEvents[0]);
	EnterCriticalSection(&m_critsec);
		DSRequest returnValue;		
		
		//put into queue.
		req.putSync(m_dwTotalRequests);		
		put(req);
		
	//leave the critical section so others can put stuff in.		
	LeaveCriticalSection(&m_critsec);	

	//Wait for message to come.
	MSG msg;
	BOOL bRet;
	BOOL bQuit=FALSE;
	
	//
	//Note that we have the possibility of this call stack:
	// putsync (A)
	//    wait for (A).
	//        putsync (B).
	//            wait for (B).
	//                 (A) appears.
	//                 (B) appears.

	CString strMsg = "PutSync threadID:"+str(::GetCurrentThreadId())+"\n";
	TRACE(strMsg);

	DSTSSetOfRequests::FindResult fr;
				
	while(!bQuit)
	{
		dwWaitResult = MsgWaitForMultipleObjects(1,hEvents,FALSE,1000,QS_ALLEVENTS);
	
		if(dwWaitResult==WAIT_TIMEOUT)
			TRACE("PutSync::MsgWait timed out: %i\n", iDebug++);
		else if((dwWaitResult>=WAIT_OBJECT_0) && (dwWaitResult<= (WAIT_OBJECT_0 + 2)))
			TRACE("PutSync::MsgWait got a signal: %i\n", iDebug++);
		else if((dwWaitResult>=WAIT_ABANDONED_0) && (dwWaitResult <= (WAIT_ABANDONED_0 + 2 - 1 )))
			TRACE("PutSync::MsgWait abandoned: %i\n", iDebug++);
		else if((dwWaitResult>=WAIT_IO_COMPLETION) && (dwWaitResult <= (WAIT_IO_COMPLETION + 2 - 1)))
			TRACE("PutSync::MsgWait IO_COMPLETION: %i\n", iDebug++);		
		else if(dwWaitResult==0xFFFFFFFF)
		{
			TRACE("MsgWait failed: %i\n", iDebug++);		
			DWORD dwError = GetLastError();	
			LPVOID lpMsgBuf;
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
			    dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf,    0,    NULL );
			TRACE((LPTSTR)lpMsgBuf);
			LocalFree( lpMsgBuf );

			//This is caused by the invalid handle of the closed thread. It's okay.
		}
		else ASSERT(0);
		
		bRet = ::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
		
		while(bRet && !bQuit)
		{
			bRet = ::GetMessage(&msg, NULL, 0, 0);
			
			ASSERT( (int)bRet != -1 );
			if (bRet)
			{
				if(msg.message==WM_SYNC_CALL_FINISHED)
				{				 			    
					ASSERT(msg.hwnd==NULL);

					DSRequest tempRetvalue;
					tempRetvalue.putSync(msg.wParam);

					//look for a response of ID tempRetValue. This will fill the rest of the struct out.
					fr = pSetRequests->find(tempRetvalue,tempRetvalue);
					if(fr == DSTSSetOfRequests::FR_TERMINATE)
					{
						TRACE("PutSync: The worker thread has terminated. Quiting without waiting for Signal\n.");
						bQuit=TRUE;					
					}
					else if(fr == DSTSSetOfRequests::FR_FOUND)
					{
						if(tempRetvalue==req)
						{
							//the operation is finished.
							bQuit=TRUE;
							
							//TODO: fire off event.
							returnValue=tempRetvalue;
						}
						else
						{
							//it's a different one.
							//check, just in case, if our req is in the map.
							fr = pSetRequests->find(req,returnValue);
							if(fr == DSTSSetOfRequests::FR_TERMINATE)
							{
								TRACE("PutSync: The worker thread has terminated. Quiting without waiting for Signal\n.");
								bQuit=TRUE;
							}
							else if(fr == DSTSSetOfRequests::FR_FOUND)
							{
								//The only time this should happen is below, the case where we're currently in 
								// a dispatch message.
								ASSERT(0);
								pSetRequests->remove(req);
								
								bQuit=TRUE;					
							}
						}
					}
				}
				else
				{
					// TRACE("DSQueue::PutSync: Got a non-queue message: %x.\n", msg.message);
					
					if(bKeepUIActive)
						::DispatchMessage(&msg);
							
					DSRequest outReq;
					//sets returnValue if found.
					fr = pSetRequests->find(req,returnValue);
					if(DSTSSetOfRequests::FR_FOUND == fr)
					{
						pSetRequests->remove(req);
						
						bQuit=TRUE;					
					}
					else if(DSTSSetOfRequests::FR_TERMINATE == fr)
					{
						bQuit=TRUE;					
					}
				}
			}

			bRet = ::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);		
		}
	}

	pSetRequests->Release();

	return returnValue;
}	

void DSRequestQueue::WaitUntilNotEmpty()
{
	MSG msg;
	BOOL bRet;

	SetWorkerThread();

	if(m_PriQueue.size()>0) return;

	// wait until there's a msg.
	bRet= ::GetMessage(&msg, NULL, 0, 0);
	ASSERT( (int)bRet != -1 );

	if (bRet)
		HandleMessage(msg);	
}

void DSRequestQueue::FlushWindowsQueue()
{
	MSG msg;

	// Flush win queue. No error checking needed: PeekMessage never fails.
	int i=0;
	while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		//prevent infinite loop.
		if(msg.message==WM_TIMER && i>2) 
			return;

		HandleMessage(msg);	
		i++;
	}	
}

void DSRequestQueue::HandleMessage(MSG & msg) 
{
	// if it's not the NULL message (used just for triggering the queue),
	if(msg.message != WM_NULL)
	{
		DSRequest req(msg);
		put(req);
	/*	// dispatch it later.
		::DispatchMessage(&msg);*/
	}
}
