// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// connlist.cpp
#include "stdafx.h"
#include "connlist.h"
#include "propbase.h"

CConnList::CConnList()
{
}

CConnList::~CConnList()
{
	// iterate through map, releasing worlds
	POSITION pos;

	for (pos=GetStartPosition(); pos!=NULL; )
	{
		IVWCommConnection *pvKey;
		ConnectionInfo cinfo;

		GetNextAssoc(pos, pvKey, cinfo);

		SAFERELEASE(cinfo.pWorld);
		SAFERELEASE(cinfo.pVWComm);
	}

    RemoveAll();
}

HRESULT CConnList::FindConnection(IVWCommConnection *pCommConnection, ConnectionInfo & cinfo)
{
	// lookup
	if (Lookup(pCommConnection, cinfo))
		return S_OK;
	//todo real hr.
	else return E_FAIL;
}

IWorld * CConnList::FindConnection(IVWCommConnection *pCommConnection)
{
	ConnectionInfo cinfo;

	// lookup
	if (Lookup(pCommConnection, cinfo))
		return cinfo.pWorld;
	else
		return NULL;
}

//3rd argument optional.
BOOL CConnList::AddConnection(IVWCommConnection *pCommConnection, IWorld *pWorld, IVWComm *vwcomm)
{
	if (FindConnection(pCommConnection) != NULL )
		return FALSE;
	ConnectionInfo cinfo;
	cinfo.pWorld=pWorld;
	cinfo.pVWComm=vwcomm;
	SAFEADDREF(vwcomm);
	SAFEADDREF(pWorld);
	SetAt( pCommConnection, cinfo );
	return TRUE;
}

BOOL CConnList::FindVWComm(IVWComm *pVWComm)
{
	// iterate through map, releasing worlds
	POSITION pos;

	for (pos=GetStartPosition(); pos!=NULL; )
	{
		IVWCommConnection *pvKey;
		ConnectionInfo cinfo;

		GetNextAssoc(pos, pvKey, cinfo);

		if(cinfo.pVWComm==pVWComm) return TRUE;
	}

	return FALSE;
}

BOOL CConnList::AddConnection(IVWCommConnection *pCommConnection, IWorld *pWorld)
{
	AddConnection(pCommConnection, pWorld, NULL);
	return TRUE;
}


BOOL CConnList::RemoveConnection(IVWCommConnection *pCommConnection)
{
	ConnectionInfo cinfo;

	// lookup
	if (Lookup(pCommConnection, cinfo))		
	{
		SAFERELEASE(cinfo.pWorld);		
		SAFERELEASE(cinfo.pVWComm);
		
		RemoveKey(pCommConnection);

		return TRUE;
	}
	else
		return FALSE;	
}

