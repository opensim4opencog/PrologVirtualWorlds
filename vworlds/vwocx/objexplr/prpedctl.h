// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_PRPEDCTL_H__192FF35D_017B_11D2_88BE_00C04FC32EF3__INCLUDED_)
#define AFX_PRPEDCTL_H__192FF35D_017B_11D2_88BE_00C04FC32EF3__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <vwuiview.h>

#define NONSECURITYFAIL(Status) (((HRESULT)(Status)<0) && ((Status)!=VWOBJECT_E_PERMISSION_DENIED))
#define SECURITYFAIL(Status) (((HRESULT)(Status)<0) && ((Status)==VWOBJECT_E_PERMISSION_DENIED))

// PrpEdCtl.h : Declaration of the CPropEdCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CPropEdCtrl : See PrpEdCtl.cpp for implementation.

class CPropEdCtrl : public CVWUIView
{
	DECLARE_DYNCREATE(CPropEdCtrl)

// Constructor
public:
	CPropEdCtrl();

	void GetTypeFromIID(GUID lID, CString &strType);
	BOOL CheckIfParent(IThing *pParent, IThing *pChild);
	BOOL GetTypeAndValueFromVariant(VARIANT varItem, CString &strType, CString &strValue);
	BOOL GetTypeAndValueFromSecurity(IPropertySecurity *pSec, CString &strType, CString &strValue);
	BOOL GetTypeAndValueFromSecurity(IPropertySecurity *pSec, CString &strType, CString &strValue, VARTYPE &varType);
	virtual void DoubleClick(int iItem, int iColumn);
	virtual void LeftClick(int iItem, int iColumn);
	virtual void PopUpMenu(CPoint point);
	virtual void VirtualKeyPressed(UINT nChar);
	virtual void VirtualKeyUp(UINT nChar);
	virtual void AlphaKeyPressed(char cLetter);
	BOOL Editable(VARTYPE vt);
	BOOL EditableType(CString strType);
	virtual void DoneEditingName(int iItem, CString strNewValue);
	virtual void DoneEditingValue(int iItem, CString strNewValue);
	virtual void LaunchExternalEditor(int iItem);
	void ConvertStringToVariant(VARTYPE vt, CString strNewValue, CComVariant &newVarValue);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropEdCtrl)
	public:
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CPropEdCtrl();

	DECLARE_OLECREATE_EX(CPropEdCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CPropEdCtrl)      // GetTypeInfo
	DECLARE_OLECTLTYPE(CPropEdCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CPropEdCtrl)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CPropEdCtrl)
	afx_msg LPDISPATCH GetVWClient();
	afx_msg void SetVWClient(LPDISPATCH newValue);
	afx_msg LPDISPATCH GetTargetObjectProperty();
	afx_msg void SetTargetObjectProperty(LPDISPATCH newValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CPropEdCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CPropEdCtrl)
	dispidVWClient = 1L,
	dispidTargetObjectProperty = 2L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRPEDCTL_H__192FF35D_017B_11D2_88BE_00C04FC32EF3__INCLUDED)
