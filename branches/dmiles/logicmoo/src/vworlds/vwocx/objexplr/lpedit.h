// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// LPEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLPEdit window

class CEditorListCtrl;

class CLPEdit : public CEdit
{
// Construction
public:
	CLPEdit(CString strValue, RECT rect, CEditorListCtrl *plc);
	void GotReturn();

// Attributes
public:
	CString m_strNew;

protected:
	CEditorListCtrl *m_pListCtrl;
	BOOL			m_bCanceled;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLPEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLPEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLPEdit)
	afx_msg void OnMaxtext();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
