// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

/************************************************
** Editing interface properties/methods for rendering control.
**
*/

#include "stdafx.h"
#include "VWRnVwCt.h"
#include "resource.h"       // main symbols
#include <menuitem.h>

#define IID_DEFINED
#include <stdtools.h>

#include "geomtool_i.c"
#include "stdtools_i.c"
#include "vwframe_i.c"
#include "menuitem_i.c"
#include "vwstudio_i.c"
#include <vwgeomsp_i.c>

#include <geomtool.h>
#include <vwgeomsp.h>

#include "cathelp.h"

#include "geomsel.h"
#include "rlutil.h"
#include "syshelp.h"
#include "tooldlg.h"

#include "optiondg.h"
#include "SelctDlg.h"

#include "vwevents.h"
#include "vwsevent.h"
#include <vwutils.h>
#include <pickdata.h>
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define JUMPONFAIL( hr, label ) \
	if (FAILED(hr)) goto label;

#define VT_POINT 73
#define VT_RECT	74
#define VT_POINT_BYREF (VT_BYREF|VT_POINT)

/*********************************************************/

typedef CVWComPtr<IPropertyList, NULL,&IID_IPropertyList> CPropertyListPtr;
typedef CVWComPtr<IThing,NULL,&IID_IThing> CThingPtr;
typedef CVWComPtr<IBoundary,NULL,&IID_IBoundary> CBoundaryPtr;
typedef CVWComPtr<IVector,NULL,&IID_IVector> CVectorPtr;
typedef CVWComPtr<IVWPickData,NULL,&IID_IVWPickData> CVWPickDataPtr;

// Field of View scaling factor.
#define FIELD_SCALE 48.0F  //6.0

////////////////////////////////////////////////////////////////////////

#define BBOX_COLOR_RED 1.0
#define BBOX_COLOR_GREEN 1.0
#define BBOX_COLOR_BLUE 1.0

#define NOMODIFY_BBOX_COLOR_RED 1.0
#define NOMODIFY_BBOX_COLOR_GREEN 0.0
#define NOMODIFY_BBOX_COLOR_BLUE 0.0

/*********************************************************/
CComBSTR bstrGeometry("Geometry");
CComBSTR bstrSpritedGeometry("SpritedGeometry");
CComBSTR bstrPosition("Position");
CComBSTR bstrDirection("Direction");
CComBSTR bstrAvatar("Avatar");
extern CComBSTR	bstrAvatarCameraPosition;
extern CComBSTR	bstrCameraPreference;
extern CComBSTR	bstrGetFrame;
extern CComBSTR	bstrProfile;
/*********************************************************
Properties for Editing
**********************************************************/


long CVWRenderViewCtrl::GetEditMode()
{
	return m_nEditingMode;
}

void CVWRenderViewCtrl::SetEditMode(long  nEditMode)
{
	IVWRender3D*			pr3D = NULL;
	IDirect3DRMViewport*	pViewport = NULL;
	IDirect3DRMDevice*		pDevice = NULL;
	COleVariant				var;
	IVWFrame*				pvwfScene = NULL, *pvwfCamera = NULL;
	HRESULT					hr = S_OK;
	IVWFrame*				pvwfRenderingContext = NULL;
	CBoundaryDisplayObject* pBDObject;
	long					nPrevEditMode;
	VARIANT_BOOL			bForceBackBuffer = VARIANT_FALSE;
	int nSize, i;

	if (!m_pRenderingContext)
		goto EXIT_FAIL;

	// set the navigation tool as current
	OnToolsNavigate();

	hr = m_pVWRenderRoot->get_Scene(&pvwfScene);
	if(FAILED(hr)) goto EXIT_FAIL;

	hr = m_pVWRenderRoot->get_Camera(&pvwfCamera);
	if(FAILED(hr)) goto EXIT_FAIL;

	if (!m_nEditingMode && nEditMode)
	{
		CreateEditingTools();

		if (m_pVWNavigationTool)
			m_pVWNavigationTool->put_CameraOnlyNavigationMode(VARIANT_TRUE);

		hr = pvwfScene->AddChild(pvwfCamera);
		if(FAILED(hr)) goto EXIT_FAIL;
	}
	else if (!nEditMode)
	{
		// Get the rendering context's loaded geometry.
		hr = m_pRenderingContext->InvokeMethodExt(bstrGetFrame, NULL, &var);
		if(FAILED(hr)) goto EXIT_FAIL;

		if (var.vt == VT_DISPATCH && var.pdispVal != NULL)
		{
			pvwfRenderingContext = (IVWFrame*)var.pdispVal;
			pvwfRenderingContext->AddRef();

			hr = pvwfRenderingContext->AddChild(pvwfCamera);
			if(FAILED(hr)) goto EXIT_FAIL;

			ResetCameraPosition();
		}

		var.Clear();

		SetViewMode(0);

		// Reset everything...
		if(NULL != m_pVWRenderRoot )
		{
			if (m_bRenderWireframe)
			{
				m_bRenderWireframe  = FALSE;

				hr = m_pVWRenderRoot->get_VWRender3D(&pr3D);
				if(FAILED(hr) || !pr3D) goto EXIT_FAIL;

				// Get the Viewport
				hr = pr3D->get_Viewport(&pViewport);
				if(FAILED(hr) || !pViewport) goto EXIT_FAIL;

				// Get the Device
				hr = pViewport->GetDevice(&pDevice);
				if(FAILED(hr) || !pDevice) goto EXIT_FAIL;

				hr = pDevice->SetQuality(D3DRMRENDER_PHONG);
				if(FAILED(hr)) goto EXIT_FAIL;
			}

			ResetCameraPosition();

		}

		if (m_pVWNavigationTool)
			m_pVWNavigationTool->put_CameraOnlyNavigationMode(VARIANT_FALSE);

		m_flZoom = (float)1.0;
		SetProjectionAndFOV();
	}

	//Store the old value locally and set to the new value
	nPrevEditMode = m_nEditingMode;
	m_nEditingMode = nEditMode;

	//Do some logic to setup or unsetup boundary editing mode
	if (m_nEditingMode == EDIT_BOUNDARIES && nPrevEditMode != EDIT_BOUNDARIES)
	{
		m_pVWRenderRoot->get_FastModeEnabled(&m_bWasUsingFastMode);
		m_pVWRenderRoot->put_FastModeEnabled(VARIANT_FALSE);

		m_nWasUsingCameraMode = m_CameraMode;

		if (m_CameraMode != TOP)
		{
			SetViewMode(TOP);
		}

		RebuildBoundaryArray();

		SetProjectionAndFOV();

		if (FAILED(m_pVWRenderRoot->put_UseBackBuffer(VARIANT_TRUE)))
		{
			VWTRACE(m_pWorld, "VWOCX", VWT_ERROR, "VWRenderViewCtrl: Failured to turn on backbuffer mode.\n");
			ASSERT(FALSE);
		}
	}
	else if (m_nEditingMode != EDIT_BOUNDARIES && nPrevEditMode == EDIT_BOUNDARIES)
	{
		//Make sure we aren't holding onto boundary display objects.
		//First wait until drawing is done.
		m_bWaitOneFrame = TRUE;
		while (m_bWaitOneFrame)
			Sleep(50);

		//Now delete the boundary display objects
		nSize = m_boundaryArray.GetSize();
		for (i = nSize - 1; i >= 0 ; i--)
		{
			pBDObject = m_boundaryArray.GetAt(i);
			m_boundaryArray.RemoveAt(i);
			delete pBDObject;
		}

		if (FAILED(m_pVWRenderRoot->put_UseBackBuffer(VARIANT_FALSE)))
		{
			VWTRACE(m_pWorld, "VWOCX", VWT_ERROR, "VWRenderViewCtrl: Failured to turn on backbuffer mode.\n");
			ASSERT(FALSE);
		}

		m_pVWRenderRoot->put_FastModeEnabled(m_bWasUsingFastMode);

		if (m_nWasUsingCameraMode != TOP)
			SetViewMode(m_nWasUsingCameraMode);
	}
	// Seem to need to force a device rebuild here on Win2K HW with forced back buffer.
	m_pVWRenderRoot->get_ForceBackBuffer(&bForceBackBuffer);
	if (bForceBackBuffer)
		m_pVWRenderRoot->RebuildDevice();

EXIT_FAIL:
	SAFERELEASE(pvwfRenderingContext);
	SAFERELEASE(pr3D);
	SAFERELEASE(pViewport);
	SAFERELEASE(pDevice);
	SAFERELEASE(pvwfScene);
	SAFERELEASE(pvwfCamera);
}



long CVWRenderViewCtrl::GetViewMode()
{
	return m_CameraMode;
}

void CVWRenderViewCtrl::SetViewMode( long nMode )
{
	HRESULT					hr = S_OK;
	IVWFrame*				pvwfScene = NULL, *pvwfCamera = NULL;
	IVWFrame*				pvwfRenderingContext = NULL;
	static CComBSTR			bstrGetFrame("GetFrame");
	COleVariant				var;
	D3DVECTOR				vecUp;

	// Set up rendering qualities for Wireframe
	if(NULL != m_pVWRenderRoot )
	{
		hr = m_pVWRenderRoot->get_Scene(&pvwfScene);
		if(FAILED(hr)) goto EXIT_FAIL;

		hr = m_pVWRenderRoot->get_Camera(&pvwfCamera);
		if(FAILED(hr)) goto EXIT_FAIL;

		if (!pvwfScene || !pvwfCamera || !m_pRenderingContext)
			goto EXIT_FAIL;

		// Set the camera orientation and position
		// Get the rendering context's loaded geometry.
		if (FAILED(hr = m_pRenderingContext->InvokeMethodExt(bstrGetFrame, NULL, &var)))
			goto EXIT_FAIL;

		if (var.vt != VT_DISPATCH || var.pdispVal == NULL)
		{
			// Getting the location's geometry should have gotten the rendering context's geometry.
			hr = E_FAIL;
			goto EXIT_FAIL;
		}
		pvwfRenderingContext = (IVWFrame*)var.pdispVal;
		pvwfRenderingContext->AddRef();

		//Save old camera direction.
		if (m_CameraMode == PERSPECTIVE)
		{
			hr = pvwfCamera->GetOrientationEx(pvwfRenderingContext, 
											&m_vecLastPerspectiveDirection.x, 
											&m_vecLastPerspectiveDirection.y, 
											&m_vecLastPerspectiveDirection.z, 
											&vecUp.x,
											&vecUp.y,
											&vecUp.z );
			if(FAILED(hr)) goto EXIT_FAIL;

			hr = pvwfCamera->GetPositionEx(pvwfRenderingContext, 
											&m_vecLastPerspectivePosition.x, 
											&m_vecLastPerspectivePosition.y, 
											&m_vecLastPerspectivePosition.z);
			if(FAILED(hr)) goto EXIT_FAIL;
		}

		m_CameraMode = (CameraMode) nMode;

		switch(nMode)
		{
			case TOP:
				hr = pvwfCamera->SetOrientationEx(NULL, 0.0, -1.0 , 0.0, -1.0, 0.0, 0.0 );
				if(FAILED(hr)) goto EXIT_FAIL;
				hr = pvwfCamera->SetPositionEx(pvwfRenderingContext, m_vecLastPerspectivePosition.x, 500.0F, m_vecLastPerspectivePosition.z);
				if(FAILED(hr)) goto EXIT_FAIL;

				// change the mode on the navigation tool
				if (m_pVWNavigationTool)
					m_pVWNavigationTool->put_TopOrthogonalMode(VARIANT_TRUE);

				m_flZoom = m_flTopZoom;

				break;
			case PERSPECTIVE:
				hr = pvwfCamera->SetOrientationEx(pvwfRenderingContext,	m_vecLastPerspectiveDirection.x,
																		0.0f, //m_vecLastPerspectiveDirection.y,
																		m_vecLastPerspectiveDirection.z,
																		0.0, 1.0, 0.0 );
				if(FAILED(hr)) goto EXIT_FAIL;

				hr = pvwfCamera->SetPositionEx(pvwfRenderingContext,		m_vecLastPerspectivePosition.x,
																		m_vecLastPerspectivePosition.y,
																		m_vecLastPerspectivePosition.z);
				if(FAILED(hr)) goto EXIT_FAIL;

				// change the mode on the navigation tool
				if (m_pVWNavigationTool)
					m_pVWNavigationTool->put_OrthogonalMode(VARIANT_FALSE);

				m_flZoom = m_flPerspectiveZoom;

				break;
			case RIGHT:
				hr = pvwfCamera->SetOrientationEx(pvwfRenderingContext, -1.0, 0.0 , 0.0, 0.0, 1.0, 0.0 );
				if(FAILED(hr)) goto EXIT_FAIL;
				hr = pvwfCamera->SetPositionEx(pvwfRenderingContext, 500.0F, 0.0F, 0.0F);
				if(FAILED(hr)) goto EXIT_FAIL;

				// change the mode on the navigation tool
				if (m_pVWNavigationTool)
					m_pVWNavigationTool->put_RightOrthogonalMode(VARIANT_TRUE);

				break;
			case FRONT:
				hr = pvwfCamera->SetOrientationEx(pvwfRenderingContext, 0.0, 0.0 , 1.0, 0.0, 1.0, 0.0 );
				if(FAILED(hr)) goto EXIT_FAIL;
				hr = pvwfCamera->SetPositionEx(pvwfRenderingContext, 0.0F, 0.0F, -500.0F);
				if(FAILED(hr)) goto EXIT_FAIL;

				m_flZoom = m_flFrontZoom;

				// change the mode on the navigation tool
				if (m_pVWNavigationTool)
					m_pVWNavigationTool->put_FrontOrthogonalMode(VARIANT_TRUE);
				break;
			default:
				break;
		}
		
		SetProjectionAndFOV();

		if (m_pWorld)
		{
			m_pWorld->FireUIEvent(NULL, CComBSTR(VWSTUDIO_VIEWMODECHANGED_EVENT_STR), CComVariant((short) m_CameraMode));
			m_pWorld->FireUIEvent(NULL, CComBSTR(VWSTUDIO_ZOOMLEVELCHANGED_EVENT_STR), CComVariant((float) m_flZoom));
		}

	}
	else
	{
		hr = E_FAIL;
		goto EXIT_FAIL;
	}

EXIT_FAIL:
	if (FAILED(hr))
		VWTRACE(m_pWorld, "VWOCX", VWT_ERROR, "VWRenderViewCtrl: Failure in SetViewMode().\n");

	SAFERELEASE(pvwfRenderingContext);
	SAFERELEASE(pvwfScene);
	SAFERELEASE(pvwfCamera);

}

long CVWRenderViewCtrl::GetViewFilter()
{
	return m_nViewFilter;
}

void CVWRenderViewCtrl::SetViewFilter( long nFilter )
{
	m_nViewFilter = nFilter;
}

BOOL CVWRenderViewCtrl::GetShowSelection()
{
	return m_bShowSelection;
}

void CVWRenderViewCtrl::SetShowSelection(BOOL bShowSelection)
{
	IPropertyList *pSelectionList = NULL;
	IObjectProperty* pObjProp = NULL;
	long nCount;
	int i;
	HRESULT hr = S_OK;

	if (m_bShowSelection && !bShowSelection)
	{
		// clear out the existing bboxes
		HandleClearSelections();
	}
	else if (!m_bShowSelection && bShowSelection)
	{
		if( !m_pControlManager  )
			CreateHelperComponents();

		if(	m_pControlManager )
		{
			hr = m_pControlManager->get_SelectionList(&pSelectionList );
			if (FAILED(hr)) goto FAIL_EXIT;

			hr = pSelectionList->get_Count(&nCount);
			if (FAILED(hr)) goto FAIL_EXIT;

			for (i = 0; i < nCount; i++)
			{
				CThingPtr cThingPtr;
				CComVariant comVariant;

				hr = pSelectionList ->get_ObjectProperty(i, &pObjProp);
				if (FAILED(hr)) goto FAIL_EXIT;

				cThingPtr = pObjProp;
				if (cThingPtr)
				{
					comVariant = pObjProp;
					HandleAddSelection(comVariant);
				}
			}
		}
	}

	m_bShowSelection = bShowSelection;

FAIL_EXIT:
	SAFERELEASE(pSelectionList);
	SAFERELEASE(pObjProp);
	
}



SCODE CVWRenderViewCtrl::LoadUIToolByProgID(LPCTSTR lpctstrProgID, LPCTSTR lpctstrName) 
{
	CComBSTR bstrProgID = lpctstrProgID,
				bstrName = lpctstrName;

	if(NULL == m_pVWUIView)
		return E_FAIL;

	return m_pVWUIView->LoadUIToolByProgID( bstrProgID, bstrName); 
}

LPDISPATCH CVWRenderViewCtrl::GetUITool(LPCTSTR lpctstrName) 
{
	CComBSTR bstrName = lpctstrName;
	IVWUITool *pUITool = NULL;

	if(NULL == m_pVWUIView)
	{
		VWTRACE(m_pWorld, "VWOCX", VWT_ERROR, "VWRenderViewCtrl: Failure in SetViewMode().\n");
		return NULL;
	}
	else
	{
		m_pVWUIView->get_LoadedUITool( bstrName, &pUITool ); 
		return pUITool;
	}
}

BOOL CVWRenderViewCtrl::IsUIToolCurrent(LPCTSTR lpctstrName) 
{
	CComBSTR bstrName = lpctstrName;
	IVWUITool *pUITool = NULL;
	CPropertyListPtr pToolList;
	BOOL bVal = FALSE;
	HRESULT hr = S_OK;
	long nCount, i;

	if(!m_pVWUIView || !m_pWorld)
		return E_FAIL;

	hr = m_pVWUIView->get_CurrentUIToolNameList(&pToolList.p);
	if (FAILED(hr)) goto FAIL_EXIT;

	if (pToolList.p)
	{
		hr = pToolList->get_Count(&nCount);
		if (FAILED(hr)) goto FAIL_EXIT;

		for (i = 0; i < nCount; i++)
		{
			CComBSTR bstrToolName;

			hr = pToolList->get_String(i, &bstrToolName.m_str);
			if (FAILED(hr)) goto FAIL_EXIT;

			if (CString(bstrToolName).CompareNoCase(lpctstrName) == 0)
				bVal = TRUE;
		}
	}

	//m_pVWUIView->get_LoadedUITool( bstrName, &pUITool ); 
	//SAFERELEASE(pUITool);

FAIL_EXIT:
	return bVal;
}

SCODE CVWRenderViewCtrl::AddCurrentUITool(LPCTSTR lpctstrName) 
{
	HRESULT hr = S_OK;
	CComBSTR bstrName = lpctstrName;

	if(NULL == m_pVWUIView || NULL == m_pWorld) return E_FAIL;

	hr = m_pVWUIView->AddCurrentUITool( bstrName); 
	if (FAILED(hr)) goto EXIT_FAIL;

	if (m_pWorld)
		m_pWorld->FireUIEvent(NULL, CComBSTR(VWSTUDIO_ADDCURRENTTOOL_EVENT_STR), CComVariant(bstrName));

	SetFocus();

EXIT_FAIL:
	return hr;
}

SCODE CVWRenderViewCtrl::CenterViewOnObject(LPDISPATCH pObject, BOOL bFollowingObject)
{
	HRESULT hr = S_OK;
	CThingPtr thingPtr, parentPtr;
	CBoundaryPtr boundaryPtr;
	CVWPickDataPtr pickDataPtr;
	IVector* pObjPos = NULL, *pObjDir = NULL;
	float fObjPosX, fObjPosY, fObjPosZ;
	float fObjDirX, fObjDirY, fObjDirZ;
	float fCamPosX, fCamPosY, fCamPosZ;
	float fCamDirX, fCamDirY, fCamDirZ;
	float fTmpX, fTmpY, fTmpZ;
	IVWFrame* pvwfCamera = NULL, *pvwfParent = NULL, *pvwfThing = NULL;
	CComVariant varFrame;
	VARIANT vX1, vY1, vZ1, vX2, vY2, vZ2;

	VariantInit(&vX1);VariantInit(&vY1);VariantInit(&vZ1);
	VariantInit(&vX2);VariantInit(&vY2);VariantInit(&vZ2);

	if (!m_pRenderingContext || !m_pWorld)
	{
		hr = E_FAIL;
		goto EXIT_FAIL;
	}

	if (!pObject) 
	{
		hr = E_INVALIDARG;
		goto EXIT_FAIL;
	}

	thingPtr = pObject;
	if (thingPtr.p == NULL)
	{
		D3DVECTOR bbox1, bbox2;

		//This must be a boundary then.
		pickDataPtr = pObject;
		if (pickDataPtr.p == NULL)
		{
			hr = E_INVALIDARG;
			goto EXIT_FAIL;
		}

		pickDataPtr->get_Boundary(&boundaryPtr.p);
		
		pickDataPtr->get_Thing(&thingPtr.p);
		
		//boundaryPtr->GetCenterExt(&fObjPosX, &fObjPosZ);
		//if (FAILED(hr)) goto EXIT_FAIL;

		hr = thingPtr->InvokeMethodExt(CComBSTR("GetFrame"), NULL, &varFrame);
		if (FAILED(hr) || varFrame.vt != VT_DISPATCH || varFrame.pdispVal == NULL) goto EXIT_FAIL;	

		pvwfThing = (IVWFrame*)varFrame.pdispVal;
		SAFEADDREF(pvwfThing);
		varFrame.Clear();

		//Use pObjPos & pObjDir to temporarily store bbox info.
		boundaryPtr->GetBoundingBoxExt(&pObjPos, &pObjDir );
		if (FAILED(hr) || !pObjPos || !pObjDir ) goto EXIT_FAIL;

		vX1.vt = vY1.vt = vZ1.vt = VT_R4;
		vX2.vt = vY2.vt = vZ2.vt = VT_R4;

		pObjPos->get(&vX1.fltVal, &vY1.fltVal, &vZ1.fltVal);
		pObjDir->get(&vX2.fltVal, &vY2.fltVal, &vZ2.fltVal);

		bbox1.x = vX1.fltVal; bbox1.y = vZ1.fltVal; bbox1.z = vY1.fltVal; 
		bbox2.x = vX2.fltVal; bbox2.y = vZ2.fltVal; bbox2.z = vY2.fltVal; 

		pvwfThing->Transform(AXIS_Y, &bbox1.x, &bbox1.y, &bbox1.z, bbox1.x, bbox1.y, bbox1.z);
		pvwfThing->Transform(AXIS_Y, &bbox2.x, &bbox2.y, &bbox2.z, bbox2.x, bbox2.y, bbox2.z);

		vX1.fltVal = bbox1.x; vZ1.fltVal = bbox1.y; vY1.fltVal = bbox1.z; 
		vX2.fltVal = bbox2.x; vZ2.fltVal = bbox2.y; vY2.fltVal = bbox2.z; 

		fObjPosX = (bbox1.x + bbox2.x) / 2.0f;
		fObjPosZ = (bbox1.z + bbox2.z) / 2.0f;
				
	}
	else
	{
		//Center on the given thing.
		hr = thingPtr->get_Container(&parentPtr.p);
		if (FAILED(hr)) goto EXIT_FAIL;

		if (!parentPtr.p)
		{
			goto EXIT_FAIL;
		}

		hr = parentPtr->InvokeMethodExt(CComBSTR("GetFrame"), NULL, &varFrame);
		if (FAILED(hr) || varFrame.vt != VT_DISPATCH || varFrame.pdispVal == NULL) goto EXIT_FAIL;	

		pvwfParent = (IVWFrame*)varFrame.pdispVal;
		SAFEADDREF(pvwfParent);
		varFrame.Clear();

		hr = thingPtr->InvokeMethodExt(CComBSTR("GetFrame"), NULL, &varFrame);
		if (FAILED(hr) || varFrame.vt != VT_DISPATCH || varFrame.pdispVal == NULL) goto EXIT_FAIL;	

		pvwfThing = (IVWFrame*)varFrame.pdispVal;
		SAFEADDREF(pvwfThing);
		varFrame.Clear();

		pvwfThing->GetBoundingBox(&vX1, &vY1, &vZ1, &vX2, &vY2, &vZ2);
		if (FAILED(hr)) goto EXIT_FAIL;

		pvwfThing->GetPositionEx(pvwfParent, &fObjPosX, &fObjPosY, &fObjPosZ);
		if (FAILED(hr)) goto EXIT_FAIL;

		hr = thingPtr->get_ObjectProperty(bstrDirection, (IObjectProperty**)&pObjDir);
		if (FAILED(hr) || !pObjDir) goto EXIT_FAIL;

		hr = pObjDir->get(&fObjDirX, &fObjDirY, &fObjDirZ);
		if (FAILED(hr)) goto EXIT_FAIL;
	}

	hr = m_pVWRenderRoot->get_Camera(&pvwfCamera);
	if (FAILED(hr) || !pvwfCamera) goto EXIT_FAIL;

	hr = pvwfCamera->GetPositionEx(pvwfParent, &fCamPosX, &fCamPosY, &fCamPosZ);
	if (FAILED(hr))	goto EXIT_FAIL;

	hr = pvwfCamera->GetOrientationEx(pvwfParent, &fCamDirX, &fCamDirY, &fCamDirZ, &fTmpX, &fTmpY, &fTmpZ);
	if (FAILED(hr))	goto EXIT_FAIL;

	if (m_CameraMode == PERSPECTIVE)
	{
		if (!bFollowingObject)
		{
			D3DVECTOR vecDir;

			SetZoomLevel(1.0f);

			vecDir.x = fObjDirX;
			vecDir.y = 0.0f;
			vecDir.z = fObjDirZ;

			D3DRMVectorNormalize(&vecDir);

			fCamPosX = fObjPosX + (vecDir.x * (float) (fabs(vX2.fltVal - vX1.fltVal) + 3) ) ;
			fCamPosY = fObjPosY;
			fCamPosZ = fObjPosZ + (vecDir.z * (float) (fabs(vZ2.fltVal - vZ1.fltVal) + 3) ) ;

			vecDir.x = -fObjDirX;
			vecDir.y = 0.0f ;//-fObjDirY;
			vecDir.z = -fObjDirZ;

			D3DRMVectorNormalize(&vecDir);

			hr = pvwfCamera->SetPositionEx(pvwfParent, fCamPosX, fCamPosY, fCamPosZ);
			if (FAILED(hr))	goto EXIT_FAIL;

			hr = pvwfCamera->SetOrientationEx(pvwfParent, vecDir.x, vecDir.y, vecDir.z, 0.0f, 1.0f, 0.0f);
			if (FAILED(hr))	goto EXIT_FAIL;
		}
		else
		{
			D3DVECTOR vec, bbox1, bbox2;
			float fMag;

			vec.x = fObjPosX - fCamPosX;
			vec.y = fObjPosY - fCamPosY;
			vec.z = fObjPosZ - fCamPosZ;
			D3DRMVectorNormalize(&vec);

			bbox1.x = vX1.fltVal; bbox1.y = vY1.fltVal; bbox1.z = vZ1.fltVal; 
			bbox2.x = vX2.fltVal; bbox2.y = vY2.fltVal; bbox2.z = vZ2.fltVal; 

			pvwfThing->Transform(AXIS_Y, &bbox1.x, &bbox1.y, &bbox1.z, bbox1.x, bbox1.y, bbox1.z);
			pvwfThing->Transform(AXIS_Y, &bbox2.x, &bbox2.y, &bbox2.z, bbox2.x, bbox2.y, bbox2.z);

			pvwfCamera->InverseTransform(AXIS_Y, &bbox1.x, &bbox1.y, &bbox1.z, bbox1.x, bbox1.y, bbox1.z);
			pvwfCamera->InverseTransform(AXIS_Y, &bbox2.x, &bbox2.y, &bbox2.z, bbox2.x, bbox2.y, bbox2.z);

			fMag =	(float) sqrt((fCamPosX - fObjPosX) * (fCamPosX - fObjPosX) + 
//					(fCamPosY - fObjPosY) * (fCamPosY - fObjPosY) + 
					(fCamPosZ - fObjPosZ) * (fCamPosZ - fObjPosZ));

			if (fMag >= 0.5f)
			{
				//If we're inside the object's boundary box, don't change the tilt.
				if (fCamPosX > bbox2.x && fCamPosX < bbox2.x &&
					fCamPosY > bbox2.y && fCamPosY < bbox1.y &&
					fCamPosZ > bbox2.z && fCamPosZ < bbox1.z)
				{
					vec.y = fCamDirY;
					D3DRMVectorNormalize(&vec);
				}

				hr = pvwfCamera->SetOrientationEx(pvwfParent, vec.x, vec.y, vec.z, 0.0f, 1.0f, 0.0f);
				if (FAILED(hr))	goto EXIT_FAIL;
			}
		}

	}
	else if (m_CameraMode == TOP)
	{
		if (!bFollowingObject)
		{
			float f1, f2, fval; 

			f1 = (float) (fabs(vX2.fltVal - vX1.fltVal)) ;
			f2 = (float) (fabs(vZ2.fltVal - vZ1.fltVal)) ;

			fval = FIELD_SCALE /  ( (f1 > f2 ? f1 : f2) );

			if (fval > 8.0f) fval = 8.0f;
			else if (fval > 4.0f) fval = 4.0f;
			else if (fval > 3.0f) fval = 3.0f;
			else if (fval > 2.0f) fval = 2.0f;
			else if (fval > 1.0f) fval = 1.0f;
			else if (fval > 0.5f) fval = 0.5f;
			else if (fval > 0.25f) fval = 0.25f;
			else fval = .125;

			SetZoomLevel(fval);
		}

		hr = pvwfCamera->SetPositionEx(pvwfParent, fObjPosX, 500.0f, fObjPosZ);
		if (FAILED(hr))	goto EXIT_FAIL;
	}
	else if (m_CameraMode == FRONT)
	{
		if (!bFollowingObject)
		{
			float f1, f2, fval; 

			f1 = (float) (fabs(vX2.fltVal - vX1.fltVal)) ;
			f2 = (float) (fabs(vY2.fltVal - vY1.fltVal)) ;

			fval = FIELD_SCALE /  ( (f1 > f2 ? f1 : f2) );

			SetZoomLevel(fval);
		}

		hr = pvwfCamera->SetPositionEx(pvwfParent, fObjPosX, fObjPosY, -500.0f);
		if (FAILED(hr))	goto EXIT_FAIL;
	}

	if (m_nEditingMode == EDIT_BOUNDARIES)
	{
		// Changes to the viewport will not be valid until renderer has rendered.
		// Wait one render cycle.
		m_bWaitOneFrame = TRUE;
		while (m_bWaitOneFrame)
			Sleep(50);
		m_bNeedUpdateBoundaryScreenCoordinates = TRUE;
	}

EXIT_FAIL:
	SAFERELEASE(pObjPos);
	SAFERELEASE(pObjDir);
	SAFERELEASE(pvwfCamera);
	SAFERELEASE(pvwfParent);
	SAFERELEASE(pvwfThing);
	
	return hr;
}

// Called from the ZoomTool
// This function SHOULD NOT redraw the viewport.
// It wants DEVICE COORDINATES.
SCODE CVWRenderViewCtrl::CenterViewOnPosition(short x, short y)
{
	HRESULT hr = S_OK;
	float fCamPosX, fCamPosY, fCamPosZ;
	float fWorldPosX, fWorldPosY, fWorldPosZ;
	IVWFrame* pvwfCamera = NULL;
	IVWRender3D*			pr3D = NULL;
	IDirect3DRMViewport*	pViewport = NULL;
	D3DRMVECTOR4D vec4d;
	D3DVECTOR vec;

	// Get the Camera
	hr = m_pVWRenderRoot->get_Camera(&pvwfCamera);
	if (FAILED(hr) || !pvwfCamera) goto EXIT_FAIL;

	// Get the VWRender3D
	hr = m_pVWRenderRoot->get_VWRender3D(&pr3D);
	if(FAILED(hr) || !pr3D) goto EXIT_FAIL;

	// Get the Viewport
	hr = pr3D->get_Viewport(&pViewport);
	if(FAILED(hr) || !pViewport) goto EXIT_FAIL;

	vec4d.x = x;
	vec4d.y = y;
	vec4d.z = 0.0f;
	vec4d.w = 1.0f;
	pViewport->InverseTransform(&vec, &vec4d);

	fWorldPosX = vec.x;
	fWorldPosY = vec.y;
	fWorldPosZ = vec.z;

	if (m_CameraMode == PERSPECTIVE)
	{
		hr = pvwfCamera->GetPositionEx(NULL, &fCamPosX, &fCamPosY, &fCamPosZ);
		if (FAILED(hr))	goto EXIT_FAIL;

		vec.x = fWorldPosX - fCamPosX;
		vec.y = fWorldPosY - fCamPosY; //0.0f;
		vec.z = fWorldPosZ - fCamPosZ;
		D3DRMVectorNormalize(&vec);

		hr = pvwfCamera->SetOrientationEx(NULL, vec.x, vec.y, vec.z, 0.0f, 1.0f, 0.0f);
		if (FAILED(hr))	goto EXIT_FAIL;
	}
	else if (m_CameraMode == TOP)
	{
		hr = pvwfCamera->SetPositionEx(NULL, fWorldPosX, 500.0f, fWorldPosZ);
		if (FAILED(hr))	goto EXIT_FAIL;

		if (m_nEditingMode == EDIT_BOUNDARIES )
		{
			// Changes to the viewport will not be valid until renderer has rendered.
			// Wait one render cycle.
			m_bWaitOneFrame = TRUE;
			while (m_bWaitOneFrame)
				Sleep(50);
			m_bNeedUpdateBoundaryScreenCoordinates = TRUE;
		}

	}
	else if (m_CameraMode == FRONT)
	{
		hr = pvwfCamera->SetPositionEx(NULL, fWorldPosX, fWorldPosY, -500.0f);
		if (FAILED(hr))	goto EXIT_FAIL;
	}

EXIT_FAIL:
	SAFERELEASE(pvwfCamera);
	SAFERELEASE(pr3D);
	SAFERELEASE(pViewport);
	
	return hr;
}


HRESULT CVWRenderViewCtrl::InitializeEditingTools()
{
	HRESULT hr = S_OK;
	IVWSelectTool *pVWSelectTool = NULL;
	IVWTranslate3DTool *pVWTransTool = NULL;
	IRotate3DTool *pVWRotTool = NULL;
	IVWScale3DTool *pVWScaleTool = NULL;
	IVWZoomTool* pVWZoomTool = NULL;
	IVWUITool *pVWUITool = NULL;

	if (m_bEditingToolsCreated)
	{
		if (FAILED(hr = m_pVWUIView->get_LoadedUITool(CComBSTR(SELECTTOOLNAME),(IVWUITool**)&pVWSelectTool )))
			goto EXIT_FAIL;

		if (FAILED(hr = m_pVWUIView->get_LoadedUITool(CComBSTR(TRANSLATETOOLNAME),(IVWUITool**)&pVWTransTool)))
			goto EXIT_FAIL;

		if (FAILED(hr = m_pVWUIView->get_LoadedUITool(CComBSTR(ROTATETOOLNAME),(IVWUITool**)&pVWRotTool)))
			goto EXIT_FAIL;
		
		if (FAILED(hr = m_pVWUIView->get_LoadedUITool(CComBSTR(SCALETOOLNAME),(IVWUITool**)&pVWScaleTool)))
			goto EXIT_FAIL;
		
		if (FAILED(hr = m_pVWUIView->get_LoadedUITool(CComBSTR(ZOOMINTOOLNAME),(IVWUITool**)&pVWZoomTool)))
			goto EXIT_FAIL;
		
		if (FAILED(hr = pVWSelectTool->Initialize(m_pVWUIView, m_pWorld, m_pVWRenderRoot)))
			goto EXIT_FAIL;

		if (FAILED(hr = pVWTransTool->Initialize(m_pVWUIView, m_pWorld, m_pVWRenderRoot)))
			goto EXIT_FAIL;

		if (FAILED(hr = pVWRotTool->Initialize(m_pVWUIView, m_pWorld, m_pVWRenderRoot)))
			goto EXIT_FAIL;

		if (FAILED(hr = pVWScaleTool->Initialize(m_pVWUIView, m_pWorld, m_pVWRenderRoot)))
			goto EXIT_FAIL;

		if (FAILED(hr = pVWZoomTool->Initialize(m_pVWUIView, m_pWorld, m_pVWRenderRoot)))
			goto EXIT_FAIL;

		hr = m_pVWUIView->get_LoadedUITool(CComBSTR(CREATEBOUNDARYTOOLNAME),(IVWUITool**)&pVWUITool);
		if(FAILED(hr)) goto EXIT_FAIL;

		hr = pVWUITool->Initialize(m_pVWUIView, m_pWorld, m_pVWRenderRoot);
		if(FAILED(hr)) goto EXIT_FAIL;

		SAFERELEASE(pVWUITool);

		hr = m_pVWUIView->get_LoadedUITool(CComBSTR(TRANSLATE2DTOOLNAME), (IVWUITool**)&pVWUITool);
		if(FAILED(hr)) goto EXIT_FAIL;

		hr = pVWUITool->Initialize(m_pVWUIView, m_pWorld, m_pVWRenderRoot);
		if(FAILED(hr)) goto EXIT_FAIL;

		SAFERELEASE(pVWUITool);

		hr = m_pVWUIView->get_LoadedUITool(CComBSTR(ROTATE2DTOOLNAME), (IVWUITool**)&pVWUITool);
		if(FAILED(hr)) goto EXIT_FAIL;

		hr = pVWUITool->Initialize(m_pVWUIView, m_pWorld, m_pVWRenderRoot);
		if(FAILED(hr)) goto EXIT_FAIL;

		SAFERELEASE(pVWUITool);

		hr = m_pVWUIView->get_LoadedUITool(CComBSTR(SCALE2DTOOLNAME), (IVWUITool**)&pVWUITool);
		if(FAILED(hr)) goto EXIT_FAIL;

		hr = pVWUITool->Initialize(m_pVWUIView, m_pWorld, m_pVWRenderRoot);
		if(FAILED(hr)) goto EXIT_FAIL;

		SAFERELEASE(pVWUITool);
	}

EXIT_FAIL:
	SAFERELEASE(pVWSelectTool);
	SAFERELEASE(pVWTransTool);
	SAFERELEASE(pVWScaleTool);
	SAFERELEASE(pVWRotTool);
	SAFERELEASE(pVWZoomTool);
	SAFERELEASE(pVWUITool);

	return hr;
}

HRESULT CVWRenderViewCtrl::CreateEditingTools()
{
	HRESULT hr = S_OK;

	if (!m_bEditingToolsCreated)
	{

		if( !m_pControlManager  )
		{
			if( FALSE == CreateHelperComponents() )
				goto ERROR_ENCOUNTERED;
		}

		hr = m_pVWUIView->LoadUIToolByCLSID(CLSID_VWSelectTool,CComBSTR(SELECTTOOLNAME));
		ASSERT(SUCCEEDED(hr));
		hr = m_pVWUIView->LoadUIToolByCLSID(CLSID_VWTranslate3DTool, CComBSTR(TRANSLATETOOLNAME));
		ASSERT(SUCCEEDED(hr));
		hr = m_pVWUIView->LoadUIToolByCLSID(CLSID_Rotate3DTool, CComBSTR(ROTATETOOLNAME));
		ASSERT(SUCCEEDED(hr));
		hr = m_pVWUIView->LoadUIToolByCLSID(CLSID_VWScale3DTool, CComBSTR(SCALETOOLNAME));
		ASSERT(SUCCEEDED(hr));
		hr = m_pVWUIView->LoadUIToolByCLSID(CLSID_VWZoomTool, CComBSTR(ZOOMINTOOLNAME));
		ASSERT(SUCCEEDED(hr));

		if (!m_pVWRenderRoot)
			goto ERROR_ENCOUNTERED;
	}

	m_bIgnoreGravity = FALSE;
	m_bIgnoreCollisions = FALSE;

	CreateBoundaryEditingTools();

	m_bEditingToolsCreated = TRUE;

	InitializeEditingTools();

ERROR_ENCOUNTERED:

	return hr;
}

HRESULT CVWRenderViewCtrl::CreateBoundaryEditingTools()
{
	HRESULT hr = S_OK;

	if(m_pVWUIView)
	{
		if( !m_pControlManager  )
		{
			if( FALSE == CreateHelperComponents() )
				goto EXIT_FAIL;
		}

		hr = m_pVWUIView->LoadUIToolByCLSID(CLSID_PolyTool,CComBSTR(CREATEBOUNDARYTOOLNAME));
		if(FAILED(hr)) goto EXIT_FAIL;

		hr = m_pVWUIView->LoadUIToolByCLSID(CLSID_VWTranslate2DTool,CComBSTR(TRANSLATE2DTOOLNAME));
		if(FAILED(hr)) goto EXIT_FAIL;

		hr = m_pVWUIView->LoadUIToolByCLSID(CLSID_VWRotate2DTool,CComBSTR(ROTATE2DTOOLNAME));
		if(FAILED(hr)) goto EXIT_FAIL;

		hr = m_pVWUIView->LoadUIToolByCLSID(CLSID_VWScale2DTool,CComBSTR(SCALE2DTOOLNAME));
		if(FAILED(hr)) goto EXIT_FAIL;

	}
	else
		hr = E_FAIL;

EXIT_FAIL:

	return hr;
}

HRESULT CVWRenderViewCtrl::HandleUIUpdate( enumVWUIEventTypes eTypes, IVWUIView*,VARIANT varHint )
{
	HRESULT hr = S_OK;
	switch(eTypes)
	{
	case VWUIT_ADDSELECTION:
		if (m_bShowSelection)
			hr = HandleAddSelection(varHint);

		if (m_nEditingMode == EDIT_BOUNDARIES && varHint.vt == VT_DISPATCH)
		{
			CVWPickDataPtr pickDataPtr;
			CBoundaryPtr boundaryPtr;

			pickDataPtr = varHint.pdispVal;
			if (pickDataPtr.p != NULL)
			{
				pickDataPtr->get_Boundary(&boundaryPtr.p);
				UpdateBoundary(boundaryPtr, BOUNDARYUPDATE_ADDSELECTION);
			}
		}
		break;
	case VWUIT_REMOVESELECTION:
		if (m_bShowSelection)
			hr = HandleRemoveSelection(varHint);

		if (m_nEditingMode == EDIT_BOUNDARIES && varHint.vt == VT_DISPATCH)
		{
			CVWPickDataPtr pickDataPtr;
			CBoundaryPtr boundaryPtr;

			pickDataPtr = varHint.pdispVal;
			if (pickDataPtr.p != NULL)
			{
				pickDataPtr->get_Boundary(&boundaryPtr.p);
				UpdateBoundary(boundaryPtr, BOUNDARYUPDATE_REMOVESELECTION);
			}
		}
		break;
	case VWUIT_TOGGLESELECTION:
		//hr = HandleToggleSelection(varHint);
		break;
	case VWUIT_CLEARSELECTIONS:
		if (m_bShowSelection)
			hr = HandleClearSelections( );

		if (m_nEditingMode == EDIT_BOUNDARIES)
		{
			UpdateBoundary(NULL, BOUNDARYUPDATE_CLEARSELECTION);
			//RebuildBoundaryArray();
		}

		break;
	case VWUIT_REFRESHOBJECTS:
		//hr = HandleRefreshObjects( );
		break;
	case VWUIT_UNDOSTACKCHANGED:
		break;
	}
	return hr;
}

HRESULT CVWRenderViewCtrl::HandleAddSelection(VARIANT varSel ) 
{
	HRESULT hr = S_OK;

	hr = CreateSelectionBox(varSel);

	return hr;
	
}
HRESULT CVWRenderViewCtrl::CreateSelectionBox(VARIANT varSel ) 
{

	HRESULT hr = S_OK;
	IThing *pThing =					NULL;
	IVWFrame *pvwf =					NULL;
	IDirect3DRMMeshBuilder *pBuilder =	NULL;
	IDirect3DRMFrame *pRMFrame =		NULL;
	IObjectProperty* pOP =				NULL;
	IVWGeometrySprite* pSpriteGeometry = NULL;
	COleVariant			var;
	CComBSTR bstrType;
	D3DRMBOX box;
	BOOL			bModify = TRUE;

	if( !m_pControlManager )
	{
		hr = E_FAIL;
		goto EXIT_FAIL;
	}

	if( varSel.vt == VT_DISPATCH )
	{   
		// get the pointer to the actual thing
		hr = varSel.pdispVal->QueryInterface(IID_IThing, (LPVOID*)&pThing);
		if (hr == E_NOINTERFACE)
		{
			hr = S_OK;
			//hr = E_INVALIDARG;
			goto EXIT_FAIL;
		}
		if (FAILED(hr)) goto EXIT_FAIL;

		// Get this thing's loaded geometry. This is the geometry we will be manipulating.
		hr = pThing->InvokeMethodExt(CComBSTR("GetFrame"), NULL, &var);

		//If there's no geometry, create bounding box.  There's nothing left to do here.
		if (VWOBJECT_E_METHODNOTEXIST == hr)
		{
			hr = S_OK;
			goto EXIT_FAIL;
		}
		if (FAILED(hr)) goto EXIT_FAIL;

		if (var.vt != VT_DISPATCH)
		{
			hr = E_FAIL;
			goto EXIT_FAIL;
		}

		pvwf = (IVWFrame*)var.pdispVal;
		if (pvwf == NULL)
			goto EXIT_FAIL;

		pvwf->AddRef();

		hr = pvwf->get_Frame3D(&pRMFrame);
		if (!pRMFrame) goto EXIT_FAIL;

		if (FAILED(hr = pThing->get_Type(&bstrType.m_str)) || CompareElements(&bstrAvatar.m_str, &bstrType.m_str))
		{
			bModify = FALSE;
		}

		if (bModify && FAILED(hr = pThing->CheckPropertySecurityExt(bstrPosition, PS_WRITE)))
		{
			bModify = FALSE;
			hr = S_OK;
		}

		hr = pThing->get_ObjectProperty(bstrGeometry, &pOP);
		if (FAILED(hr) || !pOP) goto EXIT_FAIL;
			
		pOP->QueryInterface(IID_IVWGeometrySprite, (LPVOID*)&pSpriteGeometry);
		
		//Do something different for sprites
		if (NULL != pSpriteGeometry)
		{
			box = GetSpriteBoundingBox(pRMFrame);
		}
		else  //It's geometry, build the box up.
		{
			box = GetBoundingBox(pRMFrame);
		}

		if (box.min.x == box.max.x && box.min.y == box.max.y && box.min.z == box.max.z)
			goto EXIT_FAIL;

		// VWFrame created, now create the D3D frame for the bounding box
		if (bModify)
			pBuilder = MakeWireBox(&box, m_pD3DRM, BBOX_COLOR_RED, BBOX_COLOR_GREEN, BBOX_COLOR_BLUE);
		else
			pBuilder = MakeWireBox(&box, m_pD3DRM, NOMODIFY_BBOX_COLOR_RED, NOMODIFY_BBOX_COLOR_GREEN, NOMODIFY_BBOX_COLOR_BLUE);

		if (!pBuilder) goto EXIT_FAIL;
		pBuilder->SetName(LEAVEMESHQUALITY);

		pRMFrame->AddVisual(pBuilder);

		// create the geometry selection and add it to the list
		CGeometrySelection *pGeomSel = new CGeometrySelection(pThing, pRMFrame, pBuilder);
		m_listSelected.AddSelection(pGeomSel);

		m_pControlManager->RefreshObjects((IVWUIView*) this, varSel);
	}
	else
		hr = E_INVALIDARG;

EXIT_FAIL:
//	SAFERELEASE(pvPos);
	SAFERELEASE(pSpriteGeometry);
	SAFERELEASE(pOP);
	SAFERELEASE(pThing);
	SAFERELEASE(pvwf);
	SAFERELEASE(pBuilder);
	SAFERELEASE(pRMFrame);

	return hr;
}

HRESULT CVWRenderViewCtrl::HandleRemoveSelection(VARIANT varSel ) 
{
	HRESULT hr = S_OK;
	IObjectProperty * pOP = NULL;
	IThing *pThing = NULL;

	if( !m_pControlManager )
	{
		hr = E_FAIL;
		goto EXIT_FAIL;
	}

	if( varSel.vt == VT_DISPATCH )
	{   
		hr = varSel.pdispVal->QueryInterface(IID_IObjectProperty,(LPVOID*)&pOP);
		if (hr == E_NOINTERFACE)
		{
			hr = E_INVALIDARG;
			goto EXIT_FAIL;
		}
		if( FAILED(hr)) goto EXIT_FAIL;

		// get the pointer to the actual thing
		hr = varSel.pdispVal->QueryInterface(IID_IThing, (LPVOID*)&pThing);
		if (FAILED(hr) || !pThing)
		{
			hr = E_INVALIDARG;
			goto EXIT_FAIL;
		}

		CGeometrySelection *psel = m_listSelected.GetSelection(pThing);
		if (psel != NULL)
		{
			hr = psel->m_pD3DFrame->DeleteVisual(psel->m_pBoxBuilder);
//			hr = psel->m_pVWFrame->DeleteChild(psel->m_pBoxFrame);
			m_listSelected.RemoveSelection(pThing);
//			m_pControlManager->RefreshObjects((IVWUIView*) this, varSel);
		}
	}
	else
		hr = E_INVALIDARG;

EXIT_FAIL:
	SAFERELEASE( pOP );
	SAFERELEASE(pThing);

	return hr;
}

HRESULT CVWRenderViewCtrl::HandleClearSelections(void) 
{
	HRESULT hr = S_OK;
	CComVariant varSel;

	if(	m_pControlManager )
	{
		// go through our geomselectionlist and remove the bounding boxes
		POSITION pos;

		for( pos=m_listSelected.m_list.GetHeadPosition(); pos!=0; )
		{
			CGeometrySelection *psel;
			
			psel = (CGeometrySelection *) m_listSelected.m_list.GetNext(pos);
			ASSERT( psel != NULL );
			if (psel  != NULL)
			{
				hr = psel->m_pD3DFrame->DeleteVisual(psel->m_pBoxBuilder);
				hr = m_pControlManager->RefreshObjects((IVWUIView*) this, varSel);
			}
		}

		m_listSelected.ClearList();

	}

	return hr;
}																		

HRESULT AppendSeparator(IWorld * pWorld, IPropertyList * pList )
{
	HRESULT hr = S_OK;
	IMenuItem *pSubMI = NULL;
	
	// Add The Separator
	hr = CreateMenuSeparator(pWorld, NULL, &pSubMI);
	JUMPONFAIL(hr,ERROR_ENCOUNTERED);

	// Add it to the list
	hr = pList->AddObjectProperty(pSubMI);

ERROR_ENCOUNTERED:

	SAFERELEASE( pSubMI );
	return hr;

}


HRESULT AppendSubMenu( IWorld * pWorld, BSTR bstrName, IPropertyList *pMenu, IPropertyList *pSubMenu )
{
	HRESULT hr = S_OK;
	IMenuItem *pSubMI = NULL;
					
	// Create the menu item
	hr = CreateMenuItemExt(pWorld, bstrName, MI_POPUP, NULL, NULL, NULL, NULL, &pSubMI);
	JUMPONFAIL(hr,ERROR_ENCOUNTERED);

	// Fill in the subMenu ptr
	hr = pSubMI->put_SubMenu(pSubMenu);
	JUMPONFAIL(hr,ERROR_ENCOUNTERED);

	hr = pMenu->AddObjectProperty(pSubMI);

ERROR_ENCOUNTERED:

	SAFERELEASE(pSubMI);
	return hr;
}

//Sets appropriate checks on the Tools and View submenus menu
void CVWRenderViewCtrl::UpdateMenus(CMenu *pMenu)
{
	UINT icount;
	CString strName, strEditorMenuName, strToolsMenuName, strViewsMenuName, strViewName, strTmp;
	BOOL bFound = FALSE;
	UINT i, j, jcount;
	CMenu *pSubMenu = NULL, *pGeomMenu = NULL;

	strToolsMenuName.LoadString(IDS_TOOLMENU);
	strViewsMenuName.LoadString(IDS_VIEWMENU);

	if (m_nEditingMode == EDIT_BOUNDARIES)
		strEditorMenuName.LoadString(IDS_BOUNDARY_MENU_EDITOR);
	else
		strEditorMenuName.LoadString(IDS_GEOM_MENU_EDITOR);

	icount = pMenu->GetMenuItemCount();

	//Iterate through the Geometry Editor... menu
	for (i = 0; i < icount; i++)
	{
		pMenu->GetMenuString(i, strName, MF_BYPOSITION);
	
		if (strName == strEditorMenuName)
			pGeomMenu = pMenu->GetSubMenu(i);
	}

	ASSERT(pGeomMenu);

	icount = pGeomMenu->GetMenuItemCount();

	//Iterate through the Geometry Editor... menu
	for (i = 0; i < icount; i++)
	{
		pGeomMenu->GetMenuString(i, strName, MF_BYPOSITION);
	
		if (strName == strToolsMenuName)
		{
			pSubMenu = pGeomMenu->GetSubMenu(i);
			ASSERT(pSubMenu);
			jcount = pSubMenu->GetMenuItemCount();

			//Iterate through the Tools... menu
			for (j = 0; j < jcount; j++)
			{
				pSubMenu->GetMenuString(j, strName, MF_BYPOSITION);

				//Remove the ampersand
				int nAmpPos = strName.Find("&");
				int nStrLen = strName.GetLength();
				if (nAmpPos >= 0)
				{
					strTmp = "";
					if (nAmpPos != 0)
						strTmp = strName.Left(nAmpPos);
					if (nAmpPos + 1 < nStrLen)
						strTmp += strName.Mid(nAmpPos + 1);
					strName = strTmp;
				}

				bFound = FALSE;

				// REVIEW: Probably not the best way to do this.
				if (!strName.Compare("Zoom In"))
					strName = ZOOMINTOOLNAME;
				else if (!strName.Compare("Zoom Out"))
					strName = ZOOMOUTTOOLNAME;
				else if (!strName.Compare("Move"))
				{
					if (m_nEditingMode == EDIT_BOUNDARIES)
						strName = TRANSLATE2DTOOLNAME;
					else
						strName = TRANSLATETOOLNAME;
				}
				else if (!strName.Compare("Rotate"))
				{
					if (m_nEditingMode == EDIT_BOUNDARIES)
						strName = ROTATE2DTOOLNAME;
					else
						strName = ROTATETOOLNAME;
				}
				else if (!strName.Compare("Scale"))
				{
					if (m_nEditingMode == EDIT_BOUNDARIES)
						strName = SCALE2DTOOLNAME;
					else
						strName = SCALETOOLNAME;
				}
				else if (!strName.Compare("Create Boundary"))
					strName = CREATEBOUNDARYTOOLNAME;
				else if (!strName.Compare("Move Camera"))
					strName = NAVTOOLNAME;

				if (IsUIToolCurrent(strName)) 	
					pSubMenu->CheckMenuRadioItem( 0, jcount, j, MF_BYPOSITION);
			}
		}

		if (strName == strViewsMenuName)
		{
			pSubMenu = pGeomMenu->GetSubMenu(i);
			ASSERT(pSubMenu);
			jcount = pSubMenu->GetMenuItemCount();
			bFound = FALSE;

			switch (m_CameraMode)
			{
			case PERSPECTIVE:
				strViewName.LoadString(IDS_MENU_PERSPECTIVE);
				break;
			case TOP:
				strViewName.LoadString(IDS_MENU_TOP);
				break;
			case RIGHT:
				strViewName.LoadString(IDS_MENU_RIGHT);
				break;
			case FRONT:
				strViewName.LoadString(IDS_MENU_FRONT);
				break;
			}

			//Iterate through the Views... menu
			for (j = 0; j < jcount; j++)
			{
				pSubMenu->GetMenuString(j, strName, MF_BYPOSITION);

				if (strViewName == strName)
				{
					bFound = TRUE;
					pSubMenu->CheckMenuRadioItem( 0, jcount, j, MF_BYPOSITION);
					break;
				}

				if (bFound)
					break;
			}
		}
	}
}

void CVWRenderViewCtrl::DoMenu(CPoint pt)
{
	HRESULT hr = S_OK;
	UINT Flags = 0;
	VARIANTARG varPt;
	varPt.vt = VT_POINT_BYREF;
	varPt.byref = &pt;
	COleVariant varResult;
	CThingPtr ThingPtr;
	CPropertyListPtr plistMenu;
	CMenu mStudioMenu;
	CMenu *pToolMenu;
	CString strToolsMenuName, strItemName;

	hr = HandleHitTest( varPt, 0, &varResult);
	if (FAILED(hr))
		return;

	CWnd *pwndParent;

	// get CWnd * for parent
	pwndParent = CWnd::FromHandle(m_hWnd);
	ASSERT(pwndParent != NULL);

	// munge point
	pwndParent->ClientToScreen( &pt );

	ThingPtr = varResult;

	if (ThingPtr != NULL) 
	{
		// get the menu
		CComVariant varResult;
		hr = ThingPtr->InvokeMethodExt(CComBSTR("GetMenuList"), NULL, &varResult);
		if (SUCCEEDED(hr))
		{
			plistMenu = varResult;
			if ( plistMenu != NULL )
			{
				UINT wParam = WM_USER;
				hr = m_Menu.CreatePopupFromPropList(plistMenu, &wParam);
				if (SUCCEEDED(hr))
				{
					// Append our Tools menu
					int nItems, i;
					UINT nMenuID;

					strToolsMenuName.LoadString(IDS_TOOLMENU);

					m_wpMenuItems = m_Menu.m_wLastID;

					if (m_nEditingMode == EDIT_BOUNDARIES)
						mStudioMenu.LoadMenu(IDR_BOUNDARY_POPMENUS);
					else
						mStudioMenu.LoadMenu(IDR_GEOM_POPMENUS);

					nItems = mStudioMenu.GetMenuItemCount();

					for (i = 0; i < nItems; i++)
					{
						mStudioMenu.GetMenuString( i, strItemName, MF_BYPOSITION);
						nMenuID = mStudioMenu.GetMenuItemID(i);

						if (nMenuID == -1)   //If this is a submenu, add it as a submenu.
						{
							pToolMenu = mStudioMenu.GetSubMenu(i);
//							if (strItemName == strToolsMenuName)
//								UpdateToolsMenu(pToolMenu);
					
							m_Menu.AppendMenu(MF_STRING | MF_POPUP, (UINT) pToolMenu->m_hMenu, strItemName);
						}
						else				//Otherwise add it as a menu item.
						{
							if (nMenuID == 0)
								m_Menu.AppendMenu(MF_SEPARATOR);
							else
								m_Menu.AppendMenu(MF_STRING, nMenuID, strItemName);
						}
					}
					UpdateMenus(&m_Menu);

					VERIFY(m_Menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this));

					// Peek WM_COMMAND from users menu selection off of queue and dispatch it. This
					// way we get to handle the users command right here (via WM_COMMAND handler) 
					// and can destroy menu after.
					MSG msg;
					if (::PeekMessage(&msg, m_hWnd, WM_COMMAND, WM_COMMAND, PM_REMOVE))
					{
						// yep, process right now
						::DispatchMessage(&msg);
					}
					m_Menu.DestroyMenu();
				}
			}
		}
	}
	else
	{
		m_Menu.CreatePopupMenu();

		if (m_nEditingMode == EDIT_BOUNDARIES)
			mStudioMenu.LoadMenu(IDR_BOUNDARY_POPMENUS);
		else
			mStudioMenu.LoadMenu(IDR_GEOM_POPMENUS);

		mStudioMenu.GetMenuString( 1, strItemName, MF_BYPOSITION);
		pToolMenu = mStudioMenu.GetSubMenu(1);
		m_Menu.AppendMenu(MF_STRING | MF_POPUP, (UINT) pToolMenu->m_hMenu, strItemName);
		UpdateMenus(&m_Menu);
		VERIFY(m_Menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this));

		// Peek WM_COMMAND from users menu selection off of queue and dispatch it. This
		// way we get to handle the users command right here (via WM_COMMAND handler) 
		// and can destroy menu after.
		MSG msg;
		if (::PeekMessage(&msg, m_hWnd, WM_COMMAND, WM_COMMAND, PM_REMOVE))
		{
			// yep, process right now
			::DispatchMessage(&msg);
		}
		m_Menu.DestroyMenu();
	}

//ERROR_CONDITION:
	return;
}


void CVWRenderViewCtrl::OnCommandRange(UINT nID)
{
	HRESULT hr = S_OK;
	m_Menu.OnCommandRange(nID, &hr);
}


void CVWRenderViewCtrl::OnCommandRangeUI(CCmdUI* pCmdUI)
{
	HRESULT hr;
	m_Menu.OnCommandRangeUI( pCmdUI, &hr );

	// Since this is an MFC generated command there is no way for the tools to get it.
	HandleOnMessage(WM_COMMAND, CN_UPDATE_COMMAND_UI << 16 | pCmdUI->m_nID, (LPARAM)pCmdUI);
}



void CVWRenderViewCtrl::HrError(HRESULT hr)
{
	CLSID clsid;
	LPUNKNOWN lpUnk = NULL;
	IVWErrorTool *pVWErr = NULL;
	HRESULT hresult;
	VARIANT_BOOL vBool;

	if (hr == S_OK) return;

	USES_CONVERSION;

	hresult = CLSIDFromProgID(T2OLE("stdtools.VWErrorTool.1"), &clsid);
	if (FAILED(hresult))	goto FAILED_EXIT;

	hresult = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&lpUnk);
	// if we cant even get an IUnknown, give an error
	if(FAILED(hresult)) goto FAILED_EXIT;

	hresult = lpUnk->QueryInterface(IID_IVWErrorTool, (LPVOID*)&pVWErr);
	if (FAILED(hresult)) goto FAILED_EXIT;

	if (pVWErr != NULL)
		pVWErr->DisplayError(hr, &vBool);

FAILED_EXIT:
	SAFERELEASE(lpUnk);
	SAFERELEASE(pVWErr);
	return;
}

HRESULT CVWRenderViewCtrl::HandleOnGeometryChanged(IThing* pthing)
{
	// go through our geomselectionlist and remove the bounding boxes
	POSITION pos;
	CComVariant varSel;
	HRESULT hr = S_OK;
	
	pos = m_listSelected.m_list.GetHeadPosition();

	if (pos)
	{
		for( ; pos != NULL; )
		{
			CGeometrySelection *psel;
			
			psel = (CGeometrySelection *) m_listSelected.m_list.GetNext(pos);
			ASSERT( psel != NULL );
			if (psel != NULL && psel->m_pThing == pthing)
			{
				//Clear out the old box
				hr = psel->m_pD3DFrame->DeleteVisual(psel->m_pBoxBuilder);
				if (FAILED(hr)) goto EXIT_FAIL;

				m_listSelected.RemoveSelection(pthing);

				//Create the new box
				hr = CreateSelectionBox(CComVariant(pthing));
			}
		}
		//m_pControlManager->RefreshObjects((IVWUIView*) this, varSel);
	}

EXIT_FAIL:
	return S_OK; 
};


float CVWRenderViewCtrl::GetZoomLevel() 
{
	return (float) m_flZoom;
}

void CVWRenderViewCtrl::SetZoomLevel(float newValue) 
{
	HRESULT hr = S_OK;

	//DX3 bug: Don't let the zoom level get above 2 or below .00024
	if (newValue <= 4096.0 && newValue > 0.00024f)
		m_flZoom = newValue;
	else
	{
		MessageBeep(MB_ICONASTERISK);
		goto EXIT_FAIL;
	}

//	SetViewMode(m_CameraMode);
	SetProjectionAndFOV();

	if (m_CameraMode == PERSPECTIVE)
		m_flPerspectiveZoom = m_flZoom;
	else if (m_CameraMode == TOP)
		m_flTopZoom = m_flZoom;
	else if (m_CameraMode == FRONT)
		m_flFrontZoom = m_flZoom;

	// Changes to the viewport will not be valid until renderer has rendered.
	// Wait one render cycle.
	if (m_nEditingMode == EDIT_BOUNDARIES)
	{
		m_bWaitOneFrame = TRUE;
		while (m_bWaitOneFrame)
			Sleep(50);

		m_bNeedUpdateBoundaryScreenCoordinates = TRUE;
	}

	if (m_pWorld)
		m_pWorld->FireUIEvent(NULL, CComBSTR(VWSTUDIO_ZOOMLEVELCHANGED_EVENT_STR), CComVariant((float) m_flZoom));

EXIT_FAIL:
	return;
}


void CVWRenderViewCtrl::SetProjectionAndFOV() 
{
	IVWRender3D * pr3D = NULL;
	IDirect3DRMViewport * pViewport = NULL;
	HRESULT hr = S_OK;
	int						iMaxWinSize = 0;

	// Get the VWRender3D
	hr = m_pVWRenderRoot->get_VWRender3D(&pr3D);
	if(FAILED(hr)) goto EXIT_FAIL;
	if(NULL == pr3D)
	{
		hr = E_FAIL;
		goto EXIT_FAIL;
	}

	// Get the Viewport
	hr = pr3D->get_Viewport(&pViewport);
	if(NULL == pViewport)
	{
		hr = E_FAIL;
		goto EXIT_FAIL;
	}

	// set the projection and field of view
	if (m_CameraMode != PERSPECTIVE)
	{
		hr = pViewport->SetProjection(D3DRMPROJECT_ORTHOGRAPHIC);
		if(FAILED(hr)) goto EXIT_FAIL;

		if( (0.0 == m_flZoom) || (0 == m_cyPaintInfo) || (0 == m_cxPaintInfo) || 0.0 == (FIELD_SCALE / m_flZoom) )
		{
			ASSERT(0);
			hr = E_FAIL;
			goto EXIT_FAIL;
		}

		hr = pViewport->SetField(FIELD_SCALE / m_flZoom);
		if(FAILED(hr)) goto EXIT_FAIL;
		
		// SetField uses the Max of Window height or Width for SetField()
		iMaxWinSize = ( m_cxPaintInfo > m_cyPaintInfo ) ? m_cxPaintInfo : m_cyPaintInfo;

		// FIELD_SCALE is half the height of the window in World coords.
		m_flPixelsToWorldUnit = iMaxWinSize / ( (FIELD_SCALE * 2.0F) / m_flZoom );
	}
	else
	{
		hr = pViewport->SetProjection(D3DRMPROJECT_PERSPECTIVE);
		if(FAILED(hr)) goto EXIT_FAIL;

		hr = pViewport->SetField((float) fDefaultField / m_flZoom);
		if(FAILED(hr)) goto EXIT_FAIL;

		// SetField uses the Max of Window height or Width for SetField()
		iMaxWinSize = ( m_cxPaintInfo > m_cyPaintInfo ) ? m_cxPaintInfo : m_cyPaintInfo;

		m_flPixelsToWorldUnit = iMaxWinSize / ( 1.0F / m_flZoom );

		// change the mode on the navigation tool
		if (m_pVWNavigationTool)
			m_pVWNavigationTool->put_OrthogonalMode(VARIANT_FALSE);

	}

	//m_pVWRenderRoot->Render();

EXIT_FAIL:
	SAFERELEASE(pr3D);
	SAFERELEASE(pViewport);

}


SCODE CVWRenderViewCtrl::ToolEvent(long nEvent, LPDISPATCH pObject, LPCTSTR bstr1, LPCTSTR bstr2, LPCTSTR bstr3, BOOL bBubble)
{
	int nBoundaryIndex;

	switch (nEvent)
	{
	case TOOLEVENT_BOUNDARYUPDATE:
		if (UpdateBoundary( (IBoundary*) pObject, BOUNDARYUPDATE_UPDATEINFO) == S_FALSE)
		{
			OutputDebugString("RenderView: Failed to update boundary in tool event.\n");
			//RebuildBoundaryArray();
		}
		break;
	case TOOLEVENT_BOUNDARYUPDATEBYINDEX:
		nBoundaryIndex = atoi(bstr1);

		if (UpdateBoundaryByIndex((IThing*) pObject, nBoundaryIndex) == S_FALSE)
		{
			OutputDebugString("RenderView: Failed to update boundary by index in tool event.\n");
		}
		break;
	}

	if (bBubble)
	{
		if (m_pPaintThread->m_hThread == GetCurrentThread())
		{
			FireToolEvent(nEvent, pObject, bstr1, bstr2, bstr3 );
		}
		else
		{
			ToolEventInfoStruct* pToolEventInfo;

			pToolEventInfo = new ToolEventInfoStruct;
			ZeroMemory(pToolEventInfo, sizeof ToolEventInfoStruct);

			pToolEventInfo->nEventNum = nEvent;
			pToolEventInfo->pObject = pObject; 
			strcpy(pToolEventInfo->szStr1, bstr1);
			strcpy(pToolEventInfo->szStr2, bstr2);
			strcpy(pToolEventInfo->szStr3, bstr3);

			//SendNotifyMessage(WM_FIRETOOLEVENT, (WPARAM) pToolEventInfo, 0);
			PostMessage(WM_FIRETOOLEVENT, (WPARAM) pToolEventInfo, 0);
		}
	}
	return S_OK;
}
