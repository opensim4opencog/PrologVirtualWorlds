// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// ViewPpg.h : Declaration of the CVWViewPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CVWViewPropPage : See ViewPpg.cpp.cpp for implementation.

class CVWViewPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CVWViewPropPage)
	DECLARE_OLECREATE_EX(CVWViewPropPage)

// Constructor
public:
	CVWViewPropPage();

// Dialog Data
	//{{AFX_DATA(CVWViewPropPage)
	enum { IDD = IDD_PROPPAGE_VWVIEW };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CVWViewPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
