// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// D2DRMPkA.h : Declaration of the CDirect2DRMPick


#include "resource.h"       // main symbols
#include "d2dbase.h"

/////////////////////////////////////////////////////////////////////////////
// d2d

class ATL_NO_VTABLE CDirect2DRMPick :
	public CComDualImpl<ID2DRMPickPrivate, &IID_IDirect2DRMPick, &LIBID_D2DLib>,
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<CDirect2DRMPick, &CLSID_CDirect2DRMPick>
{
public:
	CDirect2DRMPick();
	~CDirect2DRMPick();

	BEGIN_COM_MAP(CDirect2DRMPick)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IDirect2DRMObject)
		COM_INTERFACE_ENTRY(IDirect2DRMPick)
		COM_INTERFACE_ENTRY(ID2DRMPickPrivate)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
	END_COM_MAP()

	D2DOBJECT_BASE_CLASS(Pick)
	
	D2DOBJREG_DECL(Pick, D2DRMPKA);
	
public:
	//
	// IDirect2DRMPick Interface
	//
	STDMETHOD(get_Visual)(LPDIRECT2DRMVISUAL *ppVisual);
	STDMETHOD(get_Ancestory)(LPDIRECT2DRMARRAY *prgFrames);
	STDMETHOD(get_X)(float *pfX) { MMASSERT(pfX); *pfX = m_pt2Pos.x; return S_OK; }
	STDMETHOD(get_Y)(float *pfY) { MMASSERT(pfY); *pfY = m_pt2Pos.y; return S_OK; }
	STDMETHOD(get_ControlFrame)(LPDIRECT2DRMFRAME *ppParentFrame);
	STDMETHOD(MoveToFront)();
	STDMETHOD(MoveVisual)(LPD2DVECTOR pvWorldDelta);

	//
	// ID2DRMPickPrivate Interface
	//
	STDMETHOD(Init)(LPD2DPOINT ppt2HitPos, LPD2DRMVISUALPRIVATE pVisual, LPD2DRMARRAYPRIVATE pAncestory);

private:
	LPD2DRMARRAYPRIVATE			m_pAncestory;
	LPD2DRMVISUALPRIVATE		m_pVisual;
	D2DPoint					m_pt2Pos;
};
