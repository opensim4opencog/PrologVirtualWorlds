// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// ModuleDrop.cpp : implementation file
//

#include "stdafx.h"
#include "objexplr.h"
#include "methlist.h"
#include "ModDrop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModuleDrop

CModuleDrop::CModuleDrop(CString strCurrentValue, RECT rect, CMethodList *plc)
{
	m_pListCtrl = plc;
	m_strChoice = strCurrentValue;

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

CModuleDrop::~CModuleDrop()
{
}


BEGIN_MESSAGE_MAP(CModuleDrop, CComboBox)
	//{{AFX_MSG_MAP(CModuleDrop)
	ON_CONTROL_REFLECT(CBN_SELENDOK, OnSelendok)
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModuleDrop message handlers
int CModuleDrop::FillList()
{
	CStringList *pStrList;
	CString		strAdd;
	int iLength = 0;
	POSITION pos;

	pStrList = m_pListCtrl->GetModuleList();

	if (!pStrList)
		return 0;

    for( pos = pStrList->GetHeadPosition(); pos != NULL; )
	{
		strAdd = pStrList->GetNext( pos );
		if (strAdd.GetLength() > iLength)
			iLength = strAdd.GetLength();
		if (AddString(strAdd) == CB_ERR)
			return -1;
	}

	if (!m_strChoice.CompareNoCase(":VBScript"))
		m_strChoice = CString("Inline Module");

	if (SelectString(-1, m_strChoice) == CB_ERR)
	{
		// couldn't find the current string (it's probably a COM module)
		// add it and make it the first item in the list
		InsertString(0, m_strChoice);

		// just select the first item
		SetCurSel(0);
	}

	return iLength;
}

void CModuleDrop::OnSelendok() 
{
	// CommitChanges();
}

void CModuleDrop::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_ESCAPE)
	{
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

void CModuleDrop::CommitChanges()
{
	CString strSel;

	int iSel = GetCurSel();
	if (iSel == CB_ERR) return;

	GetLBText(iSel, strSel);

	m_pListCtrl->DoneEditingModule(strSel);
}

void CModuleDrop::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);

	if (!m_bCanceled)
		CommitChanges();
}
