// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWSoundO.h : Declaration of the CVWSoundObject

#include <resource.h>

#include <afxtempl.h>

#include <mmreg.h>
#include <dsound.h>
#include <VWSound.h>

#include <propbase.h>

EXTERN_C const IID LIBID_VWMMLib;
EXTERN_C const CLSID CLSID_VWSound;

/////////////////////////////////////////////////////////////////////////////
// VWSound

class ATL_NO_VTABLE CVWSoundObject :
	public IDispatchImpl<IVWSound, &IID_IVWSound, &LIBID_VWMMLib>,
	public ISupportErrorInfo,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVWSoundObject, &CLSID_VWSound>
{
DECLARE_AGGREGATABLE(CVWSoundObject)

public:
	CVWSoundObject();
	~CVWSoundObject();

BEGIN_COM_MAP(CVWSoundObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWSound)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(CVWSoundObject) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

DECLARE_REGISTRY(CVWSoundObject, _T("VWSYSTEM.Sound.1"), _T("VWSYSTEM.Sound"), IDS_VWSOUND_DESC, THREADFLAGS_BOTH)

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IVWSound
public:
	STDMETHOD(Terminate)();
	STDMETHOD(Initialize)(IVWClient *pClient);
	STDMETHOD(SetCooperativeLevel)( HWND hWnd, int nLevel );

	STDMETHOD(Create)(BSTR bstrFile, ISoundBuffer **ppBuffer );
	STDMETHOD(Create3D)(BSTR bstrFile, ISound3DBuffer **pp3DBuffer );
	STDMETHOD(Reload)(BSTR bstrFile, int hBuffer);
	STDMETHOD(Play)(ISoundBuffer *pBuffer, VARIANT_BOOL bLoop );
	STDMETHOD(Stop)(ISoundBuffer *pBuffer );
	STDMETHOD(SetPosition)(float fPosX, float fPosY, float fPosZ);
	STDMETHOD(SetOrientation)(float fDirX, float fDirY, float fDirZ, float fUpX, float fUpY, float fUpZ);
	STDMETHOD(SetDistanceFactor)( float flFactor );
	STDMETHOD(SetRolloffFactor)( float flFactor );

protected:
	IVWClient *					m_pClient;
	LPDIRECTSOUND				m_pDSound;
	IDirectSound3DListener *	m_pDSListener;
	CWnd *						m_pDummyWnd;

	// list of sounds being played
	CList<ISoundBuffer *, ISoundBuffer *&>	m_listPlaying;

	// function to clean up expired sounds
	HRESULT CleanUp();

	// functions to load and reload sound buffers from a file
	HRESULT LoadSoundBuffer( CString& strFile, BOOL b3D, IDirectSoundBuffer **ppbuf );
	HRESULT ReloadSoundBuffer( CString& strFile, IDirectSoundBuffer *pbuf );

	// lower-level functions to manipulate .wav files
	HRESULT CreateMapping( CString& strFile, void **ppvMapping );
	HRESULT ParseWAV(	void *			pvMapping,
						WAVEFORMATEX **	ppWaveFormat,
						BYTE **			ppbWaveData,
						DWORD *			pcbWaveSize );
	HRESULT FillBuffer( IDirectSoundBuffer *pbuf, BYTE *pbWaveData, DWORD cbWaveSize );
	HRESULT CloseMapping( void *pvMapping );

public:
	static CComBSTR m_bstrInetfile;
};
