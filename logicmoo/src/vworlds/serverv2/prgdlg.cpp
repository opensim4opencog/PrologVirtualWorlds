// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// PrgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "serverv2.h"
#include "PrgDlg.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ProgressDialog dialog


ProgressDialog::ProgressDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ProgressDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(ProgressDialog)
	m_Prompt = _T("");
	m_Operation = _T("");
	//}}AFX_DATA_INIT
}


void ProgressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ProgressDialog)
	DDX_Text(pDX, IDC_PROMPT, m_Prompt);
	DDX_Text(pDX, IDC_Operation, m_Operation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ProgressDialog, CDialog)
	//{{AFX_MSG_MAP(ProgressDialog)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



int ProgressDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	InterlockedIncrement(&m_pApp->m_iCurrentTasks);
	
	return 0;
}

BOOL ProgressDialog::DestroyWindow() 
{

	return CDialog::DestroyWindow();
}

void ProgressDialog::OnClose() 
{
	CDialog::OnClose();
}

ProgressDialog::~ProgressDialog()
{
	InterlockedDecrement(&m_pApp->m_iCurrentTasks);
}