// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// Tree.cpp : implementation file
//

#include "stdafx.h"
#include "objexplr.h"
#include "ThingCtl.h"
#include "Tree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define REFRESH_DELAY 500
#define TIMER_ID 914

/////////////////////////////////////////////////////////////////////////////
// CTree

CTree::CTree()
{
	m_pOCX = NULL;
	m_hItem = 0;
	m_bCollapsingModules = FALSE;
}

CTree::~CTree()
{
	m_pOCX = NULL;
}


BEGIN_MESSAGE_MAP(CTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CTree)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_TIMER()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_NOTIFY_REFLECT(TVN_SELCHANGING, OnSelchanging)
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTree message handlers



void CTree::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	// if we didnt make the selection, dont start the timer and dont call NewSelection
	if(!m_pOCX->m_bSelectedByMe)
	{
		m_pOCX->m_bSelectedByMe = TRUE;
		*pResult = 0;
		return;
	}
	// get the handle of the new selection
	m_hItem = pNMTreeView->itemNew.hItem;

	// Here is where we can regulate selection based on time on item
	KillTimer(TIMER_ID);
	if(!SetTimer(TIMER_ID, REFRESH_DELAY, NULL))
	{
		// couldnt set the timer, so call this explicitly
		m_pOCX->NewSelection(m_hItem);
	}
	*pResult = 0;
}

void CTree::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
/*	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	// get the handle of the new selection
	m_hItem = pNMTreeView->itemNew.hItem;
	long lData;

	lData = (long) GetItemData(m_hItem);

	if (lData == -1)
		*pResult = 1L;
	else */
		*pResult = 0;
}

void CTree::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == TIMER_ID)
	{
		m_pOCX->m_bSelectedByMe = TRUE;
		m_pOCX->NewSelection(m_hItem);
		KillTimer(TIMER_ID);
	}
}

/*
void CTree::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// get all of the necessary info
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	TV_ITEM tvItem = pNMTreeView->itemNew;
	UINT action = pNMTreeView->action;
	HTREEITEM hItem = tvItem.hItem;

	if(hItem == m_pOCX->m_hWorld)
	{
		*pResult = 0;
		return;
	}
	
/*	if(m_pOCX->CheckIfExemplar(hItem))
	{
		*pResult = 0;
		return;
	}

	if(action == TVE_EXPAND)
		m_pOCX->NodeIsExpanding(hItem);
	else if(action == TVE_COLLAPSE)
		m_pOCX->NodeIsCollapsing(hItem);

	//TODO - We may get action == TVE_TOGGLE here - need to test for that!!!
		
	*pResult = 0;
}

*/

void CTree::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	TV_ITEM tvItem = pNMTreeView->itemNew;
	HTREEITEM hItem = tvItem.hItem;
	UINT action = pNMTreeView->action;

	if (hItem == m_pOCX->m_hWorld)
	{
		*pResult = 1;
		return;
	}

	if ((hItem == m_pOCX->m_hModules) && ((action == TVE_COLLAPSE) || (action == TVE_TOGGLE)))
	{
		*pResult = 1;
		return;
	}

//	SortChildren(hItem);
	
	*pResult = 0;
}


int CTree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CFont *pFont = NULL;

	if (m_pOCX)
		pFont = ((CWnd *)m_pOCX)->GetFont();
	if (pFont)
		SetFont(pFont);
	
	return 0;
}

void CTree::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if ((nChar >= VK_BACK) && (nChar < VK_HELP))
		m_pOCX->VirtualKeyPressed(nChar);
	
	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTree::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if ((nChar >= VK_BACK) && (nChar < VK_HELP))
		m_pOCX->VirtualKeyUp(nChar);
	
	CTreeCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
}
