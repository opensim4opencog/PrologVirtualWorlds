// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef GEOM_SEL_H
#define GEOM_SEL_H

#include <afxcoll.h>
struct IVWFrame;

class CGeometrySelection
{
public:
	//CGeometrySelection(IThing *pThing, IVWFrame *pVWFrame, IVWFrame *pBoxFrame);
	CGeometrySelection(IThing *pThing, IDirect3DRMFrame* pD3DFrame, IDirect3DRMMeshBuilder *pBoxBuilder);
	~CGeometrySelection();

	IThing *m_pThing;						// selected thing
	IDirect3DRMMeshBuilder* m_pBoxBuilder;
	IDirect3DRMFrame*		m_pD3DFrame;	// selected D3D frame eventually
//	IVWFrame *m_pVWFrame;					// selected VW Frame
//	IVWFrame *m_pBoxFrame;					// Bounding Box frame
};

class CGeometrySelectionList
{

public:
	CGeometrySelectionList();
	~CGeometrySelectionList();

	void AddSelection(CGeometrySelection *pSel);
	void RemoveSelection(IThing *pThing);

	CGeometrySelection *GetSelection(IThing *pThing);

	void ClearList();


	CPtrList m_list;
};

#endif