// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// GlobFrm.cpp : implementation file
//

#include "stdafx.h"
#include "serverv2.h"
#include "GlobFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GlobFrm

IMPLEMENT_DYNCREATE(GlobFrm, CMDIChildWnd)

GlobFrm::GlobFrm()
{
}

GlobFrm::~GlobFrm()
{
}


BEGIN_MESSAGE_MAP(GlobFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(GlobFrm)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GlobFrm message handlers

BOOL GlobFrm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// create a splitter with 1 row, 2 columns
	if (!m_wndSplitter.CreateStatic(this, 2, 1))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}
	
	// add the first splitter pane - the default view in column 0
	RECT rect;
	GetClientRect(&rect);
	if (!m_wndSplitter.CreateView(0, 0,
		pContext->m_pNewViewClass, CSize(rect.right*2/3, rect.bottom/2), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}

	// add the second splitter pane - an input view in column 1
	if (!m_wndSplitter.CreateView(1, 0,
		RUNTIME_CLASS(GlobEdVw), CSize(rect.right/3, rect.bottom/2), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}

	// activate the input view
	SetActiveView((CView*)m_wndSplitter.GetPane(0,0));
	
	CMenu* pmenu = GetSystemMenu(FALSE);
	UINT size = pmenu->GetMenuItemCount( );
	pmenu->EnableMenuItem(pmenu->GetMenuItemID(6), MF_DISABLED | MF_GRAYED);
//	pmenu->DeleteMenu( 6, MF_BYPOSITION );
//	pmenu->RemoveMenu( 6, MF_BYPOSITION );
	DrawMenuBar();

	//set pointers correctly.
	return TRUE;
}

void GlobFrm::OnDestroy() 
{
//	CMDIChildWnd::OnDestroy();

}

BOOL GlobFrm::DestroyWindow() 
{
	return CMDIChildWnd::DestroyWindow();
}

void GlobFrm::OnClose() 
{
	WINDOWPLACEMENT wndplace;

	GetWindowPlacement(&wndplace);
	wndplace.showCmd=SW_SHOWMINIMIZED;

	SetWindowPlacement(&wndplace);

	//	CMDIChildWnd::OnClose();
}

BOOL GlobFrm::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CMDIFrameWnd* pParentWnd, CCreateContext* pContext) 
{
	m_pApp = (CServerV2App *)AfxGetApp();
//	m_pApp->m_pMainFrame->MDITile(MDITILE_HORIZONTAL);
	m_pApp->m_pMainFrame->MDICascade();	

	return CMDIChildWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, pContext);
}
