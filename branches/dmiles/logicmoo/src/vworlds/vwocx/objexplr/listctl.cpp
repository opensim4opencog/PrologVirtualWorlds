// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// MListCtl.cpp : Implementation of the CMainListCtrl ActiveX Control class.

#include "stdafx.h"
#include "objexplr.h"
#include "MListCtl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CMainListCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CMainListCtrl, COleControl)
	//{{AFX_MSG_MAP(CMainListCtrl)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CMainListCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CMainListCtrl)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CMainListCtrl, COleControl)
	//{{AFX_EVENT_MAP(CMainListCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT_MAP
END_EVENT_MAP()



/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CMainListCtrl, "OBJEXPLR.MainListCtrl.1",
	0xef7d656d, 0x161, 0x11d2, 0x88, 0xbe, 0, 0xc0, 0x4f, 0xc3, 0x2e, 0xf3)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CMainListCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DMainList =
		{ 0xef7d656b, 0x161, 0x11d2, { 0x88, 0xbe, 0, 0xc0, 0x4f, 0xc3, 0x2e, 0xf3 } };
const IID BASED_CODE IID_DMainListEvents =
		{ 0xef7d656c, 0x161, 0x11d2, { 0x88, 0xbe, 0, 0xc0, 0x4f, 0xc3, 0x2e, 0xf3 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwMainListOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CMainListCtrl, IDS_MainLIST, _dwMainListOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CMainListCtrl::CMainListCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CMainListCtrl

BOOL CMainListCtrl::CMainListCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_MainLIST,
			IDB_MainLIST,
			afxRegApartmentThreading,
			_dwMainListOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CMainListCtrl::CMainListCtrl - Constructor

CMainListCtrl::CMainListCtrl()
{
	InitializeIIDs(&IID_DMainList, &IID_DMainListEvents);

	// TODO: Initialize your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CMainListCtrl::~CMainListCtrl - Destructor

CMainListCtrl::~CMainListCtrl()
{
	// TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CMainListCtrl::OnDraw - Drawing function

void CMainListCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}


/////////////////////////////////////////////////////////////////////////////
// CMainListCtrl::DoPropExchange - Persistence support

void CMainListCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CMainListCtrl::OnResetState - Reset control to default state

void CMainListCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CMainListCtrl message handlers

