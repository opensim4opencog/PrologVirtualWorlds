// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_GLOBFRM_H__7E3D3566_2021_11D2_8ADD_00104BC911D4__INCLUDED_)
#define AFX_GLOBFRM_H__7E3D3566_2021_11D2_8ADD_00104BC911D4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// GlobFrm.h : header file
//
#include "globedvw.h"
#include "servView.h"

/////////////////////////////////////////////////////////////////////////////
// GlobFrm frame

class GlobFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(GlobFrm)
protected:
	GlobFrm();           // protected constructor used by dynamic creation

// Attributes
public:
	CServerV2View *m_pListView;
	GlobEdVw *m_pEditView;
	CSplitterWnd m_wndSplitter;
	CServerV2App * m_pApp;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GlobFrm)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CMDIFrameWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~GlobFrm();

	// Generated message map functions
	//{{AFX_MSG(GlobFrm)
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLOBFRM_H__7E3D3566_2021_11D2_8ADD_00104BC911D4__INCLUDED_)
