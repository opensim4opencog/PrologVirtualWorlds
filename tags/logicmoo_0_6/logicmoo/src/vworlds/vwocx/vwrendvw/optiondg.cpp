// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// OptionDg.cpp : implementation file
//

#include "stdafx.h"
#include "vwrendvw.h"
#include "OptionDg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDialog dialog


COptionsDialog::COptionsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionsDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COptionsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsDialog, CDialog)
	//{{AFX_MSG_MAP(COptionsDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDialog message handlers

BOOL COptionsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (m_bWireframe)
	{
		CheckDlgButton(IDC_RB_WIREFRAME, TRUE);
		CheckDlgButton(IDC_RB_TEXTURED, FALSE);
	}
	else 
	{
		CheckDlgButton(IDC_RB_WIREFRAME, FALSE);
		CheckDlgButton(IDC_RB_TEXTURED, TRUE);
	}

	CheckDlgButton(IDC_IGNORECOLLISIONS_CHECK, m_bIgnoreCollisions);
	CheckDlgButton(IDC_IGNOREGRAVITY_CHECK, m_bIgnoreGravity);

	switch(m_nView)
	{
		case ID_VIEW_PERSPECTIVE:
			CheckRadioButton(ID_VIEW_PERSPECTIVE, ID_VIEW_RIGHT, ID_VIEW_PERSPECTIVE);
			break;
		case ID_VIEW_TOP:
			CheckRadioButton(ID_VIEW_PERSPECTIVE, ID_VIEW_RIGHT, ID_VIEW_TOP);
			break;
			break;
		case ID_VIEW_FRONT:
			CheckRadioButton(ID_VIEW_PERSPECTIVE, ID_VIEW_RIGHT, ID_VIEW_FRONT);
			break;
			break;
		case ID_VIEW_RIGHT:
			CheckRadioButton(ID_VIEW_PERSPECTIVE, ID_VIEW_RIGHT, ID_VIEW_RIGHT);
			break;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsDialog::OnOK() 
{

	if (IsDlgButtonChecked(IDC_RB_WIREFRAME))
		m_bWireframe = TRUE;
	else
		m_bWireframe = FALSE;

//	if (IsDlgButtonChecked(ID_VIEW_PERSPECTIVE))
//		m_nView = ID_VIEW_PERSPECTIVE;
//	else if (IsDlgButtonChecked(ID_VIEW_TOP))
//		m_nView = ID_VIEW_TOP;
//	else if (IsDlgButtonChecked(ID_VIEW_FRONT))
//		m_nView = ID_VIEW_FRONT;
//	else if (IsDlgButtonChecked(ID_VIEW_RIGHT))
//		m_nView = ID_VIEW_RIGHT;
//	else 
//	{
//		ASSERT(0);
//		m_nView = 0xFFFFFFFF;
//	}

	if (IsDlgButtonChecked(IDC_IGNORECOLLISIONS_CHECK))
		m_bIgnoreCollisions = TRUE;
	else
		m_bIgnoreCollisions = FALSE;

	if (IsDlgButtonChecked(IDC_IGNOREGRAVITY_CHECK))
		m_bIgnoreGravity = TRUE;
	else
		m_bIgnoreGravity = FALSE;

	CDialog::OnOK();
}
