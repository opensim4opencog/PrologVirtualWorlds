// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// ThingCtl.cpp : Implementation of the CThingTreeCtrl ActiveX Control class.

#include "stdafx.h"
#include "objexplr.h"
#include <propbase.h>
#include "ThingCtl.h"

#include <vwevents.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CThingTreeCtrl, CVWUIView)

// put in because of KB Q140229
struct _TREEITEM{};

typedef CVWComPtr<IThing,NULL,&IID_IThing> CThingPtr;
typedef CVWComPtr<IScriptModule,NULL,&IID_IScriptModule> CModulePtr;

/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CThingTreeCtrl, CVWUIView)
	//{{AFX_MSG_MAP(CThingTreeCtrl)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CREATE_THING, OnCreateThing)
	ON_COMMAND(ID_DELETE_THING, OnDeleteThing)
	ON_UPDATE_COMMAND_UI(ID_DELETE_THING, OnUpdateDeleteThing)
	ON_COMMAND(ID_DELETE_MODULE, OnDeleteModule)
	ON_UPDATE_COMMAND_UI(ID_DELETE_MODULE, OnUpdateDeleteModule)
	ON_COMMAND(ID_RELOAD_MODULE, OnReloadModule)
	ON_UPDATE_COMMAND_UI(ID_RELOAD_MODULE, OnUpdateReloadModule)
	ON_COMMAND(ID_NEW_MODULE, OnNewModule)
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(ID_NEW_MODULE, OnUpdateNewModule)
	ON_WM_DESTROY()
	ON_COMMAND(ID_THING_PROPERTIES, OnThingProperties)
	ON_UPDATE_COMMAND_UI(ID_THING_PROPERTIES, OnUpdateThingProperties)
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CThingTreeCtrl, CVWUIView)
	//{{AFX_DISPATCH_MAP(CThingTreeCtrl)
	DISP_PROPERTY_EX(CThingTreeCtrl, "VWClient", GetVWClient, SetVWClient, VT_DISPATCH)
	DISP_FUNCTION(CThingTreeCtrl, "SelectThing", SelectThing, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION(CThingTreeCtrl, "DoFocus", DoFocus, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CThingTreeCtrl, CVWUIView)
	//{{AFX_EVENT_MAP(CThingTreeCtrl)
	EVENT_CUSTOM("NewThing", FireNewThing, VTS_NONE)
	EVENT_CUSTOM("LoadModule", FireLoadModule, VTS_NONE)
	EVENT_CUSTOM("KeyPressed", FireKeyPressed, VTS_I4)
	EVENT_CUSTOM("VKeyUp", FireVKeyUp, VTS_I4)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CThingTreeCtrl, "OBJEXPLR.ThingTreeCtrl.1",
	0xef7d6571, 0x161, 0x11d2, 0x88, 0xbe, 0, 0xc0, 0x4f, 0xc3, 0x2e, 0xf3)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CThingTreeCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DThingTree =
		{ 0xef7d656f, 0x161, 0x11d2, { 0x88, 0xbe, 0, 0xc0, 0x4f, 0xc3, 0x2e, 0xf3 } };
const IID BASED_CODE IID_DThingTreeEvents =
		{ 0xef7d6570, 0x161, 0x11d2, { 0x88, 0xbe, 0, 0xc0, 0x4f, 0xc3, 0x2e, 0xf3 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwThingTreeOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CThingTreeCtrl, IDS_THINGTREE, _dwThingTreeOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CThingTreeCtrl::CThingTreeCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CThingTreeCtrl

BOOL CThingTreeCtrl::CThingTreeCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_THINGTREE,
			IDB_THINGTREE,
			afxRegApartmentThreading,
			_dwThingTreeOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CThingTreeCtrl::CThingTreeCtrl - Constructor

CThingTreeCtrl::CThingTreeCtrl()
{
	InitializeIIDs(&IID_DThingTree, &IID_DThingTreeEvents);

	m_pImageList = NULL;
	m_pUser = NULL;

	m_ulFlags |= ADVISE_WORLD;

	m_hWorld = NULL;
	m_hRoom = NULL;
	m_hModules = NULL;

	m_bSelectedByMe = TRUE;
	m_bWizard = FALSE;

	m_treeCtrl.m_pOCX = this;
}


/////////////////////////////////////////////////////////////////////////////
// CThingTreeCtrl::~CThingTreeCtrl - Destructor

CThingTreeCtrl::~CThingTreeCtrl()
{
	if (m_pImageList)
		delete m_pImageList;

	SAFERELEASE(m_pUser);
}


/////////////////////////////////////////////////////////////////////////////
// CThingTreeCtrl::OnDraw - Drawing function

void CThingTreeCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	DoSuperclassPaint(pdc, rcBounds);
}


/////////////////////////////////////////////////////////////////////////////
// CThingTreeCtrl::DoPropExchange - Persistence support

void CThingTreeCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	CVWUIView::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CThingTreeCtrl::OnResetState - Reset control to default state

void CThingTreeCtrl::OnResetState()
{
	CVWUIView::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CThingTreeCtrl message handlers

int CThingTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	BOOL bCreate = FALSE;
	RECT rect;
	DWORD dwStyle = 0;
	CString strName;

	
	if (CVWUIView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	OnActivateInPlace(TRUE, NULL);
	
	// create any image list necessary here
	if (!m_pImageList)
	{
		m_pImageList = new CImageList;
		m_pImageList->Create(IDB_TREEICON, 16, 0, 0x00FFFFFF);
	}

	GetClientRect( &rect );
	dwStyle =  TVS_HASLINES | TVS_HASBUTTONS | WS_VISIBLE | WS_CHILD | TVS_SHOWSELALWAYS;

	bCreate = m_treeCtrl.Create( dwStyle, rect, this, 255);

	m_treeCtrl.SetImageList(m_pImageList,TVSIL_NORMAL);

	// add the world item
	strName.LoadString(IDS_WORLD);
	m_hWorld = m_treeCtrl.InsertItem(LPCTSTR(strName), IT_WORLD, IT_WORLD, TVI_ROOT, TVI_LAST);
	m_treeCtrl.SetItemData(m_hWorld, (DWORD) -1);
	
	// add the modules item
	strName.LoadString(IDS_MODULES);
	m_hModules = m_treeCtrl.InsertItem(LPCTSTR(strName), IT_EXEMPLAR, IT_EXEMPLAR, TVI_ROOT, TVI_LAST);
	m_treeCtrl.SetItemData(m_hModules, (DWORD) -1);

	if (m_pWorld)
	{
		FillWorldNode();
		FillRoomNode();
		m_treeCtrl.EnsureVisible(m_hWorld);
	}

	return bCreate;
}



void CThingTreeCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CVWUIView::OnSize(nType, cx, cy);
	
	if ( m_treeCtrl.m_hWnd )
		m_treeCtrl.SetWindowPos(&wndTop,0,0,cx,cy,SWP_NOZORDER );
}

void CThingTreeCtrl::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// find out where we clicked (on a thing, a module, or what)

	CMenu menu;
	CPoint temppoint;
	HTREEITEM hParent, hNextParent;
	HTREEITEM hItem = NULL;
	UINT uFlags = 0;
	
	// save the point for later
	temppoint = point;

	// tree uses client points
	m_treeCtrl.ScreenToClient(&temppoint);

	// now do a hittest on the tree
	hItem = m_treeCtrl.HitTest(temppoint, &uFlags);

	if (!hItem)
		return;

	// only select if we clicked on the icon or the label
//	if(!(uFlags & TVHT_ONITEM))
		// we arent right clicking on anything so return
//		return;

	m_treeCtrl.SelectItem(hItem);

	hParent = hItem;
	hNextParent = m_treeCtrl.GetParentItem( hItem );
	while (hNextParent != NULL)
	{
		hParent = hNextParent;
		hNextParent = m_treeCtrl.GetParentItem( hParent );
	}

	if (hParent == m_hRoom)
	{
		VERIFY(menu.LoadMenu(IDR_ROOM_MENU));
	}
	else if (hParent == m_hModules)
	{
		VERIFY(menu.LoadMenu(IDR_MODULE_MENU));
	}
	else
		return;

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

}

void CThingTreeCtrl::OnCreateThing() 
{
	FireNewThing();
}

void CThingTreeCtrl::OnDeleteThing() 
{
	if (!m_pWorld)
		return;

	// REVIEW: In 1.0 this was set up to delete multiple things, even though that
	// wasn't possible.  To fix bug #6372, I'm going to change that so that it will
	// only delete the first selected item

	HTREEITEM hSelected;
	long lID;
	HRESULT hr = S_OK;

	hSelected = m_treeCtrl.GetNextItem(m_hRoom, TVGN_CARET);

	if (hSelected)
	{
		// pop up dialog box to make sure that the user wants to delete the object
		// really this should store the strings in resources, but this is a special 1.5 fix
		if (MessageBox("Are you sure that you want to delete this object", "Delete Object", MB_YESNO) == IDYES)
		{
			lID = m_treeCtrl.GetItemData(hSelected);
			if (FAILED(hr = m_pWorld->DestroyObjectByID(lID)))
				return;

			// allow the updating code to remove the item from the tree
		}
	}
}

void CThingTreeCtrl::OnUpdateDeleteThing(CCmdUI* pCmdUI) 
{
	HTREEITEM	hSelected;
	IThing		*pThing = NULL;
	IThing		*pOwner = NULL;
	HRESULT		hr = S_OK;
	BOOL		bOwner = FALSE;
	BOOL		bEnable = FALSE;
	long		lID;
	long		lIcon;

	hSelected = m_treeCtrl.GetNextItem(m_hRoom, TVGN_CARET);

	if (!hSelected)
	{
		pCmdUI->Enable(FALSE);
		goto exit;
	}

	// get the thing
	lID = m_treeCtrl.GetItemData(hSelected);
	
	if (lID < 0)
		goto exit;

	if(m_pWorld)
		hr = m_pWorld->get_ObjectByID(lID, &pThing);
	else
	{
		pCmdUI->Enable(FALSE);
		goto exit;
	}

	if (FAILED(hr) || !pThing)
	{
		pCmdUI->Enable(FALSE);
		goto exit;
	}

	// is this a room or a avatar
	lIcon = AssignIcon(pThing);
	if ((lIcon == IT_ROOM) || (lIcon == IT_AVATAR))
	{
		pCmdUI->Enable(FALSE);
		goto exit;
	}

	if (!m_bWizard)
	{
		// need to check to see if we are the owner of this thing
		hr = pThing->get_Owner(&pOwner);
		if (SUCCEEDED(hr) && pOwner)
			bOwner = (pOwner == m_pUser);
	}

	pCmdUI->Enable(m_bWizard || bOwner);

exit:

	SAFERELEASE(pOwner);
	SAFERELEASE(pThing);
}

void CThingTreeCtrl::OnThingProperties() 
{
	// invoke the editobject method on the selected object
	HTREEITEM	hSelected;
	IThing		*pThing = NULL;
	HRESULT		hr = S_OK;
	long		lID;
	CComVariant varReturnVal;

	hSelected = m_treeCtrl.GetNextItem(m_hRoom, TVGN_CARET);

	if (!hSelected)
		goto exit;

	// get the thing
	lID = m_treeCtrl.GetItemData(hSelected);
	
	if (lID < 0)
		goto exit;

	if(m_pWorld)
		hr = m_pWorld->get_ObjectByID(lID, &pThing);
	else
		goto exit;

	if (FAILED(hr) || !pThing)
		goto exit;

	hr = pThing->InvokeMethodExt(CComBSTR("EditObject"), NULL, &varReturnVal);

exit:
	SAFERELEASE(pThing);
}

void CThingTreeCtrl::OnUpdateThingProperties(CCmdUI* pCmdUI) 
{
	HTREEITEM	hSelected;

	hSelected = m_treeCtrl.GetNextItem(m_hRoom, TVGN_CARET);

	if (!hSelected)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

void CThingTreeCtrl::OnDeleteModule() 
{
	HTREEITEM hSelected;
	CString	strModuleName;
	HRESULT hr = S_OK;

	if (!m_pWorld)
		return;

	CString strMessage;
	CString strTitle;

	strMessage.LoadString(IDS_EXEMPLARS_NOT_REMOVED);
	strTitle.LoadString(IDS_REMOVE_MODULE);

	if (MessageBox(strMessage, strTitle, MB_OKCANCEL) != IDOK)
		return;

	hSelected = m_treeCtrl.GetNextItem(m_hModules, TVGN_CARET);

	if (!hSelected)
		return;

	// get the module name from the tree
	strModuleName = m_treeCtrl.GetItemText(hSelected);

	// delete the module from the world
	hr = m_pWorld->RemoveModule(CComBSTR(strModuleName));
}

void CThingTreeCtrl::OnUpdateDeleteModule(CCmdUI* pCmdUI) 
{
	HTREEITEM hSelected;
	HTREEITEM hParent;

	hSelected = m_treeCtrl.GetNextItem(m_hModules, TVGN_CARET);
	hParent = m_treeCtrl.GetParentItem(hSelected);

	if ((!hSelected) || (m_hModules == hSelected) || (hParent != m_hModules))
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(m_bWizard);
}

void CThingTreeCtrl::OnReloadModule() 
{
	// FireReLoadModule();
	HTREEITEM hSelected;
	CString	strModuleName;
	IModule *pModule = NULL;
	HRESULT hr = S_OK;

	if (!m_pWorld)
		return;

	hSelected = m_treeCtrl.GetNextItem(m_hModules, TVGN_CARET);

	if (!hSelected)
		return;

	// get the module name from the tree
	strModuleName = m_treeCtrl.GetItemText(hSelected);

	// get the module from the world
	if (FAILED(hr = m_pWorld->get_Module(CComBSTR(strModuleName), &pModule)))
		goto exit;

	// call reload on the module
	if (FAILED(hr = pModule->Reload(VARIANT_TRUE)))
	{
		CComBSTR bstrDescription("Error reloading module.");
		CComPtr<IErrorInfo> pErrorInfo;
		CString strError;
		CString strTitle;
		CString strInfo;

		strTitle.LoadString(IDS_RELOAD_MODULE);
		strInfo.LoadString(IDS_ERROR_MODULE_RELOAD);

		::GetErrorInfo(0L, &pErrorInfo);

		// we possibly could have no errorinfo from server-side/sync method,
		// if HRESULT isn't in the IThing range (the ReportThingError above will format
		// error strings in the proper range)
		if (pErrorInfo)
		{
			//pErrorInfo->GetSource(&bstrSource.m_str);
			pErrorInfo->GetDescription(&bstrDescription.m_str);
		}
		strError = strInfo + CString(bstrDescription);

		MessageBox(strError, strTitle, MB_ICONERROR);
	}


exit:
	SAFERELEASE(pModule);
}

void CThingTreeCtrl::OnUpdateReloadModule(CCmdUI* pCmdUI) 
{
	HTREEITEM hSelected;
	HTREEITEM hParent;

	hSelected = m_treeCtrl.GetNextItem(m_hModules, TVGN_CARET);
	hParent = m_treeCtrl.GetParentItem(hSelected);

	if (!hSelected || (hSelected == m_hModules) || (hParent != m_hModules))
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(m_bWizard);
}

void CThingTreeCtrl::OnNewModule() 
{
	FireLoadModule();
}


void CThingTreeCtrl::OnUpdateNewModule(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bWizard);
}


void CThingTreeCtrl::OnDestroy() 
{
	CVWUIView::OnDestroy();

}

//
// Ole Properties and methods
//

LPDISPATCH CThingTreeCtrl::GetVWClient() 
{
	return CVWUIView::GetVWClient();
}

void CThingTreeCtrl::SetVWClient(LPDISPATCH newValue) 
{
	HRESULT hr = S_OK;

	CVWUIView::SetVWClient(newValue);

	if (m_pVWClient && m_pWorld)
	{
		// get the world pointer and fill in the world tree
//		hr = m_pVWClient->get_World(&m_pWorld);
//		if (SUCCEEDED(hr) && m_pWorld)
//		{
			// Fill Room get called by the UserConnect
			//FillWorldNode();
			//FillRoomNode();
			if (m_treeCtrl.m_hWnd)
				m_treeCtrl.EnsureVisible(m_hWorld);

			// we already should have gotten the user in the SetVWClient
			if (!m_pUser)
			{
				hr = m_pWorld->get_User(&m_pUser);
				if (FAILED(hr) || !m_pUser)
					m_pUser = NULL;
			}
			if (m_pUser)
			{
				VARIANT_BOOL vbWizard = VARIANT_FALSE;
				hr = m_pUser->get_IsWizard(&vbWizard);
				if (SUCCEEDED(hr))
					m_bWizard = (vbWizard == VARIANT_TRUE);
			}
//		}
	}
	
	SetModifiedFlag();
}

void CThingTreeCtrl::SelectThing(LPDISPATCH Thing) 
{
	IThing *pCurrentThing = NULL;
	HRESULT hr = S_OK;
	long lCurrentID;
	HTREEITEM htItem = NULL;

	if (FAILED(hr = Thing->QueryInterface(IID_IThing, (void**)&pCurrentThing)))
		goto exit;

	if(pCurrentThing)
	{
		if (FAILED(hr = pCurrentThing->get_ID(&lCurrentID)))
			goto exit;

		// Search for the htreeitem that corresponds these ids
		htItem = InTree(m_hRoom, lCurrentID);

		// if we found the item, select it and exit
		if(htItem)
		{
			// TODO - Make sure this doesn send a TVN_SELCHANGED notification!!!!
			// It does, so we have to regulate it with a bool
			m_bSelectedByMe = FALSE;
			m_treeCtrl.SelectItem(htItem);
		}
	}

exit:

	SAFERELEASE(pCurrentThing);
}

// called when the web page wants this control to start taking input
void CThingTreeCtrl::DoFocus() 
{
	if (m_treeCtrl.m_hWnd)
		m_treeCtrl.SetFocus();
}

//
// Class Methods
//

//
// InTree
// Given a place to start the search and an ID to look for
// do a depth-first search and
// return the HTREEItem associated with the ID or NULL
//
HTREEITEM CThingTreeCtrl::InTree(HTREEITEM hParent, long lID)
{
	// should we actually use this to search the whole tree?
	if (hParent == NULL)
		return NULL;

	long lCurrentId;
	HTREEITEM hCurrentChildItem;
	HTREEITEM hReturnItem = NULL;

	lCurrentId = (long) m_treeCtrl.GetItemData(hParent);

	if (lCurrentId == lID)
		return hParent;

	// not the parent, get the children
	if (!m_treeCtrl.ItemHasChildren(hParent))
		return NULL;

	hCurrentChildItem = m_treeCtrl.GetChildItem(hParent);

	// not the first child, loop through the rest
	while (hCurrentChildItem != NULL)
	{
		hReturnItem = InTree(hCurrentChildItem, lID);
		if (hReturnItem)
			return hReturnItem;

		hCurrentChildItem = m_treeCtrl.GetNextSiblingItem(hCurrentChildItem);
	}

	return hReturnItem;
}

//
// FindModuleByName
// given the name of a module in the tree
// return the HTREEItem associated with the it or NULL
//
HTREEITEM CThingTreeCtrl::FindModuleByName(CString strName)
{
	HTREEITEM hCurrentChildItem;
	CString strCurrentName;

	// not the parent, get the children
	if (!m_treeCtrl.ItemHasChildren(m_hModules))
		return NULL;

	hCurrentChildItem = m_treeCtrl.GetChildItem(m_hModules);

	// not the first child, loop through the rest
	while (hCurrentChildItem != NULL)
	{
		strCurrentName = m_treeCtrl.GetItemText(hCurrentChildItem);
		if (strCurrentName.CompareNoCase(strName) == 0)
			return hCurrentChildItem;

		hCurrentChildItem = m_treeCtrl.GetNextSiblingItem(hCurrentChildItem);
	}

	return NULL;
}

//
// AddThingToTree
// given a parent item and a thing pointer
// add the thing to three
HTREEITEM CThingTreeCtrl::AddThingToTree(HTREEITEM hParentItem, IThing *pThing	)
{
	CComBSTR bstrName;
	CString strName;
	long lID, lIcon;
	HTREEITEM hReturn = NULL;
	TV_INSERTSTRUCT tvInsert;
	TV_ITEM tvItem;
	HRESULT hr = S_OK;

	if (!pThing)
		return NULL;

	if (FAILED(hr = pThing->get_ID(&lID)))
		goto exit;

	if (FAILED(hr = pThing->get_Name(&bstrName.m_str)))
		goto exit;

//#ifdef _DEBUG
//	{
//		TRACE("CThingTreeCtrl::AddThingToTree %s \n", CString(bstrName));
//	}
//#endif

	lIcon = AssignIcon(pThing);

	tvItem.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	strName = bstrName;
	tvItem.pszText = LPTSTR(LPCTSTR(strName));
	tvItem.lParam = lID;
	tvItem.iImage = tvItem.iSelectedImage = lIcon;

	tvInsert.hParent = hParentItem;
	tvInsert.hInsertAfter = TVI_LAST;
	tvInsert.item = tvItem;

	hReturn = m_treeCtrl.InsertItem(&tvInsert);

exit:

	return hReturn;
}

//
// fill the world node of the tree
//
void CThingTreeCtrl::FillWorldNode()
{
	HRESULT			hr = S_OK;
	IPropertyMap	*pModuleMap = NULL;
	CModulePtr		pModule;
	VARIANT_BOOL	vbDone = VARIANT_FALSE;
	CComBSTR		bstrModuleName;
	CComVariant		varModule;
	CThingPtr		pThing;
	IThing			*pGlobal = NULL;
	CString			strModuleName;
	long			lID;
	TV_INSERTSTRUCT tvInsert;
	TV_ITEM			tvItem;

	if (!m_pWorld)
		return;

	// check to see if we've already set the global thing ID on the world tree item
	if (m_hWorld)
	{
		lID = (long) m_treeCtrl.GetItemData(m_hWorld);
		if (lID == -1)
		{
			if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
				goto exit;

			if (!pGlobal || FAILED(pGlobal->get_ID(&lID)))
				goto exit;

			m_treeCtrl.SetItemData(m_hWorld, lID);
			
			SAFERELEASE(pGlobal);
		}
	}

	// fill in the modules
	if (FAILED(m_pWorld->get_Modules(&pModuleMap)) || !pModuleMap)
		goto exit;

	// walk through the list
	if (FAILED(hr = pModuleMap->FirstItem(&bstrModuleName.m_str, &varModule, &vbDone)))
		goto exit;

	while (vbDone == VARIANT_FALSE)
	{
		pModule = varModule;
		if (pModule)
		{
			// it's a script module (we QI for it in CModulePtr)
			tvItem.mask = TVIF_PARAM | TVIF_TEXT;
			strModuleName = bstrModuleName;
			tvItem.pszText = LPTSTR(LPCTSTR(strModuleName));
			tvItem.lParam = -2;

			tvInsert.hParent = m_hModules;
			tvInsert.hInsertAfter = TVI_LAST;
			tvInsert.item = tvItem;

			m_treeCtrl.InsertItem(&tvInsert);
		}

		if (FAILED(hr = pModuleMap->NextItem(&bstrModuleName.m_str, &varModule, &vbDone)))
			goto exit;
	}

	m_treeCtrl.Expand(m_hWorld, TVE_EXPAND);
	m_treeCtrl.Expand(m_hModules, TVE_EXPAND);
	m_treeCtrl.SortChildren(m_hModules);

exit:
	SAFERELEASE(pModuleMap);
	SAFERELEASE(pGlobal);
	return;
}


//
// AssignIcon
// given a thing, find an exemplar for it that we have
// an icon for.
//
long CThingTreeCtrl::AssignIcon(IThing *pThing)
{
	HRESULT hr = S_OK;
	CComBSTR bstrType;
	CString strType;
	long lIcon = IT_UNKNOWN;
	IThing *pExemplar = NULL;

	if (!pThing)
		return lIcon;

	if (FAILED(hr = pThing->get_Type(&bstrType.m_str)))
		goto exit;

	strType = bstrType;

	if(!strType.CompareNoCase("world"))
		lIcon =  IT_WORLD;
	else if(!strType.CompareNoCase("room"))
		lIcon = IT_ROOM;
	else if(!strType.CompareNoCase("avatar"))
		lIcon = IT_AVATAR;
	else if(!strType.CompareNoCase("artifact"))
		lIcon =  IT_ARTIFACT;
	else if(!strType.CompareNoCase("portal"))
		lIcon =  IT_PORTAL;
	else if(!strType.CompareNoCase("thing"))
		lIcon = IT_THING;
	else
	{
		// we don't have an icon for this type,
		// try it's exemplar
		if (FAILED(hr = pThing->get_Exemplar(&pExemplar)))
			goto exit;

		if (pExemplar)
			lIcon = AssignIcon(pExemplar);
	}

exit:
	SAFERELEASE(pExemplar);
	return lIcon;
}

//
// fill the room node of the tree
//
void CThingTreeCtrl::FillRoomNode()
{
	HRESULT hr = S_OK;
	IThing *pUser = NULL;
	IThing *pRoom = NULL;

	if (!m_pWorld)
		return;

	if (FAILED(hr = m_pWorld->get_User(&pUser)))
		goto exit;

	if (!pUser || FAILED(hr = pUser->get_Container(&pRoom)))
		goto exit;

	if (!pRoom)
		goto exit;

	m_hRoom = AddThingToTree(TVI_ROOT, pRoom);
	if (m_hRoom)
		AddContentsToTree(m_hRoom, pRoom);

	m_treeCtrl.Expand(m_hRoom, TVE_EXPAND);
	m_treeCtrl.SortChildren(m_hRoom);

exit:
	SAFERELEASE(pUser);
	SAFERELEASE(pRoom);
}

//
// AddContentsToTree
// given a thing and a HTREEITEM
// goes through the contents of the thing and adds them to the tree
void CThingTreeCtrl::AddContentsToTree(HTREEITEM hParent, IThing *pParent)
{
	if (!pParent)
		return;

	HRESULT hr = S_OK;
	IPropertyList *pContents = NULL;
	long lCount, i;
	IObjectProperty *pProp = NULL;
	IThing *pThing = NULL;
	HTREEITEM hCurrentItem;
	VARIANT_BOOL vbStub = VARIANT_FALSE;

	if (FAILED(hr = pParent->get_Contents(&pContents)))
		goto exit;

	if (!pContents)
		goto exit;

	if (FAILED(hr = pContents->get_Count(&lCount)))
		goto exit;

	for (i = 0; i < lCount; i++)
	{
		if (FAILED(hr = pContents->get_ObjectPropertyExt(i, IID_IThing, (IObjectProperty **) &pProp)))
			goto exit;
		
		if (!pProp)
			goto exit;
		
		if (FAILED(hr = pProp->QueryInterface(IID_IThing, (LPVOID*)&pThing)))
			goto exit;

		if (FAILED(hr = pThing->get_IsStub(&vbStub)))
			goto exit;
		
		if (vbStub == VARIANT_TRUE)
			goto exit;

		// add the thing to the the tree
		hCurrentItem = AddThingToTree(hParent, pThing);
		if (!hCurrentItem)
			goto exit;

		AddContentsToTree(hCurrentItem, pThing);
		m_treeCtrl.SortChildren(hCurrentItem);
		SAFERELEASE(pProp);
		SAFERELEASE(pThing);
		pThing = NULL;
	}

exit:
	SAFERELEASE(pProp);
	SAFERELEASE(pThing);
	SAFERELEASE(pContents);
}

void CThingTreeCtrl::VirtualKeyPressed(UINT nChar)
{
	if (nChar == VK_DELETE)
		OnDeleteThing();	
	else
		FireKeyPressed(nChar);
}

void CThingTreeCtrl::VirtualKeyUp(UINT nChar)
{
	FireVKeyUp(nChar);
}

//
// NewSelection
// called by the tree
// handles adding the item to the selection list
void CThingTreeCtrl::NewSelection(HTREEITEM hSelection)
{
	long lID = 0;
	IThing* pThing = NULL;
	CComVariant var;
	HRESULT hr = S_OK;

	lID = m_treeCtrl.GetItemData(hSelection);
	
	if (lID < 0)
		return;

	// get the thing from the ID
	if(m_pWorld)
		hr = m_pWorld->get_ObjectByID(lID, &pThing);

	// package the thing to send back to the selection list
	if(pThing)
	{
		var = pThing;

		if(m_pControlManager)
		{
			IVWControlManager* pControlManager;
			IVWUIView* pVWUIView;

			hr = m_pControlManager->QueryInterface(IID_IVWControlManager, (LPVOID*) &pControlManager);

			if (FAILED(hr))
			{
				SAFERELEASE(pControlManager);
				goto EXIT;
			}

			hr = m_pVWUIView->QueryInterface(IID_IVWUIView, (LPVOID*) &pVWUIView);
			if (FAILED(hr))
			{
				SAFERELEASE(pVWUIView);
				goto EXIT;
			}

			hr = pControlManager->ClearSelectionsExt(pVWUIView);
			if (FAILED(hr))
			{
#ifdef _DEBUG
				VWTRACE(m_pWorld, "OBJEXPLR", VWT_ERROR, "CThingTreeCtrl::NewSelection: ClearSelectionsExt failed\n");
#endif
			}

			hr = pControlManager->AddSelectionExt(pVWUIView, var);
			if (FAILED(hr))
			{
#ifdef _DEBUG
				VWTRACE(m_pWorld, "OBJEXPLR", VWT_ERROR, "CThingTreeCtrl::NewSelection: AddSelectionExt failed\n");
#endif
			}
			
			SAFERELEASE(pControlManager);
			SAFERELEASE(pVWUIView);

		}
	}

EXIT:
	SAFERELEASE(pThing);


}

//
// CVWUIView Events
//
HRESULT CThingTreeCtrl::HandleUIUpdate( enumVWUIEventTypes enumEventType, IVWUIView* pvwSrc,VARIANT varHint )
{
	HRESULT hr = S_OK;
	IThing* pCurrentThing = NULL;
	long lCurrentID = 0;
	IDispatch* pDisp = NULL;
	HTREEITEM htItem = NULL;
	long lID = 0;

	if (enumEventType != VWUIT_ADDSELECTION)
		return S_OK;

	// if we put this selection in, dont do anything
	if (pvwSrc == m_pVWUIView)
	{
		m_bSelectedByMe = TRUE;
		goto EXIT;
	}

	// otherwise, look for this selection in the current tree
	// and if it is there, select it
	
	// first get the id of the current thing
	if(varHint.vt == VT_DISPATCH)
	{
		pDisp = varHint.pdispVal;

		if(pDisp)
		{
			hr = pDisp->QueryInterface(IID_IThing, (void**)&pCurrentThing);

			if(pCurrentThing)
			{
				hr = pCurrentThing->get_ID(&lCurrentID);
				if(FAILED(hr)) goto EXIT;

				// Search for the htreeitem that corresponds these ids
				htItem = InTree(m_hRoom, lCurrentID);

				// if we found the item, select it and exit
				if(htItem)
				{
					// TODO - Make sure this doesn't send a TVN_SELCHANGED notification!!!!
					// It does, so we have to regulate it with a bool
					m_bSelectedByMe = FALSE;
					// ha! check to see if it's selected, because if it is already
					// we won't get the selection changed message to turn on m_bSelectedByMe!
					UINT bits;
					bits = m_treeCtrl.GetItemState(htItem, TVIS_SELECTED);
					if (!(bits & TVIS_SELECTED))
						m_treeCtrl.SelectItem(htItem);
					else
						m_bSelectedByMe = TRUE;
					goto EXIT;
				}

				// if its not in the tree, walk its location and insert it into the tree and select it
				
			}
		}
	}

EXIT:
	SAFERELEASE(pCurrentThing);
	return S_OK;
}

//
// World Events
//
HRESULT CThingTreeCtrl::HandleOnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient)
{
	static CComBSTR bstrNameChanged(VW_NAMECHANGED_EVENT_STR);
	static CComBSTR bstrOnEnter(VW_CONTENTENTER_EVENT_STR);
	static CComBSTR bstrOnLeave(VW_CONTENTLEAVE_EVENT_STR);

	CVWUIView::HandleOnUIEvent(pthing, bstrEventName, varArg, bFromClient);

	if (CompareElements(&bstrOnEnter, &bstrEventName))
		return OnEnter(pthing, varArg);
	else if (CompareElements(&bstrOnLeave, &bstrEventName))
		return OnLeave(pthing, varArg);
	else if (CompareElements(&bstrNameChanged, &bstrEventName))
		return NameChanged(pthing);

	return S_OK;
}

HRESULT CThingTreeCtrl::HandleOnModuleChanged(BSTR bstrName, long lHint, VARIANT varHintData)
{
	TV_INSERTSTRUCT tvInsert;
	TV_ITEM			tvItem;
	CString			strModuleName;
	HTREEITEM		hItem;

	strModuleName = bstrName;

	// the module has changed, find out which one
	if (lHint == moCreate)
	{
		// create a new module
		tvItem.mask = TVIF_PARAM | TVIF_TEXT;
		tvItem.pszText = LPTSTR(LPCTSTR(strModuleName));
		tvItem.lParam = -2;

		tvInsert.hParent = m_hModules;
		tvInsert.hInsertAfter = TVI_LAST;
		tvInsert.item = tvItem;

		hItem = m_treeCtrl.InsertItem(&tvInsert);

		// check to see if the module node is expanded
		// if not, expand it
		m_treeCtrl.Expand(m_hModules, TVE_EXPAND);
		m_treeCtrl.SortChildren(m_hModules);

		// select the module (as per bug #5044)
		m_treeCtrl.SelectItem(hItem);
	}
	if (lHint == moRemove)
	{
		// remove an existing module
		// find the module in the tree
		hItem = FindModuleByName(strModuleName);
		if (hItem)
			m_treeCtrl.DeleteItem(hItem);
	}
	
	return S_OK;
}

HRESULT CThingTreeCtrl::HandleOnUserConnect(IThing* pthing, BOOL bFromClient)
{
	HRESULT	hr = S_OK;

	if (!m_pUser && bFromClient && m_pVWClient)
	{
		// get the world pointer and fill in the world tree
		SAFERELEASE(m_pWorld);
		hr = m_pVWClient->get_World(&m_pWorld);
		if (SUCCEEDED(hr) && m_pWorld)
		{
			if (m_treeCtrl.m_hWnd)
			{
				FillWorldNode();
				FillRoomNode();
				m_treeCtrl.EnsureVisible(m_hWorld);
			}

			hr = m_pWorld->get_User(&m_pUser);
			if (FAILED(hr) || !m_pUser)
				m_pUser = NULL;
			else
			{
				VARIANT_BOOL vbWizard = VARIANT_FALSE;
				hr = m_pUser->get_IsWizard(&vbWizard);
				if (SUCCEEDED(hr))
					m_bWizard = (vbWizard == VARIANT_TRUE);
			}
		}
	}

	return hr;
}

HRESULT CThingTreeCtrl::HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient)
{
	HRESULT	hr = S_OK;

	if (m_pUser == pthing)
	{
		// clear out the variables?
		SAFERELEASE(m_pUser);
		m_treeCtrl.DeleteAllItems();
		// re-add the header items

		CString strName;

		// add the world item
		strName.LoadString(IDS_WORLD);
		m_hWorld = m_treeCtrl.InsertItem(LPCTSTR(strName), IT_WORLD, IT_WORLD, TVI_ROOT, TVI_LAST);
		m_treeCtrl.SetItemData(m_hWorld, (DWORD) -1);
		
		// add the modules item
		strName.LoadString(IDS_MODULES);
		m_hModules = m_treeCtrl.InsertItem(LPCTSTR(strName), IT_EXEMPLAR, IT_EXEMPLAR, TVI_ROOT, TVI_LAST);
		m_treeCtrl.SetItemData(m_hModules, (DWORD) -1);

	}

	hr = CVWUIView::HandleOnUserDisconnect(pthing, bFromClient);

	return hr;
}

HRESULT CThingTreeCtrl::NameChanged(IThing *pthing)
{
	long lID = 0;
	HTREEITEM hItem = NULL;
	HTREEITEM hParentItem = NULL;
	HRESULT hr = S_OK;
	CComBSTR bstrName;
	CString strName;

	// we should update the item in question
	hr = pthing->get_ID(&lID);
	// now lets go find the thing with the different name
	if (lID > 0)
	{
		// first find the thing that is leaving
		hItem = InTree(m_hRoom, lID);

		if(hItem != NULL)
		{
			// get the new name from the thing and put it in the tree
			hr = pthing->get_Name(&bstrName.m_str);
			strName = bstrName;

			m_treeCtrl.SetItemText(hItem, strName);

			// get the parent item
			hParentItem = m_treeCtrl.GetParentItem( hItem );

			// sort
			m_treeCtrl.SortChildren(hParentItem);
			
			hr = S_OK;
		}
	}

	return hr;
}

HRESULT CThingTreeCtrl::OnEnter(IThing *pthing, CComVariant varArg)
{
	HRESULT		hr = S_OK;
	CThingPtr	pWhat;
	long lWhereID = 0;
	long lWhatID = 0;
	CComBSTR bstrName;
	CString strName;
	IThing		*pUser = NULL;
	long lIcon = 0;
	HTREEITEM hItem = NULL;
	HTREEITEM hSubItem = NULL;
	UINT uState = 0;

	pWhat = varArg;
	if (!pWhat)
		goto exit;

	if (FAILED(hr = m_pWorld->get_User(&pUser)))
		goto exit;

	if (pWhat != pUser)
	{
		if (FAILED(hr = pthing->get_ID(&lWhereID)))
			goto exit;
		
		if(lWhereID > 0)
		{
			// first find the thing that contains what is entering
			hItem = InTree(m_hRoom, lWhereID);

			hSubItem = AddThingToTree(hItem, pWhat);

			if (hSubItem)
			{
				AddContentsToTree(hSubItem, pWhat);
			}

			m_treeCtrl.SortChildren(hItem);
		}
	}
	else
	{
		// this is me entering a new room, fill in the room part of the tree
		// again
		FillRoomNode();
		// Select the room
		if (m_hRoom)
			m_treeCtrl.SelectItem(m_hRoom);
	}
exit:

	SAFERELEASE(pUser);
	return hr;
}

HRESULT CThingTreeCtrl::OnLeave(IThing *pthing, CComVariant varArg)
{
	// Get the id of the thing that's leaving the inventory (pWhat) and of its parent (pWhere)
	// Search the tree for these.  It should be in the tree someplace
	// remove from the tree.  Shouldnt need to update anything else
	long lWhatID = 0;
	HRESULT hr = S_OK;
	HTREEITEM hItem = NULL;
	CThingPtr	pWhat;
	IThing		*pUser = NULL;

	pWhat = varArg;
	if (!pWhat)
		goto exit;

	if (FAILED(hr = m_pWorld->get_User(&pUser)))
		goto exit;

	if (pWhat != pUser)
	{
		if (FAILED(hr = pWhat->get_ID(&lWhatID)))
			goto exit;

		if(lWhatID > 0)
		{
			// first find the thing that is leaving
			hItem = InTree(m_hRoom, lWhatID);

			if(hItem != NULL)
			{
				// remove from the tree
				m_treeCtrl.DeleteItem(hItem);
				hr = S_OK;
			}
		}
	}
	else
	{
		// hey, I'm leaving!  Delete the room node and wait for the
		// enter message to fill it back in
		m_treeCtrl.DeleteItem(m_hRoom);
		m_hRoom = NULL;
	}

exit:
	SAFERELEASE(pUser);
	return hr;
}

