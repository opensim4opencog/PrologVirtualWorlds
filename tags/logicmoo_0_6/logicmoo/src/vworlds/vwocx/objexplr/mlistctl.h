// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_MLISTCTL_H__EF7D6592_0161_11D2_88BE_00C04FC32EF3__INCLUDED_)
#define AFX_MLISTCTL_H__EF7D6592_0161_11D2_88BE_00C04FC32EF3__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "PrpEdCtl.h"
#include <menuitem.h>
#include "MenuList.h"

// MListCtl.h : Declaration of the CMenuListCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CMenuListCtrl : See MListCtl.cpp for implementation.

class CMenuListCtrl : public CPropEdCtrl
{
	DECLARE_DYNCREATE(CMenuListCtrl)

// Constructor
public:
	CMenuListCtrl();

	virtual void DoneEditingName(int iItem, CString strNewValue);
	virtual void DoubleClick(int iItem, int iColumn);
	virtual void LeftClick(int iItem, int iColumn);
	virtual void PopUpMenu(CPoint point);
	virtual void VirtualKeyPressed(UINT nChar);
	virtual void VirtualKeyUp(UINT nChar);
	virtual void AlphaKeyPressed(char cLetter);
	void GetMethodList(CStringList &strMethodList);
	void DoneEditingMethod(int iItem, CString strNewMethod);
	void FireSelectionEvent(BOOL bSelections = TRUE);


	int GetIndexOfSelectedItem();
	int GetIndexOfItem(long pos);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuListCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Member Variables
protected:
	CMenuList		m_listCtrl;
	IThing *		m_pThing;
	IPropertyList *	m_pMenuList;
	CString			m_strPropName;
	BOOL			m_bCanAddToMenu;
	BOOL			m_bCanPutOnMenu;

// Member functions
protected:
	void FillList();	// populates the list control
	void ClearList();	// empties the list control
	void AddMenuItemToList(long iIndex, IMenuItem *pMenuItem); // adds an item to the list control
	void EditName(int nItem, CString strValue);
	void EditMethod(int nItem, CString strValue);
	int  FindItemByIndex(long lIndex);

	// World Events
	virtual HRESULT HandleOnPropertyChanged(IThing *pthing, BSTR bstr, long lHint, VARIANT varHintData);
	virtual HRESULT HandleOnDisconnect(IDispatch* pWorld);
	virtual HRESULT HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient);

// Implementation
protected:
	~CMenuListCtrl();

	DECLARE_OLECREATE_EX(CMenuListCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CMenuListCtrl)      // GetTypeInfo
	DECLARE_OLECTLTYPE(CMenuListCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CMenuListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAddMenuItem();
	afx_msg void OnDeleteMenuItem();
	afx_msg void OnUpdateDeleteMenuItem(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddMenuItem(CCmdUI* pCmdUI);
	afx_msg void OnMoveMenuItemDown();
	afx_msg void OnUpdateMoveMenuItemDown(CCmdUI* pCmdUI);
	afx_msg void OnMoveMenuItemUp();
	afx_msg void OnUpdateMoveMenuItemUp(CCmdUI* pCmdUI);
	afx_msg void OnEditMenuText();
	afx_msg void OnUpdateEditMenuText(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CMenuListCtrl)
	afx_msg LPDISPATCH GetTargetObjectProperty();
	afx_msg void SetTargetObjectProperty(LPDISPATCH newValue);
	afx_msg LPDISPATCH GetSelectionList();
	afx_msg void SetSelectionList(LPDISPATCH newValue);
	afx_msg BSTR GetPropertyName();
	afx_msg void SetPropertyName(LPCTSTR lpszNewValue);
	afx_msg void MoveSelectedPropertyUp();
	afx_msg void MoveSelectedPropertyDown();
	afx_msg void RemoveSelectedProperty();
	afx_msg void DoFocus();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CMenuListCtrl)
	void FireEditMenuItem(LPDISPATCH menuList, long item)
		{FireEvent(eventidEditMenuItem,EVENT_PARAM(VTS_DISPATCH  VTS_I4), menuList, item);}
	void FireNewMenuItem(LPDISPATCH menuList)
		{FireEvent(eventidNewMenuItem,EVENT_PARAM(VTS_DISPATCH), menuList);}
	void FireUserMessage(LPCTSTR message)
		{FireEvent(eventidUserMessage,EVENT_PARAM(VTS_BSTR), message);}
	void FireKeyPressed(long key)
		{FireEvent(eventidKeyPressed,EVENT_PARAM(VTS_I4), key);}
	void FireVKeyUp(long key)
		{FireEvent(eventidVKeyUp,EVENT_PARAM(VTS_I4), key);}
	void FireNoSelections()
		{FireEvent(eventidNoSelections,EVENT_PARAM(VTS_NONE));}
	void FireItemSelected()
		{FireEvent(eventidItemSelected,EVENT_PARAM(VTS_NONE));}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CMenuListCtrl)
	dispidTargetObjectProperty = 1L,
	dispidSelectionList = 2L,
	dispidPropertyName = 3L,
	dispidMoveSelectedPropertyUp = 4L,
	dispidMoveSelectedPropertyDown = 5L,
	dispidRemoveSelectedProperty = 6L,
	dispidDoFocus = 7L,
	eventidEditMenuItem = 1L,
	eventidNewMenuItem = 2L,
	eventidUserMessage = 3L,
	eventidNoSelections = 4L,
	eventidItemSelected = 5L,
	eventidKeyPressed = 6L,
	eventidVKeyUp = 7L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MLISTCTL_H__EF7D6592_0161_11D2_88BE_00C04FC32EF3__INCLUDED)
