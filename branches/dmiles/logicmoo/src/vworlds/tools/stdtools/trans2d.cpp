// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// Trans2D.cpp : Implementation of CTools1App and DLL registration.
// Temporarlily used as TranslateTool

#include "stdafx.h"
#include <stdtools.h>
#include "trans2d.h"
#include "GeomUndo.h"
#include "UndoUtil.h"
#include <vwrnedit.h>

typedef CVWComPtr<IVWTransform,NULL,&IID_IVWTransform> CVWTransformPtr;
typedef CVWComPtr<IBoundary,NULL,&IID_IBoundary> CBoundaryPtr;
typedef CVWComPtr<IPropertyList,NULL,&IID_IPropertyList> CBoundaryListPtr;
typedef CVWComPtr<IThing,NULL,&IID_IThing> CThingPtr;
typedef CVWComPtr<IVWExecuteUndo, &CLSID_VWExecuteGeomUndo, &IID_IVWExecuteUndo> CExecUndoPtr;
typedef CVWComPtr<IVWPickData,NULL,&IID_IVWPickData> CVWPickDataPtr;
/////////////////////////////////////////////////////////////////////////////
#define dispidToolEvent 19L
#define X_LOCK 1
#define Y_LOCK 2
#define Z_LOCK 4
#define IsPressed(key)			(GetAsyncKeyState(key) < 0)
#define SLOWKEY_SLOWFACTOR		0.2f

#define dispidCenterViewOnObject 18L
#define dispidToolEvent 19L
#define dispidResetCameraPosition 12L
#define CLAMP(x, xmin, xmax)	if ((x) < (xmin)) (x) = (xmin); else if ((x) > (xmax)) (x) = (xmax)
#define dwMaxTimeDelta			250				// maximum of 250ms (4fps) of dead reckoning 
#define fObjectNavScale			24.0f			// General speed of object navigation


const IID DIID__RenderingControl = { 0x44fac781, 0xca4, 0x11d0, { 0x89, 0xa9, 0, 0xa0, 0xc9, 0x5, 0x41, 0x29 } };
static CComBSTR bstrNULL("NULL");

/////////////////////////////////////////////////////////////////////////////
extern CComBSTR bstrNavigate;
extern CComBSTR bstrXAxisLocked;
extern CComBSTR bstrYAxisLocked;
extern CComBSTR bstrZAxisLocked;
extern CComBSTR bstrGravity;
extern CComBSTR bstrAddSelection;
extern CComBSTR bstrRemoveSelection;
extern CComBSTR bstrClearSelection;
extern CComBSTR bstrAddCurrentTool;

CComBSTR bstrBoundaryList("BoundaryList");

/*********************************************************************/
CTranslate2DObject::CTranslate2DObject( IVWPickData* pPickData )
{
	IBoundary* pBoundary = NULL;
	IThing* pThing = NULL;
	VARIANT var;
	HRESULT hr = S_OK;
	VARIANT_BOOL vbVertMode;

	m_pBoundary = NULL;
	m_pVWFrame = NULL;

	VariantInit(&var);

	ASSERT( pPickData );

	m_pPickData = pPickData;
	SAFEADDREF(m_pPickData);

	hr = pPickData->get_Boundary(&pBoundary);
	if (FAILED(hr)) goto EXIT_FAIL;

	hr = pPickData->IsAnyVertexPicked(&vbVertMode);
	if (FAILED(hr)) goto EXIT_FAIL;

	bVerticeSelected = (vbVertMode == VARIANT_TRUE ? TRUE : FALSE);

	ASSERT( pBoundary );

	m_pBoundary = pBoundary;
	SAFEADDREF(m_pBoundary);

	hr = m_pBoundary->GetVertexXYExt(0, &fOldX, &fOldY);
	if(FAILED( hr )) goto EXIT_FAIL;

	hr = pPickData->get_Thing(&pThing);
	if (FAILED(hr)) goto EXIT_FAIL;

	hr = pThing->InvokeMethodExt(CComBSTR("GetFrame"), NULL, &var);
	if (FAILED(hr) || var.vt == VT_EMPTY) goto EXIT_FAIL;

	m_pVWFrame = (IVWFrame*) var.pdispVal;
	SAFEADDREF(m_pVWFrame);

EXIT_FAIL:
	VariantClear(&var);
	SAFERELEASE(pBoundary);
	SAFERELEASE(pThing);
}

CTranslate2DObject::~CTranslate2DObject()
{
	SAFERELEASE(m_pPickData);
	SAFERELEASE(m_pBoundary);
	SAFERELEASE(m_pVWFrame);
}

/*********************************************************************/
CVWTranslate2DToolObject::CVWTranslate2DToolObject()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_piEditorViewControl = NULL;
	m_nAxisLock = 0;
	m_pWorld = NULL;
	m_bUsingKeyboard = FALSE;
	m_bGotFocus = VARIANT_FALSE;
	m_bTrackingMouse = FALSE;
	m_bShowingNoCursor = FALSE;

	SetCursor(NULL, LoadCursor(NULL, IDC_ARROW ));

	return;
}

CVWTranslate2DToolObject::~CVWTranslate2DToolObject( )
{	
	DestroyTransformList();

	SAFERELEASE(m_piEditorViewControl);
	SAFERELEASE(m_pWorld);
}

STDMETHODIMP CVWTranslate2DToolObject::Initialize(IVWUIView* pVw, IWorld* pWorld, IVWRenderRoot *pVWRenderRoot)
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

HRESULT CVWTranslate2DToolObject::SetupEnvironment()
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

HRESULT CVWTranslate2DToolObject::SetupTransformList()
{
	HRESULT hr = S_OK;
	VARIANT_BOOL fValid = VARIANT_FALSE;

	SetupEnvironment();

	hr = IsValid(m_pUIView, &fValid);
	if (SUCCEEDED(hr) && !fValid)
		hr = E_FAIL;
	if(FAILED(hr)) goto EXIT_FAIL;

	m_flTotalDeltaX = 0.0f;
	m_flTotalDeltaZ = 0.0f;

EXIT_FAIL:

	return hr;
}

// IVWTool Methods

BOOL CVWTranslate2DToolObject::OnLButtonDown( UINT nFlags, POINT pt )
{
	HRESULT hr = S_OK;
	VARIANT_BOOL fValid = VARIANT_FALSE;
	BOOL bHandled = FALSE;
	HWND hWnd;

	if(!m_pUIView)
	{ 
		hr = VWSTUDIO_E_BADCONTROLINITIALIZATION;
		goto EXIT_FAIL;
	}

	if (!m_bTrackingMouse)
	{
		m_bTrackingMouse = TRUE;

		m_flFirstX = (float) (m_nLastX = pt.x);
		m_flFirstY = (float) (m_nLastY = pt.y);
		m_flFirstZ = 0.0;

		//Initialize start and end points

		hr = m_pUIView->DeviceToWorld( &m_flFirstX, &m_flFirstY, &m_flFirstZ);
		if(FAILED(hr)) goto EXIT_FAIL;

		m_flLastX = m_flFirstX;
		m_flLastY = m_flFirstY;
		m_flLastZ = m_flFirstZ;

		hr = SetupTransformList();
		if (FAILED(hr))
		{
			SetCursor(m_pUIView, LoadCursor(NULL, IDC_NO ));
			m_bShowingNoCursor = TRUE;
			hr = S_OK;
		}
		else
			ShowCursor(FALSE);

		hr = m_pUIView->get_HWND(&hWnd);
		if (FAILED(hr)) goto EXIT_FAIL;

		SetCapture(hWnd);


		bHandled = TRUE;
		goto EXIT_SUCCEEDED;
	}
EXIT_FAIL:

EXIT_SUCCEEDED:

	return bHandled;
}

HRESULT CVWTranslate2DToolObject::OnDoneTranslating()
{
	HRESULT			hr = S_OK;
	CTranslate2DObject* pCTrans = NULL;
	IVWUndoItem		*pVWUndoItem = NULL;
	IPropertyList	*pBoundaryList = NULL;
	int numTranslated = 0;
	int i, nCount;
	CString tmpStr;
	CComBSTR bstr1, bstr2, bstr3;

	if (!m_TransformList.IsEmpty())
	{
		//REVIEW: This is the only way to notify the server that an Boundary has changed.
		for( POSITION pos = m_TransformList.GetHeadPosition(); pos != NULL; )
		{
			pCTrans = m_TransformList.GetNext( pos );
			if(NULL != pCTrans)
			{
				CComVariant pvarBoundary;

				//Notify BoundaryList that the boundary has changed
				hr = pCTrans->m_pPickData->get_BoundaryIndex(&nCount);
				if (FAILED(hr)) goto ERROR_DONE;

				hr = pCTrans->m_pPickData->get_BoundaryList(&pBoundaryList);
				if (FAILED(hr)) goto ERROR_DONE;

				//Clone the property so put will work
				hr = m_pWorld->CloneProperty( CComVariant(pCTrans->m_pBoundary), &pvarBoundary);
				if (FAILED(hr)) goto ERROR_DONE;

				hr = pBoundaryList->put_Property(nCount, pvarBoundary);
				if (FAILED(hr)) goto ERROR_DONE;

				tmpStr = "0.0";
				bstr1 = tmpStr;
				bstr3 = tmpStr;
				tmpStr = "";
				bstr2 = tmpStr;

				InvokeToolEvent(TOOLEVENT_2DOBJECTTRANSLATED, pCTrans->m_pPickData, bstr1, bstr2, bstr3, VARIANT_TRUE); 

				SAFERELEASE(pBoundaryList);

				i = 0;
			}
		}
	}

ERROR_DONE:

	SAFERELEASE(pBoundaryList);

	return hr;
}

BOOL CVWTranslate2DToolObject::OnLButtonUp(UINT nFlags, POINT pt )
{
	if (m_bTrackingMouse)
	{
		m_bTrackingMouse = FALSE;
		OnDoneTranslating();
		ReleaseCapture();
		if (!m_bShowingNoCursor)
			ShowCursor(TRUE);
		else
		{
			m_bShowingNoCursor = FALSE;
			SetCursor(m_pUIView, LoadCursor(NULL, IDC_ARROW ));
		}
	}

	return TRUE;
}

BOOL CVWTranslate2DToolObject::OnMouseMove(UINT nFlags, POINT pt )
{
	float fTmpX = 0.0f, fTmpY = 0.0f, fTmpZ = 0.0f;
	HRESULT hr = S_OK;
	RECT rect;
	HWND hWnd;

	// If left button is down, translate all selected translatable objects.
	if( MK_LBUTTON & nFlags && (pt.x != m_nLastX || pt.y != m_nLastY) )
	{
		float flDeltaX, flDeltaY, flDeltaZ;

		if (m_nAxisLock & X_LOCK)
			flDeltaX = (float) m_nLastX;
		else
			flDeltaX = (float) pt.x;

		if (m_nAxisLock & Z_LOCK)
			flDeltaY = (float) m_nLastY;
		else
			flDeltaY = (float) pt.y;

		flDeltaZ = 0.0f;

		hr = m_pUIView->DeviceToWorld( &flDeltaX, &flDeltaY, &flDeltaZ);
		if(FAILED(hr)) goto EXIT_FAIL;

		fTmpX = flDeltaX;
		fTmpY = flDeltaY;
		fTmpZ = flDeltaZ;

		flDeltaX -= m_flLastX;
		flDeltaY -= m_flLastY;
		flDeltaZ -= m_flLastZ;

		m_flLastX = fTmpX;
		m_flLastY = fTmpY;
		m_flLastZ = fTmpZ;

		TranslateSelectedObjects( flDeltaX, flDeltaZ );

		hr = m_pUIView->get_HWND(&hWnd);
		if (FAILED(hr)) goto EXIT_FAIL;

		GetClientRect(hWnd, &rect);

		//Reset mouse position if necessary.
		if (pt.x < 0 || pt.x > rect.right || pt.y < 0 || pt.y > rect.bottom)
		{
			m_flLastX = (float) (pt.x = rect.right / 2);
			m_flLastY = (float) (pt.y = rect.bottom / 2);
			m_flLastZ = 0.0f;
			ClientToScreen(hWnd, &pt);
			SetCursorPos(pt.x, pt.y);

			hr = m_pUIView->DeviceToWorld( &m_flLastX, &m_flLastY, &m_flLastZ);
			if(FAILED(hr)) goto EXIT_FAIL;
		}

		//m_nLastX = pt.x;
		//m_nLastY = pt.y;
	}

EXIT_FAIL:
	return TRUE;
}

HRESULT CVWTranslate2DToolObject::TranslateSelectedObjects( float flDeltaX, float flDeltaZ )
{
	HRESULT hr = S_OK;
	CTranslate2DObject * pCTrans = NULL;
	BOOL bHandled = FALSE;
	VARIANT_BOOL vbPicked, vbSafe;
	float flLocalDeltaX, flLocalDeltaY, flLocalDeltaZ, fPosX, fPosY, fPosZ;
	int nVertexCount, j, nBoundaryIndex;
	static DWORD dwTime = - 1;
	DWORD dwTimeNow;
	CComBSTR bstr1, bstr2, bstr3;
	CString tmpStr;
	IThing* pThing = NULL;

	if (IsPressed('S'))
	{
		flDeltaX *= SLOWKEY_SLOWFACTOR;
		flDeltaZ *= SLOWKEY_SLOWFACTOR;
	}

	m_flTotalDeltaX += flDeltaX;
	m_flTotalDeltaZ += flDeltaZ;

	//TRACE("Delta: %g %g %g\n", flDeltaX, flDeltaY, flDeltaZ);
	for( POSITION pos = m_TransformList.GetHeadPosition(); pos != NULL; )
	{
		bHandled = TRUE;
		pCTrans = m_TransformList.GetNext( pos );
		if(NULL != pCTrans)
		{
			flLocalDeltaX = flDeltaX;
			flLocalDeltaY = 0.0f;
			flLocalDeltaZ = flDeltaZ;
			
			// Transform the Delta (which is in World coords) to Thing coords
			// (without the translation).
			hr = pCTrans->m_pVWFrame->GetPositionEx(NULL, &fPosX, &fPosY, &fPosZ);
			if(FAILED( hr )) goto EXIT_FAIL;
			flLocalDeltaX = flLocalDeltaX + fPosX;
			flLocalDeltaY = flLocalDeltaY + fPosY;
			flLocalDeltaZ = flLocalDeltaZ + fPosZ;
			hr = pCTrans->m_pVWFrame->InverseTransform(AXIS_Y, &flLocalDeltaX, &flLocalDeltaY, &flLocalDeltaZ, flLocalDeltaX, flLocalDeltaY, flLocalDeltaZ);
			if(FAILED( hr )) goto EXIT_FAIL;

			if (flLocalDeltaX < 0.01f && flLocalDeltaX > -0.01f)
				flLocalDeltaX = 0.0f;
			if (flLocalDeltaZ < 0.01f && flLocalDeltaZ > -0.01f)
				flLocalDeltaZ = 0.0f;

			if (flLocalDeltaX != 0.0f || flLocalDeltaZ != 0.0f)
			{
				if ( pCTrans->bVerticeSelected)
				{
					pCTrans->m_pBoundary->get_VertexCount(&nVertexCount);

					vbPicked = VARIANT_FALSE;
					for (j = 0; j < nVertexCount; j++)
					{
						pCTrans->m_pPickData->get_VertexPicked(j, &vbPicked);
						if (vbPicked)
						{
							hr = pCTrans->m_pBoundary->TranslateVertexSafe(j, flLocalDeltaX, flLocalDeltaZ, &vbSafe);
							if(FAILED( hr )) goto EXIT_FAIL;
						}
					}
				}
				else
				{
					hr = pCTrans->m_pBoundary->Translate(flLocalDeltaX, flLocalDeltaZ);
					if(FAILED( hr )) goto EXIT_FAIL;
				}
			}

			pCTrans->m_pPickData->get_Thing(&pThing);
			pCTrans->m_pPickData->get_BoundaryIndex(&nBoundaryIndex);
			tmpStr.Format("%d", nBoundaryIndex);

			InvokeToolEvent(TOOLEVENT_BOUNDARYUPDATEBYINDEX, pThing, CComBSTR(tmpStr), bstrNULL, bstrNULL, VARIANT_FALSE);

			SAFERELEASE(pThing);

			dwTimeNow = GetTickCount();

			if (dwTimeNow - dwTime > 200)
			{
				tmpStr.Format("%0.3f", m_flTotalDeltaX);
				bstr1 = tmpStr;
				tmpStr = "";
				bstr2 = tmpStr;
				tmpStr.Format("%0.3f", m_flTotalDeltaZ); 
				bstr3 = tmpStr;

				dwTime = dwTimeNow;

				InvokeToolEvent(TOOLEVENT_2DOBJECTTRANSLATED, pCTrans->m_pPickData, bstr1, bstr2, bstr3, VARIANT_TRUE); 
			}
		}
	}

EXIT_FAIL:
	SAFERELEASE(pThing);

	return bHandled;
}

HRESULT CVWTranslate2DToolObject::InvokeToolEvent(int nNum, IObjectProperty* pOP, BSTR bstr1, BSTR bstr2, BSTR bstr3, VARIANT_BOOL bBubble)
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

HRESULT CVWTranslate2DToolObject::InvokeCenterViewOnObject(IThing* pThing)
{
	HRESULT hr = S_OK;
	DISPPARAMS dispparams;
	VARIANT varResult;

	VariantInit(&varResult);

	hr = InitializeDISPPARAMS(&dispparams, 2);
	if (FAILED(hr)) goto EXIT_FAIL;

	dispparams.rgvarg[1].vt = VT_DISPATCH;
	dispparams.rgvarg[1].pdispVal = pThing;
	dispparams.rgvarg[0].vt = VT_BOOL;
	dispparams.rgvarg[0].boolVal = VARIANT_TRUE;

	hr = m_piEditorViewControl->Invoke(dispidCenterViewOnObject, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispparams, &varResult, NULL, NULL);
	if (FAILED(hr))
	{
		OutputDebugString("Translate3D tool:  Failed to invoke CenterViewOnObject\n");
		hr = S_OK;
	}

	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

	VariantClear(&varResult);

EXIT_FAIL:
	return hr;
}


// Temporary transform object:
// Only returns true if there is at least one selected object
// AND all selected objects have an IVWTransform.
STDMETHODIMP CVWTranslate2DToolObject::IsValid(IVWUIView * pVw, VARIANT_BOOL * pfValid)
{
	HRESULT hr = S_OK;
	CVWPickDataPtr pickDataPtr;
	BSTR bstrReturnValue = NULL;
	VARIANT_BOOL	fLastItem = VARIANT_TRUE;
	COleVariant varProperty;
	CComBSTR bstr1, bstr2, bstr3;
	CString tmpStr;
	VARIANT varResult;
	VARIANT_BOOL vbVertPicked = VARIANT_FALSE;
	float fX, fY;

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
				if( pickDataPtr != NULL )  // At least one selected item has an IVWTransform
				{
					CThingPtr ThingPtr;

					hr = pickDataPtr->get_Thing(&ThingPtr.p);
					if(FAILED(hr) || !ThingPtr.p) goto EXIT_FAIL;

					if (SUCCEEDED(hr = ThingPtr->CheckPropertySecurityExt(bstrBoundaryList, PS_WRITE)))
					{
						CTranslate2DObject *pTO = new CTranslate2DObject(  pickDataPtr );

						VariantInit(&varResult);

						SAFERELEASE(m_pWorld);
						hr = pTO->m_pBoundary->get_World(&m_pWorld);
						if(FAILED(hr) || !m_pWorld) 	goto EXIT_FAIL;

						*pfValid = TRUE;

						POSITION pos = m_TransformList.AddTail( pTO );

						pickDataPtr->IsAnyVertexPicked(&vbVertPicked);

						if (vbVertPicked)
						{
							int i, nCount;

							fX = fY = 0.0f;
							pTO->m_pBoundary->get_VertexCount(&nCount);

							for (i = 0; i < nCount; i++)
							{
								pTO->m_pPickData->get_VertexPicked(i, &vbVertPicked);
								if (vbVertPicked)
								{
									pTO->m_pBoundary->GetVertexXYExt(i, &fX, &fY);
									break;
								}
							}
						}
						else
						{
							pTO->m_pBoundary->GetVertexXYExt(0, &fX, &fY);
						}

						tmpStr = "0.0";
						bstr1 = tmpStr;
						bstr3 = tmpStr;
						tmpStr = "";
						bstr2 = tmpStr;

						InvokeToolEvent(TOOLEVENT_2DOBJECTTRANSLATED, pTO->m_pPickData, bstr1, bstr2, bstr3, VARIANT_TRUE); 
					}
					else
					{
						*pfValid = FALSE;
						goto EXIT_FAIL;
					}
				}
				//else // Found one without IVWTransform.
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

STDMETHODIMP CVWTranslate2DToolObject::OnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient)
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

HRESULT CVWTranslate2DToolObject::DestroyTransformList()
{
	HRESULT hr = S_OK;
	CTranslate2DObject* pCTrans = NULL;

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

float CVWTranslate2DToolObject::ElapsedFrameTime()
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


STDMETHODIMP CVWTranslate2DToolObject::Update()
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
		float fTmpX, fTmpY, fTmpZ;
		float flDeltaX , flDeltaY, flDeltaZ;

		if (m_nAxisLock & X_LOCK)
			flDeltaX = 0.0f;
		else
			flDeltaX = (float) nHorzMotion * 2.0f;

		if (m_nAxisLock & Z_LOCK)
			flDeltaZ = 0.0f;
		else
			flDeltaY = (float) nVertMotion * 2.0f ;

		flDeltaZ = 0.0;

		hr = m_pUIView->DeviceToWorld( &flDeltaX, &flDeltaY, &flDeltaZ);
		if(FAILED(hr)) goto EXIT_FAIL;

		fTmpX = fTmpY = fTmpZ = 0.0f;

		hr = m_pUIView->DeviceToWorld( &fTmpX, &fTmpY, &fTmpZ);
		if(FAILED(hr)) goto EXIT_FAIL;

		flDeltaX -= fTmpX;
		flDeltaY -= fTmpY;
		flDeltaZ -= fTmpZ;

		hr = TranslateSelectedObjects( flDeltaX*fObjectNavScale*fTime, flDeltaZ*fObjectNavScale*fTime);
		if (FAILED(hr))	goto EXIT_FAIL;
	}

EXIT_FAIL:

	if (SUCCEEDED(hr))
		return TRUE;
	else
		return FALSE;
}

BOOL CVWTranslate2DToolObject::OnMessage( UINT nIDEvent, WPARAM wParam, LPARAM lParam )
{
	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;

	if ((nIDEvent == WM_KILLFOCUS || nIDEvent == WM_ENTERMENULOOP) && m_pUIView && m_bTrackingMouse)
	{
		OnDoneTranslating();
		m_bTrackingMouse = FALSE;
		ReleaseCapture();
		if (!m_bShowingNoCursor)
			ShowCursor(TRUE);
		else
		{
			m_bShowingNoCursor = FALSE;
			SetCursor(m_pUIView, LoadCursor(NULL, IDC_ARROW ));
		}
	}

	if (nIDEvent == WM_KILLFOCUS)
		m_bGotFocus = VARIANT_FALSE;
	else if (nIDEvent == WM_SETFOCUS)
		m_bGotFocus = VARIANT_TRUE;

	return bHandled;
}

BOOL CVWTranslate2DToolObject::OnKeyDown( UINT nVirtKey, LPARAM lKeyData )
{
	HRESULT hr = S_OK;

	if (nVirtKey == VK_RIGHT || nVirtKey == VK_LEFT || nVirtKey == VK_DOWN ||  nVirtKey == VK_UP)
	{
		if (!m_bUsingKeyboard)
		{
			SetupTransformList();
		}
		m_bUsingKeyboard = TRUE;
	}

	return TRUE;
}

BOOL CVWTranslate2DToolObject::OnKeyUp( UINT nVirtKey, LPARAM lKeyData )
{
	if (m_bUsingKeyboard && (nVirtKey == VK_RIGHT || nVirtKey == VK_LEFT || nVirtKey == VK_DOWN ||  nVirtKey == VK_UP) )
	{
		OnDoneTranslating();
		m_bUsingKeyboard = FALSE;
	}

	return TRUE;
}
