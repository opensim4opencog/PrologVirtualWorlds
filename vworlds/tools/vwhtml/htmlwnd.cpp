// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// HtmlWnd.cpp : implementation file
//

#include "stdafx.h"
#include <vwhtml.h>
#include "vwhtmlob.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_WEBBROWSER	1

/////////////////////////////////////////////////////////////////////////////
// CHtmlWnd

CHtmlWnd::CHtmlWnd(BSTR bstrName, CVWHtmlObject *phtml)
{
	m_bstrName = bstrName;
	m_phtml = phtml;
}

CHtmlWnd::~CHtmlWnd()
{
}


BEGIN_MESSAGE_MAP(CHtmlWnd, CMiniFrameWnd)
	//{{AFX_MSG_MAP(CHtmlWnd)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_PARENTNOTIFY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

HRESULT CHtmlWnd::SetTempFile( const CString& strTempFile )
{
	m_strTempFile = strTempFile;
	return S_OK;
}

HRESULT CHtmlWnd::Navigate( CString& strURL )
{
	COleVariant varFlags, varTargetFrameName, varPostData, varHeaders;
	m_wb.Navigate(	strURL,
					&varFlags,
					&varTargetFrameName,
					&varPostData,
					&varHeaders);
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// CHtmlWnd message handlers

void CHtmlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CMiniFrameWnd::OnSize(nType, cx, cy);
	if (m_wb.m_hWnd != NULL)
	{
		m_wb.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER );
		m_wb.SetHeight(cy);
		m_wb.SetWidth(cx);
	}
}

int CHtmlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMiniFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create html control
	DWORD dwStyle = WS_VISIBLE;
	CRect rect(0, 0, 1, 1);
	VERIFY( m_wb.Create(	NULL,
							NULL,
							dwStyle,
							rect,
							this,
							IDC_WEBBROWSER,
							NULL ));

	return 0;
}

void CHtmlWnd::PostNcDestroy() 
{
	// if callback set, invoke it to tell it we're going away
	if (m_phtml != NULL)
		m_phtml->OnHTMLClosed(m_bstrName);
	
	// delete the temporary file
	if (!m_strTempFile.IsEmpty())
		::DeleteFile(m_strTempFile);

	CMiniFrameWnd::PostNcDestroy();
}

void CHtmlWnd::OnParentNotify(UINT message, LPARAM lParam) 
{
	WORD wEvent = LOWORD(message);

	CMiniFrameWnd::OnParentNotify(message, lParam);
	
	if (message == WM_DESTROY && lParam == (LPARAM) m_wb.m_hWnd)
	{
		// yep, nuke ourselves
		//DestroyWindow();
		PostMessage(WM_CLOSE);
	}
	
}
