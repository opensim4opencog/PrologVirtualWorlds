// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// D2DRMHSp.h : Declaration of the CDirect2DRMHotSpot


#include "resource.h"       // main symbols
#include "d2dbase.h"

/////////////////////////////////////////////////////////////////////////////
// d2d

class ATL_NO_VTABLE CDirect2DRMHotSpot : 
	D2DOBJECT_BASE(HotSpot),
	public ID2DRMVisualPrivate
{
public:
	CDirect2DRMHotSpot();
	~CDirect2DRMHotSpot();

	BEGIN_COM_MAP(CDirect2DRMHotSpot)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IDirect2DRMObject)
		COM_INTERFACE_ENTRY(IDirect2DRMVisual)
		COM_INTERFACE_ENTRY(IDirect2DRMHotSpot)
		COM_INTERFACE_ENTRY(ID2DRMVisualPrivate)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
	END_COM_MAP()

	D2DOBJECT_BASE_CLASS(HotSpot)
	
	D2DOBJREG_DECL(HotSpot, D2DRMHSP);

public:
	//
	// IDirect2DRMHotSpot Interface
	//

	//
	// ID2DRMVisualPrivate Interface
	//
	STDMETHOD(GetDisplayList)(CDisplayList *pdlScene, D2DTransform *pt2dCurrent);
	STDMETHOD(PickVisual)(LPD2DPOINT ppt, LPD2DRMARRAYPRIVATE pAncestory, 
					LPD2DRMARRAYPRIVATE pPicks);
};
