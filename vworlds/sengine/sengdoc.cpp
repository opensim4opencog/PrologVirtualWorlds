// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// SEngineDoc.cpp : implementation of the CSEngineDoc class
//

// NOTE: need to have afxpriv first because of ATL
#include <afxpriv.h>
#include "stdafx.h"
#include "SEngine.h"

#include "SEngDoc.h"
#include "GetMeDlg.h"
#include "dbgview.h"
#include "ocxview.h"
#include "ChildFrm.h"

#include <propbase.h>

#define IID_DEFINED
#include "vwobject_i.c"
#include "vwclient_i.c"

// 30 minute timeout
#define DEFAULT_TIMEOUT (30 * 60 * 1000)

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/
/////////////////////////////////////////////////////////////////////////////
// CSEngineDoc

IMPLEMENT_DYNCREATE(CSEngineDoc, CDocument)

BEGIN_MESSAGE_MAP(CSEngineDoc, CDocument)
	//{{AFX_MSG_MAP(CSEngineDoc)
	ON_COMMAND(ID_SCRIPT_LOAD, OnScriptLoad)
	ON_UPDATE_COMMAND_UI(ID_SCRIPT_INVOKESUB, OnUpdateScriptInvokesub)
	ON_COMMAND(ID_SCRIPT_INVOKESUB, OnScriptInvokesub)
	ON_COMMAND(ID_SCRIPT_ENUMERATETYPEINFO, OnScriptEnumeratetypeinfo)
	ON_UPDATE_COMMAND_UI(ID_SCRIPT_ENUMERATETYPEINFO, OnUpdateScriptEnumeratetypeinfo)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CSEngineDoc, CDocument)
	//{{AFX_DISPATCH_MAP(CSEngineDoc)
	DISP_PROPERTY_NOTIFY(CSEngineDoc, "StatusText", m_statusText, OnStatusTextChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CSEngineDoc, "XObject", m_xObject, OnXObjectChanged, VT_DISPATCH)
	DISP_FUNCTION(CSEngineDoc, "DoMessageBox", DoMessageBox, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CSEngineDoc, "CreateNewX", CreateNewX, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION(CSEngineDoc, "CreateObject", CreateOLEObject, VT_DISPATCH, VTS_BSTR)
	DISP_FUNCTION(CSEngineDoc, "InvokeScriptlet", oleInvokeScriptlet, VT_ERROR, VTS_BSTR)
	DISP_FUNCTION(CSEngineDoc, "CreateOCXView", CreateOCXView, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CSEngineDoc, "CreateOCX", CreateOCX, VT_DISPATCH, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ISEngin to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {2C06A6F2-D0C2-11CF-8A96-00A0C9082583}
static const IID IID_ISEngin =
{ 0x2c06a6f2, 0xd0c2, 0x11cf, { 0x8a, 0x96, 0x0, 0xa0, 0xc9, 0x8, 0x25, 0x83 } };

BEGIN_INTERFACE_MAP(CSEngineDoc, CDocument)
	INTERFACE_PART(CSEngineDoc, IID_ISEngin, Dispatch)
	INTERFACE_PART(CSEngineDoc, IID_IVWClientSite, ClientSite)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSEngineDoc construction/destruction

CSEngineDoc::CSEngineDoc():m_ScriptSite()
{
	// TODO: add one-time construction code here

	EnableAutomation();

	AfxOleLockApp();
//	m_xObject = GetIDispatch(TRUE);
	m_xObject = NULL;

#if 0
	CLSID  clsid;
	if (CLSIDFromProgID(CComBSTR("VWSYSTEM.Client.1"), &clsid ) ==S_OK )
		CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_IDispatch, (void **)&m_xObject);
	
	if ( !m_xObject ) m_xObject = GetIDispatch( TRUE );

	Advise( (IUnknown *) m_xObject, IID_IVWClientSite, &m_dwClientCookie );
#endif
}

CSEngineDoc::~CSEngineDoc()
{
	IVWClient * pVWClient = NULL;

	m_ScriptSite.Close();

	if ( m_xObject )
	{
		UnAdvise( (IUnknown *) m_xObject,IID_IVWClientSite, m_dwClientCookie);
		
		if ( m_xObject->QueryInterface(IID_IVWClient,(void**) &pVWClient) == S_OK )
		{
			pVWClient->Disconnect();
			pVWClient->Terminate();
			SAFERELEASE(pVWClient);
		}

		SAFERELEASE(m_xObject);
	}
	AfxOleUnlockApp();
}

BOOL CSEngineDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CSEngineDoc serialization

void CSEngineDoc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	
//	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);
	CView * pView = (CView *) m_viewList.GetHead();
	if ( pView->IsKindOf( RUNTIME_CLASS( CEditView)))
		((CEditView*)pView)->SerializeRaw(ar);

	if ( ar.IsLoading() )// Load the script engine
	{
		// Need to set the document path first
		CFile *cf = ar.GetFile();
		SetPathName( cf->GetFilePath() );
		OnScriptLoad();
		AfxGetMainWnd()->PostMessage( WM_COMMAND, ID_WINDOW_COMMANDWINDOW );
	}

}


BOOL CSEngineDoc::CanCloseFrame(CFrameWnd* pFrameArg)
	// permission to close all views using this frame
	//  (at least one of our views must be in this frame)
{
	ASSERT_VALID(pFrameArg);
	UNUSED(pFrameArg);   // unused in release builds

	POSITION pos = GetFirstViewPosition();
	int nEditViews = 0;
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		ASSERT_VALID(pView);
		if (pView->IsKindOf( RUNTIME_CLASS( CEditView))) nEditViews++;
		CFrameWnd* pFrame = pView->GetParentFrame();
		// assume frameless views are ok to close
		if (pFrame != NULL)
		{
			// assumes 1 document per frame
			ASSERT_VALID(pFrame);

			if (pFrame->m_nWindow > 0)
			{
				return TRUE;        // more than one frame refering to us
			}
		}
	}

	// otherwise only one frame that we know about
	return SaveModified();
}

	
void CSEngineDoc::PreCloseFrame(CFrameWnd* pFrame)
{
	CView* pView = pFrame->GetActiveView();
	ASSERT_VALID(pView);
	if (pView->IsKindOf( RUNTIME_CLASS( CEditView))) 
	{
		if ( SaveModified() )
		AfxGetMainWnd()->PostMessage( WM_COMMAND, ID_FILE_CLOSE );
		
	}
}
	
/////////////////////////////////////////////////////////////////////////////
// CSEngineDoc diagnostics

#ifdef _DEBUG
void CSEngineDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSEngineDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSEngineDoc commands

void CSEngineDoc::OnStatusTextChanged() 
{
	// TODO: Add notification handler code
	const char * statusText = m_statusText;
	AfxGetMainWnd()->GetTopLevelFrame()->SendMessage(WM_SETMESSAGESTRING, 0, (LPARAM ) statusText );
	CWnd* pMessageBar = AfxGetMainWnd()->GetTopLevelFrame()->GetMessageBar();
	pMessageBar->RedrawWindow();
}

void CSEngineDoc::OnScriptLoad() 
{
	// TODO: Add your command handler code here
	// Set up names
	// Clean up the world... 

	IVWClient* pVWClient = NULL;

	// close the existing script
	m_ScriptSite.Close();

	if ( m_xObject )
	{
		UnAdvise( (IUnknown *) m_xObject,IID_IVWClientSite, m_dwClientCookie);
		
		if ( m_xObject->QueryInterface(IID_IVWClient,(void**) &pVWClient) == S_OK )
		{
			pVWClient->Disconnect();
			pVWClient->Terminate();
			SAFERELEASE(pVWClient);
		}

		SAFERELEASE(m_xObject);
	}

	CLSID  clsid;
	// (MMar) - we might not have VWSYTEM DLL, and we don't want sengine to fail
	ASSERT(m_xObject == NULL);
	if (CLSIDFromProgID(CComBSTR("VWSYSTEM.Client.1"), &clsid ) ==S_OK )
		CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_IDispatch, (void **)&m_xObject);

//	ASSERT( m_xObject );

	if ( m_xObject )
	{
		if ( m_xObject->QueryInterface(IID_IVWClient,(void**) &pVWClient) == S_OK )
		{
			pVWClient->Initialize();
			SAFERELEASE(pVWClient);
		}
	}

	UpdateAllViews(NULL, CHANGE_CLIENT, (CObject *)m_xObject);
	if (m_xObject)
		Advise( (IUnknown *) m_xObject,IID_IVWClientSite, &m_dwClientCookie);

	// Add the name space
	IDispatch * pDisp = GetIDispatch(TRUE);
	ITypeInfo * pTypeInfo = NULL;
	ASSERT( pDisp );
	HRESULT hr = pDisp->GetTypeInfo(0,LOCALE_SYSTEM_DEFAULT, &pTypeInfo );
	m_ScriptSite.AddNamedItem(CString("Document"), pDisp, pTypeInfo);
	SAFERELEASE(pDisp);
	SAFERELEASE(pTypeInfo);

	// Set up X Object
	if (m_xObject && ((hr = m_xObject->QueryInterface(IID_IDispatch, (void**)&pDisp)) == S_OK)) {
		pDisp->GetTypeInfo(0,LOCALE_SYSTEM_DEFAULT, &pTypeInfo );
		m_ScriptSite.AddNamedItem(CString("XObject"), pDisp, NULL);// pTypeInfo);
		SAFERELEASE(pDisp);
		SAFERELEASE(pTypeInfo);
	}

	CString fname = GetPathName( ) ;
	// change the current directory...
	int index = fname.ReverseFind('\\');
	if ( index >= 0 )  // there is a directory separator
	{
		char path[MAX_PATH + 1];
		strncpy(path,LPCSTR(fname), index );
		path[index] = '\0';

		BOOL bRet = SetCurrentDirectory( path ); 
		if ( !bRet )
		{
			AfxMessageBox( "Unable To Change directories" );
		}

	}


	if ( fname.GetLength() == 0 )
		fname= ":VBScript";
	CString Context = "Document";
	if ( m_ScriptSite.LoadScript( fname, Context ) != S_OK)
	{
		int err = m_ScriptSite.GetLastError();
		CString strError = m_ScriptSite.strerror( err );
		UpdateAllViews(NULL, err, NULL );
		AfxMessageBox( strError );
	}
}

void CSEngineDoc::OnUpdateScriptInvokesub(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( m_ScriptSite.IsLoaded() );

}

void CSEngineDoc::OnScriptInvokesub() 
{
	// TODO: Add your command handler code here
	CGetMethodDlg Dlg;
	Dlg.m_MethodString = "DoIt";
	if ( Dlg.DoModal() == IDOK )
	{

		if ( m_ScriptSite.InvokeByName( Dlg.m_MethodString, 
									    CString("Document"),
										DISPATCH_METHOD, 
										NULL, 
										NULL,
										NULL,
										DEFAULT_TIMEOUT) != S_OK)
		{
		int err = m_ScriptSite.GetLastError();
		CString strError = m_ScriptSite.strerror( err );
		UpdateAllViews(NULL, err, NULL );
		AfxMessageBox( strError );
		}
	}
}

void CSEngineDoc::OnScriptEnumeratetypeinfo() 
{
	// TODO: Add your command handler code here
	if ( m_ScriptSite.ShowActions() != S_OK )
	{
		int err = m_ScriptSite.GetLastError();
		CString strError = m_ScriptSite.strerror( err );
		UpdateAllViews(NULL, err, NULL );
		AfxMessageBox( strError );
	}
}

void CSEngineDoc::OnUpdateScriptEnumeratetypeinfo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( m_ScriptSite.IsLoaded() );
}

BOOL CSEngineDoc::AddScriptSource( CString &fname )
{
	CString Context = "Document";

	if ( m_ScriptSite.LoadScript( fname, Context ) != S_OK)
	{
		int err = m_ScriptSite.GetLastError();
		CString strError = m_ScriptSite.strerror( err );
		UpdateAllViews(NULL, err, NULL );
		return FALSE;
	}
	return TRUE;
}
BOOL CSEngineDoc::AddScriptlet(CString &code, BOOL bSilent, CString *pstrErrorMsg)
{
	if ( IsModified() )
	{
		AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_SAVE );
		if ( IsModified() ) return FALSE;
	}
	if ( m_ScriptSite.InvokeScriptlet( code, CString("Document"), DEFAULT_TIMEOUT ) != S_OK )
	{
		int err = m_ScriptSite.GetLastError();
		CString strError = m_ScriptSite.strerror( err );
		if ( pstrErrorMsg )
		{
			*pstrErrorMsg = strError;
		}
		if ( !bSilent ) AfxMessageBox( strError );
		if ( err == E_RUNTIME )
		{
			CScriptSite::RunTimeErrorInfo rteInfo;
			if ( m_ScriptSite.GetRunTimeErrorInfo ( rteInfo ))
			{
				UpdateAllViews( NULL, E_RUNTIME, (CObject *) &rteInfo );
			}
			else
				UpdateAllViews(NULL, err, NULL );

		}
		else
		{
			UpdateAllViews(NULL, err, NULL );
		}
	}	
	return TRUE;
}

void CSEngineDoc::OnXObjectChanged() 
{
	// TODO: Add notification handler code

}

void CSEngineDoc::DoMessageBox(LPCTSTR msg) 
{
	// TODO: Add your dispatch handler code here
	CString strMsg = msg;
	AfxMessageBox( strMsg );
}


LPDISPATCH CSEngineDoc::CreateNewX() 
{
	// TODO: Add your dispatch handler code here
	LPDISPATCH pDisp = NULL;
	CLSID  clsid;
	if (CLSIDFromProgID(CComBSTR("ALEXDUAL.AlexDualObject.1"), &clsid ) ==S_OK )
		CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_IDispatch, (void **)&pDisp);
	 
	return pDisp;
}

LPDISPATCH CSEngineDoc::CreateOLEObject(LPCTSTR lpzClassName) 
{
	// TODO: Add your dispatch handler code here

	LPDISPATCH pDisp = NULL;
	CLSID  clsid;

	if (CLSIDFromProgID(CComBSTR(lpzClassName), &clsid ) ==S_OK )
		CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_IDispatch, (void **)&pDisp);

	return pDisp;
}



void CSEngineDoc::SetStatusText( CString text )
{
	m_statusText = text;
	OnStatusTextChanged();

}

SCODE CSEngineDoc::oleInvokeScriptlet(LPCTSTR strScript) 
{
	// TODO: Add your dispatch handler code here
	AddScriptlet( CString(strScript) );
	return S_OK;
}

LPDISPATCH CSEngineDoc::GetClientObject()
{
	return m_xObject;
}

void CSEngineDoc::CreateOCXView(LPCTSTR progid) 
{
	// TODO: Add your dispatch handler code here
	CMDIFrameWnd * pMainFrame = (CMDIFrameWnd *)AfxGetMainWnd();

	CMDIChildWnd* pActiveChild = pMainFrame->MDIGetActive();

	CView* pView = NULL;

	CDocTemplate* pTemplate = GetDocTemplate();
	ASSERT_VALID(pTemplate);
	//CFrameWnd* pFrame = pTemplate->CreateNewFrame(pDocument, pActiveChild);
	CCreateContext context;
	context.m_pCurrentFrame = pActiveChild;
	context.m_pCurrentDoc =  this;
	context.m_pNewViewClass = RUNTIME_CLASS( COCXView );
	context.m_pNewDocTemplate = pTemplate;

	
	CFrameWnd* pFrame = new CChildFrame; // custom MDI child frame
		
	ASSERT_KINDOF(CFrameWnd, pFrame);

	if (context.m_pNewViewClass == NULL)
			TRACE0("Warning: creating frame with no default view.\n");

		// create new from resource
	if (!pFrame->LoadFrame(IDR_MAINFRAME,
			WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,   // default frame styles
			pMainFrame, &context))
	{
			TRACE0("Warning: CDocTemplate couldn't create a frame.\n");
			// frame will be deleted in PostNcDestroy cleanup
			
	}

	if (pFrame == NULL)
	{
		TRACE0("Warning: failed to create new frame.\n");
		return;     // command failed
	}

	pTemplate->InitialUpdateFrame(pFrame, this);
	pView = pFrame->GetActiveView();
	ASSERT ( pView );
	ASSERT ( pView->IsKindOf( RUNTIME_CLASS( COCXView)));
	((COCXView *) pView)->SetClient(progid);
}

LPDISPATCH CSEngineDoc::CreateOCX(LPCTSTR progid) 
{
	// TODO: Add your dispatch handler code here
	CMDIFrameWnd * pMainFrame = (CMDIFrameWnd *)AfxGetMainWnd();

	CMDIChildWnd* pActiveChild = pMainFrame->MDIGetActive();

	CView* pView = NULL;

	CDocTemplate* pTemplate = GetDocTemplate();
	ASSERT_VALID(pTemplate);
	//CFrameWnd* pFrame = pTemplate->CreateNewFrame(pDocument, pActiveChild);
	CCreateContext context;
	context.m_pCurrentFrame = pActiveChild;
	context.m_pCurrentDoc =  this;
	context.m_pNewViewClass = RUNTIME_CLASS( COCXView );
	context.m_pNewDocTemplate = pTemplate;

	
	CFrameWnd* pFrame = new CChildFrame; // custom MDI child frame
		
	ASSERT_KINDOF(CFrameWnd, pFrame);

	if (context.m_pNewViewClass == NULL)
			TRACE0("Warning: creating frame with no default view.\n");

		// create new from resource
	if (!pFrame->LoadFrame(IDR_MAINFRAME,
			WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,   // default frame styles
			pMainFrame, &context))
	{
			TRACE0("Warning: CDocTemplate couldn't create a frame.\n");
			// frame will be deleted in PostNcDestroy cleanup
			
	}

	if (pFrame == NULL)
	{
		TRACE0("Warning: failed to create new frame.\n");
		return NULL;     // command failed
	}

	pTemplate->InitialUpdateFrame(pFrame, this);
	pView = pFrame->GetActiveView();
	ASSERT ( pView );
	ASSERT ( pView->IsKindOf( RUNTIME_CLASS( COCXView)));
	((COCXView *) pView)->SetClient(progid);

	IUnknown * punk = ((COCXView *) pView)->m_ClientCtrl.GetControlUnknown( );
	ASSERT ( punk );
	IDispatch * pdisp = NULL;
	if ( FAILED( punk->QueryInterface( IID_IDispatch, (void **)&pdisp )))
		return NULL;

	return pdisp;
}

void CSEngineDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	// Can only safe if we have our edit 
	CView * pView = (CView *) m_viewList.GetHead();
	if ( pView->IsKindOf( RUNTIME_CLASS( CEditView)))
			pCmdUI->Enable( TRUE );
	else
			pCmdUI->Enable( FALSE );

}

void CSEngineDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
		CView * pView = (CView *) m_viewList.GetHead();
	if ( pView->IsKindOf( RUNTIME_CLASS( CEditView)))
			pCmdUI->Enable( TRUE );
	else
			pCmdUI->Enable( FALSE );
}

// Client events implementation
STDMETHODIMP_(ULONG) CSEngineDoc::XClientSite::AddRef()
{
	METHOD_PROLOGUE(CSEngineDoc, ClientSite)
	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CSEngineDoc::XClientSite::Release()
{
	METHOD_PROLOGUE(CSEngineDoc, ClientSite)
	return pThis->ExternalRelease();
}

STDMETHODIMP CSEngineDoc::XClientSite::QueryInterface(
    REFIID iid, void FAR* FAR* ppvObj)
{
	METHOD_PROLOGUE(CSEngineDoc, ClientSite)
    return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CSEngineDoc::XClientSite::OnConnect(IWorld* pWorld)
{
	METHOD_PROLOGUE(CSEngineDoc, ClientSite)
	return S_OK;
}

STDMETHODIMP CSEngineDoc::XClientSite::OnUserReconnect(IVWClient* newClient,
			IWorld* pWorld,
			IThing* pthing)
{
	METHOD_PROLOGUE(CSEngineDoc, ClientSite)
	pThis->OnUserReconnect(newClient,pWorld,pthing);
	return S_OK;
}

STDMETHODIMP CSEngineDoc::XClientSite::OnDisconnect( IWorld* pWorld)
{
	METHOD_PROLOGUE(CSEngineDoc, ClientSite)
	return S_OK;
}

STDMETHODIMP CSEngineDoc::XClientSite::OnTrace(BSTR bstr)
{
	METHOD_PROLOGUE(CSEngineDoc, ClientSite)
	return S_OK;
}

STDMETHODIMP CSEngineDoc::XClientSite::OnReport(BSTR bstr, long lType)
{
	METHOD_PROLOGUE(CSEngineDoc, ClientSite)
	return S_OK;
}

STDMETHODIMP CSEngineDoc::XClientSite::OnUserConnect(IThing* pthing)
{
	METHOD_PROLOGUE(CSEngineDoc, ClientSite)
	return S_OK;
}

STDMETHODIMP CSEngineDoc::XClientSite::OnUserDisconnect(IThing* pthing)
{
	METHOD_PROLOGUE(CSEngineDoc, ClientSite)
	return S_OK;
}

STDMETHODIMP CSEngineDoc::XClientSite::OnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg)
{
	METHOD_PROLOGUE(CSEngineDoc, ClientSite)
	return S_OK;
}

// this is a IDispatch becuase MFC doesn't know about the world
void CSEngineDoc::OnUserReconnect(IVWClient* newClient,
			IWorld* pWorld,
			IThing* pthing) 
{
}

HRESULT CSEngineDoc::Advise( IUnknown *pUnk,REFIID riid, ULONG *pdwCookie )
{
	HRESULT hr = E_FAIL;
	IConnectionPoint* pconnpt = NULL;
	IConnectionPointContainer* pconnptctr = NULL;
	
//	TRACE("CSEngineDoc::Advise\n");
	hr = pUnk->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pconnptctr);
	if (FAILED(hr))
		goto errout;

	hr = pconnptctr->FindConnectionPoint(riid, &pconnpt);
	

	if (SUCCEEDED(hr))
	{
		hr=pconnpt->Advise(GetIDispatch( FALSE ) , pdwCookie);
		SAFERELEASE( pconnpt );
	}

errout:
	SAFERELEASE( pconnpt);
	SAFERELEASE( pconnptctr );
return hr;
}

HRESULT CSEngineDoc::UnAdvise( IUnknown *pUnk,REFIID riid, ULONG dwCookie )
{
HRESULT hr = E_FAIL;
IConnectionPoint* pconnpt = NULL;
IConnectionPointContainer* pconnptctr = NULL;
	
//	TRACE("CSEngineDoc::UnAdvise\n");
	hr = pUnk->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pconnptctr);
	if (FAILED(hr))
		goto errout;

	hr = pconnptctr->FindConnectionPoint(riid, &pconnpt);
	

	if (SUCCEEDED(hr))
	{
	
		pconnpt->Unadvise(  dwCookie );

		SAFERELEASE( pconnpt );
	}



errout:
	SAFERELEASE( pconnpt);
	SAFERELEASE( pconnptctr );
return hr;
}

