// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// OCXView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COCXView view

class COCXView : public CView
{
protected:
	COCXView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(COCXView)

// Attributes
public:
	void UpdateVWClientObject(LPDISPATCH pClient);
	BOOL SetClient( CString strClientName);
	CWnd m_ClientCtrl;
	CString m_ClientName;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COCXView)
	public:
	virtual void OnFinalRelease();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~COCXView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(COCXView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnActivexcontrolsSetcontrol();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(COCXView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_EVENTSINK_MAP()
};

/////////////////////////////////////////////////////////////////////////////
