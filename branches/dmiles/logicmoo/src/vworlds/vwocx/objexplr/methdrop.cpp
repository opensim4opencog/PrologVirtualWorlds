// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// MethodDrop.cpp : implementation file
//

#include "stdafx.h"
#include "objexplr.h"
#include "menulist.h"
#include "MethDrop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BOOL CompareAndSwap(CStringList &strList, POSITION pos) { 

   CString temp;
   POSITION posFirst = pos;
   POSITION posNext = pos;
   
   strList.GetNext(posNext);

   if (!pos || !posFirst || !posNext)
	   return FALSE;

   if (strList.GetAt(posFirst).CompareNoCase(strList.GetAt(posNext)) > 0)
   {
      temp = strList.GetAt(posFirst);
      strList.SetAt(posFirst, strList.GetAt(posNext));
      strList.SetAt(posNext, temp);
      return TRUE;
   }
   return FALSE;

} 

void SortStringList(CStringList &strList)
{ 

   BOOL bNotDone = TRUE;
   POSITION pos;

   while (bNotDone)
   {
      bNotDone = FALSE;
      for(pos = strList.GetHeadPosition(); pos != NULL; strList.GetNext(pos))
         bNotDone |= CompareAndSwap(strList, pos);
   }
}


/////////////////////////////////////////////////////////////////////////////
// CMethodDrop

CMethodDrop::CMethodDrop(CString strCurrentValue, RECT rect, CMenuList *plc)
{
	m_pListCtrl = plc;
	m_strChoice = strCurrentValue;

	int iLength;
	CFont *pFont = NULL;

	pFont = plc->GetFont();

	rect.bottom += (rect.bottom - rect.top) * 10;
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

CMethodDrop::~CMethodDrop()
{
}


BEGIN_MESSAGE_MAP(CMethodDrop, CComboBox)
	//{{AFX_MSG_MAP(CMethodDrop)
	ON_CONTROL_REFLECT(CBN_SELENDOK, OnSelendok)
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMethodDrop message handlers
int CMethodDrop::FillList()
{
	CStringList StrList;
	CString		strAdd;
	int iLength = 0;
	POSITION pos;

	m_pListCtrl->GetMethodList(StrList);

	SortStringList(StrList);

	// add the none choice
	strAdd.LoadString(IDS_NONE);
	AddString(strAdd);

    for( pos = StrList.GetHeadPosition(); pos != NULL; )
	{
		strAdd = StrList.GetNext( pos );
		if (strAdd.GetLength() > iLength)
			iLength = strAdd.GetLength();
		if (AddString(strAdd) == CB_ERR)
			return -1;
	}

	if (m_strChoice.IsEmpty())
		m_strChoice.LoadString(IDS_NONE);

	if (SelectString(-1, m_strChoice) == CB_ERR)
		return iLength;

	return iLength;
}

void CMethodDrop::OnSelendok() 
{
	// CommitChanges()
}

void CMethodDrop::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
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

void CMethodDrop::CommitChanges()
{
	CString strSel;

	int iSel = GetCurSel();
	if (iSel == CB_ERR) return;

	GetLBText(iSel, strSel);

	m_pListCtrl->DoneEditingMethod(strSel);
}

void CMethodDrop::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);

	if (!m_bCanceled)
		CommitChanges();
}
