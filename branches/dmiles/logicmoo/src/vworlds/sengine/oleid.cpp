// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// OleID.cpp : implementation file
//

#include "stdafx.h"
#include "sengine.h"
#include "OleID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COleIDDlg dialog


COleIDDlg::COleIDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COleIDDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COleIDDlg)
	m_OLEID = _T("VWView.VWViewCtrl.1");
	//}}AFX_DATA_INIT
}


void COleIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COleIDDlg)
	DDX_Text(pDX, IDC_EDIT1, m_OLEID);
	DDV_MaxChars(pDX, m_OLEID, 256);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COleIDDlg, CDialog)
	//{{AFX_MSG_MAP(COleIDDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COleIDDlg message handlers

void COleIDDlg::OnOK() 
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}
