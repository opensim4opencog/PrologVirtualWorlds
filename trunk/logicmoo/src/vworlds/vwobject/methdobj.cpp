// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// MethodObj.cpp : Implementation of CfooApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include "methdobj.h"
#include "syshelp.h"

IMPLEMENT_OBJECTPROPERTY(CMethodObject)

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMethodObject::Dump(void)
{
	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "Method: begin\n");

	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "Method: m_dispid %d\n", m_dispid);
	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "Method: m_bstrMethodName %s\n", CString(m_bstrMethodName));
	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "Method: client-side (%s), server-side (%s), sync (%s), inline (%s)\n",
		(m_lFlags & METHOD_CLIENT)  ? "true" : "false",
		(m_lFlags & METHOD_SERVER) ? "true" : "false",
		(m_lFlags & METHOD_SYNC) ? "true" : "false",
		(m_lFlags & METHOD_INLINE) ? "true" : "false");

	if (m_pModule)
	{
		BSTR bstr = NULL;
		CString strName;

		if (SUCCEEDED(m_pModule->get_ModuleName(&bstr)))
		{
			strName = bstr;
			SAFEFREESTRING(bstr);
		}

		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "Method: m_pModule (%s)\n", strName);
	}
	else
		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "Method: m_pModule (null)\n");

	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "Method: end\n");

	return S_OK;
}

STDMETHODIMP CMethodObject::get_Flags(long* plVal)
{
	ASSERT(plVal);
	if (plVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::get_Flags: outgoing pointer (long*) is NULL\n");
		return E_POINTER;
	}

	if (m_pParent)
	{
		if (FAILED(m_pParent->CheckMethodSecurityExt(m_bstrPropertyName, PS_READ)))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::get_Flags: permission denied (PS_READ)\n");

			return VWOBJECT_E_PERMISSION_DENIED;
		}
	}

	*plVal = m_lFlags;

	return S_OK;
}

STDMETHODIMP CMethodObject::put_Flags(long lVal)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CMethodObject::put_ServerSide\n");
#endif

	// at least one bit should be set
	if (!(lVal & METHOD_CLIENT || lVal & METHOD_SERVER))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::put_Flags: invalid flags parameter (%x)\n", lVal);
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckMethodSecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::put_Flags: permission denied (PS_WRITE)\n");

				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		m_lFlags = lVal;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_I4;
		dispparams.rgvarg[0].lVal = lVal;

		hr = SendMethodCommand(DISPID_METHOD_FLAGS, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of method change
		if (m_pParent)
		{
			hr = m_pParent->NotifyMethodChanged(this, meFlags, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CMethodObject::get_Module(IModule** ppmodule)
{
	ASSERT(ppmodule);
	if (ppmodule == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::get_Module: outgoing pointer (IModule**) is NULL\n");
		return E_POINTER;
	}

	if (m_pParent)
	{
		if (FAILED(m_pParent->CheckMethodSecurityExt(m_bstrPropertyName, PS_READ)))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::get_Module: permission denied (PS_READ)\n");

			return VWOBJECT_E_PERMISSION_DENIED;
		}
	}

	*ppmodule = m_pModule;
	SAFEADDREF(*ppmodule);

	return S_OK;
}

STDMETHODIMP CMethodObject::put_Module(IModule* pmodule)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;
	long lFlags = 0L;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CMethodObject::put_Module\n");
#endif

	ASSERT(pmodule);
	if (pmodule == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::put_Module: incoming argument (IModule*) is NULL\n");
		return E_INVALIDARG;
	}

	/* hr = */ pmodule->get_Flags(&lFlags);

	if (m_lFlags & METHOD_CLIENT)
	{
		if (!(lFlags & MODULE_CLIENT))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::put_Module: assigning server-side module to client-side method\n");
			return VWOBJECT_E_NOTCLIENTMODULE;
		}
	}

	if (m_lFlags & METHOD_SERVER)
	{
		if (!(lFlags & MODULE_SERVER))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::put_Module: assigning client-side module to server-side method\n");
			return VWOBJECT_E_NOTSERVERMODULE;
		}
	}

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckMethodSecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::put_Module: permission denied (PS_WRITE)\n");

				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		SAFERELEASE(m_pModule);

		m_pModule = pmodule;

		SAFEADDREF(m_pModule);
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = pmodule;

		hr = SendMethodCommand(DISPID_METHOD_MODULE, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of method change
		if (m_pParent)
		{
			hr = m_pParent->NotifyMethodChanged(this, meModule, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CMethodObject::get_ArgumentsExt(DISPPARAMS** ppdispparams)
{
	HRESULT hr = S_OK;

	ASSERT(ppdispparams);
	if (ppdispparams == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::get_ArgumentsExt: outgoing pointer (DISPPARAMS**) is NULL\n");
		return E_POINTER;
	}

	*ppdispparams = new DISPPARAMS;
	if (*ppdispparams == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::get_ArgumentsExt: failed to allocate DISPPARAMS\n");
		return E_OUTOFMEMORY;
	}

	if (m_pParent)
	{
		if (FAILED(m_pParent->CheckMethodSecurityExt(m_bstrPropertyName, PS_READ)))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::get_ArgumentsExt: permission denied (PS_READ)\n");

			return VWOBJECT_E_PERMISSION_DENIED;
		}
	}

	hr = CloneDISPPARAMS(&m_dispparams, *ppdispparams);

	return hr;
}

STDMETHODIMP CMethodObject::put_ArgumentsExt(DISPPARAMS* pdispparams)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CMethodObject::put_ArgumentsExt\n");
#endif

	ASSERT(pdispparams);
	if (pdispparams == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::put_ArgumentsExt: incoming argument (DISPPARAMS*) is NULL\n");
		return E_INVALIDARG;
	}

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckMethodSecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::put_ArgumentsExt: permission denied (PS_WRITE)\n");

				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		TerminateDISPPARAMS(VARIANT_TRUE, &m_dispparams);

		hr = CloneDISPPARAMS(pdispparams, &m_dispparams);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		// only create arglist if needed
		if (m_pArgList == NULL)
		{
			hr = CreatePropertyList(m_pWorld, &m_pArgList);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}

		DISPPARAMS dispparams;

		hr = ConvertDISPPARAMSToPropertyList(pdispparams, m_pArgList);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = m_pArgList;

		hr = SendMethodCommand(DISPID_METHOD_ARGUMENTSINT, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of method change
		if (m_pParent)
		{
			hr = m_pParent->NotifyMethodChanged(this, meArguments, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CMethodObject::get_DispID(DISPID* pdispid)
{
	ASSERT(pdispid);
	if (pdispid == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::get_DispID: outgoing pointer (DISPID*) is NULL\n");
		return E_POINTER;
	}

	if (m_pParent)
	{
		if (FAILED(m_pParent->CheckMethodSecurityExt(m_bstrPropertyName, PS_READ)))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::get_DispID: permission denied (PS_READ)\n");

			return VWOBJECT_E_PERMISSION_DENIED;
		}
	}

	*pdispid = m_dispid;

	return S_OK;
}

STDMETHODIMP CMethodObject::put_DispID(DISPID dispid)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CMethodObject::put_DispID\n");
#endif

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckMethodSecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::put_DispID: permission denied (PS_WRITE)\n");

				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		m_dispid = dispid;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_I4;
		dispparams.rgvarg[0].lVal = dispid;

		hr = SendMethodCommand(DISPID_METHOD_DISPID, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of method change
		if (m_pParent)
		{
			hr = m_pParent->NotifyMethodChanged(this, meDispID, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CMethodObject::get_MethodName(BSTR* pbstr)
{
	ASSERT(pbstr);
	if (pbstr == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::get_MethodName: outgoing pointer (BSTR*) is NULL\n");
		return E_POINTER;
	}

	if (m_pParent)
	{
		if (FAILED(m_pParent->CheckMethodSecurityExt(m_bstrPropertyName, PS_READ)))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::get_MethodName: permission denied (PS_READ)\n");

			return VWOBJECT_E_PERMISSION_DENIED;
		}
	}

	*pbstr = m_bstrMethodName.Copy();

	return S_OK;
}

STDMETHODIMP CMethodObject::put_MethodName(BSTR bstrMethodName)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CMethodObject::put_MethodName\n");
#endif

	ASSERT(bstrMethodName);
	if (bstrMethodName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::put_MethodName: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckMethodSecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::put_MethodName: permission denied (PS_WRITE)\n");

				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		m_bstrMethodName = bstrMethodName;

		// make dispid be late bound at Invoke time
		m_dispid = DISPID_UNKNOWN;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstrMethodName;

		hr = SendMethodCommand(DISPID_METHOD_METHODNAME, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of method change
		if (m_pParent)
		{
			hr = m_pParent->NotifyMethodChanged(this, meMethodName, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CMethodObject::get_Code(BSTR *pbstr)
{
	ASSERT(pbstr);
	if (pbstr == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::get_Code: outgoing pointer (BSTR*) is NULL\n");
		return E_POINTER;
	}

	if (m_pParent)
	{
		if (FAILED(m_pParent->CheckMethodSecurityExt(m_bstrPropertyName, PS_READ)))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::get_Code: permission denied (PS_READ)\n");

			return VWOBJECT_E_PERMISSION_DENIED;
		}
	}

	*pbstr = m_bstrCode.Copy();

	return S_OK;
}

STDMETHODIMP CMethodObject::put_Code(BSTR bstrCode)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CMethodObject::put_Code\n");
#endif

	ASSERT(bstrCode);
	if (bstrCode == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::put_Code: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckMethodSecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::put_Code: permission denied (PS_WRITE)\n");

				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		m_bstrCode = bstrCode;

		// set in-inline flag to TRUE
		// REVIEW: There's no way to turn an inline method back into
		// a regular method!
		m_lFlags |= METHOD_INLINE;

		// clear the parsed flag (we need to reparse)
		m_fParsed = VARIANT_FALSE;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstrCode;

		hr = SendMethodCommand(DISPID_METHOD_CODE, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of method change
		if (m_pParent)
		{
			hr = m_pParent->NotifyMethodChanged(this, meCode, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CMethodObject::get_IsInline(VARIANT_BOOL *pboolVal)
{
	ASSERT(pboolVal);
	if (pboolVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::get_IsInline: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return E_POINTER;
	}

	if (m_pParent)
	{
		if (FAILED(m_pParent->CheckMethodSecurityExt(m_bstrPropertyName, PS_READ)))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::get_IsInline: permission denied (PS_READ)\n");

			return VWOBJECT_E_PERMISSION_DENIED;
		}
	}

	*pboolVal = (m_lFlags & METHOD_INLINE) ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CMethodObject::InvokeMethodExt(DISPPARAMS* pdispparams, HRESULT* phr, VARIANT* pvar)
{
	HRESULT hr = S_OK;
	HRESULT invokehr = S_OK;
	IScriptModule* psm = NULL;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CMethodObject::InvokeMethodExt\n");

	if (m_pModule == NULL)
	{
		VWREPORT(m_pWorld, VWR_ERROR, "Failed to invoke method (%s), invalid module\n", CString(m_bstrMethodName));

		return VWOBJECT_E_INVALIDMODULE;
	}

	// NOTE: phr and pvar can be NULL

	// NOTE: pdispparams can be NULL
	if (pdispparams == NULL)
	{
		// NOTE: need to have at least an empty DISPPARAMS struct

		// if we have authored args, use them
//		if (m_dispparams.rgvarg != NULL)
			pdispparams = &m_dispparams;
	}

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CMethodObject::InvokeMethodExt: name %s, dispid %d\n", CString(m_bstrMethodName), m_dispid);

	// see if this is an inline method
	if ((m_lFlags & METHOD_INLINE) == 0)
	{
		WORD wFlags = DISPATCH_METHOD;

		if (m_lFlags & METHOD_PROPERTYPUT)
			wFlags = DISPATCH_PROPERTYPUT;
		else if (m_lFlags & METHOD_PROPERTYGET)
			wFlags = DISPATCH_PROPERTYGET;

		if (m_dispid == DISPID_UNKNOWN)
		{
			hr = m_pModule->InvokeByName(m_bstrMethodName, wFlags, &m_dispid, pdispparams, VARIANT_FALSE, pvar, phr);
		}
		else
		{
			hr = m_pModule->InvokeByDispID(m_dispid, wFlags, pdispparams, VARIANT_FALSE, pvar, phr);
		}
	}
	else
	{
		// NOTE: assuming DISPATCH_METHOD since we probably can't define a propput in an inline method

		// if we haven't parsed the code, do so now
		if (m_fParsed != VARIANT_TRUE)
		{
			// verify we have a script module
			hr = m_pModule->QueryInterface( IID_IScriptModule, (void **) &psm );
			if (FAILED(hr))
				goto ERROR_RETURNED;

			// parse the script
			hr = psm->AddScriptlet(m_bstrCode);
			if (FAILED(hr))
				goto ERROR_RETURNED;

			// set flag
			m_fParsed = VARIANT_TRUE;
		}

		// now run the method
		if (m_dispid == DISPID_UNKNOWN)
		{
			hr = m_pModule->InvokeByName(m_bstrMethodName, DISPATCH_METHOD, &m_dispid, pdispparams, VARIANT_FALSE, pvar, phr);
		}
		else
		{
			hr = m_pModule->InvokeByDispID(m_dispid, DISPATCH_METHOD, pdispparams, VARIANT_FALSE, pvar, phr);
		}
	}

ERROR_RETURNED:
	SAFERELEASE(psm);

#if _DEBUG
	if (hr == DISP_E_EXCEPTION)
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMethodObject::InvokeMethodExt: DISP_E_EXCEPTION, method name (%s), dispid %d, hr %x\n", (m_bstrMethodName.Length() == 0) ? "n/a" : CString(m_bstrMethodName), m_dispid, *phr);
#endif

	return hr;
}

STDMETHODIMP CMethodObject::Marshall(DWORD dwOptions, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;
	long lType = mcScript;

	if (FAILED(hr = CMethodObjectBase::Marshall(dwOptions, pbuffer)))
		return hr;

	ASSERT(pbuffer);
	if (pbuffer == NULL)
		return E_POINTER;

	// force options to be BYREF
	DWORD dwNewOptions = MARSHALLREF(dwOptions, MARSHALL_BYREF);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CMethodObject::Marshall: marshalling module (by ref)\n");
#endif

	if (m_pModule)
		/* hr = */ m_pModule->get_ModuleType(&lType);

	hr = m_pWorld->MarshallObjectProperty(m_pModule, (lType == mcScript) ? clsidScriptModule : clsidCOMModule, 
		dwNewOptions, pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CMethodObject::Marshall: marshalling flags, client/server (%s), inline (%s)\n",
		(m_lFlags & METHOD_SERVER) ? ((m_lFlags & METHOD_CLIENT) ? "both" : "server") : ((m_lFlags & METHOD_CLIENT) ? "client" : "neither"),
		(m_lFlags & METHOD_INLINE) ? "true" : "false");
#endif

	hr = pbuffer->put_Long(m_lFlags);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// if it's not an inline method, marshal out the dispid and method name
	if ((m_lFlags & METHOD_INLINE) == 0)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CMethodObject::Marshall: marshalling dispid (%x)\n", m_dispid);
#endif

		// NOTE: if a method name was specified, don't marshall out
		// the resolved DISPID - just let it be looked up the next time
		// the method is instantiated

		hr = pbuffer->put_Long(((BSTR)m_bstrMethodName != NULL) ? DISPID_UNKNOWN : m_dispid);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}
	else
	{
		// marshal out code for inline method
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CMethodObject::Marshall: marshalling inline code\n");
#endif

		hr = pbuffer->put_String(m_bstrCode, VARIANT_FALSE);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CMethodObject::Marshall: marshalling method name (%s)\n", CString(m_bstrMethodName));
#endif

	hr = pbuffer->put_String(m_bstrMethodName);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CMethodObject::Marshall: marshalling arguments\n");
#endif

	hr = m_pWorld->MarshallDISPPARAMS(&m_dispparams, pbuffer, dwNewOptions);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CMethodObject::UnMarshall(IUnMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = CMethodObjectBase::UnMarshall(pbuffer)))
		return hr;

	ASSERT(pbuffer);
	if (pbuffer == NULL)
		return E_POINTER;

	// free any loaded data
	SAFERELEASE(m_pModule);
	m_bstrMethodName.Empty();
	TerminateDISPPARAMS(VARIANT_TRUE, &m_dispparams);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CMethodObject::UnMarshall: unmarshalling module\n");
#endif

	hr = m_pWorld->UnMarshallObjectProperty(pbuffer, (IObjectProperty**)&m_pModule);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->get_Long(&m_lFlags);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CMethodObject::UnMarshall: unmarshalling flags, client/server (%s), inline (%s)\n",
		(m_lFlags & METHOD_SERVER) ? ((m_lFlags & METHOD_CLIENT) ? "both" : "server") : ((m_lFlags & METHOD_CLIENT) ? "client" : "neither"),
		(m_lFlags & METHOD_INLINE) ? "TRUE" : "FALSE");
#endif

	// if it's not inline, unmarshal the method dispid and name
	if ((m_lFlags & METHOD_INLINE) == 0)
	{
   		hr = pbuffer->get_Long(&m_dispid);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CMethodObject::UnMarshall: unmarshalling dispid (%d)\n", m_dispid);
#endif
	}
	else
	{
		// unmarshal the inline code
		hr = pbuffer->get_String(&m_bstrCode.m_str);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CMethodObject::UnMarshall: unmarshalling method inline code\n");
#endif
	}

	hr = pbuffer->get_String(&m_bstrMethodName.m_str);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CMethodObject::UnMarshall: unmarshalling method name (%s)\n", CString(m_bstrMethodName));
#endif

	hr = m_pWorld->UnMarshallDISPPARAMS(pbuffer, &m_dispparams);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CMethodObject::UnMarshall: unmarshalling arguments\n");
#endif

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CMethodObject::MarshallToScript(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut)
{
	HRESULT hr = S_OK;
	CString strContext = bstrContext;
	CComBSTR bstrSubContext;
	CComBSTR bstrModuleName;
	CComBSTR bstrArgs;
	CString strOut;
	CString strModule;
	CString strCode;
	CString strLine;
	IPropertyList *plistArgs = NULL;
	VARIANT_BOOL bPropAsVal;

	if (bstrContext==NULL || pbstrOut==NULL || pbAsVal==NULL)
		return E_INVALIDARG;

	ASSERT(m_bstrMethodName);

	// handle in-line and other cases differently
	if (m_lFlags & METHOD_INLINE)
	{
		// REVIEW: Todo: handle non-VBScript inline.  Would have to look at
		// language associated with dummy module
		strCode = m_bstrCode;
		Quotify(strCode);
		strOut.Format(	"\tSet %s = World.CreateInlineMethod(%s, \"%s\", \"%s\", %s)\n",
						(LPCTSTR) strContext,
						(m_lFlags & METHOD_SERVER) ? ((m_lFlags & METHOD_CLIENT) ? (LPCTSTR) "World.METHOD_CLIENT + World.METHOD_SERVER" : "World.METHOD_SERVER") : (LPCTSTR) "World.METHOD_CLIENT",
						(LPCTSTR) "VBScript",
						(LPCTSTR) CString(m_bstrMethodName),
						(LPCTSTR) strCode);
	}
	else
	{
		// normal case
		if (m_pModule != NULL && m_bstrMethodName!=NULL)
		{
			// get the module name
			hr = m_pModule->get_ModuleName(&bstrModuleName.m_str);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			strOut.Format(	"\tSet %s = World.CreateMethod(%s, World.Module(\"%s\"), \"%s\")\n",
							(LPCTSTR) strContext,
							(m_lFlags & METHOD_SERVER) ? ((m_lFlags & METHOD_CLIENT) ? (LPCTSTR) "World.METHOD_CLIENT + World.METHOD_SERVER" : "World.METHOD_SERVER") : (LPCTSTR) "World.METHOD_CLIENT",
							(LPCTSTR) CString(bstrModuleName),
							(LPCTSTR) CString(m_bstrMethodName));
		}
		else
		{
			// an obscure case: a "partially built" method.  Do 
			// a low-level create and initialize
			strOut.Format(	"\tSet %s = World.CreateObjectProperty(\"VWSYSTEM.Method.1\")\n",
							(LPCTSTR) strContext );

			// set other props as appropriate
			strLine.Format(	"\t%s.Flags = %d\n", (LPCTSTR) CString(bstrContext), m_lFlags);
			strOut += strLine;

			if (m_pModule != NULL)
			{
				// get the module name
				hr = m_pModule->get_ModuleName(&bstrModuleName.m_str);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;

				strLine.Format(	"\t%s.Module = Module(\"%s\")\n", 
								(LPCTSTR) strContext,
								(LPCTSTR) CString(bstrModuleName));
				strOut += strLine;
			}

			if (m_bstrMethodName != NULL)
			{
				strLine.Format(	"\t%s.MethodName = \"%s\"\n",
								(LPCTSTR) strContext,
								(LPCTSTR) CString(m_bstrMethodName));
				strOut += strLine;
			}

			if (m_dispid != DISPID_UNKNOWN)
			{
				strLine.Format(	"\t%s.DispID = %d\n",
								(LPCTSTR) strContext,
								(int) m_dispid );
				strOut += strLine;
			}
		}
	}

	// marshal out arguments (if any)
	if (m_dispparams.cArgs > 0)
	{
		hr = CreatePropertyList(m_pWorld, &plistArgs);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = ConvertDISPPARAMSToPropertyList(&m_dispparams, plistArgs);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// generate sub context
		bstrSubContext = bstrContext;
		bstrSubContext.Append("_t");
		hr = plistArgs->MarshallToScript(MARSHALL_BYREF, bstrSubContext, &bPropAsVal, &bstrArgs.m_str);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (bPropAsVal==VARIANT_TRUE)
		{
			strLine.Format(	"\t%s.Arguments = %s\n",
							(LPCTSTR) strContext,
							(LPCTSTR) CString(bstrArgs));

		}
		else
		{
			strOut += CString(bstrArgs);
			strLine.Format(	"\t%s.Arguments = %s\n",
							(LPCTSTR) strContext,
							(LPCTSTR) CString(bstrSubContext));
		}
		strOut += strLine;
	}

	*pbstrOut = strOut.AllocSysString();
	*pbAsVal = VARIANT_FALSE;

ERROR_ENCOUNTERED:

	SAFERELEASE(plistArgs);

	return hr;
}

//
// Handles double-quoting and any embedded special characters.  E.g.
//
// Examples
//		Dog			becomes "Dog"
//		"Quotation"	becomes ""Quotation""
//		Foo\nBar	becomes "Foo" + Chr(10) + "Bar"
//
//
void CMethodObject::Quotify(CString& str)
{
	CString strNew;
	int i, cch;
	char ch;
	BOOL bInStr;

	// add opening quote
	strNew = '"';
	bInStr = TRUE;

	// go through chars
	cch = str.GetLength();
	for (i=0; i<cch; i++)
	{
		// fetch character
		ch = str.GetAt(i);

		switch(ch)
		{
			case '"':
				// if not in string, start up
				if (!bInStr)
				{
					strNew += "+\"";
					bInStr = TRUE;
				}

				// add an extra quote
				strNew += '"';
				strNew += ch;
				break;

			case '\n':
				// if in string, close
				if (bInStr)
				{
					strNew += "\"";
					bInStr = FALSE;
				}

				// add chr(10)
				strNew += "+chr(10)";

				break;

			case '\r':
				// if in string, close
				if (bInStr)
				{
					strNew += "\"";
					bInStr = FALSE;
				}

				// add chr(13)
				strNew += "+chr(13)";

				break;

			default:
				// if not in string, start up
				if (!bInStr)
				{
					strNew += "+\"";
					bInStr = TRUE;
				}

				strNew += ch;
				break;
		}
	}

	// add closing quote, if necessary
	if (bInStr)
		strNew += '\"';

	str = strNew;
}
