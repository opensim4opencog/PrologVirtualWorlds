// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "serverv2.h"
#include "OptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogOptions dialog


CDialogOptions::CDialogOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogOptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogOptions)
	m_bAuthenticate = TRUE;
	m_bUseGroup = FALSE;
	m_cstrGroupName = _T("");
	//}}AFX_DATA_INIT
	m_hNetApi = NULL;
	m_pfnNLGGI = NULL;
}

CDialogOptions::~CDialogOptions()
{
	if (m_hNetApi)
		::FreeLibrary(m_hNetApi);
}


void CDialogOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogOptions)
	DDX_Check(pDX, IDC_CHECK1, m_bAuthenticate);
	DDX_Check(pDX, IDC_CHECK2, m_bUseGroup);
	DDX_Text(pDX, IDC_EDIT1, m_cstrGroupName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogOptions, CDialog)
	//{{AFX_MSG_MAP(CDialogOptions)
	ON_BN_CLICKED(IDC_CHECK1, On_ckAuthentication)
	ON_BN_CLICKED(IDC_CHECK2, On_ckUseGroup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogOptions message handlers

void CDialogOptions::On_ckAuthentication() 
{
	HWND hWnd=NULL;
	m_bAuthenticate=!m_bAuthenticate;
	this->GetDlgItem(IDC_CHECK2,&hWnd);
	if (hWnd)
		::EnableWindow(hWnd,m_bAuthenticate);
	hWnd=NULL;
	this->GetDlgItem(IDC_EDIT1,&hWnd);
	if (hWnd)
		::EnableWindow(hWnd,m_bAuthenticate);

		
}

void CDialogOptions::On_ckUseGroup() 
{
	// TODO: Add your control notification handler code here
	HWND hWnd=NULL;
	m_bUseGroup=!m_bUseGroup;
	this->GetDlgItem(IDC_EDIT1,&hWnd);
	if (hWnd)
		::EnableWindow(hWnd,m_bUseGroup);

}


BOOL CDialogOptions::OnInitDialog() 
{
	HWND hWnd=NULL;
	BSTR bstrGroupName=NULL;
	BOOL bOnWinNT=FALSE;
	m_pApp= (CServerV2App *)AfxGetApp();
	CEdit * pEditControl = NULL;

	if (m_pApp && m_pApp->m_pServer)
	{
		//Now talk to the server object and get the current settings
		m_pApp->m_pServer->get_UseAuthentication(&m_bAuthenticate);
		m_pApp->m_pServer->get_AuthenticationGroup(&bstrGroupName);
		m_pApp->m_pServer->get_RunningOnWinNT(&bOnWinNT);
	}

	//Enable or Disable items as appropriate
	this->GetDlgItem(IDC_CHECK2,&hWnd);
	if (hWnd)
		::EnableWindow(hWnd, bOnWinNT ? m_bAuthenticate : FALSE);
	hWnd=NULL;

	// set the text limit for the group to be 255
	this->GetDlgItem(IDC_EDIT1,&hWnd);
	pEditControl = (CEdit*) FromHandle(hWnd);
	if (pEditControl)
		pEditControl->SetLimitText(255);

	if (SysStringLen(bstrGroupName)>0)
	{
		m_cstrGroupName=bstrGroupName;
		if (hWnd)
			::EnableWindow(hWnd, bOnWinNT ? TRUE : FALSE);
		m_bUseGroup=TRUE;
	}
	else
	{
		m_bUseGroup=FALSE;
		if (hWnd)
			::EnableWindow(hWnd,FALSE);
	}

	// Call Parent OnInitDialog
	CDialog::OnInitDialog();		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogOptions::OnOK() 
{

	HRESULT hr = S_OK;
	LOCALGROUP_INFO_1* pLGI = NULL;
	
	UpdateData(TRUE);

	if (m_bUseGroup)
	{
		hr = InitializeNetApi();
		if FAILED(hr)
			AfxMessageBox("Failure Loading NetApi32.dll");
		
		hr = (HRESULT) MyNetLocalGroupGetInfo(NULL, m_cstrGroupName.AllocSysString(), 1, (BYTE**) &pLGI);
	}

//	REVIEW: I had to remove this call because it crashes.. waiting for feedback from Network
//			test team.. we may leak slightly but better than a crash..
//
//	if (pLGI)
//		MyNetApiBufferFree(pLGI);


//	if ((hr == NERR_GroupNotFound) || (hr == ERROR_NO_SUCH_ALIAS ))
	if (hr !=NERR_Success)	
		AfxMessageBox("Invalid Group Name");
	else
		CDialog::OnOK();
}


HRESULT CDialogOptions::InitializeNetApi()
{
	HRESULT hr = S_OK;

	if (m_hNetApi==NULL)
	{
		m_hNetApi = ::LoadLibrary("NETAPI32.DLL");
		if (m_hNetApi==NULL)
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
	}
	
	// get function pointers
	if (m_pfnNLGGI==NULL)
	{
		m_pfnNLGGI = (PFNNLGGI) ::GetProcAddress(m_hNetApi, "NetLocalGroupGetInfo");
		if (m_pfnNLGGI==NULL)
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
	}
	
	if (m_pfnNABF == NULL)
	{
		m_pfnNABF = (PFNNABF) ::GetProcAddress(m_hNetApi, "NetApiBufferFree");
		if (m_pfnNABF==NULL)
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}