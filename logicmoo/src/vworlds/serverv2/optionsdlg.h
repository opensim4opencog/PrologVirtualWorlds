// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_OPTIONSDLG_H__7CD6D7E2_F83E_11D2_AEFA_00C04F797C5D__INCLUDED_)
#define AFX_OPTIONSDLG_H__7CD6D7E2_F83E_11D2_AEFA_00C04F797C5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsDlg.h : header file

// helpers for local group verification
#include <lm.h>
typedef NET_API_STATUS (_stdcall *PFNNLGGI)(LPWSTR, LPWSTR, DWORD, LPBYTE *); 
typedef NET_API_STATUS (_stdcall *PFNNABF)(LPVOID); 
#define MyNetLocalGroupGetInfo		(*m_pfnNLGGI)
#define MyNetApiBufferFree			(*m_pfnNABF)

/////////////////////////////////////////////////////////////////////////////
// CDialogOptions dialog

class CDialogOptions : public CDialog
{
// Construction
public:
	CDialogOptions(CWnd* pParent = NULL);   // standard constructor
	~CDialogOptions();  

// Dialog Data
	//{{AFX_DATA(CDialogOptions)
	enum { IDD = IDD_SERVEROPTIONS };
	BOOL	m_bAuthenticate;
	BOOL	m_bUseGroup;
	CString	m_cstrGroupName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogOptions)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	CServerV2App* m_pApp;
// Implementation
protected:
	HINSTANCE	m_hNetApi;
	PFNNLGGI	m_pfnNLGGI;
	PFNNABF		m_pfnNABF;

	// helper fctn
	HRESULT InitializeNetApi();

	
	// Generated message map functions
	//{{AFX_MSG(CDialogOptions)
	afx_msg void On_ckAuthentication();
	afx_msg void On_ckUseGroup();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSDLG_H__7CD6D7E2_F83E_11D2_AEFA_00C04F797C5D__INCLUDED_)
