// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// helper functions...

#include "vwobject.h"

#define szAppKey			"SOFTWARE\\Microsoft\\V-Worlds"
#define szAppInstallValue	"InstallPath"

HRESULT GetDirectoryServiceURL(BSTR *pbstrURL);
HRESULT SetDirectoryServiceURL(BSTR bstrURL);
HRESULT	FindAvatarInRegistry(BSTR bstrAvatarAlias, BSTR *pbstrPath);
HRESULT	FindAvatarPath(BSTR *pbstrPath);
HRESULT	FindAvatarGraphicsPath(BSTR *pbstrPath);
HRESULT	FindContentPath(BSTR *pbstrPath);
HRESULT	FindDirectoryServicePage(BSTR *pbstrURL);
HRESULT	FindHelpPath(BSTR *pbstrPath);
HRESULT	FindWorldWizardPath(BSTR *pbstrPath);
HRESULT	FindWorldPath(BSTR *pbstrPath);
HRESULT GetIsSDK(VARIANT_BOOL *vBool);


// Only compile these in VWSYSTEM (rk, 11.12.98)
#ifdef VWSYSTEM_BUILD
HRESULT ComposeRootURLPath(IWorld *pWorld, CComBSTR *bstrRootURLPath);
HRESULT SetWorldContentPath(IWorld *pWorld, CComBSTR bstrNewWorldContentPath);
HRESULT SetWorldAvatarGraphicsPath(IWorld *pWorld, CComBSTR bstrNewWorldAvatarGraphicsPath);
HRESULT SetRegLocalGroupName(BSTR bstrLocalGroup);
HRESULT GetRegLocalGroupName(BSTR *pbstrLocalGroup);
HRESULT SetRegAuthentication(BOOL bAuth);
HRESULT GetRegAuthentication(BOOL *pbAuth);
HRESULT SetRegConnectionLimit(DWORD dwMax);
HRESULT GetRegConnectionLimit(DWORD *pdwMax);

#endif
HRESULT	AddAvatarToRegistry(BSTR bstrAlias, BSTR bstrPath);
HRESULT	RemoveAvatarFromRegistry(BSTR bstrAlias, BSTR bstrPath);
HRESULT CanonDatabaseName(BSTR bstrIn, BSTR *pbstrOut);

//world registration fcns.
HRESULT FindWorldInRegistry(BSTR bstrGUID, BSTR *pbstrPath, BSTR *pbstrName);
HRESULT AddWorldToRegistry(BSTR bstrGUID, BSTR bstrPath, BSTR bstrName);

HRESULT RemoveWorldFromRegistry(BSTR bstrGUID);
HRESULT FindAndPurgeWorldsByPath(BSTR bstrInPath);

//World offline features.
HRESULT FindWorldByPath(BSTR bstrInPath, BSTR *bstrGUIDOut);

HRESULT RegisterWorldOnline(BSTR bstrPath, BOOL bIsOnline);
HRESULT	IsWorldRegisteredOnline(BSTR bstrPath, BOOL * pbIsOnline);

HRESULT	RegisterWorldInDS(BSTR bstrPath, BSTR bstrInfoURL, BOOL bIsInDS);
HRESULT	IsWorldRegisteredInDS(BSTR bstrPath, BOOL *pbIsInDS);
HRESULT GetWorldInfoUrlFromRegistry(BSTR bstrPath, BSTR *pbstrInfoURL);


extern CString gStrAvatarKey;
extern CString gStrPathKey;
extern CString gStrToolsKey;
extern CString gStrWorldsKey;
extern CString gStrSDKKey;