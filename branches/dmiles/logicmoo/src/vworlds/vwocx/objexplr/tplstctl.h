// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_TPLISTCTL_H__EF7D657E_0161_11D2_88BE_00C04FC32EF3__INCLUDED_)
#define AFX_TPLISTCTL_H__EF7D657E_0161_11D2_88BE_00C04FC32EF3__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// TPListCtl.h : Declaration of the CTPListCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CTPListCtrl : See TPListCtl.cpp for implementation.


#include "4ClmList.h"

enum IconType
{ 
	EXEMPLAR_PROPERTY,
	OVERRIDEN_PROPERTY,
	INSTANCE_PROPERTY,
	EXEMPLAR_PROPERTY_NO_WRITE,
	OVERRIDEN_PROPERTY_NO_WRITE,
	INSTANCE_PROPERTY_NO_WRITE,
	EXEMPLAR_PROPERTY_NO_EDIT,
	OVERRIDEN_PROPERTY_NO_EDIT,
	INSTANCE_PROPERTY_NO_EDIT,
	EXEMPLAR_PROPERTY_NO_WRITE_NO_EDIT,
	OVERRIDEN_PROPERTY_NO_WRITE_NO_EDIT,
	INSTANCE_PROPERTY_NO_WRITE_NO_EDIT,

};

class CPropListData;

class CTPListCtrl : public CPropEdCtrl
{
	DECLARE_DYNCREATE(CTPListCtrl)

// filter stuff
#define FILTER_INSTANCE_PROPERTIES 0x00000001
#define FILTER_EXEMPLAR_PROPERTIES 0x00000002
#define FILTER_WRITABLE_PROPERTIES 0x0000000F
#define FILTER_FRIENDLY_PROPERTIES 0x00000020

// Constructor
public:
	CTPListCtrl();

	virtual void DoubleClick(int iItem, int iColumn);
	virtual void LeftClick(int iItem, int iColumn);
	virtual void PopUpMenu(CPoint point);
	virtual void AlphaKeyPressed(char cLetter);
	virtual void VirtualKeyPressed(UINT nChar);
	virtual void VirtualKeyUp(UINT nChar);
	virtual void DoneEditingValue(int iItem, CString strNewValue);
	virtual void DoneEditingName(int iItem, CString strNewValue);

	virtual void LaunchExternalEditor(int iItem);
	CString	GetNameOfSelectedItem();

	// public member variable accessed by 4Clmlist
	CString		m_strLastSelectedProperty;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTPListCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Member Variables
protected:
	C4ClmList	m_listCtrl;
	IThing	*	m_pThing;	
	long		m_lFilterType;
	CImageList*	m_pListIcons;
	BOOL		m_bRespondToSelection;
	BOOL		m_bCanAddToThing;
	BOOL		m_bCanPutOnThing;

// Member functions
protected:
	void FillList();	// populates the list control
	void ClearList();	// empties the list control
	void AddItem(IPropertySecurity *pSecurity, CString strName, IconType &type); // adds an item to the list control

	BOOL DealWithCopyOnGet(CPropListData *cpld); // handles copying a property to the instance if necessary

	// World Events
	virtual HRESULT HandleOnPropertyChanged(IThing *pthing, BSTR bstr, long lHint, VARIANT varHintData);
	HRESULT HandleOnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient);

	// client events
	virtual HRESULT HandleOnDisconnect(IDispatch* pWorld);
	virtual HRESULT HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient);

	// CVWUIView handlers
	virtual HRESULT HandleUIUpdate( enumVWUIEventTypes eTypes, IVWUIView*,VARIANT varHint );							

// Implementation
protected:
	~CTPListCtrl();

	DECLARE_OLECREATE_EX(CTPListCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CTPListCtrl)      // GetTypeInfo
	DECLARE_OLECTLTYPE(CTPListCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CTPListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAddProperty();
	afx_msg void OnDeleteProperty();
	afx_msg void OnUpdateDeleteProperty(CCmdUI* pCmdUI);
	afx_msg void OnEditName();
	afx_msg void OnUpdateEditName(CCmdUI* pCmdUI);
	afx_msg void OnEditValue();
	afx_msg void OnUpdateEditValue(CCmdUI* pCmdUI);
	afx_msg void OnShowExemplarProperties();
	afx_msg void OnUpdateShowExemplarProperties(CCmdUI* pCmdUI);
	afx_msg void OnShowInstanceProperties();
	afx_msg void OnUpdateShowInstanceProperties(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddProperty(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CTPListCtrl)
	afx_msg LPDISPATCH GetSelectionList();
	afx_msg void SetSelectionList(LPDISPATCH newValue);
	afx_msg LPDISPATCH GetTargetObjectProperty();
	afx_msg void SetTargetObjectProperty(LPDISPATCH newValue);
	afx_msg BOOL GetRespondToSelectionEvents();
	afx_msg void SetRespondToSelectionEvents(BOOL bNewValue);
	afx_msg BOOL ExternalSort(short column);
	afx_msg void DoFocus();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CTPListCtrl)
	void FireEditValue(LPDISPATCH thing, LPCTSTR property)
		{FireEvent(eventidEditValue,EVENT_PARAM(VTS_DISPATCH  VTS_BSTR), thing, property);}
	void FireNewProperty(LPDISPATCH thing)
		{FireEvent(eventidNewProperty,EVENT_PARAM(VTS_DISPATCH), thing);}
	void FireUserMessage(LPCTSTR message)
		{FireEvent(eventidUserMessage,EVENT_PARAM(VTS_BSTR), message);}
	void FireKeyPressed(long key)
		{FireEvent(eventidKeyPressed,EVENT_PARAM(VTS_I4), key);}
	void FireVKeyUp(long key)
		{FireEvent(eventidVKeyUp,EVENT_PARAM(VTS_I4), key);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CTPListCtrl)
	dispidSelectionList = 1L,
	dispidTargetObjectProperty = 2L,
	dispidRespondToSelectionEvents = 3L,
	dispidSort = 4L,
	dispidDoFocus = 5L,
	eventidEditValue = 1L,
	eventidNewProperty = 2L,
	eventidUserMessage = 3L,
	eventidKeyPressed = 4L,
	eventidVKeyUp = 5L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPLISTCTL_H__EF7D657E_0161_11D2_88BE_00C04FC32EF3__INCLUDED)
