// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_TREE_H__6A130DB9_0AA6_11D1_AFCF_00C04FB6A475__INCLUDED_)
#define AFX_TREE_H__6A130DB9_0AA6_11D1_AFCF_00C04FB6A475__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Tree.h : header file
//

#include <afxcmn.h>

/////////////////////////////////////////////////////////////////////////////
// CTree window

class CThingTreeCtrl;

class CTree : public CTreeCtrl
{
// Construction
public:
	CTree();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTree();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTree)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	CThingTreeCtrl* m_pOCX;
	BOOL m_bCollapsingModules;

private:
	HTREEITEM m_hItem;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Tree_H__6A130DB9_0AA6_11D1_AFCF_00C04FB6A475__INCLUDED_)
