// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWRnVwPp.h : Declaration of the CVWRenderViewPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CVWRenderViewPropPage : See VWRnVwPp.cpp.cpp for implementation.

class CVWRenderViewPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CVWRenderViewPropPage)
	DECLARE_OLECREATE_EX(CVWRenderViewPropPage)

// Constructor
public:
	CVWRenderViewPropPage();

// Dialog Data
	//{{AFX_DATA(CVWRenderViewPropPage)
	enum { IDD = IDD_PROPPAGE_VWRENDERVIEW };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CVWRenderViewPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
