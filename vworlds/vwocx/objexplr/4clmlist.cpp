// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// 4ClmList.cpp : implementation file for a 4 Column List
//

#include "stdafx.h"
#include "objexplr.h"
#include "PrpEdCtl.h"
#include "tplstctl.h"
#include "4ClmList.h"
#include "listdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C4ClmList

C4ClmList::C4ClmList()
{
	m_pOCX = NULL;
	m_iSortColumn = 1;
	m_iColumnCount = 4;
}

C4ClmList::~C4ClmList()
{
}


BEGIN_MESSAGE_MAP(C4ClmList, CEditorListCtrl)
	//{{AFX_MSG_MAP(C4ClmList)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C4ClmList message handlers

int C4ClmList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEditorListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	RECT rect;
	GetClientRect(&rect);
	int iScrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
	rect.right = rect.right - iScrollBarWidth;

	CString strColumnIcon, strColumnName, strColumnType, strColumnValue;
	strColumnIcon.Empty();
	strColumnName.LoadString(IDS_NAME);
	strColumnType.LoadString(IDS_TYPE);
	strColumnValue.LoadString(IDS_VALUE);

	if (InsertColumn(0, strColumnIcon, LVCFMT_LEFT, 18, 0) != -1)
		if (InsertColumn(1, strColumnName, LVCFMT_LEFT, (int)(rect.right * .35) - 6, 1) != -1)
			if (InsertColumn(2, strColumnType, LVCFMT_LEFT, (int) (rect.right * .25) - 6, 2) != -1)
				InsertColumn(3, strColumnValue, LVCFMT_LEFT, (int) (rect.right * .4) - 6, 3);

	return 0;
}

/*
void C4ClmList::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CEditorListCtrl::OnLButtonDblClk(nFlags, point);

	if (!m_pOCX)
		return;

	int iItem;
	int icol1width, icol2width, icol3width;
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
	icol3width = GetColumnWidth(2);

	if (point.x < icol1width)
	{
		// double clicked on icon
		iColumn = 0;
	}
	else if (point.x < icol1width+icol2width)
	{
		// double clicked on name
		iColumn = 1;
	}
	else if (point.x < icol1width+icol2width+icol3width)
	{
		// double clicked on type
		iColumn = 2;
	}
	else
	{
		// double clicked on value
		iColumn = 3;
	}

	m_pOCX->DoubleClick(iItem, iColumn);
}
*/


void C4ClmList::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CEditorListCtrl::OnRButtonDown(nFlags, point);

	if (!m_pOCX)
		return;

	m_pOCX->PopUpMenu(point);
}

int CALLBACK MapSort4Func(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
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
			case 0: // sort by type
				iResult = pPLD1->m_type > pPLD2->m_type;
				break;
			case 1: // sort by name
				str1 = pPLD1->m_strName;
				str2 = pPLD2->m_strName;
				iResult = lstrcmp(str1, str2);
				break;
			case 2: // sort by type
				str1 = pPLD1->m_strType;
				str2 = pPLD2->m_strType;
				iResult = lstrcmp(str1, str2);
				break;
			case 3:
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

void C4ClmList::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CEditorListCtrl::OnColumnclick(pNMHDR, pResult);

	// Do the sort, baby
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	m_iSortColumn = pNMListView->iSubItem;
	Sort();

	*pResult = 0;
}

void C4ClmList::Sort()
{
	SortItems(MapSort4Func, (DWORD) m_iSortColumn);
	ShowFirstSelectedItem();
}

// ShowButton
// called to show the button to launch the external editor
// we override it because it gets called when properties are selected
void C4ClmList::ShowButton(int nItem)
{
	CPropListData *cpld = (CPropListData *) GetItemData(nItem);
	if (cpld)
		((CTPListCtrl *) m_pOCX)->m_strLastSelectedProperty = cpld->m_strName;

	CEditorListCtrl::ShowButton(nItem);
}

