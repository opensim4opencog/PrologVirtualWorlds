// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// CmdDel.h : header file
//
#ifndef _cmddel_h
#define _cmddel_h


/////////////////////////////////////////////////////////////////////////////
// CCmdDelegate command target

class CCmdDelegate : public CCmdTarget
{
	DECLARE_DYNCREATE(CCmdDelegate)

	CCmdDelegate();           // protected constructor used by dynamic creation
public:
	virtual ~CCmdDelegate();
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdDelegate)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCmdDelegate)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CCmdDelegate)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

#endif
/////////////////////////////////////////////////////////////////////////////
