// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWUndoItem.h : Declaration of the CVWUndoItem

#ifndef __VWUNDOITEM_H_
#define __VWUNDOITEM_H_

#include <propbase.h>
#include "resource.h"       // main symbols
#include <cellprop.h>

EXTERN_C const IID LIBID_VWSTUDIOLib;
EXTERN_C const CLSID CLSID_VWUndoItem;

/////////////////////////////////////////////////////////////////////////////
// CVWUndoItem
class ATL_NO_VTABLE CVWUndoItem : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVWUndoItem, &CLSID_VWUndoItem>,
	public IDispatchImpl<IVWUndoItem, &IID_IVWUndoItem, &LIBID_VWSTUDIOLib>
{
public:

	CVWUndoItem();
	~CVWUndoItem();

DECLARE_REGISTRY_RESOURCEID(IDR_VWUNDOITEM)

BEGIN_COM_MAP(CVWUndoItem)
	COM_INTERFACE_ENTRY(IVWUndoItem)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IVWUndoItem
public:
	STDMETHOD(get_ExecuteUndo)(/*[out, retval]*/ IVWExecuteUndo ** ppexun);
	STDMETHOD(put_ExecuteUndo)(/*[in]*/ IVWExecuteUndo * pexun);
	STDMETHOD(get_UndoTarget)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_UndoTarget)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_RedoData)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_RedoData)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_UndoData)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_UndoData)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_UndoHints)(/*[out, retval]*/ int *newVal);
	STDMETHOD(put_UndoHints)(/*[in]*/ int newVal);
	STDMETHOD(get_Label)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Label)(/*[in]*/ BSTR newVal);
	STDMETHOD(Redo)();
	STDMETHOD(Undo)();

protected:
	VARIANT m_varUndoTarget;
	VARIANT m_varUndoData;
	VARIANT m_varRedoData;
	CComBSTR	m_bstrLabel;
	IVWExecuteUndo * m_pexun;
	int		m_iUndoHints;
};

#endif //__VWUNDOITEM_H_
