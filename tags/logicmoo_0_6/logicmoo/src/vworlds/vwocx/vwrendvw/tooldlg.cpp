// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// ToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "vwrendvw.h"
#include "ToolDlg.h"
#include "resource.h"
#include <vwsutils.h>
#include <vwprop.h>
#include <uitlimpl.h>
#include <vwtrans_i.c>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
typedef CVWComPtr<IThing,NULL,&IID_IThing> CThingPtr;
typedef CVWComPtr<IVWFrame,NULL,&IID_IVWFrame> CVWFramePtr;
typedef CVWComPtr<IVWExecuteUndo, &CLSID_VWExecuteGeomUndo, &IID_IVWExecuteUndo> CExecUndoPtr;

/////////////////////////////////////////////////////////////////////////////
static CComBSTR	bstrPosition("Position");

/////////////////////////////////////////////////////////////////////////////
// CToolDialog dialog


CToolDialog::CToolDialog(CWnd* pParent /*=NULL*/)
//	: CDialogBar()
	: CDialog(CToolDialog::IDD, pParent), m_bbUndo(), m_bbRedo(), m_bbNavigate(), m_bbSelectContents(),
	  m_bbHelp(), m_bbSelect(), m_bbTranslate(), m_bbRotate(), m_bbScale(), 
	  m_bbCollisionToggle(), m_bbGravityToggle()
{
	bDialogInitialized = FALSE;
	m_pVWRenderRoot = NULL;
	m_bCollision = 0;
	m_bGravity = 0;
	m_bkBrush = CreateSolidBrush(RGB( 194,195,181 ));
//	m_bkBrush = CreateSolidBrush(RGB(192,193,172));

	//{{AFX_DATA_INIT(CToolDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CToolDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolDialog)
	//}}AFX_DATA_MAP
}


CToolDialog::~CToolDialog()
{
	if (m_bkBrush)
		DeleteObject(m_bkBrush);

	SAFERELEASE(m_pVWRenderRoot);

	DeleteObject(m_hbSelectDialog ); 
}


BEGIN_MESSAGE_MAP(CToolDialog, CDialog)
	//{{AFX_MSG_MAP(CToolDialog)
	ON_CBN_SELCHANGE(IDC_DB_SELECTED, OnSelchangeSelected)
	ON_WM_HSCROLL()
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
	ON_EN_KILLFOCUS(IDC_DB_Y, OnKillfocusPosition)
	ON_EN_KILLFOCUS(IDC_DB_X, OnKillfocusPosition)
	ON_EN_KILLFOCUS(IDC_DB_Z, OnKillfocusPosition)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolDialog message handlers

BOOL CToolDialog::CheckButton(int nID, BOOL bCheck)
{
	if (nID == ID_TOOLS_COLLISIONTOGGLE || nID == ID_TOOLS_GRAVITYTOGGLE)
	{

		switch (nID)
		{
		case ID_TOOLS_COLLISIONTOGGLE:
			m_bCollision = bCheck;
			break;
		case ID_TOOLS_GRAVITYTOGGLE:
			m_bGravity = bCheck;
			break;
		}
	}
	else
	{
		m_bbNavigate.SetCheck(0);
		m_bbNavigate.SetState(0);
		m_bbSelect.SetCheck(0);
		m_bbSelect.SetState(0);
		m_bbTranslate.SetCheck(0);
		m_bbTranslate.SetState(0);
		m_bbRotate.SetCheck(0);
		m_bbRotate.SetState(0);
		m_bbScale.SetCheck(0);
		m_bbScale.SetState(0);
		m_bbZoomIn.SetCheck(0);
		m_bbZoomIn.SetState(0);
		m_bbZoomOut.SetCheck(0);
		m_bbZoomOut.SetState(0);

		CheckRadioButton(ID_TOOLS_SELECT, ID_TOOLS_SCALE, nID);

		switch (nID)
		{
		case ID_TOOLS_NAVIGATE:
			m_bbNavigate.SetCheck(1);
			m_bbNavigate.SetState(1);
			break;
		case ID_TOOLS_SELECT:
			m_bbSelect.SetCheck(1);
			m_bbSelect.SetState(1);
			break;
		case ID_TOOLS_TRANSLATE:
			m_bbTranslate.SetCheck(1);
			m_bbTranslate.SetState(1);
			break;
		case ID_TOOLS_ROTATE:
			m_bbRotate.SetCheck(1);
			m_bbRotate.SetState(1);
			break;
		case ID_TOOLS_SCALE:
			m_bbScale.SetCheck(1);
			m_bbScale.SetState(1);
			break;
		case ID_TOOLS_ZOOMIN:
			m_bbZoomIn.SetCheck(1);
			m_bbZoomIn.SetState(1);
			break;
		case ID_TOOLS_ZOOMOUT:
			m_bbZoomOut.SetCheck(1);
			m_bbZoomOut.SetState(1);
			break;
		}
	}
	
	return TRUE;
}



BOOL CToolDialog::InitDialogBar()
{
	CWnd* wndTmp;

	ShowWindow(TRUE);

	wndTmp = GetDlgItem(ID_TOOLS_NAVIGATE);
	SetWindowLong(wndTmp->m_hWnd, GWL_STYLE, BS_OWNERDRAW | wndTmp->GetStyle() );
	wndTmp = GetDlgItem(ID_TOOLS_SELECT);
	SetWindowLong(wndTmp->m_hWnd, GWL_STYLE, BS_OWNERDRAW | wndTmp->GetStyle() );
	wndTmp = GetDlgItem(ID_TOOLS_TRANSLATE);
	SetWindowLong(wndTmp->m_hWnd, GWL_STYLE, BS_OWNERDRAW | wndTmp->GetStyle() );
	wndTmp = GetDlgItem(ID_TOOLS_ROTATE);
	SetWindowLong(wndTmp->m_hWnd, GWL_STYLE, BS_OWNERDRAW | wndTmp->GetStyle() );
	wndTmp = GetDlgItem(ID_TOOLS_SCALE);
	SetWindowLong(wndTmp->m_hWnd, GWL_STYLE, BS_OWNERDRAW | wndTmp->GetStyle() );
	wndTmp = GetDlgItem(ID_TOOLS_COLLISIONTOGGLE);
	SetWindowLong(wndTmp->m_hWnd, GWL_STYLE, BS_OWNERDRAW | GetWindowLong(wndTmp->m_hWnd, GWL_STYLE) );
	wndTmp = GetDlgItem(ID_TOOLS_GRAVITYTOGGLE);
	SetWindowLong(wndTmp->m_hWnd, GWL_STYLE, BS_OWNERDRAW | GetWindowLong(wndTmp->m_hWnd, GWL_STYLE) );
	wndTmp = GetDlgItem(ID_TOOLS_ZOOMIN);
	SetWindowLong(wndTmp->m_hWnd, GWL_STYLE, BS_OWNERDRAW | wndTmp->GetStyle() );
	wndTmp = GetDlgItem(ID_TOOLS_ZOOMOUT);
	SetWindowLong(wndTmp->m_hWnd, GWL_STYLE, BS_OWNERDRAW | wndTmp->GetStyle() );

	m_bbUndo.AutoLoad( ID_EDIT_UNDO, this );
	m_bbRedo.AutoLoad( ID_EDIT_REDO, this );
	m_bbSelectContents.AutoLoad(ID_TOOLS_SELECTCONTENTS, this );
	m_bbHelp.AutoLoad(ID_TOOLS_HELP, this );
	m_bbNavigate.AutoLoad( ID_TOOLS_NAVIGATE, this );
	m_bbSelect.AutoLoad(ID_TOOLS_SELECT, this );
	m_bbTranslate.AutoLoad(ID_TOOLS_TRANSLATE, this );
	m_bbRotate.AutoLoad(ID_TOOLS_ROTATE, this );
	m_bbScale.AutoLoad(ID_TOOLS_SCALE, this );
	m_bbZoomIn.AutoLoad(ID_TOOLS_ZOOMIN, this );
	m_bbZoomOut.AutoLoad(ID_TOOLS_ZOOMOUT, this );
//	m_bbGravityToggle.AutoLoad(ID_TOOLS_GRAVITYTOGGLE, this );
//	m_bbCollisionToggle.AutoLoad(ID_TOOLS_COLLISIONTOGGLE, this );

	m_bmCollisionUp.LoadBitmap("COLLISIONTOGGLEU");
	m_bmCollisionDown.LoadBitmap("COLLISIONTOGGLED");
	m_bmGravityUp.LoadBitmap("GRAVITYTOGGLEU");
	m_bmGravityDown.LoadBitmap("GRAVITYTOGGLED");


	EnableToolTips(TRUE);	

	if (m_ToolTip.Create(this, TTS_ALWAYSTIP ) )
	{
		AddWindowTool(ID_TOOLS_COLLISIONTOGGLE);
		AddWindowTool(ID_TOOLS_GRAVITYTOGGLE);
		AddWindowTool(ID_TOOLS_SELECTCONTENTS);
		AddWindowTool(ID_TOOLS_NAVIGATE);
		AddWindowTool(ID_EDIT_REDO);
		AddWindowTool(ID_TOOLS_ROTATE);
		AddWindowTool(ID_TOOLS_SCALE);
		AddWindowTool(ID_TOOLS_SELECT);
		AddWindowTool(ID_TOOLS_TRANSLATE);
		AddWindowTool(ID_EDIT_UNDO);
		AddWindowTool(ID_TOOLS_HELP);
		AddWindowTool(ID_TOOLS_ZOOMIN);
		AddWindowTool(ID_TOOLS_ZOOMOUT);

		AddWindowTool(IDC_DB_SELECTED);
		AddWindowTool(IDC_DB_X);
		AddWindowTool(IDC_DB_Y);
		AddWindowTool(IDC_DB_Z);

		m_ToolTip.SendMessage(TTM_SETDELAYTIME, TTDT_AUTOPOP, SHRT_MAX);
		m_ToolTip.SendMessage(TTM_SETDELAYTIME, TTDT_INITIAL, 500);
		m_ToolTip.SendMessage(TTM_SETDELAYTIME, TTDT_RESHOW, 200);
	}
	else 
	{
		TRACE("ERROR in creating ToolTip");
	}

	m_ToolTip.Activate(TRUE);
	
	//Review: For now assume navigate tool is the active tool on starting edit mode.
	CheckButton(ID_TOOLS_NAVIGATE, TRUE);

	OnSelchangeSelected();

	return TRUE;
}

void CToolDialog::EnableButton(UINT nControlID, BOOL bEnable)
{
	::EnableWindow(::GetDlgItem(m_hWnd, nControlID), bEnable);
}

BOOL CToolDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitDialogBar();
	
	bDialogInitialized = TRUE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CToolDialog::AddWindowTool(UINT nControlID)
{
	TOOLINFO ti;
	CString tmpstr;
	tmpstr.LoadString(nControlID);

	ti.cbSize = sizeof TOOLINFO;
	ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
	ti.hwnd = GetDlgItem(nControlID)->GetParent()->GetSafeHwnd();
	ti.uId = (UINT) GetDlgItem(nControlID)->GetSafeHwnd();
	ti.hinst = AfxGetInstanceHandle();
	ti.lpszText = (char*) (LPCTSTR) tmpstr;

	return (BOOL) m_ToolTip.SendMessage(TTM_ADDTOOL, 0, (LPARAM) &ti);

}

BOOL CToolDialog::OnUpdateUserInterfaceGeometryInfo(IVWControlManager* m_pControlManager, VARIANT varHint)
{
	IThing* pThing = NULL;
	int nCurSel;
	HRESULT hr = S_OK;

	
	nCurSel = SendDlgItemMessage(IDC_DB_SELECTED, CB_GETCURSEL, 0, 0);
	pThing = (IThing*) SendDlgItemMessage(IDC_DB_SELECTED, CB_GETITEMDATA, nCurSel, 0);

	if (varHint.vt == VT_DISPATCH && NULL != pThing && pThing == varHint.pdispVal)
	{
		COleVariant	var; // get the geometry property
		CVWFramePtr TransPtr;

		hr = pThing->InvokeMethodExt(CComBSTR("GetFrame"), NULL, &var);
		if (FAILED(hr)) goto ERROR_DONE;

		TransPtr = var;

		if (TransPtr)
		{
			CString szTmp;
			float fX, fY, fZ;

			IVWFrame *pvwfParent = NULL;

			hr = TransPtr->GetParent(&pvwfParent);
			if (!pvwfParent)
			{
				hr = E_FAIL;
				goto ERROR_DONE;
			}

			hr = TransPtr->GetPositionEx(pvwfParent, &fX, &fY, &fZ);
//			hr = TransPtr->GetPosition(&fX, &fY, &fZ);

			SAFERELEASE(pvwfParent);

			if (FAILED(hr)) //goto ERROR_DONE;
			{
				fX = fY = fZ = 0.0F;
				hr = S_OK;
			}

			szTmp.Format("%.2f", fX);
			SendDlgItemMessage(IDC_DB_X, WM_SETTEXT, 0, (LPARAM) (LPCTSTR) szTmp);
			szTmp.Format("%.2f", fY);
			SendDlgItemMessage(IDC_DB_Y, WM_SETTEXT, 0, (LPARAM) (LPCTSTR) szTmp);
			szTmp.Format("%.2f", fZ);
			SendDlgItemMessage(IDC_DB_Z, WM_SETTEXT, 0, (LPARAM) (LPCTSTR) szTmp);
		}
	}
ERROR_DONE:

	return TRUE;
}

BOOL CToolDialog::OnUpdateUserInterface(IVWControlManager* pCtrlManager, IVWRenderRoot* pVWRenderRoot, BOOL bCollisionToggle, BOOL bGravityToggle)
{
	long nCount;
	int nThingSelected;
	IObjectProperty * pProperty = NULL;
	IPropertyList* pSelectionList = NULL;
	IVWUndoStacks *pUndoStacks = NULL;
	BSTR undoLabel = NULL;
	HRESULT hr = S_OK;
	CString szTmp;
	BSTR bstrReturnValue = NULL;
	VARIANT_BOOL	fLastItem = VARIANT_TRUE;
	COleVariant varProperty;
	CThingPtr thingPtr;
	CComBSTR bstrName;
	int nAdded;
	long nSelNum;

	if (!bDialogInitialized)
		return FALSE;

	::VariantInit( &varProperty);
	
	EnableButton(ID_EDIT_UNDO, FALSE);
	EnableButton(ID_EDIT_REDO, FALSE);

	SendDlgItemMessage(IDC_DB_SELECTED, CB_RESETCONTENT , 0, 0);
	SendDlgItemMessage(IDC_DB_X, WM_SETTEXT, 0, (LPARAM) (LPCTSTR) "");
	SendDlgItemMessage(IDC_DB_Y, WM_SETTEXT, 0, (LPARAM) (LPCTSTR) "");
	SendDlgItemMessage(IDC_DB_Z, WM_SETTEXT, 0, (LPARAM) (LPCTSTR) "");

	CheckButton(ID_TOOLS_COLLISIONTOGGLE, bCollisionToggle);
	CheckButton(ID_TOOLS_GRAVITYTOGGLE, bGravityToggle);

	if(pCtrlManager)
	{
		hr = pCtrlManager->get_UndoStack(&pUndoStacks);
		if (FAILED(hr) || !pUndoStacks) goto ERROR_DONE;

		hr = pUndoStacks->get_UndoLabel(&undoLabel);
		if (FAILED(hr) || undoLabel == NULL) 
		{
			hr = S_OK;	
		}
		else
		{
			EnableButton(ID_EDIT_UNDO, TRUE);
		}

		SAFEFREESTRING(undoLabel);

		hr = pUndoStacks->get_RedoLabel(&undoLabel);
		if (FAILED(hr) || undoLabel == NULL) 
		{
			hr = S_OK;	
		}
		else
		{
			EnableButton(ID_EDIT_REDO, TRUE);
		}

		hr = pCtrlManager->get_SelectionList(&pSelectionList);	
		if( FAILED(hr)) goto ERROR_DONE;

		if (FAILED(hr = pSelectionList->get_Count(&nCount)))
		{
			OutputDebugString("CBoundaryViewCtrl: Error getting count.\n");
			goto ERROR_DONE;
		}

		nThingSelected = 0;

		//Poke the name of the selected object/boundary in the text box
		if (pSelectionList && nCount > 0)
		{
			nSelNum = 0;

			hr = pSelectionList->FirstItem( &bstrReturnValue, &varProperty, &fLastItem );
			if( FAILED(hr) ) goto ERROR_DONE;

			//This is the main loop that goes through the selection list.
			//It added selected things and boundaries to the dropdown box
			while( VARIANT_FALSE == fLastItem ) 
			{
				thingPtr = varProperty;
				if( thingPtr != NULL )  //Only a thing selected
				{
					hr = thingPtr->get_Name(&bstrName.m_str);
					if( FAILED(hr) ) goto ERROR_DONE;

					szTmp.Format("%s",CString(bstrName));//, (nCount > 1) ? "..." : "");
					nAdded = SendDlgItemMessage(IDC_DB_SELECTED, CB_ADDSTRING, 0, (LPARAM) (LPCTSTR) szTmp);
					SendDlgItemMessage(IDC_DB_SELECTED, CB_SETITEMDATA, nAdded, (long) thingPtr.p);
					nSelNum++;
				}

				SAFEFREESTRING(bstrReturnValue);
				varProperty.Clear();

				hr = pSelectionList->NextItem( &bstrReturnValue, &varProperty, &fLastItem );
    			if( FAILED(hr) ) goto ERROR_DONE;
			}

			//Now based on what's in the box, make some cosmetic modifications
			if (nSelNum)
			{
				SendDlgItemMessage(IDC_DB_SELECTED, CB_SETCURSEL, 0, 0);
				OnSelchangeSelected();
			}
		}

		OnUpdateUserInterfaceZoomLevel(pVWRenderRoot);
	}

ERROR_DONE:
	SAFERELEASE(pSelectionList);
	SAFERELEASE(pProperty);
	SAFEFREESTRING(undoLabel);
	SAFERELEASE(pUndoStacks);
	SAFEFREESTRING(bstrReturnValue);
	varProperty.Clear();

	return TRUE;
}



void CToolDialog::OnSelchangeSelected() 
{
	int nSelNum = SendDlgItemMessage(IDC_DB_SELECTED, CB_GETCURSEL, 0, 0);
	IVector			*pvPos = NULL;
	HRESULT hr;
	BOOL bModify = TRUE;
	float			fPosx, fPosy, fPosz;
	IThing* pContainer = NULL;
	CString szTmp;
	float fX, fY, fZ;

	SendDlgItemMessage(IDC_DB_X, WM_SETTEXT, 0, (LPARAM) (LPCTSTR) "");
	SendDlgItemMessage(IDC_DB_Y, WM_SETTEXT, 0, (LPARAM) (LPCTSTR) "");
	SendDlgItemMessage(IDC_DB_Z, WM_SETTEXT, 0, (LPARAM) (LPCTSTR) "");

	if (nSelNum > -1)
	{
		COleVariant	var; // get the geometry property
//		CVWTransformPtr TransPtr;
		CVWFramePtr TransPtr;
		IVWFrame *pvwfParent = NULL;

		IThing* pThing = (IThing*) SendDlgItemMessage(IDC_DB_SELECTED, CB_GETITEMDATA, nSelNum, 0);

		if (pThing)
		{
			hr = pThing->InvokeMethodExt(CComBSTR("GetFrame"), NULL, &var);
			if (FAILED(hr)) goto ERROR_DONE;

			TransPtr = var;

			if (TransPtr)
			{

				hr = TransPtr->GetParent(&pvwfParent);
				if (!pvwfParent)
				{
					hr = E_FAIL;
					goto ERROR_DONE;
				}
//				hr = TransPtr->GetPosition(&fX, &fY, &fZ);
				hr = TransPtr->GetPositionEx(pvwfParent, &fX, &fY, &fZ);

				SAFERELEASE(pvwfParent);

				if (FAILED(hr))
				{
					bModify = FALSE;
					hr = S_OK;
				}
				if (FAILED(hr)) goto ERROR_DONE;

				hr = pThing->get_Container(&pContainer);
				if (FAILED(hr) || !pContainer)
				{
					bModify = FALSE;
					hr = S_OK;
				}
				if (FAILED(hr)) goto ERROR_DONE;

				hr = pThing->get_ObjectProperty(bstrPosition, (IObjectProperty**)&pvPos);
				if (FAILED(hr)) goto ERROR_DONE;

				hr = pvPos->get(&fPosx, &fPosy, &fPosz);
				if (FAILED(hr)) goto ERROR_DONE;

				hr = pvPos->set(fPosx, fPosy, fPosz);
				if (VWOBJECT_E_PERMISSION_DENIED == hr)
				{
					bModify = FALSE;
					hr = S_OK;
				}
				if (FAILED(hr)) goto ERROR_DONE;
			}
		}
		else
		{
			bModify = FALSE;
		}
	}
	else
	{
		bModify = FALSE;
	}

	if (!bModify) //goto ERROR_DONE;
	{
		fX = fY = fZ = 0.0F;
		hr = S_OK;

		SendDlgItemMessage(IDC_DB_X, WM_ENABLE, FALSE, 0);
		SendDlgItemMessage(IDC_DB_Y, WM_ENABLE, FALSE, 0);
		SendDlgItemMessage(IDC_DB_Z, WM_ENABLE, FALSE, 0);

		szTmp = "";
		SendDlgItemMessage(IDC_DB_X, WM_SETTEXT, 0, (LPARAM) (LPCTSTR) "");
		SendDlgItemMessage(IDC_DB_Y, WM_SETTEXT, 0, (LPARAM) (LPCTSTR) "");
		SendDlgItemMessage(IDC_DB_Z, WM_SETTEXT, 0, (LPARAM) (LPCTSTR) "");
	}
	else
	{
		SendDlgItemMessage(IDC_DB_X, WM_ENABLE, TRUE, 0);
		SendDlgItemMessage(IDC_DB_Y, WM_ENABLE, TRUE, 0);
		SendDlgItemMessage(IDC_DB_Z, WM_ENABLE, TRUE, 0);

		szTmp.Format("%.2f", fX);
		SendDlgItemMessage(IDC_DB_X, WM_SETTEXT, 0, (LPARAM) (LPCTSTR) szTmp);
		szTmp.Format("%.2f", fY);
		SendDlgItemMessage(IDC_DB_Y, WM_SETTEXT, 0, (LPARAM) (LPCTSTR) szTmp);
		szTmp.Format("%.2f", fZ);
		SendDlgItemMessage(IDC_DB_Z, WM_SETTEXT, 0, (LPARAM) (LPCTSTR) szTmp);

	}


ERROR_DONE:
	SAFERELEASE(pContainer);
	SAFERELEASE(pvPos);


	return;
}


void CToolDialog::OnKillfocusPosition() 
{
	HRESULT hr = S_OK;
	IThing* pthing = NULL;
	int nSelected;
	LRESULT lrItemData;
	CString szTmp;
	IVector			*pvPos = NULL;

	nSelected = SendDlgItemMessage(IDC_DB_SELECTED, CB_GETCURSEL, 0, 0);

	lrItemData = SendDlgItemMessage(IDC_DB_SELECTED, CB_GETITEMDATA, nSelected, 0);

	if (lrItemData > 0)
	{
		COleVariant	var; // get the geometry property
//		CVWTransformPtr TransPtr;
		CVWFramePtr TransPtr;
		IVWFrame *pvwfParent = NULL;

		pthing = (IThing*) lrItemData;

		hr = pthing->InvokeMethodExt(CComBSTR("GetFrame"), NULL, &var);
		if (FAILED(hr)) goto EXIT_FAIL;

		TransPtr = var;

		if (TransPtr)
		{
			CString szTmp;
			float fX, fY, fZ, fTmpX, fTmpY, fTmpZ;

			hr = TransPtr->GetParent(&pvwfParent);
	
			if (!pvwfParent)
			{
				hr = E_FAIL;
				goto EXIT_FAIL;
			}
			hr = TransPtr->GetPositionEx(pvwfParent, &fX, &fY, &fZ);
		//	hr = TransPtr->GetPosition(&fX, &fY, &fZ);

			SAFERELEASE(pvwfParent);
 
			if (FAILED(hr)) 
			{
				szTmp.Format("0.00");
				SendDlgItemMessage(IDC_DB_X, WM_SETTEXT, 0, (LPARAM) (LPCTSTR) szTmp);
				SendDlgItemMessage(IDC_DB_Y, WM_SETTEXT, 0, (LPARAM) (LPCTSTR) szTmp);
				SendDlgItemMessage(IDC_DB_Z, WM_SETTEXT, 0, (LPARAM) (LPCTSTR) szTmp);
				goto EXIT_FAIL;
			}

			SendDlgItemMessage(IDC_DB_X, WM_GETTEXT, 256, (LPARAM) (LPCTSTR) szTmp.GetBuffer(256));
			szTmp.ReleaseBuffer();
			fTmpX = (float) atof((LPCTSTR)szTmp);

			SendDlgItemMessage(IDC_DB_Y, WM_GETTEXT, 256, (LPARAM) (LPCTSTR) szTmp.GetBuffer(256));
			szTmp.ReleaseBuffer();
			fTmpY = (float) atof((LPCTSTR)szTmp);

			SendDlgItemMessage(IDC_DB_Z, WM_GETTEXT, 256, (LPARAM) (LPCTSTR) szTmp.GetBuffer(256));
			szTmp.ReleaseBuffer();
			fTmpZ = (float) atof((LPCTSTR)szTmp);


			if (fTmpX != fX || fTmpY != fY || fTmpZ != fZ)
			{
				hr = pthing->get_ObjectProperty(bstrPosition, (IObjectProperty**)&pvPos);
				if (FAILED(hr)) goto EXIT_FAIL;

				hr = pvPos->set(fTmpX, fTmpY, fTmpZ);
		
				if (VWOBJECT_E_PERMISSION_DENIED != hr)
					//hr = TransPtr->SetPosition(fTmpX, fTmpY, fTmpZ);
					hr = TransPtr->SetPositionEx(pvwfParent, fTmpX, fTmpY, fTmpZ);
				else
					hr = S_OK;

				if( FAILED(hr) ) goto EXIT_FAIL;
			}

//			hr = TransPtr->GetPosition(&fTmpX, &fTmpY, &fTmpZ);
			hr = TransPtr->GetPositionEx(pvwfParent, &fTmpX, &fTmpY, &fTmpZ);

			SAFERELEASE(pvwfParent);

			if (FAILED(hr)) goto EXIT_FAIL;

			szTmp.Format("%.2f", fTmpX);
			SendDlgItemMessage(IDC_DB_X, WM_SETTEXT, 0, (LPARAM) (LPCTSTR) szTmp);

			szTmp.Format("%.2f", fTmpY);
			SendDlgItemMessage(IDC_DB_Y, WM_SETTEXT, 0, (LPARAM) (LPCTSTR) szTmp);

			szTmp.Format("%.2f", fTmpZ);
			SendDlgItemMessage(IDC_DB_Z, WM_SETTEXT, 0, (LPARAM) (LPCTSTR) szTmp);
		}
	}
EXIT_FAIL:

	SAFERELEASE(pvPos);
	return;
}

void CToolDialog::EnableZoom(BOOL bEnable)
{
/*	CWnd* wndTmp;
	
	wndTmp = GetDlgItem(IDC_SLIDER_ZOOMLEVEL);
	if (wndTmp)
		wndTmp->ShowWindow( bEnable ? SW_SHOW : SW_HIDE);

	wndTmp = GetDlgItem(IDC_STATIC_ZOOMLEVEL);
	if (wndTmp)
		wndTmp->ShowWindow( bEnable ? SW_SHOW : SW_HIDE);
*/
}


void CToolDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	IVWRender3D*			pr3D = NULL;
	IDirect3DRMViewport*	pViewport = NULL;
	HRESULT					hr = S_OK;
	int						nSliderPos;

	if (TB_ENDTRACK == nSBCode && NULL != m_pVWRenderRoot)
	{
		CSliderCtrl* wndSlider = (CSliderCtrl*) pScrollBar;

		nSliderPos = wndSlider->GetPos();
		if (nSliderPos <= 0)
			nSliderPos = 5;

		// Get the VWRender3D
		hr = m_pVWRenderRoot->get_VWRender3D(&pr3D);
		if(FAILED(hr) || !pr3D) goto EXIT_FAIL;

		// Get the Viewport
		hr = pr3D->get_Viewport(&pViewport);
		if(FAILED(hr) || !pViewport) goto EXIT_FAIL;

		hr = pViewport->SetField(nSliderPos / 10.0F);
		if(FAILED(hr) ) goto EXIT_FAIL;
	}
	
EXIT_FAIL:

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

	SAFERELEASE(pr3D);
	SAFERELEASE(pViewport);

}

BOOL CToolDialog::OnUpdateUserInterfaceZoomLevel(IVWRenderRoot* pVWRenderRoot)
{
	IVWRender3D*			pr3D = NULL;
	IDirect3DRMViewport*	pViewport = NULL;
	HRESULT					hr = S_OK;
	float					fValue;
	int						nValue;
	CWnd*					wndTmp;

	SAFERELEASE(m_pVWRenderRoot);
	m_pVWRenderRoot = pVWRenderRoot;
	SAFEADDREF(m_pVWRenderRoot);

	wndTmp = GetDlgItem(IDC_SLIDER_ZOOMLEVEL);

	if (wndTmp)
	{
		// Get the VWRender3D
		hr = pVWRenderRoot->get_VWRender3D(&pr3D);
		if(FAILED(hr) || !pr3D) goto EXIT_FAIL;

		// Get the Viewport
		hr = pr3D->get_Viewport(&pViewport);
		if(FAILED(hr) || !pViewport) goto EXIT_FAIL;

		fValue = pViewport->GetField();
		nValue = (int)(fValue * 10.0F);

		((CSliderCtrl*) wndTmp)->SetPos( nValue );
	}

EXIT_FAIL:
	SAFERELEASE(pr3D);
	SAFERELEASE(pViewport);

	return TRUE;
}

LRESULT CToolDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	HDC hdc;
	RECT rc;
	HWND hwnd;

	switch (message)
	{
	case WM_CTLCOLORSTATIC:
		SetBkColor((HDC)wParam, RGB( 194,195,181 )); // 194,195,181 
		return (LRESULT) m_bkBrush;
		break;
	case WM_KEYDOWN:
		if (wParam == VK_RETURN)
		{
			hwnd = ::GetFocus();
			if (hwnd == ::GetDlgItem(m_hWnd, IDC_DB_X) || 
				hwnd == ::GetDlgItem(m_hWnd, IDC_DB_Y) || 
				hwnd == ::GetDlgItem(m_hWnd, IDC_DB_Z))
				OnKillfocusPosition();
		}

		break;
	case WM_ERASEBKGND:
		hdc = (HDC) wParam; 

		GetClientRect(&rc);

		::FillRect(hdc, &rc, m_bkBrush);

		return 1;

		break;
	}
	
	return CDialog::WindowProc(message, wParam, lParam);
}

HBRUSH CToolDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void CToolDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if (nIDCtl == ID_TOOLS_COLLISIONTOGGLE || nIDCtl == ID_TOOLS_GRAVITYTOGGLE )
	{
		CBitmap* pBitmap;

		switch(nIDCtl)
		{
		case ID_TOOLS_COLLISIONTOGGLE:
			if (m_bCollision)
				pBitmap = &m_bmCollisionDown;
			else
				pBitmap = &m_bmCollisionUp;
			break;
		case ID_TOOLS_GRAVITYTOGGLE:
			if (m_bGravity)
				pBitmap = &m_bmGravityDown;
			else
				pBitmap = &m_bmGravityUp;
			break;
		}

		// draw the whole button
		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		CDC memDC;

		memDC.CreateCompatibleDC(pDC);
		CBitmap* pOld = memDC.SelectObject(pBitmap);
		if (pOld == NULL)
			return;     // destructors will clean up

		CRect rect;
		rect.CopyRect(&lpDrawItemStruct->rcItem);
		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOld);
	}
	else
		CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

