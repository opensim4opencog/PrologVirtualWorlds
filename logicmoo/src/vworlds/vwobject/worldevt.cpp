// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// WorldEvt.cpp : Implementation of CfooApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include "worldobj.h"
#include "syshelp.h"
#include "vwevents.h"

#define SUPPORT_USER_CONTEXT

STDMETHODIMP CWorldObject::Advise(IUnknown* pUnkSink, DWORD* pdwCookie)
{
	IUnknown* p;
	HRESULT hRes = S_OK;
	if (pUnkSink == NULL || pdwCookie == NULL)
		return E_POINTER;
	IID iid;
	GetConnectionInterface(&iid);
	hRes = pUnkSink->QueryInterface(iid, (void**)&p);
	if (SUCCEEDED(hRes))
	{
		Lock();

		*pdwCookie = m_vec.Add(p);

#ifdef _DEBUG
		TRACE("CWorldObject::Advise: dwCookie 0x%0x, m_vec.m_nSize %d\n", *pdwCookie, m_vec.GetSize());
#endif

		hRes = (*pdwCookie != NULL) ? S_OK : CONNECT_E_ADVISELIMIT;
		Unlock();
		if (hRes != S_OK)
			p->Release();
	}
	else if (hRes == E_NOINTERFACE)
		hRes = CONNECT_E_CANNOTCONNECT;
	if (FAILED(hRes))
	{
		*pdwCookie = 0;
#ifdef _DEBUG
		TRACE("CWorldObject::Advise: failed, hr %x\n", hRes);
#endif
	}
	return hRes;
}

STDMETHODIMP CWorldObject::Unadvise(DWORD dwCookie)
{
	Lock();
	IUnknown* p = _CDV::GetUnknown(dwCookie);

#ifdef _DEBUG
	TRACE("CWorldObject::UnAdvise: dwCookie 0x%0x, m_vec.m_nSize %d\n", dwCookie, m_vec.GetSize());
#endif

	HRESULT hRes = m_vec.Remove(dwCookie) ? S_OK : CONNECT_E_NOCONNECTION;
	Unlock();
	if (hRes == S_OK && p != NULL)
		p->Release();
#ifdef _DEBUG
	if (FAILED(hRes))
		TRACE("CWorldObject::Unadvise: failed, hr %x\n", hRes);
#endif
	return hRes;
}

STDMETHODIMP CWorldObject::Report(BSTR bstr, long lType)
{
	HRESULT hr = S_OK;

	START_EVENT(CWorldObject, IVWObjectSite)
            /* hr = */ pEvent->OnReport(bstr, lType);
	FINISH_EVENT()

    return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::Print(VARIANT varAnything)
{
	HRESULT hr = S_OK;
	CComBSTR bstr;
	CComVariant varTemp;

	::VariantCopyInd(&varTemp, &varAnything);

	switch (varTemp.vt)
	{
	case VT_ERROR:
	case VT_EMPTY:
		bstr = "<empty>";
		break;

	case VT_DISPATCH:
		// check if it is an object property
		{
			IObjectProperty* pprop = NULL;

			// verify desired interface is supported
			if (varTemp.pdispVal != NULL && SUCCEEDED(varTemp.pdispVal->QueryInterface(IID_IObjectProperty, (LPVOID*)&pprop)))
			{
				if (pprop)
				{
					pprop->ToAscii(&bstr.m_str);
					SAFERELEASE(pprop);
				}
			}
		}
		break;

	default:
		{
			CComVariant varBSTR;

			::VariantChangeType(&varBSTR, &varTemp, 0, VT_BSTR);

			if (varBSTR.vt == VT_BSTR)
				bstr = varBSTR.bstrVal;
			else
				bstr = "<unknown>";
		}
	}

	Report(bstr, 0);

    return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::Trace(BSTR bstr)
{
	HRESULT hr = S_OK;
#ifdef _DEBUG
	// NOTE: if no-one is synced to IVWObjectSite, just use OutputDebugString
	VARIANT_BOOL bTrace = VARIANT_FALSE;
#endif

	START_EVENT(CWorldObject, IVWObjectSite)
            /* hr = */ pEvent->OnTrace(bstr);

#ifdef _DEBUG
			bTrace = VARIANT_TRUE;
#endif
	FINISH_EVENT()

#ifdef _DEBUG
	if (bTrace == VARIANT_FALSE)
		OutputDebugString(CString(bstr));
#endif

    return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::NotifyUserConnect(IThing* pthing)
{
    HRESULT hr = S_OK;

	START_EVENT(CWorldObject, IVWObjectSite)
            /* hr = */ pEvent->OnUserConnect(pthing);
	FINISH_EVENT()

    return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::NotifyUserDisconnect(IThing* pthing)
{
    HRESULT hr = S_OK;

	START_EVENT(CWorldObject, IVWObjectSite)
            /* hr = */ pEvent->OnUserDisconnect(pthing);
	FINISH_EVENT()

    return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::FireUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg)
{
	CComVariant varTemp;
    HRESULT hr = S_OK;

	if (FAILED(hr = ::VariantCopyInd(&varTemp, &varArg)))
		return ReportWorldError(hr);

#ifdef _DEBUG
	CComBSTR bstrName("n/a");

	if (pthing)
		pthing->get_Name(&bstrName.m_str);

	TRACE("CWorldObject::FireUIEvent: source %s, event %s\n", CString(bstrName), CString(bstrEventName));
#endif

	try
	{
		CSetUID SetUID(this, m_pUser, m_pUser);

#if !defined(TIMING_DEBUG)
		START_EVENT(CWorldObject, IVWObjectSite)

#ifdef _DEBUG
//				TRACE("CWorldObject::FireUIEvent: event (%s), pEvent 0x%0x, m_vec.m_nSize %d, varTemp 0x%x, varArg.vt %d, varTemp.vt %d\n", CString(bstrEventName), pEvent, m_vec.GetSize(), &varTemp, varArg.vt, varTemp.vt);
#endif

				/* hr = */ pEvent->OnUIEvent(pthing, bstrEventName, varTemp);
		FINISH_EVENT()
#endif
	}
	catch (...)
	{
#ifdef _DEBUG
		TRACE("CWorldObject::FireUIEvent: caught exception during OnUIEvent (%s)\n", CString(bstrEventName));
#endif
	}

    return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::ShowHTML(BSTR bstrTitle, BSTR bstrHTML, VARIANT cx, VARIANT cy)
{
	HRESULT hr = S_OK;
	CComVariant ccomvarCx(cx);
	CComVariant ccomvarCy(cy);

	// Convert the input parameters
	hr = ccomvarCx.ChangeType(VT_I4);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = ccomvarCy.ChangeType(VT_I4);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// Invoke the HTML pop-up
	hr = ShowHTMLExt(CComBSTR("_NEW"), bstrHTML, bstrTitle, VARIANT_TRUE, 
		NULL, 0, 0, ccomvarCx.lVal, ccomvarCy.lVal);

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CWorldObject::ShowHTMLEx(BSTR bstrWinName, BSTR bstrTitle, BSTR bstrHTML,
	VARIANT dispArg, VARIANT x,  VARIANT y, VARIANT cx, VARIANT cy)
{
	HRESULT hr = S_OK;
	CComVariant ccomvarX(x);
	CComVariant ccomvarY(y);
	CComVariant ccomvarCx(cx);
	CComVariant ccomvarCy(cy);
	CComVariant ccomvarDispArg(dispArg);

	// Convert the input parameters
	hr = ccomvarX.ChangeType(VT_I4);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = ccomvarY.ChangeType(VT_I4);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = ccomvarCx.ChangeType(VT_I4);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = ccomvarCy.ChangeType(VT_I4);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = ccomvarDispArg.ChangeType(VT_DISPATCH);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// Invoke the HTML pop-up
	hr = ShowHTMLExt(bstrWinName, bstrHTML, bstrTitle, VARIANT_FALSE,
		ccomvarDispArg.pdispVal, ccomvarX.lVal, ccomvarY.lVal, ccomvarCx.lVal, ccomvarCy.lVal);

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CWorldObject::ShowHTMLExt(		BSTR bstrWinName,	
											BSTR bstrHTML, 
											BSTR bstrTitle, 
											VARIANT_BOOL bSafeMode,
											IDispatch *pdispArg, 
											int x, 
											int y, 
											int cx, 
											int cy)
{
    HRESULT hr = S_OK;
	IPropertyList* pPropList = NULL;

	hr = CreatePropertyList(this, &pPropList);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// orphan the property list
	/* hr = */ pPropList->put_World(NULL);

	hr = pPropList->AddString(bstrWinName);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pPropList->AddString(bstrHTML);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pPropList->AddString(bstrTitle);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pPropList->AddBOOL(bSafeMode);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pPropList->AddDispatch(pdispArg);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pPropList->AddLong(x);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pPropList->AddLong(y);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pPropList->AddLong(cx);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pPropList->AddLong(cy);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = FireUIEvent(NULL, CComBSTR(VW_SHOWHTML_STR), CComVariant(pPropList));

ERROR_ENCOUNTERED:
	SAFERELEASE(pPropList);

    return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::ShowURLExt(		BSTR bstrWinName,
											VARIANT_BOOL bPopup,
											BSTR bstrURL, 
											BSTR bstrTitle, 
											VARIANT_BOOL bSafeMode,
											IDispatch *pdispArg,
											int x, 
											int y, 
											int cx, 
											int cy)
{
    HRESULT hr = S_OK;
	IPropertyList* pPropList = NULL;

	hr = CreatePropertyList(this, &pPropList);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// orphan the property list
	/* hr = */ pPropList->put_World(NULL);

	hr = pPropList->AddString(bstrWinName);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pPropList->AddBOOL(bPopup);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pPropList->AddString(bstrURL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pPropList->AddString(bstrTitle);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pPropList->AddBOOL(bSafeMode);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pPropList->AddDispatch(pdispArg);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pPropList->AddLong(x);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pPropList->AddLong(y);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pPropList->AddLong(cx);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pPropList->AddLong(cy);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = FireUIEvent(NULL, CComBSTR(VW_SHOWURL_STR), CComVariant(pPropList));

ERROR_ENCOUNTERED:
	SAFERELEASE(pPropList);

    return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::ShowURL(BSTR bstrTitle, BSTR bstrURL, VARIANT cx, VARIANT cy)
{
	HRESULT hr = S_OK;
	CComVariant ccomvarCx(cx);
	CComVariant ccomvarCy(cy);

	// Convert the input parameters
	hr = ccomvarCx.ChangeType(VT_I4);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = ccomvarCy.ChangeType(VT_I4);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// Invoke the URL pop-up
	hr = ShowURLExt(CComBSTR("_NEW"), VARIANT_TRUE, bstrURL, bstrTitle, VARIANT_TRUE, 
		NULL, 0, 0, ccomvarCx.lVal, ccomvarCy.lVal);

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CWorldObject::ShowURLEx(BSTR bstrWinName, BSTR bstrTitle, BSTR bstrURL,
	VARIANT dispArg, VARIANT x,  VARIANT y, VARIANT cx, VARIANT cy)
{
	HRESULT hr = S_OK;
	CComVariant ccomvarX(x);
	CComVariant ccomvarY(y);
	CComVariant ccomvarCx(cx);
	CComVariant ccomvarCy(cy);
	CComVariant ccomvarDispArg(dispArg);

	// Convert the input parameters
	hr = ccomvarX.ChangeType(VT_I4);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = ccomvarY.ChangeType(VT_I4);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = ccomvarCx.ChangeType(VT_I4);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = ccomvarCy.ChangeType(VT_I4);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = ccomvarDispArg.ChangeType(VT_DISPATCH);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// Invoke the URL pop-up
	hr = ShowURLExt(bstrWinName, VARIANT_TRUE, bstrURL, bstrTitle, VARIANT_FALSE, 
		ccomvarDispArg.pdispVal, ccomvarX.lVal, ccomvarY.lVal, ccomvarCx.lVal, ccomvarCy.lVal);

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CWorldObject::NotifyInvokeMethod(IThing* pthing, BSTR bstr, DISPPARAMS* pdispparams)
{
    HRESULT hr = S_OK;
#ifdef SUPPORT_USER_CONTEXT
	// force user's security context
	CSetUID SetUID(this, m_pUser, m_pUser);
#endif

/* begin DougM */

	CComPtr<IPropertyList> plistArgs = NULL; //(dougm)
	CComVariant varHintData;

		// create plistArgs 
	hr =  CreatePropertyList(m_pWorld, &plistArgs);

		// Copy the dispparams into plistArgs
	hr =  ConvertDISPPARAMSToPropertyList(pdispparams, plistArgs);

	varHintData = CComVariant(plistArgs);
	
	hr = S_OK;


/* End DougM */


#if !defined(TIMING_DEBUG)
	START_EVENT(CWorldObject, IVWObjectSite)
            /* hr = */ pEvent->OnInvokeMethod(pthing, bstr, pdispparams);
            /* hr = */ pEvent->OnPropertyChanged(pthing, bstr, 999, varHintData); 	//DougM: Send It Out anotherway
	FINISH_EVENT()
#endif

    return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::NotifyPropertyChanged(IThing* pthing, BSTR bstr, long lHint, VARIANT varHintData)
{
    HRESULT hr = S_OK;
#ifdef SUPPORT_USER_CONTEXT
	// force user's security context
	CSetUID SetUID(this, m_pUser, m_pUser);
#endif

#if !defined(TIMING_DEBUG)
	START_EVENT(CWorldObject, IVWObjectSite)
            /* hr = */ pEvent->OnPropertyChanged(pthing, bstr, lHint, varHintData);
	FINISH_EVENT()
#endif

    return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::NotifyMethodChanged(IThing* pthing, BSTR bstr, long lHint, VARIANT varHintData)
{
    HRESULT hr = S_OK;
#ifdef SUPPORT_USER_CONTEXT
	// force user's security context
	CSetUID SetUID(this, m_pUser, m_pUser);
#endif

#if !defined(TIMING_DEBUG)
	START_EVENT(CWorldObject, IVWObjectSite)
            /* hr = */ pEvent->OnMethodChanged(pthing, bstr, lHint, varHintData);
	FINISH_EVENT()
#endif

    return ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::NotifyModuleChanged(BSTR bstr, long lHint, VARIANT varHintData)
{
    HRESULT hr = S_OK;
#ifdef SUPPORT_USER_CONTEXT
	// force user's security context
	CSetUID SetUID(this, m_pUser, m_pUser);
#endif

#if !defined(TIMING_DEBUG)
	START_EVENT(CWorldObject, IVWObjectSite)
            /* hr = */ pEvent->OnModuleChanged(bstr, lHint, varHintData);
	FINISH_EVENT()
#endif

    return ReportWorldError(hr);
}

