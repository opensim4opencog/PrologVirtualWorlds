// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// vwstuexo.cpp : Implementation of CStudioExemplarObject
#include "stdafx.h"
#include <vwstuex.h>
#include <vwgeom_i.c>
#include "vwstuexo.h"
#include "vwutils.h"
#include "syshelp.h"
#include "vwsevent.h"



//******************************************************
// Install
// Installs this module - creates the exemplar and
// adds the properties and methods.

STDMETHODIMP CStudioExemplarObject::Install(IModule* pModule)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr = S_OK;
	CComPtr<IThing> pGlobal;
	CComPtr<IPropertySecurity> pSec;
	CComBSTR bstrName;
	CComBSTR bstrNameInternal;
	CComPtr<IPropertyMap> pEditorMap;

	CMethodInfo rgMethod[] =
	{
		{ METHOD_CLIENT,	"OnSelectionLockChanged",		NULL, PSBIT_SYSTEMEVENTHANDLER,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"OnGravityChanged",				NULL, PSBIT_SYSTEMEVENTHANDLER,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"OnCameraFollowsObjectChanged",	NULL, PSBIT_SYSTEMEVENTHANDLER,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"OnCollisionDetectionChanged",	NULL, PSBIT_SYSTEMEVENTHANDLER,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"OnEditingModeChanged",			NULL, PSBIT_SYSTEMEVENTHANDLER,	PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"OnPropertyEditorsAccess",	"OnWorldOwnerAccess",	PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"GetTypeFromProperty",	NULL, PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN,	PS_GLOBALDEFAULTMETHOD },
		{ METHOD_CLIENT | METHOD_SERVER,	"GetIIDFromType",		NULL, PSBIT_GLOBALDEFAULTMETHOD | PSBIT_HIDDEN,	PS_GLOBALDEFAULTMETHOD },
	};

	int cMethod = sizeof(rgMethod)/sizeof(rgMethod[0]);
	int iMethod;

	hr = m_pWorld->get_Global(&pGlobal);
	if (FAILED(hr))
		return hr;

	for (iMethod=0; iMethod<cMethod; iMethod++)
	{
		bstrName = rgMethod[iMethod].pszName;

		if (rgMethod[iMethod].pszNameInternal == NULL)
			bstrNameInternal = bstrName;
		else
			bstrNameInternal = rgMethod[iMethod].pszNameInternal;
		
		hr = pGlobal->CreateAndAddMethodExt(rgMethod[iMethod].lFlags,
											pModule, bstrNameInternal, bstrName,
											rgMethod[iMethod].psbits,
											rgMethod[iMethod].permissions);
	}
	
	// Add some properties to global object
 	hr = pGlobal->AddPropertyExt(CComBSTR("XAxisLocked"), CComVariant((bool)VARIANT_FALSE), 
		PSBIT_HIDDEN | PSBIT_NOTREMOTED | PSBIT_INTERNALLYADDED, PS_ALL_RW, VT_BOOL, IID_NULL, NULL);
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;

	hr = pGlobal->AddPropertyExt(CComBSTR("YAxisLocked"), CComVariant((bool)VARIANT_FALSE), 
		PSBIT_HIDDEN | PSBIT_NOTREMOTED | PSBIT_INTERNALLYADDED, PS_ALL_RW, VT_BOOL, IID_NULL, NULL);
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;

	hr = pGlobal->AddPropertyExt(CComBSTR("ZAxisLocked"), CComVariant((bool)VARIANT_FALSE), 
		PSBIT_HIDDEN | PSBIT_NOTREMOTED | PSBIT_INTERNALLYADDED, PS_ALL_RW, VT_BOOL, IID_NULL, NULL);
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;

 	hr = pGlobal->AddPropertyExt(CComBSTR("Gravity"), CComVariant((bool)VARIANT_TRUE), 
		PSBIT_HIDDEN | PSBIT_NOTREMOTED | PSBIT_INTERNALLYADDED, PS_ALL_RW, VT_BOOL, IID_NULL, NULL);
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;

 	hr = pGlobal->AddPropertyExt(CComBSTR("CollisionDetection"), CComVariant((bool)VARIANT_TRUE), 
		PSBIT_HIDDEN | PSBIT_NOTREMOTED | PSBIT_INTERNALLYADDED, PS_ALL_RW, VT_BOOL, IID_NULL, NULL);
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;

 	hr = pGlobal->AddPropertyExt(CComBSTR("SelectionLock"), CComVariant((bool)VARIANT_FALSE), 
		PSBIT_HIDDEN | PSBIT_NOTREMOTED | PSBIT_INTERNALLYADDED, PS_ALL_RW, VT_BOOL, IID_NULL, NULL);
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;

 	hr = pGlobal->AddPropertyExt(CComBSTR("CameraFollowsObject"), CComVariant((bool)VARIANT_FALSE), 
		PSBIT_HIDDEN | PSBIT_NOTREMOTED | PSBIT_INTERNALLYADDED, PS_ALL_RW, VT_BOOL, IID_NULL, NULL);
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;

 	hr = pGlobal->AddPropertyExt(CComBSTR("EditingMode"), CComVariant(0L, VT_I4), 
		PSBIT_HIDDEN | PSBIT_NOTREMOTED | PSBIT_INTERNALLYADDED, PS_ALL_RW, VT_I4, IID_NULL, NULL);
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;


	// add editors
	// Property editor map
	hr = ::CreatePropertyMap(m_pWorld, &pEditorMap);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pEditorMap->put_String(CComBSTR("Property List"), CComBSTR("sdk/editors/property/proplist.htm"));
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pEditorMap->put_String(CComBSTR("Property Map"), CComBSTR("sdk/editors/property/propmap.htm"));
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pEditorMap->put_String(CComBSTR("Menu"), CComBSTR("sdk/editors/property/menu.htm"));
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pEditorMap->put_String(CComBSTR("BoundaryList"), CComBSTR("sdk/editors/property/boundlst.htm"));
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pEditorMap->put_String(CComBSTR("Vector"), CComBSTR("sdk/editors/property/vector.htm"));
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pEditorMap->put_String(CComBSTR("MenuItem"), CComBSTR("sdk/editors/property/menuitem.htm"));
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
	hr = pEditorMap->put_String(CComBSTR("Method"), CComBSTR("sdk/editors/property/method.htm"));
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
	hr = pEditorMap->put_String(CComBSTR("Thing"), CComBSTR("sdk/editors/property/thing.htm"));
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pEditorMap->put_String(CComBSTR("String"), CComBSTR("sdk/editors/property/string.htm"));
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
	hr = pGlobal->AddPropertyExt(CComBSTR("PropertyEditors"), CComVariant((IDispatch*) pEditorMap),
				PSBIT_WORLDOWNERPROPERTY | PSBIT_INTERNALLYSET | PSBIT_HIDDEN, PS_WORLDOWNERPROPERTY, VT_DISPATCH, IID_IPropertyMap, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;


ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CStudioExemplarObject::GetTypeFromProperty(IThing *pThing, BSTR bstrPropertyName, BSTR *bstrType)
{
	HRESULT hr = S_OK;
	CComPtr<IPropertySecurity> pSec;
	CComPtr<IObjectProperty> pOP;
	CComPtr<IThing> pPropThing;
	CComPtr<IVector> pVec;
	CComPtr<IVWGeometry> pGeom;
	CComPtr<IMethod> pMethod;
	GUID ID;
	VARIANT_BOOL vBool;
	CString strType;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	strType.LoadString(IDS_TYPE_UNKNOWN);

	if (!pThing)
	{
		hr = E_POINTER;
		goto ERROR_ENCOUNTERED;
	}

	if (FAILED(hr = pThing->get_ObjectProperty(bstrPropertyName, (IObjectProperty **) &pOP)))
		goto ERROR_ENCOUNTERED;

	if (pOP)
	{
		// ok, we know it's an object property
		// now, which kind is it?
		hr = pOP->QueryInterface( IID_IThing, (LPVOID*) &pPropThing);
		if (SUCCEEDED(hr) && pPropThing)
		{
			strType.LoadString(IDS_TYPE_THING);
			goto ERROR_ENCOUNTERED;
		}

		hr = pOP->QueryInterface( IID_IVector, (LPVOID*) &pVec);
		if (SUCCEEDED(hr) && pVec)
		{
			strType.LoadString(IDS_TYPE_VECTOR);
			goto ERROR_ENCOUNTERED;
		}

		hr = pOP->QueryInterface( IID_IVWGeometry, (LPVOID*) &pGeom);
		if (SUCCEEDED(hr) && pGeom)
		{
			strType.LoadString(IDS_TYPE_GEOMETRY);
			goto ERROR_ENCOUNTERED;
		}

		hr = pOP->QueryInterface( IID_IMethod, (LPVOID*) &pMethod);
		if (SUCCEEDED(hr) && pMethod)
		{
			if (FAILED(hr = pMethod->get_IsInline(&vBool)))
				goto ERROR_ENCOUNTERED;	
			if (vBool == VARIANT_TRUE)
				strType.LoadString(IDS_TYPE_INLINE_METHOD);
			else
				strType.LoadString(IDS_TYPE_METHOD);
			goto ERROR_ENCOUNTERED;
		}
	}

	// use the property security slot because the current slot is empty (or doesn't contain an objectproperty)
	if (FAILED(hr = pThing->get_PropertySecurity(bstrPropertyName, (IPropertySecurity **) &pSec)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pSec->get_PropertyIIDExt(&ID)))
		goto ERROR_ENCOUNTERED;

	if (IsEqualGUID(IID_IMethod, ID))
		strType.LoadString(IDS_TYPE_INLINE_METHOD);
	else if (IsEqualGUID(IID_IThing, ID))
		strType.LoadString(IDS_TYPE_THING);
	else if (IsEqualGUID(IID_IVector, ID))
		strType.LoadString(IDS_TYPE_VECTOR);
	else if (IsEqualGUID(IID_IVWGeometry, ID))
		strType.LoadString(IDS_TYPE_GEOMETRY);
	else if (IsEqualGUID(IID_IObjectProperty, ID))
		strType.LoadString(IDS_TYPE_OBJECT_PROPERTY);
	else
		strType.LoadString(IDS_TYPE_OBJECT);

ERROR_ENCOUNTERED:
	*bstrType = strType.AllocSysString();

	return hr;
}


STDMETHODIMP CStudioExemplarObject::OnGravityChanged(long lHint, VARIANT varHintData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing>				pThis;
	VARIANT_BOOL				bGravity;
	HRESULT						hr = S_OK;
	static CComBSTR				bstrGravity("Gravity");

	if (lHint != thPut)
		return S_OK;

	hr = m_pWorld->get_This(&pThis);
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;

	hr = pThis->get_BOOL(bstrGravity, &bGravity);
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;

	m_pWorld->FireUIEvent(pThis, CComBSTR(VWSTUDIO_GRAVITYCHANGED_EVENT_STR), CComVariant((bool)bGravity));
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;
	

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CStudioExemplarObject::OnEditingModeChanged(long lHint, VARIANT varHintData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing>				pThis;
	long						nEditingMode;
	HRESULT						hr = S_OK;
	static CComBSTR				bstrEditingMode("EditingMode");

	if (lHint != thPut)
		return S_OK;

	hr = m_pWorld->get_This(&pThis);
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;

	hr = pThis->get_Long(bstrEditingMode, &nEditingMode);
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;

	m_pWorld->FireUIEvent(pThis, CComBSTR(VWSTUDIO_EDITINGMODECHANGED_EVENT_STR), CComVariant( nEditingMode ));
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;
	

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CStudioExemplarObject::OnCollisionDetectionChanged(long lHint, VARIANT varHintData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing>				pThis;
	VARIANT_BOOL				bCollision;
	HRESULT						hr = S_OK;
	static CComBSTR				bstrCollisionDetection("CollisionDetection");

	if (lHint != thPut)
		return S_OK;

	hr = m_pWorld->get_This(&pThis);
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;

	hr = pThis->get_BOOL(bstrCollisionDetection, &bCollision);
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;

	m_pWorld->FireUIEvent(pThis, CComBSTR(VWSTUDIO_COLLISIONDETECTIONCHANGED_EVENT_STR), CComVariant((bool)bCollision) );
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;
	
ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CStudioExemplarObject::OnSelectionLockChanged(long lHint, VARIANT varHintData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing>				pThis;
	VARIANT_BOOL				bSelectionLock;
	HRESULT						hr = S_OK;
	static CComBSTR				bstrSelectionLock("SelectionLock");

	if (lHint != thPut)
		return S_OK;

	hr = m_pWorld->get_This(&pThis);
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;

	hr = pThis->get_BOOL(bstrSelectionLock, &bSelectionLock);
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;

	m_pWorld->FireUIEvent(pThis, CComBSTR(VWSTUDIO_SELECTIONLOCKCHANGED_EVENT_STR), CComVariant((bool)bSelectionLock) );
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;
	
ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CStudioExemplarObject::OnCameraFollowsObjectChanged(long lHint, VARIANT varHintData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing>				pThis;
	VARIANT_BOOL				bCameraFollowsObject;
	HRESULT						hr = S_OK;
	static CComBSTR				bstrCameraFollowsObject("CameraFollowsObject");

	if (lHint != thPut)
		return S_OK;

	hr = m_pWorld->get_This(&pThis);
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;

	hr = pThis->get_BOOL(bstrCameraFollowsObject, &bCameraFollowsObject);
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;

	m_pWorld->FireUIEvent(pThis, CComBSTR(VWSTUDIO_CAMERAFOLLOWSOBJECTCHANGED_EVENT_STR), CComVariant((bool)bCameraFollowsObject) );
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;
	
ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CStudioExemplarObject::GetIIDFromType(BSTR bstrType, BSTR *bstrIID)
{
	HRESULT hr = S_OK;
	CString strType;
	CString strCompare;
	CString strGUID;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	strType = bstrType;
	strCompare.LoadString(IDS_TYPE_INLINE_METHOD);
	if (strType.CompareNoCase(strCompare) == 0)
		strGUID = VWStringFromCLSID(IID_IMethod);
	
	strCompare.LoadString(IDS_TYPE_THING);
	if (strType.CompareNoCase(strCompare) == 0)
		strGUID = VWStringFromCLSID(IID_IThing);

	strCompare.LoadString(IDS_TYPE_VECTOR);
	if (strType.CompareNoCase(strCompare) == 0)
		strGUID = VWStringFromCLSID(IID_IVector);

	strCompare.LoadString(IDS_TYPE_GEOMETRY);
	if (strType.CompareNoCase(strCompare) == 0)
		strGUID = VWStringFromCLSID(IID_IVWGeometry);

	strCompare.LoadString(IDS_TYPE_OBJECT_PROPERTY);
	if (strType.CompareNoCase(strCompare) == 0)
		strGUID = VWStringFromCLSID(IID_IObjectProperty);

	*bstrIID = strGUID.AllocSysString();

	return hr;
}