// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// PrpEdCtl.cpp : Implementation of the CPropEdCtrl ActiveX Control class.

#include "stdafx.h"
#include "objexplr.h"
#include "PrpEdCtl.h"

#include <propbase.h>
#include <cellprop.h>
#include <vector.h>
#include <vwgeom.h>
#include <menuitem.h>
#include <menuitem_i.c>
#include <vector_i.c>
#include <vwgeom_i.c>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FLT_MAX 	3.402823466e+38F	/* max value */

IMPLEMENT_DYNCREATE(CPropEdCtrl, CVWUIView)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CPropEdCtrl, CVWUIView)
	//{{AFX_MSG_MAP(CPropEdCtrl)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CPropEdCtrl, CVWUIView)
	//{{AFX_DISPATCH_MAP(CPropEdCtrl)
	DISP_PROPERTY_EX(CPropEdCtrl, "VWClient", GetVWClient, SetVWClient, VT_DISPATCH)
	DISP_PROPERTY_EX(CPropEdCtrl, "TargetObjectProperty", GetTargetObjectProperty, SetTargetObjectProperty, VT_DISPATCH)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CPropEdCtrl, CVWUIView)
	//{{AFX_EVENT_MAP(CPropEdCtrl)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CPropEdCtrl, "OBJEXPLR.PropEdCtrl.1",
	0x192ff350, 0x17b, 0x11d2, 0x88, 0xbe, 0, 0xc0, 0x4f, 0xc3, 0x2e, 0xf3)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CPropEdCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DPropEd =
		{ 0x192ff34e, 0x17b, 0x11d2, { 0x88, 0xbe, 0, 0xc0, 0x4f, 0xc3, 0x2e, 0xf3 } };
const IID BASED_CODE IID_DPropEdEvents =
		{ 0x192ff34f, 0x17b, 0x11d2, { 0x88, 0xbe, 0, 0xc0, 0x4f, 0xc3, 0x2e, 0xf3 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwPropEdOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CPropEdCtrl, IDS_PROPED, _dwPropEdOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CPropEdCtrl::CPropEdCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CPropEdCtrl

BOOL CPropEdCtrl::CPropEdCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_PROPED,
			IDB_PROPED,
			afxRegApartmentThreading,
			_dwPropEdOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CPropEdCtrl::CPropEdCtrl - Constructor

CPropEdCtrl::CPropEdCtrl()
{
	InitializeIIDs(&IID_DPropEd, &IID_DPropEdEvents);
}


/////////////////////////////////////////////////////////////////////////////
// CPropEdCtrl::~CPropEdCtrl - Destructor

CPropEdCtrl::~CPropEdCtrl()
{
	// TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CPropEdCtrl::DoPropExchange - Persistence support

void CPropEdCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	CVWUIView::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CPropEdCtrl::OnResetState - Reset control to default state

void CPropEdCtrl::OnResetState()
{
	CVWUIView::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CPropEdCtrl message handlers

LPDISPATCH CPropEdCtrl::GetVWClient() 
{
	return CVWUIView::GetVWClient();
}

void CPropEdCtrl::SetVWClient(LPDISPATCH newValue) 
{
	CVWUIView::SetVWClient(newValue);

	SetModifiedFlag();
}

LPDISPATCH CPropEdCtrl::GetTargetObjectProperty() 
{
	return CVWUIView::GetTargetObjectProperty();
}

void CPropEdCtrl::SetTargetObjectProperty(LPDISPATCH newValue) 
{
	CVWUIView::SetTargetObjectProperty(newValue);
}

void CPropEdCtrl::GetTypeFromIID(GUID lID, CString &strType)
{
	if (IsEqualGUID(IID_IMethod, lID))
		strType.LoadString(IDS_TYPE_INLINE_METHOD);
	else if (IsEqualGUID(IID_IThing, lID))
		strType.LoadString(IDS_TYPE_THING);
	else if (IsEqualGUID(IID_IVector, lID))
		strType.LoadString(IDS_TYPE_VECTOR);
	else if (IsEqualGUID(IID_IVWGeometry, lID))
		strType.LoadString(IDS_TYPE_GEOMETRY);
	else if (IsEqualGUID(IID_IObjectProperty, lID))
		strType.LoadString(IDS_TYPE_OBJECT_PROPERTY);
	else
		strType.LoadString(IDS_TYPE_OBJECT);
}

BOOL CPropEdCtrl::GetTypeAndValueFromSecurity(IPropertySecurity *pSec, CString &strType, CString &strValue, VARTYPE &varType)
{
	CComVariant var;
	HRESULT hr = S_OK;
	GUID ID;

	if (FAILED(hr = pSec->get_Property(&var)))
		goto failexit;

	varType = var.vt;

	if (!GetTypeAndValueFromVariant(var, strType, strValue))
		goto failexit;

	if (strType.IsEmpty())
	{
		// this is really only for empty dispatch or unknown vars
		if ((var.vt != VT_DISPATCH) & (var.vt != VT_UNKNOWN))
			goto failexit;

		if (FAILED(hr = pSec->get_PropertyIIDExt(&ID)))
			goto failexit;
		
		GetTypeFromIID(ID, strType);
		strValue.LoadString(IDS_VALUE_EMPTY);
	}

	return TRUE;

failexit:
	return FALSE;
}

BOOL CPropEdCtrl::GetTypeAndValueFromSecurity(IPropertySecurity *pSec, CString &strType, CString &strValue)
{
	CComVariant var;
	HRESULT hr = S_OK;
	GUID ID;

	if (FAILED(hr = pSec->get_Property(&var)))
		goto failexit;

	if (!GetTypeAndValueFromVariant(var, strType, strValue))
		goto failexit;

	if (strType.IsEmpty())
	{
		// this is really only for empty dispatch or unknown vars
		if ((var.vt != VT_DISPATCH) & (var.vt != VT_UNKNOWN))
			goto failexit;

		if (FAILED(hr = pSec->get_PropertyIIDExt(&ID)))
			goto failexit;
		
		GetTypeFromIID(ID, strType);
		strValue.LoadString(IDS_VALUE_EMPTY);
	}

	return TRUE;

failexit:
	return FALSE;
}

BOOL CPropEdCtrl::GetTypeAndValueFromVariant(VARIANT varItem, CString &strType, CString &strValue)
{
	strType.Empty();
	strValue.Empty();

	VARIANT_BOOL vbVal;
	VARIANT_BOOL vbInline;
	char buf[256];
	IObjectProperty * pProperty = NULL;
	IMethod * pMethod = NULL;
	IThing * pThing = NULL;
	IVector *pVector = NULL;
	IVWGeometry *pGeometry = NULL;
	BSTR bstrType = NULL;
	BSTR bstrValue = NULL;
	HRESULT hr = S_OK;
	const char * name = NULL;
	IMenuItem* pMenuItem = NULL;
	long lFlags = 0;

	switch(varItem.vt)
	{
		case VT_BOOL:
			vbVal = varItem.boolVal;
			strType.LoadString(IDS_TYPE_BOOLEAN);
			if (vbVal == VARIANT_TRUE)
				strValue.LoadString(IDS_VALUE_TRUE);
			else
				strValue.LoadString(IDS_VALUE_FALSE);
			break;
		case VT_BSTR:
			strType.LoadString(IDS_TYPE_STRING);
			strValue = varItem.bstrVal;
			break;
		case VT_I2:
			strType.LoadString(IDS_TYPE_INTEGER);
			sprintf(buf, "%d", varItem.iVal);
			strValue = buf;
			break;
		case VT_I4:
			strType.LoadString(IDS_TYPE_LONG);
			sprintf(buf, "%d", varItem.lVal);
			strValue = buf;
			break;
		case VT_R4:
			strType.LoadString(IDS_TYPE_FLOAT);
			sprintf(buf, "%.3f", varItem.fltVal);
			strValue = buf;
			break;
		case VT_R8:
			strType.LoadString(IDS_TYPE_DOUBLE);
			sprintf(buf, "%lg", varItem.dblVal);
			strValue = buf;
			break;
		case VT_UNKNOWN:
			// If It's a property, add it
			if (varItem.punkVal != NULL)
			{
				if ( SUCCEEDED ( hr = varItem.punkVal->QueryInterface( IID_IMethod, (LPVOID*) &pMethod) ) )
				{
					ASSERT( pMethod );
					hr = pMethod->get_IsInline(&vbInline);
					if (vbInline == VARIANT_TRUE)
					{
						strType.LoadString(IDS_TYPE_INLINE_METHOD);
					}
					else
					{
						hr = pMethod->get_Type( &bstrType );
						if (FAILED(hr))	goto failexit;
						if (bstrType)
							strType = bstrType;
						else
							strType.Empty();
					}
					strValue.LoadString(IDS_VALUE_METHOD);
				}
				else if ( SUCCEEDED ( hr = varItem.punkVal->QueryInterface( IID_IThing, (LPVOID*) &pThing) ) )
				{
					ASSERT( pThing );
					VARIANT_BOOL bStub;
					VARIANT_BOOL bExemplar;
					
					if (FAILED(hr = pThing->get_IsStub( &bStub )))
						goto failexit;
					
					if (bStub == VARIANT_FALSE)
					{
						if (FAILED(hr = pThing->get_IsExemplar(&bExemplar)))
							goto failexit;
						
						if (bExemplar == VARIANT_TRUE)
							strType.LoadString(IDS_TYPE_EXEMPLAR);
						else
						{
							if (FAILED(hr = pThing->get_Type( &bstrType )))
								goto failexit;

							if (bstrType)
								strType = bstrType;
							else
								strType.Empty();
						}
						
						BSTR bstrValue = NULL;
						
						if (FAILED(hr = pThing->ToAscii( &bstrValue )))
							goto failexit;

						if (bstrValue)
							strValue = bstrValue;
						else
							strValue.Empty();
					}
					else
					{
						// REVIEW: This is only until we put in the code that
						// lets us get the type and values out of stub objects
						strType = "Thing";
						strValue = "Stub";
					}
				}
				else if ( SUCCEEDED ( hr = varItem.punkVal->QueryInterface( IID_IVector, (LPVOID*) &pVector) ) )
				{
					ASSERT( pVector );
					hr = pVector->get_Type( &bstrType );
					if (FAILED(hr)) goto failexit;
					strType = bstrType;
					float x,y,z;
					hr = pVector->get( &x, &y, &z );
					if (FAILED(hr)) goto failexit;
					sprintf(buf, "%.3f, %.3f, %.3f", x,y,z);
					strValue = buf;
				}
				else if ( SUCCEEDED ( hr = varItem.punkVal->QueryInterface( IID_IVWGeometry, (LPVOID*) &pGeometry) ) )
				{
					ASSERT( pGeometry );
					hr = pGeometry->get_Type( &bstrType );
					if (FAILED(hr)) goto failexit;
					strType = bstrType;

					hr = pGeometry->get_GeometryName( &bstrValue );
					if (FAILED(hr)) goto failexit;
					strValue = bstrValue;
				}
				else if ( SUCCEEDED ( hr = varItem.punkVal->QueryInterface( IID_IObjectProperty, (LPVOID*) &pProperty) ) )
				{
					ASSERT( pProperty );
					hr = pProperty->get_Type( &bstrType );
					if (FAILED(hr)) goto failexit;
					strType = bstrType;
					BSTR bstrValue;
					hr = pProperty->ToAscii( &bstrValue );
					if (FAILED(hr)) goto failexit;
					if (bstrValue)
						strValue = bstrValue;
					else
						strValue.Empty();
				}
				else
				{
					strType.LoadString(IDS_TYPE_UNKNOWN);
					strValue.LoadString(IDS_VALUE_UNKNOWN);
				}
			}
			else
			{
				// strType.LoadString(IDS_TYPE_UNKNOWN);
				// strValue.LoadString(IDS_VALUE_EMPTY);
				// pass back empty strings and get the info from the security property
				strType.Empty();
				strValue.Empty();
			}
			break;
		case VT_DISPATCH:
			if (varItem.pdispVal != NULL)
			{
				if ( SUCCEEDED ( hr = varItem.pdispVal->QueryInterface( IID_IMethod, (LPVOID*) &pMethod) ) )
				{
					ASSERT( pMethod );
					hr = pMethod->get_IsInline(&vbInline);
					if (vbInline == VARIANT_TRUE)
						strType.LoadString(IDS_TYPE_INLINE_METHOD);
					else
					{
						hr = pMethod->get_Type( &bstrType );
						if (FAILED(hr)) goto failexit;
						strType = bstrType;
					}
					strValue.LoadString(IDS_VALUE_METHOD);
				}
				else if ( SUCCEEDED ( hr = varItem.pdispVal->QueryInterface( IID_IThing, (LPVOID*) &pThing) ) )
				{
					ASSERT( pThing );
					VARIANT_BOOL bStub;
					VARIANT_BOOL bExemplar;

					if (FAILED(hr = pThing->get_IsStub( &bStub )))
						goto failexit;

					if (bStub == VARIANT_FALSE)
					{
						if (FAILED(hr = pThing->get_IsExemplar(&bExemplar)))
							goto failexit;
						
						if (bExemplar == VARIANT_TRUE)
							strType.LoadString(IDS_TYPE_EXEMPLAR);
						else
						{
							if (FAILED(hr = pThing->get_Type( &bstrType )))
								goto failexit;

							if (bstrType)
								strType = bstrType;
							else
								strType.Empty();
						}

						BSTR bstrValue;
						
						if (FAILED(hr = pThing->ToAscii( &bstrValue )))
							goto failexit;

						if (bstrValue)
							strValue = bstrValue;
						else
							strValue.Empty();
					}
					else
					{
						// REVIEW: This is only until we put in the code that
						// lets us get the type and values out of stub objects
						strType = "Thing";
						strValue = "Stub";
					}
				}
				else if ( SUCCEEDED ( hr = varItem.pdispVal->QueryInterface( IID_IVector, (LPVOID*) &pVector) ) )
				{
					ASSERT( pVector );
					hr = pVector->get_Type( &bstrType );
					if (FAILED(hr)) goto failexit;
					strType = bstrType;
					float x,y,z;
					hr = pVector->get( &x, &y, &z );
					if (FAILED(hr)) goto failexit;
					sprintf(buf, "%.3f, %.3f, %.3f", x,y,z);
					strValue = buf;
				}
				else if ( SUCCEEDED ( hr = varItem.pdispVal->QueryInterface( IID_IVWGeometry, (LPVOID*) &pGeometry) ) )
				{
					ASSERT( pGeometry );
					hr = pGeometry->get_Type( &bstrType );
					if (FAILED(hr)) goto failexit;
					strType = bstrType;

					hr = pGeometry->get_GeometryName( &bstrValue );
					if (FAILED(hr)) goto failexit;
					strValue = bstrValue;
				}
				else if( SUCCEEDED ( hr = varItem.pdispVal->QueryInterface( IID_IObjectProperty, (LPVOID*) &pProperty) ) )
				{
					if(SUCCEEDED(hr = pProperty->QueryInterface(IID_IMenuItem, (void**)&pMenuItem)))
					{
						ASSERT( pProperty );
						hr = pProperty->get_Type( &bstrType );
						if (FAILED(hr)) goto failexit;
						if (bstrType)
							strType = bstrType;
						else
							strType.Empty();

						BSTR bstrValue = NULL;
						hr = pMenuItem->get_Method(&bstrValue);
						if(FAILED(hr)) goto failexit;


						if(!bstrValue)
							strValue.Empty();
						else
						{
							strValue.LoadString(IDS_MENUITEMVALUE);
							strValue+= bstrValue;
						}

						// if we are viewing a separator, clear the value and type strings
						hr = pMenuItem->get_Flags(&lFlags);
						if(FAILED(hr)) goto failexit;

						if(lFlags & MI_SEPARATOR)
							strValue.LoadString(IDS_SEPARATOR);

						SAFEFREESTRING(bstrValue);

					}
					else
					{
						ASSERT( pProperty );
						hr = pProperty->get_Type( &bstrType );
						if (FAILED(hr)) goto failexit;
						if (bstrType)
							strType = bstrType;
						else
							strType.Empty();

						BSTR bstrValue = NULL;
						hr = pProperty->ToAscii( &bstrValue );
						if (FAILED(hr) && hr != E_NOTIMPL) goto failexit;
						if (!bstrValue)
							strValue.Empty();
						else
							strValue = bstrValue;
					}
				}
				else
				{
					strType.LoadString(IDS_TYPE_DISPATCH);
					strValue.LoadString(IDS_VALUE_UNKNOWN);
				}

			}
			else
			{
				// strType.LoadString(IDS_TYPE_UNKNOWN);
				// strValue.LoadString(IDS_VALUE_EMPTY);
				// pass back empty strings and get the info from the security property
				strType.Empty();
				strValue.Empty();
			}
			break;

		default:
			// strType.LoadString(IDS_TYPE_UNKNOWN);
			// strValue.LoadString(IDS_VALUE_EMPTY);
			// pass back empty strings and get the info from the security property
			strType.Empty();
			strValue.Empty();
			break;
	}

	SAFEFREESTRING(bstrType);
	SAFEFREESTRING(bstrValue);
	SAFERELEASE(pMethod);
	SAFERELEASE(pProperty);
	SAFERELEASE(pThing);
	SAFERELEASE(pMenuItem);
	SAFERELEASE(pVector);
	return TRUE;

failexit:
	SAFEFREESTRING(bstrType);
	SAFEFREESTRING(bstrValue);
	SAFERELEASE(pMethod);
	SAFERELEASE(pProperty);
	SAFERELEASE(pThing);
	SAFERELEASE(pMenuItem);
	SAFERELEASE(pVector);
	return FALSE;
}

//
// DoubleClick
// iItem - item that was clicked on
// iColumn - column the double click was in
//
// called when a user double clicks in the list control
void CPropEdCtrl::DoubleClick(int iItem, int iColumn)
{
}


//
// LeftClick
// iItem - item that was clicked on
// iColumn - column the double click was in
//
// called when a user double clicks in the list control
void CPropEdCtrl::LeftClick(int iItem, int iColumn)
{
}


//
// PopUpMenu
// point - location of the right click
//
// called when a user double clicks in the list control
void CPropEdCtrl::PopUpMenu(CPoint point)
{
}

//
// VirtualKeyPressed
//
// called when a user presses a key in the range between VK_BACK and VK_HELP
void CPropEdCtrl::VirtualKeyPressed(UINT nChar)
{
}

//
// VirtualKeyUp
//
// called when a user releases a key in the range between VK_BACK and VK_HELP
void CPropEdCtrl::VirtualKeyUp(UINT nChar)
{
}

//
// AlphaKeyPressed
//
// called when a user presses a letter key in the list control
void CPropEdCtrl::AlphaKeyPressed(char cLetter)
{
}

//
// CheckIfParent
// see if pParent is in the exemplar chain of pChild
//
BOOL CPropEdCtrl::CheckIfParent(IThing *pParent, IThing *pChild)
{
	// corner cases
	if (!pParent)
		return FALSE;
	if (!pChild)
		return FALSE;
	if (pParent == pChild)
		return FALSE;

	IThing *pTempThing = NULL;
	if (FAILED(pChild->get_Exemplar(&pTempThing)))
		goto failexit;

	if (pTempThing != NULL)
	{
		if (pParent == pTempThing)
		{
			SAFERELEASE(pTempThing);
			return TRUE;
		}
		else
		{
			BOOL bReturn = CheckIfParent(pParent, pTempThing);
			SAFERELEASE(pTempThing);
			return bReturn;
		}
	}

failexit:
	SAFERELEASE(pTempThing);
	return FALSE;
}

//
// Editable
// given a varType, checks to see if it is one of the ones that we can
// edit in-line
//
BOOL CPropEdCtrl::Editable(VARTYPE vt)
{
	switch(vt)
	{
		case VT_BOOL:
		case VT_BSTR:
		case VT_I2:
		case VT_I4:
		case VT_R4:
		case VT_R8:
			return TRUE;
	}

	return FALSE;
}

//
// EditableType
// given a string type, checks to see if it is one we don't support editing on
//
BOOL CPropEdCtrl::EditableType(CString strType)
{
	BOOL bReturnVal = TRUE;
	IThing *pGlobalThing = NULL;
	IObjectProperty *pProp = NULL;
	IPropertyMap *pEditorMap;
	HRESULT hr = S_OK;
	VARIANT_BOOL vBool;

	if (!m_pWorld)
		return FALSE;

	if (FAILED(hr = m_pWorld->get_Global(&pGlobalThing)) || !pGlobalThing)
		return FALSE;

	if (FAILED(hr = pGlobalThing->get_ObjectProperty(CComBSTR("PropertyEditors"), (IObjectProperty **) &pProp)) || !pProp)
	{
		bReturnVal = FALSE;
		goto FAIL_EXIT;
	}
			
	if (FAILED(hr = pProp->QueryInterface(IID_IPropertyMap, (LPVOID*)&pEditorMap)))
		goto FAIL_EXIT;


	if (FAILED(hr = pEditorMap->IsValid(CComBSTR(strType), &vBool)))
	{
		bReturnVal = FALSE;
		goto FAIL_EXIT;
	}

	bReturnVal = (vBool == VARIANT_TRUE);

FAIL_EXIT:
	SAFERELEASE(pProp);
	SAFERELEASE(pEditorMap);
	SAFERELEASE(pGlobalThing);

	return bReturnVal;
}

//
// DoneEditingValue
// The list ctrl is done editing the value of a property.
// Should be overriden by the derrived list props since
// they actually have pointers to their list controls
// and they know how to change values of props
//
void CPropEdCtrl::DoneEditingValue(int iItem, CString strNewValue)
{
}

//
// DoneEditingName
// The list ctrl is done editing the name of a property.
// Should be overriden by the derrived list props since
// they actually have pointers to their list controls
// and they know how to change values of props
//
void CPropEdCtrl::DoneEditingName(int iItem, CString strNewValue)
{
}

// LaunchExternalEditor
// the button on the list ctrl has been pushed
void CPropEdCtrl::LaunchExternalEditor(int iItem)
{
}

void CPropEdCtrl::ConvertStringToVariant(VARTYPE varType, CString strNewValue, CComVariant &newVarValue)
{
	double f = 0.0F;
	long i = 0;
	CString strNum = strNewValue.SpanIncluding("0123456789.-Ee+");
	CString strCompare;

	switch (varType)
	{
		case VT_R4:
			f = atof(strNum.GetBuffer(strNum.GetLength()));
			/*
			if ((f > FLT_MAX) || (f < -FLT_MAX))
			{
				UI::Warning(IDS_WARNING_OVERFLOW);
			}
			*/
			newVarValue = (float) f;
			break;
		case VT_R8:
			/*
			if (strNum.GetLength() > 15)
			{
				//	UI::Warning(IDS_WARNING_OVERFLOW);
			}
			*/
			f = atof(strNum.GetBuffer(strNum.GetLength()));
			newVarValue = (double) f;
			break;
		case VT_I2:
			newVarValue.vt = VT_I2;
			i = atoi(strNum.GetBuffer(strNum.GetLength()));
			// warn of overflow
			/*
			if ((i > 32767) || (i < -32768))
			{
			//	UI::Warning(IDS_WARNING_OVERFLOW);
			}
			*/
			newVarValue.intVal = (int) i;
			break;
		case VT_I4:
			{
				newVarValue.vt = VT_I4;
				if (strNum.IsEmpty())
				{
					newVarValue.intVal = (long) 0;
					break;
				}
				BOOL bNeg = (strNum[0] == '-');
				CString strTestNum;
				
				if (bNeg)
					strTestNum = strNum.Right(strNum.GetLength() - 1);
				else
					strTestNum = strNum;

				int len = strTestNum.GetLength();

				if (len > 9)
				{
					// possible overflow
					if (len == 10)
					{
						// positive value with a length of 10
						// get the first 9 chars and see
						CString strCheck = strTestNum.Left(9);
						long test = atol(strCheck.GetBuffer(strCheck.GetLength()));
						if (test > 214748364)
						{
						//	UI::Warning(IDS_WARNING_OVERFLOW);
						}
						else
						{
							if (test == 214748364)
							{
								// got to check the last digit
								CString strCheck2 = strTestNum.Mid(9,1);
								int test2 = atoi(strCheck2.GetBuffer(strCheck2.GetLength()));
								if ((!bNeg && (test2 > 7)) || (bNeg && (test2 > 8)))
								{
								//	UI::Warning(IDS_WARNING_OVERFLOW);
								}
							}
						}
					}
//					else
//						UI::Warning(IDS_WARNING_OVERFLOW);
				}

				i = atol(strNum.GetBuffer(strNum.GetLength()));
				newVarValue.intVal = (long) i;
			}
			break;
		case VT_BOOL:
			newVarValue.vt = VT_BOOL;
			strCompare.LoadString(IDS_VALUE_TRUE);
			if (!strNewValue.CompareNoCase(strCompare))
			{
				newVarValue.boolVal = VARIANT_TRUE;
			}
			else
				newVarValue.boolVal = VARIANT_FALSE;
			break;
		case VT_BSTR:
				newVarValue = strNewValue;
			break;
		default:
			break;
	}
}

void CPropEdCtrl::OnDestroy() 
{
	CVWUIView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
