// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// DBGView.cpp : implementation file
//

#include "stdafx.h"
#include "sengine.h"
#include "DBGView.h"
#include "SEngDoc.h"

/*
#define IID_DEFINED
#include "vwobject_i.c"
#include "vwclient_i.c"
*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Utility functions
template<class CType>
void RELEASE( CType * &pType )
{
	if ( pType )
	{
		pType->Release();
		pType = NULL;
	}
}

template<class CType>
void ADDREF( CType * pType )
{
	if ( pType )
	{
		pType->AddRef();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDBGView

IMPLEMENT_DYNCREATE(CDBGView, CFormView)

CDBGView::CDBGView()
	: CFormView(CDBGView::IDD)
{
	m_dwClientCookie = 0;
	EnableAutomation();
	//{{AFX_DATA_INIT(CDBGView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDBGView::~CDBGView()
{
	CSEngineDoc *pDoc = (CSEngineDoc *)GetDocument();	
	UnAdvise( (IUnknown *) pDoc->GetClientObject(),IID_IVWClientSite,m_dwClientCookie );

}

void CDBGView::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CFormView::OnFinalRelease();
}

void CDBGView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDBGView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_BUTTON1, m_ReloadButton);
	DDX_Control(pDX, IDC_ENTER, m_EnterButton);
	DDX_Control(pDX, IDC_COMBO1, m_SourceCombo);
	DDX_Control(pDX, IDC_HISTORY, m_HistoryEdit);
	DDX_Text(pDX, IDC_HISTORY, m_History);
	DDX_CBString(pDX, IDC_COMBO1, m_Source);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDBGView, CFormView)
	//{{AFX_MSG_MAP(CDBGView)
	ON_BN_CLICKED(IDC_ENTER, OnEnter)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_SIZE()	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CDBGView, CFormView)
	//{{AFX_DISPATCH_MAP(CDBGView)
	DISP_FUNCTION(CDBGView, "OnUserConnect", OnUserConnect, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION(CDBGView, "OnUserDisconnect", OnUserDisconnect, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION(CDBGView, "OnTell", OnTell, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CDBGView, "OnTrace", OnTrace, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CDBGView, "OnReport", OnReport, VT_EMPTY, VTS_BSTR VTS_I4)
	DISP_FUNCTION(CDBGView, "OnConnect", OnConnect, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION(CDBGView, "OnDisconnect", OnDisconnect, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION(CDBGView, "OnPreDestroy", OnPreDestroy, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CDBGView, "OnEnterInventory", OnEnterInventory, VT_EMPTY, VTS_DISPATCH VTS_DISPATCH)
	DISP_FUNCTION(CDBGView, "OnLeaveInventory", OnLeaveInventory, VT_EMPTY, VTS_DISPATCH VTS_DISPATCH)
	DISP_FUNCTION(CDBGView, "OnEnterRoom", OnEnterRoom, VT_EMPTY, VTS_DISPATCH VTS_DISPATCH)
	DISP_FUNCTION(CDBGView, "OnLeaveRoom", OnLeaveRoom, VT_EMPTY, VTS_DISPATCH VTS_DISPATCH)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IDBGView to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {4CA95520-0B1C-11D0-8A96-00A0C9082583}
static const IID IID_IDBGView =
{ 0x4ca95520, 0xb1c, 0x11d0, { 0x8a, 0x96, 0x0, 0xa0, 0xc9, 0x8, 0x25, 0x83 } };


// event sink stuff
BEGIN_INTERFACE_MAP(CDBGView, CFormView)
	INTERFACE_PART(CDBGView, IID_IDBGView, Dispatch)
//    INTERFACE_PART(CDBGView, IID_IVWObjectSite, ObjectSite)
	INTERFACE_PART(CDBGView, IID_IVWClientSite, ClientSite )
END_INTERFACE_MAP()

// Client events implementation
STDMETHODIMP_(ULONG) CDBGView::XClientSite::AddRef()
{
	METHOD_PROLOGUE(CDBGView, ClientSite)
	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CDBGView::XClientSite::Release()
{
	METHOD_PROLOGUE(CDBGView, ClientSite)
	return pThis->ExternalRelease();
}

STDMETHODIMP CDBGView::XClientSite::QueryInterface(
    REFIID iid, void FAR* FAR* ppvObj)
{
	METHOD_PROLOGUE(CDBGView, ClientSite)
    return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CDBGView::XClientSite::OnConnect(IWorld* pWorld)
{
	METHOD_PROLOGUE(CDBGView, ClientSite)
	pThis->OnConnect(pWorld);
	return S_OK;
}

STDMETHODIMP CDBGView::XClientSite::OnUserReconnect(IVWClient* newClient,
			IWorld* pWorld,
			IThing* pthing)
{
	METHOD_PROLOGUE(CDBGView, ClientSite)
	pThis->OnUserReconnect(newClient,pWorld,pthing);
	return S_OK;
}

STDMETHODIMP CDBGView::XClientSite::OnDisconnect( IWorld* pWorld)
{
	METHOD_PROLOGUE(CDBGView, ClientSite)
	pThis->OnDisconnect(pWorld);
	return S_OK;
}

STDMETHODIMP CDBGView::XClientSite::OnTrace(BSTR bstr)
{
	METHOD_PROLOGUE(CDBGView, ClientSite)
	pThis->OnTrace( CString(bstr) );
	return S_OK;
}

STDMETHODIMP CDBGView::XClientSite::OnReport(BSTR bstr, long lType)
{
	METHOD_PROLOGUE(CDBGView, ClientSite)
	pThis->OnReport( CString(bstr) , lType);
	return S_OK;
}

STDMETHODIMP CDBGView::XClientSite::OnUserConnect(IThing* pthing)
{
	METHOD_PROLOGUE(CDBGView, ClientSite)
	pThis->OnUserConnect(pthing);
	return S_OK;
}

STDMETHODIMP CDBGView::XClientSite::OnUserDisconnect(IThing* pthing)
{
	METHOD_PROLOGUE(CDBGView, ClientSite)
	pThis->OnUserDisconnect(pthing);
	return S_OK;
}

STDMETHODIMP CDBGView::XClientSite::OnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg)
{
	METHOD_PROLOGUE(CDBGView, ClientSite)
	return S_OK;
}

HRESULT CDBGView::Advise( IUnknown *pUnk,REFIID riid, ULONG *pdwCookie )
{
	HRESULT hr = E_FAIL;
	IConnectionPoint* pconnpt = NULL;
	IConnectionPointContainer* pconnptctr = NULL;
	
	if ( pUnk == NULL )
		goto errout;

//	TRACE("CDBGView::Advise\n");
	hr = pUnk->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pconnptctr);
	if (FAILED(hr))
		goto errout;

	hr = pconnptctr->FindConnectionPoint(riid, &pconnpt);
	

	if (SUCCEEDED(hr))
	{
	
		pconnpt->Advise(GetIDispatch( FALSE ) , pdwCookie);
		RELEASE( pconnpt );
	}



errout:
	RELEASE( pconnpt);
	RELEASE( pconnptctr );
return hr;
}

HRESULT CDBGView::UnAdvise( IUnknown *pUnk,REFIID riid, ULONG dwCookie )
{
HRESULT hr = E_FAIL;
IConnectionPoint* pconnpt = NULL;
IConnectionPointContainer* pconnptctr = NULL;
	
	if ( pUnk == NULL )
		goto errout;

//	TRACE("CDBGView::UnAdvise\n");
	hr = pUnk->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pconnptctr);
	if (FAILED(hr))
		goto errout;

	hr = pconnptctr->FindConnectionPoint(riid, &pconnpt);
	

	if (SUCCEEDED(hr))
	{
	
		pconnpt->Unadvise(  dwCookie );

		RELEASE( pconnpt );
	}



errout:
	RELEASE( pconnpt);
	RELEASE( pconnptctr );
return hr;
}



// this is a IDispatch becuase MFC doesn't know about the world
void CDBGView::OnUserReconnect(IVWClient* newClient,
			IWorld* pWorld,
			IThing* pthing) 
{
	// TODO: Add your dispatch handler code here
	
	ASSERT( pWorld );

//	if ( FAILED (Advise( pWorld ,IID_IVWObjectSite, &m_dwWorldCookie )))
//		OnError("Unable to establish sink to the world");


}

// this is a IDispatch becuase MFC doesn't know about the world
void CDBGView::OnConnect(LPDISPATCH pWorld) 
{
	// TODO: Add your dispatch handler code here
	
	ASSERT( pWorld );

//	if ( FAILED (Advise( pWorld ,IID_IVWObjectSite, &m_dwWorldCookie )))
//		OnError("Unable to sink to the world");


}

void CDBGView::OnDisconnect(LPDISPATCH pWorld) 
{
	// TODO: Add your dispatch handler code here
	ASSERT( pWorld );
//	if ( FAILED (UnAdvise( pWorld,IID_IVWObjectSite,m_dwWorldCookie )))
//		OnError("Unable to release sink on the world");

}

void CDBGView::OnPreDestroy() 
{
	// TODO: Add your dispatch handler code here
}

void CDBGView::OnTrace(LPCTSTR str) 
{
	// TODO: Add your dispatch handler code here

	AddHistoryText( CString("TRACE: ") + str );
}

void CDBGView::OnReport(LPCTSTR str, long lType) 
{
	// TODO: Add your dispatch handler code here

	// don't report perflog
	if (lType != 4)
		AddHistoryText( CString("REPORT: ") + str );
}

void CDBGView::OnTell(LPCTSTR str) 
{
	// TODO: Add your dispatch handler code here
	AddHistoryText( CString("TELL: ") + str );
}

void CDBGView::OnUserConnect(LPDISPATCH pthing) 
{
	BSTR bstrName = NULL;

	if (SUCCEEDED(((IThing*)pthing)->get_Name(&bstrName)))
	{
		CString str(bstrName);

		AddHistoryText( CString("CONNECTED: ") + str );

		::SysFreeString(bstrName);
	}
}

void CDBGView::OnUserDisconnect(LPDISPATCH pthing) 
{
	BSTR bstrName = NULL;
	
	if (pthing && SUCCEEDED(((IThing*)pthing)->get_Name(&bstrName)))
	{
		CString str(bstrName);

		AddHistoryText( CString("DISCONNECTED: ") + str );

		::SysFreeString(bstrName);
	}
}

void CDBGView::OnEnterInventory(LPDISPATCH pWhere, LPDISPATCH pWhat) 
{
	CString strWhat, strWhere;
	BSTR bstrName = NULL;

	if (SUCCEEDED(((IThing*)pWhat)->get_Name(&bstrName)))
	{
		strWhat = bstrName;

		::SysFreeString(bstrName);
		bstrName = NULL;
	}

	if (SUCCEEDED(((IThing*)pWhere)->get_Name(&bstrName)))
	{
		strWhere = bstrName;

		::SysFreeString(bstrName);
		bstrName = NULL;
	}

	AddHistoryText( CString("NOTIFY: ") + strWhat + CString(" entered ") + strWhere + CString("'s inventory") );
}

void CDBGView::OnLeaveInventory(LPDISPATCH pWhere, LPDISPATCH pWhat) 
{
	CString strWhat, strWhere;
	BSTR bstrName = NULL;

	if (SUCCEEDED(((IThing*)pWhat)->get_Name(&bstrName)))
	{
		strWhat = bstrName;

		::SysFreeString(bstrName);
		bstrName = NULL;
	}

	if (SUCCEEDED(((IThing*)pWhere)->get_Name(&bstrName)))
	{
		strWhere = bstrName;

		::SysFreeString(bstrName);
		bstrName = NULL;
	}

	AddHistoryText( CString("NOTIFY: ") + strWhat + CString(" left ") + strWhere + CString("'s inventory") );
}

void CDBGView::OnEnterRoom(LPDISPATCH pWhere, LPDISPATCH pWhat) 
{
	BSTR bstrName = NULL;
	CString strWhere;
	CString strWhat;

	if (SUCCEEDED(((IThing*)pWhere)->get_Name(&bstrName)))
	{
		strWhere = bstrName;
		::SysFreeString(bstrName);
	}

	if (SUCCEEDED(((IThing*)pWhat)->get_Name(&bstrName)))
	{
		strWhat = bstrName;
		::SysFreeString(bstrName);
	}

	AddHistoryText( CString("NOTIFY: ") + strWhat + CString(" entered the '") + strWhere + CString("' room"));
}

void CDBGView::OnLeaveRoom(LPDISPATCH pWhere, LPDISPATCH pWhat) 
{
	BSTR bstrName = NULL;
	CString strWhere;
	CString strWhat;

	if (SUCCEEDED(((IThing*)pWhere)->get_Name(&bstrName)))
	{
		strWhere = bstrName;
		::SysFreeString(bstrName);
	}

	if (SUCCEEDED(((IThing*)pWhat)->get_Name(&bstrName)))
	{
		strWhat = bstrName;
		::SysFreeString(bstrName);
	}

	AddHistoryText( CString("NOTIFY: ") + strWhat + CString(" left the '") + strWhere + CString("' room"));
}

/////////////////////////////////////////////////////////////////////////////
// CDBGView diagnostics

#ifdef _DEBUG
void CDBGView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDBGView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDBGView message handlers
void CDBGView::OnInitialUpdate() 
{
	//CFormView::OnInitialUpdate();
	if (!UpdateData(FALSE))
		TRACE0("UpdateData failed during formview initial update.\n");

	
	// TODO: Add your specialized code here and/or call the base class
	CFrameWnd * pFrame= GetParentFrame();
	//pFrame->ModifyStyle( WS_MAXIMIZEBOX|WS_THICKFRAME , 0);
	ResizeParentToFit( FALSE );
	RECT rect;
	GetClientRect( &rect );
	OnRecalcLayout(rect.right-rect.left,rect.bottom-rect.top);
	m_SourceCombo.SetFocus();
	
	CSEngineDoc *pDoc = (CSEngineDoc *)GetDocument();	
	Advise( (IUnknown *) pDoc->GetClientObject() ,IID_IVWClientSite, &m_dwClientCookie );



}


void CDBGView::OnEnter() 
{
	// TODO: Add your control notification handler code here
	CSEngineDoc *pDoc = (CSEngineDoc *)GetDocument();	
	UpdateData();
	CString source = m_Source;

	int index;
	if ( (index =  m_SourceCombo.FindStringExact(0,source)) != CB_ERR )
		m_SourceCombo.DeleteString( index );
	m_SourceCombo.InsertString( 0,source );
	while ( (index = m_SourceCombo.GetCount()) > 35 )
	{
		m_SourceCombo.DeleteString(index-1);
	}
	m_SourceCombo.SetEditSel(0,0);
	AddHistoryText( source );
	if ( source.Find("source:") == 0 )
	{
		CString fname = source.Right(source.GetLength()-7);
		pDoc->AddScriptSource( fname);
	}
	else
		pDoc->AddScriptlet( source );
}

void CDBGView::OnButton1() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->PostMessage( WM_COMMAND, ID_SCRIPT_LOAD );
}

void CDBGView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( bActivate ) m_SourceCombo.SetFocus();
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CDBGView::OnSize(UINT nType, int cx, int cy) 
{
	SetScaleToFitSize( CSize(cx,cy) );
	CFormView::OnSize(nType, cx, cy);
	OnRecalcLayout(cx,cy);
}

void CDBGView::OnRecalcLayout(int cx, int cy) 
{
	RECT rect;
	if ( !m_ReloadButton.m_hWnd ) return;

	m_ReloadButton.GetWindowRect( &rect );
	int wcy = rect.bottom - rect.top;
	int wcx = rect.right - rect.left;
	ScreenToClient( &rect );
	int x = cx - wcx;
	int y = cy - wcy;

	m_HistoryEdit.SetWindowPos( &wndTop,0,0,cx, cy - wcy-1,SWP_NOZORDER );
	m_ReloadButton.SetWindowPos( &wndTop,x,y,0,0,SWP_NOZORDER | SWP_NOSIZE );
	x -=  wcx + 1;
	m_EnterButton.SetWindowPos( &wndTop,x,y,0,0,SWP_NOZORDER | SWP_NOSIZE );
	
	m_SourceCombo.SetWindowPos( &wndTop,0,y,x,y+wcy,SWP_NOZORDER  );
	
}


void CDBGView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class

	switch ( lHint ){
	case CHANGE_CLIENT:
	{
		CSEngineDoc * pDoc = ( CSEngineDoc *) GetDocument();
		Advise( (IUnknown *) pDoc->GetClientObject() ,IID_IVWClientSite, &m_dwClientCookie );
	}
	break;
	default:
	{
		CSEngineDoc *pDoc = (CSEngineDoc *)GetDocument();	
		CString strError = pDoc->m_ScriptSite.strerror( lHint );
		AddHistoryText( strError );
	}
	break;
	}
}

void CDBGView::AddHistoryText( CString str )
{
	CString text("");
	char newline[] = {0xD,0xA,0};
	for(int i=0;i< str.GetLength();i++)
	{
		switch( str[i] ){
		case '\n':
			// if not a linefeed as last character, add one to buffer
			if (i != str.GetLength() - 1)
				text += newline;
		break;
		default:
			text += str[i];
		break;
		}
	}
	m_History += text + newline;
	// truncate text if it is over 64K
	while ( m_History.GetLength() > ((int) m_HistoryEdit.GetLimitText()) )
	{
		int n = m_History.Find( newline );
		n += 2;
		m_History = m_History.Right( m_History.GetLength()- n);
	}

	// don't update the display if we're being invoked from the non-owning thread
	CWnd *pwndInMap;
	pwndInMap = CWnd::FromHandlePermanent( m_hWnd );
	if ( pwndInMap != NULL && pwndInMap == (CWnd *) this)
	{
		UpdateData( FALSE );
		DWORD len = m_HistoryEdit.GetLineCount() ;
		len = m_HistoryEdit.LineIndex( len-1 );
		m_HistoryEdit.SetSel(len  ,len,  FALSE );
	}
}

