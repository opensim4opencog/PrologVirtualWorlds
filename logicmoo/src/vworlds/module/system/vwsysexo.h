// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef __VWSYSEXO_H_
#define __VWSYSEXO_H_

#include "resource.h"       // main symbols
#include <propbase.h>
#include "objimpl.h"

#include "syshelp.h"

EXTERN_C const CLSID CLSID_SystemExemplar;

#define ReportCoreError(hr) SUCCEEDED(hr) ? hr : ReportError("VWSYSTEM.SystemEx.1", hr, 0xc60, 0xc70, "COREEXEMP")

DECLARE_VWDISPATCH(SystemExemplar, LIBID_VWSYSTEMLib)
{
public:
//	CSystemExemplarObject();

DECLARE_REGISTRY_RESOURCEID(IDR_VWSYSEX)

BEGIN_COM_MAP(CSystemExemplarObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWDispatch)
	COM_INTERFACE_ENTRY(ISystemExemplar)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

public:
	STDMETHOD(Install)(IModule* pModule);

	STDMETHOD(CreateMenu)(IPropertyList** pppropertylist);
	STDMETHOD(CreateMenuItem)(BSTR bstrName, long lFlags, IThing *pOwner, BSTR bstrUpdateMethod, BSTR bstrMethod, VARIANT var, IMenuItem **ppMenuItem);
	STDMETHOD(CreatePropertyList)(VARIANT varInitialValue, VARIANT varOrphan, IPropertyList** pppropertylist);
	STDMETHOD(CreatePropertyMap)(VARIANT varOrphan, IPropertyMap** pppropertymap);

	STDMETHOD(CreateMenuSeparator)(IThing *pOwner, IMenuItem **ppMenuItem)
	{
		return ::CreateMenuItemExt(m_pWorld, NULL, MI_SEPARATOR, pOwner, NULL, NULL, NULL, ppMenuItem);
	};
	STDMETHOD(CreateOCXProxy)(IDispatch *pOCX, IOCXProxy **ppOCXProxy);
	STDMETHOD(QueryAvatarNames)(long lFlags, IPropertyList **pNameList);
	STDMETHOD(InvokeRemoteClientMethod)(IThing *pTarget, BSTR bstrMethodName, VARIANT varUserOrList, VARIANT varParams);
	STDMETHOD(HandleRemoteClientMethod)(IThing *pTarget, BSTR bstrMethodName, VARIANT varUserOrList, VARIANT varParams);
	STDMETHOD(FireUIEvent)(IPropertyList *pClientList, IThing *pSource, BSTR bstrMethodName, VARIANT varParams);
	STDMETHOD(FireRemoteUIEvent)(VARIANT varClientList, IThing *pSource, BSTR bstrEventName, VARIANT varParams);

	STDMETHOD(OnRootURLChanged)(long lHint, VARIANT varHintData);
	STDMETHOD(UploadFile)(BSTR bstrLocalName, BSTR bstrRemoteName, BSTR *pbstrURL);

	STDMETHOD(MakeHost)(IThing* pTarget);
	STDMETHOD(UnMakeHost)(IThing* pTarget);

	STDMETHOD(CreateTimer)(int msecPeriod, IThing *pThing, BSTR bstrMethod, VARIANT varArgs, IDispatch **ppTimer);
	STDMETHOD(DestroyTimer)(IDispatch *pTimer);

	STDMETHOD(OnCreateRoomClient)(IThing* pThing);
	STDMETHOD(ShowWelcomeMessage)();

	// Security access methods
	STDMETHOD(OnWorldOwnerAccess)(long lPerms, VARIANT_BOOL *bReturnVal);
	STDMETHOD(OnGlobalConstantAccess)(long lPerms, VARIANT_BOOL *bReturnVal);

	// Utility methods
	STDMETHOD(TraceExemplarInfo)();

	// perf testing methods
	STDMETHOD(TimeGetProperty)(IThing* pthing, BSTR bstrPropertyName, long lRepeat, long* plElapsed);
	STDMETHOD(TimePutProperty)(IThing* pthing, BSTR bstrPropertyName, VARIANT varData, long lRepeat, long* plElapsed);
	STDMETHOD(TimeInvokeMethod)(IThing* pthing, BSTR bstrMethodName, VARIANT varArgs, long lRepeat, long* plElapsed);
	STDMETHOD(TimeGetMethod)(IThing* pthing, BSTR bstrMethodName, long lRepeat, long* plElapsed);
	STDMETHOD(TimeGetPropertySecurity)(IThing* pthing, BSTR bstrPropertyName, long lRepeat, long* plElapsed);
	STDMETHOD(TimeCheckPropertySecurity)(IThing* pthing, BSTR bstrPropertyName, unsigned long ulFlags, long lRepeat, long* plElapsed);
	STDMETHOD(TimeCheckMethodSecurity)(IThing* pthing, BSTR bstrMethodName, unsigned long ulFlags, long lRepeat, long* plElapsed);
	STDMETHOD(TimeCheckPropertiesSecurity)(IThing* pthing, unsigned long ulFlags, long lRepeat, long* plElapsed);
	STDMETHOD(TimeCheckMethodsSecurity)(IThing* pthing, unsigned long ulFlags, long lRepeat, long* plElapsed);
	STDMETHOD(TimeVTableInvokeMethod)(long lRepeat, long* plElapsed);
	STDMETHOD(TimeAutomationInvokeMethod)(long lRepeat, long* plElapsed);
	STDMETHOD(TimeScriptInvokeMethod)(long lRepeat, long* plElapsed);
	STDMETHOD(TimeInterfaceInvokeMethod)(IThing* pthing, BSTR bstrMethodName, long lRepeat, long* plElapsed);
	STDMETHOD(TimeNotifyInvokeMethod)(IThing* pthing, BSTR bstrMethodName, DISPPARAMS* pdispparams, long lRepeat, long* plElapsed);
	STDMETHOD(TimeNotifyPropertyChanged)(IThing* pthing, BSTR bstrPropertyName, long lHint, VARIANT varHintData, long lRepeat, long* plElapsed);
	STDMETHOD(TimeSendThingCommand)(long lFlags, IThing* pthing, DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams, long lRepeat, long* plElapsed);
	STDMETHOD(TimeSendObjectPropertyCommand)(long lFlags, IObjectProperty* pprop, DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams, long lRepeat, long* plElapsed);
	STDMETHOD(TimeSendWorldCommand)(DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams, long lRepeat, long* plElapsed);
	STDMETHOD(TimeProcessMessage)(unsigned char ch, IUnMarshallBuffer* pUBuffer, long lRepeat, long* plElapsed);
	STDMETHOD(TimeCloneProperty)(VARIANT varData, long lRepeat, long* plElapsed);
	STDMETHOD(TimeCopyProperty)(VARIANT varData, long lRepeat, long* plElapsed);
	STDMETHOD(TimePutPropertySecurity)(IPropertySecurity* pSecurity, VARIANT varData, long lRepeat, long* plElapsed);

	HRESULT SendCommandHelper(IUnMarshallBuffer** ppUBuffer, unsigned char ch, IThing* pthing, DISPID dispid, WORD wFlags, DISPPARAMS* pdispparams);

	STDMETHOD(TimeGetPropertyAll)(VARIANT varData, long lRepeat, long* plElapsed);
	STDMETHOD(TimePutPropertyAll)(VARIANT varData, long lRepeat, long* plElapsed);
	STDMETHOD(TimeInvokeMethodAll)(long lRepeat, long* plElapsed);
	STDMETHOD(TimeGetMethodAll)(long lRepeat, long* plElapsed);
	STDMETHOD(TimeGetPropertySecurityAll)(long lRepeat, long* plElapsed);
	STDMETHOD(TimeCheckPropertySecurityAll)(unsigned long ulFlags, long lRepeat, long* plElapsed);
	STDMETHOD(TimeCheckMethodSecurityAll)(unsigned long ulFlags, long lRepeat, long* plElapsed);
	STDMETHOD(TimeCheckPropertiesSecurityAll)(unsigned long ulFlags, long lRepeat, long* plElapsed);
	STDMETHOD(TimeCheckMethodsSecurityAll)(unsigned long ulFlags, long lRepeat, long* plElapsed);
	STDMETHOD(TimeSendCommandAll)(long lRepeat, long* plElapsed);
	STDMETHOD(TimeNotifyAll)(long lRepeat, long* plElapsed);
	STDMETHOD(TimeProcessMessageAll)(long lRepeat, long* plElapsed);

	STDMETHOD(TimeAll)(long lRepeat, long* plElapsed);

	STDMETHOD(YYY)();
	STDMETHOD(OnXXXAccess)(long lPerms, VARIANT_BOOL *bReturnValue);
	STDMETHOD(OnXXXChanged)(long lHint, VARIANT varHintData);

protected:
	HRESULT InstallCoreExemplars(IModule *pModule);
};

#endif //__VWSYSEXO_H_
