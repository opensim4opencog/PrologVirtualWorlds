// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// D2DRMPkA.cpp : Implementation of Cd2dApp and DLL registration.

#include "stdafx.h"
#include "D2DRMPkA.h"

/////////////////////////////////////////////////////////////////////////////
//

CDirect2DRMPick::CDirect2DRMPick()
{
	D2DOBJINIT();

	m_pAncestory = NULL;
	m_pVisual = NULL;
}

CDirect2DRMPick::~CDirect2DRMPick()
{
	D2DOBJDESTROY();

	MMRELEASE(m_pAncestory);
	MMRELEASE(m_pVisual);
}

STDMETHODIMP
CDirect2DRMPick::get_Visual(LPDIRECT2DRMVISUAL *ppVisual)
{
	return m_pVisual->QueryInterface(IID_IDirect2DRMVisual, (void **) ppVisual);
}

STDMETHODIMP
CDirect2DRMPick::get_Ancestory(LPDIRECT2DRMARRAY *prgFrames)
{
	return m_pAncestory->QueryInterface(IID_IDirect2DRMArray, (void **) prgFrames);
}

STDMETHODIMP
CDirect2DRMPick::Init(LPD2DPOINT ppt2HitPos, LPD2DRMVISUALPRIVATE pVisual, 
					  LPD2DRMARRAYPRIVATE pAncestory)
{
	HRESULT hr;
	
	MMASSERT(ppt2HitPos && pVisual && pAncestory);
	
	if (m_pVisual)
		return E_ALREADYINITIALIZED;
	
	MMASSERT((m_pVisual == NULL) && (m_pAncestory == NULL));
	
	// copy the ancestory array for storage
	if (FAILED(hr = pAncestory->Copy(&m_pAncestory)))
		return hr;
	
	m_pVisual = pVisual;
	pVisual->AddRef();
	
	m_pt2Pos.x = ppt2HitPos->x;
	m_pt2Pos.y = ppt2HitPos->y;
	
	return S_OK;
}

STDMETHODIMP
CDirect2DRMPick::MoveToFront()
{
	HRESULT hr = S_OK;
	LPDIRECT2DRMFRAME pFrame = NULL, pLastFrame = NULL;
	DWORD nType, nLastType, dwDepth;

	// get the number of ancestors for the visual
	m_pAncestory->get_Size(&dwDepth);
	MMASSERT(dwDepth > 0);

	// move the visual to the front of its parent frame
	m_pAncestory->GetElement(dwDepth - 1, &nLastType, (LPDIRECT2DRMOBJECT *) &pLastFrame);
	MMASSERT(pLastFrame);
	if (FAILED(hr = pLastFrame->MoveVisualToFront((LPDIRECT2DRMVISUAL) m_pVisual))) 
		goto e_MoveToFront;

	// move each frame to the front of its parent's child or visual list
	for (dwDepth--; SUCCEEDED(hr) && (dwDepth != 0); dwDepth--) {
		m_pAncestory->GetElement(dwDepth - 1, &nType, (LPDIRECT2DRMOBJECT *) &pFrame);
		MMASSERT(pFrame);
		if (nLastType == d2dotFrame) {
			hr = pFrame->MoveChildToFront(pLastFrame);
		} else {
			// the frame was actually a visual of its parent
			MMASSERT(nLastType == d2dotVisual);
			hr = pFrame->MoveVisualToFront(pLastFrame);
		}

		MMRELEASE(pLastFrame);
		pLastFrame = pFrame;
		nLastType = nType;
	}
	
e_MoveToFront:
	MMRELEASE(pLastFrame);
	return hr;
}

STDMETHODIMP
CDirect2DRMPick::get_ControlFrame(LPDIRECT2DRMFRAME *ppFrame)
{
	MMASSERT(m_pAncestory);

	HRESULT hr;
	DWORD nDepth, nType;
	m_pAncestory->get_Size(&nDepth);
	MMASSERT(nDepth > 0);

	hr = m_pAncestory->GetElement(nDepth - 1, &nType, (LPDIRECT2DRMOBJECT *) ppFrame);
	MMASSERT((nType == d2dotFrame) || (nType == d2dotVisual));

	return hr;
}

STDMETHODIMP
CDirect2DRMPick::MoveVisual(LPD2DVECTOR pvWorldDelta)
{
	MMASSERT(m_pAncestory);
	if (pvWorldDelta == NULL)
		return E_INVALIDOBJECT;

	D2DVector vWorldDelta;
	vWorldDelta.x = pvWorldDelta->x;
	vWorldDelta.y = pvWorldDelta->y;

	LPDIRECT2DRMFRAME pFrame = NULL;
	HRESULT hr;
	DWORD nDepth, nType;
	m_pAncestory->get_Size(&nDepth);
	MMASSERT(nDepth > 0);

	// transform the vector from world coordinates to controlling vector coordinates
	for (DWORD i = 0; i < nDepth - 1; i++) {
		if (FAILED(hr = m_pAncestory->GetElement(i, &nType, (LPDIRECT2DRMOBJECT *) &pFrame)))
			goto e_MoveVisual;
		MMASSERT(pFrame);
		pFrame->InverseTransform(&vWorldDelta, &vWorldDelta);
		pFrame->Release();
	}
	if (FAILED(hr = m_pAncestory->GetElement(nDepth - 1, &nType, (LPDIRECT2DRMOBJECT *) &pFrame)))
		goto e_MoveVisual;

	// transform the control frame, if it exists
	pFrame->AddTranslation(vWorldDelta.x, vWorldDelta.y);

e_MoveVisual:
	MMRELEASE(pFrame);

	return hr;
}
