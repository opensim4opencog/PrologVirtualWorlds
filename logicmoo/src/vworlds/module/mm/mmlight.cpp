// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// mmlight.cpp : Light exemplar

#include "stdafx.h"
#include <d3drmwin.h>
#include <vwmmex.h>
#include <vwidata.h>
#include <vwanim.h>
#include "vwutils.h"
#include "vwmmexo.h"

#include <syshelp.h>


///////////////////////////////////////////////////////////////////////
// Helpers

extern HRESULT	_GetFrameHelper(IThing *pThing, IVWFrame **ppvwf);
extern float CastToFloat(VARIANT var);
///////////////////////////////////////////////////////////////////////
// Install Light Exemplar

HRESULT CMultimediaExemplarObject::InstallLightExemplar(IModule* pModule)
{
	HRESULT hr = S_OK;
	IThing *pLightExemplar = NULL;
	IVector *pVector = NULL;
	CComBSTR bstrName, bstrNameInternal, bstrTexture, bstrLight;
	CComPtr<IPropertyList> pList;

	CMethodInfo rgMethod[] =
	{
				
		{ METHOD_CLIENT,	"OnLightTypeChanged",				"OnLightTypeChanged",				PSBIT_EVENTHANDLER,		PS_EVENTHANDLER },
		{ METHOD_CLIENT,	"OnLightColorChanged",				"OnLightColorChanged",				PSBIT_EVENTHANDLER,		PS_EVENTHANDLER },
		{ METHOD_CLIENT,	"OnLightUmbraChanged",				"OnLightUmbraChanged",				PSBIT_EVENTHANDLER,		PS_EVENTHANDLER },
		{ METHOD_CLIENT,	"OnLightPenumbraChanged",			"OnLightPenumbraChanged",			PSBIT_EVENTHANDLER,		PS_EVENTHANDLER },
		{ METHOD_CLIENT,	"OnLightRangeChanged",				"OnLightRangeChanged",				PSBIT_EVENTHANDLER,		PS_EVENTHANDLER },
		{ METHOD_CLIENT,	"OnLightEnabledChanged",			"OnLightEnabledChanged",			PSBIT_EVENTHANDLER,		PS_EVENTHANDLER },
		{ METHOD_CLIENT,	"OnLightAttenuationChanged",		"OnLightAttenuationChanged",		PSBIT_EVENTHANDLER,		PS_EVENTHANDLER },
		{ METHOD_CLIENT,	"OnLoadGeometry",					"OnLightLoadGeometry",				PSBIT_HIDDEN,			PS_EXEMPLARDEFAULTMETHOD  }

	};
	int cMethod = sizeof(rgMethod)/sizeof(rgMethod[0]);
    int iMethod;

	// Create the Light exemplar (instead of getting it from foundation exemplars)

	if (FAILED(hr = m_pWorld->CreateExemplarEx(CComBSTR("Light Exemplar"),
												CComBSTR("Light"),
												CComVariant(CComBSTR("Thing")), &pLightExemplar)))
        goto ERROR_ENCOUNTERED;

	// Set the description string
	if (FAILED(hr = pLightExemplar->put_Description(CComBSTR("Light"))))
	    goto ERROR_ENCOUNTERED;


	// Add Light methods
	for (iMethod=0; iMethod<cMethod; iMethod++)
	{
		bstrName = rgMethod[iMethod].pszName;

		if (rgMethod[iMethod].pszNameInternal == NULL)
			bstrNameInternal = bstrName;
		else
			bstrNameInternal = rgMethod[iMethod].pszNameInternal;
		
		if (FAILED(hr = pLightExemplar->CreateAndAddMethodExt(	rgMethod[iMethod].lFlags,
														pModule,
														bstrNameInternal,
														bstrName,
														rgMethod[iMethod].psbits,
														rgMethod[iMethod].permissions)))
            goto ERROR_ENCOUNTERED;
	}


	// Add Light properties
	if (FAILED(hr = pLightExemplar->AddPropertyExt(CComBSTR("LightType"),
											CComVariant((long)2l),
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_ALLACCESSPROPERTY,
											VT_I4, IID_NULL, NULL)))
        goto ERROR_ENCOUNTERED;

	//put a default vector in LightColor
	if (FAILED(hr = CreateVector(1.0f, 1.0f, 1.0f, &pVector)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pLightExemplar->AddPropertyExt(	CComBSTR("LightColor"), 
											CComVariant((IDispatch *)pVector), 
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_ALLACCESSPROPERTY,
											VT_DISPATCH, IID_IVector, NULL)))

		goto ERROR_ENCOUNTERED;

//	if (FAILED(hr = pLightExemplar->put_ObjectProperty(CComBSTR("LightColor"), pVector)))
//		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pLightExemplar->AddPropertyExt(CComBSTR("LightUmbra"),
											CComVariant((float)45.0f),
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_ALLACCESSPROPERTY,
											VT_R4, IID_NULL, NULL)))
        goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pLightExemplar->AddPropertyExt(CComBSTR("LightPenumbra"),
											CComVariant((float)60.0f),
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_ALLACCESSPROPERTY,
											VT_R4, IID_NULL, NULL)))
        goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pLightExemplar->AddPropertyExt(CComBSTR("LightRange"),
											CComVariant((float)10.0f),
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_ALLACCESSPROPERTY,
											VT_R4, IID_NULL, NULL)))
        goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pLightExemplar->AddPropertyExt(CComBSTR("LightEnabled"),
											CComVariant((bool)VARIANT_TRUE),
											PSBIT_EXEMPLARDEFAULTPROPERTY | PSBIT_FRIENDLY, 
											PS_EXEMPLARDEFAULTPROPERTY,
											VT_BOOL, IID_NULL, NULL)))
        goto ERROR_ENCOUNTERED;

	//put a default vector in LightAttenuation
	if (FAILED(hr = CreateVector(1.0f, 0.0f, 0.0f, &pVector)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pLightExemplar->AddPropertyExt(	CComBSTR("LightAttenuation"), 
											CComVariant((IDispatch *)pVector), 
											PSBIT_FRIENDLY | PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_ALLACCESSPROPERTY,
											VT_DISPATCH, IID_IVector, NULL)))

		goto ERROR_ENCOUNTERED;

//	if (FAILED(hr = pLightExemplar->put_ObjectProperty(CComBSTR("LightAttenuation"), pVector)))
//		goto ERROR_ENCOUNTERED;

	// Add items to edit maps
	bstrTexture = "Texture|Edit an object's color or texture|texture.htm";
	bstrLight = "Light|Edit the Light's properties|light.htm";

	// Owner edit map
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrTexture)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrLight)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pLightExemplar->put_ObjectProperty(CComBSTR("OwnerEditMap"), (IObjectProperty *) pList)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

	// Builder (author) edit map
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrTexture)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrLight)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pLightExemplar->put_ObjectProperty(CComBSTR("BuilderEditMap"), (IObjectProperty *) pList)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

	// Advanced (wizard) Edit Map
	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrLight)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrTexture)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pLightExemplar->put_ObjectProperty(CComBSTR("AdvancedEditMap"), (IObjectProperty *) pList)))
		goto ERROR_ENCOUNTERED;
	pList.Release();

    return S_OK;


ERROR_ENCOUNTERED:

    // Clean up
	VWTRACE(m_pWorld, "VWMMTHING", VWT_ERROR, "CMultimediaExemplarObject::InstallLightExemplar: Error encountered: %x\n", hr);
	SAFERELEASE(pLightExemplar);
	SAFERELEASE(pVector);
	return hr;
}



STDMETHODIMP CMultimediaExemplarObject::OnLightColorChanged(long lHint, VARIANT varHintData)
{
	CComPtr<IThing>     pThis;
	CComPtr<IVWFrame>	pvwfFrame;
	CComPtr<IVector>	pvLightColor;
	long		        lNumLights, lIndex; 
	float				flRed, flGreen, flBlue;

	HRESULT hr = S_OK;

	if ((lHint == thPut) || ((lHint >= 0) && (lHint <= 3)) )
    {
		if (FAILED(hr = m_pWorld->get_This(&pThis)))
			    return ReportMMError(hr);

        if (!pThis)
			    return ReportMMError(E_FAIL);

		if (FAILED(hr = _GetFrameHelper(pThis, &pvwfFrame)) || !pvwfFrame)
       		    return ReportMMError(hr);

		if (FAILED(hr = pThis->get_ObjectProperty(CComBSTR ("LightColor"), (IObjectProperty**)&pvLightColor)))
				return ReportMMError(hr);
			
		if (!pvLightColor)
		         return ReportMMError(E_FAIL);

		pvLightColor->get(&flRed, &flGreen, &flBlue);

		//get count of lights on this geometry and set the new light color on each of them

		if (FAILED(hr = pvwfFrame->GetLightCount(&lNumLights)))
		    return ReportMMError(hr);
		
		for (lIndex = 0;	lIndex<lNumLights;	lIndex++)
			if (FAILED(hr = pvwfFrame->SetLightColor(lIndex, flRed, flGreen, flBlue)))
					    return ReportMMError(hr);
		
	}
	return hr;
}



STDMETHODIMP CMultimediaExemplarObject::OnLightUmbraChanged(long lHint, VARIANT varHintData)
{

	CComPtr<IThing>     pThis;
	CComPtr<IVWFrame>	pvwfFrame;
	long		        lNumLights, lIndex; 
	CComVariant			varUmbra;
	CComVariant			varPenumbra;
	float				fUmbra, fPenumbra;
	HRESULT hr = S_OK;

	if ((lHint == thPut) || (lHint == 0))
    {
		if (FAILED(hr = m_pWorld->get_This(&pThis)))
			    return ReportMMError(hr);

        if (!pThis)
			    return ReportMMError(E_FAIL);

		if ((FAILED(hr = _GetFrameHelper(pThis, &pvwfFrame))) || (!pvwfFrame))
      		    return ReportMMError(hr);

		if (FAILED(hr = pThis->get_Property(CComBSTR("LightUmbra"), &varUmbra)))
			    return ReportMMError(hr);

		fUmbra = CastToFloat(varUmbra);

		if (FAILED(hr = pThis->get_Property(CComBSTR("LightPenumbra"), &varPenumbra)))
			    return ReportMMError(hr);

		fPenumbra = CastToFloat(varPenumbra);

		//convert degrees to radians here
		fUmbra = (fUmbra * .01745f);
		fPenumbra = (fPenumbra * .01745f);

		//get count of lights on this geometry and set the new spotlight characteristics on each of them

		if (FAILED(hr = pvwfFrame->GetLightCount(&lNumLights)))
			    return ReportMMError(hr);
		
		for (lIndex = 0;	lIndex<lNumLights;	lIndex++)
			if (FAILED(hr = pvwfFrame->SetSpotLightRadii(lIndex, fUmbra, fPenumbra)))
				    return ReportMMError(hr);
	}

	return hr;
}

STDMETHODIMP CMultimediaExemplarObject::OnLightPenumbraChanged(long lHint, VARIANT varHintData)
{

	CComPtr<IThing>     pThis;
	CComPtr<IVWFrame>	pvwfFrame;
	long		        lNumLights, lIndex; 
	CComVariant			varUmbra;
	CComVariant			varPenumbra;
	float				fUmbra, fPenumbra;
	HRESULT hr = S_OK;

	if (lHint == thPut)
    {
		if (FAILED(hr = m_pWorld->get_This(&pThis)))
			    return ReportMMError(hr);

        if (!pThis)
			    return ReportMMError(E_FAIL);

		if ((FAILED(hr = _GetFrameHelper(pThis, &pvwfFrame))) || (!pvwfFrame))
      		    return ReportMMError(hr);

		if (FAILED(hr = pThis->get_Property(CComBSTR("LightUmbra"), &varUmbra)))
		    return ReportMMError(hr);

		fUmbra = CastToFloat(varUmbra);

		if (FAILED(hr = pThis->get_Property(CComBSTR("LightPenumbra"), &varPenumbra)))
		    return ReportMMError(hr);

		fPenumbra = CastToFloat(varPenumbra);

		//convert degrees to radians here
		fUmbra = (fUmbra * .01745f);
		fPenumbra = (fPenumbra * .01745f);

		//get count of lights on this geometry and set the new spotlight characteristics on each of them

		if (FAILED(hr = pvwfFrame->GetLightCount(&lNumLights)))
			    return ReportMMError(hr);
		
		for (lIndex = 0;	lIndex<lNumLights;	lIndex++)
			if (FAILED(hr = pvwfFrame->SetSpotLightRadii(lIndex, fUmbra, fPenumbra)))
				    return ReportMMError(hr);
	}

	return hr;
}


STDMETHODIMP CMultimediaExemplarObject::OnLightTypeChanged(long lHint, VARIANT varHintData)
{
	CComPtr<IThing>     pThis;
	CComPtr<IVWFrame>	pvwfFrame;
	long		        lNumLights, lIndex; 
	long				lLightType;
	HRESULT hr = S_OK;

	if (lHint == thPut) 
    {
		if (FAILED(hr = m_pWorld->get_This(&pThis)))
			    return ReportMMError(hr);

        if (!pThis)
			    return ReportMMError(E_FAIL);

		if ((FAILED(hr = _GetFrameHelper(pThis, &pvwfFrame))) || (!pvwfFrame))
      		    return ReportMMError(hr);
      
		if (FAILED(hr = pThis->get_Long(CComBSTR("LightType"), &lLightType)))
			    return ReportMMError(hr);

		//get count of lights on this geometry and set the new LightType on each of them

		if (FAILED(hr = pvwfFrame->GetLightCount(&lNumLights)))
			    return ReportMMError(hr);

		for (lIndex = 0;	lIndex<lNumLights;	lIndex++)
			if (FAILED(hr = pvwfFrame->SetLightType(lIndex, lLightType)))
				    return ReportMMError(hr);
	}

	return hr;
}

STDMETHODIMP CMultimediaExemplarObject::OnLightRangeChanged(long lHint, VARIANT varHintData)
{

	CComPtr<IThing>     pThis;
	CComPtr<IVWFrame>	pvwfFrame;
	long		        lNumLights, lIndex; 
	CComVariant			varRange;
	float				fRange;
	HRESULT hr = S_OK;

	if (lHint == thPut) 
    {
		if (FAILED(hr = m_pWorld->get_This(&pThis)))
			    return ReportMMError(hr);

        if (!pThis)
		    return ReportMMError(E_FAIL);

		if ((FAILED(hr = _GetFrameHelper(pThis, &pvwfFrame))) || (!pvwfFrame))
      		    return ReportMMError(hr);
   
		if (FAILED(hr = pThis->get_Property(CComBSTR("LightRange"), &varRange)))
			    return ReportMMError(hr);

		fRange = CastToFloat(varRange);

		//get count of lights on this geometry and set the new Range on each of them

		if (FAILED(hr = pvwfFrame->GetLightCount(&lNumLights)))
			    return ReportMMError(hr);
		
		for (lIndex = 0;	lIndex<lNumLights;	lIndex++)
			if (FAILED(hr = pvwfFrame->SetLightRange(lIndex, fRange)))
				    return ReportMMError(hr);

	}

	return hr;
}


STDMETHODIMP CMultimediaExemplarObject::OnLightAttenuationChanged(long lHint, VARIANT varHintData)
{
	CComPtr<IThing>     pThis;
	CComPtr<IVWFrame>	pvwfFrame;
	CComPtr<IVector>	pvAttenuation;
	long		        lNumLights, lIndex; 
	float				fConstAttenuation, fLinAttenuation, fQuadAttenuation;

	HRESULT hr = S_OK;

	if ((lHint == thPut) || ((lHint >= 0) && (lHint <= 3)) )
    {
		if (FAILED(hr = m_pWorld->get_This(&pThis)))
				return ReportMMError(hr);
     
        if (!pThis)
			    return ReportMMError(E_FAIL);

		if ((FAILED(hr = _GetFrameHelper(pThis, &pvwfFrame))) || (!pvwfFrame))
      		    return ReportMMError(hr);

		if (FAILED(hr = pThis->get_ObjectProperty(CComBSTR ("LightAttenuation"), (IObjectProperty**)&pvAttenuation)))
				return ReportMMError(hr);
			
		if (!pvAttenuation)
		         return ReportMMError(E_FAIL);

		pvAttenuation->get(&fConstAttenuation, &fLinAttenuation, &fQuadAttenuation);

		//get count of lights on this geometry and set the new Attenuation on each of them

		if (FAILED(hr = pvwfFrame->GetLightCount(&lNumLights)))
			    return ReportMMError(hr);
		
		for (lIndex = 0;	lIndex<lNumLights;	lIndex++)
			if (FAILED(hr = pvwfFrame->SetLightAttenuation(lIndex, fConstAttenuation, fLinAttenuation, fQuadAttenuation)))
					    return ReportMMError(hr);
	}

	return hr;
}


STDMETHODIMP CMultimediaExemplarObject::OnLightEnabledChanged(long lHint, VARIANT varHintData)
{
	CComPtr<IThing>     pThis;
	CComPtr<IVWFrame>	pvwfFrame;
	CComPtr<IVector>	pvAttenuation;
	CComPtr<IVector>	pvLightColor;
	long		        lNumLights, lLightType;
	long				lIndex = 0l;
	VARIANT_BOOL		bLightEnabled;
	float				flRed, flGreen, flBlue, fRange, fUmbra, fPenumbra, fConstAttenuation, fLinAttenuation, fQuadAttenuation;
	CComVariant			varRange;
	CComVariant			varUmbra;
	CComVariant			varPenumbra;
	CComVariant			varLightName;
	HRESULT hr = S_OK;

	if (lHint == thPut) 
    {
		if (FAILED(hr = m_pWorld->get_This(&pThis)))
			    return ReportMMError(hr);

        if (!pThis)
			    return ReportMMError(E_FAIL);

		if ((FAILED(hr = _GetFrameHelper(pThis, &pvwfFrame))) || (!pvwfFrame))
      		    return ReportMMError(hr);
      
		if (FAILED(hr = pThis->get_Property(CComBSTR("LightRange"), &varRange)))
				return ReportMMError(hr);

		fRange = CastToFloat(varRange);

		if (FAILED(hr = pThis->get_Property(CComBSTR("LightUmbra"), &varUmbra)))
				return ReportMMError(hr);
       
		fUmbra = CastToFloat(varUmbra);

		if (FAILED(hr = pThis->get_Property(CComBSTR("LightPenumbra"), &varPenumbra)))
			    return ReportMMError(hr);

		fPenumbra = CastToFloat(varPenumbra);

		//convert degrees to radians here
		fUmbra = (fUmbra * .01745f);
		fPenumbra = (fPenumbra * .01745f);

		if (FAILED(hr = pThis->get_ObjectProperty(CComBSTR ("LightAttenuation"), (IObjectProperty**)&pvAttenuation)))
				return ReportMMError(hr);
			
		if (!pvAttenuation)
		         return ReportMMError(E_FAIL);

		pvAttenuation->get(&fConstAttenuation, &fLinAttenuation, &fQuadAttenuation);

		
		if (FAILED(hr = pThis->get_ObjectProperty(CComBSTR ("LightColor"), (IObjectProperty**)&pvLightColor)))
				return ReportMMError(hr);
			
		if (!pvLightColor)
		         return ReportMMError(E_FAIL);

		pvLightColor->get(&flRed, &flGreen, &flBlue);

		if (FAILED(hr = pThis->get_Long(CComBSTR("LightType"), &lLightType)))
				return ReportMMError(hr);

		if (FAILED(hr = pThis->get_BOOL(CComBSTR("LightEnabled"), &bLightEnabled)))
			    return ReportMMError(hr);

		if (FAILED(hr = pThis->get_Property(CComBSTR("Name"), &varLightName)))
				return ReportMMError(hr);

		if (FAILED(hr = pvwfFrame->GetLightCount(&lNumLights)))
			    return ReportMMError(hr);

		if (bLightEnabled == VARIANT_TRUE)
		{
			if (lNumLights < 1l)
			{
				if (FAILED(hr = pvwfFrame->CreateLight(varLightName)))
					return ReportMMError(hr);

				if (FAILED(hr = pvwfFrame->SetLightRange(lIndex, fRange)))
					return ReportMMError(hr);
			
				if (FAILED(hr = pvwfFrame->SetLightColor(lIndex, flRed, flGreen, flBlue)))
					return ReportMMError(hr);

				if (FAILED(hr = pvwfFrame->SetSpotLightRadii(lIndex, fUmbra, fPenumbra)))
					return ReportMMError(hr);
			
				if (FAILED(hr = pvwfFrame->SetLightAttenuation(lIndex, fConstAttenuation, fLinAttenuation, fQuadAttenuation)))
						return ReportMMError(hr);

				if (FAILED(hr = pvwfFrame->SetLightType(lIndex, lLightType)))
					return ReportMMError(hr);
			}
		}
		else
			for (lNumLights;	lNumLights>0;	lNumLights--)
				if (FAILED(hr = pvwfFrame->DeleteLight(lNumLights-1)))
				    return ReportMMError(hr);
	}

	return hr;
}


STDMETHODIMP CMultimediaExemplarObject::OnLightLoadGeometry(IVWFrame *pNewFrame)
{

		CComPtr<IThing>		pThis;
		CComPtr<IVWFrame>   pvwfFrame;
		CComPtr<IVector>	pvAttenuation;
		CComPtr<IVector>	pvLightColor;
		long		        lNumLights, lIndex, lLightType;
		VARIANT_BOOL		bLightEnabled;
		float				flRed, flGreen, flBlue, fRange, fUmbra, fPenumbra, fConstAttenuation, fLinAttenuation, fQuadAttenuation;
		CComVariant			varRange;
		CComVariant			varUmbra;
		CComVariant			varPenumbra;
		CComVariant			varLightName;
		CComBSTR			bstrName;

		HRESULT		hr = S_OK;

		if (FAILED(hr = m_pWorld->get_This(&pThis)))
				return ReportMMError(hr);

		if (!pThis)
			    return ReportMMError(E_FAIL);

		if ((FAILED(hr = _GetFrameHelper(pThis, &pvwfFrame))) || (!pvwfFrame) )
			    return ReportMMError(hr);
     
		if (FAILED(hr = pThis->get_BOOL(CComBSTR("LightEnabled"), &bLightEnabled)))
			    return ReportMMError(hr);

		if (bLightEnabled == VARIANT_TRUE)
		{

			if (FAILED(hr = pThis->get_Property(CComBSTR("LightRange"), &varRange)))
					return ReportMMError(hr);

			fRange = CastToFloat(varRange);

			if (FAILED(hr = pThis->get_Property(CComBSTR("LightUmbra"), &varUmbra)))
					return ReportMMError(hr);
       
			fUmbra = CastToFloat(varUmbra);

			if (FAILED(hr = pThis->get_Property(CComBSTR("LightPenumbra"), &varPenumbra)))
					return ReportMMError(hr);

			fPenumbra = CastToFloat(varPenumbra);

			//convert degrees to radians here
			fUmbra = (fUmbra * .01745f);
			fPenumbra = (fPenumbra * .01745f);

			if (FAILED(hr = pThis->get_ObjectProperty(CComBSTR ("LightAttenuation"), (IObjectProperty**)&pvAttenuation)))
					return ReportMMError(hr);
				
			if (!pvAttenuation)
					 return ReportMMError(E_FAIL);

			pvAttenuation->get(&fConstAttenuation, &fLinAttenuation, &fQuadAttenuation);

			if (FAILED(hr = pThis->get_ObjectProperty(CComBSTR ("LightColor"), (IObjectProperty**)&pvLightColor)))
					return ReportMMError(hr);
				
			if (!pvLightColor)
					 return ReportMMError(E_FAIL);

			pvLightColor->get(&flRed, &flGreen, &flBlue);

			if (FAILED(hr = pThis->get_Long(CComBSTR("LightType"), &lLightType)))
					return ReportMMError(hr);

			if (FAILED(hr = pThis->get_Property(CComBSTR("Name"), &varLightName)))
					return ReportMMError(hr);

			if (FAILED(hr = pvwfFrame->CreateLight(varLightName)))
					return ReportMMError(hr);

			if (FAILED(hr = pvwfFrame->GetLightCount(&lNumLights)))
					return ReportMMError(hr);

			for (lIndex = 0;	lIndex<lNumLights;	lIndex++)
			{	
					if (FAILED(hr = pvwfFrame->SetLightRange(lIndex, fRange)))
						return ReportMMError(hr);
			
					if (FAILED(hr = pvwfFrame->SetLightColor(lIndex, flRed, flGreen, flBlue)))
						return ReportMMError(hr);

					if (FAILED(hr = pvwfFrame->SetSpotLightRadii(lIndex, fUmbra, fPenumbra)))
						return ReportMMError(hr);
			
					if (FAILED(hr = pvwfFrame->SetLightAttenuation(lIndex, fConstAttenuation, fLinAttenuation, fQuadAttenuation)))
							return ReportMMError(hr);

					if (FAILED(hr = pvwfFrame->SetLightType(lIndex, lLightType)))
						return ReportMMError(hr);
			}
		}

		return S_OK;
}
