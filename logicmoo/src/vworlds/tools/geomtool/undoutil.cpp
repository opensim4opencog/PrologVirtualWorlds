// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include <d3drm.h>
#include <geomtool.h>
#include "UndoUtil.h"
//#include "GeomUndo.h"
#include <propbase.h>

// Undo Data Stuff
UndoData *CreateUndoData(OperationType ot)
{
	UndoData *pReturn = NULL;
	pReturn = (UndoData *)malloc(sizeof(UndoData));
	ZeroMemory(pReturn, sizeof(UndoData));
	pReturn->m_ot = ot;

	return pReturn;
}

IVWUndoItem *CreateUndoItem(IVWExecuteUndo *pExecuteUndo)
{
	HRESULT hr = S_OK;
	CLSID	clsidVWUndoItem;
	IVWUndoItem *pVWUndoItem = NULL;

	// Create the navigation target.
 	if (FAILED(hr = CLSIDFromProgID(CComBSTR("VWStudio.VWUndoItem.1"), &clsidVWUndoItem)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = CoCreateInstance(clsidVWUndoItem, 
 								NULL,
 								CLSCTX_INPROC_SERVER,
 								IID_IVWUndoItem,
 								(void **)&pVWUndoItem)))
		goto ERROR_ENCOUNTERED;
	
	hr = pVWUndoItem->put_ExecuteUndo(pExecuteUndo);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	return pVWUndoItem;

ERROR_ENCOUNTERED:
//	HrError(hr);
	SAFERELEASE(pVWUndoItem);
	return NULL;
}
