// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

///////////////////////////////////////////////////////////////////////////////////
// Project:		FastComm
// Module:		commlayr.h
// Description:	defines user classes: CConnection, ICallback and internal classes.
// Author:		CH
// Create:		10/28/1999
/////////////////////////////////////////////////////////////////////////////////
#ifndef CommLayerHeader
#define CommLayerHeader

#include "stdafx.h"
#include "toolkit\tools.h"
#include "queue"
#include "list"

/////////////////////////////////////////////////////////////////////////////
//Defines
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//This *has* to bigger than 2*(sizeof(SOCKADDR_IN)+16).
//const int BUFFER_SIZE = 3*(sizeof(SOCKADDR_IN)+16);
/////////////////////////////////////////////////////////////////////////////
const int BUFFER_SIZE = 1024;

/////////////////////////////////////////////////////////////////////////////
//Number of ACCEPTORS per listener.
//Perf times on quad xeon 450mhz machine:
//Acceptors Throughput (connections/second)
//20	1428
//50	1600
//
//On a single proc machine:
//20	363 conns/second.
//
//For debugging, set to 1. For perf, set to 20.
/////////////////////////////////////////////////////////////////////////////
const int NUMBER_OF_ACCEPTORS = 25;

/////////////////////////////////////////////////////////////////////////////
//Number of recvs queued per connection per processor.
/////////////////////////////////////////////////////////////////////////////
const int NUMBER_OF_QUEUED_RECVS_PER_CONNECTION_PER_PROCESSOR = 3;

/////////////////////////////////////////////////////////////////////////////
//defining this turns off pooled allocators.
/////////////////////////////////////////////////////////////////////////////
//#define DEBUG_ALLOCATORS

/////////////////////////////////////////////////////////////////////////////
//Initial wait time for exponential backoff technique to deal with 
//WSANOBUFs error. The initial wait time should equal the minimum time
//you think Winsock will take to clear the buffers
//after calling _FireOnErrorNoBuffers.
/////////////////////////////////////////////////////////////////////////////
const int INITIAL_BACKOFF_WAIT_TIME = 1000; //in ms.

/////////////////////////////////////////////////////////////////////////////
//Maximum wait time. Worst case total wait time is this value * 2.
/////////////////////////////////////////////////////////////////////////////
const int MAXIMUM_BACKOFF_WAIT_TIME = INITIAL_BACKOFF_WAIT_TIME * 4;

/////////////////////////////////////////////////////////////////////////////
//forward decl.
/////////////////////////////////////////////////////////////////////////////
class CConnection;

/////////////////////////////////////////////////////////////////////////////
//Data exchanger class. Pooled memory.
//This object is passed into every async winsock
//call. It contains all the info necessary to 
//process the callback correctly.
//Used by CConnection only (users don't need to use this).
/////////////////////////////////////////////////////////////////////////////
class CDataExchanger
{
public:
	enum BufferType {bf_OnAccept, bf_OnReceive, bf_OnMetaReceive, bf_SendAsync, bf_Empty};

	CDataExchanger();
	~CDataExchanger();
	
	////////////////////////////////////////////////////////////////////
	//Initialization fcns.

	//Prepare CDE to pass into winsock connect.
	void InitializeConnect(SmartPtr<CConnection> pThisConnection, SmartPtr<CConnection> pAcceptor);

	//Prepare CDE to pass into winsock recv.
	void InitializeRecv(SmartPtr<CConnection> pThisConnection);

	//Tell recv queue completion callback to start a recv.
	void InitializeMetaRecv(SmartPtr<CConnection> pThisConnection);

	//Prepare CDE to pass into winsock send.
	void InitializeSendAsync(SmartPtr<CConnection> pThisConnection, BYTE *pbuffer, ULONG ulSize);

	//performs some checks to make sure data is valid. (for debugging).
	void CheckIsValid(void);

	void SetProcessed(void)
	{
		m_bProcessed = true;
	}

	CString Dump(void);
	
	////////////////////////////////////////////////////////////////////
	//Data
	
	//this overlapped struct *must* be the first member here.
	WSAOVERLAPPED m_Overlapped;

	BYTE m_buffer[BUFFER_SIZE];

	//passed to WSARecv.
	WSABUF m_WSABuf;
	
	//In/Out for WSARecv.
	DWORD  m_dwFlags;              

	//In/Out for WSARecv.
	//parse the overlapped struct for
	//this info instead. This contains the
	//*initial* bytes read.
	ULONG m_dwNoBytesRead;
		
	bool m_bProcessed;

	//connection this CDE was passed into winsock call with.
	SmartPtr<CConnection> m_pThisConnection;

	BufferType m_Type;
	
	////////////////////////////////////////////////////////////////////
	//Union. (A literal union is not allowed for classes).//////////////
		//bf_OnAccept fields.
		SmartPtr<CConnection> m_pAcceptorConnection;
		ULONG m_ulSize;
	////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////
	//Global Memory Management stuff.
	static CString GlobalDump();
	
	static long m_iTotalObjectsAllocated;
	static CLockFreeStackEx<CDataExchanger *> m_AvailCDataExchangerPool;
	static CSystemAllocator m_SystemAllocator;
	static void GlobalCleanup(void);

	#ifndef DEBUG_ALLOCATORS
		//Defining these will put the pooled allocators into action.
		void * operator new(size_t size);
		void operator delete(void *);
	#endif
};

/////////////////////////////////////////////////////////////////////////////
//Callback interface users of CConnection need to implement.
/////////////////////////////////////////////////////////////////////////////
enum ReceiveMode {recv_Framed, recv_Streaming};

//It is best to use one class to receive all the callbacks, as opposed to each connection getting
//separate callbacks. This simplifies the destruction sequence, at the cost of possibly more
//lookups.
class ICallback
{
public:
	//OnPreAccept is called before the listener has accepted a new incoming connection to allow
	//the user to specify new callback and receive mode. The settings
	//specified will be used to create the new acceptor.
	//*ppOutCallback *pOutRcvMode are preset with the default listener's callback and rcvMode.
	virtual void OnPreAccept(SmartPtr<CConnection> & pAcceptor, ICallback **ppOutCallback, ReceiveMode *pOutRcvMode) = 0;
	
	//OnAccept is called when the listener has accepted (received) a new incoming connection. 
	virtual void OnAccept(SmartPtr<CConnection> &pNewConnection, SmartPtr<CConnection> pListener) = 0;
	
	//OnDisconnect is called to notify user of disconnect. Release your ref to the conn at this point.
	virtual void OnDisconnect(SmartPtr<CConnection> &pThis) = 0;

	//OnDataAvailable is called to notify user of received data. The user should copy the data
	//and must not delete it.
	virtual void OnDataAvailable(SmartPtr<CConnection> &pThis, BYTE *pBuffer, ULONG ulSize) = 0;

	//OnFramedDataAvailable is called when framed data is available.
	//NOTE: The user must delete the buffer themselves
	//using CConnection::m_SystemAllocator.free(pBuffer);
	virtual void OnFramedDataAvailable(SmartPtr<CConnection> &pThis, BYTE *pBuffer, ULONG ulSize) = 0;

	//OnSendCompletion is called to notify the completion of a send. This
	//does not mean the recipient has received the buffer, just that it has completed it's
	//trip through the TCP/IP stack.
	virtual void OnSendCompletion(SmartPtr<CConnection> &pThis, int iWinsockError, DWORD ulNumberOfBytesTransferred) = 0;

	//OnNoBuffers is called when Winsock has run out of memory. Use this
	//opportunity to free space for the system.
	virtual void OnErrorNoBuffers(SmartPtr<CConnection> &pThis) = 0;
};

/////////////////////////////////////////////////////////////////////////////
//class passed to workQ to process custom requests.
/////////////////////////////////////////////////////////////////////////////
class CConnection_Request : public CQueueMsg
{
	public:
		CConnection_Request() : m_pConnection(NULL) {}
		void Initialize(CConnection *pCon) {m_pConnection = pCon;}
		~CConnection_Request() {m_pConnection = NULL;}

	protected:
		virtual void PerformRequest(OVERLAPPED *pOverlapped);
	
	private:
		//Special case. Not ref counted.
		//No smart ptr. Prevent circular ref-count.
		//Lifetime control ensures validity of this
		//ptr.
		CConnection * m_pConnection;	
};

/////////////////////////////////////////////////////////////////////////////
//Default msg framing class for a DWORD length prefix.
/////////////////////////////////////////////////////////////////////////////
class IMessageFramerCallback
{
public:
	//OnFramedData is called when framed data is available.
	//NOTE: The user must delete the buffer themselves
	//using CSystemAllocator::m_SystemAllocator.free(pBuffer);
	virtual void OnFramedData(ULONG ulCookie, BYTE *pBuffer, ULONG ulSizeOfBuffer) = 0;			
};

class CMessageFramer
{
	//The Destructor is threadsafe. The rest of this class is *not* threadsafe and must be 
	//impl by the caller (threadsafety is guarenteed
	//in Fastcomm's case by a custom counted exclusive lock). 
public:
	CMessageFramer();
	~CMessageFramer();

	//call this function with:
	//cookie to pass back to caller for whatever purpose you may need,
	//callback pointer of object to callback on,
	//the incoming buffer of data, and how many bytes in the buffer.
	void FrameData(ULONG ulCookie, IMessageFramerCallback * pCallback, 
		BYTE *pBuffer, ULONG ulNumberOfBytesTransferred);

	static void GlobalCleanup(void)
	{
		m_SystemAllocator.~CSystemAllocator();
	}

private:
	//output framed buffer.
	BYTE * m_pFramedBuffer;
	
	//output framed buffer's size.
	ULONG m_ulFramedBufferSize;

	//length byte used for framing.
	ULONG m_ulLengthByte;

	//string representation for byte.
	BYTE m_strByte[4];

	//variable used for framing algorithm.
	BYTE * m_pFramedBufferCurrentLocation;
	
	//variable used for framing algorithm.
	ULONG m_ulFramedBufferSizeLeft;

	static CSystemAllocator m_SystemAllocator;
};

/////////////////////////////////////////////////////////////////////////////
//Connection class.
/////////////////////////////////////////////////////////////////////////////
class CConnection : private IMessageFramerCallback
{
	//let Connection_Request call Addref/Release, private fcns.
	friend SmartPtr<CConnection>;

	//let Connection_Request call _OnCallback, a private fcn.
	friend CConnection_Request;

public:
	//creation functions:

	//use on server-side.
	static int CreateListener(USHORT nPort, ICallback *pCallback, ReceiveMode rcvMode,
		SmartPtr<CConnection> & pOutListener);

	static int CreateConnector(SOCKADDR_IN & SockAddr, ICallback *pCallback, ReceiveMode rcvMode,
		SmartPtr<CConnection> & pOutConnector);

	//client side functions:

	//converts "CH" to system address SOCKADDR_IN.
	static BOOL ResolveHostNameToSocketAddress(char * lpszHostAddress, USHORT nHostPort, 
		SOCKADDR_IN &sockAddrOut);
	
	//Synchronously connects to server.
	int Connect(SOCKADDR_IN & SockAddr);

	//call this to disconnect connection. Typically, call release afterwards.
	//this fcn calls OnDisconnect and closes the socket.
	void Disconnect(void);

	//same as disconnect but immediately frees all resources and returns.
	void DisconnectAbortive(void);

	//Fcns for general use.
	int SendAsync(BYTE *pBuffer, ULONG ulSize);
	
	//status dump.
	CString Dump();
	static CString GlobalDump();

	//Static fcns for global initialization and cleanup.
	
	//Call before you begin any CConnection activity.
	static void GlobalInitialize(void);

	//Call when you're done with all CConnection activity.
	static void GlobalCleanup(void);

	static CSystemAllocator m_SystemAllocator;

	enum ConnectionType {ct_LISTENER_SOCKET, ct_ACCEPTOR_SOCKET, ct_NORMAL_SOCKET, 
		ct_CONNECTOR_SOCKET, ct_EMPTY};

	//Can use this variable for throttling.
	void GetStatistics(long *plNoAsyncSendsInWinsockQueue)
	{
		if (!plNoAsyncSendsInWinsockQueue)
			return;

		*plNoAsyncSendsInWinsockQueue = m_lNoAsyncSendsInWinsockQueue;
	}

#ifdef _DEBUG
	ConnectionType GetConnectionType(){ return m_ConnectType;}
#endif

private:

	//ctor/dtor. use createXXX calls above.
	CConnection();
	~CConnection();
	void * operator new(size_t size);
	
	//SmartPtr uses these.
	void AddRef();
	void Release();
	
	////////////////////////////////////////////////////////////////
	//Event firing functions.
	void _FireOnPreAccept(SmartPtr<CConnection> & pAcceptor, ICallback **ppCallback, ReceiveMode *pRcvMode)
	{
		ICallback * p = m_pCallback;
		if(p)
			p->OnPreAccept(pAcceptor, ppCallback, pRcvMode);
	}
	
	//Called when listener has accepted a new incoming connection.
	void _FireOnAccept(SmartPtr<CConnection> & pNewConnection, SmartPtr<CConnection> & pListener)
	{
		ICallback * p = m_pCallback;
		if(p)
			p->OnAccept(pNewConnection, pListener);
	}
	
	//Called to notify user of disconnect. Release your ref at this point.
	//The rule for this call is that it is only called in the ReadQueue code
	//at every point Disconnect is called.
	void _FireOnDisconnect(SmartPtr<CConnection> & pThis)
	{
		//this code is designed to only fire one OnDisconnect.
		//the entire function should only be called in
		//the readqueue synchronizatino code and is already threadsafe,
		//but for engineering reasons, i'm putting this extra threadsafety code in
		//since performance requirements for this function are minimal.
		ICallback* pCallback = m_pCallback;
		if(pCallback)
		{
			//try to set m_pCallback to NULL.
			ICallback* pRetCallback = 
				(ICallback*)InterlockedCompareExchange((void**)&m_pCallback, (void *)NULL, (void *)pCallback);
			if(pRetCallback == pCallback)
			{
				pCallback->OnDisconnect(pThis);
			}
		}
	}

	void _FireOnErrorNoBuffers(SmartPtr<CConnection> &pThis)
	{
		ICallback* p = m_pCallback;
		if(p)
			p->OnErrorNoBuffers(pThis);	
	}

	void _FireOnDataAvailable(SmartPtr<CConnection> & pThis, BYTE *pBuffer, ULONG ulSize)
	{
		ICallback* p = m_pCallback;
		if(p)
			p->OnDataAvailable(pThis, pBuffer, ulSize);
	}

	void _FireOnFramedDataAvailable(SmartPtr<CConnection> & pThis, BYTE *pBuffer, ULONG ulSize)
	{
		ICallback* p = m_pCallback;
		if(p)
			p->OnFramedDataAvailable(pThis, pBuffer, ulSize);
	}

	void _FireOnSendCompletion(SmartPtr<CConnection> & pThis, int iWinsockError, DWORD ulNumberOfBytesTransferred)
	{
		ICallback* p = m_pCallback;
		if(p)
			p->OnSendCompletion(pThis, iWinsockError, ulNumberOfBytesTransferred);
	}
	
	//Implement framer callback.
	void OnFramedData(ULONG ulCookie, BYTE *pBuffer, ULONG ulSizeOfBuffer)
	{
		SmartPtr<CConnection> pThis = (CConnection *)ulCookie;
		_FireOnFramedDataAvailable(pThis, pBuffer, ulSizeOfBuffer);
	}

	////////////////////////////////////////////////////////////////
	//helper fcns.

	//Helper trace function.
	void _Trace(CString str, DWORD dwError);

	//Fully initialize connection.
	int _Initialize(HANDLE hIoPort, ICallback *pCallback, ReceiveMode rcvMode);
	int _CreateAndAddAcceptor();

	int _SendAsync(BYTE *pBuffer, ULONG ulSize);

	//release calls delete. nobody else should.
	void operator delete(void *);

	//called by the listener socket to 
	//Initialize Acceptor, then informs this, a listener socket, of the new Acceptor.
	int _InformThisOfNewAcceptor(CConnection & Acceptor);

	//Callback processing.
	void _OnCallback(CDataExchanger *pData);
	void _ProcessData(CDataExchanger *pData);

	//change mode from acceptor to normal socket mode.
	int _ChangeToNormalModeAndInheritProperties(SmartPtr<CConnection> pListener);

	//fills the *pSocketAddress with the host computer's address with network
	//byte ordering. Used for listen().
	void _FillSocketAddress(USHORT nPort, PSOCKADDR_IN pSocketAddress);

	//call to make a fresh socket (with only create called above) a listener.
	int _InitAsListener(USHORT nPort);

	//Read queue management functions.
	int _StartReceiving(void);
	
	int _PostRcv(void);

	int _ProcessReadQueue(void);

	int _ProcessReceivedMessage(CDataExchanger *pData);

	//Data	
	
	//Read queue stuff.
	std::queue<CDataExchanger *> m_RecvQ;
	
	LONG m_lPendingRecvQueueTasks;

	ReceiveMode m_RcvMode;

	CMessageFramer m_MessageFramer;

	//these aren't used yet.
	//SOCKADDR m_LocalSockAddr;
	//SOCKADDR m_RemoteSockAddr;

	//type of connection.
	ConnectionType m_ConnectType;

	//socket.
	SOCKET m_hSocket;

	//socket-level callback for work q.
	//msg-level is cdataexchanger
	CConnection_Request m_RequestMsg;

	HANDLE m_hIOCompletionPort;
	
	long m_lRefCount;

	ICallback *m_pCallback;

	//This critical section is used to ensure that only
	//one thread sends to each connection at a time. We don't 
	//lock, but use TryEnter and fail the send and assert if it 
	//fails. If your app is written correctly, we should never
	//fail to TryEnter this semaphore.
	CSemExclusive m_SendSemaphore;

	//Statistics

	//m_ulNoAsyncSendsInWinsockQueue is the number of sends
	//we've tried to send on this connection, but haven't made it out of 
	//the TCP/IP stack. The size in bytes == BUFFER_SIZE * m_ulNoAsyncSendsInWinsockQueue.
	long m_lNoAsyncSendsInWinsockQueue;
	static long m_lTotalNoAsyncSendsInWinsockQueue;
	static long m_lMaxNoAsyncSendsInWinsockQueue;
	
	long m_lNoAsyncRecvsInWinsockQueue;
	
	//////////////////////////////////////////////////////////////////////////
	//Global statistics:
	
	//Total receives (nonframed) for all connections.
	static ULONG m_ulTotalReceiveCallbacks;
	
	//Total bytes received for all connections.
	static ULONG m_ulTotalReceivedBytes;

	//Total calls to SendAsync that succeeded.
	static ULONG m_ulTotalSendAsyncs;

	//Total sent bytes.
	static ULONG m_ulTotalSendAsyncBytes;

	//////////////////////////////////////////////////////////////////////////
	//Global pooled memory management stuff.
	static CLockFreeStackEx<CConnection *> m_AllConnectionsList;
	static CLockFreeStackEx<CConnection *> m_AvailConnectionsPool;
	
	//////////////////////////////////////////////////////////////////////////
	//Global thread pool.	
	static CWorkQueue *m_pWorkQ;	
	static CWaitUntilDoneSemaphore m_DestructionWaitSemaphore;
};

#endif

