// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// DlgMaxLimit.cpp : implementation file
//

#include "stdafx.h"
#include "serverv2.h"
#include "DlgMaxLimit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMaxLimit dialog


CDlgMaxLimit::CDlgMaxLimit(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMaxLimit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMaxLimit)
	m_strMaxLimit = _T("");
	m_btnNoLimit = 0;
	//}}AFX_DATA_INIT
}


void CDlgMaxLimit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMaxLimit)
	DDX_Text(pDX, IDC_MAXLIMIT, m_strMaxLimit);
	DDV_MaxChars(pDX, m_strMaxLimit, 5);
	DDX_Radio(pDX, IDC_RADIO_NOLIMIT, m_btnNoLimit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMaxLimit, CDialog)
	//{{AFX_MSG_MAP(CDlgMaxLimit)
	ON_BN_CLICKED(IDC_RADIO_SETUSERLIMIT, OnRadioSetuserlimit)
	ON_BN_CLICKED(IDC_RADIO_NOLIMIT, OnRadioNolimit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMaxLimit message handlers

void CDlgMaxLimit::OnRadioSetuserlimit() 
{
	HWND hWnd=NULL;
	this->GetDlgItem(IDC_MAXLIMIT,&hWnd);
	if (hWnd)
	{
		::EnableWindow(hWnd,TRUE);
	}

}

void CDlgMaxLimit::OnRadioNolimit() 
{
	HWND hWnd=NULL;
	this->GetDlgItem(IDC_MAXLIMIT,&hWnd);
	if (hWnd)
	{
		::EnableWindow(hWnd,FALSE);
	}
	
}

BOOL CDlgMaxLimit::OnInitDialog() 
{
	HWND hWnd=NULL;
	DWORD dwLimit = 0;
	BOOL bOnWinNT=FALSE;
	CServerV2App* pApp = (CServerV2App *)AfxGetApp();
	CComBSTR bstrLimit;
	char buffer[33];
	memset(buffer,0,33);

	if (pApp && pApp->m_pServer)
	{
		//Now talk to the server object and get the current settings (from reg on startup)
		pApp->m_pServer->get_ConnectionLimit(&dwLimit);
	}

	if (dwLimit > 0)
	{
		m_btnNoLimit=1;
		_ultoa(dwLimit,buffer,10);		
		m_strMaxLimit=buffer;
	}

	//Enable or Disable items as appropriate
	this->GetDlgItem(IDC_MAXLIMIT,&hWnd);
	if (hWnd)
		::EnableWindow(hWnd, (BOOL) m_btnNoLimit);

	// Call Parent OnInitDialog
	CDialog::OnInitDialog();		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

