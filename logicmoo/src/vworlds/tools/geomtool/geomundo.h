// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// GeomUndo.h : Declaration of the CVWExecuteGeomUndo

#ifndef __VWEXECUTEGEOMUNDO_H_
#define __VWEXECUTEGEOMUNDO_H_

#include <propbase.h>
#include "resource.h"       // main symbols
#include "UndoUtil.h"

EXTERN_C const IID LIBID_VWSTUDIOLib;
EXTERN_C const CLSID CLSID_VWExecuteGeomUndo;

/////////////////////////////////////////////////////////////////////////////
// CVWExecuteGeomUndo
class ATL_NO_VTABLE CVWExecuteGeomUndo : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVWExecuteGeomUndo, &CLSID_VWExecuteGeomUndo>,
	public IDispatchImpl<IVWExecuteUndo, &IID_IVWExecuteUndo, &LIBID_VWSTUDIOLib>
{
public:
	CVWExecuteGeomUndo()
	{
	}

	// IVWExecuteUndo event handlers
	STDMETHOD(Redo)(/*[in]*/ IVWUndoItem * pundo);
	STDMETHOD(Undo)(/*[in]*/ IVWUndoItem * predo);

DECLARE_REGISTRY_RESOURCEID(IDR_VWEXECUTEGEOMUNDO)

BEGIN_COM_MAP(CVWExecuteGeomUndo)
	COM_INTERFACE_ENTRY(IVWExecuteUndo)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IVWExecuteUndo
public:
};

#endif //__VWEXECUTEGEOMUNDO_H_
