// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWBufO.h : Declaration of CSoundBufferObject

#include <resource.h>

#include <mmreg.h>
#include <dsound.h>
#include <VWSound.h>

#include <propbase.h>

EXTERN_C const IID LIBID_VWMMLib;
EXTERN_C const CLSID CLSID_SoundBuffer;

/////////////////////////////////////////////////////////////////////////////
// CSoundBufferObject

class ATL_NO_VTABLE CSoundBufferObject :
	public IDispatchImpl<ISoundBuffer, &IID_ISoundBuffer, &LIBID_VWMMLib>,
	public ISupportErrorInfo,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CSoundBufferObject, &CLSID_SoundBuffer>
{
DECLARE_AGGREGATABLE(CSoundBufferObject)

public:
	CSoundBufferObject();
	~CSoundBufferObject();

BEGIN_COM_MAP(CSoundBufferObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISoundBuffer)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(CSoundBufferObject) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

DECLARE_REGISTRY(CSoundBufferObject, _T("VWSYSTEM.SoundBuffer.1"), _T("VWSYSTEM.SoundBuffer"), IDS_SOUNDBUFFER_DESC, THREADFLAGS_BOTH)

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// ISoundBuffer
public:
	STDMETHOD(Initialize)(IVWSound *psound, BSTR bstrFile, int nDSHandle);
	STDMETHOD(SetVolume)(int nVolume);
	STDMETHOD(SetPan)(int nPan);
	STDMETHOD(SetFrequency)(int nFrequency);
	STDMETHOD(Play)(VARIANT_BOOL bLoop);
	STDMETHOD(Stop)();
	STDMETHOD(GetStatus)(int *pnStatus);

protected:
	IVWSound *			m_psound;
	CString				m_strFile;
	IDirectSoundBuffer *m_pbuf;
};
