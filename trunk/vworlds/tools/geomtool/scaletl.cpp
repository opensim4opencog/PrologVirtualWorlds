// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWScaleTl.cpp : Implementation of CVWScale3DTool
#include "stdafx.h"
#include <d3drm.h>
#include <geomtool.h>
#include <math.h>
#include <float.h>
#include "GeomUndo.h"
#include "ScaleTl.h"
#include <vwsevent.h>
#include <vwmm_i.c>
#include <vwrnedit.h>
#include "euler.h"

/////////////////////////////////////////////////////////////////////////////
typedef CVWComPtr<IVWTransform,NULL,&IID_IVWTransform> CVWTransformPtr;
typedef CVWComPtr<IThing,NULL,&IID_IThing> CVWThingPtr;
typedef CVWComPtr<IVector,NULL,&IID_IVector> CVWVectorPtr;

#define IsPressed(key)			(GetAsyncKeyState(key) < 0)
#define X_LOCK 1
#define Y_LOCK 2
#define Z_LOCK 4
#define dispidToolEvent 19L
#define dispidResetCameraPosition 12L
#define dwMaxTimeDelta			250				// maximum of 250ms (4fps) of dead reckoning 
#define fTiltScale				DEG2RAD(90.f)	// It takes at least 1 second to look up or down 90 degrees
#define fObjectNavScale			12.0f			// General speed of object navigation
#define MOUSE_TO_WORLD_FACTOR	5.0f
#define CLAMP(x, xmin, xmax)	if ((x) < (xmin)) (x) = (xmin); else if ((x) > (xmax)) (x) = (xmax)
#define PI						3.1415927F
#define DEG2RAD(d)				(d*PI/180.0F)
#define SLOWKEY_SLOWFACTOR		0.2f
#define SIGN(x)					(x < 0.0f ? -1 : 1)
#define PIOVER180				0.0174532F
#define PIOVER2					1.5707963F
#define PITIMES2				6.2831853F
#define FLOAT_EQUAL(f_1, f_2) ( fabs(f_1 - f_2) < 0.01 )
#define FLOAT_GREATERTHAN(f_1, f_2) ( f_1 > f_2 && fabs(f_1 - f_2) > 0.01 )
#define FLOAT_LESSTHAN(f_1, f_2) ( f_1 < f_2 && fabs(f_1 - f_2) > 0.01 )
#define MIN_SCALE 0.01f
#define MAX_SCALE 99999.0f
/////////////////////////////////////////////////////////////////////////////
extern const CLSID CLSID_Vector;

//These are defined in trans3d.cpp
extern CComBSTR bstrAddSelection;
extern CComBSTR bstrRemoveSelection;
extern CComBSTR bstrClearSelection;
extern CComBSTR bstrAddCurrentTool;
extern CComBSTR bstrGravity;
extern CComBSTR bstrXAxisLocked;
extern CComBSTR bstrYAxisLocked;
extern CComBSTR bstrZAxisLocked;
extern CComBSTR bstrNavigate;
extern CComBSTR bstrAvatar;

/////////////////////////////////////////////////////////////////////////////
CComBSTR bstrScale("Scale");
CComBSTR bstrScaleChanged(VWSTUDIO_SELECTIONSCALECHANGED_EVENT_STR);

const IID DIID__RenderingControl = { 0x44fac781, 0xca4, 0x11d0, { 0x89, 0xa9, 0, 0xa0, 0xc9, 0x5, 0x41, 0x29 } };


extern void ApplyParentTransformations(IDirect3DRMFrame *pFrame, D3DVECTOR* vec);

/////////////////////////////////////////////////////////////////////////////
// CVWScale3DTool

CVWScale3DTool::CVWScale3DTool()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HRESULT hr = S_OK;

	m_pVWRenderRoot = NULL;
	m_pWorld = NULL;
	m_pVector = NULL;
	m_nAxisLock = 0;
	m_piEditorViewControl = NULL;
	m_pRMCameraFrame = NULL;
	m_bGravity = VARIANT_FALSE;
	m_nCameraMode = PERSPECTIVE;
	m_bUsingKeyboard = VARIANT_FALSE;
	m_bGotFocus = VARIANT_FALSE;
	m_bShowingNoCursor = FALSE;
	m_bTrackingMouse = FALSE;

	SetCursor(NULL, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_SCALE2D)));

}

CVWScale3DTool::~CVWScale3DTool()
{
	DeletePtrListElements(&m_TransformList);

	SAFERELEASE(m_pRMCameraFrame);
	SAFERELEASE(m_piEditorViewControl);
	SAFERELEASE(m_pVector);
	SAFERELEASE(m_pVWRenderRoot);
	SAFERELEASE(m_pWorld);
}

STDMETHODIMP CVWScale3DTool::Initialize(IVWUIView* pVw, IWorld* pWorld, IVWRenderRoot *pVWRenderRoot)
{
	IVWNavigationTool * pNavTool = NULL;
	IVWRender3D *pRender3D = NULL;
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

	SAFERELEASE(m_pWorld);
	m_pWorld = pWorld;
	SAFEADDREF(m_pWorld);

	SAFERELEASE(m_pVWRenderRoot);
	m_pVWRenderRoot = pVWRenderRoot;
	SAFEADDREF(pVWRenderRoot);

	hr = m_pVWRenderRoot->get_VWRender3D(&pRender3D);
	if (FAILED(hr) || (pRender3D == NULL)) goto EXIT_FAIL;

	SAFERELEASE(m_pRMCameraFrame);
	hr = pRender3D->get_Camera(&m_pRMCameraFrame);
	if (FAILED(hr) || m_pRMCameraFrame == NULL) goto EXIT_FAIL;

	hr = m_pWorld->get_Tool(bstrNavigate, (IUnknown**) &pNavTool);
	if (FAILED(hr) || (pNavTool == NULL)) goto EXIT_FAIL;

	for (i = 0; i < kbMax; i++)
	{
		hr = pNavTool->get_KeyBinding((KeyConstant) i, &var);
		if (FAILED(hr)) goto EXIT_FAIL;

		m_KeyBindings[i] = var.lVal;
	}

	//Initialize the start time
	ElapsedFrameTime();

	if (!m_piEditorViewControl)
	{
		hr = m_pUIView->QueryInterface(DIID__RenderingControl, (LPVOID*) &m_piEditorViewControl);
		if (FAILED(hr) || (m_piEditorViewControl == NULL)) goto EXIT_FAIL;
	}


EXIT_FAIL:
	
	SAFERELEASE(pRender3D);
	SAFERELEASE(pNavTool);
	return hr;
}

//This function builds a list of scalable objects from the selection list.
//pfValid is FALSE if there's nothing to scale.
STDMETHODIMP CVWScale3DTool::IsValid(IVWUIView *pVw,VARIANT_BOOL * pfValid)
{

	HRESULT hr = S_OK;
	CVWTransformPtr TransPtr;
	BSTR bstrReturnValue = NULL;
	VARIANT_BOOL	fLastItem = VARIANT_TRUE;
	COleVariant varProperty;
	IVWFrame *pVWFrame = NULL;
	IVWFrame *pVWParentFrame = NULL;
	CVWThingPtr ThingPtr;
	IVector			*pvPos = NULL;
	float			fPosx, fPosy, fPosz;
	CTranslate3DObject* pTmpTransObj;
	VARIANT_BOOL vbMoveable;

	ASSERT( pfValid );
	if (!pfValid ) return E_POINTER;

	*pfValid = VARIANT_FALSE;
	
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
				ThingPtr = varProperty;

				if (ThingPtr != NULL)
				{
					COleVariant			var; // get the geometry property
					CComBSTR			bstrType;

					SAFERELEASE(m_pWorld);
					hr = ThingPtr->get_World(&m_pWorld);
					if(FAILED(hr) || !m_pWorld) 	goto EXIT_FAIL;

					//Create our event vector if we don't have one already.
					if (!m_pVector)
					{
						hr = m_pWorld->CreateObjectPropertyExt(CLSID_Vector, NULL, (IObjectProperty**) &m_pVector);
						if (FAILED(hr)) goto EXIT_FAIL;
					}

					hr = ThingPtr->get_ObjectProperty(bstrScale, (IObjectProperty**)&pvPos);
					if (FAILED(hr)) goto EXIT_FAIL;

					hr = pvPos->get(&fPosx, &fPosy, &fPosz);
					if (FAILED(hr)) goto EXIT_FAIL;
					SAFERELEASE(pvPos);

					if (FAILED(hr = ThingPtr->CheckPropertySecurityExt(bstrScale, PS_WRITE)))
					{
						goto SECURITY_ERROR;
					}

					vbMoveable = VARIANT_TRUE;
					if (SUCCEEDED(hr = ThingPtr->get_BOOL(CComBSTR("IsMoveable"), &vbMoveable)) && !vbMoveable)
					{
						goto SECURITY_ERROR;
					}

					if (FAILED(hr = ThingPtr->get_Type(&bstrType.m_str)) || CompareElements(&bstrAvatar.m_str, &bstrType.m_str))
					{
						goto SECURITY_ERROR;
					}

					hr = ThingPtr->InvokeMethodExt(CComBSTR("GetFrame"), NULL, &var);
					if (FAILED(hr)) goto EXIT_FAIL;

					TransPtr = var;
					if( TransPtr != NULL )
					// At least one selected item has an IVWTransform
					{
						hr = TransPtr->QueryInterface(IID_IVWFrame, (void **) &pVWFrame);
						if (SUCCEEDED(hr) && pVWFrame != NULL)
						{
							hr = pVWFrame->GetParent(&pVWParentFrame);
							if (SUCCEEDED(hr) && pVWParentFrame != NULL)
							{
								*pfValid = VARIANT_TRUE;
								POSITION pos = m_TransformList.AddTail( pTmpTransObj = new CTranslate3DObject( ThingPtr, TransPtr, pVWFrame ) );

								pTmpTransObj->currentLocation.x = fPosx;
								pTmpTransObj->currentLocation.y = fPosy;
								pTmpTransObj->currentLocation.z = fPosz;
							}
		    				if( FAILED(hr) ) goto EXIT_FAIL;
							SAFERELEASE(pVWParentFrame);
						}
	    				if( FAILED(hr) ) goto EXIT_FAIL;
						SAFERELEASE(pVWFrame);
					}
SECURITY_ERROR:
					SAFERELEASE(pvPos);
					var.Clear();
				}
				varProperty.Clear();
				hr = m_pSelectionList->NextItem( &bstrReturnValue, &varProperty, &fLastItem );
    			if( FAILED(hr) ) goto EXIT_FAIL;
			}
		}  
	}
	goto EXIT_SUCCEED;
EXIT_FAIL:
	if (!m_TransformList.IsEmpty())
		DeletePtrListElements(&m_TransformList);

	*pfValid = FALSE;

EXIT_SUCCEED:
	SAFERELEASE(pvPos);
	SAFEFREESTRING(bstrReturnValue);
	SAFERELEASE(pVWFrame);
	SAFERELEASE(pVWParentFrame);

	return hr;

}

BOOL CVWScale3DTool::OnLButtonDown( UINT nFlags, POINT pt )
{
	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;
	HWND hWnd;

	if(m_pUIView && !m_bTrackingMouse)
	{ 
		m_cptLast = pt;
		m_pUIView->get_HWND(&hWnd);
		m_ptAnchor = pt;
		m_bTrackingMouse = VARIANT_TRUE;
		m_vecScale.x = 1.0;
		m_vecScale.y = 1.0;
		m_vecScale.z = 1.0;
		SetCapture(hWnd);
		bHandled = TRUE;

		hr = SetupTranformList();
		if (FAILED(hr))
		{
			SetCursor(m_pUIView, LoadCursor(NULL, IDC_NO ));
			m_bShowingNoCursor = TRUE;
			hr = S_OK;
		}
		else
			ShowCursor(FALSE);
	}
	else
	{
		m_bTrackingMouse = VARIANT_FALSE;
		goto EXIT_FAIL;
	}

EXIT_FAIL:
	return bHandled;
}

BOOL CVWScale3DTool::OnMouseMove(UINT nFlags, POINT pt)
{
	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;
	HWND hWnd;

	int nDeltaX, nDeltaY;

	if (!m_pUIView || !m_pVWRenderRoot || m_bTrackingMouse == VARIANT_FALSE)
		goto EXIT_FAIL;

	// If no delta then leave.
	if( (pt.x == m_cptLast.x )	&& (pt.y == m_cptLast.y ) )
		goto EXIT_FAIL;

	// If left button is down, scale all selected translatable objects.
	if( (MK_LBUTTON & nFlags)	)
	{
		CPoint ptScreen(m_ptAnchor);

		m_pUIView->get_HWND(&hWnd);
		ClientToScreen(hWnd, &ptScreen);
		SetCursorPos(ptScreen.x, ptScreen.y);

		nDeltaX = pt.x - m_cptLast.x;
		nDeltaY = pt.y - m_cptLast.y;

		if (IsPressed(VK_SHIFT))
		{
			nDeltaX = nDeltaY = (nDeltaX - nDeltaY) / 2;
			//nDeltaY = nDeltaX;
		}
		else
			nDeltaY = -nDeltaY;

		hr = ScaleSelectedObjects((float) (nDeltaX/MOUSE_TO_WORLD_FACTOR), (float) (nDeltaY/MOUSE_TO_WORLD_FACTOR) );
		if (FAILED(hr)) goto EXIT_FAIL;
	}
EXIT_FAIL:

	return bHandled;
}


BOOL CVWScale3DTool::OnLButtonUp(UINT nFlags, POINT pt )
{
	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;
	HWND hWnd;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_bTrackingMouse == VARIANT_TRUE)
	{
		m_bTrackingMouse = VARIANT_FALSE;
		m_pUIView->get_HWND(&hWnd);
		ClientToScreen(hWnd, &m_ptAnchor);
		SetCursorPos(m_ptAnchor.x, m_ptAnchor.y);
		if (!m_bShowingNoCursor)
			ShowCursor(TRUE);
		else
		{
			m_bShowingNoCursor = FALSE;
			SetCursor(m_pUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_SCALE2D)));
		}

		ReleaseCapture();
		bHandled = TRUE;

		OnDoneScaling();
	}
	else
		goto FAIL_EXIT;

FAIL_EXIT:
	return bHandled;
}

HRESULT CVWScale3DTool::ScaleSelectedObjects( float flDeltaX, float flDeltaY )
{
	CTranslate3DObject * pCTrans = NULL;
	IDirect3DRMFrame *pRMObjFrame = NULL;
	IDirect3DRMFrame *pRMParentFrame = NULL;
	IDirect3DRMFrame *pRMTmpFrame = NULL;
	D3DVECTOR axis;
	IVWFrame * pScene = NULL;
	BOOL bXAxisLocked, bYAxisLocked, bZAxisLocked;
	DWORD dwTimeNow;
	CComBSTR bstr1, bstr2, bstr3;
	CString tmpStr;
	static DWORD dwTime = - 1;
	HRESULT hr = S_OK;
	POSITION pos;
	CString szTmp;
	D3DVECTOR vecDelta, initialPos;
	IVector* vecPtr = NULL, *vecDestPtr = NULL;
	float fRot;
	float fX, fY, fZ, fTmp;

	D3DVECTOR vecCameraPos, vecObjToCam;

	if (IsPressed('S'))
	{
		flDeltaX *= SLOWKEY_SLOWFACTOR;
		flDeltaY *= SLOWKEY_SLOWFACTOR;
	}

	bXAxisLocked = m_nAxisLock & X_LOCK;
	bYAxisLocked = m_nAxisLock & Y_LOCK;
	bZAxisLocked = m_nAxisLock & Z_LOCK;

	if (m_nCameraMode == TOP)
	{
		flDeltaX /= 64.0f;
		flDeltaY /= 64.0f;
	}

	if (IsPressed('X'))
	{
		if (m_nCameraMode == TOP)
		{
			vecDelta.x = -flDeltaX / 4.0f; 
			vecDelta.y = 0.0f; 
			vecDelta.z = 0.0f;
		}
		else
		{
			vecDelta.x = -flDeltaX / 32.0f; 
			vecDelta.y = 0.0f; 
			vecDelta.z = 0.0f;
		}
	}
	else if (IsPressed('Y'))
	{
		if (m_nCameraMode == TOP)
		{
			vecDelta.x = 0.0; 
			vecDelta.y = 0.0f; 
			vecDelta.z = flDeltaY / 4.0f;
		}
		else
		{
			vecDelta.x = 0.0; 
			vecDelta.y = flDeltaY / 32.0f; 
			vecDelta.z = 0.0f;
		}
	}
	else if (IsPressed('Z'))
	{
		if (m_nCameraMode == TOP)
		{ 
			vecDelta.x = 0.0; 
			vecDelta.y = flDeltaY / 4.0f; 
			vecDelta.z = 0.0f;
		}
		else
		{
			vecDelta.x = 0.0; 
			vecDelta.y = 0.0f; 
			vecDelta.z = flDeltaY / 32.0f;
		}
	}
	else
	{
		if (m_nCameraMode == TOP)
		{
			vecDelta.x = (bXAxisLocked ? 0.0f : -flDeltaX / 4.0f);
			vecDelta.y = 0.0f; 
			vecDelta.z = (bZAxisLocked ? 0.0f : flDeltaY / 4.0f);
		}
		else
		{
			vecDelta.x = (bXAxisLocked ? 0.0f : flDeltaX / 32.0f); 
			vecDelta.y = 0.0f;
			vecDelta.z = (bZAxisLocked ? 0.0f : flDeltaY / 32.0f);  //0.0f; //(bZAxisLocked ? 0.0f : flDeltaX / 32.0f);
		}
	}
	
	hr = m_pRMCameraFrame->GetPosition(NULL, &vecCameraPos);
	if (FAILED(hr)) goto EXIT_FAIL;

	hr = CoCreateInstance(CLSID_Vector, NULL, CLSCTX_INPROC_SERVER, IID_IVector, (LPVOID*)&vecPtr);
	if (FAILED(hr)) goto EXIT_FAIL;

	hr = CoCreateInstance(CLSID_Vector, NULL, CLSCTX_INPROC_SERVER, IID_IVector, (LPVOID*)&vecDestPtr);
	if (FAILED(hr)) goto EXIT_FAIL;
	
	for( pos = m_TransformList.GetHeadPosition(); pos != NULL; )
	{
		pCTrans = m_TransformList.GetNext( pos );
		if(pCTrans != NULL && pCTrans->m_pTrans != NULL)
		{
			hr = pCTrans->m_pVWFrame->get_Frame3D(&pRMObjFrame);
			if (FAILED(hr) || (!pRMObjFrame)) goto EXIT_FAIL;

			hr = pRMObjFrame->GetParent(&pRMParentFrame);
			if (FAILED(hr) || (!pRMParentFrame)) goto EXIT_FAIL;

			pRMObjFrame->GetPosition(NULL, &initialPos);
			if (FAILED(hr)) goto EXIT_FAIL;

			if (!IsPressed(VK_SHIFT))
			{
				vecObjToCam.x = initialPos.x - vecCameraPos.x;
				vecObjToCam.y = 0.0f; //initialPos.y - vecCameraPos.y;
				vecObjToCam.z = initialPos.z - vecCameraPos.z;

				D3DRMVectorNormalize(&vecObjToCam);

				vecPtr->set(vecObjToCam.x, vecObjToCam.y, vecObjToCam.z);
				vecPtr->get_Rotation(&fRot);

				if (fRot >= 0.785 && fRot <= 2.355)
				{ //Camera is behind the object
				}
				else if (fRot >= 2.355 && fRot <= 3.925)
				{ //Camera is left of the object
					float fTmp;

					fTmp = vecDelta.x;
					vecDelta.x = vecDelta.z;
					vecDelta.z = fTmp;
				}
				else if (fRot >= 3.925 && fRot <= 5.495)
				{ //Camera is in front of the object
				}
				else
				{ //Camera is right of the object
					float fTmp;

					fTmp = vecDelta.x;
					vecDelta.x = vecDelta.z;
					vecDelta.z = fTmp;
				}

				ComputeEulerAngles(pCTrans->m_pVWFrame, vecDestPtr);
				vecDestPtr->get(&fX, &fY, &fZ);

				if (fX >= 0.785 && fX <= 2.355)
				{ //Camera is behind the object
					fTmp = vecDelta.z;
					vecDelta.z = vecDelta.y;
					vecDelta.y = fTmp;
				}
				else if (fX >= 2.355 && fX <= 3.925)
				{ //Camera is left of the object
				}
				else if (fX >= 3.925 && fX <= 5.495)
				{ //Camera is in front of the object
					fTmp = vecDelta.z;
					vecDelta.z = vecDelta.y;
					vecDelta.y = fTmp;
				}
				else
				{ //Camera is right of the object
				}

				if (fY >= 0.785 && fY <= 2.355) // && !(fX >= 2.355 && fX <= 3.925))
				{ //Camera is behind the object
					fTmp = vecDelta.x;
					vecDelta.x = vecDelta.z;
					vecDelta.z = fTmp;
				}
				else if (fY >= 2.355 && fY <= 3.925)
				{ //Camera is left of the object
				}
				else if (fY >= 3.925 && fY <= 5.495) // && !(fX >= 2.355 && fX <= 3.925) )
				{ //Camera is in front of the object
					fTmp = vecDelta.x;
					vecDelta.x = vecDelta.z;
					vecDelta.z = fTmp;
				}
				else
				{ //Camera is right of the object
				}
			}
			else
			{	//Shift it pressed
				vecDelta.y = vecDelta.x;
			}

			pRMObjFrame->SetPosition(NULL, 0.0f, 0.0f, 0.0f);
			if (FAILED(hr)) goto EXIT_FAIL;
		
			//Do a uniform (all axis) scaling
//				axis.x = 1.0f + (vecDelta.x > 0 ? vecDelta.x : vecDelta.z);
//				axis.y = 1.0f + (vecDelta.x > 0 ? vecDelta.x : vecDelta.z);
//				axis.z = 1.0f + (vecDelta.x > 0 ? vecDelta.x : vecDelta.z);
//			}
//			else //Normal perspective viewing
//			{
				axis.x = 1.0f + vecDelta.x;
				axis.y = 1.0f + vecDelta.y;
				axis.z = 1.0f + vecDelta.z;
//			}

			//Fix up current scale and fire UI event
			if (SIGN(pCTrans->currentLocation.x) == SIGN(axis.x) && axis.x != 0.0f)
			{
				fTmp = pCTrans->currentLocation.x;

				pCTrans->currentLocation.x *= axis.x;

				if (pCTrans->currentLocation.x < MIN_SCALE || pCTrans->currentLocation.x > MAX_SCALE)
				{
					pCTrans->currentLocation.x = fTmp;
					axis.x = 1.0f;
				}
			}
			else
				axis.x = 1.0f;

			if (SIGN(pCTrans->currentLocation.y) == SIGN(axis.y) && axis.y != 0.0f)
			{
				fTmp = pCTrans->currentLocation.y;

				pCTrans->currentLocation.y *= axis.y;

				if (pCTrans->currentLocation.y < MIN_SCALE || pCTrans->currentLocation.y > MAX_SCALE)
				{
					pCTrans->currentLocation.y = fTmp;
					axis.y = 1.0f;
				}
			}
			else
				axis.y = 1.0f;

			if (SIGN(pCTrans->currentLocation.z) == SIGN(axis.z) && axis.z != 0.0f)
			{
				fTmp = pCTrans->currentLocation.z;

				pCTrans->currentLocation.z *= axis.z;

				if (pCTrans->currentLocation.z < MIN_SCALE || pCTrans->currentLocation.z > MAX_SCALE)
				{
					pCTrans->currentLocation.z = fTmp;
					axis.z = 1.0f;
				}
			}
			else
				axis.z = 1.0f;

			hr = pRMObjFrame->AddScale(D3DRMCOMBINE_BEFORE, axis.x, axis.y, axis.z);
			if (FAILED(hr)) goto EXIT_FAIL;

			pRMObjFrame->SetPosition(NULL, initialPos.x, initialPos.y, initialPos.z);
			if (FAILED(hr)) goto EXIT_FAIL;

			dwTimeNow = GetTickCount();

			if (dwTimeNow - dwTime > 200)
			{
				tmpStr.Format("%0.3f", pCTrans->currentLocation.x);
				bstr1 = tmpStr;
				tmpStr.Format("%0.3f", pCTrans->currentLocation.y); 
				bstr2 = tmpStr;
				tmpStr.Format("%0.3f", pCTrans->currentLocation.z); 
				bstr3 = tmpStr;

				hr = InvokeToolEvent(TOOLEVENT_3DOBJECTSCALED, pCTrans->m_pThing, bstr1.m_str, bstr2.m_str, bstr3.m_str, VARIANT_TRUE);
				if(FAILED( hr )) goto EXIT_FAIL;

				dwTime = dwTimeNow;
			}

			SAFERELEASE(pRMObjFrame);
			SAFERELEASE(pRMParentFrame);
		}
	}

EXIT_FAIL:
	SAFERELEASE(pRMParentFrame);
	SAFERELEASE(pRMObjFrame);
	SAFERELEASE(pScene);
	SAFERELEASE(vecDestPtr);
	SAFERELEASE(vecPtr);
	
	return hr;
}

STDMETHODIMP CVWScale3DTool::ComputeEulerAngles(IVWFrame* pVWFrame, IVector* vecAngles)
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMFRAME pFrame = NULL;
	D3DRMMATRIX4D d3dmatrix;
	D3DVECTOR vec;
	HMatrix hmat;
	EulerAngles outAngles;
	int i,j;

	if (!vecAngles)
	{
		hr = E_POINTER;
		goto EXIT_FAIL;
	}

	hr = pVWFrame->get_Frame3D(&pFrame);
	if (FAILED(hr)) goto EXIT_FAIL;

	hr = pFrame->GetTransform(d3dmatrix);
	if (FAILED(hr)) goto EXIT_FAIL;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			hmat[j][i] = d3dmatrix[i][j];

	outAngles = Eul_FromHMatrix(hmat, EulOrdXZYs);  //EulOrdXYXs //EulOrdXZXs

	vec.x = outAngles.x ;
	vec.y = outAngles.z - PIOVER2; //+ PI;
	vec.z = outAngles.y; // + PI;

	if (FLOAT_GREATERTHAN(vec.x, PITIMES2 - 0.01f))
		vec.x -= PITIMES2;
	else if (FLOAT_LESSTHAN(vec.x, 0.0f))
		vec.x += PITIMES2;

	if (FLOAT_GREATERTHAN(vec.y, PITIMES2 - 0.01f))
		vec.y -= PITIMES2;
	else if (FLOAT_LESSTHAN(vec.y, 0.0f))
		vec.y += PITIMES2;

	if (FLOAT_GREATERTHAN(vec.z, PITIMES2 - 0.01f))
		vec.z -= PITIMES2;
	else if (FLOAT_LESSTHAN(vec.z, 0.0f))
		vec.z += PITIMES2;

	hr = vecAngles->set(vec.x, vec.y, vec.z);
	if (FAILED(hr)) goto EXIT_FAIL;

EXIT_FAIL:
	SAFERELEASE(pFrame);
	return hr;
}


HRESULT CVWScale3DTool::OnDoneScaling()
{
	HRESULT hr = S_OK;
	POSITION pos;
	CTranslate3DObject * pCTrans = NULL;
	IVector			*pvPos = NULL;

	for( pos = m_TransformList.GetHeadPosition(); pos != NULL; )
	{
		pCTrans = m_TransformList.GetNext( pos );
		if(NULL != pCTrans)
		{
			pvPos = NULL;
			if (FAILED(hr = pCTrans->m_pThing->get_ObjectProperty(bstrScale, (IObjectProperty**)&pvPos)))
				goto FAIL_EXIT;

			pvPos->set(pCTrans->currentLocation.x, pCTrans->currentLocation.y, pCTrans->currentLocation.z);
		}
	}

FAIL_EXIT:
	SAFERELEASE(pvPos);
	return hr;
}




HRESULT CVWScale3DTool::InvokeToolEvent(int nNum, IThing* pThing, BSTR bstr1, BSTR bstr2, BSTR bstr3, VARIANT_BOOL bBubble)
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
	dispparams.rgvarg[4].pdispVal = pThing;
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



STDMETHODIMP CVWScale3DTool::Update()
{
	HRESULT hr = S_OK;
	VARIANT_BOOL vbBool = VARIANT_TRUE;
	LPDIRECT3DRMFRAME pD3DParent = NULL;
	D3DVECTOR vCross, vDir, vUp;
	BOOL				bReset;
	int					nTilt, nHorzMotion, nVertMotion;
	float				fTilt, fTime;

	fTime = ElapsedFrameTime();

	if (!m_bGotFocus) goto EXIT_FAIL;

	nTilt = IsPressed(m_KeyBindings[kbTiltUp]) - IsPressed(m_KeyBindings[kbTiltDown]);
	bReset = IsPressed(m_KeyBindings[kbReset]);

	nHorzMotion = IsPressed(VK_RIGHT) - IsPressed(VK_LEFT);
	nVertMotion = IsPressed(VK_DOWN) - IsPressed(VK_UP);

	if ( nTilt && m_nCameraMode != TOP)
	{
		hr = m_pRMCameraFrame->GetParent(&pD3DParent);
		if (FAILED(hr) || !pD3DParent)	goto EXIT_FAIL;

		hr = m_pRMCameraFrame->GetOrientation(pD3DParent, &vDir, &vUp);
		if (FAILED(hr))	goto EXIT_FAIL;

		if (!D3DRMVectorCrossProduct(&vCross, &vDir, &vUp))
		{
			hr = E_FAIL;
			goto EXIT_FAIL;
		}

		fTilt = nTilt * fTiltScale * fTime;
			
		hr = m_pRMCameraFrame->GetPosition(pD3DParent, &vDir);
		if (FAILED(hr))	goto EXIT_FAIL;

		hr = m_pRMCameraFrame->SetPosition(pD3DParent, 0.0f, 0.0f, 0.0f);
		if (FAILED(hr))	goto EXIT_FAIL;

		hr = m_pRMCameraFrame->AddRotation(D3DRMCOMBINE_AFTER, vCross.x, vCross.y, vCross.z, fTilt);
		if (FAILED(hr))	goto EXIT_FAIL;

		hr = m_pRMCameraFrame->SetPosition(pD3DParent, vDir.x, vDir.y, vDir.z);
		if (FAILED(hr))	goto EXIT_FAIL;
	}
	
	if (bReset)
	{
		hr = m_pRMCameraFrame->GetParent(&pD3DParent);
		if (FAILED(hr) || !pD3DParent)	goto EXIT_FAIL;

		hr = m_pRMCameraFrame->GetOrientation(pD3DParent, &vDir, &vUp);
		if (FAILED(hr))	goto EXIT_FAIL;

		if (m_nCameraMode == TOP)
		{
			vDir.x = 0.0f;
			vDir.y = -1.0f;
			vDir.z = 1.0f;
			vUp.x = 0.0f;
			vUp.y = 0.0f;
			vUp.z = 1.0f;
			hr = m_pRMCameraFrame->SetOrientation(pD3DParent, vDir.x, vDir.y, vDir.z, vUp.x, vUp.y, vUp.z);
			if (FAILED(hr))	goto EXIT_FAIL;
		}
		else
		{
			DISPPARAMS dispparams;
			VARIANT varResult;

			VariantInit(&varResult);

			hr = InitializeDISPPARAMS(&dispparams, 0);
			if (FAILED(hr)) goto EXIT_FAIL;

			hr = m_piEditorViewControl->Invoke(dispidResetCameraPosition, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispparams, &varResult, NULL, NULL);
			if (FAILED(hr)) goto EXIT_FAIL;

			TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
//			vUp.x = 0.0f;
//			vUp.y = 1.0f;
//			vUp.z = 0.0f;
//			vDir.y = 0.0f;
//			D3DRMVectorNormalize(&vDir);
		}
	}

	if (m_bUsingKeyboard && (nHorzMotion || nVertMotion) )
	{
		hr = ScaleSelectedObjects( (float) nHorzMotion*fObjectNavScale*fTime, (float)-nVertMotion*fObjectNavScale*fTime);
		if (FAILED(hr))	goto EXIT_FAIL;
	}

EXIT_FAIL:

	SAFERELEASE(pD3DParent);

	if (SUCCEEDED(hr))
		return TRUE;
	else
	return FALSE;
}

float CVWScale3DTool::ElapsedFrameTime()
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



STDMETHODIMP CVWScale3DTool::OnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient)
{
	if (CompareElements(&bstrAddSelection.m_str, &bstrEventName))
	{
		SetupTranformList();
	}
	else if (CompareElements(&bstrRemoveSelection.m_str, &bstrEventName))
	{
		SetupTranformList();
	}
	else if (CompareElements(&bstrAddCurrentTool.m_str, &bstrEventName))
	{
		SetupTranformList();
	}
	else if (CompareElements(&bstrClearSelection.m_str, &bstrEventName))
	{
		DestroyTransformList();
	}
	else if (CompareElements(&bstrScaleChanged.m_str, &bstrEventName))
	{
		SetupTranformList();
	}

	return S_OK;
};

HRESULT CVWScale3DTool::SetupTranformList()
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

HRESULT CVWScale3DTool::DestroyTransformList()
{
	HRESULT hr = S_OK;

	DeletePtrListElements(&m_TransformList);

	return hr;
}

HRESULT CVWScale3DTool::SetupEnvironment()
{
	HRESULT hr = S_OK;
	IThing* pGlobal = NULL;
	VARIANT_BOOL vbLock;
	D3DVECTOR tmpVec, upVec;

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

	pGlobal->get_BOOL(bstrGravity, &m_bGravity);
	if(FAILED(hr)) goto EXIT_FAIL;

	hr = m_pRMCameraFrame->GetOrientation(NULL, &tmpVec, &upVec);
	if(FAILED(hr)) goto EXIT_FAIL;

	D3DRMVectorNormalize(&upVec);

	//Figure out which editing mode we're in.
	if (upVec.y == 0.0f)
		m_nCameraMode = TOP;
	else
		m_nCameraMode = PERSPECTIVE;


EXIT_FAIL:
	SAFERELEASE(pGlobal);

	return hr;
}


BOOL CVWScale3DTool::OnKeyDown( UINT nVirtKey, LPARAM lKeyData )
{
	if (nVirtKey == VK_RIGHT || nVirtKey == VK_LEFT || nVirtKey == VK_DOWN ||  nVirtKey == VK_UP)
	{
		if (!m_bUsingKeyboard)
		{
			SetupEnvironment();
		}
		m_bUsingKeyboard = TRUE;
	}

	return TRUE;
}

BOOL CVWScale3DTool::OnKeyUp( UINT nVirtKey, LPARAM lKeyData )
{
	if (m_bUsingKeyboard && (nVirtKey == VK_RIGHT || nVirtKey == VK_LEFT || nVirtKey == VK_DOWN ||  nVirtKey == VK_UP) )
	{
		OnDoneScaling();
		m_bUsingKeyboard = FALSE;
	}

	return TRUE;
}

BOOL CVWScale3DTool::OnMessage( UINT nIDEvent, WPARAM wParam, LPARAM lParam )
{
	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if ((nIDEvent == WM_KILLFOCUS || nIDEvent == WM_ENTERMENULOOP) &&
		m_bTrackingMouse && m_pUIView)
	{
		// Set the mouse cursor to where it would be if we hadn't been mucking with it.
		HWND hWnd;
		m_pUIView->get_HWND(&hWnd);
		ClientToScreen(hWnd, &m_ptAnchor);
		SetCursorPos(m_ptAnchor.x, m_ptAnchor.y);

		m_bTrackingMouse = VARIANT_FALSE;
		if (!m_bShowingNoCursor)
			ShowCursor(TRUE);
		else
		{
			m_bShowingNoCursor = FALSE;
			SetCursor(m_pUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_SCALE2D)));
		}

		// Stop all future mouse input getting sent to this window.
		ReleaseCapture();
	}

	if (nIDEvent == WM_KILLFOCUS)
		m_bGotFocus = VARIANT_FALSE;
	else if (nIDEvent == WM_SETFOCUS)
		m_bGotFocus = VARIANT_TRUE;

	return bHandled;
}
