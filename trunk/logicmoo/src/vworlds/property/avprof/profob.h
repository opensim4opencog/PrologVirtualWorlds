// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// AvatarProfile.h : Declaration of the CAvatarProfileObject

#ifndef __AVATARPROFILE_H_
#define __AVATARPROFILE_H_

#include "resource.h"       // main symbols
#include <PropBase.h>
#include <objimpl.h>

EXTERN_C const CLSID CLSID_AvatarProfile;
EXTERN_C const CLSID CLSID_AvatarProfileData;

EXTERN_C const CLSID CLSID_MarshallBuffer;
EXTERN_C const CLSID CLSID_UnMarshallBuffer;

/////////////////////////////////////////////////////////////////////////////
// CAvatarProfileObject

DECLARE_OBJECTPROPERTY(AvatarProfile, LIBID_VWSYSTEMLib, IDS_PROFILE_TYPE)
{
public:
	DECLARE_AGGREGATABLE(CAvatarProfileObject)

	CAvatarProfileObject();
	~CAvatarProfileObject();

DECLARE_REGISTRY(CAvatarProfileObject, _T("VWSYSTEM.AvatarProfile.1"), _T("VWSYSTEM.AvatarProfile"), IDS_AVATARPROFILE_DESC, THREADFLAGS_BOTH)

BEGIN_COM_MAP(CAvatarProfileObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectProperty)
	COM_INTERFACE_ENTRY(IAvatarProfile)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

// Override the base class
	STDMETHOD(put_World)(IWorld* pworld);
	STDMETHOD(Marshall)(DWORD dwOptions, IMarshallBuffer* pbuffer); 
	STDMETHOD(UnMarshall)(IUnMarshallBuffer* pbuffer); 
	STDMETHOD(get_RefCount)(DWORD *pdwRef);
	STDMETHOD(Terminate)(void);

// IAvatarProfile
public:	
	STDMETHOD(get_Keys)(IPropertyList** ppKeyList);
	STDMETHOD(IsValid)(BSTR bstrKey, VARIANT_BOOL* pbValid);

	HRESULT get_Key(BSTR bstrKey, IAvatarProfileData** ppProfileData);

	STDMETHOD(AddKey)(BSTR bstrKey, VARIANT varKeyVal, VARIANT varStrGraphic, VARIANT varPublic, VARIANT varOwner);
	STDMETHOD(RemoveKey)(BSTR bstrKey);

	STDMETHOD(get_KeyValue)(BSTR strKey, VARIANT *pVarKeyVal);
	STDMETHOD(get_KeyGraphic)(BSTR strKey, BSTR *pstrGraphic);
	STDMETHOD(get_KeyOwner)(BSTR strKey, IThing **ppOwner);
	STDMETHOD(get_KeyAccess)(BSTR strKey, short* psAccess);

	STDMETHOD(put_KeyValue)(BSTR strKey, VARIANT strVal);
	STDMETHOD(put_KeyGraphic)(BSTR strKey, BSTR strGraphic);
//	STDMETHOD(put_KeyOwner)(BSTR strKey, IThing* pOwner);
	STDMETHOD(put_KeyAccess)(BSTR strKey, short sAccess);

	STDMETHOD(BanObject)(IThing *pThing);
	STDMETHOD(UnbanObject)(IThing *pThing);
	
	STDMETHOD(put_Avatar)(IThing *pThing);
	STDMETHOD(get_Avatar)(IThing **pThing);

	STDMETHOD(ReadFromDisk)(BSTR bstrFilename);
	STDMETHOD(WriteToDisk)(BSTR bstrFilename);

	STDMETHOD(Reset)(void);

private:
	IPropertyList *m_pBannedObjects;
	IPropertyMap *m_pKeys;

	IThing *m_pAvatar;

protected:
	BOOL IsKeyReadable(IAvatarProfileData *pProfileData);
};

DECLARE_OBJECTPROPERTY(AvatarProfileData, LIBID_VWSYSTEMLib, IDS_PROFILEDATA_TYPE)
{
public:
	DECLARE_AGGREGATABLE(CAvatarProfileDataObject)

	CAvatarProfileDataObject();
	~CAvatarProfileDataObject();

DECLARE_REGISTRY(CAvatarProfileDataObject, _T("VWSYSTEM.AvatarProfileData.1"), _T("VWSYSTEM.AvatarProfileData"), IDS_AVATARPROFILEDATA_DESC, THREADFLAGS_BOTH)

BEGIN_COM_MAP(CAvatarProfileDataObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectProperty)
	COM_INTERFACE_ENTRY(IAvatarProfileData)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

// Override the base class
	STDMETHOD(Marshall)(DWORD dwOptions, IMarshallBuffer* pbuffer); 
	STDMETHOD(UnMarshall)(IUnMarshallBuffer* pbuffer); 
	STDMETHOD(get_RefCount)(DWORD *pdwRef);
	STDMETHOD(Terminate)(void);

// IAvatarProfileData
public:	
	STDMETHOD(get_Value)(VARIANT *pVarKeyVal);
	STDMETHOD(get_Graphic)(BSTR *pstrGraphic);
	STDMETHOD(get_Owner)(IThing **ppOwner);
	STDMETHOD(get_Access)(short* psAccess);

	STDMETHOD(put_Value)(VARIANT strVal);
	STDMETHOD(put_Graphic)(BSTR strGraphic);
	STDMETHOD(put_Owner)(IThing* pOwner);
	STDMETHOD(put_Access)(short sAccess);

private:
	CComVariant	m_varValue;
	CComBSTR	m_bstrGraphic;
	IThing*		m_pOwner;
	short		m_sAccess;
};

#endif //__AVATARPROFILE_H_
