// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// vwcliocx.cpp : Implementation of CVWClientOCX
#include "stdafx.h"
#include <vwobject.h>
#include "vwcliocx.h"

/////////////////////////////////////////////////////////////////////////////
// CVWClientOCX

STDMETHODIMP CVWClientOCX::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IVWClientOCX,
	};
	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

CVWClientOCX::CVWClientOCX() : m_pVWClientObject(NULL), m_dwCookie(0)
{
}

HRESULT CVWClientOCX::FinalConstruct()
{
	HRESULT hr = S_OK;
	CLSID clsid; 
	IConnectionPoint* pconnpt = NULL;
	IConnectionPointContainer* pconnptctr = NULL;

	if (CLSIDFromProgID(CComBSTR("VWSYSTEM.Client.1"), &clsid) == S_OK)
		CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_IVWClient, (void **)&m_pVWClientObject);

	if (m_pVWClientObject)
	{
		m_pVWClientObject->Initialize();

		// connect to IVWObjectSite
		hr = m_pVWClientObject->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pconnptctr);
		if (SUCCEEDED(hr))
		{
			hr = pconnptctr->FindConnectionPoint(IID_IVWClientSite, &pconnpt);
			if (SUCCEEDED(hr))
			{
				pconnpt->Advise(GetUnknown(), &m_dwCookie);
				pconnpt->Release();

				// artificially Release
				GetUnknown()->Release();
			}
			pconnptctr->Release();
		}
	}

	return hr;
}

CVWClientOCX::~CVWClientOCX()
{
}

void CVWClientOCX::FinalRelease()
{
	IConnectionPoint* pconnpt = NULL;
	IConnectionPointContainer* pconnptctr = NULL;

	if (m_pVWClientObject)
	{
		HRESULT hr = m_pVWClientObject->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pconnptctr);
		if (SUCCEEDED(hr))
		{
			hr = pconnptctr->FindConnectionPoint(IID_IVWClientSite, &pconnpt);
			if (SUCCEEDED(hr))
			{
				// artificially addref ptr
				IUnknown* p = (IUnknown *)m_dwCookie;
				p->AddRef();

				pconnpt->Unadvise(m_dwCookie);
				pconnpt->Release();
			}
			pconnptctr->Release();
		}

		m_pVWClientObject->Disconnect();
		m_pVWClientObject->Terminate();

		SAFERELEASE(m_pVWClientObject);
	}
}

STDMETHODIMP CVWClientOCX::OnTrace(BSTR bstr)
{ 
	FireOnTrace(bstr);

	return S_OK;
}

STDMETHODIMP CVWClientOCX::OnReport(BSTR bstr, long lType)
{ 
	FireOnReport(bstr, lType);

	return S_OK;
}

STDMETHODIMP CVWClientOCX::OnConnect(IWorld* pWorld)
{ 
	FireOnConnect(pWorld, VARIANT_TRUE); 

	return S_OK;
}

STDMETHODIMP CVWClientOCX::OnDisconnect(IWorld* pWorld)
{ 
	FireOnConnect(pWorld, VARIANT_FALSE); 

	return S_OK;
}

STDMETHODIMP CVWClientOCX::OnUserConnect(IThing* pthing)
{ 
	FireOnUserConnect(pthing, VARIANT_TRUE); 

	return S_OK;
}

STDMETHODIMP CVWClientOCX::OnUserDisconnect(IThing* pthing)
{ 
	FireOnUserConnect(pthing, VARIANT_FALSE); 

	return S_OK;
}

STDMETHODIMP CVWClientOCX::OnUserReconnect(IVWClient* pVWClient, IWorld* pWorld, IThing* pthing)
{ 
	FireOnUserReconnect(pVWClient, pWorld, pthing); 

	return S_OK;
}

STDMETHODIMP CVWClientOCX::OnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg)
{ 
	FireOnUIEvent(pthing, bstrEventName, varArg); 

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// CVwcliocxCtrl message handlers

#include "version.h"

#if (rmm < 10)
#define rmmpad "0"
#else
#define rmmpad
#endif

#define VER_STR1(a,b,c)		#a ".00." rmmpad #b "." #c
#define VER_STR2(a,b,c)		VER_STR1(a,b,c)
#define VER_VERSION_STR		VER_STR2(rmj,rmm,rup)

BSTR CVWClientOCX::GetVersion()
{
	CString strVersion = VER_VERSION_STR;

	return strVersion.AllocSysString();
}


