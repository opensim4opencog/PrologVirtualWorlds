// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// ServerV2.h : main header file for the SERVERV2 application
//
#include <afxtempl.h>

// V-Worlds optimized templates
#include <vwtempl.h>

#include <PropBase.h>

#if !defined(AFX_SERVERV2_H__7FDB7AD9_1C36_11D2_8ADD_00104BC911D4__INCLUDED_)
#define AFX_SERVERV2_H__7FDB7AD9_1C36_11D2_8ADD_00104BC911D4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "servsink.h"
#include "servdoc.h"
#include "mainfrm.h"

/////////////////////////////////////////////////////////////////////////////
// CServerV2App:
// See ServerV2.cpp for the implementation of this class
//

class CServerV2App : public CWinApp
{	
public:
	CServerV2App();
	~CServerV2App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerV2App)
	public:
	virtual BOOL InitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	virtual BOOL InitApplication();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CServerV2App)
	afx_msg void OnAppAbout();
	afx_msg void OnToolsSetdsurl();
	afx_msg void OnFileOpen();
	afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnFileNew();
	afx_msg void OnFileImport();
	afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileImport(CCmdUI* pCmdUI);
    afx_msg void OnClearPerf();
	afx_msg void OnToolsOptions();
	afx_msg void OnUpdateToolsOptions(CCmdUI* pCmdUI);
	afx_msg	void OnFileSaveLog();
	afx_msg	void OnUpdateFileSaveLog(CCmdUI* pCmdUI);
	afx_msg	void OnToolsUserLimits();
	afx_msg	void OnUpdateToolsUserLimits(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnFocus(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

//Custom stuff below.	
public:	
	BOOL m_bAppend;
	HRESULT HandleOnAvailableWorld(BSTR bstrName, int iNoUsers);
	HRESULT HandleOnChangeName(BSTR bstrName, BSTR bstrNewName, int iNoUsers);
	HRESULT HandleOnReport(CString bstrWorldName, CString bstrName, int iType);
	HRESULT HandleOnRemove(CString strName)
	{
		CServerV2Doc * pDoc;
		if(m_MapOfDocs.Lookup(strName,pDoc))
		{
			if(!m_MapOfDocs.RemoveKey(strName))
			{
				ASSERT(0);
				TRACE("Server:HandleOnRemove had a problem removing key %s.\n",strName);
			}
		}
		else
		{
			TRACE("Server:HandleOnRemove already deleted %s, this must be the destructor.\n",strName);
		}
		return S_OK;
	}

	void TryToFocusOnAWorld(void);
	void TryToFocusOnAWorld(CServerV2Doc *pDoc);

	CServerV2Doc *FindWorld(CString str) 
	{
		CServerV2Doc *pDoc; 
		if(!m_MapOfDocs.Lookup(str, pDoc)) 
			return NULL; 
		else return pDoc;
	}

	//Imglist used to initialize listctrl's throughout app. Saves resources by having only one instance.
	CImageList imglist; 
	
	HRESULT Terminate();

	IVWServer *	m_pServer;

	CMainFrame *m_pMainFrame;	

	VARIANT_BOOL m_bConnectedToDS;

	long m_iCurrentTasks;

	BOOL m_bShutdown;

private:
	//methods.
	HRESULT CreateServer(void);
	
	BOOL FirstInstance();

	void RegisterWindowClass();

	void InProgress(CString string);

	void Done(void);

	// Data members
	
	DWORD m_ServerSinkCookie;
	
	CVWMap<CString, LPCTSTR, CServerV2Doc *,  CServerV2Doc *&> m_MapOfDocs;

	CServerV2Doc * m_pdocServerScope;
	
};

class AppCriticalSection
{
public:

	AppCriticalSection(CServerV2App *pApp) : m_pApp(pApp) 
	{
		InterlockedIncrement(&m_pApp->m_iCurrentTasks);
	}

	~AppCriticalSection() 
	{
		InterlockedDecrement(&m_pApp->m_iCurrentTasks);
	}

	CServerV2App *m_pApp;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERV2_H__7FDB7AD9_1C36_11D2_8ADD_00104BC911D4__INCLUDED_)
