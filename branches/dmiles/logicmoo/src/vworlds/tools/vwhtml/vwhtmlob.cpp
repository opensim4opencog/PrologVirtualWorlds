// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWHtmlOb.cpp : Implementation of CVWHtmlObjectApp and DLL registration.

#include "stdafx.h"
#include <io.h>
#include <urlmon.h>
#include <VWHtml.h>
#include "VWHtmlOb.h"
#include "reghelp.h"
#include "urlhelp.h"

/////////////////////////////////////////////////////////////////////////////
// Macros and parameters

// temp file for html
#define szTempFilePrefix "VW"
#define szTempFileSuffix ".htm"

/////////////////////////////////////////////////////////////////////////////
// ErrorInfo stuff

STDMETHODIMP CVWHtmlObject::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IVWHtml,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

static HRESULT FindPathInRegistry(CString &path)
{
	CComBSTR	bstrPath;
	HRESULT		hr;

	hr = FindContentPath(&bstrPath.m_str);
	
	if (SUCCEEDED(hr))
		path = bstrPath;

	return hr;
}

/////////////////////////////////////////////////////////////////////////////
//

CVWHtmlObject::CVWHtmlObject() :
	m_pclient(NULL)
{
	CString strContentPath;
	CString strRootURL;

	// figure out what drive to stick the HTML into
	char szSysDir[_MAX_PATH];
	char szDrive[_MAX_DRIVE];
	HRESULT hr=S_OK;

	hr = FindPathInRegistry(strContentPath);
	if( SUCCEEDED(hr) &&
		_access(strContentPath, 0) == 0) 
	{
		strRootURL = "file://"+strContentPath;
		put_RootURL(CComBSTR(
			strRootURL
		));
	}
	else
	{
		VERIFY( ::GetSystemDirectory(szSysDir, _MAX_PATH) > 0 );
		_splitpath( szSysDir, szDrive, NULL, NULL, NULL);

		// compose cache path
		strContentPath = szDrive;
		strContentPath += "\\HTML";

		strRootURL = "file://"+strContentPath;
		put_RootURL(CComBSTR(
			strRootURL
			));
		
		// search path and download paths. REVIEW: HACK
		// create directory if necessary
		if (_access(strContentPath, 0) != 0) 
		{
			// create
			if (!CreateDirectory(strContentPath, NULL)) 
			{
				// REVIEW: error handling
				AfxMessageBox("Unable to create cache directory");
			}
		}
	}
}

CVWHtmlObject::~CVWHtmlObject()
{
	// close all windows
	CloseAll();

	SAFERELEASE(m_pclient);
}

STDMETHODIMP CVWHtmlObject::Initialize(IVWClient *pclient)
{
	SAFERELEASE(m_pclient);
	m_pclient = pclient;
	SAFEADDREF(pclient);

	return S_OK;
}

STDMETHODIMP CVWHtmlObject::Close(BSTR bstrName)
{
	CHtmlWnd *pwnd;

	// find the window to close
	if (!m_mapNameToWnd.Lookup(bstrName, pwnd))
		return E_INVALIDARG;

	ASSERT(pwnd);
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	pwnd->DestroyWindow();
	::CoFreeUnusedLibraries();

	// callback will remove it from the map

	return S_OK;
}

STDMETHODIMP CVWHtmlObject::CloseAll()
{
	HRESULT hr;
	POSITION pos;

	// iterate through map nuking all
	for (pos=m_mapNameToWnd.GetStartPosition(); pos!=NULL; )
	{
		CComBSTR bstrName;
		CHtmlWnd *pwnd;

		m_mapNameToWnd.GetNextAssoc(pos, (CBSTRAdapt<CComBSTR>&)bstrName, pwnd);

		ASSERT(bstrName != NULL);
		ASSERT(pwnd != NULL);

		hr = Close(bstrName);
		if (FAILED(hr))
			return hr;
	}

	// verify that count is down to zero

	// actually, don't for now, to account for the rare case where
	// a closing html window opens another one with showurlex via script
	// while we are still iterating through the map (as happens in the tutorial html)

	//VERIFY( m_mapNameToWnd.GetCount()==0 );

	return S_OK;
}

STDMETHODIMP CVWHtmlObject::ShowHTML(	int nWnd, 
										BSTR bstrHtml, 
										BSTR bstrTitle,
										VARIANT_BOOL bSafeMode,
										IDispatch *pdispArg,
										int x,
										int y,
										int cx,
										int cy )
{
	return ShowHTMLEx( CComBSTR("*"), nWnd, bstrHtml, bstrTitle, bSafeMode, pdispArg, x, y, cx, cy);
}

STDMETHODIMP CVWHtmlObject::ShowHTMLEx( BSTR bstrName,	
										int nWnd, 
										BSTR bstrHtml, 
										BSTR bstrTitle,
										VARIANT_BOOL bSafeMode,
										IDispatch *pdispArg,
										int x,
										int y,
										int cx,
										int cy )
{
	HRESULT hr = S_OK;
	CString strURL;
	CFile fi;
	CString strHelper;
	CHtmlWnd *pwnd;

	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

	// prep the window
	hr = PrepareWindow( bstrName, nWnd, bstrTitle, pdispArg, x, y, cx, cy, &pwnd );
	if (FAILED(hr))
		return hr;

	// if not safe mode, calculate the search parameter
	if (bSafeMode == VARIANT_FALSE)
	{
		strHelper.Format("#%0X_%0X", (int) m_pclient, (int) pdispArg);
	}

	// calculate temporary file name

	// first get the temp file path
	char szTempPath[_MAX_PATH];
	char szPath[_MAX_PATH];

	VERIFY(::GetTempPath(_MAX_PATH, szTempPath) > 0);

	VERIFY(::GetTempFileName(szTempPath, szTempFilePrefix, 0, szPath) > 0);

	// Windows actually creates/opens the temporary file, so we need to delete it
	::DeleteFile(szPath);

	CString strPath(szPath);
	if (strPath.Replace(".TMP", szTempFileSuffix) == 0)
		strPath.Replace(".tmp", szTempFileSuffix);

	// open the file
	VERIFY(fi.Open(strPath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite | CFile::typeBinary));

	// set the temporary file name
	pwnd->SetTempFile(strPath);

	// write the header
	CString strHeader("<HTML>\r\n<HEAD>\r\n");
	fi.Write(strHeader, strHeader.GetLength());

	//write out the root url (must have trailing slash)
	CString strBaseURL = CString("<BASE HREF=\"") + m_strRootURL + CString("\\\">\r\n");
	fi.Write(strBaseURL, strBaseURL.GetLength());

	// if not safe mode, write out code to set the search parameter
	if (bSafeMode == VARIANT_FALSE)
	{
		// set the search parameter of the container
		CString strSetSearchPre("<SCRIPT LANGUAGE=\"VBScript\">\r\n"
								"function GetClientArg\r\n"
								"  GetClientArg = \"");
		CString strSetSearchPost("\"\r\n"
								 "end function\r\n"
								 "</SCRIPT>\r\n");

		fi.Write(strSetSearchPre, strSetSearchPre.GetLength());
		fi.Write(strHelper, strHelper.GetLength());
		fi.Write(strSetSearchPost, strSetSearchPost.GetLength());
	}

	// write the footer
	CString strFooter("</HEAD>\r\n");
	fi.Write(strFooter, strFooter.GetLength());

	// write the html
	CString str(bstrHtml);
	fi.Write( str, str.GetLength() );

	// write the trailer
	CString strTrailer("</HTML>\r\n");
	fi.Write(strTrailer, strTrailer.GetLength());

	// close
	fi.Close();

	// calculate file URL
	strURL = "file://" + strPath;

	// go there
	pwnd->Navigate(strURL);

	return S_OK;
}

STDMETHODIMP CVWHtmlObject::ShowURL(		int nWnd, 
											BSTR bstrURL, 
											BSTR bstrTitle,
											VARIANT_BOOL bSafeMode,
											IDispatch *pdispArg,
											int x,
											int y,
											int cx,
											int cy )
{
	return ShowURLEx( CComBSTR("*"), nWnd, bstrURL, bstrTitle, bSafeMode, pdispArg, x, y, cx, cy);
}

STDMETHODIMP CVWHtmlObject::ShowURLEx(		BSTR bstrName,	
											int nWnd, 
											BSTR bstrURL, 
											BSTR bstrTitle,
											VARIANT_BOOL bSafeMode,
											IDispatch *pdispArg,
											int x,
											int y,
											int cx,
											int cy )
{
	HRESULT hr = S_OK;

	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

	if (bSafeMode == VARIANT_TRUE)
	{
		CHtmlWnd *pwnd;
		CString strURL(bstrURL);

		// if not full URL, prepend root URL
		if (!IsFullURL(bstrURL))
			strURL = m_strRootURL + "/" + strURL;

		hr = PrepareWindow( bstrName, nWnd, bstrTitle, pdispArg, x, y, cx, cy, &pwnd );
		if (FAILED(hr))
			return hr;

		pwnd->Navigate( strURL );
	}
	else
	{
		// Otherwise, create a wrapper HTML page to pass the client/arg pointers through
		CComBSTR bstrHtml;
		
		bstrHtml =	"<SCRIPT LANGUAGE=\"VBScript\">\r\n";
		bstrHtml +=	"document.write \"<FRAMESET><FRAME FRAMEBORDER=0 SCROLLING=NO SRC=\"\"";
		bstrHtml +=	bstrURL;
		bstrHtml +=	"\"\r\n";
		bstrHtml +=	"document.write GetClientArg()\r\n";
		bstrHtml +=	"document.write \"\"\">";
		bstrHtml +=	"</FRAMESET>\"\r\n";
		bstrHtml +=	"</SCRIPT>\r\n";

		hr = ShowHTMLEx(bstrName, nWnd, bstrHtml, bstrTitle, bSafeMode, pdispArg, x, y, cx, cy);
	}

	return S_OK;
}

HRESULT CVWHtmlObject::PrepareWindow(	BSTR bstrName, 
										int nWnd,
										BSTR bstrTitle,
										IDispatch *pdispArg,
										int x,
										int y,
										int cx,
										int cy,
										CHtmlWnd **ppwnd)
{
	HRESULT hr = S_OK;
	CComBSTR bstrFinalName = bstrName;
	CHtmlWnd *pwndHtml;
	CWnd *pwndParent;
	HWND hwnd = (HWND) (void *) nWnd;
	CString strTitle(bstrTitle);

	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

	if (ppwnd == NULL)
		return E_POINTER;

	// if we want a new window, make up a new name
	if (CString(bstrName).CompareNoCase("_blank") == 0)
	{
		CString strBlankName;

		// Try a lot of different names (if nothing's unique, just use the last gen'd one)
		for (int i = 0; i < 1000; i++)
		{
			strBlankName.Format("_blank_%d", i);
			bstrFinalName = strBlankName;
			if (!m_mapNameToWnd.Lookup(bstrFinalName, pwndHtml))
				break;
		}
	}

	// find the named window
	if (!m_mapNameToWnd.Lookup(bstrFinalName, pwndHtml))
	{
		// initialize m_pwnd
		pwndHtml = new CHtmlWnd(bstrFinalName, this);
		if (pwndHtml == NULL)
		{
			ASSERT(FALSE);
			return E_FAIL;
		}

		// if hwnd is null, use the currently active window
		if (hwnd == NULL)
		{
 			// first, get the active window
			pwndParent = CWnd::GetActiveWindow();

			// REVIEW: if this is null, what's the right thing to do?
			if (pwndParent == NULL)
				pwndParent = CWnd::GetDesktopWindow();
			else
				// find the topmost parent
				pwndParent = pwndParent->GetTopLevelParent();

			// now grab the handle from this
			hwnd = pwndParent->m_hWnd;
		}

		pwndParent = CWnd::FromHandle(hwnd);

		// make origin parent relative
		CPoint pt(x, y);
		pwndParent->ClientToScreen(&pt);

		// open the window
		DWORD dwStyle;
		CRect rect( pt.x, pt.y, pt.x + cx, pt.y + cy );
		dwStyle = MFS_SYNCACTIVE | MFS_THICKFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU;
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		VERIFY( pwndHtml->Create(	NULL, 
									strTitle,
									dwStyle,
									rect,
									pwndParent,
									0 ));

		// now activate the window
		pwndHtml->ShowWindow( SW_SHOW );

		// stick it in the map
		m_mapNameToWnd[bstrFinalName] = pwndHtml;

	}
	else
	{
		// change the title
		pwndHtml->SetWindowText(strTitle);

		// deref and remove the old arg
		IDispatch *pdispOldArg;

		if (m_mapNameToArg.Lookup(bstrFinalName, pdispOldArg))
		{
			SAFERELEASE(pdispOldArg);
			m_mapNameToArg.RemoveKey(bstrFinalName);
		}

		// bring it to the front..
		pwndHtml->ShowWindow(SW_SHOW);
		pwndHtml->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}

	// addref and save the arg
	if (pdispArg)
	{
		SAFEADDREF(pdispArg);
		m_mapNameToArg[bstrFinalName] = pdispArg;
	}

	// return window handle
	*ppwnd = pwndHtml;

	return S_OK;
}

// called by htmlwnd on close
void CVWHtmlObject::OnHTMLClosed(BSTR bstrName)
{
	IDispatch *pdispArg;

	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

	// zap pointer
	m_mapNameToWnd.RemoveKey(bstrName);
	// and deref arg
	if (m_mapNameToArg.Lookup(bstrName, pdispArg))
	{
		SAFERELEASE(pdispArg);
		m_mapNameToArg.RemoveKey(bstrName);
	}
	::CoFreeUnusedLibraries();
}

STDMETHODIMP CVWHtmlObject::get_RootURL(BSTR* pbstrRootURL)
{
	if (pbstrRootURL == NULL)
		return E_POINTER;

	*pbstrRootURL = m_strRootURL.AllocSysString();

	return S_OK;
}

STDMETHODIMP CVWHtmlObject::put_RootURL(BSTR bstrRootURL)
{
	if (bstrRootURL == NULL)
		return E_INVALIDARG;

	ExtractFirstURL(CString(bstrRootURL), m_strRootURL);

	return S_OK;
}
