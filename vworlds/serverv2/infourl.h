// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_INFOURL_H__3A89BDA1_434E_11D2_BC86_00C04F8EE774__INCLUDED_)
#define AFX_INFOURL_H__3A89BDA1_434E_11D2_BC86_00C04F8EE774__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InfoURL.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgInfoURL dialog

class CDlgInfoURL : public CDialog
{
// Construction
public:
	CDlgInfoURL(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgInfoURL)
	enum { IDD = IDD_SETINFOURL };
	CString	m_strInfoURL;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInfoURL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInfoURL)
		afx_msg void OnChangeEditUrl();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFOURL_H__3A89BDA1_434E_11D2_BC86_00C04F8EE774__INCLUDED_)
