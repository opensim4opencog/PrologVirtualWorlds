// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// RotTool.h : Declaration of the CRotate3DTool

#ifndef __ROTATE3DTOOL_H_
#define __ROTATE3DTOOL_H_

#include <afxtempl.h>
#include "resource.h"       // main symbols
#include <propbase.h>
#include <uitlimpl.h>
#include "geomsel.h"
#include <vwnvtool.h>

EXTERN_C const IID LIBID_VWSTUDIOLib;
EXTERN_C const CLSID CLSID_Rotate3DTool;

/////////////////////////////////////////////////////////////////////////////
// CRotate3DTool
DECLARE_VWUITOOLEX(CRotate3DTool, IRotate3DTool, IID_IRotate3DTool, CLSID_Rotate3DTool, LIBID_VWSTUDIOLib)
{

public:
	CRotate3DTool();
	~CRotate3DTool();

DECLARE_REGISTRY_RESOURCEID(IDR_ROTATE3DTOOL)

BEGIN_COM_MAP(CRotate3DTool)
	COM_INTERFACE_ENTRY(IRotate3DTool)
	COM_INTERFACE_ENTRY(IVWUITool)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

	virtual BOOL OnLButtonDown( UINT nFlags, POINT point );
	virtual BOOL OnLButtonUp( UINT nFlags, POINT point );
	virtual BOOL OnMouseMove( UINT nFlags, POINT point );
	virtual BOOL OnKeyDown( UINT nVirtKey, LPARAM lKeyData );
	virtual BOOL OnKeyUp( UINT nVirtKey, LPARAM lKeyData );
	virtual BOOL OnMessage( UINT nIDEvent, WPARAM wParam, LPARAM lParam );

	STDMETHOD(IsValid)(IVWUIView*,VARIANT_BOOL *);												\
	STDMETHOD(OnUIEvent)(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient);
	STDMETHOD(Update)();
	STDMETHOD(ComputeEulerAngles)(IVWFrame* pVWFrame, IVector* vecAngles);

	HRESULT RotateSelectedObjects( float fDeltaX, float fDeltaY );
	HRESULT SetupTranformList();
	HRESULT DestroyTransformList();
	HRESULT OnDoneRotating();
	HRESULT InvokeToolEvent(int nNum, IThing* pThing, BSTR bstr1, BSTR bstr2, BSTR bstr3, VARIANT_BOOL bBubble);
	HRESULT SetupEnvironment();

	float ElapsedFrameTime();
	float GetXRotation(float fX, float fY, float fZ);
	float GetYRotation(float fX, float fY, float fZ);
	float GetZRotation(float fX, float fY, float fZ);

// IRotate3DTool
public:
	STDMETHOD(Initialize)(IVWUIView* pVw, IWorld* pWorld, IVWRenderRoot *pVWRenderRoot);

protected:
	IVWRenderRoot*		m_pVWRenderRoot;
	LPDIRECT3DRMFRAME	m_pRMCameraFrame;
	CTransform3DList	m_TransformList;
	VARIANT_BOOL		m_bTrackingMouse;
	IVWExecuteUndo*		m_pExecuteUndo;
	CPoint				m_ptAnchor, m_cptLast;
	IWorld*				m_pWorld;
	IVector*			m_pVector;
	long				m_nAxisLock;
	IDispatch*			m_piEditorViewControl;
	VARIANT_BOOL		m_bUsingKeyboard;
	UINT				m_KeyBindings[kbMax];
	int					m_nCameraMode;
	VARIANT_BOOL		m_bGravity;
	VARIANT_BOOL		m_bGotFocus;
	VARIANT_BOOL		m_bShowingNoCursor;
	HWND		        m_hWnd;
	LPDIRECT3DRMFRAME	m_pRMDummyFrame;

};

#endif //__ROTATE3DTOOL_H_
