// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// vwsutils.h
#ifndef __vwsutils_h__
#define __vwsutils_h__


#include <afxdisp.h>
#include <vwprop.h>
#include <vwgeom.h>
#include <cmddel.h>

/// Review: how to create new VT_'s (doesn't collide with anything in wtypes.h)
#define VT_POINT 73
#define VT_RECT	74
#define VT_POINT_BYREF (VT_BYREF|VT_POINT)
#define VT_RECT_BYREF (VT_BYREF|VT_RECT)

#define FLOAT_TO_LONG_MULTIPLE 100L
#define FLOAT_TO_LONG(x) (long)(x*FLOAT_TO_LONG_MULTIPLE)
#define LONG_TO_FLOAT(x) (float)(x/(float)FLOAT_TO_LONG_MULTIPLE)


// Prototypes of helper functions.

CString PropMap_GetBSTRProp( IPropertyMap * pPM, char * pszName ) ;
HRESULT Thing_AddBSTRProp( IThing * pThing, char * pszName, char * pszValue );
HRESULT Thing_AddObjectProp( IThing * pThing, char * pszName, IObjectProperty * pObject );
HRESULT World_CreateStockExemplar( IWorld * pWorld, char * pszName, char * pszValue, IThing** ppthing );
HRESULT World_CreateExemplar( IWorld * pWorld, char * pszName, char * pszValue, IThing* pthingParent, IThing** ppthing );
//HRESULT Thing_CreateInstance( IThing * pThing, char * pszName, IThing** ppthing );
HRESULT Thing_IsOfType( IThing * pThing, char * pszName, VARIANT_BOOL * fIsOfType );
// Thing_GetObjectPropertiesMap: Expects *ppPMap to be NULL.
HRESULT Thing_GetObjectPropertiesMap( IThing * pThing, IPropertyMap ** ppPMap );


/////////////////////////////////////////////////////////////////////////////
// GenUndoItemData
// This class is implented solely to allow for cleanup of the undo item data


class CGenUndoData
{
public:
	CGenUndoData(){};
	virtual ~CGenUndoData(){};
};


#endif
