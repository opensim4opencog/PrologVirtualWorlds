// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWZoomTool.cpp : Implementation of CVWZoomTool
#include "stdafx.h"
#include "vwstudio.h"
#include "ZoomTool.h"
#include <vwrnedit.h>
#include <math.h>
/////////////////////////////////////////////////////////////////////////////
const IID DIID__BoundaryControl = {0x9BF8A469,0x1543,0x11D0,{0x89,0xA9,0x00,0xA0,0xC9,0x05,0x41,0x29}};
const IID DIID__RenderingControl = { 0x44fac781, 0xca4, 0x11d0, { 0x89, 0xa9, 0, 0xa0, 0xc9, 0x5, 0x41, 0x29 } };
#define FLOAT_EQUAL(x,y) ( fabs(x - y) < 0.001)
#define dispidToolEvent 19L
/////////////////////////////////////////////////////////////////////////////
// CVWZoomTool

STDMETHODIMP CVWZoomTool::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IVWZoomTool,
	};
	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

CVWZoomTool::CVWZoomTool()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SetCursor(NULL, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_ZOOMIN)));
	m_nMode = zmZoomIn;
	m_pVWRenderRoot = NULL;
}

BOOL CVWZoomTool::OnLButtonDown( UINT nFlags, POINT pt  )
{
	VARIANT varResult;
	DISPPARAMS dispparams;
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
	HRESULT	hr = S_OK;
	IDispatch* piEditorViewControl = NULL;
	DISPID mydispid = DISPID_PROPERTYPUT;
	DISPID didCenterViewOnPosition, didZoomLevelProperty;
	float fnewVal;

	VariantInit(&varResult);

	hr = m_pUIView->QueryInterface(DIID__RenderingControl, (LPVOID*) &piEditorViewControl);
	if (FAILED(hr) || (piEditorViewControl == NULL)) goto EXIT_FAIL;

	didCenterViewOnPosition = 15;
	didZoomLevelProperty = 0x7;


	//Get the Bounary control's ZoomLevel property 
	piEditorViewControl->Invoke(didZoomLevelProperty, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &dispparamsNoArgs, &varResult, NULL, NULL);

	fnewVal = varResult.fltVal;
	if (m_nMode == zmZoomIn)
	{
		if (FLOAT_EQUAL(fnewVal, 0.33f) )
			fnewVal = 0.5;
		else if (FLOAT_EQUAL(fnewVal, 3.0f) )
			fnewVal = 4.0f;
		else
			fnewVal *= 2;
	}
	else if (m_nMode == zmZoomOut)
	{
		if (FLOAT_EQUAL(fnewVal, 0.33f) )
			fnewVal = 0.25;
		else if (FLOAT_EQUAL(fnewVal, 3.0f) )
			fnewVal = 2.0f;
		else
			fnewVal /= 2;
	}
	else
	{
		OutputDebugString("Invalid zoom tool mode\n");
		ASSERT(0);
	}

	if (fnewVal >= 0.0625f && fnewVal <= 16.0f) 
	{
		hr = InitializeDISPPARAMS(&dispparams, 2);
		if (FAILED(hr)) goto EXIT_FAIL;

		dispparams.rgvarg[1].vt = VT_I2;
		dispparams.rgvarg[1].iVal = (short) pt.x;
		dispparams.rgvarg[0].vt = VT_I2;
		dispparams.rgvarg[0].iVal = (short) pt.y;

		//Call CBoundaryControl::CenterViewOnPosition(short x, short y);
		piEditorViewControl->Invoke(didCenterViewOnPosition, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispparams, &varResult, NULL, NULL);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		VariantClear(&varResult);

		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))	goto EXIT_FAIL;

		dispparams.rgvarg[0].vt = VT_R4;
		dispparams.rgvarg[0].fltVal = fnewVal;
		dispparams.cNamedArgs = 1;
		dispparams.rgdispidNamedArgs = &mydispid;

		//Set the render view control's ZoomLevel property 
		hr = piEditorViewControl->Invoke(didZoomLevelProperty, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYPUT, &dispparams, NULL, NULL, NULL);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		VariantClear(&varResult);

	}
	else
	{
		MessageBeep(-1);
	}

EXIT_FAIL:
	VariantClear(&varResult);
	SAFERELEASE(piEditorViewControl);

	return TRUE;
}

STDMETHODIMP CVWZoomTool::get_Mode(short *pVal)
{
	if (NULL == pVal)
		return E_INVALIDARG;

	*pVal = m_nMode;

	return S_OK;
}

STDMETHODIMP CVWZoomTool::put_Mode(short newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_nMode = newVal;

	if (m_nMode == zmZoomIn)
		SetCursor(m_pUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_ZOOMIN)));
	else
		SetCursor(m_pUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_ZOOMOUT)));

	return S_OK;
}

STDMETHODIMP CVWZoomTool::Initialize(IVWUIView* pVw, IWorld* pWorld, IVWRenderRoot *pVWRenderRoot)
{
	HRESULT hr = S_OK;

	ASSERT(pVw);
	ASSERT(pVWRenderRoot);

	if (m_pUIView != pVw)
	{
		SAFERELEASE(m_pUIView);
		m_pUIView = pVw;
		SAFEADDREF(pVw);
	}

	SAFERELEASE(m_pVWRenderRoot);
	m_pVWRenderRoot = pVWRenderRoot;
	SAFEADDREF(pVWRenderRoot);

	return hr;
}