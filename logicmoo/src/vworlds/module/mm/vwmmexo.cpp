// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// vwmmexo.cpp : Implementation of CMultimediaExemplarObject
#include "stdafx.h"
#include "vwutils.h"
#include <vwmmex.h>
#include "vwmmexo.h"
#include "syshelp.h"

//******************************************************
// Install
// Installs this module - creates the exemplar and
// adds the properties and methods.

STDMETHODIMP CMultimediaExemplarObject::Install(IModule* pModule)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CMethodInfo rgMethod[] =
	{
		{ METHOD_CLIENT | METHOD_SERVER, "CreateGeometry", 			NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER, "CreateFrame", 			NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER, "CreateSpritedGeometry",	NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER, "CreateVector",			NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER, "CreateAccessoryData",		NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER, "CreateSound",				NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER, "CreateBoundary",			NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER, "CreateBoundaryList",		NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER, "CreatePickData",			NULL,	PSBIT_GLOBALDEFAULTMETHOD, PS_GLOBALDEFAULTMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER, "OnEnableAutoDownloadAccess",	"OnAutoDownloadAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD }
	};
	int cMethod = sizeof(rgMethod)/sizeof(rgMethod[0]);

	int iMethod;
	HRESULT hr = S_OK;
	CComPtr<IThing> pGlobal;	
	CComPtr<IModule> pSysModule;
	CComBSTR bstrName;
	CComBSTR bstrNameInternal;


	hr = m_pWorld->get_Global(&pGlobal);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	for (iMethod=0; iMethod<cMethod; iMethod++)
	{
		bstrName = rgMethod[iMethod].pszName;

		if (rgMethod[iMethod].pszNameInternal == NULL)
			bstrNameInternal = bstrName;
		else
			bstrNameInternal = rgMethod[iMethod].pszNameInternal;
			
		hr = pGlobal->CreateAndAddMethodExt(rgMethod[iMethod].lFlags,
											pModule, 
											bstrName,
											bstrNameInternal,
											rgMethod[iMethod].psbits,
											rgMethod[iMethod].permissions);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	// Now, add in the OnXXXAccess methods that reference OnWorldOwnerAccess in the system module
	// First, get system module

	if (FAILED(hr = m_pWorld->get_Module(CComBSTR("System"), &pSysModule)))
	    goto ERROR_ENCOUNTERED;

	if ((pGlobal) && (pSysModule))
	{
		if (FAILED(hr = pGlobal->CreateAndAddMethodExt( METHOD_CLIENT | METHOD_SERVER,
													pSysModule,
													CComBSTR("OnWorldOwnerAccess"),
													CComBSTR("OnDefaultSpriteFileAccess"),
													PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, 
													PS_ALLEXECUTEMETHOD )))
            goto ERROR_ENCOUNTERED;
	}

	// Add properties to global object
 	hr = pGlobal->AddPropertyExt(	CComBSTR("EnableAutoDownload"), 
									CComVariant((bool)VARIANT_TRUE), 
									PSBIT_NOTREMOTED | PSBIT_WORLDOWNERPROPERTY, 
									PS_ALLACCESSPROPERTY, 
									VT_BOOL, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

 	hr = pGlobal->AddPropertyExt(	CComBSTR("DefaultSpriteFile"), 
									CComVariant(CComBSTR("default.spr")), 
									PSBIT_WORLDOWNERPROPERTY, 
									PS_WORLDOWNERPROPERTY, 
									VT_BSTR, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	//Add animation constants to the global object
	hr = pGlobal->AddPropertyExt(CComBSTR("ANIMATION_OPEN"), CComVariant((long) 0x01L), PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pGlobal->AddPropertyExt(CComBSTR("ANIMATION_CLOSED"), CComVariant((long) 0x02L), PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pGlobal->AddPropertyExt(CComBSTR("ANIMATION_LINEARPOSITION"), CComVariant((long) 0x04L), PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pGlobal->AddPropertyExt(CComBSTR("ANIMATION_SPLINEPOSITION"), CComVariant((long) 0x8L), PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pGlobal->AddPropertyExt(CComBSTR("ANIMATION_SCALEANDROTATION"), CComVariant((long) 0x10L), PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pGlobal->AddPropertyExt(CComBSTR("ANIMATION_POSITION"), CComVariant((long) 0x20L), PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pGlobal->AddPropertyExt(CComBSTR("BOLD"), CComVariant((long)BOLD), PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pGlobal->AddPropertyExt(CComBSTR("ITALIC"), CComVariant((long)ITALIC), PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pGlobal->AddPropertyExt(CComBSTR("UNDERLINE"), CComVariant((long)UNDERLINE), PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pGlobal->AddPropertyExt(CComBSTR("STRIKETHROUGH"), CComVariant((long)STRIKETHROUGH), PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pGlobal->AddPropertyExt(CComBSTR("LABEL_CENTER"), CComVariant((long)LABEL_CENTER), PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pGlobal->AddPropertyExt(CComBSTR("LABEL_ABOVE"), CComVariant((long)LABEL_ABOVE), PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pGlobal->AddPropertyExt(CComBSTR("LABEL_BELOW"), CComVariant((long)LABEL_BELOW), PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pGlobal->AddPropertyExt(CComBSTR("LABEL_RIGHT"), CComVariant((long)LABEL_RIGHT), PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pGlobal->AddPropertyExt(CComBSTR("LABEL_LEFT"), CComVariant((long)LABEL_LEFT), PSBIT_GLOBALSYSTEMCONSTANT | PSBIT_INTERNALLYSET, PS_GLOBALSYSTEMCONSTANT, VT_I4, IID_NULL, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// install extensions to the standard exemplars
	hr = InstallThingExtensions(pModule);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = InstallAvatarExtensions(pModule);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = InstallHostExtensions(pModule);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = InstallArtifactExtensions(pModule);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = InstallRoomExtensions(pModule);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = InstallPortalExtensions(pModule);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = InstallLightExemplar(pModule);

ERROR_ENCOUNTERED:
    if (FAILED(hr))
        VWTRACE(m_pWorld, "VMMEXO", VWT_ERROR, "CMultimediaExemplarObject::Install: Error encountered: %x\n", hr);

    return hr;

}

STDMETHODIMP CMultimediaExemplarObject::CreateGeometry(BSTR bstrGeometryName, IVWGeometry **ppgeometry)
{
	HRESULT hr = ::CreateGeometry(m_pWorld, bstrGeometryName, ppgeometry);

	return ReportMMError(hr);
};

STDMETHODIMP CMultimediaExemplarObject::CreateFrame(IVWFrame **ppframe)
{
	HRESULT hr = ::CreateFrame(m_pWorld, ppframe);

	return ReportMMError(hr);
};

STDMETHODIMP CMultimediaExemplarObject::CreateSpritedGeometry(BSTR bstrGeometryName, IVWGeometrySprite **ppgeometry)
{
	HRESULT hr = ::CreateSpritedGeometry(m_pWorld, bstrGeometryName, ppgeometry);

	return ReportMMError(hr);
};

STDMETHODIMP CMultimediaExemplarObject::CreateVector(float x, float y, float z, IVector** ppvector)
{
	HRESULT hr = ::CreateVector(m_pWorld, x, y, z, ppvector);

	return ReportMMError(hr);
}

STDMETHODIMP CMultimediaExemplarObject::CreateAccessoryData(BSTR bstrAccessoryGraphic, float fPosX, float fPosY, IVWAccessoryData **ppaccessorydata)
{
	HRESULT hr = ::CreateAccessoryData(m_pWorld, bstrAccessoryGraphic, fPosX, fPosY, ppaccessorydata);

	return ReportMMError(hr);
}

STDMETHODIMP CMultimediaExemplarObject::CreateSound(BSTR SoundURL, VARIANT_BOOL bIs3D, VARIANT_BOOL bIsLooping, ISound **ppSound)
{
	HRESULT hr = ::CreateSound(m_pWorld, SoundURL, bIs3D, bIsLooping, ppSound);

	return ReportMMError(hr);
}

STDMETHODIMP CMultimediaExemplarObject::CreateBoundaryList(IPropertyList** ppcell)
{
	HRESULT hr = ::CreateBoundaryList(m_pWorld, ppcell);

	return ReportMMError(hr);
};

STDMETHODIMP CMultimediaExemplarObject::CreatePickData(IVWPickData** ppPickData)
{
	HRESULT hr = ::CreatePickData(m_pWorld, ppPickData);

	return ReportMMError(hr);
};


STDMETHODIMP CMultimediaExemplarObject::CreateBoundary(VARIANT varInitialValue, IBoundary** ppBoundary)
{
	HRESULT hr = S_OK;
	CComPtr<IPropertyList> pPropertyList;
	CComVariant varTemp;
	long nCount = 0;
	int i;
	float fX, fY;
	VARIANT_BOOL vbSafe = VARIANT_FALSE;

    if (ppBoundary == NULL)
    {
        VWTRACE(m_pWorld, "VWMMEXO", VWT_ERROR, "CMultimediaExemplarObject::CreateBoundary: NULL output pointer\n");
        return ReportMMError(E_POINTER);
    }

    *ppBoundary = NULL;

	hr = ::CreateBoundary(m_pWorld, ppBoundary);
	if (FAILED(hr))
		return ReportMMError(hr);

	hr = ::CreatePropertyList(m_pWorld, &pPropertyList);
	if (FAILED(hr))
        goto ERROR_ENCOUNTERED;
	
	// VT_ERROR means optional arg not passed in
	if (varInitialValue.vt != VT_ERROR)
	{
		hr = VariantCopyInd(&varTemp, &varInitialValue);
		if (FAILED(hr))
        {
            VWTRACE(m_pWorld, "VWMMEXO", VWT_ERROR, "CMultimediaExemplarObject::CreateBoundary: VARIANT copy failed\n");
    		goto ERROR_ENCOUNTERED;
        }

		hr = ConvertJSArrayToPropertyList(&varTemp, pPropertyList);
		if (FAILED(hr))
		{
			hr = ConvertSAFEARRAYToPropertyList(&varTemp, pPropertyList);
			if (FAILED(hr))
            {
                VWTRACE(m_pWorld, "VWMMEXO", VWT_ERROR, "CMultimediaExemplarObject::CreateBoundary: Unable to convert VARIANT to property list\n");
        		goto ERROR_ENCOUNTERED;
            }
		}
	}

	hr = pPropertyList->get_Count(&nCount);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (nCount != 0)
	{
		//This is an array of floats, so make sure it's sets of 2
		//and there is at least 3 elements. (minimum to make a closed polygon)
		if (nCount < 6)
		{
			VWTRACE(m_pWorld, "MMEXO", VWT_ERROR, "CreateBoundary: list must contain at least 6 values (3 pairs of 2 numbers)\n");
			hr = E_INVALIDARG;
            goto ERROR_ENCOUNTERED;
		}

		//Go through floats in the array 3 at a time
		for (i = 0; i < nCount; i = i + 2)
		{
			hr = pPropertyList->get_Property(i, &varTemp);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

 			hr = GetNumericFromVariant(varTemp, &fX);
 			if (FAILED(hr))
            {
                VWTRACE(m_pWorld, "VWMMEXO", VWT_ERROR, "CMultimediaExemplarObject::CreateBoundary: property VARIANT is not a number\n");
                goto ERROR_ENCOUNTERED;
            }

 			varTemp.Clear();

			hr = pPropertyList->get_Property(i+1, &varTemp);
			if (FAILED(hr))
				return hr;

 			hr = GetNumericFromVariant(varTemp, &fY);
            if (FAILED(hr))
            {
                VWTRACE(m_pWorld, "VWMMEXO", VWT_ERROR, "CMultimediaExemplarObject::CreateBoundary: property VARIANT is not a number\n");
                goto ERROR_ENCOUNTERED;
            }

 			varTemp.Clear();

			hr = (*ppBoundary)->InsertVertex(-1, fX, fY);
			if (FAILED(hr)) 
				goto ERROR_ENCOUNTERED;
		}

		hr = (*ppBoundary)->TranslateVertexSafe(0, 0, 0, &vbSafe);
		if (FAILED(hr)) 
			goto ERROR_ENCOUNTERED;

 		if (vbSafe != VARIANT_TRUE)
 		{
			VWTRACE(m_pWorld, "MMEXO", VWT_ERROR, "CreateBoundary: IBoundary::TranslateVertexSafe failed, boundary not simple\n");
            hr = VWMM_E_INVALIDBOUNDARY;
 		}
	}

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppBoundary);

	return ReportMMError(hr);
};

//-------------------------------------------------------------------
// OnAutoDownloadAccess
// Check that user is caller

STDMETHODIMP CMultimediaExemplarObject::OnAutoDownloadAccess(long lPerms, VARIANT_BOOL* bReturnValue)
{
	CComPtr<IThing> pCaller;
	CComPtr<IThing> pUser;
	CComPtr<IThing> pGlobal;
	CComBSTR bstrCaller("<none>"), bstrUser("<none>");

	HRESULT hr = S_OK;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// If you have a pointer, set return value false by default
	if (bReturnValue == NULL)
    {
        VWTRACE(m_pWorld, "VWMMEXO", VWT_ERROR, "CMultimediaExemplarObject::OnAutoDownloadAccess: NULL output pointer\n");
        return ReportMMError(E_POINTER);
    }

	if (lPerms != PS_WRITE)
	{
		*bReturnValue = VARIANT_TRUE;
		return S_OK;
	}
	else
		*bReturnValue = VARIANT_FALSE;

	
	hr = m_pWorld->get_Caller(&pCaller);
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "MMEXO", VWT_ERROR, "CMMExemplarObject::OnAutoDownloadAccess - couldn't get Caller \n");	
		return hr;
	}

	hr = m_pWorld->get_User(&pUser);
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "MMEXO", VWT_ERROR, "CMMExemplarObject::OnAutoDownloadAccess - couldn't get User \n");
		return hr;
	}

#ifdef _DEBUG
	if (pCaller)
		pCaller->get_Name(&bstrCaller.m_str);
	if (pUser)
		pUser->get_Name(&bstrUser.m_str);
#endif

	// Security: only user can set property on him/herself so user must be caller
	if ((pCaller) && (pUser))
	{
		if (pCaller == pUser) 
			*bReturnValue = VARIANT_TRUE;
		else
			VWTRACE(m_pWorld, "MMEXO", VWT_ERROR, "CMMExemplarObject::OnAutoDownloadAccess - Caller not equal to user \n");
	}
	// Check if you're first setting up world and adding this property or if you're accessing from HTML
	else if (pCaller == NULL)
		*bReturnValue = VARIANT_TRUE;

	return hr;
};