// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// PolyTool.h : Declaration of the CPolyTool

#ifndef __POLYTOOL_H_
#define __POLYTOOL_H_

#include "resource.h"       // main symbols
#include <vwsutils.h>
#include <uitlimpl.h>
#include <vwuiobjs.h>
#include <cellprop.h>
#include <vwsgfxut.h>
#include <bndytool.h>

EXTERN_C const IID LIBID_VWSTUDIOLib;
EXTERN_C const CLSID CLSID_PolyTool;

/////////////////////////////////////////////////////////////////////////////
// CPolyTool
DECLARE_VWUITOOLEX(CPolyTool, IPolyTool, IID_IPolyTool, CLSID_PolyTool, LIBID_VWSTUDIOLib)
{
public:
	CPolyTool();
	~CPolyTool();

DECLARE_REGISTRY_RESOURCEID(IDR_POLYTOOL)

BEGIN_COM_MAP(CPolyTool)
	COM_INTERFACE_ENTRY2(IDispatch,IPolyTool)
	COM_INTERFACE_ENTRY(IPolyTool)
	COM_INTERFACE_ENTRY(IVWUITool)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

private:
	IBoundary*			m_pBoundary;    
	IVWExecuteUndo*		m_pExecuteUndo;
	IThing*				m_pCurrentThing;
	IVWFrame*			m_pCurrentThingFrame;
	IPropertyList*		m_pCurrentBoundaryList;
	UINT				m_cVertices;      //Vertices in current boundary
	BOOL				m_fLastVertexLegal;
	float				m_flLastX, m_flLastZ;
	int				    m_nFirstVertexX, m_nFirstVertexY;
	int				    m_nLastMousePosX, m_nLastMousePosY;
	BOOL				m_bShowRubberBand;
	IDispatch*			m_piEditorViewControl;
	UINT				nTimerID;
	BOOL				m_bShowingInsertCursor;

	C2DThingCoordTransformer m_2DThingXform;

	STDMETHOD(StartNewBoundary)(IBoundary **ppEC);

	BOOL FindWithin(IThing* pContainer, IThing* pThingToFind);
	int TryInsVert(VARIANT* pvarHitResult, int nX, int nY);

	HRESULT fSelectionGetFirst(IPropertyList * pSelectionList, IObjectProperty**ppOP); 
	HRESULT InvokeToolEvent(int nNum, IObjectProperty* pOP, BSTR bstr1, BSTR bstr2, BSTR bstr3, VARIANT_BOOL bBubble);

	// Every time we try to add a new vertex with LButtonDown we set this.
	// LButtonDBLClick tests it to see whether to try to finish the EC.
public:
	STDMETHOD(IsValid)(IVWUIView*,VARIANT_BOOL *);												\
	STDMETHOD(put_ShowRubberBand)(VARIANT_BOOL newVal);
	STDMETHOD(get_ShowRubberBand)(VARIANT_BOOL* pVal);
	STDMETHOD(OnToolChanging)( VARIANT varHint );
	STDMETHOD(OnToolActivate)( VARIANT varHint );

	BOOL OnTimer( UINT nIDEvent, TIMERPROC* tmprc);

	virtual BOOL OnLButtonDown( UINT nFlags, POINT point );
	virtual BOOL OnMouseMove(  UINT nFlags, POINT ptScreen );
	virtual BOOL OnLButtonDblClk( UINT nFlags, POINT point );
	virtual BOOL OnKeyDown( UINT nVirtKey, LPARAM lKeyData );

	virtual BOOL ClosePolygon( UINT nFlags, POINT point );
};

#endif //__POLYTOOL_H_
