// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWFrameO.cpp : Implementation of CVWFrameApp and DLL registration.

#define VWMM

#include "stdafx.h"
#include <vwobject.h>
#include "pickData.h"
#include "PickDatO.h"

IMPLEMENT_OBJECTPROPERTY(CVWPickDataObject)

/////////////////////////////////////////////////////////////////////////////
//

CVWPickDataObject::CVWPickDataObject()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVWPickDataObject::CVWPickDataObject, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif

	m_pBoundaryList = NULL;
	m_pThing = NULL;
	m_pBoundary = NULL;
	m_iEdgeIndex = -1;
}

CVWPickDataObject::~CVWPickDataObject()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVWPickDataObject::~CVWPickDataObject, this 0x%0x\n", this);
#endif

	Terminate();
}

STDMETHODIMP CVWPickDataObject::Terminate(void)
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVWPickDataObject::Terminate, this 0x%0x\n", this);
#endif

	CVWPickDataObjectBase::Terminate();

	SAFERELEASE(m_pBoundaryList);
	SAFERELEASE(m_pThing);
	SAFERELEASE(m_pBoundary);

	return S_OK;
}

STDMETHODIMP CVWPickDataObject::get_BoundaryList(IPropertyList **ppBoundaryList)
{
	*ppBoundaryList = m_pBoundaryList;
	SAFEADDREF(m_pBoundaryList);
	return S_OK;
}

STDMETHODIMP CVWPickDataObject::put_BoundaryList(IPropertyList *pBoundaryList)
{
	SAFERELEASE(m_pBoundaryList);
	m_pBoundaryList = pBoundaryList;
	ASSERT(m_pBoundaryList);
	SAFEADDREF(pBoundaryList);
	return S_OK;
}

STDMETHODIMP CVWPickDataObject::get_Thing(IThing **ppThing)
{
	*ppThing = m_pThing;
	SAFEADDREF(m_pThing);
	return S_OK;
}

STDMETHODIMP CVWPickDataObject::put_Thing(IThing *pThing)
{
	SAFERELEASE(m_pThing);
	m_pThing = pThing;
	SAFEADDREF(pThing);
	return S_OK;
}

STDMETHODIMP CVWPickDataObject::get_Boundary(IBoundary **ppBoundary)
{
	*ppBoundary = m_pBoundary;
	SAFEADDREF(m_pBoundary);
	return S_OK;
}

STDMETHODIMP CVWPickDataObject::put_Boundary(IBoundary *pBoundary)
{
	SAFERELEASE(m_pBoundary);
	m_pBoundary = pBoundary;
	ASSERT(m_pBoundary);
	SAFEADDREF(pBoundary);
	return S_OK;
}

STDMETHODIMP CVWPickDataObject::get_BoundaryIndex(int *piBoundaryIndex)
{
	*piBoundaryIndex = m_iBoundaryIndex;
	return S_OK;
}

STDMETHODIMP CVWPickDataObject::put_BoundaryIndex(int iBoundaryIndex)
{
	m_iBoundaryIndex = iBoundaryIndex;
	return S_OK;
}

STDMETHODIMP CVWPickDataObject::get_EdgeIndex(int *piEdgeIndex)
{
	*piEdgeIndex = m_iEdgeIndex;
	return S_OK;
}

STDMETHODIMP CVWPickDataObject::put_EdgeIndex(int iEdgeIndex)
{
	m_iEdgeIndex = iEdgeIndex;
	return S_OK;
}


STDMETHODIMP CVWPickDataObject::get_DataType(int *piDataType)
{
	*piDataType = m_iDataType;
	return S_OK;
}

STDMETHODIMP CVWPickDataObject::put_DataType(int iDataType)
{
	m_iDataType= iDataType;
	return S_OK;
}


STDMETHODIMP CVWPickDataObject::Marshall(DWORD dwOptions, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = CVWPickDataObjectBase::Marshall( dwOptions, pbuffer )))
		return hr;

	ASSERT(pbuffer);

	if (FAILED(hr = m_pWorld->MarshallObjectProperty(m_pBoundary, clsidUnknown, dwOptions, pbuffer)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pWorld->MarshallObjectProperty(m_pBoundaryList, clsidUnknown, dwOptions, pbuffer)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pWorld->MarshallObjectProperty(m_pThing, clsidThing, dwOptions, pbuffer)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pbuffer->put_Int(m_iBoundaryIndex)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pbuffer->put_Int(m_iEdgeIndex)))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	
	return hr;
}

STDMETHODIMP CVWPickDataObject::UnMarshall(IUnMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = CVWPickDataObjectBase::UnMarshall( pbuffer )))
		return hr;

	ASSERT(pbuffer);

	if (FAILED(hr = m_pWorld->UnMarshallObjectProperty(pbuffer, (IObjectProperty**) &m_pBoundary)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pWorld->UnMarshallObjectProperty(pbuffer, (IObjectProperty**) &m_pBoundaryList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pWorld->UnMarshallObjectProperty(pbuffer, (IObjectProperty**) &m_pThing)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pbuffer->get_Int(&m_iBoundaryIndex)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pbuffer->get_Int(&m_iEdgeIndex)))
		goto ERROR_ENCOUNTERED;
	
ERROR_ENCOUNTERED:
	
	return hr;
}


STDMETHODIMP CVWPickDataObject::get_VertexPicked(int nIndex, VARIANT_BOOL* vbPicked)
{
	if (!vbPicked)
		return E_POINTER;

	*vbPicked = VARIANT_FALSE;

	if (nIndex < m_pickedVertexArray.GetSize())
	{
		*vbPicked = (m_pickedVertexArray[nIndex] ? VARIANT_TRUE : VARIANT_FALSE);
		return S_OK;
	}

	return S_OK;
}

STDMETHODIMP CVWPickDataObject::put_VertexPicked(int nIndex, VARIANT_BOOL vbPicked)
{
	if (nIndex >= m_pickedVertexArray.GetSize())
	{
		m_pickedVertexArray.SetSize(nIndex + 1);
	}

	m_pickedVertexArray[nIndex] = (vbPicked ? 1 : 0) ;

	return S_OK;
}

		

STDMETHODIMP CVWPickDataObject::IsAnyVertexPicked(VARIANT_BOOL* pvbPicked)
{

	int i, nCount = m_pickedVertexArray.GetSize();

	if (!pvbPicked)
		return E_INVALIDARG;

	*pvbPicked = VARIANT_FALSE;

	for (i = 0; i < nCount; i++)
	{
		if (m_pickedVertexArray[i])
			*pvbPicked = VARIANT_TRUE;
	}

	return S_OK;
}

STDMETHODIMP CVWPickDataObject::get_Name(BSTR* bstrName)
{
	CComBSTR bstrThingName;
	CString szNum;

	if (!bstrName)
		return E_POINTER;

	if (!m_pThing)
		return E_FAIL;

	m_pThing->get_Name(&bstrThingName.m_str);

	szNum.Format("%d", m_iBoundaryIndex);

	bstrThingName.Append(" - Boundary: ");
	bstrThingName.Append(szNum);

	*bstrName = bstrThingName.Copy();

	return S_OK;
}
