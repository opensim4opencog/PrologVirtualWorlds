// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// socxctl.cpp : Implementation of the CScriptCtrl ActiveX Control class.

#include "stdafx.h"
#include "uictrl.h"
#include "socxctl.h"
#include <inetfile.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEFAULT_TIMEOUT -1 // (8 * 60 * 1000)

IMPLEMENT_DYNCREATE(CScriptCtrl, CVWViewCtrl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CScriptCtrl, CVWViewCtrl)
	//{{AFX_MSG_MAP(CScriptCtrl)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CScriptCtrl, CVWViewCtrl)
	//{{AFX_DISPATCH_MAP(CScriptCtrl)
	DISP_PROPERTY_EX(CScriptCtrl, "VWClient", GetVWClient, SetVWClient, VT_DISPATCH)
	DISP_PROPERTY_EX(CScriptCtrl, "ScriptURL", GetScriptURL, SetScriptURL, VT_BSTR)
	DISP_PROPERTY_EX(CScriptCtrl, "TimeOut", GetTimeOut, SetTimeOut, VT_I4)
	DISP_FUNCTION(CScriptCtrl, "Invoke", Invoke, VT_ERROR, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CScriptCtrl, CVWViewCtrl)
	//{{AFX_EVENT_MAP(CScriptCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	EVENT_CUSTOM("OnError", FireOnError, VTS_I4 VTS_BSTR)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CScriptCtrl, 2)
	PROPPAGEID( CLSID_CFontPropPage )
    PROPPAGEID( CLSID_CColorPropPage )
END_PROPPAGEIDS(CScriptCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CScriptCtrl, "VWCLIENTUI.ScriptCtrl.1",
	0xe5d5946, 0x20f, 0x11d1, 0x83, 0xab, 0, 0xc0, 0x4f, 0xb6, 0xfa, 0x46)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CScriptCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DScriptocx =
		{ 0xe5d5944, 0x20f, 0x11d1, { 0x83, 0xab, 0, 0xc0, 0x4f, 0xb6, 0xfa, 0x46 } };
const IID BASED_CODE IID_DScriptocxEvents =
		{ 0xe5d5945, 0x20f, 0x11d1, { 0x83, 0xab, 0, 0xc0, 0x4f, 0xb6, 0xfa, 0x46 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwScriptocxOleMisc =
	OLEMISC_INVISIBLEATRUNTIME |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CScriptCtrl, IDS_SCRIPTOCX, _dwScriptocxOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CScriptCtrl::CScriptCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CScriptCtrl

BOOL CScriptCtrl::CScriptCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegInsertable | afxRegApartmentThreading to afxRegInsertable.

	
	if (bRegister)
	{
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_SCRIPTOCX,
			IDB_SCRIPTOCX,
			afxRegInsertable | afxRegApartmentThreading,
			_dwScriptocxOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	}
	else
	{
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CScriptCtrl::CScriptCtrl - Constructor

CScriptCtrl::CScriptCtrl() : m_ScriptSite()
{
	InitializeIIDs(&IID_DScriptocx, &IID_DScriptocxEvents);

	m_bstrScriptPath = ":VBScript";
	m_TimeOut = DEFAULT_TIMEOUT;
	m_inScript = 0;
	m_bDelayedRefresh = FALSE;
	// TODO: Initialize your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CScriptCtrl::~CScriptCtrl - Destructor

CScriptCtrl::~CScriptCtrl()
{
	// TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CScriptCtrl::OnDraw - Drawing function

void CScriptCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}


/////////////////////////////////////////////////////////////////////////////
// CScriptCtrl::DoPropExchange - Persistence support

void CScriptCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	CVWViewCtrl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CScriptCtrl::OnResetState - Reset control to default state

void CScriptCtrl::OnResetState()
{
	CVWViewCtrl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CScriptCtrl message handlers


void CScriptCtrl::RefreshScript()
{
	if ( m_inScript )
	{
		SCRIPTTHREADSTATE stsState;
		IActiveScript  * ps = m_ScriptSite.GetIActiveScript();
		SCRIPTTHREADID stidThread = m_ScriptSite.GetScriptThreadID();

			if ( ps && 
				 (ps->GetScriptThreadState(stidThread, &stsState) == S_OK) &&
				 (stsState == SCRIPTTHREADSTATE_RUNNING))
			{
				// Set the error info
			//	m_ScriptSite.m_ERROR = E_RUNTIME;
			//	m_ScriptSite.m_bExceptionRaised = TRUE;

				// are we still running, if so time out
				CString ErrorInfo("World state is no longer valid");
				BSTR bstrError = ErrorInfo.AllocSysString();
				EXCEPINFO excepinfo;
				memset(&excepinfo, 0, sizeof(EXCEPINFO));
				excepinfo.wCode = 0;		// An error code describing the error. 
				excepinfo.wReserved = 0; 
				excepinfo.bstrSource = bstrError;				// Source of the exception. 
				excepinfo.bstrDescription =  bstrError;		// Textual description of the error. 
				excepinfo.bstrHelpFile = NULL;			// Help file path. 
				excepinfo.dwHelpContext = 0;	// Help context ID. 
				excepinfo.pvReserved = NULL; 
				/* Pointer to function that fills in Help and description info */ 
				excepinfo.scode = E_RUNTIME;					// An SCODE describing the error. 

				ps->InterruptScriptThread(stidThread, &excepinfo,SCRIPTINTERRUPT_RAISEEXCEPTION);
				::SysFreeString(bstrError);
				
			}
		m_bDelayedRefresh = TRUE;
		return;
	}
	m_bDelayedRefresh = FALSE;
	m_ScriptSite.Close();
	if ( m_pWorld )
	{
		LoadScript();
	}
}


HRESULT CScriptCtrl::LoadScript()
{
	HRESULT hr = NULL;
	ITypeInfo* pTypeInfo = NULL;
	IInternetFileManager *pInetfile = NULL;
	IDispatch* pDisp = NULL;
	CString strPath;
	static CComBSTR bstrInetfile("Inetfile");
	BSTR bstrFinalURL = NULL;
	BSTR bstrFile = NULL;

	ASSERT(m_pWorld);	
	hr = m_pWorld->GetTypeInfo(0, LOCALE_SYSTEM_DEFAULT, &pTypeInfo);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;


	// TODO: use World from resource
	hr = m_ScriptSite.AddNamedItem("World", m_pWorld, NULL );//pTypeInfo);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_ScriptSite.AddNamedItem("Client", m_pVWClient, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// Load the file
	// REVIEW: See if this is the VBScript "special" module
	strPath = m_bstrScriptPath;
	if (strPath != ":VBScript")
	{
		/* we aren't using urls anymore
		VARIANT_BOOL bFound;
		hr = m_pWorld->get_Tool(bstrInetfile, (IUnknown**)&pDisp);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		if (!pDisp)
			goto ERROR_ENCOUNTERED;

		hr = pDisp->QueryInterface(IID_IInternetFileManager, (void**) &pInetfile);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (FAILED(hr = pInetfile->FindCachedFile(m_bstrScriptPath, &bstrFinalURL, &bstrFile, &bFound)) || !bFound)
		{
			if (FAILED(hr = pInetfile->GetFile(m_bstrScriptPath, VARIANT_FALSE, &bstrFinalURL, &bstrFile))) 
			{
				CString strError = "Unable to load file: " + CString(m_bstrScriptPath);
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "V-Worlds Script Control::LoadScript: name (%s), error %s\n", CString("socxctl.cpp"), strError);
				CComBSTR bstrERROR = strError;
				m_pWorld->Report( bstrERROR ,-1);
				FireOnError(-1,strError);
				hr = m_ScriptSite.LoadScript(CString(":VBScript"), CString("World"));
				goto ERROR_ENCOUNTERED;
			}
		}
		*/
		hr = m_ScriptSite.LoadScript(CString(m_bstrScriptPath), CString("World"));
		if (FAILED(hr))
		{
			int err = m_ScriptSite.GetLastError();
			CString strError = m_ScriptSite.strerror(err);
			FireOnError(err,strError);
			m_pWorld->Trace(CComBSTR( strError ));
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptCtrl::LoadScript: %s\n", strError);
#endif
		}
		// KM: removed since it clutters up Import Wizard output
#if 0
		else
		{
			CComBSTR strSucces = CComBSTR("Loaded ");
			strSucces.Append(m_bstrScriptPath);
			m_pWorld->Trace(strSucces);
		}
#endif
	}
	else
	{
		// REVIEW: Tell the script site to load the special VBScript module.
		// This doesn't do a load - it just initializes the script engine.
		hr = m_ScriptSite.LoadScript(strPath, CString("World"));
		if (FAILED(hr))
		{
			int err = m_ScriptSite.GetLastError();
			CString strError = m_ScriptSite.strerror(err);
			FireOnError(err,strError);
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptCtrl::LoadScript: %s\n", strError);
#endif
		}
	}

ERROR_ENCOUNTERED:	
	SAFERELEASE(pTypeInfo);
	SAFEFREESTRING(bstrFile);
	SAFERELEASE(pInetfile);
	SAFERELEASE(pDisp);

	return hr;
}


LPDISPATCH CScriptCtrl::GetVWClient() 
{
	return CVWViewCtrl::GetVWClient();
}

void CScriptCtrl::SetVWClient(LPDISPATCH newValue) 
{
	RefreshScript();
	CVWViewCtrl::SetVWClient( newValue );
}


HRESULT CScriptCtrl::HandleOnConnect(IDispatch* pWorld)
{
	CVWViewCtrl::HandleOnConnect(pWorld);
	RefreshScript();
	return S_OK; 
}
HRESULT CScriptCtrl::HandleOnDisconnect(IDispatch* pWorld)
{
	CVWViewCtrl::HandleOnDisconnect(pWorld);
	RefreshScript();
	return S_OK; 
}

HRESULT CScriptCtrl::HandleOnUserConnect(IThing* pthing, BOOL bFromClient)
{
	CVWViewCtrl::HandleOnUserConnect(pthing, bFromClient);
	CThingPtr UserPtr;
	if ( SUCCEEDED(m_pWorld->get_User(&UserPtr.p)) &&
	   (UserPtr == pthing ))
	{
		RefreshScript();
	}
	return S_OK; 
}
HRESULT CScriptCtrl::HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient)
{
	CVWViewCtrl::HandleOnUserDisconnect(pthing, bFromClient);
	CThingPtr UserPtr;
	if ( SUCCEEDED(m_pWorld->get_User(&UserPtr.p)) &&
	   (UserPtr == pthing ))
	{
		RefreshScript();
	}
	return S_OK; 
}

HRESULT CScriptCtrl::HandleOnUserReconnect(IVWClient* newClient,
			IWorld* pWorld,
			IThing* pthing) 
{
	CVWViewCtrl::HandleOnUserReconnect(newClient,pWorld,pthing);
	CThingPtr UserPtr;
	if ( SUCCEEDED(m_pWorld->get_User(&UserPtr.p)) &&
	   (UserPtr == pthing ))
	{
		RefreshScript();
	}
	return S_OK; 
}

SCODE CScriptCtrl::Invoke(LPCTSTR bstrScriptText) 
{
	CComVariant	varResult;
	CComBSTR	bstrText	= bstrScriptText;
	CComBSTR	bstrOK		= "OK";
	HRESULT		hr;

	if (!m_pWorld ) return S_OK;

	m_pWorld->Report(bstrText, 0x01);
	
	m_inScript++;

	// sending in a return variant switches the script mode to expression evaluation, which screws up reporting
	hr = m_ScriptSite.InvokeScriptlet(CString(bstrScriptText), CString("World"), m_TimeOut); //, &varResult);
	if (FAILED(hr))
	{
		int err = m_ScriptSite.GetLastError();
		CString strError = m_ScriptSite.strerror(err);
		FireOnError(err,strError);
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CScriptCtrl::Invoke: %s\n", strError);
#endif

		CComBSTR bstrERROR = strError;
		m_pWorld->Report( bstrERROR ,-1);
	}	
	else
	{
		CComBSTR	bstrResult;
		// parse the result as a variant
		switch (varResult.vt)
		{
		case VT_ERROR:
		case VT_EMPTY:
			// print nothing
			goto FINALIZE;
		
		default:
			varResult.ChangeType(VT_BSTR);
			bstrResult = varResult.bstrVal;
		}

		m_pWorld->Report( bstrResult ,0x02);
	}

FINALIZE:
	m_inScript--;
	if ( m_bDelayedRefresh )
	{
		RefreshScript();
	}

	m_pWorld->Report(bstrOK, 0x03);
	return S_OK;
}

BSTR CScriptCtrl::GetScriptURL() 
{
	CString strResult = m_bstrScriptPath;
	return strResult.AllocSysString();
}

void CScriptCtrl::SetScriptURL(LPCTSTR lpszNewValue) 
{
	m_bstrScriptPath = lpszNewValue;
	RefreshScript();
	SetModifiedFlag();
}

long CScriptCtrl::GetTimeOut() 
{
	return m_TimeOut;
}

void CScriptCtrl::SetTimeOut(long nNewValue) 
{
	m_TimeOut = nNewValue;
	SetModifiedFlag();
}
