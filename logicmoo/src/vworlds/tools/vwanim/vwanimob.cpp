// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWAnimOb.cpp : Implementation of CVWAnimApp and DLL registration.

#include "stdafx.h"
#include <mmsystem.h>
#include <VWAnim.h>
#include "VWAnimOb.h"
#include "VWAnmtOb.h"

//#define ANIMATOR_PERF_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Parameters

#define msecAnimatorPeriod		(1000/20)
#define msecAnimatorResolution	(1000/8)

//statics

UINT CVWAnimator::m_TimerOffer = 1;

typedef CComObject<CVWAnimation> CComAnimation;

/////////////////////////////////////////////////////////////////////////////
// Constructor/destructor

CVWAnimator::CVWAnimator() :
	m_pWorld(NULL),
	m_idTimer(0),
	m_pAnimCurrent(NULL),
	m_pDummyWnd(NULL)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	pTheAnimator = this;
}

HRESULT CVWAnimator::FinalConstruct(void)
{
	HRESULT hr = S_OK;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//create dummywindow.
	m_pDummyWnd = new CAnimDummyWnd(this);
	ASSERT(m_pDummyWnd);
	if (m_pDummyWnd)
	{
		m_pDummyWnd->CreateEx(0L, AfxRegisterWndClass(0), _T("AnimDummyWindow"), 
			WS_OVERLAPPED, 0, 0, 0, 0, NULL, NULL);		
	}
	
	return hr;
}

LRESULT CAnimDummyWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult;

	switch (message)
	{
		case WM_TIMER:			
			if (m_pAnim)
				m_pAnim->TimerHandler(wParam);
			return 0L;
	}

	lResult = CWnd::WindowProc(message, wParam, lParam);

	return lResult;
}

CVWAnimator::~CVWAnimator()
{
	Terminate();
}

/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo

STDMETHODIMP CVWAnimator::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IVWAnimator,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// IVWAnimator

STDMETHODIMP CVWAnimator::Initialize(IWorld *pWorld)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	ASSERT(m_pWorld == NULL);

	// stow
	SAFERELEASE(m_pWorld);
	m_pWorld = pWorld;
	SAFEADDREF(m_pWorld);

	// create timer
	m_idTimer = m_pDummyWnd->SetTimer(m_TimerOffer++, msecAnimatorPeriod, NULL);

	if (m_idTimer == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CVWAnimator::Initialize: failed to set timer\n");

		return E_FAIL;
	}
	else 
		return S_OK;
}

STDMETHODIMP CVWAnimator::Terminate()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// NOTE: InterworldPortal OnIntersectee is fired from an animator (cause
	// intersection is made in rendering thread) so ReConnect happens inside an animator
	// invoke. Should just shut down everything as normal here since animator invoke now
	// happens from a separate list.

	// kill the timer
	if (m_idTimer != 0)
	{
		::KillTimer(NULL, m_idTimer);
	}

	if (m_pDummyWnd)
	{
		m_pDummyWnd->DestroyWindow();
		delete m_pDummyWnd;
		m_pDummyWnd = NULL;
	}

	RemoveAll();

	pTheAnimator = NULL;
	
	// don't need to release, it's only cached during TimerHandler
//	SAFERELEASE(m_pAnimCurrent);

	SAFERELEASE(m_pWorld);

	return S_OK;
}

STDMETHODIMP CVWAnimator::CreateEvent(	int msecPeriod, 
									IThing *pThing, 
									BSTR bstrEvent, 
									VARIANT varArgs, 
									IVWAnimation **ppAnimation)
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

		hr = ConvertSAFEARRAYToDISPPARAMS(&varArgs, &dispparams);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = CreateEventExt(msecPeriod, pThing, bstrEvent, &dispparams, ppAnimation);

		TerminateDISPPARAMS(VARIANT_TRUE, &dispparams);
	}
	else
		hr = CreateEventExt(msecPeriod, pThing, bstrEvent, NULL, ppAnimation);

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CVWAnimator::CreateEventExt(int msecPeriod, 
									IThing *pThing, 
									BSTR bstrEvent, 
									DISPPARAMS* pdispparams, 
									IVWAnimation **ppAnimation)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// create an animation object
	IVWAnimation *pani;
	HRESULT hr = S_OK;
	IThing* pUser = NULL;

	if (ppAnimation == NULL)
		return E_POINTER;

	*ppAnimation = NULL;

	m_pWorld->get_User(&pUser);

	pani = new CComAnimation;
	if (!pani)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CVWAnimator::CreateEventExt: failed to allocaate animation\n");

		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}
	pani->AddRef();

	hr = pani->Initialize(m_pWorld);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// set its period and other args
	hr = pani->put_Period(msecPeriod);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_Thing(pThing);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_Event(bstrEvent);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_ArgumentsExt(pdispparams);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_User(pUser);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// now add to list
	m_listAnimation.AddTail(pani);

	// return
	*ppAnimation = pani;
	SAFEADDREF(*ppAnimation);

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppAnimation);

	SAFERELEASE(pUser);

	return hr;
}

STDMETHODIMP CVWAnimator::CreateUIEvent(int msecPeriod, 
									IThing *pThing, 
									BSTR bstrEvent, 
									VARIANT varArgs,
									IVWAnimation **ppAnimation)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// create an animation object
	IVWAnimation *pani;
	HRESULT hr = S_OK;
	IThing* pUser = NULL;

	if (ppAnimation == NULL)
		return E_POINTER;

	*ppAnimation = NULL;

	m_pWorld->get_User(&pUser);

	pani = new CComAnimation;
	if (!pani)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CVWAnimator::CreateUIEvent: failed to allocaate animation\n");

		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}
	pani->AddRef();

	hr = pani->Initialize(m_pWorld);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// set its period and other args
	hr = pani->put_Period(msecPeriod);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_UIEvent(bstrEvent);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_UIArguments(varArgs);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_User(pUser);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// now add to list
	m_listAnimation.AddTail(pani);

	// return
	*ppAnimation = pani;
	SAFEADDREF(*ppAnimation);

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppAnimation);

	SAFERELEASE(pUser);

	return hr;
}

STDMETHODIMP CVWAnimator::Create(	int msecPeriod, 
									IThing *pThing, 
									BSTR bstrMethod, 
									VARIANT varArgs, 
									IVWAnimation **ppAnimation)
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

		hr = ConvertSAFEARRAYToDISPPARAMS(&varArgs, &dispparams);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = CreateExt(msecPeriod, pThing, bstrMethod, &dispparams, ppAnimation);

		TerminateDISPPARAMS(VARIANT_TRUE, &dispparams);
	}
	else
		hr = CreateExt(msecPeriod, pThing, bstrMethod, NULL, ppAnimation);

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CVWAnimator::CreateExt(int msecPeriod, 
									IThing *pThing, 
									BSTR bstrMethod, 
									DISPPARAMS* pdispparams, 
									IVWAnimation **ppAnimation)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// create an animation object
	IVWAnimation *pani;
	HRESULT hr = S_OK;
	IThing* pUser = NULL;

	if (ppAnimation == NULL)
		return E_POINTER;

	*ppAnimation = NULL;

	m_pWorld->get_User(&pUser);

	pani = new CComAnimation;
	if (!pani)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CVWAnimator::CreateExt: failed to allocaate animation\n");

		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}
	pani->AddRef();

	hr = pani->Initialize(m_pWorld);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// set its period and other args
	hr = pani->put_Period(msecPeriod);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_Thing(pThing);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_Method(bstrMethod);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_ArgumentsExt(pdispparams);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_User(pUser);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// now add to list
	m_listAnimation.AddTail(pani);

	// return
	*ppAnimation = pani;
	SAFEADDREF(*ppAnimation);

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppAnimation);

	SAFERELEASE(pUser);

	return hr;
}


STDMETHODIMP CVWAnimator::CreateClosedExt(int msecPeriod, 
										  int iInvokeMax,
										IThing *pThing, 
										BSTR bstrMethod, 
										DISPPARAMS* pdispparams, 
										IVWAnimation **ppAnimation)

{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// create an animation object
	IVWAnimation *pani;
	HRESULT hr = S_OK;
	IThing* pUser = NULL;

	if (ppAnimation == NULL)
		return E_POINTER;

	*ppAnimation = NULL;

	m_pWorld->get_User(&pUser);

	pani = new CComAnimation;
	if (!pani)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CVWAnimator::CreateExt: failed to allocaate animation\n");

		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}
	pani->AddRef();

	hr = pani->Initialize(m_pWorld);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// set its period and other args
	hr = pani->put_Period(msecPeriod);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_Thing(pThing);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_Method(bstrMethod);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_ArgumentsExt(pdispparams);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_User(pUser);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_InvokeMax(iInvokeMax);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// now add to list
	m_listAnimation.AddTail(pani);

	// return
	*ppAnimation = pani;
	SAFEADDREF(*ppAnimation);

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppAnimation);

	SAFERELEASE(pUser);

	return hr;
}





STDMETHODIMP CVWAnimator::CreateSynchronizedEvent(	IVWAnimation *pAnimationBase,
												IThing *pThing, 
												BSTR bstrEvent, 
												VARIANT varArgs, 
												IVWAnimation **ppAnimation)
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

		hr = ConvertSAFEARRAYToDISPPARAMS(&varArgs, &dispparams);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = CreateSynchronizedEventExt(pAnimationBase, pThing, bstrEvent,
			&dispparams, ppAnimation);

		TerminateDISPPARAMS(VARIANT_TRUE, &dispparams);
	}
	else
		hr = CreateSynchronizedEventExt(pAnimationBase, pThing, bstrEvent,
			NULL, ppAnimation);

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CVWAnimator::CreateSynchronizedEventExt(IVWAnimation *pAnimationBase,
												IThing *pThing, 
												BSTR bstrEvent, 
												DISPPARAMS* pdispparams, 
												IVWAnimation **ppAnimation)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	IVWAnimation *pani;
	HRESULT hr = S_OK;
	IThing* pUser = NULL;

	if (ppAnimation == NULL)
		return E_POINTER;

	*ppAnimation = NULL;

	m_pWorld->get_User(&pUser);

	// Clone the given animation object
	hr = pAnimationBase->Clone(&pani);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	ASSERT(pani != NULL);

	// Change necessary args
	hr = pani->put_Thing(pThing);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_Event(bstrEvent);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_ArgumentsExt(pdispparams);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_User(pUser);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// now add to list
	m_listAnimation.AddTail(pani);

	// return
	*ppAnimation = pani;
	SAFEADDREF(*ppAnimation);

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppAnimation);

	SAFERELEASE(pUser);

	return hr;
}

STDMETHODIMP CVWAnimator::CreateSynchronizedUIEvent(IVWAnimation *pAnimationBase,
												IThing *pThing, 
												BSTR bstrEvent, 
												VARIANT varArgs,
												IVWAnimation **ppAnimation)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	IVWAnimation *pani;
	HRESULT hr = S_OK;
	IThing* pUser = NULL;

	if (ppAnimation == NULL)
		return E_POINTER;

	*ppAnimation = NULL;

	m_pWorld->get_User(&pUser);

	// Clone the given animation object
	hr = pAnimationBase->Clone(&pani);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	ASSERT(pani != NULL);

	// Change necessary args
	hr = pani->put_UIEvent(bstrEvent);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_UIArguments(varArgs);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_User(pUser);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// now add to list
	m_listAnimation.AddTail(pani);

	// return
	*ppAnimation = pani;
	SAFEADDREF(*ppAnimation);

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppAnimation);

	SAFERELEASE(pUser);

	return hr;
}

STDMETHODIMP CVWAnimator::CreateSynchronized(	IVWAnimation *pAnimationBase,
												IThing *pThing, 
												BSTR bstrMethod, 
												VARIANT varArgs, 
												IVWAnimation **ppAnimation)
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

		hr = ConvertSAFEARRAYToDISPPARAMS(&varArgs, &dispparams);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = CreateSynchronizedExt(pAnimationBase, pThing, bstrMethod,
			&dispparams, ppAnimation);

		TerminateDISPPARAMS(VARIANT_TRUE, &dispparams);
	}
	else
		hr = CreateSynchronizedExt(pAnimationBase, pThing, bstrMethod,
			NULL, ppAnimation);

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CVWAnimator::CreateSynchronizedExt(IVWAnimation *pAnimationBase,
												IThing *pThing, 
												BSTR bstrMethod, 
												DISPPARAMS* pdispparams, 
												IVWAnimation **ppAnimation)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	IVWAnimation *pani;
	HRESULT hr = S_OK;
	IThing* pUser = NULL;

	if (ppAnimation == NULL)
		return E_POINTER;

	*ppAnimation = NULL;

	m_pWorld->get_User(&pUser);

	// Clone the given animation object
	hr = pAnimationBase->Clone(&pani);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	ASSERT(pani != NULL);

	// Change necessary args
	hr = pani->put_Thing(pThing);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_Method(bstrMethod);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_ArgumentsExt(pdispparams);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_User(pUser);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// now add to list
	m_listAnimation.AddTail(pani);

	// return
	*ppAnimation = pani;
	SAFEADDREF(*ppAnimation);

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppAnimation);

	SAFERELEASE(pUser);

	return hr;
}

STDMETHODIMP CVWAnimator::CreateCallbackExt(int msecPeriod,
											IVWAnimationCallback *pacb,
											IVWAnimation **ppAnimation)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// create an animation object
	IVWAnimation *pani;
	HRESULT hr = S_OK;
	IThing* pUser = NULL;

	if (ppAnimation == NULL)
		return E_POINTER;

	*ppAnimation = NULL;

	m_pWorld->get_User(&pUser);

	pani = new CComAnimation;
	if (!pani)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CVWAnimator::CreateCallbackExt: failed to allocaate animation\n");

		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}
	pani->AddRef();

	hr = pani->Initialize(m_pWorld);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// set its period and other args
	hr = pani->put_Period(msecPeriod);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_Callback(pacb);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	hr = pani->put_User(pUser);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// now add to list
	m_listAnimation.AddTail(pani);

	// return
	*ppAnimation = pani;
	SAFEADDREF(*ppAnimation);

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppAnimation);

	SAFERELEASE(pUser);

	return hr;
}

STDMETHODIMP CVWAnimator::RemoveThingAnimations(IThing *pThing)
{
	IVWAnimation *pani;
	POSITION pos;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// iterate through list, looking for items associated with Thing
	for (pos = m_listAnimation.GetHeadPosition(); pos != NULL;)
	{
		POSITION posOld;

		posOld = pos;
		pani = m_listAnimation.GetNext(pos);
		ASSERT(pani != NULL);

		// get thing
		IThing *pThingAni;
		/* hr = */ pani->get_Thing(&pThingAni);

		// see if its the one we want
		if (pThing == pThingAni)
		{
			// remove from list
			m_listAnimation.RemoveAt(posOld);

			pani->Release();
		}

		SAFERELEASE(pThingAni);
	}

	return S_OK;
}


STDMETHODIMP CVWAnimator::Remove(IVWAnimation *pAnimation)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (pAnimation == NULL)
		return E_INVALIDARG;

	// look through list, then release when found
	IVWAnimation *pani;
	POSITION pos;

	for (pos = m_listAnimation.GetHeadPosition(); pos != NULL; )
	{
		POSITION posOld;

		posOld = pos;
		pani = m_listAnimation.GetNext(pos);
		ASSERT(pani != NULL);

		// release 
		if (pani == pAnimation)
		{
			// remove from list
			m_listAnimation.RemoveAt(posOld);
			
			pani->Release();

			// done
			return S_OK;
		}

	}

	VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CVWAnimator::Remove: animation not found\n");

	return VWANIM_E_ANIMATIONNOTEXIST;
}

STDMETHODIMP CVWAnimator::RemoveAll()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// release all items in list
	IVWAnimation *pani;
	POSITION pos;

	// iterate through list, looking for items whose time as expired
	for (pos=m_listAnimation.GetHeadPosition(); pos!=NULL;)
	{
		pani = m_listAnimation.GetNext(pos);
		ASSERT(pani != NULL);

		pani->Release();
	}

	// now clear the list
	m_listAnimation.RemoveAll();

	return S_OK;
}

STDMETHODIMP CVWAnimator::get_CurrentAnimation(IVWAnimation **ppAnimation)
{
	if (ppAnimation==NULL)
		return E_POINTER;

	*ppAnimation = m_pAnimCurrent;
	SAFEADDREF(*ppAnimation);

	return S_OK;
}

void CVWAnimator::TimerHandler(UINT idTimer)
{
	IVWAnimation *pani;
	AnimInvokeNode* pAnimInvokeList = NULL;
	POSITION pos;
	int msecNow;
	HRESULT hr = S_OK;

	// if not our timer, ignore
	if (pTheAnimator == NULL || pTheAnimator->m_idTimer != idTimer)
		return;

	msecNow = (int) ::GetTickCount();

#ifdef ANIMATOR_PERF_DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_NONE, "CVWAnimator::TimerHandler: (woke up) time %d, id %d\n", msecNow, idTimer);

//	VWREPORT(m_pWorld, VWR_PERFLOG, "Animator (woke up),%d,%d\n", msecNow, idTimer);
#endif

	// iterate through list, looking for items whose time as expired
	for (pos = pTheAnimator->m_listAnimation.GetHeadPosition(); pos != NULL;)
	{
		POSITION posOld;

		posOld = pos;
		pani = pTheAnimator->m_listAnimation.GetNext(pos);
		ASSERT(pani != NULL);
		SAFEADDREF(pani);

		// get time of last invoke and period
		int msecPeriod;
		int msecLastTime;
		int iInvokeMax;
		int iTimesInvoked;

		hr = pani->get_Period(&msecPeriod);
		if (FAILED(hr))
			goto NEXT_ANIMATION;

		

		if (msecPeriod == 0)
		{
			// This is a single shot animation, just invoke it and delete it.
			m_pAnimCurrent = pani;
//			SAFEADDREF(m_pAnimCurrent);
//			hr = pani->Invoke(msecNow);
//			SAFERELEASE(m_pAnimCurrent);

			pAnimInvokeList = new AnimInvokeNode(pani, msecNow, pAnimInvokeList);

			m_listAnimation.RemoveAt(posOld);

			pani->Release();

			goto NEXT_ANIMATION;
		}

		hr = pani->get_TimeOfLastInvocation(&msecLastTime);
		if (FAILED(hr))
			goto NEXT_ANIMATION;

		// see if time to go
		if ((msecNow - msecLastTime) > msecPeriod)
		{
		// yep, but check for closed animations
		
			//ok, were an open animation, so just invoke away
			m_pAnimCurrent = pani;
//
//			SAFEADDREF(m_pAnimCurrent);
//			hr = pani->Invoke(msecNow);
//			SAFERELEASE(m_pAnimCurrent);

			pAnimInvokeList = new AnimInvokeNode(pani, msecNow, pAnimInvokeList);

			//check if its a closed animation (invokemax > 0)
			hr = pani->get_InvokeMax(&iInvokeMax);
			if (FAILED(hr))
			goto NEXT_ANIMATION;

			if (iInvokeMax > 0) 
			{
				hr = pani->get_TimesInvoked(&iTimesInvoked);
				if (FAILED(hr))
					goto NEXT_ANIMATION;
				//we are supposed to kill ourselves off after invokeMax times
				if (iTimesInvoked >= iInvokeMax-1)
				{
//					SAFERELEASE(m_pAnimCurrent);
					m_listAnimation.RemoveAt(posOld);
					pani->Release();
					goto NEXT_ANIMATION;
				}
			}

#ifdef ANIMATOR_PERF_DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_NONE, "CVWAnimator::TimerHandler: (fire) time %d, id %d, delta %d, period %d\n", msecNow, idTimer, msecNow - msecLastTime, msecPeriod);

//			VWREPORT(m_pWorld, VWR_PERFLOG, "Animator (fire),%d,%d,%d,%d\n", msecNow, idTimer, msecNow - msecLastTime, msecPeriod);
#endif




		}

NEXT_ANIMATION:
		SAFERELEASE(pani);
	}

	while (pAnimInvokeList)
	{
		AnimInvokeNode* pAnimNode = pAnimInvokeList;

		hr = pAnimNode->m_pAnimation->Invoke(pAnimNode->m_nSecNow);
		if (FAILED(hr))
		{
			ASSERT(hr);
		}
		
		pAnimInvokeList = pAnimInvokeList->m_pNext;
		delete pAnimNode;
	}
}

