// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// ServerV2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ServerV2.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "ServDoc.h"
#include "ServView.h"
#include "globdoc.h"
#include "globEdVw.h"
#include "globfrm.h"
#include <process.h>
#include "vwsystem_i.c"
#include <io.h>
#include "..\vwcommon\global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerV2App

BEGIN_MESSAGE_MAP(CServerV2App, CWinApp)
	//{{AFX_MSG_MAP(CServerV2App)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_TOOLS_SETDSURL, OnToolsSetdsurl)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_IMPORT, OnFileImport)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_IMPORT, OnUpdateFileImport)
    ON_COMMAND(ID_CLEARPERF, OnClearPerf)
	ON_COMMAND(ID_TOOLS_SERVEROPTIONS, OnToolsOptions)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SERVEROPTIONS, OnUpdateToolsOptions)
	ON_COMMAND(ID_TOOLS_SETMAXUSERLIMIT, OnToolsUserLimits)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SETMAXUSERLIMIT, OnUpdateToolsOptions)
	ON_COMMAND(ID_FILE_SAVELOG, OnFileSaveLog)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVELOG, OnUpdateFileSaveLog)
	//}}AFX_MSG_MAP
	// Standard file based document commands
//	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerV2App construction

CServerV2App::CServerV2App() : m_pServer(NULL), m_iCurrentTasks(0), m_bShutdown(FALSE)
{
	::OleInitialize(0);
	m_bAppend = true;
}

HRESULT CServerV2App::Terminate()
{
	if(m_bShutdown)
		return E_FAIL;

	if (m_pdocServerScope)
		m_pdocServerScope->OnCloseDocument();

	m_bShutdown = TRUE;

	if(m_pServer) 
	{
		m_pServer->ShutDown();
		AtlUnadvise(m_pServer, IID_DVWServerEvents, m_ServerSinkCookie);
		m_pServer->Release();
		m_pServer = 0;
	}

	return S_OK;
}

CServerV2App::~CServerV2App()
{
	ASSERT(m_bShutdown);
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CServerV2App object

CServerV2App theApp;

/////////////////////////////////////////////////////////////////////////////
// CServerV2App initialization


BOOL CServerV2App::FirstInstance()
{
	 CWnd *PrevCWnd, *ChildCWnd;

	 // Determine if another window with our class name exists...
	 if (PrevCWnd = CWnd::FindWindow("VWS Server Uniqueness Window Class",NULL))
	 {
	   // if so, does it have any popups?
	   ChildCWnd=PrevCWnd->GetLastActivePopup();

	   // Bring the main window to the top.
	   PrevCWnd->BringWindowToTop();

	   // If iconic, restore the main window.
	   if (PrevCWnd->IsIconic())
		  PrevCWnd->ShowWindow(SW_RESTORE);

	   // If there was an active popup, bring it along too
	   if (PrevCWnd != ChildCWnd)
		  ChildCWnd->BringWindowToTop();

	   // Return FALSE. This isn't the first instance
	   // and you finished activating the previous one.
	   return FALSE;
	 }
	 else
	   return TRUE;  // First instance. Proceed as normal.
}

void CServerV2App::RegisterWindowClass()
{
	

	  // Register our unique class name that we wish to use
      WNDCLASS wndcls;
      memset(&wndcls, 0, sizeof(WNDCLASS));   // start with NULL
                                              // defaults
      wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
      wndcls.lpfnWndProc = ::DefWindowProc;
      wndcls.hInstance = AfxGetInstanceHandle();
      wndcls.hIcon = LoadIcon(IDR_MAINFRAME); 
	  // or load a different
      // icon
      wndcls.hCursor = LoadCursor( IDC_ARROW );
      wndcls.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
      wndcls.lpszMenuName = NULL;
      
	  // Specify our own class name for using FindWindow later
      wndcls.lpszClassName = _T("VWS Server Uniqueness Window Class");
      
	  // Register new class and exit if it fails
      if(!AfxRegisterClass(&wndcls))          
	  {
        TRACE("Class Registration Failed\n");           
		ASSERT(0);
	  } 
}

BOOL CServerV2App::InitInstance()
{
	AfxEnableControlContainer();

/*	//detect if exists another copy of app.
	HANDLE hndMutex = CreateMutex( NULL, // pointer to security attributes
		TRUE,  // flag for initial ownership
		"VWS Uniqueness Mutex" // pointer to mutex-object name
		);
	
	DWORD dwError = GetLastError();
	if(dwError == ERROR_ALREADY_EXISTS)
	{
		AfxMessageBox("Sorry, VWS is already running.");
		m_bShutdown = TRUE;
		return 0;
	}
*/
	if (!FirstInstance()) 
	{
		m_bShutdown = TRUE;
		return FALSE;
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_VWORLDTYPE,
		RUNTIME_CLASS(CServerV2Doc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CServerV2View));
	AddDocTemplate(pDocTemplate);

	CMultiDocTemplate* pDocTemplate2;
	pDocTemplate2 = new CMultiDocTemplate(
		IDR_GlobFrame,
		RUNTIME_CLASS(GlobDoc),
		RUNTIME_CLASS(GlobFrm), // custom MDI child frame
		RUNTIME_CLASS(CServerV2View));
	AddDocTemplate(pDocTemplate2);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;
	m_pMainFrame = pMainFrame;
	pMainFrame->m_pApp = this;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
//	if (!ProcessShellCommand(cmdInfo))
//		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	//Create VWorlds system.
	HRESULT hr = CreateServer();
	if(FAILED(hr))
	{
		AfxMessageBox("Server creation failed.  Please make sure all VWorlds components are registered properly.  Shutting down.");
		return FALSE;
	}

	//Set imglist.
	if(!imglist.Create(8,8 , FALSE, 1 , 1 ))
	{
		AfxMessageBox("Problem creating imagelist. BUG.");
	}
	imglist.SetBkColor(GetSysColor(COLOR_WINDOW));

	// ADD IMAGES TO THE IMAGE LIST
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP1);	//Arrow.
	imglist.Add(&bitmap, (COLORREF)0xFFFFFF);
	bitmap.DeleteObject();

	//Create server-scope document.
	POSITION curTemplatePos = GetFirstDocTemplatePosition();
	while(curTemplatePos != NULL)
	{
		CDocTemplate* curTemplate = 
			GetNextDocTemplate(curTemplatePos);
		CString str;
		curTemplate->GetDocString(str, CDocTemplate::docName);
		if(str == _T("Glob"))
		{			
			m_pdocServerScope = (CServerV2Doc *)curTemplate->OpenDocumentFile("Server Log and Statistics");
			ASSERT(m_pdocServerScope);
	
			m_pdocServerScope->HandleOnReport("Server is ready to receive users.", 0);
		}
	}

//	m_pMainFrame->MDITile(MDITILE_HORIZONTAL);
	m_pMainFrame->MDICascade();

	return TRUE;
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CServerV2App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CServerV2App commands
HRESULT CServerV2App::HandleOnAvailableWorld(BSTR bstrName, int iNoUsers)
{
	if(m_bShutdown) return E_FAIL;

	HRESULT hr = S_OK;
	CServerV2Doc * pDoc;

	CString temp = CString(bstrName);

	TRACE("Server: HandleOnAvailableWorld: %s, %i\n", temp, iNoUsers);

	if(m_MapOfDocs.Lookup(temp,pDoc))
	{
		pDoc->HandleOnAvailableWorld(temp, iNoUsers);
	}
	else
	{

		//Create document.
		//todo: reduce conversions.
		pDoc = (CServerV2Doc *)OpenDocumentFile(temp+".vwc");
		ASSERT(pDoc);
		m_MapOfDocs.SetAt(temp,pDoc);
		pDoc->HandleOnAvailableWorld(temp, iNoUsers);

		pDoc->HandleOnReport("World is ready to receive users.", 0);

	}

	return hr;
}

void CServerV2App::TryToFocusOnAWorld(CServerV2Doc *pDoc)
{
/*	ASSERT(pDoc);
	if(pDoc)
	{
		pDoc->SetFocus();
		TRACE("Focussing on: %s\n",pDoc->m_strNameOfWorld);
	}
*/
}

void CServerV2App::TryToFocusOnAWorld(void)
{
	POSITION pos = m_MapOfDocs.GetStartPosition();
	
	if(pos)
	{
		CString string;
		CServerV2Doc *pDoc = NULL; 
		
		m_MapOfDocs.GetNextAssoc( pos, string, pDoc );    

		ASSERT(pDoc);
		if(pDoc)
		{
			pDoc->SetFocus();
			TRACE("Focussing on: %s\n",pDoc->m_strNameOfWorld);
//			m_pMainFrame->PostMessage(WM_ONFOCUS,(UINT)pDoc,0);
		}
	}
}

HRESULT CServerV2App::HandleOnChangeName(BSTR bstrName, BSTR bstrNewName, int iNoUsers)
{
	if(m_bShutdown) return E_FAIL;

	return HandleOnAvailableWorld(bstrNewName, 0);
}

HRESULT CServerV2App::HandleOnReport(CString strWorldName, CString strMessage, int iType)
{
	if(m_bShutdown) return E_FAIL;

	HRESULT hr = S_OK;
	CServerV2Doc * pDoc = NULL;

	if(iType==C_CLOSEDOCUMENT)
	{
		//it's a close document request.
		BOOL bRet = m_MapOfDocs.Lookup(strMessage,pDoc);
		HandleOnRemove(strMessage);
		TryToFocusOnAWorld();
		if(pDoc && bRet)
		{
			pDoc->HandleOnReport(strMessage, C_CLOSEDOCUMENT);
			//pDoc->OnCloseDocument();			
		}
	}
	else
	{
		if(strWorldName=="")
		{
			m_pdocServerScope->HandleOnReport(strMessage, 0);
		}
		else
		{
			if(m_MapOfDocs.Lookup(strWorldName,pDoc))
			{
				pDoc->HandleOnReport(strMessage, iType);
			}
			else
			{		
				//m_pdocServerScope->HandleOnReport("VWS Error: Couldn't find world:"+strWorldName+". The message was: "+strMessage, 0);
				ASSERT(strWorldName!="");
				HandleOnAvailableWorld(CComBSTR(strWorldName), 0);
				if(m_MapOfDocs.Lookup(strWorldName,pDoc))
				{
					pDoc->HandleOnReport(strMessage, iType);
				}
				else
				{
					ASSERT(0);
	//				m_pdocServerScope->HandleOnReport("VWS Error: Couldn't find world:"+strWorldName+". The message was: "+strMessage, 0);
				}
			}
		}
	}

	return hr;
}

HRESULT CServerV2App::CreateServer()
{
	IVWServer *pServer = NULL;
	CComObject<CServerSink> *pServerSink = NULL;
	DWORD ServerSinkCookie;
	HRESULT hr;

	hr = CoCreateInstance(CLSID_VWServer, NULL, CLSCTX_INPROC_SERVER, IID_IVWServer, (void **) &pServer);
	if(FAILED(hr))
	{
		ASSERT(0);
		goto ERROR_ENCOUNTERED;
	}

	// create a helper object
	CComObject<CServerSink>::CreateInstance(&pServerSink);

	// tell it to sink events
	AtlAdvise(pServer, pServerSink->GetUnknown(), IID_DVWServerEvents, &ServerSinkCookie);

	// now call it to accept
	hr = pServer->Accept(2525, (int)(m_pMainFrame->m_hWnd));
	if(FAILED(hr))
	{
		ASSERT(0);
		goto ERROR_ENCOUNTERED;
	}

	pServerSink->SetApp(this);

	m_pServer = pServer;
	m_pServer->AddRef();
	m_ServerSinkCookie = ServerSinkCookie;

ERROR_ENCOUNTERED:
	if(pServer) pServer->Release();

	return hr;

}


//********************************************
// OnToolsSetdsurl
// Called when the user selects the Set 
// Directory Service URL menu item.  Should
// pull the old URL out of the registry and
// allow the user to edit it.

#include "reghelp.h"
#include "dsurl.h"

void CServerV2App::OnToolsSetdsurl() 
{
	CDlgDSURL	dlg;
	CComBSTR	bstrDSURL;
	HRESULT		hr;

	hr = GetDirectoryServiceURL(&bstrDSURL.m_str);
	if (SUCCEEDED(hr))
	{
		dlg.m_strURL = bstrDSURL;
	}
	else
		dlg.m_strURL = "";

	if (dlg.DoModal() == IDOK)
	{
		if (dlg.m_strURL.CompareNoCase("") == 0)
			dlg.m_strURL = "";

		bstrDSURL = dlg.m_strURL;
		SetDirectoryServiceURL(bstrDSURL);
		
		m_pServer->ReconnectToDS();

	}
}

void CServerV2App::InProgress(CString string)
{
	BSTR bstrStatus = NULL;

	if(m_pMainFrame)
		m_pMainFrame->m_wndStatusBar.SetWindowText(string+". "+CString(bstrStatus));
	if(m_pServer)
		m_pServer->GetSimpleStatus(&bstrStatus);

	::SysFreeString(bstrStatus);
}

void CServerV2App::Done(void)
{
	if(m_pMainFrame)
		m_pMainFrame->m_wndStatusBar.SetWindowText("The operation completed successfully.");
}


void CServerV2App::OnFileOpen() 
{
	AppCriticalSection critsec(this);

	HRESULT	hr;
	CComBSTR bstrPath;
	OPENFILENAME OpenFileName;
	char szFile[MAX_PATH]="\0";
	CString Path;
	CString strSelectedFile;
	char drive[MAX_PATH], dir[MAX_PATH], fname[MAX_PATH], ext[MAX_PATH];
	CString returnedDirectory, returnedExt;
	CComBSTR returnHR;
	CString strNameOfWorld;
	BSTR bstrStatus = NULL;
	
	//Present file open dialog for user.
	hr = FindWorldPath(&bstrPath.m_str);
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
	Path = CString(bstrPath);
	Path = Path.Left(Path.GetLength()-1);

	OpenFileName.lStructSize       = sizeof(OPENFILENAME);
    OpenFileName.hwndOwner         = m_pMainFrame->m_hWnd;
    OpenFileName.hInstance         = NULL;
    OpenFileName.lpstrFilter       = "Microsoft Virtual Worlds Databases\0*.vwc\0\0";
    OpenFileName.lpstrCustomFilter = NULL;
    OpenFileName.nMaxCustFilter    = 0;
    OpenFileName.nFilterIndex      = 0;
    OpenFileName.lpstrFile         = szFile;
    OpenFileName.nMaxFile          = sizeof(szFile);
    OpenFileName.lpstrFileTitle    = NULL;
    OpenFileName.nMaxFileTitle     = 0;
    OpenFileName.lpstrInitialDir   = Path;
    OpenFileName.lpstrTitle        = "Open";
    OpenFileName.nFileOffset       = 0;
    OpenFileName.nFileExtension    = 0;
    OpenFileName.lpstrDefExt       = NULL;
    OpenFileName.lCustData         = NULL;
	OpenFileName.lpfnHook 		   = NULL;
	OpenFileName.lpTemplateName    = NULL;
    OpenFileName.Flags             = OFN_EXPLORER | OFN_HIDEREADONLY;

	if (!GetOpenFileName(&OpenFileName))
		goto ERROR_ENCOUNTERED;

	//Make sure file selected is in the right directory.
	strSelectedFile = OpenFileName.lpstrFile;

	//ExtractDirectory.
	_splitpath(strSelectedFile, drive, dir, fname, ext);
	
	//ascertain that the directory is the same.
	returnedDirectory = CString(drive)+CString(dir);
	returnedDirectory = returnedDirectory.Left(returnedDirectory.GetLength()-1);
	returnedDirectory.MakeLower();
	if(Path.CompareNoCase(returnedDirectory)!=0)
	{
		AfxMessageBox("Sorry, all checkpoint files must be in the ~\\Microsoft Virtual Worlds\\Worlds directory.");
		goto ERROR_ENCOUNTERED;
	}

	//ascertain that the extension is the same.
	returnedExt = CString(ext);
	returnedExt.MakeLower();
	if(returnedExt!=".vwc")
	{
		AfxMessageBox("Sorry, all checkpoint files must have a .vwc extension.");
		goto ERROR_ENCOUNTERED;
	}

	strNameOfWorld = CString(fname);

	//Show dialog.
	InProgress("Opening \""+strNameOfWorld+"\"...");

	// Construct msg.
	m_pServer->GetSimpleStatus(&bstrStatus);

	//now open the file.
	hr = m_pServer->OpenWorld(CComBSTR(strNameOfWorld), &returnHR.m_str);

	if(FAILED(hr))
	{
		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	if(FAILED(hr))
	{
		CString msg = "The attempt to open \""+strNameOfWorld+"\" failed.\n";
		msg +="Error msg from system: \n";
		msg += CString(returnHR);
		AfxMessageBox(msg);
	}
	else
	{
		Done();
		//CString msg = "Successfully saved to \""+strNameOfWorld+".vwc\" and \""+strNameOfWorld+".vwc.bak\".\n";
		//AfxMessageBox(msg);
	}

	::SysFreeString(bstrStatus);

	hr;
}

void CServerV2App::OnUpdateFileOpen(CCmdUI* pCmdUI) 
{
//	pCmdUI->SetText("&Open\""+m_strNameOfWorld+"\" From System");
}

void CServerV2App::OnFileNew() 
{
	//Look up path of html in registry.	
	CComBSTR bstrPath;
	HRESULT	hr;
	
	hr = FindWorldWizardPath(&bstrPath.m_str);
	
	CString finalPath = CString(bstrPath)+"main.htm";

	//Spawn IE.
	HINSTANCE hInst = ShellExecute(NULL,NULL,finalPath,NULL,NULL,0);
	//int ret = system("start iexplore \""+finalPath+"\"");
	if((int)hInst<=32)
	{
		AfxMessageBox("Error: Couldn't find the web page or unable to start Explorer.");
	}

}

void CServerV2App::OnFileImport() 
{
	//Look up path of html in registry.	
	CComBSTR bstrPath;
	HRESULT	hr;
	
	hr = FindWorldWizardPath(&bstrPath.m_str);
	
	CString finalPath = CString(bstrPath)+"loadscript.htm";
	
	//Spawn IE.
	HINSTANCE hInst = ShellExecute(NULL,NULL,finalPath,NULL,NULL,SW_SHOWNORMAL);
	//int ret = system("start iexplore \""+finalPath+"\"");
	if((int)hInst<=32)
	{
		AfxMessageBox("Error: Couldn't find the web page or unable to start Explorer.");
	}
}

void CServerV2App::OnUpdateFileNew(CCmdUI* pCmdUI) 
{
	//Look up path of html in registry.	
	CComBSTR bstrPath;
	HRESULT	hr;
	
	hr = FindWorldWizardPath(&bstrPath.m_str);
	
	CString finalPath = CString(bstrPath)+"main.htm";

	BOOL bExistsOnDisk = (_access(finalPath, 4) == 0) ? TRUE : FALSE;

	pCmdUI->Enable(bExistsOnDisk);
}

void CServerV2App::OnUpdateFileImport(CCmdUI* pCmdUI) 
{
	//Look up path of html in registry.	
	CComBSTR bstrPath;
	HRESULT	hr;
	
	hr = FindWorldWizardPath(&bstrPath.m_str);
	
	CString finalPath = CString(bstrPath)+"loadscript.htm";

	BOOL bExistsOnDisk = (_access(finalPath, 4) == 0) ? TRUE : FALSE;

	pCmdUI->Enable(bExistsOnDisk);
}

void CServerV2App::OnFileSaveLog()
{
	SaveLogHelper(m_pdocServerScope,CString("Server"));
}

void CServerV2App::OnUpdateFileSaveLog(CCmdUI* pCmdUI)
{
	POSITION p;
	CListView* pThisView = NULL;

	if (m_pdocServerScope)
	{
		p = m_pdocServerScope->GetFirstViewPosition();
		pThisView = (CListView *) m_pdocServerScope->GetNextView(p);
		if (pThisView)
		{
			CListCtrl & ThisListCtrl =  pThisView->GetListCtrl();	
			long nItemCount = ThisListCtrl.GetItemCount();
			pCmdUI->Enable(nItemCount > 0);
		}
	}
}

void CServerV2App::WinHelp(DWORD dwData, UINT nCmd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	//CWinApp::WinHelp(dwData, nCmd);
}

BOOL CServerV2App::InitApplication() 
{
	RegisterWindowClass();
	
	return CWinApp::InitApplication();
}

void CServerV2App::OnClearPerf()
{
    if (m_pServer)
        HRESULT hr = m_pServer->ClearPerformanceCounters();
}

#include "OptionsDlg.h"
void CServerV2App::OnToolsOptions() 
{
		CDialogOptions	dlg;

		if (dlg.DoModal() == IDOK)
		{
			if (m_pServer)
			{
				// set the corresponding server flag
				m_pServer->put_UseAuthentication(dlg.m_bAuthenticate);
				if (dlg.m_bUseGroup && dlg.m_bAuthenticate)
					m_pServer->put_AuthenticationGroup(dlg.m_cstrGroupName.AllocSysString());
				else
					m_pServer->put_AuthenticationGroup(NULL);
			}

		}
		
}

void CServerV2App::OnUpdateToolsOptions(CCmdUI* pCmdUI)
{ 
	// Only support these options on NT

	if (m_pServer)
	{
		BOOL bOnNT = FALSE;
		m_pServer->get_RunningOnWinNT(&bOnNT);
		pCmdUI->Enable(bOnNT);
	}
}


#include "DlgMaxLimit.h"

void CServerV2App::OnToolsUserLimits()
{
	CDlgMaxLimit dlg;
	long nLimit = 0;
	long nCurrentCount = 0;

	if (dlg.DoModal() == IDOK)
	{
		if (dlg.m_btnNoLimit == 1)  // Set Limit is checked
			nLimit=atol(dlg.m_strMaxLimit);

		// push the result to the server (and registry)
		if (m_pServer)
		{
			m_pServer->put_ConnectionLimit(nLimit);
			if (nLimit != 0)
			{
				// show a messagebox telling them there are already more users
				m_pServer->get_ConnectionCount(&nCurrentCount);
				if (nCurrentCount>nLimit)
					AfxMessageBox("There are currently more users than your new user limit. \n\rThe system will prevent new connections until the number of users has dropped below your new limit.", MB_ICONEXCLAMATION | MB_OK);
			}
		}
	}

}

//
// HELPER FUNCTIONS
//
// SaveLogHelper function used in both serverv2.cpp and servdoc.cpp
void SaveLogHelper(CDocument * pDoc, CString cstrDefaultFileName)
{
	HRESULT hr = S_OK;
	CComBSTR bstrPath;
	OPENFILENAME OpenFileName;
	char szFileName[MAX_PATH]="\0";
	CString Path;
	CString strSelectedFile;
	char drive[MAX_PATH], dir[MAX_PATH], fname[MAX_PATH], ext[MAX_PATH];
	CString returnedDirectory, returnedExt;
	CComBSTR returnHR;
	CString strNameOfWorld;
	POSITION p;
	CListView* pThisView = NULL;
	CHeaderCtrl* pThisHeaderCtrl = NULL;
	int nItemCount = 0;
	int nHdrItemCount;
	int i =0;
	CString FileText;
	HANDLE hFile = NULL;
	BOOL bSuccess = false;
	DWORD dwBytesWritten = 0;
	HDITEM* pHeaderItem = NULL;
	LVCOLUMN lvcolumn;
	CServerV2App* pApp;

	if (pDoc != NULL)
	{
		//Find the View for this doc and the count of items
		p = pDoc->GetFirstViewPosition();
		pThisView = (CListView *) pDoc->GetNextView(p);
		CListCtrl & ThisListCtrl =  pThisView->GetListCtrl();
		
		nItemCount = ThisListCtrl.GetItemCount();
		if (nItemCount <= 0)
			goto ERROR_ENCOUNTERED;

		pThisHeaderCtrl = ThisListCtrl.GetHeaderCtrl();
		nHdrItemCount = pThisHeaderCtrl->GetItemCount();


		//Present file open dialog for user.
		hr = FindWorldPath(&bstrPath.m_str);
		if(FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		Path = CString(bstrPath);
		Path = Path.Left(Path.GetLength()-1);

		//Seed with the name of the world
		strcpy(szFileName,(LPCTSTR)cstrDefaultFileName);

		pApp=(CServerV2App *)AfxGetApp();

		if (pApp == NULL)
		{	
			hr = E_POINTER;
			goto ERROR_ENCOUNTERED;
		}

		OpenFileName.lStructSize       = sizeof(OPENFILENAME);
		OpenFileName.hwndOwner         = pApp->m_pMainFrame->m_hWnd;
		OpenFileName.hInstance         = AfxGetInstanceHandle();
		OpenFileName.lpstrFilter       = "World Log Files (*.log)\0*.log\0\0";
		OpenFileName.lpstrCustomFilter = NULL;
		OpenFileName.nMaxCustFilter    = 0;
		OpenFileName.nFilterIndex      = 0;
		OpenFileName.lpstrFile         = szFileName;
		OpenFileName.nMaxFile          = sizeof(szFileName);
		OpenFileName.lpstrFileTitle    = NULL;
		OpenFileName.nMaxFileTitle     = 0;
		OpenFileName.lpstrInitialDir   = Path;
		OpenFileName.lpstrTitle        = "Save Log As";
		OpenFileName.nFileOffset       = 0;
		OpenFileName.nFileExtension    = 0;
		OpenFileName.lpstrDefExt       = ".txt\0";
		OpenFileName.lCustData         = (DWORD)&(pApp->m_bAppend);
		OpenFileName.lpfnHook 		   = MyOFNHookProc;
		OpenFileName.lpTemplateName    = (LPTSTR)MAKEINTRESOURCE(IDD_SAVELOGDLG);
		OpenFileName.Flags             = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_ENABLETEMPLATE | OFN_ENABLEHOOK;

		hr = GetSaveFileName(&OpenFileName);
		if (FAILED(hr) || (hr == 0) )
		{
			goto ERROR_ENCOUNTERED;
		}

		strSelectedFile = OpenFileName.lpstrFile;

		//ExtractDirectory.
		_splitpath(strSelectedFile, drive, dir, fname, ext);

		// Open the File
		hFile = CreateFile(
				(LPCTSTR) strSelectedFile,          
				GENERIC_WRITE | GENERIC_READ,       
				FILE_SHARE_READ,					
				NULL,							
				(pApp->m_bAppend ? OPEN_ALWAYS : CREATE_ALWAYS),						
				FILE_ATTRIBUTE_NORMAL,				
				NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			hr = GetLastError();
			goto ERROR_ENCOUNTERED;
		}
		
		// If user chose Append mode from dialog then seek to end
		if (pApp->m_bAppend)
		{
			DWORD dwPtr = SetFilePointer (hFile, NULL, NULL, FILE_END);
			if (dwPtr == 0xFFFFFFFF)
			{
				hr = GetLastError();
				goto ERROR_ENCOUNTERED;
			}
		}

		FileText="\r\n";
		//Iterate through the header columns and get their text
		for(i=0;i<nHdrItemCount;i++)
		{	
			char buff[MAX_PATH]="\0";
			memset(&lvcolumn,0,sizeof(LVCOLUMN));
			lvcolumn.mask=LVCF_TEXT ;
			lvcolumn.pszText=buff;
			lvcolumn.cchTextMax=MAX_PATH;

			if (ThisListCtrl.GetColumn(i,&lvcolumn))
				FileText=FileText +(lvcolumn.pszText)+"\t";
		}
		FileText=FileText + "\r\n";
		FileText=FileText + "--------- \t------- \r\n";

		//add header text to the file
		if (! WriteFile(hFile, FileText.GetBuffer(FileText.GetLength()),
			FileText.GetLength(), &dwBytesWritten, NULL))
		{
			hr = GetLastError();
			goto ERROR_ENCOUNTERED;
		}
			
		//Iterate through the List items and then write to the specified file
		//TODO:  if nItemCount is large should we show progress..
		for(i=0;i<nItemCount;i++)
		{
			//get each line in list
			FileText=ThisListCtrl.GetItemText(i,0)+"\t"+ThisListCtrl.GetItemText(i,1)+"\r\n";
			
			//append this line to the file
			if (!WriteFile(hFile, FileText.GetBuffer(FileText.GetLength()),
				FileText.GetLength(), &dwBytesWritten, NULL))
			{	
				// we failed for some reason.. bail out
				hr = GetLastError();
				goto ERROR_ENCOUNTERED;
			}

		}

		// Now clear the list
		ThisListCtrl.DeleteAllItems();

	} // if (pDoc!=NULL)
		
	
ERROR_ENCOUNTERED:
	if(FAILED(hr))
	{	LPSTR lpmsgbuffer = NULL;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |FORMAT_MESSAGE_ALLOCATE_BUFFER,
						NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), lpmsgbuffer, 0, NULL);

		CString msg = "The attempt to save \""+strSelectedFile+"\" failed.\n";
		msg +="Cause: \n";
		msg += CString((LPSTR)lpmsgbuffer);

		AfxMessageBox(msg, MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
		
		LocalFree(lpmsgbuffer);
	}

	// Close our file handle
	if (hFile != INVALID_HANDLE_VALUE)
		CloseHandle(hFile);
	
}