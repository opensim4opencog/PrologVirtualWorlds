// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include <vwobject.h>
#include <propbase.h>
#include "profhelp.h"
#include "syshelp.h"
			  
//******************************************************
// Helpers
// a couple of helpers for getting things in and out of
// the profile...

//******************************************************

long _FindThingInPropList(IPropertyList *pList, IThing *pThing)
{
	long lIndex = -1;

	/* hr = */ pList->Find(CComVariant(pThing), &lIndex);

	return lIndex;
}

//******************************************************
// _AddToProfileKeyList
// adds the thing to the list key, adding the key if it
// isn't there already...

const VARIANT kVarNone = {VT_ERROR, 0, 0, 0, DISP_E_MEMBERNOTFOUND};

HRESULT	_AddToProfileKeyList(IWorld *pWorld, IAvatarProfile *pProfile, BSTR bstrKey, IThing *pThing, IThing* pOwner)
{
	HRESULT hr = S_OK;
	CComVariant varKeyVal;
	VARIANT_BOOL bValid = VARIANT_FALSE;

	// find out if the key valid
	hr = pProfile->IsValid(bstrKey, &bValid);
	if (FAILED(hr))
		goto CLEAN_UP;

	// if key exists
	if (bValid == VARIANT_TRUE)
	{
		long lIndex = -1;

		hr = pProfile->get_KeyValue(bstrKey, &varKeyVal);
		if (SUCCEEDED(hr) && varKeyVal.vt == VT_DISPATCH)
		{
			IPropertyList* pList = (IPropertyList *)varKeyVal.pdispVal;

			/* hr */ pList->Find(CComVariant(pThing), &lIndex);

			// if user not on list, add them
			if (lIndex == -1)
				hr = pList->AddThing(pThing);

			hr = pProfile->put_KeyValue(bstrKey, CComVariant(pList));
		}
		else
			hr = VWOBJECT_E_INVALIDPROFILE;
	}
	// if key doesn't exist
	else
	{
		IPropertyList* pList = NULL;

		// create list
		hr = CreatePropertyList(pWorld, &pList);
		if (FAILED(hr))
			goto CLEAN_UP_1;
		
		// add user to list
		hr = pList->AddThing(pThing);
		if (FAILED(hr))
			goto CLEAN_UP_1;

		hr = pProfile->AddKey(bstrKey, CComVariant(pList), kVarNone, CComVariant(kKeyPublic), CComVariant(pOwner));

CLEAN_UP_1:
		SAFERELEASE(pList);
	}

CLEAN_UP:
	return hr;
}

//******************************************************
// _RemoveFromProfileKeyList
// Removes an object from an existing key list

HRESULT	_RemoveFromProfileKeyList(IAvatarProfile *pProfile, BSTR bstrKey, IThing *pThing)
{
	HRESULT hr = S_OK;
	CComVariant varKeyVal;
	VARIANT_BOOL bValid = VARIANT_FALSE;

	// find out if the key valid
	hr = pProfile->IsValid(bstrKey, &bValid);
	if (FAILED(hr))
		goto CLEAN_UP;

	// if key exists
	if (bValid == VARIANT_TRUE)
	{
		long lIndex = -1;

		hr = pProfile->get_KeyValue(bstrKey, &varKeyVal);
		if (SUCCEEDED(hr) && varKeyVal.vt == VT_DISPATCH)
		{
			IPropertyList* pList = (IPropertyList *)varKeyVal.pdispVal;

			/* hr */ pList->Find(CComVariant(pThing), &lIndex);

			// if user on list, remove them
			if (lIndex != -1)
				hr = pList->RemoveThing(pThing);

			hr = pProfile->put_KeyValue(bstrKey, CComVariant(pList));
		}
		else
			hr = VWOBJECT_E_INVALIDPROFILE;
	}

CLEAN_UP:
	return hr;
}


//******************************************************
// _IsObjInProfileKeyList
// Returns TRUE if the object is in the key list

BOOL _IsObjInProfileKeyList(IAvatarProfile *pProfile, BSTR bstrKey, IThing *pThing)
{
	HRESULT hr = S_OK;
	CComVariant varKeyVal;
	BOOL bRetVal = FALSE;
	VARIANT_BOOL bValid = VARIANT_FALSE;

	// find out if the key valid
	hr = pProfile->IsValid(bstrKey, &bValid);
	if (FAILED(hr))
		goto CLEAN_UP;

	// if key exists
	if (bValid == VARIANT_TRUE)
	{
		long lIndex = -1;

		hr = pProfile->get_KeyValue(bstrKey, &varKeyVal);
		if (SUCCEEDED(hr) && varKeyVal.vt == VT_DISPATCH)
		{
			IPropertyList* pList = (IPropertyList *)varKeyVal.pdispVal;

			/* hr */ pList->Find(CComVariant(pThing), &lIndex);

			// does thing exist in list
			if (lIndex != -1)
				bRetVal = TRUE;
		}
	}

CLEAN_UP:
	return bRetVal;
}

//******************************************************
// _AddStringKey
// Add a string-based key

HRESULT _AddStringKey(IAvatarProfile *pProfile, BSTR bstrKey, BSTR bstrKeyValue, IThing *pOwner)
{
	HRESULT hr = S_OK;

	hr = pProfile->AddKey(bstrKey, CComVariant(bstrKeyValue), kVarNone, CComVariant(kKeyPublic), CComVariant(pOwner));

	return hr;
}

//******************************************************
// _AddListKey
// Add a list-based key

HRESULT _AddListKey(IWorld *pWorld, IAvatarProfile *pProfile, BSTR bstrKey, IThing *pOwner)
{
	HRESULT hr = S_OK;
	IPropertyList *pList = NULL;

	hr = CreatePropertyList(pWorld, &pList);
	if (FAILED(hr))
		goto CLEAN_UP;

	hr = pProfile->AddKey(bstrKey, CComVariant(pList), kVarNone, CComVariant(kKeyPublic), CComVariant(pOwner));

CLEAN_UP:
	SAFERELEASE(pList);

	return hr;
}

//******************************************************
// _AddMapKey
// Add a map-based key

HRESULT _AddMapKey(IWorld *pWorld, IAvatarProfile *pProfile, BSTR bstrKey, IThing *pOwner)
{
	HRESULT hr = S_OK;
	IPropertyMap *pMap = NULL;

	hr = CreatePropertyMap(pWorld, &pMap);
	if (FAILED(hr))
		goto CLEAN_UP;

	hr = pProfile->AddKey(bstrKey, CComVariant(pMap), kVarNone, CComVariant(kKeyPublic), CComVariant(pOwner));

CLEAN_UP:
	SAFERELEASE(pMap);

	return hr;
}

//******************************************************
// _AddBOOLKey
// Add a BOOLEAN-based key

HRESULT _AddBOOLKey(IAvatarProfile *pProfile, BSTR bstrKey, VARIANT_BOOL bValue, IThing *pOwner)
{
	HRESULT hr = S_OK;

	hr = pProfile->AddKey(bstrKey, CComVariant((bool)bValue), kVarNone, CComVariant(kKeyPublic), CComVariant(pOwner));

	return hr;
}

