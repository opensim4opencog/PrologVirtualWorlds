// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <afxtempl.h>
#include "global.h"

//queue.h

//-----Threadsafe event queue related classes below.------

//Identifies all the kinds of requests threads can make to CommThread.
enum EventType
{
	EMPTY = 0, 	EventPutSync, MessageArrived, ShutDownCommThread, TimerMsg, SyncInitCommThread,

	ServerDirectedLogoff, ServerDirectedDisconnect,
	ServerDirectedCancel, SocketDisconnect,  SetDisconnectTimerReq,
	KillDisconnectTimerReq, WindowsMessage, ReconnectRequest, DirServMsg,
	EventSaveWorld,EventSaveWorldScript,EventTakeWorldOffline,EventTakeWorldOnline,EventDeleteWorld,
	EventLoadWorld, EventOpenWorld, EventSaveWorldAs, EventPerformanceTest
};		

//For Request's members.
interface IVWCommConnection;
interface IVWCommMessage;

//This class constructed and placed into the RequestQueue.
//The interface is fat because of the many purposes this class has to serve.
//**this class has to be threadsafe. if you add a string, use critsecs!
class Request
{
	/* For most events, only get/setEvent is significant (or the appropriate constructor).
	For SocketDisconnect, get/setConn() is relevant.
	For TimerMsg, get/setTimerID() is relevant.
	For SetTimerReq, get/setTimerStringID() and get/setTimerDuration, is relevant.
	For KillTimerReq, get/setTimerID() is relevant.
	For MessageArrived, get/setVWMsg and VWComm's Getmessage is.
	*/
public:
	//Property Get/Set's.
	EventType getEvent() {return m_event;}
	void setEvent(EventType t) {m_event=t;}
	
	IVWCommConnection * getConn() {return m_pConnPoint;}
	void setConn(IVWCommConnection * t) {m_pConnPoint=t;}
	
	long getTimerID() {return m_uiTimerLongID;} 
	void setTimerID(long t) {m_uiTimerLongID=t;}
	
	UINT getTimerDuration() {return m_uiTimerDuration;}
	void setTimerDuration(UINT t) {m_uiTimerDuration=t;}
	
	//Windows message.
	MSG getMSG() {return m_msg;}
	
	//VWCOMM message.
	IVWCommMessage * getVWMsg() {return m_pVWmsg;}
	void setVWMsg(IVWCommMessage * t) {m_pVWmsg=t;}
	
	//Server synchronous requests.
	void SaveWorld(CString str) {m_event = EventSaveWorld; m_strURL = str;}
	void getSaveWorld(CString & str) {ASSERT(m_event==EventSaveWorld); str = m_strURL;}
	
	void SaveWorldAs(CString worldname, CString newWorldName) {m_event = EventSaveWorldAs; m_strAlias=newWorldName; m_strURL = worldname;}
	void getSaveWorldAs(CString &worldname, CString &newWorldName) {ASSERT(m_event==EventSaveWorldAs); worldname=m_strURL;newWorldName=m_strAlias;}
	
	void OpenWorld(CString str) {m_event = EventOpenWorld; m_strURL = str;}
	void getOpenWorld(CString & str) {ASSERT(m_event==EventOpenWorld); str = m_strURL;}
	
	void SaveWorldScript(CString str,CString scriptname) {m_event = EventSaveWorldScript; m_strURL = str; m_strAlias = scriptname;}
	void getSaveWorldScript(CString & str,CString & scriptname) {ASSERT(m_event==EventSaveWorldScript); str = m_strURL; scriptname=m_strAlias;}
	
	void TakeWorldOffline(CString str) {m_event = EventTakeWorldOffline; m_strURL = str;}
	void getTakeWorldOffline(CString & str) {ASSERT(m_event==EventTakeWorldOffline); str = m_strURL;}
	
	void TakeWorldOnline(CString str) {m_event = EventTakeWorldOnline; m_strURL = str;}
	void getTakeWorldOnline(CString & str) {ASSERT(m_event==EventTakeWorldOnline); str = m_strURL;}
	
	void DeleteWorld(CString str) {m_event = EventDeleteWorld; m_strURL = str;}
	void getDeleteWorld(CString & str) {ASSERT(m_event==EventDeleteWorld); str = m_strURL;}
	
	void PerformanceTest(CString str,long i) {m_event = EventPerformanceTest; m_strURL = str; m_iLong = i;}
	void getPerformanceTest(CString & str, long & i) {ASSERT(m_event==EventPerformanceTest); str = m_strURL; i = m_iLong;}

	void InitCommThread() {m_event = SyncInitCommThread;}

	//======================CRUCIAL STUFF=======================================
	//Constructors.
	Request(EventType e): m_event(e), m_pConnPoint(NULL), m_lPriority(ULONG_MAX) {}
	
	Request(): m_event(EMPTY), m_pConnPoint(NULL), m_lPriority(ULONG_MAX) {}

	Request(MSG msg): m_event(WindowsMessage), m_msg(msg) {} 
	
	//other stuff	
	void setPriority(ULONG t) {m_lPriority=t;}

	//This defines the priority in the PriorityQueue. 
	BOOL operator<(Request & t);

	BOOL operator==(Request & t)
	{
		return (t.m_EntryNumber == m_EntryNumber);
	}

	void putSync(DWORD entrynumber) 
	{
		//m_event=EventPutSync;
		m_EntryNumber=entrynumber;

		m_dwThreadUI=::GetCurrentThreadId();
	}

	void putSync(Request req) 
	{
		//m_event=EventPutSync;
		m_EntryNumber=req.m_EntryNumber;

		m_dwThreadUI=req.m_dwThreadUI;
	}

	void getPutSync(DWORD &entryNumber, DWORD &ThreadID) {entryNumber=m_EntryNumber; ThreadID=m_dwThreadUI;}

	void setHR(HRESULT hr) {m_hr=hr;}
	HRESULT getHR() {return m_hr;}

	void setHRText(CString hr) {m_hrText=hr;}
	CString getHRText() {return m_hrText;}

	CString dump();

private:
	//helper fcn
	BYTE ComputePriority();
	
	//data
	CComPtr<IVWCommMessage> m_pVWmsg;
	MSG m_msg;
	UINT m_uiTimerDuration, m_uiTimerLongID;
	
	CString m_strPassword,m_strAlias,m_strURL;

	long m_iLong;

	//used for the Disconnect event.
	IVWCommConnection * m_pConnPoint; 

	//CRUCIAL STUFF BELOW
	ULONG m_lPriority; //this is set by RequestQueue to tell us who came first.
	EventType m_event;

	HRESULT m_hr;
	CString m_hrText;
	//putsync stuff.
	DWORD m_dwThreadUI;
	DWORD m_EntryNumber; //nobody writes to this but putsync!
};

class PriorityQ
{
public:
	// The operations
	BOOL IsEmpty() {return m_list.IsEmpty();}

	Request get();

	void put(Request);

	UINT size() {return m_list.GetCount();}

	void RemoveAll() {m_list.RemoveAll();}

	CString dump();

	//For assuring that the same windows timer msg
	//is not queue'd twice. 

	BOOL find(MSG);
protected:	
	CList<Request,Request&> m_list;
};


//Threadsafe set of requests, indexed by m_EntryNumber
class TSSetOfRequests 
{
public:
	enum FindResult {FR_FOUND, FR_NOTFOUND, FR_TERMINATE};

	void AddRef(void) 
	{
		EnterCriticalSection(&m_critsec);
			m_iRefCount++;
		LeaveCriticalSection(&m_critsec);	
	};
	void Release() 
	{
		EnterCriticalSection(&m_critsec);
			--m_iRefCount;
			if(!m_iRefCount) 
			{
				LeaveCriticalSection(&m_critsec);	
				delete this;
				return;
			}
		LeaveCriticalSection(&m_critsec);	
	}

	TSSetOfRequests() : m_bIsQueueAlive(FALSE)
	{
		m_iRefCount = 1;
		InitializeCriticalSection(&m_critsec);
	}

	~TSSetOfRequests() 
	{
		DeleteCriticalSection(&m_critsec);
	}
	
	void insert(Request req) 
	{
		EnterCriticalSection(&m_critsec);
			DWORD dwEntryNumber=0, dwThreadID=0;
			req.getPutSync(dwEntryNumber, dwThreadID);		
			m_map.SetAt(dwEntryNumber,req);
		LeaveCriticalSection(&m_critsec);	
	}

	FindResult find(Request req, Request & returnReq) 
	{		
		EnterCriticalSection(&m_critsec);
			FindResult frReturnValue;	

			DWORD dwEntryNumber=0, dwThreadID=0;
			req.getPutSync(dwEntryNumber, dwThreadID);		
			BOOL ret = m_map.Lookup(dwEntryNumber,returnReq);
			if(ret) frReturnValue = FR_FOUND;
			else 
			{
				if(m_bIsQueueAlive) 
					frReturnValue = FR_NOTFOUND;
				else 
					//if the queue is dead, quit even if not found.
					frReturnValue = FR_TERMINATE;
			}
		LeaveCriticalSection(&m_critsec);		

		return frReturnValue;
	}

	BOOL remove(Request req) 
	{
		EnterCriticalSection(&m_critsec);
			DWORD dwEntryNumber=0, dwThreadID=0;
			req.getPutSync(dwEntryNumber, dwThreadID);		
			BOOL ret = m_map.RemoveKey(dwEntryNumber);
		LeaveCriticalSection(&m_critsec);		
		
		return ret;
	}

	void setQueueAlive(BOOL b) {m_bIsQueueAlive = b;}

private:
	CMap<int,int,Request, Request&> m_map;
	CRITICAL_SECTION m_critsec;
	int m_iRefCount;
	BOOL m_bIsQueueAlive;
};

/*
	The central threadsafe data structure. All threads put(Request's) into
  this queue, and the CommThread get() from it.
  Unusual fcns are  
  WaitUntilNotEmpty(): The CommThread uses this function to wait until there's
	something to do. 
  WaitUntilEmpty(): The destruction (in ShutDown()) waits until the queue is
	flushed (knowing the threads cleaned up) before continuing its destruction.
*/
class RequestQueue
{
public:
	RequestQueue();

	void Terminate(void);

	~RequestQueue();

	// to set notification thread
	void SetWorkerThread(); 

	void SetWorkerThread(DWORD dwThreadID, HANDLE hThread); 

	Request get();
	
	void put(Request inp);

	Request putSync(Request req, BOOL bKeepUIActive=TRUE);

	UINT size();
	
	BOOL isEmpty();
	
	//used by commthread.
	void WaitUntilNotEmpty();
	
	//Statistics
	DWORD getTotalRequests() {return m_dwTotalRequests;	}
	
	DWORD getTotalNetworkRequestsProcessed() {return m_dwTotalNetworkRequestsProcessed;}
	DWORD getTotalNetworkRequestsReceived() {return m_dwTotalNetworkRequestsReceived;}
	DWORD getTotalNetworkRequestsInQueue() {return m_dwTotalNetworkRequestsInQueue;}
	
	DWORD getTotalTimerRequests() {return m_dwWindowsMsgRequests;}

	DWORD getWindowsMsgRequests() {return m_dwWindowsMsgRequests;}
	
	DWORD getAdminRequests() {return m_dwAdminRequests;}

	DWORD getCurrentMsgs() {return m_dwCurrentMsgs;}
	
	DWORD getTimersInQueue() {return m_dwTotalTimerRequestsInQueue;}

	DWORD getTotalTimersProcessed() {return m_dwTotalTimerRequestsProcessed;}

	DWORD getTotalAdminRequestsProcessed() {return m_dwTotalAdminRequestsProcessed;}

	DWORD getTotalAdminRequestsInQueue() {return m_dwAdminRequestsInQueue;}
		
	CString dump();

	void FireSignalToPutSync(Request req);

protected:
	//member fcns.	
	void FlushWindowsQueue();

	//must not be in a crit sec when you call this fcn.
	void HandleMessage(MSG & msg);
	
	//data	
	DWORD  m_dwThreadWorker;
	HANDLE m_hThreadWorker;
	
	HWND m_hCallbackWindowHandle;
	
	PriorityQ m_PriQueue;
	
	//Stats
	DWORD m_dwTotalRequests;
	DWORD m_dwTotalNetworkRequestsProcessed;
	DWORD m_dwTotalNetworkRequestsReceived;
	DWORD m_dwTotalTimerRequestsInQueue;
	DWORD m_dwTotalNetworkRequestsInQueue;
	DWORD m_dwTotalTimerRequests;	
	DWORD m_dwWindowsMsgRequests;	
	DWORD m_dwAdminRequests;	
	DWORD m_dwAdminRequestsInQueue;
	DWORD m_dwTotalTimerRequestsProcessed;
	DWORD m_dwTotalAdminRequestsProcessed;
	DWORD m_dwCurrentMsgs;	
	DWORD m_dwCurrentTimers;	

	CRITICAL_SECTION m_critsec;

	//debug use.
	CString m_strCurrentRequest;
	
	//Sync object.	
	HANDLE m_hEmptyEvent;

	//For putsync use.
	TSSetOfRequests *m_pSetRequests;

	BOOL m_bTerminating;
};

CString str(long in);

#endif
