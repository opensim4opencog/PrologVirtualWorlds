// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// Rotat2D.h : Declaration of the CVWScale2DTool

#ifndef __VWScale2DTool_H_
#define __VWScale2DTool_H_

#include <afxtempl.h>
#include "resource.h"       // main symbols
#include <uitlimpl.h>
#include <propbase.h>
#include <vwtrans.h>
#include <cellprop.h>
#include <PickData.h>
#include <bndytool.h>
#include <vwnvtool.h>
#include <vwrnedit.h>
#include "vwsutils.h"
#include <vwsgfxut.h>

EXTERN_C const IID LIBID_VWSTUDIOLib;
EXTERN_C const CLSID CLSID_VWScale2DTool;

class CVWScale2DTool;


class CScaleObject : public CObject
{
   friend  CVWScale2DTool;

	CScaleObject()
	{
		m_pBoundary = NULL;
		m_pPickData = NULL;
	};

	~CScaleObject(); 

public:
	CScaleObject( IVWPickData* pPickData );

	IBoundary* m_pBoundary;
	IVWPickData* m_pPickData;
	IVWFrame *m_pVWFrame;
	float fCenterX, fCenterY;
	float fnewX, fnewY;
	fPoint2D* points;
	long nPoints;
};
typedef CTypedPtrList<CObList, CScaleObject*> CScaleList;

/////////////////////////////////////////////////////////////////////////////
// CVWScale2DTool
DECLARE_VWUITOOLEX(CVWScale2DTool, IVWScale2DTool, IID_IVWScale2DTool, CLSID_VWScale2DTool, LIBID_VWSTUDIOLib)
{
public:
	CVWScale2DTool();
	~CVWScale2DTool();

DECLARE_REGISTRY_RESOURCEID(IDR_Scale2DTOOL)

BEGIN_COM_MAP(CVWScale2DTool)
	COM_INTERFACE_ENTRY(IVWScale2DTool)
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

	STDMETHOD(IsValid)(IVWUIView* pVW,VARIANT_BOOL * pB);
	STDMETHOD(Initialize)(IVWUIView* pVw, IWorld* pWorld, IVWRenderRoot *pVWRenderRoot);
	STDMETHOD(Update)();
	STDMETHOD(OnUIEvent)(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient);

	HRESULT SetupEnvironment();
	HRESULT SetupTransformList();
	HRESULT OnDoneScaling();
	HRESULT InvokeToolEvent(int nNum, IObjectProperty* pOP, BSTR bstr1, BSTR bstr2, BSTR bstr3, VARIANT_BOOL bBubble);
	HRESULT DestroyTransformList();
	HRESULT ScaleSelectedObjects( float flDeltaX, float flDeltaY );
	HRESULT OnClickOrKeyDown();

	float	ElapsedFrameTime();

	// IVWScale2DTool
protected:

	// CTypedPtrList of Objects for drawing Boundary.
	CScaleList				m_TransformList;
	CPoint					m_cptLast, m_cptFirst, m_cptCenter, m_pntCenterDiff;
	float					m_flTotalDeltaX, m_flTotalDeltaY;
	IDispatch*				m_piEditorViewControl;
	int						m_nAxisLock;
	IWorld*					m_pWorld;
	UINT					m_KeyBindings[kbMax];
	BOOL					m_bUsingKeyboard;
	IVWRenderRoot*			m_pVWRenderRoot;
	LPDIRECT3DRMVIEWPORT	m_pViewport;
	D3DVECTOR				m_vecViewportZero;
	VARIANT_BOOL			m_bGotFocus;
	VARIANT_BOOL			m_bTrackingMouse;
	VARIANT_BOOL			m_bShowingNoCursor;

};

#endif //__VWScale2DTool_H_
