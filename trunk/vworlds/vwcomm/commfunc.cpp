// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include <vwobject.h>
#include <vwcomm.h>
#include "commobj.h"
#include "marshbuf.h"

#include <process.h>
#include <limits.h>

typedef CComObject<CUnMarshallBufferObject> CComUnMarshallBuffer;

EXTERN_C const CLSID CLSID_UnMarshallBuffer;

/////////////////////////////////////////////////////////////////////
// Parameters

// Turn these on for detailed diagnostic output (to debugger and c:\vworld?.log files)
//#define PL_CHANNEL_DEBUG
#define ENABLE_LOGGING

// size of stack for service threads
#define cbThreadStack		(16*1024)

// input buffer size
#define cbReadBuf			cbMaxIO

// max amount to write at once
// REVIEW: should be set according to winsock options
#define cbWriteMax			4096

// write queue thresholds.  If the number of buffers in the write queue
// is greater than cbufWriteQMax, then write operations will block until
// this number drops below cbufWriteQThreshold
#define cbufWriteQMax		512
#define cbufWriteQThreshold	8

// Write buffer memory block signature 
#define dwSigWB	('P'+('L'<<8)+('W'<<16L)+('B'<<24L))

// These defines control how socket IO is handled
// #define FORCEWIN95MODE
// #define NOLINGER
// #define NONAGLYALGORITHM
// #define BLOCKINGSEND

/////////////////////////////////////////////////////////////////////
// Convenience macros

#define PwbToPv(p)			((char *)(p)+sizeof(WRITEBUFFER))
#define PvToPwb(p) 			((WRITEBUFFER *)((char *)(p)-sizeof(WRITEBUFFER)))
#define IsWriteBuffer(p) 	(PvToPwb(p)->dwSignature==dwSigWB )

/////////////////////////////////////////////////////////////////////
// data structures

// Communication header
//
// We need a header because sockets are stream oriented - partial packets
// may be sent or multiple ones in a single transmission.  When large packets
// are sent, the we receives them in several transmissions.  We look at the first
// transmission to get the packet size and then count down to determine
// the rest of the packet.
//
#pragma warning( disable: 4200 )
#pragma pack(2)

struct COMBLK
{
	// REVIEW: not maximum 64k message size.
	WORD		cchMessage;			// does NOT include size of COMBLK
	BYTE		chSerial;			// serial # (0 for async messages);
	BYTE		chType;				// type of message
	BYTE		rgData[];
}; 

// a write buffer
// NOTE: although this is what a write buffer looks like, PWRITEBUFFER is actually
// defined as a void * and we don't export this definition in order that the
// caller not try to look at this.  A PWRITEBUFFER actually only points down
// at the data following this WRITEBUFFER header
struct WRITEBUFFER
{
	DWORD			dwSignature;		// for consistency checking
	WRITEBUFFER *	pwbNext;			// linked list pointer for write thread
	WRITEBUFFER *	pwbPrev;			// linked list pointer for write thread
	int				cConnection;		// count of connections
	int				cConnectionLive;	// count of live (non-null) connections
	IVWCommConnection *pconn;			// connection (if just one)
	IVWCommConnection **rgpconn;		// connection list (if multiple)
	DWORD			timeEnqueued;		// time when buffer was enqueued (for stats)
	COMBLK			comblk;

	// data follows
};

typedef void *PWRITEBUFFER;

#pragma pack()


/////////////////////////////////////////////////////////////////////
// Log Helpers

#if defined(PL_CHANNEL_DEBUG) && defined(ENABLE_LOGGING)

#include <io.h>
#include <fcntl.h>
#include <time.h>
#include <share.h>
#include <sys/stat.h>

void LogInitialize();
void LogData(LPCTSTR pszHeader, BYTE *pbData, DWORD cbData);
void Log(LPCTSTR psz);

static int fiLogFile = -1;
static int iLogId = 0;

#else

#define LogInitialize()
#define LogData(pszHeader, pbData, cbData)
#define Log(psz)

#endif

/////////////////////////////////////////////////////////////////////
// CVWCommObject comm functions

HRESULT CVWCommObject::CommConstruct()
{
	m_pMessHead = m_pMessTail = NULL;
	m_pwbHead = m_pwbTail = NULL;
	m_sockBase = INVALID_SOCKET;
	m_pthreadAccept = NULL;
	m_heventDataInWriteQueue = NULL;
	m_heventWriteQueueBelowThreshold = NULL;
	m_cThreadRead = 0;
	m_hinstKernel = NULL;
	m_pfnCancel = NULL;
	m_dwCookieNext = 0;
	m_timeLastKeepAlive = 0;

	m_cthreadWrite = 0;
	for (int i=0; i<cThreadWriteMax; i++)
		m_rgpthreadWrite[i] = NULL;

	// statistics
	m_cMessagesInQueue = 0;
	m_cBuffersInQueue = 0;
	m_cPacketsSentTotal = 0;
	m_cPacketsSent = 0;
	m_cPacketsSentPerSecond = 0;
	m_cBytesSentTotal = 0;
	m_cBytesSent = 0;
	m_cBytesSentPerSecond = 0;
	m_cPacketsReceivedTotal = 0;
	m_cPacketsReceived = 0;
	m_cPacketsReceivedPerSecond = 0;
	m_cBytesReceivedTotal = 0;
	m_cBytesReceived = 0;
	m_cBytesReceivedPerSecond = 0;
	m_cQueueSamples = 0;
	m_cSumPacketsInWriteQueue = 0;
	m_cMeanPacketsInWriteQueue = 0;
	m_cSumMessagesInReadQueue = 0;
	m_cMeanMessagesInReadQueue = 0;
	m_cSumTimeInReadQueue = 0;
	m_cMeanTimeInReadQueue = 0;
	m_cTimeInReadQueueSamples = 0;
	m_cSumTimeInWriteQueue = 0;
	m_cMeanTimeInWriteQueue = 0;
	m_cTimeInWriteQueueSamples = 0;

	// set flag indicating whether we're running on Windows 95
	DWORD dwVersion = GetVersion();
	if( dwVersion & 0x80000000 && LOBYTE(LOWORD(dwVersion)) > 3 )
		m_bOnWindows95 = TRUE;
	else
		m_bOnWindows95 = FALSE;

#ifdef FORCEWIN95MODE

	m_bOnWindows95 = TRUE;

#endif

	return S_OK;

}

HRESULT CVWCommObject::CommInit()
{
	int nRet;
	WSADATA wsa;

	nRet = WSAStartup(MAKEWORD(1,1), &wsa);
	if (nRet!=0)
		return VWCOMM_E_WINSOCKERROR;

	// create write q events
	m_heventDataInWriteQueue = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (m_heventDataInWriteQueue==NULL)
		return E_OUTOFMEMORY;
	m_heventWriteQueueBelowThreshold = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (m_heventWriteQueueBelowThreshold==NULL)
		return E_OUTOFMEMORY;

	// write threads are started later (once we know whether we're
	// server or client side).

	// set time stamp for statistics
	m_timeSampleStart = GetTickCount();

	return S_OK;
	
}

HRESULT CVWCommObject::CommTerminate()
{
	int ithread;

	// ignore errors here - try as hard as possible to terminate

	// first, disconnect everything
	CommDisconnectAll();

	// kill any pending (NT) read threads.
	for (ithread=0; ithread<cThreadReadMax; ithread++)
		m_rgThreadInfo[ithread].Terminate();

	// kill write threads

	// set the data available event to unblock write threads
	if (m_heventDataInWriteQueue)
	{
		for (ithread=0; ithread<m_cthreadWrite; ithread++)
		{
			SetEvent(m_heventDataInWriteQueue);

			// yield here to let write thread kick off
			Sleep(10);
		}

		// now assure that the threads are dead
		for (ithread=0; ithread<m_cthreadWrite; ithread++)
		{
			if (m_rgpthreadWrite[ithread] != NULL)
			{
				// wait for thread to disapper
				WaitForEvent(m_rgpthreadWrite[ithread]->m_hThread);

				delete m_rgpthreadWrite[ithread];
				m_rgpthreadWrite[ithread] = NULL;
			}
		}
		m_cthreadWrite = 0;

		// nuke handle
		CloseHandle(m_heventDataInWriteQueue);
		m_heventDataInWriteQueue = NULL;
	}

	if (m_heventWriteQueueBelowThreshold != NULL)
	{
		CloseHandle(m_heventWriteQueueBelowThreshold);
		m_heventWriteQueueBelowThreshold = NULL;
	}

	// close base socket to close accept thread
	if (m_sockBase != INVALID_SOCKET)
	{
		closesocket(m_sockBase);
		m_sockBase = INVALID_SOCKET;

		ASSERT(m_pthreadAccept != NULL);

		// wait for thread to disappear
		WaitForEvent(m_pthreadAccept->m_hThread);

		// pause, to give the thread a chance to 
		// completely clean up
		// REVIEW: should fine a way to avoid this
		Sleep(100);

		delete m_pthreadAccept;
		m_pthreadAccept = NULL;
	}

	// release kernel library
	if (m_hinstKernel!=NULL)
	{
		FreeLibrary(m_hinstKernel);
		m_hinstKernel = NULL;
	}
	m_pfnCancel = NULL;

 	// release queued read messages
	CommLockMessages();
	while (m_pMessHead != NULL)
	{
		CVWCommMessage *pmess;

		pmess = m_pMessHead;
		m_pMessHead = m_pMessHead->GetNext();
		if (m_pMessHead==NULL)
			m_pMessTail = NULL;
		m_cMessagesInQueue--;

		pmess->Release();
	}
	CommUnlockMessages();

	// release queued write messages
	CommLockWriteQueue();
	while (m_pwbHead != NULL)
	{
		WRITEBUFFER *pwb;

		pwb = m_pwbHead;
		m_pwbHead = pwb->pwbNext;
		if (m_pwbHead!=NULL)
			m_pwbHead->pwbPrev = NULL;
		else
			m_pwbTail = NULL;
		m_cBuffersInQueue--;
	}
	CommUnlockWriteQueue();

	return S_OK;
}

HRESULT CVWCommObject::CommDisconnectAll()
{
	HRESULT hr;

	// Walk through the connection list, disconnecting everything
	CommLockConnList();
	POSITION pos;
	for (pos = m_listConnection.GetHeadPosition(); pos != NULL;)
	{
		// get association
		CVWCommConnection *pconn;

		pconn = m_listConnection.GetNext(pos);
		ASSERT(pconn != NULL);

		hr = CommDisconnect(pconn, FALSE);
		if (FAILED(hr))
		{
			// assert, but continue
			ASSERT(FALSE);
		}

		// release connection
		pconn->Release();
	}

	m_listConnection.RemoveAll();

	CommUnlockConnList();

	return S_OK;
}

HRESULT CVWCommObject::CommAccept(int iPort)
{
	SOCKADDR_IN	sa;
	BOOL		bTrue = TRUE;

	// should be this if in virgin 
	ASSERT(m_sockBase==INVALID_SOCKET);

	// open the base socket
	m_sockBase = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sockBase == INVALID_SOCKET)
		return VWCOMM_E_WINSOCKERROR;

#ifdef NONAGLYALGORITH
	if (setsockopt(m_sockBase, IPPROTO_TCP, TCP_NODELAY, (const char *) &bTrue, sizeof(BOOL)) != 0)
	{
		closesocket(m_sockBase);
		m_sockBase = INVALID_SOCKET;
		return VWCOMM_E_WINSOCKERROR;
	}
#endif

#ifdef NOLINGER
	LINGER linger;

	linger.l_onoff = 1;
	linger.l_linger = 0;
	if (setsockopt(m_sockBase, SOL_SOCKET, SO_LINGER, (const char *) &linger, sizeof(linger)) != 0)
	{
		closesocket(m_sockBase);
		m_sockBase = INVALID_SOCKET;
		return VWCOMM_E_WINSOCKERROR;
	}
#endif

#ifdef WINSOCKKEEPALIVE
	if (setsockopt(m_sockBase, SOL_SOCKET, SO_KEEPALIVE, (const char *) &bTrue, sizeof(BOOL)) != 0)
	{
		closesocket(m_sockBase);
		m_sockBase = INVALID_SOCKET;
		return VWCOMM_E_WINSOCKERROR;
	}
#endif

	// initialize
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	sa.sin_port = htons((u_short)iPort);

	// bind
	if (bind(m_sockBase, (PSOCKADDR) &sa, sizeof(sa))== SOCKET_ERROR)
	{
		closesocket(m_sockBase);
		m_sockBase = INVALID_SOCKET;
		return VWCOMM_E_WINSOCKERROR;
	}

	// listen 
	if (listen(m_sockBase, cConnectBacklogMax)<0)
	{
		closesocket(m_sockBase);
		m_sockBase = INVALID_SOCKET;
		return VWCOMM_E_WINSOCKERROR;
	}

	// NOTE: Write threads are started as connections are made

	// start connection thread
	m_pthreadAccept = AfxBeginThread(CommAcceptThread, (void *) this);
	if (m_pthreadAccept==NULL)
	{
		closesocket(m_sockBase);
		m_sockBase = INVALID_SOCKET;
		return VWCOMM_E_COULDNTINITIALIZE;
	}

	// specify that thread not disappear
	m_pthreadAccept->m_bAutoDelete = FALSE;

	return S_OK;
}

HRESULT CVWCommObject::CommAssignReadThread(CVWCommConnection* pconn)
{
	HRESULT		hr;
	int			iThreadRead = -1;
	int			cConnectMin = INT_MAX;
	CWinThread *pthread;

	CReadThreadInfo *pti;

	// if haven't created all read threads, just create another one
	if( m_cThreadRead < cThreadReadMax )
	{
		// peg and bump
		iThreadRead = m_cThreadRead++;
		pti = &m_rgThreadInfo[iThreadRead];

		// initialize tht thread info
		hr = pti->Initialize(this);
		if (FAILED(hr))
			return hr;

		// if we're on Windows 95, we use a dedicated reader thread and sync I/O
		// if on NT, we use async I/O.  On Windows 95, we're limited to cThreadReadMax connections
		if (m_bOnWindows95)
			pthread = AfxBeginThread(CommW95ReadThread, (void *) pti);
		else
			pthread = AfxBeginThread(CommNTReadThread, (void *) pti);

		if (pthread==NULL)
			return E_FAIL;

		// specify that thread not disappear
		pthread->m_bAutoDelete = FALSE;

		// set thread
		hr = pti->SetThread(pthread, m_bOnWindows95);
		if (FAILED(hr))
			return hr;
	}
	else
	{
		// if on Win95, see if we've got an old "slot" open
		if( m_bOnWindows95)
		{
			for( iThreadRead=0; iThreadRead<cThreadReadMax; iThreadRead++)
				if (m_rgThreadInfo[iThreadRead].ConnectionCount() == 0)
					break;

			if( iThreadRead==cThreadReadMax )
				return E_FAIL;

			// reuse thread slot (but create new thread)
			pti = &m_rgThreadInfo[iThreadRead];

			// start a read thread
			pthread = AfxBeginThread(CommW95ReadThread, (void *) pti);
			if (pthread == NULL)
				return E_FAIL;

			// specify that thread not disappear
			pthread->m_bAutoDelete = FALSE;

			// set thread
			hr = pti->SetThread(pthread, TRUE);
			if (FAILED(hr))
				return hr;
		}
		else
		{
			// On NT, iterate, looking for thread with fewest connections
			for( int iThread=0; iThread<cThreadReadMax; iThread++ )
			{
				if( m_rgThreadInfo[iThread].ConnectionCount() < cConnectMin )
				{
					iThreadRead = iThread;
					cConnectMin = m_rgThreadInfo[iThread].ConnectionCount();
				}
			}
			ASSERT( iThreadRead != -1 );
			pti = &m_rgThreadInfo[iThreadRead];
		}
	}

	// Start operation
	return pti->StartReadOnConnection(pconn);
}

HRESULT CVWCommObject::CommEnqueueMessage(int nMessageStatus, CVWCommConnection *pconn, BYTE *pbData, DWORD cbData, char chSerial)
{
	ASSERT(pconn->Type()!=CVWCommConnection::eUnknown);
	ASSERT(pconn->Type()!=CVWCommConnection::eControlHTTP);

	// create a message
	typedef CComObject<CVWCommMessage> CComCommMessage;
	CComCommMessage* pmess = NULL;

	ASSERT(pconn->Type()==CVWCommConnection::eNative || pconn->Type()==CVWCommConnection::ePrimaryHTTP);

	pmess = new CComCommMessage();
	if (pmess == NULL)
		return E_OUTOFMEMORY;

	// bump ref count (==1)
	pmess->AddRef();

	// initialize it
	pmess->SetStatus(nMessageStatus);
	pmess->Initialize(this, pconn);

	if (pbData!=NULL && cbData!=0)
	{
		HRESULT hr;
		hr = pmess->SetData(pbData, cbData, chSerial);
		if (FAILED(hr))
		{
			ASSERT(FALSE);
			return hr;
		}
	}

	// enqueue it
	pmess->SetTimeStamp(GetTickCount());
	CommLockMessages();
	if (m_pMessTail==NULL)
	{
		ASSERT(m_pMessHead==NULL);
		m_pMessHead = m_pMessTail = pmess;
	}
	else
	{
		m_pMessTail->SetNext(pmess);
		m_pMessTail = pmess;
	}
	m_cMessagesInQueue++;
	CommUnlockMessages();

	// tell site that there's data available
	ASSERT(m_pCommSite);
	m_pCommSite->OnMessageAvailable_T2(this);

	return S_OK;
}

HRESULT CVWCommObject::CommRemoveReplyMarker(void)
{
	CVWCommMessage *pmess=NULL;
	HRESULT hr = S_OK;

	// dequeue one
	// NOTE that we don't do any refcount changes since we'd be
	// adding one (for the return value) and subtracting one
	// for the list removal
		
	int iType;

	CommLockMessages();

		ASSERT(m_pMessHead);
		if (m_pMessHead != NULL)
		{
			// see what type of message we have
			hr = m_pMessHead->GetStatus(&iType);
			//should never fail.
			if (FAILED(hr)) 
			{
				ASSERT(0); 
				return E_FAIL;
			}

			//SANITY CHECK.
			#ifdef _DEBUG
/*				if(iType==CVWCommMessage::statusHaveResponse)
				{
					CVWCommConnection *pconn = NULL;
					// get the connection out of the message
					hr = m_pMessHead->GetConnection((IVWCommConnection**)&pconn);
					if (FAILED(hr))
						ASSERT(0);

					ASSERT(pconn->m_bHaveReply);

					SAFERELEASE(pconn);
				}
*/			#endif

			ASSERT(iType==CVWCommMessage::statusHaveResponse);
			if(iType==CVWCommMessage::statusHaveResponse)
			{
				pmess = m_pMessHead;
				m_pMessHead = m_pMessHead->GetNext();
				if (m_pMessHead==NULL)
					m_pMessTail = NULL;
				m_cMessagesInQueue--;
			}
		}
		
	CommUnlockMessages();

	// update statistics
	if (pmess != NULL)
	{
		CommUpdateQueueTimeStatistics(pmess->m_timeEnqueued, 0);
		pmess->Release();			
	}

	return S_OK;
}

HRESULT CVWCommObject::CommGetMessage(IVWCommMessage **ppmess)
{
	CVWCommMessage *pmess=NULL;
	HRESULT hr = S_OK;

	if (ppmess==NULL)
		return E_POINTER;

	*ppmess = NULL;

	// dequeue one
	// NOTE that we don't do any refcount changes since we'd be
	// adding one (for the return value) and subtracting one
	// for the list removal
	
		int iType;

	CommLockMessages();

		if (m_pMessHead != NULL)
		{
			// see what type of message we have
			hr = m_pMessHead->GetStatus(&iType);
			//should never fail.
			if (FAILED(hr)) ASSERT(0); 

			//SANITY CHECK.
			#ifdef _DEBUG
/*				if(iType==CVWCommMessage::statusHaveResponse)
				{
					CVWCommConnection *pconn = NULL;
					// get the connection out of the message
					hr = m_pMessHead->GetConnection((IVWCommConnection**)&pconn);
					if (FAILED(hr))
						ASSERT(0);

					ASSERT(pconn->m_bHaveReply);

					SAFERELEASE(pconn);
				}
*/			#endif

			if(iType!=CVWCommMessage::statusHaveResponse)
			{
				pmess = m_pMessHead;
				m_pMessHead = m_pMessHead->GetNext();
				if (m_pMessHead==NULL)
					m_pMessTail = NULL;
				m_cMessagesInQueue--;
			}
		}
		
	CommUnlockMessages();

	// update statistics
	if (pmess != NULL)
	{
		CommUpdateQueueTimeStatistics(pmess->m_timeEnqueued, 0);
		*ppmess = (IVWCommMessage *) pmess;
	}

	return S_OK;
}

HRESULT CVWCommObject::CommConnect(LPCTSTR pszRemote, int nPort, VARIANT_BOOL bTunnel, CVWCommConnection **ppconn)
{
	HRESULT		hr;
	SOCKET		sockNew;
	SOCKADDR_IN	sa;
	PHOSTENT	phe;
	ULONG		ulAddr;
	BOOL		bTrue = TRUE;
	CString		strServer;
	int			nPortServer;

	ASSERT(ppconn);
	*ppconn = NULL;

	// set defaults
	strServer = pszRemote;
	nPortServer = nPort;

	// if we're tunneling, check the registry to get
	// proxy information
	if (bTunnel==VARIANT_TRUE)
	{
		// Tunneling NOT supported by FastComm (Virtual Worlds V1.5)!
		return E_INVALIDARG;

		BOOL bUseProxy;
		CString strProxy;
		int nPortProxy;

		if (FAILED(CommGetProxyServer(pszRemote, bUseProxy, strProxy, nPortProxy)))
			return E_FAIL;

		if (bUseProxy)
		{
			strServer = strProxy;
			nPortServer = nPortProxy;
		}

	}

	// create unbound socket
	sockNew = socket( AF_INET, SOCK_STREAM, 0);
	if (sockNew == INVALID_SOCKET )
		return VWCOMM_E_WINSOCKERROR;

#ifdef NONAGLYALGORITHM
	if (setsockopt( sockNew, IPPROTO_TCP, TCP_NODELAY, (const char *) &bTrue, sizeof(BOOL)) != 0)
		return VWCOMM_E_WINSOCKERROR;
#endif

#ifdef NOLINGER
	LINGER linger;

	linger.l_onoff = 1;
	linger.l_linger = 0;
	if (setsockopt(sockNew, SOL_SOCKET, SO_LINGER, (const char *) &linger, sizeof(linger) )!= 0)
		return VWCOMM_E_WINSOCKERROR;
#endif

#ifdef WINSOCKKEEPALIVE
	if (setsockopt(sockNew, SOL_SOCKET, SO_KEEPALIVE, (const char *) &bTrue, sizeof(BOOL)) != 0)
		return VWCOMM_E_WINSOCKERROR;
#endif

	// compute the address of the socket

	// first, see if a dotted address
	ulAddr = inet_addr( strServer );

	if( ulAddr == 0xFFFFFFFF )
	{
		// nope, try by name
		phe = gethostbyname( strServer );
	}
	else
	{
		// yep, do by addr
		phe = gethostbyaddr( (const char *) &ulAddr, sizeof(ulAddr), PF_INET );
	}

	if( phe == NULL )
	{
		// invalid remote name
		return VWCOMM_E_WINSOCKERROR;
	}
	
	// set the network address
	sa.sin_family = AF_INET;
	memcpy ((void *) &(sa.sin_addr), phe->h_addr, phe->h_length);

	// set the port number (hardwired)
	sa.sin_port = htons(nPortServer);

	// connect to server
	if (connect(sockNew, (PSOCKADDR) &sa, sizeof(sa)) < 0)
	{
		closesocket(sockNew);
		return VWCOMM_E_WINSOCKERROR;
	}

	// create and initialize a connection object
	typedef CComObject<CVWCommConnection> CComCommConnection;

	CComCommConnection* pconn = new CComCommConnection();
	if (pconn == NULL)
	{
		closesocket(sockNew);
		return E_OUTOFMEMORY;
	}

	// put it in the connection list (this bumps the ref count to 1)
	hr = CommAddConnection(pconn);
	ASSERT(SUCCEEDED(hr));

	// set stuff and prepare request header
	CString strHeader;
	pconn->Initialize(this, sockNew, (PSOCKADDR) &sa, pszRemote, nPort, TRUE);
	if (bTunnel==VARIANT_TRUE)
	{
		CString strFormat =		"GET http://%s:%d/PRIMARY HTTP/1.0\r\n"
								"Accept: application/octet-stream\r\n"
								"Connection: Keep-Alive\r\n"
								"Cache-Control: no-cache\r\n"
								"\r\n";

		// NOTE: Don't set the connection type yet - we want to 
		// set it in CommHandleReply so that we can read the cookie
		// pconn->SetConnectionType(CVWCommConnection::ePrimaryHTTP);
		strHeader.Format(strFormat, pszRemote, nPort);
	}
	else
	{
		CString strFormat =		"GET http://%s/NATIVE HTTP/1.0\r\n"
								"Accept: application/octet-stream\r\n"
								"Cache-Control: no-cache\r\n"
								"Connection: Keep-Alive\r\n"
								"\r\n";

		strHeader.Format(strFormat, pszRemote);
		pconn->SetConnectionType(CVWCommConnection::eNative);
	}

	// start a write thread (just one - assume client "mode")
	if (m_cthreadWrite < cThreadWriteMax)
	{
		int ithread = m_cthreadWrite++;

		ASSERT(m_rgpthreadWrite[ithread]==NULL);
		m_rgpthreadWrite[ithread] = AfxBeginThread( CommWriteThread, (void *) this);
		if (m_rgpthreadWrite[ithread]==NULL)
			return VWCOMM_E_COULDNTINITIALIZE;

		// specify that thread not disappear
		m_rgpthreadWrite[ithread]->m_bAutoDelete = FALSE;

	}

#ifdef PL_CHANNEL_DEBUG
	{
		CString str;
		str.Format("VWComm: Created new %s connection: 0x%08X\r\n", bTunnel ? "tunneled" : "native", pconn );
		OutputDebugString((LPCTSTR)str);
	}
#endif
	// start initial read on it - if this fails, it's cause there's too
	// much work going on (the read threads are too busy and none can take on
	// the new thread.)  What we do here is report and error and close the
	// connection.  This is important so that others don't try writing to
	// this socket (and then get blocked since there's no one around to read it).

	hr = CommAssignReadThread(pconn);
	if (FAILED(hr))
	{
		DWORD dwError = ::GetLastError();

#ifdef PL_CHANNEL_DEBUG
		{
			CString str;
			str.Format("Connect: AssignReadThread failed at time: %d\n", dwError, ::GetTickCount() );
			OutputDebugString((LPCTSTR)str);
		}
#endif
		// Clean up and exit with error code
		CommHandleError(pconn, (int) dwError, FALSE, TRUE);
		return ReportCommError(hr);
	}

	// send request header
	hr = CommWriteToSocket(sockNew, (BYTE *) (LPCTSTR) strHeader, strHeader.GetLength());
	if (FAILED(hr))
	{
#ifdef PL_CHANNEL_DEBUG
		OutputDebugString("CVWComm: Failed to write header to socket\n");
#endif
		// Clean up and exit with error code
		CommHandleError(pconn, (int) hr, FALSE, TRUE);
		return ReportCommError(hr);
	}

	// if we're tunneling, wait for a while to get the cookie for the connection
	if (bTunnel)
	{
		DWORD dwStart = ::GetTickCount();
		while(pconn->Type()==CVWCommConnection::eUnknown && (::GetTickCount()-dwStart)<cmsecWaitDefault)
			Sleep(100);

		// if it's still unknown, then we failed to connect
		if (pconn->Type()==CVWCommConnection::eUnknown)
		{
			// Clean up and exit with error code
			CommHandleError(pconn, VWCOMM_E_COULDNTINITIALIZE, FALSE, TRUE);
			return VWCOMM_E_COULDNTINITIALIZE;
		}
	}

	// return (bump ref count for return)
	pconn->AddRef();
	*ppconn = pconn;
	
	return S_OK;
}

HRESULT CVWCommObject::CommDisconnect(CVWCommConnection *pconn, BOOL bRemoveFromList)
{
	int nType = pconn->Type();

	// peg (to not disappear)
	pconn->AddRef();

	// if this is a primary connection and we have an associated connection, disconnect
	// that one first.
	// REVIEW: First?
	if (nType==CVWCommConnection::ePrimaryHTTP)
	{
		if (pconn->m_pconnPartner)
			CommDisconnect(pconn->m_pconnPartner, TRUE);

		ASSERT(pconn->m_pconnPartner==NULL);
	}

	// set flag
	pconn->m_bInDisconnect = TRUE;

	// if m_pti is clear, that means that we've already come through here
	// before - don't try stop the read thread again
	if (pconn->m_pti != NULL)
	{
		// first, stop the read thread
		pconn->m_pti->StopReadOnConnection(pconn);

		// zap thread info to indicate we've already stopped the read thread
		pconn->m_pti = NULL;
	}

	// close the socket, if nessary
	if (pconn->m_sock != INVALID_SOCKET)
		CommCloseConn(pconn);

	pconn->m_bInDisconnect = FALSE;

	// if we're waiting for a message, unblock wait loop so that it can pass up notification of message available
	if (pconn->m_bInSendSynch)
	{
		pconn->m_bReplyFailed = TRUE;
		SetEvent(pconn->m_heventHaveData);
	}

	// if we're a client-side controlHTTP connection and our
	// associated PRIMARY connection is waiting for a message,
	// set flag and unblock it
	CVWCommConnection *pconnPartner = pconn->m_pconnPartner;
	if (pconnPartner!=NULL)
	{
		if (pconn->ClientSide() && nType==CVWCommConnection::eControlHTTP)
		{
			if (pconnPartner->m_bInSendSynch)
			{
				pconnPartner->m_bReplyFailed = TRUE;
				SetEvent(pconnPartner->m_heventHaveData);
			}
		}
	}

	// zap the connection (if requested to) (reduces ref count)
	if (bRemoveFromList)
		CommRemoveConnection(pconn);

	// delink associated connection if any (closed already for primary HTTP conns)
	if (pconnPartner != NULL)
	{
		if(pconnPartner->m_pconnPartner==pconn)
		{
			// release backreference (from partner to this one)
			pconnPartner->m_pconnPartner->Release();
			pconnPartner->m_pconnPartner = NULL;
		}

		// release reference to partner
		pconnPartner->Release();
		pconn->m_pconnPartner = NULL;
	}

	// unpeg
	pconn->Release();

	return S_OK;
}

HRESULT CVWCommObject::CommReadAsync(CVWCommConnection *pconn)
{
	HRESULT hr = S_OK;

	ASSERT(!m_bOnWindows95);

	if (pconn->m_sock==INVALID_SOCKET)
		return E_FAIL;

	if (!ReadFileEx((HANDLE) pconn->m_sock,
					pconn->m_pbRead + pconn->m_cbRead,
					pconn->m_cbReadMax - pconn->m_cbRead,
					&pconn->m_ols,
					CommReadCompletionFunction))
	{
		// if we're not disconnecting, report error
		if (!pconn->m_bInDisconnect)
		{
			// closes the socket, enqueues an error and removes the connection
			CommHandleError(pconn, CVWCommMessage::statusReadError, TRUE, TRUE);
			hr = E_FAIL;
		}
	}

	return hr;
}

void WINAPI CVWCommObject::CommReadCompletionFunction(DWORD dwError, DWORD cbTransferred, LPOVERLAPPED pols)
{
	// get the comm pointer out of the overlapped structure
	CVWCommConnection *pconn = (CVWCommConnection *) pols->hEvent;
	ASSERT(pconn);

	// check for errors
	if (dwError!=0 || cbTransferred==0)
	{
		// if not aborted and not disconnecting, then report error
		if (dwError != ERROR_OPERATION_ABORTED && !pconn->m_bInDisconnect)
		{
			// cleanup
			pconn->m_pcomm->CommHandleError(pconn, CVWCommMessage::statusReadError, TRUE, TRUE);
		}

	}
	else
	{
		ASSERT(pconn->m_cbRead + cbTransferred <= pconn->m_cbReadMax);

		// update information
		pconn->m_cbRead += cbTransferred;

		// process the information
		// NOTE: Don't handle errors here since already handled in CommHandleInput
		if (SUCCEEDED(pconn->m_pcomm->CommHandleInput(pconn)))
		{
			// start up another read operation
			pconn->m_pcomm->CommReadAsync(pconn);
		}
	}
}

HRESULT	CVWCommObject::CommAllocWriteBuffer(IMarshallBuffer *pcb, PWRITEBUFFER *ppwb)
{
	HRESULT hr;
	DWORD cb, cbExt;
	WRITEBUFFER *pwb;

	if (ppwb==NULL)
		return E_POINTER;

	*ppwb = NULL;

	hr = pcb->get_Length(&cb);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// adjust size (for header stuff)
	cbExt = cb + sizeof(WRITEBUFFER);

	// check size
	// Possible to exceed this in recent tests of FastComm server with high load.
//	if (cbExt > 65535)
//	{
//		hr = E_INVALIDARG;
//		goto ERROR_ENCOUNTERED;
//	}

	// allocate
	pwb = (WRITEBUFFER *) new BYTE [cbExt];
	if (pwb==NULL)
	{
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	pwb->WRITEBUFFER::WRITEBUFFER();

	// set up block
	pwb->dwSignature = dwSigWB;
	pwb->pwbPrev = NULL;
	pwb->pwbNext = NULL;
	pwb->cConnection = 0;
	pwb->cConnectionLive = 0;
	pwb->pconn = NULL;
	pwb->rgpconn = NULL;
	pwb->comblk.chSerial = 0;
	pwb->comblk.cchMessage = (WORD) cb;

	// bump the pointer to skip past the WRITEBUFFER stuff
	*ppwb = PwbToPv(pwb);

	// copy the data into it
	hr = pcb->CopyFrom((BYTE *) *ppwb, cb);

ERROR_ENCOUNTERED:

	return hr;

}

HRESULT	CVWCommObject::CommAllocKeepAliveBuffer(PWRITEBUFFER *ppwb)
{
	WRITEBUFFER *pwb;

	if (ppwb==NULL)
		return E_POINTER;

	*ppwb = NULL;

	// allocate
	pwb = (WRITEBUFFER *) new BYTE [sizeof(WRITEBUFFER)];
	if (pwb==NULL)
		return E_OUTOFMEMORY;

	// set up block
	pwb->dwSignature = dwSigWB;
	pwb->pwbPrev = NULL;
	pwb->pwbNext = NULL;
	pwb->cConnection = 0;
	pwb->cConnectionLive = 0;
	pwb->pconn = NULL;
	pwb->rgpconn = NULL;
	pwb->comblk.chSerial = 0;
	pwb->comblk.cchMessage = 0;

	// bump the pointer to skip past the WRITEBUFFER stuff
	*ppwb = PwbToPv(pwb);

	return S_OK;

}

HRESULT CVWCommObject::CommWrite(CVWCommConnection *pconn, PWRITEBUFFER pvData, BYTE chSynchSerial, BYTE chMsgType)
{
	IVWCommConnection *pIVWCommConnection = pconn;
	return CommWriteExt(1, &pIVWCommConnection, pvData, chSynchSerial, chMsgType);
}

HRESULT CVWCommObject::CommWriteExt(int cConnection, IVWCommConnection *rgpconn[], PWRITEBUFFER pvData, BYTE chSynchSerial, BYTE chMsgType)
{
	WRITEBUFFER *pwb;

	// check the write buffer
	if (!IsWriteBuffer(pvData))
		return E_INVALIDARG;

	// adjust incoming pointer
	pwb = PvToPwb(pvData);

	// handle single channel as special case
	if (cConnection==1)
	{
#ifdef PL_CHANNEL_DEBUG
		CString strMsg;
		char *pszType;
		if (chMsgType==typeAsync)
			pszType = "asynchronous";
		else if (chMsgType==typeSynch)
			pszType = "synchronous";
		else if (chMsgType==typeReply)
			pszType = "reply";
		else if (chMsgType==typeKeepAlive)
			pszType = "keep-alive";
		else
			pszType = "*BAD*";
		strMsg.Format("CVWComm: Writing %s type message to connection 0x%08X\r\n", pszType, rgpconn[0]);
		OutputDebugString(strMsg);
#endif
		pwb->cConnection = pwb->cConnectionLive = 1;
		pwb->pconn = rgpconn[0];
		rgpconn[0]->AddRef();
	}
	else
	{
#ifdef PL_CHANNEL_DEBUG
		CString strMsg;
		char *pszType;
		if (chMsgType==typeAsync)
			pszType = "asynchronous";
		else if (chMsgType==typeSynch)
			pszType = "synchronous";
		else if (chMsgType==typeReply)
			pszType = "reply";
		else if (chMsgType==typeKeepAlive)
			pszType = "keep-alive";
		else
			pszType = "*BAD*";
		strMsg.Format("CVWComm: Writing %s type message to multiple connections\r\n", pszType);
#endif
		// allocate connection array
		typedef IVWCommConnection *PIVWCOMMCONNECTION;
		pwb->rgpconn = (PIVWCOMMCONNECTION *) new PIVWCOMMCONNECTION[cConnection];
		if (pwb->rgpconn==NULL)
			return E_OUTOFMEMORY;

		// set the count
		pwb->cConnection = pwb->cConnectionLive = cConnection;

		// copy and addref
		for (int iconn=0; iconn<cConnection; iconn++)
		{
			pwb->rgpconn[iconn] = rgpconn[iconn];
			rgpconn[iconn]->AddRef();
		}
	}

	// set other fields
	pwb->comblk.chSerial = chSynchSerial;
	pwb->comblk.chType = chMsgType;

#ifndef BLOCKINGSEND

	// enqueue the buffer
	pwb->timeEnqueued = GetTickCount();
	CommLockWriteQueue();
	if (m_pwbTail==NULL)
	{
		ASSERT(m_pwbHead==NULL);

		m_pwbHead = m_pwbTail = pwb;

	}
	else
	{
		// doubly linked list append
		m_pwbTail->pwbNext = pwb;
		pwb->pwbPrev = m_pwbTail;
		m_pwbTail = pwb;
	}

	m_cBuffersInQueue++;
	CommUnlockWriteQueue();

	// set the event to kick off the write buffer(s)
	for (int ithread=0; ithread<m_cthreadWrite; ithread++)
	{
		SetEvent(m_heventDataInWriteQueue);
		Sleep(0);
	}

#ifdef BLOCKIFWRITEQUEUEABOVETHRESHOLD

	// if too many buffers enqueued, block until below threshold
	if (m_cBuffersInQueue > cbufWriteQMax)
	{
		if (!WaitForEvent(m_heventWriteQueueBelowThreshold))
		{
			TRACE("VWCommObject::CommWrite.  Timed out waiting for write queue to go below threshold\n");
			ASSERT(FALSE);
		}
	}

#endif

	return S_OK;

#else

	// just send
	return CommSend(pwb);

#endif

}

HRESULT CVWCommObject::CommWriteSynch(CVWCommConnection *pconn, PWRITEBUFFER pvData, BYTE chSynchSerial, IUnMarshallBuffer **ppcbReply)
{
	HRESULT hr;

	// REVIEW: we COULD support nested synchronous sends, but we wont
	if (pconn->m_bInSendSynch) 
		return VWCOMM_E_ALREADYINWRITESYNCH;

	ASSERT(ppcbReply);
	*ppcbReply = NULL;

	// set flags
	pconn->m_bInSendSynch = TRUE;
	pconn->m_bReplyFailed = FALSE;

	// allocate a reply buffer
	ASSERT(pconn->m_pBufferReply==NULL);

	pconn->m_pBufferReply = new CComUnMarshallBuffer();
	if (pconn->m_pBufferReply==NULL)
		return E_OUTOFMEMORY;
	pconn->m_pBufferReply->AddRef();

	// set the expected serial # and the reply flag
	pconn->m_chSynchSerialReply = chSynchSerial;
	pconn->m_bHaveReply = FALSE;

#ifdef PL_CHANNEL_DEBUG
	CString strMsg;
	strMsg.Format("CVWComm: Sending sync message (%d) on connection: 0x%08X\r\n", (int)chSynchSerial, pconn);
	OutputDebugString(strMsg);
#endif

	// send the message
	hr = CommWrite(pconn, pvData, chSynchSerial, typeSynch);
	if (FAILED(hr))
		return hr;

	// peg the connection (in case it disappears as a result of a Disconnect() in 
	// the Site callback
	pconn->AddRef();

	for(;;)
	{
		// if there are any messages enqueued, call the site
		// to handle them
		if (m_pMessHead != NULL)
		{
			ASSERT(m_pCommSite);
			m_pCommSite->OnMessageAvailable(this);
		}

		// wait for reply
		//if (!WaitForEvent(pconn->m_heventHaveData, msecWaitWriteSynch))
		//if (!WaitForEvent(pconn->m_heventHaveData, 10*60000))		
		if (!WaitForEvent(pconn->m_heventHaveData, 120000))		
		{

#ifdef PL_CHANNEL_DEBUG
			CString strMsg;
			strMsg.Format("CVWComm: Timed out waiting for reply to (%d)\r\n", (int) chSynchSerial);
			OutputDebugString(strMsg);
#endif
			// timed out waiting for reply
			hr = VWCOMM_E_INTERNALTIMEOUT;

			// clean up
			pconn->m_pBufferReply->Release();
			pconn->m_pBufferReply = NULL;
			pconn->m_bInSendSynch = FALSE;

			// REVIEW: enqueue an error?

			break;
		}

		// if the socket is closed, that means we ran into an
		// error while waiting.  
		if (pconn->m_sock == INVALID_SOCKET || pconn->m_bReplyFailed)
		{
#ifdef PL_CHANNEL_DEBUG
			CString strMsg;
			strMsg.Format("CVWComm: Sync write aborted (%d)\r\n", (int) chSynchSerial);
			OutputDebugString(strMsg);
#endif
			hr = VWCOMM_E_INTERNALTIMEOUT;

			// clean up
			pconn->m_pBufferReply->Release();
			pconn->m_pBufferReply = NULL;
			pconn->m_bInSendSynch = FALSE;
								
			// REVIEW: enqueue an error?

			break;
		}

		// see if we have our reply
		if (pconn->m_bHaveReply)
		{
			// set the return reply (don't add ref since nuking ref, too)
			// Do this before the OnMessageAvailable below since that
			// might cause a reentrant call to this function.
			*ppcbReply = pconn->m_pBufferReply;
			pconn->m_pBufferReply = NULL;

			// process messages here again as some might have come
			// in ahead of the reply.

			//there should always be at least the replysync marker.
			ASSERT(m_pMessHead);
			if (m_pMessHead != NULL)
			{
				ASSERT(m_pCommSite);
		
				//process the msgs before the marker.
				m_pCommSite->OnMessageAvailable(this);
			}

			ASSERT(m_pMessHead);
			if(m_pMessHead)
			{
				hr = CommRemoveReplyMarker();
				ASSERT(SUCCEEDED(hr));				
			}

			//tell client to process msgs after the marker.
			if(m_pMessHead)
				m_pCommSite->OnMessageAvailable_T2(this);

#ifdef PL_CHANNEL_DEBUG
			CString strMsg;
			strMsg.Format("CVWComm: Got reply to (%d)\r\n", (int) chSynchSerial);
			OutputDebugString(strMsg);
#endif
			// reset
			pconn->m_bInSendSynch = FALSE;
			pconn->m_bHaveReply = FALSE;

			break;
		}

		// no reply, but messages to handle - loop back around
	}

	// free up the connection
	// REVIEW: what happens if it disappears here?
	pconn->Release();

	return hr;
}

HRESULT CVWCommObject::CommReply(CVWCommConnection *pconn, PWRITEBUFFER pvData, BYTE chSynchSerial )
{
	return CommWrite(pconn, pvData, chSynchSerial, typeReply);
}

HRESULT CVWCommObject::CommSend(WRITEBUFFER *pwb)
{
	HRESULT hr=S_OK;

	// iterate through the connections, sending and then
	// releasing as we go
	for (int iconn=0; iconn<pwb->cConnection; iconn++)
	{
		CVWCommConnection *pconn;

		// get the connection
		// REVIEW: Danger!  This is a down cast!
		if (pwb->cConnection==1)
			pconn = (CVWCommConnection *) pwb->pconn;
		else
			pconn = (CVWCommConnection *) pwb->rgpconn[iconn];

		if (pconn->m_sock!=INVALID_SOCKET)
			hr = CommSendToOne(pwb, pconn);

		// release the connection
		pconn->Release();
	}

	// done with all destinations

	// free the connection list
	if (pwb->rgpconn != NULL)
		delete [] pwb->rgpconn;

	// free the buffer itself
	delete [] pwb;

	return hr;
}

HRESULT CVWCommObject::CommSendToOne(WRITEBUFFER *pwb, CVWCommConnection *pconn)
{
	HRESULT hr;
	SOCKET sock;

	// get the data
	BYTE *pbData= (BYTE *) &pwb->comblk;
	DWORD cbData = (DWORD) (pwb->comblk.cchMessage + sizeof(COMBLK));

	// if we're trying to write to a CLIENT-SIDE primary HTTP connection, we
	// need to write to the associated control connection instead.
	if (pconn->Type()==CVWCommConnection::ePrimaryHTTP && pconn->ClientSide())
	{
		CString strHeader;
		CString strFormat =	"POST http://%s:%d/CONTROL/%d HTTP/1.0\r\n"
							"Connection: Keep-alive\r\n"
							"Content-type: application/octet-stream\r\n"
							"Content-length: %d\r\n"
							"Cache-control: no-cache\r\n"
							"\r\n";

		strHeader.Format(strFormat, 
							(LPCTSTR) pconn->m_strServer,
							pconn->m_iPort,
							pconn->m_dwCookie,
							cbData);

		// if we don't have a partner, establish it
		CVWCommConnection *pconnPartner = pconn->m_pconnPartner;
		if (pconnPartner==NULL)
		{
			// connects to partner, sending header, too
			hr = CommEstablishPartner(pconn);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			// Set up for subsequent writes to partner
			pconnPartner = pconn->m_pconnPartner;
			sock = pconnPartner->m_sock;
		}
		else
		{
			// write header (and set up for subsequent writes to partner)
			sock = pconnPartner->m_sock;

		}

		// if parter still waiting for a reply, wait a little while
		if (!pconnPartner->ReplyReceived())
		{
			DWORD timeStart=::GetTickCount();
			for(;;)
			{
				// break out if got reply or ran out of time
				if (pconnPartner->ReplyReceived() || (::GetTickCount()-timeStart)<cmsecWaitDefault)
					break;
				
				Sleep(100);
			}
		}

		// set flag to false since we're writing a new header
		pconnPartner->SetReplyReceived(FALSE);

		hr = CommWriteToSocket(sock, (BYTE *) (LPCTSTR) strHeader, strHeader.GetLength());
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// reset the "keep-alive" time so that we don't send any unnecessarily
		m_timeLastKeepAlive = ::GetCurrentTime();
	}
	else
	{
		// get the socket
		sock = pconn->m_sock;
	}

	// ignore writes to broken sockets
	if (sock==INVALID_SOCKET)
		return S_OK;

#ifdef PL_CHANNEL_DEBUG

	{
		DWORD tid;
		CString str;
		tid = GetCurrentThreadId();

		str.Format("CVWComm: START: Thread %08X writing to socket %d\r\n", tid, (int) sock);
		OutputDebugString(str);
	}

#endif

	// update stats
	CommUpdateStatistics(pconn, 0, cbData);

	hr = CommWriteToSocket(sock, pbData, cbData);

#ifdef PL_CHANNEL_DEBUG
	{
		CString strMsg;
		strMsg.Format("CVWComm: Finished writing to socket %d (%s)\r\n", (int) sock, SUCCEEDED(hr) ? "Ok" : "Failed");
		OutputDebugString(strMsg);
	}
#endif

ERROR_ENCOUNTERED:

	if (FAILED(hr))
	{
		// clean up
		CommHandleError(pconn, CVWCommMessage::statusWriteError, TRUE, TRUE);
	}

	return S_OK;
}


/////////////////////////////////////////////////////////////////////
// Thread functions

UINT CVWCommObject::CommAcceptThread(void *pv)
{
	// this is a static function, but we pass in the this as pv
	CVWCommObject *pthis = (CVWCommObject *) pv;

	HRESULT		hr;
	SOCKADDR_IN	sa;
	SOCKET 		sockNew;
	int			cbsa;

	// initialize OLE in case others need it
	if (FAILED(CoInitialize(NULL)))
	{
		ASSERT(FALSE);
		return 1;
	}

	for(;;)
	{
		// wait for someone to connect
		cbsa = sizeof(sa);
		sockNew = accept(pthis->m_sockBase, (PSOCKADDR) &sa, &cbsa);

		// if call fails, see if it's because we're terminating.  If not,
		// then it's an error, else ok
		if (sockNew == INVALID_SOCKET)
		{
			if( pthis->m_state != stateTerminating)
			{
				// enqueue an accept error message
				pthis->CommEnqueueMessage(CVWCommMessage::statusAcceptError, NULL); 
			}

			return sockNew==INVALID_SOCKET;
		}

		// got a new connection
		
		// create and initialize a connection object
		typedef CComObject<CVWCommConnection> CComCommConnection;

		CComCommConnection* pconn = new CComCommConnection();
		if (pconn == NULL)
		{
			// enqueue an error message
			pthis->CommEnqueueMessage(CVWCommMessage::statusOutOfMemory, NULL); 
			return 1;
		}

		// put it in the connection list (this bumps the ref count to 1)
		hr = pthis->CommAddConnection(pconn);
		ASSERT(SUCCEEDED(hr));

		// set stuff
		pconn->Initialize(pthis, sockNew, (PSOCKADDR) &sa, NULL, 0, FALSE);

		// start initial read on it - if this fails, it's cause there's too
		// much work going on (the read threads are too busy and none can take on
		// the new thread.)  What we do here is report and error and close the
		// connection.  This is important so that others don't try writing to
		// this socket (and then get blocked since there's no one around to read it).

#ifdef PL_CHANNEL_DEBUG
		CString strOut;
		strOut.Format("CVWComm: New connection 0x%08X\r\n", pconn);
		OutputDebugString(strOut);

#endif

		// start a write thread if necessary
		if (pthis->m_cthreadWrite < cThreadWriteMax)
		{
			int ithread = pthis->m_cthreadWrite++;

			ASSERT(pthis->m_rgpthreadWrite[ithread]==NULL);
			pthis->m_rgpthreadWrite[ithread] = AfxBeginThread( CommWriteThread, (void *) pthis);
			if (pthis->m_rgpthreadWrite[ithread]==NULL)
			{
				// clean up
				pthis->CommHandleError(pconn, VWCOMM_E_COULDNTINITIALIZE, FALSE, TRUE);
				return 1;
			}

			// specify that thread not disappear
			pthis->m_rgpthreadWrite[ithread]->m_bAutoDelete = FALSE;

		}

		hr = pthis->CommAssignReadThread(pconn);
		if (FAILED(hr))
		{
#ifdef PL_CHANNEL_DEBUG
			{
				CString str;
				str.Format("AcceptThread: AssignReadThread failed at time: %d\n", ::GetLastError(), ::GetTickCount() );
				OutputDebugString((LPCTSTR)str);
			}
#endif
			// clean up
			pthis->CommHandleError(pconn, VWCOMM_E_COULDNTINITIALIZE, FALSE, TRUE);
			return 1;
		}

	}

}

UINT CVWCommObject::CommW95ReadThread(void *pv)
{
	BYTE *pbRead;
	DWORD cbRead;
	DWORD cbTransferred;

	// peg the thread info
	CReadThreadInfo *pti = (CReadThreadInfo *) pv;

	// initialize OLE (in case needed later
	if (FAILED(CoInitialize(NULL)))
	{
		// REVIEW: report?
		ASSERT(FALSE);
		return 1;
	}

	// wait for start signal
	if (!pti->WaitForStart())
	{
		// REVIEW: report?
		ASSERT(FALSE);
		return 1;
	}

	// peg the connection
	int iOperation;

	CComPtr<CVWCommConnection> pconn;

	pti->GetOperation(iOperation, &pconn);
	ASSERT(iOperation==CReadThreadInfo::opAddRead);

	// check the connection
	if (pconn==NULL)
	{
		// REVIEW: report?
		ASSERT(FALSE);
		return 1;
	}

	// bump the connection count
	int cCount = pti->IncrementConnectionCount();
	ASSERT(cCount==1);

	// tell it we're done with the op
	pti->SignalDone(S_OK);

	// do reads until we fail
	for(;;)
	{
		pbRead = pconn->m_pbRead;
		cbRead = pconn->m_cbRead;

		cbTransferred = recv(pconn->m_sock, (char *) pbRead+cbRead, pconn->m_cbReadMax-cbRead, 0);

		// handle error
		if (cbTransferred==SOCKET_ERROR || cbTransferred==0)
		{
			// close the connection (cleanly)
			pconn->m_pcomm->CommCloseConn(pconn);

			// decrement the connection count
			cCount = pti->DecrementConnectionCount();
			ASSERT(cCount==0);

			// tell the read info that we're dead
			pti->ClearThread();

			// see if we're not disconnecting, we have an error!
			CComPtr<CVWCommConnection> pconnTemp;

			pti->GetOperation(iOperation, &pconnTemp);
			ASSERT(pconn==pconnTemp);
			if (iOperation!=CReadThreadInfo::opStopRead)
				pconn->m_pcomm->CommHandleError(pconn, CVWCommMessage::statusReadError, TRUE, FALSE);

			// if we're waiting for a message, unblock wait loop so that it can pass up notification of message available
			if (pconn->m_bInSendSynch)
				SetEvent(pconn->m_heventHaveData);

			// disappear
			return iOperation!=CReadThreadInfo::opStopRead;
		}

		ASSERT(pconn->m_cbRead + cbTransferred <= pconn->m_cbReadMax);

		// update read information
		pconn->m_cbRead += cbTransferred;

		// Process the information
		// NOTE: Don't handle errors here since already handled in CommHandleInput
		if (FAILED(pconn->m_pcomm->CommHandleInput(pconn)))
		{
			// terminate the thread
			AfxEndThread(1);
		}

	} // for(;;)
}


UINT CVWCommObject::CommNTReadThread(void *pv)
{
	DWORD dwRet;

	// peg the thread info
	CReadThreadInfo *pti = (CReadThreadInfo *) pv;

	// initialize OLE (in case needed later
	if (FAILED(CoInitialize(NULL)))
	{
		// REVIEW: report?
		ASSERT(FALSE);
		return 1;
	}

	for(;;)
	{
		// wait for something to do
		dwRet = pti->WaitForStartEx(INFINITE);

		// handle different situations
		switch(dwRet)
		{
			case WAIT_OBJECT_0:
			{
				// signalled - see what to do
				int iOperation;
				CComPtr<CVWCommConnection> pconn;
				pti->GetOperation(iOperation, &pconn);
				if (iOperation==CReadThreadInfo::opTerminate)
				{
					// signal that we're through
					pti->SignalDone(S_OK);

					// die
					return 0;
				}
				else if (iOperation==CReadThreadInfo::opAddRead)
				{
					// bump the count
					pti->IncrementConnectionCount();

					// signal that we're done
					// NOTE: we do this before starting the read op
					pti->SignalDone(S_OK);

					pconn->m_pcomm->CommReadAsync(pconn);
				}
				else if (iOperation==CReadThreadInfo::opStopRead)
				{
					// cancel any pending IO
					pconn->m_pcomm->CommCancelPendingIO(pconn);

					// give it a chance to finish
					dwRet = pti->WaitForStartEx(0);

					// decrement the count
					int cCount = pti->DecrementConnectionCount();
					ASSERT(cCount>=0);

					// signal that we're done
					pti->SignalDone(S_OK);
				}

				break;
			}

			case WAIT_IO_COMPLETION:
				// some IO just completed.  Nothing to do.
				break;

			default:
			{
				// error during wait
				// REVIEW: report this?
				ASSERT(FALSE);

				// die
				return 1;
			}

		}
	}
}


UINT CVWCommObject::CommWriteThread(void *pv)
{
	CVWCommObject *pthis = (CVWCommObject *) pv;
	BOOL bDie = FALSE;

	for (;;)
	{
		// wait for something to write
		if (!WaitForEvent(pthis->m_heventDataInWriteQueue, INFINITE))
			bDie = TRUE;

		// send out any pending data
		for (;;)
		{
			WRITEBUFFER *pwb;
			CVWCommConnection *pconnWrite = NULL;
			int iconnWrite = -1;

			// look through the buffers for a connection that
			// is not already in the middle of a write
			pthis->CommLockWriteQueue();

			// get the head of the list
			pwb = pthis->m_pwbHead;
			while (pwb!=NULL)
			{
				// look through its connection list for one that's
				// not in a write
				for (int iconn=0; iconn<pwb->cConnection; iconn++)
				{
					CVWCommConnection *pconn=NULL;

					// special case
					// REVIEW: Danger! Downcast.
					if (pwb->cConnection==1)
						pconn = (CVWCommConnection *) pwb->pconn;
					else
						pconn = (CVWCommConnection *) pwb->rgpconn[iconn];

					if (pconn!=NULL && !pconn->m_bInWrite)
					{
						// peg it
						iconnWrite = iconn;
						pconnWrite = pconn;

						// mark it as in-write
						pconnWrite->m_bInWrite = TRUE;
						break;
					}
				}

				// if found a connection to write to, get out
				if (pconnWrite != NULL)
					break;

				// try the next message
				pwb = pwb->pwbNext;

			}

			pthis->CommUnlockWriteQueue();

			// done?
			if (pwb==NULL || pconnWrite==NULL)
				break;

			pthis->CommSendToOne(pwb, pconnWrite);

			// lock the queue again to remove the connection
			pthis->CommLockWriteQueue();

#ifdef _DEBUG

			// make sure we're looking at the same slot
			if (iconnWrite==0 && pwb->cConnection==1)
				ASSERT((CVWCommConnection *) pwb->pconn==pconnWrite);
			else
				ASSERT((CVWCommConnection *) pwb->rgpconn[iconnWrite]==pconnWrite);
#endif

			// zap the slot
			if (iconnWrite==0 && pwb->cConnection==1)
				pwb->pconn = NULL;
			else
				pwb->rgpconn[iconnWrite] = NULL;

			// mark the connection as not in a write state
			pconnWrite->m_bInWrite = FALSE;

			// release the connection
			pconnWrite->Release();

			// reduce the live connection count for the buffer
			pwb->cConnectionLive--;

			// if zero, then don't need the buffer anymore
			if (pwb->cConnectionLive==0)
			{
				// dequeue it.  
				
				// Connect the previous one to the next
				if (pwb->pwbPrev==NULL)
				{
					// was first buffer in list
					pthis->m_pwbHead = pwb->pwbNext;
				}
				else
				{
					pwb->pwbPrev->pwbNext = pwb->pwbNext;
				}

				// now connect the next one to the previous one
				if (pwb->pwbNext==NULL)
				{
					// was last buffer in list
					pthis->m_pwbTail = pwb->pwbPrev;
				}
				else
				{
					pwb->pwbNext->pwbPrev = pwb->pwbPrev;
				}

				// update statistics
				pthis->CommUpdateQueueTimeStatistics(0, pwb->timeEnqueued);

				// free it

				// free the connection list
				if (pwb->rgpconn != NULL)
					delete [] pwb->rgpconn;

				// free the buffer itself
				delete [] pwb;

				// reduce the buffer count
				pthis->m_cBuffersInQueue--;

				// if we just hit the threshold, signal this to unblock
				// CommWriteExt (if waiting for write queue to ebb).
				if (pthis->m_cBuffersInQueue==cbufWriteQThreshold)
					SetEvent(pthis->m_heventWriteQueueBelowThreshold);

			}

			pthis->CommUnlockWriteQueue();

		}

		// time to die?
		if (bDie || pthis->m_state==stateTerminating)
		{
			// REVIEW: enqueue an error if not disconnecting
			return pthis->m_state!=stateTerminating;
		}
	}
}

HRESULT CVWCommObject::CommCancelPendingIO(CVWCommConnection *pconn)
{
	ASSERT(!m_bOnWindows95);

	// if the cancel pointer hasn't been fetched, do so now
	if (m_pfnCancel==NULL)
	{
		// get the kernerl
		if (m_hinstKernel==NULL)
		{
			m_hinstKernel = LoadLibrary("KERNEL32.DLL");
			if (m_hinstKernel==NULL)
				return E_FAIL;
		}

		m_pfnCancel = (PFNCANCEL) GetProcAddress(m_hinstKernel, "CancelIo");
		if (m_pfnCancel==NULL)
			return E_FAIL;
	}

	// call it
	(*m_pfnCancel)((HANDLE) pconn->m_sock);

	return S_OK;
}

BOOL CVWCommObject::CommIsWriteQueueEmpty()
{
	BOOL bEmpty;

	CommLockWriteQueue();
	bEmpty = m_pwbHead==NULL;
	CommUnlockWriteQueue();

	return bEmpty;
}

HRESULT CVWCommObject::CommWaitForEmptyWriteQueue()
{
	for (int itry=0; (itry<40) && !CommIsWriteQueueEmpty(); itry++)
		Sleep(250);

	return CommIsWriteQueueEmpty() ? S_OK : E_FAIL;
}

HRESULT CVWCommObject::CommKeepAlive()
{
	DWORD dwTimeNow = ::GetTickCount();

	if ((dwTimeNow-m_timeLastKeepAlive) > timeKeepAlivePeriod)
	{
		// if we're "client-side", send a keep-alive
		CommLockConnList();
		POSITION pos;
		for (pos = m_listConnection.GetHeadPosition(); pos != NULL;)
		{
			CVWCommConnection *pconn;

			pconn = m_listConnection.GetNext(pos);
			ASSERT(pconn);

			// send keep alives only on client-side primary connections (the
			// write will eventually go out on the controlHTTP connection)
			if (pconn->ClientSide())
			{
				int nType = pconn->Type();
				if (nType==CVWCommConnection::ePrimaryHTTP)
					VERIFY(SUCCEEDED(CommSendKeepAlive(pconn)));
			}
		}
		CommUnlockConnList();

		// reset time
		m_timeLastKeepAlive = dwTimeNow;
	}

	return S_OK;
}

HRESULT CVWCommObject::CommSendKeepAlive(CVWCommConnection *pconn)
{
	PWRITEBUFFER pwb;
	HRESULT hr;

	hr = CommAllocKeepAliveBuffer( &pwb);
	if (FAILED(hr))
		return hr;

	return CommWrite(pconn, pwb, 0, typeKeepAlive);
}

HRESULT CVWCommObject::CommGetHostName(BSTR *pbstrOut)
{
	HRESULT hr = S_OK;
	char szHostName[256];
	CComBSTR bstrHostName;

	ASSERT(pbstrOut);
	if (pbstrOut == NULL)
		return E_POINTER;

	*pbstrOut = NULL;

	if (gethostname(szHostName, sizeof(szHostName)) == 0)
	{
		bstrHostName = szHostName;
		*pbstrOut = bstrHostName.Copy();
	}
	else
		hr = VWCOMM_E_UNKNOWNERROR;
		
	return hr;
}

HRESULT CVWCommObject::CommAddConnection(CVWCommConnection *pconn)
{
	CommLockConnList();
	pconn->AddRef();
	m_listConnection.AddTail(pconn);
	CommUnlockConnList();

	return S_OK;
}

HRESULT CVWCommObject::CommRemoveConnection(CVWCommConnection *pconn)
{
	POSITION pos;

	CommLockConnList();
	pos = m_listConnection.Find(pconn);
	if (pos!=NULL)
	{
		m_listConnection.RemoveAt(pos);
		pconn->Release();
	}
	CommUnlockConnList();

	return (pos!=NULL) ? S_OK : E_FAIL;
}

HRESULT CVWCommObject::CommUpdateStatistics(CVWCommConnection *pconn, int cBytesReceived, int cBytesSent)
{
	DWORD timeNow = GetTickCount();
	DWORD timeGap = timeNow - pconn->m_timeSampleStart;

	// update some statistics

	if (cBytesReceived)
	{
		// connection
		pconn->m_cPacketsReceived++;
		pconn->m_cBytesReceived += cBytesReceived;

		// total (long-term and for running average)
		m_cPacketsReceivedTotal++;
		m_cPacketsReceived++;
		m_cBytesReceivedTotal += cBytesReceived;
		m_cBytesReceived += cBytesReceived;
	}

	if (cBytesSent)
	{
		// connection
		pconn->m_cPacketsSent++;
		pconn->m_cBytesSent += cBytesSent;

		// total (long-term and for running average)
		m_cPacketsSentTotal++;
		m_cPacketsSent++;
		m_cBytesSentTotal += cBytesSent;
		m_cBytesSent += cBytesSent;
	}

	// Calculate connection-based running averages
	if (timeGap > msecSampleWindow)
	{
		// calculate averages
		pconn->m_cPacketsReceivedPerSecond = (pconn->m_cPacketsReceived*1000) / timeGap;
		pconn->m_cBytesReceivedPerSecond = (pconn->m_cBytesReceived*1000) / timeGap;
		pconn->m_cPacketsSentPerSecond = (pconn->m_cPacketsSent*1000) / timeGap;
		pconn->m_cBytesSentPerSecond = (pconn->m_cBytesSent*1000) / timeGap;

		// test for evil users
		if ( pconn->m_cPacketsReceivedPerSecond > cMaxPacketsReceivedPerSecond	||
			 pconn->m_cBytesReceivedPerSecond > cMaxBytesReceivedPerSecond		||
			 pconn->m_cPacketsSentPerSecond > cMaxPacketsSentPerSecond			||
			 pconn->m_cBytesSentPerSecond > cMaxBytesSentPerSecond )
		{
			// too much data sent or received - inform the app
			OutputDebugString("VWCommObject::CommUpdateStatistics. Evil user.\n");

			// REVIEW: disable until we can get better stats
			//pconn->m_pconn->CommEnqueueMessage(CVWCommMessage::statusEvilUser, pconn);
		}

		// adjust moving average totals
		pconn->m_cPacketsReceived = (pconn->m_cPacketsReceivedPerSecond * msecSampleWindow) / 2000 ;
		pconn->m_cBytesReceived = (pconn->m_cBytesReceivedPerSecond * msecSampleWindow) / 2000 ;
		pconn->m_cPacketsSent = (pconn->m_cPacketsSentPerSecond * msecSampleWindow) / 2000 ;
		pconn->m_cBytesSent = (pconn->m_cBytesSentPerSecond * msecSampleWindow) / 2000 ;
		pconn->m_timeSampleStart = timeNow - msecSampleWindow/2;
	}

	// compile queue data
	m_cQueueSamples++;
	m_cSumPacketsInWriteQueue += m_cBuffersInQueue;
	m_cSumMessagesInReadQueue += m_cMessagesInQueue;

	// adjust moving averages for collective results
	timeGap = timeNow - m_timeSampleStart;
	if ( timeGap > msecSampleWindow )
	{
		// calculate averages
		m_cPacketsReceivedPerSecond = (m_cPacketsReceived*1000) / timeGap;
		m_cBytesReceivedPerSecond = (m_cBytesReceived*1000) / timeGap;
		m_cPacketsSentPerSecond = (m_cPacketsSent*1000) / timeGap;
		m_cBytesSentPerSecond = (m_cBytesSent*1000) / timeGap;

		if (m_cQueueSamples>0)
		{
			m_cMeanPacketsInWriteQueue = m_cSumPacketsInWriteQueue / m_cQueueSamples;
			m_cMeanMessagesInReadQueue = m_cSumMessagesInReadQueue / m_cQueueSamples;
		}
		if (m_cTimeInWriteQueueSamples>0)
			m_cMeanTimeInWriteQueue = m_cSumTimeInWriteQueue / m_cTimeInWriteQueueSamples;
		if (m_cTimeInReadQueueSamples>0)
			m_cMeanTimeInReadQueue = m_cSumTimeInReadQueue / m_cTimeInReadQueueSamples;

		// adjust moving average totals
		m_cPacketsReceived = (m_cPacketsReceivedPerSecond * msecSampleWindow) / 2000 ;
		m_cBytesReceived = (m_cBytesReceivedPerSecond * msecSampleWindow) / 2000 ;
		m_cPacketsSent = (m_cPacketsSentPerSecond * msecSampleWindow) / 2000 ;
		m_cBytesSent = (m_cBytesSentPerSecond * msecSampleWindow) / 2000 ;
		m_timeSampleStart = timeNow - msecSampleWindow/2;

		m_cSumPacketsInWriteQueue /= 2;
		m_cSumMessagesInReadQueue /= 2;
		m_cQueueSamples /= 2;

		m_cSumTimeInWriteQueue /= 2;
		m_cTimeInWriteQueueSamples /= 2;

		m_cSumTimeInReadQueue /= 2;
		m_cTimeInReadQueueSamples /= 2;
	}

	return S_OK;
}

HRESULT CVWCommObject::CommUpdateQueueTimeStatistics(DWORD timeRead, DWORD timeWrite)
{
	DWORD timeNow = GetTickCount();

	if (timeRead>0)
	{
		m_cTimeInReadQueueSamples++;
		m_cSumTimeInReadQueue += (timeNow - timeRead);
	}

	if (timeWrite>0)
	{
		m_cTimeInWriteQueueSamples++;
		m_cSumTimeInWriteQueue += (timeNow - timeWrite);
	}

	return S_OK;
}



HRESULT CVWCommObject::GetUsageStatistics(	int *pcPacketsSent,
											int *pcPacketsReceived,
											int *pcBytesSent,
											int *pcBytesReceived,		
											int *pcPacketsReceivedPerSecond,
											int *pcBytesReceivedPerSecond,
											int *pcPacketsSentPerSecond,
											int *pcBytesSentPerSecond,
											int *pcMeanPacketsInWriteQueue,
											int *pcMeanMessagesInReadQueue,
											int *pcMeanTimeInWriteQueue,
											int *pcMeanTimeInReadQueue )
{
	if (pcPacketsSent!=NULL)
		*pcPacketsSent = m_cPacketsSentTotal;

	if (pcPacketsReceived!=NULL)
		*pcPacketsReceived = m_cPacketsReceivedTotal;

	if (pcBytesSent!=NULL)
		*pcBytesSent = m_cBytesSentTotal;

	if (pcBytesReceived!=NULL)
		*pcBytesReceived = m_cBytesReceivedTotal;

	if (pcPacketsReceivedPerSecond != NULL)
		*pcPacketsReceivedPerSecond = m_cPacketsReceivedPerSecond;

	if (pcBytesReceivedPerSecond != NULL)
		*pcBytesReceivedPerSecond = m_cBytesReceivedPerSecond;

	if (pcPacketsSentPerSecond != NULL)
		*pcPacketsSentPerSecond = m_cPacketsSentPerSecond;

	if (pcBytesSentPerSecond != NULL)
		*pcBytesSentPerSecond = m_cBytesSentPerSecond;

	if (pcMeanPacketsInWriteQueue != NULL)
		*pcMeanPacketsInWriteQueue = m_cMeanPacketsInWriteQueue;

	if (pcMeanMessagesInReadQueue != NULL)
		*pcMeanMessagesInReadQueue = m_cMeanMessagesInReadQueue;

	if (pcMeanTimeInWriteQueue != NULL)
		*pcMeanTimeInWriteQueue = m_cMeanTimeInWriteQueue;

	if (pcMeanTimeInReadQueue != NULL)
		*pcMeanTimeInReadQueue = m_cMeanTimeInReadQueue;

	return S_OK;
}

HRESULT CVWCommObject::CommCloseConn(CVWCommConnection *pconn)
{
	SOCKET sock = pconn->m_sock;

	if (sock != INVALID_SOCKET)
	{
		// mark socket as to be closed
		pconn->m_sock = INVALID_SOCKET;

		// do a shutdown first
		shutdown(sock, 1);

		// read any pending data 
		for (;;)
		{
			// see if any data to read
			const struct timeval timeout = {0, 0};
			fd_set fdRead;

			FD_ZERO(&fdRead);
			FD_SET(sock, &fdRead);
			if (select( 0, &fdRead, NULL, NULL, &timeout)==0)
				break;

			ASSERT(FD_ISSET(sock, &fdRead));

			// read it (to flush)
			DWORD cb;
			char buf[4096];

			cb = recv(sock, buf, sizeof(buf), 0);

			if (cb==0 || cb==SOCKET_ERROR)
				break;
		}

		// now close the socket
		closesocket(sock);
	}
	
	return S_OK;
}

HRESULT CVWCommObject::CommHandleError(CVWCommConnection *pconn, int nErrorCode, BOOL bEnqueue, BOOL bClose)
{
	int nType = pconn->Type();

	ASSERT(pconn);

#ifdef PL_CHANNEL_DEBUG
	CString str;
	str.Format("CVWComm: Error (0x%08X) on connection 0x%08x\r\n", nErrorCode, (void *) pconn);
	OutputDebugString(str);
	Log(str);
#endif

	// peg (to not disappear during disconnect)
	pconn->AddRef();

	// if requested, disconnect the connection 
	if (bClose)
		CommDisconnect(pconn, TRUE);

	// if requested, enqueue an error message
	if (bEnqueue)
	{
		if (nType==CVWCommConnection::eNative || nType==CVWCommConnection::ePrimaryHTTP)
			CommEnqueueMessage(nErrorCode, pconn);
		else
		{
			// try to report through parter, if present
			if (pconn->m_pconnPartner!=NULL)
				CommEnqueueMessage(nErrorCode, pconn->m_pconnPartner);
		}
	}

	// unpeg
	pconn->Release();

	return S_OK;
}

HRESULT CVWCommObject::CommHandleInput(CVWCommConnection *pconn)
{
	if (pconn->ClientSide())
		return CommHandleInput_Client(pconn);
	else
		return CommHandleInput_Server(pconn);
}

HRESULT CVWCommObject::CommHandleInput_Server(CVWCommConnection *pconn)
{
	HRESULT hr;

	// There are several cases to deal with depending on whether we're on 
	// the client or server and depending on the connection type.
	//
	// Server:
	//	eUnknown - parse request header
	//  eNative - parse binary input
	//  eControlHTTP - parse ascii request or binary input (depending on flag)
	//  ePrimaryHTTP - error.  
	//

	for(;pconn->m_cbRead>0;)
	{
		CVWCommConnection::ConnectionType ct=pconn->Type();
		BOOL bAsciiMode = pconn->AsciiMode();
		DWORD cbPacket;

		if (!bAsciiMode)
		{
			// verify we don't have bogus connection types
			if (ct==CVWCommConnection::eUnknown || ct==CVWCommConnection::ePrimaryHTTP)
			{
#ifdef PL_CHANNEL_DEBUG
				CString strMsg;
				strMsg.Format("CVWComm: unexpected connection (0x%08X) of type %d in binary mode\r\n", pconn, (int) ct);
				OutputDebugString(strMsg);
#endif
				ASSERT(FALSE);
				return E_FAIL;
			}

			// if we haven't at least read a header, exit so that we can read more
			if (pconn->m_cbRead < sizeof(COMBLK))
				break;

			// analyze the binary header
			COMBLK *pcb = (COMBLK *) pconn->m_pbRead;

			// check the message type (disallow replies since server never does
			// synch messages to client)
			BYTE chType = pcb->chType;
			if (chType != typeAsync		&&
				chType != typeSynch		&&
				chType != typeKeepAlive )
			{
				ASSERT(FALSE);

				// inform the app
				CommEnqueueMessage(CVWCommMessage::statusEvilUser, pconn);

				// REVIEW: should we keep parsing?
				return E_FAIL;
			}

			// pluck out the message size
			cbPacket = pcb->cchMessage + sizeof(COMBLK);

			// if we haven't read a whole packet...
			if (pconn->m_cbRead < cbPacket)
			{
				// make sure we've got enough room in the read buffer to 
				// receive the entire message
				if (pconn->m_cbReadMax < cbPacket)
				{
					// nope, need to expand

					// first, allocate a new buffer
					BYTE *pbNew = new BYTE[cbPacket];
					if (pbNew==NULL)
						return E_FAIL;

					// copy the data
					memcpy(pbNew, pconn->m_pbRead, pconn->m_cbRead);

					// free the old stuff
					delete [] pconn->m_pbRead;

					// switch to new
					pconn->m_pbRead = pbNew;
					pconn->m_cbReadMax = cbPacket;
				}

				// break out of the for() to get more data
				break;
			}

			// if this is came in on a controlHTTP connection, we have to
			// send a reply to it and to put it back in ascii mode to prepare 
			// for the next head.  Do this (esp. the reply) _before_ processing
			// the message (since it might take a long time to process)
			if (ct==CVWCommConnection::eControlHTTP)
			{
				pconn->SetAsciiMode(TRUE);

				// send back a reply
				// NOTE: send back at least one data "byte" to 
				// avoid proxy problems.
				CString strReply = 
							"HTTP/1.1 200 OK\r\n"
							//"Cache-Control: no-cache\r\n"
							"Connection: Keep-Alive\r\n"
							"Content-type: text/html\r\n"
							"Content-length: 0\r\n"
							"\r\n";

				CommWriteToSocket(pconn->m_sock, (BYTE *) (LPCTSTR) strReply, strReply.GetLength());
			}

			// REVIEW: this counts keep alive messages
			CommUpdateStatistics(pconn, pcb->cchMessage, 0);

#ifdef PL_CHANNEL_DEBUG

			CString strMsg;
			char *pszType;
			if (pcb->chType==typeAsync)
				pszType = "asynchronous";
			else if (pcb->chType==typeSynch)
				pszType = "synchronous";
			else if (pcb->chType==typeKeepAlive)
				pszType = "keep-alive";
			else
				pszType = "*BAD*";
			char *pszCType;
			if (ct==CVWCommConnection::eNative)
				pszCType = "native";
			else if (ct==CVWCommConnection::eControlHTTP)
				pszCType = "control";
			else
				pszCType = "*BAD*";

			strMsg.Format("CVWComm: Got %s type binary packet (logid=%d) on %s-type connection 0x%08X\r\n", pszType, iLogId++, pszCType, pconn);
			OutputDebugString(strMsg);
			LogData(strMsg, pcb->rgData, pcb->cchMessage);

#endif
			//  we've have a whole packet, process it
			if (pcb->chType==typeKeepAlive)
			{
				// send  a keep-alive back to the client. If this
				// is a control connection, send back through
				// the primary
				if (ct==CVWCommConnection::eControlHTTP)
				{
					ASSERT(pconn->m_pconnPartner!=NULL);
					VERIFY(SUCCEEDED(CommSendKeepAlive(pconn->m_pconnPartner)));
				}
				else
				{
					// shouldn't be getting keep-alives on primary conns
					ASSERT(NULL);
				}

			}
			else
			{
				// On the server, binary data should only be received in Native or 
				// Control connections.  If received in a control connection, pass it
				// on to the primary connection.  
				if (ct==CVWCommConnection::eNative)
				{
					// enqueue a data message
					CommEnqueueMessage(CVWCommMessage::statusOk, pconn, pcb->rgData, pcb->cchMessage, pcb->chSerial);

					// if we're waiting for a message, unblock wait loop so that it can pass up notification of message available
					if (pconn->m_bInSendSynch)
						SetEvent(pconn->m_heventHaveData);
				}
				else if (ct==CVWCommConnection::eControlHTTP)
				{
					CVWCommConnection *pconnPrimary;

					// peg the partner
					pconnPrimary = pconn->m_pconnPartner;
					ASSERT(pconnPrimary!=NULL);

					// pass data up as if partner (primary)
					// enqueue a data message
					CommEnqueueMessage(CVWCommMessage::statusOk, pconnPrimary, pcb->rgData, pcb->cchMessage, pcb->chSerial);

					// if we're waiting for a message, unblock wait loop so that it can pass up notification of message available
					if (pconnPrimary->m_bInSendSynch)
						SetEvent(pconnPrimary->m_heventHaveData);

				}

			}

			// REVIEW: resize big buffers here?
		}
		else
		{
			// ASCII Mode

			// see if we have a blank line (\r\n\r\n) in the message

			// append a null so we can use strstr
			// NOTE: this will never overrun the buffer as long as the header is 
			// less than the max buffer size (8K).

			pconn->m_pbRead[pconn->m_cbRead] = (BYTE) '\0';

			// if we don't have the end of header delimeter, break out of for() and read more
			char *pszDelim = strstr((const char *) pconn->m_pbRead, "\r\n\r\n");
			if (pszDelim==NULL)
				break;

			// have a header - process it
			*pszDelim = '\0';
			cbPacket = pszDelim + 4 - (char *) pconn->m_pbRead;

#ifdef PL_CHANNEL_DEBUG
			CString strMsg;
			strMsg.Format("CVWComm: Ascii message (logid=%d) on connection 0x%08X: %s\r\n", iLogId++, pconn, pconn->m_pbRead);
			OutputDebugString(strMsg);
			Log(strMsg);
#endif

			// Process the header.  The function will send replies as needed
			hr = CommHandleHeader(pconn);
			if (FAILED(hr))
			{
				// cleanup 
				CommHandleError(pconn, CVWCommMessage::statusReadError, TRUE, TRUE);
				return hr;
			}
		}

		// slide the data up and adjust
		memcpy(pconn->m_pbRead, pconn->m_pbRead+cbPacket, pconn->m_cbRead-cbPacket);
		pconn->m_cbRead -= cbPacket;

	}

	return S_OK;
}

HRESULT CVWCommObject::CommHandleInput_Client(CVWCommConnection *pconn)
{
	HRESULT hr;

	// There are several cases to deal with depending on whether we're on 
	// the client or server and depending on the connection type.
	//
	// Client:
	//  eUnknown - parse reply header
	//  eNative - parse binary input
	//  eControlHTTP - parse reply header
	//  ePrimaryHTTP - parse binary input

	for(;pconn->m_cbRead>0;)
	{
		CVWCommConnection::ConnectionType ct=pconn->Type();
		BOOL bAsciiMode = pconn->AsciiMode();
		DWORD cbPacket;

		if (!bAsciiMode)
		{
			// verify we don't have bogus connection types
			if (ct==CVWCommConnection::eUnknown || ct==CVWCommConnection::eControlHTTP)
			{
#ifdef PL_CHANNEL_DEBUG
				CString strMsg;
				strMsg.Format("CVWComm: unexpected connection (0x%08X) of type %d in binary mode\r\n", pconn, (int) ct);
				OutputDebugString(strMsg);
#endif
				ASSERT(FALSE);
				return E_FAIL;
			}

			// if we haven't at least read a header, exit so that we can read more
			if (pconn->m_cbRead < sizeof(COMBLK))
				break;

			// analyze the binary header
			COMBLK *pcb = (COMBLK *) pconn->m_pbRead;

			// check the message types.  Disallow synch messages since server
			// shouldn't be sending them
			BYTE chType = pcb->chType;
			if (chType != typeAsync		&&
				chType != typeReply		&&
				chType != typeKeepAlive )
			{
				ASSERT(FALSE);

				// inform the app
				CommEnqueueMessage(CVWCommMessage::statusEvilUser, pconn);

				// REVIEW: should we keep parsing?
				return E_FAIL;
			}

			// pluck out the message size
			cbPacket = pcb->cchMessage + sizeof(COMBLK);

			// if we haven't read a whole packet...
			if (pconn->m_cbRead < cbPacket)
			{
				// make sure we've got enough room in the read buffer to 
				// receive the entire message
				if (pconn->m_cbReadMax < cbPacket)
				{
					// nope, need to expand

					// first, allocate a new buffer
					BYTE *pbNew = new BYTE[cbPacket];
					if (pbNew==NULL)
						return E_FAIL;

					// copy the data
					memcpy(pbNew, pconn->m_pbRead, pconn->m_cbRead);

					// free the old stuff
					delete [] pconn->m_pbRead;

					// switch to new
					pconn->m_pbRead = pbNew;
					pconn->m_cbReadMax = cbPacket;
				}

				// break out of the for() to get more data
				break;
			}

			// REVIEW: this counts keep alive messages, but what the heck
			CommUpdateStatistics(pconn, pcb->cchMessage, 0);

#ifdef PL_CHANNEL_DEBUG

			CString strMsg;
			char *pszType;
			if (pcb->chType==typeAsync)
				pszType = "asynchronous";
			else if (pcb->chType==typeReply)
				pszType = "reply";
			else if (pcb->chType==typeKeepAlive)
				pszType = "keep-alive";
			else
				pszType = "*BAD*";
			char *pszCType;
			if (ct==CVWCommConnection::eNative)
				pszCType = "native";
			else if (ct==CVWCommConnection::ePrimaryHTTP)
				pszCType = "primary";
			else
				pszCType = "*BAD*";

			strMsg.Format("CVWComm: Got %s type binary packet (logid=%d) on %s-type connection 0x%08X\r\n", pszType, iLogId++, pszCType, pconn);
			OutputDebugString(strMsg);
			LogData(strMsg, pcb->rgData, pcb->cchMessage);

#endif
			//  we've have a whole packet, process it
			if (pcb->chType==typeKeepAlive)
			{
				// nothing - just swallow it
			}
			else if (pcb->chType==typeReply)
			{
				if (!pconn->m_bInSendSynch ||
					pconn->m_chSynchSerialReply!=pcb->chSerial ||
					pconn->m_pBufferReply==NULL)
				{
					// not waiting or not waiting for appropriate message
					// enqueue an error
					CommEnqueueMessage(CVWCommMessage::statusEvilUser, pconn);

					// REVIEW: should we continue?
					return E_FAIL;
				}
				else
				{
					// ok, stuff the reply
					hr = pconn->m_pBufferReply->CopyTo(pcb->rgData, pcb->cchMessage);
					if (FAILED(hr))
						return hr;

					CommEnqueueMessage(CVWCommMessage::statusHaveResponse, pconn);

					// set the event to indicate that the reply was received
					pconn->m_bHaveReply = TRUE;
					SetEvent(pconn->m_heventHaveData);
				}
			}
			else
			{
				// Enqueue data message
				CommEnqueueMessage(CVWCommMessage::statusOk, pconn, pcb->rgData, pcb->cchMessage, pcb->chSerial);

				// if we're waiting for a message, unblock wait loop so that it can pass up notification of message available
				if (pconn->m_bInSendSynch)
					SetEvent(pconn->m_heventHaveData);

			}

			// REVIEW: resize big buffers here?
		}
		else
		{
			// ASCII Mode

			// see if we have a blank line (\r\n\r\n) in the message

			// append a null so we can use strstr
			// NOTE: this will never overrun the buffer as long as the header is 
			// less than the max buffer size (8K).

			pconn->m_pbRead[pconn->m_cbRead] = (BYTE) '\0';

			// if we don't have the end of reply delimeter, break out of for() and read more
			char *pszDelim = strstr((const char *) pconn->m_pbRead, "\r\n\r\n");
			if (pszDelim==NULL)
				break;

			// have a reply - process it
			*pszDelim = '\0';
			cbPacket = pszDelim + 4 - (char *) pconn->m_pbRead;

#ifdef PL_CHANNEL_DEBUG
			CString strMsg;
			strMsg.Format("CVWComm: Ascii reply (logid=%d) on connection 0x%08X: %s\r\n", iLogId++, pconn, pconn->m_pbRead);
			OutputDebugString(strMsg);
			Log(strMsg);
#endif

			// Process the reply.
			hr = CommHandleReply(pconn);
			if (FAILED(hr))
			{
				// cleanup 
				CommHandleError(pconn, CVWCommMessage::statusReadError, TRUE, TRUE);
				return hr;
			}
		}

		// slide the data up and adjust
		memcpy(pconn->m_pbRead, pconn->m_pbRead+cbPacket, pconn->m_cbRead-cbPacket);
		pconn->m_cbRead -= cbPacket;

	}

	return S_OK;
	
}

char *CVWCommObject::CommGetLine(char **ppsz)
{
	ASSERT(ppsz);
	char *pszStart=*ppsz;

	// if we're at a null, return null (don't advance)
	if (*pszStart=='\0')
		return NULL;

	// skip leading whitespace
	while (*pszStart==' ')
		pszStart++;

	// look for eol or eos
	char *pszEnd = pszStart;
	for(;;)
	{
		// see if found eol
		if (*pszEnd=='\r')
		{
			// mark and advance
			*pszEnd++ = '\0';

			// skip \n, too
			if (*pszEnd=='\n')
				pszEnd++;

			// update and return
			*ppsz = pszEnd;
			return pszStart;
		}

		// see if at eos
		if (*pszEnd=='\0')
		{
			// yep - update and return
			*ppsz = pszEnd;
			return pszStart;
		}

		// advance
		pszEnd++;
	}
	
}

HRESULT CVWCommObject::CommHandleReply(CVWCommConnection *pconn)
{
	char *pszData = (char *) pconn->m_pbRead;
	char *pszLine;
	HRESULT hr = S_OK;

	// process the status line first
	pszLine = CommGetLine(&pszData);
	if (pszLine==NULL)
		return E_FAIL;

	// first token must be "HTTP/1.x"
	if (strstr(pszLine, "HTTP/1.")!=pszLine)
		return E_FAIL;

	// assure that status code is 200
	pszLine += 9;
	if (strstr(pszLine, "200")!=pszLine)
	{
#ifdef PL_CHANNEL_DEBUG
		CString strError;
		strError.Format("CVWComm: Received error reply: %s\r\n", pszLine);
		OutputDebugString(strError);
#endif
		return E_FAIL;
	}

	// set reply flag
	pconn->SetReplyReceived(TRUE);

	// read additional lines, looking for Etag
	// REVIEW: parse for Connection: Keep-alive?
	while (pszLine=CommGetLine(&pszData))
	{
		// see if "Etag"
		if (strstr(pszLine, "ETag: \"")==pszLine)
		{
			// yep, parse out cookie

			// verify we've got an unknown connection type (this header
			// should only come by once)
			if (pconn->Type()!=CVWCommConnection::eUnknown)
			{
				ASSERT(FALSE);
			}

			int nCookie;
			pszLine += 7;
			nCookie = atoi(pszLine);

			// set connection type and cookie (puts it in binary mode)
			pconn->SetConnectionType(CVWCommConnection::ePrimaryHTTP);
			pconn->m_dwCookie = (DWORD) nCookie;

#ifdef PL_CHANNEL_DEBUG
			CString strMsg;
			strMsg.Format("CVWConn: Set connection 0x%08X type to 'primary'\r\n", pconn);
			OutputDebugString(strMsg);
#endif

		}
	}
	return S_OK;
}

HRESULT CVWCommObject::CommHandleHeader(CVWCommConnection *pconn)
{
	char *pszData = (char *) pconn->m_pbRead;
	char *psz;
	HRESULT hr = S_OK;

	ASSERT(!pconn->ClientSide());

	// must be unknown or control connection type at this point
	if (pconn->Type()!=CVWCommConnection::eUnknown && pconn->Type()!=CVWCommConnection::eControlHTTP)
	{
#ifdef PL_CHANNEL_DEBUG
		OutputDebugString("CVWComm: Unexpected connection type when parsing header\r\n");
#endif
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	// must be in ascii mode
	if (!pconn->AsciiMode())
	{
#ifdef PL_CHANNEL_DEBUG
		OutputDebugString("CVWComm: Unexpected connection mode when parsing header\r\n");
#endif
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	// process the request line first
	psz = strtok(pszData, " ");
	if (psz==NULL)
	{
#ifdef PL_CHANNEL_DEBUG
		OutputDebugString("CVWComm: No method token\r\n");
#endif
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	// first token (method) must be "GET" or "POST"
	if (_stricmp(psz, "GET")!=0 && _stricmp(psz, "POST")!=0)
	{
#ifdef PL_CHANNEL_DEBUG
		OutputDebugString("CVWComm: Invalid method\r\n");
#endif
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	// get the request-URI
	psz = strtok(NULL, " ");
	if (psz==NULL)
	{
#ifdef PL_CHANNEL_DEBUG
		OutputDebugString("CVWComm: Invalid URI\r\n");
#endif
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	// up case
	_strupr(psz);

	// if URI begins with HTTP://, strip that and the server name off
	if (strstr(psz, "HTTP://")==psz)
	{
		// advance past the protocol
		psz += 7;

		// strip off server
		psz = strchr(psz, '/');
		if (psz==NULL)
		{
#ifdef PL_CHANNEL_DEBUG
		OutputDebugString("CVWComm: Invalid server name\r\n");
#endif
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
	}
	else
	{
		// verify we're at the root
		if (*psz != '/')
		{
#ifdef PL_CHANNEL_DEBUG
		OutputDebugString("CVWComm: Invalid URL\r\n");
#endif
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
	}

	// advance past /
	psz++;
	if (*psz=='\0')
	{
#ifdef PL_CHANNEL_DEBUG
		OutputDebugString("CVWComm: Missing doc name in URL\r\n");
#endif
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	// pluck off the connection type
	if (strstr(psz, "NATIVE")==psz)
	{
		// set connection type
		pconn->SetConnectionType(CVWCommConnection::eNative);

		// enqueue a message informing of the connection
		CommEnqueueMessage(CVWCommMessage::statusConnect, pconn);

#ifdef PL_CHANNEL_DEBUG
		CString strOut;
		strOut.Format("CVWComm: Connection 0x%08X is native mode\r\n", pconn);
		OutputDebugString(strOut);
#endif

	}
	else if (strstr(psz, "PRIMARY")==psz)
	{
		// set connection type (puts it in binary mode, too)
		pconn->SetConnectionType(CVWCommConnection::ePrimaryHTTP);

		// set and advance the cookie
		pconn->m_dwCookie = m_dwCookieNext++;

		// enqueue a message informing of the connection
		CommEnqueueMessage(CVWCommMessage::statusConnect, pconn);

#ifdef PL_CHANNEL_DEBUG
		CString strOut;
		strOut.Format("CVWComm: Connection 0x%08X is primary HTTP mode (cookie=%d)\r\n", pconn, pconn->m_dwCookie);
		OutputDebugString(strOut);
#endif
	}
	else if (strstr(psz, "CONTROL/")==psz)
	{
		// get the cookie value (advance past "CONTROL/")
		psz += 8;
		if (*psz=='\0')
		{
#ifdef PL_CHANNEL_DEBUG
		OutputDebugString("CVWComm: Missing cooking in CONTROL URL\r\n");
#endif
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}

#ifndef _DEBUG
		// lookup partner if we don't already have one
		if (pconn->m_pconnPartner==NULL)
		{
#endif
			DWORD dwCookie = (DWORD) atol(psz);
			CVWCommConnection *pconnPartner;

			// look for the matching connection and set the partner pointeres
			if (FAILED(hr=CommFindConnection(dwCookie, &pconnPartner)))
			{
#ifdef PL_CHANNEL_DEBUG
				OutputDebugString("CVWComm: Could not find partner connection\r\n");
#endif
				goto ERROR_ENCOUNTERED;
			}

#ifdef PL_CHANNEL_DEBUG
			CString strOut;
			strOut.Format("CVWComm: Connection 0x%08X is control HTTP mode (cookie=%d)\r\n", pconn, dwCookie);
			OutputDebugString(strOut);
#endif
			// if already have partner, verify it's the same one
			if (pconn->m_pconnPartner!=NULL)
			{
				// verify type
				if (pconn->m_pconnPartner!=pconnPartner || pconn->Type()!=CVWCommConnection::eControlHTTP)
				{
#ifdef PL_CHANNEL_DEBUG
					OutputDebugString("CVWComm: Messed up partner connection\r\n");
#endif
					SAFERELEASE(pconnPartner);
					hr = E_FAIL;
					goto ERROR_ENCOUNTERED;
				}
			}
			else
			{
				// set up symmetric pointers
				pconn->m_pconnPartner = pconnPartner;
				pconnPartner->AddRef();

				// on server (the only place this code should be running!), we 
				// don't connect the primary back to the control (since we 
				// never need to write through the control.		  
				// REVIEW: Is there other code that assumes this? 
			} 
			SAFERELEASE(pconnPartner);

			// set connection type (puts it in binary mode)
			pconn->SetConnectionType(CVWCommConnection::eControlHTTP);

#ifndef _DEBUG
		}
#endif

	}
	else
	{
#ifdef PL_CHANNEL_DEBUG
		OutputDebugString("CVWComm: Invalid doc in URL\r\n");
#endif
		hr = E_FAIL;
	}
	
	// ignore everything else 
	// REVIEW: we should parse the rest of the request header.  Perhaps
	// assure that "Connection: keep-alive" is set					   

ERROR_ENCOUNTERED:

	// send back reply header (except for native connections)
	if (pconn->Type()==CVWCommConnection::ePrimaryHTTP)
	{
		CString strReply;
		if (SUCCEEDED(hr))
		{
			// send back cookie in entity tag

			// Note that no length is provided (infinite reply)
			strReply.Format(	"HTTP/1.1 200 OK\r\n"
								"Connection: Keep-Alive\r\n"
								"Cache-Control: no-cache\r\n"
								"Content-type: application/octet-stream\r\n"
								"ETag: \"%d\"\r\n"
								"\r\n",
								pconn->m_dwCookie);

			// send the reply
			hr = CommWriteToSocket(pconn->m_sock, (BYTE *) (LPCTSTR) strReply, strReply.GetLength());
		}
		else
		{
			strReply =	"HTTP/1.1 400 Bad Request\r\n"
						"\r\n";

			// send the reply(ignore error code)
			CommWriteToSocket(pconn->m_sock, (BYTE *) (LPCTSTR) strReply, strReply.GetLength());
		}

	}

	return hr;
}

HRESULT CVWCommObject::CommFindConnection(DWORD dwCookie, CVWCommConnection **ppconn)
{
	POSITION pos;

	if (ppconn==NULL)
		return E_POINTER;

	*ppconn = NULL;
	pos = m_listConnection.GetHeadPosition();
	for (;pos!=NULL;)
	{
		CVWCommConnection *pconn;

		pconn = m_listConnection.GetNext(pos);
		ASSERT(pconn!=NULL);

		// check if it's the one we want
		if (pconn->m_dwCookie==dwCookie)
		{
			// yep
			*ppconn = pconn;
			pconn->AddRef();

			return S_OK;
		}
	}

	return E_FAIL;
}

HRESULT CVWCommObject::CommWriteToSocket(SOCKET sock, BYTE *pbData, DWORD cbData)
{
#ifdef PL_CHANNEL_DEBUG

	CString strMsg;
	strMsg.Format("CVWComm: Writing %d bytes (logid=%d) to socket %d\r\n", cbData, iLogId++, (int) sock);
	OutputDebugString(strMsg);
	LogData(strMsg, pbData, cbData);

#endif

	if (sock==INVALID_SOCKET)
	{
#ifdef PL_CHANNEL_DEBUG
		OutputDebugString("CVWComm: Writing to invalid socket!\r\n");
		return E_FAIL;
#endif
	}

	// send it
	while (cbData>0)
	{
		// write out in pieces, if large
		int cbWrite;
		cbWrite = min(cbData, cbWriteMax);

		int cbSent = send(sock, (char *) pbData, cbWrite, 0);
		if (cbSent==SOCKET_ERROR)
		{
#ifdef PL_CHANNEL_DEBUG
			CString strMsg;
			strMsg.Format("CVWComm: Writing to socket %d FAILED!\r\n", cbData, (int) sock);
			OutputDebugString(strMsg);
#endif
			return E_FAIL;
		}

		// advance
		pbData += cbSent;
		cbData -= cbSent;
	}

	return S_OK;
}

// NOTE: this code should only be running on the client
//
HRESULT CVWCommObject::CommEstablishPartner(CVWCommConnection *pconn)
{
	HRESULT		hr;
	SOCKET		sockNew;
	BOOL		bTrue = TRUE;

	// create unbound socket
	sockNew = socket( AF_INET, SOCK_STREAM, 0);
	if (sockNew == INVALID_SOCKET )
		return VWCOMM_E_WINSOCKERROR;

#ifdef NONAGLYALGORITHM
	if (setsockopt( sockNew, IPPROTO_TCP, TCP_NODELAY, (const char *) &bTrue, sizeof(BOOL)) != 0)
		return VWCOMM_E_WINSOCKERROR;
#endif

#ifdef NOLINGER
	LINGER linger;

	linger.l_onoff = 1;
	linger.l_linger = 0;
	if (setsockopt(sockNew, SOL_SOCKET, SO_LINGER, (const char *) &linger, sizeof(linger) )!= 0)
		return VWCOMM_E_WINSOCKERROR;
#endif

#ifdef KEEPALIVE
	if (setsockopt(sockNew, SOL_SOCKET, SO_KEEPALIVE, (const char *) &bTrue, sizeof(BOOL)) != 0)
		return VWCOMM_E_WINSOCKERROR;
#endif

	// connect to server
	if (connect(sockNew, (PSOCKADDR) &pconn->m_sockaddr, sizeof(pconn->m_sockaddr)) < 0)
	{
		closesocket(sockNew);
		return VWCOMM_E_WINSOCKERROR;
	}

	// create and initialize a connection object
	typedef CComObject<CVWCommConnection> CComCommConnection;

	CComCommConnection* pconnPartner = new CComCommConnection();
	if (pconnPartner == NULL)
	{
		closesocket(sockNew);
		return E_OUTOFMEMORY;
	}

	// put it in the connection list (this bumps the ref count to 1)
	hr = CommAddConnection(pconnPartner);
	ASSERT(SUCCEEDED(hr));

	// set stuff and prepare request header.
	// force connection to ascii mode (awaiting reply to header)
	pconnPartner->Initialize(this, sockNew, (PSOCKADDR) &pconn->m_sockaddr, pconn->m_strServer, pconn->m_iPort, TRUE);
	pconnPartner->SetConnectionType(CVWCommConnection::eControlHTTP);
	pconnPartner->SetAsciiMode(TRUE);

#ifdef STARTREADTHREADSFORCONTROLCONNECTIONS

	// start a write thread 
	if (m_cthreadWrite < cThreadWriteMax)
	{
		int ithread = m_cthreadWrite++;

		ASSERT(m_rgpthreadWrite[ithread]==NULL);
		m_rgpthreadWrite[ithread] = AfxBeginThread( CommWriteThread, (void *) this);
		if (m_rgpthreadWrite[ithread]==NULL)
		{
			CommHandleError( pconnPartner, VWCOMM_E_COULDNTINITIALIZE, FALSE, TRUE);
			return VWCOMM_E_COULDNTINITIALIZE;
		}

		// specify that thread not disappear
		m_rgpthreadWrite[ithread]->m_bAutoDelete = FALSE;

	}
#endif

#ifdef PL_CHANNEL_DEBUG
	CString strMsg;
	strMsg.Format("CVWConn: Created control connection 0x%08X for primary 0x%08X\r\n", pconnPartner, pconn);
	OutputDebugString(strMsg);
#endif

	// start initial read on it - if this fails, it's cause there's too
	// much work going on (the read threads are too busy and none can take on
	// the new thread.)  What we do here is report and error and close the
	// connection.  This is important so that others don't try writing to
	// this socket (and then get blocked since there's no one around to read it).

	hr = CommAssignReadThread(pconnPartner);
	if (FAILED(hr))
	{
#ifdef PL_CHANNEL_DEBUG
		{
			CString str;
			str.Format("Connect: AssignReadThread failed at time: %d\n", ::GetLastError(), ::GetTickCount() );
			OutputDebugString((LPCTSTR)str);
		}
#endif
		CommHandleError( pconnPartner, VWCOMM_E_COULDNTINITIALIZE, FALSE, TRUE);
		return ReportCommError(hr);
	}

	// set up partner connections
	pconn->m_pconnPartner = pconnPartner;
	pconnPartner->AddRef();
	pconnPartner->m_pconnPartner = pconn;
	pconn->AddRef();

	return S_OK;
}

HRESULT CVWCommObject::CommGetProxyServer(LPCTSTR pszServer, BOOL& bUseProxy, CString& strProxy, int& nPort)
{
	HKEY hKey;
	DWORD dwType;
	DWORD cbData;
	BOOL bEnabled;
	char szProxy[_MAX_PATH];
	char szOverride[_MAX_PATH];

	// initialize
	bUseProxy = FALSE;
	strProxy = "";
	nPort = 0;

	// get registry information
	if (RegOpenKeyEx(	HKEY_CURRENT_USER, 
						"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings",
						0,
						KEY_READ,
						&hKey) != ERROR_SUCCESS)
	{
		return E_FAIL;
	}

	cbData = sizeof(bEnabled);
	if (RegQueryValueEx(hKey,
						"ProxyEnable",
						0,
						&dwType,
						(LPBYTE) &bEnabled,
						&cbData) != ERROR_SUCCESS)
	{
		return E_FAIL;
	}
	ASSERT(cbData==4);

	if (!bEnabled)
	{
		RegCloseKey(hKey);
		return S_OK;
	}

	cbData = sizeof(szProxy);
	if (RegQueryValueEx(hKey,
						"ProxyServer",
						0,
						&dwType,
						(LPBYTE) szProxy,
						&cbData) != ERROR_SUCCESS)
	{
		return E_FAIL;
	}

	cbData = sizeof(szOverride);
	if (RegQueryValueEx(hKey,
						"ProxyOverride",
						0,
						&dwType,
						(LPBYTE) szOverride,
						&cbData) != ERROR_SUCCESS)
	{
		return E_FAIL;
	}

	RegCloseKey(hKey);

	// is there a domain name in the server (real, not proxy)
	if (strchr(pszServer, '.')==NULL)
	{
		// nope - server is local.  See if there's a local override 
		if (strstr(szOverride, "<local>")!=NULL)
		{
			// yep, don't use proxy
			RegCloseKey(hKey);
			return S_OK;
		}
	}

	bUseProxy = TRUE;

	// parse out the proxy server and port
	char *pszProxy = szProxy;
	strupr(szProxy);
	if (strstr(pszProxy, "HTTP://")==pszProxy)
	{
		// skip the protocol
		pszProxy += 7;
	}

	char *pszPort = strchr(pszProxy, ':');
	if (pszPort!=NULL)
	{
		// terminate and advance
		*pszPort++ = '\0';
		nPort = atol(pszPort);
	}
	else
	{
		// default
		nPort = 80;
	}

	strProxy = pszProxy;
	
	return S_OK;

}; 

#if defined(PL_CHANNEL_DEBUG) && defined(ENABLE_LOGGING)

void LogInitialize()
{
	ASSERT(fiLogFile==-1);

	for (int i=0; i<100; i++)
	{
		CString strFile;
		strFile.Format("c:\\vworlds%d.log", i);
		fiLogFile = _sopen((LPCTSTR) strFile, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY | _O_SEQUENTIAL, _SH_DENYWR, _S_IWRITE);
		if (fiLogFile!=-1)
			break;
	}
	ASSERT(fiLogFile!=-1);

	// write out stamp
	CString str;
	char szDate[9];
	char szTime[9];
	_strdate(szDate);
	_strtime(szTime);

	str.Format("** V-Worlds Log File, created on %s, %s\r\n\r\n", szDate, szTime);
	Log((LPCTSTR) str);
}

void Log(LPCTSTR psz)
{
	int cb = strlen(psz);

	if (fiLogFile==-1)
		LogInitialize();

	ASSERT (_write(fiLogFile, (void *)psz, cb) == cb);
}

void LogData(LPCTSTR pszHeader, BYTE *pbData, DWORD cbData)
{
	static BOOL bInitialized = FALSE;
	static CRITICAL_SECTION cs;

	if (!bInitialized)
		::InitializeCriticalSection(&cs);

	::EnterCriticalSection(&cs);

	// write out the header
	Log(pszHeader);
	Log("\r\n");

	// write out the data
	char szLine[80] = "\r\n";
	for (unsigned int i=0; i<cbData; i++)
	{
		char szNum[3];
		int k=i % 16;

		if (k==0)
		{
			if (i>0)
				Log(szLine);

			sprintf(szLine, "%05d: ", i);
			for (int j=8; j<72; j++)
				szLine[j] = ' ';
			szLine[55] = ':';
			strcpy( &szLine[72], "\r\n");
		}

		sprintf(szNum, "%02X", pbData[i]);
		szLine[k*3+7] = szNum[0];
		szLine[k*3+8] = szNum[1];

		if (isprint(pbData[i]))
			szLine[56+k] = (char) pbData[i];
		else
			szLine[56+k] = '.';
	}
	Log(szLine);

	::LeaveCriticalSection(&cs);
}
#endif