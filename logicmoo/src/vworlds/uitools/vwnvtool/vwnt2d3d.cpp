// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWNavT2D.cpp : Implementation of CVWNvToolApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include <VWNvTool.h>
#include "VWNT2D3D.h"

STDMETHODIMP CVWNavigationTool2D3D::Init(IVWRenderRoot *pVWRenderRoot)
{
	return S_OK;
}

STDMETHODIMP CVWNavigationTool2D3D::put_NavigationTarget(IThing *pThing)
{
	return S_OK;
}

STDMETHODIMP CVWNavigationTool2D3D::SetNavigationPath(IPropertyMap *pmapPositions, IPropertyMap *pmapDirections, BSTR bstrDoneEvent)
{
	return S_OK;
}

STDMETHODIMP CVWNavigationTool2D3D::get_Moving(VARIANT_BOOL *bMoving)
{
	return S_OK;
}

STDMETHODIMP CVWNavigationTool2D3D::put_OrthogonalMode(VARIANT_BOOL bOrthogonal)
{
	return S_OK;
}
