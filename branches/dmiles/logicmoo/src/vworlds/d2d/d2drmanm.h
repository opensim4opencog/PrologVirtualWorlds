// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// D2DRMAnm.h : Declaration of the CDirect2DRMAnimation

#include "resource.h"       // main symbols
#include "d2dbase.h"

/////////////////////////////////////////////////////////////////////////////
// d2d

class ATL_NO_VTABLE CDirect2DRMAnimation : D2DOBJECT_BASE(Animation) {
public:
	CDirect2DRMAnimation();
	~CDirect2DRMAnimation();

	BEGIN_COM_MAP(CDirect2DRMAnimation)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IDirect2DRMObject)
		COM_INTERFACE_ENTRY(IDirect2DRMAnimation)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
	END_COM_MAP()

	D2DOBJECT_BASE_CLASS(Animation)
	
	D2DOBJREG_DECL(Animation, D2DRMANM);
	
public:
	//
	// IDirect2DRMAnimation Interface
	//
};
