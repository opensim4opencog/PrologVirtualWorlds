// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_GLOBEDVW_H__7E3D3567_2021_11D2_8ADD_00104BC911D4__INCLUDED_)
#define AFX_GLOBEDVW_H__7E3D3567_2021_11D2_8ADD_00104BC911D4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// GlobEdVw.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// GlobEdVw view

class GlobEdVw : public CEditView
{
protected:
	GlobEdVw();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(GlobEdVw)
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GlobEdVw)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~GlobEdVw();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(GlobEdVw)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CServerV2App *m_pApp;
	CFont *m_pfont;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLOBEDVW_H__7E3D3567_2021_11D2_8ADD_00104BC911D4__INCLUDED_)
