// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWCliOCX.h : Declaration of the CVWClientOCX

#ifndef __VWCLIENTOCX_H_
#define __VWCLIENTOCX_H_

#include <vwclient.h>

#include <propbase.h>
#include <vwutils.h>

#include "resource.h"       // main symbols

EXTERN_C const CLSID CLSID_VWClientOCX;
EXTERN_C const IID DIID__VWClientOCX;

template <class T>
class CProxy_VWClientOCX : public IConnectionPointImpl<T, &DIID__VWClientOCX, CComDynamicUnkArray>
{
public:
//methods:
//_VWClientOCX : IDispatch
public:
	void FireOnConnect(IWorld* pWorld, VARIANT_BOOL bConnect)
	{
		T* pT = (T*)this;
		pT->Lock();
		IUnknown** pp = m_vec.begin();
		while (pp < m_vec.end())
		{
			if (*pp != NULL)
			{
				IDispatch* pDispatch = reinterpret_cast<IDispatch*>(*pp);
				pDispatch->Invoke(bConnect ? 0x1 : 0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, 
					CComDISPPARAMS(1, CComVariant(pWorld)), NULL, NULL, NULL);
			}
			pp++;
		}
		pT->Unlock();
	};

	void FireOnUserConnect(IThing* pThing, VARIANT_BOOL bConnect)
	{
		T* pT = (T*)this;
		pT->Lock();
		IUnknown** pp = m_vec.begin();
		while (pp < m_vec.end())
		{
			if (*pp != NULL)
			{
				IDispatch* pDispatch = reinterpret_cast<IDispatch*>(*pp);
				pDispatch->Invoke(bConnect ? 0x3 : 0x4, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, 
					CComDISPPARAMS(1, CComVariant(pThing)), NULL, NULL, NULL);
			}
			pp++;
		}
		pT->Unlock();
	};

	void FireOnUserReconnect(IVWClient* newClient, IWorld* pWorld, IThing* pthing)
	{
		T* pT = (T*)this;
		pT->Lock();
		IUnknown** pp = m_vec.begin();
		while (pp < m_vec.end())
		{
			if (*pp != NULL)
			{
				IDispatch* pDispatch = reinterpret_cast<IDispatch*>(*pp);
				pDispatch->Invoke(0x5, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, 
					CComDISPPARAMS(3, CComVariant(newClient), CComVariant(pWorld), CComVariant(pthing)), NULL, NULL, NULL);
			}
			pp++;
		}
		pT->Unlock();
	};

	void FireOnTrace(BSTR bstr)
	{
		T* pT = (T*)this;
		pT->Lock();
		IUnknown** pp = m_vec.begin();
		while (pp < m_vec.end())
		{
			if (*pp != NULL)
			{
				IDispatch* pDispatch = reinterpret_cast<IDispatch*>(*pp);
				pDispatch->Invoke(0x6, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, 
					CComDISPPARAMS(1, CComVariant(bstr)), NULL, NULL, NULL);
			}
			pp++;
		}
		pT->Unlock();
	};

	void FireOnReport(BSTR bstr, long lType)
	{
		T* pT = (T*)this;
		pT->Lock();
		IUnknown** pp = m_vec.begin();
		while (pp < m_vec.end())
		{
			if (*pp != NULL)
			{
				IDispatch* pDispatch = reinterpret_cast<IDispatch*>(*pp);
				pDispatch->Invoke(0x7, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, 
					CComDISPPARAMS(2, CComVariant(bstr), CComVariant(lType)), NULL, NULL, NULL);
			}
			pp++;
		}
		pT->Unlock();
	};

	void FireOnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg)
	{
		T* pT = (T*)this;
		pT->Lock();
		IUnknown** pp = m_vec.begin();
		while (pp < m_vec.end())
		{
			if (*pp != NULL)
			{
				IDispatch* pDispatch = reinterpret_cast<IDispatch*>(*pp);
				pDispatch->Invoke(0x8, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, 
					CComDISPPARAMS(3, CComVariant(pthing), CComVariant(bstrEventName), varArg), NULL, NULL, NULL);
			}
			pp++;
		}
		pT->Unlock();
	};
};

/////////////////////////////////////////////////////////////////////////////
// CVWClientOCX
class ATL_NO_VTABLE CVWClientOCX : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVWClientOCX, &CLSID_VWClientOCX>,
	public CComControl<CVWClientOCX>,
	public IDispatchImpl<IVWClientOCX, &IID_IVWClientOCX, &LIBID_VWSYSTEMLib>,
	public CProxy_VWClientOCX<CVWClientOCX>,
	public IProvideClassInfo2Impl<&CLSID_VWClientOCX, &DIID__VWClientOCX, &LIBID_VWSYSTEMLib>,
	public IPersistStreamInitImpl<CVWClientOCX>,
	public IPersistStorageImpl<CVWClientOCX>,
	public IQuickActivateImpl<CVWClientOCX>,
	public IOleControlImpl<CVWClientOCX>,
	public IOleObjectImpl<CVWClientOCX>,
	public IOleInPlaceActiveObjectImpl<CVWClientOCX>,
	public IViewObjectExImpl<CVWClientOCX>,
	public IOleInPlaceObjectWindowlessImpl<CVWClientOCX>,
	public IDataObjectImpl<CVWClientOCX>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CVWClientOCX>,
	public IPropertyNotifySinkCP<CVWClientOCX>,
	public ISpecifyPropertyPagesImpl<CVWClientOCX>,
	public IObjectSafetyImpl<CVWClientOCX, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
	public IVWClientSite
{
public:
	CVWClientOCX();
	~CVWClientOCX();

	HRESULT FinalConstruct();
	void FinalRelease();

	BSTR GetVersion();

DECLARE_REGISTRY_RESOURCEID(IDR_VWCLIENTOCX)
DECLARE_NOT_AGGREGATABLE(CVWClientOCX)
				  
DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CVWClientOCX)
	COM_INTERFACE_ENTRY_IID(IID_IDispatch, IDispatch)
	COM_INTERFACE_ENTRY(IVWClientOCX)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY_IID(IID_IViewObject2, IViewObjectEx)
	COM_INTERFACE_ENTRY_IID(IID_IViewObject, IViewObjectEx)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY_IID(IID_IOleInPlaceObject, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY_IID(IID_IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IDataObject)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IVWClientSite)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

BEGIN_PROPERTY_MAP(CVWClientOCX)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
END_PROPERTY_MAP()


BEGIN_CONNECTION_POINT_MAP(CVWClientOCX)
	CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
	CONNECTION_POINT_ENTRY(DIID__VWClientOCX)
END_CONNECTION_POINT_MAP()


BEGIN_MSG_MAP(CVWClientOCX)
END_MSG_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IViewObjectEx
	STDMETHOD(GetViewStatus)(DWORD* pdwStatus)
	{
		ATLTRACE(_T("IViewObjectExImpl::GetViewStatus\n"));
		*pdwStatus = 0;
		return S_OK;
	}

	STDMETHOD(get_Version)(BSTR* pbstrVersion)
	{
		if (pbstrVersion == NULL)
			return E_POINTER;

		*pbstrVersion = GetVersion();

		return S_OK;
	}

	STDMETHOD(get_VWClient)(IVWClient** ppVWClient)
	{
		if (ppVWClient == NULL)
			return E_POINTER;

		*ppVWClient = m_pVWClientObject;
		SAFEADDREF(*ppVWClient);

		return S_OK;
	}

#if 0
	STDMETHOD(put_VWClient)(IVWClient* pVWClient)
	{
		SAFERELEASE(m_pVWClientObject);
		m_pVWClientObject = pVWClient;
		SAFEADDREF(m_pVWClientObject);

		return S_OK;
	}
#endif

// IVWClientSite
    STDMETHOD(OnConnect)(IWorld* pWorld);
	STDMETHOD(OnDisconnect)(IWorld* pWorld);

    STDMETHOD(OnUserConnect)(IThing* pthing);
	STDMETHOD(OnUserDisconnect)(IThing* pthing);
	STDMETHOD(OnUserReconnect)(IVWClient* newClient, IWorld* pWorld, IThing* pthing);

	STDMETHOD(OnTrace)(BSTR bstr);
    STDMETHOD(OnReport)(BSTR bstr, long lType);

	STDMETHOD(OnUIEvent)(IThing *pthing, BSTR bstrEventName, VARIANT varArg);

// IVWClientOCX
public:
	IVWClient* m_pVWClientObject;
	DWORD m_dwCookie;
};

#endif //__VWCLIENTOCX_H_
