// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// PolyTool.cpp : Implementation of CPolyTool
#include "stdafx.h"
#include <stdtools.h>
#include "PolyTool.h"
#include "GeomUndo.h"
#include "UndoUtil.h"
#include <PickData.h>
//#include <mmhelp.h>
#include "vwsystem.h"
#include <vwrnedit.h>

#include <vwmm_i.c>

/////////////////////////////////////////////////////////////////////////////
EXTERN_C const CLSID CLSID_Boundary;

#define dispidToolEvent 19L
#define IDEVENT 3

const IID DIID__RenderingControl = { 0x44fac781, 0xca4, 0x11d0, { 0x89, 0xa9, 0, 0xa0, 0xc9, 0x5, 0x41, 0x29 } };

static CComBSTR bstrBoundaryList("BoundaryList");
static CComBSTR bstrNULL("NULL");

/////////////////////////////////////////////////////////////////////////////
// CPolyTool

/////////////////////////////////////////////////////////////////////////////
//
// Valid when a single BoundaryList or Thing is Selected in the ViewControl
// OnLButtonDown
//	If Valid selection:
//		If no BoundaryList exists add one
//		Add boundary to BoundaryList with one vertex corresponding to the mouse down point.

/////////////////////////////////////////////////////////////////////////////
enum PickResults
{
	PICKRESULT_NOBOUNDARY = 0,
	PICKRESULT_POINTUNSAFE,
	PICKRESULT_INSERTOK,
	PICKRESULT_PERMISSION_DENIED,
};

/////////////////////////////////////////////////////////////////////////////

typedef CVWComPtr<IObjectProperty,NULL,&IID_IObjectProperty> CObjectPropertyPtr;
typedef CVWComPtr<IThing,NULL,&IID_IThing> CThingPtr;
typedef CVWComPtr<IPropertyList,NULL,&IID_IPropertyList> CBoundaryListPtr;
typedef CVWComPtr<IBoundary,NULL,&IID_IBoundary> CBoundaryPtr;
typedef CVWComPtr<IVWExecuteUndo, &CLSID_VWExecuteGeomUndo, &IID_IVWExecuteUndo> CExecUndoPtr;
typedef CVWComPtr<IVWPickData, NULL,&IID_IVWPickData> CVWPickDataPtr;
/////////////////////////////////////////////////////////////////////////////


CPolyTool::CPolyTool()
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HRESULT hr;
	CExecUndoPtr pExecUndo;

	m_pCurrentThing = NULL;
	m_pCurrentBoundaryList = NULL;
	m_pBoundary = NULL;
	m_pCurrentThingFrame = NULL;

	m_cVertices = 0;

	m_fLastVertexLegal = FALSE;

	hr = pExecUndo.CoCreateInstance();
	if (FAILED(hr)) goto EXIT_FAILED;

	m_pExecuteUndo = (IVWExecuteUndo *) pExecUndo;
	m_pExecuteUndo->AddRef();

	SetCursor(m_pUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CREATEPEC)));
	m_bShowingInsertCursor = FALSE;

	m_bShowRubberBand = FALSE;
	m_piEditorViewControl = NULL;

EXIT_FAILED:
	return;
}


CPolyTool::~CPolyTool()
{
	TRACE("CPolyTool::~CPolyTool\n");
	SAFERELEASE(m_piEditorViewControl);
	SAFERELEASE(m_pCurrentThing);
	SAFERELEASE(m_pCurrentBoundaryList);
	SAFERELEASE(m_pCurrentThingFrame);
	SAFERELEASE(m_pExecuteUndo);
}

int CPolyTool::TryInsVert(VARIANT* pvarHitResult, int nX, int nY)
{
	VARIANT_BOOL vbSafe;
	EIndex pIndex;
	HRESULT hr = S_OK;
	IVWUndoItem		*pVWUndoItem = NULL;
	float flX, flY, flZ;
	C2DThingCoordTransformer c2DThingXform;


	ASSERT(pvarHitResult);

	CVWPickDataPtr hitPickDataPtr;

	hitPickDataPtr = pvarHitResult->pdispVal;

	if (hitPickDataPtr.p != NULL)
	{
		CBoundaryPtr hitBoundaryPtr;
		CBoundaryListPtr hitBoundaryListPtr;
		CThingPtr thingPtr;
		int nHitBoundaryIndex;

		hr = hitPickDataPtr->get_Boundary(&hitBoundaryPtr.p);
		if (FAILED(hr) || !hitBoundaryPtr.p) return PICKRESULT_NOBOUNDARY;

		hr = hitPickDataPtr->get_BoundaryIndex(&nHitBoundaryIndex);
		if (FAILED(hr)) return PICKRESULT_NOBOUNDARY;

		hr = hitPickDataPtr->get_BoundaryList(&hitBoundaryListPtr.p);
		if (FAILED(hr) || !hitBoundaryListPtr.p) return PICKRESULT_NOBOUNDARY;

		hr = hitPickDataPtr->get_Thing(&thingPtr.p);
		if (FAILED(hr) || !thingPtr.p) return PICKRESULT_NOBOUNDARY;

		if (FAILED(hr = thingPtr->CheckPropertySecurityExt(bstrBoundaryList, PS_WRITE)))
			return PICKRESULT_PERMISSION_DENIED;

		flX = (float) nX;
		flY = (float) nY;
		flZ = 0.0f;

		hr = m_pUIView->DeviceToWorld( &flX, &flY, &flZ);
		if(FAILED(hr)) goto EXIT_FAIL;

		hr = c2DThingXform.StoreThingCoords( thingPtr );
		if(FAILED(hr)) goto EXIT_FAIL;

		hr = c2DThingXform.TransformToThingCoords( &flX, &flY, &flZ);
		if(FAILED(hr)) goto EXIT_FAIL;

		pIndex = NULL_EINDEX;

		hitPickDataPtr->get_EdgeIndex(&pIndex);

		if(NULL_EINDEX != pIndex)
		{
			CComVariant pvarBoundary;

			hr = hitBoundaryPtr->InsertVertexSafe( pIndex + 1, flX, flZ, &vbSafe);
			if (FAILED(hr)) goto EXIT_FAIL;

			hr = m_pWorld->CloneProperty( CComVariant(hitBoundaryPtr), &pvarBoundary);
			if (FAILED(hr)) goto EXIT_FAIL;
				
			hr = hitBoundaryListPtr->put_Property(nHitBoundaryIndex, pvarBoundary);
			if (FAILED(hr)) goto EXIT_FAIL;

			//InvokeToolEvent(TOOLEVENT_BOUNDARYUPDATE, hitBoundaryPtr, bstrNULL, bstrNULL, bstrNULL, VARIANT_FALSE);

			if (VARIANT_TRUE == vbSafe)
			{
				m_pControlManager->Select(CComVariant(hitPickDataPtr));

				return PICKRESULT_INSERTOK;
			}
			else
				OutputDebugString("CPolyTool::OnLButtonDown - Inserted an unsafe vertex.\n");
		}
		else
			OutputDebugString("CPolyTool::OnLButtonDown - No picked edge.\n");

		return PICKRESULT_POINTUNSAFE;
	}

EXIT_FAIL:
	return PICKRESULT_NOBOUNDARY;

}

// IVWTool Methods

BOOL CPolyTool::OnLButtonDown(  UINT nFlags, POINT ptScreen )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));	

 	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;
	VARIANT_BOOL bSafe = VARIANT_TRUE;
	HWND hWnd;
	VARIANTARG varHitParams,varHitResult;
	BOOL bMakeNewBoundary = TRUE;
	D3DVECTOR vec;

	::VariantInit( &varHitParams );
	::VariantInit( &varHitResult );

	float flX = (float)ptScreen.x;
	float flY = (float)ptScreen.y;
	float flZ = 0.0;

	if(!m_pUIView || !m_pControlManager)
	{
		hr = VWSTUDIO_E_BADCONTROLINITIALIZATION;
		goto EXIT_FAIL;
	}

	if (!m_piEditorViewControl)
	{
		hr = m_pUIView->QueryInterface(DIID__RenderingControl, (LPVOID*) &m_piEditorViewControl);
		if (FAILED(hr) || (m_piEditorViewControl == NULL)) goto EXIT_FAIL;
	}

	hr = m_pUIView->DeviceToWorld( &flX, &flY, &flZ);
	if(FAILED(hr)) goto EXIT_FAIL;

	m_pUIView->get_HWND(&hWnd);

	::SetFocus(hWnd);


	//TRACE("CPolyTool::OnLButtonDown point.x = %d, y = %d, World.x = %7.2f, z = %7.2f\n", ptScreen.x, ptScreen.y, flX, flZ);
	
	// Get selected BoundaryList. Create a new an Boundary, because this is the first point of a new one.
	if(NULL == m_pBoundary)
	{
		varHitParams.vt = VT_POINT_BYREF;
		varHitParams.byref = (LPVOID) &ptScreen;

		hr = ((IVWUIView*)m_pUIView)->HitTest(varHitParams, 0, &varHitResult);	
		if( FAILED(hr)) goto EXIT_FAIL;

		//Attempt a vertex insertion if we clicked on one.
		if(varHitResult.vt != VT_EMPTY)
		{
			int nPickResult;

			nPickResult = TryInsVert(&varHitResult, ptScreen.x, ptScreen.y);

			if (nPickResult == PICKRESULT_POINTUNSAFE || nPickResult == PICKRESULT_INSERTOK || nPickResult == PICKRESULT_PERMISSION_DENIED)
			{
				if (nPickResult != PICKRESULT_INSERTOK)
					MessageBeep(MB_ICONASTERISK);

				bMakeNewBoundary = FALSE;
			}
		}

		if (bMakeNewBoundary)
		{
			//Reset all state just in case
			SAFERELEASE(m_pCurrentThing);
			SAFERELEASE(m_pCurrentThingFrame);
			SAFERELEASE(m_pCurrentBoundaryList);
			SAFERELEASE(m_pBoundary);
			m_cVertices = 0;

			hr = StartNewBoundary( &m_pBoundary );
			if( FAILED(hr) || !m_pBoundary)
			{
				MessageBeep(MB_ICONASTERISK);
				goto EXIT_FAIL; 
			}

			m_nFirstVertexX = ptScreen.x;
			m_nFirstVertexY = ptScreen.y;

			// Assume we fail.
			m_fLastVertexLegal = FALSE;

			// Insert first vertex offset by Thing's position
			//hr = m_2DThingXform.TransformToThingCoords( &flX, &flY, &flZ);
			//if(FAILED(hr)) goto EXIT_FAIL;

			vec.x = flX;
			vec.y = flY;
			vec.z = flZ;

			m_pCurrentThingFrame->InverseTransform(AXIS_Y, &vec.x, &vec.y, &vec.z, vec.x, vec.y, vec.z);

			hr = m_pBoundary->InsertVertex(-1, vec.x, vec.z);
			if( FAILED(hr) ) goto EXIT_FAIL;

			//Start with 2 vertices the same, boundaries require at least 2.
			hr = m_pBoundary->InsertVertex(-1, vec.x + (float)0.001 , vec.z);
			if( FAILED(hr) ) goto EXIT_FAIL;

			// The second vert will be moved to the second vertex position.
			m_cVertices = 1;
			m_flLastX = vec.x;
			m_flLastZ = vec.z;

			// Add single Boundary to BoundaryList.
			hr = m_pCurrentBoundaryList->Add( CComVariant(m_pBoundary) );
 			if( FAILED(hr) ) goto EXIT_FAIL;

			//InvokeToolEvent(TOOLEVENT_BOUNDARYUPDATE, m_pBoundary, NULL, NULL, NULL, VARIANT_FALSE);
		}
	}  
	else // Add a Vertex to existing Boundary
	{
		//If we're near the first vertex, we're done with the boundary.
		if ( abs(m_nFirstVertexX - ptScreen.x) < 5 &&
		     abs(m_nFirstVertexY - ptScreen.y) < 5 )
		{
			ClosePolygon(nFlags, ptScreen);
		}
		else
		{
			// Assume we fail.
			m_fLastVertexLegal = FALSE;

			vec.x = flX;
			vec.y = flY;
			vec.z = flZ;

			m_pCurrentThingFrame->InverseTransform(AXIS_Y, &vec.x, &vec.y, &vec.z, vec.x, vec.y, vec.z);

//			hr = m_2DThingXform.TransformToThingCoords( &flX, &flY, &flZ);
//			if(FAILED(hr)) goto EXIT_FAIL;

			if( (m_flLastX != flX) && (m_flLastZ != flZ) )
			{
				if( 1 == m_cVertices )
				{
//					float fTmpX, fTmpZ;
//					hr = m_pBoundary->GetVertexXYExt(1, &fTmpX, &fTmpZ);
//					if( FAILED(hr) ) goto EXIT_FAIL;
//
//					// Move the second vert, which was added to satisfy the Boundary.
//					hr = m_pBoundary->TranslateVertexSafe(1, flX - fTmpX, flZ - fTmpZ, &bSafe);
//					if( FAILED(hr) ) goto EXIT_FAIL;

					hr = m_pBoundary->SetVertexXY(1, vec.x, vec.z);
					if( FAILED(hr) ) goto EXIT_FAIL;

					bSafe = TRUE;
				}
				else
				{
					// Insert the next point into the BoundaryBuilder
					hr = m_pBoundary->InsertVertexSafe(-1, vec.x, vec.z, &bSafe );
					if( FAILED(hr) ) goto EXIT_FAIL;
				}

				if (bSafe)
				{
					m_flLastX = vec.x;
					m_flLastZ = vec.z;
					m_cVertices++;
				}

				InvokeToolEvent(TOOLEVENT_BOUNDARYUPDATE, m_pBoundary, bstrNULL, bstrNULL, bstrNULL, VARIANT_FALSE);
			}
		}
	}

	// Set this to signal OnLButtonDblClk that last vertex was good.
	m_fLastVertexLegal = TRUE;

	goto EXIT_SUCCEEDED;

EXIT_FAIL:
	SAFERELEASE(m_pCurrentThing);
	SAFERELEASE(m_pCurrentBoundaryList);
	SAFERELEASE(m_pCurrentThingFrame);
	SAFERELEASE(m_pBoundary);

	m_cVertices = 0;

	//MessageBeep(-1);

EXIT_SUCCEEDED:
	::VariantClear( &varHitParams );
	::VariantClear( &varHitResult );
	if (S_OK == hr)
		bHandled = TRUE;

	return bHandled;
}


BOOL CPolyTool::OnMouseMove(  UINT nFlags, POINT ptScreen )
{
	HRESULT hr = S_OK;
	float flX, flY, flZ;
	VARIANT_BOOL bSafe;
	static DWORD dwTime = - 1;
	DWORD dwTimeNow;
	CComBSTR bstr1, bstr2, bstr3;
	CString tmpStr;

	flX = (float)ptScreen.x;
	flY = (float)ptScreen.y;
	flZ = 0.0f;

	if(!m_pUIView || !m_pControlManager)
	{
		hr = VWSTUDIO_E_BADCONTROLINITIALIZATION;
		goto EXIT_FAIL;
	}

	if (!m_piEditorViewControl)
	{
		hr = m_pUIView->QueryInterface(DIID__RenderingControl, (LPVOID*) &m_piEditorViewControl);
		if (FAILED(hr) || (m_piEditorViewControl == NULL)) goto EXIT_FAIL;
	}

	dwTimeNow = GetTickCount();
	if (dwTimeNow - dwTime > 200)
	{
		hr = m_pUIView->DeviceToWorld( &flX, &flY, &flZ);
		if(FAILED(hr)) goto EXIT_FAIL;

		if (m_pCurrentThing)
		{
			// Insert first vertex offset by Thing's position
			hr = m_2DThingXform.TransformToThingCoords( &flX, &flY, &flZ);
			if(FAILED(hr)) goto EXIT_FAIL;
		}

		tmpStr.Format("%0.3f", flX);
		bstr1 = tmpStr;
		tmpStr.Format("%0.3f", flZ);
		bstr2 = tmpStr;
		bstr3 = tmpStr;

		InvokeToolEvent(TOOLEVENT_2DMOUSEPOSITION, m_pBoundary, bstr1, bstr2, bstr3, VARIANT_TRUE);

		dwTime = dwTimeNow;
	}

	if (m_bShowRubberBand &&	
		(abs(m_nLastMousePosX - ptScreen.x) > 5 || 
		abs(m_nLastMousePosY - ptScreen.y) > 5) )
	{
		hr = m_pUIView->DeviceToWorld( &flX, &flY, &flZ);
		if(FAILED(hr)) goto EXIT_FAIL;

		if(NULL != m_pBoundary && m_cVertices > 0)
		{
			hr = m_2DThingXform.TransformToThingCoords( &flX, &flY, &flZ);
			if(FAILED(hr)) goto EXIT_FAIL;

			if( (m_flLastX != flX) && (m_flLastZ != flZ) )
			{
				if (m_cVertices == 1)
				{
					// Move the second vert, which was added to satisfy the Boundary.
					hr = m_pBoundary->TranslateVertexSafe(1, flX, flZ, &bSafe);
					if( FAILED(hr) ) goto EXIT_FAIL;
				}
				else if (m_cVertices > 1)
				{
					// Move the second vert, which was added to satisfy the Boundary.
					hr = m_pBoundary->TranslateVertexSafe(m_cVertices - 1, flX, flZ, &bSafe);
					if( FAILED(hr) ) goto EXIT_FAIL;
				}
			}
		}

		m_nLastMousePosX = ptScreen.x;
		m_nLastMousePosY = ptScreen.y;
	}

	return TRUE;
EXIT_FAIL:
	return FALSE;
}


// Try to complete the Boundary.
// If vertex already inserted by LButtonDown, CompileFinal and clean up.
BOOL CPolyTool::OnLButtonDblClk(  UINT nFlags, POINT pt  )
{
	return ClosePolygon(  nFlags, pt  );
}
	
BOOL CPolyTool::ClosePolygon(  UINT nFlags, POINT pt  )
{
 	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;
	VARIANT_BOOL bSafe = VARIANT_TRUE, bOutside = VARIANT_TRUE;
	long lCount;
	IVWUndoItem		*pVWUndoItem = NULL;
	IVWPickData* pickDataPtr = NULL;
	IThing* pThing = NULL;
	CComVariant pvarBoundary;

	// If last vertex inserted by LButtonDown was good, try to compile
	// a simple polygon.
	if( NULL != m_fLastVertexLegal && NULL != m_pUIView && NULL != m_pBoundary && NULL != m_pCurrentBoundaryList)
	{
		hr = m_pBoundary->put_IsClosed(VARIANT_TRUE);
		if (FAILED(hr)) goto EXIT_FAIL;
	
		hr = m_pCurrentBoundaryList->get_Count(&lCount);
		if( FAILED(hr) ) goto EXIT_FAIL;

//		hr = m_pCurrentBoundaryList->RemoveAt( lCount - 1 );
//		if (FAILED(hr)) goto EXIT_FAIL;
//
//		hr = m_pCurrentBoundaryList->InsertAt(lCount - 1, CComVariant( m_pBoundary ) );
//		if (FAILED(hr)) goto EXIT_FAIL;

		hr = m_pWorld->CloneProperty( CComVariant(m_pBoundary), &pvarBoundary);
		if (FAILED(hr)) goto EXIT_FAIL;

		hr = m_pCurrentBoundaryList->put_Property(lCount - 1, pvarBoundary);
		if (FAILED(hr)) goto EXIT_FAIL;

		if(m_pUIView && m_pControlManager)
		{
			hr = CoCreateInstance( CLSID_VWPickData,NULL,CLSCTX_INPROC_SERVER, IID_IVWPickData,(void**) &pickDataPtr );
			if( FAILED(hr) ) goto EXIT_FAIL;

			hr = pickDataPtr->put_Boundary(m_pBoundary);
			if( FAILED(hr) ) goto EXIT_FAIL;

			hr = pickDataPtr->put_BoundaryIndex(lCount-1);
			if( FAILED(hr) ) goto EXIT_FAIL;

			hr = pickDataPtr->put_BoundaryList(m_pCurrentBoundaryList);
			if( FAILED(hr) ) goto EXIT_FAIL;

			hr = pickDataPtr->put_Thing(m_pCurrentThing);
			if( FAILED(hr) ) goto EXIT_FAIL;

			hr = pickDataPtr->put_DataType(ePickedBoundary);
			if (FAILED(hr)) goto EXIT_FAIL;

			hr = m_pControlManager->ClearSelectionsExt( m_pUIView  );
			if( FAILED(hr) ) goto EXIT_FAIL;

			hr = m_pControlManager->AddSelectionExt( m_pUIView, CComVariant( pickDataPtr ) );
			if( FAILED(hr)) goto EXIT_FAIL;
		}
		else
		{
			hr = VWSTUDIO_E_BADCONTROLINITIALIZATION;
			goto EXIT_FAIL;
		}

		bHandled = TRUE;

		m_cVertices = 0;

//		ClipCursor(NULL);
//		ReleaseCapture();

		SAFERELEASE(m_pCurrentThing);
		SAFERELEASE(m_pCurrentThingFrame);
		SAFERELEASE(m_pBoundary);
		SAFERELEASE(m_pCurrentBoundaryList);

	}
	else
	{
		MessageBeep(MB_ICONASTERISK);
	}


EXIT_FAIL: 

//	SAFERELEASE( pPL );
	SAFERELEASE( pThing );
	return bHandled;
}


STDMETHODIMP CPolyTool::IsValid( IVWUIView * pVw, VARIANT_BOOL * pfValid)
{
	ASSERT( pfValid );
	if (!pfValid )
		return E_POINTER;

	*pfValid = TRUE; // For now.
	// Review: Make this check for only one BoundaryList in selection collection of ToolManager.
	return S_OK;
}

BOOL CPolyTool::FindWithin(IThing* pContainer, IThing* pThingToFind)
{
	IThing* pthing = NULL;
	long propcnt = 0;
	int i;
	IPropertyList* pContents = NULL;
	HRESULT hr = S_OK;
	BOOL bFound = FALSE;

	if (pContainer == pThingToFind)
	{
		bFound = TRUE;
		goto EXIT_FAIL;
	}

	hr = pContainer->get_Contents(&pContents);
	if (FAILED(hr) || !pContents) goto EXIT_FAIL;

	hr = pContents->get_Count(&propcnt);
	if (FAILED(hr))	goto EXIT_FAIL;

	for (i = 0; i < propcnt && !bFound; i++)
	{
		if (SUCCEEDED(pContents->get_Thing(i, &pthing)) && pthing)
		{
			if (pThingToFind == pthing)
				bFound = TRUE;
			else
				bFound = FindWithin(pthing, pThingToFind);
		}
		SAFERELEASE(pthing);
	}

EXIT_FAIL:
	SAFERELEASE(pContents);
	SAFERELEASE(pthing);
	return bFound;
}

STDMETHODIMP CPolyTool::StartNewBoundary(IBoundary **ppEC )
{
	HRESULT hr = S_OK;
	IPropertyList * pPL = NULL;
	IVector *pVector = NULL;
	IObjectProperty * pOP = NULL;
	IObjectProperty * pOPTargetObject = NULL;
	IWorld * pWorld = NULL;
	IVWGeometry	*pGeometry = NULL;
	IVWFrame *pVWFrame = NULL;
	CBoundaryPtr cptrBoundary;
	CString strPropName;
	static CComBSTR bstrBoundaryList("BoundaryList");
	static CComBSTR bstrBoundaryListType("Boundary List");
	CThingPtr cptrThing;
	VARIANT_BOOL fBoundaryListPropertyExists = FALSE;
	CComVariant fX1, fY1, fZ1, fX2, fY2, fZ2;
	COleVariant var;
	CVWPickDataPtr pickDataPtr;
	IThing* pUser = NULL, *pContainer = NULL, *pRoot = NULL;
	CComVariant varResult;


	if(NULL == ppEC )
	{
		hr = E_POINTER;
		goto EXIT_FAIL;
	}

	hr = m_pControlManager->get_SelectionList( &pPL );
	if( FAILED(hr ) ) 
	{
		hr = VWSTUDIO_E_BADCONTROLINITIALIZATION;
		goto EXIT_FAIL;
	}

	if(NULL == pPL)
	{
		hr = VWSTUDIO_E_BADCONTROLINITIALIZATION;
		goto EXIT_FAIL;
	}

	// Get the IWorld of the VWUIView's TargetObjectPRoperty
	hr = m_pUIView->get_TargetObjectProperty(&pOPTargetObject);
	if( FAILED(hr) ) goto EXIT_FAIL;

	ASSERT(pOPTargetObject);
	if(NULL == pOPTargetObject)
	{
		hr = VWSTUDIO_E_BADCONTROLINITIALIZATION;
		goto EXIT_FAIL;
	}

	hr = pOPTargetObject->get_World(&pWorld);
	if( FAILED(hr) ) goto EXIT_FAIL;

	/// Get the thing we're attaching to.
	hr = fSelectionGetFirst(pPL, &pOP);
	if( VWSTUDIO_E_INVALIDSELECTION == hr) 
	{
		hr = m_pControlManager->ClearSelectionsExt( m_pUIView  );
		if( FAILED(hr) ) goto EXIT_FAIL;

		hr = m_pControlManager->AddSelectionExt( m_pUIView, CComVariant( pOPTargetObject ) );
		if( FAILED(hr)) goto EXIT_FAIL;

		pOP = pOPTargetObject;
		pOP->AddRef();
		hr = S_OK;
	}
	if( FAILED(hr) ) goto EXIT_FAIL;

	pickDataPtr = pOP;
	if (pickDataPtr != NULL)
	{
		hr = pickDataPtr->get_Thing(&cptrThing.p);
		if(FAILED(hr) || !cptrThing.p) goto EXIT_FAIL;

		hr = m_pControlManager->ClearSelectionsExt( m_pUIView  );
		if( FAILED(hr) ) goto EXIT_FAIL;

		hr = m_pControlManager->AddSelectionExt( m_pUIView, CComVariant( cptrThing.p ) );
		if( FAILED(hr)) goto EXIT_FAIL;
	}
	else
	{
		cptrThing = pOP;
	}

	if(cptrThing == NULL)
	{
		//MessageBeep(MB_ICONASTERISK);
		hr = E_FAIL;
		goto EXIT_FAIL;
	}

	hr = cptrThing->CheckPropertySecurityExt(bstrBoundaryList, PS_WRITE);
	if( FAILED(hr) ) goto EXIT_FAIL;

	hr = pWorld->get_User(&pUser);
	if( FAILED(hr)) goto EXIT_FAIL;

	hr = pUser->get_Container(&pContainer);
	if( FAILED(hr)) goto EXIT_FAIL;

	if (!pContainer)
	{
		hr = E_FAIL;
		goto EXIT_FAIL;
	}

	while (SUCCEEDED(pContainer->get_Container(&pRoot)) && pRoot != NULL)
	{
		SAFERELEASE(pContainer);
		pContainer = pRoot;
		pRoot = NULL;
	}

	if (FindWithin(pContainer, cptrThing.p) == FALSE)
	{
		hr = E_FAIL;
		goto EXIT_FAIL;
	}

	// It's a Thing, so get the BoundaryList property.  If it doesn't exist, create it.
	hr = cptrThing->get_ObjectPropertyExt(CComBSTR("BoundaryList"),IID_IPropertyList, (IObjectProperty**)&m_pCurrentBoundaryList);
	if( FAILED(hr) || ( NULL == m_pCurrentBoundaryList ))
	{   
		// Create a BoundaryList and add it.
		//::CreateBoundaryList(pWorld, &m_pCurrentBoundaryList);
		hr = pWorld->CreateObjectPropertyExt(CLSID_PropertyList, NULL, (IObjectProperty**) &m_pCurrentBoundaryList);
		if (FAILED(hr) || !m_pCurrentBoundaryList) goto EXIT_FAIL;

		//Set the type
		hr = m_pCurrentBoundaryList->put_Type(bstrBoundaryListType);
		if( FAILED(hr) ) goto EXIT_FAIL;

		// Add it to the selected Thing.
		hr = cptrThing->IsValidProperty(CComBSTR("BoundaryList"),&fBoundaryListPropertyExists );
		if( FAILED(hr) ) goto EXIT_FAIL;

		// Either add or put BoundaryList property.
		if(VARIANT_TRUE == fBoundaryListPropertyExists)
		{
			hr = cptrThing->put_ObjectProperty(CComBSTR("BoundaryList"), (IObjectProperty*)m_pCurrentBoundaryList);
			ASSERT(SUCCEEDED(hr));
			if( FAILED(hr) ) goto EXIT_FAIL;
		}
		else // Doesn't exist, Add it,
		{
			hr = cptrThing->AddObjectProperty(CComBSTR("BoundaryList"), (IObjectProperty*)m_pCurrentBoundaryList);
			ASSERT(SUCCEEDED(hr));
			if( FAILED(hr) ) goto EXIT_FAIL;
		}
	}

	// Store Thing
	SAFERELEASE(m_pCurrentThing);
	m_pCurrentThing = cptrThing;
	SAFEADDREF(m_pCurrentThing);
	
	SAFERELEASE(m_pCurrentThingFrame);
	hr = m_pCurrentThing->InvokeMethodExt(CComBSTR("GetFrame"), NULL, &varResult);
	if( FAILED(hr) ) goto EXIT_FAIL;

	if (varResult.vt != VT_DISPATCH || varResult.pdispVal == NULL)
	{
		hr = E_FAIL;
		goto EXIT_FAIL;
	}

	m_pCurrentThingFrame = (IVWFrame*) varResult.pdispVal;
	SAFEADDREF(m_pCurrentThingFrame);

	ASSERT(m_pCurrentBoundaryList);
	ASSERT(m_pCurrentThing);
	SAFERELEASE(pOP);

	if(NULL == m_pCurrentBoundaryList || NULL == m_pCurrentThing)
	{
		hr = E_FAIL;
		goto EXIT_FAIL;
	}

	// Store the coords of the Thing for transforming points into its frame
	hr = m_2DThingXform.StoreThingCoords(m_pCurrentThing);
	if( FAILED(hr) ) goto EXIT_FAIL;

	// Create the new Boundary.
	hr = pWorld->CreateObjectPropertyExt(CLSID_Boundary,NULL,(IObjectProperty**)ppEC);
	if( FAILED(hr) || !(*ppEC) ) goto EXIT_FAIL;

	hr = (*ppEC)->put_IsClosed(VARIANT_FALSE);
	if (FAILED(hr)) goto EXIT_FAIL;
	
	hr = m_pCurrentThing->get_ObjectProperty(CComBSTR("Geometry"), (IObjectProperty**) &pGeometry);
	if (hr == VWOBJECT_E_PROPERTYNOTEXIST)
	{
		hr = (*ppEC)->put_HeightLower(0.0F);
		if( FAILED(hr) ) goto EXIT_FAIL;

		hr = (*ppEC)->put_HeightUpper(1.0F);
		if( FAILED(hr) ) goto EXIT_FAIL;
	}
	else 
	{
		if( FAILED(hr) ) goto EXIT_FAIL;

		//hr = pGeometry->get_Frame(&pVWFrame);
		//if( FAILED(hr) ) goto EXIT_FAIL;

		hr = m_pCurrentThingFrame->GetBoundingBox(	&fX1, &fY1, &fZ1, &fX2, &fY2, &fZ2 );
		if( FAILED(hr) ) goto EXIT_FAIL;

		hr = (*ppEC)->put_HeightLower(fY1.fltVal);
		if( FAILED(hr) ) goto EXIT_FAIL;

		hr = (*ppEC)->put_HeightUpper(fY2.fltVal);
		if( FAILED(hr) ) goto EXIT_FAIL;
	}

EXIT_FAIL:
	SAFERELEASE( pWorld );
	SAFERELEASE( pVector );
	SAFERELEASE( pPL );
	SAFERELEASE( pOP );
	SAFERELEASE( pGeometry );
	SAFERELEASE( pVWFrame );
	SAFERELEASE( pOPTargetObject );
	SAFERELEASE(pUser);
	SAFERELEASE(pContainer);
	SAFERELEASE(pRoot);

	return hr;
}



HRESULT CPolyTool::fSelectionGetFirst(IPropertyList * pPL, IObjectProperty **ppOP)
{
	HRESULT hr = S_OK;
	long lcList;
	CBoundaryPtr cptrBoundary;
	CThingPtr cptrThing;
	COleVariant var;

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

	hr = pPL->get_ObjectProperty(0, ppOP);
	if(FAILED(hr)) goto EXIT_FAIL;

	goto EXIT_FAIL;

/*	for (i = 0; i < lcList; i++)
	{
		VariantClear(&var);

		hr = pPL->get_Property(i, &var);
		if(FAILED(hr)) goto EXIT_FAIL;

		goto EXIT_FAIL;

		cptrThing = var;
		if ( !(cptrThing == NULL) )
		{
			*ppOP = cptrThing;
			VariantClear(&var);
			SAFEADDREF(*ppOP);
			goto EXIT_FAIL;
		}
		else 
		{
			cptrBoundary = var;
			if ( ! (cptrBoundary == NULL) )
			{
				*ppOP = cptrBoundary;
				VariantClear(&var);
				SAFEADDREF(*ppOP);
				goto EXIT_FAIL;
			}
		}

		VariantClear(&var);
	}
*/

	hr = VWSTUDIO_E_INVALIDSELECTION;

EXIT_FAIL:
	return hr;
}



BOOL CPolyTool::OnKeyDown( UINT nVirtKey, LPARAM lKeyData )
{
	HRESULT hr = S_OK;
	VARIANT_BOOL vbBool = VARIANT_TRUE;
	long lCount;

	if (nVirtKey == VK_BACK)
	{
		if(NULL != m_pBoundary)
		{
			if (m_cVertices > 2 )
			{
				hr = m_pBoundary->DeleteVertex(m_cVertices - 1);
				if( FAILED(hr) ) goto EXIT_FAIL;

//				if(vbBool != VARIANT_TRUE)
//					MessageBeep(MB_ICONASTERISK);
//				else
//				{
					m_cVertices--;
					InvokeToolEvent(TOOLEVENT_BOUNDARYUPDATE, m_pBoundary, bstrNULL, bstrNULL, bstrNULL, VARIANT_FALSE);
//				}

			}
			else
			{
				hr = m_pCurrentBoundaryList->get_Count(&lCount);
				if( FAILED(hr) ) goto EXIT_FAIL;

				hr = m_pCurrentBoundaryList->RemoveAt( lCount-1 );
				if( FAILED(hr) ) goto EXIT_FAIL;

				m_cVertices = 0;

//				ClipCursor(NULL);
//				ReleaseCapture();

				SAFERELEASE(m_pCurrentThing);
				SAFERELEASE(m_pBoundary);
				SAFERELEASE(m_pCurrentThingFrame);
				SAFERELEASE(m_pCurrentBoundaryList);
				
			}
			return TRUE;
		}
	}
	else if (nVirtKey == VK_RETURN && m_cVertices > 2)
	{
		ClosePolygon(0, CPoint(0,0));
		return TRUE;
	}

	//MessageBeep(-1);


EXIT_FAIL:
	return FALSE; 
}



STDMETHODIMP CPolyTool::get_ShowRubberBand(VARIANT_BOOL *pVal)
{
	if (NULL == pVal)
		return E_INVALIDARG;

	*pVal = (m_bShowRubberBand ? VARIANT_TRUE : VARIANT_FALSE);

	return S_OK;
}

STDMETHODIMP CPolyTool::put_ShowRubberBand(VARIANT_BOOL newVal)
{

	m_bShowRubberBand = newVal;

	return S_OK;
}

STDMETHODIMP CPolyTool::OnToolChanging(VARIANT varHint)
{
	HRESULT hr = S_OK;
	long lCount;
	HWND hWnd;


	if (nTimerID)
	{
		m_pUIView->get_HWND(&hWnd);

		KillTimer(hWnd, nTimerID);
		nTimerID = 0;
	}

	if(NULL != m_pBoundary)
	{

		hr = m_pCurrentBoundaryList->get_Count(&lCount);
		if( FAILED(hr) ) goto EXIT_FAIL;

		m_pCurrentBoundaryList->RemoveAt( lCount-1 );
		
		SAFERELEASE(m_pCurrentThing);
		SAFERELEASE(m_pBoundary);
		SAFERELEASE(m_pCurrentThingFrame);
		SAFERELEASE(m_pCurrentBoundaryList);

		m_cVertices = 0;
	}
EXIT_FAIL:

	return S_OK;
}

STDMETHODIMP CPolyTool::OnToolActivate( VARIANT varHint )
{
	HWND hWnd;

	m_pUIView->get_HWND(&hWnd);

	nTimerID = SetTimer(hWnd, IDEVENT, 200, NULL);

	return S_OK;
}

BOOL CPolyTool::OnTimer( UINT nIDEvent , TIMERPROC * tmprc)
{
	HRESULT hr = S_OK;
	VARIANTARG varHitParams,varHitResult;
	POINT pt;
	HWND hWnd;
	BOOL bHoveringOverBoundary = FALSE;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_pUIView->get_HWND(&hWnd);

	GetCursorPos(&pt);
	ScreenToClient(hWnd, &pt);

	VariantInit(&varHitParams);
	VariantInit(&varHitResult);

	if (!m_pBoundary)
	{
		varHitParams.vt = VT_POINT_BYREF;
		varHitParams.byref = (LPVOID) &pt;

		hr = ((IVWUIView*)m_pUIView)->HitTest(varHitParams, 0, &varHitResult);	
		if( FAILED(hr)) goto EXIT_FAIL;

		if (varHitResult.vt == VT_DISPATCH && varHitResult.pdispVal)
		{
			CVWPickDataPtr pickPtr;

			pickPtr = varHitResult.pdispVal;

			if (pickPtr.p)
			{
				bHoveringOverBoundary = TRUE;
			}
		}
	}

	if (!m_bShowingInsertCursor && bHoveringOverBoundary)
	{
		SetCursor(m_pUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_INSVERT)));
		m_bShowingInsertCursor = TRUE;
	}
	else if (m_bShowingInsertCursor && !bHoveringOverBoundary)
	{
		SetCursor(m_pUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CREATEPEC)));
		m_bShowingInsertCursor = FALSE;
	}


EXIT_FAIL:
	VariantClear(&varHitParams);
	VariantClear(&varHitResult);

	return S_OK;

}

HRESULT CPolyTool::InvokeToolEvent(int nNum, IObjectProperty* pOP, BSTR bstr1, BSTR bstr2, BSTR bstr3, VARIANT_BOOL bBubble)
{
	HRESULT hr = S_OK;
	DISPPARAMS dispparams;
	VARIANT varResult;

	VariantInit(&varResult);

	hr = InitializeDISPPARAMS(&dispparams, 6);
	if (FAILED(hr)) goto EXIT_FAIL;

	dispparams.rgvarg[5].vt = VT_I4;
	dispparams.rgvarg[5].lVal = nNum;
	dispparams.rgvarg[4].vt = VT_DISPATCH;
	dispparams.rgvarg[4].pdispVal = pOP;
	dispparams.rgvarg[3].vt = VT_BSTR;
	dispparams.rgvarg[3].bstrVal = bstr1;
	dispparams.rgvarg[2].vt = VT_BSTR;
	dispparams.rgvarg[2].bstrVal = bstr2;
	dispparams.rgvarg[1].vt = VT_BSTR;
	dispparams.rgvarg[1].bstrVal = bstr3;
	dispparams.rgvarg[0].vt = VT_BOOL;
	dispparams.rgvarg[0].boolVal = bBubble;

	hr = m_piEditorViewControl->Invoke(dispidToolEvent, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispparams, &varResult, NULL, NULL);

	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

	if (FAILED(hr)) goto EXIT_FAIL;

	//TRACE("EventTime: %d\n", GetTickCount() - dwTime );

	VariantClear(&varResult);

EXIT_FAIL:
	return hr;
}
