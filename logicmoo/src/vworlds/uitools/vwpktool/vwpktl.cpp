// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWPkTl.cpp : Implementation of CvwpktoolApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include <d3drmwin.h>
#include <vwpktool.h>
#include <menuitem.h>
#include "VWPkTl.h"
#include <vwnvtool.h>

//#include <menuitem_i.c>

/////////////////////////////////////////////////////////////////////////////
//

CVWPickTool::CVWPickTool() 
{
	m_pRender = NULL;
	m_pntLButtonDown = CPoint(-1,-1);

	// package up args into DISPPARAMS struct
	InitializeDISPPARAMS(&m_dispparams, 3);
}

CVWPickTool::~CVWPickTool()
{
	Terminate();
}

STDMETHODIMP CVWPickTool::Terminate(void)
{
	CVWPickToolBase::Terminate();

	TerminateDISPPARAMS(VARIANT_FALSE, &m_dispparams);

	SAFERELEASE(m_pRender);

	return S_OK;
}

BOOL CVWPickTool::OnLButtonDown( unsigned int nFlags, POINT point )
{
	HRESULT hr = S_OK;
	IThing *pThing = NULL;
	BOOL bHandled = FALSE;

	m_pntLButtonDown = point;

	if (SUCCEEDED(HitTest(point, VARIANT_FALSE, &pThing)) && pThing != NULL && m_pUIView != NULL ) 
	{
		COleVariant varResult;
		HWND		hWnd;

		m_pUIView->get_HWND(&hWnd);
		m_dispparams.rgvarg[2].vt = VT_I4;
		m_dispparams.rgvarg[2].lVal = (long) hWnd;
		m_dispparams.rgvarg[1].vt = VT_I4;
		m_dispparams.rgvarg[1].lVal = point.x;
		m_dispparams.rgvarg[0].vt = VT_I4;
		m_dispparams.rgvarg[0].lVal = point.y;

		if (SUCCEEDED(hr = pThing->InvokeMethodExt(CComBSTR("OnLButtonDown"), &m_dispparams, (LPVARIANT)varResult)))
			bHandled = TRUE;

		SAFERELEASE(pThing);
	}
	return bHandled;
}

BOOL CVWPickTool::OnLButtonDblClk(unsigned int nFlags, POINT point )
{
	HRESULT hr;
	IThing *pThing = NULL;
	BOOL bHandled = FALSE;

	// Review: Set bHandled?
	// see if anything underneath
	if (SUCCEEDED(HitTest( point, VARIANT_FALSE, &pThing)) && pThing != NULL && m_pUIView != NULL ) 
	{
		COleVariant varResult;
		HWND		hWnd;

		m_pUIView->get_HWND(&hWnd);
		m_dispparams.rgvarg[2].vt = VT_I4;
		m_dispparams.rgvarg[2].lVal = (long) hWnd;

		m_dispparams.rgvarg[1].vt = VT_I4;
		m_dispparams.rgvarg[1].lVal = point.x;

		m_dispparams.rgvarg[0].vt = VT_I4;
		m_dispparams.rgvarg[0].lVal = point.y;

		if (SUCCEEDED(hr = pThing->InvokeMethodExt(CComBSTR("OnLButtonDblClk"), &m_dispparams, (LPVARIANT)varResult)))
			bHandled = TRUE;

		SAFERELEASE(pThing);
	}

	return bHandled;
}

BOOL CVWPickTool::OnLButtonUp(unsigned int nFlags, POINT point )
{
	HRESULT hr;
	IThing *pThing = NULL;
	BOOL bHandled = FALSE;

	// Only really an LButtonUp if not navigated in interim.
	if (m_pntLButtonDown.x != -1 && m_pntLButtonDown.y != -1 && 
		abs(point.x - m_pntLButtonDown.x) <= nPixelsOfStillArea &&
		abs(point.y - m_pntLButtonDown.y) <= nPixelsOfStillArea &&
		SUCCEEDED(HitTest( point, VARIANT_FALSE, &pThing)) && pThing != NULL && m_pUIView != NULL )
	{
		COleVariant varResult;
		HWND		hWnd;

		m_pUIView->get_HWND(&hWnd);
		m_dispparams.rgvarg[2].vt = VT_I4;
		m_dispparams.rgvarg[2].lVal = (long) hWnd;
		m_dispparams.rgvarg[1].vt = VT_I4;
		m_dispparams.rgvarg[1].lVal = point.x;
		m_dispparams.rgvarg[0].vt = VT_I4;
		m_dispparams.rgvarg[0].lVal = point.y;

		if (SUCCEEDED(hr = pThing->InvokeMethodExt(CComBSTR("OnLButtonUp"), &m_dispparams, (LPVARIANT)varResult)))
			bHandled = TRUE;

		SAFERELEASE(pThing);
	}

	m_pntLButtonDown = CPoint(-1,-1);

	return bHandled;
}

BOOL CVWPickTool::OnRButtonDown( unsigned int nFlags, POINT point )
{
	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;
	IThing *pThing = NULL;
	

	if (SUCCEEDED(HitTest( point, VARIANT_FALSE, &pThing)) && pThing != NULL && m_pUIView != NULL) 
	{
		COleVariant varResult;
		HWND		hWnd;

		m_pUIView->get_HWND(&hWnd);
		m_dispparams.rgvarg[2].vt = VT_I4;
		m_dispparams.rgvarg[2].lVal = (long) hWnd;
		m_dispparams.rgvarg[1].vt = VT_I4;
		m_dispparams.rgvarg[1].lVal = point.x;
		m_dispparams.rgvarg[0].vt = VT_I4;
		m_dispparams.rgvarg[0].lVal = point.y;

		if (SUCCEEDED(hr = pThing->InvokeMethodExt(CComBSTR("OnRButtonDown"), &m_dispparams, (LPVARIANT)varResult)))
			bHandled = TRUE;

		SAFERELEASE(pThing);
	}

	return bHandled;
}

BOOL CVWPickTool::OnRButtonDblClk(unsigned int nFlags, POINT point )
{
	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;
	IThing *pThing = NULL;

	if (SUCCEEDED(HitTest( point, VARIANT_FALSE, &pThing)) && pThing != NULL && m_pUIView != NULL ) 
	{
		COleVariant varResult;
		HWND		hWnd;

		m_pUIView->get_HWND(&hWnd);
		m_dispparams.rgvarg[2].vt = VT_I4;
		m_dispparams.rgvarg[2].lVal = (long) hWnd;
		m_dispparams.rgvarg[1].vt = VT_I4;
		m_dispparams.rgvarg[1].lVal = point.x;
		m_dispparams.rgvarg[0].vt = VT_I4;
		m_dispparams.rgvarg[0].lVal = point.y;

		if (SUCCEEDED(hr = pThing->InvokeMethodExt(CComBSTR("OnRButtonDblClk"), &m_dispparams, (LPVARIANT)varResult)))
			bHandled = TRUE;

		SAFERELEASE(pThing);
	}

	return bHandled;
}

BOOL CVWPickTool::OnRButtonUp(unsigned int nFlags, POINT point )
{
	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;
	IThing *pThing = NULL;
	IDispatch *pdisp = NULL;
	IPropertyList* plistMenu = NULL;
	VARIANT_BOOL	bIsUIVisible;
	static CComBSTR	bstrIsUIVisible("IsUIVisible");

	// see if anything underneath
	if (SUCCEEDED(HitTest( point, VARIANT_FALSE, &pThing)) && pThing != NULL && m_pUIView != NULL ) 
	{
		COleVariant varResult;
		HWND		hWnd;

		m_pUIView->get_HWND(&hWnd);
		m_dispparams.rgvarg[2].vt = VT_I4;
		m_dispparams.rgvarg[2].lVal = (long) hWnd;
		m_dispparams.rgvarg[1].vt = VT_I4;
		m_dispparams.rgvarg[1].lVal = point.x;
		m_dispparams.rgvarg[0].vt = VT_I4;
		m_dispparams.rgvarg[0].lVal = point.y;

		if (SUCCEEDED(pThing->InvokeMethodExt(CComBSTR("OnRButtonUp"), &m_dispparams, (LPVARIANT)varResult)))
			bHandled = TRUE;
		else
		{		
			// No right click menus for objects which are not UI visible.
			if (FAILED(hr = pThing->get_BOOL(bstrIsUIVisible, &bIsUIVisible)) || !bIsUIVisible)
				goto ERROR_CONDITION;

			// get the menu
			hr = pThing->InvokeMethodExt(CComBSTR("GetMenuList"), NULL, (LPVARIANT)varResult);

			// REVIEW: Treat "method not found as ok" - other errors as not ok
			if (SUCCEEDED(hr))
			{
				// display menu with the property list
				if (varResult.vt != VT_DISPATCH)
				{
					hr = E_FAIL;
					goto ERROR_CONDITION;
				}
				// NOTE: don't release pdisp later since varResult will automatically
				// release when it goes out of scope!
				pdisp = varResult.pdispVal;
				hr = pdisp->QueryInterface(IID_IPropertyList, (void **) &plistMenu);
				if (FAILED(hr))
					goto ERROR_CONDITION;

				// Bring up the popup menu
				hr = DisplayPopup(hWnd, point, plistMenu );
				if (FAILED(hr))
					goto ERROR_CONDITION;
			}
		}

ERROR_CONDITION:
		SAFERELEASE(pThing);
		SAFERELEASE(plistMenu);
	}

	return bHandled;
}

BOOL CVWPickTool::OnMouseMove( unsigned int nFlags, POINT point )
{
	return FALSE;
}

BOOL CVWPickTool::OnTimer( unsigned int nIDEvent, TIMERPROC * tmprc  )
{
	
	BOOL bHandled = FALSE;

	return bHandled;
}

BOOL CVWPickTool::OnMessage(unsigned int message, WPARAM wParam, LPARAM lParam )
{
	// default
	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;

	if (message == WM_COMMAND)
	{
		if (LOWORD(wParam) >= WM_USER && LOWORD(wParam) <= WM_USER+200)
		{
		    if (HIWORD(wParam) == (WORD)CN_UPDATE_COMMAND_UI)
			{
	            if (m_Menu.OnCommandRangeUI((CCmdUI*)lParam, &hr ))
				    bHandled = TRUE;
			} 
		    else
			{
	            if (m_Menu.OnCommandRange( wParam, &hr ))
				    bHandled = TRUE;
			}
		}
	}
	return bHandled;
}

STDMETHODIMP CVWPickTool::IsValid(IVWUIView *pVw,VARIANT_BOOL * pfValid)
{
	return E_NOTIMPL;
}

STDMETHODIMP CVWPickTool::Initialize(IVWUIView* pVw, IWorld* pWorld, IVWRenderRoot *pVWRenderRoot)
{
	if (!pVWRenderRoot) return E_FAIL;

	if (m_pUIView != pVw)
	{
		SAFERELEASE(m_pUIView);
		m_pUIView = pVw;
		SAFEADDREF(pVw);
	}

	if (m_pWorld != pWorld)
	{
		SAFERELEASE(m_pWorld);
		m_pWorld = pWorld;
		SAFEADDREF(pWorld);
	}

	if (m_pRender != pVWRenderRoot)
	{
		SAFERELEASE(m_pRender);
		m_pRender = pVWRenderRoot;
		SAFEADDREF(pVWRenderRoot);
	}
	return S_OK;
}

HRESULT CVWPickTool::DisplayPopup(HWND hWnd, POINT pt, IPropertyList *pListMenu )
{
	HRESULT		hr = S_OK;
	WPARAM		wID = WM_USER;

	// build the menu
	pListMenu->AddRef();
	hr = m_Menu.CreatePopupFromPropList(pListMenu, &wID);
	if (FAILED(hr))
		goto ERROR_CONDITION;

	// okay, now display it
	CWnd *pwndParent;
	
	// get CWnd * for parent
	pwndParent = CWnd::FromHandle(hWnd);
	ASSERT(pwndParent != NULL);

	// munge point
	pwndParent->ClientToScreen( &pt );

	// bring up menu
	VERIFY(m_Menu.TrackPopupMenu(TPM_CENTERALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, pwndParent));

	// Peek WM_COMMAND from users menu selection off of queue and dispatch it. This
	// way we get to handle the users command right here (via WM_COMMAND handler) 
	// and can destroy menu after.
	MSG msg;
	if (::PeekMessage(&msg, hWnd, WM_COMMAND, WM_COMMAND, PM_REMOVE))
	{
		// yep, process right now
		::DispatchMessage(&msg);
	}

	m_Menu.DestroyMenu();

ERROR_CONDITION:

	// Nuke menu item list
	SAFERELEASE(pListMenu);

	return hr;
}

HRESULT CVWPickTool::HitTest( POINT point, VARIANT_BOOL vbReturnAllPicked, IThing **ppThing)
{
	HRESULT hr = S_OK;
	IVWFrame *pvwFrame = NULL;
	IUnknown *punk = NULL;

	// initialize
	*ppThing = NULL;

	// call the renderer's hittest function to see what's at the point

	// bundle x and y into varParms
	// REVIEW: Hack: Better to pass args explicitly than to bundle into a VAR

	COleVariant varParms;
	COleVariant varHit;
	DWORD dw=MAKELONG(point.x, point.y);
	varParms = (long)dw;
	hr = m_pRender->HitTest(varParms, (vbReturnAllPicked == VARIANT_TRUE ? HITTEST_RETURNALLPICKED : 0), (LPVARIANT)varHit);
	if (FAILED(hr))
		goto ERROR_CONDITION;

	// if nothing, then return success (but empty)
	if (varHit.vt == VT_EMPTY)
		goto ERROR_CONDITION;

	// assure it's at least an IDispatch
	if (varHit.vt != VT_DISPATCH) {

		ASSERT(FALSE);
		return E_FAIL;
	}

	// assure that it's a VWFrame
	punk = varHit.punkVal;
	hr = punk->QueryInterface(IID_IVWFrame, (void **) &pvwFrame);
	if (FAILED(hr))
		goto ERROR_CONDITION;

	pvwFrame->get_Thing(ppThing);

ERROR_CONDITION:

	SAFERELEASE(pvwFrame);

	return hr;
}

