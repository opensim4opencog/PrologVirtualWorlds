// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWUndoItem.cpp : Implementation of CVWUndoItem
#include "stdafx.h"
#include <d3drm.h>
#include <stdtools.h>
#include "UndoItem.h"
#include "UndoUtil.h"

/////////////////////////////////////////////////////////////////////////////
// CVWUndoItem

CVWUndoItem::CVWUndoItem()
{
	m_pexun = NULL;
	VariantInit(&m_varUndoTarget);
	VariantInit(&m_varUndoData);
	VariantInit(&m_varRedoData);
	m_iUndoHints = NONE;
}

CVWUndoItem::~CVWUndoItem()
{
//	BoundaryVertex* pECTmp, *pECNext;

	SAFERELEASE(m_pexun);
	if (m_varUndoTarget.vt == VT_BYREF)
	{
		free(m_varUndoTarget.byref);
		m_varUndoTarget.byref = NULL;
	}
	VariantClear(&m_varUndoTarget);
	// special hack since we use this a lot
	if (m_varUndoData.vt == VT_BYREF)
	{
		if ( ((UndoData*)(m_varUndoData.byref))->m_ot == NEWBOUNDARY )
			((UndoData*)(m_varUndoData.byref))->extra.newboundary.BoundaryList->Release();
/*
		if ( ((UndoData*)(m_varUndoData.byref))->m_ot == TRANSLATE2D &&
			 ((UndoData*)(m_varUndoData.byref))->extra.trans.nSelectedVerts )
			free( ((UndoData*)(m_varUndoData.byref))->extra.trans.nSelectedVerts );
		else if ( ((UndoData*)(m_varUndoData.byref))->m_ot == DELETEBOUNDARY )
		{
			pECTmp = ((UndoData*)(m_varUndoData.byref))->extra.deletevertices.pECVerts;
			while (pECTmp)
			{
				pECNext = pECTmp->next;
				delete pECTmp;
				pECTmp = pECNext;
			}
		}
*/
		free(m_varUndoData.byref);
		m_varUndoData.byref = NULL;
	}
	VariantClear(&m_varUndoData);

	if (m_varRedoData.vt == VT_BYREF)
	{
		if ( ((UndoData*)(m_varRedoData.byref))->m_ot == NEWBOUNDARY )
			((UndoData*)(m_varRedoData.byref))->extra.newboundary.BoundaryList->Release();
/*
		if ( ((UndoData*)(m_varRedoData.byref))->m_ot == TRANSLATE2D &&
			((UndoData*)(m_varRedoData.byref))->extra.trans.nSelectedVerts )
			free( ((UndoData*)(m_varRedoData.byref))->extra.trans.nSelectedVerts );
		else if ( ((UndoData*)(m_varRedoData.byref))->m_ot == DELETEBOUNDARY )
		{
			pECTmp = ((UndoData*)(m_varRedoData.byref))->extra.deletevertices.pECVerts;
			while (pECTmp)
			{
				pECNext = pECTmp->next;
				delete pECTmp;
				pECTmp = pECNext;
			}
		}
*/
		free(m_varRedoData.byref);
		m_varRedoData.byref = NULL;
	}
	VariantClear(&m_varRedoData);
}

STDMETHODIMP CVWUndoItem::Undo()
{
	HRESULT hr = S_OK;

	if (m_pexun)
		hr = m_pexun->Undo(this);
	
	return hr;
}

STDMETHODIMP CVWUndoItem::Redo()
{
	HRESULT hr = S_OK;

	if (m_pexun)
		hr = m_pexun->Redo(this);
	
	return hr;
}

STDMETHODIMP CVWUndoItem::get_Label(BSTR * pVal)
{
	if (pVal)
		*pVal = m_bstrLabel.Copy();
	else
		return E_POINTER;
	return S_OK;
}

STDMETHODIMP CVWUndoItem::put_Label(BSTR newVal)
{
	m_bstrLabel = newVal;

	return S_OK;
}

STDMETHODIMP CVWUndoItem::get_UndoHints(int * pVal)
{
	if (pVal)
		*pVal = m_iUndoHints;
	else
		return E_POINTER;
	return S_OK;
}

STDMETHODIMP CVWUndoItem::put_UndoHints(int newVal)
{
	if ((newVal < NONE) || (newVal > END_UNDO_GROUP))
		return E_FAIL;

	m_iUndoHints = newVal;

	return S_OK;
}

STDMETHODIMP CVWUndoItem::get_UndoData(VARIANT * pVal)
{
	HRESULT hr = S_OK;
//	*pVal = m_varUndoData;
	if (FAILED(hr = VariantClear(pVal)))
		return hr;

	if (m_varUndoData.vt != VT_BYREF)
		return VariantCopy(pVal, &m_varUndoData);

	// unlike the above, this doesn't do a copy
	// is that going to be a problem?
	pVal->vt = m_varUndoData.vt;
	pVal->byref = m_varUndoData.byref;

	return hr;
}

STDMETHODIMP CVWUndoItem::put_UndoData(VARIANT newVal)
{
	HRESULT hr = S_OK;
	
	if (FAILED(hr = VariantClear(&m_varUndoData)))
		return hr;

	// VariantCopy barfs on BYREFs
	if (newVal.vt != VT_BYREF)
		return VariantCopy(&m_varUndoData, &newVal);

	m_varUndoData.vt = newVal.vt;
	m_varUndoData.byref = newVal.byref;

	return hr;
}

STDMETHODIMP CVWUndoItem::get_RedoData(VARIANT * pVal)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = VariantClear(pVal)))
		return hr;

	if (m_varRedoData.vt != VT_BYREF)
		return VariantCopy(pVal, &m_varRedoData);

	// unlike the above, this doesn't do a copy
	// is that going to be a problem?
	pVal->vt = m_varRedoData.vt;
	pVal->byref = m_varRedoData.byref;

	return hr;

	//*pVal = m_varRedoData;
}

STDMETHODIMP CVWUndoItem::put_RedoData(VARIANT newVal)
{
	HRESULT hr;
	
	if (FAILED(hr = VariantClear(&m_varRedoData)))
		return hr;

	return VariantCopy(&m_varRedoData, &newVal);

	//return S_OK;
}

STDMETHODIMP CVWUndoItem::get_UndoTarget(VARIANT * pVal)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = VariantClear(pVal)))
		return hr;

	if (m_varUndoTarget.vt != VT_BYREF)
		return VariantCopy(pVal, &m_varUndoTarget);

	// unlike the above, this doesn't do a copy
	// is that going to be a problem?
	pVal->vt = m_varUndoTarget.vt;
	pVal->byref = m_varUndoTarget.byref;

	return hr;

	//*pVal = m_varUndoTarget;

	return S_OK;
}

STDMETHODIMP CVWUndoItem::put_UndoTarget(VARIANT newVal)
{
	HRESULT hr;
	
	if (FAILED(hr = VariantClear(&m_varUndoTarget)))
		return hr;

	return VariantCopy(&m_varUndoTarget, &newVal);

	//return S_OK;
}

STDMETHODIMP CVWUndoItem::get_ExecuteUndo(IVWExecuteUndo ** ppexun)
{
	if (m_pexun)
	{
		m_pexun->AddRef();
		ppexun = &m_pexun;
	}
	else
		ppexun = NULL;

	return S_OK;
}

STDMETHODIMP CVWUndoItem::put_ExecuteUndo(IVWExecuteUndo * pexun)
{
	m_pexun = pexun;

	return S_OK;
}
