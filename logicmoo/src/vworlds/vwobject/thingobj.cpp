// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// ThingObj.cpp : Implementation of CfooApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include "thingobj.h"
#include "worldobj.h"
#include "propsecu.h"
#include "methdobj.h"
#include "syshelp.h"

#define SUPPORT_SECURITY_OVERRIDE

#define SUPPORT_ONINVOKEMETHODFAILED

#define SUPPORT_CHECKPROPERTYSECURITY
#define SUPPORT_INVOKEONPROPERTYACCESS

#define SUPPORT_NOTIFYPROPERTYCHANGED
#define SUPPORT_INVOKEONPROPERTYCHANGED
#define SUPPORT_NOTIFYMETHODCHANGED

//#define DISABLE_REMOVE_INTERNALLYADDED

/////////////////////////////////////////////////////////////
// Macros for dealing with "doctored" dispid's for
// direct method/property access

// use VWT_NONE to always trace, VWT_METHOD is normal setting

//#define MOVEINTO_DEBUG
//#define MOVEINTO_PERF_DEBUG

//#define MOVEINTO_TRACE VWT_NONE
#define MOVEINTO_TRACE VWT_METHOD

//#define THING_USER_DEBUG

//#define METHOD_TRACE VWT_NONE
#define METHOD_TRACE VWT_METHOD

//#define PROPERTY_TRACE VWT_NONE
#define PROPERTY_TRACE VWT_METHOD

//#define NOTIFY_TRACE VWT_NONE
#define NOTIFY_TRACE VWT_METHOD

#if 0
// force debugging of IThing creation/destruction
#undef TRACE_ALLOCATION

#define TRACE_ALLOCATION VWT_NONE
//#define TRACE_ALLOCATION VWT_METHOD
#endif

#define TRACE_PROPERTYBIT_CACHING

// NOTE! idMinDoctored must be even!
#define idMinDoctored	10000
#define idMaxDoctored	20000
#define IsProperty(id)	(!(id & 1))
#define IsMethod(id)	(id & 1)

EXTERN_C const CLSID CLSID_PropertyMap;

typedef CVWComPtr<IPropertyMap, &CLSID_PropertyMap, &IID_IPropertyMap> CComPropertyMapPtr;
typedef CVWComPtr<IObjectProperty, NULL, &IID_IObjectProperty> CObjectPropertyPtr;
typedef CVWComPtr<IMethod, NULL, &IID_IMethod> CMethodPtr;

typedef CComObject<CPropertySecurity> CComPropertySecurity;

// Utility functions
HRESULT VariantToLong(VARIANT var, long* dest, long defvalue);
HRESULT VariantToBSTR(VARIANT var, BSTR* dest, BSTR defvalue);

HRESULT IsRoomInhabited(IThing* pRoom, VARIANT_BOOL* pbInhabited);

#ifdef _DEBUG
CString TraceProperty(VARIANT var);
#endif

// Static data members, out to be const but cannot properly initialize if so...
CComBSTR CThingObject::m_bstrParentEvent;
CComBSTR CThingObject::m_bstrChildEvent;
CComBSTR CThingObject::m_bstrSelfEvent;
CComBSTR CThingObject::m_bstrPeerEvent;

CString CThingObject::m_strOn;
CString CThingObject::m_strChanged;
CString CThingObject::m_strAccess;

CComBSTR CThingObject::m_bstrOn;
CComBSTR CThingObject::m_bstrChanged;
CComBSTR CThingObject::m_bstrAccess;
CComBSTR CThingObject::m_bstrName;
CComBSTR CThingObject::m_bstrContents;
CComBSTR CThingObject::m_bstrContainer;
CComBSTR CThingObject::m_bstrDescription;
//CComBSTR CThingObject::m_bstrOwner;
CComBSTR CThingObject::m_bstrWielded;
 
CComBSTR CThingObject::m_bstrEnter;
CComBSTR CThingObject::m_bstrLeave;
CComBSTR CThingObject::m_bstrSystemEnter;
CComBSTR CThingObject::m_bstrSystemLeave;

CComBSTR CThingObject::m_bstrIsExemplar;
CComBSTR CThingObject::m_bstrIsFertile;
CComBSTR CThingObject::m_bstrIsCloneable;
CComBSTR CThingObject::m_bstrIsOpen;
CComBSTR CThingObject::m_bstrIsNoticeable;
CComBSTR CThingObject::m_bstrThing;

IMPLEMENT_OBJECTPROPERTY(CThingObject)

/////////////////////////////////////////////////////////////////////////////
//

CThingObject::CThingObject() : 
	m_pExemplar(NULL), m_pProperties(NULL), m_pMethods(NULL), m_pOwner(NULL),
	m_lObjectID(0), m_bStub(VARIANT_TRUE), m_pArgList(NULL),
	m_idNext(idMinDoctored), m_idCached(DISPID_UNKNOWN)
{ 
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CThingObject::CThingObject, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

#ifdef SUPPORT_DATESTAMPS
	SetCreatedDate();
#endif

	// pre-load commonly used strings from resources
	// if they aren't loaded yet
	// cast to over-ride the const declaration
	if ((BSTR)m_bstrName == NULL)
	{
		CString strRESOURCE;

		strRESOURCE.LoadString(IDS_NAME_PROP);
		m_bstrName = strRESOURCE;

		strRESOURCE.LoadString(IDS_CONTENTS_PROP);
		m_bstrContents = strRESOURCE;

		strRESOURCE.LoadString(IDS_CONTAINER_PROP);
		m_bstrContainer = strRESOURCE;

		strRESOURCE.LoadString(IDS_WIELDED_PROP);
		m_bstrWielded = strRESOURCE;

		strRESOURCE.LoadString(IDS_ISFERTILE_PROP);
		m_bstrIsFertile = strRESOURCE;

		strRESOURCE.LoadString(IDS_ISCLONEABLE_PROP);
		m_bstrIsCloneable = strRESOURCE;

		strRESOURCE.LoadString(IDS_ISEXEMPLAR_PROP);
		m_bstrIsExemplar = strRESOURCE;

		strRESOURCE.LoadString(IDS_ISOPEN_PROP);
		m_bstrIsOpen = strRESOURCE;

		strRESOURCE.LoadString(IDS_ISNOTICEABLE_PROP);
		m_bstrIsNoticeable = strRESOURCE;

		strRESOURCE.LoadString(IDS_DESCRIPTION_PROP);
		m_bstrDescription = strRESOURCE;

//		strRESOURCE.LoadString(IDS_OWNER_PROP);
//		m_bstrOwner = strRESOURCE;	

		strRESOURCE.LoadString(IDS_PROPCHANGE_ON);
		m_bstrOn = strRESOURCE;

		strRESOURCE.LoadString(IDS_PROPCHANGE_CHANGED);
		m_bstrChanged = strRESOURCE;

		strRESOURCE.LoadString(IDS_PROPCHANGE_ACCESS);
		m_bstrAccess = strRESOURCE;

		strRESOURCE.LoadString(IDS_PARENT_EVENT);
		m_bstrParentEvent = strRESOURCE;

		strRESOURCE.LoadString(IDS_CHILD_EVENT);
		m_bstrChildEvent = strRESOURCE;

		strRESOURCE.LoadString(IDS_SELF_EVENT);
		m_bstrSelfEvent = strRESOURCE;
		  
		strRESOURCE.LoadString(IDS_PEER_EVENT);
		m_bstrPeerEvent = strRESOURCE;

		strRESOURCE.LoadString(IDS_ENTER_EVENT);
		m_bstrEnter = strRESOURCE;

		strRESOURCE.LoadString(IDS_LEAVE_EVENT);
		m_bstrLeave = strRESOURCE;

		strRESOURCE.LoadString(IDS_SYSTEMENTER_EVENT);
		m_bstrSystemEnter = strRESOURCE;

		strRESOURCE.LoadString(IDS_SYSTEMLEAVE_EVENT);
		m_bstrSystemLeave = strRESOURCE;

		strRESOURCE.LoadString(IDS_THING_TYPE);
		m_bstrThing = strRESOURCE;

		m_strOn = m_bstrOn;
		m_strAccess = m_bstrAccess;
		m_strChanged = m_bstrChanged;
	}

	// default thing type to 'instance'
	m_bstrType = m_bstrThing;
}

CThingObject::~CThingObject()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CThingObject::~CThingObject, id %d, this 0x%0x\n", m_lObjectID, this);
#endif

	Terminate();
}

STDMETHODIMP CThingObject::Terminate(void)
{			 
	HRESULT hr = S_OK;
	CComVariant var;
	VARIANT_BOOL bDone;
	CComBSTR bstrName;
	CMethodPtr methodPtr;
	
#ifdef _DEBUG
	CComBSTR bstrObjectName("<stub>");
	if (m_bStub == VARIANT_FALSE && m_pProperties != NULL)
		/* hr = */ get_Name(&bstrObjectName.m_str);

	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CThingObject::Terminate: (begin) id %d, name (%s), refcount %d, this 0x%0x\n", m_lObjectID, CString(bstrObjectName), m_dwRef, this);
#endif

	SAFERELEASE(m_pExemplar);

	if (m_pMethods)
	{
		// iterate through the method map
		hr = m_pMethods->FirstItem(&bstrName.m_str, &var, &bDone);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::Terminate: IPropertyMap::FirstItem (methods) failed, hr %x\n", hr);
			goto ERROR_ENCOUNTERED_1;
		}

		while (bDone != VARIANT_TRUE)
		{
#ifdef _DEBUG
//			VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CThingObject::Terminate: clearing method (%s)\n", CString(bstrName));
#endif

			methodPtr = var;

			// clear back-ptr and prop name in methods
			if (methodPtr != NULL)
				/* hr = */ ClearMethodName(methodPtr);

			// free stuff
			bstrName.Empty();
			var.Clear();
			methodPtr.Release();

			// on to the next one
			hr = m_pMethods->NextItem(&bstrName.m_str, &var, &bDone);
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::Terminate: IPropertyMap::NextItem (methods) failed, hr %x\n", hr);
				break;
			}
		}

ERROR_ENCOUNTERED_1:
		m_pMethods->Terminate();

		SAFERELEASE(m_pMethods);
	}

	// release methods first, since they might reference modules
	// included in the property map (only for the Global object)
	if (m_MethodSecurity != NULL)
	{
		// release any mutual back pointers with PropertyMap
		m_MethodSecurity->Cleanup();
		m_MethodSecurity.Release();
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CThingObject::Terminate: (methods) id %d, name (%s), refcount %d, this 0x%0x\n", m_lObjectID, CString(bstrObjectName), m_dwRef, this);
#endif

	if (m_pProperties)
	{
		// iterate through the properties map
		hr = m_pProperties->FirstItem(&bstrName.m_str, &var, &bDone);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::Terminate: IPropertyMap::FirstItem (properties) failed, hr %x\n", hr);
			goto ERROR_ENCOUNTERED_2;
		}

		while (bDone != VARIANT_TRUE)
		{
#ifdef _DEBUG
//			VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CThingObject::Terminate: clearing property (%s)\n", CString(bstrName));
#endif

			// terminate object properties
			/* hr = */ ClearPropertyName(var);

			// free stuff
			bstrName.Empty();
			var.Clear();

			// on to the next one
			hr = m_pProperties->NextItem(&bstrName.m_str, &var, &bDone);
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::Terminate: IPropertyMap::NextItem (properties) failed, hr %x\n", hr);
				break;
			}
		}

ERROR_ENCOUNTERED_2:
		m_pProperties->Terminate();

		SAFERELEASE(m_pProperties);
	}

	// propsecurity cleanup should happen after freeing method map
	// and property map pointers
	if (m_PropSecurity != NULL)
	{
		// release any mutual back pointers with PropertyMap
		m_PropSecurity->Cleanup();
		m_PropSecurity.Release();
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CThingObject::Terminate: (properties) id %d, name (%s), refcount %d, this 0x%0x\n", m_lObjectID, CString(bstrObjectName), m_dwRef, this);
#endif

	SAFERELEASE(m_pOwner);

	if (m_pArgList != NULL)
	{
		m_pArgList->RemoveAll();
		SAFERELEASE(m_pArgList);
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CThingObject::Terminate: (finished) id %d, name (%s), refcount %d, this 0x%0x\n", m_lObjectID, CString(bstrObjectName), m_dwRef, this);
#endif

	CThingObjectBase::Terminate();

	return S_OK;
}

STDMETHODIMP CThingObject::put_World(IWorld* pworld)
{
	HRESULT hr = S_OK;
	CComVariant var;
	CComPropertySecurity* pPropSec = NULL;
	CComPropertySecurity* pMethodSec = NULL;

	CThingObjectBase::put_World(pworld);

	ASSERT(m_pWorld);
	if (m_pWorld == NULL)
		return VWOBJECT_E_INVALIDWORLD;

	hr = CreatePropertyMap(m_pWorld, &m_pProperties);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	pPropSec = new CComPropertySecurity();
	if (pPropSec == NULL)
	{
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	pPropSec->AddRef();

	m_PropSecurity = pPropSec;
	ASSERT(m_PropSecurity != NULL);

	// REVIEW: initializing propsec and methodsec with themselves,
	// so they'll register as attached in the propsec code

	// REVIEW: init with RWA|RWA|RW so owners and exemplars and Add/put/get properties,
	// and other owners can just put/get properties
	
	// Init with a null exemplar so we can initialize its security
	m_PropSecurity->Initialize(pworld, this, m_PropSecurity);
	m_PropSecurity->put_Permissions(PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_RW);

	var.vt = VT_DISPATCH;
	var.pdispVal = m_pProperties;
	SAFEADDREF(m_pProperties);
	m_PropSecurity->put_Property(var);
	m_PropSecurity->Enable();
	var.Clear();

	hr = CreatePropertyMap(m_pWorld, &m_pMethods);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	pMethodSec = new CComPropertySecurity();
	if (pMethodSec == NULL)
	{
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	pMethodSec->AddRef();

	m_MethodSecurity = pMethodSec;
	ASSERT(m_MethodSecurity != NULL);

	// Set default permissions to Owner Read Write and ALL Read
	m_MethodSecurity->Initialize(pworld, this, m_MethodSecurity);
	m_MethodSecurity->put_Permissions(PS_OWNER_RWA | PS_EXEMPLAR_RWA | PS_ALL_RW);

	var.vt = VT_DISPATCH;
	var.pdispVal = m_pMethods;
	SAFEADDREF(m_pMethods);
	m_MethodSecurity->put_Property(var);
	m_MethodSecurity->Enable();
	var.Clear();

ERROR_ENCOUNTERED:
	SAFERELEASE(pPropSec);
	SAFERELEASE(pMethodSec);

	if (FAILED(hr))
	{
		SAFERELEASE(m_pProperties);
		SAFERELEASE(m_pMethods);
	}

	return hr;
}

STDMETHODIMP CThingObject::ToAscii(BSTR* pbstr)
{
	ASSERT(pbstr);
	if (pbstr == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::ToAscii: outgoing pointer (BSTR*) is NULL\n");
		return ReportThingError(E_POINTER);
	}

	if (m_bStub == VARIANT_TRUE)
	{
		CComBSTR bstr("(Stub)");
		*pbstr = bstr.Copy();
		return S_OK;
	}

	VARIANT_BOOL bIsExemplar = VARIANT_FALSE;
	HRESULT hr = S_OK;

	/* hr = */ get_IsExemplar(&bIsExemplar);

	// if exemplar, return type string for ToAscii
	if (bIsExemplar == VARIANT_TRUE)
		/* hr = */ get_Type(pbstr);
	else
		/* hr = */ get_Name(pbstr);

	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::Dump(void)
{
	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "Thing: begin\n");

	CString strType("unknown");
	BSTR bstr = NULL;

	if (SUCCEEDED(get_Type(&bstr)))
	{
		strType = bstr;
		SAFEFREESTRING(bstr);
	}

	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "Thing: ID (%d), type (%s), stub (%s)\n", 
		m_lObjectID, strType, (m_bStub == VARIANT_TRUE) ? "yes" : "no");

	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "Thing: properties\n");

	m_pProperties->Dump();

	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "Thing: methods\n");

	m_pMethods->Dump();

	if (m_pExemplar)
	{
		BSTR bstr = NULL;
		CString strName;

		if (SUCCEEDED(m_pExemplar->get_Name(&bstr)))
		{
			strName = bstr;
			SAFEFREESTRING(bstr);
		}

		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "Thing: exemplar (%s)\n", strName);
	}
	else
		VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "Thing: exemplar (null)\n");

	VWTRACE(m_pWorld, "VWOBJECT", VWT_DUMP, "Thing: end\n");

	return S_OK;
}

HRESULT CThingObject::FindWithinEx(IThing* pbase, BSTR bstrName, IThing** ppthing)
{
	HRESULT hr = S_OK;
	IPropertyList* pContents = NULL;

	// REVIEW: this does a depth-first search starting from this IThing;
	// maybe it would make more sense to do a breadth-first search.

	*ppthing = NULL;

	hr = pbase->get_Contents(&pContents);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (pContents)
	{
		IThing* pthing = NULL;
		long propcnt = 0;
		int i;
		VARIANT_BOOL bStub = VARIANT_FALSE;

		hr = pContents->get_Count(&propcnt);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED_1;

		for (i = 0; i < propcnt && *ppthing == NULL; i++)
		{
			if (SUCCEEDED(pContents->get_Thing(i, &pthing)) && pthing)
			{
				CComBSTR bstr;

				/* hr = */ pthing->get_IsStub(&bStub);

				if (bStub == VARIANT_FALSE)
				{
					hr = pthing->get_Name(&bstr.m_str);
					if (SUCCEEDED(hr))
					{
						// if passed-in name matches pthing's name, return
						if (CompareElements(&bstr, &bstrName))
						{
							*ppthing = pthing;
							SAFEADDREF(*ppthing);
						}
						// else, recurse into pthing and look for object there
						else
							/* hr = */ FindWithinEx(pthing, bstrName, ppthing);
					}
				}

				SAFERELEASE(pthing);
			}
		}

ERROR_ENCOUNTERED_1:
		SAFERELEASE(pContents);
	}

ERROR_ENCOUNTERED:
	// ignore HRESULT, just return ppthing
	return S_OK;
}

STDMETHODIMP CThingObject::FindWithin(BSTR bstrName, VARIANT varQuiet, IThing** ppthing)
{
	HRESULT hr = S_OK;
	CWorldObject* pWorld = (CWorldObject*)m_pWorld;

	ASSERT(bstrName);
	if (bstrName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::FindWithin: incoming argument (BSTR) is NULL\n");
		return ReportThingError(E_INVALIDARG);
	}

	ASSERT(ppthing);
	if (ppthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::FindWithin: outgoing pointer (IThing**) is NULL\n");
		return ReportThingError(E_POINTER);
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::FindWithin: stub object, shouldn't be accessing\n");
		hr = VWOBJECT_E_STUBOBJECT;
		goto ERROR_ENCOUNTERED;
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::FindWithin: name (%s)\n", CString(bstrName));
#endif

	/* hr = */ FindWithinEx(this, bstrName, ppthing);

	if (*ppthing == NULL)
	{
		// if optional arg not passed in, or if False not passed in, report failure
		if (varQuiet.vt == VT_ERROR || (varQuiet.vt == VT_BOOL && varQuiet.boolVal == VARIANT_FALSE))
		{
			CComBSTR bstrWithinName ("n/a");
			/* hr = */ get_Name(&bstrWithinName.m_str);

			VWREPORT(m_pWorld, VWR_ERROR, "Failed to find \"%s\" within \"%s\"\n", CString(bstrName), CString(bstrWithinName));
		}

		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::MoveIntoInt(IThing* pWhere)
{
	HRESULT hr = S_OK;
	CWorldObject* pWorld = (CWorldObject*)m_pWorld;
	VARIANT_BOOL bConnected = VARIANT_FALSE;
	IThing* pUser = NULL;
	IThing* pOldWhere = NULL;
	IThing* pWheresContainer = NULL;
	VARIANT_BOOL bInhabited = VARIANT_FALSE;
	DISPPARAMS dispparams;
	VARIANT_BOOL bIsAvatar = VARIANT_FALSE;
	VARIANT_BOOL bIsRoom = VARIANT_FALSE;
	VARIANT_BOOL bIsExemplar = VARIANT_FALSE;
	VARIANT_BOOL bIsEmpty = VARIANT_TRUE;

	// NOTE: from = pOldWhere, to = pWhere, who = this

	bConnected = pWorld->m_bConnected;

	// package up args into DISPPARAMS struct
	hr = InitializeDISPPARAMS(&dispparams, 2);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// can't move into self
	if (pWhere == this)
	{
		hr = VWOBJECT_E_INVALIDCONTAINER;
		goto ERROR_ENCOUNTERED;
	}

	// Make sure that where you are trying to move to (pWhere) isn't contained within the thing
	// e.g., check for pWhere's container's container etc.. and make sure it isn't the thing
	// (pWhere will be NULL when calling DestroyObject)

	if (pWhere)
	{
		hr = pWhere->get_Container(&pWheresContainer);
		
		while ((SUCCEEDED(hr)) && (pWheresContainer!=NULL))
		{
			if (pWheresContainer == this)
			{
				hr = VWOBJECT_E_INVALIDCONTAINER;
				goto ERROR_ENCOUNTERED;
			}
			else
				hr = pWheresContainer->get_Container(&pWheresContainer);
		}
	}


	pUser = pWorld->m_pUser;
	SAFEADDREF(pUser);

	if (pWhere != NULL)
	{
		hr = pWorld->IsAvatar(this, &bIsAvatar);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pWorld->IsRoom(pWhere, &bIsRoom);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pWorld->IsExemplar(pWhere, &bIsExemplar);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// can't put avatars anywhere but rooms, and can't put anything into an exemplar
		if ((bIsAvatar == VARIANT_TRUE && bIsRoom == VARIANT_FALSE) || 
			bIsExemplar == VARIANT_TRUE)
		{
			hr = VWOBJECT_E_INVALIDCONTAINER;
			goto ERROR_ENCOUNTERED;
		}
	}

	// get old container from this
	hr = get_Container(&pOldWhere);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

#ifdef MOVEINTO_DEBUG
	{
		CComBSTR bstr("n/a");
		CComBSTR bstrWhere("n/a");
		CComBSTR bstrOldWhere("n/a");
		CComBSTR bstrUser("n/a");

		/* hr = */ get_Name(&bstr.m_str);

		if (pWhere)
			/* hr = */ pWhere->get_Name(&bstrWhere.m_str);

		if (pOldWhere)
			/* hr = */ pOldWhere->get_Name(&bstrOldWhere.m_str);

		if (pUser)
			/* hr = */ pUser->get_Name(&bstrUser.m_str);

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", MOVEINTO_TRACE, "CThingObject::MoveIntoInt: moving %s from %s to %s, user is %s\n", 
			CString(bstr), CString(bstrOldWhere), CString(bstrWhere), CString(bstrUser));
#endif

#ifdef MOVEINTO_PERF_DEBUG
		VWREPORT(m_pWorld, VWR_PERFLOG, "MoveInto,%d,%s,%s,%s,%s\n",
			GetTickCount(), CString(bstr), CString(bstrOldWhere), CString(bstrWhere), CString(bstrUser));
#endif
	}
#endif

	// don't leave if container is NULL
	if (pOldWhere != NULL)
	{
		dispparams.rgvarg[1].vt = VT_DISPATCH;
		dispparams.rgvarg[1].pdispVal = pOldWhere;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = this;

		hr = FireEventExt(m_bstrLeave, &dispparams, FIREEVENT_DEFAULT);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// internal event (for graphics support)
		hr = FireEventExt(m_bstrSystemLeave, &dispparams, FIREEVENT_DEFAULT);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// if moving to same place, don't bother removing it
		if (pOldWhere != pWhere)
		{
			IPropertyList* pContents = NULL;

			// get pOldWhere contents
			hr = pOldWhere->get_Contents(&pContents);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			if (pContents)
			{
#ifdef _DEBUG
				{
					CComBSTR bstr("n/a");
					CComBSTR bstrOldWhere("n/a");

					/* hr = */ get_Name(&bstr.m_str);

					if (pOldWhere)
						/* hr = */ pOldWhere->get_Name(&bstrOldWhere.m_str);

					VWTRACE(m_pWorld, "VWOBJECT", MOVEINTO_TRACE, "CThingObject::MoveIntoInt: removing %s from %s\n", 
						CString(bstr), CString(bstrOldWhere));
				}
#endif

				// NOTE: if this == pUser, then pUser won't get updated with this change
				// because the user is removed from the contents before the bystanders
				// are updated

				{
					CSetUID SetUID(m_pWorld, this, this, VARIANT_TRUE);

					// delete this from pOldWhere contents
					hr = pContents->RemoveThing(this);
				}

				SAFERELEASE(pContents);

				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;

				// if object which is moving is the client avatar
				// then send client the old container
				if (this == pUser && bConnected == VARIANT_TRUE)
				{
#ifdef _DEBUG
					{
						CComBSTR bstrOldWhere("n/a");

						if (pOldWhere)
							/* hr = */ pOldWhere->get_Name(&bstrOldWhere.m_str);

						VWTRACE(m_pWorld, "VWOBJECT", MOVEINTO_TRACE, "CThingObject::MoveIntoInt: sending %s to client\n", 
							CString(bstrOldWhere));
					}
#endif

					// REVIEW: sending old container may not be necessary

					// send old container to client
					hr = m_pWorld->SendThing(soClient, pOldWhere);
					if (FAILED(hr))
						goto ERROR_ENCOUNTERED;

					// NOTE: avatar is leaving its container; check container
					// for remaining logged-in avatars; if none, deactivate room recursively
					hr = IsRoomInhabited(pOldWhere, &bInhabited);
					if (FAILED(hr))
						goto ERROR_ENCOUNTERED;

					if (bInhabited == VARIANT_FALSE)
					{
#ifdef THING_USER_DEBUG
						{
							CComBSTR bstrOldWhere("n/a");

							if (pOldWhere)
								/* hr = */ pOldWhere->get_Name(&bstrOldWhere.m_str);

							VWTRACE(m_pWorld, "VWOBJECT", VWT_NONE, "CThingObject::MoveIntoInt: deactivating %s\n", 
								CString(bstrOldWhere));
						}
#endif

						hr = m_pWorld->DeactivateRoom(pOldWhere);
						if (FAILED(hr))
							goto ERROR_ENCOUNTERED;
					}
				}
			}

#ifdef _DEBUG
			{
				CComBSTR bstr("n/a");

				/* hr = */ get_Name(&bstr.m_str);

				VWTRACE(m_pWorld, "VWOBJECT", MOVEINTO_TRACE, "CThingObject::MoveIntoInt: setting container of %s to NULL\n", 
					CString(bstr));
			}
#endif

			// set new container to NULL (no container until actually moved)
			// set security to be this
			{
				CSetUID SetUID(m_pWorld, this, this, VARIANT_TRUE);

				hr = put_Container(NULL);
			}

			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

	// don't enter if new container is NULL
	if (pWhere != NULL)
	{
		// if moving to same place, don't bother removing it
		if (pOldWhere != pWhere)
		{
			IPropertyList* pContents = NULL;

			// if object which is moving is the client avatar
			// or we're doing the MoveInto during login (pUser == NULL)
			// then send client the new container
			if ((pUser == this || pUser == NULL) && bConnected == VARIANT_TRUE)
			{
#ifdef _DEBUG
				{
					CComBSTR bstrWhere("n/a");

					if (pWhere)
						/* hr = */ pWhere->get_Name(&bstrWhere.m_str);

					VWTRACE(m_pWorld, "VWOBJECT", MOVEINTO_TRACE, "CThingObject::MoveIntoInt: sending %s to client\n", 
						CString(bstrWhere));
				}
#endif

				// send container to client along with its contents
				hr = m_pWorld->SendContainer(soClient, pWhere);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;

				// NOTE: avatar is entering a new container; check container
				// for logged-in avatars; if none, activate room recursively
				hr = IsRoomInhabited(pWhere, &bInhabited);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;

				if (bInhabited == VARIANT_FALSE)
				{
#ifdef THING_USER_DEBUG
					{
						CComBSTR bstrWhere("n/a");

						if (pWhere)
							/* hr = */ pWhere->get_Name(&bstrWhere.m_str);

						VWTRACE(m_pWorld, "VWOBJECT", VWT_NONE, "CThingObject::MoveIntoInt: activating %s\n", 
							CString(bstrWhere));
					}
#endif

					hr = m_pWorld->ActivateRoom(pWhere);
					if (FAILED(hr))
						goto ERROR_ENCOUNTERED;
				}
			}

			// get pWhere contents
			hr = pWhere->get_Contents(&pContents);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			if (pContents)
			{
#ifdef _DEBUG
				{
					CComBSTR bstr("n/a");
					CComBSTR bstrWhere("n/a");

					/* hr = */ get_Name(&bstr.m_str);

					if (pWhere)
						/* hr = */ pWhere->get_Name(&bstrWhere.m_str);

					VWTRACE(m_pWorld, "VWOBJECT", MOVEINTO_TRACE, "CThingObject::MoveIntoInt: adding %s to %s\n", 
						CString(bstr), CString(bstrWhere));
				}
#endif

				// add this to pWhere contents
				{
					// Change the Security to the pWhere
					CSetUID SetUID(m_pWorld, pWhere, pWhere, VARIANT_TRUE);

					hr = pContents->AddThing(this);
				}

				SAFERELEASE(pContents);

				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;
			}

#ifdef _DEBUG
			{
				CComBSTR bstr("n/a");
				CComBSTR bstrWhere("n/a");

				/* hr = */ get_Name(&bstr.m_str);

				if (pWhere)
					/* hr = */ pWhere->get_Name(&bstrWhere.m_str);

				VWTRACE(m_pWorld, "VWOBJECT", MOVEINTO_TRACE, "CThingObject::MoveIntoInt: setting container of %s to %s\n", 
					CString(bstr), CString(bstrWhere));
			}
#endif

			// REVIEW: set container locally, so bystanders work;
			// and then we'll set it remotely so clients have 'this' pushed to them already

			// set new container to pWhere 
			// change security to be this 
			{
				CSetUID SetUID(m_pWorld, this, this, VARIANT_TRUE);

				// set the property in local mode (don't remote)
				VARIANT_BOOL bLocalMode = pWorld->m_bLocalMode;

				pWorld->m_bLocalMode = VARIANT_TRUE;

				hr = put_Container(pWhere);

				pWorld->m_bLocalMode = bLocalMode;
			}

			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			if (bConnected == VARIANT_TRUE)
			{
#ifdef _DEBUG
				{
					CComBSTR bstr("n/a");

					/* hr = */ get_Name(&bstr.m_str);

					VWTRACE(m_pWorld, "VWOBJECT", MOVEINTO_TRACE, "CThingObject::MoveIntoInt: sending %s (including contents) to %s\n", 
						CString(bstr), (this == pUser) ? "bystanders excluding user" : "bystanders");
				}
#endif

				// REVIEW: if pUser is NULL, then this MoveInto is occurring during logon
				// and it must be the user

				// update all bystanders with this;
				// skip sending user to itself if user is moving (not doing this since we have to refresh props)

				// REVIEW: need to send contents to user to refresh not-remoted properties (force user open)
				// Actually send to all bystanders since this may be IsOpen=TRUE.

				hr = m_pWorld->SendContents(soBystanders, this, VARIANT_TRUE);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;

				hr = m_pWorld->SendThing((/* this == pUser || */ NULL == pUser) ? soBystandersExclUser : soBystanders, this);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;
			}

			// set new container to pWhere 
			// change security to be this 
			{
				CSetUID SetUID(m_pWorld, this, this, VARIANT_TRUE);

				hr = put_Container(pWhere);
			}

			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}

		dispparams.rgvarg[1].vt = VT_DISPATCH;
		dispparams.rgvarg[1].pdispVal = pWhere;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = this;

		{
			// internal event (for graphics support)
			// set security context - Review: do we want pUser or this?
			CSetUID SetUID(m_pWorld, pUser, pUser /*, VARIANT_TRUE */);

			hr = FireEventExt(m_bstrSystemEnter, &dispparams, FIREEVENT_DEFAULT);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			hr = FireEventExt(m_bstrEnter, &dispparams, FIREEVENT_DEFAULT);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

	SAFERELEASE(pOldWhere);
	SAFERELEASE(pUser);

	return hr;
}

STDMETHODIMP CThingObject::MoveIntoExt(IThing* pWhere)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::MoveIntoExt\n");
#endif

	m_pWorld->get_ShouldExecuteSync(&bShouldExecute);

	// NOTE: using ShouldRemoteSync (only remote on client-side)
	m_pWorld->get_ShouldRemoteSync(&bShouldRemote);

	if (bShouldExecute == VARIANT_TRUE)
	{
		hr = MoveIntoInt(pWhere);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		HRESULT senthr = S_OK;
		CComVariant var;
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = pWhere;

		hr = SendSyncThingCommand(PSBIT_NONE, DISPID_THING_MOVEINTOINT, DISPATCH_METHOD, &dispparams, &senthr, &var);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	
		// if sendmessage and invoke both succeeded
		if (SUCCEEDED(hr) && SUCCEEDED(senthr))
		{
		}
	}

ERROR_ENCOUNTERED:
	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::get_InstanceType(BSTR* pbstr)
{
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::get_InstanceType\n");

	ASSERT(pbstr);
	if (pbstr == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_InstanceType: outgoing pointer (BSTR*) is NULL\n");
		return ReportThingError(E_POINTER);
	}

	*pbstr = m_bstrType.Copy();

	return S_OK;
}

STDMETHODIMP CThingObject::get_Type(BSTR* pbstr)
{
	HRESULT hr = S_OK;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::get_Type\n");

	ASSERT(pbstr);
	if (pbstr == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_Type: outgoing pointer (BSTR*) is NULL\n");
		return ReportThingError(E_POINTER);
	}

	*pbstr = NULL;

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_Type: stub object, shouldn't be accessing\n");
		return ReportThingError(VWOBJECT_E_STUBOBJECT);
	}

	// walk exemplar chain until we find unique type string (skip thing type)
	if (m_bStub == VARIANT_TRUE || !CompareElements(&m_bstrThing.m_str, &m_bstrType.m_str))
		*pbstr = m_bstrType.Copy();
	else if (m_pExemplar != NULL)
		hr = m_pExemplar->get_Type(pbstr);
	else
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_Type: object type is invalid\n");
		return ReportThingError(VWOBJECT_E_UNKNOWNTYPE);
	}

	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::IsOfType(BSTR bstrType, VARIANT_BOOL* pboolVal)
{
	ASSERT(bstrType);
	if (bstrType == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::IsOfType: incoming argument (BSTR) is NULL\n");
		return ReportThingError(E_INVALIDARG);
	}

	ASSERT(pboolVal);
	if (pboolVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::IsOfType: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return ReportThingError(E_POINTER);
	}

	*pboolVal = VARIANT_FALSE;

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::IsOfType: stub object, shouldn't be accessing\n");
		return ReportThingError(VWOBJECT_E_STUBOBJECT);
	}

	if (CompareElements(&bstrType, &m_bstrType.m_str))
		*pboolVal = VARIANT_TRUE;
	else if (m_bStub == VARIANT_FALSE && m_pExemplar) 
		m_pExemplar->IsOfType(bstrType, pboolVal);

	return S_OK; 
}

STDMETHODIMP CThingObject::put_Type(BSTR bstrType) 
{ 
	HRESULT hr = S_OK;

	ASSERT(bstrType);
	if (bstrType == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::put_Type: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::put_Type: stub object, shouldn't be accessing\n");
		return ReportThingError(VWOBJECT_E_STUBOBJECT);
	}

	return ReportThingError(VWOBJECT_E_PERMISSION_DENIED); 
};

STDMETHODIMP CThingObject::put_TypeInt(BSTR bstrType)
{ 
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	ASSERT(bstrType);
	if (bstrType == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::put_TypeInt: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::put_TypeInt\n");
#endif

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::put_TypeInt: stub object, shouldn't be accessing\n");
		return VWOBJECT_E_STUBOBJECT;
	}

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldExecute == VARIANT_TRUE)
	{
		m_bstrType = bstrType;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstrType;

		// log only, don't remote
		hr = LogThingCommand(DISPID_THING_TYPEINT, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

ERROR_ENCOUNTERED:
	return ReportThingError(hr);
}

// put_OwnerInt: OK to have NULL owner when first doing install
STDMETHODIMP CThingObject::put_OwnerInt(IThing* pThing)
{ 
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::put_OwnerInt\n");
#endif

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::put_OwnerInt: stub object, shouldn't be accessing\n");
		return VWOBJECT_E_STUBOBJECT;
	}

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldExecute == VARIANT_TRUE)
	{
		SAFERELEASE(m_pOwner);
		m_pOwner = pThing;
		SAFEADDREF(m_pOwner);
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = pThing;

		// log only, don't remote
		hr = LogThingCommand(DISPID_THING_OWNERINT, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

ERROR_ENCOUNTERED:
	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::put_IsStubInt(VARIANT_BOOL boolVal)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::put_IsStubInt\n");
#endif

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldExecute == VARIANT_TRUE)
	{
		m_bStub = boolVal;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BOOL;
		dispparams.rgvarg[0].boolVal = boolVal;

		// log only, don't remote
		hr = LogThingCommand(DISPID_THING_ISSTUBINT, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

ERROR_ENCOUNTERED:
	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::get_Exemplar(IThing** ppthing)
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::get_Exemplar\n");
#endif

	ASSERT(ppthing);
	if (ppthing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_Exemplar: outgoing pointer (IThing**) is NULL\n");
		return ReportThingError(E_POINTER);
	}

	*ppthing = NULL;

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_Exemplar: stub object, shouldn't be accessing\n");
		return ReportThingError(VWOBJECT_E_STUBOBJECT);
	}

	*ppthing = m_pExemplar;
	SAFEADDREF(*ppthing);

	return S_OK;
}

STDMETHODIMP CThingObject::put_ExemplarInt(IThing* pthing)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::put_ExemplarInt\n");
#endif

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::put_ExemplarInt: stub object, shouldn't be accessing\n");
		hr = VWOBJECT_E_STUBOBJECT;
		goto ERROR_ENCOUNTERED;
	}

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldExecute == VARIANT_TRUE)
	{
		SAFERELEASE(m_pExemplar);
		m_pExemplar = pthing;
		SAFEADDREF(m_pExemplar);
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = pthing;

		// Log and remote (used to be log only, don't remote - but then avatar/host change weren't remoted correctly)
		hr = SendThingCommand(PSBIT_NONE, DISPID_THING_EXEMPLARINT, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

ERROR_ENCOUNTERED:
	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::get_InstanceProperties(IPropertyMap** pppropertymap)
{
	HRESULT hr = S_OK;

	ASSERT(pppropertymap);
	if (pppropertymap == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_InstanceProperties: outgoing pointer (IPropertyMap**) is NULL\n");
		return ReportThingError(E_POINTER);
	}

	*pppropertymap = NULL;

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::get_InstanceProperties: stub object, shouldn't be accessing\n");
		hr = VWOBJECT_E_STUBOBJECT;
		goto ERROR_ENCOUNTERED;
	}

	// allow return of NULL propmap
#if 0
	if (m_pProperties == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::get_InstanceProperties: property map is invalid\n");
		hr = VWOBJECT_E_INVALIDPROPERTYMAP;
		goto ERROR_ENCOUNTERED;
	}
#endif

	*pppropertymap = m_pProperties;

	SAFEADDREF(*pppropertymap);

ERROR_ENCOUNTERED:
	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::get_Properties(IPropertyMap** pppropertymap)
{
	HRESULT hr = S_OK;

	ASSERT(pppropertymap);
	if (pppropertymap == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_Properties: outgoing pointer (IPropertyMap**) is NULL\n");
		return ReportThingError(E_POINTER);
	}

	*pppropertymap = NULL;

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::get_Properties: stub object, shouldn't be accessing\n");
		hr = VWOBJECT_E_STUBOBJECT;
		goto ERROR_ENCOUNTERED;
	}

	// allow return of NULL propmap
#if 0
	if (m_pProperties == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::get_Properties: property map is invalid\n");
		hr = VWOBJECT_E_INVALIDPROPERTYMAP;
		goto ERROR_ENCOUNTERED;
	}
#endif

	hr = CreatePropertyMap(m_pWorld, pppropertymap);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (m_pProperties != NULL)
	{
		hr = CopyPropertiesTo(*pppropertymap);
		if (FAILED(hr))
		{
			SAFERELEASE(*pppropertymap);
			goto ERROR_ENCOUNTERED;
		}

		hr = m_pProperties->CopyTo(*pppropertymap);
	}

ERROR_ENCOUNTERED:
	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::CopyPropertiesTo(IPropertyMap* ppropertymap)
{
	HRESULT hr = S_OK;

	ASSERT(ppropertymap);
	if (ppropertymap == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::CopyPropertiesTo: incoming argument (IPropertyMap*) is NULL\n");
		return E_INVALIDARG;
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::CopyPropertiesTo: stub object, shouldn't be accessing\n");
		hr = VWOBJECT_E_STUBOBJECT;
		goto ERROR_ENCOUNTERED;
	}

	if (m_pProperties == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::CopyPropertiesTo: property map is invalid\n");
		hr = VWOBJECT_E_INVALIDPROPERTYMAP;
		goto ERROR_ENCOUNTERED;
	}

	if (m_pExemplar)
	{
		hr = m_pExemplar->CopyPropertiesTo(ppropertymap);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	hr = m_pProperties->CopyTo(ppropertymap);

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CThingObject::AddProperty(BSTR bstrPropertyName, VARIANT var, VARIANT VarFlags, 
	VARIANT VarPermissions, VARIANT VarType, BSTR bstrIID, BSTR bstrType)
{
	// Just do an add property with the defaults defined in the idl
	long psbits = PSBIT_SCRIPTDEFAULTPROPERTY;
	long permissions = PS_SCRIPTDEFAULTPROPERTY;
	long vtType = VT_VARIANT;	
	HRESULT hr = S_OK;
	CWorldObject* pWorld = (CWorldObject*)m_pWorld;
	IThing* pExemplar = NULL;

	hr = VariantToLong(VarFlags, &psbits, psbits);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
	hr = VariantToLong(VarPermissions, &permissions, permissions);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = VariantToLong(VarType, &vtType, vtType);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (m_pWorld)
	{
		// figure out who is calling and set the exemplar		
		if ((pWorld->m_pSecurityContext) &&
			(pWorld->m_pThingThis == (IThing *)this))
		{
			hr = pWorld->m_pSecurityContext->get_Exemplar(&pExemplar);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
		else
		{
			pExemplar = (IThing *)this;
			SAFEADDREF(pExemplar);
		}
	}

	hr = AddPropertyInt(bstrPropertyName, var, pExemplar, psbits, 
		permissions, vtType, bstrIID, bstrType);

ERROR_ENCOUNTERED:
	SAFERELEASE(pExemplar);

	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::AddPropertyExt(BSTR bstrPropertyName, VARIANT var, long psbits,
	long permissions, long vtType, REFIID riid, BSTR bstrType)
{
	HRESULT hr = S_OK;
	CWorldObject* pWorld = (CWorldObject*)m_pWorld;
	IThing* pExemplar = NULL;

	if (m_pWorld)
	{
		// figure out who is calling and set the exemplar		
		if ((pWorld->m_pSecurityContext) &&
			(pWorld->m_pThingThis == (IThing *)this))
		{
			hr = pWorld->m_pSecurityContext->get_Exemplar(&pExemplar);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
		else
		{
			pExemplar = (IThing *)this;
			SAFEADDREF(pExemplar);
		}
	}

	hr = AddPropertyInt(bstrPropertyName, var, pExemplar, psbits, 
		permissions, vtType, VWBSTRFromCLSID(riid), bstrType);

ERROR_ENCOUNTERED:
	SAFERELEASE(pExemplar);

	return hr;
}

STDMETHODIMP CThingObject::AddPropertyInt(BSTR bstrPropertyName, VARIANT var, IThing* pExemplar, long psbits,
	long permissions, long vtType, BSTR bstrIID, BSTR bstrType)
{
	HRESULT hr = S_OK;
	CWorldObject* pWorld = (CWorldObject*)m_pWorld;
	CComVariant varTemp;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;
	IPropertySecurity* pSecurity = NULL;

	ASSERT(bstrPropertyName);

	if (bstrPropertyName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::AddPropertyExtHelper: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::AddPropertyExtHelper: stub object, shouldn't be accessing\n");
		hr = VWOBJECT_E_STUBOBJECT;
		goto ERROR_ENCOUNTERED;
	}

	if (m_pProperties == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::AddPropertyExtHelper: property map is invalid\n");
		hr = VWOBJECT_E_INVALIDPROPERTYMAP;
		goto ERROR_ENCOUNTERED;
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::AddPropertyExtHelper: name (%s)\n", 
		CString(bstrPropertyName));
#endif

	// clone incoming property to varTemp
	hr = m_pWorld->CloneProperty(var, &varTemp);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pWorld->ValidatePropertyNameExt(bstrPropertyName);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(psbits, DISPID_THING_ADDPROPERTYINT, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(bstrPropertyName), "CThingObject::AddPropertyExtHelper", "VWOBJECT");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		hr = AddPropertyIntHelper(bstrPropertyName, varTemp, pExemplar, psbits, permissions, 
			vtType, bstrIID, bstrType, &pSecurity);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 8);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[7].vt = VT_BSTR;
		dispparams.rgvarg[7].bstrVal = bstrPropertyName;

		memcpy(&dispparams.rgvarg[6], &varTemp, sizeof(VARIANT));

		dispparams.rgvarg[5].vt = VT_DISPATCH;
		dispparams.rgvarg[5].pdispVal = pExemplar;

		dispparams.rgvarg[4].vt = VT_I4;
		dispparams.rgvarg[4].lVal = psbits;

		dispparams.rgvarg[3].vt = VT_I4;
		dispparams.rgvarg[3].lVal = permissions;

		dispparams.rgvarg[2].vt = VT_I4;
		dispparams.rgvarg[2].lVal = vtType;

		dispparams.rgvarg[1].vt = VT_BSTR;
		dispparams.rgvarg[1].bstrVal = bstrIID;

		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstrType;

		hr = SendThingCommand(psbits, DISPID_THING_ADDPROPERTYINT, DISPATCH_METHOD, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		hr = NotifyPropertyChangedExt(bstrPropertyName, psbits, pSecurity, thAdd, varTemp);
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pSecurity);

	return hr;
}

HRESULT CThingObject::AddPropertyIntHelper(BSTR bstrPropertyName, VARIANT var, IThing* pExemplar, long psbits,
	long permissions, long vtType, BSTR bstrIID, BSTR bstrType, IPropertySecurity** ppSecurity)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL fValid = VARIANT_FALSE;
	CWorldObject* pWorld = (CWorldObject*)m_pWorld;
	CPropertySecurity* pPropSec = NULL;

	// check the thing's property map for PS_ADD
	if (FAILED(CheckPropertiesSecurityExt(PS_ADD)))
	{
		CComBSTR bstrThingName("<unknown>");

		/* hr = */ get_Name(&bstrThingName.m_str);

		CString strInfo;
		strInfo.Format("Security violation: %s.AddProperty(\"%s\") denied, no add permissions", CString(bstrThingName), CString(bstrPropertyName));
		m_pWorld->Report(CComBSTR(strInfo), -1);

		hr = VWOBJECT_E_PERMISSION_DENIED;
		goto ERROR_ENCOUNTERED;
	}

	hr = IsValidProperty(bstrPropertyName, &fValid);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// if prop exists in inheritance tree, then fail
	// (exists in CThingObject, not just m_pProperties)

	if (fValid)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::AddPropertyIntHelper: property already exists (%s)\n", CString(bstrPropertyName));
#endif
		hr = VWOBJECT_E_PROPERTYEXISTS;
		goto ERROR_ENCOUNTERED;
	}

	// set back-ptr and prop name in object properties
	hr = SetPropertyName(bstrPropertyName, var);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", PROPERTY_TRACE, "CThingObject::AddPropertyIntHelper: name (%s), %s\n", 
		CString(bstrPropertyName), TraceProperty(var));
#endif

	hr = m_pProperties->put_PropertyInt(bstrPropertyName, var, NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// set the security name and exemplar
	hr = get_PropertySecurityExt(bstrPropertyName, HashKey<BSTR>(bstrPropertyName), NULL, NULL, ppSecurity);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (*ppSecurity == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::AddPropertyIntHelper: property security is invalid\n");
		hr = VWOBJECT_E_INVALIDSECURITY;
		goto ERROR_ENCOUNTERED;
	}

	pPropSec = (CPropertySecurity*)*ppSecurity;
	pPropSec->m_vtType = (unsigned short)vtType;
	pPropSec->m_bstrType = bstrType;

	(*ppSecurity)->put_Bits(psbits);
	(*ppSecurity)->put_Permissions(permissions);
	(*ppSecurity)->put_PropertyIID(bstrIID);

	SAFERELEASE(pPropSec->m_pExemplar); 
	pPropSec->m_pExemplar = pExemplar; 
	SAFEADDREF(pPropSec->m_pExemplar); 

	SAFERELEASE(pPropSec->m_pInstance); 
	pPropSec->m_pInstance = this; 
	SAFEADDREF(pPropSec->m_pInstance); 

#ifdef SUPPORT_CACHED_EVENTHANDLERS
	// cache these method names
	pPropSec->m_bstrOnChanged = m_bstrOn;
	pPropSec->m_bstrOnChanged += bstrPropertyName;
	pPropSec->m_bstrOnChanged += m_bstrChanged;

	pPropSec->m_bstrOnAccess = m_bstrOn;
	pPropSec->m_bstrOnAccess += bstrPropertyName;
	pPropSec->m_bstrOnAccess += m_bstrAccess;
#endif

	(*ppSecurity)->Enable();

#ifndef DISABLE_PROPERTYBIT_CACHING
	hr = CachePropertyBits(bstrPropertyName, VARIANT_FALSE);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
#endif

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CThingObject::RemoveProperty(BSTR bstrPropertyName)
{
	HRESULT hr = S_OK;
	CWorldObject* pWorld = (CWorldObject*)m_pWorld;
	IPropertySecurity* pSecurity = NULL;
	long psbits = 0;
	VARIANT_BOOL fInstance = VARIANT_FALSE;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	ASSERT(bstrPropertyName);
	if (bstrPropertyName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::RemoveProperty: incoming argument (BSTR) is NULL\n");
		return ReportThingError(E_INVALIDARG);
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::RemoveProperty: stub object, shouldn't be accessing\n");
		hr = VWOBJECT_E_STUBOBJECT;
		goto ERROR_ENCOUNTERED;
	}

	if (m_pProperties == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::RemoveProperty: property map is invalid\n");
		hr = VWOBJECT_E_INVALIDPROPERTYMAP;
		goto ERROR_ENCOUNTERED;
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::RemoveProperty: name (%s)\n", 
		CString(bstrPropertyName));
#endif

	hr = get_PropertySecurityExt(bstrPropertyName, HashKey<BSTR>(bstrPropertyName), NULL, &fInstance, &pSecurity);
	if (FAILED(hr)) 
		goto ERROR_ENCOUNTERED;

	if (pSecurity == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::RemoveProperty: property security is invalid\n");
		hr = VWOBJECT_E_INVALIDSECURITY;
		goto ERROR_ENCOUNTERED;
	}

	psbits = ((CPropertySecurity*)pSecurity)->m_ulBits;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(psbits, DISPID_THING_REMOVEPROPERTY, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(bstrPropertyName), "CThingObject::RemoveProperty", "VWOBJECT");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
#ifdef DISABLE_REMOVE_INTERNALLYADDED
		// REVIEW: check special case for internally added property
		if (psbits & PSBIT_INTERNALLYADDED)
		{
			CComBSTR bstrThingName("<unknown>");

			/* hr = */ get_Name(&bstrThingName.m_str);

			CString strInfo;
			strInfo.Format("Security violation: %s.RemoveProperty(\"%s\") denied, cannot remove internally added property", CString(bstrThingName), CString(bstrPropertyName));
			m_pWorld->Report(CComBSTR(strInfo), -1);

			hr = VWOBJECT_E_PERMISSION_DENIED;
			goto ERROR_ENCOUNTERED;
		}
#endif

		// check thing's property map for PS_ADD
		if (FAILED(CheckPropertiesSecurityExt(PS_ADD)))
		{
			CComBSTR bstrThingName("<unknown>");

			/* hr = */ get_Name(&bstrThingName.m_str);

			CString strInfo;
			strInfo.Format("Security violation: %s.RemoveProperty(\"%s\") denied, no remove permissions", CString(bstrThingName), CString(bstrPropertyName));
			m_pWorld->Report(CComBSTR(strInfo), -1);

			hr = VWOBJECT_E_PERMISSION_DENIED;
			goto ERROR_ENCOUNTERED;
		}

		// check the property's security for PS_ADD (otherwise OnAccess methods allow access)
		if (FAILED(CheckPropertySecurityExt(bstrPropertyName, pSecurity, fInstance, PS_ADD)))
		{
			CComBSTR bstrThingName("<unknown>");

			/* hr = */ get_Name(&bstrThingName.m_str);

			CString strInfo;
			strInfo.Format("Security violation: %s.RemoveProperty(\"%s\") denied, no remove permissions", CString(bstrThingName), CString(bstrPropertyName));
			m_pWorld->Report(CComBSTR(strInfo), -1);

			hr = VWOBJECT_E_PERMISSION_DENIED;
			goto ERROR_ENCOUNTERED;
		}

		{
			VARIANT* pvar = &((CPropertySecurity*)pSecurity)->m_var;

			// clear back-ptr and prop name in object property
			if (pvar->vt == VT_DISPATCH && pvar->pdispVal != NULL)
				/* hr = */ ClearPropertyName(*pvar);
		}

		hr = m_pProperties->RemoveInt(bstrPropertyName);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

#ifndef DISABLE_PROPERTYBIT_CACHING
		// property's already gone, don't have anywhere to cache bits
//		hr = CachePropertyBits(bstrPropertyName, VARIANT_FALSE);
//		if (FAILED(hr))
//			goto ERROR_ENCOUNTERED;
#endif
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstrPropertyName;

		hr = SendThingCommand(psbits, DISPID_THING_REMOVEPROPERTY, DISPATCH_METHOD, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	if (bShouldNotify == VARIANT_TRUE)
	{
		hr = NotifyPropertyChangedExt(bstrPropertyName, psbits, pSecurity, thRemove, CComVariant());
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pSecurity);

	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::put_Property(BSTR bstrPropertyName, VARIANT var)
{
	HRESULT hr = S_OK;
	UINT nHashOrig = HashKey<BSTR>(bstrPropertyName);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::put_Property: name (%s)\n", CString(bstrPropertyName));
#endif

	hr = put_PropertyExt(bstrPropertyName, nHashOrig, var);

	return ReportThingError(hr);
}

HRESULT CThingObject::put_PropertyExt(BSTR bstrPropertyName, UINT nHashOrig, VARIANT var)
{
	HRESULT hr = S_OK;
	CWorldObject* pWorld = (CWorldObject*)m_pWorld;
	CComVariant varTemp;
	long psbits = 0L;
	IPropertySecurity* pSecurity = NULL;
	VARIANT_BOOL fInstance = VARIANT_FALSE;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	ASSERT(bstrPropertyName);
	if (bstrPropertyName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::put_PropertyExt: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::put_PropertyExt: stub object, shouldn't be accessing\n");
		hr = VWOBJECT_E_STUBOBJECT;
		goto ERROR_ENCOUNTERED;
	}

	if (m_pProperties == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::put_PropertyExt: property map is invalid\n");
		hr = VWOBJECT_E_INVALIDPROPERTYMAP;
		goto ERROR_ENCOUNTERED;
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::put_PropertyExt: name (%s)\n", 
		CString(bstrPropertyName));
#endif

	// clone incoming property to varTemp
	hr = m_pWorld->CloneProperty(var, &varTemp);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// The existing Security... Could be in the Exemplar
	hr = get_PropertySecurityExt(bstrPropertyName, nHashOrig, NULL, &fInstance, &pSecurity);
	if (FAILED(hr)) 
		goto ERROR_ENCOUNTERED;

	if (pSecurity == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::put_PropertyExt: property security is invalid\n");
		hr = VWOBJECT_E_INVALIDSECURITY;
		goto ERROR_ENCOUNTERED;
	}

	psbits = ((CPropertySecurity*)pSecurity)->m_ulBits;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(psbits, DISPID_THING_PROPERTY, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(bstrPropertyName), "CThingObject::put_PropertyExt", "VWOBJECT");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		hr = put_PropertyInt(bstrPropertyName, varTemp, pSecurity, fInstance);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 2);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[1].vt = VT_BSTR;
		dispparams.rgvarg[1].bstrVal = bstrPropertyName;

		memcpy(&dispparams.rgvarg[0], &varTemp, sizeof(VARIANT));

		hr = SendThingCommand(psbits, DISPID_THING_PROPERTY, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	if (bShouldNotify == VARIANT_TRUE)
	{
		hr = NotifyPropertyChangedExt(bstrPropertyName, psbits, pSecurity, thPut, varTemp);
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pSecurity);

	return hr;
}

HRESULT CThingObject::put_PropertyInt(BSTR bstrPropertyName, VARIANT var, IPropertySecurity* pSecurity, VARIANT_BOOL fInstance)
{
	HRESULT hr = S_OK;

	if (pSecurity == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::put_PropertyInt: property security is invalid\n");
		hr = VWOBJECT_E_INVALIDSECURITY;
		goto ERROR_ENCOUNTERED;
	}

	// check thing's property map for PS_WRITE
	if (FAILED(CheckPropertiesSecurityExt(PS_WRITE)))
	{
		CComBSTR bstrThingName("<unknown>");

		/* hr = */ get_Name(&bstrThingName.m_str);

		CString strInfo;
		strInfo.Format("Security violation: %s.put_Property(\"%s\") denied, no write permissions", CString(bstrThingName), CString(bstrPropertyName));
		m_pWorld->Report(CComBSTR(strInfo), -1);

		hr = VWOBJECT_E_PERMISSION_DENIED;
		goto ERROR_ENCOUNTERED;
	}

	// check the property's security for PS_WRITE
	if (FAILED(CheckPropertySecurityExt(bstrPropertyName, pSecurity, fInstance, PS_WRITE)))
	{
		CComBSTR bstrThingName("<unknown>");

		/* hr = */ get_Name(&bstrThingName.m_str);

		CString strInfo;
		strInfo.Format("Security violation: %s.put_Property(\"%s\") denied, no write permissions", CString(bstrThingName), CString(bstrPropertyName));
		m_pWorld->Report(CComBSTR(strInfo), -1);

		hr = VWOBJECT_E_PERMISSION_DENIED;
		goto ERROR_ENCOUNTERED;
	}

	{
		VARIANT* pvar = &((CPropertySecurity*)pSecurity)->m_var;

		// clear existing property, if already instanced
		if (pvar->vt == VT_DISPATCH && pvar->pdispVal != NULL)
			/* hr = */ ClearPropertyName(*pvar);
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", PROPERTY_TRACE, "CThingObject::put_PropertyInt: name (%s), %s\n", 
		CString(bstrPropertyName), TraceProperty(var));
#endif

	// if property exists in the instance, we can just shove the data directly in the security slot
	if (fInstance == VARIANT_TRUE)
		hr = pSecurity->put_Property(var);
	else
		hr = m_pProperties->put_PropertyInt(bstrPropertyName, var, pSecurity);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// NOTE: set parent/prop name after put_Property because it might copy the data in place, 
	// or it might push in the new ptr... so we need to look up the prop again to set the 
	// parent on the correct object property

	// NOTE: need to do this for all properties (not just object properties)
	// only do it if incoming data is valid IDispatch*
//	if (var.vt == VT_DISPATCH && var.pdispVal != NULL)
	{
		IPropertySecurity* pNewSecurity = NULL;
		VARIANT* pvar = NULL;

		if (fInstance == VARIANT_FALSE)
		{
			{
				CSetUID SetUID(m_pWorld, this, this, VARIANT_TRUE);

				// grab the new security slot
				hr = m_pProperties->get_PropertySecurityExt(bstrPropertyName, HashKey<BSTR>(bstrPropertyName), &pNewSecurity);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED_1;
			}

			if (pNewSecurity == NULL)
				goto ERROR_ENCOUNTERED_1;

			pvar = &((CPropertySecurity*)pNewSecurity)->m_var;
		}
		else
		{
			pvar = &((CPropertySecurity*)pSecurity)->m_var;
		}

		// set back-ptr and prop name in object properties
		if (pvar->vt == VT_DISPATCH && pvar->pdispVal != NULL)
		{
			hr = SetPropertyName(bstrPropertyName, *pvar);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED_1;
		}

ERROR_ENCOUNTERED_1:
		if (fInstance == VARIANT_FALSE)
		{
			// make sure the instance is pointing to this instance
			pNewSecurity->put_Instance(this);
			pNewSecurity->Enable();

			SAFERELEASE(pNewSecurity);
		}
	}

#ifndef DISABLE_PROPERTYBIT_CACHING
	// don't need to cache on put
//	hr = CachePropertyBits(bstrPropertyName, VARIANT_FALSE);
//	if (FAILED(hr))
//		goto ERROR_ENCOUNTERED;
#endif

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CThingObject::get_RemoteProperty(BSTR bstrPropertyName, VARIANT* pvar)
{
	HRESULT hr = S_OK;
	CComVariant varTemp;
	IPropertySecurity* pSecurity = NULL;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	ASSERT(bstrPropertyName);
	if (bstrPropertyName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_RemoteProperty: incoming argument (BSTR) is NULL\n");
		return ReportThingError(E_INVALIDARG);
	}

	ASSERT(pvar);
	if (pvar == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_RemoteProperty: outgoing pointer (VARIANT*) is NULL\n");
		return ReportThingError(E_POINTER);
	}

	if (m_pProperties == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::get_RemoteProperty: property map is invalid\n");
		hr = VWOBJECT_E_INVALIDPROPERTYMAP;
		goto ERROR_ENCOUNTERED;
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::get_RemoteProperty: name (%s)\n", 
		CString(bstrPropertyName));
#endif

	m_pWorld->get_ShouldExecuteSync(&bShouldExecute);

	// NOTE: using ShouldRemoteSync (only remote on client-side)
	m_pWorld->get_ShouldRemoteSync(&bShouldRemote);

	if (bShouldExecute == VARIANT_TRUE)
	{
		hr = get_Property(bstrPropertyName, pvar);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;
		HRESULT recvhr = S_OK;
		CComVariant var;

		// only remote if it's a stub
		if (m_bStub == VARIANT_FALSE)
		{
			hr = get_Property(bstrPropertyName, pvar);

			goto ERROR_ENCOUNTERED;
		}

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstrPropertyName;

		hr = SendSyncThingCommand(PSBIT_NONE, DISPID_THING_REMOTEPROPERTY, DISPATCH_PROPERTYGET, &dispparams, &recvhr, &var);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		// if sendmessage and invoke both succeeded, pull out retval
		if (SUCCEEDED(hr) && hr != VWOBJECT_S_NOTREMOTED)
		{
			hr = recvhr;

			if (SUCCEEDED(recvhr))
			{
				::VariantCopy(pvar, &var);
			}
		}
	}

ERROR_ENCOUNTERED:
	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::get_Property(BSTR bstrPropertyName, VARIANT* pvar)
{
	HRESULT hr = S_OK;
	UINT nHashOrig = HashKey<BSTR>(bstrPropertyName);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::get_Property: name (%s)\n", CString(bstrPropertyName));
#endif

	hr = get_PropertyExt(bstrPropertyName, nHashOrig, NULL, pvar);

	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::get_PropertyExt(BSTR bstrPropertyName, UINT nHashOrig, IThing** ppattached, VARIANT* pvar)
{
	HRESULT hr = S_OK;
	IPropertySecurity* pSecurity = NULL;
	VARIANT_BOOL fInstance = VARIANT_FALSE;
	IThing* pSource = NULL;

	ASSERT(bstrPropertyName);
	if (bstrPropertyName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_PropertyExt: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_PropertyExt: stub object, shouldn't be accessing\n");
		return VWOBJECT_E_STUBOBJECT;
	}

	if (m_pProperties == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::get_PropertyExt: property map is invalid\n");
		return VWOBJECT_E_INVALIDPROPERTYMAP;
	}

	// ppattached can be NULL
	if (ppattached)
		*ppattached = NULL;

	// pvar can be NULL
	if (pvar)
		::VariantClear(pvar);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::get_PropertyExt: name (%s)\n", CString(bstrPropertyName));
#endif

	// lookup property in object
	hr = get_PropertySecurityExt(bstrPropertyName, nHashOrig, &pSource, &fInstance, &pSecurity);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (pSecurity == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_PropertyExt: property security is invalid\n");
		hr = VWOBJECT_E_INVALIDSECURITY;
		goto ERROR_ENCOUNTERED;
	}

	// if property found at exemplar level, attempt to instance it
	if (pSource != this)
	{
		hr = InstanceProperty(bstrPropertyName, pSource);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// if property was instanced, lookup property in instance
		if (hr != VWOBJECT_S_NOTINSTANCED)
		{
			hr = get_PropertyExt(bstrPropertyName, nHashOrig, ppattached, pvar);
		}
		else
		{
			::VariantCopyInd(pvar, &((CPropertySecurity*)pSecurity)->m_var);

#if 0
			// grab property from security slot
			hr = pSecurity->get_Property(pvar);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
#endif
		}
	}
	else
	{
		::VariantCopyInd(pvar, &((CPropertySecurity*)pSecurity)->m_var);

#if 0
		// grab property from security slot
		hr = pSecurity->get_Property(pvar);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
#endif
	}

	if (ppattached)
	{
		*ppattached = pSource;
		SAFEADDREF(*ppattached);
	}

#ifdef SUPPORT_DATESTAMPS
	SetAccessedDate();
#endif

ERROR_ENCOUNTERED:
	SAFERELEASE(pSecurity);
	SAFERELEASE(pSource);

	return hr;
}

STDMETHODIMP CThingObject::InstanceProperty(BSTR bstrPropertyName, IThing* pExemplar)
{
	HRESULT hr = S_OK;
	long psbits = 0;
	IPropertySecurity* pSecurity = NULL;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;
	VARIANT_BOOL fInstance = VARIANT_FALSE;

	ASSERT(bstrPropertyName);
	if (bstrPropertyName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::InstanceProperty: incoming argument (BSTR) is NULL\n");
		return ReportThingError(E_INVALIDARG);
	}

	ASSERT(pExemplar);
	if (pExemplar == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::InstanceProperty: incoming argument (IThing*) is NULL\n");
		return ReportThingError(E_INVALIDARG);
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::InstanceProperty: stub object, shouldn't be accessing\n");
		hr = VWOBJECT_E_STUBOBJECT;
		goto ERROR_ENCOUNTERED;
	}

	if (m_pProperties == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::InstanceProperty: property map is invalid\n");
		hr = VWOBJECT_E_INVALIDPROPERTYMAP;
		goto ERROR_ENCOUNTERED;
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::InstanceProperty: name (%s)\n", CString(bstrPropertyName));
#endif

	// The existing Security... Could be in the Exemplar
	hr = get_PropertySecurityExt(bstrPropertyName, HashKey<BSTR>(bstrPropertyName), NULL, &fInstance, &pSecurity);
	if (FAILED(hr)) 
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::InstanceProperty: property (%s), failed to lookup property security\n", CString(bstrPropertyName));
		goto ERROR_ENCOUNTERED;
	}

	if (pSecurity == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::InstanceProperty: property security is invalid\n");
		hr = VWOBJECT_E_INVALIDSECURITY;
		goto ERROR_ENCOUNTERED;
	}

	// must check if property was already instanced (can happen with remoted InstanceProperty)
	if (fInstance == VARIANT_TRUE)
	{
		hr = VWOBJECT_S_NOTINSTANCED;
		goto ERROR_ENCOUNTERED;
	}

	psbits = ((CPropertySecurity*)pSecurity)->m_ulBits;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(psbits, DISPID_THING_INSTANCEPROPERTY, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(bstrPropertyName), "CThingObject::InstanceProperty", "VWOBJECT");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		hr = InstancePropertyInt(bstrPropertyName, pExemplar, pSecurity);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::InstanceProperty: property (%s), InstancePropertyInt failed\n", CString(bstrPropertyName));
			goto ERROR_ENCOUNTERED;
		}

		// if property didn't need to be instanced, don't bother remoting the request
		if (hr == VWOBJECT_S_NOTINSTANCED)
			goto ERROR_ENCOUNTERED;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 2);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[1].vt = VT_BSTR;
		dispparams.rgvarg[1].bstrVal = bstrPropertyName;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = pExemplar;

		hr = SendThingCommand(psbits, DISPID_THING_INSTANCEPROPERTY, DISPATCH_METHOD, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		if (FAILED(hr))
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::InstanceProperty: property (%s), SendThingCommand failed\n", CString(bstrPropertyName));
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pSecurity);

	return hr;
}

HRESULT CThingObject::InstancePropertyInt(BSTR bstrPropertyName, IThing* pExemplar, IPropertySecurity* pSecurity)
{
	HRESULT hr = S_OK;
	CComVariant var;

	hr = pExemplar->get_PropertyExt(bstrPropertyName, HashKey<BSTR>(bstrPropertyName), NULL, &var);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// only clone IObjectProperties
	if (var.vt == VT_DISPATCH)
	{
		// Do we need to copy?
		CObjectPropertyPtr objPropPtr;
		CThingPtr thingPtr;

		// convert IDispatch to IObjectProperty
		objPropPtr = var.pdispVal;

		// Make sure that the ObjectProperty is not an IThing
		// IThings don't have security backpointers
		thingPtr = var.pdispVal;

		if ((objPropPtr != NULL) &&
			(thingPtr == NULL))
		{
			VARIANT_BOOL bInstanced = VARIANT_FALSE;

			hr = InstancePropertyIntHelper(bstrPropertyName, objPropPtr, pExemplar, pSecurity, &bInstanced);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			if (bInstanced == VARIANT_FALSE)
				hr = VWOBJECT_S_NOTINSTANCED;
		}
		else
			hr = VWOBJECT_S_NOTINSTANCED;
	}
	else
		hr = VWOBJECT_S_NOTINSTANCED;

ERROR_ENCOUNTERED:
	return hr;
}

HRESULT CThingObject::InstancePropertyIntHelper(BSTR bstrPropertyName, IObjectProperty* pProperty, IThing* pExemplar, IPropertySecurity* pSecurity, VARIANT_BOOL* pbInstanced)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bRet = VARIANT_FALSE;
	CWorldObject* pWorld = (CWorldObject*)m_pWorld;

	if (pbInstanced == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::InstancePropertyIntHelper: invalid incoming pointer (VARIANT_BOOL*)\n");
		goto ERROR_ON_CLONE;
	}

	*pbInstanced = VARIANT_FALSE;

	if (pSecurity == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::InstancePropertyIntHelper: invalid security ptr from object property\n");
		goto ERROR_ON_CLONE;
	}

	// what are the conditions for copyOnGet?
	// 1 The bit is set
	// 2 ! VT_DISPATCH ie a mutable type
	
	if (FAILED(hr = pSecurity->get_BitState(PSBIT_COPYONGET, &bRet)))
		goto ERROR_ON_CLONE;
	
	// Is it instance data and mutable? if so copy
	if (bRet == VARIANT_TRUE)
	{
		// REVIEW: put_Property below will already call CloneProperty, no need to do it here
//		if (FAILED(hr = m_pWorld->CloneProperty(CComVariant(pProperty), &varCopy)))
//			goto ERROR_ON_CLONE;
		
		// now, do a local only "put" on the instance property
		// map. Need to push the security to allow a clone!
		// Put will clone the security from the exemplar!
		{
			IThing* pPropertyExemplar = NULL;

			pSecurity->get_Exemplar(&pPropertyExemplar);

			CSetUID SetUID(m_pWorld, this, pPropertyExemplar, VARIANT_TRUE);

			SAFERELEASE(pPropertyExemplar);

			// set the property in local mode (don't remote)
			VARIANT_BOOL bLocalMode = pWorld->m_bLocalMode;

			pWorld->m_bLocalMode = VARIANT_TRUE;

			hr = put_Property(bstrPropertyName, CComVariant(pProperty));

			pWorld->m_bLocalMode = bLocalMode;

			if (FAILED(hr))
				goto ERROR_ON_CLONE;

			*pbInstanced = VARIANT_TRUE;
		}	
	}

ERROR_ON_CLONE:
	return hr;
}

STDMETHODIMP CThingObject::get_PropertySecurity(BSTR bstrPropertyName, IPropertySecurity** ppSecurity)
{
	HRESULT hr = S_OK;
	UINT nHashOrig = HashKey<BSTR>(bstrPropertyName);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::get_PropertySecurity: name (%s)\n", CString(bstrPropertyName));
#endif

	hr = get_PropertySecurityExt(bstrPropertyName, nHashOrig, NULL, NULL, ppSecurity);

	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::get_PropertySecurityExt(BSTR bstrPropertyName, UINT nHashOrig, IThing** ppattached, VARIANT_BOOL* pfInstance, IPropertySecurity** ppSecurity)
{
	HRESULT hr = S_OK;
	IThing* pSource = NULL;
	IPropertyMap* pProperties = NULL;

	ASSERT(bstrPropertyName);
	if (bstrPropertyName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_PropertySecurityExt: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	// NOTE: ppattached can be NULL
	if (ppattached)
		*ppattached = NULL;

	// pfInstance can be NULL
	if (pfInstance)
		*pfInstance = VARIANT_FALSE;

	ASSERT(ppSecurity);
	if (ppSecurity == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_PropertySecurityExt: outgoing pointer (IPropertySecurity**) is NULL\n");
		return E_POINTER;
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_PropertySecurityExt: stub object, shouldn't be accessing\n");
		return VWOBJECT_E_STUBOBJECT;
	}

	if (m_pProperties == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_PropertySecurityExt: property map is invalid\n");
		return VWOBJECT_E_INVALIDPROPERTYMAP;
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::get_PropertySecurityExt: name (%s)\n", CString(bstrPropertyName));
#endif

	// check thing's property map for PS_READ
	if (FAILED(CheckPropertiesSecurityExt(PS_READ)))
	{
		CComBSTR bstrThingName("<unknown>");

		/* hr = */ get_Name(&bstrThingName.m_str);

		CString strInfo;
		strInfo.Format("Security violation: %s.get_PropertySecurity(\"%s\") denied, no read permissions", CString(bstrThingName), CString(bstrPropertyName));
		m_pWorld->Report(CComBSTR(strInfo), -1);

		hr = VWOBJECT_E_PERMISSION_DENIED;
		goto ERROR_ENCOUNTERED;
	}

	// initialize pointers
	pSource = this;
	SAFEADDREF(pSource);

	pProperties = m_pProperties;
	SAFEADDREF(pProperties);

	// lookup property in object
	hr = pProperties->get_PropertySecurityExt(bstrPropertyName, nHashOrig, ppSecurity);

	// if property doesn't exist, keep searching
	while (hr == VWOBJECT_E_PROPERTYNOTEXIST)
	{
		// release old property map
		SAFERELEASE(pProperties);

		// get new source
		IThing* pOldSource = pSource;

		hr = pOldSource->get_Exemplar(&pSource);

		// release old source
		SAFERELEASE(pOldSource);

		if (FAILED(hr))
			break;

		if (pSource)
		{
			// get new property map
			hr = pSource->get_InstanceProperties(&pProperties);
			if (FAILED(hr))
				break;

			if (pProperties == NULL)
			{
				hr = VWOBJECT_E_INVALIDPROPERTYMAP;
				break;
			}

			// lookup property in new source
			hr = pProperties->get_PropertySecurityExt(bstrPropertyName, nHashOrig, ppSecurity);
		}
		else
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::get_PropertySecurityExt: unknown property (%s)\n", CString(bstrPropertyName));
#endif
			hr = VWOBJECT_E_PROPERTYNOTEXIST;
			goto ERROR_ENCOUNTERED;
		}
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (pSource == this)
	{
		if (pfInstance)
			*pfInstance = VARIANT_TRUE;
	}

	// Now we should have the security for the slot - if it exists, check that we have read permission
	if (*ppSecurity == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_PropertySecurityExt: property security is invalid\n");
		hr = VWOBJECT_E_INVALIDSECURITY;
		goto ERROR_ENCOUNTERED;
	}
	
	// Note: if no pfInstance, assume it is an instance (called from AddPropertyIntHelper)
	if (FAILED(CheckPropertySecurityExt(bstrPropertyName, *ppSecurity, ((pfInstance) ? *pfInstance : VARIANT_TRUE), PS_READ)))
	{
		CComBSTR bstrThingName("<unknown>");

		get_Name(&bstrThingName.m_str);

		CString strInfo;
		strInfo.Format("Security violation: %s.get_Property(\"%s\") denied, no read permissions", CString(bstrThingName), CString(bstrPropertyName));
		m_pWorld->Report(CComBSTR(strInfo), -1);

		hr = VWOBJECT_E_PERMISSION_DENIED;
		goto ERROR_ENCOUNTERED;
	}


	// set the instance/exemplar where the property was found
	if (ppattached)
	{
		*ppattached = pSource;
		SAFEADDREF(*ppattached);
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pProperties);
	SAFERELEASE(pSource);
	return hr;
}

STDMETHODIMP CThingObject::CheckPropertySecurityExt(BSTR bstrName, unsigned long ulFlags)
{
	HRESULT hr = S_OK;
	IPropertySecurity* pSecurity = NULL;
	VARIANT_BOOL fInInstance = VARIANT_FALSE;
	UINT nHashOrig = HashKey<BSTR>(bstrName);

	hr = get_PropertySecurityExt(bstrName, nHashOrig, NULL, &fInInstance, &pSecurity);
	if (FAILED(hr))
	{
		// don't fail if property not found
		if (hr == VWOBJECT_E_PROPERTYNOTEXIST)
			hr = S_OK;

		goto ERROR_ENCOUNTERED;
	}

	if (pSecurity == NULL)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::CheckPropertySecurityExt: property (%s), property security is invalid\n", CString(bstrName));
#endif
		hr = VWOBJECT_E_INVALIDSECURITY;
		goto ERROR_ENCOUNTERED;
	}

	hr = CheckPropertySecurityExt(bstrName, pSecurity, fInInstance, ulFlags);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	SAFERELEASE(pSecurity);

	return hr;
}

HRESULT CThingObject::CheckPropertySecurityExt(BSTR bstrName, IPropertySecurity* pSecurity, VARIANT_BOOL fInInstance, unsigned long ulFlags)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	IThing* pInstance = NULL;

#ifdef _DEBUG
	CString strFlags = "(";
	strFlags += ulFlags & PS_READ ? "r":"-";
	strFlags += ulFlags & PS_WRITE ? "w":"-";
	strFlags += ulFlags & PS_EXECUTE ? "x":"-";
	strFlags += ulFlags & PS_ADD ? "a":"-";
	strFlags += ")";
#endif

	// REVIEW: check security bits first, then invoke OnAccess method

#ifdef SUPPORT_CHECKPROPERTYSECURITY
	// If not local check and see if we even have access to write it
	if (fInInstance == VARIANT_FALSE)
	{
		// swap the exemplar instance with our own
		pSecurity->get_Instance(&pInstance);
		pSecurity->put_Instance(this);
	}

	hr = pSecurity->CheckSecurity(ulFlags, VARIANT_TRUE, &bSuccess);
	if (FAILED(hr) || bSuccess == VARIANT_FALSE)
	{
#ifdef _DEBUG
		if (FAILED(hr))
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::CheckPropertySecurityExt: property (%s), IPropertySecurity::CheckSecurity failed, ulFlags %s\n", CString(bstrName), strFlags);
		else
			VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::CheckPropertySecurityExt: property (%s), permission was denied, ulFlags %s\n", CString(bstrName), strFlags);
#endif
		hr = VWOBJECT_E_PERMISSION_DENIED;
	}

	if (fInInstance == VARIANT_FALSE)
	{
		if (pSecurity)
		{
			// and restore original state
			pSecurity->put_Instance(pInstance);
			SAFERELEASE(pInstance);
		}
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
#endif

#ifdef SUPPORT_INVOKEONPROPERTYACCESS
	// only check PS_WRITE
	if (ulFlags == PS_WRITE)
	{
		hr = InvokeOnPropertyAccess(bstrName, ((CPropertySecurity*)pSecurity)->m_ulBits, ulFlags, pSecurity);
		if (FAILED(hr))
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::CheckPropertySecurityExt: property (%s), InvokeOnPropertyAccess failed, ulFlags %s\n", CString(bstrName), strFlags);
#endif
			goto ERROR_ENCOUNTERED;
		}
	}
#endif

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CThingObject::CheckMethodSecurityExt(BSTR bstrName, unsigned long ulFlags)
{
	HRESULT hr = S_OK;
	IPropertySecurity* pSecurity = NULL;
	VARIANT_BOOL fInInstance = VARIANT_FALSE;
	UINT nHashOrig = HashKey<BSTR>(bstrName);

	hr = get_MethodSecurityExt(bstrName, nHashOrig, NULL, &fInInstance, &pSecurity);
	if (FAILED(hr))
	{
		// don't fail if method not found
		if (hr == VWOBJECT_E_METHODNOTEXIST || hr == VWOBJECT_E_INVALIDMETHODMAP)
			hr = S_OK;

		goto ERROR_ENCOUNTERED;
	}

	if (pSecurity == NULL)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::CheckMethodSecurityExt: method (%s), property security is invalid\n", CString(bstrName));
#endif
		hr = VWOBJECT_E_INVALIDSECURITY;
		goto ERROR_ENCOUNTERED;
	}

	hr = CheckMethodSecurityExt(bstrName, pSecurity, fInInstance, ulFlags);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	SAFERELEASE(pSecurity);

	return hr;
}

HRESULT CThingObject::CheckMethodSecurityExt(BSTR bstrName, IPropertySecurity* pSecurity, VARIANT_BOOL fInInstance, unsigned long ulFlags)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	IThing* pInstance = NULL;

#ifdef _DEBUG
	CString strFlags = "(";
	strFlags += ulFlags & PS_READ ? "r":"-";
	strFlags += ulFlags & PS_WRITE ? "w":"-";
	strFlags += ulFlags & PS_EXECUTE ? "x":"-";
	strFlags += ulFlags & PS_ADD ? "a":"-";
	strFlags += ")";
#endif

	// If not local check and see if we even have access to write it
	if (fInInstance == VARIANT_FALSE)
	{
		// swap the exemplar instance with our own
		pSecurity->get_Instance(&pInstance);
		pSecurity->put_Instance(this);
	}

	hr = pSecurity->CheckSecurity(ulFlags, VARIANT_TRUE, &bSuccess);
	if (FAILED(hr) || bSuccess == VARIANT_FALSE)
	{
#ifdef _DEBUG
		if (FAILED(hr))
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::CheckMethodSecurityExt: method (%s), IPropertySecurity::CheckSecurity failed, ulFlags %s\n", CString(bstrName), strFlags);
		else
			VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::CheckMethodSecurityExt: method (%s), permission was denied, ulFlags %s\n", CString(bstrName), strFlags);
#endif
		hr = VWOBJECT_E_PERMISSION_DENIED;
		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	if (fInInstance == VARIANT_FALSE)
	{
		if (pSecurity)
		{
			// and restore original state
			pSecurity->put_Instance(pInstance);
			SAFERELEASE(pInstance);
		}
	}

	return hr;
}

STDMETHODIMP CThingObject::CheckPropertiesSecurityExt(unsigned long ulFlags)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bSuccess = VARIANT_FALSE;

#ifdef _DEBUG
	CString strFlags = "(";
	strFlags += ulFlags & PS_READ ? "r":"-";
	strFlags += ulFlags & PS_WRITE ? "w":"-";
	strFlags += ulFlags & PS_EXECUTE ? "x":"-";
	strFlags += ulFlags & PS_ADD ? "a":"-";
	strFlags += ")";
#endif

	hr = m_PropSecurity->CheckSecurity(ulFlags, VARIANT_TRUE, &bSuccess);
	if (FAILED(hr) || bSuccess == VARIANT_FALSE)
	{
#ifdef _DEBUG
		if (FAILED(hr))
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::CheckPropertiesSecurityExt: IPropertySecurity::CheckSecurity failed, ulFlags %s\n", strFlags);
		else
			VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::CheckPropertiesSecurityExt: permission was denied, ulFlags %s\n", strFlags);
#endif
		hr = VWOBJECT_E_PERMISSION_DENIED;
	}

	return hr;
}

STDMETHODIMP CThingObject::CheckMethodsSecurityExt(unsigned long ulFlags)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bSuccess = VARIANT_FALSE;

#ifdef _DEBUG
	CString strFlags = "(";
	strFlags += ulFlags & PS_READ ? "r":"-";
	strFlags += ulFlags & PS_WRITE ? "w":"-";
	strFlags += ulFlags & PS_EXECUTE ? "x":"-";
	strFlags += ulFlags & PS_ADD ? "a":"-";
	strFlags += ")";
#endif

	hr = m_MethodSecurity->CheckSecurity(ulFlags, VARIANT_TRUE, &bSuccess);
	if (FAILED(hr) || bSuccess == VARIANT_FALSE)
	{
#ifdef _DEBUG
		if (FAILED(hr))
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::CheckMethodsSecurityExt: IPropertySecurity::CheckSecurity failed, ulFlags %s\n", strFlags);
		else
			VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::CheckMethodsSecurityExt: permission was denied, ulFlags %s\n", strFlags);
#endif
		hr = VWOBJECT_E_PERMISSION_DENIED;
	}

	return hr;
}

STDMETHODIMP CThingObject::IsValidProperty(BSTR bstrPropertyName, VARIANT_BOOL* pboolVal)
{
	HRESULT hr = S_OK;

	ASSERT(bstrPropertyName);
	if (bstrPropertyName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::IsValidProperty: incoming argument (BSTR) is NULL\n");
		return ReportThingError(E_INVALIDARG);
	}

	ASSERT(pboolVal);
	if (pboolVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::IsValidProperty: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return ReportThingError(E_POINTER);
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::IsValidProperty: stub object, shouldn't be accessing\n");
		hr = VWOBJECT_E_STUBOBJECT;
		goto ERROR_ENCOUNTERED;
	}

	if (m_pProperties == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::IsValidProperty: property map is invalid\n");
		hr = VWOBJECT_E_INVALIDPROPERTYMAP;
		goto ERROR_ENCOUNTERED;
	}

	hr = m_pProperties->IsValid(bstrPropertyName, pboolVal);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// if not valid property and thing has a exemplar
	if (!*pboolVal && m_pExemplar)
	{
		// then try exemplar
		hr = m_pExemplar->IsValidProperty(bstrPropertyName, pboolVal);
	}

ERROR_ENCOUNTERED:
	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::IsValidMethod(BSTR bstrMethodName, VARIANT_BOOL* pboolVal)
{
	HRESULT hr = S_OK;

	ASSERT(bstrMethodName);
	if (bstrMethodName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::IsValidMethod: incoming argument (BSTR) is NULL\n");
		return ReportThingError(E_INVALIDARG);
	}

	ASSERT(pboolVal);
	if (pboolVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::IsValidMethod: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return ReportThingError(E_POINTER);
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::IsValidMethod: stub object, shouldn't be accessing\n");
		hr = VWOBJECT_E_STUBOBJECT;
		goto ERROR_ENCOUNTERED;
	}

	if (m_pMethods == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::IsValidMethod: method map is invalid\n");
		hr = VWOBJECT_E_INVALIDMETHODMAP;
		goto ERROR_ENCOUNTERED;
	}

	hr = m_pMethods->IsValid(bstrMethodName, pboolVal);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// if not valid property and thing has a exemplar
	if (!*pboolVal && m_pExemplar)
	{
		// then try exemplar
		hr = m_pExemplar->IsValidMethod(bstrMethodName, pboolVal);
	}

ERROR_ENCOUNTERED:
	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::IsValidInstanceProperty(BSTR bstrPropertyName, VARIANT_BOOL* pboolVal)
{
	HRESULT hr = S_OK;

	ASSERT(bstrPropertyName);
	if (bstrPropertyName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::IsValidInstanceProperty: incoming argument (BSTR) is NULL\n");
		return ReportThingError(E_INVALIDARG);
	}

	ASSERT(pboolVal);
	if (pboolVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::IsValidInstanceProperty: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return ReportThingError(E_POINTER);
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::IsValidInstanceProperty: stub object, shouldn't be accessing\n");
		hr = VWOBJECT_E_STUBOBJECT;
		goto ERROR_ENCOUNTERED;
	}

	if (m_pProperties == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::IsValidInstanceProperty: property map is invalid\n");
		hr = VWOBJECT_E_INVALIDPROPERTYMAP;
		goto ERROR_ENCOUNTERED;
	}

	hr = m_pProperties->IsValid(bstrPropertyName, pboolVal);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::IsValidInstanceMethod(BSTR bstrMethodName, VARIANT_BOOL* pboolVal)
{
	HRESULT hr = S_OK;

	ASSERT(bstrMethodName);
	if (bstrMethodName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::IsValidInstanceMethod: incoming argument (BSTR) is NULL\n");
		return ReportThingError(E_INVALIDARG);
	}

	ASSERT(pboolVal);
	if (pboolVal == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::IsValidInstanceMethod: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return ReportThingError(E_POINTER);
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::IsValidInstanceMethod: stub object, shouldn't be accessing\n");
		hr = VWOBJECT_E_STUBOBJECT;
		goto ERROR_ENCOUNTERED;
	}

	if (m_pMethods == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::IsValidInstanceMethod: method map is invalid\n");
		hr = VWOBJECT_E_INVALIDMETHODMAP;
		goto ERROR_ENCOUNTERED;
	}

	hr = m_pMethods->IsValid(bstrMethodName, pboolVal);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::get_InstanceMethods(IPropertyMap** pppropertymap)
{
	HRESULT hr = S_OK;

	ASSERT(pppropertymap);
	if (pppropertymap == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_InstanceMethods: outgoing pointer (IPropertyMap**) is NULL\n");
		return ReportThingError(E_POINTER);
	}

	*pppropertymap = NULL;

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_InstanceMethods: stub object, shouldn't be accessing\n");
		hr = VWOBJECT_E_STUBOBJECT;
		goto ERROR_ENCOUNTERED;
	}

	// allow return of NULL propmap
#if 0
	if (m_pMethods == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::get_InstanceMethods: method map is invalid\n");
		hr = VWOBJECT_E_INVALIDMETHODMAP;
		goto ERROR_ENCOUNTERED;
	}
#endif

	*pppropertymap = m_pMethods;
	SAFEADDREF(*pppropertymap);

ERROR_ENCOUNTERED:
	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::get_Methods(IPropertyMap** pppropertymap)
{
	HRESULT hr = S_OK;

	ASSERT(pppropertymap);
	if (pppropertymap == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_Methods: outgoing pointer (IPropertyMap**) is NULL\n");
		return ReportThingError(E_POINTER);
	}

	*pppropertymap = NULL;

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_Methods: stub object, shouldn't be accessing\n");
		hr = VWOBJECT_E_STUBOBJECT;
		goto ERROR_ENCOUNTERED;
	}

	if (m_pMethods == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::get_Methods: method map is invalid\n");
		hr = VWOBJECT_E_INVALIDMETHODMAP;
		goto ERROR_ENCOUNTERED;
	}

	hr = CreatePropertyMap(m_pWorld, pppropertymap);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = CopyMethodsTo(*pppropertymap);
	if (FAILED(hr))
	{
		SAFERELEASE(*pppropertymap);
		goto ERROR_ENCOUNTERED;
	}

	hr = m_pMethods->CopyTo(*pppropertymap);

ERROR_ENCOUNTERED:
	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::CopyMethodsTo(IPropertyMap* ppropertymap)
{
	HRESULT hr = S_OK;

	ASSERT(ppropertymap);
	if (ppropertymap == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::CopyMethodsTo: incoming argumente (IPropertyMap*) is NULL\n");
		return ReportThingError(E_INVALIDARG);
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::CopyMethodsTo: stub object, shouldn't be accessing\n");
		hr = VWOBJECT_E_STUBOBJECT;
		goto ERROR_ENCOUNTERED;
	}

	if (m_pMethods == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::CopyMethodsTo: method map is invalid\n");
		hr = VWOBJECT_E_INVALIDMETHODMAP;
		goto ERROR_ENCOUNTERED;
	}

	if (m_pExemplar)
	{
		hr = m_pExemplar->CopyMethodsTo(ppropertymap);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	hr = m_pMethods->CopyTo(ppropertymap);

ERROR_ENCOUNTERED:
	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::AddMethod(BSTR bstrMethodName, IMethod* pmethod, VARIANT varPsbits, VARIANT varPermissions)
{
	// Just do an add method with the defaults defined in the idl
	HRESULT hr = S_OK;
	long psbits = PSBIT_SCRIPTDEFAULTMETHOD;
	long permissions = PS_SCRIPTDEFAULTMETHOD;
	
	hr = VariantToLong(varPsbits, &psbits, psbits);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
	
	hr = VariantToLong(varPermissions, &permissions, permissions);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = AddMethodExt(bstrMethodName, pmethod, psbits, permissions);

ERROR_ENCOUNTERED:
	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::AddMethodExt(BSTR bstrMethodName, IMethod* pmethod, long psbits, long permissions)
{
	HRESULT hr = S_OK;
	IThing* pExemplar = NULL;
	CWorldObject* pWorld = (CWorldObject*)m_pWorld;

	if (pWorld)
	{
		if ((pWorld->m_pSecurityContext) &&
			(pWorld->m_pThingThis == (IThing *)this))
		{
			hr = pWorld->m_pSecurityContext->get_Exemplar(&pExemplar);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
		else
		{
			pExemplar = (IThing *)this;
			SAFEADDREF(pExemplar);
		}
	}

	hr = AddMethodInt(bstrMethodName, pmethod, pExemplar, psbits, permissions);

ERROR_ENCOUNTERED:
	SAFERELEASE(pExemplar);

	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::AddMethodInt(BSTR bstrMethodName, IMethod* pmethod, IThing* pExemplar, 
										long psbits, long permissions)
{
	HRESULT hr = S_OK;
	CComVariant varTemp;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;
	CWorldObject* pWorld = (CWorldObject*)m_pWorld;

	ASSERT(bstrMethodName);
	if (bstrMethodName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::AddMethodExtHelper: incoming argument (BSTR) is NULL\n");
		return ReportThingError(E_INVALIDARG);
	}

	ASSERT(pmethod);
	if (pmethod == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::AddMethodExtHelper: incoming argument (IMethod*) is NULL\n");
		return ReportThingError(E_INVALIDARG);
	}

	ASSERT(pExemplar);
	if (pExemplar == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::AddMethodExtHelper: incoming argument (IThing*) is NULL\n");
		return ReportThingError(E_INVALIDARG);
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::AddMethodExtHelper: stub object, shouldn't be accessing\n");
		hr = VWOBJECT_E_STUBOBJECT;
		goto ERROR_ENCOUNTERED;
	}

	if (m_pMethods == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::AddMethodExtHelper: method map is invalid\n");
		hr = VWOBJECT_E_INVALIDMETHODMAP;
		goto ERROR_ENCOUNTERED;
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::AddMethodExtHelper: name (%s)\n", CString(bstrMethodName));
#endif

	// if we're adding with execute, we must also have read permissions
	if (((permissions & PS_OWNER_E) && !(permissions & PS_OWNER_R)) ||
		((permissions & PS_EXEMPLAR_E) && !(permissions & PS_EXEMPLAR_R)) ||
		((permissions & PS_ALL_E) && !(permissions & PS_ALL_R)))
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::AddMethodExtHelper: name (%s), invalid permissions, no PS_READ with PS_EXECUTE, %x\n", CString(bstrMethodName), permissions);
#endif

		hr = VWOBJECT_E_INVALIDPERMISSIONS;
		goto ERROR_ENCOUNTERED;
	}

	if (m_pWorld)
	{
		hr = m_pWorld->ValidateMethodNameExt(bstrMethodName, ((CMethodObject *)pmethod)->m_lFlags);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	// clone incoming method to varTemp
	hr = m_pWorld->CloneProperty(CComVariant(pmethod), &varTemp);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	ASSERT(varTemp.vt == VT_DISPATCH);
	pmethod = (IMethod*)varTemp.pdispVal;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(psbits, DISPID_THING_ADDMETHODINT, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(bstrMethodName), "CThingObject::AddMethodExtHelper", "VWOBJECT");
#endif	
	}

	// if server-side or client-side and not connected or local mode
	if (bShouldExecute == VARIANT_TRUE)
	{
		hr = AddMethodIntHelper(bstrMethodName, pmethod, pExemplar, psbits, permissions);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 5);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[4].vt = VT_BSTR;
		dispparams.rgvarg[4].bstrVal = bstrMethodName;

		dispparams.rgvarg[3].vt = VT_DISPATCH;
		dispparams.rgvarg[3].pdispVal = pmethod;

		dispparams.rgvarg[2].vt = VT_DISPATCH;
		dispparams.rgvarg[2].pdispVal = pExemplar;

		dispparams.rgvarg[1].vt = VT_I4;
		dispparams.rgvarg[1].lVal = psbits;

		dispparams.rgvarg[0].vt = VT_I4;
		dispparams.rgvarg[0].lVal = permissions;

		hr = SendThingCommand(PSBIT_NONE, DISPID_THING_ADDMETHODINT, DISPATCH_METHOD, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		hr = NotifyMethodChangedExt(bstrMethodName, thAdd, CComVariant());
	}

ERROR_ENCOUNTERED:
	return hr;
}

HRESULT CThingObject::AddMethodIntHelper(BSTR bstrMethodName, IMethod* pmethod, IThing* pExemplar, 
										 long psbits, long permissions)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL fValid = VARIANT_FALSE;
	IPropertySecurity* pSecurity = NULL;
	CWorldObject* pWorld = (CWorldObject*)m_pWorld;

	// check the thing's method map for PS_ADD
	if (FAILED(CheckMethodsSecurityExt(PS_ADD)))
	{
		CComBSTR bstrThingName("<unknown>");

		/* hr = */ get_Name(&bstrThingName.m_str);

		CString strInfo;
		strInfo.Format("Security violation: %s.AddMethod(\"%s\") denied, no add permissions", CString(bstrThingName), CString(bstrMethodName));
		m_pWorld->Report(CComBSTR(strInfo), -1);

		hr = VWOBJECT_E_PERMISSION_DENIED;
		goto ERROR_ENCOUNTERED;
	}

	hr = IsValidMethod(bstrMethodName, &fValid);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// if prop exists in inheritance tree, then fail
	// (exists in CThingObject, not just m_pMethods)

	if (fValid)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::AddMethodIntHelper: method already exists (%s)\n", CString(bstrMethodName));
#endif
		hr = VWOBJECT_E_METHODEXISTS;
		goto ERROR_ENCOUNTERED;
	}

	// set back-ptr and prop name in object properties
	hr = SetMethodName(bstrMethodName, pmethod);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = m_pMethods->put_PropertyInt(bstrMethodName, CComVariant(pmethod), NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// set the security name and exemplar
	hr = get_MethodSecurityExt(bstrMethodName, HashKey<BSTR>(bstrMethodName), NULL, NULL, &pSecurity);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (pSecurity == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::AddMethodIntHelper: property security is invalid\n");
		hr = VWOBJECT_E_INVALIDSECURITY;
		goto ERROR_ENCOUNTERED;
	}

	// Set bits and permissions
	pSecurity->put_Bits(psbits);
	pSecurity->put_Permissions(permissions);
	pSecurity->put_Exemplar(pExemplar);
	pSecurity->put_Instance(this);
	pSecurity->Enable();

#ifndef DISABLE_PROPERTYBIT_CACHING
	hr = CachePropertyBits(bstrMethodName, VARIANT_TRUE);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
#endif

ERROR_ENCOUNTERED:
	SAFERELEASE(pSecurity);	
	return hr;
}

STDMETHODIMP CThingObject::RemoveMethod(BSTR bstrMethodName)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;
	VARIANT_BOOL fInstance = VARIANT_FALSE;
	IPropertySecurity* pSecurity = NULL;
	long psbits = 0;
	CWorldObject* pWorld = (CWorldObject*)m_pWorld;

	ASSERT(bstrMethodName);
	if (bstrMethodName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::RemoveMethod: incoming argument (BSTR) is NULL\n");
		return ReportThingError(E_INVALIDARG);
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::RemoveMethod: stub object, shouldn't be accessing\n");
		hr = VWOBJECT_E_STUBOBJECT;
		goto ERROR_ENCOUNTERED;
	}

	if (m_pMethods == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::RemoveMethod: method map is invalid\n");
		hr = VWOBJECT_E_INVALIDMETHODMAP;
		goto ERROR_ENCOUNTERED;
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::RemoveMethod: name (%s)\n", 
		CString(bstrMethodName));
#endif

	hr = get_MethodSecurityExt(bstrMethodName, HashKey<BSTR>(bstrMethodName), NULL, &fInstance, &pSecurity);
	if (FAILED(hr)) 
		goto ERROR_ENCOUNTERED;

	if (pSecurity == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::RemoveMethod: property security is invalid\n");
		hr = VWOBJECT_E_INVALIDSECURITY;
		goto ERROR_ENCOUNTERED;
	}

	psbits = ((CPropertySecurity*)pSecurity)->m_ulBits;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(psbits, DISPID_THING_REMOVEMETHOD, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(bstrMethodName), "CThingObject::RemoveMethod", "VWOBJECT");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
#ifdef DISABLE_REMOVE_INTERNALLYADDED
		// REVIEW: check special case for internally added method
		if (psbits & PSBIT_INTERNALLYADDED)
		{
			CComBSTR bstrThingName("<unknown>");

			/* hr = */ get_Name(&bstrThingName.m_str);

			CString strInfo;
			strInfo.Format("Security violation: %s.RemoveMethod(\"%s\") denied, cannot remove internally added methods", CString(bstrThingName), CString(bstrMethodName));
			m_pWorld->Report(CComBSTR(strInfo), -1);

			hr = VWOBJECT_E_PERMISSION_DENIED;
			goto ERROR_ENCOUNTERED;
		}
#endif

		// check thing's property map for PS_ADD
		if (FAILED(CheckMethodsSecurityExt(PS_ADD)))
		{
			CComBSTR bstrThingName("<unknown>");

			/* hr = */ get_Name(&bstrThingName.m_str);

			CString strInfo;
			strInfo.Format("Security violation: %s.RemoveMethod(\"%s\") denied, no remove permissions", CString(bstrThingName), CString(bstrMethodName));
			m_pWorld->Report(CComBSTR(strInfo), -1);

			hr = VWOBJECT_E_PERMISSION_DENIED;
			goto ERROR_ENCOUNTERED;
		}

		// check the method's security for PS_ADD
		if (FAILED(CheckMethodSecurityExt(bstrMethodName, pSecurity, fInstance, PS_ADD)))
		{
			CComBSTR bstrThingName("<unknown>");

			/* hr = */ get_Name(&bstrThingName.m_str);

			CString strInfo;
			strInfo.Format("Security violation: %s.RemoveMethod(\"%s\") denied, no remove permissions", CString(bstrThingName), CString(bstrMethodName));
			m_pWorld->Report(CComBSTR(strInfo), -1);

			hr = VWOBJECT_E_PERMISSION_DENIED;
			goto ERROR_ENCOUNTERED;
		}

		{
			VARIANT* pvar = &((CPropertySecurity*)pSecurity)->m_var;
			CMethodPtr methodPtr(*pvar);

			// clear back-ptr and prop name in method
			if (methodPtr != NULL)
				/* hr = */ ClearMethodName(methodPtr);
		}

		hr = m_pMethods->RemoveInt(bstrMethodName);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

#ifndef DISABLE_PROPERTYBIT_CACHING
		hr = CachePropertyBits(bstrMethodName, VARIANT_TRUE);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
#endif
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstrMethodName;

		hr = SendThingCommand(PSBIT_NONE, DISPID_THING_REMOVEMETHOD, DISPATCH_METHOD, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		hr = NotifyMethodChangedExt(bstrMethodName, thRemove, CComVariant());
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pSecurity);

	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::get_Method(BSTR bstrMethodName, IMethod** ppmethod)
{
	HRESULT hr = S_OK;
	UINT nHashOrig = HashKey<BSTR>(bstrMethodName);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::get_Method: name (%s)\n", CString(bstrMethodName));
#endif

	hr = get_MethodExt(bstrMethodName, nHashOrig, NULL, ppmethod);

	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::get_MethodExt(BSTR bstrMethodName, UINT nHashOrig, IThing** ppattached, IMethod** ppmethod)
{
	HRESULT hr = S_OK;
	CComVariant var;
	VARIANT_BOOL fInstance = VARIANT_FALSE;
	IPropertySecurity* pSecurity = NULL;
	IThing* pSource = NULL;
	VARIANT* pvar = NULL;

	ASSERT(bstrMethodName);
	if (bstrMethodName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_MethodExt: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	// ppattached can be NULL
	if (ppattached)
		*ppattached = NULL;

	ASSERT(ppmethod);
	if (ppmethod == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_MethodExt: outgoing pointer (IMethod**) is NULL\n");
		return E_POINTER;
	}

	*ppmethod = NULL;

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_MethodExt: stub object, shouldn't be accessing\n");
		return VWOBJECT_E_STUBOBJECT;
	}

	if (m_pMethods == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::get_MethodExt: method map is invalid\n");
		return VWOBJECT_E_INVALIDMETHODMAP;
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::get_MethodExt: name (%s)\n", CString(bstrMethodName));
#endif

	hr = get_MethodSecurityExt(bstrMethodName, nHashOrig, &pSource, &fInstance, &pSecurity);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;
/*
	// check the method's security for PS_READ
	if (FAILED(CheckMethodSecurityExt(bstrMethodName, pSecurity, fInstance, PS_READ)))
	{
		CComBSTR bstrThingName("<unknown>");

		 get_Name(&bstrThingName.m_str);

		CString strInfo;
		strInfo.Format("Security violation: %s.get_Method(\"%s\") denied, no read permissions", CString(bstrThingName), CString(bstrMethodName));
		m_pWorld->Report(CComBSTR(strInfo), -1);

		hr = VWOBJECT_E_PERMISSION_DENIED;
		goto ERROR_ENCOUNTERED;
	}
*/
	pvar = &((CPropertySecurity*)pSecurity)->m_var;

	ASSERT(pvar->vt == VT_DISPATCH);
	*ppmethod = (IMethod*)pvar->pdispVal;
	SAFEADDREF(*ppmethod);

	if (ppattached)
	{
		*ppattached = pSource;
		SAFEADDREF(*ppattached);
	}

#ifdef SUPPORT_DATESTAMPS
	SetAccessedDate();
#endif

ERROR_ENCOUNTERED:
	SAFERELEASE(pSecurity);
	SAFERELEASE(pSource);

	return hr;
}

STDMETHODIMP CThingObject::get_MethodSecurity(BSTR bstrMethodName, IPropertySecurity** ppSecurity)
{
	HRESULT hr = S_OK;
	UINT nHashOrig = HashKey<BSTR>(bstrMethodName);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::get_MethodSecurity: name (%s)\n", CString(bstrMethodName));
#endif

	hr = get_MethodSecurityExt(bstrMethodName, nHashOrig, NULL, NULL, ppSecurity);

	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::get_MethodSecurityExt(BSTR bstrMethodName, UINT nHashOrig, IThing** ppattached, VARIANT_BOOL* pfInstance, IPropertySecurity** ppSecurity)
{
	HRESULT hr = S_OK;
	IThing* pSource = NULL;
	IPropertyMap* pMethods = NULL;

	ASSERT(bstrMethodName);
	if (bstrMethodName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_MethodSecurityExt: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	// NOTE: ppattached can be NULL
	if (ppattached)
		*ppattached = NULL;

	// pfInstance can be NULL
	if (pfInstance)
		*pfInstance = VARIANT_FALSE;

	ASSERT(ppSecurity);
	if (ppSecurity == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_MethodSecurityExt: outgoing pointer (IPropertySecurity**) is NULL\n");
		return E_POINTER;
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_MethodSecurityExt: stub object, shouldn't be accessing\n");
		return VWOBJECT_E_STUBOBJECT;
	}

	if (m_pMethods == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::get_MethodSecurityExt: method map is invalid\n");
		return VWOBJECT_E_INVALIDMETHODMAP;
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::get_MethodSecurityExt: name (%s)\n", CString(bstrMethodName));
#endif

	// check thing's method map for PS_READ
	if (FAILED(CheckMethodsSecurityExt(PS_READ)))
	{
		CComBSTR bstrThingName("<unknown>");

		/* hr = */ get_Name(&bstrThingName.m_str);

		CString strInfo;
		strInfo.Format("Security violation: %s.get_MethodSecurity(\"%s\") denied, no read permissions", CString(bstrThingName), CString(bstrMethodName));
		m_pWorld->Report(CComBSTR(strInfo), -1);

		hr = VWOBJECT_E_PERMISSION_DENIED;
		goto ERROR_ENCOUNTERED;
	}

	// initialize pointers
	pSource = this;
	SAFEADDREF(pSource);

	pMethods = m_pMethods;
	SAFEADDREF(pMethods);

	// lookup method in object
	hr = pMethods->get_PropertySecurityExt(bstrMethodName, nHashOrig, ppSecurity);

	// if method doesn't exist, keep searching
	while (hr == VWOBJECT_E_PROPERTYNOTEXIST)
	{
		// release old method map
		SAFERELEASE(pMethods);

		// get new source
		IThing* pOldSource = pSource;

		hr = pOldSource->get_Exemplar(&pSource);

		// release old source
		SAFERELEASE(pOldSource);

		if (FAILED(hr))
			break;

		if (pSource)
		{
			// get new method map
			hr = pSource->get_InstanceMethods(&pMethods);
			if (FAILED(hr))
				break;

			if (pMethods == NULL)
			{
				hr = VWOBJECT_E_INVALIDMETHODMAP;
				break;
			}

			// lookup method in new source
			hr = pMethods->get_PropertySecurityExt(bstrMethodName, nHashOrig, ppSecurity);
		}
		else
		{
			hr =  VWOBJECT_E_METHODNOTEXIST;
			goto ERROR_ENCOUNTERED;
		}
	}

	if (pSource == this)
	{
		if (pfInstance)
			*pfInstance = VARIANT_TRUE;
	}

	// Now we should have the security for the slot - if it exists, check that we have read permission
	if (*ppSecurity == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::get_MethodSecurityExt: property security is invalid\n");
		hr = VWOBJECT_E_INVALIDSECURITY;
		goto ERROR_ENCOUNTERED;
	}
	
	// Note: if no pfInstance, assume it is an instance method (could be adding)
	if (FAILED(CheckMethodSecurityExt(bstrMethodName, *ppSecurity, ((pfInstance) ? *pfInstance : VARIANT_TRUE), PS_READ)))
	{
		CComBSTR bstrThingName("<unknown>");

		 get_Name(&bstrThingName.m_str);

		CString strInfo;
		strInfo.Format("Security violation: %s.get_Method(\"%s\") denied, no read permissions", CString(bstrThingName), CString(bstrMethodName));
		m_pWorld->Report(CComBSTR(strInfo), -1);

		hr = VWOBJECT_E_PERMISSION_DENIED;
		goto ERROR_ENCOUNTERED;
	}

	// set the instance/exemplar where the method was found
	if (ppattached)
	{
		*ppattached = pSource;
		SAFEADDREF(*ppattached);
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pMethods);
	SAFERELEASE(pSource);

	return hr;
}

STDMETHODIMP CThingObject::put_Method(BSTR bstrMethodName, IMethod* pmethod)
{
	HRESULT hr = S_OK;
	UINT nHashOrig = HashKey<BSTR>(bstrMethodName);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::put_Method: name (%s)\n", CString(bstrMethodName));
#endif

	hr = put_MethodExt(bstrMethodName, nHashOrig, pmethod);

	return ReportThingError(hr);
}

HRESULT CThingObject::put_MethodExt(BSTR bstrMethodName, UINT nHashOrig, IMethod* pmethod)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;
	CWorldObject* pWorld = (CWorldObject*)m_pWorld;
	CComVariant varTemp;
	IPropertySecurity* pSecurity = NULL;
	long psbits = 0L;
	VARIANT_BOOL fInstance = VARIANT_FALSE;

	ASSERT(bstrMethodName);
	if (bstrMethodName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::put_MethodExt: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::put_MethodExt: stub object, shouldn't be accessing\n");
		hr = VWOBJECT_E_STUBOBJECT;
		goto ERROR_ENCOUNTERED;
	}

	if (m_pMethods == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::put_MethodExt: method map is invalid\n");
		hr = VWOBJECT_E_INVALIDMETHODMAP;
		goto ERROR_ENCOUNTERED;
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::put_MethodExt: name (%s)\n", 
		CString(bstrMethodName));
#endif

	// clone incoming method to varTemp
	hr = m_pWorld->CloneProperty(CComVariant(pmethod), &varTemp);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	ASSERT(varTemp.vt == VT_DISPATCH);
	pmethod = (IMethod*)varTemp.pdispVal;

	// The existing Security... Could be in the Exemplar
	hr = get_MethodSecurityExt(bstrMethodName, nHashOrig, NULL, &fInstance, &pSecurity);
	if (FAILED(hr)) 
		goto ERROR_ENCOUNTERED;

	if (pSecurity == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::put_MethodExt: property security is invalid\n");
		hr = VWOBJECT_E_INVALIDSECURITY;
		goto ERROR_ENCOUNTERED;
	}

	psbits = ((CPropertySecurity*)pSecurity)->m_ulBits;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(psbits, DISPID_THING_METHOD, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(bstrMethodName), "CThingObject::put_Method", "VWOBJECT");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		hr = put_MethodInt(bstrMethodName, pmethod, pSecurity, fInstance);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 2);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[1].vt = VT_BSTR;
		dispparams.rgvarg[1].bstrVal = bstrMethodName;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = pmethod;

		hr = SendThingCommand(PSBIT_NONE, DISPID_THING_METHOD, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		hr = NotifyMethodChangedExt(bstrMethodName, thPut, CComVariant());
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pSecurity);

	return hr;
}

HRESULT CThingObject::put_MethodInt(BSTR bstrMethodName, IMethod* pmethod, IPropertySecurity* pSecurity, VARIANT_BOOL fInstance)
{
	HRESULT hr = S_OK;

	if (pSecurity == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::put_MethodInt: property security is invalid\n");
		hr = VWOBJECT_E_INVALIDSECURITY;
		goto ERROR_ENCOUNTERED;
	}

	// check thing's method map for PS_WRITE
	if (FAILED(CheckMethodsSecurityExt(PS_WRITE)))
	{
		CComBSTR bstrThingName("<unknown>");

		/* hr = */ get_Name(&bstrThingName.m_str);

		CString strInfo;
		strInfo.Format("Security violation: %s.put_Method(\"%s\") denied, no write permissions", CString(bstrThingName), CString(bstrMethodName));
		m_pWorld->Report(CComBSTR(strInfo), -1);

		hr = VWOBJECT_E_PERMISSION_DENIED;
		goto ERROR_ENCOUNTERED;
	}

	if (FAILED(CheckMethodSecurityExt(bstrMethodName, pSecurity, fInstance, PS_WRITE)))
	{
		CComBSTR bstrThingName("<unknown>");

		/* hr = */ get_Name(&bstrThingName.m_str);

		CString strInfo;
		strInfo.Format("Security violation: %s.put_Method(\"%s\") denied, no write permissions", CString(bstrThingName), CString(bstrMethodName));
		m_pWorld->Report(CComBSTR(strInfo), -1);

		hr = VWOBJECT_E_PERMISSION_DENIED;
		goto ERROR_ENCOUNTERED;
	}

	{
		VARIANT* pvar = &((CPropertySecurity*)pSecurity)->m_var;
		CMethodPtr methodPtr(*pvar);

		// clear existing method, if already instanced
		if (methodPtr != NULL)
			/* hr = */ ClearMethodName(methodPtr);
	}

	hr = m_pMethods->put_PropertyInt(bstrMethodName, CComVariant(pmethod), pSecurity);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	{
		CMethodPtr methodPtr;
		IPropertySecurity* pNewSecurity = NULL;
		VARIANT* pvar = NULL;

		if (fInstance == VARIANT_FALSE)
		{
			{
				CSetUID SetUID(m_pWorld, this, this, VARIANT_TRUE);

				// grab the new security slot
				hr = m_pMethods->get_PropertySecurityExt(bstrMethodName, HashKey<BSTR>(bstrMethodName), &pNewSecurity);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED_1;
			}

			if (pNewSecurity == NULL)
				goto ERROR_ENCOUNTERED_1;

			pvar = &((CPropertySecurity*)pNewSecurity)->m_var;
		}
		else
		{
			pvar = &((CPropertySecurity*)pSecurity)->m_var;
		}

		methodPtr = *pvar;

		// set back-ptr and method name in object properties
		if (methodPtr != NULL)
		{
			hr = SetMethodName(bstrMethodName, methodPtr);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED_1;
		}

ERROR_ENCOUNTERED_1:
		if (fInstance == VARIANT_FALSE)
		{
			// for methods, we need to fix up the exemplar to point to us
			pNewSecurity->put_Exemplar(this);

			// make sure the instance is pointing to this instance
			pNewSecurity->put_Instance(this);
			pNewSecurity->Enable();

			SAFERELEASE(pNewSecurity);
		}
	}

#ifndef DISABLE_PROPERTYBIT_CACHING
	// don't need to cache on put
//	hr = CachePropertyBits(bstrMethodName, VARIANT_TRUE);
//	if (FAILED(hr))
//		goto ERROR_ENCOUNTERED;
#endif

ERROR_ENCOUNTERED:
	return hr;
}

#ifndef DISABLE_PROPERTYBIT_CACHING
HRESULT CThingObject::CachePropertyBits(BSTR bstrName, VARIANT_BOOL bMethod)
{
	HRESULT hr = S_OK;
	IPropertySecurity* pSecurity = NULL;
	VARIANT_BOOL bOriginalAccess = VARIANT_FALSE;
	VARIANT_BOOL bOriginalChanged = VARIANT_FALSE;
	VARIANT_BOOL bAccess = VARIANT_FALSE;
	VARIANT_BOOL bChanged = VARIANT_FALSE;
	CComBSTR bstrPropertyName;
	VARIANT_BOOL bIsExemplar = VARIANT_FALSE;

	/* hr = */ m_pWorld->IsExemplar(this, &bIsExemplar);

	// if we're testing a method
	if (bMethod == VARIANT_TRUE)
	{
		CString strMethodName(bstrName);

		if (strMethodName.Left(2) == m_strOn)
		{
			// does method name fit On<Foo>Access pattern?
			if (strMethodName.Right(6) == m_strAccess) 
				bAccess = VARIANT_TRUE;
			// does method name fit On<Foo>Changed pattern?
			else if (strMethodName.Right(7) == m_strChanged) 
				bChanged = VARIANT_TRUE;

			// if neither set, we won't have a property name to tweak the bits on
			if (bAccess || bChanged)
			{
				CString strPropertyName(bstrName);

				// strip out "On"
				strPropertyName.Replace(m_strOn, "");

				// strip out "Access" or "Changed"
				strPropertyName.Replace(bAccess ? m_strAccess : m_strChanged, "");

				bstrPropertyName = strPropertyName;
			}
		}
	}
	// else, if we're testing a property
	else
	{
		CComBSTR bstrPropEvent = m_bstrOn;
		bstrPropEvent += bstrName;
		bstrPropEvent += m_bstrAccess;

		// does On<Foo>Access method exist?
		/* hr = */ IsValidInstanceMethod(bstrPropEvent, &bAccess);

		bstrPropEvent = m_bstrOn;
		bstrPropEvent += bstrName;
		bstrPropEvent += m_bstrChanged;

		// does On<Foo>Changed method exist?
		/* hr = */ IsValidInstanceMethod(bstrPropEvent, &bChanged);

		bstrPropertyName = bstrName;
	}

	if ((BSTR)bstrPropertyName != NULL)
	{
		// set bits in property
		if (SUCCEEDED(m_pProperties->get_PropertySecurityExt(bstrPropertyName, HashKey<BSTR>(bstrPropertyName), &pSecurity)) && pSecurity != NULL)
		{
			if (bAccess || bChanged)
			{
				/* hr = */ pSecurity->get_BitState(PSBIT_NOTIFYONACCESS, &bOriginalAccess);
				/* hr = */ pSecurity->get_BitState(PSBIT_NOTIFYONCHANGE, &bOriginalChanged);

#ifdef TRACE_PROPERTYBIT_CACHING
				CComBSTR bstrName("n/a");

				/* hr = */ get_Name(&bstrName.m_str);

				TRACE("CThingObject::CachePropertyBits: this (%s), exemplar %s, property (%s), original flags: access %s, changed %s, new flags: access %s, changed %s\n", 
					CString(bstrName), bIsExemplar ? "yes" : "no",
					CString(bstrPropertyName),
					bOriginalAccess ? "yes" : "no", bOriginalChanged ? "yes" : "no",
					bAccess ? "yes" : "no", bChanged ? "yes" : "no");
#endif

				/* hr = */ pSecurity->put_BitState(PSBIT_NOTIFYONACCESS, bAccess ? VARIANT_TRUE : bOriginalAccess);
				/* hr = */ pSecurity->put_BitState(PSBIT_NOTIFYONCHANGE, bChanged ? VARIANT_TRUE : bOriginalChanged);
			}

			SAFERELEASE(pSecurity);	
		}
	}

	return hr;
}
#endif

STDMETHODIMP CThingObject::InvokeSuperMethodExt(DISPPARAMS* pdispparams, VARIANT* pvar)
{
	HRESULT hr = S_OK;
	IThing* pSuper = NULL;
	CWorldObject* pWorld = (CWorldObject*)m_pWorld;

//	/* hr = */ m_pWorld->get_Super(&pSuper);

	pSuper = pWorld->m_pThingSuper;
	SAFEADDREF(pSuper);

#ifdef _DEBUG
	{
		CComBSTR bstrName;
		CComBSTR bstrProp;

		pSuper->get_Name(&bstrName.m_str);

		// took out PROPGET/PROPPUT stuff

		bstrProp = m_bstrCurrentMethod;

		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::InvokeSuperMethodExt: super (%s), method (%s)\n", CString(bstrName), CString(bstrProp));
	}
#endif

	{
		CSetUID SetUID(m_pWorld, this, pSuper /*, VARIANT_TRUE */);
		
		hr = InvokeMethodExtHelper(pSuper, m_bstrCurrentMethod, pdispparams, pvar);
	}

	if (hr == VWOBJECT_E_METHODNOTEXIST || hr == VWOBJECT_E_INVALIDMETHODMAP)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::InvokeSuperMethodExt: method (%s) not found\n", CString(m_bstrCurrentMethod));

		hr = S_OK;
	}

	SAFERELEASE(pSuper);

	return hr;
}

HRESULT CThingObject::InvokeMethodExtHelper(IThing* pThing, BSTR bstrMethodName, DISPPARAMS* pdispparams, VARIANT* pvar)
{
	HRESULT hr = S_OK;
	IThing* pAttached = NULL;
	IMethod* pMethod = NULL;
	long lMethodFlags = 0L;
	IThing* pOldInstance = NULL;
	IPropertySecurity* pSecurity = NULL;
	CWorldObject* pWorld = (CWorldObject*)m_pWorld;
#ifdef _DEBUG
	CString strName(bstrMethodName);
	CString strProp(strName);
#endif
	VARIANT_BOOL bServerSide = VARIANT_FALSE;
	VARIANT_BOOL bConnected = VARIANT_FALSE;
	HRESULT invokehr = S_OK;
	UINT nHashOrig = HashKey<BSTR>(bstrMethodName);

	DISPPARAMS dispparamsCopy;
	InitializeDISPPARAMS(&dispparamsCopy, 0);

	ASSERT(bstrMethodName);
	if (bstrMethodName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::InvokeMethodExtHelper: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	ASSERT(pThing);
	if (pThing == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::InvokeMethodExtHelper: incoming argument (IThing*) is NULL\n");
		return E_INVALIDARG;
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::InvokeMethodExtHelper: stub object, shouldn't be accessing\n");
		hr = VWOBJECT_E_STUBOBJECT;
		goto ERROR_ENCOUNTERED;
	}

	// NOTE: we want to kick out early since this will fail often

	// NOTE: we pass in pThing which is used to lookup the method (could be != this)
	hr = pThing->get_MethodExt(bstrMethodName, nHashOrig, &pAttached, &pMethod);
	if (FAILED(hr))
	{
#ifdef SUPPORT_ONINVOKEMETHODFAILED
		invokehr = hr;

		static CComBSTR bstrInvokeFailed("OnInvokeMethodFailed");

		// don't recurse into OnInvokeMethodFailed if the method itself doesn't exist
		if (CompareElements(&bstrMethodName, &bstrInvokeFailed.m_str))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::InvokeMethodExtHelper: failed to get OnInvokeMethodFailed method\n");
		}
		else
		{
			CString strMethodName(bstrMethodName);

			if (m_strOn == strMethodName.Left(2) && m_strChanged == strMethodName.Right(7))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::InvokeMethodExtHelper: attempted to call (%s), skipping OnInvokeMethodFailed\n", CString(bstrMethodName));
			}
			else if (m_strOn == strMethodName.Left(2) && m_strAccess == strMethodName.Right(6))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::InvokeMethodExtHelper: attempted to call (%s), skipping OnInvokeMethodFailed\n", CString(bstrMethodName));
			}
			else
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::InvokeMethodExtHelper: failed to get method by name (%s), calling OnInvokeMethodFailed\n", CString(bstrMethodName));

				VARIANT_BOOL bValidMethod = VARIANT_FALSE;

				// check if method exists first, before assembling DISPPARAMS, etc.
				if (SUCCEEDED(IsValidMethod(bstrInvokeFailed, &bValidMethod)))
				{
					if (bValidMethod == VARIANT_FALSE)
						goto ERROR_ENCOUNTERED_2;
				}

				if (pdispparams)
				{
					// only create arglist if needed
					if (m_pArgList == NULL)
					{
						hr = CreatePropertyList(m_pWorld, &m_pArgList);
						if (FAILED(hr))
							goto ERROR_ENCOUNTERED_2;
					}

					// REVIEW: we can just use the passed-in dispparams, since they'll be copied anyway
					hr = ConvertDISPPARAMSToPropertyList(pdispparams /* &dispparamsCopy */, m_pArgList);
					if (FAILED(hr))
						goto ERROR_ENCOUNTERED_2;
				}

				{
					DISPPARAMS dispparams;

					// package up args into DISPPARAMS struct
					hr = InitializeDISPPARAMS(&dispparams, 2);
					if (FAILED(hr))
						goto ERROR_ENCOUNTERED_2;

					dispparams.rgvarg[1].vt = VT_BSTR;
					dispparams.rgvarg[1].bstrVal = bstrMethodName;

					// NOTE: if NULL dispparams passed in, we need to make sure
					// it's treated as such on other side - not as 0 parameters
					dispparams.rgvarg[0].vt = VT_DISPATCH;
					dispparams.rgvarg[0].pdispVal = (pdispparams) ? m_pArgList : NULL;

					// REVIEW: not using HRESULT from OnInvokeMethodFailed call
					/* hr = */ InvokeMethodExtHelper(pThing, bstrInvokeFailed, &dispparams, pvar);

					TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

					if (m_pArgList)
						m_pArgList->RemoveAll();
				}
			}
		}

ERROR_ENCOUNTERED_2:
		hr = invokehr;
#endif

		goto ERROR_ENCOUNTERED;
	}

	if (pdispparams != NULL)
	{
		// copy incoming DISPPARAMS to temporary DISPPARAMS,
		// then validate property types

		InitializeDISPPARAMS(&dispparamsCopy, pdispparams->cArgs);

		for (unsigned int i = 0; i < pdispparams->cArgs; i++)
		{
			// copy to new DISPPARAMS, freeing source
			::VariantCopyInd(&dispparamsCopy.rgvarg[i], &pdispparams->rgvarg[i]);
		}
	}

	// NOTE: never fails so don't check hr
	// Check security early on, don't even try to remote failed methods.
//	pMethod->get_Security(&pSecurity);

	pSecurity = ((CMethodObject*)pMethod)->m_pSecurity;
	SAFEADDREF(pSecurity);

	if (pSecurity == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::InvokeMethodExtHelper: invalid security ptr from method\n");
		hr = VWOBJECT_E_INVALIDSECURITY;
		goto ERROR_ENCOUNTERED;
	}

	// Set the instance to this, methods are never actually copied on get.
//	pSecurity->get_Instance(&pOldInstance);
//	pSecurity->put_Instance(this);

	pOldInstance = ((CPropertySecurity*)pSecurity)->m_pInstance; 
	SAFEADDREF(pOldInstance); 

	SAFERELEASE(((CPropertySecurity*)pSecurity)->m_pInstance); 
	((CPropertySecurity*)pSecurity)->m_pInstance = this; 
	SAFEADDREF(((CPropertySecurity*)pSecurity)->m_pInstance); 

	if (!QUIET_SAFE_CHECKSECURITY(m_pWorld, pSecurity, PS_EXECUTE))
	{
		CComBSTR bstrThingName("<unknown>");

		/* hr = */ get_Name(&bstrThingName.m_str);

		CString strInfo;
		strInfo.Format("Security violation: %s.InvokeMethod(\"%s\") denied, no execute permissions", CString(bstrThingName), CString(bstrMethodName));
		m_pWorld->Report(CComBSTR(strInfo), -1);

		hr = VWOBJECT_E_PERMISSION_DENIED;
		goto ERROR_ENCOUNTERED;
	} 

	lMethodFlags = ((CMethodObject *)pMethod)->m_lFlags;

//	hr = pMethod->get_Flags(&lMethodFlags);
//	if (FAILED(hr))
//		goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
	if (lMethodFlags & METHOD_PROPERTYPUT)
		strProp = "put " + strName;
	else if (lMethodFlags & METHOD_PROPERTYGET)
		strProp = "get " + strName;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::InvokeMethodExtHelper: (%s)\n", strProp);
#endif

	bServerSide = pWorld->m_bServerSide;
	bConnected = pWorld->m_bConnected;

	// if client-side and method is client-side
	// or, if server-side and method is server-side
	// or, if client-side and not connected (invoke both client-side and server-side methods)
	if ((bServerSide == VARIANT_FALSE && (lMethodFlags & METHOD_CLIENT)) ||
		(bServerSide == VARIANT_TRUE && (lMethodFlags & METHOD_SERVER)) ||
		(bServerSide == VARIANT_FALSE && bConnected == VARIANT_FALSE))
	{
//		VARIANT_BOOL bFreezeEvents = VARIANT_TRUE;
		VARIANT_BOOL bOldInPropChange = VARIANT_TRUE;
		VARIANT_BOOL bOldShouldRemote = VARIANT_TRUE;
		CComBSTR bstrOldMethod;
		IThing* pThingOldThis = NULL;
		IThing* pThingOldCaller = NULL;
		IPropertySecurity* pOldSecurity = NULL;
		IThing* pThingOldSuper = NULL;

		// REVIEW: need to protect This and Caller state during thread reentrancy
		m_pWorld->LockWorld();

		// done already up above
//		pSecurity->put_Instance(this);

		bOldInPropChange = pWorld->m_bInPropChange;
		bOldShouldRemote = pWorld->m_bShouldRemote;

		pThingOldThis = pWorld->m_pThingThis;
		SAFEADDREF(pThingOldThis);

		pThingOldCaller = pWorld->m_pThingCaller;
		SAFEADDREF(pThingOldCaller);

		pOldSecurity = pWorld->m_pSecurityContext;
		SAFEADDREF(pOldSecurity);

		pThingOldSuper = pWorld->m_pThingSuper;
		SAFEADDREF(pThingOldSuper);

		bstrOldMethod = m_bstrCurrentMethod;

		SAFERELEASE(pWorld->m_pThingThis);
		pWorld->m_pThingThis = this;
		SAFEADDREF(pWorld->m_pThingThis);

		// the old this is now the current caller
		// unless we are at the top of the call stack, then it's the user
		if (pThingOldThis)
		{
			SAFERELEASE(pWorld->m_pThingCaller);
			pWorld->m_pThingCaller = pThingOldThis;
			SAFEADDREF(pWorld->m_pThingCaller);
		}
		// else, if we have a caller coming in (like when remoted from the client or server) don't set to be the user
		else if (pThingOldCaller == NULL)
		{
			SAFERELEASE(pWorld->m_pThingCaller);
			pWorld->m_pThingCaller = pWorld->m_pUser;
			SAFEADDREF(pWorld->m_pThingCaller);
		}

		// set the security of this method
		SAFERELEASE(pWorld->m_pSecurityContext);
		pWorld->m_pSecurityContext = pSecurity;
		SAFEADDREF(pWorld->m_pSecurityContext);

		if (pAttached)
		{
			IThing* pExemplar2 = NULL;

			// get exemplar of exemplar
//			pAttached->get_Exemplar(&pExemplar2);

			pExemplar2 = ((CThingObject*)pAttached)->m_pExemplar;
			SAFEADDREF(pExemplar2);

			if (pExemplar2)
			{
				SAFERELEASE(pWorld->m_pThingSuper);
				pWorld->m_pThingSuper = pExemplar2;
				SAFEADDREF(pWorld->m_pThingSuper);

				SAFERELEASE(pExemplar2);
			}
		}

		m_bstrCurrentMethod = bstrMethodName;

#ifdef _DEBUG
		{
			CComBSTR bstrThis("unknown");
			CComBSTR bstrCaller("unknown");
			CComBSTR bstrAttached("unknown");
			CComBSTR bstrSuper("unknown");
			IThing* pCaller = NULL;
			IThing* pSuper = NULL;

			/* hr = */ get_Name(&bstrThis.m_str);

			m_pWorld->get_Caller(&pCaller);
			m_pWorld->get_Super(&pSuper);

			if (pCaller)
				/* hr = */ pCaller->get_Name(&bstrCaller.m_str);

			if (pAttached)
				/* hr = */ pAttached->get_Name(&bstrAttached.m_str);

			if (pSuper)
				/* hr = */ pSuper->get_Name(&bstrSuper.m_str);

			VWTRACE(m_pWorld, "VWOBJECT", METHOD_TRACE, "CThingObject::InvokeMethodExtHelper: method (%s), this (%s), caller (%s), attached (%s), super (%s)\n", 
				CString(bstrMethodName), CString(bstrThis), CString(bstrCaller), CString(bstrAttached), CString(bstrSuper));

			SAFERELEASE(pCaller);
			SAFERELEASE(pSuper);
		}
#endif

		// push value since we're now not in a prop change
		pWorld->m_bInPropChange = VARIANT_FALSE;

		// push ShouldRemote so further prop changes get remoted
		pWorld->m_bShouldRemote = VARIANT_TRUE;

		hr = pMethod->InvokeMethodExt(&dispparamsCopy, &invokehr, pvar);

		if (FAILED(hr) && hr != VWOBJECT_E_METHODNOTEXIST && hr != VWOBJECT_E_INVALIDMETHODMAP)
		{
			// return the "real" HRESULT in this case
			if (hr == DISP_E_EXCEPTION)
				hr = invokehr;

#ifdef _DEBUG
			CComBSTR bstrThis("unknown");

			/* hr = */ get_Name(&bstrThis.m_str);

			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::InvokeMethodExtHelper: this (%s), failed during invocation of method (%s), hr %x\n", 
				CString(bstrThis), CString(bstrMethodName), hr);
#endif

			goto ERROR_ENCOUNTERED_1;
		}

		if (pWorld->m_bFreezeEvents == VARIANT_FALSE)
		{
			try
			{
				// Don't do notify if we fail during method invocation (method must return successful invokehr)
				// REVIEW: notify world/client of invokemethod on instance, not on 
				// base exemplars
				// REVIEW: don't call on stub objects (which can happen if OnSystemDestroy was method being called)
				if ((SUCCEEDED(invokehr)) && (pThing == this) && (m_bStub == VARIANT_FALSE))
					/* hr = */ pWorld->NotifyInvokeMethod(this, bstrMethodName, &dispparamsCopy);
			}
			catch (...)
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::InvokeMethodExtHelper: caught exception during NotifyInvokeMethod (%s)\n", 
					CString(bstrMethodName));
			}
		}

		// REVIEW: set last invoke HRESULT into IWorld?
		
ERROR_ENCOUNTERED_1:
		SAFERELEASE(pWorld->m_pThingThis);
		pWorld->m_pThingThis = pThingOldThis;
		SAFEADDREF(pWorld->m_pThingThis);

		SAFERELEASE(pWorld->m_pThingCaller);
		pWorld->m_pThingCaller = pThingOldCaller;
		SAFEADDREF(pWorld->m_pThingCaller);

		SAFERELEASE(pWorld->m_pSecurityContext);
		pWorld->m_pSecurityContext = pOldSecurity;
		SAFEADDREF(pWorld->m_pSecurityContext);

		SAFERELEASE(pWorld->m_pThingSuper);
		pWorld->m_pThingSuper = pThingOldSuper;
		SAFEADDREF(pWorld->m_pThingSuper);

		m_bstrCurrentMethod = bstrOldMethod;
		
		SAFERELEASE(pThingOldThis);
		SAFERELEASE(pThingOldCaller);
		SAFERELEASE(pOldSecurity);
		SAFERELEASE(pThingOldSuper);

		pWorld->m_bInPropChange = bOldInPropChange;

		pWorld->m_bShouldRemote = bOldShouldRemote;

		// REVIEW: need to protect This and Caller state during thread reentrancy
		pWorld->UnlockWorld();
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// if client-side and method is server-side *only* (and client is connected)
	// or, if server-side and method is client-side	*only*
	if ((bServerSide == VARIANT_FALSE && (lMethodFlags & METHOD_SERVER) && ((lMethodFlags & METHOD_CLIENT) == 0) && bConnected == VARIANT_TRUE) ||
		(bServerSide == VARIANT_TRUE && (lMethodFlags & METHOD_CLIENT) && ((lMethodFlags & METHOD_SERVER) == 0)))
	{
		DISPPARAMS dispparams;
		HRESULT recvhr = S_OK;

#ifdef _DEBUG
		{
			CComBSTR bstrThis("unknown");

			/* hr = */ get_Name(&bstrThis.m_str);


			VWTRACE(m_pWorld, "VWOBJECT", METHOD_TRACE, "CThingObject::InvokeMethodExtHelper: remoting method (%s), this (%s)\n", 
				CString(bstrMethodName), CString(bstrThis));
		}
#endif

		// REVIEW: only validate DISPPARAMS if remoting the method

		// check for valid property types
		hr = m_pWorld->ValidateDISPPARAMS(&dispparamsCopy);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::InvokeMethodExtHelper: ValidateDISPPARAMS failed\n");
			goto ERROR_ENCOUNTERED;
		}

		if (pdispparams)
		{
			// only create arglist if needed
			if (m_pArgList == NULL)
			{
				hr = CreatePropertyList(m_pWorld, &m_pArgList);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;
			}

			hr = ConvertDISPPARAMSToPropertyList(&dispparamsCopy, m_pArgList);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 3);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[2].vt = VT_DISPATCH;
		dispparams.rgvarg[2].pdispVal = pThing;

		dispparams.rgvarg[1].vt = VT_BSTR;
		dispparams.rgvarg[1].bstrVal = bstrMethodName;

		// NOTE: if NULL dispparams passed in, we need to make sure
		// it's treated as such on other side - not as 0 parameters
		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = (pdispparams) ? m_pArgList : NULL;

		if (lMethodFlags & METHOD_SYNC)
		{
			// If this succeeds, set hr to be the value returned by the method invoked
			hr = SendSyncThingCommand(PSBIT_NONE, DISPID_THING_INVOKEMETHODINT, DISPATCH_METHOD, &dispparams, &recvhr, pvar);
			if (SUCCEEDED(hr))
				hr = recvhr;
		}
		else
			hr = SendThingCommand(PSBIT_NONE, DISPID_THING_INVOKEMETHODINT, DISPATCH_METHOD, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		if (m_pArgList)
			m_pArgList->RemoveAll();
	}

ERROR_ENCOUNTERED:
	if (pSecurity)
	{
//		pSecurity->put_Instance(pOldInstance);
//		SAFERELEASE(pOldInstance);

		SAFERELEASE(((CPropertySecurity*)pSecurity)->m_pInstance); 
		((CPropertySecurity*)pSecurity)->m_pInstance = pOldInstance; 
		SAFEADDREF(((CPropertySecurity*)pSecurity)->m_pInstance); 

		SAFERELEASE(pOldInstance);

		SAFERELEASE(pSecurity);
	}

	SAFERELEASE(pMethod);
	SAFERELEASE(pAttached);

	TerminateDISPPARAMS(VARIANT_TRUE, &dispparamsCopy);

	// ReportError handled by caller
	return hr; // ReportThingError(hr);
}

STDMETHODIMP CThingObject::FireEventExt(BSTR bstrEventName, DISPPARAMS* pdispparams, long lFlags)
{
	HRESULT hr = S_OK;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	DISPPARAMS dispparamsCopy;

	InitializeDISPPARAMS(&dispparamsCopy, 0);

	ASSERT(bstrEventName);
	if (bstrEventName == NULL)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::FireEventExt: incoming argument (BSTR) is NULL\n");
		return E_INVALIDARG;
	}

	if (m_bStub == VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::FireEventExt: stub object, shouldn't be accessing\n");
		hr = VWOBJECT_E_STUBOBJECT;
		goto ERROR_ENCOUNTERED;
	}

	if (pdispparams != NULL)
	{
		// copy incoming DISPPARAMS to temporary DISPPARAMS,
		// then validate property types

		InitializeDISPPARAMS(&dispparamsCopy, pdispparams->cArgs);

		for (unsigned int i = 0; i < pdispparams->cArgs; i++)
		{
			// copy to new DISPPARAMS, freeing source
			::VariantCopyInd(&dispparamsCopy.rgvarg[i], &pdispparams->rgvarg[i]);
		}

		// check for valid property types
		hr = m_pWorld->ValidateDISPPARAMS(&dispparamsCopy);
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::FireEventExt: ValidateDISPPARAMS failed\n");
			goto ERROR_ENCOUNTERED;
		}
	}

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::FireEventExt: event name (%s)\n", 
		CString(bstrEventName));
#endif

	m_pWorld->get_ShouldExecuteSync(&bShouldExecute);

	// using this call for the proper semantics, not that FireEvent is a sync command
	m_pWorld->get_ShouldRemoteSync(&bShouldRemote);

	if (bShouldExecute == VARIANT_TRUE)
	{
		hr = FireEventExtHelper(bstrEventName, &dispparamsCopy, lFlags);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;
		HRESULT recvhr = S_OK;

		if (pdispparams)
		{
			// only create arglist if needed
			if (m_pArgList == NULL)
			{
				hr = CreatePropertyList(m_pWorld, &m_pArgList);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;
			}

			hr = ConvertDISPPARAMSToPropertyList(&dispparamsCopy, m_pArgList);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 3);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[2].vt = VT_BSTR;
		dispparams.rgvarg[2].bstrVal = bstrEventName;

		// NOTE: if NULL dispparams passed in, we need to make sure
		// it's treated as such on other side - not as 0 parameters
		dispparams.rgvarg[1].vt = VT_DISPATCH;
		dispparams.rgvarg[1].pdispVal = (pdispparams) ? m_pArgList : NULL;

		dispparams.rgvarg[0].vt = VT_I4;
		dispparams.rgvarg[0].lVal = lFlags;

		hr = SendThingCommand(PSBIT_NONE, DISPID_THING_FIREEVENTINT, DISPATCH_METHOD, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		if (m_pArgList)
			m_pArgList->RemoveAll();
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	TerminateDISPPARAMS(VARIANT_TRUE, &dispparamsCopy);

	return hr; // ReportThingError(hr);
}

// REVIEW: should we really be overriding security for invoked methods (fromserver flag)?

HRESULT CThingObject::FireEventExtHelper(BSTR bstrEventName, DISPPARAMS* pdispparams, long lFlags)
{
	HRESULT hr = S_OK;
	IThing* pContainer = NULL;
	IPropertyList* pContents = NULL;
	CWorldObject* pWorld = (CWorldObject*)m_pWorld;
	VARIANT_BOOL bAbortEvent = VARIANT_FALSE;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::FireEventExtHelper: %s\n", CString(bstrEventName));
#endif

#ifdef _DEBUG
	{
		CComBSTR bstrThis("unknown");
		CComBSTR bstrCaller("unknown");
		CComBSTR bstrSuper("unknown");
		IThing* pCaller = NULL;
		IThing* pSuper = NULL;

		/* hr = */ get_Name(&bstrThis.m_str);

		m_pWorld->get_Caller(&pCaller);
		m_pWorld->get_Super(&pSuper);

		if (pCaller)
			/* hr = */ pCaller->get_Name(&bstrCaller.m_str);

		if (pSuper)
			/* hr = */ pSuper->get_Name(&bstrSuper.m_str);

		VWTRACE(m_pWorld, "VWOBJECT", METHOD_TRACE, "CThingObject::FireEventExtHelper: event (%s), this (%s), caller (%s), super (%s)\n", 
			CString(bstrEventName), CString(bstrThis), CString(bstrCaller), CString(bstrSuper));

		SAFERELEASE(pCaller);
		SAFERELEASE(pSuper);
	}
#endif

//	m_pWorld->get_IsEventAborted(&bAbortEvent);

	bAbortEvent = pWorld->m_bAbortEvent;
	ASSERT(bAbortEvent == VARIANT_FALSE);

	hr = get_Container(&pContainer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = get_Contents(&pContents);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	{
		// for all things in contents, fire OnContainer
		CComBSTR bstrParentEvent = m_bstrParentEvent;
		bstrParentEvent += bstrEventName;

		if (pContents)
		{
			IThing* pthing = NULL;
			long propcnt = 0;
			int i;

			hr = pContents->get_Count(&propcnt);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED_2;

			for (i = 0; i < propcnt; i++)
			{
				if (SUCCEEDED(pContents->get_Thing(i, &pthing)) && pthing)
				{
					CSetUID SetUID(m_pWorld, pthing, pthing /* , VARIANT_TRUE */);

					hr = pthing->InvokeMethodExt(bstrParentEvent, pdispparams, NULL);

					SAFERELEASE(pthing);

//					m_pWorld->get_IsEventAborted(&bAbortEvent);
					bAbortEvent = pWorld->m_bAbortEvent;

					if (bAbortEvent == VARIANT_TRUE)
					{
						hr = VWOBJECT_E_EVENTABORTED;
						goto ERROR_ENCOUNTERED_2;
					}

//					if (FAILED(hr) && hr != VWOBJECT_E_METHODNOTEXIST)
//						goto ERROR_ENCOUNTERED_2;

					// negate METHODNOTEXIST error
					hr = S_OK;
				}
			}

ERROR_ENCOUNTERED_2:
			SAFERELEASE(pContents);
		}

		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	if (pContainer)
	{
		CSetUID SetUID(m_pWorld, pContainer, pContainer /* , VARIANT_TRUE */);

		// for parent, fire OnContent
		CComBSTR bstrChildEvent = m_bstrChildEvent;
		bstrChildEvent += bstrEventName;

		hr = pContainer->InvokeMethodExt(bstrChildEvent, pdispparams, NULL);

//		m_pWorld->get_IsEventAborted(&bAbortEvent);
		bAbortEvent = pWorld->m_bAbortEvent;

		if (bAbortEvent == VARIANT_TRUE)
		{
			hr = VWOBJECT_E_EVENTABORTED;
			goto ERROR_ENCOUNTERED;
		}

//		if (FAILED(hr) && hr != VWOBJECT_E_METHODNOTEXIST)
//			goto ERROR_ENCOUNTERED;

		// negate METHODNOTEXIST error
		hr = S_OK;
	}

	{
		CSetUID SetUID(m_pWorld, this, this /* , VARIANT_TRUE */ );

		// fire event to self
		CComBSTR bstrSelfEvent = m_bstrSelfEvent;
		bstrSelfEvent += bstrEventName;

		hr = InvokeMethodExt(bstrSelfEvent, pdispparams, NULL);

//		m_pWorld->get_IsEventAborted(&bAbortEvent);
		bAbortEvent = pWorld->m_bAbortEvent;

		if (bAbortEvent == VARIANT_TRUE)
		{
			hr = VWOBJECT_E_EVENTABORTED;
			goto ERROR_ENCOUNTERED;
		}

//		if (FAILED(hr) && hr != VWOBJECT_E_METHODNOTEXIST)
//			goto ERROR_ENCOUNTERED;

		// negate METHODNOTEXIST error
		hr = S_OK;
	}

	if (pContainer)
	{
		// for all things in parent's contents, fire OnPeer
		CComBSTR bstrPeerEvent = m_bstrPeerEvent;
		bstrPeerEvent += bstrEventName;

		IPropertyList* pContents = NULL;
		hr = pContainer->get_Contents(&pContents);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (pContents)
		{
			IThing* pthing = NULL;
			long propcnt = 0;
			int i;

			hr = pContents->get_Count(&propcnt);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED_1;

			for (i = 0; i < propcnt; i++)
			{
				if (SUCCEEDED(pContents->get_Thing(i, &pthing)) && pthing)
				{
					CSetUID SetUID(m_pWorld, pthing, pthing /* , VARIANT_TRUE */);

					// don't send OnPeer to originating object
					if (pthing != this)
						hr = pthing->InvokeMethodExt(bstrPeerEvent, pdispparams, NULL);

					SAFERELEASE(pthing);
					
					/*	// peers should not be allowed to abort events
//					m_pWorld->get_IsEventAborted(&bAbortEvent);
					bAbortEvent = pWorld->m_bAbortEvent;

					if (bAbortEvent == VARIANT_TRUE)
					{
						hr = VWOBJECT_E_EVENTABORTED;
						goto ERROR_ENCOUNTERED_1;
					}
					*/
//					if (FAILED(hr) && hr != VWOBJECT_E_METHODNOTEXIST)
//						goto ERROR_ENCOUNTERED_1;

					// negate METHODNOTEXIST error
					hr = S_OK;
				}
			}

ERROR_ENCOUNTERED_1:
			SAFERELEASE(pContents);
		}

		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
//	m_pWorld->put_IsEventAborted(VARIANT_FALSE);
	pWorld->m_bAbortEvent = VARIANT_FALSE;

	SAFERELEASE(pContainer);

	return hr;
}

STDMETHODIMP CThingObject::Marshall(DWORD dwOptions, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = CThingObjectBase::Marshall(dwOptions, pbuffer)))
		return hr;

	// force options to be BYREF, BYID
	DWORD dwNewOptions = MARSHALLDEST(MARSHALLREF(dwOptions, MARSHALL_BYREF), MARSHALL_BYID);

	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CThingObject::Marshall: marshalling exemplar (by ref)\n");

	// need to marshall as object property to take care of NULL ptr
	hr = m_pWorld->MarshallObjectProperty(m_pExemplar, clsidThing, dwNewOptions, pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CThingObject::Marshall: marshalling owner (by ref)\n");

	// need to marshall as object property to take care of NULL ptr
	hr = m_pWorld->MarshallObjectProperty(m_pOwner, clsidThing, dwNewOptions, pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CThingObject::Marshall: marshalling object properties (by ref)\n");

	ASSERT(m_PropSecurity != NULL);
	hr = m_PropSecurity->Marshall(dwNewOptions, pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CThingObject::Marshall: marshalling object methods (by ref)\n");

	ASSERT(m_MethodSecurity != NULL);
	hr = m_MethodSecurity->Marshall(dwNewOptions, pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::UnMarshall(IUnMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;
	CComPropertyMapPtr MapPtr;
	CComVariant var;
	VARIANT_BOOL bDone;
	BSTR bstrName = NULL;
	IDispatch* pdisp = NULL;
	IMethod* pmethod = NULL;

	// clean up data
	SAFERELEASE(m_pExemplar);
	SAFERELEASE(m_pOwner);

	SAFERELEASE(m_pMethods);

	// release methods first, since they might reference modules
	// included in the property map (only for the Global object)
	if (m_MethodSecurity != NULL)
		m_MethodSecurity->Cleanup();

	SAFERELEASE(m_pProperties);

	// propsecurity cleanup should happen after freeing method map
	// and property map pointers
	if (m_PropSecurity != NULL)
		m_PropSecurity->Cleanup();

	if (FAILED(hr = CThingObjectBase::UnMarshall(pbuffer)))
		return hr;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CThingObject::UnMarshall: unmarshalling exemplar\n");

	// need to marshall as object property to take care of NULL ptr
	hr = m_pWorld->UnMarshallObjectProperty(pbuffer, (IObjectProperty**)&m_pExemplar);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CThingObject::UnMarshall: unmarshalling owner\n");

	// need to marshall as object property to take care of NULL ptr
	hr = m_pWorld->UnMarshallObjectProperty(pbuffer, (IObjectProperty**)&m_pOwner);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CThingObject::UnMarshall: unmarshalling object properties\n");

	// UnMarshall the security
	hr = m_PropSecurity->UnMarshall(pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// Get its property
	MapPtr = ((CPropertySecurity*)(IPropertySecurity*)m_PropSecurity)->m_var;

	// Make sure it is a property map!
	ASSERT(MapPtr != NULL);

	m_pProperties = MapPtr;
	SAFEADDREF(m_pProperties);

	var.Clear();
	MapPtr.Release();

	// restore property names

	// iterate through the properties map
	hr = m_pProperties->FirstItem(&bstrName, &var, &bDone);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	while (bDone != VARIANT_TRUE)
	{
		ASSERT(bstrName != NULL);

#ifdef _DEBUG
//		VWTRACE(m_pWorld, "VWOBJECT", VWT_NONE, "CThingObject::UnMarshall: unmarshalling property (%s)\n", CString(bstrName));
#endif

		// set back-ptr and prop name in object properties
		hr = SetPropertyName(bstrName, var);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		// free stuff
		SAFEFREESTRING(bstrName);
		var.Clear();

		// on to the next one
		hr = m_pProperties->NextItem(&bstrName, &var, &bDone);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CThingObject::UnMarshall: unmarshalling object methods\n");

	hr = m_MethodSecurity->UnMarshall(pbuffer);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// Get its property
	MapPtr = ((CPropertySecurity*)(IPropertySecurity*)m_MethodSecurity)->m_var;

	// Make sure it is a property map!
	ASSERT(MapPtr != NULL);

	m_pMethods = MapPtr;
	SAFEADDREF(m_pMethods);

	var.Clear();
	MapPtr.Release();

	// restore method names

	// iterate through the method map
	hr = m_pMethods->FirstItem(&bstrName, &var, &bDone);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	while (bDone != VARIANT_TRUE)
	{
		ASSERT(bstrName != NULL);

		if (var.vt != VT_DISPATCH || var.pdispVal == NULL)
			goto NEXT_METHOD;

		pdisp = var.pdispVal;
		SAFEADDREF(pdisp);

		// check if pdisp is an IObjectProperty
		if (SUCCEEDED(pdisp->QueryInterface(IID_IMethod, (LPVOID*)&pmethod)) && pmethod)
		{
#ifdef _DEBUG
//			VWTRACE(m_pWorld, "VWOBJECT", VWT_NONE, "CThingObject::UnMarshall: unmarshalling method (%s)\n", CString(bstrName));
#endif

			// set back-ptr and prop name in object properties
			hr = SetMethodName(bstrName, pmethod);

			SAFERELEASE(pmethod);
		}

NEXT_METHOD:
		// free stuff
		SAFERELEASE(pdisp);
		SAFEFREESTRING(bstrName);
		var.Clear();

		// on to the next one
		hr = m_pMethods->NextItem(&bstrName, &var, &bDone);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	SAFEFREESTRING(bstrName);

	return ReportThingError(hr);
}

STDMETHODIMP CThingObject::MarshallToScript(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut)
{
	HRESULT hr = S_OK;
	CComVariant var;
	VARIANT_BOOL bDone;
	VARIANT_BOOL bAdd;
	VARIANT_BOOL bPropAsVal;
	VARIANT_BOOL bIsValid;
	CComBSTR bstrPropOut;
	CString strOut;
	CString strOne;
	CString strVal;
	IMethod *pMethod = NULL;
	IPropertySecurity *pSec = NULL;
	CString strPropExtra;
	CString strSec;
	long lBitsSec;
	CComBSTR bstrName;
	static CComBSTR bstrObjProp("ObjProp");
	static CComBSTR bstrMethod("Method");

	VWTRACE(m_pWorld, "VWOBJECT", VWT_VERBOSE, "CThingObject::MarshallToScript\n");

	if (pbstrOut == NULL)
		return E_INVALIDARG;

	// initialize
	*pbstrOut = NULL;

	// if byref, then just write out ID and done
	if (EXTRACTREF(dwOptions) == MARSHALL_BYREF)
	{
		strVal.Format("ObjArray(%d)", m_lObjectID);
		*pbstrOut = strVal.AllocSysString();
		*pbAsVal = VARIANT_TRUE;
		return S_OK;
	}

	if (bstrContext==NULL)
		return E_INVALIDARG;

	// iterate through the properies map
	hr = m_pProperties->FirstItem(&bstrName.m_str, &var, &bDone);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	while (bDone != VARIANT_TRUE)
	{
		ASSERT((BSTR)bstrName != NULL);

		strOne = "";

		// get the security info to see if we need to override the defaults
		hr = m_pProperties->get_PropertySecurityExt(bstrName, HashKey<BSTR>(bstrName), &pSec);
		if (FAILED(hr))
		{
			strOne.Format("\t' Could not get security bits for property \"%s\"\n", (LPCTSTR) CString(bstrName));
			goto NEXT_PROPERTY;
		}

		if (pSec == NULL)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::MarshallToScript: property (%s), property security is invalid\n", CString(bstrName));
#endif
			hr = VWOBJECT_E_INVALIDSECURITY;
			goto ERROR_ENCOUNTERED;
		}

		lBitsSec = ((CPropertySecurity*)pSec)->m_ulBits;

		// see if one that should be skipped automatically
		if (lBitsSec & PSBIT_INTERNALLYSET)
			goto NEXT_PROPERTY;

		// set the add/put flag
		if (lBitsSec & PSBIT_INTERNALLYADDED)
			bAdd = VARIANT_FALSE;
		else
		{
			bAdd = VARIANT_TRUE;

			// see whether the prop should be added (new) or put (exists in exemplar)
			if (m_pExemplar != NULL)
			{
				hr = m_pExemplar->IsValidProperty(bstrName, &bIsValid);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;

				// it's in the exemplar, so don't add it
				if (bIsValid)
					bAdd = VARIANT_FALSE;
			}
		}

		hr = m_pWorld->MarshallPropertyToScript(var, MARSHALL_BYREF, bstrObjProp, &bPropAsVal, &bstrPropOut.m_str);
		if (FAILED(hr) || (BSTR)bstrPropOut == NULL)
		{
			strOne.Format("\t' Failed to marshall property \"%s\"\n", (LPCTSTR) CString(bstrName));
			goto NEXT_PROPERTY;
		}

		// handle value vs. context case
		if (bPropAsVal == VARIANT_TRUE)
		{
			strVal = bstrPropOut;
		}
		else
		{
			// objprop has marshalled out script to set context, so current value
			// is context itself
			strOut += CString(bstrPropOut);
			strVal = "ObjProp";
		}

		// format assignment
		if (bAdd == VARIANT_TRUE)
		{
			BOOL bDefaultBits = TRUE;
			BOOL bDefaultPerm = TRUE;

			long lPerm = ((CPropertySecurity*)pSec)->m_ulSecurityFlags;
			unsigned short usType = ((CPropertySecurity*)pSec)->m_vtType;

			strPropExtra = "";

			if (lBitsSec != (PSBIT_SCRIPTDEFAULTPROPERTY | PSBIT_ENABLED))
			{
				strSec.Format(", &H%X", (lBitsSec & ~PSBIT_ENABLED));
				strPropExtra = strSec;

				bDefaultBits = FALSE;
			}

			// review: is this still the right way to do this?
			if (lPerm != PS_SCRIPTDEFAULTPROPERTY)
			{
				if (bDefaultBits == TRUE)
					strPropExtra += ", ";

				strSec.Format(", &H%X", lPerm);
				strPropExtra += strSec;

				bDefaultPerm = FALSE;
			}

			if (usType != VT_VARIANT)
			{
				if (bDefaultBits == TRUE)
					strPropExtra += ", ";
				if (bDefaultPerm == TRUE)
					strPropExtra += ", ";

				strSec.Format(", %d", usType);
				strPropExtra += strSec;
			}

			strOne.Format(	"\t%s.AddProperty \"%s\", %s%s\n", 
							(LPCTSTR) CString(bstrContext),
							(LPCTSTR) CString(bstrName),
							(LPCTSTR) strVal,
							(LPCTSTR) strPropExtra);
		}
		else
		{
			// just do a "set"
			strOne.Format(	"\t%s.Property(\"%s\") = %s\n", 
							(LPCTSTR) CString(bstrContext),
							(LPCTSTR) CString(bstrName),
							(LPCTSTR) strVal);
		}

NEXT_PROPERTY:
		// concatenate
		if (!strOne.IsEmpty())
			strOut += strOne;

		// free stuff
		SAFERELEASE(pSec);
		var.Clear();

		// on to the next one
		hr = m_pProperties->NextItem(&bstrName.m_str, &var, &bDone);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	// iterate through the method map
	hr = m_pMethods->FirstItem(&bstrName.m_str, &var, &bDone);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	while (bDone != VARIANT_TRUE)
	{
		// verify we've got a method
		ASSERT(var.vt==VT_DISPATCH);
		hr = var.pdispVal->QueryInterface(IID_IMethod, (void **)&pMethod);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		strOne = "";

		// get the security info to see if we need to override the defaults
		hr = m_pMethods->get_PropertySecurityExt(bstrName.m_str, HashKey<BSTR>(bstrName), &pSec);
		if (FAILED(hr))
		{
			strOne.Format("\t' Could not get security bits for method \"%s\"\n", (LPCTSTR) CString(bstrName));
			goto NEXT_METHOD;
		}

		if (pSec == NULL)
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::MarshallToScript: method (%s), method security is invalid\n", CString(bstrName));
#endif
			hr = VWOBJECT_E_INVALIDSECURITY;
			goto ERROR_ENCOUNTERED;
		}

		// get the security bits
		lBitsSec = ((CPropertySecurity*)pSec)->m_ulBits;

		// see if one that should be skipped automatically
		if (lBitsSec & PSBIT_INTERNALLYSET)
			goto NEXT_METHOD;

		// set the add/put flag
		if (lBitsSec & PSBIT_INTERNALLYADDED)
			bAdd = VARIANT_FALSE;
		else
		{
			bAdd = VARIANT_TRUE;

			// see whether the prop should be added (new) or put (exists in exemplar)
			if (m_pExemplar != NULL)
			{
				hr = m_pExemplar->IsValidMethod(bstrName, &bIsValid);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;

				// it's in the exemplar, so don't add it
				if (bIsValid)
					bAdd = VARIANT_FALSE;
			}
		}

		// tell method to marshall itself out
		hr = pMethod->MarshallToScript(MARSHALL_BYREF, bstrMethod, &bPropAsVal, &bstrPropOut.m_str);
		if (FAILED(hr) || (BSTR)bstrPropOut == NULL)
		{
			strOne.Format("\t' Error marshalling method \"%s\"\n", (LPCTSTR) CString(bstrName));
			goto NEXT_METHOD;
		}

		ASSERT(bPropAsVal==VARIANT_FALSE);

		// Method has marshalled out script to set context, so current value
		// is context itself
		strOut += CString(bstrPropOut);

		if (bAdd == VARIANT_TRUE)
		{
			BOOL bDefaultBits = TRUE;

			long lPerm = ((CPropertySecurity*)pSec)->m_ulSecurityFlags;

			strPropExtra = "";

			if (lBitsSec != (PSBIT_SCRIPTDEFAULTMETHOD | PSBIT_ENABLED))
			{
				strSec.Format(", &H%X", (lBitsSec & ~PSBIT_ENABLED));
				strPropExtra = strSec;

				bDefaultBits = FALSE;
			}

			// add override information if not the default
			if (lPerm != PS_SCRIPTDEFAULTMETHOD)
			{
				if (bDefaultBits == TRUE)
					strPropExtra += ", ";

				strSec.Format(", &H%X", lPerm);
				strPropExtra += strSec;
			}

			strOne.Format(	"\t%s.AddMethod \"%s\", Method%s\n", 
							(LPCTSTR) CString(bstrContext),
							(LPCTSTR) CString(bstrName),
							(LPCTSTR) CString(strPropExtra));
		}
		else
			strOne.Format(	"\t%s.Method(\"%s\") = Method\n", 
							(LPCTSTR) CString(bstrContext),
							(LPCTSTR) CString(bstrName));

NEXT_METHOD:
		// concatenate
		if (!strOne.IsEmpty())
			strOut += strOne;

		// free stuff
		SAFERELEASE(pSec);
		SAFERELEASE(pMethod);
		var.Clear();

		// on to the next one
		hr = m_pMethods->NextItem(&bstrName.m_str, &var, &bDone);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
	}

	// allocate
	*pbstrOut = strOut.AllocSysString();

ERROR_ENCOUNTERED:
	SAFERELEASE(pMethod);
	SAFERELEASE(pSec);

	return hr;
}

HRESULT CThingObject::SetPropertyName(BSTR bstrPropertyName, VARIANT var)
{
	HRESULT hr = S_OK;
	IDispatch* pdisp = NULL;
	IObjectProperty* pprop = NULL;
	IThing* pParent = NULL;
	IThing* pthing = NULL;

	if (var.vt != VT_DISPATCH || var.pdispVal == NULL)
		return S_OK;

	pdisp = var.pdispVal;
	SAFEADDREF(pdisp);

	// check if pdisp is an IObjectProperty
	if (SUCCEEDED(pdisp->QueryInterface(IID_IObjectProperty, (LPVOID*)&pprop)) && pprop)
	{
		// don't set back-ptr on IThing
		if (SUCCEEDED(pprop->QueryInterface(IID_IThing, (LPVOID*)&pthing)) && pthing)
		{
			// no-op 

			SAFERELEASE(pthing);
		}
		else
		{
			hr = pprop->get_Parent(&pParent);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED_1;

			if (pParent == NULL)
			{
				hr = pprop->put_Parent(this);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED_1;

				hr = pprop->put_PropertyName(bstrPropertyName);
			}
			else
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::SetPropertyName: name (%s), failed because parent already set\n", CString(bstrPropertyName));

				hr = VWOBJECT_E_INVALIDPARENT;
			}
		}

ERROR_ENCOUNTERED_1:
		SAFERELEASE(pprop);
		SAFERELEASE(pParent);
	}

	SAFERELEASE(pdisp);	

	return hr;
}

HRESULT CThingObject::ClearPropertyName(VARIANT var)
{
	HRESULT hr = S_OK;
	IDispatch* pdisp = NULL;
	IObjectProperty* pprop = NULL;
	IThing* pthing = NULL;

	if (var.vt != VT_DISPATCH || var.pdispVal == NULL)
		return S_OK;

	pdisp = var.pdispVal;
	SAFEADDREF(pdisp);

	// check if pdisp is an IObjectProperty
	if (SUCCEEDED(pdisp->QueryInterface(IID_IObjectProperty, (LPVOID*)&pprop)) && pprop)
	{
		// don't set back-ptr on IThing
		if (SUCCEEDED(pprop->QueryInterface(IID_IThing, (LPVOID*)&pthing)) && pthing)
		{
			// no-op 

			SAFERELEASE(pthing);
		}
		else
		{
			hr = pprop->put_Parent(NULL);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED_1;

			hr = pprop->put_PropertyName(NULL);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED_1;
		}

ERROR_ENCOUNTERED_1:
		SAFERELEASE(pprop);
	}

	SAFERELEASE(pdisp);

	return hr;
}

HRESULT CThingObject::SetMethodName(BSTR bstrMethodName, IMethod* pmethod)
{
	HRESULT hr = S_OK;
	IThing* pParent = NULL;

	if (pmethod)
	{
		hr = pmethod->get_Parent(&pParent);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		if (pParent == NULL)
		{
			hr = pmethod->put_Parent(this);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			hr = pmethod->put_PropertyName(bstrMethodName);
			if (FAILED(hr)) 
				goto ERROR_ENCOUNTERED;
		}
		else
		{
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::SetMethodName: name (%s), failed because parent already set\n", CString(bstrMethodName));

			hr = VWOBJECT_E_INVALIDPARENT;
		}
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pParent);

	return hr;
}

HRESULT CThingObject::ClearMethodName(IMethod* pmethod)
{
	HRESULT hr = S_OK;

	if (pmethod == NULL)
		return S_OK;

	hr = pmethod->put_Parent(NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pmethod->put_PropertyName(NULL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	return hr;
}

HRESULT CThingObject::NotifyPropertyChangedExt(BSTR bstrPropName, long psbits, IPropertySecurity* pSecurity, long lHint, VARIANT varHintData)
{
	HRESULT hr = S_OK;
//	VARIANT_BOOL bFreezeEvents = VARIANT_TRUE;
	CWorldObject* pWorld = (CWorldObject*)m_pWorld;

//	m_pWorld->get_FreezeEvents(&bFreezeEvents);

#ifdef SUPPORT_NOTIFYPROPERTYCHANGED
	if (pWorld->m_bFreezeEvents == VARIANT_FALSE)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::NotifyPropertyChangedExt: name %s, hint %d\n", CString(bstrPropName), lHint);
#endif

		hr = m_pWorld->NotifyPropertyChanged(this, bstrPropName, lHint, varHintData);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

#ifdef SUPPORT_INVOKEONPROPERTYCHANGED
		// REVIEW: ignore in-world notification errors
		/* hr = */ InvokeOnPropertyChanged(bstrPropName, psbits, pSecurity, lHint, varHintData);
#endif
	}
#endif

#ifdef SUPPORT_DATESTAMPS
	SetModifiedDate();
#endif

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CThingObject::NotifyObjectPropertyChanged(IObjectProperty* pprop, long lHint, VARIANT varHintData)
{
	HRESULT hr = S_OK;
	CComBSTR bstrPropName;
//	VARIANT_BOOL bFreezeEvents = VARIANT_TRUE;
	IPropertySecurity* pSecurity = NULL;
	CWorldObject* pWorld = (CWorldObject*)m_pWorld;

//	m_pWorld->get_FreezeEvents(&bFreezeEvents);

#ifdef SUPPORT_NOTIFYPROPERTYCHANGED
	if (pWorld->m_bFreezeEvents == VARIANT_FALSE)
	{
		hr = pprop->get_PropertyName(&bstrPropName.m_str);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::NotifyObjectPropertyChanged: name %s, hint %d\n", CString(bstrPropName), lHint);
#endif

		hr = m_pWorld->NotifyPropertyChanged(this, bstrPropName, lHint, varHintData);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

#ifdef SUPPORT_INVOKEONPROPERTYCHANGED
		if (SUCCEEDED(pprop->get_Security(&pSecurity)) && pSecurity != NULL)
		{
			// REVIEW: ignore in-world notification errors
			/* hr = */ InvokeOnPropertyChanged(bstrPropName, ((CPropertySecurity*)pSecurity)->m_ulBits, pSecurity, lHint, varHintData);

			SAFERELEASE(pSecurity);
		}
#endif
	}
#endif

#ifdef SUPPORT_DATESTAMPS
	SetModifiedDate();
#endif

ERROR_ENCOUNTERED:
	return hr;
}

HRESULT CThingObject::NotifyMethodChangedExt(BSTR bstrMethodName, long lHint, VARIANT varHintData)
{
	HRESULT hr = S_OK;
//	VARIANT_BOOL bFreezeEvents = VARIANT_TRUE;
	CWorldObject* pWorld = (CWorldObject*)m_pWorld;

//	m_pWorld->get_FreezeEvents(&bFreezeEvents);

#ifdef SUPPORT_NOTIFYMETHODCHANGED
	if (pWorld->m_bFreezeEvents == VARIANT_FALSE)
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CThingObject::NotifyMethodChanged: name %s, hint %d\n", CString(bstrMethodName), lHint);
#endif

		hr = m_pWorld->NotifyMethodChanged(this, bstrMethodName, lHint, varHintData);
	}
#endif

#ifdef SUPPORT_DATESTAMPS
	SetModifiedDate();
#endif

	return hr;
}

STDMETHODIMP CThingObject::NotifyMethodChanged(IMethod* pmethod, long lHint, VARIANT varHintData)
{
	HRESULT hr = S_OK;
	CComBSTR bstrMethodName;
//	VARIANT_BOOL bFreezeEvents = VARIANT_TRUE;
	CWorldObject* pWorld = (CWorldObject*)m_pWorld;

//	m_pWorld->get_FreezeEvents(&bFreezeEvents);

#ifdef SUPPORT_NOTIFYMETHODCHANGED
	if (pWorld->m_bFreezeEvents == VARIANT_FALSE)
	{
		hr = pmethod->get_PropertyName(&bstrMethodName.m_str);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = NotifyMethodChangedExt(bstrMethodName, lHint, varHintData);
	}
#endif

#ifdef SUPPORT_DATESTAMPS
	SetModifiedDate();
#endif

ERROR_ENCOUNTERED:
	return hr;
}

HRESULT CThingObject::InvokeOnPropertyChanged(BSTR bstrPropertyName, long psbits, IPropertySecurity* pSecurity, long lHint, VARIANT varHintData)
{
#ifndef DISABLE_PROPERTYBIT_CACHING
	if (!(psbits & PSBIT_NOTIFYONCHANGE))
		return S_OK;
#ifdef TRACE_PROPERTYBIT_CACHING
	else
		TRACE("CThingObject::InvokeOnPropertyChanged: property (%s), NOTIFYONCHANGE bit is set\n", CString(bstrPropertyName));
#endif
#endif

	CWorldObject* pWorld = (CWorldObject*)m_pWorld;

	// if property isn't local
	if (!(psbits & PSBIT_NOTREMOTED))
	{
		// don't invoke OnXXXChanged method on client when connected
		if (pWorld->m_bServerSide == VARIANT_FALSE && pWorld->m_bConnected == VARIANT_TRUE)
			return S_OK;
	}

#ifdef _DEBUG
	{
		CComBSTR bstrName;

		/* hr = */ get_Name(&bstrName.m_str);

		VWTRACE(m_pWorld, "VWOBJECT", NOTIFY_TRACE, "CThingObject::InvokeOnPropertyChanged: this (%s), property (%s), hint %d, local %s, volatile %s\n", 
			CString(bstrName), CString(bstrPropertyName), lHint, 
			psbits & PSBIT_LOCAL ? "yes" : "no",
			psbits & PSBIT_VOLATILE ? "yes" : "no");
	}
#endif

	HRESULT hr = S_OK;
	VARIANT_BOOL bValidMethod = VARIANT_FALSE;
	DISPPARAMS dispparams;

#ifdef SUPPORT_CACHED_EVENTHANDLERS
	BSTR bstrOnChanged = (BSTR)((CPropertySecurity*)pSecurity)->m_bstrOnChanged;
#else
	CComBSTR bstrOnChanged = m_bstrOn;
	bstrOnChanged += bstrPropertyName;
	bstrOnChanged += m_bstrChanged;
#endif

	// check if method exists first, before assembling DISPPARAMS, etc.
	hr = IsValidMethod(bstrOnChanged, &bValidMethod);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bValidMethod == VARIANT_FALSE)
		goto ERROR_ENCOUNTERED;

	// package up args into DISPPARAMS struct
	hr = InitializeDISPPARAMS(&dispparams, 2);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	dispparams.rgvarg[1].vt = VT_I4;
	dispparams.rgvarg[1].lVal = lHint;

	memcpy(&dispparams.rgvarg[0], &varHintData, sizeof(VARIANT));

	hr = InvokeMethodExt(bstrOnChanged, &dispparams, NULL);

	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

	if (SUCCEEDED(hr))
	{
		// no-op
	}
	else if (hr == VWOBJECT_E_METHODNOTEXIST || hr == VWOBJECT_E_INVALIDMETHODMAP)
	{
		// absorb METHODNOTEXIST error
		hr = S_OK;
	}
#ifdef _DEBUG
	else
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::InvokeOnPropertyChanged: property (%s), method invocation failed, hr %x\n", 
			CString(bstrPropertyName), hr);
	}
#endif

ERROR_ENCOUNTERED:
	return hr;
}

HRESULT CThingObject::InvokeOnPropertyAccess(BSTR bstrPropertyName, long psbits, long permissions, IPropertySecurity* pSecurity)
{		
#ifndef DISABLE_PROPERTYBIT_CACHING
	if (!(psbits & PSBIT_NOTIFYONACCESS))
		return S_OK;
#ifdef TRACE_PROPERTYBIT_CACHING
	else
		TRACE("CThingObject::InvokeOnPropertyAccess: property (%s), NOTIFYONACCESS bit is set\n", CString(bstrPropertyName));
#endif
#endif

#ifdef SUPPORT_SECURITY_OVERRIDE
	{
		VARIANT_BOOL bSecurityEnabled = VARIANT_TRUE;

		/* hr = */ m_pWorld->get_EnableSecurity(&bSecurityEnabled);

		if (bSecurityEnabled == VARIANT_FALSE)
			return S_OK;
	}
#endif

	{
		CWorldObject* pWorld = (CWorldObject*)m_pWorld;

		// ignore if SYSTEMOVERRIDE is set in world security context
		if (pWorld->m_pSecurityContext)
		{
			if (((CPropertySecurity*)pWorld->m_pSecurityContext)->m_ulBits & PSBIT_SYSTEMOVERRIDE)
				return S_OK;
		}
	}

	HRESULT hr = S_OK;
	IMethod* pmethod = NULL;

#ifdef SUPPORT_CACHED_EVENTHANDLERS
	BSTR bstrOnAccess = (BSTR)((CPropertySecurity*)pSecurity)->m_bstrOnAccess;
#else
	CComBSTR bstrOnAccess = m_bstrOn;
	bstrOnAccess += bstrPropertyName;
	bstrOnAccess += m_bstrAccess;
#endif

	hr = get_MethodExt(bstrOnAccess, HashKey<BSTR>(bstrOnAccess), NULL, &pmethod);
	if (SUCCEEDED(hr))
	{
		long lFlags = ((CMethodObject *)pmethod)->m_lFlags;

		SAFERELEASE(pmethod);

		if ((lFlags & (METHOD_CLIENT | METHOD_SERVER)) != (METHOD_CLIENT | METHOD_SERVER))
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::InvokeOnPropertyAccess: property (%s), method not marked METHOD_CLIENT+METHOD_SERVER, ignoring\n", 
				CString(bstrPropertyName));
#endif
			return S_OK;
		}
	}
	else
	{
		// OnXXXAccess method not found
		return S_OK;
	}

	CComVariant var;
	DISPPARAMS dispparams;

	// package up args into DISPPARAMS struct
	hr = InitializeDISPPARAMS(&dispparams, 1);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	dispparams.rgvarg[0].vt = VT_I4;
	dispparams.rgvarg[0].lVal = permissions;

	hr = InvokeMethodExt(bstrOnAccess, &dispparams, &var);

	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

	if (SUCCEEDED(hr))
	{
		ASSERT(var.vt == VT_BOOL);

		// absorb other retval types
		if (var.vt == VT_BOOL)
		{
			if (var.boolVal == VARIANT_FALSE)
			{
#ifdef _DEBUG
				VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::InvokeOnPropertyAccess: property (%s), permission denied, method returned VARIANT_FALSE\n", 
					CString(bstrPropertyName));
#endif
				hr = VWOBJECT_E_PERMISSION_DENIED;
			}
		}
		else
		{
#ifdef _DEBUG
			VWTRACE(m_pWorld, "VWOBJECT", VWT_IMPORTANT, "CThingObject::InvokeOnPropertyAccess: property (%s), permission denied, method returned incorrect VARIANT retval (%d)\n", 
				CString(bstrPropertyName), var.vt);
#endif
			hr = VWOBJECT_E_PERMISSION_DENIED;
		}
	}
	else if (hr == VWOBJECT_E_METHODNOTEXIST || hr == VWOBJECT_E_INVALIDMETHODMAP)
	{
		// absorb METHODNOTEXIST error
		hr = S_OK;
	}
#ifdef _DEBUG
	else
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CThingObject::InvokeOnPropertyAccess: property (%s), method invocation failed, hr %x\n", 
			CString(bstrPropertyName), hr);
	}
#endif

ERROR_ENCOUNTERED:
	return hr;
}

// IDispatchImpl overrides

STDMETHODIMP CThingObject::GetIDsOfNames(	REFIID riid, 
											LPOLESTR* rgszNames, 
											UINT cNames,
											LCID lcid, 
											DISPID* rgdispid)
{
	HRESULT hr = S_OK;

#ifndef DISABLEDISPATCHHACK
	// first, try property and method maps (so added props/method name will be found before name in typeinfo)
	if (cNames == 1)
	{
		// peg
		LPOLESTR psz = rgszNames[0];

		// check if the provided name is a property or method.  If so, get
		// "doctored" dispid for it
		DISPID id;
		hr = IdFromName(psz, &id);
		if (SUCCEEDED(hr))
		{
			// cache it
			m_bstrCached = psz;
			m_idCached = id;

			rgdispid[0] = id;
			return S_OK;
		}
	}
#endif

	// if not found, try default thing
	hr = IDispatchImpl<IThing, &IID_IThing, &LIBID_VWSYSTEMLib>::GetIDsOfNames(	riid, 
																				rgszNames, 
																				cNames, 
																				lcid, 
																				rgdispid);

	return hr;
}

STDMETHODIMP CThingObject::Invoke(	DISPID dispidMember, 
									REFIID riid,
									LCID lcid, 
									WORD wFlags, 
									DISPPARAMS* pdispparams, 
									VARIANT* pvarResult,
									EXCEPINFO* pexcepinfo, 
									UINT* puArgErr)
{
#ifndef DISABLEDISPATCHHACK
	VARIANT_BOOL bProp;
	HRESULT hr = S_OK;

	// REVIEW: we're not setting the proper puArgErr value on E_TYPEMISMATCH and E_PARAMNOTFOUND errors

	// NOTE: need to special case some calls since get_Property/put_Property already call ReportThingError,
	// while InvokeMethodExt doesn't

	// if the dispid is a "doctored one", handle this through the specific method or property
	if (IsDoctoredId(dispidMember, &bProp))
	{
		CComBSTR bstr;

		// clear out outgoing data
		if (puArgErr != NULL)
			*puArgErr = 0;

		if (pexcepinfo != NULL)
			memset(pexcepinfo, 0, sizeof(EXCEPINFO));

		// get the name associated with the doctored id
		hr = NameFromId(dispidMember, &bstr.m_str);
		if (SUCCEEDED(hr))
		{
			// if it's a property, see whether we're setting or getting it
			if (bProp == VARIANT_TRUE)
			{
				// if property get, then return value
				if (wFlags & DISPATCH_PROPERTYGET)
				{
					hr = get_Property(bstr, pvarResult);
				}
				else if (wFlags & (DISPATCH_PROPERTYPUT | DISPATCH_PROPERTYPUTREF))
				{
					// should only have one argument (the value)
					if (pdispparams->cArgs != 1)
					{
						hr = ReportThingError(DISP_E_BADPARAMCOUNT);
					}
					else
					{
						// should have exactly one named arg
						if (pdispparams->cNamedArgs != 1 ||
							pdispparams->rgdispidNamedArgs[0] != DISPID_PROPERTYPUT)
						{
							hr = ReportThingError(DISP_E_PARAMNOTOPTIONAL);
						}
						else
						{
							// set the property
							hr = put_Property(bstr, pdispparams->rgvarg[0]);
						}
					}
				}
				else
				{
					// trying to do a method invoke on a property
					hr = ReportThingError(DISP_E_TYPEMISMATCH);
				}
			}
			else
			{
				// it's a method, should be an invoke
				if (wFlags & DISPATCH_METHOD) 
				{
					hr = InvokeMethodExt(bstr, pdispparams, pvarResult);

					// fill in ErrorInfo structure
					hr = ReportThingError(hr);
				}
				else
				{
					// trying to do property get/put on a method
					hr = ReportThingError(DISP_E_TYPEMISMATCH);
				}
			}
		}

		// must fill in EXCEPINFO structure ourselves
		if (FAILED(hr) && HRESULT_FACILITY(hr) == FACILITY_ITF)
		{
			if (pexcepinfo)
			{
				CComBSTR bstrSource("unknown"), bstrDescription("Error in server-side synchronous method");
				CComPtr<IErrorInfo> pErrorInfo;

				::GetErrorInfo(0L, &pErrorInfo);

				// we possibly could have no errorinfo from server-side/sync method,
				// if HRESULT isn't in the IThing range (the ReportThingError above will format
				// error strings in the proper range)
				if (pErrorInfo)
				{
					pErrorInfo->GetSource(&bstrSource.m_str);
					pErrorInfo->GetDescription(&bstrDescription.m_str);
				}

				pexcepinfo->wCode = 0;
				pexcepinfo->wReserved = 0;
				pexcepinfo->bstrSource = bstrSource.Copy();
				pexcepinfo->bstrDescription = bstrDescription.Copy();
				pexcepinfo->bstrHelpFile = NULL;
				pexcepinfo->dwHelpContext = 0L;
				pexcepinfo->pvReserved = NULL;
				pexcepinfo->pfnDeferredFillIn = NULL;
				pexcepinfo->scode = (SCODE)hr;
			}

			// must return this HRESULT for EXCEPINFO to be used
			hr = DISP_E_EXCEPTION;
		}

		return hr;
	}
	else
	{
#endif
		// call base class version of this
		return IDispatchImpl<IThing, &IID_IThing, &LIBID_VWSYSTEMLib>::Invoke(	dispidMember, 
																				riid, 
																				lcid,
																				wFlags, 
																				pdispparams, 
																				pvarResult, 
																				pexcepinfo, 
																				puArgErr);

#ifndef DISABLEDISPATCHHACK
	}
#endif
}

// helpers for doctored dispids
BOOL CThingObject::IsDoctoredId(DISPID id, VARIANT_BOOL* pbProp)
{
	ASSERT(pbProp);
	if (id >= idMinDoctored && id < idMaxDoctored)
	{
		// set prop/method flag
		*pbProp = IsProperty(id) ? VARIANT_TRUE : VARIANT_FALSE;
		return TRUE;
	}
	return FALSE;
}

HRESULT CThingObject::IdFromName(LPOLESTR psz, DISPID* pid)
{
	HRESULT hr;
	VARIANT_BOOL bValid;
	CComBSTR bstr(psz);

	ASSERT(psz);
	ASSERT(pid);
	if (psz == NULL || pid == NULL)
		return E_POINTER;

	if (m_bStub == VARIANT_FALSE)
	{
		// check if a property or method
		hr = IsValidProperty(bstr, &bValid);
		if (SUCCEEDED(hr) && bValid == VARIANT_TRUE)
		{
			// set id to next available (even) doctored one
			*pid = m_idNext;

			// increment (wrap around).
			// NOTE: we don't have to be unique here.  Mostly, we're
			// doing this so that we can check to assure that OLE Scripting
			// is always calling GetIDsOfNames before calling Invoke
			m_idNext += 2;
			if ((m_idNext+1) >= idMaxDoctored)
				m_idNext = idMinDoctored;

			return S_OK;
		}

		hr = IsValidMethod(bstr, &bValid);
		if (SUCCEEDED(hr) && bValid==VARIANT_TRUE)
		{
			// set id to next available (odd) doctored one
			*pid = m_idNext + 1;

			// increment (wrap around).
			// NOTE: we don't have to be unique here.  Mostly, we're
			// doing this so that we can check to assure that OLE Scripting
			// is always calling GetIDsOfNames before calling Invoke
			m_idNext += 2;
			if ((m_idNext+1) >= idMaxDoctored)
				m_idNext = idMinDoctored;

			return S_OK;
		}
	}

	// didn't find it
	return DISP_E_UNKNOWNNAME;
}

HRESULT CThingObject::NameFromId(DISPID id, BSTR* pbstr)
{
	ASSERT(pbstr);
	if (id == m_idCached)
	{
		// yep
		*pbstr = m_bstrCached.Copy();
		return S_OK;
	}

	// Shouldn't happen - VBScript (at least), always seems to call
	// GetIDsOfNames immediately before Invoke
	*pbstr = NULL;
	return DISP_E_UNKNOWNNAME;

}

HRESULT VariantToLong(VARIANT var, long* dest, long defvalue)
{
	HRESULT hr = S_OK;

	if (var.vt == VT_ERROR) 
		*dest = defvalue;
	else if (var.vt == VT_I4) 
		*dest = var.lVal;
	else
	{
		CComVariant temp;
		
		hr = ::VariantChangeType(&temp, &var, 0, VT_I4);
		if (FAILED(hr))
			hr = DISP_E_TYPEMISMATCH;
		else 
			*dest = temp.lVal;
	}

	return hr;
}

HRESULT VariantToBSTR(VARIANT var, BSTR* dest, BSTR defvalue)
{
	HRESULT hr = S_OK;

	if (var.vt == VT_ERROR) 
		*dest = SysAllocString(defvalue);
	else if (var.vt == VT_BSTR) 
		*dest = SysAllocString(var.bstrVal);
	else
	{
		CComVariant temp;
		
		hr = ::VariantChangeType(&temp, &var, 0, VT_BSTR);
		if (FAILED(hr))
			hr = DISP_E_TYPEMISMATCH;
		else 
			*dest = SysAllocString(temp.bstrVal);
	}

	return hr;
}

HRESULT IsRoomInhabited(IThing* pRoom, VARIANT_BOOL* pbInhabited)
{
	HRESULT hr = S_OK;
	IPropertyList* pList = NULL;
	IThing* pThing = NULL;
	long lCount = 0L;
	long i;
	VARIANT_BOOL bAvatarConnected = VARIANT_FALSE;
	static CComBSTR bstrAvatars("Avatars");
	static CComBSTR bstrIsConnected("IsConnected");

	if (pRoom == NULL)
		return E_INVALIDARG;

	if (pbInhabited == NULL)
		return E_POINTER;

	*pbInhabited = VARIANT_FALSE;

	hr = pRoom->get_ObjectProperty(bstrAvatars, (IObjectProperty**)&pList);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED_1;

	pList->get_Count(&lCount);

	for (i = 0; i < lCount; i++)
	{
		hr = pList->get_ObjectProperty(i, (IObjectProperty**)&pThing);
		if (SUCCEEDED(hr) && pThing != NULL)
		{
			/* hr = */ pThing->get_BOOL(bstrIsConnected, &bAvatarConnected);

			SAFERELEASE(pThing);

			// found a connected avatar, room is inhabited
			if (bAvatarConnected == VARIANT_TRUE)
			{
				*pbInhabited = VARIANT_TRUE;
				break;
			}
		}
	}

ERROR_ENCOUNTERED_1:
	SAFERELEASE(pList);

	return hr;
}

#ifdef _DEBUG
CString TraceProperty(VARIANT var)
{
	CString str;

	switch (var.vt)
	{
	case VT_EMPTY:
		str.Format("VT_EMPTY");
		break;
	case VT_ERROR:
		str.Format("VT_ERROR");
		break;
	case VT_NULL:
		str.Format("VT_NULL");
		break;
	case VT_I2:
		str.Format("VT_I2 %d", var.iVal);
		break;
	case VT_I4:
		str.Format("VT_I4 %ld", var.lVal);
		break;
	case VT_INT:
		str.Format("VT_INT %ld", var.intVal);
		break;
	case VT_R4:
		str.Format("VT_R4 %f", var.fltVal);
		break;
	case VT_R8:
		str.Format("VT_R8 %lf", var.dblVal);
		break;
	case VT_BSTR:
		str.Format("VT_BSTR %s", CString(var.bstrVal));
		break;
	case VT_BOOL:
		str.Format("VT_BOOL %s", var.boolVal == VARIANT_TRUE ? "true" : "false");
		break;
	case VT_UI1:
		str.Format("VT_UI1 %c", var.bVal);
		break;
	case VT_UI2:
		str.Format("VT_UI2 %u", var.uiVal);
		break;
	case VT_UI4:
		str.Format("VT_UI4 %lu", var.ulVal);
		break;
	case VT_UINT:
		str.Format("VT_UINT %lu", var.uintVal);
		break;
	case VT_DISPATCH:
		{
			IDispatch* pdisp = var.pdispVal;
			CLSID clsid;

			// marshall tag byte for null pointers
			if (pdisp == NULL)
			{
				str.Format("VT_DISPATCH (null)");
			}
			else
			{
				IObjectProperty* pprop = NULL;

				if (SUCCEEDED(pdisp->QueryInterface(IID_IObjectProperty, (LPVOID*)&pprop)))
				{
					HRESULT hr = pprop->get_CLSID(&clsid);
					
					IThing* pthing = NULL;

					// if IThing, dump object name
					if (SUCCEEDED(pprop->QueryInterface(IID_IThing, (LPVOID*)&pthing)))
					{
						BSTR bstr = NULL;
						VARIANT_BOOL bStub;
						CString strName;

						if (SUCCEEDED(pthing->get_IsStub(&bStub)))
						{
							long lObjectID = NULL_OBJECT;

							if (bStub == VARIANT_TRUE)
								strName	= "(stub)";
							else if (SUCCEEDED(pthing->get_Name(&bstr)))
							{
								strName = bstr;
								SAFEFREESTRING(bstr);
							}

							pthing->get_ID(&lObjectID);

							str.Format("VT_DISPATCH IThing, name (%s), ID %d", strName, lObjectID);
						}

						SAFERELEASE(pthing);
					}
					// else, dump property
					else
					{
						CComBSTR bstrType;

						pprop->get_Type(&bstrType.m_str);

						CString strCLSID = VWStringFromCLSID(clsid);
						str.Format("VT_DISPATCH IObjectProperty, type (%s), %s", CString(bstrType), strCLSID);
					}

					SAFERELEASE(pprop);
				}
				// else, just an IDispatch, dump as NULL
				else
				{
					str.Format("VT_DISPATCH IDispatch (marshalled as NULL)");
				}
			}
		}
		break;
	case VT_UNKNOWN:
		str.Format("VT_UNKNOWN");
		break;
	default:
		// unknown property type
		str.Format("VT_??? (%d) unknown property", var.vt);
		break;
	}

	return str;
}
#endif