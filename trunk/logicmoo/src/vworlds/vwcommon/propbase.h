// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// PropBase.h : Declaration of the PROPERTY_BASE macros

#ifndef __PROPBASE_H
#define __PROPBASE_H

#include "vwtempl.h"

#define nDefaultSocketPort		2525

EXTERN_C const IID LIBID_VWSYSTEMLib;

interface IWorld;
interface IPropertyList;

#pragma warning( disable : 4786 )

// define to turn off security checking
//#define DISABLE_SECURITY
//#define ENABLE_VW_MACROS

#define INLINE_VW_HELPERS

#define VWToLower(c) (((c >= L'A') && (c <= L'Z')) ? (c - L'A' + L'a') : c)

#ifdef _DEBUG
void _cdecl VWTrace(IWorld* pworld, LPCTSTR lpszComponent, int iLevel, LPCTSTR lpszFormat, ...);
#define VWTRACE VWTrace
#else
inline void _cdecl VWTrace(IWorld* pworld, LPCTSTR lpszComponent, int iLevel, LPCTSTR lpszFormat, ...) {};
#define VWTRACE 1 ? (void)0 : VWTrace
#endif //_DEBUG

void _cdecl VWReport(IWorld* pworld, long lHint, LPCTSTR lpszFormat, ...);
#define VWREPORT VWReport

HRESULT VWReportError(LPCSTR lpszCLSID, LPCSTR lpszDesc,
	const IID& iid, HRESULT hRes);
HRESULT VWReportError(LPCOLESTR lpszCLSID, LPCOLESTR lpszDesc,
	const IID& iid, HRESULT hRes);

inline HRESULT ReportError(LPSTR lpszCLSID, REFIID iid, HRESULT hr, HRESULT hrLower, HRESULT hrUpper, IWorld *pWorld = NULL, int iLevel = 0, LPCTSTR lpszComponent = "")
{
	HLOCAL lpMsgBuf = NULL;

	if (HRESULT_FACILITY(hr) == FACILITY_ITF && 
		HRESULT_CODE(hr) >= 0x0200)
	{
		if (HRESULT_CODE(hr) >= hrLower && HRESULT_CODE(hr) < hrUpper)
		{
			AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

			// pick off last 4 hex digits of HRESULT
			// match these to resource string table
			int resid = HRESULT_CODE(hr);

			CString strError;
			strError.LoadString(resid);

			VWReportError(lpszCLSID, (LPCTSTR)strError, iid, hr);

#ifdef _DEBUG
			if (pWorld)
				VWTRACE(pWorld, lpszComponent, iLevel, "%s::ReportError: hr %x, %s\n", lpszCLSID, hr, (LPCTSTR)strError);
			else
				TRACE("%s::ReportError: hr %x, %s\n", lpszCLSID, hr, (LPCTSTR)strError);
#endif
		}
	}
	// If there's a message associated with this error, report that
	else if (::FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
			NULL, (DWORD)hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf, 0, NULL)
		> 0)
	{
		VWReportError(lpszCLSID, (LPCTSTR)lpMsgBuf, iid, hr);

#ifdef _DEBUG
		if (pWorld)
			VWTRACE(pWorld, lpszComponent, iLevel, "%s::ReportError: hr %x, %s\n", lpszCLSID, hr, (LPCTSTR)lpMsgBuf);
		else
			TRACE("%s::ReportError: hr %x, %s\n", lpszCLSID, hr, (LPCTSTR)lpMsgBuf);
#endif

		// Free the buffer, which was allocated by FormatMessage
		if (lpMsgBuf != NULL)
			::LocalFree(lpMsgBuf);
	}

	return hr;
};

#ifdef INLINE_VW_HELPERS

inline void VWLPTSTRFromCLSID(REFCLSID rclsid, LPTSTR pszCLSID)
{
	wsprintf(pszCLSID, _T("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"),
		rclsid.Data1, rclsid.Data2, rclsid.Data3,
		rclsid.Data4[0], rclsid.Data4[1], rclsid.Data4[2], rclsid.Data4[3],
		rclsid.Data4[4], rclsid.Data4[5], rclsid.Data4[6], rclsid.Data4[7]);
}

inline CString VWStringFromCLSID(REFCLSID rclsid)
{
	TCHAR szCLSID[256];
	wsprintf(szCLSID, _T("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"),
		rclsid.Data1, rclsid.Data2, rclsid.Data3,
		rclsid.Data4[0], rclsid.Data4[1], rclsid.Data4[2], rclsid.Data4[3],
		rclsid.Data4[4], rclsid.Data4[5], rclsid.Data4[6], rclsid.Data4[7]);
	return szCLSID;
}

inline CComBSTR VWBSTRFromCLSID(REFCLSID rclsid)
{
	TCHAR szCLSID[256];
	wsprintf(szCLSID, _T("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"),
		rclsid.Data1, rclsid.Data2, rclsid.Data3,
		rclsid.Data4[0], rclsid.Data4[1], rclsid.Data4[2], rclsid.Data4[3],
		rclsid.Data4[4], rclsid.Data4[5], rclsid.Data4[6], rclsid.Data4[7]);
	return szCLSID;
}

inline CComBSTR VWBSTRFromLong(long x)
{
	char temp[20];
	_ltoa( x, temp, 10 );

	return CComBSTR(temp);
}

inline CComBSTR NormalizeBSTR(BSTR bstr)
{
	CString temp(bstr);
	temp.MakeLower();

	return CComBSTR(temp);
}

inline HRESULT InitializeDISPPARAMS(DISPPARAMS* pdispparams, int propcnt)
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

inline HRESULT TerminateDISPPARAMS(VARIANT_BOOL bClear, DISPPARAMS* pdispparams)
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

#else

CString VWStringFromCLSID(REFCLSID rclsid);
CComBSTR VWBSTRFromCLSID(REFCLSID rclsid);
void VWLPTSTRFromCLSID(REFCLSID rclsid, LPTSTR pszCLSID);
CComBSTR VWBSTRFromLong(long x);

HRESULT InitializeDISPPARAMS(DISPPARAMS* pdispparams, int propcnt);
HRESULT TerminateDISPPARAMS(VARIANT_BOOL bClear, DISPPARAMS* pdispparams);

#endif

HRESULT ConvertJSArrayToDISPPARAMS(VARIANT* pvarArgs, DISPPARAMS* pdispparams);
HRESULT ConvertJSArrayToPropertyList(VARIANT* pvarArgs, IPropertyList* pproplist);
HRESULT ConvertSAFEARRAYToDISPPARAMS(VARIANT* pvarArgs, DISPPARAMS* pdispparams);
HRESULT ConvertSAFEARRAYToPropertyList(VARIANT* pvarArgs, IPropertyList* pproplist);
HRESULT ConvertPropertyListToDISPPARAMS(IPropertyList* ppropertylist, DISPPARAMS* pdispparams);
HRESULT ConvertDISPPARAMSToPropertyList(DISPPARAMS* pdispparams, IPropertyList* ppropertylist);
HRESULT CloneDISPPARAMS(DISPPARAMS* pfrom, DISPPARAMS* pto);

// NOTE: these methods found in non-inline form in vwimpl.cpp; don't forget to change both places

// BSTR support for template collections
#ifdef INLINE_VW_HELPERS

inline UINT AFXAPI HashKey(BSTR key)
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

inline BOOL AFXAPI CompareElements(const CComBSTR* pElement1, const BSTR* pElement2)
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

inline BOOL AFXAPI CompareElements(BSTR* pElement1, BSTR* pElement2)
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
inline BOOL AFXAPI CompareElements(const CBSTRAdapt<CComBSTR>* pElement1, const BSTR* pElement2)
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
#else

UINT AFXAPI HashKey(BSTR key);
BOOL AFXAPI CompareElements(const CComBSTR* pElement1, const BSTR* pElement2);
BOOL AFXAPI CompareElements(BSTR* pElement1, BSTR* pElement2);
BOOL AFXAPI CompareElements(const CBSTRAdapt<CComBSTR>* pElement1, const BSTR* pElement2);

#endif

CComBSTR NormalizeBSTR(BSTR bstrAlias);

#ifdef ENABLE_VW_MACROS
/*
 * Dual-interface COM object (IDispatch) macros
 */

// IID base, CLSID base, LIBID base, class name, 
#define OBJECT_GENERIC_BASE(x, y, z, w) \
	public IDispatchImpl<I##x, &IID_I##x, &LIBID_##z>, \
	public ISupportErrorInfo, \
	public CComObjectRootEx<CComMultiThreadModel>, \
	public CComCoClass<w, &CLSID_##y>

// NOTE: use when class name is CFooObject for IID_IFoo
// IID base, LIBID base
#define OBJECT_BASE(x, y) OBJECT_GENERIC_BASE(x, x, y, C##x##Object)

// IID base, class name
#define OBJECT_GENERIC_BASE_CLASS(x, y) \
public: \
DECLARE_AGGREGATABLE(y) \
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid) \
	{ \
		if (riid == IID_I##x) \
			return S_OK; \
		return S_FALSE; \
	};

// NOTE: use when class name is CFooObject for IID_IFoo
// IID base
#define OBJECT_BASE_CLASS(x) \
	OBJECT_GENERIC_BASE_CLASS(x, C##x##Object)

/*
 * Object property (IObjectProperty) macros
 */

// IID base (same as CLSID base), LIBID base, class name, 
#define PROPERTY_GENERIC_BASE(x, y, z, w) OBJECT_GENERIC_BASE(x, y, z, w) 

// NOTE: use when class name is CFooObject for IID_IFoo
// IID base, LIBID base
#define PROPERTY_BASE(x, y) OBJECT_BASE(x, y)

// IID base, CLSID base, class name
#define PROPERTY_GENERIC_BASE_CLASS_(x, y, z) \
	OBJECT_GENERIC_BASE_CLASS(x, z) \
\
	STDMETHOD(get_CLSID)(CLSID* pclsid) \
	{ \
		ASSERT(pclsid); \
 \
		*pclsid = CLSID_##y; \
 \
		return S_OK; \
	}; \
 \
	STDMETHOD(Clone)(IObjectProperty* pobjectproperty) \
	{ \
		return S_OK; \
	}; \
 \
	STDMETHOD(Dump)(void); \
	STDMETHOD(ToAscii)(BSTR* pbstr); \
	STDMETHOD(Marshall)(DWORD dwOptions, IMarshallBuffer* pbuffer); \
	STDMETHOD(UnMarshall)(IUnMarshallBuffer* pbuffer); \
	STDMETHOD(Terminate)(void); \
\
	STDMETHOD(MarshallToScript)(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut) \
	{\
		ASSERT(pbstrOut); \
		ASSERT(pbAsVal);  \
		*pbstrOut = NULL; \
		*pbAsVal = VARIANT_TRUE; \
		return S_OK; \
	}; \
\
public: \
	CComBSTR m_bstrType;

// NOTE: use when class name is CFooObject for IID_IFoo
// IID base (same as CLSID base)
#define PROPERTY_BASE_CLASS_(x) \
	OBJECT_BASE_CLASS(x) \
 \
	STDMETHOD(get_CLSID)(CLSID* pclsid) \
	{ \
		ASSERT(pclsid); \
 \
		*pclsid = CLSID_##x; \
 \
		return S_OK; \
	}; \
 \
	STDMETHOD(Clone)(IObjectProperty* pobjectproperty) \
	{ \
		return S_OK; \
	}; \
 \
	STDMETHOD(Dump)(void); \
	STDMETHOD(ToAscii)(BSTR* pbstr); \
	STDMETHOD(Marshall)(DWORD dwOptions, IMarshallBuffer* pbuffer); \
	STDMETHOD(UnMarshall)(IUnMarshallBuffer* pbuffer); \
	STDMETHOD(Terminate)(void); \
 \
	STDMETHOD(MarshallToScript)(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut) \
	{\
		ASSERT(pbstrOut); \
		ASSERT(pbAsVal);  \
		*pbstrOut = NULL; \
		*pbAsVal = VARIANT_TRUE; \
		return S_OK; \
	}; \
\
public: \
	CComBSTR m_bstrType;

#define PROPERTY_GENERIC_BASE_CLASS(x, y, z) \
	PROPERTY_GENERIC_BASE_CLASS_(x, y, z) \
	PROPERTY_BASE_TYPE()

#define PROPERTY_BASE_CLASS(x) \
	PROPERTY_BASE_CLASS_(x) \
	PROPERTY_BASE_TYPE()

#define PROPERTY_BASE_TYPE_() \
	STDMETHOD(IsOfType)(BSTR bstr, VARIANT_BOOL* pbool); \
	STDMETHOD(get_Type)(BSTR* pbstr);  \
	STDMETHOD(put_Type)(BSTR bstr); \

#define PROPERTY_BASE_TYPE() \
	STDMETHOD(IsOfType)(BSTR bstr, VARIANT_BOOL* pbool) \
	{ \
		*pbool = (CString(bstr) == CString(m_bstrType)) ? TRUE : FALSE; \
		return S_OK; \
	}; \
 \
	STDMETHOD(get_Type)(BSTR* pbstr)  \
	{ \
		*pbstr = m_bstrType.Copy(); \
		return S_OK; \
	}; \
 \
	STDMETHOD(put_Type)(BSTR bstr) \
	{ \
		m_bstrType = bstr; \
		return S_OK; \
	};

#define PROPERTY_BASE_CTOR() \
	m_pSecurity = NULL; \
	m_pParent = NULL;\
	m_pWorld = NULL;

#define PROPERTY_BASE_DTOR() \
	SAFERELEASE(m_pSecurity); \
	SAFERELEASE(m_pParent);\
	if (m_pWorld) \
		m_pWorld = NULL;\


#define PROPERTY_BASE_INITIALIZE(pw) \
	ASSERT(pw); \
	m_pWorld = pw;

#define PROPERTY_BASE_MARSHALL(x) \
	{ \
		HRESULT hr = S_OK; \
\
		VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, #x "::Marshall: marshalling parent (by ref)\n"); \
\
		if (m_pParent == NULL) \
		{ \
			hr = pbuffer->put_Byte(0); \
		} \
		else \
		{ \
			hr = pbuffer->put_Byte(1); \
\
			hr = m_pWorld->MarshallObjectProperty(m_pParent, MARSHALLREF(dwOptions,MARSHALL_BYREF), \
				pbuffer); \
			if (FAILED(hr)) \
				return hr; \
\
			VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, #x "::Marshall: marshalling property name\n"); \
\
			hr = pbuffer->put_String(m_bstrPropertyName); \
			if (FAILED(hr)) \
				return hr; \
		} \
		\
		hr = pbuffer->put_String(m_bstrType); \
		if (FAILED(hr)) \
			return hr;\
	}

#define PROPERTY_BASE_UNMARSHALL(x) \
	{ \
		HRESULT hr = S_OK; \
		unsigned char tag = 0; \
\
		VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, #x "::UnMarshall: unmarshalling parent\n"); \
\
		hr = pbuffer->get_Byte(&tag); \
		if (FAILED(hr)) \
			return hr; \
\
		if (tag) \
		{ \
			SAFERELEASE(m_pParent); \
			hr = m_pWorld->UnMarshallObjectProperty(pbuffer, (IObjectProperty**)&m_pParent); \
			if (FAILED(hr)) \
				return hr; \
\
			VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, #x "::UnMarshall: unmarshalling property name\n"); \
\
			m_bstrPropertyName.Empty(); \
			hr = pbuffer->get_String(&m_bstrPropertyName.m_str); \
			if (FAILED(hr))	\
				return hr; \
		} \
\
		m_bstrType.Empty(); \
		hr = pbuffer->get_String(&m_bstrType.m_str); \
		if (FAILED(hr))	\
			return hr; \
	}

/*
 * Referenced property (IRefProperty) macros
 */

#define REFPROPERTY_BASE_CTOR() \
	m_lObjectID = 0;

#define REFPROPERTY_BASE_CLASS() \
public: \
	STDMETHOD(get_ID)(long* id) \
	{ \
		ASSERT(id); \
\
		*id = m_lObjectID; \
\
		return S_OK; \
	}; \
\
	STDMETHOD(put_ID)(long id) \
	{ \
		m_lObjectID = id; \
\
		return S_OK; \
	}; \
\
private: \
	long m_lObjectID;
#endif

// helper template
template<class CType>
void SAFERELEASE( CType * &pType )
{
	if ( pType )
	{
		pType->Release();
		pType = NULL;
	}
}

template<class CType>
void SAFEADDREF( CType * pType )
{
	if ( pType )
	{
		pType->AddRef();
	}
}

// templated so IPropertySecurity doesn't have to be included
template<class CWorldType, class CSecurityType>
VARIANT_BOOL SAFE_CHECKSECURITY(CWorldType *pWorld, CSecurityType * pSecurity, unsigned long ulFlags)
{
#ifndef DISABLE_SECURITY
	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "SAFE_CHECKSECURITY: ulFlags %x\n", ulFlags);

	if (pSecurity == NULL ) 
	{
		// REVIEW: don't make NULL sec ptr an error
		VWTRACE(pWorld, "VWOBJECT", VWT_IMPORTANT, "SAFE_CHECKSECURITY: NULL SecurityProperty\n");
		return VARIANT_TRUE;
	//	return VARIANT_FALSE;
	}

	VARIANT_BOOL bRet = VARIANT_TRUE;

	if (FAILED(pSecurity->CheckSecurity(ulFlags, VARIANT_FALSE, &bRet)))
		return VARIANT_FALSE;

	return bRet;
#else
	return VARIANT_TRUE;
#endif
}

// templated so IPropertySecurity doesn't have to be included
template<class CWorldType, class CSecurityType>
VARIANT_BOOL QUIET_SAFE_CHECKSECURITY(CWorldType *pWorld, CSecurityType * pSecurity, unsigned long ulFlags)
{
#ifndef DISABLE_SECURITY
	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "QUIET_SAFE_CHECKSECURITY: ulFlags %x\n", ulFlags);

	if (pSecurity == NULL ) 
	{
		// REVIEW: don't make NULL sec ptr an error
		VWTRACE(pWorld, "VWOBJECT", VWT_IMPORTANT, "QUIET_SAFE_CHECKSECURITY: NULL SecurityProperty\n");
		return VARIANT_TRUE;
	//	return VARIANT_FALSE;
	}

	VARIANT_BOOL bRet = VARIANT_TRUE;

	if (FAILED(pSecurity->CheckSecurity(ulFlags, VARIANT_TRUE, &bRet)))
		return VARIANT_FALSE;

	return bRet;
#else
	return VARIANT_TRUE;
#endif
}

// helper macros

#define SAFEFREESTRING(bstr) \
if (bstr) \
{ \
	::SysFreeString(bstr); \
	bstr = NULL; \
}


template<class CType>
CType * SAFE_ASSIGN(CType * &pTypeTo, CType * pTypeFrom)
{
	SAFERELEASE( pTypeTo );
	pTypeTo = pTypeFrom;
	SAFEADDREF( pTypeTo );
	return pTypeTo;
}

template<class CType>
CType * SAFE_QIASSIGN(CType * &pTypeTo, IUnknown* lp, REFIID riid)
{
	SAFERELEASE( pTypeTo );
	if ( !lp ) return pTypeTo;
	void * temp;
	lp->QueryInterface(riid, &temp);
	return pTypeTo = (CType *) temp;
}


template <class T, const CLSID* pclsid, const IID* piid>
class CVWComPtr
{
public:
	typedef T _PtrClass;
	CVWComPtr() {p=NULL;}
	CVWComPtr(CComVariant var)
	{
		p=NULL;
		switch ( var.vt )
		{
		case VT_UNKNOWN:
			SAFE_QIASSIGN(p, var.punkVal, *piid);
		break;
		case VT_DISPATCH:
			SAFE_QIASSIGN(p, (IUnknown*) var.pdispVal, *piid);
		break;
		}
	}
	CVWComPtr(T* lp)
	{
		if ((p = lp) != NULL)
			p->AddRef();
	}
	CVWComPtr(const CVWComPtr<T,pclsid,piid>& lp)
	{
		if ((p = lp.p) != NULL)
			p->AddRef();
	}
	// If you get an error that this member is already defined, you are probably
	// using a CVWComPtr<IUnknown, &IID_IUnknown>.  This is not necessary.
	// Use CComPtr<IUnknown>
	CVWComPtr(IUnknown* lp)
	{
		p=NULL;
		if (lp != NULL)
			lp->QueryInterface(*piid, (void **)&p);
	}
	~CVWComPtr() { SAFERELEASE(p);}
	void Release() { SAFERELEASE(p); }
	HRESULT CoCreateInstance()
	{
		SAFERELEASE(p);
		return ::CoCreateInstance(*pclsid,NULL,CLSCTX_INPROC_SERVER, *piid,(void**) &p );
	}

	operator T*() {return p;}
	// Pointer operations shouldn't be used in general, 
	// These are smart pointers and shouldn't be used as pointers themselves
	// But the list collection templates do create heap objects, so these better 
	// do what they are expected and not forward pointer dereferencing and addressing to
	// the actuall pointer, becuase then well get a real mess.
//	T& operator*() {ASSERT(p!=NULL); return *p; }
//	T** operator&() {ASSERT(p==NULL); return &p; }


	T* operator->() {ASSERT(p!=NULL); return p; }
	T* operator=(T* lp){ return SAFE_ASSIGN( p, lp ); }
	T* operator=(const CVWComPtr<T,pclsid,piid>& lp){return SAFE_ASSIGN(p, lp.p);}
	T* operator=(IUnknown* lp){return SAFE_QIASSIGN(p, lp, *piid);}
	T* operator=(CComVariant const var)
	{
		SAFERELEASE(p);
		
		switch ( var.vt )
		{
		case VT_UNKNOWN:
			return SAFE_QIASSIGN(p, var.punkVal, *piid);
		break;
		
		case VT_DISPATCH:
			return SAFE_QIASSIGN(p, (IUnknown *) var.pdispVal, *piid);
		break;
		}
		
		return p;
	}
	// Avoid Copy constructor creation
	T* operator=(VARIANT const &var)
	{
		SAFERELEASE(p);
		
		switch ( var.vt )
		{
		case VT_UNKNOWN:
			return SAFE_QIASSIGN(p, var.punkVal, *piid);
		break;
		
		case VT_DISPATCH:
			return SAFE_QIASSIGN(p, (IUnknown *) var.pdispVal, *piid);
		break;
		}
		
		return p;
	}
	BOOL operator!(){return (p == NULL) ? TRUE : FALSE;}
	T* p;
};

// macros to ease the redundant pain of firing events
// use pEvent to call your event interface

// 02-11-98 dv - removed the hresult check

#define START_EVENT(class, iface) \
	Lock(); \
	\
	IUnknown** pp = m_vec.begin(); \
    while (pp < m_vec.end()) \
    { \
        if (*pp != NULL) \
        { \
			iface* pEvent = reinterpret_cast<iface*>(*pp);

#define FINISH_EVENT() \
        } \
        pp++; \
    } \
	\
    Unlock();

#endif //__PROPBASE_H
