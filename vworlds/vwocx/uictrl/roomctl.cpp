// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// RoomCtl.cpp : Implementation of the CRoomCtrl ActiveX Control class.

#include "stdafx.h"
#include "cutils.h"
#include "uictrl.h"
#include "RoomCtl.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CRoomCtrl, CVWViewCtrl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CRoomCtrl, CVWViewCtrl)
	//{{AFX_MSG_MAP(CRoomCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(WM_USER, (WM_USER+200), OnCommandRange)
	ON_UPDATE_COMMAND_UI_RANGE(WM_USER, (WM_USER+200), OnCommandRangeUI)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CRoomCtrl, CVWViewCtrl)
	//{{AFX_DISPATCH_MAP(CRoomCtrl)
	DISP_PROPERTY_EX(CRoomCtrl, "VWClient", GetVWClient, SetVWClient, VT_DISPATCH)
	DISP_STOCKPROP_BACKCOLOR()
	DISP_STOCKPROP_FONT()
	DISP_STOCKPROP_FORECOLOR()
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CRoomCtrl, CVWViewCtrl)
	//{{AFX_EVENT_MAP(CRoomCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CRoomCtrl, 2)
	PROPPAGEID( CLSID_CFontPropPage )
    PROPPAGEID( CLSID_CColorPropPage )
END_PROPPAGEIDS(CRoomCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CRoomCtrl, "VWCLIENTUI.RoomCtrl.1",
	0xdc9ca0cc, 0x336f, 0x11d1, 0xbb, 0x22, 0, 0xc0, 0x4f, 0xc2, 0xca, 0xb1)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CRoomCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DRoom =
		{ 0xdc9ca0ca, 0x336f, 0x11d1, { 0xbb, 0x22, 0, 0xc0, 0x4f, 0xc2, 0xca, 0xb1 } };
const IID BASED_CODE IID_DRoomEvents =
		{ 0xdc9ca0cb, 0x336f, 0x11d1, { 0xbb, 0x22, 0, 0xc0, 0x4f, 0xc2, 0xca, 0xb1 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwRoomOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CRoomCtrl, IDS_ROOM, _dwRoomOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CRoomCtrl::CRoomCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CRoomCtrl
BOOL CRoomCtrl::CRoomCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_ROOM,
			IDB_ROOM,
			afxRegApartmentThreading,
			_dwRoomOleMisc,
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
// CRoomCtrl::CRoomCtrl - Constructor

CRoomCtrl::CRoomCtrl()
{
	InitializeIIDs(&IID_DRoom, &IID_DRoomEvents);

	m_pImageList = NULL;
}


/////////////////////////////////////////////////////////////////////////////
// CRoomCtrl::~CRoomCtrl - Destructor

CRoomCtrl::~CRoomCtrl()
{
	if(m_pImageList)
		delete m_pImageList;
}



/////////////////////////////////////////////////////////////////////////////
// CRoomCtrl::OnDraw - Drawing function

void CRoomCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
}


/////////////////////////////////////////////////////////////////////////////
// CRoomCtrl::DoPropExchange - Persistence support

void CRoomCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	CVWViewCtrl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CRoomCtrl::OnResetState - Reset control to default state

void CRoomCtrl::OnResetState()
{
	CVWViewCtrl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



//******************************************************
// OnCreate
// Create the room list and any other controls

int CRoomCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	RECT rect;
	GetClientRect( &rect );

	if (CVWViewCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// now, create the inventory list view
	if (RoomList.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_NOCOLUMNHEADER | LVS_AUTOARRANGE, rect, this, 254) == -1)
	{
		return -1;
	}

	RoomList.InsertColumn(0, "Object", LVCFMT_LEFT,150,0);
	CFontHolder& font = InternalGetFont();
	HFONT hFont = font.GetFontHandle();
	if ( hFont )
	{
		RoomList.SetFont(CFont::FromHandle(hFont ), TRUE); 
	}

	COLORREF cref = TranslateColor(GetBackColor());
	RoomList.SetBkColor( cref );
	RoomList.SetTextBkColor( cref );
	RoomList.SetTextColor(TranslateColor(GetForeColor()));


	if (!m_pImageList)
	{
		m_pImageList = new CImageList;
		m_pImageList->Create(IDB_TREEICON, 16, 0, 0x00FFFFFF);
	}

	RoomList.m_pParent = this;

	RoomList.SetImageList(m_pImageList,LVSIL_SMALL);


	return 0;
}

//******************************************************
// OnSize
// Layout the controls

void CRoomCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CVWViewCtrl::OnSize(nType, cx, cy);
		
	RoomList.SetWindowPos(&wndTop,0,0,cx-10,cy,SWP_NOZORDER );
}


//******************************************************
// HandleOnConnect
// Update the inventory list

HRESULT CRoomCtrl::HandleOnConnect(IDispatch* pWorld)
{
	CVWViewCtrl::HandleOnConnect(pWorld);

	RoomList.Refresh();

	return S_OK;
}

HRESULT CRoomCtrl::HandleOnDisconnect(IDispatch* pWorld)
{
	CVWViewCtrl::HandleOnDisconnect(pWorld);

	RoomList.DeleteAllItems();

	return S_OK;
}

//******************************************************
// HandleOnEnterInventory
// Add it to the inventory list

HRESULT CRoomCtrl::HandleOnEnterRoom(IThing* pWhere, IThing* pWhat)
{
//	CVWViewCtrl::HandleOnEnterRoom(pWhere,pWhat);

	HRESULT	hr;
	IThing	*pUser = NULL;

	hr = m_pWorld->get_User(&pUser);
	if (FAILED(hr))
		goto CLEAN_UP;
	
	if (pWhat == pUser)
		RoomList.Refresh();
	else if (pWhat)
	{
		RoomList.AddItem(pWhat);
	}

CLEAN_UP:
	SAFERELEASE(pUser);

	return S_OK;
}

//******************************************************
// HandleOnUIEvent
// Update if an object's name changes

#include <vwevents.h>

HRESULT CRoomCtrl::HandleOnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient)
{
	CString	strEventName = bstrEventName;
	
	if (!strEventName.CompareNoCase(VW_NAMECHANGED_EVENT_STR))
	{
		// redraw the list
		// TO DO: Make sure the item is in the list first
		RoomList.Refresh();
	}
	else
		CVWViewCtrl::HandleOnUIEvent(pthing, bstrEventName, varArg, bFromClient);

	return S_OK;
}


//******************************************************
// HandleOnLeaveInventory
// Remove it from the inventory list

HRESULT CRoomCtrl::HandleOnLeaveRoom(IThing* pWhere, IThing* pWhat)
{
//	CVWViewCtrl::HandleOnLeaveRoom(pWhere,pWhat);

	RoomList.RemoveItem(pWhat);

	return S_OK;
}

void CRoomCtrl::OnCommandRange(UINT nID)
{
	HRESULT hr = S_OK;
	m_Menu.OnCommandRange(nID, &hr);
		
}


void CRoomCtrl::OnCommandRangeUI(CCmdUI* pCmdUI)
{

	HRESULT hr;
	m_Menu.OnCommandRangeUI( pCmdUI, &hr );
}

LPDISPATCH CRoomCtrl::GetVWClient() 
{
	return CVWViewCtrl::GetVWClient();
}

void CRoomCtrl::SetVWClient(LPDISPATCH newValue) 
{
	CVWViewCtrl::SetVWClient(newValue);
	SetModifiedFlag();
}

void CRoomCtrl::OnBackColorChanged() 
{
	CVWViewCtrl::OnBackColorChanged();
	OLE_COLOR nNewValue = GetBackColor();
	COLORREF cref = TranslateColor(nNewValue);
	RoomList.SetBkColor( cref );
	RoomList.SetTextBkColor( cref );
	RoomList.InvalidateRect(NULL);
}

void CRoomCtrl::OnForeColorChanged() 
{
	CVWViewCtrl::OnForeColorChanged();
	OLE_COLOR nNewValue = GetForeColor();
	COLORREF cref = TranslateColor(nNewValue);
	RoomList.SetTextColor( cref );
	RoomList.InvalidateRect(NULL);
}

void CRoomCtrl::OnAmbientPropertyChange(DISPID dispid)
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

void CRoomCtrl::OnFontChanged()
{
	CVWViewCtrl::OnFontChanged();

	
	CFontHolder& font = InternalGetFont();

	HFONT hFont = font.GetFontHandle();

	if ( hFont && RoomList.m_hWnd )
	{
		RoomList.SetFont(CFont::FromHandle(hFont ), TRUE); 
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRoomCtrl message handlers
/////////////////////////////////////////////////////////////////////////////
// CRoomList

IMPLEMENT_DYNCREATE(CRoomList, CListView)

CRoomList::CRoomList()
{
	m_pParent = NULL;
	m_bSelected = FALSE;
}

CRoomList::~CRoomList()
{
	m_pParent = NULL;
}


BEGIN_MESSAGE_MAP(CRoomList, CListCtrl)
	//{{AFX_MSG_MAP(CRoomList)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGING, OnItemchanging)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRoomList drawing

void CRoomList::OnDraw(CDC* pDC)
{
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CRoomList diagnostics

#ifdef _DEBUG
void CRoomList::AssertValid() const
{
	CListCtrl::AssertValid();
}

void CRoomList::Dump(CDumpContext& dc) const
{
	CListCtrl::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRoomList message handlers

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	IThing* pParam1Thing = NULL;
	IThing* pParam2Thing = NULL;
	VARIANT_BOOL bParam1IsPortal = NULL;
	VARIANT_BOOL bParam2IsPortal = NULL;
	CComBSTR szParam1Name;
	CComBSTR szParam2Name;
	CString strParam1Name;
	CString strParam2Name;
	int nResult = 0;

	CRoomCtrl* pParent = (CRoomCtrl*)lParamSort;

	HRESULT hr = S_OK;

	if(pParent)
	{
		if(pParent->m_pWorld)
		{

			hr = pParent->m_pWorld->get_ObjectByID(lParam1, &pParam1Thing);
			if((FAILED(hr)) || (!pParam1Thing)) goto EXIT;

			hr = pParent->m_pWorld->get_ObjectByID(lParam2, &pParam2Thing);
			if((FAILED(hr)) || (!pParam2Thing)) goto EXIT;

			hr = pParam1Thing->IsOfType(CComBSTR("Portal"), &bParam1IsPortal);
			if(FAILED(hr)) goto EXIT;

			hr = pParam2Thing->IsOfType(CComBSTR("Portal"), &bParam2IsPortal);
			if(FAILED(hr)) goto EXIT;

			// first check the exclusive case
			if((bParam1IsPortal == VARIANT_TRUE) && (bParam2IsPortal == VARIANT_FALSE))
			{
				nResult = -1;
				goto EXIT;
			}

			if((bParam1IsPortal == VARIANT_FALSE) && (bParam2IsPortal == VARIANT_TRUE))
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

void CRoomList::InsertThing(IThing * pNewThing)
{
	if (pNewThing)
	{
		CComBSTR	bstrName, bstrText;
		HRESULT		hr;
		long lImage = 0;
		VARIANT_BOOL bAvatar = VARIANT_FALSE;

		// per spec, dont show avatars in roomlist
		hr = pNewThing->IsOfType(CComBSTR("Avatar"), &bAvatar);
		if(FAILED(hr)) return;

		if(bAvatar == VARIANT_TRUE)
			return;
		

		hr = pNewThing->get_Name( &bstrName.m_str );
		if (FAILED(hr))
			return;
		
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
			//	return;
			//ctl.SetItemText(item, 1, CString(bstrText.m_str));
		}

		VERIFY(SortItems(CompareFunc, (DWORD)m_pParent));
	}
}


//******************************************************
// Refresh
// Refreshes the list view to show all items in the inventory

void CRoomList::Refresh()
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
	IThing				*pContainer	= NULL;
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
		VARIANT_BOOL	bShow;

		hr = pContents->get_Thing(i, &pThing  );
		if (FAILED(hr))
			goto CLEAN_UP;
		
		if (pThing)
		{
			// REVIEW:  If we want to filter other items, they can go here...

			// Filter for visible items only...
			hr = pThing->get_BOOL(CComBSTR("IsUIVisible"), &bShow);
			if (FAILED(hr))
				goto CLEAN_UP;
			
			// Insert the item if we are actually showing it
			if (bShow == VARIANT_TRUE)
				InsertThing(pThing);
		}
		SAFERELEASE(pThing);
	}

	// lets sort here
	VERIFY(SortItems(CompareFunc, DWORD(m_pParent)));
	
CLEAN_UP:
	SAFERELEASE(pContents);
	SAFERELEASE(pContainer);
	SAFERELEASE(pUser);
	SAFERELEASE(pThing);

	Invalidate();
}


//******************************************************
// AddItem
// Inserts a new item into the list view

void CRoomList::AddItem(IThing * pThing)
{
	IWorld			*pWorld		= m_pParent->m_pWorld;
	IThing			*pMyCont	= NULL;
	IThing			*pUser		= NULL;
	IThing			*pThingCont	= NULL;
	HRESULT			hr;
	VARIANT_BOOL	bShow;

	hr = pThing->get_BOOL(CComBSTR("IsUIVisible"), &bShow);
	if (SUCCEEDED(hr))
	{	
		hr = pWorld->get_User(&pUser);
		if (FAILED(hr) || (!pUser))
			goto CLEAN_UP;
		
		hr = pUser->get_Container(&pMyCont);
		if (FAILED(hr))
			goto CLEAN_UP;
		
		hr = pThing->get_Container(&pThingCont);
		if (FAILED(hr))
			goto CLEAN_UP;
		
		// Insert the item if we are actually showing it
		if ((bShow == VARIANT_TRUE) && (pMyCont == pThingCont))
			InsertThing(pThing);
	}

CLEAN_UP:
	SAFERELEASE(pMyCont);
	SAFERELEASE(pUser);
	SAFERELEASE(pThingCont);
}

//******************************************************
// RemoveItem
// Removes an existing item from the list view

void CRoomList::RemoveItem(IThing * pThing)
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

void CRoomList::RefreshItem(IThing * pThing)
{
	// TO DO - refresh name, description, etc..
}


void CRoomList::OnContextMenu(CWnd* pWnd, CPoint point) 
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
		// display menu with the property list VT_DISPATCH
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


long CRoomList::AssignIcon(IThing* pThing)
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

void CRoomList::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;


	if((pNMListView->uNewState & LVIS_SELECTED) && !m_bSelected)
		SetItemState(pNMListView->iItem,0, LVIS_SELECTED | LVIS_FOCUSED);
	
	*pResult = 0;
}

void CRoomList::OnItemchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;


	if((pNMListView->uNewState & LVIS_SELECTED) && !m_bSelected)
		SetItemState(pNMListView->iItem,0, LVIS_SELECTED | LVIS_FOCUSED);
	
	*pResult = 0;
}


