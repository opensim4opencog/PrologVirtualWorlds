// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// ViewPpg.cpp : Implementation of the CVWViewPropPage property page class.

#include "stdafx.h"
#include "VWView.h"
#include "ViewPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CVWViewPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CVWViewPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CVWViewPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CVWViewPropPage, "VWVIEW.VWViewPropPage.1",
	0x812e6304, 0xc8c, 0x11d0, 0x8a, 0x96, 0, 0xa0, 0xc9, 0x8, 0x25, 0x83)


/////////////////////////////////////////////////////////////////////////////
// CVWViewPropPage::CVWViewPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CVWViewPropPage

BOOL CVWViewPropPage::CVWViewPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
#ifndef _VWSTANDALONE_CLASS_
		return TRUE;
#else
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_VWVIEW_PPG);
#endif
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CVWViewPropPage::CVWViewPropPage - Constructor

CVWViewPropPage::CVWViewPropPage() :
	COlePropertyPage(IDD, IDS_VWVIEW_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CVWViewPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CVWViewPropPage::DoDataExchange - Moves data between page and properties

void CVWViewPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CVWViewPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CVWViewPropPage message handlers
