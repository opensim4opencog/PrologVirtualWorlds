// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// Trans2D.h : Declaration of the CVWTranslate2DTool

#include <afxtempl.h>
#include <propbase.h>
#include <vwsutils.h>
#include <uitlimpl.h>
#include "resource.h"       // main symbols
#include <vwtrans.h>
#include <PickData.h>
#include <bndytool.h>
#include <vwnvtool.h>

EXTERN_C const IID LIBID_VWSTUDIOLib;
EXTERN_C const CLSID CLSID_VWTranslate2DTool;

class CVWTranslate2DToolObject;

/////////////////////////////////////////////////////////////////////////////
class CTranslate2DObject : public CObject
{
   friend  CVWTranslate2DToolObject;

    CTranslate2DObject() { m_pPickData = NULL;  m_pBoundary = NULL; };
	~CTranslate2DObject();
public:
	CTranslate2DObject( IVWPickData* pPickData);

	IVWPickData* m_pPickData;
	IBoundary* m_pBoundary;
	BOOL bVerticeSelected;
	IVWFrame* m_pVWFrame;

	float fOldX, fOldY;
};
typedef CTypedPtrList<CObList, CTranslate2DObject*> CTransform2DList;

/////////////////////////////////////////////////////////////////////////////
// Stdtools

DECLARE_VWUITOOLEX(CVWTranslate2DToolObject, IVWTranslate2DTool, IID_IVWTranslate2DTool, CLSID_VWTranslate2DTool, LIBID_VWSTUDIOLib)
{
public:
	CVWTranslate2DToolObject() ;
	~CVWTranslate2DToolObject() ;

BEGIN_COM_MAP(CVWTranslate2DToolObject)
	COM_INTERFACE_ENTRY2(IDispatch, IVWTranslate2DTool)
	COM_INTERFACE_ENTRY(IVWUITool)
	COM_INTERFACE_ENTRY(IVWTranslate2DTool)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

//DECLARE_NOT_AGGREGATABLE(CVWTranslate2DTool) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

DECLARE_REGISTRY_RESOURCEID(IDR_VWTranslate2DTool)

protected:
		// CTypedPtrList of Objects for drawing Boundary.
	CTransform2DList	m_TransformList;
	int					m_nLastX, m_nLastY;
	float				m_flFirstX, m_flFirstY , m_flFirstZ;
	float				m_flLastX, m_flLastY , m_flLastZ;
	float				m_flTotalDeltaX, m_flTotalDeltaZ;
	IDispatch*			m_piEditorViewControl;
	int					m_nAxisLock;
	IWorld*				m_pWorld;
	UINT				m_KeyBindings[kbMax];
	BOOL				m_bUsingKeyboard;
	VARIANT_BOOL		m_bGotFocus;
	VARIANT_BOOL		m_bTrackingMouse;
	VARIANT_BOOL		m_bShowingNoCursor;

	virtual BOOL OnLButtonDown( UINT nFlags, POINT point );
	virtual BOOL OnLButtonUp( UINT nFlags, POINT point );
	virtual BOOL OnMouseMove( UINT nFlags, POINT point );
	virtual BOOL OnKeyDown( UINT nVirtKey, LPARAM lKeyData );
	virtual BOOL OnKeyUp( UINT nVirtKey, LPARAM lKeyData );
	virtual BOOL OnMessage( UINT nIDEvent, WPARAM wParam, LPARAM lParam );

	// IVWTranslate2DTool
	STDMETHOD(Initialize)(IVWUIView* pVw, IWorld* pWorld, IVWRenderRoot *pVWRenderRoot);
	STDMETHOD(OnUIEvent)(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient);
	STDMETHOD(IsValid)(IVWUIView* pVW,VARIANT_BOOL * pB);
	STDMETHOD(Update)();

	HRESULT SetupEnvironment();
	HRESULT SetupTransformList();
	HRESULT OnDoneTranslating();
	HRESULT InvokeToolEvent(int nNum, IObjectProperty* pOP, BSTR bstr1, BSTR bstr2, BSTR bstr3, VARIANT_BOOL bBubble);
	HRESULT InvokeCenterViewOnObject(IThing* pThing);
	HRESULT DestroyTransformList();
	HRESULT TranslateSelectedObjects( float flDeltaX, float flDeltaZ );

	float	ElapsedFrameTime();

};
