// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// WorldMar.cpp : Implementation of CfooApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include "worldobj.h"

EXTERN_C const CLSID CLSID_Thing;
EXTERN_C const CLSID CLSID_COMModule;
EXTERN_C const CLSID CLSID_ScriptModule;

typedef CVWComPtr<IObjectProperty, NULL, &IID_IObjectProperty> CObjectPropertyPtr;
typedef CVWComPtr<IThing, NULL, &IID_IThing> CThingPtr;

//#define TRACE_MARSHALLPROPERTY VWT_NONE
#define TRACE_MARSHALLPROPERTY VWT_METHOD

//#define TRACE_MARSHALLDISPPARAMS VWT_NONE
#define TRACE_MARSHALLDISPPARAMS VWT_METHOD

//#define TRACE_MARSHALLTHING VWT_NONE
#define TRACE_MARSHALLTHING VWT_METHOD

//#define TRACE_UNMARSHALLTHING VWT_NONE
#define TRACE_UNMARSHALLTHING VWT_METHOD

STDMETHODIMP CWorldObject::MarshallObjectProperty(IObjectProperty* pprop, long lHint, DWORD dwOptions, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::MarshallObjectProperty\n");

	ASSERT(pbuffer);
	if (pbuffer == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::MarshallObjectProperty: incoming argument (IMarshallBuffer*) is NULL\n");
		return E_INVALIDARG;
	}

	// marshall variant type
	hr = pbuffer->put_UShort(VT_DISPATCH);

	// marshall tag byte for null pointers
	if (pprop == NULL)
	{
		hr = pbuffer->put_Byte(0);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::MarshallObjectProperty: IObjectProperty (null)\n");
#endif
	}
	else
	{
		hr = pbuffer->put_Byte((unsigned char)lHint);

		// if IThing, marshall object
		if (lHint == clsidThing)
		{
			IThing* pthing = NULL;

#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::MarshallObjectProperty: IThing\n");
#endif

			if (SUCCEEDED(pprop->QueryInterface(IID_IThing, (LPVOID*)&pthing)))
			{
				hr = MarshallThing(pthing, dwOptions, pbuffer);

				SAFERELEASE(pthing);
			}
		}
		// else if IModule, marshall module
		else if (lHint == clsidCOMModule || lHint == clsidScriptModule)
		{
			IModule* pmodule = NULL;

#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::MarshallObjectProperty: IModule\n");
#endif

			if (SUCCEEDED(pprop->QueryInterface(IID_IModule, (LPVOID*)&pmodule)))
			{
				hr = MarshallModule(pmodule, dwOptions, pbuffer);

				SAFERELEASE(pmodule);
			}
		}
		// else, marshall property
		else
		{
			if (lHint == clsidUnknown)
			{
				CLSID clsid;

				/* hr = */ pprop->get_CLSID(&clsid);

				// marshall CLSID
				/* hr =  */ pbuffer->put_ClassID(clsid);
			}

#ifdef _DEBUG
			{
				CComBSTR bstrType;
				pprop->get_Type(&bstrType.m_str);
				VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::MarshallObjectProperty: IObjectProperty (%s)\n", CString(bstrType));
			}
#endif

			hr = pprop->Marshall(dwOptions, pbuffer);
		}
	}

	return hr; // ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::UnMarshallObjectProperty(IUnMarshallBuffer* pbuffer, IObjectProperty** ppprop)
{
	HRESULT hr = S_OK;
	CComVariant var;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallObjectProperty\n");

	hr = UnMarshallProperty(pbuffer, &var);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (var.vt == VT_DISPATCH)
	{
		// deref current value (if any)
		SAFERELEASE(*ppprop);

		*ppprop = (IObjectProperty*)var.pdispVal;

		SAFEADDREF(*ppprop);
	}
	else
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::UnMarshallObjectProperty: property not object (%d)\n", var.vt);
		hr = VWOBJECT_E_WRONGPROPERTYTYPE;
	}

ERROR_ENCOUNTERED:
	return hr; // ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::MarshallProperty(VARIANT var, DWORD dwOptions, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	ASSERT(pbuffer);
	if (pbuffer == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::MarshallProperty: incoming argument (IMarshallBuffer*) is NULL\n");
		return E_INVALIDARG;
	}

	// marshall variant type
	hr = pbuffer->put_UShort(var.vt);
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::MarshallProperty: failed to marshall variant type\n");
		goto ERROR_ENCOUNTERED;
	}

	switch (var.vt)
	{
	case VT_EMPTY:
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLPROPERTY, "CWorldObject::MarshallProperty: VT_EMPTY\n");
#endif
		break;
	case VT_ERROR:
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLPROPERTY, "CWorldObject::MarshallProperty: VT_ERROR\n");
#endif
		break;
	case VT_NULL:
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLPROPERTY, "CWorldObject::MarshallProperty: VT_NULL\n");
#endif
		break;
	case VT_I2:
		hr = pbuffer->put_Short(var.iVal);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLPROPERTY, "CWorldObject::MarshallProperty: VT_I2 %d\n", var.iVal);
#endif
		break;
	case VT_I4:
		hr = pbuffer->put_Long(var.lVal);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLPROPERTY, "CWorldObject::MarshallProperty: VT_I4 %ld\n", var.lVal);
#endif
		break;
	case VT_INT:
		hr = pbuffer->put_Int(var.intVal);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLPROPERTY, "CWorldObject::MarshallProperty: VT_INT %ld\n", var.intVal);
#endif
		break;
	case VT_R4:
		hr = pbuffer->put_Float(var.fltVal);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLPROPERTY, "CWorldObject::MarshallProperty: VT_R4 %f\n", var.fltVal);
#endif
		break;
	case VT_R8:
		hr = pbuffer->put_Double(var.dblVal);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLPROPERTY, "CWorldObject::MarshallProperty: VT_R8 %lf\n", var.dblVal);
#endif
		break;
	case VT_BSTR:
		hr = pbuffer->put_String(var.bstrVal);

#ifdef _DEBUG
		{
			CString str(var.bstrVal);
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLPROPERTY, "CWorldObject::MarshallProperty: VT_BSTR %s\n", str);
		}
#endif
		break;

	case VT_BLOB:
		hr = pbuffer->put_BLOB(var.bstrVal);

#ifdef _DEBUG
		{
			CString str(var.bstrVal);
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLPROPERTY, "CWorldObject::MarshallProperty: VT_BSTR %s\n", str);
		}
#endif
		break;

	case VT_BOOL:
		hr = pbuffer->put_Boolean(var.boolVal);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLPROPERTY, "CWorldObject::MarshallProperty: VT_BOOL %s\n", var.boolVal == VARIANT_TRUE ? "true" : "false");
#endif
		break;
	case VT_UI1:
		hr = pbuffer->put_Byte(var.bVal);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLPROPERTY, "CWorldObject::MarshallProperty: VT_UI1 %c\n", var.bVal);
#endif
		break;
	case VT_UI2:
		hr = pbuffer->put_UShort(var.uiVal);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLPROPERTY, "CWorldObject::MarshallProperty: VT_UI2 %u\n", var.uiVal);
#endif
		break;
	case VT_UI4:
		hr = pbuffer->put_ULong(var.ulVal);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLPROPERTY, "CWorldObject::MarshallProperty: VT_UI4 %lu\n", var.ulVal);
#endif
		break;
	case VT_UINT:
		hr = pbuffer->put_UInt(var.uintVal);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLPROPERTY, "CWorldObject::MarshallProperty: VT_UINT %lu\n", var.uintVal);
#endif
		break;
	case VT_DISPATCH:
		{
			IDispatch* pdisp = var.pdispVal;

			// marshall tag byte for null pointers
			if (pdisp == NULL)
			{
				hr = pbuffer->put_Byte(0);

#ifdef _DEBUG
				VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLPROPERTY, "CWorldObject::MarshallProperty: VT_DISPATCH (null)\n");
#endif
			}
			else
			{
				IObjectProperty* pprop = NULL;

				// REVIEW: proposal for streamlining marshalling
				// 0: NULL ptr
				// 1 - 254: built-in object property (IThing, IPropertyMap, etc.)
				//			(lookup in index to CLSID map)
				// 255: custom object property (needs CLSID)

				if (SUCCEEDED(pdisp->QueryInterface(IID_IObjectProperty, (LPVOID*)&pprop)))
				{
					CLSID clsid;
					unsigned char bIndex = 0;
					TCHAR szCLSID[256];

					hr = pprop->get_CLSID(&clsid);

					VWLPTSTRFromCLSID(clsid, szCLSID);

					if (m_indexTable.Lookup((LPCTSTR)szCLSID, bIndex))
					{
						// well-known CLSID
						hr = pbuffer->put_Byte(bIndex);
						if (FAILED(hr))
						{
							VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::MarshallProperty: failed to marshall tag byte (known CLSID %d)\n", bIndex);
							goto ERROR_ENCOUNTERED;
						}
					}
					else
					{
						// unknown CLSID
						hr = pbuffer->put_Byte(255);
						if (FAILED(hr))
						{
							VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::MarshallProperty: failed to marshall tag byte (unknown CLSID)\n");
							goto ERROR_ENCOUNTERED;
						}

						// marshall CLSID
						hr = pbuffer->put_ClassID(clsid);
						if (FAILED(hr))
						{
							VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::MarshallProperty: failed to marshall CLSID\n");
							goto ERROR_ENCOUNTERED;
						}

#ifdef _DEBUG
						{
							CComBSTR bstrType("n/a");
							pprop->get_Type(&bstrType.m_str);

							VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CWorldObject::MarshallProperty: type (%s) not in lookup table, clsid %s\n", CString(bstrType), szCLSID);
						}
#endif
					}

					IThing* pthing = NULL;
					IModule* pmodule = NULL;

					// if IThing, marshall object
					if (SUCCEEDED(pprop->QueryInterface(IID_IThing, (LPVOID*)&pthing)))
					{
#ifdef _DEBUG
						VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLPROPERTY, "CWorldObject::MarshallProperty: VT_DISPATCH IThing\n");
#endif

						hr = MarshallThing(pthing, dwOptions, pbuffer);

						SAFERELEASE(pthing);

						if (FAILED(hr))
							VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::MarshallProperty: (VT_DISPATCH) failed to marshall IThing\n");
					}
					else if (SUCCEEDED(pprop->QueryInterface(IID_IModule, (LPVOID*)&pmodule)))
					{
#ifdef _DEBUG
						VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLPROPERTY, "CWorldObject::MarshallProperty: VT_DISPATCH IModule\n");
#endif

						hr = MarshallModule(pmodule, dwOptions, pbuffer);

						SAFERELEASE(pmodule);

						if (FAILED(hr))
							VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::MarshallProperty: (VT_DISPATCH) failed to marshall IModule\n");
					}
					// else, marshall property
					else
					{
#ifdef _DEBUG
						VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLPROPERTY, "CWorldObject::MarshallProperty: VT_DISPATCH IObjectProperty %s\n", szCLSID);
#endif

						hr = pprop->Marshall(dwOptions, pbuffer);
						if (FAILED(hr))
							VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::MarshallProperty: (VT_DISPATCH) failed to marshall IObjectProperty\n");
					}

					SAFERELEASE(pprop);

					if (FAILED(hr))
						goto ERROR_ENCOUNTERED;
				}
				// else, just an IDispatch, marshall as NULL
				else
				{
#ifdef _DEBUG
					VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLPROPERTY, "CWorldObject::MarshallProperty: VT_DISPATCH IDispatch (marshall as NULL)\n");
#endif

					hr = pbuffer->put_Byte(0);
					if (FAILED(hr))
					{
						VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::MarshallProperty: (VT_DISPATCH) failed to marshall null ptr\n");
						goto ERROR_ENCOUNTERED;
					}
				}
			}
		}
		break;
	case VT_UNKNOWN:
		// no-op
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLPROPERTY, "CWorldObject::MarshallProperty: VT_UNKNOWN\n");
#endif
		break;
	default:
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::MarshallProperty: unknown property type (%d)\n", var.vt);
#endif
		hr = VWOBJECT_E_UNKNOWNPROPERTYTYPE;
		break;
	}

ERROR_ENCOUNTERED:
#ifdef _DEBUG
	if (FAILED(hr))
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::MarshallProperty: failed to marshall property type (%d), hr %x\n", var.vt, hr);
#endif

	return hr; // ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::UnMarshallProperty(IUnMarshallBuffer* pbuffer, VARIANT* pvar)
{
	HRESULT hr = S_OK;

	ASSERT(pbuffer);
	if (pbuffer == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::UnMarshallProperty: incoming argument (IUnMarshallBuffer*) is NULL\n");
		return E_INVALIDARG;
	}

	ASSERT(pvar);
	if (pvar == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::UnMarshallProperty: outgoing pointer (VARIANT*) is NULL\n");
		return E_POINTER;
	}

	::VariantClear(pvar);

	// unmarshall variant type
	hr = pbuffer->get_UShort(&pvar->vt);
	if (FAILED(hr))
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::UnMarshallProperty: failed to unmarshall variant type\n");
		goto ERROR_ENCOUNTERED;
	}

	switch (pvar->vt)
	{
	case VT_EMPTY:
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallProperty: VT_EMPTY\n");
#endif
		break;
	case VT_ERROR:
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallProperty: VT_ERROR\n");
#endif
		break;
	case VT_NULL:
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallProperty: VT_NULL\n");
#endif
		break;
	case VT_I2:
		hr = pbuffer->get_Short(&pvar->iVal);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallProperty: VT_I2 %d\n", pvar->iVal);
#endif
		break;
	case VT_I4:
		hr = pbuffer->get_Long(&pvar->lVal);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallProperty: VT_I4 %ld\n", pvar->lVal);
#endif
		break;
	case VT_INT:
		hr = pbuffer->get_Int(&pvar->intVal);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallProperty: VT_INT %ld\n", pvar->intVal);
#endif
		break;
	case VT_R4:
		hr = pbuffer->get_Float(&pvar->fltVal);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallProperty: VT_R4 %f\n", pvar->fltVal);
#endif
		break;
	case VT_R8:
		hr = pbuffer->get_Double(&pvar->dblVal);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallProperty: VT_R8 %lf\n", pvar->dblVal);
#endif
		break;
	case VT_BSTR:
		pvar->bstrVal = NULL;

		hr = pbuffer->get_String(&pvar->bstrVal);

#ifdef _DEBUG
		{
			CString str(pvar->bstrVal);
			VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallProperty: VT_BSTR %s\n", str);
		}
#endif
		break;

	case VT_BLOB:
		pvar->bstrVal = NULL;

		hr = pbuffer->get_BLOB(&pvar->bstrVal);
		
		// REVIEW:  set this to a bstr so as not to make ::Invoke fail
		//pvar->vt = VT_BSTR;
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallProperty: VT_BLOB\n");
#endif
		break;

	case VT_BOOL:
		hr = pbuffer->get_Boolean(&pvar->boolVal);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallProperty: VT_BOOL %s\n", pvar->boolVal == VARIANT_TRUE ? "true" : "false");
#endif
		break;
	case VT_UI1:
		hr = pbuffer->get_Byte(&pvar->bVal);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallProperty: VT_UI1 %c\n", pvar->bVal);
#endif
		break;
	case VT_UI2:
		hr = pbuffer->get_UShort(&pvar->uiVal);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallProperty: VT_UI2 %u\n", pvar->uiVal);
#endif
		break;
	case VT_UI4:
		hr = pbuffer->get_ULong(&pvar->ulVal);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallProperty: VT_UI4 %lu\n", pvar->ulVal);
#endif
		break;
	case VT_UINT:
		hr = pbuffer->get_UInt(&pvar->uintVal);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallProperty: VT_UINT %lu\n", pvar->uintVal);
#endif
		break;
	case VT_DISPATCH:
		{
			unsigned char tag = 0;

			pvar->pdispVal = NULL;
			
			// REVIEW: proposal for streamlining marshalling
			// 0: NULL ptr
			// 1 - 254: built-in object property (IThing, IPropertyMap, etc.)
			//			(lookup in index to CLSID map)
			// 255: custom object property (needs CLSID)

			hr = pbuffer->get_Byte(&tag);
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::UnMarshallProperty: (VT_DISPATCH) failed reading tag byte\n");
				goto ERROR_ENCOUNTERED;
			}

			// null pointer
			if (tag == 0)
			{
#ifdef _DEBUG
				VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallProperty: VT_DISPATCH (null)\n");
#endif
			}
			else
			{
				if (tag == clsidThing)
				{
					IThing* pthing = NULL;

#ifdef _DEBUG
					VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallProperty: VT_DISPATCH IThing\n");
#endif

					hr = UnMarshallThing(pbuffer, &pthing);

					// REVIEW: done to get around get_Global failure
					if (hr == VWODB_E_OBJECTNOTEXIST)
						hr = S_OK;

					if (FAILED(hr))
					{
						VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::UnMarshallProperty: (VT_DISPATCH) UnMarshallThing failed\n");
						goto ERROR_ENCOUNTERED;
					}

					// NOTE: don't release pthing, pvar takes ownership
					pvar->pdispVal = pthing;
				}
				else if (tag == clsidCOMModule || tag == clsidScriptModule)
				{
					IModule* pmodule = NULL;

#ifdef _DEBUG
					VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallProperty: VT_DISPATCH IModule\n");
#endif

					hr = UnMarshallModule((tag == clsidCOMModule) ? CLSID_COMModule : CLSID_ScriptModule, pbuffer, &pmodule);

					// absorb non-existent modules, just return NULL ptr
					if (hr == VWOBJECT_E_MODULENOTEXIST)
						hr = S_OK;

					if (FAILED(hr))
					{
						VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::UnMarshallProperty: (VT_DISPATCH) UnMarshallModule failed\n");
						goto ERROR_ENCOUNTERED;
					}

					// NOTE: don't release pmodule, pvar takes ownership
					pvar->pdispVal = pmodule;
				}
				else
				{
					CLSID clsid;
					IObjectProperty* pprop = NULL;

					// custom CLSID: tag == 255
					if (tag == 255)
					{
						hr = pbuffer->get_ClassID(&clsid);
						if (FAILED(hr))
						{
							VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::UnMarshallProperty: (VT_DISPATCH) failed to unmarshall CLSID for IObjectProperty\n");
							goto ERROR_ENCOUNTERED;
						}
					}
					else
					{
						// well-known CLSID: tag >= 1 && tag <= 254
						if (m_clsidTable.Lookup(tag, clsid) == FALSE)
						{
#ifdef _DEBUG
							VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::UnMarshallProperty: unknown CLSID index (%d)\n", tag);
#endif
							hr = VWOBJECT_E_UNKNOWNCLSIDINDEX;
							goto ERROR_ENCOUNTERED;
						}
					}

#ifdef _DEBUG
					CString strCLSID = VWStringFromCLSID(clsid);
					VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallProperty: VT_DISPATCH IObjectProperty %s\n", strCLSID);
#endif

					hr = CreateObjectPropertyExt(clsid, NULL, (IObjectProperty**)&pprop);
					if (FAILED(hr))
					{
#ifdef _DEBUG
						VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::UnMarshallProperty: (VT_DISPATCH) failed to create object property (tag %d), %s\n", tag, strCLSID);
#endif
						goto ERROR_ENCOUNTERED;
					}

					ASSERT(pprop);

					hr = pprop->UnMarshall(pbuffer);
					if (FAILED(hr))
					{
#ifdef _DEBUG
						VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::UnMarshallProperty: (VT_DISPATCH) failed to unmarshall object property (tag %d) %s, hr %x\n", tag, strCLSID, hr);
#endif
						SAFERELEASE(pprop);
						goto ERROR_ENCOUNTERED;
					}

					// NOTE: don't release pprop, pvar takes ownership
					pvar->pdispVal = pprop;
				}
			}
		}
		break;
	case VT_UNKNOWN:
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallProperty: VT_UNKNOWN\n");
#endif

		pvar->punkVal = NULL;
		break;
	default:
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::UnMarshallProperty: unknown property type (%d)\n", pvar->vt);
#endif
		hr = VWOBJECT_E_UNKNOWNPROPERTYTYPE;
		break;
	}

ERROR_ENCOUNTERED:
	if (FAILED(hr))
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::UnMarshallProperty: failed to unmarshall property type (%d), hr %x\n", pvar->vt, hr);
#endif

		::VariantClear(pvar);
	}

	return hr; // ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::MarshallThing(IThing* pthing, DWORD dwOptions, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;
	long lObjectID = NULL_OBJECT;
	CComBSTR bstrGUID;
	CComBSTR bstrName;
	DWORD dwRef = 0, dwID = 0;
	
	ASSERT(pthing);
	if (pthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::MarshallThing: incoming argument (IThing*) is NULL\n");		
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(pbuffer);
	if (pbuffer == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::MarshallThing: incoming argument (IMarshallBuffer*) is NULL\n");
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}	

/*	
	if (EXTRACTID(dwOptions) == MARSHALL_BYGUID)
	{
		hr = pthing->get_BSTR(CComBSTR("GUID"), &bstrGUID);
		if(FAILED(hr))
		{
			dwOptions = MARSHALLDEST(dwOptions,MARSHALL_BYID);
			hr = S_OK;
		}			
	}
*/		

	dwRef = EXTRACTREF(dwOptions);
	dwID = EXTRACTID(dwOptions);

	//Put a type code before the rest..
	// 0: GUID/BYREF.
	// 1: GUID/BYVAL.
	// 2: ID/BYREF.
	// 3: ID/BYVAL.
	if (dwRef == MARSHALL_BYREF)
	{
		if (dwID == MARSHALL_BYGUID)
		{
			// BYGUID, BYREF
			hr = pbuffer->put_Byte(0);
	
			hr = pthing->get_String(CComBSTR("GUID"), &bstrGUID.m_str);
			if (SUCCEEDED(hr))
			{
				hr = pbuffer->put_String(bstrGUID, VARIANT_FALSE);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;
			}			
			else
			{
				hr = VWOBJECT_E_INVALIDOBJECT;
				goto ERROR_ENCOUNTERED;
			}

			hr = pthing->get_Name(&bstrName.m_str);
			if (SUCCEEDED(hr))
			{
				hr = pbuffer->put_String(bstrName);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;
			}			
			else
			{
				hr = VWOBJECT_E_INVALIDOBJECT;
				goto ERROR_ENCOUNTERED;
			}			

#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLTHING, "CWorldObject::MarshallThing: by ref (GUID), name (%s), GUID %s\n", CString(bstrName), CString(bstrGUID));
#endif
		}
		else if (dwID == MARSHALL_BYID)
		{
			// BYID, BYREF
			hr = pbuffer->put_Byte(2);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			hr = pbuffer->put_Long(((CThingObject*)pthing)->m_lObjectID);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLTHING, "CWorldObject::MarshallThing: by ref (ID), object ID (%d)\n", lObjectID);
#endif
		}
		else 
			ASSERT(0);
	}
	else if (dwRef == MARSHALL_BYVALUE)
	{
		if (dwID == MARSHALL_BYGUID)
		{
			// BYGUID, BYVALUE
			hr = pbuffer->put_Byte(1);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			hr = pthing->get_String(CComBSTR("GUID"), &bstrGUID.m_str);
			if(SUCCEEDED(hr))
			{
				hr = pbuffer->put_String(bstrGUID, VARIANT_FALSE);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;

				hr = pthing->Marshall(MARSHALLREF(dwOptions, MARSHALL_BYREF), pbuffer);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;
			}			
			else
			{
				hr = VWOBJECT_E_INVALIDOBJECT;
				goto ERROR_ENCOUNTERED;
			}

#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLTHING, "CWorldObject::MarshallThing: by value (GUID), GUID %s\n", CString(bstrGUID));
#endif
		}
		else if (dwID == MARSHALL_BYID)
		{
			// BYID, BYVALUE
			hr = pbuffer->put_Byte(3);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			hr = pbuffer->put_Long(((CThingObject*)pthing)->m_lObjectID);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			hr = pthing->Marshall(MARSHALLREF(dwOptions, MARSHALL_BYREF), pbuffer);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLTHING, "CWorldObject::MarshallThing: by value (ID), object ID (%d)\n", lObjectID);
#endif
		}
		else 
			ASSERT(0);
	}
	else
		ASSERT(0);

ERROR_ENCOUNTERED:
	return hr; // ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::UnMarshallThing(IUnMarshallBuffer* pbuffer, IThing** ppthing)
{
	HRESULT hr = S_OK;
	long lObjectID = NULL_OBJECT;
	unsigned char ucType = 0;
	CComBSTR bstrGUID;
	CComBSTR bstrName;
	IPropertyMap *pGUIDs = NULL;
	long currentID;

	ASSERT(pbuffer);
	if (pbuffer == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::UnMarshallThing: incoming argument (IUnMarshallBuffer*) is NULL\n");
		return E_INVALIDARG;
	}

	ASSERT(ppthing);
	if (ppthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::UnMarshallThing: outgoing pointer (IThing**) is NULL\n");
		return E_POINTER;
	}

	*ppthing = NULL;

	hr = pbuffer->get_Byte(&ucType);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	switch(ucType)
	{
	case 0: // BYGUID, BYREF
		hr = pbuffer->get_String(&bstrGUID.m_str);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pbuffer->get_String(&bstrName.m_str);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = get_GUIDs(&pGUIDs);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGUIDs->get_Thing(bstrGUID, ppthing);
		
		// if by ref and object doesn't exist in map,
		// create empty instance and mark it as a stub object
		if (FAILED(hr))
		{
			// assume it's an avatar.
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_UNMARSHALLTHING, "CWorldObject::UnMarshallThing: by ref (GUID), creating incomplete instance, name (%s), GUID %s\n", 
				CString(bstrName), CString(bstrGUID));
#endif

			// create a new incomplete object.
			hr = CreateInstanceIncomplete(bstrName, bstrGUID, ppthing);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
			
			// This code is very rich in semantics.
		}
		else
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_UNMARSHALLTHING, "CWorldObject::UnMarshallThing: by ref (GUID), found object by GUID %s, name (%s)\n", 
				CString(bstrGUID), CString(bstrName));
#endif
		}

		break;
	case 1: // BYGUID, BYVALUE
		hr = pbuffer->get_String(&bstrGUID.m_str);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = get_GUIDs(&pGUIDs);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pGUIDs->get_Thing(bstrGUID, ppthing);

		// if object not found, create new instance
		if (FAILED(hr))
		{
			hr = m_pGlobal->get_Long(m_bstrCurrentID, &currentID);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			hr = CreateObjectInt(currentID, ppthing);
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::UnMarshallThing: CreateObjectInt failed, object ID (%d)\n", currentID);
				goto ERROR_ENCOUNTERED;
			}

			// don't log
			m_bShouldRemote = VARIANT_FALSE;

			(*ppthing)->put_IsStubInt(VARIANT_FALSE);

			m_bShouldRemote = VARIANT_TRUE;

			// increment current obj ID
			currentID++;

			// update persistent value
			hr = m_pGlobal->put_Long(m_bstrCurrentID, currentID);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;			

			{
				CSetUID SetUID(this, m_pGlobal, m_pGlobal, VARIANT_TRUE);

				hr = pGUIDs->put_Thing(bstrGUID, *ppthing);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;			
			}

#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_UNMARSHALLTHING, "CWorldObject::UnMarshallThing: by value (GUID), created new object with ID (%d), GUID %s\n", 
				lObjectID, CString(bstrGUID));
#endif

			// NOTE: don't log this call to CreateObject
		}
		// else, it's a stub object and we need to make it real
		else
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_UNMARSHALLTHING, "CWorldObject::UnMarshallThing: by value (GUID), filling in stub object, GUID %s\n", CString(bstrGUID));
#endif

			// don't log
			m_bShouldRemote = VARIANT_FALSE;

			(*ppthing)->put_IsStubInt(VARIANT_FALSE);

			m_bShouldRemote = VARIANT_TRUE;
		}

		hr = (*ppthing)->UnMarshall(pbuffer);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		break;
	case 2: // BYID, BYREF
		hr = pbuffer->get_Long(&lObjectID);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = get_ObjectByID(lObjectID, ppthing);

		// if by ref and object doesn't exist in map,
		// create empty instance and mark it as a stub object
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_UNMARSHALLTHING, "CWorldObject::UnMarshallThing: by ref (ID), creating stub object, object ID (%d)\n", lObjectID);
	
			hr = CreateObjectInt(lObjectID, ppthing);
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::UnMarshallThing: CreateObject failed, object ID (%d)\n", currentID);
				goto ERROR_ENCOUNTERED;
			}

			// NOTE: don't log this call to CreateObject
		}
		else
		{
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_UNMARSHALLTHING, "CWorldObject::UnMarshallThing: by ref (ID), object ID (%d)\n", lObjectID);
		}

		break;
	case 3: // BYID, BYVALUE
		hr = pbuffer->get_Long(&lObjectID);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = get_ObjectByID(lObjectID, ppthing);

		// if object not found, create new instance
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_UNMARSHALLTHING, "CWorldObject::UnMarshallThing: by value (ID), created new object with ID (%d)\n", lObjectID);

			hr = CreateObjectInt(lObjectID, ppthing);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
		// else, it's a stub object and we need to make it real
		else
		{
			VWTRACE(m_pWorld, "VWOBJECT", TRACE_UNMARSHALLTHING, "CWorldObject::UnMarshallThing: by value (ID), filling in stub object, object ID (%d)\n", lObjectID);
		}

		// mark as not a stub, don't log
		m_bShouldRemote = VARIANT_FALSE;

		(*ppthing)->put_IsStubInt(VARIANT_FALSE);

		m_bShouldRemote = VARIANT_TRUE;

		hr = (*ppthing)->UnMarshall(pbuffer);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		break;

	default:
		ASSERT(0);
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pGUIDs);

	return hr; // ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::MarshallDISPPARAMS(DISPPARAMS* pdispparams, IMarshallBuffer* pbuffer, DWORD dwOptions)
{
	HRESULT hr = S_OK;
	unsigned int paramcnt = 0;

	ASSERT(pdispparams);
	if (pdispparams == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::MarshallDISPPARAMS: incoming argument (DISPPARAMS*) is NULL\n");
		return E_INVALIDARG;
	}

	paramcnt = pdispparams->cArgs;

	ASSERT(pbuffer);
	if (pbuffer == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::MarshallDISPPARAMS: incoming argument (IMarshallBuffer*) is NULL\n");
		return E_INVALIDARG;
	}

	hr = pbuffer->put_UInt(paramcnt);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLDISPPARAMS, "CWorldObject::MarshallDISPPARAMS: %d params\n", paramcnt);
#endif

	for (unsigned int i = 0; i < paramcnt && SUCCEEDED(hr); i++)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLDISPPARAMS, "CWorldObject::MarshallDISPPARAMS: parameter %d\n", i);
#endif

		hr = MarshallProperty(pdispparams->rgvarg[i], dwOptions, pbuffer);
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_MARSHALLDISPPARAMS, "CWorldObject::MarshallDISPPARAMS: end\n");
#endif

	return hr; // ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::UnMarshallDISPPARAMS(IUnMarshallBuffer* pbuffer, DISPPARAMS* pdispparams)
{
	HRESULT hr = S_OK;
	unsigned int paramcnt = 0;
	unsigned int i = 0;

	ASSERT(pbuffer);
	if (pbuffer == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::UnMarshallDISPPARAMS: incoming argument (IUnMarshallBuffer*) is NULL\n");
		return E_INVALIDARG;
	}

	ASSERT(pdispparams);
	if (pdispparams == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::UnMarshallDISPPARAMS: incoming argument (DISPPARAMS*) is NULL\n");
		return E_INVALIDARG;
	}

	hr = pbuffer->get_UInt(&paramcnt);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallDISPPARAMS: %d params\n", paramcnt);
#endif

 	hr = InitializeDISPPARAMS(pdispparams, paramcnt);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	for (i = 0; i < paramcnt && SUCCEEDED(hr); i++)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallDISPPARAMS: parameter %d\n", i);
#endif

		hr = UnMarshallProperty(pbuffer, &pdispparams->rgvarg[i]);
	}

#ifdef _DEBUG
	if (FAILED(hr))
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::UnMarshallDISPPARAMS: UnMarshallProperty failed, hr %x\n", hr);
#endif

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallDISPPARAMS: end\n");
#endif

ERROR_ENCOUNTERED:
	return hr; // ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::MarshallModule(IModule* pmodule, DWORD dwOptions, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;
	BSTR bstr = NULL;

	ASSERT(pmodule);
	if (pmodule == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::MarshallModule: incoming argument (IModule*) is NULL\n");
		return E_INVALIDARG;
	}

	ASSERT(pbuffer);
	if (pbuffer == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::MarshallModule: incoming argument (IMarshallBuffer*) is NULL\n");
		return E_INVALIDARG;
	}

	hr = pmodule->get_ModuleName(&bstr);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (EXTRACTREF(dwOptions) == MARSHALL_BYREF)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::MarshallModule: by ref, module name (%s)\n", CString(bstr));
#endif
		
		hr = pbuffer->put_Byte(0);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// marshall module name
		hr = pbuffer->put_String(bstr);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}
	else if (EXTRACTREF(dwOptions) == MARSHALL_BYVALUE)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::MarshallModule: by value, module name (%s)\n", CString(bstr));
#endif

		hr = pbuffer->put_Byte(1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// REVIEW: used to pass dwOptions
		// marshall module itself
		hr = pmodule->Marshall(MARSHALLREF(dwOptions, MARSHALL_BYREF), pbuffer);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}
	else
		ASSERT(0);

ERROR_ENCOUNTERED:
	SAFEFREESTRING(bstr);

	return hr; // ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::UnMarshallModule(REFCLSID clsid, IUnMarshallBuffer* pbuffer, IModule** ppmodule)
{
	HRESULT hr = S_OK;
	BSTR bstr = NULL;
	unsigned char bval = 0;

	ASSERT(pbuffer);
	if (pbuffer == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::UnMarshallModule: incoming argument (IUnMarshallBuffer*) is NULL\n");
		return E_INVALIDARG;
	}

	ASSERT(ppmodule);
	if (ppmodule == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CWorldObject::UnMarshallModule: outgoing pointer (IModule**) is NULL\n");
		return E_POINTER;
	}

	*ppmodule = NULL;

	hr = pbuffer->get_Byte(&bval);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// if by ref
	if (bval == 0)
	{
		// unmarshall module name
		hr = pbuffer->get_String(&bstr);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallModule: by ref, module name (%s)\n", CString(bstr));
#endif

		hr = get_Module(bstr, ppmodule);
	}
	// else, by value
	else
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CWorldObject::UnMarshallModule: by val\n");
#endif

		// create new object property
		hr = CreateObjectPropertyExt(clsid, NULL, (IObjectProperty**)ppmodule);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// unmarshall module itself
		hr = (*ppmodule)->UnMarshall(pbuffer);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	SAFEFREESTRING(bstr);

	return hr; // ReportWorldError(hr);
}

STDMETHODIMP CWorldObject::DumpProperty(VARIANT var)
{
	HRESULT hr = S_OK;

	switch (var.vt)
	{
	case VT_EMPTY:
		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "VT_EMPTY\n");
		break;
	case VT_ERROR:
		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "VT_ERROR\n");
		break;
	case VT_NULL:
		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "VT_NULL\n");
		break;
	case VT_I2:
		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "VT_I2 %d\n", var.iVal);
		break;
	case VT_I4:
		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "VT_I4 %ld\n", var.lVal);
		break;
	case VT_INT:
		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "VT_INT %ld\n", var.intVal);
		break;
	case VT_R4:
		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "VT_R4 %f\n", var.fltVal);
		break;
	case VT_R8:
		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "VT_R8 %lf\n", var.dblVal);
		break;
	case VT_BSTR:
		{
			CString str(var.bstrVal);
			VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "VT_BSTR %s\n", str);
		}
		break;
	case VT_BOOL:
		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "VT_BOOL %s\n", var.boolVal == VARIANT_TRUE ? "true" : "false");
		break;
	case VT_UI1:
		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "VT_UI1 %c\n", var.bVal);
		break;
	case VT_UI2:
		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "VT_UI2 %u\n", var.uiVal);
		break;
	case VT_UI4:
		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "VT_UI4 %lu\n", var.ulVal);
		break;
	case VT_UINT:
		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "VT_UINT %lu\n", var.uintVal);
		break;
	case VT_DISPATCH:
		{
			IDispatch* pdisp = var.pdispVal;
			CLSID clsid;

			// marshall tag byte for null pointers
			if (pdisp == NULL)
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "VT_DISPATCH (null)\n");
			}
			else
			{
				IObjectProperty* pprop = NULL;

				if (SUCCEEDED(pdisp->QueryInterface(IID_IObjectProperty, (LPVOID*)&pprop)))
				{
					hr = pprop->get_CLSID(&clsid);
					
					IThing* pthing = NULL;

					// if IThing, dump object name
					if (SUCCEEDED(pprop->QueryInterface(IID_IThing, (LPVOID*)&pthing)))
					{
						BSTR bstr = NULL;
						VARIANT_BOOL bStub;
						CString strName;

						if (SUCCEEDED(pthing->get_IsStub(&bStub)))
						{
							if (bStub == VARIANT_TRUE)
								strName	= "(stub)";
							else
								if (SUCCEEDED(pthing->get_Name(&bstr)))
								{
									strName = bstr;
									SAFEFREESTRING(bstr);
								}

							VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "VT_DISPATCH IThing %s\n", strName);
						}

						SAFERELEASE(pthing);
					}
					// else, dump property
					else
					{
#ifdef _DEBUG
						CString strCLSID = VWStringFromCLSID(clsid);
						VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "VT_DISPATCH IObjectProperty %s\n", strCLSID);
#endif

						pprop->Dump();
					}

					SAFERELEASE(pprop);
				}
				// else, just an IDispatch, dump as NULL
				else
				{
					VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "VT_DISPATCH IDispatch (marshalled as NULL)\n");
				}
			}
		}
		break;
	case VT_UNKNOWN:
		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "VT_UNKNOWN\n");
		break;
	default:
		// unknown property type
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "VT_??? (%d) unknown property\n", var.vt);
		break;
	}

	return S_OK;
}

STDMETHODIMP CWorldObject::MarshallPropertyToScript( VARIANT var, DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut)
{
	HRESULT hr = S_OK;
	CString strOut;
	BSTR bstrOut = NULL;
	IObjectProperty *pProp = NULL;

	if (pbstrOut==NULL || pbAsVal==NULL)
		return E_INVALIDARG;

	// initialize
	*pbstrOut = NULL;
	*pbAsVal = VARIANT_TRUE;

	// handle simple types
	switch(var.vt)
	{
		case VT_I2:
			strOut.Format("%d", var.iVal);
			break;

		case VT_I4:
			strOut.Format("%d", var.lVal);
			break;

		case VT_INT:
			strOut.Format("%d", var.intVal);
			break;

		case VT_R4:
			strOut.Format("%g", var.fltVal);
			break;

		case VT_R8:
			strOut.Format("%g", var.dblVal);
			break;

		case VT_BSTR:
		{
			CString str = var.bstrVal;

			if (str.Find('"')!=-1)
			{
				CString strNew;
				int ich;
				TCHAR ch;

				// quotify the string (double-up quotes)
				for (ich=0; ich<str.GetLength(); ich++)
				{
					ch = str.GetAt(ich);
					if (ch=='"')
						strNew += '"';
					strNew += ch;
				}
				str = strNew;
			}
			strOut.Format("\"%s\"", (LPCTSTR) str);
			break;
		}

		case VT_BOOL:
			strOut.Format("%s", (var.boolVal==VARIANT_TRUE) ? "True" : "False");
			break;

		case VT_UI1:
			strOut.Format("CByte(%u)", var.bVal);
			break;

		case VT_UI2:
			strOut.Format("%u", var.uiVal);
			break;

		case VT_UI4:
			strOut.Format("%u", var.ulVal);
			break;

		case VT_UINT:
			strOut.Format("%u", var.uintVal);
			break;

		case VT_DISPATCH:
			// if null, marshall out "nothing"
			if (var.pdispVal==NULL)
			{
				strOut = "Nothing";
			}
			else if (SUCCEEDED(var.pdispVal->QueryInterface( IID_IObjectProperty, (void **) &pProp)))
			{
				// it's an object property

				// REVIEW: Do we need a different context?
				hr = pProp->MarshallToScript( dwOptions, bstrContext, pbAsVal, &bstrOut);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;

				if (bstrOut != NULL)
				{
					strOut = bstrOut;

				}
				else
				{
					// get the property type
					hr = pProp->get_Type(&bstrOut);
					if (FAILED(hr))
						goto ERROR_ENCOUNTERED;

					strOut.Format(	"\tSet %s = Nothing ' REVIEW: Property \"%s\" doesn't know how to marshall to script\n",
									(LPCTSTR)CString(bstrContext),
									(LPCTSTR)CString(bstrOut) );

					*pbAsVal = VARIANT_FALSE;
				}

				SAFEFREESTRING(bstrOut);
			}
			else
			{
				strOut.Format(	"\tSet %s = Nothing ' Some non IObjectProperty\n",
								(LPCTSTR)CString(bstrContext));
				*pbAsVal = VARIANT_FALSE;
			}
			break;

		case VT_EMPTY:
			strOut = "Empty";
			break;

		case VT_NULL:
			strOut = "Null";
			break;

		default:
			// REVIEW: error code?
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
			break;
	}


	*pbstrOut = strOut.AllocSysString();

ERROR_ENCOUNTERED:

	SAFERELEASE(pProp);
	SAFEFREESTRING(bstrOut);

	return hr;
}


STDMETHODIMP CWorldObject::Dump(void)
{
	HRESULT hr = S_OK;
	IPropertyMap* pExemplars = NULL;
	IPropertyMap* pModules = NULL;
	IPropertyMap* pAvatars = NULL;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "\nWorld: begin\n");

	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "\nWorld: exemplar map\n");

	hr = get_Exemplars(&pExemplars);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	pExemplars->Dump();

	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "\nWorld: module map\n");

	hr = get_Modules(&pModules);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	pModules->Dump();

	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "\nWorld: creature map\n");

	hr = get_Avatars(&pAvatars);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	pAvatars->Dump();

	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "World: end\n");

ERROR_ENCOUNTERED:
	SAFERELEASE(pExemplars);
	SAFERELEASE(pAvatars);
	SAFERELEASE(pModules);

	return hr;
}
