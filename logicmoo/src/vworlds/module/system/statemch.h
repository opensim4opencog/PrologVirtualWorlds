// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// Room.h : Declaration of the CRoom

#ifndef __STATEMACHINE_H_
#define __STATEMACHINE_H_

#include "resource.h"       // main symbols
#include "objimpl.h"

/////////////////////////////////////////////////////////////////////////////
// CStateMachine

EXTERN_C const CLSID CLSID_StateMachineExemplar;

#define ReportStateMachineError(hr) SUCCEEDED(hr) ? hr : ReportError("VWSYSTEM.StateMachine.1", hr, 0xc80, 0xc90, "VWSTATEMACHINE")

DECLARE_VWDISPATCH( StateMachineExemplar, LIBID_VWSYSTEMLib )
{
public:
DECLARE_REGISTRY_RESOURCEID(IDR_STATEMACHINEEXEMPLAR)

BEGIN_COM_MAP(CStateMachineExemplarObject)
	COM_INTERFACE_ENTRY(IStateMachineExemplar)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

// IStateMachine 

public:
	// StateMachine exemplar
	STDMETHOD(Install)(IModule* pModule);
	STDMETHOD(SMchAddStateMachine)(BSTR bstrStateMachineName, BSTR bstrInitialValue);
	STDMETHOD(SMchSetState)(BSTR bstrState);
	STDMETHOD(SMchSetStateMachineState)(BSTR bstrStateMachineName, BSTR bstrState);
	STDMETHOD(SMchIsInState)(BSTR bstrState, VARIANT_BOOL *pboolResult);
	STDMETHOD(SMchIsStateMachineInState)(BSTR bstrStateMachineName, BSTR bstrState, 
										 VARIANT_BOOL *pboolResult);
	STDMETHOD(SMchSetIfState)(BSTR bstrOldState, BSTR bstrNewState);
	STDMETHOD(SMchSetStateMachineIfState)(BSTR bstrStateMachineName, BSTR bstrOldState, 
										  BSTR bstrNewState);
	STDMETHOD(SMchGetStateConcatName)(BSTR bstrStateMachineName, BSTR bstrState, 
									  BSTR bstrName, BSTR* pbstrOut);
	STDMETHOD(SMchGetCurrentStateConcatName)(BSTR bstrStateMachineName, 
											 BSTR bstrName, BSTR *pbstrOut);
	STDMETHOD(SMchGetStateProperty)(BSTR bstrStateMachineName, BSTR bstrState, 
									BSTR bstrPropertyName, VARIANT *var);
	STDMETHOD(SMchPutStateProperty)(BSTR bstrState, 
									BSTR bstrPropertyName, VARIANT var);
	STDMETHOD(SMchPutStateMachineStateProperty)(BSTR bstrStateMachineName, BSTR bstrState, 
									BSTR bstrPropertyName, VARIANT var);
	STDMETHOD(SMchPutInlineStateMethod)(long lFlags, 
										BSTR bstrState,
										BSTR bstrMethodName, BSTR bstrCode,
										VARIANT varParams);
	STDMETHOD(SMchPutStateMachineInlineStateMethod)(long lFlags, 
										BSTR bstrStateMachineName, BSTR bstrState,
										BSTR bstrMethodName, BSTR bstrCode,
										VARIANT varParams);
	STDMETHOD(SMchGetInlineStateMethod)(BSTR bstrStateMachineName, BSTR bstrState,
										BSTR bstrMethodName, BSTR* bstrCode);
	STDMETHOD(SMchAddStateMethod)(long lFlags, IModule *pModule,
								  BSTR bstrModuleMethodName,
								  BSTR bstrStateMachineName, BSTR bstrState,
								  BSTR bstrMethodName);
	STDMETHOD(SMchGetStateMethodFlags)(BSTR bstrStateMachineName, BSTR bstrState,
									   BSTR bstrMethodName, long *plFlags);
	STDMETHOD(SMchOnInvokeMethodFailed)(BSTR bstrMethodName, IPropertyList *plistParams);

private:
	// Helpers.
	STDMETHOD(SMchInvokeStateMethod)(BSTR bstrStateMachineName, BSTR bstrMethodName,
									 DISPPARAMS *pdispparams);
	STDMETHOD(SMchInvokeAllStateMethods)(BSTR bstrMethodName,
										 DISPPARAMS *pdispparams);
};

#endif //__STATEMACHINE_H_
