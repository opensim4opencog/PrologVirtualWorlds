// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// BoolDrop.cpp : implementation file
//

#include "stdafx.h"
#include "objexplr.h"
#include "EdLstCtl.h"
#include "BoolDrop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBoolDrop

CBoolDrop::CBoolDrop(BOOL bCurrentValue, RECT rect, CEditorListCtrl *plc)
{
	m_pListCtrl = plc;
	m_bChoice = bCurrentValue;

	int iLength;
	CFont *pFont = NULL;

	pFont = plc->GetFont();

	rect.bottom += (rect.bottom - rect.top) * 5;
	if (Create( WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | CBS_DROPDOWNLIST, rect, plc, 213 ))
	{
		iLength = FillList();
		ShowWindow(SW_SHOWNORMAL);
		SetDroppedWidth((UINT) (iLength * 10));
		if (pFont)
			SetFont(pFont);
		EnableWindow();
	}

	m_bCanceled = FALSE;
}

CBoolDrop::~CBoolDrop()
{
}


BEGIN_MESSAGE_MAP(CBoolDrop, CComboBox)
	//{{AFX_MSG_MAP(CBoolDrop)
	ON_CONTROL_REFLECT(CBN_SELENDOK, OnSelendok)
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBoolDrop message handlers
int CBoolDrop::FillList()
{
	CString strSelect;
	int iIndex, iErr;
	int iLength = 0;
	CString strTrue, strFalse;

	strTrue.LoadString(IDS_VALUE_TRUE);
	iLength = strTrue.GetLength();

	strFalse.LoadString(IDS_VALUE_FALSE);
	if (strFalse.GetLength() > iLength)
		iLength = strFalse.GetLength();

	if (m_bChoice == TRUE)
		strSelect = strTrue;
	else
		strSelect = strFalse;

	iIndex = AddString(strTrue);
	if (iIndex == CB_ERR) return -1;
	
	iErr = SetItemData(iIndex, (DWORD) 1);
	if (iErr == CB_ERR) return -1;

	iIndex = AddString(strFalse);
	if (iIndex == CB_ERR) return -1;
	
	iErr = SetItemData(iIndex, (DWORD) 0);
	if (iErr == CB_ERR) return -1;

	iErr = SelectString(-1, strSelect);
	if (iErr == CB_ERR) return iLength;

	return iLength;
}

void CBoolDrop::OnSelendok() 
{
	// CommitChanges
}

void CBoolDrop::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_ESCAPE)
	{
		m_bCanceled = TRUE;
		m_pListCtrl->CancelEditing();
		return;
	}
	else if (nChar == VK_DOWN)
	{
		if (!GetDroppedState())
			ShowDropDown();
	}

	CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CBoolDrop::CommitChanges()
{
	CString strTrue, strFalse;
	BOOL bVal;

	int iSel = GetCurSel();
	if (iSel == CB_ERR) return;

	bVal = (int) GetItemData(iSel);

	if (bVal)
		m_bChoice = TRUE;
	else
		m_bChoice = FALSE;

	strTrue.LoadString(IDS_VALUE_TRUE);
	strFalse.LoadString(IDS_VALUE_FALSE);

	m_pListCtrl->DoneEditing((m_bChoice) ? strTrue : strFalse);
}

void CBoolDrop::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);
	
	if (!m_bCanceled)
		CommitChanges();
	
}
