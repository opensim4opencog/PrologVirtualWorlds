// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_TOOLDLG_H__0FC3136A_86FA_11D1_84E8_00C04FB6A5C8__INCLUDED_)
#define AFX_TOOLDLG_H__0FC3136A_86FA_11D1_84E8_00C04FB6A5C8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ToolDlg.h : header file
//
#include <afxcmn.h>
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CToolDialog dialog

class CToolDialog : public CDialog //Bar
{
// Construction
public:
	BOOL OnUpdateUserInterfaceZoomLevel(IVWRenderRoot* pVWRenderRoot);
	void EnableZoom(BOOL bEnable);
	BOOL OnUpdateUserInterface(IVWControlManager* pCtrlManager, IVWRenderRoot* pVWRenderRoot, BOOL bCollisionToggle, BOOL bGravityToggle);
	BOOL OnUpdateUserInterfaceGeometryInfo(IVWControlManager* m_pControlManager, VARIANT varHint);
	BOOL CheckButton( int nID, BOOL bCheck = TRUE );
	CToolDialog(CWnd* pParent = NULL);   // standard constructor
	~CToolDialog();

	BOOL AddWindowTool(UINT nControlID);

	BOOL InitDialogBar();
	void EnableButton(UINT nControlID, BOOL bEnable);

// Dialog Data
	//{{AFX_DATA(CToolDialog)
	enum { IDD = IDD_DIALOGBAR };
	CEdit	m_ebZ;
	CEdit	m_ebY;
	CEdit	m_ebX;
	CComboBox	m_cbSelected;
	//}}AFX_DATA


	CToolTipCtrl m_ToolTip;
	IVWRenderRoot* m_pVWRenderRoot;

	//Bitmaps for buttons
	HICON m_hiHelp, m_hiNavigate, /*m_hiRedo, m_hiRedoGrey,*/ m_hiCollisionToggleUp, m_hiGravityToggleUp,
		  m_hiCollisionToggleDown, m_hiGravityToggleDown,
		  m_hiRotate, m_hiScale, m_hiSelect, m_hiTranslate; /*, m_hiUndo, m_hiUndoGrey*/  //m_hiZoomIn,
			//m_hiZoomInGrey, m_hiZoomOut, m_hiZoomOutGrey;
	
	HBITMAP m_hbSelectDialog;

	HBRUSH m_bkBrush;

	CBitmapButton m_bbUndo, m_bbRedo, m_bbNavigate, m_bbSelectContents,	m_bbHelp, m_bbSelect, 
		          m_bbTranslate, m_bbRotate, m_bbScale, m_bbCollisionToggle, m_bbGravityToggle,
				  m_bbZoomIn, m_bbZoomOut;

	BOOL m_bCollision, m_bGravity;
	CBitmap m_bmCollisionUp;    // normal image 
	CBitmap m_bmCollisionDown;  // selected image 
	CBitmap m_bmGravityUp;      // normal image 
	CBitmap m_bmGravityDown;    // selected image 

	BOOL bDialogInitialized;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeSelected();
	afx_msg void OnKillfocusPosition();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLDLG_H__0FC3136A_86FA_11D1_84E8_00C04FB6A5C8__INCLUDED_)
