// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_SOCXCTL_H__0E5D5954_020F_11D1_83AB_00C04FB6FA46__INCLUDED_)
#define AFX_SOCXCTL_H__0E5D5954_020F_11D1_83AB_00C04FB6FA46__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "..\vwview\viewctl.h"
#include "cutils.h"
#include <scripts.h>

// socxctl.h : Declaration of the CScriptCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CScriptCtrl : See socxctl.cpp for implementation.

class CScriptCtrl : public CVWViewCtrl
{
	DECLARE_DYNCREATE(CScriptCtrl)

// Constructor
public:
	CScriptCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScriptCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

	CScriptSite m_ScriptSite;
	CComBSTR m_bstrScriptPath;
	long m_TimeOut;
	long m_inScript, m_bDelayedRefresh;

// Implementation
protected:
	~CScriptCtrl();

	void RefreshScript();
	HRESULT LoadScript();

	virtual HRESULT HandleOnConnect(IDispatch* pWorld);
	virtual HRESULT HandleOnDisconnect(IDispatch* pWorld);
	
	virtual HRESULT HandleOnUserConnect(IThing* pthing, BOOL bFromClient);
	virtual HRESULT HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient);
	virtual HRESULT HandleOnUserReconnect(IVWClient* newClient,
			IWorld* pWorld,
			IThing* pthing);


	DECLARE_OLECREATE_EX(CScriptCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CScriptCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CScriptCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CScriptCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CScriptCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CScriptCtrl)
	afx_msg LPDISPATCH GetVWClient();
	afx_msg void SetVWClient(LPDISPATCH newValue);
	afx_msg BSTR GetScriptURL();
	afx_msg void SetScriptURL(LPCTSTR lpszNewValue);
	afx_msg long GetTimeOut();
	afx_msg void SetTimeOut(long nNewValue);
	afx_msg SCODE Invoke(LPCTSTR bstrScriptText);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	void FireOnError(long errCode, LPCTSTR msg)
		{FireEvent(eventidOnError,EVENT_PARAM(VTS_I4 VTS_BSTR), errCode, msg);}
// Event maps
	//{{AFX_EVENT(CScriptCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CScriptCtrl)
	dispidVWClient = 1L,
	dispidScriptURL = 2L,
	dispidTimeOut = 3L,
	dispidInvoke = 4L,

	eventidOnError = 1L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCXCTL_H__0E5D5954_020F_11D1_83AB_00C04FB6FA46__INCLUDED)
