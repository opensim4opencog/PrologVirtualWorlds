// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SEngine.h"

#include "MainFrm.h"
//#include "TEView.h"
#include "DBGView.h"
#include "ChildFrm.h"
#include "OCXView.h"

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
	ON_COMMAND(ID_WINDOW_COMMANDWINDOW, OnWindowCommandwindow)
	ON_COMMAND(ID_WINDOW_NEWOCXWINDOW, OnWindowNewocxwindow)
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
	ON_COMMAND(ID_SCRIPT_TESTMULTITHREADEDVB, OnScriptTestmultithreadedvb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_ROWCOL,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
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
	
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

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
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
//	cs.style |= WS_MAXIMIZE;

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


void CMainFrame::OnWindowCommandwindow() 
{
	// TODO: Add your command handler code here
	
	

	CMDIChildWnd* pActiveChild = MDIGetActive();
	CDocument* pDocument;
	if (pActiveChild == NULL ||
	  (pDocument = pActiveChild->GetActiveDocument()) == NULL)
	{
		TRACE0("Warning: No active document for WindowNew command.\n");
		AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
		return;     // command failed
	}
	CView* pView = NULL;
	POSITION pos = pDocument->GetFirstViewPosition();
	while (pos != NULL && 
		   !pView)
	{
		CView * pCurrentView = pDocument->GetNextView(pos);
		if ( pCurrentView->IsKindOf(RUNTIME_CLASS( CDBGView ) )) 
			pView = pCurrentView;
	} 
	if ( pView )
	{
		pView->ShowWindow( SW_SHOWNORMAL );
		pView->SetFocus();
		return;
	}

	// otherwise we have a new frame !
	CDocTemplate* pTemplate = pDocument->GetDocTemplate();
	ASSERT_VALID(pTemplate);
	//CFrameWnd* pFrame = pTemplate->CreateNewFrame(pDocument, pActiveChild);
	CCreateContext context;
	context.m_pCurrentFrame = pActiveChild;
	context.m_pCurrentDoc = pDocument;
	context.m_pNewViewClass = RUNTIME_CLASS( CDBGView );
	context.m_pNewDocTemplate = pTemplate;

	
	CFrameWnd* pFrame = new CChildFrame; // custom MDI child frame
		
	ASSERT_KINDOF(CFrameWnd, pFrame);

	if (context.m_pNewViewClass == NULL)
			TRACE0("Warning: creating frame with no default view.\n");

		// create new from resource
	if (!pFrame->LoadFrame(IDR_MAINFRAME,
			WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,   // default frame styles
			this, &context))
	{
			TRACE0("Warning: CDocTemplate couldn't create a frame.\n");
			// frame will be deleted in PostNcDestroy cleanup
			
	}

	if (pFrame == NULL)
	{
		TRACE0("Warning: failed to create new frame.\n");
		return;     // command failed
	}

	pTemplate->InitialUpdateFrame(pFrame, pDocument);

}

void CMainFrame::OnWindowNewocxwindow() 
{
	// TODO: Add your command handler code here
	
	

	CMDIChildWnd* pActiveChild = MDIGetActive();
	CDocument* pDocument;
	if (pActiveChild == NULL ||
	  (pDocument = pActiveChild->GetActiveDocument()) == NULL)
	{
		TRACE0("Warning: No active document for WindowNew command.\n");
		AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
		return;     // command failed
	}
	CView* pView = NULL;

	CDocTemplate* pTemplate = pDocument->GetDocTemplate();
	ASSERT_VALID(pTemplate);
	//CFrameWnd* pFrame = pTemplate->CreateNewFrame(pDocument, pActiveChild);
	CCreateContext context;
	context.m_pCurrentFrame = pActiveChild;
	context.m_pCurrentDoc = pDocument;
	context.m_pNewViewClass = RUNTIME_CLASS( COCXView );
	context.m_pNewDocTemplate = pTemplate;

	
	CFrameWnd* pFrame = new CChildFrame; // custom MDI child frame
		
	ASSERT_KINDOF(CFrameWnd, pFrame);

	if (context.m_pNewViewClass == NULL)
			TRACE0("Warning: creating frame with no default view.\n");

		// create new from resource
	if (!pFrame->LoadFrame(IDR_MAINFRAME,
			WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,   // default frame styles
			this, &context))
	{
			TRACE0("Warning: CDocTemplate couldn't create a frame.\n");
			// frame will be deleted in PostNcDestroy cleanup
			
	}

	if (pFrame == NULL)
	{
		TRACE0("Warning: failed to create new frame.\n");
		return;     // command failed
	}

	pTemplate->InitialUpdateFrame(pFrame, pDocument);
	pView = pFrame->GetActiveView();
	ASSERT ( pView );
	ASSERT ( pView->IsKindOf( RUNTIME_CLASS( COCXView)));
	PostMessage( WM_COMMAND, ID_ACTIVEXCONTROLS_SETCONTROL );

}

BOOL CMainFrame::OnQueryNewPalette() 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bRet = CMDIFrameWnd::OnQueryNewPalette();
	SendMessageToDescendants( WM_QUERYNEWPALETTE );
	return bRet;
}

void CMainFrame::OnPaletteChanged(CWnd* pFocusWnd) 
{
	CMDIFrameWnd::OnPaletteChanged(pFocusWnd);
	
	// TODO: Add your message handler code here
	SendMessageToDescendants( WM_PALETTECHANGED, (WPARAM) pFocusWnd->m_hWnd );	
}



UINT CMainFrame::Thread2( void *pDlg )
{
	((CMainFrame*)pDlg)->parse2();
	return 0;
}

void CMainFrame::parse2()
{
	/*
	HRESULT hr;
	int err;
	site.AddNamedItem(CString("document"), NULL, NULL);
	hr=site.LoadScript(CString("d:\\projects\\worlds96\\threadvb\\test.scr"),CString("document"));

	for(long i=0;i<10000000;i++)
	{
		hr=site.InvokeByName(CString("test"),CString("document"));
		if (hr != S_OK )
		{
			err = site.GetLastError();
			if(err==E_RUNTIME)
			{
				CString strError = site.strerror( err );
#ifdef _DEBUG
                                afxDump<<strError;
#endif
			}
		}	

		ASSERT(SUCCEEDED(hr));
	}
	*/
}


void CMainFrame::OnScriptTestmultithreadedvb() 
{
/*
	HRESULT hr;
	site.AddNamedItem(CString("document"), NULL, NULL);
	hr=site.LoadScript(CString("d:\\projects\\worlds96\\threadvb\\test.scr"),CString("document"));

	ASSERT(SUCCEEDED(hr));
	CWinThread* m_pthreadComm = AfxBeginThread( CMainFrame::Thread2, this, THREAD_PRIORITY_NORMAL );
	ASSERT(m_pthreadComm);
	for (long i=0;i<10000000;i++)
	{
		hr=site.InvokeByName(CString("test"),CString("document"));
		if (hr != S_OK )
		{
			int err = site.GetLastError();
			if(err==E_RUNTIME)
			{
				CString strError = site.strerror( err );
			}
		}	
		ASSERT(SUCCEEDED(hr));
	}
	*/
}
