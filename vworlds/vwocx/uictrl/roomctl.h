// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_ROOMCTL_H__DC9CA0F1_336F_11D1_BB22_00C04FC2CAB1__INCLUDED_)
#define AFX_ROOMCTL_H__DC9CA0F1_336F_11D1_BB22_00C04FC2CAB1__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// RoomCtl.h : Declaration of the CRoomCtrl ActiveX Control class.
#include "..\vwview\viewctl.h"
#include <afxcview.h>
#include "vwmenu.h"

/////////////////////////////////////////////////////////////////////////////
// CRoomList view
class CRoomCtrl;

class CRoomList : public CListCtrl
{
public:
	CRoomList();           // protected constructor used by dynamic creation
protected:
	DECLARE_DYNCREATE(CRoomList)

// Attributes
public:
	CRoomCtrl	*m_pParent;

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
		IT_UNKNOWN
	};

	BOOL m_bSelected;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRoomList)
	public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
public:
	void	RefreshItem(IThing *pThing);
	void	RemoveItem(IThing *pItem);
	void	AddItem(IThing *pItem);
	void	Refresh(void);
	void	InsertThing(IThing *pDataItem);
	long	AssignIcon(IThing* pThing);
	virtual ~CRoomList();
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CRoomList)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchanging(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
};

/////////////////////////////////////////////////////////////////////////////
// CRoomCtrl : See RoomCtl.cpp for implementation.

class CRoomCtrl : public CVWViewCtrl
{
	DECLARE_DYNCREATE(CRoomCtrl)

// Constructor
public:
	CVWMenu			m_Menu;
	CRoomCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRoomCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CRoomCtrl();

	DECLARE_OLECREATE_EX(CRoomCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CRoomCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CRoomCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CRoomCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CRoomCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg void OnCommandRange(UINT nID);
	afx_msg void OnCommandRangeUI(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CRoomCtrl)
	afx_msg LPDISPATCH GetVWClient();
	afx_msg void SetVWClient(LPDISPATCH newValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CRoomCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CRoomCtrl)
	dispidVWClient = 1L,
	//}}AFX_DISP_ID
	};
protected:
	virtual HRESULT HandleOnConnect(IDispatch* pWorld);
	virtual HRESULT HandleOnDisconnect(IDispatch* pWorld);
	virtual HRESULT HandleOnEnterRoom(IThing* pWhere, IThing* pWhat);
	virtual HRESULT HandleOnLeaveRoom(IThing* pWhere, IThing* pWhat);
	virtual HRESULT HandleOnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient);

	CRoomList	RoomList;
	CImageList	*m_pImageList;

	void OnBackColorChanged();
	void OnForeColorChanged();

	void OnFontChanged();
	void OnAmbientPropertyChange(DISPID dispid);

};


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROOMCTL_H__DC9CA0F1_336F_11D1_BB22_00C04FC2CAB1__INCLUDED)
