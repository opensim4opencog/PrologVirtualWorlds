// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_SELFCTL_H__DC9CA0EC_336F_11D1_BB22_00C04FC2CAB1__INCLUDED_)
#define AFX_SELFCTL_H__DC9CA0EC_336F_11D1_BB22_00C04FC2CAB1__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// SelfCtl.h : Declaration of the CSelfCtrl ActiveX Control class.

#include "vwmenu.h"
#include <afxcview.h>

/////////////////////////////////////////////////////////////////////////////
// CInvList view
class CSelfCtrl;

class CInvList : public CListCtrl
{
public:
	CInvList();           // protected constructor used by dynamic creation
protected:
	DECLARE_DYNCREATE(CInvList)

// Attributes
public:
	BOOL m_bSelected;

// Operations
public:
	CSelfCtrl	*m_pParent;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInvList)
	public:
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
public:
	void RefreshItem(IThing *pThing);
	void RemoveItem(IThing *pItem);
	void AddItem(IThing *pItem);
	void Refresh(void);
	void InsertThing(IThing *pDataItem);
	virtual ~CInvList();

	enum IconType
	{
		IT_WORLD,
		IT_ROOM,
		IT_AVATAR,
		IT_ARTIFACT,
		IT_PORTAL,
		IT_EXEMPLAR,
		IT_GLOBAL,
		IT_THING,
		IT_ACCESSORY,
		IT_WORN,
		IT_UNKNOWN
	};

	long AssignIcon(IThing* pThing);

protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CInvList)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchanging(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CSelfCtrl : See SelfCtl.cpp for implementation.
#include "..\vwview\viewctl.h"

class CSelfCtrl : public CVWViewCtrl
{
	DECLARE_DYNCREATE(CSelfCtrl)

// Constructor
public:
	CVWMenu			m_Menu;
	CSelfCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelfCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CSelfCtrl();

	DECLARE_OLECREATE_EX(CSelfCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CSelfCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CSelfCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CSelfCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CSelfCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg void OnCommandRange(UINT nID);
	afx_msg void OnCommandRangeUI(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CSelfCtrl)
	afx_msg LPDISPATCH GetVWClient();
	afx_msg void SetVWClient(LPDISPATCH newValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()


// Event maps
	//{{AFX_EVENT(CSelfCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CSelfCtrl)
	dispidVWClient = 1L,
	//}}AFX_DISP_ID
	};

protected:
	virtual HRESULT HandleOnConnect(IDispatch* pWorld);
	virtual HRESULT HandleOnDisconnect(IDispatch* pWorld);
	virtual HRESULT HandleOnEnterInventory(IThing* pWhere, IThing* pWhat);
	virtual HRESULT HandleOnLeaveInventory(IThing* pWhere, IThing* pWhat);
	virtual HRESULT HandleOnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient);
	virtual HRESULT HandleOnWear(IThing* pWho, IThing* pWhat);
	virtual HRESULT HandleOnUnWear(IThing* pWho, IThing* pWhat);
	//virtual HRESULT HandleOnInvokeMethod( WORD wFlags, IThing* pthing, BSTR bstrName,DISPPARAMS* pdispparams);

	CInvList InvList;

public:
	CImageList*		m_pImageList;

	void OnBackColorChanged();
	void OnForeColorChanged();

	void OnFontChanged();
	void OnAmbientPropertyChange(DISPID dispid);
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELFCTL_H__DC9CA0EC_336F_11D1_BB22_00C04FC2CAB1__INCLUDED)
