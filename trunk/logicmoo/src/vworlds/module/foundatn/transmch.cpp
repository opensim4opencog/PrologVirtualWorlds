// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// TransMch.cpp: Implementation of TransactionMachine exemplar

#include "stdafx.h"
#include <vwfound.h>
#include <vector.h>
#include <menuitem.h>
#include <vwutils.h>
#include <tranito.h>
#include <vwevents.h>  //string definitions for events
#include "foundatn.h"
#include "propbase.h"
#include "syshelp.h"
#include "mmhelp.h"
// #include "..\..\vwobject\propsecu.h" // CSetUID needed for security overrides

extern HRESULT _GetFrameHelper(IThing *pThing, IVWFrame **ppvwf);
#define GETPROP(obj,name,val) hr = ##obj->get_##name(##val); if (FAILED(hr)) goto CLEAN_UP
#define GETADDEDPROP(obj,type,name,val) hr = ##obj->get_##type(##name, ##val); if (FAILED(hr)) goto CLEAN_UP

#define PUTPROP(obj,name,val) hr = ##obj->put_##name(##val); if (FAILED(hr)) goto CLEAN_UP
#define PUTADDEDPROP(obj,type,name,val) hr = ##obj->put_##type(##name, ##val); if (FAILED(hr)) goto CLEAN_UP

//////////////////////////////////////////////////////////////////////////
// A TransactionMachine, or vending machine, dispenses objects to users.
// The transaction machine contains several different objects (so you can
// get the on left mouse click event) that are all descended from artifact:
// a display, previous button, next button, and select button.
//
// The TransactionMachine also has a property list of TransactionItems 
// (see /property/tranitem/tranito.cpp for details) that contain
// all the information needed to generate an object when a user selects one:
// name, description, graphic file, exemplar type, and a property map that 
// contains additional information about the object. So, for an accessory,
// the property map would contain the property name/value pair of  
// IsAccessory, true.
//
// Note: the TransactionItem's propertymap, PropertyValues, is not remoted 
// since it is a collection within a collection.  Right now it is isn't 
// necessary since the user only needs the object that is created on the 
// server using the property map. [RK, 5.19.98]


//////////////////////////////////////////////////////////////////////////
// Install method:
// Installs transaction machine exemplar and the exemplars contained within it:
// Display, PreviousButton, NextButton, and Select Button

HRESULT CFoundationExemplars::TMchInstall(IModule* pModule)
{
	// Array of methods for transaction machine
	CMethodInfo rgMethod[] =
	{
		{ METHOD_SERVER,	"OnCreate",					"TMchOnCreate",					PSBIT_NONE, PS_EXEMPLARDEFAULTMETHOD },
		{ METHOD_CLIENT,	"UpdateCurrentIndex",		"TMchUpdateCurrentIndex",		PSBIT_NONE, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"SetCurrentIndex",			"TMchSetCurrentIndex",			PSBIT_NONE, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"OnCurrentIndexChanged",	"TMchOnCurrentIndexChanged",	PSBIT_NONE, PS_ALLEXECUTEMETHOD },
		{ METHOD_CLIENT,	"GetCurrentItem",			"TMchGetCurrentItem",			PSBIT_NONE, PS_ALLEXECUTEMETHOD },
		{ METHOD_SERVER,	"CreateAndDispenseObject",	"TMchCreateAndDispenseObject",	PSBIT_NONE, PS_ALLEXECUTEMETHOD }
	};
	int cMethod = sizeof(rgMethod)/sizeof(rgMethod[0]);
    int iMethod;

	HRESULT hr = S_OK;
	CComPtr<IThing> pTransMachineEx;
	CComPtr<IVector> pVector;
	CComPtr<IPropertyList> pItemList;

	CComPtr<IPropertyList> pMenu;
	CComPtr<IPropertyList> pMenuEx;
	CComPtr<IMenuItem> pMenuItem;
	CComPtr<IThing> pPrevBtnEx;
	CComPtr<IThing> pNextBtnEx;
	CComPtr<IThing> pSelectBtnEx;
	CComPtr<IThing> pDisplayEx;
	CComBSTR bstrName;
	CComBSTR bstrNameInternal;

	static CComBSTR bstrGeometryName("GeometryName");

	// Create the transaction machine exemplar
	if (FAILED(hr = m_pWorld->CreateExemplarEx(CComBSTR("Transaction Machine Exemplar"),
												CComBSTR("TransactionMachine"),
												CComVariant(CComBSTR("Artifact")), &pTransMachineEx)))
		goto ERROR_ENCOUNTERED;

	// Set the description string
	if (FAILED(hr = pTransMachineEx->put_Description(CComBSTR("A transaction machine which dispenses goods"))))
		goto ERROR_ENCOUNTERED;

	// Create a geometry for it
	if (FAILED(hr = pTransMachineEx->put_String(bstrGeometryName, CComBSTR("Client/Exemplar/Vending/Hvend.x"))))
		goto ERROR_ENCOUNTERED;

	// Set contents (buttons, display) to be visible
	if (FAILED(hr = pTransMachineEx->put_BOOL(CComBSTR("IsContentsVisible"), VARIANT_TRUE)))
		goto ERROR_ENCOUNTERED;

	// Add properties
	if (FAILED(hr = pTransMachineEx->AddPropertyExt(CComBSTR("Display"),
												CComVariant((IDispatch *) NULL),
												PSBIT_EXEMPLARDEFAULTPROPERTY,
												PS_EXEMPLARDEFAULTPROPERTY,
												VT_DISPATCH, IID_IThing, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pTransMachineEx->AddPropertyExt(CComBSTR("CurrentIndex"),
												CComVariant((short) 0),
												PSBIT_EXEMPLARDEFAULTPROPERTY,
												PS_ALLACCESSPROPERTY,
												VT_I2, IID_IDispatch, NULL)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = CreatePropertyList(m_pWorld, &pItemList)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pTransMachineEx->AddPropertyExt(CComBSTR("ItemList"), 
												CComVariant((IDispatch *) pItemList), 
												PSBIT_EXEMPLARDEFAULTPROPERTY,
												PS_EXEMPLARDEFAULTCOLLECTION, 
												VT_DISPATCH, IID_IPropertyList, NULL)))
		goto ERROR_ENCOUNTERED;

    pItemList.Release();

	// Create "sub" exemplars with geometry for previous/next buttons, select button, & display
	// Previous Button
	if (FAILED(hr = m_pWorld->CreateExemplarEx(CComBSTR("Previous Button Exemplar"),
									   			 CComBSTR("PreviousButton"),
												 CComVariant(CComBSTR("Artifact")), &pPrevBtnEx)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pPrevBtnEx->put_String(bstrGeometryName, CComBSTR("Client/Exemplar/Vending/VButLeft.x"))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = CreateVector(m_pWorld, 0.0, 0.0, 0.0, &pVector)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pPrevBtnEx->put_ObjectProperty(CComBSTR("Position"), pVector)))
		goto ERROR_ENCOUNTERED;
	pVector.Release();

	if (FAILED(hr = pPrevBtnEx->put_BOOL(CComBSTR("IsNoticeable"), VARIANT_TRUE)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pPrevBtnEx->put_BOOL(CComBSTR("IsTakeable"), VARIANT_FALSE)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pPrevBtnEx->CreateAndAddMethodExt(METHOD_CLIENT, pModule,
												 CComBSTR("TMchPreviousButton_OnLButtonDown"),
												 CComBSTR("OnLButtonDown"),
												 PSBIT_NONE, PS_ALLEXECUTEMETHOD)))
		goto ERROR_ENCOUNTERED;

	// Next Button
	if (FAILED(hr = m_pWorld->CreateExemplarEx(CComBSTR("Next Button Exemplar"),
												 CComBSTR("NextButton"),
												 CComVariant(CComBSTR("Artifact")), &pNextBtnEx)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pNextBtnEx->put_String(bstrGeometryName, CComBSTR("Client/Exemplar/Vending/VBuRight.x"))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = CreateVector(m_pWorld, 0.0, 0.0, 0.0, &pVector)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pNextBtnEx->put_ObjectProperty(CComBSTR("Position"), pVector)))
		goto ERROR_ENCOUNTERED;
	pVector.Release();

	if (FAILED(hr = pNextBtnEx->put_BOOL(CComBSTR("IsNoticeable"), VARIANT_TRUE)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pNextBtnEx->put_BOOL(CComBSTR("IsTakeable"), VARIANT_FALSE)))
		goto ERROR_ENCOUNTERED;
	
	if (FAILED(hr = pNextBtnEx->CreateAndAddMethodExt(METHOD_CLIENT, pModule,
												 CComBSTR("TMchNextButton_OnLButtonDown"),
												 CComBSTR("OnLButtonDown"),
												 PSBIT_NONE, PS_ALLEXECUTEMETHOD)))
		goto ERROR_ENCOUNTERED;

	// Select Button
	if (FAILED(hr = m_pWorld->CreateExemplarEx(CComBSTR("Select Button Exemplar"),
												 CComBSTR("SelectButton"),
												 CComVariant(CComBSTR("Artifact")), &pSelectBtnEx)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pSelectBtnEx->put_String(bstrGeometryName, CComBSTR("Client/Exemplar/Vending/VButBuy.x"))))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = CreateVector(m_pWorld, 0.0, 0.0, 0.0, &pVector)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pSelectBtnEx->put_ObjectProperty(CComBSTR("Position"), pVector)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pSelectBtnEx->put_BOOL(CComBSTR("IsNoticeable"), VARIANT_TRUE)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pSelectBtnEx->put_BOOL(CComBSTR("IsTakeable"), VARIANT_FALSE)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pSelectBtnEx->CreateAndAddMethodExt(METHOD_CLIENT, pModule,
												 CComBSTR("TMchSelectButton_OnLButtonDown"),
												 CComBSTR("OnLButtonDown"),
												 PSBIT_NONE, PS_ALLEXECUTEMETHOD)))
		goto ERROR_ENCOUNTERED;

	// Display
	if (SUCCEEDED(hr = m_pWorld->CreateExemplarEx(CComBSTR("Display Exemplar"),
											   CComBSTR("Display"),
											   CComVariant(CComBSTR("Artifact")), &pDisplayEx)))
    {
	    CComBSTR bstrGeomFile("worlds/plazas/commons/filerock.gif");
	    CComVariant varFile(bstrGeomFile);

	    CComDISPPARAMS dpInitGraphics(10, varFile, 
							      CComVariant(0.0f), CComVariant(0.45f), CComVariant(0.0f),
							      CComVariant(0.0f), CComVariant(0.0f), CComVariant(1.0f),
							      CComVariant(0.25f), CComVariant(0.25f), CComVariant(0.25f));

	    pDisplayEx->InvokeMethodExt(CComBSTR("InitializeSpriteGraphics"), (DISPPARAMS *) dpInitGraphics, NULL);
    }
    else
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pDisplayEx->put_BOOL(CComBSTR("IsNoticeable"), VARIANT_TRUE)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pDisplayEx->put_BOOL(CComBSTR("IsTakeable"), VARIANT_FALSE)))
		goto ERROR_ENCOUNTERED;
	// SetGraphic must be server-side so that we have the proper security context
	if (FAILED(hr = pDisplayEx->CreateAndAddMethodExt(METHOD_SERVER, pModule,
												 CComBSTR("TMchDisplay_SetGraphic"),
												 CComBSTR("SetGraphic"),
												 PSBIT_NONE, PS_ALLEXECUTEMETHOD)))
		goto ERROR_ENCOUNTERED;
	
	// Add in the methods
	for (iMethod=0; iMethod<cMethod; iMethod++)
	{
		bstrName = rgMethod[iMethod].pszName;
		if (rgMethod[iMethod].pszNameInternal == NULL)
			bstrNameInternal = bstrName;
		else
			bstrNameInternal = rgMethod[iMethod].pszNameInternal;
		if (FAILED(hr = pTransMachineEx->CreateAndAddMethodExt(	rgMethod[iMethod].lFlags,
														pModule,
														bstrNameInternal,
														bstrName,
														rgMethod[iMethod].psbits,
														rgMethod[iMethod].permissions)))
    		goto ERROR_ENCOUNTERED;
	}


    // Add menu items here
    return S_OK;

ERROR_ENCOUNTERED:
	
    return hr;
}

//////////////////////////////////////////////////////////////////////////
// OnCreate
// Adds in instances of "subobjects":
// vending display, previous button, next button, and select button 

STDMETHODIMP CFoundationExemplars::TMchOnCreate(IThing* pThing)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
    HRESULT         hr;
    
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		goto ERROR_ENCOUNTERED;
	
	if (pThis)
	{
		//Create a display and set the TransactionMachine.Display to it
		CComPtr<IThing> pDisplay;
		CComPtr<IVWGeometry> pGeometry;
		CComPtr<IThing> pPrevBtn;
		CComPtr<IThing> pNextBtn;
		CComPtr<IThing> pSelectBtn;
		
		if (FAILED(hr = m_pWorld->CreateInstanceExt(CComBSTR("Display"),
														 CComVariant(CComBSTR("Display")), pThis,
														 &pDisplay)))
		    goto ERROR_ENCOUNTERED;
		
		if (FAILED(hr = pThis->put_ObjectProperty(CComBSTR("Display"), (IObjectProperty*) pDisplay)))
		    goto ERROR_ENCOUNTERED;

		// Add buttons to contents
		if (FAILED(hr = m_pWorld->CreateInstanceExt(CComBSTR("PreviousButton"),
														 CComVariant(CComBSTR("PreviousButton")), pThis,
														 &pPrevBtn)))
		    goto ERROR_ENCOUNTERED;

		if (FAILED(hr = m_pWorld->CreateInstanceExt(CComBSTR("NextButton"),
														 CComVariant(CComBSTR("NextButton")), pThis,
														 &pNextBtn)))
		    goto ERROR_ENCOUNTERED;

		if (FAILED(hr = m_pWorld->CreateInstanceExt(CComBSTR("SelectButton"),
														 CComVariant(CComBSTR("SelectButton")), pThis,
														 &pSelectBtn)))
		    goto ERROR_ENCOUNTERED;
	}

    return S_OK;

ERROR_ENCOUNTERED:
    
    VWTRACE(m_pWorld, "VWFOUND", VWT_ERROR, "CFoundationExemplars::TMchOnCreate  Error %x", hr);

	return hr;
}

//////////////////////////////////////////////////////////////////////////
// UpdateCurrentIndex
// Updates the current index after a user has clicked previous or next

STDMETHODIMP CFoundationExemplars::TMchUpdateCurrentIndex(short nChangeAmt)
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
	CComPtr<IPropertyList> pItemList;
	short nCurIndex;
	long nCount;
    HRESULT hr;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		goto ERROR_ENCOUNTERED;
	
	// Get current index and property list and update index depending on where you're at
	if (pThis)
	{
		if (FAILED(hr = pThis->get_Short(CComBSTR("CurrentIndex"), &nCurIndex)))
	    	goto ERROR_ENCOUNTERED;
		
        nCurIndex += nChangeAmt;
		
        if (FAILED(hr = pThis->get_ObjectProperty(CComBSTR("ItemList"), (IObjectProperty **) &pItemList)))
		    goto ERROR_ENCOUNTERED;
	
		if (pItemList)
		{
			if (FAILED(hr = pItemList->get_Count(&nCount)))
		        goto ERROR_ENCOUNTERED;
			// Check edge conditions
			// Are we on first item and trying to go back one?
			// If so, go to last item in item list
			if (nCurIndex<0)
				nCurIndex = ((short)nCount-1);
			// Are we on last item trying to go forward one?
			// If so, go to first item on list
			if (nCurIndex>((short)nCount-1))
				nCurIndex = 0;
			// Now set current index
			if (FAILED(hr = pThis->put_Short(CComBSTR("CurrentIndex"), nCurIndex)))
        		goto ERROR_ENCOUNTERED;
		}
	}

    return S_OK;

ERROR_ENCOUNTERED:

    VWTRACE(m_pWorld, "VWFOUND", VWT_ERROR, "CFoundationExemplars::TMchUpdateCurrentIndex  Error %x\n", hr);

	return hr;
}

//////////////////////////////////////////////////////////////////////////
// SetCurrentIndex
// Accessor method to set current index 

STDMETHODIMP CFoundationExemplars::TMchSetCurrentIndex(short nIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing>	pThis;
    HRESULT         hr;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		goto ERROR_ENCOUNTERED;

	if (pThis)
    {
		if (FAILED(hr = pThis->put_Short(CComBSTR("CurrentIndex"), nIndex)))
	    	goto ERROR_ENCOUNTERED;
    }

    return S_OK;

ERROR_ENCOUNTERED:
    
    VWTRACE(m_pWorld, "VWFOUND", VWT_ERROR, "CFoundationExemplars:TMchSetCurrentIndex - index not set: %x\n", hr);

	return hr;
}

//////////////////////////////////////////////////////////////////////////
// OnCurrentIndexChanged
// When the current TransactionItem is changed, this is called to
// update the display with the current item's graphic file

STDMETHODIMP CFoundationExemplars::TMchOnCurrentIndexChanged(long lHint, VARIANT varHintData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing>				pThis;
	CComPtr<IThing>				pDisplay;
	CComPtr<IVWGeometry>		pDisplayGeometry;
	CComPtr<IPropertyList>		pItemList;
	CComPtr<IVWTransactionItem> pCurItem;
	CComVariant					varReturnVal;
	CComBSTR					bstrNewGraphicFile;
    HRESULT                     hr;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
    {
	    CComPtr<IThing> pOwner, pCaller;
	    CComBSTR bstrOwnerName, bstrCallerName;
	    if (FAILED(hr = pThis->get_Owner(&pOwner)))
		    goto ERROR_ENCOUNTERED;
	    // No owner on client, is a stub
	    // if (pOwner)
	    //		if (FAILED(hr = pOwner->get_Name(&bstrOwnerName.m_str)))
		//          goto ERROR_ENCOUNTERED;
	    if (FAILED(hr = m_pWorld->get_Caller(&pCaller)))
		    goto ERROR_ENCOUNTERED;
	    if (pCaller)
        {
		    if (FAILED(hr = pCaller->get_Name(&bstrCallerName.m_str)))
		       goto ERROR_ENCOUNTERED;
        }
    }
#endif		
	if (pThis)
	{
		if (FAILED(hr = pThis->InvokeMethodExt(CComBSTR("GetCurrentItem"), NULL, &varReturnVal)))
		    goto ERROR_ENCOUNTERED;
		if (varReturnVal.vt != VT_DISPATCH || !varReturnVal.pdispVal)
        {
            hr = E_FAIL;
            goto ERROR_ENCOUNTERED;
        }

        pCurItem = (IVWTransactionItem *) varReturnVal.pdispVal;
		if (pCurItem)
        {
			if (FAILED(hr = pCurItem->get_Graphic(&bstrNewGraphicFile.m_str)))
		        goto ERROR_ENCOUNTERED;
        }

		// Now get the display object and set its graphic(GeometryName) to the new graphic
		// create function to do this?
		if (FAILED(hr = pThis->get_Thing(CComBSTR("Display"), &pDisplay)))
		    goto ERROR_ENCOUNTERED;
		if (FAILED(hr = pDisplay->InvokeMethodExt(CComBSTR("SetGraphic"), CComDISPPARAMS(1, CComVariant(bstrNewGraphicFile)), NULL)))
		    goto ERROR_ENCOUNTERED;
	}

    return S_OK;
ERROR_ENCOUNTERED:
    
    VWTRACE(m_pWorld, "VWFOUND", VWT_ERROR, "CFoundationExemplars::TMchOnCurrentIndexChanged  Error %x\n", hr);

	return hr;
}


//////////////////////////////////////////////////////////////////////////
// GetCurrentItem
// Returns a pointer to current TransactionItem in ItemList

STDMETHODIMP CFoundationExemplars::TMchGetCurrentItem(IVWTransactionItem **pTransactionItem)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	ASSERT(pTransactionItem);
	*pTransactionItem = NULL;

	CComPtr<IThing>				pThis;	
	CComPtr<IPropertyList>		pItemList;
	short						nCurIndex;
    HRESULT                     hr;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThis->get_Short(CComBSTR("CurrentIndex"), &nCurIndex)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pThis->get_ObjectProperty(CComBSTR("ItemList"), (IObjectProperty**) &pItemList)))
		goto ERROR_ENCOUNTERED;
	if (pItemList)
	{
		if (FAILED(hr = pItemList->get_ObjectProperty(nCurIndex, (IObjectProperty**) pTransactionItem)))
    		goto ERROR_ENCOUNTERED;
	}

    return S_OK;

ERROR_ENCOUNTERED:
    VWTRACE(m_pWorld, "VWFOUND", VWT_ERROR, "CFoundationExemplars::TMchGetCurrentItem  Error %x\n", hr);

	return hr;
}

//////////////////////////////////////////////////////////////////////////
// CreateAndDispenseObject
// Once a user has clicked select, creates a new object within the user

STDMETHODIMP CFoundationExemplars::TMchCreateAndDispenseObject()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	VWTRACE(m_pWorld, "VWFOUND", VWT_ERROR, "CFoundationExemplars::TMchCreateAndDispenseObject\n");

	CComPtr<IThing>				pThis;
	CComPtr<IThing>				pUser;
	CComPtr<IThing>				pNewObject;
	CComPtr<IPropertyList>		pItemList;
	CComPtr<IVWTransactionItem>	pCurItem;
	CComPtr<IPropertyMap>		pPropertyValues;
	CComBSTR					bstrName;
	CComBSTR					bstrDescription;
	CComBSTR					bstrGraphic;
	CComBSTR					bstrExemplarType;
	CComVariant					varReturnVal;
	CComVariant					varPropertyValue;
	BSTR						bstrPropertyName;
	VARIANT_BOOL				bLastValue;
	VARIANT_BOOL				bIsEmpty;
	VARIANT_BOOL				bIsValid;
	short						nCurIndex;
    HRESULT                     hr;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pWorld->get_User(&pUser)))
		goto ERROR_ENCOUNTERED;

	if ((pThis)&&(pUser))
	{
		if (FAILED(hr = pThis->get_Short(CComBSTR("CurrentIndex"), &nCurIndex)))
    		goto ERROR_ENCOUNTERED;
		if (FAILED(hr = pThis->get_ObjectProperty(CComBSTR("ItemList"), (IObjectProperty**) &pItemList)))
	    	goto ERROR_ENCOUNTERED;
		if (pItemList)
		{
			if (FAILED(hr = pItemList->get_ObjectProperty(nCurIndex, (IObjectProperty**) &pCurItem)))
		        goto ERROR_ENCOUNTERED;
			if (pCurItem)
			{
				if (FAILED(hr = pCurItem->get_Name(&bstrName.m_str)))
		            goto ERROR_ENCOUNTERED;
				if (FAILED(hr = pCurItem->get_Description(&bstrDescription.m_str)))
		            goto ERROR_ENCOUNTERED;
				if (FAILED(hr = pCurItem->get_Graphic(&bstrGraphic.m_str)))
		            goto ERROR_ENCOUNTERED;
				if (FAILED(hr = pCurItem->get_ExemplarType(&bstrExemplarType.m_str)))
		            goto ERROR_ENCOUNTERED;
				if (FAILED(hr = pCurItem->get_PropertyValues(&pPropertyValues)))
		            goto ERROR_ENCOUNTERED;

				// Now, create new object
				if (FAILED(hr = m_pWorld->CreateInstanceExt(bstrName, CComVariant(bstrExemplarType), NULL, &pNewObject)))
		            goto ERROR_ENCOUNTERED;
				
				// Set new object's properties
				if (pNewObject)
				{
					CComVariant varFile(bstrGraphic);
					CComDISPPARAMS dpInitGraphics(10, varFile, 
					CComVariant(0.0f), CComVariant(0.45f), CComVariant(0.0f),
					CComVariant(0.0f), CComVariant(0.0f), CComVariant(1.0f),
					CComVariant(0.25f), CComVariant(0.25f), CComVariant(0.25f));
					VARIANT_BOOL	bIsArtifact;
					VARIANT_BOOL	bIsAccessory;

#ifdef _DEBUG
					CComPtr<IThing> pOwner, pCaller;
					CComBSTR bstrOwnerName, bstrCallerName;
					if (FAILED(hr = pNewObject->get_Owner(&pOwner)))
		                goto ERROR_ENCOUNTERED;
					if (pOwner)
                    {
						if (FAILED(hr = pOwner->get_Name(&bstrOwnerName.m_str)))
		                    goto ERROR_ENCOUNTERED;
                    }
					if (FAILED(hr = m_pWorld->get_Caller(&pCaller)))
		                goto ERROR_ENCOUNTERED;
					if (pCaller)
                    {
						if (FAILED(hr = pCaller->get_Name(&bstrCallerName.m_str)))
		                    goto ERROR_ENCOUNTERED;
                    }
#endif

				    if (FAILED(hr = pNewObject->InvokeMethodExt(CComBSTR("InitializeSpriteGraphics"), (DISPPARAMS *) dpInitGraphics, NULL)))
		                goto ERROR_ENCOUNTERED;
					if (FAILED(hr = pNewObject->put_Description(bstrDescription)))
		                goto ERROR_ENCOUNTERED;
					if (FAILED(hr = pNewObject->put_BOOL(CComBSTR("IsTakeable"), VARIANT_TRUE)))
		                goto ERROR_ENCOUNTERED;

					// Set additional properties if specified in PropertyValue propertymap
					if (FAILED(hr = pPropertyValues->get_IsEmpty(&bIsEmpty)))
		                goto ERROR_ENCOUNTERED;						
					if (bIsEmpty!=VARIANT_TRUE)
					{
						// Get first pair of values 
						if (FAILED(hr = pPropertyValues->FirstItem(&bstrPropertyName, &varPropertyValue, &bLastValue)))
		                    goto ERROR_ENCOUNTERED;
							
						// Add name/value pair to object and iterate through rest of map
						while (bLastValue!=VARIANT_TRUE)
						{
							// Test that the property name is valid
							// Review: do we need other checks on property name, value?
							if (FAILED(hr = pPropertyValues->IsValid(bstrPropertyName, &bIsValid)))
		                        goto ERROR_ENCOUNTERED;
							if (bIsValid)
                            {
								if (FAILED(hr = pNewObject->put_Property(bstrPropertyName, varPropertyValue)))
		                            goto ERROR_ENCOUNTERED;
                            }

							// Now free values
							SAFEFREESTRING(bstrPropertyName);
							varPropertyValue.Clear();

							// Get next item
							if (FAILED(hr = pPropertyValues->NextItem(&bstrPropertyName, &varPropertyValue, &bLastValue)))
		                        goto ERROR_ENCOUNTERED;
						}
					}
					// Now check if exemplar type is Artifact and IsAccessory set to true,
					// then set the AccessoryGraphic property
					// Review: is there a better way of doing this? strcmp? part of loop in property map?
					// Seems like it would be much cleaner with an Accessory exemplar
					
					if (FAILED(hr = pNewObject->IsOfType(CComBSTR("Artifact"), &bIsArtifact)))
                		goto ERROR_ENCOUNTERED;
					if (bIsArtifact)
					{
						if (FAILED(hr = pNewObject->get_BOOL(CComBSTR("IsAccessory"), &bIsAccessory)))
		                    goto ERROR_ENCOUNTERED;
						if (bIsAccessory)
						{
							CComPtr<IThing> pGlobal;
							CComBSTR bstrEventName(VW_UIELEMENTCHANGED_EVENT_STR );

							if (FAILED(hr = pNewObject->put_Property(CComBSTR("AccessoryGraphic"), CComVariant(bstrGraphic))))
		                        goto ERROR_ENCOUNTERED;
							// Need to fire UI event to get the icon to update in inventory tab
							if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
		                        goto ERROR_ENCOUNTERED;
							if (FAILED(hr = pGlobal->InvokeMethodExt(CComBSTR("FireRemoteUIEvent"), 
								CComDISPPARAMS(4, CComVariant((IDispatch*) pUser), CComVariant((IDispatch*) pNewObject),
								CComVariant(bstrEventName), CComVariant()), NULL)))
		                        goto ERROR_ENCOUNTERED;
						}
					}
					// Tell user the object has been moved to inventory
					CComBSTR bstrWhisperText(bstrName);
					bstrWhisperText += " is now in your inventory.";
					if (FAILED(hr = pThis->InvokeMethodExt(CComBSTR("Whisper"), CComDISPPARAMS(2, CComVariant(bstrWhisperText), CComVariant((IDispatch*) pUser)), NULL)))
		                goto ERROR_ENCOUNTERED;
				}
			}
		}
	}

    return S_OK;	

ERROR_ENCOUNTERED:

    VWTRACE(m_pWorld, "VWFOUND", VWT_ERROR, "CFoundationExemplars::TMchCreateAndDispenseObject  Error %x\n", hr);

	return hr;
}

//////////////////////////////////////////////////////////////////////////
// TMchPreviousButton_OnLButtonDown
// Handles user clicking on next button
// Calls method to decrement the current index

STDMETHODIMP CFoundationExemplars::TMchPreviousButton_OnLButtonDown(long hWnd, long x, long y)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
	CComPtr<IThing> pContainer;
    HRESULT         hr;

	//Get container (the transaction machine) and call method to update current index
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		goto ERROR_ENCOUNTERED;
	if (pThis)
    {
		if (FAILED(hr = pThis->get_Container(&pContainer)))
	    	goto ERROR_ENCOUNTERED;
    }
	
	if (pContainer)
	{
		if (FAILED(hr = pContainer->InvokeMethodExt(CComBSTR("UpdateCurrentIndex"),
			CComDISPPARAMS(1, CComVariant(-1L)), NULL)))
		    	goto ERROR_ENCOUNTERED;
    }

	// [RK] trace out all exemplars
	// CComPtr<IThing> pGlobal;
	// m_pWorld->get_Global(&pGlobal);
	// pGlobal->InvokeMethodExt(CComBSTR("TraceExemplarInfo"), NULL, NULL);
	
    return S_OK;

ERROR_ENCOUNTERED:

	VWTRACE(m_pWorld, "VWFOUND", VWT_ERROR, "CFoundationExemplars::TMchPreviousButton_OnLButtonDown  Error %x\n", hr);

	return hr;
}

//////////////////////////////////////////////////////////////////////////
// TMchNextButton_OnLButtonDown
// Handles user clicking on next button
// Calls method to increment the current index

STDMETHODIMP CFoundationExemplars::TMchNextButton_OnLButtonDown(long hWnd, long x, long y)
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing> pThis;
	CComPtr<IThing> pContainer;
    HRESULT         hr;

	//Get container (the transaction machine) and call method to update current index
	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		goto ERROR_ENCOUNTERED;

	if (pThis)
    {
		if (FAILED(hr = pThis->get_Container(&pContainer)))
	    	goto ERROR_ENCOUNTERED;
    }
	
	if (pContainer)
	{
		if (FAILED(hr = pContainer->InvokeMethodExt(CComBSTR("UpdateCurrentIndex"),
			CComDISPPARAMS(1, CComVariant(1L)), NULL)))
	    	goto ERROR_ENCOUNTERED;
	}

    return S_OK;

ERROR_ENCOUNTERED:
    VWTRACE(m_pWorld, "VWFOUND", VWT_ERROR, "CFoundationExemplars::TMchPreviousButton_OnLButtonDown  Error %x\n", hr);

	return hr;
}

//////////////////////////////////////////////////////////////////////////
// TMchSelectButton_OnLButtonDown
// Handles user clicking on select button
// Calls CreateAndDispenseObject to do all the work

STDMETHODIMP CFoundationExemplars::TMchSelectButton_OnLButtonDown(long hWnd, long x, long y)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComPtr<IThing>	pThis;
	CComPtr<IThing>	pTransactionMachine;
    HRESULT         hr;

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		goto ERROR_ENCOUNTERED;

	if (pThis)
	{
		if (FAILED(hr = pThis->get_Container(&pTransactionMachine)))
	    	goto ERROR_ENCOUNTERED;
		if (pTransactionMachine)
		{
			if (FAILED(hr = pTransactionMachine->InvokeMethodExt(CComBSTR("CreateAndDispenseObject"), NULL, NULL)))
		    goto ERROR_ENCOUNTERED;
		}
	}

    return S_OK;

ERROR_ENCOUNTERED:
    
    VWTRACE(m_pWorld, "VWFOUND", VWT_ERROR, "CFoundationExemplars::TMchSelectButton_OnLButtonDown  Error \n");

	return hr;
}

//////////////////////////////////////////////////////////////////////////
// TMchDisplay_SetGraphic
// Changes graphic displayed when user clicks next or previous
// Need accessor method for security reasons

STDMETHODIMP CFoundationExemplars::TMchDisplay_SetGraphic(BSTR bstrNewGraphicFile)
{
	CComPtr<IThing>	pThis;
	CComPtr<IThing>	pContainer;
	CComPtr<IThing>	pContainerDisplay;
    HRESULT         hr;

	static CComBSTR bstrGeometryName("GeometryName");

	if (FAILED(hr = m_pWorld->get_This(&pThis)))
		goto ERROR_ENCOUNTERED;

#ifdef _DEBUG
    {
	    CComPtr<IThing> pOwner, pCaller;
	    CComBSTR bstrOwnerName, bstrCallerName;
	    if (FAILED(hr = pThis->get_Owner(&pOwner)))
		    goto ERROR_ENCOUNTERED;
	    if (pOwner)
        {
		    if (FAILED(hr = pOwner->get_Name(&bstrOwnerName.m_str)))
		       goto ERROR_ENCOUNTERED;
        }
	    if (FAILED(hr = m_pWorld->get_Caller(&pCaller)))
		    goto ERROR_ENCOUNTERED;
	    if (pCaller)
        {
		    if (FAILED(hr = pCaller->get_Name(&bstrCallerName.m_str)))
		       goto ERROR_ENCOUNTERED;
        }
    }
#endif
	if (pThis)
	{
		// Do security check:
		// Make sure that this instance is equal to the 
		// its TransactionMachine container's display

		if (FAILED(hr = pThis->get_Container(&pContainer)))
		    goto ERROR_ENCOUNTERED;
		if (pContainer)
		{
			if (FAILED(hr = pContainer->get_Thing(CComBSTR("Display"), &pContainerDisplay)))
		        goto ERROR_ENCOUNTERED;
			if (pContainerDisplay)
			{
				if (pThis==pContainerDisplay)
				{
					// Now go ahead and change the geometry
					if (FAILED(hr = pThis->put_String(bstrGeometryName, bstrNewGraphicFile)))
		                goto ERROR_ENCOUNTERED;
				}
			}
		}
	}

    return S_OK;

ERROR_ENCOUNTERED:
    VWTRACE(m_pWorld, "VWFOUND", VWT_ERROR, "CFoundationExemplars::TMchDisplay_SetGraphic  Error \n");

	return hr;
}