// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWBufO.cpp : Implementation of CSoundBufferObject

#include "stdafx.h"
#include <mmsystem.h>
#include "VWBufO.h"

/////////////////////////////////////////////////////////////////////////////
// ErrorInfo stuff

STDMETHODIMP CSoundBufferObject::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ISoundBuffer,
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

CSoundBufferObject::CSoundBufferObject() :
	m_psound(NULL),
	m_pbuf(NULL)
{
}

CSoundBufferObject::~CSoundBufferObject()
{
	SAFERELEASE(m_psound);
	SAFERELEASE(m_pbuf);
}

/////////////////////////////////////////////////////////////////////////////
// ISoundBuffer

STDMETHODIMP CSoundBufferObject::Initialize(IVWSound *psound, BSTR bstrFile, int nDSHandle)
{
	CString str(bstrFile);

	// stow handle
	ASSERT( nDSHandle != 0 );
	ASSERT( psound != NULL );
	m_pbuf = (IDirectSoundBuffer *) (void *) nDSHandle;
	m_pbuf->AddRef();
	m_psound = psound;
	psound->AddRef();

	m_strFile = str;

	return S_OK;;
}

STDMETHODIMP CSoundBufferObject::SetVolume(int nVolume)
{
	ASSERT(m_pbuf);

	return m_pbuf->SetVolume((long) nVolume);
}

STDMETHODIMP CSoundBufferObject::SetPan(int nPan)
{
	ASSERT(m_pbuf);

	return m_pbuf->SetPan((long) nPan);
}

STDMETHODIMP CSoundBufferObject::SetFrequency(int nFrequency)
{
	ASSERT(m_pbuf);

	return m_pbuf->SetFrequency((DWORD) nFrequency);
}


STDMETHODIMP CSoundBufferObject::Play(VARIANT_BOOL bLoop)
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

STDMETHODIMP CSoundBufferObject::Stop()
{
	return m_pbuf->Stop();
}

STDMETHODIMP CSoundBufferObject::GetStatus(int *pnStatus)
{
	return m_pbuf->GetStatus((DWORD *) pnStatus);
}