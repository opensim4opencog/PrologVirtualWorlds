// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_SELCTDLG_H__02DD394D_92C6_11D1_84EB_00C04FB6A5C8__INCLUDED_)
#define AFX_SELCTDLG_H__02DD394D_92C6_11D1_84EB_00C04FB6A5C8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SelctDlg.h : header file
//

#include <afxcmn.h>
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CSelectionDialog dialog

class CSelectionDialog : public CDialog
{
// Construction
public:
	BOOL AddThingToList(IThing* pThing);
	HRESULT Initialize(IVWControlManager* pControlManager, IPropertyList* pSelectedList);
	BOOL Refresh();
	void ShowThings(BOOL bThings);
	CSelectionDialog(CWnd* pParent = NULL);   // standard constructor
	~CSelectionDialog();

// Dialog Data
	//{{AFX_DATA(CSelectionDialog)
	enum { IDD = IDD_SELECTOR };
	CListCtrl	m_listControl;
	//}}AFX_DATA

	BOOL m_bShowThings;
	IVWControlManager* m_pControlManager;
	IPropertyList* m_pSelectedList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectionDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectionDialog)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELCTDLG_H__02DD394D_92C6_11D1_84EB_00C04FB6A5C8__INCLUDED_)
