// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_DLGMAXLIMIT_H__4189B554_22B6_11D3_AEFC_00C04F797C5D__INCLUDED_)
#define AFX_DLGMAXLIMIT_H__4189B554_22B6_11D3_AEFC_00C04F797C5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMaxLimit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMaxLimit dialog

class CDlgMaxLimit : public CDialog
{
// Construction
public:
	CDlgMaxLimit(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMaxLimit)
	enum { IDD = IDD_SERVERMAXLIMIT };
	CString	m_strMaxLimit;
	int		m_btnNoLimit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMaxLimit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMaxLimit)
	afx_msg void OnRadioSetuserlimit();
	afx_msg void OnRadioNolimit();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMAXLIMIT_H__4189B554_22B6_11D3_AEFC_00C04F797C5D__INCLUDED_)
