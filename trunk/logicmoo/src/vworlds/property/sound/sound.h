// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// ISound.h : Declaration of the CSoundObject

#ifndef __SOUND_H_
#define __SOUND_H_

#include "resource.h"       // main symbols
#include <PropBase.h>
#include <objimpl.h>
#include <isound.h>


EXTERN_C const IID LIBID_VWMMLib;
EXTERN_C const CLSID CLSID_Sound;
/////////////////////////////////////////////////////////////////////////////
// CSoundObject
DECLARE_OBJECTPROPERTY(Sound, LIBID_VWMMLib, IDS_SOUND_TYPE)
{
DECLARE_AGGREGATABLE(CSoundObject)

public:
	CSoundObject()
	{
		m_bstrSoundURL = "";
		m_bIsLooping = VARIANT_TRUE;
		m_bIs3D = VARIANT_TRUE;
		// 2D stuff
		m_lFrequency = 0;
		m_lVolume = 0;
		m_lPan = 0;
		// 3D Stuff
		m_dwInsideConeAngle = 360;
		m_dwOutsideConeAngle = 360;
		m_lConeOutsideVolume = 0;
		m_flMinDistance = 0.0f;
		m_flMaxDistance = 1000.0f;
	}

//DECLARE_REGISTRY_RESOURCEID(IDR_ISOUND)

BEGIN_COM_MAP(CSoundObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectProperty)
	COM_INTERFACE_ENTRY(ISound)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

DECLARE_REGISTRY(CSoundObject, _T("VWSYSTEM.Sound.1"), _T("VWSYSTEM.Sound"), IDS_SOUND_DESC, THREADFLAGS_BOTH)

// IISound
public:

	STDMETHOD(ToAscii)(BSTR* pbstr); 
	STDMETHOD(Marshall)(DWORD dwOptions, IMarshallBuffer* pbuffer); 
	STDMETHOD(UnMarshall)(IUnMarshallBuffer* pbuffer); 
	STDMETHOD(MarshallToScript)(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut);
	STDMETHOD(get_RefCount)(DWORD *pdwRef);

	STDMETHOD(get_SoundURL)(BSTR *pbstrURL);
	STDMETHOD(put_SoundURL)(BSTR bstrURL);
	STDMETHOD(get_IsLooping)(VARIANT_BOOL *pbIsLooping);
	STDMETHOD(put_IsLooping)(VARIANT_BOOL bIsLooping);
	STDMETHOD(get_Is3D)(VARIANT_BOOL *pbIs3D);
	STDMETHOD(put_Is3D)(VARIANT_BOOL bIs3D);
	STDMETHOD(Set2DData)(long lVolume, long lFrequency, long lPan);
	STDMETHOD(Get2DData)(VARIANT* plVolume,VARIANT* plFrequency, VARIANT* plPan);
	STDMETHOD(Get2DDataExt)(long* plVolume,long* plFrequency, long* plPan);
	
	STDMETHOD(Set3DData)(long dwInsideConeAngle,
					 long dwOutsideConeAngle,
					 long lConeOutsideVolume,
					 float flMinDistance,
					 float flMaxDistance);
	STDMETHOD(Get3DData)(VARIANT* pdwInsideConeAngle,
					 VARIANT* pdwOutsideConeAngle,
					 VARIANT* plConeOutsideVolume,
					 VARIANT* pflMinDistance,
					 VARIANT* pflMaxDistance);
	
	STDMETHOD(Get3DDataExt)(long* pdwInsideConeAngle,
					 long* pdwOutsideConeAngle,
					 long* plConeOutsideVolume,
					 float* pflMinDistance,
					 float* pflMaxDistance);

private:
			// sound support props
	CComBSTR m_bstrSoundURL;
	VARIANT_BOOL m_bIsLooping;
	VARIANT_BOOL m_bIs3D;
		// 2D stuff
	long m_lFrequency;
	long m_lVolume;
	long m_lPan;
		// 3D Stuff
	long m_dwInsideConeAngle;
	long m_dwOutsideConeAngle;
	long m_lConeOutsideVolume;
	float m_flMinDistance;
	float m_flMaxDistance;

	
};

#endif //__ISOUND_H_
