// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// trans3D.cpp : Implementation of CVWTranslate3DTool
#include "stdafx.h"
#include <d3drm.h>
#include <geomtool.h>
#include <math.h>
#include "GeomUndo.h"
#include "trans3D.h"
#include <vwsevent.h>
#include <vwmm_i.c>
#include <vwrnedit.h>

/////////////////////////////////////////////////////////////////////////////
typedef CVWComPtr<IThing,NULL,&IID_IThing> CThingPtr;
typedef CVWComPtr<IVWExecuteUndo, &CLSID_VWExecuteGeomUndo, &IID_IVWExecuteUndo> CExecUndoPtr;

/////////////////////////////////////////////////////////////////////////////
//Externs
extern UndoData *CreateUndoData(OperationType ot);
extern IVWUndoItem *CreateUndoItem(IVWExecuteUndo *);

extern const CLSID CLSID_Vector;

extern D3DVECTOR TransformNormal(D3DRMMATRIX4D matrix, const D3DVECTOR& vec);
extern void ApplyParentTransformations(IDirect3DRMFrame *pFrame, D3DVECTOR* vec);


/////////////////////////////////////////////////////////////////////////////
#define X_LOCK 1
#define Y_LOCK 2
#define Z_LOCK 4
#define IDEVENT 19
#define GRAVITY_CONST 9.8f
#define MOUSE_TO_WORLD_FACTOR 40.0f
#define MOUSE_TO_WORLD_FACTOR_ORBIT 150.0f
#define dispidCenterViewOnObject 18L
#define dispidToolEvent 19L
#define dispidResetCameraPosition 12L
#define dwMaxTimeDelta			250				// maximum of 250ms (4fps) of dead reckoning 
#define PI						3.1415927F
#define DEG2RAD(d)				(d*PI/180.0F)
#define CLAMP(x, xmin, xmax)	if ((x) < (xmin)) (x) = (xmin); else if ((x) > (xmax)) (x) = (xmax)
#define SLOWKEY_SLOWFACTOR		0.2f
#define SIGN(x)					(x < 0 ? 1 : 0)
#define fTiltScale				DEG2RAD(90.f)	// It takes at least 1 second to look up or down 90 degrees

#define IsPressed(key)			(GetAsyncKeyState(key) < 0)


#define POSITION_ANIM_DURATION 500
#define POSITION_ANIM_DESTROY_PADDING 50
/////////////////////////////////////////////////////////////////////////////
static CComBSTR	bstrPosition("Position");
static CComBSTR bstrGetFrame("GetFrame");
static CComBSTR bstrGetGeometryFrame("GetGeometryFrame");
static CComBSTR bstrPositionDuration("PositionDuration");
CComBSTR bstrNavigate("Navigate");
CComBSTR bstrXAxisLocked("XAxisLocked");
CComBSTR bstrYAxisLocked("YAxisLocked");
CComBSTR bstrZAxisLocked("ZAxisLocked");
CComBSTR bstrGravity("Gravity");
CComBSTR bstrAddSelection(VWSTUDIO_ADDSELECTION_EVENT_STR);
CComBSTR bstrRemoveSelection(VWSTUDIO_REMOVESELECTION_EVENT_STR);
CComBSTR bstrClearSelection(VWSTUDIO_CLEARSELECTION_EVENT_STR);
CComBSTR bstrAddCurrentTool(VWSTUDIO_ADDCURRENTTOOL_EVENT_STR);
CComBSTR bstrAvatar("Avatar");
CComBSTR bstrCameraFollowsObject("CameraFollowsObject");

const IID DIID__RenderingControl = { 0x44fac781, 0xca4, 0x11d0, { 0x89, 0xa9, 0, 0xa0, 0xc9, 0x5, 0x41, 0x29 } };

D3DVECTOR g_vecYAxis;

/////////////////////////////////////////////////////////////////////////////
// CVWTranslate3DTool

CVWTranslate3DTool::CVWTranslate3DTool()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CExecUndoPtr pExecUndo;
	HRESULT hr = S_OK;

	m_pControlManager = NULL;
	m_nAxisLock = 0;

	hr = pExecUndo.CoCreateInstance();
	if (FAILED(hr))
		m_pExecuteUndo = NULL;
	else
		m_pExecuteUndo = (IVWExecuteUndo *) pExecUndo;

	m_pExecuteUndo->AddRef();

	m_pVWRenderRoot = NULL;
	m_pWorld = NULL;
	m_pVector = NULL;
	m_pRMCameraFrame = NULL;
	m_piEditorViewControl = NULL;
	m_nCameraMode = PERSPECTIVE;
	m_bGravity = FALSE;
	m_bUsingKeyboard = m_bTrackingMouseLButton = m_bTrackingMouseRButton = VARIANT_FALSE;
	m_pRMDummyFrame = NULL;
	m_bInOrbitMode = VARIANT_FALSE;
	m_bShowingNoCursor = FALSE;
	g_vecYAxis.x = 0.0f;
	g_vecYAxis.y = 1.0f;
	g_vecYAxis.z = 0.0f;
	m_bGotFocus = VARIANT_FALSE;

	//SetCursor(NULL, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_TRANS2D)));
	SetCursor(NULL, LoadCursor(NULL, IDC_ARROW ));

}

CVWTranslate3DTool::~CVWTranslate3DTool()
{
	DestroyTransformList();

	SAFERELEASE(m_pRMDummyFrame);
	SAFERELEASE(m_piEditorViewControl);
	SAFERELEASE(m_pVector);
	SAFERELEASE(m_pWorld);
	SAFERELEASE(m_pControlManager);
	SAFERELEASE(m_pExecuteUndo);
	SAFERELEASE(m_pVWRenderRoot);
	SAFERELEASE(m_pRMCameraFrame);
}

STDMETHODIMP CVWTranslate3DTool::Initialize(IVWUIView* pVw, IWorld* pWorld, IVWRenderRoot *pVWRenderRoot)
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

	hr = m_pVWRenderRoot->get_VWRender3D(&pRender3D);
	if (FAILED(hr) || (pRender3D == NULL)) goto EXIT_FAIL;

	SAFERELEASE(m_pRMCameraFrame);
	hr = pRender3D->get_Camera(&m_pRMCameraFrame);
	if (FAILED(hr) || m_pRMCameraFrame == NULL) goto EXIT_FAIL;

	SAFERELEASE(m_pRMDummyFrame);
	hr = m_pRMCameraFrame->Clone(NULL, IID_IDirect3DRMFrame, (LPVOID*) &m_pRMDummyFrame);
	if (FAILED(hr) ) goto EXIT_FAIL;

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

STDMETHODIMP CVWTranslate3DTool::IsValid(IVWUIView *pVw,VARIANT_BOOL * pfValid)
{
	HRESULT hr = S_OK;
	CVWTransformPtr TransPtr;
	CThingPtr ThingPtr;
	BSTR bstrReturnValue = NULL;
	VARIANT_BOOL vbMoveable, fLastItem = VARIANT_TRUE;
	COleVariant varProperty;
	IVWFrame *pVWFrame = NULL;
	IVWFrame *pVWParentFrame = NULL;
	IVWTransform *pTrans = NULL;
	IVector			*pvPos = NULL;


	*pfValid = FALSE;

	ASSERT( pfValid );
	if (!pfValid )
		return E_POINTER;

	if (!m_pControlManager)
	{
		hr = pVw->get_ControlManager( &m_pControlManager );
		if (FAILED(hr))
		{
			m_pControlManager = NULL;
			goto EXIT_FAIL;
		}
	}

	if (!m_TransformList.IsEmpty())
		DeletePtrListElements(&m_TransformList);

	if(	m_pSelectionList )
	{
		hr = m_pSelectionList->get_IsEmpty( &fLastItem );
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

					//Create our event vector if we don't have one already.
					if (!m_pVector)
					{
						hr = m_pWorld->CreateObjectPropertyExt(CLSID_Vector, NULL, (IObjectProperty**) &m_pVector);
						if (FAILED(hr)) goto EXIT_FAIL;
					}

					if (FAILED(hr = ThingPtr->CheckPropertySecurityExt(bstrPosition, PS_WRITE)))
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

					hr = ThingPtr->InvokeMethodExt(bstrGetFrame, NULL, &var);
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
								POSITION pos = m_TransformList.AddTail( new CTranslate3DObject( ThingPtr, TransPtr, pVWFrame) );
							}
		    				if( FAILED(hr) ) goto EXIT_FAIL;
							SAFERELEASE(pVWParentFrame);
						}
	    				if( FAILED(hr) ) goto EXIT_FAIL;
						SAFERELEASE(pVWFrame);
					}
SECURITY_ERROR:
					var.Clear();
				}

				varProperty.Clear();
				hr = m_pSelectionList->NextItem( &bstrReturnValue, &varProperty, &fLastItem );
    			if( FAILED(hr) ) goto EXIT_FAIL;

				SAFERELEASE(pvPos);
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
	return hr;

	*pfValid = TRUE; // For now.
	return S_OK;
}

HRESULT CVWTranslate3DTool::SetupEnvironment()
{
	HRESULT hr = S_OK;
	VARIANT_BOOL vbLock;
	D3DVECTOR tmpVec, upVec;
	IThing* pGlobal = NULL;

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

HRESULT CVWTranslate3DTool::SetupTranformList()
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

BOOL CVWTranslate3DTool::OnRButtonDown( UINT nFlags, POINT point )
{ 
	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;

	if(m_pUIView && m_pVWRenderRoot )
	{ 
		m_cptLast = point;
		m_pUIView->get_HWND(&m_hWnd);
		m_ptAnchor = point;
		m_bTrackingMouseRButton = VARIANT_TRUE;
		SetCapture(m_hWnd);
		ShowCursor(FALSE);

		SetupEnvironment();

		m_nAxisLock = X_LOCK | Z_LOCK;
	}

	bHandled = TRUE;

	return bHandled;
};

BOOL CVWTranslate3DTool::OnRButtonUp( UINT nFlags, POINT point )
{
	HWND hWnd;
	HRESULT hr = S_OK;

	if (m_bTrackingMouseRButton == VARIANT_TRUE)
	{
		m_bTrackingMouseRButton = VARIANT_FALSE;
		m_pUIView->get_HWND(&hWnd);
		ClientToScreen(hWnd, &m_ptAnchor);
		//SetCursorPos(m_ptAnchor.x, m_ptAnchor.y);
		ShowCursor(TRUE);
		if (!m_bTrackingMouseLButton)
			ReleaseCapture();

		OnDoneTranslating();
	}

	return TRUE;
};


BOOL CVWTranslate3DTool::OnLButtonDown(UINT nFlags, POINT pt)
{
	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;

	if(m_pUIView && m_pVWRenderRoot )
	{ 
		m_cptLast = pt;
		m_pUIView->get_HWND(&m_hWnd);
		m_ptAnchor = pt;
		m_bTrackingMouseLButton = VARIANT_TRUE;
		SetCapture(m_hWnd);

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

	bHandled = TRUE;

	return bHandled;
}

BOOL CVWTranslate3DTool::OnLButtonUp(UINT nFlags, POINT pt )
{
	HWND hWnd;
	HRESULT hr = S_OK;

	if (m_bTrackingMouseLButton == VARIANT_TRUE)
	{
		m_bTrackingMouseLButton = VARIANT_FALSE;
		m_pUIView->get_HWND(&hWnd);
		ClientToScreen(hWnd, &m_ptAnchor);
		//SetCursorPos(m_ptAnchor.x, m_ptAnchor.y);

		if (!m_bTrackingMouseRButton)
			ReleaseCapture();

		if ( !m_bShowingNoCursor )
			ShowCursor(TRUE);
		else
		{
			m_bShowingNoCursor = FALSE;
			SetCursor(m_pUIView, LoadCursor(NULL, IDC_ARROW ));
		}

		OnDoneTranslating();
	}

	return TRUE;
}

BOOL CVWTranslate3DTool::OnKeyDown( UINT nVirtKey, LPARAM lKeyData )
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMFRAME pD3DFrame = NULL;
	D3DVECTOR vecTmp, vecPos;
	VARIANTARG varHitParams, varHitResult;

	VariantInit(&varHitParams);
	VariantInit(&varHitResult);
		
	if (nVirtKey == VK_RIGHT || nVirtKey == VK_LEFT || nVirtKey == VK_DOWN ||  nVirtKey == VK_UP)
	{
		if (!m_bUsingKeyboard)
		{
			SetupEnvironment();
		}
		m_bUsingKeyboard = TRUE;
	}
	else if (nVirtKey == 'O' && m_nCameraMode != TOP && !m_bInOrbitMode)
	{
		CTranslate3DObject * pCTrans = NULL;
		POSITION pos;

		m_bInOrbitMode = VARIANT_TRUE;

		m_fHorizontalOrbitPercent = 0.0f;
		m_fVerticalOrbitPercent = 0.0f;

		m_pRMCameraFrame->GetPosition(NULL, &vecPos);

		pos = m_TransformList.GetHeadPosition();

		if (pos)
		{
			pCTrans = m_TransformList.GetNext( pos );
			if(pCTrans != NULL && pCTrans->m_pTrans != NULL)
			{
				hr = InvokeCenterViewOnObject(pCTrans->m_pThing);
				if(FAILED( hr )) goto EXIT_FAIL;

				pCTrans->m_pVWFrame->get_Frame3D(&pD3DFrame);
				if(FAILED(hr) || !pD3DFrame) goto EXIT_FAIL;

				pD3DFrame->GetPosition(NULL, &m_vOrbitPoint);

				m_vOrbitDirection.x = vecPos.x - m_vOrbitPoint.x;
				m_vOrbitDirection.y = vecPos.y - m_vOrbitPoint.y;
				m_vOrbitDirection.z = vecPos.z - m_vOrbitPoint.z;
			}
		}
		else  //Nothing valid is selected
		{
			POINT ptMouse;

			GetCursorPos(&ptMouse);
			ScreenToClient(m_hWnd, &ptMouse);

			varHitParams.vt = VT_POINT_BYREF;
			varHitParams.byref = (LPVOID)&ptMouse;

			hr = ((IVWUIView*)m_pUIView)->HitTest(varHitParams, 0, &varHitResult);	
			if( FAILED(hr)) goto EXIT_FAIL;

			if (varHitResult.vt == VT_DISPATCH && varHitResult.pdispVal != NULL)
			{
				CComVariant var;

				hr = ((IThing*)varHitResult.pdispVal)->InvokeMethodExt(bstrGetFrame, NULL, &var);
				if (FAILED(hr) || var.vt != VT_DISPATCH || var.pdispVal == NULL) goto EXIT_FAIL;
				
				((IVWFrame*)var.pdispVal)->get_Frame3D(&pD3DFrame);
				if(FAILED(hr) || !pD3DFrame) goto EXIT_FAIL;

				pD3DFrame->GetPosition(NULL, &m_vOrbitPoint);

				m_vOrbitDirection.x = vecPos.x - m_vOrbitPoint.x;
				m_vOrbitDirection.y = vecPos.y - m_vOrbitPoint.y;
				m_vOrbitDirection.z = vecPos.z - m_vOrbitPoint.z;
			}
			else
			{
				m_pRMCameraFrame->GetOrientation(NULL, &vecPos, &vecTmp);
				m_pRMCameraFrame->GetPosition(NULL, &m_vOrbitPoint);

				m_vOrbitDirection.x = vecPos.x * 5.0f;
				m_vOrbitDirection.y = vecPos.y * 5.0f;
				m_vOrbitDirection.z = vecPos.z * 5.0f;

				m_vOrbitPoint.x += m_vOrbitDirection.x;
				m_vOrbitPoint.y += m_vOrbitDirection.y;
				m_vOrbitPoint.z += m_vOrbitDirection.z;
			}
		}

		m_fDirectionLength = D3DRMVectorModulus(&m_vOrbitDirection);

		D3DRMVectorNormalize(&m_vOrbitDirection);
 
		D3DRMVectorCrossProduct(&m_vOrbitLeft, &m_vOrbitDirection, &g_vecYAxis);
	}

EXIT_FAIL:
	SAFERELEASE(pD3DFrame );

	return TRUE;
}

BOOL CVWTranslate3DTool::OnKeyUp( UINT nVirtKey, LPARAM lKeyData )
{
	if (m_bUsingKeyboard && (nVirtKey == VK_RIGHT || nVirtKey == VK_LEFT || nVirtKey == VK_DOWN ||  nVirtKey == VK_UP) )
	{
		m_bUsingKeyboard = FALSE;
		OnDoneTranslating();
	}
	else if (nVirtKey == 'O')
	{
		m_bInOrbitMode = VARIANT_FALSE;
	}

	return TRUE;
}

HRESULT CVWTranslate3DTool::DestroyTransformList()
{
	HRESULT hr = S_OK;

	DeletePtrListElements(&m_TransformList);

	return hr;
}

HRESULT CVWTranslate3DTool::OnDoneTranslating()
{
	HRESULT hr = S_OK;
	float			fPosx, fPosy, fPosz;
	IVector			*pvPos = NULL;
	CTranslate3DObject * pCTrans = NULL;
	POSITION pos;

	// handle the heartbeat update
	for( pos = m_TransformList.GetHeadPosition(); pos != NULL; )
	{
		pCTrans = m_TransformList.GetNext( pos );
		if(NULL != pCTrans)
		{
			ASSERT(pCTrans->m_pTrans);
			if((pCTrans->m_pTrans) && (pCTrans->m_pThing))
			{
				//we are assuming there cant be an instance of IVWTransform that doesnt support
				//IVWFrame interfaces, so we can just cast it.
				IVWFrame *pvwfParent = NULL;
				hr = ((IVWFrame*)(pCTrans->m_pTrans))->GetParent(&pvwfParent);
				
				hr = ((IVWFrame*)(pCTrans->m_pTrans))->GetPositionEx(pvwfParent, &fPosx, &fPosy, &fPosz);
				if (FAILED(hr))	goto FAIL_EXIT;

				// Get our thing's world position.
				hr = pCTrans->m_pThing->get_ObjectProperty(bstrPosition, (IObjectProperty**)&pvPos);
				if (FAILED(hr))	goto FAIL_EXIT;

				hr = pvPos->set(fPosx, fPosy, fPosz);
				if (FAILED(hr))	goto FAIL_EXIT;

				if (!m_bTrackingMouseLButton && !m_bTrackingMouseRButton && !m_bUsingKeyboard)
				// Invoke Fall method to make sure this object gets to the ground.
				if (FAILED(hr = pCTrans->m_pThing->InvokeMethodExt(CComBSTR("Fall"), NULL, NULL)))
					return hr;

				SAFERELEASE(pvPos);
				SAFERELEASE(pvwfParent);
			}
		}
	}

FAIL_EXIT:
	
	SAFERELEASE(pvPos);

	return hr;
}

HRESULT CVWTranslate3DTool::TranslateSelectedObjects( float flDeltaX, float flDeltaY, BOOL bUsingRButton )
{
	HRESULT hr = S_OK;
	CComPtr<IDirect3DRMViewport> pViewport;
	CTranslate3DObject * pCTrans = NULL;
	POSITION pos;
	D3DVECTOR worldVec3d, vecPos, vecUp, vecNew, vecTmp2;
	D3DRMVECTOR4D vec4d;
	LPDIRECT3DRMFRAME pD3DFrame = NULL; //, pD3DParent = NULL;
	IVWRender3D *pRender3D = NULL;
	IVWFrame* pVWParent = NULL;
	IThing* pGlobal = NULL;
	BOOL bXAxisLocked, bYAxisLocked, bZAxisLocked;
	VARIANT varResult;
	BOOL bCentered = FALSE;
	static DWORD dwTime = - 1;
	DWORD dwTimeNow;
	CComBSTR bstr1, bstr2, bstr3;
	CString tmpStr;
	RECT rect;
	VARIANT_BOOL vbCameraFollowsObject = VARIANT_FALSE;
	GetClientRect(m_hWnd, &rect);

	if (IsPressed('S'))
	{
		flDeltaX *= SLOWKEY_SLOWFACTOR;
		flDeltaY *= SLOWKEY_SLOWFACTOR;
	}

	//If O is pressed, do orbit mode instead of movement.
	if (m_bInOrbitMode)
	{
		m_fHorizontalOrbitPercent += flDeltaX;
		m_fVerticalOrbitPercent += flDeltaY;

		//First rotate vertically, testing to see if it's a legal move.
		D3DRMVectorRotate(&vecTmp2, &m_vOrbitDirection, &m_vOrbitLeft, m_fVerticalOrbitPercent);
		if ( (SIGN(vecTmp2.x) != SIGN(m_vOrbitDirection.x) && SIGN(vecTmp2.z) != SIGN(m_vOrbitDirection.z) ) )   
		{
			//Don't let camera go all the way up or down.

			m_fVerticalOrbitPercent -= flDeltaY;
			D3DRMVectorRotate(&vecTmp2, &m_vOrbitDirection, &m_vOrbitLeft, m_fVerticalOrbitPercent);
		}
		D3DRMVectorRotate(&vecTmp2, &vecTmp2, &g_vecYAxis, m_fHorizontalOrbitPercent);

		D3DRMVectorScale(&vecNew, &vecTmp2, m_fDirectionLength);

		m_pRMCameraFrame->SetPosition(NULL, vecNew.x + m_vOrbitPoint.x, vecNew.y + m_vOrbitPoint.y, vecNew.z + m_vOrbitPoint.z);

		m_pRMCameraFrame->SetOrientation(NULL, -vecTmp2.x, -vecTmp2.y, -vecTmp2.z, 0.0f, 1.0f, 0.0f);

		//Don't move, so goto to end.
		goto EXIT_FAIL;
	}

	hr = m_pWorld->get_Global(&pGlobal);
	if(FAILED(hr)) goto EXIT_FAIL;

	//Don't break on error here
	if (FAILED(pGlobal->get_BOOL(bstrCameraFollowsObject, &vbCameraFollowsObject)))
		vbCameraFollowsObject = VARIANT_TRUE;

	hr = m_pVWRenderRoot->get_VWRender3D(&pRender3D);
	if (FAILED(hr) || (pRender3D == NULL)) goto EXIT_FAIL;

	hr = pRender3D->get_Viewport(&pViewport);
	if (FAILED(hr) || pViewport == NULL) goto EXIT_FAIL;

	if (m_nCameraMode != TOP)
	{
		bXAxisLocked = m_nAxisLock & X_LOCK;
		bYAxisLocked = m_nAxisLock & Y_LOCK;
		bZAxisLocked = m_nAxisLock & Z_LOCK;

		bYAxisLocked = TRUE;

		if (IsPressed('X'))
		{
			bXAxisLocked = FALSE; bYAxisLocked = TRUE; bZAxisLocked = TRUE;
		}
		else if (bUsingRButton || IsPressed('Y'))
		{
			bXAxisLocked = TRUE; bYAxisLocked = FALSE; bZAxisLocked = TRUE;
		}
		else if (IsPressed('Z'))
		{
			bXAxisLocked = TRUE; bYAxisLocked = TRUE; bZAxisLocked = FALSE;
		}
	}
	else
	{
		bXAxisLocked = m_nAxisLock & X_LOCK;
		bYAxisLocked = m_nAxisLock & Z_LOCK;
		bZAxisLocked = m_nAxisLock & Y_LOCK;

		bZAxisLocked = TRUE;
		if (IsPressed('X'))
		{
			bXAxisLocked = FALSE; bYAxisLocked = TRUE; bZAxisLocked = TRUE;
		}
		else if (IsPressed('Y'))
		{
			bXAxisLocked = TRUE; bYAxisLocked = TRUE; bZAxisLocked = FALSE;
		}
		else if (IsPressed('Z'))
		{
			bXAxisLocked = TRUE; bYAxisLocked = FALSE; bZAxisLocked = TRUE;
		}
	}

	m_pRMCameraFrame->GetOrientation(NULL, &vecPos, &vecUp);
	if (m_nCameraMode != TOP)
	{
		vecPos.y = 0.0f;
		D3DRMVectorNormalize(&vecPos);
		vecUp.x = 0.0f;
		vecUp.y = 1.0f;
		vecUp.z = 0.0f;
	}
	m_pRMDummyFrame->SetOrientation(NULL, vecPos.x, vecPos.y, vecPos.z, vecUp.x, vecUp.y, vecUp.z);

	m_pRMCameraFrame->GetPosition(NULL, &vecPos);
	m_pRMDummyFrame->SetPosition(NULL, vecPos.x, vecPos.y, vecPos.z);

	for( pos = m_TransformList.GetHeadPosition(); pos != NULL; )
	{
		pCTrans = m_TransformList.GetNext( pos );
		if(pCTrans != NULL && pCTrans->m_pTrans != NULL)
		{
			hr = pCTrans->m_pVWFrame->GetParent(&pVWParent);
			if(FAILED( hr )) goto EXIT_FAIL;

			hr = pCTrans->m_pVWFrame->get_Frame3D(&pD3DFrame);
			if(FAILED( hr )) goto EXIT_FAIL;

			if (!pD3DFrame)
				continue;
			
			hr = pD3DFrame->GetPosition(m_pRMDummyFrame, &worldVec3d);
			if(FAILED( hr )) goto EXIT_FAIL;

			if (m_nCameraMode != TOP)
			{
				if ( !bXAxisLocked )
					worldVec3d.x += flDeltaX;
				if ( !bYAxisLocked )
					worldVec3d.y -= flDeltaY;
				if ( !bZAxisLocked )
					worldVec3d.z -= flDeltaY;
			}
			else
			{
				if ( !bXAxisLocked )
					worldVec3d.x += flDeltaX;
				if ( !bYAxisLocked )
					worldVec3d.y -= flDeltaY;
				if ( !bZAxisLocked )
					worldVec3d.z -= flDeltaY;
			}

			if (worldVec3d.x > 1.0f || worldVec3d.z > 1.0f)
			{
				hr = pD3DFrame->SetPosition(m_pRMDummyFrame, worldVec3d.x, worldVec3d.y, worldVec3d.z);
				if(FAILED( hr )) goto EXIT_FAIL;

				hr = pCTrans->m_pVWFrame->GetPositionEx(pVWParent, &worldVec3d.x, &worldVec3d.y, &worldVec3d.z);
				if(FAILED( hr )) goto EXIT_FAIL;
			}

			VariantInit(&varResult);

			dwTimeNow = GetTickCount();

			if (dwTimeNow - dwTime > 200)
			{
				tmpStr.Format("%0.3f", worldVec3d.x);
				bstr1 = tmpStr;
				tmpStr.Format("%0.3f", worldVec3d.y);
				bstr2 = tmpStr;
				tmpStr.Format("%0.3f", worldVec3d.z);
				bstr3 = tmpStr;

				hr = InvokeToolEvent(TOOLEVENT_3DOBJECTTRANSLATED, pCTrans->m_pThing, bstr1.m_str, bstr2.m_str, bstr3.m_str, VARIANT_TRUE);
				if(FAILED( hr )) goto EXIT_FAIL;

				dwTime = dwTimeNow;
			}

			if (vbCameraFollowsObject)
			{
				pViewport->Transform(&vec4d, &worldVec3d);
				if(FAILED( hr )) goto EXIT_FAIL;

				vec4d.x /= vec4d.w;
				vec4d.y /= vec4d.w;

				if (!bCentered && (vec4d.x <= 0 || vec4d.x >= rect.right || vec4d.y <= 0 || vec4d.y >= rect.bottom ) )
				{
					bCentered = TRUE;

					hr = InvokeCenterViewOnObject(pCTrans->m_pThing);
					if(FAILED( hr )) goto EXIT_FAIL;
				}
			}
		}
	}

EXIT_FAIL:
	SAFERELEASE(pRender3D);
	SAFERELEASE(pD3DFrame);
	SAFERELEASE(pVWParent);
	SAFERELEASE(pGlobal);

	return hr;
}

BOOL CVWTranslate3DTool::OnMouseMove( UINT nFlags, POINT pt )
{
	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;
	RECT rect;
	float flDeltaX = 0.0,flDeltaY = 0.0, flDeltaZ = 0.0;

	if (!m_pUIView || !m_pVWRenderRoot || (m_bTrackingMouseLButton == VARIANT_FALSE && m_bTrackingMouseRButton == VARIANT_FALSE) )
		goto EXIT_FAIL;

	bHandled = TRUE;

	// If no delta then leave.
	if( (pt.x == m_cptLast.x ) && (pt.y == m_cptLast.y ) )
		goto EXIT_FAIL;

	//Compute delta in screeen coordinates
	flDeltaX = (float)(pt.x - m_cptLast.x);
	flDeltaY = (float)(pt.y - m_cptLast.y);

	//Get delta in world coordinates
	if (m_bInOrbitMode)
	{
		flDeltaX /= MOUSE_TO_WORLD_FACTOR_ORBIT;
		flDeltaY /= MOUSE_TO_WORLD_FACTOR_ORBIT;
	}
	else
	{
		flDeltaX /= MOUSE_TO_WORLD_FACTOR;
		flDeltaY /= MOUSE_TO_WORLD_FACTOR;
	}

	m_cptLast = pt;

	GetClientRect(m_hWnd, &rect);

	//Reset mouse position if necessary.
	if (pt.x < 0 || pt.x > rect.right || pt.y < 0 || pt.y > rect.bottom)
	{
		pt.x = rect.right / 2;
		pt.y = rect.bottom / 2;
		m_cptLast = pt;
		ClientToScreen(m_hWnd, &pt);
		SetCursorPos(pt.x, pt.y);
	}

	hr = TranslateSelectedObjects( flDeltaX, flDeltaY, (nFlags & MK_RBUTTON) );
	if (FAILED(hr)) goto EXIT_FAIL;

EXIT_FAIL:

	return bHandled;

}

HRESULT CVWTranslate3DTool::InvokeToolEvent(int nNum, IThing* pThing, BSTR bstr1, BSTR bstr2, BSTR bstr3, VARIANT_BOOL bBubble)
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

HRESULT CVWTranslate3DTool::InvokeCenterViewOnObject(IThing* pThing)
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

HRESULT CVWTranslate3DTool::GetObjectCenter(D3DVECTOR& center, IVWFrame * pTrans)
{
	HRESULT hr = S_OK;
	IVWFrame * pScene = NULL;

	center.x = center.y = center.z = 0.0f;

	hr = pTrans->GetScene(&pScene);
	if (FAILED(hr) || (pScene == NULL))
		goto FAIL_EXIT;
	
	D3DVECTOR pos;
	pTrans->GetPositionEx(pScene, &pos.x, &pos.y, &pos.z);

//	D3DRMVectorAdd(&center, &center, &pos);
	center = pos;

FAIL_EXIT:
	SAFERELEASE(pScene);

	return hr;
}

STDMETHODIMP CVWTranslate3DTool::Update()
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
		if (m_nCameraMode == TOP)
		{
			hr = m_pRMCameraFrame->GetParent(&pD3DParent);
			if (FAILED(hr) || !pD3DParent)	goto EXIT_FAIL;

			hr = m_pRMCameraFrame->GetOrientation(pD3DParent, &vDir, &vUp);
			if (FAILED(hr))	goto EXIT_FAIL;

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
		hr = TranslateSelectedObjects(nHorzMotion*fTime, nVertMotion*fTime, FALSE );
		if (FAILED(hr))	goto EXIT_FAIL;
	}

EXIT_FAIL:

	SAFERELEASE(pD3DParent);

	if (SUCCEEDED(hr))
		return TRUE;
	else
		return FALSE;
}

float CVWTranslate3DTool::ElapsedFrameTime()
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



STDMETHODIMP CVWTranslate3DTool::OnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient)
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

	return S_OK;
};

BOOL CVWTranslate3DTool::OnMessage( UINT nIDEvent, WPARAM wParam, LPARAM lParam )
{
	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;

	if ((nIDEvent == WM_KILLFOCUS || nIDEvent == WM_ENTERMENULOOP) &&
		(m_bTrackingMouseLButton || m_bTrackingMouseRButton) && m_pUIView)
	{
		// Set the mouse cursor to where it would be if we hadn't been mucking with it.
		HWND hWnd;
		m_pUIView->get_HWND(&hWnd);
		ClientToScreen(hWnd, &m_ptAnchor);
		//SetCursorPos(m_ptAnchor.x, m_ptAnchor.y);

		if (m_bTrackingMouseLButton)
		{
			m_bTrackingMouseLButton = VARIANT_FALSE;
			if (!m_bShowingNoCursor)
				ShowCursor(TRUE);
			else
			{
				m_bShowingNoCursor = FALSE;
				SetCursor(m_pUIView, LoadCursor(NULL, IDC_ARROW ));
			}
		}

		if (m_bTrackingMouseRButton)
		{
			m_bTrackingMouseRButton = VARIANT_FALSE;
			ShowCursor(TRUE);
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
