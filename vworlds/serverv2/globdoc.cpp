// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// GlobDoc.cpp : implementation file
//

#include "stdafx.h"
#include "serverv2.h"
#include "GlobDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GlobDoc

IMPLEMENT_DYNCREATE(GlobDoc, CDocument)

GlobDoc::GlobDoc()
{
}

BOOL GlobDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

GlobDoc::~GlobDoc()
{
}


BEGIN_MESSAGE_MAP(GlobDoc, CDocument)
	//{{AFX_MSG_MAP(GlobDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GlobDoc diagnostics

#ifdef _DEBUG
void GlobDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void GlobDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// GlobDoc serialization

void GlobDoc::Serialize(CArchive& ar)
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
// GlobDoc commands

BOOL GlobDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
/*	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
*/	
		
	return TRUE;
}
