// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// ViewCtl.h : Declaration of the CVWViewCtrl OLE control class.

/////////////////////////////////////////////////////////////////////////////
// CVWViewCtrl : See ViewCtl.cpp for implementation.

#ifndef _VIEWCTRL_H_
#define _VIEWCTRL_H_

#include <atlbase.h>

#include <objsafe.h>
#include <vwobject.h>
#include <vwclient.h>

#define ADVISE_CLIENT 0x1
#define ADVISE_WORLD  0x2

#ifdef _DEBUG
//#define HANDLEONEVENT_DEBUG
#endif

class CVWViewCtrl : public COleControl
{
	DECLARE_DYNCREATE(CVWViewCtrl)

// Constructor
public:
	IWorld* m_pWorld;
	IVWClient* m_pVWClient;
	CVWViewCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVWViewCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void OnClose(DWORD dwSaveOption);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual DWORD GetControlFlags();


	//}}AFX_VIRTUAL

// Implementation
protected:
	~CVWViewCtrl();

	HRESULT OnActivateInPlace(BOOL bUIActivate, LPMSG pMsg);
	
	DECLARE_OLECREATE_EX(CVWViewCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CVWViewCtrl)      // GetTypeInfo
#ifdef _VIEW_STANDALONE_CLASS_
	DECLARE_PROPPAGEIDS(CVWViewCtrl)     // Property page IDs
#endif
	DECLARE_OLECTLTYPE(CVWViewCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CVWViewCtrl)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CVWViewCtrl)
	afx_msg LPDISPATCH GetVWClient();
	virtual afx_msg void SetVWClient(LPDISPATCH newValue);
	//}}AFX_DISPATCH


	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CVWViewCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CVWViewCtrl)
	dispidVWClient = 1L,
	};	//}}AFX_DISP_ID
protected:
	ULONG	m_ulFlags;

	// -------- Event implementation -----
	DECLARE_INTERFACE_MAP()
	ULONG m_dwClientCookie;
	ULONG m_dwWorldCookie;
    BEGIN_INTERFACE_PART(ObjectSite, IVWObjectSite)
		STDMETHOD(OnUserConnect)(IThing* pthing);
		STDMETHOD(OnUserDisconnect)(IThing* pthing);
		STDMETHOD(OnUserReconnect)(BSTR bstrURL, BSTR bstrName, BSTR bstrPassword);	

		STDMETHOD(OnUIEvent)(IThing *pthing, BSTR bstrEventName, VARIANT varArg);

		STDMETHOD(OnInvokeMethod)(IThing* pthing, BSTR bstr, DISPPARAMS* pdispparams);

		STDMETHOD(OnPropertyChanged)(IThing* pthing, BSTR bstrName, long lHint, VARIANT varHintData);
		STDMETHOD(OnMethodChanged)(IThing* pthing, BSTR bstrName, long lHint, VARIANT varHintData);
		STDMETHOD(OnModuleChanged)(BSTR bstr, long lHint, VARIANT varHintData);

		STDMETHOD(OnTrace)(BSTR bstr);
		STDMETHOD(OnReport)(BSTR bstr, long lType);
	END_INTERFACE_PART(ObjectSite)

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

	DWORD m_dwSafety;
    BEGIN_INTERFACE_PART(ObjectSafety, IObjectSafety)
		STDMETHOD(GetInterfaceSafetyOptions)(REFIID riid, DWORD *pdwSupportedOptions, DWORD *pdwEnabledOptions);
		STDMETHOD(SetInterfaceSafetyOptions)(REFIID riid, DWORD dwOptionSetMask, DWORD dwEnabledOptions);
	END_INTERFACE_PART(ObjectSafety)

private:
	void ReleaseAllObjects();
	HRESULT OnConnect(LPDISPATCH pWorld) ;
	HRESULT OnDisconnect(LPDISPATCH pWorld) ;
//	HRESULT OnPreDestroy();
	void OnSetClientSite();
	
	BOOL	SyncNew(LPDISPATCH newValue);

	HRESULT AddToUser( IThing* pUser );
	HRESULT RemoveFromUser( IThing* pUser );

protected:
	virtual HRESULT Advise( IUnknown *pUnk,REFIID riid, ULONG *pdwCoookie );
	virtual HRESULT UnAdvise( IUnknown *pUnk,REFIID riid, ULONG dwCoookie );

protected:
	virtual HRESULT HandleOnInvokeMethod(IThing* pthing, BSTR bstrName,DISPPARAMS* pdispparams){ return S_OK; };

	virtual HRESULT HandleOnPropertyChanged(IThing* pthing, BSTR bstr, long lHint, VARIANT varHintData){ return S_OK; };
	virtual HRESULT HandleOnMethodChanged(IThing* pthing, BSTR bstr, long lHint, VARIANT varHintData){ return S_OK; };
	virtual HRESULT HandleOnModuleChanged(BSTR bstr, long lHint, VARIANT varHintData){ return S_OK; };

	virtual HRESULT HandleOnTrace(BSTR bstr,BOOL bFromClient){ return S_OK; };
	virtual HRESULT HandleOnReport(BSTR bstr, long lType, BOOL bFromClient){ return S_OK; };

	virtual HRESULT HandleOnConnect(IDispatch* pWorld){ return S_OK; };
	virtual HRESULT HandleOnDisconnect(IDispatch* pWorld){ return S_OK; };

	virtual HRESULT HandleOnUserConnect(IThing* pthing, BOOL bFromClient);
	virtual HRESULT HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient);
	virtual HRESULT HandleOnUserReconnect(BSTR bstrURL, BSTR bstrName, BSTR bstrPassword,BOOL bFromClient) {return S_OK;};
	virtual HRESULT HandleOnUserReconnect(IVWClient* newClient, IWorld* pWorld, IThing* pthing) {return S_OK;};

	virtual HRESULT HandleOnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient){ return S_OK; };

// ----- END  event sink implementation -----
};
#endif
