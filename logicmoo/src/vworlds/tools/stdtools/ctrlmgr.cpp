// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// CtrlMgr.cpp : Implementation of CControlMgrObject
#include "stdafx.h"
#include <stdtools.h>
#include "CtrlMgr.h"
#include "vwuiobjs.h"
#include "pickdata.h"
#include "syshelp.h"
#include "vwsevent.h"


EXTERN_C const CLSID CLSID_PropertyList;
EXTERN_C const CLSID CLSID_VWUndoStacks;
EXTERN_C const CLSID CLSID_VWErrorTool;

//#include <vwsystem_i.c>

typedef CVWComPtr<IObjectProperty,NULL,&IID_IObjectProperty> CObjectPropertyPtr;
typedef CVWComPtr<IVWPickData,NULL,&IID_IVWPickData> CVWPickDataPtr;
typedef CVWComPtr<IBoundary,NULL,&IID_IBoundary> CBoundaryPtr;
typedef CVWComPtr<IThing,NULL,&IID_IThing> CThingPtr;

/////////////////////////////////////////////////////////////////////////////
// CControlMgrObject

// Create aggregated objects
HRESULT CControlMgrObject::FinalConstruct()
{
	// Review: ATL doesn't like me to cache pointers. m_dwRef gets inc'ed and
	// it is checked on the way out of this function for zero.
	HRESULT hr = S_OK;
	IUnknown* pUnkOuter = GetControllingUnknown();

	hr = CoCreateInstance(CLSID_PropertyList, pUnkOuter, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&m_pdispSelection.p);
	if (FAILED(hr)) goto EXIT_FAIL;

	// Agregate the UndoStacks
	hr = CoCreateInstance(CLSID_VWUndoStacks, pUnkOuter, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&m_pdispUndoStacks.p);
	if (FAILED(hr)) goto EXIT_FAIL;

	// Agregate the VWErrorTool
	hr = CoCreateInstance(CLSID_VWErrorTool, pUnkOuter, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&m_pdispErrorTool.p);
	if (FAILED(hr)) goto EXIT_FAIL;

	hr = CoCreateInstance(CLSID_PropertyList, pUnkOuter, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&m_pClipboard);
	if (FAILED(hr)) goto EXIT_FAIL;
	 

EXIT_FAIL:
	return hr;
}

CControlMgrObject::CControlMgrObject()	
{
	m_pdispSelection = NULL;
	m_pdispUndoStacks = NULL;
	m_pdispErrorTool = NULL;
	m_pClipboard = NULL;
	m_pWorld = NULL;
}

void CControlMgrObject::FinalRelease()
{ 
	m_pdispSelection.Release();
	m_pdispUndoStacks.Release();
	m_pdispErrorTool.Release();
	m_pClipboard->Release();
	SAFERELEASE(m_pWorld);
}

HRESULT CControlMgrObject::get_World(IWorld ** ppWorld)
{
	ASSERT(ppWorld);
	if (!ppWorld)
		return E_POINTER;

	*ppWorld = m_pWorld;

	return S_OK;
}

HRESULT CControlMgrObject::put_World(IWorld * pWorld)
{
	SAFERELEASE(m_pWorld);
	m_pWorld = pWorld;
	SAFEADDREF(m_pWorld);

	return S_OK;
}

HRESULT CControlMgrObject::get_SelectionList(IPropertyList ** ppPL)
{
	HRESULT hr = S_OK;

	if(NULL == ppPL)
		return E_POINTER;
	if( ! m_pdispSelection )
		hr = E_FAIL;
	else
		hr = m_pdispSelection->QueryInterface(IID_IPropertyList, (LPVOID*)ppPL);
	return hr;
}

HRESULT CControlMgrObject::get_UndoStack( IVWUndoStacks ** ppUS)
{
	HRESULT hr = S_OK;
	if(NULL == ppUS)

		return E_POINTER;
	if( !m_pdispUndoStacks )
		hr = E_FAIL;
	else
		hr = m_pdispUndoStacks->QueryInterface(IID_IVWUndoStacks, (LPVOID*)ppUS);
	return hr;
}
 
HRESULT CControlMgrObject::AddVWUndoItem( IVWUndoItem * pUI )
{
	HRESULT hr = S_OK;
	IVWUndoStacks * pUS = NULL;

#ifdef _DEBUG
	if (m_pWorld)
		VWTRACE(m_pWorld, "VWSTUDIO", VWT_METHOD, "CControlMgrObject::AddVWUndoItem\n");
#endif
	hr = get_UndoStack(&pUS);
	if( FAILED(hr) ) goto EXIT_FAIL;

	hr = pUS->AddVWUndoItem( pUI );
	if( FAILED(hr) ) goto EXIT_FAIL;

EXIT_FAIL:
	SAFERELEASE(pUS);
	return hr;
}

HRESULT CControlMgrObject::Undo(void)
{
	HRESULT hr = S_OK;
	IVWUndoStacks * pUS = NULL;
#ifdef _DEBUG
	if (m_pWorld)
		VWTRACE(m_pWorld, "VWSTUDIO", VWT_METHOD, "CControlMgrObject::Undo\n");
#endif

	hr = get_UndoStack(&pUS);
	if( FAILED(hr) ) goto EXIT_FAIL;

	hr = pUS->Undo();
	if( FAILED(hr) ) goto EXIT_FAIL;

	hr = UndoStackChanged(  NULL, CComVariant(NULL) );
	if( FAILED(hr) ) goto EXIT_FAIL;

EXIT_FAIL:
	SAFERELEASE(pUS);
	return hr;
}

HRESULT CControlMgrObject::Redo(void)
{
	HRESULT hr = S_OK;
	IVWUndoStacks * pUS = NULL;
#ifdef _DEBUG
	if (m_pWorld)
		VWTRACE(m_pWorld, "VWSTUDIO", VWT_METHOD, "CControlMgrObject::Redo\n");
#endif

	hr = get_UndoStack(&pUS);
	if( FAILED(hr) ) goto EXIT_FAIL;

	hr = pUS->Redo();
	if( FAILED(hr) ) goto EXIT_FAIL;

	hr = UndoStackChanged(  NULL, CComVariant(NULL) );
	if( FAILED(hr) ) goto EXIT_FAIL;

EXIT_FAIL:
	SAFERELEASE(pUS);
	return hr;
}

HRESULT CControlMgrObject::DisplayError(HRESULT hrDisplay, VARIANT_BOOL *pbHandled)
{
	HRESULT hr = S_OK;
	IVWErrorTool * pET = NULL;
#ifdef _DEBUG
	if (m_pWorld)
		VWTRACE(m_pWorld, "VWSTUDIO", VWT_METHOD, "CControlMgrObject::DisplayError\n");
#endif
	if( ! m_pdispErrorTool )
	{
		hr = E_FAIL;
		goto EXIT_FAIL;
	}
	hr = m_pdispErrorTool->QueryInterface(IID_IVWErrorTool, (LPVOID*)&pET);
	if( FAILED(hr) ) goto EXIT_FAIL;
	hr = pET->DisplayError( hrDisplay, pbHandled );
	if( FAILED(hr) ) goto EXIT_FAIL;

EXIT_FAIL:
	SAFERELEASE(pET);
	return hr;
}

HRESULT CControlMgrObject::Select( VARIANT varSel )
{
	HRESULT hr = S_OK;

	hr = ClearSelections();
	if (FAILED(hr)) goto EXIT_FAIL;

	hr = AddSelection( varSel );
	if (FAILED(hr)) goto EXIT_FAIL;

EXIT_FAIL:
	return hr;
} 

HRESULT CControlMgrObject::AddSelection( VARIANT varSel )
{
	return AddSelectionExt(NULL, varSel);
}

HRESULT CControlMgrObject::AddSelectionExt( IVWUIView * pVw, VARIANT varSel )
{
	HRESULT hr = S_OK;
	CObjectPropertyPtr OPPtr;
	IPropertyList * pSel = NULL;
	IObjectProperty * pOP = NULL;
	IObjectProperty * pIndOP = NULL;
	long lCount;
	long lIndex;
	BOOL bFound = FALSE;
	CComVariant varToSelect;
	
#ifdef _DEBUG
	if (m_pWorld)
		VWTRACE(m_pWorld, "VWSTUDIO", VWT_METHOD, "CControlMgrObject::AddSelection\n");
#endif

	hr = get_SelectionList(&pSel);
	if( FAILED(hr)) goto EXIT_FAIL;

	hr = ((IPropertyCollection *)pSel)->get_Count(&lCount);
	if ( FAILED(hr) ) goto EXIT_FAIL;

	//Test if it's coming in BYREF and de-ref is so.
	if (varSel.vt & VT_BYREF | VT_DISPATCH)
	{
		varToSelect.ChangeType(VT_DISPATCH, &varSel);
		OPPtr = varToSelect;
	}
	else
	{
		OPPtr = varSel;
	}

	if(OPPtr != NULL)
	{
		pOP = OPPtr;
		SAFEADDREF(pOP);
		for (lIndex = 0; lIndex < lCount; lIndex++)
		{
			hr = pSel->get_ObjectProperty(lIndex, &pIndOP);
			if (FAILED(hr)) goto EXIT_FAIL;

			if (pIndOP == pOP)
			{
				bFound = TRUE;
				hr = VWSTUDIO_S_ALREADYSELECTED;
				break;
			}
			SAFERELEASE(pIndOP);
		}
		
		if (!bFound)
		{
			hr = pSel->AddObjectProperty( pOP );
			if( FAILED(hr)) goto EXIT_FAIL;

			if (m_pWorld)
				m_pWorld->FireUIEvent(NULL, CComBSTR(VWSTUDIO_ADDSELECTION_EVENT_STR), CComVariant(pOP) );

		// This has been changed.  It used to not send the UI Update if the selected item
		// was already in the selection list.  This now works so that if the selected item
		// is already in the selection list, it ONLY fires the update.
		// if the selected item wasn't in the selection list, it is added and then the event
		// is fired.
			if (varSel.vt & VT_BYREF | VT_DISPATCH)
				hr = FireUIUpdate( VWUIT_ADDSELECTION, pVw, varToSelect, FALSE);
			else
				hr = FireUIUpdate( VWUIT_ADDSELECTION, pVw, varSel, FALSE);
			if( FAILED(hr)) goto EXIT_FAIL;

		}
	}
	else
		hr = E_INVALIDARG;
	
EXIT_FAIL:
	SAFERELEASE( pOP );
	SAFERELEASE( pIndOP );
	SAFERELEASE( pSel);
	return hr;
}


HRESULT CControlMgrObject::RemoveSelection( VARIANT varSel )
{
	return RemoveSelectionExt(NULL, varSel);
}

HRESULT CControlMgrObject::RemoveSelectionExt( IVWUIView * pVw, VARIANT varSel )
{
	HRESULT hr = S_OK;
	IPropertyList * pSel = NULL;

#ifdef _DEBUG
	if (m_pWorld)
		VWTRACE(m_pWorld, "VWSTUDIO", VWT_METHOD, "CControlMgrObject::RemoveSelection\n");
#endif

	hr = get_SelectionList(&pSel);
	if( FAILED(hr)) goto EXIT_FAIL;

	hr = pSel->Remove(varSel);
	if( FAILED(hr)) goto EXIT_FAIL;

	hr = FireUIUpdate( VWUIT_REMOVESELECTION, pVw, varSel, FALSE);
	if( FAILED(hr)) goto EXIT_FAIL;

	if (m_pWorld)
		m_pWorld->FireUIEvent(NULL, CComBSTR(VWSTUDIO_REMOVESELECTION_EVENT_STR), varSel);

EXIT_FAIL:
	SAFERELEASE(pSel);
	return hr;
}

HRESULT CControlMgrObject::ToggleSelection( VARIANT varSel )
{
	return ToggleSelectionExt(NULL, varSel);
}

HRESULT CControlMgrObject::ToggleSelectionExt( IVWUIView * pVw, VARIANT varSel )
{
	HRESULT hr = S_OK;

#ifdef _DEBUG
	if (m_pWorld)
		VWTRACE(m_pWorld, "VWSTUDIO", VWT_METHOD, "CControlMgrObject::ToggleSelection\n");
#endif

	hr = FireUIUpdate( VWUIT_TOGGLESELECTION, pVw, varSel, FALSE);
	if( FAILED(hr)) goto EXIT_FAIL;

EXIT_FAIL:
	return hr;
}

HRESULT CControlMgrObject::ClearSelections(void)
{
	return ClearSelectionsExt(NULL);
}

HRESULT CControlMgrObject::ClearSelectionsExt( IVWUIView * pVw )
{
	HRESULT hr = S_OK;
	COleVariant cvarDummy;
	IPropertyList * pSel = NULL;

#ifdef _DEBUG
	if (m_pWorld)
		VWTRACE(m_pWorld, "VWSTUDIO", VWT_METHOD, "CControlMgrObject::ClearSelection\n");
#endif

	cvarDummy.Clear();

	hr = get_SelectionList(&pSel);
	if( FAILED(hr)) goto EXIT_FAIL;

	hr = pSel->RemoveAll();
	// It's okay that the property list was already empty.
	if( hr == VWOBJECT_E_EMPTYPROPERTYLIST )
		hr = S_OK;
	if( FAILED(hr)) goto EXIT_FAIL;


	hr = FireUIUpdate( VWUIT_CLEARSELECTIONS, pVw, cvarDummy, FALSE);
	if( FAILED(hr)) goto EXIT_FAIL;

	if (m_pWorld)
		m_pWorld->FireUIEvent(NULL, CComBSTR(VWSTUDIO_CLEARSELECTION_EVENT_STR), CComVariant());

EXIT_FAIL:
	SAFERELEASE( pSel);
	return hr;
}

//Call this method to repaint a vwuiview or all the vwuiviews.
//  varHint is the object that changed (spawning the need to repaint the view
//  varRefreshAllViews tells whether to repaint all the views or only pVw.
HRESULT CControlMgrObject::RefreshView(  IVWUIView * pVw, VARIANT varHint, VARIANT_BOOL varRefreshAllViews  )
{
	HRESULT hr = S_OK;

	hr = FireUIUpdate( VWUIT_REFRESHVIEW, pVw, varHint, (varRefreshAllViews == VARIANT_TRUE ? FALSE : TRUE) );
	if( FAILED(hr)) goto EXIT_FAIL;


EXIT_FAIL:
	return hr;
}

//This event is fired after Undo() or Redo() is called
HRESULT CControlMgrObject::UndoStackChanged(  IVWUIView * pVw, VARIANT varHint)
{
	HRESULT hr = S_OK;

	hr = FireUIUpdate( VWUIT_UNDOSTACKCHANGED, pVw, varHint, FALSE);
	if( FAILED(hr)) goto EXIT_FAIL;


EXIT_FAIL:
	return hr;
}

HRESULT CControlMgrObject::GetSelectedCount( IID* pIID, long* nCount )
{
	IPropertyList * pSel = NULL;
	IObjectProperty* pobjprop = NULL;
	IBoundary* pBoundary = NULL;
	HRESULT hr = S_OK;
	long nsize, i;

	if (nCount == NULL)
		return E_POINTER;

	hr = get_SelectionList(&pSel);
	if( FAILED(hr)) goto EXIT_FAIL;

	hr = ((IPropertyCollection *)pSel)->get_Count(&nsize);
	if ( FAILED(hr) ) goto EXIT_FAIL;

	for (i = 0; i < nsize; i++)
	{
		hr = pSel->get_ObjectProperty(i, &pobjprop);
		if (FAILED(hr)) goto EXIT_FAIL;

		hr = pobjprop->QueryInterface(IID_IBoundary, (LPVOID*)pBoundary);

		if (SUCCEEDED(hr) && pBoundary != NULL)
			(*nCount)++;

		SAFERELEASE(pobjprop);
		SAFERELEASE(pBoundary);
	}

EXIT_FAIL:
	SAFERELEASE(pobjprop);
	SAFERELEASE(pBoundary);
	SAFERELEASE(pSel);

	return hr;
}


HRESULT CControlMgrObject::RefreshObjectsInView( IVWUIView * pVw, VARIANT varRefresh)
{
	HRESULT hr;

	hr = FireUIUpdate( VWUIT_REFRESHOBJECTS, pVw, varRefresh, TRUE);
	if( FAILED(hr)) goto EXIT_FAIL;

EXIT_FAIL:
	return hr;
}

HRESULT CControlMgrObject::RefreshObjects( IVWUIView * pVw, VARIANT varRefresh)
{
	HRESULT hr = S_OK;

#ifdef _DEBUG
	if (m_pWorld)
		VWTRACE(m_pWorld, "VWSTUDIO", VWT_METHOD, "CControlMgrObject::RefreshObjects\n");
#endif

	hr = FireUIUpdate( VWUIT_REFRESHOBJECTS, pVw, varRefresh, FALSE);
	if( FAILED(hr)) goto EXIT_FAIL;

EXIT_FAIL:
	return hr;
}


// broadcast to all the objects
HRESULT CControlMgrObject::FireUIUpdate(enumVWUIEventTypes enumType, IVWUIView * pvwSrc, VARIANT varHint, BOOL bOnlyRefreshSource)
{
	IConnectionPointImpl<CControlMgrObject, &IID_IVWUIEvents, CComDynamicUnkArray>* p = this;

	HRESULT hr = S_OK;
	IUnknown** pp = p->m_vec.begin();
#ifdef _DEBUG
	if (m_pWorld)
		VWTRACE(m_pWorld, "VWSTUDIO", VWT_METHOD, "CControlMgrObject::FireUIUpdate type %d\n",enumType);
#endif
	// Loop through all connected event sinks and call their OnUIUpdate.
	while (pp < p->m_vec.end() /*&& S_OK == hr*/ )
	{
		if (*pp != NULL)
		{
			//IVWUIEvents* pIIVWUIEvents = (IVWUIEvents*)*pp;
			IVWUIEvents* pVWUIEvents = NULL;
			hr = (*pp)->QueryInterface(IID_IVWUIEvents, (LPVOID*) &pVWUIEvents);
			if (FAILED(hr))
			{
#ifdef _DEBUG
				VWTRACE(m_pWorld, "VWSTUDIO", VWT_ERROR, "CControlMgrObject::FireUIUpdate QI failed\n");
#endif
			}

			hr = pVWUIEvents->OnUIUpdate(enumType , pvwSrc , varHint, (bOnlyRefreshSource ? VARIANT_TRUE : VARIANT_FALSE) );
		}
		pp++;
		// E_INVALIDARG is an acceptable return value
		if( E_INVALIDARG == hr)
			hr = S_OK;

		// E_PERMISSION DENIED is an acceptable return value
		if ( hr == VWOBJECT_E_PERMISSION_DENIED )
			hr = S_OK;

		ASSERT( hr == S_OK );

// This loop doesn't error out if one of the controls reports an error.
#ifdef _DEBUG
	if(S_OK != hr)
	{
		if (m_pWorld)
			VWTRACE(m_pWorld, "VWSTUDIO", VWT_ERROR, "CControlMgrObject::FireUIUpdate pIIVWUIEvents->OnUIUpdate error hr = %x\n",hr);
	}
#endif
	ASSERT(S_OK == hr);
	}
	return S_OK;
}

HRESULT CControlMgrObject::IsBoundarySelected( IThing* pThing, int nBoundaryIndex, VARIANT_BOOL* vbSelected )
{
	IPropertyList * pSel = NULL;
	HRESULT hr = S_OK;
	long nCount, i;

	*vbSelected = VARIANT_FALSE;

	hr = get_SelectionList(&pSel);
	if( FAILED(hr)) goto EXIT_FAIL;

	hr = ((IPropertyCollection *)pSel)->get_Count(&nCount);
	if ( FAILED(hr) ) goto EXIT_FAIL;

	for (i = 0; i < nCount; i++)
	{
		CObjectPropertyPtr pObjPtr;
		CVWPickDataPtr pickDataPtr;
		CThingPtr pTmpThingPtr;
		int nTmpIndex;
			
		hr = pSel->get_ObjectProperty(i, &pObjPtr.p);
		if (FAILED(hr)) goto EXIT_FAIL;

		pickDataPtr = pObjPtr;
		if (pickDataPtr.p != NULL)
		{
			pickDataPtr->get_Thing(&pTmpThingPtr.p);
			pickDataPtr->get_BoundaryIndex(&nTmpIndex);

			if (pThing == pTmpThingPtr.p && nBoundaryIndex == nTmpIndex)
			{
				*vbSelected = VARIANT_TRUE;
				break;
			}
		}
	}
	
EXIT_FAIL:
	SAFERELEASE(pSel);

	return hr;
}


HRESULT CControlMgrObject::IsSelected( IObjectProperty* pObjProp, VARIANT_BOOL* vbSelected )
{
	IPropertyList * pSel = NULL;
	IObjectProperty* pobjprop = NULL;
	IBoundary* pBoundary = NULL;
	HRESULT hr = S_OK;
	long nCount, i;

	*vbSelected = VARIANT_FALSE;

	hr = get_SelectionList(&pSel);
	if( FAILED(hr)) goto EXIT_FAIL;

	hr = ((IPropertyCollection *)pSel)->get_Count(&nCount);
	if ( FAILED(hr) ) goto EXIT_FAIL;

	for (i = 0; i < nCount; i++)
	{
		hr = pSel->get_ObjectProperty(i, &pobjprop);
		if (FAILED(hr)) goto EXIT_FAIL;

		if (pobjprop == pObjProp)
		{
			*vbSelected = VARIANT_TRUE;
			break;
		}
		else
		{
			CVWPickDataPtr pickDataPtr;
			
			pickDataPtr = pobjprop;
			if (pickDataPtr.p != NULL)
			{
				pickDataPtr->get_Boundary(&pBoundary);

				if (pBoundary == pObjProp)
				{
					*vbSelected = VARIANT_TRUE;
					break;
				}
			}
		}

		SAFERELEASE(pBoundary);
		SAFERELEASE(pobjprop);
	}
	
EXIT_FAIL:
	SAFERELEASE(pobjprop);
	SAFERELEASE(pBoundary);
	SAFERELEASE(pSel);

	return hr;
}

HRESULT CControlMgrObject::IsVertexSelected( IObjectProperty* pObjProp, int nIndex, VARIANT_BOOL* vbSelected )
{
	IPropertyList * pSel = NULL;
	IObjectProperty* pobjprop = NULL;
	IBoundary* pBoundary = NULL;
	HRESULT hr = S_OK;
	long nCount, i;

	*vbSelected = VARIANT_FALSE;

	hr = get_SelectionList(&pSel);
	if( FAILED(hr)) goto EXIT_FAIL;

	hr = ((IPropertyCollection *)pSel)->get_Count(&nCount);
	if ( FAILED(hr) ) goto EXIT_FAIL;

	for (i = 0; i < nCount; i++)
	{
		hr = pSel->get_ObjectProperty(i, &pobjprop);
		if (FAILED(hr)) goto EXIT_FAIL;

		CVWPickDataPtr pickDataPtr;
		
		pickDataPtr = pobjprop;
		if (pickDataPtr.p != NULL)
		{
			pickDataPtr->get_Boundary(&pBoundary);

			if (pBoundary == pObjProp)
			{
				pickDataPtr->get_VertexPicked(nIndex, vbSelected);
				break;
			}
		}

		SAFERELEASE(pBoundary);
		SAFERELEASE(pobjprop);
	}
	
EXIT_FAIL:
	SAFERELEASE(pobjprop);
	SAFERELEASE(pBoundary);
	SAFERELEASE(pSel);

	return hr;

	return S_OK;
}

HRESULT CControlMgrObject::IsAnyVertexSelected( IObjectProperty* pObjProp, VARIANT_BOOL* vbSelected )
{
	IPropertyList * pSel = NULL;
	IObjectProperty* pobjprop = NULL;
	IBoundary* pBoundary = NULL;
	HRESULT hr = S_OK;
	long nCount, i;

	*vbSelected = VARIANT_FALSE;

	hr = get_SelectionList(&pSel);
	if( FAILED(hr)) goto EXIT_FAIL;

	hr = ((IPropertyCollection *)pSel)->get_Count(&nCount);
	if ( FAILED(hr) ) goto EXIT_FAIL;

	for (i = 0; i < nCount; i++)
	{
		hr = pSel->get_ObjectProperty(i, &pobjprop);
		if (FAILED(hr)) goto EXIT_FAIL;

		CVWPickDataPtr pickDataPtr;
		
		pickDataPtr = pobjprop;
		if (pickDataPtr.p != NULL)
		{
			pickDataPtr->get_Boundary(&pBoundary);

			if (pBoundary == pObjProp)
			{
				pickDataPtr->IsAnyVertexPicked(vbSelected);
				break;
			}
		}

		SAFERELEASE(pBoundary);
		SAFERELEASE(pobjprop);
	}
	
EXIT_FAIL:
	SAFERELEASE(pobjprop);
	SAFERELEASE(pBoundary);
	SAFERELEASE(pSel);

	return hr;

	return S_OK;
}

HRESULT CControlMgrObject::get_Clipboard( IPropertyList** pPropList)
{
	HRESULT hr = S_OK;

	if (!pPropList)
		hr = E_POINTER;
	else if( ! m_pClipboard )
		hr = E_FAIL;
	else
	{
		*pPropList = m_pClipboard;
		SAFEADDREF(m_pClipboard);
	}

	return hr;
}

HRESULT CControlMgrObject::Copy( IObjectProperty* pObjProp )
{
	HRESULT hr = S_OK;

	if( ! m_pClipboard )
		hr = E_FAIL;
	else
	{
		hr = E_NOTIMPL;
	}

	return hr;
}

HRESULT CControlMgrObject::Paste( IObjectProperty* pObjProp )
{
	HRESULT hr = S_OK;

	if( ! m_pClipboard )
		hr = E_FAIL;
	else
	{
		hr = E_NOTIMPL;
	}

	return hr;
}
