// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// InsVert.h : Declaration of the CInsVert

#ifndef __INSVERT_H_
#define __INSVERT_H_

#include "resource.h"       // main symbols
#include <vwsutils.h>
#include <uitlimpl.h>
#include <vwuiobjs.h>
#include <cellprop.h>
#include <vwsgfxut.h>
#include <bndytool.h>

EXTERN_C const IID LIBID_VWSTUDIOLib;
EXTERN_C const CLSID CLSID_VWInsertEdgeCycVert;

/////////////////////////////////////////////////////////////////////////////
// CInsVert
DECLARE_VWUITOOLEX(CInsVert, IVWInsertEdgeCycVert, IID_IVWInsertEdgeCycVert, CLSID_VWInsertEdgeCycVert, LIBID_VWSTUDIOLib)
{
public:

DECLARE_REGISTRY_RESOURCEID(IDR_INSVERT)

BEGIN_COM_MAP(CInsVert)
	COM_INTERFACE_ENTRY2(IDispatch, IVWInsertEdgeCycVert)
	COM_INTERFACE_ENTRY(IVWUITool)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

// IVWInsertEdgeCycVert
public:
	float m_flLastX, m_flLastY , m_flLastZ;
	C2DThingCoordTransformer m_2DThingXform;
	IVWExecuteUndo *m_pExecuteUndo;

	CInsVert();
	virtual BOOL OnLButtonDown( UINT nFlags, POINT point );
	virtual BOOL OnLButtonUp( UINT nFlags, POINT point );
	STDMETHOD(IsValid)(IVWUIView* pVW,VARIANT_BOOL * pB);
	HRESULT fSelectionIsSingleEdgeOrThing(IPropertyList * pPL, IObjectProperty **ppOP);
	BOOL TryInsVert(IBoundary* pBoundary, float flX, float flY, float flZ, IPropertyList* pBoundaryList);
};

#endif //__INSVERT_H_
