// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _WLDINFO_H_
#define _WLDINFO_H_

//A fcn to release the world in the proper way.
void DeleteWorld(IWorld *pworld,DWORD dwCookie);

/////////////////////////////////////////////////////////////////////////////
// WorldInfo

class CWorldInfo
{
public:
	CWorldInfo(IWorld *pWorld, DWORD dwCookie);
	~CWorldInfo();

	IWorld * World()	{ return m_pWorld;	}
	int AddUser()		{ return ++m_cUsers;}  
	int RemoveUser()	{ return --m_cUsers;}  
	int UserCount()		{ return m_cUsers;	}
	DWORD get_Cookie()	{return m_dwCookie;}
protected:
	IWorld *m_pWorld;
	int		m_cUsers;
	DWORD	m_dwCookie;
};

#endif