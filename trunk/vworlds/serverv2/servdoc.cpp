// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// ServerV2Doc.cpp : implementation of the CServerV2Doc class
//

#include "stdafx.h"
#include "ServerV2.h"
#include "servview.h"
#include "ServDoc.h"
#include "SaveDlg.h"
#include "..\vwcommon\reghelp.h"
#include <io.h>
#include <wininet.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerV2Doc

IMPLEMENT_DYNCREATE(CServerV2Doc, CDocument)

BEGIN_MESSAGE_MAP(CServerV2Doc, CDocument)
	//{{AFX_MSG_MAP(CServerV2Doc)
	ON_COMMAND(ID_SAVEWORLD, OnSaveworld)
	ON_UPDATE_COMMAND_UI(ID_SAVEWORLD, OnUpdateSaveworld)

	ON_COMMAND(ID_FILE_SAVEWORLDAS, OnFileSaveworldas)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVEWORLDAS, OnUpdateFileSaveworldas)

	ON_COMMAND(ID_DELETEWORLDMENU, OnDeleteworldmenu)
	ON_UPDATE_COMMAND_UI(ID_DELETEWORLDMENU, OnUpdateDeleteworldmenu)

	ON_COMMAND(ID_ExportScript, OnExportScript)
	ON_UPDATE_COMMAND_UI(ID_ExportScript, OnUpdateExportScript)

	ON_COMMAND(ID_FILE_EXPORTWORLDAS, OnFileExportworldas)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORTWORLDAS, OnUpdateFileExportworldas)

	ON_COMMAND(ID_FILE_SAVELOG, OnFileSaveLog)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVELOG, OnUpdateFileSaveLog)

	ON_UPDATE_COMMAND_UI(ID_TakeWorldOffline, OnUpdateTakeWorldOffline)
	ON_COMMAND(ID_TakeWorldOffline, OnTakeWorldOffline)

	ON_COMMAND(ID_TOOLS_SHOWWORLDINDS, OnToolsShowWorldInDS)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SHOWWORLDINDS, OnUpdateToolsShowWorldInDS)

	ON_COMMAND(ID_TOOLS_SETINFOURL, OnToolsSetInfoURL)

	ON_COMMAND(ID_TEST_PERFORMANCETEST1, OnTestPerformancetest1)
	ON_UPDATE_COMMAND_UI(ID_TEST_PERFORMANCETEST1, OnUpdateTestPerformancetest1)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerV2Doc construction/destruction

CServerV2Doc::CServerV2Doc()
{
	// TODO: add one-time construction code here
}

CServerV2Doc::~CServerV2Doc()
{
	if(m_pApp)
		m_pApp->HandleOnRemove(m_strNameOfWorld);
}

void CServerV2Doc::InProgress(CString string)
{
	BSTR bstrStatus = NULL;

	if(m_pApp)
	{
		if(m_pApp->m_pMainFrame)
			m_pApp->m_pMainFrame->m_wndStatusBar.SetWindowText(string+". "+CString(bstrStatus));
		if(m_pApp->m_pServer)
			m_pApp->m_pServer->GetSimpleStatus(&bstrStatus);
	}
	::SysFreeString(bstrStatus);
}

void CServerV2Doc::Done(void)
{
	if(m_pApp)
		if(m_pApp->m_pMainFrame)
			m_pApp->m_pMainFrame->m_wndStatusBar.SetWindowText("The operation completed successfully.");
}

void CServerV2Doc::Done(CString temp)
{
	if(m_pApp)
		if(m_pApp->m_pMainFrame)
			m_pApp->m_pMainFrame->m_wndStatusBar.SetWindowText(temp);
}

BOOL CServerV2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CServerV2Doc serialization

void CServerV2Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CServerV2Doc diagnostics

#ifdef _DEBUG
void CServerV2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CServerV2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CServerV2Doc commands

BOOL CServerV2Doc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	//if (!CDocument::OnOpenDocument(lpszPathName))
	//	return FALSE;
	HandleOnAvailableWorld(lpszPathName, 0);

	m_pApp = (CServerV2App *)AfxGetApp();

//	m_pApp->m_pMainFrame->MDITile(MDITILE_HORIZONTAL);
	m_pApp->m_pMainFrame->MDICascade();

	return TRUE;
}

HRESULT CServerV2Doc::CheckIfOnline()
{
	HRESULT hr = S_OK;
	if(m_strNameOfWorld!="")
	{
		BOOL	bIsOnline;

		hr = IsWorldRegisteredOnline(CComBSTR(m_strNameOfWorld), &bIsOnline);
		m_bOffline = !bIsOnline;
	}
	return hr;
}


HRESULT CServerV2Doc::HandleOnAvailableWorld(CString strName, int iNoUsers)
{
	m_pApp = (CServerV2App *)AfxGetApp();

	HRESULT hr = S_OK;
	CString strTitle = GetTitle();
	CString newTitle;
	
	m_iNoUsers = iNoUsers;
	m_strNameOfWorld = strName;

	hr = CheckIfOnline();

	ASSERT(strName!="");
	
	newTitle = strName;

	if(m_strNameOfWorld!="")
	{
		BOOL	bIsInDS = FALSE;
		CComBSTR	bstrInfoURL;

		hr = GetWorldInfoUrlFromRegistry(CComBSTR(m_strNameOfWorld), &bstrInfoURL.m_str);
		if (SUCCEEDED(hr) && bstrInfoURL.m_str)
			m_strWorldInfoURL = bstrInfoURL;

		IsWorldRegisteredInDS(CComBSTR(m_strNameOfWorld), &bIsInDS);
		
		if (bIsInDS)
		{
			if(m_pApp->m_bConnectedToDS)
			{
				newTitle+=" (Public)";
			}
			else
			{
				newTitle+=" (Public)";
				//newTitle+=" (public but no DS)";
			}
		}
		else
		{
			newTitle+=" (Private)";
		}
	}

	if(m_bOffline==TRUE)
	{
		newTitle += ": Offline";
	}
	else
	{
		newTitle += ": " + str(m_iNoUsers) + " User(s)";
	}

	SetTitle(newTitle);

	

	return hr;
}

HRESULT CServerV2Doc::HandleOnReport(CString string, int iType)
{
	HRESULT hr = S_OK;
	
	ASSERT(string!="");
	POSITION pos = GetFirstViewPosition();   
	while (pos != NULL)   
	{
      CServerV2View* pView = (CServerV2View *)GetNextView(pos);      
	  pView->HandleOnReport(string, iType);   
	}   

	return hr;
}


void CServerV2Doc::OnUpdateSaveworld(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bOffline);
//	pCmdUI->SetText("&Save \""+m_strNameOfWorld+"\"\tCtrl+S");
}

void CServerV2Doc::SetFocus(void)
{
	UpdateAllViews(NULL);

	POSITION pos = GetFirstViewPosition();   
	ASSERT(pos);
	if(pos)
	{
		CServerV2View* pView = (CServerV2View *)GetNextView(pos);      
		pView->SetFocus();
		pView->SetForegroundWindow( );
		pView->SetActiveWindow( );
		TRACE("Setting focus.\n");
	}
}

void CServerV2Doc::OnSaveworld() 
{
	AppCriticalSection critsec(m_pApp);

	InProgress("Saving \""+m_strNameOfWorld+"\"...");
	
	BSTR bstrHR = NULL;
	HRESULT hr = m_pApp->m_pServer->SaveWorld(CComBSTR(m_strNameOfWorld), &bstrHR);

	if(FAILED(hr))
	{
		CString msg = "The attempt to save \""+m_strNameOfWorld+".vwc and \""+m_strNameOfWorld+".vwc.bak\" failed.\n";
		msg +="Error msg from system: \n";
		msg += CString(bstrHR);
		AfxMessageBox(msg);
	}
	else
	{
		Done("Successfully saved to \""+m_strNameOfWorld+".vwc\" and \""+m_strNameOfWorld+".bak.vwc\".\n");
		//AfxMessageBox(msg);
	}
	
	::SysFreeString(bstrHR);
}

void CServerV2Doc::OnUpdateExportScript(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bOffline);
//	pCmdUI->SetText("&Export \""+m_strNameOfWorld+"\"\tCtrl+E");
}

void CServerV2Doc::OnExportScript() 
{
	AppCriticalSection critsec(m_pApp);

	CComBSTR bstrFinalPath;
	HRESULT hr;
	BSTR bstrHR = NULL;

	InProgress("Exporting script for \""+m_strNameOfWorld+"\"...");

	hr = CanonDatabaseName(CComBSTR(m_strNameOfWorld), &bstrFinalPath.m_str);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;


	hr = m_pApp->m_pServer->SaveWorldScript(CComBSTR(m_strNameOfWorld), CComBSTR(CString(bstrFinalPath)+".vbs"), &bstrHR);

ERROR_ENCOUNTERED:

	if(FAILED(hr))
	{
		CString msg = "The attempt to save-to-script \""+m_strNameOfWorld+".vbs\" failed.\n";
		msg +="Error msg from system: \n";
		msg += CString(bstrHR);
		AfxMessageBox(msg);
	} else
	{
		Done("Successfully exported script to \""+m_strNameOfWorld+".vbs\".\n");
		//AfxMessageBox(msg);
	}
	
	::SysFreeString(bstrHR);
}

void CServerV2Doc::OnDeleteworldmenu() 
{
	//WARNING: MULTITHREAD SENSITIVE CODE.
	AppCriticalSection critsec(m_pApp);

	if(IDOK==AfxMessageBox("This will permanently delete \""+m_strNameOfWorld+"\". Are you sure you wish to proceed?", MB_OKCANCEL))
	{
		InProgress("Deleting \""+m_strNameOfWorld+"\" from system");

		BSTR bstrHR = NULL;
		HRESULT hr = m_pApp->m_pServer->DeleteWorldFromSystem(CComBSTR(m_strNameOfWorld), &bstrHR);


		if(FAILED(hr))
		{
			CString msg = "The attempt to delete \""+m_strNameOfWorld+"\" failed.\n";
			msg +="Error msg from system: \n";
			msg += CString(bstrHR);
			AfxMessageBox(msg);
		} else
		{
			//Cannot do the below operation. This document could be gone by now already.
			//Done("Successfully deleted \""+m_strNameOfWorld+"\".\n");

			//delete from map.
			
			//cache the app.
			//CServerV2App *pApp = m_pApp;

			//m_pApp->HandleOnRemove(m_strNameOfWorld);

//			OnCloseDocument();

			//pApp->TryToFocusOnAWorld();
		}
		
		::SysFreeString(bstrHR);

	}
}

void CServerV2Doc::OnUpdateDeleteworldmenu(CCmdUI* pCmdUI) 
{
//	pCmdUI->SetText("&Delete \""+m_strNameOfWorld+"\"\tCtrl+D");
}

void CServerV2Doc::OnUpdateTakeWorldOffline(CCmdUI* pCmdUI) 
{
	if(!m_bOffline)
	{
//		pCmdUI->SetText("&Take \""+m_strNameOfWorld+"\" Offline");
		pCmdUI->SetText("&Take World Offline");
	}
	else
	{
//		pCmdUI->SetText("&Put \""+m_strNameOfWorld+"\" Back Online");
		pCmdUI->SetText("&Put World Online");
	}
}

void CServerV2Doc::OnTakeWorldOffline() 
{
	AppCriticalSection critsec(m_pApp);

	if(m_bOffline)
	{
		//Put world online.
		InProgress("Putting \""+m_strNameOfWorld+"\" online...");
		
		BSTR bstrHR = NULL;
		HRESULT hr = m_pApp->m_pServer->TakeWorldOnline(CComBSTR(m_strNameOfWorld), &bstrHR);

		RegisterWorldInDS(CComBSTR(m_strNameOfWorld), NULL, FALSE);
		//m_pApp->m_pServer->RefreshDS();

		if(FAILED(hr))
		{
			CString msg = "The attempt to put \""+m_strNameOfWorld+"\" online failed.\n";
			msg +="Error msg from system: \n";
			msg += CString(bstrHR);
			AfxMessageBox(msg);
		} else
		{
			Done("Successfully put \""+m_strNameOfWorld+"\" online.");
			//AfxMessageBox(msg);
		}
		
		::SysFreeString(bstrHR);
	}
	else
	{
		//Put world offline.
		InProgress("Taking \""+m_strNameOfWorld+"\" offline...");
		
		BSTR bstrHR = NULL;
		HRESULT hr = m_pApp->m_pServer->TakeWorldOffline(CComBSTR(m_strNameOfWorld), &bstrHR);

		RegisterWorldInDS(CComBSTR(m_strNameOfWorld), NULL, FALSE);
		m_pApp->m_pServer->RefreshDS();

		if(FAILED(hr))
		{
			CString msg = "The attempt to take \""+m_strNameOfWorld+"\" offline failed.\n";
			msg +="Error msg from system: \n";
			msg += CString(bstrHR);
			AfxMessageBox(msg);
		} else
		{
			Done("Successfully took \""+m_strNameOfWorld+"\" offline.\n");
			//AfxMessageBox(msg);
		}
		
		::SysFreeString(bstrHR);
	}

	HandleOnAvailableWorld(m_strNameOfWorld, 0);

}

//********************************************
// OnToolsShowWorldInDS
// Removes the world from the Directory Server
// This writes the new value into the registry,
// then refreshes the DS thread

void CServerV2Doc::OnToolsShowWorldInDS() 
{
	if(m_strNameOfWorld!="")
	{
		BOOL	bIsInDS = FALSE;

		IsWorldRegisteredInDS(CComBSTR(m_strNameOfWorld), &bIsInDS);
		RegisterWorldInDS(CComBSTR(m_strNameOfWorld), NULL, !bIsInDS);
		m_pApp->m_pServer->RefreshDS();
	}

	HandleOnAvailableWorld(m_strNameOfWorld, m_iNoUsers);
}

//********************************************
// OnUpdateToolsShowWorldInDS
// Puts a checkmark by the menu item if theworld
// is currently listed in the directory server.

void CServerV2Doc::OnUpdateToolsShowWorldInDS(CCmdUI* pCmdUI) 
{
	if(m_pApp->m_bConnectedToDS && !m_bOffline)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
	
	pCmdUI->SetCheck(0);
	if(m_strNameOfWorld!="")
	{
		BOOL	bIsInDS = FALSE;

		IsWorldRegisteredInDS(CComBSTR(m_strNameOfWorld), &bIsInDS);
		if (bIsInDS)
			pCmdUI->SetCheck(1);
	}
//	pCmdUI->SetText("&Publish \""+m_strNameOfWorld+"\" in Directory Service");
}

#include "infoURL.h"

void CServerV2Doc::OnToolsSetInfoURL() 
{
	if(m_strNameOfWorld!="")
	{
		BOOL		bIsInDS = FALSE;
		CComBSTR	bstrInfoURL;
		HRESULT		hr;
		char buf[80];
		DWORD length = 80;	
		CDlgInfoURL	dlg;
		
		hr = GetWorldInfoUrlFromRegistry(CComBSTR(m_strNameOfWorld), &bstrInfoURL.m_str);
		if (SUCCEEDED(hr) && bstrInfoURL.m_str)
			m_strWorldInfoURL = bstrInfoURL;

		//Decode URL first.
		BOOL bret = InternetCanonicalizeUrl(m_strWorldInfoURL, buf, &length, ICU_DECODE | ICU_NO_ENCODE);
		if(bret==TRUE)
		{
			dlg.m_strInfoURL  = CString(buf);			
		}
		else
		{
			dlg.m_strInfoURL = m_strWorldInfoURL;
		}

		if (dlg.DoModal() == IDOK)
		{
			BOOL	bIsInDS = TRUE;
			m_strWorldInfoURL = dlg.m_strInfoURL;
			
			bstrInfoURL = m_strWorldInfoURL;
			IsWorldRegisteredInDS(CComBSTR(m_strNameOfWorld), &bIsInDS);
			RegisterWorldInDS(CComBSTR(m_strNameOfWorld), bstrInfoURL, bIsInDS);
			m_pApp->m_pServer->RefreshDS();
		}
	}
}


void CServerV2Doc::OnFileSaveworldas() 
{
	AppCriticalSection critsec(m_pApp);

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
	int retValue;

	//Present file open dialog for user.
	hr = FindWorldPath(&bstrPath.m_str);
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
	Path = CString(bstrPath);
	Path = Path.Left(Path.GetLength()-1);

	OpenFileName.lStructSize       = sizeof(OPENFILENAME);
    OpenFileName.hwndOwner         = m_pApp->m_pMainFrame->m_hWnd;
    OpenFileName.hInstance         = NULL;
    OpenFileName.lpstrFilter       = "Microsoft Virtual Worlds Databases\0*.vwc\0";
    OpenFileName.lpstrCustomFilter = NULL;
    OpenFileName.nMaxCustFilter    = 0;
    OpenFileName.nFilterIndex      = 0;
    OpenFileName.lpstrFile         = szFile;
    OpenFileName.nMaxFile          = sizeof(szFile);
    OpenFileName.lpstrFileTitle    = NULL;
    OpenFileName.nMaxFileTitle     = 0;
    OpenFileName.lpstrInitialDir   = Path;
    OpenFileName.lpstrTitle        = "Save As:";
    OpenFileName.nFileOffset       = 0;
    OpenFileName.nFileExtension    = 0;
    OpenFileName.lpstrDefExt       = NULL;
    OpenFileName.lCustData         = NULL;
	OpenFileName.lpfnHook 		   = NULL;
	OpenFileName.lpTemplateName    = NULL;
    OpenFileName.Flags             = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

	if (!GetSaveFileName(&OpenFileName))
	{
		goto ERROR_ENCOUNTERED;
	}

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
	if(returnedExt!="" && returnedExt!=".vwc")
	{
		AfxMessageBox("Sorry, all checkpoint files must have a .vwc extension.");
		goto ERROR_ENCOUNTERED;
	}

	strNameOfWorld = CString(fname);
	strNameOfWorld += ".vwc";

	if ((retValue=_access(strNameOfWorld, 06))!=-1) 
	{
		if(m_pApp->FindWorld(CString(fname)))
		{
			AfxMessageBox("Sorry, this filename is being used by an already registered world.");
			goto ERROR_ENCOUNTERED;
		}
	}

	InProgress("Saving \""+m_strNameOfWorld+"\" to \""+CString(fname)+"\"...");

	//now open the file.
	hr = m_pApp->m_pServer->SaveWorldAs(CComBSTR(m_strNameOfWorld), CComBSTR(strNameOfWorld), &returnHR.m_str);
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;

	//change name.
	//Rely on refreshds to do this.
	//hr = m_pApp->HandleOnChangeName(CComBSTR(m_strNameOfWorld), CComBSTR(fname), m_iNoUsers);
	//if(FAILED(hr))
	//	goto ERROR_ENCOUNTERED;

	hr = m_pApp->m_pServer->RefreshDS();
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	if(FAILED(hr))
	{
		CString msg = "The attempt to save \""+strNameOfWorld+"\" failed.\n";
		msg +="Error msg from system: \n";
		msg += CString(returnHR);
		AfxMessageBox(msg);

	}
	else
	{
		Done();
		m_pApp->m_pMainFrame->MDICascade();
		//AfxMessageBox(msg);
	}

	::SysFreeString(bstrStatus);

	hr;
}

void CServerV2Doc::OnUpdateFileSaveworldas(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bOffline);
//	pCmdUI->SetText("Save \""+m_strNameOfWorld+"\" &As...\tCtrl+A");
}


void CServerV2Doc::OnFileSaveLog()
{
	SaveLogHelper(this,m_strNameOfWorld);
}

void CServerV2Doc::OnUpdateFileSaveLog(CCmdUI* pCmdUI)
{
	POSITION p;
	CListView* pThisView = NULL;

	p = this->GetFirstViewPosition();
	pThisView = (CListView *) this->GetNextView(p);
	if (pThisView)
	{
		CListCtrl & ThisListCtrl =  pThisView->GetListCtrl();	
		long nItemCount = ThisListCtrl.GetItemCount();
		pCmdUI->Enable(nItemCount > 0);
	}

}

void CServerV2Doc::OnFileExportworldas() 
{
	AppCriticalSection critsec(m_pApp);

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

	ProgressDialog dlg(TRUE, m_pApp);
		
	//Present file open dialog for user.
	hr = FindWorldPath(&bstrPath.m_str);
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
	Path = CString(bstrPath);
	Path = Path.Left(Path.GetLength()-1);

	OpenFileName.lStructSize       = sizeof(OPENFILENAME);
    OpenFileName.hwndOwner         = m_pApp->m_pMainFrame->m_hWnd;
    OpenFileName.hInstance         = NULL;
    OpenFileName.lpstrFilter       = "Microsoft Virtual Worlds Exported Databases\0*.vbs\0\0";
    OpenFileName.lpstrCustomFilter = NULL;
    OpenFileName.nMaxCustFilter    = 0;
    OpenFileName.nFilterIndex      = 0;
    OpenFileName.lpstrFile         = szFile;
    OpenFileName.nMaxFile          = sizeof(szFile);
    OpenFileName.lpstrFileTitle    = NULL;
    OpenFileName.nMaxFileTitle     = 0;
    OpenFileName.lpstrInitialDir   = Path;
    OpenFileName.lpstrTitle        = "Export As";
    OpenFileName.nFileOffset       = 0;
    OpenFileName.nFileExtension    = 0;
    OpenFileName.lpstrDefExt       = NULL;
    OpenFileName.lCustData         = NULL;
	OpenFileName.lpfnHook 		   = NULL;
	OpenFileName.lpTemplateName    = NULL;
    OpenFileName.Flags             = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

	if (!GetSaveFileName(&OpenFileName))
	{
		goto ERROR_ENCOUNTERED;
	}

	strSelectedFile = OpenFileName.lpstrFile;

	//ExtractDirectory.
	_splitpath(strSelectedFile, drive, dir, fname, ext);
	
	//ascertain that the extension is the same.
	returnedExt = CString(ext);
	returnedExt.MakeLower();
	if(returnedExt!="" && returnedExt!=".vbs")
	{
		AfxMessageBox("Script files should have a .vbs extension.");
		goto ERROR_ENCOUNTERED;
	}

	InProgress("Saving \""+m_strNameOfWorld+"\" to \""+CString(fname)+"\"...");

	hr = m_pApp->m_pServer->SaveWorldScript(CComBSTR(m_strNameOfWorld), CComBSTR(CString(drive)+CString(dir)+CString(fname)+".vbs"), &returnHR.m_str);

	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pApp->m_pServer->RefreshDS();
	if(FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	if(FAILED(hr))
	{
		CString msg = "The attempt to save \""+strNameOfWorld+"\" failed.\n";
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

void CServerV2Doc::OnUpdateFileExportworldas(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bOffline);
//	pCmdUI->SetText("Ex&port \""+m_strNameOfWorld+"\" As...");
}

void CServerV2Doc::OnTestPerformancetest1() 
{
	AppCriticalSection critsec(m_pApp);

	InProgress("Testing \""+m_strNameOfWorld+"\"...");
	
	BSTR bstrHR = NULL;

	DWORD dwStart = ::GetTickCount();

	HRESULT hr = m_pApp->m_pServer->PerformanceTest(CComBSTR(m_strNameOfWorld), 0, &bstrHR);

	if(FAILED(hr))
	{
		CString msg = "The test of \""+m_strNameOfWorld+".vwc failed.\n";
		msg +="Error msg from system: \n";
		msg += CString(bstrHR);
		AfxMessageBox(msg);
	}
	else
	{
		CString str;
		str.Format("Test succeeded. Elapsed time: %dms.\n", ::GetTickCount()-dwStart);
		AfxMessageBox(str);
	}
	
	Done();

	::SysFreeString(bstrHR);	
}

void CServerV2Doc::OnUpdateTestPerformancetest1(CCmdUI* pCmdUI) 
{
	pCmdUI->SetText("Test \""+m_strNameOfWorld+"\"");
}

void CServerV2Doc::OnCloseDocument() 
{
	m_pApp->HandleOnRemove(m_strNameOfWorld);
	
	CDocument::OnCloseDocument();
}



UINT CALLBACK MyOFNHookProc(HWND hdlg,      // handle to child dialog window
			UINT uiMsg,     // message identifier
			WPARAM wParam,  // message parameter
			LPARAM lParam   // message parameter
			)
{
	OFNOTIFY* pOFNotify = NULL;
	CSaveDlg* pSaveDlg = NULL;
	CFileDialog* pFileDlg = NULL;
	static BOOL* pbAppend;
	switch(uiMsg)
	{
		case WM_NOTIFY:
		{	
			break;
		}

		case WM_COMMAND:
		{
			if (HIWORD(wParam)==BN_CLICKED)
			{
				// see if clicking on the Append box and change state
				int idButton = (int) LOWORD(wParam);   
				if (idButton==IDC_CHECK1)
				{
					if (pbAppend)
						*pbAppend = !*pbAppend;
				}		
			}
			break;
		}
		case WM_INITDIALOG:
		{
			pbAppend = NULL;
			pbAppend = (BOOL*) ((OPENFILENAME*)lParam)->lCustData;
			*pbAppend = true;
			break;
		}
	} // end switch

	return 0;
}


