// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// SelfCtl.cpp : Implementation of the CSelfCtrl ActiveX Control class.

#include "stdafx.h"
#include "cutils.h"
#include "uictrl.h"
#include "SelfCtl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSelfCtrl, CVWViewCtrl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSelfCtrl, CVWViewCtrl)
	//{{AFX_MSG_MAP(CSelfCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(WM_USER, (WM_USER+200), OnCommandRange)
	ON_UPDATE_COMMAND_UI_RANGE(WM_USER, (WM_USER+200), OnCommandRangeUI)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CSelfCtrl, CVWViewCtrl)
	//{{AFX_DISPATCH_MAP(CSelfCtrl)
	DISP_PROPERTY_EX(CSelfCtrl, "VWClient", GetVWClient, SetVWClient, VT_DISPATCH)
	DISP_STOCKPROP_BACKCOLOR()
	DISP_STOCKPROP_FONT()
	DISP_STOCKPROP_FORECOLOR()
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CSelfCtrl, CVWViewCtrl)
	//{{AFX_EVENT_MAP(CSelfCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CSelfCtrl, 2)
	PROPPAGEID( CLSID_CFontPropPage )
    PROPPAGEID( CLSID_CColorPropPage )
END_PROPPAGEIDS(CSelfCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSelfCtrl, "VWCLIENTUI.SelfCtrl.1",
	0xdc9ca0c8, 0x336f, 0x11d1, 0xbb, 0x22, 0, 0xc0, 0x4f, 0xc2, 0xca, 0xb1)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CSelfCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DSelf =
		{ 0xdc9ca0c6, 0x336f, 0x11d1, { 0xbb, 0x22, 0, 0xc0, 0x4f, 0xc2, 0xca, 0xb1 } };
const IID BASED_CODE IID_DSelfEvents =
		{ 0xdc9ca0c7, 0x336f, 0x11d1, { 0xbb, 0x22, 0, 0xc0, 0x4f, 0xc2, 0xca, 0xb1 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwSelfOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CSelfCtrl, IDS_SELF, _dwSelfOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CSelfCtrl::CSelfCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CSelfCtrl
BOOL CSelfCtrl::CSelfCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_SELF,
			IDB_SELF,
			afxRegApartmentThreading,
			_dwSelfOleMisc,
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
// CSelfCtrl::CSelfCtrl - Constructor

CSelfCtrl::CSelfCtrl()
{
	InitializeIIDs(&IID_DSelf, &IID_DSelfEvents);

	m_pImageList = NULL;
}


/////////////////////////////////////////////////////////////////////////////
// CSelfCtrl::~CSelfCtrl - Destructor

CSelfCtrl::~CSelfCtrl()
{
	if(m_pImageList)
		delete m_pImageList;
}


/////////////////////////////////////////////////////////////////////////////
// CSelfCtrl::OnDraw - Drawing function

void CSelfCtrl::OnDraw(
			CDC* pDC, const CRect& rcBounds, const CRect& rcInvalid)
{
	// This is not spec'ed.
/*	pDC->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	if (m_pWorld)
	{
		CComBSTR	bstrName;
		IThing		*pUser	= NULL;
		CString		strName = "<ERROR>";
		CFont		font, *pOldFont;
		HRESULT		hr;

		// Get the User Name
		hr = m_pWorld->get_User(&pUser);
		if (SUCCEEDED(hr) && pUser)
		{
			hr = pUser->get_Name(&bstrName.m_str);
			if (SUCCEEDED(hr))
				strName = bstrName;
		}

		// Draw the User Name CreateInstance
		font.CreatePointFont(180, "Arial");
		pOldFont = pDC->SelectObject(&font);
		pDC->TextOut(10,5,strName);

		pDC->SelectObject(pOldFont);
	}
	else
	{
		// no world - just erase the view rect...
		pDC->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	}*/
}


/////////////////////////////////////////////////////////////////////////////
// CSelfCtrl::DoPropExchange - Persistence support

void CSelfCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	CVWViewCtrl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CSelfCtrl::OnResetState - Reset control to default state

void CSelfCtrl::OnResetState()
{
	CVWViewCtrl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


//******************************************************
// OnCreate
// Creates the inventory view, 30 pixels down

const	short	kTopMargin = 35;

int CSelfCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	RECT rect;
	GetClientRect( &rect );

	if (CVWViewCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// now, create the inventory list view
	//rect.top += kTopMargin;
	if (InvList.Create( LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE| LVS_REPORT | LVS_NOCOLUMNHEADER | LVS_AUTOARRANGE, rect, this, 254) == -1)
	{
		return -1;
	}

	InvList.InsertColumn(0, "Object", LVCFMT_LEFT,150,0);

	CFontHolder& font = InternalGetFont();
	HFONT hFont = font.GetFontHandle();
	if ( hFont )
	{
		InvList.SetFont(CFont::FromHandle(hFont ), TRUE); 
	}

	COLORREF cref = TranslateColor(GetBackColor());
	InvList.SetBkColor( cref );
	InvList.SetTextBkColor( cref );
	InvList.SetTextColor(TranslateColor(GetForeColor()));


	if (!m_pImageList)
	{
		m_pImageList = new CImageList;
		m_pImageList->Create(IDB_TREEICON, 16, 0, 0x00FFFFFF);
	}

	InvList.m_pParent = this;

	InvList.SetImageList(m_pImageList,LVSIL_SMALL);

	return 0;
}

//******************************************************
// OnSize
// Ensure that we keep the inventory list being resized properly also...

void CSelfCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CVWViewCtrl::OnSize(nType, cx, cy);
		
	InvList.SetWindowPos(&wndTop,0,0 ,cx-10,cy,SWP_NOZORDER );
}

/////////////////////////////////////////////////////////////////////////////
// CSelfCtrl message handlers

//******************************************************
// HandleOnConnect
// Update the inventory list

HRESULT CSelfCtrl::HandleOnConnect(IDispatch* pWorld)
{
	CVWViewCtrl::HandleOnConnect(pWorld);

	InvList.Refresh();

	return S_OK;
}

HRESULT CSelfCtrl::HandleOnDisconnect(IDispatch* pWorld)
{
	CVWViewCtrl::HandleOnDisconnect(pWorld);

	InvList.DeleteAllItems();

	return S_OK;
}

//******************************************************
// HandleOnWear
// Change icon to reflect the worn state

HRESULT CSelfCtrl::HandleOnWear(IThing* pWho, IThing* pWhat)
{
//	CVWViewCtrl::HandleOnWear(pWho, pWhat);
	InvList.Refresh();
	return S_OK;
}


//******************************************************
// HandleOnUnWear
// Change icon to reflect the worn state

HRESULT CSelfCtrl::HandleOnUnWear(IThing* pWho, IThing* pWhat)
{
//	CVWViewCtrl::HandleOnUnWear(pWho, pWhat);
	InvList.Refresh();
	return S_OK;
}


//******************************************************
// HandleOnEnterInventory
// Add it to the inventory list

HRESULT CSelfCtrl::HandleOnEnterInventory(IThing* pWhere, IThing* pWhat)
{
//	CVWViewCtrl::HandleOnEnterInventory(pWhere,pWhat);

	IThing	*pUser	= NULL;
	HRESULT	hr;

	hr = m_pWorld->get_User(&pUser);

	if (SUCCEEDED(hr) && pUser)
	{
		if (pWhere == pUser)
		{
			InvList.AddItem(pWhat);
		}
	}

	SAFERELEASE(pUser);
	return S_OK;
}

//******************************************************
// HandleOnLeaveInventory
// Remove it from the inventory list

HRESULT CSelfCtrl::HandleOnLeaveInventory(IThing* pWhere, IThing* pWhat)
{
//	CVWViewCtrl::HandleOnLeaveInventory(pWhere,pWhat);

	IThing	*pUser	= NULL;
	HRESULT	hr;

	hr = m_pWorld->get_User(&pUser);

	if (SUCCEEDED(hr) && pUser)
	{
		if (pWhere == pUser)
		{
			InvList.RemoveItem(pWhat);
		}
	}

	SAFERELEASE(pUser);
	return S_OK;
}


//******************************************************
// HandleOnUIEvent
// Update if an object's name changes
#include <vwevents.h>

HRESULT CSelfCtrl::HandleOnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient)
{
	CString	strEventName = bstrEventName;
	
	if ((!strEventName.CompareNoCase(VW_NAMECHANGED_EVENT_STR))||
		(!strEventName.CompareNoCase(VW_UIELEMENTCHANGED_EVENT_STR)))
	{
		// redraw the list
		// TO DO: Make sure the item is in the list first
		InvList.Refresh();
	}
	else
		CVWViewCtrl::HandleOnUIEvent(pthing, bstrEventName, varArg, bFromClient);

	return S_OK;
}

//******************************************************
// HandleOnWield
// Update the icon

// we arent actually going to handle Wield.  We are going to handle wore instead.

/*HRESULT CSelfCtrl::HandleOnWield(IThing* pWho, IThing* pWhat)
{
	IThing	*pUser	= NULL;
	HRESULT	hr;

	hr = m_pWorld->get_User(&pUser);

	if (SUCCEEDED(hr) && pUser)
	{
		if (pWho == pUser)
		{
			InvList.RefreshItem(pWhat);
		}
	}

	SAFERELEASE(pUser);
	return S_OK;
}*/


//******************************************************
// HandleOnUnWield
// Update the icon

/*HRESULT CSelfCtrl::HandleOnUnWield(IThing* pWho, IThing* pWhat)
{
	IThing	*pUser	= NULL;
	HRESULT	hr;

	hr = m_pWorld->get_User(&pUser);

	if (SUCCEEDED(hr) && pUser)
	{
		if (pWho == pUser)
		{
			InvList.RefreshItem(pWhat);
		}
	}

	SAFERELEASE(pUser);
	return S_OK;
}*/


//******************************************************
// HandleOnInvokeMethod
// We need to check for wear/unwear

/*HRESULT CSelfCtrl::HandleOnInvokeMethod( WORD wFlags, IThing* pthing, BSTR bstrName,DISPPARAMS* pdispparams)
{
	// TO DO:: check for wear/unwear events and refresh the item
	return S_OK; 
}*/

void CSelfCtrl::OnCommandRange(UINT nID)
{
	HRESULT hr = S_OK;
	m_Menu.OnCommandRange(nID, &hr);
		
}


void CSelfCtrl::OnCommandRangeUI(CCmdUI* pCmdUI)
{

	HRESULT hr;
	m_Menu.OnCommandRangeUI( pCmdUI, &hr );
}

LPDISPATCH CSelfCtrl::GetVWClient() 
{
	return CVWViewCtrl::GetVWClient();
}

void CSelfCtrl::SetVWClient(LPDISPATCH newValue) 
{
	CVWViewCtrl::SetVWClient(newValue);
	SetModifiedFlag();
}


void CSelfCtrl::OnBackColorChanged() 
{
	CVWViewCtrl::OnBackColorChanged();
	OLE_COLOR nNewValue = GetBackColor();
	COLORREF cref = TranslateColor(nNewValue);
	InvList.SetBkColor( cref );
	InvList.SetTextBkColor( cref );
	InvList.InvalidateRect(NULL);
}

void CSelfCtrl::OnForeColorChanged() 
{
	CVWViewCtrl::OnForeColorChanged();
	OLE_COLOR nNewValue = GetForeColor();
	COLORREF cref = TranslateColor(nNewValue);
	InvList.SetTextColor( cref );
	InvList.InvalidateRect(NULL);
}

void CSelfCtrl::OnAmbientPropertyChange(DISPID dispid)
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

void CSelfCtrl::OnFontChanged()
{
	CVWViewCtrl::OnFontChanged();
	
	CFontHolder& font = InternalGetFont();

	HFONT hFont = font.GetFontHandle();

	if ( hFont && InvList.m_hWnd )
	{
		InvList.SetFont(CFont::FromHandle(hFont ), TRUE); 
	}
}

/////////////////////////////////////////////////////////////////////////////
// CInvList

IMPLEMENT_DYNCREATE(CInvList, CListView)

CInvList::CInvList()
{
	m_pParent = NULL;
	m_bSelected = FALSE;
}

CInvList::~CInvList()
{
	m_pParent = NULL;
}


BEGIN_MESSAGE_MAP(CInvList, CListCtrl)
	//{{AFX_MSG_MAP(CInvList)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGING, OnItemchanging)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInvList drawing

void CInvList::OnDraw(CDC* pDC)
{
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CInvList diagnostics

#ifdef _DEBUG
void CInvList::AssertValid() const
{
	CListCtrl::AssertValid();
}

void CInvList::Dump(CDumpContext& dc) const
{
	CListCtrl::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInvList message handlers

int CALLBACK CompareInv(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	IThing* pParam1Thing = NULL;
	IThing* pParam2Thing = NULL;
	CComBSTR szParam1Name;
	CComBSTR szParam2Name;
	CString strParam1Name;
	CString strParam2Name;
	int nResult = 0;

	CSelfCtrl* pParent = (CSelfCtrl*)lParamSort;

	HRESULT hr = S_OK;

	if(pParent)
	{
		if(pParent->m_pWorld)
		{

			hr = pParent->m_pWorld->get_ObjectByID(lParam1, &pParam1Thing);
			if((FAILED(hr)) || (!pParam1Thing)) goto EXIT;

			hr = pParent->m_pWorld->get_ObjectByID(lParam2, &pParam2Thing);
			if((FAILED(hr)) || (!pParam2Thing)) goto EXIT;

			hr = pParam1Thing->get_Name(&szParam1Name.m_str);
			if(FAILED(hr)) goto EXIT;

			strParam1Name = szParam1Name;

			hr = pParam2Thing->get_Name(&szParam2Name.m_str);
			if(FAILED(hr)) goto EXIT;

			strParam2Name = szParam2Name;

			// now compare these suckers
			nResult = strParam1Name.CompareNoCase(strParam2Name);
		}
	}

EXIT:
	SAFERELEASE(pParam1Thing);
	SAFERELEASE(pParam2Thing);
	return nResult;
}

//******************************************************
// InsertThing
// Inserts an IThing into the list view

void CInvList::InsertThing(IThing * pNewThing)
{
	VARIANT_BOOL bAccessory = VARIANT_FALSE;
	VARIANT_BOOL bWorn = VARIANT_FALSE;

	if (pNewThing)
	{
		CComBSTR	bstrName, bstrText;
		HRESULT		hr;
		long		lImage = 0;

		hr = pNewThing->get_Name( &bstrName.m_str );
		if (FAILED(hr))
			return;

		// if its worn or if its an accessory thats not worn, we need special icons
		hr = pNewThing->get_BOOL(CComBSTR("IsAccessory"), &bAccessory);
		// we really dont care if we fail here, so we'll just continue

		hr = pNewThing->get_BOOL(CComBSTR("IsWorn"), &bWorn);
		// neither here

		if(bWorn == VARIANT_TRUE)
			lImage = IT_WORN;
		else if(bAccessory == VARIANT_TRUE)
			lImage = IT_ACCESSORY;
		else
			// set the correct icon
			lImage = AssignIcon(pNewThing);

		//int item = ctl.InsertItem(0,CString(bstrName.m_str));
		int item = InsertItem(0, CString(bstrName.m_str), lImage);

		if ( item >= 0 )
		{
			long id = 0;
			
			// set ID
			hr = pNewThing->get_ID( &id );
			if (FAILED(hr))
				return;
			SetItemData( item,(unsigned long)id );
			
			// description
			//hr = pNewThing->get_Description( &bstrText.m_str );
			//if (FAILED(hr))
				//return;
			//ctl.SetItemText(item, 1, CString(bstrText.m_str));
		}
		VERIFY(SortItems(CompareInv, (DWORD)m_pParent));
	}
}

//******************************************************
// Refresh
// Refreshes the list view to show all items in the inventory

void CInvList::Refresh()
{
	if (!m_hWnd)
		return;

	DeleteAllItems();

	if (!m_pParent)
		return;
	
	IWorld				*pWorld		= m_pParent->m_pWorld;
	HRESULT				hr			= S_OK;
	IPropertyList		*pContents	= NULL;
	IThing				*pUser		= NULL;;
	IThing				*pThing		= NULL;
	long				lItems;
	int					i;

	hr = pWorld->get_User(&pUser);
	if (FAILED(hr) || (!pUser))
		goto CLEAN_UP;

	hr = pUser->get_Contents(&pContents);
	if (FAILED(hr) || (!pContents))
		goto CLEAN_UP;

	hr = pContents->get_Count( &lItems );
	if (FAILED(hr))
		goto CLEAN_UP;

	for ( i = 0; i < lItems; i++)
	{
		VARIANT_BOOL	bShow;

		hr = pContents->get_Thing(i, &pThing  );
		if (FAILED(hr))
			goto CLEAN_UP;
		
		if (pThing)
		{
			// REVIEW:  If we actually want to show other items, they can go here...

			// Filter for visible items only...
			hr = pThing->get_BOOL(CComBSTR(L"IsUIVisible"), &bShow);
			if (FAILED(hr))
				goto CLEAN_UP;
			
			// Insert the item if we are actually showing it
			if (bShow == VARIANT_TRUE)
				InsertThing(pThing);
		}
		SAFERELEASE(pThing);
	}
	VERIFY(SortItems(CompareInv, (DWORD)m_pParent));
CLEAN_UP:
	SAFERELEASE(pContents);
	SAFERELEASE(pUser);
	SAFERELEASE(pThing);

	Invalidate();
}

//******************************************************
// AddItem
// Inserts a new item into the list view

void CInvList::AddItem(IThing * pThing)
{
	/*
	HRESULT			hr;
	VARIANT_BOOL	bShow;
	
	hr = pThing->get_BOOL(CComBSTR(L"IsUIVisible"), &bShow);
	if (SUCCEEDED(hr))
	{	
		// Insert the item if we are actually showing it
		if (bShow == VARIANT_TRUE)
			InsertThing(pThing);
	}
	*/
	// dv 12/01/97 - For now, we are showing EVERYTHING in the inventory,
	// whether it is ShownInContents or not.
	InsertThing(pThing);
}

//******************************************************
// RemoveItem
// Removes an existing item from the list view

void CInvList::RemoveItem(IThing * pThing)
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
				DeleteItem(iItem);

		}
	}
}


//******************************************************
// RefreshItem
// Call this when properties of the item change

void CInvList::RefreshItem(IThing * pThing)
{
	// TO DO - refresh name, description, etc..
}

//******************************************************
// OnContextMenu
// Bring up the current item..

void CInvList::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	HRESULT		hr		= S_OK;
	UINT		Flags	= 0;
	CPoint		pt		= point;

	pWnd->ScreenToClient( &pt );

	int Item = HitTest( pt, &Flags );

	if ( Item < 0 ) 
		return;

	m_bSelected = TRUE;
	SetItemState(Item,LVIS_SELECTED, LVIS_SELECTED);
	SetItemState(Item,LVIS_DROPHILITED,LVIS_DROPHILITED);

	IThing			*pThing			= NULL;
	long			id				=(long ) GetItemData( Item );


	hr = m_pParent->m_pWorld->get_ObjectByID( id, &pThing );
	if ( FAILED(hr))
		return;

	ASSERT ( pThing != NULL);

	//Review
	// Select this item to avoid any confusion!!!
	//if ( m_SelectedID != id )
		//SetSelected( pThing );

	// get the menu
	CComVariant varResult;
	hr = pThing->InvokeMethodExt(CComBSTR("GetMenuList"), NULL, &varResult);
	if (SUCCEEDED(hr))
	{
		CPropertyListPtr ListPtr = varResult;
		if (ListPtr != NULL )
		{
			UINT wParam = WM_USER;
			hr = m_pParent->m_Menu.CreatePopupFromPropList(ListPtr, &wParam);
			if (SUCCEEDED(hr))
			{
				VERIFY(m_pParent->m_Menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, m_pParent));
			}
			m_pParent->m_Menu.DestroyMenu();
		}
	}

	m_bSelected = FALSE;
	SetItemState(Item, ~LVIS_SELECTED, LVIS_SELECTED);
	SetItemState(Item,~LVIS_DROPHILITED,LVIS_DROPHILITED);

	// clean up
	SAFERELEASE(pThing);
}


long CInvList::AssignIcon(IThing* pThing)
{					
	HRESULT hr = S_OK;
	CComBSTR bstrType;
	CString strType;
	long lIcon = IT_UNKNOWN;
	IThing* pExemplarThing = NULL;
	IThing* pLoopThing = NULL;
	pLoopThing = pThing;
	SAFEADDREF(pLoopThing);

	if (!pThing)
		return E_POINTER;

	hr = pLoopThing->get_Type(&bstrType.m_str);
	strType = bstrType;

	while(lIcon == IT_UNKNOWN)
	{
		if(!strType.CompareNoCase("world"))
		{
			lIcon =  IT_WORLD;
			break;
		}
		else if(!strType.CompareNoCase("room"))
		{
			lIcon = IT_ROOM;
			break;
		}
		else if(!strType.CompareNoCase("avatar"))
		{
			lIcon = IT_AVATAR;
			break;
		}
		else if(!strType.CompareNoCase("artifact"))
		{
			lIcon =  IT_ARTIFACT;
			break;
		}
		else if(!strType.CompareNoCase("portal"))
		{
			lIcon =  IT_PORTAL;
			break;
		}
		else if(!strType.CompareNoCase("thing"))
		{
			lIcon = IT_THING;
			break;
		}
		else lIcon =  IT_UNKNOWN;

		bstrType.Empty();
		hr = pLoopThing->get_Exemplar(&pExemplarThing);
		if(pExemplarThing)
		{
			hr = pExemplarThing->get_Type(&bstrType.m_str);
			strType = bstrType;
		}
		SAFERELEASE(pLoopThing);
		pLoopThing = pExemplarThing;
		SAFEADDREF(pLoopThing);
		SAFERELEASE(pExemplarThing);
		if (!pLoopThing)
			break;
	}

	bstrType.Empty();
	SAFERELEASE(pLoopThing);
	SAFERELEASE(pExemplarThing);
	return lIcon;
}

void CInvList::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;


	if((pNMListView->uNewState & LVIS_SELECTED) && !m_bSelected)
		SetItemState(pNMListView->iItem,0, LVIS_SELECTED | LVIS_FOCUSED);
	
	*pResult = 0;
}

void CInvList::OnItemchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;


	if((pNMListView->uNewState & LVIS_SELECTED) && !m_bSelected)
		SetItemState(pNMListView->iItem,0, LVIS_SELECTED | LVIS_FOCUSED);
	
	*pResult = 0;
}



