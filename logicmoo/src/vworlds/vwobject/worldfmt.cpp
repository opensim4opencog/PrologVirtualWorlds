// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// worldfmt.cpp : Implementation of text formatting.

#include <stdafx.h>
#include <vwobject.h>
#include "vwutils.h"

#ifndef LOCALTEST

#include "worldobj.h"

#else // LOCALTEST

typedef void IPropertyList;
typedef void IThing;
typedef void IWorld;

IWorld *m_pWorld = NULL;

#endif // LOCALTEST

static void TranslateCString(IWorld *pWorld, CString *cstrIn, IPropertyList *pObjects,
							 IThing *pPOV, CString *cstrOut);
static void SubstituteCString(IWorld *pWorld, CString *cstrIn, IPropertyList *pObjects,
							  IThing *pPOV, CString *cstrOut);
static void GetVarFromCString(IWorld *pWorld, CString cstrIn, IPropertyList *pObjects,
							  IThing *pPOV, VARIANT *varResult);
static int GetAltIndex(IPropertyList *pObjects, IThing *pPOV);
static void GetMainObject(IWorld *pWorld, CString cstrName, IPropertyList *pObjects,
						  IThing *pPOV, VARIANT *varResult);
static void GetSubObject(CString cstrName, VARIANT *varResult);

//----------------------------------------------------------------------------------
#ifndef LOCALTEST
STDMETHODIMP CWorldObject::Format(BSTR bstrIn, IPropertyList *pObjects, IThing *pPOV,
								  BSTR *bstrOut)
#else
HRESULT Format(BSTR bstrIn, IPropertyList *pObjects, IThing *pPOV, BSTR *bstrOut)
#endif
{
	// Catch any CString exceptions
	try
	{
		CString cstrIn = bstrIn;
		CString cstrOut;

		// Translate
		TranslateCString(m_pWorld, &cstrIn, pObjects, pPOV, &cstrOut);

		// Return the results
		*bstrOut = cstrOut.AllocSysString();
	}
	catch (CHRESULTException chr)
	{
		// Catch HRESULT problems
		return chr.GetHRESULT();
	}
	catch(CException*)
	{
		// Otherwise, it's almost certainly a CString memory problem
		return E_OUTOFMEMORY;
	}

	return S_OK;
}

//----------------------------------------------------------------------------------
#ifndef LOCALTEST
STDMETHODIMP CWorldObject::FormatSentences(BSTR bstrIn, IPropertyList *pObjects,
										   IThing *pPOV, BSTR *bstrOut)
#else
HRESULT FormatSentences(BSTR bstrIn, IPropertyList *pObjects, IThing *pPOV, BSTR *bstrOut)
#endif
{
	// Catch any CString exceptions
	try
	{
		CString cstrIn = bstrIn;
		CString cstrOut;

		// Translate
		TranslateCString(m_pWorld, &cstrIn, pObjects, pPOV, &cstrOut);

		// Capitalize as needed
		if (!cstrOut.IsEmpty())
		{
			VARIANT_BOOL boolCapitalizeNext = VARIANT_TRUE;
			int iSize = cstrOut.GetLength();

			// Go through each character
			for (int i = 0; i < iSize; i++)
			{
				// Skip spaces
				if (cstrOut[i] != ' ')
				{
					// If we find a period, then capitalize the next non-space
					if (cstrOut[i] == '.')
						boolCapitalizeNext = VARIANT_TRUE;
					else if (boolCapitalizeNext)
					{
						// Use CString to capitalize so we handle multi-byte properly
						CString cstrLetter = cstrOut[i];
						cstrLetter.MakeUpper();
						cstrOut.SetAt(i, cstrLetter[0]);
						boolCapitalizeNext = VARIANT_FALSE;
					}
				}
			}
		}

		// Return the results
		*bstrOut = cstrOut.AllocSysString();
	}
	catch (CHRESULTException chr)
	{
		// Catch HRESULT problems
		return chr.GetHRESULT();
	}
	catch(CException*)
	{
		// Otherwise, it's almost certainly a CString memory problem
		return E_OUTOFMEMORY;
	}

	return S_OK;
}

//----------------------------------------------------------------------------------
static void TranslateCString(IWorld *pWorld, CString *cstrIn, IPropertyList *pObjects,
							 IThing *pPOV, CString *cstrOut)
{
	int i;

	// Figure out which of the alternate strings to use
	int iAltIndex = GetAltIndex(pObjects, pPOV)+1;
	CString cstrAlternate;

	// Truncate everything up to the indexed alternate
	while (iAltIndex--)
	{
		// Start with an empty alternate
		cstrAlternate.Empty();
		// Skip through any quoted bars
		while (TRUE)
		{
			// Cycle through all the bracketed strings
			while (((i = cstrIn->FindOneOf("{|")) >= 0) && ((*cstrIn)[i] == '{'))
			{
				// Only process if it's not quoted
				if ((i <= 0) || ((*cstrIn)[i-1] != '\\'))
				{
					// Find the end of the bracket
					cstrAlternate += cstrIn->Left(i+1);
					*cstrIn = cstrIn->Mid(i+1);
					i = cstrIn->Find('}');
					if (i < 0)
						i = cstrIn->GetLength()-1;
				}
				// Copy it into the alternate
				cstrAlternate += cstrIn->Left(i+1);
				*cstrIn = cstrIn->Mid(i+1);
			}
			// If it's a quoted bar, skip it
			if ((i > 0) && ((*cstrIn)[i-1] == '\\'))
			{
				cstrAlternate += cstrIn->Left(i-1);
				cstrAlternate += "|";
				*cstrIn = cstrIn->Mid(i+1);
			}
			else
				// Otherwise, we're ready to continue
				break;
		}
		// If there was nothing left, this must have been it
		if (i < 0)
			break;

		// Otherwise, copy the remainder and try again
		cstrAlternate += cstrIn->Left(i);
		*cstrIn = cstrIn->Mid(i+1);
	}
	// Use the last alternate processed
	*cstrIn = cstrAlternate;

	while ((i = cstrIn->Find('{')) >= 0)
	{
		// Is this a real bracket?
		if ((i > 0) && ((*cstrIn)[i-1] == '\\'))
		{
			// If not, then output minus the escape character
			*cstrOut += cstrIn->Left(i-1);
			*cstrOut += "{";
			*cstrIn = cstrIn->Mid(i+1);
			continue;
		}

		// Append the front
		*cstrOut += cstrIn->Left(i);
		*cstrIn = cstrIn->Mid(i);

		// Process the substitution
		SubstituteCString(pWorld, cstrIn, pObjects, pPOV, cstrOut);
	}

	// Append the remainder
	*cstrOut += *cstrIn;
}

//----------------------------------------------------------------------------------
static void SubstituteCString(IWorld *pWorld, CString *cstrIn, IPropertyList *pObjects,
							  IThing *pPOV, CString *cstrOut)
{
	// Find the end of the substitution
	int iSubEnd = cstrIn->Find('}');

	// If end, pass through everything
	if (iSubEnd < 0)
	{
		*cstrOut = *cstrIn;
		cstrIn->Empty();
		return;
	}

	// Find the end of the first segment
	int iSegEnd = cstrIn->Find('|');

	// If it's past the substitution, it's not really for us
	if ((iSegEnd < 0) || (iSegEnd > iSubEnd))
		iSegEnd = iSubEnd;

	// Get the variable value
	CComVariant var;
	GetVarFromCString(pWorld, cstrIn->Mid(1, iSegEnd-1), pObjects, pPOV, &var);

	// Are we doing text or indexed substitution?
	if (iSegEnd == iSubEnd)
	{
		// Convert to a text string (if possible)
		TestHRESULT(var.ChangeType(VT_BSTR));
		*cstrOut += var.bstrVal;
	}
	else
	{
		// Convert to an index (if possible)
		TestHRESULT(var.ChangeType(VT_I4));

		// Go through one segment at a time (plus one for the var section)
		int iCnt = var.lVal+1;
		while (iCnt--)
		{
			// Drop this segment
			*cstrIn = cstrIn->Mid(iSegEnd+1);
			iSubEnd -= iSegEnd+1;
			// Find the next segment end
			iSegEnd = cstrIn->Find('|');
			if (iSegEnd < 0)
			{
				// If there is none, use this segment
				iSegEnd = iSubEnd;
				break;
			}
		}

		// Now use the segment we're on
		*cstrOut += cstrIn->Mid(0, iSegEnd);
	}

	// Trim the part we processed
	*cstrIn = cstrIn->Mid(iSubEnd+1);
}

//----------------------------------------------------------------------------------
static void GetVarFromCString(IWorld *pWorld, CString cstrIn, IPropertyList *pObjects,
							  IThing *pPOV, VARIANT *varResult)
{
	// Find the first element
	int iSegEnd = cstrIn.Find('.');

	// If there wasn't one, then use the whole string
	if (iSegEnd < 0)
		iSegEnd = cstrIn.GetLength();

	// Get the object
	CString cstrName = cstrIn.Left(iSegEnd);
	cstrName.TrimLeft();
	cstrName.TrimRight();
	GetMainObject(pWorld, cstrName, pObjects, pPOV, varResult);

	// Iterate through the sub-parts
	while (TRUE)
	{
		// don't let Mid run past end of string
		if (iSegEnd+1 > cstrIn.GetLength())
			break;

		// Remove last segment
		cstrIn = cstrIn.Mid(iSegEnd+1);
		// Done?
		if (cstrIn.IsEmpty())
			break;
		// Find the end
		iSegEnd = cstrIn.Find('.');
		if (iSegEnd < 0)
			iSegEnd = cstrIn.GetLength();
		// Process it
		cstrName = cstrIn.Left(iSegEnd);
		cstrName.TrimLeft();
		cstrName.TrimRight();
		GetSubObject(cstrName, varResult);
	};

	// If we still have an object, try to get it's name
	if (varResult->vt == VT_DISPATCH)
		GetSubObject(CString("name"), varResult);
}

#ifndef LOCALTEST

//----------------------------------------------------------------------------------
static int GetAltIndex(IPropertyList *pObjects, IThing *pPOV)
{
	CComPtr<IThing> pThing;

	// Only if we have both an object list and a POV
	if (pObjects && pPOV)
		// Find the POV object if it's in the list
		for (int i = 0; SUCCEEDED(pObjects->get_Thing(i, &pThing)); i++)
			if (pThing == pPOV)
				// If we found it, return the index
				return i;
			else
				pThing.Release();

	// Otherwise, return something very large (past the end for sure)
	return 10000;
}

//----------------------------------------------------------------------------------
static void GetMainObject(IWorld *pWorld, CString cstrName, IPropertyList *pObjects,
						  IThing *pPOV, VARIANT *varResult)
{
	// Default to IDispatch return
	varResult->vt = VT_DISPATCH;
	varResult->pdispVal = NULL;

	// First try for an index
	if (cstrName.SpanIncluding("0123456789") == cstrName)
	{
		CComVariant var;
		CComBSTR bstrName = cstrName;

		// Convert to a number
		var.vt = VT_BSTR;
		var.bstrVal = bstrName;
		TestHRESULT(var.ChangeType(VT_I4));

		// Get that object
		TestHRESULT(pObjects->get_Property(var.lVal, varResult));
	}
	// Otherwise, check for keywords
	else if (cstrName.CompareNoCase("world") == 0)
	{
		varResult->pdispVal = pWorld;
		SAFEADDREF(varResult->pdispVal);
	}
	else if (cstrName.CompareNoCase("user") == 0)
	{
		TestHRESULT(pWorld->get_User((IThing**) &varResult->pdispVal));
	}
	else if (cstrName.CompareNoCase("room") == 0)
	{
		CComPtr<IThing> pUser;
		TestHRESULT(pWorld->get_User(&pUser));
		TestHRESULT(pUser->get_Container((IThing**) &varResult->pdispVal));
	}
	else if (cstrName.CompareNoCase("this") == 0)
	{
		TestHRESULT(pWorld->get_This((IThing**) &varResult->pdispVal));
	}
	else if (cstrName.CompareNoCase("global") == 0)
	{
		TestHRESULT(pWorld->get_Global((IThing**) &varResult->pdispVal));
	}
	else if (cstrName.CompareNoCase("pov") == 0)
	{
		varResult->pdispVal = pPOV;
		SAFEADDREF(pPOV);
	}
	else if (cstrName.CompareNoCase("subject") == 0)
	{
		TestHRESULT(pObjects->get_Property(0, varResult));
	}
	else if (cstrName.CompareNoCase("object") == 0)
	{
		TestHRESULT(pObjects->get_Property(1, varResult));
	}
	else if (cstrName.CompareNoCase("indirectObject") == 0)
	{
		TestHRESULT(pObjects->get_Property(2, varResult));
	}
	else
	{
		// Default to a property or method on this
		TestHRESULT(pWorld->get_This((IThing**) &varResult->pdispVal));
		GetSubObject(cstrName, varResult);
	}
}

//----------------------------------------------------------------------------------
static void GetSubObject(CString cstrName, VARIANT *varResult)
{
	// Only object have subs
	if (varResult->vt == VT_DISPATCH)
	{
		HRESULT hr;
		DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
		DISPID dispid;
		CComVariant var;

		// Get the dispID
		CComBSTR bstrName = cstrName;
		hr = varResult->pdispVal->GetIDsOfNames(IID_NULL, &bstrName.m_str, 1,
												LOCALE_USER_DEFAULT, &dispid);
		if (SUCCEEDED(hr))
		{
			// Try getting a property
			hr = varResult->pdispVal->Invoke(dispid,IID_NULL, LOCALE_USER_DEFAULT,
											 DISPATCH_PROPERTYGET, &dispparamsNoArgs,
											 &var, NULL, NULL);
			if (FAILED(hr))
			{
				// If that failed, try a method invocation
				hr = varResult->pdispVal->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT,
												 DISPATCH_METHOD, &dispparamsNoArgs,
												 &var, NULL, NULL);
			}

			// If we got something, remember it
			if (SUCCEEDED(hr))
			{
				SAFERELEASE(varResult->pdispVal);
				TestHRESULT(var.Detach(varResult));
			}
		}
	}
}

#else // LOCALTEST

// The LOCALTEST versions of these routines fake actual object look-ups.
// Instead, they return numbers that are used later to index into tables
// to return the test values.

//----------------------------------------------------------------------------------
static int GetAltIndex(IPropertyList *pObjects, IThing *pPOV)
{
	return (int) pPOV;
}

//----------------------------------------------------------------------------------
static void GetMainObject(IWorld *pWorld, CString cstrName, IPropertyList *pObjects,
						  IThing *pPOV, VARIANT *varResult)
{
	int iResult = 100;

	// First try for an index
	if (cstrName.SpanIncluding("0123456789") == cstrName)
	{
		CComVariant var;
		CComBSTR bstrName = cstrName;

		// Convert to a number
		var.vt = VT_BSTR;
		var.bstrVal = bstrName;
		TestHRESULT(var.ChangeType(VT_I4));
		iResult = 100*var.lVal;
	}
	else if (cstrName.CompareNoCase("world") == 0)
		iResult = 1100;
	else if (cstrName.CompareNoCase("user") == 0)
		iResult = 1200;
	else if (cstrName.CompareNoCase("room") == 0)
		iResult = 1300;
	else if (cstrName.CompareNoCase("this") == 0)
		iResult = 1400;
	else if (cstrName.CompareNoCase("global") == 0)
		iResult = 1500;
	else if (cstrName.CompareNoCase("subject") == 0)
		iResult = 0;
	else if (cstrName.CompareNoCase("object") == 0)
		iResult = 100;
	else if (cstrName.CompareNoCase("indirectObject") == 0)
		iResult = 200;

	varResult->vt = VT_DISPATCH;
	varResult->pdispVal = (IDispatch*) iResult;
}

//----------------------------------------------------------------------------------
int iIndices[] = {1, 101, 201, 301, 401,
				  2, 102, 202, 302, 402,
				  1101, 1201, 1301, 1401, 1501,
				  1102, 1202, 1302, 1402, 1502,
				  -1
};

char *szText[] = {
	"the subject",
	"the object",
	"the ind-object1",
	"the ind-object2",
	"the ind-object3",

	"a subject",
	"an object",
	"an ind-object1",
	"an ind-object2",
	"an ind-object3",

	"the world",
	"the user",
	"the room",
	"the this",
	"the global",

	"a world",
	"a user",
	"a room",
	"a this",
	"a global"
};

static void GetSubObject(CString cstrName, VARIANT *varResult)
{
	// First try for an index
	if (cstrName.SpanIncluding("0123456789") == cstrName)
	{
		CComVariant var;
		CComBSTR bstrName = cstrName;

		// Convert to a number
		var.vt = VT_BSTR;
		var.bstrVal = bstrName;
		TestHRESULT(var.ChangeType(VT_I4));

		// And return it
		varResult->vt = VT_I4;
		varResult->lVal = var.lVal;
	}
	else
	{
		// Check for key properties
		if (cstrName.CompareNoCase("name") == 0)
			varResult->pdispVal = (IDispatch*) (((int) varResult->pdispVal)+1);
		else if (cstrName.CompareNoCase("description") == 0)
			varResult->pdispVal = (IDispatch*) (((int) varResult->pdispVal)+2);
		else if (cstrName.CompareNoCase("container") == 0)
			varResult->pdispVal = (IDispatch*) 1300;

		// If we've got a property, convert to a return value
		if ((((int) varResult->pdispVal) % 100) != 0)
		{
			// Set the default value
			char *iReturnString = "dunno";

			// Now look up the result
			int iResult = (int) varResult->pdispVal;
			for(int i = 0; iIndices[i] >= 0; i++)
				if (iIndices[i] == iResult)
				{
					iReturnString = szText[i];
					break;
				}

			varResult->vt = VT_BSTR;
			varResult->bstrVal = CComBSTR(iReturnString).Detach();
		}
	
	}
}

#endif // LOCALTEST
