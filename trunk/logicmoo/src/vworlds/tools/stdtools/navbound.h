// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWBoundaryNavigateTool.h : Declaration of the CVWBoundaryNavigateTool


#include <afxtempl.h>
#include <uitlimpl.h>
#include "resource.h"       // main symbols
#include <vwsutils.h>
#include <bndytool.h>

EXTERN_C const IID LIBID_VWSTUDIOLib;
EXTERN_C const CLSID CLSID_VWBoundaryNavigateTool;

/////////////////////////////////////////////////////////////////////////////
// Stdtools

DECLARE_VWUITOOLEX(CVWBoundaryNavigateTool, IVWBoundaryNavigateTool, IID_IVWBoundaryNavigateTool, CLSID_VWBoundaryNavigateTool, LIBID_VWSTUDIOLib)
{
public:
BEGIN_COM_MAP(CVWBoundaryNavigateTool)
	COM_INTERFACE_ENTRY2(IDispatch, IVWBoundaryNavigateTool)
	COM_INTERFACE_ENTRY(IVWUITool)
	COM_INTERFACE_ENTRY(IVWBoundaryNavigateTool)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

DECLARE_REGISTRY_RESOURCEID(IDR_VWBOUNDARYNAVIGATETOOL)

	CVWBoundaryNavigateTool();

	STDMETHOD(IsValid)(IVWUIView*,VARIANT_BOOL *);												\
	STDMETHOD(put_UseAvatarPosition)(VARIANT_BOOL newVal);
	STDMETHOD(get_UseAvatarPosition)(VARIANT_BOOL *pVal);

	virtual BOOL OnLButtonDown( UINT nFlags, POINT point );
	virtual BOOL OnLButtonUp(UINT nFlags, POINT point);
	virtual BOOL OnMouseMove( UINT nFlags, POINT pt  );
	//virtual BOOL OnTimer( UINT nIDEvent );
	virtual BOOL OnTimer( UINT nIDEvent , TIMERPROC * tmprc);

	POINT		m_cpLastClick;

	IDispatch*	m_piBoundaryViewControl;  //Boundary control pointer if any
	int			m_nEventID;
	IVWFrame*	m_pUserVWFrame;
	IVWFrame*	m_pParentVWFrame;
	BOOL		m_bUseAvatarPosition;
	BOOL		m_bCapturedCursor;

// IVWBoundaryNavigateTool

public:
};
