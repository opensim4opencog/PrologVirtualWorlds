// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_EDLSTCTL_H__BD73CC91_0D29_11D2_88C3_00C04FC32EF3__INCLUDED_)
#define AFX_EDLSTCTL_H__BD73CC91_0D29_11D2_88C3_00C04FC32EF3__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// EdLstCtl.h : header file
//

#include <afxcmn.h>

class CPropEdCtrl;
class CLPEdit;
class CBoolDrop;
class CLaunchEditorButton;

/////////////////////////////////////////////////////////////////////////////
// CEditorListCtrl window

class CEditorListCtrl : public CListCtrl
{
// Construction
public:
	CEditorListCtrl();

	void CancelEditing();

	void LaunchExternalEditor();

	BOOL GetItemAndColumnFromPoint(POINT point, int &iItem, int &iColumn);

	void ClearSelections();
	void SelectItem(int iItem, int iColumn = 0, BOOL bClearAll = TRUE);
	virtual int  FindItemByName(CString strName);

	// OVERRIDEN FUNCTIONS
	BOOL DeleteItem(int nItem);
	BOOL DeleteAllItems();
	DWORD GetItemData( int nItem );

// Attributes
public:
	CPropEdCtrl*	m_pOCX;
	int				m_iEditingItem;
	int				m_iColumnCount;
	BOOL			m_bEditingName;
	BOOL			m_bInClick;

// Operations
public:
	virtual void DoneEditing(CString strNewValue);
	virtual BOOL EditName(int nItem, CString strValue, int iColumn = 0);
	virtual BOOL EditValue(int nItem, CString strValue);
	virtual BOOL EditValue(int nItem, BOOL bValue);
	virtual void ShowButton(int nItem);
	virtual void DeleteButton();
	BOOL ShowFirstSelectedItem();
	BOOL ShowItemAndColumn(int iItem, int iColumn, RECT &rect);
	BOOL ShowItemAndColumn(int iItem, int iColumn);

	virtual void CommitEditors();
	virtual void DeleteEditors();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditorListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditorListCtrl();

protected:

	CLPEdit *m_pStringEditor;
	CBoolDrop *m_pBoolEditor;
	CLaunchEditorButton *m_pButton;
	int		m_iClickedItem;
	int		m_iClickedColumn;
	BOOL	m_bTimerSet;

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditorListCtrl)
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDLSTCTL_H__BD73CC91_0D29_11D2_88C3_00C04FC32EF3__INCLUDED_)
