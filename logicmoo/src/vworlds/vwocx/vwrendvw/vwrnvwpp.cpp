// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWRnVwPp.cpp : Implementation of the CVWRenderViewPropPage property page class.

#include "stdafx.h"
#include "VWRendVw.h"
#include "VWRnVwPp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CVWRenderViewPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CVWRenderViewPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CVWRenderViewPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CVWRenderViewPropPage, "VWCLIENTUI.RenderViewPropPage.1",
	0x44fac784, 0xca4, 0x11d0, 0x89, 0xa9, 0, 0xa0, 0xc9, 0x5, 0x41, 0x29)


/////////////////////////////////////////////////////////////////////////////
// CVWRenderViewPropPage::CVWRenderViewPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CVWRenderViewPropPage

BOOL CVWRenderViewPropPage::CVWRenderViewPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_VWRENDERVIEW_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CVWRenderViewPropPage::CVWRenderViewPropPage - Constructor

CVWRenderViewPropPage::CVWRenderViewPropPage() :
	COlePropertyPage(IDD, IDS_VWRENDERVIEW_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CVWRenderViewPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CVWRenderViewPropPage::DoDataExchange - Moves data between page and properties

void CVWRenderViewPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CVWRenderViewPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CVWRenderViewPropPage message handlers
