// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_PLISTCTL_H__EF7D6588_0161_11D2_88BE_00C04FC32EF3__INCLUDED_)
#define AFX_PLISTCTL_H__EF7D6588_0161_11D2_88BE_00C04FC32EF3__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// PListCtl.h : Declaration of the CPropListCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CPropListCtrl : See PListCtl.cpp for implementation.

#include "ListList.h"

class CPropListData;

class CPropListCtrl : public CPropEdCtrl
{
	DECLARE_DYNCREATE(CPropListCtrl)

// Constructor
public:
	CPropListCtrl();

	virtual void DoubleClick(int iItem, int iColumn);
	virtual void LeftClick(int iItem, int iColumn);
	virtual void PopUpMenu(CPoint point);
	virtual void VirtualKeyPressed(UINT nChar);
	virtual void VirtualKeyUp(UINT nChar);
	virtual void AlphaKeyPressed(char cLetter);
	virtual void DoneEditingValue(int iItem, CString strNewValue);
	virtual void LaunchExternalEditor(int iItem);
	void FireSelectionEvent(BOOL bSelections = TRUE);

	int	GetIndexOfSelectedItem();
	int GetIndexOfItem(long pos);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropListCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Member Variables
protected:
	CListList		m_listCtrl;
	IThing *		m_pThing;
	IPropertyList *	m_pPropList;
	CString			m_strPropName;
	BOOL			m_bCanAddToList;
	BOOL			m_bCanPutOnList;

// Member functions
protected:
	int	 FindItemByIndex(long lIndex);
	void FillList();	// populates the list control
	void ClearList();	// empties the list control
	void AddVariantItem(long iIndex, COleVariant varProperty); // adds an item to the list control

	// World Events
	virtual HRESULT HandleOnPropertyChanged(IThing *pthing, BSTR bstr, long lHint, VARIANT varHintData);
	virtual HRESULT HandleOnDisconnect(IDispatch* pWorld);
	virtual HRESULT HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient);

// Implementation
protected:
	~CPropListCtrl();

	DECLARE_OLECREATE_EX(CPropListCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CPropListCtrl)      // GetTypeInfo
	DECLARE_OLECTLTYPE(CPropListCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CPropListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAddProperty();
	afx_msg void OnDeleteProperty();
	afx_msg void OnUpdateDeleteProperty(CCmdUI* pCmdUI);
	afx_msg void OnEditValue();
	afx_msg void OnUpdateEditValue(CCmdUI* pCmdUI);
	afx_msg void OnMovePropDown();
	afx_msg void OnUpdateMovePropDown(CCmdUI* pCmdUI);
	afx_msg void OnMovePropUp();
	afx_msg void OnUpdateMovePropUp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddProperty(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CPropListCtrl)
	afx_msg LPDISPATCH GetSelectionList();
	afx_msg void SetSelectionList(LPDISPATCH newValue);
	afx_msg LPDISPATCH GetTargetObjectProperty();
	afx_msg void SetTargetObjectProperty(LPDISPATCH newValue);
	afx_msg BSTR GetPropertyName();
	afx_msg void SetPropertyName(LPCTSTR lpszNewValue);
	afx_msg void DoFocus();
	afx_msg void MoveSelectedPropertyUp();
	afx_msg void MoveSelectedPropertyDown();
	afx_msg void RemoveSelectedProperty();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CPropListCtrl)
	void FireEditValue(LPDISPATCH propertyList, long index)
		{FireEvent(eventidEditValue,EVENT_PARAM(VTS_DISPATCH  VTS_I4), propertyList, index);}
	void FireNewProperty(LPDISPATCH propertyList)
		{FireEvent(eventidNewProperty,EVENT_PARAM(VTS_DISPATCH), propertyList);}
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
	//{{AFX_DISP_ID(CPropListCtrl)
	dispidSelectionList = 1L,
	dispidTargetObjectProperty = 2L,
	dispidPropertyName = 3L,
	dispidDoFocus = 4L,
	dispidMoveSelectedPropertyUp = 5L,
	dispidMoveSelectedPropertyDown = 6L,
	dispidRemoveSelectedProperty = 7L,
	eventidEditValue = 1L,
	eventidNewProperty = 2L,
	eventidUserMessage = 3L,
	eventidKeyPressed = 4L,
	eventidVKeyUp = 5L,
	eventidNoSelections = 6L,
	eventidItemSelected = 7L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLISTCTL_H__EF7D6588_0161_11D2_88BE_00C04FC32EF3__INCLUDED)
