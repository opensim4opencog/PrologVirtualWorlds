// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.


// VWClientObj.cpp : Implementation of CvwclientApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include <io.h>
#include <vwclient.h>
#include "vwcliobj.h"
#include "marshbuf.h"
#include "global.h"
#include "urlhelp.h"
#include "reghelp.h"
#include "syshelp.h"
#include "vwevents.h"
#include "propsecu.h"
#include "vwserver.h"

EXTERN_C const CLSID CLSID_VWComm;
EXTERN_C const CLSID CLSID_World;
EXTERN_C const CLSID CLSID_MarshallBuffer;
EXTERN_C const CLSID CLSID_PropertyMap;
EXTERN_C const CLSID CLSID_DServ;
EXTERN_C const CLSID CLSID_Authenticate;
EXTERN_C const CLSID CLSID_Authentication;
EXTERN_C const CLSID CLSID_VWAdmin;

typedef CComObject<CMarshallBufferObject> CComMarshallBuffer;

extern CString gStrToolsKey;

/////////////////////////////////////////////////////////////////////////////
// Macros and parameters

#define HANDLE_CLIENT_EVENTS

// #define RECV_NETWORK_DEBUG

#define TRACE_ALLOCATION VWT_NONE

//#define TRACE_CLIENT_EVENTS VWT_NONE
#define TRACE_CLIENT_EVENTS VWT_METHOD

#define idTimerCheckMessage		5
#define msecPeriodCheckMessage	1000

#ifdef _DEBUG
void CrtSetBreakAlloc(long l);
#endif

/////////////////////////////////////////////////////////////////////////////
//

CVWClientObject::CVWClientObject(void) : 
	m_pTools(NULL),	
	m_pDummyWnd(NULL), 
	m_iMessagesInQueue(0), 
	m_hWnd(NULL),
	m_pAuthenticate(NULL),
	m_pPrimaryWorld(NULL), 
	m_pPrimaryCon(NULL), 
	m_pPrimaryVWComm(NULL),
	m_dwPrimaryCookie(0),
	m_bDisconnectingPrimaryConn(FALSE),
	m_bTerminated(VARIANT_FALSE)
{
#ifdef TRACE_ALLOCATION
	TRACE("CVWClientObject::CVWClientObject, sizeof %d, this 0x%0x\n", sizeof(this), this);
#endif

	//TRACE("CVWClientObject::CVWClientObject is created in thread: %i, Client object: %i\n", ::GetCurrentThreadId(), this);
	
	// REVIEW: we probably don't need crit section here, but
	// leave in for now
	InitializeCriticalSection(&m_CritSec); 
} 

HRESULT CVWClientObject::FinalConstruct(void)
{
	HRESULT hr = S_OK;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_pDummyWnd = new CDummyWnd(this);
	if (m_pDummyWnd)
	{		
		if (m_pDummyWnd->CreateEx(0L, AfxRegisterWndClass(0), _T("DummyWindow"), WS_OVERLAPPED, 
			0, 0, 0, 0, NULL, NULL) == FALSE)
		{
			VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::FinalConstruct: Failed to create hidden window\n");
			hr = VWCLIENT_E_WINDOWCREATIONFAILURE;
		}
	}
	else
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::FinalConstruct: Failed to instantiate hidden window\n");
		hr = VWCLIENT_E_WINDOWCREATIONFAILURE;
	}

	// REVIEW: Acknowledged hack [mv]. Create a timer on the dummy window to see 
	// if this helps with "stuck" messages
	VERIFY(m_pDummyWnd->SetTimer(idTimerCheckMessage, msecPeriodCheckMessage, NULL)==idTimerCheckMessage);

	return hr;
}

CVWClientObject::~CVWClientObject(void)
{
#ifdef TRACE_ALLOCATION
	TRACE("CVWClientObject::~CVWClientObject, this 0x%0x\n", this);
#endif

	Disconnect();

	Terminate();

	DeleteCriticalSection(&m_CritSec); 
}

// do complete shutdown.
STDMETHODIMP CVWClientObject::Terminate()
{
#ifdef TRACE_ALLOCATION
	TRACE("CVWClientObject::Terminate, this 0x%0x\n", this);
#endif

	HRESULT hr = S_OK;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_bTerminated == VARIANT_FALSE)
	{
		if (m_pTools)
		{
			// Note should really QI here!
			UpdateWorldReferences(NULL);
			SAFERELEASE(m_pTools);
		}

		m_clist.RemoveAll();
		m_wlist.RemoveAll();

		// stop the "stuck messages" hack timer before releasing connections.  
		// This is to avoid it referencing connections that may no longer be valid
		if (m_pDummyWnd)
			m_pDummyWnd->KillTimer(idTimerCheckMessage);

		SAFERELEASE(m_pPrimaryCon);

		if (m_pPrimaryWorld)
		{
			m_pPrimaryWorld->Terminate();
			SAFERELEASE(m_pPrimaryWorld);
			m_dwPrimaryCookie = 0;
		}

		SAFERELEASE(m_pPrimaryVWComm);

		SAFERELEASE(m_pAuthenticate);

		// delete dummywnd.
		if (m_pDummyWnd)
		{
			if (m_pDummyWnd->m_hWnd)
			{
				// remove any pending messages on the windows q
				MSG msg;
				while (::PeekMessage(&msg, m_pDummyWnd->m_hWnd, NULL, NULL, PM_REMOVE));

				m_pDummyWnd->DestroyWindow();
				delete m_pDummyWnd;
				m_pDummyWnd = NULL;
			}
		}

		m_bTerminated = VARIANT_TRUE;
	}

	return hr;
}

// ErrorInfo

STDMETHODIMP CVWClientObject::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IVWClient
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

// Sets m_pWorld, m_pVWConnection. No side effects.
HRESULT CVWClientObject::ConnectHelper(BSTR bstrInitialURL, IWorld** ppWorld, IVWComm** ppVWComm,
	IVWCommConnection** ppVWConnection, DWORD* pdwCookie)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HRESULT hr = S_OK;
	CString strMessage;
	DWORD cbMessage;
//	int nPort;
	CComBSTR bstrProtocol;
	CComBSTR bstrServer;
	CComMarshallBuffer* pbuffer = NULL;
	IUnMarshallBuffer* pbufReply = NULL;
	HRESULT replyhr = S_OK;
//	VARIANT_BOOL bTunnel;
		

	VWTRACE(NULL, "VWCLIENT", VWT_VERBOSE, "CVWClientObject::Connect\n");

	if (ppWorld == NULL)
		return E_POINTER;
	if (ppVWComm == NULL)
		return E_POINTER;
	if (ppVWConnection == NULL)
		return E_POINTER;
	if (pdwCookie == NULL)
		return E_POINTER;

	// initialize
	*ppWorld = NULL;
	*ppVWComm = NULL;
	*ppVWConnection = NULL;
	
	hr=ConnectToServerInt(bstrInitialURL, VARIANT_TRUE /* bAuthenticate */, ppVWComm, ppVWConnection);
	if (FAILED(hr))
	{
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::Connect: Failed to connect to server\n");
		goto ERROR_ENCOUNTERED;
	}

	// start up VWODB (overwrite any local database). Sets pWorld.
	hr = CreateLocalWorld(NULL, VARIANT_TRUE, ppWorld, pdwCookie);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// initialize world with connection (must come after ConnectLocalEx)
	hr = (*ppWorld)->InitializeConnection(*ppVWConnection);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// now send message indicating what world we want to go into

	// compose the message
	strMessage = 'A';		// 'A' is for "Administrative" messages
	strMessage += 'W';		// 'W' is for logging onto a World
//	strMessage += bstrInitialURL;

	cbMessage = strMessage.GetLength()+1;

	// create marshall buffer
	pbuffer = new CComMarshallBuffer();
	if (pbuffer == NULL)
	{
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::Connect: failed to allocate CComMarshallBuffer\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	pbuffer->AddRef();

	// write out client version to send to server
	hr = pbuffer->put_ULong(PROTOCOL_VERSION);
	if (FAILED(hr))
	{
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::Connect: Failed to write to marshall buffer\n");
		goto ERROR_ENCOUNTERED;
	}

	hr = pbuffer->put_Bytes((BYTE *) (LPCSTR) strMessage, cbMessage);
	if (FAILED(hr))
	{
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::Connect: Failed to write to marshall buffer\n");
		goto ERROR_ENCOUNTERED;
	}

	// changed from appending to strMessage above
	hr = pbuffer->put_String(bstrInitialURL, VARIANT_FALSE);
	if (FAILED(hr))
	{
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::Connect: Failed to write to marshall buffer\n");
		goto ERROR_ENCOUNTERED;
	}

	// send it, waiting for reply
	hr = (*ppVWConnection)->SendSync(pbuffer, &pbufReply);
	if (FAILED(hr))
	{
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::Connect: Failed to send World URL message\n");
		goto ERROR_ENCOUNTERED;
	}

	hr = pbufReply->get_Long((long*)&replyhr);
	if (FAILED(hr))
	{
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::Connect: Failed to get text of reply to World URL message\n");
		goto ERROR_ENCOUNTERED;
	}

	if (FAILED(replyhr))
	{
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::Connect: Server failed to connect to World URL, hr %x\n", replyhr);
		hr = replyhr;
		goto ERROR_ENCOUNTERED;
	}

	// remember server
	m_bstrPrimaryServer = bstrServer;

	// if no error, release is required as well.
	

ERROR_ENCOUNTERED:
	SAFERELEASE(pbufReply);
	SAFERELEASE(pbuffer);

// if any error, clean up connection stuff
	if (FAILED(hr))
	{
		// CreateLocalWorld will cleanup ppWorld on failure

		if (*ppVWConnection)
			(*ppVWConnection)->Disconnect();
		SAFERELEASE(*ppVWConnection);

		if (*ppVWComm)
			(*ppVWComm)->DisconnectAll();
		SAFERELEASE(*ppVWComm);
	}
	
	return hr;
}

// No side effects except for putting stuff into maps and setting primary vars.
STDMETHODIMP CVWClientObject::Connect(BSTR bstrInitialURL, IWorld** ppWorld)
{
	HRESULT hr = S_OK;
	IVWComm* pVWComm = NULL;
	IVWCommConnection* pVWConnection = NULL;
	DWORD dwCookie = 0;
	CComBSTR bstrURL, bstrServer;
	CString strWorldID;
	CWorldInfo* pwinfo;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_VERBOSE, "CVWClientObject::Connect\n");

	// shouldn't call Connect if already connected
	if (m_pPrimaryWorld)
	{
		hr = VWCLIENT_E_ALREADYCONNECTED;
		goto ERROR_ENCOUNTERED;
	}

	hr = ConnectHelper(bstrInitialURL, ppWorld, &pVWComm, &pVWConnection, &dwCookie);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// move temp stuff to maps.
	ASSERT(pVWComm);
	ASSERT(pVWConnection);

	// succeeds for sure since this was done earlier in helper.
	hr = ParseURLHelper(bstrInitialURL, &bstrURL.m_str, NULL, &bstrServer.m_str, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (m_clist.FindConnection(pVWConnection))
	{
		hr = VWCLIENT_E_ALREADYCONNECTED;
		goto ERROR_ENCOUNTERED;
	}
	
	strWorldID = CString(bstrServer) + "/" + CString(bstrURL);

	pwinfo = m_wlist.FindWorld(strWorldID);
	if (pwinfo)
	{
		hr = VWCLIENT_E_ALREADYCONNECTED;
		goto ERROR_ENCOUNTERED;
	}

	m_clist.AddConnection(pVWConnection, *ppWorld, pVWComm, CComBSTR(""));
	
	// does own ref count.
	m_wlist.AddWorld(strWorldID, *ppWorld, dwCookie);

	// set primary connection/world information. Bump ref counts as needed
	m_dwPrimaryCookie = dwCookie;

	m_pPrimaryWorld = *ppWorld;
	SAFEADDREF(m_pPrimaryWorld);

	m_pPrimaryCon = pVWConnection;
	SAFEADDREF(m_pPrimaryCon);

	m_pPrimaryVWComm = pVWComm;	
	SAFEADDREF(m_pPrimaryVWComm);

	// fire the connection event
	FireOnConnect(VARIANT_TRUE, m_pPrimaryWorld);

ERROR_ENCOUNTERED:
	// clear scratchpad.
	SAFERELEASE(pVWConnection);
	SAFERELEASE(pVWComm);

	return ReportClientError(hr);
}

STDMETHODIMP CVWClientObject::ConnectLocal(BSTR bstr, IWorld** ppWorld)
{
	HRESULT hr = S_OK;
	IThing *pGlobal = NULL;
	BSTR bstrGUID = NULL;
	static CComBSTR bstrGUIDStatic("GUID");

	// overwrite when using sengine: he's the administrator anyway... [C.H.].
	hr = ConnectLocalEx(bstr, VARIANT_TRUE, ppWorld);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// Get world's Global object.
	hr = (*ppWorld)->get_Global(&pGlobal);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pGlobal->get_String(bstrGUIDStatic, &bstrGUID);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// Add servername 
	{
		CSetUID SetUID((*ppWorld), pGlobal, pGlobal, VARIANT_TRUE);

		hr = pGlobal->put_String(CComBSTR("ServerName"), CComBSTR("localhost"));
		if (FAILED(hr))
		{
			//should already be there. (addproperty should not be necessary).
			ASSERT(0); 
			goto ERROR_ENCOUNTERED;
		}

		hr = pGlobal->put_String(CComBSTR("WorldName"), bstr);
		if (FAILED(hr))
		{
			//should already be there. (addproperty should not be necessary).
			ASSERT(0);
			goto ERROR_ENCOUNTERED;
		}
	}

	hr = AddWorldToRegistry(bstrGUID, bstr, CComBSTR("Friendly name here."));
	if (FAILED(hr))
	{
		VWTRACE(*ppWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::ConnectLocal: AddWorldToRegistry failed\n");
		goto ERROR_ENCOUNTERED;
	}

	hr = RegisterWorldInDS(bstr, CComBSTR(""), FALSE);		
	if (FAILED(hr))
	{
		ASSERT(0);
		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pGlobal);	
	SAFEFREESTRING(bstrGUID);

	return ReportClientError(hr);
}

// No side effects except for tool stuff, primary vars.
STDMETHODIMP CVWClientObject::ConnectLocalEx(BSTR bstr, VARIANT_BOOL bOverwrite, IWorld** ppWorld)
{
	HRESULT hr = S_OK;
	CWorldInfo* pwinfo = NULL;
	DWORD dwCookie = 0;
	IVWComm* pVWComm = NULL;

	// shouldn't call Connect if already connected
	if (m_pPrimaryWorld)
	{
		hr = VWCLIENT_E_ALREADYCONNECTED;
		goto ERROR_ENCOUNTERED;
	}

	if (bstr == NULL)
	{
		hr = VWCLIENT_E_INVALIDDATABASENAME;
		goto ERROR_ENCOUNTERED;
	}

	hr = CoCreateInstance(CLSID_VWComm, NULL, CLSCTX_INPROC_SERVER, IID_IVWComm, (LPVOID *)&pVWComm);
	if (FAILED(hr))
	{		
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::ConnectLocalEx: Failed to create VWComm object\n");	
		goto ERROR_ENCOUNTERED;
	} 

	hr = pVWComm->Initialize(this);
	if (FAILED(hr))
	{
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::ConnectLocalEx: Failed to set VWComm site\n");
		goto ERROR_ENCOUNTERED;
	}

	hr = CreateLocalWorld(bstr, bOverwrite, ppWorld, &dwCookie);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// TODO, format bstr.
	pwinfo = m_wlist.FindWorld(CString(bstr));
	if (pwinfo)
	{
		hr = VWCLIENT_E_ALREADYCONNECTED;
		goto ERROR_ENCOUNTERED;
	}

	m_wlist.AddWorld(CString(bstr), *ppWorld, dwCookie);

	// store primary info. Bump ref count as needed
	m_dwPrimaryCookie = dwCookie;

	m_pPrimaryWorld = *ppWorld;
	SAFEADDREF(m_pPrimaryWorld);

	m_pPrimaryVWComm = pVWComm;	
	SAFEADDREF(m_pPrimaryVWComm);

	FireOnConnect(VARIANT_TRUE, m_pPrimaryWorld);

ERROR_ENCOUNTERED:
	SAFERELEASE(pVWComm);

	// CreateLocalWorld will cleanup ppWorld on failure

	return ReportClientError(hr);
}

// No side effects except for tool stuff.
HRESULT CVWClientObject::CreateLocalWorld(BSTR bstrFileName, VARIANT_BOOL bOverwrite, IWorld** ppWorld, DWORD *pdwCookie)
{
	HRESULT hr = S_OK;
	BOOL isAfterAdvise = FALSE;
	IConnectionPoint* pconnpt = NULL;
	IConnectionPointContainer* pconnptctr = NULL;

	VWTRACE(NULL, "VWCLIENT", VWT_VERBOSE, "CVWClientObject::CreateLocalWorld\n");

	// NOTE: bstrFileName can be NULL

	if (ppWorld == NULL || pdwCookie == NULL)
		return E_POINTER;

	*ppWorld = NULL;
	*pdwCookie = 0;

	hr = CoCreateInstance(CLSID_World, NULL, CLSCTX_INPROC_SERVER, IID_IWorld, (LPVOID *)ppWorld);
	if (FAILED(hr))
	{
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::CreateLocalWorld: Failed to create world object\n");
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
		// HACK: artificially Release
		GetUnknown()->Release();
	}
	else
		goto ERROR_ENCOUNTERED;
	
	isAfterAdvise = TRUE;

	// must come before put_World
	hr = (*ppWorld)->put_ServerSide(VARIANT_FALSE);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// IObjectProperty requirement
	hr = (*ppWorld)->put_World(*ppWorld);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	(*ppWorld)->put_TraceLevel(CComBSTR("*"), VWT_ERROR);

	// now that we have a world...
	UpdateWorldReferences(*ppWorld);

	// Add the cachemanager to the world before it connects.
	hr = (*ppWorld)->InitializeTools(m_pTools);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// tell world what repository to use
	hr = (*ppWorld)->OpenDatabase(bstrFileName, VARIANT_TRUE /* bCreate */, bOverwrite);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	SAFERELEASE(pconnptctr);
	
	if (FAILED(hr) && isAfterAdvise)
	{
		if (*ppWorld)
		{
			DeleteWorld(*ppWorld, *pdwCookie);		
			SAFERELEASE(*ppWorld);
		}
	}

	return hr;
}

// send administrative message to server indicating logging off world.
// No side effects 
HRESULT CVWClientObject::SendDisconnectMessageToServer(IVWCommConnection *pCommConnection)
{	
	CString strMessage;
	CComMarshallBuffer* pbuffer = NULL;
	IUnMarshallBuffer* pbufReply = NULL;
	HRESULT replyhr = S_OK;
	HRESULT hr = S_OK;
	
	// compose the message
	
	// 'A' is for "Administrative" messages
	strMessage = 'A';		
	
	// 'D' is for 'D'isconnecting from a World
	strMessage += 'D';		
	DWORD cbMessage = strMessage.GetLength()+1;
	
	// create marshall buffer
	pbuffer = new CComMarshallBuffer();
	if (pbuffer == NULL)
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::SendDisconnectMessageToServer: failed to allocate CComMarshallBuffer\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	pbuffer->AddRef();

	// write out client version to send to server
	hr = pbuffer->put_ULong(PROTOCOL_VERSION);
	if (FAILED(hr))
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::SendDisconnectMessageToServer: Failed to write to marshall buffer\n");
		goto ERROR_ENCOUNTERED;
	}

	hr = pbuffer->put_Bytes((BYTE *) (LPCSTR) strMessage, cbMessage);
	if (FAILED(hr))
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::SendDisconnectMessageToServer: Failed to attach to marshall buffer\n");
		goto ERROR_ENCOUNTERED;
	}
	
	// (C.H.):  the check for disconnected connection has proven unreliable (see bug 5349, 5317 etc).
	// send it, and continue
	hr = pCommConnection->SendAsync(pbuffer);
	//hr = pCommConnection->SendSync(pbuffer, &pbufReply);
	if (FAILED(hr))
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::SendDisconnectMessageToServer: Failed to send World disconnect message\n");
		goto ERROR_ENCOUNTERED;
	}
/*	
	hr = pbufReply->get_Long((long*)&replyhr);
	if (FAILED(hr))
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::SendDisconnectMessageToServer: Failed to get text of reply to World Disconnect message\n");
		goto ERROR_ENCOUNTERED;
	}

	if (FAILED(replyhr))
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::SendDisconnectMessageToServer: Server failed to connect to Disconnect message\n");		
		hr = replyhr;
		goto ERROR_ENCOUNTERED;
	}
*/		
ERROR_ENCOUNTERED:
	SAFERELEASE(pbufReply);
	SAFERELEASE(pbuffer);
	
	return hr;
}

// No side effects. Uses primarycon if pCommConnection is NULL.
HRESULT CVWClientObject::SendReconnectMessageToServer(BSTR bstrURL, BSTR bstrName, BSTR bstrPassword, IVWCommConnection *pCommConnection)
{	
	CString strMessage;
	CComMarshallBuffer* pbuffer = NULL;
	IUnMarshallBuffer* pbufReply = NULL;
	HRESULT replyhr = S_OK;
	HRESULT hr = S_OK;
	
	// if not currently in a processmessage...
	if(!pCommConnection) 
	{
		if(m_pPrimaryCon)
			pCommConnection = m_pPrimaryCon;
		else
			// TODO.
			return E_FAIL;
	}

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
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::SendReconnectMessageToServer: failed to allocate CComMarshallBuffer\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	pbuffer->AddRef();

	// write out client version to send to server
	hr = pbuffer->put_ULong(PROTOCOL_VERSION);
	if (FAILED(hr))
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::SendReconnectMessageToServer: Failed to write to marshall buffer\n");
		goto ERROR_ENCOUNTERED;
	}

	hr = pbuffer->put_Bytes((BYTE *) (LPCSTR) strMessage, cbMessage);
	if (FAILED(hr))
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::SendReconnectMessageToServer: Failed to attach to marshall buffer\n");
		goto ERROR_ENCOUNTERED;
	}

	// We put the password/login info into buffer.
	hr = pbuffer->put_String(bstrURL, VARIANT_FALSE);
	if (FAILED(hr))
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::SendReconnectMessageToServer: Failed to attach to marshall buffer\n");
		goto ERROR_ENCOUNTERED;
	}

	hr = pbuffer->put_String(bstrName, VARIANT_FALSE);
	if (FAILED(hr))
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::SendReconnectMessageToServer: Failed to attach to marshall buffer\n");
		goto ERROR_ENCOUNTERED;
	}

	hr = pbuffer->put_String(bstrPassword, VARIANT_FALSE);
	if (FAILED(hr))
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::SendReconnectMessageToServer: Failed to attach to marshall buffer\n");
		goto ERROR_ENCOUNTERED;
	}

	// send it, waiting for reply
	hr = pCommConnection->SendSync(pbuffer, &pbufReply);
	if (FAILED(hr))
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::SendReconnectMessageToServer: Failed to send World disconnect message\n");
		goto ERROR_ENCOUNTERED;
	}
	
	hr = pbufReply->get_Long((long*)&replyhr);
	if (FAILED(hr))
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::SendReconnectMessageToServer: Failed to get text of reply to World Disconnect message\n");
		goto ERROR_ENCOUNTERED;
	}

	if (FAILED(replyhr))
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::SendReconnectMessageToServer: Server failed to connect to Disconnect message\n");		
		hr = replyhr;
		goto ERROR_ENCOUNTERED;
	}
		
ERROR_ENCOUNTERED:
	SAFERELEASE(pbufReply);
	SAFERELEASE(pbuffer);
	
	return hr;
}

// No side effects besides map erasure. Releases pworld, cnctn if necessary.
// connection parameter is null if irrelevant.
void CVWClientObject::DisconnectWorld(IWorld *pworld, IVWCommConnection *pCommConnection)
{
	HRESULT hr = S_OK;
	CWorldInfo *pinfo = m_wlist.FindWorld(pworld);

	SAFEADDREF(pCommConnection);

	if (pworld)
	{
		SAFEADDREF(pworld);

		/* hr = */ pworld->DisconnectConnection(pCommConnection);		

		// tell the connection to hang up 
		if (pCommConnection) 
		{
			pCommConnection->Disconnect();

			// remove the connection-to-world association
			m_clist.RemoveConnection(pCommConnection);
		}

		m_wlist.RemoveWorld(pworld);

		// fire event before releasing world
		FireOnConnect(VARIANT_FALSE, pworld);
		
		if (pworld == m_pPrimaryWorld)
		{
			SAFERELEASE(m_pPrimaryWorld);
			m_dwPrimaryCookie = 0;
		}

		SAFERELEASE(pworld);
	}
	else
	{
		if (pCommConnection)
		{
			// still need to nuke the connection
			pCommConnection->Disconnect();

		}
	}

	SAFERELEASE(pCommConnection);
}

// No side effects besides map erasure. Releases pworld, cnctn.
void CVWClientObject::DisconnectConnection(IVWCommConnection *pCommConnection, BOOL bIsSocketAlive)
{
	SAFEADDREF(pCommConnection);

	// STEP 1: Get the info about the world assoc'ed with the connection.
	IWorld* pworld = m_clist.FindConnection(pCommConnection);
	SAFEADDREF(pworld);

	if (bIsSocketAlive)
	{
		// NOTE: we ignore the return value 'cause the connection may
		// already be closed (in the case of a server-side comm error)
		SendDisconnectMessageToServer(pCommConnection); 
	}

	// If pworld is null, this still nukes the connection
	DisconnectWorld(pworld, pCommConnection);
	SAFERELEASE(pworld);

	SAFERELEASE(pCommConnection);
}

// with the side of effect of zeroing primary vars when appropriate.
void CVWClientObject::DisconnectHelper(IVWCommConnection *pcon, BOOL bIsSocketAlive)
{
	if (pcon != NULL)
	{
		SAFEADDREF(pcon);

		if (pcon == m_pPrimaryCon)
		{
			if (!m_bDisconnectingPrimaryConn)
			{
				m_bDisconnectingPrimaryConn = TRUE;

				DisconnectConnection(m_pPrimaryCon, bIsSocketAlive);

				SAFERELEASE(m_pPrimaryCon);

				if (m_pPrimaryVWComm)
				{
					m_pPrimaryVWComm->Terminate();
					SAFERELEASE(m_pPrimaryVWComm);
				}

				m_bDisconnectingPrimaryConn = FALSE;
			}
		}
		else
			DisconnectConnection(pcon, bIsSocketAlive);

		SAFERELEASE(pcon);
	}
}

// do shutdown of primary vars.
STDMETHODIMP CVWClientObject::Disconnect(void)
{
	HRESULT hr = S_OK;

	VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_VERBOSE, "CVWClientObject::Disconnect\n");

	if (m_pPrimaryCon)
	{
		DisconnectHelper(m_pPrimaryCon);
	}
	else if (m_pPrimaryWorld)
	{	
		// disconnect user from connection
		DisconnectWorld(m_pPrimaryWorld, NULL);

		if (m_pPrimaryVWComm)
		{
			m_pPrimaryVWComm->Terminate();
			SAFERELEASE(m_pPrimaryVWComm);
		}
	}
	else
	{
		if (m_pPrimaryVWComm)
		{
			m_pPrimaryVWComm->Terminate();
			SAFERELEASE(m_pPrimaryVWComm);
		}

		ASSERT(m_dwPrimaryCookie == 0);
	}

	return ReportClientError(hr);
}

STDMETHODIMP CVWClientObject::Initialize()
{
	HRESULT hr = S_OK;

	hr = CoCreateInstance(CLSID_PropertyMap, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (LPVOID *)&m_pTools);
	if (FAILED(hr))
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::Initialize: Failed to create tool map\n");	
		goto ERROR_ENCOUNTERED;
	}

	hr = CreateTools();
	if (FAILED(hr))
	{		
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::Initialize: Failed to create tools\n");	
		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	return hr;
}

HRESULT CVWClientObject::OnReconnect(IUnMarshallBuffer* puBuffer, IVWCommConnection* pVWConnection, BOOL& bConnectionClosed)
{
	// this function is called when the client receives the server's okay to
	// reconnect.

	HRESULT hr = S_OK;
	BSTR bstrURL = NULL;
	BSTR bstrName = NULL;
	BSTR bstrPassword = NULL;			
	CString strWorldID;
	CComBSTR bstrServer;

	HRESULT retHR = S_OK;
		
	IWorld* pNewWorld = NULL;
	IThing* pNewAvatar = NULL;
	CWorldInfo* pwinfo = NULL;
	IVWCommConnection* pNewConn = NULL;
	IVWComm* pNewComm = NULL;
	DWORD dwNewCookie = 0;

	// get hresult.
	hr = puBuffer->get_Bytes((BYTE *)&retHR, sizeof(HRESULT));
	if (FAILED(hr))
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::OnReconnect: Operation failed.\n");
		goto ERROR_ENCOUNTERED;
	}

	// check if server is okay.
	if (FAILED(retHR))
	{
		// TODO: fire event. tell ui to post messagebox or something to 
		// tell user reconnect failed on server side.
		
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::OnReconnect: Operation failed.\n");
		goto ERROR_ENCOUNTERED;
	}

	// get password/login stuff.
	hr = puBuffer->get_String(&bstrURL);
	if (FAILED(hr))
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::OnReconnect: Failed.\n");				
		goto ERROR_ENCOUNTERED;
	}
	
	hr = puBuffer->get_String(&bstrName);
	if (FAILED(hr))
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::OnReconnect: Failed.\n");				
		goto ERROR_ENCOUNTERED;
	}
	
	hr = puBuffer->get_String(&bstrPassword);
	if (FAILED(hr))
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::OnReconnect: Failed.\n");				
		goto ERROR_ENCOUNTERED;
	}

	// Connect to new site.

	// create new world. connect to server.
	hr = ConnectHelper(bstrURL, &pNewWorld, &pNewComm, &pNewConn, &dwNewCookie);
	if (FAILED(hr))
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::OnReconnect: Failed.\n");				
		goto ERROR_ENCOUNTERED;
	}	
	
	// Put stuff into maps.
	hr = ParseURLHelper(bstrURL, NULL, NULL, &bstrServer.m_str, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (m_clist.FindConnection(pNewConn))
	{
		hr = VWCLIENT_E_ALREADYCONNECTED;
		goto ERROR_ENCOUNTERED;
	}
	
	//strWorldID = CString(bstrServer) + "/" + CString(bstrURL);
	strWorldID = CString(bstrURL);

	pwinfo = m_wlist.FindWorld(strWorldID);
	if (pwinfo)
	{
		hr = VWCLIENT_E_ALREADYCONNECTED;
		goto ERROR_ENCOUNTERED;
	}

	m_clist.AddConnection(pNewConn, pNewWorld, pNewComm, CComBSTR(""));
	
	// does own ref count.
	m_wlist.AddWorld(strWorldID, pNewWorld, dwNewCookie);

	// login as new avatar.
	hr = pNewWorld->Connect(bstrName, bstrPassword, &pNewAvatar);
	if (FAILED(hr))
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::OnReconnect: IWorld::Connect failed.\n");				
		goto ERROR_ENCOUNTERED;
	}	
	
	// let UI know we're reconnecting so that it doesn't get confused
	// when it sees its primary connection going away
	FireOnUIEvent(NULL, CComBSTR(VW_RECONNECT_EVENT_STR), CComVariant());

	DisconnectHelper(pVWConnection);
	bConnectionClosed = TRUE;
	
	UpdateWorldReferences(pNewWorld);

	// if primary world was the old world, change it...
	// put variables back to original.
	if (!m_pPrimaryCon || m_pPrimaryCon == pVWConnection) 
	{
		SAFERELEASE(m_pPrimaryWorld);
		m_pPrimaryWorld = pNewWorld;
		SAFEADDREF(m_pPrimaryWorld);

		SAFERELEASE(m_pPrimaryCon);
		m_pPrimaryCon = pNewConn;
		SAFEADDREF(m_pPrimaryCon);

		SAFERELEASE(m_pPrimaryVWComm);
		m_pPrimaryVWComm = pNewComm;
		SAFEADDREF(m_pPrimaryVWComm);

		m_dwPrimaryCookie = dwNewCookie;
	}

	// tell everybody about new client.
	// Hope everything works for this. Recovery here is very complex. (Some ocx's failed,
	// others succeeded, etc. We go on no matter what. NO if(FAIL...)
	hr = FireOnUserReconnect(this, pNewWorld, pNewAvatar);

ERROR_ENCOUNTERED:
	if(FAILED(hr))
	{
		DisconnectHelper(pNewConn);
		FireOnUIEvent(NULL, CComBSTR(VW_RECONNECTFAILED_EVENT_STR), CComVariant(bstrURL));

	}
	SAFEFREESTRING(bstrURL);
	SAFEFREESTRING(bstrName);
	SAFEFREESTRING(bstrPassword);

	SAFERELEASE(pNewAvatar);
	SAFERELEASE(pNewWorld);
	SAFERELEASE(pNewConn);
	SAFERELEASE(pNewComm);

	// clear scratchpad.
	return ReportClientError(hr);
}

STDMETHODIMP CVWClientObject::OnUserReconnect(BSTR bstrURL, BSTR bstrName, BSTR bstrPassword)
{
	// this function is called when a client side script calls world.reconenct.

	// if there are vwobject/vwclient states where a reconnect is
	// not possible, this is the place to return a failure.

	// tell the server about the reconnect request.
	return SendReconnectMessageToServer(bstrURL, bstrName, bstrPassword,NULL);
}

STDMETHODIMP CVWClientObject::get_World(IWorld** ppWorld)
{
	if (ppWorld == NULL)
		return E_POINTER;

	*ppWorld = m_pPrimaryWorld;
	SAFEADDREF(*ppWorld);

	return S_OK;
}


STDMETHODIMP CVWClientObject::get_IsConnected(VARIANT_BOOL* pBool)
{
	VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_VERBOSE, "CVWClientObject::get_IsConnected\n");

	if(m_bDisconnectingPrimaryConn)
	{
		*pBool = VARIANT_FALSE;
	}
	else
	{
		if (m_pPrimaryWorld)
			*pBool = VARIANT_TRUE;
		else
			*pBool = VARIANT_FALSE;
	}

	return S_OK;
}

HRESULT CVWClientObject::OnMessageAvailableHelper(IVWComm *pcomm)
{
	// process messages 'til done
	IVWCommMessage *pcmm;
	static UINT ret;

#ifdef RECV_NETWORK_DEBUG
	CString strTemp;
	DWORD dwTime = GetTickCount();

	strTemp.Format("OnMessageAvailableHelper (begin),%d,%d\n", dwTime, m_iMessagesInQueue);

	OutputDebugString((LPCTSTR)strTemp);
#endif

	// REVIEW: we probably don't need crit section here, but
	// leave in for now
	EnterCriticalSection(&m_CritSec);

	if (pcomm != NULL)
	{
		// there's a danger that the comm pointer will disappear during 
		// HandleMessage. AddRef it to avoid this
		// REVIEW: Probably don't need bConnectionClosed anymore
		pcomm->AddRef();

		// sometimes, comm is deleted already.
		if (m_clist.FindVWComm(pcomm))
		{
			BOOL bConnectionClosed = FALSE;
			while (!bConnectionClosed && SUCCEEDED(pcomm->GetMessage(&pcmm)) && pcmm != NULL)
			{
				InterlockedDecrement(&m_iMessagesInQueue);
				HandleMessage(pcmm, bConnectionClosed);	
				pcmm->Release();
				
			}
			m_iMessagesInQueue = 0;
		}

		// release
		pcomm->Release();
	}
	
	LeaveCriticalSection(&m_CritSec);


#ifdef RECV_NETWORK_DEBUG
	{
		CString strTemp;
		DWORD dwNow = GetTickCount();

		strTemp.Format("OnMessageAvailableHelper (end),%d,%d,%d\n", dwNow, dwNow - dwTime, m_iMessagesInQueue);

		OutputDebugString((LPCTSTR)strTemp);
	}
#endif

	return S_OK;
}

// called by vwcomm directly in it's wait loop during a sendsync.
STDMETHODIMP CVWClientObject::OnMessageAvailable(IVWComm *pcomm)
{
	// process messages 'til done
	IVWCommMessage *pcmm;
	static UINT ret;	

#ifdef RECV_NETWORK_DEBUG
	CString strTemp;
	DWORD dwTime = GetTickCount();

	strTemp.Format("OnMessageAvailable (begin),%d,%d\n", dwTime, m_iMessagesInQueue);

	OutputDebugString((LPCTSTR)strTemp);
#endif

	// REVIEW: we probably don't need crit section here, but
	// leave in for now
	EnterCriticalSection(&m_CritSec);

	if (pcomm != NULL)
	{
		// there's a danger that the comm will pointer disappear during 
		// HandleMessage. AddRef it to avoid this
		// REVIEW: Probably don't need bConnectionClosed anymore
		pcomm->AddRef();

		BOOL bConnectionClosed = FALSE;
		while (!bConnectionClosed && SUCCEEDED(pcomm->GetMessage(&pcmm)) && pcmm != NULL)
		{
			m_iMessagesInQueue--;

			DWORD dwTime = GetTickCount();

			HandleMessage(pcmm, bConnectionClosed);	

			pcmm->Release();			
		}

		m_iMessagesInQueue = 0;

		// release
		pcomm->Release();
	}	

	LeaveCriticalSection(&m_CritSec);

#ifdef RECV_NETWORK_DEBUG
	{
		CString strTemp;
		DWORD dwNow = GetTickCount();

		strTemp.Format("OnMessageAvailable (end),%d,%d,%d\n", dwNow, dwNow - dwTime, m_iMessagesInQueue);

		OutputDebugString((LPCTSTR)strTemp);
	}
#endif

	return S_OK;
}

HRESULT CVWClientObject::ProcessAdminMessage(IUnMarshallBuffer *puBuffer, BYTE chType, IVWCommConnection *pVWConnection, BOOL& bConnectionClosed)
{
	CString str;
		
	switch(chType)
	{
		// Receive warning that server shuts down soon.	
		case 'W': 
		{
			char buffer[20];
			ltoa(SHUTDOWN_WAIT_TIME,buffer,10);
			// for Debug use:
			FireOnTrace(CComBSTR("CVWClientObject::ProcessAdminMessage: Received admin message. Request client to complete his session. Logoff forced in "+CString(buffer)+" ms.\n"));
			VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_IMPORTANT, "CVWClientObject::ProcessAdminMessage: Received admin message. Request client to complete his session. Logoff forced in "+CString(buffer)+" ms.\n");			
			// TODO
			// FireOnDisconnect(pconn); Fire something when we have that.			
			// do nothing.
			break;
		}

		// Receive warning that client should shut down now. (Server inoperative now).
		case 'D': 
		{
			// for Debug use:
			FireOnTrace(CComBSTR("CVWClientObject::ProcessAdminMessage: Received admin message. Server is commanding client to logoff now.\n"));
			VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_IMPORTANT, "CVWClientObject::ProcessAdminMessage: Received admin message. Server is commanding client to logoff now.\n");			
			// FireOnDisconnect(pconn); Fire something when we have that.		
			// Disconnect the world.			
			// Disconnect the client.
			// This tells client's Disconnect() not to bother sending server the disconnect msg,
			// since this is server directed, and the server's gone already.
			DisconnectHelper(pVWConnection);
			bConnectionClosed = TRUE;
			break;
		}

		// Receive cancellation msg that server's not shutting down anymore.
		case 'C': 
		{
			FireOnTrace(CComBSTR("CVWClientObject::ProcessAdminMessage: Received admin message. Previous logoff warning cancelled. Server not shutting down.\n"));
			VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_IMPORTANT, "CVWClientObject::ProcessAdminMessage: Received admin message. Previous logoff warning cancelled. Server not shutting down.\n");			
			// TODO:
			// FireOnDisconnect(pconn); Fire something when we have that.			
			break;		
		}

		case 'R':
		{
			OnReconnect(puBuffer, pVWConnection, bConnectionClosed);
			break;
		}

		default:
		{
			VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::ProcessAdminMessage: Received invalid admin message.\n");			
			break;
		}
	}
	return S_OK;
}

HRESULT CVWClientObject::HandleMessage(IVWCommMessage* pmess, BOOL& bConnectionClosed)
{
	HRESULT hr;
	IVWCommConnection *pconn = NULL;
	int iType;

	// get the connection out of the message
	hr = pmess->GetConnection(&pconn);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// see what type of message we have
	hr = pmess->GetStatus(&iType);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// handle different types of messages
	switch(iType)
	{
		case mscOk:
			// plain old message
			hr = HandleDataMessage(pmess, pconn, bConnectionClosed);
			break;

		case mscReadError:
		case mscWriteError:
		case mscAcceptError:
			// REVIEW: display some type of message
			DisconnectHelper(pconn,FALSE);		
			break;

		case mscEvilUser:
			// REVIEW: display some type of message
			DisconnectHelper(pconn);		
			break;

		case mscConnect:
			// nothing
			break;

		case mscDisconnect:
		case mscDirtyDisconnect:
			DisconnectHelper(pconn);		
			break;

		case mscOutOfMemory:
			ASSERT(FALSE);
			break;

		default:
			ASSERT(FALSE);
			break;
	}

ERROR_ENCOUNTERED:

	SAFERELEASE(pconn);

	return hr;
}

HRESULT CVWClientObject::HandleDataMessage(IVWCommMessage* pmess, IVWCommConnection *pconn, BOOL& bConnectionClosed)
{
	HRESULT hr;
	IUnMarshallBuffer *pcb = NULL;
	BYTE ch = 0;
	DWORD cb = 0;
	unsigned long ulVersion = 0L;
	IWorld *pWorld;
	CWorldInfo *pwinfo;
	ConnectionInfo cinfo;

	// check if we've seen this connection before
	hr = m_clist.FindConnection(pconn, cinfo);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
	pWorld = cinfo.pWorld;
	if (pWorld == NULL)
	{
		// todo. real hr.
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	pwinfo = m_wlist.FindWorld(pWorld);
	if (pwinfo == NULL)
	{
		// todo. real hr.
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	
	hr = pmess->GetData(&pcb);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// check length
	if(FAILED(pcb->get_Length(&cb)) || cb<3)
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::HandleMessage: Bad message received from server\n");
		goto ERROR_ENCOUNTERED;
	}

#ifdef RECV_NETWORK_DEBUG
	{
		CString strTemp;

		strTemp.Format("Handle Message,%d,%d\n", GetTickCount(), cb);

		OutputDebugString((LPCTSTR)strTemp);
	}
#endif

	hr = pcb->get_ULong(&ulVersion);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (ulVersion != PROTOCOL_VERSION)
	{
		TRACE("CVWServer::ProcessDataMessage: Server version number not same as client version number\n");
		hr = VWOBJECT_E_INVALIDCLIENTVERSION;
		goto ERROR_ENCOUNTERED;
	}

	// check the admin byte
	hr = pcb->get_Byte(&ch);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (ch == 'A')
	{
		hr = pcb->get_Byte(&ch); // get second byte.
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = ProcessAdminMessage(pcb, ch, pconn, bConnectionClosed);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}
	else
	{
		hr = pcb->SeekToBegin();
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// might not have one in weird situation where
		// server sends us messages before ConnectLocal finishes
		if (pWorld)
		{
			hr = pWorld->ProcessMessage(pmess);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}
	
ERROR_ENCOUNTERED:
	SAFERELEASE(pcb);
	
	return hr;
}

STDMETHODIMP CVWClientObject::OnMessageAvailable_T2(IVWComm *pcomm)
{
	if (m_iMessagesInQueue == 0) 
	{		
#ifdef RECV_NETWORK_DEBUG
		{
			CString strTemp;

			strTemp.Format("OnMessageAvailable_T2 (posting WM_MESSAGE_AVAILABLE),%d\n", GetTickCount());

			OutputDebugString((LPCTSTR)strTemp);
		}
#endif

		if (pcomm==m_pPrimaryVWComm)
			if(m_pDummyWnd) m_pDummyWnd->PostMessage(WM_MESSAGE_AVAILABLE, 0, 0);
		else
			if(m_pDummyWnd) m_pDummyWnd->PostMessage(WM_MESSAGE_AVAILABLE, (UINT)pcomm, 0);
	}

	InterlockedIncrement(&m_iMessagesInQueue);

#ifdef RECV_NETWORK_DEBUG
	{
		CString strTemp;

		strTemp.Format("OnMessageAvailable_T2,%d,%d\n", GetTickCount(), m_iMessagesInQueue);

		OutputDebugString((LPCTSTR)strTemp);
	}
#endif

	return S_OK;
}

HRESULT CVWClientObject::FireOnConnect(VARIANT_BOOL fConnect, IWorld *pworld)
{
    HRESULT hr = S_OK;

	START_EVENT(CVWClientObject, IVWClientSite)

			if (fConnect == VARIANT_TRUE)
				hr = pEvent->OnConnect(pworld);
			else
				hr = pEvent->OnDisconnect(pworld);

	FINISH_EVENT()

    return hr;
}

HRESULT CVWClientObject::FireOnUserReconnect(IVWClient* newClient, IWorld* pWorld, IThing* pthing)
{
    HRESULT hr = S_OK;

	START_EVENT(CVWClientObject, IVWClientSite)

			hr = pEvent->OnUserReconnect(newClient,pWorld,pthing);

	FINISH_EVENT()

    return hr;
}

HRESULT CVWClientObject::FireOnUserConnect(IThing* pthing, VARIANT_BOOL fConnect)
{
    HRESULT hr = S_OK;

	START_EVENT(CVWClientObject, IVWClientSite)

			if (fConnect == VARIANT_TRUE)
				hr = pEvent->OnUserConnect(pthing);
			else
				hr = pEvent->OnUserDisconnect(pthing);

	FINISH_EVENT()

    return hr;
}

HRESULT CVWClientObject::FireOnTrace(BSTR bstr)
{
	HRESULT hr = S_OK;

	START_EVENT(CVWClientObject, IVWClientSite)

			hr = pEvent->OnTrace(bstr);

	FINISH_EVENT()

    return hr;
}

HRESULT CVWClientObject::FireOnReport(BSTR bstr, long lType)
{
	HRESULT hr = S_OK;

	START_EVENT(CVWClientObject, IVWClientSite)

			hr = pEvent->OnReport(bstr, lType);

	FINISH_EVENT()

    return hr;
}

HRESULT CVWClientObject::FireOnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg)
{
    HRESULT hr = S_OK;

	START_EVENT(CVWClientObject, IVWClientSite)

			hr = pEvent->OnUIEvent(pthing, bstrEventName, varArg);

	FINISH_EVENT()

    return hr;
}

LRESULT CDummyWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// bump stats
	m_count++;

	ASSERT(m_pVWClient);
	
	CVWClientObject *pClient = m_pVWClient;
	SAFEADDREF(pClient);

	//TRACE("CVWClientObject::WindowProc is being called in thread: %i, Client object: %i\n", ::GetCurrentThreadId(), pClient);

	if(!pClient) return 0L;

	switch (message)
	{

	case WM_MESSAGE_AVAILABLE: 

#ifdef RECV_NETWORK_DEBUG
		{
			CString strTemp;
			strTemp.Format("WM_MESSAGE_AVAILABLE,%d\n", GetTickCount());
			OutputDebugString((LPCTSTR)strTemp);
		}
#endif
		if (wParam==0)
		{
			if (pClient->m_pPrimaryVWComm != NULL)
				pClient->OnMessageAvailableHelper(m_pVWClient->m_pPrimaryVWComm);
		}
		else
		{
			pClient->OnMessageAvailableHelper((IVWComm *) wParam);
		}
		SAFERELEASE(pClient);
		return 0L;
	
	case WM_TIMER:
		if (wParam==idTimerCheckMessage)
		{
			// REVIEW: this is the hack "stuck message" timer.  It only
			// works with the "primary" comm pointer, so if we get stuck
			// handling messages for a secondary comm connection (e.g.
			// when going through inter-world portals, then we're out of 
			// luck

			if (pClient->m_pPrimaryVWComm != NULL)
			{
				pClient->OnMessageAvailableHelper(m_pVWClient->m_pPrimaryVWComm);
				if(pClient->m_pPrimaryVWComm)
				{	
					// Do keep alive processing here, too.
					pClient->m_pPrimaryVWComm->KeepAlive();
				}
			}
			SAFERELEASE(pClient);
			return 0L;
		}
		break;

	case WM_SOCKET_DISCONNECT:
		pClient->DisconnectHelper((IVWCommConnection *) wParam);		
		SAFERELEASE(pClient);
		return 0L;
	}

	SAFERELEASE(pClient);
	return CWnd::WindowProc(message, wParam, lParam);
}

HRESULT CVWClientObject::OnPropertyChanged(IThing* pthing, BSTR bstr, long lHint, VARIANT varHintData)
{
	HRESULT hr = S_OK;
#ifdef HANDLE_CLIENT_EVENTS
	IThing* pUser = NULL;
	IWorld* pWorld = NULL;
	IThing* pGlobal = NULL;
#ifdef _DEBUG
	CComBSTR bstrThingName("n/a");
#endif
	static CComBSTR bstrName("Name");
	static CComBSTR bstrGeometry("Geometry");
	static CComBSTR bstrBackgroundColor("BackgroundColor");
	static CComBSTR bstrExemplars("Exemplars");
	static CComBSTR kbstrUIVisible("IsUIVisible");

	if (bstr == NULL)
	{
#ifdef _DEBUG
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_IMPORTANT, "CVWClientObject::OnPropertyChanged: invalid property name\n");
#endif
		goto ERROR_ENCOUNTERED;
	}

	hr = pthing->get_World(&pWorld);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pWorld->get_User(&pUser);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (pUser == NULL)
	{
#ifdef _DEBUG
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_IMPORTANT, "CVWClientObject::OnPropertyChanged: no user logged in, exiting\n");
#endif
		goto ERROR_ENCOUNTERED;
	}

#ifdef _DEBUG
	if (pthing)
		pthing->get_Name(&bstrThingName.m_str);

	VWTRACE(m_pPrimaryWorld, "VWCLIENT", TRACE_CLIENT_EVENTS, "CVWClientObject::OnPropertyChanged: %s, hint %d, this (%s)\n", 
		CString(bstr), lHint, CString(bstrThingName));
#endif

	// Did an object's name change?
	if (CompareElements(&bstr, &bstrName.m_str))
	{
		/* hr = */ FireOnUIEvent(pthing, CComBSTR(VW_NAMECHANGED_EVENT_STR), CComVariant());
	}
	// Did an object's UIVisible change?
	else if (CompareElements(&bstr, &kbstrUIVisible.m_str))
	{
		/* hr = */ FireOnUIEvent(pthing, CComBSTR(VW_UIELEMENTCHANGED_EVENT_STR), CComVariant());
	}
	// Did an object's geometry change?
	else if (CompareElements(&bstr, &bstrGeometry.m_str))
	{
		/* hr = */ FireOnUIEvent(pthing, CComBSTR(VW_GEOMETRYCHANGED_EVENT_STR), CComVariant());
	}
	// Did a rooms's background color change?
	else if (CompareElements(&bstr, &bstrBackgroundColor.m_str))
	{
		/* hr = */ FireOnUIEvent(pthing, CComBSTR(VW_BACKGROUNDCOLORCHANGED_EVENT_STR), CComVariant());
	}
	else if (CompareElements(&bstr, &bstrExemplars.m_str))
	{
		// check to see if this is the global thing
		hr = pWorld->get_Global(&pGlobal);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		if (pGlobal == pthing)
			FireOnUIEvent(pthing, CComBSTR(VW_EXEMPLARS_CHANGED_EVENT_STR), CComVariant(lHint));
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pUser);
	SAFERELEASE(pWorld);
	SAFERELEASE(pGlobal);
#endif
	return hr;
}

HRESULT CVWClientObject::OnInvokeMethod(IThing* pthing, BSTR bstr, DISPPARAMS* pdispparams)
{
	HRESULT hr = S_OK;
#ifdef HANDLE_CLIENT_EVENTS
#ifdef _DEBUG
	CComBSTR bstrContainerName("n/a");
	CComBSTR bstrUserName("n/a");
	CComBSTR bstrName("n/a");
#endif
	IThing* pUser = NULL;
	IThing* pContainer = NULL;
	IWorld* pWorld = NULL;
	VARIANT_BOOL bContentEnterLeave = VARIANT_FALSE;
	static CComBSTR bstrContentTell("OnContentSystemTell");
    static CComBSTR bstrContentSystemEnter("OnContentSystemEnter");
    static CComBSTR bstrContentSystemLeave("OnContentSystemLeave");
	static CComBSTR bstrOnWield("OnWield");
	static CComBSTR bstrOnUnWield("OnUnWield");
    static CComBSTR bstrContentSystemConnect("OnContentSystemConnect");
    static CComBSTR bstrContentSystemDisconnect("OnContentSystemDisconnect");
    static CComBSTR bstrDoGesture("DoGesture");
    static CComBSTR bstrWear("Wear");
    static CComBSTR bstrUnWear("UnWear");

	if (bstr == NULL)
	{
#ifdef _DEBUG
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_IMPORTANT, "CVWClientObject::OnInvokeMethod: invalid method name\n");
#endif
		goto ERROR_ENCOUNTERED_1;
	}

	hr = pthing->get_World(&pWorld);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED_1;

	hr = pWorld->get_User(&pUser);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED_1;

	if (pUser == NULL)
	{
#ifdef _DEBUG
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_IMPORTANT, "CVWClientObject::OnInvokeMethod: no user logged in, exiting\n");
#endif
		goto ERROR_ENCOUNTERED_1;
	}

	// user events
	if (pUser == pthing)
	{
		if (CompareElements(&bstr, &bstrContentSystemEnter.m_str) || 
			CompareElements(&bstr, &bstrContentSystemLeave.m_str))
		{
			// if we know it's one or the other, flag it
			bContentEnterLeave = VARIANT_TRUE;

			// params: 0: where, 1: what
			// but DISPPARAMS are in reverse order
			IThing* pWhere = (IThing*)pdispparams->rgvarg[1].pdispVal;
			IThing* pWhat = (IThing*)pdispparams->rgvarg[0].pdispVal;

			VARIANT_BOOL bEnter = CompareElements(&bstr, &bstrContentSystemEnter.m_str) ? VARIANT_TRUE : VARIANT_FALSE;

			ASSERT(pWhere == pUser);

			if (bEnter == VARIANT_TRUE)
				/* hr = */ FireOnUIEvent(pthing, CComBSTR(VW_ENTERINVENTORY_EVENT_STR), CComVariant((IDispatch*)pWhat));
			else
				/* hr = */ FireOnUIEvent(pthing, CComBSTR(VW_LEAVEINVENTORY_EVENT_STR), CComVariant((IDispatch*)pWhat));
		}
		else if (CompareElements(&bstr, &bstrWear.m_str))
		{
			// params: 0: what
			IThing* pWhat = (IThing*)pdispparams->rgvarg[0].pdispVal;

			/* hr = */ FireOnUIEvent(pthing, CComBSTR(VW_WEAR_EVENT_STR), CComVariant((IDispatch*)pWhat));
		}
		else if (CompareElements(&bstr, &bstrUnWear.m_str))
		{
			// params: 0: what
			IThing* pWhat = (IThing*)pdispparams->rgvarg[0].pdispVal;

			/* hr = */ FireOnUIEvent(pthing, CComBSTR(VW_UNWEAR_EVENT_STR), CComVariant((IDispatch*)pWhat));
		}
		else if (CompareElements(&bstr, &bstrDoGesture.m_str))
		{
			// params: 0: gesture
			int iGesture = (int)pdispparams->rgvarg[0].iVal;

			/* hr = */ FireOnUIEvent(pthing, CComBSTR(VW_DOGESTURE_EVENT_STR), CComVariant(iGesture));
		}
		else if (CompareElements(&bstr, &bstrOnWield.m_str) || 
			CompareElements(&bstr, &bstrOnUnWield.m_str))
		{
			// params: 0: what
			// but DISPPARAMS are in reverse order

			VARIANT_BOOL bWield = CompareElements(&bstr, &bstrOnWield.m_str) ? VARIANT_TRUE : VARIANT_FALSE;

			if (bWield == VARIANT_TRUE)
			{
				IThing* pWhat = (IThing*)pdispparams->rgvarg[0].pdispVal;

				/* hr = */ FireOnUIEvent(pthing, CComBSTR(VW_WIELD_EVENT_STR), CComVariant((IDispatch*)pWhat));
			}
			else
			{
				/* hr = */ FireOnUIEvent(pthing, CComBSTR(VW_UNWIELD_EVENT_STR), CComVariant());
			}
		}
		else if (CompareElements(&bstr, &bstrContentTell.m_str))
		{
			IThing* pFrom = (IThing*)pdispparams->rgvarg[3].pdispVal;
			IPropertyList* pToList = (IPropertyList*)pdispparams->rgvarg[2].pdispVal;
			BSTR bstrTellMsg = pdispparams->rgvarg[1].bstrVal;
			long lval = pdispparams->rgvarg[0].lVal;

#ifdef _DEBUG
			CString strTell(bstrTellMsg);
			if (strTell.Right(1) != _T("\n"))
				strTell += "\n";
			OutputDebugString((LPCTSTR)strTell);
#endif
						  
			{
				IPropertyList* pproplist;

				hr = ::CreatePropertyList(m_pPrimaryWorld, &pproplist);
				if (SUCCEEDED(hr))
				{
					// orphan this proplist
					/* hr = */ pproplist->put_World(NULL);

					hr = pproplist->AddThing(pFrom);
					if (FAILED(hr))
						goto ERROR_ENCOUNTERED_TELL;

					hr = pproplist->AddObjectProperty(pToList);
					if (FAILED(hr))
						goto ERROR_ENCOUNTERED_TELL;

					hr = pproplist->AddString(bstrTellMsg);
					if (FAILED(hr))
						goto ERROR_ENCOUNTERED_TELL;

					hr = pproplist->AddLong(lval);
					if (FAILED(hr))
						goto ERROR_ENCOUNTERED_TELL;

					hr = FireOnUIEvent(pthing, CComBSTR(VW_TELL_EVENT_STR), CComVariant(pproplist));

ERROR_ENCOUNTERED_TELL:
					SAFERELEASE(pproplist);
				}
			}
		}

	}
	// room events
	else 
	{
		hr = pUser->get_Container(&pContainer);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED_1;

		if (pContainer == pthing)
		{
			if (CompareElements(&bstr, &bstrContentSystemEnter.m_str) || 
				CompareElements(&bstr, &bstrContentSystemLeave.m_str))
			{
				// if we know it's one or the other, flag it
				bContentEnterLeave = VARIANT_TRUE;

				// params: 0: where, 1: what
				// but DISPPARAMS are in reverse order
				IThing* pWhere = (IThing*)pdispparams->rgvarg[1].pdispVal;
				IThing* pWhat = (IThing*)pdispparams->rgvarg[0].pdispVal;

				VARIANT_BOOL bEnter = CompareElements(&bstr, &bstrContentSystemEnter.m_str) ? VARIANT_TRUE : VARIANT_FALSE;

				ASSERT(pWhere == pContainer);

				if (bEnter == VARIANT_TRUE)
					/* hr = */ FireOnUIEvent(pthing, CComBSTR(VW_ENTERROOM_EVENT_STR), CComVariant((IDispatch*)pWhat));
				else
					/* hr = */ FireOnUIEvent(pthing, CComBSTR(VW_LEAVEROOM_EVENT_STR), CComVariant((IDispatch*)pWhat));
			}
			else if (CompareElements(&bstr, &bstrContentSystemConnect.m_str) || 
				CompareElements(&bstr, &bstrContentSystemDisconnect.m_str))
			{
				// params: 0: who
				// but DISPPARAMS are in reverse order
				IThing* pWho = (IThing*)pdispparams->rgvarg[0].pdispVal;

				// FireOnUserConnect takes pWho, bConnect

				// ConnectInt/DisconnectInt take care of events for user
				if (pWho != pUser)
				{
					VARIANT_BOOL bConnect = CompareElements(&bstr, &bstrContentSystemConnect.m_str) ? VARIANT_TRUE : VARIANT_FALSE;

					hr = FireOnUserConnect(pWho, bConnect);
				}
			}
			else if (CompareElements(&bstr, &bstrContentTell.m_str))
			{
				IThing* pFrom = (IThing*)pdispparams->rgvarg[3].pdispVal;
				IPropertyList* pToList = (IPropertyList*)pdispparams->rgvarg[2].pdispVal;
				BSTR bstrTellMsg = pdispparams->rgvarg[1].bstrVal;
				long lval = pdispparams->rgvarg[0].lVal;

#ifdef _DEBUG
				CString strTell(bstrTellMsg);
				if (strTell.Right(1) != _T("\n"))
					strTell += "\n";
				OutputDebugString((LPCTSTR)strTell);
#endif
							  
				{
					IPropertyList* pproplist;

					hr = ::CreatePropertyList(m_pPrimaryWorld, &pproplist);
					if (SUCCEEDED(hr))
					{
						// orphan this proplist
						/* hr = */ pproplist->put_World(NULL);

						hr = pproplist->AddThing(pFrom);
						if (FAILED(hr))
							goto ERROR_ENCOUNTERED_2;

						hr = pproplist->AddObjectProperty(pToList);
						if (FAILED(hr))
							goto ERROR_ENCOUNTERED_2;

						hr = pproplist->AddString(bstrTellMsg);
						if (FAILED(hr))
							goto ERROR_ENCOUNTERED_2;

						hr = pproplist->AddLong(lval);
						if (FAILED(hr))
							goto ERROR_ENCOUNTERED_2;

						hr = FireOnUIEvent(pthing, CComBSTR(VW_TELL_EVENT_STR), CComVariant(pproplist));

ERROR_ENCOUNTERED_2:
						SAFERELEASE(pproplist);
					}
				}
			}
		}
	}

#ifdef _DEBUG
	if (pthing)
		pthing->get_Name(&bstrName.m_str);
	if (pUser)
		pUser->get_Name(&bstrUserName.m_str);
	if (pContainer)
		pContainer->get_Name(&bstrContainerName.m_str);

	VWTRACE(m_pPrimaryWorld, "VWCLIENT", TRACE_CLIENT_EVENTS, "CVWClientObject::OnInvokeMethod: %s, this (%s), container (%s), user (%s)\n", 
		CString(bstr), CString(bstrName), CString(bstrContainerName), CString(bstrUserName));
#endif

	// oncontententer/leave for any object
	if (bContentEnterLeave == VARIANT_TRUE ||
		CompareElements(&bstr, &bstrContentSystemEnter.m_str) || 
		CompareElements(&bstr, &bstrContentSystemLeave.m_str))
	{
		// params: 0: where, 1: what
		// but DISPPARAMS are in reverse order
		IThing* pWhere = (IThing*)pdispparams->rgvarg[1].pdispVal;
		IThing* pWhat = (IThing*)pdispparams->rgvarg[0].pdispVal;

		VARIANT_BOOL bEnter = CompareElements(&bstr, &bstrContentSystemEnter.m_str) ? VARIANT_TRUE : VARIANT_FALSE;

		if (bEnter == VARIANT_TRUE)
			/* hr = */ FireOnUIEvent(pWhere, CComBSTR(VW_CONTENTENTER_EVENT_STR), CComVariant(pWhat));
		else
			/* hr = */ FireOnUIEvent(pWhere, CComBSTR(VW_CONTENTLEAVE_EVENT_STR), CComVariant(pWhat));
	}

ERROR_ENCOUNTERED_1:
	SAFERELEASE(pUser);
	SAFERELEASE(pContainer);
	SAFERELEASE(pWorld);
#endif
	return hr;
};

STDMETHODIMP CVWClientObject::CreateTool(BSTR bstrName, REFCLSID clsid, IUnknown** ppunk)
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
		return ReportClientError(hr);

	if (fValid)
	{
#ifdef _DEBUG
		VWTRACE(m_pPrimaryWorld, "VWOBJECT", VWT_ERROR, "CVWClientObject::CreateTool: tool already exists (%s)\n", str);
#endif
		return ReportClientError(VWOBJECT_E_TOOLEXISTS);
	}

	hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (LPVOID *)ppunk);
	if (FAILED(hr))
		return ReportClientError(hr);

	hr = m_pTools->put_Property(bstrName, CComVariant(*ppunk));

	return ReportClientError(hr);
}

STDMETHODIMP CVWClientObject::CreateToolFromProgID(BSTR bstrName, BSTR progid, IUnknown** ppunk)
{
	CLSID clsid;

	if (SUCCEEDED(CLSIDFromProgID(progid, &clsid)))
		return CreateTool(bstrName, clsid, ppunk);
	else
	{
#ifdef _DEBUG
		CString str(progid);
		VWTRACE(m_pPrimaryWorld, "VWOBJECT", VWT_ERROR, "CVWClientObject::CreateToolFromProgID: unknown progID (%s)\n", str);
#endif
		return ReportClientError(VWOBJECT_E_UNKNOWNPROGID);
	}
}

STDMETHODIMP CVWClientObject::get_Tool(BSTR bstrName, IUnknown** ppunk)
{
	HRESULT hr = S_OK;
	VARIANT var;
	::VariantInit(&var);

	ASSERT(ppunk);
	if (ppunk == NULL)
		return E_POINTER;

	if (m_pTools == NULL)
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::get_Tool: invalid tool map\n");
		return ReportClientError(VWOBJECT_E_INVALIDTOOLMAP);
	}

	hr = m_pTools->get_Property(bstrName, &var);
	if (FAILED(hr))
	{
		if (hr == VWOBJECT_E_PROPERTYNOTEXIST)
			hr = VWOBJECT_E_TOOLNOTEXIST;
		goto ERROR_ENCOUNTERED;
	}

	if (var.vt != VT_UNKNOWN)
	{
#ifdef _DEBUG
		VWTRACE(m_pPrimaryWorld, "VWOBJECT", VWT_ERROR, "CVWClientObject::get_Tool: name (%s), property not IUnknown (%d)\n", CString(bstrName), var.vt);
#endif

		::VariantClear(&var);

		return VWOBJECT_E_WRONGPROPERTYTYPE;
	}

	*ppunk = var.punkVal;

ERROR_ENCOUNTERED:
	return ReportClientError(hr);
}

STDMETHODIMP CVWClientObject::RemoveTool(BSTR name)
{
	ASSERT(m_pTools);

	if (m_pTools == NULL)
	{
		VWTRACE(m_pPrimaryWorld, "VWCLIENT", VWT_ERROR, "CVWClientObject::RemoveTool: invalid tool map\n");
		return ReportClientError(VWOBJECT_E_INVALIDTOOLMAP);
	}

	HRESULT hr = S_OK;

	hr = m_pTools->Remove(name);

	return ReportClientError(hr);
}

STDMETHODIMP CVWClientObject::get_Tools(IPropertyMap** pppropertymap)
{
	ASSERT(pppropertymap);
	if (pppropertymap == NULL)
		return E_POINTER;

	*pppropertymap = m_pTools;

	SAFEADDREF(*pppropertymap);

	return S_OK;
}

HRESULT CVWClientObject::CreateTools()
{
	HRESULT hr = S_OK;

	return hr;
}

void CVWClientObject::UpdateWorldReferences(IWorld * pWorld)
{
	CComVariant var;
	CVWComPtr<IObjectProperty,NULL,&IID_IObjectProperty> PropPtr;

	if (m_pTools)
	{
//		m_pTools->put_World(pWorld);

		VARIANT_BOOL done = VARIANT_FALSE;
		
		HRESULT hr = m_pTools->FirstItem(NULL, &var, &done);

		while (done == VARIANT_FALSE && SUCCEEDED(hr))
		{
			PropPtr = var;
			if (PropPtr != NULL)
				PropPtr->put_World(pWorld);

			// clear out data
			var.Clear();
			PropPtr.Release();

			hr = m_pTools->NextItem(NULL, &var, &done);
		}
	}
}

STDMETHODIMP CVWClientObject::Report(BSTR bstr, long lType)
{
	return FireOnReport(bstr, lType);
}

STDMETHODIMP CVWClientObject::Trace(BSTR bstr)
{
	return FireOnTrace(bstr);
}

HRESULT CVWClientObject::ConnectToServerInt(BSTR bstrURL, VARIANT_BOOL bAuthenticate,IVWComm** ppVWComm,
	IVWCommConnection** ppVWConnection )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HRESULT hr = S_OK;
	int nPort;
	CComBSTR bstrProtocol;
	CComBSTR bstrServer;
	CString strMessage;
	DWORD cbMessage;
	CComMarshallBuffer* pbuffer = NULL;
	IUnMarshallBuffer* pbufReply = NULL;
	IAuthentication *pauth = NULL;
	int cbBlob = 0;
	BYTE *prgchBlob = NULL;
	HRESULT hrReply;
	VARIANT_BOOL bAuthenticationDone = VARIANT_FALSE;
	VARIANT_BOOL bTunnel;
	VARIANT_BOOL bShowDialog = VARIANT_FALSE;

	VWTRACE(NULL, "VWCLIENT", VWT_VERBOSE, "CVWClientObject::ConnectToServer\n");

	// initialize
	hr = ParseURLHelper(bstrURL, NULL, &bstrProtocol.m_str, &bstrServer.m_str, &nPort);
	if (FAILED(hr))  
	{
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::ConnectToServer: invalid URL format\n");        
		goto ERROR_ENCOUNTERED;
	}

	hr = CoCreateInstance(CLSID_VWComm, NULL, CLSCTX_INPROC_SERVER, IID_IVWComm, (LPVOID *)ppVWComm);
	if (FAILED(hr))
	{		
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::ConnectToServer: Failed to create VWComm object\n");	
		goto ERROR_ENCOUNTERED;
	}

	hr = (*ppVWComm)->Initialize(this);
	if (FAILED(hr))
	{
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::ConnectToServer: Failed to set VWComm site\n");
		goto ERROR_ENCOUNTERED;
	}

	if (CString(bstrProtocol)=="HTTP")
		bTunnel = VARIANT_TRUE;
	else
		bTunnel = VARIANT_FALSE;

	hr = (*ppVWComm)->Connect((LPSTR)(LPCSTR)CString(bstrServer), nPort, bTunnel, ppVWConnection);
	if (FAILED(hr))
	{
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::ConnectToServer: Failed to connect to server\n");
		goto ERROR_ENCOUNTERED;
	}

	// bookkeeping
	if (bAuthenticate == VARIANT_TRUE)
	{
		// load the security stuff if we already haven't
		if (m_pAuthenticate == NULL)
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
			// REVIEW: [T.O.] Should we use 'Negotiate' for future compatibility with kerberos
			hr = m_pAuthenticate->put_Package(CComBSTR("NTLM"));
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}

		// see if the connection even has an authentication
		hr = (*ppVWConnection)->get_Authentication(&pauth);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (pauth == NULL)
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
			hr = (*ppVWConnection)->put_Authentication(pauth);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

	// create marshall buffer
	pbuffer = new CComMarshallBuffer();
	if (pbuffer == NULL)
	{
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::ConnectToServer: failed to allocate CComMarshallBuffer\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	pbuffer->AddRef();

	// now send message indicating that we just want to log on
	for(;;)
	{
		// compose the message
		strMessage = 'A';		// 'A' is for "Administrative" messages
		strMessage += 'L';		// 'L' is for "Logon" message

		// get and add security information
		if (bAuthenticate)
		{
			// If we've failed with the current credentials pass a Reset flag
			// else just pass the Yes flag
			if (bShowDialog==VARIANT_TRUE && hrReply!=VWOBJECT_E_INVALIDSECURITY)
				strMessage += 'R';
			else
				strMessage += 'Y';

			// should have fetched authentication earlier
			ASSERT(pauth);

			// get a client context
			hr = m_pAuthenticate->GenerateClientContext(pauth, bShowDialog, &bAuthenticationDone);

			// Check if we have not yet logged in or otherwise have invalid credentials.. lets pop dialog and retry
			if ((hr == SEC_E_NO_CREDENTIALS) || (hr ==  SEC_E_UNKNOWN_CREDENTIALS) || (hr == SEC_E_LOGON_DENIED))
			{
				// first reset the authenticaiton
				pauth->put_Initialized(VARIANT_FALSE);

				pauth->put_Authenticated(VARIANT_FALSE);

				// then set the flag for GenerateClientContext to prompt
				bShowDialog = VARIANT_TRUE;

				// everything is ok.. really.. you can trust us ;-)
				hr = S_OK;
				hrReply = S_OK;

				// jump back into for loop
				goto CONTINUE;
			}

			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			// get the blob from the authentication
			hr = pauth->GetBlobLength(&cbBlob);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			if (cbBlob>0)
			{
				// allocate a buffer
				prgchBlob = new BYTE[cbBlob];
				if (prgchBlob == NULL)
				{
					hr = E_OUTOFMEMORY;
					goto ERROR_ENCOUNTERED;
				}

				// get the blob
				hr = pauth->CopyBlob(prgchBlob);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;
			}

		}
		else
		{
			strMessage += 'N';
		}

		cbMessage = strMessage.GetLength()+1;

		// write out client version to send to server
		hr = pbuffer->put_ULong(PROTOCOL_VERSION);
		if (FAILED(hr))
		{
			VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::ConnectToServer: Failed to write to marshall buffer\n");
			goto ERROR_ENCOUNTERED;
		}

		hr = pbuffer->put_Bytes((BYTE *) (LPCSTR) strMessage, cbMessage);
		if (FAILED(hr))
		{
			VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::ConnectToServer: Failed to attach to marshall buffer\n");
			goto ERROR_ENCOUNTERED;
		}

		// add the blob length
		hr = pbuffer->put_Int(cbBlob);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// add the blob
		if (cbBlob>0 && prgchBlob != NULL)
		{
			hr = pbuffer->put_Bytes(prgchBlob, cbBlob);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			// free up blob
			delete [] prgchBlob;
			prgchBlob = NULL;
			cbBlob = 0;
		}

		// send it, waiting for reply
		hr = (*ppVWConnection)->SendSync(pbuffer, &pbufReply);
		if (FAILED(hr))
		{
			VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::ConnectToServer: Failed to send\n");
			goto ERROR_ENCOUNTERED;
		}

		hr = pbufReply->get_Long((long*)&hrReply);
		if (FAILED(hr))
		{
			VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::ConnectToServer: Failed to get text of reply to World URL message\n");
			goto ERROR_ENCOUNTERED;
		}

		// if succeeded, then we're done
		if (SUCCEEDED(hrReply))
			break;

		//this is just a warning case.. trace it and reset the hresult
		if (hrReply == VWOBJECT_E_UNAUTHENTICATEDCONNECTION)
		{
			if (bAuthenticate == VARIANT_TRUE)
				VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::ConnectToServer: Non-Authenticated Connection : Client requested Authentication but Server is not in authentication mode\n");
			hr = S_OK;
			break;
		}

		// check if our credentials were denied or were generally invalid.. if so we need to prompt
		if ((hrReply == SEC_E_LOGON_DENIED) || (hrReply ==  SEC_E_UNKNOWN_CREDENTIALS)   || (hrReply == SEC_E_NO_CREDENTIALS) || (hrReply == VWSERVER_E_GROUPAUTHENTICATIONFAILURE))
		{
			// first reset the authenticaiton
			pauth->put_Initialized(VARIANT_FALSE);

			// then set the flag for GenerateClientContext to prompt
			bShowDialog=VARIANT_TRUE;

			// everything is ok.. really.. you can trust us ;-)
			hr=S_OK;

			// jump back into for loop
			goto CONTINUE;
		}

		// else, check why we failed. If failed due to security, keep working on it
		if (hrReply != VWOBJECT_E_INVALIDSECURITY)
		{
			VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::ConnectToServer: Server failed to connect to World URL\n");
			hr = hrReply;
			goto ERROR_ENCOUNTERED;
		}

		ASSERT(pauth != NULL);

		// read the new blob out of it
		hr = pbufReply->get_Int(&cbBlob);
		if (FAILED(hr))
		{
			VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::ConnectToServer: Failed to read length of Blob in reply\n");
			goto ERROR_ENCOUNTERED;
		}

		ASSERT(prgchBlob == NULL);
		prgchBlob = new BYTE[cbBlob];
		if (prgchBlob == NULL)
		{
			hr = E_OUTOFMEMORY;
			goto ERROR_ENCOUNTERED;
		}

		hr = pbufReply->get_Bytes(prgchBlob, cbBlob);
		if (FAILED(hr))
		{
			VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::ConnectToServer: Failed to read Blob in reply\n");
			goto ERROR_ENCOUNTERED;
		}

		// stick the blob in the authentication
		hr = pauth->SetBlob(prgchBlob, cbBlob);
		if (FAILED(hr))
		{
			VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::ConnectToServer: Failed to set Blob as per reply\n");
			goto ERROR_ENCOUNTERED;
		}

		// free stuff
		delete [] prgchBlob;
		prgchBlob = NULL;
		cbBlob = 0;
CONTINUE:
		SAFERELEASE(pbufReply);
	}

	// remember server
	m_bstrPrimaryServer = bstrServer;

ERROR_ENCOUNTERED:
	// if we failed, then clean up

	SAFERELEASE(pbuffer);
	SAFERELEASE(pbufReply);
	SAFERELEASE(pauth);

	return hr;
}


STDMETHODIMP CVWClientObject::ConnectToServer(BSTR bstrURL, VARIANT_BOOL bAuthenticate, IVWAdmin** ppAdmin)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HRESULT hr = S_OK;
	CComBSTR bstrProtocol;
	CComBSTR bstrServer;
	CString strMessage;
	CComMarshallBuffer* pbuffer = NULL;
	IUnMarshallBuffer* pbufReply = NULL;
	IAuthentication *pauth = NULL;
	int cbBlob = 0;
	BYTE *prgchBlob = NULL;
	VARIANT_BOOL bAuthenticationDone = VARIANT_FALSE;

	if (ppAdmin == NULL)
	{
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::ConnectToServer: outgoing pointer (IVWAdmin**) is NULL\n");
		return E_POINTER;
	}

	VWTRACE(NULL, "VWCLIENT", VWT_VERBOSE, "CVWClientObject::ConnectToServer\n");

	// shouldn't call Connect if already connected
	if (m_pPrimaryWorld != NULL	||
		m_pPrimaryVWComm != NULL ||
		m_pPrimaryCon != NULL)
	{
		hr = VWCLIENT_E_ALREADYCONNECTED;
		return hr;
	}

	hr=ConnectToServerInt(bstrURL, bAuthenticate, &m_pPrimaryVWComm, &m_pPrimaryCon);
	if (FAILED(hr))  
	{
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::ConnectToServer: Failed to connect\n");        
		goto ERROR_ENCOUNTERED;
	}


ERROR_ENCOUNTERED:
	// if we failed, then clean up
	if (FAILED(hr))
	{
		SAFERELEASE(*ppAdmin);
	}

	return hr;
}

STDMETHODIMP CVWClientObject::CreateWorld(BSTR bstrWorldName, IWorld** ppWorld)
{
	return OpenOrCreateWorld(bstrWorldName, VARIANT_TRUE, ppWorld);
}

STDMETHODIMP CVWClientObject::OpenWorld(BSTR bstrWorldName, IWorld** ppWorld)
{
	return OpenOrCreateWorld(bstrWorldName, VARIANT_FALSE, ppWorld);
}

HRESULT CVWClientObject::OpenOrCreateWorld(BSTR bstrWorldName, VARIANT_BOOL bCreate, IWorld **ppWorld)
{
	HRESULT hr = S_OK;
	CComMarshallBuffer* pbuffer = NULL;
	IUnMarshallBuffer* pbufReply = NULL;
	CString strMessage;
	DWORD cbMessage;
	HRESULT hrReply;
	DWORD dwCookie;
	CString strWorldID;

	if (ppWorld == NULL)
		return E_POINTER;

	// initialize
	*ppWorld = NULL;

	// we must have an existing, secure, connection
	if (m_pPrimaryCon == NULL)
	{
		hr = VWOBJECT_E_INVALIDCONNECTION;
		goto ERROR_ENCOUNTERED;
	}

	// can't be in a world already
	if (m_pPrimaryWorld != NULL)
	{
		hr = VWCLIENT_E_ALREADYCONNECTED;
		goto ERROR_ENCOUNTERED;
	}

	// start up VWODB (overwrite any local database). Sets pWorld.
	hr = CreateLocalWorld(NULL, VARIANT_TRUE, ppWorld, &dwCookie);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// initialize world with connection 
	hr = (*ppWorld)->InitializeConnection(m_pPrimaryCon);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// create marshall buffer
	pbuffer = new CComMarshallBuffer();
	if (pbuffer == NULL)
	{
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::CreateWorld: failed to allocate CComMarshallBuffer\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	pbuffer->AddRef();

	// send CreateWorld message

	// compose the message.  'A' is for "Administrative" messages
	strMessage = 'A';
	if (bCreate==VARIANT_TRUE)
		// 'C' is for "Create" message
		strMessage += 'C';		
	else
		// 'W' is for "Open" message (yep, it really is)
		strMessage += 'W';

	cbMessage = strMessage.GetLength()+1;

	// write out client version to send to server
	hr = pbuffer->put_ULong(PROTOCOL_VERSION);
	if (FAILED(hr))
	{
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::CreateWorld: Failed to write to marshall buffer\n");
		goto ERROR_ENCOUNTERED;
	}

	hr = pbuffer->put_Bytes((BYTE *) (LPCSTR) strMessage, cbMessage);
	if (FAILED(hr))
	{
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::CreateWorld: Failed to attach to marshall buffer\n");
		goto ERROR_ENCOUNTERED;
	}

	hr = pbuffer->put_String(bstrWorldName, VARIANT_FALSE);
	if (FAILED(hr))
	{
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::CreateWorld: Failed to write to marshall buffer\n");
		goto ERROR_ENCOUNTERED;
	}

	// send it, waiting for reply
	hr = m_pPrimaryCon->SendSync(pbuffer, &pbufReply);
	if (FAILED(hr))
	{
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::CreateWorld: Failed to send\n");
		goto ERROR_ENCOUNTERED;
	}

	hr = pbufReply->get_Long((long*)&hrReply);
	if (FAILED(hr))
	{
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::CreateWorld: Failed to get return value\n");
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hrReply))
	{
		VWTRACE(NULL, "VWCLIENT", VWT_ERROR, "CVWClientObject::CreateWorld: Server failed to create world\n");
		hr = hrReply;
		goto ERROR_ENCOUNTERED;
	}

	// put in connection and world lists
	m_clist.AddConnection(m_pPrimaryCon, *ppWorld, m_pPrimaryVWComm, CComBSTR(""));

	// does own ref count
	strWorldID = CString(m_bstrPrimaryServer) + "/" + CString(bstrWorldName);
	m_wlist.AddWorld(strWorldID, *ppWorld, dwCookie);

	// set up globals
	m_pPrimaryWorld = *ppWorld;
	SAFEADDREF(m_pPrimaryWorld);

ERROR_ENCOUNTERED:
	SAFERELEASE(pbuffer);
	SAFERELEASE(pbufReply);

	return hr;
}

