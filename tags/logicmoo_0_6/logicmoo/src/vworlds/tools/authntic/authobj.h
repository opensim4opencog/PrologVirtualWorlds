// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// authobj.h : Declaration of the CAuthenticate

#ifndef __AUTHENTICATE_H_
#define __AUTHENTICATE_H_

#include "resource.h"       // main symbols
#include <propbase.h>
#include <lm.h>
#include <winbase.h>
#include <winnt.h>

EXTERN_C const CLSID CLSID_Authenticate;

// types for GetProcAddress and indirect calls
typedef NET_API_STATUS (_stdcall *PFNWUGI)(LPTSTR, DWORD, BYTE **);
typedef NET_API_STATUS (_stdcall *PFNABF)(LPVOID);
typedef NET_API_STATUS (_stdcall *PFNLGGM)(LPWSTR, LPWSTR, DWORD, LPBYTE *, DWORD,  LPDWORD, LPDWORD, LPDWORD); 
typedef NET_API_STATUS (_stdcall *PFNNGGU)(LPWSTR, LPWSTR, DWORD, LPBYTE *, DWORD,  LPDWORD, LPDWORD, LPDWORD); 
typedef NET_API_STATUS (_stdcall *PFNNLGGI)(LPWSTR, LPWSTR, DWORD, LPBYTE *); 
typedef WINADVAPI BOOL	(_stdcall *PFNOTT)(HANDLE, DWORD, BOOL, PHANDLE);
typedef WINADVAPI BOOL	(_stdcall *PFNEQSID)(PSID, PSID);
typedef WINADVAPI BOOL	(_stdcall *PFNGTI)(HANDLE, TOKEN_INFORMATION_CLASS, LPVOID, DWORD, PDWORD);
typedef WINADVAPI PVOID	(_stdcall *PFNFSID)(PSID);
typedef WINADVAPI PVOID	(_stdcall *PFNAISID)(PSID_IDENTIFIER_AUTHORITY, BYTE, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, PSID );

// indirect helpers
#define MyNetWkstaUserGetInfo		(*m_pfnWUGI)
#define	MyNetLocalGroupGetMembers	(*m_pfnLGGM)
#define MyNetApiBufferFree			(*m_pfnABF)
#define MyNetGroupGetUsers			(*m_pfnNGGU)
#define MyNetLocalGroupGetInfo		(*m_pfnNLGGI)
#define MyOpenThreadToken			(*m_pfnOTT)
#define MyEqualSid					(*m_pfnEQSID)
#define MyGetTokenInformation		(*m_pfnGTI)
#define MyFreeSid					(*m_pfnFSID)
#define MyAllocateAndInitializeSid	(*m_pfnAISID)

/////////////////////////////////////////////////////////////////////////////
// CAuthenticate
class ATL_NO_VTABLE CAuthenticate : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CAuthenticate, &CLSID_Authenticate>,
	public IDispatchImpl<IVWAuthenticate, &IID_IVWAuthenticate, &LIBID_VWSYSTEMLib>
{
public:
	CAuthenticate();
	~CAuthenticate();

DECLARE_REGISTRY_RESOURCEID(IDR_AUTHENTICATE)

BEGIN_COM_MAP(CAuthenticate)
	COM_INTERFACE_ENTRY(IVWAuthenticate)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IVWAuthenticate
public:
	STDMETHOD(RevertContext)(/*[in]*/ IAuthentication *pauth);
	STDMETHOD(Impersonate)(/*[in]*/ IAuthentication *pauth);
	STDMETHOD(IsGroupMember)(/*[in]*/ IAuthentication *pauth, /*[in]*/ BSTR bstrGroup, /*[out, retval]*/ VARIANT_BOOL *pbIsMember);
	STDMETHOD(Terminate)(/*[in]*/ IAuthentication *pauth);
	STDMETHOD(Initialize)();
	STDMETHOD(get_Package)(/*[out, retval]*/ BSTR *pbstrVal);
	STDMETHOD(put_Package)(/*[in]*/ BSTR newVal);
	STDMETHOD(GenerateServerContext)(/*[in]*/ IAuthentication *pauth, /*[out, retval]*/ VARIANT_BOOL *pbDone);
	STDMETHOD(GenerateClientContext)(/*[in]*/ IAuthentication *pauth, /*[in]*/ VARIANT_BOOL bShowDialog, /*[out, retval]*/ VARIANT_BOOL *pbDone);
	STDMETHOD(GetUserInfo)(/*[in]*/ IAuthentication *pauth, /*[out]*/ BSTR* pbstrDomainAndName);
protected:

	HINSTANCE	m_hSecurity;
	HINSTANCE	m_hNetApi;
	HINSTANCE	m_hAdvApi;
	SecurityFunctionTable *m_psft;
	DWORD		m_cbTokenMax;
	CComBSTR	m_bstrPackage;
	BOOL		m_bOnNT;

	PFNWUGI	m_pfnWUGI;
	PFNABF	m_pfnABF;
	PFNLGGM	m_pfnLGGM;
	PFNNGGU m_pfnNGGU;
	PFNOTT	m_pfnOTT;
	PFNEQSID	m_pfnEQSID;
	PFNGTI	m_pfnGTI;
	PFNFSID	m_pfnFSID;
	PFNAISID	m_pfnAISID;

	// helpers
	HRESULT InitializeNetApi();
	HRESULT InitializeAdvApi();
	HRESULT GetAuthenticationData(IAuthentication *pauth, void **pvAuthData);
	HRESULT IsUserInGroup(IAuthentication *pauth, CString& strDomAndName, BSTR bstrGroup, VARIANT_BOOL *pbInGroup);
	HRESULT IsUserInGlobalGroup(IAuthentication *pauth, CString& strDomAndName, PSID psidGlobalGroup, VARIANT_BOOL *pbInGroup);
	HRESULT IsUserInWellKnownGroup(IAuthentication *pauth, CString&, VARIANT_BOOL* pbInGroup);
};

#endif //__AUTHENTICATE_H_
