// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// D2DRMFrm.cpp : Implementation of Cd2dApp and DLL registration.

#include "stdafx.h"
#include "D2DTrans.h"
#include "DispList.h"
#include "D2DRMFrm.h"

/////////////////////////////////////////////////////////////////////////////
//

CDirect2DRMFrame::CDirect2DRMFrame()
: m_dsChildren(), m_dsVisuals()
{
	D2DOBJINIT();

	// hierarchy
	m_pParent = NULL;
	m_t2dLocal.Init();
}

CDirect2DRMFrame::~CDirect2DRMFrame()
{
	D2DOBJDESTROY();

	unsigned int cItems, i;

	// if the parent still exists, the ref count should still be at least 1
	MMASSERT(m_pParent == NULL);

	// delete the visuals
	cItems = m_dsVisuals.Items();
	for (i = 0; i < cItems; i++) {
		LPD2DRMVISUALPRIVATE pVisual = m_dsVisuals[i];
		MMASSERT(pVisual);
		pVisual->Release();
	}
	
	// delete the children
	cItems = m_dsChildren.Items();
	for (i = 0; i < cItems; i++) {
		LPD2DRMFRAMEPRIVATE pChild = m_dsChildren[i];
		MMASSERT(pChild);
		// make sure the parent is NULL'ed out before the child is released
		pChild->SetParent(NULL);
		pChild->Release();
	}
}


//
// frame construction
//
STDMETHODIMP 
CDirect2DRMFrame::AddChild(LPDIRECT2DRMFRAME pChild)
{
	LPD2DRMFRAMEPRIVATE pChildPrivate;
	HRESULT hr;

	// get a private interface to the Frame
	if ((pChild == NULL) ||
		FAILED(hr = pChild->QueryInterface(IID_ID2DRMFramePrivate, (void **) &pChildPrivate)))
		return E_INVALIDOBJECT;

	// if the child already has a parent, remove it from that parent's children
	LPD2DRMFRAMEPRIVATE pChildParent = pChildPrivate->GetParent();
	if (pChildParent) {
		hr = pChildParent->DeleteChild(pChild);
		// this can only fail because of a dangling pointer
		MMASSERT(SUCCEEDED(hr));
	}

	pChildPrivate->SetParent(this);
	m_dsChildren.Ins(pChildPrivate);

	return S_OK;
}

STDMETHODIMP 
CDirect2DRMFrame::DeleteChild(LPDIRECT2DRMFRAME pChild)
{
	unsigned int nIndex;
	HRESULT hr;
	LPD2DRMFRAMEPRIVATE pChildPrivate;

	// get a private interface for the child
	if ((pChild == NULL) || 
		FAILED(hr = pChild->QueryInterface(IID_ID2DRMFramePrivate, (void **) &pChildPrivate)))
		return E_INVALIDOBJECT;

	// find the child that is being deleted
	if (FAILED(m_dsChildren.Find(pChildPrivate, &nIndex)))
		return E_NOTFOUND;

	// remove the child from the list of children and decrease the ref count
	m_dsChildren.Del(nIndex);
	pChildPrivate->Release();

	// NULL out the child frame's parent
	pChildPrivate->SetParent(NULL);

	// release the local copy of the interface
	pChildPrivate->Release();

	return S_OK;
}

STDMETHODIMP 
CDirect2DRMFrame::get_Parent(LPDIRECT2DRMFRAME *ppParent)
{
	if (m_pParent) {
		// this should always work
		return m_pParent->QueryInterface(IID_IDirect2DRMFrame, (void **) ppParent);
	} else {
		*ppParent = NULL;
		return S_OK;
	}
}

STDMETHODIMP 
CDirect2DRMFrame::get_Children(LPDIRECT2DRMARRAY *prgChildren)
{
	HRESULT hr;

	// CoCreate the array object
	LPD2DRMARRAYPRIVATE pChildren;

	if (FAILED(hr = CoCreateInstance(
		CLSID_CDirect2DRMArray,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ID2DRMArrayPrivate,
		(LPVOID *) &pChildren)))
		return hr;

	// fill the array
	DWORD cItems = m_dsChildren.Items();
	for (DWORD i = 0; i < cItems; i++) {
		pChildren->InsElement(d2dotFrame, (LPDIRECT2DRMOBJECT) m_dsChildren[i]);
	}

	// QI for the public version
	hr = pChildren->QueryInterface(IID_IDirect2DRMArray, (LPVOID *) prgChildren);

	// release the private pointer
	MMRELEASE(pChildren);

	return hr;
}


STDMETHODIMP 
CDirect2DRMFrame::MoveChildToFront(LPDIRECT2DRMFRAME pChild)
{
	DSetHandle nIndex;
	HRESULT hr;
	LPD2DRMFRAMEPRIVATE pChildPrivate;

	// get a private interface for the child
	if ((pChild == NULL) || 
		FAILED(hr = pChild->QueryInterface(IID_ID2DRMFramePrivate, (void **) &pChildPrivate)))
		return E_INVALIDOBJECT;

	// find the child that is being deleted
	if (FAILED(m_dsChildren.Find(pChildPrivate, &nIndex)))
		return E_NOTFOUND;

	// remove the child from the list of children and reinsert
	m_dsChildren.Del(nIndex);
	m_dsChildren.Ins(pChildPrivate);

	// release the local copy of the interface
	pChildPrivate->Release();

	return S_OK;
}

STDMETHODIMP 
CDirect2DRMFrame::MoveChildToBack(LPDIRECT2DRMFRAME pChild)
{
	return E_NOTIMPL;
}

//
// visuals
//
STDMETHODIMP 
CDirect2DRMFrame::AddVisual(LPDIRECT2DRMVISUAL pVisual)
{
	LPD2DRMVISUALPRIVATE pVisualPrivate;
	HRESULT hr;

	// get a private interface to the Frame
	if ((pVisual == NULL) ||
		FAILED(hr = pVisual->QueryInterface(IID_ID2DRMVisualPrivate, (void **) &pVisualPrivate)))
		return E_INVALIDOBJECT;

	m_dsVisuals.Ins(pVisualPrivate);

	return S_OK;
}

STDMETHODIMP 
CDirect2DRMFrame::DeleteVisual(LPDIRECT2DRMVISUAL pVisual)
{
	DSetHandle nIndex;
	HRESULT hr;
	LPD2DRMVISUALPRIVATE pVisualPrivate;

	// get a private interface for the visual
	if ((pVisual == NULL) ||
		FAILED(hr = pVisual->QueryInterface(IID_ID2DRMVisualPrivate, (void **) &pVisualPrivate)))
		return E_INVALIDOBJECT;

	// find the visual that is being deleted
	if (FAILED(m_dsVisuals.Find(pVisualPrivate, &nIndex)))
		return E_NOTFOUND;

	// remove the visual from the list of visuals and decrease the ref count
	m_dsVisuals.Del(nIndex);
	pVisualPrivate->Release();

	// release the local copy of the interface
	pVisualPrivate->Release();

	return S_OK;
}


STDMETHODIMP 
CDirect2DRMFrame::get_Visuals(LPDIRECT2DRMARRAY *prgVisuals)
{
	return E_NOTIMPL;
}

STDMETHODIMP 
CDirect2DRMFrame::MoveVisualToFront(LPDIRECT2DRMVISUAL pVisual)
{
	DSetHandle nIndex;
	HRESULT hr;
	LPD2DRMVISUALPRIVATE pVisualPrivate;

	// get a private interface for the visual
	if ((pVisual == NULL) ||
		FAILED(hr = pVisual->QueryInterface(IID_ID2DRMVisualPrivate, (void **) &pVisualPrivate)))
		return E_INVALIDOBJECT;

	// find the visual that is being deleted
	if (FAILED(m_dsVisuals.Find(pVisualPrivate, &nIndex)))
		return E_NOTFOUND;

	// remove the visual from the list of visuals and reinsert
	m_dsVisuals.Del(nIndex);
	m_dsVisuals.Ins(pVisualPrivate);

	// release the local copy of the interface
	pVisualPrivate->Release();

	return S_OK;
}

STDMETHODIMP 
CDirect2DRMFrame::MoveVisualToBack(LPDIRECT2DRMVISUAL pVisual)
{
	return E_NOTIMPL;
}


//
// transformations
//
STDMETHODIMP 
CDirect2DRMFrame::AddTranslation(float fX, float fY)
{
	m_t2dLocal.Translate(fX, fY);
	return S_OK;
}

STDMETHODIMP 
CDirect2DRMFrame::SetTranslation(float fX, float fY)
{
	m_t2dLocal.SetTranslation(fX, fY);
	return S_OK;
}

STDMETHODIMP 
CDirect2DRMFrame::AddScale(float fX, float fY)
{
	m_t2dLocal.Scale(fX, fY);
	return S_OK;
}

STDMETHODIMP 
CDirect2DRMFrame::SetScale(float fX, float fY)
{
	m_t2dLocal.SetScale(fX, fY);
	return S_OK;
}

STDMETHODIMP 
CDirect2DRMFrame::AddRotation(float fTheta)
{
	return E_NOTIMPL;
}


STDMETHODIMP
CDirect2DRMFrame::get_XOffset(float *pfX)
{
	*pfX = m_t2dLocal.GetXPos();
	return S_OK;
}

STDMETHODIMP
CDirect2DRMFrame::put_XOffset(float fX)
{
	return E_NOTIMPL;
}

STDMETHODIMP
CDirect2DRMFrame::get_YOffset(float *pfY)
{
	*pfY = m_t2dLocal.GetYPos();
	return S_OK;
}

STDMETHODIMP
CDirect2DRMFrame::put_YOffset(float fY)
{
	return E_NOTIMPL;
}

STDMETHODIMP
CDirect2DRMFrame::InverseTransform(LPD2DVECTOR pvSrc, LPD2DVECTOR pvDst)
{
	if (pvSrc && pvDst) {
		m_t2dLocal.InverseTransform(*pvSrc, pvDst);
		return S_OK;
	} else {
		return E_INVALIDOBJECT;
	}
}

STDMETHODIMP
CDirect2DRMFrame::Transform(LPD2DVECTOR pvSrc, LPD2DVECTOR pvDst)
{
	if (pvSrc && pvDst) {
		m_t2dLocal.Transform(*pvSrc, pvDst);
		return S_OK;
	} else {
		return E_INVALIDOBJECT;
	}
}

STDMETHODIMP
CDirect2DRMFrame::TransformPt(LPD2DPOINT ppt2Src, LPD2DPOINT ppt2Dst)
{
	if (ppt2Src && ppt2Dst) {
		m_t2dLocal.Transform(*ppt2Src, ppt2Dst);
		return S_OK;
	} else {
		return E_INVALIDOBJECT;
	}
}


STDMETHODIMP
CDirect2DRMFrame::GetInverseTransform(D2DTransform *pTransform)
{
	MMASSERT(pTransform);
	m_t2dLocal.GetInverse(pTransform);
	return S_OK;
}



//
// DATA ACCESS
//
// hierarchy

// Function: GetScene
//    Return the root frame node, bumping the ref count.  Unlike D3D, if this node
//  is the root node, a pointer to this node will be returned and the ref count bumped.
STDMETHODIMP
CDirect2DRMFrame::GetScene(LPDIRECT2DRMFRAME *ppScene)
{
	LPD2DRMFRAMEPRIVATE pParentFrame, pCurFrame = this;

	for (pParentFrame = pCurFrame->GetParent(); pParentFrame; pParentFrame = pCurFrame->GetParent()) {
		pCurFrame = pParentFrame;
	}

	pCurFrame->AddRef();
	*ppScene = pCurFrame;

	return S_OK;
}

//
// transformations
//
STDMETHODIMP
CDirect2DRMFrame::GetPosition(LPDIRECT2DRMFRAME pRefFrame, float *pfX, float *pfY)
{
	// TODO: need to implement private stuff that returns the relative offsets

	return E_NOTIMPL;
}


//
// rendering
//
STDMETHODIMP
CDirect2DRMFrame::RenderToSurface(LPDIRECTDRAWSURFACE pdds, LPD2DRECT prDst, DWORD dwFlags)
{
	D2DTransform t2d;
	t2d.Init();

	return RenderToSurface(pdds, prDst, &t2d, dwFlags);
}


HRESULT
GetSurfaceDimensions(LPDIRECTDRAWSURFACE pdds, LPD2DRECT prDimensions)
{
	MMASSERT(pdds && prDimensions);

	HRESULT hr;
	DDSURFACEDESC ddsd;
	INIT_DXSTRUCT(ddsd);
	if (FAILED(hr = pdds->GetSurfaceDesc(&ddsd))) {
		MMASSERT(SUCCEEDED(hr));
		return hr;
	}
	prDimensions->left = 0.f;
	prDimensions->top = 0.f;
	prDimensions->right = ((float) ddsd.dwWidth);
	prDimensions->bottom = ((float) ddsd.dwHeight);

	return S_OK;
}


STDMETHODIMP
CDirect2DRMFrame::RenderToSurface(LPDIRECTDRAWSURFACE pdds, LPD2DRECT pr2Dst,
								  D2DTransform *pt2dCurrent, DWORD dwFlags)
{
	if ((pdds == NULL) || (pt2dCurrent == NULL))
		return E_INVALIDOBJECT;

	D2DRect r2Dst;
	if (pr2Dst == NULL) {
		::GetSurfaceDimensions(pdds, &r2Dst);
		pr2Dst = &r2Dst;
	}

	HRESULT hr;
	CDisplayList dlScene;
	if (FAILED(hr = GetDisplayList(&dlScene, pt2dCurrent)) ||
		FAILED(hr = dlScene.Clip(*pr2Dst)) ||
		FAILED(hr = dlScene.BltToSurface(pdds, dwFlags))) {
		// do nothing...
	}

	return hr;
}

HRESULT
CDirect2DRMFrame::Pick(LPD2DPOINT ppt2, DWORD nType, LPD2DRMARRAYPRIVATE pAncestory, 
							 LPD2DRMARRAYPRIVATE pPicks)
{
	DWORD nIndex = pAncestory->InsElement(nType, (LPDIRECT2DRMOBJECT) this);

	HRESULT hr = S_OK;

	// transform the point into this frame
	D2DPoint pt2New;
	m_t2dLocal.InverseTransform(*ppt2, &pt2New);

	int i;
	DWORD cItems;
	cItems = m_dsChildren.Items();
	for (i = cItems - 1; i >= 0; i--) {
		if (FAILED(hr = m_dsChildren[i]->PickFrame(&pt2New, pAncestory, pPicks)))
			goto ePick;
	}
	cItems = m_dsVisuals.Items();
	for (i = cItems - 1; i >= 0; i--) {
		if (FAILED(hr = m_dsVisuals[i]->PickVisual(&pt2New, pAncestory, pPicks)))
			goto ePick;
	}

ePick:
	pAncestory->DelElement(nIndex);
	return hr;
}


//
// ID2DRMFramePrivate Interface
//
LPD2DRMFRAMEPRIVATE
CDirect2DRMFrame::GetParent()
{
	// this GetParent is from the private interface and will not AddRef
	return m_pParent;
}

void
CDirect2DRMFrame::SetParent(LPD2DRMFRAMEPRIVATE pParent)
{
	m_pParent = pParent;
}


HRESULT
CDirect2DRMFrame::GetDisplayList(CDisplayList *pdlScene, D2DTransform *pt2dCurrent)
{
	HRESULT hr = S_OK;
	int i, cItems;

	// create a new transformation on the program stack
	D2DTransform t2dNew;

	// add the local transformation to the transformation stack
	// Tn = Tc(Tl(x)) ==> T = T0(T1(T2(...Tn(x))..)), where T0 is the rootmost transformation
	pt2dCurrent->Transform(m_t2dLocal, &t2dNew);

	//
	// the display list is constructed last in first out
	//
	// put on the visuals for the children
	cItems = m_dsChildren.Items();
	for (i = cItems - 1; i >= 0; i--) {
		if (FAILED(hr = m_dsChildren[i]->GetFrameDisplayList(pdlScene, &t2dNew)))
			return hr;
	}
	cItems = m_dsVisuals.Items();
	for (i = cItems - 1; i >= 0; i--) {
		if (FAILED(hr = m_dsVisuals[i]->GetDisplayList(pdlScene, &t2dNew)))
			return hr;
	}

	return S_OK;
}

