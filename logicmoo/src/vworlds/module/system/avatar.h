// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// Avatar.h : Declaration of the CAvatar

#ifndef __AVATAR_H_
#define __AVATAR_H_

#include "resource.h"       // main symbols
#include "objimpl.h"

/////////////////////////////////////////////////////////////////////////////
// CAvatar

EXTERN_C const CLSID CLSID_AvatarExemplar;

#define ReportAvatarError(hr) SUCCEEDED(hr) ? hr : ReportError("VWSYSTEM.Avatar.1", hr, 0xc10, 0xc20, "VWAVATAR")

DECLARE_VWDISPATCH(AvatarExemplar, LIBID_VWSYSTEMLib )
{
public:

DECLARE_REGISTRY_RESOURCEID(IDR_AVATAREXEMPLAR)

BEGIN_COM_MAP(CAvatarExemplarObject)
	COM_INTERFACE_ENTRY(IAvatarExemplar)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

// IAvatar
public:
	STDMETHOD(Install)(IModule* pModule);
	STDMETHOD(Gesture)(BSTR strGestureName);
//	STDMETHOD(RetrieveOwnedObject)(IThing *pThing);
	STDMETHOD(Wield)(IThing *pThing);
	STDMETHOD(UnWield)();
	STDMETHOD(Teleport)(IThing *pNewRoom);

	STDMETHOD(UpdateUserProfileList)(IMenuItem *pmi);
	STDMETHOD(MenuUserProfileList)(BSTR bstrKey);
	STDMETHOD(AddToUserProfileList)(IThing *pThing, BSTR bstrKey);		
	STDMETHOD(RemoveFromUserProfileList)(IThing *pThing, BSTR bstrKey);		
	STDMETHOD(IsInUserProfileList)(IThing *pThing, BSTR bstrKey, VARIANT_BOOL *pBool);

	STDMETHOD(OnSystemCreate)(IThing *pthing);
	STDMETHOD(OnSystemConnect)(IThing *pWho);
	STDMETHOD(OnSystemDisconnect)(IThing *pWho);

	STDMETHOD(OnIsAuthorChanged)(long lHint, VARIANT varHintData);

	STDMETHOD(OnIsWizardOrIsAuthorAccess)(long lPerms, VARIANT_BOOL* bReturnValue);
	STDMETHOD(OnWizardAccess)(long lPerms, VARIANT_BOOL* bReturnValue);

	STDMETHOD(FireConnectAvatar)(IThing *pthing);

	// Menu commands
	STDMETHOD(LookAt)();
	STDMETHOD(EditProfile)();
	STDMETHOD(UpdateMenuItemName)(IMenuItem * pmi);
	STDMETHOD(UpdateWhisperMenuItem)(IMenuItem *pmi);

	// Queries
	STDMETHOD(Query)(BSTR bstrTitle, BSTR bstrHTML, BSTR bstrPrompt, BSTR bstrDefault,
					 VARIANT varNotifyObject, VARIANT varNotifyMethod, VARIANT varNotifyParam);
	STDMETHOD(Alert)(BSTR bstrTitle, BSTR bstrText,
					 VARIANT varNotifyObject, VARIANT varNotifyMethod, VARIANT varNotifyParam);
	STDMETHOD(Confirm)(BSTR bstrTitle, BSTR bstrText,
					   VARIANT varNotifyObject, VARIANT varNotifyMethod, VARIANT varNotifyParam);
	STDMETHOD(Prompt)(BSTR bstrTitle, BSTR bstrPrompt, BSTR bstrDefault,
					  VARIANT varNotifyObject, VARIANT varNotifyMethod, VARIANT varNotifyParam);

	STDMETHOD(ClientQuery)(BSTR bstrTitle, BSTR bstrHTML, BSTR bstrPrompt, BSTR bstrDefault,
						   IThing *pNotifyObject, BSTR bstrNotifyMethod, IDispatch *pNotifyParam);
	STDMETHOD(QueryReturn)(IPropertyList *pArg, VARIANT_BOOL boolOK, BSTR bstrResult);

	// Whisper
	STDMETHOD(WhisperTo)();
	STDMETHOD(GestureText)(BSTR bstrGesture, BSTR *pbstrText);
	STDMETHOD(DoNamedGesture)(BSTR bstrGestureName);
	STDMETHOD(ThumbnailFileName)(BSTR *pbstrFileName);

	STDMETHOD(OnContainerSystemTell)(IThing* pfrom, IPropertyList* pto, BSTR bstr, long lval, BSTR *pbstrOut);
	STDMETHOD(OnSystemTell)(IThing* pfrom, IPropertyList* pto, BSTR bstr, long lval, BSTR *pbstrOut);
	STDMETHOD(OnPeerSystemTell)(IThing* pfrom, IPropertyList* pto, BSTR bstr, long lval);

	// for quota management
	STDMETHOD(OnSystemPreCreate)(IThing *pExemplar, VARIANT_BOOL *pbOkToCreate);
	STDMETHOD(OnSystemPostCreate)(IThing *pThing);
	STDMETHOD(OnSystemPreDestroy)(IThing *pThing, VARIANT_BOOL *pbOkToDestroy);
	STDMETHOD(OnSystemPostDestroy)(IThing *pThing);
	STDMETHOD(SetObjectQuota)(LONG lQuota);

	STDMETHOD(AddDefaultKeys)();
	STDMETHOD(OnAction)(BSTR bstrAction, BSTR bstrDescription, IPropertyList *pParticipants);
};

#endif //__AVATAR_H_
