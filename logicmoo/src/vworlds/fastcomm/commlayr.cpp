// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

/////////////////////////////////////////////////////////////////////////////////////////
// Project:		FastComm
// Module:		commlayr.cpp
// Description:	implements user classes: CConnection, ICallback plus internal classes.
// Author:		CH
// Create:		10/28/1999
/////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "commlayr.h"

////////////////////////////////////////////////////////////////////////////////////////////
//CDataExchanger methods////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
CLockFreeStackEx<CDataExchanger *> CDataExchanger::m_AvailCDataExchangerPool;
CSystemAllocator CDataExchanger::m_SystemAllocator;
long CDataExchanger::m_iTotalObjectsAllocated = 0;

CString CDataExchanger::GlobalDump()
{
	CString out = "CDataExchanger::GlobalDump():\r\n";
	out += "      Avail buffers in pool:     " + str(m_AvailCDataExchangerPool.getSize())+"\r\n";
	out += "      No of buffers in use:      " + str(m_iTotalObjectsAllocated - m_AvailCDataExchangerPool.getSize())+"\r\n";
	out += "      Total no of buffers:       " + str(m_iTotalObjectsAllocated)+"\r\n";

	return out;
}

CString CDataExchanger::Dump()
{
	CString out = "=============CDataExchanger: "+str((long)this)+".\r\n";
	out +="  This connection: "+m_pThisConnection->Dump()+"\r\n";
	switch(m_Type)
	{
		case bf_OnAccept:
			out+="  Type: Accept. " + CString("Acceptor: ")+m_pAcceptorConnection->Dump();
			break;
		case bf_OnReceive:
			out+="  Type: Receive.";
			break;
		case bf_Empty:
			out+="  Type: Empty.";
			break;
		default:
			ASSERT(0);
	}
	out+="\r\n===============\r\n";
	return out;
}

#ifndef DEBUG_ALLOCATORS
	void * CDataExchanger::operator new(size_t size)
	{
		CDataExchanger *p;
		bool b = m_AvailCDataExchangerPool.pop(p);
		if(b)
		{
			return (void*)p;
		}
		else
		{
			InterlockedIncrement(&m_iTotalObjectsAllocated);
			p = (CDataExchanger *)m_SystemAllocator.allocate(size);
			return p;
		}
	}

	void CDataExchanger::operator delete(void *p)
	{
		m_AvailCDataExchangerPool.push((CDataExchanger*)p);
	}
#endif

void CDataExchanger::GlobalCleanup(void)
{
	CDataExchanger *p;
	bool b;

	//deallocate all p's.
	b = m_AvailCDataExchangerPool.pop(p);
	while(b)
	{
		ASSERT(p);
		m_SystemAllocator.free(p);
		b = m_AvailCDataExchangerPool.pop(p);
	}

	//explicitly control lifetime of static objects.
	m_AvailCDataExchangerPool.~CLockFreeStackEx<CDataExchanger *>();
	m_SystemAllocator.~CSystemAllocator();
}

CDataExchanger::CDataExchanger()
{	
	//TRACE("Allocating DataExchanger: %x\n",this);
	m_dwFlags = 0;
	m_WSABuf.buf = (char *)&m_buffer;
	m_WSABuf.len = BUFFER_SIZE;
	FillMemory (&m_Overlapped,  sizeof(WSAOVERLAPPED), 0);
	m_pThisConnection = NULL;
	m_bProcessed = false;
}

CDataExchanger::~CDataExchanger()
{	
	//TRACE("  Destroying DataExchanger: %x\n",this);
	m_pAcceptorConnection = NULL;
	m_pThisConnection = NULL;
	m_Type = bf_Empty;
}

void CDataExchanger::InitializeConnect(SmartPtr<CConnection> pThisConnection, SmartPtr<CConnection> pAcceptor)
{
	m_pThisConnection = pThisConnection;	
	m_pAcceptorConnection = pAcceptor;
	m_Type = bf_OnAccept;
}

void CDataExchanger::InitializeRecv(SmartPtr<CConnection> pThisConnection)
{
	m_dwFlags = 0;
	m_pThisConnection = pThisConnection;
	m_Type = bf_OnReceive;
}

void CDataExchanger::InitializeMetaRecv(SmartPtr<CConnection> pThisConnection)
{
	m_pThisConnection = pThisConnection;
	m_Type = bf_OnMetaReceive;
}

void CDataExchanger::InitializeSendAsync(SmartPtr<CConnection> pThisConnection, BYTE *pbuffer, ULONG ulSize)
{
	ASSERT(ulSize<=BUFFER_SIZE);

	m_pThisConnection = pThisConnection;	
	m_Type = bf_SendAsync;
	m_WSABuf.len = ulSize;

	CopyMemory(&m_buffer, pbuffer, ulSize);
}

void CDataExchanger::CheckIsValid(void)
{
	#ifdef _DEBUG
		//check for access violations
		ASSERT(m_pThisConnection);
		ASSERT(m_Type == m_Type);
	#endif
}

////////////////////////////////////////////////////////////////////////////////////////////
// CMessageFramer
////////////////////////////////////////////////////////////////////////////////////////////
CMessageFramer::CMessageFramer()
{		
	m_pFramedBuffer = NULL;
	m_ulFramedBufferSize = 0;
	m_ulLengthByte = 0;
	m_pFramedBufferCurrentLocation = NULL;
}

CMessageFramer::~CMessageFramer()
{
	//let only one guy delete pFramedBuffer...do it after processing for
	//disconnect finishes.
	BYTE * p = m_pFramedBuffer;
	if(p)
	{
		BYTE *pRet = (BYTE *)InterlockedCompareExchange((void**)&m_pFramedBuffer, (void *)NULL, (void *)p);
		if(p == pRet)
		{
			m_SystemAllocator.free(p);
		}
	}
}

void CMessageFramer::FrameData(ULONG ulCookie, IMessageFramerCallback * pCallback, BYTE *pBuffer, ULONG ulNumberOfBytesTransferred)
{
	//process all data.
	while(ulNumberOfBytesTransferred>0)
	{
		if(m_ulLengthByte<sizeof(ULONG))
		{
			//read length.
			while(m_ulLengthByte<sizeof(ULONG) && ulNumberOfBytesTransferred>0)
			{
				m_strByte[m_ulLengthByte]=*pBuffer;
				//read byte into m_ulFramedBufferSize
				m_ulLengthByte++;
				ulNumberOfBytesTransferred--;
				pBuffer++;
			}		
			if(m_ulLengthByte==sizeof(ULONG))
			{				
				//finished read of length DWORD.
				m_ulFramedBufferSize = *((ULONG*)m_strByte);
				
				//assert less than 1/2 megabyte per buffer. if bigger
				//probably a misframing.
				ASSERT(m_ulFramedBufferSize<500000); 

				//allocate buffer.
				ASSERT(NULL==m_pFramedBuffer);
				
				m_pFramedBuffer = (BYTE *)m_SystemAllocator.allocate(m_ulFramedBufferSize);
				
				m_pFramedBufferCurrentLocation = m_pFramedBuffer;
				
				m_ulFramedBufferSizeLeft = m_ulFramedBufferSize;
			}
		}
		else
		{		
			ASSERT(m_pFramedBuffer);

			if(ulNumberOfBytesTransferred>=m_ulFramedBufferSizeLeft)
			{
				//more transferred than buffer size.
				CopyMemory(m_pFramedBufferCurrentLocation, pBuffer, m_ulFramedBufferSizeLeft);
				
				//execute callback.
				// user deletes m_pFramedBuffer.
				pCallback->OnFramedData(ulCookie, m_pFramedBuffer, 
					m_ulFramedBufferSize);			

				pBuffer += m_ulFramedBufferSizeLeft;
				ulNumberOfBytesTransferred -= m_ulFramedBufferSizeLeft;				
				
				m_pFramedBuffer = NULL;		
				m_ulFramedBufferSize = 0;
				m_ulLengthByte = 0;
				m_pFramedBufferCurrentLocation = 0;
				m_ulFramedBufferSizeLeft = 0;					
			}		
			else
			{
				// ulNumberOfBytesTransferred<m_ulFramedBufferSize
				CopyMemory(m_pFramedBufferCurrentLocation, pBuffer, ulNumberOfBytesTransferred);
			
				pBuffer += ulNumberOfBytesTransferred;

				m_ulFramedBufferSizeLeft -= ulNumberOfBytesTransferred;
				ASSERT(m_ulFramedBufferSizeLeft>0);

				m_pFramedBufferCurrentLocation += ulNumberOfBytesTransferred;
				ASSERT(m_pFramedBufferCurrentLocation < m_pFramedBuffer + m_ulFramedBufferSize);

				ulNumberOfBytesTransferred = 0;
			}
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////
// CConnection
////////////////////////////////////////////////////////////////////////////////////////////
CLockFreeStackEx<CConnection *> CConnection::m_AllConnectionsList;
CLockFreeStackEx<CConnection *> CConnection::m_AvailConnectionsPool;
CSystemAllocator CConnection::m_SystemAllocator;
CWaitUntilDoneSemaphore CConnection::m_DestructionWaitSemaphore;
CWorkQueue * CConnection::m_pWorkQ;	
ULONG CConnection::m_ulTotalReceiveCallbacks = 0;
ULONG CConnection::m_ulTotalReceivedBytes = 0;
ULONG CConnection::m_ulTotalSendAsyncs = 0;
ULONG CConnection::m_ulTotalSendAsyncBytes = 0;
long CConnection::m_lTotalNoAsyncSendsInWinsockQueue = 0;
long CConnection::m_lMaxNoAsyncSendsInWinsockQueue = 0;
CSystemAllocator CMessageFramer::m_SystemAllocator;
void CConnection::AddRef()
{
	//TRACE("Addref'ing: "+Dump());
	InterlockedIncrement(&m_lRefCount);
}

void CConnection::Release()
{
	//TRACE("Releasing: "+Dump());
	long lCurr = InterlockedDecrement(&m_lRefCount);
	if(lCurr<0)
		ASSERT(0);
	if(lCurr==0)
	{
		delete this;
	}
}

void* CConnection::operator new(size_t size)
{
	CConnection *p;
	m_DestructionWaitSemaphore.Increment();

	bool b = m_AvailConnectionsPool.pop(p);
	if(b)
	{
		return (void*)p;
	}
	else
	{
		p = (CConnection *)m_SystemAllocator.allocate(size);
		m_AllConnectionsList.push(p);
		return p;
	}		
}

void CConnection::operator delete(void *p)
{
	m_AvailConnectionsPool.push((CConnection*)p);
	m_DestructionWaitSemaphore.Decrement();
}

void CConnection::GlobalCleanup(void)
{
	CConnection *p;
	bool b;

	//Cleanup Worker Threads.

	//first wait for user should disconnect all listeners.
	if(WAIT_TIMEOUT==CConnection::m_DestructionWaitSemaphore.WaitUntilZeroCount(60000))
	{
		//there are outstanding connections not cleaned up (leaking).
		//this is probably caused by clients of fastcomm (not fastcomm itself).
		ASSERT(0);
	}

	//delete q.
	ASSERT(m_pWorkQ);
	delete m_pWorkQ;

	//Cleanup all connections.
	ASSERT(m_AvailConnectionsPool.getSize() == m_AllConnectionsList.getSize());

	//deallocate all p's.
	b = m_AllConnectionsList.pop(p);
	while(b)
	{
		ASSERT(p);
		if(p->m_ConnectType!=ct_EMPTY)
		{
			TRACE(p->Dump());
		}
		m_SystemAllocator.free(p);
		b = m_AllConnectionsList.pop(p);
	}

	//explicitly control lifetime of static objects.
	m_AvailConnectionsPool.~CLockFreeStackEx<CConnection *>();
	m_AllConnectionsList.~CLockFreeStackEx<CConnection *>();
	m_SystemAllocator.~CSystemAllocator();

	//Cleanup CDataExchanger.
	CDataExchanger::GlobalCleanup();	

	//Cleanup stackpool.
	CLockFreeStackPool::CStackPoolNode::GlobalCleanup();		

	//Cleanup framer.
	CMessageFramer::GlobalCleanup();
}

void CConnection::_Trace(CString strDescr, DWORD dwError)
{
	OutputDebugString("Error "+str(dwError)+". "+strDescr+"\r\n"+Dump());
}

void CConnection::_ProcessData(CDataExchanger *pData)
{
	ASSERT(pData);

	//TRACE(pData->Dump());
	//check type of this:
	#ifdef _DEBUG
		switch(m_ConnectType)
		{
			case ct_LISTENER_SOCKET:
				ASSERT(pData->m_Type == CDataExchanger::bf_OnAccept);
				break;
			case ct_ACCEPTOR_SOCKET:
				ASSERT(0);
				break;
			case ct_NORMAL_SOCKET:
				ASSERT(pData->m_Type == CDataExchanger::bf_OnReceive || 
					pData->m_Type == CDataExchanger::bf_SendAsync);
				break;
			case ct_CONNECTOR_SOCKET:
				ASSERT(pData->m_Type == CDataExchanger::bf_OnReceive || 
					pData->m_Type == CDataExchanger::bf_SendAsync);
				break;
			case ct_EMPTY:
				ASSERT(pData->m_Type == CDataExchanger::bf_OnReceive ||
					pData->m_Type == CDataExchanger::bf_OnAccept ||
					pData->m_Type == CDataExchanger::bf_SendAsync);
				//ASSERT(0);
				break;
			default:
				ASSERT(0);
		}
	#endif
	
	BOOL bRet;
	DWORD lNumberOfBytesTransferred;
	int iRet = 0;
	DWORD dwFlags;
	DWORD dwError;

	//process data request.
	switch(pData->m_Type)
	{
		case CDataExchanger::bf_OnAccept:
			ASSERT(pData->m_pAcceptorConnection);
			ASSERT(pData->m_pAcceptorConnection->m_ConnectType==ct_ACCEPTOR_SOCKET ||
				pData->m_pAcceptorConnection->m_ConnectType==ct_EMPTY);

			//check if Accept is an error.
			bRet = WSAGetOverlappedResult(m_hSocket,
				&pData->m_Overlapped, &lNumberOfBytesTransferred, FALSE, &dwFlags);

			//is this an error?	
			if((bRet == 0))
			{
				dwError = GetLastError();
				ASSERT(dwError == WSA_OPERATION_ABORTED || dwError == WSAENOTSOCK);

				//_Trace("CConnection::_ProcessData: Accept WSAGetOverlappedResult error.", dwError);

				//listener is finished. cleanup listener.
				Disconnect();

				//acceptor is finished. clean up acceptor.
				pData->m_pAcceptorConnection->Disconnect();
			}
			else
			{
				//add a new acceptor.
				_CreateAndAddAcceptor();				
				
				int iRet = pData->m_pAcceptorConnection->_ChangeToNormalModeAndInheritProperties(this);				
				if(iRet)
				{
					ASSERT(iRet == WSAECONNRESET || iRet == WSAENOTSOCK || iRet == WSAENOTCONN 
						|| iRet == WSAENOBUFS);
					//do nothing. msg has already been posted.
				}
				else
				{
					pData->m_pAcceptorConnection->_FireOnAccept
						(pData->m_pAcceptorConnection, SmartPtr<CConnection>(this));				
				}
			}	

			delete pData;

			break;
		case CDataExchanger::bf_OnReceive:
			InterlockedIncrement((long*)&m_ulTotalReceiveCallbacks);
			InterlockedDecrement((long*)&m_lNoAsyncRecvsInWinsockQueue);
			
			pData->SetProcessed();

			iRet = _ProcessReadQueue();

			//pdata is released in _ProcessReadQueue appropriately.

			break;
		case CDataExchanger::bf_SendAsync:
			dwError = 0;

			bRet = WSAGetOverlappedResult(m_hSocket,
				&pData->m_Overlapped, &lNumberOfBytesTransferred, FALSE, &dwFlags);
			if(bRet==0)
			{
				dwError = GetLastError();
				ASSERT(dwError == WSAECONNRESET || dwError == WSAECONNABORTED
					|| dwError == WSAENOTSOCK || dwError == ERROR_OPERATION_ABORTED 
					|| dwError == ERROR_NETNAME_DELETED //NT 4.0 bug workaround.
					);

				//There was an error in the read.
				//The socket's down.
				_FireOnSendCompletion(SmartPtr<CConnection>(this), dwError, 0);

				//_Trace("CConnection::_ProcessData: Send WSAGetOverlappedResult error.", dwError);

				Disconnect();
			}
			else if(lNumberOfBytesTransferred==0)
			{
				//The socket's gracefully shutdown.
				_FireOnSendCompletion(SmartPtr<CConnection>(this), dwError, lNumberOfBytesTransferred);

				//_Trace("CConnection::_ProcessData: Send lNumberOfBytesTransferred==0 error.", dwError);

				Disconnect();
			}						
			else
			{
				InterlockedDecrement(&m_lNoAsyncSendsInWinsockQueue);
				InterlockedDecrement(&m_lTotalNoAsyncSendsInWinsockQueue);
//				char buff[200];
//				sprintf(buff, "Decrement. m_lTotalNoAsyncSendsInWinsockQueue = %d, m_lMaxNoAsyncSendsInWinsockQueue = %d", m_lTotalNoAsyncSendsInWinsockQueue, m_lMaxNoAsyncSendsInWinsockQueue);
//				_Trace(buff, 0);
				_FireOnSendCompletion(SmartPtr<CConnection>(this), dwError, lNumberOfBytesTransferred);
			}

			delete pData;

			break;
		default:
			ASSERT(0);
			break;
	}
}

int CConnection::SendAsync(BYTE *pBuffer, ULONG ulSize)
{
	BOOL bRet = m_SendSemaphore.TryLock();
	if(!bRet)
	{
		//two threads sending to one socket at a time.
		//doesn't make sense for tcp/ip.
		ASSERT(0);
		//_Trace("CConnection::SendAsync: More than one thread is sending to one socket simultaneously. Doesn't make sense for TcpIp.",-1);
		return -1;
	}

	int iRet = 0;

	if(INVALID_SOCKET == m_hSocket)
	{
		m_SendSemaphore.Unlock();
		return WSAENOTSOCK;
	}
	else
	{
		if(ulSize==0)
		{
			m_SendSemaphore.Unlock();
			return 0;
		}
	}

	if(m_RcvMode == recv_Framed)
	{	
		//send length first.
		BYTE buf[4];
		*((ULONG*)buf) = ulSize;
		iRet = _SendAsync(buf, sizeof(ULONG));
		if(iRet) 
		{
			m_SendSemaphore.Unlock();
			return iRet;
		}
	}

	//send user buffer in chunks of BUFFER_SIZE;
	ULONG ulCurrentSize;

	// ulSize is what's left to copy.
	while(ulSize>0)
	{
		if(ulSize>BUFFER_SIZE)
		{
			ulCurrentSize = BUFFER_SIZE;
		}
		else
		{
			ulCurrentSize = ulSize;
		}

		//send pdata.
		iRet = _SendAsync(pBuffer, ulCurrentSize);
		if(iRet) 
		{
			m_SendSemaphore.Unlock();
			return iRet;
		}

		//change counters to new data.
		ulSize -= ulCurrentSize;
		pBuffer += ulCurrentSize;
	}

	InterlockedIncrement((long*)&m_ulTotalSendAsyncs);

	m_SendSemaphore.Unlock();
	return iRet;
}

int CConnection::_SendAsync(BYTE *pBuffer, ULONG ulSize)
{
	int iRet = 1;

	if(INVALID_SOCKET == m_hSocket)
	{
		return WSAENOTSOCK;
	}

	//allocate buffer.
	CDataExchanger *pData = new CDataExchanger;
			
	//copy buffer.
	pData->InitializeSendAsync(this, pBuffer, ulSize);

	//implement an exponential backoff technique to deal with 
	//WSANOBUFs error. each time it fails, double the wait time.
	//the initial iWaitTime should equal the minimum time
	//you think Winsock will take to clear the buffers
	//after calling _FireOnErrorNoBuffers.
	int iWaitTime = INITIAL_BACKOFF_WAIT_TIME; //ms.
	while(iRet)
	{
		iRet = WSASend(m_hSocket, &pData->m_WSABuf, 1, &pData->m_dwNoBytesRead,
			0, &pData->m_Overlapped, 0);

		// Before we send, make sure we haven't backed up sends in WinSock greater than
		// our threshold (this should avoid us ever getting WSAENOBUFS).
		if(iRet || m_lTotalNoAsyncSendsInWinsockQueue > m_lMaxNoAsyncSendsInWinsockQueue)
		{
			iRet = WSAGetLastError();

			if(WSAENOBUFS == iRet || m_lTotalNoAsyncSendsInWinsockQueue > m_lMaxNoAsyncSendsInWinsockQueue)
			{
				if(iWaitTime > MAXIMUM_BACKOFF_WAIT_TIME )
				{
					delete pData;

					char buff[200];
					sprintf(buff, "CConnection::_SendAsync: Send timeout error. m_lTotalNoAsyncSendsInWinsockQueue = %d, m_lMaxNoAsyncSendsInWinsockQueue = %d, Time = %d", m_lTotalNoAsyncSendsInWinsockQueue, m_lMaxNoAsyncSendsInWinsockQueue, GetTickCount());
					_Trace(buff, iRet);

					DisconnectAbortive();

					//don't increment counts.
					return iRet;
				}
				else
				{
					char buff[200];
					sprintf(buff, "CConnection::_SendAsync: trying to recover. m_lTotalNoAsyncSendsInWinsockQueue = %d, m_lMaxNoAsyncSendsInWinsockQueue = %d, Time = %d", m_lTotalNoAsyncSendsInWinsockQueue, m_lMaxNoAsyncSendsInWinsockQueue, GetTickCount());
					_Trace(buff, iRet);
					
					//call client to try to release buffers.
					_FireOnErrorNoBuffers(SmartPtr<CConnection>(this));

					//wait.
					Sleep(iWaitTime);
					
					//double the time for the next wait. (exponential backoff).
					iWaitTime *= 2;
				}
			}
			else if(ERROR_IO_PENDING == iRet)
			{
				iRet = 0;
			}
			else 
			{
				ASSERT(WSAENOTSOCK == iRet || WSAECONNABORTED==iRet || WSAENOTCONN == iRet || 
					WSAECONNRESET == iRet || WSAEINVAL == iRet);
				delete pData;
				
				//_Trace("CConnection::_SendAsync: WSASend error.", iRet);

				Disconnect();

				//don't increment counts.
				return iRet;
			}
		}
		else
		{
			//iret == 0. Quitting.
		}
	}

	InterlockedIncrement(&m_lNoAsyncSendsInWinsockQueue);
	InterlockedIncrement(&m_lTotalNoAsyncSendsInWinsockQueue);
//	char buff[200];
//	sprintf(buff, "Increment. m_lTotalNoAsyncSendsInWinsockQueue = %d, m_lMaxNoAsyncSendsInWinsockQueue = %d", m_lTotalNoAsyncSendsInWinsockQueue, m_lMaxNoAsyncSendsInWinsockQueue);
//	_Trace(buff, 0);
	InterlockedExchangeAdd((long*)&m_ulTotalSendAsyncBytes, ulSize);

	return iRet;
}


CConnection::CConnection()
{
	//TRACE("Constructing CConnection: "+str((void *)this)+"\r\n");
	m_lRefCount = 0;
	m_ConnectType = ct_EMPTY;
	m_hSocket = INVALID_SOCKET;
	m_pCallback = NULL;
	m_lPendingRecvQueueTasks = 0;
	m_lNoAsyncSendsInWinsockQueue = 0;
	m_lNoAsyncRecvsInWinsockQueue = 0;
	if (m_lMaxNoAsyncSendsInWinsockQueue == 0)
	{
		MEMORYSTATUS memstatus;
		GlobalMemoryStatus(&memstatus);
		// Since our BUFFER_SIZE <= 4K WinSock will lock 4K for each buffer we give to
		// WSASend (WinSock has to lock a whole 4K page at a time). Our application will
		// have access to 1/4 of the total available physical pages. So our absolute maximum
		// no of unconfirmed sends (i.e. buffers/pages locked by WinSock) is 1/16 of the
		// total available physical memory. We will allow 75% of this for safety.
		m_lMaxNoAsyncSendsInWinsockQueue = (memstatus.dwTotalPhys/16384)*0.75;
	}
}

CConnection::~CConnection()
{
	//TRACE("  Cleaning up CConnection: %x\n",this);
	Disconnect();	
}

int CConnection::_Initialize(HANDLE hIoPort, ICallback *pCallback, ReceiveMode rcvMode)
{
	m_hIOCompletionPort = hIoPort;
	m_pCallback = pCallback;
	m_RcvMode = rcvMode;

	m_hSocket = socket( AF_INET, SOCK_STREAM, 0);	
    if ((m_hSocket  == INVALID_SOCKET))
		ASSERT(0);

	//create request type.
	m_RequestMsg.Initialize(this);	

	m_ConnectType =  ct_EMPTY;

	// advise the IO Completion Port of new file or socket.
	HANDLE h = CreateIoCompletionPort((HANDLE)m_hSocket, hIoPort, (ULONG) &m_RequestMsg, 0 );
	if(h==0)
		return GetLastError();
	else return 0;
}

//call to make a fresh socket a listener.
int CConnection::_InitAsListener(USHORT nPort)
{
	int iRet = 0;
	SOCKADDR_IN SocketAddress;  

	//set type to listen.
	if((void *)ct_EMPTY != InterlockedCompareExchange(
		(void **)&m_ConnectType, 
		(void *)ct_LISTENER_SOCKET, 
		(void *)ct_EMPTY))
		ASSERT(0);

	//   Retrieve the IP address and TCP Port number
	_FillSocketAddress(nPort, &SocketAddress);

	//   Associate an address with the socket
	if (bind(m_hSocket, (struct sockaddr FAR *) &SocketAddress, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) 
	{			
		return WSAGetLastError();
	}

	//   And go into the listening state.       
	if (listen(m_hSocket, SOMAXCONN) < 0) 
	{
		return WSAGetLastError();
	}

	return iRet;
}

int CConnection::Connect(SOCKADDR_IN & SockAddr)
{
	int iRet = 0;

	if((void *)ct_EMPTY != InterlockedCompareExchange(
		(void **)&m_ConnectType, 
		(void *)ct_CONNECTOR_SOCKET, 
		(void *)ct_EMPTY))
		ASSERT(0);

	if(connect(m_hSocket, (const  SOCKADDR *)&SockAddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		iRet = WSAGetLastError(); 		

		//_Trace("CConnection::Connect: Connect error.", iRet);

		Disconnect();
		return iRet;
	}

	return _StartReceiving();
}

int CConnection::_StartReceiving(void)
{
	//set the send and receive buffers to zero
	//to eliminate winsock buffer copies.
	//verify with getsockopt.
	int iBuf, iBuflen, iRet;

	iBuf = 0;
	if(setsockopt(m_hSocket, SOL_SOCKET, SO_SNDBUF, 
		(char *)&iBuf, sizeof(iBuf)))
	{
		return WSAGetLastError();
	}

	iBuf = 0;
	if(setsockopt(m_hSocket, SOL_SOCKET, SO_RCVBUF, 
		(char *)&iBuf, sizeof(iBuf)))
	{
		return WSAGetLastError();
	}

	iBuflen = sizeof(iBuf);
	if(iRet = getsockopt(m_hSocket, SOL_SOCKET, SO_RCVBUF, (char *)&iBuf, &iBuflen))
	{
		return WSAGetLastError();
	}

	ASSERT(iBuf == 0 && iBuflen==sizeof(iBuf));
	if(iBuf!=0 || iBuflen!=sizeof(iBuf))
		return 1;

	iBuflen = sizeof(iBuf);
	if(iRet = getsockopt(m_hSocket, SOL_SOCKET, SO_SNDBUF, (char *)&iBuf, &iBuflen))
	{
		return WSAGetLastError();
	}

	ASSERT(iBuf == 0 && iBuflen==sizeof(iBuf));
	if(iBuf!=0 || iBuflen!=sizeof(iBuf))
		return 1;

	//place requests in queue to initialize receives.
	for(long i = 0; 
		i < g_SystemInfo.dwNumberOfProcessors * NUMBER_OF_QUEUED_RECVS_PER_CONNECTION_PER_PROCESSOR; 
		i++)
	{
		CDataExchanger *pRecvData = new CDataExchanger;
		pRecvData->InitializeMetaRecv(this);
		
		//place	it in the queue.
		m_RecvQ.push(pRecvData);
	}

	return _ProcessReadQueue();
}

int CConnection::_CreateAndAddAcceptor()
{		
	int iRet = 0;
	ICallback *pCallback = m_pCallback;
	ReceiveMode RcvMode = m_RcvMode;

	SmartPtr<CConnection> pAcceptor = new CConnection;

	_FireOnPreAccept(pAcceptor, &pCallback, &RcvMode);

	iRet = pAcceptor->_Initialize(m_hIOCompletionPort, pCallback, RcvMode);
	if(iRet)
	{
		return iRet;
	}

	iRet = _InformThisOfNewAcceptor(*pAcceptor);
	if(iRet)
	{
		return iRet;
	}
	
	return iRet;
}

CString CConnection::Dump()
{
	CString out;

	out += "Connection dump: \r\n";
	if(m_hSocket==INVALID_SOCKET)
	{
		out+="Invalid socket: \r\n";
	}
	else
	{
		out+="Socket ID: "+str((ULONG)m_hSocket)+"\r\n";
	}
	
	switch(m_ConnectType)
	{
		case ct_LISTENER_SOCKET:
			out+="  Listener.";
			break;
		case ct_ACCEPTOR_SOCKET:
			out+="  Acceptor.";
			break;
		case ct_NORMAL_SOCKET:
			out+="  Normal.";
			break;
		case ct_CONNECTOR_SOCKET:
			out+="  Connector.";
			break;
		case ct_EMPTY:
			out+="  Empty.";
			break;
		default:
			ASSERT(0);
	}
	out += "  Address: "+str((void *)this);
	out += "  Ref count: "+str(m_lRefCount) + "\r\n";
	out += "Sends in WinsockQueue: " + str(m_lNoAsyncSendsInWinsockQueue) + "\r\n";
	return out;
}

//Initializes Acceptor.
//Then informs this, a listener socket, of the new Acceptor.
int CConnection::_InformThisOfNewAcceptor(CConnection & Acceptor)
{
	DWORD dwBytes = 0;
	int iRet = 0;

	ASSERT(m_ConnectType == ct_LISTENER_SOCKET);

	//set type.
	if((void *)ct_EMPTY != InterlockedCompareExchange(
		(void **)&Acceptor.m_ConnectType, 
		(void *)ct_ACCEPTOR_SOCKET, 
		(void *)ct_EMPTY))
		ASSERT(0);

	//allocate buffer.
	CDataExchanger *pData = new CDataExchanger;
	ASSERT(Acceptor.m_ConnectType==ct_ACCEPTOR_SOCKET);
	pData->InitializeConnect(this, &Acceptor);

	pData->CheckIsValid();

	if (AcceptEx(m_hSocket, Acceptor.m_hSocket, pData->m_buffer, 0, 
		sizeof(SOCKADDR_IN)+16, sizeof(SOCKADDR_IN)+16,
		&dwBytes, &pData->m_Overlapped) == FALSE) 
	{
		int iRet = WSAGetLastError();
		ASSERT(iRet == ERROR_IO_PENDING || iRet == WSAENOTSOCK);
		if (iRet != ERROR_IO_PENDING)
		{
			//_Trace("CConnection::_InformThisOfNewAcceptor: AcceptEx error.", iRet);

			Disconnect();
		}
	}

	return iRet;
}

void CConnection::_OnCallback(CDataExchanger *pData)
{
	_ProcessData(pData);
}

//change mode from acceptor to normal socket mode.
int CConnection::_ChangeToNormalModeAndInheritProperties(SmartPtr<CConnection> pListener)
{
	//set type.
	if((void *)ct_ACCEPTOR_SOCKET != InterlockedCompareExchange(
		(void **)&m_ConnectType, 
		(void *)ct_NORMAL_SOCKET, 
		(void *)ct_ACCEPTOR_SOCKET))
		ASSERT(0);

	if(m_hSocket==INVALID_SOCKET)
		return WSAENOTCONN;

	//change socket from acceptor to normal socket.
	if (setsockopt(m_hSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char *)&pListener->m_hSocket,
		sizeof(pListener->m_hSocket) )) 
	{
		return WSAGetLastError();
	} 

/*	int LocalSockaddrLength;
	int RemoteSockaddrLength;

	//parse data
	GetAcceptExSockaddrs (pData->m_buffer, BUFFER_SIZE,  
		sizeof(SOCKADDR_IN)+16, sizeof(SOCKADDR_IN)+16, 
		&m_LocalSockAddr, &LocalSockaddrLength,  
		&m_RemoteSockAddr, &RemoteSockaddrLength);
*/

	return _StartReceiving();
}

//not threadsafe. you need to sync access to this.
//Fires callback if necessary. Disconnects if necessary.
int CConnection::_ProcessReceivedMessage(CDataExchanger *pData)
{
	BOOL bRet;
	DWORD lNumberOfBytesTransferred;
	int iRet = 0;
	DWORD dwFlags;
	DWORD dwError;

	if(m_hSocket==INVALID_SOCKET)
	{
		_FireOnDisconnect(SmartPtr<CConnection>(this));

		return WSAENOTCONN;
	}

	bRet = WSAGetOverlappedResult(m_hSocket,
		&pData->m_Overlapped, &lNumberOfBytesTransferred, FALSE, &dwFlags);
	if(bRet==0)
	{
		dwError = GetLastError();
		ASSERT(dwError == WSAECONNRESET || dwError == WSAECONNABORTED
			|| dwError == WSAENOTSOCK || dwError == ERROR_OPERATION_ABORTED);

		//There was an error in the read.
		//_Trace("CConnection::_ProcessReceivedMessage: WSAGetOverlappedResult error.", dwError);
		
		//The socket's down.
		Disconnect();
		_FireOnDisconnect(SmartPtr<CConnection>(this));
	}
	else if(lNumberOfBytesTransferred==0)
	{
		//_Trace("CConnection::_ProcessReceivedMessage: lNumberOfBytesTransferred==0.", 0);

		//The socket's gracefully shutdown.
		Disconnect();
		_FireOnDisconnect(SmartPtr<CConnection>(this));
	}
	else
	{
		InterlockedExchangeAdd((long*)&m_ulTotalReceivedBytes, lNumberOfBytesTransferred);
		if(m_RcvMode == recv_Streaming)
		{
			_FireOnDataAvailable(SmartPtr<CConnection>(this), pData->m_buffer, lNumberOfBytesTransferred);
		}
		else if(m_RcvMode == recv_Framed)
		{
			_FireOnDataAvailable(SmartPtr<CConnection>(this), pData->m_buffer, lNumberOfBytesTransferred);
			m_MessageFramer.FrameData((ULONG)this, this, pData->m_buffer, lNumberOfBytesTransferred);
		}
		else 
			ASSERT(0);
	}

	return iRet;
}

int CConnection::_ProcessReadQueue(void)
{		
	LONG lRet;
	bool bKeepGoing;
	BOOL bRet;
	CDataExchanger *pCurrentCDE = NULL;
	int iRet = 0;

	//distributed lockfree design here. counted critical section.
	lRet = InterlockedIncrement((long*)&m_lPendingRecvQueueTasks);
	ASSERT(lRet<10000);
	if(lRet==1) 
	{
		//this thread is the designated worker thread.
		//entering lock lRet times.
		do
		{
			//keep looping while there is something legitimate to processs:
			//  eg. receive and completed OR metaReceive.
			bKeepGoing = true;		
			while(bKeepGoing)
			{
				if(m_hSocket==INVALID_SOCKET)
				{
					_FireOnDisconnect(SmartPtr<CConnection>(this));
				}

				if(!m_RecvQ.empty())
				{
					pCurrentCDE = m_RecvQ.front();
	
					if(pCurrentCDE->m_Type==CDataExchanger::bf_OnReceive)
					{
						pCurrentCDE = m_RecvQ.front();
					
						//a normal receive.
						//bRet = HasOverlappedIoCompleted(&pCurrentCDE->m_Overlapped);
						bRet = pCurrentCDE->m_bProcessed;
						if(bRet)
						{
							m_RecvQ.pop();
				
							//start another receive.
							iRet = _PostRcv();

							//no error.
							iRet = _ProcessReceivedMessage(pCurrentCDE);

							delete pCurrentCDE;
						}
						else
						{
							bKeepGoing = false;
						}										
					}
					else if(pCurrentCDE->m_Type==CDataExchanger::bf_OnMetaReceive)
					{
						m_RecvQ.pop();
						delete pCurrentCDE;

						iRet = _PostRcv();
					}
					else
						ASSERT(0);
				}
				else
				{
					bKeepGoing = false;
				}										
			}	//end iteration of queue.
			
			lRet = InterlockedDecrement((long*)&m_lPendingRecvQueueTasks);
			ASSERT(lRet>=0);
		}
		while (lRet>0);
	}		
	return iRet;
}

int CConnection::_PostRcv(void)
{
	if(m_hSocket==INVALID_SOCKET)
	{
		_FireOnDisconnect(SmartPtr<CConnection>(this));
		return WSAENOTCONN;
	}
	
	//allocate and initialize buffer.
	CDataExchanger *pRecvData = new CDataExchanger;
	pRecvData->InitializeRecv(this);
	pRecvData->CheckIsValid();
	
	int iRet = 1;
	int iWaitTime = INITIAL_BACKOFF_WAIT_TIME;

	while(iRet)
	{
		iRet = 0;

		//start listening.
		if(WSARecv(m_hSocket, &pRecvData->m_WSABuf, 1, &pRecvData->m_dwNoBytesRead,
			&pRecvData->m_dwFlags, &pRecvData->m_Overlapped, NULL))
		{
			iRet = WSAGetLastError();
			if(iRet)
			{
				if(iRet == ERROR_IO_PENDING)
				{
					//it's okay.
					iRet = 0;
				}
				else 
				{
					ASSERT(iRet == WSAECONNRESET || iRet == WSAENOTCONN || iRet == WSAESHUTDOWN
					|| iRet == WSAECONNABORTED || iRet == WSAENOBUFS);
					//There was an error in the read.
					//_Trace("CConnection::_PostRcv: WSARecv error.", iRet);
					if(iRet == WSAENOBUFS && iWaitTime < MAXIMUM_BACKOFF_WAIT_TIME)
					{
						char buff[200];
						sprintf(buff, "CConnection::_PostRcv: trying to recover. Time = %d", GetTickCount());
						_Trace(buff, iRet);
					
						//call client to try to release buffers.
						_FireOnErrorNoBuffers(SmartPtr<CConnection>(this));

						Sleep(iWaitTime);
						iWaitTime *= 2;
					}
					else
					{					
						char buff[200];
						sprintf(buff, "CConnection::_PostRcv: Error. Time = %d", GetTickCount());
						_Trace(buff, iRet);

						//The socket's down.
						DisconnectAbortive();
						_FireOnDisconnect(SmartPtr<CConnection>(this));
			
						delete pRecvData;
						
						return iRet;
					}
				}
			}
		}
	}
	
	m_RecvQ.push(pRecvData);
	InterlockedIncrement((long*)&m_lNoAsyncRecvsInWinsockQueue);

	return iRet;
}

//fills the *pSocketAddress with the host computer's address with network
//byte ordering.
void CConnection::_FillSocketAddress(USHORT nPort, PSOCKADDR_IN pSocketAddress)
{
	memset(pSocketAddress, 0, sizeof(PSOCKADDR_IN));

	pSocketAddress->sin_family = AF_INET;

	pSocketAddress->sin_addr.s_addr = INADDR_ANY;

	// Convert to network ordering
	pSocketAddress->sin_port = htons(nPort);        
}

CString CConnection::GlobalDump()
{
	CString out = "CConnection::GlobalDump::\r\n";
	out += "      Avail connections in pool:   " + str(m_AvailConnectionsPool.getSize())+"\r\n";
	out += "      No of connections in use:    " + str(m_AllConnectionsList.getSize() - m_AvailConnectionsPool.getSize())+"\r\n";
	out += "      Total no of connections:     " + str(m_AllConnectionsList.getSize())+"\r\n";
	out += "      Total received bytes: "+str(m_ulTotalReceivedBytes)+"\r\n";
	out += "      Total receive callbacks: "+str(m_ulTotalReceiveCallbacks)+"\r\n";
	out += "      Total sent bytes: "+str(m_ulTotalSendAsyncBytes)+"\r\n";
	out += "      Total calls to sendasync: "+str(m_ulTotalSendAsyncs)+"\r\n";

	out += CDataExchanger::GlobalDump();
	out += m_pWorkQ->Dump();

	return out;
}

void CConnection::Disconnect(void)
{
	//this logic ensures only one thread closes the socket.
	SOCKET hSocket = m_hSocket;
	if(hSocket!=INVALID_SOCKET)
	{
		//try to set m_hSocket to INVALID_SOCKET.
		SOCKET hRetSocket = 
			(SOCKET)InterlockedCompareExchange((void**)&m_hSocket, (void *)INVALID_SOCKET, (void *)hSocket);
		if(hRetSocket == hSocket)
		{
			//TRACE("Disconnecting: "+Dump());

			ASSERT(hRetSocket!=INVALID_SOCKET);
			//winsock recommends this careful shutdown.
			//stop sends.
			shutdown(hSocket, SD_SEND);
			//stop both.
			shutdown(hSocket, SD_BOTH);
			//close socket.
			closesocket(hSocket);

			_ProcessReadQueue();

			m_ConnectType = ct_EMPTY;

			InterlockedExchangeAdd(&m_lTotalNoAsyncSendsInWinsockQueue, 0-m_lNoAsyncSendsInWinsockQueue);
//			char buff[200];
//			sprintf(buff, "Decrement(disconnect). m_lTotalNoAsyncSendsInWinsockQueue = %d, m_lMaxNoAsyncSendsInWinsockQueue = %d", m_lTotalNoAsyncSendsInWinsockQueue, m_lMaxNoAsyncSendsInWinsockQueue);
//			_Trace(buff, 0);
			m_lNoAsyncSendsInWinsockQueue = 0;
		}
	}
}

void CConnection::DisconnectAbortive(void)
{
	//this logic ensures only one thread closes the socket.
	SOCKET hSocket = m_hSocket;
	if(hSocket!=INVALID_SOCKET)
	{
		//try to set m_hSocket to INVALID_SOCKET.
		SOCKET hRetSocket = 
			(SOCKET)InterlockedCompareExchange((void**)&m_hSocket, (void *)INVALID_SOCKET, (void *)hSocket);
		if(hRetSocket == hSocket)
		{
			//TRACE("Disconnecting: "+Dump());

			ASSERT(hRetSocket!=INVALID_SOCKET);

			// For an abortive disconnect we want an immediate return and all associated resources 
			// freed.
			LINGER linger;
			linger.l_onoff = 1;
			linger.l_linger = 0;
			VERIFY(!setsockopt(hSocket, SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(linger)));

			//close socket.
			closesocket(hSocket);

			_ProcessReadQueue();

			m_ConnectType = ct_EMPTY;

			InterlockedExchangeAdd(&m_lTotalNoAsyncSendsInWinsockQueue, 0-m_lNoAsyncSendsInWinsockQueue);
//			char buff[200];
//			sprintf(buff, "Decrement(disconnect). m_lTotalNoAsyncSendsInWinsockQueue = %d, m_lMaxNoAsyncSendsInWinsockQueue = %d", m_lTotalNoAsyncSendsInWinsockQueue, m_lMaxNoAsyncSendsInWinsockQueue);
//			_Trace(buff, 0);
			m_lNoAsyncSendsInWinsockQueue = 0;
		}
	}
}

void CConnection::GlobalInitialize(void)
{
	WSADATA WsaData;
	if(SOCKET_ERROR == WSAStartup (MAKEWORD( 2, 2 ), &WsaData))
	{
		ASSERT(0);
	}	

	VERIFY(SUCCEEDED(CWorkQueue::Create(&m_pWorkQ)));
}

BOOL CConnection::ResolveHostNameToSocketAddress(char * lpszHostAddress, USHORT nHostPort, SOCKADDR_IN &sockAddrOut)
{
	memset(&sockAddrOut, 0, sizeof(SOCKADDR_IN));

	sockAddrOut.sin_family = AF_INET;
	sockAddrOut.sin_addr.s_addr = inet_addr(lpszHostAddress);

	if (sockAddrOut.sin_addr.s_addr == INADDR_NONE)
	{
		LPHOSTENT lphost;
		lphost = gethostbyname(lpszHostAddress);
		if (lphost != NULL)
			sockAddrOut.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		else
		{
			WSASetLastError(WSAEINVAL);
			return FALSE;
		}
	}

	sockAddrOut.sin_port = htons((u_short)nHostPort);		

	return TRUE;
}

int CConnection::CreateListener(USHORT nPort, ICallback *pCallback, ReceiveMode rcvMode, SmartPtr<CConnection> & pOutListener)
{		
	int iRet = 0;
	HANDLE hIoPort;

	HRESULT hr = m_pWorkQ->GetIoCompletionPort(&hIoPort);
	ASSERT(SUCCEEDED(hr));

	SmartPtr<CConnection> pListener = new CConnection;
	
	iRet = pListener->_Initialize(hIoPort, pCallback, rcvMode);
	if(iRet)
	{
		ASSERT(0);
		return iRet;
	}

	iRet = pListener->_InitAsListener(nPort);
	if(iRet)
	{
		//probably, this port being used already.
		ASSERT(0);
		return iRet;
	}

	// 20 is the best number according to my benchmarks.
	for(long i = 0; i < NUMBER_OF_ACCEPTORS; i++)
	{	
		iRet = pListener->_CreateAndAddAcceptor();
		ASSERT(0==iRet);
	}

	pOutListener = pListener;
	
	return iRet;
}

int CConnection::CreateConnector(SOCKADDR_IN & SockAddr, ICallback *pCallback, ReceiveMode rcvMode,
							 SmartPtr<CConnection> & pOutConnector)
{
	int iRet = 0;
	HANDLE hIoPort;

	HRESULT hr = m_pWorkQ->GetIoCompletionPort(&hIoPort);
	ASSERT(SUCCEEDED(hr));

	SmartPtr<CConnection> pConnector = new CConnection;
	
	iRet = pConnector->_Initialize(hIoPort, pCallback, rcvMode);
	if(iRet)
	{
		return iRet;
	}

	iRet = pConnector->Connect(SockAddr);
	if(iRet)
	{
		pConnector->Disconnect();
		return iRet;
	}

	pOutConnector = pConnector;
	
	return iRet;
}


/////////////////////////////////////////////////////////////////////////
//Connection_Request methods
/////////////////////////////////////////////////////////////////////////
void CConnection_Request::PerformRequest(OVERLAPPED *pOverlapped)
{
	CDataExchanger *pData = (CDataExchanger *)pOverlapped;
	
	ASSERT(pData);
	ASSERT(m_pConnection);
	ASSERT(pData->m_pThisConnection);
	ASSERT(pData->m_pThisConnection == m_pConnection);

	//Addref yourself. This
	//reference ensurs the existance of this object (which is embedded
	//in the connection) during the lifetime of this call.
	SmartPtr<CConnection> p(pData->m_pThisConnection);

	pData->m_pThisConnection->_OnCallback(pData);
	
	//the above call can delete this.
	//ASSERT(pData->m_pThisConnection == m_pConnection);
}

