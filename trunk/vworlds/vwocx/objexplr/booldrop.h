// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_BOOLDROP_H__67F8C49B_E812_11D0_889C_00C04FC32EF3__INCLUDED_)
#define AFX_BOOLDROP_H__67F8C49B_E812_11D0_889C_00C04FC32EF3__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// BoolDrop.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBoolDrop window

class CEditorListCtrl;

class CBoolDrop : public CComboBox
{
// Construction
public:
	CBoolDrop(BOOL bCurrentValue, RECT rect, CEditorListCtrl *plc);

// Attributes
public:
	BOOL m_bChoice;

// Operations
public:
	void CommitChanges();

protected:
	int FillList();
	CEditorListCtrl *m_pListCtrl;
	BOOL m_bCanceled;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBoolDrop)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBoolDrop();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBoolDrop)
	afx_msg void OnSelendok();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOOLDROP_H__67F8C49B_E812_11D0_889C_00C04FC32EF3__INCLUDED_)
