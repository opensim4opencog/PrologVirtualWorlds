// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// PropListObj.cpp : Implementation of CfooApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include "collenum.h"
#include "plistobj.h"
#include "propsecu.h"

IMPLEMENT_OBJECTPROPERTY(CPropertyListObject);

typedef CComObject<CPropertySecurity> CComPropertySecurity;

#define ReportObjectPropertyError(hr) SUCCEEDED(hr) ? hr : ReportError("VWSYSTEM.PropertyList.1", hr, 0x200, 0x300, "VWOBJECT")


/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP CPropertyListObject::ToAscii(BSTR* pbstr)
{
	ASSERT(pbstr);
	if (pbstr == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::ToAscii: outgoing pointer (BSTR*) is NULL\n");
		return E_POINTER;
	}

	CString strValue;
	int iSize = m_varList.GetSize();

	if (iSize == 1)
		strValue.Format("1 property");
	else
		strValue.Format("%d properties", iSize);

	*pbstr = strValue.AllocSysString();

	return S_OK;
}

STDMETHODIMP CPropertyListObject::Dump(void)
{
	CComVariant var;
	CComSecurityPtr SecPtr;

	if (m_varList.GetSize() == 0)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "PropertyList: (empty)\n");
	}
	else
	{
		CComVariant var;
		int iCount = m_varList.GetSize();

		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "PropertyList: begin\n");

//		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "\n");

		for (int i = 0; i < iCount; i++)
		{
			SecPtr = m_varList.GetAt(i);
			ASSERT(SecPtr != NULL);

			if (SecPtr != NULL)
			{
				SecPtr->get_Property(&var);

				m_pWorld->DumpProperty(var);

				var.Clear();
			}
		}

		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "PropertyList: end\n");
	}

	return S_OK;
}

STDMETHODIMP CPropertyListObject::Add(VARIANT var)
{
	HRESULT hr = S_OK;
	CComVariant varTemp;
	long psbits = 0;
	long newIndex = -1;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;
	CComPropertySecurity* pPropSec = NULL;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CPropertyListObject::Add\n");
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
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CPropertyListObject::Add", "VWOBJECT");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_ADD)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::Add: security violation (PS_ADD)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		// if we have a parent, then we can't embed a 
		// collection object in ourselves
		if (m_pParent != NULL && varTemp.vt == VT_DISPATCH && varTemp.pdispVal != NULL)
		{
			IPropertyCollection* ppropcoll = NULL;

			// verify it's not an IPropertyCollection ptr
			if (SUCCEEDED(varTemp.pdispVal->QueryInterface(IID_IPropertyCollection, (LPVOID*)&ppropcoll)))
			{
				SAFERELEASE(ppropcoll);

				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::Add: cannot embed collection in property list\n");
				hr = VWOBJECT_E_COLLECTIONLEVEL;
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
			// Create A Security Slot...
			CComSecurityPtr SecPtr;

			// Add the world, exemplar
			pPropSec->Initialize(m_pWorld, m_pParent, m_pSecurity);

			// set the type to variant
			pPropSec->m_vtType = VT_VARIANT;
			pPropSec->put_PropertyType(NULL);
			pPropSec->put_PropertyIIDExt(IID_NULL);

			// REVIEW: for completeness, should check the slot for PS_WRITE

			// Add the variant to the slot
			if (FAILED(hr = pPropSec->put_Property(varTemp)))
			{
#ifdef _DEBUG
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::Add: failed to put property into security slot\n");
#endif
				goto ERROR_ENCOUNTERED;
			}

			// assign it to smartptr
			SecPtr = pPropSec;
			ASSERT(SecPtr != NULL);

			newIndex = m_varList.Add(SecPtr);

			SAFERELEASE(pPropSec);
		}
		else
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::Add: security slot is NULL\n");

			hr = VWOBJECT_E_INVALIDSECURITY;
			goto ERROR_ENCOUNTERED;
		}
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		memcpy(&dispparams.rgvarg[0], &varTemp, sizeof(VARIANT));

		hr = SendObjectPropertyCommand(psbits, DISPID_PROPERTYLIST_ADD, DISPATCH_METHOD, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			ASSERT(newIndex != -1);

			// notify client/server and world of property change
			hr = m_pParent->NotifyObjectPropertyChanged(this, plAdd, CComVariant(newIndex, VT_I4));
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pPropSec);

	return hr;
}

STDMETHODIMP CPropertyListObject::InsertAt(long Index, VARIANT var)
{
	HRESULT hr = S_OK;
	CComVariant varTemp;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;
	CComPropertySecurity* pPropSec = NULL;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CPropertyListObject::InsertAt\n");
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
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CPropertyListObject::InsertAt", "VWOBJECT");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_ADD)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::InsertAt: security violation (PS_ADD), index %d\n", Index);
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		// if we have a parent, then we can't embed a 
		// collection object in ourselves
		if (m_pParent != NULL && varTemp.vt == VT_DISPATCH && varTemp.pdispVal != NULL)
		{
			IPropertyCollection* ppropcoll = NULL;

			// verify it's not an IPropertyCollection ptr
			if (SUCCEEDED(varTemp.pdispVal->QueryInterface(IID_IPropertyCollection, (LPVOID*)&ppropcoll)))
			{
				SAFERELEASE(ppropcoll);

				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::InsertAt: cannot embed collection in property list\n");
				hr = VWOBJECT_E_COLLECTIONLEVEL;
				goto ERROR_ENCOUNTERED;
			}
		}

		// Create A Security Slot...
		CComSecurityPtr SecPtr;

		pPropSec = new CComPropertySecurity();
		if (pPropSec == NULL)
		{
			hr = E_OUTOFMEMORY;
			goto ERROR_ENCOUNTERED;
		}

		pPropSec->AddRef();

		if (pPropSec != NULL)
		{
			// Set default security, world, exemplar etc...
			pPropSec->Initialize(m_pWorld, m_pParent, m_pSecurity);

			// set the type to variant
			pPropSec->m_vtType = VT_VARIANT;
			pPropSec->put_PropertyType(NULL);
			pPropSec->put_PropertyIIDExt(IID_NULL);
		
			// REVIEW: for completeness, should check the slot for PS_WRITE

			// Add the variant to the slot
			if (FAILED(hr = pPropSec->put_Property(varTemp)))
			{
#ifdef _DEBUG
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::InsertAt: index %d, failed to put property into security slot\n", Index);
#endif
				goto ERROR_ENCOUNTERED;
			}

			// assign it to smartptr
			SecPtr = pPropSec;
			ASSERT(SecPtr != NULL);

			m_varList.InsertAt(Index, SecPtr);

			SAFERELEASE(pPropSec);
		}
		else
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::InsertAt: security slot is NULL\n");

			hr = VWOBJECT_E_INVALIDSECURITY;
			goto ERROR_ENCOUNTERED;
		}
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 2);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[1].vt = VT_I4;
		dispparams.rgvarg[1].lVal = Index;

		memcpy(&dispparams.rgvarg[0], &varTemp, sizeof(VARIANT));

		hr = SendObjectPropertyCommand(psbits, DISPID_PROPERTYLIST_INSERTAT, DISPATCH_METHOD, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			// notify client/server and world of property change
			hr = m_pParent->NotifyObjectPropertyChanged(this, plInsertAt, CComVariant(Index, VT_I4));
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pPropSec);

	return hr;
}

STDMETHODIMP CPropertyListObject::Remove(VARIANT var)
{
	HRESULT hr = S_OK;
	CComVariant varTemp;
	long psbits = 0;
	long removeIndex = -1;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CPropertyListObject::Remove\n");
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
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CPropertyListObject::Remove", "VWOBJECT");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_ADD)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::Remove: security violation (PS_ADD)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		int iCount = m_varList.GetSize();

		if (iCount == 0)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::Remove: list already empty\n");
			hr = VWOBJECT_E_EMPTYPROPERTYLIST;
			goto ERROR_ENCOUNTERED;
		}

		BOOL bFound = FALSE;

		for (int i = 0; i < iCount; i++)
		{
			CComSecurityPtr SecPtr = m_varList.GetAt(i);

			if (SecPtr != NULL)
			{
				CComVariant cvar;

				if (FAILED(hr = SecPtr->get_Property(&cvar)))
					goto ERROR_ENCOUNTERED;

				// did we find it?
				if (cvar == varTemp) 
				{
					// REVIEW: should check the slot for PS_WRITE

					// clear out the property reference
					SecPtr->Cleanup();
		
					bFound = TRUE;
					break;
				}
			}
			else
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::Remove: property not found\n");

				hr = VWOBJECT_E_PROPERTYNOTEXIST;
				goto ERROR_ENCOUNTERED;
			}
		}

		if (!bFound)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::Remove: property not found\n");

			hr = VWOBJECT_E_PROPERTYNOTEXIST;
			goto ERROR_ENCOUNTERED;
		}

		removeIndex = i;

		m_varList.RemoveAt(i);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		memcpy(&dispparams.rgvarg[0], &varTemp, sizeof(VARIANT));

		hr = SendObjectPropertyCommand(psbits, DISPID_PROPERTYLIST_REMOVE, DISPATCH_METHOD, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			ASSERT(removeIndex != -1);

			// notify client/server and world of property change
			hr = m_pParent->NotifyObjectPropertyChanged(this, plRemove, CComVariant(removeIndex, VT_I4));
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CPropertyListObject::RemoveAt(long Index)
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CPropertyListObject::RemoveAt: %d\n", Index);
#endif

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CPropertyListObject::RemoveAt", "VWOBJECT");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_ADD)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::RemoveAt: security violation (PS_ADD), index %d\n", Index);
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		CComSecurityPtr SecPtr;

		if (Index < 0 || Index > m_varList.GetUpperBound())
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CPropertyListObject::RemoveAt: invalid property index (%d)\n", Index);
			hr = VWOBJECT_E_INVALIDLISTINDEX;
			goto ERROR_ENCOUNTERED;
		}

		// must remember to clean things up, because of backpointers
		SecPtr = m_varList.GetAt(Index);

		if (SecPtr != NULL)
		{
			// REVIEW: should check the slot for PS_WRITE

			SecPtr->Cleanup();

			m_varList.RemoveAt(Index);
		}
		else
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::RemoveAt: property not found\n");

			hr = VWOBJECT_E_PROPERTYNOTEXIST;
			goto ERROR_ENCOUNTERED;
		}
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_I4;
		dispparams.rgvarg[0].lVal = Index;

		hr = SendObjectPropertyCommand(psbits, DISPID_PROPERTYLIST_REMOVEAT, DISPATCH_METHOD, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			// notify client/server and world of property change
			hr = m_pParent->NotifyObjectPropertyChanged(this, plRemoveAt, CComVariant(Index, VT_I4));
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CPropertyListObject::RemoveAll()
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CPropertyListObject::RemoveAll\n");
#endif

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CPropertyListObject::RemoveAll", "VWOBJECT");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_ADD)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::RemoveAll: security violation (PS_ADD)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		EmptyList();
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 0);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = SendObjectPropertyCommand(psbits, DISPID_PROPERTYLIST_REMOVEALL, DISPATCH_METHOD, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			// notify client/server and world of property change
			hr = m_pParent->NotifyObjectPropertyChanged(this, plRemoveAll, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

// REVIEW: Find doesn't check security
STDMETHODIMP CPropertyListObject::Find(VARIANT var, long* plval)
{
	HRESULT hr = S_OK;
	CComVariant varTemp;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CPropertyListObject::Find\n");
#endif

	ASSERT(plval);
	if (plval == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::Find: outgoing pointer (long*) is NULL\n");
		return E_POINTER;
	}

	*plval = -1;

	hr = VariantCopyInd(&varTemp, &var);
	if (FAILED(hr))
		return hr;

	int iCount = m_varList.GetSize();
	BOOL bFound = FALSE;

	for (int i = 0; i < iCount; i++)
	{
		CComSecurityPtr SecPtr = m_varList.GetAt(i);

		if (SecPtr != NULL)
		{
			CComVariant cvar;

			if (FAILED(hr = SecPtr->get_Property(&cvar)))
				// REVIEW: error, just kick out
				break;

			// did we find it?
			if (cvar == varTemp) 
			{
				bFound = TRUE;
				break;
			}
		}
		else
			// REVIEW: error, just kick out
			break;
	}

	if (bFound)
		*plval = i;

	return hr;
}

STDMETHODIMP CPropertyListObject::get_Property(long Index, VARIANT* pvar)
{
	HRESULT hr = S_OK;

	ASSERT(pvar);
	if (pvar == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::get_Property: outgoing pointer (VARIANT*) is NULL\n");
		return E_POINTER;
	}

	::VariantClear(pvar);

	if (Index < 0 || Index > m_varList.GetUpperBound())
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CPropertyListObject::get_Property: invalid property index (%d)\n", Index);
		return VWOBJECT_E_INVALIDLISTINDEX;
	}

	if (m_pParent)
	{
		if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_READ)))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::get_Property: security violation (PS_READ), index %d\n", Index);
			return VWOBJECT_E_PERMISSION_DENIED;
		}
	}

	CComSecurityPtr SecPtr;

	SecPtr = m_varList.GetAt(Index);
	ASSERT(SecPtr != NULL);

	if (SecPtr != NULL)
	{
		// No need to check security, the slot will return an error if the property isn't readable
		hr = SecPtr->get_Property(pvar);
	}
	else
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::get_Property: property not found\n");

		hr = VWOBJECT_E_PROPERTYNOTEXIST;
		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CPropertyListObject::put_Property(long Index, VARIANT var)
{
	HRESULT hr = S_OK;
	CComVariant varTemp;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CPropertyListObject::put_Property: %d\n", Index);
#endif

	hr = VariantCopyInd(&varTemp, &var);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (m_pWorld)
	{
		hr = m_pWorld->ValidateProperty(varTemp);
		if (FAILED(hr))
			return hr;
	}

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CPropertyListObject::put_Property", "VWOBJECT");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		// No Need to check security here...
		// The slot decides whether it is readable...

		// if we have a parent, then we can't embed a 
		// collection object in ourselves
		// Comment: varTemp.pdispVal wasn't being tested before being used
		if ((m_pParent != NULL) && (varTemp.vt == VT_DISPATCH) && (varTemp.pdispVal != NULL))
		{
			IPropertyCollection* ppropcoll = NULL;

			// verify it's not an IPropertyCollection ptr
			if (SUCCEEDED(varTemp.pdispVal->QueryInterface(IID_IPropertyCollection, (LPVOID*)&ppropcoll)))
			{
				SAFERELEASE(ppropcoll);

				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::put_Property: cannot embed collection in property list\n");
				hr = VWOBJECT_E_COLLECTIONLEVEL;
				goto ERROR_ENCOUNTERED;
			}
		}

		if (Index < 0 || Index > m_varList.GetUpperBound())
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CPropertyListObject::put_Property: invalid property index (%d)\n", Index);
			hr = VWOBJECT_E_INVALIDLISTINDEX;
			goto ERROR_ENCOUNTERED;
		}

		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::put_Property: security violation (PS_WRITE), index %d\n", Index);
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		CComSecurityPtr SecPtr = m_varList.GetAt(Index);
		ASSERT(SecPtr != NULL);

		if (SecPtr != NULL)
		{
			if (FAILED(hr = SecPtr->put_Property(varTemp)))
			{
#ifdef _DEBUG
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::put_Property: index %d, failed to put property into security slot\n", Index);
#endif
				goto ERROR_ENCOUNTERED;
			}
		}
		else
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::put_Property: property not found\n");

			hr = VWOBJECT_E_PROPERTYNOTEXIST;
			goto ERROR_ENCOUNTERED;
		}
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 2);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[1].vt = VT_I4;
		dispparams.rgvarg[1].lVal = Index;

		memcpy(&dispparams.rgvarg[0], &varTemp, sizeof(VARIANT));

		hr = SendObjectPropertyCommand(psbits, DISPID_PROPERTYLIST_PROPERTY, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			// notify client/server and world of property change
			hr = m_pParent->NotifyObjectPropertyChanged(this, plPut, CComVariant(Index, VT_I4));
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

// REVIEW: FirstItem, NextItem don't check security
STDMETHODIMP CPropertyListObject::FirstItem(BSTR* pbstrName, VARIANT* pvar, VARIANT_BOOL* pboolVal)
{
	m_curpos = 0;

	return NextItem(pbstrName, pvar, pboolVal);
}

STDMETHODIMP CPropertyListObject::NextItem(BSTR* pbstrName, VARIANT* pvar, VARIANT_BOOL* pboolVal)
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

	// lists don't return name, name is optional retval
	if (pbstrName)
		*pbstrName = NULL;

	if (m_curpos <= m_varList.GetUpperBound())
	{
		*pboolVal = VARIANT_FALSE;

		CComSecurityPtr SecPtr = m_varList.GetAt(m_curpos++);
		ASSERT(SecPtr != NULL);

		if (SecPtr != NULL)
		{
			hr = SecPtr->get_Property(pvar);
		}
	}
	else
		*pboolVal = VARIANT_TRUE;

	return hr;
}

// REVIEW: CopyTo doesn't check security

// REVIEW: won't work correctly on server since Add gets remoted

STDMETHODIMP CPropertyListObject::CopyTo(IPropertyList* ppropertylist)
{
	HRESULT hr = S_OK;
	CComVariant var, dest;
	int iCount = m_varList.GetSize();
	int i = 0;

	// CopyTo not supported for orphaned lists, check for world pointer
	if (m_pWorld==NULL)
	{
		hr =  VWOBJECT_E_COPYINGNOTSUPPORTED;
		goto ERROR_ENCOUNTERED;
	}

	ppropertylist->put_Type(m_bstrType);

	for (i = 0; i < iCount && SUCCEEDED(hr); i++)
	{
		CComSecurityPtr SecPtr = m_varList.GetAt(i);
		ASSERT(SecPtr != NULL);

		if (SecPtr != NULL)
		{
			hr = SecPtr->get_Property(&var);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			hr = m_pWorld->CloneProperty(var, &dest);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			hr = ppropertylist->Add(dest);

			var.Clear();
			dest.Clear();
		}
	}

ERROR_ENCOUNTERED:
	return ReportObjectPropertyError(hr);
}

STDMETHODIMP CPropertyListObject::Marshall(DWORD dwOptions, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bAllDone = VARIANT_FALSE;
	CComVariant var;
	int i = 0;
	long propcnt = m_varList.GetSize();

	hr = CPropertyListObjectBase::Marshall(dwOptions, pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// marshall property count
	hr = pbuffer->put_Long(propcnt);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CPropertyListObject::Marshall: marshalling number of properties in list (%d)\n", propcnt);

	for (i = 0; i < propcnt; i++)
	{
		CComSecurityPtr SecPtr = m_varList.GetAt(i);
		ASSERT(SecPtr != NULL);

		if (SecPtr != NULL)
		{
			// force to BYREF
			hr = SecPtr->Marshall(MARSHALLREF(dwOptions, MARSHALL_BYREF), pbuffer);
			if (FAILED(hr))
				break;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CPropertyListObject::UnMarshall(IUnMarshallBuffer* pbuffer)
{
	long propcnt = 0;
	int i = 0;
	HRESULT hr = S_OK;
	CComPropertySecurity* pPropSec = NULL;

	// kill off previous data (done by IPropertySecurity::UnMarshall)
	EmptyList();

	hr = CPropertyListObjectBase::UnMarshall(pbuffer);
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::UnMarshall: failed to unmarshall property base\n");
		goto ERROR_ENCOUNTERED;
	}

	hr = pbuffer->get_Long(&propcnt);
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::UnMarshall: failed to unmarshall property count\n");
		goto ERROR_ENCOUNTERED;
	}

	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CPropertyListObject::UnMarshall: unmarshalling number of properties in list (%d)\n", propcnt);

	for (i = 0; i < propcnt; i++)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CPropertyListObject::UnMarshall: unmarshalling property\n");

		pPropSec = new CComPropertySecurity();
		if (pPropSec == NULL)
		{
			hr = E_OUTOFMEMORY;
			goto ERROR_ENCOUNTERED;
		}

		pPropSec->AddRef();

		if (pPropSec != NULL)
		{
			CComSecurityPtr SecPtr;

			pPropSec->put_World(m_pWorld);

			hr = pPropSec->UnMarshall(pbuffer);
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::UnMarshall: failed to unmarshall property security slot, index %d\n", i);
				break;
			}
		
			// assign it to smartptr
			SecPtr = pPropSec;
			ASSERT(SecPtr != NULL);

			// add property to property list
			// NOTE: same as core code from Add (but didn't want to remote it)
			m_varList.Add(SecPtr);

			SAFERELEASE(pPropSec);
		}
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pPropSec);

	return hr;
}

STDMETHODIMP CPropertyListObject::MarshallToScript(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut)
{
	HRESULT hr = S_OK;
	CString strContext = bstrContext;
	VARIANT_BOOL bDone;
	VARIANT_BOOL bPropAsVal;
	CComVariant var;
	BSTR bstrName = NULL;
	BSTR bstrOut = NULL;
	CString strVal;
	CString strOne;
	CString strOut;
	CComBSTR bstrContextSub;

	if (pbstrOut==NULL || bstrContext==NULL)
		return E_INVALIDARG;

	// initialize
	*pbstrOut = NULL;
	*pbAsVal = VARIANT_FALSE;

	strOut.Format("\tSet %s = World.Global.CreatePropertyList\n", (LPCTSTR) strContext);

	// see if our type anything but "List"
	if (CString(m_bstrType)!="Property List")
	{
		// not standard, generate type set
		strOne.Format("\t%s.Type = \"%s\"\n", (LPCTSTR) strContext, (LPCTSTR) CString(m_bstrType));
		strOut += strOne;
	}

	// calculate new context
	bstrContextSub = bstrContext;
	bstrContextSub.Append("_t");

	// write out values
	hr = FirstItem(&bstrName, &var, &bDone);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	while (bDone!=VARIANT_TRUE)
	{
		// marshall out the property
		hr = m_pWorld->MarshallPropertyToScript(var, dwOptions, bstrContextSub, &bPropAsVal, &bstrOut);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (bstrOut==NULL)
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}

		// handle value and context cases
		if (bPropAsVal == VARIANT_TRUE)
		{
			strVal = bstrOut;
		}
		else
		{
			strOut += CString(bstrOut);
			strVal = bstrContextSub;

		}

		// add the value to the list
		strOne.Format( "\t%s.Add %s\n",
						(LPCTSTR) CString(bstrContext),
						(LPCTSTR) strVal );

		strOut += strOne;

		// free stuff
		SAFEFREESTRING(bstrName);
		SAFEFREESTRING(bstrOut);
		var.Clear();

		// go on to the next one
		hr = NextItem( &bstrName, &var, &bDone);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	// allocate
	*pbstrOut = strOut.AllocSysString();

ERROR_ENCOUNTERED:

	SAFEFREESTRING(bstrName);
	SAFEFREESTRING(bstrOut);

	return hr;
}

STDMETHODIMP CPropertyListObject::get_PropertySecurity(long Index, IPropertySecurity** ppSecurity)
{
	ASSERT(ppSecurity);
	if (ppSecurity == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::get_PropertySecurity: outgoing pointer (IPropertySecurity**) is NULL\n");
		return E_POINTER;
	}

	*ppSecurity = NULL;

	if (Index < 0 || Index > m_varList.GetUpperBound())
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CPropertyListObject::get_PropertySecurity: property not found (%d)\n", Index);
		return VWOBJECT_E_INVALIDLISTINDEX;
	}

	if (m_pParent)
	{
		if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_READ)))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::get_PropertySecurity: security violation (PS_READ), index %d\n", Index);
			return VWOBJECT_E_PERMISSION_DENIED;
		}
	}

	CComSecurityPtr SecPtr;

	SecPtr = m_varList.GetAt(Index);
	ASSERT(SecPtr != NULL);

	*ppSecurity = SecPtr;
	SAFEADDREF(*ppSecurity);

	return S_OK;
}

STDMETHODIMP CPropertyListObject::get_PropertyAtIndex(long index, VARIANT *pvar)
{
	return get_Property(index, pvar);
}

STDMETHODIMP CPropertyListObject::get__NewEnum(IUnknown** ppunk)
{
	CEnumCollection *pec;
	HRESULT hr;

	ASSERT(ppunk);
	if (ppunk == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertyListObject::get__NewEnum: outgoing pointer (IUnknown**) is NULL\n");
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

void CPropertyListObject::EmptyList()
{
	// We must not call RemoveAll without first clearing the properties
	// becuase IObjectProperties may contain a circular reference!

	int iCount = m_varList.GetSize();

	for (int i = 0; i < iCount; i++)
	{
		CComSecurityPtr SecPtr = m_varList.GetAt(i);
		ASSERT(SecPtr != NULL);

		if (SecPtr != NULL)
			SecPtr->Cleanup();
	}

	m_varList.RemoveAll();
}

