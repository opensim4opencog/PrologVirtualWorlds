// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include <wininet.h>
#include <dispex.h>
#include <afxtempl.h>

#include <atlbase.h>
#include <atlcom.h>
#include <string.h>
#include <vwobject.h>
#include "propbase.h"

#ifdef _DEBUG
void _cdecl VWTrace(IWorld* pworld, LPCTSTR lpszComponent, int iLevel, LPCTSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);

//	int nBuf;
	TCHAR szBuffer[4096];

	try
	{
		/* nBuf = */ _vsntprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
//		ASSERT(nBuf < sizeof(szBuffer));
	}
	catch (...)
	{
		// caught low-level exception in _vsntprintf
		goto ERROR_ENCOUNTERED;
	}

	// NOTE: added OutputDebugString in VWClient::OnTrace
	// so we don't need it here
	if (pworld)
	{
		VARIANT_BOOL bTrace = VARIANT_FALSE;

		pworld->get_IsTraceEnabled(&bTrace);

		if (bTrace)
		{
			int iCoLevel = 0;
			CString strBuffer(szBuffer);
			BSTR bstrBuffer = strBuffer.AllocSysString();

			CString strComponent(lpszComponent);
			BSTR bstrComponent = strComponent.AllocSysString();

			CString strAsterisk("*");
			BSTR bstrAsterisk = strAsterisk.AllocSysString();

			if (SUCCEEDED(pworld->get_TraceLevel(bstrComponent, &iCoLevel)) ||
				SUCCEEDED(pworld->get_TraceLevel(bstrAsterisk, &iCoLevel)))
			{
				if (iLevel >= iCoLevel)
					pworld->Trace(bstrBuffer);
			}
			else
			{
				if (iLevel >= VWT_ERROR)
					pworld->Trace(bstrBuffer);
			}

			SAFEFREESTRING(bstrBuffer);
			SAFEFREESTRING(bstrComponent);
			SAFEFREESTRING(bstrAsterisk);
		}
		else
		{
			// if no world, only output errors
			if (iLevel >= VWT_ERROR)
				OutputDebugString(szBuffer);
		}
	}
	else
	{
		// if no world, only output errors
		if (iLevel >= VWT_ERROR)
			OutputDebugString(szBuffer);
	}

ERROR_ENCOUNTERED:
	va_end(args);
}
#endif //_DEBUG

void _cdecl VWReport(IWorld* pworld, long lHint, LPCTSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);

//	int nBuf;
	TCHAR szBuffer[4096];

	/* nBuf = */ _vsntprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);
//	ASSERT(nBuf < sizeof(szBuffer));

	// NOTE: added OutputDebugString in VWClient::OnReport
	// so we don't need it here
	if (pworld)
	{
		CString strBuffer(szBuffer);
		BSTR bstrBuffer = strBuffer.AllocSysString();

		pworld->Report(bstrBuffer, lHint);

		SAFEFREESTRING(bstrBuffer);
	}
	else
	{
		// if no world, only output errors
		OutputDebugString(szBuffer);
	}

	va_end(args);
}

// NOTE: these methods found in inline form in propbase.h; don't forget to change both places
#ifndef INLINE_VW_HELPERS

void VWLPTSTRFromCLSID(REFCLSID rclsid, LPTSTR pszCLSID)
{
	wsprintf(pszCLSID, _T("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"),
		rclsid.Data1, rclsid.Data2, rclsid.Data3,
		rclsid.Data4[0], rclsid.Data4[1], rclsid.Data4[2], rclsid.Data4[3],
		rclsid.Data4[4], rclsid.Data4[5], rclsid.Data4[6], rclsid.Data4[7]);
}

CString VWStringFromCLSID(REFCLSID rclsid)
{
	TCHAR szCLSID[256];
	wsprintf(szCLSID, _T("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"),
		rclsid.Data1, rclsid.Data2, rclsid.Data3,
		rclsid.Data4[0], rclsid.Data4[1], rclsid.Data4[2], rclsid.Data4[3],
		rclsid.Data4[4], rclsid.Data4[5], rclsid.Data4[6], rclsid.Data4[7]);
	return szCLSID;
}

CComBSTR VWBSTRFromCLSID(REFCLSID rclsid)
{
	TCHAR szCLSID[256];
	wsprintf(szCLSID, _T("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"),
		rclsid.Data1, rclsid.Data2, rclsid.Data3,
		rclsid.Data4[0], rclsid.Data4[1], rclsid.Data4[2], rclsid.Data4[3],
		rclsid.Data4[4], rclsid.Data4[5], rclsid.Data4[6], rclsid.Data4[7]);
	return szCLSID;
}

CComBSTR VWBSTRFromLong(long x)
{
	char temp[20];
	_ltoa( x, temp, 10 );

	return CComBSTR(temp);
}

CComBSTR NormalizeBSTR(BSTR bstr)
{
	CString temp(bstr);
	temp.MakeLower();

	return CComBSTR(temp);
}

HRESULT InitializeDISPPARAMS(DISPPARAMS* pdispparams, int propcnt)
{
	ASSERT(pdispparams);
	if (pdispparams == NULL)
		return E_INVALIDARG;

	memset(pdispparams, 0x0, sizeof(DISPPARAMS));

	if (propcnt)
	{
		pdispparams->rgvarg = new VARIANT[propcnt];
		if (pdispparams->rgvarg == NULL)
			return E_OUTOFMEMORY;

		pdispparams->cArgs = propcnt;

		for (int i = 0; i < propcnt; i++)
		{
			memset(&pdispparams->rgvarg[i], 0x0, sizeof(VARIANT));

//			::VariantInit(&pdispparams->rgvarg[i]);
			pdispparams->rgvarg[i].vt = VT_EMPTY;
		}
	}

	return S_OK;
}

HRESULT TerminateDISPPARAMS(VARIANT_BOOL bClear, DISPPARAMS* pdispparams)
{
	ASSERT(pdispparams);
	if (pdispparams == NULL)
		return E_INVALIDARG;

	if (pdispparams->rgvarg)
	{
		if (bClear == VARIANT_TRUE)
		{
			for (unsigned int i = 0; i < pdispparams->cArgs; i++)
			{
				// REVIEW:  (dv) Check for BLOBs and treat them as BSTRs
				if (pdispparams->rgvarg[i].vt == VT_BLOB)
					pdispparams->rgvarg[i].vt = VT_BSTR;
				::VariantClear(&pdispparams->rgvarg[i]);
			}
		}

		delete [] pdispparams->rgvarg;
		pdispparams->rgvarg = NULL;
	}

	memset(pdispparams, 0x0, sizeof(DISPPARAMS));

	return S_OK;
}

#endif

HRESULT ConvertJSArrayToDISPPARAMS(VARIANT* pvarArgs, DISPPARAMS* pdispparams)
{
	ASSERT(pvarArgs);
	if (pvarArgs == NULL)
		return E_INVALIDARG;

	ASSERT(pdispparams);
	if (pdispparams == NULL)
		return E_POINTER;

	if (pvarArgs->vt != VT_DISPATCH || pvarArgs->pdispVal == NULL)
		return E_INVALIDARG;

	long propcnt = 0;
	HRESULT hr = S_OK;
	IDispatchEx* pdispEx = NULL;
	CComVariant var;
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
	DISPID dispid;

	hr = pvarArgs->pdispVal->QueryInterface(IID_IDispatchEx, (void**)&pdispEx);
	if (FAILED(hr) || pdispEx == NULL)
		goto ERROR_ENCOUNTERED;

	hr = pdispEx->GetNextDispID(fdexEnumAll, DISPID_STARTENUM, &dispid);
	while (hr != S_FALSE)
	{
		// calculate number of properties
		propcnt++;

		hr = pdispEx->GetNextDispID(fdexEnumAll, dispid, &dispid);
	}

	hr = InitializeDISPPARAMS(pdispparams, propcnt);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	propcnt = 0;

	hr = pdispEx->GetNextDispID(fdexEnumAll, DISPID_STARTENUM, &dispid);
	while (hr != S_FALSE)
	{
		pdispEx->InvokeEx(dispid, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dispparamsNoArgs,
			&var, NULL, NULL);

		hr = VariantCopyInd(&pdispparams->rgvarg[propcnt++], &var);   

		// free data
		var.Clear();

		hr = pdispEx->GetNextDispID(fdexEnumAll, dispid, &dispid);
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pdispEx);

	return hr;
}

HRESULT ConvertSAFEARRAYToDISPPARAMS(VARIANT* pvarArgs, DISPPARAMS* pdispparams)
{
	ASSERT(pvarArgs);
	if (pvarArgs == NULL)
		return E_INVALIDARG;

	ASSERT(pdispparams);
	if (pdispparams == NULL)
		return E_POINTER;

	long propcnt = 0;
	HRESULT hr = S_OK;
	VARIANT v;
	long lLBound = 0, lUBound = 0;
	long i = 0, l = 0;
	SAFEARRAY* psaArguments = NULL;

	// we have a SAFEARRAY
	if (pvarArgs->vt == (VT_VARIANT | VT_ARRAY))
	{
		psaArguments = (SAFEARRAY*)pvarArgs->parray;

		// Verify that the SafeArray is the proper shape.
		if (SafeArrayGetDim(psaArguments) != 1)
		{
			hr = E_INVALIDARG;
			goto ERROR_ENCOUNTERED;
		}

		// Get array bounds    
		hr = SafeArrayGetLBound(psaArguments, 1, &lLBound);     
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = SafeArrayGetUBound(psaArguments, 1, &lUBound);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED; 

		propcnt = lUBound - lLBound + 1;
	}
	// optional arg
	else if (pvarArgs->vt == VT_ERROR)
		propcnt = 0;
	else
		propcnt = 1;

	hr = InitializeDISPPARAMS(pdispparams, propcnt);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (psaArguments != NULL)
	{
		// Loop through the array retrieving parameters
		VariantInit(&v);  

		for (i = 0, l = lLBound; l <= lUBound; i++, l++)
		{
			hr = SafeArrayGetElement(psaArguments, &l, &v);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;   
        
			// Check if optional/variable parameter was passed. 
			// VT_ERROR/DISP_E_PARAMNOTFOUND indicates that the parameter was not passed by the controller.
			if (V_VT(&v) == VT_ERROR && V_ERROR(&v) == DISP_E_PARAMNOTFOUND)
			{
				hr = ResultFromScode(DISP_E_PARAMNOTFOUND);   // No parameter passed
				goto ERROR_ENCOUNTERED;
			}

			// If a parameter was passed by reference, get the value after de-reference.  
			// VBA will pass variables by reference.
			hr = VariantCopyInd(&pdispparams->rgvarg[propcnt - i - 1], &v);   
			if (FAILED(hr))
			{    
				// Parameter not passed. This corresponds to an example like Obj.Add(1, 2, , 4), 
				// where the third parameter is not passed. Ignore and continue.
				if (GetScode(hr) == DISP_E_PARAMNOTFOUND)   
					continue;
				else 
					goto ERROR_ENCOUNTERED;  // Other error
			}

			VariantClear(&v); 
		}
	}
	else if (propcnt)
	{
		hr = VariantCopyInd(&pdispparams->rgvarg[0], pvarArgs);   
	}

ERROR_ENCOUNTERED:
	return hr;
}

HRESULT ConvertJSArrayToPropertyList(VARIANT* pvarArgs, IPropertyList* pproplist)
{
	ASSERT(pvarArgs);
	if (pvarArgs == NULL)
		return E_INVALIDARG;

	ASSERT(pproplist);
	if (pproplist == NULL)
		return E_POINTER;

	if (pvarArgs->vt != VT_DISPATCH || pvarArgs->pdispVal == NULL)
		return E_INVALIDARG;

	HRESULT hr = S_OK;
	IDispatchEx* pdispEx = NULL;
	CComVariant var;
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
	DISPID dispid;

	hr = pvarArgs->pdispVal->QueryInterface(IID_IDispatchEx, (void**)&pdispEx);
	if (FAILED(hr) || pdispEx == NULL)
		goto ERROR_ENCOUNTERED;

	hr = pdispEx->GetNextDispID(fdexEnumAll, DISPID_STARTENUM, &dispid);
	while (hr != S_FALSE)
	{
		pdispEx->InvokeEx(dispid, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dispparamsNoArgs,
			&var, NULL, NULL);

		hr = pproplist->Add(var);

		// free data
		var.Clear();

		hr = pdispEx->GetNextDispID(fdexEnumAll, dispid, &dispid);
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pdispEx);

	return hr;
}

HRESULT ConvertSAFEARRAYToPropertyList(VARIANT* pvarArgs, IPropertyList* pproplist)
{
	ASSERT(pvarArgs);
	if (pvarArgs == NULL)
		return E_INVALIDARG;

	ASSERT(pproplist);
	if (pproplist == NULL)
		return E_POINTER;

	long propcnt = 0;
	HRESULT hr = S_OK;
	long lLBound = 0, lUBound = 0;
	long i = 0, l = 0;
	SAFEARRAY* psaArguments = NULL;

	// clear out existing properties
	pproplist->RemoveAll();

	// we have a SAFEARRAY
	if (pvarArgs->vt == (VT_VARIANT | VT_ARRAY))
	{
		psaArguments = (SAFEARRAY*)pvarArgs->parray;

		// Verify that the SafeArray is the proper shape.
		if (SafeArrayGetDim(psaArguments) != 1)
		{
			hr = E_INVALIDARG;
			goto ERROR_ENCOUNTERED;
		}

		// Get array bounds    
		hr = SafeArrayGetLBound(psaArguments, 1, &lLBound);     
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = SafeArrayGetUBound(psaArguments, 1, &lUBound);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED; 

		propcnt = lUBound - lLBound + 1;
	}
	// optional arg
	else if (pvarArgs->vt == VT_ERROR)
		propcnt = 0;
	else
		propcnt = 1;

	if (psaArguments != NULL)
	{
		// Loop through the array retrieving parameters
		for (i = 0, l = lLBound; l <= lUBound; i++, l++)
		{
			CComVariant v;
			CComVariant varTemp;

			hr = SafeArrayGetElement(psaArguments, &l, &v);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;   
        
			// Check if optional/variable parameter was passed. 
			// VT_ERROR/DISP_E_PARAMNOTFOUND indicates that the parameter was not passed by the controller.
			if (V_VT(&v) == VT_ERROR && V_ERROR(&v) == DISP_E_PARAMNOTFOUND)
			{
				hr = ResultFromScode(DISP_E_PARAMNOTFOUND);   // No parameter passed
				goto ERROR_ENCOUNTERED;
			}

			// If a parameter was passed by reference, get the value after de-reference.  
			// VBA will pass variables by reference.
			hr = VariantCopyInd(&varTemp, &v);   
			if (FAILED(hr))
			{    
				// Parameter not passed. This corresponds to an example like Obj.Add(1, 2, , 4), 
				// where the third parameter is not passed. Ignore and continue.
				if (GetScode(hr) == DISP_E_PARAMNOTFOUND)   
					continue;
				else 
					goto ERROR_ENCOUNTERED;  // Other error
			}

			hr = pproplist->Add(varTemp);

			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}
	else if (propcnt)
	{
		CComVariant varTemp;

		hr = VariantCopyInd(&varTemp, pvarArgs);   
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pproplist->Add(varTemp);

		VariantClear(&varTemp);
	}

ERROR_ENCOUNTERED:
	return hr;
}

HRESULT ConvertPropertyListToDISPPARAMS(IPropertyList* ppropertylist, DISPPARAMS* pdispparams)
{
	// NOTE: ppropertylist can be NULL

	ASSERT(pdispparams);
	if (pdispparams == NULL)
		return E_POINTER;

	HRESULT hr = S_OK;
	long propcnt = 0;
	long modtype = 0;
	int i;

	memset(pdispparams, 0x0, sizeof(DISPPARAMS));

	if (ppropertylist)
	{
		hr = ppropertylist->get_Count(&propcnt);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	hr = InitializeDISPPARAMS(pdispparams, propcnt);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// use number of args from proplist
	for (i = 0; i < propcnt && SUCCEEDED(hr); i++)
	{
		// get_Property inits the variant

		// DISPPARAMS stored in reverse order
		// i = 0, propcnt = 2, index = 1
		// i = 1, propcnt = 2, index = 0

		hr = ppropertylist->get_Property(propcnt - i - 1, &pdispparams->rgvarg[i]);
	}

ERROR_ENCOUNTERED:
	return hr;
}

HRESULT ConvertDISPPARAMSToPropertyList(DISPPARAMS* pdispparams, IPropertyList* ppropertylist)
{
	// NOTE: pdispparams can be NULL

	ASSERT(ppropertylist);
	if (ppropertylist == NULL)
		return E_POINTER;

	// clear out existing properties
	ppropertylist->RemoveAll();

	if (pdispparams)
	{
		HRESULT hr = S_OK;
		int propcnt = pdispparams->cArgs;

		// DISPPARAMS stored in reverse order
		// i = 0, propcnt = 2, index = 1
		// i = 1, propcnt = 2, index = 0
		for (int i = 0; i < propcnt && SUCCEEDED(hr); i++)
			hr = ppropertylist->Add(pdispparams->rgvarg[propcnt - i - 1]);

		return hr;
	}

	return S_OK;
}

HRESULT CloneDISPPARAMS(DISPPARAMS* pfrom, DISPPARAMS* pto)
{
	// NOTE: pfrom can be NULL

	ASSERT(pto);
	if (pto == NULL)
		return E_POINTER;

	HRESULT hr = S_OK;
	int propcnt = 0;
	
	if (pfrom)
		propcnt = pfrom->cArgs;

	hr = InitializeDISPPARAMS(pto, propcnt);
	if (FAILED(hr))
		return hr;

	for (int i = 0; i < propcnt && SUCCEEDED(hr); i++)
		hr = ::VariantCopy(&pto->rgvarg[i], &pfrom->rgvarg[i]);

	return hr;
}

#ifndef OLE2ANSI
HRESULT VWReportError(LPCSTR lpszCLSID, LPCSTR lpszDesc,
	const IID& iid, HRESULT hRes)
{
	USES_CONVERSION;
	return VWReportError(A2CW(lpszCLSID), A2CW(lpszDesc), iid, hRes);
}
#endif

HRESULT VWReportError(LPCOLESTR lpszCLSID, LPCOLESTR lpszDesc,
	const IID& iid, HRESULT hRes)
{
	CComPtr<IErrorInfo> pErrorInfo;
	CComPtr<ICreateErrorInfo> pICEI;

#if 0
	::GetErrorInfo(0L, &pErrorInfo);

	// don't overwrite existing errorinfo
	if (pErrorInfo == NULL)
	{
#endif
		if (SUCCEEDED(CreateErrorInfo(&pICEI)))
		{
			pICEI->SetGUID(iid);
			pICEI->SetSource((LPOLESTR)lpszCLSID);
			pICEI->SetDescription((LPOLESTR)lpszDesc);

			if (SUCCEEDED(pICEI->QueryInterface(IID_IErrorInfo, (void**)&pErrorInfo)))
				::SetErrorInfo(0, pErrorInfo);
		}
#if 0
	}
	else
	{
		// have to push existing errorinfo back, otherwise it gets cleared
		::SetErrorInfo(0, pErrorInfo);
	}
#endif
	return (hRes == 0) ? DISP_E_EXCEPTION : hRes;
}

///////////////////////////////////////////////////////////////////////////////
// BSTR support for template collections

// NOTE: these methods found in inline form in propbase.h; don't forget to change both places
#ifndef INLINE_VW_HELPERS

UINT AFXAPI HashKey(BSTR key)
{
	UINT nHash = 0;
	wchar_t c;

	if (key == NULL)
		return 0;

	while (*key)
	{
		c = *(const wchar_t *)key;

		nHash = (nHash<<5) + nHash + VWToLower(c);
		key++;
	}
	return nHash;
}

BOOL AFXAPI CompareElements(const CComBSTR* pElement1, const BSTR* pElement2)
{
	ASSERT(AfxIsValidAddress(pElement1, sizeof(CComBSTR), FALSE));
	ASSERT(AfxIsValidAddress(pElement2, sizeof(BSTR), FALSE));

	// check for E_POINTER
	if (pElement1 == NULL || pElement2 == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (pElement1->m_str == NULL && *pElement2 == NULL)
		return TRUE;
	else if (pElement1->m_str == NULL || *pElement2 == NULL)
		return FALSE;

	wchar_t e1 = *(const wchar_t *)pElement1->m_str;
	wchar_t e2 = *(const wchar_t *)*pElement2;

	e1 = VWToLower(e1);
	e2 = VWToLower(e2);

	// REVIEW: test first byte equivalence, then string length, then actual string equivalence
	return (e1 == e2) &&
		(::SysStringByteLen(pElement1->m_str) == ::SysStringByteLen(*pElement2)) &&
		(::_wcsicmp((const wchar_t *)pElement1->m_str, (const wchar_t *)*pElement2) == 0);
}

BOOL AFXAPI CompareElements(BSTR* pElement1, BSTR* pElement2)
{
	ASSERT(AfxIsValidAddress(pElement1, sizeof(BSTR)));
	ASSERT(AfxIsValidAddress(pElement2, sizeof(BSTR)));

	// check for E_POINTER
	if (pElement1 == NULL || pElement2 == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (*pElement1 == NULL && *pElement2 == NULL)
		return TRUE;
	else if (*pElement1 == NULL || *pElement2 == NULL)
		return FALSE;

	wchar_t e1 = *(const wchar_t *)*pElement1;
	wchar_t e2 = *(const wchar_t *)*pElement2;

	e1 = VWToLower(e1);
	e2 = VWToLower(e2);

	// REVIEW: test first byte equivalence, then string length, then actual string equivalence
	return (e1 == e2) &&
		(::SysStringByteLen(*pElement1) == ::SysStringByteLen(*pElement2)) &&
		(::_wcsicmp((const wchar_t *)*pElement1, (const wchar_t *)*pElement2) == 0);
}
BOOL AFXAPI CompareElements(const CBSTRAdapt<CComBSTR>* pElement1, const BSTR* pElement2)
{
	ASSERT(AfxIsValidAddress(pElement1, sizeof(CComBSTR), FALSE));
	ASSERT(AfxIsValidAddress(pElement2, sizeof(BSTR), FALSE));

	CComBSTR* pAdaptElement1 = (CComBSTR*)pElement1;

	// check for E_POINTER
	if (pAdaptElement1 == NULL || pElement2 == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (pAdaptElement1->m_str == NULL && *pElement2 == NULL)
		return TRUE;
	else if (pAdaptElement1->m_str == NULL || *pElement2 == NULL)
		return FALSE;

	wchar_t e1 = *(const wchar_t *)pAdaptElement1->m_str;
	wchar_t e2 = *(const wchar_t *)*pElement2;

	e1 = VWToLower(e1);
	e2 = VWToLower(e2);

	// REVIEW: test first byte equivalence, then string length, then actual string equivalence
	return (e1 == e2) &&
		(::SysStringByteLen(pAdaptElement1->m_str) == ::SysStringByteLen(*pElement2)) &&
		(::_wcsicmp((const wchar_t *)pAdaptElement1->m_str, (const wchar_t *)*pElement2) == 0);
}
#endif