// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _CVWObjectPropertyImpl_
#define _CVWObjectPropertyImpl_

#include <atlcom.h>
#include <objsafe.h>
#include <propbase.h>
#include <atlctl.h> // for IObjectSafetyImpl

interface IWorld;
interface IPropertyList;
interface IPropertySecurity;

// define this to print out every constructor/destructor
//#define TRACE_ALLOCATION VWT_NONE
#define TRACE_ALLOCATION VWT_METHOD

//#define MARSHFLAG_TRACE

// This template can replace the implentation macros found in propbase.h and
// propsec.inl.  Rather than using the macros to implement the class you can derive
// from this templated class, that implements IObjectProperty.
//
//How to use...
// The Simple way Implementing IFoo, with CFooObject in fobj.h and fobj.cpp
// the atl generated class looks something like:
// class ATL_NO_VTABLE CFooObject : 
//	public CComObjectRootEx<CComSingleThreadModel>,
//	public CComCoClass<CMenuListObject, &CLSID_FooObject>,
//	public IDispatchImpl<IFooObject, &IID_IFooObject, &LIBID_VWSYSTEM>
// {
// public:
// .......
// }
// Change this to
// DECLARE_OBJECTPROPERTY(Foo, LIBID_VWSYSTEM, IDS_FOO_TYPE) 
// {
//  DECLARE_AGGREGATABLE(CFooObject)
// .....
// }
// This creates a typedef for the parent class CFooObjectBase and adds in all
// the neccessary base classes.  Base class methods can be called via CFooObjectBase::method
// all of the IObjectProperty interfaces and various utilities are implemented.
// Since all of the base class methods are implemented, you may have to override some
// that weren't incuded in the propbase.h macros.
//
// In the implementation file you need to 
// IMPLEMENT_OBJECTPROPERTY(CFooObject), this creates the class data needed for the base class.
//
// If you don't have a well formed Foo, you can use the other macros or
// define the class yourself, substituting IObjectPropertyImpl for IDispatchImpl 
//  
//

#define DECLARE_OBJECTPROPERTY(NAME, LIBID, RESOURCE_ID) \
	DECLARE_OBJECTPROPERTYEX(C##NAME##Object, I##NAME, IID_I##NAME, CLSID_##NAME, LIBID, RESOURCE_ID)

#define DECLARE_OBJECTPROPERTYEX(CLASSNAME, INTERFACENAME, IID_BASE, CLSID_BASE, LIBID, RESOURCE_ID) \
	typedef IObjectPropertyImpl<INTERFACENAME, &IID_BASE, &CLSID_BASE, &LIBID, RESOURCE_ID> CLASSNAME##Base; \
	DECLARE_ATLCLASS(CLASSNAME, CLASSNAME##Base, IID_BASE, CLSID_BASE)

#define DECLARE_ATLCLASS(CLASSNAME, BASE_CLASS, IID_BASE, CLSID_BASE) \
	class ATL_NO_VTABLE CLASSNAME : \
	public BASE_CLASS,\
	public CComObjectRootEx<CComMultiThreadModel>, \
	public CComCoClass<CLASSNAME, &CLSID_BASE>, \
	public IObjectSafetyImpl<CLASSNAME, INTERFACESAFE_FOR_UNTRUSTED_CALLER>, \
	public ISupportErrorInfoImpl<&IID_BASE>

// NOTE: you must put a prototype for get_RefCount in your class definition; the implementation
// is provided here

#define IMPLEMENT_OBJECTPROPERTY(CLASSNAME) \
	CString CLASSNAME##Base::g_strIDSType;  \
											 \
	STDMETHODIMP CLASSNAME::get_RefCount(DWORD *pdwRef) \
	{ \
		ASSERT(pdwRef); \
		if (pdwRef == NULL) \
		{ \
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, CLASSNAME##Base::g_strIDSType + "::get_RefCount: outgoing pointer (DWORD*) is NULL\n");  \
			return E_POINTER; \
		} \
		\
		*pdwRef = m_dwRef; \
		\
		return S_OK; \
	}

template <class T, const IID* IID_BASE, const CLSID* CLSID_BASE, const GUID* LIBID, long IDS_TYPE>
class ATL_NO_VTABLE IObjectPropertyImpl : 
	public IDispatchImpl<T, IID_BASE, LIBID>
{
public:
	CComBSTR m_bstrType;

	IWorld* m_pWorld;

	IThing* m_pParent;
	IPropertySecurity *m_pSecurity;
	CComBSTR m_bstrPropertyName;

	static CString g_strIDSType;

	IObjectPropertyImpl() : m_pSecurity(NULL), m_pParent(NULL), m_pWorld(NULL)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		if (g_strIDSType.GetLength() == 0)
		{
			g_strIDSType.LoadString(IDS_TYPE);
		}
		m_bstrType = g_strIDSType;
	}

	virtual ~IObjectPropertyImpl() 
	{
		Terminate();
	}

	STDMETHOD(get_CLSID)(CLSID* pclsid) 
	{ 
		ASSERT(pclsid); 
		if (pclsid == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, g_strIDSType + "::get_CLSID: outgoing pointer (CLSID*) is NULL\n"); 
			return E_POINTER;
		}

		*pclsid = *CLSID_BASE; 

		return S_OK; 
	}; 

	
	STDMETHOD(get_IID)(IID* piid) 
	{ 
		ASSERT(piid); 
		if (piid == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, g_strIDSType + "::get_CLSID: outgoing pointer (CLSID*) is NULL\n"); 
			return E_POINTER;
		}

		*piid = *IID_BASE; 

		return S_OK; 
	}; 
 
	STDMETHOD(get_World)(IWorld** ppWorld) 
	{ 
		ASSERT(ppWorld);
		if (ppWorld == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, g_strIDSType + "::get_World: outgoing pointer (IWorld**) is NULL\n"); 
			return E_POINTER;
		}

		*ppWorld = m_pWorld; 
		SAFEADDREF(*ppWorld); 
 
		return S_OK; 
	}; 
 
	STDMETHOD(Terminate)(void)
	{
		SAFERELEASE(m_pSecurity); 
		SAFERELEASE(m_pParent);

		// REVIEW: if we null out m_pWorld, derived classes can't use m_pWorld 
		// in their Terminate methods unless they call CFooBase::Terminate last
		SAFERELEASE(m_pWorld);

		return S_OK;
	}

	STDMETHOD(put_World)(IWorld* pworld)
	{
		// NOTE: pworld can be NULL

		// NOTE: will be called multiple times when reconnecting to new worlds

		SAFERELEASE(m_pWorld);
		m_pWorld = pworld;
		SAFEADDREF(m_pWorld);

		return S_OK;
	}

	STDMETHOD(get_Parent)(IThing** ppThing) 
	{ 
		ASSERT(ppThing); 
		if (ppThing == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, g_strIDSType + "::get_Parent: outgoing pointer (IThing**) is NULL\n"); 
			return E_POINTER;
		}

		*ppThing = m_pParent; 
		SAFEADDREF(*ppThing); 

		return S_OK; 
	}; 
 
	STDMETHOD(put_Parent)(IThing* pthing) 
	{ 
		SAFERELEASE(m_pParent); 
		m_pParent = pthing; 
		SAFEADDREF(m_pParent); 
 
		return S_OK; 
	}; 

	STDMETHOD(get_Security)(IPropertySecurity** ppSecurity) 
	{ 
		ASSERT(ppSecurity); 
		if (ppSecurity == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, g_strIDSType + "::get_Security: outgoing pointer (IPropertySecurity**) is NULL\n"); 
			return E_POINTER;
		}

		*ppSecurity = m_pSecurity; 
		SAFEADDREF(*ppSecurity); 

		return S_OK; 
	}; 
 
	STDMETHOD(put_Security)(IPropertySecurity* pSecurity) 
	{ 
		SAFERELEASE(m_pSecurity); 
		m_pSecurity = pSecurity; 
		SAFEADDREF(m_pSecurity); 
 
		return S_OK; 
	}; 

	STDMETHOD(get_PropertyName)(BSTR* pbstr)  
	{ 
		ASSERT(pbstr); 
		if (pbstr == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, g_strIDSType + "::get_PropertyName: outgoing pointer (BSTR*) is NULL\n"); 
			return E_POINTER;
		}

		*pbstr = m_bstrPropertyName.Copy(); 

		return S_OK; 
	}; 
 
	STDMETHOD(put_PropertyName)(BSTR bstr) 
	{ 
		m_bstrPropertyName = bstr; 

		return S_OK; 
	}; 

	STDMETHOD(get_Type)(BSTR* pbstr)  
	{ 
		ASSERT(pbstr); 
		if (pbstr == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, g_strIDSType + "::get_Type: outgoing pointer (BSTR*) is NULL\n"); 
			return E_POINTER;
		}

		*pbstr = m_bstrType.Copy(); 

		return S_OK; 
	}; 
 
	STDMETHOD(put_Type)(BSTR bstr) 
	{ 
		HRESULT hr = S_OK;
		VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
		VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
		VARIANT_BOOL bShouldNotify = VARIANT_TRUE;
		long psbits = 0L;

		ASSERT(bstr);
		if (bstr == NULL)
		{
			VWTRACE(m_pWorld, "VWPROPERTY", VWT_ERROR, "IObjectPropertyImpl::put_Type: incoming argument (BSTR) is NULL\n");
			return E_INVALIDARG;
		}

		GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "IObjectPropertyImpl::put_Type: type (%s)\n", CString(bstr));
#endif

		if (bShouldRemote == VARIANT_TRUE)
		{
			GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);
		}

		if (bShouldExecute == VARIANT_TRUE)
		{
			if (!SAFE_CHECKSECURITY(m_pWorld, m_pSecurity, PS_WRITE))
			{
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}

			m_bstrType = bstr;
		}

		if (bShouldRemote == VARIANT_TRUE)
		{
			DISPPARAMS dispparams;

			// package up args into DISPPARAMS struct
			hr = InitializeDISPPARAMS(&dispparams, 1);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			dispparams.rgvarg[0].vt = VT_BSTR;
			dispparams.rgvarg[0].bstrVal = bstr;

			hr = SendObjectPropertyCommand(PSBIT_NONE, DISPID_PROPERTY_TYPE, DISPATCH_PROPERTYPUT, &dispparams);

			TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
		}

		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	
		if (bShouldNotify == VARIANT_TRUE)
		{
			if (m_pParent)
			{
				// notify client/server and world of property change
				hr = m_pParent->NotifyObjectPropertyChanged(this, opType, CComVariant());
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;
			}
		}
		
	ERROR_ENCOUNTERED:
		return hr; 
	};

	STDMETHOD(IsOfType)(BSTR bstr, VARIANT_BOOL* pbool) 
	{ 
		ASSERT(bstr);
		if (bstr == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, g_strIDSType + "::IsOfType: incoming argument (BSTR) is NULL\n"); 
			return E_INVALIDARG;
		}

		ASSERT(pbool);
		if (pbool == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, g_strIDSType + "::IsOfType: outgoing pointer (VARIANT_BOOL*) is NULL\n"); 
			return E_POINTER;
		}

		if (CompareElements(&bstr, &m_bstrType.m_str))
			*pbool = VARIANT_TRUE;
		else
			*pbool = VARIANT_FALSE;

		return S_OK; 
	}; 
 
	STDMETHOD(Dump)(void)
	{
		VWTRACE(m_pWorld, "VWPROPERTY", VWT_VERBOSE, g_strIDSType + ": begin\n");

		VWTRACE(m_pWorld, "VWPROPERTY", VWT_VERBOSE, g_strIDSType + ": end\n");

		return S_OK;
	}

	STDMETHOD(Clone)(IObjectProperty* pobjectproperty)
	{
		HRESULT hr = S_OK;

		CVWComPtr<T, NULL, IID_BASE> pProp = pobjectproperty;

		if (pProp == NULL)
		{
			hr = VWOBJECT_E_TYPE_MISMATCH;
			goto ERROR_ENCOUNTERED;
		}

		hr = HandleClone(pProp);

ERROR_ENCOUNTERED:		
		return hr;
	}

	virtual HRESULT HandleClone(T* pPropPtr) 
	{
		// base class no-op
		return S_OK;
	};

	STDMETHOD(ToAscii)(BSTR* pbstr)
	{
		ASSERT(pbstr);
		if (pbstr == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, g_strIDSType + "::ToAscii: outgoing pointer (BSTR*) is NULL\n"); 
			return E_POINTER;
		}

		*pbstr = m_bstrType.Copy();

		return S_OK;
	}

	STDMETHOD(Marshall)(DWORD dwOptions, IMarshallBuffer* pbuffer)
	{
		HRESULT hr = S_OK; 
#ifdef OPTIMIZE_TYPE_MARSHALLING
		CComBSTR bstrIDSType = g_strIDSType;
#endif

		VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, g_strIDSType + "::Marshall: marshalling parent (by ref)\n"); 

		// REVIEW if parent == NULL, these should still be marshalled

#ifdef OPTIMIZE_TYPE_MARSHALLING
		// if type is same as default type for property class, don't marshall
		if (CompareElements(&m_bstrType.m_str, &bstrIDSType.m_str))
		{
			/* hr = */ pbuffer->put_Byte(0); 
		}
		else
		{
			/* hr = */ pbuffer->put_Byte(1); 
#endif

			hr = pbuffer->put_String(m_bstrType); 

#ifdef OPTIMIZE_TYPE_MARSHALLING
		}
#endif

//ERROR_ENCOUNTERED:		
		return hr;
	}

	STDMETHOD(UnMarshall)(IUnMarshallBuffer* pbuffer)
	{ 
		HRESULT hr = S_OK; 
#ifdef OPTIMIZE_TYPE_MARSHALLING
		unsigned char tag = 0; 
#endif

		VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, g_strIDSType + "::UnMarshall: unmarshalling parent\n");

		// clear out data
//		SAFERELEASE(m_pSecurity); 
		SAFERELEASE(m_pParent); 
		m_bstrPropertyName.Empty(); 
		m_bstrType.Empty(); 

#ifdef OPTIMIZE_TYPE_MARSHALLING
		hr = pbuffer->get_Byte(&tag); 
		if (FAILED(hr)) 
			goto ERROR_ENCOUNTERED;

		if (tag) 
		{ 
#endif

			hr = pbuffer->get_String(&m_bstrType.m_str); 

#ifdef OPTIMIZE_TYPE_MARSHALLING
		}
		else
		{
			// set to default for property class
			m_bstrType = g_strIDSType;
		}
#endif
		
//ERROR_ENCOUNTERED:
		return hr;
	}

	STDMETHOD(MarshallToScript)(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut)
	{ 
		ASSERT(pbstrOut);
		ASSERT(pbAsVal);

		*pbstrOut = NULL;
		*pbAsVal = VARIANT_TRUE;

		return S_OK;
	};
 
	// common utils...
	virtual HRESULT SendObjectPropertyCommand(long lFlags, DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams)
	{
		if (m_pWorld == NULL)
			return VWOBJECT_E_INVALIDWORLD;
		else
			return m_pWorld->SendObjectPropertyCommand(lFlags, this, dispid, wFlags, pdispparams);
	};

	void ValidatePropertyName(long psbits, CString& strPropertyName, char* pzMethod, char* pzModule)
	{
		if ((strPropertyName.Find("LOCAL:") == 0) &&
			!(psbits & PSBIT_LOCAL))
		{
			VWTRACE(m_pWorld, pzModule, VWT_ERROR, CString(pzMethod) + ": %s does not have PSBIT_LOCAL set\n", strPropertyName);
		}

		if ((strPropertyName.Find("VOLATILE:") == 0) &&
			!(psbits & PSBIT_VOLATILE))
		{
			VWTRACE(m_pWorld, pzModule, VWT_ERROR, CString(pzMethod) + ": %s does not have PSBIT_VOLATILE set\n", strPropertyName);
		}
		
		if ((strPropertyName.Find("BYVAL:") == 0) &&
			!(psbits & PSBIT_BYVAL))
		{
			VWTRACE(m_pWorld, pzModule, VWT_ERROR, CString(pzMethod) + ": %s does not have PSBIT_BYVAL set\n", strPropertyName);
		}
	}

	virtual void GetMarshallingFlags(VARIANT_BOOL *bShouldExecute, VARIANT_BOOL *bShouldRemote, VARIANT_BOOL* bShouldNotify)
	{
		// NOTE: make sure to sync any changes with thingobj.h, comobj.h, scrptobj.h

		if (m_pParent != NULL)
		{
			if (m_pWorld)
			{
				m_pWorld->get_ShouldExecute(bShouldExecute);

				m_pWorld->get_ShouldRemote(bShouldRemote);

				m_pWorld->get_ShouldNotify(bShouldNotify);

#ifdef MARSHFLAG_TRACE
				{
					CComBSTR bstrName;

					/* hr = */ m_pParent->get_Name(&bstrName.m_str);

					TRACE("CThingObject::GetMarshallingFlags: name (%s), property (%s), execute %s, remote %s, notify %s\n",
						CString(bstrName),
						CString(m_bstrPropertyName),
						*bShouldExecute ? "yes" : "no",
						*bShouldRemote ? "yes" : "no",
						*bShouldNotify ? "yes" : "no");
				}
#endif
			}
		}
	};

	virtual void GetMarshallingBits(long* psbits, VARIANT_BOOL *bShouldExecute, VARIANT_BOOL *bShouldRemote, VARIANT_BOOL *bShouldNotify)
	{
		if (m_pSecurity)
			/* hr = */ m_pSecurity->get_Bits(psbits);

		VARIANT_BOOL bServerSide = VARIANT_FALSE;
		VARIANT_BOOL bConnected = VARIANT_FALSE;

		/* hr = */ m_pWorld->get_ServerSide(&bServerSide);
		/* hr = */ m_pWorld->get_IsConnected(&bConnected);

		VARIANT_BOOL bConnectLocal = (bServerSide == VARIANT_FALSE && bConnected == VARIANT_FALSE) ? VARIANT_TRUE : VARIANT_FALSE;

		// if we're neither remoting or logging (on server), don't go into the remoting/logging block
		// if we're not remoting (on client), don't go into the remoting/logging block
		// if ConnectLocal mode, assume PSBIT_NOTREMOTED
		if ((*psbits & PSBIT_NOTREMOTED) || bConnectLocal == VARIANT_TRUE)
		{
			// if client-side and in ConnectLocal mode, or if server-side, also look at PSBIT_NOTLOGGED bit 
			if (bConnectLocal == VARIANT_TRUE || bServerSide == VARIANT_TRUE)
			{
				if (*psbits & PSBIT_NOTLOGGED)
					*bShouldRemote = VARIANT_FALSE;
				else
				{
					// if we're not remoting, let's check if logging is enabled
					// if it isn't, let's skip the remoting/logging block
					VARIANT_BOOL bLogging = VARIANT_FALSE;

					/* hr = */ m_pWorld->get_IsLoggingEnabled(&bLogging);

					if (bLogging == VARIANT_FALSE)
						*bShouldRemote = VARIANT_FALSE;
				}
			}
			// else, just having PSBIT_NOTREMOTED flag set is enough to skip the remoting/logging block
			else
				*bShouldRemote = VARIANT_FALSE;
		}
	};

	HRESULT ReportError(LPSTR lpszCLSID, HRESULT hr, HRESULT hrLower, HRESULT hrUpper, LPCTSTR lpszComponent = "")
	{
		return ::ReportError(lpszCLSID, *IID_BASE, hr, hrLower, hrUpper, m_pWorld, VWT_ERROR, lpszComponent);
	};
};

#define DECLARE_VWDISPATCH(NAME, LIBID) \
	DECLARE_VWDISPATCHEX(C##NAME##Object, I##NAME, IID_I##NAME, CLSID_##NAME, LIBID)

#define DECLARE_VWDISPATCHEX(CLASSNAME, INTERFACENAME, IID_BASE, CLSID_BASE, LIBID) \
	typedef IVWDispatchImpl<INTERFACENAME, &IID_BASE, &LIBID> CLASSNAME##Base; \
	DECLARE_ATLCLASS(CLASSNAME, CLASSNAME##Base, IID_BASE, CLSID_BASE)

template < class T, const IID* IID_BASE, const GUID* LIBID >
class ATL_NO_VTABLE IVWDispatchImpl : 
	public IDispatchImpl<T, IID_BASE, LIBID>
{
public:
	IWorld* m_pWorld;
	
	IVWDispatchImpl() : m_pWorld(NULL)
	{
	}

	virtual ~IVWDispatchImpl() 
	{
		Terminate();
	}

	STDMETHOD(Initialize)(IWorld* pworld)
	{
		ASSERT(pworld); 

		// weak reference, don't addref
		m_pWorld = pworld;

		return S_OK;
	}

	STDMETHOD(Terminate)()
	{
		// weak reference, don't release
		if (m_pWorld) 
			m_pWorld = NULL;

		return S_OK;
	}

	STDMETHOD(Install)(IModule* pmodule)
	{
		return S_OK;
	};

	STDMETHOD(OnLoad)(/* IModule* pmodule */)
	{
		return S_OK;
	};

	STDMETHOD(OnUnload)(/* IModule* pmodule */)
	{
		return S_OK;
	};

	HRESULT ReportError(LPSTR lpszCLSID, HRESULT hr, HRESULT hrLower, HRESULT hrUpper, LPCTSTR lpszComponent = "")
	{
		return ::ReportError(lpszCLSID, *IID_BASE, hr, hrLower, hrUpper, m_pWorld, VWT_ERROR, lpszComponent);
	};
};
#endif