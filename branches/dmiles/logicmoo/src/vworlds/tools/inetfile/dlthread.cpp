// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// DLThread.cpp : Implementation of CDLThread
#include "stdafx.h"
#include "DLThread.h"
#include "urlhelp.h"
//this file only for str().
#include "queue.h"
#include <io.h>

#ifdef _DEBUG
//#define DLTHREAD_DEBUG
//#define DLTHREAD_THREADMAP_DEBUG
#endif

typedef CComObject<CDLThread> CComDLThread;

/////////////////////////////////////////////////////////////////////////////
// CDLThread

HRESULT CDLThread::DoCallbacks(HRESULT hrRet, CString strFinalPath, CString strFilename)
{
	POSITION pos;
	CallbackType currCallback;
	long lCurrCookie;
	IInternetFileManager *pCurrInetFile = NULL;
	IIFMCallback *pCurrCallback = NULL;
	HRESULT hr = S_OK;

	//iterate through list calling back fcns.
	pos = m_listCallback.GetHeadPosition();
	while( pos != NULL )
	{
		currCallback = m_listCallback.GetNext( pos );
		currCallback.Get(&pCurrInetFile, &pCurrCallback, &lCurrCookie);

		if(pCurrCallback)
		{
			hr = pCurrCallback->OnDone_T2(hrRet, CComBSTR(strFinalPath), lCurrCookie, CComBSTR(strFilename));
			if (FAILED(hr))
			{
				ASSERT(0);
				goto ERROR_ENCOUNTERED;    
			}			
		}
		
		ASSERT(pCurrInetFile);			
		if(pCurrInetFile)
		{
			hr = pCurrInetFile->FileDLDone(CComBSTR(m_strFilename));
			if (FAILED(hr))
			{
				ASSERT(0);
				goto ERROR_ENCOUNTERED;    
			}
		}

		SAFERELEASE(pCurrInetFile);
		SAFERELEASE(pCurrCallback);
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pCurrInetFile);
	SAFERELEASE(pCurrCallback);
	
	return hr;
}

HRESULT CDLThread::ProcessURLFromQueue()
{
	EnterCriticalSection(&m_critsec);	
	
	IMoniker * pMoniker = NULL;
	IBindCtx * pBindContext = NULL;
	IStream * pStream = NULL;	
    HRESULT hr = S_OK;
	CString strURL;
	BOOL bRetVal;

	//make sure last dl is finished.
	ASSERT(!m_pMoniker);

	//get url from queue.
	bRetVal = m_qURLs.get(strURL);
	if(bRetVal == FALSE) 
	{
#ifdef DLTHREAD_DEBUG
		afxDump<<"CDLThread::ProcessURLFromQueue: File: "<<m_strFilename<<". NOT FOUND."<<"\n";
#endif

		//TODO NOT FOUND.
		hr = E_FAIL;
	    goto ERROR_ENCOUNTERED;;
	}

	//check if it's a file url.
	strURL.MakeLower();
	if(strURL.Left(7)=="file://")
	{
		CString temp;
		temp = strURL.Right(strURL.GetLength()-7);
		if(_access(temp,0)!=-1)
		{
			hr = DoCallbacks(S_OK, temp, m_strFilename);
			if (FAILED(hr))
			{
				ASSERT(0);
				goto ERROR_ENCOUNTERED;    
			}
		}
		else
		{
			hr = ProcessURLFromQueue();
			if (FAILED(hr))
			{
#ifdef DLTHREAD_DEBUG
				afxDump<<"CDLThread::ProcessURLFromQueue: File: "<<m_strFilename<<". FAILED to dl'ed to: "<<strURL<<"\n";
#endif

				hr = DoCallbacks(E_FAIL, "", m_strFilename);
				if (FAILED(hr))
				{
					ASSERT(0);
					goto ERROR_ENCOUNTERED;
				}
			}
		}
	}
	else
	{
		//start download.
		m_dwTotalRead = 0;
		m_dwAvailableToRead = 0;
		
		hr = CreateURLMoniker(NULL, CComBSTR(strURL), &pMoniker);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		//hr = CreateBindCtx(0, &pBindContext);
		hr = CreateAsyncBindCtx(0, this, NULL, &pBindContext);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = RegisterBindStatusCallback(pBindContext,
				this, 0, 0L);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		//set's off thread.
		hr = pMoniker->BindToStorage(pBindContext, 0, IID_IStream, (void**)&pStream);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;    

		ASSERT(SUCCEEDED(hr));

		//store new info.
		m_pMoniker = pMoniker;
		SAFEADDREF(m_pMoniker);

		m_pBindContext = pBindContext;
		SAFEADDREF(m_pBindContext);
		
		//not using a stream right now.
		ASSERT(!pStream);
		
		m_pStream = pStream;
		SAFEADDREF(m_pStream);
	}
	
ERROR_ENCOUNTERED:
	SAFERELEASE(pStream);
	SAFERELEASE(pBindContext);
	SAFERELEASE(pMoniker);

	LeaveCriticalSection(&m_critsec);	
    
	return hr;
}

/////////////////////////////////////////////////////////////////////////////
// IDLThread

STDMETHODIMP CDLThread::Download(BSTR bstrFilename, BSTR bstrRootURL)
{
	EnterCriticalSection(&m_critsec);	

	CString strFullURL;
	HRESULT hr;	
	BOOL bretval = TRUE;
	CString URLLeft, ExtractedURL;

	//todo errors.
	ASSERT(bstrRootURL);
	ASSERT(bstrFilename);

	bretval = IsFullURL(bstrFilename);
	if(bretval)
	{
		m_strFilename = CString(bstrFilename);
		m_qURLs.put(m_strFilename);
	}
	else
	{
		m_strFilename = CString(bstrFilename);

		//extract root url from string and put into queue.
		URLLeft = CString(bstrRootURL);	

		bretval = TRUE;
		while (bretval)
		{
			bretval = ExtractFirstURL(URLLeft, ExtractedURL);
			if (ExtractedURL.GetLength() != 0)
			{
				strFullURL = ExtractedURL + "/" + CString(bstrFilename);
				m_qURLs.put(strFullURL);
			}
		}
	}

	//kick it off.
	hr = ProcessURLFromQueue();

	LeaveCriticalSection(&m_critsec);	
	
	return hr;
}

/////////////////////////////////////////////////////////////////////////////
// CBindStatusCallback

STDMETHODIMP CDLThread::OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC* pfmtetc, STGMEDIUM* pstgmed)
{
//	EnterCriticalSection(&m_critsec);	
	
	HRESULT hr = S_OK;

#ifdef DLTHREAD_DEBUG
	//afxDump<<"CDLThread::OnDataAvailable: File: "<<m_strFilename<<". Bytes: "<<dwSize<<"."<<"\n";
#endif

//	LeaveCriticalSection(&m_critsec);	
    return hr;
}  // CBindStatusCallback::OnDataAvailable

STDMETHODIMP CDLThread::OnStartBinding(DWORD dwReserved, IBinding* pbinding)
{
	EnterCriticalSection(&m_critsec);	
	
	//this is executed in the main thread.
	ASSERT(!m_pBinding);

	m_dwTick = GetTickCount();


    m_pBinding = pbinding;
	SAFEADDREF(m_pBinding);
	
	m_dwTotalRead = 0;
	m_dwAvailableToRead = 0;

#ifdef DLTHREAD_DEBUG
	afxDump<<"CDLThread::OnStartBinding: File: "<<m_strFilename<<". Start Binding."<<"\n";
#endif

	LeaveCriticalSection(&m_critsec);	
    
	return S_OK;
}  // CBindStatusCallback::OnStartBinding

STDMETHODIMP CDLThread::GetPriority(LONG* pnPriority)
{
//	EnterCriticalSection(&m_critsec);	
	
	*pnPriority = THREAD_PRIORITY_LOWEST; 
	
//	LeaveCriticalSection(&m_critsec);	
	return S_OK;
}
    
STDMETHODIMP CDLThread::OnStopBinding(HRESULT hrStatus, LPCWSTR pszError)
{
	EnterCriticalSection(&m_critsec);	
	
	HRESULT hr = S_OK;

    if (hrStatus)
	{
#ifdef DLTHREAD_DEBUG
		afxDump<<"CDLThread::OnStopBinding: File: "<<m_strFilename<<". Failed DL."<<"\n";
#endif
	}
	
	//check state
	ASSERT(m_pBinding);
	//ASSERT(m_pStream);
	ASSERT(m_pBindContext);
	ASSERT(m_pMoniker);

	//release everything.
	SAFERELEASE(m_pBinding);
	SAFERELEASE(m_pStream);
	SAFERELEASE(m_pBindContext);
	SAFERELEASE(m_pMoniker);

	if(m_bCacheFileAvail==TRUE)
	{
		//call it's finished.
		#ifdef DLTHREAD_DEBUG
				afxDump<<"CDLThread::OnStopBinding: File: "<<m_strFilename<<". Has successfully dl'ed to: "<<m_strCacheFilename<<"\n";

				DWORD dwNow = GetTickCount();
				CString strTemp;
			
				strTemp.Format("Download URL succeeded. Tick count now: %d. File name: %s. Time elapsed: %d\n", dwNow, m_strFilename, dwNow - m_dwTick);
				OutputDebugString((LPCTSTR)strTemp);
		#endif

		hr = DoCallbacks(S_OK, m_strCacheFilename, m_strFilename);
		if (FAILED(hr))
		{
			ASSERT(0);
			goto ERROR_ENCOUNTERED;    
		}
	}
	else
	{
		hr = ProcessURLFromQueue();
		if (FAILED(hr))
		{
			#ifdef DLTHREAD_DEBUG
			afxDump<<"CDLThread::OnStopBinding: File: "<<m_strFilename<<". FAILED to dl'ed to: "<<m_strCacheFilename<<"\n";
			DWORD dwNow = GetTickCount();
			CString strTemp;
		
			strTemp.Format("Download URL failed. Tick count now: %d. File name: %s. Time elapsed: %d\n", dwNow, m_strFilename, dwNow - m_dwTick);
			OutputDebugString((LPCTSTR)strTemp);
			#endif

			hr = DoCallbacks(E_FAIL, "", m_strFilename);
			if (FAILED(hr))
			{
				ASSERT(0);
				goto ERROR_ENCOUNTERED;    
			}
		}
	}

ERROR_ENCOUNTERED:

	LeaveCriticalSection(&m_critsec);	

    return hr;
}  // CBindStatusCallback::OnStopBinding

STDMETHODIMP CDLThread::OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusText)
{	
//	EnterCriticalSection(&m_critsec);	

	if(szStatusText!=NULL)
	{
		if(ulProgressMax)
		{

			double p = double(ulProgress) / double(ulProgressMax) * 100.00;
#ifdef DLTHREAD_DEBUG
			//afxDump<<"CDLThread::OnProgress: File: "<<m_strFilename<<". Progress: "<<(int)p<<"%."<<"\n";
#endif

//			if(m_pCallback)
			{
				//Don't need this. need to write another docallback fcn just for this.
//				m_pCallback->OnProgress_T2(ulProgress, ulProgressMax, m_lCookie);
				//ignore hr 
			}
		}

		if(BINDSTATUS_CACHEFILENAMEAVAILABLE == ulStatusCode)
		{
			m_strCacheFilename = CString(szStatusText);
			m_bCacheFileAvail = TRUE;
#ifdef DLTHREAD_DEBUG
			//afxDump<<"DLTHREAD: File: "<<m_strFilename<<". Status text: "<<CString(szStatusText)<<"\n";
#endif
		}
    }

//	LeaveCriticalSection(&m_critsec);	

	return S_OK;
}  // CBindStatusCallback::OnProgress

STDMETHODIMP CDLThread::GetBindInfo(DWORD* pgrfBINDF, BINDINFO* pbindInfo)
{
//	EnterCriticalSection(&m_critsec);	

    *pgrfBINDF = BINDF_ASYNCHRONOUS | BINDF_ASYNCSTORAGE ;
    *pgrfBINDF |= BINDF_GETNEWESTVERSION | BINDF_NEEDFILE;
    pbindInfo->cbSize = sizeof(BINDINFO);
    pbindInfo->szExtraInfo = NULL;
    memset(&pbindInfo->stgmedData, 0, sizeof(STGMEDIUM));
    pbindInfo->grfBindInfoF = 0;
    pbindInfo->dwBindVerb = BINDVERB_GET;
    pbindInfo->szCustomVerb = NULL;
    
//	LeaveCriticalSection(&m_critsec);	

	return S_OK;
}  // CBindStatusCallback::GetBindInfo

/////////////////////////////////////////////////////////////////////////////
// CThreadMap

CThreadMap::CThreadMap()
{
	InitializeCriticalSection(&m_critsec);		
}

CThreadMap::~CThreadMap()
{
	RemoveAll();

	DeleteCriticalSection(&m_critsec);
}

CString CThreadMap::Dump()
{
	EnterCriticalSection(&m_critsec);	
	
	CString output;
	
	output+="Number open threads: "+str(GetCount())+"\r\n";
	POSITION pos;
	
	for (pos = GetStartPosition(); pos != NULL; )
	{
		CString strKey;
		IDLThread *pvValue;

		GetNextAssoc(pos, strKey, pvValue);
		output+="    URL: \""+strKey+"\"\r\n";
	}
	
	LeaveCriticalSection(&m_critsec);	
	return output;
}

void CThreadMap::RemoveAll()
{
	CString strKey;
	IDLThread *pvValue;

	// iterate through map, freeing 
	EnterCriticalSection(&m_critsec);	

	POSITION pos;
		
	for (pos=GetStartPosition(); pos!=NULL; )
	{

		GetNextAssoc(pos, strKey, pvValue);
		ASSERT( pvValue != NULL );

		SAFERELEASE(pvValue);
	}

	ThreadMapType::RemoveAll();
	
	LeaveCriticalSection(&m_critsec);	
}

IDLThread* CThreadMap::Find(CString strURL)
{
	EnterCriticalSection(&m_critsec);	

	IDLThread *pvValue;
	if (Lookup(strURL, pvValue))
	{
		LeaveCriticalSection(&m_critsec);			
		return pvValue;
	}
	else
	{
		LeaveCriticalSection(&m_critsec);	
		return NULL;
	}
}

//add thread. does everything.
HRESULT CThreadMap::Add(CString strURL, CString strRootURL, long lCookie, 
						IInternetFileManager *pInetFile, IIFMCallback *pCallback)
{
	EnterCriticalSection(&m_critsec);		
	
	IDLThread *pThread = NULL;
	HRESULT hr = S_OK;

	IDLThread * pFoundThread = Find(strURL);
	if(NULL == pFoundThread)
	{
		pThread = new CComDLThread;
		ASSERT(pThread);
		SAFEADDREF(pThread);

		if (pThread == NULL)
		{
			hr = E_OUTOFMEMORY;
			goto ERROR_ENCOUNTERED;
		}

		//see callback too.
		SetAt(strURL, pThread);
		
		//for map.
		SAFEADDREF(pThread);
		
		
	#ifdef DLTHREAD_THREADMAP_DEBUG
		afxDump <<"CThreadMap::Add: The thread map has "<< GetCount()<<" entries now, after the addition of: "<<strURL<<", Cookie: "<<lCookie<<".\n";
	#endif

		hr = pThread->AddCallback(pInetFile, pCallback, lCookie);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;    
			
		hr= pThread->Download(CComBSTR(strURL), CComBSTR(strRootURL));
		if (FAILED(hr))
				goto ERROR_ENCOUNTERED;    
	}
	else
	{
		//no addref.
		pFoundThread->AddCallback(pInetFile, pCallback, lCookie);
	}

ERROR_ENCOUNTERED:	
	SAFERELEASE(pThread);
	
	LeaveCriticalSection(&m_critsec);	
	
	return hr;
}

BOOL CThreadMap::Remove(CString strURL)
{
	EnterCriticalSection(&m_critsec);	
	
	IDLThread *pwi = Find(strURL);
	
	if (pwi != NULL)
	{
		RemoveKey(strURL);
		pwi->Terminate();
		SAFERELEASE(pwi);
	}
	
	if(IsEmpty())
	{
#ifdef DLTHREAD_THREADMAP_DEBUG
		afxDump <<"CThreadMap::Remove: The thread map is empty now, after the deletion of: "<<strURL<<"."<<"\n";
#endif
	}

#ifdef DLTHREAD_THREADMAP_DEBUG
	afxDump <<"CThreadMap::Remove: The thread map has "<< GetCount()<<" entries now, after the removal of: "<<strURL<<"."<<"\n";
#endif

	LeaveCriticalSection(&m_critsec);	
	
	return pwi != NULL;
}

