// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWServe.cpp : Implementation of CVWServerApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include <vwserver.h>
#include <inetfile.h>
#include <vwclient.h>
#include <vwodb.h>
#include "vwserve.h"
#include "commsite.h"
#include "global.h"
#include "urlhelp.h"
#include "reghelp.h"
#include "scripts.h"
#include "marshbuf.h"
#include "propsecu.h"
#include "math.h"
#include "..\fastcomm\commlayr.h"
#include <io.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _DEBUG
#include <crtdbg.h>
#endif

//#define TRACE_ALLOCATION
//#define VWSERVER_LOGOFF_DEBUG

EXTERN_C const CLSID CLSID_VWComm;
EXTERN_C const CLSID CLSID_FastComm;
EXTERN_C const CLSID CLSID_InternetFileManager;
EXTERN_C const CLSID CLSID_World;
EXTERN_C const CLSID CLSID_MarshallBuffer;
EXTERN_C const CLSID CLSID_PropertyMap;
EXTERN_C const CLSID CLSID_DServ;
EXTERN_C const CLSID CLSID_Authenticate;
EXTERN_C const CLSID CLSID_Authentication;
EXTERN_C const CLSID CLSID_PropertyList;

typedef CComObject<CMarshallBufferObject> CComMarshallBuffer;

/////////////////////////////////////////////////////////////////////////////
// Parameters

#define cchMaxURL			258
#define cchMaxMessage		512

/////////////////////////////////////////////////////////////////////////////
// local helper function

CString ToStr(int n, LPCTSTR pszFormat="%-d") // "%6d")
{
	CString str;

	str.Format(pszFormat, n);

	return str;
}

CString fToStr(double n, LPCTSTR pszFormat="%8.2f") // "%6d")
{
    CString str;

    str.Format(pszFormat, n);

    return str;
}


/////////////////////////////////////////////////////////////////////////////
// VWServer constructor/destructor================================================

CVWServer::CVWServer() :
	m_pcomm(NULL),
	m_pAuthenticate(NULL),
	m_pthreadComm(NULL),
	m_bIsShuttingDownArmed(FALSE),
	m_pTools(NULL),
	m_LoopState(0),
	m_pDServ(NULL),
	m_uiDisconnectTimerID(NULL),
	m_pCurrentConnection(NULL),
	m_bForceAuthentication(FALSE),
	m_bstrAuthenticationGroup(NULL),
	m_bWinNT(FALSE),
	m_dwMaxConnections(0)
{
#ifdef TRACE_ALLOCATION
	TRACE("CVWServer::CVWServer, this 0x%0x\n", this);
#endif

    ClearPerformanceCounters();

    // This is effectively a constant, so figure it out just once (milliseconds)
    LARGE_INTEGER liFreq;
    QueryPerformanceFrequency(&liFreq);
    m_dFreq = (liFreq.HighPart * 2^32 + liFreq.LowPart) / 1000.0;

}

CVWServer::~CVWServer()
{
#ifdef TRACE_ALLOCATION
	TRACE("CVWServer::~CVWServer, this 0x%0x\n", this);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// ErrorInfo

STDMETHODIMP CVWServer::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IVWServer
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

HRESULT CVWServer::FinalConstruct()
{
	HRESULT hr = S_OK;
	return hr;
}

/////////////////////////////////////////////////////////////////////////////
// IVWServer methods

STDMETHODIMP CVWServer::Accept(int iPort, int iOptionalCallbackWindowHandle)
{
	HRESULT hr = S_OK;
	Request req;
	OSVERSIONINFO osvi;

	m_hCallbackWindowHandle = (HWND) iOptionalCallbackWindowHandle;

	// verify platform before we do any communication
	osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	GetVersionEx (&osvi);

	// if we are running server on NT set appropriate flags
	if ((osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) && (osvi.dwMajorVersion >= 4))
	{
		m_bWinNT=TRUE;

		// load Authentication settings from registry
		if FAILED(GetRegAuthentication(&m_bForceAuthentication))
			m_bForceAuthentication=TRUE;

		// load local group name (if it exists) from registry.. reghelp.cpp
		if FAILED(GetRegLocalGroupName(&m_bstrAuthenticationGroup.m_str))
			m_bstrAuthenticationGroup="";
	}
	
	// also load the MaxUser limit (if it exists) from registry
		if FAILED(GetRegConnectionLimit(&m_dwMaxConnections))
			m_dwMaxConnections = 0;

	// initialize VWComm (FastComm on NT)
	if (m_bWinNT) 
		hr = CoCreateInstance(CLSID_FastComm, NULL, CLSCTX_INPROC_SERVER, IID_IVWComm, (void **) &m_pcomm);
	else
		hr = CoCreateInstance(CLSID_VWComm, NULL, CLSCTX_INPROC_SERVER, IID_IVWComm, (void **) &m_pcomm);
	if (FAILED(hr))
	{
		TRACE("CVWServer::Accept: CoCreateInstance of VWComm failed\n");
		goto ERROR_ENCOUNTERED;
	}
	
	hr = m_pcomm->Initialize(this);
	if (FAILED(hr))
	{
		TRACE("CVWServer::Accept: IVWComm::SetSite failed\n");
		goto ERROR_ENCOUNTERED;
	}

	// start thread to handle communications
	m_pthreadComm = AfxBeginThread(CVWServer::CommThread, this, THREAD_PRIORITY_NORMAL);

	if (m_pthreadComm == NULL)
	{
		TRACE("CVWServer::Accept: Failed to create communication thread\n");
		hr = VWSERVER_E_THREADCREATION;
		goto ERROR_ENCOUNTERED;
	}

	m_queue.SetWorkerThread(m_pthreadComm->m_nThreadID, m_pthreadComm->m_hThread);

	// now tell comm to accept connections
	hr = m_pcomm->Accept(iPort);

	//pause this thread to wait for other thread to initialize.
	req.InitCommThread();
	m_queue.putSync(req, TRUE);

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CVWServer::DisconnectWarning()
{
	// Put it on the m_queue.
	Request req(ServerDirectedLogoff);
	m_queue.put(req);

	// Start the clock.
	Request req2(SetDisconnectTimerReq);
	req2.setTimerDuration(LOGOFF_WAIT_TIME);
	m_queue.put(req2);

	return S_OK;
}

STDMETHODIMP CVWServer::Disconnect()
{
	// Send msg to thread 2.
	Request req(ServerDirectedDisconnect);
	m_queue.put(req);
	
	return S_OK;
}

STDMETHODIMP CVWServer::DisconnectCancel()
{
	// Send request.
	Request req(ServerDirectedCancel);
	m_queue.put(req);
	
	// Cancel timer.	
	Request req2(KillDisconnectTimerReq);
	m_queue.put(req2);
	
	return S_OK;
}

STDMETHODIMP CVWServer::ShutDown()
{
	// ask commthread to shut down.
	Request req(ShutDownCommThread);
	
	//pass it this request.
	Request returnValue=m_queue.putSync(req, FALSE);
	
	ASSERT(SUCCEEDED(returnValue.getHR()));
	//Wait until 2nd thread is done. Then go on.
	
	// The Commthread is dead now.
	m_pthreadComm = NULL;

	return S_OK;
}

STDMETHODIMP CVWServer::OnUserReconnect(BSTR bstrURL, BSTR bstrName, BSTR bstrPassword)
{	
	// this runs in commthread and is called by vwobject on the server side. Usually, by
	// a server side script module.
	
	HRESULT hr = S_OK;
	
	// TODO: For now, just ignore server side requests to reconnect.
	// get world, then get connection, then call below.
	// SendReconnectMessageToClient(pconn,hr,bstrURL,bstrName,bstrPassword);
	return S_OK;
}

STDMETHODIMP CVWServer::GetSimpleStatus(BSTR *bstrMsg)
{
	DWORD totalReq = m_queue.getTotalRequests();
	DWORD queueSize = m_queue.size();
	CString output;

	output = "There are "+str(queueSize)+" request(s) ahead of yours.";

	*bstrMsg = output.AllocSysString();
	
	return S_OK;
}

// returns a formatted string to display stats to users.
STDMETHODIMP CVWServer::GetStatusInfo(DWORD *pqueueSize, DWORD *ptotalReq, BSTR *bstrWorldInfo,
		DWORD *pdwTotalConnections, VARIANT_BOOL *pbIsDSConnected)
{
	*pqueueSize = m_queue.size();
	*ptotalReq = m_queue.getTotalRequests();
	*pdwTotalConnections = m_clist.GetCount();
	
	CString output;
	
	if (m_LoopState==1)
	{
		output = "Initializing...";
	}
	else if (m_LoopState==3)
	{
		output = "An error has occurred. The server cannot communicate with clients";
	}
	else
	{
		if (m_pDServ)
		{
			BSTR bstrds = NULL;
			m_pDServ->GetStatus(&bstrds,pbIsDSConnected);
			ASSERT(bstrds);

			output += "Directory Service Information: ";
			output += CString(bstrds);
			output += "\r\n";
			SAFEFREESTRING(bstrds);
		}

		// Do world info. Iterate through the map.
		output += m_wlist.Dump();

		// Print Current statistics of queue.
		output += "Client connections:\t\t\t\t\t\t\t" + ToStr(*pdwTotalConnections) + "\r\n";	
		output += "\r\n";

		// Queue statistics:
		DWORD dwTotalRequestsReceived = m_queue.getTotalRequests();
		DWORD dwTotalNetworkRequestsReceived = m_queue.getTotalNetworkRequestsReceived();
		DWORD dwTotalTimerRequestsReceived = m_queue.getTotalTimerRequests();	
		DWORD dwWindowsMsgRequestsReceived = m_queue.getWindowsMsgRequests();	
		DWORD dwAdminRequestsReceived = m_queue.getAdminRequests();

		DWORD dwTotalNetworkRequestsProcessed = m_queue.getTotalNetworkRequestsProcessed();
		DWORD dwTotalNetworkRequestsInQueue = m_queue.getTotalNetworkRequestsInQueue();

		output += "Queue statistics:\r\n";
		output += "\tCurrent message: " + m_queue.dump() + "\r\n";
		output += "\tTotal messages in queue:\t\t\t\t" + ToStr(m_queue.size()) + "\r\n";
		output += "\t\tNetwork msgs in queue:\t\t\t\t" + ToStr(dwTotalNetworkRequestsInQueue) + "\r\n";
		output += "\t\tAdministrative msgs in queue:\t\t" + ToStr(m_queue.getTotalAdminRequestsInQueue()) + "\r\n";
		output += "\t\tTimer msgs in queue:\t\t\t\t" + ToStr(m_queue.getTimersInQueue()) + "\r\n";

		
/*
// Removed for now.. Not enough screen real estate for all of this:
		output += "\tTotal received messages:\t\t\t\t" + ToStr(dwTotalRequestsReceived) + "\r\n";
		output += "\t\tNetwork msgs received:\t\t\t\t" + ToStr(dwTotalNetworkRequestsReceived) + "\r\n";
		output += "\t\tAdministrative msgs received:\t\t" + ToStr(dwAdminRequestsReceived) + "\r\n";
		output += "\t\tTimer msgs received:\t\t\t\t" + ToStr(m_queue.getTotalTimerRequests()) + "\r\n";
		output += "\tTotal processed messages:\t\t\t\t" + ToStr(m_queue.getTotalAdminRequestsProcessed()+m_queue.getTotalTimersProcessed()+dwTotalNetworkRequestsProcessed) + "\r\n";
		output += "\t\tNetwork msgs processed:\t\t\t\t" + ToStr(dwTotalNetworkRequestsProcessed) + "\r\n";
		output += "\t\tAdministrative msgs processed:\t\t" + ToStr(m_queue.getTotalAdminRequestsProcessed()) + "\r\n";
		output += "\t\tTimer msgs processed:\t\t\t\t" + ToStr(m_queue.getTotalTimersProcessed()) + "\r\n";
*/
        // Processing time statistics
        output += "\r\n\r\nProcessing Time (ms): Average ± Std. Dev. \r\n";        
        output += "\tWorld:\t\t\t\t";
        if (m_World.Cnt >0 )
            output += fToStr(m_World.Total / m_World.Cnt / m_dFreq ) + " ± " + fToStr(sqrt(m_World.SD / m_World.Total) / m_dFreq) + "  Last: " + fToStr(m_World.Last / m_dFreq ) + "     (" + fToStr(1000/ (m_World.Total / m_World.Cnt / m_dFreq)) + " trans/sec)" ;

        output +="\r\n\tEnter:\t\t\t\t";
        if (m_Enter.Cnt >0 ) 
            output += fToStr(m_Enter.Total / m_Enter.Cnt / m_dFreq ) + " ± " + fToStr(sqrt(m_Enter.SD / m_Enter.Total) / m_dFreq) + "  Last: " + fToStr(m_Enter.Last / m_dFreq );

        output +="\r\n\tLogon:\t\t\t\t";
        if (m_Logon.Cnt >0 ) 
            output += fToStr(m_Logon.Total / m_Logon.Cnt / m_dFreq ) + " ± " + fToStr(sqrt(m_Logon.SD / m_Logon.Total) / m_dFreq) + "  Last: " + fToStr(m_Logon.Last / m_dFreq );

        output +="\r\n\tCreate:\t\t\t\t";
        if (m_Create.Cnt >0 ) 
            output += fToStr(m_Create.Total / m_Create.Cnt / m_dFreq ) + " ± " + fToStr(sqrt(m_Create.SD / m_Create.Total) / m_dFreq) + "  Last: " + fToStr(m_Create.Last / m_dFreq );

        output +="\r\n\tDisconnect:\t\t\t";
        if (m_Disconnect.Cnt >0 ) 
            output += fToStr(m_Disconnect.Total / m_Disconnect.Cnt / m_dFreq ) + " ± " + fToStr(sqrt(m_Disconnect.SD / m_Disconnect.Total) / m_dFreq) + "  Last: " + fToStr(m_Disconnect.Last / m_dFreq );

        output +="\r\n\tReconnect:\t\t\t";
        if (m_Reconnect.Cnt >0 ) 
            output += fToStr(m_Reconnect.Total / m_Reconnect.Cnt / m_dFreq ) + " ± " + fToStr(sqrt(m_Reconnect.SD / m_Reconnect.Total) / m_dFreq) + "  Last: " + fToStr(m_Reconnect.Last / m_dFreq );
        
		output +="\r\n";

        // get and add some communication statistics
		if (m_pcomm!=NULL)		
		{
			int cPacketsSentTotal;
			int cPacketsReceivedTotal;
			int cBytesSentTotal;
			int cBytesReceivedTotal;
			int cPacketsReceivedPerSecond; 
			int cBytesReceivedPerSecond;   			
			int cPacketsSentPerSecond;     			
			int cBytesSentPerSecond;       			
			int cMeanPacketsInWriteQueue;  			
			int cMeanMessagesInReadQueue;  			
			int cMeanTimeInWriteQueue;     			
			int cMeanTimeInReadQueue;
			
			if (SUCCEEDED(m_pcomm->GetUsageStatistics(	&cPacketsSentTotal,
														&cPacketsReceivedTotal,
														&cBytesSentTotal,
														&cBytesReceivedTotal,
														&cPacketsReceivedPerSecond,
														&cBytesReceivedPerSecond,
														&cPacketsSentPerSecond,
														&cBytesSentPerSecond,
														&cMeanPacketsInWriteQueue,
														&cMeanMessagesInReadQueue,
														&cMeanTimeInWriteQueue,
														&cMeanTimeInReadQueue)))
			{
				// add stuff
				//output += "\r\n";

				//output += "Communication details:\r\n";
				//output += "\tPackets:\tsent: " + ToStr(cPacketsSentTotal) + ", received: " + ToStr(cPacketsReceivedTotal)	+ "\r\n";
				//output += "\tBytes:\t\tsent: " + ToStr(cBytesSentTotal) + ", received: " + ToStr(cBytesReceivedTotal)		+ "\r\n";
			}

			//add some fastcomm stats (fastcomm only on NT)
			if (m_bWinNT)
			{
				output+="\r\n";
				output+="FastComm Statistics:\r\n";
				output+=CConnection::GlobalDump();
				//output+=CDataExchanger::GlobalDump();
			}

			
		}

//		output += "Queue contents:\r\n" + m_queue.dump();

	
	}

	*bstrWorldInfo = output.AllocSysString();

	return S_OK;
}

///////////////////////////////////////////////////////////
// This method can be used to verify that the server queue
// properly behaves
STDMETHODIMP CVWServer::TestQueue()
{
	long pick;

	for(long i=0;i<10000;i++)
	{
		//pick=rand()%3;
		pick=0;			
		switch(pick)
		{
		case 0:
		{
			Request req(SocketDisconnect);
			req.setConn(NULL);
			m_queue.put(req);
			break;
		}
		case 1:
		{
			Request req(MessageArrived);
			req.setVWMsg(NULL);
			m_queue.put(req);
			break;
		}
		case 2:
		{
			MSG msg;
			msg.message=WM_TIMER;
			Request req(msg);
			m_queue.put(req);
			break;
		}
		}
	}
	//TRACE("Queue Stats: size "+ToStr(m_queue.size())+", contents:\n"+m_queue.dump());	
	
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////////
// CommThread routines.

// send administrative message to Client indicating logging off world.
HRESULT CVWServer::SendAdminMessageToClient(IVWCommConnection * pconn, BYTE option)
{	
	CString strMessage;
	CComMarshallBuffer* pbuffer = NULL;
	HRESULT replyhr = S_OK;
	HRESULT hr = S_OK;
	
	// compose the message
	// 'A' is for "Administrative" messages
	strMessage = 'A';		
	strMessage += option;		
	DWORD cbMessage = strMessage.GetLength()+1;

	// create marshall buffer
	pbuffer = new CComMarshallBuffer();
	if (pbuffer == NULL)
	{
		TRACE("CVWServer::Disconnect: failed to allocate CComMarshallBuffer\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	pbuffer->AddRef();

	// write out server version to send to client
	hr = pbuffer->put_ULong(PROTOCOL_VERSION);
	if (FAILED(hr))
	{
		TRACE("CVWServer::Disconnect: Failed to write to marshall buffer\n");
		goto ERROR_ENCOUNTERED;
	}

	hr = pbuffer->put_Bytes((BYTE *) (LPCSTR) strMessage, cbMessage);
	if (FAILED(hr))
	{
		TRACE("CVWServer::Disconnect: Failed to write to marshall buffer\n");
		goto ERROR_ENCOUNTERED;
	}	
	
	hr = pconn->SendAsync(pbuffer);	
	if (FAILED(hr))
	{
		TRACE("CVWServer::Disconnect: Failed to send Client admin message\n");
		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:

	SAFERELEASE(pbuffer);
	TRACE("CVWServer::Disconnect: Something went wrong with the send msg attempt.\n");
	return hr;
}

// Disconnect timer coming in.
void CVWServer::HandleTimerMsg_T2(UINT uiTimerID) 
{
	if(uiTimerID == getDSRefreshTimer())
	{
		RefreshDS();
	}
	else if (m_bIsShuttingDownArmed)
	{
		MessageBeep(0xFFFFFFFF);
		Disconnect();

		//Cancel timer.	
		Request req2(KillDisconnectTimerReq);
		m_queue.put(req2);
	}
}

HRESULT CVWServer::DisconnectWarning_T2()
{
	//Iterate through the connlist, give users SHUTDOWN_WAIT_TIME msecs for shutdown by sending
	//them a notification message. 
	TRACE("CVWServer::Disconnect: Sending warning to all clients.\n");

	POSITION pos;
	
	for (pos = m_clist.GetStartPosition(); pos != NULL;)
	{
		IVWCommConnection *pconn = NULL;
		ConnectionInfo cinfo;
	
		m_clist.GetNextAssoc(pos, pconn, cinfo);
		
		SendAdminMessageToClient(pconn, 'W'); //Warn'em.
	}	
	
	m_bIsShuttingDownArmed=TRUE;		
	
	return S_OK;
}

HRESULT CVWServer::DisconnectCancel_T2()
{
	TRACE("CVWServer::Disconnect: Sending cancel warning to all clients.\n");

	//Iterate through the connlist, tell users that
	//shutdown has been cancelled.
	
	if (!m_bIsShuttingDownArmed) 
	{
		FireOnReport(CComBSTR("CVWServer::DisconnectCancel_T2: There is no pending global disconnect in progress. Cancel failed.\n"), 0);
						
		return S_OK; //not armed, can't cancel. Do nothing.
	}
	
	POSITION pos;
	
	for (pos = m_clist.GetStartPosition(); pos != NULL;)
	{
		IVWCommConnection *pconn = NULL;
		ConnectionInfo cinfo;
	
		m_clist.GetNextAssoc(pos, pconn, cinfo);
		
		SendAdminMessageToClient(pconn,'C'); 
	}	

	m_bIsShuttingDownArmed = FALSE;	
	
	FireOnReport(CComBSTR("CVWServer::DisconnectCancel_T2: Succeeded in cancelling pending global disconnect.\n"), 0);
	return S_OK;
}

HRESULT CVWServer::Disconnect_T2()
{
	TRACE("CVWServer::ShutDown_T2: Initiate final stage disconnecting all clients.\n");

	//Iterate through the connlist, 
	//send them a notification message that server's dying *now*. 

	POSITION pos;
	
	for (pos = m_clist.GetStartPosition(); pos != NULL;)
	{
		IVWCommConnection *pconn = NULL;
		ConnectionInfo cinfo;
	
		m_clist.GetNextAssoc(pos, pconn, cinfo);
		
		IWorld *pWorld = cinfo.pWorld;

		SendAdminMessageToClient(pconn,'D'); //Warn'em.
	}	

	m_bIsShuttingDownArmed = FALSE;

	return S_OK;;
}

HRESULT CVWServer::ShutDown_T2()
{
	TRACE("CVWServer::ShutDown_T2: Initiate final stage disconnecting all clients.\n");
	
	//unload each world.
	
	POSITION pos;
	HRESULT hr = S_OK;
	
	for (pos = m_clist.GetStartPosition(); pos != NULL;)
	{
		IVWCommConnection *pconn = NULL;
		ConnectionInfo cinfo;
	
		m_clist.GetNextAssoc(pos, pconn, cinfo);
		
		ProcessLogoffRequest(pconn); //for that connection.
	}	
	
	m_clist.RemoveAll();
	m_wlist.RemoveAll();

	// Now disconnect all the connections (DisconnectAll releases all conn's).
	// Disconnect from client UI.
	if (m_pcomm != NULL)
	{
		m_pcomm->DisconnectAll();		

		//clear all requests.
		m_queue.Terminate();
		
		m_pcomm->Release();
		m_pcomm = NULL;
	}
	
	SAFERELEASE(m_pTools);

	SAFERELEASE(m_pDServ);

	// Release authentication package
	if (m_pAuthenticate)
	{
		m_pAuthenticate->Release();
		m_pAuthenticate = NULL;
	}
	
	m_bIsShuttingDownArmed = FALSE;	
	
	TRACE("CVWServer::ShutDown_T2: Finished disconnecting all clients.\n");

	return S_OK;
}

void CheckPaths()
{
	HRESULT	hr;
	BSTR bstrPath = NULL;
	HANDLE h;
	WIN32_FIND_DATA FindFileData;	
	CString temp;

	hr = FindWorldPath(&bstrPath);	
	if(FAILED(hr) || bstrPath == NULL || CComBSTR(bstrPath)==CComBSTR(""))
	{
		hr = E_FAIL;
		AfxMessageBox("The world path was not found in the registry.");
		goto ERROR_ENCOUNTERED;
	}

	temp = CString(bstrPath).Left(CString(bstrPath).GetLength()-1);

	h = FindFirstFile(temp, &FindFileData);
	if(INVALID_HANDLE_VALUE == h)
	{
		hr = E_FAIL;
		AfxMessageBox("The world directory path was not found on disk. There may have been an install problem.");
		goto ERROR_ENCOUNTERED;
	}
	SAFEFREESTRING(bstrPath);

/*	hr = FindAvatarGraphicsPath(&bstrPath);
	hr = FindContentPath(&bstrPath);
	hr = FindHelpPath(&bstrPath);
	hr = FindWorldWizardPath(&bstrPath);
	hr = FindWorldPath(&bstrPath);
*/
ERROR_ENCOUNTERED:
	SAFEFREESTRING(bstrPath);
}

// main communications loop.
UINT CVWServer::CommThread(void *pvVWServer)
{
	HRESULT hr = S_OK;
	
	//the this ptr not accessible from here..
	CVWServer *pServer = (CVWServer *) pvVWServer;	
	
	//The server needs to be refcounted to avoid shutdown race condition.
	pServer->AddRef();

	pServer->m_LoopState=0;

	if (FAILED(CoInitialize(NULL)))
	{
		TRACE("CVWServer::CommThread: CoInitialize in comm thread failed\n");
		pServer->m_LoopState=3;
		return 1;
	}

	pServer->m_LoopState=1;

	// Create the tools
	hr = pServer->CreateTools_T2();
	if (FAILED(hr))
	{
		TRACE("CVWServer::CommThread: CreateTools_T2 in comm thread failed\n");
		pServer->m_LoopState=3;
		return 1;
	}

	//Ask the DServ to refresh every 15 mins. 
	//this code has been stressed at 40 secs before.
	//10 mins * 60s * 1000ms/s = 600000.
	UINT ret=::SetTimer(NULL,NULL,600000,NULL);
	ASSERT(ret);
	pServer->setDSRefreshTimer(ret);
	pServer->RefreshDS();

	#ifdef DLTHREAD_DEBUG
	IInternetFileManager *pInetfile = NULL;
	hr = pServer->CreateTool_T2(CComBSTR("Inetfile"), CLSID_InternetFileManager, (IUnknown**)&pInetfile);
	if (FAILED(hr))
		ASSERT(0);
	pInetfile->Test();
	SAFERELEASE(pInetfile);
	#endif

	//Check for existance of world directory.
	CheckPaths();

	//all is well. normal loopstate.
	pServer->m_LoopState=2;

	//pServer->TestQueue();

	for(;;)
	{
		// block on an event, waiting for action
		pServer->m_queue.WaitUntilNotEmpty();
		
		// handle all m_queue messages
		for (;;)
		{
			//to deal with the worst case when the queue is rarely empty, we
			//ensure that the windows queue is flushed often.
			//pServer->TestQueue();
			Request req=pServer->m_queue.get();

			// if empty, break out and wait again
			if (req.getEvent() == EMPTY)
				break;

			long messageCount=0;
			switch (req.getEvent())
			{
				case SyncInitCommThread:
					{
						//Main thread will wait until the following line completes.
						Request returnValue;
						
						//set the return's hr.
						returnValue.setHR(S_OK);

						//sets required internal variables.
						returnValue.putSync(req);						
						
						pServer->m_queue.FireSignalToPutSync(returnValue);

						break;
					}
				case MessageArrived:
					{						
						//TRACE("CVWServer::CommThread: Received Message\n"); too freq for listing.				
						messageCount++;
						IVWCommMessage *pmess=req.getVWMsg();
						if (pmess)
						{
							pServer->ProcessMessage(pmess);
							//C.H.: PRESHIP CHANGE pmess->Release();
						}
						break;
					}
				case SocketDisconnect:
					{
						TRACE("CVWServer::CommThread: Received socket disconnect.\n");
						hr = pServer->FireOnReport(CComBSTR("CVWServer::CommThread: A socket has disconnected from the server.\n"), 0);
						//got socket disconnect - logoff the user associated with the connection.					
						IVWCommConnection *pconn = req.getConn();
						if (pconn)
							pServer->ProcessLogoffRequest(pconn);
						SAFERELEASE(pconn);
						break;
					}
				case ServerDirectedLogoff:
					{
						TRACE("CVWServer::CommThread: Received server directed logoff.\n");
						hr = pServer->FireOnReport(CComBSTR("CVWServer::CommThread: Sending warnings to all users of emminent disconnect.\n"), 0);
						//ServerDirectedNiceLogoff
						hr = pServer->DisconnectWarning_T2();
						break;
					}
				case ServerDirectedDisconnect:
					{
						TRACE("CVWServer::CommThread: Received server directed disconnect.\n");
						hr = pServer->FireOnReport(CComBSTR("CVWServer::CommThread: Server is disconnecting all users.\n"), 0);
						
						hr = pServer->Disconnect_T2();

						break;
					}
				case ServerDirectedCancel:
					{
						TRACE("CVWServer::CommThread: Received server directed cancel logoff.\n");
						
						//ServerDirectedCancel Disconnect
						hr = pServer->DisconnectCancel_T2();
						break;
					}
				case ShutDownCommThread:
					{
						//Remove worlds from DS before shutting down.
						pServer->RemoveWorldsFromDS();

						TRACE("CVWServer::CommThread: Received shutdown comm thread request.\n");
						//hr = pServer->FireOnReport(CComBSTR("CVWServer::CommThread: Shutting down now!\n"), 0);
				
						//Clean up this thread and all objects in it.
						hr = pServer->ShutDown_T2();
						
						//Main thread should wait until the following line completes.
						Request returnValue;
						
						//set the return's hr.
						returnValue.setHR(hr);

						//sets required internal variables.
						returnValue.putSync(req);						

						pServer->m_queue.FireSignalToPutSync(returnValue);

						pServer->m_queue.Terminate();

						//Quit COMMThread.
						pServer->m_LoopState=3;				

						SAFERELEASE(pServer);

						return 0;
						break;
					}
				case EventSaveWorld:
					{
						CString strWorldName;
						req.getSaveWorld(strWorldName);
						
						hr = pServer->SaveWorldHelper(strWorldName);

						//Main thread will wait until the following stuff completes.
						Request returnValue;
						
						//set the return's hr.
						returnValue.setHR(hr);

						returnValue.setHRText(pServer->ReportErrorSimple(hr));

						//sets required internal variables.
						returnValue.putSync(req);						
						
						pServer->m_queue.FireSignalToPutSync(returnValue);

						break;
					}
				case EventSaveWorldAs:
					{
						CString strWorldName,strNewWorldName;
						req.getSaveWorldAs(strWorldName,strNewWorldName);
						
						hr = pServer->SaveWorldAsHelper(strWorldName,strNewWorldName);

						//Main thread will wait until the following stuff completes.
						Request returnValue;
						
						//set the return's hr.
						returnValue.setHR(hr);

						returnValue.setHRText(pServer->ReportErrorSimple(hr));

						//sets required internal variables.
						returnValue.putSync(req);						
						
						pServer->m_queue.FireSignalToPutSync(returnValue);

						break;
					}
				case EventOpenWorld:
					{
						CString strWorldName, strReturnError;
						req.getOpenWorld(strWorldName);
						
						hr = pServer->OpenWorldHelper(strWorldName, strReturnError);

						//Main thread will wait until the following stuff completes.
						Request returnValue;
						
						//set the return's hr.
						returnValue.setHR(hr);

						returnValue.setHRText(pServer->ReportErrorSimple(hr)+"\nDetails: " +strReturnError);

						//sets required internal variables.
						returnValue.putSync(req);						
						
						pServer->m_queue.FireSignalToPutSync(returnValue);

						break;
					}
				case EventDeleteWorld:
					{
						CString strWorldName;
						req.getDeleteWorld(strWorldName);
						
						hr = pServer->DeleteWorldHelper(strWorldName);

						//Main thread will wait until the following stuff completes.
						Request returnValue;
						
						//set the return's hr.
						returnValue.setHR(hr);

						returnValue.setHRText(pServer->ReportErrorSimple(hr));

						//sets required internal variables.
						returnValue.putSync(req);						
						
						pServer->m_queue.FireSignalToPutSync(returnValue);

						break;
					}
				case EventTakeWorldOffline:
					{
						CString strWorldName;
						req.getTakeWorldOffline(strWorldName);
						
						hr = pServer->TakeWorldOfflineHelper(strWorldName);

						//Main thread will wait until the following stuff completes.
						Request returnValue;
						
						//set the return's hr.
						returnValue.setHR(hr);

						returnValue.setHRText(pServer->ReportErrorSimple(hr));

						//sets required internal variables.
						returnValue.putSync(req);						
						
						pServer->m_queue.FireSignalToPutSync(returnValue);

						break;
					}
				case EventTakeWorldOnline:
					{
						CString strWorldName;
						req.getTakeWorldOnline(strWorldName);
						
						hr = pServer->TakeWorldOnlineHelper(strWorldName);

						//Main thread will wait until the following stuff completes.
						Request returnValue;
						
						//set the return's hr.
						returnValue.setHR(hr);

						returnValue.setHRText(pServer->ReportErrorSimple(hr));

						//sets required internal variables.
						returnValue.putSync(req);						
						
						pServer->m_queue.FireSignalToPutSync(returnValue);

						break;
					}
				case EventSaveWorldScript:
					{
						CString strWorldName, strScriptName;
						req.getSaveWorldScript(strWorldName,strScriptName);
						
						hr = pServer->SaveWorldScriptHelper(strWorldName,strScriptName);

						//Main thread will wait until the following stuff completes.
						Request returnValue;
						
						//set the return's hr.
						returnValue.setHR(hr);

						returnValue.setHRText(pServer->ReportErrorSimple(hr));

						//sets required internal variables.
						returnValue.putSync(req);						
						
						pServer->m_queue.FireSignalToPutSync(returnValue);

						break;
					}
				case EventPerformanceTest:
					{
						CString strWorldName;
						long iType;
						
						req.getPerformanceTest(strWorldName, iType);
						
						hr = pServer->PerformanceTestHelper1(strWorldName, iType);

						//Main thread will wait until the following stuff completes.
						Request returnValue;
						
						//set the return's hr.
						returnValue.setHR(hr);

						returnValue.setHRText(pServer->ReportErrorSimple(hr));

						//sets required internal variables.
						returnValue.putSync(req);						
						
						pServer->m_queue.FireSignalToPutSync(returnValue);

						break;
					}
				case TimerMsg:
					{											
						pServer->HandleTimerMsg_T2(req.getTimerID());
						break;
					}
				case SetDisconnectTimerReq:
					{
						TRACE("CVWServer::CommThread: Starting countdown timer for total disconnect.\n");
						UINT TimerDuration=req.getTimerDuration();
						UINT ret;
						ret=::SetTimer(NULL,NULL,TimerDuration,NULL);
						ASSERT(ret);
						pServer->m_uiDisconnectTimerID=ret;

						pServer->FireOnReport(CComBSTR("CVWServer::CommThread: Starting disconnect timer for countdown. (Timer ID: "+str((long)ret)+")."), 0);
						break;
					}
				case KillDisconnectTimerReq:
					{						
						TRACE("CVWServer::CommThread: Shutting down disconnection timer.\n");

						if(pServer->m_uiDisconnectTimerID == 0)
						{
							MessageBeep(MB_ICONASTERISK);
							pServer->FireOnReport(CComBSTR("CVWServer::CommThread: Kill disconnect timer was called with invalid timer--ignoring request."), 0);
						}
						else
						{
							VERIFY(::KillTimer(NULL,pServer->m_uiDisconnectTimerID));
							pServer->FireOnReport(CComBSTR("CVWServer::CommThread: Killing disconnect timer for countdown. (Timer ID: "+str((long)pServer->m_uiDisconnectTimerID)+")."), 0);
						}
						pServer->m_uiDisconnectTimerID = 0;
						break;
					}
				case WindowsMessage:
					{
						// dispatch the message.
						::DispatchMessage(&(req.getMSG()));
						break;
					}
				case DirServMsg:
					{
					}
				default:
					{
						TRACE("CVWServer::CommThread: ERROR: Communication thread terminating\n");
						ASSERT(FALSE);
						pServer->m_LoopState=3;
						SAFERELEASE(pServer);
						return 0;
					}
			}
		}
	}
}

//Server UI features
HRESULT CVWServer::PerformanceTestHelper1(CString strWorldName, long iType)
{
	HRESULT hr = S_OK;
	CWorldInfo *pwi = NULL;
	DWORD dwCookie = 0;
	IWorld *pworld = NULL;
	CComPtr<IThing> spGlobal;
	long i;
	CComBSTR bstrWorldName(strWorldName);

	// see if the world has already been loaded
	// NOTE: pwi is not a comm object - don't release later
	pwi = m_wlist.FindWorld(strWorldName);
	if (pwi == NULL)
	{
		// world not loaded, attempt to load
		hr = OpenWorld(bstrWorldName, bstrWorldName, VARIANT_FALSE, VARIANT_FALSE, &pworld, &dwCookie);
		if(FAILED(hr))
		{
			TRACE("CVWServer::SaveWorldScriptHelper.  Could not load world\n");
			goto ERROR_ENCOUNTERED;
		}		
	}
	else 
	{
		pworld = pwi->World();
		SAFEADDREF(pworld);
	}

	//do tests here!

	hr = pworld->get_Global(&spGlobal);
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = spGlobal->AddPropertyExt(CComBSTR("ServerTestProp"), 
											CComVariant(CComBSTR("")),
											PSBIT_COPYONGET | PSBIT_HIDDEN,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_BSTR, IID_NULL, NULL);

	for (i = 1; i<500;i++)
	{
		hr = spGlobal->put_Property(CComBSTR("ServerTestProp"), CComVariant(CComBSTR("test")));
		if(FAILED(hr))
			goto ERROR_ENCOUNTERED;
	
		CComVariant var;
		hr = spGlobal->get_Property(CComBSTR("ServerTestProp"), &var);
		if(FAILED(hr))
			goto ERROR_ENCOUNTERED;
	
	}

ERROR_ENCOUNTERED:
	if (pworld) 
	{
		if (dwCookie)
			DeleteWorld(pworld, dwCookie);
		SAFERELEASE(pworld);
	}

	return hr;
}

HRESULT CVWServer::SaveWorldScriptHelper(CString strWorldName, CString strScriptName)
{
	HRESULT hr = S_OK;
	CWorldInfo *pwi = NULL;
	DWORD dwCookie = 0;
	IWorld *pworld = NULL;
	CComBSTR bstrWorldName(strWorldName);
	CComBSTR bstrScriptName(strScriptName);

	// see if the world has already been loaded
	// NOTE: pwi is not a comm object - don't release later
	pwi = m_wlist.FindWorld(strWorldName);
	if (pwi == NULL)
	{
		// world not loaded, attempt to load
		hr = OpenWorld(bstrWorldName, bstrWorldName, VARIANT_FALSE, VARIANT_FALSE, &pworld, &dwCookie);
		if(FAILED(hr))
		{
			TRACE("CVWServer::SaveWorldScriptHelper: Could not load world\n");
			goto ERROR_ENCOUNTERED;
		}		
	}
	else 
	{
		pworld = pwi->World();
		SAFEADDREF(pworld);
	}

	if (pworld == NULL)
	{
		TRACE("CVWServer::SaveWorldScriptHelper: Invalid world\n");
		hr = VWOBJECT_E_INVALIDWORLD;
		goto ERROR_ENCOUNTERED;
	}

	hr = pworld->SaveToScript(bstrScriptName);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	if (pworld) 
	{
		if (dwCookie)
			DeleteWorld(pworld, dwCookie);
		SAFERELEASE(pworld);
	}

	return hr;
}

HRESULT CVWServer::SaveWorldHelper(CString strWorldName)
{
	HRESULT hr = S_OK;
	CWorldInfo *pwi = NULL;
	DWORD dwCookie = 0;
	IWorld *pworld = NULL;
	CComBSTR bstrWorldName(strWorldName);
	
	// see if the world has already been loaded
	// NOTE: pwi is not a comm object - don't release later
	pwi = m_wlist.FindWorld(strWorldName);
	if (pwi == NULL)
	{
		// world not loaded, attempt to load
		hr = OpenWorld(bstrWorldName, bstrWorldName, VARIANT_FALSE, VARIANT_FALSE, &pworld, &dwCookie);
		if(FAILED(hr))
		{
			TRACE("CVWServer::SaveWorldHelper: Could not load world\n");
			goto ERROR_ENCOUNTERED;
		}		
	}
	else 
	{
		pworld = pwi->World();
		SAFEADDREF(pworld);
	}

	if (pworld == NULL)
	{
		TRACE("CVWServer::SaveWorldHelper: Invalid world\n");
		hr = VWOBJECT_E_INVALIDWORLD;
		goto ERROR_ENCOUNTERED;
	}

	hr = pworld->SaveDatabase();

ERROR_ENCOUNTERED:
	if (pworld) 
	{
		if (dwCookie)
			DeleteWorld(pworld, dwCookie);
		SAFERELEASE(pworld);
	}	

	return hr;
}

HRESULT CVWServer::SaveWorldAsHelper(CString strWorldName, CString strNewWorldName)
{
	HRESULT hr = S_OK;
	CWorldInfo *pwi = NULL;
	DWORD dwCookie = 0;
	IWorld *pworld = NULL;

	strNewWorldName = strNewWorldName.Left(strNewWorldName.GetLength() - 4);

	CComBSTR bstrNewWorldName(strNewWorldName);
	CComBSTR bstrWorldName(strWorldName);

	// see if the world has already been loaded
	// NOTE: pwi is not a comm object - don't release later
	pwi = m_wlist.FindWorld(strWorldName);
	if (pwi == NULL)
	{
		// world not loaded, attempt to load
		hr = OpenWorld(bstrWorldName, bstrWorldName, VARIANT_FALSE, VARIANT_FALSE, &pworld, &dwCookie);
		if (FAILED(hr))
		{
			TRACE("CVWServer::SaveWorldAsHelper: Could not load world\n");
			goto ERROR_ENCOUNTERED;
		}		
	}
	else 
	{
		pworld = pwi->World();
		SAFEADDREF(pworld);
	}

	if (pworld == NULL)
	{
		TRACE("CVWServer::SaveWorldAsHelper: Invalid world\n");
		hr = VWOBJECT_E_INVALIDWORLD;
		goto ERROR_ENCOUNTERED;
	}

	hr = pworld->SaveDatabaseAs(CComBSTR(strNewWorldName));

ERROR_ENCOUNTERED:
	if (pworld) 
	{
		if (dwCookie)
			DeleteWorld(pworld, dwCookie);
		SAFERELEASE(pworld);
	}	

	return hr;
}

HRESULT CVWServer::OpenWorldHelper(CString strWorldName, CString &strReturnError)
{
	HRESULT hr = S_OK;
	CWorldInfo *pwi = NULL;
	DWORD dwCookie = 0;
	IWorld *pworld = NULL;
	CComBSTR bstrGUIDOut, bstrGlobalGUIDOut;
	IThing *pGlobal = NULL;
	CComBSTR bstrPath, bstrName;
	CComBSTR bstrWorldName(strWorldName);
	static CComBSTR bstrGUIDStatic("GUID");

	//see if world's name is already in the registry.
	hr = FindWorldByPath(bstrWorldName, &bstrGUIDOut.m_str);
	if (bstrGUIDOut.m_str)
	{
		hr = VWSERVER_E_WORLDPREVIOUSLYREGISTERED;
		strReturnError += ("\nThere exists a previously registered world of the same name: \""+strWorldName+"\". The two cannot be loaded simultaneously.");
		goto ERROR_ENCOUNTERED;
	}

	// see if the world has already been loaded
	// NOTE: pwi is not a comm object - don't release later
	pwi = m_wlist.FindWorld(strWorldName);
	if (pwi == NULL)
	{
		// world not loaded, attempt to load
		hr = OpenWorld(bstrWorldName, bstrWorldName, VARIANT_FALSE, VARIANT_FALSE, &pworld, &dwCookie);
		if (FAILED(hr))
		{
			TRACE("CVWServer::OpenWorldHelper: Could not load world\n");
			goto ERROR_ENCOUNTERED;
		}		
	}
	else 
	{
		hr = VWSERVER_E_WORLDPREVIOUSLYREGISTERED;
		goto ERROR_ENCOUNTERED;
	}

	if (pworld == NULL)
	{
		TRACE("CVWServer::OpenWorldHelper: Invalid world\n");
		hr = VWOBJECT_E_INVALIDWORLD;
		goto ERROR_ENCOUNTERED;
	}

	hr = pworld->get_Global(&pGlobal);
	if (FAILED(hr) || pGlobal == NULL)
		goto ERROR_ENCOUNTERED;

	hr = pGlobal->get_String(bstrGUIDStatic, &bstrGlobalGUIDOut.m_str);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	//now make sure GUID's not in registry.
	hr = FindWorldInRegistry(bstrGlobalGUIDOut, &bstrPath.m_str, &bstrName.m_str);
	if (bstrPath.m_str != NULL)
	{
		hr = VWSERVER_E_WORLDPREVIOUSLYREGISTERED;
		strReturnError += ("\nA previously registered world \""+CString(bstrPath)+"\" has the same GUID as the world you are trying to load: \""+strWorldName+"\". The two cannot be loaded simultaneously.");
		goto ERROR_ENCOUNTERED;
	}

	//Register world.
	hr = AddWorldToRegistry(bstrGlobalGUIDOut, bstrWorldName, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = RegisterWorldInDS(bstrWorldName, CComBSTR(""), FALSE);		
	if (FAILED(hr))
	{
		ASSERT(0);
		goto ERROR_ENCOUNTERED;
	}

	hr = FireOnAvailableWorld(bstrWorldName, 0);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	SAFERELEASE(pGlobal);

	if (pworld) 
	{
		if (dwCookie)
			DeleteWorld(pworld, dwCookie);
		SAFERELEASE(pworld);
	}	

	return hr;
}

HRESULT CVWServer::DeleteWorldHelper(CString strWorldName)
{
	HRESULT hr = S_OK;
	CComBSTR bstrFinalPath;
	int retValue;
	CString strfinalFinalPath;
	CComBSTR bstrWorldName(strWorldName);
	CComBSTR bstrGUID;
	CComPtr<IThing> spGlobal;;
	static CComBSTR bstrGUIDStatic("GUID");
	
	hr = TakeWorldOfflineHelper(strWorldName);
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = CanonDatabaseName(bstrWorldName, &bstrFinalPath.m_str);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	//check for existence and read/write permission
	strfinalFinalPath = CString(bstrFinalPath)+".vwc";

	if ((retValue=_access(strfinalFinalPath, 06)) != -1) 
	{
		if (!DeleteFile(strfinalFinalPath))
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
			goto ERROR_ENCOUNTERED;
		}
	}
	else
	{		
		hr = HRESULT_FROM_WIN32(GetLastError());
			goto ERROR_ENCOUNTERED;
	}	
	
	//delete world from ds.
	{
		hr = FindWorldByPath(bstrWorldName, &bstrGUID.m_str);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = m_pDServ->RemoveWorld(bstrGUID);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	} 

	//remove from registry.
	hr = FindAndPurgeWorldsByPath(bstrWorldName);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = FireOnReport(bstrWorldName, C_CLOSEDOCUMENT);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
						
ERROR_ENCOUNTERED:
	return hr;
}

HRESULT CVWServer::TakeWorldOfflineHelper(CString strWorldName)
{
	HRESULT hr = S_OK;
	CWorldInfo *pwi = NULL;
	IWorld *pworld = NULL;
	POSITION pos;

	//Register world as offline.
	RegisterWorldOnline(CComBSTR(strWorldName), FALSE);

	//Disconnect all users from world.
	//pwi is not addref'ed.
	pwi = m_wlist.FindWorld(strWorldName);
	if (pwi == NULL)
	{
	}
	else 
	{
		pworld = pwi->World();
		SAFEADDREF(pworld);

		//Shutdown world.
		for (pos = m_clist.GetStartPosition(); pos != NULL;)
		{
			IVWCommConnection *pconn = NULL;
			ConnectionInfo cinfo;
		
			m_clist.GetNextAssoc(pos, pconn, cinfo);
			
			IWorld *pcurrWorld = cinfo.pWorld;
			
			//pcurrWorld is not addref'ed.
			if (pcurrWorld == pworld) 
			{
				SendAdminMessageToClient(pconn,'D'); //Warn'em.
				ProcessLogoffRequest(pconn); //for that connection.
			}
		}	
	}

//ERROR_ENCOUNTERED:
	SAFERELEASE(pworld);

	return hr;
}

HRESULT CVWServer::TakeWorldOnlineHelper(CString strWorldName)
{
	HRESULT hr = S_OK;
	
	hr = RegisterWorldOnline(CComBSTR(strWorldName), TRUE);

	return hr;
}

///////////////////////////////////////////////////////////////////////////////////
// utility functions. Helper fcns. (Still in commthread)
void CVWServer::ProcessLogoffRequest(IVWCommConnection *pconn)
{
	static CComBSTR bstrGUIDStatic("GUID");

#ifdef VWSERVER_LOGOFF_DEBUG
	{
		CString	str;

		str.Format("VWServer::ProcessLogoffRequest: connection (0x%x), starting at time %d\n", (DWORD)pconn, ::GetTickCount());
		OutputDebugString(str);
	}
#endif

	// STEP 1: Get the info about the world assoc'ed with the connection.
	IWorld * pworld = NULL;
	if (pworld = m_clist.FindConnection(pconn))
	{
		CWorldInfo *pinfo = m_wlist.FindWorld(pworld);
		CComBSTR bstrURL("(unknown)");

		ASSERT(pinfo);

		IThing* pGlobal = NULL;

#ifdef VWSERVER_LOGOFF_DEBUG
		{
			CString	str;

			str.Format("VWServer::ProcessLogoffRequest: found connection (0x%x) in m_clist\n", (DWORD)(void *)pconn);
			OutputDebugString(str);

		}
#endif

		pworld->get_Global(&pGlobal);
		if (pGlobal)
		{
			HRESULT hr = S_OK;
			CComBSTR bstrGUID;
			CComBSTR bstrPath, bstrName;

			// K.M.: changed to lookup WorldName rather than WorldURL
			pGlobal->get_String(CComBSTR("WorldName"), &bstrURL.m_str);

#ifdef VWSERVER_LOGOFF_DEBUG
			CString str;

			str.Format("VWServer::ProcessLogoffRequest: logging connection (0x%x) off world: %s\n", (DWORD)(void *)pconn, CString(bstrURL));
			OutputDebugString(str);
#endif

			//register world globally with new user count.
			hr = pGlobal->get_String(bstrGUIDStatic, &bstrGUID.m_str);
			if (SUCCEEDED(hr))
			{
				hr = FindWorldInRegistry(bstrGUID, &bstrPath.m_str, &bstrName.m_str);
				if (SUCCEEDED(hr))
				{
					if(bstrPath != CComBSTR(""))
					{
						hr = UpdateDS(bstrGUID, bstrPath, pinfo->UserCount()-1);
					}
				}
			}

			// REVIEW: need to get strURL from m_wlist for pworld
			FireOnLeaveWorld(pconn, CString(bstrURL));

			SAFERELEASE(pGlobal);
		}
		else
			ASSERT(0);

#ifdef VWSERVER_LOGOFF_DEBUG
		{
			CString	str;

			str.Format("VWServer::ProcessLogoffRequest: telling world to disconnect connection (0x%x)\n", (DWORD)(void *)pconn);
			OutputDebugString(str);
		}
#endif

		// STEP 2: Ask vwObject to logoff the avatar.
		pworld->DisconnectConnection(pconn);		
		
		// STEP 3: Release one reference to the world.
		if (pinfo->RemoveUser() <= 0)
		{
			//If there are no more users of the world, unload it.
			m_wlist.RemoveWorld(pworld);

#ifdef VWSERVER_LOGOFF_DEBUG
			{
				CString	str;

				str.Format("VWServer::ProcessLogoffRequest: unloading world\n");
				OutputDebugString(str);
			}
#endif

			// REVIEW: need to get strURL from m_wlist for pworld
			FireOnWorldUnloaded(CString(bstrURL)); // strURL);
		}

#ifdef VWSERVER_LOGOFF_DEBUG
		{
			CString	str;

			str.Format("VWServer::ProcessLogoffRequest: telling connection (0x%x) to disconnect\n",
				(DWORD)(void *)pconn);
			OutputDebugString(str);
		}
#endif

		// Tell connection to hang up
		pconn->Disconnect();
		
#ifdef VWSERVER_LOGOFF_DEBUG
		{
			CString	str;

			str.Format("VWServer::ProcessLogoffRequest: removing connection (0x%x) from m_clist\n", (DWORD)(void *)pconn);
			OutputDebugString(str);
		}
#endif

		VERIFY(m_clist.RemoveConnection(pconn));
	}
	else
	{
#ifdef VWSERVER_LOGOFF_DEBUG
		{
			CString	str;

			str.Format("VWServer::ProcessLogoffRequest: failed to find connection (0x%x) in m_clist\n", (DWORD)(void *)pconn);
			OutputDebugString(str);
		}
#endif

		// send is disconnect everytime ondisconnect_t2 happens.
		pconn->Disconnect();
		FireOnDisconnect(pconn);
	}


#ifdef VWSERVER_LOGOFF_DEBUG
	{
		CString	str;

		str.Format("VWServer::ProcessLogoffRequest: connection (0x%x), finished at time %d\n", (DWORD)(void *)pconn, ::GetTickCount());
		OutputDebugString(str);
	}
#endif
}

HRESULT CVWServer::SendReconnectMessageToClient(IVWCommConnection *pconn, HRESULT hrRet, BSTR bstrURL, BSTR bstrName, BSTR bstrPassword)
{	
	CString strMessage;
	CComMarshallBuffer* pbuffer = NULL;
	HRESULT hr = S_OK;
	
	// compose the message
	
	// 'A' is for "Administrative" messages
	strMessage = 'A';		
	
	// 'R' is for 'R'econnecting to a new World
	strMessage += 'R';		

	DWORD cbMessage = strMessage.GetLength()+1;	
	
	// create marshall buffer
	pbuffer = new CComMarshallBuffer();
	if (pbuffer == NULL)
	{
		TRACE("CVWServer::SendReconnectMessageToClient: failed to allocate CComMarshallBuffer\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	pbuffer->AddRef();

	// write out server version to send to client
	hr = pbuffer->put_ULong(PROTOCOL_VERSION);
	if (FAILED(hr))
	{
		TRACE("CVWServer::SendReconnectMessageToClient: failed to write to CComMarshallBuffer\n");
		goto ERROR_ENCOUNTERED;
	}

	hr = pbuffer->put_Bytes((BYTE *) (LPCSTR) strMessage, cbMessage);
	if (FAILED(hr))
	{
		TRACE("CVWServer::SendReconnectMessageToClient: failed to write to CComMarshallBuffer\n");
		goto ERROR_ENCOUNTERED;
	}

	//We put in the HRESULT as to whether the ping to new server succeeded or not.
	hr = pbuffer->put_Bytes((BYTE *) &hrRet, sizeof(HRESULT));
	if (FAILED(hr))
	{
		TRACE("CVWServer::SendReconnectMessageToClient: failed to write to CComMarshallBuffer\n");
		goto ERROR_ENCOUNTERED;
	}

	//We put the password/login info into buffer.

	hr = pbuffer->put_String(bstrURL, VARIANT_FALSE);
	if (FAILED(hr))
	{
		goto ERROR_ENCOUNTERED;
	}

	hr = pbuffer->put_String(bstrName, VARIANT_FALSE);
	if (FAILED(hr))
	{
		goto ERROR_ENCOUNTERED;
	}

	hr = pbuffer->put_String(bstrPassword, VARIANT_FALSE);
	if (FAILED(hr))
	{
		goto ERROR_ENCOUNTERED;
	}

	// send it
	hr = pconn->SendAsync(pbuffer);
	if (FAILED(hr))
	{
		goto ERROR_ENCOUNTERED;
	}
		
ERROR_ENCOUNTERED:

	SAFERELEASE(pbuffer);
	
	return hr;
}

void CVWServer::ProcessReconnectRequest(IUnMarshallBuffer *pcb, IVWCommConnection *pconn)
{
	HRESULT hr = S_OK;
	ASSERT(pcb);
	
	CComBSTR bstrURL, bstrName, bstrPassword;

	hr = pcb->get_String(&bstrURL.m_str);
	if (FAILED(hr))
	{
		TRACE("CVWServer::ProcessReconnectRequest: FAILED.\n");
		goto ERROR_ENCOUNTERED;
	}

	hr = pcb->get_String(&bstrName.m_str);
	if (FAILED(hr))
	{
		TRACE("CVWServer::ProcessReconnectRequest: FAILED.\n");
		goto ERROR_ENCOUNTERED;
	}

	hr = pcb->get_String(&bstrPassword.m_str);
	if (FAILED(hr))
	{
		TRACE("CVWServer::ProcessReconnectRequest: FAILED.\n");
		goto ERROR_ENCOUNTERED;
	}

	// TODO: ping target server and do object migration.
	
ERROR_ENCOUNTERED:
	//if succeeded, tell client to go ahead and reconnect,
	//otherwise, tell client that object migration failed and
	//just stay. but send no matter what.
	SendReconnectMessageToClient(pconn,hr,bstrURL,bstrName,bstrPassword);
}

// only side effect: sets m_pTools.
HRESULT CVWServer::OpenWorld(BSTR bstrURL, BSTR bstrFileName, VARIANT_BOOL bCreate, VARIANT_BOOL bOverwrite, IWorld **ppWorld, DWORD *pdwCookie, VARIANT_BOOL bCheckOffline)
{
	HRESULT hr = S_OK;
	IThing *pGlobal = NULL;
	IConnectionPoint* pconnpt = NULL;
	IConnectionPointContainer* pconnptctr = NULL;
	CComBSTR bstrGUID;
	BOOL bIsOnline;
	static CComBSTR bstrGUIDStatic("GUID");
	BSTR bstrServerName = NULL;
	CComBSTR bstrOldWorldName;

	if (ppWorld == NULL || pdwCookie == NULL)
		return E_POINTER;

	// initialize
	*ppWorld = NULL;
	*pdwCookie = 0;

	if (bCheckOffline == VARIANT_TRUE)
	{
		//see if offline.
		hr = IsWorldRegisteredOnline(bstrFileName, &bIsOnline);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		if (!bIsOnline)
		{
			hr = VWSERVER_E_WORLDISOFFLINE;
			goto ERROR_ENCOUNTERED;
		}
	}

	TRACE("CVWServer::OpenWorld: creating world object (%s)\n", CString(bstrURL));

	// load world
	hr = CoCreateInstance(CLSID_World, NULL, CLSCTX_INPROC_SERVER, IID_IWorld, (void **)ppWorld);
	if (FAILED(hr))
	{
		// return error to client
		TRACE("CVWServer::OpenWorld: Failed to CoCreateInstance an IWorld\n");
		goto ERROR_ENCOUNTERED;
	}

	// connect to IVWObjectSite
	hr = (*ppWorld)->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pconnptctr);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pconnptctr->FindConnectionPoint(IID_IVWObjectSite, &pconnpt);
	if (SUCCEEDED(hr))
	{
		pconnpt->Advise(GetUnknown(), pdwCookie);
		SAFERELEASE(pconnpt);
		// HACK: artificially release ptr
		GetUnknown()->Release();
	}
	else
		goto ERROR_ENCOUNTERED;

	// must come before put_World
	(*ppWorld)->put_ServerSide(VARIANT_TRUE);

	// IObjectProperty requirement
	(*ppWorld)->put_World(*ppWorld);

	// if creating world, put it in local mode while we initialize it
	if (bCreate == VARIANT_TRUE)
	{
		hr = (*ppWorld)->put_LocalMode(VARIANT_TRUE);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	(*ppWorld)->put_TraceLevel(CComBSTR("*"), VWT_ERROR); // VWT_IMPORTANT);

	// disable tracing for speed
	(*ppWorld)->EnableTrace(VARIANT_FALSE);
	
	// Add the cachemanager to the world before it connects.
	(*ppWorld)->InitializeTools(m_pTools);

	// Load (or create) checkpoint
	hr = (*ppWorld)->OpenDatabase(bstrFileName, bCreate, bOverwrite);
	if (FAILED(hr))			
		goto ERROR_ENCOUNTERED;			

	// Poke URL into global property list.
	hr = (*ppWorld)->get_Global(&pGlobal);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;


	// Always make sure the worldname is the filename.
	hr = pGlobal->get_String(CComBSTR("WorldName"), &bstrOldWorldName.m_str);
	if (FAILED(hr))
	{
		//should already be there. (addproperty should not be necessary).
		ASSERT(0);
		goto ERROR_ENCOUNTERED;
	}

	if(0!=CString(bstrOldWorldName).CompareNoCase(CString(bstrFileName)))
	{
		CSetUID SetUID((*ppWorld), pGlobal, pGlobal, VARIANT_TRUE);

		TRACE("Server::OpenWorld: Resetting worldname to: %s\n", CString(bstrFileName));
		hr = pGlobal->put_String(CComBSTR("WorldName"), bstrFileName);
		if (FAILED(hr))
		{
			//should already be there. (addproperty should not be necessary).
			ASSERT(0);
			goto ERROR_ENCOUNTERED;
		}
	}

	//This is called from ProcessCreateWorld.	
	if (bCreate == VARIANT_TRUE)
	{
		// only reset these values in create case
		
		//opened a new chckpt file.
		//register world locally

		hr = pGlobal->get_String(bstrGUIDStatic, &bstrGUID.m_str);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = AddWorldToRegistry(bstrGUID, bstrFileName, CComBSTR("Friendly name here."));
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;	

		hr = RegisterWorldInDS(bstrFileName, CComBSTR(""), FALSE);		
		if (FAILED(hr))
		{
			ASSERT(0);
			goto ERROR_ENCOUNTERED;
		}

		//don't send to DS.
		if(0)
		{
			//Put result in DS.			
			hr = UpdateDS(bstrGUID, bstrFileName, 0);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;	
		}
	} 
	// Need to put the ServerName in non-create case 
	// (ServerName is null in create case)
	else
	{
		// First, parse out servername
		hr = ParseURLHelper(bstrURL, NULL, NULL, &bstrServerName, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		CSetUID SetUID((*ppWorld), pGlobal, pGlobal, VARIANT_TRUE);

		hr = pGlobal->put_String(CComBSTR("ServerName"), CComBSTR(bstrServerName));
		if (FAILED(hr))
		{
			//should already be there. (addproperty should not be necessary).
			ASSERT(0); 
			goto ERROR_ENCOUNTERED;
		}
	}


ERROR_ENCOUNTERED:
	// if creating world, get out of local mode
	if (*ppWorld != NULL && bCreate == VARIANT_TRUE)
		(*ppWorld)->put_LocalMode(VARIANT_FALSE);

	SAFERELEASE(pconnptctr);
	SAFERELEASE(pGlobal);
	SAFEFREESTRING(bstrServerName);
	// CAREFUL WITH THIS BLOCK OF CODE!
	if (FAILED(hr))
	{
		// unadvise the world without decrementing the reference count of the client.
		if (*ppWorld)
		{						
			if (*pdwCookie)
				DeleteWorld(*ppWorld, *pdwCookie);
			SAFERELEASE(*ppWorld);
		}
	}

	return hr;
}

HRESULT CVWServer::GetURLFromMess(IUnMarshallBuffer *pcb, BSTR *pbstrFileName, BSTR *pbstrURL)
{
	HRESULT hr = S_OK;

	if (pbstrFileName==NULL || pbstrURL==NULL)
		return E_POINTER;

	// initialize
	*pbstrFileName = NULL;
	*pbstrURL = NULL;
	
	// let's crack the message
	// to see what URL user wants to connect to

	// read the message out

	hr = pcb->get_String(pbstrURL);
	if (FAILED(hr))
		return hr;

	// returns filename; ignore server and port
	hr = ParseURLHelper(*pbstrURL, pbstrFileName, NULL, NULL, NULL);
	if (FAILED(hr))  
		return hr;

	return S_OK;
}

void CVWServer::ProcessMessage(IVWCommMessage *pmess)
{
	HRESULT hr = S_OK;
	IVWCommConnection *pconn = NULL;
	int iType;

	// see what kind of message we have
	hr = pmess->GetStatus(&iType);
	if (FAILED(hr))
	{
		TRACE("CVWServer::ProcessMessage: Couldn't get message type\n");
		goto ERROR_ENCOUNTERED;
	}

	// get the connection out of the message. 
	hr = pmess->GetConnection(&pconn);
	if (FAILED(hr))
	{
		TRACE("CVWServer::ProcessMessage: Couldn't get connection out of message\n");
		goto ERROR_ENCOUNTERED;
	}

	m_pCurrentConnection = pconn;

	// handle different types of messages
	switch(iType)
	{
		case mscOk:
			// plain old message
			ProcessDataMessage(pmess, pconn);
			break;

		case mscReadError:
		case mscWriteError:
		case mscAcceptError:
			// REVIEW: display some type of message
			ProcessLogoffRequest(pconn);
			break;

		case mscEvilUser:
			// REVIEW: display some type of message
			ProcessLogoffRequest(pconn);
			break;

		case mscConnect:
			FireOnConnect(pconn);
			break;

		case mscDisconnect:
		case mscDirtyDisconnect:
			ProcessLogoffRequest(pconn);
			break;

		case mscOutOfMemory:
			ASSERT(FALSE);
			break;

		default:
			ASSERT(FALSE);
			break;
	}

ERROR_ENCOUNTERED:

	m_pCurrentConnection = NULL;
	SAFERELEASE(pconn);
}

void CVWServer::ProcessDataMessage(IVWCommMessage *pmess, IVWCommConnection *pconn)
{
	HRESULT hr = S_OK;
	IUnMarshallBuffer *pcb = NULL;
	BYTE chType;
	DWORD cb = 0;
	BOOL bReply = TRUE;
	unsigned long ulVersion = 0L;
	
	if (pmess==NULL) 
		return;

	// don't free this guy! (weak reference)
	IWorld *pworld = NULL;

	// get the message buffer
	hr = pmess->GetData(&pcb);
	if (FAILED(hr))
	{
		TRACE("CVWServer::ProcessDataMessage: Couldn't get buffer out of message\n");
		goto ERROR_ENCOUNTERED;
	}

	// check length
	if (FAILED(pcb->get_Length(&cb)))
	{
		TRACE("CVWServer::ProcessDataMessage: Bad message\n");
		goto ERROR_ENCOUNTERED;
	}

	if (cb<3)
	{
		TRACE("CVWServer::ProcessDataMessage: Too short message\n");
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	// get the connection out of the message. 
	hr = pmess->GetConnection(&pconn);
	if (FAILED(hr))
	{
		TRACE("CVWServer::ProcessDataMessage: Couldn't get connection out of message\n");
		goto ERROR_ENCOUNTERED;
	}

	// check if we've seen this connection before. DOES NOT ADDREF.
	pworld = m_clist.FindConnection(pconn);

	hr = pcb->get_ULong(&ulVersion);
	if (FAILED(hr))
	{
		TRACE("CVWServer::ProcessDataMessage: Could not get client version from message\n");
		goto ERROR_ENCOUNTERED;
	}

	if (ulVersion != PROTOCOL_VERSION)
	{
		TRACE("CVWServer::ProcessDataMessage: Client version number not same as server version number\n");
		hr = VWOBJECT_E_INVALIDCLIENTVERSION;
		goto ERROR_ENCOUNTERED;
	}

	// see if it's an admin or world message
	pcb->get_Byte(&chType);

	// Let handlers do the replies, as needed
	// NOTE: We have to do this since we sometimes need to reply before
	// the handlers finish their work
	bReply = FALSE;

	if (chType=='A')
		hr = ProcessAdminMessage(pworld, pconn, pmess, pcb);
	else
		hr = ProcessWorldMessage(pworld, pconn, pmess, pcb);

ERROR_ENCOUNTERED:

	if (bReply)
		ReplyWithHResult(pmess, hr);

	SAFERELEASE(pcb);
	SAFERELEASE(pconn);

}

STDMETHODIMP CVWServer::ClearPerformanceCounters() {

    ZeroMemory(&m_Enter,sizeof(PERFSTAT));
    ZeroMemory(&m_Logon,sizeof(PERFSTAT));
    ZeroMemory(&m_Create,sizeof(PERFSTAT));
    ZeroMemory(&m_Disconnect,sizeof(PERFSTAT));
    ZeroMemory(&m_Reconnect,sizeof(PERFSTAT));
    ZeroMemory(&m_World,sizeof(PERFSTAT));
    
    return S_OK;
}


HRESULT CVWServer::ProcessWorldMessage(IWorld *pworld, IVWCommConnection *pconn, IVWCommMessage *pmess, IUnMarshallBuffer *pcb)
{
	// if no world, then this is erroneous
	if (pworld==NULL)
	{
		TRACE("CVWServer::ProcessWorldMessage: World message w/o logging into world\n");
		ReplyWithHResult(pmess, E_FAIL);
		// REVIEW: Better HRESULT
		return E_FAIL;
	}

	
	// rewind to beginning of buffer
	pcb->SeekToBegin();
	
    // Start clock
    LARGE_INTEGER liPerf1, liPerf2;
    QueryPerformanceCounter(&liPerf1);

    // pass on to world
    pworld->ProcessMessage(pmess);

    // Calculate performance statistics
    QueryPerformanceCounter(&liPerf2);	
    m_World.Last = ((liPerf2.HighPart - liPerf1.HighPart) * 2^32 + (liPerf2.LowPart - liPerf1.LowPart));
    m_World.Total += m_World.Last;
    m_World.Cnt++;
    m_World.SD += (m_World.Last - (m_World.Total / m_World.Cnt)) ^ 2;

    return S_OK;
}

HRESULT CVWServer::ProcessAdminMessage(IWorld *pworld, IVWCommConnection *pconn, IVWCommMessage *pmess, IUnMarshallBuffer *pcb)
{
	BYTE chType;
	CString str;		

    //Which averaging counter we will be updating, if any
    PERFSTAT *pPerf = NULL;

	// Get message type
	pcb->get_Byte(&chType);

	// Start clock
	LARGE_INTEGER liPerf1, liPerf2;	
	QueryPerformanceCounter(&liPerf1);
	
    switch(chType)
	{
		case 'W':
		{
			TRACE("CVWServer::ProcessAdminMessage: Got 'World' admin message\n");
			ProcessEnterWorld(pworld, pconn, pmess, pcb);          
            pPerf = &m_Enter;
			break;
		}

		case 'L':
		{
			TRACE("CVWServer::ProcessAdminMessage: Got 'Logon' admin message\n");
			ProcessLogon(pworld, pconn, pmess, pcb);
            pPerf = &m_Logon;
			break;
		}

		case 'C':
		{
			TRACE("CVWServer::ProcessAdminMessage: Get 'Create World' admin message\n");
			ProcessCreateWorld(pworld, pconn, pmess, pcb);
            pPerf = &m_Create;
			break;
		}

		case 'M':
		{
			TRACE("CVWServer::ProcessAdminMessage: Got diagnostic admin message\n");
			ProcessDiagnostic(pworld, pconn, pmess, pcb);
			break;
		}
		
		case 'D':
		{
			TRACE("CVWServer::ProcessAdminMessage: Got client's disconnect admin message\n");
            ProcessDisconnect(pworld, pconn, pmess, pcb);
            pPerf = &m_Disconnect;
            break;
		}

		case 'R':
		{
			TRACE("CVWServer::ProcessAdminMessage: Got client's reconnect admin message\n");			

			// Reply immediately with okay (to prevent a delay)
			ReplyWithHResult(pmess, S_OK);

			ProcessReconnectRequest(pcb, pconn);
            pPerf = &m_Reconnect;
            break;	
		}
		
		default:
		{
			TRACE("CVWServer::ProcessAdminMessage: Invalid administrative message received from client\n");

			ReplyWithHResult(pmess, E_FAIL);
			break;
		}
	}

    QueryPerformanceCounter(&liPerf2);
    if (pPerf) {
        pPerf->Last = ((liPerf2.HighPart - liPerf1.HighPart) * 2^32 + (liPerf2.LowPart - liPerf1.LowPart));
        pPerf->Total += pPerf->Last;
        pPerf->Cnt++;
        pPerf->SD += (pPerf->Last - (pPerf->Total / pPerf->Cnt)) ^ 2;
        
    }

	return S_OK;

}

HRESULT CVWServer::ReplyWithHResult(IVWCommMessage *pmess, HRESULT replyhr)
{
	return ReplyWithHResultAndData(pmess, replyhr, NULL, 0);
}

HRESULT CVWServer::ReplyWithHResultAndData(IVWCommMessage *pmess, HRESULT replyhr, BYTE *pbData, int cbData)
{
	// assure proper message
	VARIANT_BOOL bSync;
	HRESULT hr = S_OK;
	IVWCommConnection *pconn = NULL;
	CComMarshallBuffer *pbufReply = NULL;
	
	if (!pmess) 
		return E_FAIL;

	hr = pmess->GetSynchronous(&bSync);
	if (FAILED(hr) || bSync == VARIANT_FALSE)
	{
//		TRACE("CVWServer::ReplyWithHResult: Warning: Replying to non synchronous message\n");
		goto ERROR_ENCOUNTERED;
	}

	// get a reply buffer
	hr = pmess->GetConnection(&pconn);
	if (FAILED(hr))
	{
		TRACE("CVWServer::ReplyWithHResult: Failed to retrieve connection from a message\n");
		goto ERROR_ENCOUNTERED;
	}

	// create marshall buffer
	pbufReply = new CComMarshallBuffer();
	if (pbufReply == NULL)
	{
		TRACE("CVWServer::ReplyWithHResult: failed to allocate CComMarshallBuffer\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	pbufReply->AddRef();

	hr = pbufReply->put_Long(replyhr);
	if (FAILED(hr))
	{
		TRACE("CVWServer::ReplyWithHResult: Failed to write to buffer\n");
		goto ERROR_ENCOUNTERED;
	}

	// add in any data (could be zero sized
	if (pbData!=NULL)
	{
		// write out the length
		hr = pbufReply->put_Int(cbData);
		if (FAILED(hr))
		{
			TRACE("CVWServer::ReplyWithHResult: Failed to write data length to buffer\n");
			goto ERROR_ENCOUNTERED;
		}
		if (cbData>0)
		{
			hr = pbufReply->put_Bytes(pbData, cbData);
			if (FAILED(hr))
			{
				TRACE("CVWServer::ReplyWithHResult: Failed to write data to buffer\n");
				goto ERROR_ENCOUNTERED;
			}
		}
	}


	// send it
	hr = pmess->Reply(pbufReply);
	if (FAILED(hr))
	{
		TRACE("CVWServer::ReplyWithHResult: Failed to send reply\n");
		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	// free stuff
	SAFERELEASE(pbufReply);
	SAFERELEASE(pconn);

	return hr;
}

HRESULT CVWServer::ProcessLogon(IWorld *pworld, IVWCommConnection *pconn, IVWCommMessage *pmess, IUnMarshallBuffer *pcb)
{
	HRESULT hr = S_OK;
	BYTE chAuthenticate;
	VARIANT_BOOL bAuthenticationDone;
	IAuthentication *pauth = NULL;
	int cbBlob;
	BYTE *prgchBlob = NULL;
	CComBSTR bstrGroup=NULL;

	// allow at anytime in order to validate a connection, after connecting to a world
	//
	// see if already in world
	// if (pworld!=NULL)
	// {
	//	TRACE("CVWServer::ProcessLogon: Already in a world!\n");
	//	hr = E_FAIL;
	//	goto ERROR_ENCOUNTERED;
	// }


	// see if we have a connection limit and if we are at the max yet
	if ((m_dwMaxConnections > 0) && ((DWORD)m_clist.GetCount()>=m_dwMaxConnections))
	{
		hr=VWSERVER_E_TOOMANYCONNECTIONS;
		TRACE("CVWServer::ProcessLogon  Connection denied:  %x\n",hr);
		CString cstr;
        cstr.Format("Connection Attempt Denied. Too many users on server.", hr);
        FireOnReport(CComBSTR(cstr), 0);
		goto ERROR_ENCOUNTERED;
	}

	// If not requiring authentication on this server bailout with Success with Info
	if (!m_bForceAuthentication) 
	{
		// REVIEW:  instead of FAILURE code this should be custom success code if possible
		// SIDE-EFFECT: this changes behaviour of the WorldWizard.. we now just look at the server's 
		//				authentication flag.. if set we always authenticate on any connection,
		//				if not we bailout
		hr=VWOBJECT_E_UNAUTHENTICATEDCONNECTION;
		goto NO_AUTHENTICATION;
	} 

	// see if we're already authenticated
	hr = pconn->get_IsAuthenticated(&bAuthenticationDone);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bAuthenticationDone==VARIANT_TRUE)
		goto AUTHENTICATED;

	// get the authentication flag out
	hr = pcb->get_Byte(&chAuthenticate);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if ((chAuthenticate=='N')&&(m_bForceAuthentication))
	{
		hr = VWSERVER_E_AUTHENTICATIONREQUIRED;
		
		CComBSTR bstrAddress;
		pconn->get_SocketAddress(&bstrAddress.m_str);
		TRACE("CVWServer::ProcessLogon  Connection denied:  %x\n",hr);
		CString cstr;
        cstr.Format("Connection Attempt Denied: Client Requested unauthenticated Connection");
        FireOnReport(CComBSTR(cstr), 0);

		goto ERROR_ENCOUNTERED;
	}
	else
	{
		// need to authenticate the connection  chAuthenticate=="Y"
		// load the security stuff if we already haven't
		if (m_pAuthenticate==NULL)
		{
			hr = CoCreateInstance(	CLSID_Authenticate, 
									NULL, 
									CLSCTX_INPROC_SERVER,
									IID_IVWAuthenticate,
									(void **) &m_pAuthenticate);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			// initialize it
			hr = m_pAuthenticate->Initialize();
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			// set the security package
			// REVIEW: set from registry?
			hr = m_pAuthenticate->put_Package(CComBSTR("NTLM"));
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}

		// see if the connection even has an authentication
		hr = pconn->get_Authentication(&pauth);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (pauth==NULL)
		{
			// create one
			hr = CoCreateInstance(	CLSID_Authentication,
									NULL,
									CLSCTX_INPROC_SERVER,
									IID_IAuthentication,
									(void **) &pauth);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			// initialize
			hr = pauth->put_Authenticate(m_pAuthenticate);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			// poke into the connection
			hr = pconn->put_Authentication(pauth);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
		else
		{
			if (chAuthenticate=='R') 
			{
				pauth->put_Initialized(VARIANT_FALSE);
//				pauth->put_Authenticated(VARIANT_FALSE);
			}
		}

		// read blob (if any) from incoming message and stick it in the authentication
		hr = pcb->get_Int(&cbBlob);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (cbBlob>0)
		{
			// allocate blob space
			prgchBlob = new BYTE[cbBlob];
			if (prgchBlob==NULL)
			{
				hr = E_OUTOFMEMORY;
				goto ERROR_ENCOUNTERED;
			}
			hr = pcb->get_Bytes(prgchBlob, cbBlob);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			// give it to the authentication
			hr = pauth->SetBlob(prgchBlob, cbBlob);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			// free stuff
			delete [] prgchBlob;
			prgchBlob = NULL;
		}

		// get a server context
		hr = m_pAuthenticate->GenerateServerContext(pauth, &bAuthenticationDone);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// see if all done
		if (bAuthenticationDone==VARIANT_TRUE)
		{
			// now check Group Status if it is set
			if (m_bstrAuthenticationGroup.Length()>0)
			{
				VARIANT_BOOL bMember=VARIANT_FALSE;
				hr=m_pAuthenticate->IsGroupMember(pauth,m_bstrAuthenticationGroup.Copy(),&bMember);
				if (FAILED(hr) || (bMember==VARIANT_FALSE))
				{
					hr=VWSERVER_E_GROUPAUTHENTICATIONFAILURE;

					CComBSTR bstrAddress;
					pconn->get_SocketAddress(&bstrAddress.m_str);
					TRACE("CVWServer::ProcessLogon  Connection denied:  %x\n",hr);
					CString cstr;
					cstr.Format("Connection Attempt Denied. LocalGroup authentication failure from: %s",CString(bstrAddress));
					FireOnReport(CComBSTR(cstr), 0);

					// Must reset this now because if we failed the group lookup we force a relogin
					pauth->put_Initialized(VARIANT_FALSE);
					pauth->put_Authenticated(VARIANT_FALSE);

					goto ERROR_ENCOUNTERED;
				}
					
			}
			goto AUTHENTICATED;
		}

		// get the blob back out of the authentication and marshall it back to the client
		hr = pauth->GetBlobLength(&cbBlob);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// allocate a buffer
		prgchBlob = new BYTE[cbBlob];
		if (prgchBlob==NULL)
		{
			hr = E_OUTOFMEMORY;
			goto ERROR_ENCOUNTERED;
		}

		// get the blob
		hr = pauth->CopyBlob(prgchBlob);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// set status indicating we're not done
		// REVIEW: better error code
		hr = VWOBJECT_E_INVALIDSECURITY;
	}

NO_AUTHENTICATION:
AUTHENTICATED:
ERROR_ENCOUNTERED:

	// send back data, if not complete
	if (hr==VWOBJECT_E_INVALIDSECURITY)
		ReplyWithHResultAndData(pmess, hr, prgchBlob, cbBlob);
	else
		ReplyWithHResult(pmess, hr);
	SAFERELEASE(pauth);

	if (prgchBlob!=NULL)
		delete [] prgchBlob;

	return S_OK;
}

HRESULT CVWServer::ProcessDiagnostic(IWorld *pworld, IVWCommConnection *pconn, IVWCommMessage *pmess, IUnMarshallBuffer *pcb)
{
	CString str;
	char szMsg[cchMaxMessage];

	pcb->get_Bytes((BYTE *) szMsg, cchMaxMessage);
	str = szMsg;

	ReplyWithHResult(pmess, S_OK);

	FireOnCommMessage(pconn, str);
	
	return S_OK;
}

HRESULT CVWServer::ProcessDisconnect(IWorld *pworld, IVWCommConnection *pconn, IVWCommMessage *pmess, IUnMarshallBuffer *pcb)
{
	// Reply immediately with okay before disconnecting.
	ReplyWithHResult(pmess, S_OK);

	ProcessLogoffRequest(pconn);

	return S_OK;
}

//register world globally helper.
HRESULT CVWServer::UpdateDS(BSTR bstrGUID, BSTR path, int NewUserCount)
{
	HRESULT hr;
	char ComputerName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD dwLength =  MAX_COMPUTERNAME_LENGTH + 1;
	BOOL bFoundComputerName;
	BOOL	bIsInDS = TRUE;
	CString strPath(path);

	CString strUserCount = str((long)NewUserCount);
		
	//push it to the DS.

	//REVIEW: delete this later, to just change attribute.
	hr = m_pDServ->RemoveWorld(bstrGUID);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	//tell server.
	hr = FireOnAvailableWorld(path,NewUserCount);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	bFoundComputerName = GetComputerName(ComputerName, &dwLength);
	if(bFoundComputerName)
	{
		strPath = CString(ComputerName) +"/"+ strPath;
	}
	else
	{
		ASSERT(0);
	}


	// check to see if the world in the DS
	IsWorldRegisteredInDS(path, &bIsInDS);
	
	if (bIsInDS)
	{
		CComBSTR	bstrInfoURL;

		hr = GetWorldInfoUrlFromRegistry(path, &bstrInfoURL.m_str);
		if (FAILED(hr))
			bstrInfoURL = "http://www.microsoft.com";	// TO DO: better default value
		
		#ifdef _DEBUG
			afxDump<<"Adding:"<<strPath<<", users:"<<strUserCount<<"\n";
		#endif
		hr = m_pDServ->AddWorld(bstrGUID, CComBSTR(strPath), CComBSTR(strUserCount), bstrInfoURL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;	
	}
	
ERROR_ENCOUNTERED:
	
	return hr;
} 

HRESULT CVWServer::ProcessEnterWorld(IWorld *pworld, IVWCommConnection *pconn, IVWCommMessage *pmess, IUnMarshallBuffer *pcb)
{
	HRESULT hr = S_OK;
	CComBSTR bstrFileName;
	CComBSTR bstrURL;
	CWorldInfo *pwi = NULL;
	DWORD dwCookie = 0;
	IThing *pGlobal = NULL;
	CComBSTR bstrGUID;
	CString strFileName;
	static CComBSTR bstrGUIDStatic("GUID");
	VARIANT_BOOL bAuthenticationDone;

	// see if already in world
	if (pworld != NULL)
	{
		TRACE("CVWServer::ProcessEnterWorld: Already in a world!\n");
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	// See if the _server_ requires an authenticated connection
	// and then check if it is already authenticated..
	if (m_bForceAuthentication)
	{
		hr = pconn->get_IsAuthenticated(&bAuthenticationDone);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		if (bAuthenticationDone!=VARIANT_TRUE)
		{
			hr = VWOBJECT_E_INVALIDSECURITY;
			goto ERROR_ENCOUNTERED;
		}

	}

	
	//T.O.:	Note: Normally we'd have already done this in ProcessLogon, but 
	//		we are adding this for backward compatibility with v1.1 client
	//		which don't send a AdminMessage of type Login (L)
	//
	if ((m_dwMaxConnections > 0) && ((DWORD)m_clist.GetCount()>=m_dwMaxConnections))
	{
		hr=VWSERVER_E_TOOMANYCONNECTIONS;
		TRACE("CVWServer::ProcessEnterWorld  Connection denied:  %x\n",hr);
		CString cstr;
        cstr.Format("Connection Attempt Denied. Too many users on server.", hr);
        FireOnReport(CComBSTR(cstr), 0);
		goto ERROR_ENCOUNTERED;
	}

	hr = GetURLFromMess(pcb, &bstrFileName.m_str, &bstrURL.m_str);
	if(FAILED(hr))
	{
		TRACE("CVWServer::ProcessEnterWorld: Could not get URL from message\n");
		goto ERROR_ENCOUNTERED;
	}

	if (bstrFileName.Length() == 0)
	{
		TRACE("CVWServer::ProcessEnterWorld: Blank world name\n");
		hr = VWOBJECT_E_INVALIDURLFORMAT;
		goto ERROR_ENCOUNTERED;
	}

	strFileName = bstrFileName;

	// see if the world has already been loaded
	// NOTE: pwi is not a comm object - don't release later
	pwi = m_wlist.FindWorld(strFileName);
	if (pwi == NULL)
	{
		// world not loaded, attempt to load
		hr = OpenWorld(bstrURL, bstrFileName, VARIANT_FALSE, VARIANT_FALSE, &pworld, &dwCookie);
		if(FAILED(hr))
		{
			TRACE("CVWServer::ProcessEnterWorld.  Could not load world\n");
			goto ERROR_ENCOUNTERED;
		}
			
		// add world to list. Incr's ref count.
		pwi = m_wlist.AddWorld(strFileName, pworld, dwCookie);
		if (pwi == NULL)
		{
			TRACE("CVWServer::ProcessEnterWorld.  Could not add world to world list\n");
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
			
		// send event
		FireOnWorldLoaded(strFileName);
	}
	else 
	{
		pworld = pwi->World();
		SAFEADDREF(pworld);
	}

	// REVIEW: NYI: See if the world requires an authenticated connection (look in the registry)

	// add connection, add user to world count
	m_clist.AddConnection(pconn, pworld, NULL, bstrFileName);
	
	pwi->AddUser();

	//register world globally with new user count.
	{
		hr = pworld->get_Global(&pGlobal);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGlobal->get_String(bstrGUIDStatic, &bstrGUID.m_str);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		hr = UpdateDS(bstrGUID, bstrFileName, pwi->UserCount());
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;		
	} 

	FireOnEnterWorld(pconn, strFileName);

ERROR_ENCOUNTERED:
	ReplyWithHResult(pmess, hr);
	
	SAFERELEASE(pGlobal);
	SAFERELEASE(pworld);

	return hr;
}

HRESULT CVWServer::ProcessCreateWorld(IWorld *pworld, IVWCommConnection *pconn, IVWCommMessage *pmess, IUnMarshallBuffer *pcb)
{
	HRESULT hr = S_OK;
	CComBSTR bstrFileName, bstrURL;
	CWorldInfo *pwi = NULL;
	DWORD dwCookie = 0;
	IWorld *pWorldNew = NULL;
	VARIANT_BOOL bIsAuthenticated;
	CString strFileName;

	// see if already in world
	if (pworld != NULL)
	{
		TRACE("CVWServer::ProcessCreateWorld: Already in a world!\n");
		hr = VWSERVER_E_WORLDPREVIOUSLYREGISTERED;
		goto ERROR_ENCOUNTERED;
	}

	
	if (m_bForceAuthentication)
	{
		// Assure we have an authenticated connection
		hr = pconn->get_IsAuthenticated(&bIsAuthenticated);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (bIsAuthenticated != VARIANT_TRUE)
		{
			// REVIEW: We're "borrowing" an error code.  Should have a more explicit one.
			hr = VWOBJECT_E_INVALIDSECURITY;
			goto ERROR_ENCOUNTERED;
		}
	}

	// get the world name from the message
	hr = pcb->get_String(&bstrFileName.m_str);
	if (FAILED(hr))
		return hr;

	if (bstrFileName.Length() == 0)
	{
		TRACE("CVWServer::ProcessCreateWorld: Blank world name\n");
		hr = VWOBJECT_E_INVALIDURLFORMAT;
		goto ERROR_ENCOUNTERED;
	}

	strFileName = bstrFileName;

	// see if the world has already been loaded
	// NOTE: pwi is not a comm object - don't release later
	pwi = m_wlist.FindWorld(strFileName);
	if (pwi != NULL)
	{
		TRACE("CVWServer::ProcessCreateWorld: World is already loaded\n");
		hr = VWSERVER_E_WORLDPREVIOUSLYREGISTERED;
		goto ERROR_ENCOUNTERED;
	}

	// REVIEW: NYI. See if the world is already on the disk.
	// REVIEW: Impersonate the user?

	// Create the world database
	//no overwrites allowed 
	hr = OpenWorld(NULL, bstrFileName, VARIANT_TRUE, VARIANT_FALSE, &pWorldNew, &dwCookie);
	if (FAILED(hr))
	{
		TRACE("CVWServer::ProcessCreateWorld.  Could not load world: %x\n", hr);
        CString cstr;
        cstr.Format("Error creating world.  (%x)", hr);
        FireOnReport(CComBSTR(cstr), 0);

        // fold the VWODB errors into VWSERVER errors for the client.
        switch (hr)
        {
        case VWODB_E_DBALREADYEXIST:
            hr = VWSERVER_E_WORLDPREVIOUSLYREGISTERED;
            break;
        case VWODB_E_INVALIDDBNAME:
            hr = VWSERVER_E_INVALIDWORLDNAME;
            break;
        default:
		    hr = VWSERVER_E_ERRORCREATINGWORLD;
        }

		goto ERROR_ENCOUNTERED;
	}

	// add world to list. Incr's ref count.
	pwi = m_wlist.AddWorld(strFileName, pWorldNew, dwCookie);
	if (pwi == NULL)
	{
		TRACE("CVWServer::ProcessCreateWorld.  Could not add world to world list\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}
			
	// send event
	FireOnWorldLoaded(strFileName);

	// add connection, add user to world count
	m_clist.AddConnection(pconn, pWorldNew, NULL, bstrFileName);

	pwi->AddUser();

	FireOnEnterWorld(pconn, strFileName);

ERROR_ENCOUNTERED:
	ReplyWithHResult(pmess, hr);

	SAFERELEASE(pWorldNew);

	return hr;
}

///////////////////////////////////////////////////////////////////////////////////
// IVWCommSite methods. These are actually in **Thread 3**.
HRESULT CVWServer::OnMessageAvailable(IVWComm *pcomm)
{
	// This routine should never be called on the server side (messaging is
	// done in the thread 2. 
	return E_NOTIMPL;
}

#if 0
// This is actually considered **Thread 3** on the server side.
HRESULT CVWServer::OnConnect_T2(IVWCommConnection *pICommConnection)
{
	TRACE("CVWServer::OnConnect_T2: Received.\n");

	// this is actually considered Thread 3 on the server side.
	FireOnConnect(pICommConnection);
	
	return S_OK;
}

// This is actually considered **Thread 3** on the server side.
HRESULT CVWServer::OnDisconnect_T2(IVWCommConnection *pICommConnection)
{
	TRACE("CVWServer::OnDisconnect_T2: Received.\n");
	
	//this is actually considered Thread 3 on the server side.
	Request req(SocketDisconnect);
	req.setConn(pICommConnection);
	
	SAFEADDREF(pICommConnection);
	
	m_queue.put(req);
	FireOnDisconnect(pICommConnection);
	
	return S_OK;
}

// This is actually considered **Thread 3** on the server side.
HRESULT CVWServer::OnError_T2(IVWCommConnection *pICommConnection, int nErrorCode)
{
#ifdef _DEBUG
	{
		CString	str;

		str.Format("VWServer::OnError_T2. Error %d at time %d\n", nErrorCode, ::GetTickCount());
		OutputDebugString(str);
	}
#endif

	// this is actually considered Thread 3 on the server side.
	FireOnCommError(pICommConnection);
	return S_OK;
}

#endif 

HRESULT CVWServer::OnMessageAvailable_T2(IVWComm *pcomm)
{
	// this is actually considered Thread 3 on the server side.
	
	IVWCommMessage *pmess = NULL;
	
	ASSERT(pcomm);
	if(!pcomm)
		return E_FAIL;

	while (SUCCEEDED(pcomm->GetMessage(&pmess)) && pmess!=NULL)
	{
		Request req(MessageArrived);
		req.setVWMsg(pmess);
	
		m_queue.put(req);

		SAFERELEASE(pmess);
	}
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////////
// Event Firers (helper fcns)

HRESULT CVWServer::FireOnWorldLoaded(CString& strWorld)
{
    HRESULT hr = S_OK;

	START_EVENT(CVWServer, DVWServerEvents)

            hr = pEvent->OnWorldLoaded(CComBSTR(strWorld));

	FINISH_EVENT()

	if (m_hCallbackWindowHandle)
	{
		REPORTSTRUCT *pStruct = new REPORTSTRUCT;
		
		// we need a non-destructing copybstr. 
		pStruct->bstrMessage = ::SysAllocString(CComBSTR("World has been loaded."));	
		pStruct->bstrWorldName = strWorld.AllocSysString();
		pStruct->lType =  0;
		ASSERT(strWorld!="");

		::PostMessage(m_hCallbackWindowHandle, WM_ONREPORT, (WPARAM)pStruct, (LPARAM)NULL);
	}
    return hr;    
}

HRESULT	CVWServer::FireOnWorldUnloaded(CString& strWorld)
{
	HRESULT hr = S_OK;

	START_EVENT(CVWServer, DVWServerEvents)

            hr = pEvent->OnWorldUnloaded(CComBSTR(strWorld));

	FINISH_EVENT()

	FireOnReport(CComBSTR("World has been unloaded."), 1);
    
	return hr;
}

HRESULT	CVWServer::FireOnConnect(IVWCommConnection *pconn)
{
	HRESULT hr = S_OK;

	START_EVENT(CVWServer, DVWServerEvents)

            hr = pEvent->OnConnect((DWORD) (void *) pconn);

	FINISH_EVENT()

    return hr;
}

HRESULT	CVWServer::FireOnDisconnect(IVWCommConnection *pconn)
{
	HRESULT hr = S_OK;

	START_EVENT(CVWServer, DVWServerEvents)

            hr = pEvent->OnDisconnect((DWORD) (void *) pconn);

	FINISH_EVENT()

    return hr;
}

HRESULT	CVWServer::FireOnEnterWorld(IVWCommConnection *pconn, CString& strWorld)
{
	HRESULT hr = S_OK;

	START_EVENT(CVWServer, DVWServerEvents)

            hr = pEvent->OnEnterWorld((DWORD) (void *) pconn, CComBSTR(strWorld));

	FINISH_EVENT()

	if(m_hCallbackWindowHandle)
	{
		REPORTSTRUCT *pStruct = new REPORTSTRUCT;
		
		// we need a non-destructing copybstr. 
		pStruct->bstrMessage = ::SysAllocString(CComBSTR("User has entered world."));	
		pStruct->bstrWorldName = strWorld.AllocSysString();
		pStruct->lType =  0;
		ASSERT(strWorld!="");
		
		::PostMessage(m_hCallbackWindowHandle, WM_ONREPORT, (WPARAM)pStruct, (LPARAM)NULL);
	}

    return hr;
}

HRESULT	CVWServer::FireOnLeaveWorld(IVWCommConnection *pconn, CString& strWorld)
{
	HRESULT hr = S_OK;

	START_EVENT(CVWServer, DVWServerEvents)

            hr = pEvent->OnLeaveWorld((DWORD) (void *) pconn, CComBSTR(strWorld));

	FINISH_EVENT()

	if(m_hCallbackWindowHandle)
	{
		REPORTSTRUCT *pStruct = new REPORTSTRUCT;
		
		// we need a non-destructing copybstr. 
		pStruct->bstrMessage = ::SysAllocString(CComBSTR("User has left world."));	
		pStruct->bstrWorldName = strWorld.AllocSysString();
		pStruct->lType =  0;
		ASSERT(strWorld!="");
		
		::PostMessage(m_hCallbackWindowHandle, WM_ONREPORT, (WPARAM)pStruct, (LPARAM)NULL);
	}

    return hr;
}

HRESULT	CVWServer::FireOnCommError(IVWCommConnection *pconn)
{
	HRESULT hr = S_OK;

	START_EVENT(CVWServer, DVWServerEvents)

            hr = pEvent->OnCommError((DWORD) (void *) pconn);

	FINISH_EVENT()

	FireOnReport(CComBSTR("Comm error."),1);

    return hr;
}

HRESULT	CVWServer::FireOnTrace(BSTR bstr)
{
	HRESULT hr = S_OK;

	START_EVENT(CVWServer, DVWServerEvents)

            hr = pEvent->OnTrace(bstr);

	FINISH_EVENT()

	FireOnReport(CComBSTR("TRACE: "+CString(bstr)),1);
    return hr;
}

HRESULT	CVWServer::FireOnAvailableWorld(BSTR bstrName, int iUsers)
{
	HRESULT hr = S_OK;

		if(m_hCallbackWindowHandle)
		{
			// we need a non-destructing copybstr. TODO: use sysalloc sometime for better efficiency.
			BSTR temp = ::SysAllocString(bstrName);
			ASSERT(CString(bstrName)!="");
			
			if(CString(bstrName)[0]=='/')
				ASSERT(0);
			::PostMessage(m_hCallbackWindowHandle, WM_ONAVAILABLEWORLD, (WPARAM)temp, (LPARAM)iUsers);
		}
		else
		{
			START_EVENT(CVWServer, DVWServerEvents)
			
				hr = pEvent->OnAvailableWorld(bstrName, iUsers);

			FINISH_EVENT()
		}
    return hr;
}

HRESULT	CVWServer::FireOnReport(BSTR textMessage, long lType)
{
	HRESULT hr = S_OK;

	if(m_hCallbackWindowHandle)
	{
		REPORTSTRUCT *pStruct = new REPORTSTRUCT;
		
		// we need a non-destructing copybstr. 
		pStruct->bstrMessage = ::SysAllocString(textMessage);
		
		//find world.
		if(m_pCurrentConnection)
		{
			ConnectionInfo cinfo;
			hr = m_clist.FindConnection(m_pCurrentConnection, cinfo);
			if(FAILED(hr))
			{
				pStruct->bstrWorldName =  NULL;
			}
			else
			{
				pStruct->bstrWorldName =  ::SysAllocString(cinfo.bstrWorldName);
				ASSERT(CString(pStruct->bstrWorldName)!="");
			}
		}
		else
		{
			pStruct->bstrWorldName =  NULL;
		}		
		
		pStruct->lType =  lType;
		
		::PostMessage(m_hCallbackWindowHandle, WM_ONREPORT, (WPARAM)pStruct, (LPARAM)NULL);
	}
	else
	{
		START_EVENT(CVWServer, DVWServerEvents)

				hr = pEvent->OnReport(textMessage, lType);

		FINISH_EVENT()
	}
    return hr;
}

HRESULT	CVWServer::FireOnCommMessage(IVWCommConnection *pconn, CString& strMessage)
{
	HRESULT hr = S_OK;

	START_EVENT(CVWServer, DVWServerEvents)

            hr = pEvent->OnCommMessage((DWORD) (void *) pconn, CComBSTR(strMessage));

	FINISH_EVENT()

	FireOnReport(CComBSTR(strMessage),0);

    return hr;
}

///////////////////////////////////////////////////////////////////////////////////
//Event handlers

HRESULT CVWServer::OnInvokeMethod(IThing* pthing, BSTR bstr, DISPPARAMS* pdispparams)
{
	return S_OK;
};

HRESULT CVWServer::CreateTool_T2(BSTR bstrName, CLSID clsid, IUnknown** ppunk)
{
	ASSERT(ppunk);
	if (ppunk == NULL)
		return E_POINTER;

	ASSERT(m_pTools);

#ifdef _DEBUG
	CString str(bstrName);
#endif
	HRESULT hr = S_OK;
	VARIANT_BOOL fValid = VARIANT_FALSE;

	hr = m_pTools->IsValid(bstrName, &fValid);
	if (FAILED(hr))
		return ReportServerError(hr);

	if (fValid)
	{
#ifdef _DEBUG
		TRACE("CVWServer::CreateTool_T2: tool already exists (%s)\n", str);
#endif
		return ReportServerError(VWOBJECT_E_TOOLEXISTS);
	}

	hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (LPVOID *)ppunk);
	if (FAILED(hr))
		return ReportServerError(hr);

	hr = m_pTools->put_Property(bstrName, CComVariant(*ppunk));

	return ReportServerError(hr);
}

STDMETHODIMP CVWServer::CreateToolFromProgID(BSTR bstrName, BSTR progid, IUnknown** ppunk)
{
	//This has to be done properly so code only executes on CommThread:
	// First, create a queue event.
	// Then transfer below code to ...::CreateTool_T2.
	// Then below, just go queue.put(event).
	return E_NOTIMPL;

/*	CLSID clsid;

	if (SUCCEEDED(CLSIDFromProgID(progid, &clsid)))
		return CreateTool(bstrName, clsid, ppdisp);
	else
	{
#ifdef _DEBUG
		CString str(progid);
		TRACE("CVWClientObject::CreateToolFromProgID: unknown progID (%s)\n", str);
#endif
		return ReportServerError(VWOBJECT_E_UNKNOWNPROGID);
	}
*/
}

STDMETHODIMP CVWServer::get_Tool(BSTR bstrName, IUnknown** ppunk)
{
	//This has to be done properly so code only executes on CommThread:
	// First, create a queue event.
	// Then transfer below code to ...::CreateTool_T2.
	// Then below, just go queue.put(event).
	return E_NOTIMPL;
/*
	HRESULT hr = S_OK;

	ASSERT(ppdisp);
	if (ppdisp == NULL)
		return E_POINTER;

	if (m_pTools == NULL)
	{
		TRACE("CVWClientObject::get_Tool: invalid tool map\n");
		return ReportServerError(VWOBJECT_E_INVALIDTOOLMAP);
	}

	hr = m_pTools->get_Dispatch(bstrName, ppdisp);

	return ReportServerError(hr);
	*/
}

STDMETHODIMP CVWServer::RemoveTool(BSTR name)
{
	//This has to be done properly so code only executes on CommThread:
	// First, create a queue event.
	// Then transfer below code to ...::CreateTool_T2.
	// Then below, just go queue.put(event).
	return E_NOTIMPL;

	/*
	ASSERT(m_pTools);

	if (m_pTools == NULL)
	{
		TRACE("CVWClientObject::RemoveTool: invalid tool map\n");
		return ReportServerError(VWOBJECT_E_INVALIDTOOLMAP);
	}

	HRESULT hr = S_OK;

	hr = m_pTools->Remove(name);

	return ReportServerError(hr);
	*/
}

STDMETHODIMP CVWServer::get_Tools(IPropertyMap** pppropertymap)
{
	//This has to be done properly so code only executes on CommThread:
	// First, create a queue event.
	// Then transfer below code to ...::CreateTool_T2.
	// Then below, just go queue.put(event).
	return E_NOTIMPL;

	/*
	ASSERT(pppropertymap);
	if (pppropertymap == NULL)
		return E_POINTER;

	*pppropertymap = m_pTools;

	SAFEADDREF(*pppropertymap);

	return S_OK;
	*/
}

HRESULT CVWServer::CreateTools_T2()
{
	HRESULT hr = S_OK;
	
	static CComBSTR bstrDService("DService");
	IDServ *pDServ = NULL;
	CComBSTR bstrDSURL;

	// unlike the world and vwclient we need to create m_pTools first, this gets called from another thread.
	ASSERT(m_pTools == NULL);

	hr = CoCreateInstance(CLSID_PropertyMap, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (LPVOID *)&m_pTools);
	if (FAILED(hr))
	{
		TRACE("CVWServer::CreateTools: Failed to create tool map\n");	
		goto ERROR_ENCOUNTERED;
	}

	// create Directory service object.
	hr = CreateTool_T2(bstrDService, CLSID_DServ, (IUnknown**)&pDServ);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	m_pDServ=pDServ;
	SAFEADDREF(m_pDServ);

	hr = GetDirectoryServiceURL(&bstrDSURL.m_str);
	if(FAILED(hr))
	{
			// do we really care?
		hr = S_OK;
	}

	//okay if failed. no notification necessary.
	pDServ->ConnectEx2(NULL, bstrDSURL);
	
ERROR_ENCOUNTERED:
	SAFERELEASE(pDServ);

	return hr;
}

STDMETHODIMP CVWServer::SaveWorld(BSTR bstrWorldName, BSTR *pbstrHRESULT)
{
	Request req;
	req.SaveWorld(CString(bstrWorldName));
	
	//call the queue synchronously.
	Request returnValue=m_queue.putSync(req);
	
	*pbstrHRESULT=returnValue.getHRText().AllocSysString();

	return returnValue.getHR();
}

STDMETHODIMP CVWServer::PerformanceTest(BSTR bstrWorldName, long iType, BSTR *pbstrHRESULT)
{
	Request req;
	req.PerformanceTest(CString(bstrWorldName), iType);
	
	//call the queue synchronously.
	Request returnValue=m_queue.putSync(req);
	
	*pbstrHRESULT=returnValue.getHRText().AllocSysString();

	return returnValue.getHR();
}

STDMETHODIMP CVWServer::SaveWorldAs(BSTR bstrWorldName, BSTR bstrNewWorldName, BSTR *pbstrHRESULT)
{
	Request req;
	req.SaveWorldAs(CString(bstrWorldName), CString(bstrNewWorldName));
	
	//call the queue synchronously.
	Request returnValue=m_queue.putSync(req);
	
	*pbstrHRESULT=returnValue.getHRText().AllocSysString();

	return returnValue.getHR();
}

STDMETHODIMP CVWServer::OpenWorld(BSTR bstrWorldName, BSTR *pbstrHRESULT)
{
	Request req;
	req.OpenWorld(CString(bstrWorldName));
	
	//call the queue synchronously.
	Request returnValue=m_queue.putSync(req);
	
	*pbstrHRESULT=returnValue.getHRText().AllocSysString();

	return returnValue.getHR();
}

STDMETHODIMP CVWServer::SaveWorldScript(BSTR bstrWorldName, BSTR bstrScriptName, BSTR *pbstrHRESULT)
{
	Request req;
	
	req.SaveWorldScript(CString(bstrWorldName), CString(bstrScriptName));
	
	//call the queue synchronously.
	Request returnValue=m_queue.putSync(req);
	
	*pbstrHRESULT=returnValue.getHRText().AllocSysString();

	return returnValue.getHR();
}

STDMETHODIMP CVWServer::DeleteWorldFromSystem(BSTR bstrWorldName, BSTR *pbstrHRESULT)
{
	Request req;
	req.DeleteWorld(CString(bstrWorldName));
	
	//call the queue synchronously.
	Request returnValue=m_queue.putSync(req);
	
	*pbstrHRESULT=returnValue.getHRText().AllocSysString();

	if(SUCCEEDED(returnValue.getHR())) 
		FireOnReport(bstrWorldName, C_CLOSEDOCUMENT);

	return returnValue.getHR();
}

STDMETHODIMP CVWServer::TakeWorldOffline(BSTR bstrWorldName, BSTR *pbstrHRESULT)
{
	Request req;
	req.TakeWorldOffline(CString(bstrWorldName));
	
	//call the queue synchronously.
	Request returnValue=m_queue.putSync(req);
	
	*pbstrHRESULT=returnValue.getHRText().AllocSysString();

	return returnValue.getHR();
}

STDMETHODIMP CVWServer::TakeWorldOnline(BSTR bstrWorldName, BSTR *pbstrHRESULT)
{
	Request req;
	req.TakeWorldOnline(CString(bstrWorldName));
	
	//call the queue synchronously.
	Request returnValue=m_queue.putSync(req);
	
	*pbstrHRESULT=returnValue.getHRText().AllocSysString();

	return returnValue.getHR();
}

//******************************************************
// EnumerateWorlds
// Returns a list of all of the GUIDs for all of the worlds
// in the registry

STDMETHODIMP CVWServer::EnumerateWorlds(IPropertyList **ppGUIDList)
{
	HRESULT	hr = S_OK;
	HKEY keyWorlds = NULL;
	DWORD dwStatus;

	if (ppGUIDList == NULL)
		return E_POINTER;

	*ppGUIDList = NULL;

	// 1. Open the WORLD key
	if (RegCreateKeyEx(HKEY_CURRENT_USER, gStrWorldsKey, 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &keyWorlds, &dwStatus) == ERROR_SUCCESS)
	{
		DWORD dwCurKey = 0;
		char curGUID[255];
		DWORD dwStrSize;
		LONG lResult;
		FILETIME fileTime;
		
		// 2.  Create the list
		hr = CoCreateInstance(CLSID_PropertyList, NULL, CLSCTX_INPROC_SERVER, IID_IObjectProperty, (LPVOID *)ppGUIDList);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		// 3.  Iterate to populate the list
		do
		{
			CComBSTR bstrGUID, bstrPath;
			CComBSTR bstrFinalPath, bstrName;

			dwStrSize = 255;

			lResult = RegEnumKeyEx(keyWorlds, dwCurKey, curGUID, &dwStrSize, NULL, NULL, NULL, &fileTime);
			if (lResult != ERROR_SUCCESS)
				goto ERROR_ENCOUNTERED;

			bstrGUID = curGUID;

			(*ppGUIDList)->AddString(bstrGUID);	

			// EnumerateWorlds doesn't need to do all this, does it?
			// RefreshDS does it already
#if 0
			//check if on disk. if not, erase key.
			hr = FindWorldInRegistry(bstrGUID, &bstrPath.m_str, &bstrName.m_str);
			if (FAILED(hr))
			{
				// if regkeys are screwy, delete world from registry
				/* hr = */ RemoveWorldFromRegistry(bstrGUID);

				goto ERROR_ENCOUNTERED;
			}

			hr = CanonDatabaseName(bstrPath, &bstrFinalPath.m_str);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			//check for existance and read/write permission
			if (_access(CString(bstrFinalPath)+".vwc", 06)!=-1) 
			{
				//get current number of users.

				//Put result in DS.			
				hr = UpdateDS(bstrGUID, bstrPath, 0);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;

				//incr only if world exists.
				dwCurKey++;
			}
			else
			{
				hr = RemoveWorldFromRegistry(bstrGUID);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;
			}	
#endif
		}
		while (SUCCEEDED(hr));

ERROR_ENCOUNTERED:
		if (keyWorlds) 
			RegCloseKey(keyWorlds);
	}

	return hr;
}

//******************************************************
// RefreshDS
// Maintains the auto-expiring DS, and updates registry.
STDMETHODIMP CVWServer::RefreshDS()
{
	TRACE("Refreshing DS on timer.\n");

	HRESULT	hr = S_OK;
	HKEY keyWorlds = NULL;
	DWORD dwStatus;
	IThing* pGlobal = NULL;
	CWorldInfo *pwi;
	IWorld *pworld = NULL;
	static CComBSTR bstrGUIDStatic("GUID");

	// 1. Open the WORLD key
	if (RegCreateKeyEx(HKEY_CURRENT_USER, gStrWorldsKey, 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &keyWorlds, &dwStatus) == ERROR_SUCCESS)
	{
		DWORD dwCurKey = 0;
		CComBSTR bstrGUID;
		char curGUID[255];
		DWORD dwStrSize;
		LONG lResult;
		FILETIME fileTime;
		
		// 3.  Iterate to populate the list
		do
		{
			CComBSTR bstrPath, bstrName, bstrFinalPath;

			dwStrSize = 255;

			lResult = RegEnumKeyEx(keyWorlds, dwCurKey, curGUID, &dwStrSize, NULL, NULL, NULL, &fileTime);
			if (lResult != ERROR_SUCCESS)
				goto ERROR_ENCOUNTERED1;

			// cache BSTR GUID
			bstrGUID = curGUID;

			//check if on disk. if not, erase key.
			hr = FindWorldInRegistry(bstrGUID, &bstrPath.m_str, &bstrName.m_str);
			if (FAILED(hr))
			{
				// if regkeys are screwy, delete world from registry
				/* hr = */ RemoveWorldFromRegistry(bstrGUID);

				hr = S_OK;
				continue;
			}

			hr = CanonDatabaseName(bstrPath, &bstrFinalPath.m_str);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED1;

			//check for existance and read/write permission
			if (_access(CString(bstrFinalPath)+".vwc", 06) != -1) 
			{
				pwi = m_wlist.FindWorld(CString(bstrPath));
				
				if(pwi)
				{
					// Maybe put result in DS.						
					hr = UpdateDS(bstrGUID, bstrPath, pwi->UserCount());
					if (FAILED(hr))
						goto ERROR_ENCOUNTERED1;

					//Update server.
					hr = FireOnAvailableWorld(bstrPath, pwi->UserCount());
					if (FAILED(hr))
						goto ERROR_ENCOUNTERED1;
				}
				else
				{
					// Maybe put result in DS.	
					hr = UpdateDS(bstrGUID, bstrPath, 0);
					if (FAILED(hr))
						goto ERROR_ENCOUNTERED1;

					//Update server.
					hr = FireOnAvailableWorld(bstrPath, 0);
					if (FAILED(hr))
						goto ERROR_ENCOUNTERED1;
				}
					
				//incr only if world exists.
				dwCurKey++;
			}
			else
			{
				/* hr = */ RemoveWorldFromRegistry(bstrGUID);
			}	
		}
		while (lResult == ERROR_SUCCESS);

ERROR_ENCOUNTERED1:
		if (keyWorlds) 
		{
			RegCloseKey(keyWorlds);
			keyWorlds = NULL;
		}
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	SAFERELEASE(pGlobal);

	return hr;
}

//******************************************************
// RemoveWorldsFromDS
// Returns a list of all of the GUIDs for all of the worlds
// in the registry
HRESULT CVWServer::RemoveWorldsFromDS()
{
	HRESULT	hr = E_FAIL;
	HKEY	keyWorlds = NULL;
	DWORD	dwStatus;	

	// 1. Open the WORLD key
	if (RegCreateKeyEx(HKEY_CURRENT_USER, gStrWorldsKey, 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &keyWorlds, &dwStatus) == ERROR_SUCCESS)
	{
		DWORD dwCurKey = 0;
		char curGUID[255];
		DWORD dwStrSize;
		LONG lResult;
		FILETIME fileTime;
		
		// Iterate to populate the list
		do
		{
			dwStrSize = 255;

			lResult = RegEnumKeyEx(keyWorlds, dwCurKey++, curGUID, &dwStrSize, NULL, NULL, NULL, &fileTime);
			if (lResult != ERROR_SUCCESS)
				goto ERROR_ENCOUNTERED;

			//Remove result in DS.			
			hr = m_pDServ->RemoveWorld(CComBSTR(curGUID));
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
		while (SUCCEEDED(hr));

ERROR_ENCOUNTERED:
		if (keyWorlds) 
		{
			RegCloseKey(keyWorlds);
			keyWorlds = NULL;
		}
	}

	return hr;
}

#ifdef _DEBUG

struct _CrtMemState ms0;

void PegHeap()
{
	_CrtMemCheckpoint(&ms0);
	OutputDebugString("Pegged the current heap state\r\n");
}

void CheckHeap()
{
	OutputDebugString("Heap objects allocated after peg:\r\n");
	_CrtMemDumpAllObjectsSince(&ms0);
}


#endif

//********************************************
// ReconnectToDS
// Reconnects to the Directory Server

HRESULT CVWServer::ReconnectToDS()
{
	BSTR	bstrDSURL;

	// 1. Remove all of the worlds for the DS
	RemoveWorldsFromDS();
	
	// 2. Get the new URL
	GetDirectoryServiceURL(&bstrDSURL);

	// 2.5. Disconnect.
	m_pDServ->Disconnect();

	// 3. Connect to it.
	m_pDServ->ConnectEx2(NULL, bstrDSURL);
	
	// 4. Refresh the DS
	RefreshDS();
	
	// 5. Clean up
	SAFEFREESTRING(bstrDSURL);
	return S_OK;
}

HRESULT CVWServer::get_UseAuthentication(BOOL *pVal)
{
	// pass back the NTLM Authenticate option
	*pVal=m_bForceAuthentication;
	return S_OK;
}

HRESULT CVWServer::put_UseAuthentication(BOOL newVal)
{
	HRESULT hr = S_OK;

	// set the NTLM Authentication opton
	m_bForceAuthentication=newVal;

	// update the registry
	hr = SetRegAuthentication(newVal);

	return hr;
}

HRESULT CVWServer::get_AuthenticationGroup(BSTR *bstrGroup)
{
	// get the optional group value for group authentication
	if (bstrGroup)
	{
		SysFreeString(*bstrGroup);
		*bstrGroup=m_bstrAuthenticationGroup.Copy();
		return S_OK;
	}
	return E_POINTER;

}

HRESULT CVWServer::put_AuthenticationGroup(BSTR bstrGroup)
{
	HRESULT hr = S_OK;

	// set the optional local machine group to authenticate against
	m_bstrAuthenticationGroup=bstrGroup;

	// write it to the registry.. helper in reghelp.cpp
	hr = SetRegLocalGroupName(bstrGroup);
	return hr;
}

HRESULT CVWServer::get_RunningOnWinNT(BOOL *pbRunningOnWinNT)
{
	HRESULT hr = S_OK;

	// check for null pointer
	if (pbRunningOnWinNT == NULL)
		return E_POINTER;
	
	// set the value
	*pbRunningOnWinNT=m_bWinNT;
	
	return hr;
}


HRESULT CVWServer::put_ConnectionLimit(DWORD dwLimit)
{
	HRESULT hr = S_OK;

	// (validate?) and set the member
	m_dwMaxConnections = dwLimit;

	// write it to the registry
	hr = SetRegConnectionLimit(dwLimit);

	return hr;
}

HRESULT CVWServer::get_ConnectionLimit(DWORD *dwLimit)
{
	HRESULT hr = S_OK;

	// check invalid pointer
	if (dwLimit == NULL)
		return E_POINTER;

	// return the current value
	*dwLimit=m_dwMaxConnections;

	return hr;
}


HRESULT CVWServer::get_ConnectionCount(long *pCount)
{
	HRESULT hr = S_OK;

	if (pCount == NULL)
		hr = E_FAIL;

	*pCount=m_clist.GetCount();

	return hr;
}
