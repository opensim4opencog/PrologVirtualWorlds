// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include <afxwin.h>
#include <objsafe.h>
#include "cathelp.h"

// Helper function to create a component category and associated description
HRESULT CreateComponentCategory(CATID catid, WCHAR* catDescription)
{
    ICatRegister* pcr = NULL ;
	IUnknown* punk = NULL;
    HRESULT hr = S_OK ;

    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
			NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&punk);
    if (SUCCEEDED(hr) && punk != NULL)
    {
		hr = punk->QueryInterface(IID_ICatRegister, (void**)&pcr);

		if (SUCCEEDED(hr) && pcr != NULL)
		{
			// Make sure the HKCR\Component Categories\{..catid...}
			// key is registered
			CATEGORYINFO catinfo;
			catinfo.catid = catid;
			catinfo.lcid = 0x0409 ; // english

			// Make sure the provided description is not too long.
			// Only copy the first 127 characters if it is
			int len = wcslen(catDescription);
			if (len>127)
				len = 127;
			wcsncpy(catinfo.szDescription, catDescription, len);
			// Make sure the description is null terminated
			catinfo.szDescription[len] = '\0';

			hr = pcr->RegisterCategories(1, &catinfo);

			pcr->Release();
		}
		
		punk->Release();
	}

	return hr;
}

// Helper function to register a CLSID as belonging to a component category
HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
// Register your component categories information.
    ICatRegister* pcr = NULL;
	IUnknown* punk = NULL;
    HRESULT hr = S_OK;

    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
			NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&punk);
    if (SUCCEEDED(hr) && punk != NULL)
    {
		hr = punk->QueryInterface(IID_ICatRegister, (void**)&pcr);

		if (SUCCEEDED(hr) && pcr != NULL)
		{
			// Register this category as being "implemented" by
			// the class.
			CATID rgcatid[1] ;
			rgcatid[0] = catid;
			hr = pcr->RegisterClassImplCategories(clsid, 1, rgcatid);

	        pcr->Release();
		}

		punk->Release();
	}

	return hr;
}

// Helper function to unregister a CLSID as belonging to a component category
HRESULT UnRegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
    ICatRegister* pcr = NULL;
	IUnknown* punk = NULL;
    HRESULT hr = S_OK;

    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
			NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&punk);
    if (SUCCEEDED(hr) && punk != NULL)
    {
		hr = punk->QueryInterface(IID_ICatRegister, (void**)&pcr);

		if (SUCCEEDED(hr) && pcr != NULL)
		{
			// Unregister this category as being "implemented" by
			// the class.
			CATID rgcatid[1] ;
			rgcatid[0] = catid;
			hr = pcr->UnRegisterClassImplCategories(clsid, 1, rgcatid);

			pcr->Release();
		}

		punk->Release();
	}
  
	return hr;
}

// returns S_OK if in category, S_FALSE if not
HRESULT IsCLSIDInImplCategory(REFCLSID clsid, CATID catid)
{
	ICatInformation* pci = NULL;
	IUnknown* punk = NULL;
    HRESULT hr = S_OK;

    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
			NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&punk);
    if (SUCCEEDED(hr) && punk != NULL)
    {
		hr = punk->QueryInterface(IID_ICatInformation, (void**)&pci);

		if (SUCCEEDED(hr) && pci != NULL)
		{
			// Check if the class is registered with the component category
			CATID rgcatid[1] ;
			rgcatid[0] = catid;
			hr = pci->IsClassOfCategories(clsid, 1, rgcatid,0,NULL);

			pci->Release();
		}

		punk->Release();
	}

	return hr;
}

