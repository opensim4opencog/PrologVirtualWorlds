// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// StateMch.cpp : Implementation of StateMachine exemplar.

#include "stdafx.h"
//#include <vector.h>
#include <menuitem.h>
#include <vwanim.h>
#include <vwutils.h>
#include <vwsysex.h>
#include "syshelp.h"
#include "statemch.h"

/////////////////////////////////////////////////////////////////////////////
//

// Note: State properties are marked as writeable by anyone. While this opens a bit of a
// security hole, it allows sub-exemplars to directly manipulate the state properties
// with less overhead.

const CComBSTR bstrStateMachines = "StateMachines";
const CComBSTR bstrDefault = "Default";

HRESULT CStateMachineExemplarObject::Install(IModule* pModule)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	CMethodInfo rgMethod[] =
	{
		// flags							name						internal						psbits							perms
		{ METHOD_SERVER,					"AddStateMachine",			"SMchAddStateMachine",		PSBIT_EXEMPLARDEFAULTMETHOD,			PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER | METHOD_CLIENT,	"SetState",					"SMchSetState",				PSBIT_EXEMPLARDEFAULTMETHOD,			PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER | METHOD_CLIENT,	"SetStateMachineState",		"SMchSetStateMachineState",	PSBIT_EXEMPLARDEFAULTMETHOD,			PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER | METHOD_CLIENT,	"IsInState",				"SMchIsInState",			PSBIT_EXEMPLARDEFAULTMETHOD,			PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER | METHOD_CLIENT,	"IsStateMachineInState",	"SMchIsStateMachineInState",PSBIT_EXEMPLARDEFAULTMETHOD,			PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER | METHOD_CLIENT,	"SetIfState",				"SMchSetIfState",			PSBIT_EXEMPLARDEFAULTMETHOD,			PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER | METHOD_CLIENT,	"SetStateMachineIfState",	"SMchSetStateMachineIfState",	PSBIT_EXEMPLARDEFAULTMETHOD,		PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER,					"PutInlineStateMethod",		"SMchPutInlineStateMethod",	PSBIT_EXEMPLARDEFAULTMETHOD,			PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER,					"PutStateMachineInlineStateMethod",		"SMchPutStateMachineInlineStateMethod",	PSBIT_EXEMPLARDEFAULTMETHOD, PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER | METHOD_CLIENT,	"GetInlineStateMethod",		"SMchGetInlineStateMethod",	PSBIT_EXEMPLARDEFAULTMETHOD,			PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER,					"AddStateMethod",			"SMchAddStateMethod",		PSBIT_EXEMPLARDEFAULTMETHOD,			PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER | METHOD_CLIENT,	"GetStateMethodFlags",		"SMchGetStateMethodFlags",	PSBIT_EXEMPLARDEFAULTMETHOD,			PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER | METHOD_CLIENT,	"PutStateProperty",			"SMchPutStateProperty",		PSBIT_EXEMPLARDEFAULTMETHOD,			PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER | METHOD_CLIENT,	"PutStateMachineStateProperty",		"SMchPutStateMachineStateProperty",	PSBIT_EXEMPLARDEFAULTMETHOD, PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER | METHOD_CLIENT,	"GetStateProperty",			"SMchGetStateProperty",		PSBIT_EXEMPLARDEFAULTMETHOD,			PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER | METHOD_CLIENT,	"OnInvokeMethodFailed",		"SMchOnInvokeMethodFailed",	PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,			PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER | METHOD_CLIENT,	"GetStateConcatName",		"SMchGetStateConcatName",	PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,			PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_SERVER | METHOD_CLIENT,	"GetCurrentStateConcatName","SMchGetCurrentStateConcatName", PSBIT_EXEMPLARDEFAULTMETHOD | PSBIT_HIDDEN,		PS_EXEMPLARDEFAULTMETHOD }

	};
	int cMethod = sizeof(rgMethod)/sizeof(CMethodInfo);

	// Create the state machine exemplar
	CComPtr<IThing> pSMchEx;
	CComPtr<IPropertyList> pList;
	CComBSTR bstrName;
	CComBSTR bstrNameInternal;
    HRESULT  hr = S_OK;
    int      iMethod;

	if (FAILED(hr = m_pWorld->CreateExemplar(CComBSTR("StateMachine"), CComVariant(CComBSTR("Artifact")), &pSMchEx)))
        goto ERROR_ENCOUNTERED;

	// Set the description string
//	if (FAILED(hr = pSMchEx->put_Description(CComBSTR("This is a state machine."))))
//	    goto ERROR_ENCOUNTERED;

	// add in the methods
	for (iMethod=0; iMethod<cMethod; iMethod++)
	{
		bstrName = rgMethod[iMethod].pszName;

		if (rgMethod[iMethod].pszNameInternal == NULL)
			bstrNameInternal = bstrName;
		else
			bstrNameInternal = rgMethod[iMethod].pszNameInternal;
		
		if (FAILED(hr = pSMchEx->CreateAndAddMethodExt(rgMethod[iMethod].lFlags,
													pModule,
													bstrNameInternal,
													bstrName,
													rgMethod[iMethod].psbits,
													rgMethod[iMethod].permissions)))
            goto ERROR_ENCOUNTERED;
	}

	// Create the default properties
	if (FAILED(hr = pSMchEx->AddPropertyExt(bstrDefault, CComVariant(CComBSTR("")),
									   PSBIT_EXEMPLARDEFAULTPROPERTY, 
									   PS_EXEMPLARDEFAULTCOLLECTION | (PS_WRITE << 16),
									   VT_BSTR, IID_NULL, NULL)))
    	goto ERROR_ENCOUNTERED;

	if (FAILED(hr = CreatePropertyList(m_pWorld, &pList)))
	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pList->AddString(bstrDefault)))
	    goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pSMchEx->AddPropertyExt(bstrStateMachines, 
											CComVariant(pList), 
											PSBIT_EXEMPLARDEFAULTPROPERTY,
											PS_EXEMPLARDEFAULTCOLLECTION | (PS_WRITE << 16), 
											VT_DISPATCH, IID_IPropertyList, NULL)))
    	goto ERROR_ENCOUNTERED;

	pList.Release();
    
    return S_OK;

ERROR_ENCOUNTERED:
    VWTRACE(m_pWorld, "VWSTATEMCH", VWT_ERROR, "CStateMachineExemplarObject::Install: Error encountered: %x\n", hr);
    
	return hr;
}

STDMETHODIMP CStateMachineExemplarObject::SMchAddStateMachine(BSTR bstrStateMachineName,
															  BSTR bstrInitialValue)
{
	// Get the this object
	CComPtr<IThing> pThis;
	CComPtr<IPropertyList> pList;
    HRESULT hr = S_OK;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	// Add the new state property
	if (FAILED(hr = pThis->AddPropertyExt(	bstrStateMachineName, 
										CComVariant(bstrInitialValue),
										PSBIT_COPYONGET, 
										PS_EXEMPLARDEFAULTPROPERTY | (PS_WRITE << 16),
										VT_BSTR, IID_NULL, NULL)))
	    return hr;

	if (FAILED(hr = pThis->get_ObjectProperty(bstrStateMachines, (IObjectProperty**) &pList)) || !pList)
	    return hr;

	hr = pList->AddString(bstrStateMachineName);
    
	return hr;
}

STDMETHODIMP CStateMachineExemplarObject::SMchSetState(BSTR bstrState)
{
	return SMchSetStateMachineState(bstrDefault, bstrState);
}

STDMETHODIMP CStateMachineExemplarObject::SMchSetStateMachineState(BSTR bstrStateMachineName, BSTR bstrState)
{
	// Get the this object
	CComPtr<IThing> pThis;
	CComPtr<IVWAnimator> pVWAnimator;
	CComPtr<IVWAnimation> pTimer;
	CComBSTR bstrTimerName;
	CComBSTR bstrTimeOut;
    HRESULT  hr = S_OK;
	VARIANT_BOOL bValidProperty = VARIANT_FALSE;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	// Get the Animator
	if (FAILED(hr = m_pWorld->get_Tool(CComBSTR("Animator"), (IUnknown**) &pVWAnimator)))
    	return hr;

	// Get the name of the timer
	bstrTimerName = bstrStateMachineName;
	bstrTimerName += "_Timer";

	// Get the old timer (if any)
	pThis->get_Dispatch(bstrTimerName, (IDispatch**) &pTimer);

	// If there is an old timer, kill it
	if (pTimer)
	{
		// Remove the timer
		if (FAILED(hr = pVWAnimator->Remove(pTimer)))
        	return hr;

		// Take it out of the property
		if (FAILED(hr = pThis->put_Dispatch(bstrTimerName, NULL)))
	        return hr;

        pTimer.Release();
	}

	// Set the new state
	if (FAILED(hr = pThis->put_String(bstrStateMachineName, bstrState)))
	    return hr;

	// Get the name of the time-out for this state
	if (FAILED(hr = SMchGetCurrentStateConcatName(bstrStateMachineName, CComBSTR("TimeOut"),
												&bstrTimeOut.m_str)))
	    return hr;

	// Invoke the OnEnter method
	SMchInvokeStateMethod(bstrStateMachineName, CComBSTR("OnEnterState"), NULL);

	// If the time-out value's not there, don't start a timer
	// Note: This could fail if the OnEnterState caused the object to be destroyed
	pThis->IsValidProperty(bstrTimeOut, &bValidProperty);
	if (bValidProperty)
	{
		// Check that we're still in the same state (don't start timer on another state)
		VARIANT_BOOL bInState;
		if (FAILED(hr = SMchIsStateMachineInState(bstrStateMachineName, bstrState, &bInState)))
        	return hr;

		if (bInState)
		{
			// Check that no one else has started a timer during the OnEnterState call
			pThis->get_Dispatch(bstrTimerName, (IDispatch**) &pTimer);
			if (pTimer == NULL)
			{
				// Get the time-out value
				CComVariant timeOut;
				if (FAILED(hr = pThis->get_Property(bstrTimeOut, &timeOut)))
                	return hr;
				if (FAILED(hr = timeOut.ChangeType(VT_I4)))
                {
                    VWTRACE(m_pWorld, "VWSTATEMCH", VWT_IMPORTANT, "CStateMachineExemplarObject::SMchSetStateMachineState: timeout not an integer\n");
	                return ReportStateMachineError(hr);
                }

				// Create a new timer
				if (FAILED(hr = pVWAnimator->CreateExt(timeOut.lVal, pThis,
												   CComBSTR("OnTimeOut"),
												   NULL,
												   &pTimer)))
            	    return hr;

				// Stick it in the property
				if (FAILED(hr = pThis->IsValidProperty(bstrTimerName, &bValidProperty)))
	                return hr;

				if (bValidProperty)
				{
					if (FAILED(hr = pThis->put_Dispatch(bstrTimerName, pTimer)))
	                    return hr;
				}
				else
				{
					// Add the new state property
					if (FAILED(hr = pThis->AddPropertyExt(bstrTimerName,
													 CComVariant((IDispatch*) pTimer),
													 PSBIT_COPYONGET | PSBIT_NOTREMOTED | PSBIT_HIDDEN,
													 PS_EXEMPLARDEFAULTPROPERTY | (PS_WRITE << 16),
													 VT_DISPATCH, IID_IDispatch, NULL)))
                        return hr;
				}
			}
		}
	}

	return hr;
}

STDMETHODIMP CStateMachineExemplarObject::SMchIsInState(BSTR bstrState, VARIANT_BOOL *pboolResult)
{
	return SMchIsStateMachineInState(bstrDefault, bstrState, pboolResult);
}

STDMETHODIMP CStateMachineExemplarObject::SMchIsStateMachineInState(BSTR bstrStateMachineName, BSTR bstrState, 
									 VARIANT_BOOL *pboolResult)
{
	// Get the this object
	CComPtr<IThing> pThis;
    HRESULT         hr = S_OK;

    if (pboolResult==NULL)
    {
        VWTRACE(m_pWorld, "VWSTATEMCH", VWT_IMPORTANT, "CStateMachineExemplarObject::SMchIsStateMachineInState: NULL output pointer\n");
	    return ReportStateMachineError(E_POINTER);
    }

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	// Get the current state value
	CComBSTR bstrStateValue;
	if (SUCCEEDED(hr = pThis->get_String(bstrStateMachineName, &bstrStateValue.m_str)))
	    // Test if it's equal
	    *pboolResult = CString(bstrStateValue) == CString(bstrState);

	return hr;
}

STDMETHODIMP CStateMachineExemplarObject::SMchSetIfState(BSTR bstrOldState, BSTR bstrNewState)
{
	return SMchSetStateMachineIfState(bstrDefault, bstrOldState, bstrNewState);
}

STDMETHODIMP CStateMachineExemplarObject::SMchSetStateMachineIfState(BSTR bstrStateMachineName, BSTR bstrOldState, 
									  BSTR bstrNewState)
{
	// Check if we're in the old state
	VARIANT_BOOL bIsInState;
    HRESULT      hr = S_OK;

	if (FAILED(hr = SMchIsStateMachineInState(bstrStateMachineName, bstrOldState, &bIsInState)))
	    return hr;

	if (bIsInState)
		// If so, set the new state value
		hr = SMchSetStateMachineState(bstrStateMachineName, bstrNewState);

	return hr;
}

STDMETHODIMP CStateMachineExemplarObject::SMchGetStateConcatName(BSTR bstrStateMachineName, BSTR bstrState, 
								  BSTR bstrName, BSTR* pbstrOut)
{
    if (pbstrOut==NULL)
    {
        VWTRACE(m_pWorld, "VWSTATEMCH", VWT_IMPORTANT, "CStateMachineExemplarObject::SMchGetStateConcatName: NULL output pointer\n");
	    return ReportStateMachineError(E_POINTER);
    }

	CComBSTR result;
	result = bstrStateMachineName;
	result += "_";
	result += bstrState;
	result += "_";
	result += bstrName;

	*pbstrOut = result.Detach();

	return S_OK;
}

STDMETHODIMP CStateMachineExemplarObject::SMchGetCurrentStateConcatName(BSTR bstrStateMachineName, 
										 BSTR bstrName, BSTR *pbstrOut)
{
	// Get the this object
	CComPtr<IThing> pThis;
    HRESULT         hr = S_OK;

    if (pbstrOut==NULL)
    {
        VWTRACE(m_pWorld, "VWSTATEMCH", VWT_IMPORTANT, "CStateMachineExemplarObject::SMchGetCurrentStateConcatName: NULL output pointer\n");
	    return ReportStateMachineError(E_POINTER);
    }

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	// Get the current state
	CComBSTR bstrVal;
	if (FAILED(hr = pThis->get_String(bstrStateMachineName, &bstrVal.m_str)))
	    return hr;

	// Now calculate the answer
	hr = SMchGetStateConcatName(bstrStateMachineName, bstrVal, bstrName, pbstrOut);

	return hr;
}

STDMETHODIMP CStateMachineExemplarObject::SMchGetStateProperty(BSTR bstrStateMachineName, BSTR bstrState, 
								BSTR bstrPropertyName, VARIANT *var)
{
	HRESULT hr = S_OK;

    if (var==NULL)
    {
        VWTRACE(m_pWorld, "VWSTATEMCH", VWT_IMPORTANT, "CStateMachineExemplarObject::SMchGetStateProperty: NULL output pointer\n");
	    return ReportStateMachineError(E_POINTER);
    }

	// Get the property name
	CComBSTR bstrStatePropertyName;
	if (FAILED(hr = SMchGetStateConcatName(bstrStateMachineName, bstrState, bstrPropertyName,
										&bstrStatePropertyName.m_str)))
        return hr;

	// Get the this object
	CComPtr<IThing>pThis;
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	// Put the property
	hr = pThis->get_Property(bstrStatePropertyName, var);

	return hr;
}

STDMETHODIMP CStateMachineExemplarObject::SMchPutStateProperty(BSTR bstrState,
								BSTR bstrPropertyName, VARIANT var)
{
	return SMchPutStateMachineStateProperty(bstrDefault, bstrState, bstrPropertyName, var);
}

STDMETHODIMP CStateMachineExemplarObject::SMchPutStateMachineStateProperty(BSTR bstrStateMachineName, BSTR bstrState, 
								BSTR bstrPropertyName, VARIANT var)
{
	CComBSTR bstrStatePropertyName;
    HRESULT  hr = S_OK;

	// Get the property name
	if (FAILED(hr = SMchGetStateConcatName(bstrStateMachineName, bstrState, bstrPropertyName,
										&bstrStatePropertyName.m_str)))
        return hr;

	// Get the this object
	CComPtr<IThing> pThis;
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	// Existing property?
	VARIANT_BOOL bValidProperty;
	if (SUCCEEDED(hr = pThis->IsValidProperty(bstrStatePropertyName, &bValidProperty)))
    {
	    if (bValidProperty)
		    // Put the property
		    hr = pThis->put_Property(bstrStatePropertyName, var);
	    else
		    // Add the property
		    hr = pThis->AddPropertyExt(bstrStatePropertyName, var, 
											PSBIT_COPYONGET | PSBIT_HIDDEN, 
											PS_EXEMPLARDEFAULTPROPERTY,
										    var.vt, IID_IDispatch, NULL);
    }

	return hr;
}

STDMETHODIMP CStateMachineExemplarObject::SMchPutInlineStateMethod(long lFlags, 
									BSTR bstrState,
									BSTR bstrMethodName, BSTR bstrCode,
									VARIANT varParams)
{
	return SMchPutStateMachineInlineStateMethod(lFlags, 
												bstrDefault, bstrState,
												bstrMethodName, bstrCode,
												varParams);
}

STDMETHODIMP CStateMachineExemplarObject::SMchPutStateMachineInlineStateMethod(long lFlags, 
									BSTR bstrStateMachineName, BSTR bstrState,
									BSTR bstrMethodName, BSTR bstrCode,
									VARIANT varParams)
{
	CComPtr<IMethod> pMethod;
	CComBSTR bstrUniqueName;

	if (varParams.vt != VT_BSTR)
    {
        VWTRACE(m_pWorld, "VWSTATEMCH", VWT_IMPORTANT, "CStateMachineExemplarObject::SMchPutStateMachineInlineStateMethod: function body must be a BSTR\n");
	    return ReportStateMachineError(E_INVALIDARG);
    }

	CComPtr<IThing> pThis;
	CComBSTR bstrStateMethodName;
	VARIANT_BOOL bMethodPresent;
    HRESULT      hr = S_OK;

    // Get the this object
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
	    return hr;

	// Get the method name
	if (FAILED(hr = SMchGetStateConcatName(bstrStateMachineName, bstrState, bstrMethodName,
				&bstrStateMethodName.m_str)))
	    return hr;

	// Check if the method is there already
	if (FAILED(hr = pThis->IsValidMethod(bstrStateMethodName, &bMethodPresent)))
	    return hr;

    // If not, create it
	if (!bMethodPresent)
	{
		// Create a unique subroutine name
		long lObjID;
		time_t lTime;
		CString cstringUniqueName;
		if (FAILED(hr = pThis->get_ID(&lObjID)))
	        return hr;

		time(&lTime);
		cstringUniqueName.Format("_%lx%lx", lObjID, lTime);
		bstrUniqueName = CComBSTR(CString(bstrStateMethodName) + cstringUniqueName);

		// Create the inline method
		if (FAILED(hr = m_pWorld->CreateInlineMethod(   lFlags, CComBSTR("VBScript"),
												        bstrUniqueName,
												        CComBSTR(CString("function ") +
														bstrUniqueName.m_str +
														CString("(") +
														varParams.bstrVal +
														CString(") : end function")),
												        &pMethod)))
	        return hr;


		// Put it in the trigger
		if (FAILED(hr = pThis->AddMethodExt(bstrStateMethodName, pMethod, 
											PSBIT_HIDDEN, 
											PS_EXEMPLARDEFAULTMETHOD)))
        	return hr;
		pMethod.Release();

		if (FAILED(hr = pThis->get_Method(bstrStateMethodName, &pMethod)) || !pMethod)
        	return hr;
	}
	else
	{
		long lOldFlags;
		// Otherwise, get the existing in-line method & name
		if (FAILED(hr = pThis->get_Method(bstrStateMethodName, &pMethod))|| !pMethod)
	        return hr;

		if (FAILED(hr = pMethod->get_MethodName(&bstrUniqueName.m_str)))
	        return hr;

		// KM: placed put_Flags in 'else' section since CreateInlineMethod already would set server bit
		// for new methods; this would incur another network send for newly created methods

		// overwrite client/server bits
		if (FAILED(hr = pMethod->get_Flags(&lOldFlags)))
        	return hr;

		if (FAILED(hr = pMethod->put_Flags(METHODCLIENTSERVER(lOldFlags, lFlags))))
	        return hr;
	}

	// Now insert the code
	hr = pMethod->put_Code( CComBSTR(CString("function ") +
							bstrUniqueName.m_str + CString("(") +
							varParams.bstrVal + CString(") : ") + bstrCode +
							CString(" : end function")));

	return hr;
}

STDMETHODIMP CStateMachineExemplarObject::SMchGetInlineStateMethod(BSTR bstrStateMachineName, BSTR bstrState,
									BSTR bstrMethodName, BSTR* bstrCode)
{
	CComBSTR bstrMethodCode;
	CComBSTR bstrStateMethodName;
	CString cstringCode;
	CComPtr<IMethod> pMethod;
	CComPtr<IThing> pThis;
    HRESULT         hr = S_OK;
    int             iStart, iCount;

    if (bstrCode==NULL)
    {
        VWTRACE(m_pWorld, "VWSTATEMCH", VWT_IMPORTANT, "CStateMachineExemplarObject::SMchGetInlineStateMethod: NULL output pointer\n");
	    return ReportStateMachineError(E_POINTER);
    }

	// Get the this object
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

	// Get the method name
	if (FAILED(hr = SMchGetStateConcatName(bstrStateMachineName, bstrState, bstrMethodName,
				&bstrStateMethodName.m_str)))
        return hr;

	// Get the method
	if (FAILED(hr = pThis->get_Method(bstrStateMethodName, &pMethod)) || !pMethod)
    	return hr;

	// Get the code
	if (FAILED(hr = pMethod->get_Code(&bstrMethodCode.m_str)))
    	return hr;

	// Convert to a CString for processing
	cstringCode = bstrMethodCode;

	// Find the actual code within the function
	iStart = cstringCode.Find(':')+2;
	iCount = cstringCode.ReverseFind(':') - iStart - 1;

	// Return the code
	if (iCount > 0)
		*bstrCode = CComBSTR(cstringCode.Mid(iStart, iCount)).Detach();
	else
		*bstrCode = CComBSTR("").Detach();
    
	return S_OK;
}

STDMETHODIMP CStateMachineExemplarObject::SMchAddStateMethod(long lFlags, IModule *pModule,
							  BSTR bstrModuleMethodName,
							  BSTR bstrStateMachineName, BSTR bstrState,
							  BSTR bstrMethodName)
{
	CComPtr<IThing> pThis;
    HRESULT         hr = S_OK;

	// Get the this object
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

	// Get the method name
	CComBSTR bstrStateMethodName;
	if (SUCCEEDED(hr = SMchGetStateConcatName(bstrStateMachineName, bstrState, bstrMethodName,
				&bstrStateMethodName.m_str)))
    {
        // Add the method
	    hr = pThis->CreateAndAddMethodExt(  lFlags, pModule, bstrModuleMethodName,
											bstrStateMethodName, 
											PSBIT_HIDDEN, 
											PS_EXEMPLARDEFAULTMETHOD);
    }

	return hr;
}

STDMETHODIMP CStateMachineExemplarObject::SMchGetStateMethodFlags(BSTR bstrStateMachineName, BSTR bstrState,
								   BSTR bstrMethodName, long *plFlags)
{
	CComPtr<IThing> pThis;
    HRESULT         hr = S_OK;

    if (plFlags==NULL)
    {
        VWTRACE(m_pWorld, "VWSTATEMCH", VWT_IMPORTANT, "CStateMachineExemplarObject::SMchGetStateMethodFlags: NULL output pointer\n");
	    return ReportStateMachineError(E_POINTER);
    }

	// Get the this object
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

	// Get the method name
	CComBSTR bstrStateMethodName;
	if (FAILED(hr = SMchGetStateConcatName(bstrStateMachineName, bstrState, bstrMethodName,
				&bstrStateMethodName.m_str)))
        return hr;

	// Get the method
	CComPtr<IMethod> pMethod;
	if (SUCCEEDED(hr = pThis->get_Method(bstrStateMethodName, &pMethod)) && pMethod)
    {
    	// Get the server flag
	    if (SUCCEEDED(hr = pMethod->get_Flags(plFlags)))
        	// mask out only the client/server and sync bits
	        *plFlags &= ~(METHOD_CLIENTSERVERMASK & METHOD_SYNCMASK);
    }

	return S_OK;
}

STDMETHODIMP CStateMachineExemplarObject::SMchInvokeStateMethod(BSTR bstrStateMachineName, BSTR bstrMethodName,
								 DISPPARAMS *pdispparams)
{
	CComPtr<IThing> pThis;
	VARIANT_BOOL	bMethodValid;
    HRESULT         hr = S_OK;

	// Get the this object
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

	// Get the method name
	CComBSTR bstrStateMethodName;
	if (FAILED(hr = SMchGetCurrentStateConcatName(bstrStateMachineName, bstrMethodName,
												&bstrStateMethodName.m_str)))
        return hr;

	// Only call if method exists, to stop recursion via OnInvokeMethodFailed.
	if (SUCCEEDED(hr = pThis->IsValidMethod(bstrStateMethodName, &bMethodValid)))
    {
	    if (bMethodValid)
		    // Invoke the method
		    hr = pThis->InvokeMethodExt(bstrStateMethodName, pdispparams, NULL);
    }

	return hr;
}

STDMETHODIMP CStateMachineExemplarObject::SMchInvokeAllStateMethods(BSTR bstrMethodName,
															 DISPPARAMS* pdispparams)
{
	CComPtr<IThing> pThis;
    HRESULT         hr = S_OK;

	// Get the this object
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
    	return hr;

	// Get the state properties list
	CComPtr<IPropertyList> pList;
	if (SUCCEEDED(hr = pThis->get_ObjectProperty(bstrStateMachines,
											(IObjectProperty**) &pList)))
    {
	    // Iterate through all the state property names
	    CComBSTR bstrStateMachineName;
	    for (int i = 0; SUCCEEDED(pList->get_String(i, &bstrStateMachineName.m_str)); i++)
	    {
		    SMchInvokeStateMethod(bstrStateMachineName, bstrMethodName, pdispparams);
		    bstrStateMachineName.Empty();
	    }
    }

	return hr;
}

STDMETHODIMP CStateMachineExemplarObject::SMchOnInvokeMethodFailed(BSTR bstrMethodName, IPropertyList *plistParams)
{
	CComDISPPARAMS  dispparams;
    HRESULT         hr = S_OK;

	// Call the appropriate state methods (if they exist).
	if (SUCCEEDED(hr = ConvertPropertyListToDISPPARAMS(plistParams, dispparams)))
    	hr = SMchInvokeAllStateMethods(bstrMethodName, dispparams);

	dispparams.Terminate(VARIANT_TRUE);

	return hr;
}
