// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWDeluxeSelectTool.h : Declaration of the CVWSelectToolObject


#include <afxtempl.h>
#include <uitlimpl.h>
#include "resource.h"       // main symbols
#include <vwsutils.h>
//#include <ECVWType.h>
//#include <ecvwctrl.h>

EXTERN_C const IID LIBID_VWSTUDIOLib;
EXTERN_C const CLSID CLSID_VWDeluxeSelectTool;

/////////////////////////////////////////////////////////////////////////////
// Stdtools

DECLARE_VWUITOOLEX(CVWDeluxeSelectToolObject, IVWDeluxeSelectTool, IID_IVWDeluxeSelectTool, CLSID_VWDeluxeSelectTool, LIBID_VWSTUDIOLib)
{
public:
BEGIN_COM_MAP(CVWDeluxeSelectToolObject)
	COM_INTERFACE_ENTRY2(IDispatch, IVWDeluxeSelectTool)
	COM_INTERFACE_ENTRY(IVWUITool)
	COM_INTERFACE_ENTRY(IVWDeluxeSelectTool)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

DECLARE_REGISTRY_RESOURCEID(IDR_VWDeluxeSelectTool)

	CVWDeluxeSelectToolObject();

	STDMETHOD(IsValid)(IVWUIView*,VARIANT_BOOL *);												\

	virtual BOOL OnLButtonDown( UINT nFlags, POINT point );
	virtual BOOL OnLButtonUp(UINT nFlags, POINT point);
	virtual BOOL OnMouseMove( UINT nFlags, POINT pt  );

	POINT	m_pPoint;
	IObjectProperty* m_pUnRefedSelectedObjectProperty;

// IVWSelectTool

public:
};
