// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.


// VWAdminObj.cpp : Implementation of CvwclientApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include <vwadmin.h>
#include "vwadmobj.h"

/////////////////////////////////////////////////////////////////////////////
// Macros and parameters

/////////////////////////////////////////////////////////////////////////////
//

CVWAdminObject::CVWAdminObject(void)
{
#ifdef TRACE_ALLOCATION
	TRACE("CVWAdminObject::CVWAdminObject, this 0x%0x\n", this);
#endif
} 

CVWAdminObject::~CVWAdminObject(void)
{
#ifdef TRACE_ALLOCATION
	TRACE("CVWAdminObject::~CVWAdminObject, this 0x%0x\n", this);
#endif

	Terminate();
}

// do complete shutdown.
STDMETHODIMP CVWAdminObject::Terminate()
{
#ifdef TRACE_ALLOCATION
	TRACE("CVWAdminObject::Terminate, this 0x%0x\n", this);
#endif

	HRESULT hr = S_OK;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return hr;
}

// ErrorInfo

STDMETHODIMP CVWAdminObject::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IVWAdmin
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
