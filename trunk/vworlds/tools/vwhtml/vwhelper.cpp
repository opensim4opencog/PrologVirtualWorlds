// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWHelper.cpp : Implementation of CVWHtmlHelperObject

#include "stdafx.h"
#include "mshtml.h"
#include <VWHtml.h>
#include "VWHelper.h"

/////////////////////////////////////////////////////////////////////////////
//

CVWHtmlHelperObject::CVWHtmlHelperObject() : 
	m_pclient(NULL), 
	m_pdispArg(NULL) 
{
}

CVWHtmlHelperObject::~CVWHtmlHelperObject()
{
	SAFERELEASE(m_pclient);
	SAFERELEASE(m_pdispArg);
}


STDMETHODIMP CVWHtmlHelperObject::GetShortPathName(BSTR bstrLongName, BSTR *pbstrShortName)
{
	CString	strLongName = bstrLongName;
	CString	strShortName;
	char	*buf = strShortName.GetBuffer(1024);

	::GetShortPathName(strLongName, buf, 1024);

	strShortName.ReleaseBuffer();

	*pbstrShortName = strShortName.AllocSysString();

	return S_OK;
}


STDMETHODIMP CVWHtmlHelperObject::CloseWindow()
{
	HRESULT hr = S_OK;
	CComPtr<IOleClientSite> pClientSite;
	CComPtr<IOleContainer> pContainer;

	// Get the document
	hr = this->GetClientSite(&pClientSite);
	if (FAILED(hr)) return hr;
	
	hr = pClientSite->GetContainer(&pContainer);
	if (FAILED(hr)) return hr;

	CComQIPtr<IHTMLDocument2, &IID_IHTMLDocument2> pDoc(pContainer);

	if (pDoc)
	{
		CComPtr<IOleWindow>	pRealWind;

		hr = pDoc->QueryInterface(IID_IOleWindow, (void **)&pRealWind);

		if (SUCCEEDED(hr) && pRealWind)
		{
			HWND	hwnd;
			
			hr = pRealWind->GetWindow(&hwnd);
			if (FAILED(hr))
				return hr;
			else
			{
				CWnd		*pcwnd;
				CWnd		*parent;

				pcwnd = CWnd::FromHandle(hwnd);

				parent = pcwnd->GetParentOwner();
				
				if (parent)
//					parent->DestroyWindow();
					parent->PostMessage(WM_CLOSE);

				return S_OK;
			}
		}
//		else
//			::DestroyWindow(::GetActiveWindow());

		::CoFreeUnusedLibraries();
	}

	return E_FAIL;
}

HRESULT CVWHtmlHelperObject::GetPtrsFromContainer()
{
	HRESULT hr = S_OK;
	CComPtr<IOleClientSite> pClientSite;
	CComPtr<IOleContainer> pContainer;

	// If we've already done this, skip it
	if (m_pclient || m_pdispArg)
		return S_OK;

	// Get the document
	hr = this->GetClientSite(&pClientSite);
	if (FAILED(hr)) return hr;
	
	hr = pClientSite->GetContainer(&pContainer);
	if (FAILED(hr)) return hr;

	CComQIPtr<IHTMLDocument2, &IID_IHTMLDocument2> pDoc(pContainer);

	if (pDoc)
	{
		CComPtr<IHTMLWindow2> pWindow;

		// Get the top window
		hr = pDoc->get_parentWindow(&pWindow);
		if (FAILED(hr)) return hr;

		CComPtr<IHTMLWindow2> pTopWindow;

		hr = pWindow->get_top(&pTopWindow);
		if (FAILED(hr)) return hr;

		CComQIPtr<IDispatch, &IID_IDispatch> pDispatch(pTopWindow);

		// Call GetClientArg
		CComBSTR bstrMethod = "GetClientArg";
		DISPID dispid;
		DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
		CComVariant varResult;

		hr = pDispatch->GetIDsOfNames(IID_NULL, &bstrMethod.m_str, 1,
										LOCALE_USER_DEFAULT, &dispid);
		if (FAILED(hr)) return hr;

		hr = pDispatch->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD,
							&dispparamsNoArgs, &varResult, NULL, NULL);

		// If it fails, try getting the anchor input instead
		if (FAILED(hr) || (varResult.vt != VT_BSTR))
		{
			CComPtr<IHTMLLocation> pLocation;

			// varResult = document.location.hash
			hr = pDoc->get_location(&pLocation);
			if (FAILED(hr)) return hr;
			varResult.Clear();	// in case GetClientArg returned something weird
			hr = pLocation->get_hash(&varResult.bstrVal);
			if (FAILED(hr)) return hr;
			varResult.vt = VT_BSTR;
		}
		
		// Parse "#xxxxxxxx_xxxxxxxx" into m_pclient and m_pdispArg
		CString cstringAnchor = varResult.bstrVal;
		int i = cstringAnchor.Find('_');

		if (i > 0)
		{
			// Set the client and arg
			// Note: make sure they've got the right interfaces; otherwise this control could
			// be used to gain uncontrolled access to objects from an unfriendly web page
			unsigned long ul;
			CComQIPtr<IVWClient, &IID_IVWClient> pClient;
			ul = strtoul(cstringAnchor.Mid(1, i-1), NULL, 16);
			if (ul!=0)
			{
				pClient = (IUnknown *) (void *) ul;
				ASSERT(pClient);
				m_pclient = pClient;
				SAFEADDREF(m_pclient);
			}
			else
				m_pclient = NULL;

			CComQIPtr<IDispatch, &IID_IDispatch> pdispArg;
			ul = strtoul(cstringAnchor.Mid(i+1, cstringAnchor.GetLength()-i-1), NULL, 16);
			if (ul!=0)
			{
				pdispArg = (IUnknown *) (void *) ul;
				ASSERT(pdispArg);
				m_pdispArg = pdispArg;
				SAFEADDREF(m_pdispArg);
			}
			else
				m_pdispArg = NULL;
		}
	}

	return S_OK;
}

STDMETHODIMP CVWHtmlHelperObject::get_Client(IVWClient **ppclient) 
{
	HRESULT hr = S_OK;

	hr = GetPtrsFromContainer();
	if (SUCCEEDED(hr))
	{
		*ppclient = m_pclient; 
		SAFEADDREF(m_pclient);
	}
	else
	{
		TRACE("VWHtmlHelperObject: Failed to get client object.\n");
	}

	return hr;
}

STDMETHODIMP CVWHtmlHelperObject::get_Arg(IDispatch **ppdispArg)
{
	HRESULT hr = S_OK;

	hr = GetPtrsFromContainer();
	if (SUCCEEDED(hr))
	{
		*ppdispArg = m_pdispArg;
		SAFEADDREF(m_pdispArg);
	}
	else
	{
		TRACE("VWHtmlHelperObject: Failed to get argument.\n");
	}

	return hr;
}
