// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// SEngineDoc.h : interface of the CSEngineDoc class
//
/////////////////////////////////////////////////////////////////////////////
#include "scripts.h"
#include <vwobject.h>
#include <vwclient.h>

class CTextEntryView;

#define CHANGE_CLIENT -1L

class CSEngineDoc : public CDocument
{
protected: // create from serialization only
	CSEngineDoc();
	DECLARE_DYNCREATE(CSEngineDoc)
	BOOL CanCloseFrame(CFrameWnd* pFrameArg);
	virtual void PreCloseFrame(CFrameWnd* pFrame);


// Attributes
public:
	LPDISPATCH GetClientObject();
	void SetStatusText( CString text );
	BOOL AddScriptlet(CString &code, BOOL bSilent = FALSE, CString *pstrError = NULL);
	BOOL AddScriptSource( CString &fname );
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSEngineDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL


	// -------- Event implementation -----
	DECLARE_INTERFACE_MAP()
	ULONG m_dwClientCookie;
	BEGIN_INTERFACE_PART(ClientSite, IVWClientSite)
       	STDMETHOD(OnConnect)(IWorld* pWorld);
		STDMETHOD(OnDisconnect)( IWorld* pWorld);

       	STDMETHOD(OnUserConnect)(IThing* pthing);
		STDMETHOD(OnUserDisconnect)(IThing* pthing);
		STDMETHOD(OnUserReconnect)(IVWClient* newClient, IWorld* pWorld, IThing* pthing);

		STDMETHOD(OnTrace)(BSTR bstr);
        STDMETHOD(OnReport)(BSTR bstr, long lType);

		STDMETHOD(OnUIEvent)(IThing *pthing, BSTR bstrEventName, VARIANT varArg);
	END_INTERFACE_PART(ClientSite)
	
	// Implementation
public:
	virtual ~CSEngineDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	HRESULT Advise( IUnknown *pUnk,REFIID riid, ULONG *pdwCookie );
	HRESULT UnAdvise( IUnknown *pUnk,REFIID riid, ULONG dwCookie );
// Generated message map functions
protected:
	//{{AFX_MSG(CSEngineDoc)
	afx_msg void OnScriptLoad();
	afx_msg void OnUpdateScriptInvokesub(CCmdUI* pCmdUI);
	afx_msg void OnScriptInvokesub();
	afx_msg void OnScriptEnumeratetypeinfo();
	afx_msg void OnUpdateScriptEnumeratetypeinfo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CSEngineDoc)
	CString m_statusText;
	afx_msg void OnStatusTextChanged();
	LPDISPATCH m_xObject;
	afx_msg void OnXObjectChanged();
	afx_msg void DoMessageBox(LPCTSTR msg);
	afx_msg LPDISPATCH CreateNewX();
	afx_msg LPDISPATCH CreateOLEObject(LPCTSTR lpzClassName);
	afx_msg SCODE oleInvokeScriptlet(LPCTSTR strScript);
	afx_msg void CreateOCXView(LPCTSTR progid);
	afx_msg LPDISPATCH CreateOCX(LPCTSTR oleid);
	afx_msg void OnUserReconnect(IVWClient* newClient,
		IWorld* pWorld,	IThing* pthing);

	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

public:
	CScriptSite m_ScriptSite;
};

/////////////////////////////////////////////////////////////////////////////
