// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWUtils.h : Helpful utilities when dealing with V-Worlds

#include <propbase.h>

//---------------------------------------------------------------------------------------
//
//	HRESULT exception handling
//
// The following exception class and test function allow C++ code to use try...catch
// to deal with HRESULTs. Rather than having to save each result, test it, and goto an
// error label, you just surround the code with try...catch(CHRESULTException) and test
// any HRESULTs returned using TestHRESULT. For example:
//
//		HRESULT hr = S_OK;
//
//		try
//		{
//			TestHRESULT(foo(...));
//			TestHRESULT(bar(...));
//			TestHRESULT(baz(...));
//		}
//		catch(CHRESULTException chr) { hr = chr.GetHRESULT(); }
//
//		// ... Clean-up code ...
//
//		return hr;


// An exception generated from an HRESULT
class CHRESULTException
{
public:
	CHRESULTException(HRESULT hr) : hr(hr) {};	// Create an exception with an HRESULT
	HRESULT GetHRESULT() { return hr; };		// Return the HRESULT

private:
	HRESULT hr;									// The HRESULT that caused the exception
};

// Test for a failed HRESULT
inline void TestHRESULT(HRESULT hr)
{
	if (FAILED(hr))
	{
#ifdef DEBUG
		// ASSERT(FALSE);
#endif
		throw CHRESULTException(hr);
	}
}


#if 0

// Not recommended, but here nonetheless

#define GETPROP(obj,name,val) TestHRESULT(##obj->get_##name(##val))
#define GETADDEDPROP(obj,type,name,val) TestHRESULT(##obj->get_##type(##name, ##val))

#define PUTPROP(obj,name,val) TestHRESULT(##obj->put_##name(##val))
#define PUTADDEDPROP(obj,type,name,val) TestHRESULT(##obj->put_##type(##name, ##val))

#endif

class CMethodInfo
{
public:
	long				lFlags;
	LPCTSTR				pszName;
	LPCTSTR				pszNameInternal;
	long				psbits;
	long				permissions;
};

// CComDISPPARAMS
class CComDISPPARAMS
{
public:
	CComDISPPARAMS() 
	{ 
		InitializeDISPPARAMS(&m_dp, 0); 
	}

	CComDISPPARAMS(int i, ...)
	{
		HRESULT hr = InitializeDISPPARAMS(&m_dp, i);

		ASSERT(hr == S_OK);

		if (SUCCEEDED(hr))
		{
			va_list vl;

			va_start(vl, i);

			while (i--)
			{
				// REVIEW: This asserts on VT_EMPTYs since it's filled with zeros
				// ASSERT(*vl != NULL);

				memcpy(&m_dp.rgvarg[i], &va_arg(vl, VARIANT), sizeof(VARIANT));
			}

			va_end(vl);
		}
	}

	~CComDISPPARAMS() 
	{ 
		HRESULT hr = TerminateDISPPARAMS(VARIANT_FALSE, &m_dp); 

		ASSERT(hr == S_OK);
	}

	HRESULT Initialize(int i, ...)
	{
		_ASSERTE(m_dp.cArgs == 0);

		HRESULT hr = InitializeDISPPARAMS(&m_dp, i);

		ASSERT(hr == S_OK);
		
		if (SUCCEEDED(hr))
		{
			va_list vl;

			va_start(vl, i);

			while (i--)
			{
//				ASSERT(*vl != NULL);

				memcpy(&m_dp.rgvarg[i], &va_arg(vl, VARIANT), sizeof(VARIANT));
			}

			va_end(vl);
		}

		return hr;
	}

	HRESULT Terminate(VARIANT_BOOL bClear = VARIANT_FALSE) 
	{ 
		HRESULT hr = TerminateDISPPARAMS(bClear, &m_dp); 

		ASSERT(hr == S_OK);

		return hr;
	}

	operator DISPPARAMS*() { return &m_dp; }

	DISPPARAMS* operator->() { _ASSERTE(m_dp.cArgs == 0); return &m_dp; }

	DISPPARAMS m_dp;
};
