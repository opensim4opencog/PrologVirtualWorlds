// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// UserCtl.cpp : Implementation of the CUsersCtrl ActiveX Control class.

#include "stdafx.h"
#include "uictrl.h"
#include "afxcmn.h"
#include "UserCtl.h"
#include "cutils.h"
#include "ddutil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CUsersCtrl, CVWViewCtrl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CUsersCtrl, CVWViewCtrl)
	//{{AFX_MSG_MAP(CUsersCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(WM_USER, (WM_USER+200), OnCommandRange)
	ON_UPDATE_COMMAND_UI_RANGE(WM_USER, (WM_USER+200), OnCommandRangeUI)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CUsersCtrl, CVWViewCtrl)
	//{{AFX_DISPATCH_MAP(CUsersCtrl)
	DISP_PROPERTY_EX(CUsersCtrl, "VWClient", GetVWClient, SetVWClient, VT_DISPATCH)
	DISP_FUNCTION(CUsersCtrl, "RendererIsLoaded", RendererIsLoaded, VT_EMPTY, VTS_NONE)
	DISP_STOCKPROP_FONT()
	DISP_STOCKPROP_BACKCOLOR()
	DISP_STOCKPROP_FORECOLOR()
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CUsersCtrl, CVWViewCtrl)
	//{{AFX_EVENT_MAP(CUsersCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CUsersCtrl, 2)
	PROPPAGEID( CLSID_CFontPropPage )
    PROPPAGEID( CLSID_CColorPropPage )
END_PROPPAGEIDS(CUsersCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CUsersCtrl, "VWCLIENTUI.UsersCtrl.1",
	0xdc9ca0c4, 0x336f, 0x11d1, 0xbb, 0x22, 0, 0xc0, 0x4f, 0xc2, 0xca, 0xb1)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CUsersCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DUsers =
		{ 0xdc9ca0c2, 0x336f, 0x11d1, { 0xbb, 0x22, 0, 0xc0, 0x4f, 0xc2, 0xca, 0xb1 } };
const IID BASED_CODE IID_DUsersEvents =
		{ 0xdc9ca0c3, 0x336f, 0x11d1, { 0xbb, 0x22, 0, 0xc0, 0x4f, 0xc2, 0xca, 0xb1 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwUsersOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CUsersCtrl, IDS_USERS, _dwUsersOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CUsersCtrl::CUsersCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CUsersCtrl
BOOL CUsersCtrl::CUsersCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_USERS,
			IDB_USERS,
			afxRegApartmentThreading,
			_dwUsersOleMisc,
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
// CUsersCtrl::CUsersCtrl - Constructor

CUsersCtrl::CUsersCtrl()
{
	InitializeIIDs(&IID_DUsers, &IID_DUsersEvents);


}


/////////////////////////////////////////////////////////////////////////////
// CUsersCtrl::~CUsersCtrl - Destructor

CUsersCtrl::~CUsersCtrl()
{

}


/////////////////////////////////////////////////////////////////////////////
// CUsersCtrl::OnDraw - Drawing function

void CUsersCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	//pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
}


/////////////////////////////////////////////////////////////////////////////
// CUsersCtrl::DoPropExchange - Persistence support

void CUsersCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	CVWViewCtrl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CUsersCtrl::OnResetState - Reset control to default state

void CUsersCtrl::OnResetState()
{
	CVWViewCtrl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



/////////////////////////////////////////////////////////////////////////////
// CUsersCtrl message handlers


LPDISPATCH CUsersCtrl::GetVWClient() 
{
	return CVWViewCtrl::GetVWClient();
}

void CUsersCtrl::SetVWClient(LPDISPATCH newValue) 
{
	CVWViewCtrl::SetVWClient(newValue);
	SetModifiedFlag();
}

//******************************************************
// OnCreate
// Called when the control is created - this makes
// the various views...

int CUsersCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CVWViewCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	RECT rect = { 0,0, lpCreateStruct->cx, lpCreateStruct->cy };

   	//if (m_userList.Create(WS_VISIBLE | LVS_LIST | LVS_AUTOARRANGE ,rect, this, 254 ) == -1 )
	if (m_userList.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_NOCOLUMNHEADER  | LVS_AUTOARRANGE, rect, this, 254) == -1)
		return -1;

	m_userList.InsertColumn(0, "Object", LVCFMT_LEFT,120,0);
	m_userList.m_pUserCtrl = this;

	CFontHolder& font = InternalGetFont();
	HFONT hFont = font.GetFontHandle();
	if ( hFont )
	{
		m_userList.SetFont(CFont::FromHandle(hFont ), TRUE); 
	}

	COLORREF cref = TranslateColor(GetBackColor());
	m_userList.SetBkColor( cref );
	m_userList.SetTextBkColor( cref );
	m_userList.SetTextColor(TranslateColor(GetForeColor()));


	
	m_userList.EnableScrollBar(SB_VERT);
	return 0;
}


//******************************************************
// OnSize
// Resize the embedded control

void CUsersCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CVWViewCtrl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_userList.SetWindowPos(&wndTop,0,0,cx-10,cy,SWP_NOZORDER );
	
}


//******************************************************
// HandleOnUserConnect
// Called when the user initially connects...

HRESULT CUsersCtrl::HandleOnUserConnect(IThing* pthing, BOOL bFromClient)
{
	CVWViewCtrl::HandleOnUserConnect(pthing,bFromClient);
	
	HRESULT	hr;
	IThing	*pUser = NULL;

	hr = m_pWorld->get_User(&pUser);
	if (FAILED(hr))
		goto CLEAN_UP;
	
	if (pthing == pUser)
		hr = m_userList.RefreshList();

	else if (pthing)
	{
		VARIANT_BOOL	bShow;

		// Filter for avatars
		hr = m_pWorld->IsAvatar(pthing, &bShow);
		if (FAILED(hr))
			goto CLEAN_UP;
		
		if (bShow == VARIANT_TRUE)
		{
			hr = pthing->get_BOOL(CComBSTR("IsConnected"), &bShow);
			if (FAILED(hr))
				bShow = VARIANT_FALSE;
		}
		else
		{
			hr = pthing->get_BOOL(CComBSTR("IsAvatar"), &bShow);
			if (FAILED(hr))
				bShow = VARIANT_FALSE;
		}

		if (bShow == VARIANT_TRUE)
				m_userList.AddThing(pthing);
	}

CLEAN_UP:
	SAFERELEASE(pUser);
	return hr;
}


HRESULT CUsersCtrl::HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient)
{
	CVWViewCtrl::HandleOnUserDisconnect(pthing,bFromClient);

		HRESULT	hr;
	IThing	*pUser = NULL;

	hr = m_pWorld->get_User(&pUser);
	if (FAILED(hr))
		goto CLEAN_UP;
	
	if (pthing == pUser)
		hr = m_userList.ClearList();
	else if (pthing)
		m_userList.RemoveThing(pthing);

CLEAN_UP:
	SAFERELEASE(pUser);
	return hr;
}

HRESULT CUsersCtrl::HandleOnDisconnect(IDispatch* pWorld)
{
	CVWViewCtrl::HandleOnDisconnect(pWorld);

	return m_userList.ClearList();
}

//******************************************************
// HandleOnEnterRoom
// Called when something enters a room

HRESULT CUsersCtrl::HandleOnEnterRoom(IThing* pWhere, IThing* pWhat)
{
//	CVWViewCtrl::HandleOnEnterRoom(pWhere, pWhat);

	HRESULT	hr;
	IThing	*pUser = NULL;

	hr = m_pWorld->get_User(&pUser);
	if (FAILED(hr))
		goto CLEAN_UP;
	
	if (pWhat == pUser)
		hr = m_userList.RefreshList();
	else if (pWhat)
	{
		VARIANT_BOOL	bShow;

		// Filter for avatars
		hr = m_pWorld->IsAvatar(pWhat, &bShow);
		if (FAILED(hr))
			goto CLEAN_UP;
		
		if (bShow == VARIANT_TRUE)
		{
			hr = pWhat->get_BOOL(CComBSTR("IsConnected"), &bShow);
			if (FAILED(hr))
				bShow = VARIANT_FALSE;
		}
		else
		{
			hr = pWhat->get_BOOL(CComBSTR("IsAvatar"), &bShow);
			if (FAILED(hr))
				bShow = VARIANT_FALSE;
		}

		if (bShow == VARIANT_TRUE)
			m_userList.AddThing(pWhat);
	}

CLEAN_UP:
	SAFERELEASE(pUser);
	return hr;
}

//******************************************************
// HandleOnLeaveRoom
// Called when an object leaves the room....


// REVIEW  The current case that doesnt work is when a peer disconnects, we dont get notified
// I consulted with AC and found out that eventually, when a peer disconnects, they will get 
// to another room.  So... this event handler should eventually handle that scenerio, and I wont 
// change any code.
HRESULT CUsersCtrl::HandleOnLeaveRoom(IThing* pWhere, IThing* pWhat)
{
//	CVWViewCtrl::HandleOnLeaveRoom(pWhere, pWhat);
	
	HRESULT	hr;
	IThing	*pUser = NULL;

	hr = m_pWorld->get_User(&pUser);
	if (FAILED(hr))
		goto CLEAN_UP;
	
	if (pWhat == pUser)
		hr = m_userList.ClearList();
	else if (pWhat)
		m_userList.RemoveThing(pWhat);

CLEAN_UP:
	SAFERELEASE(pUser);
	return hr;
}

HRESULT CUsersCtrl::HandleOnGeometryChanged(IThing* pthing)
{
//	CVWViewCtrl::HandleOnGeometryChanged(pthing);
	m_userList.ResetImage(pthing);
	return S_OK; 
}

void CUsersCtrl::OnCommandRange(UINT nID)
{
	HRESULT hr = S_OK;
	m_Menu.OnCommandRange(nID, &hr);
		
}


void CUsersCtrl::OnCommandRangeUI(CCmdUI* pCmdUI)
{

	HRESULT hr;
	m_Menu.OnCommandRangeUI( pCmdUI, &hr );
}


void CUsersCtrl::OnBackColorChanged() 
{
	CVWViewCtrl::OnBackColorChanged();
	OLE_COLOR nNewValue = GetBackColor();
	COLORREF cref = TranslateColor(nNewValue);
	m_userList.SetBkColor( cref );
	m_userList.SetTextBkColor( cref );
	m_userList.InvalidateRect(NULL);
}

void CUsersCtrl::OnForeColorChanged() 
{
	CVWViewCtrl::OnForeColorChanged();
	OLE_COLOR nNewValue = GetForeColor();
	COLORREF cref = TranslateColor(nNewValue);
	m_userList.SetTextColor( cref );
	m_userList.InvalidateRect(NULL);
}

void CUsersCtrl::OnAmbientPropertyChange(DISPID dispid)
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

void CUsersCtrl::OnFontChanged()
{
	CVWViewCtrl::OnFontChanged();

	
	CFontHolder& font = InternalGetFont();

	HFONT hFont = font.GetFontHandle();
	if ( hFont && m_userList.m_hWnd )
	{
		m_userList.SetFont(CFont::FromHandle(hFont ), TRUE); 
	}
	
}


void CUsersCtrl::RendererIsLoaded() 
{
	// TODO: Add your dispatch handler code here

}

#include <vwevents.h>

//******************************************************
// HandleOnUIEvent
// OK, we pick off the various events that need to be
// reflected in the user list. 

HRESULT CUsersCtrl::HandleOnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient)
{
	CString	strEventName = bstrEventName;
	
	if (!strEventName.CompareNoCase(VW_BEFRIEND_EVENT_STR))
		return HandleOnBefriend((IThing *)varArg.pdispVal);
	else if (!strEventName.CompareNoCase(VW_UNBEFRIEND_EVENT_STR))
		return HandleOnUnBefriend((IThing *)varArg.pdispVal);
	else if (!strEventName.CompareNoCase(VW_IGNOREDUSER_EVENT_STR))
		return HandleOnIgnore((IThing *)varArg.pdispVal);
	else if (!strEventName.CompareNoCase(VW_UNIGNOREDUSER_EVENT_STR))
		return HandleOnUnIgnore((IThing *)varArg.pdispVal);
	else if (!strEventName.CompareNoCase(VW_IGNOREDBYUSER_EVENT_STR))
	{
		// do nothing for now
	}
	else if (!strEventName.CompareNoCase(VW_PROFILECHANGED_EVENT_STR))
	{
		// do nothing for now
	}
	else if (!strEventName.CompareNoCase(VW_NAMECHANGED_EVENT_STR))
	{
		// redraw the list
		// TO DO: Make sure the item is in the list first
		m_userList.RefreshList();
	}
	else if (!strEventName.CompareNoCase(VW_NAMECHANGED_EVENT_STR))
	{
		// redraw the list
		// TO DO: Make sure the item is in the list first
		m_userList.RefreshList();
	}
	else
		CVWViewCtrl::HandleOnUIEvent(pthing, bstrEventName, varArg, bFromClient);

	return S_OK;
}


HRESULT	CUsersCtrl::HandleOnBefriend(IThing *pNewFriend)
{
	TRACE0("HandleOnBefriend\n");
	m_userList.RefreshList();
	return S_OK;
}


HRESULT CUsersCtrl::HandleOnUnBefriend(IThing *pFormerFriend)
{
	TRACE0("HandleOnUnBefriend\n");
	m_userList.RefreshList();
	return S_OK;
}


HRESULT CUsersCtrl::HandleOnIgnore(IThing *pIgnored)
{
	TRACE0("HandleOnIgnore\n");
	m_userList.RefreshList();
	return S_OK;
}


HRESULT CUsersCtrl::HandleOnUnIgnore(IThing *pUnIngored)
{
	TRACE0("HandleOnUnIgnore\n");
	m_userList.RefreshList();
	return S_OK;
}




//********************************************
//********************************************
// CUserListView
// The actual view that draws the user list...


IMPLEMENT_DYNCREATE(CUserListView, CListCtrl)


CUserListView::CUserListView()
{
	m_pUserCtrl = NULL;
	m_bSelected = FALSE;
	m_pImageList = NULL;
	m_imagedx = m_imagedy = 24;
}

CUserListView::~CUserListView()
{
	m_pUserCtrl = NULL;
	if(m_pImageList)
		delete m_pImageList;
}


BEGIN_MESSAGE_MAP(CUserListView, CListCtrl)
	//{{AFX_MSG_MAP(CUserListView)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_USERLIST_REDRAW, OnRedrawList)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGING, OnItemchanging)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserListView drawing

void CUserListView::OnDraw(CDC* pDC)
{
	CRect	rcBounds;

	GetClientRect(&rcBounds);
	pDC->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
}

/////////////////////////////////////////////////////////////////////////////
// CUserListView diagnostics

#ifdef _DEBUG
void CUserListView::AssertValid() const
{
	CListCtrl::AssertValid();
}

void CUserListView::Dump(CDumpContext& dc) const
{
	CListCtrl::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CUserListView message handlers

//LPCTSTR MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen);
/*LPCTSTR MakeShortString(CDC* pDC, LPCTSTR lpszLong, int nColumnLen)
{
	static const _TCHAR szThreeDots[]=_T("...");

	int nStringLen=lstrlen(lpszLong);

	if(nStringLen==0 || pDC->GetTextExtent(lpszLong,nStringLen).cx<=nColumnLen)
		return(lpszLong);

	static _TCHAR szShort[MAX_PATH];

	lstrcpy(szShort,lpszLong);
	int nAddLen=pDC->GetTextExtent(szThreeDots,sizeof(szThreeDots)).cx;

	for(int i=nStringLen-1; i>0; i--)
	{
		szShort[i]=0;
		if(pDC->GetTextExtent(szShort,i).cx+nAddLen<=nColumnLen)
			break;
	}

	lstrcat(szShort,szThreeDots);

	return(szShort);
}*/


//******************************************************
// DrawItem
// Actually draw the item.

/*const short	kNumAstacons = 4;
const short	kAstaconWidth = 26;
const short	kEmblemSize = kNumAstacons * kAstaconWidth;
const short	kMinTextWidth = 50;

void CUserListView::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CUserRecord	userRec		= m_userMap[(long)lpDrawItemStruct->itemData];
	CDC*		pDC			= CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect		itemRect	= lpDrawItemStruct->rcItem;
	CRect		iconRect	(0,0,26,26);
	CBrush		blackBrush	(RGB(0,0,0));
	CBrush		grayBrush	(RGB(0x7F,0x7F,0x7F));
	CBrush		ltGrayBrush	(RGB(0xAF,0xAF,0xAF));
	COLORREF	clrTextSave;
	COLORREF	clrBkSave;

	BOOL bSelected=(lpDrawItemStruct->itemState & ODS_SELECTED);

	if(bSelected)
	{
		clrTextSave=pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		clrBkSave=pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));

		pDC->FillRect(&itemRect,&CBrush(::GetSysColor(COLOR_HIGHLIGHT)));
	}
	else
	{
		// TO DO:  Don't actually have to do this unless the selection
		// state is changing...
		pDC->FillRect(&itemRect,&CBrush(::GetSysColor(COLOR_WINDOW)));
	}
		

	// draw the icon
	iconRect.OffsetRect(itemRect.left+2, itemRect.top+2);
	if (userRec.m_strPreviewIcon != "")
	{
		// TO DO:  actually draw the icon
		if (userRec.m_bHost)
		{
			// draw the host icon
		}
		else if (userRec.m_bIgnored)
		{
			// draw the ignored icon
		}
		else if (userRec.m_bIgnoringYou)
		{
			// draw the ignoring you icon
		}
		else
		{
			// draw the thumbnail
		}
	}
	pDC->FrameRect(&iconRect, &blackBrush);

	// draw the name
	CString	strName		= userRec.m_strName;
	LPCTSTR	pszText;
	long	lTextStart	= itemRect.left + 30;
	long	lTextWidth	= (itemRect.right) - lTextStart;	// width of item rect - icon
	CFont	boldFont, *pOldFont;

	// now, shrink it to fit as much emblem as possible..
	lTextWidth -= kEmblemSize;
	if (lTextWidth < kMinTextWidth)
		lTextWidth = kMinTextWidth;	// trim to minimum size

	pOldFont = pDC->GetCurrentFont();

	if (userRec.m_bFriend)
	{
		LOGFONT	oldFont;

		pOldFont->GetLogFont(&oldFont);
		oldFont.lfWeight = FW_BOLD;

		boldFont.CreateFontIndirect(&oldFont);
		pOldFont = pDC->SelectObject(&boldFont);
	}

	// draw that text!
	pszText=MakeShortString(pDC,strName, lTextWidth);
	pDC->TextOut(lTextStart, itemRect.top + 6, pszText, strlen(pszText));
	pDC->SelectObject(pOldFont);

	// draw the emblem
	// TO DO:  Use the real icons...
	long	lEmblemStart = lTextStart + lTextWidth;
	CRgn	astaconRgn;
	POINT	pts[6] = {{10,0}, {14, 0}, {24, 12}, {14, 24}, {10, 24}, {0, 12}};

	astaconRgn.CreatePolygonRgn(pts, 6, ALTERNATE);
	astaconRgn.OffsetRgn(lEmblemStart, itemRect.top+1);

	for (short i = 0; i < kNumAstacons; i++)
	{
		iconRect.SetRect(lEmblemStart + (kAstaconWidth * i), itemRect.top + 1, lEmblemStart + (kAstaconWidth * (i+1)), itemRect.top + kAstaconWidth);
		if (userRec.m_strEmblemIcon[i] != "")
		{
			// TO DO:  actually draw the emblem icon
		}
		else 
		{
			// draw the empty astacon
			pDC->FillRgn(&astaconRgn, &grayBrush);
			pDC->FrameRgn(&astaconRgn, &ltGrayBrush, 1, 1);
		}

		astaconRgn.OffsetRgn(kAstaconWidth, 0);
	}

	// restore pen colors if we changed them for selection....
	if(bSelected)
	{
		pDC->SetTextColor(clrTextSave);
		pDC->SetBkColor(clrBkSave);
	}

	if (userRec.m_bIgnored)
	{
		// TO DO: gray out the entire line - use PatBlt?
	}

}*/

//******************************************************
// MeasureItem
// Returns the size of the item - we should fix this at
// the standard size

/*void CUserListView::MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct )
{
	lpMeasureItemStruct->itemWidth = 300;
	lpMeasureItemStruct->itemHeight = 28;
}*/


//******************************************************
// CompareItem
// Compares two items

/*int CUserListView::CompareItem( LPCOMPAREITEMSTRUCT lpCompareItemStruct )
{
	// TO DO:  sort by name??
	return 0;
}*/

//******************************************************
// AddThing
// Adds a thing to the list

int CALLBACK CompareUsers(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	IThing* pParam1Thing = NULL;
	IThing* pParam2Thing = NULL;
	int nParam1 = 1; // 0 designates friends, 1 normal and 2 ignored.
	int nParam2 = 1;
	CComBSTR szParam1Name;
	CComBSTR szParam2Name;
	CString strParam1Name;
	CString strParam2Name;
	int nResult = 0;
	IThing* pUser = NULL;

	CComVariant varResult;
	VARIANT rgvarg[] = {{0,0,0,0,0}, {0,0,0,0,0}};
	DISPPARAMS	dispparams = {rgvarg,NULL,2,0}; 	
	static CComBSTR bstrFriends("Friends");
	static CComBSTR bstrIgnoring("Ignoring");


	CUsersCtrl* pParent = (CUsersCtrl*)lParamSort;

	HRESULT hr = S_OK;

	if(pParent)
	{
		if(pParent->m_pWorld)
		{
			hr = pParent->m_pWorld->get_User(&pUser);
			if (FAILED(hr) || (!pUser))
				goto EXIT;

			hr = pParent->m_pWorld->get_ObjectByID(lParam1, &pParam1Thing);
			if((FAILED(hr)) || (!pParam1Thing)) goto EXIT;

			hr = pParent->m_pWorld->get_ObjectByID(lParam2, &pParam2Thing);
			if((FAILED(hr)) || (!pParam2Thing)) goto EXIT;


			if ( pUser == pParam1Thing )
			{
				nResult = -1;
				goto EXIT;
			}
			if ( pUser == pParam2Thing )
			{
				nResult = 1;
				goto EXIT;
			}

			// Test to see if it is a friend
			::VariantInit(&(rgvarg[0]));
			::VariantInit(&(rgvarg[1]));

			dispparams.rgvarg[1].vt = VT_DISPATCH;
			dispparams.rgvarg[1].pdispVal = pParam1Thing;
			dispparams.rgvarg[0].vt = VT_BSTR;
			dispparams.rgvarg[0].bstrVal = bstrFriends.m_str;

			hr = pUser->InvokeMethodExt(CComBSTR("IsInUserProfileList"), &dispparams, &varResult);
			if (SUCCEEDED(hr))
			{
				if ((varResult.vt == VT_BOOL ) &&
					(varResult.boolVal == VARIANT_TRUE))
				{
					nParam1 = 0;
				}
			}

			//Are we ignoring it?
			dispparams.rgvarg[0].vt = VT_BSTR;
			dispparams.rgvarg[0].bstrVal = bstrIgnoring.m_str;
			hr = pUser->InvokeMethodExt(CComBSTR("IsInUserProfileList"), &dispparams, &varResult);
			if (SUCCEEDED(hr))
			{
				if ((varResult.vt == VT_BOOL ) &&
					(varResult.boolVal == VARIANT_TRUE))
				{
					nParam1 = 2;
				}
			}

			dispparams.rgvarg[1].vt = VT_DISPATCH;
			dispparams.rgvarg[1].pdispVal = pParam2Thing;
			dispparams.rgvarg[0].vt = VT_BSTR;
			dispparams.rgvarg[0].bstrVal = bstrFriends.m_str;

			hr = pUser->InvokeMethodExt(CComBSTR("IsInUserProfileList"), &dispparams, &varResult);
			if (SUCCEEDED(hr))
			{
				if ((varResult.vt == VT_BOOL ) &&
					(varResult.boolVal == VARIANT_TRUE))
				{
					nParam2 = 0;
				}
			}

			//Are we ignoring it?
			dispparams.rgvarg[0].vt = VT_BSTR;
			dispparams.rgvarg[0].bstrVal = bstrIgnoring.m_str;
			hr = pUser->InvokeMethodExt(CComBSTR("IsInUserProfileList"), &dispparams, &varResult);
			if (SUCCEEDED(hr))
			{
				if ((varResult.vt == VT_BOOL ) &&
					(varResult.boolVal == VARIANT_TRUE))
				{
					nParam2 = 2;
				}
			}

			// first check the exclusive case
			if(nParam1 < nParam2)
			{
				nResult = -1;
				goto EXIT;
			}

			if(nParam1 > nParam2)
			{
				nResult = 1;
				goto EXIT;
			}

			// now need the names to check the non-exclusive case (alphabetical)
			hr = pParam1Thing->get_Name(&szParam1Name.m_str);
			if(FAILED(hr)) goto EXIT;

			strParam1Name = szParam1Name;

			hr = pParam2Thing->get_Name(&szParam2Name.m_str);
			if(FAILED(hr)) goto EXIT;

			strParam2Name = szParam2Name;

			// now compare these guys
			nResult = strParam1Name.CompareNoCase(strParam2Name);
		}
	}

EXIT:
	SAFERELEASE(pParam1Thing);
	SAFERELEASE(pParam2Thing);
	SAFERELEASE(pUser);
	return nResult;
}


int GetProfileStatus( IThing *pUser, IThing *pThing )
{

	CComVariant varResult;
	VARIANT rgvarg[] = {{0,0,0,0,0}, {0,0,0,0,0}};
	DISPPARAMS	dispparams = {rgvarg,NULL,2,0}; 	
	static CComBSTR bstrFriends("Friends");
	static CComBSTR bstrIgnoring("Ignoring");

	HRESULT hr = S_OK;

	// Test to see if it is a friend
	::VariantInit(&(rgvarg[0]));
	::VariantInit(&(rgvarg[1]));

	dispparams.rgvarg[1].vt = VT_DISPATCH;
	dispparams.rgvarg[1].pdispVal = pThing;

	// Friend and ignore don't seem to be mutually exclusing, so ignore first
	//Are we ignoring it?
	dispparams.rgvarg[0].vt = VT_BSTR;
	dispparams.rgvarg[0].bstrVal = bstrIgnoring.m_str;

	hr = pUser->InvokeMethodExt(CComBSTR("IsInUserProfileList"), &dispparams, &varResult);
	if ( SUCCEEDED(hr) &&
		(varResult.vt == VT_BOOL ) &&
		(varResult.boolVal == VARIANT_TRUE))
	{
		return 2;
	}
	
	dispparams.rgvarg[0].vt = VT_BSTR;
	dispparams.rgvarg[0].bstrVal = bstrFriends.m_str;

	hr = pUser->InvokeMethodExt(CComBSTR("IsInUserProfileList"), &dispparams, &varResult);
	if ( SUCCEEDED(hr) &&
	    (varResult.vt == VT_BOOL ) &&
		(varResult.boolVal == VARIANT_TRUE))
	{
		return 1;
	}

	return 0;
}


HRESULT CUserListView::AddThing(IThing *pThing)
{
	HRESULT	hr = S_OK;
	LPBITMAPINFOHEADER pDIB = NULL;
	COLORREF crBack = GetBkColor();
	int item = -1;
	int nStatus = 0;
	COLORREF ProfileColor[3]= { crBack, crBack, RGB(0,0,0)};
	CThingPtr UserPtr;



	if (pThing)
	{
		long	id			= 0;
		// get ID doesn't fail
		pThing->get_ID( &id );
		CThingPtr	ThingPtr;
		if (m_userMap.Lookup(id, ThingPtr ) != 0 )
		{
			//already in the list...
			//get out
			return S_OK; 
		}

		CComBSTR	bstrName, bstrText;
		long lImage = 0;


		hr = pThing->get_Name( &bstrName.m_str );
		if (FAILED(hr))
			return hr;

		hr = m_pUserCtrl->m_pWorld->get_User( &UserPtr.p );
		if ( FAILED( hr ))
			return hr;
		nStatus = GetProfileStatus(UserPtr,pThing);
		crBack = ProfileColor[nStatus];

		// get the thumbnail for this avatar
		switch (nStatus) 
		{
			case 0:
			case 1:
			case 2:
			hr = GetBitmapFromThing(m_pUserCtrl->m_pWorld, pThing, crBack, &pDIB);
			if ( SUCCEEDED(hr) && pDIB)
			{
					HBITMAP hbm= DIBToBitmap(m_imagedx,m_imagedy,8,pDIB, nStatus);
					if ( hbm )
					{
						lImage =ImageList_Add(m_pImageList->m_hImageList, hbm,hbm);
						::DeleteObject( hbm );					
					}
					free(pDIB);

			}
			break;
			default:
			break;
		}
		// insert the item w/ bitmap
		item = InsertItem(0, CString(bstrName.m_str),lImage);

		if ( item >= 0 )
		{
			
			SetItemData( item,(unsigned long)id );
			m_userMap[id] = CThingPtr(pThing);
		}

		VERIFY(SortItems(CompareUsers, (DWORD)m_pUserCtrl));
	}
	return hr;
}

//******************************************************
// RemoveThing
// Removes a thing from the list...

HRESULT CUserListView::RemoveThing(IThing *pThing)
{
	if (pThing && m_hWnd)
	{
		HRESULT		hr;
		long		id = 0;

		hr = pThing->get_ID( &id );
		
		if (SUCCEEDED(hr))
		{
			LV_FINDINFO	findInfo;
			int			iItem;

			findInfo.flags = LVFI_PARAM;
			findInfo.lParam = id;
		
			iItem = FindItem(&findInfo);

			if (iItem != -1)
			{
				DeleteItem(iItem);
				m_userMap.RemoveKey(id);
			}
		}
	}


	return S_OK;
}

HRESULT CUserListView::ResetImage(IThing *pThing)
{
	if (pThing && m_hWnd)
	{

		HRESULT		hr;
		long		id = 0;

		hr = pThing->get_ID( &id );
		
		if (SUCCEEDED(hr))
		{
			LV_FINDINFO	findInfo;
			int			iItem;

			findInfo.flags = LVFI_PARAM;
			findInfo.lParam = id;
		
			iItem = FindItem(&findInfo);

			if (iItem != -1)
			{
				LV_ITEM item;
				memset(&item,0,sizeof(LV_ITEM));
				item.mask = LVIF_IMAGE;
				item.iItem = iItem;
				if (GetItem(&item))
				{
					HRESULT	hr;
					CThingPtr UserPtr;
					LPBITMAPINFOHEADER pDIB = NULL;
					int nStatus;
					COLORREF crBack = GetBkColor();
					COLORREF ProfileColor[3]= { crBack, crBack, RGB(0,0,0)};
					BOOL bRet;

					hr = m_pUserCtrl->m_pWorld->get_User( &UserPtr.p );
					if ( FAILED( hr ))
						return hr;

					nStatus = GetProfileStatus(UserPtr,pThing);
					crBack = ProfileColor[nStatus];

					// get the thumbnail for this avatar
					switch (nStatus) 
					{
						case 0:
						case 1:
						case 2:
						// get the thumbnail for this avatar
						hr = GetBitmapFromThing(m_pUserCtrl->m_pWorld, pThing, crBack, &pDIB);
						if ( SUCCEEDED(hr) && pDIB)
						{
								HBITMAP hbm= DIBToBitmap(m_imagedx,m_imagedy,8,pDIB, nStatus);
								if ( hbm )
								{
									if ( item.iImage > 1 )
										bRet = ImageList_Replace(m_pImageList->m_hImageList,item.iImage,hbm,hbm);
									else
									{
										memset(&item,0,sizeof(LV_ITEM));
										item.iImage = ImageList_Add(m_pImageList->m_hImageList,hbm,hbm);
										item.mask = LVIF_IMAGE;
										item.iItem = iItem;
										SetItem(&item);
									}

									::DeleteObject( hbm );					
								}
								free(pDIB);
								RedrawItems(iItem,iItem);
								UpdateWindow();

						}
						else // replace the image with the default bitmap
						{
							if ( item.iImage > 1 )
							{
								memset(&item,0,sizeof(LV_ITEM));
								item.iImage = nStatus == 2? 1 : 0;
								item.mask = LVIF_IMAGE;
								item.iItem = iItem;
								SetItem(&item);
							}
								
						}	
						break;
						default:
						break;
					}

					
				}
			}
		}
	}


	return S_OK;
}

//******************************************************
// RefreshList
// Refreshes the list...

HRESULT CUserListView::RefreshList(void)
{
	if (!m_hWnd)
		return S_OK;

	if (!m_pUserCtrl)
		return S_OK;
	
	ClearList();

	IWorld				*pWorld		= m_pUserCtrl->m_pWorld;
	HRESULT				hr			= S_OK;
	IPropertyList		*pContents	= NULL;
	IThing				*pContainer	= NULL;;
	IThing				*pUser		= NULL;;
	IThing				*pThing		= NULL;
	long				lItems;
	int					i;

	hr = pWorld->get_User(&pUser);
	if (FAILED(hr) || (!pUser))
		goto CLEAN_UP;

	hr = pUser->get_Container(&pContainer);
	if (FAILED(hr) || (!pContainer))
		goto CLEAN_UP;

	hr = pContainer->get_Contents(&pContents);
	if (FAILED(hr) || (!pContents))
		goto CLEAN_UP;

	hr = pContents->get_Count( &lItems );
	if (FAILED(hr))
		goto CLEAN_UP;

	for ( i = 0; i < lItems; i++)
	{
		VARIANT_BOOL bShow = VARIANT_FALSE;

		hr = pContents->get_Thing(i, &pThing);
		if (FAILED(hr))
			goto CLEAN_UP;
		
		if (pThing)
		{
			// Filter for avatars
			hr = pWorld->IsAvatar(pThing, &bShow);
			if (FAILED(hr))
				goto CLEAN_UP;
			
			if (bShow == VARIANT_TRUE)
			{
				hr = pThing->get_BOOL(CComBSTR("IsConnected"), &bShow);
				if (FAILED(hr))
					bShow = VARIANT_FALSE;
			}
			else
			{
				hr = pThing->get_BOOL(CComBSTR("IsAvatar"), &bShow);
				if (FAILED(hr))
					bShow = VARIANT_FALSE;
			}

			if (bShow == VARIANT_TRUE)
				hr = AddThing(pThing);
		}
		SAFERELEASE(pThing);
	}
	
CLEAN_UP:
	VERIFY(SortItems(CompareUsers, DWORD(m_pUserCtrl)));
	SAFERELEASE(pContents);
	SAFERELEASE(pContainer);
	SAFERELEASE(pUser);
	SAFERELEASE(pThing);

	Invalidate();
	return hr;
}



//******************************************************
// ClearList
// Clears the User List...

HRESULT CUserListView::ClearList(void)
{
	//while (GetCount())
	//	DeleteString(0);
	DeleteAllItems();
	m_userMap.RemoveAll();
	int count = m_pImageList->GetImageCount();
	//Remove all images except for the first
	for(int i=2;i<count;i++)
	{
		m_pImageList->Remove(2);
	}
	return S_OK;
}


//******************************************************
// OnCreate
// Set the default Item Height, etc.

int CUserListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//SetItemHeight(0, 28);
	if (!m_pImageList)
	{
		CBitmap cbmDefault,cbmIgnore;
		cbmDefault.LoadBitmap(IDB_DEFAULTAVATAR);
		cbmIgnore.LoadBitmap(IDB_IGNORE);

		m_imagedx = 24;
		m_imagedy = 24;		
		m_pImageList = new CImageList;
		m_pImageList->Create(m_imagedx, m_imagedy, ILC_COLOR8, 1024, 1024);

		m_pImageList->Add(&cbmDefault,&cbmDefault);
		m_pImageList->Add(&cbmIgnore,&cbmIgnore);

	}

	SetImageList(m_pImageList, LVSIL_SMALL);

	return 0;
}

//******************************************************
// OnContextMenu
// Bring up the context menu...

void CUserListView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CPoint	pt = point;
	UINT		Flags	= 0;


	pWnd->ScreenToClient( &pt );
	int Item = HitTest( pt, &Flags );

	if ( Item < 0 ) 
	{
		// TODO: Add your message handler code here
		CMenu menu;
		menu.CreatePopupMenu();
		// build the menu
		menu.AppendMenu( MF_STRING, ID_USERLIST_REDRAW, "Refresh List");

		VERIFY(menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this));
		menu.DestroyMenu();
	}
	else
	{
		m_bSelected = TRUE;
		SetItemState(Item,LVIS_SELECTED, LVIS_SELECTED);
		SetItemState(Item,LVIS_DROPHILITED,LVIS_DROPHILITED);

		HRESULT			hr		= S_OK;
		IThing			*pThing			= NULL;
		long			id				=(long ) GetItemData( Item );


		hr = m_pUserCtrl->m_pWorld->get_ObjectByID( id, &pThing );
		if ( FAILED(hr))
			return;

		ASSERT ( pThing != NULL);

		// get the menu
		CComVariant varResult;
		hr = pThing->InvokeMethodExt(CComBSTR("GetMenuList"), NULL, &varResult);
		if (SUCCEEDED(hr))
		{
			CPropertyListPtr ListPtr = varResult;
			if (ListPtr != NULL )
			{
				// build the menu
				UINT wParam = WM_USER;
				hr = m_pUserCtrl->m_Menu.CreatePopupFromPropList(ListPtr, &wParam);
				if (SUCCEEDED(hr))
				{
					VERIFY(m_pUserCtrl->m_Menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, m_pUserCtrl));
				}
				m_pUserCtrl->m_Menu.DestroyMenu();
			}
		}

		m_bSelected = FALSE;
		SetItemState(Item, ~LVIS_SELECTED, LVIS_SELECTED);
		SetItemState(Item,~LVIS_DROPHILITED,LVIS_DROPHILITED);

		// clean up
		SAFERELEASE(pThing);
	}
}

void CUserListView::OnRedrawList() 
{
	RefreshList();
	Invalidate(TRUE);
}

void CUserListView::OnItemchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if((pNMListView->uNewState & LVIS_SELECTED) && !m_bSelected)
		SetItemState(pNMListView->iItem,0, LVIS_SELECTED | LVIS_FOCUSED);
	
	*pResult = 0;
}

void CUserListView::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if((pNMListView->uNewState & LVIS_SELECTED) && !m_bSelected)
		SetItemState(pNMListView->iItem,0, LVIS_SELECTED | LVIS_FOCUSED);
	
	*pResult = 0;
}

