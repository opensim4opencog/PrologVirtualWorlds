// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWZoomTool.h : Declaration of the CVWZoomTool

#ifndef __VWZOOMTOOL_H_
#define __VWZOOMTOOL_H_

#include <afxtempl.h>
#include <uitlimpl.h>
#include "resource.h"       // main symbols
#include <vwsutils.h>
//#include <ECVWType.h>

EXTERN_C const IID LIBID_VWSTUDIOLib;
EXTERN_C const CLSID CLSID_VWZoomTool;

/////////////////////////////////////////////////////////////////////////////
// CVWZoomTool


DECLARE_VWUITOOLEX(CVWZoomTool, IVWZoomTool, IID_IVWZoomTool, CLSID_VWZoomTool, LIBID_VWSTUDIOLib)
{
public:
BEGIN_COM_MAP(CVWZoomTool)
	COM_INTERFACE_ENTRY2(IDispatch, IVWZoomTool)
	COM_INTERFACE_ENTRY(IVWUITool)
	COM_INTERFACE_ENTRY(IVWZoomTool)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

public:
	CVWZoomTool();
	~CVWZoomTool() { SAFERELEASE(m_pVWRenderRoot); }
	STDMETHOD(IsValid)(IVWUIView*,VARIANT_BOOL *) { return E_NOTIMPL; }
	virtual BOOL OnLButtonDown( UINT nFlags, POINT point );

	STDMETHOD(put_Mode)(short newVal);
	STDMETHOD(get_Mode)(short *pVal);
	STDMETHOD(Initialize)(IVWUIView* pVw, IWorld* pWorld, IVWRenderRoot *pVWRenderRoot);

	short			m_nMode;
	IVWRenderRoot	*m_pVWRenderRoot;

DECLARE_REGISTRY_RESOURCEID(IDR_VWZOOMTOOL)


// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IVWZoomTool
public:
};

#endif //__VWZOOMTOOL_H_
