// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// SelctDlg.cpp : implementation file
//

#include "stdafx.h"
//#include <vwrendvw.h>
#include "SelctDlg.h"
#include <vwsutils.h>
#include <vwprop.h>
#include <uitlimpl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
typedef CVWComPtr<IObjectProperty,NULL,&IID_IObjectProperty> CObjectPropertyPtr;
typedef CVWComPtr<IThing,NULL,&IID_IThing> CThingPtr;
/////////////////////////////////////////////////////////////////////////////
// CSelectionDialog dialog


CSelectionDialog::CSelectionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectionDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectionDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bShowThings = TRUE;
	m_pSelectedList = NULL;
	m_pControlManager = NULL;
}

CSelectionDialog::~CSelectionDialog()
{
	SAFERELEASE(m_pSelectedList);
	SAFERELEASE(m_pControlManager);
}

void CSelectionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectionDialog)
	DDX_Control(pDX, IDC_LIST, m_listControl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectionDialog, CDialog)
	//{{AFX_MSG_MAP(CSelectionDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectionDialog message handlers

void CSelectionDialog::ShowThings(BOOL bThings)
{
	m_bShowThings = bThings;
	Refresh();

}

BOOL CSelectionDialog::Refresh()
{
	HRESULT hr = S_OK;
	long nCount, nAdded;
	CThingPtr roomPtr;
	
	nAdded = 0;
	if (NULL != m_pSelectedList)
	{
		m_listControl.DeleteAllItems();

		if( FAILED(hr)) goto ERROR_DONE;
		hr = m_pSelectedList->get_Count(&nCount);

		for(int i = 0; i < nCount; i++)
		{
			CObjectPropertyPtr objPropPtr;

			hr = m_pSelectedList->get_ObjectProperty(i, &objPropPtr.p);
			if( FAILED(hr)) goto ERROR_DONE;

			if (objPropPtr.p != NULL)
			{
				CThingPtr thingPtr;
				CThingPtr containterPtr;

				if (m_bShowThings)
				{
					thingPtr = objPropPtr;
					if (thingPtr.p != NULL)
					{
						hr = thingPtr->get_Container(&containterPtr.p);
						if( FAILED(hr)) goto ERROR_DONE;

						if (NULL != containterPtr.p)
						{
							AddThingToList(thingPtr.p);
							nAdded++;
						}
						else
						{
							roomPtr = thingPtr;
						}
					}
										//If the item is a VWFrame, we should select it's thing.
				}
			}
		}

		if (nAdded > 0)
			m_listControl.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED );
		else if (NULL != roomPtr.p)
		{
			AddThingToList(roomPtr.p);
			nAdded++;
			m_listControl.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED );
		}

		if (nAdded > 1)
			return TRUE;
	}

ERROR_DONE:
	return FALSE;
}

HRESULT CSelectionDialog::Initialize(IVWControlManager * pControlManager, IPropertyList* pSelectedList)
{
	SAFERELEASE(m_pControlManager);
	m_pControlManager = pControlManager;
	SAFEADDREF(m_pControlManager);

	SAFERELEASE(m_pSelectedList);
	m_pSelectedList = pSelectedList;
	SAFEADDREF(m_pSelectedList);

	if (::IsWindow(m_hWnd))
		Refresh();

	return S_OK;
}


BOOL CSelectionDialog::AddThingToList(IThing * pThing)
{
	int nAdded;
	CComBSTR bstrName;
	CString strName;

	pThing->get_Name(&bstrName.m_str);
	strName = bstrName;
	nAdded = m_listControl.InsertItem( m_listControl.GetItemCount(), strName );
	
	m_listControl.SetItemData(nAdded, (DWORD) pThing);

	return TRUE;
}

void CSelectionDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CSelectionDialog::OnOK() 
{
	int nItem = -1;
	IObjectProperty* pobjprop;
	HRESULT hr = S_OK;

	hr = m_pControlManager->ClearSelections();

	do 
	{
		nItem = m_listControl.GetNextItem(nItem, LVNI_SELECTED);

		if (nItem > -1)
		{
			pobjprop = (IObjectProperty*) m_listControl.GetItemData(nItem);
			hr = m_pControlManager->AddSelection(CComVariant(pobjprop));
		}

	} while (nItem != -1);
	
	CDialog::OnOK();
}

BOOL CSelectionDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (Refresh() != TRUE)
		OnOK();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectionDialog::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnOK();

	*pResult = 1;
}
