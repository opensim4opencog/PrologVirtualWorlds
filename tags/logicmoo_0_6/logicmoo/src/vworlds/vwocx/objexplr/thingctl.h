// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_THINGCTL_H__EF7D6597_0161_11D2_88BE_00C04FC32EF3__INCLUDED_)
#define AFX_THINGCTL_H__EF7D6597_0161_11D2_88BE_00C04FC32EF3__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <vwuiview.h>
#include "Tree.h"

// ThingCtl.h : Declaration of the CThingTreeCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CThingTreeCtrl : See ThingCtl.cpp for implementation.

class CThingTreeCtrl : public CVWUIView
{
	DECLARE_DYNCREATE(CThingTreeCtrl)

// Constructor
public:
	CThingTreeCtrl();

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

	HTREEITEM	m_hModules;
	HTREEITEM	m_hWorld;
	BOOL		m_bSelectedByMe;

	void	NewSelection(HTREEITEM hSelection);

	// called by the tree to pop-up the menu
	void ContextMenu(HTREEITEM hItem, POINT pt);

	void VirtualKeyPressed(UINT nChar);
	void VirtualKeyUp(UINT nChar);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThingTreeCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CThingTreeCtrl();

	void FillWorldNode();
	HTREEITEM InTree(HTREEITEM hParent, long lID);
	HTREEITEM FindModuleByName(CString strName);
	HTREEITEM AddThingToTree(HTREEITEM hParentItem, IThing *pThing);
	long AssignIcon(IThing *pThing);

	void FillRoomNode();
	void AddContentsToTree(HTREEITEM hParent, IThing *pParent);

	CTree		m_treeCtrl;
	CImageList* m_pImageList;

	IThing *	m_pUser;

	HTREEITEM	m_hRoom;

	BOOL		m_bWizard;

	// EVENT HANDLERS
	// vwuiview events
	HRESULT HandleUIUpdate( enumVWUIEventTypes enumEventType, IVWUIView* pvwSrc,VARIANT varHint );

	// client events
	HRESULT HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient);
	HRESULT HandleOnUserConnect(IThing* pthing, BOOL bFromClient);

	// world events
	virtual HRESULT HandleOnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient);
	virtual HRESULT HandleOnModuleChanged(BSTR bstrName, long lHint, VARIANT varHintData);
	HRESULT NameChanged(IThing *pthing);
	HRESULT OnEnter(IThing *pthing, CComVariant varArg);
	HRESULT OnLeave(IThing *pthing, CComVariant varArg);
	
	DECLARE_OLECREATE_EX(CThingTreeCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CThingTreeCtrl)      // GetTypeInfo
	DECLARE_OLECTLTYPE(CThingTreeCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CThingTreeCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnCreateThing();
	afx_msg void OnDeleteThing();
	afx_msg void OnUpdateDeleteThing(CCmdUI* pCmdUI);
	afx_msg void OnDeleteModule();
	afx_msg void OnUpdateDeleteModule(CCmdUI* pCmdUI);
	afx_msg void OnReloadModule();
	afx_msg void OnUpdateReloadModule(CCmdUI* pCmdUI);
	afx_msg void OnNewModule();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateNewModule(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnThingProperties();
	afx_msg void OnUpdateThingProperties(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CThingTreeCtrl)
	afx_msg LPDISPATCH GetVWClient();
	afx_msg void SetVWClient(LPDISPATCH newValue);
	afx_msg void SelectThing(LPDISPATCH Thing);
	afx_msg void DoFocus();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CThingTreeCtrl)
	void FireNewThing()
		{FireEvent(eventidNewThing,EVENT_PARAM(VTS_NONE));}
	void FireLoadModule()
		{FireEvent(eventidLoadModule,EVENT_PARAM(VTS_NONE));}
	void FireKeyPressed(long key)
		{FireEvent(eventidKeyPressed,EVENT_PARAM(VTS_I4), key);}
	void FireVKeyUp(long key)
		{FireEvent(eventidVKeyUp,EVENT_PARAM(VTS_I4), key);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CThingTreeCtrl)
	dispidVWClient = 1L,
	dispidSelectThing = 2L,
	dispidDoFocus = 3L,
	eventidNewThing = 1L,
	eventidLoadModule = 2L,
	eventidKeyPressed = 3L,
	eventidVKeyUp = 4L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THINGCTL_H__EF7D6597_0161_11D2_88BE_00C04FC32EF3__INCLUDED)
