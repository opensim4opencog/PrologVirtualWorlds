// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_MENULIST_H__EF7D659C_0161_11D2_88BE_00C04FC32EF3__INCLUDED_)
#define AFX_MENULIST_H__EF7D659C_0161_11D2_88BE_00C04FC32EF3__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MENUList.h : header file
//

#include "EdLstCtl.h"

class CMethodDrop;

/////////////////////////////////////////////////////////////////////////////
// CMenuList window

class CMenuList : public CEditorListCtrl
{
// Construction
public:
	CMenuList();

	void Sort();
	void EditMethod(int nItem, CString strValue);
	void GetMethodList(CStringList &strMethodList);

	virtual void DeleteEditors();
	virtual void CommitEditors();

	virtual void ShowButton(int nItem);
	virtual void DeleteButton();
	void DoneEditingMethod(CString strNewValue);


// Attributes
public:
	int				m_iSortColumn;
	CMethodDrop		*m_pMethodEditor;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMenuList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMenuList)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENULIST_H__EF7D659C_0161_11D2_88BE_00C04FC32EF3__INCLUDED_)
