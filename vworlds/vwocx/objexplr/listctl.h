// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_MLISTCTL_H__EF7D6592_0161_11D2_88BE_00C04FC32EF3__INCLUDED_)
#define AFX_MLISTCTL_H__EF7D6592_0161_11D2_88BE_00C04FC32EF3__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// MListCtl.h : Declaration of the CMainListCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CMainListCtrl : See MListCtl.cpp for implementation.

class CMainListCtrl : public COleControl
{
	DECLARE_DYNCREATE(CMainListCtrl)

// Constructor
public:
	CMainListCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainListCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CMainListCtrl();

	DECLARE_OLECREATE_EX(CMainListCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CMainListCtrl)      // GetTypeInfo
	DECLARE_OLECTLTYPE(CMainListCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CMainListCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CMainListCtrl)
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CMainListCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CMainListCtrl)
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MLISTCTL_H__EF7D6592_0161_11D2_88BE_00C04FC32EF3__INCLUDED)
