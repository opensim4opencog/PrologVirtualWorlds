// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// mmportal.cpp : Portal extensions

#include "stdafx.h"
#include <d3drmwin.h>
#include <vwmmex.h>
#include <vwidata.h>
#include <vwanim.h>
#include "syshelp.h"
#include "vwutils.h"
#include "vwmmexo.h"

///////////////////////////////////////////////////////////////////////
// Parameters


///////////////////////////////////////////////////////////////////////
// Helpers

///////////////////////////////////////////////////////////////////////
// Install 

HRESULT CMultimediaExemplarObject::InstallPortalExtensions(IModule* pModule)
{
	CComPtr<IThing> pPortalEx;
	CComPtr<IThing> pIWPortalEx;
	CComPtr<IPropertyList> pMenu;
	CComPtr<IPropertyList> pList;
	CComPtr<IMenuItem> pMenuItem;
	CComPtr<IVector> pVec;
	CComBSTR bstrTexture;
	CComBSTR bstrPortal;
	CComBSTR bstrIWPortal;
    HRESULT hr = S_OK;

	if (FAILED(hr = m_pWorld->get_Exemplar(CComBSTR("Portal"), &pPortalEx)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pWorld->get_Exemplar(CComBSTR("IWPortal"), &pIWPortalEx)))
		goto ERROR_ENCOUNTERED;

	// add in methods
	if (FAILED(hr = pPortalEx->CreateAndAddMethodExt(METHOD_SERVER, pModule, 
												CComBSTR("TransportTo"),
												CComBSTR("TransportTo"),
												PSBIT_EXEMPLARDEFAULTMETHOD, PS_ALLEXECUTEMETHOD)))
		goto ERROR_ENCOUNTERED;

    if (FAILED(hr = pPortalEx->CreateAndAddMethodExt(METHOD_CLIENT, pModule, 
												CComBSTR("OnIntersectee"),
												CComBSTR("OnIntersectee"),
												PSBIT_EXEMPLARDEFAULTMETHOD, PS_ALLEXECUTEMETHOD)))
		goto ERROR_ENCOUNTERED;

	// add in properties
	if (FAILED(hr = CreateVector(0.0, 0.0, 0.0, &pVec)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pPortalEx->AddPropertyExt(	CComBSTR("EntryPoint"), 
											CComVariant((IDispatch *) pVec), 
											PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_DISPATCH, IID_IVector, NULL)))
		goto ERROR_ENCOUNTERED;

	pVec.Release();
	if (FAILED(hr = CreateVector(0.0, 0.0, 0.0, &pVec)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pPortalEx->AddPropertyExt(	CComBSTR("EntryOrientation"), 
											CComVariant((IDispatch *) pVec), 
											PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_DISPATCH, IID_IVector, NULL)))
		goto ERROR_ENCOUNTERED;

	pVec.Release();
	if (FAILED(hr = pPortalEx->AddPropertyExt(	CComBSTR("EntryRadius"), 
											CComVariant((float) 3.0f),
											PSBIT_EXEMPLARDEFAULTPROPERTY, 
											PS_EXEMPLARDEFAULTPROPERTY, 
											VT_R4, IID_NULL, NULL)))
		goto ERROR_ENCOUNTERED;

	// Add items to edit maps
	bstrTexture = "Texture|Edit an object's color or texture|texture.htm";
	bstrPortal = "Portal|Edit the Portal's properties|portaledit.htm";
	bstrIWPortal = "Portal|Edit the InterWorld Portal's properties|iwportaledit.htm";

	// Owner edit map
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrTexture)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrPortal)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pPortalEx->put_ObjectProperty(CComBSTR("OwnerEditMap"), (IObjectProperty *) pList)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrTexture)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrIWPortal)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pIWPortalEx->put_ObjectProperty(CComBSTR("OwnerEditMap"), (IObjectProperty *) pList)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

	// Builder (author) edit map
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrTexture)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrPortal)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pPortalEx->put_ObjectProperty(CComBSTR("BuilderEditMap"), (IObjectProperty *) pList)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrTexture)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrIWPortal)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pIWPortalEx->put_ObjectProperty(CComBSTR("BuilderEditMap"), (IObjectProperty *) pList)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

	// Advanced (wizard) Edit Map
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrPortal)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrTexture)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pPortalEx->put_ObjectProperty(CComBSTR("AdvancedEditMap"), (IObjectProperty *) pList)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrIWPortal)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrTexture)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pIWPortalEx->put_ObjectProperty(CComBSTR("AdvancedEditMap"), (IObjectProperty *) pList)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

    return S_OK;

ERROR_ENCOUNTERED:
	
	VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::InstallPortalExtensions: Error encountered: %x\n", hr);
    return hr;
}

// ******************************************************
// TransportTo
// Called on destination portal
// Moves transported object into correct position, scattering entrants with an EntryRadius


STDMETHODIMP CMultimediaExemplarObject::TransportTo(IThing * pThing)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing>		puser;
	CComPtr<IThing>		pThis;
	CComPtr<IThing>		pDestRoom;
	CComPtr<IVector>	pNewLoc;
	CComPtr<IVector>	pNewOrient;
	CComPtr<IVector>	pDoorLoc;
	CComPtr<IVector>	pDoorDir;
	CComPtr<IVector>	pScatterOff;
	CComPtr<IVector>	pvPos;
	CComPtr<IVector>	pvDir;
	CComPtr<IPropertyList> pList;
	float				fPosX, fPosY, fPosZ;
	float				fDirX, fDirY, fDirZ;
	CComVariant			varScatter;
    HRESULT             hr = S_OK;

	VWTRACE(m_pWorld, "MMPORTAL", VWT_METHOD, "CMMPortalExemplarObject:: TransportTo \n");

    if (pThing==NULL)
    {
	    VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::TransportTo: NULL pointer\n");
        return ReportMMError(E_POINTER);
    }

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		return hr;

	// Get entry point 
	if (FAILED(hr = pThis->get_ObjectProperty(CComBSTR("EntryPoint"), (IObjectProperty **)&pNewLoc)))
		return hr;
    if (pNewLoc == NULL)
        return S_OK;
	if (FAILED(hr = pNewLoc->get(&fPosX, &fPosY, &fPosZ)))
		return hr;
	pNewLoc.Release();

    if (FAILED(hr = CreateVector(fPosX, fPosY, fPosZ, &pNewLoc)))
		return hr;

	// Get entry orientation
	if (FAILED(hr = pThis->get_ObjectProperty(CComBSTR("EntryOrientation"), (IObjectProperty **)&pNewOrient)))
		return hr;
    if (pNewOrient == NULL)
        return S_OK;
	if (FAILED(hr = pNewOrient->get(&fDirX, &fDirY, &fDirZ)))
		return hr;
	pNewOrient.Release();

    if (FAILED(hr = CreateVector(fDirX, fDirY, fDirZ, &pNewOrient)))
		return hr;
			
	// Get the scatter
	if (FAILED(hr = pThis->get_Property(CComBSTR("EntryRadius"), &varScatter)))
		return hr;
	if (FAILED(hr = varScatter.ChangeType(VT_R4)))
    {
	    VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::TransportTo: Entry radius not a float\n");
        return ReportMMError(hr);
    }
	
    varScatter.fltVal *= ((float) (rand() % 1000)) / 1000.0f;

    if (FAILED(hr = CreateVector(varScatter.fltVal, 0.0f, 0.0f, &pScatterOff)))
		return hr;
	if (FAILED(hr = pScatterOff->Rotate((float) rand(), pScatterOff)))
		return hr;
	pNewLoc->Add(pScatterOff, pNewLoc);

	// Set position & orientation
	if (FAILED(hr = pThing->get_ObjectProperty(CComBSTR("Position"), (IObjectProperty**)&pvPos)))
		return hr;
    if (pvPos == NULL)
        return S_OK;
    pvPos->set(fPosX, fPosY, fPosZ);

	if (FAILED(hr = pThing->get_ObjectProperty(CComBSTR("Direction"), (IObjectProperty**)&pvDir)))
		return hr;
    if (pvDir == NULL)
        return S_OK;
	pvDir->set(fDirX, fDirY, fDirZ);

	// Now do actual move - call on tranportee (pThing) 
	if (FAILED(hr = pThis->get_Container(&pDestRoom)))
		return hr;
	if (FAILED(hr = pThing->InvokeMethodExt(CComBSTR("MoveInto"),
						CComDISPPARAMS(1, CComVariant((IDispatch*) pDestRoom)),
						NULL)))
		return hr;

#ifdef _DEBUG
	CComPtr<IThing> pCaller;
	CComBSTR bstrCallerName;
	m_pWorld->get_Caller(&pCaller);
	if (pCaller)
		pCaller->get_Name(&bstrCallerName.m_str);
#endif


    VWTRACE(m_pWorld, "MMPORTAL", VWT_METHOD, "CMMPortalExemplarObject::TransportTo Finished; hr = %x \n", hr);
	return hr;
}

// ******************************************************
// OnIntersectee
// This method is called when something approaches the portal by intersecting with a defined cell
// It calls the TransportMenu method, which will either show the user an URL and give him/her 
// the option of going through, or just go through the portal

STDMETHODIMP CMultimediaExemplarObject::OnIntersectee(IThing *pIntersector, float fX, float fY, float fZ, 
												  int iEdgeCycIndex, int iEdgeIndex, VARIANT_BOOL bTransition)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CComPtr<IThing> pThis;
    HRESULT         hr = S_OK;

	if (SUCCEEDED(hr = m_pWorld->get_This(&pThis)))
    {
    	if (SUCCEEDED(hr = pThis->InvokeMethodExt(CComBSTR("ShowTransportDialog"),
									   NULL, NULL)))
            return S_OK;
    }

    VWTRACE(m_pWorld, "MMPORTAL", VWT_METHOD, "CMMPortalExemplarObject::OnIntersectee Error: %x\n", hr);
	return hr;
}
