// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// TPListCtl.cpp :  Implementation of the CTPListCtrl ActiveX Control class.
//
//					This Thing PropertyList control is used to edit the properties of
//					an IThing. 

#include "stdafx.h"
#include "objexplr.h"
#include "PrpEdCtl.h"
#include "TPLstCtl.h"
#include "listdata.h"

#include <propbase.h>
#include <vwevents.h>

#include "PExistDl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef CVWComPtr<IThing,NULL,&IID_IThing> CThingPtr;

#define ISWRITABLE(type) ((type != EXEMPLAR_PROPERTY_NO_WRITE) && (type != OVERRIDEN_PROPERTY_NO_WRITE) && (type != INSTANCE_PROPERTY_NO_WRITE) && (type != EXEMPLAR_PROPERTY_NO_WRITE_NO_EDIT) && (type != OVERRIDEN_PROPERTY_NO_WRITE_NO_EDIT) && (type != INSTANCE_PROPERTY_NO_WRITE_NO_EDIT))
#define ISOVERRIDEN(type) ((type == OVERRIDEN_PROPERTY) || (type == OVERRIDEN_PROPERTY_NO_WRITE) || (type == OVERRIDEN_PROPERTY_NO_EDIT) || (type == OVERRIDEN_PROPERTY_NO_WRITE_NO_EDIT))
#define ISEXEMPLAR(type) ((type == EXEMPLAR_PROPERTY) || (type == EXEMPLAR_PROPERTY_NO_WRITE) || (type == EXEMPLAR_PROPERTY_NO_EDIT) || (type == EXEMPLAR_PROPERTY_NO_WRITE_NO_EDIT))
#define ISINSTANCE(type) ((type == INSTANCE_PROPERTY) || (type == INSTANCE_PROPERTY_NO_WRITE) || (type == INSTANCE_PROPERTY_NO_EDIT) || (type == INSTANCE_PROPERTY_NO_WRITE_NO_EDIT))

#define NO_COPY_PROMPT

IMPLEMENT_DYNCREATE(CTPListCtrl, CPropEdCtrl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CTPListCtrl, CPropEdCtrl)
	//{{AFX_MSG_MAP(CTPListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_ADD_PROPERTY, OnAddProperty)
	ON_COMMAND(ID_DELETE_PROPERTY, OnDeleteProperty)
	ON_UPDATE_COMMAND_UI(ID_DELETE_PROPERTY, OnUpdateDeleteProperty)
	ON_COMMAND(ID_EDIT_NAME, OnEditName)
	ON_UPDATE_COMMAND_UI(ID_EDIT_NAME, OnUpdateEditName)
	ON_COMMAND(ID_EDIT_VALUE, OnEditValue)
	ON_UPDATE_COMMAND_UI(ID_EDIT_VALUE, OnUpdateEditValue)
	ON_COMMAND(ID_SHOW_EXEMPLAR_PROPERTIES, OnShowExemplarProperties)
	ON_UPDATE_COMMAND_UI(ID_SHOW_EXEMPLAR_PROPERTIES, OnUpdateShowExemplarProperties)
	ON_COMMAND(ID_SHOW_INSTANCE_PROPERTIES, OnShowInstanceProperties)
	ON_UPDATE_COMMAND_UI(ID_SHOW_INSTANCE_PROPERTIES, OnUpdateShowInstanceProperties)
	ON_UPDATE_COMMAND_UI(ID_ADD_PROPERTY, OnUpdateAddProperty)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CTPListCtrl, CPropEdCtrl)
	//{{AFX_DISPATCH_MAP(CTPListCtrl)
	DISP_PROPERTY_EX(CTPListCtrl, "SelectionList", GetSelectionList, SetSelectionList, VT_DISPATCH)
	DISP_PROPERTY_EX(CTPListCtrl, "TargetObjectProperty", GetTargetObjectProperty, SetTargetObjectProperty, VT_DISPATCH)
	DISP_PROPERTY_EX(CTPListCtrl, "RespondToSelectionEvents", GetRespondToSelectionEvents, SetRespondToSelectionEvents, VT_BOOL)
	DISP_FUNCTION(CTPListCtrl, "Sort", ExternalSort, VT_BOOL, VTS_I2)
	DISP_FUNCTION(CTPListCtrl, "DoFocus", DoFocus, VT_EMPTY, VTS_NONE)
	DISP_STOCKPROP_FONT()
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CTPListCtrl, CPropEdCtrl)
	//{{AFX_EVENT_MAP(CTPListCtrl)
	EVENT_CUSTOM("EditValue", FireEditValue, VTS_DISPATCH  VTS_BSTR)
	EVENT_CUSTOM("NewProperty", FireNewProperty, VTS_DISPATCH)
	EVENT_CUSTOM("UserMessage", FireUserMessage, VTS_BSTR)
	EVENT_CUSTOM("KeyPressed", FireKeyPressed, VTS_I4)
	EVENT_CUSTOM("VKeyUp", FireVKeyUp, VTS_I4)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CTPListCtrl, "OBJEXPLR.TPListCtrl.1",
	0xef7d655d, 0x161, 0x11d2, 0x88, 0xbe, 0, 0xc0, 0x4f, 0xc3, 0x2e, 0xf3)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CTPListCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DTPList =
		{ 0xef7d655b, 0x161, 0x11d2, { 0x88, 0xbe, 0, 0xc0, 0x4f, 0xc3, 0x2e, 0xf3 } };
const IID BASED_CODE IID_DTPListEvents =
		{ 0xef7d655c, 0x161, 0x11d2, { 0x88, 0xbe, 0, 0xc0, 0x4f, 0xc3, 0x2e, 0xf3 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwTPListOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CTPListCtrl, IDS_TPLIST, _dwTPListOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CTPListCtrl::CTPListCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CTPListCtrl

BOOL CTPListCtrl::CTPListCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_TPLIST,
			IDB_TPLIST,
			afxRegApartmentThreading,
			_dwTPListOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CTPListCtrl::CTPListCtrl - Constructor

CTPListCtrl::CTPListCtrl()
{
	InitializeIIDs(&IID_DTPList, &IID_DTPListEvents);

	m_pThing = NULL;

	m_lFilterType = FILTER_INSTANCE_PROPERTIES |
					FILTER_EXEMPLAR_PROPERTIES |
					FILTER_WRITABLE_PROPERTIES ;
					//FILTER_FRIENDLY_PROPERTIES ;
	
	m_pListIcons = NULL;
	m_bRespondToSelection = FALSE;

	// add world events
	m_ulFlags |= ADVISE_WORLD;

	m_bCanAddToThing = FALSE;
	m_bCanPutOnThing = FALSE;
	m_strLastSelectedProperty.Empty();
}


/////////////////////////////////////////////////////////////////////////////
// CTPListCtrl::~CTPListCtrl - Destructor

CTPListCtrl::~CTPListCtrl()
{
	// TODO: Cleanup your control's instance data here.
	SAFERELEASE(m_pThing);
	delete m_pListIcons;
}


/////////////////////////////////////////////////////////////////////////////
// CTPListCtrl::OnDraw - Drawing function

void CTPListCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// select the font
	CFont* pOldFont;
	pOldFont = SelectStockFont( pdc );
	DoSuperclassPaint(pdc, rcBounds);
	pdc->SelectObject(pOldFont);
}

BOOL CTPListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY*)lParam;
	
	switch(phdn->hdr.code)
	{
		case HDN_ITEMCLICK:
		case HDN_DIVIDERDBLCLICK:
		case HDN_ITEMCHANGING:
	//		m_listCtrl.DeleteEditors();
			m_listCtrl.CommitEditors();
			break;
	}

	return CVWUIView::OnNotify(wParam, lParam, pResult);
}

/////////////////////////////////////////////////////////////////////////////
// CTPListCtrl::DoPropExchange - Persistence support

void CTPListCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	CPropEdCtrl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CTPListCtrl::OnResetState - Reset control to default state

void CTPListCtrl::OnResetState()
{
	CPropEdCtrl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CTPListCtrl message handlers

LPDISPATCH CTPListCtrl::GetTargetObjectProperty() 
{
	SAFEADDREF(m_pThing);
	return m_pThing;
}

void CTPListCtrl::SetTargetObjectProperty(LPDISPATCH newValue) 
{
	// make sure it is a thing pointer
	IThing *pThing = NULL;
	HRESULT hr = S_OK;
	int iItem = 0;

	if (newValue == NULL)
	{
		ClearList();
		SAFERELEASE(m_pThing);
		m_pThing = NULL;
		m_bCanAddToThing = FALSE;
		m_bCanPutOnThing = FALSE;
		return;
	}

	if (FAILED( hr = newValue->QueryInterface( IID_IThing, (LPVOID*)&pThing) ) )
		return;

	if (!pThing)
		return;

	if (pThing == m_pThing)
		return;

	if (m_pThing)
		SAFERELEASE(m_pThing);

	m_pThing = pThing;

	CPropEdCtrl::SetTargetObjectProperty(newValue);

	// check to see if we have add permission on the props
	VARIANT_BOOL vBool;

	m_bCanAddToThing = FALSE;
	m_bCanPutOnThing = FALSE;

	if (SUCCEEDED(hr = m_pThing->CheckPropertiesSecurity(PS_ADD, &vBool)))
		m_bCanAddToThing = (vBool == VARIANT_TRUE);
	if (SUCCEEDED(hr = m_pThing->CheckPropertiesSecurity(PS_WRITE, &vBool)))
		m_bCanPutOnThing = (vBool == VARIANT_TRUE);

	SetModifiedFlag();

	if (m_listCtrl.m_hWnd)
	{
		ClearList();
		FillList();

		if (!m_strLastSelectedProperty.IsEmpty())
		{
			iItem = m_listCtrl.FindItemByName(m_strLastSelectedProperty);
			if (iItem < 0)
				iItem = 0;
		}
		
		m_listCtrl.SelectItem(iItem,0,FALSE);
	}
}

LPDISPATCH CTPListCtrl::GetSelectionList() 
{
	// TODO: Add your property handler here

	return NULL;
}

void CTPListCtrl::SetSelectionList(LPDISPATCH newValue) 
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
	// don't have to do this anymore, it's part of selectitem
	// m_listCtrl.ShowItemAndColumn(iListIndex, 0);

exit:

	SAFERELEASE(pPropList);
}

BOOL CTPListCtrl::GetRespondToSelectionEvents() 
{
	return m_bRespondToSelection;
}

void CTPListCtrl::SetRespondToSelectionEvents(BOOL bNewValue) 
{

	m_bRespondToSelection = bNewValue;

	SetModifiedFlag();
}

BOOL CTPListCtrl::ExternalSort(short column) 
{
	if ((column >= 0) && (column <= 3))
	{
		m_listCtrl.m_iSortColumn = column;
		m_listCtrl.Sort();
	}

	return TRUE;
}


//
// windows event handlers
//
int CTPListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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
	
	if (!m_pListIcons)
	{
		m_pListIcons = new CImageList;
		m_pListIcons->Create(IDB_PROP_ICONS, 16, 0, 0x00FFFFFF);
	}

	m_listCtrl.SetImageList(m_pListIcons, LVSIL_SMALL);

	m_listCtrl.m_pOCX = this;

	if (m_pThing)
	{
		int iItem;

		ClearList();
		FillList();

		if (!m_strLastSelectedProperty.IsEmpty())
		{
			iItem = m_listCtrl.FindItemByName(m_strLastSelectedProperty);
			if (iItem < 0)
				iItem = 0;
		}
		
		m_listCtrl.SelectItem(iItem,0,FALSE);
	}

	return 0;
}

void CTPListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CPropEdCtrl::OnSize(nType, cx, cy);
	
	if ( m_listCtrl.m_hWnd )
		m_listCtrl.SetWindowPos(&wndTop,0,0,cx,cy,SWP_NOZORDER );
}

//
// Class Functions
//

// ClearList
// Empties the items in the list control, freeing memory as necessary
void CTPListCtrl::ClearList()
{
	m_listCtrl.DeleteAllItems();
}

// AddVariantItem
// given a Variant and the name for it, creates a new ListCtrl Item and adds it
void CTPListCtrl::AddItem(IPropertySecurity *pSecurity, CString strName, IconType &type)
{
	BOOL bReturn;
	BOOL bEditable;
	VARTYPE varType;
	VARIANT_BOOL vBool;
	HRESULT hr = S_OK;

	// 	vBool = QUIET_SAFE_CHECKSECURITY(m_pWorld, pSecurity, PS_WRITE);
	if (FAILED(hr = m_pThing->CheckPropertySecurity(CComBSTR(strName), PS_WRITE, &vBool)))
		return;

	bEditable = (vBool == VARIANT_TRUE);

	// now if this is an exemplar property and we can put on the object, then really it is editable
	//if ((type == EXEMPLAR_PROPERTY) && m_bCanPutOnThing)
	//	bEditable = TRUE;

	// If no name that would be bad
	ASSERT( !strName.IsEmpty() );

	CString strType, strValue;
	int iSubItem = 0;
	int iActualItem;
	LV_ITEM lvi;

//	pSecurity->get_vtType(&varType);

	bReturn = GetTypeAndValueFromSecurity(pSecurity, strType, strValue, varType);
	if (!bReturn) return;

	if (!bEditable)
	{
		if (type == INSTANCE_PROPERTY)
			type = INSTANCE_PROPERTY_NO_WRITE;
		if (type == OVERRIDEN_PROPERTY)
			type = OVERRIDEN_PROPERTY_NO_WRITE;
		if (type == EXEMPLAR_PROPERTY)
			type = EXEMPLAR_PROPERTY_NO_WRITE;
	}

	// check to see if we this will be filtered out
	if (m_pThing &&
		(((type == INSTANCE_PROPERTY) && !(m_lFilterType & FILTER_INSTANCE_PROPERTIES)) ||
		((type == OVERRIDEN_PROPERTY) && !(m_lFilterType & FILTER_INSTANCE_PROPERTIES)) ||
		((type == EXEMPLAR_PROPERTY) && !(m_lFilterType & FILTER_EXEMPLAR_PROPERTIES)) ||
		((type == INSTANCE_PROPERTY_NO_WRITE) && !(m_lFilterType & FILTER_WRITABLE_PROPERTIES)) ||
		((type == OVERRIDEN_PROPERTY_NO_WRITE) && !(m_lFilterType & FILTER_WRITABLE_PROPERTIES)) ||
		((type == EXEMPLAR_PROPERTY_NO_WRITE) && !(m_lFilterType & FILTER_WRITABLE_PROPERTIES))))
		return;

	if ((varType == VT_DISPATCH) || (varType == VT_UNKNOWN))
	{
		// check to see if this is one of the properties that aren't editable through current UI
		if (!EditableType(strType))
		{
			CThingPtr testThing;
			CComVariant testVar;

			// make sure it isn't just a thing (which can have all sorts of crazy type names
			if (FAILED(hr = m_pThing->get_Property(CComBSTR(strName), &testVar)))
				bEditable = FALSE;
			else
			{
				BOOL bTest;
				BOOL bExemplar = TRUE;
				testThing = testVar;
				bTest = testThing != NULL;
				if (!bTest)
				{
					type = (enum IconType) (((int) type) + 6);
					bEditable = FALSE;
				}
				else
				{
					// make sure it isn't an exemplar, because we can't edit those
					if (FAILED(hr = testThing->get_IsExemplar(&vBool)))
						bEditable = FALSE;
					else
						bExemplar = (vBool == VARIANT_TRUE);

					if (bExemplar)
					{
						type = (enum IconType) (((int) type) + 6);
						bEditable = FALSE;
					}
				}
			}
		}
	}

	CPropListData * pPropListData;
	pPropListData = new CPropListData(strName, strType, strValue, type, varType, bEditable);

	// set the icon for the item
	lvi.mask = LVIF_IMAGE | LVIF_PARAM;
	lvi.iItem = 0;
	lvi.iSubItem = iSubItem++;
	lvi.iImage = type;
	lvi.lParam = (LPARAM) pPropListData;
	iActualItem = m_listCtrl.InsertItem(&lvi);

	// set the name for the item
	lvi.mask = LVIF_TEXT;
	lvi.iItem = iActualItem;
	lvi.iSubItem = iSubItem++;
	lvi.pszText = strName.GetBuffer(strName.GetLength() + 1);
	lvi.cchTextMax = strName.GetLength() + 1;
	m_listCtrl.SetItem(&lvi);

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

	if (type > 6)
		m_listCtrl.SetItemState(iActualItem, LVIS_CUT, LVIS_CUT);

}

// FillList
// Called when a new Target Object Property is Set
// Iterates through the propList of the Thing and adds the items to the
// List Control
void CTPListCtrl::FillList()
{
	// REVIEW: handle Security Checking when that stuff is done

	if (!m_pThing)
	{
		// if the thing is null it means that the list should be emptied
		ClearList();
		return;
	}

	HRESULT hr = S_OK;
	CString cstrName;
	IThing * pExemplar = NULL;
	IPropertyMap * pInstancePropertyMap = NULL;
	IPropertyMap * pExemplarPropertyMap = NULL;
	IPropertySecurity *pSecurity = NULL;
	BSTR bstrReturnValue = NULL;
	long lBits = 0;

	// Does it have a Properties PropertyMap?
	hr = m_pThing->get_InstanceProperties( &pInstancePropertyMap );
	if (FAILED(hr)) goto failexit;

	// does it have an exemplar
	hr = m_pThing->get_Exemplar( &pExemplar );
	if (FAILED(hr)) goto failexit;

	if (pExemplar)
	{
		hr = pExemplar->get_Properties( &pExemplarPropertyMap );
		if (NONSECURITYFAIL(hr)) goto failexit;
		if (SECURITYFAIL(hr))
			pExemplarPropertyMap = NULL;
	}

	// yes, iterate through properties if the filter is set to show them
	if (pInstancePropertyMap && (m_lFilterType & FILTER_INSTANCE_PROPERTIES))
	{
		VARIANT_BOOL fLastItem = VARIANT_FALSE;
		COleVariant varProperty;
		BOOL bFriendly = TRUE;
		BOOL bHidden = FALSE;
		BOOL bConstant = FALSE;

		hr = pInstancePropertyMap->get_IsEmpty( &fLastItem );
		if (FAILED(hr)) goto failexit;

		if( ! (fLastItem == VARIANT_TRUE) )
		{
			varProperty.Clear();

			// RK: First,NextItem don't check for security, don't need to use NONSECURITYFAIL
			if (FAILED(hr = pInstancePropertyMap->FirstItem( &bstrReturnValue, &varProperty, &fLastItem )))
				goto failexit;

			while( !(fLastItem == VARIANT_TRUE))
			{
				// Get the name of the Item in the PropertyMap.
				cstrName = bstrReturnValue;

				// get the security pointer (inside here is where we check for READ permissions)
				hr = m_pThing->get_PropertySecurity(bstrReturnValue, &pSecurity );
				
				// RK: If succeeded (we have read permissions), add the item
				if (SUCCEEDED(hr))
				{
					// get the bits out of the security pointer
					if (FAILED(hr = pSecurity->get_Bits(&lBits)))
						goto failexit;

					bHidden = (lBits & PSBIT_HIDDEN);
					bFriendly = (lBits & PSBIT_FRIENDLY);
					bConstant = (lBits & PSBIT_CONSTANT);

					if (!bHidden && !bConstant)
					{
						enum IconType type = INSTANCE_PROPERTY;
						VARIANT_BOOL vBool;

						// check to see if this item is overriding an exemplar prop
						if (pExemplarPropertyMap)
						{
							hr = pExemplarPropertyMap->IsValid(bstrReturnValue, &vBool);
							if (FAILED(hr)) goto failexit;

							if (vBool == VARIANT_TRUE)
								type = OVERRIDEN_PROPERTY;
						}

						
						if (m_lFilterType & FILTER_FRIENDLY_PROPERTIES)
						{
							if (bFriendly)
								AddItem( pSecurity, cstrName,  type);
						}
						else
							AddItem( pSecurity, cstrName,  type);
					}
				}
				// We had a non-security failure getting security
				else if (hr!=VWOBJECT_E_PERMISSION_DENIED)
					goto failexit;

			SAFEFREESTRING(bstrReturnValue);

			hr = pInstancePropertyMap->NextItem( &bstrReturnValue, &varProperty, &fLastItem );
			if (FAILED(hr)) goto failexit;

			SAFERELEASE(pSecurity);
			}
		}
	}

	if (pExemplarPropertyMap && (m_lFilterType & FILTER_EXEMPLAR_PROPERTIES))
	{
		VARIANT_BOOL fLastItem = VARIANT_FALSE;
		VARIANT_BOOL vBool = VARIANT_FALSE;
		COleVariant varProperty;
		BOOL bAddItem = TRUE;

		hr = pExemplarPropertyMap->get_IsEmpty( &fLastItem );
		if (FAILED(hr)) goto failexit;

		if( ! (fLastItem == VARIANT_TRUE))
		{
			varProperty.Clear();
			hr = pExemplarPropertyMap->FirstItem( &bstrReturnValue, &varProperty, &fLastItem );
			if (FAILED(hr)) goto failexit;

			while( ! (fLastItem == VARIANT_TRUE))
			{
				// check to see if this item has been replaced with a value in the
				// instance property map
				if (pInstancePropertyMap)
				{
					hr = pInstancePropertyMap->IsValid(bstrReturnValue, &vBool);
					if (FAILED(hr)) goto failexit;

					if (vBool == VARIANT_TRUE) bAddItem = FALSE;
				}

				if (bAddItem)
				{
					BOOL bHidden = FALSE;
					BOOL bConstant = FALSE;
					BOOL bFriendly = TRUE;

					// get the security pointer
					if (FAILED(hr = m_pThing->get_PropertySecurity(bstrReturnValue, &pSecurity )))
						goto failexit;

					// If succeeded (we have read permissions), add the item
					if (SUCCEEDED(hr))
					{
						// get the bits out of the security pointer
						if (FAILED(hr = pSecurity->get_Bits(&lBits)))
							goto failexit;

						bHidden = (lBits & PSBIT_HIDDEN);
						bFriendly = (lBits & PSBIT_FRIENDLY);
						bConstant = (lBits & PSBIT_CONSTANT);

						if (!bHidden && !bConstant)
						{
							// Get the name of the Item in the PropertyMap.
							cstrName = bstrReturnValue;
							IconType type = EXEMPLAR_PROPERTY;
							if (m_lFilterType & FILTER_FRIENDLY_PROPERTIES)
							{
								if (bFriendly)
									AddItem( pSecurity, cstrName, type );
							}
							else
								AddItem( pSecurity, cstrName, type );
						}
					}
					// We had a non-security failure getting security
					else if (hr!=VWOBJECT_E_PERMISSION_DENIED)
						goto failexit;
				}

			SAFEFREESTRING(bstrReturnValue);
		
			hr = pExemplarPropertyMap->NextItem( &bstrReturnValue, &varProperty, &fLastItem );
			if (FAILED(hr)) goto failexit;

			SAFERELEASE(pSecurity);
			bAddItem = TRUE;
			}
		}
	}

	m_listCtrl.Sort();

failexit:
	SAFERELEASE(pSecurity);
	SAFEFREESTRING(bstrReturnValue);
	SAFERELEASE(pExemplar);
	SAFERELEASE(pInstancePropertyMap);
	SAFERELEASE(pExemplarPropertyMap);
}

//
// DoubleClick
// iItem - item that was clicked on
// iColumn - column the double click was in
//
// called when a user double clicks in the list control
void CTPListCtrl::DoubleClick(int iItem, int iColumn)
{
	if (iItem < 0)
		return;

	CPropListData *cpld = NULL;
	cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	if (!cpld)
		return;

	// always edit
	if (!cpld->m_bEditable)
		return;

	if (Editable(cpld->m_vtType))
	{
		if (cpld->m_vtType != VT_BOOL)
			m_listCtrl.EditValue(iItem, cpld->m_strValue);
		else
			m_listCtrl.EditValue(iItem, (cpld->m_strValue.CompareNoCase("true") == 0));
	}
	else
	{
		if (!DealWithCopyOnGet(cpld))
			return;

		FireEditValue(m_pThing, cpld->m_strName);
	}
}

//
// LeftClick
// iItem - item that was clicked on
// iColumn - column the double click was in
//
// called when a user double clicks in the list control
void CTPListCtrl::LeftClick(int iItem, int iColumn)
{
	if (iItem < 0)
		return;

	VARIANT_BOOL vBool = VARIANT_FALSE;
	HRESULT hr = S_OK;
	CPropListData *cpld = NULL;
//	UINT state;

	cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	if (!cpld)
		return;

	// check to see if the item was already selected
//	state = m_listCtrl.GetItemState(iItem, LVIS_SELECTED);

//	if (state != LVIS_SELECTED)
//		return;

	switch (iColumn)
	{
		case 0:
			// do nothing
			break;
		case 1:
			if (ISEXEMPLAR(cpld->m_type))
				return;

			m_pThing->CheckPropertySecurity(CComBSTR(cpld->m_strName), PS_ADD, &vBool);
			if (FAILED(hr))
				return;

			if (!((vBool == VARIANT_TRUE) && m_bCanAddToThing))
				return;

			m_listCtrl.EditName(iItem, cpld->m_strName, 1);
			break;
		case 2:
			break;
		case 3:
			if (!cpld->m_bEditable)
				return;

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

// LaunchExternalEditor
// iItem - item that the button was on
void CTPListCtrl::LaunchExternalEditor(int iItem)
{
	CPropListData *cpld = NULL;
	cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	if (!cpld)
		return;

	if (!DealWithCopyOnGet(cpld))
		return;

	FireEditValue(m_pThing, cpld->m_strName);
}

//
// DoneEditingName
// the list ctrl is done editing the value of a property
//
void CTPListCtrl::DoneEditingName(int iItem, CString strNewValue)
{
	CComVariant varProp;
	HRESULT hr = S_OK;
	VARIANT_BOOL vBool = VARIANT_FALSE;
	CString strError;
	CPropListData *cpld = NULL;
	IPropertySecurity *pPropSec = NULL;
	CComBSTR bstrIID;
	CComBSTR bstrType;
	long lBits;
	long lFlags;
	BOOL bOverwrite = FALSE;

	if (!m_pThing)
		return;

	cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	if (!cpld)
		return;

	if (strNewValue.CompareNoCase(cpld->m_strName) == 0)
		return;

	// check to see if there is already a prop by the new name
	if (FAILED(hr = m_pThing->IsValidProperty(CComBSTR(strNewValue), &vBool)))
		goto exit;

	if (vBool == VARIANT_TRUE)
	{
		// first check if we have permission to overwrite that slot
		if (FAILED(hr = m_pThing->CheckPropertySecurity(CComBSTR(strNewValue), PS_WRITE, &vBool)))
			goto exit;
		if (vBool == VARIANT_FALSE)
		{
			strError.LoadString(IDS_ERROR_CANT_WRITE_PROP);
			FireUserMessage(strError);
			return;
		}

		CPropExistsDlog cped;
		if (cped.DoModal() == IDOK)
			bOverwrite = TRUE;
		else
			return;
	}

	// get the prop out of the old slot
	if (FAILED(hr = m_pThing->get_PropertySecurity(CComBSTR(cpld->m_strName), &pPropSec)))
		goto exit;
	
	if (!pPropSec)
		goto exit;

	if (FAILED(hr = pPropSec->get_Permissions(&lBits)))
		goto exit;

	if (FAILED(hr = pPropSec->get_Bits(&lFlags)))
		goto exit;

	if (FAILED(hr = pPropSec->get_PropertyIID(&bstrIID.m_str)))
		goto exit;

	if (FAILED(hr = pPropSec->get_PropertyType(&bstrType.m_str)))
		goto exit;

	if (FAILED(hr = pPropSec->get_Property(&varProp)))
		goto exit;

	if (!bOverwrite)
	{
		// add it to the new slot
		if (FAILED(hr = m_pThing->AddProperty(CComBSTR(strNewValue), varProp, COleVariant(lFlags), COleVariant(lBits),
									 		  CComVariant(varProp.vt), bstrIID, bstrType)))
			goto exit;
	}
	else
	{
		if (FAILED(hr = m_pThing->put_Property(CComBSTR(strNewValue), varProp)))
			goto exit;
	}
	// delete the old slot
	if (FAILED(hr = m_pThing->RemoveProperty(CComBSTR(cpld->m_strName))))
		goto exit;

exit:
	SAFERELEASE(pPropSec);
}

void CTPListCtrl::DoneEditingValue(int iItem, CString strNewValue)
{
	CPropListData *pld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	CComVariant newVarValue;

	if (!pld)
		return;

	if (!pld->m_strName.CompareNoCase("name"))
	{
		// gotta verify that you aren't sticking something bogus into the
		// name property
		VARIANT_BOOL vBool = VARIANT_TRUE;
		CString strError;
		if (SUCCEEDED(m_pWorld->ValidateInstanceName(CComBSTR(strNewValue), &vBool)))
		{
			if (vBool == VARIANT_FALSE)
			{
				// send a warning message to the user and return
				strError.LoadString(IDS_ERROR_BAD_INSTANCE_NAME);
				FireUserMessage(strError);
				return;
			}
		}
	}

	ConvertStringToVariant(pld->m_vtType, strNewValue, newVarValue);

	// put the value of the variant into the propList
	HRESULT hr = S_OK;
	hr = m_pThing->put_Property(CComBSTR(pld->m_strName), newVarValue);
}

//
// GetNameOfSelectedItem
//
// finds the first selected item and returns it's name
CString CTPListCtrl::GetNameOfSelectedItem()
{
	CString strName;
	int iItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);
	if (iItem != -1)
	{

		LV_ITEM	item;
		char pszName[256];

		item.mask = LVIF_TEXT;
		item.iItem = iItem;
		item.iSubItem = 1;
		item.pszText = (char *) &pszName;
		item.cchTextMax = 256;
		m_listCtrl.GetItem(&item);

		strName = pszName;
	}

	return strName;
}


//
// PopUpMenu
// point - location of the right click
//
// called when a user double clicks in the list control
void CTPListCtrl::PopUpMenu(CPoint point)
{
	CMenu menu;
	CMenu *pPopup;
	
	if (!menu.LoadMenu(IDR_THING_PROP_MENU))
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


void CTPListCtrl::VirtualKeyPressed(UINT nChar)
{
	switch (nChar)
	{
		case VK_DELETE:
			OnDeleteProperty();
			break;
		case VK_RETURN:
			OnEditValue();
			break;
		default:
			FireKeyPressed(nChar);
			break;
	}
}

void CTPListCtrl::VirtualKeyUp(UINT nChar)
{
	FireVKeyUp(nChar);
}


//
// AlphaKeyPressed
//
// called when a user presses a letter key in the list control
void CTPListCtrl::AlphaKeyPressed(char cLetter)
{
	// walk through items in list
	int iListCount;
	CPropListData *cpld = NULL;
	CString strCompare;

	if (!m_pThing)
		return;

	if ((m_listCtrl.m_iSortColumn < 1) || (m_listCtrl.m_iSortColumn > 3))
		return;

	iListCount = m_listCtrl.GetItemCount();

	// go through the rest of the list and fix the indexes on the items
	for (int i = 0; i < iListCount; i++)
	{
		cpld = (CPropListData *) m_listCtrl.GetItemData(i);
		switch(m_listCtrl.m_iSortColumn)
		{
			case 1:
				strCompare = cpld->m_strName;
				break;
			case 2:
				strCompare = cpld->m_strType;
				break;
			case 3:
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


// MENU STUFF
void CTPListCtrl::OnUpdateAddProperty(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bCanAddToThing);
}

void CTPListCtrl::OnAddProperty() 
{
	// Fire the add property
	FireNewProperty(m_pThing);
	
}

void CTPListCtrl::OnDeleteProperty() 
{

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

		hr = m_pThing->RemoveProperty(bstrName);
		if (NONSECURITYFAIL(hr))
			goto failexit;
		iLastItem--;
	}

	m_listCtrl.SelectItem(iLastItem + 1);


failexit:
	// clean up
	SAFEFREESTRING(bstrName);
}

void CTPListCtrl::OnUpdateDeleteProperty(CCmdUI* pCmdUI) 
{
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();
	VARIANT_BOOL vBool;
	HRESULT hr = S_OK;

	// REVIEW: since this is a multi-select operation possibly,
	// should I actually cycle through and make sure you can delete them
	// all, or is it ok to assume that if you can delete one, you can
	// dump the rest?
	if (uiSelected > 0)
	{
		int iItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);
		CPropListData *cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
		if (ISEXEMPLAR(cpld->m_type))
			pCmdUI->Enable(FALSE);
		else
		{
			m_pThing->CheckPropertySecurity(CComBSTR(cpld->m_strName), PS_ADD, &vBool);
			if (SUCCEEDED(hr))
				pCmdUI->Enable(vBool == VARIANT_TRUE);
			else
				pCmdUI->Enable(FALSE);
		}
	}
	else
		pCmdUI->Enable(FALSE);
}

void CTPListCtrl::OnEditName() 
{
	int iItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);
	
	if (!(iItem >= 0))
		return;

	CPropListData *cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	m_listCtrl.EditName(iItem, cpld->m_strName, 1);
}

void CTPListCtrl::OnUpdateEditName(CCmdUI* pCmdUI) 
{
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();
	VARIANT_BOOL vBool = VARIANT_FALSE;
	HRESULT hr = S_OK;

	if (uiSelected == 1)
	{
		int iItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);
		CPropListData *cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
		m_pThing->CheckPropertySecurity(CComBSTR(cpld->m_strName), PS_ADD, &vBool);
		if (SUCCEEDED(hr))
			pCmdUI->Enable((vBool == VARIANT_TRUE) && m_bCanAddToThing && !ISEXEMPLAR(cpld->m_type));
		else
			pCmdUI->Enable(FALSE);
	}
	else
		pCmdUI->Enable(FALSE);
}

BOOL CTPListCtrl::DealWithCopyOnGet(CPropListData *cpld)
{
	IPropertySecurity *pSec = NULL;
	long lBits = 0;
	BOOL bCopyOnGet = FALSE;
	HRESULT hr = S_OK;

	if (!cpld || !cpld->m_bEditable)
		goto failexit;
	
	if (ISEXEMPLAR(cpld->m_type))
	{
		if (FAILED(m_pThing->get_PropertySecurity(CComBSTR(cpld->m_strName), &pSec)))
			goto failexit;

		if (FAILED(pSec->get_Bits(&lBits)))
			goto failexit;

		bCopyOnGet = (lBits & PSBIT_COPYONGET);

		if (!bCopyOnGet)
		{
#ifndef NO_COPY_PROMPT
			if (CWnd::MessageBox("Do you want me to copy this for you?", "Object Explorer", MB_OKCANCEL | MB_ICONQUESTION) != IDOK)
				goto failexit;
#endif

			// copy the property
			// clone the property and then add it to the instance and then edit it
			CComVariant var;
			CComVariant cloneVar;

			// get the property
			if (FAILED(hr = m_pThing->get_Property(CComBSTR(cpld->m_strName), &var)))
				goto failexit;

			// copy it
			if (m_pWorld)
				hr = m_pWorld->CloneProperty(var, &cloneVar);
			if (FAILED(hr))
				goto failexit;

			// add it to the instance
			if (FAILED(hr = m_pThing->put_Property(CComBSTR(cpld->m_strName), cloneVar)))
				goto failexit;
		}
	}

	SAFERELEASE(pSec);

	return TRUE;

failexit:
	SAFERELEASE(pSec);
	return FALSE;
}

void CTPListCtrl::OnEditValue() 
{
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();

	if (!m_pThing)
		return;

	if (uiSelected == 1)
	{
		int iItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);
		CPropListData *cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);

		if (!DealWithCopyOnGet(cpld))
			return;

		if (Editable(cpld->m_vtType))
		{
			if (cpld->m_vtType != VT_BOOL)
				m_listCtrl.EditValue(iItem, cpld->m_strValue);
			else
				m_listCtrl.EditValue(iItem, (cpld->m_strValue.CompareNoCase("true") == 0));
		}
		else
		{
			if (!DealWithCopyOnGet(cpld))
				return;

			FireEditValue(m_pThing, cpld->m_strName);
		}
	}
}

void CTPListCtrl::OnUpdateEditValue(CCmdUI* pCmdUI) 
{
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();

	if (uiSelected == 1)
	{
		int iItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);
		CPropListData *cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
		pCmdUI->Enable(cpld->m_bEditable);
	}
	else
		pCmdUI->Enable(FALSE);
}

void CTPListCtrl::OnShowExemplarProperties() 
{
	if (m_lFilterType & FILTER_EXEMPLAR_PROPERTIES)
		m_lFilterType = m_lFilterType & ~FILTER_EXEMPLAR_PROPERTIES;
	else
		m_lFilterType = m_lFilterType | FILTER_EXEMPLAR_PROPERTIES;

	ClearList();
	FillList();
}

void CTPListCtrl::OnUpdateShowExemplarProperties(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_lFilterType & FILTER_EXEMPLAR_PROPERTIES);
}

void CTPListCtrl::OnShowInstanceProperties() 
{
	if (m_lFilterType & FILTER_INSTANCE_PROPERTIES)
		m_lFilterType = m_lFilterType & ~FILTER_INSTANCE_PROPERTIES;
	else
		m_lFilterType = m_lFilterType | FILTER_INSTANCE_PROPERTIES;

	ClearList();
	FillList();
}

void CTPListCtrl::OnUpdateShowInstanceProperties(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_lFilterType & FILTER_INSTANCE_PROPERTIES);
}


void CTPListCtrl::OnDestroy() 
{
	ClearList();
	CPropEdCtrl::OnDestroy();
}


// WORLD EVENT HANDLERS
//
HRESULT CTPListCtrl::HandleOnPropertyChanged(IThing *pthing, BSTR bstr, long lHint, VARIANT varHintData)
{
	HRESULT hr = S_OK;
	CString strName(bstr);
	CComVariant var;
	BOOL bIsMe = FALSE;
	BOOL bIsExemplar = FALSE;

	IPropertyMap *pMap = NULL;
	IThing *pExemplar = NULL;
	VARIANT_BOOL vBool = VARIANT_FALSE;
	IPropertySecurity *pSecurity = NULL;
	VARTYPE			varType;

	IconType itType;
	BOOL bAdd = TRUE;

	if (!pthing)
		goto exit;

	if (!m_listCtrl.m_hWnd)
		goto exit;

	if (pthing == m_pThing)
		bIsMe = TRUE;
	
	if (!bIsMe && CheckIfParent(pthing, m_pThing))
	{
		bIsMe = TRUE;
		bIsExemplar = TRUE;
	}

	if (!bIsMe)
		goto exit;

	if (lHint == thAdd)
	{
		// add the new property to the list
		
		// figure out what the deal with this new property is
		if (bIsExemplar)
		{
			if (FAILED(hr = m_pThing->get_InstanceProperties(&pMap)))
				goto exit;

			if (FAILED(hr = pMap->IsValid(bstr, &vBool)))
				goto exit;

			if (vBool == VARIANT_TRUE)
			{
				// this is a new exemplar property, we need to change the type of the property that
				// is currently being displayed as an instance property
				itType = OVERRIDEN_PROPERTY;
				bAdd = FALSE;

				// find the existing property in the list
				int iItem;
				iItem = m_listCtrl.FindItemByName(strName);

				if (iItem != -1)
				{
					// ok, found the item, just change the iconType
					LV_ITEM lvi;
					CPropListData * pPropListData;
					pPropListData = (CPropListData *) m_listCtrl.GetItemData(iItem);
					if (pPropListData)
					{
						if (pPropListData->m_type != EXEMPLAR_PROPERTY)
							itType = (enum IconType) ((int) pPropListData->m_type - 1);
					}
					lvi.mask = LVIF_IMAGE;
					lvi.iItem = iItem;
					lvi.iSubItem = 0;
					lvi.iImage = itType;
					m_listCtrl.SetItem(&lvi);
				}
				else
					// for some reason we can't find the item in the list, just add it
					bAdd = TRUE;

			}
			else
				itType = EXEMPLAR_PROPERTY;
		}
		else
			itType = INSTANCE_PROPERTY;

		if (bAdd)
		{
			// actually get the property
			if (itType == EXEMPLAR_PROPERTY)
			{

				// go to the exemplar and get it out of the property map of it
				if (!pExemplar && FAILED(hr = m_pThing->get_Exemplar(&pExemplar)))
					goto exit;

				if (FAILED(hr = pExemplar->get_Properties(&pMap)))
					goto exit;

				if (FAILED(hr = pMap->get_PropertySecurity(bstr, &pSecurity)))
					goto exit;
			}
			else
			{
				if (FAILED(hr = m_pThing->get_PropertySecurity(bstr, &pSecurity)))
					goto exit;
			}

			AddItem(pSecurity, strName, itType);
		}

		// resort the list
		m_listCtrl.Sort();
	}
	else if (lHint == thPut)
	{
		// change the value in the list
		// with the new rules, the type, security and name can't change, so we're cool on that one.
		// the value must have changed
		int iItem;
		iItem = m_listCtrl.FindItemByName(strName);

		if (iItem != -1)
		{
			// ok, found the item, just change the value, and possibly where it comes from
			// if we are overriding an exemplar prop

			LV_ITEM lvi;
			CPropListData * pPropListData;
			pPropListData = (CPropListData *) m_listCtrl.GetItemData(iItem);

			// if this is a put on the object, then check to see if
			// it is an override of an exemplar prop
			if (!bIsExemplar && pPropListData && ISEXEMPLAR(pPropListData->m_type))
			{
				// it is indeed an override
				pPropListData->m_type = (IconType)((int) pPropListData->m_type + 1);
				lvi.mask = LVIF_IMAGE;
				lvi.iItem = iItem;
				lvi.iSubItem = 0;
				lvi.iImage= pPropListData->m_type;
				m_listCtrl.SetItem(&lvi);
			}

			CString strType, strValue;
			// actually get the property
			if (FAILED( hr = pthing->get_PropertySecurity(bstr, &pSecurity)))
				goto exit;

			if (!GetTypeAndValueFromSecurity(pSecurity, strType, strValue, varType))
				goto exit;

			if (pPropListData)
				pPropListData->m_strValue = strValue;

			lvi.mask = LVIF_TEXT;
			lvi.iItem = iItem;
			lvi.iSubItem = 3;
			lvi.pszText = strValue.GetBuffer(strValue.GetLength() + 1);
			lvi.cchTextMax = strValue.GetLength() + 1;
			m_listCtrl.SetItem(&lvi);

			if (strType.CompareNoCase(pPropListData->m_strType) != 0)
			{
				pPropListData->m_strType = strType;
				pPropListData->m_vtType = varType;
				lvi.mask = LVIF_TEXT;
				lvi.iItem = iItem;
				lvi.iSubItem = 2;
				lvi.pszText = strType.GetBuffer(strType.GetLength() + 1);
				lvi.cchTextMax = strType.GetLength() + 1;
				m_listCtrl.SetItem(&lvi);
				if (m_listCtrl.m_iSortColumn == 2)
					m_listCtrl.Sort();
			}

			if (m_listCtrl.m_iSortColumn == 3)
				m_listCtrl.Sort();
		}
		else
		{
			// the item isn't in the list, add it (for fix to 6275)
			// figure out what the deal with this new property is
			if (bIsExemplar)
			{
				if (FAILED(hr = m_pThing->get_InstanceProperties(&pMap)))
					goto exit;

				if (FAILED(hr = pMap->IsValid(bstr, &vBool)))
					goto exit;

				if (vBool == VARIANT_TRUE)
					itType = OVERRIDEN_PROPERTY;
				else
					itType = EXEMPLAR_PROPERTY;
			}
			else
				itType = INSTANCE_PROPERTY;

			// actually get the property
			if (itType == EXEMPLAR_PROPERTY)
			{
				// go to the exemplar and get it out of the property map of it
				if (!pExemplar && FAILED(hr = m_pThing->get_Exemplar(&pExemplar)))
					goto exit;

				if (FAILED(hr = pExemplar->get_Properties(&pMap)))
					goto exit;

				if (FAILED(hr = pMap->get_PropertySecurity(bstr, &pSecurity)))
					goto exit;
			}
			else
			{
				if (FAILED(hr = m_pThing->get_PropertySecurity(bstr, &pSecurity)))
					goto exit;
			}

			AddItem(pSecurity, strName, itType);
		}

		// resort the list
		m_listCtrl.Sort();

	}
	else if (lHint == thRemove)
	{
		// remove the property from the list

		// find the item
		int iItem;
		iItem = m_listCtrl.FindItemByName(strName);
		
		if (iItem != -1)
		{
			if (bIsExemplar)
			{
				// see if this is an overidden prop, if so just update the icon
				// it could still be an overriden prop, gotta check for that
				if (FAILED(hr = pthing->IsValidProperty(bstr, &vBool)))
					goto exit;

				if (vBool != VARIANT_TRUE)
				{
					LV_ITEM lvi;

					lvi.mask = LVIF_IMAGE;
					lvi.iItem = iItem;
					lvi.iSubItem = 0;
					//lvi.iImage = itType;
					if (m_listCtrl.GetItem(&lvi))
					{
						// got it
						if (ISOVERRIDEN(lvi.iImage))
						{
							lvi.iImage = lvi.iImage + 1;
							m_listCtrl.SetItem(&lvi);
							CPropListData * pPropListData;
							pPropListData = (CPropListData *) m_listCtrl.GetItemData(iItem);
							if (pPropListData)
								pPropListData->m_type = (IconType) ((int)pPropListData->m_type + 1);
						}
						else
						{
							// it's just an exemplar prop, remove it
							m_listCtrl.DeleteItem(iItem);
						}
					}
				}
			}
			else
			{
				// remove the item from the list and check to see if there is an exemplar
				// version lying around to add back into the list
				m_listCtrl.DeleteItem(iItem);

				if (FAILED(hr = m_pThing->IsValidProperty(bstr, &vBool)))
					goto exit;

				if (vBool == VARIANT_TRUE)
				{
					// this has got to be an exemplar prop. 
					// since it is an exemplar property and might be copy on get, we can't get it from the
					// thing.  we have to get it from the thing's exemplar.
					IPropertySecurity *pSecurity = NULL;
					IconType type = EXEMPLAR_PROPERTY;

					if (FAILED( hr = m_pThing->get_Exemplar(&pExemplar)))
						goto exit;
					if (FAILED( hr = pExemplar->get_Properties(&pMap)))
						goto exit;
					if (FAILED( hr = pMap->get_PropertySecurity(bstr, &pSecurity)))
						goto exit;
					
					AddItem(pSecurity, strName, type);
					SAFERELEASE(pSecurity);
					m_listCtrl.Sort();
				}
			}
		}
	}
	else
	{
		// an object property's value is changing?
		int iItem;

		if (FAILED(hr = pthing->get_Property(bstr, &var)))
			goto exit;

		iItem = m_listCtrl.FindItemByName(strName);
		if (iItem != -1)
		{
			// ok, found the item, just change the value
			CString strType, strValue;

			// actually get the property
			if (FAILED( hr = pthing->get_Property(bstr, &var)))
				goto exit;

			if (!GetTypeAndValueFromVariant(var, strType, strValue))
				goto exit;

			LV_ITEM lvi;

			lvi.mask = LVIF_TEXT;
			lvi.iItem = iItem;
			lvi.iSubItem = 3;
			lvi.pszText = strValue.GetBuffer(strValue.GetLength() + 1);
			lvi.cchTextMax = strValue.GetLength() + 1;
			m_listCtrl.SetItem(&lvi);

			CPropListData * pPropListData;
			pPropListData = (CPropListData *) m_listCtrl.GetItemData(iItem);
			if (pPropListData)
				pPropListData->m_strValue = strValue;

			if (m_listCtrl.m_iSortColumn == 3)
				m_listCtrl.Sort();
		}

	}

	hr = CVWUIView::HandleOnPropertyChanged(pthing, bstr, lHint, varHintData);
  
exit:

	SAFERELEASE(pSecurity);
	SAFERELEASE(pExemplar);
	SAFERELEASE(pMap);
	return hr;
}


//
// World Events
//
HRESULT CTPListCtrl::HandleOnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient)
{
	static CComBSTR bstrLeaveRoom(VW_LEAVEROOM_EVENT_STR);
	CThingPtr pThing;
	IThing *pUser = NULL;
	HRESULT hr=S_OK;

	CVWUIView::HandleOnUIEvent(pthing, bstrEventName, varArg, bFromClient);

	if (CompareElements(&bstrLeaveRoom, &bstrEventName))
	{
		pThing = varArg;
		if (FAILED(hr = m_pWorld->get_User(&pUser)))
			goto exit;

		if (pUser == pThing)
		{
			// I'm leaving the room!
			SetTargetObjectProperty(NULL);
		}
	}

exit:
	SAFERELEASE(pUser);
	return S_OK;
}

// client events
HRESULT CTPListCtrl::HandleOnDisconnect(IDispatch* pWorld)
{
	SetTargetObjectProperty(NULL);

	return CVWUIView::HandleOnDisconnect(pWorld);
}

HRESULT CTPListCtrl::HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient)
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

// VWUIView events
HRESULT CTPListCtrl::HandleUIUpdate( enumVWUIEventTypes eTypes, IVWUIView*,VARIANT varHint )
{
	HRESULT hr = S_OK;
	CThingPtr thingPtr;

	if (!m_bRespondToSelection)
		return hr;

	thingPtr = varHint;

	switch(eTypes)
	{
	case VWUIT_ADDSELECTION:
		if (!thingPtr)
			return hr;
		else
			SetTargetObjectProperty(thingPtr);
		break;

	case VWUIT_REMOVESELECTION:
	case VWUIT_TOGGLESELECTION:
	case VWUIT_CLEARSELECTIONS:
	case VWUIT_REFRESHOBJECTS:
	default:
		break;
	}

	return hr;
}


void CTPListCtrl::DoFocus() 
{
	m_listCtrl.SetFocus();
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();

	if (uiSelected == 0)
		m_listCtrl.SelectItem(0, m_listCtrl.m_iSortColumn);
}
