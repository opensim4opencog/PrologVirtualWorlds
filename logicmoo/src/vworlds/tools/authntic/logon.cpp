// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// logon.cpp : implementation file
//

#include "stdafx.h"
#include "Authntic.h"
#include "resource.h"
#include "logon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogonDlg dialog


CLogonDlg::CLogonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogonDlg)
	m_strDomain = _T("");
	m_strPassword = _T("");
	m_strUser = _T("");
	//}}AFX_DATA_INIT
}


void CLogonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogonDlg)
	DDX_Text(pDX, IDC_DOMAIN, m_strDomain);
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_USER, m_strUser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogonDlg, CDialog)
	//{{AFX_MSG_MAP(CLogonDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogonDlg message handlers
