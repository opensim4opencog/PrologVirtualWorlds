// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_LOGON_H__D71A5EEF_7332_11D1_9082_00C04FD91EFE__INCLUDED_)
#define AFX_LOGON_H__D71A5EEF_7332_11D1_9082_00C04FD91EFE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// logon.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogonDlg dialog

class CLogonDlg : public CDialog
{
// Construction
public:
	CLogonDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogonDlg)
	enum { IDD = IDD_LOGON };
	CString	m_strDomain;
	CString	m_strPassword;
	CString	m_strUser;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogonDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGON_H__D71A5EEF_7332_11D1_9082_00C04FD91EFE__INCLUDED_)
