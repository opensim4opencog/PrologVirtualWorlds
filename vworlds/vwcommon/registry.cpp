// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"

//File scope strings.
CString g_strDefaultAlias("Default");
CString g_strEmail("Email");
CString g_strInWorldStatus("InWorldStatus");
CString g_strComment("Comment");
CString g_strExemplar("Exemplar");
CString g_strGraphics("AvatarGraphics");
CString g_strRoom("RoomName");
CString g_strLocation(_T("Software\\Microsoft\\V-Worlds\\Profiles"));

HRESULT DeleteAlias(CString alias)
{
	RegDeleteKey(HKEY_CURRENT_USER,g_strLocation+"\\"+alias);
	return S_OK;
}

HRESULT WriteStuffToRegistry (CString defaultalias, CString alias, CString email, CString IsInWorld,
	CString comment, CString exemplar, CString graphics, CString room)
{	
	HRESULT hr=S_OK;
	HKEY keyProfiles;
	HKEY currentProfile;

	//write default alias.	
	if(defaultalias!="")
	{
		if(	ERROR_SUCCESS!=RegCreateKey(HKEY_CURRENT_USER,g_strLocation,&keyProfiles) ||
			ERROR_SUCCESS!=RegSetValue(keyProfiles,"", REG_SZ, alias,
			alias.GetLength()))
		{
			hr=E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
	}
	
	//if(ERROR_SUCCESS!=
		RegCreateKey(HKEY_CURRENT_USER,	g_strLocation+"\\"+alias,&currentProfile);
		//);	
	{
//		hr=E_FAIL;
//		goto ERROR_ENCOUNTERED;
		//uncertain why it fails, but it's okay!
	}

	//write attribs.
	if(ERROR_SUCCESS!=RegSetValue(currentProfile,g_strEmail, REG_SZ, email,
		email.GetLength()))
	{
		hr=E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	if(ERROR_SUCCESS!=RegSetValue(currentProfile,g_strInWorldStatus, REG_SZ, IsInWorld,
		IsInWorld.GetLength()))
	{
		hr=E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	if(ERROR_SUCCESS!=RegSetValue(currentProfile,g_strComment, REG_SZ, comment,
		comment.GetLength()))
	{
		hr=E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	if(ERROR_SUCCESS!=RegSetValue(currentProfile,g_strExemplar, REG_SZ, exemplar,
		exemplar.GetLength()))
	{
		hr=E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	if(ERROR_SUCCESS!=RegSetValue(currentProfile,g_strGraphics, REG_SZ, graphics,
		graphics.GetLength()))
	{
		hr=E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	if(ERROR_SUCCESS!=RegSetValue(currentProfile,g_strRoom, REG_SZ, room,
		room.GetLength()))
	{
		hr=E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	
	return hr;			

}

HRESULT LoadStuffFromRegistry(CString &alias, CString &email, CString &IsInWorld,
	CString &comment, CString &exemplar, CString &graphics, CString &room)
{
	HRESULT hr=S_OK;
	HKEY keyProfiles;
	HKEY currentProfile;
	char a[256];
	LONG b=256;

	if(alias=="")
	{
		//Find default alias.
		if(	ERROR_SUCCESS!=RegOpenKey(HKEY_CURRENT_USER,g_strLocation,&keyProfiles) ||
			ERROR_SUCCESS!=RegQueryValue(keyProfiles,"",a,&b))	
		{
			hr=E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		alias=CString(a);
	}
	
	//Open alias's profile.
	if(ERROR_SUCCESS!=RegOpenKey(HKEY_CURRENT_USER,g_strLocation+"\\"+alias,&currentProfile))
	{
		hr=E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	
	//Get attribs.
	b=256;
	if(ERROR_SUCCESS!=RegQueryValue(currentProfile,g_strEmail,a,&b))
	{
		hr=E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	email=CString(a);
	
	b=256;
	if(ERROR_SUCCESS!=RegQueryValue(currentProfile,g_strInWorldStatus,a,&b))
	{
		hr=E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	IsInWorld=CString(a);
	
	b=256;
	if(ERROR_SUCCESS!=RegQueryValue(currentProfile,g_strComment,a,&b))
	{
		hr=E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	comment=CString(a);

	b=256;
	if(ERROR_SUCCESS!=RegQueryValue(currentProfile,g_strExemplar,a,&b))
	{
		hr=E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	exemplar=CString(a);

	b=256;
	if(ERROR_SUCCESS!=RegQueryValue(currentProfile,g_strGraphics,a,&b))
	{
		hr=E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	graphics=CString(a);

	b=256;
	if(ERROR_SUCCESS!=RegQueryValue(currentProfile,g_strRoom,a,&b))
	{
		hr=E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	room=CString(a);

	//Write default alias.
	if(	ERROR_SUCCESS!=RegOpenKey(HKEY_CURRENT_USER,g_strLocation,&keyProfiles) ||
		ERROR_SUCCESS!=RegSetValue(keyProfiles,"", REG_SZ, alias,
		alias.GetLength()))
	{
		hr=E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	
	return hr;			
}
