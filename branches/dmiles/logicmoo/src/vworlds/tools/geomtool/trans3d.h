// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// trans3D.h : Declaration of the CVWTranslate3DTool

#ifndef __VWTRANSLATE3DTOOL_H_
#define __VWTRANSLATE3DTOOL_H_

#include <afxtempl.h>
#include "resource.h"       // main symbols
#include <uitlimpl.h>
#include <vwsutils.h>
#include <vwtrans.h>
#include "geomsel.h"
#include <vwnvtool.h>

typedef CVWComPtr<IVWTransform,NULL,&IID_IVWTransform> CVWTransformPtr;

class CVWTranslate3DTool;
class CVWExecuteGeomUndo;

EXTERN_C const IID LIBID_VWSTUDIOLib;
EXTERN_C const CLSID CLSID_VWTranslate3DTool;

/////////////////////////////////////////////////////////////////////////////
// CVWTranslate3DTool
DECLARE_VWUITOOLEX(CVWTranslate3DTool, IVWTranslate3DTool, IID_IVWTranslate3DTool, CLSID_VWTranslate3DTool, LIBID_VWSTUDIOLib)
{
public:

	CVWTranslate3DTool();
	~CVWTranslate3DTool();

DECLARE_REGISTRY_RESOURCEID(IDR_VWTRANSLATE3DTOOL)

BEGIN_COM_MAP(CVWTranslate3DTool)
	COM_INTERFACE_ENTRY(IVWTranslate3DTool)
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
	virtual BOOL OnRButtonDown( UINT nFlags, POINT point );
	virtual BOOL OnRButtonUp( UINT nFlags, POINT point );

	STDMETHOD(IsValid)(IVWUIView*,VARIANT_BOOL *);												\

// IVWTranslate3DTool
	STDMETHOD(Initialize)(IVWUIView* pVw, IWorld* pWorld, IVWRenderRoot *pVWRenderRoot);
	STDMETHOD(Update)();
	STDMETHOD(OnUIEvent)(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient);

public:
	HRESULT InvokeCenterViewOnObject(IThing* pThing);
	HRESULT GetObjectCenter(D3DVECTOR& center, IVWFrame * pTrans);
	HRESULT InvokeToolEvent(int nNum, IThing* pThing, BSTR bstr1, BSTR bstr2, BSTR bstr3, VARIANT_BOOL bBubble);
	HRESULT TranslateSelectedObjects( float flDeltaX, float flDeltaY, BOOL bUsingRButton  );
	HRESULT SetupTranformList();
	HRESULT DestroyTransformList();
	HRESULT OnDoneTranslating();
	HRESULT SetupEnvironment();

	float ElapsedFrameTime();

protected:
	CTransform3DList	m_TransformList;
	CPoint				m_cptLast;
	BOOL				m_bFirstTime;
	IVWExecuteUndo*		m_pExecuteUndo;
	IVWControlManager*	m_pControlManager;
	IVWRenderRoot*		m_pVWRenderRoot;
	VARIANT_BOOL		m_bTrackingMouseLButton;
	VARIANT_BOOL		m_bTrackingMouseRButton;
	VARIANT_BOOL		m_bUsingKeyboard;
	VARIANT_BOOL		m_bInOrbitMode;
	D3DVECTOR			m_vOrbitPoint;
	D3DVECTOR			m_vOrbitDirection;
	D3DVECTOR			m_vOrbitUp;
	D3DVECTOR			m_vOrbitLeft;
	float				m_fHorizontalOrbitPercent;
	float				m_fVerticalOrbitPercent;
	float				m_fDirectionLength;
	CPoint				m_ptAnchor;
	IWorld*				m_pWorld;
	IVector*			m_pVector;
	LPDIRECT3DRMFRAME	m_pRMCameraFrame;
	long				m_nAxisLock;
	VARIANT_BOOL		m_bGravity;
	HWND				m_hWnd;
	IDispatch*			m_piEditorViewControl;
	int					m_nCameraMode;
	UINT				m_KeyBindings[kbMax];
	VARIANT_BOOL		m_bGotFocus;
	LPDIRECT3DRMFRAME	m_pRMDummyFrame;
	VARIANT_BOOL		m_bShowingNoCursor;

};

#endif //__VWTRANSLATE3DTOOL_H_
