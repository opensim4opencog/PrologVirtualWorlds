// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "ERRNO.H"
#include "stdafx.h"
#include <process.h>
#include "helpers.h"

#include <StdLib.h>
#include <Memory.h>
#include <OleCtl.h>
#include <winnls.h>
#include <Objsafe.h>

// ActiveX Scripting related headers

//#include <ocidl.h>
#include <ActivScp.h>

#include "VBSGuids.h"
#include "CompMgr.h"

#include <math.h>
#include <sys/types.h>

#include <sys/stat.h>
#include <stdio.h>
#include <scripts.h>

#define SCRIPTSITE_DEBUG

#define E_MAX 17


CString CScriptSite::error_list[] = {
/*E_RUNTIME 0*/					"", // set at runtime	
/*E_CREATE_ENGINE_FAILED 1*/	"Creating the ActiveX Scripting engine failed.  Scripting engine is probably not correctly registered or CLSID incorrect.",
/*E_PARSE_UNSUPPORTED 2*/		"ActiveX Scripting engine does not support IActiveScriptParse",
/*E_BAD_DIRECTIVE_SYNTAX 3*/	"Missing '=' on $ENGINE or $INCLUDE line",
/*E_BAD_DIRECTIVE_ARG	4*/		"Missing argument on $ENGINE or $INCLUDE line",
/*E_BAD_DIRECTIVE 5*/			"Unrecognized token; must be $ENGINE, $INCLUDE",
/*E_FILE_IO	6*/					"xxxx", // runtime setable
/*E_MEMORY	7*/					"Unable to allocate memory",
/*E_ENGINE_NOT_FOUND 8*/		"Syntax Error or Engine Not found:\n$ENGINE={xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}\n$ENGINE=LANG",
/*E_ENGINE_COLLIDE 9*/			"$ENGINE is different from existing engine.",
/*E_NO_ENGINE	10*/			"No Script Engine Loaded",
/*E_NO_TYPEINFO 11*/			"ITypeInfo is not available for this engine",
/*E_NO_DISPATCH 12*/			"IDispatch is not available for this engine",
/*E_NO_IDOFNAME 13*/			"Cannot obtain a dispatch ID for name",
/*E_FAIL_ADDNAME 14*/			"Unable to add name to name space",
/*E_TIMEOUT 15 */				"Function has timed out",
/*E_THREAD_ID 16 */				"Failed to get the thread ID",
};

CString CScriptSite::error_strings[] = {
"E_RUNTIME",
"E_CREATE_ENGINE_FAILED",	
"E_PARSE_UNSUPPORTED",
"E_BAD_DIRECTIVE_SYNTAX",
"E_BAD_DIRECTIVE_ARG",
"E_BAD_DIRECTIVE",
"E_FILE_IO",
"E_MEMORY",
"E_ENGINE_NOT_FOUND",
"E_ENGINE_COLLIDE",
"E_NO_ENGINE",
"E_NO_TYPEINFO",
"E_NO_DISPATCH",
"E_NO_IDOFNAME",
"E_FAIL_ADDNAME",
"E_TIMEOUT",
"E_THREAD_ID",
};

BOOL CScriptSite::g_ThreadState = FALSE;
HANDLE CScriptSite::g_WaitEvents[4] = { NULL, NULL, NULL, NULL };
HANDLE CScriptSite::g_hWatchThread = NULL;
UINT CScriptSite::g_cref = 0;
volatile CScriptSite* CScriptSite::g_pCurrentSite = NULL;

// from screrror.h:
// FACILITY_DISPATCH. The error has been reported to the host via 
// IActiveScriptSite::OnScriptError.
#define SCRIPT_E_REPORTED   0x80020101L

//static int reentrant = 0;

/*
A quick explaination of the time out features:
There is a single Watcher thread that may be awakened and terminate script execution
In General this takes three events, Set from 2 different threads (execution thread & watcher)
1) execution ---> Wakes up watcher
2) execution ---> Tell watcher thread that the script finished
3) Watcher	 --->   Tell the execution thread that it is finished w/ the script
				   so that the execution may delete it
  
The General flow is like

  Execution							Watcher
	Reset FinishedEvent
	Toggle WakeUP Event ----------->WakeUP
	Invoke script					Reset WatchCompleted
	Set   FinishedEvent ----------->WaitFor FinishedEvent or Timeout
									If timeout interupt the engine
									
	WaitFor WatchCompleted<---------Set WatchCompleted
	Check for interupts &
		possibly delete the
		script engine

  We need to wait for the watch to complete because if the script does time out it
  appears that the there can be a race condition, 
  Invoke returns and the engine is deleted before the watch thread finishes the
  interupt call.  Not good.

*/

HRESULT CScriptSite::LoadScript(CString& strScript, CString& strNameSpace)
{
	HRESULT hr;

	m_strCodeFile = strScript;

	// load code, load it
	// REVIEW: if this is the "special" VBScript module, just initialize
	// the engine
	if (strScript != ":VBScript")
	{
		hr = ParseFile(strScript, strNameSpace);

		// eat SCRIPT_E_REPORTED HRESULTs
		if (hr == SCRIPT_E_REPORTED)
			hr = E_FAIL;

		if (FAILED(hr))
			return hr;
	}
	else
	{
		// just initialize the script engine
		if (!m_ps)
		{
			hr = CreateScriptEngine(strNameSpace);
			if (FAILED(hr))
				return hr;
		}
	}

	// set the script state
	hr = m_ps->SetScriptState(SCRIPTSTATE_CONNECTED);
	if (FAILED(hr))
		return hr;

	// Success
	return S_OK;
}


//---------------------------------------------------------------------------
// Constructor
//---------------------------------------------------------------------------

DEFINE_GUID(CLSID_NULL,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
extern const CLSID CLSID_VBScript;

CScriptSite::CScriptSite() :
	m_cref(1), m_ps(NULL), m_psp(NULL), m_clsidEngine(CLSID_NULL), m_ERROR(-1),
	m_clineOffset(0), m_bEnableActiveScriptSiteWindow(TRUE), m_bExceptionRaised(FALSE),
	m_timeout(2000)
{
#ifdef SCRIPTSITE_DEBUG
	TRACE("CScriptSite::CScriptSite: this 0x%0x, g_cref %d\n", this, g_cref);
#endif

	if (!g_cref) // We are the only instance of this class, init events and start a thread
	{
//		reentrant = 0;

		SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE }; 

#ifdef SCRIPTSITE_DEBUG
		TRACE("CScriptSite::CScriptSite: this 0x%0x, creating event handles\n", this);
#endif

		// REVIEW: can't use named events since they'll be shared across processes
		g_WaitEvents[0] = CreateEvent(&sa, FALSE, FALSE, NULL); // "WaitUntilInvoke"); 
		g_WaitEvents[1] = CreateEvent(&sa, FALSE, FALSE, NULL); // "WaitForExecution"); 
		g_WaitEvents[2] = CreateEvent(&sa, FALSE, FALSE, NULL); // "WaitForWatchCompletion"); 
		g_WaitEvents[3] = CreateEvent(&sa, TRUE, FALSE, NULL); // "WaitForThreadStart"); 

		g_ThreadState = TRUE;

#ifdef SCRIPTSITE_DEBUG
		TRACE("CScriptSite::CScriptSite: this 0x%0x, starting WatchThread\n", this);
#endif

		g_hWatchThread = (HANDLE)_beginthread(CScriptSite::WatchThread, 0, this);

		ASSERT(((HANDLE)-1) != g_hWatchThread);
	}

	g_cref++;
}


//---------------------------------------------------------------------------
// Clean up allocated resources
//---------------------------------------------------------------------------
CScriptSite::~CScriptSite( void)
{
#ifdef SCRIPTSITE_DEBUG
	TRACE("CScriptSite::~CScriptSite: this 0x%0x, g_cref %d\n", this, g_cref);
#endif

	Close();

	g_cref--;

	if (!g_cref) // the last instance is being deleted, kill the thread
	{
		g_pCurrentSite = this;

#ifdef SCRIPTSITE_DEBUG
		TRACE("CScriptSite::~CScriptSite: this 0x%0x, set g_pCurrentSite to 0x%0x\n", this, g_pCurrentSite);
#endif

#ifdef SCRIPTSITE_DEBUG
		TRACE("CScriptSite::~CScriptSite: this 0x%0x, set ThreadState to FALSE\n", this);
#endif

		g_ThreadState = FALSE; // the thread can quit

#ifdef SCRIPTSITE_DEBUG
		TRACE("CScriptSite::~CScriptSite: this 0x%0x, set WaitForInvoke event\n", this);
#endif

		VERIFY(::SetEvent(g_WaitEvents[0]));

//		Sleep(0);

#ifdef SCRIPTSITE_DEBUG
		TRACE("CScriptSite::~CScriptSite: this 0x%0x, waiting for WatchThread (0x%0x) to exit, timeout 5000 msec\n", this, g_hWatchThread);
#endif

		if (WaitForSingleObject(g_hWatchThread, 5000) == WAIT_TIMEOUT)
		{

//#ifdef SCRIPTSITE_DEBUG
			TRACE("CScriptSite::~CScriptSite: this 0x%0x, timed out waiting for WatchThread to exit, terminating WatchThread\n", this);
//#endif

			// we failed so kill it!
			TerminateThread(g_hWatchThread, 0);
		}
		else
		{
#ifdef SCRIPTSITE_DEBUG
			TRACE("CScriptSite::~CScriptSite: this 0x%0x, WatchThread has exited\n", this);
#endif
		}

#ifdef SCRIPTSITE_DEBUG
		TRACE("CScriptSite::~CScriptSite: this 0x%0x, closing event handles\n", this);
#endif

		CloseHandle(g_WaitEvents[0]);
		CloseHandle(g_WaitEvents[1]);
		CloseHandle(g_WaitEvents[2]);
		CloseHandle(g_WaitEvents[3]);

		g_pCurrentSite = NULL;

#ifdef SCRIPTSITE_DEBUG
		TRACE("CScriptSite::~CScriptSite: this 0x%0x, set g_pCurrentSite to NULL\n", this);
#endif

//		reentrant = 0;
	}
	else
	{
		ASSERT(g_pCurrentSite != this);

		if (g_pCurrentSite == this) 
			g_pCurrentSite = NULL;
	}
}


//---------------------------------------------------------------------------
// Clean up allocated resources
//---------------------------------------------------------------------------
void CScriptSite::Close(void)
{
	POSITION NextPosition = m_IUnknownMap.GetStartPosition();	

	while (NextPosition) 
	{
		void *pv;
		CString Key;
		m_IUnknownMap.GetNextAssoc(NextPosition, Key, pv);
		if (pv) ((IUnknown *) pv)->Release();
	}

	m_IUnknownMap.RemoveAll();

	NextPosition = m_ITypeInfoMap.GetStartPosition();	
	while (NextPosition) 
	{
		void *pv;		
		CString Key;
		m_ITypeInfoMap.GetNextAssoc(NextPosition, Key, pv);
		if (pv) ((ITypeInfo*)pv)->Release();
	}

	m_ITypeInfoMap.RemoveAll();

	// Release the language engine, since it may hold on to us
	if (m_psp)
	{
		m_psp->Release();
		m_psp = NULL;
	}

	if (m_ps)
	{
		m_ps->Close();
		m_ps->Release();
		m_ps = NULL;
	}
}


//***************************************************************************
// IUnknown Interface
//***************************************************************************

//---------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------
STDMETHODIMP CScriptSite::QueryInterface(REFIID iid, LPVOID* ppvObjOut)
{
	if (!ppvObjOut)
		return E_INVALIDARG;
	
	*ppvObjOut = NULL;
	
	if (iid == IID_IUnknown)
		*ppvObjOut = (IActiveScriptSite *) this;
	else if (iid == IID_IActiveScriptSite)
		*ppvObjOut = (IActiveScriptSite *)this;
	else if (iid == IID_IActiveScriptSiteWindow && 
	     m_bEnableActiveScriptSiteWindow)
		*ppvObjOut = (IActiveScriptSiteWindow *)this;

	if (*ppvObjOut)
	{
		this->AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}


//---------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------
STDMETHODIMP_(ULONG) CScriptSite::AddRef(void)
{
	return ++m_cref;
}

//---------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------
STDMETHODIMP_(ULONG) CScriptSite::Release(void)
{
	ASSERT(m_cref);

	m_cref--;

	if (!m_cref)
	{
		if (!m_cref) 
		{
			delete this;
			return 0;
		}
	}

	return m_cref;
}

//---------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------
STDMETHODIMP CScriptSite::GetLCID(LCID *plcid)
{
	return E_NOTIMPL;     // Use system settings
}

//---------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------
STDMETHODIMP CScriptSite::GetItemInfo( LPCOLESTR   pstrName,
										DWORD       dwReturnMask,
										IUnknown**  ppunkItemOut,
										ITypeInfo** pptinfoOut)
{
	HRESULT hr = TYPE_E_ELEMENTNOTFOUND;

	if (dwReturnMask & SCRIPTINFO_ITYPEINFO)
    {
		if (!pptinfoOut) 
			return E_INVALIDARG;

		*pptinfoOut = NULL;
    }

	if (dwReturnMask & SCRIPTINFO_IUNKNOWN)
    {
		if (!ppunkItemOut) 
			return E_INVALIDARG;

		*ppunkItemOut = NULL;
    }

	CString strName = pstrName;
	void * pInfo;
	void * pUnk;

	if (dwReturnMask & SCRIPTINFO_ITYPEINFO)
	{ 
		if (m_ITypeInfoMap.Lookup(strName, pInfo) && pInfo)
		{
			*pptinfoOut = (ITypeInfo *)pInfo;
			(*pptinfoOut)->AddRef();      // because returning
		}
		else
		{
			return TYPE_E_ELEMENTNOTFOUND;
		}
	}
	
	if (dwReturnMask & SCRIPTINFO_IUNKNOWN)
	{
		if (m_IUnknownMap.Lookup(strName, pUnk) && pUnk)
		{
			*ppunkItemOut = (IUnknown *) pUnk;
			(*ppunkItemOut)->AddRef();    // because returning
		}
		else 
			return TYPE_E_ELEMENTNOTFOUND;
	}
	
	return S_OK;	
}

STDMETHODIMP CScriptSite::GetWindow(HWND *phwndOut)
{
	if (!phwndOut) 
		return E_INVALIDARG;

	*phwndOut = AfxGetMainWnd() ? AfxGetMainWnd()->m_hWnd : NULL;

	return S_OK;
}

//---------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------
STDMETHODIMP CScriptSite::EnableModeless(BOOL fEnable)
{
	if (m_bEnableActiveScriptSiteWindow)
		return S_OK; // if this isn't implemented popups won't work
	else
		return E_NOTIMPL;   // TODO: Implement this method
}

//---------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------
STDMETHODIMP CScriptSite::GetDocVersionString(BSTR *pbstrVersion)
{
	return E_NOTIMPL;   // TODO: Implement this method
}

//---------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------
STDMETHODIMP CScriptSite::OnScriptTerminate(const VARIANT   *pvarResult,
											const EXCEPINFO *pexcepinfo)
{
	// TODO: Put up error dlg here
	return S_OK;
}

//---------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------
STDMETHODIMP CScriptSite::OnStateChange(SCRIPTSTATE ssScriptState)
{
	// Don't care about notification
	return S_OK;
}

//---------------------------------------------------------------------------
// Display the error
//---------------------------------------------------------------------------
STDMETHODIMP CScriptSite::OnScriptError(IActiveScriptError *pse)
{
	CComBSTR bstrLine;
	HRESULT hr = S_OK;

	pse->GetExceptionInfo(&m_rteInfo.ei);
	pse->GetSourcePosition(&m_rteInfo.dwSrcContext, 
						   &m_rteInfo.ulLine, 
						   &m_rteInfo.ichError);

 	m_rteInfo.ulLine += m_clineOffset;

	hr = pse->GetSourceLineText(&bstrLine.m_str);
	if (SUCCEEDED(hr))
		m_rteInfo.strLine = bstrLine;
 	else 
		m_rteInfo.strLine.Empty();

	{
		CString Src =  m_rteInfo.ei.bstrSource;
		CString Desc = m_rteInfo.ei.bstrDescription;
		CString Line = m_rteInfo.strLine;

		HRESULT rtehr = ResultFromScode(m_rteInfo.ei.scode);
		
		runtime_error.Format("%s: %s, Line:%d, Char:%d, %s", 
			Src, Desc, m_rteInfo.ulLine, m_rteInfo.ichError, Line);

		m_ERROR = E_RUNTIME;
	}

	return hr;
}

//---------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------
STDMETHODIMP CScriptSite::OnEnterScript(void)
{
	// No need to do anything
	return S_OK;
}

//---------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------
STDMETHODIMP CScriptSite::OnLeaveScript(void)
{
	// No need to do anything
	return S_OK;
}

// ##### BEGIN ACTIVEX SCRIPTING SUPPORT #####
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
HRESULT CScriptSite::CreateScriptEngine(CString strItemName)
{
	HRESULT hr = S_OK;
	IObjectSafety* pObjectSafety = NULL;

	if (m_ps) 
		return E_FAIL;   // Already created it
	
	// Create the ActiveX Scripting Engine
	if (m_clsidEngine == CLSID_NULL)
	{
		// No one specified an engine, go with VBScript
		m_clsidEngine = CLSID_VBScript;
	}

	hr = CoCreateInstance(m_clsidEngine, NULL, CLSCTX_INPROC_SERVER, IID_IActiveScript, (void **)&m_ps);
	if FAILED(hr)
	{
		m_ps = NULL;
		m_ERROR = E_CREATE_ENGINE_FAILED;
		return E_FAIL;
	}

	// Script Engine must implement IObjectSafety
	hr = m_ps->QueryInterface(IID_IObjectSafety, (void **)&pObjectSafety);
	if FAILED(hr)
	{
		ASSERT(FALSE);
		if (pObjectSafety)
			pObjectSafety->Release();
		m_ERROR = hr;
		return E_FAIL;
	}

	DWORD dwOptions, dwSettings;
	hr = pObjectSafety->GetInterfaceSafetyOptions(IID_IDispatch, &dwOptions, &dwSettings);
	if (FAILED(hr))
	{
		ASSERT(FALSE);
		if (pObjectSafety)
			pObjectSafety->Release();
		m_ERROR = hr;
		return E_FAIL;
	}

	hr = pObjectSafety->SetInterfaceSafetyOptions(IID_IDispatch, dwOptions, dwSettings | INTERFACE_USES_SECURITY_MANAGER);
	if (FAILED(hr))
	{
		ASSERT(FALSE);
		if (pObjectSafety)
			pObjectSafety->Release();
		m_ERROR = hr;
		return E_FAIL;
	}

	if (pObjectSafety)
	{
		pObjectSafety->Release();
		pObjectSafety = NULL;
	}

	// Script Engine must support IActiveScriptParse for us to use it
	hr = m_ps->QueryInterface(IID_IActiveScriptParse, (void **)&m_psp);
	if FAILED(hr)
	{
		m_psp->Release();
		m_psp = NULL;
		m_ERROR = E_PARSE_UNSUPPORTED;
		return E_FAIL;
	}

	// Script Engine parser must implement IObjectSafety
	hr = m_psp->QueryInterface(IID_IObjectSafety, (void **)&pObjectSafety);
	if FAILED(hr)
	{
		ASSERT(FALSE);
		if (pObjectSafety)
			pObjectSafety->Release();
		m_psp->Release();
		m_psp = NULL;
		m_ERROR = hr;
		return E_FAIL;
	}

	hr = pObjectSafety->GetInterfaceSafetyOptions(IID_IActiveScriptParse, &dwOptions, &dwSettings);
	if (FAILED(hr))
	{
		ASSERT(FALSE);
		if (pObjectSafety)
			pObjectSafety->Release();
		m_psp->Release();
		m_psp = NULL;
		m_ERROR = hr;
		return E_FAIL;
	}

	hr = pObjectSafety->SetInterfaceSafetyOptions(IID_IActiveScriptParse, dwOptions, dwSettings | INTERFACE_USES_SECURITY_MANAGER);
	if (FAILED(hr))
	{
		ASSERT(FALSE);
		if (pObjectSafety)
			pObjectSafety->Release();
		m_psp->Release();
		m_psp = NULL;
		m_ERROR = hr;
		return E_FAIL;
	}

	if (pObjectSafety)
	{
		pObjectSafety->Release();
		pObjectSafety = NULL;
	}

	ASSERT( m_ps );
	hr = m_ps->SetScriptSite(this);
	if FAILED(hr) 
	{
		Close();
		return hr;
	}

	// InitNew the object:
	ASSERT( m_psp );
	hr = m_psp->InitNew();
	if FAILED(hr) 
	{
		Close();
		return hr;
	}

	// Add our name space
	hr = AddNameSpace();
	return hr;
}

//***************************************************************************
// Parsing support
//***************************************************************************

//---------------------------------------------------------------------------
// Skip white space, counting lines.  Treat \r & \n as white space only if
// fSkipNewLines is TRUE.
//---------------------------------------------------------------------------
void CScriptSite::SkipWhite(char **ppsz, BOOL fSkipNewLines)
{
	// Skip white space
	while (**ppsz && **ppsz<=' ')
	{
		if (**ppsz == '\r' || **ppsz == '\n')
		{
			if (!fSkipNewLines)
				return;
			if (**ppsz == '\r' && (*ppsz)[1] == '\n')
				++*ppsz;
			m_clineOffset++;
		}

		++*ppsz;
	}
}

//---------------------------------------------------------------------------
// Finds $OBJECT, $ENGINE, $INCLUDE, etc, parses past '=' to argument.  NULL
// terminates the argument, then returns a ptr to begining of arg & to beg
// of next line.
//---------------------------------------------------------------------------
struct  
{
	int   cch;
	char *psz;
} g_rgtoken[] = 
{ 
	{7, "$ENGINE"},
	{8, "$INCLUDE"},
	{0, ""},
};

int CScriptSite::FindToken(
  char    *pszCur,
  char   **ppszBeginArg,
  char   **ppszNextLine,
  HRESULT *phr)
{
  char chQuote = 0;
  char ch;

  *phr = S_OK;
  SkipWhite(&pszCur, TRUE);
  if (*pszCur != '$')
    {
    *ppszNextLine = pszCur;
    return 0;   // No token found
    }

  for (int i=0; g_rgtoken[i].cch; i++)
    {
    if (!strncmp(pszCur, g_rgtoken[i].psz, g_rgtoken[i].cch))
      {
      pszCur += g_rgtoken[i].cch;
      SkipWhite(&pszCur, FALSE);
      if (*pszCur != '=')
        {
		m_ERROR = E_BAD_DIRECTIVE_SYNTAX;
        *phr = E_FAIL;
        return 0;
        }
      pszCur++;
      SkipWhite(&pszCur, FALSE);
      if (*pszCur <= ' ')
        {
		m_ERROR = E_BAD_DIRECTIVE_ARG;
        *phr = E_FAIL;
        return 0;
        }
      if (*pszCur=='"' || *pszCur=='\'' || *pszCur=='<')
        {
        chQuote = *pszCur;
        if (chQuote == '<')
          chQuote = '>';
        pszCur++;
        }
      *ppszBeginArg = pszCur;
      if (chQuote)
        {
        // Look for End of String, EOL, or End of Quote
        while (*pszCur && *pszCur!='\n' && *pszCur!='\r' && *pszCur!=chQuote)
          pszCur++;
        }
      else
        {
        // Skip non-white space
        while (*pszCur > ' ')
          pszCur++;
        }
      ch = *pszCur;
      *pszCur = 0;
NextChar:
      switch (ch)
        {
        default:
          // Look for EOL
          pszCur++;
          ch = *pszCur;
          goto NextChar;

        case '\r':
          // Skip the cr we're at
		  m_clineOffset++;

          pszCur++;
          if (*pszCur == '\n')
            pszCur++;
          break;

        case '\n':
          // Skip the newline we're at
           m_clineOffset++;
          pszCur++;
          break;

        case 0:
           break;
        }

      *ppszNextLine = pszCur;
      return i+1;
      }
    }
  m_ERROR = E_BAD_DIRECTIVE;
  *phr = E_FAIL;
  return 0;
}


//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
HRESULT CScriptSite::ParseFile(CString& pszFileName, CString& pstrItemName, int nOffset)
{
	struct _stat stat;
	size_t       cch;
	EXCEPINFO    ei;
	char        *pszT;
	char        *pszAlloc;
	FILE        *pfile;
	HRESULT      hr = S_OK;
	int          tk = 0;
	char        *pszArg;
	
	memset(&ei, 0x0, sizeof(EXCEPINFO));

	char drive[_MAX_DRIVE];   
	char dir[_MAX_DIR];
	char ext[_MAX_EXT];
	char fname[_MAX_FNAME];
   _splitpath( LPCSTR(pszFileName), drive, dir,fname,ext);


	// Find the length of the file
	if (_stat(pszFileName, &stat) != 0)
	{
		m_ERROR = E_FILE_IO;
		ASSERT(m_ERROR >= 0);
		ASSERT(m_ERROR < E_MAX);
		error_list[ E_FILE_IO ] = ::strerror(errno);
		return E_FAIL;
	}
	cch = stat.st_size;

	// Allocate a buffer to read the file
	pszAlloc = new char[cch+1];
	if (!pszAlloc) 
	{
		m_ERROR = E_MEMORY;
		return E_OUTOFMEMORY;
	}
	memset(pszAlloc, 0, cch);   // Since cch may be too large, zero-fill string

	// Open & read the file
	m_clineOffset = nOffset;
	pfile = fopen(pszFileName, "rb");
	if (!pfile)
	{
		m_ERROR = E_FILE_IO;
		error_list[ E_FILE_IO] = ::strerror(errno);
		hr = E_FAIL;
		goto Error;
	}
	fread(pszAlloc, cch, 1, pfile);
	fclose(pfile);

	// Parse lines begining w/$
	pszAlloc[cch] = 0;
	pszT = pszAlloc;
	while (tk=FindToken(pszT, &pszArg, &pszT, &hr))
	{
		switch (tk)
		{
		case 1:   // $ENGINE
			{
			MAKE_WIDEPTR_FROMANSI(pwszCLSID, pszArg);

			CLSID  clsidEngine;
			if ((hr = CLSIDFromString(pwszCLSID, &clsidEngine)) != S_OK)
			{
				m_ERROR = E_ENGINE_NOT_FOUND;
				goto Error;
			}
			
			// Test to see if we've created an engine and if so is it the same as what
			// we are requesting?
			if (m_ps)
			{
				if  (clsidEngine != m_clsidEngine)
				{
					// we've got a problem here
					hr = E_FAIL;
					m_ERROR = E_ENGINE_COLLIDE;
					goto Error;
				}
			// Otherwise we have an engine and it is the same
			}
			else
			{
				m_clsidEngine = clsidEngine;
				hr = this->CreateScriptEngine(pstrItemName);
				if (hr)	goto Error;
			}
			break;
			}

		case 2:   // $INCLUDE
			{
			int nOffset = m_clineOffset;
			char idrive[_MAX_DRIVE];   
			char idir[_MAX_DIR];
			char iext[_MAX_EXT];
			char ifname[_MAX_FNAME];
			CString strFile = pszArg;

			_splitpath( pszArg,idrive, idir, ifname, iext );
			// Separate absolute from relative path names
			// Fully qualified path name
			if (( strlen(idrive) > 0 ) ||
				( idir[0] == '\\' ) ||
				( idir[0] == '/' ))
			{
				strFile = pszArg;
			}
			else // relative path
			{
				char full[_MAX_PATH], part[_MAX_PATH];
				strcpy(part,drive);
				strcat(part,dir);
				strcat(part,pszArg);

				if( _fullpath( full, part, _MAX_PATH ) != NULL )
				{
					strFile = full;
				}
			}
		
			hr = this->ParseFile(strFile, pstrItemName);
			if FAILED(hr)
				goto Error;
			m_clineOffset = nOffset;
			}

		break;
		}
	}
    // Make sure we've created the scripting engine
	if (!m_ps)
	{
	  hr = this->CreateScriptEngine(pstrItemName);
	  if FAILED(hr)
		goto Error;
	}

  // If there's anything left, assume it's code
	if (*pszT)
	{
		// ##### BEGIN ACTIVEX SCRIPTING SUPPORT #####

		// Hand it the code
		MAKE_WIDEPTR_FROMANSI(pwszCode, pszT);
		//hr = m_psp->ParseScriptText(pwszCode, pstrItemName, NULL, NULL, 0, 0, 0L, NULL, &ei);
		BSTR bstr = pstrItemName.AllocSysString();
		hr = m_psp->ParseScriptText(pwszCode, bstr, NULL, NULL, 0, 0, SCRIPTTEXT_ISVISIBLE |SCRIPTTEXT_ISPERSISTENT  , NULL, &ei);
		::SysFreeString(bstr);
		//hr = S_OK;    // Ignore parse errors, so user can press F9 to view source...

		// #####  END  ACTIVEX SCRIPTING SUPPORT #####
	}

Error:
  delete pszAlloc;
  return hr;
}

HRESULT CScriptSite::AddNamedItem(CString strname, IUnknown *pUnk, ITypeInfo *pTypeInfo)
{
	// AddRef becuase we need to hold on
	if (pUnk) 
		pUnk->AddRef();

	if (pTypeInfo) 
		pTypeInfo->AddRef();

	// Check to see if the strname is already here
	// if so Release it
	void *pv = NULL;

	if (m_IUnknownMap.Lookup(strname, pv) && pv)
	{
		((IUnknown *) pv)->Release();
	}

	pv = NULL;

	if (m_ITypeInfoMap.Lookup(strname, pv) && pv)
	{
		((ITypeInfo *) pv)->Release();
	}

	m_IUnknownMap[strname] = (void*)pUnk;
	m_ITypeInfoMap[strname] = (void*)pTypeInfo;

	return S_OK;
}

HRESULT CScriptSite::AddNameSpace()
{
	POSITION NextPosition = m_IUnknownMap.GetStartPosition();	
	while (NextPosition) 
	{
		void *pv;		
		CString Key;

		m_IUnknownMap.GetNextAssoc(NextPosition, Key, pv);
		BSTR wname = Key.AllocSysString();

		HRESULT hr = m_ps->AddNamedItem(wname, SCRIPTITEM_ISVISIBLE | SCRIPTITEM_ISSOURCE);

		::SysFreeString(wname);

		if (FAILED(hr))
		{
			m_ERROR = E_FAIL_ADDNAME;
			return hr;
		}
	}
	
	return S_OK;
}

HRESULT CScriptSite::InvokeEvent(IDispatch* pdisp, DISPID dispid, WORD wFlags, 
								 DISPPARAMS* pdispparams, LPVARIANT pvar, HRESULT *pinvokehr)
{
	HRESULT hr = S_OK;
	EXCEPINFO ei;
	UINT uArgErr = 0;
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};

	memset(&ei, 0x0, sizeof(EXCEPINFO));

	if (pdispparams == NULL) 
		pdispparams = &dispparamsNoArgs;

	if (pinvokehr)
		*pinvokehr = S_OK;

	if (pvar)
		::VariantClear(pvar);

	hr = pdisp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, wFlags, pdispparams, 
		(wFlags == DISPATCH_PROPERTYPUT) ? NULL : pvar, &ei, &uArgErr);

	if (hr == DISP_E_EXCEPTION)
	{
		HRESULT excephr = ResultFromScode(ei.scode);

		if (pinvokehr)
			*pinvokehr = excephr;
	}
	else if (hr == SCRIPT_E_REPORTED)
	{
		HRESULT excephr = ResultFromScode(m_rteInfo.ei.scode);

		if (pinvokehr)
			*pinvokehr = excephr;

		hr = E_FAIL;
	}

	return hr;
}

// helper template
template<class CType>
void SAFERELEASE(CType * &pType)
{
	if (pType)
	{
		pType->Release();
		pType = NULL;
	}
}

template<class CType>
void SAFEADDREF(CType * pType)
{
	if (pType)
	{
		pType->AddRef();
	}
}

//******************************************************
// ShowActions()
// Untested functionality to enumerate script's typeinfo
HRESULT CScriptSite::ShowActions()
{
	HRESULT hr = E_FAIL;
	IDispatch *pdisp = NULL;

	if (!m_ps) 
	{
		m_ERROR = E_NO_ENGINE;
		return hr;
	}

	if (m_ps->GetScriptDispatch(NULL, &pdisp) == S_OK && pdisp != NULL)
	{
		UINT pctinfo = 0;

		pdisp->GetTypeInfoCount(&pctinfo);
		
		ITypeInfo *ptinfo = NULL;

		pdisp->QueryInterface(IID_ITypeInfo, (void**) &ptinfo);

		FILE * fout = fopen("actions.txt","wt");

		if ((pdisp->GetTypeInfo(0,LOCALE_SYSTEM_DEFAULT, &ptinfo) == S_OK) && ptinfo && fout)
		{	
			TYPEATTR *ptattr;

			if (ptinfo->GetTypeAttr(&ptattr) == S_OK)
			{
				for (int i=0;i< ptattr->cFuncs;i++)
				{
					FUNCDESC *pFuncDesc = NULL;

					if (ptinfo->GetFuncDesc(i, &pFuncDesc) == S_OK && pFuncDesc != NULL)
					{
						CComBSTR bstrName;
						int idMember = pFuncDesc->memid;
						
						ptinfo->GetDocumentation(idMember, &bstrName.m_str, NULL, NULL, NULL);
						fwprintf(fout, L"ID %d, name = %s", idMember, bstrName);

						ptinfo->ReleaseFuncDesc(pFuncDesc);
					}
				}

				ptinfo->ReleaseTypeAttr(ptattr);
			}

			ptinfo->Release();	

			WinExec("actions.txt", SW_SHOW);

			hr = S_OK;
		}
		else 
		{
			m_ERROR = E_NO_TYPEINFO;
		}

		if (fout) 
			fclose(fout);

		SAFERELEASE(pdisp);
	}
	else
	{
		m_ERROR = E_NO_DISPATCH;
	}

	return hr;
}

int CScriptSite::GetLastError()
{
	return m_ERROR;
}

CString CScriptSite::strerror(INT errnum)
{
	CString strError("");

	if (errnum < 0) 
		return strError;
	if (errnum >= E_MAX) 
		return strError;

	if (errnum == E_RUNTIME)
		strError = runtime_error;
	else
		strError = error_list[errnum];
	
	//m_ERROR = -1;
	return strError;
}

BOOL CScriptSite::GetRunTimeErrorInfo(RunTimeErrorInfo &rteInfo)
{
	if (m_ERROR != E_RUNTIME) 
		return FALSE;
	
	rteInfo = m_rteInfo;

	return TRUE;
}

BOOL CScriptSite::IsSiteWindowEnabled()
{
	return m_bEnableActiveScriptSiteWindow;
}

void CScriptSite::EnableSiteWindow(BOOL TF)
{
	m_bEnableActiveScriptSiteWindow = TF;
}

BOOL CScriptSite::GetThreadState()
{
	return g_ThreadState;
}

int CScriptSite::GetTimeOut()
{
	return m_timeout;
}

IActiveScript* CScriptSite::GetIActiveScript()
{
	return m_ps;
}

SCRIPTTHREADID CScriptSite::GetScriptThreadID()
{
	return m_stidThread;
}

HRESULT CScriptSite::InvokeByName(CString &strName, 
								  CString &strContext, 
								  WORD		wFlags,
								  DISPPARAMS* pdispparams, 
								  LPVARIANT pvar,
								  HRESULT* pinvokehr,
								  int timeout)
{
 	HRESULT hresult = S_OK;
	
	hresult = InvokeByName(CComBSTR(strName), CComBSTR(strContext), wFlags, pdispparams, pvar, pinvokehr, timeout);

	return hresult;
}

HRESULT CScriptSite::InvokeByName(BSTR bstrName, 
								   BSTR bstrContext, 
								   WORD		wFlags,
								   DISPPARAMS* pdispparams, 
								   LPVARIANT pvar,
								   HRESULT* pinvokehr,
								   int timeout)
{
	HRESULT hresult = E_FAIL;
	IDispatch *pdisp = NULL;

	// reset error id
	m_ERROR = -1;

	if (!m_ps) 
	{
		m_ERROR = E_NO_ENGINE;
		return hresult;
	}
	
	if (SUCCEEDED((hresult = m_ps->GetScriptDispatch(bstrContext, &pdisp))))
	{
		DISPID dispid = DISPID_UNKNOWN;

		hresult = pdisp->GetIDsOfNames(IID_NULL, &bstrName, 1, LOCALE_SYSTEM_DEFAULT, &dispid);

		if (SUCCEEDED(hresult))
		{
			if (FAILED(PrimeWatchThread(timeout)))
				goto errout;

#ifdef SCRIPTSITE_DEBUG
			TRACE("CScriptSite::InvokeByName: this 0x%0x, before InvokeEvent\n", this);
#endif

 			hresult = InvokeEvent(pdisp, dispid, wFlags, pdispparams, pvar, pinvokehr);

#ifdef SCRIPTSITE_DEBUG
			TRACE("CScriptSite::InvokeByName: this 0x%0x, after InvokeEvent\n", this);
#endif

			if (FAILED(ClearWatchThread()))
				goto errout;
		}
		else 
			m_ERROR = E_NO_IDOFNAME;		
	}
	else
		m_ERROR = E_NO_DISPATCH;

errout:
	SAFERELEASE(pdisp);

	return hresult;
}


HRESULT CScriptSite::InvokeByDispID(DISPID dispid, 
									BSTR bstrContext, 
									WORD		wFlags,
									DISPPARAMS* pdispparams, 
 								    LPVARIANT pvar,
									HRESULT* pinvokehr,
									int timeout)
{
  	HRESULT hresult = E_FAIL;
  	IDispatch *pdisp = NULL;
  
	// reset error id
	m_ERROR = -1;

	if (!m_ps) 
  	{
  		m_ERROR = E_NO_ENGINE;
  		return hresult;
  	}
  	
  	if (SUCCEEDED(m_ps->GetScriptDispatch(bstrContext, &pdisp)) && pdisp)
  	{
		if (FAILED(PrimeWatchThread(timeout)))
			goto errout;

#ifdef SCRIPTSITE_DEBUG
		TRACE("CScriptSite::InvokeByDispID: this 0x%0x, before InvokeEvent\n", this);
#endif

  		hresult = InvokeEvent(pdisp, dispid, wFlags, pdispparams, pvar, pinvokehr);

#ifdef SCRIPTSITE_DEBUG
		TRACE("CScriptSite::InvokeByDispID: this 0x%0x, after InvokeEvent\n", this);
#endif

		SAFERELEASE(pdisp);

		if (FAILED(ClearWatchThread()))
			goto errout;
  	}
  	else
  		m_ERROR = E_NO_DISPATCH;

errout:
  	return hresult;
}

HRESULT CScriptSite::InvokeScriptlet(CString &strScript, CString &strContext, int timeout, LPVARIANT pVariant)
{
	HRESULT hresult = E_FAIL;
	DWORD dwFlags = SCRIPTTEXT_ISVISIBLE | SCRIPTTEXT_ISPERSISTENT;
	EXCEPINFO ei;

	memset(&ei, 0x0, sizeof(EXCEPINFO));

	// reset error id
	m_ERROR = -1;

	if (!m_psp) 
	{
		m_ERROR = E_NO_ENGINE;

		return hresult;
	}

	m_bExceptionRaised = FALSE;

	if (FAILED(PrimeWatchThread(timeout)))
		goto errout;
	
#ifdef SCRIPTSITE_DEBUG
	TRACE("CScriptSite::InvokeScriptlet: this 0x%0x, before ParseScriptText\n", this);
#endif

	hresult = m_psp->ParseScriptText(CComBSTR(strScript), CComBSTR(strContext), NULL, NULL, 0, 0, dwFlags, pVariant, &ei);

#ifdef SCRIPTSITE_DEBUG
	TRACE("CScriptSite::InvokeScriptlet: this 0x%0x, after ParseScriptText\n", this);
#endif

	if (FAILED(ClearWatchThread()))
		goto errout;

	// eat SCRIPT_E_REPORTED HRESULT
	if (hresult == SCRIPT_E_REPORTED)
		hresult = E_FAIL;

errout:
	return hresult;
}

//		g_WaitEvents[0] = CreateEvent(&sa, FALSE, FALSE, "WaitUntilInvoke"); 
//		g_WaitEvents[1] = CreateEvent(&sa, FALSE, FALSE, "WaitForExecution"); 
//		g_WaitEvents[2] = CreateEvent(&sa, FALSE, FALSE, "WaitForWatchCompletion"); 

void CScriptSite::WatchThread(void *vp)
{
	CScriptSite *pSite = NULL;

#ifdef SCRIPTSITE_DEBUG
	TRACE("CScriptSite::WatchThread: original site 0x%x, thread 0x%0x, starting up...\n", vp, g_hWatchThread);
#endif

	// tell constructor that thread has started 
	::SetEvent(g_WaitEvents[3]);

	while (TRUE)
	{
#ifdef SCRIPTSITE_DEBUG
		TRACE("CScriptSite::WatchThread: original site 0x%0x, thread 0x%0x, waiting for WaitUntilInvoke event...\n", vp, g_hWatchThread);
#endif

		// Wait until we need to watch something
		if (WaitForSingleObject(g_WaitEvents[0], INFINITE) != WAIT_OBJECT_0)
		{
#ifdef SCRIPTSITE_DEBUG
			TRACE("CScriptSite::WatchThread: original site 0x%0x, failed waiting for WaitUntilInvoke event\n", vp);
#endif
			goto noerror;
		}

#ifdef SCRIPTSITE_DEBUG
		TRACE("CScriptSite::WatchThread: original site 0x%0x, got WaitUntilInvoke event\n", vp);
#endif

		// Get the active Site
		pSite = (CScriptSite *)g_pCurrentSite;

#ifdef SCRIPTSITE_DEBUG
		TRACE("CScriptSite::WatchThread: pSite 0x%0x, original site 0x%0x, caching current site\n", pSite, vp);
#endif

#ifdef SCRIPTSITE_DEBUG
		TRACE("CScriptSite::WatchThread: pSite 0x%0x, original site 0x%0x, reset WaitForWatchCompletion event\n", pSite, vp);
#endif

		// Set the event so that the Invoke will wait till we are done here.
		// That way we can set some error information
		::ResetEvent(g_WaitEvents[2]);

		// Should we exit?
		if (pSite == NULL || !pSite->GetThreadState())
		{
#ifdef SCRIPTSITE_DEBUG
			TRACE("CScriptSite::WatchThread: pSite 0x%0x, set WaitForWatchCompletion event\n", pSite, vp);
#endif

#ifdef SCRIPTSITE_DEBUG
			if (pSite == NULL)
				TRACE("CScriptSite::WatchThread: pSite == NULL, original site 0x%0x, exiting thread\n", vp);
			else
				TRACE("CScriptSite::WatchThread: pSite 0x%0x, original site 0x%0x, g_ThreadState is FALSE, exiting thread\n", pSite, vp);
#endif

			pSite = NULL;

			// Tell the main thread that we are done
			::SetEvent(g_WaitEvents[2]); // wake up the invoker

//			::Sleep(0);

			goto errout;		
		}

#ifdef SCRIPTSITE_DEBUG
		TRACE("CScriptSite::WatchThread: pSite 0x%0x, original site 0x%0x, wait for WaitForExecution event, timeout %d msec\n", pSite, vp, pSite->GetTimeOut());
#endif

		// wait for the script to finish executing or time out!
		if (WaitForSingleObject(g_WaitEvents[1], pSite->GetTimeOut()) == WAIT_TIMEOUT)
		{
//#ifdef SCRIPTSITE_DEBUG
			TRACE("CScriptSite::WatchThread: pSite 0x%0x, original site 0x%0x, timed out waiting for WaitForExecution event\n", pSite, vp);
//#endif

			// we've timed out kill the thread
			SCRIPTTHREADSTATE stsState;
			IActiveScript* ps = pSite->GetIActiveScript();
			SCRIPTTHREADID stidThread = pSite->GetScriptThreadID();

			if (ps && 
				(ps->GetScriptThreadState(stidThread, &stsState) == S_OK) &&
				(stsState == SCRIPTTHREADSTATE_RUNNING))
			{
				// Set the error info
				pSite->m_ERROR = E_TIMEOUT;
				pSite->m_bExceptionRaised = TRUE;

				// are we still running, if so time out
				static CString ErrorInfo("Method invocation has timed out");

				EXCEPINFO excepinfo;
				memset(&excepinfo, 0, sizeof(EXCEPINFO));

				excepinfo.wCode = 0;
				excepinfo.wReserved = 0; 
				excepinfo.bstrSource = ErrorInfo.AllocSysString();
				excepinfo.bstrDescription =  ErrorInfo.AllocSysString();
				excepinfo.bstrHelpFile = NULL;
				excepinfo.dwHelpContext = 0;
				excepinfo.pvReserved = NULL; 
				excepinfo.scode = E_TIMEOUT;

				ps->InterruptScriptThread(stidThread, &excepinfo, SCRIPTINTERRUPT_RAISEEXCEPTION);
			}
		}
		else
		{
#ifdef SCRIPTSITE_DEBUG
			TRACE("CScriptSite::WatchThread: pSite 0x%0x, original site 0x%0x, got WaitForExecution event\n", pSite, vp);
#endif
		}

noerror:
#ifdef SCRIPTSITE_DEBUG
		TRACE("CScriptSite::WatchThread: pSite 0x%0x, original site 0x%0x, resetting cached site to NULL\n", pSite, vp);
#endif

		pSite = NULL;

#ifdef SCRIPTSITE_DEBUG
		TRACE("CScriptSite::WatchThread: original site 0x%0x, set WaitForWatchCompletion event\n", vp);
#endif

		// Tell the main thread that we are done
		::SetEvent(g_WaitEvents[2]); // wake up the invoker

//		::Sleep(0);
	}

errout:
#ifdef SCRIPTSITE_DEBUG
	TRACE("CScriptSite::WatchThread: original site 0x%0x, closing down...\n", vp);
#endif

	_endthread();
}

HRESULT CScriptSite::PrimeWatchThread(int timeout)
{
	static int reentrant = 0;

#ifdef SCRIPTSITE_DEBUG
	TRACE("CScriptSite::PrimeWatchThread: this 0x%0x, reentrant %d, timeout %d msec\n", this, reentrant, timeout);
#endif

	m_timeout = timeout;

	if (m_timeout >= 0 && !reentrant)
  	{
		reentrant++;

		// wait for thread to start up
		if (WaitForSingleObject(g_WaitEvents[3], 5000) == WAIT_TIMEOUT)
		{
//#ifdef SCRIPTSITE_DEBUG
			TRACE("CScriptSite::PrimeWatchThread: this 0x%0x, timed out waiting for WatchThread to start (5000 msec)\n", this);
//#endif
  			m_ERROR = E_TIMEOUT;
			reentrant--;
  			return E_FAIL;
		}

		// fill in the ThreadID so the watch thread can use it
		if (m_ps)
		{
  			if (FAILED(m_ps->GetScriptThreadID(GetCurrentThreadId(), (unsigned long *)&m_stidThread)))
  			{
#ifdef SCRIPTSITE_DEBUG
				TRACE("CScriptSite::PrimeWatchThread: this 0x%0x, failed to get script thread ID\n", this);
#endif
  				m_ERROR = E_THREAD_ID;
				reentrant--;
  				return E_FAIL;
			}
  		}

#ifdef SCRIPTSITE_DEBUG
		TRACE("CScriptSite::PrimeWatchThread: this 0x%0x, reset WaitForExecution event\n", this);
#endif

  		// Turn the "WaitForExecution" event off, this also sets the static g_pScriptSite
		::ResetEvent(g_WaitEvents[1]);

#ifdef SCRIPTSITE_DEBUG
		TRACE("CScriptSite::PrimeWatchThread: this 0x%0x, set g_pCurrentSite to 0x%0x\n", this, g_pCurrentSite);
#endif

		g_pCurrentSite = this;

#ifdef SCRIPTSITE_DEBUG
		TRACE("CScriptSite::PrimeWatchThread: this 0x%0x, set WaitUntilInvoke event\n", this);
#endif

  		// Wake up the Watcher thread
		if (!::SetEvent(g_WaitEvents[0])) // wake up the thread
		{
#ifdef SCRIPTSITE_DEBUG
			TRACE("CScriptSite::PrimeWatchThread: this 0x%0x, failure setting WaitUntilInvoke event\n", this);
#endif

			//we've got problems! the watch thread hasn't been woken up
			m_timeout = -1;
		}

//		::Sleep(0);

		reentrant--;
	}

	return S_OK;
}

HRESULT CScriptSite::ClearWatchThread()
{
	static int reentrant = 0;

#ifdef SCRIPTSITE_DEBUG
	TRACE("CScriptSite::ClearWatchThread: this 0x%0x, reentrant %d\n", this, reentrant);
#endif

	if (m_timeout >= 0 && !reentrant)
	{
		reentrant++;

#ifdef SCRIPTSITE_DEBUG
		TRACE("CScriptSite::ClearWatchThread: this 0x%0x, set WaitForExecution event\n", this);
#endif

		// Tell the Watcher that we are done here
		::SetEvent(g_WaitEvents[1]); 

//		::Sleep(0);

#ifdef SCRIPTSITE_DEBUG
		TRACE("CScriptSite::ClearWatchThread: this 0x%0x, wait for WaitForWatchCompletion event, timeout %d msec\n", this, m_timeout == -1 ? 5000 : m_timeout);
#endif

		// Wait for the watcher to finish any work it needs to do
		if (WaitForSingleObject(g_WaitEvents[2], m_timeout == -1 ? 5000 : m_timeout) == WAIT_TIMEOUT)
		{
//#ifdef SCRIPTSITE_DEBUG
			TRACE("CScriptSite::ClearWatchThread: this 0x%0x, timed out waiting for WaitForWatchCompletion event (%d msec)\n", this, m_timeout == -1 ? 5000 : m_timeout);
//#endif
		}
		else
		{
#ifdef SCRIPTSITE_DEBUG
			TRACE("CScriptSite::ClearWatchThread: this 0x%0x, got WaitForWatchCompletion event\n", this);
#endif
		}

		// Did the watcher thread raise an exception?
		if (m_bExceptionRaised && !reentrant) 
		{
#ifdef SCRIPTSITE_DEBUG
			TRACE("CScriptSite::ClearWatchThread: this 0x%0x, WatchThread raised an exception\n", this);
#endif

			m_bExceptionRaised = FALSE;
			m_ERROR = E_TIMEOUT;

			Close();
		}

#ifdef SCRIPTSITE_DEBUG
		TRACE("CScriptSite::ClearWatchThread: this 0x%0x, set g_pCurrentSite to NULL\n", this);
#endif

		// Make sure the global doesn't point to us
		g_pCurrentSite = NULL;

		reentrant--;
	}

	return S_OK;
}
