// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// ServerV2View.h : interface of the CServerV2View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERV2VIEW_H__7FDB7AE3_1C36_11D2_8ADD_00104BC911D4__INCLUDED_)
#define AFX_SERVERV2VIEW_H__7FDB7AE3_1C36_11D2_8ADD_00104BC911D4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CServerV2View : public CListView
{
protected: // create from serialization only
	CServerV2View();
	DECLARE_DYNCREATE(CServerV2View)

// Attributes
public:
	CServerV2Doc* GetDocument();

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerV2View)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CServerV2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	HRESULT HandleOnReport(CString strString, long lType);
protected:
	void AppendToList(CString string);
// Generated message map functions
protected:
	//{{AFX_MSG(CServerV2View)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ServerV2View.cpp
inline CServerV2Doc* CServerV2View::GetDocument()
   { return (CServerV2Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERV2VIEW_H__7FDB7AE3_1C36_11D2_8ADD_00104BC911D4__INCLUDED_)
