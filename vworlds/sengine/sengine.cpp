// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// SEngine.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SEngine.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "SEngDoc.h"
#include "SEngView.h"
//#include "TEView.h"
#include <fstream.h>

CComModule _Module;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CCustomCommandLineInfo : public CCommandLineInfo
{
public:
	int m_bExitOnFinish;
	int m_bNoWindow;
	int m_bDoCommand;
	int m_bErrorFile;
	
	CString m_strCommand;
	CString m_strFileName;

	CCustomCommandLineInfo() : CCommandLineInfo()
	{
		m_bExitOnFinish = FALSE;
		m_bNoWindow = FALSE;
		m_bDoCommand = FALSE;
		m_bErrorFile = FALSE;
	};
	virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast)
	{
		CCommandLineInfo::ParseParam(pszParam, bFlag, bLast );		
		if (bFlag)
		{
			USES_CONVERSION;
			LocalParseParamFlag(T2CA(pszParam));
		}
		else
			LocalParseParamNotFlag(pszParam);

	
	}

	void LocalParseParamFlag(const char* pszParam)
	{
		if (lstrcmpA(pszParam, "nowindow") == 0)
		{
			m_bExitOnFinish = m_bNoWindow = TRUE;
		}
		else if (lstrcmpA(pszParam, "exit") == 0)
			m_bExitOnFinish = TRUE;
		else if (lstrcmpA(pszParam, "c") == 0)
			m_bDoCommand = TRUE;
		else if (lstrcmpA(pszParam, "f") == 0)
			m_bErrorFile = TRUE;
	}

	void LocalParseParamNotFlag(const TCHAR* pszParam)
	{
		if (m_bDoCommand && 
			(m_strCommand.GetLength() == 0)) 
		{
			m_strCommand = pszParam;
		}
		else
		if (m_bErrorFile &&
			(m_strFileName.GetLength() == 0 ))
		{
			m_strFileName = pszParam;
		}
		
	}

};

/////////////////////////////////////////////////////////////////////////////
// CSEngineApp

BEGIN_MESSAGE_MAP(CSEngineApp, CWinApp)
	//{{AFX_MSG_MAP(CSEngineApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSEngineApp construction

CSEngineApp::CSEngineApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSEngineApp object

CSEngineApp theApp;

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {2C06A6F0-D0C2-11CF-8A96-00A0C9082583}
static const CLSID clsid =
{ 0x2c06a6f0, 0xd0c2, 0x11cf, { 0x8a, 0x96, 0x0, 0xa0, 0xc9, 0x8, 0x25, 0x83 } };

/////////////////////////////////////////////////////////////////////////////
// CSEngineApp initialization

BOOL CSEngineApp::InitInstance()
{
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_SENGINTYPE,
		RUNTIME_CLASS(CSEngineDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CSEngineView));
//		RUNTIME_CLASS(CTextEntryView));
	AddDocTemplate(pDocTemplate);

	// Connect the COleTemplateServer to the document template.
	//  The COleTemplateServer creates new documents on behalf
	//  of requesting OLE containers by using information
	//  specified in the document template.
	m_server.ConnectTemplate(clsid, pDocTemplate, FALSE);

	// Register all OLE server factories as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleTemplateServer::RegisterAll();
		// Note: MDI applications register all server objects without regard
		//  to the /Embedding or /Automation on the command line.

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCustomCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Check to see if launched as OLE server
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Application was run with /Embedding or /Automation.  Don't show the
		//  main window in this case.
		return TRUE;
	}

	// When a server application is launched stand-alone, it is a good idea
	//  to update the system registry in case it has been damaged.
	m_server.UpdateRegistry(OAT_DISPATCH_OBJECT);
	COleObjectFactory::UpdateRegistryAll();

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	if ( !cmdInfo.m_bNoWindow )
	{
		pMainFrame->ShowWindow(m_nCmdShow);
		pMainFrame->UpdateWindow();
	}


	return TRUE;
}

int CSEngineApp::Run()
{

	// Re-Parse command line for standard shell commands, DDE, file open
	CCustomCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	int retval = TRUE;

	if ( cmdInfo.m_bDoCommand )
	{
		// Find the document... Window ID's won't help
		POSITION pos = GetFirstDocTemplatePosition( );
		while ( pos )
		{
			CDocTemplate *pTemplate = GetNextDocTemplate( pos );
			ASSERT( pTemplate );
			{
				POSITION pos = pTemplate->GetFirstDocPosition( );
				while ( pos )
				{
					CSEngineDoc * pDoc= (CSEngineDoc *) pTemplate->GetNextDoc( pos );
					ASSERT( pDoc );
					
					CString ErrorMsg;
					pDoc->AddScriptlet(cmdInfo.m_strCommand, TRUE, &ErrorMsg);
					if ( ErrorMsg.GetLength() && cmdInfo.m_bNoWindow )
					{
						if ( cmdInfo.m_bErrorFile )
						{
							fstream fout( cmdInfo.m_strFileName,ios::app | ios::ate |ios::out );
							fout << ErrorMsg + "\n";
						}
						retval = FALSE;

					}
				
				}
			}
		}
	}
	if ( cmdInfo.m_bExitOnFinish )
	{
		// Note this still quits w/ some memory leaks....
		PostQuitMessage( retval );
		CWinApp::Run();
		ExitInstance();
		CloseAllDocuments( TRUE );
		return retval;
	}
	
	return CWinApp::Run();
}
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CSEngineApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CSEngineApp commands
