// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWRnVwCt.cpp : Implementation of the CVWRenderViewCtrl OLE control class.

#include "stdafx.h"
//#include "initguid.h"
#include <d3drmwin.h>
#include "VWRendVw.h"
#include <propbase.h>
#include <vwmenu.h>
#include "VWRnVwCt.h"
#include "VWRnVwPp.h"
#include <graphtls.h>
#include <vwutils.h>
#include "vwsgfxut.h"
#include "Pickdata.h"

#define IID_DEFINED
#include "VWRender_i.c"
#include "VWNvTool_i.c"
#include "VWPkTool_i.c"
#include "Pickdata_i.c"
#include "cellprop_i.c"

#include "syshelp.h"
#include <math.h>

#include <vwevents.h>
#include <vwsevent.h>
#include <vwanim.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define iFramesForFocus 10
/// Review: how to create new VT_'s
#define VT_POINT 73
#define VT_RECT	74
#define VT_POINT_BYREF (VT_BYREF|VT_POINT)
#define CURSOR_TOOLTIP 25
#define MAIN_TOOLTIP 127
#define MENUCLICK_TOLERANCE 4

/////////////////////////////////////////////////////////////////////////////
// Utility functions

template<class CType>
void RELEASE( CType * &pType )
{
	if ( pType )
	{
		pType->Release();
		pType = NULL;
	}
}

#ifndef MMRELEASE
#define MMRELEASE(_ptr) do { if (_ptr != NULL) _ptr->Release(); _ptr = NULL; } while (0)
#endif


IMPLEMENT_DYNCREATE(CVWRenderViewCtrl, CVWUIView)

/////////////////////////////////////////////////////////////////////////////
CComBSTR	bstrAvatarCameraPosition("AvatarCameraPosition");
CComBSTR	bstrCameraPreference("CameraPreference");
CComBSTR	bstrGetFrame("GetFrame");
CComBSTR	bstrProfile("Profile");
CComBSTR	bstrBoundaryList("BoundaryList");
CComBSTR    bstrBoundaryUpdate(VWSTUDIO_BOUNDARYUPDATE_EVENT_STR);
CComBSTR    bstrEditingMode("EditingMode");
CComBSTR    bstrSetCamera("SetCamera");
/////////////////////////////////////////////////////////////////////////////
typedef CVWComPtr<IThing,NULL,&IID_IThing> CThingPtr;
typedef CVWComPtr<IBoundary,NULL,&IID_IThing> CBoundaryPtr;
typedef CVWComPtr<IPropertyList,NULL,&IID_IPropertyList> CPropertyListPtr;
typedef CVWComPtr<IVWFrame,NULL,&IID_IVWFrame> CVWFramePtr;
typedef CVWComPtr<IObjectProperty, NULL, &IID_IObjectProperty> CObjectPropertyPtr;

/////////////////////////////////////////////////////////////////////////////
// Forward declarations

UINT PaintThread(void *pvParam);

/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CVWRenderViewCtrl, CVWUIView)
	//{{AFX_MSG_MAP(CVWRenderViewCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CHAR()
	ON_WM_TIMER()
	ON_MESSAGE(WM_CONTROLGETFOCUS, OnControlGetFocus)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_NAVIGATE, OnUpdateToolsNavigate)

	ON_COMMAND(ID_TOOLS_EDITBOUNDARIES, OnToolsEditBoundaries)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_EDITBOUNDARIES, OnUpdateToolsEditBoundaries)
	
	ON_COMMAND(ID_TOOLS_NAVIGATE, OnToolsNavigate)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SELECT, OnUpdateToolsSelect)

	ON_COMMAND(ID_TOOLS_SELECT, OnToolsSelect)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_TRANSLATE, OnUpdateToolsTranslate)
	ON_COMMAND(ID_TOOLS_TRANSLATE, OnToolsTranslate)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ROTATE, OnUpdateToolsRotate)
	ON_COMMAND(ID_TOOLS_ROTATE, OnToolsRotate)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SCALE, OnUpdateToolsScale)
	ON_COMMAND(ID_TOOLS_SCALE, OnToolsScale)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateToolsUndo)
	ON_COMMAND(ID_EDIT_UNDO, OnToolsUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateToolsRedo)
	ON_COMMAND(ID_EDIT_REDO, OnToolsRedo)
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsShowOptions)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_OPTIONS, OnUpdateToolsShowOptions)
	ON_COMMAND(ID_TOOLS_GRAVITYTOGGLE, OnToolsGravity)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_GRAVITYTOGGLE, OnUpdateToolsGravity)
	ON_COMMAND(ID_TOOLS_COLLISIONTOGGLE, OnToolsCollisions)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_COLLISIONTOGGLE, OnUpdateToolsCollisions)
	ON_COMMAND(ID_VIEW_PERSPECTIVE, OnViewPerspective)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PERSPECTIVE, OnUpdateViewPerspective)
	ON_COMMAND(ID_VIEW_TOP, OnViewTop)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOP, OnUpdateViewTop)
	ON_COMMAND(ID_VIEW_RIGHT, OnViewRight)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RIGHT, OnUpdateViewRight)
	ON_COMMAND(ID_VIEW_FRONT, OnViewFront)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FRONT, OnUpdateViewFront)
	ON_COMMAND(ID_TOOLS_GRAVITYTOGGLE, OnToolsGravity)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_GRAVITYTOGGLE, OnUpdateToolsGravity)
	ON_COMMAND(ID_TOOLS_COLLISIONTOGGLE, OnToolsCollisions)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_COLLISIONTOGGLE, OnUpdateToolsCollisions)
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsShowOptions)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_OPTIONS, OnUpdateToolsShowOptions)
	ON_COMMAND(ID_TOOLS_SELECTCONTENTS, OnToolsSelectContents)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SELECTCONTENTS, OnUpdateToolsSelectContents)
	ON_COMMAND(ID_TOOLS_HELP, OnToolsHelp)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMOUT, OnUpdateToolsZoomOut)
	ON_COMMAND(ID_TOOLS_ZOOMOUT, OnToolsZoomOut)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMIN, OnUpdateToolsZoomIn)
	ON_COMMAND(ID_TOOLS_ZOOMIN, OnToolsZoomIn)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMNORMAL, OnUpdateToolsZoomNormal)
	ON_COMMAND(ID_TOOLS_ZOOMNORMAL, OnToolsZoomNormal)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_CREATEPOLYGONBOUNDARY, OnUpdateToolsCreatePolygonBoundary)
	ON_COMMAND(ID_TOOLS_CREATEPOLYGONBOUNDARY, OnToolsCreatePolygonBoundary)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_DELETE, OnUpdateToolsDelete)
	ON_COMMAND(ID_TOOLS_DELETE, OnToolsDelete)

	ON_UPDATE_COMMAND_UI(ID_TOOLS_DUMPBOUNDARYSCRIPT, OnUpdateToolsDumpToScript)
	ON_COMMAND(ID_TOOLS_DUMPBOUNDARYSCRIPT, OnToolsDumpToScript)


	
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNeedText)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_COMMAND_RANGE(WM_USER, (WM_USER+200), OnCommandRange)
	ON_UPDATE_COMMAND_UI_RANGE(WM_USER, (WM_USER+200), OnCommandRangeUI)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CVWRenderViewCtrl, CVWUIView)
	//{{AFX_DISPATCH_MAP(CVWRenderViewCtrl)
	DISP_PROPERTY_EX(CVWUIView, "Client", CVWUIView::GetVWClient, CVWUIView::SetVWClient, VT_DISPATCH)
	DISP_PROPERTY_EX(CVWUIView, "TargetObjectProperty", CVWUIView::GetTargetObjectProperty, CVWUIView::SetTargetObjectProperty, VT_DISPATCH)
	DISP_PROPERTY_EX(CVWUIView, "ControlManager", CVWUIView::GetControlManager, CVWUIView::SetControlManager, VT_DISPATCH)
	DISP_PROPERTY_EX(CVWRenderViewCtrl, "RenderRoot", GetVWRenderRoot, SetVWRenderRoot, VT_DISPATCH)
	DISP_PROPERTY_EX(CVWRenderViewCtrl, "ShowSelection", GetShowSelection, SetShowSelection, VT_BOOL)
	DISP_PROPERTY_EX(CVWRenderViewCtrl, "EditMode", GetEditMode, SetEditMode, VT_I4)
	DISP_PROPERTY_EX(CVWRenderViewCtrl, "ZoomLevel", GetZoomLevel, SetZoomLevel, VT_R4)
	DISP_PROPERTY_EX(CVWRenderViewCtrl, "ViewMode", GetViewMode, SetViewMode, VT_I4)
	DISP_PROPERTY_EX(CVWRenderViewCtrl, "ViewFilter", GetViewFilter, SetViewFilter, VT_I4)
	DISP_FUNCTION(CVWRenderViewCtrl, "LoadUIToolByProgID", LoadUIToolByProgID, VT_ERROR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CVWRenderViewCtrl, "AddCurrentUITool", AddCurrentUITool, VT_ERROR, VTS_BSTR)
	DISP_FUNCTION(CVWRenderViewCtrl, "ResetCameraPosition", ResetCameraPosition, VT_ERROR, VTS_NONE)
	DISP_FUNCTION(CVWRenderViewCtrl, "SetNavigationPath", SetNavigationPath, VT_EMPTY, VTS_DISPATCH VTS_DISPATCH)
	DISP_FUNCTION(CVWRenderViewCtrl, "SetCamera", SetCamera, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION(CVWRenderViewCtrl, "CenterViewOnPosition", CenterViewOnPosition, VT_ERROR, VTS_I2 VTS_I2)
	DISP_FUNCTION(CVWRenderViewCtrl, "GetUITool", GetUITool, VT_DISPATCH, VTS_BSTR)
	DISP_FUNCTION(CVWRenderViewCtrl, "IsUIToolCurrent", IsUIToolCurrent, VT_BOOL, VTS_BSTR )
	DISP_FUNCTION(CVWRenderViewCtrl, "CenterViewOnObject", CenterViewOnObject, VT_ERROR, VTS_DISPATCH VTS_BOOL)
	DISP_FUNCTION(CVWRenderViewCtrl, "ToolEvent", ToolEvent, VT_ERROR, VTS_I4 VTS_DISPATCH VTS_BSTR VTS_BSTR VTS_BSTR VTS_BOOL)

	DISP_STOCKPROP_READYSTATE()
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/*
	DISP_PROPERTY_EX(CVWRenderViewCtrl, "NavigationTool", GetVWNavigationTool, SetVWNavigationTool, VT_DISPATCH)
*/

/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CVWRenderViewCtrl, CVWUIView)
	//{{AFX_EVENT_MAP(CVWRenderViewCtrl)
	EVENT_CUSTOM("AlphaKeyPressed", FireAlphaKeyPressed, VTS_I4)
	EVENT_CUSTOM("ToolEvent", FireToolEvent, VTS_I4 VTS_DISPATCH VTS_BSTR VTS_BSTR VTS_BSTR)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CVWRenderViewCtrl, 1)
	PROPPAGEID(CVWRenderViewPropPage::guid)
END_PROPPAGEIDS(CVWRenderViewCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CVWRenderViewCtrl, "VWCLIENTUI.RenderViewCtrl.1",
	0x44fac783, 0xca4, 0x11d0, 0x89, 0xa9, 0, 0xa0, 0xc9, 0x5, 0x41, 0x29)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CVWRenderViewCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DVWRenderView =
		{ 0x44fac781, 0xca4, 0x11d0, { 0x89, 0xa9, 0, 0xa0, 0xc9, 0x5, 0x41, 0x29 } };
const IID BASED_CODE IID_DVWRenderViewEvents =
		{ 0x44fac782, 0xca4, 0x11d0, { 0x89, 0xa9, 0, 0xa0, 0xc9, 0x5, 0x41, 0x29 } };

/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwVWRenderViewOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CVWRenderViewCtrl, IDS_VWRENDERVIEW, _dwVWRenderViewOleMisc)

/////////////////////////////////////////////////////////////////////////////
// CVWRenderViewCtrl::CVWRenderViewCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CVWRenderViewCtrl

BOOL CVWRenderViewCtrl::CVWRenderViewCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
	{
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_VWRENDERVIEW,
			IDB_VWRENDERVIEW,
			afxRegInsertable | afxRegApartmentThreading,
			_dwVWRenderViewOleMisc,
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
// CVWRenderViewCtrl::CVWRenderViewCtrl - Constructor

CVWRenderViewCtrl::CVWRenderViewCtrl() : m_boundaryArray()
{
	InitializeIIDs(&IID_DVWRenderView, &IID_DVWRenderViewEvents);

	EnableConnections();

	// TODO: Initialize your control's instance data here.
	m_VWClient = NULL;
	m_pPaintThread = NULL;
	m_bQuit = FALSE;
	m_hDC = NULL;
	m_pVWRenderRoot = NULL;
	m_bNavigationToolIsCurrentlySelected = TRUE;  //The default for the client
	m_pVWNavigationTool = NULL;
	m_pVWPickTool = NULL;
	m_pVWGeometryCache = NULL;
	m_cxPaintInfo = m_cyPaintInfo = m_xPaintInfo = m_yPaintInfo = 0;
	m_pDD = NULL;
	m_pD3DRM = NULL;
	m_pD2DRM = NULL;
	m_pRenderingContext = NULL;
	m_hwndFocus = NULL;
	m_hwndParent = NULL;
	m_ToolTipShown = FALSE;
	m_CursorToolTipShown = FALSE;
	m_bRenderWireframe = FALSE;
	m_nEditingMode = FALSE;
	m_bShowSelection = FALSE;
	m_bEditingToolsCreated = FALSE;
	m_bRendererInitialized = FALSE;
	m_bUsingDX5 = TRUE;
	m_bNeedUpdateBoundaryScreenCoordinates = FALSE;
	m_bHaltRendering = FALSE;
	m_bRenderingHasHalted = TRUE;
	m_bWaitOneFrame = FALSE;
	m_pLastClickedPoint.x = m_pLastClickedPoint.y = -1;

	m_nViewFilter = 0;
	m_CameraMode = PERSPECTIVE;
	m_flZoom = m_flPerspectiveZoom = m_flTopZoom = m_flFrontZoom = 1.0f;

	m_flPixelsToWorldUnit = (float)20.0;
	m_bWasUsingFastMode = FALSE;

	//World events are too much overhead.
	//m_ulFlags |= ADVISE_WORLD;

	m_hAccelerator = NULL;
	m_hInterpMutex = CreateMutex(NULL,FALSE,NULL);
	m_fInterpTime = 0;
}


/////////////////////////////////////////////////////////////////////////////
// CVWRenderViewCtrl::~CVWRenderViewCtrl - Destructor

CVWRenderViewCtrl::~CVWRenderViewCtrl()
{
	// TODO: Cleanup your control's instance data here.
}

/////////////////////////////////////////////////////////////////////////////
// CVWRenderViewCtrl::OnDraw - Drawing function

void CVWRenderViewCtrl::OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (m_bRendererInitialized && m_nEditingMode != EDIT_BOUNDARIES )
	{
		m_pVWRenderRoot->HandlePaint(pdc->m_hDC);
	}
}


BOOL 
CVWRenderViewCtrl::OnGetColorSet(DVTARGETDEVICE *ptd, HDC hicTargetDev,	LPLOGPALETTE *ppColorSet)
{
//	TRACE("OnGetColorSet()\n");

	return FALSE;
}

// DoCreateTooltip - creates a ToolTip control and adds some tools 
void
CVWRenderViewCtrl::DoCreateTooltip() 
{ 
  //int row, col;   // rows and columns 

 
    // Ensure that the common control DLL is loaded, and create 
    // a ToolTip control. 
  //  InitCommonControls(); 
	CRect ClientRect(0,0,2000,2000);
	if (m_ToolTip.Create(this, TTS_ALWAYSTIP) && m_ToolTip.AddTool(this, LPSTR_TEXTCALLBACK, ClientRect, MAIN_TOOLTIP)) 
	{
		m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, SHRT_MAX);
		m_ToolTip.SendMessage(TTM_SETDELAYTIME, TTDT_AUTOPOP, SHRT_MAX);
		m_ToolTip.SendMessage(TTM_SETDELAYTIME, TTDT_INITIAL, 800);
		m_ToolTip.SendMessage(TTM_SETDELAYTIME, TTDT_RESHOW, 200);
	} 
	else 
	{
		TRACE("ERROR in creating ToolTip");
	}
	m_ToolTip.Activate(TRUE);

	if (m_CursorToolTip.Create(this, TTS_ALWAYSTIP) && m_CursorToolTip.AddTool(this, LPSTR_TEXTCALLBACK, ClientRect, CURSOR_TOOLTIP)) 
	{
		m_CursorToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, SHRT_MAX);
		m_CursorToolTip.SendMessage(TTM_SETDELAYTIME, TTDT_AUTOPOP, SHRT_MAX);
		m_CursorToolTip.SendMessage(TTM_SETDELAYTIME, TTDT_INITIAL, 50);
		m_CursorToolTip.SendMessage(TTM_SETDELAYTIME, TTDT_RESHOW, 200);
	} 
	else 
	{
		TRACE("ERROR in creating cursor ToolTip");
	}
	m_CursorToolTip.Activate(TRUE);
} 

void CVWRenderViewCtrl::OnSize(UINT nType, int cx, int cy) 
{
	if (m_bRendererInitialized) 
	{
//		TRACE("OnSize() cx = %d, cy = %d\n", cx, cy);

		if ((m_cxPaintInfo != cx) || (m_cyPaintInfo != cy))
		{
			//If the rendering is running, we need to halt it before the resize operation.
			//if (m_nEditingMode == EDIT_BOUNDARIES && !m_bRenderingHasHalted) //WaitForSingleObject(m_hGraphicsEnabled, 0) != WAIT_TIMEOUT)
			//{
			//	m_bHaltRendering = TRUE;
			//	while (m_bRenderingHasHalted == FALSE);
			//}

			// don't want to prompt a rebuild unless size actually changes...
			m_cxPaintInfo = cx;
			m_cyPaintInfo = cy;
			
			m_pVWRenderRoot->HandleSize(m_cxPaintInfo, m_cyPaintInfo);

			//m_bHaltRendering = FALSE;
		}

		// notify the paint thread that we are ready to start rendering
		SetEvent(m_hGraphicsEnabled);
	}

	CVWUIView::OnSize(nType, cx, cy);

	if (m_nEditingMode == EDIT_BOUNDARIES)
	{
		m_bWaitOneFrame = TRUE;
		while (m_bWaitOneFrame)
			Sleep(50);

		m_bNeedUpdateBoundaryScreenCoordinates = TRUE;
	}
}

void CVWRenderViewCtrl::OnDestroy()
{
	if (!m_bQuit)
	{
		m_bQuit = TRUE;

		// Make sure that the event is set (in case it hasn't been yet) so the paint thread can stop 
		// waiting and see that it is time to quit.
		SetEvent(m_hGraphicsEnabled);

		// wait for paint thread to die
		while (m_bQuit == TRUE)
			Sleep(100);

		m_bQuit = TRUE;
	}

	CVWUIView::OnDestroy();

	RenderRelease();
}

void CVWRenderViewCtrl::OnControlGetFocus(WPARAM wParam, LPARAM lParam)
{
	m_hwndFocus = ::GetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// Message handlers that delegate to tools
//
// REVIEW: Error handling?
//
#define MINMOVE 9
LRESULT CVWRenderViewCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bHandled = VARIANT_FALSE;
	MSG msg;

	//Initial the old MSG structure winuser.h for WM_MOUSEMOVE and TranslateAccelerator()
	msg.hwnd = m_hWnd;
	msg.message = message;
	msg.wParam = wParam;
	msg.lParam = lParam;

	if (message == WM_LBUTTONDOWN ||
		message == WM_MOUSEMOVE ||
		message == WM_LBUTTONUP ||
		message == WM_RBUTTONDOWN ||
		message == WM_MBUTTONDOWN ||
		message == WM_RBUTTONUP ||
		message == WM_MBUTTONUP)
	{
		msg.pt = CPoint(LOWORD(lParam), HIWORD(lParam));
		// m_ToolTip.RelayEvent(&msg);
		::SendMessage(m_ToolTip.m_hWnd, TTM_RELAYEVENT, 0, (LPARAM)(LPMSG)&msg);
		::SendMessage(m_CursorToolTip.m_hWnd, TTM_RELAYEVENT, 0, (LPARAM)(LPMSG)&msg);
	
		if (message == WM_MOUSEMOVE) 
		{
			CPoint diff;

			diff = m_ToolTipPos - CPoint(LOWORD(lParam), HIWORD(lParam));
			if ((diff.x * diff.x + diff.y * diff.y) > MINMOVE) 
			{
				if (m_CursorToolTipShown)
				{
					if (FAILED(hr = m_pVWNavigationTool->SetCursor(m_pVWUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_NAVIGATE) ) ) ))
						OutputDebugString("Failed to reset navigation cursor.\n");
					if (FAILED(hr = m_pVWPickTool->SetCursor(m_pVWUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_NAVIGATE) ) ) ))
						OutputDebugString("Failed to reset pick cursor.\n");

					m_CursorToolTip.Activate(TRUE);
					m_CursorToolTipShown = FALSE;
				}

				if (m_ToolTipShown) 
				{
					m_ToolTip.Activate(FALSE);
					m_ToolTip.Activate(TRUE);
					m_ToolTipShown = FALSE;
				}
			}
		}
	}

/*	if (m_nEditingMode)
	{
		if (message == WM_KEYDOWN || message == WM_SYSKEYDOWN && m_hAccelerator)  //m_hAccelerator == NULL before OnCreate is called
			TranslateAccelerator(m_hWnd, m_hAccelerator, &msg);

		if (message == WM_RBUTTONUP)
		{
		}
	}*/

	switch(message)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (m_hAccelerator && m_nEditingMode)
			TranslateAccelerator(m_hWnd, m_hAccelerator, &msg);
		break;
	case WM_RBUTTONDOWN:
		m_pLastClickedPoint = msg.pt;
		break;
	case WM_RBUTTONUP:
		if (m_nEditingMode && 
			abs(m_pLastClickedPoint.x - msg.pt.x) < MENUCLICK_TOLERANCE &&
			abs(m_pLastClickedPoint.y - msg.pt.y) < MENUCLICK_TOLERANCE)
		{
			// do the popup
			DoMenu(CPoint(LOWORD(lParam), HIWORD(lParam)));
			return CWnd::DefWindowProc( message, wParam, lParam);
		}
		break;
	case WM_FIRETOOLEVENT:
		ToolEventInfoStruct* pToolEventInfo;

		pToolEventInfo = (ToolEventInfoStruct*) wParam;
		FireToolEvent(	pToolEventInfo->nEventNum, 
						pToolEventInfo->pObject, 
						pToolEventInfo->szStr1, 
						pToolEventInfo->szStr2,
						pToolEventInfo->szStr3 );

		delete pToolEventInfo;
		break;
	case WM_LBUTTONDOWN:
		SetFocus();
//		if (m_pVWNavigationTool)
//			m_pVWNavigationTool->OnMessage(m_pVWUIView, WM_SETFOCUS, (WPARAM)NULL, (LPARAM)NULL, &bHandled);
		HandleOnMessage(WM_SETFOCUS, NULL, NULL );

		break;
	}

	bHandled = (VARIANT_BOOL) CVWUIView::WindowProc( message, wParam, lParam );

	return bHandled;
}

BOOL CVWRenderViewCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	HWND hWnd;
	HRESULT hr = S_OK;

	if (m_pVWUIView)
	{
		hr = m_pVWUIView->get_HWND(&hWnd);

		if (hWnd == pWnd->m_hWnd)
		{
			CVWUIView::HandleOnMessage( WM_SETCURSOR, message, nHitTest);
			return TRUE;
		}
	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CVWRenderViewCtrl::DoPropExchange - Persistence support

void CVWRenderViewCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	CVWUIView::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CVWRenderViewCtrl::OnResetState - Reset control to default state

void CVWRenderViewCtrl::OnResetState()
{
	CVWUIView::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CVWRenderViewCtrl message handlers

LPDISPATCH CVWRenderViewCtrl::GetVWRenderRoot() 
{
	// TODO: Add your property handler here
	if (m_pVWRenderRoot)
	{
		m_pVWRenderRoot->AddRef();
	}
	return m_pVWRenderRoot;
}

void CVWRenderViewCtrl::SetVWRenderRoot(LPDISPATCH newValue) 
{
	AfxMessageBox("Why are you trying to set the renderer via the RenderViewCtrl?");
}

LPDISPATCH CVWRenderViewCtrl::GetVWNavigationTool() 
{
	// TODO: Add your property handler here
	if (m_pVWNavigationTool)
	{
		m_pVWNavigationTool->AddRef();
	}
	return m_pVWNavigationTool;
}

void CVWRenderViewCtrl::SetVWNavigationTool(LPDISPATCH newValue) 
{
	AfxMessageBox("Why are you trying to set the navigation tool via the RenderViewCtrl?");
}

int CVWRenderViewCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	IVWRender3D* pRend3D = NULL;
	HRESULT hr = S_OK;
	LPDIRECT3DRM pD3DRM = NULL;

	if (CVWUIView::OnCreate(lpCreateStruct) == -1)
	{
		hr = -1;
		goto EXIT_FAIL;
	}

	// add some sync stuff...
	m_hGraphicsEnabled = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (m_hGraphicsEnabled == NULL)
	{
		hr = -1;
		goto EXIT_FAIL;
	}

	// Initialize the focus member variable.
	m_hwndFocus = ::GetFocus();

	// Our parent should remain the same for the lifetime of the control.
	m_hwndParent = ::GetParent(m_hWnd);

	// create paint thread
	m_pPaintThread = AfxBeginThread(PaintThread, this, THREAD_PRIORITY_BELOW_NORMAL);
	
	m_hAccelerator = LoadAccelerators(lpCreateStruct->hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));
	ASSERT(m_hAccelerator);

	// enable drop file
	EnableDropFile(TRUE);


	// Create the renderer (if we haven't already).
	CLSID clsidVWRenderRoot;
 	if (FAILED(hr = CLSIDFromProgID(CComBSTR("VWSYSTEM.RenderRoot.1"), &clsidVWRenderRoot)))
		goto EXIT_FAIL;
	if (FAILED(hr = CoCreateInstance(clsidVWRenderRoot, 
 								NULL,
 								CLSCTX_INPROC_SERVER,
 								IID_IVWRenderRoot,
 								(void **)&m_pVWRenderRoot)))
		goto EXIT_FAIL;

	// If OnConnect has already been called, do what we would have done in OnConnect.
	if (m_pWorld && FAILED(hr = HandleOnConnect(m_pWorld)))
		goto EXIT_FAIL;
	
EXIT_FAIL:

	return hr;
}

HRESULT
CVWRenderViewCtrl::RenderRelease()
{
	CString				strKeyName;
	IThing				*pThis = NULL;
	POSITION			pos;

	SAFERELEASE(m_pVWRenderRoot);
	m_bRendererInitialized = FALSE;
	SAFERELEASE(m_pVWGeometryCache);
	SAFERELEASE(m_pDD);
	SAFERELEASE(m_pD3DRM);
	SAFERELEASE(m_pD2DRM);
	SAFERELEASE(m_pRenderingContext);
	SAFERELEASE(m_pVWNavigationTool);
	SAFERELEASE(m_pVWPickTool);
	CloseHandle(m_hGraphicsEnabled);
	CloseHandle(m_hInterpMutex);
	pos = m_DoneMethodMap.GetStartPosition();
	while (pos!=NULL)
	{
		m_DoneMethodMap.GetNextAssoc(pos, strKeyName, pThis);
		SAFERELEASE(pThis);
	}
	pos = m_animList.GetHeadPosition();
	while (pos!=NULL) ((IUnknown*)(m_animList.GetNext(pos).Animation))->Release();
	m_animList.RemoveAll();
	return S_OK;
}

HRESULT
CVWRenderViewCtrl::RenderDoProcessing()
{
	VARIANT_BOOL	bInFastMode = VARIANT_FALSE;
	VARIANT_BOOL	bMoving = VARIANT_FALSE;
	VARIANT_BOOL	bWaitSetAtStartFrame;
	static int		iNeedFocus = 0;
	static DWORD dwFrameTime = 0;
	DWORD dwTime = 0;
	HRESULT hr = S_OK;


	if (!m_bRendererInitialized)
	{
		// Renderer not initialized yet - quite OK just wait for OnConnect.
		return S_OK;
	}

	ASSERT(m_pVWRenderRoot);

	// Do render stuff, as long as we're not paused and we're activated.
	// SK - have to tick the interpolations even without focus- does callbacks on time+indexing
	if (/*m_hwndFocus  &&*/ !m_bHaltRendering)
	{
		m_bRenderingHasHalted = FALSE;
		bWaitSetAtStartFrame = FALSE;
		if (m_bWaitOneFrame)
			bWaitSetAtStartFrame = TRUE;

		if(m_nEditingMode) 
			UpdateCurrentUITools();
		else
			m_pVWNavigationTool->Update();

		// REVIEW: This cast is a hack for now but will have to be changed when we can select 
		// multiple UI tools.
		// DWS change: Use cached pointer.
		m_pVWNavigationTool->get_Moving(&bMoving);
		m_pVWRenderRoot->put_FastMode(bMoving);

		// If we're moving and in boundary editing mode, set the screen coord update flag
		if (bMoving && m_nEditingMode == EDIT_BOUNDARIES)
			m_bNeedUpdateBoundaryScreenCoordinates = TRUE;

		dwTime = GetTickCount();
		if (m_pD2DRM)
		{	
			if (dwFrameTime > 0 && dwFrameTime < dwTime)
				m_pD2DRM->Tick(dwTime-dwFrameTime);
			else
				m_pD2DRM->Tick(100);
		}
		dwFrameTime = dwTime;

		WaitForSingleObject(m_hInterpMutex,INFINITE);

		POSITION pos = m_animList.GetHeadPosition();
		POSITION pos_tmp;
		DWORD dwOffset;
		int		iPSwitch = 0;
		IDirect3DRMInterpolator* pInterp = NULL;
		D3DAnimation* d3dAnim = NULL;
		IDirect3DRMAnimation2* pAnim = NULL;
		IDirect3DRMAnimationSet2* pAnimSet = NULL;
		float fTime = (float)dwTime;

		while (pos != NULL) 
		{
			d3dAnim = &(m_animList.GetNext(pos));

			dwOffset = 0l;
			if (dwTime-(d3dAnim->StartTime) < 0) 
				dwOffset = 0xFFFFFFFF;
			
			//get the correct animation or interpolation pointer and type
			if (SUCCEEDED(((IUnknown*)(d3dAnim->Animation))->QueryInterface(IID_IDirect3DRMAnimation2,(void**)&pAnim))) 
			{
				iPSwitch = 0;
				// Extra AddRef from doing QI, need to release it.
				((IUnknown*)d3dAnim->Animation)->Release();
			}
			else if (SUCCEEDED(((IUnknown*)(d3dAnim->Animation))->QueryInterface(IID_IDirect3DRMAnimationSet2,(void**)&pAnimSet)))
			{
				iPSwitch = 1;
				// Extra AddRef from doing QI, need to release it.
				((IUnknown*)d3dAnim->Animation)->Release();
			}
			else 
			{   //we had better be sure this is an interpolation!!!
				pInterp = (IDirect3DRMInterpolator*)d3dAnim->Animation;
				iPSwitch = 2;
			}
			float fElapsedTime;  
			//Get elapsed time in animation local time
			if (d3dAnim->fIndex)
				fElapsedTime = (((float)(dwTime - d3dAnim->StartTime + dwOffset))* d3dAnim->fTimeScale) + d3dAnim->fIndex;
			else
				fElapsedTime = (((float)(dwTime - d3dAnim->StartTime + dwOffset))* d3dAnim->fTimeScale) + d3dAnim->fRangeStart;
				
			if (fElapsedTime < d3dAnim->fRangeEnd)
			{
				switch(iPSwitch)
				{
					case 0:
						pAnim->SetTime( fElapsedTime );
						break;
					case 1:
						pAnimSet->SetTime( fElapsedTime ); 
						break;
					case 2:
						pInterp->Interpolate( fElapsedTime,NULL,(d3dAnim->Options));
						break;
				}
			}		
			else	//we are either done or closed
			{
				if(d3dAnim->Options & D3DRMANIMATION_CLOSED)
				{
					//loop the animation
					d3dAnim->StartTime = GetTickCount();
					d3dAnim->fIndex = 0.0f;
					switch(iPSwitch)
					{
						case 0:
							pAnim->SetTime( d3dAnim->fRangeStart );
							break;
						case 1:
							pAnimSet->SetTime( d3dAnim->fRangeStart ); 
							break;
						case 2:
							pInterp->Interpolate( d3dAnim->fRangeStart ,NULL,(d3dAnim->Options));
							break;
					}
				}
				else 
				{
					//tick the animation at the last position, do the callback if any
					switch(iPSwitch)
					{
						case 0:
							pAnim->SetTime( d3dAnim->fRangeEnd );
							break;
						case 1:
							pAnimSet->SetTime( d3dAnim->fRangeEnd ); 
							break;
						case 2:   
							// SK: changed this since nearest interps didnt get to last frame
							//but can't let non-nearest interps set to last frame due to D3D interp bug
							if ((d3dAnim->Options) & 256)
								pInterp->Interpolate( (d3dAnim->fRangeEnd), NULL, (d3dAnim->Options) );
							else
								pInterp->Interpolate( (d3dAnim->fRangeEnd - 0.001f), NULL, (d3dAnim->Options) );
							break;
					}
					if (d3dAnim->bstrAnimDoneMethod.Length() > 0)
					{
						CComBSTR bstrTmp;
						bstrTmp = d3dAnim->bstrAnimDoneMethod.m_str;
						m_DoneMethodMap.SetAt(CString(d3dAnim->bstrAnimDoneMethod), d3dAnim->pThis);
						// m_DoneMethodMap will not AddRef pThing, need to do it explicitly.
						SAFEADDREF(d3dAnim->pThis.p);
						SetTimer((UINT)bstrTmp.m_str, 0, NULL);
					}		
					//get the correct pointer to release
					switch(iPSwitch)
					{
						case 0:
							SAFERELEASE(pAnim);  //pAnim->Release();
							break;
						case 1:
							SAFERELEASE(pAnimSet);//pAnimSet->Release();
							break;
						case 2:
							SAFERELEASE(pInterp); //pInterp->Release();
							break;
					}
					// delete the animation from the list of current anims
					pos_tmp = pos;
					if (pos == NULL) 
						pos = m_animList.GetTailPosition();
					else 
						m_animList.GetPrev(pos);
					m_animList.RemoveAt(pos);
					pos = pos_tmp;
				}
			}
		}

		m_fInterpTime = fTime;
		ReleaseMutex(m_hInterpMutex);
		//End tick interpolations and animations
		//only really render if we have focus
		if (m_hwndFocus)
		{
			m_pVWRenderRoot->Render();

			if (m_nEditingMode == EDIT_BOUNDARIES)
				DrawBoundaries();

			m_pVWRenderRoot->Update();
		}
		// Textures HAVE to be released in this thread after rendering update due to a
		// D3D bug where textures used by the render operation can be deleted before the
		// update and then crash the update operation.
		if (m_pD2DRM)
			m_pD2DRM->ReleaseTextures();

		if (bWaitSetAtStartFrame)
			m_bWaitOneFrame = FALSE;
	}
	else
	{
		//Make SURE we're not going to get into a hanging state.
		m_bWaitOneFrame = FALSE;

		// Don't hog resources!
		if (m_bHaltRendering)
		{
			m_bRenderingHasHalted = TRUE;
			Sleep(50);
		}
		else 
			Sleep(100);
	}
	if (iNeedFocus++ == iFramesForFocus)
	{
		::PostMessage(m_hWnd, WM_CONTROLGETFOCUS, (WPARAM)0, (LPARAM)0);
		iNeedFocus = 0;
	}

	return S_OK;
}

// SK: this timer callback does the ondone method invokes on the animating objects
void CVWRenderViewCtrl::OnTimer(UINT nProperty) 
{
	if (nProperty == 3) // Is this message coming from the boundary editor instead of an animation?
		return;

	//block the render thread from altering this property map while we're here
	WaitForSingleObject(m_hInterpMutex,INFINITE);

	CString				strKeyName;
	IThing				*pThis = NULL;
	int					nLen;
	POSITION			pos;
	HRESULT hr = S_OK;
	static CString		strPosition("OnPositionDone");
	static CString		strDirection("OnDirectionDone");
	static CString		strScale("OnScaleDone");
	static CString		strUp("OnUpDone");
	static CString		strApproach("OnApproachDone");

	if (!m_DoneMethodMap.GetCount())
		goto ERROR_ENCOUNTERED;

	pos = m_DoneMethodMap.GetStartPosition();
	
	while (pos)
	{
		m_DoneMethodMap.GetNextAssoc(pos, strKeyName, pThis);
		if (!pThis)
		{	
			m_DoneMethodMap.RemoveKey(strKeyName);
			goto ERROR_ENCOUNTERED;
		}

		nLen = strKeyName.GetLength();
		if (nLen > 0)
		{
			//we have to distinguish between an OnXXXDone call (no args)
			//and a custom animationDone call which takes 2 args		
			if (	((strKeyName.CompareNoCase(strPosition)==0)|| (strKeyName.CompareNoCase(strDirection)==0)) ||
					((strKeyName.CompareNoCase(strScale)==0)	|| (strKeyName.CompareNoCase(strUp)==0)    || (strKeyName.CompareNoCase(strApproach)==0)))
			{
				pThis->InvokeMethodExt(CComBSTR(strKeyName), NULL, NULL);
				m_DoneMethodMap.RemoveKey(strKeyName);
			}
			else
			{	
				//pass VARIANT_TRUE and the current time to the method
				pThis->InvokeMethodExt(CComBSTR(strKeyName), CComDISPPARAMS(2, CComVariant((bool)VARIANT_TRUE), CComVariant(CurrentTime(pThis) )), NULL);
				m_DoneMethodMap.RemoveKey(strKeyName);
			}   
		}	
		SAFERELEASE(pThis);
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pThis);
	KillTimer(nProperty);
	//let the render thread start again
	ReleaseMutex(m_hInterpMutex);
	return;
}


/////////////////////////////////////////////////////////////////////////////
// CVWRenderViewCtrl::GetControlFlags -
// Flags to customize MFC's implementation of ActiveX controls.
//
// For information on using these flags, please see MFC technical note
// #nnn, "Optimizing an ActiveX Control".
DWORD CVWRenderViewCtrl::GetControlFlags()
{
	// BEGIN CVWUIView inheritence changes
	// DWORD dwFlags = COleControl::GetControlFlags();
	DWORD dwFlags = CVWUIView::GetControlFlags();
	// END CVWUIView inheritence changes



	// The control will not be redrawn when making the transition
	// between the active and inactivate state.
	dwFlags |= noFlickerActivate;

	// The control can receive mouse notifications when inactive.
	// TODO: if you write handlers for WM_SETCURSOR and WM_MOUSEMOVE,
	//		avoid using the m_hWnd member variable without first
	//		checking that its value is non-NULL.
	dwFlags |= pointerInactive;

	// The control can optimize its OnDraw method, by not restoring
	// the original GDI objects in the device context.
	dwFlags |= canOptimizeDraw;
	return dwFlags;
}

HRESULT CVWRenderViewCtrl::HandleOnUserConnect(IThing* pthing, BOOL bFromClient)
{
	IThing	*pContainer = NULL, *pUser = NULL;
	IWorld	*pWorld = NULL;
	HRESULT	hr = S_OK;

	CVWViewCtrl::HandleOnUserConnect(pthing, bFromClient);

	// Get the new world.
	if (FAILED(hr = pthing->get_World(&pWorld)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pWorld->get_User(&pUser)))
		goto ERROR_ENCOUNTERED;

	if (pthing != pUser)
		goto ERROR_ENCOUNTERED;

	if (m_pWorld != pWorld)
	{
		// OK, this is a reconnect. Do nothing - HandleOnUserReconnect will do all.
		goto ERROR_ENCOUNTERED;
	}

	// This OCX is designed to render from the context of the connected user.
	m_pRenderingContext = pthing;
	pthing->AddRef();

	// Does the connected user have a location yet?
	if (FAILED(hr = m_pRenderingContext->get_Container(&pContainer)))
		goto ERROR_ENCOUNTERED;

	// If the connected user has a location, we need to build a scene for that location,
	// otherwise the scene will be built when the user enters a location.
	if (pContainer)
	{
		// Emulate an on enter to effect the scene build.
		if (FAILED(hr = HandleOnEnterRoom(pContainer, m_pRenderingContext)))
			goto ERROR_ENCOUNTERED;
	}
		
ERROR_ENCOUNTERED:

	SAFERELEASE(pContainer);
	SAFERELEASE(pWorld);
	SAFERELEASE(pUser);

	return hr;
}

HRESULT CVWRenderViewCtrl::HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient)
{
	IThing	*pContainer = NULL, *pUser = NULL;
	IWorld	*pWorld = NULL;
	HRESULT	hr = S_OK;

	CVWViewCtrl::HandleOnUserDisconnect(pthing, bFromClient);

	if (m_pRenderingContext)
	{
		if (pthing != m_pRenderingContext)
			goto ERROR_ENCOUNTERED;

		// Does the connected user have a location?
		if (FAILED(hr = m_pRenderingContext->get_Container(&pContainer)))
			goto ERROR_ENCOUNTERED;

		// If the connected user has a location, we need to destroy the scene for that location.
		if (pContainer)
		{
			// Does the connected user have a location?
			SAFERELEASE(pContainer);
			if (FAILED(hr = m_pRenderingContext->get_Container(&pContainer)))
				goto ERROR_ENCOUNTERED;

			// If the connected user has a location, we need to destroy the scene for that location.
			if (pContainer)
			{
				// Emulate an on leave to effect the scene destruction.
				if (FAILED(hr = HandleOnLeaveRoom(pContainer, m_pRenderingContext)))
					goto ERROR_ENCOUNTERED;
			}
		}

		// This OCX is designed to render from the context of the connected user.
		SAFERELEASE(m_pRenderingContext);
	}

	// Destroy the geometry cache in case we re-connect (it caches frames created in 
	// the old world).
	SAFERELEASE(m_pVWGeometryCache);

ERROR_ENCOUNTERED:

	SAFERELEASE(pContainer);
	SAFERELEASE(pWorld);
	SAFERELEASE(pUser);

	return hr;
}

HRESULT CVWRenderViewCtrl::HandleOnUserReconnect(IVWClient* newClient, IWorld* pWorld, IThing* pthing)
{
	HRESULT hr = S_OK;

 	if (FAILED(hr = HandleOnConnect(pWorld)))
		return hr;
 	return HandleOnUserConnect(pthing, VARIANT_TRUE);
}

HRESULT CVWRenderViewCtrl::HandleOnConnect(IDispatch* pWorld)
{
	VARIANT_BOOL		bHandled = VARIANT_FALSE;
	HRESULT				hr = S_OK;

	if (!m_pVWRenderRoot)
	{
		// This can be NULL if received OnConnect BEFORE OnCreate. OnCreate will do the work.
		return S_OK;
	}

	// Create the graphics tools (if not already created) and put them into the world 
	// if they haven't been already by another OCX, otherwise just get them from the world.
	if (FAILED(hr = CreateGraphicsTools((IWorld*)pWorld, &m_pDD, &m_pD2DRM, &m_pD3DRM, &m_pVWGeometryCache)))
		goto ERROR_ENCOUNTERED;
	
	// We will need the UI tool map before we create any tools.
	if (FAILED(hr = CreateHelperComponents(VARIANT_FALSE)))
			goto ERROR_ENCOUNTERED;

	if (!m_bRendererInitialized)
	{
		// Initialise the renderer for this HWND and with the objects.
		if (FAILED(hr = m_pVWRenderRoot->Init(m_hWnd, m_pDD, m_pD3DRM, m_pD2DRM)))
			goto ERROR_ENCOUNTERED;

		// Create the navigation tool.
		if (FAILED(hr = LoadUIToolByProgID(CString("VWSYSTEM.NavigationTool3D.1"), CString(NAVTOOLNAME))))
			goto ERROR_ENCOUNTERED;

		// And select it as the current tool.
		if (FAILED(hr = AddCurrentUITool(CString(NAVTOOLNAME))))
			goto ERROR_ENCOUNTERED;

		// sdrucker
		if (FAILED(hr = LoadUIToolByProgID(CString("VWSYSTEM.PickTool.1"), CString(PICKTOOLNAME))))
			goto ERROR_ENCOUNTERED;
		// And select it as the current tool.

		if (FAILED(hr = AddCurrentUITool(CString(PICKTOOLNAME))))
			goto ERROR_ENCOUNTERED;

// DWS change: Cache pointer to Nav tool.
		ASSERT(m_pVWUIView);
		if (FAILED(hr = m_pVWUIView->get_LoadedUITool(CComBSTR(PICKTOOLNAME),(IVWUITool**)&m_pVWPickTool)))
			goto ERROR_ENCOUNTERED;
		
		if (FAILED(hr = m_pVWUIView->get_LoadedUITool(CComBSTR(NAVTOOLNAME),(IVWUITool**)&m_pVWNavigationTool)))
			goto ERROR_ENCOUNTERED;
			
		//Initialize with the UIView.
		m_pVWNavigationTool->Initialize(m_pVWUIView, NULL, m_pVWRenderRoot);

		// Tell the navigation tool we have focus since if we already have it it won't 
		// receive the message.
		if (m_hWnd == ::GetFocus())
			m_pVWNavigationTool->OnMessage(m_pVWUIView, WM_SETFOCUS, 
										   (WPARAM)NULL, (LPARAM)NULL, &bHandled);

		// Initialize the navigation tool for with the renderer.
		// REVIEW: This cast is a hack for now but will have to be changed when we can select 
		// multiple UI tools.
		// DWSS change: Used cached pointer to Nav tool.
		// SD: remove pick tool from initialization
		//if (FAILED(hr = m_pVWNavigationTool->Init(m_pVWRenderRoot)))
		//	goto ERROR_ENCOUNTERED;

		// SD: SINCE Pick Tool is not being Inited from the navtool any more, 
		// we need to make sure that it's inited
		if (FAILED(hr = m_pVWPickTool->Initialize(m_pVWUIView, m_pWorld, m_pVWRenderRoot)))
			goto ERROR_ENCOUNTERED;


		//Set the default navigation cursor
		if (FAILED(hr = m_pVWNavigationTool->SetCursor(m_pVWUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_NAVIGATE) ) ) ))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = m_pVWPickTool->SetCursor(m_pVWUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_NAVIGATE) ) ) ))
			goto ERROR_ENCOUNTERED;

		// add the studio UI Tools
		if (m_nEditingMode)
			CreateEditingTools();

		m_bRendererInitialized = TRUE;

		RECT r;
		GetClientRect(&r);
		OnSize(0, r.right, r.bottom);
		DoCreateTooltip();
	}
	else
	{
		// Must place nav tool and pick tool into new world.
		if (FAILED(hr = ((IWorld*)pWorld)->CreateToolFromInterface(CComBSTR(PICKTOOLNAME), m_pVWPickTool)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = ((IWorld*)pWorld)->CreateToolFromInterface(CComBSTR(NAVTOOLNAME), m_pVWNavigationTool)))
			goto ERROR_ENCOUNTERED;

		if( FAILED(hr= m_pmapLoadedUITools->put_Dispatch( CComBSTR(PICKTOOLNAME), (IDispatch*) m_pVWPickTool )))
			goto ERROR_ENCOUNTERED;
		if( FAILED(hr= m_pmapLoadedUITools->put_Dispatch( CComBSTR(NAVTOOLNAME), (IDispatch*) m_pVWNavigationTool )))
			goto ERROR_ENCOUNTERED;

		if (FAILED(hr = AddCurrentUITool(CString(NAVTOOLNAME))))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = AddCurrentUITool(CString(PICKTOOLNAME))))
			goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:

	return hr;
}

HRESULT CVWRenderViewCtrl::HandleOnDisconnect(IDispatch* pWorld)
{
	ReleaseHelperComponents();

	m_bEditingToolsCreated = FALSE;

	return S_OK;
}

//Resets the camera position, respecting the avatar's profile.
SCODE CVWRenderViewCtrl::ResetCameraPosition()
{
	HRESULT hr = S_OK;
	D3DVECTOR					vecStartPos, vecStartDir;
	CString						strPref, strCameraMode;
	COleVariant					var;
	IVWFrame					*pvwfCamera = NULL;
	IAvatarProfile*				pProfile = NULL;
	IThing*						pCameraThing = NULL, *pRoom = NULL;
	IVWFrame					*pvwfRenderingContext = NULL;

	vecStartPos.x = 0.0f;
	vecStartPos.y = 0.7f;
	vecStartPos.z = 0.0f;

	vecStartDir.x = 0.0f;
	vecStartDir.y = 0.0f;
	vecStartDir.z = 1.0f;

	// Get the rendering context's loaded geometry.
	if (FAILED(hr = m_pRenderingContext->InvokeMethodExt(bstrGetFrame, NULL, &var)))
		goto ERROR_ENCOUNTERED;
	if (var.vt != VT_DISPATCH || var.pdispVal == NULL)
	{
		// Getting the location's geometry should have gotten the rendering context's geometry.
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	pvwfRenderingContext = (IVWFrame*)var.pdispVal;
	pvwfRenderingContext->AddRef();

	// Get the camera from the renderer.
	if (FAILED(hr = m_pVWRenderRoot->get_Camera(&pvwfCamera)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = m_pRenderingContext->get_ObjectProperty(CComBSTR("Profile"), (IObjectProperty **)&pProfile)) || !pProfile)
		goto ERROR_ENCOUNTERED;

	var.Clear();
	hr = pProfile->get_KeyValue(bstrAvatarCameraPosition, &var);
	if (FAILED(hr))
	{
		TRACE("Warning: Error getting avatar profile camera position.\n");
		hr = S_OK;
		var.Clear();
	}

	if (var.vt == VT_DISPATCH)
	{
		if (FAILED(hr = ((IVector*)var.pdispVal)->get(&vecStartPos.x, &vecStartPos.y, &vecStartPos.z)))
			goto ERROR_ENCOUNTERED;
	}

	var.Clear();
	hr = pProfile->get_KeyValue(bstrCameraPreference, &var);
	if (FAILED(hr))
	{
		TRACE("Warning: Error getting avatar profile camera preference.\n");
		hr = S_OK;
		var.Clear();
	}

	if (var.vt == VT_BSTR)
	{
		strPref = var.bstrVal;

		strCameraMode.LoadString(IDS_WINGMAN);
		if (strPref == strCameraMode)  //Set camera relative to the default position
		{
			vecStartPos.x += 0.5f;
			vecStartPos.y += 0.3f;
			vecStartPos.z += -3.0f;

			if (FAILED(hr = pvwfCamera->SetPositionEx(pvwfRenderingContext, vecStartPos.x, vecStartPos.y, vecStartPos.z)))
				goto ERROR_ENCOUNTERED;
			if (FAILED(hr = pvwfCamera->SetOrientationEx(pvwfRenderingContext, vecStartDir.x, vecStartDir.y, vecStartDir.z, 0.0F, 1.0F, 0.0F)))
				goto ERROR_ENCOUNTERED;

		}
		else
		{
			strCameraMode.LoadString(IDS_FACING);
			if (strPref == strCameraMode)
			{
				vecStartPos.x += 0.0f;
				vecStartPos.y += -0.5f;
				vecStartPos.z += 3.0f;
				vecStartDir.z = -1.0f;
				if (FAILED(hr = pvwfCamera->SetPositionEx(pvwfRenderingContext, vecStartPos.x, vecStartPos.y, vecStartPos.z)))
					goto ERROR_ENCOUNTERED;
				if (FAILED(hr = pvwfCamera->SetOrientationEx(pvwfRenderingContext, vecStartDir.x, vecStartDir.y, vecStartDir.z, 0.0F, 1.0F, 0.0F)))
					goto ERROR_ENCOUNTERED;

			}
			else
			{
				strCameraMode.LoadString(IDS_FIRSTPERSON);
				if (strPref == strCameraMode)
				{
					if (FAILED(hr = pvwfCamera->SetPositionEx(pvwfRenderingContext, vecStartPos.x, vecStartPos.y, vecStartPos.z)))
						goto ERROR_ENCOUNTERED;
					if (FAILED(hr = pvwfCamera->SetOrientationEx(pvwfRenderingContext, vecStartDir.x, vecStartDir.y, vecStartDir.z, 0.0F, 1.0F, 0.0F)))
						goto ERROR_ENCOUNTERED;
				}
				else
				{
					//Try to find the object name in the room and set the camera on it.
					if (FAILED(hr = m_pRenderingContext->get_Container(&pRoom)) || !pRoom)
						goto ERROR_ENCOUNTERED;
					
					if (FAILED(hr = pRoom->FindWithin(CComBSTR(strPref), CComVariant((bool)VARIANT_TRUE), &pCameraThing)) || !pCameraThing)
						goto ERROR_ENCOUNTERED;

					SetCamera(pCameraThing);
				}
			}
		}
	}
	else
	{
	}

	// Camera at rendering context's eye's looking forward.
	// REVIEW: This should NOT be hard coded. If nothing else it is dependent on the
	// avatar's size/scale.

	m_vecLastPerspectivePosition.x = vecStartPos.x;
	m_vecLastPerspectivePosition.y = vecStartPos.y;
	m_vecLastPerspectivePosition.z = vecStartPos.z;
	m_vecLastPerspectiveDirection.x = 0.0f;
	m_vecLastPerspectiveDirection.y = 0.0f;
	m_vecLastPerspectiveDirection.z = 1.0f;

	goto EXIT_SUCCESSFUL;

ERROR_ENCOUNTERED:
	if (pvwfCamera)
	{
		if (FAILED(hr = pvwfCamera->SetPositionEx(pvwfRenderingContext, vecStartPos.x, vecStartPos.y, vecStartPos.z)))
			goto EXIT_SUCCESSFUL;
		if (FAILED(hr = pvwfCamera->SetOrientationEx(pvwfRenderingContext, vecStartDir.x, vecStartDir.y, vecStartDir.z, 0.0F, 1.0F, 0.0F)))
			goto EXIT_SUCCESSFUL;
	}

EXIT_SUCCESSFUL:
	SAFERELEASE(pRoom);
	SAFERELEASE(pCameraThing);
	SAFERELEASE(pvwfRenderingContext);
	SAFERELEASE(pvwfCamera);
	SAFERELEASE(pProfile);

	return hr;
}


HRESULT CVWRenderViewCtrl::HandleOnEnterRoom(IThing* pWhere, IThing* pWhat)
{
	IVWFrame					*pvwfCamera = NULL;
	IVWFrame					*pVWFrame = NULL, *pvwfRenderingContext = NULL;
	COleVariant					var;
	DISPPARAMS					paramsSound, paramsComposeScene;
	static CComBSTR				bstrComposeScene("ComposeScene");
	static CComBSTR				bstrRenderSound("RenderSound");
	static CComBSTR				bstrBackgroundColor("BackgroundColor");
	static CComBSTR				bstrStartComposit = VW_STARTCOMPOSIT_EVENT_STR;
	static CComBSTR				bstrFinishComposit = VW_FINISHCOMPOSIT_EVENT_STR;
	CComVariant					varRGBBackground;
	HRESULT						hr = S_OK;

	// Initialize dispparams so TerminateDispparams won't fail if not used.
	paramsSound.rgvarg = NULL;
	paramsComposeScene.rgvarg = NULL;

	// This OCX is designed to render from the context of the connected user.
	if (pWhat == m_pRenderingContext)
	{
		m_pWorld->FireUIEvent(pWhat, bstrStartComposit, CComVariant());

		// Package up args for sound stuff into DISPPARAMS struct.
		if (FAILED(hr = InitializeDISPPARAMS(&paramsSound, 1)))
			goto ERROR_ENCOUNTERED;
		paramsSound.rgvarg[0].vt = VT_I4;
		paramsSound.rgvarg[0].lVal = 0;

		// We must push the location's geometry into the renderer and deal with the camera.
		// Get the location's geometry.
		// Package up args for ComposeScene into DISPPARAMS struct.
		if (FAILED(hr = InitializeDISPPARAMS(&paramsComposeScene, 1)))
			goto ERROR_ENCOUNTERED;
		paramsComposeScene.rgvarg[0].vt = VT_DISPATCH;
		paramsComposeScene.rgvarg[0].pdispVal = pWhat;
		if (FAILED(hr = pWhere->InvokeMethodExt(bstrComposeScene, &paramsComposeScene, &var)))
			goto ERROR_ENCOUNTERED;
		if (var.vt != VT_DISPATCH || var.pdispVal == NULL)
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		pVWFrame = (IVWFrame*)var.pdispVal;
		pVWFrame->AddRef();
		VariantClear(&var);

		// Get the rendering context's loaded geometry.
		if (FAILED(hr = m_pRenderingContext->InvokeMethodExt(bstrGetFrame, NULL, &var)))
			goto ERROR_ENCOUNTERED;
		if (var.vt != VT_DISPATCH || var.pdispVal == NULL)
		{
			// Getting the location's geometry should have gotten the rendering context's geometry.
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		pvwfRenderingContext = (IVWFrame*)var.pdispVal;
		pvwfRenderingContext->AddRef();

		// Place the location's geometry into the renderer.
		if (FAILED(hr = m_pVWRenderRoot->put_Scene(pVWFrame)))
			goto ERROR_ENCOUNTERED;

		// Get the location's background color and set the scene's background 
		// color from it.
		// REVIEW: alternatively, get the background bitmap.
		pWhere->get_Property(bstrBackgroundColor, &varRGBBackground);
		if (FAILED(varRGBBackground.ChangeType(VT_I4)))
		{
			varRGBBackground.Clear();
			varRGBBackground.vt = VT_I4;
			varRGBBackground.lVal = 0;
		}
		BYTE r = (BYTE) (varRGBBackground.lVal >> 16) & 0xFF;
		BYTE g = (BYTE) (varRGBBackground.lVal >> 8) & 0xFF;
		BYTE b = (BYTE) varRGBBackground.lVal & 0xFF;
		if (FAILED(hr = m_pVWRenderRoot->SetBackgroundColor(r, g, b)))
			goto ERROR_ENCOUNTERED;

		// Get the camera from the renderer.
		if (FAILED(hr = m_pVWRenderRoot->get_Camera(&pvwfCamera)))
			goto ERROR_ENCOUNTERED;

		// Attach the camera to the rendering context.
		if (FAILED(hr = pvwfRenderingContext->AddChild(pvwfCamera)))
			goto ERROR_ENCOUNTERED;

		SAFERELEASE(pvwfCamera);

		ResetCameraPosition();

		// Tell the location to render its sound (ambient)
		pWhere->InvokeMethodExt(bstrRenderSound, &paramsSound, NULL);

		// And finally associate the navigation target with the rendering context.
		if (FAILED(hr = m_pVWNavigationTool->put_NavigationTarget(m_pRenderingContext)))
			goto ERROR_ENCOUNTERED;

		CVWUIView::SetTargetObjectProperty(pWhere);

		m_pWorld->FireUIEvent(pWhat, bstrFinishComposit, CComVariant());

		InitializeEditingTools();
	}
ERROR_ENCOUNTERED:

	TerminateDISPPARAMS(VARIANT_FALSE, &paramsSound);
	TerminateDISPPARAMS(VARIANT_FALSE, &paramsComposeScene);
	SAFERELEASE(pVWFrame);
	SAFERELEASE(pvwfCamera);
	SAFERELEASE(pvwfRenderingContext);

	return hr;
}

HRESULT CVWRenderViewCtrl::HandleOnLeaveRoom(IThing* pWhere, IThing* pWhat)
{
	DISPPARAMS					paramsSound;
	static CComBSTR				bstrUnrenderSound("UnrenderSound");
	HRESULT						hr = S_OK;
	IWorld*						pWorld = NULL;
	IThing*						pGlobal = NULL;

	// Initialize dispparams so TerminateDispparams won't fail if not used.
	paramsSound.rgvarg = NULL;

	// This OCX is designed to render from the context of the connected user.
	if (pWhat == m_pRenderingContext)
	{
		//Turn off editing boundaries temporarily.
		if (m_nEditingMode != EDIT_OFF)
		{
			if (FAILED(hr = m_pRenderingContext->get_World(&pWorld)))
				goto ERROR_ENCOUNTERED;

			if (FAILED(hr = pWorld->get_Global(&pGlobal)))
				goto ERROR_ENCOUNTERED;

			if (FAILED(hr = pGlobal->put_Long(bstrEditingMode, EDIT_OFF)))
				goto ERROR_ENCOUNTERED;

			//SetEditMode(0);
		}

		//Clear and release the selected geometry list
		m_listSelected.ClearList();

		//Clear selection list
		if (m_pControlManager)
			hr = m_pControlManager->ClearSelectionsExt( m_pVWUIView );

		// Disassociate navigation.
		if (FAILED(hr = m_pVWNavigationTool->put_NavigationTarget(NULL)))
			goto ERROR_ENCOUNTERED;

		// Set the scene to null.
		if (FAILED(hr = m_pVWRenderRoot->put_Scene(NULL)))
			goto ERROR_ENCOUNTERED;

		// Tell the location to remove its sound (ambient)
		// Package up args for sound stuff into DISPPARAMS struct.
		if (FAILED(hr = InitializeDISPPARAMS(&paramsSound, 1)))
			goto ERROR_ENCOUNTERED;
		paramsSound.rgvarg[0].vt = VT_I4;
		paramsSound.rgvarg[0].lVal = 0;
		pWhere->InvokeMethodExt(bstrUnrenderSound, &paramsSound, NULL);

		//m_flZoom = m_flPerspectiveZoom = 1.0f;
		//if (m_nEditingMode == EDIT_BOUNDARIES)
		//	RebuildBoundaryArray();

	}

ERROR_ENCOUNTERED:

	if (paramsSound.rgvarg != NULL)
		TerminateDISPPARAMS(VARIANT_FALSE, &paramsSound);

	SAFERELEASE(pWorld);
	SAFERELEASE(pGlobal);
	return hr;
}


void CVWRenderViewCtrl::SetNavigationPath(LPDISPATCH pmapPositions, LPDISPATCH pmapDirections)
{
	IPropertyMap *pmapPos = NULL, *pmapDir = NULL;

	if (FAILED(pmapPositions->QueryInterface(IID_IPropertyMap, (void **)&pmapPos)) ||
		!pmapPos ||
		FAILED(pmapDirections->QueryInterface(IID_IPropertyMap, (void **)&pmapDir)) ||
		!pmapDir)
	{
		SAFERELEASE(pmapPos);
		SAFERELEASE(pmapDir);
		return;
	}

	if (m_pVWNavigationTool)
		m_pVWNavigationTool->SetNavigationPath(pmapPos, pmapDir, NULL);

	SAFERELEASE(pmapPos);
	SAFERELEASE(pmapDir);
}

void CVWRenderViewCtrl::SetCamera(LPDISPATCH pThing)
{
	IThing			*pThis = NULL;
	IVWFrame		*pvwfCamera = NULL;
	COleVariant		var;
	static CComBSTR	bstrGetFrame("GetFrame");

	if (!m_bRendererInitialized)
		goto ERROR_ENCOUNTERED;

	if (FAILED(pThing->QueryInterface(IID_IThing, (void **)&pThis)) || !pThis)
		goto ERROR_ENCOUNTERED;

	// Get the camera.
	if (FAILED(m_pVWRenderRoot->get_Camera(&pvwfCamera)))
		goto ERROR_ENCOUNTERED;

	// Get the thing's loaded geometry.
	if (FAILED(pThis->InvokeMethodExt(bstrGetFrame, NULL, &var)))
		goto ERROR_ENCOUNTERED;
	if (var.vt != VT_DISPATCH || var.pdispVal == NULL)
	{
		// Thing must have geometry to attach camera to.
		goto ERROR_ENCOUNTERED;
	}

	// Attach the camera to the thing's geometry.
	if (FAILED(((IVWFrame*)var.pdispVal)->AddChild(pvwfCamera)))
		goto ERROR_ENCOUNTERED;

	if (GetViewMode() != TOP)
	{
		if (FAILED(pvwfCamera->SetPositionEx(((IVWFrame*)var.pdispVal), 0.0F, 0.0F, 0.0F)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(pvwfCamera->SetOrientationEx(((IVWFrame*)var.pdispVal), 0.0F, 0.0F, 1.0F, 0.0F, 1.0F, 0.0F)))
			goto ERROR_ENCOUNTERED;
	}
	else
	{
		if (FAILED(pvwfCamera->SetPositionEx(((IVWFrame*)var.pdispVal), 0.0F, 500.0F, 0.0F)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(pvwfCamera->SetOrientationEx(NULL, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f)))
			goto ERROR_ENCOUNTERED;
	}

	m_pWorld->FireUIEvent(NULL, CComBSTR(VWSTUDIO_CAMERACHANGED_EVENT_STR), CComVariant( pThing));

ERROR_ENCOUNTERED:

	SAFERELEASE(pvwfCamera);
	SAFERELEASE(pThis);
}

/////////////////////////////////////////////////////////////////////////////
// Drag/drop support  (MM: Commented out for Version 1)
BOOL CVWRenderViewCtrl::HandleDropFile(CPoint pt, LPCTSTR pszFilename)
{
	// REVIEW: for now, we're going to do this here.  Later, it may make
	// sense to delegate this to a tool

	/*VARIANTARG varPt;			// don't use CComVARIANT since it doesn't know VT_POINT_BYREF
	CComVariant varResult;

	varPt.vt = VT_POINT_BYREF;
	varPt.byref = &pt;

	if (SUCCEEDED(HandleHitTest(varPt, 0, &varResult))) 
	{
		ASSERT(varResult.vt == VT_DISPATCH);
		CThingPtr pThing;
		
		pThing = varResult.pdispVal;

		if (pThing != NULL) 
		{	
			// now, invoke OnDropFile(x, y, filename) on it
			CComVariant varFilename(pszFilename);
			CComDISPPARAMS dp(3,  CComVariant(pt.x, VT_I4), CComVariant(pt.y, VT_I4), varFilename);
			CComVariant varResult;

			if (SUCCEEDED(pThing->InvokeMethodExt(CComBSTR("OnDropFile"), dp, &varResult)))
			{
				if (varResult.vt == VT_BOOL)
					return varResult.boolVal==VARIANT_TRUE;
			}
		}
	}
	*/

	return FALSE;

}

/////////////////////////////////////////////////////////////////////////////
// Paint thread

UINT PaintThread(void *pvParam)
{
	CVWRenderViewCtrl *pctrl = (CVWRenderViewCtrl *) pvParam;


	// Must call co initialize 'cause this new thread will cause OLE objects to be
	// created.
	if (FAILED(CoInitialize(NULL)))
	{
		return FALSE;
	}

	// enter the paint thread loop once the graphics has been fully enabled
	// (after the first HandleOnConnect message)
	WaitForSingleObject(pctrl->m_hGraphicsEnabled, INFINITE);

	for(;;)
	{
		if (pctrl->m_bQuit)
			break;

		pctrl->RenderDoProcessing();
	}

	CoUninitialize();

	pctrl->m_bQuit = FALSE;
	return FALSE;
}

BOOL CVWRenderViewCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if (::IsWindow(m_ToolTip.m_hWnd) && pMsg->hwnd == m_hWnd)
	{
		switch(pMsg->message)
		{
		case WM_LBUTTONDOWN:	
		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:	
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:	
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
			m_ToolTip.RelayEvent(pMsg);
			m_CursorToolTip.RelayEvent(pMsg);
			break;
		}
	}
	return CVWUIView::PreTranslateMessage(pMsg);
}



//Works in local coords
HRESULT CVWRenderViewCtrl::PickVertex(CBoundaryDisplayObject* boundaryDisplayObj, int nX, int nY, int nThresh, long nFlags, EIndex *pIndex)
{
	long lRadius = nThresh * nThresh; //flAx, flAy, 
	int nEdges, i;

	*pIndex = NULL_EINDEX;

	if (nFlags & HITTEST_VERTEXSELECT)
	{
		nEdges = boundaryDisplayObj->m_nNumPoints;

		for (i = 0; i < nEdges; i++) 
		{
			int nDX = boundaryDisplayObj->m_pLocalPoints[i].screenPnt.x - nX;
			int nDY = boundaryDisplayObj->m_pLocalPoints[i].screenPnt.y - nY;;

			long lCurDist = nDX * nDX + nDY * nDY;

			if (lCurDist <= lRadius) 
			{
				*pIndex = i;
				goto SUCCESS;  
			}
		}
	}

SUCCESS:
	return S_OK;
}


HRESULT CVWRenderViewCtrl::PickEdge(CBoundaryDisplayObject* boundaryDisplayObj, int nX, int nY, int nThresh, long nFlags, EIndex *pIndex)
{
	float flAx, flAy, flBx, flBy, flA2, dist;
	int nEdges, i;
	HRESULT hr = S_OK;

	*pIndex = NULL_EINDEX;

	nEdges = boundaryDisplayObj->m_nNumPoints;

	for (i = 0; i < nEdges; i++) 
	{
		flAx = (float) boundaryDisplayObj->m_pLocalPoints[i].screenPnt.x;
		flAy = (float) boundaryDisplayObj->m_pLocalPoints[i].screenPnt.y;

		// If last edge, make endpoint the Oth
		if (i == (nEdges - 1) )
		{
			flBx = (float) boundaryDisplayObj->m_pLocalPoints[0].screenPnt.x;
			flBy = (float) boundaryDisplayObj->m_pLocalPoints[0].screenPnt.y;

		}
		else
		{
			flBx = (float) boundaryDisplayObj->m_pLocalPoints[i+1].screenPnt.x;
			flBy = (float) boundaryDisplayObj->m_pLocalPoints[i+1].screenPnt.y;
		}

		// Do simple bounding box test before more expensive length calculation.
		if (   ( ( min(flAx,flBx) - nThresh ) < nX )
			&& ( ( max(flAx,flBx) + nThresh ) > nX )
			&& ( ( min(flAy,flBy) - nThresh ) < nY )
			&& ( ( max(flAy,flBy) + nThresh ) > nY )
			)
		{
			
			flA2 = (nY - flAy) * (flBx - flAx) - (nX - flAx) * (flBy - flAy);
			dist = (float) flA2 / (float)( fabs( flBx - flAx) + fabs(flBy - flAy) - min( fabs(flBx - flAx), fabs( flBy - flAy )) / 2 );

			if ( ((float) nThresh) > fabs(dist) )
			{
				*pIndex = i;

				goto SUCCESS;  
			}
		}
	}

SUCCESS:
	return hr;
}

//Search through all the boundaries of the thing and see if the give point is close to an edge.
HRESULT CVWRenderViewCtrl::DoBoundaryHitTest(CPoint& ptClick, CBoundaryDisplayObject* boundaryDisplayObj, long nFlags, VARIANT * pvarHitResult)
{
	IPropertyList* pBoundaryList = NULL;
	IPropertyList* pPropList = NULL;
	IVector* pVector = NULL;
	HRESULT hr = S_OK;
	int nTolerance;
	IVWFrame *pvwf = NULL;
	IDirect3DRMFrame	*pd3drmframe = NULL;
	HPEN hPen = NULL, hOldPen = NULL;
	VARIANT var;
	EIndex eIndex;
	BOOL bVertexPicked;
	static CComBSTR bstrPickDataProgId = "VWSYSTEM.PickData.1";
	IVWPickData* pPickData = NULL;
	VARIANT_BOOL vbReturnAllPicked, vbPickVertices;
	
	VariantInit(&var);

	vbReturnAllPicked = (nFlags & HITTEST_RETURNALLPICKED ? VARIANT_TRUE : VARIANT_FALSE);
	vbPickVertices = (nFlags & HITTEST_VERTEXSELECT ? VARIANT_TRUE : VARIANT_FALSE);

	static CComBSTR	bstrGetFrame("GetFrame");

	nTolerance = 8;

	eIndex = NULL_EINDEX;
	bVertexPicked = FALSE;
	hr = PickVertex(boundaryDisplayObj, ptClick.x, ptClick.y, nTolerance, nFlags, &eIndex);
	if (FAILED(hr)) goto EXIT_FAIL;

	if (eIndex != NULL_EINDEX)
		bVertexPicked = TRUE;
	else 
	{
		eIndex = NULL_EINDEX;
		hr = PickEdge(boundaryDisplayObj, ptClick.x, ptClick.y, nTolerance, nFlags, &eIndex);
		if (FAILED(hr)) goto EXIT_FAIL;
	}

	if (eIndex != NULL_EINDEX)
	{
		hr = m_pWorld->CreateObjectProperty(bstrPickDataProgId, (IObjectProperty**)&pPickData);
		if (FAILED(hr) || !pPickData) goto EXIT_FAIL;

		hr = pPickData->put_Boundary(boundaryDisplayObj->m_pBoundary);
		if (FAILED(hr)) goto EXIT_FAIL;

		hr = boundaryDisplayObj->m_pThing->get_ObjectProperty(bstrBoundaryList, (IObjectProperty**) &pBoundaryList);
		if (FAILED(hr) || !pBoundaryList) goto EXIT_FAIL;

		hr = pPickData->put_BoundaryList(pBoundaryList);
		if (FAILED(hr)) goto EXIT_FAIL;

		hr = pPickData->put_Thing(boundaryDisplayObj->m_pThing);
		if (FAILED(hr)) goto EXIT_FAIL;

		hr = pPickData->put_BoundaryIndex(boundaryDisplayObj->m_nBoundaryIndex);
		if (FAILED(hr)) goto EXIT_FAIL;

		hr = pPickData->put_EdgeIndex(eIndex);
		if (FAILED(hr)) goto EXIT_FAIL;

		hr = pPickData->put_DataType(ePickedBoundary);
		if (FAILED(hr)) goto EXIT_FAIL;

		//If it was a vertex that was picked
		if (bVertexPicked)
		{
			hr = pPickData->put_VertexPicked(eIndex, VARIANT_TRUE);
			if (FAILED(hr)) goto EXIT_FAIL;
		}

		if (vbReturnAllPicked)  //If true, then return a list of all edges picked.
		{
			if (NULL == pPropList)  //Need to create the list if we haven't already
			{
				hr = CreatePropertyList(m_pWorld, &pPropList);
				if (FAILED(hr)) goto EXIT_FAIL;

				(pvarHitResult)->vt= VT_DISPATCH;
				(pvarHitResult)->pdispVal = pPropList;
				(pvarHitResult)->pdispVal->AddRef();
			}

			hr = pPropList->AddObjectProperty(pPickData);
			if (FAILED(hr)) goto EXIT_FAIL;

		}
		else  //Otherwise return only the picked boundary
		{
			(pvarHitResult)->vt= VT_DISPATCH;
			(pvarHitResult)->pdispVal = pPickData;
			(pvarHitResult)->pdispVal->AddRef();
		}
	}

EXIT_FAIL:
	VariantClear(&var);
	SAFERELEASE(pd3drmframe);
	SAFERELEASE(pBoundaryList);
	SAFERELEASE(pVector);

	return hr;

}

HRESULT CVWRenderViewCtrl::HandleBoundaryHitTest(CPoint& ptClick, long nFlags, VARIANT * pvarHitResult)
{
	IVWRender3D*			pVWRender3D = NULL;
	IVWFrame* pvwfCamera = NULL, *pvwfParent = NULL;
	float fUpX, fUpY, fUpZ;
	HRESULT hr = S_OK;
	VARIANT_BOOL vbReturnAllPicked, vbPickVertices, vbMoving;
	int i, nSize;

	vbReturnAllPicked = (nFlags & HITTEST_RETURNALLPICKED ? VARIANT_TRUE : VARIANT_FALSE);
	vbPickVertices = (nFlags & HITTEST_VERTEXSELECT ? VARIANT_TRUE : VARIANT_FALSE);

	hr = m_pVWRenderRoot->get_Camera(&pvwfCamera);
	if (FAILED(hr) || !pvwfCamera) goto EXIT_FAIL;

	hr = m_pVWRenderRoot->get_Scene(&pvwfParent);
	if (FAILED(hr) || !pvwfParent) goto EXIT_FAIL;

	hr = pvwfCamera->GetPositionEx(pvwfParent, &m_flCameraX, &m_flCameraY, &m_flCameraZ);
	if (FAILED(hr))	goto EXIT_FAIL;

	hr = pvwfCamera->GetOrientationEx(pvwfParent, &fUpX, &fUpY, &fUpZ, &m_flCameraDirX, &m_flCameraDirY, &m_flCameraDirZ );
	if (FAILED(hr))	goto EXIT_FAIL;

	m_flCameraAngle = ComputeAngleFromVector(m_flCameraDirX, m_flCameraDirY, m_flCameraDirZ);

	m_pVWNavigationTool->get_Moving(&vbMoving);

	if (vbMoving)   //Adjust for fast mode when navigating.
	{
		ptClick.x *= 2;
		ptClick.y *= 2;
	}

	nSize = m_boundaryArray.GetSize();

	for (i = nSize - 1; i >= 0; i--)
	{
		CBoundaryDisplayObject* boundaryDisplayObj = m_boundaryArray.GetAt(i);

		DoBoundaryHitTest(ptClick, boundaryDisplayObj, nFlags, pvarHitResult);

		if (pvarHitResult->vt == VT_DISPATCH && pvarHitResult->pdispVal != NULL && vbReturnAllPicked != VARIANT_TRUE)
			break;
	}

EXIT_FAIL:
	SAFERELEASE(pVWRender3D);
	SAFERELEASE(pvwfCamera);
	SAFERELEASE(pvwfParent);

	return S_OK;
}


// Review: This only responds to Point input and returns only one item.
HRESULT CVWRenderViewCtrl::HandleHitTest( VARIANT varHitparams, long nFlags, VARIANT * pvarHitResult )
{
	HRESULT hr = S_OK;
	CPoint cptHit;
	IDispatch * pdisp = NULL;
	IVWFrame *pvwFrame = NULL;
	IThing *pthing = NULL;
	IUnknown *punk = NULL;
	IObjectProperty *pobjprop = NULL;
	COleVariant varParams;
	COleVariant varHit;
	DWORD	dw;
	int i;
	long nCount, nFound;
	CPropertyListPtr cPropList;
	CPropertyListPtr cThingPropList;
	CVWFramePtr cVWFrame;
	VARIANT_BOOL vbReturnAllPicked, vbPickVertices;

	//Test for good arguments and intialization
	if (!m_bRendererInitialized)
	{
		hr = E_FAIL;
		pvarHitResult->vt = VT_EMPTY;
		goto EXIT_FAIL;
	}
	
	if( varHitparams.vt != VT_POINT_BYREF) 
	{
		hr = E_INVALIDARG;
		pvarHitResult->vt = VT_EMPTY;
		goto EXIT_FAIL;
	};

	//Do initialization
	VariantInit(pvarHitResult);   //MM: was clear, why?
	cptHit = *(POINT*)varHitparams.byref;
	vbReturnAllPicked = (nFlags & HITTEST_RETURNALLPICKED ? VARIANT_TRUE : VARIANT_FALSE);
	vbPickVertices = (nFlags & HITTEST_VERTEXSELECT ? VARIANT_TRUE : VARIANT_FALSE);

	//If in boundary edit mode, do a boundary pick test.
	if (m_nEditingMode == EDIT_BOUNDARIES)
	{
		HandleBoundaryHitTest(cptHit, nFlags, pvarHitResult);
		if (pvarHitResult->vt == VT_DISPATCH)
			goto EXIT_FAIL;
	}

	//Do geometry pick test 
	dw=MAKELONG(cptHit.x, cptHit.y);
	varParams = (long)dw;
	hr = m_pVWRenderRoot->HitTest(varParams, vbReturnAllPicked, (LPVARIANT)varHit);
	if (FAILED(hr))
	{
		pvarHitResult->vt = VT_EMPTY;
		goto EXIT_FAIL;
	}

	// if nothing, then return success (but empty)
	if (varHit.vt == VT_EMPTY)
	{
		pvarHitResult->vt = VT_EMPTY;
		goto EXIT_FAIL;
	}

	// assure it's at least an IDispatch
	if (varHit.vt != VT_DISPATCH) 
	{
		pvarHitResult->vt = VT_EMPTY;
		hr = E_FAIL;
		goto EXIT_FAIL;
	}

	//And now if RenderRoot sent us a proplist of frames, 
	//then build a list of things.
	cPropList = varHit.punkVal;

	if (cPropList != NULL)
	{	//If hit result is a PropertyList, build a list of things only.

		hr = CreatePropertyList(m_pWorld, &cThingPropList.p);
		if (FAILED(hr)) goto EXIT_FAIL;

		hr = cPropList->get_Count(&nCount);
		if (FAILED(hr)) goto EXIT_FAIL;

		for (i = 0; i < nCount; i++)
		{	//For each selected frame

			hr = cPropList->get_ObjectProperty(i, &pobjprop);
			if (FAILED(hr)) goto EXIT_FAIL;

			cVWFrame = pobjprop;
			SAFERELEASE(pobjprop);

			if (cVWFrame != NULL)
			{
				hr = cVWFrame->get_Thing(&pthing);
				if (FAILED(hr)) goto EXIT_FAIL;

				nFound = -1; //initialization
				hr = cThingPropList->Find(CComVariant(pthing), &nFound);
				if (FAILED(hr)) goto EXIT_FAIL;

				if (nFound == -1)
				{
					hr = cThingPropList->AddObjectProperty(pthing);
					if (FAILED(hr)) goto EXIT_FAIL;
				}
			}

			cVWFrame.Release();
			SAFERELEASE(pthing);
		}

		pvarHitResult->vt = VT_DISPATCH;
		pvarHitResult->pdispVal = cThingPropList.p;
		pvarHitResult->pdispVal->AddRef();
	}
	else
	{	//If it's a frame we're returning, substitute it's thing.
		punk = varHit.punkVal;
	
		// assure that it's a IVWFrame
		hr = punk->QueryInterface(IID_IVWFrame, (void **) &pvwFrame);
		if (FAILED(hr))
		{
			pvarHitResult->vt = VT_EMPTY;
			goto EXIT_FAIL;
		}
		
		//Return the IVWFrame's Thing
		pvwFrame->get_Thing((IThing**)&pdisp);
		if (pdisp==0)
		{
			pvarHitResult->vt = VT_EMPTY;
			hr = E_FAIL;
			goto EXIT_FAIL;
		}

		pvarHitResult->vt = VT_DISPATCH;
		pvarHitResult->pdispVal = pdisp;
	}

EXIT_FAIL:

	SAFERELEASE(pvwFrame);
	SAFERELEASE(pobjprop);
	SAFERELEASE(pthing);

	return hr;
}

BOOL CVWRenderViewCtrl::OnToolTipNeedText(UINT id, NMHDR * pNMHDR, LRESULT *pResult)
{
	BOOL bHandledNotify = FALSE;
	CPoint CursorPos;
	VERIFY(::GetCursorPos(&CursorPos));
	ScreenToClient(&CursorPos);
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	static CComBSTR	bstrName("Name");
	static CComBSTR	bstrLButtonDown("OnLButtonDown");
	static CComBSTR	bstrLButtonUp("OnLButtonUp");
	BSTR		bstrObjName = NULL;
	COleVariant varResult;
	VARIANTARG varPt;
	varPt.vt = VT_POINT_BYREF;
	varPt.byref = &CursorPos;
	CThingPtr pThing;
	CRect ClientRect;
	GetClientRect(ClientRect);
	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;
	VARIANT_BOOL bMethodExits = FALSE;

	if (pTTT->hdr.idFrom == CURSOR_TOOLTIP && m_bNavigationToolIsCurrentlySelected  && !m_CursorToolTipShown && IsUIToolCurrent(PICKTOOLNAME))
	{
		if (ClientRect.PtInRect(CursorPos) && !bHandled) 
		{
			if (m_bRendererInitialized) 
			{
				if (SUCCEEDED(HandleHitTest(varPt, 0, &varResult)) && varResult.vt == VT_DISPATCH) 
				{
					pThing = varResult.pdispVal;
					if (pThing != NULL) 
					{	
						if (FAILED(hr = pThing->IsValidMethod(bstrLButtonDown, &bMethodExits)))
							goto ERROR_ENCOUNTERED;

						if (bMethodExits == VARIANT_FALSE)
						{
							if (FAILED(hr = pThing->IsValidMethod(bstrLButtonUp, &bMethodExits)))
								goto ERROR_ENCOUNTERED;
						}
						
						if (bMethodExits == VARIANT_FALSE)
						{
							if (FAILED(hr = pThing->IsValidMethod(bstrLButtonUp, &bMethodExits)))
								goto ERROR_ENCOUNTERED;
						}

						if (bMethodExits == VARIANT_TRUE)
						{
							if (FAILED(hr = m_pVWNavigationTool->SetCursor( m_pVWUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_LINK) ) ) ))
								goto ERROR_ENCOUNTERED;
							if (FAILED(hr = m_pVWPickTool->SetCursor( m_pVWUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_LINK) ) ) ))
								goto ERROR_ENCOUNTERED;

							m_CursorToolTip.Activate(FALSE);
							m_CursorToolTipShown = TRUE;
							m_ToolTipPos = CursorPos;
						}

					}
				}
			}
		}
	}
	else if (pTTT->hdr.idFrom == MAIN_TOOLTIP)
	{
		if (ClientRect.PtInRect(CursorPos) && !bHandled) 
		{
			if (m_bRendererInitialized) 
			{
				if (SUCCEEDED(HandleHitTest(varPt, 0, &varResult)) && varResult.vt == VT_DISPATCH) 
				{
					pThing = varResult.pdispVal;
					if (pThing != NULL) 
					{	
						if (FAILED(hr = pThing->get_String(bstrName, &bstrObjName)))
							goto ERROR_ENCOUNTERED;

						CString strTip(bstrObjName);
						::strncpy(pTTT->szText, strTip, 79);
						pTTT->szText[79] = 0;
						bHandled = TRUE;
						m_ToolTipShown = TRUE;
						m_ToolTipPos = CursorPos;
					}
				}
			}
		}
	}

	if (!bHandled)
		pTTT->szText[0] = 0;


ERROR_ENCOUNTERED:

	SAFEFREESTRING(bstrObjName);
	return TRUE;
}

BOOL	IsAlphaKey(UINT nChar)
{
	switch (nChar)
	{
	case 13:	// return
	case 8:		// backspace
		return TRUE;
		break;
	default:
		if (iscntrl(nChar))
			return FALSE;
		else
			return TRUE;
	}
}

void CVWRenderViewCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (!m_nEditingMode)
	{
		if (IsAlphaKey(nChar))
		{
			FireAlphaKeyPressed(nChar);
			return;
		}
	}
	CVWUIView::OnChar(nChar, nRepCnt, nFlags);
}

HRESULT CVWRenderViewCtrl::HandleOnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient)
{
	static CComBSTR bstrBackgroundColorEvent(VW_BACKGROUNDCOLORCHANGED_EVENT_STR);
	static CComBSTR bstrBackgroundColor("BackgroundColor");
	static CComBSTR bstrEnterRoom(VW_ENTERROOM_EVENT_STR);
	static CComBSTR bstrLeaveRoom(VW_LEAVEROOM_EVENT_STR);
	static CComBSTR bstrGeometryChanged(VW_GEOMETRYCHANGED_EVENT_STR);
	static CComBSTR bstrVideoModeChanged(VWSTUDIO_VIEWMODECHANGED_EVENT_STR);
	static CComBSTR bstrVisibilityChanged(VWSTUDIO_VISIBILITYCHANGED_EVENT_STR);
	static CComBSTR bstrDeviceChanged(VWSTUDIO_DEVICECHANGED_EVENT_STR);
	static CComBSTR bstrEditingModeChanged(VWSTUDIO_EDITINGMODECHANGED_EVENT_STR);
	static CComBSTR bstrIsMoveableChanged(VWSTUDIO_ISMOVEABLECHANGED_EVENT_STR);
	static CComBSTR bstrAddAnimation("AddAnimation");
	static CComBSTR bstrDeleteAnimation("DeleteAnimation");
	static CComBSTR bstrStartAnimation("StartAnimation");
	static CComBSTR bstrStopAnimation("StopAnimation");
	static CComBSTR bstrSetTimeOfAnimation("SetTimeOfAnimation");
	static CString	strPosition("OnPositionDone");
	static CString	strDirection("OnDirectionDone");
	static CString	strScale("OnScaleDone");
	static CString	strUp("OnUpDone");

	CComVariant	varRGBBackground;
	HRESULT hr = S_OK;
	IThing *pContainer = NULL, *pRoot = NULL;
	IBoundary* pBoundary = NULL;
	BOOL bFound = FALSE;
	
	CVWUIView::HandleOnUIEvent(pthing, bstrEventName, varArg, bFromClient);

	if (CompareElements(&bstrBackgroundColorEvent.m_str, &bstrEventName))
	{
		pContainer = pthing;
		SAFEADDREF(pthing);
		while (SUCCEEDED(pContainer->get_Container(&pRoot)) && pRoot != NULL)
		{
			SAFERELEASE(pContainer);
			pContainer = pRoot;
			pRoot = NULL;
		}

		if (pContainer == pthing)
		{
			pContainer->get_Property(bstrBackgroundColor, &varRGBBackground);
			if (FAILED(varRGBBackground.ChangeType(VT_I4)))
			{
				varRGBBackground.Clear();
				varRGBBackground.vt = VT_I4;
				varRGBBackground.lVal = 0;
			}
				// set the scene background color
			BYTE r = (BYTE) (varRGBBackground.lVal >> 16) & 0xFF;
			BYTE g = (BYTE) (varRGBBackground.lVal >> 8) & 0xFF;
			BYTE b = (BYTE) varRGBBackground.lVal & 0xFF;

			if (FAILED(hr = m_pVWRenderRoot->SetBackgroundColor(r, g, b)))
				goto EXIT_FAIL;
		}				
	}
	else if (CompareElements(&bstrEnterRoom.m_str, &bstrEventName))
	{
		ASSERT(varArg.vt == VT_DISPATCH);
		CThingPtr pWhat(varArg.pdispVal);

		hr = HandleOnEnterRoom(pthing, pWhat);
	}
	
	else if (CompareElements(&bstrEditingModeChanged.m_str, &bstrEventName))
	{
		SetEditMode(varArg.lVal);
	}
	else if (CompareElements(&bstrLeaveRoom.m_str, &bstrEventName))
	{
		ASSERT(varArg.vt == VT_DISPATCH);
		CThingPtr pWhat(varArg.pdispVal);

		hr = HandleOnLeaveRoom(pthing, pWhat);

		if (m_nEditingMode == EDIT_BOUNDARIES)
			RebuildBoundaryArray();
	}
	else if (m_nEditingMode && CompareElements(&bstrGeometryChanged.m_str, &bstrEventName))
	{
		hr = HandleOnGeometryChanged(pthing);
	}
	else if (m_nEditingMode == EDIT_BOUNDARIES && CompareElements(&bstrBoundaryUpdate.m_str, &bstrEventName))
	{
		if (varArg.vt == VT_I4)
		{
			int nBoundaryIndex = ((unsigned long)varArg.lVal) >> 8;
			int nHint = ((unsigned long)varArg.lVal) & 0xFF;

			if (nHint == BOUNDARYUPDATE_REMOVE)
				RemoveBoundaryByIndex(pthing, nBoundaryIndex);
			else if (nHint == BOUNDARYUPDATE_UPDATEINFO)
				UpdateBoundaryByIndex(pthing, nBoundaryIndex);
			else if (nHint == BOUNDARYUPDATE_ADD)
				AddBoundaryByIndex(pthing, nBoundaryIndex);
			else if (nHint == BOUNDARYUPDATE_UPDATESCREENINFO)
				m_bNeedUpdateBoundaryScreenCoordinates = TRUE;
			else
				OutputDebugString("RenderView: Bad boundary update hint in UIEvent.\n");
		}
		else
		{
			ASSERT(0);
		}

	}
	else if (m_nEditingMode == EDIT_BOUNDARIES && CompareElements(&bstrVideoModeChanged.m_str, &bstrEventName))
	{
		m_bNeedUpdateBoundaryScreenCoordinates = TRUE;
	}
	else if (m_nEditingMode && (CompareElements(&bstrVisibilityChanged.m_str, &bstrEventName) || CompareElements(&bstrIsMoveableChanged.m_str, &bstrEventName) ))
	{
		HandleOnGeometryChanged(pthing) ;
	}
	else if (m_nEditingMode && CompareElements(&bstrDeviceChanged.m_str, &bstrEventName))
	{
		SetViewMode(m_CameraMode);
	}
	else if (CompareElements(&bstrStartAnimation.m_str, &bstrEventName))
	{
		//Get the animation from the animation list and start it
		if (varArg.vt == VT_BSTR) 
		{
			WaitForSingleObject(m_hInterpMutex,INFINITE);
			POSITION pos = m_animList.GetHeadPosition();
			D3DAnimation* d3dAnim;
			CComBSTR bstrTmp(varArg.bstrVal);

			while (pos!=NULL) 
			{
				d3dAnim = &(m_animList.GetNext(pos));
				if (bstrTmp == d3dAnim->Name) 
				{
					d3dAnim->StartTime = GetTickCount();  //Reset to first position
					bFound = TRUE;
				}
			}
			ReleaseMutex(m_hInterpMutex);

			if (!bFound)
				TRACE("Warning: Failed to find animation to start.\n");
		}
	}
	else if (CompareElements(&bstrStopAnimation.m_str, &bstrEventName))
	{
		//Get the animation from the animation list and stop it
		if (varArg.vt == VT_BSTR) 
		{
			WaitForSingleObject(m_hInterpMutex,INFINITE);
			POSITION pos = m_animList.GetHeadPosition();
			D3DAnimation* d3dAnim;
			CComBSTR bstrTmp(varArg.bstrVal);

			while (pos!=NULL) 
			{
				d3dAnim = &(m_animList.GetNext(pos));
				if (bstrTmp == d3dAnim->Name) 
				{
					d3dAnim->StartTime = GetTickCount();  //Reset to first position
					bFound = TRUE;
				}
			}
			ReleaseMutex(m_hInterpMutex);

			if (!bFound)
				TRACE("Warning: Failed to find animation to start.\n");
		}
	}
	else if (CompareElements(&bstrDeleteAnimation.m_str, &bstrEventName))
	{
		//Delete the animation from the animation list
		if (varArg.vt == VT_BSTR) 
		{
			WaitForSingleObject(m_hInterpMutex,INFINITE);
			POSITION pos = m_animList.GetHeadPosition();
			POSITION pos_tmp;
			D3DAnimation* d3dAnim;
			CString strAnim(varArg.bstrVal);

			while (pos!=NULL) 
			{
				d3dAnim = &(m_animList.GetNext(pos));
				CString strCurrent(d3dAnim->Name);

				if (strCurrent.CompareNoCase(strAnim)==0)
				{
					((IUnknown*)(d3dAnim->Animation))->Release();
					//if the animation specifies a method to call when it is killed programatically, call it
					if ((d3dAnim->bstrAnimDoneMethod.Length()) > 0)	
					{
						CString szTmp = d3dAnim->bstrAnimDoneMethod.m_str;
						//pass the programatic flag and the index time at which the animation stopped.
						//but only if it is not a basic onXXXDone method call
						if (	((szTmp.CompareNoCase(strPosition)==0)|| (szTmp.CompareNoCase(strDirection)==0)) ||
								((szTmp.CompareNoCase(strScale)==0)	|| (szTmp.CompareNoCase(strUp)==0))			)
						{
							(d3dAnim->pThis)->InvokeMethodExt(CComBSTR(d3dAnim->bstrAnimDoneMethod.m_str), NULL, NULL);
						}
						else  
						{
//							(d3dAnim->pThis)->InvokeMethodExt(CComBSTR(d3dAnim->bstrAnimDoneMethod.m_str), NULL, NULL);
							(d3dAnim->pThis)->InvokeMethodExt(CComBSTR(d3dAnim->bstrAnimDoneMethod.m_str), CComDISPPARAMS(2, CComVariant((bool)VARIANT_FALSE), CComVariant(CurrentTime(d3dAnim->pThis))), NULL);
						} 
					}
					pos_tmp = pos;

					if (pos==NULL) 
						pos = m_animList.GetTailPosition();
					else 
						m_animList.GetPrev(pos);

					m_animList.RemoveAt(pos);
					pos = pos_tmp;
				}				
			}
			ReleaseMutex(m_hInterpMutex);
		}
	}
	else if (CompareElements(&bstrSetTimeOfAnimation.m_str, &bstrEventName))
	{
		if (varArg.vt == VT_UNKNOWN) 
		{
			POSITION pos = m_animList.GetHeadPosition();
			POSITION pos_tmp;
			D3DAnimation* d3dAnim;
			CComPtr<IPropertyList>	pPropList;
			CComVariant		var;
			BSTR			bstrStr;
			float			fIndex;

// SK
			if (FAILED(hr = varArg.punkVal->QueryInterface(IID_IPropertyList,(void**)&pPropList)))
			{
				goto EXIT_FAIL;
			}

			WaitForSingleObject(m_hInterpMutex,INFINITE);

			pPropList->get_String(0,&bstrStr);
			var.Clear();
			pPropList->get_Property(1, &var);
			fIndex = var.fltVal;
			var.Clear();
			CString strAnim(bstrStr);
 
			while (pos!=NULL) 
			{
				d3dAnim = &(m_animList.GetNext(pos));
				CString strCurrent(d3dAnim->Name);
				if (strCurrent.CompareNoCase(strAnim)==0)
				{	
					d3dAnim->StartTime = (unsigned long)GetTickCount();
					d3dAnim->fIndex = fIndex;
					
					pos_tmp = pos;
					if (pos==NULL) 
						pos = m_animList.GetTailPosition();
					else 
						m_animList.GetPrev(pos);
					pos = pos_tmp;
				}	
			}
			ReleaseMutex(m_hInterpMutex);
		}
	}
	else if (CompareElements(&bstrAddAnimation.m_str, &bstrEventName))
	{
		//Create an animation based on the information in the property list passed
		//up and add the animation to the renderer's list.
		if (varArg.vt == VT_UNKNOWN) 
		{
			CComPtr<IPropertyList>	pPropList;
			CComPtr<IThing>		pThing;
			CComPtr<IDirect3DRMAnimationSet2> pAnimationSet;
			CComBSTR			bstrTmp;
			long				dwTmp;
			VARIANT				varTmp;
			D3DAnimation		d3dAnim;
			static CComBSTR		bstrPosDur("PositionDuration");
			static CComBSTR		bstrDirDur("DirectionDuration");
			static CComBSTR		bstrUpDur("UpDuration");
			static CComBSTR		bstrScaleDur("ScaleDuration");
	
			if (FAILED(hr = varArg.punkVal->QueryInterface(IID_IPropertyList,(void**)&pPropList)))
				goto EXIT_FAIL;

			pPropList->get_Long(0,&dwTmp);
			((IUnknown*)dwTmp)->AddRef();
			d3dAnim.Animation = (void*)dwTmp;

			pPropList->get_Long(1,&dwTmp);
			d3dAnim.StartTime = dwTmp;

			pPropList->get_Long(2,&dwTmp);
			d3dAnim.Options = dwTmp;
			
			::VariantInit(&varTmp);
			pPropList->get_Property(3, &varTmp);
			d3dAnim.fDuration = varTmp.fltVal;
			::VariantClear(&varTmp);

			pPropList->get_String(4,&bstrTmp.m_str);
			d3dAnim.Name = bstrTmp;
			bstrTmp.Empty();

			WaitForSingleObject(m_hInterpMutex,INFINITE);

			POSITION pos = m_animList.GetHeadPosition();
			POSITION pos_tmp;
			D3DAnimation* d3dAnim2 = NULL;
			CComBSTR	bstrMethod("");
			
			while (pos!=NULL) 
			{
				d3dAnim2 = &(m_animList.GetNext(pos));
				if (d3dAnim.Name == d3dAnim2->Name) 
				{
					if ((d3dAnim2->bstrAnimDoneMethod.Length()) > 0)
					{
						bstrMethod = d3dAnim2->bstrAnimDoneMethod;
						d3dAnim.pThis = d3dAnim2->pThis;
						// pThis is now a CComPtr.
						// SAFEADDREF(d3dAnim.pThis);
					}
					((IUnknown*)(d3dAnim2->Animation))->Release();
					pos_tmp = pos;
					if (pos==NULL) 
						pos = m_animList.GetTailPosition();
					else 
						m_animList.GetPrev(pos);
					m_animList.RemoveAt(pos);
					pos = pos_tmp;
				}				
			}
			ReleaseMutex(m_hInterpMutex);

			if ((bstrMethod.Length()) > 0)	
			{	
				//pass the programatic flag and the index time at which the animation stopped.
				//but only do this for custom animDone methods, not OnXXXDone method calls.
				CString szTmp = bstrMethod.m_str;
				if (	((szTmp.CompareNoCase(strPosition)==0)|| (szTmp.CompareNoCase(strDirection)==0)) ||
						((szTmp.CompareNoCase(strScale)==0)	|| (szTmp.CompareNoCase(strUp)==0))			)
				{
					(d3dAnim.pThis)->InvokeMethodExt(CComBSTR(bstrMethod.m_str), NULL, NULL);
				}
				else
				{	
					(d3dAnim.pThis)->InvokeMethodExt(CComBSTR(bstrMethod.m_str), CComDISPPARAMS(2, CComVariant((bool)VARIANT_FALSE), CComVariant(CurrentTime(d3dAnim.pThis))), NULL);
				}  
				// pThis is now a CComPtr.
				// SAFERELEASE(d3dAnim.pThis);
			}
	
			pPropList->get_Thing(5, &pThing);
			d3dAnim.pThis = pThing;

			pPropList->get_Property(6, &varTmp);
			d3dAnim.fTimeScale = varTmp.fltVal;
			::VariantClear(&varTmp);


			pPropList->get_Property(7, &varTmp);
			d3dAnim.fIndex = varTmp.fltVal;
			::VariantClear(&varTmp);

			pPropList->get_String(8, &bstrTmp.m_str);
			d3dAnim.bstrAnimDoneMethod = bstrTmp;
			bstrTmp.Empty();

			pPropList->get_Property(9, &varTmp);
			d3dAnim.fRangeStart = varTmp.fltVal;
			::VariantClear(&varTmp);

			pPropList->get_Property(10, &varTmp);
			d3dAnim.fRangeEnd = varTmp.fltVal;
			::VariantClear(&varTmp);
			//work around for DX6 bug - index can't be on last key of interpolation
			if (SUCCEEDED(((IUnknown*)d3dAnim.Animation)->QueryInterface(IID_IDirect3DRMInterpolator, &d3dAnim.Animation)))
			{
				((IDirect3DRMInterpolator*)(d3dAnim.Animation))->SetIndex((float)(d3dAnim.fDuration + 1.0f));
				// Extra AddRef from doing QI, need to release it.
				((IUnknown*)d3dAnim.Animation)->Release();
				//d3dAnim.fTimeScale = 1.0f;
			}
			else if (SUCCEEDED(((IUnknown*)d3dAnim.Animation)->QueryInterface(IID_IDirect3DRMAnimation2, &d3dAnim.Animation)))
			{
				((IDirect3DRMAnimation2*)(d3dAnim.Animation))->SetOptions(d3dAnim.Options);
				// Extra AddRef from doing QI, need to release it.
				((IUnknown*)d3dAnim.Animation)->Release();
				//d3dAnim.fTimeScale = 1.0f;
			}
			else if (SUCCEEDED(((IUnknown*)d3dAnim.Animation)->QueryInterface(IID_IDirect3DRMAnimationSet2, (LPVOID*) &d3dAnim.Animation)))
			{
				//if (d3dAnim.fDuration == -1 || d3dAnim.fRangeStart < -0.0f || d3dAnim.fRangeEnd < -0.0f)
				//	d3dAnim.fTimeScale = 1.0f;
				//else
				//	d3dAnim.fTimeScale = (d3dAnim.fRangeEnd - d3dAnim.fRangeStart) / d3dAnim.fDuration;

				((IDirect3DRMAnimationSet2*)(d3dAnim.Animation))->SetTime(d3dAnim.fRangeStart);
				// Extra AddRef from doing QI, need to release it.
				((IUnknown*)d3dAnim.Animation)->Release();
			}

			WaitForSingleObject(m_hInterpMutex, INFINITE);

			//Add the animation node to our list
			m_animList.AddHead(d3dAnim);

			ReleaseMutex(m_hInterpMutex);
		}
	}

EXIT_FAIL:
	SAFERELEASE(pBoundary);
	SAFERELEASE(pContainer);
	SAFERELEASE(pRoot);
	return hr;
}


// SK - added these helpers to do time calc the same as the server for OnAnimationDone methods
void CVWRenderViewCtrl::ULongAdd(unsigned long lNum1, unsigned long lNum2, unsigned long * lNum3) 
{
	const unsigned long ulHalfMax = (unsigned long)ULONG_MAX/2;
		//2147483647l;
	if (lNum2 >= ulHalfMax)
	{
		if (lNum1 >= ulHalfMax)
			*lNum3 = (lNum1-ulHalfMax)+(lNum2-ulHalfMax);
		else
		{
			if (lNum1 >= (ULONG_MAX - lNum2))
				*lNum3 = lNum1 - (ULONG_MAX - lNum2);
			else
				*lNum3 = lNum1 + lNum2;
		}
	}
	else 
	{
		if (lNum1 >= ulHalfMax)
		{
				if (lNum2 >= (ULONG_MAX - lNum1))
					*lNum3 = lNum2 - (ULONG_MAX - lNum1);
				else
					*lNum3 = lNum2 + lNum1;
		}
		else
			*lNum3 = lNum2 + lNum1;
	}
	return;
}

void CVWRenderViewCtrl::ULongSubtract(unsigned long lNum1, unsigned long lNum2, unsigned long * lNum3) 
{
	HRESULT hr = S_OK;

	if (lNum2 > lNum1)
		ULongAdd(lNum1, (ULONG_MAX - lNum2), lNum3);
	else
		*lNum3 = lNum1 - lNum2;

	return;
}

long CVWRenderViewCtrl::CurrentTime(IThing* pThing)
{
		if (pThing)
		{
			long lSystemTime = 0;
			long lSystemStartTicks = 0;
			unsigned long	ulSystemTicks = 0;
			unsigned long	ulSystemTickTime = 0;
			CComPtr <IWorld> pWorld;
			CComPtr <IThing> pGlobal;
			pThing->get_World(&pWorld);
			if (!pWorld)
				return 0;
			pWorld->get_Global(&pGlobal);
			if (!pGlobal)
				return 0;
			pGlobal->get_Long(CComBSTR("SystemTime"), &lSystemTime);
			pGlobal->get_Long(CComBSTR("SystemStartTicks"), &lSystemStartTicks);
			ULongSubtract((unsigned long)GetTickCount(),(unsigned long)lSystemStartTicks, &ulSystemTicks);
			ULongAdd((unsigned long)lSystemTime , ulSystemTicks, &ulSystemTickTime);	
			return (long)ulSystemTickTime;
		}
		else
			return 0;
}

						
