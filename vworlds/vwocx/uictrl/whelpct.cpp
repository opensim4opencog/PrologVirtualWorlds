// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// AvListCt.cpp : Implementation of the CAvatarListCtrl ActiveX Control class.

#include "stdafx.h"
#include <propbase.h>
#include <vwclient.h>
#include "uictrl.h"
#include "WHelpCt.h"
#include <afxtempl.h>
//#include <afx.h>
//#include <afx.h>
#include <afxwin.h>
#include <afxinet.h>
#include "cutils.h"
#include "syshelp.h"

//#include <iostream.h>
//#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CWebBrowserHelperCtrl, CVWViewCtrl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CWebBrowserHelperCtrl, CVWViewCtrl)
	//{{AFX_MSG_MAP(CWebBrowserHelperCtrl)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(WM_USER, (WM_USER+200), OnCommandRange)
	ON_UPDATE_COMMAND_UI_RANGE(WM_USER, (WM_USER+200), OnCommandRangeUI)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CWebBrowserHelperCtrl, CVWViewCtrl)
	//{{AFX_DISPATCH_MAP(CWebBrowserHelperCtrl)
	DISP_FUNCTION(CWebBrowserHelperCtrl, "HideBrowserAdornments", HideBrowserAdornments, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CWebBrowserHelperCtrl, "RestoreBrowserAdornments", RestoreBrowserAdornments, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CWebBrowserHelperCtrl, "FetchURLText", FetchURLText, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CWebBrowserHelperCtrl, "PostForm", PostForm, VT_BSTR, VTS_BSTR VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CWebBrowserHelperCtrl, "TrackThingMenu", TrackThingMenu, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION(CWebBrowserHelperCtrl, "GetSubFolderNames", GetSubFolderNames, VT_DISPATCH, VTS_BSTR)
	DISP_FUNCTION(CWebBrowserHelperCtrl, "GetFileNames", GetFileNames, VT_DISPATCH, VTS_BSTR VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CWebBrowserHelperCtrl, CVWViewCtrl)
	//{{AFX_EVENT_MAP(CWebBrowserHelperCtrl)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CWebBrowserHelperCtrl, 2)
	PROPPAGEID( CLSID_CFontPropPage )
    PROPPAGEID( CLSID_CColorPropPage )
END_PROPPAGEIDS(CWebBrowserHelperCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CWebBrowserHelperCtrl, "VWCLIENTUI.WebBrowserHelperCtrl.1",
	0x7886A636, 0x606B, 0x11d1, 0xbb, 0x36, 0, 0xc0, 0x4f, 0xc2, 0xca, 0xb1)

/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CWebBrowserHelperCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DWebBrowserHelper =
		{ 0x7886A634, 0x606B, 0x11d1, { 0xbb, 0x36, 0, 0xc0, 0x4f, 0xc2, 0xca, 0xb1 } };
const IID BASED_CODE IID_DWebBrowserHelperEvents =
		{ 0x7886A635, 0x606B, 0x11d1, { 0xbb, 0x36, 0, 0xc0, 0x4f, 0xc2, 0xca, 0xb1 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwWebBrowserHelperOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
//	OLEMISC_INVISIBLEATRUNTIME |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT | OLEMISC_NOUIACTIVATE |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CWebBrowserHelperCtrl, IDS_WEBBROWSERHELPER, _dwWebBrowserHelperOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CWebBrowserHelperCtrl::CWebBrowserHelperCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CWebBrowserHelperCtrl
BOOL CWebBrowserHelperCtrl::CWebBrowserHelperCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
	{
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_WEBBROWSERHELPER,
			IDB_WEBBROWSERHELPER,
			afxRegApartmentThreading,
			_dwWebBrowserHelperOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	}
	else
	{
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CWebBrowserHelperCtrl::CWebBrowserHelperCtrl - Constructor

CWebBrowserHelperCtrl::CWebBrowserHelperCtrl()
{
	InitializeIIDs(&IID_DWebBrowserHelper, &IID_DWebBrowserHelperEvents);
	m_bStatusSaved	= FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CWebBrowserHelperCtrl::~CWebBrowserHelperCtrl - Destructor

CWebBrowserHelperCtrl::~CWebBrowserHelperCtrl()
{
	m_Menu.DestroyMenu();
}


/////////////////////////////////////////////////////////////////////////////
// CWebBrowserHelperCtrl::OnDraw - Drawing function

void CWebBrowserHelperCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
}


/////////////////////////////////////////////////////////////////////////////
// CWebBrowserHelperCtrl::DoPropExchange - Persistence support

void CWebBrowserHelperCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	CVWViewCtrl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CWebBrowserHelperCtrl::OnResetState - Reset control to default state

void CWebBrowserHelperCtrl::OnResetState()
{
	CVWViewCtrl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


void CWebBrowserHelperCtrl::OnCommandRange(UINT nID)
{
	HRESULT hr = S_OK;
	m_Menu.OnCommandRange(nID, &hr);
		
}


void CWebBrowserHelperCtrl::OnCommandRangeUI(CCmdUI* pCmdUI)
{

	HRESULT hr;
	m_Menu.OnCommandRangeUI( pCmdUI, &hr );
}

#include <exdisp.h>
// some GUIDs
const GUID SID_STopLevelBrowser = { 0x4C96BE40L, 0x915C, 0x11CF, { 0x99, 0xD3, 0x00, 0xAA, 0x00, 0x4A, 0xE8, 0x37 } };
const GUID IID_IWebBrowserApp = { 0x0002DF05, 0x0000,0x0000, { 0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46 } };

//#include <shlguid.h>

void CWebBrowserHelperCtrl::HideBrowserAdornments() 
{
    IServiceProvider	*psp1 = NULL, *psp2 = NULL;
    IWebBrowserApp		*pWB = NULL;
	IOleClientSite		*pOleClientSite	= NULL;

	pOleClientSite = GetClientSite();

	if (!pOleClientSite)
		return;

    if (SUCCEEDED(pOleClientSite->QueryInterface(IID_IServiceProvider, (void **)&psp1) ))
    {
        if (SUCCEEDED(psp1->QueryService(SID_STopLevelBrowser, IID_IServiceProvider, (void **)&psp2) ))
        {
            if (SUCCEEDED(psp2->QueryService(IID_IWebBrowserApp, IID_IWebBrowserApp,
                                            (void **)&pWB) ))
            {
				pWB->get_ToolBar(&m_iSavedToolBar);
                pWB->get_MenuBar(&m_bSavedMenuBar);

                pWB->put_ToolBar(FALSE);
                pWB->put_MenuBar(VARIANT_FALSE);
				m_bStatusSaved = TRUE;
				pWB->Release();
           }
            psp2->Release();
        }
        psp1->Release();
    }
}

void CWebBrowserHelperCtrl::RestoreBrowserAdornments() 
{
	if (!m_bStatusSaved)
		return;

    IServiceProvider	*psp1 = NULL, *psp2 = NULL;
    IWebBrowserApp		*pWB = NULL;
	IOleClientSite		*pOleClientSite	= NULL;

	pOleClientSite = GetClientSite();

	if (!pOleClientSite)
		return;

    if (SUCCEEDED(pOleClientSite->QueryInterface(IID_IServiceProvider, (void **)&psp1) ))
    {
        if (SUCCEEDED(psp1->QueryService(SID_STopLevelBrowser, IID_IServiceProvider, (void **)&psp2) ))
        {
            if (SUCCEEDED(psp2->QueryService(IID_IWebBrowserApp, IID_IWebBrowserApp,
                                            (void **)&pWB) ))
            {
                pWB->put_MenuBar(m_bSavedMenuBar);
                pWB->put_ToolBar(m_iSavedToolBar);
                pWB->Release();
				m_bStatusSaved = FALSE;
            }
            psp2->Release();
        }
        psp1->Release();
    }
}

//******************************************************
// FetchURLText
// Given an URL, fetch its HTML as a string....

#include <wininet.h>

BSTR CWebBrowserHelperCtrl::FetchURLText(LPCTSTR strURL) 
{
	CComBSTR	bstrResult = "no data found";
	HINTERNET	hUrlFile;
	HINTERNET	hInetSession;
	
	// check for file URL
	if (_strnicmp( strURL, "file://", 7 ) == 0)
	{
		CString		strData;
		char		*strBuffer;
		DWORD		dwBytesRead = 0;
		DWORD		dwBufSize;
		CFile		theFile;
		char		*realFileName = (char *)strURL + 7;

		if (theFile.Open(realFileName, CFile::modeRead))
		{
			dwBufSize = theFile.GetLength() + 1;
			strBuffer = strData.GetBuffer(dwBufSize);
			memset(strBuffer, 0x00, dwBufSize);
			theFile.Read(strBuffer, dwBufSize-1);
			strData.ReleaseBuffer();
			bstrResult = strData;
			theFile.Close();
		}
	}
	else
	{
		// try to use Inet features
		hInetSession = InternetOpen("Microsoft Virtual Worlds", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		if (hInetSession)
		{
			hUrlFile = InternetOpenUrl(hInetSession, strURL, NULL, -1, INTERNET_FLAG_NO_CACHE_WRITE, 0);
			
			if (hUrlFile)
			{
				CString		strData;
				DWORD		dwBytesRead = 0;
				DWORD		dwBufSize = 16000;
				char		strBuffer[16000];

				strData = "";
				memset(strBuffer, 0x00, dwBufSize);

				do
				{
					if (InternetReadFile(hUrlFile, strBuffer, dwBufSize, &dwBytesRead))
					{
						if (dwBytesRead)
						{
							strBuffer[dwBytesRead] = 0x00;
							strData += strBuffer;
							memset(strBuffer, 0x00, dwBufSize);
						}	
					}	
					else
						dwBytesRead = 0;
				}
				while 
					(dwBytesRead);

				bstrResult = strData;
				InternetCloseHandle(hUrlFile);
			}

			InternetCloseHandle(hInetSession);
		}
	}
	return bstrResult.Copy();
}






// StripTags() rips through a buffer and removes HTML tags from it.
// The function uses a static variable to remember its state in case
// a HTML tag spans a buffer boundary.

void StripTags(LPTSTR pszBuffer)
{
	static BOOL bInTag = FALSE;
	LPTSTR pszSource = pszBuffer;
	LPTSTR pszDest = pszBuffer;

	while (*pszSource != '\0')
	{
		if (bInTag)
		{
			if (*pszSource == '>')
				bInTag = FALSE;
			pszSource++;

			if (*pszSource == '\n')
				*pszSource++;
		}
		else
		{
			if (*pszSource == '<')
				bInTag = TRUE;
			else
			{
				*pszDest = *pszSource;
				pszDest++;
			}
			pszSource++;
		}
	}
	*pszDest = '\0';
}

BOOL CWebBrowserHelperCtrl::IsInvokeAllowed(DISPID)
{
	return true;
}


BSTR CWebBrowserHelperCtrl::PostForm(LPCTSTR URL, LPCTSTR Data, BOOL bFilter) 
{
	CString strResult;
	LPCTSTR pszURL = URL;
	BOOL    bStripMode = bFilter;

	DWORD   dwAccessType = PRE_CONFIG_INTERNET_ACCESS;

	DWORD dwHttpRequestFlags =
		INTERNET_FLAG_EXISTING_CONNECT;

	const TCHAR szHeaders[] =
		_T("Accept: text/*\r\nUser-Agent: WebHelper\r\n");

	int nRetCode = 0;

	CInternetSession session(_T("WebHelper"), dwAccessType);
	CHttpConnection* pServer = NULL;
	CHttpFile* pFile = NULL;
	
	// check to see if this is a reasonable URL

	CString strServerName;
	CString strNewLocation;
	CString strObject;
	INTERNET_PORT nPort;
	DWORD dwServiceType;

	if (!AfxParseURL(pszURL, dwServiceType, strServerName, strObject, nPort) ||
		dwServiceType != INTERNET_SERVICE_HTTP)
	{
		strResult = CString("Error: can only use URLs beginning with http://");
		goto ERROR_ENCOUNTERED;
	}


	pServer = session.GetHttpConnection(strServerName, nPort);

	pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST,
		strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
	pFile->AddRequestHeaders(szHeaders);
	pFile->SendRequest(CString(""), (void*)Data, strlen(Data) );

	DWORD dwRet;
	pFile->QueryInfoStatusCode(dwRet);

	// if access was denied, prompt the user for the password

	if (dwRet == HTTP_STATUS_DENIED)
	{
		DWORD dwPrompt;
		dwPrompt = pFile->ErrorDlg(NULL, ERROR_INTERNET_INCORRECT_PASSWORD,
			FLAGS_ERROR_UI_FLAGS_GENERATE_DATA | FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS, NULL);

		// if the user cancelled the dialog, bail out

		if (dwPrompt != ERROR_INTERNET_FORCE_RETRY)
		{
			strResult = CString("Access denied: Invalid password");
			goto ERROR_ENCOUNTERED;
		}

		pFile->SendRequest();
		pFile->QueryInfoStatusCode(dwRet);
	}


	pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, strNewLocation);

	// were we redirected?
	// these response status codes come from WININET.H

	if (dwRet == HTTP_STATUS_MOVED ||
		dwRet == HTTP_STATUS_REDIRECT ||
		dwRet == HTTP_STATUS_REDIRECT_METHOD)
	{
		CString strNewLocation;
		pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, strNewLocation);

		int nPlace = strNewLocation.Find(_T("Location: "));
		if (nPlace == -1)
		{
			strResult = CString("Error: Site redirects with no new location");
			goto ERROR_ENCOUNTERED;
		}

		strNewLocation = strNewLocation.Mid(nPlace + 10);
		nPlace = strNewLocation.Find('\n');
		if (nPlace > 0)
			strNewLocation = strNewLocation.Left(nPlace);

		// close up the redirected site

		pFile->Close();
		delete pFile; pFile = NULL;
		pServer->Close();
		delete pServer; pServer = NULL;


		// figure out what the old place was
		if (!AfxParseURL(strNewLocation, dwServiceType, strServerName, strObject, nPort))
		{
			strResult = CString("Error: the redirected URL could not be parsed.");
			goto ERROR_ENCOUNTERED;
		}

		if (dwServiceType != INTERNET_SERVICE_HTTP)
		{
			strResult = CString("Error: the redirected URL does not reference a HTTP resource.");
			goto ERROR_ENCOUNTERED;
		}

		// try again at the new location
		pServer = session.GetHttpConnection(strServerName, nPort);
		pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,
			strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
		pFile->AddRequestHeaders(szHeaders);
		pFile->SendRequest();

		pFile->QueryInfoStatusCode(dwRet);
		if (dwRet != HTTP_STATUS_OK)
		{
			strResult = CString("Error: Got status code ");
			goto ERROR_ENCOUNTERED;
		}
	
	}

	TCHAR sz[1024];
	while (pFile->ReadString(sz, 1022))
	{
		sz[1023] = '\0'; //force a termination
		if (bStripMode)
			StripTags(sz);
		CString str(sz); 
		strResult += str; 
	}
	
ERROR_ENCOUNTERED:
	
	if (pFile != NULL)
	{
		pFile->Close();
		delete pFile;
	}
	if (pServer != NULL)
	{
		pServer->Close();
		delete pServer;
	}
	session.Close();



	return strResult.AllocSysString();
}



//******************************************************
// TrackThingMenu
// Tracks a generic IThing Menu

void CWebBrowserHelperCtrl::TrackThingMenu(LPDISPATCH pDispatch) 
{
	HRESULT		hr		= S_OK;
	IThing		*pThing = (IThing *)pDispatch;
	ASSERT ( pThing != NULL);
	POINT		ptCursor;

	// if menu existed previously, clear it
	m_Menu.DestroyMenu();

	::GetCursorPos(&ptCursor);

	// get the menu
	CComVariant varResult;
	hr = pThing->InvokeMethodExt(CComBSTR("GetMenuList"), NULL, &varResult);
	if (SUCCEEDED(hr))
	{
		CPropertyListPtr ListPtr = varResult;
		if (ListPtr != NULL )
		{
			// build the menu
			UINT wParam = WM_USER;
			hr = m_Menu.CreatePopupFromPropList(ListPtr, &wParam);
			if (SUCCEEDED(hr))
			{
				VERIFY(m_Menu.TrackPopupMenu(TPM_LEFTALIGN | /* TPM_LEFTBUTTON | */ TPM_RIGHTBUTTON, ptCursor.x, ptCursor.y, this));
			}
		}
	}
}

LPDISPATCH CWebBrowserHelperCtrl::GetSubFolderNames(LPCTSTR strRootPath) 
{
	IPropertyList	*pNewList = NULL;
	CFileFind		finder;
    BOOL			bWorking;
	HRESULT			hr = S_OK;

	hr = ::CreatePropertyList(m_pWorld, &pNewList);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// Now, add the contents of the directory
    bWorking = finder.FindFile(strRootPath);
    while (bWorking)
    {
        bWorking = finder.FindNextFile();
        if (finder.IsDirectory())
		{
			CString	newName = finder.GetFileName();
			pNewList->AddString(newName.AllocSysString());
		}
    }


ERROR_ENCOUNTERED:
	return pNewList;
}

LPDISPATCH CWebBrowserHelperCtrl::GetFileNames(LPCTSTR strRootPath, LPCTSTR strWildCard) 
{
	IPropertyList	*pNewList = NULL;
	CFileFind		finder;
    BOOL			bWorking;
	HRESULT			hr = S_OK;
	CStringArray	strWildcards;
	CString			strTemp = strWildCard;
	CString			strCurrent;
	BOOL			bDone = FALSE;
	int				iPos;

	while (!bDone)
	{
		iPos = strTemp.Find(",");
		if (iPos != -1)
		{
			// got the first comma
			strCurrent = strTemp.Left(iPos);
			
			// REVIEW: is an empty search string valid?
			strTemp = strTemp.Right(strTemp.GetLength() - iPos - 1);

			bDone = strTemp.IsEmpty();
		}
		else
		{
			strCurrent = strTemp;
			bDone = TRUE;
		}

		// add strCurrent to the list
		strWildcards.Add(strCurrent);
	}

	hr = ::CreatePropertyList(m_pWorld, &pNewList);
	if (FAILED(hr) || !pNewList)
		goto ERROR_ENCOUNTERED;

	for (iPos = 0; iPos <= strWildcards.GetUpperBound(); iPos++)
	{

		CString	strSearch = (CString)strRootPath + strWildcards.GetAt(iPos);

		// Now, add the contents of the directory
		bWorking = finder.FindFile(strSearch);
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			if (!finder.IsDirectory())
			{
				CString	newName = finder.GetFileName();
				pNewList->AddString(newName.AllocSysString());
			}
		}
	}

ERROR_ENCOUNTERED:
	return pNewList;
}
