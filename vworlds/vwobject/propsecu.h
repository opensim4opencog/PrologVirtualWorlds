// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _PROPSECU_H_
#define _PROPSECU_H_

// PropSecu.h : Declaration of the CPropertySecurity

#include <resource.h>
#include <afxtempl.h>
#include <propbase.h>

#define SUPPORT_CACHED_EVENTHANDLERS

//#define TRACE_PROPSEC_ALLOCATION
//#define TRACE_SETUID_ALLOCATION

EXTERN_C const CLSID CLSID_PropertySecurity;

/////////////////////////////////////////////////////////////////////////////
// PropSec

class ATL_NO_VTABLE CPropertySecurity :
	public IDispatchImpl<IPropertySecurity, &IID_IPropertySecurity, &LIBID_VWSYSTEMLib>,
	public ISupportErrorInfoImpl<&IID_IPropertySecurity>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CPropertySecurity, &CLSID_PropertySecurity>
{
friend class CWorldObject;

	DECLARE_AGGREGATABLE(CPropertySecurity)

public:
BEGIN_COM_MAP(CPropertySecurity)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPropertySecurity)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

//DECLARE_NOT_AGGREGATABLE(CPropertySecurity) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

	DECLARE_REGISTRY(CPropertySecurity, _T("VWSYSTEM.PropertySecurity.1"), _T("VWSYSTEM.PropertySecurity"), IDS_PROPERTYSECURITY_DESC, THREADFLAGS_BOTH)

// IPropertySecurity
	IWorld* m_pWorld;

	CComVariant m_var;
	IThing* m_pExemplar;  // The exemplar (or instance) defined on...
	IThing* m_pInstance;  // The thing attached to...

	unsigned long m_ulSecurityFlags;
	unsigned long m_ulBits;
	VARTYPE  m_vtType;
	IID m_IID;
	CComBSTR m_bstrType;

#ifdef SUPPORT_CACHED_EVENTHANDLERS
	// cached method names to speed lookup
	CComBSTR m_bstrOnChanged;
	CComBSTR m_bstrOnAccess;
#endif

private:
	VARIANT_BOOL m_bSlave;

	BOOL ParseString(CString &str, unsigned long &ulFlags, BOOL &bAdd);

	inline HRESULT SetBackPointer(VARIANT var);
	inline HRESULT ClearBackPointer(VARIANT var);
	inline HRESULT SetSecurity(IObjectProperty* pprop);
	inline HRESULT ClearSecurity(IObjectProperty* pprop);

public:
	CPropertySecurity()	: m_pWorld(NULL), m_pExemplar(NULL), m_pInstance(NULL),
		m_ulSecurityFlags(PS_READ | PS_WRITE), m_ulBits(0), m_vtType(VT_VARIANT),
		m_bSlave(VARIANT_FALSE), m_IID(IID_NULL)
	{
#ifdef TRACE_PROPSEC_ALLOCATION
		TRACE("CPropertySecurity::CPropertySecurity, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif

//		memcpy(&m_IID, &IID_IUnknown, sizeof(IID));

		m_bSlave = VARIANT_FALSE;
	}

	~CPropertySecurity()
	{
#ifdef TRACE_PROPSEC_ALLOCATION
		TRACE("CPropertySecurity::~CPropertySecurity, this 0x%0x\n", this);
#endif

		Cleanup();		

		SAFERELEASE(m_pWorld);
	}

	STDMETHOD(Cleanup)();

	STDMETHOD(CopyPermission)(IPropertySecurity *pFrom);

	STDMETHOD(Initialize)( IWorld *pWorld, IThing * pExemplar, IPropertySecurity* pSecurity);

	STDMETHOD(get_World)(IWorld** ppWorld);
	STDMETHOD(put_World)(IWorld* pWorld);

	STDMETHOD(get_Property)(VARIANT* pvar);
	STDMETHOD(put_Property)(VARIANT var);

	STDMETHOD(get_Exemplar)(IThing** ppThing); 
	STDMETHOD(put_Exemplar)(IThing* pthing);

	STDMETHOD(get_Instance)(IThing** ppThing); 
	STDMETHOD(put_Instance)(IThing* pthing);  

	STDMETHOD(put_Permissions)(LONG ulFlags);
	STDMETHOD(get_Permissions)(LONG* ulFlags);	

	STDMETHOD(ChangeMode)(BSTR bstrMode );
	STDMETHOD(QueryMode)(BSTR group, BSTR permission, VARIANT_BOOL *bPermitted);

	STDMETHOD(put_BitState)(long bits, VARIANT_BOOL bSet);
	STDMETHOD(get_BitState)(long bits, VARIANT_BOOL* bSet);

	STDMETHOD(get_Bits)(long *bits);
	STDMETHOD(put_Bits)(long bits);

	STDMETHOD(get_vtType)(VARTYPE *type);
	STDMETHOD(put_vtType)(VARTYPE type);

	STDMETHOD(get_PropertyIID)(BSTR* piid);
	STDMETHOD(put_PropertyIID)(BSTR riid);

	STDMETHOD(get_PropertyIIDExt)(IID* piid);
	STDMETHOD(put_PropertyIIDExt)(REFIID riid);

	STDMETHOD(get_PropertyType)(BSTR* Type);
	STDMETHOD(put_PropertyType)(BSTR Type);

	STDMETHOD(CheckSecurity)(unsigned long ulFlags, VARIANT_BOOL bQuiet, VARIANT_BOOL* pbSuccess);

	STDMETHOD(Marshall)(DWORD dwOptions, IMarshallBuffer* pbuffer);
	STDMETHOD(UnMarshall)(IUnMarshallBuffer* pbuffer);

	STDMETHOD(Enable)() 
	{ 
		m_ulBits |= PSBIT_ENABLED;
		return S_OK;
	};

	
	STDMETHOD(Disable)() 
	{ 
		m_ulBits &= ~(PSBIT_ENABLED);
		return S_OK;
	};
};

typedef CComObject<CPropertySecurity> CComPropertySecurity;

class CSetUID : CComPropertySecurity
{
public:
	CSetUID(IWorld *pWorld, IThing *pInstance, IThing *pExemplar, VARIANT_BOOL bOverride = VARIANT_FALSE) :
		m_pOldSecurity(NULL), m_pOldThis(NULL)
	{
		// REVIEW: not setting parent security
		   
#ifdef TRACE_SETUID_ALLOCATION
		TRACE("CSetUID::CSetUID, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif

		ASSERT(pWorld);
		m_pWorld = pWorld;
		SAFEADDREF(m_pWorld);

		m_pInstance = pInstance;
		SAFEADDREF(m_pInstance);

		m_pExemplar = pExemplar;
		SAFEADDREF(m_pExemplar);

		if (bOverride == VARIANT_TRUE)
			m_ulBits |= PSBIT_SYSTEMOVERRIDE;

		// stash old settings
		m_pWorld->get_This(&m_pOldThis);
		m_pWorld->get_SecurityContext(&m_pOldSecurity);

		// poke in new settings
		m_pWorld->put_This(pInstance);

		IPropertySecurity* pPropSec = (IPropertySecurity*)GetUnknown();

		m_pWorld->put_SecurityContext(pPropSec);

		// artificially addref since destructor will release security context ptr
		pPropSec->AddRef();

#ifdef DEBUG
		VWTRACE(m_pWorld, "SECURITY", VWT_METHOD, "CSetUID::CSetUID: pushed context");
#endif
	}

	~CSetUID()
	{
#ifdef TRACE_SETUID_ALLOCATION
		TRACE("CSetUID::~CSetUID, this 0x%0x\n", this);
#endif

		// return old settings
		m_pWorld->put_This(m_pOldThis);
		m_pWorld->put_SecurityContext(m_pOldSecurity);

		// free old settings
		SAFERELEASE(m_pOldSecurity);
		SAFERELEASE(m_pOldThis);

#ifdef DEBUG
		VWTRACE(m_pWorld, "SECURITY", VWT_METHOD, "CSetUID::~CSetUID: popped context");
#endif
	}

private:
	IPropertySecurity* m_pOldSecurity;
	IThing* m_pOldThis; 
};

#endif