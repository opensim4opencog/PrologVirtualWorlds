// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// AvListCt.cpp : Implementation of the CAvatarListCtrl ActiveX Control class.

#include "stdafx.h"
#include <propbase.h>
#include <vwclient.h>
#include "uictrl.h"
#include "AvListCt.h"
#include "reghelp.h"
#include <afxtempl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CAvatarListCtrl, CVWViewCtrl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CAvatarListCtrl, CVWViewCtrl)
	//{{AFX_MSG_MAP(CAvatarListCtrl)
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)	
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CAvatarListCtrl, CVWViewCtrl)
	//{{AFX_DISPATCH_MAP(CAvatarListCtrl)
	DISP_PROPERTY_EX(CAvatarListCtrl, "VWClient", GetVWClient, SetVWClient, VT_DISPATCH)
	DISP_PROPERTY_EX(CAvatarListCtrl, "currentUser", GetCurrentUser, SetCurrentUser, VT_DISPATCH)
	DISP_PROPERTY_EX(CAvatarListCtrl, "CostumeList", GetCostumeList, SetCostumeList, VT_VARIANT)
	DISP_PROPERTY_EX(CAvatarListCtrl, "AvatarList", GetAvatarList, SetAvatarList, VT_VARIANT)
	DISP_FUNCTION(CAvatarListCtrl, "CreateNewAvatar", CreateNewAvatar, VT_BOOL, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CAvatarListCtrl, "ConnectAvatar", ConnectAvatar, VT_BOOL, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CAvatarListCtrl, "DisconnectAvatar", DisconnectAvatar, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CAvatarListCtrl, "DeleteAvatar", DeleteAvatar, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CAvatarListCtrl, "DeleteDeadAvatar", DeleteDeadAvatar, VT_BOOL, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CAvatarListCtrl, CVWViewCtrl)
	//{{AFX_EVENT_MAP(CAvatarListCtrl)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CAvatarListCtrl, 2)
	PROPPAGEID( CLSID_CFontPropPage )
    PROPPAGEID( CLSID_CColorPropPage )
END_PROPPAGEIDS(CAvatarListCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CAvatarListCtrl, "VWCLIENTUI.AvatarListCtrl.1",
	0xaf0ad5cc, 0x418b, 0x11d1, 0xbb, 0x2b, 0, 0xc0, 0x4f, 0xc2, 0xca, 0xb1)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CAvatarListCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DAvatarList =
		{ 0xaf0ad5ca, 0x418b, 0x11d1, { 0xbb, 0x2b, 0, 0xc0, 0x4f, 0xc2, 0xca, 0xb1 } };
const IID BASED_CODE IID_DAvatarListEvents =
		{ 0xaf0ad5cb, 0x418b, 0x11d1, { 0xbb, 0x2b, 0, 0xc0, 0x4f, 0xc2, 0xca, 0xb1 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwAvatarListOleMisc =
	OLEMISC_INVISIBLEATRUNTIME |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CAvatarListCtrl, IDS_AVATARLIST, _dwAvatarListOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CAvatarListCtrl::CAvatarListCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CAvatarListCtrl

BOOL CAvatarListCtrl::CAvatarListCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
	{
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_AVATARLIST,
			IDB_AVATARLIST,
			afxRegApartmentThreading,
			_dwAvatarListOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	}
	else
	{
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CAvatarListCtrl::CAvatarListCtrl - Constructor

CAvatarListCtrl::CAvatarListCtrl()
{
	InitializeIIDs(&IID_DAvatarList, &IID_DAvatarListEvents);

	m_pLocalWorld = NULL;
	m_bConnected = FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CAvatarListCtrl::~CAvatarListCtrl - Destructor

CAvatarListCtrl::~CAvatarListCtrl()
{
	if (m_bConnected)
		DisconnectAvatar();

	if (m_pLocalWorld)
	{
		m_pLocalWorld->Terminate();
		SAFERELEASE(m_pLocalWorld);
	}

	// REVIEW:  If we addref the client, un-addref it here....
}


/////////////////////////////////////////////////////////////////////////////
// CAvatarListCtrl::OnDraw - Drawing function

void CAvatarListCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
//	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
}


/////////////////////////////////////////////////////////////////////////////
// CAvatarListCtrl::DoPropExchange - Persistence support

void CAvatarListCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	CVWViewCtrl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CAvatarListCtrl::OnResetState - Reset control to default state

void CAvatarListCtrl::OnResetState()
{
	CVWViewCtrl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



//******************************************************
// CreateLocalWorld
// Creates a local world, complete the core exemplars.

IWorld* CreateLocalWorld(IVWClient *pClient)
{
	HRESULT hr = S_OK;
	IWorld* pWorld = NULL;
	static CComBSTR bstrTempWorld("~TemporaryWorldForCreatingAvatars");

	// REVIEW: use a hard wired name for now.  Will get overwritten every time.
	hr = pClient->ConnectLocal(bstrTempWorld, &pWorld);
	if (SUCCEEDED(hr) && pWorld)
	{
		// create mm exemplars
//		hr = pWorld->CreateCOMModule(CComBSTR("Multimedia"), CComBSTR("VWSYSTEM.MultimediaEx.1"), MODULE_CLIENT | MODULE_SERVER, &pModule);
	}
	else if (hr == VWCLIENT_E_ALREADYCONNECTED)
	{
		hr = pClient->get_World(&pWorld);
		if (SUCCEEDED(hr) && pWorld)
		{
			pWorld->Terminate();
			pClient->Disconnect();
			pWorld = CreateLocalWorld(pClient);
		}
	}

	return pWorld;
}

//******************************************************
// CreateNewAvatar
// Creates a new Avatar by invoking the Avatar Wizard

extern CUictrlApp theApp;

const long	kLenMinAvatarName	= 2;
const long	kLenMaxAvatarName	= 24;
const char	kstrAvatarNameIllegalChars[] = "& ";


//******************************************************
// _VerifyAvatarName
// Returns TRUE if the name is a valid avatar name,
// FALSE otherwise.

BOOL _VerifyAvatarName(CString &strAvatarName)
{
	BOOL	bResult = FALSE;

	if (strAvatarName)
	{
		long	len = strlen(strAvatarName);

		if ((len >= kLenMinAvatarName) && (len <= kLenMaxAvatarName))
		{
			if (strAvatarName.FindOneOf(kstrAvatarNameIllegalChars) == -1)
			{
				// more tests here
				bResult = TRUE;
			}
		}
	}

	return bResult;
}


//******************************************************
// CreateNewAvatar
// Attemps to create a new avatar.  Returns TRUE if
// successful, false otherwise.

BOOL CAvatarListCtrl::CreateNewAvatar(LPCTSTR strAvatarName, LPCTSTR strAvatarPassword) 
{
	ASSERT(m_pVWClient);
	CComBSTR	bstrPswd	= strAvatarPassword;
	CComBSTR	bstrName	= strAvatarName;
	CComBSTR	bstrPath;
	HCURSOR		oldCursor;
	HRESULT		hr;
	
	if (!_VerifyAvatarName((CString)strAvatarName))
		return FALSE;

	hr = FindAvatarInRegistry(bstrName, &bstrPath.m_str);
	if (SUCCEEDED(hr))	// the avatar already exists...
		return FALSE;
	
	oldCursor	= SetCursor(theApp.LoadStandardCursor(IDC_WAIT));
	
	// log off current user, if any
	if(m_bConnected)
		DisconnectAvatar();

	// 1. Create a world (make the client and connect to it)
	if (!m_pVWClient)
		return FALSE;
	
	if (!m_pLocalWorld)
		m_pLocalWorld = CreateLocalWorld(m_pVWClient);
	else
	{
		// we may have been disconnected
		IWorld	*pClientWorld;

		hr = m_pVWClient->get_World(&pClientWorld);
		if (FAILED(hr))
			return FALSE;
		else if (!pClientWorld)
		{
			if (m_pLocalWorld)
			{
				m_pLocalWorld->Terminate();
				SAFERELEASE(m_pLocalWorld);
			}

			m_pLocalWorld = CreateLocalWorld(m_pVWClient);
		}
	}

	if (m_pLocalWorld)
	{
		IThing	*pUser	= NULL;

		// 2. Connect as new
		hr = m_pLocalWorld->Connect(bstrName, bstrPswd, &pUser);
		
		if (SUCCEEDED(hr) && pUser)
		{
			SAFERELEASE(pUser);
			m_pLocalWorld->Disconnect();
			return TRUE;
		}
	}
	SetCursor(oldCursor);
	return FALSE;
}

//******************************************************
// GetVWClient
// Gets and Sets the VWClient property

LPDISPATCH CAvatarListCtrl::GetVWClient() 
{
	return CVWViewCtrl::GetVWClient();
}

void CAvatarListCtrl::SetVWClient(LPDISPATCH newValue) 
{
	CVWViewCtrl::SetVWClient(newValue);
	SetModifiedFlag();
}

//******************************************************
// ConnectAvatar
// Connects to a local world as a particular avatar...

BOOL CAvatarListCtrl::ConnectAvatar(LPCTSTR newName, LPCTSTR strAvatarPassword) 
{
	IThing		*pUser		= NULL;
	CString		strNewName	= newName;
	CComBSTR	bstrNewName	= newName;
	CComBSTR	bstrPswd	= ""; //strAvatarPassword;
	HRESULT		hr;

	ASSERT(m_pVWClient);
	
	if (m_bConnected)
	{
		hr = m_pLocalWorld->get_User(&pUser);
		
		if (SUCCEEDED(hr) && pUser)
		{
			CComBSTR	bstrUserName;

			hr = pUser->get_Name(&bstrUserName.m_str);
			
			SAFERELEASE(pUser);
			if (SUCCEEDED(hr))
			{
				CString	strCurName = bstrUserName;

				if (!strCurName.CompareNoCase(strNewName))
					return TRUE;
			}
		}
		DisconnectAvatar();
	}

	if (!m_pLocalWorld)
		m_pLocalWorld = CreateLocalWorld(m_pVWClient);
	else
	{
		// we may have been disconnected
		IWorld	*pClientWorld;

		hr = m_pVWClient->get_World(&pClientWorld);
		if (FAILED(hr))
			return FALSE;
		else if (m_pLocalWorld != pClientWorld)
		{
			if (m_pLocalWorld)
			{
				m_pLocalWorld->Terminate();
				SAFERELEASE(m_pLocalWorld);
			}
			m_pLocalWorld = CreateLocalWorld(m_pVWClient);
		}
		SAFERELEASE(pClientWorld);
	}

	if (!m_pLocalWorld)
		return FALSE;
	else
	{
		IThing	*pCurUser = NULL;

		hr = m_pLocalWorld->get_User(&pCurUser);
		if (SUCCEEDED(hr) && (pCurUser != NULL))
		{
			// must already be connected to a world...
			m_pLocalWorld->Disconnect();
			SAFERELEASE(pCurUser);
		}

		hr = m_pLocalWorld->Connect(bstrNewName, bstrPswd, &pUser);
		
		if (SUCCEEDED(hr) && pUser)
		{
			m_bConnected = TRUE;
			SAFERELEASE(pUser);
			return TRUE;
		}
		else
			return FALSE;
	}
}

//******************************************************
// DisconnectAvatar
// disconnects the avatar from the local world

void CAvatarListCtrl::DisconnectAvatar() 
{
	if (m_bConnected)
	{
		ASSERT(m_pLocalWorld);
		m_pLocalWorld->Disconnect();
		m_bConnected = FALSE;
	}
}

//******************************************************
// GetCurrentUser
// Returns the current locally connected user

LPDISPATCH CAvatarListCtrl::GetCurrentUser() 
{
	if (m_bConnected)
	{
		ASSERT(m_pLocalWorld);
		IThing	*pUser = NULL;
		HRESULT	hr;

		hr = m_pLocalWorld->get_User(&pUser);

		if (SUCCEEDED(hr))
		{
			// REVIEW: should we release the IThing?
			return (LPDISPATCH)pUser;
		}
	}

	return NULL;
}

//******************************************************
// SetCurrentUser
// Sets the current user - this is actually illegal!

void CAvatarListCtrl::SetCurrentUser(LPDISPATCH) 
{
	// actually, the current user cannot be set!
	ASSERT(FALSE);
}

//******************************************************
// GetCostumeList
// Returns a SAFEARRAY of all of the locally
// defined avatar costumes....

VARIANT CAvatarListCtrl::GetCostumeList() 
{
	VARIANT		vaResult;
	SAFEARRAY	*pAvatarList = NULL;
	HRESULT		hr;
	long		cAvatarNames;
	CList<CString, CString>		avNames;
	CFileFind	finder;
	CComBSTR	bstrPath;
	CString		strFilePath;
	
	FindAvatarGraphicsPath(&bstrPath.m_str);
	strFilePath = bstrPath;
	strFilePath += "*.spr";

	// find the files....
	BOOL	bWorking = finder.FindFile(strFilePath);

    while (bWorking)    
	{        
		bWorking = finder.FindNextFile();
        avNames.AddTail(finder.GetFilePath());
	}
	
	cAvatarNames = avNames.GetCount();

	VariantInit(&vaResult);
	pAvatarList = SafeArrayCreateVector(VT_VARIANT, 0, cAvatarNames);
	
	if (pAvatarList)
	{
		POSITION	pos;
		CString		curName;
		VARIANT		var;

		::VariantInit(&var);

		pos = avNames.GetHeadPosition();
		// we have an array - fill it in!
		for (long i = 0; i < cAvatarNames; i++)
		{
			curName = avNames.GetNext(pos);
			bstrPath = curName;

			// doesn't allocate data
			var.vt = VT_BSTR;
			var.bstrVal = (BSTR)bstrPath;

			hr = SafeArrayPutElement(pAvatarList, &i, &var);
			if (FAILED(hr))
				goto ERROR_OUT; 
		}

		// now, convert it to a VARIANT
		vaResult.vt		= VT_VARIANT | VT_ARRAY;
		vaResult.parray	= pAvatarList;

		return vaResult;
	}

ERROR_OUT:
	if (pAvatarList)
        SafeArrayDestroy(pAvatarList);


	return vaResult;
}

void CAvatarListCtrl::SetCostumeList(const VARIANT FAR& theVar) 
{
	// actually, the costume list cannot be set!
	ASSERT(FALSE);
}

//******************************************************
// GetAvatarList
// Returns a list of the avatars currently on the disk..

VARIANT CAvatarListCtrl::GetAvatarList() 
{
	VARIANT		vaResult;
	SAFEARRAY	*pAvatarList = NULL;
	HRESULT		hr;
	long		cAvatarNames;
	HKEY		keyAvatars;
	DWORD		dwStatus;
	DWORD		dwAvCount=0;
	CComBSTR	bstrName;
	CList<CString, CString>		avNames;
	
	// First, get the avatar names
	if (RegCreateKeyEx(HKEY_CURRENT_USER, gStrAvatarKey, 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &keyAvatars, &dwStatus)
		== ERROR_SUCCESS)
	{
		LONG		lResult	= ERROR_SUCCESS;
		char		strKeyName[64];
		DWORD		dwNameLen;
		FILETIME	ftKeyTime;
	
		// now, enumerate them...
		while (lResult == ERROR_SUCCESS)
		{
			dwNameLen = 64;
			lResult =  RegEnumKeyEx(keyAvatars, dwAvCount, strKeyName, &dwNameLen, NULL, NULL, NULL, &ftKeyTime);
			if (lResult == ERROR_SUCCESS)
			{
				avNames.AddTail(strKeyName);
			}
			dwAvCount++;
		}

		RegCloseKey(keyAvatars);
	}
	
	// now, turn it into a SAFEARRAY
	cAvatarNames = avNames.GetCount();

	VariantInit(&vaResult);

	pAvatarList = SafeArrayCreateVector(VT_VARIANT, 0, cAvatarNames);
	
	if (pAvatarList)
	{
		POSITION	pos;
		CString		curName;
		VARIANT		var;
		
		::VariantInit(&var);

		pos = avNames.GetHeadPosition();
		// we have an array - fill it in!
		for (long i = 0; i < cAvatarNames; i++)
		{
			curName = avNames.GetNext(pos);
			bstrName = curName;

			// doesn't allocate data
			var.vt = VT_BSTR;
			var.bstrVal = (BSTR)bstrName;

			hr = SafeArrayPutElement(pAvatarList, &i, &var);
			if (FAILED(hr))
				goto ERROR_OUT; 
		}

		// now, convert it to a VARIANT
		vaResult.vt		= VT_VARIANT | VT_ARRAY;
		vaResult.parray	= pAvatarList;

		return vaResult;
	}

ERROR_OUT:
	if (pAvatarList)
        SafeArrayDestroy(pAvatarList);

	return vaResult;
}

void CAvatarListCtrl::SetAvatarList(const VARIANT FAR& newValue) 
{
	// Actually, we cannot set this..
	ASSERT(FALSE);
}

//******************************************************
// ShowErrDialog
// Brings up a message box with the error in it

void	ShowErrDialog(DWORD	dwErr)
{
	LPVOID	lpMsgBuf;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL, dwErr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf, 0, NULL );

	::MessageBox( NULL, (LPTSTR)lpMsgBuf, "Error!", MB_OK|MB_ICONINFORMATION );
	// Free the buffer.
	LocalFree( lpMsgBuf );
}
//******************************************************
// DeleteAvatar
// Deletes the currently selected avatar
// 1.  Delete the disk file
// 2.  Delete the registry key
// 3.  Reset the current avatar

BOOL CAvatarListCtrl::DeleteAvatar() 
{
	CComBSTR	bstrAvName;
	CComBSTR	bstrAvPath;
	CString		tempStr;
	IThing		*pUser = NULL;
	HKEY		keyAvatars;
	HRESULT		hr;
	DWORD		dwStatus;
	BOOL		bResult = FALSE;

	ASSERT(m_pVWClient);

	if (!m_bConnected)
		goto CLEAN_UP;
	
	hr = m_pLocalWorld->get_User(&pUser);
	if (FAILED(hr) || (!pUser))
		goto CLEAN_UP;

	hr = pUser->get_Name(&bstrAvName.m_str);
	if (FAILED(hr))
		goto CLEAN_UP;
	
	DisconnectAvatar();
	// 1.  Get the file name & delete it
	hr = FindAvatarInRegistry(bstrAvName, &bstrAvPath.m_str);
	if (FAILED(hr))
		goto CLEAN_UP;

	tempStr = bstrAvPath;

	if (!DeleteFile(tempStr))
	{
		DWORD	dwErr = GetLastError();
		ShowErrDialog(dwErr);
		goto CLEAN_UP;
	}

	// 2.  Delete the key
	if (RegCreateKeyEx(HKEY_CURRENT_USER, gStrAvatarKey, 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &keyAvatars, &dwStatus) == ERROR_SUCCESS)
	{
		// delete the appropriate sub-key
		DWORD	dwErr;
		
		tempStr = bstrAvName;
		dwErr = RegDeleteKey(keyAvatars, tempStr);

		if (dwErr != ERROR_SUCCESS)
			ShowErrDialog(dwErr);

		// clean up
		RegCloseKey(keyAvatars);
		bResult = TRUE;
	}

CLEAN_UP:
	SAFERELEASE(pUser);
	return bResult;
}


//********************************************
// DeleteDeadAvatar
// Given the name of a bogus avatar, deletes it
// from the disk and the registry

BOOL CAvatarListCtrl::DeleteDeadAvatar(LPCTSTR strDeadName) 
{
	CComBSTR	bstrAvName;
	CComBSTR	bstrAvPath;
	CString		tempStr;
	IThing		*pUser = NULL;
	HKEY		keyAvatars;
	HRESULT		hr;
	DWORD		dwStatus;
	BOOL		bResult = FALSE;

	bstrAvName = strDeadName;

	// 1.  Get the file name & delete it
	hr = FindAvatarInRegistry(bstrAvName, &bstrAvPath.m_str);
	if (FAILED(hr))
		goto CLEAN_UP;

	tempStr = bstrAvPath;

	if (!DeleteFile(tempStr))
	{
		DWORD	dwErr = GetLastError();
		ShowErrDialog(dwErr);
		goto CLEAN_UP;
	}

	// 2.  Delete the key
	if (RegCreateKeyEx(HKEY_CURRENT_USER, gStrAvatarKey, 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &keyAvatars, &dwStatus) == ERROR_SUCCESS)
	{
		// delete the appropriate sub-key
		DWORD	dwErr;
		
		tempStr = bstrAvName;
		dwErr = RegDeleteKey(keyAvatars, tempStr);

		if (dwErr != ERROR_SUCCESS)
			ShowErrDialog(dwErr);

		// clean up
		RegCloseKey(keyAvatars);
		bResult = TRUE;
	}

CLEAN_UP:
	return bResult;
}
