// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// Foundatn.cpp : Implementation of CFoundationExemplars and DLL registration.

#include "stdafx.h"
#include <vwfound.h>
#include <vector.h>
#include <menuitem.h>
#include <vwutils.h>
#include <tranito.h>
#include "foundatn.h"
#include "propbase.h"

#define IID_DEFINED
#include "menuitem_i.c"
#include "avprof_i.c"
#include "vector_i.c"


/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP CFoundationExemplars::Install(IModule* pModule)
{
    HRESULT hr;

	// First, install extensions to the core exemplars
	if (FAILED(hr = CoreInstall(pModule)))
    	goto ERROR_ENCOUNTERED;

	// Install new exemplars (must be called after CoreInstall)
	if (FAILED(hr = PaintInstall(pModule)))
    	goto ERROR_ENCOUNTERED;
	if (FAILED(hr = SignInstall(pModule)))
	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = TMchInstall(pModule)))
	    goto ERROR_ENCOUNTERED;

    return S_OK;

ERROR_ENCOUNTERED:

	return hr;
}

HRESULT CFoundationExemplars::CoreInstall(IModule *pModule)
{
	CComPtr<IThing> pGlobal;
    HRESULT         hr;

	// install extensions to basic exemplars
	if (FAILED(hr = ThngInstall(pModule)))
	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = AvaInstall(pModule)))
	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = HostInstall(pModule)))
	    goto ERROR_ENCOUNTERED;

	// install global stuff
	if (FAILED(hr = GlobInstall(pModule)))
	    goto ERROR_ENCOUNTERED;

    return S_OK;

ERROR_ENCOUNTERED:
	
    return hr;
}