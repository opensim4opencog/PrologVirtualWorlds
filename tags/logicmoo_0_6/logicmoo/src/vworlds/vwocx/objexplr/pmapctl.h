// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_PMAPCTL_H__EF7D658D_0161_11D2_88BE_00C04FC32EF3__INCLUDED_)
#define AFX_PMAPCTL_H__EF7D658D_0161_11D2_88BE_00C04FC32EF3__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// PMapCtl.h : Declaration of the CPropMapCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CPropMapCtrl : See PMapCtl.cpp for implementation.

#include "3ClmList.h"

class CPropMapCtrl : public CPropEdCtrl
{
	DECLARE_DYNCREATE(CPropMapCtrl)

// Constructor
public:
	CPropMapCtrl();

	virtual void DoubleClick(int iItem, int iColumn);
	virtual void LeftClick(int iItem, int iColumn);
	virtual void PopUpMenu(CPoint point);
	virtual void VirtualKeyPressed(UINT nChar);
	virtual void VirtualKeyUp(UINT nChar);
	virtual void AlphaKeyPressed(char cLetter);
	virtual void DoneEditingValue(int iItem, CString strNewValue);
	virtual void DoneEditingName(int iItem, CString strNewValue);
	virtual void LaunchExternalEditor(int iItem);

	void FireSelectionEvent(BOOL bSelections = TRUE);

	CString	GetNameOfSelectedItem();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropMapCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Member Variables
protected:
	C3ClmList		m_listCtrl;
	IThing *		m_pThing;
	IPropertyMap *	m_pPropMap;
	CString			m_strPropName;
	BOOL			m_bCanAddToMap;
	BOOL			m_bCanPutOnMap;

// Member functions
protected:
	void FillList();	// populates the list control
	void ClearList();	// empties the list control
	void AddVariantItem(COleVariant varProperty, CString strName); // adds an item to the list control

	// World Events
	virtual HRESULT HandleOnPropertyChanged(IThing *pthing, BSTR bstr, long lHint, VARIANT varHintData);

	// client events
	virtual HRESULT HandleOnDisconnect(IDispatch* pWorld);
	virtual HRESULT HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient);

// Implementation
protected:
	~CPropMapCtrl();

	DECLARE_OLECREATE_EX(CPropMapCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CPropMapCtrl)      // GetTypeInfo
	DECLARE_OLECTLTYPE(CPropMapCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CPropMapCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAddProperty();
	afx_msg void OnDeleteProperty();
	afx_msg void OnUpdateDeleteProperty(CCmdUI* pCmdUI);
	afx_msg void OnEditName();
	afx_msg void OnUpdateEditName(CCmdUI* pCmdUI);
	afx_msg void OnEditValue();
	afx_msg void OnUpdateEditValue(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddProperty(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CPropMapCtrl)
	afx_msg LPDISPATCH GetSelectionList();
	afx_msg void SetSelectionList(LPDISPATCH newValue);
	afx_msg LPDISPATCH GetTargetObjectProperty();
	afx_msg void SetTargetObjectProperty(LPDISPATCH newValue);
	afx_msg BSTR GetPropertyName();
	afx_msg void SetPropertyName(LPCTSTR lpszNewValue);
	afx_msg BOOL ExternalSort(short column);
	afx_msg void DoFocus();
	afx_msg void RemoveSelectedProperty();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CPropMapCtrl)
	void FireEditValue(LPDISPATCH propertyMap, LPCTSTR name)
		{FireEvent(eventidEditValue,EVENT_PARAM(VTS_DISPATCH  VTS_BSTR), propertyMap, name);}
	void FireNewProperty(LPDISPATCH propertyMap)
		{FireEvent(eventidNewProperty,EVENT_PARAM(VTS_DISPATCH), propertyMap);}
	void FireUserMessage(LPCTSTR message)
		{FireEvent(eventidUserMessage,EVENT_PARAM(VTS_BSTR), message);}
	void FireKeyPressed(long key)
		{FireEvent(eventidKeyPressed,EVENT_PARAM(VTS_I4), key);}
	void FireVKeyUp(long key)
		{FireEvent(eventidVKeyUp,EVENT_PARAM(VTS_I4), key);}
	void FireItemSelected()
		{FireEvent(eventidItemSelected,EVENT_PARAM(VTS_NONE));}
	void FireNoSelections()
		{FireEvent(eventidNoSelections,EVENT_PARAM(VTS_NONE));}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CPropMapCtrl)
	dispidSelectionList = 1L,
	dispidTargetObjectProperty = 2L,
	dispidPropertyName = 3L,
	dispidSort = 4L,
	dispidDoFocus = 5L,
	dispidRemoveSelectedProperty = 6L,
	eventidEditValue = 1L,
	eventidNewProperty = 2L,
	eventidUserMessage = 3L,
	eventidKeyPressed = 4L,
	eventidVKeyUp = 5L,
	eventidItemSelected = 6L,
	eventidNoSelections = 7L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMAPCTL_H__EF7D658D_0161_11D2_88BE_00C04FC32EF3__INCLUDED)
