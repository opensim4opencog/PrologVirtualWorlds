// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// PropMapObj.cpp : Implementation of CfooApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include "collenum.h"
#include "pmapobj.h"
#include "propsecu.h"

//#define TRACE_MARSHALL VWT_NONE
#define TRACE_MARSHALL VWT_VERBOSE

IMPLEMENT_OBJECTPROPERTY(CPropertyMapObject);

typedef CComObject<CPropertySecurity> CComPropertySecurity;

/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP CPropertyMapObject::ToAscii(BSTR* pbstr)
{
	ASSERT(pbstr);
	if (pbstr == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::ToAscii: outgoing pointer (BSTR*) is NULL\n");
		return E_POINTER;
	}

	CString strValue;
	int iSize = m_varMap.GetCount();

	if (iSize == 1)
		strValue.Format("1 property");
	else
		strValue.Format("%d properties", iSize);

	*pbstr = strValue.AllocSysString();

	return S_OK;
}

STDMETHODIMP CPropertyMapObject::Dump(void)
{
	if (m_varMap.IsEmpty())
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "PropertyMap: (empty)\n");
	}
	else
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "PropertyMap: begin\n");


		POSITION pos = m_varMap.GetStartPosition();
		while (pos)
		{
			CComSecurityPtr SecPtr;
			CComBSTR bstr;
			CComVariant var;

			m_varMap.GetNextAssoc(pos, (CBSTRAdapt<CComBSTR>&)bstr, SecPtr);
			ASSERT(SecPtr != NULL);

			if (SecPtr != NULL)
			{
				if (FAILED(SecPtr->get_Property(&var)))
					break;

				VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "PropertyMap: property '%s'\n", CString(bstr));

				m_pWorld->DumpProperty(var);
			}
		}

		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "PropertyMap: end\n");
	}

	return S_OK;
}

STDMETHODIMP CPropertyMapObject::IsValid(BSTR bstrName, VARIANT_BOOL* pboolVal)
{
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CPropertyMapObject::IsValid\n");

	ASSERT(pboolVal);
	if (pboolVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::IsValid: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return E_POINTER;
	}

	*pboolVal = (SUCCEEDED(get_Property(bstrName, NULL))) ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CPropertyMapObject::get_Property(BSTR bstrName, VARIANT* pvar)
{
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CPropertyMapObject::get_Property\n");

	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::get_Property: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	HRESULT hr = S_OK;
	UINT nHashOrig = HashKey<BSTR>(bstrName);

	hr = get_PropertyExt(bstrName, nHashOrig, pvar);

	return hr;
}

STDMETHODIMP CPropertyMapObject::get_PropertyExt(BSTR bstrName, UINT nHashOrig, VARIANT* pvar)
{
	CComSecurityPtr SecPtr;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CPropertyMapObject::get_PropertyExt\n");

	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::get_PropertyExt: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	// NOTE: pvar can be NULL

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CPropertyMapObject::get_PropertyExt: bstrName (%s), hash key %d\n", CString(bstrName), nHashOrig);
#endif

	if (m_pParent)
	{
		if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_READ)))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::get_PropertyExt: security violation (PS_READ), bstrName (%s)\n", CString(bstrName));
			return VWOBJECT_E_PERMISSION_DENIED;
		}
	}

	if (m_varMap.LookupExt(bstrName, nHashOrig, SecPtr))
	{
		ASSERT(SecPtr != NULL);

		if (SecPtr != NULL)
		{
			if (pvar)
			{
				HRESULT hr = S_OK;

				hr = SecPtr->get_Property(pvar);

				if (FAILED(hr))
					return hr;
			}
		}
	}
	else
	{
		// changed to VWT_IMPORTANT since this happens often
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CPropertyMapObject::get_PropertyExt: property not found (%s)\n", CString(bstrName));
#endif
		return VWOBJECT_E_PROPERTYNOTEXIST;
	}

	return S_OK;
}

STDMETHODIMP CPropertyMapObject::put_Property(BSTR bstrName, VARIANT var)
{
	HRESULT hr = S_OK;
	CComVariant varTemp;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CPropertyMapObject::put_Property\n");

	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::put_Property: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

#ifdef _DEBUG
	CString strName(bstrName);		
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CPropertyMapObject::put_Property: bstrName (%s)\n", strName);
#endif

	hr = VariantCopyInd(&varTemp, &var);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (m_pWorld)
	{
		hr = m_pWorld->ValidateProperty(varTemp);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CPropertyMapObject::put_Property", "VWOBJECT");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		hr = put_PropertyInt(bstrName, varTemp, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 2);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[1].vt = VT_BSTR;
		dispparams.rgvarg[1].bstrVal = bstrName;

		memcpy(&dispparams.rgvarg[0], &varTemp, sizeof(VARIANT));

		hr = SendObjectPropertyCommand(psbits, DISPID_PROPERTYMAP_PROPERTY, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, pmPut, CComVariant(bstrName));
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CPropertyMapObject::put_PropertyInt(BSTR bstrName, VARIANT var, IPropertySecurity* pSecurity)
{
	HRESULT hr = S_OK;
	CComPropertySecurity* pPropSec = NULL;
	CComSecurityPtr SecPtr;

	// if we have a parent, then we can't embed a 
	// collection object in ourselves

	if (m_pParent != NULL && var.vt == VT_DISPATCH && var.pdispVal != NULL)
	{
		IPropertyCollection* ppropcoll = NULL;

		// verify it's not an IPropertyCollection ptr
		if (SUCCEEDED(var.pdispVal->QueryInterface(IID_IPropertyCollection, (LPVOID*)&ppropcoll)))
		{
			SAFERELEASE(ppropcoll);

			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::put_PropertyInt: cannot embed collection in property map\n");
			hr = VWOBJECT_E_COLLECTIONLEVEL;
			goto ERROR_ENCOUNTERED;
		}
	}

	// Get the existing slot
	if (!m_varMap.Lookup(bstrName, SecPtr))
	{
		// if new property, check PS_ADD
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_ADD)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::put_PropertyInt: security violation (PS_ADD), name (%s)\n", CString(bstrName));
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		pPropSec = new CComPropertySecurity();
		if (pPropSec == NULL)
		{
			hr = E_OUTOFMEMORY;
			goto ERROR_ENCOUNTERED;
		}

		pPropSec->AddRef();

		if (pPropSec != NULL)
		{
			pPropSec->Initialize(m_pWorld, m_pParent, (pSecurity == NULL) ? m_pSecurity : pSecurity);

			// if inheriting security from map itself, reset type/prop type/prop iid
			// this will just copy the exemplar, instance, bits and permissions
			if (pSecurity == NULL)
			{
				pPropSec->m_vtType = VT_VARIANT;
				pPropSec->put_PropertyType(NULL);
				pPropSec->put_PropertyIIDExt(IID_NULL);
			}

			// assign it to smartptr
			SecPtr = pPropSec;
			ASSERT(SecPtr != NULL);

			SAFERELEASE(pPropSec);
		}
	}
	else
	{
		// if existing property, check PS_WRITE
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::put_PropertyInt: security violation (PS_WRITE), name (%s)\n", CString(bstrName));
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}
	}

	if (FAILED(hr = SecPtr->put_Property(var)))
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::put_Property: name (%s), failed to put property into security slot\n", CString(bstrName));
#endif
		goto ERROR_ENCOUNTERED;
	}

	m_varMap.SetAt(bstrName, SecPtr);

ERROR_ENCOUNTERED:
	SAFERELEASE(pPropSec);

	return hr;
}

STDMETHODIMP CPropertyMapObject::get_PropertySecurity(BSTR bstrName, IPropertySecurity **ppSecurity)
{
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CPropertyMapObject::get_PropertySecurity\n");

	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::get_PropertySecurity: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	HRESULT hr = S_OK;
	UINT nHashOrig = HashKey<BSTR>(bstrName);

	hr = get_PropertySecurityExt(bstrName, nHashOrig, ppSecurity);

	return hr;
}

STDMETHODIMP CPropertyMapObject::get_PropertySecurityExt(BSTR bstrName, UINT nHashOrig, IPropertySecurity **ppSecurity)
{
	CComSecurityPtr SecPtr;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CPropertyMapObject::get_PropertySecurityExt\n");

	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::get_PropertySecurityExt: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

#ifdef _DEBUG
	CString strName(bstrName);
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CPropertyMapObject::get_PropertySecurityExt: bstrName (%s)\n", strName);
#endif

	if (ppSecurity == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::get_PropertySecurityExt: outgoing pointer (IPropertySecurity**) is NULL\n");
		return E_POINTER;
	}

	*ppSecurity = NULL;

	if (m_pParent)
	{
		if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_READ)))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::get_PropertySecurityExt: security violation (PS_READ), bstrName (%s)\n", CString(bstrName));
			return VWOBJECT_E_PERMISSION_DENIED;
		}
	}

	if (m_varMap.LookupExt(bstrName, nHashOrig, SecPtr))
	{
		ASSERT(SecPtr != NULL);

		*ppSecurity = SecPtr;
		SAFEADDREF(*ppSecurity);
	}
	else
	{
		// changed to VWT_IMPORTANT since this happens often
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CPropertyMapObject::get_PropertySecurityExt: property not found (%s)\n", strName);
#endif
		return VWOBJECT_E_PROPERTYNOTEXIST;
	}

	return S_OK;
}

// REVIEW: get_PropertyAtIndex doesn't check security
STDMETHODIMP CPropertyMapObject::get_PropertyAtIndex(long index, VARIANT *pvar)
{
	HRESULT hr = S_OK;
	POSITION pos;
	CComSecurityPtr SecPtr;
	CComBSTR bstr;
	long i;

	ASSERT(pvar);
	if (pvar == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::get_PropertyAtIndex: outgoing pointer (VARIANT*) is NULL\n");
		return E_POINTER;
	}

	::VariantClear(pvar);

	if (index < 0 || index >= m_varMap.GetCount())
		return E_INVALIDARG;

	// seek to the indicated item
	pos = m_varMap.GetStartPosition();
	for(i=0;;i++)
	{
		// get the item
		m_varMap.GetNextAssoc(pos, (CBSTRAdapt<CComBSTR>&)bstr, SecPtr);

		// done?
		if (pos==NULL || i==index)
			break;
	}

	// make sure we didn't run out early!
	if (i < index)
		return E_INVALIDARG;

	// return the name of the property
	pvar->vt = VT_BSTR;
	pvar->bstrVal = bstr.Copy();

	return S_OK;
}

STDMETHODIMP CPropertyMapObject::get__NewEnum(IUnknown **ppunk)
{
	CEnumCollection *pec;
	HRESULT hr;

	ASSERT(ppunk);
	if (ppunk == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::get__NewEnum: outgoing pointer (IUnknown**) is NULL\n");
		return E_POINTER;
	}

	*ppunk = NULL;

	// allocate an enumerator
	pec = new CEnumCollection();
	if (pec == NULL)
		return E_OUTOFMEMORY;
	pec->AddRef();

	// initialize it
	hr = pec->Initialize(this);
	if (FAILED(hr))
	{
		pec->Release();
		return hr;
	}

	// return it
	*ppunk = (IUnknown*)pec;

	return S_OK;
}

void CPropertyMapObject::EmptyMap()
{
	POSITION pos = m_varMap.GetStartPosition();

	while (pos != NULL)
	{
		CComBSTR bstr;
		CComSecurityPtr SecPtr;

		m_varMap.GetNextAssoc(pos, (CBSTRAdapt<CComBSTR>&)bstr, SecPtr);
		ASSERT(SecPtr != NULL);

		if (SecPtr != NULL)
		{
#ifdef _DEBUG
//			VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CPropertyMapObject::EmptyMap: name (%s)\n", CString(bstr));
#endif

			SecPtr->Cleanup();
		}
	}

	m_varMap.RemoveAll();
}

STDMETHODIMP CPropertyMapObject::Remove(BSTR bstrName)
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CPropertyMapObject::Remove\n");

	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::Remove: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

#ifdef _DEBUG
	CString strName(bstrName);
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CPropertyMapObject::Remove: bstrName (%s)\n", strName);
#endif

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CPropertyMapObject::Remove", "VWOBJECT");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		hr = RemoveInt(bstrName);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstrName;

		hr = SendObjectPropertyCommand(psbits, DISPID_PROPERTYMAP_REMOVE, DISPATCH_METHOD, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, pmRemove, CComVariant(bstrName));
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CPropertyMapObject::RemoveInt(BSTR bstrName)
{
	HRESULT hr = S_OK;
	CComSecurityPtr SecPtr;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CPropertyMapObject::RemoveInt\n");

	if (m_pParent)
	{
		if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_ADD)))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::RemoveInt: security violation (PS_ADD), name (%s)\n", CString(bstrName));
			hr = VWOBJECT_E_PERMISSION_DENIED;
			goto ERROR_ENCOUNTERED;
		}
	}

	if (!m_varMap.Lookup(bstrName, SecPtr))
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::RemoveInt: property not found (%s)\n", CString(bstrName));
#endif
		hr = VWOBJECT_E_PROPERTYNOTEXIST;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(SecPtr != NULL);

	if (SecPtr != NULL)
	{
#if 0
		if (!QUIET_SAFE_CHECKSECURITY(m_pWorld, SecPtr.p, PS_WRITE))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::RemoveInt: security violation (PS_WRITE), name (%s)\n", CString(bstrName));
			hr = VWOBJECT_E_PERMISSION_DENIED;
			goto ERROR_ENCOUNTERED;
		}
#endif

		SecPtr->Cleanup();
	}
	else
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::RemoveInt: security slot is NULL\n");

		hr = VWOBJECT_E_INVALIDSECURITY;
		goto ERROR_ENCOUNTERED;
	}

	m_varMap.RemoveKey(bstrName);

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CPropertyMapObject::RemoveAll()
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CPropertyMapObject::RemoveAll\n"); 
#endif

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CPropertyMapObject::RemoveAll", "VWOBJECT");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_ADD)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::RemoveAll: security violation (PS_ADD)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

#if 0
		if (!QUIET_SAFE_CHECKSECURITY(m_pWorld, m_pSecurity, PS_WRITE))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::RemoveAll: security violation (PS_WRITE)\n");
			hr = VWOBJECT_E_PERMISSION_DENIED;
			goto ERROR_ENCOUNTERED;
		}
#endif

		EmptyMap();
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 0);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = SendObjectPropertyCommand(psbits, DISPID_PROPERTYMAP_REMOVEALL, DISPATCH_METHOD, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, pmRemoveAll, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

// REVIEW: FirstItem, NextItem don't check security
STDMETHODIMP CPropertyMapObject::FirstItem(BSTR* pbstrName, VARIANT* pvar, VARIANT_BOOL* pboolVal)
{
	m_curpos = m_varMap.GetStartPosition();

	return NextItem(pbstrName, pvar, pboolVal);
}

STDMETHODIMP CPropertyMapObject::NextItem(BSTR* pbstrName, VARIANT* pvar, VARIANT_BOOL* pboolVal)
{
	HRESULT hr = S_OK;

	ASSERT(pvar);
	if (pvar == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::NextItem: outgoing pointer (VARIANT*) is NULL\n");
		return E_POINTER;
	}

	::VariantClear(pvar);

	ASSERT(pboolVal);
	if (pboolVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::NextItem: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return E_POINTER;
	}

	if (m_curpos)
	{
		*pboolVal = VARIANT_FALSE;

		CComBSTR bstr;
		CComSecurityPtr SecPtr;

		m_varMap.GetNextAssoc(m_curpos, (CBSTRAdapt<CComBSTR>&)bstr, SecPtr);
		ASSERT(SecPtr != NULL);

		// bstrName is optional retval
		if (pbstrName)
			*pbstrName = bstr.Copy();

		hr = SecPtr->get_Property(pvar);
	}
	else
		*pboolVal = VARIANT_TRUE;

	return hr;
}

// REVIEW: CopyTo doesn't check security

// REVIEW: won't work correctly on server since put_Property gets remoted
STDMETHODIMP CPropertyMapObject::CopyTo(IPropertyMap* ppropertymap)
{
	HRESULT hr = S_OK;
	IWorld* pWorld = NULL;

	// grab IWorld* from incoming pmap since source might not have been initialized with world
	/* hr = */ ppropertymap->get_World(&pWorld);

	if (pWorld == NULL)
		return VWOBJECT_E_INVALIDWORLD;

	POSITION pos = m_varMap.GetStartPosition();
	while (pos && SUCCEEDED(hr))
	{
		CComBSTR bstr;
		CComSecurityPtr SecPtr;
		CComVariant var, dest;

		m_varMap.GetNextAssoc(pos, (CBSTRAdapt<CComBSTR>&)bstr, SecPtr);
		ASSERT(SecPtr != NULL);

		// REVIEW: sort of a hack, do checksecurity here before calling get_Property so we can
		// do it quietly
		if (!QUIET_SAFE_CHECKSECURITY(m_pWorld, (IPropertySecurity*)SecPtr, PS_READ))
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CPropertyMapObject::CopyTo: skipping property (%s), permission denied\n", CString(bstr));
#endif
			hr = S_OK;
			continue;
		}

		hr = SecPtr->get_Property(&var);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pWorld->CloneProperty(var, &dest);
		if (FAILED(hr))	
			goto ERROR_ENCOUNTERED;

		hr = ppropertymap->put_Property(bstr, dest);
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pWorld);

	return hr;
}

STDMETHODIMP CPropertyMapObject::Marshall(DWORD dwOptions, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;
	long propcnt = 0;
	POSITION pos;
	long psbits = 0;

	if (FAILED(hr = CPropertyMapObjectBase::Marshall(dwOptions, pbuffer)))
		goto ERROR_ENCOUNTERED;

	hr = get_Count(&propcnt);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CPropertyMapObject::Marshall: marshalling number of properties in map (%d)\n", propcnt);

	// marshall property count
	hr = pbuffer->put_Long(propcnt);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	pos = m_varMap.GetStartPosition();
	while (pos && SUCCEEDED(hr))
	{
		CComBSTR bstr;
		CComSecurityPtr SecPtr;

		m_varMap.GetNextAssoc(pos, (CBSTRAdapt<CComBSTR>&)bstr, SecPtr);
		ASSERT(SecPtr != NULL);

#ifdef _DEBUG
//		VWTRACE(m_pWorld, "VWOBJECT", VWT_NONE, "CPropertyMapObject::Marshall: marshalling property (%s)\n", CString(bstr));
#endif

		hr = SecPtr->get_Bits(&psbits);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (EXTRACTLOCATION(dwOptions) == MARSHALL_TODISK)
		{
			// don't write out volatile properties to disk (write out tag byte)
			// but, do write out local properties (since we're on the server)
			if (psbits & PSBIT_NOTLOGGED)
			{
#ifdef _DEBUG		
				VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALL, "CPropertyMapObject::Marshall: (MARSHALL_TODISK) skipping not-logged property (%s)\n", CString(bstr));
#endif

				hr = pbuffer->put_Byte(0);
				if (FAILED(hr)) 
					break;

				continue;
			}
		}
		else if (EXTRACTLOCATION(dwOptions) == MARSHALL_TONETWORK)
		{
			// REVIEW: took out since there was some reliance on getting client local properties
			// marshalled to all clients
#if 1
			// don't write out properties which are both local and volatile to network (write out tag byte)
			if ((psbits & PSBIT_NOTREMOTED) && (psbits & PSBIT_NOTLOGGED))
			{
#ifdef _DEBUG		
				VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALL, "CPropertyMapObject::Marshall: (MARSHALL_TONETWORK) skipping %s property (%s)\n", 
					(psbits & PSBIT_NOTREMOTED) ? "not-remoted" : "not-logged", CString(bstr));
#endif

				hr = pbuffer->put_Byte(0);
				if (FAILED(hr)) 
					break;

				continue;
			}
#endif
		}
		else if (EXTRACTLOCATION(dwOptions) == MARSHALL_TOMEMORY)
		{
			// no-op: don't skip any properties
		}
		else
		{
			// didn't have MARSHALL_TOXXX set
			ASSERT(FALSE);
		}

		hr = pbuffer->put_Byte(1);
		if (FAILED(hr)) 
			break;

		hr = pbuffer->put_String(bstr);
		if (FAILED(hr)) 
			break;

#ifdef _DEBUG		
		CString strName(bstr);
		VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CPropertyMapObject::Marshall: marshalling property (%s)\n", strName);
#endif

		if (psbits & PSBIT_BYVAL)
		{
			hr = SecPtr->Marshall(MARSHALLREF(dwOptions, MARSHALL_BYVALUE), pbuffer);
		}
		else
		{
			hr = SecPtr->Marshall(dwOptions, pbuffer);
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CPropertyMapObject::UnMarshall(IUnMarshallBuffer* pbuffer)
{
	long propcnt = 0;
	CComBSTR bstr;
	int i = 0;
	unsigned char tag = 0;
	HRESULT hr = S_OK;

	// kill off previous data (done by IPropertySecurity::UnMarshall)
	EmptyMap();

	if (FAILED(hr = CPropertyMapObjectBase::UnMarshall(pbuffer)))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->get_Long(&propcnt);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CPropertyMapObject::UnMarshall: unmarshalling number of properties in map (%d)\n", propcnt);

	for (i = 0; i < propcnt; i++)
	{
		hr = pbuffer->get_Byte(&tag);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::UnMarshall: failed to unmarshall tag byte\n");
			break;
		}

		// skipped property
		if (tag == 0)
			continue;

		hr = pbuffer->get_String(&bstr.m_str);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::UnMarshall: failed to unmarshall key\n");
			break;
		}

#ifdef _DEBUG
//		VWTRACE(m_pWorld, "VWOBJECT", VWT_NONE, "CPropertyMapObject::UnMarshall: unmarshalling property (%s)\n", CString(bstr));
#endif

		CComSecurityPtr PropSec;

		typedef CComObject<CPropertySecurity> CComPropertySecurity;
		CComPropertySecurity* pPropSec = NULL;

		pPropSec = new CComPropertySecurity();
		if (pPropSec == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::UnMarshall: failed to create propertysecurity slot\n");
			hr = E_OUTOFMEMORY;
			goto ERROR_ENCOUNTERED;
		}

		pPropSec->AddRef();

		// assign it to smartptr
		PropSec = pPropSec;

		pPropSec->Release();

//		PropSec.CoCreateInstance();
		ASSERT(PropSec != NULL);

		PropSec->put_World(m_pWorld);

		hr = PropSec->UnMarshall(pbuffer);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyMapObject::UnMarshall: failed to unmarshall propertysecurity slot\n");
			break;
		}

		// add property to property map
		// NOTE: same as core code from put_Property (but didn't want to remote it)
		m_varMap.SetAt(bstr, PropSec);

		bstr.Empty();
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CPropertyMapObject::MarshallToScript(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bDone;
	VARIANT_BOOL bPropAsVal;
	CComVariant var;
	CComBSTR bstrName;
	CComBSTR bstrOut;
	CString strVal;
	CString strOne;
	CString strOut;
	CComBSTR bstrContextSub;

	if (pbstrOut==NULL || bstrContext==NULL)
		return E_INVALIDARG;

	// initialize
	*pbstrOut = NULL;
	*pbAsVal = VARIANT_FALSE;

	strOut.Format("\tSet %s = World.CreatePropertyMap\n", (LPCTSTR) CString(bstrContext));

	// calculate new context
	bstrContextSub = bstrContext;
	bstrContextSub.Append("_t");

	// write out values
	hr = FirstItem(&bstrName.m_str, &var, &bDone);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	while (bDone!=VARIANT_TRUE)
	{
		// marshall out the property
		hr = m_pWorld->MarshallPropertyToScript(var, dwOptions, bstrContextSub, &bPropAsVal, &bstrOut.m_str);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if ((BSTR)bstrOut == NULL)
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}

		// handle value and context cases
		if (bPropAsVal==VARIANT_TRUE)
		{
			strVal = bstrOut;
		}
		else
		{
			strOut += CString(bstrOut);
			strVal = bstrContextSub;

		}

		// add the value to the list
		strOne.Format( "\t%s.Property(\"%s\") = %s\n",
						(LPCTSTR) CString(bstrContext),
						(LPCTSTR) CString(bstrName),
						(LPCTSTR) strVal );

		strOut += strOne;

		// free stuff
		bstrName.Empty();
		bstrOut.Empty();
		var.Clear();

		// go on to the next one
		hr = NextItem( &bstrName.m_str, &var, &bDone);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	// allocate
	*pbstrOut = strOut.AllocSysString();

ERROR_ENCOUNTERED:
	return hr;
}