// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// LEButton.cpp : implementation file
//

#include "stdafx.h"
#include "objexplr.h"
#include "LEButton.h"
#include "EdLstCtl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLaunchEditorButton

CLaunchEditorButton::CLaunchEditorButton(RECT rect, CEditorListCtrl *plc)
{
	CString strText;
	CFont *pFont;

	strText.LoadString(IDS_ELIPSES);
	m_pListCtrl = plc;

	pFont = plc->GetFont();

	if (Create( strText, BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_BORDER, rect, (CWnd *) plc, 213 ))
	{
//		SetWindowText(m_strNew);

		ShowWindow(SW_SHOWNORMAL);
		if (pFont)
			SetFont(pFont);

		EnableWindow();
//		SetLimitText(256);
	}
}

CLaunchEditorButton::~CLaunchEditorButton()
{
}


BEGIN_MESSAGE_MAP(CLaunchEditorButton, CButton)
	//{{AFX_MSG_MAP(CLaunchEditorButton)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLaunchEditorButton message handlers

void CLaunchEditorButton::OnClicked() 
{
	m_pListCtrl->LaunchExternalEditor();
}
