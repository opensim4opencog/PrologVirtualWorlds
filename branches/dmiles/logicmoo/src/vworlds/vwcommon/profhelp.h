// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// a variety of profile helper functions....

long	_FindThingInPropList(IPropertyList *pList, IThing *pThing);

HRESULT	_AddToProfileKeyList(IWorld *pWorld, IAvatarProfile *pProfile, BSTR strKey, IThing *pThing, IThing* pOwner = NULL);
HRESULT	_RemoveFromProfileKeyList(IAvatarProfile *pProfile, BSTR strKey, IThing *pThing);
BOOL	_IsObjInProfileKeyList(IAvatarProfile *pProfile, BSTR strKey, IThing *pThing);

HRESULT _AddStringKey(IAvatarProfile *pProfile, BSTR bstrKey, BSTR bstrKeyValue, IThing *pOwner = NULL);
HRESULT _AddListKey(IWorld *pWorld, IAvatarProfile *pProfile, BSTR bstrKey, IThing *pOwner = NULL);
HRESULT _AddMapKey(IWorld *pWorld, IAvatarProfile *pProfile, BSTR bstrKey, IThing *pOwner = NULL);
HRESULT _AddBOOLKey(IAvatarProfile *pProfile, BSTR bstrKey, VARIANT_BOOL bValue, IThing *pOwner = NULL);


