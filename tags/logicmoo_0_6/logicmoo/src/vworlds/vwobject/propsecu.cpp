// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// PropSecu.cpp : Implementation of CPropSecApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include "propsecu.h"
#include "marshbuf.h"
#include "worldobj.h"
#include "thingobj.h"

typedef CVWComPtr<IObjectProperty, NULL, &IID_IObjectProperty> CObjectPropertyPtr;
typedef CVWComPtr<IThing, NULL, &IID_IThing> CThingPtr;

#define SUPPORT_SECURITY_OVERRIDE

/////////////////////////////////////////////////////////////////////////////
//
static int nPropertySecurityCallDepth = 0;

STDMETHODIMP CPropertySecurity::Cleanup()
{
	// NOTE: moved SAFERELEASE(m_pWorld) into destructor
	// this allows Cleanup to be called from UnMarshall

	SAFERELEASE(m_pExemplar);
	SAFERELEASE(m_pInstance);

	// clear out the property back ptrs
	ClearBackPointer(m_var);

	// clear out the property data
	m_var.Clear();

	// REVIEW: constructor didn't have EXEMPLAR_R set

	// Set to owner ReadWrite, no bits set
	m_ulSecurityFlags = (PS_READ | PS_WRITE); // | (PS_READ << 8);

	// by default security is not enabled...
	m_ulBits = 0;
	m_vtType = VT_VARIANT;
	m_IID = IID_NULL;
	m_bstrType.Empty();
			  
	return S_OK;
}

STDMETHODIMP CPropertySecurity::Initialize(IWorld* pWorld, IThing* pThing, IPropertySecurity* pSecurity)
{
	// these never fail
	put_World(pWorld);
	put_Instance(pThing);
	put_Exemplar(pThing);

	// copy permission first, because we'll overwrite the exemplar ptr later
	if (pSecurity)
		CopyPermission(pSecurity);

	return S_OK;
}

STDMETHODIMP CPropertySecurity::CopyPermission(IPropertySecurity* pSource)
{
	HRESULT hr = S_OK;
	long bits;

	ASSERT(pSource);
	if (pSource == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::CopyPermission: incoming argument (IPropertySecurity*) is NULL\n");
		return E_INVALIDARG;
	}

	// Need to copy everything used for permissions, everything except parent, prop name and m_var

	// save off exemplar, so reentrant CopyPermissions is OK
	IThing* pOldExemplar = m_pExemplar;

	m_pExemplar = ((CPropertySecurity*)pSource)->m_pExemplar;
	SAFEADDREF(m_pExemplar);

	SAFERELEASE(pOldExemplar);

	m_ulSecurityFlags = ((CPropertySecurity*)pSource)->m_ulSecurityFlags;

	bits = ((CPropertySecurity*)pSource)->m_ulBits;

	// Mask off the enabling bits
	bits &= ~PSMASK_ENABLED;

	// Keep the enabled state
	m_ulBits &= PSMASK_ENABLED;
	m_ulBits |= bits;

	m_vtType = ((CPropertySecurity*)pSource)->m_vtType;
	m_IID = ((CPropertySecurity*)pSource)->m_IID;

	m_bstrType = ((CPropertySecurity*)pSource)->m_bstrType;

#ifdef SUPPORT_CACHED_EVENTHANDLERS
	m_bstrOnChanged = ((CPropertySecurity*)pSource)->m_bstrOnChanged;
	m_bstrOnAccess = ((CPropertySecurity*)pSource)->m_bstrOnAccess;
#endif

	return hr;
}

STDMETHODIMP CPropertySecurity::get_World(IWorld** ppWorld) 
{ 
	ASSERT(ppWorld);
	if (ppWorld == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::get_World: outgoing pointer (IWorld*) is NULL\n");
		return E_POINTER;
	}

	*ppWorld = m_pWorld; 
	SAFEADDREF(*ppWorld); 

	return S_OK; 
}; 
 
STDMETHODIMP CPropertySecurity::put_World(IWorld* pworld)
{
	SAFERELEASE(m_pWorld);
	m_pWorld = pworld;
	SAFEADDREF(m_pWorld);

	return S_OK;
}

STDMETHODIMP CPropertySecurity::get_Property(VARIANT* pvar) 
{ 
	ASSERT(pvar);
	if (pvar == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::get_Property: outgoing pointer (VARIANT*) is NULL\n");
		return E_POINTER;
	}

	// REVIEW: removed since caller will always call this already
//	::VariantClear(pvar);

#if 0
	// REVIEW: assume that PS_READ was already checked by caller (propmap or proplist)
	if (!SAFE_CHECKSECURITY(m_pWorld, this, PS_READ))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::get_Property: security violation (PS_READ)\n");
		return VWOBJECT_E_PERMISSION_DENIED;
	}
#endif

 	return ::VariantCopyInd(pvar, &m_var);
}; 
 
STDMETHODIMP CPropertySecurity::put_Property(VARIANT var)
{
	HRESULT hr = S_OK;
	BOOL bCopy = TRUE;

#if 0
	// REVIEW: assume that PS_WRITE was already checked by caller (propmap or proplist)
	if (!SAFE_CHECKSECURITY(m_pWorld, this, PS_WRITE))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::put_Property: security violation (PS_WRITE)\n");
		return VWOBJECT_E_PERMISSION_DENIED;
	}
#endif

	if ((m_vtType != VT_VARIANT) &&
		(m_vtType != VT_EMPTY))
	{
		if (m_vtType != var.vt)
		{
			switch (m_vtType)
			{
			case VT_I2:
			case VT_I4:
			case VT_INT:
				switch (var.vt)
				{
				case VT_I2:
				case VT_I4:
				case VT_INT:
				case VT_UI1:
				case VT_UI2:
				case VT_UI4:
				case VT_UINT:
					// change integer or unsigned int to integer
					::VariantChangeType(&var, &var, 0, m_vtType);
					break;
				}
				break;
			case VT_R4:
			case VT_R8:
				switch (var.vt)
				{
				case VT_R4:
				case VT_R8:
				case VT_I2:
				case VT_I4:
				case VT_INT:
				case VT_UI1:
				case VT_UI2:
				case VT_UI4:
				case VT_UINT:
					// change float, double, integer or unsigned int to float/double
					::VariantChangeType(&var, &var, 0, m_vtType);
					break;
				}
				break;
			case VT_UI1:
			case VT_UI2:
			case VT_UI4:
			case VT_UINT:
				switch (var.vt)
				{
				case VT_I2:
				case VT_I4:
				case VT_INT:
				case VT_UI1:
				case VT_UI2:
				case VT_UI4:
				case VT_UINT:
					// change integer or unsigned int to unsigned int
					::VariantChangeType(&var, &var, 0, m_vtType);
					break;
				}
				break;
			}
		}

		// type might not have been coerced
		if (m_vtType != var.vt)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::put_Property: PERMISSION DENIED - type mismatch, m_vtType %d, var.vt %d\n", m_vtType, var.vt);
			return VWOBJECT_E_TYPE_MISMATCH;
		}
	}

	// Test for IID
	if (var.vt == VT_DISPATCH || var.vt == VT_UNKNOWN)
	{
		if ((m_vtType == VT_DISPATCH || m_vtType == VT_UNKNOWN) && !IsEqualIID(m_IID, IID_NULL))
		{
			// no addref here
			IUnknown* pVarUnk = (m_vtType == VT_UNKNOWN) ? var.punkVal : (IUnknown*)var.pdispVal;

			// only test for non-null values
			if (pVarUnk)
			{
				IUnknown* pUnk = NULL;

				SAFE_QIASSIGN(pUnk, pVarUnk, m_IID);

				if (pUnk == NULL)
				{
					VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::put_Property: PERMISSION DENIED - IID mismatch, m_vtType %d, var.vt %d, type (%s)\n", m_vtType, var.vt, CString(m_bstrType));
					return VWOBJECT_E_TYPE_MISMATCH;
				}

				SAFERELEASE(pUnk);
			}
		}
	}

	if (var.vt == VT_DISPATCH)
	{
		// if incoming NULL ptr, just do VariantCopy and return
		if (var.pdispVal == NULL)
			goto CONTINUE_COPY;

		// var has to be an IObjectProperty but not a thing
		CObjectPropertyPtr newPropPtr(var);

		// REVIEW: why were we testing for VT_UNKNOWN?  we never use that type

		// test to see if the incoming is of the right type
		if ((m_vtType == VT_DISPATCH /* || m_vtType == VT_UNKNOWN */) && (m_bstrType.Length() > 0))
		{
			VARIANT_BOOL bVal = VARIANT_FALSE;

			// REVIEW: if IsOfType returns E_STUBOBJECT, ignore it
			hr = newPropPtr->IsOfType(m_bstrType, &bVal);

			if (SUCCEEDED(hr))
			{
				// if not of same type
				if (bVal == VARIANT_FALSE)
				{
					VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::put_Property: PERMISSION DENIED - type mismatch, m_vtType %d, var.vt %d, type (%s)\n", m_vtType, var.vt, CString(m_bstrType));
					return VWOBJECT_E_TYPE_MISMATCH;
				}
			}
			else
			{
				// if IsOfType failed (and object wasn't a stub)
				if (hr != VWOBJECT_E_STUBOBJECT)
				{
					VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::put_Property: PERMISSION DENIED - type mismatch (IsOfType failed), m_vtType %d, var.vt %d, type (%s)\n", m_vtType, var.vt, CString(m_bstrType));
					return VWOBJECT_E_TYPE_MISMATCH;
				}
			}

			// clear out VWOBJECT_E_STUBOBJECT error
			hr = S_OK;
		}

		// first check to see if this property security slot has been added to a thing;
		// then check to see if incoming property has security backpointer already set
		// if so, error out since that shouldn't be allowed
		if (m_pExemplar != NULL && newPropPtr != NULL)
		{
			IPropertySecurity* pSecurity = NULL;

			hr = newPropPtr->get_Security(&pSecurity);
			if (FAILED(hr))
				return hr;

			if (pSecurity != NULL) 
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::put_Property: security backpointer already set\n");
				hr = VWOBJECT_E_INVALIDSECURITY;
			}

			SAFERELEASE(pSecurity);

			if (FAILED(hr))
				return hr;
		}

		CObjectPropertyPtr mPropPtr(m_var);

		if (newPropPtr != NULL && mPropPtr != NULL)  
		{		
			CLSID clsid1;
			CLSID clsid2;
			
			newPropPtr->get_CLSID(&clsid1);
			mPropPtr->get_CLSID(&clsid2);

			if (memcmp(&clsid1, &clsid2, sizeof(CLSID)) == 0)
			{
				CThingPtr newThing = var;
				CThingPtr mThing = m_var;		

				if ((newThing == NULL) && (mThing == NULL))
				{
					typedef CComObject<CMarshallBufferObject> CComMarshallBuffer;
					typedef CComObject<CUnMarshallBufferObject> CComUnMarshallBuffer;

					CComMarshallBuffer *pMarshall = NULL;
					CComUnMarshallBuffer *pUnMarshall = NULL;
					DWORD lLen = 0;
					unsigned char* pbyte = NULL;

					IThing* pParent = NULL;
					CComBSTR bstrPropertyName;

					/* hr = */ mPropPtr->get_Parent(&pParent);
					/* hr = */ mPropPtr->get_PropertyName(&bstrPropertyName.m_str);

					// create marshall buffer
					pMarshall = new CComMarshallBuffer();
					if (pMarshall == NULL)
					{
						hr = E_OUTOFMEMORY;
						goto ERROR_ENCOUNTERED_1;
					}

					pMarshall->AddRef();

					// create unmarshall buffer
					pUnMarshall = new CComUnMarshallBuffer();
					if (pUnMarshall == NULL)
					{
						hr = E_OUTOFMEMORY;
						goto ERROR_ENCOUNTERED_1;
					}

					pUnMarshall->AddRef();

					if (FAILED(newPropPtr->Marshall(MARSHALL_BYID | MARSHALL_BYREF | MARSHALL_TOMEMORY, pMarshall)))
						goto ERROR_ENCOUNTERED_1;
			
					// convert pbuffer to pUbuffer
					hr = pMarshall->Detach(&pbyte, &lLen);
					if (FAILED(hr))
						goto ERROR_ENCOUNTERED_1;
			
					hr = pUnMarshall->Attach(pbyte, lLen);
					if (FAILED(hr))
						goto ERROR_ENCOUNTERED_1;
			
					// Unmarshall back into the destination
					if (FAILED(mPropPtr->UnMarshall(pUnMarshall)))
						goto ERROR_ENCOUNTERED_1;

					// REVIEW: need to reset back pointers since they're wiped clean by the 
					// UnMarshall step

					/* hr = */ mPropPtr->put_Parent(pParent);
					/* hr = */ mPropPtr->put_PropertyName(bstrPropertyName);

					// we succeeded, don't do the default copy operation!!!!
					bCopy = FALSE;

ERROR_ENCOUNTERED_1:
					SAFERELEASE(pParent);

					// NOTE: need to free unmarshall buffer before marshall buffer because it 
					// has a ptr to the detached marshall buffer and will try and test it 
					// for a valid address
					SAFERELEASE(pUnMarshall);	
					SAFERELEASE(pMarshall);

					if (FAILED(hr))
						goto ERROR_ENCOUNTERED;
				}
			}
		}
		
		// release refs on these ptrs, otherwise it screws up ClearBackPointer logic
		// for calling terminate
		newPropPtr.Release();
		mPropPtr.Release();
	}

CONTINUE_COPY:
	if (bCopy)
	{
		if (m_var.vt == VT_DISPATCH && m_var.pdispVal != NULL)
			ClearBackPointer(m_var);

		::VariantCopyInd(&m_var, &var);

		if (m_var.vt == VT_DISPATCH && m_var.pdispVal != NULL)
			SetBackPointer(m_var);
	}

ERROR_ENCOUNTERED:
	return hr;
}

inline HRESULT CPropertySecurity::SetBackPointer(VARIANT var)
{
	HRESULT hr = S_OK;
	CObjectPropertyPtr PropPtr = var;
	CThingPtr ThingPtr = var;

	if (PropPtr != NULL && ThingPtr == NULL)
	{
		SetSecurity(PropPtr);
	}

	return hr;
}

inline HRESULT CPropertySecurity::ClearBackPointer(VARIANT var)
{
	HRESULT hr = S_OK;
	CObjectPropertyPtr PropPtr = var;
	CThingPtr ThingPtr = var;
	
	if (PropPtr != NULL && ThingPtr == NULL)
	{
		ClearSecurity(PropPtr);
	}

	return hr;
}

inline HRESULT CPropertySecurity::SetSecurity(IObjectProperty* pprop)
{
	HRESULT hr = S_OK;
	IPropertySecurity * pSecurity = NULL;

	if (pprop == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::SetSecurity: invalid incoming parameter (IObjectProperty*)\n");
		return E_INVALIDARG;
	}

	pprop->get_Security(&pSecurity);

	// don't overwrite security ptr if already set; mark as slave
	if (pSecurity == NULL)
		hr = pprop->put_Security(this);
	else
		m_bSlave = VARIANT_TRUE;

	SAFERELEASE(pSecurity);

	return hr;
}

inline HRESULT CPropertySecurity::ClearSecurity(IObjectProperty* pprop)
{
	if (pprop == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::ClearSecurity: invalid incoming parameter (IObjectProperty*)\n");
		return E_INVALIDARG;
	}

	if (m_bSlave == VARIANT_FALSE)
		/* hr = */ pprop->put_Security(NULL);
	else
		m_bSlave = VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CPropertySecurity::get_Exemplar(IThing** ppThing) 
{ 
	ASSERT(ppThing);
	if (ppThing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::get_Exemplar: outgoing pointer (IThing**) is NULL\n");
		return E_POINTER;
	}

	*ppThing = m_pExemplar; 
	SAFEADDREF(*ppThing); 

	return S_OK; 
}

STDMETHODIMP CPropertySecurity::put_Exemplar(IThing* pthing) 
{ 
	SAFERELEASE(m_pExemplar); 
	m_pExemplar = pthing; 
	SAFEADDREF(m_pExemplar); 

	return S_OK; 
}

STDMETHODIMP CPropertySecurity::get_Instance(IThing** ppThing) 
{ 
	ASSERT(ppThing);
	if (ppThing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::get_Instance: outgoing pointer (IThing**) is NULL\n");
		return E_POINTER;
	}

	*ppThing = m_pInstance; 
	SAFEADDREF(*ppThing); 

	return S_OK; 
}

STDMETHODIMP CPropertySecurity::put_Instance(IThing* pthing) 
{ 
	SAFERELEASE(m_pInstance); 
	m_pInstance = pthing; 
	SAFEADDREF(m_pInstance); 

	return S_OK; 
}

STDMETHODIMP CPropertySecurity::put_Permissions(long ulFlags)
{
	HRESULT hr = S_OK;

	// Set up a fake permission
	unsigned long ulTempFlags = m_ulSecurityFlags;

	m_ulSecurityFlags = PS_WRITE;	
	
	// have to see if we have permission to change something
	if (!SAFE_CHECKSECURITY(m_pWorld, this, PS_WRITE))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::put_Permissions: security violation (PS_WRITE)\n");

		// oops restore the security attributte
		m_ulSecurityFlags = ulTempFlags;
		hr = VWOBJECT_E_PERMISSION_DENIED;
		goto errout;
	}

	// set the security flags, masking off invalid bits
	m_ulSecurityFlags = ulFlags & 0x000F0F0F;

errout:
	return S_OK;
}

STDMETHODIMP CPropertySecurity::get_Permissions(long* pulFlags)
{
	ASSERT(pulFlags);
	if (pulFlags == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::get_Permissions: outgoing pointer (long*) is NULL\n");
		return E_POINTER;
	}

	*pulFlags = m_ulSecurityFlags;

	return S_OK;
}

BOOL CPropertySecurity::ParseString(CString &str, unsigned long &ulFlags, BOOL &bAdd)
{
	BOOL bRet = FALSE;
	unsigned long ulGroup = 0;
	unsigned long ulPerms = 0;
	long maskflag = 0;		
	
	ulFlags = 0;

	// must be in the form of
	// [O|S|A] [+|-] [R|W|E|M]

	// simple parsing flags
	// o|s|a can be added if +|- not set (maskflag == 0)
	// ignore dups...
	// +|- can be set if they aren't already and [o|s|a] have be set (ulGroup ! = 0)
	// w|r|h|e can be added if +|- has been set, !maskflag
	for(int index = 0; index < str.GetLength(); index++)
	{
		switch (str[index]){
		case 'o':
		case 'O':
			if (maskflag) goto errout;				
			ulGroup |= PS_OWNER;
		break;

		case 's':
		case 'S':
			if (maskflag) goto errout;
			ulGroup |= PS_EXEMPLAR;
		break;

		case 'a':
		case 'A':
			if (maskflag) goto errout;
			ulGroup |= PS_ALL;
		break;

		case '+':
			if (maskflag || !ulGroup) goto errout;
			maskflag = 1;
		break;

		case '-':
			if (maskflag || !ulGroup) goto errout;				
			maskflag = 2;
		break;

		case 'r':
		case 'R':
			if (!maskflag) goto errout;
			ulPerms |= PS_READ;
		break;

		case 'w':
		case 'W':
			if (!maskflag) goto errout;
			ulPerms |= PS_WRITE;
		break;

		case 'e':
		case 'E':
			if (!maskflag) goto errout;
			ulPerms |= PS_EXECUTE;
		break;

		case 'm' :
		case 'M' :
			if (!maskflag) goto errout;
			ulPerms |= PS_ADD;
		break;

		default:
			goto errout;
		break;
		} 
	}
	// Did we specify permisions?
	if (!ulPerms) goto errout;

	// the bit layout is as follows
	// [RWHMXXXX][RWHMXXXX][RWHMXXXX]
	//   ALL SHARED OWNER
	if (ulGroup & PS_OWNER) ulFlags = ulPerms;
	if (ulGroup & PS_EXEMPLAR) ulFlags |= ulPerms << 8 ;
	if (ulGroup & PS_ALL) ulFlags |= ulPerms << 16;

	if (maskflag == 1)	
		bAdd = TRUE;
	else
		bAdd = FALSE;

	bRet = TRUE;

errout:
	return bRet;
}

STDMETHODIMP CPropertySecurity::ChangeMode(BSTR bstrMode)
{
	HRESULT hr = S_OK;	
	unsigned long ulFlags = 0;
	BOOL bAdd = FALSE;

	ASSERT(bstrMode);
	if (bstrMode == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::ChangeMode: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	if (!ParseString(CString(bstrMode), ulFlags, bAdd))
	{
		hr = VWOBJECT_E_SYNTAX_ERROR;
		goto errout;
	}

	if (bAdd) 
		hr = put_Permissions(m_ulSecurityFlags | ulFlags);
	else
		hr = put_Permissions(m_ulSecurityFlags & ~ulFlags);

errout:
	return hr;
}

STDMETHODIMP CPropertySecurity::QueryMode(BSTR bstrGroup, BSTR bstrPermission, VARIANT_BOOL* pbPermitted)
{
	HRESULT hr = S_OK;
	unsigned long ulFlags;
	BOOL bAdd;

	ASSERT(bstrGroup);
	if (bstrGroup == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::QueryMode: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	ASSERT(bstrPermission);
	if (bstrPermission == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::QueryMode: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	ASSERT(pbPermitted);
	if (pbPermitted == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::QueryMode: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return E_POINTER;
	}

	*pbPermitted = VARIANT_FALSE;

	CString strMode = CString(bstrGroup) + "+" + CString(bstrPermission);
	
	if (!ParseString(strMode, ulFlags, bAdd))
	{
		hr = VWOBJECT_E_SYNTAX_ERROR;
		goto errout;
	}

	*pbPermitted = (ulFlags & m_ulSecurityFlags) ? VARIANT_TRUE : VARIANT_FALSE;

errout:
	return hr;
}

STDMETHODIMP CPropertySecurity::put_BitState(long bits, VARIANT_BOOL bSet)
{
	if (!SAFE_CHECKSECURITY(m_pWorld, this, PS_WRITE))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::put_BitState: security violation (PS_WRITE)\n");
		return VWOBJECT_E_PERMISSION_DENIED;
	}

	if (bSet == VARIANT_TRUE)
	{
		m_ulBits &= PSMASK_ENABLED;
		bits &= ~PSMASK_ENABLED;
		m_ulBits |= bits;
	}
	else
	{
		m_ulBits &= (~bits) | PSMASK_ENABLED;
	}

	return S_OK;
}

STDMETHODIMP CPropertySecurity::get_BitState(long bits, VARIANT_BOOL* pbSet)
{
	ASSERT(pbSet);
	if (pbSet == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::get_BitState: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return E_POINTER;
	}

	// if the bits masked with m_ulBits remain the same then all are set.
	*pbSet = ((long)m_ulBits & bits) == bits ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CPropertySecurity::put_Bits(long bits)
{
	if (!SAFE_CHECKSECURITY(m_pWorld, this, PS_WRITE))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::put_Bits: security violation (PS_WRITE)\n");
		return VWOBJECT_E_PERMISSION_DENIED;
	}

	m_ulBits &= PSMASK_ENABLED;
	bits &= ~PSMASK_ENABLED;
	m_ulBits |= bits;
	
	return S_OK;
}

STDMETHODIMP CPropertySecurity::get_Bits(long *pbits)
{
	ASSERT(pbits);
	if (pbits == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::get_Bits: outgoing pointer (long*) is NULL\n");
		return E_POINTER;
	}

	*pbits = m_ulBits;

	return S_OK;
}

STDMETHODIMP CPropertySecurity::put_vtType(VARTYPE vartype)
{
	if (!SAFE_CHECKSECURITY(m_pWorld, this, PS_WRITE))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::put_vtType: security violation (PS_WRITE)\n");
		return VWOBJECT_E_PERMISSION_DENIED;
	}

	m_vtType = vartype;
	
	return S_OK;
}

STDMETHODIMP CPropertySecurity::get_vtType(VARTYPE *pvartype)
{
	ASSERT(pvartype);
	if (pvartype == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::get_vtType: outgoing pointer (VARTYPE*) is NULL\n");
		return E_POINTER;
	}

	*pvartype = m_vtType;

	return S_OK;
}	

STDMETHODIMP CPropertySecurity::get_PropertyIID(BSTR* pbstrIID)
{
	if (pbstrIID == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::get_PropertyIID: outgoing pointer (BSTR*) is NULL\n");
		return E_POINTER;
	}

	*pbstrIID = VWBSTRFromCLSID(m_IID);

	return S_OK;
}

STDMETHODIMP CPropertySecurity::put_PropertyIID(BSTR bstrIID)
{
	HRESULT hr = S_OK;

	if (bstrIID == NULL)
		hr = put_PropertyIIDExt(IID_NULL);
	else
	{
		IID iid;

		if (FAILED(::CLSIDFromString(bstrIID, &iid)))
			hr = put_PropertyIIDExt(IID_NULL);
		else
			hr = put_PropertyIIDExt(iid);
	}		

	return hr;
}

STDMETHODIMP CPropertySecurity::get_PropertyIIDExt(IID* piid)
{
	if (piid == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::get_PropertyIIDExt: outgoing pointer (IID*) is NULL\n");
		return E_POINTER;
	}

	memcpy(piid, &m_IID, sizeof(IID));

	return S_OK;
}

STDMETHODIMP CPropertySecurity::put_PropertyIIDExt(REFIID riid)
{
	memcpy(&m_IID, &riid, sizeof(IID));
	return S_OK;
}

STDMETHODIMP CPropertySecurity::get_PropertyType(BSTR* pbstr)
{
	if (pbstr == NULL)
			return E_POINTER;

	*pbstr = m_bstrType.Copy();

	return S_OK;
}

STDMETHODIMP CPropertySecurity::put_PropertyType(BSTR Type)
{
	m_bstrType = Type;
	return S_OK;
}

// This has been re-ordered for clarity and error message information
// For a call to succeed we need to determine access rights. 
// if the access is OWNER only then we need to determine if owner of the call stack also own
// There are 2 cases:
// 1) direct access (via C++, or raw script)
// 2) indirect calls via IThing.InvokeMethod
// 
// In general we need to prove that the owner of the method is also the owner of the security attribute.
// In the case of an IThing.InvokeMethod call.
//		caller == property.this
//		property.owner == Verb.Exemplar.Owner	
// 
// Direct access must show that the User == Security.Exemplar.Owner


// take 2
// the rules are now:
/*
	a) DispatchThis = FOO
	b) Search FOO and up the exemplar chain until you find Property
	c) If Property is UNRESTRICTED then allow access
	d) If Property is INACCESSIBLE then deny access
	e) If World.VerbThis.Owner == DispatchThis.Owner then allow access
	f) If World.VerbThis == Property.Exemplar AND World.This == DispatchThis
		then allow access
	g) If World.This == DispatchThis then allow execute access
	h) otherwise deny access
*/
/* Modification #3

I think we need to add a few more permissions to make a few things more restrictive.  
The case I'm thinking about is for RESTRICTED properties such as "Location" 
and "Owner".  Currently restricted just means that there are no permissions, 
and a  method would have to hack security to get around it.  
I'd like to propose having a more restrictive permission  
than OWNER, EXEMPLAR so that only the exemplar that defined the property 
can access it, regardless of owner.


	Method.Instance is the instance from which the method is invoked
	Property.Instance is the instance from which the property is instantiated
	Method & Property.Exemplar is the Exemplar where the method or property was defined.
	The Owner is the World.this when the property or method was created, or the user.  The Owner never changes.

	Each property/method has access bits Read/Write/Execute of course only methods can be executed
	Each property/method has access permissions OWNER/ALL, we are considering a more restrictive case of EXEMPLAR
	Not all bit combinations are permitted, for example ALL & WRITE.

	a) If Property is UNRESTRICTED then allow access (the appropriate bits set for ALL access)
	b) If Property is INACCESSIBLE then deny access (no bits set)
	c - old) If Method.Exemplar.Owner == Property.Instance.Owner then allow access
	c - modified) If Method.Exemplar.Owner == Property.Instance.Owner AND permissions allow OWNER then allow access
	d) If Method.Exemplar == Property.Exemplar AND World.This == Property.Instance
		then allow access
	e - old) If World.This == Property.Instance AND EXECUTE then allow execute access
	e - modlifed) If World.This == Property.Instance AND EXECUTE AND OWNER then allow execute access
	f) otherwise deny access

	Rule C allows owners to access their own properties
	Rule D permits exemplar methods to access properties defined by that exemplar, on the instance
	Rule E permits exemplars to execute methods defined by other exemplars on the instance.

We have another problem with the property map implementing properties, 
which may be a common problem.  
The propertymap does not allow an Exemplar to add new properties, 
because it is being treated just like any other property on the instance level, 
and an exemplar can only muck with its own properties.  
I Think another bit with the same rules as EXECUTE will solve the problem. 
Call it  ADD which is similar to the NTFS permissions.  
This is probably a better general solution than special casing 
IThing::put_Property/put_method.
---- see security.doc for latest explanation ---
*/

STDMETHODIMP CPropertySecurity::CheckSecurity(unsigned long ulFlags, VARIANT_BOOL bQuiet, VARIANT_BOOL* bSuccess)
{
	ASSERT(bSuccess);
	if (bSuccess == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CPropertySecurity::CheckSecurity: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return E_POINTER;
	}

	// assume failure
	*bSuccess = VARIANT_FALSE;

	if (m_pWorld == NULL) 
	{
		// invalid world
		*bSuccess = VARIANT_TRUE; // VARIANT_FALSE;
		return S_OK;
	}

	if ((m_ulBits & PSMASK_ENABLED) == 0)
	{
		*bSuccess = VARIANT_TRUE;
		return S_OK;
	}

	if (m_pInstance == NULL)
	{
		// we've got problems, assume it is a unattached property anything goes
		*bSuccess = VARIANT_TRUE;
		return S_OK;
	}	
	
	//	a) If Property is UNRESTRICTED then allow access (the appropriate bits set for ALL access)
	if (((ulFlags << 16) & m_ulSecurityFlags) == (ulFlags << 16))
	{
		// Success!
		*bSuccess = VARIANT_TRUE;
		return S_OK;
	}

	// Do an early escape on recursive checks, these are due to 
	// get_Owner and stuff like that
	if (nPropertySecurityCallDepth > 0) 
	{
		*bSuccess = VARIANT_TRUE;
		return S_OK;
	}
	
 	nPropertySecurityCallDepth++;

	HRESULT hr = S_OK;	
	CWorldObject* pWorld = (CWorldObject*)m_pWorld;
	CThingObject* pInstance = (CThingObject*)m_pInstance;
	IPropertySecurity *pContext = NULL;
	IThing* pMethodInstance = NULL;
	IThing* pMethodExemplar = NULL;
	IThing* pMethodOwner  = NULL;
	IThing* pPropExemplarOwner = NULL;
	IThing* pPropInstanceOwner = NULL;
	IThing* pUser = NULL;
	IThing* pThis = NULL;
	IThing* pCaller = NULL;
#ifdef _DEBUG
	CString strWhy;
#endif

	// test to see that our instance is usable
	if (pInstance->m_bStub == VARIANT_TRUE) 
	{
#ifdef _DEBUG
		strWhy = "instance is a stub";
#endif
		goto errout;
	}

	// this means that if we can determine ownership, then the permissions are OK.
	// 2 cases, invoke method with a call stack
	// and raw script...
	// in the case of raw script (pContext == NULL) caller == owner only

	pContext = pWorld->m_pSecurityContext;
	SAFEADDREF(pContext);

	pThis = pWorld->m_pThingThis;
	SAFEADDREF(pThis);

	pUser = pWorld->m_pUser;
	SAFEADDREF(pUser);

	pCaller = (pWorld->m_pThingCaller) ? pWorld->m_pThingCaller : pWorld->m_pUser;
	SAFEADDREF(pCaller);

	// Do wizard check whether or not we have a context
	if (pCaller)
	{
		VARIANT_BOOL bIsWizard = VARIANT_FALSE;

		/* hr = */ pCaller->get_IsWizard(&bIsWizard);

		// if wizard, then all done
		if (bIsWizard == VARIANT_TRUE)
		{
			*bSuccess = VARIANT_TRUE;
#ifdef _DEBUG
			strWhy = "user is Wizard";
#endif
			goto errout;
		}
	}

	// if we have a source context, see if security is disabled
	// for it
	if (pContext)
	{
		CPropertySecurity* pSecurityContext = (CPropertySecurity*)pContext;

		// if "server" bit set, all done
		if (pSecurityContext->m_ulBits & PSBIT_SYSTEMOVERRIDE)
		{
			*bSuccess = VARIANT_TRUE;
#ifdef _DEBUG
			strWhy = "source context is PSBIT_SYSTEMOVERRIDE";
#endif
			goto errout;
		}

		pMethodInstance = pSecurityContext->m_pInstance;
		SAFEADDREF(pMethodInstance);

		// get method exemplar from source context
		pMethodExemplar = pSecurityContext->m_pExemplar;
		SAFEADDREF(pMethodExemplar);

		if (pMethodExemplar)
		{
			pMethodOwner = ((CThingObject*)pMethodExemplar)->m_pOwner;
			SAFEADDREF(pMethodOwner);
		}
	}
	// no Context?  raw script...
	else if (pThis == NULL)
	{
		// REVIEW: do we really want to be checking
		// wizard policy down at this level?
		if (pUser)
		{
			// assume the pThis is the user
			pThis = pUser;
			SAFEADDREF(pThis);

			pMethodInstance = pThis;
			SAFEADDREF(pMethodInstance);

			pMethodExemplar = pThis;
			SAFEADDREF(pMethodExemplar);

			pMethodOwner = pThis;
			SAFEADDREF(pMethodOwner);
		}

		// This is the case when you're bootstrapping the world
		else
		{
			*bSuccess = VARIANT_TRUE;
#ifdef _DEBUG
			strWhy = "no call stack or user - assuming Wizard mode";
#endif
			goto errout;
		}
	}

	// NOTE: now we have pUser, pThis, pMethodOwner, pMethodExemplar (and pMethodInstance)

	// b) If Property is INACCESSIBLE then deny access (no bits set)
	if (((ulFlags | (ulFlags << 8)) & m_ulSecurityFlags) == 0)
	{
#ifdef _DEBUG
		strWhy = "property access flags for object owner and exemplar do not allow access";
#endif
		*bSuccess = VARIANT_FALSE;
		goto errout;
	}

	ASSERT(m_pInstance);

	pPropInstanceOwner = ((CThingObject*)m_pInstance)->m_pOwner;
	SAFEADDREF(pPropInstanceOwner);

	//	c-case1) If Method.Exemplar.Owner == Property.Instance.Owner AND permissions allow OWNER then allow access
	if ((pMethodOwner == pPropInstanceOwner) && (ulFlags & m_ulSecurityFlags) && (pPropInstanceOwner != NULL))
	{
		*bSuccess = VARIANT_TRUE;
#ifdef _DEBUG
		strWhy = "method.exemplar.owner == property.instance.owner";
#endif
		goto errout;
	}
	// c-case2)  If Method.Exemplar == Property.Instance AND permissions allow OWNER then allow access
	// This gets around the case of in c where pPropInstanceOwner == null. on yourself.
	if ((pMethodExemplar == pThis) && (m_pInstance == pThis) && (ulFlags & m_ulSecurityFlags)) 
	{
		*bSuccess = VARIANT_TRUE;
#ifdef _DEBUG
		strWhy = "method.exemplar == property.instance";
#endif
		goto errout;
	}

	// c-case3)  If Method.Exemplar == Property.Instance AND permissions allow OWNER then allow access
	// This gets around the case of in c where pPropInstanceOwner == null. On things you own
	if ((pMethodExemplar == pThis) && (pPropInstanceOwner == pThis) && (ulFlags & m_ulSecurityFlags)) 
	{
		*bSuccess = VARIANT_TRUE;
#ifdef _DEBUG
		strWhy = "method.exemplar == property.instance.owner && this == property.instance.owner";
#endif
		goto errout;
	}

	// d) If Method.Exemplar == Property.Exemplar AND World.This == Property.Instance
	// REVIEW: is there ever an instance that EXEMPLAR can't execute and owner can?
	if ((pMethodExemplar == m_pExemplar) && (pThis == m_pInstance))
	{
		*bSuccess = VARIANT_TRUE;
#ifdef _DEBUG
		strWhy = "method.exemplar == property.exemplar && this == property.instance";
#endif
		goto errout;
	}

	ASSERT(m_pExemplar);

	pPropExemplarOwner = ((CThingObject*)m_pExemplar)->m_pOwner;
	SAFEADDREF(pPropExemplarOwner);

	// d-part2) If Method.Exemplar.Owner == Property.Exemplar.Owner AND World.This == Property.Instance
	if ((pMethodOwner == pPropExemplarOwner) && (pThis == m_pInstance))
	{
		*bSuccess = VARIANT_TRUE;
#ifdef _DEBUG
		strWhy = "method.exemplar.owner == property.exemplar.owner && this == property.instance";
#endif
		goto errout;
	}

	// REVIEW: comments say EXECUTE AND OWNER, but code just says EXECUTE
	// alexco: i think this comment does point out a bug
	
	// e) If World.This == Property.Instance AND EXECUTE AND OWNER then allow execute access

	if ((pThis == m_pInstance) && (ulFlags & PS_EXECUTE & m_ulSecurityFlags))
	{
		*bSuccess = VARIANT_TRUE;
#ifdef _DEBUG
		strWhy = "this == property.instance && PS_EXECUTE";
#endif
		goto errout;
	}

	// REVIEW: comments say ADD AND OWNER, but code just says ADD
	// same as above, added an owner 
	// e) If World.This == Property.Instance AND ADD AND OWNER then allow execute access

	if ((pThis == m_pInstance) && (ulFlags & PS_ADD & m_ulSecurityFlags))
	{
		*bSuccess = VARIANT_TRUE;
#ifdef _DEBUG
		strWhy = "this == property.instance && PS_ADD";
#endif
		goto errout;
	}

	// Test for bootstrapping mistakes
	if (pPropInstanceOwner == NULL && pUser == NULL)
	{
		*bSuccess = VARIANT_TRUE;
#ifdef _DEBUG
		strWhy = "property.instance.owner == NULL && user == NULL: must be in Wizard mode";
#endif
		goto errout;
	}
	
#ifdef _DEBUG
	strWhy = "no permission - check ownership or access rights";
	if (pThis != m_pInstance)
		strWhy += " (this != instance)";
#endif
	
errout:
	VARIANT_BOOL bSecurityEnabled = VARIANT_TRUE;

	if (*bSuccess == VARIANT_FALSE)
	{
#ifdef SUPPORT_SECURITY_OVERRIDE
		m_pWorld->get_EnableSecurity(&bSecurityEnabled);
#endif

		if ((bQuiet == VARIANT_FALSE) && (PSBIT_ENABLED & m_ulBits))
		{
			CComBSTR bstrInstanceName("<unknown>");
			CComBSTR bstrThisName("<unknown>");
			CComBSTR bstrPropertyName("<unknown>");
			CComBSTR bstrMethodName("<unknown>");
			VARIANT_BOOL bIsExemplar = VARIANT_TRUE;
			long lInstanceID = NULL_OBJECT;

			CObjectPropertyPtr ObjPtr = m_var;

			if (ObjPtr != NULL)
			{
				CComBSTR bstrName;

				ObjPtr->get_PropertyName(&bstrName.m_str);

				if (bstrName.Length() != 0)
					bstrPropertyName = bstrName;
			}

			if (pThis)
			{
				if (((CThingObject*)pThis)->m_bstrCurrentMethod.Length() != 0)
					bstrMethodName = ((CThingObject*)pThis)->m_bstrCurrentMethod;

				if (SUCCEEDED(pThis->get_IsExemplar(&bIsExemplar)) && bIsExemplar == VARIANT_TRUE)
					pThis->get_Type(&bstrThisName.m_str);
				else
					pThis->get_Name(&bstrThisName.m_str);
			}

			if (SUCCEEDED(m_pInstance->get_IsExemplar(&bIsExemplar)) && bIsExemplar == VARIANT_TRUE)
				m_pInstance->get_Type(&bstrInstanceName.m_str);
			else
				m_pInstance->get_Name(&bstrInstanceName.m_str);

			m_pInstance->get_ID(&lInstanceID);

			CString strInfo;

#ifdef _DEBUG
			strInfo.Format("Security violation: %s", strWhy);
			if (m_pWorld)
				m_pWorld->Report(CComBSTR(strInfo), -1);

			strInfo.Format(	"Security violation: "
							"name (%s), "
							"method (%s)",
							CString(bstrInstanceName) + "." + CString(bstrPropertyName),
							CString(bstrThisName) + "." + CString(bstrMethodName));
			if (m_pWorld)
				m_pWorld->Report(CComBSTR(strInfo), -1);

			CString strFlags = "(";
			strFlags += ulFlags & PS_READ ? "r":"-";
			strFlags += ulFlags & PS_WRITE ? "w":"-";
			strFlags += ulFlags & PS_EXECUTE ? "x":"-";
			strFlags += ulFlags & PS_ADD ? "a":"-";
			strFlags += ")";

			CString strSecurityFlags = "(";
			strSecurityFlags += m_ulSecurityFlags & PS_READ ? "r":"-";
			strSecurityFlags += m_ulSecurityFlags & PS_WRITE ? "w":"-";
			strSecurityFlags += m_ulSecurityFlags & PS_EXECUTE ? "x":"-";
			strSecurityFlags += m_ulSecurityFlags & PS_ADD ? "a":"-";
			strSecurityFlags += "|";
			strSecurityFlags += m_ulSecurityFlags & (PS_READ << 8) ? "r":"-";
			strSecurityFlags += m_ulSecurityFlags & (PS_WRITE << 8) ? "w":"-";
			strSecurityFlags += m_ulSecurityFlags & (PS_EXECUTE << 8) ? "x":"-";
			strSecurityFlags += m_ulSecurityFlags & (PS_ADD << 8) ? "a":"-";
			strSecurityFlags += "|";
			strSecurityFlags += m_ulSecurityFlags & (PS_READ << 16) ? "r":"-";
			strSecurityFlags += m_ulSecurityFlags & (PS_WRITE << 16) ? "w":"-";
			strSecurityFlags += m_ulSecurityFlags & (PS_EXECUTE << 16) ? "x":"-";
			strSecurityFlags += m_ulSecurityFlags & (PS_ADD << 16) ? "a":"-";
			strSecurityFlags += ")";

			CComBSTR bstrMethodInstanceName("<unknown>");
			CComBSTR bstrMethodExemplarName("<unknown>");
			CComBSTR bstrMethodOwnerName("<unknown>");
			CComBSTR bstrExemplarName("<unknown>");
			CComBSTR bstrUserName("<unknown>");
			CComBSTR bstrOwnerName("<unknown>");
			long lMethodInstanceID = NULL_OBJECT;

			if (pPropInstanceOwner)
				pPropInstanceOwner->get_Name(&bstrOwnerName.m_str);

			if (m_pExemplar)
				m_pExemplar->get_Type(&bstrExemplarName.m_str);

			if (pMethodInstance)
			{
				pMethodInstance->get_ID(&lMethodInstanceID);

				if (SUCCEEDED(pMethodInstance->get_IsExemplar(&bIsExemplar)) && bIsExemplar == VARIANT_TRUE)
					pMethodInstance->get_Type(&bstrMethodInstanceName.m_str);
				else
					pMethodInstance->get_Name(&bstrMethodInstanceName.m_str);
			}

			if (pMethodExemplar)
				pMethodExemplar->get_Type(&bstrMethodExemplarName.m_str);

			if (pMethodOwner)
				pMethodOwner->get_Name(&bstrMethodOwnerName.m_str);

			if (pUser)
				pUser->get_Name(&bstrUserName.m_str);

			strInfo.Format(	"Security violation: "
							"check %s, "
							"flags %s, "
							"this (%s), "
							"user (%s)",
							strFlags,
							strSecurityFlags,
							CString(bstrThisName),
							CString(bstrUserName));
			if (m_pWorld)
				m_pWorld->Report(CComBSTR(strInfo), -1);

			strInfo.Format(	"Security violation: "
							"instance (%s:%d), "
							"instance owner (%s), "
							"exemplar (%s)",
							CString(bstrInstanceName),
							lInstanceID,
							CString(bstrOwnerName),
							CString(bstrExemplarName));
			if (m_pWorld)
				m_pWorld->Report(CComBSTR(strInfo), -1);

			strInfo.Format(	"Security violation: "
							"method instance (%s:%d), "
							"method exemplar (%s), "
							"method exemplar owner (%s)",
							CString(bstrMethodInstanceName),
							lMethodInstanceID,
							CString(bstrMethodExemplarName),
							CString(bstrMethodOwnerName));
			if (m_pWorld)
				m_pWorld->Report(CComBSTR(strInfo), -1);
#else
			strInfo.Format(	"Security violation: "
							"name (%s), "
							"method (%s)",
							CString(bstrInstanceName) + "." + CString(bstrPropertyName),
							CString(bstrThisName) + "." + CString(bstrMethodName));
			if (m_pWorld)
				m_pWorld->Report(CComBSTR(strInfo), -1);
#endif
		}
	}

	SAFERELEASE(pContext);
	SAFERELEASE(pMethodExemplar);
	SAFERELEASE(pMethodInstance);
	SAFERELEASE(pMethodOwner);
	SAFERELEASE(pPropExemplarOwner);
	SAFERELEASE(pPropInstanceOwner);
	SAFERELEASE(pUser);
	SAFERELEASE(pThis);
	SAFERELEASE(pCaller);

	nPropertySecurityCallDepth--;

#ifdef SUPPORT_SECURITY_OVERRIDE
	if (bSecurityEnabled == VARIANT_FALSE)
	{
		if (*bSuccess == VARIANT_FALSE)
		{
			*bSuccess = VARIANT_TRUE;

			if (m_pWorld)
				m_pWorld->Report(CComBSTR("**Security disabled: violation ignored"), -1);
		}
	}
#endif

	return hr;
}

STDMETHODIMP CPropertySecurity::Marshall(DWORD dwOptions, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	ASSERT(m_pWorld);

	// make sure we only marshall attached propsec slots
	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CPropertySecurity::Marshall: marshalling Security\n");

//	hr = pbuffer->put_Boolean(m_bAttached);
//	if (FAILED(hr))
//		return hr;	

	// if no exemplar backpointer, only marshall property itself
	if (m_pExemplar == NULL)
	{
		hr = pbuffer->put_Byte(0);
		if (FAILED(hr))
			return hr;	

		hr = m_pWorld->MarshallProperty(m_var, dwOptions, pbuffer);
		if (FAILED(hr))
			return hr;
	}
	else
	{
		hr = pbuffer->put_Byte(1);
		if (FAILED(hr))
			return hr;	

		hr = m_pWorld->MarshallObjectProperty(m_pExemplar, clsidThing, 
			MARSHALLREF(dwOptions, MARSHALL_BYREF), pbuffer);
		if (FAILED(hr))
			return hr;

		hr = m_pWorld->MarshallObjectProperty(m_pInstance, clsidThing, 
			MARSHALLREF(dwOptions, MARSHALL_BYREF), pbuffer);
		if (FAILED(hr))
			return hr;

		hr = m_pWorld->MarshallProperty(m_var, dwOptions, pbuffer);
		if (FAILED(hr))
			return hr;
		
		hr = pbuffer->put_ULong(m_ulSecurityFlags);
		if (FAILED(hr))
			return hr;

		hr = pbuffer->put_ULong(m_ulBits);
		if (FAILED(hr))
			return hr;

		hr = pbuffer->put_UShort(m_vtType);
		if (FAILED(hr))
			return hr;

		hr = pbuffer->put_ClassID(m_IID);
		if (FAILED(hr))
			return hr;

		hr = pbuffer->put_String(m_bstrType);
		if (FAILED(hr))
			return hr;

#ifdef SUPPORT_CACHED_EVENTHANDLERS
		hr = pbuffer->put_String(m_bstrOnChanged);
		if (FAILED(hr))
			return hr;

		hr = pbuffer->put_String(m_bstrOnAccess);
		if (FAILED(hr))
			return hr;
#endif
	}

	return hr;
}

STDMETHODIMP CPropertySecurity::UnMarshall(IUnMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;
	unsigned char bval = 0;

	ASSERT(m_pWorld);

	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CPropertySecurity::UnMarshall: unmarshalling Security\n");

	// remove old data
	Cleanup();

//	hr = pbuffer->get_Boolean(&m_bAttached);
//	if (FAILED(hr))
//		goto ERROR_ENCOUNTERED;	

	hr = pbuffer->get_Byte(&bval);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;	

	// if no exemplar (bval == 0), only unmarshall property itself
	if (bval == 0)
	{
		hr = m_pWorld->UnMarshallProperty(pbuffer, &m_var);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;	

		hr = SetBackPointer(m_var);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;	
	}
	else
	{
		hr = m_pWorld->UnMarshallObjectProperty(pbuffer, (IObjectProperty**)&m_pExemplar);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		hr = m_pWorld->UnMarshallObjectProperty(pbuffer, (IObjectProperty**)&m_pInstance);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;	

		hr = m_pWorld->UnMarshallProperty(pbuffer, &m_var);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;	

		//if (FAILED(hr = put_Property(var))) 
		//	goto ERROR_ENCOUNTERED;

		// Can't do a put_property because it may call check security on a
		// stub... set the security directly...
		hr = SetBackPointer(m_var);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;	

		hr = pbuffer->get_ULong(&m_ulSecurityFlags);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;	

		hr = pbuffer->get_ULong(&m_ulBits);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;	

		hr = pbuffer->get_UShort(&m_vtType);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;	

		hr = pbuffer->get_ClassID(&m_IID);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;	

		m_bstrType.Empty();
		hr = pbuffer->get_String(&m_bstrType.m_str);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;	

#ifdef SUPPORT_CACHED_EVENTHANDLERS
		m_bstrOnChanged.Empty();
		hr = pbuffer->get_String(&m_bstrOnChanged.m_str);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;	

		m_bstrOnAccess.Empty();
		hr = pbuffer->get_String(&m_bstrOnAccess.m_str);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;	
#endif
	}

ERROR_ENCOUNTERED:
	return hr;
}
