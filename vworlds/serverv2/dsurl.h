// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_DSURL_H__E3655FBC_2FD2_11D2_B918_00C04F8EE774__INCLUDED_)
#define AFX_DSURL_H__E3655FBC_2FD2_11D2_B918_00C04F8EE774__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DSURL.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDSURL dialog

class CDlgDSURL : public CDialog
{
// Construction
public:
	CDlgDSURL(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDSURL)
	enum { IDD = IDD_SETDSURL };
	CString	m_strURL;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDSURL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDSURL)
	afx_msg void OnChangeEditUrl();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSURL_H__E3655FBC_2FD2_11D2_B918_00C04F8EE774__INCLUDED_)
