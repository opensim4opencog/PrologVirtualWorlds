// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _DSQUEUE_H_
#define _DSQUEUE_H_

#include <afxtempl.h>
#include "global.h"

//Collection class. Maps from a DSAttribute string to below struct.
struct CAttrib
{
	CAttrib(): m_bIsSensitive(FALSE) {}
	CAttrib(CAttrib &in)
	{
		m_bstrVWorldAttrib=m_bstrVWorldAttrib;
		m_varValue=in.m_varValue;
		m_bIsSensitive=in.m_bIsSensitive;
	}
	CComBSTR m_bstrVWorldAttrib;
	CComVariant m_varValue;
	
	//can't set this property into DS after creation.
	BOOL m_bIsSensitive;  
};

//queue.h//////////////////////////////////////////////////////////////////

//-----Threadsafe event queue related classes below.------

//Identifies all the kinds of requests threads can make to CommThread.
enum DSEventType
{
	DSEMPTY = 0, DSShutDownCommThread, DSTimerMsg, DSWindowsMessage,
	DSEventPutSync, DSSyncInitCommThread,

	RequestConnect, RequestRemoveAll, RequestAddAttrib,
	RequestDisconnect,RequestUploadChanges,RequestUpload,
	RequestDownload,RequestFindAttrib,RequestDeleteObject,
	RequestEnumerate,
	RequestTestQueue
};		

//This class constructed and placed into the DSRequestQueue.
//The interface is fat because of the many purposes this class has to serve.
//It mediates information between the two threads.
class DSRequest
{
public:
	DSRequest(DSEventType e): m_event(e), m_lPriority(ULONG_MAX) {}
	DSRequest(): m_event(DSEMPTY), m_lPriority(ULONG_MAX) {}
	DSRequest(MSG msg): m_event(DSWindowsMessage), m_msg(msg) {} 

	DSEventType getEvent() {return m_event;}

	void setEvent(DSEventType t) {m_event=t;}

	//Windows message.
	MSG getMSG() {return m_msg;}

	void InitWorkerThread() {m_event = DSSyncInitCommThread;}

	CString dump();

	void setPriority(ULONG t) {m_lPriority=t;}

	void Connect(BSTR addr,BSTR user,BSTR passwd)
	{
		m_event=RequestConnect;
		m_bstr1=addr; m_bstr2=user; m_bstr3=passwd; 
	}
	void getConnect(BSTR * addr,BSTR * user,BSTR * passwd)
	{
		*addr=m_bstr1.Copy(); *user=m_bstr2.Copy(); *passwd=m_bstr3.Copy();
	}

	void AddAttrib(BSTR dsattrib, CAttrib attrib)
	{
		m_event=RequestAddAttrib;
		m_attrib=attrib; m_bstr1=dsattrib;
	}
	void getAddAttrib(BSTR *dsattrib,CAttrib &attrib)
	{
		*dsattrib=m_bstr1.Copy(); attrib=m_attrib;
	}

	void UploadObjectToDS(BSTR objectClass, BSTR objectKey)
	{
		m_event=RequestUpload;
		m_bstr1=objectClass;
		m_bstr2=objectKey;
	}
	void getUploadObjectToDS(BSTR *objectClass, BSTR *objectKey)
	{
		*objectClass=m_bstr1.Copy(); *objectKey=m_bstr2.Copy();
	}

	void DownloadObjectFromDS(BSTR objectClass, BSTR objectKey)
	{
		m_event=RequestDownload;
		m_bstr1=objectClass;
		m_bstr2=objectKey;
	}
	void getDownloadObjectFromDS(BSTR *objectClass, BSTR *objectKey)
	{
		*objectClass=m_bstr1.Copy(); *objectKey=m_bstr2.Copy();
	}
	
	void FindAttrib(BSTR a) 
	{
		m_event=RequestFindAttrib;
		m_bstr1=a;
	}
	void getFindAttrib(BSTR *a)
	{
		*a=m_bstr1.Copy(); 
	}
	void setReturnedAttrib(CAttrib a) {m_attrib=a;}
	CAttrib returnedAttrib() {return m_attrib;}
	
	void setHR(HRESULT hr) {m_hr=hr;}
	HRESULT getHR() {return m_hr;}

	void DeleteObjectFromDS(BSTR objectClass, BSTR objectKey)
	{
		m_event=RequestDeleteObject;
		m_bstr1=objectClass;
		m_bstr2=objectKey;
	}
	void getDeleteObjectFromDS(BSTR *objectClass, BSTR *objectKey)
	{
		*objectClass=m_bstr1.Copy(); *objectKey=m_bstr2.Copy();
	}

	void putSync(DWORD entrynumber) 
	{
		//m_event=EventPutSync;
		m_EntryNumber=entrynumber;

		m_dwThreadUI=::GetCurrentThreadId();
	}

	BOOL operator==(DSRequest & t)
	{
		return (t.m_EntryNumber == m_EntryNumber);
	}

	void putSync(DSRequest req) 
	{
		//m_event=EventPutSync;
		m_EntryNumber=req.m_EntryNumber;

		m_dwThreadUI=req.m_dwThreadUI;
	}

	void getPutSync(DWORD &entryNumber, DWORD &ThreadID) {entryNumber=m_EntryNumber; ThreadID=m_dwThreadUI;}

	void testQueue(HRESULT hr)
	{
		m_event=RequestTestQueue;
		m_hr=hr;
	}
	HRESULT getTestQueue() {return m_hr;}

	void Enumerate() {m_event=RequestEnumerate;}

	//This defines the priority in the PriorityQueue. 
	BOOL operator<(DSRequest & t);

private:
	BYTE ComputePriority();

	//data
		

	DWORD m_dword;

	//nobody writes to this but putsync!
	DWORD m_EntryNumber;

	//this is set by DSRequestQueue to tell us who comes first.
	ULONG m_lPriority; 
	
	DSEventType m_event;
	
	//Misc data.
	HRESULT m_hr;

	MSG m_msg;
	
	CComBSTR m_bstr1,m_bstr2,m_bstr3;

	BOOL m_bool;

	CComVariant m_var;

	CAttrib m_attrib;

	CString m_hrText;

	//putsync stuff.
	DWORD m_dwThreadUI;
	

};

class DSPriorityQ
{
public:
	// The operations
	BOOL IsEmpty() {return m_list.IsEmpty();}

	DSRequest get();
	
	void put(DSRequest);
	
	UINT size() {return m_list.GetCount();}
	
	void RemoveAll() {m_list.RemoveAll();}
	
	CString dump();
	//For assuring that the same windows timer msg
	//is not queue'd twice. 
	BOOL find(MSG);

protected:	
	CList<DSRequest,DSRequest&> m_list;
};

//Threadsafe set of requests, indexed by m_EntryNumber
class DSTSSetOfRequests 
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

	DSTSSetOfRequests() : m_bIsQueueAlive(FALSE)
	{
		m_iRefCount = 1;
		InitializeCriticalSection(&m_critsec);
	}

	~DSTSSetOfRequests() 
	{
		DeleteCriticalSection(&m_critsec);
	}
	
	void insert(DSRequest req) 
	{
		EnterCriticalSection(&m_critsec);
			DWORD dwEntryNumber=0, dwThreadID=0;
			req.getPutSync(dwEntryNumber, dwThreadID);		
			m_map.SetAt(dwEntryNumber,req);
		LeaveCriticalSection(&m_critsec);	
	}

	FindResult find(DSRequest req, DSRequest & returnReq) 
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

	BOOL remove(DSRequest req) 
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
	CMap<int,int,DSRequest, DSRequest&> m_map;
	CRITICAL_SECTION m_critsec;
	int m_iRefCount;
	BOOL m_bIsQueueAlive;
};


/*The central threadsafe data structure. All threads put(DSRequest's) into
  this queue, and the CommThread get() from it.
  Unusual fcns are  
  WaitUntilNotEmpty(): The CommThread uses this function to wait until there's
	something to do. 
  WaitUntilEmpty(): The destruction (in ShutDown()) waits until the queue is
	flushed (knowing the threads cleaned up) before continuing its destruction.
*/
class DSRequestQueue
{
public:
	DSRequestQueue();
	
	~DSRequestQueue();

	void Terminate(void);

	// to set notification thread
	// to set notification thread
	void SetWorkerThread(); 

	void SetWorkerThread(DWORD dwThreadID, HANDLE hThread); 

	DSRequest get();
	
	void put(DSRequest inp);
	
	DSRequest putSync(DSRequest req, BOOL bKeepUIAlive = TRUE);
	
	void FireSignalToPutSync(DSRequest req);

	UINT size();
	
	BOOL isEmpty();
	
	//used by commthread.
	void WaitUntilNotEmpty();

	//Statistics
	DWORD getTotalRequests() {return m_dwTotalRequests;	}
	
	DWORD getTotalTimerRequests() {return m_dwTotalTimerRequests;}

	DWORD getWindowsMsgRequests() {return m_dwWindowsMsgRequests;}
	
	DWORD getCurrentTimers() {return m_dwCurrentTimers;}

	void testQueue();
	//For debug.
	CString dump();

protected:
	//member fcns.	////////////////////////////////////////////////////	
	void FlushWindowsQueue();
	
	void HandleMessage(MSG & msg);
	
	//data	
	DWORD  m_dwThreadWorker;
	HANDLE m_hThreadWorker;
	
	DSTSSetOfRequests *m_pSetRequests;

	DSPriorityQ m_PriQueue;

	//stats
	DWORD m_dwTotalRequests;
	DWORD m_dwTotalTimerRequests;	
	DWORD m_dwWindowsMsgRequests;	

	DWORD m_dwCurrentMsgs;	
	DWORD m_dwCurrentTimers;	

	//debug use.
	CString m_strCurrentRequest;
	
	//putsync stuff.
	DWORD m_dwThreadUI;

	//Sync object.	
	HANDLE m_hEmptyEvent;
	
	CRITICAL_SECTION m_critsec;
};

CString str(long in);

#endif
