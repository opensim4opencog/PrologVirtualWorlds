// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// InetMgrO.cpp : Implementation of CInetFileApp and DLL registration.

#include "stdafx.h"
#include <InetFile.h>
#include <io.h>
#include <urlmon.h>
#include "InetMgrO.h"
#include "reghelp.h"
#include "urlhelp.h"

//#define INETFILE_PERF_DEBUG
//#define INETFILE_DOWNLOAD_DEBUG

// not defined in new INetSDK for some reason
#ifndef MAX_CACHE_ENTRY_INFO_SIZE
#define MAX_CACHE_ENTRY_INFO_SIZE       4096
#endif

/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP CInternetFileManagerObject::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IInternetFileManager,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

CInternetFileManagerObject::~CInternetFileManagerObject()
{
	Terminate();
}

STDMETHODIMP CInternetFileManagerObject::Terminate()
{
	return S_OK;
}

STDMETHODIMP CInternetFileManagerObject::FindCachedFile(BSTR bstrURL, 
		BSTR *pbstrFinalURL, BSTR *pbstrFinalPath, VARIANT_BOOL *pbool)
{
	CString strFullURL,strFilename;
	long index=0;
	ULONG length;
	char cinfo[MAX_CACHE_ENTRY_INFO_SIZE];
	HRESULT hr=S_OK;

	if (bstrURL == NULL)
	{
		TRACE("CInternetFileManagerObject::FindCachedFile: incoming argument (BSTR) is NULL\n");
		return ReportInetFileError(E_INVALIDARG);
	}

	if (pbstrFinalPath == NULL)
	{
		TRACE("CInternetFileManagerObject::FindCachedFile: outgoing pointer (BSTR*) is NULL\n");
		return ReportInetFileError(E_POINTER);
	}

	if (pbool == NULL)
	{
		TRACE("CInternetFileManagerObject::FindCachedFile: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return ReportInetFileError(E_POINTER);
	}

	*pbstrFinalPath = NULL;
	*pbstrFinalURL = NULL;
	*pbool = VARIANT_FALSE;

	strFullURL = CString(bstrURL);

	if (strFullURL.IsEmpty())
		return ReportInetFileError(hr);	

	if (IsFullURL(bstrURL))
	{
		length = sizeof(cinfo);
		
		//find a cache name.
		if (RetrieveUrlCacheEntryFile(strFullURL,(INTERNET_CACHE_ENTRY_INFO *)&cinfo, &length, 0))
		{
			UnlockUrlCacheEntryFile(strFullURL, 0);
			strFilename = CString(((INTERNET_CACHE_ENTRY_INFO *)&cinfo)->lpszLocalFileName);		
			*pbstrFinalPath = strFilename.AllocSysString();
			*pbstrFinalURL = strFullURL.AllocSysString();
			*pbool = VARIANT_TRUE;
		}
		else if (strFullURL.Left(7).CompareNoCase("file://") == 0 && _access(strFullURL.Mid(7), 0) == 0)
		{
			*pbstrFinalPath = strFullURL.Mid(7).AllocSysString();
			*pbstrFinalURL = strFullURL.AllocSysString();
			*pbool = VARIANT_TRUE;
		}
	}
	else
	{
		if (m_RootURLs.GetSize() == 0)
		{
			TRACE("CInternetFileManagerObject::FindCachedFile: root URL list is empty\n");
			return ReportInetFileError(INETFILE_E_INVALIDROOTURL);
		}

		for (index=0; index<m_RootURLs.GetSize(); index++)
		{
			hr=S_OK;
			strFullURL = m_RootURLs[index] + "/" + CString(bstrURL);
			length = sizeof(cinfo);
			
			//find a cache name.
			if (RetrieveUrlCacheEntryFile(strFullURL,(INTERNET_CACHE_ENTRY_INFO *)&cinfo, &length, 0))
			{
				UnlockUrlCacheEntryFile(strFullURL, 0);
				strFilename = CString(((INTERNET_CACHE_ENTRY_INFO *)&cinfo)->lpszLocalFileName);
				*pbstrFinalPath = strFilename.AllocSysString();
				*pbstrFinalURL = strFullURL.AllocSysString();
				*pbool = VARIANT_TRUE;
				break;
			}
			else if (strFullURL.Left(7).CompareNoCase("file://") == 0 && _access(strFullURL.Mid(7), 0) == 0)
			{
				*pbstrFinalPath = strFullURL.Mid(7).AllocSysString();
				*pbstrFinalURL = strFullURL.AllocSysString();
				*pbool = VARIANT_TRUE;
				break;
			}
		}
	}
	
	return ReportInetFileError(hr);	
}

void CInternetFileManagerObject::TestHelper(CString blah)
{
#ifdef DLTHREAD_DEBUG

	BSTR bstrFinalURL;
	BSTR bstrFinalPath;
	HRESULT hr;
	
	hr = GetFile(CComBSTR(blah), FALSE, &bstrFinalURL, 
		&bstrFinalPath);
	
	if(FAILED(hr))
		afxDump<<"Getfile failed for: "<<blah<<"\n";

	SAFEFREESTRING(bstrFinalURL); 
	SAFEFREESTRING(bstrFinalPath);

#endif
}

STDMETHODIMP CInternetFileManagerObject::Test()
{
	HRESULT hr = S_OK;
	static long g_TestCookie = 0;
	

	//async test
	#ifdef DLTHREAD_DEBUG
			DWORD dwNow = GetTickCount();
			CString strTemp;
		
			strTemp.Format("Async test starting. Tick count is ,%d\n", dwNow);
			OutputDebugString((LPCTSTR)strTemp);
	#endif

	// Do a bunch of GetFileAsync's on known files here.

	//sync test
	#ifdef DLTHREAD_DEBUG
		dwNow = GetTickCount();

		strTemp.Format("\nSync test starting. Tick count is ,%d\n", dwNow);
		OutputDebugString((LPCTSTR)strTemp);
	#endif

	// Invoke TestHelper a bunch of times with known files here.

	return hr;
}

STDMETHODIMP CInternetFileManagerObject::GetFile(BSTR bstrURL, VARIANT_BOOL bCheckCache, BSTR *pbstrFinalPath)
{
	CComBSTR	tempBSTR;

	return GetFileEx(bstrURL, bCheckCache, &tempBSTR.m_str, pbstrFinalPath);
}

STDMETHODIMP CInternetFileManagerObject::GetFileEx(BSTR bstrURL, VARIANT_BOOL bCheckCache, BSTR *pbstrFinalURL, BSTR *pbstrFinalPath)
{
	HRESULT hr=S_OK;
	VARIANT_BOOL bInCache;

	if(bCheckCache==VARIANT_TRUE)
	{
		hr = FindCachedFile(bstrURL, pbstrFinalURL, pbstrFinalPath, &bInCache);
		if(FAILED(hr) || (bInCache==VARIANT_FALSE))
			//continue on.
			hr = S_OK; 
		else
			return hr;
	}
	else
	{
		hr = FindCachedFile(bstrURL, pbstrFinalURL, pbstrFinalPath, &bInCache);
		if(FAILED(hr) || (bInCache==VARIANT_FALSE))
			//continue on.
			hr = S_OK; 
		else
		{
			//Delete file in cache.
			DeleteUrlCacheEntry(CString(*pbstrFinalURL));
		}
	}

	//getfile implementation
	
	if (bstrURL == NULL)
	{
		TRACE("CInternetFileManagerObject::GetFile: incoming argument (BSTR) is NULL\n");
		return ReportInetFileError(E_INVALIDARG);
	}

	if (pbstrFinalPath == NULL)	
	{
		TRACE("CInternetFileManagerObject::GetFile: outgoing pointer (BSTR*) is NULL\n");
		return ReportInetFileError(E_POINTER);
	}

	if (pbstrFinalURL == NULL)	
	{
		TRACE("CInternetFileManagerObject::GetFile: outgoing pointer (BSTR*) is NULL\n");
		return ReportInetFileError(E_POINTER);
	}

	*pbstrFinalPath = NULL;
	*pbstrFinalURL = NULL;

#ifdef _DEBUG
//	TRACE("CInternetFileManagerObject::GetFile: attempting to retrieve %s\n", CString(bstrURL));
#endif

	hr = GetFileSimplified(bstrURL, pbstrFinalURL, pbstrFinalPath);
	
	return ReportInetFileError(hr);
}

//simplified version. slower, checks for server-side date.
HRESULT CInternetFileManagerObject::GetFileSimplified(BSTR bstrURL, BSTR *pbstrFinalURL, 
													  BSTR *pbstrFinalPath)
{
	char szPath[INTERNET_MAX_PATH_LENGTH];
	HRESULT hr = S_OK;
	CString strFullURL,strFilename,strExtension;
	long index = 0;
#ifdef _DEBUG
	DWORD dwTick;
#endif

	if (bstrURL == NULL)
	{
		TRACE("CInternetFileManagerObject::GetFileSimplified: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	if (pbstrFinalPath == NULL)	
	{
		TRACE("CInternetFileManagerObject::GetFileSimplified: outgoing pointer (BSTR*) is NULL\n");
		return E_POINTER;
	}

	*pbstrFinalPath = NULL;

	strFullURL = CString(bstrURL);

	//check if is fullurl.
	if (IsFullURL(bstrURL))
	{
#ifdef _DEBUG
		dwTick = GetTickCount();
#endif

		hr = URLDownloadToCacheFile(NULL, strFullURL, szPath, INTERNET_MAX_PATH_LENGTH, 0 /*BINDF_GETNEWESTVERSION*/, NULL);

		if (SUCCEEDED(hr))
		{
			strFilename = CString(szPath);

#ifdef _DEBUG
#ifdef DLTHREAD_DEBUG
			TRACE("CInternetFileManagerObject::GetFileSimplified: successfully downloaded %s as %s, duration %3.2f sec\n", strFullURL, strFilename, (float)(GetTickCount() - dwTick) / 1000.0);
#endif

#ifdef DLTHREAD_DEBUG
			DWORD dwNow = GetTickCount();
			CString strTemp;
			
			strTemp.Format("Download URL (succeeded),%d,%s,%d\n", dwNow, strFullURL, dwNow - dwTick);
			OutputDebugString((LPCTSTR)strTemp);
#endif
#endif

			*pbstrFinalURL = strFullURL.AllocSysString();
			*pbstrFinalPath = strFilename.AllocSysString();
		}
		else
		{
#ifdef _DEBUG
#ifdef INETFILE_DOWNLOAD_DEBUG
			TRACE("CInternetFileManagerObject::GetFileSimplified: failed to download %s, hr %x, duration %3.2f sec\n", strFullURL, (float)(GetTickCount() - dwTick) / 1000.0);
#endif

#ifdef DLTHREAD_DEBUG
			DWORD dwNow = GetTickCount();
			CString strTemp;
			
			strTemp.Format("Download URL (failed),%d,%s,%d\n", dwNow, strFullURL, dwNow - dwTick);
			OutputDebugString((LPCTSTR)strTemp);
#endif
#endif

			hr = INETFILE_E_FILENOTFOUND;
		}
	}
	else
	{
		if (m_RootURLs.GetSize() == 0)
		{
			TRACE("CInternetFileManagerObject::GetFileSimplified: root URL list is empty\n");
			return INETFILE_E_INVALIDROOTURL;
		}

		//iterate through each url;
		for (index=0; index<m_RootURLs.GetSize(); index++)
		{
			strFullURL = m_RootURLs[index] + "/" + CString(bstrURL);

#ifdef _DEBUG
			dwTick = GetTickCount();
#endif
			
			hr = URLDownloadToCacheFile(NULL, strFullURL, szPath, INTERNET_MAX_PATH_LENGTH, 0 /*BINDF_GETNEWESTVERSION*/, NULL);

			if (SUCCEEDED(hr)) 
			{
				strFilename = CString(szPath);
			
#ifdef _DEBUG
#ifdef DLTHREAD_DEBUG
				TRACE("CInternetFileManagerObject::GetFileSimplified: successfully downloaded %s as %s, duration %3.2f sec\n", strFullURL, strFilename, (float)(GetTickCount() - dwTick) / 1000.0);
#endif

#ifdef DLTHREAD_DEBUG
				DWORD dwNow = GetTickCount();
				CString strTemp;
			
				strTemp.Format("Download URL (succeeded),%d,%s,%d\n", dwNow, strFullURL, dwNow - dwTick);
				OutputDebugString((LPCTSTR)strTemp);
#endif
#endif

				*pbstrFinalURL = strFullURL.AllocSysString();
				*pbstrFinalPath = strFilename.AllocSysString();

				break;
			}
			else
			{
#ifdef _DEBUG
#ifdef INETFILE_DOWNLOAD_DEBUG
				TRACE("CInternetFileManagerObject::GetFileSimplified: failed to download %s, duration %3.2f sec\n", strFullURL, (float)(GetTickCount() - dwTick) / 1000.0);
#endif

#ifdef DLTHREAD_DEBUG
				DWORD dwNow = GetTickCount();
				CString strTemp;
			
				strTemp.Format("Download URL (failed),%d,%s,%d\n", dwNow, strFullURL, dwNow - dwTick);
				OutputDebugString((LPCTSTR)strTemp);
#endif
#endif

				hr = INETFILE_E_FILENOTFOUND;
			}
		}
	}
	
	return hr;
}

//fast. doesn't check for server-side date.
HRESULT CInternetFileManagerObject::GetFileComplex(BSTR bstrURL, BSTR *pbstrFinalURL, 
												   BSTR *pbstrFinalPath)
{
	char szPath[INTERNET_MAX_PATH_LENGTH];
	BOOL bRetVal;
	char cinfo[MAX_CACHE_ENTRY_INFO_SIZE];
	HRESULT hr = S_OK;
	CString strFullURL, strFilename, strExtension;
	ULONG length;
	long index=0;
#ifdef _DEBUG
	DWORD dwTick = 0;
#endif
	
	if (bstrURL == NULL)
	{
		TRACE("CInternetFileManagerObject::GetFileComplex: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	if (pbstrFinalPath == NULL)	
	{
		TRACE("CInternetFileManagerObject::GetFileComplex: outgoing pointer (BSTR*) is NULL\n");
		return E_POINTER;
	}

	*pbstrFinalPath = NULL;

	//extract extension
	_splitpath(CString(bstrURL), NULL, NULL, NULL, szPath);
	strExtension = CString(szPath);
	strExtension = strExtension.Right(strExtension.GetLength()-1);

	strFullURL = bstrURL;

	//check if is fullurl.
	if (IsFullURL(bstrURL))
	{
#ifdef _DEBUG
		dwTick = GetTickCount();
#endif

		hr = URLDownloadToCacheFile(NULL, strFullURL, szPath, INTERNET_MAX_PATH_LENGTH, 0 /*BINDF_GETNEWESTVERSION*/, NULL);

		if (SUCCEEDED(hr))
		{
			strFilename = CString(szPath);
			
#ifdef _DEBUG
			TRACE("CInternetFileManagerObject::GetFileComplex: successfully downloaded %s as %s, duration %3.2f sec\n", strFullURL, strFilename, (float)(GetTickCount() - dwTick) / 1000.0);
#endif

			*pbstrFinalURL = strFullURL.AllocSysString();
			*pbstrFinalPath = strFilename.AllocSysString();
		}
		else
		{
#ifdef _DEBUG
//			TRACE("CInternetFileManagerObject::GetFileComplex: failed to download %s, hr %x, duration %3.2f sec\n", strFullURL, (float)(GetTickCount() - dwTick) / 1000.0);
#endif

			hr = INETFILE_E_FILENOTFOUND;
		}
	}
	else
	{
		if (m_RootURLs.GetSize() == 0)
		{
			TRACE("CInternetFileManagerObject::GetFileComplex: root URL list is empty\n");
			return INETFILE_E_INVALIDROOTURL;
		}

		//iterate through each url;
		for (index=0; index<m_RootURLs.GetSize(); index++)
		{
			hr = S_OK;

#ifdef _DEBUG
			dwTick = GetTickCount();
#endif

			strFullURL = m_RootURLs[index] + "/" + CString(bstrURL);
			
			//find a cache name.
			length = sizeof(cinfo);
			
			bRetVal = RetrieveUrlCacheEntryFile(strFullURL,(INTERNET_CACHE_ENTRY_INFO *)&cinfo, &length, 0);

			UnlockUrlCacheEntryFile(strFullURL, 0);

			if (!bRetVal) 
			{	
				hr = URLDownloadToCacheFile(NULL, strFullURL, szPath, INTERNET_MAX_PATH_LENGTH, 0 /*BINDF_GETNEWESTVERSION*/, NULL);
				if (SUCCEEDED(hr)) 
				{
					strFilename = CString(szPath);
				
#ifdef _DEBUG
					TRACE("CInternetFileManagerObject::GetFileComplex: successfully downloaded %s as %s, duration %3.2f sec\n", strFullURL, strFilename, (float)(GetTickCount() - dwTick) / 1000.0);
#endif
				}
				else
				{
#ifdef _DEBUG
//					TRACE("CInternetFileManagerObject::GetFileComplex: failed to download %s, duration %3.2f sec\n", strFullURL, (float)(GetTickCount() - dwTick) / 1000.0);
#endif

					UnlockUrlCacheEntryFile(strFullURL, 0);
					DeleteUrlCacheEntry(strFullURL);

					hr = INETFILE_E_FILENOTFOUND;

					continue;
				}
			}
			else
			{
				strFilename = CString(((INTERNET_CACHE_ENTRY_INFO *)&cinfo)->lpszLocalFileName);

#ifdef _DEBUG
				TRACE("CInternetFileManagerObject::GetFileComplex: using cached URL %s as %s, duration %3.2f sec\n", 
					strFullURL, strFilename, (float)(GetTickCount() - dwTick) / 1000.0);	
#endif
			}
			
			*pbstrFinalURL = strFullURL.AllocSysString();
			*pbstrFinalPath = strFilename.AllocSysString();

			break;
		}
	}
	
//	if(*pbstrFinalPath==NULL)
//		hr=E_FAIL;

	return hr;
}

STDMETHODIMP CInternetFileManagerObject::GetFileAsync(BSTR bstrURL, 
													  long lCookie,
													  IIFMCallback *pIFMCallback, VARIANT_BOOL bCheckCache)
{
	HRESULT hr = S_OK;
	BSTR bstrFinalURL = NULL;
	BSTR bstrFinalPath = NULL;
	VARIANT_BOOL bInCache;
	
	if(bCheckCache==VARIANT_TRUE)
	{		
		hr = FindCachedFile(bstrURL, &bstrFinalURL, &bstrFinalPath, &bInCache);
		if(FAILED(hr) || !bInCache)
		{
			//continue on.
			hr = S_OK; 
		}
		else
		{
			hr = pIFMCallback->OnDone_T2(S_OK, bstrFinalPath, lCookie, bstrFinalURL);
			goto ERROR_ENCOUNTERED;
		}
	}

	hr = m_ThreadMap.Add(bstrURL, m_strRootURL, 
		lCookie, this, pIFMCallback);

ERROR_ENCOUNTERED:
	SAFEFREESTRING(bstrFinalURL);
	SAFEFREESTRING(bstrFinalPath);
	
	return hr;

}

STDMETHODIMP CInternetFileManagerObject::CancelAllDownloads()
{
	return E_NOTIMPL;
}


STDMETHODIMP CInternetFileManagerObject::get_RootURL(BSTR* pbstrRootURL)
{
	if (pbstrRootURL == NULL)
		return E_POINTER;

	*pbstrRootURL = m_strRootURL.AllocSysString();

	return S_OK;
}

STDMETHODIMP CInternetFileManagerObject::put_RootURL(BSTR bstrRootURL)
{
	BOOL bretval = TRUE;
	CString strURLLeft, strExtractedURL;

#ifdef INETFILE_DOWNLOAD_DEBUG
	TRACE("CInternetFileManagerObject::put_RootURL: refreshing root URL\n");
#endif

	// NOTE: bstrRootURL can be NULL

	// cache desired root URL
	m_strRootURL = bstrRootURL; 
	strURLLeft = m_strRootURL;

	m_RootURLs.RemoveAll();

	while (bretval)
	{
		bretval = ExtractFirstURL(strURLLeft, strExtractedURL);
		if (strExtractedURL.GetLength() != 0)
		{
#ifdef INETFILE_DOWNLOAD_DEBUG
			TRACE("CInternetFileManagerObject::put_RootURL: adding path %s\n", strExtractedURL);
#endif
			m_RootURLs.Add(strExtractedURL);
		}
	}

	return S_OK;
}

STDMETHODIMP CInternetFileManagerObject::FileDLDone(BSTR bstrFilename)
{
	return m_ThreadMap.Remove(CString(bstrFilename));
}
