// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// MethodList.cpp : implementation file
//

#include "stdafx.h"
#include "objexplr.h"
#include "PrpEdCtl.h"
#include "MethList.h"
#include "listdata.h"
#include "lebutton.h"
#include "ModDrop.h"
#include "tvlstctl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int CALLBACK MapSort4Func(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

#define ALWAYS_DROP_DOWN

/////////////////////////////////////////////////////////////////////////////
// CMethodList

CMethodList::CMethodList()
{
	m_pOCX = NULL;
	m_iSortColumn = 1;
	m_iColumnCount = 4;
	m_pModuleDrop = NULL;
}

CMethodList::~CMethodList()
{
	if (m_pModuleDrop)
		delete m_pModuleDrop;
	m_pOCX = NULL;
}


BEGIN_MESSAGE_MAP(CMethodList, CEditorListCtrl)
	//{{AFX_MSG_MAP(CMethodList)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMethodList message handlers

int CMethodList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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
	strColumnType.LoadString(IDS_MODULE);
	strColumnValue.LoadString(IDS_SUB);

	if (InsertColumn(0, strColumnIcon, LVCFMT_LEFT, 18, 0) != -1)
		if (InsertColumn(1, strColumnName, LVCFMT_LEFT, (int) (rect.right * .35) - 6, 1) != -1)
			if (InsertColumn(2, strColumnType, LVCFMT_LEFT, (int) (rect.right * .25) - 6, 2) != -1)
				InsertColumn(3, strColumnValue, LVCFMT_LEFT, (int) (rect.right * .4) - 6, 3);

	return 0;
}


void CMethodList::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CEditorListCtrl::OnRButtonDown(nFlags, point);

	if (!m_pOCX)
		return;

	m_pOCX->PopUpMenu(point);
}


void CMethodList::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CEditorListCtrl::OnColumnclick(pNMHDR, pResult);

	// Do the sort
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	m_iSortColumn = pNMListView->iSubItem;
	Sort();

	*pResult = 0;
}

void CMethodList::Sort()
{
	SortItems(MapSort4Func, (DWORD) m_iSortColumn);
	ShowFirstSelectedItem();
}

// ShowButton
// called to show the button to launch the external editor
void CMethodList::ShowButton(int nItem)
{
	RECT rect;
	m_iEditingItem = nItem;
	CPropListData *cpld = NULL;

	cpld = (CPropListData *) GetItemData(nItem);
	if (!cpld)
		return;

	// set the name of the last selected method
	((CTVListCtrl *) m_pOCX)->m_strLastSelectedMethod = cpld->m_strName;

	// only show this on in-line methods
	if (cpld->m_bInLineMethod && cpld->m_bEditable)
	{
		GetItemRect(nItem, &rect, LVIR_BOUNDS);
		rect.left = rect.right - 20;

		DeleteButton();
		m_pButton = new CLaunchEditorButton(rect, this);
	}
}

void CMethodList::DoneEditingModule(CString strModule)
{
	((CTVListCtrl *) m_pOCX)->DoneEditingModule(m_iEditingItem, strModule);

	DeleteEditors();
}

void CMethodList::EditModule(int iItem, CString strModule)
{
	RECT rect;
	m_iEditingItem = iItem;
	ShowItemAndColumn(iItem, 2, rect);

	CStringList *pModList = ((CTVListCtrl *) m_pOCX)->GetModuleList();

	// if ALWAYS_DROP_DOWN flag is not set and there is only one module
	// in the list then take a shortcut and exit immediately
#ifndef ALWAYS_DROP_DOWN
	if (pModList->GetCount() == 1)
	{
		CString strModule;
		strModule = pModList->GetHead();

		((CTVListCtrl *) m_pOCX)->DoneEditingModule(m_iEditingItem, strModule);
		return;
	}
#endif

	m_pModuleDrop = new CModuleDrop(strModule, rect, this);
	
	if (!m_pModuleDrop)
		return;

	m_pModuleDrop->SetFocus();
//	m_pModuleDrop->SetSel(0,-1);
}

void CMethodList::DeleteEditors()
{
	if (m_pModuleDrop)
	{
		delete m_pModuleDrop;
		m_pModuleDrop = NULL;
	}

	CEditorListCtrl::DeleteEditors();
}

void CMethodList::CommitEditors()
{
	if (m_pModuleDrop)
		m_pModuleDrop->CommitChanges();

	CEditorListCtrl::CommitEditors();
}

CStringList *CMethodList::GetModuleList()
{
	return ((CTVListCtrl *) m_pOCX)->GetModuleList();
}

void CMethodList::ColumnResize()
{
	RECT rect;
	GetClientRect(&rect);

	SetColumnWidth(1, LVSCW_AUTOSIZE);
	SetColumnWidth(2, LVSCW_AUTOSIZE);

	// now take care of the third column
	int iLeft = 0;
	for (int i = 0; i < 3; i++)
		iLeft += GetColumnWidth(i);

	SetColumnWidth(3, rect.right - iLeft);
}
