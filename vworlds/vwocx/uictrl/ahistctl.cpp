// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// HistCtl.cpp : Implementation of the CHistoryCtrl ActiveX Control class.

#include "stdafx.h"
#include "uictrl.h"
#include "ctdata.h"
#include "AHistCtl.h"
#include <afxdlgs.h>
#include "ddutil.h"
#include "cutils.h"
#include "vwutils.h"
#include "syshelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CHistoryCtrl, CVWViewCtrl)

/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CHistoryCtrl, CVWViewCtrl)
	//{{AFX_MSG_MAP(CHistoryCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_SAVE_HISTORY, OnSaveHistory)
	ON_UPDATE_COMMAND_UI(ID_SAVE_HISTORY, OnUpdateSaveHistory)
	ON_COMMAND(ID_CLEAR_HISTORY, OnClearHistory)
	ON_UPDATE_COMMAND_UI(ID_CLEAR_HISTORY, OnUpdateClearHistory)
	ON_COMMAND(ID_REFRESH_HISTORY, OnRefreshHistory)
	ON_UPDATE_COMMAND_UI(ID_REFRESH_HISTORY, OnUpdateRefreshHistory)
	ON_COMMAND(ID_FILTER_GESTURES, OnFilterGestures)
	ON_UPDATE_COMMAND_UI(ID_FILTER_GESTURES, OnUpdateFilterGestures)
	ON_COMMAND(ID_FILTER_EVENTS, OnFilterEvents)
	ON_UPDATE_COMMAND_UI(ID_FILTER_EVENTS, OnUpdateFilterEvents)
	ON_COMMAND(ID_FILTER_GENERAL, OnFilterGeneral)
	ON_UPDATE_COMMAND_UI(ID_FILTER_GENERAL, OnUpdateFilterGeneral)
	ON_COMMAND(ID_FILTER_TRACE, OnFilterTrace)
	ON_UPDATE_COMMAND_UI(ID_FILTER_TRACE, OnUpdateFilterTrace)
	ON_COMMAND(ID_FILTER_REPORT, OnFilterReport)
	ON_UPDATE_COMMAND_UI(ID_FILTER_REPORT, OnUpdateFilterReport)
	ON_COMMAND(ID_EDIT_COPY, OnCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateCopy)
	ON_WM_FONTCHANGE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX(EN_LINK,255, OnLink)
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CHistoryCtrl, CVWViewCtrl)
	//{{AFX_DISPATCH_MAP(CHistoryCtrl)
	DISP_PROPERTY_NOTIFY(CHistoryCtrl, "AppendOnSave", m_bAppendOnSave, OnAppendOnSaveChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CHistoryCtrl, "AutoSave", m_bAutoSave, OnAutoSaveChanged, VT_BOOL)
	DISP_PROPERTY_EX(CHistoryCtrl, "VWClient", GetVWClient, SetVWClient, VT_DISPATCH)
	DISP_PROPERTY_EX(CHistoryCtrl, "FilterEvents", GetFilterEvents, SetFilterEvents, VT_BOOL)
	DISP_PROPERTY_EX(CHistoryCtrl, "FilterGestures", GetFilterGestures, SetFilterGestures, VT_BOOL)
	DISP_PROPERTY_EX(CHistoryCtrl, "FilterGeneral", GetFilterGeneral, SetFilterGeneral, VT_BOOL)
	DISP_PROPERTY_EX(CHistoryCtrl, "FilterTrace", GetFilterTrace, SetFilterTrace, VT_BOOL)
	DISP_PROPERTY_EX(CHistoryCtrl, "FilterList", GetFilterList, SetFilterList, VT_BOOL)
	DISP_PROPERTY_EX(CHistoryCtrl, "FilterReport", GetFilterReport, SetFilterReport, VT_BOOL)
	DISP_PROPERTY_EX(CHistoryCtrl, "AutoSaveFileName", GetAutoSaveFileName, SetAutoSaveFileName, VT_BSTR)
	DISP_PROPERTY_EX(CHistoryCtrl, "MaxAutoSaveFileSize", GetMaxAutoSaveFileSize, SetMaxAutoSaveFileSize, VT_I4)
	DISP_PROPERTY_EX(CHistoryCtrl, "SayColor", GetSayColor, SetSayColor, VT_COLOR)
	DISP_PROPERTY_EX(CHistoryCtrl, "EmoteColor", GetEmoteColor, SetEmoteColor, VT_COLOR)
	DISP_PROPERTY_EX(CHistoryCtrl, "ThinkColor", GetThinkColor, SetThinkColor, VT_COLOR)
	DISP_PROPERTY_EX(CHistoryCtrl, "WhisperColor", GetWhisperColor, SetWhisperColor, VT_COLOR)
	DISP_PROPERTY_EX(CHistoryCtrl, "AnnounceColor", GetAnnounceColor, SetAnnounceColor, VT_COLOR)
	DISP_PROPERTY_EX(CHistoryCtrl, "GestureColor", GetGestureColor, SetGestureColor, VT_COLOR)
	DISP_PROPERTY_EX(CHistoryCtrl, "ShoutColor", GetShoutColor, SetShoutColor, VT_COLOR)
	DISP_PROPERTY_EX(CHistoryCtrl, "ReportColor", GetReportColor, SetReportColor, VT_COLOR)
	DISP_PROPERTY_EX(CHistoryCtrl, "EventColor", GetEventColor, SetEventColor, VT_COLOR)
	DISP_PROPERTY_EX(CHistoryCtrl, "DirectColor", GetDirectColor, SetDirectColor, VT_COLOR)
	DISP_PROPERTY_EX(CHistoryCtrl, "TraceColor", GetTraceColor, SetTraceColor, VT_COLOR)
	DISP_PROPERTY_EX(CHistoryCtrl, "FriendColor", GetFriendColor, SetFriendColor, VT_COLOR)
	DISP_FUNCTION(CHistoryCtrl, "RefreshText", RefreshText, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CHistoryCtrl, "OnTrace", OnTrace, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CHistoryCtrl, "ViewAsHistory", ViewAsHistory, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CHistoryCtrl, "ViewAsScript", ViewAsScript, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CHistoryCtrl, "SetWhisperList", SetWhisperList, VT_ERROR, VTS_VARIANT)
	DISP_FUNCTION(CHistoryCtrl, "SetPageNameList", SetPageNameList, VT_ERROR, VTS_VARIANT)
	DISP_FUNCTION(CHistoryCtrl, "AutoSaveNow", AutoSaveNow, VT_EMPTY, VTS_NONE)
	DISP_STOCKPROP_FONT()
	DISP_STOCKPROP_BACKCOLOR()
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CHistoryCtrl, CVWViewCtrl)
	//{{AFX_EVENT_MAP(CHistoryCtrl)
	EVENT_CUSTOM("OnLink", FireOnLink, VTS_BSTR)
	EVENT_CUSTOM("OnPrefsChanged", FireOnPrefsChanged, VTS_NONE)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CHistoryCtrl, 2)
	//PROPPAGEID(CHistoryPropPage::guid)
	PROPPAGEID( CLSID_CFontPropPage )
    PROPPAGEID( CLSID_CColorPropPage )
END_PROPPAGEIDS(CHistoryCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CHistoryCtrl, "VWCLIENTUI.HistoryCtrl.1",
	0xf5029f6b, 0xaf74, 0x11d0, 0xa7, 0x8c, 0, 0xa0, 0xc9, 0xa, 0xaa, 0x10)

 
/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CHistoryCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DHistory =
		{ 0xf5029f69, 0xaf74, 0x11d0, { 0xa7, 0x8c, 0, 0xa0, 0xc9, 0xa, 0xaa, 0x10 } };
const IID BASED_CODE IID_DHistoryEvents =
		{ 0xf5029f6a, 0xaf74, 0x11d0, { 0xa7, 0x8c, 0, 0xa0, 0xc9, 0xa, 0xaa, 0x10 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwHistoryOleMisc =
	//OLEMISC_INVISIBLEATRUNTIME |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CHistoryCtrl, IDS_HISTORY, _dwHistoryOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CHistoryCtrl::CHistoryCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CHistoryCtrl

BOOL CHistoryCtrl::CHistoryCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_HISTORY,
			IDB_HISTORY,
			afxRegInsertable | afxRegApartmentThreading,
			_dwHistoryOleMisc,
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
// CHistoryCtrl::CHistoryCtrl - Constructor

CHistoryCtrl::CHistoryCtrl()
{
	InitializeIIDs(&IID_DHistory, &IID_DHistoryEvents);

	m_bWasSuper = FALSE;
	m_cyFont = 0;
	m_cyCharOld = 0;
	m_cyWindow = 0;
	m_hLibModule = NULL;
	m_CurrentIndex = 0;
	m_FilterFlags = FILTER_DEFAULT | FILTER_REPORT;
	try
	{
		m_HistoryData.SetSize( 1024*100, 1024);
	}

	catch( CMemoryException E)
	{
		// If we can't allocate this, we have real problems....
		ASSERT( 0 );
	}


	m_rgbSay = RGB(0,0,0); //tcSay = 0,	black
	m_rgbEmote = RGB(0,0,0); //tcEmote = 1,	
	m_rgbThink = RGB(0,0,0); //tcThink = 2,	
	m_rgbWhisper = RGB(0,0,0); //tcWhisper = 3,	
	m_rgbAnnounce = RGB(0,0,0);//tcAnnounce = 4,
	m_rgbGesture = RGB(0,0,0); //tcGesture = 5,
	m_rgbShout = RGB(0,0,0);//tcShout = 6,
	m_rgbReport = RGB(0,0,0); //

	m_rgbEvent= RGB(0,0,255); //	Dark blue
	m_rgbDirect= RGB(0,0,255); //Directed text	Dark blue
	m_rgbTrace = RGB(153,153,153);

	//currently not used
	m_rgbFriend = RGB(0,0,0); //FriendColor...
	//built in
	m_rgbBackColor = RGB(255,255,255);//Background	white

	m_pDIB = NULL;
	m_AutoSaveFileName = "";
	m_pAutoSaveFile = NULL;
	m_AutoSaveFileSize = 100000L;
	m_bAppendOnSave = TRUE;
	m_bAutoSave = FALSE;
	// TODO: Initialize your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CHistoryCtrl::~CHistoryCtrl - Destructor

CHistoryCtrl::~CHistoryCtrl()
{
	// TODO: Cleanup your control's instance data here.
	if ( m_pDIB ) free( m_pDIB);
	if ( m_pAutoSaveFile )
	{
		fclose( m_pAutoSaveFile );
	}
	if (m_hLibModule)
	{
		::FreeLibrary(m_hLibModule);
	}
}



/////////////////////////////////////////////////////////////////////////////
// CHistoryCtrl::OnDraw - Drawing function

void CHistoryCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	if ( m_pDIB )
	{
		StretchDIBits(pdc->m_hDC,0, 0,
			DibWidth(m_pDIB),
			DibHeight(m_pDIB),
			0, 0, DibWidth(m_pDIB), DibHeight(m_pDIB),
			DibPtr(m_pDIB), DibInfo(m_pDIB),
			DIB_RGB_COLORS, SRCCOPY);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CHistoryCtrl::DoPropExchange - Persistence support

void CHistoryCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	CVWViewCtrl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CHistoryCtrl::OnResetState - Reset control to default state

void CHistoryCtrl::OnResetState()
{
	CVWViewCtrl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////

HRESULT CHistoryCtrl::HandleOnReport( BSTR bstr, long reportType, BOOL bFromClient)
{
	CVWViewCtrl::HandleOnReport(bstr,reportType,bFromClient);
	LogType	logType = ltReport;
	long	hint = 0;

	switch (reportType)
	{
	case VWR_PERFLOG:
		// don't report perflog
		return S_OK;
		break;

	case VWR_ERROR:
		logType = ltTrace;
		break;

	case VWR_PAGE:
		hint = lhDirected;
		logType = ltText;
		reportType = tcWhisper;
		break;

	default:
		logType = ltReport;
	}

	if ( bFromClient)
	{
		m_HistoryData.SetAtGrow(m_CurrentIndex,CTextData(  NULL,NULL, bstr, reportType, logType, hint));
		if ( m_HistoryData[m_CurrentIndex ].Filter( m_FilterFlags, m_WhisperList, m_IgnoreList, m_PageNameList) )
		{
			m_HistoryData[m_CurrentIndex ].Format(m_pWorld, *this);
		}
		m_CurrentIndex++;
	}
	return S_OK;
}



HRESULT CHistoryCtrl::HandleOnEnterInventory(IThing* pWhere, IThing* pWhat)
{
//	CVWViewCtrl::HandleOnEnterInventory(pWhere,pWhat);


	long lhHint = TestForHints(pWhat, NULL);
	m_HistoryData.SetAtGrow(m_CurrentIndex,CTextData(pWhere, pWhat, NULL,etEnterInv,ltEvent,lhHint));
	if ( m_HistoryData[m_CurrentIndex ].Filter( m_FilterFlags, m_WhisperList,m_IgnoreList, m_PageNameList) )
	{
		m_HistoryData[m_CurrentIndex ].Format(m_pWorld, *this);
	}
	m_CurrentIndex++;

	return S_OK;
}

HRESULT CHistoryCtrl::HandleOnLeaveInventory(IThing* pWhere, IThing* pWhat)
{
//	CVWViewCtrl::HandleOnLeaveInventory(pWhere,pWhat);
	
	long lhHint = TestForHints(pWhat, NULL);
	m_HistoryData.SetAtGrow(m_CurrentIndex,CTextData(pWhere, pWhat,NULL,etLeaveInv,ltEvent,lhHint));
	if ( m_HistoryData[m_CurrentIndex ].Filter( m_FilterFlags, m_WhisperList,m_IgnoreList, m_PageNameList) )
	{
		m_HistoryData[m_CurrentIndex ].Format(m_pWorld, *this);
	}
	m_CurrentIndex++;

	return S_OK;
}

HRESULT CHistoryCtrl::HandleOnConnect(IDispatch* pWorld)
{
	CVWViewCtrl::HandleOnConnect(pWorld);
	m_HistoryData.SetAtGrow(m_CurrentIndex,CTextData(NULL, NULL,NULL,etConnect,ltEvent));
	if ( m_HistoryData[m_CurrentIndex ].Filter( m_FilterFlags, m_WhisperList,m_IgnoreList, m_PageNameList) )
	{
		m_HistoryData[m_CurrentIndex ].Format(m_pWorld, *this);
	}
	m_CurrentIndex++;

	return S_OK;
}

HRESULT CHistoryCtrl::HandleOnUserConnect(IThing* pThing, BOOL bFromClient)
{

	CVWViewCtrl::HandleOnUserConnect(pThing,bFromClient);
	if ( m_pDIB )
	{
		free(m_pDIB );
		m_pDIB = NULL;
	}

	//did the user pointer change?
	if (m_UserPtr ==  NULL )
	{
		HRESULT hr = m_pWorld->get_User( &(m_UserPtr.p) );
		if ( FAILED( hr ))
			return S_OK;

		
		//	GetBitmapFromThing(m_pWorld,pThing, RGB(255,0,0), &m_pDIB);
	}



	long lhHint = TestForHints(pThing, NULL);
	m_HistoryData.SetAtGrow(m_CurrentIndex,CTextData(pThing, NULL,NULL,etConnect,ltEvent,lhHint));

	if ( m_HistoryData[m_CurrentIndex ].Filter( m_FilterFlags, m_WhisperList,m_IgnoreList, m_PageNameList) )
	{
		m_HistoryData[m_CurrentIndex ].Format(m_pWorld, *this);
	}
	m_CurrentIndex++;

	return S_OK;
}

HRESULT CHistoryCtrl::HandleOnUserDisconnect(IThing* pThing, BOOL bFromClient)
{
	//	RemoveAll();
	CVWViewCtrl::HandleOnUserDisconnect(pThing, bFromClient);
	
	long lhHint = TestForHints(pThing, NULL);
	m_HistoryData.SetAtGrow(m_CurrentIndex,CTextData(pThing,NULL,NULL,etDisconnect,ltEvent,lhHint));
	if ( m_HistoryData[m_CurrentIndex ].Filter( m_FilterFlags, m_WhisperList,m_IgnoreList, m_PageNameList) )
	{
		m_HistoryData[m_CurrentIndex ].Format(m_pWorld, *this);
	}
	m_CurrentIndex++;

	// Don't disconnect unless it was us...
	if ( m_UserPtr == pThing )
	{
		m_UserPtr = (IDispatch *) NULL;
	}
	return S_OK;
}

HRESULT CHistoryCtrl::HandleOnWield(IThing* pWho, IThing* pWhat)
{
//	CVWViewCtrl::HandleOnWield(pWho, pWhat);

	long lhHint = TestForHints(pWho, NULL);
	m_HistoryData.SetAtGrow(m_CurrentIndex,CTextData(pWho, pWhat, NULL,etWield,ltEvent,lhHint));
	if ( m_HistoryData[m_CurrentIndex ].Filter( m_FilterFlags, m_WhisperList,m_IgnoreList, m_PageNameList) )
	{
		m_HistoryData[m_CurrentIndex ].Format(m_pWorld, *this);
	}
	m_CurrentIndex++;

	return S_OK;
}

HRESULT CHistoryCtrl::HandleOnUnWield(IThing* pWho, IThing* pWhat)
{
//	CVWViewCtrl::HandleOnUnWield(pWho,pWhat );

	long lhHint = TestForHints(pWho, NULL);
	m_HistoryData.SetAtGrow(m_CurrentIndex,CTextData(pWho, pWhat, NULL,etUnWield,ltEvent,lhHint));
	if ( m_HistoryData[m_CurrentIndex ].Filter( m_FilterFlags, m_WhisperList,m_IgnoreList, m_PageNameList) )
	{
		m_HistoryData[m_CurrentIndex ].Format(m_pWorld, *this);
	}
	m_CurrentIndex++;

	return S_OK;
}

HRESULT CHistoryCtrl::HandleOnEnterRoom(IThing* pWhere, IThing* pWhat)
{
//	CVWViewCtrl::HandleOnEnterRoom(pWhere,pWhat);
	
	long lhHint = TestForHints(pWhat, NULL);
	m_HistoryData.SetAtGrow(m_CurrentIndex,CTextData(pWhat, pWhere, NULL,etEnterRoom,ltEvent,lhHint));
	if ( m_HistoryData[m_CurrentIndex ].Filter( m_FilterFlags, m_WhisperList,m_IgnoreList, m_PageNameList))
	{
		m_HistoryData[m_CurrentIndex ].Format(m_pWorld, *this);
	}
	m_CurrentIndex++;

	return S_OK;
}

HRESULT CHistoryCtrl::HandleOnLeaveRoom(IThing* pWhere, IThing* pWhat)
{
//	CVWViewCtrl::HandleOnLeaveRoom(pWhere,pWhat);

	long lhHint = TestForHints(pWhat, NULL);
	m_HistoryData.SetAtGrow(m_CurrentIndex,CTextData(pWhat, pWhere, NULL,etLeaveRoom,ltEvent,lhHint));
	if ( m_HistoryData[m_CurrentIndex ].Filter( m_FilterFlags, m_WhisperList,m_IgnoreList, m_PageNameList) )
	{
		m_HistoryData[m_CurrentIndex ].Format(m_pWorld, *this);
	}
	m_CurrentIndex++;

	return S_OK;
}


HRESULT CHistoryCtrl::HandleOnTrace( BSTR bstr,BOOL bFromClient)
{
	CVWViewCtrl::HandleOnTrace(bstr,bFromClient);

	if ( bFromClient ) 
	{
		m_HistoryData.SetAtGrow(m_CurrentIndex,CTextData( NULL,NULL,bstr, -1,ltTrace ));
		if ( m_HistoryData[m_CurrentIndex ].Filter( m_FilterFlags, m_WhisperList,m_IgnoreList, m_PageNameList) )
		{
			m_HistoryData[m_CurrentIndex ].Format(m_pWorld, *this);
		}
		m_CurrentIndex++;
	}
	return S_OK;
}


HRESULT CHistoryCtrl::HandleOnTell(IThing* pThing, IPropertyList* pToList, BSTR bstr, long lval)
{
//	CVWViewCtrl::HandleOnTell(pThing,pToList,bstr,lval);

	// REVIEW:: Just pull the first thing out of the list...
	IThing *pTo	= NULL;
	LogType	ltSpeakType	= ltUnknown;
	

	ASSERT( m_UserPtr != NULL );
	// we should have had a user connect before this
	if (m_UserPtr ==  NULL )
	{
		HRESULT hr = m_pWorld->get_User( &(m_UserPtr.p) );
		if ( FAILED( hr ))
			return S_OK;
	}

	// Is this a directed tell?
	if (pToList)
		//&&
		//(( lval == tcWhisper ) || (lval == tcReport)))
	{
		HRESULT	hr;
		long lIndex = -1;
		pToList->Find(CComVariant(m_UserPtr.p), &lIndex);
		if ( lIndex < 0 ) lIndex= 0; // not us? just use the first index for now
		hr = pToList->get_Thing(0, &pTo);
		if (FAILED(hr))
			pTo = NULL;
	}
	else 
		pToList = NULL;
	
	switch (lval)
	{
	case tcSay:
	case tcThink:
	case tcWhisper:
	case tcShout:
	case tcAnnounce:
	case tcReport:
	case tcAction:
		ltSpeakType = ltText;
		break;

	case tcGesture:
	case tcEmote:
		ltSpeakType = ltGesture;
		break;
	}

	// test for hints...
	long lhHint = TestForHints(pThing, pToList);
	CTextData TextData( pThing, pTo, bstr, lval, ltSpeakType,lhHint);
	m_HistoryData.SetAtGrow(m_CurrentIndex,TextData);	
	SAFERELEASE(pTo);

	// REVIEW: This bypasses the normal filtering logic for
	// Action reporting. Ignored Avatar filtering is actually done in-world with Actions,
	// so the functionality is the same
	if (((lval == tcAction) && GetFilterEvents()) || ((lval != tcAction) &&
		m_HistoryData[m_CurrentIndex ].Filter( m_FilterFlags, m_WhisperList,m_IgnoreList,
											   m_PageNameList)) )
	{
		m_HistoryData[m_CurrentIndex ].Format(m_pWorld, *this);
	}
	m_CurrentIndex++;

return S_OK;
}




/////////////////////////////////////////////////////////////////////////////
// CHistoryCtrl message handlers

LPDISPATCH CHistoryCtrl::GetVWClient() 
{

	return CVWViewCtrl::GetVWClient();
}

void CHistoryCtrl::SetVWClient(LPDISPATCH newValue) 
{
	CVWViewCtrl::SetVWClient(newValue);
	RemoveAll();
}

void CHistoryCtrl::RemoveAll()
{
	m_CurrentIndex = 0;
	m_HistoryData.RemoveAll( );
	RefreshText();
}

void CHistoryCtrl::RefreshText() 
{
	if ( m_Edit.m_hWnd )
	{
		m_Edit.SetRedraw( FALSE );
		m_Edit.SetWindowText("" );
		//REVIEW at this point we ought to get the ignore list and use it
		for(int i=0;i< m_CurrentIndex;i++)
		{
			if ( m_HistoryData[i].Filter( m_FilterFlags, m_WhisperList,m_IgnoreList, m_PageNameList ) )
				m_HistoryData[i].Format( m_pWorld, *this);
		}
		m_Edit.SetRedraw( TRUE );
		m_Edit.Invalidate( );
	}
}

BOOL CHistoryCtrl::GetFilterEvents() 
{
	return (m_FilterFlags & FILTER_EVENTS) ? VARIANT_TRUE : VARIANT_FALSE;
}

void CHistoryCtrl::SetFilterEvents(BOOL bNewValue) 
{
	if ( bNewValue )
		m_FilterFlags |= FILTER_EVENTS;
	else
		m_FilterFlags &= ~FILTER_EVENTS;

	SetModifiedFlag();
}

BOOL CHistoryCtrl::GetFilterGestures() 
{
	return (m_FilterFlags & FILTER_GESTURES) ? VARIANT_TRUE : VARIANT_FALSE;
}

void CHistoryCtrl::SetFilterGestures(BOOL bNewValue) 
{
	if ( bNewValue )
		m_FilterFlags |= FILTER_GESTURES;
	else
		m_FilterFlags &= ~FILTER_GESTURES;

	SetModifiedFlag();
}

BOOL CHistoryCtrl::GetFilterGeneral() 
{

	return (m_FilterFlags & FILTER_TEXT) ? VARIANT_TRUE : VARIANT_FALSE;
}

void CHistoryCtrl::SetFilterGeneral(BOOL bNewValue) 
{
	if ( bNewValue )
		m_FilterFlags |= FILTER_TEXT;
	else
		m_FilterFlags &= ~FILTER_TEXT;
	SetModifiedFlag();
}

BOOL CHistoryCtrl::GetFilterTrace() 
{
	return (m_FilterFlags & FILTER_TRACE) ? VARIANT_TRUE : VARIANT_FALSE;
}

void CHistoryCtrl::SetFilterTrace(BOOL bNewValue) 
{
	if ( bNewValue )
		m_FilterFlags |= FILTER_TRACE;
	else
		m_FilterFlags &= ~FILTER_TRACE;

	SetModifiedFlag();
}

BOOL CHistoryCtrl::CRichEditCtrl_Create(CRichEditCtrl *pCtrl, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{

	m_hLibModule = ::LoadLibrary("RICHED20.DLL");
	CWnd* pWnd = pCtrl;
	BOOL bRet = pWnd->Create(_T(RICHEDIT_CLASSA), NULL, dwStyle, rect, pParentWnd, nID);
	//free it in the destructor instead of here.. 
	//FreeLibrary(hLibModule);
	return bRet;
}


VOID IndentFirst(CWnd *pWnd, BOOL fIndent)
{
	PARAFORMAT pf;

	pf.cbSize = sizeof(PARAFORMAT);

	::SendMessage(pWnd->m_hWnd, EM_GETPARAFORMAT, 0, (LPARAM) &pf);

	pf.dwMask = PFM_OFFSET | PFM_OFFSETINDENT;
	if(fIndent)
	{
		if(pf.dxOffset < 0)
		{
			pf.dxStartIndent = pf.dxOffset;
			pf.dxOffset = 0;
		}
		else
		{
			pf.dxOffset = pf.dxOffset ? -pf.dxOffset : -lDefaultTab/2;
			pf.dxStartIndent = -pf.dxOffset;
		}
	}
	else
	{
		if(pf.dxOffset < 0)
		{
			pf.dxStartIndent = pf.dxOffset;
			pf.dxOffset = -pf.dxOffset;
		}
		else
		{
			pf.dxStartIndent = 0;
			pf.dxOffset = pf.dxOffset ? 0 : lDefaultTab/2;
		}
	}

	::SendMessage(pWnd->m_hWnd, EM_SETPARAFORMAT, 0, (LPARAM) &pf);
}

int CHistoryCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CVWViewCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwStyle = ES_DISABLENOSCROLL | 
					ES_LEFT | 
					ES_READONLY | 
					ES_MULTILINE | 
					ES_SAVESEL | 
					ES_NOHIDESEL | 
					ES_AUTOVSCROLL |
					WS_CHILD | 
					WS_VISIBLE | 
					WS_VSCROLL | 
					//WS_HSCROLL |
					WS_CLIPSIBLINGS |
					WS_CLIPCHILDREN  ;


	RECT rect;
	GetClientRect( &rect );


	if (!CRichEditCtrl_Create( &m_Edit, dwStyle, rect, this, 255 ))
	{
		m_Edit.Create( dwStyle, rect, this, 255 );
		Log(bstr_t("Unable to create RichEdit 2.0, using default"), eLNewPara,RGB(255,0,0));
	}
	else
	{

		DWORD dwMask = m_Edit.GetEventMask();
		dwMask |= ENM_LINK | ENM_SELCHANGE;
		m_Edit.SetEventMask( dwMask );


		m_Edit.SendMessage( EM_AUTOURLDETECT ,1 ); 
		BOOL b = m_Edit.SendMessage( EM_GETAUTOURLDETECT );
		CHARFORMAT cf;
		m_Edit.GetDefaultCharFormat(cf);
		cf.dwMask |= CFM_COLOR;
		cf.dwEffects |= CFE_AUTOCOLOR;
		cf.yHeight = 180;
		cf.bPitchAndFamily = FF_SWISS | VARIABLE_PITCH;
		strcpy(cf.szFaceName, "Arial");
		m_Edit.SetDefaultCharFormat(cf);
		IndentFirst( &m_Edit, FALSE );

	}		

	CFontHolder& font = InternalGetFont();

	HFONT hFont = font.GetFontHandle();
	if ( hFont )
	{
		SetFont(CFont::FromHandle(hFont ), TRUE); 
	}

	m_Edit.SendMessage(EM_SETBKGNDCOLOR ,0,m_rgbBackColor);
//	HandleOnTell(NULL, NULL, CComBSTR("History List Control"), tcAnnounce );


	return 0;
}

void CHistoryCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CVWViewCtrl::OnSize(nType, cx, cy);
	
	if ( m_Edit.m_hWnd )
	{
	//	m_Edit.SetWindowPos(&wndTop,0,0,cx,cy,SWP_NOZORDER );
		int offset = m_pDIB? DibWidth(m_pDIB):0;
		m_Edit.SetWindowPos(&wndTop,offset,0,cx-offset,cy,SWP_NOZORDER );
		m_cyWindow = cy;
	}
}

// formatting stuff



void CHistoryCtrl::Log(const bstr_t bstr, int eLogOptions, COLORREF crTextColor, BOOL bTrim)
{

	// REVIEW: [mv] Don't log things coming in from
	// secondary threads
	CWnd *pwndInMap;
	pwndInMap = CWnd::FromHandlePermanent( m_hWnd );
	if ( pwndInMap == NULL || pwndInMap != (CWnd *) this)
		return ;
	

	CHARRANGE cr;
	BOOL bRestore;
	int  iHoldScroll = 0;
	CString strLog;
	strLog = BSTR(bstr); 
	if ( bTrim )
	{
		strLog.TrimLeft();
		strLog.TrimRight();
	}
	// log to file

	if ( m_pAutoSaveFile )
	{
		//REVIEW
		// Test here to check if we've reached our max size!!!
		// and do something about it...
		// the _ftprintf ought to be cool for TSTR and CString::LPCTSTR ought to always return the right TSTR
		// and the whole wide character thing should fall out, right?
		CString strFormat = (eLogOptions & eLNewPara )?"\n%s":"%s";
		_ftprintf(m_pAutoSaveFile,LPCTSTR(strFormat), LPCTSTR(strLog) );
		fflush( m_pAutoSaveFile );
	}

	// If the accessibility feature, high contrast is on, the background color could be black.
	// Therefore if the selected text color is black, we will swap it to white to make it 
	// visible.
	if (crTextColor == 0)
	{
		if (GetSysColor(COLOR_WINDOW) == 0)
		{
			crTextColor = 0x00FFFFFF;
		}
	}

	iHoldScroll = m_Edit.GetFirstVisibleLine();

	bRestore = SaveAndMoveSel(&cr);


	// handle new para if specified
	if( eLogOptions & eLNewPara )
	{
		NewParagraph();
	}

	// handle other options
	CharacterFormat( (eLogOptions & eLBold),(eLogOptions & eLItalic), crTextColor );
	m_Edit.ReplaceSel(  (const char *) strLog , FALSE);

	if (bRestore)
	{
		
		// Restore the selection that was in place before the log.
		m_Edit.SetSel(cr);		
		
		// make the old top the same as the current top!
		int currentTop = m_Edit.GetFirstVisibleLine();
	
		// this is kind of odd, you'd think that LineScroll( currentTop - iHoldScroll ) would work. not
		// scroll to top...
		m_Edit.LineScroll( -currentTop);
		//scroll to where we left off
		m_Edit.LineScroll( iHoldScroll);

		currentTop = m_Edit.GetFirstVisibleLine();
		ASSERT( currentTop == iHoldScroll );
/*
		while( currentTop != iHoldScroll )
		{
			currentTop -= iHoldScroll;
			m_Edit.LineScroll( -currentTop);
			currentTop = m_Edit.GetFirstVisibleLine();
		}
*/
		//::LockWindowUpdate(NULL);	
		SetWindowPos(&wndTop,0,0,0,0,SWP_SHOWWINDOW|SWP_NOSIZE  );

	

	}
	else
	{
		// select the end
		DWORD len = m_Edit.GetWindowTextLength();
		m_Edit.SetSel(len ,len );
	}
}

BOOL CHistoryCtrl::SaveAndMoveSel( CHARRANGE *pcr )
{
	CHARRANGE cr;
	int cch;

	// get current selection
	m_Edit.GetSel( *pcr );

	// get total text length
	cch = m_Edit.GetTextLength();
	//POINT pt = {0,0};
	//::SendMessage(m_Edit.m_hWnd, EM_CHARFROMPOS, (WPARAM)&pt, (LPARAM)pcr->cpMin);
	//RECT rect = {0,0,0,0};
	//m_Edit.GetClientRect( &rect );

	//LPARAM pos = MAKELPARAM(0, rect.bottom);
	//pos = ::SendMessage(m_Edit.m_hWnd, EM_CHARFROMPOS, 0, (LPARAM) &pos );

	//if ( 1) 
	//{
	//	int x = pt.x;
	//}
	// Check if anything is selected.
	if( pcr->cpMin == pcr->cpMax )
	{
		// Nope, move carat to the end if it's not already there.
		if (pcr->cpMin != cch)
		{
			cr.cpMin = cr.cpMax = cch;
			m_Edit.SetSel(cr);
		}
		// is the last character visible?


		// Nothing more to do
		return FALSE;
	}
	// Yep, must move sel to end and restore later
	//Got to stop the window from redrawing at this point!
	SetWindowPos( &wndTop,0,0,0,0,SWP_HIDEWINDOW | SWP_NOREDRAW|SWP_NOSIZE  );
	cr.cpMin = cr.cpMax = cch;
	m_Edit.SetSel( cr );

	return TRUE;
}

BOOL CHistoryCtrl::IsEndInView()
{
	return TRUE;
}

void CHistoryCtrl::SetFont( CFont *pfont, BOOL bRedraw )
{
	int 			cch;
	CHARRANGE 		cr;
	CHARFORMAT 		cf;
	//CDC 			MemDC;
	CWindowDC		dc(&m_Edit);
	TEXTMETRIC 		TextMetrics;

	// Must make sure SuperScript is turned off here.
	if (m_bWasSuper)
	{
		// Set selection to last character (again!).
		cch = (short) m_Edit.GetWindowTextLength();
		cr.cpMin = cr.cpMax = cch;
		m_Edit.SendMessage( EM_EXSETSEL, 0, (LPARAM) &cr );
		SuperScript(FALSE);	
	}

	// First set the font of the window as requested. This will ensure the window has the correct font type at
	// start-up - we have to have some font to change the size of!
	
	m_Edit.SetFont( pfont );

	// Now select all text in the log window.
	cch = (short) m_Edit.GetWindowTextLength();
	cr.cpMin = 0;
	cr.cpMax = cch;
	m_Edit.SendMessage( EM_EXSETSEL, 0, (LPARAM) &cr );

	// To set the font point size, we must first determine the point size requested by pfont.
	CFont* pFontOld = dc.SelectObject(pfont);
	dc.GetTextMetrics(&TextMetrics);
	int iLogPixelsY = dc.GetDeviceCaps(LOGPIXELSY);
	int iPointSize = (TextMetrics.tmHeight - TextMetrics.tmInternalLeading) * 72 / iLogPixelsY;

	// cache for later
	m_cyFont = TextMetrics.tmHeight;

	// Now get the current character format of the log wnd.
	cf.cbSize = sizeof(CHARFORMAT);
	// Want to keep the bolding, color and offset already applied to various portions of text.
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_OFFSET;
	m_Edit.SendMessage( EM_GETCHARFORMAT, TRUE, (LPARAM) &cf );
	// Just change the overall size of the text.
	cf.dwMask |= CFM_SIZE;
	cf.yHeight = iPointSize * 20;

	// And set character format.
	m_Edit.SendMessage( EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf );

	// Finally deselect the text.
	cch = (short) m_Edit.GetWindowTextLength();
	cr.cpMin = cr.cpMax = cch;
	m_Edit.SendMessage( EM_EXSETSEL, 0, (LPARAM) &cr );

	// And free GDI stuff.
	dc.SelectObject(pFontOld);
}

void CHistoryCtrl::CharacterFormat(BOOL bBold, BOOL bItalic, COLORREF crTextColor)
{
	CHARFORMAT cf;

	// Get the current character format.
	cf.cbSize = sizeof(CHARFORMAT);
	m_Edit.SendMessage( EM_GETCHARFORMAT, TRUE, (LPARAM) &cf );

	// Set the required color (stop auto-coloring first).

	cf.dwMask |= CFM_COLOR;
	cf.dwEffects &= ~CFE_AUTOCOLOR;
	cf.crTextColor = crTextColor;

	// Set the text to bold or not as required.
	cf.dwMask |= CFM_BOLD;
	if( bBold )
		cf.dwEffects |= CFE_BOLD;
	else
		cf.dwEffects &= ~(CFE_BOLD);

		// Set the text to bold or not as required.
	cf.dwMask |= CFM_ITALIC;
	if( bItalic )
		cf.dwEffects |= CFE_ITALIC;
	else
		cf.dwEffects &= ~(CFE_ITALIC);

	// Finally do actual character format set.
	m_Edit.SendMessage( EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf );
	
}

void CHistoryCtrl::Indent(BOOL bIndent)
{
	PARAFORMAT pf;

	pf.cbSize = sizeof(PARAFORMAT);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = bIndent ? cxIndentation : cxLeftMargin;
	m_Edit.SendMessage( EM_SETPARAFORMAT, 0, (LPARAM) &pf );
}

void CHistoryCtrl::NewParagraph()
{
	CHARRANGE cr;
	cr.cpMin = cr.cpMax = m_Edit.GetWindowTextLength();
	m_Edit.SetSel( cr );
	m_Edit.ReplaceSel("\r\n");
	
}

void CHistoryCtrl::SuperScript( BOOL bSuperScript )
{
	CHARFORMAT cf;

	cf.cbSize = sizeof(CHARFORMAT);
	m_Edit.SendMessage( EM_GETCHARFORMAT, TRUE, (LPARAM) &cf );
	if( bSuperScript )
	{
		cf.dwMask |= CFM_OFFSET | CFM_SIZE;
		m_bWasSuper = TRUE;
		m_cyCharOld = cf.yHeight;

		// Never want superscript to drop below the 9 point superscript size (3/4 * 9pt) (it's just too darn small!).
		if (cf.yHeight < 180)
		{
			cf.yHeight = 180;
		}
		cf.yHeight = (cf.yHeight * 3) / 4;
		cf.yOffset = cf.yHeight / 4;
	}
	else
	{
		if( m_bWasSuper )
		{
			cf.dwMask |= CFM_OFFSET | CFM_SIZE;
			cf.yHeight = m_cyCharOld;
			cf.yOffset = 0;
			m_bWasSuper = FALSE;
		}
	}
	m_Edit.SendMessage( EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf );
}

BOOL CHistoryCtrl::IsSelectionEmpty()
{
	CHARRANGE	cr;

	m_Edit.SendMessage( EM_EXGETSEL, 0, (LPARAM)&cr );
	return cr.cpMin == cr.cpMax ? TRUE : FALSE;
}


BOOL CHistoryCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	//return CVWViewCtrl::OnEraseBkgnd(pDC);
	return TRUE;

}


BOOL CHistoryCtrl::OnLink(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
	ENLINK * pENLINK = ( ENLINK *) pNMHDR;
	switch( pENLINK->msg )
	{
		case WM_RBUTTONDOWN:
		case WM_LBUTTONDOWN:
		{
			CHARRANGE cr;
			m_Edit.GetSel( cr );
			m_Edit.SetSel( pENLINK->chrg);
			CString str = m_Edit.GetSelText( ); 
			m_Edit.SetSel( cr );
			/*
			HINSTANCE hinst;
			if ( 32>= (INT) (hinst =ShellExecute(
				NULL,// handle to parent window
				"open",	// pointer to string that specifies operation to perform
				str,	// pointer to filename or folder name string
				NULL,	// pointer to string that specifies executable-file parameters 
				NULL,	// pointer to string that specifies default directory
				SW_SHOWNORMAL)))	// whether file is shown when opened

			{
				Log("Unable to launch URL", eLNewPara,RGB(255,0,0));
			}
			*/
			// Fire Event instead
			FireOnLink( str );
		}
		break;
	}
	return TRUE;
}




void CHistoryCtrl::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu menu;
	menu.CreatePopupMenu();
	// build the menu
	menu.AppendMenu( MF_STRING, ID_EDIT_COPY, "&Copy");
	menu.AppendMenu( MF_STRING, ID_SAVE_HISTORY, "Save &As...");
	menu.AppendMenu( MF_STRING, ID_CLEAR_HISTORY, "C&lear History");
	menu.AppendMenu( MF_STRING, ID_REFRESH_HISTORY, "&Refresh");
	menu.AppendMenu( MF_STRING, ID_FILTER_GESTURES, "Show &Gestures");
	menu.AppendMenu( MF_STRING, ID_FILTER_EVENTS, "Show &Events");
	menu.AppendMenu( MF_STRING, ID_FILTER_GENERAL, "Show General &Text");
	menu.AppendMenu( MF_STRING, ID_FILTER_TRACE, "Show Trac&es");
	menu.AppendMenu( MF_STRING, ID_FILTER_REPORT, "Show Re&ports");

	VERIFY(menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this));
	menu.DestroyMenu();

}

DWORD CALLBACK MyWrite(DWORD dwCookie, LPBYTE pbBuffer, LONG cb, LONG *pcb)
{
	FILE *	pFile = (FILE*) dwCookie;

	if(!pFile)
		return (DWORD) E_FAIL;

	*pcb = fwrite(pbBuffer,sizeof(BYTE), cb,pFile);
	return (DWORD) (*pcb == cb ? NOERROR : E_FAIL);
}

void CHistoryCtrl::OnSaveHistory() 
{
	static char BASED_CODE szFilter[] = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||";
	CFileDialog dlg( FALSE, "*.txt", "History.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	if ( dlg.DoModal() == IDOK )
	{
		FILE *pFile = fopen(dlg.m_ofn.lpstrFile,"wt");
		if ( pFile )
		{
			EDITSTREAM es;
			es.dwCookie = (DWORD) pFile;
			es.dwError = 0;
			es.pfnCallback = MyWrite;

			m_Edit.SendMessage(EM_STREAMOUT,SF_TEXTIZED, (LPARAM) &es);
			
			fclose( pFile );
		}
	}
}

void CHistoryCtrl::OnUpdateSaveHistory(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CHistoryCtrl::OnClearHistory() 
{
	if (m_bAutoSave)
		PerformAutosave();

	RemoveAll();
}

void CHistoryCtrl::OnUpdateClearHistory(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CHistoryCtrl::OnRefreshHistory() 
{
	RefreshText();

}

void CHistoryCtrl::OnUpdateRefreshHistory(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CHistoryCtrl::OnFilterGestures() 
{

	SetFilterGestures( GetFilterGestures() ?  FALSE : TRUE );
	//RefreshText();
	FireOnPrefsChanged();
}

void CHistoryCtrl::OnUpdateFilterGestures(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetFilterGestures() );
}

void CHistoryCtrl::OnFilterEvents() 
{
	SetFilterEvents( GetFilterEvents() ?  FALSE : TRUE );
	//RefreshText();
	FireOnPrefsChanged();
}

void CHistoryCtrl::OnUpdateFilterEvents(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetFilterEvents() );
}

void CHistoryCtrl::OnFilterGeneral() 
{
	SetFilterGeneral( GetFilterGeneral() ?  FALSE : TRUE );
	//RefreshText() ;
	FireOnPrefsChanged();
}

void CHistoryCtrl::OnUpdateFilterGeneral(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetFilterGeneral() );
}

void CHistoryCtrl::OnFilterTrace() 
{
	SetFilterTrace( GetFilterTrace() ?  FALSE : TRUE );
	//RefreshText() ;
	FireOnPrefsChanged();
}

void CHistoryCtrl::OnUpdateFilterTrace(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetFilterTrace() );
}

void CHistoryCtrl::OnFilterReport() 
{
	SetFilterReport( GetFilterReport() ?  FALSE : TRUE );
	//RefreshText() ;
	FireOnPrefsChanged();
}

void CHistoryCtrl::OnUpdateFilterReport(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(GetFilterReport() );
}

void CHistoryCtrl::OnCopy() 
{
	m_Edit.Copy();
}

void CHistoryCtrl::OnUpdateCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_Edit.GetSelectionType() == SEL_EMPTY? FALSE : TRUE);	
}




void CHistoryCtrl::OnBackColorChanged()
{

	CVWViewCtrl::OnBackColorChanged();
	OLE_COLOR nNewValue = GetBackColor();
	m_rgbBackColor = TranslateColor(nNewValue);
	if ( m_Edit.m_hWnd ) 
		m_Edit.SendMessage(EM_SETBKGNDCOLOR ,0,m_rgbBackColor);
	
}
void CHistoryCtrl::OnFontChanged()
{
	CVWViewCtrl::OnFontChanged();

	if ( m_Edit.m_hWnd ) 
	{
		CFontHolder& font = InternalGetFont();

		HFONT hFont = font.GetFontHandle();
		if ( hFont )
		{
			SetFont(CFont::FromHandle(hFont ), TRUE); 
		}
	}
}



void CHistoryCtrl::OnFontChange() 
{
	CVWViewCtrl::OnFontChange();
		
}

void CHistoryCtrl::OnAmbientPropertyChange(DISPID dispid)
{
	switch (dispid )
	{
	case DISPID_AMBIENT_FONT:
		{
			LPFONTDISP pFont = AmbientFont();
			if ( pFont )
				COleControl::SetFont( pFont );
			SAFERELEASE( pFont );
		}
	break;
	}
}


void CHistoryCtrl::OnTrace(LPCTSTR bstr) 
{
	HandleOnTrace( _bstr_t( bstr ), VARIANT_TRUE);
}

void CHistoryCtrl::ViewAsHistory() 
{
	if (m_FilterFlags != FILTER_DEFAULT)
	{
		m_FilterFlags = FILTER_DEFAULT | FILTER_REPORT;
		RefreshText();
	}
}

void CHistoryCtrl::ViewAsScript() 
{
	if (m_FilterFlags != FILTER_TRACE)
	{
		m_FilterFlags = FILTER_TRACE | FILTER_REPORT;
		RefreshText();
	}
}


BOOL CHistoryCtrl::GetFilterList() 
{
	return (m_FilterFlags & FILTER_SELECTION) ? VARIANT_TRUE : VARIANT_FALSE;
}

void CHistoryCtrl::SetFilterList(BOOL bNewValue) 
{
	if ( bNewValue )
		m_FilterFlags |= FILTER_SELECTION;
	else
		m_FilterFlags &= ~FILTER_SELECTION;
	SetModifiedFlag();
}


SCODE CHistoryCtrl::SetWhisperList(const VARIANT FAR& List) 
{
	HRESULT hr = E_FAIL;
	CComPtr<IPropertyList>	pPropList;
	CComVariant varArgs;
	CThingPtr	ThingPtr;
	long id;

	try
	{
		TestHRESULT(VariantCopyInd(&varArgs, (VARIANT *)&List));
		m_WhisperList.RemoveAll();

		// Add the user in first
		if ( m_UserPtr != NULL)
		{
			TestHRESULT(m_UserPtr->get_ID( &id ));
			m_WhisperList.Add(id);
		}

		// do we have a SAFEARRAY
		if (varArgs.vt == (VT_VARIANT | VT_ARRAY))
		{
			TestHRESULT(CreatePropertyList(m_pWorld, &pPropList));
			TestHRESULT(ConvertSAFEARRAYToPropertyList(&varArgs, pPropList));
		}
		else if (varArgs.vt == VT_DISPATCH)
		{
			if (varArgs.pdispVal)
			{
				// could be either an IThing or an IPropertyList...
				CComQIPtr<IThing, &IID_IThing> pthing;
				CComQIPtr<IPropertyList, &IID_IPropertyList> plist;

				pthing = varArgs.pdispVal;
				if (pthing)
				{
					// we have a thing - create a list and add to it
					TestHRESULT(CreatePropertyList(m_pWorld, &pPropList));
					TestHRESULT(pPropList->AddDispatch(pthing));
					pthing.Release();
				}
				else
				{
					// if we have a list - use it
					if (plist = varArgs.pdispVal)
					{
						pPropList = plist;
						plist.Release();
					}
				}
			}
		}

		if (pPropList != NULL)
		{
			long		lCount;
			CComVariant v, varTemp;

			TestHRESULT(pPropList->get_Count(&lCount));

			for (long i = 0; i < lCount; i++)
			{
				TestHRESULT(pPropList->get_Property(i, &v));
				
				// If a parameter was passed by reference, get the value after de-reference.  
				// VBA will pass variables by reference.
				TestHRESULT(VariantCopyInd(&varTemp, &v));
				
				if (varTemp.vt == VT_DISPATCH)
				{
					ThingPtr = varTemp;
					if ( ThingPtr != NULL )
					{
						TestHRESULT(ThingPtr->get_ID( &id ));
						m_WhisperList.Add(id);
					}
				}
			}
		}
	}
	catch (CHRESULTException chr)
	{
		return chr.GetHRESULT();
	}

	return S_OK;
}


long CHistoryCtrl::TestForHints( IThing *pThing, IPropertyList *pToList )
{
	if (!pThing) return 0;
	
	// changed this so that it can be used before the user's avatar is created
	if (m_UserPtr == NULL)
		return 0;

	ASSERT( m_UserPtr != NULL );

	long lhHint =  lhNone;
	VARIANT_BOOL bShow= VARIANT_TRUE;
	HRESULT hr;
	CComVariant varResult;
	VARIANT rgvarg[] = {{0,0,0,0,0}, {0,0,0,0,0}};
	DISPPARAMS	dispparams = {rgvarg,NULL,2,0}; 	
	static CComBSTR bstrFriends("Friends");
	static CComBSTR bstrIgnoring("Ignoring");


	// Is it text directed at us?
	if ( pToList )
	{
		long lIndex = -1;
		pToList->Find(CComVariant(m_UserPtr.p), &lIndex);
		if (( lIndex >= 0 ) || (m_UserPtr == pThing ))
			lhHint |= lhDirected;

	}

	// Test to see if it is a friend
	::VariantInit(&(rgvarg[0]));
	::VariantInit(&(rgvarg[1]));
	

	dispparams.rgvarg[1].vt = VT_DISPATCH;
	dispparams.rgvarg[1].pdispVal = pThing;
	dispparams.rgvarg[0].vt = VT_BSTR;
	dispparams.rgvarg[0].bstrVal = bstrFriends.m_str;

	hr = m_UserPtr->InvokeMethodExt(CComBSTR("IsInUserProfileList"), &dispparams, &varResult);
	if (SUCCEEDED(hr))
	{
		if ((varResult.vt == VT_BOOL ) &&
			(varResult.boolVal == VARIANT_TRUE))
		{
			lhHint |= lhFriend;
		}
	}

	//Are we ignoring it?
	dispparams.rgvarg[0].vt = VT_BSTR;
	dispparams.rgvarg[0].bstrVal = bstrIgnoring.m_str;
	hr = m_UserPtr->InvokeMethodExt(CComBSTR("IsInUserProfileList"), &dispparams, &varResult);
	if (SUCCEEDED(hr))
	{
		if ((varResult.vt == VT_BOOL ) &&
			(varResult.boolVal == VARIANT_TRUE))
		{
			lhHint |= lhIgnored;
		}
	}
	return lhHint;
}

BSTR CHistoryCtrl::GetAutoSaveFileName() 
{

	return m_AutoSaveFileName.AllocSysString();
}

void CHistoryCtrl::SetAutoSaveFileName(LPCTSTR lpszNewValue) 
{

	if ( m_pAutoSaveFile ) fclose( m_pAutoSaveFile );
	m_AutoSaveFileName = lpszNewValue;
	m_pAutoSaveFile = fopen( m_AutoSaveFileName,"a+t");
	SetModifiedFlag();
}

long CHistoryCtrl::GetMaxAutoSaveFileSize() 
{
	return m_AutoSaveFileSize;
}

void CHistoryCtrl::SetMaxAutoSaveFileSize(long nNewValue) 
{
	m_AutoSaveFileSize = nNewValue;
	SetModifiedFlag();
}


OLE_COLOR CHistoryCtrl::GetSayColor() 
{
	return m_rgbSay;
}

void CHistoryCtrl::SetSayColor(OLE_COLOR nNewValue) 
{
	m_rgbSay = nNewValue;
	SetModifiedFlag();
}

OLE_COLOR CHistoryCtrl::GetEmoteColor() 
{
	return m_rgbEmote;
}

void CHistoryCtrl::SetEmoteColor(OLE_COLOR nNewValue) 
{
	m_rgbEmote = nNewValue;
	SetModifiedFlag();
}

OLE_COLOR CHistoryCtrl::GetThinkColor() 
{
	return m_rgbThink;
}

void CHistoryCtrl::SetThinkColor(OLE_COLOR nNewValue) 
{
	m_rgbThink = nNewValue;
	SetModifiedFlag();
}

OLE_COLOR CHistoryCtrl::GetWhisperColor() 
{
	return m_rgbWhisper;
}

void CHistoryCtrl::SetWhisperColor(OLE_COLOR nNewValue) 
{
	m_rgbWhisper = nNewValue;
	SetModifiedFlag();
}

OLE_COLOR CHistoryCtrl::GetAnnounceColor() 
{
	return m_rgbAnnounce;
}

void CHistoryCtrl::SetAnnounceColor(OLE_COLOR nNewValue) 
{
	m_rgbAnnounce = nNewValue;
	SetModifiedFlag();
}

OLE_COLOR CHistoryCtrl::GetGestureColor() 
{
	return m_rgbGesture;
}

void CHistoryCtrl::SetGestureColor(OLE_COLOR nNewValue) 
{
	m_rgbGesture = nNewValue;
	SetModifiedFlag();
}

OLE_COLOR CHistoryCtrl::GetShoutColor() 
{
	return m_rgbShout;
}

void CHistoryCtrl::SetShoutColor(OLE_COLOR nNewValue) 
{
	m_rgbShout = nNewValue;
	SetModifiedFlag();
}

OLE_COLOR CHistoryCtrl::GetReportColor() 
{
	return m_rgbEvent;;
}

void CHistoryCtrl::SetReportColor(OLE_COLOR nNewValue) 
{
	m_rgbReport = nNewValue;
	SetModifiedFlag();
}

OLE_COLOR CHistoryCtrl::GetEventColor() 
{
	return m_rgbEvent;
}

void CHistoryCtrl::SetEventColor(OLE_COLOR nNewValue) 
{
	m_rgbEvent = nNewValue;
	SetModifiedFlag();
}

OLE_COLOR CHistoryCtrl::GetDirectColor() 
{
	return m_rgbDirect;
}

void CHistoryCtrl::SetDirectColor(OLE_COLOR nNewValue) 
{
	m_rgbDirect = nNewValue;
	SetModifiedFlag();
}

OLE_COLOR CHistoryCtrl::GetTraceColor() 
{
	return m_rgbTrace;
}

void CHistoryCtrl::SetTraceColor(OLE_COLOR nNewValue) 
{
	m_rgbTrace = nNewValue;
	SetModifiedFlag();
}

OLE_COLOR CHistoryCtrl::GetFriendColor() 
{
	return m_rgbFriend;
}

void CHistoryCtrl::SetFriendColor(OLE_COLOR nNewValue) 
{
	m_rgbFriend = nNewValue;
	SetModifiedFlag();
}

BOOL CHistoryCtrl::GetFilterReport() 
{
	return (m_FilterFlags & FILTER_REPORT) ? VARIANT_TRUE : VARIANT_FALSE;
}

void CHistoryCtrl::SetFilterReport(BOOL bNewValue) 
{
	if ( bNewValue )
		m_FilterFlags |= FILTER_REPORT;
	else
		m_FilterFlags &= ~FILTER_REPORT;

	SetModifiedFlag();
}

SCODE CHistoryCtrl::SetPageNameList(const VARIANT FAR& List) 
{
	HRESULT hr = E_FAIL;
	CComPtr<IPropertyList>	pPropList;
	CComVariant varArgs;
	CThingPtr	ThingPtr;
	CComBSTR	bstrName;
	CString		strName;

	try
	{
		TestHRESULT(VariantCopyInd(&varArgs, (VARIANT *)&List));
		m_PageNameList.RemoveAll();

		// Add the user in first
		if ( m_UserPtr != NULL)
		{
			TestHRESULT(m_UserPtr->get_Name(&bstrName.m_str));
			strName = bstrName;
			strName.MakeUpper();
			m_PageNameList.AddTail(strName);
		}

		// do we have a SAFEARRAY
		if (varArgs.vt == (VT_VARIANT | VT_ARRAY))
		{
			TestHRESULT(CreatePropertyList(m_pWorld, &pPropList));
			TestHRESULT(ConvertSAFEARRAYToPropertyList(&varArgs, pPropList));
		}
		else if (varArgs.vt == VT_DISPATCH)
		{
			if (varArgs.pdispVal)
			{
				// could be either an IThing or an IPropertyList...
				CComQIPtr<IThing, &IID_IThing> pthing;
				CComQIPtr<IPropertyList, &IID_IPropertyList> plist;

				pthing = varArgs.pdispVal;
				if (pthing)
				{
					// we have a thing - create a list and add to it
					TestHRESULT(CreatePropertyList(m_pWorld, &pPropList));
					TestHRESULT(pPropList->AddDispatch(pthing));
					pthing.Release();
				}
				else
				{
					// if we have a list - use it
					if (plist = varArgs.pdispVal)
					{
						pPropList = plist;
						plist.Release();
					}
				}
			}
		}

		if (pPropList != NULL)
		{
			long		lCount;
			CComVariant v, varTemp;

			TestHRESULT(pPropList->get_Count(&lCount));

			for (long i = 0; i < lCount; i++)
			{
				TestHRESULT(pPropList->get_Property(i, &v));
				
				// If a parameter was passed by reference, get the value after de-reference.  
				// VBA will pass variables by reference.
				TestHRESULT(VariantCopyInd(&varTemp, &v));
				
				if (varTemp.vt == VT_BSTR)
				{
					// it is a string - add it to the list
					strName = varTemp.bstrVal;
					strName.MakeUpper();
					m_PageNameList.AddTail(strName);
				}
				else if (varTemp.vt == VT_DISPATCH)
				{
					ThingPtr = varTemp;
					if ( ThingPtr != NULL )
					{
						TestHRESULT(ThingPtr->get_Name(&bstrName.m_str));
						strName = bstrName;
						strName.MakeUpper();
						m_PageNameList.AddTail(strName);
					}
				}
			}
		}
	}
	catch (CHRESULTException chr)
	{
		return chr.GetHRESULT();
	}

	return S_OK;
}

void CHistoryCtrl::OnAppendOnSaveChanged() 
{
	SetModifiedFlag();
}

void CHistoryCtrl::PerformAutosave()
{
	static char BASED_CODE szFilter[] = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||";

	if (m_AutoSaveFileName.IsEmpty())
	{
		DWORD	dwFlags;

		if (m_bAppendOnSave)
			dwFlags = OFN_HIDEREADONLY;
		else
			dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

		CFileDialog dlg( FALSE, "*.txt", "History.txt", dwFlags, szFilter, this);

		if ( dlg.DoModal() == IDOK )
		{
			m_AutoSaveFileName = dlg.m_ofn.lpstrFile;
		}
	}

	if (!m_AutoSaveFileName.IsEmpty())
	{
		FILE *pFile;
		
		if (m_bAppendOnSave)
			pFile = fopen(m_AutoSaveFileName, "at");
		else
			pFile = fopen(m_AutoSaveFileName,"wt");

		if ( pFile )
		{
			EDITSTREAM es;
			es.dwCookie = (DWORD) pFile;
			es.dwError = 0;
			es.pfnCallback = MyWrite;

			m_Edit.SendMessage(EM_STREAMOUT,SF_TEXTIZED, (LPARAM) &es);
			
			fclose( pFile );
		}
	}
}

void CHistoryCtrl::AutoSaveNow() 
{
	if (m_bAutoSave)
		PerformAutosave();
}

void CHistoryCtrl::OnAutoSaveChanged() 
{
	SetModifiedFlag();
}
