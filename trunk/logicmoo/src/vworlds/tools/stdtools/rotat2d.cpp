// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// Rotat2D.cpp : Implementation of CRotate2DTool

#include "stdafx.h"
#include <d3drm.h>
#include <stdtools.h>
#include "Rotat2D.h"
#include "GeomUndo.h"
#include "UndoUtil.h"
#include <vwrnedit.h>

/////////////////////////////////////////////////////////////////////////////
typedef CVWComPtr<IVWTransform,NULL,&IID_IVWTransform> CVWTransformPtr;
typedef CVWComPtr<IBoundary,NULL,&IID_IBoundary> CBoundaryPtr;
typedef CVWComPtr<IVWExecuteUndo, &CLSID_VWExecuteGeomUndo, &IID_IVWExecuteUndo> CExecUndoPtr;
typedef CVWComPtr<IVWPickData,NULL,&IID_IVWPickData> CVWPickDataPtr;
typedef CVWComPtr<IThing,NULL,&IID_IThing> CThingPtr;
static CComBSTR bstrBoundaryList("BoundaryList");
static CComBSTR bstrGetFrame ("GetFrame");
extern CComBSTR bstrNavigate;
extern CComBSTR bstrXAxisLocked;
extern CComBSTR bstrYAxisLocked;
extern CComBSTR bstrZAxisLocked;
extern CComBSTR bstrAddSelection;
extern CComBSTR bstrRemoveSelection;
extern CComBSTR bstrClearSelection;
extern CComBSTR bstrAddCurrentTool;

extern void RotateVector(float fRot, D3DVECTOR* pvec);

#define CLAMP(x, xmin, xmax)	if ((x) < (xmin)) (x) = (xmin); else if ((x) > (xmax)) (x) = (xmax)
#define dispidToolEvent 19L
#define dwMaxTimeDelta			250				// maximum of 250ms (4fps) of dead reckoning 
#define PI  3.141592
#define X_LOCK 1
#define Y_LOCK 2
#define Z_LOCK 4
#define IsPressed(key)			(GetAsyncKeyState(key) < 0)
#define SLOWKEY_SLOWFACTOR		0.2f
const IID DIID__RenderingControl = { 0x44fac781, 0xca4, 0x11d0, { 0x89, 0xa9, 0, 0xa0, 0xc9, 0x5, 0x41, 0x29 } };
static CComBSTR bstrNULL("NULL");

/////////////////////////////////////////////////////////////////////////////
// CRotate2DTool

CRotateObject::CRotateObject( IVWPickData* pPickData )
{
	HRESULT hr = S_OK;
	IThing* pThing = NULL;
	VARIANT var;
	int i;

	VariantInit(&var);

	float fTmp;

	ASSERT( pPickData );

	m_pPickData = pPickData;
	SAFEADDREF(m_pPickData);

	pPickData->get_Boundary(&m_pBoundary);

	m_pPickData->get_Thing(&pThing);

	hr = pThing->InvokeMethodExt(bstrGetFrame, NULL, &var);
	if (FAILED(hr) || var.vt == VT_EMPTY) goto EXIT_FAIL;

	m_pVWFrame = (IVWFrame*) var.pdispVal;
	SAFEADDREF(m_pVWFrame);

	hr = m_pBoundary->GetCenterExt(&m_fCenter.x, &m_fCenter.y);
	if(FAILED(hr)) goto EXIT_FAIL;

	hr = m_pBoundary->get_VertexCount(&m_nPoints);
	if(FAILED( hr )) goto EXIT_FAIL;

	hr = m_pVWFrame->Transform(AXIS_Y, &m_fCenter.x, &fTmp, &m_fCenter.y, m_fCenter.x, 0.0f, m_fCenter.y);
	if(FAILED( hr )) goto EXIT_FAIL;

	m_pPoints = new fPoint2D[m_nPoints];
	for (i = 0; i < m_nPoints; i++)
	{
		hr = m_pBoundary->GetVertexXYExt(i, &m_pPoints[i].x, &m_pPoints[i].y );
		if(FAILED( hr )) goto EXIT_FAIL;

		hr = m_pVWFrame->Transform(AXIS_Y, &m_pPoints[i].x, &fTmp, &m_pPoints[i].y, m_pPoints[i].x, 0.0f, m_pPoints[i].y);
		if(FAILED( hr )) goto EXIT_FAIL;

		m_pPoints[i].x -= m_fCenter.x;
		m_pPoints[i].y -= m_fCenter.y;
	}

	goto EXIT_SUCCESSFUL;

EXIT_FAIL:
	ASSERT(0);

EXIT_SUCCESSFUL:
	VariantClear(&var);
	SAFERELEASE(pThing);
	return;
}

CRotate2DTool::CRotate2DTool()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_piEditorViewControl = NULL;
	m_bGotFocus = VARIANT_FALSE;
	m_bTrackingMouse = FALSE;
	m_bShowingNoCursor = FALSE;

	SetCursor(m_pUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_ROTATE2D)));


	return;
}


CRotate2DTool::~CRotate2DTool( )
{	
	SAFERELEASE(m_piEditorViewControl);

	DestroyTransformList();
}
// IVWTool Methods


STDMETHODIMP CRotate2DTool::Initialize(IVWUIView* pVw, IWorld* pWorld, IVWRenderRoot *pVWRenderRoot)
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

//	SAFERELEASE(m_pVWRenderRoot);
//	m_pVWRenderRoot = pVWRenderRoot;
//	SAFEADDREF(pVWRenderRoot);

	hr = pVWRenderRoot->get_VWRender3D(&pRender3D);
	if (FAILED(hr) || (pRender3D == NULL)) goto EXIT_FAIL;

	hr = m_pWorld->get_Tool(bstrNavigate, (IUnknown**) &pNavTool);
	if (FAILED(hr) || (pNavTool == NULL)) goto EXIT_FAIL;

	m_bUsingKeyboard = FALSE;

	for (i = 0; i < kbMax; i++)
	{
		hr = pNavTool->get_KeyBinding((KeyConstant) i, &var);
		if (FAILED(hr)) goto EXIT_FAIL;

		m_KeyBindings[i] = var.lVal;
	}

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

HRESULT CRotate2DTool::SetupEnvironment()
{
	HRESULT hr = S_OK;
	IThing* pGlobal = NULL;
	VARIANT_BOOL vbLock;

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

EXIT_FAIL:
	SAFERELEASE(pGlobal);

	return hr;
}

HRESULT CRotate2DTool::SetupTransformList()
{
	HRESULT hr = S_OK;
	VARIANT_BOOL fValid = VARIANT_FALSE;

	SetupEnvironment();

	hr = IsValid(m_pUIView, &fValid);
	if (SUCCEEDED(hr) && !fValid)
		hr = E_FAIL;
	if(FAILED(hr)) goto EXIT_FAIL;

EXIT_FAIL:

	return hr;
}


BOOL CRotate2DTool::OnLButtonDown(UINT nFlags, POINT pt )
{
	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;
	CRotateObject * pCTrans = NULL;
	CComBSTR bstr1, bstr2, bstr3;
	CString tmpStr;
	HWND hWnd;

	if(m_pUIView && !m_bTrackingMouse)
	{ 
		m_fDeltaTotal = 0.0f;
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

		m_cptLast = pt;

		hr = m_pUIView->get_HWND(&hWnd);
		if (FAILED(hr)) goto EXIT_FAIL;

		SetCapture(hWnd);

		for( POSITION pos = m_TransformList.GetHeadPosition(); pos != NULL; )
		{
			pCTrans = m_TransformList.GetNext( pos );
			tmpStr = "0.0";
			bstr1 = tmpStr;
			tmpStr = "";
			bstr2 = tmpStr;
			bstr3 = tmpStr;

			InvokeToolEvent(TOOLEVENT_2DOBJECTROTATED, pCTrans->m_pPickData, bstr1, bstr2, bstr3, VARIANT_TRUE); 
		}
	}

	bHandled = TRUE;
	goto EXIT_SUCCEEDED;
EXIT_FAIL:

EXIT_SUCCEEDED:
	return bHandled;
}

HRESULT CRotate2DTool::InvokeToolEvent(int nNum, IObjectProperty* pOP, BSTR bstr1, BSTR bstr2, BSTR bstr3, VARIANT_BOOL bBubble)
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

BOOL CRotate2DTool::OnLButtonUp(UINT nFlags, POINT pt )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_bTrackingMouse)
	{
		OnDoneRotating();
		m_bTrackingMouse = FALSE;
		ReleaseCapture();

		if (!m_bShowingNoCursor )
			ShowCursor(TRUE);
		else
		{
			SetCursor(m_pUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_ROTATE2D)));
			m_bShowingNoCursor = FALSE;
		}
	}

	return TRUE;
}

HRESULT CRotate2DTool::OnDoneRotating()
{
	BOOL bHandled = FALSE; 
	IVWUndoItem		*pVWUndoItem = NULL;
	HRESULT hr = S_OK;
	CRotateObject * pCTrans = NULL;
	int numRotated = 0, nCount;
	IPropertyList* pBoundaryList = NULL;
	CComBSTR bstr1, bstr2, bstr3;
	CString tmpStr;

	if (!m_TransformList.IsEmpty())
	{
		for( POSITION pos = m_TransformList.GetHeadPosition(); pos != NULL; )
		{
			bHandled = TRUE;
			pCTrans = m_TransformList.GetNext( pos );
			if(NULL != pCTrans)
			{
				CComVariant pvarBoundary;

				//Notify BoundaryList that the boundary has changed
				hr = pCTrans->m_pPickData->get_BoundaryIndex(&nCount);
				if (FAILED(hr)) goto FAIL_EXIT;

				hr = pCTrans->m_pPickData->get_BoundaryList(&pBoundaryList);
				if (FAILED(hr)) goto FAIL_EXIT;

				hr = m_pWorld->CloneProperty( CComVariant(pCTrans->m_pBoundary), &pvarBoundary);
				if (FAILED(hr)) goto FAIL_EXIT;
					
				hr = pBoundaryList->put_Property(nCount, pvarBoundary);
				if (FAILED(hr)) goto FAIL_EXIT;

				tmpStr = "0.0";
				bstr1 = tmpStr;
				tmpStr = "";
				bstr2 = tmpStr;
				bstr3 = tmpStr;

				InvokeToolEvent(TOOLEVENT_2DOBJECTROTATED, pCTrans->m_pPickData, bstr1, bstr2, bstr3, VARIANT_TRUE); 

				SAFERELEASE(pBoundaryList);
			}
		}
		bHandled = TRUE;
	}

FAIL_EXIT:
	SAFERELEASE(pBoundaryList);

	return bHandled;
}


HRESULT CRotate2DTool::RotateSelectedObjects( float flDeltaX )
{
	HRESULT hr = S_OK;
	CRotateObject * pCTrans = NULL;
	BOOL bHandled = FALSE;
	static DWORD dwTime = - 1;
	DWORD dwTimeNow;
	CComBSTR bstr1, bstr2, bstr3;
	CString tmpStr;
	D3DVECTOR vec;
	int i;

	if (IsPressed('S'))
	{
		flDeltaX *= SLOWKEY_SLOWFACTOR;
	}

	m_fDeltaTotal += flDeltaX;

	for( POSITION pos = m_TransformList.GetHeadPosition(); pos != NULL; )
	{
		bHandled = TRUE;
		pCTrans = m_TransformList.GetNext( pos );
		if(NULL != pCTrans)
		{
			for (i = 0; i < pCTrans->m_nPoints; i++)
			{
				vec.x = pCTrans->m_pPoints[i].x;
				vec.y = 0.0f;
				vec.z = pCTrans->m_pPoints[i].y;

				RotateVector(m_fDeltaTotal, &vec);

				vec.x += pCTrans->m_fCenter.x;
				vec.z += pCTrans->m_fCenter.y;

				hr = pCTrans->m_pVWFrame->InverseTransform(AXIS_Y, &vec.x, &vec.y, &vec.z, vec.x, vec.y, vec.z);
				if(FAILED( hr )) goto EXIT_FAIL;

				hr = pCTrans->m_pBoundary->SetVertexXY(i, vec.x, vec.z);
				if(FAILED( hr )) goto EXIT_FAIL;
			}

			InvokeToolEvent(TOOLEVENT_BOUNDARYUPDATE, pCTrans->m_pBoundary, bstrNULL, bstrNULL, bstrNULL, VARIANT_FALSE);

			dwTimeNow = GetTickCount();

			if (dwTimeNow - dwTime > 200)
			{
				tmpStr.Format("%0.3f", m_fDeltaTotal / PI * 180.0f);
				bstr1 = tmpStr;
				tmpStr = "";
				bstr2 = tmpStr;
				bstr3 = tmpStr;

				InvokeToolEvent(TOOLEVENT_2DOBJECTROTATED, pCTrans->m_pPickData, bstr1, bstr2, bstr3, VARIANT_TRUE); 

				dwTime = dwTimeNow;
			}
		}
	}

EXIT_FAIL:
	return hr;
}

// TODO: Get scaling factor from StudioControl.
BOOL CRotate2DTool::OnMouseMove(UINT nFlags, POINT pt)
{
	HRESULT hr = S_OK;
	float flUpX, flUpY;
	float flDeltaX = 0.0, flDeltaY = 0.0;
	RECT rect;
	HWND hWnd;

	// If left button is down, translate all selected translatable objects.
	if( (MK_LBUTTON & nFlags) )
	{
		CPoint cptThis = pt;

		// If no delta then leave.
		if( (cptThis.x == m_cptLast.x )	&& (cptThis.y == m_cptLast.y ) )
			goto EXIT_FAIL;

		hr = m_pUIView->get_HWND(&hWnd);
		if (FAILED(hr)) goto EXIT_FAIL;

		GetClientRect(hWnd, &rect);

		//Reset mouse position if necessary.
		if (pt.x < 0 || pt.x > rect.right || pt.y < 0 || pt.y > rect.bottom)
		{
			pt.x = rect.right / 2;
			pt.y = rect.bottom / 2;
			m_cptLast = pt;
			ClientToScreen(hWnd, &pt);
			SetCursorPos(pt.x, pt.y);
		}

		flDeltaX = (float)(cptThis.x - m_cptLast.x) / 30.0f;
		flDeltaY = (float)(cptThis.y - m_cptLast.y);

		m_cptLast = pt;

		flUpX = (float) m_cptFirst.x;
		flUpY = (float) m_cptFirst.y;

		RotateSelectedObjects( flDeltaX );
	}

EXIT_FAIL:
	return TRUE;
}

// Temporary transform object:
// Only returns true if there is at least one selected object
// AND all selected objects have an IVWTransform.
STDMETHODIMP CRotate2DTool::IsValid(IVWUIView * pVw, VARIANT_BOOL * pfValid)
{
	HRESULT hr = S_OK;
	CVWPickDataPtr pickDataPtr;
	BSTR bstrReturnValue = NULL;
	VARIANT_BOOL	fLastItem = VARIANT_TRUE;
	COleVariant varProperty;
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
		if( VARIANT_FALSE == fLastItem ) {
			hr = m_pSelectionList->FirstItem( &bstrReturnValue, &varProperty, &fLastItem );
			if( FAILED(hr) ) goto EXIT_FAIL;
			while( VARIANT_FALSE == fLastItem ) 
			{
				SAFEFREESTRING(bstrReturnValue);

				pickDataPtr = varProperty;
				if( pickDataPtr != NULL )  
				// At least one selected item has an IVWPickData
				{
					CThingPtr ThingPtr;

					hr = pickDataPtr->get_Thing(&ThingPtr.p);
					if(FAILED(hr) || !ThingPtr.p) goto EXIT_FAIL;

					if (SUCCEEDED(hr = ThingPtr->CheckPropertySecurityExt(bstrBoundaryList, PS_WRITE)))
					{
						*pfValid = TRUE;
						POSITION pos = m_TransformList.AddTail( new CRotateObject(  pickDataPtr ) );
					}
					else
					{
						*pfValid = FALSE;
						goto EXIT_FAIL;
					}
				}
				//else
				// Found one without IVWTransform.
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


float CRotate2DTool::ElapsedFrameTime()
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

STDMETHODIMP CRotate2DTool::OnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient)
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

BOOL CRotate2DTool::OnMessage( UINT nIDEvent, WPARAM wParam, LPARAM lParam )
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
			SetCursor(m_pUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_ROTATE2D)));
		}

		OnDoneRotating();
	}

	if (nIDEvent == WM_KILLFOCUS)
		m_bGotFocus = VARIANT_FALSE;
	else if (nIDEvent == WM_SETFOCUS)
		m_bGotFocus = VARIANT_TRUE;

	return bHandled;
}

BOOL CRotate2DTool::OnKeyDown( UINT nVirtKey, LPARAM lKeyData )
{
	HRESULT hr = S_OK;

	if (nVirtKey == VK_RIGHT || nVirtKey == VK_LEFT || nVirtKey == VK_DOWN ||  nVirtKey == VK_UP)
	{
		if (!m_bUsingKeyboard)
		{
			CString tmpStr;
			CComBSTR bstr1, bstr2, bstr3;
			CRotateObject * pCTrans = NULL;

			SetupTransformList();
			m_fDeltaTotal = 0.0f;
			m_bUsingKeyboard = TRUE;
	
			for( POSITION pos = m_TransformList.GetHeadPosition(); pos != NULL; )
			{
				pCTrans = m_TransformList.GetNext( pos );
				tmpStr = "0.0";
				bstr1 = tmpStr;
				tmpStr = "";
				bstr2 = tmpStr;
				bstr3 = tmpStr;

				InvokeToolEvent(TOOLEVENT_2DOBJECTROTATED, pCTrans->m_pPickData, bstr1, bstr2, bstr3, VARIANT_TRUE); 
			}
		}
	}

	return TRUE;
}

BOOL CRotate2DTool::OnKeyUp( UINT nVirtKey, LPARAM lKeyData )
{
	if (m_bUsingKeyboard && (nVirtKey == VK_RIGHT || nVirtKey == VK_LEFT || nVirtKey == VK_DOWN ||  nVirtKey == VK_UP) )
	{
		OnDoneRotating();
		m_bUsingKeyboard = FALSE;
	}

	return TRUE;
}


STDMETHODIMP CRotate2DTool::Update()
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
		hr = RotateSelectedObjects(nHorzMotion*fTime);
		if (FAILED(hr))	goto EXIT_FAIL;
	}

EXIT_FAIL:

	if (SUCCEEDED(hr))
		return TRUE;
	else
		return FALSE;
}

HRESULT CRotate2DTool::DestroyTransformList()
{
	HRESULT hr = S_OK;
	CRotateObject* pCTrans = NULL;

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
