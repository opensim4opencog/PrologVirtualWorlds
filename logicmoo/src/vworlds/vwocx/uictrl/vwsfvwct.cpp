// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWSfVwCt.cpp : Implementation of the CVWSelfVwCtrl OLE control class.

#include "stdafx.h"
#include "uictrl.h"
#include "VWSfVwCt.h"
#include <propbase.h>
#include <vwadata.h>
#include <vwgcache.h>
#include <vwgeomsp.h>
#include <menuitem.h>
#include <vwevents.h>

//#define IID_DEFINED
//#include <vwgeomsp_i.c>
//#include <spritemn_i.c>
//#include <menuitem_i.c>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TimerPeriod 100

HRESULT CreateClipper(IDirectDraw *pDD, HWND hWnd, IDirectDrawClipper **ppClipper);
HRESULT CreateFrontBuffer(IDirectDraw *pDD, IDirect2DRM *pD2DRM, IDirectDrawSurface **ppFrontBuffer);
HRESULT CreateGraphicsTools(IWorld *pWorld, IDirectDraw **ppDD, IDirect2DRM **ppD2DRM, IDirect3DRM **ppD3DRM, IVWGeometryCache **ppVWGCache);

typedef CVWComPtr<IThing, NULL, &IID_IThing> CThingPtr;

IMPLEMENT_DYNCREATE(CVWSelfVwCtrl, CVWViewCtrl)

/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CVWSelfVwCtrl, CVWViewCtrl)
	//{{AFX_MSG_MAP(CVWSelfVwCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CONTEXTMENU()
	ON_WM_TIMER()
	ON_COMMAND_RANGE(WM_USER, (WM_USER+200), OnCommandRange)
	ON_UPDATE_COMMAND_UI_RANGE(WM_USER, (WM_USER+200), OnCommandRangeUI)
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CVWSelfVwCtrl, CVWViewCtrl)
	//{{AFX_DISPATCH_MAP(CVWSelfVwCtrl)
	DISP_PROPERTY_EX(CVWViewCtrl, "Client", CVWViewCtrl::GetVWClient, CVWViewCtrl::SetVWClient, VT_DISPATCH)
	DISP_PROPERTY_EX(CVWSelfVwCtrl, "GeometryName", GetGeometryName, SetGeometryName, VT_BSTR)
	DISP_PROPERTY_EX(CVWSelfVwCtrl, "CurrentGesture", GetCurrentGesture, SetCurrentGesture, VT_I4)
	DISP_PROPERTY_EX(CVWSelfVwCtrl, "HandleWorldEvents", GetHandleWorldEvents, SetHandleWorldEvents, VT_BOOL)
	DISP_FUNCTION(CVWSelfVwCtrl, "AddAccessory", AddAccessory, VT_EMPTY, VTS_BSTR VTS_I4 VTS_R4 VTS_R4)
	DISP_FUNCTION(CVWSelfVwCtrl, "RemoveAccessory", RemoveAccessory, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CVWSelfVwCtrl, "SetBackgroundColor", SetBackgroundColor, VT_EMPTY, VTS_I2 VTS_I2 VTS_I2)
	DISP_FUNCTION(CVWSelfVwCtrl, "NamedGesture", GetNamedGesture, VT_I4, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CVWSelfVwCtrl, CVWViewCtrl)
	//{{AFX_EVENT_MAP(CVWSelfVwCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CVWSelfVwCtrl, 1)
//	PROPPAGEID(CVWSelfVwPropPage::guid)
END_PROPPAGEIDS(CVWSelfVwCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CVWSelfVwCtrl, "VWCLIENTUI.SelfViewCtrl.1",
	0x63fb7203, 0x909e, 0x11d0, 0x89, 0xaa, 0, 0xa0, 0xc9, 0x5, 0x41, 0x29)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CVWSelfVwCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DVWSelfVw =
		{ 0x63fb7201, 0x909e, 0x11d0, { 0x89, 0xaa, 0, 0xa0, 0xc9, 0x5, 0x41, 0x29 } };
const IID BASED_CODE IID_DVWSelfVwEvents =
		{ 0x63fb7202, 0x909e, 0x11d0, { 0x89, 0xaa, 0, 0xa0, 0xc9, 0x5, 0x41, 0x29 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwVWSelfVwOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CVWSelfVwCtrl, IDS_VWSELFVW, _dwVWSelfVwOleMisc)

/////////////////////////////////////////////////////////////////////////////
// CVWSelfVwCtrl::CVWSelfVwCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CVWSelfVwCtrl

BOOL CVWSelfVwCtrl::CVWSelfVwCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_VWSELFVW,
			IDB_VWSELFVW,
			afxRegInsertable | afxRegApartmentThreading,
			_dwVWSelfVwOleMisc,
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
// CVWSelfVwCtrl::CVWSelfVwCtrl - Constructor

static HRESULT
D2DTestFailureCallback(HRESULT hr, const char *szErrMsg, va_list pMsgArgs)
{
	switch(hr) {
	case DDERR_INVALIDRECT:
//		Sleep(100);
//		MMASSERT(0);
		break;
	case STG_E_FILENOTFOUND:
		ErrMgr::ComposeGenericString(hr, g_rgchTmpBuf, nTMPBUFSIZE, szErrMsg, pMsgArgs);
		AfxMessageBox(g_rgchTmpBuf);
		break;
	default:
		ErrMgr::ComposeGenericString(hr, g_rgchTmpBuf, nTMPBUFSIZE, szErrMsg, pMsgArgs);
		AfxMessageBox(g_rgchTmpBuf);
		MMASSERT(SUCCEEDED(hr));
		break;
	}
	return hr;
}

CVWSelfVwCtrl::CVWSelfVwCtrl()
: m_em(D2DTestFailureCallback)
{
	InitializeIIDs(&IID_DVWSelfVw, &IID_DVWSelfVwEvents);

	// COM objects are actually created with the WM_CREATE message -
	// this way, a window has actually been generated
	m_pDD = NULL;
	m_pFrontBuffer = NULL;
	m_pPalette = NULL;
	INIT_DXSTRUCT(m_ddpfTarget);
	m_pClipper = NULL;
	m_pD2DRM = NULL;
	m_pDevice = NULL;
	m_pViewport = NULL;
	m_pCamera = NULL;
	m_pAccessoryScene = NULL;
	m_bTrackingMouse = VARIANT_FALSE;
	m_pPick = NULL;
	m_ptAnchor.x = m_ptAnchor.y = 0;
	m_pINetFileManager = NULL;
	m_pSpriteManager = NULL;
	
	m_nZDepth = 0;

	// self view stuff
	m_pBackBuffer = NULL;
	m_pd2diBackground = NULL;
	m_nSVWidth = m_nSVHeight = 0;
	m_pd2diSelfView = NULL;
	m_fXWinScale = m_fYWinScale = 0.f;
	m_dwTimerID = 0;
	m_crBackgroundColor = RGB(255,255,255);
	m_bHandleWorldEvents = TRUE;

	// Need a unique cookie for the self view image.
	char buff[10];
	m_bstrSelfViewName = CComBSTR(ltoa(DWORD(this), buff, 10));
}


/////////////////////////////////////////////////////////////////////////////
// CVWSelfVwCtrl::~CVWSelfVwCtrl - Destructor

CVWSelfVwCtrl::~CVWSelfVwCtrl()
{
	// everything is removed when the window is destroyed
	
	// for some reason, we don't get an OnClose event
//	CVWSelfVwCtrl::OnClose();
}

/////////////////////////////////////////////////////////////////////////////
// CVWSelfVwCtrl::OnDraw - Drawing function

void 
CVWSelfVwCtrl::OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	DoPaint(rcInvalid);
}

/////////////////////////////////////////////////////////////////////////////
// CVWSelfVwCtrl::DoPropExchange - Persistence support

void CVWSelfVwCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	CVWViewCtrl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CVWSelfVwCtrl::OnResetState - Reset control to default state

void CVWSelfVwCtrl::OnResetState()
{
	CVWViewCtrl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CVWSelfVwCtrl message handlers

int CVWSelfVwCtrl::OnCreate(LPCREATESTRUCT lpcs) 
{
	if (CVWViewCtrl::OnCreate(lpcs) == -1)
		return -1;
	return 0;
}
   
void CVWSelfVwCtrl::OnSize(UINT nType, int cx, int cy) 
{
	if ((cx > 0) && (cy > 0)) {
		m_fXWinScale = float(m_nSVWidth) / float(cx);
		m_fYWinScale = float(m_nSVHeight) / float(cy);
	} else {
		m_fXWinScale = m_fYWinScale = 0.f;
	}
	CVWViewCtrl::OnSize(nType, cx, cy);
}

void CVWSelfVwCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	IDirect2DRMArray	*pPicks = NULL;
	IDirect2DRMFrame	*pFrame = NULL;
	IThing				*pThing = NULL;
	IDirect2DRMPick		*pPick = NULL;
	DWORD				dwSize;
	CRect				rect;
	long				lThingID;
	VARIANT_BOOL		bWorn;
	long				nXPick, nYPick;

	if (!m_bShowAccessories || !m_pAccessoryScene || !m_pViewport || !m_pWorld)
		goto ERROR_ENCOUNTERED;

	// compute the adjusted pick point
	nXPick = LONG(float(point.x) * m_fXWinScale);
	nYPick = LONG(float(point.y) * m_fYWinScale);

	// Pick the accessory.
	if (FAILED(m_pViewport->Pick(m_pAccessoryScene, nXPick, nYPick, &pPicks)) ||
		!pPicks)
		goto ERROR_ENCOUNTERED;

	pPicks->get_Size(&dwSize);
	if (dwSize == 0)
		goto ERROR_ENCOUNTERED;

	if (FAILED(pPicks->GetPick(0, &pPick)))
		goto ERROR_ENCOUNTERED;

	// Don't do anything if the accessory isn't REALLY worn yet.
	if (FAILED(pPick->get_ControlFrame(&pFrame)))
		goto ERROR_ENCOUNTERED;
	pFrame->get_AppData((unsigned long*)&lThingID);
	if (FAILED(m_pWorld->get_ObjectByID(lThingID, &pThing)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(pThing->get_BOOL(CComBSTR("isWorn"), &bWorn)) || !bWorn)
		goto ERROR_ENCOUNTERED;

	m_pPick = pPick;
	SAFEADDREF(pPick);

	// Make sure picked object is at the front.
	m_pPick->MoveToFront();

	// Force a repaint of the self-view.
	UpdateSelfView();

	// Start moving accessory mode
	m_bTrackingMouse = VARIANT_TRUE;
	m_ptAnchor = point;
	
	// Ensure all future mouse input gets sent to this window.
	SetCapture();

ERROR_ENCOUNTERED:

	SAFERELEASE(pPicks);
	SAFERELEASE(pFrame);
	SAFERELEASE(pThing);
	SAFERELEASE(pPick);
}

void CVWSelfVwCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	CRect				rect;
	IDirect2DRMFrame	*pFrame = NULL;
	IThing				*pUser = NULL;
	IPropertyMap		*pWornAccessories = NULL;
	IVWAccessoryData	*pVWAccessoryData = NULL;
	IDirect2DRMPick		*pPick = NULL;
	CComVariant			pvarClone;
	float				fX, fY;
	long				lThingID;
	static CComBSTR		bstrWornAccessories("WornAccessories");
	char				buff[10];

	if (m_bTrackingMouse)
	{
		// End moving accessory mode.
		m_bTrackingMouse = VARIANT_FALSE;

		// Force a repaint of the self-view.
		UpdateSelfView();

		// Stop all future mouse input getting sent to this window.
		ReleaseCapture();

		// Release picked object.
		pPick = m_pPick;
		m_pPick = NULL;

		// Get final position of picked object.
		if (FAILED(pPick->get_ControlFrame(&pFrame)))
			goto ERROR_ENCOUNTERED;
		pFrame->get_XOffset(&fX);
		pFrame->get_YOffset(&fY);

		// Get in world object ID for accessory.
		pFrame->get_AppData((unsigned long*)&lThingID);

		// Get user.
		if (!m_pWorld || FAILED(m_pWorld->get_User(&pUser)))
			goto ERROR_ENCOUNTERED;
		
		// Update in world property for the accessory.
		if (FAILED(pUser->get_ObjectProperty(bstrWornAccessories, (IObjectProperty**)&pWornAccessories)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(pWornAccessories->get_ObjectProperty(CComBSTR(ltoa(lThingID, buff, 10)), (IObjectProperty**)&pVWAccessoryData)))
			goto ERROR_ENCOUNTERED;
		pVWAccessoryData->PutPos(fX, fY);
		if (FAILED(m_pWorld->CloneProperty(CComVariant(pVWAccessoryData), &pvarClone)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(pWornAccessories->put_Property(CComBSTR(ltoa(lThingID, buff, 10)), pvarClone)))
			goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:

	SAFERELEASE(pPick);
	SAFERELEASE(pFrame);
	SAFERELEASE(pUser);
	SAFERELEASE(pWornAccessories);
	SAFERELEASE(pVWAccessoryData);
}


void 
CVWSelfVwCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bTrackingMouse)
	{
		CRect rect;

		// Calculate displacement delta.
		float dX = float(point.x - m_ptAnchor.x) * m_fXWinScale;
		float dY = float(point.y - m_ptAnchor.y) * m_fYWinScale;
//		CSize sizeDisp(point - m_ptAnchor);
		m_ptAnchor = point;

		// Actually move the picked object.
		m_pViewport->MovePicked(dX, dY, m_pPick);

		// Force a repaint of the self-view.
		UpdateSelfView();
	}
}

void CVWSelfVwCtrl::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	IDirect2DRMArray	*pPicks = NULL;
	IDirect2DRMPick		*pPick = NULL;
	IThing				*pThing = NULL;
	IDirect2DRMFrame	*pFrame = NULL;
	DWORD				dwSize;
	CRect				rect;
	CPoint				pt = point;
	long				lThingID;
	CPropertyListPtr	plistMenu;
	CComVariant			varResult;
	UINT				wParam;
	HRESULT				hr = S_OK;

	// point is in screen coords!
	pWnd->ScreenToClient( &pt );

	if (!m_bShowAccessories || !m_pAccessoryScene || !m_pViewport || !m_pWorld)
		goto ERROR_ENCOUNTERED;

	// Pick the accessory.
	if (FAILED(m_pViewport->Pick(m_pAccessoryScene, pt.x, pt.y, &pPicks)) ||
		!pPicks)
		goto ERROR_ENCOUNTERED;

	pPicks->get_Size(&dwSize);
	if (dwSize == 0)
		goto ERROR_ENCOUNTERED;

	if (FAILED(pPicks->GetPick(0, &pPick)))
		goto ERROR_ENCOUNTERED;

	// Make sure picked object is at the front.
	pPick->MoveToFront();

	// Force a repaint of the self-view.
	UpdateSelfView();

	// Get the in-world thing associated with this accessory.
	if (FAILED(pPick->get_ControlFrame(&pFrame)))
		goto ERROR_ENCOUNTERED;
	pFrame->get_AppData((unsigned long*)&lThingID);
	if (FAILED(m_pWorld->get_ObjectByID(lThingID, &pThing)))
		goto ERROR_ENCOUNTERED;
	
	// get the menu
	if (FAILED(hr = pThing->InvokeMethodExt(CComBSTR("GetMenuList"), NULL, &varResult)))
		goto ERROR_ENCOUNTERED;

	// display menu with the property list
	plistMenu = varResult;
	varResult.Clear();
	if ( plistMenu != NULL )
	{
		// build the menu
		wParam = WM_USER;
		hr = m_Menu.CreatePopupFromPropList(plistMenu, &wParam);
		if (SUCCEEDED(hr))
		{
			VERIFY(m_Menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this));
			// Peek WM_COMMAND from users menu selection off of queue and dispatch it. This
			// way we get to handle the users command right here (via WM_COMMAND handler) 
			// and can destroy menu after.
			MSG msg;
			if (::PeekMessage(&msg, m_hWnd, WM_COMMAND, WM_COMMAND, PM_REMOVE))
			{
				// yep, process right now
				::DispatchMessage(&msg);
			}
			m_Menu.DestroyMenu();
		}
	}

ERROR_ENCOUNTERED:

	SAFERELEASE(pPicks);
	SAFERELEASE(pPick);
	SAFERELEASE(pThing);
	SAFERELEASE(pFrame);
}

void CVWSelfVwCtrl::OnCommandRange(UINT nID)
{
	HRESULT hr;
	m_Menu.OnCommandRange(nID, &hr);
}

void CVWSelfVwCtrl::OnCommandRangeUI(CCmdUI* pCmdUI)
{
	HRESULT hr;
	m_Menu.OnCommandRangeUI( pCmdUI, &hr );
}

HRESULT
CVWSelfVwCtrl::UpdateSelfView()
{
	HRESULT hr = S_OK;

	if (m_pViewport)
	{
		// Want transparent color of image on m_pd2diSelfView surface since 
		// this is the color key set when m_pd2diSelfView surface is created via 
		// CreateFittedSurface and will be used in the m_pd2diSelfView->BltToSurface call.
		m_pViewport->Clear(flagBLT_IGNORETRANSPARENCY);

		if (m_bShowAccessories && m_pAccessoryScene)
			m_pViewport->Render(m_pAccessoryScene, 0);
	}
	if (m_pBackBuffer)
	{
		// First blt background color (this will be in screen pixel format,
		// unlike the viewport background color which is in the image's pixel format).
		HDC hDC;
		if (SUCCEEDED(m_pBackBuffer->GetDC(&hDC)))
		{
			DDSURFACEDESC ddsd;
			memset(&ddsd,0,sizeof(DDSURFACEDESC));
			ddsd.dwSize = sizeof(ddsd);
			m_pBackBuffer->GetSurfaceDesc(&ddsd);
			CRect rect(0, 0, ddsd.dwWidth, ddsd.dwHeight);
			CDC::FromHandle(hDC)->FillSolidRect(&rect, m_crBackgroundColor);
			m_pBackBuffer->ReleaseDC(hDC);
		}
		if (m_pd2diSelfView)
			m_pd2diSelfView->BltToSurface(NULL, m_pBackBuffer, NULL, flagBLT_USEDYNAMICMAPPING);
	}

	RECT r;
	GetClientRect(&r);
	DoPaint(r);

	return hr;
}

void
CVWSelfVwCtrl::DoPaint(const RECT &rInvalid)
{
	CDC		*pDC;
	HRESULT hr;

	if (!m_pBackBuffer || !m_pFrontBuffer)
		goto ERROR_ENCOUNTERED;
	
	// make sure the blt is non-zero sized
	if ((rInvalid.left != rInvalid.right) && (rInvalid.top != rInvalid.bottom))
	{
		if (!m_pClipper)
		{
			if (FAILED(hr = CreateClipper(m_pDD, m_hWnd, &m_pClipper)))
				goto ERROR_ENCOUNTERED;
		}
		
		//
		//	must ensure our clipper is used
		//
		m_em = m_pFrontBuffer->SetClipper(m_pClipper);

		// Ignore rInvalid
		RECT rTo;
		GetClientRect(&rTo);

		// Stretch blt the backbuffer to the frontbuffer
		ClientToScreen(&rTo);
		RECT rFrom;
		SetRect(&rFrom, 0, 0, m_nSVWidth, m_nSVHeight);
		m_pFrontBuffer->Blt(&rTo, m_pBackBuffer, &rFrom, DDBLT_WAIT, NULL);
	}

	return;

ERROR_ENCOUNTERED:

	if (::IsWindow(m_hWnd))
	{
		pDC = GetDC();
		pDC->FillSolidRect(&rInvalid, m_crBackgroundColor);
		ReleaseDC(pDC);
	}
}

HRESULT
CVWSelfVwCtrl::ReleaseAll()
{
//	MMTRACE("CVWSelfVwCtrl::ReleaseAll\n");
	MMRELEASE(m_pd2diSelfView);
	MMRELEASE(m_pd2diBackground);
	MMRELEASE(m_pViewport);
	MMRELEASE(m_pDevice);
	MMRELEASE(m_pCamera);
	MMRELEASE(m_pAccessoryScene);
	MMRELEASE(m_pPick);
	MMRELEASE(m_pBackBuffer);
	MMRELEASE(m_pClipper);
	MMRELEASE(m_pFrontBuffer);
	MMRELEASE(m_pPalette);
	MMRELEASE(m_pD2DRM);
	MMRELEASE(m_pDD);
	MMRELEASE(m_pINetFileManager);
	MMRELEASE(m_pSpriteManager);

	return S_OK;
}

HRESULT
CreatePlainSurfaceSV(LPDIRECTDRAW pDD, DWORD nWidth, DWORD nHeight, DWORD nBPP,
				     const DDPIXELFORMAT *pddpf, LPDIRECTDRAWPALETTE pPalette, 
				     LPDIRECTDRAWSURFACE *ppdds)
{
	MMASSERT(pDD && ppdds);

	HRESULT hr;
	DDSURFACEDESC ddsd;
	INIT_DXSTRUCT(ddsd);
    ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS | DDSD_PIXELFORMAT;
	ddsd.dwWidth = nWidth;
	ddsd.dwHeight = nHeight;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	if (pddpf) {
		// nBPP is ignored if a PixelFormat exists
		ddsd.ddpfPixelFormat = *pddpf;
	} else {
		if (nBPP <= 8) {
			// palettized
			if (pPalette == NULL)
				return E_INVALIDARG;
			DWORD dwFlags = DDPF_RGB;
			switch (nBPP) {
			case 8:	dwFlags |= DDPF_PALETTEINDEXED8;
			case 4:	dwFlags |= DDPF_PALETTEINDEXED4;
			case 2:	dwFlags |= DDPF_PALETTEINDEXED2;
			case 1:	dwFlags |= DDPF_PALETTEINDEXED1;
			}
			ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
			ddsd.ddpfPixelFormat.dwFlags = dwFlags;
			ddsd.ddpfPixelFormat.dwRGBBitCount = nBPP;
		} else {
			return E_INVALIDARG;
		}
	}

	LPDIRECTDRAWSURFACE pdds;
	if (FAILED(hr = pDD->CreateSurface(&ddsd, &pdds, NULL)))
		return hr;

	// attach palette if it exists
	if (pPalette)
		if (FAILED(hr = pdds->SetPalette(pPalette))) {
			pdds->Release();
			return hr;
		}

	*ppdds = pdds;

	return hr;
}

// blue is assumed to have a weight of 1.f
#define fSimpleRedWeight 2.1f
#define fSimpleGreenWeight 2.4f
#define fMaxColorDistance ((1.f + fSimpleRedWeight + fSimpleGreenWeight) * float(257 * 256))

static inline float
_ColorDistance(const PALETTEENTRY &pe, BYTE r, BYTE g, BYTE b)
{
	float fTotal, fTmpR, fTmpG, fTmpB;
	fTmpR = (float) (pe.peRed - r);
	fTotal = fSimpleRedWeight * fTmpR * fTmpR;
	fTmpG = (float) (pe.peGreen - g);
	fTotal += fSimpleGreenWeight * fTmpG * fTmpG;
	fTmpB = (float) (pe.peBlue - b);
	// blue is assumed to have a weight of 1.f
	fTotal += fTmpB * fTmpB;

	return fTotal;
}

DWORD
SimpleFindClosestIndex(const PALETTEENTRY *rgpePalette, DWORD cEntries, BYTE r, BYTE g, BYTE b)
{
	float fTmp, fMinDistance = fMaxColorDistance;
	DWORD nMinIndex = cEntries;

	for (DWORD i = 0; i < cEntries; i++) {
		const PALETTEENTRY &peTmp = rgpePalette[i];
		if (!(peTmp.peFlags & (PC_RESERVED | PC_EXPLICIT))) {
			if ((fTmp = _ColorDistance(peTmp, r, g, b)) < fMinDistance) {
				// check for exact match
				if (fTmp == 0.f)
					return i;
				nMinIndex = i;
				fMinDistance = fTmp;
			}
		}
	}
	return nMinIndex;
}

HRESULT
CVWSelfVwCtrl::SetSelfViewBackground(BSTR bstrNewImageFilename)
{
	HRESULT hr = S_OK;
	LPDIRECT2DRMIMAGE pd2diNewBackground = NULL;
	LPDIRECTDRAWSURFACE pddsSelfView = NULL;
	CComPtr<IDirect2DRM> pD2DRM;
	CComPtr<IDirectDraw> pDD;
	CComPtr<IDirectDrawSurface> pFrontBuffer;

	if (!m_pD2DRM)
		goto e_SetSelfViewBackground;
	
	// release the old self view stuff
	MMRELEASE(m_pd2diSelfView);
	MMRELEASE(m_pd2diBackground);

	if (FAILED(hr = m_pD2DRM->QueryInterface(IID_IDirect2DRM, (void**) &pD2DRM)))
		goto e_SetSelfViewBackground;

	if (FAILED(hr = m_pDD->QueryInterface(IID_IDirectDraw, (void**) &pDD)))
		goto e_SetSelfViewBackground;

	if (FAILED(hr = pD2DRM->CreateImageFromImageFile(bstrNewImageFilename, 
						flagCREATE_IGNOREANIMATION, &pd2diNewBackground)))
	{
		m_pWorld->Report(CComBSTR(CString("VWSelfView: Failed to create image from file: ") + CString(bstrNewImageFilename)), -1);
		goto e_SetSelfViewBackground;
	}

	DWORD nNewWidth, nNewHeight;
	pd2diNewBackground->get_Width(&nNewWidth);
	pd2diNewBackground->get_Height(&nNewHeight);

	// create a new backbuffer, if necessary
	if ((nNewWidth != m_nSVWidth) || (nNewHeight != m_nSVHeight) || !m_pBackBuffer) {
		// release the old backbuffer
		MMRELEASE(m_pBackBuffer);

		RECT r;
		GetClientRect(&r);
		if ((r.right > 0) && (r.bottom > 0)) {
			m_fXWinScale = float(nNewWidth) / float(r.right);
			m_fYWinScale = float(nNewHeight) / float(r.bottom);
			
			if (!m_pFrontBuffer)
			{
				if (FAILED(hr = CreateFrontBuffer(pDD, pD2DRM, &m_pFrontBuffer)))
					goto e_SetSelfViewBackground;
				if (FAILED(hr = m_pFrontBuffer->QueryInterface(IID_IDirectDrawSurface, (void**) &pFrontBuffer)))
					goto e_SetSelfViewBackground;
				m_pFrontBuffer->GetPixelFormat(&m_ddpfTarget);
				if (m_ddpfTarget.dwRGBBitCount <= 8)
					m_pFrontBuffer->GetPalette(&m_pPalette);
			}

			// create the new backbuffer
			if (FAILED(hr = CreatePlainSurfaceSV(pDD, nNewWidth, nNewHeight, 
								m_ddpfTarget.dwRGBBitCount, &m_ddpfTarget, 
								m_pPalette, &m_pBackBuffer)))
				goto e_SetSelfViewBackground;
		} else {
			m_fXWinScale = m_fYWinScale = 0.f;
			goto e_SetSelfViewBackground;
		}
	}

	// create a surface fitted to the new background image to serve as the render target
	if (FAILED(hr = pd2diNewBackground->CreateFittedSurface(&pddsSelfView)))
		goto e_SetSelfViewBackground;

	// set the device and viewport to be dependent on this surface
	if (m_pDevice == NULL) {
		// if the device doesn't currently exist, create one from scratch
		if (FAILED(hr = pD2DRM->CreateDeviceFromSurface(NULL, pddsSelfView, &m_pDevice)) ||
			FAILED(hr = pD2DRM->CreateViewport(m_pDevice, m_pCamera, NULL, &m_pViewport)))
			goto e_SetSelfViewBackground;
	} else {
		// just initialize the device with the new surface
		MMASSERT(m_pViewport);
		if (FAILED(hr = m_pDevice->InitFromSurface(pddsSelfView)))
			goto e_SetSelfViewBackground;
	}

	// set the background image of the viewport to the new image
	if (FAILED(hr = m_pViewport->SetBackgroundImage(pd2diNewBackground)) ||
		// create an image from the render target surface
		FAILED(hr = pD2DRM->CreateImageFromSurface(pddsSelfView, m_bstrSelfViewName, &m_pd2diSelfView)) ||
		FAILED(hr = m_pd2diSelfView->put_Animated(VARIANT_FALSE)))
		goto e_SetSelfViewBackground;
	MMRELEASE(pddsSelfView);

	// Note: we do not need to AddRef this
	m_pd2diBackground = pd2diNewBackground;

	m_nSVWidth = nNewWidth;
	m_nSVHeight = nNewHeight;

	return S_OK;

e_SetSelfViewBackground:

	MMRELEASE(pddsSelfView);
	MMRELEASE(pd2diNewBackground);
	m_nSVWidth = m_nSVHeight = 0;
	return hr;
}

HRESULT CVWSelfVwCtrl::HandleOnUserConnect(IThing* pthing, BOOL bFromClient)
{
	IThing				*pUser = NULL, *pGlobal = NULL;
	IWorld				*pWorld = NULL;
	IVWGeometry			*pVWGeometry = NULL;
	IVWGeometrySprite	*pVWGeometrySprite = NULL;
	IPropertyMap		*pWornAccessories = NULL;
	IPropertyMap		*pWornAccessoriesMap = NULL;
	IDirect2DRM			*pD2D = NULL;
	IDirect2DRMFrame	*pCamera = NULL;
	static CComBSTR		bstrWornAccessories("WornAccessories");
	static CComBSTR		bstrGeometry("Geometry");
	VARIANT_BOOL		bIsValid, bLastItem;
	CComBSTR			bstrAccessoryGraphic, bstrID;
	COleVariant			var;
	float				fPosX, fPosY;
	long				lWornAccessories = 0;
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

	// Create the graphics tools (if not already created) and put them into the world 
	// if they haven't been already by another OCX, otherwise just get them from the world.
	if (FAILED(hr = CreateGraphicsTools(m_pWorld, &m_pDD, &m_pD2DRM, NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pGlobal->get_String(CComBSTR("DefaultSpriteFile"), &m_bstrDefaultSpriteFile.m_str)))
		goto ERROR_ENCOUNTERED;

	ASSERT(m_dwTimerID == 0);
	if ((m_dwTimerID = SetTimer(NULL, TimerPeriod, NULL)) == 0)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	// Add explitict QI's for now..

	if (FAILED(hr = m_pD2DRM->QueryInterface(IID_IDirect2DRM, (void**)&pD2D)))
		goto ERROR_ENCOUNTERED;

	hr = pD2D->CreateFrame(NULL, &m_pCamera);

	if (!m_bHandleWorldEvents)
		goto ERROR_ENCOUNTERED;

	if (FAILED(pUser->get_ObjectProperty(bstrGeometry, (IObjectProperty**)&pVWGeometry)) || !pVWGeometry)
		goto ERROR_ENCOUNTERED;
	if (FAILED(pVWGeometry->QueryInterface(IID_IVWGeometrySprite, (void**)&pVWGeometrySprite)))
		goto ERROR_ENCOUNTERED;

	pVWGeometrySprite->get_GeometryName(&m_bstrGeometryName.m_str);
	
	GeometryNameChanged();
	
	// Add accessories to the sprite (if any).
	// Get the WornAccessories property list and cycle through it adding the accessories
	// to this sprite.
	if (FAILED(hr = pthing->IsValidProperty(bstrWornAccessories, &bIsValid)))
		goto ERROR_ENCOUNTERED;
	if (bIsValid)
	{
		if (FAILED(hr = pthing->get_ObjectProperty(bstrWornAccessories, (IObjectProperty**)&pWornAccessories)))
			goto ERROR_ENCOUNTERED;

		if (FAILED(hr = pWornAccessories->QueryInterface(IID_IPropertyMap, (void**) &pWornAccessoriesMap)))
			goto ERROR_ENCOUNTERED;

		if (FAILED(hr = pWornAccessoriesMap->get_Count(&lWornAccessories)))
			goto ERROR_ENCOUNTERED;
		if (lWornAccessories > 0)
		{
			if (FAILED(hr = pWornAccessoriesMap->FirstItem(&bstrID.m_str, &var, &bLastItem)))
				goto ERROR_ENCOUNTERED;
			do
			{
				((IVWAccessoryData*)var.pdispVal)->get_AccessoryGraphic(&bstrAccessoryGraphic.m_str);
				((IVWAccessoryData*)var.pdispVal)->GetPos(&fPosX, &fPosY);
				AddAccessory(bstrAccessoryGraphic, atol(CString(bstrID)), fPosX, fPosY);
				if (!bLastItem)
				{
					if (FAILED(hr = pWornAccessoriesMap->NextItem(&bstrID.m_str, &var, &bLastItem)))
						goto ERROR_ENCOUNTERED;
				}
			} 
			while (bLastItem == VARIANT_FALSE);
		}
	}

ERROR_ENCOUNTERED:	

	UpdateSelfView();
	SAFERELEASE(pWorld);
	SAFERELEASE(pGlobal);
	SAFERELEASE(pUser);
	SAFERELEASE(pVWGeometry);
	SAFERELEASE(pVWGeometrySprite);
	SAFERELEASE(pD2D);
	SAFERELEASE(pWornAccessories);
	SAFERELEASE(pWornAccessoriesMap);

	return hr;
}

HRESULT CVWSelfVwCtrl::HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient)
{
	IThing	*pUser = NULL;
	IWorld	*pWorld = NULL;
	HRESULT	hr = S_OK;
	
	CVWViewCtrl::HandleOnUserDisconnect(pthing, bFromClient);

	if (FAILED(hr = pthing->get_World(&pWorld)))
		goto ERROR_ENCOUNTERED;

	// If world has gone away then we should clean up as if this is the user disconnect.
	// If we're in a ShowModlessDialog we will be in a different thread so we can receive
	// OnUserDisconnect AFTER world has gone away.
	if (pWorld && FAILED(hr = pWorld->get_User(&pUser)))
		goto ERROR_ENCOUNTERED;

	if (pWorld && (pthing != pUser))
		goto ERROR_ENCOUNTERED;

	if (m_dwTimerID)
	{
		KillTimer(m_dwTimerID);
		m_dwTimerID = 0;
	}
	ReleaseAll();

ERROR_ENCOUNTERED:
	UpdateSelfView();	// dv - force a redraw here

	SAFERELEASE(pWorld);
	SAFERELEASE(pUser);

	return hr;
}

HRESULT CVWSelfVwCtrl::HandleOnUserReconnect(IVWClient* newClient, IWorld* pWorld, IThing* pthing)
{
 	return HandleOnUserConnect(pthing, VARIANT_TRUE);
}

HRESULT CVWSelfVwCtrl::HandleOnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient)
{
	HRESULT hr = S_OK;
	static CComBSTR bstrWear(VW_WEAR_EVENT_STR);
	static CComBSTR bstrUnWear(VW_UNWEAR_EVENT_STR);
	static CComBSTR bstrGesture(VW_DOGESTURE_EVENT_STR);
	static CComBSTR bstrGeometryChanged(VW_GEOMETRYCHANGED_EVENT_STR);

	if (!m_bHandleWorldEvents)
		return hr;
	
	CVWViewCtrl::HandleOnUIEvent(pthing, bstrEventName, varArg, bFromClient);

	if (CompareElements(&bstrWear.m_str, &bstrEventName))
	{
		ASSERT(varArg.vt == VT_DISPATCH);
		CThingPtr pWhat(varArg.pdispVal);

		hr = HandleOnWear(pthing, pWhat);
	}
	else if (CompareElements(&bstrUnWear.m_str, &bstrEventName))
	{
		ASSERT(varArg.vt == VT_DISPATCH);
		CThingPtr pWhat(varArg.pdispVal);

		hr = HandleOnUnWear(pthing, pWhat);
	}
	else if (CompareElements(&bstrGesture.m_str, &bstrEventName))
	{
		ASSERT(varArg.vt == VT_I4);
		hr = HandleOnDoGesture(varArg.iVal);
	}
	else if (CompareElements(&bstrGeometryChanged.m_str, &bstrEventName))
	{
		hr = HandleOnGeometryChanged(pthing);
	}

	return hr;
}

HRESULT CVWSelfVwCtrl::HandleOnGeometryChanged(IThing* pthing)
{
	IThing				*pUser = NULL;
	IVWGeometry			*pVWGeometry = NULL;
	IVWGeometrySprite	*pVWGeometrySprite = NULL;
	static CComBSTR		bstrGeometry("Geometry");
	CComBSTR			bstrGeometryName;
	HRESULT	hr = S_OK;
	
	if (!m_bHandleWorldEvents || !m_pWorld)
		return hr;
	
	if (FAILED(hr = m_pWorld->get_User(&pUser)))
		goto ERROR_ENCOUNTERED;

	if (pthing != pUser)
		goto ERROR_ENCOUNTERED;

	if (FAILED(pUser->get_ObjectProperty(bstrGeometry, (IObjectProperty**)&pVWGeometry)) || !pVWGeometry)
		goto ERROR_ENCOUNTERED;
	if (FAILED(pVWGeometry->QueryInterface(IID_IVWGeometrySprite, (void**)&pVWGeometrySprite)))
		goto ERROR_ENCOUNTERED;

	pVWGeometrySprite->get_GeometryName(&bstrGeometryName.m_str);

	SetGeometryName((CString)bstrGeometryName);

ERROR_ENCOUNTERED:

	SAFERELEASE(pUser);
	SAFERELEASE(pVWGeometry);
	SAFERELEASE(pVWGeometrySprite);

	return hr;
}

HRESULT CVWSelfVwCtrl::HandleOnWear(IThing* pWho, IThing* pWhat)
{
	CComBSTR			bstrGraphic;
	long				lID;
	HRESULT				hr = S_OK;

	if (!m_bHandleWorldEvents)
		return hr;
	
	if (FAILED(hr = pWhat->get_String(CComBSTR("accessoryGraphic"), &bstrGraphic.m_str)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pWhat->get_ID(&lID)))
		goto ERROR_ENCOUNTERED;
	AddAccessory(bstrGraphic, lID, 0, 0);

ERROR_ENCOUNTERED:

	return hr;
}

HRESULT CVWSelfVwCtrl::HandleOnUnWear(IThing* pWho, IThing* pWhat)
{
	long				lID;
	HRESULT				hr = S_OK;

	if (!m_bHandleWorldEvents)
		return hr;
	
	if (FAILED(hr = pWhat->get_ID(&lID)))
		goto ERROR_ENCOUNTERED;
	RemoveAccessory(lID);

ERROR_ENCOUNTERED:

	return hr;
}

HRESULT CVWSelfVwCtrl::HandleOnDoGesture(int iGesture)
{
	if (!m_bHandleWorldEvents)
		return S_OK;
	
	SetCurrentGesture(iGesture);
	return S_OK;
}

void CVWSelfVwCtrl::SetGeometryName(LPCTSTR lpszNewValue)
{
	m_bstrGeometryName = lpszNewValue;
	GeometryNameChanged();
	UpdateSelfView();
}

BSTR CVWSelfVwCtrl::GetGeometryName()
{
	return m_bstrGeometryName.Copy();
}

void CVWSelfVwCtrl::SetCurrentGesture(int iGesture)
{
	if (m_pSpriteManager)
		m_pSpriteManager->put_CurrentGesture(iGesture);
}

int CVWSelfVwCtrl::GetCurrentGesture()
{
	if (m_pSpriteManager)
	{
		int iGesture;
		m_pSpriteManager->get_CurrentGesture(&iGesture);
		return iGesture;
	}
	return 0;
}

int CVWSelfVwCtrl::GetNamedGesture(LPCTSTR szGesture)
{
	if (m_pSpriteManager)
	{
		int iGestureNum;
		m_pSpriteManager->get_NamedGesture(CComBSTR(szGesture), &iGestureNum);
		return iGestureNum;
	}
	return 0;
}

void CVWSelfVwCtrl::AddAccessory(BSTR bstrImageFileName, long lID, float fPosX, float fPosY)
{
	IDirect2DRMFrame	*pFrame = NULL;
	IDirect2DRMImage	*pImage = NULL;
	BSTR				bstrLocalName = NULL;
	CComBSTR	bstrFullURL;
	VARIANT_BOOL		bFound;
	HRESULT hr = S_OK;

	if (!m_pD2DRM || !m_pWorld)
	{
		// Need to have created the tools to do this!
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	if (!m_pAccessoryScene)
	{
		if (FAILED(hr = m_pD2DRM->CreateFrame(NULL, &m_pAccessoryScene)))
			goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = m_pD2DRM->CreateFrame(m_pAccessoryScene, &pFrame)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pFrame->SetTranslation(fPosX, fPosY)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pINetFileManager->FindCachedFile(bstrImageFileName, &bstrFullURL.m_str, &bstrLocalName, &bFound)) || !bFound)
	{
		if (FAILED(hr = m_pINetFileManager->GetFileEx(bstrImageFileName, VARIANT_FALSE, &bstrFullURL.m_str, &bstrLocalName)))
		{
			m_pWorld->Report(CComBSTR(CString("VWSelfView: Failed to download file: ") + CString(bstrImageFileName)), -1);
			goto ERROR_ENCOUNTERED;
		}
	}
	if (FAILED(hr = m_pD2DRM->CreateImageFromImageFile(bstrLocalName, 
						flagCREATE_IGNOREANIMATION, &pImage)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pFrame->AddVisual(pImage)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pFrame->put_AppData(lID)))
		goto ERROR_ENCOUNTERED;
		
	UpdateSelfView();

ERROR_ENCOUNTERED:

	SAFEFREESTRING(bstrLocalName);
	SAFERELEASE(pFrame);
	SAFERELEASE(pImage);
}

void CVWSelfVwCtrl::RemoveAccessory(long lID)
{
	IDirect2DRMFrame	*pFrame = NULL;
	IDirect2DRMArray	*pChildren = NULL;
	DWORD				dwChildren;
	HRESULT hr = S_OK;

	if (!m_pAccessoryScene)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	if (FAILED(hr = AccessoryFrameFromID(lID, &pFrame)))
		goto ERROR_ENCOUNTERED;
	
	if (FAILED(hr = m_pAccessoryScene->DeleteChild(pFrame)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = m_pAccessoryScene->get_Children(&pChildren)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pChildren->get_Size(&dwChildren)))
		goto ERROR_ENCOUNTERED;
	if (dwChildren == 0)
		SAFERELEASE(m_pAccessoryScene);

	UpdateSelfView();

ERROR_ENCOUNTERED:

	SAFERELEASE(pFrame);
	SAFERELEASE(pChildren);
}

void CVWSelfVwCtrl::SetBackgroundColor(short r, short g, short b)
{
	m_crBackgroundColor = RGB((BYTE)r,(BYTE)g,(BYTE)b);
	UpdateSelfView();
}

HRESULT CVWSelfVwCtrl::GeometryNameChanged()
{
	CComBSTR			bstrGeometryName;
	CComBSTR			bstrGeometryURL, bstrGeometryPath, bstrError,
						bstrImageFile, bstrImageFileURL, bstrImageFilePath,
						bstrDefaultFileFullURL, bstrDefaultFileFinalPath, bstrReturnedURL;
	CComPtr<IInternetFileManager> pINetFileMgr;
	VARIANT_BOOL		bFound;
	CRect				rect;
	static CComBSTR		bstrGeometry("Geometry");
	static CComBSTR		bstrDirect2DRM("Direct2DRM");
	CString				strImageFileName;
	char				szExt[_MAX_EXT];
	int					iFwdSlash, iBackSlash;
	HRESULT	hr = S_OK;

	// Want no background image until this operation is complete.
	if (m_pViewport)
		m_pViewport->SetBackgroundImage(NULL);

	if (!m_pWorld)
		goto ERROR_ENCOUNTERED;

	if (!m_pINetFileManager)
	{
		if (FAILED(hr = m_pWorld->get_Tool(CComBSTR("Inetfile"),(IUnknown**) &m_pINetFileManager)))
			goto ERROR_ENCOUNTERED;
	}

	// Explicitly QI so interface is marshalled
	if (FAILED(m_pINetFileManager->QueryInterface(IID_IInternetFileManager, (void**) &pINetFileMgr)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pINetFileMgr->FindCachedFile(m_bstrGeometryName, &bstrGeometryURL.m_str, &bstrGeometryPath.m_str, &bFound)) || !bFound)
	{
		if (FAILED(hr = pINetFileMgr->GetFileEx(m_bstrGeometryName, VARIANT_FALSE, &bstrGeometryURL.m_str, &bstrGeometryPath.m_str)))
		{
			m_pWorld->Report(CComBSTR(CString("VWSelfView: Failed to download file: ") + CString(m_bstrGeometryName)), -1);
			goto ERROR_ENCOUNTERED;
		}
	}

	if (!m_pSpriteManager)
	{
		CLSID  clsid;
		if (FAILED(hr = CLSIDFromProgID(CComBSTR("VWSYSTEM.SpriteManager.1"), &clsid)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_ISpriteManager, (void **)&m_pSpriteManager)))
			goto ERROR_ENCOUNTERED;
	}

	// Extract the necessary info from the sprite file.
	_splitpath(CString(bstrGeometryPath), NULL, NULL, NULL, szExt );
	if (_stricmp(szExt, ".SPR") == 0)
	{
		// Read the sprite file.
		if (FAILED(hr = m_pSpriteManager->ReadSpriteFile(bstrGeometryPath, &bstrError.m_str)))
		{
			m_pWorld->Report(bstrError, -1);
			goto ERROR_ENCOUNTERED;
		}
		// The image file should be in the same URL as the sprite file.
		m_pSpriteManager->get_ImageFile(&bstrImageFile.m_str);
		iFwdSlash = CString(bstrGeometryURL).ReverseFind('/');
		iBackSlash = CString(bstrGeometryURL).ReverseFind('\\');
		if (iBackSlash > iFwdSlash)
			bstrImageFileURL = CString(bstrGeometryURL).Left(iBackSlash+1) + CString(bstrImageFile);
		else
			bstrImageFileURL = CString(bstrGeometryURL).Left(iFwdSlash+1) + CString(bstrImageFile);
		// Now get it!
		if (FAILED(hr = pINetFileMgr->FindCachedFile(bstrImageFileURL, &bstrReturnedURL.m_str, &bstrImageFilePath.m_str, &bFound)) || !bFound)
		{
			if (FAILED(hr = pINetFileMgr->GetFileEx(bstrImageFileURL, VARIANT_FALSE, &bstrReturnedURL.m_str, &bstrImageFilePath.m_str)))
			{
				m_pWorld->Report(CComBSTR(CString("VWSelfView: Failed to download file: ") + CString(bstrImageFileURL)), -1);
				goto ERROR_ENCOUNTERED;
			}
		}
	}
	else
	{
		// Assume this is an image file name.
		bstrImageFilePath = CComBSTR(bstrGeometryPath);
		bstrImageFileURL = CComBSTR(bstrGeometryURL);
		if ((SUCCEEDED(pINetFileMgr->FindCachedFile(m_bstrDefaultSpriteFile, &bstrDefaultFileFullURL.m_str, &bstrDefaultFileFinalPath.m_str, &bFound)) && bFound) ||
			SUCCEEDED(pINetFileMgr->GetFileEx(m_bstrDefaultSpriteFile, VARIANT_FALSE, &bstrDefaultFileFullURL.m_str, &bstrDefaultFileFinalPath.m_str)))
		{
			// Read the sprite file.
			if (FAILED(hr = m_pSpriteManager->ReadSpriteFile(bstrDefaultFileFinalPath.m_str, &bstrError.m_str)))
			{
				m_pWorld->Report(bstrError, -1);
				goto ERROR_ENCOUNTERED;
			}
		}
		else
		{
			m_pWorld->Report(CComBSTR(CString("VWSelfView: Failed to download default sprite file: ") + CString(m_bstrDefaultSpriteFile)), -1);
			// We need to create sprite file data (just show frame 0 of avatar).
			if (FAILED(hr = m_pSpriteManager->CreateSpriteFileData(NULL, 1)))
				goto ERROR_ENCOUNTERED;
		}
	}
	
	if (FAILED(hr = SetSelfViewBackground(bstrImageFilePath)))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:

	if (FAILED(hr))
	{
		// Avatars should always default back to the default sprite file.
		if (m_bstrDefaultSpriteFile.Length() != 0 &&
			!(CString(m_bstrGeometryName) == CString(m_bstrDefaultSpriteFile)))
		{
			bstrGeometryName = m_bstrGeometryName;
			m_bstrGeometryName = m_bstrDefaultSpriteFile;
			GeometryNameChanged();
			m_bstrGeometryName = bstrGeometryName;
		}
	}
	return hr;
}

HRESULT CVWSelfVwCtrl::AccessoryFrameFromID(long lID, LPDIRECT2DRMFRAME *ppFrame)
{
	IDirect2DRMArray	*pChildren = NULL;
	DWORD				dwChildren, dwType, dwAppData;
	int					iChild;
	HRESULT hr = S_OK;

	*ppFrame = NULL;

	if (!m_pAccessoryScene)
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = m_pAccessoryScene->get_Children(&pChildren)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pChildren->get_Size(&dwChildren)))
		goto ERROR_ENCOUNTERED;

	for (iChild = 0; iChild < (long)dwChildren; iChild++)
	{
		if (FAILED(hr = pChildren->GetElement(iChild, &dwType, (IDirect2DRMObject**)ppFrame)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = (*ppFrame)->get_AppData(&dwAppData)))
			goto ERROR_ENCOUNTERED;
		if ((long)dwAppData == lID)
		{
			break;
		}
		SAFERELEASE(*ppFrame);
	}

ERROR_ENCOUNTERED:

	SAFERELEASE(pChildren);

	return hr;
}

void CVWSelfVwCtrl::OnTimer(UINT nIDEvent)
{
	DWORD dwVisibleImage, dwImageFrame;

	if (!m_pd2diBackground)
		return;
	
	if (FAILED(m_pSpriteManager->CalcCurrentGestureFrame()) ||
		FAILED(m_pSpriteManager->GetImageFrame(0.0F, (int*)&dwImageFrame, &m_bShowAccessories)))
		return;

	m_pd2diBackground->get_VisibleImage(&dwVisibleImage);
	if (dwImageFrame != dwVisibleImage)
	{
		m_pd2diBackground->put_VisibleImage(dwImageFrame);
		UpdateSelfView();
	}
}

void CVWSelfVwCtrl::SetHandleWorldEvents(BOOL bHandleWorldEvents)
{
	if (m_pWorld)
		return;
	m_bHandleWorldEvents = bHandleWorldEvents;
}

BOOL CVWSelfVwCtrl::GetHandleWorldEvents()
{
	return m_bHandleWorldEvents;
}