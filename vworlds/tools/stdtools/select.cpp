// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// select.cpp : Implementation of CVWSelectToolObject and DLL registration.

#include "stdafx.h"
#include <stdtools.h>
#include "select.h"
#include "pickdata.h"
#include "vwrnedit.h"

typedef CVWComPtr<IObjectProperty,NULL,&IID_IObjectProperty> CObjectPropertyPtr;
typedef CVWComPtr<IPropertyList,NULL,&IID_IPropertyList> CPropertyListPtr;
typedef CVWComPtr<IThing,NULL,&IID_IThing> CThingPtr;
typedef CVWComPtr<IVWFrame,NULL,&IID_IVWFrame> CVWFramePtr;
typedef CVWComPtr<IVWPickData,NULL,&IID_IVWPickData> CVWPickDataPtr;
typedef CVWComPtr<IBoundary,NULL,&IID_IBoundary> CBoundaryPtr;

#define dispidToolEvent 19L
#define IsPressed(key)			(GetAsyncKeyState(key) < 0)

const IID DIID__RenderingControl = { 0x44fac781, 0xca4, 0x11d0, { 0x89, 0xa9, 0, 0xa0, 0xc9, 0x5, 0x41, 0x29 } };
static CComBSTR bstrNULL("NULL");

/////////////////////////////////////////////////////////////////////////////
//Helper method to determine if an object is selected.
//If bCheckContainer is true, containers above pOp are also checked.
//Returns a ref'ed object property of the item selected.

IObjectProperty* CVWSelectToolObject::FindSelected( IObjectProperty* pOp, BOOL bCheckContainer, BOOL& bFoundInContainer)
{
	CThingPtr thingPtr;
	IPropertyList * plistSel = NULL;
	IObjectProperty* pObjProp = NULL;
	IObjectProperty* pSelected = NULL;
	HRESULT hr = S_OK;
	int i;
	long nCount;

	hr = m_pControlManager->get_SelectionList(&plistSel);
	if( FAILED(hr)) goto EXIT_FAIL;

	thingPtr = pOp;

	//Scan through hierarchy for object or parent on selection list.
	while ( thingPtr && !pSelected )
	{
		CThingPtr parentThingPtr;

		//Check it see if it or it's parent is in the list
		hr = plistSel->get_Count(&nCount);
		if (FAILED(hr)) goto EXIT_FAIL;

		for (i = 0; i < nCount; i++)
		{
			hr = plistSel->get_ObjectProperty(i, &pObjProp);
			if (FAILED(hr)) goto EXIT_FAIL;

			if ( pObjProp == thingPtr.p )
			{
				pSelected = pObjProp;
				SAFEADDREF(pSelected);

				break;
			}
		}

		if (pSelected == NULL) 
		{
			//Get the thing's parent
			hr = thingPtr->get_Container(&parentThingPtr.p);
			if (FAILED(hr)) goto EXIT_FAIL;

			thingPtr.Release();

			//Assign as the new object we're scrutinizing
			thingPtr = parentThingPtr;

			parentThingPtr.Release();

			if (thingPtr.p)
			{
				//Now check if the new object has a container.
				//If not, it's the room and we don't care about it.
				hr = thingPtr->get_Container(&parentThingPtr.p);
				if (FAILED(hr)) goto EXIT_FAIL;

				//Ditch the room.
				if (!parentThingPtr.p)
					thingPtr.Release();
			}

			bFoundInContainer = TRUE;
		}
	}

	if (pSelected == NULL)
	{
		CVWPickDataPtr hitPickDataPtr;

		bFoundInContainer = FALSE;
		hitPickDataPtr = pOp;

		if (hitPickDataPtr.p != NULL)
		{
			CBoundaryPtr hitBoundaryPtr;

			hr = hitPickDataPtr->get_Boundary(&hitBoundaryPtr.p);
			if (FAILED(hr)) goto EXIT_FAIL;

			hr = plistSel->get_Count(&nCount);
			if (FAILED(hr)) goto EXIT_FAIL;

			for (i = 0; i < nCount; i++)
			{
				CVWPickDataPtr pickDataPtr;
				CBoundaryPtr boundaryPtr;

				hr = plistSel->get_ObjectProperty(i, &pObjProp);
				if (FAILED(hr)) goto EXIT_FAIL;

				pickDataPtr = pObjProp;

				if (pickDataPtr.p != NULL)
				{
					hr = pickDataPtr->get_Boundary(&boundaryPtr.p);
					if (FAILED(hr)) goto EXIT_FAIL;

					if (boundaryPtr.p == hitBoundaryPtr.p)
					{
						//pSelected = pickDataPtr.p;
						pSelected = hitPickDataPtr.p;
						pSelected->AddRef();
						break;
					}
				}
				SAFERELEASE(pObjProp);
			}
		}
	}

EXIT_FAIL:
	SAFERELEASE(plistSel);
	SAFERELEASE(pObjProp);

	return pSelected;
}


// IVWUITool Methods
CVWSelectToolObject::CVWSelectToolObject()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//SetCursor(m_pUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_SELECT)));
	//m_piBoundaryViewControl = NULL;
	m_bDeluxeSelection = FALSE;
	m_bVertexSelection = FALSE;

	m_pSelectedObjectProperty = NULL;
	m_piEditorViewControl = NULL;

}

CVWSelectToolObject::~CVWSelectToolObject()
{
	SAFERELEASE(m_piEditorViewControl);
	SAFERELEASE(m_pSelectedObjectProperty);
}

STDMETHODIMP CVWSelectToolObject::Initialize(IVWUIView* pVw, IWorld* pWorld, IVWRenderRoot *pVWRenderRoot)
{
	HRESULT hr = S_OK;

	if (m_pUIView != pVw)
	{
		SAFERELEASE(m_pUIView);
		m_pUIView = pVw;
		SAFEADDREF(pVw);
	}

	if (m_pWorld != pWorld)
	{
		SAFERELEASE(m_pWorld);
		m_pWorld = pWorld;
		SAFEADDREF(pWorld);
	}

	if (!m_piEditorViewControl)
	{
		hr = m_pUIView->QueryInterface(DIID__RenderingControl, (LPVOID*) &m_piEditorViewControl);
		if (FAILED(hr) || (m_piEditorViewControl == NULL)) goto EXIT_FAIL;
	}

EXIT_FAIL:
	return hr;
}

BOOL CVWSelectToolObject::OnLButtonDown( UINT nFlags, POINT pt  )
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bHandled;
	VARIANTARG varHitParams,varHitResult;
	VARIANTARG *pvarHitResult = &varHitResult;
	IPropertyList* plistSel = NULL;
	CPropertyListPtr pPropList;
	IObjectProperty* pObjProp = NULL;
	IObjectProperty* pSelected = NULL;
	BOOL bDoNotAddSelection = FALSE;
	BOOL bDoNotRemove = FALSE;
	long nHitTestFlags;
	IThing* pGlobal = NULL;
	VARIANT_BOOL vbSelLock;
	static CComBSTR bstrSelectionLock("SelectionLock");

	bHandled = FALSE;
	m_pPoint = pt;
	SAFERELEASE(m_pSelectedObjectProperty);

	::VariantInit( &varHitResult );

	//Do nothing is O is down, we're busy orbiting.  
	//Do nothing if right button is also down, that's the alternate axis key.
	if (IsPressed('O') || nFlags & MK_RBUTTON)
		goto EXIT_SUCCEEDED;

	//Do nothing if selection lock is ON.
	hr = m_pWorld->get_Global(&pGlobal);
	if( SUCCEEDED(hr))
	{
		hr = pGlobal->get_BOOL(bstrSelectionLock, &vbSelLock);
		if( SUCCEEDED(hr) )
		{
			if (vbSelLock == VARIANT_TRUE)
				goto EXIT_SUCCEEDED;
		}
	}

	if(m_pUIView && m_pControlManager)
	{
		varHitParams.vt = VT_POINT_BYREF;
		varHitParams.byref = (LPVOID)&pt;

		nHitTestFlags = (m_bVertexSelection ? HITTEST_VERTEXSELECT : 0);
		nHitTestFlags |= (m_bDeluxeSelection && !(MK_CONTROL & nFlags) ? HITTEST_RETURNALLPICKED : 0);

		hr = ((IVWUIView*)m_pUIView)->HitTest(varHitParams, nHitTestFlags, pvarHitResult);	
		if( FAILED(hr)) goto EXIT_FAIL;

		if (VWSTUDIO_S_IGNORESELECTION == hr) //If we hit something, don't remove it from selection list.
			bDoNotRemove = TRUE;

		if(varHitResult.vt != VT_EMPTY)
		{
			//Always a PropertyList if VARIANT_TRUE is sent to HitTest
			pPropList = *pvarHitResult;

			if (pPropList != NULL)
			{
				m_pUIView->OptionalSelection(*pvarHitResult);
			}
			else
			{
				// We hit just one thing.
				// Decide if the item has already been selected.

				bDoNotAddSelection = FALSE;

				//If multi-select isn't active but the item is already selected, don't do anything.
				if (pSelected = FindSelected( (IObjectProperty*) pvarHitResult->pdispVal, TRUE, bDoNotAddSelection))
				{
					//The object is already selected.
					if (!bDoNotAddSelection && pSelected != pvarHitResult->pdispVal)
					{
						hr = m_pControlManager->AddSelectionExt( m_pUIView, *pvarHitResult );
						if( FAILED(hr)) goto EXIT_FAIL;
					}

					m_pSelectedObjectProperty = pSelected; //pvarHitResult->pdispVal;
					SAFEADDREF(m_pSelectedObjectProperty);
				}
				else  //The object wasn't selected already.
				{
					//If the control key IS NOT down, clear all selections!
					if( !(MK_CONTROL & nFlags)	)
					{
						hr = m_pControlManager->ClearSelectionsExt( m_pUIView );
						if( FAILED(hr)) goto EXIT_FAIL;
					}

					if (!bDoNotAddSelection)
					{
						hr = m_pControlManager->AddSelectionExt( m_pUIView, *pvarHitResult );
						if( FAILED(hr)) goto EXIT_FAIL;
					}
				}
			}
		}
		else
		// We hit nothing. Clear Selection.
		{
			hr = m_pControlManager->ClearSelectionsExt( m_pUIView );
			if( FAILED(hr)) goto EXIT_FAIL;
		}
	}
	else
	{
		hr = E_FAIL;
		goto EXIT_FAIL;
	}

	bHandled = TRUE;
	goto EXIT_SUCCEEDED;

EXIT_FAIL:
	ASSERT(0);
EXIT_SUCCEEDED:

	VariantClear(&varHitResult);
	SAFERELEASE(plistSel);
	SAFERELEASE(pGlobal);
	SAFERELEASE(pSelected);
	return bHandled;
}

BOOL CVWSelectToolObject::OnLButtonUp( UINT nFlags, POINT pt  )
{
	IPropertyList * plistSel = NULL;
	IObjectProperty* pObjProp = NULL;
	long nCount, i, j;
	int nVertexCount;
	HRESULT hr = S_OK;
	CVWPickDataPtr selectedPickDataPtr;
	CBoundaryPtr selectedBoundaryPtr;
	VARIANT_BOOL vbPicked, vbAnyAtAllPicked = VARIANT_FALSE;

	//If this is a click & drag operation, we dont want to do anything.
	//This code should only be entered if the user clicks and releases the mouse on an 
	//object that is already selected without moving the mouse.
	if(m_pSelectedObjectProperty && abs(pt.x) - abs(m_pPoint.x) < 1 && abs(pt.y) - abs(m_pPoint.y) < 1 )
	{
		//Set up the pick data stuff in case this is a boundary.
		selectedPickDataPtr = m_pSelectedObjectProperty;
		if (selectedPickDataPtr.p)
			selectedPickDataPtr->get_Boundary(&selectedBoundaryPtr.p);

		if ( ! (MK_CONTROL & nFlags) )
		{
			//Remove everything but the picked object.
			hr = m_pControlManager->get_SelectionList(&plistSel);
			if( FAILED(hr)) goto EXIT_FAIL;

			hr = plistSel->get_Count(&nCount);
			if (FAILED(hr)) goto EXIT_FAIL;

			for (i = nCount - 1; i >=0 ; i--)
			{
				CVWPickDataPtr pickDataPtr;
				CBoundaryPtr boundaryPtr;

				hr = plistSel->get_ObjectProperty(i, &pObjProp);
				if (FAILED(hr)) goto EXIT_FAIL;

				pickDataPtr = pObjProp;

				if (pickDataPtr.p != NULL)
				{
					hr = pickDataPtr->get_Boundary(&boundaryPtr.p);
					if (FAILED(hr)) goto EXIT_FAIL;

					if (boundaryPtr.p != selectedBoundaryPtr.p)
					{
						m_pControlManager->RemoveSelectionExt( m_pUIView, CComVariant(pObjProp) );
						break;
					}
					else
					{
						selectedBoundaryPtr->get_VertexCount(&nVertexCount);

						for (j = 0; j < nVertexCount; j++)
						{
							selectedPickDataPtr->get_VertexPicked(j, &vbPicked);
							pickDataPtr->put_VertexPicked(j, vbPicked);
						}

						InvokeToolEvent(TOOLEVENT_BOUNDARYUPDATE, boundaryPtr.p, bstrNULL, bstrNULL, bstrNULL, VARIANT_FALSE);

					}
				}
				else if (pObjProp != m_pSelectedObjectProperty)
				{
					m_pControlManager->RemoveSelectionExt( m_pUIView, CComVariant(pObjProp) );
				}
				SAFERELEASE(pObjProp);

/*				hr = plistSel->get_ObjectProperty(i, &pObjProp);
				if (FAILED(hr)) goto EXIT_FAIL;

				if (pObjProp != m_pSelectedObjectProperty)
				{
					hr = m_pControlManager->RemoveSelectionExt( m_pUIView, CComVariant(pObjProp) );
					if (FAILED(hr)) goto EXIT_FAIL;
				}
				SAFERELEASE(pObjProp);
*/
			}
		}
		else
		{
			if (selectedBoundaryPtr.p)  //We clicked on a boundary
			{
				hr = m_pControlManager->get_SelectionList(&plistSel);
				if( FAILED(hr)) goto EXIT_FAIL;

				hr = plistSel->get_Count(&nCount);
				if (FAILED(hr)) goto EXIT_FAIL;

				for (i = nCount-1; i >= 0; i--)
				{
					CVWPickDataPtr pickDataPtr;
					CBoundaryPtr boundaryPtr;

					hr = plistSel->get_ObjectProperty(i, &pObjProp);
					if (FAILED(hr)) goto EXIT_FAIL;

					pickDataPtr = pObjProp;

					if (pickDataPtr.p != NULL)
					{
						hr = pickDataPtr->get_Boundary(&boundaryPtr.p);
						if (FAILED(hr)) goto EXIT_FAIL;

						if (boundaryPtr.p == selectedBoundaryPtr.p)
						{
							selectedBoundaryPtr->get_VertexCount(&nVertexCount);

							vbPicked = VARIANT_FALSE;
							for (j = 0; j < nVertexCount; j++)
							{
								selectedPickDataPtr->get_VertexPicked(j, &vbPicked);
								if (vbPicked)
								{
									pickDataPtr->get_VertexPicked(j, &vbPicked);
									if (vbPicked)
										pickDataPtr->put_VertexPicked(j, VARIANT_FALSE);
									else
										pickDataPtr->put_VertexPicked(j, VARIANT_TRUE);

									vbAnyAtAllPicked = VARIANT_TRUE;
								}
							}

							if (!vbAnyAtAllPicked)
							{
								for (j = 0; j < nVertexCount; j++)
								{
									pickDataPtr->put_VertexPicked(j, VARIANT_FALSE);
								}
								m_pControlManager->RemoveSelectionExt( m_pUIView, CComVariant(pickDataPtr) );
							}

							InvokeToolEvent(TOOLEVENT_BOUNDARYUPDATE, boundaryPtr.p, bstrNULL, bstrNULL, bstrNULL, VARIANT_FALSE);

							//	m_pControlManager->RemoveSelectionExt( m_pUIView, CComVariant(pObjProp) );

							break;
						}
					}
					SAFERELEASE(pObjProp);
				}
			}
			else
			{
				m_pControlManager->RemoveSelectionExt( m_pUIView, CComVariant(m_pSelectedObjectProperty) );
			}
		}
	}

EXIT_FAIL:
	SAFERELEASE(plistSel);
	SAFERELEASE(pObjProp);
	SAFERELEASE(m_pSelectedObjectProperty);

	return TRUE;
}


HRESULT CVWSelectToolObject::InvokeToolEvent(int nNum, IObjectProperty* pOP, BSTR bstr1, BSTR bstr2, BSTR bstr3, VARIANT_BOOL bBubble)
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

BOOL CVWSelectToolObject::OnMouseMove( UINT nFlags, POINT pt  )
{

	if(m_pSelectedObjectProperty && abs(pt.x) - abs(m_pPoint.x) < 1 && abs(pt.y) - abs(m_pPoint.y) < 1 )
	{
		SAFERELEASE(m_pSelectedObjectProperty);
	}

	return TRUE;
}


STDMETHODIMP CVWSelectToolObject::IsValid(IVWUIView * pVw, VARIANT_BOOL * pfValid)
{
	ASSERT( pfValid );
	if (!pfValid )
		return E_POINTER;

	*pfValid = TRUE; // For now.
	return S_OK;
}


STDMETHODIMP CVWSelectToolObject::get_DeluxeSelection(VARIANT_BOOL *pVal)
{
	if (NULL == pVal)
		return E_INVALIDARG;

	*pVal = (m_bDeluxeSelection ? VARIANT_TRUE : VARIANT_FALSE);

	return S_OK;
}

STDMETHODIMP CVWSelectToolObject::put_DeluxeSelection(VARIANT_BOOL newVal)
{

	m_bDeluxeSelection = newVal;

	return S_OK;
}




STDMETHODIMP CVWSelectToolObject::get_VertexSelection(VARIANT_BOOL *pVal)
{
	if (NULL == pVal)
		return E_INVALIDARG;

	*pVal = (m_bVertexSelection ? VARIANT_TRUE : VARIANT_FALSE);

	return S_OK;
}

STDMETHODIMP CVWSelectToolObject::put_VertexSelection(VARIANT_BOOL newVal)
{

	m_bVertexSelection = newVal;

	return S_OK;
}



