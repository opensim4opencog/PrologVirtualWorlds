// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// authobj.cpp : Implementation of CAuthenticate
#include "stdafx.h"
#include "Authntic.h"
#include "authobj.h"
#include "logon.h"

/////////////////////////////////////////////////////////////////////////////
// CAuthenticate

CAuthenticate::CAuthenticate() :
	m_hSecurity(NULL),
	m_hNetApi(NULL),
	m_hAdvApi(NULL),
	m_pfnWUGI(NULL),
	m_pfnABF(NULL),
	m_pfnLGGM(NULL),
	m_pfnNGGU(NULL),
	m_psft(NULL),
	m_cbTokenMax(0),
	m_bOnNT(FALSE),
	m_pfnOTT(NULL),
	m_pfnEQSID(NULL),
	m_pfnGTI(NULL),
	m_pfnAISID(NULL),
	m_pfnFSID(NULL)
{
}

CAuthenticate::~CAuthenticate()
{
	if (m_hSecurity)
		::FreeLibrary(m_hSecurity);

	if (m_hNetApi)
		::FreeLibrary(m_hNetApi);

	if (m_hAdvApi)
		::FreeLibrary(m_hAdvApi);

}

STDMETHODIMP CAuthenticate::GenerateClientContext(IAuthentication * pauth, VARIANT_BOOL bShowDialog, VARIANT_BOOL * pbDone)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bInitialized;
	SECURITY_STATUS ss;
	CredHandle hcred;
	CtxtHandle hctxt;
	TimeStamp ts;
	SecBufferDesc sbdOut;
	SecBuffer sbOut;
	SecBufferDesc sbdIn;
	SecBuffer sbIn;
	ULONG ulCtxtAttr;
	int cbBlob;
	BYTE *pbBlob=NULL;
	char *pchToken=NULL;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pauth==NULL || pbDone==NULL)
		return E_POINTER;

	// verify that we're initialized
	if (m_psft==NULL)
		return E_FAIL;

	// initialize
	*pbDone = VARIANT_FALSE;

	// if this is a new authentication, get the initial credentials
	hr = pauth->get_Initialized(&bInitialized);
	if (FAILED(hr))
		goto CLEAN_UP;

	if (bInitialized!=VARIANT_TRUE)
	{
		// get the security package name first
		CString str = m_bstrPackage;
		char *psz = str.GetBuffer(_MAX_PATH);

		// get any authentication data that we might need
		void *pvAuthData;
		if (bShowDialog == VARIANT_TRUE)
			hr = GetAuthenticationData(pauth, &pvAuthData);

		if (FAILED(hr))
			goto CLEAN_UP;

		ss = m_psft->AcquireCredentialsHandle(	NULL,
												psz,
												SECPKG_CRED_OUTBOUND,
												NULL,
												(bShowDialog==VARIANT_TRUE) ? pvAuthData : NULL, // NULL,//pvAuthData,
												NULL,
												NULL,
												&hcred,
												&ts);
		if (FAILED(ss))
		{
			hr = (HRESULT) ss;
			goto CLEAN_UP;
		}

		// stow the credentials in the authentication
		hr = pauth->put_Credentials(*((SECHANDLE *) &hcred));
		if (FAILED(hr))
			goto CLEAN_UP;
	}
	else
	{
		// get the credentials
		hr = pauth->get_Credentials((SECHANDLE *) &hcred);
		if (FAILED(hr))
			goto CLEAN_UP;

		// and the context
		hr = pauth->get_Context((SECHANDLE *) &hctxt);
		if (FAILED(hr))
			goto CLEAN_UP;
	}

	// output
	pchToken = new char[m_cbTokenMax];
	if (pchToken==NULL)
	{
		hr = E_OUTOFMEMORY;
		goto CLEAN_UP;
	}
	sbdOut.ulVersion = 0;
	sbdOut.cBuffers = 1;
	sbdOut.pBuffers = &sbOut;
	sbOut.cbBuffer = m_cbTokenMax;
	sbOut.BufferType = SECBUFFER_TOKEN;
	sbOut.pvBuffer = pchToken;

	// input (if needed)
	if (bInitialized==VARIANT_TRUE)
	{
		// get the incoming blob
		hr = pauth->GetBlobLength(&cbBlob);
		if (cbBlob>0)
		{
			pbBlob = new BYTE[cbBlob];
			if (pbBlob==NULL)
			{
				hr = E_OUTOFMEMORY;
				goto CLEAN_UP;
			}
			hr = pauth->CopyBlob(pbBlob);
			if (FAILED(hr))
				goto CLEAN_UP;
		}
		else
			pbBlob = NULL;

		sbdIn.ulVersion = 0;
		sbdIn.cBuffers = 1;
		sbdIn.pBuffers = &sbIn;

		// get information out of the blob
		sbIn.cbBuffer = cbBlob;
		sbIn.BufferType = SECBUFFER_TOKEN;
		sbIn.pvBuffer = pbBlob;
	}

	ss = m_psft->InitializeSecurityContext(	&hcred,
											(bInitialized==VARIANT_TRUE) ? &hctxt : NULL,
											"V-Worlds Authentication",
											0,
											0,
											SECURITY_NATIVE_DREP,
											(bInitialized==VARIANT_TRUE) ? &sbdIn : NULL,
											0,
											&hctxt,
											&sbdOut,
											&ulCtxtAttr,
											&ts );

	if (FAILED(ss))
	{
		hr = (HRESULT) ss;
		goto CLEAN_UP;
	}

	// Complete token -- if applicable
	//
	if (ss==SEC_I_COMPLETE_NEEDED || ss==SEC_I_COMPLETE_AND_CONTINUE)
	{
		if (m_psft->CompleteAuthToken!=NULL)
		{
			ss = m_psft->CompleteAuthToken(&hctxt, &sbdOut);
			if (FAILED(ss))
			{
				hr = (HRESULT) ss;
				goto CLEAN_UP;
			}
		}
		else
		{
			hr = E_FAIL;
			goto CLEAN_UP;
		}
	}

	// stick the context into the authentication
	hr = pauth->put_Context(*((SECHANDLE *) &hctxt));
	if (FAILED(hr))
		goto CLEAN_UP;

	// stick the output buffer into the blob
	hr = pauth->SetBlob((BYTE *) pchToken, (int) sbOut.cbBuffer);
	if (FAILED(hr))
		goto CLEAN_UP;

	if (ss!=SEC_I_CONTINUE_NEEDED && ss!=SEC_I_COMPLETE_AND_CONTINUE)
	{
		// set the flag in the authentication, too
		hr = pauth->put_Authenticated(VARIANT_TRUE);
		if (FAILED(hr))
			goto CLEAN_UP;

		*pbDone = VARIANT_TRUE;
	}

CLEAN_UP:

	if (pchToken)
		delete [] pchToken;
	if (pbBlob)
		delete [] pbBlob;

	return hr;

}

STDMETHODIMP CAuthenticate::GenerateServerContext(IAuthentication * pauth, VARIANT_BOOL * pbDone)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bInitialized;
	SECURITY_STATUS ss;
	CredHandle hcred;
	CtxtHandle hctxt;
	TimeStamp ts;
	SecBufferDesc sbdOut;
	SecBuffer sbOut;
	SecBufferDesc sbdIn;
	SecBuffer sbIn;
	ULONG ulCtxtAttr;
	BYTE *pbBlob = NULL;
	char *pchToken = NULL;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pauth==NULL || pbDone==NULL)
		return E_POINTER;

#ifndef REMOVEWIN95SERVERHACK

	if (!m_bOnNT)
	{
		// force authentication to true
		// set the flag in the authentication, too
		hr = pauth->put_Initialized(VARIANT_TRUE);
		if (FAILED(hr))
			goto CLEAN_UP;

		hr = pauth->put_Authenticated(VARIANT_TRUE);
		if (FAILED(hr))
			goto CLEAN_UP;

		*pbDone = VARIANT_TRUE;
		goto CLEAN_UP;
	}


#endif

	// verify that we're initialized
	if (m_psft==NULL)
		return E_FAIL;

	// initialize
	*pbDone = VARIANT_FALSE;

	// if this is a new authentication, get the initial credentials
	hr = pauth->get_Initialized(&bInitialized);
	if (FAILED(hr))
		goto CLEAN_UP;

	if (bInitialized!=VARIANT_TRUE)
	{
		// get the security package name first
		CString str = m_bstrPackage;
		char *psz = str.GetBuffer(_MAX_PATH);

		ss = m_psft->AcquireCredentialsHandle(	NULL,
												psz,
												SECPKG_CRED_INBOUND,
												NULL,
												NULL,
												NULL,
												NULL,
												&hcred,
												&ts);
		if (FAILED(ss))
		{
			hr = (HRESULT) ss;
			goto CLEAN_UP;
		}

		// stow the credentials in the authentication
		hr = pauth->put_Credentials(*((SECHANDLE *) &hcred));
		if (FAILED(hr))
			goto CLEAN_UP;
	}
	else
	{
		// get the credentials
		hr = pauth->get_Credentials((SECHANDLE *) &hcred);
		if (FAILED(hr))
			goto CLEAN_UP;

		// and the content
		hr = pauth->get_Context((SECHANDLE *) &hctxt);
		if (FAILED(hr))
			goto CLEAN_UP;
	}

	// get the incoming blob
	int cbBlob;
	hr = pauth->GetBlobLength(&cbBlob);
	if (cbBlob>0)
	{
		pbBlob = new BYTE[cbBlob];
		if (pbBlob==NULL)
		{
			hr = E_OUTOFMEMORY;
			goto CLEAN_UP;
		}
		hr = pauth->CopyBlob(pbBlob);
		if (FAILED(hr))
			goto CLEAN_UP;
	}
	else
		pbBlob = NULL;

	// output
	pchToken = new char[m_cbTokenMax];
	if (pchToken==NULL)
	{
		hr = E_OUTOFMEMORY;
		goto CLEAN_UP;
	}
	sbdOut.ulVersion = 0;
	sbdOut.cBuffers = 1;
	sbdOut.pBuffers = &sbOut;
	sbOut.cbBuffer = m_cbTokenMax;
	sbOut.BufferType = SECBUFFER_TOKEN;
	sbOut.pvBuffer = pchToken;

	// input
	sbdIn.ulVersion = 0;
	sbdIn.cBuffers = 1;
	sbdIn.pBuffers = &sbIn;

	// get information out of the blob
	sbIn.cbBuffer = cbBlob;
	sbIn.BufferType = SECBUFFER_TOKEN;
	sbIn.pvBuffer = pbBlob;

	ss = m_psft->AcceptSecurityContext(	&hcred,
										(bInitialized==VARIANT_TRUE) ? &hctxt : NULL,
										&sbdIn,
										0,
										SECURITY_NATIVE_DREP,
										&hctxt,
										&sbdOut,
										&ulCtxtAttr,
										&ts );

	if (FAILED(ss))
	{
		hr = (HRESULT) ss;
		goto CLEAN_UP;
	}

	// Complete token -- if applicable
	//
	if (ss==SEC_I_COMPLETE_NEEDED || ss==SEC_I_COMPLETE_AND_CONTINUE)
	{
		if (m_psft->CompleteAuthToken!=NULL)
		{
			ss = m_psft->CompleteAuthToken(&hctxt, &sbdOut);
			if (FAILED(ss))
			{
				hr = (HRESULT) ss;
				goto CLEAN_UP;
			}
		}
		else
		{
			hr = E_FAIL;
			goto CLEAN_UP;
		}
	}

	// stick the context into the authentication
	hr = pauth->put_Context(*((SECHANDLE *) &hctxt));
	if (FAILED(hr))
		goto CLEAN_UP;

	// stick the output buffer into the blob
	hr = pauth->SetBlob((BYTE *) pchToken, (int) sbOut.cbBuffer);
	if (FAILED(hr))
		goto CLEAN_UP;

	if (ss!=SEC_I_CONTINUE_NEEDED && ss!=SEC_I_COMPLETE_AND_CONTINUE)
	{
		// set the flag in the authentication, too
		hr = pauth->put_Authenticated(VARIANT_TRUE);
		if (FAILED(hr))
			goto CLEAN_UP;

		*pbDone = VARIANT_TRUE;
	}

CLEAN_UP:

	if (pchToken)
		delete [] pchToken;
	if (pbBlob)
		delete [] pbBlob;

	return hr;
}


STDMETHODIMP CAuthenticate::Initialize()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	FARPROC	pfnInit;
	OSVERSIONINFO ovi;
	CString strSecurityDLL;

	// determine which DLL to load
	ovi.dwOSVersionInfoSize = sizeof(ovi);
	::GetVersionEx(&ovi);
	m_bOnNT = (ovi.dwPlatformId==VER_PLATFORM_WIN32_NT);
	if (m_bOnNT)
		strSecurityDLL = "SECURITY.DLL";
	else
		// REVIEW: reject win32s or don't bother?
		strSecurityDLL = "SECUR32.DLL";

	// release any old library
	if (m_hSecurity!=NULL)
		::FreeLibrary(m_hSecurity);
	m_hSecurity = ::LoadLibrary(strSecurityDLL);
	if (m_hSecurity==NULL)
		return E_FAIL;

	pfnInit = ::GetProcAddress(m_hSecurity, SECURITY_ENTRYPOINT);
	if (pfnInit==NULL)
	{
		::FreeLibrary(m_hSecurity);
		m_hSecurity = NULL;
		return E_FAIL;
	}

	// call the initialization entry point to get the security function table
	m_psft = (SecurityFunctionTable *) pfnInit();
	if (m_psft==NULL)
	{
		::FreeLibrary(m_hSecurity);
		m_hSecurity = NULL;
		return E_FAIL;
	}

	return S_OK;
}

STDMETHODIMP CAuthenticate::get_Package(BSTR *pbstrVal)
{
	if (pbstrVal==NULL)
		return E_POINTER;

	*pbstrVal = m_bstrPackage.Copy();

	return S_OK;
};

STDMETHODIMP CAuthenticate::put_Package(BSTR newVal)
{
	SECURITY_STATUS ss;
	SecPkgInfo *pspi;

	// verify that we're initialized
	if (m_psft==NULL)
		return E_FAIL;

	m_bstrPackage = newVal;

	// get the information for this package
	CString str = m_bstrPackage;
	char *psz = str.GetBuffer(_MAX_PATH);
	ss = m_psft->QuerySecurityPackageInfo(psz, &pspi);
	if (FAILED(ss))
		return (HRESULT) ss;

	m_cbTokenMax = pspi->cbMaxToken;
	m_psft->FreeContextBuffer(pspi);

	return S_OK;
}

STDMETHODIMP CAuthenticate::Terminate(IAuthentication * pauth)
{
	CtxtHandle hctxt;
	CredHandle hcred;
	SecHandle hNil = {0};

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pauth==NULL)
		return E_POINTER;

	// verify that we're initialized
	if (m_psft==NULL)
		return E_FAIL;

	// see if he can get a context handle
	if (SUCCEEDED(pauth->get_Context((SECHANDLE *) &hctxt)))
	{
		m_psft->DeleteSecurityContext(&hctxt);
		pauth->put_Context(*((SECHANDLE *) &hNil));
	}

	if (SUCCEEDED(pauth->get_Credentials((SECHANDLE *) &hcred)))
	{
		m_psft->FreeCredentialHandle(&hcred);
		pauth->put_Credentials(*((SECHANDLE *) &hNil));
		pauth->put_Initialized(VARIANT_FALSE);
	}

	return S_OK;
}

STDMETHODIMP CAuthenticate::Impersonate(IAuthentication * pauth)
{
	HRESULT hr = S_OK;
	CtxtHandle hctxt;
	SECURITY_STATUS ss;
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pauth==NULL)
		return E_POINTER;

	// verify that we're initialized
	if (m_psft==NULL)
		return E_FAIL;

	if (!m_bOnNT)
		return E_FAIL;

	// get context handle
	hr = pauth->get_Context((SECHANDLE *) &hctxt);
	if (FAILED(hr))
		goto CLEAN_UP;

	// try to impersonate
	ss = m_psft->ImpersonateSecurityContext(&hctxt);
	if (FAILED(hr))
		hr = (HRESULT) ss;

CLEAN_UP:

	return hr;
}

STDMETHODIMP CAuthenticate::RevertContext(IAuthentication * pauth)
{
	HRESULT hr = S_OK;
	CtxtHandle hctxt;
	SECURITY_STATUS ss;
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pauth==NULL)
		return E_POINTER;

	// verify that we're initialized
	if (m_psft==NULL)
		return E_FAIL;

	if (!m_bOnNT)
		return E_FAIL;

	// get context handle
	hr = pauth->get_Context((SECHANDLE *) &hctxt);
	if (FAILED(hr))
		goto CLEAN_UP;

	// try to impersonate
	ss = m_psft->RevertSecurityContext(&hctxt);
	if (FAILED(hr))
		hr = (HRESULT) ss;

CLEAN_UP:

	return hr;
}

STDMETHODIMP CAuthenticate::IsGroupMember(IAuthentication *pauth, BSTR bstrGroup, VARIANT_BOOL *pbIsMember)
{
	HRESULT hr;
	BSTR bstrDomAndName = NULL;
	CString strDomAndName;

	if (pbIsMember==NULL)
		return E_POINTER;

	*pbIsMember = VARIANT_FALSE;

	if (!m_bOnNT)
		return E_FAIL;

	// now, get the user's information
	hr = GetUserInfo(pauth, &bstrDomAndName);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// get the membership list for the specified group
	strDomAndName = bstrDomAndName;
	hr = IsUserInGroup(pauth, strDomAndName, bstrGroup, pbIsMember);
	
ERROR_ENCOUNTERED:

	return hr;
}



HRESULT CAuthenticate::GetAuthenticationData(IAuthentication *pauth, void **ppvAuthData)
{
	HRESULT hr=S_OK;
	CString strUser;
	CString strDomain;
	CString strPassword;
	DWORD cb;

	if (ppvAuthData==NULL)
		return E_POINTER;

	// initialize
	*ppvAuthData = NULL;

	// if not NTLM package, no data
	if (CString(m_bstrPackage)!="NTLM")
		return S_OK;

	// if on NT, get the user name and domain
	if (m_bOnNT)
	{
		// use NetWkstaUserGetInfo, but can't call it directly since
		// not available on win95

		// get the module and function address
		HINSTANCE hinst;
		typedef NET_API_STATUS (_stdcall *PFNWUGI)(LPTSTR, DWORD, BYTE **);
		PFNWUGI pfn;
		hinst = ::LoadLibrary("NETAPI32.DLL");
		if (hinst==NULL)
			return E_FAIL;
		pfn = (PFNWUGI) ::GetProcAddress(hinst, "NetWkstaUserGetInfo");
		if (pfn==NULL)
			return E_FAIL;

		// call it
		WKSTA_USER_INFO_1 *pwi;
		if ((*pfn)(NULL, 1, (BYTE **) &pwi)!=0)
			return E_FAIL;

		strUser = (LPCWSTR)pwi->wkui1_username;
		strDomain = (LPCWSTR)pwi->wkui1_logon_domain;

		// release stuff
		::FreeLibrary(hinst);

	}
	else
	{
		cb = _MAX_PATH;
		::GetUserName(strUser.GetBuffer(cb), &cb);
	}

	// prime the logon dialog with these
	CLogonDlg dlg;
	dlg.m_strUser = strUser;
	dlg.m_strDomain = strDomain;

	// process
	if (dlg.DoModal()==IDOK)
	{
		// pass into auth
		hr = pauth->SetUserInfo(CComBSTR(dlg.m_strUser),
								CComBSTR(dlg.m_strPassword),
								CComBSTR(dlg.m_strDomain));
		if (FAILED(hr))
			return hr;

		// create an authdata thang and passback
		hr = pauth->get_AuthenticationData((BYTE **) ppvAuthData);
	}
	else
	{
		// need to know they cancelled
		hr=E_FAIL;
	}

	return hr;
	
}

HRESULT CAuthenticate::InitializeNetApi()
{
	if (!m_bOnNT)
		return E_FAIL;

	// if no library loaded yet, do it
	if (m_hNetApi==NULL)
	{
		m_hNetApi = ::LoadLibrary("NETAPI32.DLL");
		if (m_hNetApi==NULL)
			return E_FAIL;
	}
	
	// get function pointers
	if (m_pfnWUGI==NULL)
	{
		m_pfnWUGI = (PFNWUGI) ::GetProcAddress(m_hNetApi, "NetWkstaUserGetInfo");
		if (m_pfnWUGI==NULL)
			return E_FAIL;
	}

	if (m_pfnABF==NULL)
	{
		m_pfnABF = (PFNABF) ::GetProcAddress(m_hNetApi, "NetApiBufferFree");
		if (m_pfnABF==NULL)
			return E_FAIL;
	}
	
	if (m_pfnLGGM==NULL)
	{
		m_pfnLGGM = (PFNLGGM) ::GetProcAddress(m_hNetApi, "NetLocalGroupGetMembers");
		if (m_pfnLGGM==NULL)
			return E_FAIL;
	}

	if (m_pfnNGGU==NULL)
	{
		m_pfnNGGU = (PFNNGGU) ::GetProcAddress(m_hNetApi, "NetGroupGetUsers");
		if (m_pfnNGGU==NULL)
			return E_FAIL;
	}

	return S_OK;
}


HRESULT CAuthenticate::InitializeAdvApi()
{
	if (!m_bOnNT)
			return E_FAIL;

	// if not loaded then load now
	if (m_hAdvApi==NULL)
	{
		m_hAdvApi = ::LoadLibrary("ADVAPI32.DLL");
		if (m_hAdvApi==NULL)
			return E_FAIL;
	}

	if (m_pfnOTT == NULL)
	{
		m_pfnOTT = (PFNOTT) ::GetProcAddress(m_hAdvApi, "OpenThreadToken");
		if (m_pfnOTT==NULL)
			return E_FAIL;
	}

	if (m_pfnEQSID == NULL)
	{
		m_pfnEQSID = (PFNEQSID) ::GetProcAddress(m_hAdvApi, "EqualSid");
		if (m_pfnEQSID==NULL)
			return E_FAIL;
	}

	if (m_pfnGTI == NULL)
	{
		m_pfnGTI = (PFNGTI) ::GetProcAddress(m_hAdvApi, "GetTokenInformation");
		if (m_pfnGTI==NULL)
			return E_FAIL;
	}

	if (m_pfnFSID == NULL)
	{
		m_pfnFSID = (PFNFSID) ::GetProcAddress(m_hAdvApi, "FreeSid");
		if (m_pfnFSID==NULL)
			return E_FAIL;
	}

	if (m_pfnAISID == NULL)
	{
		m_pfnAISID = (PFNAISID) ::GetProcAddress(m_hAdvApi, "AllocateAndInitializeSid");
		if (m_pfnAISID==NULL)
			return E_FAIL;
	}

	return S_OK;
}

HRESULT	CAuthenticate::GetUserInfo(IAuthentication *pauth, BSTR* pbstrDomainAndName)
{
	HRESULT hr = S_OK;
	CString strDomAndName;
	CtxtHandle hctxt;
	SecPkgContext_Authority secCtxAuthority;
	SecPkgContext_Names secCtxNames;

	memset(&secCtxAuthority,0,sizeof(SecPkgContext_Authority));
	memset(&secCtxNames,0,sizeof(SecPkgContext_Names));

	if (pauth == NULL || pbstrDomainAndName == NULL)
		return E_POINTER;

	// verify that we're initialized
	if (m_psft == NULL)
		return E_FAIL;
	
	// get context handle
	hr = pauth->get_Context((SECHANDLE *) &hctxt);

	//NOTE: [T.O.] it appears the NTML doesn't support SECPKG_ATTR_AUTHORITY and instead
	//			packages the domain/name in the sUserName in the ATTR_NAMES request. For
	//			now I'll leave in a check for this so we are generic for newer providers
	//			like kerberos

	// check its attributes for an authority
	hr=m_psft->QueryContextAttributes(&hctxt, SECPKG_ATTR_AUTHORITY, &secCtxAuthority);
	if (!FAILED(hr))
	{
		// copy data 
		strDomAndName=secCtxAuthority.sAuthorityName;
		strDomAndName+="\\";
	}

	// check attributes for a user name
	hr=m_psft->QueryContextAttributes(&hctxt, SECPKG_ATTR_NAMES, &secCtxNames);
	if (FAILED(hr))
	{
		goto ERROR_ENCOUNTERED;
	}
	else
	{
		//copy data
		strDomAndName+=secCtxNames.sUserName;

		// make uppercase for later comparisons
		strDomAndName.MakeUpper();
	}


ERROR_ENCOUNTERED:

	// Alloc a bstr for our out pointer
	if (!FAILED(hr))
		*pbstrDomainAndName = strDomAndName.AllocSysString();

	// free buffers
	if (secCtxAuthority.sAuthorityName)
		m_psft->FreeContextBuffer(&secCtxAuthority.sAuthorityName);

	if (secCtxNames.sUserName)
	{
		//[C.H.]: this line can crash in some flavors of nt4... not too expensive to skip free
		//m_psft->FreeContextBuffer(&secCtxNames.sUserName);
	}

	return hr;
}

HRESULT CAuthenticate::IsUserInGroup(IAuthentication *pauth, CString& strDomAndName, BSTR bstrGroup, VARIANT_BOOL *pbInGroup)
{
	HRESULT hr=S_OK;
	LOCALGROUP_MEMBERS_INFO_2 *prginfo=NULL;
	DWORD cEntriesRead;
	DWORD cEntries;
	DWORD dwResumeHandle=0;

	if (pbInGroup==NULL)
		return E_POINTER;

	*pbInGroup = VARIANT_FALSE;

	hr = InitializeNetApi();
	if (FAILED(hr))
		return hr;

	// get the membership list for the specified group
	for(;;)
	{
		BSTR bstrGlobalGroup;
		hr = MyNetLocalGroupGetMembers(	NULL, 
										bstrGroup,
										2,
										(BYTE **) &prginfo,
										8192,
										&cEntriesRead,
										&cEntries,
										&dwResumeHandle);
		if FAILED(hr)
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}

		// search through cEntriesRead for user
		for (int i=0; ((i<(int)cEntriesRead) && (*pbInGroup == VARIANT_FALSE)); i++)
		{
			switch( prginfo[i].lgrmi2_sidusage )
			{
				case 2:
				//	if (prginfo[i].lgrmi2_sidusage	== 2)
				{
					// This member of the group is a Domain Global Group....
					bstrGlobalGroup = (BSTR)prginfo[i].lgrmi2_domainandname;
					// this is a global group which can only contain users.. lets check it out
					hr = IsUserInGlobalGroup(pauth, strDomAndName,prginfo[i].lgrmi2_sid,pbInGroup);
					break;
				}
				case 3:
				{
					// This member of the group is a "Well-Known Group"..
					break;
				}
				default:
				{				
					CString strMember = ((LPCWSTR)prginfo[i].lgrmi2_domainandname);
					strMember.MakeUpper();
					if (strDomAndName == strMember)
					{
						*pbInGroup = VARIANT_TRUE;
						goto ERROR_ENCOUNTERED;
					}
				}
			}
		}

		// if no more members left to check then fail.  Else
		// go get s'more
		if (cEntries==cEntriesRead)
			break;

		MyNetApiBufferFree(prginfo);
		prginfo = NULL;
	}

ERROR_ENCOUNTERED:

	if (prginfo)
		MyNetApiBufferFree(prginfo);

	return hr;

}


HRESULT CAuthenticate::IsUserInGlobalGroup(IAuthentication *pauth, CString& strDomAndName, PSID psidGlobalGroup, VARIANT_BOOL *pbInGroup)
{
	HRESULT hr = S_OK;
	GROUP_USERS_INFO_0 *prginfo = NULL;
	DWORD cEntriesRead = 0;
	DWORD cEntries = 0;
	DWORD dwResumeHandle = 0;
	DWORD dwSize = 0;
	CWinThread* pThread = NULL;
	HANDLE HandleToken = NULL;
	PTOKEN_GROUPS pTokenGroupInfo = NULL;

	hr = InitializeAdvApi();
	if (FAILED(hr))
		return hr;

	hr = Impersonate(pauth);
	// REVIEW: any cases where we get a failed hr but still need to revert the context?
	if FAILED(hr)
		return hr;

	pThread = AfxGetThread();
	if (pThread)
	{
			if (!MyOpenThreadToken(pThread->m_hThread, TOKEN_QUERY, FALSE, &HandleToken))
			{
				hr = GetLastError();
				if FAILED(hr)
					goto ERROR_ENCOUNTERED;	
			}
	
			// Call GetTokenInformation to get the buffer size.
			if(! MyGetTokenInformation(HandleToken, TokenGroups, NULL, dwSize, &dwSize))
			{
				hr = GetLastError();
				if( hr != ERROR_INSUFFICIENT_BUFFER )
					goto ERROR_ENCOUNTERED;
			}

			// Allocate the buffer.
			pTokenGroupInfo = (PTOKEN_GROUPS) GlobalAlloc(GPTR, dwSize);

			// Call GetTokenInformation again to get the group information.

			if(! MyGetTokenInformation(HandleToken, TokenGroups, pTokenGroupInfo, dwSize, &dwSize))
			{
				hr = GetLastError();
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;
			}

		if (pTokenGroupInfo == NULL)
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}


		for(DWORD i=0; i<pTokenGroupInfo->GroupCount; i++) {
		    if (MyEqualSid(psidGlobalGroup, pTokenGroupInfo->Groups[i].Sid))
				*pbInGroup = VARIANT_TRUE;
		}
		
	}

ERROR_ENCOUNTERED:
	// Cleanup allocations and revert context	
	if (HandleToken)
		CloseHandle(HandleToken);
	
	if (pTokenGroupInfo)
		GlobalFree(pTokenGroupInfo);

	if (pauth)
		RevertContext(pauth);

	return hr;
}


HRESULT CAuthenticate::IsUserInWellKnownGroup(IAuthentication *pauth, CString& cstrWellKnownGroupName, VARIANT_BOOL* pbInGroup)
{

// REVIEW: THIS ISN'T USED OR TESTED YET... USE AT OWN RISK
//		
//	Expected usage.. given a context we impersonate it, get token access SIDs
//	Compare tokens with the SID of wellknown group name that was passed in

	HRESULT hr = S_OK;
	GROUP_USERS_INFO_0 *prginfo = NULL;
	DWORD cEntriesRead = 0;
	DWORD cEntries = 0;
	DWORD dwResumeHandle = 0;
	DWORD dwSize = 0;
	CWinThread* pThread = NULL;
	HANDLE HandleToken = NULL;
	PTOKEN_GROUPS pTokenGroupInfo = NULL;
	PSID psidGroup = NULL;

	hr = InitializeAdvApi();
	if (FAILED(hr))
		return hr;

	hr = Impersonate(pauth);
	// REVIEW: any cases where we get a failed hr but still need to revert the context?
	if FAILED(hr)
		return hr;

	pThread = AfxGetThread();
	if (pThread)
	{
			if (!MyOpenThreadToken(pThread->m_hThread, TOKEN_QUERY, FALSE, &HandleToken))
			{
				hr = GetLastError();
				if FAILED(hr)
					goto ERROR_ENCOUNTERED;	
			}
	
			// Call GetTokenInformation to get the buffer size.
			if(! MyGetTokenInformation(HandleToken, TokenGroups, NULL, dwSize, &dwSize))
			{
				hr = GetLastError();
				if( hr != ERROR_INSUFFICIENT_BUFFER )
					goto ERROR_ENCOUNTERED;
			}

			// Allocate the buffer.
			pTokenGroupInfo = (PTOKEN_GROUPS) GlobalAlloc(GPTR, dwSize);

			// Call GetTokenInformation again to get the group information.

			if(! MyGetTokenInformation(HandleToken, TokenGroups, pTokenGroupInfo, dwSize, &dwSize))
			{
				hr = GetLastError();
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;
			}

			if (pTokenGroupInfo == NULL)
			{
				hr = E_FAIL;
				goto ERROR_ENCOUNTERED;
			}

			// FORM THE SID FOR THE WELL KNOWN GROUPS..  
			if (cstrWellKnownGroupName.CompareNoCase("Administrators") == 0)
			{
			
				// Now lets create a System Identifier for the Admin group and compare it..
				SID_IDENTIFIER_AUTHORITY SystemSidAuthority= SECURITY_NT_AUTHORITY;
				if (MyAllocateAndInitializeSid( &SystemSidAuthority, 2, 
					SECURITY_BUILTIN_DOMAIN_RID, 
					DOMAIN_ALIAS_RID_ADMINS,
					0, 0, 0, 0, 0, 0, &psidGroup) )
				{
					goto ERROR_ENCOUNTERED;
				}
			}

			if (cstrWellKnownGroupName.CompareNoCase("Everyone") == 0)
			{
				//Now lets try the Domain/Everyone Group and succeed if that happens
				SID_IDENTIFIER_AUTHORITY WorldSidAuthority= SECURITY_WORLD_SID_AUTHORITY;
				if (!MyAllocateAndInitializeSid( &WorldSidAuthority, 1, 
					SECURITY_WORLD_RID, 
					0, 0, 0, 0, 0, 0, 0, &psidGroup) )
				{
					goto ERROR_ENCOUNTERED;
				}

			}
			
			if (psidGroup)
			{
				// loop through all the user's SID's and compare to the well know ones..
				for(DWORD i=0; i<pTokenGroupInfo->GroupCount; i++)
				{
					if (MyEqualSid(psidGroup, pTokenGroupInfo->Groups[i].Sid))
						*pbInGroup = VARIANT_TRUE;
				}
			}
			else
			{
				// we don't know that group.. harsh error for now
				hr = E_FAIL;
			}
			
	}

ERROR_ENCOUNTERED:		
	// Cleanup allocations and revert context	
	if (HandleToken)
		CloseHandle(HandleToken);
	
	if (pTokenGroupInfo)
		GlobalFree(pTokenGroupInfo);

	if (pauth)
		RevertContext(pauth);

	if (psidGroup)
		MyFreeSid(psidGroup);

	return hr;
}