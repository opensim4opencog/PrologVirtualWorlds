// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef __VWSTUEXO_H_
#define __VWSTUEXO_H_

#include "resource.h"       // main symbols
#include <propbase.h>
#include "objimpl.h"

EXTERN_C const IID LIBID_VWSTUDIOLib;
EXTERN_C const CLSID CLSID_StudioExemplar;

DECLARE_VWDISPATCH(StudioExemplar, LIBID_VWSTUDIOLib)
{
public:
//	CStudioExemplarObject();

DECLARE_REGISTRY_RESOURCEID(IDR_VWSTUEX)

BEGIN_COM_MAP(CStudioExemplarObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWDispatch)
	COM_INTERFACE_ENTRY(IStudioExemplar)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

public:
	STDMETHOD(Install)(IModule* pModule);
	STDMETHOD(OnSelectionLockChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnCameraFollowsObjectChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnGravityChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnCollisionDetectionChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(OnEditingModeChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(GetTypeFromProperty)(IThing *pThing, BSTR bstrPropertyName, BSTR *bstrType);
	STDMETHOD(GetIIDFromType)(BSTR bstrType, BSTR *bstrIID);

};

#endif //__VWSTUEXO_H_
