// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include <wininet.h>
#include <vwobject.h>
#include <propbase.h>
#include "urlhelp.h"

// parse out server and world from URL
// trim off protocol if present
HRESULT ParseURLHelper(BSTR bstrURL, BSTR* pbstrOut, BSTR* pbstrProtocol, BSTR* pbstrServer, int* pnPort)
{	
	HRESULT hr = S_OK;
	int ich = 0;
	CString strURL = bstrURL;
	CString strServer;
	static CComBSTR bstrVWORLD("vworld");
	static CComBSTR bstrHTTP("http");

	if (bstrURL == NULL)
		return E_INVALIDARG;

	// NOTE: all output params are optional

	if (pnPort)
		*pnPort = nDefaultSocketPort;

	if (pbstrOut)
		*pbstrOut = NULL;

	if (pbstrProtocol)
		*pbstrProtocol = NULL;

	if (pbstrServer)
		*pbstrServer = NULL;

	strURL.TrimLeft();
//	strURL.MakeLower();

	// we don't require vworld: anymore, but let's just peel it off if it exists
	if (strURL.Find("vworld://") == 0)
	{
		strURL = strURL.Mid(9);
		if (pbstrProtocol)
			*pbstrProtocol = bstrVWORLD.Copy();
	}
	else if (strURL.Find("http://") == 0)
	{
		strURL = strURL.Mid(7);
		if (pbstrProtocol)
			*pbstrProtocol = bstrHTTP.Copy();
	}
	else
	{
		if (pbstrProtocol)
			*pbstrProtocol = bstrVWORLD.Copy();
	}

	// Parse out server name if present
	// First, check for forward slash
	ich = strURL.Find('/');
	if (ich >= 0)
	{
		strServer = strURL.Left(ich);
		strURL = strURL.Mid(ich+1);
	}
	// Now check for backward slash
	else
	{
		ich = strURL.Find('\\');
		if (ich >= 0)
		{
			strServer = strURL.Left(ich);
			strURL = strURL.Mid(ich+1);
		}
	// Otherwise server is whole URL
		else
		{
			strServer = strURL;
			strURL = "";
		}
	}

	// see if server name has port number on it
	ich = strServer.Find(':');
	if (ich >= 0)
	{
		CString strPort;

		strPort = strServer.Mid(ich+1);
		strServer = strServer.Left(ich);

		// parse port into numeric
		if (pnPort)
			*pnPort = atoi(strPort);
	}

	// world name left in strURL
	if (pbstrServer)
		*pbstrServer = strServer.AllocSysString();

	if (pbstrOut)
		*pbstrOut = strURL.AllocSysString();
	
	return hr;
}

BOOL IsFullURL(BSTR bstrURL)
{
	char buffer[INTERNET_MAX_PATH_LENGTH];
	URL_COMPONENTS uc = {0};
	DWORD lBufLength = INTERNET_MAX_PATH_LENGTH;
	CString strFullURL(bstrURL);

	// get the root name from the URL
	uc.dwStructSize = sizeof(URL_COMPONENTS);
	uc.lpszUrlPath = buffer;
	uc.dwUrlPathLength = INTERNET_MAX_PATH_LENGTH;

	if (!InternetCrackUrl(	strFullURL,
							strFullURL.GetLength(),
							ICU_ESCAPE,
							&uc )) 
	{
		return FALSE;
	}

	if(uc.nScheme == INTERNET_SCHEME_UNKNOWN)
	{
		return FALSE;
	}

	if (!InternetCanonicalizeUrl(strFullURL, buffer, 
		&lBufLength, ICU_BROWSER_MODE)) 
	{
		return FALSE;
	}

	return TRUE;
}

// Removes first URL from in and returns it.
BOOL ExtractFirstURL(CString& in, CString& out)
{
	long index = in.Find(";");
	BOOL retval = TRUE;	

	if (index == -1) 
	{
		out = in;		
		in = "";
		retval = FALSE;
	}
	else
	{
		out = in.Left(index + 1);
		in = in.Right(in.GetLength() - index - 1);
		retval = TRUE;
	}

	// clean up
	out.TrimLeft();
	out.TrimRight();

	if (out==";")
	{
		out = "";
	}

	if (!out.IsEmpty())
	{
		int cch;

		// get rid of trailing ; if present
		cch = out.GetLength();
		if (out[cch-1] == ';')
			out = out.Left(cch-1);		

		// get rid of trailing slash if present		
		cch = out.GetLength();
		if (out[cch-1] == '/' || out[cch-1] == '\\')
			out = out.Left(cch-1);
	}

	// clean up
	out.TrimLeft();
	out.TrimRight();	
	
	return retval;
}



