// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include <vwobject.h>
#include "wldinfo.h"
#include "propbase.h"

// unadvise the world without decrementing the reference count of the client.
//reduces refcount of pworld by 1.
void DeleteWorld(IWorld* pWorld, DWORD dwCookie)
{
	IConnectionPoint* pconnpt = NULL;
	IConnectionPointContainer* pconnptctr = NULL;

	if (pWorld == NULL) 
		return;

	HRESULT hr = pWorld->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pconnptctr);
	if (SUCCEEDED(hr))
	{
		hr = pconnptctr->FindConnectionPoint(IID_IVWObjectSite, &pconnpt);
		if (SUCCEEDED(hr))
		{
			// HACK: artificially addref ptr
			IUnknown* p = (IUnknown *)dwCookie;
			p->AddRef();

			pconnpt->Unadvise(dwCookie);
			pconnpt->Release();
		}
		pconnptctr->Release();
	}

	pWorld->Terminate();

	// REVIEW: let caller SAFERELEASE ptr rather than do it in here
//	SAFERELEASE(pWorld);
}

/////////////////////////////////////////////////////////////////////////////
// WorldInfo

CWorldInfo::CWorldInfo(IWorld *pWorld, DWORD dwCookie)
{	
	m_pWorld = pWorld; 
	SAFEADDREF(m_pWorld);

	m_dwCookie = dwCookie;
	m_cUsers = 0; 
} 

CWorldInfo::~CWorldInfo()
{
	DeleteWorld(m_pWorld, m_dwCookie);
	SAFERELEASE(m_pWorld);
}

           
           
           
                                   