// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// CtrlMgr.h : Declaration of the CControlMgrObject

#ifndef __CONTROLMGR_H_
#define __CONTROLMGR_H_

#include "resource.h"       // main symbols
#include <propbase.h>
#include <vwuiobjs.h>
#include <atlctl.h>

EXTERN_C const IID LIBID_VWSTUDIOLib;
EXTERN_C const CLSID CLSID_VWControlManager;

/////////////////////////////////////////////////////////////////////////////
// CControlMgrObject
class ATL_NO_VTABLE CControlMgrObject : 
	public IDispatchImpl<IVWControlManager, &IID_IVWControlManager, &LIBID_VWSTUDIOLib>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CControlMgrObject, &CLSID_VWControlManager>,
	public IObjectSafetyImpl<CControlMgrObject, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
	public ISupportErrorInfoImpl<&IID_IVWControlManager>,
	public IConnectionPointContainerImpl<CControlMgrObject>,
	public IConnectionPointImpl<CControlMgrObject, &IID_IVWUIEvents, CComDynamicUnkArray>
{
public:
	CControlMgrObject();
	HRESULT FinalConstruct();
	void FinalRelease();

DECLARE_REGISTRY_RESOURCEID(IDR_CONTROLMGR)
DECLARE_GET_CONTROLLING_UNKNOWN()

BEGIN_COM_MAP(CControlMgrObject)
	COM_INTERFACE_ENTRY2(IDispatch,IVWControlManager)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
	COM_INTERFACE_ENTRY(IVWControlManager)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IPropertyList, m_pdispSelection.p)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IVWUndoStacks, m_pdispUndoStacks.p)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IVWErrorTool, m_pdispErrorTool.p)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

// Connection Point
	BEGIN_CONNECTION_POINT_MAP(CControlMgrObject)
		CONNECTION_POINT_ENTRY(IID_IVWUIEvents)
	END_CONNECTION_POINT_MAP()
	

// IVWControlManager

	STDMETHOD(get_World)(IWorld ** );
	STDMETHOD(put_World)(IWorld * );
	STDMETHOD(get_SelectionList)(IPropertyList ** );
	STDMETHOD(get_UndoStack)( IVWUndoStacks ** ); 
	STDMETHOD(AddVWUndoItem)( IVWUndoItem *);
	STDMETHOD(Undo)(void);
	STDMETHOD(Redo)(void);
	STDMETHOD(DisplayError)(HRESULT hr, VARIANT_BOOL *pbHandled);
	STDMETHOD(AddSelectionExt)( IVWUIView *, VARIANT );
	STDMETHOD(AddSelection)( VARIANT );
	STDMETHOD(Select)( VARIANT );
	STDMETHOD(RemoveSelectionExt)( IVWUIView *, VARIANT ); 
	STDMETHOD(RemoveSelection)( VARIANT ); 
	STDMETHOD(ToggleSelectionExt)( IVWUIView *, VARIANT );
	STDMETHOD(ToggleSelection)( VARIANT );
	STDMETHOD(ClearSelectionsExt)(IVWUIView * );
	STDMETHOD(ClearSelections)(void);
	STDMETHOD(RefreshObjects)( IVWUIView *, VARIANT );
	STDMETHOD(RefreshObjectsInView)( IVWUIView *, VARIANT );
	STDMETHOD(RefreshView)(  IVWUIView * pVw, VARIANT varHint, VARIANT_BOOL varRefreshAllViews);
	STDMETHOD(UndoStackChanged)(  IVWUIView * pVw, VARIANT varHint);
	STDMETHOD(GetSelectedCount)( IID* pIID, long* nCount );
	STDMETHOD(IsSelected)( IObjectProperty* pObjProp, VARIANT_BOOL* vbSelected );
	STDMETHOD(IsVertexSelected)( IObjectProperty* pObjProp, int nIndex, VARIANT_BOOL* vbSelected );
	STDMETHOD(IsAnyVertexSelected)( IObjectProperty* pObjProp, VARIANT_BOOL* vbSelected );
	STDMETHOD(get_Clipboard)( IPropertyList** pPropList );
	STDMETHOD(Copy)( IObjectProperty* pObjProp );
	STDMETHOD(Paste)( IObjectProperty* pObjProp );
	STDMETHOD(IsBoundarySelected)( IThing* pThing, int nBoundaryIndex, VARIANT_BOOL* vbSelected );

	// method to broadcast a call on the current connections
	HRESULT FireUIUpdate(enumVWUIEventTypes, IVWUIView * pvwSource, VARIANT varHint, BOOL bOnlyRefreshSource );

private:
	CComPtr<IPropertyList>	m_pdispSelection;	// Aggregated PropertyList for SelectionList
	CComPtr<IVWUndoStacks>	m_pdispUndoStacks;	// Aggregated UndoStacks
	CComPtr<IVWErrorTool>	m_pdispErrorTool;		// Aggregated VWErrorTool
	IPropertyList*			m_pClipboard;
	IWorld*					m_pWorld;
};

#endif //__CONTROLMGR_H_
