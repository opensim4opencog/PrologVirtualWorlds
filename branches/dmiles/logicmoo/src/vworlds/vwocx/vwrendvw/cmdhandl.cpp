// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

/************************************************
** Command handlers for renderview control.  Mostly
** these call functions in edit.cpp.
*/

#include "stdafx.h"
#include "VWRnVwCt.h"
#include "resource.h"       // main symbols
#include "syshelp.h"
#include "vwevents.h"
#include "optiondg.h"
#include "SelctDlg.h"


/*********************************************************/

typedef CVWComPtr<IPropertyList, NULL,&IID_IPropertyList> CPropertyListPtr;
typedef CVWComPtr<IThing,NULL,&IID_IThing> CThingPtr;

void CVWRenderViewCtrl::OnToolsHelp()
{
	HRESULT hr = S_OK;
	IPropertyList *pPropList = NULL;

	if (m_pWorld)
	{
		if (FAILED(hr = CreatePropertyList(m_pWorld, &pPropList)))
			goto failexit;

		// make list an orphan
		/* hr = */ pPropList->put_World(NULL);

		if (FAILED(hr = pPropList->AddString(CComBSTR("Authoring"))))
			goto failexit;

		if (FAILED(hr = pPropList->AddString(CComBSTR("Geometry Editor"))))
			goto failexit;

		// do we want to provide tool specific context?

		m_pWorld->FireUIEvent(NULL, CComBSTR(VW_HELP_EVENT_STR), CComVariant((IDispatch *) pPropList));
	}

failexit:
	SAFERELEASE(pPropList);
}
void CVWRenderViewCtrl::OnToolsNavigate()
{
	HRESULT hr = S_OK;

	// turn on Select Tool
	if(m_pVWUIView)
	{
		if (!IsUIToolCurrent(NAVTOOLNAME))
		{
			hr = AddCurrentUITool(NULL);
			if(FAILED(hr)) goto EXIT_FAIL;
			hr = AddCurrentUITool(CString(NAVTOOLNAME));
			if (FAILED(hr)) goto EXIT_FAIL;
			hr = AddCurrentUITool(CString(PICKTOOLNAME));
			if (FAILED(hr)) goto EXIT_FAIL;

			// And finally (re)enable navigation.
			//if (FAILED(hr = m_pVWNavigationTool->put_AllNavigationDisabled(VARIANT_FALSE)))
			//	goto EXIT_FAIL;

			SetFocus();
			HandleOnMessage(WM_SETFOCUS, NULL, NULL );

			m_bNavigationToolIsCurrentlySelected = TRUE;
		}
	}

EXIT_FAIL:

	return;
}

void CVWRenderViewCtrl::OnUpdateToolsNavigate(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CVWRenderViewCtrl::OnToolsSelect()
{
	HRESULT hr = S_OK;
	IVWSelectTool* pselTool = NULL;

	// turn on Select Tool
	if(m_pVWUIView)
	{
		hr = AddCurrentUITool(NULL);
		if(FAILED(hr)) goto EXIT_FAIL;
		hr = AddCurrentUITool(SELECTTOOLNAME);
		if(FAILED(hr))
		{
			hr = m_pVWUIView->LoadUIToolByCLSID( CLSID_VWSelectTool,CComBSTR(SELECTTOOLNAME));
			if(FAILED( hr )) goto EXIT_FAIL;
			hr = AddCurrentUITool( SELECTTOOLNAME);
			if(FAILED( hr )) goto EXIT_FAIL;
		}

		// And finally disable navigation.
		//if (FAILED(hr = m_pVWNavigationTool->put_AllNavigationDisabled(VARIANT_TRUE)))
		//	goto EXIT_FAIL;

		m_bNavigationToolIsCurrentlySelected = FALSE;

		hr = m_pWorld->get_Tool(CComBSTR("Select"), (IUnknown**) &pselTool);
		if(FAILED(hr)) goto EXIT_FAIL;

		ASSERT(pselTool);
		if (pselTool)
		{
			pselTool->put_DeluxeSelection(VARIANT_TRUE);
			pselTool->put_VertexSelection(VARIANT_FALSE);
		}
	}

EXIT_FAIL:
	SAFERELEASE(pselTool);
	return;
}

void CVWRenderViewCtrl::OnUpdateToolsSelect(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CVWRenderViewCtrl::OnToolsTranslate()
{
	HRESULT hr = S_OK;
	IVWSelectTool* pselTool = NULL;
	IPropertyList* pToolNameList = NULL;
	CString strToolName;
	const CLSID* clsidTool = NULL;

	// turn on Translate Tool
	if(m_pVWUIView)
	{
		if (m_nEditingMode == EDIT_BOUNDARIES)
		{
			strToolName = TRANSLATE2DTOOLNAME;
			clsidTool = &CLSID_VWTranslate2DTool ;
		}
		else
		{
			strToolName = TRANSLATETOOLNAME;
			clsidTool = &CLSID_VWTranslate3DTool;
		}


		if (!IsUIToolCurrent(strToolName))
		{
			hr = AddCurrentUITool(NULL);
			if(FAILED(hr)) goto EXIT_FAIL;

			hr = AddCurrentUITool(SELECTTOOLNAME);
			if(FAILED(hr))
			{
				hr = m_pVWUIView->LoadUIToolByCLSID( CLSID_VWSelectTool,CComBSTR(SELECTTOOLNAME));
				if(FAILED( hr )) goto EXIT_FAIL;
				hr = AddCurrentUITool( SELECTTOOLNAME);
				if(FAILED( hr )) goto EXIT_FAIL;
			}

			hr = AddCurrentUITool(strToolName);
			if(FAILED(hr))
			{
				hr = m_pVWUIView->LoadUIToolByCLSID( *clsidTool, CComBSTR(strToolName) );
				if(FAILED( hr )) goto EXIT_FAIL;
				hr = AddCurrentUITool( strToolName );
				if(FAILED( hr )) goto EXIT_FAIL;
			}

			m_bNavigationToolIsCurrentlySelected = FALSE;
			SetFocus();
			HandleOnMessage(WM_SETFOCUS, NULL, NULL );

			//Set the correct mode in the select tool
			hr = m_pWorld->get_Tool(CComBSTR("Select"), (IUnknown**) &pselTool);
			if(FAILED(hr)) goto EXIT_FAIL;

			ASSERT(pselTool);
			if (pselTool)
			{
				pselTool->put_DeluxeSelection(VARIANT_FALSE);
				pselTool->put_VertexSelection(VARIANT_TRUE);
			}
		}
	}
EXIT_FAIL:
	SAFERELEASE(pselTool);
	return;
}

void CVWRenderViewCtrl::OnUpdateToolsTranslate(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CVWRenderViewCtrl::OnToolsRotate()
{
	HRESULT hr = S_OK;
	IVWSelectTool* pselTool = NULL;
	CString strToolName;
	const CLSID* clsidTool = NULL;

	// turn on Rotate Tool
	if(m_pVWUIView)
	{
		if (m_nEditingMode == EDIT_BOUNDARIES)
		{
			strToolName = ROTATE2DTOOLNAME;
			clsidTool = &CLSID_VWRotate2DTool;
		}
		else
		{
			strToolName = ROTATETOOLNAME;
			clsidTool = &CLSID_Rotate3DTool;
		}

		if (!IsUIToolCurrent(strToolName))
		{
			hr = AddCurrentUITool(NULL);
			if(FAILED(hr)) goto EXIT_FAIL;

			hr = AddCurrentUITool(SELECTTOOLNAME);
			if(FAILED(hr))
			{
				hr = m_pVWUIView->LoadUIToolByCLSID( CLSID_VWSelectTool,CComBSTR(SELECTTOOLNAME));
				if(FAILED( hr )) goto EXIT_FAIL;
				hr = AddCurrentUITool( SELECTTOOLNAME);
				if(FAILED( hr )) goto EXIT_FAIL;
			}

			// And finally disable navigation.
			//if (FAILED(hr = m_pVWNavigationTool->put_AllNavigationDisabled(VARIANT_TRUE)))
			//	goto EXIT_FAIL;

			hr = AddCurrentUITool(strToolName);
			if(FAILED(hr))
			{
				hr = m_pVWUIView->LoadUIToolByCLSID( *clsidTool, CComBSTR(strToolName) );
				if(FAILED( hr )) goto EXIT_FAIL;
				hr = AddCurrentUITool( strToolName );
				if(FAILED( hr )) goto EXIT_FAIL;
			}
			m_bNavigationToolIsCurrentlySelected = FALSE;
			SetFocus();
			HandleOnMessage(WM_SETFOCUS, NULL, NULL );

			//Set the correct mode in the select tool
			hr = m_pWorld->get_Tool(CComBSTR("Select"), (IUnknown**) &pselTool);
			if(FAILED(hr)) goto EXIT_FAIL;

			ASSERT(pselTool);
			if (pselTool)
			{
				pselTool->put_DeluxeSelection(VARIANT_FALSE);
				pselTool->put_VertexSelection(VARIANT_FALSE);
			}
		}
	}

EXIT_FAIL:
	SAFERELEASE(pselTool);
	return;
}

void CVWRenderViewCtrl::OnUpdateToolsRotate(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CVWRenderViewCtrl::OnToolsScale()
{
	HRESULT hr = S_OK;
	IVWSelectTool* pselTool = NULL;
	CString strToolName;
	const CLSID* clsidTool = NULL;

	// turn on Select Tool
	if(m_pVWUIView)
	{
		if (m_nEditingMode == EDIT_BOUNDARIES)
		{
			strToolName = SCALE2DTOOLNAME;
			clsidTool = &CLSID_VWScale2DTool;
		}
		else
		{
			strToolName = SCALETOOLNAME;
			clsidTool = &CLSID_VWScale3DTool;
		}

		if (!IsUIToolCurrent(strToolName))
		{
			hr = AddCurrentUITool(NULL);
			if(FAILED(hr)) goto EXIT_FAIL;

			hr = AddCurrentUITool(SELECTTOOLNAME);
			if(FAILED(hr))
			{
				hr = m_pVWUIView->LoadUIToolByCLSID( CLSID_VWSelectTool,CComBSTR(SELECTTOOLNAME));
				if(FAILED( hr )) goto EXIT_FAIL;
				hr = AddCurrentUITool( SELECTTOOLNAME);
				if(FAILED( hr )) goto EXIT_FAIL;
			}

			hr = AddCurrentUITool(strToolName);
			if(FAILED(hr))
			{
				hr = m_pVWUIView->LoadUIToolByCLSID( *clsidTool, CComBSTR(strToolName) );
				if(FAILED( hr )) goto EXIT_FAIL;
				hr = AddCurrentUITool( strToolName );
				if(FAILED( hr )) goto EXIT_FAIL;
			}

			// And finally disable navigation.
			//if (FAILED(hr = m_pVWNavigationTool->put_AllNavigationDisabled(VARIANT_TRUE)))
			//	goto EXIT_FAIL;

			m_bNavigationToolIsCurrentlySelected = FALSE;

			SetFocus();
			HandleOnMessage(WM_SETFOCUS, NULL, NULL );

			//Set the correct mode in the select tool
			hr = m_pWorld->get_Tool(CComBSTR("Select"), (IUnknown**) &pselTool);
			if(FAILED(hr)) goto EXIT_FAIL;

			ASSERT(pselTool);
			if (pselTool)
			{
				pselTool->put_DeluxeSelection(VARIANT_FALSE);
				pselTool->put_VertexSelection(VARIANT_FALSE);
			}
		}
	}

EXIT_FAIL:
	SAFERELEASE(pselTool);
	return;
}

void CVWRenderViewCtrl::OnUpdateToolsScale(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CVWRenderViewCtrl::OnToolsUndo()
{
	HRESULT hr = S_OK;

	hr = m_pControlManager->Undo();
	if (FAILED(hr))
		HrError(hr);

}

void CVWRenderViewCtrl::OnUpdateToolsRedo(CCmdUI* pCmdUI)
{
	// check to see if there is anything we can undo
	HRESULT hr = S_OK;
	IVWUndoStacks *pUndoStacks = NULL;
	CString strRedoLabel;
	strRedoLabel.LoadString(IDS_REDO_MENU_LABEL);
	BSTR redoLabel = NULL;

	hr = m_pControlManager->get_UndoStack(&pUndoStacks);
	if (FAILED(hr) || !pUndoStacks) goto exit_fail;

	hr = pUndoStacks->get_RedoLabel(&redoLabel);
	if (FAILED(hr)) goto exit_fail;

	if (redoLabel)
	{
		strRedoLabel += redoLabel;
		pCmdUI->SetText(strRedoLabel);
	}
	pCmdUI->Enable(TRUE);

	SAFEFREESTRING(redoLabel);
	SAFERELEASE(pUndoStacks);
	return;

exit_fail:
	pCmdUI->Enable(FALSE);
	SAFERELEASE(pUndoStacks);
	SAFEFREESTRING(redoLabel);
}

void CVWRenderViewCtrl::OnToolsRedo()
{
	HRESULT hr = S_OK;

	hr = m_pControlManager->Redo();
	if (FAILED(hr))
		HrError(hr);

}

void CVWRenderViewCtrl::OnUpdateToolsUndo(CCmdUI* pCmdUI)
{
	// check to see if there is anything we can undo
	HRESULT hr = S_OK;
	IVWUndoStacks *pUndoStacks = NULL;
	CString strUndoLabel;
	strUndoLabel.LoadString(IDS_UNDO_MENU_LABEL);
	BSTR undoLabel = NULL;

	hr = m_pControlManager->get_UndoStack(&pUndoStacks);
	if (FAILED(hr) || !pUndoStacks) goto exit_fail;

	hr = pUndoStacks->get_UndoLabel(&undoLabel);
	if (FAILED(hr)) goto exit_fail;

	if (undoLabel)
	{
		strUndoLabel += undoLabel;
		pCmdUI->SetText(strUndoLabel);
	}
	else
		pCmdUI->SetText(strUndoLabel);
	pCmdUI->Enable(TRUE);

	SAFEFREESTRING(undoLabel);
	SAFERELEASE(pUndoStacks);
	return;

exit_fail:
	pCmdUI->Enable(FALSE);
	SAFERELEASE(pUndoStacks);
	SAFEFREESTRING(undoLabel);
}

void CVWRenderViewCtrl::OnViewPerspective()
{
	if (m_CameraMode != PERSPECTIVE)
		m_flZoom = 1.0;

	SetViewMode(PERSPECTIVE);
}

void CVWRenderViewCtrl::OnUpdateViewPerspective(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CVWRenderViewCtrl::OnViewTop()
{
	SetViewMode(TOP);
}

void CVWRenderViewCtrl::OnUpdateViewTop(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CVWRenderViewCtrl::OnViewRight()
{
	SetViewMode(RIGHT);
}

void CVWRenderViewCtrl::OnUpdateViewRight(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CVWRenderViewCtrl::OnViewFront()
{
	SetViewMode(FRONT);
}

void CVWRenderViewCtrl::OnUpdateViewFront(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


void CVWRenderViewCtrl::OnUpdateToolsCreatePolygonBoundary(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CVWRenderViewCtrl::OnToolsCreatePolygonBoundary()
{
	HRESULT hr = S_OK;
	CComBSTR bstrToolName;

	// turn on Translate Tool
	if(m_pVWUIView)
	{
		if (!IsUIToolCurrent(CREATEBOUNDARYTOOLNAME))
		{
			hr = AddCurrentUITool(NULL);
			if(FAILED(hr)) goto EXIT_FAIL;

			hr = AddCurrentUITool(CREATEBOUNDARYTOOLNAME);
			if(FAILED(hr))
			{
				hr = m_pVWUIView->LoadUIToolByCLSID( CLSID_PolyTool,CComBSTR(CREATEBOUNDARYTOOLNAME));
				if(FAILED( hr )) goto EXIT_FAIL;
				hr = AddCurrentUITool( CREATEBOUNDARYTOOLNAME);
				if(FAILED( hr )) goto EXIT_FAIL;
			}

			// And finally disable navigation.
			//if (FAILED(hr = m_pVWNavigationTool->put_AllNavigationDisabled(VARIANT_TRUE)))
			//	goto EXIT_FAIL;

			m_bNavigationToolIsCurrentlySelected = FALSE;
			SetFocus();
			HandleOnMessage(WM_SETFOCUS, NULL, NULL );
		}
	}

EXIT_FAIL:
	return;
	
}

void CVWRenderViewCtrl::OnToolsGravity()
{
	VARIANT_BOOL vbGrav;

	ASSERT(m_pVWNavigationTool);

	m_pVWNavigationTool->get_Gravity(&vbGrav);
	m_bIgnoreGravity = (vbGrav == VARIANT_TRUE ? TRUE : FALSE);

	m_pVWNavigationTool->put_Gravity(m_bIgnoreGravity ? VARIANT_FALSE : VARIANT_TRUE);

}

void CVWRenderViewCtrl::OnUpdateToolsGravity(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CVWRenderViewCtrl::OnToolsCollisions()
{ 
	VARIANT_BOOL vbColl;

	ASSERT(m_pVWNavigationTool);

	m_pVWNavigationTool->get_CollisionDetection(&vbColl);
	m_bIgnoreCollisions = (vbColl == VARIANT_TRUE ? TRUE : FALSE);

	m_pVWNavigationTool->put_CollisionDetection(m_bIgnoreCollisions ? VARIANT_FALSE : VARIANT_TRUE);

}

void CVWRenderViewCtrl::OnUpdateToolsCollisions(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


void CVWRenderViewCtrl::OnToolsShowOptions()
{ 
	COptionsDialog optdlg;
	HRESULT hr = S_OK;
	IVWRender3D * pr3D = NULL;
	IDirect3DRMViewport * pViewport = NULL;
	IDirect3DRMDevice * pDevice = NULL;
	
	optdlg.m_bWireframe = m_bRenderWireframe;
	optdlg.m_bIgnoreCollisions = m_bIgnoreCollisions;
	optdlg.m_bIgnoreGravity = m_bIgnoreGravity;

	if (optdlg.DoModal() == IDOK)
	{
		m_bRenderWireframe = optdlg.m_bWireframe;

		if(NULL != m_pVWRenderRoot )
		{
			// Get the VWRender3D
			hr = m_pVWRenderRoot->get_VWRender3D(&pr3D);
			if(FAILED(hr) || !pr3D) goto EXIT_FAIL;

			// Get the Viewport
			hr = pr3D->get_Viewport(&pViewport);
			if(FAILED(hr) || !pViewport) goto EXIT_FAIL;

			// Get the Device
			hr = pViewport->GetDevice(&pDevice);
			if(FAILED(hr) || !pDevice) goto EXIT_FAIL;

			hr = pDevice->SetQuality(m_bRenderWireframe ? D3DRMRENDER_WIREFRAME : D3DRMRENDER_PHONG );
			if(FAILED(hr)) goto EXIT_FAIL;

			m_bIgnoreCollisions = optdlg.m_bIgnoreCollisions;
			m_bIgnoreGravity = optdlg.m_bIgnoreGravity;
		}
	}

EXIT_FAIL:
	SAFERELEASE(pr3D);
	SAFERELEASE(pViewport);
	SAFERELEASE(pDevice);
}

void CVWRenderViewCtrl::OnUpdateToolsShowOptions(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


HRESULT CVWRenderViewCtrl::HandleOptionalSelection( VARIANT varSelection ) 
{
	CSelectionDialog selectDlg(this);
	CPropertyListPtr propListPtr;
	HRESULT hr = S_OK;

	if (varSelection.vt == VT_DISPATCH)
	{
		propListPtr = varSelection.pdispVal;

		if (propListPtr.p != NULL)
		{
			selectDlg.Initialize(m_pControlManager, propListPtr.p);
			selectDlg.DoModal();
		}
		else
		{
			hr = m_pControlManager->AddSelectionExt(m_pVWUIView, varSelection);
			if (FAILED(hr)) goto ERROR_DONE;
		}
	}

ERROR_DONE:
	return hr; 
}


void CVWRenderViewCtrl::OnToolsZoomNormal()
{
	SetZoomLevel(1.0F);
}

void CVWRenderViewCtrl::OnUpdateToolsZoomNormal(CCmdUI* pCmdUI)
{
	CString szNormal, szTmp;

	szNormal.LoadString(IDS_ZOOMNORMAL);

	if (m_flZoom == 1.0)
		pCmdUI->Enable(FALSE);
	else
	{
		pCmdUI->Enable(TRUE);
	}

	szTmp.Format(" (Current: %1.1f)", 1.0F / m_flZoom);
	szNormal += szTmp;
	pCmdUI->SetText(szNormal);
	
}


void CVWRenderViewCtrl::OnToolsZoomIn()
{
	HRESULT hr = S_OK;
	IVWZoomTool* pzoomTool = NULL;
	short nMode;

	// turn on Zoom In Tool
	if(m_pVWUIView)
	{
		//Set the correct mode in the zoom tool
		hr = m_pWorld->get_Tool(CComBSTR(ZOOMINTOOLNAME), (IUnknown**) &pzoomTool);
		if(FAILED(hr)) goto EXIT_FAIL;

		if (IsUIToolCurrent(ZOOMINTOOLNAME))
		{
			hr = pzoomTool->get_Mode(&nMode);
			if(FAILED(hr)) goto EXIT_FAIL;

			if (nMode == zmZoomIn)
				goto EXIT_FAIL;
		}

		hr = AddCurrentUITool(NULL);
		if(FAILED(hr)) goto EXIT_FAIL;

		hr = AddCurrentUITool(ZOOMINTOOLNAME);
		if(FAILED(hr))
		{
			hr = m_pVWUIView->LoadUIToolByCLSID( CLSID_VWZoomTool,CComBSTR(ZOOMINTOOLNAME));
			if(FAILED( hr )) goto EXIT_FAIL;
			hr = AddCurrentUITool( ZOOMINTOOLNAME );
			if(FAILED( hr )) goto EXIT_FAIL;
		}

		m_bNavigationToolIsCurrentlySelected = FALSE;
		SetFocus();
		HandleOnMessage(WM_SETFOCUS, NULL, NULL );

		// And finally disable navigation.
		//if (FAILED(hr = m_pVWNavigationTool->put_AllNavigationDisabled(VARIANT_TRUE)))
		//	goto EXIT_FAIL;

		ASSERT(pzoomTool);
		if (pzoomTool)
			pzoomTool->put_Mode(zmZoomIn);
	}

EXIT_FAIL:
//	SAFERELEASE(pselTool);
	return;
}

void CVWRenderViewCtrl::OnUpdateToolsZoomIn(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CVWRenderViewCtrl::OnToolsZoomOut()
{
	HRESULT hr = S_OK;
	short nMode;

	IVWZoomTool* pzoomTool = NULL;

	// turn on Zoom In Tool
	if(m_pVWUIView)
	{
		if (!IsUIToolCurrent(ZOOMOUTTOOLNAME))
		{
			//Set the correct mode in the zoom tool
			hr = m_pWorld->get_Tool(CComBSTR(ZOOMOUTTOOLNAME), (IUnknown**) &pzoomTool);
			if(FAILED(hr)) goto EXIT_FAIL;

			if (IsUIToolCurrent(ZOOMINTOOLNAME))
			{
				hr = pzoomTool->get_Mode(&nMode);
				if(FAILED(hr)) goto EXIT_FAIL;

				if (nMode == zmZoomOut)
					goto EXIT_FAIL;
			}

			hr = AddCurrentUITool(NULL);
			if(FAILED(hr)) goto EXIT_FAIL; 

			hr = AddCurrentUITool(ZOOMOUTTOOLNAME);
			if(FAILED(hr))
			{
				hr = m_pVWUIView->LoadUIToolByCLSID( CLSID_VWZoomTool,CComBSTR(ZOOMOUTTOOLNAME));
				if(FAILED( hr )) goto EXIT_FAIL;
				hr = AddCurrentUITool( ZOOMOUTTOOLNAME);
				if(FAILED( hr )) goto EXIT_FAIL;
			}

			// And finally disable navigation.
			//if (FAILED(hr = m_pVWNavigationTool->put_AllNavigationDisabled(VARIANT_TRUE)))
			//	goto EXIT_FAIL;

			m_bNavigationToolIsCurrentlySelected = FALSE;
			SetFocus();
			HandleOnMessage(WM_SETFOCUS, NULL, NULL );

			ASSERT(pzoomTool);
			if (pzoomTool)
				pzoomTool->put_Mode(zmZoomOut);
		}
	}

EXIT_FAIL:
//	SAFERELEASE(pselTool);
	return;
}

void CVWRenderViewCtrl::OnUpdateToolsZoomOut(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CVWRenderViewCtrl::OnToolsSelectContents()
{
	IThing* pContainer = NULL;
	IPropertyList* pContents = NULL;
	HRESULT hr = S_OK;

	if (!m_pRenderingContext)
		goto ERROR_ENCOUNTERED;

	// Does the connected user have a location?
	hr = m_pRenderingContext->get_Container(&pContainer);
	if (FAILED(hr) || !pContainer) goto ERROR_ENCOUNTERED;

	hr = pContainer->get_Contents(&pContents);
	if (FAILED(hr) || !pContents) goto ERROR_ENCOUNTERED;

	HandleOptionalSelection( CComVariant(pContents) );

ERROR_ENCOUNTERED:
	SAFERELEASE(pContents);
	SAFERELEASE(pContainer);
	return;
}

void CVWRenderViewCtrl::OnUpdateToolsSelectContents(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

