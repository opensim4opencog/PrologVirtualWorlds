// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWServe.h : Declaration of the CVWServer

#include <resource.h>

#include "queue.h"
#include "wldlist.h"
#include "connserv.h"
#include <dirserv.h>

#include <propbase.h>

EXTERN_C const CLSID CLSID_VWServer;

#define ReportServerError(hr) SUCCEEDED(hr) ? hr : ReportError("VWSYSTEM.Server.1", IID_IVWServer, hr, 0x300, 0x400)

/////////////////////////////////////////////////////////////////////////////
// VWServer
class ATL_NO_VTABLE CVWServer :
	public IDispatchImpl<IVWServer, &IID_IVWServer, &LIBID_VWSYSTEMLib>,
	public ISupportErrorInfo,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVWServer, &CLSID_VWServer>,
	public IConnectionPointImpl<CVWServer, &IID_DVWServerEvents, CComDynamicUnkArray>,
	public IConnectionPointContainerImpl<CVWServer>,
	public IVWCommSite,
	public IVWObjectSite
{
DECLARE_AGGREGATABLE(CVWServer)

public:
	CVWServer();
	~CVWServer();

	HRESULT FinalConstruct();

	void FinalRelease(void) 
	{
		if(m_pthreadComm)
			ShutDown();
		CComObjectRootEx<CComMultiThreadModel>::FinalRelease();
	}

BEGIN_COM_MAP(CVWServer)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWServer)
	COM_INTERFACE_ENTRY(IVWCommSite)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IVWObjectSite)
END_COM_MAP()

// Connection Point
	BEGIN_CONNECTION_POINT_MAP(CVWServer)
		CONNECTION_POINT_ENTRY(IID_DVWServerEvents)		
	END_CONNECTION_POINT_MAP()

//DECLARE_NOT_AGGREGATABLE(CVWServer) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it
DECLARE_AGGREGATABLE(CVWServer)

DECLARE_REGISTRY(CLSID_CVWServer, _T("VWSYSTEM.Server.1"), _T("VWSYSTEM.Server"), IDS_VWSERVER_DESC, THREADFLAGS_BOTH)

public:
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IVWServer
	STDMETHOD(Accept) (int iPort, int iOptionalCallbackWindowHandle);
	STDMETHOD(TestQueue)();
	STDMETHOD(get_UseAuthentication)(BOOL *pVal);
	STDMETHOD(put_UseAuthentication)(BOOL newVal);
	STDMETHOD(get_AuthenticationGroup)(BSTR *bstrGroup);
	STDMETHOD(put_AuthenticationGroup)(BSTR bstrGroup);
    STDMETHOD(get_RunningOnWinNT)(BOOL *pbRunningOnWinNT);
	STDMETHOD(get_ConnectionLimit)(DWORD *dwLimit);
	STDMETHOD(put_ConnectionLimit)(DWORD dwLimit);
	STDMETHOD(get_ConnectionCount)(long *pCount);

	//Registry routines
	STDMETHOD(EnumerateWorlds)(IPropertyList **ppGUIDList);

	//Disconnect routines.
	STDMETHOD(Disconnect) ();
	STDMETHOD(DisconnectWarning) ();
	STDMETHOD(DisconnectCancel) ();
	STDMETHOD(ShutDown) ();

	//Tool routines
	STDMETHOD(CreateToolFromProgID)(BSTR name, BSTR bstr, IUnknown** ppunk);
	STDMETHOD(RemoveTool)(BSTR name);
	STDMETHOD(get_Tool)(BSTR name, IUnknown** ppunk);
	STDMETHOD(get_Tools)(IPropertyMap** pppropertymap);

	// IVWCommSite
	STDMETHOD(OnMessageAvailable)(IVWComm *pcomm);
	STDMETHOD(OnMessageAvailable_T2)(IVWComm *pcomm);
	
	//Features for UI.
	STDMETHOD(SaveWorld)(BSTR bstrWorldName, BSTR *pbstrHR);
	STDMETHOD(SaveWorldAs)(BSTR bstrWorldName, BSTR bstrNewWorldName, BSTR *bstrHR);
	STDMETHOD(OpenWorld)(BSTR bstrWorldName, BSTR *pbstrHR);
	STDMETHOD(SaveWorldScript)(BSTR bstrWorldName, BSTR bstrScriptName, BSTR *pbstrHR);
	STDMETHOD(DeleteWorldFromSystem)(BSTR bstrWorldName, BSTR *pbstrHR);
	STDMETHOD(TakeWorldOffline)(BSTR bstrWorldName, BSTR *pbstrHR);
	STDMETHOD(TakeWorldOnline)(BSTR bstrWorldName, BSTR *pbstrHR);
	STDMETHOD(ReconnectToDS)();
	STDMETHOD(RefreshDS)();
	STDMETHOD(PerformanceTest)(BSTR bstrWorldName, long iType, BSTR *pbstrHRESULT);
	STDMETHOD(ClearPerformanceCounters)();

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

		FireOnTrace(bstr);

		return S_OK;
	};

	STDMETHOD(OnReport)(BSTR bstr, long lType)
	{
#if 1 // def _DEBUG
		CString str(bstr);
		if (str.Right(1) != _T("\n"))
			str += "\n";
		OutputDebugString((LPCTSTR)str);
#endif

		FireOnReport(bstr, lType);

		return S_OK;
	};

	STDMETHOD(OnInvokeMethod)(IThing* pthing, BSTR bstr, DISPPARAMS* pdispparams);

	STDMETHOD(OnPropertyChanged)(IThing* pthing, BSTR bstr, long lHint, VARIANT varHintData)
	{
		return S_OK;
	};

	STDMETHOD(OnMethodChanged)(IThing* pthing, BSTR bstr, long lHint, VARIANT varHintData)
	{
		return S_OK;
	};

	STDMETHOD(OnModuleChanged)(BSTR bstr, long lHint, VARIANT varHintData)
	{
		return S_OK;
	};

	STDMETHOD(GetStatusInfo)(DWORD *queueSize, DWORD *totalReq, BSTR *bstrWorldInfo,
		DWORD *dwTotalConnections, VARIANT_BOOL *pbIsDSConnected);

	STDMETHOD(GetSimpleStatus)(BSTR *bstrMsg);

	STDMETHOD(OnUserConnect)(IThing* pthing)
	{
		return S_OK;
	};

	STDMETHOD(OnUserReconnect)(BSTR bstrURL, BSTR bstrName, BSTR bstrPassword);

	STDMETHOD(OnUserDisconnect)(IThing* pthing)
	{
		return S_OK;
	};

	STDMETHOD(OnUIEvent)(IThing *pthing, BSTR bstrEventName, VARIANT varArg)
	{
		return S_OK;
	}

protected:
	//Features for UI.
	HRESULT SaveWorldHelper(CString strWorldName);
	
	HRESULT SaveWorldAsHelper(CString strWorldName, CString bstrNewWorldName);

	HRESULT OpenWorldHelper(CString strWorldName, CString &returnError);

	HRESULT SaveWorldScriptHelper(CString strWorldName, CString strScriptName);

	HRESULT DeleteWorldHelper(CString strWorldName);

	HRESULT TakeWorldOfflineHelper(CString strWorldName);
	
	HRESULT TakeWorldOnlineHelper(CString strWorldName);
	
	HRESULT PerformanceTestHelper1(CString strWorldName, long iType);	    

	//World registration stuff.
	HRESULT UpdateDS(BSTR bstrGUID, BSTR path, int NewUserCount);
	HRESULT RemoveWorldsFromDS();
	
	void setDSRefreshTimer(UINT i) {m_uiDSRefreshTimer=i;}
	UINT getDSRefreshTimer() {return m_uiDSRefreshTimer;}

	UINT m_uiDSRefreshTimer;

	// Data==================================================================
	
	//Current connection. Non-refcounted.
	IVWCommConnection *m_pCurrentConnection;

	//Callback window.
	HWND m_hCallbackWindowHandle;
	
	// Message loop state
	INT m_LoopState;

	// Server variables.
	UINT m_uiDisconnectTimerID;

	// Disconnect state variable.
	BOOL m_bIsShuttingDownArmed;
	
	// pointer to communications interface for server.
	IVWComm* m_pcomm;

	// pointer to authentication interface
	IVWAuthenticate* m_pAuthenticate;

	// communication thread
	CWinThread* m_pthreadComm;

	// Interthread event servicing queue.
	RequestQueue m_queue;

	// map from url to world. but used as just a collection of ptrs.
	CWorldList m_wlist;

	// map of connections to associated world (maps IVWCommConnection->IWorld)
	CWorldConnList m_clist;

	// tools
	IPropertyMap* m_pTools;

	IDServ * m_pDServ;

	// Performance counters
	typedef struct sPERFSTAT {
		DWORD Last;
		DWORD Total;
		DWORD SD;
		DWORD Cnt;
	} PERFSTAT;
    
	PERFSTAT m_Enter, m_Logon, m_Create, m_Disconnect, m_Reconnect, m_World;    
	double  m_dFreq;

	// force user authentication on login
	BOOL m_bForceAuthentication;

	// Optional local group name to authenticate against
	CComBSTR m_bstrAuthenticationGroup;

	// Optional Max connection limit
	DWORD m_dwMaxConnections;

	// Is Server running on NT?
	BOOL m_bWinNT;

	// Utility functions===================================================	
	
	// Tool functions
	HRESULT CreateTools_T2( );
	HRESULT CreateTool_T2(BSTR name, CLSID clsid, IUnknown** ppunk);

	// comm handler thread
	static UINT CommThread( void *pvVWServer );

	// sends a message to the client and tells it to reconnect or that object 
	// migration failed...
	HRESULT SendReconnectMessageToClient(IVWCommConnection *pConnection, HRESULT hr, BSTR bstrURL, BSTR bstrName, BSTR bstrPassword);

	void HandleTimerMsg_T2(UINT uiTimerID);

	void ProcessReconnectRequest(IUnMarshallBuffer *pcb,IVWCommConnection *pConnection);
	
	// Disconnect handlers in thread 2.
	HRESULT Disconnect_T2();
	HRESULT DisconnectWarning_T2();
	HRESULT DisconnectCancel_T2();
	HRESULT ShutDown_T2();

	// generic message handlers
	void ProcessMessage(IVWCommMessage *pmess);
	void ProcessDataMessage(IVWCommMessage *pmess, IVWCommConnection *pconn);

	// no side effects.
	HRESULT OpenWorld(BSTR bstrURL, BSTR bstrFileName, VARIANT_BOOL bCreate, VARIANT_BOOL bOverwrite, IWorld **ppWorld, DWORD *pdwCookie, VARIANT_BOOL bCheckOffline = VARIANT_TRUE);

	// no side effects.
	HRESULT GetURLFromMess(IUnMarshallBuffer *pcb, BSTR *pbstrFileName, BSTR *pbstrURL);

	// administrative message handler
	HRESULT ProcessAdminMessage(IWorld *pworld, IVWCommConnection *pconn, IVWCommMessage *pmess, IUnMarshallBuffer *pcb);

	// world message handler
	HRESULT ProcessWorldMessage(IWorld *pworld, IVWCommConnection *pconn, IVWCommMessage *pmess, IUnMarshallBuffer *pcb);

	// handler for Client's enter request
	HRESULT ProcessEnterWorld( IWorld *pworld, IVWCommConnection *pconn, IVWCommMessage *pmess, IUnMarshallBuffer *pcb );

	// handler for Client's ConnecToServer request
	HRESULT ProcessLogon(IWorld *pworld, IVWCommConnection *pconn, IVWCommMessage *pmess, IUnMarshallBuffer *pcb );

	// handler for Client's CreateWorld request
	HRESULT ProcessCreateWorld(IWorld *pworld, IVWCommConnection *pconn, IVWCommMessage *pmess, IUnMarshallBuffer *pcb );

	// handler for Client's Diagnostic request
	HRESULT ProcessDiagnostic(IWorld *pworld, IVWCommConnection *pconn, IVWCommMessage *pmess, IUnMarshallBuffer *pcb );

	// handler for Client's disconnect request
	HRESULT ProcessDisconnect(IWorld *pworld, IVWCommConnection *pconn, IVWCommMessage *pmess, IUnMarshallBuffer *pcb );

	// handler for Client's logoff request.
	void ProcessLogoffRequest(IVWCommConnection *pCommConnection);

	// sends admin message to client (used for logoff warnings and signals right now).
	HRESULT SendAdminMessageToClient(IVWCommConnection * pVWConnection,BYTE option);
	
	// sends results back to client
	HRESULT ReplyWithHResult(IVWCommMessage *pmess, HRESULT hr);
	HRESULT ReplyWithHResultAndData(IVWCommMessage *pmess, HRESULT hr, BYTE *pbData, int cbData);

	// event firers
	HRESULT FireOnWorldLoaded(CString& strWorld);
	HRESULT FireOnWorldUnloaded(CString& strWorld);

	HRESULT FireOnConnect(IVWCommConnection *pconn);
	HRESULT FireOnDisconnect(IVWCommConnection *pconn);

	HRESULT FireOnEnterWorld(IVWCommConnection *pconn, CString& strWorld);
	HRESULT FireOnLeaveWorld(IVWCommConnection *pconn, CString& strWorld);

	HRESULT FireOnCommError(IVWCommConnection *pconn);
	HRESULT FireOnCommMessage(IVWCommConnection *pconn, CString& strMessage);

	HRESULT	FireOnReport(BSTR bstr, long lType);
	HRESULT FireOnTrace(BSTR bstr);

	HRESULT	FireOnAvailableWorld(BSTR bstrName, int iUsers);

	CString ReportErrorSimple(HRESULT hr)
	{
		LPSTR lpszclsid = "VWSYSTEM.Server.1";
		HLOCAL lpMsgBuf = NULL;

		if (HRESULT_FACILITY(hr) == FACILITY_ITF && 
			HRESULT_CODE(hr) >= 0x0200)
		{
//			if (HRESULT_CODE(hr) >= 0x0300 && HRESULT_CODE(hr) < 0x0400)
//			{
				AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

				// pick off last 4 hex digits of HRESULT
				// match these to resource string table
				int resid = HRESULT_CODE(hr);
				CString strError;

				strError.LoadString(resid);

				CString temp;
				temp.Format("CVWServer::ReportError: hr %x, %s\n", hr, (LPCTSTR)strError);
				return temp;
//			}
		}
		// If there's a message associated with this error, report that
		else if (::FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, (DWORD)hr,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf, 0, NULL)
			> 0)
		{
			CString temp;
			temp.Format("CVWServer::ReportError: hr %x, %s", hr, (LPCTSTR)lpMsgBuf);
	
			// Free the buffer, which was allocated by FormatMessage
			if (lpMsgBuf != NULL)
				::LocalFree(lpMsgBuf);

			return temp;
		}
		return "";
	};
};

	