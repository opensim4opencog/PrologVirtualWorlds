// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// ListList.cpp : implementation file
//

#include "stdafx.h"
#include "objexplr.h"
#include "PrpEdCtl.h"
#include "PListCtl.h"
#include "ListList.h"
#include "listdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListList

CListList::CListList()
{
	m_pOCX = NULL;
	m_iSortColumn = 0;
	m_iColumnCount = 3;
}

CListList::~CListList()
{
}


BEGIN_MESSAGE_MAP(CListList, CEditorListCtrl)
	//{{AFX_MSG_MAP(CListList)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListList message handlers

int CListList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEditorListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	RECT rect;
	GetClientRect(&rect);
	int iScrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
	rect.right = rect.right - iScrollBarWidth;

	CString strColumnName, strColumnType, strColumnValue;
	strColumnName.LoadString(IDS_INDEX);
	strColumnType.LoadString(IDS_TYPE);
	strColumnValue.LoadString(IDS_VALUE);

	if (InsertColumn(0, strColumnName, LVCFMT_LEFT, 20, 0) != -1)
		if (InsertColumn(1, strColumnType, LVCFMT_LEFT, (int) ((rect.right - 20) *.4), 1) != -1)
			InsertColumn(2, strColumnValue, LVCFMT_LEFT, (int) ((rect.right - 20) *.6), 2);

	return 0;
}

void CListList::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CEditorListCtrl::OnRButtonDown(nFlags, point);

	if (!m_pOCX)
		return;

	m_pOCX->PopUpMenu(point);
}

int CALLBACK ListSort3Func(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CPropListData *pPLD1 = (CPropListData *) lParam1;
	CPropListData *pPLD2 = (CPropListData *) lParam2;
	int iColumn = (int) lParamSort;

	int iResult = 0;

	CString str1, str2;
	if (pPLD1 && pPLD2)
	{
		switch( iColumn )
		{
			case 0: // sort by index
				iResult = pPLD1->m_iIndex > pPLD2->m_iIndex;
				break;
			case 1: // sort by type 
				str1 = pPLD1->m_strType;
				str2 = pPLD2->m_strType;
				iResult = lstrcmp(str1, str2);
				break;
			case 2:
				// sort by value
				// if we aren't sorting by value, this is an invalid condition
				str1 = pPLD1->m_strValue;
				str2 = pPLD2->m_strValue;
				iResult = lstrcmp(str1, str2);
				break;
			default:
				iResult = 0;
		}
	}

	return iResult;
}

void CListList::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CEditorListCtrl::OnColumnclick(pNMHDR, pResult);

	// Do the sort
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	m_iSortColumn = pNMListView->iSubItem;
	Sort();

	*pResult = 0;
}

void CListList::Sort()
{
	SortItems(ListSort3Func, (DWORD) m_iSortColumn);
	ShowFirstSelectedItem();
}

void CListList::ShowButton(int nItem)
{
	CEditorListCtrl::ShowButton(nItem);

	((CPropListCtrl *) m_pOCX)->FireSelectionEvent();
}

void CListList::DeleteButton()
{
	CEditorListCtrl::DeleteButton();

	if (GetSelectedCount() == 0)
		((CPropListCtrl *) m_pOCX)->FireSelectionEvent(FALSE);
}
