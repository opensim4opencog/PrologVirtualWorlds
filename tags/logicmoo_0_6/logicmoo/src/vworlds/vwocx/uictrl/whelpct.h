// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_WHELPCT_H__AF0AD5D9_418B_11D1_BB2B_00C04FC2CAB1__INCLUDED_)
#define AFX_WHELPCT_H__AF0AD5D9_418B_11D1_BB2B_00C04FC2CAB1__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "..\vwview\viewctl.h"

#include "vwmenu.h"

// whelpCt.h : Declaration of the CWebBrowserHelperCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CWebBrowserHelperCtrl : See whelpCt.cpp for implementation.

class CWebBrowserHelperCtrl : public CVWViewCtrl
{
	DECLARE_DYNCREATE(CWebBrowserHelperCtrl)

// Constructor
public:
	CWebBrowserHelperCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebBrowserHelperCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CWebBrowserHelperCtrl();

	DECLARE_OLECREATE_EX(CWebBrowserHelperCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CWebBrowserHelperCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CWebBrowserHelperCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CWebBrowserHelperCtrl)		// Type name and misc status

	BOOL IsInvokeAllowed(DISPID);

// Message maps
	//{{AFX_MSG(CWebBrowserHelperCtrl)
	//}}AFX_MSG
	afx_msg void OnCommandRange(UINT nID);
	afx_msg void OnCommandRangeUI(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CWebBrowserHelperCtrl)
	afx_msg void HideBrowserAdornments();
	afx_msg void RestoreBrowserAdornments();
	afx_msg BSTR FetchURLText(LPCTSTR bstrURL);
	afx_msg BSTR PostForm(LPCTSTR URL, LPCTSTR Data, BOOL bFilter);
	afx_msg void TrackThingMenu(LPDISPATCH pThing);
	afx_msg LPDISPATCH GetSubFolderNames(LPCTSTR strRootPath);
	afx_msg LPDISPATCH GetFileNames(LPCTSTR strRootPath, LPCTSTR strWildCard);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CWebBrowserHelperCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CWebBrowserHelperCtrl)
	dispidHideBrowserAdornments = 1L,
	dispidRestoreBrowserAdornments = 2L,
	dispidFetchURLText = 3L,
	dispidPostForm = 4L,
	dispidTrackThingMenu = 5L,
	dispidGetSubFolderNames = 6L,
	dispidGetFileNames = 7L,
	//}}AFX_DISP_ID
	};

protected:
	CVWMenu			m_Menu;
	int				m_iSavedToolBar;
	VARIANT_BOOL	m_bSavedMenuBar;
	BOOL			m_bStatusSaved;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WHELPCT_H__AF0AD5D9_418B_11D1_BB2B_00C04FC2CAB1__INCLUDED_)
