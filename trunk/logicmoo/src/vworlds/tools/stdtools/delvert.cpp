// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// DelVert.cpp : Implementation of CDelVert
#include "stdafx.h"
#include <stdtools.h>
#include "DelVert.h"

#include "GeomUndo.h"
#include "UndoUtil.h"
#include <pickdata.h>

/////////////////////////////////////////////////////////////////////////////
typedef CVWComPtr<IThing,NULL,&IID_IThing> CThingPtr;
typedef CVWComPtr<IVWExecuteUndo, &CLSID_VWExecuteGeomUndo, &IID_IVWExecuteUndo> CExecUndoPtr;
typedef CVWComPtr<IBoundary,NULL,&IID_IBoundary> CBoundaryPtr;
typedef CVWComPtr<IVWPickData,NULL,&IID_IVWPickData> CVWPickDataPtr;
typedef CVWComPtr<IPropertyList,NULL,&IID_IPropertyList> CBoundaryListPtr;
/////////////////////////////////////////////////////////////////////////////
// CDelVert

CDelVert::CDelVert()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HRESULT hr;
	CExecUndoPtr pExecUndo;

	hr = pExecUndo.CoCreateInstance();
	if (FAILED(hr)) goto EXIT_FAILED;

	m_pExecuteUndo = (IVWExecuteUndo *) pExecUndo;
	m_pExecuteUndo->AddRef();
	
	SetCursor(m_pUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_DELVERT)));

EXIT_FAILED:
	return;
}

BOOL CDelVert::OnLButtonDown( UINT nFlags, POINT pt )
{
	HRESULT hr = S_OK;
	VARIANT_BOOL fValid = VARIANT_FALSE;
	BOOL bHandled = FALSE;
	IPropertyList * pPL = NULL;
	IObjectProperty* pOP = NULL;
	IPropertyList* pBoundaryList = NULL;
//	IPropertyList* pEdgeList = NULL;
	IBoundary* pBoundary = NULL;
	long lCount;
	int nCount, i;
	CThingPtr cptrThing;
	CBoundaryPtr cptrEC;
	CVWPickDataPtr cptrPickData;
	BOOL bSuccess = TRUE;

	float flX = (float)pt.x;
	float flY = (float)pt.y;
	float flZ = 0.0;

	if(m_pUIView)
	{ 
		hr = m_pControlManager->get_SelectionList( &pPL );
		if( FAILED(hr ) || NULL == pPL) 
		{
			hr = VWSTUDIO_E_BADCONTROLINITIALIZATION;
			goto EXIT_FAIL;
		}

		hr = fSelectionIsSingleEdgeOrThing(pPL, &pOP);
		if( FAILED(hr) )
		{
 			hr = VWSTUDIO_E_INVALIDSELECTION;
 			goto EXIT_FAIL;
		}

		cptrPickData = pOP;
		if (cptrPickData != NULL)
		{
			cptrPickData->get_Boundary(&cptrEC.p);
			cptrPickData->get_Thing(&cptrThing.p);

			hr = m_2DThingXform.StoreThingCoords(cptrThing);
			if( FAILED(hr) ) goto EXIT_FAIL;

			hr = cptrPickData->get_BoundaryList(&pBoundaryList);
			if (FAILED(hr)) goto EXIT_FAIL;

			if (TryDelVert(cptrEC, flX, flY, flZ, pBoundaryList))
			{
				bSuccess = TRUE;
				//Notify BoundaryList that the boundary has changed
				hr = cptrPickData->get_BoundaryIndex(&nCount);
				if (FAILED(hr)) goto EXIT_FAIL;
				
				hr = pBoundaryList->RemoveAt( nCount );
				if (FAILED(hr)) goto EXIT_FAIL;
				hr = pBoundaryList->InsertAt(nCount, CComVariant( cptrEC.p ) );
				if (FAILED(hr)) goto EXIT_FAIL;

			}
			SAFERELEASE(pBoundaryList);
		}
		else
		{
			cptrThing = pOP;

			hr = m_2DThingXform.StoreThingCoords(cptrThing);
			if( FAILED(hr) ) goto EXIT_FAIL;

			hr = cptrThing->get_ObjectPropertyExt(CComBSTR("BoundaryList"), IID_IPropertyList, (IObjectProperty**)&pBoundaryList);
			if( FAILED(hr) || ( NULL == pBoundaryList )) goto EXIT_FAIL;
			// Create a BoundaryList and add it.

			hr = pBoundaryList->get_Count(&lCount);
			if(FAILED(hr)) goto EXIT_FAIL;

			for (i = 0; i < lCount && !bSuccess; i++)
			{
				hr = pBoundaryList->get_ObjectProperty(i, (IObjectProperty**) &pBoundary);
				if(FAILED(hr)) goto EXIT_FAIL;

				hr = cptrPickData->get_BoundaryList(&pBoundaryList);
				if (FAILED(hr)) goto EXIT_FAIL;

				if (TryDelVert(pBoundary, flX, flY, flZ, pBoundaryList))
				{
					bSuccess = TRUE;
					//Notify BoundaryList that the boundary has changed
					hr = cptrPickData->get_BoundaryIndex(&nCount);
					if (FAILED(hr)) goto EXIT_FAIL;

					hr = pBoundaryList->RemoveAt( nCount );
					if (FAILED(hr)) goto EXIT_FAIL;
					hr = pBoundaryList->InsertAt(nCount, CComVariant( pBoundary ) );
					if (FAILED(hr)) goto EXIT_FAIL;

				}
				SAFERELEASE(pBoundaryList);
				SAFERELEASE(pBoundary);
			}
		}
		SAFERELEASE(pOP);
	}
	else
	{
		hr = VWSTUDIO_E_BADCONTROLINITIALIZATION;
		goto EXIT_FAIL;
	}
	bHandled = TRUE;
	goto EXIT_SUCCEEDED;
EXIT_FAIL:

EXIT_SUCCEEDED:
//	SAFERELEASE(pEdgeList);
	SAFERELEASE(pPL);
	SAFERELEASE(pOP);
	SAFERELEASE(pBoundaryList);
	SAFERELEASE(pBoundary);

	return bHandled;
}

BOOL CDelVert::OnLButtonUp(UINT nFlags, POINT pt )
{
	BOOL bHandled = FALSE;

	return bHandled;
}

STDMETHODIMP CDelVert::IsValid(IVWUIView * pVw, VARIANT_BOOL * pfValid)
{
	HRESULT hr = S_OK;
	*pfValid = FALSE;
	
	ASSERT( pfValid );
	if (!pfValid )
	{
		hr = E_POINTER;
		goto EXIT_FAIL;
	}

EXIT_FAIL:
	return hr;
}

HRESULT CDelVert::fSelectionIsSingleEdgeOrThing(IPropertyList * pPL, IObjectProperty **ppOP)
{
	HRESULT hr = S_OK;
	COleVariant var;
	long lcList;
	int i;
	CThingPtr cptrThing;
	CBoundaryPtr cptrEC;
	CVWPickDataPtr cptrPickData;

 	::VariantInit(&var);

	if(!pPL)
	{
		hr = E_INVALIDARG;
		goto EXIT_FAIL;
	}

 	if(!ppOP)
	{
		hr = E_POINTER;
		goto EXIT_FAIL;
	}

 	if(*ppOP)
	{
		hr = E_INVALIDARG;
		goto EXIT_FAIL;
	}

	hr = pPL->get_Count(&lcList);
	if(FAILED(hr)) goto EXIT_FAIL;

	if( 1 != lcList )
	{
		hr = VWSTUDIO_E_INVALIDSELECTION;
		goto EXIT_FAIL;
	}

	for (i = 0; i < lcList; i++)
	{
		hr = pPL->get_Property(i, &var);
		if(FAILED(hr)) goto EXIT_FAIL;

		cptrPickData = var;
		if ( !(cptrPickData == NULL) )
		{
			*ppOP = cptrPickData;
			VariantClear(&var);
			SAFEADDREF(*ppOP);
			goto EXIT_FAIL;
		}

		cptrThing = var;
		if ( !(cptrThing == NULL) )
		{
			*ppOP = cptrThing;
			VariantClear(&var);
			SAFEADDREF(*ppOP);
			goto EXIT_FAIL;
		}

		cptrEC = var;
		if ( !(cptrEC == NULL) )
		{
			*ppOP = cptrEC;
			VariantClear(&var);
			SAFEADDREF(*ppOP);
			goto EXIT_FAIL;
		}
		VariantClear(&var);

	}

	hr = VWSTUDIO_E_INVALIDSELECTION;

EXIT_FAIL:
	return hr;
}


BOOL CDelVert::TryDelVert(IBoundary* pBoundary, float flX, float flY, float flZ, IPropertyList* pBoundaryList)
{
	VARIANT_BOOL vbSafe;
	EIndex pIndex = NULL_EINDEX;
	HRESULT hr;
	int nCount = 0;
	IVWUndoItem		*pVWUndoItem = NULL;

	ASSERT(pBoundary);

	hr = pBoundary->get_VertexCount(&nCount);
	if(FAILED(hr)) goto EXIT_FAIL;

	if (nCount <= 3)
		goto EXIT_FAIL;
	
	hr = m_pUIView->DeviceToWorld( &flX, &flY, &flZ);
	if(FAILED(hr)) goto EXIT_FAIL;

	hr = m_2DThingXform.TransformToThingCoords( &flX, &flY, &flZ);
	if(FAILED(hr)) goto EXIT_FAIL;

	if(NULL_VINDEX != pIndex)
	{
		hr = pBoundary->GetVertexXYExt(pIndex, &flX, &flZ);
		if (FAILED(hr)) goto EXIT_FAIL;

		hr = pBoundary->DeleteVertexSafe(pIndex, &vbSafe);
		if (FAILED(hr)) goto EXIT_FAIL;

		if (VARIANT_TRUE != vbSafe)
			OutputDebugString("CInsVert::OnLButtonDown - Deleting vertex is unsafe.\n");
		else
		{
			// create the undo item
			pVWUndoItem = CreateUndoItem(m_pExecuteUndo);
			if (pVWUndoItem)
			{
				CString strLabel;
				AFX_MANAGE_STATE(AfxGetStaticModuleState());
				strLabel.LoadString(IDS_UNDO_DELETEVERTEX);
				hr = pVWUndoItem->put_Label(strLabel.AllocSysString());
				if (FAILED(hr)) goto EXIT_FAIL;

				UndoData *pud;
				pud = CreateUndoData(DELETEVERTEX2D);
				pud->extra.deletevertex.ecVert.index = pIndex;
				pud->extra.deletevertex.ecVert.x = flX;
				pud->extra.deletevertex.ecVert.y = flZ;
				pud->extra.deletevertex.BoundaryList = pBoundaryList;

				VARIANT varUndo;
				VariantInit(&varUndo);
				varUndo.vt = VT_BYREF;
				varUndo.byref = (void *) pud;
				hr = pVWUndoItem->put_UndoData(varUndo);
				if (FAILED(hr)) goto EXIT_FAIL;
				
				VARIANT varTarget;
				VariantInit(&varTarget);
				varTarget.vt = VT_DISPATCH;
				pBoundary->AddRef();
				varTarget.pdispVal = pBoundary;
				hr = pVWUndoItem->put_UndoTarget(varTarget);
				if (FAILED(hr)) goto EXIT_FAIL;
				
				// add the undo item to the stack
				if (m_pControlManager)
				{
					pVWUndoItem->AddRef();
					hr = m_pControlManager->AddVWUndoItem(pVWUndoItem);
					if (FAILED(hr)) goto EXIT_FAIL;
				}
			}
		}

		return TRUE;
	}
EXIT_FAIL:
	MessageBeep(MB_ICONASTERISK);
	return FALSE;
}
