// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// AvatarProfile.cpp : Implementation of CAvatarProfileObject
#include "stdafx.h"
#include <vwobject.h>
#include <avprof.h>
#include "profob.h"
#include "profhelp.h"
#include "syshelp.h"
#include "global.h"

#if 0
// force debugging of AvatarProfile creation/destruction
#undef TRACE_ALLOCATION

#define TRACE_ALLOCATION VWT_NONE
//#define TRACE_ALLOCATION VWT_METHOD
#endif

IMPLEMENT_OBJECTPROPERTY(CAvatarProfileObject)
IMPLEMENT_OBJECTPROPERTY(CAvatarProfileDataObject)

typedef CComObject<CAvatarProfileDataObject> CComAvatarProfileData;

const VARIANT		kVarNone = {VT_ERROR, 0, 0, 0, DISP_E_MEMBERNOTFOUND};

/////////////////////////////////////////////////////////////////////////////
// CAvatarProfileObject

CAvatarProfileObject::CAvatarProfileObject()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CAvatarProfileObject::CAvatarProfileObject, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif

	m_pBannedObjects = NULL;
	m_pKeys = NULL;
	m_pAvatar = NULL;
}

CAvatarProfileObject::~CAvatarProfileObject()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CAvatarProfileObject::~CAvatarProfileObject, this 0x%0x\n", this);
#endif

	Terminate();
}

STDMETHODIMP CAvatarProfileObject::Terminate(void)
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CAvatarProfileObject::Terminate, this 0x%0x\n", this);
#endif

	CAvatarProfileObjectBase::Terminate();

	SAFERELEASE(m_pBannedObjects);
	SAFERELEASE(m_pKeys);
	SAFERELEASE(m_pAvatar);

	return S_OK;
}

STDMETHODIMP CAvatarProfileObject::put_World(IWorld* pworld)
{
	HRESULT hr = S_OK;

	CAvatarProfileObjectBase::put_World(pworld);

	ASSERT(m_pWorld);
	if (m_pWorld == NULL)
		return VWOBJECT_E_INVALIDWORLD;

	hr = Reset();

	return hr;
}

STDMETHODIMP CAvatarProfileObject::put_Avatar(IThing *pThing) 
{
	SAFERELEASE(m_pAvatar);
	m_pAvatar = pThing;
	SAFEADDREF(m_pAvatar);

	return S_OK;
}

STDMETHODIMP CAvatarProfileObject::get_Avatar(IThing **ppThing) 
{
	if (ppThing == NULL)
		return E_POINTER;

	*ppThing = m_pAvatar;
	SAFEADDREF(*ppThing);

	return S_OK;
}

//******************************************************
// IsKeyReadable
// Given an object and the actual profile data, returns
// true if the key can be read by the object.
// Here are the rules, in order
// 1)  A script in SEngine can read anything
// 2)  The user can read anything
// 3)  If the key is public, anyone can read it
// 4)  If the user is the key's owner, they can read it
// 5)  If the key is private, no one can read it
// 6)  If the key is for friends, and the user is in the friend's
//     list, then they can read.

BOOL CAvatarProfileObject::IsKeyReadable(IAvatarProfileData *pProfileData)
{
	static CComBSTR	kbstrFriends("Friends");
	HRESULT hr = S_OK;
	BOOL bReadable = FALSE;
	short sAccess = 0;
	IThing* pUser = NULL;
	IThing* pKeyOwner = NULL;
#ifdef _DEBUG
	CComBSTR bstrUser("n/a");
#endif

	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CAvatarProfileObject::IsKeyReadable\n");

	if (pProfileData == NULL)
	{
		VWTRACE(m_pWorld, "VWPROPERTY", VWT_ERROR, "CAvatarProfileObject::IsKeyReadable: incoming argument (IAvatarProfileData*) is NULL\n");
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	hr = m_pWorld->get_User(&pUser);
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWPROPERTY", VWT_ERROR, "CAvatarProfileObject::IsKeyReadable: failed to get user from world\n");
		hr = E_FAIL; // should never happen, but we eat the HRESULT anyway
		goto ERROR_ENCOUNTERED;
	}
	
#ifdef _DEBUG
	if (pUser)
		/* hr = */ pUser->get_Name(&bstrUser.m_str);
#endif

	/* hr = */ pProfileData->get_Access(&sAccess);
	/* hr = */ pProfileData->get_Owner(&pKeyOwner);

	if (pUser == NULL)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWPROPERTY", VWT_IMPORTANT, "CAvatarProfileObject::IsKeyReadable: no user logged into world, readable\n");
#endif
		bReadable = TRUE;
		goto ERROR_ENCOUNTERED;
	}
	else if (pUser == m_pAvatar)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWPROPERTY", VWT_IMPORTANT, "CAvatarProfileObject::IsKeyReadable: user (%s) is avatar, readable\n", CString(bstrUser));
#endif
		bReadable = TRUE;
		goto ERROR_ENCOUNTERED;
	}
	else if (pUser == pKeyOwner)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWPROPERTY", VWT_IMPORTANT, "CAvatarProfileObject::IsKeyReadable: user (%s) is owner, readable\n", CString(bstrUser));
#endif
		bReadable = TRUE;
		goto ERROR_ENCOUNTERED;
	}

	if (sAccess == kKeyPublic)
	{
		VWTRACE(m_pWorld, "VWPROPERTY", VWT_IMPORTANT, "CAvatarProfileObject::IsKeyReadable: public key, readable\n");
		bReadable = TRUE;
		goto ERROR_ENCOUNTERED;
	}
	else if (sAccess == kKeyPrivate)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWPROPERTY", VWT_ERROR, "CAvatarProfileObject::IsKeyReadable: user (%s) not owner or avatar, private key, not readable\n", CString(bstrUser));
#endif
		bReadable = FALSE;
		goto ERROR_ENCOUNTERED;
	}
	else if (sAccess == kKeyFriend)
	{
		CComVariant varKeyVal;
		IPropertyList *pFriendList = NULL;

		// check for friend access
		hr = get_KeyValue(kbstrFriends, &varKeyVal);
		if (SUCCEEDED(hr))
		{
			long lFriendLoc = 0L;

			pFriendList = (IPropertyList *)varKeyVal.pdispVal;
			SAFEADDREF(pFriendList);

			hr = pFriendList->Find(CComVariant(pUser), &lFriendLoc);

			if (FAILED(hr) || (lFriendLoc == -1))
			{
#ifdef _DEBUG
				VWTRACE(m_pWorld, "VWPROPERTY", VWT_ERROR, "CAvatarProfileObject::IsKeyReadable: user (%s) not in friends list, not readable\n", CString(bstrUser));
#endif
				bReadable = FALSE;
			}
			else
			{
#ifdef _DEBUG
				VWTRACE(m_pWorld, "VWPROPERTY", VWT_IMPORTANT, "CAvatarProfileObject::IsKeyReadable: user (%s) in friends list, readable\n", CString(bstrUser));
#endif
				bReadable = TRUE;
			}
		
			SAFERELEASE(pFriendList);
		}
		else
		{
			VWTRACE(m_pWorld, "VWPROPERTY", VWT_ERROR, "CAvatarProfileObject::IsKeyReadable: failed to read friends key, not readable\n");
			bReadable = FALSE;
		}
	}
	else
	{
		VWTRACE(m_pWorld, "VWPROPERTY", VWT_ERROR, "CAvatarProfileObject::IsKeyReadable: invalid access type\n");
		ASSERT(FALSE);
		hr = E_INVALIDARG;
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pKeyOwner);
	SAFERELEASE(pUser);

	return bReadable;
}


//******************************************************
// AddKey
// Adds a key to the Profile.  
// By default, all keys are owned by this profile's avatar.
// However, the user can make itself the owner if it
// so desires...

STDMETHODIMP CAvatarProfileObject::AddKey(BSTR bstrKey, VARIANT varKeyVal, VARIANT varStrGraphic, VARIANT varAccess, VARIANT varOwner)
{
	HRESULT hr = S_OK;
	CComVariant varTemp;
	long psbits = 0;
	IThing* pUser = NULL;
	IThing* pCaller = NULL;
	IThing* pOwner = NULL;
	IAvatarProfileData* pProfileData = NULL;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	// REVIEW: should really create an AddKeyExt method which takes the real datatypes

	hr = VariantCopyInd(&varTemp, &varKeyVal);
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
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CAvatarProfileObject::AddKey\n");
#endif

	if (bShouldExecute == VARIANT_TRUE)
	{
		VARIANT_BOOL bValid = VARIANT_FALSE;
		CComBSTR bstrGraphic;
		short sAccess = 0;

		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CAvatarProfileObject::AddKey: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		if (m_pKeys == NULL)
		{
			VWTRACE(m_pWorld, "VWPROPERTY", VWT_ERROR, "CAvatarProfileObject::AddKey: invalid key map\n");
			hr = VWAVATARPROFILE_E_INVALIDKEYMAP;
			goto ERROR_ENCOUNTERED;
		}

		if (m_pBannedObjects == NULL)
		{
			VWTRACE(m_pWorld, "VWPROPERTY", VWT_ERROR, "CAvatarProfileObject::AddKey: invalid banned object list\n");
			hr = VWAVATARPROFILE_E_INVALIDBANNEDOBJECTLIST;
			goto ERROR_ENCOUNTERED;
		}

		// if user isn't avatar
		if (bShouldRemote == VARIANT_TRUE)
		{
			hr = m_pWorld->get_User(&pUser);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			// we are the originator - check protocol
			if (pUser && m_pAvatar && (pUser != m_pAvatar))
			{
				long lIndex = -1;

				/* hr = */ m_pBannedObjects->Find(CComVariant(pUser), &lIndex);

				// if user is banned object, fail
				if (lIndex != -1)
				{
					VWTRACE(m_pWorld, "VWPROPERTY", VWT_ERROR, "CAvatarProfileObject::AddKey: user is banned object, failed to add key\n");
					hr = VWAVATARPROFILE_E_BANNEDOBJECT;
					goto ERROR_ENCOUNTERED;
				}
			}
		}

		// if key already exists, fail
		hr = m_pKeys->IsValid(bstrKey, &bValid);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (bValid == VARIANT_TRUE)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWPROPERTY", VWT_IMPORTANT, "CAvatarProfileObject::AddKey: key (%s) already exists, failed to add key\n", CString(bstrKey));
#endif
			hr = VWAVATARPROFILE_E_KEYEXISTS;
			goto ERROR_ENCOUNTERED;
		}

		// add the property..
		pProfileData = new CComAvatarProfileData;
		if (pProfileData == NULL)
			goto ERROR_ENCOUNTERED;

		pProfileData->AddRef();

		/* hr = */ pProfileData->put_World(m_pWorld);

		// now, create the optional data items
		if (varStrGraphic.vt == VT_BSTR)
			bstrGraphic = varStrGraphic.bstrVal;
		
		if ((varAccess.vt == VT_EMPTY) || (varAccess.vt == VT_ERROR))
			sAccess = kKeyPublic;
		else
		{
			CComVariant varI2;

			::VariantChangeType(&varI2, &varAccess, 0, VT_I2);
			ASSERT(varI2.vt == VT_I2);

			sAccess = (short)varI2.iVal;
		}

		if (varOwner.vt == VT_DISPATCH)
		{
			pOwner = (IThing *)varOwner.pdispVal;
			SAFEADDREF(pOwner);
		}
		else
		{
			// if no owner passed in, set owner to be avatar
			pOwner = m_pAvatar;
			SAFEADDREF(pOwner);
		}

		// can only set the owner to the user
		if (bShouldRemote == VARIANT_TRUE)
		{
			// REVIEW: why are we resetting the owner?

			hr = m_pWorld->get_Caller(&pCaller);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			// check protocol:
			// otherwise, make sure passed in owner is either user or caller
			if ((pOwner != NULL) && (pUser != NULL) && (pCaller != NULL) && (pOwner != pUser) && (pOwner != pCaller))
			{
#ifdef _DEBUG
				CComBSTR bstrUser("n/a"), bstrCaller("n/a"), bstrOwner("n/a");

				/* hr = */ pUser->get_Name(&bstrUser.m_str);
				/* hr = */ pCaller->get_Name(&bstrCaller.m_str);
				/* hr = */ pOwner->get_Name(&bstrOwner.m_str);

				VWTRACE(m_pWorld, "VWPROPERTY", VWT_ERROR, "CAvatarProfileObject::AddKey: invalid owner (not user or caller), user (%s), caller (%s), owner (%s)\n", 
					CString(bstrUser), CString(bstrCaller), CString(bstrOwner));
#endif

				hr = VWAVATARPROFILE_E_INVALIDOWNER;
				goto ERROR_ENCOUNTERED;
			}
		}

		/* hr = */ pProfileData->put_Value(varTemp);
		/* hr = */ pProfileData->put_Owner(pOwner);
		/* hr = */ pProfileData->put_Graphic(bstrGraphic);
		/* hr = */ pProfileData->put_Access(sAccess);

		hr = m_pKeys->put_ObjectProperty(bstrKey, pProfileData);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	// remote
	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 5);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[4].vt = VT_BSTR;
		dispparams.rgvarg[4].bstrVal = bstrKey;
		dispparams.rgvarg[3] = varTemp;
		dispparams.rgvarg[2] = varStrGraphic;
		dispparams.rgvarg[1] = varAccess;
		dispparams.rgvarg[0] = varOwner;

		hr = SendObjectPropertyCommand(psbits, DISPID_AVATARPROFILE_ADDKEY, DISPATCH_METHOD, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	// Notify
	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, profileAddKey, CComVariant(bstrKey));
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}
	
ERROR_ENCOUNTERED:
	SAFERELEASE(pOwner);
	SAFERELEASE(pUser);
	SAFERELEASE(pCaller);
	SAFERELEASE(pProfileData);
	
	return hr;
}

HRESULT CAvatarProfileObject::get_Key(BSTR bstrKey, IAvatarProfileData** ppProfileData)
{
	HRESULT hr = S_OK;

	if (ppProfileData == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CAvatarProfileObject::get_Key: outgoing pointer (IAvatarProfileData**) is NULL\n");
		return E_POINTER;
	}

	*ppProfileData = NULL;

	if (m_pKeys == NULL)
	{
		VWTRACE(m_pWorld, "VWPROPERTY", VWT_ERROR, "CAvatarProfileObject::get_Key: invalid key map\n");
		hr = VWAVATARPROFILE_E_INVALIDKEYMAP;
		goto ERROR_ENCOUNTERED;
	}

	hr = m_pKeys->get_ObjectProperty(bstrKey, (IObjectProperty**)ppProfileData);
	if (FAILED(hr) || *ppProfileData == NULL)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWPROPERTY", VWT_IMPORTANT, "CAvatarProfileObject::get_Key: key (%s) does not exist\n", CString(bstrKey));
#endif
		hr = VWAVATARPROFILE_E_KEYNOTEXIST;
		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	return hr;
}

//******************************************************
// IsValid
// Returns TRUE if the specified key is valid

STDMETHODIMP CAvatarProfileObject::IsValid(BSTR bstrKey, VARIANT_BOOL *pbValid)
{
	HRESULT hr = S_OK;
	IAvatarProfileData* pProfileData = NULL;

	if (pbValid == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CAvatarProfileObject::IsValid: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return E_POINTER;
	}

	*pbValid = VARIANT_FALSE;

	hr = get_Key(bstrKey, &pProfileData);
	if (FAILED(hr))
	{
		hr = S_OK; // DV:  this is not an error - this is what we are checking for...
		goto ERROR_ENCOUNTERED;
	}

	// REVIEW: need to check security in IsValid?
	if (IsKeyReadable(pProfileData))
		*pbValid = VARIANT_TRUE;

ERROR_ENCOUNTERED:
	SAFERELEASE(pProfileData);

	return hr;
}


//******************************************************
// get_KeyValue
// Returns the value of the key, if the key is accessible.
// Only the avatar & the key's owner can read private keys.
// Anyone can read public keys.

STDMETHODIMP CAvatarProfileObject::get_KeyValue(BSTR bstrKey, VARIANT *pVarKeyVal)
{
	HRESULT hr = S_OK;
	IAvatarProfileData* pProfileData = NULL;

	if (pVarKeyVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CAvatarProfileObject::get_KeyValue: outgoing pointer (VARIANT*) is NULL\n");
		return E_POINTER;
	}

	::VariantClear(pVarKeyVal);

	hr = get_Key(bstrKey, &pProfileData);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (!IsKeyReadable(pProfileData))
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWPROPERTY", VWT_ERROR, "CAvatarProfileObject::get_KeyValue: key (%s) isn't readable, failed to get key value\n", CString(bstrKey));
#endif
		hr = VWAVATARPROFILE_E_PERMISSIONDENIED;
	}
	else
		hr = pProfileData->get_Value(pVarKeyVal);

ERROR_ENCOUNTERED:
	SAFERELEASE(pProfileData);

	return hr;
}

//******************************************************
// get_KeyGraphic
// Returns the URL of the key's graphic.  Only accessible if you can
// read the key in the first place.

STDMETHODIMP CAvatarProfileObject::get_KeyGraphic(BSTR bstrKey, BSTR *pbstrGraphic)
{
	HRESULT hr = S_OK;
	IAvatarProfileData* pProfileData = NULL;

	if (pbstrGraphic == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CAvatarProfileObject::get_KeyGraphic: outgoing pointer (BSTR*) is NULL\n");
		return E_POINTER;
	}

	*pbstrGraphic = NULL;

	hr = get_Key(bstrKey, &pProfileData);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (!IsKeyReadable(pProfileData))
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWPROPERTY", VWT_ERROR, "CAvatarProfileObject::get_KeyGraphic: key (%s) isn't readable, failed to get key value\n", CString(bstrKey));
#endif
		hr = VWAVATARPROFILE_E_PERMISSIONDENIED;
	}
	else
		hr = pProfileData->get_Graphic(pbstrGraphic);

ERROR_ENCOUNTERED:
	SAFERELEASE(pProfileData);

	return hr;
}


//******************************************************
// get_KeyGraphic
// Returns the URL of the key's graphic.  Only accessible if you can
// read the key in the first place.

STDMETHODIMP CAvatarProfileObject::get_KeyAccess(BSTR bstrKey, short *psAccess)
{
	HRESULT hr = S_OK;
	IAvatarProfileData* pProfileData = NULL;

	if (psAccess == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CAvatarProfileObject::get_KeyAccess: outgoing pointer (short*) is NULL\n");
		return E_POINTER;
	}

	*psAccess = kKeyPublic;

	hr = get_Key(bstrKey, &pProfileData);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (!IsKeyReadable(pProfileData))
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWPROPERTY", VWT_ERROR, "CAvatarProfileObject::get_KeyAccess: key (%s) isn't readable, failed to get key value\n", CString(bstrKey));
#endif
		hr = VWAVATARPROFILE_E_PERMISSIONDENIED;
	}
	else
		hr = pProfileData->get_Access(psAccess);

ERROR_ENCOUNTERED:
	SAFERELEASE(pProfileData);

	return hr;
}

//******************************************************
// get_KeyOwner
// Returns the owner of the key.  This method is only callable
// by the avatar itself.

STDMETHODIMP CAvatarProfileObject::get_KeyOwner(BSTR bstrKey, IThing **ppOwner)
{
	HRESULT hr = S_OK;
	IAvatarProfileData* pProfileData = NULL;

	if (ppOwner == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CAvatarProfileObject::get_KeyOwner: outgoing pointer (IThing**) is NULL\n");
		return E_POINTER;
	}

	*ppOwner = NULL;

	hr = get_Key(bstrKey, &pProfileData);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (!IsKeyReadable(pProfileData))
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWPROPERTY", VWT_ERROR, "CAvatarProfileObject::get_KeyOwner: key (%s) isn't readable, failed to get key value\n", CString(bstrKey));
#endif
		hr = VWAVATARPROFILE_E_PERMISSIONDENIED;
	}
	else
		hr = pProfileData->get_Owner(ppOwner);

ERROR_ENCOUNTERED:
	SAFERELEASE(pProfileData);

	return hr;
}

//******************************************************
// RemoveKey
// Removes a key from the object.  Only the avatar and the key
// owner can remove a key.

STDMETHODIMP CAvatarProfileObject::RemoveKey(BSTR bstrKey)
{
	HRESULT hr = S_OK;
	IThing* pOwner = NULL;
	IThing* pUser = NULL;
	long psbits = 0;
	IAvatarProfileData* pProfileData = NULL;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldRemote == VARIANT_TRUE)
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CAvatarProfileObject::RemoveKey\n");
#endif
	
	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CAvatarProfileObject::RemoveKey: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		hr = get_Key(bstrKey, &pProfileData);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// if user isn't avatar
		if (bShouldRemote == VARIANT_TRUE)
		{
			hr = m_pWorld->get_User(&pUser);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
			
			// check protocol
			if (pUser && m_pAvatar && (pUser != m_pAvatar))
			{
				/* hr = */ pProfileData->get_Owner(&pOwner);

				// then user must be owner of key
				if (pOwner && pOwner != pUser)			
				{
#ifdef _DEBUG
					CComBSTR bstrUser("n/a"), bstrOwner("n/a");

					/* hr = */ pUser->get_Name(&bstrUser.m_str);
					/* hr = */ pOwner->get_Name(&bstrOwner.m_str);

					VWTRACE(m_pWorld, "VWPROPERTY", VWT_ERROR, "CAvatarProfileObject::RemoveKey: key (%s) isn't readable, user (%s), owner (%s), failed to remove key\n", CString(bstrKey), CString(bstrUser), CString(bstrOwner));
#endif

					hr = VWAVATARPROFILE_E_PERMISSIONDENIED;
					goto ERROR_ENCOUNTERED;
				}
			}
		}

		hr = m_pKeys->Remove(bstrKey);
	}

	// remote
	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstrKey;

		hr = SendObjectPropertyCommand(psbits, DISPID_AVATARPROFILE_REMOVEKEY, DISPATCH_METHOD, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	// Notify
	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, profileRemoveKey, CComVariant(bstrKey));
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pProfileData);
	SAFERELEASE(pOwner);
	SAFERELEASE(pUser);

	return hr;
}

//******************************************************
// put_KeyValue
// Sets the value of an existing key.  Only the owner of the
// key can set its value.

STDMETHODIMP CAvatarProfileObject::put_KeyValue(BSTR bstrKey, VARIANT varNewVal)
{
	HRESULT hr = S_OK;
	IThing* pUser = NULL;
	IThing* pOwner = NULL;
	IAvatarProfileData* pProfileData = NULL;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;
	
	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldRemote == VARIANT_TRUE)
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CAvatarProfileObject::put_KeyValue\n");
#endif

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CAvatarProfileObject::put_KeyValue: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		hr = get_Key(bstrKey, &pProfileData);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (bShouldRemote == VARIANT_TRUE)
		{
			hr = m_pWorld->get_User(&pUser);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
			
			/* hr = */ pProfileData->get_Owner(&pOwner);
		
			// a null user or null owner can always set anything...
			if (pUser && pOwner && (pUser != pOwner))	// NULL user can write to any key!
			{
#ifdef _DEBUG
				CComBSTR bstrUser("n/a"), bstrOwner("n/a");

				/* hr = */ pUser->get_Name(&bstrUser.m_str);
				/* hr = */ pOwner->get_Name(&bstrOwner.m_str);

				VWTRACE(m_pWorld, "VWPROPERTY", VWT_ERROR, "CAvatarProfileObject::put_KeyValue: key (%s) isn't readable, user (%s), owner (%s), failed to put key value\n", CString(bstrKey), CString(bstrUser), CString(bstrOwner));
#endif
				hr = VWAVATARPROFILE_E_PERMISSIONDENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		hr = pProfileData->put_Value(varNewVal);
	}

	// remote
	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 2);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[1].vt = VT_BSTR;
		dispparams.rgvarg[1].bstrVal = bstrKey;
		dispparams.rgvarg[0] = varNewVal;

		hr = SendObjectPropertyCommand(psbits, DISPID_AVATARPROFILE_KEYVALUE, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	// Notify
	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, profileKeyValue, CComVariant(bstrKey));
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}
	

ERROR_ENCOUNTERED:
	SAFERELEASE(pProfileData);
	SAFERELEASE(pOwner);
	SAFERELEASE(pUser);

	return hr;
}

//******************************************************
// put_KeyGraphic
// Sets the graphic associated with the key.  Only the key's
// owner can change a key's graphic.

STDMETHODIMP CAvatarProfileObject::put_KeyGraphic(BSTR bstrKey, BSTR bstrGraphic)
{
	HRESULT hr = S_OK;
	IThing* pUser = NULL;
	IThing* pOwner = NULL;
	IAvatarProfileData* pProfileData = NULL;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldRemote == VARIANT_TRUE)
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CAvatarProfileObject::put_KeyGraphic\n");
#endif

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CAvatarProfileObject::put_KeyGraphic: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		hr = get_Key(bstrKey, &pProfileData);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (bShouldRemote == VARIANT_TRUE)
		{
			hr = m_pWorld->get_User(&pUser);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
			
			/* hr = */ pProfileData->get_Owner(&pOwner);

			// if user isn't owner of key
			if (pUser && pOwner && (pUser != pOwner))
			{
#ifdef _DEBUG
				CComBSTR bstrUser("n/a"), bstrOwner("n/a");

				/* hr = */ pUser->get_Name(&bstrUser.m_str);
				/* hr = */ pOwner->get_Name(&bstrOwner.m_str);

				VWTRACE(m_pWorld, "VWPROPERTY", VWT_ERROR, "CAvatarProfileObject::put_KeyGraphics: key (%s) isn't readable, user (%s), owner (%s), failed to put key value\n", CString(bstrKey), CString(bstrUser), CString(bstrOwner));
#endif
				hr = VWAVATARPROFILE_E_PERMISSIONDENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		/* hr = */ pProfileData->put_Graphic(bstrGraphic);
	}

	// remote
	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 2);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[1].vt = VT_BSTR;
		dispparams.rgvarg[1].bstrVal = bstrKey;
		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstrGraphic;

		hr = SendObjectPropertyCommand(psbits, DISPID_AVATARPROFILE_KEYGRAPHIC, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	// Notify
	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, profileKeyGraphic, CComVariant(bstrKey));
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}
	
ERROR_ENCOUNTERED:
	SAFERELEASE(pProfileData);
	SAFERELEASE(pOwner);
	SAFERELEASE(pUser);

	return hr;
}

//******************************************************
// put_KeyAccess
// Sets whether a key has public access or not.  Only the avatar
// itself can make a key public.

STDMETHODIMP CAvatarProfileObject::put_KeyAccess(BSTR bstrKey, short sAccess)
{
	HRESULT hr = S_OK;
	IThing* pUser = NULL;
	IAvatarProfileData* pProfileData = NULL;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldRemote == VARIANT_TRUE)
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CAvatarProfileObject::put_KeyAccess\n");
#endif
	
	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CAvatarProfileObject::put_KeyAccess: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		hr = get_Key(bstrKey, &pProfileData);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// if user isn't avatar
		if (bShouldRemote == VARIANT_TRUE)
		{
			hr = m_pWorld->get_User(&pUser);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
			
			if (pUser && m_pAvatar && pUser != m_pAvatar)
			{
#ifdef _DEBUG
				CComBSTR bstrUser("n/a"), bstrAvatar("n/a");

				/* hr = */ pUser->get_Name(&bstrUser.m_str);
				/* hr = */ m_pAvatar->get_Name(&bstrAvatar.m_str);

				VWTRACE(m_pWorld, "VWPROPERTY", VWT_ERROR, "CAvatarProfileObject::put_KeyAccess: key (%s) isn't readable, user (%s), avatar (%s), failed to put key value\n", CString(bstrKey), CString(bstrUser), CString(bstrAvatar));
#endif

				hr = VWAVATARPROFILE_E_PERMISSIONDENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		/* hr = */ pProfileData->put_Access(sAccess);
	}

	// remote
	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 2);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[1].vt = VT_BSTR;
		dispparams.rgvarg[1].bstrVal = bstrKey;
		dispparams.rgvarg[0].vt = VT_I2;
		dispparams.rgvarg[0].iVal = sAccess;

		hr = SendObjectPropertyCommand(psbits, DISPID_AVATARPROFILE_KEYACCESS, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	// Notify
	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, profileKeyAccess, CComVariant(bstrKey));
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pProfileData);
	SAFERELEASE(pUser);

	return hr;
}

//******************************************************
// BanObject
// Bans the object from adding more keys to the profile.

STDMETHODIMP CAvatarProfileObject::BanObject(IThing *pThing)
{
	HRESULT hr = S_OK;
	IThing* pUser = NULL;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldRemote == VARIANT_TRUE)
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CAvatarProfileObject::BanObject\n");
#endif

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CAvatarProfileObject::BanObject: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		if (m_pBannedObjects == NULL)
		{
			VWTRACE(m_pWorld, "VWPROPERTY", VWT_ERROR, "CAvatarProfileObject::BanObject: invalid banned object list\n");
			hr = VWAVATARPROFILE_E_INVALIDBANNEDOBJECTLIST;
			goto ERROR_ENCOUNTERED;
		}

		if (bShouldRemote == VARIANT_FALSE) 
		{
			hr = m_pWorld->get_User(&pUser);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
			
			if (!pUser || (pUser && m_pAvatar && (pUser == m_pAvatar)))
			{
				long lIndex = -1;

				/* hr = */ m_pBannedObjects->Find(CComVariant(pUser), &lIndex);

				// add them if they are not there already
				if (lIndex == -1)
					hr = m_pBannedObjects->AddThing(pThing);
			}
		}
	}

	// remote
	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = pThing;

		hr = SendObjectPropertyCommand(psbits, DISPID_AVATARPROFILE_BANOBJECT, DISPATCH_METHOD, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	// Notify
	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, profileBanObject, CComVariant(pThing));
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pUser);

	return hr;
}

//******************************************************
// UnbanObject
// Removes the ban on an object, allowing it to once again add
// keys to the profile.

STDMETHODIMP CAvatarProfileObject::UnbanObject(IThing *pThing)
{
	HRESULT hr = S_OK;
	IThing* pUser = NULL;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldRemote == VARIANT_TRUE)
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CAvatarProfileObject::UnbanObject\n");
#endif	

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CAvatarProfileObject::UnbanObject: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		if (m_pBannedObjects == NULL)
		{
			VWTRACE(m_pWorld, "VWPROPERTY", VWT_ERROR, "CAvatarProfileObject::UnbanObject: invalid banned object list\n");
			hr = VWAVATARPROFILE_E_INVALIDBANNEDOBJECTLIST;
			goto ERROR_ENCOUNTERED;
		}

		if (bShouldRemote == VARIANT_FALSE)
		{
			hr = m_pWorld->get_User(&pUser);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
			
			if (!pUser || (pUser && m_pAvatar && (pUser == m_pAvatar)))
				m_pBannedObjects->RemoveThing(pThing);
		}
	}

	// remote
	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = pThing;

		hr = SendObjectPropertyCommand(psbits, DISPID_AVATARPROFILE_UNBANOBJECT, DISPATCH_METHOD, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	// Notify
	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, profileUnbanObject, CComVariant(pThing));
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pUser);

	return hr;
}

//******************************************************
// Marshall
// Marshalls the avatar to a buffer

STDMETHODIMP CAvatarProfileObject::Marshall(DWORD dwOptions, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	ASSERT(pbuffer);

	hr = CAvatarProfileObjectBase::Marshall(dwOptions, pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pWorld->MarshallObjectProperty(m_pBannedObjects, clsidPropertyList, 
		dwOptions, pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pWorld->MarshallObjectProperty(m_pKeys, clsidPropertyMap, 
		dwOptions, pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pWorld->MarshallObjectProperty(m_pAvatar, clsidThing, 
		MARSHALLREF(dwOptions, MARSHALL_BYREF), pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	return hr;
}

//******************************************************
// UnMarshall
// Unmarshals the avatar from a buffer

STDMETHODIMP CAvatarProfileObject::UnMarshall(IUnMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	ASSERT(pbuffer);

	//clear current data.
	SAFERELEASE(m_pBannedObjects);
	SAFERELEASE(m_pKeys);
	SAFERELEASE(m_pAvatar);

	hr = CAvatarProfileObjectBase::UnMarshall(pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pWorld->UnMarshallObjectProperty(pbuffer, (IObjectProperty**)&m_pBannedObjects);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pWorld->UnMarshallObjectProperty(pbuffer, (IObjectProperty**)&m_pKeys);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pWorld->UnMarshallObjectProperty(pbuffer, (IObjectProperty**)&m_pAvatar);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	return hr;
}

//******************************************************
// ReadFromDisk
// Reads the avatar from disk

STDMETHODIMP CAvatarProfileObject::ReadFromDisk(BSTR bstrFilename)
{
	//clear current data.
	SAFERELEASE(m_pBannedObjects);
	SAFERELEASE(m_pKeys);

	//load into buffer.
	IUnMarshallBuffer *pbuffer = NULL;

	HRESULT hr = CoCreateInstance(CLSID_UnMarshallBuffer, NULL, CLSCTX_INPROC_SERVER,
		IID_IUnMarshallBuffer, (LPVOID*)&pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->ReadFromDisk(bstrFilename);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	//load into current object.
	hr = UnMarshall(pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		Reset();

	SAFERELEASE(pbuffer);

	return hr;
}

//******************************************************
// WriteToDisk
// Writes the avatar to disk

STDMETHODIMP CAvatarProfileObject::WriteToDisk(BSTR bstrFilename)
{
	IMarshallBuffer *pbuffer = NULL;

	HRESULT hr = CoCreateInstance(CLSID_MarshallBuffer, NULL, CLSCTX_INPROC_SERVER,
		IID_IMarshallBuffer, (LPVOID*)&pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
	hr = Marshall(MARSHALL_BYGUID | MARSHALL_BYVALUE | MARSHALL_TODISK, pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->WriteToDisk(bstrFilename);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	SAFERELEASE(pbuffer);

	return hr;
}

//******************************************************
// get_Keys
// Returns a SafeArray of all of the keys in this
// profile

STDMETHODIMP CAvatarProfileObject::get_Keys(IPropertyList** ppKeyList)
{
	HRESULT hr = S_OK;
	IAvatarProfileData* pProfileData = NULL;
	CComBSTR curStr;
	VARIANT_BOOL bDone;
	CComVariant varVal;
	long cKeys = 0;
	
	if (ppKeyList == NULL)
		return E_POINTER;

	*ppKeyList = NULL;

	if (m_pKeys == NULL)
	{
		VWTRACE(m_pWorld, "VWPROPERTY", VWT_ERROR, "CAvatarProfileObject::get_Keys: invalid key map\n");
		hr = VWAVATARPROFILE_E_INVALIDKEYMAP;
		goto ERROR_ENCOUNTERED;
	}

	hr = CreatePropertyList(m_pWorld, ppKeyList);
	if (FAILED(hr) || (!*ppKeyList))
		goto ERROR_ENCOUNTERED;

	// map over the key list, collecting the names of valid keys
	m_pKeys->FirstItem(&curStr.m_str, &varVal, &bDone);

	while (bDone == VARIANT_FALSE)
	{
		hr = m_pKeys->get_ObjectProperty(curStr, (IObjectProperty **)&pProfileData);
		if (SUCCEEDED(hr) && pProfileData)
		{
			if (IsKeyReadable(pProfileData))
				(*ppKeyList)->AddString(curStr);

			SAFERELEASE(pProfileData);
		}

		curStr.Empty();
		varVal.Clear();

		m_pKeys->NextItem(&curStr.m_str, &varVal, &bDone);
	}
		
ERROR_ENCOUNTERED:
	return hr;
}


//******************************************************
// Reset
// Resets the avatar profile to a known good state

STDMETHODIMP CAvatarProfileObject::Reset(void)
{
	HRESULT	hr = S_OK;

	SAFERELEASE(m_pBannedObjects);
	SAFERELEASE(m_pKeys);

	// banned objects
	if (m_pWorld == NULL)
		return VWOBJECT_E_INVALIDWORLD;

	hr = CreatePropertyList(m_pWorld, &m_pBannedObjects);
	if (FAILED(hr) || (!m_pBannedObjects))
		goto ERROR_ENCOUNTERED;

	// keys - the actual values
	hr = CreatePropertyMap(m_pWorld, &m_pKeys);
	if (FAILED(hr) || (!m_pKeys))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{
		SAFERELEASE(m_pBannedObjects);
		SAFERELEASE(m_pKeys);
	}

	return hr;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CAvatarProfileDataObject

CAvatarProfileDataObject::CAvatarProfileDataObject()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CAvatarProfileDataObject::CAvatarProfileDataObject, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif

	m_pOwner = NULL;
	m_sAccess = 0;
}

CAvatarProfileDataObject::~CAvatarProfileDataObject()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CAvatarProfileDataObject::~CAvatarProfileDataObject, this 0x%0x\n", this);
#endif

	Terminate();
}

STDMETHODIMP CAvatarProfileDataObject::Terminate(void)
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CAvatarProfileDataObject::Terminate, this 0x%0x\n", this);
#endif

	CAvatarProfileDataObjectBase::Terminate();

	SAFERELEASE(m_pOwner);

	return S_OK;
}

STDMETHODIMP CAvatarProfileDataObject::get_Value(VARIANT* pvarValue)
{
	if (pvarValue == NULL)
		return E_POINTER;

	::VariantCopyInd(pvarValue, &m_varValue); 

	return S_OK;
}

STDMETHODIMP CAvatarProfileDataObject::put_Value(VARIANT varValue)
{
	::VariantCopyInd(&m_varValue, &varValue);

	return S_OK;
}

STDMETHODIMP CAvatarProfileDataObject::get_Graphic(BSTR* pbstrGraphic)
{
	if (pbstrGraphic == NULL)
		return E_POINTER;

	*pbstrGraphic = m_bstrGraphic.Copy();

	return S_OK;
}

STDMETHODIMP CAvatarProfileDataObject::put_Graphic(BSTR bstrGraphic)
{
	m_bstrGraphic = bstrGraphic;

	return S_OK;
}

STDMETHODIMP CAvatarProfileDataObject::get_Owner(IThing** ppOwner)
{
	if (ppOwner == NULL)
		return E_POINTER;

	*ppOwner = m_pOwner;
	SAFEADDREF(*ppOwner);

	return S_OK;
}

STDMETHODIMP CAvatarProfileDataObject::put_Owner(IThing* pOwner)
{
	SAFERELEASE(m_pOwner);
	m_pOwner = pOwner;
	SAFEADDREF(m_pOwner);

	return S_OK;
}

STDMETHODIMP CAvatarProfileDataObject::get_Access(short* psAccess)
{
	if (psAccess == NULL)
		return E_POINTER;

	*psAccess = m_sAccess;

	return S_OK;
}

STDMETHODIMP CAvatarProfileDataObject::put_Access(short sAccess)
{
	m_sAccess = sAccess;

	return S_OK;
}

STDMETHODIMP CAvatarProfileDataObject::Marshall(DWORD dwOptions, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	ASSERT(pbuffer);

	hr = CAvatarProfileDataObjectBase::Marshall(dwOptions, pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->put_ULong(DATABASE_VERSION);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->put_Short(m_sAccess);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->put_String(m_bstrGraphic);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pWorld->MarshallProperty(m_varValue, MARSHALLREF(dwOptions, MARSHALL_BYREF), pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pWorld->MarshallObjectProperty(m_pOwner, clsidThing, 
		MARSHALLREF(dwOptions, MARSHALL_BYREF), pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CAvatarProfileDataObject::UnMarshall(IUnMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;
	unsigned long ulVersion = 0L;

	// free alloced data
	m_bstrGraphic.Empty();
	m_varValue.Clear();
	m_sAccess = 0;
	SAFERELEASE(m_pOwner);

	ASSERT(pbuffer);

	hr = CAvatarProfileDataObjectBase::UnMarshall(pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->get_ULong(&ulVersion);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (ulVersion != DATABASE_VERSION)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CAvatarProfileDataObject::UnMarshall: profile version number doesn't match client version number\n");
#endif
		hr = VWOBJECT_E_INVALIDPROFILEVERSION;
		goto ERROR_ENCOUNTERED;
	}

	hr = pbuffer->get_Short(&m_sAccess);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->get_String(&m_bstrGraphic.m_str);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pWorld->UnMarshallProperty(pbuffer, &m_varValue);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pWorld->UnMarshallObjectProperty(pbuffer, (IObjectProperty**)&m_pOwner);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	return hr;
}

