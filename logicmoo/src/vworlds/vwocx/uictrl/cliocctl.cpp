// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// cliocCtl.cpp : Implementation of the CVwcliocxCtrl OLE control class.

#include "stdafx.h"
#include "uictrl.h"
#include "cliocCtl.h"
#include "propbase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CVwcliocxCtrl, CVWViewCtrl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CVwcliocxCtrl, CVWViewCtrl)
	//{{AFX_MSG_MAP(CVwcliocxCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CVwcliocxCtrl, CVWViewCtrl)
	//{{AFX_DISPATCH_MAP(CVwcliocxCtrl)
	// NOTE - ClassWizard will add and remove dispatch map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH_MAP
	DISP_PROPERTY_EX(CVWViewCtrl, "VWClient", CVWViewCtrl::GetVWClient, CVWViewCtrl::SetVWClient, VT_DISPATCH)
	DISP_PROPERTY_EX(CVwcliocxCtrl, "Version", GetVersion, SetNotSupported, VT_BSTR)
	DISP_STOCKPROP_READYSTATE()
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CVwcliocxCtrl, CVWViewCtrl)
	//{{AFX_EVENT_MAP(CVwcliocxCtrl)
	EVENT_CUSTOM("OnConnect", FireOnConnect, VTS_DISPATCH)
	EVENT_CUSTOM("OnDisconnect", FireOnDisconnect, VTS_DISPATCH)
	EVENT_CUSTOM("OnUserConnect", FireOnUserConnect, VTS_DISPATCH)
	EVENT_CUSTOM("OnUserDisconnect", FireOnUserDisconnect, VTS_DISPATCH)
	EVENT_CUSTOM("OnUserReconnect", FireOnUserReconnect, VTS_DISPATCH VTS_DISPATCH VTS_DISPATCH)
	EVENT_CUSTOM("OnTrace", FireOnTrace, VTS_BSTR)
	EVENT_CUSTOM("OnReport", FireOnReport, VTS_BSTR VTS_I4)
	EVENT_CUSTOM("OnUIEvent", FireOnUIEvent, VTS_DISPATCH  VTS_BSTR  VTS_VARIANT)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
//BEGIN_PROPPAGEIDS(CVwcliocxCtrl, 0)
//END_PROPPAGEIDS(CVwcliocxCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CVwcliocxCtrl, "VWCLIENTUI.ClientCtrl.1",
	0xd4898803, 0x20ef, 0x11d0, 0x8a, 0x96, 0, 0xa0, 0xc9, 0x8, 0x25, 0x83)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CVwcliocxCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DVwcliocx =
		{ 0xd4898801, 0x20ef, 0x11d0, { 0x8a, 0x96, 0, 0xa0, 0xc9, 0x8, 0x25, 0x83 } };
const IID BASED_CODE IID_DVwcliocxEvents =
		{ 0xd4898802, 0x20ef, 0x11d0, { 0x8a, 0x96, 0, 0xa0, 0xc9, 0x8, 0x25, 0x83 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwVwcliocxOleMisc =
	OLEMISC_INVISIBLEATRUNTIME |
//	OLEMISC_ACTIVATEWHENVISIBLE |
//	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CVwcliocxCtrl, IDS_VWCLIOCX, _dwVwcliocxOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CVwcliocxCtrl::CVwcliocxCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CVwcliocxCtrl

BOOL CVwcliocxCtrl::CVwcliocxCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegInsertable | afxRegApartmentThreading to afxRegInsertable.
	if (bRegister)
	{
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_VWCLIOCX,
			IDB_VWCLIOCX,
			afxRegInsertable | afxRegApartmentThreading,
			_dwVwcliocxOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	}
	else
	{
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CVwcliocxCtrl::CVwcliocxCtrl - Constructor

CVwcliocxCtrl::CVwcliocxCtrl()
{
	IVWClient * pClient = NULL;

	InitializeIIDs(&IID_DVwcliocx, &IID_DVwcliocxEvents);

	// TODO: Initialize your control's instance data here.
	m_ulFlags = ADVISE_CLIENT | ADVISE_WORLD;
	m_pVWClientObject = NULL;
	CLSID  clsid;
	if (CLSIDFromProgID(CComBSTR("VWSYSTEM.Client.1"), &clsid ) ==S_OK )
		CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_IDispatch, (void **)&m_pVWClientObject);

	if ( m_pVWClientObject )
	{
		if ( m_pVWClientObject->QueryInterface(IID_IVWClient,(void**) &pClient) == S_OK )
		{
			pClient->Initialize(m_hWnd);
			SAFERELEASE(pClient);
		}
	}

	SetVWClient( m_pVWClientObject );
}


/////////////////////////////////////////////////////////////////////////////
// CVwcliocxCtrl::~CVwcliocxCtrl - Destructor

CVwcliocxCtrl::~CVwcliocxCtrl()
{
	// TODO: Cleanup your control's instance data here.
	SetVWClient( NULL );
	IVWClient * pClient = NULL;
	if ( m_pVWClientObject )
	{
		if ( m_pVWClientObject->QueryInterface(IID_IVWClient,(void**) &pClient) == S_OK )
		{
			pClient->Disconnect();
			pClient->Terminate();
			SAFERELEASE(pClient);
		}
	}
	SAFERELEASE(m_pVWClientObject);
}


/////////////////////////////////////////////////////////////////////////////
// overrides

void CVwcliocxCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
//	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(DKGRAY_BRUSH)));
//	pdc->Ellipse(rcBounds);
}

void CVwcliocxCtrl::OnClose(DWORD dwSaveOption) 
{
	CVWViewCtrl::OnClose(dwSaveOption);

	// Need to disconnect and whatnot
	IVWClient * pClient = NULL;
	if ( m_pVWClientObject )
	{
		if ( m_pVWClientObject->QueryInterface(IID_IVWClient,(void**) &pClient) == S_OK )
		{
			pClient->Disconnect();
			pClient->Terminate();
			SAFERELEASE(pClient);
		}
	}
	SAFERELEASE(m_pVWClientObject);
	
}


/////////////////////////////////////////////////////////////////////////////
// CVwcliocxCtrl::DoPropExchange - Persistence support

void CVwcliocxCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	CVWViewCtrl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CVwcliocxCtrl::GetControlFlags -
// Flags to customize MFC's implementation of ActiveX controls.
//
// For information on using these flags, please see MFC technical note
// #nnn, "Optimizing an ActiveX Control".
DWORD CVwcliocxCtrl::GetControlFlags()
{
	DWORD dwFlags = CVWViewCtrl::GetControlFlags();


	// The control can activate without creating a window.
	// TODO: when writing the control's message handlers, avoid using
	//		the m_hWnd member variable without first checking that its
	//		value is non-NULL.
	dwFlags |= windowlessActivate;
	return dwFlags;
}


/////////////////////////////////////////////////////////////////////////////
// CVwcliocxCtrl::OnResetState - Reset control to default state

void CVwcliocxCtrl::OnResetState()
{
	CVWViewCtrl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CVwcliocxCtrl message handlers

#include "version.h"

#if (rmm < 10)
#define rmmpad "0"
#else
#define rmmpad
#endif

#define VER_STR1(a,b,c)		#a ".00." rmmpad #b "." #c
#define VER_STR2(a,b,c)		VER_STR1(a,b,c)
#define VER_VERSION_STR		VER_STR2(rmj,rmm,rup)

BSTR CVwcliocxCtrl::GetVersion()
{
	CString strVersion = VER_VERSION_STR;

	return strVersion.AllocSysString();
}


