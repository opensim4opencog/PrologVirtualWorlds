// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _CONNSERV_H_
#define _CONNSERV_H_

#include <afxtempl.h>

// V-Worlds optimized templates
#include <vwtempl.h>
#include <COMDEF.H>

struct ConnectionInfo
{
	ConnectionInfo(): pWorld(NULL), pVWComm(NULL) {}
	IWorld *pWorld;
	IVWComm *pVWComm;
	CComBSTR bstrWorldName;
};

class CWorldConnList : public CVWMap<IVWCommConnection *, IVWCommConnection *,ConnectionInfo, ConnectionInfo&>
{
public:
	CWorldConnList();
	~CWorldConnList();

	IWorld* FindConnection(IVWCommConnection *pCommConnection);
	HRESULT FindConnection(IVWCommConnection *pCommConnection, ConnectionInfo & cinfo);
	BOOL FindVWComm(IVWComm *pVWComm);
	//BOOL AddConnection(IVWCommConnection *pCommConnection, IWorld *pWorld);
	BOOL RemoveConnection(IVWCommConnection *pCommConnection);
	BOOL AddConnection(IVWCommConnection *pCommConnection, IWorld *pWorld, IVWComm *vwcomm, BSTR bstrWorldName);

	void RemoveAll();
};

#endif