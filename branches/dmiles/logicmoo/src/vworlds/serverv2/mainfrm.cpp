// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// MainFrm.cpp : implementation of the CMainFrame class
//
#include "stdafx.h"
#include "ServerV2.h"
#include <global.h>
#include "reghelp.h"
#include <io.h>
#include <htmlhelp.h>
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
	ON_UPDATE_COMMAND_UI(ID_HELP_FINDER, OnUpdateHelpFinder)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)
	
	ON_MESSAGE(WM_ONAVAILABLEWORLD, OnAvailableWorld)
	ON_MESSAGE(WM_ONREPORT, OnReport)
	ON_MESSAGE(WM_ONFOCUS, OnFocus)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame() : m_ShutDownPhase(0)
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
/*	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
*/
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
/*	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
*/	
	return 0;
}

afx_msg LRESULT CMainFrame::OnFocus(WPARAM wParam, LPARAM lParam)
{
	m_pApp->TryToFocusOnAWorld((CServerV2Doc *)wParam);
	return S_OK;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.lpszClass = "VWS Server Uniqueness Window Class";

	return CMDIFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
afx_msg LRESULT CMainFrame::OnAvailableWorld(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = S_OK;
	BSTR temp	= (BSTR)wParam;
	hr = m_pApp->HandleOnAvailableWorld(temp, (int)lParam);
	
	::SysFreeString(temp);
	
	return hr;
}

afx_msg LRESULT CMainFrame::OnReport(WPARAM wParam, LPARAM lParam)
{
	HRESULT hr = S_OK;
	REPORTSTRUCT *pReportStruct = (REPORTSTRUCT *)wParam;
	hr = m_pApp->HandleOnReport(CString(pReportStruct->bstrWorldName), CString(pReportStruct->bstrMessage), 
		pReportStruct->lType);

	delete pReportStruct;

	return hr;
}

void CMainFrame::OnDestroy() 
{
	CMDIFrameWnd::OnDestroy();
}

void CMainFrame::OnClose() 
{
	if(m_pApp->m_iCurrentTasks>0)
	{
		AfxMessageBox("You have " + str(m_pApp->m_iCurrentTasks)+ " job(s) pending completion. Please wait for this to finish before closing.");
	}
	else
	{
		//disable close system menu.
		CMenu* pmenu = GetSystemMenu(FALSE);
		UINT size = pmenu->GetMenuItemCount( );
		pmenu->EnableMenuItem(pmenu->GetMenuItemID(6), MF_DISABLED | MF_GRAYED);
		DrawMenuBar();

		//instead of the 2 phase shutdown, i'm
		//just gonna risk leaking a little memory 
		//and simplify the code. markme saw a crash here.
		m_pApp->Terminate();
		
		//close the frame.
		CMDIFrameWnd::OnClose();

		//ALERT. Unusual code.
		//This is a 2phase shutdown to ensure we don't leak any memory due to floating msgs coming
		//in from the server.
		/*if(m_ShutDownPhase==0)
		{
			if(E_FAIL==m_pApp->Terminate())
			{
				//Terminate's already happened. Shutdown now.
				m_ShutDownPhase = 0;
				CMDIFrameWnd::OnClose();
				return;
			}
			m_ShutDownPhase=-23; //number same as below. number not used anywhere else.
			PostMessage(WM_CLOSE);
		}
		else if(m_ShutDownPhase==-23) //see number above.
		{
			m_ShutDownPhase = 0;
			CMDIFrameWnd::OnClose();
		}
		else
			ASSERT(0);
		*/
	}
}

BOOL CMainFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CMainFrame::OnHelpFinder() 
{
	//Look up path of html in registry.	
	CComBSTR bstrPath;
	HRESULT	hr;
	
	hr = FindHelpPath(&bstrPath.m_str);
	
	CString finalPath = "winhlp32.exe ";
	CString params = CString(bstrPath)+"vworlds.chm::/admin/StAdmin.htm";
	//mk:@MSITStore:C:\Program%20Files\Microsoft%20Virtual%20Worlds\docs\vworlds.chm::/admin/StAdmin.htm
	//Spawn IE.
	//HINSTANCE hInst = ShellExecute(NULL,NULL,finalPath,params,NULL,SW_SHOWNORMAL);
	//int ret = system("start iexplore \""+finalPath+"\"");
//	HtmlHelp(NULL, params, HH_DISPLAY_TOPIC, 0); 
	
	HtmlHelp(GetSafeHwnd(),params,HH_DISPLAY_TOPIC,0);

/*	if((int)hInst<=32)
	{
		AfxMessageBox("Error: Couldn't find the web page or unable to start Explorer.");
	}
	*/
}

void CMainFrame::OnUpdateHelpFinder(CCmdUI* pCmdUI) 
{
	//Look up path of html in registry.	
	CComBSTR bstrPath;
	HRESULT	hr;
	
	hr = FindHelpPath(&bstrPath.m_str);
	
	CString finalPath = CString(bstrPath)+"vworlds.chm";

	BOOL bExistsOnDisk = (_access(finalPath, 4) == 0) ? TRUE : FALSE;

	if(bExistsOnDisk)
		pCmdUI->SetText("&Help Topics");
	else
	{
		pCmdUI->Enable(FALSE);
		pCmdUI->SetText("&Help Topics (Help not installed)");
	}
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIFrameWnd::OnSize(nType, cx, cy);
	
	MDICascade();
}
