// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWClientObj.h : Declaration of the CVWClientObject

#include <resource.h>

#include <vwadmin.h>
#include <vwcomm.h>
#include <dirserv.h>

#include "..\vwserver\wldlist.h"
#include "..\vwserver\connserv.h"

#include <propbase.h>

EXTERN_C const CLSID CLSID_VWClient;
EXTERN_C const CLSID IID_IVWClientSite;

class CVWClientObject;

#define WM_MESSAGE_AVAILABLE (WM_USER+1)
#define WM_SOCKET_DISCONNECT (WM_USER+2)

class CDummyWnd : public CWnd
{
public:
	CDummyWnd(CVWClientObject* pvw) : m_pVWClient(pvw), m_count(0) { };

	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );

private:
	CVWClientObject* m_pVWClient;
	ULONG m_count;
};

#define ReportClientError(hr) SUCCEEDED(hr) ? hr : ReportError("VWSYSTEM.Client.1", IID_IVWClient, hr, 0x300, 0x700)

/////////////////////////////////////////////////////////////////////////////
// vwclient

// REVIEW -- using pointers to ID's is necessary because some compilers don't like
// references as template arguments.

class ATL_NO_VTABLE CVWClientObject :
	public IDispatchImpl<IVWClient, &IID_IVWClient, &LIBID_VWSYSTEMLib>,
	public ISupportErrorInfo,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVWClientObject, &CLSID_VWClient>,
	public IConnectionPointImpl<CVWClientObject, &IID_IVWClientSite, CComDynamicUnkArray>,
	public IConnectionPointContainerImpl<CVWClientObject>,
	public IVWObjectSite,
	public IVWCommSite
{
DECLARE_AGGREGATABLE(CVWClientObject)

friend CDummyWnd;

public:
	CVWClientObject();
	~CVWClientObject();

	HRESULT FinalConstruct(void);

	void FinalRelease(void) 
	{
		// need to call disconnect before releasing IVWClient
		if (m_pPrimaryWorld)
			Disconnect();

		CComObjectRootEx<CComMultiThreadModel>::FinalRelease();
	}

BEGIN_COM_MAP(CVWClientObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWClient)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IVWCommSite)
	COM_INTERFACE_ENTRY(IVWObjectSite)
END_COM_MAP()

// Connection Point
public:
	BEGIN_CONNECTION_POINT_MAP(CVWClientObject)
		CONNECTION_POINT_ENTRY(IID_IVWClientSite)
	END_CONNECTION_POINT_MAP()
	
DECLARE_PROTECT_FINAL_CONSTRUCT()

DECLARE_REGISTRY(CLSID_VWClient, "VWSYSTEM.Client.1", "VWSYSTEM.Client", IDS_VWCLIENT_DESC, THREADFLAGS_BOTH)

// IVWClient
public:
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IVWClient
	STDMETHOD(ConnectLocalEx)(BSTR bstr, VARIANT_BOOL bOverwrite, IWorld** ppWorld);
	STDMETHOD(ConnectLocal)(BSTR bstr, IWorld** ppWorld);
	STDMETHOD(Connect)(BSTR bstrURL, IWorld** ppWorld);
	STDMETHOD(Disconnect)(void);

	STDMETHOD(ConnectToServer)(BSTR bstrURL, VARIANT_BOOL bAuthenticated, IVWAdmin** ppAdmin);
	STDMETHOD(CreateWorld)(BSTR bstrWorldName, IWorld** ppWorld);
	STDMETHOD(OpenWorld)(BSTR bstrWorldName, IWorld** ppWorld);

	STDMETHOD(get_World)(IWorld** ppWorld);

	STDMETHOD(get_IsConnected)(VARIANT_BOOL* pBool);

	STDMETHOD(CreateTool)(BSTR name, REFCLSID clsid, IUnknown** ppunk);
	STDMETHOD(CreateToolFromProgID)(BSTR name, BSTR bstr, IUnknown** ppunk);
	STDMETHOD(RemoveTool)(BSTR name);

	STDMETHOD(get_Tool)(BSTR name, IUnknown** ppunk);

	STDMETHOD(get_Tools)(IPropertyMap** pppropertymap);

	STDMETHOD(Report)(BSTR bstr, long lType);
	STDMETHOD(Trace)(BSTR bstr);

// IVWCommSite
public:
	STDMETHOD(OnMessageAvailable)(IVWComm *);	
	STDMETHOD(OnMessageAvailable_T2)(IVWComm *);

// IVWObjectSite
public:
	STDMETHOD(OnTrace)(BSTR bstr)
	{
#ifdef _DEBUG
		CString str(bstr);
		if (str.Right(1) != _T("\n"))
			str += "\n";
		OutputDebugString((LPCTSTR)str);
#endif

		return FireOnTrace(bstr);
	};

	STDMETHOD(OnReport)(BSTR bstr, long lVal)
	{ 
#if 1 // def _DEBUG
		CString str(bstr);
		if (str.Right(1) != _T("\n"))
			str += "\n";
		OutputDebugString((LPCTSTR)str);
#endif

		return FireOnReport(bstr, lVal);
	};

	STDMETHOD(OnInvokeMethod)(IThing* pthing, BSTR bstr, DISPPARAMS* pdispparams);

	STDMETHOD(OnPropertyChanged)(IThing* pthing, BSTR bstr, long lHint, VARIANT varHintData);

	STDMETHOD(OnMethodChanged)(IThing* pthing, BSTR bstr, long lHint, VARIANT varHintData)
	{
		return S_OK;
	};

	STDMETHOD(OnModuleChanged)(BSTR bstr, long lHint, VARIANT varHintData)
	{
		return S_OK;
	};

	STDMETHOD(OnUserConnect)(IThing* pthing)
	{
		return FireOnUserConnect(pthing, VARIANT_TRUE);
	};

	STDMETHOD(OnUserDisconnect)(IThing* pthing)
	{
		return FireOnUserConnect(pthing, VARIANT_FALSE);
	};

	STDMETHOD(OnUIEvent)(IThing *pthing, BSTR bstrEventName, VARIANT varArg)
	{
		return FireOnUIEvent(pthing, bstrEventName, varArg);
	}

	STDMETHOD(OnUserReconnect)(BSTR bstrURL, BSTR bstrName, BSTR bstrPassword);	

public:
	STDMETHOD(Initialize)();	
	STDMETHOD(Terminate)();	

protected:
	HRESULT HandleMessage(IVWCommMessage* pmess, BOOL& bConnectionClosed);
	HRESULT HandleDataMessage(IVWCommMessage* pmess, IVWCommConnection *pconn, BOOL& bConnectionClosed);

	// Called by windowproc.
	HRESULT OnMessageAvailableHelper(IVWComm *);
	
	// Helper to disconnect a given world (and optional connection).
	void DisconnectWorld(IWorld *pworld, IVWCommConnection *pCommConnection);

	// Helper to disconnect a given connection.
	void DisconnectConnection(IVWCommConnection *pCommConnection, BOOL bIsSocketAlive = TRUE);
	
	// Helper to disconnect a connection given a socket disconnect.
	void DisconnectHelper(IVWCommConnection *pcon, BOOL bIsSocketAlive = TRUE);
	
	// Helper for connection. No restrictions re: primary connection.
	HRESULT ConnectHelper(BSTR initialURL, IWorld** ppWorld, IVWComm** ppVWComm,
		IVWCommConnection** ppVWConnection, DWORD* pdwCookie);

	// Internal helper called by external ConnectToServer and by internal ConnectHelper
	HRESULT ConnectToServerInt(BSTR bstrURL, VARIANT_BOOL bAuthenticate,IVWComm** ppVWComm, IVWCommConnection** ppVWConnection );

	// Helper for creating world and db. No restrictions re: primary connection.
	HRESULT CreateLocalWorld(BSTR bstr, VARIANT_BOOL bOverwrite,
		IWorld** ppWorld, DWORD* pdwCookie);

	// Helper to deal reconnect msg from the server.
	HRESULT OnReconnect(IUnMarshallBuffer *puBuffer,IVWCommConnection *pVWConnection, BOOL& bConnectionClosed);
	
	// Helper to deal with admin messages from the server.
	HRESULT ProcessAdminMessage(IUnMarshallBuffer *puBuffer, BYTE chType, IVWCommConnection *pVWConnection, BOOL& bConnectionClosed);

	// Helper to send administrative message to server that client is disconnecting.
	HRESULT SendDisconnectMessageToServer(IVWCommConnection *);

	// Helper to send administrative message to server that client is reconnecting.	
	HRESULT SendReconnectMessageToServer(BSTR bstrURL, BSTR bstrName, BSTR bstrPassword, IVWCommConnection *pCommConnection);

	// Helper to open or create a world on a connection already established
	HRESULT OpenOrCreateWorld(BSTR bstrURL, VARIANT_BOOL bCreate, IWorld **ppworld);

	// VARIANT_TRUE = connect, VARIANT_FALSE = disconnect
	HRESULT FireOnConnect(VARIANT_BOOL fConnect, IWorld *pWorld);
	HRESULT FireOnUserConnect(IThing* pthing, VARIANT_BOOL fConnect);

	HRESULT FireOnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg);

	HRESULT FireOnUserReconnect(IVWClient* newClient, IWorld* pWorld, IThing* pthing);

	HRESULT FireOnReport(BSTR bstr, long lVal);
	HRESULT FireOnTrace(BSTR bstr);

	HRESULT CreateTools();
	void	UpdateWorldReferences(IWorld * pWorld);

private:
	HWND m_hWnd;

	IPropertyMap* m_pTools;

	// map from url to world. but used as just a collection of ptrs.
	CWorldList m_wlist;

	// map of connections to associated world (maps IVWCommConnection->IWorld)
	CWorldConnList m_clist;

	// The client's primary connection/world.
	IWorld *			m_pPrimaryWorld;
	IVWCommConnection * m_pPrimaryCon;
	DWORD				m_dwPrimaryCookie;
	IVWComm *			m_pPrimaryVWComm;
	CComBSTR			m_bstrPrimaryServer;
	BOOL				m_bDisconnectingPrimaryConn;

	// Authentication pointer
	IVWAuthenticate *m_pAuthenticate;

	// wnd for process msg.
	CDummyWnd * m_pDummyWnd;

	VARIANT_BOOL m_bTerminated;

	long m_iMessagesInQueue;
	
	CRITICAL_SECTION m_CritSec;
};
