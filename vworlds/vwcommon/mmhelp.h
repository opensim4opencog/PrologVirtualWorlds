// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include <vector.h>
#include <vwgeom.h>
#include <vwgeomsp.h>
#include <vwadata.h>
#include <pickdata.h>
#include <isound.h>
#include <cellprop.h>

HRESULT CreateGeometry(IWorld* pWorld, BSTR bstrGeometryName, IVWGeometry **ppgeometry);
HRESULT CreateFrame(IWorld* pWorld, IVWFrame **ppframe);
HRESULT CreateSpritedGeometry(IWorld* pWorld, BSTR bstrGeometryName, IVWGeometrySprite **ppgeometry);
HRESULT CreateVector(IWorld* pWorld, float x, float y, float z, IVector** ppVector);
HRESULT CreateAccessoryData(IWorld* pWorld, BSTR bstrAccessoryGraphic, float fPosX, float fPosY, IVWAccessoryData **ppgeometry);
HRESULT CreateSound(IWorld* pWorld,BSTR SoundURL, VARIANT_BOOL bIs3D,VARIANT_BOOL bIsLooping, ISound **ppSound);
HRESULT CreateBoundaryList(IWorld* pWorld, IPropertyList** ppcell);
HRESULT CreatePickData(IWorld* pWorld, IVWPickData** ppPickData);
HRESULT CreateBoundary(IWorld* pWorld, IBoundary** ppBoundary);

HRESULT GetNumericFromVariant(VARIANT& varNumber, float* fDest);
