// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// MenuItem.h : Declaration of the CMenuItemObject
#include <resource.h>
#include <PropBase.h>
#include <objimpl.h>
#include "syshelp.h"

#if 0
// force debugging of IMenuItem creation/destruction
#undef TRACE_ALLOCATION

#define TRACE_ALLOCATION VWT_NONE
//#define TRACE_ALLOCATION VWT_METHOD
#endif

EXTERN_C const CLSID CLSID_MenuItem;

/////////////////////////////////////////////////////////////////////////////
// MenuItem

DECLARE_OBJECTPROPERTY(MenuItem, LIBID_VWSYSTEMLib, IDS_MENUITEM_TYPE)
{
DECLARE_AGGREGATABLE(CMenuItemObject)

public:
BEGIN_COM_MAP(CMenuItemObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectProperty)
	COM_INTERFACE_ENTRY(IMenuItem)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

//DECLARE_NOT_AGGREGATABLE(CMenuItemObject) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

DECLARE_REGISTRY(CMenuItemObject, _T("VWSYSTEM.MenuItem.1"), _T("VWSYSTEM.MenuItem"), IDS_MENUITEM_DESC, THREADFLAGS_BOTH)

// ISupportsErrorInfo

// IMenuItem
public:
	// Override the base class
	STDMETHOD(get_RefCount)(DWORD *pdwRef);
	STDMETHOD(ToAscii)(BSTR* pbstr); 
	STDMETHOD(Marshall)(DWORD dwOptions, IMarshallBuffer* pbuffer); 
	STDMETHOD(UnMarshall)(IUnMarshallBuffer* pbuffer); 
	STDMETHOD(MarshallToScript)(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut);

	CMenuItemObject() : m_Flags(0), m_pSubMenu(NULL),
						m_pOwner(NULL), m_pTarget(NULL),
						m_boolAddTargetParameter(VARIANT_FALSE),
						m_boolIsOverlayItem(VARIANT_FALSE),
						m_boolIsProximitySensitive(VARIANT_FALSE)
	{ 
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CMenuItemObject::CMenuItemObject, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif

		InitializeDISPPARAMS(&m_dispparams, 0);

		// REVIEW: redundant with CComVariant constructor
		m_var.vt = VT_EMPTY;
	};

	~CMenuItemObject() 
	{	
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CMenuItemObject::~CMenuItemObject, this 0x%0x\n", this);
#endif

		Terminate();
	};

	STDMETHOD(Terminate)(void)
	{	
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CMenuItemObject::Terminate, this 0x%0x\n", this);
#endif

		CMenuItemObjectBase::Terminate();

		if (m_pSubMenu)
		{
			m_pSubMenu->RemoveAll();

			SAFERELEASE(m_pSubMenu);
		}

		TerminateDISPPARAMS(VARIANT_TRUE, &m_dispparams);

		SAFERELEASE(m_pOwner);
		SAFERELEASE(m_pTarget);

		return S_OK;
	};

	STDMETHOD(get_MenuText)(BSTR* pbstrMenuText);
	STDMETHOD(put_MenuText)(BSTR bstrMenuText);

	STDMETHOD(get_Flags)(long * nFlags);
	STDMETHOD(put_Flags)(long nFlags);

	STDMETHOD(get_UserData)(VARIANT* pvar);
	STDMETHOD(put_UserData)(VARIANT var);

	STDMETHOD(get_SubMenu)(IPropertyList** pvar);
	STDMETHOD(put_SubMenu)(IPropertyList* var);

	STDMETHOD(get_StatusText)(BSTR* pbstrStatusText);
	STDMETHOD(put_StatusText)(BSTR bstrStatusText);

	STDMETHOD(get_Method)(BSTR* pbstrMethod);
	STDMETHOD(put_Method)(BSTR bstrMethod);

	STDMETHOD(get_OnUpdateMethod)(BSTR* pbstrMethod);
	STDMETHOD(put_OnUpdateMethod)(BSTR bstrMethod);

	STDMETHOD(get_Arguments)(VARIANT* pvarArgs)
	{
		HRESULT hr = S_OK;
		IPropertyList *pList = NULL;

		if (pvarArgs==NULL)
			return E_POINTER;

		// initialize
		::VariantInit(pvarArgs);

		// Create an empty list
		hr = CreatePropertyList(m_pWorld, &pList);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = ConvertDISPPARAMSToPropertyList(&m_dispparams, pList);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// return in variant
		pvarArgs->vt = VT_DISPATCH;
		pvarArgs->pdispVal = pList;
		pList->AddRef();

	ERROR_ENCOUNTERED:

		SAFERELEASE(pList);

		return hr;
	};

	STDMETHOD(put_Arguments)(VARIANT varArgs)
	{
		HRESULT hr = S_OK;
		DISPPARAMS dispparams;

		hr = ConvertSAFEARRAYToDISPPARAMS(&varArgs, &dispparams);

		if (SUCCEEDED(hr))
			hr = put_ArgumentsExt(&dispparams);

		TerminateDISPPARAMS(VARIANT_TRUE, &dispparams);

		return hr;
	};

	STDMETHOD(get_ArgumentsExt)(DISPPARAMS** ppdispparams);
	STDMETHOD(put_ArgumentsExt)(DISPPARAMS* pdispparams);

	STDMETHOD(put_ArgumentsInt)(IPropertyList* pproplist)
	{
		HRESULT hr = S_OK;
		DISPPARAMS dispparams;

		hr = ConvertPropertyListToDISPPARAMS(pproplist, &dispparams);

		if (SUCCEEDED(hr))
			hr = put_ArgumentsExt(&dispparams);

		TerminateDISPPARAMS(VARIANT_TRUE, &dispparams);

		return hr;
	};

	STDMETHOD(get_Owner)(IThing ** ppIThing);
	STDMETHOD(put_Owner)(IThing * pIThing);
	
	STDMETHOD(get_Target)(IThing ** ppIThing);
	STDMETHOD(put_Target)(IThing * pIThing);

	STDMETHOD(get_AddTargetParameter)(VARIANT_BOOL *pboolAddTargetParameter);
	STDMETHOD(put_AddTargetParameter)(VARIANT_BOOL boolAddTargetParameter);

	STDMETHOD(get_IsOverlayItem)(VARIANT_BOOL *pboolIsOverlayItem);
	STDMETHOD(put_IsOverlayItem)(VARIANT_BOOL boolIsOverlayItem);

	STDMETHOD(get_IsProximitySensitive)(VARIANT_BOOL *pboolIsProximitySensitive);
	STDMETHOD(put_IsProximitySensitive)(VARIANT_BOOL boolIsProximitySensitive);

	STDMETHOD(Execute)();
	STDMETHOD(OnUpdate)();

	STDMETHOD(Duplicate)(IMenuItem **ppIMenuItem);

private:
	long m_Flags;							// Menu item flags

	CComVariant m_var;						// User data

	IPropertyList* m_pSubMenu;				// Sub-menu list

	CComBSTR m_bstrMenuText;				// Text to be displayed in pop-up menu
	CComBSTR m_bstrStatusText;				// Status text (used???)
	CComBSTR m_bstrMethod;					// Method to call when invoked
	CComBSTR m_bstrOnUpdateMethod;			// Method to call to update

	IThing * m_pOwner;						// Who do you call
	IThing * m_pTarget;						// Target of the operation (or NULL if same as
											// m_pOwner)

	VARIANT_BOOL m_boolAddTargetParameter;	// True if target should be added to parameters
	VARIANT_BOOL m_boolIsOverlayItem;		// True if should appear in overlay plane
	VARIANT_BOOL m_boolIsProximitySensitive;// True if should only appear when object is close

	DISPPARAMS m_dispparams;				// Arguments to invoked method

};
