// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VW3DBufO.cpp : Implementation of CSound3DBufferObject

#include "stdafx.h"
#include <mmsystem.h>
#include "VW3DBufO.h"

/////////////////////////////////////////////////////////////////////////////
// ErrorInfo stuff

STDMETHODIMP CSound3DBufferObject::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ISound3DBuffer,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

CSound3DBufferObject::CSound3DBufferObject() :
	m_psound(NULL),
	m_pbuf(NULL),
	m_pbuf3D(NULL)
{
}

CSound3DBufferObject::~CSound3DBufferObject()
{
	SAFERELEASE(m_psound);
	SAFERELEASE(m_pbuf);
	SAFERELEASE(m_pbuf3D);
}

/////////////////////////////////////////////////////////////////////////////
// ISound3DBuffer stuff

STDMETHODIMP CSound3DBufferObject::Initialize(IVWSound *psound, BSTR bstrFile, int nDSHandle)
{
	HRESULT hr = S_OK;
	CString str(bstrFile);

	// stow
	ASSERT( psound != NULL);
	ASSERT( nDSHandle != 0);
	m_psound = psound;
	psound->AddRef();
	m_pbuf = (IDirectSoundBuffer *) (void *) nDSHandle;
	m_pbuf->AddRef();
	m_strFile = str;

	// now query for 3D interface
	hr = m_pbuf->QueryInterface( IID_IDirectSound3DBuffer, (void **) &m_pbuf3D);

	return S_OK;
}

STDMETHODIMP CSound3DBufferObject::SetVolume(int nVolume)
{
	ASSERT(m_pbuf);

	return m_pbuf->SetVolume((long) nVolume);
}

STDMETHODIMP CSound3DBufferObject::Play(VARIANT_BOOL bLoop)
{
	HRESULT hr = S_OK;

	hr = m_pbuf->Play( 0, 0, (bLoop == VARIANT_TRUE) ? DSBPLAY_LOOPING : 0);

	if (FAILED(hr) && hr == DSERR_BUFFERLOST )
	{
		hr = m_pbuf->Restore();
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = m_psound->Reload( CComBSTR(m_strFile), (int) (void *) m_pbuf );
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = m_pbuf->Play( 0, 0, (bLoop == VARIANT_TRUE) ? DSBPLAY_LOOPING : 0);
	}

ERROR_ENCOUNTERED:

	return hr;
}

STDMETHODIMP CSound3DBufferObject::Stop()
{
	return m_pbuf->Stop();
}

STDMETHODIMP CSound3DBufferObject::GetStatus(int *pnStatus)
{
	return m_pbuf->GetStatus((DWORD *) pnStatus);
}

STDMETHODIMP CSound3DBufferObject::SetConeAngles(int nInsideCone, int nOutsideCone )
{
	return m_pbuf3D->SetConeAngles( (DWORD) nInsideCone, (DWORD) nOutsideCone, DS3D_IMMEDIATE );
}

STDMETHODIMP CSound3DBufferObject::SetConeOrientation( IVector *pvector )
{
	HRESULT hr = S_OK;
	float x, y, z;

	// get scalars
	hr = pvector->get_x(&x);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pvector->get_y(&y);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pvector->get_z(&z);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pbuf3D->SetConeOrientation( (D3DVALUE) x, (D3DVALUE) y, (D3DVALUE) z, DS3D_IMMEDIATE );

ERROR_ENCOUNTERED:

	return hr;
}

STDMETHODIMP CSound3DBufferObject::SetConeOutsideVolume( int nVolume )
{
	return m_pbuf3D->SetConeOutsideVolume( (long) nVolume, DS3D_IMMEDIATE );
}

STDMETHODIMP CSound3DBufferObject::SetMaxDistance( float flDistance )
{
	return m_pbuf3D->SetMaxDistance( (D3DVALUE) flDistance, DS3D_IMMEDIATE );
}

STDMETHODIMP CSound3DBufferObject::SetMinDistance( float flDistance )
{
	return m_pbuf3D->SetMinDistance( (D3DVALUE) flDistance, DS3D_IMMEDIATE );
}

STDMETHODIMP CSound3DBufferObject::SetPosition( IVector *pvector )
{
	HRESULT hr = S_OK;
	float x, y, z;

	// get scalars
	hr = pvector->get_x(&x);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pvector->get_y(&y);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pvector->get_z(&z);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pbuf3D->SetPosition( (D3DVALUE) x, (D3DVALUE) y, (D3DVALUE) z, DS3D_IMMEDIATE );

ERROR_ENCOUNTERED:

	return hr;
}

