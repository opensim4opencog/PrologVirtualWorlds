// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "comcat.h"

// Helper function to create a component category and associated description
HRESULT CreateComponentCategory(CATID catid, WCHAR* catDescription);

// Helper function to register a CLSID as belonging to a component category
HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid);

// Helper function to unregister a CLSID as belonging to a component category
HRESULT UnRegisterCLSIDInCategory(REFCLSID clsid, CATID catid);

// Helper to see if CLSID belongs to a category
HRESULT IsCLSIDInImplCategory(REFCLSID clsid, CATID catid);
