// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _CONNLIST_H_
#define _CONNLIST_H_

class CConnList
{
public:
	CConnList();
	~CConnList();

	IThing *	FindConnection(IVWCommConnection *pCommConnection);
	BOOL		AddConnection(IVWCommConnection *pCommConnection, IThing* pthing);
	BOOL		RemoveConnection(IVWCommConnection *pCommConnection);
	BOOL		IsEmpty() {return m_map.IsEmpty();}
protected:
	// map of connection to world
	CMapPtrToPtr	m_map;
};

#endif