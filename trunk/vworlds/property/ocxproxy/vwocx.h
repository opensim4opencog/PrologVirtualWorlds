// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// vwocx.h : Declaration of the COCXProxyObject

#ifndef __VWOCX_H_
#define __VWOCX_H_

#include <resource.h>
#include <PropBase.h>
#include <objimpl.h>

EXTERN_C const CLSID CLSID_OCXProxy;

/////////////////////////////////////////////////////////////////////////////
// COCXProxyObject

HRESULT WINAPI CustomQI(void* pv, REFIID riid, LPVOID* ppv, DWORD dw);

DECLARE_OBJECTPROPERTY(OCXProxy, LIBID_VWSYSTEMLib, IDS_OCXPROXY_TYPE)
{
DECLARE_AGGREGATABLE(COCXProxyObject)

public:

BEGIN_COM_MAP(COCXProxyObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectProperty)
	COM_INTERFACE_ENTRY(IOCXProxy)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
	COM_INTERFACE_ENTRY_FUNC_BLIND(0,CustomQI)
END_COM_MAP()

//DECLARE_NOT_AGGREGATABLE(CMenuItemObject) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

DECLARE_REGISTRY(COCXProxyObject, _T("VWSYSTEM.OCXProxy.1"), _T("VWSYSTEM.OCXProxy"), IDS_OCXPROXY_DESC, THREADFLAGS_BOTH)
private:
	LPDISPATCH m_pOCX;
	DWORD	m_dwCookie;
	ITypeInfo* m_pEventTypeInfo;
public:
	GUID m_guidDISPEVENTS;

public:
	COCXProxyObject();
	~COCXProxyObject();

	STDMETHOD(get_RefCount)(DWORD *pdwRef);
	STDMETHOD(Terminate)(void);

	STDMETHOD(get_OCX)(/*[out, retval]*/ LPDISPATCH *pVal);
	STDMETHOD(put_OCX)(/*[in]*/ LPDISPATCH newVal);

	STDMETHOD(Advise)(IUnknown *pUnk, ULONG *pdwCookie );
	STDMETHOD(UnAdvise)(IUnknown *pUnk, ULONG dwCookie );
	
	STDMETHODIMP Invoke(DISPID dispidMember,REFIID riid,LCID lcid,WORD wFlags, 
						DISPPARAMS* pdispparams,VARIANT* pvarResult,EXCEPINFO* pexcepinfo,
						UINT* puArgErr);
	ULONG InternalRelease();

	

	
public:
};

#endif //__VWOCX_H_
