// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// ScaleTl.h : Declaration of the CVWScale3DTool

#ifndef __VWSCALE3DTOOL_H_
#define __VWSCALE3DTOOL_H_

#include <afxtempl.h>
#include "resource.h"       // main symbols
#include <propbase.h>
#include <uitlimpl.h>
#include "geomsel.h"
#include <vwnvtool.h>

EXTERN_C const IID LIBID_VWSTUDIOLib;
EXTERN_C const CLSID CLSID_VWScale3DTool;

/////////////////////////////////////////////////////////////////////////////
// CVWScale3DTool
DECLARE_VWUITOOLEX(CVWScale3DTool, IVWScale3DTool, IID_IVWScale3DTool, CLSID_VWScale3DTool, LIBID_VWSTUDIOLib)
{
public:

	CVWScale3DTool();
	~CVWScale3DTool();

DECLARE_REGISTRY_RESOURCEID(IDR_VWSCALE3DTOOL)

BEGIN_COM_MAP(CVWScale3DTool)
	COM_INTERFACE_ENTRY(IVWScale3DTool)
	COM_INTERFACE_ENTRY(IVWUITool)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

	//Event handlers from UITool
	virtual BOOL OnLButtonDown( UINT nFlags, POINT point );
	virtual BOOL OnMouseMove( UINT nFlags, POINT point );
	virtual BOOL OnLButtonUp( UINT nFlags, POINT point );
	virtual BOOL OnKeyDown( UINT nVirtKey, LPARAM lKeyData );
	virtual BOOL OnKeyUp( UINT nVirtKey, LPARAM lKeyData );
	virtual BOOL OnMessage( UINT nIDEvent, WPARAM wParam, LPARAM lParam );

public:
	//UITool overloads
	STDMETHOD(IsValid)(IVWUIView*,VARIANT_BOOL *);
	STDMETHOD(Initialize)(IVWUIView* pVw, IWorld* pWorld, IVWRenderRoot *pVWRenderRoot);
	STDMETHOD(Update)();
	STDMETHOD(OnUIEvent)(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient);
	STDMETHOD(ComputeEulerAngles)(IVWFrame* pVWFrame, IVector* vecAngles);

	//Helper functions
	HRESULT ScaleSelectedObjects( float flDeltaX, float flDeltaY );
	HRESULT OnDoneScaling();
	HRESULT InvokeToolEvent(int nNum, IThing* pThing, BSTR bstr1, BSTR bstr2, BSTR bstr3, VARIANT_BOOL bBubble);
	float ElapsedFrameTime();
	HRESULT SetupTranformList();
	HRESULT DestroyTransformList();
	HRESULT SetupEnvironment();

// IVWScale3DTool
public:
protected:
	D3DVECTOR				m_vecScale;
	IVWRenderRoot*			m_pVWRenderRoot;
	CTransform3DList		m_TransformList;
	CPoint					m_cptLast;
	CPoint					m_ptAnchor;
	VARIANT_BOOL			m_bTrackingMouse;
	D3DVECTOR				m_vecOriginalScale;
	IWorld*					m_pWorld;
	IVector*				m_pVector;
	long					m_nAxisLock;
	IDispatch*				m_piEditorViewControl;
	UINT					m_KeyBindings[kbMax];
	LPDIRECT3DRMFRAME		m_pRMCameraFrame;
	VARIANT_BOOL			m_bGravity;
	int						m_nCameraMode;
	VARIANT_BOOL			m_bUsingKeyboard;
	VARIANT_BOOL			m_bGotFocus;
	D3DVECTOR				m_vecViewportZero;
	VARIANT_BOOL			m_bShowingNoCursor;

};

#endif //__VWSCALE3DTOOL_H_
