// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// select.cpp : Implementation of CVWSelectToolObject and DLL registration.

#include "stdafx.h"
#include <stdtools.h>
#include "NavBound.h"

typedef CVWComPtr<IObjectProperty,NULL,&IID_IObjectProperty> CObjectPropertyPtr;
/////////////////////////////////////////////////////////////////////////////
const IID DIID__BoundaryControl = {0x9BF8A469,0x1543,0x11D0,{0x89,0xA9,0x00,0xA0,0xC9,0x05,0x41,0x29}};

/////////////////////////////////////////////////////////////////////////////
//

// IVWUITool Methods

// Review: Any reason to implement?

CVWBoundaryNavigateTool::CVWBoundaryNavigateTool()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SetCursor(m_pUIView, LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_NAV)));
	m_piBoundaryViewControl = NULL;
	m_pUserVWFrame = NULL;
	m_pParentVWFrame = NULL;
	m_bUseAvatarPosition = FALSE;
	m_bCapturedCursor = FALSE;
}

BOOL CVWBoundaryNavigateTool::OnTimer( UINT nIDEvent, TIMERPROC * tmprc)
{

	HRESULT		hr = S_OK;
	float		flUserX, flUserY, flUserZ, flDirX, flDirY, flDirZ ;
	BOOL		bDoRefresh = FALSE;   //Review: Should only call InvalidateRect if there's a change
	CPoint		cpClick;
	VARIANT		varResult;
	DISPPARAMS dispparams;;

	VariantInit(&varResult);

	if(m_bUseAvatarPosition)
	{

		//SSync-up the position based on the user's VWFrame position, since that is the only
		//accurate position variable -during- navigation.
		if (FAILED(hr = m_pUserVWFrame->GetPositionEx(m_pParentVWFrame, &flUserX, &flUserY, &flUserZ)))
			goto EXIT_FAIL;

		hr = InitializeDISPPARAMS(&dispparams, 2);
		if (FAILED(hr))
			goto EXIT_FAIL;

		dispparams.rgvarg[1].vt = VT_R4;
		dispparams.rgvarg[1].fltVal = (float) flUserX;
		dispparams.rgvarg[0].vt = VT_R4;
		dispparams.rgvarg[0].fltVal = (float) flUserZ;

		//Call CenterViewOnWorldPosition
		m_piBoundaryViewControl->Invoke(12, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispparams, &varResult, NULL, NULL);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		if (FAILED(hr = m_pUserVWFrame->GetOrientationEx(m_pParentVWFrame, &flDirX, &flDirY, &flDirZ, &flUserX, &flUserY, &flUserZ)))
			goto EXIT_FAIL;

		hr = InitializeDISPPARAMS(&dispparams, 3);
		if (FAILED(hr))
			goto EXIT_FAIL;

		dispparams.rgvarg[2].vt = VT_R4;
		dispparams.rgvarg[2].fltVal = (float) flDirX;
		dispparams.rgvarg[1].vt = VT_R4;
		dispparams.rgvarg[1].fltVal = (float) flDirY;
		dispparams.rgvarg[0].vt = VT_R4;
		dispparams.rgvarg[0].fltVal = (float) flDirZ;

		//Call UpdateCameraDirection
		m_piBoundaryViewControl->Invoke(13, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispparams, &varResult, NULL, NULL);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		bDoRefresh = TRUE;
	}
	else  //Camera is detached from the avatar
	{
		GetCursorPos(&cpClick);

		hr = InitializeDISPPARAMS(&dispparams, 2);
		if (FAILED(hr))
			goto EXIT_FAIL;
		
		dispparams.rgvarg[1].vt = VT_I2;
		dispparams.rgvarg[1].iVal = (short) (cpClick.x - m_cpLastClick.x) / 16;
		dispparams.rgvarg[0].vt = VT_I2;
		dispparams.rgvarg[0].iVal = (short) (cpClick.y - m_cpLastClick.y) / 16;

		//Call MoveViewingPosition
		m_piBoundaryViewControl->Invoke(10, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispparams, &varResult, NULL, NULL);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		bDoRefresh = TRUE;
	}

//	if (bDoRefresh)
//		m_pUIView->InvalidateRect(NULL);


EXIT_FAIL:
	VariantClear(&varResult);

	return FALSE;




}


BOOL CVWBoundaryNavigateTool::OnLButtonDown( UINT nFlags, POINT pt  )
{
	HRESULT hr = S_OK;
	IThing* pUsersLocation = NULL;
	IWorld* pWorld = NULL;	
	IThing * pUserThing = NULL;
	VARIANT var;
	HWND hWnd;

	SAFERELEASE(m_pUserVWFrame);
	SAFERELEASE(m_pParentVWFrame);

	VariantInit(&var);

	SAFERELEASE(m_piBoundaryViewControl); //(IDispatch*&)
	hr = m_pUIView->QueryInterface(DIID__BoundaryControl, (LPVOID*) &m_piBoundaryViewControl);
	if(FAILED(hr)) goto EXIT_FAIL;

	hr = m_pUIView->get_TargetObjectProperty((IObjectProperty**) &pUsersLocation);
	if(FAILED(hr) || !pUsersLocation) goto EXIT_FAIL;

	hr = pUsersLocation->get_World(&pWorld);
	if(FAILED(hr) || !pWorld) goto EXIT_FAIL;

	hr = pWorld->get_User(&pUserThing);
	if(FAILED(hr)) goto EXIT_FAIL;

	if (FAILED(hr = pUserThing->InvokeMethodExt(CComBSTR("GetLoadedGeometry"), NULL, &var)))
		goto EXIT_FAIL;

	if (var.vt != VT_DISPATCH || var.pdispVal == NULL)
	{
		// Getting the location's geometry should have gotten the rendering context's geometry.
		hr = E_FAIL;
		goto EXIT_FAIL;
	}

	m_pUserVWFrame = (IVWFrame*)var.pdispVal;
	m_pUserVWFrame->AddRef();

	if (FAILED(hr = m_pUserVWFrame->GetParent(&m_pParentVWFrame)) || !m_pParentVWFrame)
		goto EXIT_FAIL;

	hr = m_pUIView->get_HWND(&hWnd);

	m_cpLastClick.x = pt.x;
	m_cpLastClick.y = pt.y;

	ClientToScreen(hWnd, &m_cpLastClick);

	if (!m_bUseAvatarPosition)
	{
		m_bCapturedCursor = TRUE;
		SetCapture(hWnd);
	}

	//Create the timer to simulate velocity.
	m_nEventID = SetTimer( hWnd, 1, 250, NULL);

EXIT_FAIL:
	VariantClear(&var);
	SAFERELEASE(pUserThing);
	SAFERELEASE(pWorld);
	SAFERELEASE(pUsersLocation);

	return FALSE;
}

BOOL CVWBoundaryNavigateTool::OnLButtonUp( UINT nFlags, POINT pt  )
{
	HWND hWnd;

	m_pUIView->get_HWND(&hWnd);
	
	if (!KillTimer(hWnd, m_nEventID))
		TRACE("CVWBoundaryNavigateTool: Could not kill timer.\n");

	if (m_bCapturedCursor)
	{
		m_bCapturedCursor = FALSE;
		ReleaseCapture();
	}

	SAFERELEASE(m_pUserVWFrame);
	SAFERELEASE(m_pParentVWFrame);
	SAFERELEASE(m_piBoundaryViewControl); //(IDispatch*&)

	return FALSE;
}


BOOL CVWBoundaryNavigateTool::OnMouseMove( UINT nFlags, POINT pt  )
{
	HRESULT	hr = S_OK;

/*
	VARIANT varResult;
	DISPPARAMS dispparams;;

	if( m_bMovesView && (MK_LBUTTON & nFlags) && NULL != m_piBoundaryViewControl )
	{

		VariantInit(&varResult);

		hr = InitializeDISPPARAMS(&dispparams, 2);
		if (FAILED(hr))
			goto CLEAN_UP;

		dispparams.rgvarg[1].vt = VT_R4;
		dispparams.rgvarg[1].fltVal = (float) (m_pPoint.x - pt.x);
		dispparams.rgvarg[0].vt = VT_R4;
		dispparams.rgvarg[0].fltVal = (float) (m_pPoint.y - pt.y);

		m_piBoundaryViewControl->Invoke(0xa, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispparams, &varResult, NULL, NULL);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		m_pPoint = pt;
	}

CLEAN_UP:
	VariantClear(&varResult);
*/

	return TRUE;
}


STDMETHODIMP CVWBoundaryNavigateTool::IsValid(IVWUIView * pVw, VARIANT_BOOL * pfValid)
{
	ASSERT( pfValid );
	if (!pfValid )
		return E_POINTER;

	*pfValid = TRUE; // For now.
	return S_OK;
}


STDMETHODIMP CVWBoundaryNavigateTool::get_UseAvatarPosition(VARIANT_BOOL *pVal)
{
	ASSERT(pVal);

	if (m_bUseAvatarPosition)
		*pVal = VARIANT_TRUE;
	else
		*pVal = VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CVWBoundaryNavigateTool::put_UseAvatarPosition(VARIANT_BOOL newVal)
{
	m_bUseAvatarPosition = (newVal == VARIANT_TRUE ? TRUE : FALSE);
	
	return S_OK;
}

