// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// CommObj.h : Declaration of the CFastCommObject

#include <resource.h>
#include <propbase.h>
#include <authntic.h>
#include <afxtempl.h>
#include <afxmt.h>

// V-Worlds optimized templates
#include <vwtempl.h>

// FastComm implementation.
#include "commlayr.h"

#undef ReportCommError
#undef ReportMessageError
#undef ReportConnectionError

#define ReportCommError(hr) SUCCEEDED(hr) ? hr : ReportError("VWSYSTEM.FastComm.1", IID_IVWComm, hr)
#define ReportMessageError(hr) SUCCEEDED(hr) ? hr : m_pcomm->ReportError("VWSYSTEM.FastComm.1", IID_IVWCommMessage, hr)
#define ReportConnectionError(hr) SUCCEEDED(hr) ? hr : m_pFastCommObject->ReportError("VWSYSTEM.FastComm.1", IID_IVWCommConnection, hr)

EXTERN_C const CLSID CLSID_FastComm;
EXTERN_C const CLSID CLSID_FastCommConnection;
EXTERN_C const CLSID CLSID_FastCommMessage;

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

#pragma pack()

// message types
enum
{
	typeAsync=0,
	typeSynch,
	typeReply,
	typeKeepAlive
};

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
// CReadThreadInfo::Class to encapsulate read thread management

class CFastCommObject;
class CFastCommConnection;
class CFastCommMessage;

/////////////////////////////////////////////////////////////////////////////
// CFastCommObject

class ATL_NO_VTABLE CFastCommObject : 
	public IVWComm,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CFastCommObject, &CLSID_FastComm>
{
friend class CFastCommConnection;
friend class CFastCommMessage;

public:
	CFastCommObject();
	~CFastCommObject();

BEGIN_COM_MAP(CFastCommObject)
	COM_INTERFACE_ENTRY(IVWComm)
END_COM_MAP()

// Use DECLARE_NOT_AGGREGATABLE(CFastCommObject) if you don't want your object
// to support aggregation
DECLARE_AGGREGATABLE(CFastCommObject)

DECLARE_REGISTRY(CLSID_FastComm, "VWSYSTEM.FastComm.1", "VWSYSTEM.FastComm", IDS_VWCOMM_DESC, THREADFLAGS_BOTH)

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
				TRACE("FASTCOMM::ReportError: hr %x, %s\n", hr, (LPCTSTR)strError);
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
			TRACE("FASTCOMM::ReportError: hr %x, %s", hr, (LPCTSTR)lpMsgBuf);
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
									int *pcMeanTimeInReadQueue )
	{
		return S_OK;
	};

protected:

	HRESULT SendExt(IMarshallBuffer *pcb, int cConnection, IVWCommConnection *rgpConnection[], BYTE chSynchSerial, BYTE chMsgType);

	int							m_cMessagesInQueue;
	CFastCommMessage			*m_pMessHead;
	CFastCommMessage			*m_pMessTail;
	CComPtr<IVWCommSite>		m_pCommSite;
	CComPtr<CFastCommConnection>	m_pListener;
	CList<CFastCommConnection*, CFastCommConnection*>	m_pConnections;
	CRITICAL_SECTION			m_ConnectionsCS;
	CRITICAL_SECTION			m_MessagesCS;
};

class IMyMessageFramerCallback
{
public:
	virtual void FramedMessage(ULONG ulCookie, BYTE *pBuffer, ULONG ulSizeOfBuffer, COMBLK *pCallback) = 0;			
};

class CMyMessageFramer
{
	//The Destructor is threadsafe. The rest of this class is *not* threadsafe and must be 
	//impl by the caller (threadsafety is guarenteed
	//in Fastcomm's case by a custom counted exclusive lock). 
public:
	CMyMessageFramer(IMyMessageFramerCallback *pCallback);
	
	~CMyMessageFramer();

	void FrameData(ULONG ulCookie, BYTE *pBuffer, ULONG ulNumberOfBytesTransferred);

private:
	//callback
	IMyMessageFramerCallback * m_pCallback;

	//output framed buffer.
	BYTE * m_pFramedBuffer;
	
	//output framed buffer's size.
	ULONG m_ulFramedBufferSize;

	//length byte used for framing.
	ULONG m_ulLengthByte;

	//string representation for byte.
	BYTE m_strByte[sizeof(COMBLK)];

	//variable used for framing algorithm.
	BYTE * m_pFramedBufferCurrentLocation;
	
	//variable used for framing algorithm.
	ULONG m_ulFramedBufferSizeLeft;

	// flag indicating ascii/binary parse (receive) mode
	BOOL  m_bAsciiMode;
};


/////////////////////////////////////////////////////////////////////////////
// CFastCommConnection - represents a connection between machines

class ATL_NO_VTABLE CFastCommConnection:
	public IVWCommConnection,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CFastCommConnection, &CLSID_FastCommConnection>,
	public ICallback,
	private IMyMessageFramerCallback
{
friend class CFastCommObject;
friend class CFastCommMessage;

public:
	CFastCommConnection();
	~CFastCommConnection();

BEGIN_COM_MAP(CFastCommConnection)
	COM_INTERFACE_ENTRY(IVWCommConnection)
END_COM_MAP()

// Use DECLARE_NOT_AGGREGATABLE(CFastCommObject) if you don't want your object
// to support aggregation
DECLARE_AGGREGATABLE(CFastCommConnection)

// IVWCommConnection
public:
	STDMETHOD(SendAsync)(IMarshallBuffer *pcb);
	STDMETHOD(SendAsyncExt)(IMarshallBuffer *pcb, int cConnections, IVWCommConnection *rgpConnection[]);
	STDMETHOD(SendSync)(IMarshallBuffer *pcb, IUnMarshallBuffer **ppcbReply);
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

	void *m_pData;
	STDMETHOD(put_UserData)(void *pvData)
	{
		m_pData=pvData;
		return S_OK;
	}

	STDMETHOD(get_UserData)(void **ppvData)
	{
		*ppvData=m_pData;
		return S_OK;
	}

//ICallback
public:
	//no need for virtual. CH.
	void OnPreAccept(SmartPtr<CConnection> & pAcceptor, ICallback **ppCallback, ReceiveMode *pRcvMode);
	void OnAccept(SmartPtr<CConnection> &pNewConnection, SmartPtr<CConnection> pListener);
	void OnDisconnect(SmartPtr<CConnection> &pThis);
	void OnDataAvailable(SmartPtr<CConnection> &pThis, BYTE *pBuffer, ULONG ulSize);
	void OnFramedDataAvailable(SmartPtr<CConnection> &pThis, BYTE *pBuffer, ULONG ulSize)
	{
		ASSERT(0);
	}
	void OnErrorNoBuffers(SmartPtr<CConnection> &pThis);
	void OnSendCompletion(SmartPtr<CConnection> &pThis, int iWinsockError, DWORD ulNumberOfBytesTransferred){};

//IMessageFramerCallback
	void FramedMessage(ULONG ulCookie, BYTE *pBuffer, ULONG ulSizeOfBuffer, COMBLK *pComblk);			

// Helper
	void GetStatistics(long *plNoAsyncSendsInWinsockQueue);
protected:

	HRESULT Initialize(CFastCommObject *pFastCommObject, IVWCommSite *pCommSite);
	HRESULT SetConnector(CConnection *pConnector);
	HRESULT EnqueueMessage(int nMessageStatus, CFastCommConnection *pconn, BYTE *pBuffer, ULONG ulSize, char chSerial);

	CMyMessageFramer m_Framer;
	SmartPtr<CConnection>		m_pConnector;
	CFastCommObject				*m_pFastCommObject;
	CComPtr<IVWCommSite>		m_pCommSite;
	// authentication pointer
	IAuthentication *m_pauth;
	BOOL						m_bDirtyDisconnect;
};

/////////////////////////////////////////////////////////////////////////////
// CFastCommMessage - A message. Can represent a received message
//		or some kind of status or error condition

class ATL_NO_VTABLE CFastCommMessage:
	public IVWCommMessage,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CFastCommMessage, &CLSID_FastCommMessage>
{
friend class CFastCommObject;
friend class CFastCommConnection;

public:
	CFastCommMessage();
	~CFastCommMessage();

BEGIN_COM_MAP(CFastCommMessage)
	COM_INTERFACE_ENTRY(IVWCommMessage)
END_COM_MAP()

// Use DECLARE_NOT_AGGREGATABLE(CFastCommObject) if you don't want your object
// to support aggregation
DECLARE_AGGREGATABLE(CFastCommMessage)

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
		if (pnStatus == NULL)
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
	CFastCommMessage *m_pMessNext;
	void SetNext(CFastCommMessage *pMessNext) { m_pMessNext = pMessNext; }
	CFastCommMessage *GetNext() { return m_pMessNext; }

	// status code and settor
	int m_nStatus;
	void SetStatus(int nStatus) { m_nStatus = nStatus; }

	// back pointers and settor functions
	CFastCommObject			*m_pcomm;
	CFastCommConnection 	*m_pconn;
	HRESULT Initialize(CFastCommObject *pcomm, CFastCommConnection *pconn);

	// associated data (can be null) and settor function
	IUnMarshallBuffer* m_pUbuffer;
	HRESULT SetData(BYTE * pb, DWORD cb, char chSynch);

	// sequence number for replying to synchronous sends
	BYTE m_chSynchSerial;

	// time when buffer was enqueued (for stats)
	DWORD m_timeEnqueued;
	void SetTimeStamp(DWORD time) { m_timeEnqueued = time; }
};