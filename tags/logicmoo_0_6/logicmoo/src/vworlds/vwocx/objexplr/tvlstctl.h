// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_TVLISTCTL_H__EF7D6583_0161_11D2_88BE_00C04FC32EF3__INCLUDED_)
#define AFX_TVLISTCTL_H__EF7D6583_0161_11D2_88BE_00C04FC32EF3__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// TVListCtl.h : Declaration of the CTVListCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CTVListCtrl : See TVListCtl.cpp for implementation.

#include "MethList.h"
#include "PrpEdCtl.h"

enum IconType
{ 
	EXEMPLAR_METHOD,
	OVERRIDEN_METHOD,
	INSTANCE_METHOD,
	EXEMPLAR_METHOD_NO_WRITE,
	OVERRIDEN_METHOD_NO_WRITE,
	INSTANCE_METHOD_NO_WRITE,
};

class CPropListData;

class CTVListCtrl : public CPropEdCtrl
{
	DECLARE_DYNCREATE(CTVListCtrl)

// filter stuff
#define FILTER_INSTANCE_METHODS 0x00000001
#define FILTER_EXEMPLAR_METHODS 0x00000002
#define FILTER_WRITABLE_METHODS 0x0000000F
#define FILTER_FRIENDLY_METHODS 0x00000020

// Constructor
public:
	CTVListCtrl();

	virtual void DoubleClick(int iItem, int iColumn);
	virtual void LeftClick(int iItem, int iColumn);
	virtual void PopUpMenu(CPoint point);
	virtual void AlphaKeyPressed(char cLetter);
	virtual void VirtualKeyPressed(UINT nChar);
	virtual void VirtualKeyUp(UINT nChar);
	virtual void LaunchExternalEditor(int iItem);
	void		 EnterKeyPressed();
	virtual void DoneEditingValue(int iItem, CString strNewValue);
	virtual void DoneEditingName(int iItem, CString strNewValue);
	void		 DoneEditingModule(int iItem, CString strNewValue);

	CString	GetNameOfSelectedItem();
	CStringList* GetModuleList();

	// public member variable accessed by CMethlist
	CString		m_strLastSelectedMethod;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTVListCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Member Variables
protected:
	CMethodList		m_listCtrl;
	IThing	*		m_pThing;	
	long			m_lFilterType;
	CImageList*		m_pListIcons;
	BOOL			m_bRespondToSelection;
	CStringList*	m_pModuleNameList;
	BOOL			m_bModuleListDirty;
	BOOL			m_bCanAddToThing;
	BOOL			m_bCanPutOnThing;
	CPropListData * m_pCurrentlyEditing;

// Member functions
protected:
	void FillList();	// populates the list control
	void ClearList();	// empties the list control
	void AddItem(IPropertySecurity *pSec, CString strName, IconType type);
	void GetTypeAndValueFromMethod(IMethod *pMethod, CString &strType, CString &strValue);
	BOOL GetModuleAndSubFromMethod(IMethod *pMethod, CString &strModule, CString &strSub, BOOL &bScriptModule);
	BOOL DealWithCopyOnGet(CPropListData *cpld);

	// World Events
	virtual HRESULT HandleOnMethodChanged(IThing *pthing, BSTR bstr, long lHint, VARIANT varHintData);
	HRESULT HandleOnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient);
	virtual HRESULT HandleOnModuleChanged(BSTR bstrName, long lHint, VARIANT varHintData);

	// client events
	virtual HRESULT HandleOnDisconnect(IDispatch* pWorld);
	virtual HRESULT HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient);

	// CVWUIView handlers
	virtual HRESULT HandleUIUpdate( enumVWUIEventTypes eTypes, IVWUIView*,VARIANT varHint );							

// Implementation
protected:
	~CTVListCtrl();

	DECLARE_OLECREATE_EX(CTVListCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CTVListCtrl)      // GetTypeInfo
	DECLARE_OLECTLTYPE(CTVListCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CTVListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAddMethod();
	afx_msg void OnDeleteMethod();
	afx_msg void OnUpdateDeleteMethod(CCmdUI* pCmdUI);
	afx_msg void OnEditName();
	afx_msg void OnUpdateEditName(CCmdUI* pCmdUI);
	afx_msg void OnShowExemplarMethods();
	afx_msg void OnUpdateShowExemplarMethods(CCmdUI* pCmdUI);
	afx_msg void OnShowInstanceMethods();
	afx_msg void OnUpdateShowInstanceMethods(CCmdUI* pCmdUI);
	afx_msg void OnEditSubName();
	afx_msg void OnUpdateEditSubName(CCmdUI* pCmdUI);
	afx_msg void OnChangeModule();
	afx_msg void OnUpdateChangeModule(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddMethod(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CTVListCtrl)
	afx_msg LPDISPATCH GetSelectionList();
	afx_msg void SetSelectionList(LPDISPATCH newValue);
	afx_msg LPDISPATCH GetTargetObjectProperty();
	afx_msg void SetTargetObjectProperty(LPDISPATCH newValue);
	afx_msg BOOL GetRespondToSelectionEvents();
	afx_msg void SetRespondToSelectionEvents(BOOL bNewValue);
	afx_msg void ExternalSort(short column);
	afx_msg void DoFocus();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CTVListCtrl)
	void FireNewMethod(LPDISPATCH thing)
		{FireEvent(eventidNewMethod,EVENT_PARAM(VTS_DISPATCH), thing);}
	void FireEditValue(LPDISPATCH thing, LPCTSTR methodname)
		{FireEvent(eventidEditValue,EVENT_PARAM(VTS_DISPATCH  VTS_BSTR), thing, methodname);}
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
	//{{AFX_DISP_ID(CTVListCtrl)
	dispidSelectionList = 1L,
	dispidTargetObjectProperty = 2L,
	dispidRespondToSelectionEvents = 3L,
	dispidExternalSort = 4L,
	dispidDoFocus = 5L,
	eventidNewMethod = 1L,
	eventidEditValue = 2L,
	eventidUserMessage = 3L,
	eventidKeyPressed = 4L,
	eventidVKeyUp = 5L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TVLISTCTL_H__EF7D6583_0161_11D2_88BE_00C04FC32EF3__INCLUDED)
