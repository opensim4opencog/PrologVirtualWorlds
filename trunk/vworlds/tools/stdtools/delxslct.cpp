// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// select.cpp : Implementation of CVWSelectToolObject and DLL registration.

#include "stdafx.h"
#include <stdtools.h>
#include "Delxslct.h"

typedef CVWComPtr<IObjectProperty,NULL,&IID_IObjectProperty> CObjectPropertyPtr;
typedef CVWComPtr<IPropertyList,NULL,&IID_IPropertyList> CPropertyListPtr;
typedef CVWComPtr<IThing,NULL,&IID_IThing> CThingPtr;
typedef CVWComPtr<IVWFrame,NULL,&IID_IVWFrame> CVWFramePtr;

/////////////////////////////////////////////////////////////////////////////
//const IID DIID__BoundaryControl = {0x9BF8A469,0x1543,0x11D0,{0x89,0xA9,0x00,0xA0,0xC9,0x05,0x41,0x29}};

/////////////////////////////////////////////////////////////////////////////
//

// IVWUITool Methods

// Review: Any reason to implement?

CVWDeluxeSelectToolObject::CVWDeluxeSelectToolObject()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SetCursor(m_pUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_SELECT)));
	m_pUnRefedSelectedObjectProperty = NULL;
}

BOOL CVWDeluxeSelectToolObject::OnLButtonDown( UINT nFlags, POINT pt  )
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bHandled;
	IPropertyList * pPL = NULL;
	VARIANTARG varHitParams,varHitResult;
	VARIANTARG *pvarHitResult = &varHitResult;
	IPropertyList * plistSel = NULL;
	IObjectProperty * pIndOP = NULL;
	CObjectPropertyPtr OPPtr;
	BOOL bDoNotRemove = FALSE;
	long nFindResult, nCount, i;
	IObjectProperty* pObjProp = NULL;
	IObjectProperty* pPickedObjProp = NULL;
	CPropertyListPtr pPropList;

	bHandled = FALSE;
	m_pPoint = pt;
	m_pUnRefedSelectedObjectProperty = NULL;

	::VariantInit( &varHitResult );
	if(m_pUIView && m_pControlManager)
	{
//		SAFERELEASE((IDispatch*&)m_piBoundaryViewControl);
//		hr = m_pUIView->QueryInterface(DIID__BoundaryControl, (LPVOID*) &m_piBoundaryViewControl);

		varHitParams.vt = VT_POINT_BYREF;
		varHitParams.byref = (LPVOID)&pt;

		hr = ((IVWUIView*)m_pUIView)->HitTest(varHitParams, (MK_CONTROL & nFlags) ? 0 : HITTEST_RETURNALLPICKED, pvarHitResult);	
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
				// We hit something.
				// Decide if the item has already been selected.
				hr = m_pControlManager->get_SelectionList(&plistSel);
				if( FAILED(hr)) goto EXIT_FAIL;

				hr = plistSel->Find(*pvarHitResult, &nFindResult);
				if( FAILED(hr)) goto EXIT_FAIL;

				// If the Control key isn't down, clear the selection list.
				if( !(MK_CONTROL & nFlags)	)
				{
					//If multi-select isn't active but the item is already selected, don't do anything.
					if (nFindResult == -1)
					{
						hr = m_pControlManager->ClearSelectionsExt( m_pUIView );
						if( FAILED(hr)) goto EXIT_FAIL;
					}
					else
					{
						hr = plistSel->get_Count(&nCount);
						if (FAILED(hr)) goto EXIT_FAIL;

						for (i = 0; i < nCount; i++)
						{
							hr = plistSel->get_ObjectProperty(i, &pObjProp);
							if (FAILED(hr)) goto EXIT_FAIL;

							if (pObjProp != pvarHitResult->pdispVal)
							{
								hr = m_pControlManager->RemoveSelectionExt( m_pUIView, CComVariant(pObjProp) );
								if (FAILED(hr)) goto EXIT_FAIL;
									
								i--;
							}
							SAFERELEASE(pObjProp);
						}

					}
				
					hr = m_pControlManager->AddSelectionExt( m_pUIView, *pvarHitResult );
					if( FAILED(hr)) goto EXIT_FAIL;
				}
				else
				{
					// the control is down, check to see if this item is already selected
					// if it is, then unselect it, otherwise select it
//					if (!bDoNotRemove)
//						hr = m_pControlManager->RemoveSelectionExt( m_pUIView, *pvarHitResult );

//					if (hr == VWOBJECT_E_PROPERTYNOTEXIST || hr == VWOBJECT_E_EMPTYPROPERTYLIST || bDoNotRemove)
//					{
//						hr = m_pControlManager->AddSelectionExt( m_pUIView, *pvarHitResult );
//						if( FAILED(hr)) goto EXIT_FAIL;
//					}
//					else if (FAILED(hr)) goto EXIT_FAIL;

					hr = m_pControlManager->AddSelectionExt( m_pUIView, *pvarHitResult );
					if( FAILED(hr)) goto EXIT_FAIL;

					if (hr == VWSTUDIO_S_ALREADYSELECTED)
					{
						m_pUnRefedSelectedObjectProperty = (IObjectProperty*)pvarHitResult->pdispVal;
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
	// Review: Need E_TOOLNOT_INITED
		goto EXIT_FAIL;
	}
	bHandled = TRUE;
	goto EXIT_SUCCEEDED;
EXIT_FAIL:

EXIT_SUCCEEDED:

	VariantClear(&varHitResult);
	SAFERELEASE(plistSel);
	SAFERELEASE(pIndOP);
	SAFERELEASE(pObjProp);
	SAFERELEASE(pPickedObjProp);
	
	return bHandled;
}

BOOL CVWDeluxeSelectToolObject::OnLButtonUp( UINT nFlags, POINT pt  )
{
	if(m_pUnRefedSelectedObjectProperty && abs(pt.x) - abs(m_pPoint.x) < 2 && abs(pt.y) - abs(m_pPoint.y) < 2 )
	{
		m_pControlManager->RemoveSelectionExt( m_pUIView, CComVariant(m_pUnRefedSelectedObjectProperty) );
		m_pUnRefedSelectedObjectProperty = NULL;
	}

	return TRUE;
}


BOOL CVWDeluxeSelectToolObject::OnMouseMove( UINT nFlags, POINT pt  )
{
/*	VARIANT varResult;
	DISPPARAMS dispparams;;
	HRESULT	hr = S_OK;

	if( m_bMovesView && (MK_LBUTTON & nFlags) && NULL != m_piBoundaryViewControl )
	{

		VariantInit(&varResult);

		hr = InitializeDISPPARAMS(&dispparams, 2);
		if (FAILED(hr))
			goto CLEAN_UP;

		dispparams.rgvarg[1].vt = VT_R4;
		dispparams.rgvarg[1].fltVal = (float) (m_pPoint.x - pt.x);
		dispparams.rgvarg[0].vt = VT_R4;
		dispparams.rgvarg[0].fltVal = (float) (m_pPoint.y - pt.y);

		m_piBoundaryViewControl->Invoke(0xa, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispparams, &varResult, NULL, NULL);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		m_pPoint = pt;
	}

CLEAN_UP:
	VariantClear(&varResult);
*/
	return TRUE;
}


STDMETHODIMP CVWDeluxeSelectToolObject::IsValid(IVWUIView * pVw, VARIANT_BOOL * pfValid)
{
	ASSERT( pfValid );
	if (!pfValid )
		return E_POINTER;

	*pfValid = TRUE; // For now.
	return S_OK;
}


