// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// ISound.cpp : Implementation of CSoundObject
#include "stdafx.h"
#include <vwobject.h>

#include "Sound.h"

IMPLEMENT_OBJECTPROPERTY(CSoundObject)


STDMETHODIMP CSoundObject::get_SoundURL(BSTR *pbstrURL)
{
	ASSERT ( pbstrURL != NULL );

	*pbstrURL = m_bstrSoundURL.Copy();
	return S_OK;
}

STDMETHODIMP CSoundObject::put_SoundURL(BSTR bstrURL)
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	// Do a quick compare to see if it the same, if so return
	if (CompareElements(&m_bstrSoundURL.m_str, &bstrURL))
		return hr;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CSoundObject::put_SoundURL\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CSoundObject::put_SoundURL", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CSoundObject::put_SoundURL: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		m_bstrSoundURL = bstrURL;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstrURL;

		hr = SendObjectPropertyCommand(psbits, DISPID_SOUND_URL, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, soundURL, CComVariant(bstrURL));
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}
STDMETHODIMP CSoundObject::get_IsLooping(VARIANT_BOOL *pbIsLooping)
{
	ASSERT( pbIsLooping != NULL );

	*pbIsLooping = m_bIsLooping;
	return S_OK;
}
STDMETHODIMP CSoundObject::put_IsLooping(VARIANT_BOOL bIsLooping)
{

	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	//if ( bIsLooping == m_bIsLooping )
	//	return hr;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CSoundObject::put_IsLooping\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CSoundObject::put_IsLooping", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CSoundObject::put_IsLooping: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		m_bIsLooping = bIsLooping;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BOOL;
		dispparams.rgvarg[0].boolVal = bIsLooping;

		hr = SendObjectPropertyCommand(psbits, DISPID_SOUND_ISLOOPING, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, soundIsLooping, CComVariant((bool)bIsLooping));
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}
STDMETHODIMP CSoundObject::get_Is3D(VARIANT_BOOL *pbIs3D)
{
	ASSERT (pbIs3D != NULL );
	*pbIs3D = m_bIs3D;
	return S_OK;
}
STDMETHODIMP CSoundObject::put_Is3D(VARIANT_BOOL bIs3D)
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	if ( bIs3D == m_bIs3D )
		return hr;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CSoundObject::put_Is3D\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CSoundObject::put_Is3D", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CSoundObject::put_Is3D: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		m_bIs3D = bIs3D;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BOOL;
		dispparams.rgvarg[0].boolVal = bIs3D;

		hr = SendObjectPropertyCommand(psbits, DISPID_SOUND_IS3D, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, soundIs3D, CComVariant((bool)bIs3D));
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}
STDMETHODIMP CSoundObject::Set2DData(long lVolume, long lFrequency, long lPan)
{
		HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CSoundObject::Set2DData\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CSoundObject::Set2DData", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CSoundObject::Set2DData: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		m_lVolume = lVolume;
		m_lFrequency = lFrequency;
		m_lPan = lPan;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 3);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[2].vt = VT_I4;
		dispparams.rgvarg[2].lVal = lVolume;
		dispparams.rgvarg[1].vt = VT_I4;
		dispparams.rgvarg[1].lVal = lFrequency;
		dispparams.rgvarg[0].vt = VT_I4;
		dispparams.rgvarg[0].lVal = lPan;

		hr = SendObjectPropertyCommand(psbits, DISPID_SOUND_SET2DDATA, DISPATCH_METHOD, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, sound2DData, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CSoundObject::Get2DData(VARIANT* pvarlVolume,VARIANT* pvarlFrequency, VARIANT* pvarlPan)
{
	HRESULT hr = S_OK;

	if (( !pvarlVolume ) ||
		( !pvarlFrequency ) ||
		( !pvarlPan))
	{
		hr = E_POINTER;
		goto ERROR_ENCOUNTERED;
	}

	::VariantClear( pvarlVolume ); pvarlVolume->vt = VT_I4;
	::VariantClear( pvarlFrequency );pvarlFrequency->vt = VT_I4;
	::VariantClear( pvarlPan);pvarlPan->vt = VT_I4;

	hr = Get2DDataExt( &(pvarlVolume->lVal), &(pvarlFrequency->lVal), &(pvarlPan->lVal) );


ERROR_ENCOUNTERED:
	return hr;
}
STDMETHODIMP CSoundObject::Get2DDataExt(long* plVolume,long* plFrequency, long* plPan)
{

	ASSERT(plVolume != NULL);
	ASSERT(plFrequency!= NULL);
	ASSERT(plPan!= NULL);

	*plVolume = m_lVolume;
	*plFrequency = m_lFrequency;
	*plPan = m_lPan;
	return S_OK;
}


STDMETHODIMP CSoundObject::Set3DData(long dwInsideConeAngle,
					 long dwOutsideConeAngle,
					 long lConeOutsideVolume,
					 float flMinDistance,
					 float flMaxDistance)
{
		HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CSoundObject::Set3DData\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CSoundObject::Set3DData", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CSoundObject::Set3DData: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		m_dwInsideConeAngle = dwInsideConeAngle;
		m_dwOutsideConeAngle = dwOutsideConeAngle;
		m_lConeOutsideVolume = lConeOutsideVolume;
		m_flMinDistance = flMinDistance;
		m_flMaxDistance = flMaxDistance;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 5);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[4].vt = VT_I4;
		dispparams.rgvarg[4].lVal = dwInsideConeAngle;
		dispparams.rgvarg[3].vt = VT_I4;
		dispparams.rgvarg[3].lVal = dwOutsideConeAngle;
		dispparams.rgvarg[2].vt = VT_I4;
		dispparams.rgvarg[2].lVal = lConeOutsideVolume;
		dispparams.rgvarg[1].vt = VT_R4;
		dispparams.rgvarg[1].fltVal = flMinDistance;
		dispparams.rgvarg[0].vt = VT_R4;
		dispparams.rgvarg[0].fltVal = flMaxDistance;

		hr = SendObjectPropertyCommand(psbits, DISPID_SOUND_SET3DDATA, DISPATCH_METHOD, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, sound3DData, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CSoundObject::Get3DData(VARIANT * pdwInsideConeAngle,
					 VARIANT* pdwOutsideConeAngle,
					 VARIANT* plConeOutsideVolume,
					 VARIANT* pflMinDistance,
					 VARIANT* pflMaxDistance)
{

	HRESULT hr = S_OK;

	if (( !pdwInsideConeAngle ) ||
		( !pdwOutsideConeAngle ) ||
		( !plConeOutsideVolume) ||
		( !pflMinDistance) ||
		( !pflMaxDistance ))
	{
		hr = E_POINTER;
		goto ERROR_ENCOUNTERED;
	}

	::VariantClear( pdwInsideConeAngle ); pdwInsideConeAngle->vt = VT_I4;
	::VariantClear( pdwOutsideConeAngle );pdwOutsideConeAngle->vt = VT_I4;
	::VariantClear( plConeOutsideVolume);plConeOutsideVolume->vt = VT_I4;
	::VariantClear( pflMinDistance ); pflMinDistance->vt = VT_R4;
	::VariantClear( pflMaxDistance ); pflMaxDistance->vt = VT_R4;

	hr = Get3DDataExt( &(pdwInsideConeAngle->lVal), &(pdwOutsideConeAngle->lVal), &(plConeOutsideVolume->lVal), &(pflMinDistance->fltVal), &(pflMaxDistance->fltVal) );


ERROR_ENCOUNTERED:
	return hr;
}


STDMETHODIMP CSoundObject::Get3DDataExt(long* pdwInsideConeAngle,
					 long* pdwOutsideConeAngle,
					 long* plConeOutsideVolume,
					 float* pflMinDistance,
					 float* pflMaxDistance)
{
	ASSERT( pdwInsideConeAngle != NULL );
	ASSERT( pdwOutsideConeAngle != NULL );
	ASSERT( plConeOutsideVolume != NULL );
	ASSERT( pflMinDistance != NULL );
	ASSERT( pflMaxDistance != NULL );

	*pdwInsideConeAngle = m_dwInsideConeAngle;
	*pdwOutsideConeAngle = m_dwOutsideConeAngle;
	*plConeOutsideVolume = m_lConeOutsideVolume;
	*pflMinDistance = m_flMinDistance;
	*pflMaxDistance = m_flMaxDistance;
	return S_OK;
}

STDMETHODIMP CSoundObject::ToAscii(BSTR* pbstr)
{
	ASSERT(pbstr);

	CString strValue;
	strValue.Format("SoundURL: %s, isLooping: %s, is3D: %s", 
		CString(m_bstrSoundURL), 
		m_bIsLooping == VARIANT_TRUE ?"True":"False", 
		m_bIs3D == VARIANT_TRUE ?"True":"False");

	*pbstr = strValue.AllocSysString();

	return S_OK;
}

STDMETHODIMP CSoundObject::Marshall(DWORD dwOptions, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = CSoundObjectBase::Marshall( dwOptions, pbuffer )))
		return hr;

	ASSERT(pbuffer);

	hr = pbuffer->put_String(m_bstrSoundURL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->put_Boolean(m_bIsLooping);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->put_Boolean(m_bIs3D);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	 
	// 2D stuff
	hr = pbuffer->put_Int(m_lFrequency);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
	hr = pbuffer->put_Int(m_lVolume);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->put_Int(m_lPan);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED; 
	 
	// 3D Stuff

	hr = pbuffer->put_Int(m_dwInsideConeAngle);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	 

	hr = pbuffer->put_Int(m_dwOutsideConeAngle);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	 

	hr = pbuffer->put_Int(m_lConeOutsideVolume);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	 

	hr = pbuffer->put_Float(m_flMinDistance);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	 

	hr = pbuffer->put_Float(m_flMaxDistance);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	 

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CSoundObject::MarshallToScript(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut)
{
	CString strOut;
	CString strOut1;
	CString strOut2;
	CString strOut3;

	if (bstrContext==NULL || pbstrOut==NULL || pbAsVal==NULL)
		return E_INVALIDARG;

	strOut1.Format(	"\tSet %s = World.CreateSound(\"%s\", %s, %s)\n",
					(LPCTSTR) CString(bstrContext),
					 CString(m_bstrSoundURL), 
					 m_bIsLooping == VARIANT_TRUE ?"True":"False", 
					 m_bIs3D == VARIANT_TRUE ?"True":"False");
	
	strOut2.Format(	"\t%s.Set2DData %d, %d, %d\n",
					(LPCTSTR) CString(bstrContext),
					 m_lVolume, m_lFrequency, m_lPan);

	strOut3.Format(	"\t%s.Set3DData %d, %d, %d, %2.1f, %2.1f \n",
					(LPCTSTR) CString(bstrContext),
					 m_dwInsideConeAngle,
					 m_dwOutsideConeAngle,
					 m_lConeOutsideVolume,
					 m_flMinDistance,
					 m_flMaxDistance);
	strOut = strOut1 + strOut2 + strOut3;
	*pbstrOut = strOut.AllocSysString();
	*pbAsVal = VARIANT_FALSE;

	return S_OK;
}


STDMETHODIMP CSoundObject::UnMarshall(IUnMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = CSoundObjectBase::UnMarshall( pbuffer )))
		return hr;

	ASSERT(pbuffer);

	m_bstrSoundURL = (BSTR)NULL;
	hr = pbuffer->get_String( &m_bstrSoundURL.m_str);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->get_Boolean(&m_bIsLooping);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->get_Boolean(&m_bIs3D);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	 
	// 2D stuff
	hr = pbuffer->get_Int((int*)&m_lFrequency);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
	hr = pbuffer->get_Int((int*)&m_lVolume);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->get_Int((int*)&m_lPan);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED; 
	 
	// 3D Stuff

	hr = pbuffer->get_Int((int*)&m_dwInsideConeAngle);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	 

	hr = pbuffer->get_Int((int*)&m_dwOutsideConeAngle);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	 

	hr = pbuffer->get_Int((int*)&m_lConeOutsideVolume);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	 

	hr = pbuffer->get_Float(&m_flMinDistance);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	 

	hr = pbuffer->get_Float(&m_flMaxDistance);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	 
ERROR_ENCOUNTERED:
	return hr;
}

/////////////////////////////////////////////////////////////////////////////
// CSoundObject


