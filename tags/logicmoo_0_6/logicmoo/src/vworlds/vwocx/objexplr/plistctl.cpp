// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// PListCtl.cpp : Implementation of the CPropListCtrl ActiveX Control class.

#include "stdafx.h"
#include "objexplr.h"
#include "PrpEdCtl.h"
#include "PListCtl.h"
#include "listdata.h"
#include "syshelp.h"

#include <propbase.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CPropListCtrl, CPropEdCtrl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CPropListCtrl, CPropEdCtrl)
	//{{AFX_MSG_MAP(CPropListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_ADD_PROPERTY, OnAddProperty)
	ON_COMMAND(ID_DELETE_PROPERTY, OnDeleteProperty)
	ON_UPDATE_COMMAND_UI(ID_DELETE_PROPERTY, OnUpdateDeleteProperty)
	ON_COMMAND(ID_EDIT_VALUE, OnEditValue)
	ON_UPDATE_COMMAND_UI(ID_EDIT_VALUE, OnUpdateEditValue)
	ON_COMMAND(ID_MOVE_PROP_DOWN, OnMovePropDown)
	ON_UPDATE_COMMAND_UI(ID_MOVE_PROP_DOWN, OnUpdateMovePropDown)
	ON_COMMAND(ID_MOVE_PROP_UP, OnMovePropUp)
	ON_UPDATE_COMMAND_UI(ID_MOVE_PROP_UP, OnUpdateMovePropUp)
	ON_UPDATE_COMMAND_UI(ID_ADD_PROPERTY, OnUpdateAddProperty)
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CPropListCtrl, CPropEdCtrl)
	//{{AFX_DISPATCH_MAP(CPropListCtrl)
	DISP_PROPERTY_EX(CPropListCtrl, "SelectionList", GetSelectionList, SetSelectionList, VT_DISPATCH)
	DISP_PROPERTY_EX(CPropListCtrl, "TargetObjectProperty", GetTargetObjectProperty, SetTargetObjectProperty, VT_DISPATCH)
	DISP_PROPERTY_EX(CPropListCtrl, "PropertyName", GetPropertyName, SetPropertyName, VT_BSTR)
	DISP_FUNCTION(CPropListCtrl, "DoFocus", DoFocus, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CPropListCtrl, "MoveSelectedPropertyUp", MoveSelectedPropertyUp, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CPropListCtrl, "MoveSelectedPropertyDown", MoveSelectedPropertyDown, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CPropListCtrl, "RemoveSelectedProperty", RemoveSelectedProperty, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CPropListCtrl, CPropEdCtrl)
	//{{AFX_EVENT_MAP(CPropListCtrl)
	EVENT_CUSTOM("EditValue", FireEditValue, VTS_DISPATCH  VTS_I4)
	EVENT_CUSTOM("NewProperty", FireNewProperty, VTS_DISPATCH)
	EVENT_CUSTOM("UserMessage", FireUserMessage, VTS_BSTR)
	EVENT_CUSTOM("KeyPressed", FireKeyPressed, VTS_I4)
	EVENT_CUSTOM("VKeyUp", FireVKeyUp, VTS_I4)
	EVENT_CUSTOM("NoSelections", FireNoSelections, VTS_NONE)
	EVENT_CUSTOM("ItemSelected", FireItemSelected, VTS_NONE)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()



/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CPropListCtrl, "OBJEXPLR.PropListCtrl.1",
	0xef7d6565, 0x161, 0x11d2, 0x88, 0xbe, 0, 0xc0, 0x4f, 0xc3, 0x2e, 0xf3)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CPropListCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DPropList =
		{ 0xef7d6563, 0x161, 0x11d2, { 0x88, 0xbe, 0, 0xc0, 0x4f, 0xc3, 0x2e, 0xf3 } };
const IID BASED_CODE IID_DPropListEvents =
		{ 0xef7d6564, 0x161, 0x11d2, { 0x88, 0xbe, 0, 0xc0, 0x4f, 0xc3, 0x2e, 0xf3 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwPropListOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CPropListCtrl, IDS_PROPLIST, _dwPropListOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CPropListCtrl::CPropListCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CPropListCtrl

BOOL CPropListCtrl::CPropListCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_PROPLIST,
			IDB_PROPLIST,
			afxRegApartmentThreading,
			_dwPropListOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CPropListCtrl::CPropListCtrl - Constructor

CPropListCtrl::CPropListCtrl()
{
	InitializeIIDs(&IID_DPropList, &IID_DPropListEvents);

	m_pThing = NULL;
	m_pPropList = NULL;

	m_bCanAddToList = FALSE;
	m_bCanPutOnList = FALSE;

	// add world events
	m_ulFlags |= ADVISE_WORLD;
}


/////////////////////////////////////////////////////////////////////////////
// CPropListCtrl::~CPropListCtrl - Destructor

CPropListCtrl::~CPropListCtrl()
{
	SAFERELEASE(m_pThing);
	SAFERELEASE(m_pPropList);
}


/////////////////////////////////////////////////////////////////////////////
// CPropListCtrl::OnDraw - Drawing function

void CPropListCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	DoSuperclassPaint(pdc, rcBounds);
}

BOOL CPropListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY*)lParam;
	
	switch(phdn->hdr.code)
	{
		case HDN_ITEMCLICK:
		case HDN_DIVIDERDBLCLICK:
		case HDN_ITEMCHANGING:
			//m_listCtrl.DeleteEditors();
			m_listCtrl.CommitEditors();
			break;
	}

	return CVWUIView::OnNotify(wParam, lParam, pResult);
}


/////////////////////////////////////////////////////////////////////////////
// CPropListCtrl::DoPropExchange - Persistence support

void CPropListCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	CPropEdCtrl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CPropListCtrl::OnResetState - Reset control to default state

void CPropListCtrl::OnResetState()
{
	CPropEdCtrl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CPropListCtrl message handlers

int CPropListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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

	if (m_pPropList)
		FillList();

	return 0;
}

void CPropListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CPropEdCtrl::OnSize(nType, cx, cy);
	
	if ( m_listCtrl.m_hWnd )
		m_listCtrl.SetWindowPos(&wndTop,0,0,cx,cy,SWP_NOZORDER );
	
}

void CPropListCtrl::OnAddProperty() 
{
	// Fire the add property
	FireNewProperty(m_pPropList);
}

void CPropListCtrl::OnUpdateAddProperty(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bCanAddToList);
}


void CPropListCtrl::OnDeleteProperty() 
{
	// Fire the Delete Property Event
	//FireDeleteProperty(m_pPropList, NULL);
	// go through the list of properties and delete each selected one
	int iSelected = m_listCtrl.GetSelectedCount();
	int iLastItem;
	if (iSelected == 0)
		return;

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

		hr = m_pPropList->RemoveAt(pld->m_iIndex);
		if (FAILED(hr))
			break;
		iLastItem--;
	}

	m_listCtrl.SelectItem(iLastItem + 1);
}


void CPropListCtrl::OnUpdateDeleteProperty(CCmdUI* pCmdUI) 
{
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();

	if (uiSelected > 0)
		pCmdUI->Enable(m_bCanAddToList);
	else
		pCmdUI->Enable(FALSE);
}

void CPropListCtrl::OnEditValue() 
{
	int iIndex;
	iIndex = GetIndexOfSelectedItem();

	if (iIndex < 0)
		return;

	CPropListData *cpld = NULL;
	cpld = (CPropListData *) m_listCtrl.GetItemData(iIndex);

	if (!cpld)
		return;

	if (Editable(cpld->m_vtType))
	{
		if (cpld->m_vtType != VT_BOOL)
			m_listCtrl.EditValue(iIndex, cpld->m_strValue);
		else
			m_listCtrl.EditValue(iIndex, (cpld->m_strValue.CompareNoCase("true") == 0));
	}
	else
		FireEditValue(m_pPropList, iIndex);
}

void CPropListCtrl::OnUpdateEditValue(CCmdUI* pCmdUI) 
{
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();

	if (uiSelected == 1)
		pCmdUI->Enable(m_bCanPutOnList);
	else
		pCmdUI->Enable(FALSE);
}

void CPropListCtrl::OnMovePropDown() 
{
	int iIndex, iNewIndex;
	CPropListData *cpld;
	HRESULT hr = S_OK;
	COleVariant var;
	long oldIndex;
	long count;
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();

	if (uiSelected != 1)
		return;

	// REVIEW: what about security info?

	iIndex = GetIndexOfSelectedItem();
	cpld = (CPropListData *) m_listCtrl.GetItemData(iIndex);
	oldIndex = cpld->m_iIndex;
	//FireMovePropertyDown(m_pPropList, iIndex);
	if (FAILED(hr = m_pPropList->get_Count(&count)))
		return;

	if (oldIndex == count - 1)
		return;

	// ok, get the property
	if (FAILED(hr = m_pPropList->get_Property(oldIndex, &var)))
	{
		// Review: ERROR!
		return;
	}

	// remove the property
	if (FAILED(hr = m_pPropList->RemoveAt(oldIndex)))
	{
		// Review: ERROR!
		return;
	}
	
	// add the property
	if (FAILED(hr = m_pPropList->InsertAt(oldIndex + 1, var)))
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

void CPropListCtrl::OnUpdateMovePropDown(CCmdUI* pCmdUI) 
{
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();

	if (uiSelected == 1)
		pCmdUI->Enable(m_bCanAddToList && m_bCanPutOnList);
	else
		pCmdUI->Enable(FALSE);
}

void CPropListCtrl::OnMovePropUp() 
{
	int iIndex, iNewIndex;
	CPropListData *cpld;
	HRESULT hr = S_OK;
	COleVariant var;
	long oldIndex;
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();

	if (uiSelected != 1)
		return;

	// REVIEW: what about security info?

	iIndex = GetIndexOfSelectedItem();
	cpld = (CPropListData *) m_listCtrl.GetItemData(iIndex);
	oldIndex = cpld->m_iIndex;
	//FireMovePropertyUp(m_pPropList, iIndex);
	
	if (oldIndex < 1)
		return;

	// ok, get the property
	if (FAILED(hr = m_pPropList->get_Property(cpld->m_iIndex, &var)))
	{
		// Review: ERROR!
		return;
	}

	// remove the property
	if (FAILED(hr = m_pPropList->RemoveAt(oldIndex)))
	{
		// Review: ERROR!
		return;
	}
	
	// add the property
	if (FAILED(hr = m_pPropList->InsertAt(oldIndex - 1, var)))
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

void CPropListCtrl::OnUpdateMovePropUp(CCmdUI* pCmdUI) 
{
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();

	if (uiSelected == 1)
		pCmdUI->Enable(m_bCanAddToList && m_bCanPutOnList);
	else
		pCmdUI->Enable(FALSE);
}

//
// OCX Properties
//

LPDISPATCH CPropListCtrl::GetSelectionList() 
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

void CPropListCtrl::SetSelectionList(LPDISPATCH newValue) 
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
				m_listCtrl.SelectItem(iListIndex, 0, FALSE);
		}
	}

	SetModifiedFlag();

	// display the last selection
	m_listCtrl.ShowItemAndColumn(iListIndex, 0);

exit:

	SAFERELEASE(pPropList);

}

LPDISPATCH CPropListCtrl::GetTargetObjectProperty() 
{
	SAFEADDREF(m_pPropList);
	return m_pPropList;
}

void CPropListCtrl::SetTargetObjectProperty(LPDISPATCH newValue) 
{
	// make sure it is a PropMap pointer
	IPropertyList *pPropList = NULL;
	IPropertySecurity *pSecurity = NULL;
	HRESULT hr = S_OK;
	VARIANT_BOOL vBool = VARIANT_FALSE;
	CComBSTR	bstrName;

	if (newValue == NULL)
	{
		ClearList();
		SAFERELEASE(m_pPropList);
		SAFERELEASE(m_pThing);
		m_pThing = NULL;
		m_pPropList = NULL;
		return;
	}

	if (FAILED( hr = newValue->QueryInterface( IID_IPropertyList, (LPVOID*)&pPropList) ) )
		return;

	if (!pPropList)
		return;

	if (m_pPropList)
		SAFERELEASE(m_pPropList);

	m_pPropList = pPropList;

	hr = m_pPropList->get_Parent(&m_pThing);
	if (!m_pThing || FAILED(hr))
	{
		SAFERELEASE(m_pThing);
		m_pThing = NULL;
	}
	
	SetModifiedFlag();

	m_bCanAddToList = FALSE;
	m_bCanPutOnList = FALSE;

	if (FAILED(hr = m_pPropList->get_PropertyName(&bstrName.m_str)))
		return;

	m_strPropName = bstrName;

	if (FAILED(hr = m_pThing->CheckPropertySecurity(bstrName, PS_WRITE, &vBool)))
		return;

	m_bCanPutOnList = (vBool == VARIANT_TRUE);

	if (FAILED(hr = m_pThing->CheckPropertySecurity(bstrName, PS_ADD, &vBool)))
		return;

	m_bCanAddToList = (vBool == VARIANT_TRUE);

	if (m_listCtrl.m_hWnd)
		FillList();
}

BSTR CPropListCtrl::GetPropertyName() 
{
	CString strResult;
	
	strResult = m_strPropName;

	return strResult.AllocSysString();
}

void CPropListCtrl::SetPropertyName(LPCTSTR lpszNewValue) 
{
	m_strPropName = lpszNewValue;

	SetModifiedFlag();
}

//
// Class Functions
//

HRESULT CPropListCtrl::HandleOnPropertyChanged(IThing *pthing, BSTR bstr, long lHint, VARIANT varHintData)
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
	CString strIndex;
	LV_ITEM lvi;

	if (pthing != m_pThing)
		return hr;

	// this is a message for our thing, but does it affect us?
	strName = bstr;
	if (strName.CompareNoCase(m_strPropName) != 0)
		return hr;

	if (varHintData.vt = VT_I4)
		lHintData = varHintData.iVal;

	hr = m_pPropList->get_Count( &count );
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
		CString strError;
		strError.LoadString(IDS_MSG_LIST_REPLACED);
		FireUserMessage(strError);

		SetTargetObjectProperty(pList);
		SAFERELEASE(pProp);
		SAFERELEASE(pList);
	}
	else if (lHint == thRemove)
	{
		// has the property we are working with been removed?
		ClearList();
		SAFERELEASE(m_pPropList);
		SAFERELEASE(m_pThing);
		
		// fire some error or warning message...
		CString strError;
		strError.LoadString(IDS_MSG_LIST_GONE);
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

		iListCount = m_listCtrl.GetItemCount();
		if (lHintData < iListCount)
		{
			// go through the the list and fix the indexes on the items
			for (int i = 0; i < iListCount; i++)
			{
				pPropListData = (CPropListData *) m_listCtrl.GetItemData(i);
				if (pPropListData->m_iIndex >= lHintData)
				{
					pPropListData->m_iIndex++;
					// change the display also
					strIndex.Format("%d", pPropListData->m_iIndex);

					lvi.mask = LVIF_TEXT;
					lvi.iItem = i;
					lvi.iSubItem = 0;
					lvi.pszText = strIndex.GetBuffer(strIndex.GetLength() + 1);
					lvi.cchTextMax = strIndex.GetLength() + 1;
					m_listCtrl.SetItem(&lvi);
				}
			}
		}

		// ok, we should be able to add it to the list control
		hr = m_pPropList->get_Property(lHintData, &varProperty);
		if (FAILED(hr)) goto failexit;

		AddVariantItem( lHintData, varProperty );

		m_listCtrl.Sort();
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

		iListCount = m_listCtrl.GetItemCount();

		// update the indexes of the other items
		if (lHintData < iListCount)
		{
			// go through the rest of the list and fix the indexes on the items
			for (int i = iListCount - 1; i >= 0; i--)
			{
				pPropListData = (CPropListData *) m_listCtrl.GetItemData(i);
				if (pPropListData->m_iIndex > lHintData)
				{
					pPropListData->m_iIndex--;

					strIndex.Format("%d", pPropListData->m_iIndex);

					lvi.mask = LVIF_TEXT;
					lvi.iItem = i;
					lvi.iSubItem = 0;
					lvi.pszText = strIndex.GetBuffer(strIndex.GetLength() + 1);
					lvi.cchTextMax = strIndex.GetLength() + 1;
					m_listCtrl.SetItem(&lvi);
				}
				else if (pPropListData->m_iIndex == lHintData)
					m_listCtrl.DeleteItem(i);
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
		m_listCtrl.DeleteItem(lHintData);

		// ok, we should be able to add it to the list control
		hr = m_pPropList->get_Property(lHintData, &varProperty);
		if (FAILED(hr)) goto failexit;

		AddVariantItem( lHintData, varProperty );
	}
	else
	{
		// when would this ever be called?
		ClearList();
		FillList();
	}

failexit:
	return hr;
}


// client events
HRESULT CPropListCtrl::HandleOnDisconnect(IDispatch* pWorld)
{
	SetTargetObjectProperty(NULL);

	return CVWUIView::HandleOnDisconnect(pWorld);
}

HRESULT CPropListCtrl::HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient)
{
	IThing *pUser = NULL;
	HRESULT hr=S_OK;

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

// ClearList
// Empties the items in the list control, freeing memory as necessary
void CPropListCtrl::ClearList()
{
	m_listCtrl.DeleteAllItems();
	m_listCtrl.DeleteEditors();
}

// AddVariantItem
// given a Variant, and an Index for it creates a new ListCtrl Item and adds it
void CPropListCtrl::AddVariantItem(long iIndex, COleVariant varProperty)
{
	BOOL bReturn;

	CString strType, strValue, strIndex;
	int iSubItem = 0;
	int iActualItem;
	LV_ITEM lvi;

	bReturn = GetTypeAndValueFromVariant(varProperty, strType, strValue);
	if (!bReturn) return;

	if (strType.IsEmpty())
		strType.LoadString(IDS_TYPE_OBJECT);
	if (strValue.IsEmpty())
		strValue.LoadString(IDS_VALUE_EMPTY);

	CPropListData * pPropListData;
	pPropListData = new CPropListData(iIndex, strType, strValue, varProperty.vt);

	pPropListData->m_bEditable = m_bCanPutOnList;

	strIndex.Format("%d", iIndex);

	lvi.mask = LVIF_TEXT | LVIF_PARAM;
	lvi.iItem = iIndex;
	lvi.iSubItem = iSubItem++;
	lvi.pszText = strIndex.GetBuffer(strIndex.GetLength() + 1);
	lvi.cchTextMax = strIndex.GetLength() + 1;
	lvi.lParam = (LPARAM) pPropListData;
	iActualItem = m_listCtrl.InsertItem(&lvi);

	// set the type string for the item
	lvi.mask = LVIF_TEXT;
	lvi.iItem = iActualItem;
	lvi.iSubItem = iSubItem++;
	lvi.pszText = strType.GetBuffer(strType.GetLength() + 1);
	lvi.cchTextMax = strType.GetLength() + 1;
	m_listCtrl.SetItem(&lvi);

	// set the value string for the item
	lvi.mask = LVIF_TEXT;
	lvi.iItem = iActualItem;
	lvi.iSubItem = iSubItem++;
	lvi.pszText = strValue.GetBuffer(strValue.GetLength() + 1);
	lvi.cchTextMax = strValue.GetLength() + 1;
	m_listCtrl.SetItem(&lvi);
}

// FindItemByIndex
// since we can sort the list, the index of an item in the list
// control can be different that the index of an item in the
// property list, this function takes an index from a 
// property list and returns the PropListData class that
// corresponds to it
int CPropListCtrl::FindItemByIndex(long lIndex)
{
	int iListCount;
	CPropListData *cpld = NULL;

	if (!m_pPropList)
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


// FillList
// Called when a new Target Object Property is Set
// Iterates through the propList of the Thing and adds the items to the
// List Control
void CPropListCtrl::FillList()
{
	// REVIEW: handle Security Checking when that stuff is done

	if (!m_pPropList)
	{
		// if the thing is null it means that the list should be emptied
		ClearList();
		return;
	}

	HRESULT hr = S_OK;
	long iCount;

	if (FAILED(hr = m_pPropList->get_Count(&iCount)))
		return;

	COleVariant varProperty;

	for (long i = 0; i < iCount; i++)
	{
		varProperty.Clear();
		if (NONSECURITYFAIL(hr = m_pPropList->get_Property(i, &varProperty)))
			return;

		if (SUCCEEDED(hr))
			AddVariantItem(i, varProperty );
	}
}

// LaunchExternalEditor
// iItem - item that the button was on
void CPropListCtrl::LaunchExternalEditor(int iItem)
{
	FireEditValue(m_pPropList, iItem);
}


//
// LeftClick
// iItem - item that was clicked on
// iColumn - column the double click was in
//
// called when a user double clicks in the list control
void CPropListCtrl::LeftClick(int iItem, int iColumn)
{
	CPropListData *cpld = NULL;
	UINT state;

	if (iItem < 0)
		return;

	cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	if (!cpld)
		return;

	if (!m_bCanPutOnList)
		return;

	// check to see if the item was already selected
	state = m_listCtrl.GetItemState(iItem, LVIS_SELECTED);
	if (state == LVIS_SELECTED)
	{
		switch (iColumn)
		{
			case 0:
				break;
			case 1:
				break;
			case 2:
				// do nothing
				if (Editable(cpld->m_vtType))
				{
					if (cpld->m_vtType != VT_BOOL)
						m_listCtrl.EditValue(iItem, cpld->m_strValue);
					else
						m_listCtrl.EditValue(iItem, (cpld->m_strValue.CompareNoCase("true") == 0));
				}
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
void CPropListCtrl::DoubleClick(int iItem, int iColumn)
{
	if (iItem < 0)
		return;

	CPropListData *cpld = NULL;
	cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	if (!cpld)
		return;

	if (!m_bCanPutOnList)
		return;

	if (Editable(cpld->m_vtType))
	{
		if (cpld->m_vtType != VT_BOOL)
			m_listCtrl.EditValue(iItem, cpld->m_strValue);
		else
			m_listCtrl.EditValue(iItem, (cpld->m_strValue.CompareNoCase("true") == 0));
	}
	else
		FireEditValue(m_pPropList, iItem);
}

//
// GetNameOfSelectedItem
//
// finds the first selected item and returns it's name
int CPropListCtrl::GetIndexOfSelectedItem()
{
	int iItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);

	return iItem;
}

int CPropListCtrl::GetIndexOfItem(long pos)
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
void CPropListCtrl::PopUpMenu(CPoint point)
{
	CMenu menu;
	CMenu *pPopup;
	
	if (!menu.LoadMenu(IDR_PROP_LIST_MENU))
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

void CPropListCtrl::VirtualKeyPressed(UINT nChar)
{
	switch(nChar)
	{
		case VK_RETURN:
			OnEditValue();
			break;
		case VK_DELETE:
			OnDeleteProperty();
			break;
		default:
			FireKeyPressed(nChar);
			break;
	}
}

void CPropListCtrl::VirtualKeyUp(UINT nChar)
{
	FireVKeyUp(nChar);
}

void CPropListCtrl::FireSelectionEvent(BOOL bSelections)
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
void CPropListCtrl::AlphaKeyPressed(char cLetter)
{
	// walk through items in list
	int iListCount;
	CPropListData *cpld = NULL;
	CString strCompare;

	if (!m_pPropList)
		return;

	if ((m_listCtrl.m_iSortColumn < 0) || (m_listCtrl.m_iSortColumn > 2))
		return;

	iListCount = m_listCtrl.GetItemCount();

	// go through the rest of the list and fix the indexes on the items
	for (int i = 0; i < iListCount; i++)
	{
		cpld = (CPropListData *) m_listCtrl.GetItemData(i);
		switch(m_listCtrl.m_iSortColumn)
		{
			case 0:
			case 1:
				strCompare = cpld->m_strType;
				break;
			case 2:
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

void CPropListCtrl::DoneEditingValue(int iItem, CString strNewValue)
{
	CPropListData *pld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	CComVariant newVarValue;

	if (!pld)
		return;

	ConvertStringToVariant(pld->m_vtType, strNewValue, newVarValue);

	// put the value of the variant into the propList
	m_pPropList->put_Property(iItem, newVarValue);
}

void CPropListCtrl::DoFocus() 
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

void CPropListCtrl::MoveSelectedPropertyUp() 
{
	OnMovePropUp();
}

void CPropListCtrl::MoveSelectedPropertyDown() 
{
	OnMovePropDown();
}

void CPropListCtrl::RemoveSelectedProperty() 
{
	OnDeleteProperty();
}
