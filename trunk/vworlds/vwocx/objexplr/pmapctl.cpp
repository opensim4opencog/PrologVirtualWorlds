// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// PMapCtl.cpp : Implementation of the CPropMapCtrl ActiveX Control class.

#include "stdafx.h"
#include "objexplr.h"
#include "PrpEdCtl.h"
#include "PMapCtl.h"
#include "listdata.h"

#include <propbase.h>

#include "PExistDl.h"

#include "syshelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CPropMapCtrl, CPropEdCtrl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CPropMapCtrl, CPropEdCtrl)
	//{{AFX_MSG_MAP(CPropMapCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_ADD_PROPERTY, OnAddProperty)
	ON_COMMAND(ID_DELETE_PROPERTY, OnDeleteProperty)
	ON_UPDATE_COMMAND_UI(ID_DELETE_PROPERTY, OnUpdateDeleteProperty)
	ON_COMMAND(ID_EDIT_NAME, OnEditName)
	ON_UPDATE_COMMAND_UI(ID_EDIT_NAME, OnUpdateEditName)
	ON_COMMAND(ID_EDIT_VALUE, OnEditValue)
	ON_UPDATE_COMMAND_UI(ID_EDIT_VALUE, OnUpdateEditValue)
	ON_UPDATE_COMMAND_UI(ID_ADD_PROPERTY, OnUpdateAddProperty)
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CPropMapCtrl, CPropEdCtrl)
	//{{AFX_DISPATCH_MAP(CPropMapCtrl)
	DISP_PROPERTY_EX(CPropMapCtrl, "SelectionList", GetSelectionList, SetSelectionList, VT_DISPATCH)
	DISP_PROPERTY_EX(CPropMapCtrl, "TargetObjectProperty", GetTargetObjectProperty, SetTargetObjectProperty, VT_DISPATCH)
	DISP_PROPERTY_EX(CPropMapCtrl, "PropertyName", GetPropertyName, SetPropertyName, VT_BSTR)
	DISP_FUNCTION(CPropMapCtrl, "Sort", ExternalSort, VT_BOOL, VTS_I2)
	DISP_FUNCTION(CPropMapCtrl, "DoFocus", DoFocus, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CPropMapCtrl, "RemoveSelectedProperty", RemoveSelectedProperty, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CPropMapCtrl, CPropEdCtrl)
	//{{AFX_EVENT_MAP(CPropMapCtrl)
	EVENT_CUSTOM("EditValue", FireEditValue, VTS_DISPATCH  VTS_BSTR)
	EVENT_CUSTOM("NewProperty", FireNewProperty, VTS_DISPATCH)
	EVENT_CUSTOM("UserMessage", FireUserMessage, VTS_BSTR)
	EVENT_CUSTOM("KeyPressed", FireKeyPressed, VTS_I4)
	EVENT_CUSTOM("VKeyUp", FireVKeyUp, VTS_I4)
	EVENT_CUSTOM("ItemSelected", FireItemSelected, VTS_NONE)
	EVENT_CUSTOM("NoSelections", FireNoSelections, VTS_NONE)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CPropMapCtrl, "OBJEXPLR.PropMapCtrl.1",
	0xef7d6569, 0x161, 0x11d2, 0x88, 0xbe, 0, 0xc0, 0x4f, 0xc3, 0x2e, 0xf3)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CPropMapCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DPropMap =
		{ 0xef7d6567, 0x161, 0x11d2, { 0x88, 0xbe, 0, 0xc0, 0x4f, 0xc3, 0x2e, 0xf3 } };
const IID BASED_CODE IID_DPropMapEvents =
		{ 0xef7d6568, 0x161, 0x11d2, { 0x88, 0xbe, 0, 0xc0, 0x4f, 0xc3, 0x2e, 0xf3 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwPropMapOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CPropMapCtrl, IDS_PROPMAP, _dwPropMapOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CPropMapCtrl::CPropMapCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CPropMapCtrl

BOOL CPropMapCtrl::CPropMapCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_PROPMAP,
			IDB_PROPMAP,
			afxRegApartmentThreading,
			_dwPropMapOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CPropMapCtrl::CPropMapCtrl - Constructor

CPropMapCtrl::CPropMapCtrl()
{
	InitializeIIDs(&IID_DPropMap, &IID_DPropMapEvents);

	m_pThing = NULL;
	m_pPropMap = NULL;

	// add world events
	m_ulFlags |= ADVISE_WORLD;

	m_bCanAddToMap = FALSE;
	m_bCanPutOnMap = FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CPropMapCtrl::~CPropMapCtrl - Destructor

CPropMapCtrl::~CPropMapCtrl()
{
	SAFERELEASE(m_pThing);
	SAFERELEASE(m_pPropMap);
}


/////////////////////////////////////////////////////////////////////////////
// CPropMapCtrl::OnDraw - Drawing function

void CPropMapCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	DoSuperclassPaint(pdc, rcBounds);
}

BOOL CPropMapCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY*)lParam;
	
	switch(phdn->hdr.code)
	{
		case HDN_ITEMCLICK:
		case HDN_DIVIDERDBLCLICK:
		case HDN_ITEMCHANGING:
//			m_listCtrl.DeleteEditors();
			m_listCtrl.CommitEditors();
			break;
	}

	return CVWUIView::OnNotify(wParam, lParam, pResult);
}

/////////////////////////////////////////////////////////////////////////////
// CPropMapCtrl::DoPropExchange - Persistence support

void CPropMapCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	CPropEdCtrl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CPropMapCtrl::OnResetState - Reset control to default state

void CPropMapCtrl::OnResetState()
{
	CPropEdCtrl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CPropMapCtrl message handlers

int CPropMapCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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

	if (m_pPropMap)
		FillList();

	return 0;
}

void CPropMapCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CPropEdCtrl::OnSize(nType, cx, cy);
	
	if ( m_listCtrl.m_hWnd )
		m_listCtrl.SetWindowPos(&wndTop,0,0,cx,cy,SWP_NOZORDER );
	
}

LPDISPATCH CPropMapCtrl::GetSelectionList() 
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
		hr = pPropList->AddString(CComBSTR(pld->m_strName));
		if (FAILED(hr))
			break;

		iItem = m_listCtrl.GetNextItem(iItem, LVNI_SELECTED);
	}

	return pPropList;

FAIL_EXIT:
	return NULL;
}

void CPropMapCtrl::SetSelectionList(LPDISPATCH newValue) 
{
	IPropertyList *pPropList = NULL;
	HRESULT hr = S_OK;
	long iCount;
	CComBSTR bstrItem;
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
		if (NONSECURITYFAIL(hr = pPropList->get_String(i, &bstrItem.m_str)))
			goto exit;

		if (SUCCEEDED(hr))
		{
			// select the item
			iListIndex = m_listCtrl.FindItemByName(CString(bstrItem));
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

LPDISPATCH CPropMapCtrl::GetTargetObjectProperty() 
{
	SAFEADDREF(m_pPropMap);
	return m_pPropMap;
}

void CPropMapCtrl::SetTargetObjectProperty(LPDISPATCH newValue) 
{
	// make sure it is a PropMap pointer
	IPropertyMap *pPropMap = NULL;
	HRESULT hr = S_OK;
	CComBSTR	bstrName;

	if (newValue == NULL)
	{
		ClearList();
		SAFERELEASE(m_pPropMap);
		SAFERELEASE(m_pThing);
		m_pThing = NULL;
		m_pPropMap = NULL;
		return;
	}

	if (FAILED( hr = newValue->QueryInterface( IID_IPropertyMap, (LPVOID*)&pPropMap) ) )
		return;

	if (!pPropMap)
		return;

	if (m_pPropMap)
		SAFERELEASE(m_pPropMap);

	m_pPropMap = pPropMap;

	if (m_pThing)
		SAFERELEASE(m_pThing);

	hr = m_pPropMap->get_Parent(&m_pThing);
	if (!m_pThing || FAILED(hr))
	{
		SAFERELEASE(m_pThing);
		m_pThing = NULL;
	}
	
	SetModifiedFlag();

	VARIANT_BOOL vBool = VARIANT_FALSE;

	m_bCanAddToMap = FALSE;
	m_bCanPutOnMap = FALSE;

	if (FAILED(hr = m_pPropMap->get_PropertyName(&bstrName.m_str)))
		return;

	m_strPropName = bstrName;

	if (!m_pThing)
		return;

	if (FAILED(hr = m_pThing->CheckPropertySecurity(bstrName, PS_WRITE, &vBool)))
		return;

	m_bCanPutOnMap = (vBool == VARIANT_TRUE);

	if (FAILED(hr = m_pThing->CheckPropertySecurity(bstrName, PS_ADD, &vBool)))
		return;

	m_bCanAddToMap = (vBool == VARIANT_TRUE);

	if (m_listCtrl.m_hWnd)
		FillList();
}

BSTR CPropMapCtrl::GetPropertyName() 
{
	CString strResult;

	strResult = m_strPropName;

	return strResult.AllocSysString();
}

void CPropMapCtrl::SetPropertyName(LPCTSTR lpszNewValue) 
{
	m_strPropName = lpszNewValue;

	SetModifiedFlag();
}

BOOL CPropMapCtrl::ExternalSort(short column) 
{
	if ((column >= 0) && (column <= 2))
	{
		m_listCtrl.m_iSortColumn = column;
		m_listCtrl.Sort();
	}

	return TRUE;
}


//
// Class Functions
//

HRESULT CPropMapCtrl::HandleOnPropertyChanged(IThing *pthing, BSTR bstr, long lHint, VARIANT varHintData)
{
	HRESULT hr = S_OK;
	CString strName;
	IObjectProperty *pProp = NULL;
	IPropertyMap *pMap = NULL;
	CComBSTR bstrHintData;
	VARIANT_BOOL vBool = VARIANT_TRUE;
	CPropListData * pPropListData = NULL;

	if (pthing != m_pThing)
		return hr;

	// this is a message for our thing, but does it affect us?
	strName = bstr;
	if (strName.CompareNoCase(m_strPropName) != 0)
		return hr;

	if (varHintData.vt = VT_BSTR)
		bstrHintData = varHintData.bstrVal;

	CString strHintData = bstrHintData;

	int i = 0;
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


		hr = pProp->QueryInterface(IID_IPropertyMap, (LPVOID*)&pMap);
		if (FAILED(hr))
		{
			SAFERELEASE(pMap);
			return hr;
		}

		// fire some error or warning message...
		CString strError;
		strError.LoadString(IDS_MSG_MAP_REPLACED);
		FireUserMessage(strError);

		SetTargetObjectProperty(pMap);
		SAFERELEASE(pProp);
		SAFERELEASE(pMap);
	}
	else if (lHint == thRemove)
	{
		// has the property we are working with been removed?
		ClearList();
		SAFERELEASE(m_pPropMap);
		SAFERELEASE(m_pThing);
		
		// fire some error or warning message...
		CString strError;
		strError.LoadString(IDS_MSG_MAP_GONE);
		FireUserMessage(strError);
	}
	else if (lHint == pmPut)
	{
		// has a property been added to the map?

		if (strHintData.IsEmpty())
		{
			ClearList();
			FillList();
			return hr;
		}

		// see if it is already there
		hr = m_pPropMap->IsValid( bstrHintData, &vBool );
		if (FAILED(hr)) goto failexit;

		if (vBool == VARIANT_TRUE)
		{
			// locate the thingy
			for (i=0; i < m_listCtrl.GetItemCount(); i++)
			{
				pPropListData = (CPropListData *) m_listCtrl.GetItemData(i);
				if (pPropListData->m_strName == strHintData)
				{
					m_listCtrl.DeleteItem(i);
					break;
				}
			}
		}

		// add it
		CComVariant varProperty;
		hr = m_pPropMap->get_Property(bstrHintData, &varProperty);
		if (FAILED(hr)) goto failexit;

		AddVariantItem( varProperty, strHintData );
		m_listCtrl.Sort();
	}
	else if (lHint == pmRemove)
	{
		// a property has been removed from the map
		if (strHintData.IsEmpty())
		{
			ClearList();
			FillList();
			goto failexit;
		}
		// locate the thingy
		for (i=0; i < m_listCtrl.GetItemCount(); i++)
		{
			pPropListData = (CPropListData *) m_listCtrl.GetItemData(i);
			if (pPropListData->m_strName == strHintData)
			{
				m_listCtrl.DeleteItem(i);
				break;
			}
		}
	}
	else if (lHint == pmRemoveAll)
	{
		// all the properties have been removed from the map
		ClearList();
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
HRESULT CPropMapCtrl::HandleOnDisconnect(IDispatch* pWorld)
{
	SetTargetObjectProperty(NULL);

	return CVWUIView::HandleOnDisconnect(pWorld);
}

HRESULT CPropMapCtrl::HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient)
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
void CPropMapCtrl::ClearList()
{
	m_listCtrl.DeleteAllItems();
}

// AddVariantItem
// given a Variant and the name for it, creates a new ListCtrl Item and adds it
void CPropMapCtrl::AddVariantItem(COleVariant varProperty, CString strName)
{
	BOOL bReturn;

	// If no name that would be bad
	ASSERT( !strName.IsEmpty() );

	CString strType, strValue;
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
	pPropListData = new CPropListData(strName, strType, strValue, varProperty.vt);

	pPropListData->m_bEditable = m_bCanPutOnMap;

	// set the name for the item
	lvi.mask = LVIF_TEXT | LVIF_PARAM;
	lvi.iItem = 0;
	lvi.iSubItem = iSubItem++;
	lvi.pszText = strName.GetBuffer(strName.GetLength() + 1);
	lvi.cchTextMax = strName.GetLength() + 1;
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

// FillList
// Called when a new Target Object Property is Set
// Iterates through the property map and adds the items to the
// List Control
void CPropMapCtrl::FillList()
{
	// REVIEW: handle Security Checking when that stuff is done

	if (!m_pPropMap)
	{
		// if the thing is null it means that the list should be emptied
		ClearList();
		return;
	}

	HRESULT hr = S_OK;
	CString cstrName;
	BSTR bstrReturnValue = NULL;

	// yes, iterate through properties
	VARIANT_BOOL fLastItem = VARIANT_FALSE;
	COleVariant varProperty;

	hr = m_pPropMap->get_IsEmpty( &fLastItem );
	if (FAILED(hr)) goto failexit;

	if( ! fLastItem )
	{
		varProperty.Clear();
		
		hr = m_pPropMap->FirstItem( &bstrReturnValue, &varProperty, &fLastItem );
		if (NONSECURITYFAIL(hr))
			goto failexit;

		while( !(fLastItem == VARIANT_TRUE))
		{
			// Get the name of the Item in the PropertyMap.
			cstrName = bstrReturnValue;

			if (SUCCEEDED(hr))
				AddVariantItem( varProperty, cstrName );

			SAFEFREESTRING(bstrReturnValue);

			hr = m_pPropMap->NextItem( &bstrReturnValue, &varProperty, &fLastItem );
			if (NONSECURITYFAIL(hr))
				goto failexit;
		}
	}

	m_listCtrl.Sort();

failexit:
	SAFEFREESTRING(bstrReturnValue);
}

// LaunchExternalEditor
// iItem - item that the button was on
void CPropMapCtrl::LaunchExternalEditor(int iItem)
{
	CPropListData *cpld = NULL;
	cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	if (!cpld)
		return;

	FireEditValue(m_pPropMap, cpld->m_strName);
}

//
// LeftClick
// iItem - item that was clicked on
// iColumn - column the double click was in
//
// called when a user double clicks in the list control
void CPropMapCtrl::LeftClick(int iItem, int iColumn)
{
	CPropListData *cpld = NULL;
	UINT state;

	if (iItem < 0)
		return;

	cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	if (!cpld)
		return;

	if (!m_bCanPutOnMap)
		return;

	// check to see if the item was already selected
	state = m_listCtrl.GetItemState(iItem, LVIS_SELECTED);
	if (state == LVIS_SELECTED)
	{
		switch (iColumn)
		{
			case 0:
				if (!m_bCanAddToMap)
					return;
				
				m_listCtrl.EditName(iItem, cpld->m_strName, 0);
				break;
			case 1:
				// do nothing
				break;
			case 2:
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
void CPropMapCtrl::DoubleClick(int iItem, int iColumn)
{
	if (iItem < 0)
		return;

	CPropListData *cpld = NULL;
	cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	if (!cpld)
		return;

	if (!m_bCanPutOnMap)
		return;

	if (Editable(cpld->m_vtType))
	{
		if (cpld->m_vtType != VT_BOOL)
			m_listCtrl.EditValue(iItem, cpld->m_strValue);
		else
			m_listCtrl.EditValue(iItem, (cpld->m_strValue.CompareNoCase("true") == 0));
	}
	else
		FireEditValue(m_pPropMap, cpld->m_strName);
}

//
// DoneEditingName
// the list ctrl is done editing the value of a property
//
void CPropMapCtrl::DoneEditingName(int iItem, CString strNewValue)
{
	CComVariant varProp;
	HRESULT hr = S_OK;
	VARIANT_BOOL vBool = VARIANT_FALSE;
	CString strError;
	CPropListData *cpld = NULL;

	if (!m_pThing)
		return;

	cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	if (!cpld)
		return;

	if (strNewValue.CompareNoCase(cpld->m_strName) == 0)
		return;

	// check to see if there is already a prop by the new name
	if (FAILED(hr = m_pPropMap->IsValid(CComBSTR(strNewValue), &vBool)))
		goto exit;

	if (vBool == VARIANT_TRUE)
	{
		if (!m_bCanPutOnMap)
		{
			strError.LoadString(IDS_ERROR_CANT_WRITE_PROP);
			FireUserMessage(strError);
			return;
		}

		CPropExistsDlog cped;
		if (cped.DoModal() != IDOK)
			return;
	}

	// get the prop out of the old slot
	if (FAILED(hr = m_pPropMap->get_Property(CComBSTR(cpld->m_strName), &varProp)))
		goto exit;
	
	// add it to the new slot
	if (FAILED(hr = m_pPropMap->put_Property(CComBSTR(strNewValue), varProp)))
		goto exit;

	// delete the old slot
	if (FAILED(hr = m_pPropMap->Remove(CComBSTR(cpld->m_strName))))
		goto exit;

exit:
	return;
}

void CPropMapCtrl::DoneEditingValue(int iItem, CString strNewValue)
{
	CPropListData *pld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	CComVariant newVarValue;

	if (!pld)
		return;

	ConvertStringToVariant(pld->m_vtType, strNewValue, newVarValue);

	// put the value of the variant into the propMap
	m_pPropMap->put_Property(CComBSTR(pld->m_strName), newVarValue);
}

//
// GetNameOfSelectedItem
//
// finds the first selected item and returns it's name
CString CPropMapCtrl::GetNameOfSelectedItem()
{
	CString strName;
	int iItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);
	if (iItem != -1)
	{

		LV_ITEM	item;
		char pszName[256];

		item.mask = LVIF_TEXT;
		item.iItem = iItem;
		item.iSubItem = 0;
		item.pszText = (char *) &pszName;
		item.cchTextMax = 256;
		m_listCtrl.GetItem(&item);

		strName = pszName;
	}

	return strName;
}

void CPropMapCtrl::FireSelectionEvent(BOOL bSelections)
{
	if (bSelections)
		FireItemSelected();
	else
		FireNoSelections();
}


//
// PopUpMenu
// point - location of the right click
//
// called when a user double clicks in the list control
void CPropMapCtrl::PopUpMenu(CPoint point)
{
	CMenu menu;
	CMenu *pPopup;
	
	if (!menu.LoadMenu(IDR_PROP_MAP_MENU))
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

//
// VirtualKeyPressed
//
// called when a user hits the one of the virtual keys in the list control
void CPropMapCtrl::VirtualKeyPressed(UINT nChar)
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

void CPropMapCtrl::VirtualKeyUp(UINT nChar)
{
	FireVKeyUp(nChar);
}

//
// AlphaKeyPressed
//
// called when a user presses a letter key in the list control
void CPropMapCtrl::AlphaKeyPressed(char cLetter)
{
	// walk through items in list
	int iListCount;
	CPropListData *cpld = NULL;
	CString strCompare;

	if (!m_pPropMap)
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
				strCompare = cpld->m_strName;
				break;
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

void CPropMapCtrl::OnAddProperty() 
{
	// Fire the add property
	FireNewProperty(m_pPropMap);
}

void CPropMapCtrl::OnUpdateAddProperty(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bCanAddToMap);
}


void CPropMapCtrl::OnDeleteProperty() 
{
	// Fire the Delete Property Event
	// FireDeleteProperty(m_pPropMap, NULL);
	// go through the list of properties and delete each selected one
	int iSelected = m_listCtrl.GetSelectedCount();
	if (iSelected == 0)
		return;

	CPtrArray cpaSelections;
	cpaSelections.SetSize(iSelected);
	int iItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);
	int i = 0;
	int iLastItem;
	
	while (iItem != -1)
	{
		iLastItem = iItem;
		cpaSelections.SetAt(i++, (CPropListData *) m_listCtrl.GetItemData(iItem));
		iItem = m_listCtrl.GetNextItem(iItem, LVNI_SELECTED);
	}

	HRESULT hr = S_OK;
	BSTR bstrName = NULL;

	for (i = cpaSelections.GetSize() - 1; i > -1; i--)
	{
		CPropListData *pld = (CPropListData *) cpaSelections.GetAt(i);
		if (!pld)
			break;

		// iItem is the selected item
		BOOL bFailed = FALSE;

		bstrName = pld->m_strName.AllocSysString();

		hr = m_pPropMap->Remove(bstrName);
		if (FAILED(hr))	goto failexit;
		iLastItem--;
	}

	m_listCtrl.SelectItem(iLastItem + 1);

failexit:
	// clean up
	SAFEFREESTRING(bstrName);
}


void CPropMapCtrl::OnUpdateDeleteProperty(CCmdUI* pCmdUI) 
{
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();

	if (uiSelected > 0)
		pCmdUI->Enable(m_bCanAddToMap);
	else
		pCmdUI->Enable(FALSE);
}

void CPropMapCtrl::OnEditName() 
{
	int iItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);
	
	if (!(iItem >= 0))
		return;

	CPropListData *cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	m_listCtrl.EditName(iItem, cpld->m_strName, 1);
}

void CPropMapCtrl::OnUpdateEditName(CCmdUI* pCmdUI) 
{
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();

	if (uiSelected == 1)
		pCmdUI->Enable(m_bCanPutOnMap && m_bCanAddToMap);
	else
		pCmdUI->Enable(FALSE);
}

void CPropMapCtrl::OnEditValue() 
{
	int iItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);
	
	if (!(iItem >= 0))
		return;

	CPropListData *cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);

	if (!cpld)
		return;

	if (!m_bCanPutOnMap)
		return;

	if (Editable(cpld->m_vtType))
	{
		if (cpld->m_vtType != VT_BOOL)
			m_listCtrl.EditValue(iItem, cpld->m_strValue);
		else
			m_listCtrl.EditValue(iItem, (cpld->m_strValue.CompareNoCase("true") == 0));
	}
	else
		FireEditValue(m_pPropMap, cpld->m_strName);
}

void CPropMapCtrl::OnUpdateEditValue(CCmdUI* pCmdUI) 
{
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();

	if (uiSelected == 1)
		pCmdUI->Enable(m_bCanPutOnMap);
	else
		pCmdUI->Enable(FALSE);
}

void CPropMapCtrl::DoFocus() 
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

void CPropMapCtrl::RemoveSelectedProperty() 
{
	OnDeleteProperty();
}
