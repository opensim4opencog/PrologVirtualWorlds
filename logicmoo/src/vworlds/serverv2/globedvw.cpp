// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// GlobEdVw.cpp : implementation file
//

#include "stdafx.h"
#include "serverv2.h"
#include "GlobEdVw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GlobEdVw

IMPLEMENT_DYNCREATE(GlobEdVw, CEditView)

GlobEdVw::GlobEdVw()
{
	m_pfont=new CFont;
	m_pfont->CreatePointFont(90,"Courier New");
}

GlobEdVw::~GlobEdVw()
{
	if(m_pfont) delete m_pfont;
}


BEGIN_MESSAGE_MAP(GlobEdVw, CEditView)
	//{{AFX_MSG_MAP(GlobEdVw)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GlobEdVw drawing

void GlobEdVw::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

}

/////////////////////////////////////////////////////////////////////////////
// GlobEdVw diagnostics

#ifdef _DEBUG
void GlobEdVw::AssertValid() const
{
	CEditView::AssertValid();
}

void GlobEdVw::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// GlobEdVw message handlers

void GlobEdVw::OnInitialUpdate() 
{
	CEditView::OnInitialUpdate();

	// default is 32
	CEditView::SetTabStops(16);

	CEdit& ctl = GetEditCtrl( );

	ctl.SetFont(m_pfont);

	ctl.SetWindowText("Server starting up. Just a moment please...");

	ctl.SetReadOnly();

	SetTimer(2, 1000, NULL);	

	//m_pApp->m_pMainFrame->MDITile(MDITILE_HORIZONTAL);
	m_pApp->m_pMainFrame->MDICascade();
}

void GlobEdVw::OnDestroy() 
{
	CEditView::OnDestroy();
}

void GlobEdVw::OnTimer(UINT nIDEvent) 
{
	CEdit& ctl = GetEditCtrl( );
	DWORD queueSize,totalReq,dwTotalConnections;
	BSTR bstrWorldInfo;

	// Construct msg.
	if(m_pApp && m_pApp->m_pServer)
	{
		m_pApp->m_pServer->GetStatusInfo(&queueSize,
									&totalReq,
									&bstrWorldInfo,
									&dwTotalConnections, &m_pApp->m_bConnectedToDS);
		CString msg(bstrWorldInfo);
		::SysFreeString(bstrWorldInfo);

		// Set the text.
		int nStart, nEnd;
		ctl.GetSel(nStart, nEnd);
		ctl.SetWindowText(msg);
		ctl.SetSel(nStart, nEnd, FALSE);
	}	
	CEditView::OnTimer(nIDEvent);
}

BOOL GlobEdVw::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	BOOL bret = CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

	m_pApp = (CServerV2App *)AfxGetApp();

	//m_pApp->m_pMainFrame->MDITile(MDITILE_HORIZONTAL);
	m_pApp->m_pMainFrame->MDICascade();

	return bret;

}
