// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// GeomUndo.cpp : Implementation of CVWExecuteGeomUndo
#include "stdafx.h"
#include <d3drm.h>
#include <geomtool.h>
#include "GeomUndo.h"

//#include <CellProp_i.c>

/////////////////////////////////////////////////////////////////////////////
// CVWExecuteGeomUndo

STDMETHODIMP CVWExecuteGeomUndo::Undo( IVWUndoItem * pundo )
{
	HRESULT hr = S_OK;
	VARIANT varUndo, varTarget;  //REVIEW:  Variant memory leak here?
	IThing *pThing = NULL;
	IBoundary* pEC = NULL;
	IVector	*pvPos = NULL;
	IVector	*pvUp = NULL;
	IPropertyList* pBoundaryList = NULL;
	float fX, fY, fCenterX, fCenterY;
	static CComBSTR	bstrScale("Scale");
	static CComBSTR	bstrPosition("Position");
	static CComBSTR	bstrDirection("Direction");
	static CComBSTR	bstrUp("Up");
	UndoData *pud;
	VARIANT_BOOL vbSafe;
	int i;

	ZeroMemory(&varTarget, sizeof(VARIANT));
	ZeroMemory(&varUndo, sizeof(VARIANT));

	hr = pundo->get_UndoTarget(&varTarget);
	if (FAILED(hr))	goto FAIL_EXIT;

	hr = pundo->get_UndoData(&varUndo);
	if (FAILED(hr))	goto FAIL_EXIT;

	pud = (UndoData*) varUndo.byref;

	ASSERT(pud);
	switch(pud->m_ot)
	{
	case SCALE:
		pThing = (IThing*) varTarget.pdispVal;
		ASSERT(pThing);

		if (FAILED(hr = pThing->get_ObjectProperty(bstrScale, (IObjectProperty**)&pvPos)))
			goto FAIL_EXIT;
		pvPos->set(pud->extra.scale.oldScale.x, pud->extra.scale.oldScale.y, pud->extra.scale.oldScale.z);
		SAFERELEASE(pvPos);
		break;
	case SCALE2D:
		pEC = (IBoundary*) varTarget.pdispVal;
		ASSERT(pEC);

		fCenterX = pud->extra.scale.oldScale.x;
		fCenterY = pud->extra.scale.oldScale.y;

		i = 0;
		while (TRUE)
		{
			hr = pEC->GetVertexXYExt(i, &fX, &fY);
			if (hr == E_INVALIDARG)
			{
				hr = S_OK;
				break;
			}
			if(FAILED( hr )) goto FAIL_EXIT;

			hr = pEC->SetVertexXY(i, (fX - fCenterX) * (1.0F / pud->extra.scale.newScale.x) + fCenterX, (fY - fCenterY) * (1.0F / pud->extra.scale.newScale.y) + fCenterY);
			if(FAILED( hr )) goto FAIL_EXIT;

			i++;
		}
		break;
	case TRANSLATE:
		pThing = (IThing*) varTarget.pdispVal;
		ASSERT(pThing);

		if (FAILED(hr = pThing->get_ObjectProperty(bstrPosition, (IObjectProperty**)&pvPos)))
			goto FAIL_EXIT;
		pvPos->set(pud->extra.trans.oldPosition.x, pud->extra.trans.oldPosition.y, pud->extra.trans.oldPosition.z);
		SAFERELEASE(pvPos);
		break;
	case TRANSLATE2D:
		pEC = (IBoundary*) varTarget.pdispVal;
		ASSERT(pEC);

		if (pud->extra.trans.nSelectedVert >= 0)  
		{
			hr = pEC->SetVertexXY(	pud->extra.trans.nSelectedVert, 
									pud->extra.trans.oldPosition.x,  // + fX, 
									pud->extra.trans.oldPosition.y); // + fY);
			if (FAILED(hr))	goto FAIL_EXIT;
		}
		else //Move the whole Boundary
		{
			hr = pEC->SetVertexXY(	pud->extra.trans.nSelectedVert, 
									pud->extra.trans.oldPosition.x, 
									pud->extra.trans.oldPosition.y);
		}
		break;
	case ROTATE:
		pThing = (IThing*) varTarget.pdispVal;
		ASSERT(pThing);

		if (FAILED(hr = pThing->get_ObjectProperty(bstrDirection, (IObjectProperty**)&pvPos)))
			goto FAIL_EXIT;
		if (FAILED(hr = pThing->get_ObjectProperty(bstrUp, (IObjectProperty**)&pvUp)))
			goto FAIL_EXIT;
		pvPos->set(pud->extra.rotate.oldDirection.x, pud->extra.rotate.oldDirection.y, pud->extra.rotate.oldDirection.z);
		pvUp->set(pud->extra.rotate.oldUp.x, pud->extra.rotate.oldUp.y, pud->extra.rotate.oldUp.z);
		break;
	case ROTATE2D:
		pEC = (IBoundary*) varTarget.pdispVal;
		ASSERT(pEC );

		hr = pEC->Rotate(pud->extra.rotate.oldDirection.x);
		break;
	case NEWBOUNDARY:
		pEC = (IBoundary*) varTarget.pdispVal;
		ASSERT(pEC);

		pud->extra.newboundary.BoundaryList->Remove(CComVariant(pEC));
		if (FAILED(hr)) goto FAIL_EXIT;

		break;
	case DELETEBOUNDARY:
		pEC = (IBoundary*) varTarget.pdispVal;
		ASSERT(pEC);

		pud->extra.newboundary.BoundaryList->Add( CComVariant(pEC) );
		if (FAILED(hr)) goto FAIL_EXIT;
		break;
	case INSERTVERTEX2D:
		pEC = (IBoundary*) varTarget.pdispVal;
		ASSERT(pEC );

		hr = pEC->DeleteVertexSafe(pud->extra.insertvertex.ecVert.index, &vbSafe);
		if (FAILED(hr)) goto FAIL_EXIT;

		hr = pud->extra.insertvertex.BoundaryList->Remove(CComVariant(pEC));
		if (FAILED(hr)) goto FAIL_EXIT;
		hr = pud->extra.insertvertex.BoundaryList->Add(CComVariant(pEC));
		if (FAILED(hr)) goto FAIL_EXIT;

		break;

	case DELETEVERTEX2D:
		pEC = (IBoundary*) varTarget.pdispVal;
		ASSERT(pEC);

		hr = pEC->InsertVertexSafe(	pud->extra.deletevertex.ecVert.index,
									pud->extra.deletevertex.ecVert.x, 
									pud->extra.deletevertex.ecVert.y, 
									&vbSafe);
		if (FAILED(hr)) goto FAIL_EXIT;

		hr = pud->extra.deletevertex.BoundaryList->Remove(CComVariant(pEC));
		if (FAILED(hr)) goto FAIL_EXIT;
		hr = pud->extra.deletevertex.BoundaryList->Add(CComVariant(pEC));
		if (FAILED(hr)) goto FAIL_EXIT;

		break;
	case FLIPBOUNDARY:
		//pEC = (IBoundary*) varTarget.pdispVal;
		//ASSERT(pECB);
		//pECB->ReverseVertices();
		break;
	default:
		OutputDebugString("CVWExecuteGeomUndo::Undo: Unhandled undo type\n");
	}

FAIL_EXIT:
	SAFERELEASE(pvPos);
	SAFERELEASE(pvUp);
	SAFERELEASE(pEC);
	SAFERELEASE(pBoundaryList);
	return hr;
}

STDMETHODIMP CVWExecuteGeomUndo::Redo( IVWUndoItem * predo )
{
	HRESULT hr = S_OK;
	VARIANT varTarget, varRedo;
	IThing *pThing = NULL;
	IVector	*pvPos = NULL;
	IVector	*pvUp = NULL;
	float fX, fY, fCenterX, fCenterY;
	static CComBSTR	bstrScale("Scale");
	static CComBSTR	bstrPosition("Position");
	static CComBSTR	bstrDirection("Direction");
	static CComBSTR	bstrUp("Up");
	IBoundary *pEC = NULL;
	IPropertyList *pBoundaryList = NULL;
	UndoData *pud;
	VARIANT_BOOL vbSafe;
	int i;

	VariantInit(&varTarget);
	VariantInit(&varRedo);

	hr = predo->get_UndoTarget(&varTarget);
	if (FAILED(hr)) goto FAIL_EXIT;

	hr = predo->get_UndoData(&varRedo);
	if (FAILED(hr))	goto FAIL_EXIT;

	pud = (UndoData*) varRedo.byref;
	ASSERT(pud);
	switch(pud->m_ot)
	{
	case SCALE:
		pThing = (IThing*) varTarget.pdispVal;
		ASSERT(pThing);

		if (FAILED(hr = pThing->get_ObjectProperty(bstrScale, (IObjectProperty**)&pvPos)))
			goto FAIL_EXIT;
		pvPos->set(pud->extra.scale.newScale.x, pud->extra.scale.newScale.y, pud->extra.scale.newScale.z);
		break;
	case SCALE2D:
		pEC = (IBoundary*) varTarget.pdispVal;
		ASSERT(pEC);

		fCenterX = pud->extra.scale.oldScale.x;
		fCenterY = pud->extra.scale.oldScale.y;

		i = 0;
		while (TRUE)
		{
			hr = pEC->GetVertexXYExt(i, &fX, &fY);
			if (hr == E_INVALIDARG)
			{
				hr = S_OK;
				break;
			}
			if(FAILED( hr )) goto FAIL_EXIT;

			hr = pEC->SetVertexXY(i, (fX - fCenterX) * pud->extra.scale.newScale.x + fCenterX, (fY - fCenterY) * pud->extra.scale.newScale.y + fCenterY);
			if(FAILED( hr )) goto FAIL_EXIT;
			i++;
		}

		break;
	case TRANSLATE:
		pThing = (IThing*) varTarget.pdispVal;
		ASSERT(pThing);

		if (FAILED(hr = pThing->get_ObjectProperty(bstrPosition, (IObjectProperty**)&pvPos)))
			goto FAIL_EXIT;
		pvPos->set(pud->extra.trans.newPosition.x, pud->extra.trans.newPosition.y, pud->extra.trans.newPosition.z);
		break;
	case TRANSLATE2D:
		pEC = (IBoundary*) varTarget.pdispVal;
		ASSERT(pEC);

		if (pud->extra.trans.nSelectedVert >= 0)  
		{
			//hr = pEC->Translate(pud->extra.trans.newPosition.x + fX, pud->extra.trans.newPosition.y + fY);
			hr = pEC->Translate(pud->extra.trans.newPosition.x, pud->extra.trans.newPosition.y);
			if (FAILED(hr))	goto FAIL_EXIT;
		}
		else //Move the whole Boundary
		{
			hr = pEC->Translate(pud->extra.trans.newPosition.x, pud->extra.trans.newPosition.y);
			if (FAILED(hr))	goto FAIL_EXIT;
		}

		break;
	case ROTATE:
		pThing = (IThing*) varTarget.pdispVal;
		ASSERT(pThing);

		if (FAILED(hr = pThing->get_ObjectProperty(bstrDirection, (IObjectProperty**)&pvPos)))
			goto FAIL_EXIT;
		if (FAILED(hr = pThing->get_ObjectProperty(bstrUp, (IObjectProperty**)&pvUp)))
			goto FAIL_EXIT;
		pvPos->set(pud->extra.rotate.newDirection.x, pud->extra.rotate.newDirection.y, pud->extra.rotate.newDirection.z);
		pvUp->set(pud->extra.rotate.newUp.x, pud->extra.rotate.newUp.y, pud->extra.rotate.newUp.z);
		break;
	case ROTATE2D:
		pEC = (IBoundary*) varTarget.pdispVal;
		ASSERT(pEC);

		hr = pEC->Rotate(0.0f);
		break;
	case NEWBOUNDARY:
		pEC = (IBoundary*) varTarget.pdispVal;
		ASSERT(pEC);

		pud->extra.newboundary.BoundaryList->Add( CComVariant(pEC) );
		if (FAILED(hr)) goto FAIL_EXIT;

		break;
	case DELETEBOUNDARY:
		pEC = (IBoundary*) varTarget.pdispVal;
		ASSERT(pEC);

		pud->extra.newboundary.BoundaryList->Remove(CComVariant(pEC));
		if (FAILED(hr)) goto FAIL_EXIT;

		break;
	case DELETEVERTEX2D:
		pEC = (IBoundary*) varTarget.pdispVal;
		ASSERT(pEC);

		hr = pEC->DeleteVertexSafe(pud->extra.deletevertex.ecVert.index, &vbSafe);
		if (FAILED(hr)) goto FAIL_EXIT;

		hr = pud->extra.deletevertex.BoundaryList->Remove(CComVariant(pEC));
		if (FAILED(hr)) goto FAIL_EXIT;
		hr = pud->extra.deletevertex.BoundaryList->Add(CComVariant(pEC));
		if (FAILED(hr)) goto FAIL_EXIT;

		break;

	case INSERTVERTEX2D:
		pEC = (IBoundary*) varTarget.pdispVal;
		ASSERT(pEC);

		hr = pEC->InsertVertexSafe(	pud->extra.insertvertex.ecVert.index, 
									pud->extra.insertvertex.ecVert.x, 
									pud->extra.insertvertex.ecVert.y, 
									&vbSafe);
		if (FAILED(hr)) goto FAIL_EXIT;

		hr = pud->extra.insertvertex.BoundaryList->Remove(CComVariant(pEC));
		if (FAILED(hr)) goto FAIL_EXIT;
		hr = pud->extra.insertvertex.BoundaryList->Add(CComVariant(pEC));
		if (FAILED(hr)) goto FAIL_EXIT;

		break;
	case FLIPBOUNDARY:
		//pECB = (IBoundaryBuilder*) varTarget.pdispVal;
		//ASSERT(pECB);
		//pECB->ReverseVertices();
		break;
	default:
		OutputDebugString("CVWExecuteGeomRedo::Redo: Unhandled redo type\n");
	}

FAIL_EXIT:
	SAFERELEASE(pvUp);
	SAFERELEASE(pvPos);
	SAFERELEASE(pEC);
	SAFERELEASE(pBoundaryList);
	
	return hr;
}
