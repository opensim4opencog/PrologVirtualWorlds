// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// D2DRMObj.h : Declaration of the CDirect2DRMObject


#include "resource.h"       // main symbols
#include "d2dbase.h"

/////////////////////////////////////////////////////////////////////////////
// d2d

class ATL_NO_VTABLE CDirect2DRMObject : D2DOBJECT_BASE(Object) {
public:
	CDirect2DRMObject() {}
	~CDirect2DRMObject() {}

	BEGIN_COM_MAP(CDirect2DRMObject)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IDirect2DRMObject)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
	END_COM_MAP()

	D2DIERRORINFO_DECL(Direct2DRMObject);

	D2DREG_DECL(Direct2DRMObject, D2DRMOBJ);

public:
	//
	// IDirect2DRMObject Interface
	//
	D2DOBJMETHODS_DECL()
};
