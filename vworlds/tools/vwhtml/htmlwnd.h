// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "wbbrowsr.h"

#include <resource.h>

// HtmlWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHtmlWnd window

class CVWHtmlObject;

class CHtmlWnd : public CMiniFrameWnd
{
friend class CVWHtml;
// Construction
public:
	CHtmlWnd(BSTR bstrName, CVWHtmlObject *phtml);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHtmlWnd)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHtmlWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHtmlWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// back pointer
	CVWHtmlObject *m_phtml;

	// window name 
	CComBSTR m_bstrName;

	// temporary file used for html
	CString m_strTempFile;

	// embedded browser window (child)
	CWebBrowser	m_wb;

public:
	// sets temp file
	HRESULT SetTempFile( const CString& strTempFile );
	// function to navigate
	HRESULT Navigate( CString& strURL );

};

/////////////////////////////////////////////////////////////////////////////
