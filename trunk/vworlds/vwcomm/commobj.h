// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// CommObj.h : Declaration of the CVWCommObject

#include <resource.h>
#include <propbase.h>
#include <authntic.h>
#include <winsock2.h>
#include <afxtempl.h>
#include <afxmt.h>

// V-Worlds optimized templates
#include <vwtempl.h>

#undef ReportCommError
#undef ReportMessageError
#undef ReportConnectionError

#define ReportCommError(hr) SUCCEEDED(hr) ? hr : ReportError("VWSYSTEM.Comm.1", IID_IVWComm, hr)
#define ReportMessageError(hr) SUCCEEDED(hr) ? hr : m_pcomm->ReportError("VWSYSTEM.Comm.1", IID_IVWCommMessage, hr)
#define ReportConnectionError(hr) SUCCEEDED(hr) ? hr : m_pcomm->ReportError("VWSYSTEM.Comm.1", IID_IVWCommConnection, hr)

EXTERN_C const CLSID CLSID_VWComm;
EXTERN_C const CLSID CLSID_VWCommMessage;

/////////////////////////////////////////////////////////////////////////////
// Parameters

// maximum connection backlog
#define cConnectBacklogMax	8

// maximum number of read and write threads (on Win95, max # of connections)
#define	cThreadReadMax		32
#define cThreadWriteMax		16

// default (initial) buffer size
#define cbReadMaxDefault	8192

// how often to send "keep-alive" messages
#define timeKeepAlivePeriod	10000

// default time to wait for a inter-thread operation
#define cmsecWaitDefault	10000

// amount of time to wait for a reply to a synchronous message
#ifdef _DEBUG
// 5 minutes (for debug version)
#define msecWaitWriteSynch	(5*60000)
#else
// 1 minute
#define msecWaitWriteSynch	(1*60000)
#endif

// maximum number of packets that a connection should send and
// receive per second (too many and we kill it).  These are maxima 
// for averages computed over a large enough sample.  msecMinSample
// is the minimum sample time (in milliseconds).  
//
// REVIEW: adjust these numbers
//
#define msecSampleWindow				5000
#define cMaxPacketsReceivedPerSecond	100
#define cMaxBytesReceivedPerSecond		100000
#define cMaxPacketsSentPerSecond		100
#define cMaxBytesSentPerSecond			100000

/////////////////////////////////////////////////////////////////////////////
// Convenience macros

inline BOOL WaitForEvent(HANDLE hevent, DWORD cmsecWait=cmsecWaitDefault)
{
	if (WaitForSingleObject(hevent, cmsecWait)!=WAIT_OBJECT_0)
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CReadThreadInfo::Class to encapsulate read thread management

class CVWCommObject;
class CVWCommConnection;
class CVWCommMessage;
typedef void *PWRITEBUFFER;
struct WRITEBUFFER;


class CReadThreadInfo
{
public:

	CReadThreadInfo();
	~CReadThreadInfo();

	// operations
	HRESULT Initialize(CVWCommObject *pcomm);
	HRESULT SetThread(CWinThread *pthread, BOOL bWin95);
	HRESULT ClearThread();
	HRESULT StartReadOnConnection(CVWCommConnection *pconn);
	HRESULT StopReadOnConnection(CVWCommConnection *pconn);
	HRESULT Terminate();

	BOOL WaitForStart(DWORD cmsecWait=cmsecWaitDefault);
	DWORD WaitForStartEx(DWORD cmsecWait=cmsecWaitDefault);
	void GetOperation(int& iOperation, CVWCommConnection** ppconn);
	void SignalDone(HRESULT hrReturn);
	int ConnectionCount();
	int IncrementConnectionCount();
	int DecrementConnectionCount();

	void Lock();
	void Unlock();

	// operation codes 
	enum Operation
	{
		opUninitialized=-1,
		opAddRead=0,
		opStopRead,
		opTerminate
	};

protected:

	BOOL WaitUntilDone(DWORD cmsecWait=cmsecWaitDefault);

	// Internal stuff
	CCriticalSection m_lock;

	// back pointer (weak ref)
	CVWCommObject *m_pcomm;

	// the thread 
	CWinThread * m_pthread;

	// Flag to indicate Windows 95 read threads
	BOOL m_bW95Thread;

	// event objects to control lpc

	// handle that thread waits on (to be told what to do)
	HANDLE	m_heventStart;

	// handle set by thread when it is done with an operation
	HANDLE	m_heventFinished;

	// operation code
	int m_iOperation;

	// argument for operation (weak reference)
	// NOTE: on Win95 this remains set to the connection for the thread
	// On NT, this identifies which connection m_iOperation
	// is to be used on
	CVWCommConnection *m_pconn;

	// Return code from operation
	HRESULT m_hrReturn;

	// number of connections assigned to this thread
	int m_cConnection;
};

/////////////////////////////////////////////////////////////////////////////
// CVWCommObject

class ATL_NO_VTABLE CVWCommObject : 
	public IVWComm,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVWCommObject, &CLSID_VWComm>
{
friend class CVWCommConnection;
friend class CVWCommMessage;
friend class CReadThreadInfo;
public:
	CVWCommObject();
	~CVWCommObject();

BEGIN_COM_MAP(CVWCommObject)
	COM_INTERFACE_ENTRY(IVWComm)
END_COM_MAP()

// Use DECLARE_NOT_AGGREGATABLE(CVWCommObject) if you don't want your object
// to support aggregation
DECLARE_AGGREGATABLE(CVWCommObject)

DECLARE_REGISTRY(CLSID_VWComm, "VWSYSTEM.Comm.1", "VWSYSTEM.Comm", IDS_VWCOMM_DESC, THREADFLAGS_BOTH)

// IVWComm
public:
	HRESULT ReportError(LPSTR lpszCLSID, IID iid, HRESULT hr)
	{
		HLOCAL lpMsgBuf = NULL;

		if (HRESULT_FACILITY(hr) == FACILITY_ITF && 
			HRESULT_CODE(hr) >= 0x0200)
		{
			if (HRESULT_CODE(hr) >= 0x0500 && HRESULT_CODE(hr) < 0x0600)
			{
				AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

				// pick off last 4 hex digits of HRESULT
				// match these to resource string table
				int resid = HRESULT_CODE(hr);
				CString strError;

				strError.LoadString(resid);

				VWReportError(lpszCLSID, (LPCTSTR)strError, iid, hr);
			
#ifdef _DEBUG
				TRACE("VWCOMM::ReportError: hr %x, %s\n", hr, (LPCTSTR)strError);
#endif

			}
		}
		// If there's a message associated with this error, report that
		else if (::FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, (DWORD)hr,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf, 0, NULL)
			> 0)
		{
			VWReportError(lpszCLSID, (LPCTSTR)lpMsgBuf, iid, hr);

#ifdef _DEBUG
			TRACE("VWCOMM::ReportError: hr %x, %s", hr, (LPCTSTR)lpMsgBuf);
#endif
			// Free the buffer, which was allocated by FormatMessage
			if (lpMsgBuf != NULL)
				::LocalFree(lpMsgBuf);
		}

		return hr;
	};

public:

// IVWComm
	STDMETHOD(Initialize)(IVWCommSite *pCommSite);
	STDMETHOD(Terminate)();
	STDMETHOD(Connect)(LPSTR pszRemote, int iPort, VARIANT_BOOL bTunnel, IVWCommConnection **ppICommConnection);
	STDMETHOD(DisconnectAll)();
	STDMETHOD(Accept)(int iPort);
	STDMETHOD(GetMessage)(IVWCommMessage **pICommMessage);
	STDMETHOD(get_HostName)(BSTR* pbstrVal);
	STDMETHOD(KeepAlive)();
	STDMETHOD(GetUsageStatistics)(	int *pcPacketsSent,
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
									int *pcMeanTimeInReadQueue );

protected:

	enum State
	{
		stateUninitialized=0,
		stateInitialized,
		stateAccepting,
		stateConnected,
		stateTerminating,

	} m_state;

	// back-pointer to site (for notifications)
	IVWCommSite	*m_pCommSite;

	// general stuff
	BOOL	m_bOnWindows95;
	SOCKET	m_sockBase;

	// accept thread
	CWinThread *		m_pthreadAccept;

	// connection list
	CCriticalSection	m_lockConnList;
	CList<CVWCommConnection*,CVWCommConnection*&> m_listConnection;

	// cookie (for associating primary and control HTTP connections)
	DWORD	m_dwCookieNext;

	// message list
	CCriticalSection	m_lockMessages;
	int					m_cMessagesInQueue;
	CVWCommMessage *	m_pMessHead;
	CVWCommMessage *	m_pMessTail;

	// write thread stuff
	CCriticalSection	m_lockWriteQueue;
	HANDLE				m_heventDataInWriteQueue;
	HANDLE				m_heventWriteQueueBelowThreshold;
	int					m_cthreadWrite;
	CWinThread *		m_rgpthreadWrite[cThreadWriteMax];
	int					m_cBuffersInQueue;
	WRITEBUFFER *		m_pwbHead;
	WRITEBUFFER *		m_pwbTail;

	// read threads
	int					m_cThreadRead;
	CReadThreadInfo		m_rgThreadInfo[cThreadReadMax];

	// some kernel information (for avoiding linkage problems in win95)
	typedef BOOL (_stdcall *PFNCANCEL)(HANDLE);
	HINSTANCE			m_hinstKernel;
	PFNCANCEL			m_pfnCancel;

	// time since last keep-alive message was sent (client)
	DWORD				m_timeLastKeepAlive;

	// communication statistics
	DWORD	m_timeSampleStart;
	int		m_cPacketsSentTotal;
	int		m_cPacketsSent;
	int		m_cPacketsSentPerSecond;
	int		m_cBytesSentTotal;
	int		m_cBytesSent;
	int		m_cBytesSentPerSecond;
	int		m_cPacketsReceivedTotal;
	int 	m_cPacketsReceived;
	int		m_cPacketsReceivedPerSecond;
	int		m_cBytesReceivedTotal;
	int 	m_cBytesReceived;
	int		m_cBytesReceivedPerSecond;
	int		m_cQueueSamples;
	int		m_cSumPacketsInWriteQueue;
	int		m_cMeanPacketsInWriteQueue;
	int		m_cSumMessagesInReadQueue;
	int		m_cMeanMessagesInReadQueue;
	int		m_cTimeInWriteQueueSamples;
	int		m_cSumTimeInWriteQueue;
	int		m_cMeanTimeInWriteQueue;
	int		m_cTimeInReadQueueSamples;
	int		m_cSumTimeInReadQueue;
	int		m_cMeanTimeInReadQueue;

	// message types (for CommWrite*)
	enum
	{
		typeAsync=0,
		typeSynch,
		typeReply,
		typeKeepAlive
	};

	// low-level IO functions
	HRESULT CommConstruct();
	HRESULT CommInit();
	HRESULT CommTerminate();
	HRESULT CommDisconnectAll();
	HRESULT CommAccept(int iPort);
	HRESULT CommConnect(LPCTSTR pszRemote, int nPort, VARIANT_BOOL bTunnel, CVWCommConnection **ppconn);
	HRESULT CommDisconnect(CVWCommConnection *pcon, BOOL bRemoveFromList);
	HRESULT CommAssignReadThread(CVWCommConnection *pconn);
	HRESULT CommEnqueueMessage(int nMessageStatus, CVWCommConnection *pconn, BYTE *pbData=NULL, DWORD cbData=0, char chSerial=0);
	HRESULT CommReadAsync(CVWCommConnection *pconn);
	HRESULT	CommAllocWriteBuffer(IMarshallBuffer *pcb, PWRITEBUFFER *ppwb);
	HRESULT CommAllocKeepAliveBuffer(PWRITEBUFFER *ppwb);
	HRESULT CommWrite(CVWCommConnection *pconn, PWRITEBUFFER pvData, BYTE chSyncSerial=0, BYTE chType=typeAsync);
	HRESULT CommWriteExt(int cConnection, IVWCommConnection *rgpConnection[], PWRITEBUFFER pvData, BYTE chSyncSerial=0, BYTE chType=typeAsync);
	HRESULT CommWriteSynch(CVWCommConnection *pconn, PWRITEBUFFER pvData, BYTE chSyncSerial, IUnMarshallBuffer **ppcbReply);
	HRESULT CommReply(CVWCommConnection *pconn, PWRITEBUFFER pvData, BYTE chSynchSerial );
	HRESULT CommSend(WRITEBUFFER* pwb);
	HRESULT CommSendToOne(WRITEBUFFER* pwb, CVWCommConnection *pconn);
	HRESULT CommCancelPendingIO(CVWCommConnection *pconn);
	BOOL	CommIsWriteQueueEmpty();
	HRESULT	CommWaitForEmptyWriteQueue();
	HRESULT CommKeepAlive();
	HRESULT	CommSendKeepAlive(CVWCommConnection *pconn);
	HRESULT CommGetHostName(BSTR *pbstrOut);
	HRESULT CommRemoveReplyMarker(void);
	HRESULT CommGetMessage(IVWCommMessage **pmess);
	HRESULT CommAddConnection(CVWCommConnection *pconn);
	HRESULT CommRemoveConnection(CVWCommConnection *pconn);
	HRESULT	CommUpdateStatistics(CVWCommConnection *pconn, int cBytesReceived, int cBytesSent);
	HRESULT CommUpdateQueueTimeStatistics(DWORD m_timeRead, DWORD m_timeWrite);
	HRESULT CommCloseConn(CVWCommConnection *pconn);

	HRESULT CommHandleError(CVWCommConnection *pconn, int nErrorCode, BOOL bEnqueue=TRUE, BOOL bClose=TRUE);
	HRESULT CommHandleInput(CVWCommConnection *pconn);
	HRESULT CommHandleInput_Server(CVWCommConnection *pconn);
	HRESULT CommHandleInput_Client(CVWCommConnection *pconn);
	HRESULT CommHandleHeader(CVWCommConnection *pconn);
	HRESULT CommHandleReply(CVWCommConnection *pconn);
	HRESULT CommFindConnection(DWORD dwCookie, CVWCommConnection **ppconn);
	HRESULT CommWriteToSocket(SOCKET sock, BYTE *pbData, DWORD cbData);
	HRESULT CommEstablishPartner(CVWCommConnection  *pconn);
	HRESULT CommGetProxyServer(LPCTSTR pszServer, BOOL& bUseProxy, CString& strProxy, int& nPort);
	char *	CommGetLine(char **psz);


	void CommLockConnList()
	{
		m_lockConnList.Lock();
	}

	void CommUnlockConnList()
	{
		m_lockConnList.Unlock();
	}

	void CommLockMessages()
	{
		m_lockMessages.Lock();
	}

	void CommUnlockMessages()
	{
		m_lockMessages.Unlock();
	}

	void CommLockWriteQueue()
	{
		m_lockWriteQueue.Lock();
	}

	void CommUnlockWriteQueue()
	{
		m_lockWriteQueue.Unlock();
	}

	static UINT CommWriteThread(void *pv);
	static UINT CommAcceptThread(void *pv);
	static UINT CommNTReadThread(void *pv);
	static UINT CommW95ReadThread(void *pv);
	static void WINAPI CommReadCompletionFunction(DWORD dwError, DWORD cbTransferred, LPOVERLAPPED pols);

private:
	void AddStringToAtomMap(BSTR bstr);

public:
	static CVWMap<CBSTRAdapt<CComBSTR>, BSTR, int, int&> m_atomMap;
	static CVWMap<int, int&, CBSTRAdapt<CComBSTR>, BSTR> m_stringMap;
	static int m_iAtomIndex;
};

/////////////////////////////////////////////////////////////////////////////
// CVWCommConnection - represents a connection between machines

class ATL_NO_VTABLE CVWCommConnection:
	public IVWCommConnection,
	public CComObjectRoot
{
friend class CVWCommObject;
friend class CVWCommMessage;
friend class CReadThreadInfo;
public:
	CVWCommConnection();
	~CVWCommConnection();
BEGIN_COM_MAP(CVWCommConnection)
	COM_INTERFACE_ENTRY(IVWCommConnection)
END_COM_MAP()
// Use DECLARE_NOT_AGGREGATABLE(CVWCommObject) if you don't want your object
// to support aggregation
DECLARE_AGGREGATABLE(CVWCommConnection)

// IVWCommConnection
public:
	STDMETHOD(SendAsync)( IMarshallBuffer *pcb);
	STDMETHOD(SendAsyncExt)(IMarshallBuffer *pcb, int cConnections, IVWCommConnection *rgpConnection[]);
	STDMETHOD(SendSync)( IMarshallBuffer *pcb, IUnMarshallBuffer **ppcbReply);
	STDMETHOD(Disconnect)();

	STDMETHOD(get_SocketAddress)(BSTR* pbstrVal);

	STDMETHOD(put_Authentication)(IAuthentication *pauthP)
	{
		SAFERELEASE(m_pauth);
		SAFEADDREF(pauthP);
		m_pauth = pauthP;
		return S_OK;
	}

	STDMETHOD(get_Authentication)(IAuthentication **ppauth)
	{
		if (ppauth==NULL)
			return E_POINTER;

		SAFEADDREF(m_pauth);
		*ppauth = m_pauth;

		return S_OK;
	}

	STDMETHOD(get_IsAuthenticated)(VARIANT_BOOL *pbAuthenticated)
	{
		if (pbAuthenticated==NULL)
			return E_POINTER;

		*pbAuthenticated = VARIANT_FALSE;

		if (m_pauth!=NULL)
			return m_pauth->get_Authenticated(pbAuthenticated);
		else
			return S_OK;
	}

	STDMETHOD(put_UserData)(void *pvData)
	{
		m_pvData = pvData;
		return S_OK;
	}

	STDMETHOD(get_UserData)(void **ppvData)
	{
		if (ppvData==NULL)
			return E_POINTER;

		*ppvData = m_pvData;
		return S_OK;
	}

	enum ConnectionType
	{
		eUnknown=0,
		eNative,
		ePrimaryHTTP,
		eControlHTTP
	};

	HRESULT Initialize(CVWCommObject *pcomm, SOCKET sock, PSOCKADDR psa, LPCTSTR pszServer=NULL, int iPort=0, BOOL bClientSide=TRUE);
	HRESULT SetConnectionType(ConnectionType ct);
	void SetAsciiMode(BOOL bAscii) { m_bAsciiMode = bAscii; };
	void SetReplyReceived(BOOL bReceived) { m_bReplyReceived = bReceived; };

	ConnectionType Type() { return m_ct; };
	BOOL ClientSide() { return m_bClientSide; };
	BOOL AsciiMode() { return m_bAsciiMode; };
	BOOL ReplyReceived() { return m_bReplyReceived; };

protected:

	// socket
	SOCKET			m_sock;

	// Connection type and client-side flag
	ConnectionType	m_ct;
	BOOL			m_bClientSide;

	// flag indicating ascii/binary parse (receive) mode
	BOOL			m_bAsciiMode;

	// flag indicating that reply to last tunneled message has been received
	BOOL			m_bReplyReceived;

	// cookie (for associating primary and control connections)
	DWORD			m_dwCookie;

	// back-pointer to comm object
	CVWCommObject *	m_pcomm;

	// pointer to associated connection
	CVWCommConnection *m_pconnPartner;

	// back-pointer to read thread info
	CReadThreadInfo *m_pti;
	void SetReadThreadInfo(CReadThreadInfo *pti)
	{
		m_pti = pti;
	}

	// authentication pointer
	IAuthentication *m_pauth;

	// user data
	void *		m_pvData;

	// comm stuff
	CString		m_strServer;
	int			m_iPort;
	SOCKADDR_IN	m_sockaddr;
	HRESULT GetSockAddr(PSOCKADDR psa);

	// buffer
	BYTE *	m_pbRead;
	DWORD	m_cbRead;
	DWORD	m_cbReadMax;

	// flag indicating that the connection is being closed cleanly
	BOOL	m_bInDisconnect;

	// flag indicating that a buffer is being written for the connection
	BOOL	m_bInWrite;

	// next sequence number for synchronouse sends
	BYTE	m_chSynchSerial;

	// Information for processing synchronous sends

	// 1) flag indicating that we're waiting for a reply
	BOOL	m_bInSendSynch;

	// 2) expected serial# of reply
	BYTE	m_chSynchSerialReply;

	// 3) handle of wait event for sync sends
	HANDLE	m_heventHaveData;

	// 4) buffer for reply message
	IUnMarshallBuffer * m_pBufferReply;

	// 5) flag indicating that the reply has been received
	BOOL	m_bHaveReply;

	// 6) flag indicating failure during wait
	BOOL	m_bReplyFailed;

	// overlapped structure for asynchronous IO
	OVERLAPPED	m_ols;

	// data to keep track of communication statistics and to
	// detect evil users
	DWORD	m_timeSampleStart;
	int		m_cPacketsSent;
	int		m_cPacketsSentPerSecond;
	int		m_cBytesSent;
	int		m_cBytesSentPerSecond;
	int 	m_cPacketsReceived;
	int		m_cPacketsReceivedPerSecond;
	int 	m_cBytesReceived;
	int		m_cBytesReceivedPerSecond;
};

/////////////////////////////////////////////////////////////////////////////
// CVWCommMessage - A message. Can represent a received message
//		or some kind of status or error condition

class ATL_NO_VTABLE CVWCommMessage:
	public IVWCommMessage,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVWCommMessage, &CLSID_VWCommMessage>
{
friend class CVWCommObject;
public:
	CVWCommMessage();
	~CVWCommMessage();
BEGIN_COM_MAP(CVWCommMessage)
	COM_INTERFACE_ENTRY(IVWCommMessage)
END_COM_MAP()
// Use DECLARE_NOT_AGGREGATABLE(CVWCommObject) if you don't want your object
// to support aggregation
DECLARE_AGGREGATABLE(CVWCommMessage)

// IVWCommMessage
public:

	enum
	{
		statusUninitialized=-1,	//-1 = Uninitialized
		statusOk=0,				// 0 = Ok - regular incoming message
		statusReadError,		// 1 = Read error
		statusWriteError,		// 2 = Write error
		statusAcceptError,		// 3 = Accept error
		statusConnect,			// 4 = New connection
		statusCleanDisconnect,	// 5 = Clean disconnect
		statusDirtyDisconnect,	// 6 = Dirty disconnect
		statusOutOfMemory,		// 7 = Out of memory error
		statusEvilUser,			// 8 = Disconnected due to bad packet or too
								//     much activity
		statusHaveResponse		// 9 = Received response to a syncsend.
	};

	STDMETHOD(GetStatus)(int *pnStatus)
	{
		if (pnStatus==NULL)
			return E_POINTER;

		*pnStatus = m_nStatus;

		return S_OK;
	};

	STDMETHOD(GetConnection)(IVWCommConnection **ppIVWCommConnection);
	STDMETHOD(GetData)(IUnMarshallBuffer **ppbuffer);
	STDMETHOD(GetSynchronous)(VARIANT_BOOL *pbSync);
	STDMETHOD(Reply)(IMarshallBuffer* pbuffer);

protected:
	// forward pointer and accessors
	CVWCommMessage *m_pMessNext;
	void SetNext(CVWCommMessage *pMessNext) { m_pMessNext = pMessNext; }
	CVWCommMessage *GetNext() { return m_pMessNext; }

	// status code and settor
	int m_nStatus;
	void SetStatus(int nStatus) { m_nStatus = nStatus; }

	// back pointers and settor functions
	CVWCommObject *m_pcomm;
	CVWCommConnection *m_pconn;
	void Initialize(CVWCommObject *pcomm, CVWCommConnection *pconn);

	// associated data (can be null) and settor function
	IUnMarshallBuffer* m_pUbuffer;
	HRESULT SetData(BYTE * pb, DWORD cb, char chSynch);

	// sequence number for replying to synchronous sends
	BYTE m_chSynchSerial;

	// time when buffer was enqueued (for stats)
	DWORD m_timeEnqueued;
	void SetTimeStamp(DWORD time) { m_timeEnqueued = time; }
};