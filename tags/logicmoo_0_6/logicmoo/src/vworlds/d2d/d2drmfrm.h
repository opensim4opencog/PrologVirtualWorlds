// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// D2DRMFrm.h : Declaration of the CDirect2DRMFrame


#include "resource.h"       // main symbols
#include "d2dbase.h"
#include "DSet.h"

/////////////////////////////////////////////////////////////////////////////
// d2d

class ATL_NO_VTABLE CDirect2DRMFrame :
	public CComDualImpl<ID2DRMFramePrivate, &IID_IDirect2DRMFrame, &LIBID_D2DLib>,
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<CDirect2DRMFrame, &CLSID_CDirect2DRMFrame>,
	public ID2DRMVisualPrivate
{
public:
	CDirect2DRMFrame();
	~CDirect2DRMFrame();

	BEGIN_COM_MAP(CDirect2DRMFrame)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IDirect2DRMObject)
		COM_INTERFACE_ENTRY(IDirect2DRMVisual)
		COM_INTERFACE_ENTRY(IDirect2DRMFrame)
		COM_INTERFACE_ENTRY(ID2DRMFramePrivate)
		COM_INTERFACE_ENTRY(ID2DRMVisualPrivate)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
	END_COM_MAP()

	D2DOBJECT_BASE_CLASS(Frame)
	
	D2DOBJREG_DECL(Frame, D2DRMFRM);
	
public:
	//
	// IDirect2DRMFrame Interface
	//

	//
	// CONSTRUCTION
	//
	// hierarchy
	STDMETHOD(AddChild)(LPDIRECT2DRMFRAME pChild);
	STDMETHOD(DeleteChild)(LPDIRECT2DRMFRAME pChild);
	STDMETHOD(get_Parent)(LPDIRECT2DRMFRAME *ppParent);
	STDMETHOD(get_Children)(LPDIRECT2DRMARRAY *prgChildren);
	STDMETHOD(MoveChildToFront)(LPDIRECT2DRMFRAME pChild);
	STDMETHOD(MoveChildToBack)(LPDIRECT2DRMFRAME pChild);
	// visuals
	STDMETHOD(AddVisual)(LPDIRECT2DRMVISUAL pVisual);
	STDMETHOD(DeleteVisual)(LPDIRECT2DRMVISUAL pVisual);
	STDMETHOD(get_Visuals)(LPDIRECT2DRMARRAY *prgVisuals);
	STDMETHOD(MoveVisualToFront)(LPDIRECT2DRMVISUAL pVisual);
	STDMETHOD(MoveVisualToBack)(LPDIRECT2DRMVISUAL pVisual);
	// transforms
	STDMETHOD(AddTranslation)(float fX, float fY);
	STDMETHOD(SetTranslation)(float fX, float fY);
	STDMETHOD(AddScale)(float fX, float fY);
	STDMETHOD(SetScale)(float fX, float fY);
	STDMETHOD(AddRotation)(float fTheta);
	STDMETHOD(get_XOffset)(float *pfX);
	STDMETHOD(put_XOffset)(float fX);
	STDMETHOD(get_YOffset)(float *pfY);
	STDMETHOD(put_YOffset)(float fY);
//	STDMETHOD(GetTransform)(LPDIRECT2DRMFRAME *ppd2dRMFrame);
//	STDMETHOD(SetTransform)(LPDIRECT2DRMFRAME pd2dRMFrame);
	// rendering
	STDMETHOD(RenderToSurface)(LPDIRECTDRAWSURFACE pdds, LPD2DRECT prDst, DWORD dwFlags);
	STDMETHOD(InverseTransform)(LPD2DVECTOR pvSrc, LPD2DVECTOR pvDst);
	STDMETHOD(Transform)(LPD2DVECTOR pvSrc, LPD2DVECTOR pvDst);
	STDMETHOD(TransformPt)(LPD2DPOINT ppt2Src, LPD2DPOINT ppt2Dst);

	//
	// DATA ACCESS
	//
	// hierarchy
	STDMETHOD(GetScene)(LPDIRECT2DRMFRAME *ppScene);
	// transforms
	STDMETHOD(GetPosition)(LPDIRECT2DRMFRAME pRefFrame, float *pfX, float *pfY);


	//
	// ID2DRMVisualPrivate Interface
	//
	STDMETHOD(GetDisplayList)(CDisplayList *pdlScene, D2DTransform *pt2dCurrent);
	STDMETHOD(PickVisual)(LPD2DPOINT ppt, LPD2DRMARRAYPRIVATE pAncestory, 
					LPD2DRMARRAYPRIVATE pPicks);


	//
	// ID2DRMFramePrivate Interface
	//
	STDMETHOD_(void, SetParent)(LPD2DRMFRAMEPRIVATE pParent);
	STDMETHOD_(LPD2DRMFRAMEPRIVATE, GetParent)();
	STDMETHOD(GetFrameDisplayList)(CDisplayList *pdlScene, D2DTransform *pt2dCurrent);
	STDMETHOD(RenderToSurface)(LPDIRECTDRAWSURFACE pdds, LPD2DRECT prDst, D2DTransform *pt2dCurrent, DWORD dwFlags);
	STDMETHOD(PickFrame)(LPD2DPOINT ppt, LPD2DRMARRAYPRIVATE pAncestory, 
				LPD2DRMARRAYPRIVATE pPicks);
	STDMETHOD(GetInverseTransform)(D2DTransform *pTransform);

private:
	HRESULT		Pick(LPD2DPOINT ppt, DWORD nType, LPD2DRMARRAYPRIVATE pAncestory, 
					LPD2DRMARRAYPRIVATE pPicks);
	
private:
	// frame transformation data
	D2DTransform					m_t2dLocal;

	// tree hierarchy data
	LPD2DRMFRAMEPRIVATE				m_pParent;
	DSet<LPD2DRMVISUALPRIVATE>		m_dsVisuals;
	DSet<LPD2DRMFRAMEPRIVATE>		m_dsChildren;
};


inline STDMETHODIMP
CDirect2DRMFrame::GetFrameDisplayList(CDisplayList *pdlScene, D2DTransform *pt2dCurrent)
{ 
	return GetDisplayList(pdlScene, pt2dCurrent);
}

inline STDMETHODIMP
CDirect2DRMFrame::PickVisual(LPD2DPOINT ppt, LPD2DRMARRAYPRIVATE pAncestory, 
							 LPD2DRMARRAYPRIVATE pPicks)
{
	return Pick(ppt, d2dotVisual, pAncestory, pPicks);
}

inline STDMETHODIMP
CDirect2DRMFrame::PickFrame(LPD2DPOINT ppt, LPD2DRMARRAYPRIVATE pAncestory, 
					   LPD2DRMARRAYPRIVATE pPicks)
{
	return Pick(ppt, d2dotFrame, pAncestory, pPicks);
}

