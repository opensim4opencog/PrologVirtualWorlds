// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include <vwobject.h>
#include <propbase.h>
#include "syshelp.h"
#include <vwevents.h>

// NOTE: can use faster CComObject construction if building VWSYSTEM
// otherwise, use CoCreateInstance by CLSID

#ifdef VWSYSTEM_BUILD
#include "plistobj.h"
#include "pmapobj.h"
#include "cmenuitm.h"
#include "vwocx.h"
#include "marshbuf.h"

typedef CComObject<CPropertyListObject> CComPropertyList;
typedef CComObject<CPropertyMapObject> CComPropertyMap;
typedef CComObject<CMenuItemObject> CComMenuItem;
typedef CComObject<COCXProxyObject> CComOCXProxy;
typedef CComObject<CMarshallBufferObject> CComMarshallBuffer;
typedef CComObject<CUnMarshallBufferObject> CComUnMarshallBuffer;
#else
EXTERN_C const CLSID CLSID_PropertyList;
EXTERN_C const CLSID CLSID_PropertyMap;
EXTERN_C const CLSID CLSID_MenuItem;
EXTERN_C const CLSID CLSID_OCXProxy;
#endif

#ifdef VWSYSTEM_BUILD
HRESULT CreateMarshallBuffer(IMarshallBuffer** ppMarshBuf)
{
	HRESULT hr = S_OK;

	// create marshall buffer
	(*ppMarshBuf) = new CComMarshallBuffer();
	if ((*ppMarshBuf) == NULL)
	{
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	(*ppMarshBuf)->AddRef();

ERROR_ENCOUNTERED:
	return hr;
}

HRESULT CreateUnMarshallBuffer(IUnMarshallBuffer** ppMarshBuf)
{
	HRESULT hr = S_OK;

	// create unmarshall buffer
	(*ppMarshBuf) = new CComUnMarshallBuffer();
	if ((*ppMarshBuf) == NULL)
	{
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	(*ppMarshBuf)->AddRef();

ERROR_ENCOUNTERED:
	return hr;
}

HRESULT CreatePropertyList(IWorld* pWorld, IPropertyList** pppropertylist)
{
	HRESULT hr = S_OK;

	if (pWorld == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreatePropertyList: incoming argument (IWorld*) is NULL\n");
		return E_INVALIDARG;
	}

	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "CreatePropertyList\n");

	ASSERT(pppropertylist);
	if (pppropertylist == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreatePropertyList: outgoing pointer (IPropertyList**) is NULL\n");
		return E_POINTER;
	}

	*pppropertylist = new CComPropertyList;

	if (*pppropertylist == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreatePropertyList: failed to allocate CComPropertyList\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(*pppropertylist);

	(*pppropertylist)->AddRef();

	hr = (*pppropertylist)->put_World(pWorld);

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*pppropertylist);
	return hr;
}

HRESULT CreatePropertyMap(IWorld* pWorld, IPropertyMap** pppropertymap)
{
	HRESULT hr = S_OK;

	if (pWorld == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreatePropertyMap: incoming argument (IWorld*) is NULL\n");
		return E_INVALIDARG;
	}

	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "CreatePropertyMap\n");

	ASSERT(pppropertymap);
	if (pppropertymap == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreatePropertyMap: outgoing pointer (IPropertyMap**) is NULL\n");
		return E_POINTER;
	}

	*pppropertymap = new CComPropertyMap;

	if (*pppropertymap == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreatePropertyMap: failed to allocate CComPropertyMap\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(*pppropertymap);

	(*pppropertymap)->AddRef();

	hr = (*pppropertymap)->put_World(pWorld);

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*pppropertymap);
	return hr;
}

HRESULT CreateMenu(IWorld* pWorld, IPropertyList** ppmenu)
{
	if (pWorld == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateMenu: incoming argument (IWorld*) is NULL\n");
		return E_INVALIDARG;
	}

	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "CreateMenu\n");

	HRESULT hr = S_OK;
	static CComBSTR bstrMenu("Menu");

	hr = ::CreatePropertyList(pWorld, ppmenu);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmenu)->put_Type(bstrMenu);

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppmenu);
	return hr;
}

HRESULT CreateMenuItem(IWorld* pWorld, BSTR bstrMenuText, long lFlags, IThing* pOwner, BSTR bstrUpdateMethod, BSTR bstrMethod, VARIANT varArgs, IMenuItem** ppMenuItem)
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

		hr = ConvertSAFEARRAYToDISPPARAMS(&varTemp, &dispparams);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = CreateMenuItemExt(pWorld, bstrMenuText, lFlags, pOwner, bstrUpdateMethod, bstrMethod, &dispparams, ppMenuItem);

		TerminateDISPPARAMS(VARIANT_TRUE, &dispparams);
	}
	else
		hr = CreateMenuItemExt(pWorld, bstrMenuText, lFlags, pOwner, bstrUpdateMethod, bstrMethod, NULL, ppMenuItem);

ERROR_ENCOUNTERED:
	return hr;
}

HRESULT CreateMenuItemExt(IWorld* pWorld, BSTR bstrMenuText, long lFlags, IThing* pOwner, BSTR bstrUpdateMethod, BSTR bstrMethod, DISPPARAMS* params, IMenuItem** ppmenuitem)
{
	HRESULT	hr = S_OK;

	if (pWorld == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateMenuItemExt: incoming argument (IWorld*) is NULL\n");
		return E_INVALIDARG;
	}

	ASSERT(ppmenuitem);
	if (ppmenuitem == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateMenuItemExt: outgoing pointer (IObjectProperty**) is NULL\n");
		return E_POINTER;
	}

	*ppmenuitem = new CComMenuItem;

	if (*ppmenuitem == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateMenuItemExt: failed to allocate CComMenuItem\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(*ppmenuitem);

	(*ppmenuitem)->AddRef();

	hr = (*ppmenuitem)->put_World(pWorld);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmenuitem)->put_MenuText(bstrMenuText);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmenuitem)->put_Method(bstrMethod);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmenuitem)->put_ArgumentsExt(params);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmenuitem)->put_OnUpdateMethod(bstrUpdateMethod);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmenuitem)->put_Flags(lFlags);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmenuitem)->put_Owner(pOwner);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppmenuitem);
	return hr;
}

HRESULT CreateMenuSeparator(IWorld* pWorld, IThing* pOwner, IMenuItem** ppmenuitem)
{
	return ::CreateMenuItemExt(pWorld, NULL, MI_SEPARATOR, pOwner, NULL, NULL, NULL, ppmenuitem); 
}

HRESULT CreateOCXProxy(IWorld* pWorld, IDispatch *pOCX, IOCXProxy** ppOCXProxy)
{
	HRESULT hr = S_OK;

	if (pWorld == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateOCXProxy: incoming argument (IWorld*) is NULL\n");
		return E_INVALIDARG;
	}

	ASSERT(ppOCXProxy);
	if (ppOCXProxy == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateOCXProxy: outgoing pointer (IObjectProperty**) is NULL\n");
		return E_POINTER;
	}

	*ppOCXProxy = new CComOCXProxy;

	if (*ppOCXProxy == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateOCXProxy: failed to allocate CComOCXProxy\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(*ppOCXProxy);

	(*ppOCXProxy)->AddRef();

	hr = (*ppOCXProxy)->put_World(pWorld);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppOCXProxy)->put_OCX(pOCX);
   	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppOCXProxy);
	return hr;
}
#else
HRESULT CreatePropertyList(IWorld* pWorld, IPropertyList** pppropertylist)
{
	HRESULT hr = S_OK;

	if (pWorld == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreatePropertyList: incoming argument (IWorld*) is NULL\n");
		return E_INVALIDARG;
	}

	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "CreatePropertyList\n");

	ASSERT(pppropertylist);
	if (pppropertylist == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreatePropertyList: outgoing pointer (IPropertyList**) is NULL\n");
		return E_POINTER;
	}

	*pppropertylist = NULL;

	hr = pWorld->CreateObjectPropertyExt(CLSID_PropertyList, NULL, (IObjectProperty**)pppropertylist);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (*pppropertylist == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreatePropertyList: failed to create object property\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*pppropertylist);
	return hr;
}

HRESULT CreatePropertyMap(IWorld* pWorld, IPropertyMap** pppropertymap)
{
	HRESULT hr = S_OK;

	if (pWorld == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreatePropertyMap: incoming argument (IWorld*) is NULL\n");
		return E_INVALIDARG;
	}

	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "CreatePropertyMap\n");

	ASSERT(pppropertymap);
	if (pppropertymap == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreatePropertyMap: outgoing pointer (IPropertyMap**) is NULL\n");
		return E_POINTER;
	}

	*pppropertymap = NULL;

	hr = pWorld->CreateObjectPropertyExt(CLSID_PropertyMap, NULL, (IObjectProperty**)pppropertymap);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (*pppropertymap == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreatePropertyMap: failed to create object property\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*pppropertymap);
	return hr;
}

HRESULT CreateMenu(IWorld* pWorld, IPropertyList** ppmenu)
{
	if (pWorld == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateMenu: incoming argument (IWorld*) is NULL\n");
		return E_INVALIDARG;
	}

	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "CreateMenu\n");

	HRESULT hr = S_OK;
	static CComBSTR bstrMenu("Menu");

	hr = ::CreatePropertyList(pWorld, ppmenu);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmenu)->put_Type(bstrMenu);

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppmenu);
	return hr;
}

HRESULT CreateMenuItem(IWorld* pWorld, BSTR bstrMenuText, long lFlags, IThing* pOwner, BSTR bstrUpdateMethod, BSTR bstrMethod, VARIANT varArgs, IMenuItem** ppMenuItem)
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

		hr = ConvertSAFEARRAYToDISPPARAMS(&varTemp, &dispparams);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = CreateMenuItemExt(pWorld, bstrMenuText, lFlags, pOwner, bstrUpdateMethod, bstrMethod, &dispparams, ppMenuItem);

		TerminateDISPPARAMS(VARIANT_TRUE, &dispparams);
	}
	else
		hr = CreateMenuItemExt(pWorld, bstrMenuText, lFlags, pOwner, bstrUpdateMethod, bstrMethod, NULL, ppMenuItem);

ERROR_ENCOUNTERED:
	return hr;
}

HRESULT CreateMenuItemExt(IWorld* pWorld, BSTR bstrMenuText, long lFlags, IThing* pOwner, BSTR bstrUpdateMethod, BSTR bstrMethod, DISPPARAMS* params, IMenuItem** ppmenuitem)
{
	HRESULT	hr = S_OK;

	if (pWorld == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateMenuItemExt: incoming argument (IWorld*) is NULL\n");
		return E_INVALIDARG;
	}

	ASSERT(ppmenuitem);
	if (ppmenuitem == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateMenuItemExt: outgoing pointer (IObjectProperty**) is NULL\n");
		return E_POINTER;
	}

	*ppmenuitem = NULL;

	hr = pWorld->CreateObjectPropertyExt(CLSID_MenuItem, NULL, (IObjectProperty**)ppmenuitem);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (*ppmenuitem == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateMenuItemExt: failed to create object property\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	hr = (*ppmenuitem)->put_MenuText(bstrMenuText);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmenuitem)->put_Method(bstrMethod);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmenuitem)->put_ArgumentsExt(params);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmenuitem)->put_OnUpdateMethod(bstrUpdateMethod);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmenuitem)->put_Flags(lFlags);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppmenuitem)->put_Owner(pOwner);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppmenuitem);
	return hr;
}

HRESULT CreateMenuSeparator(IWorld* pWorld, IThing* pOwner, IMenuItem** ppmenuitem)
{
	return ::CreateMenuItemExt(pWorld, NULL, MI_SEPARATOR, pOwner, NULL, NULL, NULL, ppmenuitem); 
}

HRESULT CreateOCXProxy(IWorld* pWorld, IDispatch *pOCX, IOCXProxy** ppOCXProxy)
{
	HRESULT hr = S_OK;

	if (pWorld == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateOCXProxy: incoming argument (IWorld*) is NULL\n");
		return E_INVALIDARG;
	}

	ASSERT(ppOCXProxy);
	if (ppOCXProxy == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateOCXProxy: outgoing pointer (IObjectProperty**) is NULL\n");
		return E_POINTER;
	}

	*ppOCXProxy = NULL;

	hr = pWorld->CreateObjectPropertyExt(CLSID_OCXProxy, NULL, (IObjectProperty**)ppOCXProxy);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (*ppOCXProxy == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateOCXProxy: failed to create object property\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	hr = (*ppOCXProxy)->put_OCX(pOCX);
   	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppOCXProxy);
	return hr;
}
#endif


HRESULT ShowDialog(IWorld* pWorld, VARIANT_BOOL bIsModal, BSTR bstrURL, IDispatch* pArguments, BSTR bstrWindowFeatures)
{
	HRESULT hr;
	CComPtr<IPropertyList> pPropList;

	hr = CreatePropertyList(pWorld, &pPropList);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pPropList->AddBOOL(bIsModal);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pPropList->AddString(bstrURL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pPropList->AddDispatch(pArguments);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;


	hr = pPropList->AddString(bstrWindowFeatures);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;


	hr = pWorld->FireUIEvent(NULL, CComBSTR(VW_SHOWDIALOG_STR), CComVariant(pPropList));

ERROR_ENCOUNTERED:
	return hr;
}
