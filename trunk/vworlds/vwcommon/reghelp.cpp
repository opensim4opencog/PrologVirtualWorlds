// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include "reghelp.h"
#include "propbase.h"

#ifdef VWSYSTEM_BUILD
#include "..\vwobject\propsecu.h"
#endif

EXTERN_C const CLSID CLSID_PropertyList;

CString gStrToolsKey(_T("Software\\Microsoft\\V-Worlds\\Tools"));
CString gStrAvatarKey(_T("Software\\Microsoft\\V-Worlds\\Avatars"));
CString gStrPathKey(_T("Software\\Microsoft\\V-Worlds\\Paths"));
CString gStrWorldsKey(_T("Software\\Microsoft\\V-Worlds\\Worlds"));
CString gStrSDKKey(_T("Software\\Microsoft\\V-Worlds"));

// REVIEW: don't use default paths, but return specific error codes
//#define USE_DEFAULTPATHS

const CString	kstrAvatarPath	= "AvatarPath";
const CString	kstrContentPath	= "ContentPath";
const CString	kstrAvatarGraphicsPath = "AvatarGraphicsPath";
const CString	kstrWorldPath = "WorldPath";
const CString	kstrHelpPath = "HelpPath";
const CString	kstrDSURL		= "DirectoryServiceURL";
const CString	kstrDSPage		= "DirectoryServicePage";
const CString	kstrWorldWiz = "WorldWizPath";
const CString	kstrIsSDK = "isSDK";

const CString	kstrDefaultWorldPath = "c:\\Program Files\\Microsoft Virtual Worlds\\Worlds\\";
const CString	kstrDefaultAvatarPath = "c:\\Program Files\\Microsoft Virtual Worlds\\Avatars\\";
const CString	kstrDefaultContentPath = "c:\\Program Files\\Microsoft Virtual Worlds\\Local Content\\";
const CString	kstrDefaultAvatarGraphicsPath = "c:\\Program Files\\Microsoft Virtual Worlds\\Avatar Graphics\\";
const CString	kstrDefaultHelpPath = "c:\\Program Files\\Microsoft Virtual Worlds\\docs\\";
const CString	kstrDefaultDSPage = "client\\html\\dservice.htm";


//******************************************************
/*
	VWorld Registry Keys

  HKEY_LOCAL_MACHINE
	Software\Microsoft\V-Worlds\Authentication
	Software\Microsoft\V-Worlds\InstallPath
	Software\Microsoft\V-Worlds\isSDK
	Software\Microsoft\V-Worlds\LocalGroup
	Software\Microsoft\V-Worlds\MaxUsers
	Software\Microsoft\V-Worlds\Paths\AvatarGraphicsPath
	Software\Microsoft\V-Worlds\Paths\AvatarPath
	Software\Microsoft\V-Worlds\Paths\ContentPath
	Software\Microsoft\V-Worlds\Paths\DirectoryServicePage
	Software\Microsoft\V-Worlds\Paths\DirectoryServiceURL
	Software\Microsoft\V-Worlds\Paths\HelpPath
	Software\Microsoft\V-Worlds\Paths\WorldPath
	Software\Microsoft\V-Worlds\Paths\WorldWizPath

  HKEY_CURRENT_USER
	Software\Microsoft\V-Worlds\Avatars\<avname(x)>\(Default)  // this is the Path
  	Software\Microsoft\V-Worlds\Worlds\<worldguid(x)>\FriendlyName
	Software\Microsoft\V-Worlds\Worlds\<worldguid(x)>\InfoURL
	Software\Microsoft\V-Worlds\Worlds\<worldguid(x)>\NotInDirectory
	Software\Microsoft\V-Worlds\Worlds\<worldguid(x)>\Offline
	Software\Microsoft\V-Worlds\Worlds\<worldguid(x)>\Path

*/
//******************************************************


//******************************************************
// FindAvatarInRegistry
// Given a BSTR of an avatar alias, returns the path

HRESULT	FindAvatarInRegistry(BSTR bstrAvatarAlias, BSTR *pbstrPath)
{
	HRESULT	hr			= E_FAIL;
	CString	strAlias	= bstrAvatarAlias;
	HKEY	keyAvatars;
	DWORD	dwStatus;

	ASSERT(pbstrPath);
	if (pbstrPath == NULL)
		return E_INVALIDARG;

	*pbstrPath = NULL;

	// Open the AVATAR key
	if (RegCreateKeyEx(HKEY_CURRENT_USER, gStrAvatarKey, 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &keyAvatars, &dwStatus) == ERROR_SUCCESS)
	{
		LONG		lResult	= ERROR_SUCCESS;
		char		strKeyName[64];
		DWORD		dwAvCount = 0;
		DWORD		dwNameLen;
		FILETIME	ftKeyTime;

		// now, enumerate them...
		do
		{
			dwNameLen = 64;
			lResult =  RegEnumKeyEx(keyAvatars, dwAvCount, strKeyName, &dwNameLen, 
				NULL, NULL, NULL, &ftKeyTime);
			
			if (lResult == ERROR_SUCCESS)
			{
				dwAvCount++;
				
				if (!CString(strKeyName).CompareNoCase(strAlias))
				{
					char	strPath[256];
					long	lPathLen = 255;

					// fetch the value of that key
					lResult = RegQueryValue(keyAvatars, strKeyName, strPath, &lPathLen); 
					
					if (lResult == ERROR_SUCCESS)
					{
						*pbstrPath = CComBSTR(strPath).Copy();
						hr = S_OK;
						break;
					}
				}
			}
		}
		while (lResult == ERROR_SUCCESS);

		// clean up
		RegCloseKey(keyAvatars);
	}

	return hr;
}


//******************************************************
// FindDefaultPathInReg
// Returns the default directory that avatar files are stored in

HRESULT	FindDefaultPathInReg(CString strWhichPath, BSTR *pbstrPath)
{
	HRESULT	hr = E_FAIL;
	HKEY keyPaths;
	DWORD dwStatus;

	// Open the PATH key
	ASSERT(pbstrPath);

	if (pbstrPath == NULL)
		return E_POINTER;

	*pbstrPath = NULL;

	if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, gStrPathKey, 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &keyPaths, &dwStatus)
		== ERROR_SUCCESS)
	{
		LONG lResult = ERROR_SUCCESS;
		char strPath[256];
		DWORD lPathLen = 255;

		// fetch the value of that key 
		lResult = RegQueryValueEx(keyPaths, strWhichPath, NULL, NULL, (LPBYTE)strPath, &lPathLen); 
		
		if (lResult == ERROR_SUCCESS)
		{
			*pbstrPath = CComBSTR(strPath).Copy();
			hr = S_OK;
		}

		// clean up
		RegCloseKey(keyPaths);
	}

	return hr;
}

//********************************************
// GetDirectoryServiceURL
// Returns the URL for the directory service
// out of the registry.

HRESULT GetDirectoryServiceURL(BSTR *pbstrURL)
{
	HRESULT	hr = S_OK;

	hr = FindDefaultPathInReg(kstrDSURL, pbstrURL);

	if (FAILED(hr))
	{
		TRACE("GetDirectoryServiceURL: DSURL registry key not found\n");
		hr = VWOBJECT_E_INVALIDDSURL;
	}

	return hr;
}


//********************************************
// SetDirectoryServiceURL
// Sets the URL for the directory service
// in the registry.

HRESULT SetDirectoryServiceURL(BSTR bstrURL)
{
	HRESULT	hr			= E_FAIL;
	CString	strURL		= bstrURL;
	HKEY	keyPath;
	DWORD	dwStatus;

	// Open the PATHS key
	if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, gStrPathKey, 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &keyPath, &dwStatus) == ERROR_SUCCESS)
	{
		LONG	lResult;
		long	len = strURL.GetLength()+1;
		char	*pBuffer = strURL.GetBuffer(len);

		// we have the key - add the value...
		//lResult = RegSetValue(keyPath, kstrDSURL, REG_SZ, strURL, strURL.GetLength()+1);
		lResult = ::RegSetValueEx(keyPath, kstrDSURL, 0, REG_SZ, (byte *)pBuffer, len);
		if (lResult == ERROR_SUCCESS)
			hr = S_OK;
		// clean up
		RegCloseKey(keyPath);
	}

	return hr;
}


//******************************************************
// FindAvatarPath
// Returns the default directory that avatar files are stored in

HRESULT	FindAvatarPath(BSTR *pbstrPath)
{
	HRESULT	hr = S_OK;

	hr = FindDefaultPathInReg(kstrAvatarPath, pbstrPath);
#ifdef USE_DEFAULT_PATHS
	if (FAILED(hr))
	{
		TRACE0("WARNING:  Using default Avatar Path!\n");
		*pbstrPath = CComBSTR(kstrDefaultAvatarPath).Copy();
	}
#endif

	if (FAILED(hr))
	{
		TRACE("FindAvatarPath: AvatarPath registry key not found\n");
		hr = VWOBJECT_E_INVALIDAVATARPATH;
	}

	return hr;
}

//******************************************************
// FindHelpPath
// Returns the default directory that avatar files are stored in

HRESULT	FindHelpPath(BSTR *pbstrPath)
{
	HRESULT	hr = S_OK;

	hr = FindDefaultPathInReg(kstrHelpPath, pbstrPath);
#ifdef USE_DEFAULT_PATHS
	if (FAILED(hr))
	{
		TRACE0("WARNING:  Using default Help Path!\n");
		*pbstrPath = CComBSTR(kstrDefaultHelpPath).Copy();
	}
#endif

	if (FAILED(hr))
	{
		TRACE("FindHelpPath: HelpPath registry key not found\n");
		hr = VWOBJECT_E_INVALIDHELPPATH;
	}

	return hr;
}


//******************************************************
// FindDirectoryServicePage
// Returns the default directory service HTML page

HRESULT	FindDirectoryServicePage(BSTR *pbstrPath)
{
	HRESULT	hr = S_OK;

	hr = FindDefaultPathInReg(kstrDSPage, pbstrPath);
#ifdef USE_DEFAULT_PATHS
	if (FAILED(hr))
	{
		TRACE0("WARNING:  Using default Directory Service Page!\n");
		*pbstrPath = CComBSTR(kstrDefaultDSPage).Copy();
	}
#endif

	if (FAILED(hr))
	{
		TRACE("FindDirectoryServicePage: DirectoryServicePage registry key not found\n");
		hr = VWOBJECT_E_INVALIDDIRECTORYSERVICEPAGE;
	}

	return hr;
}


//******************************************************
// FindContentPath
// Returns the default directory that avatar files are stored in

HRESULT	FindContentPath(BSTR *pbstrPath)
{
	HRESULT	hr = S_OK;

	hr = FindDefaultPathInReg(kstrContentPath, pbstrPath);
#ifdef USE_DEFAULT_PATHS
	if (FAILED(hr))
	{
		TRACE0("WARNING:  Using default Content Path!\n");
		*pbstrPath = CComBSTR(kstrDefaultContentPath).Copy();
	}
#endif

	if (FAILED(hr))
	{
		TRACE("FindContentPath: ContentPath registry key not found\n");
		hr = VWOBJECT_E_INVALIDCONTENTPATH;
	}

	return hr;
}

//******************************************************
// FindAvatarGraphicsPath
// Returns the default directory that avatar files are stored in

HRESULT	FindAvatarGraphicsPath(BSTR *pbstrPath)
{
	HRESULT	hr = S_OK;

	hr = FindDefaultPathInReg(kstrAvatarGraphicsPath, pbstrPath);
#ifdef USE_DEFAULT_PATHS
	if (FAILED(hr))
	{
		TRACE0("WARNING:  Using default Avatar Graphics Path!\n");
		*pbstrPath = CComBSTR(kstrAvatarGraphicsPath).Copy();
	}
#endif

	if (FAILED(hr))
	{
		TRACE("FindAvatarGraphicsPath: AvatarGraphicsPath registry key not found\n");
		hr = VWOBJECT_E_INVALIDAVATARGRAPHICSPATH;
	}

	return hr;
}


//******************************************************
// FindWorldPath
// Returns the default directory that world files are stored in

HRESULT	FindWorldPath(BSTR *pbstrPath)
{
	HRESULT	hr = S_OK;

	hr = FindDefaultPathInReg(kstrWorldPath, pbstrPath);
#ifdef USE_DEFAULT_PATHS
	if (FAILED(hr))
	{
		TRACE0("WARNING:  Using default World Path!\n");
		*pbstrPath = CComBSTR(kstrDefaultWorldPath).Copy();
	}
#endif

	if (FAILED(hr))
	{
		TRACE("FindWorldPath: WorldPath registry key not found\n");
		hr = VWOBJECT_E_INVALIDWORLDPATH;
	}

	return hr;
}

HRESULT	FindWorldWizardPath(BSTR *pbstrPath)
{
	HRESULT	hr = S_OK;

	hr = FindDefaultPathInReg(kstrWorldWiz, pbstrPath);

	if (FAILED(hr))
	{
		TRACE("FindWorldWizardPath: WorldWiz registry key not found\n");
		hr = VWOBJECT_E_INVALIDWORLDPATH;
	}

	return hr;
}

HRESULT	GetIsSDK(VARIANT_BOOL *vBool)
{
	HRESULT	hr = E_FAIL;
	HKEY keyPaths;
	DWORD dwStatus;
	CString strValue;

	if (vBool == NULL)
		return E_POINTER;

	*vBool = VARIANT_FALSE;

	if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, gStrSDKKey, 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &keyPaths, &dwStatus)
		== ERROR_SUCCESS)
	{
		LONG lResult = ERROR_SUCCESS;
		char strPath[256];
		DWORD lPathLen = 255;

		// fetch the value of that key
		lResult = RegQueryValueEx(keyPaths, kstrIsSDK, NULL, NULL, (LPBYTE)strPath, &lPathLen); 
		
		if (lResult == ERROR_SUCCESS)
		{
			strValue = strPath;
			//*pbstrPath = CComBSTR(strPath).Copy();
			if (strValue.Compare("1") == 0)
				*vBool = VARIANT_TRUE;
			else
				*vBool = VARIANT_FALSE;
			hr = S_OK;
		}

		// clean up
		RegCloseKey(keyPaths);
	}

	return hr;
}

// Only compile these methods in VWSYSTEM
#ifdef VWSYSTEM_BUILD
//******************************************************
// ComposeRootURLPath
// Gets ContentPath, AvatarGraphicsPath,and adds 
// world-specific paths to these to create a RootURL

HRESULT ComposeRootURLPath(IWorld* pWorld, CComBSTR* bstrRootURLPath)
{
	HRESULT hr = S_OK;
	IThing* pGlobal;
	CComBSTR bstrAvatarGraphicsPath, bstrContentPath;
	CComBSTR bstrWorldAvatarGraphicsPath, bstrWorldContentPath;
	CComBSTR bstrWorldName;

	// If any of the FindPath's fail, we want to keep getting as many paths as possible, so don't error out
	hr = FindContentPath(&bstrContentPath.m_str);
	if (FAILED(hr))
		TRACE("ComposeRootURL: ContentPath not found\n");

	hr = FindAvatarGraphicsPath(&bstrAvatarGraphicsPath.m_str);
	if (FAILED(hr))
		TRACE("ComposeRootURL: AvatarGraphicsPath not found\n");

	// Now get the current world's name - if we don't have a Global or WorldName, error out
	hr = pWorld->get_Global(&pGlobal);
	if (FAILED(hr))
	{
		TRACE("ComposeRootURL: Failed to get global\n");
		goto ERROR_ENCOUNTERED;
	}

	hr = pGlobal->get_String(CComBSTR("WorldName"),&bstrWorldName.m_str);
	if (FAILED(hr))
	{
		TRACE("ComposeRootURL: Failed to get WorldName\n");
		goto ERROR_ENCOUNTERED;
	}

	// if any data in RootURLPath, prepend semicolon
	if ((*bstrRootURLPath).Length() > 0)
		*bstrRootURLPath += ";";

	// Add the directories to RootURL as defined in the registry. If there is a world name, 
	// add world-specific directories.
	if (bstrContentPath.Length() > 0)
	{
		if (bstrWorldName.Length() > 0)
		{
			// Add world content path, e.g., worlds\plazas
			bstrWorldContentPath =  "file://";
			bstrWorldContentPath += bstrContentPath;
			bstrWorldContentPath += "Worlds\\";
			bstrWorldContentPath += bstrWorldName;
			bstrWorldContentPath += "\\";
		}

		*bstrRootURLPath += CComBSTR(CString("file://") + CString(bstrContentPath));
		*bstrRootURLPath += ";";
		*bstrRootURLPath += bstrWorldContentPath;
		*bstrRootURLPath += ";";

		// Now set global WorldContentPath
		hr = SetWorldContentPath(pWorld, bstrWorldContentPath);
		if (FAILED(hr))
		{
			TRACE("ComposeRootURL: WorldContentPath not set\n");
			goto ERROR_ENCOUNTERED;
		}
	}

	if (bstrAvatarGraphicsPath.Length() > 0)
	{
		if (bstrWorldName.Length() > 0)
		{
			// Add world avatar graphics path, e.g., worlds\plazas\avatar graphics
			bstrWorldAvatarGraphicsPath += bstrWorldContentPath;
			bstrWorldAvatarGraphicsPath += "Avatar Graphics";
			bstrWorldAvatarGraphicsPath += "\\";
		}

		*bstrRootURLPath += CComBSTR(CString("file://") + CString(bstrAvatarGraphicsPath));
		*bstrRootURLPath += ";";
		*bstrRootURLPath += bstrWorldAvatarGraphicsPath;
		*bstrRootURLPath += ";";

		// Now set global WorldAvatarGraphicsPath
		hr = SetWorldAvatarGraphicsPath(pWorld, bstrWorldAvatarGraphicsPath);
		if (FAILED(hr))
		{
			TRACE("ComposeRootURL: WorldAvatarGraphicsPath not set\n");
			goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pGlobal);

	return hr;
}

//******************************************************
// SetWorldContentPath
// Sets the WorldContentPath with "worlds\worldname" at end

HRESULT SetWorldContentPath(IWorld* pWorld, CComBSTR bstrNewWorldContentPath)
{
	HRESULT hr = S_OK;
	IThing * pGlobal;

	hr = pWorld->get_Global(&pGlobal);
	if (FAILED(hr))
	{
		TRACE("SetWorldContentPath: Failed to get global\n");
		goto ERROR_ENCOUNTERED;
	}

	{
		CSetUID SetUID(pWorld, pGlobal, pGlobal, VARIANT_TRUE);

		hr = pGlobal->put_String(CComBSTR("WorldContentPath"), bstrNewWorldContentPath);
		if (FAILED(hr))
		TRACE("SetWorldContentPath: Failed to set World content path\n");
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pGlobal);
	return hr;
}


//******************************************************
// SetWorldAvatarGraphicsPath
// Sets the WorldAvatarGraphicsPath with "worlds\worldname" at end

HRESULT SetWorldAvatarGraphicsPath(IWorld* pWorld, CComBSTR bstrNewWorldAvatarGraphicsPath)
{
	HRESULT hr = S_OK;
	IThing * pGlobal;

	hr = pWorld->get_Global(&pGlobal);
	if (FAILED(hr))
	{
		TRACE("SetWorldAvatarGraphicsPath: Failed to get global\n");
		goto ERROR_ENCOUNTERED;
	}

	{
		CSetUID SetUID(pWorld, pGlobal, pGlobal, VARIANT_TRUE);

		hr = pGlobal->put_String(CComBSTR("WorldAvatarGraphicsPath"), bstrNewWorldAvatarGraphicsPath);
		if (FAILED(hr))
			TRACE("SetWorldAvatarGraphicsPath: Failed to set World content path\n");
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pGlobal);
	return hr;
}

#endif  // end methods only compiled into VWSYSTEM

//******************************************************
// AddAvatarToRegistry
// Adds the specified avatar to the registry

HRESULT	AddAvatarToRegistry(BSTR bstrAlias, BSTR bstrPath)
{
	HRESULT	hr			= E_FAIL;
	CString	strAlias	= bstrAlias;
	CString	strPath		= bstrPath;
	HKEY	keyAvatars;
	DWORD	dwStatus;

	// Open the AVATAR key
	if (RegCreateKeyEx(HKEY_CURRENT_USER, gStrAvatarKey, 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &keyAvatars, &dwStatus) == ERROR_SUCCESS)
	{
		LONG	lResult;

		// we have the key - add the value...
		lResult = RegSetValue(keyAvatars, strAlias, REG_SZ, strPath, strPath.GetLength()+1);
		
		if (lResult == ERROR_SUCCESS)
			hr = S_OK;

		// clean up
		RegCloseKey(keyAvatars);
	}

	return hr;
}

HRESULT CanonDatabaseName(BSTR bstrIn, BSTR *pbstrOut)
{
	HRESULT hr = S_OK;
	CComBSTR bstrOut;
/*	char szDrive[_MAX_DRIVE];
	char szDir[_MAX_DIR];
	char szFName[_MAX_FNAME];
	char szExt[_MAX_EXT];
*/
	if (pbstrOut == NULL)
		return E_POINTER;

	// initialize
	*pbstrOut = NULL;

	if (bstrIn == NULL)
	{
		TRACE("CanonDatabaseName: invalid URL, NULL string\n");
		return VWOBJECT_E_INVALIDURLFORMAT;
	}

	// split it up
	//_splitpath(CString(bstrIn), szDrive, szDir, szFName, szExt);

	// Don't do the check here. Do it at higher levels. carlh.
	// if anything specified except FName, fail	
	/*	if (szDrive[0] != '\0'	||
		szDir[0] != '\0'	||
		szExt[0] != '\0' )
	{
		// fail
		TRACE("CanonDatabaseName: invalid URL, shouldn't pass in drive, directory or extension\n");
		return VWOBJECT_E_INVALIDURLFORMAT;
	}
*/
	hr = FindWorldPath(&bstrOut.m_str);

	if (SUCCEEDED(hr))
	{
		bstrOut += bstrIn;

		*pbstrOut = bstrOut.Copy();
	}

	return hr;
}

//******************************************************
// FindWorldInRegistry
// Finds a world in the registry and returns the info on it.

HRESULT FindWorldInRegistry(BSTR bstrGUID, BSTR *pbstrPath, BSTR *pbstrName)
{
	HRESULT	hr = S_OK;
	CString	strGUID(bstrGUID);
	CString	strPath;
	CString	strName;
	CComBSTR bstrName, bstrPath;
	HKEY keyWorlds;
	HKEY keyThisWorld;
	DWORD dwStatus;

	// Open the WORLD key
	if (RegCreateKeyEx(HKEY_CURRENT_USER, gStrWorldsKey, 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &keyWorlds, &dwStatus) == ERROR_SUCCESS)
	{
		if (RegCreateKeyEx(keyWorlds, strGUID, 0, NULL, 
			REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &keyThisWorld, &dwStatus) == ERROR_SUCCESS)
		{
			LONG lResult;
			DWORD dwSize;
			BYTE path[1024];
		
			// we have the key - add the value...
			dwSize = 1024;
			lResult = RegQueryValueEx(keyThisWorld, "Path", 0, NULL, path, &dwSize);

			if (lResult == ERROR_SUCCESS)
			{
				BYTE name[256];

				dwSize = 256;
				lResult = RegQueryValueEx(keyThisWorld, "FriendlyName", 0, NULL, name, &dwSize);

				if (lResult == ERROR_SUCCESS)
				{
					// copy them over
					bstrName = (LPSTR)name;
					bstrPath = (LPSTR)path;

					*pbstrPath = bstrPath.Copy();
					*pbstrName = bstrName.Copy();
				}
				else
				{
					TRACE("FindWorldInRegistry: failed to query FriendlyName value for World, GUID %s\n", strGUID);
					hr = VWOBJECT_E_INVALIDWORLDREGISTRY;
				}
			}
			else
			{
				TRACE("FindWorldInRegistry: failed to query Path value for World, GUID %s\n", strGUID);
				hr = VWOBJECT_E_INVALIDWORLDREGISTRY;
			}

			// clean up
			RegCloseKey(keyThisWorld);
		}

		RegCloseKey(keyWorlds);
	}

	return hr;
}

//******************************************************
// This function iterates through the registry 
// and removes all entries with the given path.
HRESULT FindAndPurgeWorldsByPath(BSTR bstrInPath)
{
	HRESULT	hr = S_OK;
	HKEY keyWorlds = NULL;
	DWORD dwStatus;

	// 1. Open the WORLD key
	if (RegCreateKeyEx(HKEY_CURRENT_USER, gStrWorldsKey, 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &keyWorlds, &dwStatus) == ERROR_SUCCESS)
	{
		DWORD dwCurKey = 0;
		char curGUID[255];
		DWORD dwStrSize;
		LONG lResult;
		FILETIME fileTime;
		CComBSTR bstrGUID;
		
		// Iterate to populate the list
		do
		{
			CComBSTR bstrPath, bstrName;

			dwStrSize = 255;

			lResult = RegEnumKeyEx(keyWorlds, dwCurKey, curGUID, &dwStrSize, NULL, NULL, NULL, &fileTime);
			if (lResult != ERROR_SUCCESS)
				continue;

			bstrGUID = curGUID;

			//check if right path.
			hr = FindWorldInRegistry(bstrGUID, &bstrPath.m_str, &bstrName.m_str);
			if (FAILED(hr))
			{
				// if regkey is screwy, remove the world entry
				/* hr = */ RemoveWorldFromRegistry(bstrGUID);

				hr = S_OK;

				continue;
			}

			if (!CString(bstrPath).CompareNoCase(CString(bstrInPath)))
			{
				/* hr = */ RemoveWorldFromRegistry(bstrGUID);
			}		
			else
				dwCurKey++;
		}
		while (lResult == ERROR_SUCCESS);
	}

	if (keyWorlds) 
		RegCloseKey(keyWorlds);
	
	return hr;
}

//******************************************************
// RemoveWorldFromRegistry
// Removes a world from the registry

HRESULT RemoveWorldFromRegistry(BSTR bstrGUID)
{
	HRESULT	hr = S_OK;
	CString	strGUID(bstrGUID);
	HKEY keyWorlds = NULL;
	DWORD dwStatus;

	// Open the WORLD key
	if (RegCreateKeyEx(HKEY_CURRENT_USER, gStrWorldsKey, 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &keyWorlds, &dwStatus) == ERROR_SUCCESS)
	{
		LONG lResult;
		
		// TO DO:  Do we want to delete the world?

		// we have the key - add the value...
		lResult = RegDeleteKey(keyWorlds, strGUID);
		if (lResult != ERROR_SUCCESS)
		{
			TRACE("RemoveWorldFromRegistry: failed to delete World key, GUID %s\n", strGUID);
			hr = VWOBJECT_E_INVALIDWORLDREGISTRY;
		}

		RegCloseKey(keyWorlds);
	}

	return hr;
}

//******************************************************
// This should only be in the server,
// but I want to keep the sengine approach still working.
// AddWorldToRegistry
// Adds a world into the registry
HRESULT AddWorldToRegistry(BSTR bstrGUID, BSTR bstrPath, BSTR bstrName)
{
	HRESULT	hr = S_OK;
	CString	strGUID(bstrGUID);
	CString	strPath(bstrPath);
	CString	strName(bstrName);
	HKEY keyWorlds;
	HKEY keyThisWorld;
	DWORD dwStatus;

	hr = FindAndPurgeWorldsByPath(bstrPath);
	if (FAILED(hr))
		return hr;

	// if temporary world, don't stick in registry
	char szFName[_MAX_FNAME];

	_splitpath(strPath, NULL, NULL, szFName, NULL);

	if (szFName[0] == '~')
		return S_OK;

	// Open the WORLD key
	if (RegCreateKeyEx(HKEY_CURRENT_USER, gStrWorldsKey, 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &keyWorlds, &dwStatus) == ERROR_SUCCESS)
	{
		if (RegCreateKeyEx(keyWorlds, strGUID, 0, NULL, 
			REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &keyThisWorld, &dwStatus) == ERROR_SUCCESS)
		{
			LONG	lResult;

			// we have the key - add the value...
			lResult = RegSetValueEx(keyThisWorld, "Path", 0, REG_SZ, (const unsigned char *)((const char *)strPath), strPath.GetLength()+1);
			if (lResult != ERROR_SUCCESS)
			{
				TRACE("AddWorldToRegistry: failed to set Path value for World, GUID %s\n", strGUID);
				hr = VWOBJECT_E_INVALIDWORLDREGISTRY;
				goto ERROR_ENCOUNTERED;
			}
			
			lResult = RegSetValueEx(keyThisWorld, "FriendlyName", 0, REG_SZ, (const unsigned char *)((const char *)strName), strName.GetLength()+1);
			if (lResult != ERROR_SUCCESS)
			{
				TRACE("AddWorldToRegistry: failed to set FriendlyName value for World, GUID %s\n", strGUID);
				hr = VWOBJECT_E_INVALIDWORLDREGISTRY;
			}

ERROR_ENCOUNTERED:
			// clean up
			RegCloseKey(keyThisWorld);
		}

		RegCloseKey(keyWorlds);
	}

	return hr;
}

//******************************************************
// This function iterates through the registry looking for the world with the path. 
HRESULT FindWorldByPath(BSTR bstrInPath, BSTR *pbstrGUIDOut)
{
	HRESULT	hr = S_OK;
	HKEY keyWorlds = NULL;
	DWORD dwStatus;

	if (pbstrGUIDOut == NULL)
		return E_POINTER;

	*pbstrGUIDOut = NULL;

	// 1. Open the WORLD key
	if (RegCreateKeyEx(HKEY_CURRENT_USER, gStrWorldsKey, 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &keyWorlds, &dwStatus) == ERROR_SUCCESS)
	{
		DWORD dwCurKey = 0;
		char curGUID[255];
		DWORD dwStrSize;
		LONG lResult;
		FILETIME fileTime;
		CComBSTR bstrGUID;
		
		// Iterate to populate the list
		do
		{
			CComBSTR bstrPath, bstrName;

			dwStrSize = 255;

			lResult = RegEnumKeyEx(keyWorlds, dwCurKey, curGUID, &dwStrSize, NULL, NULL, NULL, &fileTime);
			if (lResult != ERROR_SUCCESS)
				continue;

			bstrGUID = curGUID;

			//check if right path.
			hr = FindWorldInRegistry(bstrGUID, &bstrPath.m_str, &bstrName.m_str);
			if (FAILED(hr))
			{
				// if regkey is screwy, remove the world entry
				/* hr = */ RemoveWorldFromRegistry(bstrGUID);

				hr = S_OK;

				continue;
			}

			if (!CString(bstrPath).CompareNoCase(CString(bstrInPath)))
			{
				*pbstrGUIDOut = bstrGUID.Copy();

				//break out.
				lResult = !ERROR_SUCCESS;
			}		
			else
				dwCurKey++;
		}
		while (lResult == ERROR_SUCCESS);
	}

	if (keyWorlds) 
		RegCloseKey(keyWorlds);
	
	return hr;
}


//********************************************
// RegisterWorldOnline
// Given a world path and a BOOL, sets the 
// registry key for that world to indicate
// whether it is online or not.

HRESULT RegisterWorldOnline(BSTR bstrPath, BOOL bIsOnline)
{
	HRESULT	hr = S_OK;
	CString	strGUID;
	CComBSTR bstrGUID;
	HKEY keyWorlds;
	HKEY keyThisWorld;
	DWORD dwStatus;

	//Find GUID.
	hr = FindWorldByPath(bstrPath, &bstrGUID.m_str);
	if (FAILED(hr))
		return hr;

	if ((BSTR)bstrGUID == NULL) 
		return S_OK;
	else
		strGUID = bstrGUID;

	// Open the WORLD key
	if (RegCreateKeyEx(HKEY_CURRENT_USER, gStrWorldsKey, 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &keyWorlds, &dwStatus) == ERROR_SUCCESS)
	{
		if (RegOpenKeyEx(keyWorlds, strGUID, 0, KEY_SET_VALUE, &keyThisWorld) == ERROR_SUCCESS)
		{
			LONG lResult;
			CString strValue;
			
			if (bIsOnline)
				strValue = "FALSE";
			else
				strValue = "TRUE";

			// we have the key - add the value...
			lResult = RegSetValueEx(keyThisWorld, "Offline", 0, REG_SZ, (const unsigned char *)(LPCTSTR)strValue, strValue.GetLength());
			if (lResult != ERROR_SUCCESS)
			{
				TRACE("RegisterWorldOnline: failed to set Offline value for World, GUID %s\n", strGUID);
				hr = VWOBJECT_E_INVALIDWORLDREGISTRY;
				goto ERROR_ENCOUNTERED;
			}
					
ERROR_ENCOUNTERED:
			// clean up
			RegCloseKey(keyThisWorld);
		}

		RegCloseKey(keyWorlds);
	}

	return hr;
}

//********************************************
// IsWorldRegisteredOffline
// Returns TRUE if the world is in online

HRESULT	IsWorldRegisteredOnline(BSTR bstrPath, BOOL *pbIsOnline)
{
	HRESULT	hr = S_OK;
	CString	strGUID;
	CComBSTR bstrGUID;
	HKEY	keyWorlds;
	HKEY	keyThisWorld;
	unsigned char buffer[256];
	DWORD	dwStatus;

	if (pbIsOnline == NULL)
		return E_POINTER;

	// By default, every world is online
	*pbIsOnline = TRUE;

	//Find GUID.
	hr = FindWorldByPath(bstrPath, &bstrGUID.m_str);
	if (FAILED(hr))
		return hr;

	if ((BSTR)bstrGUID == NULL) 
		return S_OK;
	else
		strGUID = bstrGUID;

	// Open the WORLD key
	if (RegCreateKeyEx(HKEY_CURRENT_USER, gStrWorldsKey, 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &keyWorlds, &dwStatus) == ERROR_SUCCESS)
	{
		if (RegOpenKeyEx(keyWorlds, strGUID, 0, KEY_ALL_ACCESS, &keyThisWorld) == ERROR_SUCCESS)
		{
			DWORD length = 256;
			DWORD type; 

			if (ERROR_SUCCESS == RegQueryValueEx(keyThisWorld, "Offline", 0, &type, buffer, &length))
			{
				if (CString(buffer) == "TRUE")
					*pbIsOnline = FALSE;			
			}

			// clean up
			RegCloseKey(keyThisWorld);
		}

		RegCloseKey(keyWorlds);
	}

	return hr;
}

//********************************************
// RegisterWorldInDS
// Given a world path, sets whether that world's
// regkey should mark it as being available for
// the directory service

HRESULT	RegisterWorldInDS(BSTR bstrPath, BSTR bstrInfoURL, BOOL bIsInDS)
{
	HRESULT	hr = S_OK;
	CString	strGUID;
	CComBSTR bstrGUID;
	HKEY keyWorlds;
	HKEY keyThisWorld;
	DWORD dwStatus;

	//Find GUID.
	hr = FindWorldByPath(bstrPath, &bstrGUID.m_str);
	if (FAILED(hr))
		return hr;

	if ((BSTR)bstrGUID == NULL) 
		return S_OK;
	else
		strGUID = bstrGUID;

	// Open the WORLD key
	if (RegCreateKeyEx(HKEY_CURRENT_USER, gStrWorldsKey, 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &keyWorlds, &dwStatus) == ERROR_SUCCESS)
	{
		if (RegOpenKeyEx(keyWorlds, strGUID, 0, KEY_SET_VALUE, &keyThisWorld) == ERROR_SUCCESS)
		{
			LONG	lResult;
			CString strValue;
			
			if (bIsInDS)
				strValue = "FALSE";
			else
				strValue = "TRUE";

			// we have the key - add the value...
			lResult = RegSetValueEx(keyThisWorld, "NotInDirectory", 0, REG_SZ, (const unsigned char *)(LPCTSTR)strValue, strValue.GetLength());
			if (lResult != ERROR_SUCCESS)
			{
				TRACE("RegisterWorldInDS: failed to set NotInDirectory value for World, GUID %s\n", strGUID);
				hr = VWOBJECT_E_INVALIDWORLDREGISTRY;
				goto ERROR_ENCOUNTERED;
			}
			
			if (bstrInfoURL)
			{
				strValue = bstrInfoURL;
				lResult = RegSetValueEx(keyThisWorld, "InfoURL", 0, REG_SZ, (const unsigned char *)(LPCTSTR)strValue, strValue.GetLength());
				if (lResult != ERROR_SUCCESS)
				{
					TRACE("RegisterWorldInDS: failed to set InfoURL value for World, GUID %s\n", strGUID);
					hr = VWOBJECT_E_INVALIDWORLDREGISTRY;
					goto ERROR_ENCOUNTERED;
				}
			}
					
ERROR_ENCOUNTERED:
			// clean up
			RegCloseKey(keyThisWorld);
		}

		RegCloseKey(keyWorlds);
	}

	return hr;
}

//********************************************
// IsWorldRegisteredInDS
// Returns TRUE if this world is registered in 
// the directory service

HRESULT	IsWorldRegisteredInDS(BSTR bstrPath, BOOL *pbIsInDS)
{
	HRESULT	hr = S_OK;
	CString	strGUID;
	CComBSTR bstrGUID;
	HKEY	keyWorlds;
	HKEY	keyThisWorld;
	unsigned char buffer[256];
	DWORD	dwStatus;

	memset(&buffer,0,256);

	// By default, every world is in the directory
	*pbIsInDS = TRUE;

	hr = FindWorldByPath(bstrPath, &bstrGUID.m_str);
	if (FAILED(hr))
		return hr;

	if ((BSTR)bstrGUID == NULL) 
		return S_OK;
	else
		strGUID = bstrGUID;

	// Open the WORLD key
	if (RegCreateKeyEx(HKEY_CURRENT_USER, gStrWorldsKey, 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &keyWorlds, &dwStatus) == ERROR_SUCCESS)
	{
		if (RegOpenKeyEx(keyWorlds, strGUID, 0, KEY_ALL_ACCESS, &keyThisWorld) == ERROR_SUCCESS)
		{
			DWORD length = 256;
			DWORD type; 

			if (ERROR_SUCCESS == RegQueryValueEx(keyThisWorld, "NotInDirectory", 0, &type, buffer, &length))
			{
				if (CString(buffer)=="TRUE")
					*pbIsInDS = FALSE;			
			}

			// clean up
			RegCloseKey(keyThisWorld);
		}

		RegCloseKey(keyWorlds);
	}

	return hr;
}


//********************************************
// GetWorldInfoUrlFromRegistry
// Returns the WorldInfoURL from the registry

HRESULT	GetWorldInfoUrlFromRegistry(BSTR bstrPath, BSTR *pbstrInfoURL)
{
	HRESULT	hr = S_OK;
	CString	strGUID, strInfoURL;
	CComBSTR bstrGUID;
	HKEY	keyWorlds;
	HKEY	keyThisWorld;
	unsigned char buffer[256];
	DWORD	dwStatus;

	memset(&buffer,0,256);

	//Find GUID.
	*pbstrInfoURL = NULL;

	hr = FindWorldByPath(bstrPath, &bstrGUID.m_str);
	if (FAILED(hr))
		return hr;

	if ((BSTR)bstrGUID == NULL) 
		return S_OK;
	else
		strGUID = bstrGUID;

	// Open the WORLD key
	if (RegCreateKeyEx(HKEY_CURRENT_USER, gStrWorldsKey, 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &keyWorlds, &dwStatus) == ERROR_SUCCESS)
	{
		if (RegOpenKeyEx(keyWorlds, strGUID, 0, KEY_ALL_ACCESS, &keyThisWorld) == ERROR_SUCCESS)
		{
			DWORD length = 256;
			DWORD type; 

			if (ERROR_SUCCESS == RegQueryValueEx(keyThisWorld, "InfoURL", 0, &type, buffer, &length))
			{
				strInfoURL = (LPCTSTR)buffer;

				*pbstrInfoURL = strInfoURL.AllocSysString();
			}
			// clean up
			RegCloseKey(keyThisWorld);
		}

		RegCloseKey(keyWorlds);
	}

	return hr;
}


//***************************************************************
// GetRegLocalGroupName
// Get name of optional local group for user validation on server
HRESULT GetRegLocalGroupName(BSTR* pbstrLocalGroup)
{
	HRESULT	hr = S_OK;
	CString	strLocalGroup;
	HKEY	keyWorlds;
	unsigned char buffer[256];

	if (pbstrLocalGroup == NULL) 
		 return E_POINTER;

	memset(&buffer,0,256);

	// Open the WORLD key
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, szAppKey, 0, KEY_ALL_ACCESS, &keyWorlds) == ERROR_SUCCESS)
	{
		DWORD length = 256;
		DWORD type = 0; 

		if (ERROR_SUCCESS == RegQueryValueEx(keyWorlds, "LocalGroup", 0, &type, buffer, &length))
		{
			if (type == REG_SZ)
			{
				strLocalGroup = (LPCTSTR)buffer;
				strLocalGroup.SetSysString(pbstrLocalGroup);
			}
		}
		// clean up
		RegCloseKey(keyWorlds);
	}

	return hr;
}

//***************************************************************
// SetRegLocalGroupName
// Set name of optional local group for user validation on server
HRESULT SetRegLocalGroupName(BSTR bstrLocalGroup)
{
	HRESULT	hr = S_OK;
	CString	strLocalGroup;
	HKEY	keyWorlds;

	strLocalGroup = bstrLocalGroup;
	
	// Open the WORLD key
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, szAppKey, 0, KEY_ALL_ACCESS, &keyWorlds) == ERROR_SUCCESS)
	{
		int length = strLocalGroup.GetLength()+1;
		char *pbuffer = strLocalGroup.GetBuffer(length);
		hr = RegSetValueEx(keyWorlds, "LocalGroup", 0,REG_SZ, (byte *)pbuffer, length);

		// clean up
		RegCloseKey(keyWorlds);
	}
	return hr;
}

//************************************************
// GetRegAuthentication
// Key for server to turn on/off NT Authentication
HRESULT GetRegAuthentication(BOOL *pbAuth)
{
	HRESULT	hr = S_OK;
	CString	strAuth;
	HKEY	keyWorlds;
	unsigned char buffer[256];

	if (pbAuth == NULL) 
		 return E_POINTER;

	*pbAuth=TRUE;

	memset(&buffer,0,256);

	// Open the WORLD key
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, szAppKey, 0, KEY_ALL_ACCESS, &keyWorlds) == ERROR_SUCCESS)
	{
		DWORD length = 256;
		DWORD type = 0; 

		if (ERROR_SUCCESS == RegQueryValueEx(keyWorlds, "Authentication", 0, &type, buffer, &length))
		{
			if (type == REG_SZ)
			{
				strAuth = (LPCTSTR)buffer;
				if (strAuth=="0")
					*pbAuth=FALSE;
			}
		}
		// clean up
		RegCloseKey(keyWorlds);
	}

	return hr;
}

//************************************************
// SetRegAuthentication
// Key for server to turn on/off NT Authentication
HRESULT SetRegAuthentication(BOOL bAuth)
{
	HRESULT	hr = S_OK;
	CString	strUseAuth = "0";
	HKEY	keyWorlds;

	if (bAuth)
		strUseAuth="1";
		
	// Open the WORLD key
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, szAppKey, 0, KEY_ALL_ACCESS, &keyWorlds) == ERROR_SUCCESS)
	{
		int length = strUseAuth.GetLength()+1;
		char *pbuffer = strUseAuth.GetBuffer(length);

		hr = RegSetValueEx(keyWorlds, "Authentication", 0,REG_SZ, (byte *)pbuffer, length);

		// clean up
		RegCloseKey(keyWorlds);
	}
	return hr;
}


//******************************************
// SetRegConnectionLimit
// Key for server to set optional user limit

HRESULT SetRegConnectionLimit(DWORD dwMax)
{
	HRESULT	hr = S_OK;
	DWORD	dwConnectionLimit = 0;
	HKEY	keyWorlds;

	// Open the WORLD key
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, szAppKey, 0, KEY_ALL_ACCESS, &keyWorlds) == ERROR_SUCCESS)
	{
		hr = RegSetValueEx(keyWorlds, "MaxUsers", 0,REG_DWORD, (byte *)&dwMax, 4);

		// clean up
		RegCloseKey(keyWorlds);
	}
	return hr;
}


//*******************************************
// GetRegConnectionLimit
// Key for server to get optional user limit

HRESULT GetRegConnectionLimit(DWORD *pdwMax)
{
	HRESULT	hr = S_OK;
	DWORD	dwConnectionLimit = 0;
	HKEY	keyWorlds;

	if (pdwMax == NULL) 
		 return E_POINTER;
		
	// Open the WORLD key
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, szAppKey, 0, KEY_ALL_ACCESS, &keyWorlds) == ERROR_SUCCESS)
	{
		DWORD dwtype = 0; 
		DWORD dwcblength = 4;
		if (ERROR_SUCCESS == RegQueryValueEx(keyWorlds, "MaxUsers", 0, &dwtype, (LPBYTE) pdwMax, &dwcblength))
		{
			if (!(dwtype == REG_DWORD))
			{
				//something is wrong.. just default to no limit
					*pdwMax=0;
				//	hr=E_FAIL;  // no need to fail here.. gobble up the hr
			}
		}

		// clean up
		RegCloseKey(keyWorlds);
	}
	return hr;
}