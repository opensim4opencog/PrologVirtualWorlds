// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// MenuItem.cpp : Implementation of CMenuItemObjectApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include <menuitem.h>
#include "cmenuitm.h"

IMPLEMENT_OBJECTPROPERTY(CMenuItemObject)

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMenuItemObject::ToAscii(BSTR* pbstr)
{
	ASSERT(pbstr);

	CString strValue("Menu Text: " + CString(m_bstrMenuText) + ", Method: " + CString(m_bstrMethod));
	*pbstr = strValue.AllocSysString();

	return S_OK;
}

STDMETHODIMP CMenuItemObject::Marshall(DWORD dwOptions, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = CMenuItemObjectBase::Marshall(dwOptions, pbuffer)))
		return hr;
		
	ASSERT(pbuffer);
	ASSERT(m_pWorld);
	
	hr = pbuffer->put_Long(m_Flags);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->put_String(m_bstrMenuText);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->put_String(m_bstrStatusText);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->put_String(m_bstrMethod);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->put_String(m_bstrOnUpdateMethod);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// REVIEW: used to pass MARSHALL_BYVALUE
	hr = m_pWorld->MarshallProperty(m_var, 
		MARSHALLREF(dwOptions, MARSHALL_BYREF), pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// REVIEW: used to pass dwOptions
	hr = m_pWorld->MarshallObjectProperty(m_pSubMenu, clsidPropertyList, 
		MARSHALLREF(dwOptions, MARSHALL_BYREF), pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pWorld->MarshallDISPPARAMS(&m_dispparams, pbuffer, 
		MARSHALLREF(dwOptions, MARSHALL_BYREF));
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pWorld->MarshallObjectProperty(m_pOwner, clsidThing, 
		MARSHALLREF(dwOptions, MARSHALL_BYREF), pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pWorld->MarshallObjectProperty(m_pTarget, clsidThing, 
		MARSHALLREF(dwOptions,MARSHALL_BYREF), pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->put_Boolean(m_boolAddTargetParameter);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->put_Boolean(m_boolIsOverlayItem);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->put_Boolean(m_boolIsProximitySensitive);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CMenuItemObject::UnMarshall(IUnMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = CMenuItemObjectBase::UnMarshall(pbuffer)))
		return hr;

	ASSERT(pbuffer);
	ASSERT(m_pWorld);

	// free alloced data
	m_bstrMenuText.Empty();
	m_bstrStatusText.Empty();
	m_bstrMethod.Empty();
	m_bstrOnUpdateMethod.Empty();
	m_var.Clear();
	TerminateDISPPARAMS(VARIANT_TRUE, &m_dispparams);
	SAFERELEASE(m_pOwner);

	hr = pbuffer->get_Long(&m_Flags);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->get_String(&m_bstrMenuText.m_str);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->get_String(&m_bstrStatusText.m_str);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->get_String(&m_bstrMethod.m_str);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->get_String(&m_bstrOnUpdateMethod.m_str);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pWorld->UnMarshallProperty(pbuffer, &m_var);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pWorld->UnMarshallObjectProperty(pbuffer, (IObjectProperty**)&m_pSubMenu);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pWorld->UnMarshallDISPPARAMS(pbuffer, &m_dispparams);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pWorld->UnMarshallObjectProperty(pbuffer, (IObjectProperty**)&m_pOwner);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pWorld->UnMarshallObjectProperty(pbuffer, (IObjectProperty**)&m_pTarget);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->get_Boolean(&m_boolAddTargetParameter);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->get_Boolean(&m_boolIsOverlayItem);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->get_Boolean(&m_boolIsProximitySensitive);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:	
	return hr;
}

STDMETHODIMP CMenuItemObject::MarshallToScript(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut)
{
	HRESULT hr = S_OK;
	CString strContext = bstrContext;
	CString strOut;
	CString strLine;
	CString strOwner;
	CString strArgValue;
	CString strUserData;
	BSTR bstrOwner = NULL;
	BSTR bstrTarget = NULL;
	BSTR bstrArgs = NULL;
	BSTR bstrUserData = NULL;
	IPropertyList *plistArgs = NULL;
	CComBSTR bstrSubContext;
	VARIANT_BOOL bPropAsVal;

	if (bstrContext==NULL || pbstrOut==NULL || pbAsVal==NULL)
		return E_INVALIDARG;

	// initialize
	*pbstrOut = NULL;
	*pbAsVal = VARIANT_FALSE;

	// get the owner
	if (m_pOwner==NULL)
		strOwner = "Nothing";
	else
	{
		hr = m_pOwner->MarshallToScript(MARSHALL_BYREF, NULL, &bPropAsVal, &bstrOwner);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		ASSERT(bPropAsVal==VARIANT_TRUE);
		strOwner = bstrOwner;
	}

	// get the args
	if (m_dispparams.cArgs>0)
	{
		hr = CreatePropertyList(m_pWorld, &plistArgs);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = ConvertDISPPARAMSToPropertyList(&m_dispparams, plistArgs);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		bstrSubContext = bstrContext;
		bstrSubContext.Append("_t");
		hr = plistArgs->MarshallToScript(MARSHALL_BYREF, bstrSubContext, &bPropAsVal, &bstrArgs);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (bPropAsVal==VARIANT_TRUE)
		{
			strOut = "";
			strArgValue = ", " + CString(bstrArgs);
		}
		else
		{
			strOut = bstrArgs;
			strArgValue = ", " + CString(bstrSubContext);
		}
	}
	else
	{
		strArgValue = "";
	}

	// write out stuff to create the menu item
	strLine.Format(	"\tSet %s = World.CreateMenuItem(\"%s\", %d, %s, %s, %s%s)\n",
					(LPCTSTR) strContext,
					(LPCTSTR) CString(m_bstrMenuText),
					m_Flags,
					(LPCTSTR) strOwner,
					((BSTR)m_bstrOnUpdateMethod==NULL) ? (LPCTSTR) "\"\"" : (LPCTSTR) ("\"" + CString(m_bstrOnUpdateMethod) + "\""),
					((BSTR)m_bstrMethod==NULL) ? (LPCTSTR) "\"\"" : (LPCTSTR) ("\"" + CString(m_bstrMethod) + "\""),
					(LPCTSTR) strArgValue );

	strOut += strLine;

	// write out other stuff if not default

	// user data
	if (m_var.vt != VT_EMPTY)
	{
		// marshal the userdata
		hr = m_pWorld->MarshallPropertyToScript( m_var, MARSHALL_BYREF, bstrSubContext, &bPropAsVal, &bstrUserData);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (bPropAsVal==VARIANT_TRUE)
		{
			strLine.Format("\t%s.UserData = %s\n", (LPCTSTR) strContext, (LPCTSTR) CString(bstrUserData));
		}
		else
		{
			strOut += CString(bstrUserData);
			strLine.Format("\t%s.UserData = %s\n", (LPCTSTR) strContext, (LPCTSTR) CString(bstrSubContext));
		}
		strOut += strLine;

	}

	// m_boolAddTargetParameter
	if (m_boolAddTargetParameter!=VARIANT_FALSE)
	{
		strLine.Format("\t%s.AddTargetParameter = True\n", (LPCTSTR) strContext );
		strOut += strLine;
	}

	// m_boolIsOverlayItem
	if (m_boolIsOverlayItem!=VARIANT_FALSE)
	{
		strLine.Format("\t%s.IsOverlayItem = True\n", (LPCTSTR) strContext );
		strOut += strLine;
	}

	// m_boolIsProximitySensitive
	if (m_boolIsProximitySensitive!=VARIANT_FALSE)
	{
		strLine.Format("\t%s.IsProximitySensitive = True\n", (LPCTSTR) strContext );
		strOut += strLine;
	}

	// get the target
	if (m_pTarget!=NULL)
	{
		hr = m_pTarget->MarshallToScript(MARSHALL_BYREF, NULL, &bPropAsVal, &bstrTarget);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		ASSERT(bPropAsVal==VARIANT_TRUE);
		strLine.Format("\t%s.Target = %s\n", (LPCTSTR) strContext, (LPCTSTR) CString(bstrTarget));
		strOut += strLine;
	}

	*pbstrOut = strOut.AllocSysString();

ERROR_ENCOUNTERED:

	SAFEFREESTRING(bstrOwner);
	SAFEFREESTRING(bstrTarget);
	SAFEFREESTRING(bstrArgs);
	SAFERELEASE(plistArgs);

	return hr;

}

STDMETHODIMP CMenuItemObject::get_MenuText(BSTR* pbstrMenuText)
{
	ASSERT(pbstrMenuText);

	*pbstrMenuText = m_bstrMenuText.Copy();

	return S_OK;
}

STDMETHODIMP CMenuItemObject::put_MenuText(BSTR bstrMenuText)
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CMenuItemObject::put_MenuText\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CMenuItemObject::put_MenuText", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMenuItemObject::put_MenuText: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		m_bstrMenuText = bstrMenuText;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstrMenuText;

		hr = SendObjectPropertyCommand(psbits, DISPID_MENUITEM_MENUTEXT, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, miMenuText, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CMenuItemObject::get_Flags(long * nFlags)
{
	ASSERT(nFlags);
	*nFlags = m_Flags;
	return S_OK;
}

STDMETHODIMP CMenuItemObject::put_Flags(long nFlags)
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CMenuItemObject::put_Flags\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CMenuItemObject::put_Flags", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMenuItemObject::put_Flags: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		m_Flags = nFlags;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_I4;
		dispparams.rgvarg[0].lVal = nFlags;

		hr = SendObjectPropertyCommand(psbits, DISPID_MENUITEM_FLAGS, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, miFlags, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CMenuItemObject::get_UserData(VARIANT* pvar)
{
	return VariantCopyInd(pvar, &m_var); 
	
}

STDMETHODIMP CMenuItemObject::put_UserData(VARIANT var)
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CMenuItemObject::put_UserData\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CMenuItemObject::put_UserData", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMenuItemObject::put_UserData: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		::VariantCopyInd(&m_var, &var);
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		memcpy(&dispparams.rgvarg[0], &var, sizeof(VARIANT));

		hr = SendObjectPropertyCommand(psbits, DISPID_MENUITEM_USERDATA, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, miUserData, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CMenuItemObject::get_SubMenu(IPropertyList** ppPList)
{
	ASSERT(ppPList);

	*ppPList = m_pSubMenu;
	SAFEADDREF(*ppPList);

	return S_OK;
}

STDMETHODIMP CMenuItemObject::put_SubMenu(IPropertyList* pPList)
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CMenuItemObject::put_SubMenu\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CMenuItemObject::put_SubMenu", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMenuItemObject::put_SubMenu: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		SAFERELEASE(m_pSubMenu);
		m_pSubMenu = pPList;
		SAFEADDREF(pPList);
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = pPList;

		hr = SendObjectPropertyCommand(psbits, DISPID_MENUITEM_SUBMENU, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, miSubMenu, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CMenuItemObject::get_StatusText(BSTR* pbstrStatusText)
{
	ASSERT(pbstrStatusText);

	*pbstrStatusText = m_bstrStatusText.Copy();

	return S_OK;
}

STDMETHODIMP CMenuItemObject::put_StatusText(BSTR bstrStatusText)
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CMenuItemObject::put_StatusText\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CMenuItemObject::put_StatusText", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMenuItemObject::put_StatusText: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		m_bstrStatusText = bstrStatusText;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstrStatusText;

		hr = SendObjectPropertyCommand(psbits, DISPID_MENUITEM_STATUSTEXT, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, miStatusText, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CMenuItemObject::get_Method(BSTR* pbstrMethod)
{
	ASSERT(pbstrMethod);

	*pbstrMethod = m_bstrMethod.Copy();

	return S_OK;
}

STDMETHODIMP CMenuItemObject::put_Method(BSTR bstrMethod)
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CMenuItemObject::put_Method\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CMenuItemObject::put_Method", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMenuItemObject::put_Method: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		m_bstrMethod = bstrMethod;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstrMethod;

		hr = SendObjectPropertyCommand(psbits, DISPID_MENUITEM_METHOD, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, miMethod, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CMenuItemObject::get_OnUpdateMethod(BSTR* pbstrMethod)
{
	ASSERT(pbstrMethod);

	*pbstrMethod = m_bstrOnUpdateMethod.Copy();

	return S_OK;
}

STDMETHODIMP CMenuItemObject::put_OnUpdateMethod(BSTR bstrMethod)
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CMenuItemObject::put_OnUpdateMethod\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CMenuItemObject::put_OnUpdateMethod", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMenuItemObject::put_OnUpdateMethod: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		m_bstrOnUpdateMethod = bstrMethod;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstrMethod;

		hr = SendObjectPropertyCommand(psbits, DISPID_MENUITEM_ONUPDATEMETHOD, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, miOnUpdateMethod, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CMenuItemObject::get_ArgumentsExt(DISPPARAMS** ppdispparams)
{
	HRESULT hr = S_OK;

	if (ppdispparams == NULL)
		return E_POINTER;

	*ppdispparams = new DISPPARAMS;
	if (*ppdispparams == NULL)
		return E_OUTOFMEMORY;

	hr = CloneDISPPARAMS(&m_dispparams, *ppdispparams);

	return hr;
}

STDMETHODIMP CMenuItemObject::put_ArgumentsExt(DISPPARAMS* pdispparams)
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;
	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CMenuItemObject::put_ArgumentsExt\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CMenuItemObject::put_ArgumentsExt", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMenuItemObject::put_ArgumentsExt: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		TerminateDISPPARAMS(VARIANT_TRUE, &m_dispparams);

		hr = CloneDISPPARAMS(pdispparams, &m_dispparams);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;
		IPropertyList *plist = NULL;

		hr = CreatePropertyList(m_pWorld, &plist);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = ConvertDISPPARAMSToPropertyList(pdispparams, plist);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = plist;

		hr = SendObjectPropertyCommand(psbits, DISPID_MENUITEM_ARGUMENTSINT, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
		SAFERELEASE(plist);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, miArguments, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:

	return hr;
}

STDMETHODIMP CMenuItemObject::get_Owner(IThing ** ppIThing)
{
	ASSERT(ppIThing);

	*ppIThing = m_pOwner;
	SAFEADDREF(*ppIThing);

	return S_OK;
}

STDMETHODIMP CMenuItemObject::put_Owner(IThing * pIThing)
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CMenuItemObject::put_Owner\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CMenuItemObject::put_Owner", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMenuItemObject::put_Owner: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		SAFERELEASE(m_pOwner);
		m_pOwner = pIThing;
		SAFEADDREF(pIThing);
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = pIThing;

		hr = SendObjectPropertyCommand(psbits, DISPID_MENUITEM_OWNER, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, miOwner, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CMenuItemObject::get_Target(IThing ** ppIThing)
{
	ASSERT(ppIThing);

	*ppIThing = m_pTarget;
	SAFEADDREF(*ppIThing);

	return S_OK;
}

STDMETHODIMP CMenuItemObject::put_Target(IThing * pIThing)
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CMenuItemObject::put_Target\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CMenuItemObject::put_Target", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMenuItemObject::put_Target: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		SAFERELEASE(m_pTarget);
		m_pTarget = pIThing;
		SAFEADDREF(pIThing);
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = pIThing;

		hr = SendObjectPropertyCommand(psbits, DISPID_MENUITEM_TARGET, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, miTarget, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CMenuItemObject::get_AddTargetParameter(VARIANT_BOOL *pboolAddTargetParameter)
{
	ASSERT(pboolAddTargetParameter);

	*pboolAddTargetParameter = m_boolAddTargetParameter;

	return S_OK;
}

STDMETHODIMP CMenuItemObject::put_AddTargetParameter(VARIANT_BOOL boolAddTargetParameter)
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CMenuItemObject::put_AddTargetParameter\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CMenuItemObject::put_AddTargetParameter", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMenuItemObject::put_AddTargetParameter: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		m_boolAddTargetParameter = boolAddTargetParameter;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BOOL;
		dispparams.rgvarg[0].boolVal = boolAddTargetParameter;

		hr = SendObjectPropertyCommand(psbits, DISPID_MENUITEM_ADDTARGETPARAMETER,
										DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, miAddTargetParameter, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CMenuItemObject::get_IsOverlayItem(VARIANT_BOOL *pboolIsOverlayItem)
{
	ASSERT(pboolIsOverlayItem);

	*pboolIsOverlayItem = m_boolIsOverlayItem;

	return S_OK;
}

STDMETHODIMP CMenuItemObject::put_IsOverlayItem(VARIANT_BOOL boolIsOverlayItem)
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CMenuItemObject::put_IsOverlayItem\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CMenuItemObject::put_IsOverlayItem", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMenuItemObject::put_IsOverlayItem: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		m_boolIsOverlayItem = boolIsOverlayItem;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BOOL;
		dispparams.rgvarg[0].boolVal = boolIsOverlayItem;

		hr = SendObjectPropertyCommand(psbits, DISPID_MENUITEM_ISOVERLAYITEM,
										DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, miIsOverlayItem, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CMenuItemObject::get_IsProximitySensitive(VARIANT_BOOL *pboolIsProximitySensitive)
{
	ASSERT(pboolIsProximitySensitive);

	*pboolIsProximitySensitive = m_boolIsProximitySensitive;

	return S_OK;
}

STDMETHODIMP CMenuItemObject::put_IsProximitySensitive(VARIANT_BOOL boolIsProximitySensitive)
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CMenuItemObject::put_IsProximitySensitive\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CMenuItemObject::put_IsProximitySensitive", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CMenuItemObject::put_IsProximitySensitive: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		m_boolIsProximitySensitive = boolIsProximitySensitive;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BOOL;
		dispparams.rgvarg[0].boolVal = boolIsProximitySensitive;

		hr = SendObjectPropertyCommand(psbits, DISPID_MENUITEM_ISPROXIMITYSENSITIVE,
										DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, miIsProximitySensitive, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CMenuItemObject::Execute()
{
	HRESULT hr = S_OK;

	if (m_pOwner && (BSTR)m_bstrMethod != NULL && m_bstrMethod.Length() != 0) 
	{
		// Add in the target parameter if requested to
		if (m_boolAddTargetParameter)
		{
			DISPPARAMS dispparams;

			// Create a new parameter array
			hr = InitializeDISPPARAMS(&dispparams, m_dispparams.cArgs+1);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
			// Copy over the old values (but don't clone 'em -- no need)
			for (unsigned int i = 0; i < m_dispparams.cArgs ; i++)
				dispparams.rgvarg[i] = m_dispparams.rgvarg[i];
			// Add the target parameter
			dispparams.rgvarg[m_dispparams.cArgs].vt = VT_DISPATCH;
			dispparams.rgvarg[m_dispparams.cArgs].pdispVal = m_pTarget ? m_pTarget : m_pOwner;

			// Now call the method
			hr = m_pOwner->InvokeMethodExt(m_bstrMethod, &dispparams, NULL);

			// And free dispparams
			TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
		}
		else
		{
			// Otherwise, just call with the existing arguments
			hr = m_pOwner->InvokeMethodExt(m_bstrMethod, &m_dispparams, NULL);
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CMenuItemObject::OnUpdate()
{
	HRESULT hr = S_OK;

	// invoke update method, iff supplied
	if (m_pOwner && (BSTR)m_bstrOnUpdateMethod != NULL && m_bstrOnUpdateMethod.Length() != 0)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = (IObjectProperty *) GetUnknown();

		hr = m_pOwner->InvokeMethodExt(m_bstrOnUpdateMethod, &dispparams, NULL);

ERROR_ENCOUNTERED:
		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	return hr;
}

STDMETHODIMP CMenuItemObject::Duplicate(IMenuItem **ppIMenuItem)
{
	HRESULT hr = E_FAIL;
	IMenuItem * pIMenuItem = NULL;
	IPropertyList *pListCopy = NULL;
	IMenuItem *pMIReal = NULL;
	IMenuItem *pMICopy = NULL;

	ASSERT(ppIMenuItem);

	if (FAILED(CreateMenuItemExt(m_pWorld, m_bstrMenuText, m_Flags, m_pOwner, 
		m_bstrOnUpdateMethod, m_bstrMethod, &m_dispparams, &pIMenuItem)))
		goto errout;

	if (FAILED(pIMenuItem->put_Target(m_pTarget)))
		goto errout;

	if (FAILED(pIMenuItem->put_AddTargetParameter(m_boolAddTargetParameter)))
		goto errout;

	if (FAILED(pIMenuItem->put_IsOverlayItem(m_boolIsOverlayItem)))
		goto errout;

	if (FAILED(pIMenuItem->put_IsProximitySensitive(m_boolIsProximitySensitive)))
		goto errout;

	// Duplicate the sub-menu (if any)
	if (m_pSubMenu)
	{
		long lFlags = 0;
		long iItem;
		long cItem;

		// create an empty list
		hr = CreatePropertyList(m_pWorld, &pListCopy);
		if (FAILED(hr))
			goto errout;

		// get count of items
		hr = m_pSubMenu->get_Count(&cItem);
		if (FAILED(hr))
			goto errout;

		for (iItem=0; iItem<cItem; iItem++)
		{
			// get an item
			hr = m_pSubMenu->get_ObjectPropertyExt(iItem, IID_IMenuItem, (IObjectProperty**)&pMIReal);
			if (FAILED(hr))
				goto errout;
			ASSERT(pMIReal);

			// copy the item
			hr = pMIReal->Duplicate(&pMICopy);
			if (FAILED(hr))
				goto errout;
			ASSERT(pMICopy);

			// Add it into the list
			hr = pListCopy->AddObjectProperty(pMICopy);
			if (FAILED(hr))
				goto errout;

			// release loop stuff
			SAFERELEASE(pMIReal);
			SAFERELEASE(pMICopy);
		}
	}

	if (FAILED(pIMenuItem->put_SubMenu(pListCopy)))
		goto errout;

	if (FAILED(pIMenuItem->put_UserData(m_var)))
		goto errout;

	if (FAILED(pIMenuItem->put_StatusText(m_bstrStatusText)))
		goto errout;

	hr = S_OK;

	*ppIMenuItem = pIMenuItem;

	SAFEADDREF(*ppIMenuItem);

errout:

	SAFERELEASE(pIMenuItem);
	SAFERELEASE(pListCopy);
	SAFERELEASE(pMIReal);
	SAFERELEASE(pMICopy);

	return hr;
}

