// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// ServerV2View.cpp : implementation of the CServerV2View class
//

#include "stdafx.h"
#include "ServerV2.h"

#include "ServDoc.h"
#include "ServView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerV2View

IMPLEMENT_DYNCREATE(CServerV2View, CListView)

BEGIN_MESSAGE_MAP(CServerV2View, CListView)
	//{{AFX_MSG_MAP(CServerV2View)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	// Standard printing commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerV2View construction/destruction

CServerV2View::CServerV2View()
{
	int j=0;

}

CServerV2View::~CServerV2View()
{
}

BOOL CServerV2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CServerV2View diagnostics

#ifdef _DEBUG
void CServerV2View::AssertValid() const
{
	CView::AssertValid();
}

void CServerV2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CServerV2Doc* CServerV2View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CServerV2Doc)));
	return (CServerV2Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CServerV2View message handlers

void CServerV2View::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();	

			
	CListCtrl & ctl = GetListCtrl();
	
	ctl.RedrawWindow(); 


	ctl.SetImageList(&(((CServerV2App *)AfxGetApp())->imglist), TVSIL_NORMAL);
	
	ctl.InsertColumn(0, "Date/Time");

	ctl.SetColumnWidth(0, 130);

	ctl.InsertColumn(1, "Message");

	ctl.SetColumnWidth(1, 400);	

	UpdateWindow();

}

void CServerV2View::AppendToList(CString string)
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	
	CListCtrl & ctl = GetListCtrl();

	int index = ctl.GetItemCount();

	CString str;
	str.Format("%02d-%02d-%02d %02d:%02d:%02d",
					time.wMonth,
					time.wDay,
					time.wYear,
					time.wHour,
					time.wMinute,
					time.wSecond);

	//insert into list.
	LV_ITEM lvi;
	
	{
		lvi.mask = LVIF_TEXT;
		lvi.iItem = index ;
		lvi.iSubItem = 0;
		lvi.pszText = (char *)(LPCTSTR)str;
		lvi.iImage = 0;
		lvi.stateMask = 0;
		lvi.state = 0;
	}
	ctl.InsertItem(&lvi);	

	ctl.SetItemText(index ,1,(char *)(LPCTSTR)string);	

	ctl.EnsureVisible(index, FALSE);
}

HRESULT CServerV2View::HandleOnReport(CString strString, long lType)
{
	if(lType==C_CLOSEDOCUMENT)
	{
		GetParentFrame()->PostMessage(WM_CLOSE);
	}
	else
	{
		strString.Replace("\n", "");

		AppendToList(strString);
	}

	return S_OK;
}

void CServerV2View::OnDraw(CDC* pDC) 
{
	int i = 0;

}
 
BOOL CServerV2View::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	dwStyle |= LVS_REPORT|LVS_SHAREIMAGELISTS|LVS_SHOWSELALWAYS|LVS_SINGLESEL|WS_BORDER |WS_TABSTOP;
		
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CServerV2View::OnSize(UINT nType, int cx, int cy) 
{
	CListView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}


void CServerV2View::OnDestroy() 
{
	CListView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
