// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// D2DRMHSp.cpp : Implementation of Cd2dApp and DLL registration.

#include "stdafx.h"
#include "DispList.h"
#include "D2DRMHSp.h"

/////////////////////////////////////////////////////////////////////////////
//

CDirect2DRMHotSpot::CDirect2DRMHotSpot()
{
	D2DOBJINIT();
}

CDirect2DRMHotSpot::~CDirect2DRMHotSpot()
{
	D2DOBJDESTROY();
}

STDMETHODIMP
CDirect2DRMHotSpot::GetDisplayList(CDisplayList *pdlScene, D2DTransform *pt2dCurrent)
{
	return S_OK;
}

STDMETHODIMP
CDirect2DRMHotSpot::PickVisual(LPD2DPOINT ppt, LPD2DRMARRAYPRIVATE pAncestory, 
							   LPD2DRMARRAYPRIVATE pPicks)
{
	return E_NOTIMPL;
}
