// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWSoundO.cpp : Implementation of CVWSoundObject

#include "stdafx.h"
#include <mmsystem.h>
#include <inetfile.h>
#include "VWSoundO.h"

EXTERN_C const CLSID CLSID_Sound3DBuffer;
EXTERN_C const CLSID CLSID_SoundBuffer;

CComBSTR CVWSoundObject::m_bstrInetfile;
						
#define ReportSoundError(hr) SUCCEEDED(hr) ? hr : ReportError("VWSYSTEM.Sound", IID_IVWSound, hr, 0x700, 0x800, )

/////////////////////////////////////////////////////////////////////////////
// ErrorInfo stuff

STDMETHODIMP CVWSoundObject::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IVWSound,
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

CVWSoundObject::CVWSoundObject() :
	m_pClient(NULL),
	m_pDSound(NULL),
	m_pDSListener(NULL),
	m_pDummyWnd(NULL)
{
	if ((BSTR)m_bstrInetfile == NULL)
	{
		m_bstrInetfile = "Inetfile";
	}
}

CVWSoundObject::~CVWSoundObject()
{
	Terminate();
}

/////////////////////////////////////////////////////////////////////////////
// IVWSound

STDMETHODIMP CVWSoundObject::Initialize(IVWClient *pClient)
{
	HRESULT hr = S_OK;
	DSBUFFERDESC dsbd = {0};
	IDirectSoundBuffer *pbuf = NULL;

	m_pClient = pClient;
	SAFEADDREF( m_pClient );

	hr = DirectSoundCreate( NULL, &m_pDSound, NULL );
	if (FAILED(hr))
	{
		TRACE("CVWSoundObject::Initialize: DirectSoundCreate failed, hr %x\n", hr);
		goto ERROR_ENCOUNTERED;
	}

	hr = SetCooperativeLevel(NULL, VWS_PRIORITY);
	if (FAILED(hr))
	{
		TRACE("CVWSoundObject::Initialize: SetCooperativeLevel failed, hr %x\n", hr);
		goto ERROR_ENCOUNTERED;
	}

	// create listener

	// first, create a primary buffer
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D;

	hr = m_pDSound->CreateSoundBuffer(&dsbd, &pbuf, NULL);
	if (FAILED(hr))
	{
		TRACE("CVWSoundObject::Initialize: IDirectSound::CreateSoundBuffer failed, hr %x\n", hr);
		goto ERROR_ENCOUNTERED;
	}

	// query for the listener interface
	hr = pbuf->QueryInterface( IID_IDirectSound3DListener, (void **) &m_pDSListener );
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// REVIEW: if we can't get 3D sound, should we still tolerate this?

ERROR_ENCOUNTERED:

	SAFERELEASE(pbuf);
	
	return hr;
}

STDMETHODIMP CVWSoundObject::SetCooperativeLevel( HWND hwnd, int nLevel )
{
	HRESULT hr = S_OK;
	DWORD dwLevel = 0;

	if (m_pDSound == NULL)
	{
		hr = VWSOUND_E_NOTINITIALIZED;
		goto ERROR_ENCOUNTERED;
	}

	// map nLevel into DSound values
	if (nLevel == VWS_EXCLUSIVE)
		dwLevel = DSSCL_EXCLUSIVE;
	else if (nLevel == VWS_NORMAL)
		dwLevel = DSSCL_NORMAL;
	else if (nLevel == VWS_PRIORITY)
		dwLevel = DSSCL_PRIORITY;
	else
	{
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	// if hwnd is null, use the currently active window
	if (hwnd == NULL)
	{
		CWnd *pwnd;

		// first, get the active window
		pwnd = CWnd::GetActiveWindow();

		// REVIEW: if this is null, what's the right thing to do?
		if (pwnd == NULL)
			pwnd = CWnd::GetDesktopWindow();
		else
			// find the topmost parent
			pwnd = pwnd->GetTopLevelParent();

		// create a dummy window using this as its parent
		m_pDummyWnd = new CWnd();
		if (m_pDummyWnd == NULL)
		{
			hr = E_OUTOFMEMORY;
			goto ERROR_ENCOUNTERED;
		}

		AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
		if( !m_pDummyWnd->CreateEx(	0L, 
									AfxRegisterWndClass(0), 
									_T("V-Worlds Dummy Sound Window"), 
									WS_CHILD, 
									0, 0, 
									0, 0, 
									pwnd->m_hWnd, 
									NULL))
		{
			ASSERT(FALSE);
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		m_pDummyWnd->SetActiveWindow();

		// now grab the handle from this
		hwnd = m_pDummyWnd->m_hWnd;
	}

	hr = m_pDSound->SetCooperativeLevel( hwnd, dwLevel );
	if (FAILED(hr))
	{
		TRACE("CVWSoundObject::SetCooperativeLevel: IDirectSound::SetCooperativeLevel failed, hr %x\n", hr);
	}

ERROR_ENCOUNTERED:

	return ReportSoundError(hr);
}

STDMETHODIMP CVWSoundObject::Create(BSTR bstrFile, ISoundBuffer **ppBuffer )
{
	HRESULT hr = S_OK;
	ISoundBuffer *pbuf = NULL;
	IDirectSoundBuffer *pdsb = NULL;
	CString strFile( bstrFile );

	// Cleanup old stuff
	VERIFY( SUCCEEDED(CleanUp()) );

	// init
	*ppBuffer = NULL;

	// load the file
	hr = LoadSoundBuffer( strFile, FALSE, &pdsb );
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// create a sound buffer object
	hr = CoCreateInstance( CLSID_SoundBuffer, NULL, CLSCTX_INPROC_SERVER, IID_ISoundBuffer, (void **) &pbuf );
	if (FAILED(hr))
	{
		TRACE("CVWSoundObject::Create: failed to create SoundBuffer\n");
		goto ERROR_ENCOUNTERED;
	}

	// set the DS buffer
	hr = pbuf->Initialize(this, bstrFile, (int)(void *)pdsb);
	if (FAILED(hr))
	{
		TRACE("CVWSoundObject::Create: ISoundBuffer::Initialize failed\n");
		goto ERROR_ENCOUNTERED;
	}

	// return
	*ppBuffer = pbuf;
	pbuf->AddRef();

ERROR_ENCOUNTERED:

	SAFERELEASE(pbuf);
	SAFERELEASE(pdsb);

	return ReportSoundError(hr);
}

STDMETHODIMP CVWSoundObject::Create3D(BSTR bstrFile, ISound3DBuffer **pp3DBuffer )
{
	HRESULT hr = S_OK;
	ISound3DBuffer *pbuf = NULL;
	IDirectSoundBuffer *pdsb = NULL;
	IDirectSound3DBuffer *pdsb3D = NULL;
	CString strFile(bstrFile);

	// Cleanup old stuff
	VERIFY( SUCCEEDED(CleanUp()) );

	if (pp3DBuffer == NULL)
	{
		hr = E_POINTER;
		goto ERROR_ENCOUNTERED;
	}

	// init
	*pp3DBuffer = NULL;

	// load the file
	hr = LoadSoundBuffer( strFile, TRUE, &pdsb );
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// get 3D interface
	hr = pdsb->QueryInterface( IID_IDirectSound3DBuffer, (void **) &pdsb3D );
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// create a 3D sound buffer object
	hr = CoCreateInstance( CLSID_Sound3DBuffer, NULL, CLSCTX_INPROC_SERVER, IID_ISound3DBuffer, (void **) &pbuf );
	if (FAILED(hr))
	{
		TRACE("CVWSoundObject::Create3D: failed to create Sound3DBuffer\n");
		goto ERROR_ENCOUNTERED;
	}

	// set the DS buffer
	hr = pbuf->Initialize(this, bstrFile, (int)(void *)pdsb);
	if (FAILED(hr))
	{
		TRACE("CVWSoundObject::Create3D: ISound3DBuffer::Initialize failed\n");
		goto ERROR_ENCOUNTERED;
	}

	// return
	*pp3DBuffer = pbuf;
	pbuf->AddRef();

ERROR_ENCOUNTERED:

	SAFERELEASE(pbuf);
	SAFERELEASE(pdsb);
	SAFERELEASE(pdsb3D);

	return ReportSoundError(hr);

}

STDMETHODIMP CVWSoundObject::Reload(BSTR bstrFile, int hBuffer)
{
	HRESULT hr = S_OK;
	CString str(bstrFile);
	IDirectSoundBuffer *pbuf = (IDirectSoundBuffer *) (void *) hBuffer;
	
	ASSERT(pbuf != NULL );
	hr = ReloadSoundBuffer( str, pbuf );

	return ReportSoundError(hr);
}

STDMETHODIMP CVWSoundObject::Play(ISoundBuffer *pbuffer, VARIANT_BOOL bLoop )
{
	HRESULT hr;

	// Cleanup old stuff
	VERIFY( SUCCEEDED(CleanUp()) );

	hr = pbuffer->Play(bLoop);

	if (SUCCEEDED(hr))
	{
		// add to list
		m_listPlaying.AddTail( pbuffer );
		pbuffer->AddRef();
	}

	return ReportSoundError(hr);
}

STDMETHODIMP CVWSoundObject::Stop(ISoundBuffer *pbuffer )
{
	HRESULT hr;

	hr = pbuffer->Stop();

	// Cleanup old stuff
	VERIFY( SUCCEEDED(CleanUp()) );

	return ReportSoundError(hr);
}

STDMETHODIMP CVWSoundObject::SetPosition(float fPosX, float fPosY, float fPosZ)
{
	HRESULT hr = S_OK;

	if (m_pDSListener == NULL)
	{
		hr = VWSOUND_E_NOTINITIALIZED;
		goto ERROR_ENCOUNTERED;
	}

	// set pos
	hr = m_pDSListener->SetPosition((D3DVALUE)fPosX, (D3DVALUE)fPosY, (D3DVALUE)fPosZ, DS3D_IMMEDIATE);
	if (FAILED(hr))
	{
		TRACE("CVWSoundObject::SetPosition: IDirectSound3DListener::SetPosition failed, hr %x\n", hr);
	}

ERROR_ENCOUNTERED:
	return ReportSoundError(hr);
}

STDMETHODIMP CVWSoundObject::SetOrientation(float fDirX, float fDirY, float fDirZ, float fUpX, float fUpY, float fUpZ)
{
	HRESULT hr = S_OK;

	if (m_pDSListener == NULL)
	{
		hr = VWSOUND_E_NOTINITIALIZED;
		goto ERROR_ENCOUNTERED;
	}

	hr = m_pDSListener->SetOrientation((D3DVALUE)fDirX, (D3DVALUE)fDirY, (D3DVALUE)fDirZ,
									     (D3DVALUE)fUpX, (D3DVALUE)fUpY, (D3DVALUE)fUpZ,
										 DS3D_IMMEDIATE);
	if (FAILED(hr))
	{
		TRACE("CVWSoundObject::SetOrientation: IDirectSound3DListener::SetOrientation failed, hr %x\n", hr);
	}

	// set pos
ERROR_ENCOUNTERED:
	return ReportSoundError(hr);
}

STDMETHODIMP CVWSoundObject::SetDistanceFactor( float flFactor )
{
	HRESULT hr = S_OK;

	if (m_pDSListener == NULL)
	{
		hr = VWSOUND_E_NOTINITIALIZED;
		goto ERROR_ENCOUNTERED;
	}

	hr = m_pDSListener->SetDistanceFactor( (D3DVALUE) flFactor, DS3D_IMMEDIATE );
	if (FAILED(hr))
	{
		TRACE("CVWSoundObject::SetDistanceFactor: IDirectSound3DListener::SetDistanceFactor failed, hr %x\n", hr);
	}

ERROR_ENCOUNTERED:
	return ReportSoundError(hr);
}

STDMETHODIMP CVWSoundObject::SetRolloffFactor( float flFactor )
{
	HRESULT hr = S_OK;

	if (m_pDSListener == NULL)
	{
		hr = VWSOUND_E_NOTINITIALIZED;
		goto ERROR_ENCOUNTERED;
	}

	hr = m_pDSListener->SetRolloffFactor( (D3DVALUE) flFactor, DS3D_IMMEDIATE );
	if (FAILED(hr))
	{
		TRACE("CVWSoundObject::SetRolloffFactor: IDirectSound3DListener::SetRolloffFactor failed, hr %x\n", hr);
	}

ERROR_ENCOUNTERED:
	return ReportSoundError(hr);
}

/////////////////////////////////////////////////////////////////////////////
// Internal helper functions

HRESULT CVWSoundObject::CleanUp()
{
	POSITION pos, posOld;

	// iterate through Playing list, seeing if any sounds are done
	for (pos=m_listPlaying.GetHeadPosition(); pos!=NULL; )
	{
		ISoundBuffer *psb;

		// get buffer
		posOld = pos;
		psb = m_listPlaying.GetNext(pos);
		ASSERT(psb);

		// see if sound expired
		int nStatus;
		VERIFY( SUCCEEDED(psb->GetStatus(&nStatus)) );
		if (!(nStatus & DSBSTATUS_PLAYING))
		{
			// yep - release and delete from list
			//psb->Release();
			SAFERELEASE( psb );

			m_listPlaying.RemoveAt(posOld);
		}
	}

	return S_OK;
}

HRESULT CVWSoundObject::LoadSoundBuffer( CString& strFile, BOOL b3D, IDirectSoundBuffer **ppbuf )
{
	HRESULT hr = S_OK;
	void *pvMapping = NULL;
	DSBUFFERDESC dsbd = {0};
	BYTE *pbWaveData;

	if (m_pDSound == NULL)
	{
		hr = VWSOUND_E_NOTINITIALIZED;
		goto ERROR_ENCOUNTERED;
	}

	// open the file
	hr = CreateMapping( strFile, &pvMapping );
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// parse it to make sure it's ok
	hr = ParseWAV(	pvMapping, 
					&dsbd.lpwfxFormat, 
					&pbWaveData, 
					&dsbd.dwBufferBytes );
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// create a sound buffer
	dsbd.dwSize = sizeof(dsbd);
	//dsbd.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLVOLUME | DSBCAPS_LOCSOFTWARE;
	dsbd.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLDEFAULT ;
	if (b3D)
		dsbd.dwFlags |= DSBCAPS_CTRL3D;

	try
	{
		hr = m_pDSound->CreateSoundBuffer( &dsbd, ppbuf, NULL );
		if (FAILED(hr))
		{
			TRACE("CVWSoundObject::LoadSoundBuffer: IDirectSound::CreateSoundBuffer failed, hr %x\n", hr);
			goto ERROR_ENCOUNTERED;
		}

	}
	catch(CException* e)
	{
		ASSERT(FALSE);

		// display error message
		char szError[256];
		if (e->GetErrorMessage(szError, sizeof(szError)))
			OutputDebugString(szError);

		e->Delete();

		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	catch(...)
	{
		ASSERT(FALSE);

		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	hr = FillBuffer( *ppbuf, pbWaveData, dsbd.dwBufferBytes);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// bump ref count of buffer since we want to keep it
	(*ppbuf)->AddRef();

ERROR_ENCOUNTERED:

	if (pvMapping != NULL )
		CloseMapping(pvMapping);

	if (*ppbuf != NULL)
		(*ppbuf)->Release();

	return hr;
}

HRESULT CVWSoundObject::ReloadSoundBuffer( CString& strFile, IDirectSoundBuffer *pbuf )
{
	HRESULT hr = S_OK;
	void *pvMapping = NULL;
	BYTE *pbWaveData;
	DWORD cbWaveSize;

	hr = CreateMapping( strFile, &pvMapping );
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// parse it to make sure it's ok
	hr = ParseWAV(	pvMapping, 
					NULL, 
					&pbWaveData, 
					&cbWaveSize );
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuf->Restore();
	if (FAILED(hr))
	{
		TRACE("CVWSoundObject::ReloadSoundBuffer: IDirectSoundBuffer::Restore failed, hr %x\n", hr);
		goto ERROR_ENCOUNTERED;
	}

	hr = FillBuffer( pbuf, pbWaveData, cbWaveSize );

ERROR_ENCOUNTERED:

	if (pvMapping != NULL )
		CloseMapping(pvMapping);

	return hr;
}

HRESULT CVWSoundObject::FillBuffer( IDirectSoundBuffer *pbuf, BYTE *pbWaveData, DWORD cbWaveSize )
{
	HRESULT hr = S_OK;
	void *pvMem1;
	DWORD cbMem1;
	void *pvMem2;
	DWORD cbMem2;

	if (pbuf == NULL || pbWaveData == NULL || cbWaveSize == 0)
	{
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}
 
	// lock the buffer
	hr = pbuf->Lock(0,
					cbWaveSize,
					&pvMem1,
					&cbMem1,
					&pvMem2,
					&cbMem2,
					0);
	if (FAILED(hr))
	{
		TRACE("CVWSoundObject::FillBuffer: IDirectSoundBuffer::Lock failed, hr %x\n", hr);
		goto ERROR_ENCOUNTERED;
	}

	// copy first part of buffer
	::CopyMemory(pvMem1, pbWaveData, cbMem1);

	// if necessary, copy second part
	if (cbMem2 != 0)
		::CopyMemory(pvMem2, pbWaveData + cbMem1, cbMem2);

	hr = pbuf->Unlock( pvMem1, cbMem1, pvMem2, cbMem2);
	if (FAILED(hr))
	{
		TRACE("CVWSoundObject::FillBuffer: IDirectSoundBuffer::Unlock failed, hr %x\n", hr);
	}

ERROR_ENCOUNTERED:

	return hr;
}

HRESULT CVWSoundObject::CreateMapping( CString& strFile, void **ppvMapping )
{
	HRESULT hr = S_OK;
	HANDLE hFile;
	HANDLE hMapping;
	IInternetFileManager *pInetfile = NULL;
	IWorld* pWorld = NULL;
	VARIANT_BOOL bFound;

	if (ppvMapping == NULL)
	{
		hr = E_POINTER;
		goto ERROR_ENCOUNTERED;
	}

	// initialize
	*ppvMapping = NULL;

	// open the file
	if ( m_pClient )
	{
		if (FAILED(hr = m_pClient->get_World(&pWorld)) || !pWorld)
			goto ERROR_ENCOUNTERED;

		hr = pWorld->get_Tool(m_bstrInetfile, (IUnknown**)&pInetfile);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		CComBSTR bstrURLPath, bstrFilePath;
		CComBSTR bstrFile = strFile;

		if (FAILED(hr = pInetfile->FindCachedFile(bstrFile, &bstrURLPath.m_str, &bstrFilePath.m_str, &bFound)) || !bFound)
		{
			if (FAILED(hr = pInetfile->GetFileEx(bstrFile, &bstrURLPath.m_str, &bstrFilePath.m_str))) 
			{
				m_pClient->Report(CComBSTR(CString("Failed to download file: ") + CString(bstrFile)), -1);
				goto ERROR_ENCOUNTERED;
			}
		}

		strFile = bstrFilePath;
	}

	// open the file
	hFile = CreateFile(	strFile, 
						GENERIC_READ, 
						FILE_SHARE_READ, 
						NULL, 
						OPEN_EXISTING, 
						0, 
						NULL);
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		hr = VWSOUND_E_ERRORMAPPINGFILE;
		goto ERROR_ENCOUNTERED;
	}

	hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if (hMapping == INVALID_HANDLE_VALUE) 
	{
		CloseHandle(hFile);
		hr = VWSOUND_E_ERRORMAPPINGFILE;
		goto ERROR_ENCOUNTERED;
	}

	CloseHandle (hFile);

	*ppvMapping = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
	if (*ppvMapping == NULL) 
	{
		CloseHandle(hMapping);
		hr = VWSOUND_E_ERRORMAPPINGFILE;
		goto ERROR_ENCOUNTERED;
	}

	CloseHandle (hMapping);

ERROR_ENCOUNTERED:

	SAFERELEASE(pInetfile);
	SAFERELEASE(pWorld);

	return hr;
}

HRESULT CVWSoundObject::ParseWAV(	void *			pvMapping,
							WAVEFORMATEX **	ppWaveFormat,
							BYTE **			ppbWaveData,
							DWORD *			pcbWaveSize )
{
	HRESULT hr = S_OK;
	DWORD *pdw;
	DWORD *pdwEnd;
	DWORD dwRiff;
	DWORD dwType;
	DWORD dwLength;

	if (ppWaveFormat)
		*ppWaveFormat = NULL;

	if (ppbWaveData)
		*ppbWaveData = NULL;

	if (pcbWaveSize)
		*pcbWaveSize = 0;

	pdw = (DWORD *)pvMapping;
	dwRiff = *pdw++;
	dwLength = *pdw++;
	dwType = *pdw++;

	if (dwRiff != mmioFOURCC('R', 'I', 'F', 'F'))
	{
		// not even RIFF
		hr = VWSOUND_E_INVALIDSOUNDFORMAT;
		goto ERROR_ENCOUNTERED;
	}

	if (dwType != mmioFOURCC('W', 'A', 'V', 'E'))
	{
		// not a WAV
		hr = VWSOUND_E_INVALIDSOUNDFORMAT;
		goto ERROR_ENCOUNTERED;
	}

	pdwEnd = (DWORD *)((BYTE *)pdw + dwLength-4);

	while (pdw < pdwEnd)
	{
		dwType = *pdw++;
		dwLength = *pdw++;

		switch (dwType)
		{
			case mmioFOURCC('f', 'm', 't', ' '):
				if (ppWaveFormat && !*ppWaveFormat)
				{
					if (dwLength < sizeof(WAVEFORMAT))
					{
						// not a proper WAV
						hr = VWSOUND_E_INVALIDSOUNDFORMAT;
						goto ERROR_ENCOUNTERED;
					}

					*ppWaveFormat = (WAVEFORMATEX *)pdw;

					if ((!ppbWaveData || *ppbWaveData) &&
						(!pcbWaveSize || *pcbWaveSize))
					{
						// done (no error)
						goto ERROR_ENCOUNTERED;
					}
				}
				break;

			case mmioFOURCC('d', 'a', 't', 'a'):
				if ((ppbWaveData && !*ppbWaveData) ||
					(pcbWaveSize && !*pcbWaveSize))
				{
					if (ppbWaveData)
						*ppbWaveData = (LPBYTE)pdw;

					if (pcbWaveSize)
						*pcbWaveSize = dwLength;

					if (!ppWaveFormat || *ppWaveFormat)
					{
						// done (no error)
						goto ERROR_ENCOUNTERED;
					}
				}
				break;
		}

		// advance to next 
		pdw = (DWORD *)((BYTE *)pdw + ((dwLength+1)&~1));
	}

ERROR_ENCOUNTERED:

	return hr;
}

HRESULT CVWSoundObject::CloseMapping( void *pvMapping )
{
	HRESULT hr = S_OK;

	if ( !UnmapViewOfFile( pvMapping ) )
		hr = VWSOUND_E_ERRORMAPPINGFILE;

	return hr;
}

STDMETHODIMP CVWSoundObject::Terminate()
{
	// TODO: Add your implementation code here
	POSITION pos;

	// iterate through Playing list, freeings sounds
	for (pos=m_listPlaying.GetHeadPosition(); pos!=NULL; )
	{
		ISoundBuffer *psb;

		// REVIEW: don't we need to stop 'em?

		// get buffer
		psb = m_listPlaying.GetNext(pos);
		ASSERT(psb);

		// release it
		SAFERELEASE( psb );;
	}

	// empty the list
	m_listPlaying.RemoveAll();

	SAFERELEASE(m_pDSListener);
	SAFERELEASE(m_pDSound);
	SAFERELEASE(m_pClient);

	// kill the dummy window
	if (m_pDummyWnd)
	{
		m_pDummyWnd->DestroyWindow();
		delete m_pDummyWnd;
		m_pDummyWnd = NULL;
	}

	return S_OK;
}
