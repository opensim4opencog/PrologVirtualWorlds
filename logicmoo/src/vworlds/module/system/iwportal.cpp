// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// IwPortal.cpp : Implementation of CIWPortal - InterWorld Portal
#include "stdafx.h"
#include <vwsysex.h>
#include "IWportal.h"
#include "syshelp.h"
#include "vwutils.h"
#include "..\..\vwobject\propsecu.h"
#include "vwevents.h"
#include <time.h>
#include <stdlib.h>

#define PASSWORDIMPLEMENTED

/////////////////////////////////////////////////////////////////////////////
// CIWPortal

//******************************************************
// Install
// Installs this module - creates the exemplar and
// adds the properties and methods.

STDMETHODIMP CIWPortalExemplarObject::Install(IModule* pModule)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pIWPortalEx;
    HRESULT         hr = S_OK;

	// Create the portal exemplar
	if (FAILED(hr = m_pWorld->CreateExemplar(CComBSTR("IWPortal"), CComVariant(CComBSTR("Portal")), &pIWPortalEx)))
	    goto ERROR_ENCOUNTERED;

//	if (FAILED(hr = pIWPortalEx->put_Description(CComBSTR("A means of transporting to other worlds."))))
//	    goto ERROR_ENCOUNTERED;

	// Add Transport method to overwrite portal method

	if (FAILED(hr = pIWPortalEx->CreateAndAddMethodExt(METHOD_CLIENT, pModule, CComBSTR("Transport"),
													CComBSTR("Transport"),
													PSBIT_EXEMPLARDEFAULTMETHOD, PS_EXEMPLARDEFAULTMETHOD)))
        goto ERROR_ENCOUNTERED;

    return S_OK;

ERROR_ENCOUNTERED:
	VWTRACE(m_pWorld, "VWPORTAL", VWT_ERROR, "CIWPortalExemplarObject::Install Error creating InterWorldPortal %x \n", hr);

	return hr;
}

//******************************************************
// Transport
// Overwrites portal exemplar Transport method 
// Connects transportee to new world on client-side

STDMETHODIMP CIWPortalExemplarObject::Transport(IThing *pThing, int iIndex)
{
	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    HRESULT                 hr = S_OK;
	CComPtr<IThing>			pThis;  //pThis is the source portal
	CComPtr<IThing>	pGlobal;
	CComPtr<IPropertyList>	pDestinations;
	CComBSTR				bstrDestWorld;
	long					count = 0;

    if (pThing == NULL)
    {
        VWTRACE(m_pWorld, "VWIWPORTAL", VWT_ERROR, "CIWPortalExemplarObject::Transport: NULL pointer\n");
        return ReportIWPortalError(E_POINTER);
    }

    if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	if (FAILED(hr = pThis->get_ObjectProperty(CComBSTR("Destinations"), (IObjectProperty **)&pDestinations)))
	    return hr;

	if (pDestinations)
	{
		if (FAILED(hr = pDestinations->get_Count(&count)))
	        return hr;
	}

	if (count)
	{
        // make sure the index is within range
        if (iIndex>=count)
        {
            VWTRACE(m_pWorld, "VWIWPORTAL", VWT_ERROR, "CIWPortalExemplarObject::Index out of range\n");
            return ReportIWPortalError(E_INVALIDARG);
        }

		if (FAILED(hr = pDestinations->get_String(iIndex,&bstrDestWorld.m_str)))
	        return hr;

		if (bstrDestWorld.Length() > 0)
		{
			CComPtr<IThing> pUser;
			CComBSTR bstrName;
			CComBSTR bstrPassword;

			if (FAILED(hr = m_pWorld->get_User(&pUser)))
	            return hr;

			// do a "Reconnect"
			// only the user entering the portal gets reconnected, NOT all clients
			if (pUser == pThing)
			{
				CComVariant	varKeyVal;
				// get the user name and password
				// DV:  Note that we need to get the preferred name from the
				// profile, not the decorated name.
				CComPtr<IAvatarProfile>	pProfile;
				static CComBSTR	kbstrProfile("Profile");
				static CComBSTR	kbstrPrefName("PreferredName");

				if (FAILED(hr = pUser->get_ObjectProperty(kbstrProfile, (IObjectProperty **)&pProfile)))
                    return hr;

				if (FAILED(hr = pProfile->get_KeyValue(kbstrPrefName, &varKeyVal)))
					return hr;
				bstrName = varKeyVal.bstrVal;

				ASSERT(bstrName);

#ifdef PASSWORDIMPLEMENTED
				{
					// IWPortal needs to override security to grab user's password
					CSetUID SetUID(m_pWorld, pUser, pUser, VARIANT_TRUE);

					hr = pUser->get_String(CComBSTR("Password"), &bstrPassword.m_str);
				}

				if (FAILED(hr))
                    return hr;

				ASSERT(bstrPassword);
#else
				bstrPassword = "";
#endif

				// Since this isn't a synchronous call, if it fails,
				// we fire a UI event for the failure later, in VWClientObject:OnConnect
				if (FAILED(hr = m_pWorld->ReconnectExt(bstrDestWorld, bstrName, bstrPassword)))
				{
                    return ReportIWPortalError(VWIWPORTAL_E_RECONNECTFAILED);
				}
			}
		}
	}

	// If no destination set, send a message to the user informing them that this portal has no destinations (handled in text history)
	if ((count==0) || (bstrDestWorld.Length() == 0))
	{
		static CComBSTR kbstrEvent = VW_PORTALDESTINATIONNOTSET_EVENT_STR;

		if (SUCCEEDED(m_pWorld->get_Global(&pGlobal)))
		{
			pGlobal->InvokeMethodExt(CComBSTR("FireRemoteUIEvent"),  
									  CComDISPPARAMS(4, CComVariant((IDispatch *) pThing), 
									  CComVariant((IDispatch *) pThis), 
									  CComVariant(kbstrEvent), 
									  CComVariant()), NULL);
		}
		return ReportIWPortalError(VWIWPORTAL_E_NODESTINATION);
	}

	return hr;
}