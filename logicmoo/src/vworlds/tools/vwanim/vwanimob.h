// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWAnimOb.h : Declaration of the CVWAnim

#include <resource.h>
#include <afxtempl.h>
#include <propbase.h>

EXTERN_C const CLSID CLSID_VWAnimator;

class CVWAnimator;

class CAnimDummyWnd : public CWnd
{
public:
	CAnimDummyWnd(CVWAnimator* pvw) : m_pAnim(pvw) { };
	~CAnimDummyWnd() {m_pAnim = NULL;};

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	CVWAnimator* m_pAnim;
};

/////////////////////////////////////////////////////////////////////////////
// VWAnim

class ATL_NO_VTABLE CVWAnimator :
	public IDispatchImpl<IVWAnimator, &IID_IVWAnimator, &LIBID_VWSYSTEMLib>,
	public ISupportErrorInfo,
	public IObjectSafetyImpl<CVWAnimator, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVWAnimator, &CLSID_VWAnimator>
{
DECLARE_AGGREGATABLE(CVWAnimator)

//Give Window access to timerhandler. Nobody else.
friend CAnimDummyWnd;

public:
	CVWAnimator();
	~CVWAnimator();

	HRESULT FinalConstruct(void);

BEGIN_COM_MAP(CVWAnimator)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWAnimator)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

//DECLARE_NOT_AGGREGATABLE(CVWAnim) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

DECLARE_PROTECT_FINAL_CONSTRUCT()

DECLARE_REGISTRY(CVWAnimator, _T("VWSYSTEM.Animator.1"), _T("VWSYSTEM.Animator"), IDS_VWANIMATOR_DESC, THREADFLAGS_BOTH)

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IVWAnimator
public:
	STDMETHOD(Initialize)(IWorld *pWorld);
	STDMETHOD(Terminate)();

	STDMETHOD(CreateEventExt)(int msecPeriod, 
						IThing *pThing, 
						BSTR bstrEvent, 
						DISPPARAMS* pdispparams, 
						IVWAnimation **ppAnimation);
	STDMETHOD(CreateEvent)(int msecPeriod, 
						IThing *pThing, 
						BSTR bstrEvent, 
						VARIANT varArgs, 
						IVWAnimation **ppAnimation);

	STDMETHOD(CreateUIEvent)(int msecPeriod, 
						IThing *pThing, 
						BSTR bstrEvent, 
						VARIANT varArgs, 
						IVWAnimation **ppAnimation);

	STDMETHOD(CreateExt)(int msecPeriod, 
						IThing *pThing, 
						BSTR bstrMethod, 
						DISPPARAMS* pdispparams, 
						IVWAnimation **ppAnimation);
	STDMETHOD(CreateClosedExt)(int msecPeriod, 
						int iInvokeMax,
						IThing *pThing, 
						BSTR bstrMethod, 
						DISPPARAMS* pdispparams, 
						IVWAnimation **ppAnimation);
	STDMETHOD(Create)(int msecPeriod, 
						IThing *pThing, 
						BSTR bstrMethod, 
						VARIANT varArgs, 
						IVWAnimation **ppAnimation);

	STDMETHOD(CreateSynchronizedEventExt)(IVWAnimation *pAnimation,
									IThing *pThing, 
									BSTR bstrEvent, 
									DISPPARAMS* pdispparams, 
									IVWAnimation **ppAnimation);
	STDMETHOD(CreateSynchronizedEvent)(IVWAnimation *pAnimation,
									IThing *pThing, 
									BSTR bstrEvent, 
									VARIANT varArgs, 
									IVWAnimation **ppAnimation);

	STDMETHOD(CreateSynchronizedUIEvent)(IVWAnimation *pAnimation,
									IThing *pThing, 
									BSTR bstrEvent, 
									VARIANT varArgs, 
									IVWAnimation **ppAnimation);

	STDMETHOD(CreateSynchronizedExt)(IVWAnimation *pAnimation,
									IThing *pThing, 
									BSTR bstrMethod, 
									DISPPARAMS* pdispparams, 
									IVWAnimation **ppAnimation);
	STDMETHOD(CreateSynchronized)(IVWAnimation *pAnimation,
									IThing *pThing, 
									BSTR bstrMethod, 
									VARIANT varArgs, 
									IVWAnimation **ppAnimation);
	STDMETHOD(CreateCallbackExt)(int msecPeriod,
								 IVWAnimationCallback *pacb,
								 IVWAnimation **ppAnimation);

	STDMETHOD(Remove)(IVWAnimation *pAnimation);
	STDMETHOD(RemoveThingAnimations)(IThing *pThing);
	STDMETHOD(RemoveAll)();

	STDMETHOD(get_CurrentAnimation)(IVWAnimation **ppAnimation);

protected:
	
	//handler for a timer msg called by dummywnd.
	void TimerHandler(UINT idTimer);
	
	//wnd for posting timer msgs.
	CAnimDummyWnd*	m_pDummyWnd;

	// back pointer
	IWorld* m_pWorld;

	// timer id
	UINT m_idTimer;

	// pointer to current animation
	IVWAnimation* m_pAnimCurrent;

	// list of animation objects
	CList<IVWAnimation*, IVWAnimation*&> m_listAnimation;

	CVWAnimator* pTheAnimator;	

	static UINT	m_TimerOffer;
};

//Animation invokation list.  This list is queued during the main event loop and invoked immediately after.
//This code was introduced to fix bug 6274.
class AnimInvokeNode
{
public:
	AnimInvokeNode(IVWAnimation* pAnim, int nSecNow, AnimInvokeNode* pHead )
	{
		m_pAnimation = pAnim;
		SAFEADDREF(m_pAnimation);

		m_pNext = pHead;
		m_nSecNow = nSecNow;
	};
	
	~AnimInvokeNode() 
	{
		SAFERELEASE(m_pAnimation);
	}

public:
	IVWAnimation* m_pAnimation;
	AnimInvokeNode* m_pNext;
	int m_nSecNow;

};
