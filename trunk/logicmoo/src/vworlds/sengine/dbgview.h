// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// DBGView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDBGView form view
#ifndef _DBGVIEW_H_
#define _DBGVIEW_H_

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include <vwobject.h>
#include <vwclient.h>

class CDBGView : public CFormView
{
protected:

	void OnRecalcLayout(int cx, int cy);

	CDBGView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDBGView)

// Form Data
public:
	void AddHistoryText( CString str );
	//{{AFX_DATA(CTextEntryView)

	enum { IDD = IDD_DIALOG2 };
	CButton	m_ReloadButton;
	CButton	m_EnterButton;
	CComboBox	m_SourceCombo;
	CEdit	m_HistoryEdit;
	CString	m_History;
	CString	m_Source;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDBGView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDBGView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDBGView)
		// NOTE - the ClassWizard will add and remove member functions here.
		afx_msg void OnEnter();
		afx_msg void OnButton1();
		afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CDBGView)
	afx_msg void OnTell(LPCTSTR str);
	afx_msg void OnTrace(LPCTSTR str);
	afx_msg void OnReport(LPCTSTR str, long lType);
	afx_msg void OnConnect(LPDISPATCH pWorld);
	afx_msg void OnDisconnect(LPDISPATCH pWorld);
	afx_msg void OnUserConnect(LPDISPATCH pThing);
	afx_msg void OnUserDisconnect(LPDISPATCH pThing);
	afx_msg void OnUserReconnect(IVWClient* newClient,
			IWorld* pWorld,	IThing* pthing);
	afx_msg void OnPreDestroy();
	afx_msg void OnEnterInventory(LPDISPATCH pWhere, LPDISPATCH pWhat);
	afx_msg void OnLeaveInventory(LPDISPATCH pWhere, LPDISPATCH pWhat);
	afx_msg void OnEnterRoom(LPDISPATCH pWhere, LPDISPATCH pWhat);
	afx_msg void OnLeaveRoom(LPDISPATCH pWhere, LPDISPATCH pWhat);
	afx_msg void OnUIEvent(LPDISPATCH pThing, LPCTSTR str, VARIANT varArg);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// -------- Event implementation -----
	DECLARE_INTERFACE_MAP()
	ULONG m_dwClientCookie;
	BEGIN_INTERFACE_PART(ClientSite, IVWClientSite)
       	STDMETHOD(OnConnect)(IWorld* pWorld);
		STDMETHOD(OnDisconnect)( IWorld* pWorld);

       	STDMETHOD(OnUserConnect)(IThing* pthing);
		STDMETHOD(OnUserDisconnect)(IThing* pthing);
		STDMETHOD(OnUserReconnect)(IVWClient* newClient, IWorld* pWorld, IThing* pthing);

		STDMETHOD(OnTrace)(BSTR bstr);
        STDMETHOD(OnReport)(BSTR bstr, long lType);

		STDMETHOD(OnUIEvent)(IThing *pthing, BSTR bstrEventName, VARIANT varArg);
	END_INTERFACE_PART(ClientSite)

private:
	HRESULT Advise( IUnknown *pUnk,REFIID riid, ULONG *pdwCoookie );
	HRESULT UnAdvise( IUnknown *pUnk,REFIID riid, ULONG dwCoookie );

// ----- END  event sink implementation -----

};

/////////////////////////////////////////////////////////////////////////////
#endif
