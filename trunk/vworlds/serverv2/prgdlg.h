// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_PRGDLG_H__61DDA063_272D_11D2_8AE5_00104BC911D4__INCLUDED_)
#define AFX_PRGDLG_H__61DDA063_272D_11D2_8AE5_00104BC911D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrgDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ProgressDialog dialog

class ProgressDialog : public CDialog
{
// Construction
public:
	ProgressDialog(CWnd* pParent = NULL);   // standard constructor
	ProgressDialog(BOOL bModeless, CServerV2App *pApp) {ASSERT(bModeless); m_pApp=pApp;}

	~ProgressDialog(); 
	
// Dialog Data
	//{{AFX_DATA(ProgressDialog)
	enum { IDD = IDD_FORMVIEW };
	CString	m_Prompt;
	CString	m_Operation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ProgressDialog)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	CServerV2App *m_pApp;
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ProgressDialog)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRGDLG_H__61DDA063_272D_11D2_8AE5_00104BC911D4__INCLUDED_)
