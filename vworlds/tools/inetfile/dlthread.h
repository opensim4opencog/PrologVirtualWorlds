// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// DLThread.h : Declaration of the CDLThread

#ifndef __DLTHREAD_H_
#define __DLTHREAD_H_

#include "resource.h"       // main symbols
#include <afxtempl.h>
#include <atlctl.h>
#include <COMDEF.H>
#include <wininet.h>
#include <propbase.h>
#include "inetfile.h"

EXTERN_C const CLSID CLSID_DLThread;

typedef CMap<CString, LPCSTR,IDLThread *,IDLThread *> ThreadMapType;

//////////////////////////////////////////////////////////////////////////////////
// threadsafe map of filename to Thread
//////////////////////////////////////////////////////////////////////////////////
class CThreadMap : private ThreadMapType
{
public:
	CThreadMap();
	~CThreadMap();
	
	IDLThread *	Find(CString strURL);

	HRESULT	Add(CString strURL, CString strRootURL, long lCookie, 
		IInternetFileManager *pInetFile, IIFMCallback *pCallback);
	
	BOOL		Remove(CString strURL);
	
	CString		Dump();	

private:
	void GetNextAssocThreadSafe( POSITION& rNextPosition, CString& rKey, IDLThread * & rValue ) 
	{
		EnterCriticalSection(&m_critsec);		
		CMap<CString, LPCSTR, IDLThread *,IDLThread *>::GetNextAssoc(rNextPosition,rKey,rValue );
		LeaveCriticalSection(&m_critsec);	
	}
	
	void		RemoveAll();
	
	LONG		GetCount() {return ThreadMapType::GetCount();}
	
	POSITION	GetStartPosition() {return ThreadMapType::GetStartPosition();}
//data
	
	CRITICAL_SECTION m_critsec;

};

//////////////////////////////////////////////////////////////////////////////////
//Queue of URLS. not necessary to be threadsafe. but should do anyway.
//////////////////////////////////////////////////////////////////////////////////
class CURLQueue : public CList< CString, CString& >
{ 
public:
    // Go to the end of the line    
	void put(CString URL) { AddTail( URL); }      

    // Get first element in line    
	BOOL get(CString &URL)
    { 
		if(IsEmpty()) return FALSE;
		URL = RemoveHead();
		return TRUE;
	} 
};

class CallbackType
{
public:
	CallbackType(): m_pInetFile(NULL), m_pCallback(NULL) {}

	CallbackType& operator=(const CallbackType& in)
	{
		Set(in.m_pInetFile, in.m_pCallback, in.m_lCookie);
		return *this;
	}
	
	CallbackType(const CallbackType &in)
	{
		Set(in.m_pInetFile, in.m_pCallback, in.m_lCookie);
	}

	~CallbackType()
	{
		SAFERELEASE(m_pInetFile);
		SAFERELEASE(m_pCallback);
	}

	void Set(IInternetFileManager *pInetFile, IIFMCallback *pCallback, long lCookie)
	{
		SAFERELEASE(m_pInetFile);
		SAFERELEASE(m_pCallback);

		m_pInetFile = pInetFile;
		SAFEADDREF(m_pInetFile);

		m_pCallback = pCallback;
		SAFEADDREF(m_pCallback);

		m_lCookie = lCookie;
	}

	void Get(IInternetFileManager **ppInetFile, IIFMCallback **ppCallback, long * plCookie)
	{
		ASSERT(m_pInetFile);
//		ASSERT(m_pCallback);

		*ppInetFile = m_pInetFile;
		SAFEADDREF(m_pInetFile);

		*ppCallback = m_pCallback;
		SAFEADDREF(m_pCallback);

		*plCookie = m_lCookie;
	}

private:
	IInternetFileManager *m_pInetFile;
	long m_lCookie;
	IIFMCallback *m_pCallback;
};

/////////////////////////////////////////////////////////////////////////////
// CDLThread
/////////////////////////////////////////////////////////////////////////////
class ATL_NO_VTABLE CDLThread : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CDLThread, &CLSID_DLThread>,
	public IConnectionPointContainerImpl<CDLThread>,
	public IDispatchImpl<IDLThread, &IID_IDLThread, &LIBID_VWSYSTEMLib>,
	public IBindStatusCallback
{
public:
	CDLThread() : 
		m_pMoniker(NULL), m_pBindContext(NULL), m_pBinding(NULL),m_pStream(NULL),
		m_bCacheFileAvail(FALSE)
	{
		InitializeCriticalSection(&m_critsec);		
	}

	~CDLThread()
	{
		SAFERELEASE(m_pMoniker);
		SAFERELEASE(m_pBindContext);
		SAFERELEASE(m_pBinding);
		SAFERELEASE(m_pStream);

		DeleteCriticalSection(&m_critsec);
	}
	
DECLARE_REGISTRY_RESOURCEID(IDR_DLTHREAD)

BEGIN_COM_MAP(CDLThread)
	COM_INTERFACE_ENTRY(IDLThread)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IBindStatusCallback)
END_COM_MAP()

//use this later for callback. right now, just pass pointer to yourself.
BEGIN_CONNECTION_POINT_MAP(CDLThread)
END_CONNECTION_POINT_MAP()

// IDLThread
public:
	STDMETHOD(Download)(BSTR bstrFilename, BSTR bstrRootURL);

	STDMETHOD(AddCallback)(IInternetFileManager *pInetFile, IIFMCallback *pCallback, long lCookie) {
		EnterCriticalSection(&m_critsec);	
			CallbackType callback;
			callback.Set(pInetFile, pCallback, lCookie);
			m_listCallback.AddTail(callback);
		LeaveCriticalSection(&m_critsec);	
		return S_OK;
	}

	STDMETHOD(Terminate)()
	{
		EnterCriticalSection(&m_critsec);	
			SAFERELEASE(m_pMoniker);
			SAFERELEASE(m_pBindContext);
			SAFERELEASE(m_pBinding);
			SAFERELEASE(m_pStream);

/* Not necessary. Selfdestructing.
			POSITION pos;
			CallbackType currCallback;

			//iterate through list cleaning up.
			pos = m_listCallback.GetHeadPosition();
			while( pos != NULL )
			{
				currCallback = m_listCallback.GetNext( pos );
				currCallback->Release();
			}
*/
		LeaveCriticalSection(&m_critsec);		
		return S_OK;
	}

// IBindStatusCallback
public:
    STDMETHOD(OnStartBinding)(DWORD dwReserved, IBinding* pbinding);

    STDMETHOD(GetPriority)(LONG* pnPriority); 
    
	STDMETHOD(OnLowResource)(DWORD dwReserved) {return E_NOTIMPL;}
    
	STDMETHOD(OnProgress)(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode,
                        LPCWSTR pwzStatusText);
    
	STDMETHOD(OnStopBinding)(HRESULT hrResult, LPCWSTR szError);
    
	STDMETHOD(GetBindInfo)(DWORD* pgrfBINDF, BINDINFO* pbindinfo);
    
	STDMETHOD(OnDataAvailable)(DWORD grfBSCF, DWORD dwSize, FORMATETC *pfmtetc,
                        STGMEDIUM* pstgmed);
    
	STDMETHOD(OnObjectAvailable)(REFIID riid, IUnknown* punk) {return E_NOTIMPL;}

private:
	HRESULT ProcessURLFromQueue();
	HRESULT DoCallbacks(HRESULT hrRet, CString strFinalPath, CString strFilename);

	//data////////////////////////////////////////////////////////

	IMoniker * m_pMoniker;
	IBindCtx * m_pBindContext;
    IBinding* m_pBinding;
	IStream * m_pStream;
	DWORD m_dwTotalRead;
	DWORD m_dwAvailableToRead;
	CURLQueue m_qURLs;
	CString m_strFilename;
	CString m_strCacheFilename;
	BOOL m_bCacheFileAvail;
	CList<CallbackType,CallbackType&> m_listCallback;	
	CRITICAL_SECTION m_critsec;
	DWORD m_dwTick;
};

#endif //__DLTHREAD_H_
