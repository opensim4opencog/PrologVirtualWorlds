// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// File:	VWSUtils.cpp
// Author:	D.S.
//
// Description: 
//   Utility functions for objlib, mainly to deal with BSTRs without cluttering
//  code.
//
#include <stdafx.h>
#include <d3drm.h>
#include <stdtools.h>
#include "vwsutils.h"
#include <propbase.h>
#include <math.h>
#include "syshelp.h"

//////////////////////////////////////////////////////////////////////////
typedef CVWComPtr<IObjectProperty,NULL,&IID_IObjectProperty> CObjectPropertyPtr;

static CComBSTR cbstrGeometry = "Geometry";
static CComBSTR cbstrPosition = "Position";

#define PI 3.14159265359f
//////////////////////////////////////////////////////////////////////////

CString PropMap_GetBSTRProp( IPropertyMap * pPM, char * pszName ) {
	HRESULT hr = S_OK;
	CString cstrPropName = pszName;
	CString cstrReturn = "";
	BSTR bstrPropName = cstrPropName.AllocSysString();
	BSTR bstrReturnValue = NULL;

	ASSERT( pPM );
	if( pPM	) {
		hr = pPM->get_String(bstrPropName, &bstrReturnValue);
		// Ignore hr because property may not exist.
	}
	// Only return string if it got a good value.
	if(S_OK == hr)
		cstrReturn = bstrReturnValue;
	SAFEFREESTRING(bstrPropName);
	SAFEFREESTRING(bstrReturnValue);
	return cstrReturn;
}

HRESULT Thing_AddBSTRProp( IThing * pThing, char * pszName, char * pszValue ) {
	HRESULT hr = S_OK;
	CString strPropName(pszName);
	CString strPropValue(pszValue);
	BSTR bstrPropName = strPropName.AllocSysString();
	BSTR bstrPropValue = strPropValue.AllocSysString();

	ASSERT( pThing );
	if ( pThing ) {
		hr = pThing->AddString( bstrPropName, bstrPropValue ); 
		ASSERT( S_OK == hr );
	}
	else
		hr = E_POINTER;

	SAFEFREESTRING(bstrPropValue);
	SAFEFREESTRING(bstrPropName);

	return hr;
}

HRESULT Thing_AddObjectProp( IThing * pThing, char * pszName, IObjectProperty * pObject ) {
	HRESULT hr = S_OK;
	CString strPropName(pszName);
	BSTR bstrPropName = strPropName.AllocSysString();

	ASSERT( pThing );
	if ( pThing ) {
		hr = pThing->AddObjectProperty( bstrPropName, pObject ); 
		ASSERT( S_OK == hr );
	}
	else
		hr = E_POINTER;
	SAFEFREESTRING(bstrPropName);

	return hr;
}

HRESULT World_CreateStockExemplar( IWorld * pWorld, char * pszName, char * pszValue, IThing** ppthing ) {
	HRESULT hr = S_OK;
	CString strPropName(pszName);
	CString strPropValue(pszValue);
	BSTR bstrPropName = strPropName.AllocSysString();
	BSTR bstrPropValue = strPropValue.AllocSysString();
	ASSERT( pWorld );
	if ( pWorld ) {
		hr = pWorld->CreateRootExemplar( /*bstrPropName,*/ bstrPropValue, ppthing ); 
		ASSERT( S_OK == hr );
	}
	else
		hr = E_POINTER;
	SAFEFREESTRING(bstrPropName);
	SAFEFREESTRING(bstrPropValue);
	return hr;
}

HRESULT World_CreateExemplar( IWorld * pWorld, char * pszName, char * pszValue, IThing* pthingParent, IThing** ppthing ) {
	HRESULT hr = S_OK;
	CString strPropName(pszName);
	CString strPropValue(pszValue);
	BSTR bstrPropName = strPropName.AllocSysString();
	BSTR bstrPropValue = strPropValue.AllocSysString();
	ASSERT( pWorld );
	if ( pWorld ) {
		hr = pWorld->CreateExemplarExt( bstrPropName, bstrPropValue, CComVariant(pthingParent), ppthing ); 
		ASSERT( S_OK == hr );
	}
	else
		hr = E_POINTER;
	SAFEFREESTRING(bstrPropName);
	SAFEFREESTRING(bstrPropValue);
	return hr;
}


HRESULT Thing_IsOfType( IThing * pThing, char * pszName, VARIANT_BOOL * fIsOfType ) 
{
	HRESULT hr = S_OK;
	CString strPropName(pszName);
	BSTR bstrPropName = strPropName.AllocSysString();
	ASSERT( pThing );
	if ( pThing ) {
		hr = pThing->IsOfType( bstrPropName, fIsOfType ); 
		ASSERT( S_OK == hr );
		if(FAILED(hr))
			goto EXIT_FAIL;
	}
	else
		hr = E_POINTER;

EXIT_FAIL:
	SAFEFREESTRING(bstrPropName);
	return hr;
}

HRESULT Thing_GetObjectPropertiesMap( IThing * pThing, IPropertyMap ** ppReturnPMap )
{
	HRESULT hr = S_OK;
	IPropertyMap * pmapPropertyClones = NULL;
	IWorld * pWorld = NULL;
	COleVariant varThingProperty,varSelectedObjProp;
	CObjectPropertyPtr OPPtr;
	BSTR bstrReturnValue = NULL;
	VARIANT_BOOL	fLastItem = VARIANT_FALSE;
	varSelectedObjProp.Clear();
	varThingProperty.Clear();

	if( NULL == ppReturnPMap)
	{
		hr = E_POINTER;
		goto EXIT_FAIL;
	}

	// Expect *ppReturnPMap to be null
	if( NULL != *ppReturnPMap )
	{
		hr = E_INVALIDARG;
		goto EXIT_FAIL;
	}
	// Get a map containing clones of all the properties.
	hr = pThing->get_Properties(&pmapPropertyClones);
	if(FAILED(hr)) 
		goto EXIT_FAIL;

	hr = pThing->get_World( & pWorld );
	if(FAILED(hr)) 
		goto EXIT_FAIL;

	// Create the property map we're going to return.
	hr = CreatePropertyMap(pWorld, ppReturnPMap);
	if(FAILED(hr)) 
		goto EXIT_FAIL;

	hr = pmapPropertyClones->get_IsEmpty( &fLastItem );
	if( FAILED ( hr) ) 	
		goto EXIT_FAIL;

	if( VARIANT_FALSE == fLastItem ) 
	{
		hr = pmapPropertyClones->FirstItem( &bstrReturnValue, &varThingProperty, &fLastItem );
		if( FAILED ( hr) ) 	
			goto EXIT_FAIL;

		while( VARIANT_FALSE == fLastItem ) 
		{
			OPPtr = varThingProperty;

			// If it's an ObjectProperty,"get" it into our PMap.
			if( OPPtr != NULL )
			{
				hr = pThing->get_Property(bstrReturnValue, varSelectedObjProp);
				if( FAILED ( hr) ) 	
					goto EXIT_FAIL;

				hr = (*ppReturnPMap)->put_Property(bstrReturnValue, varSelectedObjProp);
				if( FAILED ( hr) ) 	
					goto EXIT_FAIL;

				varSelectedObjProp.Clear();
			}
			SAFEFREESTRING(bstrReturnValue);

			// Get the next item from the list of clones.
			varThingProperty.Clear();
			hr = pmapPropertyClones->NextItem( &bstrReturnValue, &varThingProperty, &fLastItem );
			if( FAILED ( hr) ) 
				goto EXIT_FAIL;
		} // while( VARIANT_FALSE != fLastItem ) 
	}  // if( VARIANT_FALSE == fLastItem )

	goto EXIT_SUCCEEDED;
EXIT_FAIL:
	// Release the returned map if there are errors.
	SAFERELEASE( *ppReturnPMap );

EXIT_SUCCEEDED:
	SAFEFREESTRING(bstrReturnValue);
	SAFERELEASE( pmapPropertyClones );
	SAFERELEASE( pWorld );
	return hr;
}

