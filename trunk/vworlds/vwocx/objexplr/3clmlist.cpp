// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// 3ClmList.cpp : implementation file for the 3 Column List
//

#include "stdafx.h"
#include "objexplr.h"
#include "PrpEdCtl.h"
#include "PMapCtl.h"
#include "3ClmList.h"
#include "listdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3ClmList

C3ClmList::C3ClmList()
{
	m_pOCX = NULL;
	m_iSortColumn = 0;
	m_iColumnCount = 3;
}

C3ClmList::~C3ClmList()
{
}


BEGIN_MESSAGE_MAP(C3ClmList, CEditorListCtrl)
	//{{AFX_MSG_MAP(C3ClmList)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3ClmList message handlers

int C3ClmList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEditorListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	RECT rect;
	int iScrollBarWidth;
	GetClientRect(&rect);

	iScrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
	rect.right = rect.right - iScrollBarWidth;

	CString strColumnName, strColumnType, strColumnValue;
	strColumnName.LoadString(IDS_NAME);
	strColumnType.LoadString(IDS_TYPE);
	strColumnValue.LoadString(IDS_VALUE);

	if (InsertColumn(0, strColumnName, LVCFMT_LEFT, (int) (rect.right * .35), 0) != -1)
		if (InsertColumn(1, strColumnType, LVCFMT_LEFT, (int) (rect.right * .25), 1) != -1)
			InsertColumn(2, strColumnValue, LVCFMT_LEFT, (int) (rect.right * .4), 2);

	return 0;
}

/*
void C3ClmList::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CEditorListCtrl::OnLButtonDblClk(nFlags, point);

	if (!m_pOCX)
		return;

	int iItem;
	int icol1width, icol2width;
	int iColumn = -1;
	UINT iFlags;
	BOOL bPos = TRUE;
	POINT ptItem;

	iItem = HitTest(point, &iFlags);

	bPos = GetItemPosition(iItem, &ptItem);
	if (!bPos) return;
	if (ptItem.x < 0) point.x -= ptItem.x;

	// got the item, figure out which column
	icol1width = GetColumnWidth(0);
	icol2width = GetColumnWidth(1);

	if (point.x < icol1width)
	{
		// double clicked on name
		iColumn = 0;
	}
	else if (point.x < icol1width+icol2width)
	{
		// double clicked on type
		iColumn = 1;
	}
	else
	{
		// double clicked on value
		iColumn = 2;
	}

	m_pOCX->DoubleClick(iItem, iColumn);
}
*/

void C3ClmList::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CEditorListCtrl::OnRButtonDown(nFlags, point);

	if (!m_pOCX)
		return;

	m_pOCX->PopUpMenu(point);
}

int CALLBACK MapSort3Func(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
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
			case 0: // sort by name
				str1 = pPLD1->m_strName;
				str2 = pPLD2->m_strName;
				iResult = lstrcmp(str1, str2);
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

void C3ClmList::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CEditorListCtrl::OnColumnclick(pNMHDR, pResult);

	// Do the sort
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	m_iSortColumn = pNMListView->iSubItem;
	Sort();

	*pResult = 0;
}

void C3ClmList::Sort()
{
	SortItems(MapSort3Func, (DWORD) m_iSortColumn);
	ShowFirstSelectedItem();
}

void C3ClmList::ShowButton(int nItem)
{
	CEditorListCtrl::ShowButton(nItem);

	((CPropMapCtrl *) m_pOCX)->FireSelectionEvent();
}

void C3ClmList::DeleteButton()
{
	CEditorListCtrl::DeleteButton();

	if (GetSelectedCount() == 0)
		((CPropMapCtrl *) m_pOCX)->FireSelectionEvent(FALSE);
}
