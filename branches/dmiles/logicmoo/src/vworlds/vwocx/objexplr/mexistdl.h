// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_MEXISTDL_H__08041423_579E_11D2_96EC_00C04F797C5F__INCLUDED_)
#define AFX_MEXISTDL_H__08041423_579E_11D2_96EC_00C04F797C5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MExistDl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMethExistsDlog dialog

class CMethExistsDlog : public CDialog
{
// Construction
public:
	CMethExistsDlog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMethExistsDlog)
	enum { IDD = IDD_METH_EXISTS_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMethExistsDlog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMethExistsDlog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEXISTDL_H__08041423_579E_11D2_96EC_00C04F797C5F__INCLUDED_)
