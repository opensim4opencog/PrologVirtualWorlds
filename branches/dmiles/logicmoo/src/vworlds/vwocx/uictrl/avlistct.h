// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_AVLISTCT_H__AF0AD5D9_418B_11D1_BB2B_00C04FC2CAB1__INCLUDED_)
#define AFX_AVLISTCT_H__AF0AD5D9_418B_11D1_BB2B_00C04FC2CAB1__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "..\vwview\viewctl.h"

// AvListCt.h : Declaration of the CAvatarListCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CAvatarListCtrl : See AvListCt.cpp for implementation.

class CAvatarListCtrl : public CVWViewCtrl
{
	DECLARE_DYNCREATE(CAvatarListCtrl)

// Constructor
public:
	CAvatarListCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAvatarListCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CAvatarListCtrl();

	DECLARE_OLECREATE_EX(CAvatarListCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CAvatarListCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CAvatarListCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CAvatarListCtrl)		// Type name and misc status


// Message maps
	//{{AFX_MSG(CAvatarListCtrl)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CAvatarListCtrl)
	afx_msg LPDISPATCH GetVWClient();
	afx_msg void SetVWClient(LPDISPATCH newValue);
	afx_msg LPDISPATCH GetCurrentUser();
	afx_msg void SetCurrentUser(LPDISPATCH newValue);
	afx_msg VARIANT GetCostumeList();
	afx_msg void SetCostumeList(const VARIANT FAR& newValue);
	afx_msg VARIANT GetAvatarList();
	afx_msg void SetAvatarList(const VARIANT FAR& newValue);
	afx_msg BOOL CreateNewAvatar(LPCTSTR bstrAvatarName, LPCTSTR bstrAvatarPassword);
	afx_msg BOOL ConnectAvatar(LPCTSTR strAvatarName, LPCTSTR bstrAvatarPassword);
	afx_msg void DisconnectAvatar();
	afx_msg BOOL DeleteAvatar();
	afx_msg BOOL DeleteDeadAvatar(LPCTSTR strAvatarName);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CAvatarListCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CAvatarListCtrl)
	dispidVWClient = 1L,
	dispidCurrentUser = 2L,
	dispidCostumeList = 3L,
	dispidAvatarList = 4L,
	dispidCreateNewAvatar = 5L,
	dispidConnectAvatar = 6L,
	dispidDisconnectAvatar = 7L,
	dispidDeleteAvatar = 8L,
	dispidDeleteDeadAvatar = 9L,
	//}}AFX_DISP_ID
	};

protected:
	IWorld		*m_pLocalWorld;
	BOOL		m_bConnected;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AVLISTCT_H__AF0AD5D9_418B_11D1_BB2B_00C04FC2CAB1__INCLUDED)
