// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWAnmtOb.cpp 

#include "stdafx.h"
#include <mmsystem.h>
#include <VWAnim.h>
#include "VWAnmtOb.h"

typedef CComObject<CVWAnimation> CComAnimation;

/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo

STDMETHODIMP CVWAnimation::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IVWAnimation,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Constructor/destructor

CVWAnimation::CVWAnimation() :
	m_eAnimType(eNotSet),
	m_pWorld(NULL),
	m_pthing(NULL),
	m_pacb(NULL),
	m_msecCurrent(0),
	m_msecPeriod(0),
	m_cInvoked(0),
	m_iInvokeMax(0),
	m_pUser(NULL)
{
	DWORD dwNow = ::GetTickCount();

	m_msecCreated = dwNow;
	m_msecLastTimeInvoked = dwNow;

	InitializeDISPPARAMS(&m_dispparams, 0);
}

CVWAnimation::~CVWAnimation()
{
	Terminate();
}

STDMETHODIMP CVWAnimation::Initialize(IWorld *pWorld)
{
	ASSERT(m_pWorld == NULL);

	// stow
	SAFERELEASE(m_pWorld);
	m_pWorld = pWorld;
	SAFEADDREF(m_pWorld);

	return S_OK;
}

STDMETHODIMP CVWAnimation::Terminate()
{
	SAFERELEASE(m_pthing);
	SAFERELEASE(m_pUser);

	TerminateDISPPARAMS(VARIANT_TRUE, &m_dispparams);

	SAFERELEASE(m_pacb);

	SAFERELEASE(m_pWorld);

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// IVWAnimation

STDMETHODIMP CVWAnimation::Invoke(int msecNow)
{
	// make sure we have what we need
	if (((m_eAnimType == eMethod || m_eAnimType == eEvent) && m_pthing == NULL) || 
		(m_eAnimType == eCallback && m_pacb == NULL) ||
		(m_eAnimType == eUIEvent) || 
		(m_eAnimType == eNotSet))
	{
		m_msecLastTimeInvoked = msecNow;
		m_msecCurrent = 0;
		return E_FAIL;
	}

	IThing* pThingOldUser = NULL;
	VARIANT_BOOL bServerSide = VARIANT_FALSE;
	CComVariant varConn;
	IVWCommConnection* pConnection = NULL;
	IVWCommConnection* pOldConnection = NULL;

	m_pWorld->get_ServerSide(&bServerSide);

	if (bServerSide == VARIANT_TRUE)
	{
		// get current user (should be NULL on server)
		m_pWorld->get_User(&pThingOldUser);
		ASSERT(pThingOldUser == NULL);

		// force in stashed user
		m_pWorld->put_User(m_pUser);
		//	ASSERT(m_pUser);

		if (m_pUser)
		{
			// get current connection (should be NULL on server)
			m_pWorld->get_Connection(&pOldConnection);
			ASSERT(pOldConnection == NULL);

			// grab connection from current user
			if (SUCCEEDED(m_pUser->get_Property(CComBSTR("Connection"), &varConn)))
			{
				pConnection = (IVWCommConnection*)varConn.punkVal;
				ASSERT(pConnection);

				m_pWorld->put_Connection(pConnection);
			}
		}
	}

	// set current time
	m_msecCurrent = msecNow;

	// invoke, as appropriate
	HRESULT hr;

	// If we fail in here, trace out debugging info here and handle failure further down
	if (m_eAnimType == eMethod)
	{
		// REVIEW: should we add time argument(s) to arglist?
		hr = m_pthing->InvokeMethodExt(m_bstrMethod, &m_dispparams, NULL);
		if (FAILED(hr))
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CVWAnimation::Invoke: Failed to invoke method %s\n", CString(m_bstrMethod));

	}
	else if (m_eAnimType == eEvent)
	{
		// REVIEW: should we add time argument(s) to arglist?
		hr = m_pthing->FireEventExt(m_bstrEvent, &m_dispparams, FIREEVENT_DEFAULT);
		if (FAILED(hr))
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CVWAnimation::Invoke: Failed to invoke event %s\n", CString(m_bstrEvent));

	}
	else if (m_eAnimType == eUIEvent)
	{
		// REVIEW: should we add time argument(s) to arglist?
		hr = m_pWorld->FireUIEvent(m_pthing, m_bstrEvent, m_varArgs);
		if (FAILED(hr))
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CVWAnimation::Invoke: Failed to invoke UI event %s\n", CString(m_bstrEvent));

	}
	else if (m_eAnimType == eCallback)
	{
		// call the callback
		hr = m_pacb->Invoke(this);
		if (FAILED(hr))
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CVWAnimation::Invoke: Failed to invoke callback");
	}
	else
		ASSERT(FALSE);

	if (bServerSide == VARIANT_TRUE)
	{
		// reset user
		m_pWorld->put_User(pThingOldUser);
		SAFERELEASE(pThingOldUser);

		// reset connection if we stashed on
		if ( m_pUser )
		{
			m_pWorld->put_Connection(pOldConnection);
		}

		SAFERELEASE(pOldConnection);
	}

	if (FAILED(hr))
	{
		m_msecLastTimeInvoked = msecNow;
		m_msecCurrent = 0;
		return E_FAIL;
	}

	// it's ok
	m_cInvoked++;
	m_msecLastTimeInvoked = msecNow;
	m_msecCurrent = 0;

	return S_OK;
}

STDMETHODIMP CVWAnimation::Clone(IVWAnimation **ppAnimation)
{
	HRESULT hr = S_OK;

	// create a new animation
	CVWAnimation *pani = new CComAnimation;
	if (pani == NULL)
		return E_FAIL;

	// now copy stuff from this to the new one
	*pani = *this;

	// return the clone animation
	*ppAnimation = pani;
	(*ppAnimation)->AddRef();

	hr = (*ppAnimation)->Initialize(m_pWorld);

	return hr;
}

STDMETHODIMP CVWAnimation::get_TimeOfLastInvocation(int *pmsec)
{
	*pmsec = m_msecLastTimeInvoked;
	return S_OK;
}

STDMETHODIMP CVWAnimation::get_TimeOfCreation(int *pmsec)
{
	*pmsec = m_msecCreated;
	return S_OK;
}

STDMETHODIMP CVWAnimation::get_CurrentTime(int *pmsec)
{
	// if currently in an invoke, return that time
	if (m_msecCurrent != 0)
		*pmsec = m_msecCurrent;
	else
		// else, return clock time
		// REVIEW: should we return 0?
		*pmsec = ::GetTickCount();

	return S_OK;
}

STDMETHODIMP CVWAnimation::get_TimesInvoked(int *pcInvoked)
{
	*pcInvoked = m_cInvoked;
	return S_OK;
}


STDMETHODIMP CVWAnimation::put_InvokeMax(int iInvokeMax)
{
	m_iInvokeMax = iInvokeMax;
	return S_OK;
}

STDMETHODIMP CVWAnimation::get_InvokeMax(int *iInvokeMax)
{
	*iInvokeMax = m_iInvokeMax;
	return S_OK;
}

STDMETHODIMP CVWAnimation::put_Period(int msecPeriod)
{
	m_msecPeriod = msecPeriod;
	return S_OK;
}

STDMETHODIMP CVWAnimation::get_Period(int *pmsecPeriod)
{
	*pmsecPeriod = m_msecPeriod;
	return S_OK;
}

STDMETHODIMP CVWAnimation::put_Thing(IThing *pthing)
{
	SAFERELEASE(m_pthing);
	m_pthing = pthing;
	SAFEADDREF(m_pthing);

	return S_OK;
}

STDMETHODIMP CVWAnimation::get_Thing(IThing **ppthing)
{
	*ppthing = m_pthing;
	SAFEADDREF(*ppthing);

	return S_OK;
}

STDMETHODIMP CVWAnimation::put_User(IThing *pthing)
{
	SAFERELEASE(m_pUser);
	m_pUser = pthing;
	SAFEADDREF(m_pUser);

	return S_OK;
}

STDMETHODIMP CVWAnimation::get_User(IThing **ppthing)
{
	*ppthing = m_pUser;
	SAFEADDREF(*ppthing);

	return S_OK;
}

STDMETHODIMP CVWAnimation::put_Method(BSTR bstrMethod)
{
	// copy the bstr
	m_bstrMethod = bstrMethod;

	// set the type (and clear the unused field)
	m_eAnimType = eMethod;

	m_bstrEvent.Empty();
	SAFERELEASE(m_pacb);

	return S_OK;
}

STDMETHODIMP CVWAnimation::get_Method(BSTR *pbstrMethod)
{
	// check the type
	if (m_eAnimType != eMethod)
		return E_FAIL;

	// make a copy of the method
	*pbstrMethod = m_bstrMethod.Copy();

	return S_OK;
}

STDMETHODIMP CVWAnimation::put_Event(BSTR bstrEvent)
{
	// copy the bstr
	m_bstrEvent = bstrEvent;

	// set the type (and clear the unused field)
	m_eAnimType = eEvent;

	m_bstrMethod.Empty();
	SAFERELEASE(m_pacb);

	return S_OK;
}

STDMETHODIMP CVWAnimation::get_Event(BSTR *pbstrEvent)
{
	// check the type
	if (m_eAnimType != eEvent)
		return E_FAIL;

	// make a copy of the Event
	*pbstrEvent = m_bstrEvent.Copy();

	return S_OK;
}

STDMETHODIMP CVWAnimation::put_UIEvent(BSTR bstrUIEvent)
{
	// copy the bstr
	m_bstrEvent = bstrUIEvent;

	// set the type (and clear the unused field)
	m_eAnimType = eUIEvent;

	m_bstrMethod.Empty();
	SAFERELEASE(m_pacb);

	return S_OK;
}

STDMETHODIMP CVWAnimation::get_UIEvent(BSTR *pbstrUIEvent)
{
	// check the type
	if (m_eAnimType != eUIEvent)
		return E_FAIL;

	// make a copy of the Event
	*pbstrUIEvent = m_bstrEvent.Copy();

	return S_OK;
}

STDMETHODIMP CVWAnimation::put_Callback(IVWAnimationCallback *pacb)
{
	// check arg
	if (pacb == NULL)
	{
		m_eAnimType = eNotSet;
		return E_FAIL;
	}

	// keep the pointer
	m_pacb = pacb;
	SAFEADDREF(m_pacb);

	// set the type (and clear the unused field)
	m_eAnimType = eCallback;

	m_bstrMethod.Empty();
	m_bstrEvent.Empty();

	return S_OK;
}

STDMETHODIMP CVWAnimation::get_Callback(IVWAnimationCallback **ppacb)
{
	// check the type
	if (m_eAnimType != eCallback)
		return E_FAIL;

	// return the value
	*ppacb = m_pacb;
	SAFEADDREF(*ppacb);

	return S_OK;
}

STDMETHODIMP CVWAnimation::put_ArgumentsExt(DISPPARAMS* pdispparams)
{
	HRESULT hr = S_OK;

	hr = CloneDISPPARAMS(pdispparams, &m_dispparams);

	return hr;
}

STDMETHODIMP CVWAnimation::get_ArgumentsExt(DISPPARAMS** ppdispparams)
{
	HRESULT hr = S_OK;

	*ppdispparams = new DISPPARAMS;
	if (*ppdispparams == NULL)
		return E_OUTOFMEMORY;

	hr = CloneDISPPARAMS(&m_dispparams, *ppdispparams);

	return hr;
}
