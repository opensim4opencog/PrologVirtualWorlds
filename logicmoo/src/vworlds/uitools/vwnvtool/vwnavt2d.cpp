// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWNavT2D.cpp : Implementation of CVWNvToolApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include <VWNvTool.h>
#include "VWNavT2D.h"

/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP CVWNavigationTool2D::Init(IVWRenderRoot *pVWRenderRoot)
{
	return S_OK;
}

STDMETHODIMP CVWNavigationTool2D::put_NavigationTarget(IThing *pThing)
{
	return S_OK;
}

STDMETHODIMP CVWNavigationTool2D::SetNavigationPath(IPropertyMap *pmapPositions, IPropertyMap *pmapDirections, BSTR bstrDoneEvent)
{
	return S_OK;
}

STDMETHODIMP CVWNavigationTool2D::get_Moving(VARIANT_BOOL *bMoving)
{
	return S_OK;
}

STDMETHODIMP CVWNavigationTool2D::put_OrthogonalMode(VARIANT_BOOL bOrthogonal)
{
	return S_OK;
}
STDMETHODIMP CVWNavigationTool2D::put_TopOrthogonalMode(VARIANT_BOOL bOrthogonal)
{
	return S_OK;
}
STDMETHODIMP CVWNavigationTool2D::put_FrontOrthogonalMode(VARIANT_BOOL bOrthogonal)
{
	return S_OK;
}

STDMETHODIMP CVWNavigationTool2D::put_RightOrthogonalMode(VARIANT_BOOL bOrthogonal)
{
	return S_OK;
}

STDMETHODIMP CVWNavigationTool2D::get_CollisionDetection(VARIANT_BOOL *pVal)
{
	return S_OK;
}

STDMETHODIMP CVWNavigationTool2D::put_CollisionDetection(VARIANT_BOOL newVal)
{
	return S_OK;
}

STDMETHODIMP CVWNavigationTool2D::get_Gravity(VARIANT_BOOL *pVal)
{
	return S_OK;
}

STDMETHODIMP CVWNavigationTool2D::put_Gravity(VARIANT_BOOL newVal)
{
	return S_OK;
}
