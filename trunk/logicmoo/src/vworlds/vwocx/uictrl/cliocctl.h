// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// cliocCtl.h : Declaration of the CVwcliocxCtrl OLE control class.

/////////////////////////////////////////////////////////////////////////////
// CVwcliocxCtrl : See cliocCtl.cpp for implementation.
#include "..\vwview\viewctl.h"

class CVwcliocxCtrl : public CVWViewCtrl
{
	DECLARE_DYNCREATE(CVwcliocxCtrl)

// Constructor
public:
	IDispatch * m_pVWClientObject;
	CVwcliocxCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVwcliocxCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual DWORD GetControlFlags();
	virtual void OnClose(DWORD dwSaveOption);
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CVwcliocxCtrl();

	DECLARE_OLECREATE_EX(CVwcliocxCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CVwcliocxCtrl)      // GetTypeInfo
//	DECLARE_PROPPAGEIDS(CVwcliocxCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CVwcliocxCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CVwcliocxCtrl)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CVwcliocxCtrl)
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	BSTR GetVersion();

// Event maps
	//{{AFX_EVENT(CVwcliocxCtrl)
	void FireOnConnect(LPDISPATCH pWorld)
		{FireEvent(eventidOnConnect,EVENT_PARAM(VTS_DISPATCH), pWorld);}
	void FireOnDisconnect(LPDISPATCH pWorld)
		{FireEvent(eventidOnDisconnect,EVENT_PARAM(VTS_DISPATCH), pWorld);}

	void FireOnUserConnect(LPDISPATCH User)
		{FireEvent(eventidOnUserConnect,EVENT_PARAM(VTS_DISPATCH), User);}
	void FireOnUserDisconnect(LPDISPATCH user)
		{FireEvent(eventidOnUserDisconnect,EVENT_PARAM(VTS_DISPATCH), user);}
	void FireOnUserReconnect(LPDISPATCH VWClient, LPDISPATCH World, LPDISPATCH User)
		{FireEvent(eventidOnUserReconnect,EVENT_PARAM(VTS_DISPATCH  VTS_DISPATCH  VTS_DISPATCH), VWClient, World, User);}

	void FireOnTrace(LPCTSTR bstr)
		{FireEvent(eventidOnTrace,EVENT_PARAM(VTS_BSTR), bstr);}
	void FireOnReport(LPCTSTR bstr, long lType)
		{FireEvent(eventidOnReport,EVENT_PARAM(VTS_BSTR VTS_I4), bstr, lType);}

	void FireOnUIEvent(LPDISPATCH Thing, LPCTSTR bstr, VARIANT varArg)
		{FireEvent(eventidOnUIEvent,EVENT_PARAM(VTS_DISPATCH  VTS_BSTR  VTS_VARIANT), Thing, bstr, &varArg);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CVwcliocxCtrl)
	dispidVWClient = 1L,
	dispidVersion = 2L,

	eventidOnConnect = 1L,
	eventidOnDisconnect = 2L,
	
	eventidOnUserConnect = 3L,
	eventidOnUserDisconnect = 4L,
	eventidOnUserReconnect = 5L,

	eventidOnTrace = 6L,
	eventidOnReport = 7L,

	eventidOnUIEvent = 8L,
	//}}AFX_DISP_ID
	};

	virtual HRESULT HandleOnConnect(IDispatch* pWorld)
	{
		FireOnConnect( pWorld );
		return S_OK; 
	};

	virtual HRESULT HandleOnDisconnect(IDispatch* pWorld)
	{
		FireOnDisconnect( pWorld );
		return S_OK; 
	};

	virtual HRESULT HandleOnUserConnect(IThing* pthing, BOOL bFromClient)
	{
		if (bFromClient)
			FireOnUserConnect( pthing );
		return S_OK; 
	};

	virtual HRESULT HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient)
	{
		if (bFromClient)
			FireOnUserDisconnect( pthing );
		return S_OK; 
	};

	virtual HRESULT HandleOnUserReconnect(IVWClient* newClient, IWorld* pWorld, IThing* pthing)
	{
		FireOnUserReconnect(newClient, pWorld, pthing);
		return S_OK; 
	};

	virtual HRESULT HandleOnTrace( BSTR bstr,BOOL bFromClient)
	{
		if ( bFromClient) FireOnTrace( CString(bstr) );
		return S_OK; 
	};

	virtual HRESULT HandleOnReport( BSTR bstr, long lType, BOOL bFromClient)
	{
		if ( bFromClient) FireOnReport( CString(bstr) , lType);
		return S_OK; 
	};

	virtual HRESULT HandleOnUIEvent(IThing *pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient)
	{
		if (bFromClient)
			FireOnUIEvent(pthing, CString(bstrEventName), varArg);
		return S_OK;
	}
};
