// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "ServerV2.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_CLOSE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	return CMDIChildWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

void CChildFrame::OnClose() 
{
	WINDOWPLACEMENT wndplace;

	GetWindowPlacement(&wndplace);
	wndplace.showCmd=SW_SHOWMINIMIZED;

	SetWindowPlacement(&wndplace);
	CMDIChildWnd::OnClose();
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
/*	if(lpcs->cy == 0)
	{
		lpcs->cy = 370;
		lpcs->cx = 670;
		lpcs->y = 10;
		lpcs->x = 10;
	}
*/
	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

BOOL CChildFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	return CMDIChildWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);
}

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
/*	if(lpCreateStruct->cy == 0)
	{
		lpCreateStruct->cy = 370;
		lpCreateStruct->cx = 670;
		lpCreateStruct->y = 10;
		lpCreateStruct->x = 10;
	}
*/
	int returnval=CMDIChildWnd::OnCreate(lpCreateStruct);

	CMenu* pmenu = GetSystemMenu(FALSE);
	UINT size = pmenu->GetMenuItemCount( );
	pmenu->EnableMenuItem(pmenu->GetMenuItemID(6), MF_DISABLED | MF_GRAYED);
	//pmenu->DeleteMenu( 6, MF_BYPOSITION );
	//pmenu->RemoveMenu( 6, MF_BYPOSITION );
	DrawMenuBar();
/*	for(ULONG i = 0;i<size;i++)
	{
		CString str;
		pmenu->GetMenuString(i, str, MF_BYPOSITION);
		TRACE("%i: %s\n",i,str);
		pmenu->EnableMenuItem(pmenu->GetMenuItemID(i), MF_GRAYED|MF_DISABLED);
	}
*/	
	return returnval;
}

BOOL CChildFrame::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	return CMDIChildWnd::OnNotify(wParam, lParam, pResult);
}

BOOL CChildFrame::PreTranslateMessage(MSG* pMsg) 
{
	return CMDIChildWnd::PreTranslateMessage(pMsg);
}

BOOL CChildFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CMDIFrameWnd* pParentWnd, CCreateContext* pContext) 
{
	
/*	RECT tRect;
	tRect.left = 10;     
	tRect.top = 10;     
	tRect.right = 680;     
	tRect.bottom = 380;     
*/
	BOOL b = CMDIChildWnd::Create(lpszClassName, lpszWindowName, dwStyle|WS_MINIMIZE, rect, pParentWnd, pContext);

	m_pApp = (CServerV2App *)AfxGetApp();
	//m_pApp->m_pMainFrame->MDITile(MDITILE_HORIZONTAL);
	m_pApp->m_pMainFrame->MDICascade();

	return b;
}
