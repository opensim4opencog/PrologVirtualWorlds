// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// InfoURL.cpp : implementation file
//

#include "stdafx.h"
#include "serverv2.h"
#include "InfoURL.h"
#include <wininet.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInfoURL dialog


CDlgInfoURL::CDlgInfoURL(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInfoURL::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInfoURL)
	m_strInfoURL = _T("");
	//}}AFX_DATA_INIT
}


void CDlgInfoURL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInfoURL)
	DDX_Text(pDX, IDC_EDIT_URL, m_strInfoURL);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInfoURL, CDialog)
	//{{AFX_MSG_MAP(CDlgInfoURL)
		ON_EN_CHANGE(IDC_EDIT_URL, OnChangeEditUrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInfoURL message handlers
void CDlgInfoURL::OnChangeEditUrl() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	//scan the string for

	// TODO: Add your control notification handler code here
	//Scan 
}

void CDlgInfoURL::OnOK() 
{
	//char szPath[_MAX_PATH];
	//ULONG lBufLength = _MAX_PATH;
	// TODO: Add extra validation here
/*	if (FALSE==InternetCanonicalizeUrl(m_strInfoURL, szPath, 
		&lBufLength, ICU_NO_ENCODE )) 
	{
		AfxMessageBox("This must be a valid URL.");
	}
	else
	{
*/
	UpdateData();

	char buf[80];
	DWORD length = 80;	

	if(m_strInfoURL.GetLength()>0)
	{
		BOOL bret;	
		URL_COMPONENTS UrlComponents;
		memset(&UrlComponents, 0,sizeof(URL_COMPONENTS));
		UrlComponents.dwStructSize = sizeof(URL_COMPONENTS);
		bret = InternetCrackUrl(m_strInfoURL, m_strInfoURL.GetLength(), 0, &UrlComponents);
		if(bret==TRUE && (UrlComponents.nScheme!=INTERNET_SCHEME_UNKNOWN))
		{
			bret = InternetCanonicalizeUrl(m_strInfoURL, buf, &length, ICU_DECODE);
			if(bret==TRUE)
			{			
			
				m_strInfoURL = CString(buf);
				UpdateData(FALSE);
				CDialog::OnOK();
			}
			else
			{
				AfxMessageBox("The string has invalid characters.");
			}
		}
		else
			AfxMessageBox("The string is not a valid URL.");
	}
	else
		CDialog::OnOK();
	
}
