// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWThing.h : Declaration of the CVWThing

#ifndef __VWTHING_H_
#define __VWTHING_H_

#include "resource.h"       // main symbols

#include <propbase.h>

/////////////////////////////////////////////////////////////////////////////
// CVWThing


#include "objimpl.h"

EXTERN_C const CLSID CLSID_ThingExemplar;

#define ReportVWThingError(hr) SUCCEEDED(hr) ? hr : ReportError("VWSYSTEM.Thing.1", hr, 0xc30, 0xc40, "VWTHING")

DECLARE_VWDISPATCH( ThingExemplar, LIBID_VWSYSTEMLib )
{
public:
	CThingExemplarObject(){};

DECLARE_REGISTRY_RESOURCEID(IDR_THINGEXEMPLAR)

BEGIN_COM_MAP(CThingExemplarObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWDispatch)
	COM_INTERFACE_ENTRY(IThingExemplar)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

// IVWThing
public:
	STDMETHOD(Whisper)(BSTR strText, VARIANT userOrList);
	STDMETHOD(Shout)(BSTR strText);
	STDMETHOD(SayGesture)(BSTR strText);
	STDMETHOD(Say)(BSTR strText, VARIANT userOrList);
	STDMETHOD(Announce)(BSTR strText);
	STDMETHOD(Report)(BSTR strText);
	STDMETHOD(MoveInto)(IThing *pNewContainer);
	STDMETHOD(Emote)(VARIANT varGesture, VARIANT varText);
	STDMETHOD(GestureText)(BSTR bstrGesture, BSTR *pbstrText);
//	STDMETHOD(Destroy)();
	STDMETHOD(Clone)(IThing **ppNewThing);

	STDMETHOD(AllowLeave)(IThing *pThing, VARIANT_BOOL *pbool);
	STDMETHOD(AllowEnter)(IThing *pThing, VARIANT_BOOL *pbool);
	STDMETHOD(AcceptLeave)(VARIANT_BOOL *pbool);
	STDMETHOD(AcceptEnter)(IThing *pThing, VARIANT_BOOL *pbool);

	STDMETHOD(Install)(IModule* pModule);

	// from thingdef
	STDMETHOD(LookAt)();

	STDMETHOD(OnSystemDestroyClient)(IThing* pThing);

	STDMETHOD(OnContentSystemEnter)(IThing *pWhere, IThing *pWhat);
	STDMETHOD(OnContentSystemLeave)(IThing *pOldWhere, IThing *pWhat);

	// Menu support
	STDMETHOD(GetMenuList)(IPropertyList **pplistMenu);
	HRESULT BuildMenuList(IPropertyList* pMenuList);
	HRESULT SetMenuListTarget(IPropertyList* pMenuList, IThing *pTarget, IThing *pOwner);
	HRESULT UpdateMenuList(IPropertyList* pMenuList, IThing *pTarget, IThing *pThing);
	HRESULT AddMenuSource(IThing *pTarget, IThing *pThing, BSTR bstrMethod,
							IPropertyList *pMenuList);
//	HRESULT SortMenuList(IPropertyList* pMenuList);
	STDMETHOD(GetMenuItems)(IThing *pTarget, IPropertyList **ppMenuList);
	STDMETHOD(GetPeerMenuItems)(IThing *pTarget, IPropertyList **ppMenuList);
	STDMETHOD(GetContainerMenuItems)(IThing *pTarget, IPropertyList **ppMenuList);
	STDMETHOD(GetContentMenuItems)(IThing *pTarget, IPropertyList **ppMenuList);
	STDMETHOD(GetSelfMenuItems)(IThing *pTarget, IPropertyList **ppMenuList);
	STDMETHOD(GetInventoryMenuItems)(IThing *pTarget, IPropertyList **ppMenuList);
	STDMETHOD(GetEditMenuItems)(IThing *pTarget, IPropertyList **ppMenuList);
	STDMETHOD(GetMenuItemsFromProperty)(BSTR bstrMenuListProperty,
										IPropertyList **ppMenuList);

	STDMETHOD(UpdateIsArtifact)(IMenuItem *pmi);
	STDMETHOD(UpdateIsAvatar)(IMenuItem *pmi);
	STDMETHOD(UpdateIsAvatarAndNotWizard)(IMenuItem *pmi);
	STDMETHOD(UpdateIsArtifactOrAvatar)(IMenuItem *pmi);
	STDMETHOD(UpdateIsTakeable)(IMenuItem *pmi);
	STDMETHOD(UpdateMenuItemName)(IMenuItem *pmi);
	STDMETHOD(UpdatePropertiesMenuItem)(IMenuItem *pmi);
	STDMETHOD(UpdatePropertyListMenu)(IMenuItem *pmi, IPropertyList *pList);
	STDMETHOD(UpdateInventoryMenu)(IMenuItem *pmi);
	STDMETHOD(UpdateContentsMenu)(IMenuItem *pmi);
	STDMETHOD(UpdateRoomAvatarsMenu)(IMenuItem *pmi);

	STDMETHOD(DoGesture)(int iGesture);
	STDMETHOD(DoNamedGesture)(BSTR bstrGesture);
	STDMETHOD(DoGestureServer)(IThing *pThingOriginator, int iGesture);
	STDMETHOD(DoGestureClient)(IThing *pThingOriginator, int iGesture);

	STDMETHOD(Take)(IThing *pThing);
	STDMETHOD(Drop)(IThing *pThing);

	STDMETHOD(OnIsOpenChanged)(long lHint, VARIANT varHintData);

	// Editor support
	STDMETHOD(EditObject)();
	STDMETHOD(GetEditTabMap)(IPropertyList **pMap);
	STDMETHOD(GetSortedEditTabMap)(IPropertyList **pMap);

	// Client Only property support
	STDMETHOD(AddClientProperty)(BSTR bstrName,VARIANT var);
	STDMETHOD(RemoveClientProperty)(BSTR bstrName);

	// Accessors
	STDMETHOD(OnIsNoticeableAccess)(long lPerms, VARIANT_BOOL *bReturnVal);
	STDMETHOD(OnNameAccess) (long lPerms, VARIANT_BOOL *bReturnVal);
	STDMETHOD(OnOwnerOrWizardAccess) (long lPerms, VARIANT_BOOL *bReturnVal);

	// Utility
	STDMETHOD(CheckPropertyBits)(IThing *pThis, BSTR bstrName);
	STDMETHOD(CheckSecurity)( );

public:
	static HRESULT AppendPropertyMap(IThing *pThing, BSTR bstrPropName, IPropertyMap *pMap);
	static HRESULT MergePropertyMaps(IPropertyMap *pFrom, IPropertyMap *pTo);
	static HRESULT SortEditMap(IPropertyList* pEditMap);
	static HRESULT AppendPropertyList(IThing *pThing, BSTR bstrPropName, IPropertyList *pMenu);
	static HRESULT MergePropertyLists(IPropertyList *pFrom, IPropertyList *pTo);

protected:
	// helper functions

};


#endif //__VWTHING_H_
