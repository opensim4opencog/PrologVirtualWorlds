// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_LEBUTTON_H__9D8D4E24_1113_11D2_88C3_00C04FC32EF3__INCLUDED_)
#define AFX_LEBUTTON_H__9D8D4E24_1113_11D2_88C3_00C04FC32EF3__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// LEButton.h : header file
//

class CEditorListCtrl;

/////////////////////////////////////////////////////////////////////////////
// CLaunchEditorButton window

class CLaunchEditorButton : public CButton
{
// Construction
public:
	CLaunchEditorButton(RECT rect, CEditorListCtrl *plc);

// Attributes
public:
	CEditorListCtrl *m_pListCtrl;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLaunchEditorButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLaunchEditorButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLaunchEditorButton)
	afx_msg void OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEBUTTON_H__9D8D4E24_1113_11D2_88C3_00C04FC32EF3__INCLUDED_)
