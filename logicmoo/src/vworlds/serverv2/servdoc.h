// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// ServerV2Doc.h : interface of the CServerV2Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERV2DOC_H__7FDB7AE1_1C36_11D2_8ADD_00104BC911D4__INCLUDED_)
#define AFX_SERVERV2DOC_H__7FDB7AE1_1C36_11D2_8ADD_00104BC911D4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "prgdlg.h"
#include "..\vwcommon\global.h"

UINT CALLBACK MyOFNHookProc(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);

void SaveLogHelper(CDocument * pDoc, CString cstrDefaultFileName);

//This is represents each world.
class CServerV2Doc : public CDocument
{
protected: // create from serialization only
	CServerV2Doc();
	DECLARE_DYNCREATE(CServerV2Doc)

// Operations
public:
	HRESULT HandleOnAvailableWorld(CString strName, int iNoUsers);
	HRESULT CheckIfOnline();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerV2Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	
	virtual ~CServerV2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	HRESULT HandleOnReport(CString string, int iType);
	void InProgress(CString string);
	void Done(void);
	void Done(CString);
	void SetFocus(void);

	CString m_strNameOfWorld;	

private:
	CString m_strWorldInfoURL;	
	CServerV2App *m_pApp;
	BOOL m_bOffline;
	int m_iNoUsers;

	// Generated message map functions
protected:
	//{{AFX_MSG(CServerV2Doc)
	afx_msg void OnUpdateSaveworld(CCmdUI* pCmdUI);
	afx_msg void OnSaveworld();
	afx_msg void OnUpdateExportScript(CCmdUI* pCmdUI);
	afx_msg void OnExportScript();
	afx_msg void OnDeleteworldmenu();
	afx_msg void OnUpdateDeleteworldmenu(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTakeWorldOffline(CCmdUI* pCmdUI);
	afx_msg void OnTakeWorldOffline();
	afx_msg void OnToolsShowWorldInDS();
	afx_msg void OnUpdateToolsShowWorldInDS(CCmdUI* pCmdUI);
	afx_msg void OnToolsSetInfoURL();
	afx_msg void OnFileSaveworldas();
	afx_msg void OnUpdateFileSaveworldas(CCmdUI* pCmdUI);
	afx_msg void OnFileExportworldas();
	afx_msg void OnUpdateFileExportworldas(CCmdUI* pCmdUI);
	afx_msg void OnTestPerformancetest1();
	afx_msg void OnUpdateTestPerformancetest1(CCmdUI* pCmdUI);
	afx_msg	void OnFileSaveLog();
	afx_msg	void OnUpdateFileSaveLog(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERV2DOC_H__7FDB7AE1_1C36_11D2_8ADD_00104BC911D4__INCLUDED_)

