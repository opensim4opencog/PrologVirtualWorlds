// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWNavT2D.h : Declaration of the CVWNavigationTool2D

#include <resource.h>

#include <propbase.h>
#include <uitlimpl.h>

EXTERN_C const IID LIBID_VWMMLib;
EXTERN_C const CLSID CLSID_VWNavigationTool2D;

/////////////////////////////////////////////////////////////////////////////
// VWNvTool

DECLARE_VWUITOOLEX(CVWNavigationTool2D, IVWNavigationTool, IID_IVWNavigationTool, CLSID_VWNavigationTool2D, LIBID_VWMMLib)
{
public:
BEGIN_COM_MAP(CVWNavigationTool2D)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWUITool)
	COM_INTERFACE_ENTRY(IVWNavigationTool)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

//DECLARE_NOT_AGGREGATABLE(CVWNavigationTool2D) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

	DECLARE_REGISTRY(CVWNavigationTool2D, _T("VWSYSTEM.NavigationTool2D.1"), _T("VWSYSTEM.NavigationTool2D"), IDS_VWNAVIGATIONTOOL2D_DESC, THREADFLAGS_BOTH)

// ISupportsErrorInfo

// IVWUITool

// IVWNavigationTool
	STDMETHOD(Init)(IVWRenderRoot *pVWRenderRoot);
	STDMETHOD(Update)() {return E_NOTIMPL;};
	STDMETHOD(put_NavigationTarget)(IThing *pThing);
	STDMETHOD(SetNavigationPath)(IPropertyMap *pmapPositions, IPropertyMap *pmapDirections, BSTR bstrDoneEvent);
	STDMETHOD(get_Moving)(VARIANT_BOOL *bMoving);
	STDMETHOD(put_OrthogonalMode)(VARIANT_BOOL bOrthogonal);
	STDMETHOD(put_TopOrthogonalMode)(VARIANT_BOOL bOrthogonal);
	STDMETHOD(put_FrontOrthogonalMode)(VARIANT_BOOL bOrthogonal);
	STDMETHOD(put_RightOrthogonalMode)(VARIANT_BOOL bOrthogonal);
	STDMETHOD(put_KeyboardNavEnabled)(VARIANT_BOOL bEnable){return E_NOTIMPL;};
	STDMETHOD(get_KeyBinding)(KeyConstant index, VARIANT* pvar){return E_NOTIMPL;};
	STDMETHOD(put_KeyBinding)(KeyConstant index, VARIANT var){return E_NOTIMPL;};
	STDMETHOD(get_CollisionDetection)(VARIANT_BOOL *pVal); 
	STDMETHOD(put_CollisionDetection)(VARIANT_BOOL newVal);
	STDMETHOD(get_Gravity)(VARIANT_BOOL *pVal); 
	STDMETHOD(put_Gravity)(VARIANT_BOOL newVal);
};
