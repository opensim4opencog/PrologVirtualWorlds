// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// MListCtl.cpp : Implementation of the CMenuListCtrl ActiveX Control class.

#include "stdafx.h"
#include "objexplr.h"
#include "PrpEdCtl.h"
#include <propbase.h>
#include <menuitem.h>
#include "MenuList.h"
#include "MListCtl.h"
#include "syshelp.h"

#include "listdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CMenuListCtrl, CPropEdCtrl)

typedef CVWComPtr<IMethod, NULL, &IID_IMethod> CMethodPtr;

/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CMenuListCtrl, CPropEdCtrl)
	//{{AFX_MSG_MAP(CMenuListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_ADD_MENU_ITEM, OnAddMenuItem)
	ON_COMMAND(ID_DELETE_MENU_ITEM, OnDeleteMenuItem)
	ON_UPDATE_COMMAND_UI(ID_DELETE_MENU_ITEM, OnUpdateDeleteMenuItem)
	ON_UPDATE_COMMAND_UI(ID_ADD_MENU_ITEM, OnUpdateAddMenuItem)
	ON_COMMAND(ID_MOVE_MENU_ITEM_DOWN, OnMoveMenuItemDown)
	ON_UPDATE_COMMAND_UI(ID_MOVE_MENU_ITEM_DOWN, OnUpdateMoveMenuItemDown)
	ON_COMMAND(ID_MOVE_MENU_ITEM_UP, OnMoveMenuItemUp)
	ON_UPDATE_COMMAND_UI(ID_MOVE_MENU_ITEM_UP, OnUpdateMoveMenuItemUp)
	ON_COMMAND(ID_EDIT_MENU_TEXT, OnEditMenuText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MENU_TEXT, OnUpdateEditMenuText)
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CMenuListCtrl, CPropEdCtrl)
	//{{AFX_DISPATCH_MAP(CMenuListCtrl)
	DISP_PROPERTY_EX(CMenuListCtrl, "TargetObjectProperty", GetTargetObjectProperty, SetTargetObjectProperty, VT_DISPATCH)
	DISP_PROPERTY_EX(CMenuListCtrl, "SelectionList", GetSelectionList, SetSelectionList, VT_DISPATCH)
	DISP_PROPERTY_EX(CMenuListCtrl, "PropertyName", GetPropertyName, SetPropertyName, VT_BSTR)
	DISP_FUNCTION(CMenuListCtrl, "MoveSelectedPropertyUp", MoveSelectedPropertyUp, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMenuListCtrl, "MoveSelectedPropertyDown", MoveSelectedPropertyDown, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMenuListCtrl, "RemoveSelectedProperty", RemoveSelectedProperty, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMenuListCtrl, "DoFocus", DoFocus, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CMenuListCtrl, CPropEdCtrl)
	//{{AFX_EVENT_MAP(CMenuListCtrl)
	EVENT_CUSTOM("EditMenuItem", FireEditMenuItem, VTS_DISPATCH  VTS_I4)
	EVENT_CUSTOM("NewMenuItem", FireNewMenuItem, VTS_DISPATCH)
	EVENT_CUSTOM("UserMessage", FireUserMessage, VTS_BSTR)
	EVENT_CUSTOM("NoSelections", FireNoSelections, VTS_NONE)
	EVENT_CUSTOM("ItemSelected", FireItemSelected, VTS_NONE)
	EVENT_CUSTOM("KeyPressed", FireKeyPressed, VTS_I4)
	EVENT_CUSTOM("VKeyUp", FireVKeyUp, VTS_I4)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()



/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CMenuListCtrl, "OBJEXPLR.MenuListCtrl.1",
	0xef7d656d, 0x161, 0x11d2, 0x88, 0xbe, 0, 0xc0, 0x4f, 0xc3, 0x2e, 0xf3)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CMenuListCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DMenuList =
		{ 0xef7d656b, 0x161, 0x11d2, { 0x88, 0xbe, 0, 0xc0, 0x4f, 0xc3, 0x2e, 0xf3 } };
const IID BASED_CODE IID_DMenuListEvents =
		{ 0xef7d656c, 0x161, 0x11d2, { 0x88, 0xbe, 0, 0xc0, 0x4f, 0xc3, 0x2e, 0xf3 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwMenuListOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CMenuListCtrl, IDS_MENULIST, _dwMenuListOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CMenuListCtrl::CMenuListCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CMenuListCtrl

BOOL CMenuListCtrl::CMenuListCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_MENULIST,
			IDB_MENULIST,
			afxRegApartmentThreading,
			_dwMenuListOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CMenuListCtrl::CMenuListCtrl - Constructor

CMenuListCtrl::CMenuListCtrl()
{
	InitializeIIDs(&IID_DMenuList, &IID_DMenuListEvents);

	m_pThing = NULL;
	m_pMenuList = NULL;

	// add world events
	m_ulFlags |= ADVISE_WORLD;

	m_bCanAddToMenu = FALSE;
	m_bCanPutOnMenu = FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CMenuListCtrl::~CMenuListCtrl - Destructor

CMenuListCtrl::~CMenuListCtrl()
{
	SAFERELEASE(m_pThing);
	SAFERELEASE(m_pMenuList);
}


/////////////////////////////////////////////////////////////////////////////
// CMenuListCtrl::OnDraw - Drawing function

void CMenuListCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	DoSuperclassPaint(pdc, rcBounds);
}


/////////////////////////////////////////////////////////////////////////////
// CMenuListCtrl::DoPropExchange - Persistence support

void CMenuListCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	CPropEdCtrl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CMenuListCtrl::OnResetState - Reset control to default state

void CMenuListCtrl::OnResetState()
{
	CPropEdCtrl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CMenuListCtrl message handlers


int CMenuListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropEdCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	DWORD dwStyle = LVS_REPORT | WS_VISIBLE | WS_CHILD | LVS_SHOWSELALWAYS;

	RECT rect;
	GetClientRect(&rect);
	m_listCtrl.Create( dwStyle, rect, this, 255 );

	// try setting the extended styles
	m_listCtrl.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE,0,
                m_listCtrl.SendMessage (LVM_GETEXTENDEDLISTVIEWSTYLE) |
                LVS_EX_FULLROWSELECT);
	
	m_listCtrl.m_pOCX = this;
	
	if (m_pMenuList)
	{
		ClearList();
		FillList();
	}

	return 0;
}

void CMenuListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CPropEdCtrl::OnSize(nType, cx, cy);
	
	if ( m_listCtrl.m_hWnd )
		m_listCtrl.SetWindowPos(&wndTop,0,0,cx,cy,SWP_NOZORDER );
}

BOOL CMenuListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY*)lParam;
	
	switch(phdn->hdr.code)
	{
		case HDN_ITEMCLICK:
		case HDN_DIVIDERDBLCLICK:
		case HDN_ITEMCHANGING:
			//m_listCtrl.DeleteEditors();
			m_listCtrl.CommitEditors();
	}

	return CPropEdCtrl::OnNotify(wParam, lParam, pResult);
}

LPDISPATCH CMenuListCtrl::GetTargetObjectProperty() 
{
	SAFEADDREF(m_pMenuList);
	return m_pMenuList;
}

void CMenuListCtrl::SetTargetObjectProperty(LPDISPATCH newValue) 
{
	// make sure it is a PropList pointer
	IPropertyList *pMenuList = NULL;
	IPropertySecurity *pSecurity = NULL;
	HRESULT hr = S_OK;
	BSTR bstrType;
	CString strType;
	CString strMenu;
	VARIANT_BOOL vBool;
	CComBSTR	bstrName;

	if (newValue == NULL)
	{
		ClearList();
		SAFERELEASE(m_pMenuList);
		SAFERELEASE(m_pThing);
		m_pThing = NULL;
		m_pMenuList = NULL;
		return;
	}

	if (FAILED( hr = newValue->QueryInterface( IID_IPropertyList, (LPVOID*)&pMenuList) ) )
		return;

	if (!pMenuList)
		return;

	if (FAILED( hr = pMenuList->get_Type(&bstrType)))
		return;

	strMenu.LoadString(IDS_TYPE_MENU);
	strType = bstrType;
	SAFEFREESTRING(bstrType);
	if (strType.CompareNoCase(strMenu) != 0)
		return;


	if (m_pMenuList)
		SAFERELEASE(m_pMenuList);

	m_pMenuList = pMenuList;

	hr = m_pMenuList->get_Parent(&m_pThing);
	if (!m_pThing || FAILED(hr))
	{
		SAFERELEASE(m_pThing);
		m_pThing = NULL;
		return;
	}
	
	m_bCanAddToMenu = FALSE;
	m_bCanPutOnMenu = FALSE;

	if (FAILED(hr = m_pMenuList->get_PropertyName(&bstrName.m_str)))
		return;

	m_strPropName = bstrName;

	if (FAILED(hr = m_pThing->CheckPropertySecurity(bstrName, PS_WRITE, &vBool)))
		return;

	m_bCanPutOnMenu = (vBool == VARIANT_TRUE);

	if (FAILED(hr = m_pThing->CheckPropertySecurity(bstrName, PS_ADD, &vBool)))
		return;

	m_bCanAddToMenu = (vBool == VARIANT_TRUE);


	SetModifiedFlag();

	if (m_listCtrl.m_hWnd)
	{
		ClearList();
		FillList();
	}
}

LPDISPATCH CMenuListCtrl::GetSelectionList() 
{
	IPropertyList *pPropList = NULL;
	int iSelected = m_listCtrl.GetSelectedCount();
	HRESULT hr = S_OK;
	int iItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);
	int i = 0;
	CPropListData *pld = NULL;

	// create a property list
	if (!m_pWorld)
		goto FAIL_EXIT;

	if (FAILED(hr = CreatePropertyList(m_pWorld, &pPropList)) || !pPropList)
		goto FAIL_EXIT;

	for (i = iSelected; i > -1; i--)
	{
		if (iItem == -1)
			break;

		pld = (CPropListData *) m_listCtrl.GetItemData(iItem);
		if (!pld)
			break;

		// iItem is the selected item
		hr = pPropList->AddLong((long) pld->m_iIndex);
		if (FAILED(hr))
			break;

		iItem = m_listCtrl.GetNextItem(iItem, LVNI_SELECTED);
	}

	return pPropList;

FAIL_EXIT:
	return NULL;
}

void CMenuListCtrl::SetSelectionList(LPDISPATCH newValue) 
{
	IPropertyList *pPropList = NULL;
	HRESULT hr = S_OK;
	long iCount;
	long lItem;
	int iListIndex;
	long i;

	if (FAILED( hr = newValue->QueryInterface( IID_IPropertyList, (LPVOID*)&pPropList) ) )
		goto exit;

	if (!pPropList)
		goto exit;

	if (FAILED(hr = pPropList->get_Count(&iCount)))
		goto exit;

	m_listCtrl.ClearSelections();

	for (i = 0; i < iCount; i++)
	{
		if (NONSECURITYFAIL(hr = pPropList->get_Long(i, &lItem)))
			goto exit;

		if (SUCCEEDED(hr))
		{
			// select the item
			iListIndex = FindItemByIndex(lItem);
			if (iListIndex >= 0)
			{
				m_listCtrl.SelectItem(iListIndex, 0, FALSE);
//				m_listCtrl.SetItemState(iListIndex, LVIS_SELECTED, LVIS_SELECTED);
//				m_listCtrl.SetItemState(iListIndex, LVIS_DROPHILITED, LVIS_DROPHILITED);
			}
		}
	}

	// display the last selection
	m_listCtrl.ShowItemAndColumn(iListIndex, 0);

	SetModifiedFlag();

exit:

	SAFERELEASE(pPropList);
}

BSTR CMenuListCtrl::GetPropertyName() 
{
	CString strResult;
	strResult = m_strPropName;

	return strResult.AllocSysString();
}

void CMenuListCtrl::SetPropertyName(LPCTSTR lpszNewValue) 
{
	m_strPropName = lpszNewValue;

	SetModifiedFlag();
}

void CMenuListCtrl::MoveSelectedPropertyUp() 
{
	OnMoveMenuItemUp();
}

void CMenuListCtrl::MoveSelectedPropertyDown() 
{
	OnMoveMenuItemDown();
}

void CMenuListCtrl::RemoveSelectedProperty() 
{
	OnDeleteMenuItem();
}

//
// Class Functions
//

// world events
HRESULT CMenuListCtrl::HandleOnPropertyChanged(IThing *pthing, BSTR bstr, long lHint, VARIANT varHintData)
{
	HRESULT hr = S_OK;
	CString strName;
	IObjectProperty *pProp = NULL;
	IPropertyList *pList = NULL;
	VARIANT_BOOL vBool = VARIANT_TRUE;
	CPropListData * pPropListData = NULL;
	long lHintData = -1;
	long iListCount;
	long count;
	CComVariant varProperty;
	int index;

	if (pthing != m_pThing)
		return hr;

	// this is a message for our thing, but does it affect us?
	strName = bstr;
	if (strName.CompareNoCase(m_strPropName) != 0)
		return hr;

	if (varHintData.vt = VT_I4)
		lHintData = varHintData.iVal;

	hr = m_pMenuList->get_Count( &count );
	ASSERT(S_OK == hr);
	iListCount = m_listCtrl.GetItemCount();

	if (lHint == thPut)
	{
		// did a new value replace the list we are showing?
		ClearList();

		hr = m_pThing->get_ObjectProperty(bstr, (IObjectProperty **) &pProp);

		if (FAILED(hr) || !pProp)
		{
			SAFERELEASE(pProp);
			return hr;
		}

		hr = pProp->QueryInterface(IID_IPropertyList, (LPVOID*) &pList);
		if (FAILED(hr))
		{
			SAFERELEASE(pList);
			return hr;
		}

		// fire some error or warning message...
		/*CString strError;
		strError.LoadString(IDS_MSG_MENU_REPLACED);
		FireUserMessage(strError);
		*/
		SetTargetObjectProperty(NULL);

		SetTargetObjectProperty(pList);
		SAFERELEASE(pProp);
		SAFERELEASE(pList);
	}
	else if (lHint == thRemove)
	{
		// has the property we are working with been removed?
		ClearList();
		SAFERELEASE(m_pMenuList);
		SAFERELEASE(m_pThing);
		
		// fire some error or warning message...
		CString strError;
		strError.LoadString(IDS_MSG_MENU_GONE);
		FireUserMessage(strError);
	}
	else if ((lHint == plAdd) || (lHint == plInsertAt))
	{
		// has a property been added to the list?
		if ((lHintData == -1) || (count != (iListCount + 1)))
		{
			ClearList();
			FillList();
			return hr;
		}
		// ok, we should be able to add it to the list control
		hr = m_pMenuList->get_ObjectPropertyExt(lHintData, IID_IMenuItem, (IObjectProperty **) &pProp);
		if (hr == S_OK)
		{
			IMenuItem *pMenuItem = NULL;
			
			hr = pProp->QueryInterface(IID_IMenuItem, (LPVOID*) &pMenuItem);

			if (hr == S_OK)
			{
				iListCount = m_listCtrl.GetItemCount();
				if (lHintData < iListCount )
				{
					// go through the rest of the list and fix the indexes on the items
					for (int i = 0; i < iListCount; i++)
					{
						pPropListData = (CPropListData *) m_listCtrl.GetItemData(i);
						if (pPropListData->m_iIndex >= lHintData)
							pPropListData->m_iIndex = pPropListData->m_iIndex+1;
					}
				}
			
				AddMenuItemToList(lHintData, pMenuItem);
				m_listCtrl.Sort();
			}
			SAFERELEASE(pMenuItem);

		}
		SAFERELEASE(pProp);
	}
	else if ((lHint == plRemove) || (lHint == plRemoveAt))
	{
		if (lHintData == -1)
		{
			ClearList();
			FillList();
			return hr;
		}
		if (count != (iListCount - 1))
		{
			ClearList();
			FillList();
			return hr;
		}
		index = GetIndexOfItem(lHintData);
		m_listCtrl.DeleteItem(index);

		iListCount = m_listCtrl.GetItemCount();
		if (lHintData < iListCount )
		{
			// go through the rest of the list and fix the indexes on the items
			for (int i = 0; i < iListCount; i++)
			{
				pPropListData = (CPropListData *) m_listCtrl.GetItemData(i);
				if (pPropListData->m_iIndex > lHintData)
					pPropListData->m_iIndex = pPropListData->m_iIndex-1;
			}
		}
	}
	else if (lHint == plRemoveAll)
	{
		// all the properties have been removed from the map
		ClearList();
	}
	else if (lHint == plPut)
	{
		// put on the list

		// need to update that particular property
		index = GetIndexOfItem(lHintData);
		m_listCtrl.DeleteItem(index);

		// ok, we should be able to add it to the list control
		hr = m_pMenuList->get_ObjectPropertyExt(lHintData, IID_IMenuItem, (IObjectProperty **) &pProp);
		if (hr == S_OK)
		{
			IMenuItem *pMenuItem = NULL;

			hr = pProp->QueryInterface(IID_IMenuItem, (LPVOID*) &pMenuItem);
			if (hr == S_OK)
			{
				AddMenuItemToList(lHintData, pMenuItem);
			}
			
			SAFERELEASE(pMenuItem);
		}
		m_listCtrl.Sort();
		SAFERELEASE(pProp);
	}
	else
	{
		// when would this ever be called?
		ClearList();
		FillList();
	}

	return hr;
}


// client events
HRESULT CMenuListCtrl::HandleOnDisconnect(IDispatch* pWorld)
{
	SetTargetObjectProperty(NULL);

	return CVWUIView::HandleOnDisconnect(pWorld);
}

HRESULT CMenuListCtrl::HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient)
{
	IThing *pUser = NULL;
	HRESULT hr=S_OK;

	if (!m_pWorld)
	{
		SetTargetObjectProperty(NULL);
		goto exit;
	}

	if (FAILED(hr = m_pWorld->get_User(&pUser)))
		goto exit;

	if (pUser == pthing)
	{
		// I'm disconnecting!
		SetTargetObjectProperty(NULL);
	}

exit:

	SAFERELEASE(pUser);

	return CVWUIView::HandleOnUserDisconnect(pthing, bFromClient);
}

// FindItemByIndex
// since we can sort the list, the index of an item in the list
// control can be different that the index of an item in the
// property list, this function takes an index from a 
// property list and returns the PropListData class that
// corresponds to it
int CMenuListCtrl::FindItemByIndex(long lIndex)
{
	int iListCount;
	CPropListData *cpld = NULL;

	if (!m_pMenuList)
		return -1;

	iListCount = m_listCtrl.GetItemCount();

	// go through the rest of the list and fix the indexes on the items
	for (int i = 0; i < iListCount; i++)
	{
		cpld = (CPropListData *) m_listCtrl.GetItemData(i);
		if (cpld->m_iIndex == lIndex)
			return i;
	}

	return -1;
}

// ClearList
// Empties the items in the list control, freeing memory as necessary
void CMenuListCtrl::ClearList()
{
	m_listCtrl.DeleteAllItems();
}

// AddMenuItemToList
// given a MenuItem, adds it to the list ctrl
void CMenuListCtrl::AddMenuItemToList(long iIndex, IMenuItem *pMenuItem)
{
	BSTR bstrMenuText;
	BSTR bstrMethodName;
	CString	strMenuText, strMethodName;
	long lSortOrder = 0;
	HRESULT hr = S_OK;
	LV_ITEM lvi;
	int iSubItem = 0;
	int iActualItem = 0;

	if (!pMenuItem)
		return;

	if (FAILED(hr = pMenuItem->get_MenuText(&bstrMenuText)))
		return;

	strMenuText = bstrMenuText;
	SAFEFREESTRING(bstrMenuText);

	if (FAILED(hr = pMenuItem->get_Method(&bstrMethodName)))
		return;

	strMethodName = bstrMethodName;
	SAFEFREESTRING(bstrMethodName);

//	if (FAILED(hr = pMenuItem->get_SortOrder(&lSortOrder)))
//		return;

	CPropListData * pPropListData;
	pPropListData = new CPropListData(iIndex, lSortOrder);
	pPropListData->m_strName = strMenuText;
	pPropListData->m_strValue = strMethodName;
	pPropListData->m_bEditable = m_bCanPutOnMenu;

	// set the name for the item
	lvi.mask = LVIF_TEXT | LVIF_PARAM;
	lvi.iItem = iIndex;
	lvi.iSubItem = iSubItem++;
	lvi.pszText = strMenuText.GetBuffer(strMenuText.GetLength() + 1);
	lvi.cchTextMax = strMenuText.GetLength() + 1;
	lvi.lParam = (LPARAM) pPropListData;
	iActualItem = m_listCtrl.InsertItem(&lvi);

	// set the type string for the item
	lvi.mask = LVIF_TEXT;
	lvi.iItem = iActualItem;
	lvi.iSubItem = iSubItem++;
	lvi.pszText = strMethodName.GetBuffer(strMethodName.GetLength() + 1);
	lvi.cchTextMax = strMethodName.GetLength() + 1;
	m_listCtrl.SetItem(&lvi);
}

// LaunchExternalEditor
// iItem - item that the button was on
/*void CMenuListCtrl::LaunchExternalEditor(int iItem)
{
	CPropListData *cpld = NULL;
	cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	if (!cpld)
		return;

	FireEditMenuItem(m_pMenuList, cpld->m_iIndex);
}
*/

// FillList
// Called when a new Target Object Property is Set
// Iterates through the propList of the Thing and adds the items to the
// List Control
void CMenuListCtrl::FillList()
{
	// REVIEW: handle Security Checking when that stuff is done

	if (!m_pMenuList)
	{
		// if the thing is null it means that the list should be emptied
		ClearList();
		return;
	}

	if (!m_hWnd)
		return;

	HRESULT hr = S_OK;
	long iCount;
	IObjectProperty *pProp = NULL;

	if (FAILED(hr = m_pMenuList->get_Count(&iCount)))
		return;

	// REVIEW: everything will be friendly until the security stuff is re-designed
	BOOL bFriendly = TRUE;

	for (long i = 0; i < iCount; i++)
	{
		hr = m_pMenuList->get_ObjectPropertyExt(i, IID_IMenuItem, (IObjectProperty **) &pProp);
		if (hr == S_OK)
		{
			IMenuItem *pMenuItem = NULL;

			hr = pProp->QueryInterface(IID_IMenuItem, (LPVOID*) &pMenuItem);
			if (hr == S_OK)
			{
				AddMenuItemToList(i, pMenuItem);
			}
	
			SAFERELEASE(pMenuItem);
		}

		SAFERELEASE(pProp);
	}

	m_listCtrl.Sort();
}


//
// EditName
// iItem - item that was clicked on
// strName - the old menu text
//
// called when a user 2nd clicks on the menu text of an item
// show the edit box
void CMenuListCtrl::EditName(int iItem, CString strName)
{
	m_listCtrl.EditName(iItem, strName, 0);
}

//
// DoneEditingName
// iItem - item that the name is changing on
// strNewValue - the new menu text
//
void CMenuListCtrl::DoneEditingName(int iItem, CString strNewValue)
{
	// get the menu item and set the menu text on it with the new value
	IObjectProperty *pProp = NULL;
	IMenuItem *pMenuItem = NULL;
	CPropListData *cpld = NULL;
	HRESULT hr = S_OK;
	long iIndex;

	cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	if (!cpld)
		return;

	// check to see if we've actually changed the value
	if (!cpld->m_strName.CompareNoCase(strNewValue))
		return;

	iIndex = cpld->m_iIndex;

	if (FAILED(hr = m_pMenuList->get_ObjectPropertyExt(iIndex, IID_IMenuItem, (IObjectProperty **) &pProp)))
		goto exit;

	if (!pProp)
		goto exit;

	if (FAILED(hr = pProp->QueryInterface(IID_IMenuItem, (LPVOID*) &pMenuItem)))
		goto exit;

	if (FAILED(hr = pMenuItem->put_MenuText(CComBSTR(strNewValue))))
		goto exit;
	
	// remove the property from the list to make sure the security pointer is cleared
	if (FAILED(hr = m_pMenuList->RemoveAt(iIndex)))
		goto exit;

	// put the property back in the list to make sure it's remoted
	if (FAILED(hr = m_pMenuList->InsertAt(iIndex, CComVariant(pMenuItem))))
		goto exit;

exit:
	SAFERELEASE(pProp);
	SAFERELEASE(pMenuItem);
}


void CMenuListCtrl::GetMethodList(CStringList &strMethodList)
{
	HRESULT			hr = S_OK;
	IPropertyMap	*pMethodMap = NULL;
//	CMethodPtr		pMethod;
	VARIANT_BOOL	vbDone = VARIANT_FALSE;
	CComBSTR		bstrMethodName;
	CComVariant		varMethod;
	CString			strMethodName;

	if (!m_pThing)
		return;

	// fill in the Methods
	if (FAILED(m_pThing->get_Methods(&pMethodMap)) || !pMethodMap)
		return;

	// walk through the list
	if (FAILED(hr = pMethodMap->FirstItem(&bstrMethodName.m_str, &varMethod, &vbDone)))
		goto failexit;

	while (vbDone == VARIANT_FALSE)
	{
//		pMethod = varMethod;
//		if (pMethod)
//		{
			// it's a script Method (we QI for it in CMethodPtr)
			strMethodName = bstrMethodName;
			strMethodList.AddHead(strMethodName);
//		}

		if (FAILED(hr = pMethodMap->NextItem(&bstrMethodName.m_str, &varMethod, &vbDone)))
			goto failexit;
	}

failexit:
	SAFERELEASE(pMethodMap);
}


void CMenuListCtrl::DoneEditingMethod(int iItem, CString strNewMethod)
{
	// get the menu item and set the menu text on it with the new value
	IObjectProperty *pProp = NULL;
	IMenuItem *pMenuItem = NULL;
	CPropListData *cpld = NULL;
	HRESULT hr = S_OK;
	CString strNone;
	int iIndex;

	cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	if (!cpld)
		return;

	iIndex = cpld->m_iIndex;

	if (FAILED(hr = m_pMenuList->get_ObjectPropertyExt(cpld->m_iIndex, IID_IMenuItem, (IObjectProperty **) &pProp)))
		goto exit;

	if (!pProp)
		goto exit;

	if (FAILED(hr = pProp->QueryInterface(IID_IMenuItem, (LPVOID*) &pMenuItem)))
		goto exit;

	strNone.LoadString(IDS_NONE);

	if (strNewMethod.Compare(strNone) == 0)
		strNewMethod.Empty();

	if (FAILED(hr = pMenuItem->put_Method(CComBSTR(strNewMethod))))
		goto exit;
	
	// put the property back in the list to make sure it's remoted
//	if (FAILED(hr = m_pMenuList->put_ObjectProperty(cpld->m_iIndex, pMenuItem)))
//		goto exit;

	// remove the property from the list to make sure the security pointer is cleared
	if (FAILED(hr = m_pMenuList->RemoveAt(iIndex)))
		goto exit;

	// put the property back in the list to make sure it's remoted
	if (FAILED(hr = m_pMenuList->InsertAt(iIndex, CComVariant(pMenuItem))))
		goto exit;

exit:
	SAFERELEASE(pProp);
	SAFERELEASE(pMenuItem);
}


//
// EditMethod
// iItem - item that was clicked on
// strMethod - the old method Name
//
// called when a user 2nd clicks on the method of an item
// show the drop down list of methods on the object
void CMenuListCtrl::EditMethod(int iItem, CString strMethod)
{
	m_listCtrl.EditMethod(iItem, strMethod);
}


//
// LeftClick
// iItem - item that was clicked on
// iColumn - column the double click was in
//
// called when a user double clicks in the list control
void CMenuListCtrl::LeftClick(int iItem, int iColumn)
{
	CPropListData *cpld = NULL;
	UINT state;

	if (iItem < 0)
		return;

	cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	if (!cpld)
		return;

	if (!m_bCanPutOnMenu)
		return;

	// check to see if the item was already selected
	state = m_listCtrl.GetItemState(iItem, LVIS_SELECTED);
	if (state == LVIS_SELECTED)
	{
		switch (iColumn)
		{
			case 0:
				EditName(iItem, cpld->m_strName);
				break;
			case 1:
				EditMethod(iItem, cpld->m_strValue);
				break;
			default:
				break;
		}
	}
}

//
// DoubleClick
// iItem - item that was clicked on
// iColumn - column the double click was in
//
// called when a user double clicks in the list control
void CMenuListCtrl::DoubleClick(int iItem, int iColumn)
{
	CPropListData *cpld = NULL;

	if (iItem < 0)
		return;

	cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	if (!cpld)
		return;

	if (!m_bCanPutOnMenu)
		return;

	EditMethod(iItem, cpld->m_strValue);
}

//
// GetIndexOfSelectedItem
//
// finds the first selected item and returns it's real index
int CMenuListCtrl::GetIndexOfSelectedItem()
{
	int iItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);

	return iItem;
}

int CMenuListCtrl::GetIndexOfItem(long pos)
{
	int iListCount;
	CPropListData *pPropListData = NULL;

	iListCount = m_listCtrl.GetItemCount();
	for (int i = 0; i < iListCount; i++)
	{
		pPropListData = (CPropListData *) m_listCtrl.GetItemData(i);
		if (pPropListData->m_iIndex == pos)
			return i;
	}

	return -1;
}

//
// PopUpMenu
// point - location of the right click
//
// called when a user double clicks in the list control
void CMenuListCtrl::PopUpMenu(CPoint point)
{
	CMenu menu;
	CMenu *pPopup;
	
	if (!menu.LoadMenu(IDR_MENU_LIST_MENU))
		return;
	
	pPopup = menu.GetSubMenu(0);
	if (!pPopup)
		return;

	RECT winRect;
	GetWindowRect( &winRect );
	int newx = point.x + winRect.left;
	int newy = point.y + winRect.top;

	BOOL bResult = pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, newx, newy, this);

	// not doing anything with bResult, do we care?

	menu.DestroyMenu();

}

void CMenuListCtrl::VirtualKeyPressed(UINT nChar)
{
	switch(nChar)
	{
		case VK_DELETE:
			OnDeleteMenuItem();
			break;
		case VK_RETURN:
			if (m_listCtrl.ShowFirstSelectedItem())
			{
				int iItem;
				CPropListData *pPropListData = NULL;
				iItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);
				if (iItem >= 0)
				{
					pPropListData = (CPropListData *) m_listCtrl.GetItemData(iItem);
					if (pPropListData)
						EditMethod(iItem, pPropListData->m_strValue);
				}
			}
			break;
		default:
			FireKeyPressed(nChar);
			break;
	}
}

void CMenuListCtrl::VirtualKeyUp(UINT nChar)
{
	FireVKeyUp(nChar);
}

void CMenuListCtrl::FireSelectionEvent(BOOL bSelections)
{
	if (bSelections)
		FireItemSelected();
	else
		FireNoSelections();
}

//
// AlphaKeyPressed
//
// called when a user presses a letter key in the list control
void CMenuListCtrl::AlphaKeyPressed(char cLetter)
{
	// walk through items in list
	int iListCount;
	CPropListData *cpld = NULL;
	CString strCompare;

	if (!m_pMenuList)
		return;

	if ((m_listCtrl.m_iSortColumn < 0) || (m_listCtrl.m_iSortColumn > 1))
		return;

	iListCount = m_listCtrl.GetItemCount();

	// go through the rest of the list and fix the indexes on the items
	for (int i = 0; i < iListCount; i++)
	{
		cpld = (CPropListData *) m_listCtrl.GetItemData(i);
		switch(m_listCtrl.m_iSortColumn)
		{
			case 0:
				strCompare = cpld->m_strName;
				break;
			case 1:
				strCompare = cpld->m_strValue;
				break;
		}
		if (!strCompare.IsEmpty() && ((strCompare[0] == cLetter) || (toupper(strCompare[0]) == cLetter)))
		{
			m_listCtrl.SelectItem(i, m_listCtrl.m_iSortColumn);
			return;
		}
	}
}

void CMenuListCtrl::OnAddMenuItem() 
{
	FireNewMenuItem(m_pMenuList);
}

void CMenuListCtrl::OnUpdateAddMenuItem(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bCanAddToMenu);
}

void CMenuListCtrl::OnEditMenuText() 
{
	int iItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);
	if (iItem == -1)
		return;

	CPropListData *cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	
	EditName(iItem, cpld->m_strName);
	
}

void CMenuListCtrl::OnUpdateEditMenuText(CCmdUI* pCmdUI) 
{
	int iSelected = m_listCtrl.GetSelectedCount();
	
	pCmdUI->Enable((iSelected == 1) && m_bCanPutOnMenu);
}

void CMenuListCtrl::OnDeleteMenuItem() 
{
//	FireDeleteMenuItems(m_pMenuList, NULL);
		// go through the list of properties and delete each selected one
	int iSelected = m_listCtrl.GetSelectedCount();
	if (iSelected == 0)
		return;

	int iLastItem;

	CPtrArray cpaSelections;
	cpaSelections.SetSize(iSelected);
	int iItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);
	int i = 0;
	
	while (iItem != -1)
	{
		iLastItem = iItem;
		cpaSelections.SetAt(i++, (CPropListData *) m_listCtrl.GetItemData(iItem));
		iItem = m_listCtrl.GetNextItem(iItem, LVNI_SELECTED);
	}

	HRESULT hr = S_OK;
	for (i = cpaSelections.GetSize() - 1; i > -1; i--)
	{
		CPropListData *pld = (CPropListData *) cpaSelections.GetAt(i);
		if (!pld)
			break;

		// iItem is the selected item
		hr = m_pMenuList->RemoveAt(pld->m_iIndex);
		if (FAILED(hr))
			break;
		iLastItem--;
	}

	m_listCtrl.SelectItem(iLastItem + 1);
}

void CMenuListCtrl::OnUpdateDeleteMenuItem(CCmdUI* pCmdUI) 
{
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();

	if (uiSelected > 0)
		pCmdUI->Enable(m_bCanAddToMenu);
	else
		pCmdUI->Enable(FALSE);
}

void CMenuListCtrl::OnMoveMenuItemDown() 
{
	int iIndex;
	CPropListData *cpld;
	HRESULT hr = S_OK;
	COleVariant var;
	long oldIndex;
	long count;
	int iNewIndex;
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();

	if (uiSelected != 1)
		return;

	// REVIEW: what about security info?

	iIndex = GetIndexOfSelectedItem();
	if (iIndex < 0)
		return;

	cpld = (CPropListData *) m_listCtrl.GetItemData(iIndex);
	oldIndex = cpld->m_iIndex;
	//FireMovePropertyDown(m_pPropList, iIndex);
	if (FAILED(hr = m_pMenuList->get_Count(&count)))
		return;

	if (oldIndex == count - 1)
		return;

	// ok, get the property
	if (FAILED(hr = m_pMenuList->get_Property(oldIndex, &var)))
	{
		// Review: ERROR!
		return;
	}

	// remove the property
	if (FAILED(hr = m_pMenuList->RemoveAt(oldIndex)))
	{
		// Review: ERROR!
		return;
	}
	
	// add the property
	if (FAILED(hr = m_pMenuList->InsertAt(oldIndex + 1, var)))
	{
		// Review: ERROR!
		return;
	}

	m_listCtrl.Sort();

	// select it
	iNewIndex = GetIndexOfItem(oldIndex + 1);
	m_listCtrl.SetItemState(iNewIndex, LVIS_SELECTED, LVIS_SELECTED);
//	m_listCtrl.SetItemState(iNewIndex, LVIS_DROPHILITED, LVIS_DROPHILITED);
}

void CMenuListCtrl::OnUpdateMoveMenuItemDown(CCmdUI* pCmdUI) 
{
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();

	if (uiSelected == 1)
		pCmdUI->Enable(m_bCanPutOnMenu && m_bCanAddToMenu);
	else
		pCmdUI->Enable(FALSE);
}

void CMenuListCtrl::OnMoveMenuItemUp() 
{
	int iIndex;
	CPropListData *cpld;
	HRESULT hr = S_OK;
	COleVariant var;
	long oldIndex;
	int iNewIndex;
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();

	if (uiSelected != 1)
		return;

	// REVIEW: what about security info?

	iIndex = GetIndexOfSelectedItem();
	if (iIndex < 0)
		return;

	cpld = (CPropListData *) m_listCtrl.GetItemData(iIndex);
	oldIndex = cpld->m_iIndex;
	//FireMovePropertyUp(m_pPropList, iIndex);
	
	if (oldIndex < 1)
		return;

	// ok, get the property
	if (FAILED(hr = m_pMenuList->get_Property(oldIndex, &var)))
	{
		// Review: ERROR!
		return;
	}

	// remove the property
	if (FAILED(hr = m_pMenuList->RemoveAt(oldIndex)))
	{
		// Review: ERROR!
		return;
	}
	
	// add the property
	if (FAILED(hr = m_pMenuList->InsertAt(oldIndex - 1, var)))
	{
		// Review: ERROR!
		return;
	}

	m_listCtrl.Sort();

	// select it
	iNewIndex = GetIndexOfItem(oldIndex - 1);
	m_listCtrl.SetItemState(iNewIndex, LVIS_SELECTED, LVIS_SELECTED);
//	m_listCtrl.SetItemState(iNewIndex, LVIS_DROPHILITED, LVIS_DROPHILITED);

}

void CMenuListCtrl::OnUpdateMoveMenuItemUp(CCmdUI* pCmdUI) 
{
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();

	if (uiSelected == 1)
		pCmdUI->Enable(m_bCanPutOnMenu && m_bCanAddToMenu);
	else
		pCmdUI->Enable(FALSE);
}

void CMenuListCtrl::DoFocus() 
{
	if (m_listCtrl.m_hWnd)
	{
		m_listCtrl.SetFocus();
		UINT uiSelected;
		uiSelected = m_listCtrl.GetSelectedCount();

		if (uiSelected == 0)
			m_listCtrl.SelectItem(0, m_listCtrl.m_iSortColumn);
	}
}
