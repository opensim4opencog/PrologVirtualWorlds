// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// wldlist.cpp
#include "stdafx.h"
#include <vwobject.h>
#include "wldlist.h"
#include "queue.h"

CString ToStrA(int n, LPCTSTR pszFormat="%-d") // "%6d")
{
	CString str;

	str.Format(pszFormat, n);

	return str;
}

CWorldList::CWorldList()
{
	InitializeCriticalSection(&m_critsec);		
}

CWorldList::~CWorldList()
{
	RemoveAll();

	DeleteCriticalSection(&m_critsec);
}

CString CWorldList::Dump()
{
	EnterCriticalSection(&m_critsec);	
	
	CString output;

	output += "Number of open worlds:\t\t\t\t\t\t"+ToStrA(GetCount())+"\r\n";

	POSITION pos;
	
	for (pos = GetStartPosition(); pos != NULL; )
	{
		CString strKey;
		void *pvValue;

		GetNextAssoc(pos, strKey, pvValue);
		ASSERT( pvValue != NULL );
		CWorldInfo *pwi = (CWorldInfo *) pvValue;
		output += "\tWorld name: \""+strKey+"\"\r\n";
		output += "\t\tNumber of users: "+ToStrA(pwi->UserCount())+"\r\n";

		// get more info from world itself
		BSTR bstrInfo;

		HRESULT hr;
		hr = pwi->World()->GetStatus(&bstrInfo);
		if (SUCCEEDED(hr) && bstrInfo)
		{
			CString strInfo(bstrInfo);
			output += "\t\t";
			output += bstrInfo;

			::SysFreeString(bstrInfo);
		}
	}
	
	output += "\r\n";

	LeaveCriticalSection(&m_critsec);	
	return output;
}

void CWorldList::RemoveAll()
{
	// iterate through map, freeing worldinfo's
	EnterCriticalSection(&m_critsec);	

	POSITION pos;
		
	for (pos=GetStartPosition(); pos!=NULL; )
	{
		CString strKey;
		void *pvValue;

		GetNextAssoc(pos, strKey, pvValue);
		ASSERT( pvValue != NULL );

		CWorldInfo *pwi = (CWorldInfo *) pvValue;
		delete pwi;
	}

	CMapStringToPtr::RemoveAll();
	
	LeaveCriticalSection(&m_critsec);	
}

CWorldInfo*	CWorldList::FindWorld(CString strURL)
{
	EnterCriticalSection(&m_critsec);	

	strURL.MakeLower();
	void *pvValue;
	if (Lookup(strURL, pvValue))
	{
		LeaveCriticalSection(&m_critsec);	
		return (CWorldInfo *) pvValue;
	}
	else
	{
		LeaveCriticalSection(&m_critsec);	
		return NULL;
	}
}

CWorldInfo*	CWorldList::FindWorld(IWorld *pWorld)
{
	EnterCriticalSection(&m_critsec);	
	POSITION pos;

	for (pos=GetStartPosition(); pos!=NULL; )
	{
		CString strKey;
		void *pvValue;

		GetNextAssoc(pos, strKey, pvValue);
		ASSERT( pvValue != NULL );

		// check if this is the desired world
		CWorldInfo *pwi = (CWorldInfo *) pvValue;
		if ( pwi->World() == pWorld )
		{
			LeaveCriticalSection(&m_critsec);	
			return pwi;
		}
	}
	
	LeaveCriticalSection(&m_critsec);	

	return NULL;
}

CWorldInfo*	CWorldList::AddWorld(CString strURL, IWorld *pWorld, DWORD dwCookie)
{
	ASSERT( FindWorld(strURL) == NULL );
	ASSERT( FindWorld(pWorld) == NULL );
	EnterCriticalSection(&m_critsec);		

	strURL.MakeLower();
	CWorldInfo *pwi = new CWorldInfo(pWorld, dwCookie);
	if (pwi != NULL)
		SetAt( strURL, (void *) pwi );
	
	LeaveCriticalSection(&m_critsec);	
	return pwi;
}

BOOL CWorldList::RemoveWorld(CString strURL)
{
	EnterCriticalSection(&m_critsec);	

	strURL.MakeLower();

	CWorldInfo *pwi = FindWorld(strURL);
	
	if (pwi != NULL)
	{
		RemoveKey(strURL);
		delete pwi;
	}

	LeaveCriticalSection(&m_critsec);	
	return pwi != NULL;
}

BOOL CWorldList::RemoveWorld(IWorld *pWorld)
{
	POSITION pos;
	EnterCriticalSection(&m_critsec);	

	for (pos=GetStartPosition(); pos!=NULL; )
	{
		CString strKey;
		void *pvValue;

		GetNextAssoc(pos, strKey, pvValue);
		ASSERT( pvValue != NULL );

		// Is this the desired world?
		CWorldInfo *pwi = (CWorldInfo *) pvValue;
		if ( pwi->World() == pWorld )
		{
			// yes
			RemoveKey(strKey);
			delete pwi;

			LeaveCriticalSection(&m_critsec);	
			return TRUE;
		}
	}
	
	LeaveCriticalSection(&m_critsec);	
	
	return FALSE;
}

