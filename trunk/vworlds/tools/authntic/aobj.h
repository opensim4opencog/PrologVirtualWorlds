// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// aobj.h: Definition of the Authentication class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOBJ_H__3D3CE791_7323_11D1_9082_00C04FD91EFE__INCLUDED_)
#define AFX_AOBJ_H__3D3CE791_7323_11D1_9082_00C04FD91EFE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "resource.h"       // main symbols
#include <propbase.h>
#include "authobj.h"

EXTERN_C const CLSID CLSID_Authentication;

/////////////////////////////////////////////////////////////////////////////
// Authentication

class Authentication : 
	public CComDualImpl<IAuthentication, &IID_IAuthentication, &LIBID_VWSYSTEMLib>, 
	public ISupportErrorInfo,
	public IObjectSafetyImpl<Authentication, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
	public CComObjectRoot,
	public CComCoClass<Authentication,&CLSID_Authentication>
{
public:

	Authentication::Authentication() :
		m_pAuthenticate(NULL),
		m_pvBlob(NULL),
		m_cbBlob(0),
		m_bAuthenticated(VARIANT_FALSE),
		m_bInitialized(VARIANT_FALSE)
	{
		SecHandle hNil = {0};

		// clear data
		memset( m_szUser, 0, sizeof(m_szUser));
		memset( m_szPassword, 0, sizeof(m_szPassword));
		memset( m_szDomain, 0, sizeof(m_szDomain));

		m_hCredentials = hNil;
		m_hContext = hNil;
	}

	Authentication::~Authentication()
	{
		// tell authenticator to terminate us
		if (m_pAuthenticate!=NULL)
			m_pAuthenticate->Terminate(this);

		if (m_pvBlob)
			delete [] m_pvBlob;

	}



BEGIN_COM_MAP(Authentication)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IAuthentication)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(Authentication) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

DECLARE_REGISTRY_RESOURCEID(IDR_AUTHENTICATION)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IAuthentication
public:
	STDMETHOD(put_Authenticate)(IVWAuthenticate *pAuthenticate)
	{
		// this is a weak reference - do not addref!
		m_pAuthenticate = pAuthenticate;

		return S_OK;
	}

	STDMETHOD(get_Context)(SECHANDLE *pVal)
	{
		if (pVal==NULL)
			return E_POINTER;

		*pVal = *((SECHANDLE *) &m_hContext);
		return S_OK;
	}

	STDMETHOD(put_Context)(SECHANDLE newVal)
	{
		m_hContext = *((SecHandle *) &newVal);
		return S_OK;
	}

	STDMETHOD(get_Credentials)(/*[out, retval]*/ SECHANDLE *pVal)
	{
		if (pVal==NULL)
			return E_POINTER;

		*pVal = *((SECHANDLE *) &m_hCredentials);
		return S_OK;
	}

	STDMETHOD(put_Credentials)(/*[in]*/ SECHANDLE newVal)
	{
		if (m_bInitialized==VARIANT_TRUE)
			return E_FAIL;

		m_hCredentials = *((SecHandle *) &newVal);
		m_bInitialized = VARIANT_TRUE;

		return S_OK;
	}

	STDMETHOD(get_Authenticated)(/*[out, retval]*/ VARIANT_BOOL *pVal)
	{
		if (pVal==NULL)
			return E_POINTER;

		if (m_bInitialized!=VARIANT_TRUE)
			m_bAuthenticated = VARIANT_FALSE; //			return E_FAIL;
		

		*pVal = m_bAuthenticated;
		return S_OK;
	}

	STDMETHOD(put_Authenticated)(/*[in]*/ VARIANT_BOOL newVal)
	{
		if (m_bInitialized!=VARIANT_TRUE)
			return E_FAIL;

		m_bAuthenticated = newVal;
		return S_OK;
	}

	STDMETHOD(get_Initialized)(/*[out, retval]*/ VARIANT_BOOL *pVal)
	{
		if (pVal==NULL)
			return E_POINTER;

		*pVal = m_bInitialized;
		return S_OK;
	}

	STDMETHOD(put_Initialized)(/*[in]*/ VARIANT_BOOL newVal)
	{
		m_bInitialized = newVal;

		// clear data
		memset( m_szUser, 0, sizeof(m_szUser));
		memset( m_szPassword, 0, sizeof(m_szPassword));
		memset( m_szDomain, 0, sizeof(m_szDomain));

		return S_OK;
	}

	STDMETHOD(GetBlobLength)(/*[out]*/ int *pcbBlob)
	{
		if (pcbBlob==NULL)
			return E_POINTER;

		*pcbBlob = m_cbBlob;
		return S_OK;
	}

	STDMETHOD(CopyBlob)(/*[in]*/ BYTE *pvDest)
	{
		if (pvDest==NULL)
			return E_POINTER;
		memcpy( pvDest, m_pvBlob, m_cbBlob);
		return S_OK;
	}

	STDMETHOD(SetBlob)(BYTE *pvBlob, int cbBlob)
	{
		// free any existing blob
		if (m_pvBlob)
			delete [] m_pvBlob;

		// allocate and copy new blob
		m_pvBlob = new BYTE[cbBlob];
		if (m_pvBlob==NULL)
		{
			m_cbBlob = 0;
			return E_OUTOFMEMORY;
		}
		m_cbBlob = cbBlob;

		memcpy(m_pvBlob, pvBlob, cbBlob);

		return S_OK;
	}
	
	STDMETHOD(get_AuthenticationInfo)(/*[out, retval]*/ BSTR *pbstrVal)
	{
//		CString strDomAndName;
//		BSTR bstrDomainandName;
//		BSTR strName;
		HRESULT hr=S_OK;

		if (pbstrVal == NULL)
			return E_POINTER;

		if (!m_pAuthenticate)
			return E_FAIL;
		
		//hr = m_pAuthenticate->Impersonate(this);
		//if (FAILED(hr))
		//	goto ERROR_ENCOUNTERED;

		hr = m_pAuthenticate->GetUserInfo(this, pbstrVal);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
/*
		// combine domain and user name
		strDomAndName = strDomain;
		strDomAndName += "\\";
		strDomAndName += strName;
		//strDomAndName.MakeUpper();

		bstrDomAndName.SetSysString(pbstrVal);
*/
//		hr = m_pAuthenticate->RevertContext(this);

ERROR_ENCOUNTERED:
		return hr;
	}

	STDMETHOD(get_AuthenticationData)(/*[out, retval]*/ BYTE **ppVal)
	{

		if (ppVal==NULL)
			return E_POINTER;

		// setup structure
		m_ad.Flags = 1;
		m_ad.User = (BYTE *) m_szUser;
		m_ad.UserLength = strlen(m_szUser);
		m_ad.Domain = (BYTE *) m_szDomain;
		m_ad.DomainLength = strlen(m_szDomain);
		m_ad.Password = (BYTE *) m_szPassword;
		m_ad.PasswordLength = strlen(m_szPassword);

		*ppVal = (BYTE *) &m_ad;

		return S_OK;
	}


	STDMETHOD(SetUserInfo)(/*[in]*/ BSTR bstrUser, /*[in]*/ BSTR bstrPassword, /*[in]*/ BSTR bstrDomain)
	{
		strcpy(m_szUser, CString(bstrUser));
		strcpy(m_szPassword, CString(bstrPassword));
		strcpy(m_szDomain, CString(bstrDomain));
		return S_OK;
	}

private:

	// weak reference backpointer
	IVWAuthenticate *m_pAuthenticate;

	SecHandle		m_hCredentials;
	SecHandle		m_hContext;
	VARIANT_BOOL	m_bAuthenticated;
	VARIANT_BOOL	m_bInitialized;
	BYTE *			m_pvBlob;
	int				m_cbBlob;
	char			m_szUser[_MAX_PATH];
	char			m_szPassword[_MAX_PATH];
	char			m_szDomain[_MAX_PATH];

	SEC_WINNT_AUTH_IDENTITY m_ad;

};

#endif // !defined(AFX_AOBJ_H__3D3CE791_7323_11D1_9082_00C04FD91EFE__INCLUDED_)
