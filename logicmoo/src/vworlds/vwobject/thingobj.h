// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _THINGOBJ_H_
#define _THINGOBJ_H_

// ThingObj.h : Declaration of the CThingObject

#include <resource.h>

#include <objimpl.h>

EXTERN_C const CLSID CLSID_Thing;
EXTERN_C const CLSID CLSID_PropertySecurity;

typedef CVWComPtr<IPropertySecurity, &CLSID_PropertySecurity, &IID_IPropertySecurity> CComSecurityPtr;

#define ReportThingError(hr) SUCCEEDED(hr) ? hr : ReportError("VWSYSTEM.Thing.1", hr, 0x200, 0x300, "VWOBJECT")

//#define MARSHFLAG_TRACE

// define to disable OnFooAccess and OnFooChanged property bit caching
#define DISABLE_PROPERTYBIT_CACHING

// define to support access/created/modified datestamps
//#define SUPPORT_DATESTAMPS

//#define SUPPORT_CACHED_ISWIZARD

//#define OVERRIDE_INTERNAL_REFCOUNT

class CPropertySecurity;

/////////////////////////////////////////////////////////////////////////////
// VWOBJECT

//REVIEW -- using pointers to ID's is necessary because some compilers don't like
//references as template arguments.

DECLARE_OBJECTPROPERTY(Thing, LIBID_VWSYSTEMLib, IDS_THING_TYPE)
{
friend class CPropertySecurity;
friend class CWorldObject;

	DECLARE_AGGREGATABLE(CThingObject)

public:
BEGIN_COM_MAP(CThingObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectProperty)
	COM_INTERFACE_ENTRY(IThing)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

	CThingObject();
	~CThingObject();

	DECLARE_REGISTRY(CLSID_Thing, "VWSYSTEM.Thing.1", "VWSYSTEM.Thing", IDS_THING_DESC, THREADFLAGS_BOTH)

// Overrides of IDispatchImpl
public:
	STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR* rgszNames, UINT cNames,
		LCID lcid, DISPID* rgdispid);

	STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid,
		LCID lcid, WORD wFlags, DISPPARAMS* pdispparams, VARIANT* pvarResult,
		EXCEPINFO* pexcepinfo, UINT* puArgErr);

// IThing
public:
#ifdef OVERRIDE_INTERNAL_REFCOUNT
	ULONG InternalAddRef()
	{
#ifdef _DEBUG
		TRACE("CThingObject::InternalAddRef: id %d, m_cref %d\n", m_lObjectID, m_dwRef);
#endif

		return CComObjectRootEx<CComMultiThreadModel>::InternalAddRef();
	}

	ULONG InternalRelease()
	{
		ULONG dwRef = CComObjectRootEx<CComMultiThreadModel>::InternalRelease();

#ifdef _DEBUG
		TRACE("CThingObject::InternalRelease: id %d, m_cref %d\n", m_lObjectID, dwRef);
#endif

		return dwRef;
	}
#endif

	STDMETHOD(get_ID)(long* pid) 
	{ 
		if (pid == NULL)
			return E_POINTER;

		*pid = m_lObjectID; 

		return S_OK; 
	}; 

	STDMETHOD(put_ID)(long id) 
	{ 
		m_lObjectID = id; 

		return S_OK; 
	}; 

public:
	// Override the base class
	STDMETHOD(get_Type)(BSTR* pbstrVal);
	STDMETHOD(put_Type)(BSTR bstrVal);
	STDMETHOD(IsOfType)(BSTR bstrVal, VARIANT_BOOL* pbool);
	STDMETHOD(put_World)(IWorld* pworld);
	STDMETHOD(get_RefCount)(DWORD *pdwRef);
	STDMETHOD(Dump)(); 
	STDMETHOD(ToAscii)(BSTR* pbstrVal); 
	STDMETHOD(Marshall)(DWORD dwOptions, IMarshallBuffer* pbuffer); 
	STDMETHOD(UnMarshall)(IUnMarshallBuffer* pbuffer); 
	STDMETHOD(MarshallToScript)(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut);
	STDMETHOD(Terminate)(void);

	STDMETHOD(put_TypeInt)(BSTR bstrType);
	STDMETHOD(put_OwnerInt)(IThing* pThing);

	void GetMarshallingFlags(VARIANT_BOOL *bShouldExecute, VARIANT_BOOL *bShouldRemote, VARIANT_BOOL *bShouldNotify)
	{
		if (m_pWorld)
		{
			m_pWorld->get_ShouldExecute(bShouldExecute);

			m_pWorld->get_ShouldRemote(bShouldRemote);

			m_pWorld->get_ShouldNotify(bShouldNotify);

#ifdef MARSHFLAG_TRACE
			{
				CComBSTR bstrName;

				/* hr = */ get_Name(&bstrName.m_str);

				TRACE("CThingObject::GetMarshallingFlags: name (%s), execute %s, remote %s, notify %s\n",
					CString(bstrName),
					*bShouldExecute ? "yes" : "no",
					*bShouldRemote ? "yes" : "no",
					*bShouldNotify ? "yes" : "no");
			}
#endif
		}
	};

	void GetMarshallingBits(long psbits, DISPID dispid, VARIANT_BOOL *bShouldExecute, VARIANT_BOOL *bShouldRemote, VARIANT_BOOL *bShouldNotify)
	{
		VARIANT_BOOL bServerSide = VARIANT_FALSE;
		VARIANT_BOOL bConnected = VARIANT_FALSE;

		/* hr = */ m_pWorld->get_ServerSide(&bServerSide);
		/* hr = */ m_pWorld->get_IsConnected(&bConnected);

		VARIANT_BOOL bConnectLocal = (bServerSide == VARIANT_FALSE && bConnected == VARIANT_FALSE) ? VARIANT_TRUE : VARIANT_FALSE;

		if (bConnectLocal == VARIANT_TRUE)
		{
			// if both bits are set, don't remote (special case)
			if (psbits & PSBIT_NOTLOGGED)
				*bShouldRemote = VARIANT_FALSE;
			// if server-side, look at IsLoggingEnabled
			else if (bServerSide == VARIANT_TRUE)
				{
					// if we're not remoting, let's check if logging is enabled
					// if it isn't, let's skip the remoting/logging block
					VARIANT_BOOL bLogging = VARIANT_FALSE;

					/* hr = */ m_pWorld->get_IsLoggingEnabled(&bLogging);

					if (bLogging == VARIANT_FALSE)
						*bShouldRemote = VARIANT_FALSE;
				}
		}
		else if (psbits & PSBIT_NOTREMOTED)
		{
			// if we're on the client
			if (bServerSide == VARIANT_FALSE)
			{
				// if both bits are set, don't remote (special case)
				if (psbits & PSBIT_NOTLOGGED)
					*bShouldRemote = VARIANT_FALSE;

				// skip remoting block for [propput] methods
				if (dispid == DISPID_THING_PROPERTY || 
					dispid == DISPID_THING_METHOD)
					*bShouldRemote = VARIANT_FALSE;
			}
			// else, if we're on the server, look at the logging flags
			else
			{
				// if both bits are set, don't remote (special case)
				if (psbits & PSBIT_NOTLOGGED)
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
		}
	};

// IThing
public:
	// back-ptr helper fns
	inline HRESULT SetPropertyName(BSTR name, VARIANT var);
	inline HRESULT ClearPropertyName(VARIANT var);

	inline HRESULT SetMethodName(BSTR name, IMethod* pmethod);
	inline HRESULT ClearMethodName(IMethod* pmethod);

	// in-world event helper fn
	HRESULT InvokeOnPropertyChanged(BSTR bstrPropName, long psbits, IPropertySecurity* pSecurity, long lHint, VARIANT varHintData);
	HRESULT InvokeOnPropertyAccess(BSTR bstrPropName, long psbits, long perms, IPropertySecurity* pSecurity);

	HRESULT NotifyPropertyChangedExt(BSTR bstrPropName, long psbits, IPropertySecurity* pSecurity, long lHint, VARIANT varHintData);
	HRESULT NotifyMethodChangedExt(BSTR bstrMethodName, long lHint, VARIANT varHintData);

	STDMETHOD(NotifyObjectPropertyChanged)(IObjectProperty* pprop, long lHint, VARIANT varHintData);
	STDMETHOD(NotifyMethodChanged)(IMethod* pmethod, long lHint, VARIANT varHintData);

#ifdef SUPPORT_DATESTAMPS
	STDMETHOD(get_CreatedDate)(DATE* pDate)
	{
		if (pDate == NULL)
			return E_POINTER;

		*pDate = (DATE)m_dateCreated;

		return S_OK;
	};

	STDMETHOD(get_ModifiedDate)(DATE* pDate)
	{
		if (pDate == NULL)
			return E_POINTER;

		*pDate = (DATE)m_dateModified;

		return S_OK;
	};

	STDMETHOD(get_AccessedDate)(DATE* pDate)
	{
		if (pDate == NULL)
			return E_POINTER;

		*pDate = (DATE)m_dateAccessed;

		return S_OK;
	};

	// these three methods set date to current date/time
	HRESULT SetCreatedDate()
	{
		m_dateCreated = COleDateTime::GetCurrentTime();
		return S_OK;
	};

	HRESULT SetModifiedDate()
	{
		m_dateModified = COleDateTime::GetCurrentTime();
		return S_OK;
	};

	HRESULT SetAccessedDate()
	{
		m_dateAccessed = COleDateTime::GetCurrentTime();
		return S_OK;
	};
#endif

	STDMETHOD(get_InstanceType)(BSTR* pbstr);

	STDMETHOD(get_CurrentMethodName)(BSTR* pbstr)
	{
		if (pbstr == NULL)
			return E_POINTER;

		*pbstr = m_bstrCurrentMethod.Copy();

		return S_OK;
	};

	STDMETHOD(put_CurrentMethodName)(BSTR bstr)
	{
		m_bstrCurrentMethod = bstr;

		return S_OK;
	};

	STDMETHOD(get_Exemplar)(IThing** ppthing);
	STDMETHOD(put_ExemplarInt)(IThing* pthing);

	STDMETHOD(get_IsExemplar)(VARIANT_BOOL* pboolVal)
	{
		return get_BOOL(m_bstrIsExemplar, pboolVal);
	};

	STDMETHOD(get_IsWizard)(VARIANT_BOOL* pboolVal)
	{
		static CComBSTR bstrIsWizard("IsWizard");
		HRESULT hr = S_OK;
	
#ifdef SUPPORT_CACHED_ISWIZARD
		static VARIANT_BOOL bIsWizard = VARIANT_FALSE;
		static VARIANT_BOOL bIsWizardCached = VARIANT_FALSE;

		if (bIsWizardCached == VARIANT_FALSE)
		{
			hr = get_BOOL(bstrIsWizard, pboolVal);

			if (SUCCEEDED(hr))
			{
				bIsWizard = *pboolVal;
				bIsWizardCached = VARIANT_TRUE;
			}
		}
		else
			*pboolVal = bIsWizard;
#else
		hr = get_BOOL(bstrIsWizard, pboolVal);
#endif

		return hr;
	};

	STDMETHOD(get_Name)(BSTR* pbstr)
	{
		return get_String(m_bstrName, pbstr);
	};

	STDMETHOD(put_Name)(BSTR bstr)
	{
		return put_String(m_bstrName, bstr);
	};

	STDMETHOD(get_Description)(BSTR* pbstr)
	{
		return get_String(m_bstrDescription, pbstr);
	};

	STDMETHOD(put_Description)(BSTR bstr)
	{
		return put_String(m_bstrDescription, bstr);
	};

	STDMETHOD(get_Contents)(IPropertyList** pppropertylist)
	{
		return get_Dispatch(m_bstrContents, (IDispatch**)pppropertylist);
//		return get_ObjectProperty(m_bstrContents, (IObjectProperty**)pppropertylist);
	};

	STDMETHOD(get_Container)(IThing** ppthing)
	{
		return get_Dispatch(m_bstrContainer, (IDispatch**)ppthing);
//		return get_Thing(m_bstrContainer, ppthing);
	};

	STDMETHOD(put_Container)(IThing* pthing)
	{
		return put_Thing(m_bstrContainer, pthing);
	};

	STDMETHOD(get_Owner)(IThing** ppthing)
	{
		if (ppthing == NULL)
			return E_POINTER;

		*ppthing = m_pOwner;
		SAFEADDREF(*ppthing);

		return S_OK;
	};

	STDMETHOD(CopyPropertiesTo)(IPropertyMap* ppropertymap);

	STDMETHOD(get_InstanceProperties)(IPropertyMap** pppropertymap);
	STDMETHOD(get_Properties)(IPropertyMap** pppropertymap);

	// helpers
	STDMETHOD(CreateAndAddMethod)(long lFlags, IModule *pModule, BSTR bstrName, 
		VARIANT varFuncName, VARIANT varPsbits, VARIANT varPermissions);
	STDMETHOD(CreateAndAddMethodExt)(long lFlags, IModule *pModule,
		BSTR bstrFuncName, BSTR bstrMethName, long psbits, long permissions);
	STDMETHOD(CreateAndAddProperty)(BSTR bstrProgID, BSTR bstrPropName, VARIANT var);

	STDMETHOD(AddProperty)(BSTR name, VARIANT var, VARIANT psbits, VARIANT permissions, VARIANT type,BSTR bstrIID, BSTR bstrType);
	STDMETHOD(AddPropertyExt)(BSTR name, VARIANT var, long psbits, long permissions, long type, REFIID riid, BSTR bstrType);
	STDMETHOD(AddPropertyInt)(BSTR bstrPropertyName, VARIANT var, IThing* pExemplar, 
		long psbits, long permissions, long vtType, BSTR bstrIID, BSTR bstrType);
	
	HRESULT AddPropertyIntHelper(BSTR bstrPropertyName, VARIANT var, IThing *pExemplar, 
		long psbits, long permissions, long vtType, BSTR bstrIID, BSTR bstrType, IPropertySecurity** ppSecurity);

	STDMETHOD(RemoveProperty)(BSTR name);

	STDMETHOD(get_Property)(BSTR name, VARIANT* pvar);
	STDMETHOD(get_PropertyExt)(BSTR name, UINT nHashOrig, IThing** ppattached, VARIANT* pvar);

	STDMETHOD(put_Property)(BSTR name, VARIANT var);

	HRESULT put_PropertyExt(BSTR name, UINT nHashOrig, VARIANT var);
	HRESULT put_PropertyInt(BSTR name, VARIANT var, IPropertySecurity* pSecurity, VARIANT_BOOL fInInstance);

	STDMETHOD(InstanceProperty)(BSTR name, IThing* pExemplar);

	HRESULT InstancePropertyInt(BSTR name, IThing* pExemplar, IPropertySecurity* pSecurity);
	HRESULT InstancePropertyIntHelper(BSTR bstrPropertyName, IObjectProperty* pProperty, IThing* pExemplar, IPropertySecurity* pSecurity, VARIANT_BOOL* pbInstanced);

	STDMETHOD(get_PropertySecurity)(BSTR name, IPropertySecurity** ppSecurity);
	STDMETHOD(get_PropertySecurityExt)(BSTR name, UINT nHashOrig, IThing** ppattached, VARIANT_BOOL* pfInstance, IPropertySecurity** ppSecurity);

	STDMETHOD(get_RemoteProperty)(BSTR name, VARIANT* pvar);

	STDMETHOD(CopyMethodsTo)(IPropertyMap* ppropertymap);

	STDMETHOD(get_InstanceMethods)(IPropertyMap** pppropertymap);
	STDMETHOD(get_Methods)(IPropertyMap** pppropertymap);

	STDMETHOD(AddMethod)(BSTR name, IMethod* pmethod, VARIANT varPsbits, VARIANT varPermissions);
	STDMETHOD(AddMethodExt)(BSTR name, IMethod* pmethod, long psbits, long permissions);
	STDMETHOD(AddMethodInt)(BSTR name, IMethod* pmethod, IThing* pExemplar, long psbits, long permissions);

	HRESULT AddMethodIntHelper(BSTR name, IMethod* pmethod, IThing* pExemplar, long psbits, long permissions);

	STDMETHOD(RemoveMethod)(BSTR name);

	STDMETHOD(get_Method)(BSTR name, IMethod** ppmethod);
	STDMETHOD(get_MethodExt)(BSTR name, UINT nHashOrig, IThing** ppattached, IMethod** ppmethod);

	STDMETHOD(put_Method)(BSTR name, IMethod* pmethod);

	HRESULT put_MethodExt(BSTR name, UINT nHashOrig, IMethod* pmethod);
	HRESULT put_MethodInt(BSTR name, IMethod* pmethod, IPropertySecurity* pSecurity, VARIANT_BOOL fInstance);

	STDMETHOD(get_MethodSecurity)(BSTR name, IPropertySecurity** ppSecurity);
	STDMETHOD(get_MethodSecurityExt)(BSTR name, UINT nHashOrig, IThing** ppattached, VARIANT_BOOL* pfInstance, IPropertySecurity** ppSecurity);

#ifndef DISABLE_PROPERTYBIT_CACHING
	HRESULT CachePropertyBits(BSTR bstrName, VARIANT_BOOL bMethod);
#endif

	STDMETHOD(CheckPropertiesSecurity)(unsigned long ulFlags, VARIANT_BOOL* pboolVal)
	{
		HRESULT hr = S_OK;

		if (pboolVal == NULL)
			return ReportThingError(E_POINTER);

		*pboolVal = VARIANT_TRUE;

		hr = CheckPropertiesSecurityExt(ulFlags);

		// REVIEW: don't return failed HRESULT, just return VARIANT_BOOL
		if (FAILED(hr))
			*pboolVal = VARIANT_FALSE;

		return S_OK;
	};

	STDMETHOD(CheckMethodsSecurity)(unsigned long ulFlags, VARIANT_BOOL* pboolVal)
	{
		HRESULT hr = S_OK;

		if (pboolVal == NULL)
			return ReportThingError(E_POINTER);

		*pboolVal = VARIANT_TRUE;

		hr = CheckMethodsSecurityExt(ulFlags);

		// REVIEW: don't return failed HRESULT, just return VARIANT_BOOL
		if (FAILED(hr))
			*pboolVal = VARIANT_FALSE;

		return S_OK;
	};

	STDMETHOD(CheckPropertiesSecurityExt)(unsigned long ulFlags);
	STDMETHOD(CheckMethodsSecurityExt)(unsigned long ulFlags);

	STDMETHOD(CheckPropertySecurity)(BSTR bstrName, unsigned long ulFlags, VARIANT_BOOL* pboolVal)
	{
		HRESULT hr = S_OK;

		if (pboolVal == NULL)
			return ReportThingError(E_POINTER);

		*pboolVal = VARIANT_TRUE;

		hr = CheckPropertySecurityExt(bstrName, ulFlags);

		// REVIEW: don't return failed HRESULT, just return VARIANT_BOOL
		if (FAILED(hr))
			*pboolVal = VARIANT_FALSE;

		return S_OK;
	};

	STDMETHOD(CheckMethodSecurity)(BSTR bstrName, unsigned long ulFlags, VARIANT_BOOL* pboolVal)
	{
		HRESULT hr = S_OK;

		if (pboolVal == NULL)
			return ReportThingError(E_POINTER);

		*pboolVal = VARIANT_TRUE;

		hr = CheckMethodSecurityExt(bstrName, ulFlags);

		// REVIEW: don't return failed HRESULT, just return VARIANT_BOOL
		if (FAILED(hr))
			*pboolVal = VARIANT_FALSE;

		return S_OK;
	};

	STDMETHOD(CheckPropertySecurityExt)(BSTR bstrName, unsigned long ulFlags);
	STDMETHOD(CheckMethodSecurityExt)(BSTR bstrName, unsigned long ulFlags);

	HRESULT CheckPropertySecurityExt(BSTR bstrName, IPropertySecurity* pSecurity, VARIANT_BOOL fInInstance, unsigned long ulFlags);
	HRESULT CheckMethodSecurityExt(BSTR bstrName, IPropertySecurity* pSecurity, VARIANT_BOOL fInInstance, unsigned long ulFlags);

	STDMETHOD(MoveIntoExt)(IThing* pWhere);
	STDMETHOD(MoveIntoInt)(IThing* pWhere);

#if 0
	STDMETHOD(GiveEx)(IThing* pWhat, IThing* pWho);
	STDMETHOD(GiveInt)(IThing* pWhat, IThing* pWho);
#endif

	STDMETHOD(FindWithin)(BSTR name, VARIANT varQuiet, IThing** ppthing);
	HRESULT FindWithinEx(IThing* pbase, BSTR bstrName, IThing** ppthing);

	STDMETHOD(FireEvent)(BSTR name, VARIANT varArgs)
	{
		HRESULT hr = S_OK;
		DISPPARAMS dispparams;
		CComVariant varTemp;

		// VT_ERROR means optional arg not passed in
		if (varArgs.vt != VT_ERROR)
		{
			hr = VariantCopyInd(&varTemp, &varArgs);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			hr = ConvertJSArrayToDISPPARAMS(&varTemp, &dispparams);
			if (FAILED(hr))
			{
				hr = ConvertSAFEARRAYToDISPPARAMS(&varTemp, &dispparams);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;
			}
	
			hr = FireEventExt(name, &dispparams, FIREEVENT_DEFAULT);

			TerminateDISPPARAMS(VARIANT_TRUE, &dispparams);
		}
		else
			hr = FireEventExt(name, NULL, FIREEVENT_DEFAULT);

ERROR_ENCOUNTERED:
		return ReportThingError(hr);
	};

	STDMETHOD(FireEventExt)(BSTR name, DISPPARAMS* pdispparams, long lFlags);

	STDMETHOD(FireEventInt)(BSTR name, IPropertyList* pproplist, long lFlags)
	{
		HRESULT hr = S_OK;

		if (pproplist)
		{
			DISPPARAMS dispparams;

			hr = ConvertPropertyListToDISPPARAMS(pproplist, &dispparams);

			if (SUCCEEDED(hr))
				hr = FireEventExtHelper(name, &dispparams, lFlags);

			TerminateDISPPARAMS(VARIANT_TRUE, &dispparams);
		}
		else
			hr = FireEventExtHelper(name, NULL, lFlags);

		return hr;
	};

	HRESULT FireEventExtHelper(BSTR name, DISPPARAMS* pdispparams, long lFlags);

	STDMETHOD(InvokeMethodInt)(IThing* pthing, BSTR name, IPropertyList* pproplist, VARIANT* pvar)
	{
		HRESULT hr = S_OK;

		if (pproplist)
		{
			DISPPARAMS dispparams;

			hr = ConvertPropertyListToDISPPARAMS(pproplist, &dispparams);

			if (SUCCEEDED(hr))
				hr = InvokeMethodExtHelper(pthing, name, &dispparams, pvar);

			TerminateDISPPARAMS(VARIANT_TRUE, &dispparams);
		}
		else
			hr = InvokeMethodExtHelper(pthing, name, NULL, pvar);

		return ReportThingError(hr);
	};

	// pthing is the source for looking up methods, used by 'super' and normal case
	HRESULT InvokeMethodExtHelper(IThing* pthing, BSTR name, DISPPARAMS* pdispparams, VARIANT* pvar);

	STDMETHODIMP InvokeMethodExt(BSTR bstrMethodName, DISPPARAMS* pdispparams, VARIANT* pvar)
	{
		return InvokeMethodExtHelper(this, bstrMethodName, pdispparams, pvar);
	};

	STDMETHOD(InvokeMethod)(BSTR name, VARIANT varArgs, VARIANT* pvar)
	{
		HRESULT hr = S_OK;
		DISPPARAMS dispparams;
		CComVariant varTemp;

		// VT_ERROR means optional arg not passed in
		if (varArgs.vt != VT_ERROR)
		{
			hr = VariantCopyInd(&varTemp, &varArgs);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			hr = ConvertJSArrayToDISPPARAMS(&varTemp, &dispparams);
			if (FAILED(hr))
			{
				hr = ConvertSAFEARRAYToDISPPARAMS(&varTemp, &dispparams);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;
			}

			hr = InvokeMethodExtHelper(this, name, &dispparams, pvar);

			TerminateDISPPARAMS(VARIANT_TRUE, &dispparams);
		}
		else
			hr = InvokeMethodExtHelper(this, name, NULL, pvar);

ERROR_ENCOUNTERED:
		return ReportThingError(hr);
	};

	STDMETHOD(InvokeSuperMethodExt)(DISPPARAMS* pdispparams, VARIANT* pvar);

	STDMETHOD(InvokeSuperMethod)(VARIANT varArgs, VARIANT* pvar)
	{
		HRESULT hr = S_OK;
		DISPPARAMS dispparams;
		CComVariant varTemp;

		// VT_ERROR means optional arg not passed in
		if (varArgs.vt != VT_ERROR)
		{
			hr = VariantCopyInd(&varTemp, &varArgs);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			hr = ConvertJSArrayToDISPPARAMS(&varTemp, &dispparams);
			if (FAILED(hr))
			{
				hr = ConvertSAFEARRAYToDISPPARAMS(&varTemp, &dispparams);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;
			}

			hr = InvokeSuperMethodExt(&dispparams, pvar);

			TerminateDISPPARAMS(VARIANT_TRUE, &dispparams);
		}
		else
			hr = InvokeSuperMethodExt(NULL, pvar);

ERROR_ENCOUNTERED:
		return ReportThingError(hr);
	};

	STDMETHOD(IsValidProperty)(BSTR name, VARIANT_BOOL* pbool);
	STDMETHOD(IsValidMethod)(BSTR name, VARIANT_BOOL* pbool);

	STDMETHOD(IsValidInstanceProperty)(BSTR name, VARIANT_BOOL* pbool);
	STDMETHOD(IsValidInstanceMethod)(BSTR name, VARIANT_BOOL* pbool);

// IThingHelper
public: 
	STDMETHOD(AddObjectPropertyExt)(BSTR name, REFIID iid, IObjectProperty* pobjectproperty);
	STDMETHOD(AddString)(BSTR name, BSTR bstr);
	STDMETHOD(AddObjectProperty)(BSTR name, IObjectProperty* pobjectproperty);
	STDMETHOD(AddThing)(BSTR name, IThing* pthing);
	STDMETHOD(AddBOOL)(BSTR name, VARIANT_BOOL boolVal);
	STDMETHOD(AddLong)(BSTR name, long lval);
	STDMETHOD(AddShort)(BSTR name, short sval);
	STDMETHOD(AddDispatch)(BSTR name, IDispatch* pdisp);

	STDMETHOD(get_String)(BSTR name, BSTR* pbstr);
	STDMETHOD(put_String)(BSTR name, BSTR bstr);

	STDMETHOD(get_ObjectPropertyExt)(BSTR name, REFIID iid, IObjectProperty** ppobjectproperty);
	STDMETHOD(put_ObjectPropertyExt)(BSTR name, REFIID iid, IObjectProperty* pobjectproperty);

	STDMETHOD(get_ObjectProperty)(BSTR name, IObjectProperty** ppobjectproperty);
	STDMETHOD(put_ObjectProperty)(BSTR name, IObjectProperty* pobjectproperty);

	STDMETHOD(get_Thing)(BSTR name, IThing** ppthing);
	STDMETHOD(put_Thing)(BSTR name, IThing* pthing);

	STDMETHOD(get_BOOL)(BSTR name, VARIANT_BOOL* pboolVal);
	STDMETHOD(put_BOOL)(BSTR name, VARIANT_BOOL boolVal);

	STDMETHOD(get_Long)(BSTR name, long* plval);
	STDMETHOD(put_Long)(BSTR name, long lval);

	STDMETHOD(get_Short)(BSTR name, short* psval);
	STDMETHOD(put_Short)(BSTR name, short sval);

	STDMETHOD(get_Dispatch)(BSTR name, IDispatch** ppdisp);
	STDMETHOD(put_Dispatch)(BSTR name, IDispatch* pdisp);

	STDMETHOD(get_IsStub)(VARIANT_BOOL* pboolVal)
	{
		if (pboolVal == NULL)
			return E_POINTER;

		*pboolVal = m_bStub;

		return S_OK;
	};
	STDMETHOD(put_IsStubInt)(VARIANT_BOOL boolVal);

	HRESULT LogThingCommand(DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams)
	{
		if (m_pWorld == NULL)
			return VWOBJECT_E_INVALIDWORLD;
		else
			return m_pWorld->LogThingCommand(this, dispid, wFlags, pdispparams);
	};

	HRESULT SendThingCommand(long lFlags, DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams)
	{
		if (m_pWorld == NULL)
			return VWOBJECT_E_INVALIDWORLD;
		else
			return m_pWorld->SendThingCommand(lFlags, this, dispid, wFlags, pdispparams);
	};

	HRESULT SendSyncThingCommand(long lFlags, DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams, HRESULT* recvhr, VARIANT* pvar)
	{
		if (m_pWorld == NULL)
			return VWOBJECT_E_INVALIDWORLD;
		else
			return m_pWorld->SendThingCommandExt(VARIANT_TRUE, VARIANT_TRUE, lFlags, this, dispid, wFlags, pdispparams, recvhr, pvar);
	};

// IThing
private:
	// pointer to parent in inheritance path (i.e. exemplar)
	IThing* m_pExemplar;

	// current method name (during InvokeMethod)
	CComBSTR m_bstrCurrentMethod;

	// instance properties
	IPropertyMap* m_pProperties;
	CComSecurityPtr m_PropSecurity;

	// instance methods
	IPropertyMap* m_pMethods;
	CComSecurityPtr m_MethodSecurity;

	// pre-alloced list for converting DISPPARAMS
	IPropertyList* m_pArgList;

	// unique object ID
	long m_lObjectID;

	COleDateTime m_dateCreated;
	COleDateTime m_dateModified;
	COleDateTime m_dateAccessed;

	// pointer to owner
	IThing* m_pOwner;

	// stub object?
	VARIANT_BOOL m_bStub;

	// helpers for IDispatch overriding
	BOOL IsDoctoredId(DISPID id, VARIANT_BOOL *pbProp);
	HRESULT IdFromName(LPOLESTR psz, DISPID* pid);
	HRESULT NameFromId(DISPID id, BSTR *pbstr);
	CComBSTR m_bstrCached;
	DISPID m_idCached;
	DISPID m_idNext;

	// other helpers
	HRESULT InstancePropertyInt(BSTR name, UINT nHashOrig, IThing** ppattached, VARIANT* pvar);


	// pre-loaded strings
	static CComBSTR m_bstrParentEvent;
	static CComBSTR m_bstrChildEvent;
	static CComBSTR m_bstrSelfEvent;
	static CComBSTR m_bstrPeerEvent;

	// allow IWorld to access
public:
	static CComBSTR m_bstrOn;
	static CComBSTR m_bstrChanged;
	static CComBSTR m_bstrAccess;

	static CString m_strOn;
	static CString m_strChanged;
	static CString m_strAccess;

	static CComBSTR m_bstrEnter;
	static CComBSTR m_bstrLeave;
	static CComBSTR m_bstrSystemEnter;
	static CComBSTR m_bstrSystemLeave;
	static CComBSTR m_bstrName;
	static CComBSTR m_bstrContents;
	static CComBSTR m_bstrContainer;
	static CComBSTR m_bstrDescription;
//	static CComBSTR m_bstrOwner;
	static CComBSTR m_bstrWielded;
	static CComBSTR m_bstrIsFertile;
	static CComBSTR m_bstrIsCloneable;
	static CComBSTR m_bstrIsExemplar;
	static CComBSTR m_bstrIsOpen;
	static CComBSTR m_bstrIsNoticeable;
	static CComBSTR m_bstrThing;
};

#endif // _THINGOBJ_H_