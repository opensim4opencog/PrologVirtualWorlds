// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// Rotat2D.h : Declaration of the CRotate2DTool

#ifndef __ROTATE2DTOOL_H_
#define __ROTATE2DTOOL_H_

#include <afxtempl.h>
#include "resource.h"       // main symbols
#include <uitlimpl.h>
#include <propbase.h>
#include <vwtrans.h>
#include <cellprop.h>
#include <PickData.h>
#include <bndytool.h>
#include <vwnvtool.h>
#include <vwsgfxut.h>

EXTERN_C const IID LIBID_VWSTUDIOLib;
EXTERN_C const CLSID CLSID_VWRotate2DTool;

class CRotate2DTool;

class CRotateObject : public CObject
{
   friend  CRotate2DTool;

	CRotateObject()
	{
		m_pPickData = NULL;
		m_pBoundary = NULL;
		m_pPoints = NULL;
		m_pVWFrame = NULL;
	};

	~CRotateObject() 
	{ 
		if (m_pPoints)
			delete [] m_pPoints;

		SAFERELEASE(m_pBoundary); 
		SAFERELEASE(m_pVWFrame); 
		SAFERELEASE(m_pPickData);
	};

public:
	CRotateObject( IVWPickData* pPickData);

	IVWPickData* m_pPickData;
	IBoundary* m_pBoundary;
	IVWFrame* m_pVWFrame;
	fPoint2D* m_pPoints;
	int m_nPoints;

	fPoint2D m_fCenter;
};
typedef CTypedPtrList<CObList, CRotateObject*> CRotateList;

/////////////////////////////////////////////////////////////////////////////
// CRotate2DTool
DECLARE_VWUITOOLEX(CRotate2DTool, IVWRotate2DTool, IID_IVWRotate2DTool, CLSID_VWRotate2DTool, LIBID_VWSTUDIOLib)
{
public:
	CRotate2DTool();
	~CRotate2DTool();

	IVWExecuteUndo *m_pExecuteUndo;

DECLARE_REGISTRY_RESOURCEID(IDR_ROTATE2DTOOL)

BEGIN_COM_MAP(CRotate2DTool)
	COM_INTERFACE_ENTRY(IVWRotate2DTool)
	COM_INTERFACE_ENTRY(IVWUITool)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

	virtual BOOL OnLButtonDown( UINT nFlags, POINT point );
	virtual BOOL OnLButtonUp( UINT nFlags, POINT point );
	virtual BOOL OnMouseMove( UINT nFlags, POINT point );
	virtual BOOL OnKeyUp( UINT nVirtKey, LPARAM lKeyData );
	virtual BOOL OnMessage( UINT nIDEvent, WPARAM wParam, LPARAM lParam );
	virtual BOOL OnKeyDown( UINT nVirtKey, LPARAM lKeyData );

	STDMETHOD(IsValid)(IVWUIView* pVW,VARIANT_BOOL * pB);
	STDMETHOD(Initialize)(IVWUIView* pVw, IWorld* pWorld, IVWRenderRoot *pVWRenderRoot);
	STDMETHOD(Update)();
	STDMETHOD(OnUIEvent)(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient);

	// IVWRotate2DTool
protected:

	HRESULT RotateSelectedObjects( float flDeltaX );
	HRESULT SetupEnvironment();
	HRESULT SetupTransformList();
	HRESULT DestroyTransformList();
	HRESULT InvokeToolEvent(int nNum, IObjectProperty* pOP, BSTR bstr1, BSTR bstr2, BSTR bstr3, VARIANT_BOOL bBubble);
	HRESULT OnDoneRotating();
	float	ElapsedFrameTime();

	// CTypedPtrList of Objects for drawing Boundary.
	CRotateList			m_TransformList;
	CPoint				m_cptLast, m_cptFirst;
	IDispatch*			m_piEditorViewControl;
	float				m_fDeltaTotal;
	UINT				m_KeyBindings[kbMax];
	BOOL				m_bUsingKeyboard;
	int					m_nAxisLock;
	VARIANT_BOOL		m_bGotFocus;
	VARIANT_BOOL		m_bTrackingMouse;
	VARIANT_BOOL		m_bShowingNoCursor;
};

#endif //__ROTATE2DTOOL_H_
