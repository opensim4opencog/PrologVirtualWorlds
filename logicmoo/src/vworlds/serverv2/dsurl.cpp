// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// DSURL.cpp : implementation file
//

#include "stdafx.h"
#include "serverv2.h"
#include "DSURL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDSURL dialog


CDlgDSURL::CDlgDSURL(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDSURL::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDSURL)
	m_strURL = _T("");
	//}}AFX_DATA_INIT
}


void CDlgDSURL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDSURL)
	DDX_Text(pDX, IDC_EDIT_URL, m_strURL);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDSURL, CDialog)
	//{{AFX_MSG_MAP(CDlgDSURL)
	ON_EN_CHANGE(IDC_EDIT_URL, OnChangeEditUrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDSURL message handlers

void CDlgDSURL::OnChangeEditUrl() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	//scan the string for

	// TODO: Add your control notification handler code here
	
}

#include <wininet.h>
void CDlgDSURL::OnOK() 
{
	CDialog::OnOK();
}
