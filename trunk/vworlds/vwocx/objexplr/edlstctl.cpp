// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// EdLstCtl.cpp : implementation file
//

#include "stdafx.h"
#include "objexplr.h"
#include "PrpEdCtl.h"
#include "lpedit.h"
#include "booldrop.h"
#include "lebutton.h"
#include "EdLstCtl.h"
#include "listdata.h"

#include <ctype.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMER_ID 668 
#define REFRESH_DELAY 500

/////////////////////////////////////////////////////////////////////////////
// CEditorListCtrl

CEditorListCtrl::CEditorListCtrl()
{
	m_pOCX = NULL;
	m_pStringEditor = NULL;
	m_pBoolEditor = NULL;
	m_pButton = NULL;
	m_iColumnCount = 0;
	m_bEditingName = FALSE;
	m_iClickedItem = -1;
	m_iClickedColumn = -1;
	m_bTimerSet = FALSE;
	m_iEditingItem = 0;
	m_bInClick = FALSE;
}

CEditorListCtrl::~CEditorListCtrl()
{
	DeleteEditors();
}


BEGIN_MESSAGE_MAP(CEditorListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CEditorListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_VSCROLL()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemChanged)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_WM_TIMER()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditorListCtrl message handlers

// Delete Item
// overriden from CListCtrl so that we can delete the item data and not leak mem.
BOOL CEditorListCtrl::DeleteItem(int nItem)
{
	CPropListData * pPropListData = (CPropListData *) GetItemData(nItem);
	if (nItem == m_iEditingItem)
		DeleteEditors();
	else
		CancelEditing();
	if (pPropListData)
		delete pPropListData;
	return CListCtrl::DeleteItem(nItem);
}

// DeleteAllItems
// overriden from CListCtrl so that we can delete the item data and not leak mem.
BOOL CEditorListCtrl::DeleteAllItems()
{
	DeleteEditors();

	if (m_hWnd)
	{
		for (int i=0; i < GetItemCount(); i++)
		{
			CPropListData *cpld = (CPropListData *) GetItemData(i);
			if (cpld)
				delete cpld;
		}

		return CListCtrl::DeleteAllItems();
	}

	return TRUE;
}

// GetItemData
// overidden so we can verify that nItem is valid
DWORD CEditorListCtrl::GetItemData( int nItem )
{
	if ((nItem < GetItemCount()) && (nItem >= 0))
		return CListCtrl::GetItemData(nItem);
	else
		return NULL;
}

// Edit Name
// given the item number and the current value, and the column
// scrolls the list to the appropriate column and row
// and then pops up the string editor
BOOL CEditorListCtrl::EditName(int nItem, CString strValue, int iColumn)
{
	RECT rect;
	m_iEditingItem = nItem;
	ShowItemAndColumn(nItem, iColumn, rect);

	m_pStringEditor = new CLPEdit(strValue, rect, this);
	
	if (!m_pStringEditor)
		return FALSE;

	m_pStringEditor->SetFocus();
	m_pStringEditor->SetSel(0,-1);

	m_bEditingName = TRUE;
	return TRUE;
}


// Edit Value
// given the item number and the current value
// scrolls the list to the appropriate column and row
// and then pops up the string editor or boolean editor
BOOL CEditorListCtrl::EditValue(int nItem, CString strValue)
{
	RECT rect;
	m_iEditingItem = nItem;
	ShowItemAndColumn(nItem, m_iColumnCount-1, rect);

	m_pStringEditor = new CLPEdit(strValue, rect, this);
	
	if (!m_pStringEditor)
		return FALSE;

	m_pStringEditor->SetFocus();
	m_pStringEditor->SetSel(0,-1);

	return TRUE;
}

BOOL CEditorListCtrl::EditValue(int nItem, BOOL bValue)
{
	RECT rect;
	m_iEditingItem = nItem;
	ShowItemAndColumn(nItem, m_iColumnCount-1, rect);

	m_pBoolEditor = new CBoolDrop(bValue, rect, this);
	
	if (!m_pBoolEditor)
		return FALSE;

	m_pBoolEditor->SetFocus();

	return TRUE;
}

// ShowButton
// called to show the button to launch the external editor
void CEditorListCtrl::ShowButton(int nItem)
{
	RECT rect;
	m_iEditingItem = nItem;
	CPropListData *cpld = NULL;

	cpld = (CPropListData *) GetItemData(nItem);
	if (!cpld)
		return;

	if (!cpld->m_bEditable)
		return;

	switch(cpld->m_vtType)
	{
		case VT_BSTR:
		case VT_DISPATCH:
		case VT_UNKNOWN:
		case VT_DATE:
			DeleteButton();
			ShowItemAndColumn(nItem, -1, rect);
			rect.left = rect.right - 20;

			m_pButton = new CLaunchEditorButton(rect, this);
			break;
	}
}


// Delete Button
// called when an item is being unselected
// a sneaky way for controls to know when items are being un selected
void CEditorListCtrl::DeleteButton()
{
	if (m_pButton)
	{
		delete m_pButton;
		m_pButton = NULL;
	}	
}

// LaunchExternalEditor
// Calls the OCX
void CEditorListCtrl::LaunchExternalEditor()
{
	m_pOCX->LaunchExternalEditor(m_iEditingItem);
}

// GetItemAndColumnFromPoint
// pt - the point that was clicked
// iItem - returns the iItem
// iColumn - returns the column
// return value: TRUE if we clicked on an item, false if not
BOOL CEditorListCtrl::GetItemAndColumnFromPoint(POINT point, int &iItem, int &iColumn)
{
	int icol1width, icol2width, icol3width;
	UINT iFlags;
	BOOL bPos = TRUE;
	POINT ptItem;

	iItem = HitTest(point, &iFlags);
	if (iItem > GetItemCount() - 1)
		return FALSE;

	if (iItem < 0)
		return FALSE;

	bPos = GetItemPosition(iItem, &ptItem);
	if (!bPos)
		return FALSE;
	
	if (ptItem.x < 0) point.x -= ptItem.x;

	// got the item, figure out which column
	icol1width = GetColumnWidth(0);
	icol2width = GetColumnWidth(1);
	if (m_iColumnCount > 2)
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

	return TRUE;
}

// ClearSelections
// clear out all the selections in the list
void CEditorListCtrl::ClearSelections()
{
	int iItem;
	iItem = GetNextItem(-1, LVNI_SELECTED);
	while (iItem != -1)
	{
		// found it
	//	SetItemState(iItem, ~LVIS_DROPHILITED, LVIS_DROPHILITED);
		SetItemState(iItem, ~LVIS_SELECTED, LVIS_SELECTED);
		SetItemState(iItem, ~LVIS_FOCUSED, LVIS_FOCUSED);
		iItem = GetNextItem(-1, LVNI_SELECTED);
	}
	m_iClickedItem = -1;
}

// selectItem
// select an item in the control
void CEditorListCtrl::SelectItem(int iItem, int iColumn, BOOL bClearAll)
{
	int iCount;

	if (iItem < 0)
		iItem = 0;

	iCount = GetItemCount();

	if (iCount == 0)
		return;

	if (iItem >= iCount)
		iItem = iCount - 1;

	if (bClearAll)
		ClearSelections();

	// found it
	SetItemState(iItem, LVIS_SELECTED, LVIS_SELECTED);
//	SetItemState(iItem, LVIS_DROPHILITED, LVIS_DROPHILITED);
	SetItemState(iItem, LVIS_FOCUSED, LVIS_FOCUSED);
	ShowItemAndColumn(iItem, iColumn);
	m_iClickedItem = iItem;
}

// Find Item By Name
// given a name of an item, find it's item # or return -1
int CEditorListCtrl::FindItemByName(CString strName)
{
	for (int i = 0; i < GetItemCount(); i++)
	{
		CPropListData *cpld = (CPropListData *) GetItemData(i);
		if (cpld && strName.CompareNoCase(cpld->m_strName) == 0)
				return i;
	}

	return -1;
}

// DoneEditing
// called by the value editors to push the new value back to the control
void CEditorListCtrl::DoneEditing(CString strNewValue)
{
	if (!m_bEditingName)
		m_pOCX->DoneEditingValue(m_iEditingItem, strNewValue);
	else
		m_pOCX->DoneEditingName(m_iEditingItem, strNewValue);

	DeleteEditors();

	// if name changed, the item may have moved, so find and select the item again
	if(m_bEditingName)
		m_iEditingItem = FindItemByName(strNewValue);

	SelectItem(m_iEditingItem, 0, FALSE);

	// and since the item is selected, bring up the editor button again
	ShowButton(m_iEditingItem);
}

// DoneEditing
// called by the value editors to push the new value back to the control
void CEditorListCtrl::CancelEditing()
{
	DeleteEditors();
	// and show the button since we have something selected
	ShowButton(m_iEditingItem);
}

BOOL CEditorListCtrl::ShowFirstSelectedItem()
{
	int iItem;
	iItem = GetNextItem(-1, LVNI_SELECTED);
	if (iItem != -1)
		return ShowItemAndColumn(iItem, 0);
	else
		return FALSE;
}

// ShowItemAndColumn
// convenience version
BOOL CEditorListCtrl::ShowItemAndColumn(int iItem, int iColumn)
{
	RECT rWhatever;

	return ShowItemAndColumn(iItem, iColumn, rWhatever);
}

// ShowItemAndColumn
// given an item number and a column, scrolls the list control so that
// it shows as much as that item and column as possible and then returns
// a rect for it
BOOL CEditorListCtrl::ShowItemAndColumn(int iItem, int iColumn, RECT &rect)
{
	// bounds checking
	if ((iItem < 0) || (iItem > GetItemCount() - 1))
		return FALSE;

	if (iColumn >= m_iColumnCount)
		return FALSE;

	RECT rectClient;
	GetClientRect(&rectClient);

	RECT rectItem;
	POINT ptItem;
	BOOL bPos;
	BOOL bScrolled = FALSE;
	bPos = GetItemPosition(iItem, &ptItem);
	if (!bPos)
		return FALSE;
	int iTopIndex = GetTopIndex();
	int iBottomIndex = iTopIndex + GetCountPerPage() - 1;

	// is the item already visible?  (don't need to   up or down)
	if (!((iItem > iTopIndex) && (iItem <= iBottomIndex)))
	{
		CSize sizeY;
		sizeY.cx = 0;
		if (iItem <= iTopIndex)
			// scroll down
			sizeY.cy = ptItem.y - 20;
		else
		{
			// scroll up
			POINT ptBottomItem;
			bPos = GetItemPosition(iBottomIndex, &ptBottomItem);
			sizeY.cy = ptItem.y - ptBottomItem.y;
		}
		Scroll(sizeY);
		bScrolled = TRUE;
	}

	if (iColumn < 0)
	{
		// we only want to scroll in Y
		// get the new rect and return;
		bPos = GetItemRect(iItem, &rectItem, LVIR_BOUNDS);
		rect = rectItem;
		return bScrolled;
	}

	// now figure out how much to scroll based on the column
	int iLeftSide = 0;
	for (int i = 0; i < iColumn; i++)
		iLeftSide += GetColumnWidth(i);
	int iRightSide = iLeftSide + GetColumnWidth(iColumn);

	// scroll
	if (ptItem.x < 0)
	{
		CSize size(ptItem.x + iLeftSide, 0);
		Scroll(size);
		bScrolled = TRUE;

		// now get the info to set the rect
		bPos = GetItemPosition(iItem, &ptItem);
		if (!bPos)
			return FALSE;

		if (ptItem.x < 0)
		{
			rect.left = ptItem.x + iLeftSide;
			rect.right = ptItem.x + iRightSide;
		}
		else
		{
			rect.left = iLeftSide;
			rect.right = iRightSide;
		}
	}
	else
	{
		if (ptItem.x == 0)
		{
			rect.left = iLeftSide;
			rect.right = iRightSide;
		}
		else
		{
			// scrolled to the left
			CSize size(ptItem.x + iLeftSide, 0);
			Scroll(size);
			bScrolled = TRUE;
			bPos = GetItemPosition(iItem, &ptItem);
			if (!bPos)
				return FALSE;
			if (ptItem.x < 0)
			{
				rect.left = ptItem.x + iLeftSide;
				rect.right = ptItem.x + iRightSide;
			}
			else
			{
				rect.left = iLeftSide;
				rect.right = iRightSide;
			}
		}
	}

	if (rect.right >= rectClient.right)
		rect.right = rectClient.right;

	bPos = GetItemRect(iItem, &rectItem, LVIR_BOUNDS);


	rect.top = rectItem.top;
	rect.bottom = rectItem.bottom;

	if (bScrolled)
		DeleteEditors();

	return bScrolled;
}

//
// CommitEditors
// if any editor is up, commit it's change and
// then delete it
void CEditorListCtrl::CommitEditors()
{
	if (m_pStringEditor)
		m_pStringEditor->GotReturn();

	if (m_pBoolEditor)
		m_pBoolEditor->CommitChanges();

	m_bEditingName = FALSE;
}

//
// DeleteEditors
// if the editors are up, get rid of them
//
void CEditorListCtrl::DeleteEditors()
{
	if (m_pStringEditor)
	{
		delete m_pStringEditor;
		m_pStringEditor = NULL;
	}
	if (m_pBoolEditor)
	{
		delete m_pBoolEditor;
		m_pBoolEditor = NULL;
	}
	if (m_pButton)
	{
		delete m_pButton;
		m_pButton = NULL;
	}

	//m_bEditingName = FALSE;
}

void CEditorListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//DeleteEditors();
	CommitEditors();
	*pResult = 0;
}

void CEditorListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	//DeleteEditors();
	CommitEditors();
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CEditorListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//DeleteEditors();
	CommitEditors();
	m_bInClick = TRUE;
	
	CListCtrl::OnLButtonDown(nFlags, point);
}

void CEditorListCtrl::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == TIMER_ID)
	{
		m_pOCX->LeftClick(m_iClickedItem, m_iClickedColumn);
		m_bTimerSet = FALSE;
		KillTimer(TIMER_ID);
	}

	CListCtrl::OnTimer(nIDEvent);
}

void CEditorListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	KillTimer(TIMER_ID);
	m_bInClick = FALSE;

	if (m_iClickedItem == pNMListView->iItem)
	{
		m_iClickedColumn = pNMListView->iSubItem;

		if (m_bTimerSet)
			OnDblclk(pNMHDR, pResult);

		// couldnt set the timer, so call this explicitly
		if(!SetTimer(TIMER_ID, REFRESH_DELAY, NULL))
			m_pOCX->LeftClick(pNMListView->iItem, pNMListView->iSubItem);
		else
			m_bTimerSet = TRUE;
	}
	else
	{
		m_iClickedItem = pNMListView->iItem;
		m_iClickedColumn = pNMListView->iSubItem;
	}

	*pResult = 0;
}

void CEditorListCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	CommitEditors();
	DeleteEditors();
	
	KillTimer(TIMER_ID);
	m_bTimerSet = FALSE;	
	m_pOCX->DoubleClick(pNMListView->iItem, pNMListView->iSubItem);
	
	*pResult = 0;
}

void CEditorListCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	//DeleteEditors();
	CommitEditors();
	CListCtrl::OnRButtonDown(nFlags, point);
}

void CEditorListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	//DeleteEditors();
	CommitEditors();
	DeleteButton();
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CEditorListCtrl::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	// if the string editor is up, we dont care about turning the button on or off
	// per bug 5027
	if(m_pStringEditor != NULL)
		return;

	if (pNMListView->uChanged != 0)
	{
		if (pNMListView->uChanged & LVIF_STATE)
		{
			// the state has changed
			int iItem = pNMListView->iItem;
			
			if (pNMListView->uNewState & LVIS_FOCUSED)
			{
				// item has been selected
				// show the launch editor control
				ShowButton(iItem);
				if (!m_bInClick)
					m_iClickedItem = pNMListView->iItem;
			}
			else
			{
				if (pNMListView->uOldState & LVIS_FOCUSED)
				{
					// item has been un-selected
					// delete the button
					DeleteButton();
					m_iClickedItem = -1;
				}
			}
		}
	}

	*pResult = 0;
}

int CEditorListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	CFont *pFont = NULL;

	if (m_pOCX)
		pFont = ((CWnd *)m_pOCX)->GetFont();
	if (pFont)
		SetFont(pFont);
	
	return 0;
}

void CEditorListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);

	// look for the delete key
	if ((nChar >= VK_BACK) && (nChar < VK_HELP))
		m_pOCX->VirtualKeyPressed(nChar);
	else if (isalpha(nChar))
		m_pOCX->AlphaKeyPressed(nChar);
}

void CEditorListCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if ((nChar >= VK_BACK) && (nChar < VK_HELP))
		m_pOCX->VirtualKeyUp(nChar);
	
	CListCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
}
