// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// Undo Data Stuff

#ifndef UNDO_UTIL_H
#define UNDO_UTIL_H


enum OperationType {
	TRANSLATE,
	ROTATE,
	SCALE,
	TRANSLATE2D,
	ROTATE2D,
	SCALE2D,
	NEWBOUNDARY,
	INSERTVERTEX2D,
	DELETEBOUNDARY,
	DELETEVERTEX2D,
	FLIPBOUNDARY,
};

struct BoundaryVertex
{
	float x;
	float y;
	int index;
};

struct UndoData {
	OperationType m_ot;
	union {
		struct {
			D3DVECTOR oldPosition;
			D3DVECTOR newPosition;
			int		  nSelectedVert;
		} trans;
		struct {
			D3DVECTOR oldScale;
			D3DVECTOR newScale;
		} scale;
		struct {
			D3DVECTOR oldDirection;
			D3DVECTOR oldUp;

			D3DVECTOR newDirection;
			D3DVECTOR newUp;
		} rotate;
		struct {
			IPropertyList*	  BoundaryList;
		} newboundary;
		struct {
			BoundaryVertex	ecVert;
			IPropertyList*	  BoundaryList;
		} deletevertex;
		struct {
			BoundaryVertex	ecVert;
			IPropertyList*	  BoundaryList;
		} insertvertex;

	} extra;
};


UndoData *CreateUndoData(OperationType ot);
IVWUndoItem *CreateUndoItem(IVWExecuteUndo *pExecuteUndo);


#endif