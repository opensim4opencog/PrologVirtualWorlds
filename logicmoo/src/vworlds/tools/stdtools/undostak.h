// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWUndoStack.h : Declaration of the CVWUndoStacksObject

#include <propbase.h>
#include <objimpl.h>
#include "resource.h"       // main symbols
				 
EXTERN_C const IID LIBID_VWSTUDIOLib;
EXTERN_C const CLSID CLSID_VWUndoStacks;

/////////////////////////////////////////////////////////////////////////////
// Stdtools

DECLARE_OBJECTPROPERTY(VWUndoStacks, LIBID_VWSTUDIOLib, IDS_VWUNDOSTACKS_DESC)
{
DECLARE_AGGREGATABLE(CVWUndoStacksObject)

public:
BEGIN_COM_MAP(CVWUndoStacksObject)
	COM_INTERFACE_ENTRY2(IDispatch, IVWUndoStacks)
	COM_INTERFACE_ENTRY(IObjectProperty)
	COM_INTERFACE_ENTRY(IVWUndoStacks)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

DECLARE_REGISTRY_RESOURCEID(IDR_VWUndoStacks)

// IVWUndoStacks
public:
	CVWUndoStacksObject();
	~CVWUndoStacksObject();

// Override the base class
	STDMETHOD(put_World)(IWorld* pworld);
	STDMETHOD(Dump)(void);
	STDMETHOD(Terminate)(void);
	STDMETHOD(get_RefCount)(DWORD* pdwRef);

	STDMETHOD(Flush)( void );
	STDMETHOD(Undo)(void);
	STDMETHOD(Redo)(void);
	STDMETHOD(AddVWUndoItem)(IVWUndoItem * pVWUndoItem);
	STDMETHOD(put_SizeUndoStack)(unsigned int); 
	STDMETHOD(get_SizeUndoStack)(unsigned int *);
	STDMETHOD(get_UndoLabel)(BSTR * );
	STDMETHOD(get_RedoLabel)(BSTR * );

protected:
	IPropertyList* m_pproplistStacks;
	unsigned int m_iSizeUndo; // Size of the Undo Stack
	unsigned int m_cUndoMax; // Maximum number of UndoItems.
	unsigned int m_cStacksSize;	// Current size of the combined undo and redo stacks.
	BOOL m_fReentrancyCheck;	// No UndoItems should be added during Undo and Redo.
	BOOL StacksAreValid( void );
	BOOL ResetStacks( void );
};
