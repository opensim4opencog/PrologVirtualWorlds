// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// WorldObj.h : Declaration of the CWorldObject
#include <COMDEF.H>
#include <afxtempl.h>

// V-Worlds optimized templates
#include <vwtempl.h>

#include <resource.h>

#include <vwodb.h>		// object repository

#include <menuitem.h>
#include <objimpl.h>

#include "connlist.h"
#include "marshbuf.h"
#include "propsecu.h"
#include "thingobj.h"
#include "reghelp.h"

typedef CComObject<CMarshallBufferObject> CComMarshallBuffer;
typedef CComObject<CUnMarshallBufferObject> CComUnMarshallBuffer;
typedef CVWComPtr<IObjectProperty, NULL, &IID_IObjectProperty> CObjectPropertyPtr;
typedef CVWComPtr<IThing, NULL, &IID_IThing> CThingPtr;

EXTERN_C const CLSID CLSID_World;

// defining this sends the current callstack to the server with commands
#define SUPPORT_SERVER_CALLSTACK

// #define SUPPORT_CACHED_ENABLESECURITY

// duped from vwodbobj.h
#define COPYBUFFER_SIZE 32768
#define WORLDFILEEXTENSIONSIZE 8  /* length of ".vwc.bak", used in ValidateWorldNameExt */

#define ReportWorldError(hr) SUCCEEDED(hr) ? hr : ReportError("VWSYSTEM.World.1", hr, 0x200, 0x300, "VWOBJECT")

class CThingObject;
class CPropertySecurity;
class CCOMModuleObject;
class CScriptModuleObject;
class CMethodObject;
class CPropertyListObject;
class CPropertyMapObject;

/////////////////////////////////////////////////////////////////////////////
// VWOBJECT

//REVIEW -- using pointers to ID's is necessary because some compilers don't like
//references as template arguments.

DECLARE_OBJECTPROPERTY(World, LIBID_VWSYSTEMLib, IDS_WORLD_TYPE),
	public IConnectionPointContainerImpl<CWorldObject>,
	public IConnectionPointImpl<CWorldObject, &IID_IVWObjectSite, CComDynamicUnkArray>
{
friend class CThingObject;
friend class CPropertySecurity;
friend class CCOMModuleObject;
friend class CScriptModuleObject;
friend class CMethodObject;
friend class CPropertyListObject;
friend class CPropertyMapObject;

	DECLARE_AGGREGATABLE(CWorldObject)

public:
	CWorldObject();
	~CWorldObject();

BEGIN_COM_MAP(CWorldObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectProperty)
	COM_INTERFACE_ENTRY(IWorld)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

// Connection Point
BEGIN_CONNECTION_POINT_MAP(CWorldObject)
	CONNECTION_POINT_ENTRY(IID_IVWObjectSite)
END_CONNECTION_POINT_MAP()

	DECLARE_REGISTRY(CLSID_World, "VWSYSTEM.World.1", "VWSYSTEM.World", IDS_WORLD_DESC, THREADFLAGS_BOTH)

// Overrides of IDispatchImpl
public:
	STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR* rgszNames, UINT cNames,
		LCID lcid, DISPID* rgdispid);

	STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid,
		LCID lcid, WORD wFlags, DISPPARAMS* pdispparams, VARIANT* pvarResult,
		EXCEPINFO* pexcepinfo, UINT* puArgErr);

	STDMETHOD(Advise)(IUnknown* pUnkSink, DWORD* pdwCookie);
	STDMETHOD(Unadvise)(DWORD dwCookie);

public:
	// override base class
	STDMETHOD(put_World)(IWorld* pworld);
	STDMETHOD(Dump)();
	STDMETHOD(get_RefCount)(DWORD *pdwRef);

	void GetMarshallingFlags(VARIANT_BOOL *bShouldExecute, VARIANT_BOOL *bShouldRemote)
	{
		// NOTE: overriding base to support sync methods by default
		get_ShouldExecuteSync(bShouldExecute);

		get_ShouldRemoteSync(bShouldRemote);
	};

// IWorld
public:

	STDMETHOD(get_Version)(BSTR* pbstrVersion);

	STDMETHOD(WriteCommandHeader)(IMarshallBuffer* pBuffer, unsigned char ucReply, unsigned char ucCommand);

#ifdef SUPPORT_SERVER_CALLSTACK
	HRESULT ReadCommandCallstack(IUnMarshallBuffer* pUBuffer, CSetUID** ppSetUID);
	STDMETHOD(WriteCommandCallstack)(IMarshallBuffer* pBuffer /* , VARIANT_BOOL bSendCallstack */);
#else
	STDMETHOD(WriteCommandCallstack)(IMarshallBuffer* pBuffer /* , VARIANT_BOOL bSendCallstack */) { return S_OK; };
#endif

	HRESULT LogCreateObject(long lID);
	HRESULT LogDeleteObject(IThing* pthing);

	HRESULT LogModule(IModule* pmodule, BSTR bstrName);
	HRESULT LogCommand(long lID, IMarshallBuffer* pbuffer);

	STDMETHOD(LogWorldCommand)(DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams);
	STDMETHOD(LogThingCommand)(IThing* pthing, DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams);

	STDMETHOD(RegisterInterest)(IThing* pthing);
	STDMETHOD(UnregisterInterest)(IThing* pthing);

	STDMETHOD(ActivateRoom)(IThing* pthing);
	STDMETHOD(DeactivateRoom)(IThing* pthing);

	HRESULT SendModules(void);
	HRESULT SendExemplars(void);

	HRESULT SendToAvatars(VARIANT_BOOL bExcludeUser, IMarshallBuffer* pbuffer);
	HRESULT SendToAvatar(IThing* pAvatar, IMarshallBuffer* pbuffer);
	HRESULT SendToBystanders(IThing* pwho, VARIANT_BOOL bExcludeUser, IMarshallBuffer* pbuffer);
	HRESULT SendToConnection(IMarshallBuffer* pbuffer, VARIANT_BOOL bSync, VARIANT_BOOL bReply, HRESULT* precvhr, VARIANT* pvar);

	HRESULT AddThingToBystanders(IThing* pthing);
	HRESULT AddContainerToBystanders(IThing* pcontainer);
	HRESULT AddContentsToBystanders(IThing* pcontainer);

	STDMETHOD(IsExemplar)(IThing* pthing, VARIANT_BOOL* pboolVal);
	STDMETHOD(IsAvatar)(IThing* pthing, VARIANT_BOOL* pboolVal);
	STDMETHOD(IsRoom)(IThing* pthing, VARIANT_BOOL* pboolVal);

	STDMETHOD(InitializeConnection)(IVWCommConnection* pvwcommconnection);
	STDMETHOD(Terminate)();

	STDMETHOD(SendContainer)(long lval, IThing* pcontainer);
	STDMETHOD(SendContents)(long lval, IThing* pcontainer, VARIANT_BOOL bForceOpen);

	STDMETHOD(SendThingByID)(long lval, long lID);

	STDMETHOD(SendWorld)(long lval);
	STDMETHOD(SendThing)(long lval, IThing* pthing);
	STDMETHOD(SendModule)(long lval, IModule* pmodule, BSTR bstrName);
	STDMETHOD(SendExemplar)(long lval, IThing* pthing, BSTR bstrType);

	HRESULT SendCommandExt(long lval, IThing* pthing, IMarshallBuffer* pbuffer, VARIANT_BOOL bSync = VARIANT_FALSE, VARIANT_BOOL bReply = VARIANT_FALSE, HRESULT* recvhr = NULL, VARIANT* pvar = NULL);

	STDMETHOD(SendWorldCommand)(VARIANT_BOOL bReply, DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams, HRESULT* recvhr = NULL, VARIANT* pvar = NULL)
	{
		return SendWorldCommandExt(bReply, VARIANT_TRUE, dispid, wFlags, pdispparams, recvhr, pvar); 
	};

	STDMETHOD(SendThingCommand)(long lFlags, IThing* pthing, DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams)
	{
		return SendThingCommandExt(VARIANT_FALSE, VARIANT_FALSE, lFlags, pthing, dispid, wFlags, pdispparams); 
	};

	STDMETHOD(SendObjectPropertyCommand)(long lFlags, IObjectProperty* pprop, DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams)
	{
		return SendObjectPropertyCommandExt(VARIANT_FALSE, VARIANT_FALSE, lFlags, pprop, dispid, wFlags, pdispparams); 
	};

	STDMETHOD(SendWorldCommandExt)(VARIANT_BOOL bReply, VARIANT_BOOL bSync, DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams, HRESULT* recvhr = NULL, VARIANT* pvar = NULL);
	STDMETHOD(SendThingCommandExt)(VARIANT_BOOL bReply, VARIANT_BOOL bSync, long lFlags, IThing* pthing, DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams, HRESULT* recvhr = NULL, VARIANT* pvar = NULL);
	STDMETHOD(SendObjectPropertyCommandExt)(VARIANT_BOOL bReply, VARIANT_BOOL bSync, long lFlags, IObjectProperty* pprop, DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams, HRESULT* recvhr = NULL, VARIANT* pvar = NULL);

	STDMETHOD(SendMethodCommand)(IMethod* pmethod, DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams);
	STDMETHOD(SendModuleCommand)(IModule* pmodule, DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams);

	STDMETHOD(get_Global)(IThing** ppthing)
	{
		HRESULT hr = S_OK;

		ASSERT(ppthing);
		if (ppthing == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_Global: outgoing pointer (IThing**) is NULL\n");
			hr = E_POINTER;
			goto ERROR_ENCOUNTERED;
		}

		*ppthing = NULL;

		if (m_pGlobal == NULL)
		{
			hr = get_ObjectByID(WORLD_OBJECT, &m_pGlobal);
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CWorldObject::get_Global: get_ObjectByID failed\n");
				goto ERROR_ENCOUNTERED;
			}

			if (m_pGlobal == NULL)
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_Global: invalid global object\n");

				// REVIEW: correct HRESULT?
				hr = VWOBJECT_E_INVALIDWORLD;  
				goto ERROR_ENCOUNTERED;
			}
		}

		*ppthing = m_pGlobal;

		SAFEADDREF(*ppthing);

	ERROR_ENCOUNTERED:
		return ReportWorldError(hr);
	};

	STDMETHOD(get_Owner)(IThing** ppthing)
	{
		if (ppthing == NULL)
			return ReportWorldError(E_POINTER);

		HRESULT hr = S_OK;
		IThing* pGlobal = NULL;

		hr = get_Global(&pGlobal);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_Owner: get_Global failed\n");
			goto ERROR_ENCOUNTERED;
		}

		hr = pGlobal->get_Owner(ppthing);

ERROR_ENCOUNTERED:
		SAFERELEASE(pGlobal);

		return ReportWorldError(hr);
	};

	STDMETHOD(get_Modules)(IPropertyMap** pppropertymap)
	{
		IThing* pGlobal = NULL;
		HRESULT hr = S_OK;
		
		ASSERT(pppropertymap);
		if (pppropertymap == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_Modules: outgoing pointer (IPropertyMap**) is NULL\n");
			hr = E_POINTER;
			goto ERROR_ENCOUNTERED;
		}

		*pppropertymap = NULL;

		if (m_pModules == NULL)
		{
			hr = get_Global(&pGlobal);
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_Modules: get_Global failed\n");
				goto ERROR_ENCOUNTERED;
			}

			hr = pGlobal->get_ObjectProperty(m_bstrModules, (IObjectProperty**)&m_pModules);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			if (m_pModules == NULL)
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_Modules: invalid module map\n");
				hr = VWOBJECT_E_INVALIDMODULEMAP;
				goto ERROR_ENCOUNTERED;
			}
		}

		*pppropertymap = m_pModules;
		
		SAFEADDREF(*pppropertymap);

	ERROR_ENCOUNTERED:
		SAFERELEASE(pGlobal);

		return ReportWorldError(hr);
	};

	STDMETHOD(get_Exemplars)(IPropertyMap** pppropertymap)
	{
		IThing* pGlobal = NULL;
		HRESULT hr = S_OK;
		
		ASSERT(pppropertymap);
		if (pppropertymap == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_Exemplars: outgoing pointer (IPropertyMap**) is NULL\n");
			hr = E_POINTER;
			goto ERROR_ENCOUNTERED;
		}

		*pppropertymap = NULL;

		if (m_pExemplars == NULL)
		{
			hr = get_Global(&pGlobal);
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_Exemplars: get_Global failed\n");
				goto ERROR_ENCOUNTERED;
			}

			hr = pGlobal->get_ObjectProperty(m_bstrExemplars, (IObjectProperty**)&m_pExemplars);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			if (m_pExemplars == NULL)
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_Exemplars: invalid exemplar map\n");
				hr = VWOBJECT_E_INVALIDEXEMPLARMAP;
				goto ERROR_ENCOUNTERED;
			}
		}

		*pppropertymap = m_pExemplars;
		
		SAFEADDREF(*pppropertymap);

	ERROR_ENCOUNTERED:
		SAFERELEASE(pGlobal);

		return ReportWorldError(hr);
	};

	HRESULT get_Avatars(IPropertyMap** pppropertymap)
	{
		IThing* pGlobal = NULL;
		HRESULT hr = S_OK;
		
		ASSERT(pppropertymap);
		if (pppropertymap == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_Avatars: outgoing pointer (IPropertyMap**) is NULL\n");
			hr = E_POINTER;
			goto ERROR_ENCOUNTERED;
		}

		*pppropertymap = NULL;

		if (m_pAvatars == NULL)
		{
			hr = get_Global(&pGlobal);
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_Avatars: get_Global failed\n");
				goto ERROR_ENCOUNTERED;
			}

			hr = m_pGlobal->get_ObjectProperty(m_bstrAvatars, (IObjectProperty**)&m_pAvatars);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			if (m_pAvatars == NULL)
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_Avatars: invalid avatar map\n");
				hr = VWOBJECT_E_INVALIDCREATUREMAP;
				goto ERROR_ENCOUNTERED;
			}
		}

		*pppropertymap = m_pAvatars;
		
		SAFEADDREF(*pppropertymap);

	ERROR_ENCOUNTERED:
		SAFERELEASE(pGlobal);

		return hr;
	};

	HRESULT get_GUIDs(IPropertyMap** pppropertymap)
	{
		IThing* pGlobal = NULL;
		HRESULT hr = S_OK;
		
		ASSERT(pppropertymap);
		if (pppropertymap == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_GUIDs: outgoing pointer (IPropertyMap**) is NULL\n");
			hr = E_POINTER;
			goto ERROR_ENCOUNTERED;
		}

		*pppropertymap = NULL;

		if (m_pGUIDs == NULL)
		{
			hr = get_Global(&pGlobal);
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_GUIDs: get_Global failed\n");
				goto ERROR_ENCOUNTERED;
			}

			hr = m_pGlobal->get_ObjectProperty(m_bstrGUIDs, (IObjectProperty**)&m_pGUIDs);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			if (m_pGUIDs == NULL)
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_GUIDs: invalid avatar map\n");
				hr = VWOBJECT_E_INVALIDCREATUREMAP;
				goto ERROR_ENCOUNTERED;
			}
		}

		*pppropertymap = m_pGUIDs;
		
		SAFEADDREF(*pppropertymap);

	ERROR_ENCOUNTERED:
		SAFERELEASE(pGlobal);

		return hr;
	};

	STDMETHOD(get_Connection)(IVWCommConnection** ppvwcommconnection)
	{
		ASSERT(ppvwcommconnection);
		if (ppvwcommconnection == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_Connection: outgoing pointer (IVWCommConnection**) is NULL\n");
			return E_POINTER;
		}

		*ppvwcommconnection = m_pVWConnection;
		SAFEADDREF(*ppvwcommconnection);

		return S_OK;
	};

	STDMETHOD(put_Connection)(IVWCommConnection* pvwcommconnection)
	{
		SAFERELEASE(m_pVWConnection);

		m_pVWConnection = pvwcommconnection;

		SAFEADDREF(m_pVWConnection);

		return S_OK;
	};

	STDMETHOD(get_IsConnected)(VARIANT_BOOL* pboolVal)
	{
		ASSERT(pboolVal);
		if (pboolVal == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_IsConnected: outgoing pointer (VARIANT_BOOL*) is NULL\n");
			return E_POINTER;
		}

		*pboolVal = m_bConnected;

		return S_OK;
	};

	STDMETHOD(get_ShouldExecute)(VARIANT_BOOL* pboolVal)
	{
		ASSERT(pboolVal);
		if (pboolVal == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_ShouldExecute: outgoing pointer (VARIANT_BOOL*) is NULL\n");
			return E_POINTER;
		}

		// allow overriding of bShouldExecute by bLocalMode
		*pboolVal = (m_bShouldExecute == VARIANT_TRUE || 
				  m_bLocalMode == VARIANT_TRUE) ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	};

	STDMETHOD(get_ShouldExecuteSync)(VARIANT_BOOL* pboolVal)
	{
		ASSERT(pboolVal);
		if (pboolVal == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_ShouldExecuteSync: outgoing pointer (VARIANT_BOOL*) is NULL\n");
			return E_POINTER;
		}

		// allow overriding of bShouldExecuteSync by bLocalMode
		*pboolVal = (m_bShouldExecuteSync == VARIANT_TRUE || 
				  m_bLocalMode == VARIANT_TRUE) ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	};

	STDMETHOD(get_ShouldRemote)(VARIANT_BOOL* pboolVal)
	{
		ASSERT(pboolVal);
		if (pboolVal == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_ShouldRemote: outgoing pointer (VARIANT_BOOL*) is NULL\n");
			return E_POINTER;
		}

		// setting bLocalMode will override SendCommandExt, but will still call LogCommand
		*pboolVal = m_bShouldRemote;

		return S_OK;
	};

	STDMETHOD(get_ShouldRemoteSync)(VARIANT_BOOL* pboolVal)
	{
		ASSERT(pboolVal);
		if (pboolVal == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_ShouldRemoteSync: outgoing pointer (VARIANT_BOOL*) is NULL\n");
			return E_POINTER;
		}

		*pboolVal = m_bShouldRemoteSync;

		return S_OK;
	};

	STDMETHOD(get_ShouldNotify)(VARIANT_BOOL* pboolVal)
	{
		ASSERT(pboolVal);
		if (pboolVal == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_ShouldNotify: outgoing pointer (VARIANT_BOOL*) is NULL\n");
			return E_POINTER;
		}

		*pboolVal = m_bShouldNotify;

		return S_OK;
	};

	STDMETHOD(get_EnableSecurity)(VARIANT_BOOL* pboolVal)
	{
		static CComBSTR bstrEnableSecurity("EnableSecurity");
		IThing* pGlobal = NULL;
		HRESULT hr = S_OK;

#ifdef SUPPORT_CACHED_ENABLESECURITY
		static VARIANT_BOOL bEnableSecurity = VARIANT_FALSE;
		static VARIANT_BOOL bEnableSecurityCached = VARIANT_FALSE;

		if (bEnableSecurityCached == VARIANT_FALSE)
		{
			if (SUCCEEDED(get_Global(&pGlobal)))
			{
				hr = pGlobal->get_BOOL(bstrEnableSecurity, pboolVal);

				SAFERELEASE(pGlobal);

				if (SUCCEEDED(hr))
				{
					bEnableSecurityCached = VARIANT_TRUE;
					bEnableSecurity = *pboolVal;
				}
			}
		}
		else
			*pboolVal = bEnableSecurity;
#else
		if (SUCCEEDED(get_Global(&pGlobal)))
		{
			hr = pGlobal->get_BOOL(bstrEnableSecurity, pboolVal);

			SAFERELEASE(pGlobal);
		}
#endif

		return hr;
	};

	STDMETHOD(Report)(BSTR bstr, long lType);
	STDMETHOD(Trace)(BSTR bstr);
	STDMETHOD(Print)(VARIANT varAnything);

	STDMETHOD(EnableTrace)(VARIANT_BOOL boolVal)
	{
		m_bTrace = boolVal;

		return S_OK;
	};

	STDMETHOD(get_IsTraceEnabled)(VARIANT_BOOL* pboolVal)
	{
		ASSERT(pboolVal);
		if (pboolVal == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_IsTraceEnabled: outgoing pointer (VARIANT_BOOL*) is NULL\n");
			return E_POINTER;
		}

		*pboolVal = m_bTrace;

		return S_OK;
	};

	STDMETHOD(get_TraceLevel)(BSTR bstrName, int* pival)
	{
		ASSERT(bstrName);
		if (bstrName == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_TraceLevel: incoming argument (BSTR) is NULL\n");
			return E_INVALIDARG;
		}

		ASSERT(pival);
		if (pival == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_TraceLevel: outgoing pointer (int*) is NULL\n");
			return E_POINTER;
		}

		if (m_componentMap.Lookup(bstrName, *pival))
			return S_OK;
		else
			return VWOBJECT_E_UNKNOWNTRACECOMPONENT;
	};

	STDMETHOD(put_TraceLevel)(BSTR bstrName, int ival)
	{
		ASSERT(bstrName);
		if (bstrName == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::put_TraceLevel: incoming argument (BSTR) is NULL\n");
			return E_INVALIDARG;
		}

		if (ival < VWT_VERBOSE || ival > VWT_NONE)
			return VWOBJECT_E_INVALIDTRACELEVEL;

		m_componentMap.SetAt(bstrName, ival);

		return S_OK;
	};

	STDMETHOD(RemoveTraceLevel)(BSTR bstrName)
	{
		ASSERT(bstrName);
		if (bstrName == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::RemoveTraceLevel: incoming argument (BSTR) is NULL\n");
			return E_INVALIDARG;
		}

		m_componentMap.RemoveKey(bstrName);

		return S_OK;
	};

	// formatting
	STDMETHOD(Format)(BSTR bstrIn, IPropertyList *pObjects, IThing *pPOV,
					  BSTR *bstrOut);
	STDMETHOD(FormatSentences)(BSTR bstrIn, IPropertyList *pObjects, IThing *pPOV,
							   BSTR *bstrOut);

	STDMETHOD(NotifyUserConnect)(IThing* pthing);
	STDMETHOD(NotifyUserDisconnect)(IThing* pthing);

	STDMETHOD(FireUIEvent)(IThing* pthing, BSTR bstrEventName, VARIANT varArg);

	STDMETHOD(ShowHTML)(BSTR bstrTitle, BSTR bstrHTML, VARIANT cx, VARIANT cy);
	STDMETHOD(ShowHTMLEx)(BSTR bstrWinName, BSTR bstrTitle, BSTR bstrHTML,
						  VARIANT dispArg, VARIANT x,  VARIANT y, VARIANT cx, VARIANT cy);

	STDMETHOD(ShowURL)(BSTR bstrTitle, BSTR bstrURL, VARIANT cx, VARIANT cy);
	STDMETHOD(ShowURLEx)(BSTR bstrWinName, BSTR bstrTitle, BSTR bstrURL,
						 VARIANT dispArg, VARIANT x, VARIANT y, VARIANT cx, VARIANT cy);

	STDMETHOD(ShowHTMLExt)(BSTR bstrWinName, BSTR bstrHTML,
		BSTR bstrTitle, VARIANT_BOOL bSafeMode,
		IDispatch *pdispArg, 
		int x, int y, int cx, int cy);
	STDMETHOD(ShowURLExt)(BSTR bstrWinName, VARIANT_BOOL bPopup, BSTR bstrURL,
		BSTR bstrTitle, VARIANT_BOOL bSafeMode,
		IDispatch *pdispArg, 
		int x, int y, int cx, int cy);

	STDMETHOD(NotifyInvokeMethod)(IThing* pthing, BSTR bstrVal, DISPPARAMS* pdispparams);

	STDMETHOD(NotifyPropertyChanged)(IThing* pthing, BSTR bstrVal, long lHint, VARIANT varHintData);
	STDMETHOD(NotifyMethodChanged)(IThing* pthing, BSTR bstrVal, long lHint, VARIANT varHintData);
	STDMETHOD(NotifyModuleChanged)(BSTR bstrVal, long lHint, VARIANT varHintData);

	// helper fns for VWODB callbacks
	HRESULT CreateTools();
	HRESULT CreateWorldObject();

	// VWODB callbacks
	STDMETHOD(OnOpenWorld)(VARIANT_BOOL bPersist);
	STDMETHOD(OnCreateWorld)();

	// ProcessMessage callback
	HRESULT OnReceiveWorld();

//	HRESULT PutRootURL();

	HRESULT AddGlobalPaths();
	STDMETHOD(UpdateGlobalPaths)();

	STDMETHOD(get_Database)(BSTR* pbstrVal);

	STDMETHOD(OpenDatabase)(BSTR bstrVal, VARIANT_BOOL bCreate = VARIANT_FALSE, VARIANT_BOOL bOverwrite = VARIANT_FALSE);
	STDMETHOD(SaveDatabaseAs)(BSTR bstrFilename);
	STDMETHOD(SaveDatabase)();
	STDMETHOD(SaveToScript)(BSTR bstrFilename);

	STDMETHOD(Synchronize)();

	STDMETHOD(LockWorld)()
	{
		// synchronize deletion of world object
		EnterCriticalSection(&m_critsec);

		return S_OK;
	};

	STDMETHOD(UnlockWorld)()
	{
		// synchronize deletion of world object
		LeaveCriticalSection(&m_critsec);

		return S_OK;
	};

	STDMETHOD(get_ServerSide)(VARIANT_BOOL* pboolVal);
	STDMETHOD(put_ServerSide)(VARIANT_BOOL boolVal);

	STDMETHOD(get_FreezeEvents)(VARIANT_BOOL* pboolVal);
	STDMETHOD(put_FreezeEvents)(VARIANT_BOOL boolVal);

	STDMETHOD(get_LocalMode)(VARIANT_BOOL* pboolVal);
	STDMETHOD(put_LocalMode)(VARIANT_BOOL boolVal);

	STDMETHOD(get_IsLoggingEnabled)(VARIANT_BOOL* pboolVal);
	STDMETHOD(EnableLogging)(VARIANT_BOOL boolVal);

	// helper for CreateAvatarExt
	HRESULT DecorateAlias(BSTR bstrAlias, BSTR* pbstrNewAlias);

	// helper for SaveToScript
	HRESULT SaveToScriptInt(BSTR bstrFilename);

	// helpers for ConnectInt
	HRESULT FindOrCreateAvatar(BSTR bstrName, BSTR bstrPassword, IAvatarProfile *pProfile,
		IThing** ppthing);

	HRESULT LoginByName(BSTR bstrName, BSTR bstrPassword, IThing** ppThing, BSTR* pBstrGUID, VARIANT_BOOL* bFirstTime);
	HRESULT LoginByGUID(BSTR bstrName, BSTR bstrPassword, IAvatarProfile *pProfile, IThing** ppThing, 
		BSTR* pBstrGUID, VARIANT_BOOL* bFirstTime);
	HRESULT CheckAvatarConnectionAndPassword(BSTR bstrPassword, IThing* pThing); 

	HRESULT CreateAvatarExt(BSTR bstrName, BSTR bstrPassword, BSTR bstrGUID,
		IThing** ppthing, BSTR* pbstrNewName);
	HRESULT CreateAvatarExtInPlace(BSTR bstrName, BSTR bstrPassword, BSTR bstrGUID,
		IThing* pthing, BSTR* pbstrNewName);

	HRESULT ConnectAvatar(IThing* pthing);

	// create avatar on server, don't return avatar to client (does return new avatar name, though)
	STDMETHOD(CreateAvatar)(BSTR bstrName, BSTR bstrPassword, VARIANT varGUID, BSTR* pbstrNewName);

	STDMETHOD(Connect)(BSTR bstrName, BSTR bstrPassword, IThing** ppthing);
	STDMETHOD(ConnectInt)(BSTR bstrName, BSTR bstrPassword, VARIANT varProfile, IThing** ppthing);

	STDMETHOD(Disconnect)(void);
	STDMETHOD(DisconnectInt)(void);
	STDMETHOD(DisconnectAvatar)(IThing* pAvatar);
	STDMETHOD(DisconnectAvatarInt)(IThing* pAvatar);
	STDMETHOD(DisconnectClient)(IThing* pAvatar);

	STDMETHOD(DisconnectConnection)(IVWCommConnection* pvwcommconnection);

	STDMETHOD(Reconnect)(BSTR bstrURL, BSTR bstrName, BSTR bstrPassword);
	STDMETHOD(ReconnectExt)(BSTR bstrURL, BSTR bstrName, BSTR bstrPassword);

	STDMETHOD(get_User)(IThing** ppthing);
	STDMETHOD(put_User)(IThing* pthing);

	STDMETHOD(get_This)(IThing** ppthing);
	STDMETHOD(put_This)(IThing* pthing);

	STDMETHOD(get_Caller)(IThing** ppthing);
	STDMETHOD(put_Caller)(IThing* pthing);

	STDMETHOD(get_SecurityContext)(IPropertySecurity** ppSecurity);
	STDMETHOD(put_SecurityContext)(IPropertySecurity *pSecurity );

	STDMETHOD(get_Super)(IThing** ppthing);
	STDMETHOD(put_Super)(IThing* pthing);

	STDMETHOD(AbortEvent)(void)
	{
		return put_IsEventAborted(VARIANT_TRUE);
	};

	STDMETHOD(get_IsEventAborted)(VARIANT_BOOL* pboolVal)
	{
		ASSERT(pboolVal);
		if (pboolVal == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_IsEventAborted: outgoing pointer (VARIANT_BOOL*) is NULL\n");
			return E_POINTER;
		}

		*pboolVal = m_bAbortEvent;

		return S_OK;
	};

	STDMETHOD(put_IsEventAborted)(VARIANT_BOOL boolVal)
	{
		m_bAbortEvent = boolVal;

		return S_OK;
	};

	STDMETHOD(get_IsInPropChange)(VARIANT_BOOL* pboolVal)
	{
		ASSERT(pboolVal);
		if (pboolVal == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_IsInPropChange: outgoing pointer (VARIANT_BOOL*) is NULL\n");
			return E_POINTER;
		}

		*pboolVal = m_bInPropChange;

		return S_OK;
	};

	STDMETHOD(put_IsInPropChange)(VARIANT_BOOL boolVal)
	{
		m_bInPropChange = boolVal;

		return S_OK;
	};
	STDMETHOD(get_Avatar)(BSTR bstrName, IThing** ppthing);
	STDMETHOD(get_AvatarByGUID)(BSTR bstrName, IThing** ppthing);

	STDMETHOD(ProcessMessageExt)(unsigned char ch, IUnMarshallBuffer* pUBuffer, HRESULT* recvhr, VARIANT* pvar);
	STDMETHOD(ProcessMessage)(IVWCommMessage* pmessage);

	HRESULT ReplyWithVARIANT(IVWCommMessage *pmess, HRESULT hr, VARIANT* pvar);

	STDMETHOD(ProcessLogRecord)(IUnMarshallBuffer *pbuffer);

	STDMETHOD(get_ObjectByName)(BSTR bstrName, IThing** ppthing);
	STDMETHOD(get_ObjectByID)(long id, IThing** ppthing);

	STDMETHOD(get_Exemplar)(BSTR bstrType, IThing** ppthing);

	STDMETHOD(CreateObjectInt)(long lObjectID, IThing** ppthing);
	STDMETHOD(DeleteObjectInt)(IThing* pthing);

	STDMETHOD(FireActivateObject)(IThing* pthing);
	STDMETHOD(FireDeactivateObject)(IThing* pthing);

	STDMETHOD(FireCreateObject)(IThing* pthing);
	STDMETHOD(FireDestroyObject)(IThing* pthing);

	STDMETHOD(FireConnectAvatar)(IThing* pthing);
	STDMETHOD(FireDisconnectAvatar)(IThing* pthing);

	STDMETHOD(InvokeMoveInto)(IThing* pWhat, IThing* pWhere);
	STDMETHOD(InvokeAddDefaultKeys)(IThing* pWho, IAvatarProfile* pProfile);

	HRESULT IncrementObjectCount(long* plObjects)
	{
		HRESULT hr = S_OK;

		if (plObjects == NULL)
			return E_POINTER;

		*plObjects = 0L;

		{
			CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);

			hr = m_pGlobal->get_Long(CComBSTR("NumberOfObjects"), plObjects);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			hr = m_pGlobal->put_Long(CComBSTR("NumberOfObjects"), ++*plObjects);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}

ERROR_ENCOUNTERED:
		return hr;
	}

	HRESULT DecrementObjectCount(long* plObjects)
	{
		HRESULT hr = S_OK;

		if (plObjects == NULL)
			return E_POINTER;

		*plObjects = 0L;

		{
			CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);

			hr = m_pGlobal->get_Long(CComBSTR("NumberOfObjects"), plObjects);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			hr = m_pGlobal->put_Long(CComBSTR("NumberOfObjects"), --*plObjects);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}

ERROR_ENCOUNTERED:
		return hr;
	}

	HRESULT IncrementAvatarCount(long* plAvatars)
	{
		HRESULT hr = S_OK;

		if (plAvatars == NULL)
			return E_POINTER;

		*plAvatars = 0L;

		{
			CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);

			hr = m_pGlobal->get_Long(CComBSTR("NumberOfAvatars"), plAvatars);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			hr = m_pGlobal->put_Long(CComBSTR("NumberOfAvatars"), ++*plAvatars);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}

ERROR_ENCOUNTERED:
		return hr;
	}

	HRESULT DecrementAvatarCount(long* plAvatars)
	{
		HRESULT hr = S_OK;

		if (plAvatars == NULL)
			return E_POINTER;

		*plAvatars = 0L;

		{
			CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);

			hr = m_pGlobal->get_Long(CComBSTR("NumberOfAvatars"), plAvatars);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			hr = m_pGlobal->put_Long(CComBSTR("NumberOfAvatars"), --*plAvatars);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}

ERROR_ENCOUNTERED:
		return hr;
	}

	HRESULT IncrementUserCount(long* plUsers)
	{
		HRESULT hr = S_OK;

		if (plUsers == NULL)
			return E_POINTER;

		*plUsers = 0L;

		{
			CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);

			hr = m_pGlobal->get_Long(CComBSTR("NumberOfUsers"), plUsers);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			*plUsers = *plUsers + 1;

			hr = m_pGlobal->put_Long(CComBSTR("NumberOfUsers"), *plUsers);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}

ERROR_ENCOUNTERED:
		return hr;
	}

	HRESULT DecrementUserCount(long* plUsers)
	{
		HRESULT hr = S_OK;

		if (plUsers == NULL)
			return E_POINTER;

		*plUsers = 0L;

		{
			CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);
	
			hr = m_pGlobal->get_Long(CComBSTR("NumberOfUsers"), plUsers);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			*plUsers = *plUsers - 1;

			hr = m_pGlobal->put_Long(CComBSTR("NumberOfUsers"), *plUsers);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}

ERROR_ENCOUNTERED:
		return hr;
	}

	STDMETHOD(CreateInstance)(BSTR bstrName, VARIANT varExemplar, VARIANT varContainer, IThing** ppthing);
	STDMETHOD(CreateInstanceExt)(BSTR bstrName, VARIANT varExemplar, IThing* pthingWhere, IThing** ppthing);
	STDMETHOD(CreateInstanceInt)(BSTR bstrName, IThing* pthingParent, IThing* pthingWhere, IThing** ppthing);
	
	HRESULT CreateInstanceIntHelper(BSTR bstrName, IThing* pthingParent, IThing** ppthing);
	HRESULT CreateInstanceInPlace(BSTR bstrName, IThing* pthingParent, IThing* pThing);
	HRESULT CreateInstanceIncomplete(BSTR bstrName, BSTR bstrGUID, IThing** ppthing);

	STDMETHOD(CreateRootExemplar)(BSTR bstrType, IThing** ppthing)
	{
		return CreateExemplarExt(NULL, bstrType, CComVariant((IDispatch*)NULL), ppthing);
	};

	STDMETHOD(CreateExemplar)(BSTR bstrType, VARIANT varExemplar, IThing** ppthing);
	STDMETHOD(CreateExemplarEx)(BSTR bstrName, BSTR bstrType, VARIANT varExemplar, IThing** ppthing);
	STDMETHOD(CreateExemplarExt)(BSTR bstrName, BSTR bstrType, VARIANT varExemplar, IThing** ppthing);
	STDMETHOD(CreateExemplarInt)(BSTR bstrName, BSTR bstrType, IThing* pthingParent, IThing** ppthing);

	STDMETHOD(IsValidExemplar)(BSTR bstrType, VARIANT_BOOL* pboolVal);

	STDMETHOD(CloneInstance)(BSTR bstrName, IThing* pthingSource, IThing** ppthing);

	HRESULT ConvertInstanceToExemplar(BSTR bstrType, IThing* pthing);
	HRESULT ConvertExemplarToInstance(IThing* pthing);

	STDMETHOD(DestroyObject)(IThing* pthing);
	STDMETHOD(DestroyObjectByID)(long ID);

	HRESULT DestroyObjectInt(IThing* pthing);

	STDMETHOD(CreateObjectPropertyExt)(REFCLSID clsid, IUnknown* punk, IObjectProperty** ppproperty);
	STDMETHOD(CreateObjectProperty)(BSTR progid, IObjectProperty** ppproperty);

	STDMETHOD(CreateMethod)(long lFlags, VARIANT varModule, BSTR bstr, IMethod** ppmethod);
	STDMETHOD(CreateMethodByDispID)(long lFlags, VARIANT varModule, DISPID dispid, IMethod** ppmethod);
	STDMETHOD(CreateInlineMethod)(long lFlags,	BSTR bstrName, BSTR bstrLanguage, BSTR bstrCode, IMethod** ppmethod);

	STDMETHOD(CreateCOMModule)(BSTR bstrName, BSTR bstr, long lFlags, IModule** ppmodule);
	STDMETHOD(CreateScriptModule)(BSTR bstrName, BSTR bstr, long lFlags, IModule** ppmodule);
	STDMETHOD(RemoveModule)(BSTR bstrName);

	STDMETHOD(CreateCOMModuleEx)(BSTR bstrName, BSTR bstrProgID, long lFlags, IModule** ppmodule);
	STDMETHOD(CreateScriptModuleEx)(BSTR bstrName, BSTR bstrScriptPath, long lFlags, IModule** ppmodule);
	STDMETHOD(CreateLocalScriptModuleEx)(BSTR bstrName, IModule **ppmodule);

	STDMETHOD(get_Module)(BSTR bstrName, IModule** ppmodule);

	STDMETHOD(IsValidModule)(BSTR bstrName, VARIANT_BOOL* pboolVal);

	STDMETHOD(CreateToolExt)(BSTR bstrName, REFCLSID clsid, IUnknown** ppunk);
	STDMETHOD(CreateTool)(BSTR bstrName, BSTR bstr, IUnknown** ppunk);
	STDMETHOD(CreateToolFromInterface)(BSTR bstrName, IUnknown* punk);
	STDMETHOD(RemoveTool)(BSTR bstrName);

	STDMETHOD(IsValidTool)(BSTR bstrName, VARIANT_BOOL* pboolVal);

	STDMETHOD(get_Tool)(BSTR bstrName, IUnknown** ppunk);
	STDMETHOD(get_ToolExt)(BSTR bstrName, IUnknown** ppunk);

	STDMETHOD(get_Tools)(IPropertyMap** pppropertymap);
	STDMETHOD(InitializeTools)(IPropertyMap* ppropertymap);

	STDMETHOD(ValidateExemplarType)(BSTR bstrType, VARIANT_BOOL* pboolVal)
	{
		HRESULT hr = S_OK;

		if (pboolVal == NULL)
			return E_POINTER;

		*pboolVal = VARIANT_FALSE;

		if (SUCCEEDED(ValidateExemplarTypeExt(bstrType)))
			*pboolVal = VARIANT_TRUE;

		return S_OK;
	}

	STDMETHOD(ValidateInstanceName)(BSTR bstrName, VARIANT_BOOL* pboolVal)
	{
		HRESULT hr = S_OK;

		if (pboolVal == NULL)
			return E_POINTER;

		*pboolVal = VARIANT_FALSE;

		if (SUCCEEDED(ValidateInstanceNameExt(bstrName)))
			*pboolVal = VARIANT_TRUE;

		return S_OK;
	}

	STDMETHOD(ValidatePropertyName)(BSTR bstrPropertyName, VARIANT_BOOL* pboolVal)
	{
		HRESULT hr = S_OK;

		if (pboolVal == NULL)
			return E_POINTER;

		*pboolVal = VARIANT_FALSE;

		if (SUCCEEDED(ValidatePropertyNameExt(bstrPropertyName)))
			*pboolVal = VARIANT_TRUE;

		return S_OK;
	}

	STDMETHOD(ValidateMethodName)(BSTR bstrMethodName, long psbits, VARIANT_BOOL* pboolVal)
	{
		HRESULT hr = S_OK;

		if (pboolVal == NULL)
			return E_POINTER;

		*pboolVal = VARIANT_FALSE;

		hr = ValidateMethodNameExt(bstrMethodName, psbits);
		if (SUCCEEDED(hr))
			*pboolVal = VARIANT_TRUE;

		return ReportWorldError(hr);
	}

	STDMETHOD(ValidateWorldName)(BSTR bstrName, VARIANT_BOOL* pboolVal)
	{
		HRESULT hr = S_OK;

		if (pboolVal == NULL)
			return E_POINTER;

		*pboolVal = VARIANT_FALSE;

		if (SUCCEEDED(ValidateWorldNameExt(bstrName)))
			*pboolVal = VARIANT_TRUE;

		return S_OK;
	}

	STDMETHOD(ValidateAvatarName)(BSTR bstrName, VARIANT_BOOL* pboolVal)
	{
		HRESULT hr = S_OK;

		if (pboolVal == NULL)
			return E_POINTER;

		*pboolVal = VARIANT_FALSE;

		if (SUCCEEDED(ValidateAvatarNameExt(bstrName)))
			*pboolVal = VARIANT_TRUE;

		return S_OK;
	}

	// NOTE: URL rules for canonicalization:
	// Characters that must be encoded include any characters that have no corresponding 
	// graphic character in the US-ASCII coded character set (hexadecimal 80-FF, which are 
	// not used in the US-ASCII coded character set, and hexadecimal 00-1F and 7F, which are control characters), 
	// blank spaces, "%" (which is used to encode other characters), and unsafe characters (<, >, ", #, {, }, |, \, ^, ~, [, ], and '). 

	// we're picking off <, >, %, ?, ,, ;, :, |, ', \t, \n
	// not /, \, & (Flatland needed these)

	STDMETHOD(ValidateNameExt)(BSTR bstrName)
	{
		HRESULT hr = S_OK;

		if (bstrName == NULL || ::SysStringLen(bstrName) == 0)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateNameExt: name is NULL or empty\n");
#endif
			hr = VWOBJECT_E_INVALIDNAME;
		}
		else if (::SysStringLen(bstrName) > 255)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateNameExt: name too long (%s)\n", CString(bstrName));
#endif
			hr = VWOBJECT_E_INVALIDNAME;
		}
		else if (wcschr((const wchar_t*)bstrName, '<') != NULL)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateNameExt: name cannot contain an embedded less than sign (%s)\n", CString(bstrName));
#endif
			hr = VWOBJECT_E_INVALIDNAME;
		}
		else if (wcschr((const wchar_t*)bstrName, '>') != NULL)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateNameExt: name cannot contain an embedded greater than sign (%s)\n", CString(bstrName));
#endif
			hr = VWOBJECT_E_INVALIDNAME;
		}
		else if (wcschr((const wchar_t*)bstrName, '%') != NULL)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateNameExt: name cannot contain an embedded percent sign (%s)\n", CString(bstrName));
#endif
			hr = VWOBJECT_E_INVALIDNAME;
		}
		else if (wcschr((const wchar_t*)bstrName, '#') != NULL)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateNameExt: name cannot contain an embedded pound sign (%s)\n", CString(bstrName));
#endif
			hr = VWOBJECT_E_INVALIDNAME;
		}
		else if (wcschr((const wchar_t*)bstrName, '?') != NULL)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateNameExt: name cannot contain an embedded question mark (%s)\n", CString(bstrName));
#endif
			hr = VWOBJECT_E_INVALIDNAME;
		}
		else if (wcschr((const wchar_t*)bstrName, ',') != NULL)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateNameExt: name cannot contain an embedded comma (%s)\n", CString(bstrName));
#endif
			hr = VWOBJECT_E_INVALIDNAME;
		}
		else if (wcschr((const wchar_t*)bstrName, ';') != NULL)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateNameExt: name cannot contain an embedded semicolon (%s)\n", CString(bstrName));
#endif
			hr = VWOBJECT_E_INVALIDNAME;
		}
		else if (wcschr((const wchar_t*)bstrName, ':') != NULL)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateNameExt: name cannot contain an embedded colon (%s)\n", CString(bstrName));
#endif
			hr = VWOBJECT_E_INVALIDNAME;
		}
		// removed / \ & filtering
#if 0
		else if (wcschr((const wchar_t*)bstrName, '/') != NULL)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateNameExt: name cannot contain an embedded forward slash (%s)\n", CString(bstrName));
#endif
			hr = VWOBJECT_E_INVALIDNAME;
		}
		else if (wcschr((const wchar_t*)bstrName, '\\') != NULL)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateNameExt: name cannot contain an embedded backslash (%s)\n", CString(bstrName));
#endif
			hr = VWOBJECT_E_INVALIDNAME;
		}
		else if (wcschr((const wchar_t*)bstrName, '&') != NULL)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateNameExt: name cannot contain an embedded ampersand (%s)\n", CString(bstrName));
#endif
			hr = VWOBJECT_E_INVALIDNAME;
		}
#endif
		else if (wcschr((const wchar_t*)bstrName, '|') != NULL)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateNameExt: name cannot contain an embedded vertical bar (%s)\n", CString(bstrName));
#endif
			hr = VWOBJECT_E_INVALIDNAME;
		}
		else if (wcschr((const wchar_t*)bstrName, '\t') != NULL)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateNameExt: name cannot contain an embedded tab character (%s)\n", CString(bstrName));
#endif
			hr = VWOBJECT_E_INVALIDNAME;
		}
		else if (wcschr((const wchar_t*)bstrName, '\n') != NULL)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateNameExt: name cannot contain an embedded newline character (%s)\n", CString(bstrName));
#endif
			hr = VWOBJECT_E_INVALIDNAME;
		}
		else if (wcschr((const wchar_t*)bstrName, '\"') != NULL)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateNameExt: name cannot contain an embedded newline character (%s)\n", CString(bstrName));
#endif
			hr = VWOBJECT_E_INVALIDNAME;
		}

		return hr;
	}

	STDMETHOD(ValidateAvatarNameExt)(BSTR bstrName)
	{
		HRESULT hr = S_OK;

		if (bstrName == NULL || ::SysStringLen(bstrName) == 0)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateAvatarNameExt: name is NULL or empty\n");
#endif
			hr = VWOBJECT_E_INVALIDAVATARNAME;
		}
		else if (::SysStringLen(bstrName) < 2 || ::SysStringLen(bstrName) > 12)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateAvatarNameExt: invalid name length (%s)\n", CString(bstrName));
#endif
			hr = VWOBJECT_E_INVALIDAVATARNAME;
		}
		else if (wcschr((const wchar_t*)bstrName, ' ') != NULL)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateAvatarNameExt: name cannot contain an embedded space (%s)\n", CString(bstrName));
#endif
			hr = VWOBJECT_E_INVALIDAVATARNAME;
		}
		else
		{
			hr = ValidateNameExt(bstrName);

			if (hr == VWOBJECT_E_INVALIDNAME)
				hr = VWOBJECT_E_INVALIDAVATARNAME;
		}

		return hr;
	}

	STDMETHOD(ValidateWorldNameExt)(BSTR bstrName)
	{
		HRESULT hr = S_OK;
		CComBSTR bstrWorldPath("c:\\Program Files\\Microsoft Virtual Worlds\\Worlds\\");
		int Count = 0;

		if (bstrName == NULL || ::SysStringLen(bstrName) == 0)
			{
	#ifdef _DEBUG
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateNameExt: name is NULL or empty\n");
	#endif
				hr = VWOBJECT_E_INVALIDWORLDNAME;
				goto ERROR_ENCOUNTERED;
			}

		hr = FindWorldPath(&bstrWorldPath.m_str);
		if (FAILED(hr))
			return hr;

		Count = bstrWorldPath.Length() + ::SysStringLen(bstrName) + WORLDFILEEXTENSIONSIZE;

		if (Count > 256)
		{
			hr = VWOBJECT_E_INVALIDWORLDNAME;
			goto ERROR_ENCOUNTERED;
		}

		hr = ValidateNameExt(bstrName);

		if (hr == VWOBJECT_E_INVALIDNAME)
			hr = VWOBJECT_E_INVALIDWORLDNAME;

ERROR_ENCOUNTERED:
		return hr;
	}

	STDMETHOD(ValidateExemplarTypeExt)(BSTR bstrType)
	{
		// don't allow new exemplars with 'thing' type
		if (CompareElements(&CThingObject::m_bstrThing.m_str, &bstrType))
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateExemplarType: invalid exemplar type (%s)\n", CString(bstrType));
#endif
			return VWOBJECT_E_INVALIDEXEMPLARTYPE;
		}
		else
		{
			HRESULT hr = S_OK;
			
			hr = ValidateInstanceNameExt(bstrType);

			if (hr == VWOBJECT_E_INVALIDINSTANCENAME)
				hr = VWOBJECT_E_INVALIDEXEMPLARTYPE;

			return hr;
		}
	}

	STDMETHOD(ValidatePropertyNameExt)(BSTR bstrName)
	{
		HRESULT hr = S_OK;

		if (!iswalnum((wint_t)*(const wchar_t*)bstrName))
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidatePropertyNameExt: property name must begin with an alphanumeric character (%s)\n", CString(bstrName));
#endif
			hr = VWOBJECT_E_INVALIDPROPERTYNAME;
		}
		else if (wcschr((const wchar_t*)bstrName, ' ') != NULL)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidatePropertyNameExt: name cannot contain an embedded space (%s)\n", CString(bstrName));
#endif
			hr = VWOBJECT_E_INVALIDPROPERTYNAME;
		}
		else if (wcschr((const wchar_t*)bstrName, '.') != NULL)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidatePropertyNameExt: name cannot contain an embedded period (%s)\n", CString(bstrName));
#endif
			hr = VWOBJECT_E_INVALIDPROPERTYNAME;
		}
		else
		{
			HRESULT hr = S_OK;
			
			hr = ValidateNameExt(bstrName);

			if (hr == VWOBJECT_E_INVALIDNAME)
				hr = VWOBJECT_E_INVALIDPROPERTYNAME;

			return hr;
		}

		return hr;
	}

	STDMETHOD(ValidateMethodNameExt)(BSTR bstrName, long lFlags)
	{
		HRESULT hr = S_OK;
		CString strMethodName(bstrName);
#ifdef _DEBUG
		CComBSTR bstrPropertyName;
#endif
		// Does method name fit On<Foo>Access pattern?
		if (strMethodName.Left(2).CompareNoCase(CThingObject::m_strOn) == 0)
		{
			
			if (strMethodName.Right(6).CompareNoCase(CThingObject::m_strAccess) ==0) 
			{
#ifdef _DEBUG
				CString strPropertyName(bstrName);

				// strip out "On"
				strPropertyName.Replace(CThingObject::m_strOn, "");

				// strip out "Access"
				strPropertyName.Replace(CThingObject::m_strAccess, "");
#endif

				if ((lFlags & (METHOD_CLIENT | METHOD_SERVER)) != (METHOD_CLIENT | METHOD_SERVER))
				{
#ifdef _DEBUG
					VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateMethodNameExt: property (%s), %s method not marked METHOD_CLIENT+METHOD_SERVER\n", 
						strPropertyName, strMethodName);
#endif
					hr = VWOBJECT_E_INVALIDONACCESSMETHOD;
				}
			}
		}
		else
		{
			hr = ValidatePropertyNameExt(bstrName);

			if (hr == VWOBJECT_E_INVALIDPROPERTYNAME)
				hr = VWOBJECT_E_INVALIDMETHODNAME;
		}

		return hr;
	}

	STDMETHOD(ValidateInstanceNameExt)(BSTR bstrName)
	{
		HRESULT hr = S_OK;
		
		hr = ValidateNameExt(bstrName);

		if (hr == VWOBJECT_E_INVALIDNAME)
			hr = VWOBJECT_E_INVALIDINSTANCENAME;

		return hr;
	}

	STDMETHOD(ValidateDISPPARAMS)(DISPPARAMS* pdispparams)
	{
		HRESULT hr = S_OK;

		if (pdispparams == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateDISPPARAMS: invalid incoming argument (DISPPARAMS*)\n");
			return E_INVALIDARG;
		}

		for (unsigned int i = 0; i < pdispparams->cArgs && SUCCEEDED(hr); i++)
			hr = ValidateProperty(pdispparams->rgvarg[i]);

		return hr;
	}

	STDMETHOD(ValidateProperty)(VARIANT var)
	{
		HRESULT hr = S_OK;

		// NOTE: these are all the supported marshallable types

		switch (var.vt)
		{
		case VT_EMPTY:
		case VT_ERROR:
		case VT_NULL:
		case VT_I2:
		case VT_I4:
		case VT_INT:
		case VT_R4:
		case VT_R8:
		case VT_BOOL:
		case VT_UI1:
		case VT_UI2:
		case VT_UI4:
		case VT_UINT:
		case VT_UNKNOWN:
		case VT_DISPATCH:
			break;
		case VT_BSTR:
			{
				long lSize = ::SysStringByteLen(var.bstrVal);

				// NOTE: using 100 bytes as wiggle room for extra stuff in packet
				if (lSize > COPYBUFFER_SIZE - 100)
				{
					VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateProperty: BSTR too large (%d)\n", lSize);
					hr = VWOBJECT_E_INVALIDBSTRSIZE;
				}
			}
			break;
		default:
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::ValidateProperty: unknown property type (%d)\n", var.vt);
			hr = VWOBJECT_E_UNKNOWNPROPERTYTYPE;
			break;
		}

		return hr;
	}

	CString ModeToCString(long lval)
	{
		switch (lval)
		{
		case soClient:
			return "soClient";
		case soServer:
			return "soServer";
		case soAvatars:
			return "soAvatars";
		case soBystanders:
			return "soBystanders";
		case soBystandersExclUser:
			return "soBystandersExclUser";
		case soAvatarsExclUser:
			return "soAvatarsExclUser";
		}

		return "n/a";
	}

	long GetMarshallingMode(IThing* pthing, long lID, VARIANT_BOOL bIncludeUser)
	{
		long lval;

		if (m_bServerSide == VARIANT_FALSE)
		{
			lval = soServer;

			VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::GetMarshallingMode: client-side, soServer\n");
		}
		else
		{
			if (lID == WORLD_OBJECT)
			{
				// if object is global object, we need to send command to all avatars
				if (m_bInPropChange == VARIANT_TRUE)
				{
					lval = bIncludeUser ? soAvatars : soAvatarsExclUser;

#ifdef _DEBUG
					VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::GetMarshallingMode: server-side, global object, prop change yes, %s\n", ModeToCString(lval));
#endif
				}
				else
				{
					lval = soAvatars;

					VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::GetMarshallingMode: server-side, global object, prop change no, soAvatars\n");
				}
			}
			else
			{
				VARIANT_BOOL bExemplar = VARIANT_FALSE;
				HRESULT hr = S_OK;

				hr = pthing->get_IsExemplar(&bExemplar);

				// if object is an exemplar, we need to send command to all avatars
				if (SUCCEEDED(hr) && bExemplar == VARIANT_TRUE)
				{
					if (m_bInPropChange == VARIANT_TRUE)
					{
						lval = bIncludeUser ? soAvatars : soAvatarsExclUser;
		
#ifdef _DEBUG
						VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::GetMarshallingMode: server-side, exemplar, prop change yes, %s\n", ModeToCString(lval));
#endif
					}
					else
					{
						lval = soAvatars;

						VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::GetMarshallingMode: server-side, exemplar, prop change no, soAvatars\n");
					}
				}
				else 
				{
					if (m_bInPropChange == VARIANT_TRUE)
					{
						lval = bIncludeUser ? soBystanders : soBystandersExclUser;

#ifdef _DEBUG
						VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::GetMarshallingMode: server-side, prop change yes, %s\n", ModeToCString(lval));
#endif
					}
					else
					{
						lval = soBystanders;

						VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::GetMarshallingMode: server-side, prop change no, soBystanders\n");
					}
				}
			}
		}

		return lval;
	}

// IWorldMarshall
	STDMETHOD(MarshallThing)(IThing* pthing, DWORD dwOptions, IMarshallBuffer* pbuffer);
	STDMETHOD(UnMarshallThing)(IUnMarshallBuffer* pbuffer, IThing** ppthing);

	STDMETHOD(MarshallProperty)(VARIANT var, DWORD dwOptions, IMarshallBuffer* pbuffer);
	STDMETHOD(UnMarshallProperty)(IUnMarshallBuffer* pbuffer, VARIANT* pvar);

	STDMETHOD(MarshallObjectProperty)(IObjectProperty* pprop, long lHint, DWORD dwOptions, IMarshallBuffer* pbuffer);
	STDMETHOD(UnMarshallObjectProperty)(IUnMarshallBuffer* pbuffer, IObjectProperty** ppprop);

	STDMETHOD(MarshallModule)(IModule* pmodule, DWORD dwOptions, IMarshallBuffer* pbuffer);
	STDMETHOD(UnMarshallModule)(REFCLSID clsid, IUnMarshallBuffer* pbuffer, IModule** ppmodule);

	STDMETHOD(MarshallDISPPARAMS)(DISPPARAMS* pdispparams, IMarshallBuffer* pbuffer, DWORD dwOptions);
	STDMETHOD(UnMarshallDISPPARAMS)(IUnMarshallBuffer* pbuffer, DISPPARAMS* pdispparams);

	STDMETHOD(MarshallPropertyToScript)(VARIANT var, DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut);

	STDMETHOD(CheckPolicy)(IThing *pThing, unsigned long lFlags, VARIANT var, VARIANT_BOOL *pbAccessOk);
	STDMETHOD(CheckPolicyExt)(IThing *pThing, unsigned long lFlags, VARIANT var);
	
	STDMETHOD(CloneProperty)(VARIANT source, VARIANT* pDest)
	{
		HRESULT hr = S_OK;

		// need to deref any VT_BYREF props (writing into destination VARIANT)
		hr = ::VariantCopyInd(pDest, &source);
		if (FAILED(hr))
			return hr;

		// make sure incoming property is of marshallable type
		hr = ValidateProperty(*pDest);
		if (FAILED(hr))
		{
			// need to clear outgoing data on error
			::VariantClear(pDest);
			return hr;
		}
		
		if (pDest->vt == VT_DISPATCH)
		{
			// is this an object property?
			CObjectPropertyPtr PropPtr = *pDest;
			CThingPtr ThingPtr = *pDest;

			// if it's an objectproperty, but not a thing
			if (PropPtr != NULL && ThingPtr == NULL)
			{
				CComMarshallBuffer *pMarshall = NULL;
				CComUnMarshallBuffer *pUnMarshall = NULL;
				DWORD lLen = 0;
				unsigned char* pbyte = NULL;

				// create marshall buffer
				pMarshall = new CComMarshallBuffer();
				if (pMarshall == NULL)
				{
					VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CloneProperty: failed to allocate CComMarshallBuffer\n");
					hr = E_OUTOFMEMORY;
					goto ERROR_ENCOUNTERED;
				}

				pMarshall->AddRef();

				// create unmarshall buffer
				pUnMarshall = new CComUnMarshallBuffer();
				if (pUnMarshall == NULL)
				{
					VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::CloneProperty: failed to allocate CComUnMarshallBuffer\n");
					hr = E_OUTOFMEMORY;
					goto ERROR_ENCOUNTERED;
				}

				pUnMarshall->AddRef();

				// Marshall the source, by reference, 
				// This should clone IObjectPropertys, but not referenced things...
				// remember IThings will not be cloned, but referenced
				if (FAILED(MarshallProperty(*pDest, MARSHALL_BYID | MARSHALL_BYREF | MARSHALL_TOMEMORY, pMarshall)))
					goto ERROR_ENCOUNTERED;
			
				// convert pbuffer to pUbuffer
				hr = pMarshall->Detach(&pbyte, &lLen);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;
			
				hr = pUnMarshall->Attach(pbyte, lLen);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;
			
				::VariantClear(pDest);

				// Unmarshall back into the destination (in place, actually)
				if (FAILED(UnMarshallProperty(pUnMarshall, pDest)))
					goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
				// still need to free detached buffer
		//		if (pbyte)
		//			free(pbyte);

				// NOTE: need to free unmarshall buffer before marshall buffer because it 
				// has a ptr to the detached marshall buffer and will try and test it 
				// for a valid address
				SAFERELEASE(pUnMarshall);	
				SAFERELEASE(pMarshall);

				// need to clear outgoing data on error
				if (FAILED(hr))
					::VariantClear(pDest);
			}
		}

		return hr;
	}

	STDMETHOD(DumpProperty)(VARIANT var);

	// lookup tables for CLSIDs
	STDMETHOD(AddCLSIDToTable)(REFCLSID clsid);

	// status
	STDMETHOD(GetStatus)(BSTR *pbstr);

	STDMETHOD(ValidateObjectSafety)(REFCLSID clsid, IUnknown* punk, DWORD dwOptions);

protected:
	// Helper functions
	HRESULT SaveRoamingProfile(IThing *pUser);
	HRESULT LoadRegistryIntoProfile(BSTR bstrName, BSTR* pbstrProfile);
	HRESULT PutProfileIntoAvatar(IThing *pAvatar, IAvatarProfile *pProfile);

	HRESULT MoveProfileToDS(IThing *pAvatar,IAvatarProfile *pProfile);
	HRESULT DeleteThingFromDS(IThing *pAvatar);

	HRESULT get_ModuleNameList(IPropertyList** pppropertylist)
	{
		IThing* pGlobal = NULL;
		HRESULT hr = S_OK;
		
		ASSERT(pppropertylist);
		if (pppropertylist == NULL)
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_ModuleNameList: outgoing pointer (IPropertyList**) is NULL\n");
			hr = E_POINTER;
			goto ERROR_ENCOUNTERED;
		}

		*pppropertylist = NULL;

		if (m_pModuleNameList == NULL)
		{
			hr = get_Global(&pGlobal);
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_ModuleNameList: get_Global failed\n");
				goto ERROR_ENCOUNTERED;
			}

			hr = pGlobal->get_ObjectProperty(CComBSTR("ModuleNameList"), (IObjectProperty**)&m_pModuleNameList);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			if (m_pModuleNameList == NULL)
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::get_ModuleNameList: invalid module list\n");
				hr = VWOBJECT_E_INVALIDMODULEMAP;
				goto ERROR_ENCOUNTERED;
			}
		}

		*pppropertylist = m_pModuleNameList;
		
		SAFEADDREF(*pppropertylist);

	ERROR_ENCOUNTERED:
		SAFERELEASE(pGlobal);

		return hr;
	};


	// Data./////////////////////////////////////////////////////////////////////

	// back-ptr to connection
	IVWCommConnection* m_pVWConnection;

	// cached user ptr (on client)
	IThing* m_pUser;

	// repository info
	IVWObjectRepository* m_pDb;
	CComBSTR m_bstrDatabase;

	// freeze events flag
	VARIANT_BOOL m_bFreezeEvents;

	// logging flag
	VARIANT_BOOL m_bLogging;

	// tracing flag
	VARIANT_BOOL m_bTrace;

	// connected flag
	VARIANT_BOOL m_bConnected;

	// terminated flag
	VARIANT_BOOL m_bTerminated;

	// user connected flag
	VARIANT_BOOL m_bUserConnected;

	// am I on the server?
	VARIANT_BOOL m_bServerSide;

	// local-only mode?
	VARIANT_BOOL m_bLocalMode;

	VARIANT_BOOL m_bShouldExecute;
	VARIANT_BOOL m_bShouldExecuteSync;
	VARIANT_BOOL m_bShouldRemote;
	VARIANT_BOOL m_bShouldRemoteSync;
	VARIANT_BOOL m_bShouldNotify;

	VARIANT_BOOL m_bInPropChange;

	VARIANT_BOOL m_bAbortEvent;

	IPropertyMap* m_pTools;
	IPropertyMap* m_pExemplars;
	IPropertyMap* m_pModules;
	IPropertyMap* m_pAvatars;

	// This list is redundant (given m_pModules), but we need it in
	// order to be able to determine the _order_ that modules were
	// created (maps are not canonically ordered);
	IPropertyList* m_pModuleNameList;

	// Stores all objects in the system that have meaningful GUIDs,
	// not just avatars. 
	IPropertyMap* m_pGUIDs;

	IThing* m_pGlobal;

	// global marshall buffer
	IMarshallBuffer* m_pMarshBuf;

	// component map
	CVWMap<CBSTRAdapt<CComBSTR>, BSTR, int, int&> m_componentMap;

	// connection list (to IThing*)
	CConnList m_clist;

	// Method invocation/ call stack properties
	IThing* m_pThingThis;
	IThing* m_pThingCaller;
	IPropertySecurity *m_pSecurityContext;
	IThing* m_pThingSuper;

	// Special module for VBScript in-line methods
	IModule* m_pmodVBScript;

	CArray<IThing*, IThing*&> m_bystanderList;

	CRITICAL_SECTION m_critsec;

	// various status related things
	VARIANT_BOOL m_bInProcessMessage;
	unsigned char m_ucCurrentMessage;

	// helpers for IDispatch overriding
	BOOL IsDoctoredId(DISPID id, VARIANT_BOOL *pbProp);
	HRESULT IdFromName(LPOLESTR psz, DISPID* pid);
	HRESULT NameFromId(DISPID id, BSTR *pbstr);
	DISPID m_idNext;

	CVWMap<CBSTRAdapt<CComBSTR>, BSTR, DISPID, DISPID&> m_nameMap;
	CVWMap<DISPID, DISPID&, CBSTRAdapt<CComBSTR>, BSTR> m_DISPIDMap;

public:
	static CVWMap<CString, LPCTSTR, unsigned char, unsigned char&> m_indexTable;
	static CVWMap<unsigned char, unsigned char&, CLSID, CLSID&> m_clsidTable;
	static unsigned char m_ucIndex;

public:
	static CComBSTR m_bstrAddDefaultKeys;
	static CComBSTR m_bstrMoveTo;
	static CComBSTR m_bstrConnection;
	static CComBSTR m_bstrIsConnected;
	static CComBSTR m_bstrPassword;
	static CComBSTR m_bstrConnect;
	static CComBSTR m_bstrDisconnect;
	static CComBSTR m_bstrSystemConnect;
	static CComBSTR m_bstrSystemDisconnect;
	static CComBSTR m_bstrDescriptionDefault;
	static CComBSTR m_bstrCurrentID;
	static CComBSTR m_bstrCreate;
	static CComBSTR m_bstrDestroy;
	static CComBSTR m_bstrSystemCreate;
	static CComBSTR m_bstrSystemDestroy;
	static CComBSTR m_bstrAvatars;
	static CComBSTR m_bstrGUIDs;
	static CComBSTR m_bstrExemplars;
	static CComBSTR m_bstrModules;
	static CComBSTR m_bstrRoom;
	static CComBSTR m_bstrAvatar;	
	static CComBSTR m_bstrMenu;
	static CComBSTR m_bstrActivate;
	static CComBSTR m_bstrDeactivate;
	static CComBSTR m_bstrSystemActivate;
	static CComBSTR m_bstrSystemDeactivate;
	static CComBSTR m_bstrGUID;
	static CComBSTR m_bstrAuthentication;
};
