// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// MenuList.cpp : implementation file
//

#include "stdafx.h"
#include "objexplr.h"
#include "MListCtl.h"
#include "MenuList.h"
#include "listdata.h"
#include "methdrop.h"
#include <propbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMenuList

CMenuList::CMenuList()
{
	m_pOCX = NULL;
	m_iSortColumn = 0;
	m_iColumnCount = 2;
	m_pMethodEditor = NULL;
}

CMenuList::~CMenuList()
{
	if (m_pMethodEditor)
		delete m_pMethodEditor;
}


BEGIN_MESSAGE_MAP(CMenuList, CEditorListCtrl)
	//{{AFX_MSG_MAP(CMenuList)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuList message handlers

int CMenuList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEditorListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	RECT rect;
	GetClientRect(&rect);
	int iScrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
	rect.right = rect.right - iScrollBarWidth;

	CString strColumnName, strColumnType, strColumnValue;
	strColumnName.LoadString(IDS_MENU_TEXT);
	strColumnType.LoadString(IDS_METHOD);

	if (InsertColumn(0, strColumnName, LVCFMT_LEFT, (int) (rect.right / 2), 0) != -1)
		InsertColumn(1, strColumnType, LVCFMT_LEFT, (int) (rect.right / 2), 1);

	return 0;
}


void CMenuList::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CEditorListCtrl::OnRButtonDown(nFlags, point);

	if (!m_pOCX)
		return;

	m_pOCX->PopUpMenu(point);
}

int CALLBACK MenuSortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CPropListData *pPLD1 = (CPropListData *) lParam1;
	CPropListData *pPLD2 = (CPropListData *) lParam2;
	int iColumn = (int) lParamSort;

	int iResult = 0;

	if (pPLD1 && pPLD2)
	{
		if (pPLD1->m_iIndex < pPLD2->m_iIndex)
			iResult = -1;
		else if (pPLD1->m_iIndex > pPLD2->m_iIndex)
			iResult = 1;
	}

	return iResult;
}


void CMenuList::Sort()
{
	SortItems(MenuSortFunc, (DWORD) m_iSortColumn);
	ShowFirstSelectedItem();
}

void CMenuList::EditMethod(int nItem, CString strValue)
{
	// create the method drop-down
	RECT rect;
	m_iEditingItem = nItem;
	ShowItemAndColumn(nItem, 1, rect);

	m_pMethodEditor = new CMethodDrop(strValue, rect, this);
	m_pMethodEditor->SetFocus();

}

void CMenuList::GetMethodList(CStringList &strMethodList)
{
	((CMenuListCtrl *) m_pOCX)->GetMethodList(strMethodList);
}

void CMenuList::ShowButton(int nItem)
{
	((CMenuListCtrl *) m_pOCX)->FireSelectionEvent();
}

void CMenuList::DeleteButton()
{
	if (GetSelectedCount() == 0)
		((CMenuListCtrl *) m_pOCX)->FireSelectionEvent(FALSE);
}

//
// CommitEditors
// if the editors are up, get rid of them
//
void CMenuList::CommitEditors()
{
	if (m_pMethodEditor)
		m_pMethodEditor->CommitChanges();

	CEditorListCtrl::CommitEditors();
}

//
// DeleteEditors
// if the editors are up, get rid of them
//
void CMenuList::DeleteEditors()
{
	if (m_pMethodEditor)
		delete m_pMethodEditor;
	m_pMethodEditor = NULL;

	CEditorListCtrl::DeleteEditors();
}

// DoneEditing
// called by the value editors to push the new value back to the control
void CMenuList::DoneEditingMethod(CString strNewValue)
{
	((CMenuListCtrl *) m_pOCX)->DoneEditingMethod(m_iEditingItem, strNewValue);

	DeleteEditors();

	SelectItem(m_iEditingItem, 0, FALSE);
}
