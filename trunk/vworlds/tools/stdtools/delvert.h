// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// DelVert.h : Declaration of the CDelVert

#ifndef __DELVERT_H_
#define __DELVERT_H_

#include "resource.h"       // main symbols
#include <vwsutils.h>
#include <uitlimpl.h>
#include <vwuiobjs.h>
#include <cellprop.h>
#include <vwsgfxut.h>
#include <bndytool.h>

EXTERN_C const IID LIBID_VWSTUDIOLib;
EXTERN_C const CLSID CLSID_DeleteEdgeCycVertex;

/////////////////////////////////////////////////////////////////////////////
// CDelVert
DECLARE_VWUITOOLEX(CDelVert, IDeleteVertex, IID_IDeleteVertex, CLSID_DeleteEdgeCycVertex, LIBID_VWSTUDIOLib)
{
public:

DECLARE_REGISTRY_RESOURCEID(IDR_DELVERT)

BEGIN_COM_MAP(CDelVert)
	COM_INTERFACE_ENTRY2(IDispatch, IDeleteVertex)
	COM_INTERFACE_ENTRY(IVWUITool)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

// IDeleteVertex
public:

	IVWExecuteUndo *m_pExecuteUndo;
	float m_flLastX, m_flLastY , m_flLastZ;
	C2DThingCoordTransformer m_2DThingXform;

	CDelVert();

	virtual BOOL OnLButtonDown( UINT nFlags, POINT point );
	virtual BOOL OnLButtonUp( UINT nFlags, POINT point );
	STDMETHOD(IsValid)(IVWUIView* pVW,VARIANT_BOOL * pB);
	BOOL TryDelVert(IBoundary* pBoundary, float flX, float flY, float flZ, IPropertyList* pBoundaryList);
	HRESULT fSelectionIsSingleEdgeOrThing(IPropertyList * pPL, IObjectProperty **ppOP);
};

#endif //__DELVERT_H_
