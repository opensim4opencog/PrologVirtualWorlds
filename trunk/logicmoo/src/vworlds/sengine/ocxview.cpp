// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// OCXView.cpp : implementation file
//

#include "stdafx.h"
#include "sengine.h"
#include "OCXView.h"
#include "OleID.h"
#include "SEngDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COCXView

IMPLEMENT_DYNCREATE(COCXView, CView)

BEGIN_EVENTSINK_MAP(COCXView, CView)
    //{{AFX_EVENTSINK_MAP(COCXView)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

COCXView::COCXView()
{
	EnableAutomation();
	m_ClientName = "VWView.VWViewCtrl.1";
}

COCXView::~COCXView()
{




}

void COCXView::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.
//	m_ClientCtrl.DestroyWindow();

	CView::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(COCXView, CView)
	//{{AFX_MSG_MAP(COCXView)
	ON_WM_SIZE()
	ON_COMMAND(ID_ACTIVEXCONTROLS_SETCONTROL, OnActivexcontrolsSetcontrol)
	ON_WM_DESTROY()
	ON_WM_PALETTECHANGED()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(COCXView, CView)
	//{{AFX_DISPATCH_MAP(COCXView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IOCXView to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {812E6309-0C8C-11D0-8A96-00A0C9082583}
static const IID IID_IOCXView =
{ 0x812e6309, 0xc8c, 0x11d0, { 0x8a, 0x96, 0x0, 0xa0, 0xc9, 0x8, 0x25, 0x83 } };

BEGIN_INTERFACE_MAP(COCXView, CView)
	INTERFACE_PART(COCXView, IID_IOCXView, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COCXView drawing

void COCXView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// COCXView diagnostics

#ifdef _DEBUG
void COCXView::AssertValid() const
{
	CView::AssertValid();
}

void COCXView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COCXView message handlers

void COCXView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	if ( m_ClientCtrl.m_hWnd )
		m_ClientCtrl.SetWindowPos(&wndTop,0,0,cx,cy,SWP_NOZORDER );
	
	// TODO: Add your message handler code here
	
}

BOOL COCXView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	BOOL bRet = CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	if (!bRet ) return bRet;

	return bRet;


}

BOOL COCXView::SetClient( CString strClientName)
{

	if ( m_ClientCtrl.m_hWnd )
	{
		m_ClientCtrl.DestroyWindow();
		m_ClientCtrl.m_hWnd = NULL;
	}

	m_ClientName = strClientName;
	RECT crect;
	GetClientRect( &crect );
	LONG dwStyle = WS_VISIBLE;
	if (!m_ClientCtrl.CreateControl( m_ClientName, NULL, dwStyle, crect, this, 255))
	{
		AfxMessageBox("Unable to Create Control");
		return FALSE;
	}
	CSEngineDoc * pDoc = ( CSEngineDoc *) GetDocument();	
	UpdateVWClientObject(pDoc->GetClientObject());
	return TRUE;
}

void COCXView::UpdateVWClientObject(LPDISPATCH pClient)
{


	IUnknown * punk = m_ClientCtrl.GetControlUnknown( );
	ASSERT ( punk );
	IDispatch * pdisp = NULL;
	if ( FAILED( punk->QueryInterface( IID_IDispatch, (void **)&pdisp )))
		return ;

	DISPID dispid;
	CString Name("VWClient");

	BSTR bstrName = Name.AllocSysString();

	HRESULT hresult = pdisp->GetIDsOfNames( IID_NULL,&bstrName,1, LOCALE_SYSTEM_DEFAULT, &dispid );
	::SysFreeString( bstrName );
	if ( hresult != S_OK ) 
		goto errout;
//  for some reason this causes ref counting propblems	
	m_ClientCtrl.SetProperty(dispid, VT_DISPATCH ,  pClient);
/*
	VARIANT var;
	var.vt = VT_DISPATCH;
	var.pdispVal = pClient;

	DISPPARAMS dp = { &var,NULL, 1,0 };
	EXCEPINFO ei;
	UINT	uerr;

	pdisp->Invoke( dispid, IID_NULL,
					LOCALE_SYSTEM_DEFAULT,
					DISPATCH_PROPERTYPUT,
					&dp,
					NULL,
					&ei,&uerr );
*/
errout:
	if ( pdisp)	pdisp->Release();

	
}

void COCXView::OnActivexcontrolsSetcontrol() 
{
	// TODO: Add your command handler code here
	COleIDDlg dlg;
	if ( dlg.DoModal() == IDOK )
	{
		SetClient(dlg.m_OLEID);
	}
}

void COCXView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	switch ( lHint ){
	case CHANGE_CLIENT:
	{
		CSEngineDoc * pDoc = ( CSEngineDoc *) GetDocument();	
		UpdateVWClientObject(pDoc->GetClientObject());

	}
	}
}


void COCXView::OnDestroy() 
{
	// forcibly close the script site so it will release any AddRefs
	CSEngineDoc * pDoc = ( CSEngineDoc *) GetDocument();
	pDoc->m_ScriptSite.Close();

	UpdateVWClientObject(NULL);

	CView::OnDestroy();
}

BOOL COCXView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	//return CView::OnEraseBkgnd(pDC);
	return TRUE;
}
