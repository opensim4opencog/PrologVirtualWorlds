// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// CutTool.cpp : Implementation of CTools1App and DLL registration.

#include "stdafx.h"
#include <stdtools.h>
#include "CutTool.h"
	     
/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP CCutTool::InterfaceSupportsErrorInfo(REFIID riid)
{
	if (riid == IID_ICutTool)
		return S_OK;
	return S_FALSE;
}
