// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// WWHelper.cpp : Implementation of CWorldWizHelper
#include "stdafx.h"
#include <webtools.h>
#include "WWHelper.h"
#include "reghelp.h"

/////////////////////////////////////////////////////////////////////////////
// CWorldWizHelper

STDMETHODIMP CWorldWizHelper::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IWorldWizHelper,
	};
	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CWorldWizHelper::get_TypeNames(VARIANT * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	long		cTypeNames;
	SAFEARRAY	*pTypeList = NULL;
	HRESULT		hr;

	if (m_typeNames.IsEmpty())
		return S_OK;

	cTypeNames = m_typeNames.GetCount();

	VariantInit(pVal);
	pTypeList = SafeArrayCreateVector(VT_VARIANT, 0, cTypeNames);
	
	if (pTypeList)
	{
		VARIANT		var;
		POSITION	pos;
		CString		curName;
		int			iPos;
		CString		strName;
		CComBSTR	bstrName;

		pos = m_typeNames.GetHeadPosition();
		// we have an array - fill it in!
		for (long i = 0; i < cTypeNames; i++)
		{
			curName = m_typeNames.GetNext(pos);
			iPos = curName.ReverseFind('\\');
			strName = curName.Right(curName.GetLength() - iPos - 1);
			strName = strName.Left(strName.GetLength() - 4);

			strName += "|";
			strName += curName;

			bstrName = strName;

			VariantInit(&var);

			var.vt = VT_BSTR;
			var.bstrVal = bstrName.Copy();

			hr = SafeArrayPutElement(pTypeList, &i, &var);
			if (FAILED(hr))
				goto ERROR_OUT; 

		}

		// now, convert it to a VARIANT
		pVal->vt		= VT_VARIANT | VT_ARRAY;
		pVal->parray	= pTypeList;

		return S_OK;
	}

ERROR_OUT:
	if (pTypeList)
        SafeArrayDestroy(pTypeList);

	return E_FAIL;
}

STDMETHODIMP CWorldWizHelper::initialize()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	BSTR bstrPath;
	CString strPath;
	HRESULT hr = S_OK;
	CFileFind finder;
	BOOL	bWorking;

	// First, get the path to the wizards
	hr = FindContentPath(&bstrPath);
	if (FAILED(hr))
		goto failexit;

	strPath = bstrPath;
	if (strPath.GetAt(strPath.GetLength() - 1) == '\\')
		strPath += "SDK\\Wizards\\World\\Kits\\*.htm";
	else
		strPath += "\\SDK\\Wizards\\World\\Kits\\*.htm";

	// walk the directory looking for files of *.htm
	// find the files....
	bWorking = finder.FindFile(strPath);

    while (bWorking)    
	{        
		bWorking = finder.FindNextFile();
        m_typeNames.AddTail(finder.GetFilePath());
	}

failexit:
	return hr;
}
