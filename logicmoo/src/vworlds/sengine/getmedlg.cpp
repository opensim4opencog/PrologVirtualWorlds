// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// GetMethodDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SEngine.h"
#include "GetMeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGetMethodDlg dialog


CGetMethodDlg::CGetMethodDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetMethodDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGetMethodDlg)
	m_MethodString = _T("");
	//}}AFX_DATA_INIT
}


void CGetMethodDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetMethodDlg)
	DDX_Text(pDX, IDC_EDIT1, m_MethodString);
	DDV_MaxChars(pDX, m_MethodString, 256);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGetMethodDlg, CDialog)
	//{{AFX_MSG_MAP(CGetMethodDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetMethodDlg message handlers
