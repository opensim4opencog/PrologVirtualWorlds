// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// SEngineView.h : interface of the CSEngineView class
//
/////////////////////////////////////////////////////////////////////////////

class CSEngineView : public CEditView
{
protected: // create from serialization only
	CSEngineView();
	DECLARE_DYNCREATE(CSEngineView)

// Attributes
public:
	CSEngineDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSEngineView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSEngineView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSEngineView)
	afx_msg void OnEditGoto();
	afx_msg void OnUpdateEditGoto(CCmdUI* pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditDebugoffby1();
	afx_msg void OnUpdateEditDebugoffby1(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void UpdateRowColInfo();
private:
	int m_cOffBy1;
};

#ifndef _DEBUG  // debug version in SEngineView.cpp
inline CSEngineDoc* CSEngineView::GetDocument()
   { return (CSEngineDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
