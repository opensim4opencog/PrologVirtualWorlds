// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// TVListCtl.cpp : Implementation of the CTVListCtrl ActiveX Control class.

#include "stdafx.h"
#include "objexplr.h"
#include "PrpEdCtl.h"
#include "TVLstCtl.h"
#include "listdata.h"

#include <propbase.h>
#include <vwevents.h>

#include "MExistDl.h"

typedef CVWComPtr<IMethod, NULL, &IID_IMethod> CMethodPtr;
typedef CVWComPtr<IThing,NULL,&IID_IThing> CThingPtr;
typedef CVWComPtr<IScriptModule,NULL,&IID_IScriptModule> CScriptModulePtr;
typedef CVWComPtr<IModule,NULL,&IID_IModule> CModulePtr;
typedef CVWComPtr<IPropertyMap,NULL,&IID_IPropertyMap> CPropertyMapPtr;
typedef CVWComPtr<IPropertySecurity,NULL,&IID_IPropertySecurity> CPropertySecurityPtr;

#define ISWRITABLE(type) ((type != EXEMPLAR_METHOD_NO_WRITE) && (type != OVERRIDEN_METHOD_NO_WRITE) && (type != INSTANCE_METHOD_NO_WRITE))
#define ISEXEMPLAR(type) ((type == EXEMPLAR_METHOD) || (type == EXEMPLAR_METHOD_NO_WRITE))

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CTVListCtrl, CPropEdCtrl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CTVListCtrl, CPropEdCtrl)
	//{{AFX_MSG_MAP(CTVListCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_ADD_METHOD, OnAddMethod)
	ON_COMMAND(ID_DELETE_METHOD, OnDeleteMethod)
	ON_UPDATE_COMMAND_UI(ID_DELETE_METHOD, OnUpdateDeleteMethod)
	ON_COMMAND(ID_EDIT_NAME, OnEditName)
	ON_UPDATE_COMMAND_UI(ID_EDIT_NAME, OnUpdateEditName)
	ON_COMMAND(ID_SHOW_EXEMPLAR_METHODS, OnShowExemplarMethods)
	ON_UPDATE_COMMAND_UI(ID_SHOW_EXEMPLAR_METHODS, OnUpdateShowExemplarMethods)
	ON_COMMAND(ID_SHOW_INSTANCE_METHODS, OnShowInstanceMethods)
	ON_UPDATE_COMMAND_UI(ID_SHOW_INSTANCE_METHODS, OnUpdateShowInstanceMethods)
	ON_COMMAND(ID_EDIT_SUB_NAME, OnEditSubName)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SUB_NAME, OnUpdateEditSubName)
	ON_COMMAND(ID_CHANGE_MODULE, OnChangeModule)
	ON_UPDATE_COMMAND_UI(ID_CHANGE_MODULE, OnUpdateChangeModule)
	ON_UPDATE_COMMAND_UI(ID_ADD_METHOD, OnUpdateAddMethod)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CTVListCtrl, CPropEdCtrl)
	//{{AFX_DISPATCH_MAP(CTVListCtrl)
	DISP_PROPERTY_EX(CTVListCtrl, "SelectionList", GetSelectionList, SetSelectionList, VT_DISPATCH)
	DISP_PROPERTY_EX(CTVListCtrl, "TargetObjectProperty", GetTargetObjectProperty, SetTargetObjectProperty, VT_DISPATCH)
	DISP_PROPERTY_EX(CTVListCtrl, "RespondToSelectionEvents", GetRespondToSelectionEvents, SetRespondToSelectionEvents, VT_BOOL)
	DISP_FUNCTION(CTVListCtrl, "ExternalSort", ExternalSort, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CTVListCtrl, "DoFocus", DoFocus, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CTVListCtrl, CPropEdCtrl)
	//{{AFX_EVENT_MAP(CTVListCtrl)
	EVENT_CUSTOM("NewMethod", FireNewMethod, VTS_DISPATCH)
	EVENT_CUSTOM("EditValue", FireEditValue, VTS_DISPATCH  VTS_BSTR)
	EVENT_CUSTOM("UserMessage", FireUserMessage, VTS_BSTR)
	EVENT_CUSTOM("KeyPressed", FireKeyPressed, VTS_I4)
	EVENT_CUSTOM("VKeyUp", FireVKeyUp, VTS_I4)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CTVListCtrl, "OBJEXPLR.TVListCtrl.1",
	0xef7d6561, 0x161, 0x11d2, 0x88, 0xbe, 0, 0xc0, 0x4f, 0xc3, 0x2e, 0xf3)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CTVListCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DTVList =
		{ 0xef7d655f, 0x161, 0x11d2, { 0x88, 0xbe, 0, 0xc0, 0x4f, 0xc3, 0x2e, 0xf3 } };
const IID BASED_CODE IID_DTVListEvents =
		{ 0xef7d6560, 0x161, 0x11d2, { 0x88, 0xbe, 0, 0xc0, 0x4f, 0xc3, 0x2e, 0xf3 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwTVListOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CTVListCtrl, IDS_TVLIST, _dwTVListOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CTVListCtrl::CTVListCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CTVListCtrl

BOOL CTVListCtrl::CTVListCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_TVLIST,
			IDB_TVLIST,
			afxRegApartmentThreading,
			_dwTVListOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CTVListCtrl::CTVListCtrl - Constructor

CTVListCtrl::CTVListCtrl()
{
	InitializeIIDs(&IID_DTVList, &IID_DTVListEvents);

	m_pThing = NULL;

	m_lFilterType = FILTER_INSTANCE_METHODS |
					FILTER_EXEMPLAR_METHODS |
					FILTER_WRITABLE_METHODS ;
					//FILTER_FRIENDLY_METHODS ;
	
	m_pListIcons = NULL;

	m_bRespondToSelection = FALSE;

	m_pModuleNameList = NULL;

	// add world events
	m_ulFlags |= ADVISE_WORLD;

	// initialize security flags
	m_bCanAddToThing = TRUE;
	m_bCanPutOnThing = TRUE;

	m_bModuleListDirty = TRUE;
	m_pCurrentlyEditing = NULL;

	m_strLastSelectedMethod.Empty();
}


/////////////////////////////////////////////////////////////////////////////
// CTVListCtrl::~CTVListCtrl - Destructor

CTVListCtrl::~CTVListCtrl()
{
	SAFERELEASE(m_pThing);
	delete m_pListIcons;
	delete m_pModuleNameList;
}


/////////////////////////////////////////////////////////////////////////////
// CTVListCtrl::OnDraw - Drawing function

void CTVListCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	DoSuperclassPaint(pdc, rcBounds);
}


BOOL CTVListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
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
// CTVListCtrl::DoPropExchange - Persistence support

void CTVListCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CTVListCtrl::OnResetState - Reset control to default state

void CTVListCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CTVListCtrl message handlers

LPDISPATCH CTVListCtrl::GetSelectionList() 
{
	// TODO: Add your property handler here

	return NULL;
}

void CTVListCtrl::SetSelectionList(LPDISPATCH newValue) 
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

	// get rid of the old selections
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
	// don't have to do this anymore, it's part of SelectItem
	// m_listCtrl.ShowItemAndColumn(iListIndex, 0);

exit:

	SAFERELEASE(pPropList);

}

LPDISPATCH CTVListCtrl::GetTargetObjectProperty() 
{
	SAFEADDREF(m_pThing);
	return m_pThing;
}

void CTVListCtrl::SetTargetObjectProperty(LPDISPATCH newValue) 
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

	if (SUCCEEDED(hr = m_pThing->CheckMethodsSecurity(PS_ADD, &vBool)))
		m_bCanAddToThing = (vBool == VARIANT_TRUE);
	if (SUCCEEDED(hr = m_pThing->CheckMethodsSecurity(PS_WRITE, &vBool)))
		m_bCanPutOnThing = (vBool == VARIANT_TRUE);

	SetModifiedFlag();

	if (m_listCtrl.m_hWnd)
	{
		ClearList();
		FillList();

		if (!m_strLastSelectedMethod.IsEmpty())
		{
			iItem = m_listCtrl.FindItemByName(m_strLastSelectedMethod);
			if (iItem < 0)
				iItem = 0;
		}
		
		m_listCtrl.SelectItem(iItem,0,FALSE);
	}
}

void CTVListCtrl::ExternalSort(short column) 
{
	if ((column >= 0) && (column <= 3))
	{
		m_listCtrl.m_iSortColumn = column;
		m_listCtrl.Sort();
	}
}

BOOL CTVListCtrl::GetRespondToSelectionEvents() 
{
	return m_bRespondToSelection;
}

void CTVListCtrl::SetRespondToSelectionEvents(BOOL bNewValue) 
{
	m_bRespondToSelection = bNewValue;

	SetModifiedFlag();
}

//
// windows events
//
int CTVListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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
		m_pListIcons->Create(IDB_METH_ICONS, 16, 0, 0x00FFFFFF);
	}

	m_listCtrl.SetImageList(m_pListIcons, LVSIL_SMALL);

	m_listCtrl.m_pOCX = this;

	if (m_pThing)
	{
		int iItem;

		ClearList();
		FillList();

		if (!m_strLastSelectedMethod.IsEmpty())
		{
			iItem = m_listCtrl.FindItemByName(m_strLastSelectedMethod);
			if (iItem < 0)
				iItem = 0;
		}
		
		m_listCtrl.SelectItem(iItem,0,FALSE);
	}
	
	return 0;
}

void CTVListCtrl::OnSize(UINT nType, int cx, int cy) 
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
void CTVListCtrl::ClearList()
{
	m_listCtrl.DeleteAllItems();
}


//
// DoneEditingName
// the list ctrl is done editing the value of a property
//
void CTVListCtrl::DoneEditingName(int iItem, CString strNewValue)
{
	IMethod *pMethod = NULL;
	HRESULT hr = S_OK;
	VARIANT_BOOL vBool = VARIANT_FALSE;
	CString strError;
	CPropListData *cpld = NULL;
	IPropertySecurity *pPropSec = NULL;
	long lPermissions;
	long lBits;
	BOOL bOverwrite = FALSE;

	if (!m_pThing)
		return;

	cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	if (!cpld)
		return;

	if (strNewValue.CompareNoCase(cpld->m_strName) == 0)
		return;

	// check to see if there is already a prop by the new name
	if (FAILED(hr = m_pThing->IsValidMethod(CComBSTR(strNewValue), &vBool)))
		goto exit;

	if (vBool == VARIANT_TRUE)
	{
		// first check if we have permission to overwrite that slot
		if (FAILED(hr = m_pThing->CheckMethodSecurity(CComBSTR(strNewValue), PS_WRITE, &vBool)))
			goto exit;
		if (vBool == VARIANT_FALSE)
		{
			strError.LoadString(IDS_ERROR_CANT_WRITE_METH);
			FireUserMessage(strError);
			return;
		}

		CMethExistsDlog cped;
		if (cped.DoModal() == IDOK)
			bOverwrite = TRUE;
		else
			return;
	}

	// get the prop out of the old slot
	if (FAILED(hr = m_pThing->get_MethodSecurity(CComBSTR(cpld->m_strName), &pPropSec)))
		goto exit;
	
	if (!pPropSec)
		goto exit;

	if (FAILED(hr = pPropSec->get_Permissions(&lPermissions)))
		goto exit;

	if (FAILED(hr = pPropSec->get_Bits(&lBits)))
		goto exit;

	if (lBits & PSBIT_HIDDEN)
	{
		//Do not allow the overwriting of a system method
		// REVIEW: this string should be changed later to be a resource if there is ever an effort
		// to do internationalization
		strError = "Cannot overwrite a system method";
		FireUserMessage(strError);
		goto exit;
	}

	if (FAILED(hr = m_pThing->get_Method(CComBSTR(cpld->m_strName), &pMethod)))
		goto exit;

	if (!bOverwrite)
	{
		// add it to the new slot
		if (FAILED(hr = m_pThing->AddMethodExt(CComBSTR(strNewValue), pMethod, lBits, lPermissions)))
			goto exit;
	}
	else
	{
		// put it over the other slot
		if (FAILED(hr = m_pThing->put_Method(CComBSTR(strNewValue), pMethod)))
			goto exit;
	}

	// delete the old slot
	if (FAILED(hr = m_pThing->RemoveMethod(CComBSTR(cpld->m_strName))))
		goto exit;

exit:
	SAFERELEASE(pPropSec);
}

//
// DoneEditingValue
// the list ctrl is done editing the value of a property
// should be overriden by the derrived list props since
// they actually have pointers to their list controls
// and they know how to change values of props
//
void CTVListCtrl::DoneEditingValue(int iItem, CString strNewValue)
{
	// we have a new sub name to set on the method
	HRESULT hr = S_OK;
	IMethod *pMethod = NULL;
	CPropListData *cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	
	if (!cpld || !m_pThing || !m_pWorld)
		return;

	// get the method
	if (FAILED( hr = m_pThing->get_Method(CComBSTR(cpld->m_strName), &pMethod)))
		goto exit;
	
	// empty out the sub name
	if (FAILED(hr = pMethod->put_MethodName(CComBSTR(strNewValue))))
		goto exit;
	
	// put the method back
	if (FAILED(hr = m_pThing->put_Method(CComBSTR(cpld->m_strName), pMethod)))
		goto exit;
exit:
	SAFERELEASE(pMethod);
}

// DoneEditingModule
// called up from the module drop down to change the module
// on a method
void CTVListCtrl::DoneEditingModule(int iItem, CString strNewValue)
{
	HRESULT hr = S_OK;
	IMethod *pMethod = NULL;
	IModule *pModule = NULL;
	long lFlags = 0;
	CPropListData *cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	CString strError;
	
	if (!cpld || !m_pThing || !m_pWorld)
		return;

	if (strNewValue == cpld->m_strType)
		return;

	if (FAILED( hr = m_pThing->get_Method(CComBSTR(cpld->m_strName), &pMethod)))
		goto exit;
	
	if (FAILED( hr = pMethod->get_Flags(&lFlags)))
		goto exit;

	if (strNewValue != "Inline Module")
	{
		// ok, we have the name of the new module
		// change the module on the method and reset the sub names

		// get the new module
		if (FAILED(hr = m_pWorld->get_Module(CComBSTR(strNewValue), &pModule)))
			goto exit;

		if (cpld->m_type == EXEMPLAR_METHOD)
		{
			// need to create a new method so we don't change the exemplar one
			SAFERELEASE(pMethod);
			if (FAILED(hr = m_pWorld->CreateMethod(lFlags, CComVariant(pModule), CComBSTR(""), &pMethod)))
				goto failexit;

			if (FAILED(hr = m_pThing->put_Method(CComBSTR(cpld->m_strName), pMethod)))
				goto failexit;
		}
		else
		{
			// put the module on the method
			if (FAILED(hr = pMethod->put_Module(pModule)))
				goto exit;

			// empty out the sub name
			if (FAILED(hr = pMethod->put_MethodName(CComBSTR(""))))
				goto exit;

			if (FAILED(hr = m_pThing->put_Method(CComBSTR(cpld->m_strName), pMethod)))
				goto exit;
		}
	}
	else
	{
		if (cpld->m_strType == ":VBScript")
			goto exit;


		// handle adding a new in-line method
		// first create a new in-line method
		CString strObjId;
		CString strMethodText;
		long lID;
		char buf[256];

		SAFERELEASE(pMethod);
		
		if (FAILED(hr = m_pThing->get_ID(&lID)))
			goto exit;
		
		sprintf(buf, "%d", lID);
		strObjId = buf;

		strMethodText = "Sub " + cpld->m_strName + strObjId + "()" + char(13) + char(10)
						+ "   'TODO: Add code here" + char(13) + char(10) + "   'this.InvokeSuperMethod(Array(arg1, arg2, ...))" + char(13) + char(10) + "End Sub"+ char(13) + char(10);
		if (FAILED(hr = m_pWorld->CreateInlineMethod(lFlags, CComBSTR("VBScript"), CComBSTR(cpld->m_strName + strObjId), CComBSTR(strMethodText), &pMethod)))
			goto exit;

		// then replace the old one
		if (FAILED(hr = m_pThing->put_Method(CComBSTR(cpld->m_strName), pMethod)))
			goto exit;
	}

exit:
	SAFERELEASE(pModule);
	SAFERELEASE(pMethod);
	return;

failexit:
	strError.LoadString(IDS_ERROR);
	CString strMessage;

	switch (hr)
	{
		case VWOBJECT_E_NOTSERVERMODULE:
			strMessage.LoadString(IDS_ERROR_NOTSERVERMODULE);
			break;
		case VWOBJECT_E_NOTCLIENTMODULE:
			strMessage.LoadString(IDS_ERROR_NOTCLIENTMODULE);
			break;
		default:
			strMessage.LoadString(IDS_ERROR_DEFAULT);
			break;
	}
	FireUserMessage(strError+strMessage);
	goto exit;
}

// there isn't a lot of stuff here, but if we change what the value of a method is printed as,
// this will be useful because there will be only one place to change the code
void CTVListCtrl::GetTypeAndValueFromMethod(IMethod *pMethod, CString &strType, CString &strValue)
{
	VARIANT_BOOL vbInline;
	HRESULT hr = S_OK;

	if (FAILED(hr = pMethod->get_IsInline(&vbInline)))
		return;

	if (vbInline == VARIANT_TRUE)
		strType.LoadString(IDS_TYPE_INLINE_METHOD);
	else
	{
		CComBSTR bstrType;
		hr = pMethod->get_Type( &bstrType.m_str );
		if (FAILED(hr))
			return;
		strType = bstrType;
	}

	strValue.LoadString(IDS_VALUE_METHOD);
}

// given a method, get the name of the module
// and the name of the sub
BOOL CTVListCtrl::GetModuleAndSubFromMethod(IMethod *pMethod, CString &strModule, CString &strSub, BOOL &bScriptModule)
{
	CModulePtr pModule;
	HRESULT hr = S_OK;
	CComBSTR bstrModule;
	CComBSTR bstrMethod;
	CString strModuleType;
	long	moduleType;
	VARIANT_BOOL vbIsShutdown = VARIANT_FALSE;

	if (FAILED(hr = pMethod->get_Module(&pModule.p)))
		return FALSE;

	if (pModule)
	{
		if (FAILED(hr = pModule->get_IsShutdown(&vbIsShutdown)))
			return FALSE;

		if (vbIsShutdown == VARIANT_FALSE)
		{
			if (FAILED(hr = pModule->get_ModuleName(&bstrModule.m_str)))
				return FALSE;

			if (FAILED(hr = pModule->get_ModuleType(&moduleType)))
				return FALSE;

			if (moduleType == mcCOM)
			{
				strModule.LoadString(IDS_COM_MODULE);
				strModule += CString(bstrModule);
				bScriptModule = FALSE;
			}
			else
			{
				// module is mcScript
				strModule = bstrModule;
				bScriptModule = TRUE;
			}
		}
		else
		{
			// the module has been shutdown
			strModule.LoadString(IDS_DELETED_MODULE);
			bScriptModule = FALSE;
		}
	}
	else
	{
		// the module has been shutdown
		strModule.LoadString(IDS_NO_MODULE);
		bScriptModule = FALSE;
	}

	if (FAILED(hr = pMethod->get_MethodName(&bstrMethod.m_str)))
		return FALSE;

	strSub = bstrMethod;

	return TRUE;
}

// returns a CStringList
CStringList* CTVListCtrl::GetModuleList()
{
	HRESULT			hr = S_OK;
	IPropertyMap	*pModuleMap = NULL;
	CScriptModulePtr		pModule;
	VARIANT_BOOL	vbDone = VARIANT_FALSE;
	CComBSTR		bstrModuleName;
	CComVariant		varModule;
	CString			strModuleName;
	long			lModuleFlags = 0;
	long			lMethodFlags = 0;
	IMethod			*pMethod = NULL;
	BOOL			bServer = FALSE;
	BOOL			bClient = FALSE;


	if (!m_bModuleListDirty)
		return m_pModuleNameList;
	
	if (m_pModuleNameList)
		delete m_pModuleNameList;

	m_pModuleNameList = new CStringList();

	if (!m_pWorld || !m_pCurrentlyEditing || !m_pCurrentlyEditing->m_strName || !m_pThing)
		return NULL;

	// fill in the modules
	if (FAILED(m_pWorld->get_Modules(&pModuleMap)) || !pModuleMap)
		return NULL;

	// get the current method that we are trying to edit
	if (FAILED(hr = m_pThing->get_Method(CComBSTR(m_pCurrentlyEditing->m_strName), &pMethod)))
		goto failexit;

	if (!pMethod)
		goto failexit;

	// get the flags on the method
	if (FAILED(hr = pMethod->get_Flags(&lMethodFlags)))
		goto failexit;

	bServer = lMethodFlags & METHOD_SERVER;
	bClient = lMethodFlags & METHOD_CLIENT;

	// walk through the list
	if (FAILED(hr = pModuleMap->FirstItem(&bstrModuleName.m_str, &varModule, &vbDone)))
		goto failexit;

	while (vbDone == VARIANT_FALSE)
	{
		pModule = varModule;
		if (pModule)
		{
			// get the flags
			if (FAILED(hr = pModule->get_Flags(&lModuleFlags)))
				goto failexit;

			// check to see if is the same type as the method
			if ((bServer && (lModuleFlags & MODULE_SERVER)) ||
				(bClient && (lModuleFlags & MODULE_CLIENT)))
			{
				// it's a script module (we QI for it in CModulePtr)
				strModuleName = bstrModuleName;
				m_pModuleNameList->AddHead(strModuleName);
			}
		}

		if (FAILED(hr = pModuleMap->NextItem(&bstrModuleName.m_str, &varModule, &vbDone)))
			goto failexit;
	}

	// add the In-Line Module
	m_pModuleNameList->AddHead(CString("Inline Module"));

	SAFERELEASE(pModuleMap);
	SAFERELEASE(pMethod);
	m_bModuleListDirty = FALSE;
	return m_pModuleNameList;

failexit:
	SAFERELEASE(pMethod);
	SAFERELEASE(pModuleMap);
	return NULL;
}


// AddVariantItem
// given a Variant and the name for it, creates a new ListCtrl Item and adds it
// luckily we always know it is a method
void CTVListCtrl::AddItem(IPropertySecurity *pSec, CString strName, IconType type)
{
	CMethodPtr pMethod;
	IObjectProperty* pProp = NULL;
	CComVariant varProperty;
	BOOL bEditable;
	VARIANT_BOOL vBool;
	HRESULT hr = S_OK;

	// vBool = QUIET_SAFE_CHECKSECURITY(m_pWorld, pSec, PS_WRITE);
	if (FAILED(hr = m_pThing->CheckMethodSecurity(CComBSTR(strName), PS_WRITE, &vBool)))
		return;

	bEditable = (vBool == VARIANT_TRUE);

	// now if this is an exemplar property and we can put on the object, then really it is editable
//	if ((type == EXEMPLAR_METHOD) && m_bCanPutOnThing)
//		bEditable = TRUE;

	if (!bEditable)
	{
		if (type == INSTANCE_METHOD)
			type = INSTANCE_METHOD_NO_WRITE;
		if (type == OVERRIDEN_METHOD)
			type = OVERRIDEN_METHOD_NO_WRITE;
		if (type == EXEMPLAR_METHOD)
			type = EXEMPLAR_METHOD_NO_WRITE;
	}

	// check to see if we this will be filtered out
	if (m_pThing &&
		(((type == INSTANCE_METHOD) && !(m_lFilterType & FILTER_INSTANCE_METHODS)) ||
		((type == OVERRIDEN_METHOD) && !(m_lFilterType & FILTER_INSTANCE_METHODS)) ||
		((type == EXEMPLAR_METHOD) && !(m_lFilterType & FILTER_EXEMPLAR_METHODS)) ||
		((type == INSTANCE_METHOD_NO_WRITE) && !(m_lFilterType & FILTER_WRITABLE_METHODS)) ||
		((type == OVERRIDEN_METHOD_NO_WRITE) && !(m_lFilterType & FILTER_WRITABLE_METHODS)) ||
		((type == EXEMPLAR_METHOD_NO_WRITE) && !(m_lFilterType & FILTER_WRITABLE_METHODS))))
		return;

#if 0
	// get the method from the security
	if (FAILED(pSec->ObjectProperty((IObjectProperty **) &pProp)))
		return;

	hr = pProp->QueryInterface(IID_IMethod, (LPVOID*) &pMethod.p);

	if (FAILED(hr))
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "OBJEXPLR", VWT_ERROR, "CTVListCtrl::AddItem: QI for IMethod failed\n");
#endif
	}


  	// get the method from the security
	hr = pSec->get_Method(&pMethod.p);
	if (FAILED(hr))
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "OBJEXPLR", VWT_ERROR, "CTVListCtrl::AddItem: get_Method failed\n");
#endif
		return;
	}
#endif


	if (FAILED(pSec->get_Property(&varProperty)))
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "OBJEXPLR", VWT_ERROR, "CTVListCtrl::AddItem: get_Property failed\n");
#endif
		return;
	}

	// Cast to IObjectProperty and QI so proxy/stub marshalling works correctly ?
	hr = ((IObjectProperty*) varProperty.pdispVal)->QueryInterface(IID_IObjectProperty, (LPVOID*) &pProp);
	if (FAILED(hr))
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "OBJEXPLR", VWT_ERROR, "CTVListCtrl::AddItem: QI for IThing failed\n");
#endif
	}

	hr = pProp->QueryInterface(IID_IMethod, (LPVOID*) &pMethod.p);
	if (FAILED(hr))
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "OBJEXPLR", VWT_ERROR, "CTVListCtrl::AddItem: QI for IThing failed\n");
#endif
	}


	
/*
	pMethod = varProperty;
	if (!pMethod)
		return;
*/

	ASSERT( !strName.IsEmpty() );

	CString strType, strValue;
	int iSubItem = 0;
	int iActualItem;
	LV_ITEM lvi;
	BOOL bScriptModule;

	//GetTypeAndValueFromMethod(pMethod, strType, strValue);
	if (!GetModuleAndSubFromMethod(pMethod, strType, strValue, bScriptModule))
		return;

	CPropListData * pPropListData;
	pPropListData = new CPropListData(strName, strType, strValue, type, VT_DISPATCH, bEditable);

	pPropListData->m_bScriptMethod = bScriptModule;
	
	if (strType == ":VBScript")
	{
		pPropListData->m_bInLineMethod = TRUE;
		strType.Empty();
		strType = "Inline Module";
	}

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
}

// FillList
// Called when a new Target Object Property is Set
// Iterates through the methods of the Thing and adds the items to the
// List Control
void CTVListCtrl::FillList()
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
	CThingPtr pExemplar;
	CPropertyMapPtr pMethodMap;
	CPropertyMapPtr pExemplarMethodMap;
	CPropertySecurityPtr pSec;
	CComBSTR bstrReturnValue;
	long lBits = 0;

	// Does it have a methods PropertyMap?
	hr = m_pThing->get_InstanceMethods( &pMethodMap.p );
	if (FAILED(hr)) goto failexit;

	if (FAILED(hr = m_pThing->get_Exemplar( &pExemplar.p )))
		goto failexit;

	// does the exemplar have a methods propertyMap?
	if (pExemplar)
	{
		hr = pExemplar->get_Methods( &pExemplarMethodMap.p );
		if (NONSECURITYFAIL(hr)) goto failexit;
		if (SECURITYFAIL(hr))
			pExemplarMethodMap.p = NULL;
	}

	// yes, iterate through methods
	if (pMethodMap.p && (m_lFilterType & FILTER_INSTANCE_METHODS))
	{
		VARIANT_BOOL fLastItem = FALSE;
		COleVariant varProperty;
		BOOL bFriendly = TRUE;
		BOOL bHidden = FALSE;
		BOOL bConstant = FALSE;

		hr = pMethodMap->get_IsEmpty( &fLastItem );
		if (FAILED(hr)) goto failexit;

		if( !(fLastItem == VARIANT_TRUE)) {

			varProperty.Clear();

			if (FAILED(pMethodMap->FirstItem( &bstrReturnValue.m_str, &varProperty, &fLastItem )))
				goto failexit;

			while( !(fLastItem == VARIANT_TRUE) )
			{
				// Get the name of the Item in the PropertyMap.
				cstrName = bstrReturnValue;

				// get the security pointer (inside here is where we check for READ permissions)
				if (FAILED(hr = m_pThing->get_MethodSecurity( bstrReturnValue, &pSec.p )))
					goto failexit;

				// rk: If succeeded (we have read permissions), add the item
				if (SUCCEEDED(hr))
				{
					// get the bits out of the security pointer
					if (FAILED(hr = pSec->get_Bits(&lBits)))
						goto failexit;

					bHidden = (lBits & PSBIT_HIDDEN);
					bFriendly = (lBits & PSBIT_FRIENDLY);
					bConstant = (lBits & PSBIT_CONSTANT);

					if (!bHidden && !bConstant)
					{
						enum IconType type = INSTANCE_METHOD;
				
						VARIANT_BOOL vBool;
						// check to see if this item is overriding an exemplar prop
						if (pExemplarMethodMap.p)
						{
							hr = pExemplarMethodMap->IsValid(bstrReturnValue, &vBool);
							if (FAILED(hr))
								goto failexit;

							if (vBool == VARIANT_TRUE)
								type = OVERRIDEN_METHOD;
						}

						if (m_lFilterType & FILTER_FRIENDLY_METHODS)
						{
							if (bFriendly)
								AddItem( pSec, cstrName,  type);
						}
						else
							AddItem( pSec, cstrName,  type);
					}
				}
				// We had a non-security failure getting security
				else if (hr!=VWOBJECT_E_PERMISSION_DENIED)
					goto failexit;			

				hr = pMethodMap->NextItem( &bstrReturnValue.m_str, &varProperty, &fLastItem );
				if (NONSECURITYFAIL(hr)) goto failexit;

				pSec.Release();
			}
		}
	}
	
	// yes, iterate through methods
	if (pExemplarMethodMap.p && (m_lFilterType & FILTER_EXEMPLAR_METHODS))
	{
		VARIANT_BOOL fLastItem = FALSE;
		VARIANT_BOOL vBool = FALSE;
		COleVariant varProperty;
		BOOL bAddItem = TRUE;

		hr = pExemplarMethodMap->get_IsEmpty( &fLastItem );
		if (FAILED(hr)) goto failexit;

		if( ! (fLastItem == VARIANT_TRUE) ) {
			varProperty.Clear();
			if (FAILED(pExemplarMethodMap->FirstItem( &bstrReturnValue.m_str, &varProperty, &fLastItem )))
				goto failexit;

			while( !(fLastItem == VARIANT_TRUE) )
			{
				// check to see if this item has been replaced with a value in the
				// instance property map
				if (pMethodMap.p)
				{
					hr = pMethodMap->IsValid(bstrReturnValue, &vBool);
					if (FAILED(hr)) goto failexit;

					if (vBool == VARIANT_TRUE)
						bAddItem = FALSE;
				}

				if (bAddItem)
				{
					BOOL bHidden = FALSE;
					BOOL bConstant = FALSE;
					BOOL bFriendly = TRUE;
					IThing* pThing = NULL;

					// Get pointer to IThing interface so proxy/stub marshalling will work correctly?
					hr = m_pThing->QueryInterface(IID_IThing, (LPVOID*) &pThing);
					if (FAILED(hr))
					{
#ifdef _DEBUG
						VWTRACE(m_pWorld, "OBJEXPLR", VWT_ERROR, "CTVListCtrl::FillList(): QI for IThing failed\n");
#endif
						goto failexit;
					}

					// get the security pointer
					hr = pThing->get_MethodSecurity(bstrReturnValue, &pSec.p);
			
					if (SUCCEEDED(hr))
					{
						// get the bits out of the security pointer
						if (FAILED(hr = pSec->get_Bits(&lBits)))
							goto failexit;

						bHidden = (lBits & PSBIT_HIDDEN);
						bFriendly = (lBits & PSBIT_FRIENDLY);
						bConstant = (lBits & PSBIT_CONSTANT);

						if (!bHidden && !bConstant)
						{
							// Get the name of the Item in the PropertyMap.
							cstrName = bstrReturnValue;
							
						IconType type = EXEMPLAR_METHOD;
							if (m_lFilterType & FILTER_FRIENDLY_METHODS)
							{
								if (bFriendly)
									AddItem( pSec, cstrName, type );
							}
							else
								AddItem( pSec, cstrName, type );
						}
					}
					// We had a non-security failure getting security
					else if (hr!=VWOBJECT_E_PERMISSION_DENIED)
						goto failexit;	
					
					SAFERELEASE(pThing);
				}

				hr = pExemplarMethodMap->NextItem( &bstrReturnValue.m_str, &varProperty, &fLastItem );
				if (NONSECURITYFAIL(hr)) goto failexit;

				pSec.Release();
				bAddItem = TRUE;
			}
		}
	}

	m_listCtrl.Sort();
	m_listCtrl.ColumnResize();

failexit:

	return;
}

// LaunchExternalEditor
// iItem - item that the button was on
void CTVListCtrl::LaunchExternalEditor(int iItem)
{
	CPropListData *cpld = NULL;
	cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	if (!cpld)
		return;

	FireEditValue(m_pThing, cpld->m_strName);
}


BOOL CTVListCtrl::DealWithCopyOnGet(CPropListData *cpld)
{
	IPropertySecurity *pSec = NULL;
	long lBits = 0;
	BOOL bCopyOnGet = FALSE;
	HRESULT hr = S_OK;
	CMethodPtr pMethod;

	if (!cpld || !cpld->m_bEditable)
		goto failexit;
	
	if (ISEXEMPLAR(cpld->m_type))
	{
		if (FAILED(m_pThing->get_MethodSecurity(CComBSTR(cpld->m_strName), &pSec)))
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

			// get the method
			hr = m_pThing->get_Method(CComBSTR(cpld->m_strName), &pMethod.p);
			if (pMethod)
				var = (IDispatch *) pMethod;
			else
				goto failexit;

			// copy it
			if (m_pWorld)
				hr = m_pWorld->CloneProperty(var, &cloneVar);
			if (FAILED(hr))
				goto failexit;

			// add it to the instance
			if (FAILED(hr = m_pThing->put_Method(CComBSTR(cpld->m_strName), (IMethod *)cloneVar.pdispVal)))
				goto failexit;
		}
	}

	return TRUE;

failexit:
	SAFERELEASE(pSec);
	return FALSE;
}

//
// DoubleClick
// iItem - item that was clicked on
// iColumn - column the double click was in
//
// called when a user double clicks in the list control
void CTVListCtrl::DoubleClick(int iItem, int iColumn)
{
	if (iItem < 0)
		return;

	CPropListData *cpld = NULL;
	cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	// don't pop up the edit box if this isn't a method
	// from a script module
	if (!cpld || !cpld->m_bEditable || !cpld->m_bScriptMethod)
		return;

	// this might be useful, can't depend on it's value though, it might get deleted
	m_pCurrentlyEditing = cpld;

	if (cpld->m_bInLineMethod)
		FireEditValue(m_pThing, cpld->m_strName); // launch the method editor
	else
		m_listCtrl.EditValue(iItem, cpld->m_strValue);	// show the editor box
}

//
// LeftClick
// iItem - item that was clicked on
// iColumn - column the double click was in
//
// called when a user does a second left click in the list control
void CTVListCtrl::LeftClick(int iItem, int iColumn)
{
	if (iItem < 0)
		return;

	CPropListData *cpld = NULL;
	UINT state;

	cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	if (!cpld)
		return;

	// check to see if the item was already selected
	state = m_listCtrl.GetItemState(iItem, LVIS_SELECTED);
	if (state != LVIS_SELECTED)
		return;

	// this might be useful, can't depend on it's value though, it might get deleted
	m_pCurrentlyEditing = cpld;

	// in the first column edit the name of the method
	// in the third column show the drop down list of script modules
	// in the fourth column show the edit box if this is a script module
	switch (iColumn)
	{
		case 0:
			// do nothing
			break;
		case 1:
			if (!m_bCanAddToThing || !m_bCanPutOnThing)
				return;
			
			if ((cpld->m_type == EXEMPLAR_METHOD) || !ISWRITABLE(cpld->m_type))
				return;

			m_listCtrl.EditName(iItem, cpld->m_strName, 1);
			break;
		case 2:
			// get the module drop down
			// do we need to check anything else?
			if (!cpld->m_bEditable)
				return;
		
			OnChangeModule();
			break;
		case 3:
			// 4th column
			// don't pop up the edit box if this isn't a method
			// from a script module
			if (!cpld->m_bEditable)
				return;

			if (!cpld->m_bScriptMethod || !cpld->m_bInLineMethod)
				return;

			// show the editor box
			m_listCtrl.EditValue(iItem, cpld->m_strValue);
			break;
		default:
			break;
	}
}

//
// GetNameOfSelectedItem
//
// finds the first selected item and returns it's name
CString CTVListCtrl::GetNameOfSelectedItem()
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
void CTVListCtrl::PopUpMenu(CPoint point)
{
	CMenu menu;
	CMenu *pPopup;
	
	if (!menu.LoadMenu(IDR_THING_METH_MENU))
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

void CTVListCtrl::VirtualKeyPressed(UINT nChar)
{
	switch (nChar)
	{
		case VK_DELETE:
			OnDeleteMethod();
			break;
		case VK_RETURN:
			EnterKeyPressed();
			break;
		default:
			FireKeyPressed(nChar);
	}
}

void CTVListCtrl::VirtualKeyUp(UINT nChar)
{
	FireVKeyUp(nChar);
}

//
// EnterKeyPressed
//
// called when a user hits the enter key in the list control
void CTVListCtrl::EnterKeyPressed()
{
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();

	if (uiSelected != 1)
	{
		return;
	}

	int iItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);

	CPropListData *cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);

	if (!cpld || !cpld->m_bEditable || !cpld->m_bScriptMethod)
		return;

	if (cpld->m_bInLineMethod)
		FireEditValue(m_pThing, cpld->m_strName); // launch the method editor
	else if (cpld->m_bScriptMethod)
		OnEditSubName();
}

//
// AlphaKeyPressed
//
// called when a user presses a letter key in the list control
void CTVListCtrl::AlphaKeyPressed(char cLetter)
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


// WORLD EVENTS
HRESULT CTVListCtrl::HandleOnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient)
{
	static CComBSTR bstrLeaveRoom(VW_LEAVEROOM_EVENT_STR);
	CThingPtr pThing;
	IThing *pUser = NULL;
	HRESULT hr = S_OK;

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

HRESULT CTVListCtrl::HandleOnMethodChanged(IThing *pthing, BSTR bstr, long lHint, VARIANT varHintData)
{
	HRESULT hr = S_OK;
	IMethod* pmethod = NULL;
	IPropertySecurity* pSec = NULL;
	IThing* pExemplar = NULL;
	IPropertyMap* pMap = NULL;
	CComVariant var;
	BOOL bIsMe = FALSE;
	BOOL bIsExemplar = FALSE;
	CString strName;
	VARIANT_BOOL vBool = VARIANT_FALSE;

	if (pthing == m_pThing)
		bIsMe = TRUE;

	if (!bIsMe && CheckIfParent(pthing, m_pThing))
	{
		bIsMe = TRUE;
		bIsExemplar = TRUE;
	}

	if (!bIsMe)
		goto exit;

	if (!m_listCtrl.m_hWnd)
		goto exit;
	
	strName = bstr;
	
	if (lHint != thRemove)
	{
		if (FAILED(hr = pthing->get_Method(bstr, &pmethod)))
			goto exit;
		if (FAILED(hr = pthing->get_MethodSecurity(bstr, &pSec)))
			goto exit;
	}

	if (lHint == thAdd)
	{
		short itType;
		BOOL bAdd = TRUE;
		
		if (bIsExemplar)
		{
			if (FAILED(hr = m_pThing->get_InstanceMethods(&pMap)))
				goto exit;
			if (FAILED(hr = pMap->IsValid(bstr, &vBool)))
				goto exit;

			if (vBool == VARIANT_TRUE)
			{
				// this is a new exemplar method, we need to change the type of the method that
				// is currently being displayed as an instance method
				itType = OVERRIDEN_METHOD;
				bAdd = FALSE;

				// find the existing property in the list
				int iItem;
				iItem = m_listCtrl.FindItemByName(strName);

				if (iItem != -1)
				{
					// ok, found the item, just change the iconType
					LV_ITEM lvi;

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
				itType = EXEMPLAR_METHOD;
		}
		else
			itType = INSTANCE_METHOD;

		if (bAdd)
			AddItem(pSec, strName, (IconType) itType);

		// resort the list
		m_listCtrl.Sort();
	}
	else if (lHint == thPut)
	{
		// the name can't have changed, but it might have changed from an in-line method
		// to a regular method, or vice-versa.  Easiest to just get the new type and
		// value and set them in the list and list item data.  Also it might be a new
		// method on the instance overriding an exemplar method, got to check for that
		// as well
		int iItem;
		iItem = m_listCtrl.FindItemByName(strName);

		if (iItem != -1)
		{
			// ok, found the item, just change the value, and possibly where it comes from
			// if we are overriding an exemplar prop

			LV_ITEM lvi;
			BOOL bScriptModule;

			CPropListData * pPropListData;
			pPropListData = (CPropListData *) m_listCtrl.GetItemData(iItem);

			// if this is a put on the object, then check to see if
			// it is an override of an exemplar prop
			if (!bIsExemplar && pPropListData && (pPropListData->m_type == EXEMPLAR_METHOD))
			{
				// it is indeed an override
				pPropListData->m_type = OVERRIDEN_METHOD;
				lvi.mask = LVIF_IMAGE;
				lvi.iItem = iItem;
				lvi.iSubItem = 0;
				lvi.iImage= OVERRIDEN_METHOD;
				m_listCtrl.SetItem(&lvi);
			}

			CString strType, strValue;
			BOOL bTypeChanged;
			BOOL bValueChanged;
			GetModuleAndSubFromMethod(pmethod, strType, strValue, bScriptModule);

			ASSERT(pPropListData);

			bTypeChanged = (strType != pPropListData->m_strType);
			bValueChanged = (strValue != pPropListData->m_strValue);
			pPropListData->m_bScriptMethod = bScriptModule;
			pPropListData->m_bInLineMethod = (strType == ":VBScript");
			if (pPropListData->m_bInLineMethod)
				strType = "Inline Module";

			if (bTypeChanged)
			{
				pPropListData->m_strType = strType;
				lvi.mask = LVIF_TEXT;
				lvi.iItem = iItem;
				lvi.iSubItem = 2;
				lvi.pszText = strType.GetBuffer(strType.GetLength() + 1);
				lvi.cchTextMax = strType.GetLength() + 1;
				m_listCtrl.SetItem(&lvi);
			}

			if (bValueChanged)
			{
				pPropListData->m_strValue = strValue;
				lvi.mask = LVIF_TEXT;
				lvi.iItem = iItem;
				lvi.iSubItem = 3;
				lvi.pszText = strValue.GetBuffer(strValue.GetLength() + 1);
				lvi.cchTextMax = strValue.GetLength() + 1;
				m_listCtrl.SetItem(&lvi);
			}	
		}
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
				// see if this is an overidden prop, if so just update the icon, maybe
				if (FAILED(hr = pthing->IsValidMethod(bstr, &vBool)))
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
						if (lvi.iImage == OVERRIDEN_METHOD)
						{
							lvi.iImage = INSTANCE_METHOD;
							m_listCtrl.SetItem(&lvi);
							CPropListData * pPropListData;
							pPropListData = (CPropListData *) m_listCtrl.GetItemData(iItem);
							if (pPropListData)
								pPropListData->m_type = INSTANCE_METHOD;
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

				if (FAILED(hr = m_pThing->IsValidMethod(bstr, &vBool)))
					goto exit;

				if (vBool == VARIANT_TRUE)
				{
					// this has got to be an exemplar prop.
					// since it is an exemplar property and might just be copy on get, we can't
					// get it from the thing.  we have to get it from the thing's exemplar.
					//if (FAILED( hr = m_pThing->get_Exemplar(&pExemplar)))
					//	goto exit;
					//if (FAILED( hr = pExemplar->get_Methods(&pMap)))
					//	goto exit;
					//if (FAILED( hr = pMap->get_Property(bstr, &var)))
					//	goto exit;
					if (pSec)
						SAFERELEASE(pSec);

					if (FAILED(hr = pthing->get_MethodSecurity(bstr, &pSec)))
						goto exit;

					IconType type = EXEMPLAR_METHOD;
					AddItem(pSec, strName, type);
					m_listCtrl.Sort();
				}
			}
		}
	}
	else if ((lHint == meModule) || (lHint == meMethodName))
	{
		// fix column2
		int iItem;
		iItem = m_listCtrl.FindItemByName(strName);

		if (iItem != -1)
		{
			// ok, found the item, just change the module

			LV_ITEM lvi;
			CPropListData * pPropListData;
			pPropListData = (CPropListData *) m_listCtrl.GetItemData(iItem);

			CString strType, strValue;
			BOOL bTypeChanged;
			BOOL bValueChanged;
			BOOL bInLine;
			GetModuleAndSubFromMethod(pmethod, strType, strValue, bInLine);

			ASSERT(pPropListData);

			bTypeChanged = (strType != pPropListData->m_strType);
			bValueChanged = (strValue != pPropListData->m_strValue);
			pPropListData->m_bInLineMethod = (strType == ":VBScript");
			if (pPropListData->m_bInLineMethod)
				strType = "Inline Module";

			if (bTypeChanged)
			{
				pPropListData->m_strType = strType;
				lvi.mask = LVIF_TEXT;
				lvi.iItem = iItem;
				lvi.iSubItem = 2;
				lvi.pszText = strType.GetBuffer(strType.GetLength() + 1);
				lvi.cchTextMax = strType.GetLength() + 1;
				m_listCtrl.SetItem(&lvi);
			}

			if (bValueChanged)
			{
				pPropListData->m_strValue = strValue;
				lvi.mask = LVIF_TEXT;
				lvi.iItem = iItem;
				lvi.iSubItem = 3;
				lvi.pszText = strValue.GetBuffer(strValue.GetLength() + 1);
				lvi.cchTextMax = strValue.GetLength() + 1;
				m_listCtrl.SetItem(&lvi);
			}	
		}
		m_listCtrl.Sort();
	}

	hr = CVWUIView::HandleOnMethodChanged(pthing, bstr, lHint, varHintData);

exit:
	SAFERELEASE(pmethod);
	SAFERELEASE(pExemplar);
	SAFERELEASE(pMap);
	SAFERELEASE(pSec);

	return hr;
}

HRESULT CTVListCtrl::HandleOnModuleChanged(BSTR bstrName, long lHint, VARIANT varHintData)
{
	m_bModuleListDirty = TRUE;

	if (lHint == moRemove)
	{
		// update the displayed methods
		CString strComModule;
		CString	strModule = bstrName;
		strComModule.LoadString(IDS_COM_MODULE);
		LV_ITEM lvi;
		
		// walk the list and find out if any of the methods are pointing at the deleted module
		for (int i = 0; i < m_listCtrl.GetItemCount(); i++)
		{
			CPropListData *cpld = (CPropListData *) m_listCtrl.GetItemData(i);
			if (cpld && (!strModule.CompareNoCase(cpld->m_strType) || !cpld->m_strType.CompareNoCase(strComModule + strModule)))
			{
				// found one
				// update the info
				cpld->m_strType.LoadString(IDS_DELETED_MODULE);

				// update the UI
				lvi.mask = LVIF_TEXT;
				lvi.iItem = i;
				lvi.iSubItem = 2;
				lvi.pszText = cpld->m_strType.GetBuffer(cpld->m_strType.GetLength() + 1);
				lvi.cchTextMax = cpld->m_strType.GetLength() + 1;
				m_listCtrl.SetItem(&lvi);
			}
		}
	}

	return CVWUIView::HandleOnModuleChanged(bstrName, lHint, varHintData);
}

// VWUIView events
HRESULT CTVListCtrl::HandleUIUpdate( enumVWUIEventTypes eTypes, IVWUIView*,VARIANT varHint )
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

// client events
HRESULT CTVListCtrl::HandleOnDisconnect(IDispatch* pWorld)
{
	SetTargetObjectProperty(NULL);

	return CVWUIView::HandleOnDisconnect(pWorld);
}

HRESULT CTVListCtrl::HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient)
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


// MENU STUFF

void CTVListCtrl::OnUpdateAddMethod(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bCanAddToThing);
}

void CTVListCtrl::OnAddMethod() 
{
	FireNewMethod(m_pThing);
}

void CTVListCtrl::OnDeleteMethod() 
{
//	FireDeleteMethod(m_pThing, NULL);
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

		if ((pld->m_type == INSTANCE_METHOD) || (pld->m_type == OVERRIDEN_METHOD))
		{
			hr = m_pThing->RemoveMethod(bstrName);
			if (NONSECURITYFAIL(hr))
				goto failexit;
			iLastItem--;
		}
	}

	m_listCtrl.SelectItem(iLastItem + 1);

failexit:
	// clean up
	SAFEFREESTRING(bstrName);
}

void CTVListCtrl::OnUpdateDeleteMethod(CCmdUI* pCmdUI) 
{
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();
	VARIANT_BOOL vBool = VARIANT_FALSE;
	HRESULT hr = S_OK;

	// REVIEW: since this is a multi-select operation possibly,
	// should I actually cycle through and make sure you can delete them
	// all, or is it ok to assume that if you can delete one, you can
	// dump the rest?
	if (uiSelected > 0)
	{
		int iItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);
		CPropListData *cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
		if (cpld->m_type == EXEMPLAR_METHOD)
			pCmdUI->Enable(FALSE);
		else
		{
			m_pThing->CheckMethodSecurity(CComBSTR(cpld->m_strName), PS_ADD, &vBool);
			if (SUCCEEDED(hr))
				pCmdUI->Enable(vBool == VARIANT_TRUE);
			else
				pCmdUI->Enable(FALSE);
		}
	}
	else
		pCmdUI->Enable(FALSE);
}

void CTVListCtrl::OnShowExemplarMethods() 
{
	if (m_lFilterType & FILTER_EXEMPLAR_METHODS)
		m_lFilterType = m_lFilterType & ~FILTER_EXEMPLAR_METHODS;
	else
		m_lFilterType = m_lFilterType | FILTER_EXEMPLAR_METHODS;

	ClearList();
	FillList();
}

void CTVListCtrl::OnUpdateShowExemplarMethods(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_lFilterType & FILTER_EXEMPLAR_METHODS);
}

void CTVListCtrl::OnShowInstanceMethods() 
{
	if (m_lFilterType & FILTER_INSTANCE_METHODS)
		m_lFilterType = m_lFilterType & ~FILTER_INSTANCE_METHODS;
	else
		m_lFilterType = m_lFilterType | FILTER_INSTANCE_METHODS;

	ClearList();
	FillList();
}

void CTVListCtrl::OnUpdateShowInstanceMethods(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_lFilterType & FILTER_INSTANCE_METHODS);
}

void CTVListCtrl::OnEditSubName() 
{
	int iItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);
	
	CPropListData *cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);

	if (cpld->m_bInLineMethod)
		FireEditValue(m_pThing, cpld->m_strName); // launch the method editor
}

void CTVListCtrl::OnUpdateEditSubName(CCmdUI* pCmdUI) 
{
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();
	if (uiSelected == 0)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	int iItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);

	CPropListData *cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);

	if ((uiSelected == 1) && cpld->m_bInLineMethod && cpld->m_bScriptMethod)
		pCmdUI->Enable(cpld->m_bEditable);
	else
		pCmdUI->Enable(FALSE);
}

void CTVListCtrl::OnChangeModule() 
{
	int iItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);

	CPropListData *cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);

	// show the module drop-down module list
	if (!cpld->m_bEditable)
		return;

	m_pCurrentlyEditing = cpld;

	m_listCtrl.EditModule(iItem, cpld->m_strType);
}

void CTVListCtrl::OnUpdateChangeModule(CCmdUI* pCmdUI) 
{
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();

	if (uiSelected == 0)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	int iItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);

	CPropListData *cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);

	if ((uiSelected == 1) && cpld)
		pCmdUI->Enable(cpld->m_bEditable);
	else
		pCmdUI->Enable(FALSE);
}

void CTVListCtrl::OnEditName()
{
	int iItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);
	
	if (!(iItem >= 0))
		return;

	CPropListData *cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
	m_listCtrl.EditName(iItem, cpld->m_strName, 1);
}

void CTVListCtrl::OnUpdateEditName(CCmdUI* pCmdUI)
{
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();

	if (uiSelected == 1)
	{
		int iItem = m_listCtrl.GetNextItem(-1, LVNI_SELECTED);
		CPropListData *cpld = (CPropListData *) m_listCtrl.GetItemData(iItem);
		pCmdUI->Enable(m_bCanAddToThing && m_bCanPutOnThing &&
						(cpld->m_type != EXEMPLAR_METHOD) &&
						ISWRITABLE(cpld->m_type));
	}
	else
		  pCmdUI->Enable(FALSE);
}

void CTVListCtrl::OnDestroy() 
{
	ClearList();

	CPropEdCtrl::OnDestroy();
}

void CTVListCtrl::DoFocus() 
{
	m_listCtrl.SetFocus();
	UINT uiSelected;
	uiSelected = m_listCtrl.GetSelectedCount();

	if (uiSelected == 0)
		m_listCtrl.SelectItem(0, m_listCtrl.m_iSortColumn);
}
