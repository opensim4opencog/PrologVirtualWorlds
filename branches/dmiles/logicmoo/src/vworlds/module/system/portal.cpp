// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// Portal.cpp : Implementation of CPortal
#include "stdafx.h"
#include <vwsysex.h>
#include "portal.h"
#include "syshelp.h"
#include "vwutils.h"
#include "propbase.h" 
#include <resource.h>
#include <time.h>
#include <stdlib.h>
#include <vwevents.h>

//////////////////////////////////////////////////////////
// CPortal

//******************************************************
// Install
// Installs this module - creates the exemplar and
// adds the properties and methods.

STDMETHODIMP CPortalExemplarObject::Install(IModule* pModule)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CMethodInfo rgMethod[] =
	{
		{ METHOD_CLIENT,				"ShowTransportDialog",	NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,				"TransportMenu",		NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER | METHOD_SYNC,	"Transport",			NULL, PSBIT_EXEMPLARDEFAULTMETHOD,					PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER,				"TransportTo",			NULL, PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,	PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER,				"AllowTransport",		NULL, PSBIT_EXEMPLARDEFAULTMETHOD,					PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER,				"SetDestinationByID",	NULL, PSBIT_EXEMPLARDEFAULTMETHOD,					PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER,				"AcceptTransport",		NULL, PSBIT_EXEMPLARDEFAULTMETHOD,					PS_ALLEXECUTEMETHOD }

	};
	int cMethod = sizeof(rgMethod)/sizeof(CMethodInfo);

	CComPtr<IThing>			pPortalEx;
	CComPtr<IThing>			pThingEx;
	CComPtr<IPropertyList>	pDestinations;
	CComPtr<IPropertyList>	pDescriptions;
	CComPtr<IPropertyList>	pMenu;
	CComPtr<IPropertyList>	pThingExMenu;
	CComPtr<IMenuItem>		pMenuItem;
	CComBSTR				bstrName;
	CComBSTR				bstrNameInternal;
    int                     iMethod;
    HRESULT                 hr = S_OK;

	// Create the portal exemplar
	if (FAILED(hr = m_pWorld->CreateExemplar(CComBSTR("Portal"), CComVariant(CComBSTR("Thing")), &pPortalEx)))
        goto ERROR_ENCOUNTERED;

    // Property  HasPortalDialogURL
	if (FAILED(hr = pPortalEx->AddPropertyExt(	CComBSTR("HasPortalDialogURL"), 
											CComVariant((bool)VARIANT_TRUE), 
											PSBIT_EXEMPLARDEFAULTPROPERTY, 
											PS_EXEMPLARDEFAULTPROPERTY,	
											VT_BOOL, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	// List of destinations (portals)
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pDestinations)))
	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pPortalEx->AddPropertyExt(	CComBSTR("Destinations"), 
											CComVariant(pDestinations), 
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_EXEMPLARDEFAULTCOLLECTION, 
											VT_DISPATCH, IID_IPropertyList, NULL)))
	    goto ERROR_ENCOUNTERED;

	// List of descriptions (strings)
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pDescriptions)))
	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pPortalEx->AddPropertyExt(	CComBSTR("Descriptions"), 
											CComVariant(pDescriptions), 
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_EXEMPLARDEFAULTCOLLECTION, 
											VT_DISPATCH, IID_IPropertyList, NULL)))
	    goto ERROR_ENCOUNTERED;

	// Add portal methods
	for (iMethod=0; iMethod<cMethod; iMethod++)
	{
		bstrName = rgMethod[iMethod].pszName;

		if (rgMethod[iMethod].pszNameInternal == NULL)
			bstrNameInternal = bstrName;
		else
			bstrNameInternal = rgMethod[iMethod].pszNameInternal;
		
		if (FAILED(hr = pPortalEx->CreateAndAddMethodExt(rgMethod[iMethod].lFlags,
													pModule,
													bstrNameInternal,
													bstrName,
													rgMethod[iMethod].psbits,
													rgMethod[iMethod].permissions)))
    	    goto ERROR_ENCOUNTERED;
	}

	// Menus

	// Create the menu
	if (FAILED(hr = CreateMenu(m_pWorld, &pMenu)))
	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pPortalEx->get_ObjectProperty(CComBSTR("Menu"), (IObjectProperty **) &pThingExMenu)))
	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThingExMenu->CopyTo(pMenu)))
	    goto ERROR_ENCOUNTERED;

	// Add portal menu items below that
	if (FAILED(hr = CreateMenuItemExt(m_pWorld, CComBSTR("Go Through"), 0, pPortalEx, NULL, CComBSTR("TransportMenu"), NULL, &pMenuItem)))
	    goto ERROR_ENCOUNTERED;
//		if (FAILED(hr = pMenuItem->put_SortOrder(MI_SORT_PRIMARY+100)))
//	        goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pMenu->AddObjectProperty(pMenuItem)))
    	goto ERROR_ENCOUNTERED;

	// do this set at the end so that property remotes correctly...
	if (FAILED(hr = pPortalEx->put_ObjectProperty(CComBSTR("Menu"), pMenu)))
	    goto ERROR_ENCOUNTERED;

    return S_OK;

ERROR_ENCOUNTERED:
    
    VWTRACE(m_pWorld, "VWPORTAL", VWT_ERROR, "CPortalExemplarObject::Install: Error creating Portal %x\n", hr);

	return hr;
}

//******************************************************
// TransportMenu
// Handles user right-click on "Go Through"
// Takes the user through the portal's first
// destination...

STDMETHODIMP CPortalExemplarObject::TransportMenu()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pUser;
	CComPtr<IThing> pThis;
    HRESULT         hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

	if (FAILED(hr = m_pWorld->get_User(&pUser)))
	    return hr;

	if (pUser && pThis)
	{
		hr = pThis->InvokeMethodExt(CComBSTR("Transport"),
							CComDISPPARAMS(2, CComVariant((IDispatch*) pUser), CComVariant(0L)),
							NULL);
		if (FAILED(hr))
			VWREPORT(m_pWorld, VWR_ERROR, "Portal transport failed; hr %x\n", hr);
	}
	
	return hr;
}

//******************************************************
// ShowTransportDialog
// Brings up a dialog, if the portal has an URL,
// otherwise transports the user through the portal
// (currently used in mm OnIntersectee)

STDMETHODIMP CPortalExemplarObject::ShowTransportDialog()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr = S_OK;
	CComPtr<IThing> pUser;
	CComPtr<IThing> pThis;
	CComPtr<IPropertyList> pDestinations;
	VARIANT_BOOL	bHasPortalDialogURL = VARIANT_FALSE;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;
	if (FAILED(hr = m_pWorld->get_User(&pUser)))
	    return hr;
//	if (FAILED(hr = pThis->get_Name( &(bstrName.m_str) )))
//	    return hr;

	if (pUser && pThis)
	{
		if (FAILED(hr = pThis->get_ObjectProperty(CComBSTR("Destinations"), (IObjectProperty **)&pDestinations)))
	        return hr;
	
		// Check to see if there are any portals in the  destinations list
		if (pDestinations)
		{
			long	count;

			if (FAILED(hr = pDestinations->get_Count(&count)))
	            return hr;
			if (count)
			{			
				// Open  URL if user has

				if (FAILED(hr = pThis->get_BOOL(CComBSTR("HasPortalDialogURL"), &bHasPortalDialogURL)))
	                return hr;

				if (bHasPortalDialogURL)
				{
					if (FAILED(hr = m_pWorld->FireUIEvent(pThis, CComBSTR(VW_SHOWTRANSPORT_EVENT_STR), CComVariant(NULL))))
							return hr;
				}

				// No URL, just transport through the first portal on the list
				else
				{
					if (FAILED(hr = pThis->InvokeMethodExt(
											CComBSTR("Transport"),
											CComDISPPARAMS(2, CComVariant((IDispatch*) pUser), 
											CComVariant(0L)), NULL)))
                        return hr;
				}
			}
		}

		else
        {
			VWTRACE(m_pWorld, "VWPORTAL", VWT_ERROR, "CPortalExemplarObject::TransportDialog: No primary or secondary destinations \n");
			return ReportPortalError(VWPORTAL_E_NODESTINATIONS);
        }
	}

	return hr;
}

// ******************************************************
// Transport
// Moves from source portal to destination portal on server
// Uses index to determine which portal to use as destination:
// has 0 to n-1 destinations
// Checks AllowTransport and fires PortalLeave and Portal Arrive events

STDMETHODIMP CPortalExemplarObject::Transport(IThing * pThing, int iIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CComPtr<IThing> pThis;
	CComPtr<IThing> pDestPortal;
	CComPtr<IThing> pGlobal;
	CComPtr<IPropertyList> pDestinationList;
	CComVariant		varReturn;
	long			lCount = 0;
	HRESULT			hr = S_OK;
	HRESULT			transportHr = S_OK;  // keeps track of whether transport failed because of bad destination

	if (pThing==NULL)
    {
        VWTRACE(m_pWorld, "VWPORTAL", VWT_ERROR, "CPortalExemplarObject::Transport: NULL pointer\n");
		return ReportPortalError(E_POINTER);
    }

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

	if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
    	return hr;

	// Check if Transport is allowed
	if (FAILED(hr = pThis->InvokeMethodExt(CComBSTR("AllowTransport"), 
				CComDISPPARAMS(1, CComVariant((IDispatch*) pThing)), &varReturn)))
        return hr;

	if (varReturn.vt!=VT_BOOL)
    {
		VWREPORT(m_pWorld, VWR_ERROR, "CPortalExemplarObject::Transport AllowTransport returned invalid type\n");
        return ReportPortalError(VWPORTAL_E_UNKNOWN);
    }

	// if doesn't allow, exit with ok status
	if (varReturn.boolVal != VARIANT_TRUE)
		return S_OK;

	if (pThis)
	{
		// Get property list of portals
		if (FAILED(hr = pThis->get_ObjectProperty(CComBSTR("Destinations"), (IObjectProperty **)&pDestinationList)))
        	return hr;

		if (pDestinationList)
		{
			if (FAILED(hr = pDestinationList->get_Count(&lCount)))
				return hr;
		}

		if (lCount)
		{
			// Check that index is valid - between 0 and the number of portals
			// in the secondary destination list
			if ( (iIndex >= 0) && (iIndex <= (int) (lCount-1)))
            {
				VARIANT_BOOL bIsPortal;

				if (FAILED(hr = pDestinationList->get_Thing(iIndex, &pDestPortal)))
	            {
					// Check if we failed because the list didn't contain a thing
					// and return a more comprehensible error message
					if (hr == VWOBJECT_E_WRONGPROPERTYTYPE)
					{			
						VWTRACE(m_pWorld, "VWPORTAL", VWT_ERROR, "CPortalExemplarObject::Transport: Invalid destination \n");
						transportHr = VWPORTAL_E_INVALIDDESTINATION;
						goto ERROR_ENCOUNTERED;
					}
					else 
						return hr;
				}

				if (pDestPortal)
				{
					if (FAILED(hr = pDestPortal->IsOfType(CComBSTR("Portal"), &bIsPortal)))
						return hr;

					if (bIsPortal==VARIANT_FALSE)
					{
						VWTRACE(m_pWorld, "VWPORTAL", VWT_ERROR, "CPortalExemplarObject::Transport: Invalid destination \n");
						transportHr = VWPORTAL_E_INVALIDDESTINATION;
						goto ERROR_ENCOUNTERED;
					}
				}
            }
			else
			{
				VWTRACE(m_pWorld, "VWPORTAL", VWT_ERROR, "CPortalExemplarObject::Transport: Invalid index \n");
				transportHr = VWPORTAL_E_NOTAVALIDINDEX;
				goto ERROR_ENCOUNTERED;
			}
		}
		else
		{
			// Send a message to the user informing them that this portal has no destinations
			static CComBSTR kbstrEvent = VW_PORTALDESTINATIONNOTSET_EVENT_STR;

			if (pGlobal)
			{
				pGlobal->InvokeMethodExt(CComBSTR("FireRemoteUIEvent"),  
											  CComDISPPARAMS(4, CComVariant((IDispatch *) pThing), 
											  CComVariant((IDispatch *) pThis), 
											  CComVariant(kbstrEvent), 
											  CComVariant()), NULL);
			}
			VWTRACE(m_pWorld, "VWPORTAL", VWT_ERROR, "CPortalExemplarObject::Transport: No destinations \n");
			return ReportPortalError(VWPORTAL_E_NODESTINATIONS);
		}
	}

	//Make sure destination portal exists
	if (pDestPortal)
	{
		CComPtr<IPropertyList> pList;
		CComBSTR bstrDescription("You exit through {object}.|{subject} exits through {object}.");
		CComBSTR bstrName("PortalLeave");

		CComPtr<IPropertyList> pListArrive;
		CComBSTR bstrDescriptionArrive("You enter {object}.|{subject} enters {object}.");
		CComBSTR bstrNameArrive("PortalArrive");
		CComPtr<IThing> pDestRoom;

		
		// Fire portal leave event 
		if (FAILED(hr = pThis->FireEventExt(bstrName,
								CComDISPPARAMS(3, CComVariant((IDispatch*) pThing),
											   CComVariant((IDispatch*) pThis),
											   CComVariant((long) iIndex)),
								FIREEVENT_DEFAULT)))
	        return hr;

		// Create the Action event subject list
		if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
        {
            // Helper function has produced a trace, but we need errorinfo here
	        return ReportPortalError(hr);
        }
		if (FAILED(hr = pList->AddThing(pThing)))
	        return hr;
		if (FAILED(hr = pList->AddThing(pThis)))
	        return hr;

		// Fire the Action event
		if (FAILED(hr = pThis->FireEventExt(CComBSTR("Action"),
								CComDISPPARAMS(3, CComVariant(bstrName),
											   CComVariant(bstrDescription),
											   CComVariant((IDispatch*) pList)),
								FIREEVENT_DEFAULT)))
	        return hr;

		// Now call method to move (and in mm position) object correctly on destination portal
		if (FAILED(hr = pDestPortal->InvokeMethodExt(CComBSTR("TransportTo"),
									CComDISPPARAMS(1, CComVariant((IDispatch*) pThing)),
									NULL)))
		{
	        transportHr = hr;
			goto ERROR_ENCOUNTERED;
		}


		// Fire portal arrive event
		// First get destination room
		if (FAILED(hr = pDestPortal->get_Container(&pDestRoom)))
	        return hr;

		if (FAILED(hr = pThis->FireEventExt(bstrNameArrive,
							CComDISPPARAMS(2, CComVariant((IDispatch*) pThing),
										   CComVariant((IDispatch*) pThis)),
							FIREEVENT_DEFAULT)))
			return hr;

		// Create the Action event subject list
		if (FAILED(hr = CreatePropertyList(m_pWorld, &pListArrive)))
        {
            // Helper function has produced a trace, but we need errorinfo here
	        return ReportPortalError(hr);
        }
		if (FAILED(hr = pListArrive->AddThing(pThing)))
	        return hr;
		if (FAILED(hr = pListArrive->AddThing(pDestRoom)))
	        return hr;

		// Fire the Action event
		if (FAILED(hr = pDestRoom->FireEventExt(CComBSTR("Action"),
								CComDISPPARAMS(3, CComVariant(bstrNameArrive),
											   CComVariant(bstrDescriptionArrive),
											   CComVariant((IDispatch*) pListArrive)),
								FIREEVENT_DEFAULT)))
			return hr;
	}

	// No destination set
	else
	{
		VWTRACE(m_pWorld, "VWPORTAL", VWT_ERROR, "CPortalExemplarObject::Transport: No destination thing\n");
		transportHr = VWPORTAL_E_NODESTINATIONS;
	}

ERROR_ENCOUNTERED:
	// Check if we had an error occur related to the portal destination in Transport
	// Fire UI event to display message in text history if so...
	if (transportHr!=S_OK)
	{
		// Send message to the user that the destination is invalid
		static CComBSTR kbstrEvent = VW_PORTALDESTINATIONINVALID_EVENT_STR;

		if (pGlobal)
		{
				pGlobal->InvokeMethodExt(CComBSTR("FireRemoteUIEvent"),  
										  CComDISPPARAMS(4, CComVariant((IDispatch *) pThing), 
										  CComVariant((IDispatch *) pThis), 
										  CComVariant(kbstrEvent), 
										  CComVariant()), NULL);
		}
		return (ReportPortalError(transportHr));
	}

	// Otherwise return hr 
	else
		return hr;
}

// ******************************************************
// TransportTo
// Called on destination portal
// Checks AcceptTransport and does the MoveInto

STDMETHODIMP CPortalExemplarObject::TransportTo(IThing * pThing)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HRESULT hr = S_OK;
	CComPtr<IThing> pThis;
	CComPtr<IThing> pDestRoom;
	CComVariant varReturn;

	VWTRACE(m_pWorld, "VWPORTAL", VWT_METHOD, "CPortalExemplarObject::TransportTo\n");

	if (pThing==NULL)
    {
        VWTRACE(m_pWorld, "VWPORTAL", VWT_ERROR, "CPortalExemplarObject::TransportTo: NULL pointer\n");
		return ReportPortalError(E_POINTER);
    }

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
        return hr;
	if (FAILED(hr = pThis->get_Container(&pDestRoom)))
	    return hr;

	// Check if this portal is accepting transport
	if (FAILED(hr = pThis->InvokeMethodExt(CComBSTR("AcceptTransport"), 
		CComDISPPARAMS(1, CComVariant((IDispatch*) pThing)), &varReturn)))
        return hr;
	
	if (varReturn.vt!=VT_BOOL)
    {
		VWREPORT(m_pWorld, VWR_ERROR, "CPortalExemplarObject::TransportTo AcceptTransport returned invalid type\n");
        return ReportPortalError(VWPORTAL_E_UNKNOWN);
    }

	// if doesn't allow, exit with ok status
	if (varReturn.boolVal != VARIANT_TRUE)
		return S_OK;

	// Now do actual move - call on tranportee (pThing) 
	if (FAILED(hr = pThing->InvokeMethodExt(CComBSTR("MoveInto"),
						CComDISPPARAMS(1, CComVariant((IDispatch*) pDestRoom)),
						NULL)))
        return hr;

    return hr;
}

// ******************************************************
// AllowTransport
// Called on source portal
// Can be overridden to do checks on who is allowed to 
// transport through a portal

STDMETHODIMP CPortalExemplarObject::AllowTransport(IThing* pThing, VARIANT_BOOL* pbool)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pThing==NULL || pbool==NULL)
    {
        VWTRACE(m_pWorld, "VWPORTAL", VWT_ERROR, "CPortalExemplarObject::AllowTransport: NULL pointer\n");
		return ReportPortalError(E_POINTER);
    }

	// default
	*pbool = VARIANT_TRUE;

	 return S_OK;
}

// ******************************************************
// AcceptTransport
// Called on destination portal
// Can be overridden to do checks on who is allowed 
// to transport in though this portal

STDMETHODIMP CPortalExemplarObject::AcceptTransport(IThing* pThing, VARIANT_BOOL* pbool)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (pThing==NULL || pbool==NULL)
    {
        VWTRACE(m_pWorld, "VWPORTAL", VWT_ERROR, "CPortalExemplarObject::AcceptTransport: NULL pointer\n");
		return ReportPortalError(E_POINTER);
    }

	// default
	*pbool = VARIANT_TRUE;

	 return S_OK;
}


//********************************************
// SetDestinationByID
// Given an id, sets the portal's first item
// in its destination list to that object

STDMETHODIMP CPortalExemplarObject::SetDestinationByID(long lNewID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	HRESULT			hr = S_OK;
	CComPtr<IThing>	pThis;
	CComPtr<IThing>	pNewObj;
	CComVariant		varNewDest;
	CComPtr<IPropertyList>	pDestList;
	long			lCount;
	static CComBSTR	kbstrDest	= "Destinations";

	hr = m_pWorld->get_ObjectByID(lNewID, &pNewObj);
	if (FAILED(hr) || (!pNewObj.p))
		return hr;

	varNewDest = pNewObj;

	hr = m_pWorld->get_This(&pThis);
	if (FAILED(hr) || (!pThis.p))
		return hr;

	hr = pThis->get_ObjectProperty(kbstrDest, (IObjectProperty **)&pDestList);
	if (FAILED(hr) || (!pDestList.p))
		return hr;
	
	hr = pDestList->get_Count(&lCount);
	if (FAILED(hr))
		return hr;

	if (lCount > 0) 
		hr = pDestList->put_Property(0, varNewDest);
	else
		hr = pDestList->AddThing(pNewObj);

	if (FAILED(hr))
		return hr;

	hr = pThis->put_ObjectProperty(kbstrDest, (IObjectProperty *)pDestList);
	
	return hr;
}
