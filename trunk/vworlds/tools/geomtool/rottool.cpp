// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// RotTool.cpp : Implementation of CRotate3DTool
#include "stdafx.h"
#include <d3drm.h>
#include <geomtool.h>
#include <math.h>
#include "GeomUndo.h"
#include "RotTool.h"
#include <vwsevent.h>
#include <vwmm_i.c>
#include <vwrnedit.h>
#include "euler.h"

/////////////////////////////////////////////////////////////////////////////
typedef CVWComPtr<IVWTransform,NULL,&IID_IVWTransform> CVWTransformPtr;
typedef CVWComPtr<IThing,NULL,&IID_IThing> CThingPtr;
typedef CVWComPtr<IVWExecuteUndo, &CLSID_VWExecuteGeomUndo, &IID_IVWExecuteUndo> CExecUndoPtr;

/////////////////////////////////////////////////////////////////////////////
extern UndoData *CreateUndoData(OperationType ot);
extern IVWUndoItem *CreateUndoItem(IVWExecuteUndo *);
extern const CLSID CLSID_Vector;
/////////////////////////////////////////////////////////////////////////////
static CComBSTR	bstrDirection("Direction");
static CComBSTR	bstrUp("Up");
static CComBSTR bstrNavigate("Navigate");
extern CComBSTR bstrAddSelection;
extern CComBSTR bstrRemoveSelection;
extern CComBSTR bstrClearSelection;
extern CComBSTR bstrAddCurrentTool;
extern CComBSTR bstrXAxisLocked;
extern CComBSTR bstrYAxisLocked;
extern CComBSTR bstrZAxisLocked;
extern CComBSTR bstrGravity;
extern CComBSTR bstrAvatar;

/////////////////////////////////////////////////////////////////////////////
#define CLAMP(x, xmin, xmax)	if ((x) < (xmin)) (x) = (xmin); else if ((x) > (xmax)) (x) = (xmax)
#define IsPressed(key)			(GetAsyncKeyState(key) < 0)
#define X_LOCK 1
#define Y_LOCK 2
#define Z_LOCK 4
#define dispidToolEvent 19L
#define dispidResetCameraPosition 12
#define fTiltScale				DEG2RAD(90.f)	// It takes at least 1 second to look up or down 90 degrees
#define fObjectNavScale			12.0f			// General speed of object navigation
#define MOUSE_TO_WORLD_FACTOR	5.0f
#define dwMaxTimeDelta			250				// maximum of 250ms (4fps) of dead reckoning 
#define PI						3.1415927F
#define DEG2RAD(d)				(d*PI/180.0F)
#define CLAMP(x, xmin, xmax)	if ((x) < (xmin)) (x) = (xmin); else if ((x) > (xmax)) (x) = (xmax)
#define PIOVER180				0.0174532F
#define PIOVER2					1.5707963F
#define PITIMES2				6.2831853F
#define SLOWKEY_SLOWFACTOR		0.2f


const IID DIID__RenderingControl = { 0x44fac781, 0xca4, 0x11d0, { 0x89, 0xa9, 0, 0xa0, 0xc9, 0x5, 0x41, 0x29 } };
/////////////////////////////////////////////////////////////////////////////
// CRotate3DTool

CRotate3DTool::CRotate3DTool()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HRESULT hr = S_OK;

	m_pVWRenderRoot = NULL;
	m_pWorld = NULL;
	m_pVector = NULL;

	m_nAxisLock = 0;
	m_pRMCameraFrame = NULL;
	m_piEditorViewControl = NULL;
	m_nCameraMode = PERSPECTIVE;
	m_bTrackingMouse = FALSE;
	m_bGotFocus = VARIANT_FALSE;
	m_bShowingNoCursor = FALSE;
	m_pRMDummyFrame = NULL;

	SetCursor(NULL, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_ROTATE2D)));

}

CRotate3DTool::~CRotate3DTool( )
{	
	DestroyTransformList();

	SAFERELEASE(m_pRMDummyFrame);
	SAFERELEASE(m_pVector);
	SAFERELEASE(m_pWorld);
	SAFERELEASE(m_pVWRenderRoot);
	SAFERELEASE(m_pRMCameraFrame);
	SAFERELEASE(m_piEditorViewControl);

}


STDMETHODIMP CRotate3DTool::Initialize(IVWUIView* pVw, IWorld* pWorld, IVWRenderRoot *pVWRenderRoot)
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

	//Store the user's camera frame
	SAFERELEASE(m_pRMCameraFrame);
	hr = pRender3D->get_Camera(&m_pRMCameraFrame);
	if (FAILED(hr) || m_pRMCameraFrame == NULL) goto EXIT_FAIL;

	//Create and store a dummy frame used to fudge out camera tilt
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

EXIT_FAIL:
	
	SAFERELEASE(pNavTool);
	SAFERELEASE(pRender3D);
	return hr;
}


STDMETHODIMP CRotate3DTool::IsValid(IVWUIView *pVw,VARIANT_BOOL * pfValid)
{
	HRESULT hr = S_OK;
	CVWTransformPtr TransPtr;
	CThingPtr ThingPtr;
	BSTR bstrReturnValue = NULL;
	VARIANT_BOOL	fLastItem = VARIANT_TRUE;
	COleVariant varProperty;
	IVWFrame *pVWFrame = NULL;
	IVWFrame *pVWParentFrame = NULL;
	IVWTransform *pTrans = NULL;
	IVector			*pvPos = NULL;
	VARIANT_BOOL vbMoveable;

	if (!pfValid )
		return E_POINTER;

	*pfValid = FALSE;

	if (!m_pControlManager)
	{
		hr = pVw->get_ControlManager( &m_pControlManager );
		if (FAILED(hr))
		{
			m_pControlManager = NULL;
			goto EXIT_FAIL;
		}
	}

	DestroyTransformList();

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

					SAFERELEASE(m_pWorld);
					hr = ThingPtr->get_World(&m_pWorld);
					if(FAILED(hr) || !m_pWorld) 	goto EXIT_FAIL;

					//Create our event vector if we don't have one already.
					if (!m_pVector)
					{
						hr = m_pWorld->CreateObjectPropertyExt(CLSID_Vector, NULL, (IObjectProperty**) &m_pVector);
						if (FAILED(hr)) goto EXIT_FAIL;
					}

					if (FAILED(hr = ThingPtr->CheckPropertySecurityExt(bstrDirection, PS_WRITE)))
					{
						goto SECURITY_ERROR;
					}

					vbMoveable = VARIANT_TRUE;
					if (SUCCEEDED(hr = ThingPtr->get_BOOL(CComBSTR("IsMoveable"), &vbMoveable)) && !vbMoveable)
					{
						goto SECURITY_ERROR;
					}

					if (FAILED(hr = ThingPtr->CheckPropertySecurityExt(bstrUp, PS_WRITE)))
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
								POSITION pos = m_TransformList.AddTail( new CTranslate3DObject( ThingPtr, TransPtr, pVWFrame ) );
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
		}  // if( VARIANT_FALSE == fLastItem )
	}
	goto EXIT_SUCCEED;

EXIT_FAIL:
	DestroyTransformList();
	*pfValid = FALSE;

EXIT_SUCCEED:
	SAFEFREESTRING(bstrReturnValue);
	SAFERELEASE(pVWFrame);
	SAFERELEASE(pvPos);

	return S_OK;
}





BOOL CRotate3DTool::OnLButtonDown(UINT nFlags, POINT pt )
{
	HRESULT hr = S_OK;

	if(m_pUIView && m_pVWRenderRoot && !m_bTrackingMouse)
	{
		m_cptLast = pt;
		m_pUIView->get_HWND(&m_hWnd);
		m_ptAnchor = pt;
		m_bTrackingMouse = VARIANT_TRUE;
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

		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

HRESULT CRotate3DTool::SetupEnvironment()
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

HRESULT CRotate3DTool::SetupTranformList()
{
	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;
	VARIANT_BOOL fValid = VARIANT_FALSE;

	if(m_pUIView)
	{ 
		SetupEnvironment();

		hr = IsValid(m_pUIView, &fValid);
		if (SUCCEEDED(hr) && !fValid)
			hr = E_FAIL;
		if(FAILED(hr)) goto EXIT_FAIL;
	}

	goto EXIT_SUCCEEDED;
EXIT_FAIL:

EXIT_SUCCEEDED:
	return hr;
}

BOOL CRotate3DTool::OnLButtonUp( UINT nFlags, POINT pt )
{
	HRESULT hr = S_OK;
	HWND hWnd;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (m_bTrackingMouse == VARIANT_TRUE)
	{
		OnDoneRotating();

		m_bTrackingMouse = VARIANT_FALSE;
		m_pUIView->get_HWND(&hWnd);
		ClientToScreen(hWnd, &m_ptAnchor);
		SetCursorPos(m_ptAnchor.x, m_ptAnchor.y);

		if (!m_bShowingNoCursor)
			ShowCursor(TRUE);
		else
		{
			m_bShowingNoCursor = FALSE;
			SetCursor(m_pUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_ROTATE2D)));
		}

		ReleaseCapture();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

HRESULT CRotate3DTool::OnDoneRotating()
{
	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;
	D3DVECTOR		vecDir, vecUp;
	IVector			*pvDir = NULL, *pvUp = NULL;
	CTranslate3DObject * pCTrans = NULL;
	IVWFrame		*pvwfParent = NULL;
	IVWFrame		*pVWFrame = NULL;
	IDirect3DRMFrame *pRMObjFrame = NULL;
	IDirect3DRMFrame *pRMParentFrame = NULL;
	IVWUndoItem		*pVWUndoItem = NULL;
	int				numRotated = 0;
	POSITION pos;

	// handle the heartbeat update
	for(pos = m_TransformList.GetHeadPosition(); pos != NULL; )
	{
		bHandled = TRUE;
		pCTrans = m_TransformList.GetNext( pos );
		if(NULL != pCTrans)
		{
			ASSERT(pCTrans->m_pTrans);
			if((pCTrans->m_pTrans) && (pCTrans->m_pThing))
			{
				hr = pCTrans->m_pTrans->QueryInterface(IID_IVWFrame, (void **) &pVWFrame);
				if (FAILED(hr) || (pVWFrame == NULL))
					goto FAIL_EXIT;

				//Get the parent frame
				if (FAILED(hr = pVWFrame->GetParent(&pvwfParent)))
					goto FAIL_EXIT;

				if (!pvwfParent)
					goto FAIL_EXIT;

				//Get the D3D Frames
				if (FAILED(hr = pVWFrame->get_Frame3D(&pRMObjFrame)))
					goto FAIL_EXIT;

				if (FAILED(hr = pvwfParent->get_Frame3D(&pRMParentFrame )))
					goto FAIL_EXIT;

				//Release now to avoid jumping
				SAFERELEASE(pvwfParent);
				SAFERELEASE(pVWFrame);

				//Get the current, newly rotated orientation
				if (FAILED(hr = pRMObjFrame->GetOrientation(pRMParentFrame, &vecDir, &vecUp)))
					goto FAIL_EXIT;

				SAFERELEASE(pRMObjFrame);
				SAFERELEASE(pRMParentFrame);

				if (FAILED(hr = pCTrans->m_pThing->get_ObjectProperty(bstrDirection, (IObjectProperty**)&pvDir)))
					goto FAIL_EXIT;
				if (FAILED(hr = pCTrans->m_pThing->get_ObjectProperty(bstrUp, (IObjectProperty**)&pvUp)))
					goto FAIL_EXIT;

				// Make sure it's sync'd up with the geometry orientation.
				pvDir->set(vecDir.x, vecDir.y, vecDir.z);
				pvUp->set(vecUp.x, vecUp.y, vecUp.z);

				SAFERELEASE(pvDir);
				SAFERELEASE(pvUp);
			}
		}
	}

FAIL_EXIT:
	SAFERELEASE(pRMParentFrame);
	SAFERELEASE(pRMObjFrame);
	SAFERELEASE(pvDir);
	SAFERELEASE(pvUp);
	SAFERELEASE(pvwfParent);
	SAFERELEASE(pVWFrame);

	return bHandled;
}

BOOL CRotate3DTool::OnKeyDown( UINT nVirtKey, LPARAM lKeyData )
{
	if (nVirtKey == VK_RIGHT || nVirtKey == VK_LEFT || nVirtKey == VK_DOWN ||  nVirtKey == VK_UP)
	{
		SetupEnvironment();
		m_bUsingKeyboard = TRUE;
	}

	return TRUE;
}

BOOL CRotate3DTool::OnKeyUp( UINT nVirtKey, LPARAM lKeyData )
{
	if (m_bUsingKeyboard && (nVirtKey == VK_RIGHT || nVirtKey == VK_LEFT || nVirtKey == VK_DOWN ||  nVirtKey == VK_UP) )
	{
		OnDoneRotating();
		m_bUsingKeyboard = FALSE;
	}

	return TRUE;
}

HRESULT CRotate3DTool::DestroyTransformList()
{
	HRESULT hr = S_OK;

	DeletePtrListElements(&m_TransformList);

	return hr;
}


D3DVECTOR TransformNormal(D3DRMMATRIX4D matrix, const D3DVECTOR& vec)
{
	// a3 = b3 * m;
	float a3[3], b3[3];
	b3[0] = vec.x;
	b3[1] = vec.y;
	b3[2] = vec.z;

	for (int i=0; i < 3; i++) 
	{
		float temp = 0.0f;

		for (int j=0; j < 3; j++)
			temp += b3[j] * matrix[j][i];

		a3[i] = temp;
	}

	D3DVECTOR result;
	result.x = a3[0];
	result.y = a3[1];
	result.z = a3[2];
	return result;
}

void ApplyParentTransformations(IDirect3DRMFrame *pFrame, D3DVECTOR* vec)
{
	IDirect3DRMFrame *pRMTmpFrame = NULL;
	IDirect3DRMFrame *pRMParentFrame = NULL;
	D3DRMMATRIX4D matrix;

	pFrame->GetTransform(matrix);
	*vec = TransformNormal(matrix, *vec);

	pRMTmpFrame = pFrame;
	pRMTmpFrame->AddRef();

	while ((pRMTmpFrame->GetParent(&pRMParentFrame) == S_OK) && pRMParentFrame)
	{
		pRMParentFrame->GetTransform(matrix);
		*vec = TransformNormal(matrix, *vec);

		SAFERELEASE(pRMTmpFrame);
		pRMTmpFrame = pRMParentFrame;
		SAFEADDREF(pRMTmpFrame);

		SAFERELEASE(pRMParentFrame);
	}

	SAFERELEASE(pRMTmpFrame);
	SAFERELEASE(pRMParentFrame);
}


float CRotate3DTool::GetXRotation(float fX, float fY, float fZ)
{
/*	D3DVECTOR rlvPreDef, rlvDir;
	D3DVALUE  valCosTheta;
	float fTheta;

	// The pre-defined angle.
	rlvPreDef.x = 0.0F;
	rlvPreDef.y	= 1.0F;
	rlvPreDef.z	= 0.0F;

	// Make a D3DVECTOR from this vector.
	rlvDir.x = fX;
	rlvDir.y = fY;
	rlvDir.z = fZ;

	D3DRMVectorNormalize(&rlvDir);

	// First need angle between the pre-defined angle and rlvDir.
	valCosTheta = D3DRMVectorDotProduct(&rlvPreDef, &rlvDir);
	fTheta = (D3DVALUE)acos(valCosTheta);

	fTheta = fTheta - PIOVER2;

	// This will always be the acute angle. Since rotation will always be in positive direction we must
	// give correct angle (possibly obtuse) for that direction.
	if (fTheta < 0.0F)
	{
		// Acute angle will not work, need obtuse angle.
		fTheta = PITIMES2 + fTheta;
	}

	fTheta = fTheta / PIOVER180;

	if (fTheta - 360.0f > -0.1f && fTheta - 360.0f < 0.1f )
		fTheta = 0.0f;
	return fTheta;
*/
	
	CComVariant fex, fey, fez;

	m_pVector->set(fX, fY, fZ);

	m_pVector->GetEulerAngles(&fex, &fey, &fez);

	return (float) (fex.fltVal * 57.2957795130823);
}
float CRotate3DTool::GetYRotation(float fX, float fY, float fZ)
{
/*	D3DVECTOR rlvPreDef, rlvDir;
	D3DVALUE  valCosTheta;
	float fTheta;

	// The pre-defined angle.
	rlvPreDef.x = 0.0F;
	rlvPreDef.y	= 0.0F;
	rlvPreDef.z	= 1.0F;

	// Make a D3DVECTOR from this vector.
	rlvDir.x = fX;
	rlvDir.y = fY;
	rlvDir.z = fZ;

	D3DRMVectorNormalize(&rlvDir);

	// First need angle between the pre-defined angle and rlvDir.
	valCosTheta = D3DRMVectorDotProduct(&rlvPreDef, &rlvDir);
	fTheta = (D3DVALUE)acos(valCosTheta);

	fTheta = fTheta - PIOVER2;

	// This will always be the acute angle. Since rotation will always be in positive direction we must
	// give correct angle (possibly obtuse) for that direction.
	if (rlvDir.x < 0.0F)
		fTheta = PI - fTheta;
	else if (rlvDir.x > 0.0F && rlvDir.z > 0.0F)
		fTheta = PITIMES2 + fTheta;

	fTheta = fTheta / PIOVER180;

	if (fTheta - 360.0f > -0.1f && fTheta - 360.0f < 0.1f )
		fTheta = 0.0f;

	return fTheta;
*/
	CComVariant fex, fey, fez;

	m_pVector->set(fX, fY, fZ);

	m_pVector->GetEulerAngles(&fex, &fey, &fez);
	return (float) (fey.fltVal * 57.2957795130823);

}

float CRotate3DTool::GetZRotation(float fX, float fY, float fZ)
{
/*	D3DVECTOR rlvPreDef, rlvDir;
	D3DVALUE  valCosTheta;
	float fTheta;

	// The pre-defined angle.
	rlvPreDef.x = 0.0F;
	rlvPreDef.y	= 0.0F;
	rlvPreDef.z	= 1.0F;

	// Make a D3DVECTOR from this vector.
	rlvDir.x = fX;
	rlvDir.y = fY;
	rlvDir.z = fZ;

	D3DRMVectorNormalize(&rlvDir);

	// First need angle between the pre-defined angle and rlvDir.
	valCosTheta = D3DRMVectorDotProduct(&rlvPreDef, &rlvDir);
	fTheta = (D3DVALUE)acos(valCosTheta);

	fTheta = fTheta - (PI / 2.0f);

	// This will always be the acute angle. Since rotation will always be in positive direction we must
	// give correct angle (possibly obtuse) for that direction.
	if (rlvDir.y < 0.0F)
		fTheta = PI - fTheta;
	else if (rlvDir.y > 0.0F && rlvDir.z > 0.0F)
		fTheta = PITIMES2 + fTheta;

	fTheta = fTheta / PIOVER180;

	if (fTheta - 360.0f > -0.1f && fTheta - 360.0f < 0.1f )
		fTheta = 0.0f;

	return fTheta;
*/

	CComVariant fex, fey, fez;

	m_pVector->set(fX, fY, fZ);

	m_pVector->GetEulerAngles(&fex, &fey, &fez);
	return (float) (fey.fltVal * 57.2957795130823);

}

BOOL CRotate3DTool::OnMouseMove(UINT nFlags, POINT pt)
{
	float fDeltaX, fDeltaY;
	RECT rect;

	// Need to make sure we are initialized
	if (!m_pVWRenderRoot || !m_pUIView || m_bTrackingMouse == VARIANT_FALSE)
		goto EXIT_FAIL;

	// If left button is down, translate all selected translatable objects.

	if (MK_LBUTTON & nFlags && ( (pt.x != m_cptLast.x ) || (pt.y != m_cptLast.y ) ) )
	{
		fDeltaX = (float) pt.x - m_cptLast.x;
		fDeltaY = (float) m_cptLast.y - pt.y;

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

		RotateSelectedObjects(fDeltaX/MOUSE_TO_WORLD_FACTOR, fDeltaY/MOUSE_TO_WORLD_FACTOR);
	}

EXIT_FAIL:
	return TRUE;
}


HRESULT CRotate3DTool::RotateSelectedObjects( float fDeltaX, float fDeltaY )
{
	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;
	float fDeltaMagnitute;
	D3DVECTOR cameraAxis, worldRotationAxis, localRotationAxis, vecDir, vecUp, vecPos;
	POSITION pos;
	LPDIRECT3DRMFRAME pD3DFrame = NULL, pD3DParentFrame = NULL;
	IVWFrame* pVWParent = NULL;
	CTranslate3DObject * pCTrans = NULL;
	static DWORD dwTime = - 1;
	DWORD dwTimeNow;
	CComBSTR bstr1, bstr2, bstr3;
	CString tmpStr;

	bHandled = TRUE;

	if (IsPressed('S'))
	{
		fDeltaX *= SLOWKEY_SLOWFACTOR;
		fDeltaY *= SLOWKEY_SLOWFACTOR;
	}

	if (m_nCameraMode == TOP)
	{
		if ( IsPressed('Y') )  //Not much to do for world-Y
		{
			cameraAxis.x = 0.0;
			cameraAxis.y = 0.0;
			cameraAxis.z = 1.0;
			fDeltaMagnitute = -fDeltaX;
		} 
		else if ( IsPressed('X') )  //Rotate on camera X
		{
			cameraAxis.x = 1.0;
			cameraAxis.y = 0.0;
			cameraAxis.z = 0.0;
			fDeltaMagnitute = fDeltaY;
		}
		else if ( IsPressed('Z') ) //Rotate on camera Z
		{
			cameraAxis.x = 0.0;
			cameraAxis.y = 1.0;
			cameraAxis.z = 0.0;
			fDeltaMagnitute = -fDeltaX;
		}
		else //Rotate on camera XY
		{
			cameraAxis.x = 0.0;
			cameraAxis.y = 0.0;
			cameraAxis.z = 1.0;
			fDeltaMagnitute = -fDeltaX;
		}
	}
	else
	{
		if ( IsPressed('Y') )  //Not much to do for world-Y
		{
			cameraAxis.x = 0.0;
			cameraAxis.y = 1.0;
			cameraAxis.z = 0.0;
			fDeltaMagnitute = -fDeltaX;
		} 
		else if ( IsPressed('X') )  //Rotate on camera X
		{
			cameraAxis.x = 1.0;
			cameraAxis.y = 0.0;
			cameraAxis.z = 0.0;
			fDeltaMagnitute = fDeltaY;
		}
		else if ( IsPressed('Z') ) //Rotate on camera Z
		{
			cameraAxis.x = 0.0;
			cameraAxis.y = 0.0;
			cameraAxis.z = 1.0;
			fDeltaMagnitute = -fDeltaX;
		}
		else //Rotate on camera XY
		{
			//If both locked we're done
			if ( (m_nAxisLock & X_LOCK) && (m_nAxisLock & Y_LOCK) )
				return S_OK;

			if ( (m_nAxisLock & X_LOCK) )
			{
				fDeltaMagnitute = fDeltaX;
				cameraAxis.x = 0.0f;
				cameraAxis.y = (float) -fabs(fDeltaX);
			}
			else if ( (m_nAxisLock & Y_LOCK) )
			{
				fDeltaMagnitute = fDeltaY;
				cameraAxis.x = (float) fabs(fDeltaY);
				cameraAxis.y = 0.0f;
			}
			else
			{
				fDeltaMagnitute = (float) sqrt(fDeltaX * fDeltaX + fDeltaY * fDeltaY);
				cameraAxis.x = fDeltaY;
				cameraAxis.y = -fDeltaX;
			}

			cameraAxis.z = 0.0f;
			D3DRMVectorNormalize(&cameraAxis);
		}
	}

	//Fudge out the camera tilt
	m_pRMCameraFrame->GetOrientation(NULL, &vecDir, &vecUp);
	if (m_nCameraMode != TOP)
	{
		vecDir.y = 0.0f;
		D3DRMVectorNormalize(&vecDir);
		vecUp.x = 0.0f;
		vecUp.y = 1.0f;
		vecUp.z = 0.0f;
	}
	m_pRMDummyFrame->SetOrientation(NULL, vecDir.x, vecDir.y, vecDir.z, vecUp.x, vecUp.y, vecUp.z);

	m_pRMCameraFrame->GetPosition(NULL, &vecPos);
	m_pRMDummyFrame->SetPosition(NULL, vecPos.x, vecPos.y, vecPos.z);

	ApplyParentTransformations(m_pRMDummyFrame, &cameraAxis);

	worldRotationAxis = cameraAxis;
	D3DRMVectorNormalize(&worldRotationAxis);

	for( pos = m_TransformList.GetHeadPosition(); pos != NULL; )
	{
		pCTrans = m_TransformList.GetNext( pos );
		if(pCTrans != NULL && pCTrans->m_pTrans != NULL)
		{
			hr = pCTrans->m_pVWFrame->GetParent(&pVWParent);
			if(FAILED( hr )) goto EXIT_FAIL;

			hr = pVWParent->get_Frame3D(&pD3DParentFrame);
			if (FAILED(hr) || (!pD3DParentFrame )) goto EXIT_FAIL;

			hr = pCTrans->m_pVWFrame->GetPositionEx(NULL, &vecPos.x, &vecPos.y, &vecPos.z);
			if(FAILED( hr )) goto EXIT_FAIL;

			hr = pCTrans->m_pVWFrame->get_Frame3D(&pD3DFrame);
			if (FAILED(hr) || (!pD3DFrame )) goto EXIT_FAIL;

			pD3DParentFrame->Transform(&localRotationAxis, &worldRotationAxis);
			D3DRMVectorNormalize(&localRotationAxis);

			pD3DFrame->SetPosition(NULL, -vecPos.x, -vecPos.y, -vecPos.z);
			pD3DFrame->AddRotation(D3DRMCOMBINE_AFTER, localRotationAxis.x, localRotationAxis.y, localRotationAxis.z, fDeltaMagnitute / 40.0f); //Rnd!
			pD3DFrame->SetPosition(NULL, vecPos.x, vecPos.y, vecPos.z);

			hr = pCTrans->m_pVWFrame->GetOrientationEx(pVWParent, &vecDir.x, &vecDir.y, &vecDir.z, 
																	&vecUp.x, &vecUp.y, &vecUp.z);
			if(FAILED( hr )) goto EXIT_FAIL;

			dwTimeNow = GetTickCount();

			if (dwTimeNow - dwTime > 200)
			{
				D3DVECTOR vecAngles;

				ComputeEulerAngles(pCTrans->m_pVWFrame, m_pVector);
				m_pVector->get(&vecAngles.x, &vecAngles.y, &vecAngles.z);

				tmpStr.Format("%.3f", vecAngles.x * 57.2957795130823);
				bstr1 = tmpStr;
				tmpStr.Format("%.3f", vecAngles.y * 57.2957795130823);
				bstr2 = tmpStr;
				tmpStr.Format("%.3f", vecAngles.z * 57.2957795130823);
				bstr3 = tmpStr;
				InvokeToolEvent(TOOLEVENT_3DOBJECTROTATED, pCTrans->m_pThing, bstr1.m_str, bstr2.m_str, bstr3.m_str, VARIANT_TRUE);

				dwTime = dwTimeNow;
			}

			SAFERELEASE(pD3DFrame);
			SAFERELEASE(pVWParent);
			SAFERELEASE(pD3DParentFrame);
		}
	}

EXIT_FAIL:
	SAFERELEASE(pD3DParentFrame);
	SAFERELEASE(pD3DFrame);
	SAFERELEASE(pVWParent);
	return bHandled;
}

HRESULT CRotate3DTool::InvokeToolEvent(int nNum, IThing* pThing, BSTR bstr1, BSTR bstr2, BSTR bstr3, VARIANT_BOOL bBubble)
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


STDMETHODIMP CRotate3DTool::Update()
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
	nVertMotion = IsPressed(VK_UP) - IsPressed(VK_DOWN) ;

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
		}
	}

	if (m_bUsingKeyboard && (nHorzMotion || nVertMotion) )
	{
		hr = RotateSelectedObjects( nHorzMotion*fObjectNavScale*fTime, nVertMotion*fObjectNavScale*fTime);
		if (FAILED(hr))	goto EXIT_FAIL;
	}

EXIT_FAIL:

	SAFERELEASE(pD3DParent);

	if (SUCCEEDED(hr))
		return TRUE;
	else
	return FALSE;
}

float CRotate3DTool::ElapsedFrameTime()
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



STDMETHODIMP CRotate3DTool::OnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient)
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

#define FLOAT_EQUAL(f_1, f_2) ( fabs(f_1 - f_2) < 0.01 )
#define FLOAT_GREATERTHAN(f_1, f_2) ( f_1 > f_2 && fabs(f_1 - f_2) > 0.01 )
#define FLOAT_LESSTHAN(f_1, f_2) ( f_1 < f_2 && fabs(f_1 - f_2) > 0.01 )


/* Convert matrix to Euler angles (in radians). */
EulerAngles Eul_FromHMatrix(HMatrix M, int order)
{
    EulerAngles ea;
    int i, j, k, h, n, s, f;

    EulGetOrd(order, i, j, k, h, n, s, f);

    if (s == EulRepYes) 
	{
		double sy = sqrt(M[i][j] * M[i][j] + M[i][k] * M[i][k]);
		if (sy > 16 * FLT_EPSILON) 
		{
			ea.x = (float) atan2(M[i][j], M[i][k]);
			ea.y = (float) atan2(sy, M[i][i]);
			ea.z = (float) atan2(M[j][i], -M[k][i]);
		} 
		else 
		{
			ea.x = (float) atan2(-M[j][k], M[j][j]);
			ea.y = (float) atan2(sy, M[i][i]);
			ea.z = 0.0f;
		}
	}
	else 
	{
		double cy = sqrt(M[i][i]*M[i][i] + M[j][i]*M[j][i]);

		if (cy > 16.0f * FLT_EPSILON) 
		{
			ea.x = (float) atan2(M[k][j], M[k][k]);
			ea.y = (float) atan2(-M[k][i], cy);
			ea.z = (float) atan2(M[j][i], M[i][i]);
		} 
		else
		{
			ea.x = (float) atan2(-M[j][k], M[j][j]);
			ea.y = (float) atan2(-M[k][i], cy);
			ea.z = 0.0f;
		}
    }

    if (n == EulParOdd) 
	{
		ea.x = -ea.x; 
		ea.y = - ea.y; 
		ea.z = -ea.z;
	}

    if (f == EulFrmR) 
	{
		float t = ea.x; 

		ea.x = ea.z; 
		ea.z = t;
	}

    ea.w = (float) order;

    return (ea);
}



STDMETHODIMP CRotate3DTool::ComputeEulerAngles(IVWFrame* pVWFrame, IVector* vecAngles)
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

BOOL CRotate3DTool::OnMessage( UINT nIDEvent, WPARAM wParam, LPARAM lParam )
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
			SetCursor(m_pUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_ROTATE2D)));
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
