// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// LPEdit.cpp : implementation file
//

#include "stdafx.h"
#include "objexplr.h"
#include "EdLstCtl.h"
#include "LPEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLPEdit

CLPEdit::CLPEdit(CString strValue, RECT rect, CEditorListCtrl *plc)
{
	CFont *pFont = NULL;

	m_strNew = strValue;
	m_pListCtrl = plc;

	pFont = plc->GetFont();

	if (Create( WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, rect, plc, 213 ))
	{
		SetWindowText(m_strNew);

		ShowWindow(SW_SHOWNORMAL);
		if (pFont)
			SetFont(pFont);

		EnableWindow();
		SetLimitText(256);

		SetFocus();
		SetSel(0,-1);
	}

	m_bCanceled = FALSE;
}

CLPEdit::~CLPEdit()
{
}


BEGIN_MESSAGE_MAP(CLPEdit, CEdit)
	//{{AFX_MSG_MAP(CLPEdit)
	ON_CONTROL_REFLECT(EN_MAXTEXT, OnMaxtext)
	ON_WM_KEYDOWN()
	ON_WM_MOUSEACTIVATE()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLPEdit message handlers

void CLPEdit::OnMaxtext() 
{
	// TODO: Add your control notification handler code here
	
}

void CLPEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_RETURN)
	{
		GotReturn();
		return;
	}
	else if (nChar == VK_ESCAPE)
	{
		m_pListCtrl->CancelEditing();
		return;
	}

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CLPEdit::GotReturn()
{
	GetWindowText(m_strNew);
	m_bCanceled = TRUE;
	m_pListCtrl->DoneEditing(m_strNew);
}

BOOL CLPEdit::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			GotReturn();
			return TRUE;
		}
		if (pMsg->wParam == VK_ESCAPE)
		{
			m_bCanceled = TRUE;
			m_pListCtrl->CancelEditing();
			return TRUE;
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}

int CLPEdit::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
/*	LPUNKNOWN lpUnk = GetControllingUnknown ();
	ASSERT (lpUnk);
	LPOLEOBJECT lpObj;
	VERIFY (S_OK == lpUnk->QueryInterface (IID_IOleObject, (void**)(&lpObj)));
	lpObj->DoVerb (OLEIVERB_UIACTIVATE, NULL, GetClientSite(), 0, NULL, NULL);
	lpObj->Release ();
*/
//	m_pListCtrl->ActivateEdit();
	return CEdit::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CLPEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);

	if (!m_bCanceled)
		GotReturn();
}
