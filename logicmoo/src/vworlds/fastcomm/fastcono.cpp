// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include <vwobject.h>
#include <vwcomm.h>
#include <vwbuffer.h>
#include "fastobj.h"

#define MAXBUFFERASCII 8192

BOOL StringMatch(char *szStr1, char *szStr2, int iStrLength)
{
	while (iStrLength-- > 0)
	{
		if (*szStr1++ != *szStr2++)
			return FALSE;
	}
	return TRUE;
}

int IsInStr(char *szSub, int iSubLen, char *szString, int iStringLen)
{
	for (int i = 0; i < iStringLen; i++)
	{
		if (StringMatch(szSub, szString, iSubLen))
			return i+iSubLen;
		szString++;
	}
	return -1;
}

CMyMessageFramer::CMyMessageFramer(IMyMessageFramerCallback *pCallback)
{		
	m_pFramedBuffer = NULL;
	m_ulFramedBufferSize = 0;
	m_ulLengthByte = 0;
	m_pFramedBufferCurrentLocation = NULL;
	m_pCallback = pCallback;
	m_bAsciiMode = TRUE;
}

CMyMessageFramer::~CMyMessageFramer()
{
	//let only one guy delete pFramedBuffer...do it after processing for
	//disconnect finishes.
	BYTE * p = m_pFramedBuffer;
	if(p)
	{
		BYTE *pRet = (BYTE *)InterlockedCompareExchange((void**)&m_pFramedBuffer, (void *)NULL, (void *)p);
		if(p == pRet)
		{
			free(p);
		}
	}
};

void CMyMessageFramer::FrameData(ULONG ulCookie, BYTE *pBuffer, ULONG ulNumberOfBytesTransferred)
{
	//process all data.
	while(ulNumberOfBytesTransferred>0)
	{
		if (m_bAsciiMode)
		{
			// Do we have the whole caboodle?
			int iIndex;
			if ((iIndex = IsInStr("\r\n\r\n", 4, (char*)pBuffer, ulNumberOfBytesTransferred)) != -1)
			{
				// Got it all, continue in binary mode.
				ulNumberOfBytesTransferred -= iIndex;
				m_bAsciiMode = FALSE;
				pBuffer += iIndex;
			}
			else
				break;
		}
		if(m_ulLengthByte<sizeof(COMBLK))
		{
			//read length.
			while(m_ulLengthByte<sizeof(COMBLK) && ulNumberOfBytesTransferred>0)
			{
				m_strByte[m_ulLengthByte]=*pBuffer;
				//read byte into m_ulFramedBufferSize
				m_ulLengthByte++;
				ulNumberOfBytesTransferred--;
				pBuffer++;
			}		
			if(m_ulLengthByte==sizeof(COMBLK))
			{				
				//finished read of length DWORD.
				COMBLK *pHeader = (COMBLK*)m_strByte;
				m_ulFramedBufferSize = pHeader->cchMessage;
				
				//the size does not include comblk size.
				//m_ulFramedBufferSize += sizeof(COMBLK);

				//assert less than 1/2 megabyte per buffer. if bigger
				//probably a misframing.
				ASSERT(m_ulFramedBufferSize<500000); 

				//allocate buffer.
				ASSERT(NULL==m_pFramedBuffer);
				
				m_pFramedBuffer = new BYTE[m_ulFramedBufferSize];
				
				//copy comblk into buffer.
				//CopyMemory(m_pFramedBuffer, m_strByte, sizeof(COMBLK));
				
				m_pFramedBufferCurrentLocation = m_pFramedBuffer;// + sizeof(COMBLK);
				
				m_ulFramedBufferSizeLeft = m_ulFramedBufferSize;// - sizeof(COMBLK);
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
				m_pCallback->FramedMessage(ulCookie, m_pFramedBuffer, 
					m_ulFramedBufferSize, (COMBLK*)m_strByte);			

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

//#define TRACE_ALLOCATION

typedef CComObject<CFastCommMessage> CComFastCommMessage;
typedef CComObject<CFastCommConnection> CComFastCommConnection;

// typedef CComObject<CUnMarshallBufferObject> CComUnMarshallBuffer;
// EXTERN_C const CLSID CLSID_UnMarshallBuffer;

//
// CFastCommConnection
//
CFastCommConnection::CFastCommConnection(): m_Framer(this), m_pauth(NULL), m_pFastCommObject(NULL), m_pCommSite(NULL), m_bDirtyDisconnect(FALSE)
{
#ifdef TRACE_ALLOCATION
	TRACE("CFastCommConnection::CFastCommConnection, sizeof %d, this 0x%0x\n", sizeof(*this), this);
#endif
}

CFastCommConnection::~CFastCommConnection()
{
#ifdef TRACE_ALLOCATION
	TRACE("CFastCommConnection::~CFastCommConnection, this 0x%0x\n", this);
#endif

	SAFERELEASE(m_pauth);
	m_pConnector = NULL;
}

HRESULT CFastCommConnection::Initialize(CFastCommObject *pFastCommObject, IVWCommSite *pCommSite)
{
#ifdef TRACE_ALLOCATION
	TRACE("CFastCommConnection::Initialize, this 0x%0x\n", this);
#endif

	if (m_pFastCommObject || m_pCommSite)
		return ReportConnectionError(VWCOMM_E_IMPROPERSTATE);

	// Weak references to avoid cycles.
	m_pFastCommObject = pFastCommObject;
	m_pCommSite = pCommSite;

	return S_OK;
}

HRESULT CFastCommConnection::SetConnector(CConnection *pConnector)
{
#ifdef TRACE_ALLOCATION
	TRACE("CFastCommConnection::SetConnector, this 0x%0x\n", this);
#endif

	if (m_pConnector)
		return ReportConnectionError(VWCOMM_E_IMPROPERSTATE);

	m_pConnector = pConnector;

	return S_OK;
}

STDMETHODIMP CFastCommConnection::SendAsync(IMarshallBuffer *pcb)
{
	IVWCommConnection *pIVWCommConnection = this;
	return m_pFastCommObject->SendExt(pcb, 1, &pIVWCommConnection, 0, typeAsync);
}

STDMETHODIMP CFastCommConnection::SendAsyncExt(IMarshallBuffer *pcb, int cConnection, IVWCommConnection *rgpConnection[])
{
	return m_pFastCommObject->SendExt(pcb, cConnection, rgpConnection, 0, typeAsync);
}

STDMETHODIMP CFastCommConnection::SendSync(IMarshallBuffer *pcb, IUnMarshallBuffer **ppcbReply)
{
	// Only needed client-side.
	return E_NOTIMPL;
}

STDMETHODIMP CFastCommConnection::Disconnect()
{
	if(!m_pConnector || !m_pFastCommObject || !m_pCommSite)
		return ReportConnectionError(VWCOMM_E_IMPROPERSTATE);

	m_bDirtyDisconnect = FALSE;
	m_pConnector->Disconnect();

	return S_OK;
}

STDMETHODIMP CFastCommConnection::get_SocketAddress(BSTR* pbstrVal)
{
	CComBSTR bstrSockAddr;

	if (pbstrVal == NULL)
		return ReportConnectionError(E_POINTER);

	*pbstrVal = NULL;

	if(!m_pConnector || !m_pFastCommObject || !m_pCommSite)
		return ReportConnectionError(VWCOMM_E_IMPROPERSTATE);

	// verify all's ok
//	if (m_sock==INVALID_SOCKET || m_pcomm==NULL)
//		return ReportConnectionError(VWCOMM_E_NOTINITIALIZED);

//	bstrSockAddr = inet_ntoa(m_sockaddr.sin_addr);
//	*pbstrVal = bstrSockAddr.Copy();

	return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////////////
//
// FastComm Callback Handlers
//

void CFastCommConnection::OnPreAccept(SmartPtr<CConnection> & pAcceptor, ICallback **ppCallback, ReceiveMode *pRcvMode)
{
	CFastCommConnection *pFastCommConnection;

	ASSERT(m_pFastCommObject && m_pCommSite);

	// Create the CFastCommConnection wrapper.
	pFastCommConnection = new CComFastCommConnection;

	// Initialize the CFastCommConnection wrapper.
	pFastCommConnection->Initialize(m_pFastCommObject, m_pCommSite);
	// Give the CFastCommConnection wrapper the connector it wraps.
	pFastCommConnection->SetConnector(pAcceptor);

	EnterCriticalSection(&m_pFastCommObject->m_ConnectionsCS);

	// Place it in our list of connections.
	m_pFastCommObject->m_pConnections.AddTail(pFastCommConnection);

	LeaveCriticalSection(&m_pFastCommObject->m_ConnectionsCS);

	pFastCommConnection->AddRef();

	// Set return values
	*ppCallback = pFastCommConnection;
	*pRcvMode = recv_Streaming;
}

void CFastCommConnection::OnAccept(SmartPtr<CConnection> &pNewConnection, SmartPtr<CConnection> pListener)
{
	ASSERT(m_pConnector && m_pFastCommObject && m_pCommSite);

	// This had better match up!
	ASSERT(pNewConnection == m_pConnector);

	// Just enqueue an empty Connect message.
	EnqueueMessage(CFastCommMessage::statusConnect, this, NULL, 0, 0);

	// Assume from now on that unless we are explicity told to disconnect, a disconnect will
	// be a dirty one.
	m_bDirtyDisconnect = TRUE;
}

void CFastCommConnection::OnDataAvailable(SmartPtr<CConnection> &pThis, BYTE *pBuffer, ULONG ulSize)
{
	ASSERT(pThis == m_pConnector);

	m_Framer.FrameData((ULONG)(CConnection *)pThis, pBuffer, ulSize);
}

void CFastCommConnection::FramedMessage(ULONG cookie, BYTE *pBuffer, ULONG ulSize, COMBLK *pComBlk)
{	
	ASSERT(m_pConnector && m_pFastCommObject && m_pCommSite);

	// This had better match up!
	SmartPtr<CConnection> pThis((CConnection *)cookie);
	ASSERT(pThis == m_pConnector);

	if(m_bDirtyDisconnect==TRUE)
		EnqueueMessage(CFastCommMessage::statusOk, this, pBuffer, ulSize, pComBlk->chSerial);

	delete pBuffer;
}


void CFastCommConnection::OnDisconnect(SmartPtr<CConnection> &pThis)
{
	CFastCommObject *pFastCommObject;
	POSITION pos;
	
	ASSERT(m_pConnector && m_pFastCommObject && m_pCommSite);

	// This had better match up!
	ASSERT(pThis == m_pConnector);

	pFastCommObject = m_pFastCommObject;
	((IVWComm*)pFastCommObject)->AddRef();

	if (m_bDirtyDisconnect)
	{
		// Dirty disconnect. Enqueue a disconnect to make sure its bubbled up.
		EnqueueMessage(CFastCommMessage::statusDirtyDisconnect, this, NULL, 0, 0);
	}

	EnterCriticalSection(&pFastCommObject->m_ConnectionsCS);

	// Remove this from our list of connections.
	pos = pFastCommObject->m_pConnections.Find(this);
	// The listener is not in the list of connections.
	if (pos)
		pFastCommObject->m_pConnections.RemoveAt(pos);

	m_bDirtyDisconnect = FALSE;

	// Release any connections that were in the list 'cause no one has references to
	// them any more (we removed them from the list).
	if (pos)
		Release();
	
	LeaveCriticalSection(&pFastCommObject->m_ConnectionsCS);

	((IVWComm*)pFastCommObject)->Release();
}

void CFastCommConnection::OnErrorNoBuffers(SmartPtr<CConnection> &pThis)
{
	long		lTemp = 0, lStatistics = 0;
	POSITION	pos;
	CFastCommConnection *pTemp = NULL, *pFastCommConnection = NULL;

	ASSERT(m_pConnector && m_pFastCommObject && m_pCommSite);

	// This had better match up!
	ASSERT(pThis == m_pConnector);

	EnterCriticalSection(&m_pFastCommObject->m_ConnectionsCS);

	pos = m_pFastCommObject->m_pConnections.GetHeadPosition();
	while (pos)
	{
		pTemp = m_pFastCommObject->m_pConnections.GetNext(pos);
		pTemp->GetStatistics(&lTemp);

		if (lTemp > lStatistics)
		{
			lStatistics = lTemp;
			pFastCommConnection = pTemp;
		}
	}

	if (pFastCommConnection)
	{
		char buff[200];
		sprintf(buff, "CFastCommConnection::OnErrorNoBuffers: Disconnecting connection. m_pConnector = %x, lStatistics = %d, Time = %d\r\n", pFastCommConnection->m_pConnector, lStatistics, GetTickCount());
		OutputDebugString(buff);
		// Got statistics from the connection, so I know its m_pConnector is OK.
		// Don't want to call CFastCommConnection::Disconnect 'cause this is a FastComm
		// (not a server) initiated disconnect
		pFastCommConnection->m_pConnector->DisconnectAbortive();
		// Disconnect will fire OnDisconnect which will release the CFastCommConnection
		// and remove it from the list.
	}
	LeaveCriticalSection(&m_pFastCommObject->m_ConnectionsCS);
}

/////////////////////////////////////////////////////////////////////////////
//
// Helpers
//

HRESULT CFastCommConnection::EnqueueMessage(int nMessageStatus, CFastCommConnection *pconn, BYTE *pBuffer, ULONG ulSize, char chSerial)
{
	// Create a message.
	CComFastCommMessage* pmess = new CComFastCommMessage();
	pmess->AddRef();

	// Initialize it
	pmess->SetStatus(nMessageStatus);
	pmess->Initialize(m_pFastCommObject, pconn);

	if (pBuffer)
		VERIFY(pmess->SetData(pBuffer, ulSize, chSerial) == S_OK);

	// enqueue it
	pmess->SetTimeStamp(GetTickCount());
	EnterCriticalSection(&m_pFastCommObject->m_MessagesCS);
	if (m_pFastCommObject->m_pMessTail==NULL)
	{
		ASSERT(m_pFastCommObject->m_pMessHead==NULL);
		m_pFastCommObject->m_pMessHead = m_pFastCommObject->m_pMessTail = pmess;
	}
	else
	{
		m_pFastCommObject->m_pMessTail->SetNext(pmess);
		m_pFastCommObject->m_pMessTail = pmess;
	}
	m_pFastCommObject->m_cMessagesInQueue++;
	LeaveCriticalSection(&m_pFastCommObject->m_MessagesCS);

	// tell site that there's data available
	m_pCommSite->OnMessageAvailable_T2(m_pFastCommObject);

	return S_OK;
}

void CFastCommConnection::GetStatistics(long *plNoAsyncSendsInWinsockQueue)
{
	if (!plNoAsyncSendsInWinsockQueue)
		return;

	*plNoAsyncSendsInWinsockQueue = 0;

	if (m_pConnector)
		m_pConnector->GetStatistics(plNoAsyncSendsInWinsockQueue);
}
