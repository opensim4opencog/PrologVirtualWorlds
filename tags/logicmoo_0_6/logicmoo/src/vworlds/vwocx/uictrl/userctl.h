// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_USERCTL_H__DC9CA0E7_336F_11D1_BB22_00C04FC2CAB1__INCLUDED_)
#define AFX_USERCTL_H__DC9CA0E7_336F_11D1_BB22_00C04FC2CAB1__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// UserCtl.h : Declaration of the CUsersCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CUsersCtrl : See UserCtl.cpp for implementation.
#include <afxwin.h>

#include "..\vwview\viewctl.h"
#include "vwmenu.h"
#include "cutils.h"

/////////////////////////////////////////////////////////////////////////////
// CUserListView view
class CUsersCtrl;

/*
class CUserRecord
{
public:
	// methods
			CUserRecord(IThing *pThing = NULL);
			CUserRecord(CUserRecord &otherRec);
			~CUserRecord();

	CUserRecord& operator=(CUserRecord &otherRec);
	void		Refresh();

	// data members
	BOOL			m_bIgnored;
	BOOL			m_bIgnoringYou;
	BOOL			m_bFriend;
	BOOL			m_bHost;
	CString			m_strName;
	CString			m_strPreviewIcon;
	IThing			*m_pThing;
	CStringArray	m_strEmblemIcon;
};
*/


// converted this from a list box to a listctrl
class CUserListView : public CListCtrl
{
protected:
	DECLARE_DYNCREATE(CUserListView)

// Attributes
public:
				CUserListView();     
	virtual		~CUserListView();

	CUsersCtrl	*m_pUserCtrl;
	CImageList* m_pImageList;
	int m_imagedx, m_imagedy;
	CMap <long, long, CThingPtr, CThingPtr&>	m_userMap;
	BOOL m_bSelected;

// Operations
public:
	HRESULT		AddThing(IThing *pThing);
	HRESULT		RemoveThing(IThing *pThing);
	HRESULT		RefreshList(void);
	HRESULT		ClearList(void);
	HRESULT		ResetImage(IThing *pThing);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserListView)
	public:
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//virtual void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct );
	//virtual int CompareItem( LPCOMPAREITEMSTRUCT lpCompareItemStruct );
	//virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//{{AFX_MSG(CUserListView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnItemchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRedrawList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CUsersCtrl control


class CUsersCtrl : public CVWViewCtrl
{
	DECLARE_DYNCREATE(CUsersCtrl)

// Constructor
public:
	CUsersCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUsersCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CUsersCtrl();

	DECLARE_OLECREATE_EX(CUsersCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CUsersCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CUsersCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CUsersCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CUsersCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg void OnCommandRange(UINT nID);
	afx_msg void OnCommandRangeUI(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CUsersCtrl)
	afx_msg LPDISPATCH GetVWClient();
	afx_msg void SetVWClient(LPDISPATCH newValue);
	afx_msg void RendererIsLoaded();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CUsersCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CUsersCtrl)
	dispidVWClient = 1L,
	dispidRendererIsLoaded = 2L,
	//}}AFX_DISP_ID
	};
	
	void OnBackColorChanged();
	void OnForeColorChanged();

	void OnFontChanged();
	void OnAmbientPropertyChange(DISPID dispid);

	// various member functions
	CVWMenu		m_Menu;
	CUserListView	m_userList;


	// VWView Overrides

	virtual HRESULT HandleOnUserConnect(IThing* pthing, BOOL bFromClient);
	virtual HRESULT HandleOnEnterRoom(IThing* pWhere, IThing* pWhat);
	virtual HRESULT HandleOnLeaveRoom(IThing* pWhere, IThing* pWhat);
	virtual HRESULT HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient);
	virtual HRESULT HandleOnDisconnect(IDispatch* pWorld);
	virtual HRESULT HandleOnGeometryChanged(IThing* pthing);
	virtual HRESULT HandleOnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient);

protected:
	// local UI event handlers
	HRESULT	HandleOnBefriend(IThing *pNewFriend);
	HRESULT HandleOnUnBefriend(IThing *pFormerFriend);
	HRESULT HandleOnIgnore(IThing *pIgnored);
	HRESULT HandleOnUnIgnore(IThing *pUnIngored);
};


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERCTL_H__DC9CA0E7_336F_11D1_BB22_00C04FC2CAB1__INCLUDED)
