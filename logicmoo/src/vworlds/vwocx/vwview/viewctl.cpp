// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// ViewCtl.cpp : Implementation of the CVWViewCtrl OLE control class.

#include "stdafx.h"
#include "VWView.h"
#include <vwsystem.h>
#include "ViewCtl.h"
#ifdef _VIEW_STANDALONE_CLASS_
#include "ViewPpg.h"
#endif
#include "propbase.h"

#include "syshelp.h"

#define IID_DEFINED
#include "vwsystem_i.c"
#include "vwprop_i.c"
#include "vwobject_i.c"
#include "vwclient_i.c"
#include "ocxproxy_i.c"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNCREATE(CVWViewCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CVWViewCtrl, COleControl)
	//{{AFX_MSG_MAP(CVWViewCtrl)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CVWViewCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CVWViewCtrl)
	DISP_PROPERTY_EX(CVWViewCtrl, "VWClient", GetVWClient, SetVWClient, VT_DISPATCH)
	DISP_STOCKPROP_READYSTATE()
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CVWViewCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CVWViewCtrl, COleControl)
	//{{AFX_EVENT_MAP(CVWViewCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages
#ifdef _VWSTANDALONE_CLASS_
// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CVWViewCtrl, 1)
	PROPPAGEID(CVWViewPropPage::guid)
END_PROPPAGEIDS(CVWViewCtrl)
#endif

/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CVWViewCtrl, "VWVIEW.VWViewCtrl.1",
	0x812e6303, 0xc8c, 0x11d0, 0x8a, 0x96, 0, 0xa0, 0xc9, 0x8, 0x25, 0x83)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CVWViewCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DVWView =
		{ 0x812e6301, 0xc8c, 0x11d0, { 0x8a, 0x96, 0, 0xa0, 0xc9, 0x8, 0x25, 0x83 } };
const IID BASED_CODE IID_DVWViewEvents =
		{ 0x812e6302, 0xc8c, 0x11d0, { 0x8a, 0x96, 0, 0xa0, 0xc9, 0x8, 0x25, 0x83 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information
/*
static const DWORD BASED_CODE _dwVWViewOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_IGNOREACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;
*/
static const DWORD BASED_CODE _dwVWViewOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

#ifdef _VWSTANDALONE_CLASS_
IMPLEMENT_OLECTLTYPE(CVWViewCtrl, IDS_VWVIEW, _dwVWViewOleMisc)
#else
UINT CVWViewCtrl::GetUserTypeNameID() { ASSERT(FALSE); return 0; }
DWORD CVWViewCtrl::GetMiscStatus() { ASSERT(FALSE); return 0; }
#endif


/////////////////////////////////////////////////////////////////////////////
// CVWViewCtrl::CVWViewCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CVWViewCtrl


BOOL CVWViewCtrl::CVWViewCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegInsertable | afxRegApartmentThreading to afxRegInsertable.

#ifndef _VWSTANDALONE_CLASS_
	return TRUE;
#else

	if (bRegister)
	{
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_VWVIEW,
			IDB_VWVIEW,
			afxRegInsertable | afxRegApartmentThreading,
			_dwVWViewOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	}
	else
	{
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
	}
#endif
}

// event sink stuff
BEGIN_INTERFACE_MAP(CVWViewCtrl, COleControl)
    INTERFACE_PART(CVWViewCtrl, IID_IVWObjectSite, ObjectSite)
	INTERFACE_PART(CVWViewCtrl, IID_IVWClientSite, ClientSite)
	INTERFACE_PART(CVWViewCtrl, IID_IObjectSafety, ObjectSafety)
END_INTERFACE_MAP()

STDMETHODIMP_(ULONG) CVWViewCtrl::XObjectSite::AddRef()
{
	METHOD_PROLOGUE(CVWViewCtrl, ObjectSite)
	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CVWViewCtrl::XObjectSite::Release()
{
	METHOD_PROLOGUE(CVWViewCtrl, ObjectSite)
	return pThis->ExternalRelease();
}

STDMETHODIMP CVWViewCtrl::XObjectSite::QueryInterface(
    REFIID iid, void FAR* FAR* ppvObj)
{
	METHOD_PROLOGUE(CVWViewCtrl, ObjectSite)
    return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CVWViewCtrl::XObjectSite::OnInvokeMethod(IThing* pthing, BSTR bstrName, DISPPARAMS* pdispparams)
{
	METHOD_PROLOGUE(CVWViewCtrl, ObjectSite)
	return pThis->HandleOnInvokeMethod(pthing, bstrName, pdispparams);
}

STDMETHODIMP CVWViewCtrl::XObjectSite::OnPropertyChanged(IThing *pthing, BSTR bstrName, long lHint, VARIANT varHintData)
{
	METHOD_PROLOGUE(CVWViewCtrl, ObjectSite)
	return pThis->HandleOnPropertyChanged(pthing, bstrName, lHint, varHintData);
}

STDMETHODIMP CVWViewCtrl::XObjectSite::OnMethodChanged(IThing *pthing, BSTR bstrName, long lHint, VARIANT varHintData)
{
	METHOD_PROLOGUE(CVWViewCtrl, ObjectSite)
	return pThis->HandleOnMethodChanged(pthing, bstrName, lHint, varHintData);
}

STDMETHODIMP CVWViewCtrl::XObjectSite::OnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg)
{
	METHOD_PROLOGUE(CVWViewCtrl, ObjectSite)
	return pThis->HandleOnUIEvent(pthing, bstrEventName, varArg, FALSE);
}

STDMETHODIMP CVWViewCtrl::XObjectSite::OnTrace(BSTR bstr)
{
	METHOD_PROLOGUE(CVWViewCtrl, ObjectSite)
	return pThis->HandleOnTrace(bstr, FALSE);
}

STDMETHODIMP CVWViewCtrl::XObjectSite::OnReport(BSTR bstr, long lType)
{
	METHOD_PROLOGUE(CVWViewCtrl, ObjectSite)
	return pThis->HandleOnReport(bstr, lType, FALSE);
}

STDMETHODIMP CVWViewCtrl::XObjectSite::OnModuleChanged(BSTR bstrName, long lHint, VARIANT varHintData)
{
	METHOD_PROLOGUE(CVWViewCtrl, ObjectSite)
	return pThis->HandleOnModuleChanged(bstrName, lHint, varHintData);
}

STDMETHODIMP CVWViewCtrl::XObjectSite::OnUserConnect(IThing* pthing)
{
	METHOD_PROLOGUE(CVWViewCtrl, ObjectSite)
	return pThis->HandleOnUserConnect(pthing, FALSE);
}

STDMETHODIMP CVWViewCtrl::XObjectSite::OnUserDisconnect(IThing* pthing)
{
	METHOD_PROLOGUE(CVWViewCtrl, ObjectSite)
	return pThis->HandleOnUserDisconnect(pthing, FALSE);
}

STDMETHODIMP CVWViewCtrl::XObjectSite::OnUserReconnect(BSTR bstrURL, BSTR bstrName, BSTR bstrPassword)
{
	METHOD_PROLOGUE(CVWViewCtrl, ObjectSite)
	return pThis->HandleOnUserReconnect(bstrURL,bstrName,bstrPassword, FALSE);
}

// Client events implementation
STDMETHODIMP_(ULONG) CVWViewCtrl::XClientSite::AddRef()
{
	METHOD_PROLOGUE(CVWViewCtrl, ClientSite)
	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CVWViewCtrl::XClientSite::Release()
{
	METHOD_PROLOGUE(CVWViewCtrl, ClientSite)
	return pThis->ExternalRelease();
}

STDMETHODIMP CVWViewCtrl::XClientSite::QueryInterface(
    REFIID iid, void FAR* FAR* ppvObj)
{
	METHOD_PROLOGUE(CVWViewCtrl, ClientSite)
    return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CVWViewCtrl::XClientSite::OnConnect(IWorld* pWorld)
{
	METHOD_PROLOGUE(CVWViewCtrl, ClientSite)
	return pThis->OnConnect(pWorld);
}

STDMETHODIMP CVWViewCtrl::XClientSite::OnDisconnect(IWorld* pWorld)
{
	METHOD_PROLOGUE(CVWViewCtrl, ClientSite)
	return pThis->OnDisconnect(pWorld);
}

STDMETHODIMP CVWViewCtrl::XClientSite::OnUserReconnect(IVWClient* newClient,
			IWorld* pWorld,
			IThing* pthing)
{
	METHOD_PROLOGUE(CVWViewCtrl, ClientSite)
	if (!pThis->SyncNew(newClient)) return E_FAIL;
	return pThis->HandleOnUserReconnect(newClient,pWorld,pthing);
}

STDMETHODIMP CVWViewCtrl::XClientSite::OnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg)
{
	METHOD_PROLOGUE(CVWViewCtrl, ClientSite)
	return pThis->HandleOnUIEvent(pthing, bstrEventName, varArg, TRUE);
}

STDMETHODIMP CVWViewCtrl::XClientSite::OnTrace(BSTR bstr)
{
	METHOD_PROLOGUE(CVWViewCtrl, ClientSite)
	return pThis->HandleOnTrace(bstr, TRUE);
}

STDMETHODIMP CVWViewCtrl::XClientSite::OnReport(BSTR bstr, long lType)
{
	METHOD_PROLOGUE(CVWViewCtrl, ClientSite)
	return pThis->HandleOnReport(bstr, lType, TRUE);
}

STDMETHODIMP CVWViewCtrl::XClientSite::OnUserConnect(IThing* pthing)
{
	METHOD_PROLOGUE(CVWViewCtrl, ClientSite)
	return pThis->HandleOnUserConnect(pthing, TRUE);
}

STDMETHODIMP CVWViewCtrl::XClientSite::OnUserDisconnect(IThing* pthing)
{
	METHOD_PROLOGUE(CVWViewCtrl, ClientSite)
	return pThis->HandleOnUserDisconnect(pthing, TRUE);
}

STDMETHODIMP_(ULONG) CVWViewCtrl::XObjectSafety::AddRef()
{
	METHOD_PROLOGUE(CVWViewCtrl, ObjectSafety)
	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CVWViewCtrl::XObjectSafety::Release()
{
	METHOD_PROLOGUE(CVWViewCtrl, ObjectSafety)
	return pThis->ExternalRelease();
}

STDMETHODIMP CVWViewCtrl::XObjectSafety::QueryInterface(
    REFIID iid, void FAR* FAR* ppvObj)
{
	METHOD_PROLOGUE(CVWViewCtrl, ObjectSafety)
    return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CVWViewCtrl::XObjectSafety::GetInterfaceSafetyOptions(REFIID riid, DWORD *pdwSupportedOptions, DWORD *pdwEnabledOptions)
{
	METHOD_PROLOGUE(CVWViewCtrl, ObjectSafety)
	if (pdwSupportedOptions == NULL || pdwEnabledOptions == NULL)
		return E_POINTER;

	HRESULT hr = S_OK;
	if (riid == IID_IDispatch)
	{
		*pdwSupportedOptions = INTERFACESAFE_FOR_UNTRUSTED_CALLER;
		*pdwEnabledOptions = pThis->m_dwSafety & INTERFACESAFE_FOR_UNTRUSTED_CALLER;
	}
	else
	{
		*pdwSupportedOptions = 0;
		*pdwEnabledOptions = 0;
		hr = E_NOINTERFACE;
	}

	return hr;
}

STDMETHODIMP CVWViewCtrl::XObjectSafety::SetInterfaceSafetyOptions(REFIID riid, DWORD dwOptionSetMask, DWORD dwEnabledOptions)
{
	METHOD_PROLOGUE(CVWViewCtrl, ObjectSafety)

	// If we're being asked to set our safe for scripting option then oblige
	if (riid == IID_IDispatch)
	{
		// Store our current safety level to return in GetInterfaceSafetyOptions
		pThis->m_dwSafety = dwEnabledOptions & dwOptionSetMask;
		return S_OK;
	}

	return E_NOINTERFACE;
}

HRESULT CVWViewCtrl::Advise(IUnknown *pUnk, REFIID riid, ULONG *pdwCookie)
{
	HRESULT hr = E_FAIL;
	IConnectionPoint* pconnpt = NULL;
	IConnectionPointContainer* pconnptctr = NULL;
	
	if (!pUnk) goto errout;

	hr = pUnk->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pconnptctr);
	if (SUCCEEDED(hr) && pconnptctr)
	{
		hr = pconnptctr->FindConnectionPoint(riid, &pconnpt);
		
		if (SUCCEEDED(hr) && pconnpt)
		{
			hr = pconnpt->Advise(GetIDispatch(FALSE), pdwCookie);
			SAFERELEASE(pconnpt);
		}

		SAFERELEASE(pconnptctr);
	}

errout:
#ifdef _DEBUG
	if (FAILED(hr))
		TRACE("CVWViewCtrl::Advise: failed to advise, hr %x, pUnk %x, dwCookie %x\n", hr, pUnk, *pdwCookie);
#endif
	return hr;
}

HRESULT CVWViewCtrl::UnAdvise(IUnknown *pUnk, REFIID riid, ULONG dwCookie)
{
	HRESULT hr = E_FAIL;
	IConnectionPoint* pconnpt = NULL;
	IConnectionPointContainer* pconnptctr = NULL;
	
	if (!pUnk) goto errout;

	hr = pUnk->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pconnptctr);
	if (SUCCEEDED(hr) && pconnptctr)
	{
		hr = pconnptctr->FindConnectionPoint(riid, &pconnpt);
	
		if (SUCCEEDED(hr) && pconnpt)
		{
			hr = pconnpt->Unadvise(dwCookie);
			SAFERELEASE(pconnpt);
		}

		SAFERELEASE(pconnptctr);
	}

errout:
#ifdef _DEBUG
	if (FAILED(hr))
		TRACE("CVWViewCtrl::UnAdvise: failed to unadvise, hr %x, pUnk %x, dwCookie %x\n", hr, pUnk, dwCookie);
#endif
	return hr;
}

/////////////////////////////////////////////////////////////////////////////
// CVWViewCtrl::CVWViewCtrl - Constructor

CVWViewCtrl::CVWViewCtrl()
{
#ifdef _VWSTANDALONE_CLASS_
	InitializeIIDs(&IID_DVWView, &IID_DVWViewEvents);
#endif 
	EnableAutomation();

	// TODO: Initialize your control's instance data here.
	m_pVWClient = NULL;
	m_pWorld = NULL;
	m_ulFlags = ADVISE_CLIENT;
	m_dwWorldCookie = 0;
	m_dwClientCookie = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CVWViewCtrl::~CVWViewCtrl - Destructor

CVWViewCtrl::~CVWViewCtrl()
{
	// TODO: Cleanup your control's instance data here.
	ReleaseAllObjects();
}

/////////////////////////////////////////////////////////////////////////////
// CVWViewCtrl::OnDraw - Drawing function

void CVWViewCtrl::OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
}

/////////////////////////////////////////////////////////////////////////////
// CVWViewCtrl::DoPropExchange - Persistence support

void CVWViewCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}

/////////////////////////////////////////////////////////////////////////////
// CVWViewCtrl::GetControlFlags -
// Flags to customize MFC's implementation of ActiveX controls.
//
// For information on using these flags, please see MFC technical note
// #nnn, "Optimizing an ActiveX Control".
DWORD CVWViewCtrl::GetControlFlags()
{
	DWORD dwFlags = COleControl::GetControlFlags();

	// The control's output is not being clipped.
	// The control guarantees that it will not paint outside its
	// client rectangle.
	dwFlags &= ~clipPaintDC;

	// The control will not be redrawn when making the transition
	// between the active and inactivate state.
	dwFlags |= noFlickerActivate;

	// The control can receive mouse notifications when inactive.
	// TODO: if you write handlers for WM_SETCURSOR and WM_MOUSEMOVE,
	//		avoid using the m_hWnd member variable without first
	//		checking that its value is non-NULL.
	dwFlags |= pointerInactive;

	// The control can optimize its OnDraw method, by not restoring
	// the original GDI objects in the device context.
	dwFlags |= canOptimizeDraw;

	return dwFlags;
}

/////////////////////////////////////////////////////////////////////////////
// CVWViewCtrl::OnResetState - Reset control to default state

void CVWViewCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}

/////////////////////////////////////////////////////////////////////////////
// CVWViewCtrl::AboutBox - Display an "About" box to the user

void CVWViewCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_VWVIEW);
	dlgAbout.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CVWViewCtrl message handlers

LPDISPATCH CVWViewCtrl::GetVWClient() 
{
	// TODO: Add your property handler here

	SAFEADDREF(m_pVWClient);
	return m_pVWClient;
}

void CVWViewCtrl::SetVWClient(LPDISPATCH newValue) 
{
	IThing *pConnectedUser = NULL;

	if (!newValue)
	{
		if (m_pWorld)
		{
			if (FAILED(m_pWorld->get_User(&pConnectedUser))) 
				goto ERROR_ENCOUNTERED;
		}

		if (pConnectedUser) 
		{
			if (FAILED(HandleOnUserDisconnect(pConnectedUser, TRUE))) 
				goto ERROR_ENCOUNTERED;
		}

		if (m_pWorld)
		{
			if (FAILED(HandleOnDisconnect(m_pWorld))) 
				goto ERROR_ENCOUNTERED;

			SAFERELEASE(pConnectedUser);
		}
	}
	
	if (!SyncNew(newValue)) 
		goto ERROR_ENCOUNTERED;

	if (FAILED(HandleOnConnect(m_pWorld))) 
		goto ERROR_ENCOUNTERED;

	if (FAILED(m_pWorld->get_User(&pConnectedUser))) 
		goto ERROR_ENCOUNTERED;

	if (pConnectedUser)
	{
		if (FAILED(HandleOnUserConnect(pConnectedUser, TRUE))) 
			goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pConnectedUser);
}

BOOL CVWViewCtrl::SyncNew(LPDISPATCH newValue) 
{
	SetModifiedFlag();
	ReleaseAllObjects();
	if (!newValue) return FALSE;

	if (FAILED(newValue->QueryInterface(IID_IVWClient, (void**)&m_pVWClient)) || !m_pVWClient)
		return FALSE;

	if (m_ulFlags & ADVISE_CLIENT)
	{
		HRESULT hr = S_OK;

		hr = Advise(m_pVWClient, IID_IVWClientSite, &m_dwClientCookie);
		if (SUCCEEDED(hr))
		{
			TRACE("CVWViewCtrl::SyncNew: Advise, client %x, dwClientCookie %x\n", m_pVWClient, m_dwClientCookie);
		}
		else
		{
			TRACE("CVWViewCtrl::SyncNew: failed during Advise, client, hr %x\n", hr);

			SAFERELEASE(m_pVWClient);

			return FALSE;
		}
	}

	// test to see if there is a world to Advise
	if (FAILED(m_pVWClient->get_World(&m_pWorld)) || !m_pWorld) 
		return FALSE;

	if (m_ulFlags & ADVISE_WORLD)
	{
		HRESULT hr = S_OK;

		hr = Advise(m_pWorld, IID_IVWObjectSite, &m_dwWorldCookie);
		if (SUCCEEDED(hr))
		{
			TRACE("CVWViewCtrl::SyncNew: Advise, world %x, dwWorldCookie %x\n", m_pWorld, m_dwWorldCookie);
		}
		else
		{
			TRACE("CVWViewCtrl::SyncNew: failed during Advise, world, hr %x\n", hr);

			SAFERELEASE(m_pWorld);

			return FALSE;
		}
	}

	return TRUE;
}

HRESULT CVWViewCtrl::OnConnect(LPDISPATCH pWorld) 
{
	// TODO: Add your dispatch handler code here
	ASSERT(pWorld);

	ASSERT(m_pWorld == NULL);

	if (pWorld->QueryInterface(IID_IWorld, (void **)&m_pWorld) != S_OK)
		return S_OK;
	
	if (m_ulFlags & ADVISE_WORLD) 
	{
		HRESULT hr = S_OK;

		hr = Advise(m_pWorld, IID_IVWObjectSite, &m_dwWorldCookie);
		if (SUCCEEDED(hr))
		{
			TRACE("CVWViewCtrl::OnConnect: Advise, world %x, dwWorldCookie %x\n", m_pWorld, m_dwWorldCookie);
		}
		else
		{
			TRACE("CVWViewCtrl::OnConnect: failed during Advise, world, hr %x\n", hr);

			SAFERELEASE(m_pWorld);

			return hr;
		}
	}

	return HandleOnConnect(pWorld);
}

HRESULT CVWViewCtrl::OnDisconnect(LPDISPATCH pWorld) 
{
	if (!pWorld || !m_pWorld || m_pWorld != pWorld)
		return S_OK;

	ASSERT(pWorld);

	if (m_pWorld)
	{
		HRESULT hr = S_OK;

		if (m_ulFlags & ADVISE_WORLD)
		{
			hr = UnAdvise(m_pWorld, IID_IVWObjectSite, m_dwWorldCookie);
			if (SUCCEEDED(hr))
			{
				TRACE("CVWViewCtrl::OnDisconnect: UnAdvise, world %x, dwWorldCookie %x\n", m_pWorld, m_dwWorldCookie);
			}
			else
			{
				TRACE("CVWViewCtrl::OnDisconnect: failed during UnAdvise, world, hr %x\n", hr);
			}
	
			m_dwWorldCookie = 0;
		}

		SAFERELEASE(m_pWorld);
	}

	return HandleOnDisconnect(pWorld);
}

#if 0
HRESULT CVWViewCtrl::OnPreDestroy() 
{
	// TODO: Add your dispatch handler code here
	SetVWClient(NULL);
	return HandleOnPreDestroy();
}
#endif

void CVWViewCtrl::ReleaseAllObjects()
{
	HRESULT hr = S_OK;

	if (m_pWorld)
	{
		IThing* pUser = NULL;
		HRESULT hr;
		hr = m_pWorld->get_User(&pUser);
		if (SUCCEEDED(hr))
			RemoveFromUser(pUser);
		SAFERELEASE(pUser);
	}

	if (m_pWorld)
	{
		if (m_ulFlags & ADVISE_WORLD)
		{
			hr = UnAdvise(m_pWorld, IID_IVWObjectSite, m_dwWorldCookie);
			if (SUCCEEDED(hr))
			{
				TRACE("CVWViewCtrl::ReleaseAllObjects: UnAdvise, world %x, dwWorldCookie %x\n", m_pWorld, m_dwWorldCookie);
			}
			else
			{
				TRACE("CVWViewCtrl::ReleaseAllObjects: failed during UnAdvise, world, hr %x\n", hr);
			}

			m_dwWorldCookie = 0;
		}

		SAFERELEASE(m_pWorld);
	}

	if (m_pVWClient)
	{
		if (m_ulFlags & ADVISE_CLIENT)
		{
			hr = UnAdvise(m_pVWClient, IID_IVWClientSite, m_dwClientCookie);
			if (SUCCEEDED(hr))
			{
				TRACE("CVWViewCtrl::ReleaseAllObjects: UnAdvise, client %x, dwClientCookie %x\n", m_pVWClient, m_dwClientCookie);
			}
			else
			{
				TRACE("CVWViewCtrl::ReleaseAllObjects: failed during UnAdvise, world, hr %x\n", hr);
			}
	
			m_dwClientCookie = 0;
		}

		SAFERELEASE(m_pVWClient);
	}
}

void CVWViewCtrl::OnDestroy() 
{
	COleControl::OnDestroy();
	
	SetVWClient(NULL);
}

void CVWViewCtrl::OnClose(DWORD dwSaveOption) 
{
	COleControl::OnClose(dwSaveOption);
	
	SetVWClient(NULL);
}

void CVWViewCtrl::OnSetClientSite()
{
	COleControl::OnSetClientSite();
	if (GetClientSite() == NULL)
	{
		SetVWClient(NULL);
	}
}

// REVIEW: This function is only overridden due to an MFC bug in COleControl - Q167697.
HRESULT CVWViewCtrl::OnActivateInPlace(BOOL bUIActivate, LPMSG pMsg)
{
  static BOOL bInsideFunc = FALSE;
  if (!bInsideFunc)
  {
     bInsideFunc = TRUE;
     HRESULT hr = COleControl::OnActivateInPlace(bUIActivate, pMsg);
     bInsideFunc = FALSE;
     return hr;
  }
  return S_OK;
}



HRESULT CVWViewCtrl::AddToUser(IThing* pUser)
{
typedef CVWComPtr<IPropertyMap, NULL,&IID_IPropertyMap> CPropertyMapPtr;
 
	CPropertyMapPtr MapPtr;
	HRESULT hr = S_OK;
	CComVariant var;
	IDispatch *pDisp = NULL;
	IOCXProxy* pOCXProxy = NULL;
	CString strID(AmbientDisplayName());

	if(pUser == NULL)
		goto ERROR_ENCOUNTERED;

	// Get a decent name
	if (strID.GetLength() == 0)
	{

		LPOLESTR lpszProgID = NULL;
		CLSID clsid;
		int index;

		hr = GetClassID(&clsid);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = ::ProgIDFromCLSID(clsid, &lpszProgID);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		strID = lpszProgID;
		//SAFEFREESTRING(lpszProgID);
		{
			IMalloc *pMalloc = NULL;
			hr =  ::CoGetMalloc(1, &pMalloc); 
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
			pMalloc->Free(lpszProgID);
			SAFERELEASE(pMalloc);
		}
		index = strID.Find('.');
		strID = strID.Right(strID.GetLength() - index - 1);
		index = strID.Find('.');
		strID = strID.Left(index);
	}

	// get the property map
	hr = pUser->get_Property(CComBSTR("OCXMAP"), &var);
	if (FAILED(hr))
	{
		// we failed, create an OCXMAP
		hr = CreatePropertyMap(m_pWorld,  &MapPtr.p);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	
		hr = pUser->AddPropertyExt(CComBSTR("OCXMAP"), CComVariant(MapPtr),
			PSBIT_SYSTEMPROPERTY | PSBIT_HIDDEN | PSBIT_NOTREMOTED | PSBIT_NOTLOGGED, PS_SYSTEMPROPERTY, VT_DISPATCH, IID_IPropertyMap, NULL);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}
	else
	{
		MapPtr = var;
		if (MapPtr == NULL)
			goto ERROR_ENCOUNTERED;
	}


	pDisp = GetIDispatch(TRUE);
	/*	pDisp = (IDispatch *) GetInterface(m_piidPrimary);
	pDisp->AddRef();
	
	hr = ExternalQueryInterface((void*)m_piidPrimary,(void **)&pDisp);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	{
	IUnknown * punk = GetInterface(&IID_IUnknown);
	ASSERT (punk);
	if (FAILED(punk->QueryInterface(IID_IDispatch, (void **)&pDisp)))
		return NULL;

	}

	pDisp = (IDispatch *) &m_xDispatch;
	pDisp->AddRef();
*/	
	hr = CreateOCXProxy(m_pWorld, pDisp, &pOCXProxy);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pOCXProxy->put_PropertyName(CComBSTR(strID));
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = MapPtr->put_Property(CComBSTR(strID), CComVariant(pOCXProxy));

ERROR_ENCOUNTERED:
	SAFERELEASE(pDisp);
	SAFERELEASE(pOCXProxy);
return hr; 
}

HRESULT CVWViewCtrl::RemoveFromUser(IThing* pUser)
{
typedef CVWComPtr<IPropertyMap, NULL,&IID_IPropertyMap> CPropertyMapPtr;
typedef CVWComPtr<IOCXProxy,NULL,&IID_IOCXProxy> COCXMapPtr;

	CPropertyMapPtr MapPtr;
	COCXMapPtr OCXProxyPtr;

	HRESULT hr = S_OK;
	CComVariant var;
	CString strID(AmbientDisplayName());

	if(pUser == NULL)
		goto ERROR_ENCOUNTERED;

	//Get a name
	if (strID.GetLength() == 0)
	{
		CLSID clsid;
		LPOLESTR lpszProgID = NULL;
		int index;
		hr = GetClassID(&clsid);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = ::ProgIDFromCLSID(clsid, &lpszProgID);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		strID = lpszProgID;
		//SAFEFREESTRING(lpszProgID);
		{
			IMalloc *pMalloc = NULL;
			hr =  ::CoGetMalloc(1, &pMalloc); 
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
			pMalloc->Free(lpszProgID);
			SAFERELEASE(pMalloc);
		}
		index = strID.Find('.');
		strID = strID.Right(strID.GetLength() - index - 1);
		index = strID.Find('.');
		strID = strID.Left(index);
	}

	// Get the propertymap
	hr = pUser->get_Property(CComBSTR("OCXMAP"), &var);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	MapPtr = var;
	if (MapPtr == NULL)
		goto ERROR_ENCOUNTERED;

	var.Clear();

	//explicity clear circular references within the OCXProxy!
	//It does do it itself...
	hr = MapPtr->get_Property(CComBSTR(strID), &var);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	OCXProxyPtr = var;
	if (OCXProxyPtr != NULL)
	{
		OCXProxyPtr->put_OCX(NULL);
	}

	hr = MapPtr->Remove(CComBSTR(strID));

ERROR_ENCOUNTERED:
	return hr; 
}

HRESULT CVWViewCtrl::HandleOnUserConnect(IThing* pthing, BOOL bFromClient)
{
/*	IThing* pUser = NULL;

	if (m_pWorld)
	{
		m_pWorld->get_User(&pUser);

		if (pUser)
		{
			if (pthing == pUser)
				AddToUser(pthing);

			SAFERELEASE(pUser);
		}
	}
*/
	return S_OK;
}

HRESULT CVWViewCtrl::HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient)
{
	IThing* pUser = NULL;

	if (m_pWorld)
	{
		m_pWorld->get_User(&pUser);

		if (pUser)
		{
			if (pthing == pUser)
				RemoveFromUser(pthing);

			SAFERELEASE(pUser);
		}
	}

	return S_OK;
}
