// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// FoundationExemplars.h : Declaration of the CFoundationExemplars


#include "resource.h"       // main symbols

#include <objimpl.h>

#define ReportFoundationError(hr) SUCCEEDED(hr) ? hr : ReportError("VWEXEMP.FoundationExemplars.1", hr, 0xf00, 0x1000)


/////////////////////////////////////////////////////////////////////////////
// FoundationExemplars

DECLARE_VWDISPATCHEX(CFoundationExemplars, IFoundationExemplars, IID_IFoundationExemplars, CLSID_FoundationExemplars, LIBID_VWFOUNDLib)
{
public:
BEGIN_COM_MAP(CFoundationExemplars)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWDispatch)
	COM_INTERFACE_ENTRY(IFoundationExemplars)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

//DECLARE_NOT_AGGREGATABLE(CFoundationExemplars) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

DECLARE_REGISTRY(CFoundationExemplars, _T("VWEXEMP.FoundationExemplars.1"), _T("VWEXEMP.FoundationExemplars"), IDS_FOUNDATIONEXEMPS_DESC, THREADFLAGS_BOTH)

// IFoundationExemplars
public:
	STDMETHOD(Install)(IModule* pModule);

	// Extensions to core exemplars
	HRESULT(CoreInstall)(IModule* pModule);

	// a. Thing
	HRESULT ThngInstall(IModule *pModule);
	STDMETHOD(ThngInteractAt)(IThing *pThing, BSTR bstrSprFile, BSTR bstrSoundURL, BSTR bstrVerb);
	STDMETHOD(ThngOnApproachDone)();
	STDMETHOD(ThngStartInteractSprite)(BSTR bstrSprFile, BSTR bstrSoundURL, float fPosX, float fPosY, float fPosZ);
	STDMETHOD(ThngMonitorInteractSprite)(IThing *pInteractSprite, DWORD dwTimeStarted);
	STDMETHOD(ThngOnDropFile)(long x, long y, BSTR bstrFilename, VARIANT_BOOL *pbHandled);

	// b. Avatar
	HRESULT AvaInstall(IModule *pModule);
	STDMETHOD(AvaIgnoreAvatarMenu)();
	STDMETHOD(AvaUpdateIgnore)(IMenuItem *pmi);
	STDMETHOD(AvaUnIgnoreAvatar)(IThing *pToUnIgnore);
	STDMETHOD(AvaIgnoreAvatar)(IThing *pToIgnore);
	STDMETHOD(AvaBefriendAvatarMenu)();
	STDMETHOD(AvaUpdateFriend)(IMenuItem *pmi);
	STDMETHOD(AvaBefriendAvatar)(IThing *pFriend);
	STDMETHOD(AvaUnBefriendAvatar)(IThing *pFriend);
	STDMETHOD(AvaConfirmFriend)(IThing *pOtherGuy);
	STDMETHOD(AvaFinalizeFriend)(IThing *pNewFriend, VARIANT_BOOL bFriend);
	STDMETHOD(AvaUpdateInteractions)(IMenuItem *pMI);
	STDMETHOD(AvaGetMenuItems)(IThing *pThing, IPropertyList **ppMenuList);
	STDMETHOD(AvaGetPeerMenuItems)(IThing *pPeer, IPropertyList **ppMenuList);
	STDMETHOD(AvaOnPreComposeGeometry)();

	// Host
	HRESULT HostInstall(IModule *pModule);

	// Global methods
	HRESULT GlobInstall(IModule *pModule);
	STDMETHOD(CreateTransactionItem)(BSTR bstrName, BSTR bstrDescription, 
		BSTR bstrGraphic, BSTR bstrExemplarType, IVWTransactionItem **ppTransactionItem);
	STDMETHOD(GetPortalIDs)(long idRoom, IPropertyList **ppPortalList);
	STDMETHOD(GetContainerID)(long idObj, long *pidContainer);
	STDMETHOD(GetNamesFromIDs)(VARIANT idOrList, IPropertyList **ppNameList);

	// Painting exemplar
	HRESULT PaintInstall(IModule* pModule);
	STDMETHOD(OnLoadPainting)(IVWFrame *pvwf);
	STDMETHOD(OnPaintingPropChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(RefreshPainting)();
	STDMETHOD(PaintingUpdateMenu)(IMenuItem *pmi);
	STDMETHOD(OnSelectPaintingCustomize)();

	// Sign exemplar
	HRESULT SignInstall(IModule* pModule);
	STDMETHOD(OnLoadSign)(IVWFrame *pvwf);
	STDMETHOD(OnSignPropChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(SetTextAttributes)(VARIANT_BOOL bShadow, short nWidth, short nHeight, short nPointSize);
	STDMETHOD(SignSetTextureFromText)();
	STDMETHOD(RefreshSign)();
	STDMETHOD(SignUpdateMenu)(IMenuItem *pmi);
	STDMETHOD(OnSelectSignCustomize)();

	// TransactionMachine exemplar
	HRESULT TMchInstall(IModule* pModule);
	STDMETHOD (TMchOnCreate) (IThing* pThing);
	STDMETHOD (TMchSetCurrentIndex)(short nIndex);
	STDMETHOD (TMchUpdateCurrentIndex) (short nChangeAmt);
	STDMETHOD (TMchOnCurrentIndexChanged) (long lHint, VARIANT varHintData);
	STDMETHOD (TMchGetCurrentItem) (IVWTransactionItem **pTransactionItem);
	STDMETHOD (TMchCreateAndDispenseObject)();
	STDMETHOD (TMchPreviousButton_OnLButtonDown)(long hWnd, long x, long y);
	STDMETHOD (TMchNextButton_OnLButtonDown)(long hWnd, long x, long y);
	STDMETHOD (TMchSelectButton_OnLButtonDown)(long hWnd, long x, long y);
	STDMETHOD (TMchDisplay_SetGraphic) (BSTR bstrNewGraphicFile);
};
