// VWFrameO.cpp : Implementation of CVWFrameApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include <VWIData.h>
#include "VWIDataO.h"

IMPLEMENT_OBJECTPROPERTY(CVWIntersectDataObject)

/////////////////////////////////////////////////////////////////////////////
//

CVWIntersectDataObject::CVWIntersectDataObject()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVWIntersectDataObject::CVWIntersectDataObject, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif

	m_pBoundaryList = NULL;
	m_iEdgeIndex = -1;
	m_bPassed = 0;
	m_bTransition = -1;
	m_pvProjection = NULL;
}

CVWIntersectDataObject::~CVWIntersectDataObject()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVWIntersectDataObject::~CVWIntersectDataObject, this 0x%0x\n", this);
#endif

	Terminate();
}

STDMETHODIMP CVWIntersectDataObject::Terminate(void)
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVWIntersectDataObject::Terminate, this 0x%0x\n", this);
#endif

	CVWIntersectDataObjectBase::Terminate();

	SAFERELEASE(m_pBoundaryList);
	SAFERELEASE(m_pvProjection);

	return S_OK;
}

STDMETHODIMP CVWIntersectDataObject::get_BoundaryList(IPropertyList **ppCell)
{
	*ppCell = m_pBoundaryList;
	SAFEADDREF(m_pBoundaryList);
	return S_OK;
}

STDMETHODIMP CVWIntersectDataObject::put_BoundaryList(IPropertyList *pCell)
{
	SAFERELEASE(m_pBoundaryList);
	m_pBoundaryList = pCell;
	SAFEADDREF(pCell);
	return S_OK;
}

STDMETHODIMP CVWIntersectDataObject::get_BoundaryIndex(int *piBoundaryIndex)
{
	*piBoundaryIndex = m_iBoundaryIndex;
	return S_OK;
}

STDMETHODIMP CVWIntersectDataObject::put_BoundaryIndex(int iBoundaryIndex)
{
	m_iBoundaryIndex = iBoundaryIndex;
	return S_OK;
}

STDMETHODIMP CVWIntersectDataObject::get_EdgeIndex(int *piEdgeIndex)
{
	*piEdgeIndex = m_iEdgeIndex;
	return S_OK;
}

STDMETHODIMP CVWIntersectDataObject::put_EdgeIndex(int iEdgeIndex)
{
	m_iEdgeIndex = iEdgeIndex;
	return S_OK;
}

STDMETHODIMP CVWIntersectDataObject::get_DistToIntersect(float *pfDist)
{
	*pfDist = m_fDistToIntersect;
	return S_OK;
}

STDMETHODIMP CVWIntersectDataObject::put_DistToIntersect(float fDist)
{
	m_fDistToIntersect = fDist;
	return S_OK;
}

STDMETHODIMP CVWIntersectDataObject::get_Passed(BYTE *pbPassed)
{
	*pbPassed = m_bPassed;
	return S_OK;
}

STDMETHODIMP CVWIntersectDataObject::put_Passed(BYTE bPassed)
{
	m_bPassed = bPassed;
	return S_OK;
}

STDMETHODIMP CVWIntersectDataObject::get_Transition(BYTE *pbTransition)
{
	*pbTransition = m_bTransition;
	return S_OK;
}

STDMETHODIMP CVWIntersectDataObject::put_Transition(BYTE bTransition)
{
	m_bTransition = bTransition;
	return S_OK;
}

STDMETHODIMP CVWIntersectDataObject::get_Projection(IVector **ppvProjection)
{
	*ppvProjection = m_pvProjection;
	SAFEADDREF(m_pvProjection);
	return S_OK;
}

STDMETHODIMP CVWIntersectDataObject::put_Projection(IVector *pvProjection)
{
	SAFERELEASE(m_pvProjection);
	m_pvProjection = pvProjection;
	SAFEADDREF(pvProjection);
	return S_OK;
}

STDMETHODIMP CVWIntersectDataObject::Get(IPropertyList **ppCell, int *piBoundaryIndex,
										 int *piEdgeIndex, float *pfDist, BYTE *pbPassed, 
										 BYTE *pbTransition, IVector **ppvProjection)
{
	if (ppCell)
	{
		*ppCell = m_pBoundaryList;
		SAFEADDREF(m_pBoundaryList);
	}
	if (piBoundaryIndex)
		*piBoundaryIndex = m_iBoundaryIndex;
	if (piEdgeIndex)
		*piEdgeIndex = m_iEdgeIndex;
	if (pfDist)
		*pfDist = m_fDistToIntersect;
	if (pbPassed)
		*pbPassed = m_bPassed;
	if (pbTransition)
		*pbTransition = m_bTransition;
	if (ppvProjection)
	{
		*ppvProjection = m_pvProjection;
		SAFEADDREF(m_pvProjection);
	}
	return S_OK;
}

STDMETHODIMP CVWIntersectDataObject::Marshall(DWORD dwOptions, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = CVWIntersectDataObjectBase::Marshall( dwOptions, pbuffer )))
		return hr;

	ASSERT(pbuffer);

	if (m_pBoundaryList)
	{
		if (FAILED(hr = m_pBoundaryList->Marshall(dwOptions, pbuffer)))
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pbuffer->put_Int(m_iBoundaryIndex)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pbuffer->put_Int(m_iEdgeIndex)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pbuffer->put_Float(m_fDistToIntersect)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pbuffer->put_Byte(m_bPassed)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pbuffer->put_Byte(m_bTransition)))
		goto ERROR_ENCOUNTERED;
	if (m_pvProjection)
	{
		if (FAILED(hr = m_pvProjection->Marshall(dwOptions, pbuffer)))
		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	
	return hr;
}

STDMETHODIMP CVWIntersectDataObject::UnMarshall(IUnMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = CVWIntersectDataObjectBase::UnMarshall( pbuffer )))
		return hr;

	ASSERT(pbuffer);

	if (m_pBoundaryList)
	{
		if (FAILED(hr = m_pBoundaryList->UnMarshall(pbuffer)))
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pbuffer->get_Int(&m_iBoundaryIndex)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pbuffer->get_Int(&m_iEdgeIndex)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pbuffer->get_Float(&m_fDistToIntersect)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pbuffer->get_Byte(&m_bPassed)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pbuffer->get_Byte(&m_bTransition)))
		goto ERROR_ENCOUNTERED;
	if (m_pvProjection)
	{
		if (FAILED(hr = m_pvProjection->UnMarshall(pbuffer)))
		goto ERROR_ENCOUNTERED;
	}
	
ERROR_ENCOUNTERED:
	
	return hr;
}
