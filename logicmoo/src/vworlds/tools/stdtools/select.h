// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWSelectTool.h : Declaration of the CVWSelectToolObject


#include <afxtempl.h>
#include <uitlimpl.h>
#include "resource.h"       // main symbols
#include <vwsutils.h>
//#include <ECVWType.h>
//#include <ecvwctrl.h>

EXTERN_C const IID LIBID_VWSTUDIOLib;
EXTERN_C const CLSID CLSID_VWSelectTool;

/////////////////////////////////////////////////////////////////////////////
// Stdtools

DECLARE_VWUITOOLEX(CVWSelectToolObject, IVWSelectTool, IID_IVWSelectTool, CLSID_VWSelectTool, LIBID_VWSTUDIOLib)
{
public:
BEGIN_COM_MAP(CVWSelectToolObject)
	COM_INTERFACE_ENTRY2(IDispatch, IVWSelectTool)
	COM_INTERFACE_ENTRY(IVWUITool)
	COM_INTERFACE_ENTRY(IVWSelectTool)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

DECLARE_REGISTRY_RESOURCEID(IDR_VWSelectTool)

	CVWSelectToolObject();
	~CVWSelectToolObject();

	STDMETHOD(Initialize)(IVWUIView* pVw, IWorld* pWorld, IVWRenderRoot *pVWRenderRoot);
	STDMETHOD(IsValid)(IVWUIView*,VARIANT_BOOL *);												\
	STDMETHOD(put_DeluxeSelection)(VARIANT_BOOL newVal);
	STDMETHOD(get_DeluxeSelection)(VARIANT_BOOL* pVal);
	STDMETHOD(put_VertexSelection)(VARIANT_BOOL newVal);
	STDMETHOD(get_VertexSelection)(VARIANT_BOOL* pVal);

	virtual BOOL OnLButtonDown( UINT nFlags, POINT point );
	virtual BOOL OnLButtonUp(UINT nFlags, POINT point);
	virtual BOOL OnMouseMove( UINT nFlags, POINT pt  );
	
	IObjectProperty* FindSelected( IObjectProperty* pOp, BOOL bCheckContainer, BOOL& bFoundInContainer);
	HRESULT InvokeToolEvent(int nNum, IObjectProperty* pOP, BSTR bstr1, BSTR bstr2, BSTR bstr3, VARIANT_BOOL bBubble);

	POINT	m_pPoint;
	BOOL	m_bDeluxeSelection;
	BOOL	m_bVertexSelection;
	IDispatch*		m_piEditorViewControl;

	IObjectProperty* m_pSelectedObjectProperty;

	//IDispatch* m_piBoundaryViewControl;  //Boundary control pointer if any

// IVWSelectTool

public:
};
