// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// Rotat2D.cpp : Implementation of CVWScale2DTool

#include "stdafx.h"
#include <d3drm.h>
#include <stdtools.h>
#include "Scale2D.h"
#include "GeomUndo.h"
#include "UndoUtil.h"
#include <math.h>

/////////////////////////////////////////////////////////////////////////////
typedef CVWComPtr<IVWTransform,NULL,&IID_IVWTransform> CVWTransformPtr;
typedef CVWComPtr<IThing,NULL,&IID_IThing> CThingPtr;
typedef CVWComPtr<IPropertyList,NULL,&IID_IPropertyList> CBoundaryListPtr;
typedef CVWComPtr<IBoundary,NULL,&IID_IBoundary> CBoundaryPtr;
typedef CVWComPtr<IVWExecuteUndo, &CLSID_VWExecuteGeomUndo, &IID_IVWExecuteUndo> CExecUndoPtr;
typedef CVWComPtr<IVWPickData,NULL,&IID_IVWPickData> CVWPickDataPtr;
typedef CVWComPtr<IVector,NULL,&IID_IVector> CVectorPtr;


/////////////////////////////////////////////////////////////////////////////
#define dispidToolEvent 19L
#define X_LOCK 1
#define Y_LOCK 2
#define Z_LOCK 4
#define IsPressed(key)			(GetAsyncKeyState(key) < 0)
#define PI 3.141596
#define dispidCenterViewOnObject 18L
#define dispidToolEvent 19L
#define dispidResetCameraPosition 12L
#define CLAMP(x, xmin, xmax)	if ((x) < (xmin)) (x) = (xmin); else if ((x) > (xmax)) (x) = (xmax)
#define dwMaxTimeDelta			250				// maximum of 250ms (4fps) of dead reckoning 
#define SLOWKEY_SLOWFACTOR		0.2f
#define fObjectNavScale			24.0f			// General speed of object navigation
#define fMouseScaleFactor		0.45f

extern CComBSTR bstrBoundaryList;
extern CComBSTR bstrNavigate;
extern CComBSTR bstrXAxisLocked;
extern CComBSTR bstrYAxisLocked;
extern CComBSTR bstrZAxisLocked;
extern CComBSTR bstrGravity;
extern CComBSTR bstrAddSelection;
extern CComBSTR bstrRemoveSelection;
extern CComBSTR bstrClearSelection;
extern CComBSTR bstrAddCurrentTool;

void GetRotationFromVector(float* fRot, D3DVECTOR* pvec);
void RotateVector(float fRot, D3DVECTOR* pvec);

const IID DIID__RenderingControl = { 0x44fac781, 0xca4, 0x11d0, { 0x89, 0xa9, 0, 0xa0, 0xc9, 0x5, 0x41, 0x29 } };
static CComBSTR bstrNULL("NULL");

/////////////////////////////////////////////////////////////////////////////
// CVWScale2DTool

CScaleObject::CScaleObject( IVWPickData* pPickData)
{
	ASSERT( pPickData );
	IThing* pThing = NULL;
	HRESULT hr = S_OK;
	VARIANT var;

	VariantInit(&var);

	points = NULL;
	m_pPickData = pPickData;
	SAFEADDREF(m_pPickData);

	m_pPickData->get_Boundary(&m_pBoundary);

	m_pPickData->get_Thing(&pThing);

	hr = pThing->InvokeMethodExt(CComBSTR("GetFrame"), NULL, &var);
	if (FAILED(hr) || var.vt == VT_EMPTY) goto EXIT_FAIL;

	m_pVWFrame = (IVWFrame*) var.pdispVal;
	SAFEADDREF(m_pVWFrame);


EXIT_FAIL:
	VariantClear(&var);
	SAFERELEASE(pThing);
	return;
}

CScaleObject::~CScaleObject()
{
	SAFERELEASE(m_pPickData);
	SAFERELEASE(m_pBoundary);
	SAFERELEASE(m_pVWFrame);

	if (points)
		delete [] points;
}

CVWScale2DTool::CVWScale2DTool()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

//	HRESULT hr;
//	CExecUndoPtr pExecUndo;
//
//	hr = pExecUndo.CoCreateInstance();
//	if (FAILED(hr)) goto EXIT_FAILED;

	m_flTotalDeltaX = 0.0f;
	m_flTotalDeltaY = 0.0f;
	m_pVWRenderRoot = NULL;
	m_pWorld = NULL;
	m_pViewport = NULL;
	m_bGotFocus = VARIANT_FALSE;
	m_bTrackingMouse = FALSE;
	m_bShowingNoCursor = FALSE;

	m_piEditorViewControl = NULL;

	SetCursor(m_pUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_SCALE2D)));
//EXIT_FAILED:
	return;
}


CVWScale2DTool::~CVWScale2DTool( )
{	
	SAFERELEASE(m_piEditorViewControl);
	SAFERELEASE(m_pWorld);
	SAFERELEASE(m_pVWRenderRoot);
	SAFERELEASE(m_pViewport);

	DestroyTransformList();
}
// IVWTool Methods

STDMETHODIMP CVWScale2DTool::Initialize(IVWUIView* pVw, IWorld* pWorld, IVWRenderRoot *pVWRenderRoot)
{
	IVWRender3D *pRender3D = NULL;
	IVWNavigationTool * pNavTool = NULL;
	VARIANT  var;
	HRESULT hr = S_OK;
	int i;

	VariantInit(&var);

	if (m_pUIView != pVw)
	{
		SAFERELEASE(m_pUIView);
		m_pUIView = pVw;
		SAFEADDREF(pVw);
	}

	if (!m_pControlManager)
	{
		if (FAILED(hr = m_pUIView->get_ControlManager(&m_pControlManager)))
			goto EXIT_FAIL;
	}

	// A VWUIView doesn't need a ControlManager.
	if(!m_pSelectionList && m_pControlManager != NULL)
	{
		if (FAILED(hr = m_pControlManager->get_SelectionList(&m_pSelectionList)))
			goto EXIT_FAIL;
	}

	SAFERELEASE(m_pWorld);
	m_pWorld = pWorld;
	SAFEADDREF(m_pWorld);

	SAFERELEASE(m_pVWRenderRoot);
	m_pVWRenderRoot = pVWRenderRoot;
	SAFEADDREF(pVWRenderRoot);

	hr = pVWRenderRoot->get_VWRender3D(&pRender3D);
	if (FAILED(hr) || (pRender3D == NULL)) goto EXIT_FAIL;

	hr = m_pWorld->get_Tool(bstrNavigate, (IUnknown**) &pNavTool);
	if (FAILED(hr) || (pNavTool == NULL)) goto EXIT_FAIL;

	for (i = 0; i < kbMax; i++)
	{
		hr = pNavTool->get_KeyBinding((KeyConstant) i, &var);
		if (FAILED(hr)) goto EXIT_FAIL;

		m_KeyBindings[i] = var.lVal;
	}

	m_bUsingKeyboard = FALSE;

	if (!m_piEditorViewControl)
	{
		hr = m_pUIView->QueryInterface(DIID__RenderingControl, (LPVOID*) &m_piEditorViewControl);
		if (FAILED(hr) || (m_piEditorViewControl == NULL)) goto EXIT_FAIL;
	}

	//Initialize the start time
	ElapsedFrameTime();

EXIT_FAIL:
	
	SAFERELEASE(pNavTool);
	SAFERELEASE(pRender3D);
	return hr;
}

HRESULT CVWScale2DTool::SetupEnvironment()
{
	HRESULT hr = S_OK;
	IThing* pGlobal = NULL;
	VARIANT_BOOL vbLock;
	IVWRender3D *pRender3D = NULL;
	D3DRMVECTOR4D vec4d;

	hr = m_pWorld->get_Global(&pGlobal);
	if(FAILED(hr)) goto EXIT_FAIL;

	m_nAxisLock = 0;

	pGlobal->get_BOOL(bstrXAxisLocked, &vbLock);
	if(FAILED(hr)) goto EXIT_FAIL;
	if (vbLock)
		m_nAxisLock |= X_LOCK;

	pGlobal->get_BOOL(bstrYAxisLocked, &vbLock);
	if(FAILED(hr)) goto EXIT_FAIL;
	if (vbLock)
		m_nAxisLock |= Y_LOCK;

	pGlobal->get_BOOL(bstrZAxisLocked, &vbLock);
	if(FAILED(hr)) goto EXIT_FAIL;
	if (vbLock)
		m_nAxisLock |= Z_LOCK;

	hr = m_pVWRenderRoot->get_VWRender3D(&pRender3D);
	if (FAILED(hr) || (pRender3D == NULL)) goto EXIT_FAIL;

	SAFERELEASE(m_pViewport);
	hr = pRender3D->get_Viewport(&m_pViewport);

	vec4d.x = 0.0f;
	vec4d.y = 0.0f;
	vec4d.z = 0.0f;
	vec4d.w = 1.0f;

	m_pViewport->InverseTransform(&m_vecViewportZero, &vec4d);
	if(FAILED( hr )) goto EXIT_FAIL;

EXIT_FAIL:
	SAFERELEASE(pRender3D);
	SAFERELEASE(pGlobal);

	return hr;
}

HRESULT CVWScale2DTool::SetupTransformList()
{
	HRESULT hr = S_OK;
	VARIANT_BOOL fValid = VARIANT_FALSE;
	CScaleObject * pCTrans = NULL;

	SetupEnvironment();

	hr = IsValid(m_pUIView, &fValid);
	if (SUCCEEDED(hr) && !fValid)
		hr = E_FAIL;
	if(FAILED(hr)) goto EXIT_FAIL;

EXIT_FAIL:

	return hr;
}


BOOL CVWScale2DTool::OnLButtonDown(UINT nFlags, POINT pt )
{
	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;
	CScaleObject * pCTrans = NULL;
	CVWPickDataPtr cClickedPickData;
	CBoundaryPtr cClickedBoundary;
	CThingPtr pThing;
	CBoundaryListPtr pBoundaryList;
	VARIANT varHitparams, varHitResult, varFrame;
	HWND hWnd;
	D3DRMVECTOR4D vec4d;
	D3DVECTOR vecCenter, vecWorldCenter, vecTmp1;
	IVWFrame* pVWFrame = NULL;
	IVector* pLU = NULL, *pRL = NULL;
	float fX, fY;

	VariantInit(&varHitparams);
	VariantInit(&varHitResult);
	VariantInit(&varFrame);
	
	if(!m_pUIView || m_bTrackingMouse)
		return FALSE;

	m_bTrackingMouse = TRUE;
	hr = SetupTransformList();
	if (FAILED(hr))
	{
		SetCursor(m_pUIView, LoadCursor(NULL, IDC_NO ));
		m_bShowingNoCursor = TRUE;
		hr = S_OK;
	}
	else
		ShowCursor(FALSE);

	m_cptFirst = m_cptLast = pt;

	m_pUIView->get_HWND(&hWnd);

	SetCapture(hWnd);

	OnClickOrKeyDown();
	m_flTotalDeltaX = 0.0f;
	m_flTotalDeltaY = 0.0f;

	//Do a hittest and transform to find the center of scaling.
	varHitparams.vt = VT_POINT_BYREF;
	varHitparams.byref = &pt;

	hr = m_pUIView->HitTest(varHitparams, 0, &varHitResult);
	if(FAILED(hr) || varHitResult.vt == VT_EMPTY) goto EXIT_FAIL;

	cClickedPickData = varHitResult.pdispVal;
	if(cClickedPickData.p == NULL) goto EXIT_FAIL;

	hr = cClickedPickData->get_Boundary(&cClickedBoundary.p);
	if(FAILED(hr) || cClickedBoundary.p == NULL) goto EXIT_FAIL;

	hr = cClickedPickData->get_Thing(&pThing.p);
	if(FAILED(hr)) goto EXIT_FAIL;

	hr = cClickedBoundary->GetCenterExt(&vecCenter.x, &vecCenter.z);
	if(FAILED(hr)) goto EXIT_FAIL;

	hr = cClickedBoundary->GetBoundingBoxExt(&pLU, &pRL);
	if(FAILED(hr)) goto EXIT_FAIL;

	hr = cClickedBoundary->GetVertexXYExt(0, &fX, &fY);
	if(FAILED(hr)) goto EXIT_FAIL;

	hr = pLU->get(&vecTmp1.x, &vecTmp1.y, &vecTmp1.z);
	if(FAILED(hr)) goto EXIT_FAIL;
	
	vecCenter.y = 0.0f;

	hr = pThing->InvokeMethodExt(CComBSTR("GetFrame"), NULL, &varFrame);
	if (FAILED(hr) || varFrame.vt == VT_EMPTY) goto EXIT_FAIL;

	pVWFrame = (IVWFrame*) varFrame.pdispVal;
	SAFEADDREF(pVWFrame);

	pVWFrame->Transform(AXIS_Y, &vecWorldCenter.x, &vecWorldCenter.y, &vecWorldCenter.z, vecCenter.x, vecCenter.y, vecCenter.z);

	m_pViewport->Transform(&vec4d, &vecWorldCenter);
	if(FAILED( hr )) goto EXIT_FAIL;

	m_cptCenter.x = (int) (vec4d.x / vec4d.w);
	m_cptCenter.y = (int) (vec4d.y / vec4d.w);

	vecTmp1.z = vecTmp1.y;
	vecTmp1.y = 0.0f;

	pVWFrame->Transform(AXIS_Y, &vecWorldCenter.x, &vecWorldCenter.y, &vecWorldCenter.z, vecTmp1.x, vecTmp1.y, vecTmp1.z);

	m_pViewport->Transform(&vec4d, &vecWorldCenter);
	if(FAILED( hr )) goto EXIT_FAIL;

	m_pntCenterDiff.x = (long) fabs(vec4d.x - m_cptCenter.x);
	m_pntCenterDiff.y = (long) fabs(vec4d.y - m_cptCenter.y);

	if (pt.x < m_cptCenter.x)
		m_pntCenterDiff.x = -m_pntCenterDiff.x;

	if (pt.y < m_cptCenter.y)
		m_pntCenterDiff.y = -m_pntCenterDiff.y;

	bHandled = TRUE;

	goto EXIT_SUCCEEDED;
EXIT_FAIL:
	m_cptCenter = pt;

	m_pntCenterDiff.x = 1;
	m_pntCenterDiff.y = 1;

EXIT_SUCCEEDED:
	SAFERELEASE(pVWFrame);
	SAFERELEASE(pRL);
	SAFERELEASE(pLU);
	VariantClear(&varHitparams);
	VariantClear(&varHitResult);
	VariantClear(&varFrame);

	return bHandled;
}

//Do initialization work when starting a scale operation
HRESULT CVWScale2DTool::OnClickOrKeyDown()
{
	HRESULT hr = S_OK;
	CScaleObject * pCTrans = NULL;
	D3DRMVECTOR4D vecScreen;
	D3DVECTOR vecWorld;

	SetupTransformList();

	for( POSITION pos = m_TransformList.GetHeadPosition(); pos != NULL; )
	{
		int i, nPoints;

		pCTrans = m_TransformList.GetNext( pos );
		if(NULL != pCTrans)
		{
			pCTrans->fnewX = 1.0F;
			pCTrans->fnewY = 1.0F;

			hr = pCTrans->m_pBoundary->GetCenterExt(&pCTrans->fCenterX, &pCTrans->fCenterY);
			if(FAILED(hr)) goto EXIT_FAIL;

			hr = pCTrans->m_pBoundary->get_VertexCount(&nPoints);
			if(FAILED( hr )) goto EXIT_FAIL;

			pCTrans->nPoints = nPoints;

			if (pCTrans->points)
				delete [] pCTrans->points;

			vecWorld.x = pCTrans->fCenterX;
			vecWorld.y = 0.0f;
			vecWorld.z = pCTrans->fCenterY;

			pCTrans->m_pVWFrame->Transform(AXIS_Y, &vecWorld.x, &vecWorld.y, &vecWorld.z, vecWorld.x, vecWorld.y, vecWorld.z);

			hr = m_pViewport->Transform(&vecScreen, &vecWorld);
			if(FAILED( hr )) goto EXIT_FAIL;

			pCTrans->fCenterX = vecScreen.x;
			pCTrans->fCenterY = vecScreen.y;

			pCTrans->points = new fPoint2D[pCTrans->nPoints];
			for (i = 0; i < pCTrans->nPoints; i++)
			{
				vecWorld.y = 0.0f;

				hr = pCTrans->m_pBoundary->GetVertexXYExt(i, &vecWorld.x, &vecWorld.z );
				if(FAILED( hr )) goto EXIT_FAIL;

				pCTrans->m_pVWFrame->Transform(AXIS_Y, &vecWorld.x, &vecWorld.y, &vecWorld.z, vecWorld.x, vecWorld.y, vecWorld.z);

				hr = m_pViewport->Transform(&vecScreen, &vecWorld);
				if(FAILED( hr )) goto EXIT_FAIL;

				pCTrans->points[i].x = vecScreen.x - pCTrans->fCenterX;
				pCTrans->points[i].y = vecScreen.y - pCTrans->fCenterY;
			}
		}
	}

EXIT_FAIL:
	return TRUE;
}

HRESULT CVWScale2DTool::OnDoneScaling()
{
	HRESULT hr = S_OK;
	CScaleObject * pCTrans = NULL;
	int numScaled = 0, nCount;
	IPropertyList* pBoundaryList = NULL;
	CComBSTR bstr1, bstr2, bstr3;
	CString tmpStr;

	if (!m_TransformList.IsEmpty())
	{
		for( POSITION pos = m_TransformList.GetHeadPosition(); pos != NULL; )
		{
			pCTrans = m_TransformList.GetNext( pos );
			if(NULL != pCTrans)
			{
				CComVariant pvarBoundary;

				//Notify BoundaryList that the boundary has changed
				hr = pCTrans->m_pPickData->get_BoundaryIndex(&nCount);
				if (FAILED(hr)) goto EXIT_FAIL;

				hr = pCTrans->m_pPickData->get_BoundaryList(&pBoundaryList);
				if (FAILED(hr)) goto EXIT_FAIL;

				hr = m_pWorld->CloneProperty( CComVariant(pCTrans->m_pBoundary), &pvarBoundary);
				if (FAILED(hr)) goto EXIT_FAIL;
					
				hr = pBoundaryList->put_Property(nCount, pvarBoundary);
				if (FAILED(hr)) goto EXIT_FAIL;

				tmpStr = "1.0";
				bstr1 = tmpStr;
				bstr2 = tmpStr;
				bstr3 = tmpStr;

				InvokeToolEvent(TOOLEVENT_2DOBJECTSCALED, pCTrans->m_pPickData, bstr1, bstr2, bstr3, VARIANT_TRUE); 

				SAFERELEASE(pBoundaryList);
			}
		}
	}

EXIT_FAIL:

	SAFERELEASE(pBoundaryList);

	return hr;
}


BOOL CVWScale2DTool::OnLButtonUp(UINT nFlags, POINT pt )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_bTrackingMouse)
	{
		m_bTrackingMouse = FALSE;
		ReleaseCapture();

		if (!m_bShowingNoCursor)
			ShowCursor(TRUE);
		else
		{
			m_bShowingNoCursor = FALSE;
			SetCursor(m_pUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_SCALE2D)));
		}

		OnDoneScaling();
	}

	return TRUE;
}


HRESULT CVWScale2DTool::ScaleSelectedObjects( float flDeltaX, float flDeltaY )
{
	HRESULT hr = S_OK;
	CScaleObject * pCTrans = NULL;
	BOOL bHandled = FALSE;
	int i;
	static DWORD dwTime = - 1;
	DWORD dwTimeNow;
	CComBSTR bstr1, bstr2, bstr3;
	CString tmpStr;
	D3DVECTOR vecDelta, vecThingPos;
	BOOL bXAxisLocked, bZAxisLocked;
	POSITION pos ;
	IVector *pVictorVector = NULL;
	IThing* pThing = NULL;

	if (IsPressed('S'))
	{
		flDeltaX *= SLOWKEY_SLOWFACTOR;
		flDeltaY *= SLOWKEY_SLOWFACTOR;
	}

	m_flTotalDeltaX += flDeltaX;
	m_flTotalDeltaY += flDeltaY;

	bXAxisLocked = IsPressed('X') || m_nAxisLock & X_LOCK;
	bZAxisLocked = IsPressed('Z') || m_nAxisLock & Z_LOCK;

	if (IsPressed(VK_MENU))
	{
		m_flTotalDeltaX *= SLOWKEY_SLOWFACTOR;
		m_flTotalDeltaY *= SLOWKEY_SLOWFACTOR;
	}

	if (bXAxisLocked) m_flTotalDeltaX = 0.0f;
	if (bZAxisLocked) m_flTotalDeltaY = 0.0f;

	for( pos = m_TransformList.GetHeadPosition(); pos != NULL; )
	{
		pCTrans = m_TransformList.GetNext( pos );
		if(NULL != pCTrans)
		{
			hr = pCTrans->m_pVWFrame->GetPositionEx(NULL, &vecThingPos.x, &vecThingPos.y, &vecThingPos.z);
			if (FAILED(hr)) goto EXIT_FAIL;

			hr = pCTrans->m_pPickData->get_Thing(&pThing);
			if (FAILED(hr)) goto EXIT_FAIL;

			vecDelta.x = m_flTotalDeltaX + 1.0f;
			vecDelta.y = 0.0f;
			vecDelta.z = m_flTotalDeltaY + 1.0f;

			if (vecDelta.x < 0.01f)
				vecDelta.x = 0.01f;
			if (vecDelta.z < 0.01f)
				vecDelta.z = 0.01f;

			for (i = 0; i < pCTrans->nPoints; i++)
			{
				D3DRMVECTOR4D vecScreen;
				D3DVECTOR vecWorld;

				vecScreen.x = pCTrans->points[i].x * vecDelta.x + pCTrans->fCenterX;
				vecScreen.y = pCTrans->points[i].y * vecDelta.z + pCTrans->fCenterY;
				vecScreen.z = 0.0f;
				vecScreen.w = 1.0f;

				hr = m_pViewport->InverseTransform(&vecWorld, &vecScreen);
				if(FAILED( hr )) goto EXIT_FAIL;

				pCTrans->m_pVWFrame->InverseTransform(AXIS_Y, &vecWorld.x, &vecWorld.y, &vecWorld.z, vecWorld.x, vecWorld.y, vecWorld.z);

				hr = pCTrans->m_pBoundary->SetVertexXY(	i, vecWorld.x, vecWorld.z);
				if(FAILED( hr )) goto EXIT_FAIL;
			}

			InvokeToolEvent(TOOLEVENT_BOUNDARYUPDATE, pCTrans->m_pBoundary, bstrNULL, bstrNULL, bstrNULL, VARIANT_FALSE);

			dwTimeNow = GetTickCount();

			if (dwTimeNow - dwTime > 200)
			{
				tmpStr.Format("%0.3f", vecDelta.x);
				bstr1 = tmpStr;
				tmpStr.Format("%0.3f", vecDelta.z);
				bstr2 = tmpStr;
				bstr3 = tmpStr;

				InvokeToolEvent(TOOLEVENT_2DOBJECTSCALED, pCTrans->m_pPickData, bstr1, bstr2, bstr3, VARIANT_TRUE); 

				dwTime = dwTimeNow;
			}

			SAFERELEASE(pThing);
		}
	}

EXIT_FAIL:
	SAFERELEASE(pThing);

	return hr;
}

BOOL CVWScale2DTool::OnMouseMove(UINT nFlags, POINT pt)
{
	HRESULT hr = S_OK;
	float fDeltaX, fDeltaY;
	RECT rect;
	HWND hWnd;

	// If left button is down, translate all selected translatable objects.

	if( (MK_LBUTTON & nFlags)	)
	{
		if( (pt.x == m_cptLast.x )	&& (pt.y == m_cptLast.y ) )
			goto EXIT;

		fDeltaX = ((float) pt.x - m_cptLast.x) / m_pntCenterDiff.x;
		fDeltaY = ((float) pt.y - m_cptLast.y) / m_pntCenterDiff.y;

		if (IsPressed(VK_SHIFT))
		{
			fDeltaX = fDeltaY = (fDeltaX + fDeltaY) / 2;
		}

		hr = ScaleSelectedObjects((float) (fDeltaX ) * fMouseScaleFactor, (float) (fDeltaY ) * fMouseScaleFactor );
		if (FAILED(hr)) goto EXIT_FAIL;

		hr = m_pUIView->get_HWND(&hWnd);
		if (FAILED(hr)) goto EXIT_FAIL;

		GetClientRect(hWnd, &rect);

		//Reset mouse position if necessary.
		if (pt.x < 0 || pt.x > rect.right || pt.y < 0 || pt.y > rect.bottom)
		{
			m_cptFirst.x = (rect.right / 2) - (pt.x - m_cptFirst.x);
			m_cptFirst.y = (rect.bottom / 2) - (pt.y - m_cptFirst.y);

			pt.x = rect.right / 2;
			pt.y = rect.bottom / 2;
			m_cptLast = pt;
			ClientToScreen(hWnd, &pt);
			SetCursorPos(pt.x, pt.y);
		}
		else
			m_cptLast = pt;
	}

EXIT:
EXIT_FAIL:
	return TRUE;
}

// Temporary transform object:
// Only returns true if there is at least one selected object
// AND all selected objects have an IVWTransform.
STDMETHODIMP CVWScale2DTool::IsValid(IVWUIView * pVw, VARIANT_BOOL * pfValid)
{
	HRESULT hr = S_OK;
	CVWPickDataPtr pickDataPtr;
	BSTR bstrReturnValue = NULL;
	VARIANT_BOOL	fLastItem = VARIANT_TRUE;
	COleVariant varProperty;
	CScaleObject* pSO;

	::VariantInit( &varProperty);

	*pfValid = FALSE;
	
	ASSERT( pfValid );
	if (!pfValid )
	{
		hr = E_POINTER;
		goto EXIT_FAIL;
	}

	DestroyTransformList();

	if(	m_pSelectionList )
	{
		hr = m_pSelectionList->get_IsEmpty( &fLastItem  );
		if( FAILED(hr) )  goto EXIT_FAIL;

		if( VARIANT_FALSE == fLastItem ) 
		{
			hr = m_pSelectionList->FirstItem( &bstrReturnValue, &varProperty, &fLastItem );
			if( FAILED(hr) ) goto EXIT_FAIL;

			while( VARIANT_FALSE == fLastItem ) 
			{
				SAFEFREESTRING(bstrReturnValue);

				pickDataPtr = varProperty;
				if( pickDataPtr != NULL )  
				// At least one selected item has an PickData
				{
					CThingPtr ThingPtr;

					hr = pickDataPtr->get_Thing(&ThingPtr.p);
					if(FAILED(hr) || !ThingPtr.p) goto EXIT_FAIL;

					if (SUCCEEDED(hr = ThingPtr->CheckPropertySecurityExt(bstrBoundaryList, PS_WRITE)))
					{
						*pfValid = TRUE;
						POSITION pos = m_TransformList.AddTail( pSO = new CScaleObject(  pickDataPtr ) );

						SAFERELEASE(m_pWorld);
						hr = pSO->m_pBoundary->get_World(&m_pWorld);
						if(FAILED(hr) || !m_pWorld) 	goto EXIT_FAIL;
					}
					else
					{
						*pfValid = FALSE;
						goto EXIT_FAIL;
					}
				}
				//else
				//{
				//	*pfValid = FALSE;
				//	goto EXIT_FAIL;
				//}

				varProperty.Clear();

				hr = m_pSelectionList->NextItem( &bstrReturnValue, &varProperty, &fLastItem );
    			if( FAILED(hr) ) goto EXIT_FAIL;
			}
		}  // if( VARIANT_FALSE == fLastItem )
	}
	goto EXIT_SUCCEED;

EXIT_FAIL:
	DestroyTransformList();

EXIT_SUCCEED:
	SAFEFREESTRING(bstrReturnValue);
	return hr;
}


STDMETHODIMP CVWScale2DTool::OnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient)
{
	if (CompareElements(&bstrAddSelection.m_str, &bstrEventName))
	{
		SetupTransformList();
	}
	else if (CompareElements(&bstrRemoveSelection.m_str, &bstrEventName))
	{
		SetupTransformList();
	}
	else if (CompareElements(&bstrAddCurrentTool.m_str, &bstrEventName))
	{
		SetupTransformList();
	}
	else if (CompareElements(&bstrClearSelection.m_str, &bstrEventName))
	{
		DestroyTransformList();
	}

	return S_OK;
};

HRESULT CVWScale2DTool::DestroyTransformList()
{
	HRESULT hr = S_OK;
	CScaleObject* pCTrans = NULL;

	if (!m_TransformList.IsEmpty())
	{
		//REVIEW: This is the only way to notify the server that an Boundary has changed.
		for( POSITION pos = m_TransformList.GetHeadPosition(); pos != NULL; )
		{
			pCTrans = m_TransformList.GetNext( pos );
			if(NULL != pCTrans)
				delete pCTrans;
		}
	}

	m_TransformList.RemoveAll();

	return hr;
}

float CVWScale2DTool::ElapsedFrameTime()
{
	static DWORD dwTimeThisFrame = 0;
	static DWORD dwTimeLastFrame;

	// update the frame times
	dwTimeLastFrame = dwTimeThisFrame;
	dwTimeThisFrame = GetTickCount();

	DWORD dwElapsedTime = dwTimeThisFrame - dwTimeLastFrame;
	
	// make sure the elapsed time is always sensible
	CLAMP(dwElapsedTime, 0, dwMaxTimeDelta);

	return float(dwElapsedTime) * 0.001f;	// in seconds...
}


STDMETHODIMP CVWScale2DTool::Update()
{
	HRESULT hr = S_OK;
	int	nHorzMotion, nVertMotion;
	float fTime;

	fTime = ElapsedFrameTime();

	if (!m_bGotFocus) goto EXIT_FAIL;

	nHorzMotion = IsPressed(VK_RIGHT) - IsPressed(VK_LEFT);
	nVertMotion = IsPressed(VK_DOWN) - IsPressed(VK_UP);

	if (m_bUsingKeyboard && (nHorzMotion || nVertMotion))
	{
		float flDeltaX = (float) -nVertMotion;
		float flDeltaY = (float) nHorzMotion;
		float flDeltaZ = 0.0;
		float fTmpX, fTmpY, fTmpZ;

		hr = m_pUIView->DeviceToWorld(&flDeltaX, &flDeltaY, &flDeltaZ);
		if(FAILED(hr)) goto EXIT_FAIL;

		fTmpX = fTmpY = fTmpZ = 0.0f;

		hr = m_pUIView->DeviceToWorld( &fTmpX, &fTmpY, &fTmpZ);
		if(FAILED(hr)) goto EXIT_FAIL;

		hr = ScaleSelectedObjects((flDeltaX - fTmpX)*fTime*fObjectNavScale, (flDeltaZ - fTmpZ)*fTime*fObjectNavScale);
		if (FAILED(hr))	goto EXIT_FAIL;
	}

EXIT_FAIL:

	if (SUCCEEDED(hr))
		return TRUE;
	else
		return FALSE;
}

BOOL CVWScale2DTool::OnMessage( UINT nIDEvent, WPARAM wParam, LPARAM lParam )
{
	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if ((nIDEvent == WM_KILLFOCUS || nIDEvent == WM_ENTERMENULOOP) && m_pUIView && m_bTrackingMouse)
	{
		m_bTrackingMouse = FALSE;
		ReleaseCapture();

		if (!m_bShowingNoCursor)
			ShowCursor(TRUE);
		else
		{
			m_bShowingNoCursor = FALSE;
			SetCursor(m_pUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_SCALE2D)));
		}

		OnDoneScaling();
	}

	if (nIDEvent == WM_KILLFOCUS)
		m_bGotFocus = VARIANT_FALSE;
	else if (nIDEvent == WM_SETFOCUS)
		m_bGotFocus = VARIANT_TRUE;

	return bHandled;
}

BOOL CVWScale2DTool::OnKeyDown( UINT nVirtKey, LPARAM lKeyData )
{
	HRESULT hr = S_OK;

	if (nVirtKey == VK_RIGHT || nVirtKey == VK_LEFT || nVirtKey == VK_DOWN ||  nVirtKey == VK_UP)
	{
		if (!m_bUsingKeyboard)
		{
			OnClickOrKeyDown();
			m_flTotalDeltaX = 0.0f;
			m_flTotalDeltaY = 0.0f;
		}
		m_bUsingKeyboard = TRUE;
	}

	return TRUE;
}

BOOL CVWScale2DTool::OnKeyUp( UINT nVirtKey, LPARAM lKeyData )
{
	if (m_bUsingKeyboard && (nVirtKey == VK_RIGHT || nVirtKey == VK_LEFT || nVirtKey == VK_DOWN ||  nVirtKey == VK_UP) )
	{
		OnDoneScaling();
		m_bUsingKeyboard = FALSE;
	}

	return TRUE;
}

HRESULT CVWScale2DTool::InvokeToolEvent(int nNum, IObjectProperty* pOP, BSTR bstr1, BSTR bstr2, BSTR bstr3, VARIANT_BOOL bBubble)
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

void GetRotationFromVector(float* fRot, D3DVECTOR* pvec)
{
	D3DVECTOR rlvPreDef, rlvDir;
	D3DVALUE  valCosTheta;

	// The pre-defined angle.
	rlvPreDef.x = 0.0F;
	rlvPreDef.y	= 0.0F;
	rlvPreDef.z	= 1.0F;

	// Make a D3DVECTOR from this vector.
	rlvDir.x = pvec->x;
	rlvDir.y = pvec->y;
	rlvDir.z = pvec->z;

	D3DRMVectorNormalize(&rlvDir);

	// First need angle between the pre-defined angle and rlvDir.
	valCosTheta = D3DRMVectorDotProduct(&rlvPreDef, &rlvDir);
	*fRot = (D3DVALUE)acos(valCosTheta);

	// This will always be the acute angle. Since rotation will always be in positive direction we must
	// give correct angle (possibly obtuse) for that direction.
	if (rlvDir.x < 0.0F)
	{
		// Acute angle will not work, need obtuse angle.
		*fRot = (float) (2.0f * PI) - *fRot;
	}
	return;

}

void RotateVector(float fRot, D3DVECTOR* pvec)
{
	D3DVECTOR	rlvThis, rlvRotated, rlvAxis;
	D3DVALUE	valModulus;

	rlvThis.x = pvec->x;
	rlvThis.y = pvec->y;
	rlvThis.z = pvec->z;
	rlvAxis.x = 0.0F;
	rlvAxis.y = 1.0F;
	rlvAxis.z = 0.0F;

	valModulus = D3DRMVectorModulus(&rlvThis);
	D3DRMVectorRotate(&rlvRotated, &rlvThis, &rlvAxis, fRot);

	// D3DRMVectorRotate will return a unit vector. We need the original size vector.
	D3DRMVectorScale(&rlvRotated, &rlvRotated, valModulus);

	pvec->x = rlvRotated.x;
	pvec->y = rlvRotated.y;
	pvec->z = rlvRotated.z;
	
	return;
}