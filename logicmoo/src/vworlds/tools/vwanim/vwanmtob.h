// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWAnmtOb.h : Declaration of the CVWAnimation

#include <resource.h>
#include <propbase.h>

EXTERN_C const CLSID CLSID_VWAnimation;

/////////////////////////////////////////////////////////////////////////////
// VWAnim

class ATL_NO_VTABLE CVWAnimation :
	public IDispatchImpl<IVWAnimation, &IID_IVWAnimation, &LIBID_VWSYSTEMLib>,
	public ISupportErrorInfo,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVWAnimation, &CLSID_VWAnimation>
{
DECLARE_AGGREGATABLE(CVWAnimation)

public:
	CVWAnimation();
	~CVWAnimation();

BEGIN_COM_MAP(CVWAnimation)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWAnimation)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

//DECLARE_NOT_AGGREGATABLE(CVWAnim) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

DECLARE_REGISTRY(CVWAnimation, _T("VWSYSTEM.Animation.1"), _T("VWSYSTEM.Animation"), IDS_VWANIMATION_DESC, THREADFLAGS_BOTH)

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IVWAnimation
public:
	STDMETHOD(Initialize)(IWorld* pWorld);
	STDMETHOD(Terminate)();

	STDMETHOD(Invoke)(int msecNow);
	STDMETHOD(Clone)(IVWAnimation **ppAnimation);

	STDMETHOD(get_TimeOfLastInvocation)(int *pmsec);
	STDMETHOD(get_TimeOfCreation)(int *pmsec);
	STDMETHOD(get_CurrentTime)(int *pmsec);
	STDMETHOD(get_TimesInvoked)(int *pcInvoked);
	STDMETHOD(put_Period)(int msecPeriod);
	STDMETHOD(get_Period)(int *pmsecPeriod);
	STDMETHOD(put_InvokeMax)(int iInvokeMax);
	STDMETHOD(get_InvokeMax)(int *iInvokeMax);
	STDMETHOD(put_Thing)(IThing *pthing);
	STDMETHOD(get_Thing)(IThing **ppthing);
	STDMETHOD(put_User)(IThing *pthing);
	STDMETHOD(get_User)(IThing **ppthing);
	STDMETHOD(put_Method)(BSTR bstrMethod);
	STDMETHOD(get_Method)(BSTR *pbstrMethod);
	STDMETHOD(put_Event)(BSTR bstrEvent);
	STDMETHOD(get_Event)(BSTR *pbstrEvent);
	STDMETHOD(put_UIEvent)(BSTR bstrEvent);
	STDMETHOD(get_UIEvent)(BSTR *pbstrEvent);
	STDMETHOD(put_Callback)(IVWAnimationCallback *ppacb);
	STDMETHOD(get_Callback)(IVWAnimationCallback **ppacb);

	STDMETHOD(put_ArgumentsExt)(DISPPARAMS* pdispparams);
	STDMETHOD(get_ArgumentsExt)(DISPPARAMS** ppdispparams);

	STDMETHOD(put_UIArguments)(VARIANT varArgs)
	{
		m_varArgs = varArgs;

		return S_OK;
	}

protected:
	IWorld* m_pWorld;

	// thing associated with animation
	IThing* m_pthing;

	// user associated with animation
	IThing* m_pUser;

	// type of animation
	enum
	{
		eNotSet,
		eMethod,
		eCallback,
		eEvent,
		eUIEvent
	} m_eAnimType;

	// method to call...
	CComBSTR m_bstrMethod;
	CComBSTR m_bstrEvent;

	// ...or, callback function
	IVWAnimationCallback* m_pacb;

	// argument list
	DISPPARAMS m_dispparams;

	// argument list for UI events
	CComVariant m_varArgs;

	// period
	int m_msecPeriod;

	// time of creation
	int m_msecCreated;

	// time of last invoke
	int m_msecLastTimeInvoked;

	// current time (during invocation)
	int m_msecCurrent;

	// count of times invoked
	int m_cInvoked;

	// count of number of times to invoke
	int m_iInvokeMax;
};
