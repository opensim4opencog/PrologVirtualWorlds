// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWPkTl.h : Declaration of the CVWPkTool

#include <afxtempl.h>
#include <resource.h>
#include <vwmenu.h>

#include <propbase.h>
#include <uitlimpl.h>

EXTERN_C const IID LIBID_VWMMLib;
EXTERN_C const CLSID CLSID_VWPickTool;

/////////////////////////////////////////////////////////////////////////////
// vwpktool

DECLARE_VWUITOOLEX(CVWPickTool, IVWPickTool, IID_IVWPickTool, CLSID_VWPickTool, LIBID_VWMMLib)
{
public:
	CVWPickTool(); 
	~CVWPickTool();

BEGIN_COM_MAP(CVWPickTool)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWUITool)
	COM_INTERFACE_ENTRY(IVWPickTool)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

//DECLARE_NOT_AGGREGATABLE(CVWPkTool) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

DECLARE_REGISTRY(CVWPickTool, _T("VWSYSTEM.PickTool.1"), _T("VWSYSTEM.PickTool"), IDS_VWPKTOOL_DESC, THREADFLAGS_BOTH)

public:
	STDMETHOD(Terminate)(void);

// IVWUITool
	virtual BOOL OnLButtonDown( UINT nFlags, POINT point );
	virtual BOOL OnLButtonUp( UINT nFlags, POINT point );
	virtual BOOL OnLButtonDblClk( UINT nFlags, POINT point );
	virtual BOOL OnRButtonDown( UINT nFlags, POINT point );
	virtual BOOL OnRButtonUp( UINT nFlags, POINT point );
	virtual BOOL OnRButtonDblClk( UINT nFlags, POINT point );
	virtual BOOL OnMouseMove( UINT nFlags, POINT point );
	virtual BOOL OnTimer( UINT nIDEvent , TIMERPROC * tmprc);
	virtual BOOL OnMessage( UINT nIDEvent, WPARAM wParam, LPARAM lParam );

	STDMETHOD(IsValid)(IVWUIView *pVw, VARIANT_BOOL* pbool);
	STDMETHOD(Initialize)(IVWUIView* pVw, IWorld* pWorld, IVWRenderRoot *pVWRenderRoot);

// IVWPickTool

	DISPPARAMS			m_dispparams;
	IVWRenderRoot*		m_pRender;
	BOOL				m_bHandlingPopup;		// TRUE if in the middle of a popup
	CVWMenu				m_Menu;
	CPoint				m_pntLButtonDown;

	// Helper functions
	HRESULT	HitTest(POINT point, VARIANT_BOOL vbReturnAllPicked, IThing **ppThing);
	HRESULT DisplayPopup(HWND hWnd, POINT point, IPropertyList *plistMenu);
	HRESULT HandlePopupCommand(WPARAM wParam);
	HRESULT HandlePopupUpdateCommand(CCmdUI *pCmdUI);
};
