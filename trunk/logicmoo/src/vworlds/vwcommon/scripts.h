// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _CSCRIPTSITE_
#define _CSCRIPTSITE_

 #include <activscp.h>

#include <afxcoll.h>

#define E_RUNTIME 0
#define E_CREATE_ENGINE_FAILED 1
#define E_PARSE_UNSUPPORTED 2
#define E_BAD_DIRECTIVE_SYNTAX 3
#define E_BAD_DIRECTIVE_ARG	4
#define E_BAD_DIRECTIVE 5
#define E_FILE_IO	6
#define E_MEMORY	7
#define E_ENGINE_NOT_FOUND 8
#define E_ENGINE_COLLIDE 9
#define E_NO_ENGINE	10
#define E_NO_TYPEINFO 11
#define E_NO_DISPATCH 12
#define E_NO_IDOFNAME 13
#define E_FAIL_ADDNAME 14
#define E_TIMEOUT 15
#define E_THREAD_ID 16

class CScriptSite :	public IActiveScriptSiteWindow,
					public IActiveScriptSite
  {

public:
	static CString error_strings[];
	CString runtime_error;

	typedef struct RunTimeErrorInfo {
		EXCEPINFO ei;
		DWORD     dwSrcContext;
		ULONG     ulLine;
		LONG      ichError;
		CString   strLine;

	} RunTimeErrorInfo;

	CScriptSite();
	~CScriptSite();

	// Error information 
	INT GetLastError();
	CString strerror( INT errnum );

	// specific Runtime error file info
	
	BOOL GetRunTimeErrorInfo( RunTimeErrorInfo &rteInfo );

	// Enable runtime use of dialogs etc...

	void EnableSiteWindow( BOOL TF );
	BOOL IsSiteWindowEnabled();

	// Loading and invoking scripts...

	HRESULT LoadScript( CString& strScript, CString& strNameSpace );
	BOOL IsLoaded() { return m_ps ? TRUE : FALSE; };

	HRESULT GetScriptDispatch(IDispatch** ppdispatch)
	{
		if (m_ps == NULL)
			return E_FAIL;
		else
			return m_ps->GetScriptDispatch(NULL, ppdispatch);
	};

	HRESULT InvokeByName( CString &strName, 
						  CString &strContext = CString(""),
						  WORD		wFlags = DISPATCH_METHOD,
 						  DISPPARAMS* pdispparams = NULL,
						  LPVARIANT pvar = NULL,
						  HRESULT* pinvokehr = NULL,
						  int timeout = -1);
	HRESULT InvokeByName( BSTR bstrName, 
						  BSTR bstrContext, 
						  WORD		wFlags = DISPATCH_METHOD,
 						  DISPPARAMS* pdispparams = NULL,
						  LPVARIANT pvar = NULL,
						  HRESULT* pinvokehr = NULL,
 						  int timeout = -1);
 	HRESULT InvokeByDispID(DISPID dispid,
						  BSTR bstrContext, 
	  					  WORD		wFlags = DISPATCH_METHOD,
						  DISPPARAMS* pdispparams = NULL,
						  LPVARIANT pvar = NULL,
						  HRESULT* pinvokehr = NULL,
						  int timeout = -1);
	
	HRESULT InvokeEvent(IDispatch  *pdisp,    // IDispatch of Sink
						DISPID      dispid,   // DISPID of event
						WORD		wFlags = DISPATCH_METHOD,
						DISPPARAMS* pdispparams = NULL,
						LPVARIANT pvar = NULL,
						HRESULT* pinvokehr = NULL);

	HRESULT InvokeScriptlet(CString &strScriptLet, CString &strContext, int timeout = -1, LPVARIANT pVar = NULL);

	HRESULT GetDispatch(CString &strContext, IDispatch **ppDisp)
	{
		if (m_ps)
			return m_ps->GetScriptDispatch(CComBSTR(strContext), ppDisp);
		else
			return E_FAIL;
	}

	HRESULT ShowActions();
	
	// shut down the current engine
	void Close(void);
	
	// *** IUnknown methods ***
	STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj);
	STDMETHOD_(ULONG, AddRef)(void);
	STDMETHOD_(ULONG, Release)(void);
	
	// *** IActiveScriptSiteWindow methods ***
	STDMETHOD(GetWindow)(HWND *phwnd);
	STDMETHOD(EnableModeless)(BOOL fEnable);
	
	// *** IActiveScriptSite methods ***
	STDMETHOD(GetLCID)(LCID *plcid);
	STDMETHOD(GetItemInfo)(LPCOLESTR pstrName, DWORD dwReturnMask, IUnknown **ppiunkItem, ITypeInfo **ppti);
	STDMETHOD(GetDocVersionString)(BSTR *pszVersion);
	STDMETHOD(OnScriptTerminate)(const VARIANT *pvarResult, const EXCEPINFO *pexcepinfo);
	STDMETHOD(OnStateChange)(SCRIPTSTATE ssScriptState);
	STDMETHOD(OnScriptError)(IActiveScriptError *pscripterror);
	STDMETHOD(OnEnterScript)(void);
	STDMETHOD(OnLeaveScript)(void);
	
	HRESULT AddNamedItem(CString strname, IUnknown *pUnk, ITypeInfo *pTypeInfo);

private:
	BOOL m_bEnableActiveScriptSiteWindow;
	static CString error_list[];
	
	int m_ERROR;

	// line error numbers
	RunTimeErrorInfo m_rteInfo;

 	// keeps track of preprocessed offsets
	int m_clineOffset;

	// *** Member Variables ***
	unsigned int	      m_cref;
	IActiveScript        *m_ps;
	IActiveScriptParse   *m_psp;

	CLSID          m_clsidEngine;

	CString m_strCodeFile;

	CMapStringToPtr m_IUnknownMap, m_ITypeInfoMap;

	// Private methods
	HRESULT AddNameSpace();

	// Parsing file methods
	int FindToken(char    *pszCur, char   **ppszBeginArg, char   **ppszNextLine, HRESULT *phr);
 	void SkipWhite(char **ppsz, BOOL   fSkipNewLines);
    HRESULT ParseFile( CString& strFileName, CString& strItemName, int nOffset = 0);

	// Create the engine
	HRESULT CreateScriptEngine(CString strItemName);
	
	// Thread watching variables and methods
	volatile BOOL m_bExceptionRaised;
	SCRIPTTHREADID m_stidThread;
	int m_timeout;
	
	static BOOL g_ThreadState;
	static HANDLE g_WaitEvents[4];
	static HANDLE g_hWatchThread;
	static UINT g_cref;
	static volatile CScriptSite * g_pCurrentSite;

public:
	SCRIPTTHREADID GetScriptThreadID();
	IActiveScript  * GetIActiveScript();
	int GetTimeOut();
	BOOL GetThreadState();
private:

	static void WatchThread(void *vp);

	HRESULT PrimeWatchThread(int timeout);
	HRESULT ClearWatchThread();

	// wrap some synchonization calls to avoid problems
	BOOL ResetEvent(HANDLE hEvent) 
	{
		g_pCurrentSite = this;
		return ::ResetEvent(hEvent);
	}

	BOOL PulseEvent(HANDLE hEvent) 
	{
		g_pCurrentSite = this;

		return ::PulseEvent(hEvent);
	}

	BOOL SetEvent(HANDLE hEvent) 
	{
		g_pCurrentSite = this;

		return ::SetEvent(hEvent);
	}
};


#endif
