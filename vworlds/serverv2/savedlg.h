// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_SAVEDLG_H__7053BFF4_2434_11D3_AEFC_00C04F797C5D__INCLUDED_)
#define AFX_SAVEDLG_H__7053BFF4_2434_11D3_AEFC_00C04F797C5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SaveDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSaveDlg dialog

class CSaveDlg : public CDialog
{
// Construction
public:
	CSaveDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSaveDlg)
	enum { IDD = IDD_SAVELOGDLG };
	BOOL	m_ckOverwrite;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSaveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSaveDlg)
	afx_msg void OnCheck1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVEDLG_H__7053BFF4_2434_11D3_AEFC_00C04F797C5D__INCLUDED_)
