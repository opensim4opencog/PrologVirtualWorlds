// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// vwocx.cpp : Implementation of Cvwocx
#include "stdafx.h"
#include <vwobject.h>
#include <ocxproxy.h>
#include "vwocx.h"

IMPLEMENT_OBJECTPROPERTY(COCXProxyObject)
/////////////////////////////////////////////////////////////////////////////
// Cvwocx

HRESULT WINAPI CustomQI(void* pv, REFIID riid, LPVOID* ppv, DWORD dw)
{

	COCXProxyObject * pProxy = ( COCXProxyObject *) pv;
	*ppv = NULL;

	if (InlineIsEqualGUID(pProxy->m_guidDISPEVENTS, riid)) // use Events interface
	{
		IOCXProxy* pUnk = (IOCXProxy*)(pProxy);
		pUnk->AddRef();
		*ppv = pUnk;
		return  S_OK;
	}
	return E_NOINTERFACE;
}

COCXProxyObject::COCXProxyObject()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "COCXProxyObject::COCXProxyObject, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif

	m_pEventTypeInfo = NULL;
	m_pOCX = NULL;
	m_guidDISPEVENTS = IID_IDispatch;
}

COCXProxyObject::~COCXProxyObject()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "COCXProxyObject::~COCXProxyObject, this 0x%0x\n", this);
#endif

	Terminate();
}

STDMETHODIMP COCXProxyObject::Terminate(void)
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "COCXProxyObject::Terminate, this 0x%0x\n", this);
#endif

	COCXProxyObjectBase::Terminate();

	if (m_pOCX)
	{
		UnAdvise(m_pOCX, m_dwCookie);

		SAFERELEASE(m_pOCX);
	}

	SAFERELEASE(m_pEventTypeInfo);

	return S_OK;
}

STDMETHODIMP COCXProxyObject::get_OCX(/*[out, retval]*/ LPDISPATCH *pVal)
{
	*pVal = m_pOCX;
	SAFEADDREF( *pVal );

	return S_OK;
}

STDMETHODIMP COCXProxyObject::put_OCX(/*[in]*/ LPDISPATCH newVal)
{
	UnAdvise(m_pOCX, m_dwCookie);

	SAFERELEASE( m_pOCX );
	m_pOCX = newVal;
	SAFEADDREF( m_pOCX );

	Advise(m_pOCX, &m_dwCookie);

	return S_OK;
}

STDMETHODIMP COCXProxyObject::Advise( IUnknown *pUnk, ULONG *pdwCookie )
{
	HRESULT hr = E_FAIL;
	IConnectionPoint* pconnpt = NULL;
	IConnectionPointContainer* pconnptctr = NULL;
	IProvideClassInfo2* pProvideClassInfo = NULL;
	ITypeInfo* pTypeInfo = NULL;
	ITypeLib* pTypeLib = NULL;
	unsigned int index;

	if (!pUnk ) goto errout;

	hr = pUnk->QueryInterface(IID_IProvideClassInfo2, (LPVOID*)&pProvideClassInfo);
	if (FAILED(hr))
		goto errout;

	hr = pProvideClassInfo->GetClassInfo(&pTypeInfo);
	if (FAILED(hr))
		goto errout;

	hr = pTypeInfo->GetContainingTypeLib(&pTypeLib, &index);
	if (FAILED(hr))
		goto errout;

	hr = pProvideClassInfo->GetGUID(GUIDKIND_DEFAULT_SOURCE_DISP_IID,&m_guidDISPEVENTS);
	if (FAILED(hr))
		goto errout;

	SAFERELEASE( m_pEventTypeInfo );
	hr = pTypeLib->GetTypeInfoOfGuid(m_guidDISPEVENTS,&m_pEventTypeInfo);
	if (FAILED(hr))
		goto errout;

	hr = pUnk->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pconnptctr);
	if (FAILED(hr))
		goto errout;

	hr = pconnptctr->FindConnectionPoint(m_guidDISPEVENTS, &pconnpt);
	if (SUCCEEDED(hr))
	{
		hr = pconnpt->Advise((IOCXProxy*)this , pdwCookie);
		SAFERELEASE( pconnpt );
	}

errout:
	SAFERELEASE( pTypeInfo );
	SAFERELEASE( pTypeLib );
	SAFERELEASE( pProvideClassInfo );
	SAFERELEASE( pconnpt);
	SAFERELEASE( pconnptctr );

	return hr;
}

STDMETHODIMP COCXProxyObject::UnAdvise( IUnknown *pUnk, ULONG dwCookie )
{
	HRESULT hr = E_FAIL;
	IConnectionPoint* pconnpt = NULL;
	IConnectionPointContainer* pconnptctr = NULL;
	IProvideClassInfo2* pProvideClassInfo = NULL;
	GUID guid;

	if (!pUnk ) goto errout;

	hr = pUnk->QueryInterface(IID_IProvideClassInfo2, (LPVOID*)&pProvideClassInfo);
	if (FAILED(hr))
		goto errout;

	hr = pProvideClassInfo->GetGUID(GUIDKIND_DEFAULT_SOURCE_DISP_IID,&guid);
	if (FAILED(hr))
		goto errout;

	hr = pUnk->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pconnptctr);
	if (FAILED(hr))
	goto errout;

	hr = pconnptctr->FindConnectionPoint(guid, &pconnpt);

	if (SUCCEEDED(hr))
	{
		pconnpt->Unadvise(  dwCookie );
		SAFERELEASE( pconnpt );
		SAFERELEASE( m_pEventTypeInfo );
	}

errout:
	SAFERELEASE( pconnpt);
	SAFERELEASE( pconnptctr );
	SAFERELEASE( pProvideClassInfo );

	return hr;
}

STDMETHODIMP COCXProxyObject::Invoke(	DISPID dispidMember, 
								REFIID riid,
								LCID lcid, 
								WORD wFlags, 
								DISPPARAMS* pdispparams, 
								VARIANT* pvarResult,
								EXCEPINFO* pexcepinfo, 
								UINT* puArgErr)
{	
	BSTR bstrName = NULL;
	HRESULT hr = E_FAIL;
	IThing *pThing = NULL;
	static CComBSTR bstrUnderscore("_");

	hr = COCXProxyObjectBase::Invoke(dispidMember,riid,lcid,wFlags,pdispparams,pvarResult,pexcepinfo,puArgErr);
	if ( FAILED(hr) && m_pEventTypeInfo)
	{
		// Clear any results that may have been returned from Invoke
		//::VariantClear( pvarResult );

		// Figure out the name of the method call
		hr = m_pEventTypeInfo->GetDocumentation(dispidMember, &bstrName, NULL, NULL,NULL);
		if ( FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// Get the Instance
		if ( m_pSecurity )
		{
			hr = m_pSecurity->get_Instance( &pThing );
			if ( FAILED(hr))
				goto ERROR_ENCOUNTERED;

			if ( pThing )
			{
				CComBSTR bstrMangledName = m_bstrPropertyName;
				bstrMangledName += bstrUnderscore;
				bstrMangledName += bstrName;
				hr = pThing->InvokeMethodExt(bstrMangledName.m_str, pdispparams, pvarResult);
			}
		}
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pThing);
	SAFEFREESTRING(bstrName);

	if (hr == VWOBJECT_E_METHODNOTEXIST)
		hr = S_OK;

	return hr;
}

ULONG COCXProxyObject::InternalRelease()
{
	ULONG ref = CComObjectRootEx<CComMultiThreadModel>::InternalRelease();

	// Are we down to a circular reference?
	// if so release it...
	if ((m_dwRef == 1) &&
	   (m_pOCX))
	{
		//Artificially addref so we don't self destruct before we are ready...
		CComObjectRootEx<CComMultiThreadModel>::InternalAddRef();
		CComObjectRootEx<CComMultiThreadModel>::InternalAddRef();
		CComObjectRootEx<CComMultiThreadModel>::InternalAddRef();

		UnAdvise(m_pOCX, m_dwCookie );
		SAFERELEASE(m_pOCX);
		SAFERELEASE(m_pEventTypeInfo );

		ref = CComObjectRootEx<CComMultiThreadModel>::InternalRelease();
		ref = CComObjectRootEx<CComMultiThreadModel>::InternalRelease();
		ref = CComObjectRootEx<CComMultiThreadModel>::InternalRelease();
	}

	return ref;
}