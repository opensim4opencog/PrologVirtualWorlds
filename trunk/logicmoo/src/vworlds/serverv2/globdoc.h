// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_GLOBDOC_H__7E3D3565_2021_11D2_8ADD_00104BC911D4__INCLUDED_)
#define AFX_GLOBDOC_H__7E3D3565_2021_11D2_8ADD_00104BC911D4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// GlobDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// GlobDoc document

class GlobDoc : public CDocument
{
protected:
	GlobDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(GlobDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GlobDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~GlobDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(GlobDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLOBDOC_H__7E3D3565_2021_11D2_8ADD_00104BC911D4__INCLUDED_)
