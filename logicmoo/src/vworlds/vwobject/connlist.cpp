// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// connlist.cpp
#include "stdafx.h"
#include <vwobject.h>
#include <vwcomm.h>
#include "connlist.h"

CConnList::CConnList()
{
}

CConnList::~CConnList()
{
	m_map.RemoveAll();
}

IThing * CConnList::FindConnection(IVWCommConnection *pCommConnection)
{
	void *pvValue;

	// lookup
	if (m_map.Lookup((void *) pCommConnection, pvValue))
		return (IThing *) pvValue;
	else
		return NULL;
}

BOOL CConnList::AddConnection(IVWCommConnection *pCommConnection, IThing *pThing)
{
	if (FindConnection(pCommConnection) != NULL )
		return FALSE;

	m_map.SetAt( (void *) pCommConnection, (void *) pThing );
	return TRUE;
}

BOOL CConnList::RemoveConnection(IVWCommConnection *pCommConnection)
{
	if( FindConnection(pCommConnection) == NULL )
		return FALSE;

	m_map.RemoveKey( (void *) pCommConnection );
	return TRUE;
}

