// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _WLDLIST_H_
#define _WLDLIST_H_

#include "wldinfo.h"

/////////////////////////////////////////////////////////////////////////////
// WorldList

// threadsafe map of URL to WorldInfo
class CWorldList : private CMapStringToPtr	
{
public:
	CWorldList();
	~CWorldList();
	
	CWorldInfo*	FindWorld(CString strURL);
	CWorldInfo*	FindWorld(IWorld *pWorld);
	CWorldInfo*	AddWorld(CString strURL, IWorld *pWorld, DWORD dwCookie);
	BOOL		RemoveWorld(CString strURL);
	BOOL		RemoveWorld(IWorld *pWorld);
	CString		Dump();
	void GetNextAssocThreadSafe( POSITION& rNextPosition, CString& rKey, void * & rValue ) 
	{
		EnterCriticalSection(&m_critsec);		
		CMapStringToPtr::GetNextAssoc(rNextPosition,rKey,rValue );
		LeaveCriticalSection(&m_critsec);	
	}
	void		RemoveAll();
	LONG		GetCount() {return CMapStringToPtr::GetCount();}
	POSITION	GetStartPosition() {return CMapStringToPtr::GetStartPosition();}

private:
	CRITICAL_SECTION m_critsec;
};

#endif