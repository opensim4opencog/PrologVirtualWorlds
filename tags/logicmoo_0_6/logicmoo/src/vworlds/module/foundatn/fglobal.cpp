// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include <vwfound.h>
#include <vwutils.h>
#include "foundatn.h"
#include <tranito.h>
#include "syshelp.h"

///////////////////////////////////////////////////////////////////////////////
// Global stuff

HRESULT CFoundationExemplars::GlobInstall(IModule *pModule)
{
	// This is overkill for one method, but leaves us room to grow!
	CMethodInfo rgMethod[] =
	{
		{ METHOD_CLIENT|METHOD_SERVER,	"CreateTransactionItem",	NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_SYNC|METHOD_SERVER,	"GetPortalIDs",				NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_SYNC|METHOD_SERVER,	"GetContainerID",			NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_SYNC|METHOD_SERVER,	"GetNamesFromIDs",			NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_GLOBALDEFAULTMETHOD },
	};
	int cMethod = sizeof(rgMethod)/sizeof(rgMethod[0]);

	HRESULT			hr = S_OK;
	CComPtr<IThing>	pGlobal;
	CComBSTR		bstrName;
	CComBSTR		bstrNameInternal;
	CComPtr<IPropertySecurity> pSec;

	// find the global object
	hr = m_pWorld->get_Global(&pGlobal);
	if (FAILED(hr))
		return hr;

	// add in methods
	for (int iMethod=0; iMethod<cMethod; iMethod++)
	{
		bstrName = rgMethod[iMethod].pszName;

		if (rgMethod[iMethod].pszNameInternal == NULL)
			bstrNameInternal = bstrName;
		else
			bstrNameInternal = rgMethod[iMethod].pszNameInternal;
			
		hr = pGlobal->CreateAndAddMethodExt(rgMethod[iMethod].lFlags,
											pModule,
											bstrNameInternal,
											bstrName,
											rgMethod[iMethod].psbits,
											rgMethod[iMethod].permissions);
		if (FAILED(hr))
			return hr;
	}

#ifdef _DEBUG
	// pGlobal->InvokeMethodExt(CComBSTR("TraceExemplarInfo"), NULL, NULL);
#endif

	return hr;
}

// CreateTransactionItem
// Method to create a new TransactionItem and initialize all values
// Assume that we are in foundation
 
typedef CComObject<CVWTransactionItemObject> CComVWTransactionItem;

STDMETHODIMP CFoundationExemplars::CreateTransactionItem(BSTR bstrName, BSTR bstrDescription, 
		BSTR bstrGraphic, BSTR bstrExemplarType, IVWTransactionItem **ppTransactionItem)
{
	HRESULT hr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CreateTransactionItem\n");

	ASSERT(ppTransactionItem);
	if (ppTransactionItem == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CreateTransactionItem: outgoing pointer (IVWTransactionItem**) is NULL\n");
		return E_POINTER;
	}

	*ppTransactionItem = new CComVWTransactionItem;

	if (*ppTransactionItem == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CreateTransactionItem: failed to allocate CComVWTransactionItem\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(*ppTransactionItem);

	(*ppTransactionItem)->AddRef();

	hr = (*ppTransactionItem)->put_World(m_pWorld);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppTransactionItem)->put_Name(bstrName);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppTransactionItem)->put_Description(bstrDescription);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppTransactionItem)->put_Graphic(bstrGraphic);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppTransactionItem)->put_ExemplarType(bstrExemplarType);

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppTransactionItem);

	return hr;
}


//********************************************
// GetPortalIDs
// Returns a list of all of the IDs of all of the
// portals in a room.

STDMETHODIMP CFoundationExemplars::GetPortalIDs(long idRoom, IPropertyList **ppPortalList)
{
	HRESULT	hr = S_OK;
	CComPtr<IThing>	pRoom;
	CComPtr<IThing>	pCurPortal;
	CComPtr<IPropertyList>	pPortals;
	long	lPortals, lcurID, i;
	static CComBSTR	kbstrPortals = "Portals";

	if (ppPortalList == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CFoundationExemplars::GetPortalIDs: NULL ouput pointer\n");
		return E_POINTER;
    }

	*ppPortalList = NULL;

	hr = m_pWorld->get_ObjectByID(idRoom, &pRoom);
	if (FAILED(hr) || (!pRoom.p))
		goto ERROR_ENCOUNTERED;

	hr = ::CreatePropertyList(m_pWorld, ppPortalList);
	if (FAILED(hr) || (!*ppPortalList))
		goto ERROR_ENCOUNTERED;

	hr = pRoom->get_ObjectProperty(kbstrPortals, (IObjectProperty **)&pPortals);
	if (FAILED(hr) || (!pPortals.p))
		goto ERROR_ENCOUNTERED;

	hr = pPortals->get_Count(&lPortals);
	if (FAILED(hr) || (lPortals == 0))
		goto ERROR_ENCOUNTERED;

	for (i = 0; i < lPortals; i++)
	{
		hr = pPortals->get_Thing(i, &pCurPortal);
		if (SUCCEEDED(hr) && (pCurPortal.p))
		{
			hr = pCurPortal->get_ID(&lcurID);
			if (SUCCEEDED(hr))
			{
				(*ppPortalList)->AddLong(lcurID);
			}

			pCurPortal.Release();
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}


//********************************************
// GetContainerID
// Given an objID, returns the ID of its container.

STDMETHODIMP CFoundationExemplars::GetContainerID(long idObj, long *pidContainer)
{
	HRESULT	hr = S_OK;
	CComPtr<IThing>	pObj;
	CComPtr<IThing>	pContainer;
	static CComBSTR	kbstrContainer	= "Container";
	*pidContainer = -1;
	
	hr = m_pWorld->get_ObjectByID(idObj, &pObj);
	if (FAILED(hr) || (!pObj.p))
		goto ERROR_ENCOUNTERED;

	hr = pObj->get_Thing(kbstrContainer, &pContainer);
	if (FAILED(hr) || (!pContainer.p))
		goto ERROR_ENCOUNTERED;

	hr = pContainer->get_ID(pidContainer);

ERROR_ENCOUNTERED:
	return hr;
}


//********************************************
// GetNamesFromIDs
// Given one or more ids, returns a list of
// the name of the objects.

STDMETHODIMP CFoundationExemplars::GetNamesFromIDs(VARIANT varIDorList, IPropertyList **ppNameList)
{
	HRESULT	hr = S_OK;
	CComPtr<IThing>	pCurObj;
	CComPtr<IPropertyList>	pIDList;
	BSTR	bstrCurName	= NULL;
	long	lObjCount, curObjID, i;

	if (ppNameList == NULL)
    {
        VWTRACE(m_pWorld, "COREEXEMP", VWT_ERROR, "CFoundationExemplars::GetNamesFromIDs: NULL ouput pointer\n");
		return E_POINTER;
    }

	*ppNameList = NULL;

	hr = ::CreatePropertyList(m_pWorld, ppNameList);
	if (FAILED(hr) || (!*ppNameList))
		goto ERROR_ENCOUNTERED;


	if (varIDorList.vt == VT_DISPATCH)
	{
		pIDList = (IPropertyList *)varIDorList.pdispVal;
		
		hr = pIDList->get_Count(&lObjCount);
		if (FAILED(hr) || (lObjCount == 0))
			goto ERROR_ENCOUNTERED;
	}
	else
	{
		::VariantChangeType(&varIDorList, &varIDorList, 0, VT_I4);
		lObjCount = 1;
	}

	for (i = 0; i < lObjCount; i++)
	{
		if (pIDList.p)
			pIDList->get_Long(i, &curObjID);
		else
			curObjID = varIDorList.lVal;

		hr = m_pWorld->get_ObjectByID(curObjID, &pCurObj);
		if (FAILED(hr) || (!pCurObj.p))
			goto ERROR_ENCOUNTERED;

		hr = pCurObj->get_Name(&bstrCurName);
		if (SUCCEEDED(hr))
		{
			(*ppNameList)->AddString(bstrCurName);
			SAFEFREESTRING(bstrCurName);
		}

		pCurObj.Release();
	}


ERROR_ENCOUNTERED:
	return hr;
}
