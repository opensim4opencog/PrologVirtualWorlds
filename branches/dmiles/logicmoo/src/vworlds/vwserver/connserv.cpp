// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// connserv.cpp
#include "stdafx.h"
#include <vwobject.h>
#include <vwcomm.h>
#include "connserv.h"
#include "propbase.h"

CWorldConnList::CWorldConnList()
{
}

CWorldConnList::~CWorldConnList()
{
	RemoveAll();
}

void CWorldConnList::RemoveAll()
{
	// iterate through map, releasing worlds
	POSITION pos;

	for (pos = GetStartPosition(); pos != NULL; )
	{
		IVWCommConnection *pvKey = NULL;
		ConnectionInfo cinfo;

		GetNextAssoc(pos, pvKey, cinfo);

		SAFERELEASE(cinfo.pWorld);
		SAFERELEASE(cinfo.pVWComm);
	}

    CVWMap<IVWCommConnection *,IVWCommConnection *,ConnectionInfo,ConnectionInfo&>::RemoveAll();
}

HRESULT CWorldConnList::FindConnection(IVWCommConnection *pCommConnection, ConnectionInfo & cinfo)
{
	// lookup
	if (Lookup(pCommConnection, cinfo))
		return S_OK;
	// TODO: need real hr.
	else 
		return E_FAIL;
}

IWorld * CWorldConnList::FindConnection(IVWCommConnection *pCommConnection)
{
	ConnectionInfo cinfo;

	// lookup
	if (Lookup(pCommConnection, cinfo))
		return cinfo.pWorld;
	else
		return NULL;
}

//3rd argument optional.
BOOL CWorldConnList::AddConnection(IVWCommConnection *pCommConnection, IWorld *pWorld, IVWComm *vwcomm, BSTR bstrWorldName)
{
	if (FindConnection(pCommConnection) != NULL)
		return FALSE;

	ConnectionInfo cinfo;

	cinfo.pWorld = pWorld;
	cinfo.pVWComm = vwcomm;
	cinfo.bstrWorldName = bstrWorldName;
	SAFEADDREF(vwcomm);
	SAFEADDREF(pWorld);

	SetAt(pCommConnection, cinfo);

	SAFEADDREF(pCommConnection);

	return TRUE;
}

BOOL CWorldConnList::FindVWComm(IVWComm *pVWComm)
{
	POSITION pos;

	for (pos = GetStartPosition(); pos != NULL; )
	{
		IVWCommConnection *pvKey = NULL;
		ConnectionInfo cinfo;

		GetNextAssoc(pos, pvKey, cinfo);

		if (cinfo.pVWComm == pVWComm) 
			return TRUE;	 
	}

	return FALSE;
}

/*BOOL CWorldConnList::AddConnection(IVWCommConnection *pCommConnection, IWorld *pWorld)
{
	AddConnection(pCommConnection, pWorld, NULL, NULL);

	return TRUE;
}
*/

BOOL CWorldConnList::RemoveConnection(IVWCommConnection *pCommConnection)
{
	ConnectionInfo cinfo;

	if (Lookup(pCommConnection, cinfo))		
	{
		SAFERELEASE(cinfo.pWorld);		
		SAFERELEASE(cinfo.pVWComm);
		
		RemoveKey(pCommConnection);

		SAFERELEASE(pCommConnection);

		return TRUE;
	}
	else
		return FALSE;	
}

