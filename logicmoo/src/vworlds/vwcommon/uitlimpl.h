// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _CVWUIToolImpl_
#define _CVWUIToolImpl_

#include <atlcom.h>
#include <propbase.h>
#include <vwuiobjs.h>
#include <atlctl.h>

interface IWorld;
interface IPropertyList;
interface IPropertySecurity;

// This template is based on the ObjectProperty template by A.C.

//How to use...
// The Simple way Implementing IFoo, with CFooObject in fobj.h and fobj.cpp
// the atl generated class looks something like:
// class ATL_NO_VTABLE CFooObject : 
//	public CComObjectRootEx<CComSingleThreadModel>,
//	public CComCoClass<CMenuListObject, &CLSID_FooObject>,
//	public IDispatchImpl<IFooObject, &IID_IFooObject, &LIBID_VWSYSTEM>
// {
// public:
// .......
// }
// Change this to
// DECLARE_VWUITOOL( Foo, LIBID_VWSYSTEM ) 
// {
// .....
// }
// This creates a typedef for the parent class CFooObjectBase and adds in all
// the neccessary base classes.  Base class methods can be called via CFooObjectBase::method
// all of the IVWUITool interfaces and various utilities are implemented.
////
// This base class template calls virtual handler methods for each of these WM's:
//		WM_LBUTTONDOWN:
//		WM_LBUTTONUP:
//		WM_LBUTTONDBLCLK:
//		WM_RBUTTONDOWN:
//		WM_RBUTTONUP:
//		WM_RBUTTONDBLCLK:
//		WM_MOUSEMOVE:
//		WM_KEYUP:
//		WM_KEYDOWN:
//		WM_TIMER:
// Override the handlers for any messages you want to respond to.
// All other messages are handled by the OnMessage method.
//
// If you don't have a well formed Foo, you can use the other macros or
// define the class yourself, substituting IVWUIToolImpl for IDispatchImpl 
//  
//

#define DECLARE_VWUITOOL( NAME, LIBID ) \
	DECLARE_VWUITOOLEX(C##NAME##Object, I##NAME, IID_I##NAME, CLSID_##NAME, LIBID)

#define DECLARE_VWUITOOLEX( CLASSNAME, INTERFACENAME, IID_BASE, CLSID_BASE, LIBID ) \
	typedef IVWUIToolImpl<INTERFACENAME, &IID_BASE, &LIBID > CLASSNAME##Base; \
	DECLARE_ATLCLASS(CLASSNAME, CLASSNAME##Base, IID_BASE, CLSID_BASE)

#define DECLARE_ATLCLASS(CLASSNAME, BASE_CLASS, IID_BASE, CLSID_BASE) \
	class ATL_NO_VTABLE CLASSNAME : \
	public BASE_CLASS,\
	public CComObjectRootEx<CComMultiThreadModel>, \
	public CComCoClass<CLASSNAME, &CLSID_BASE>, \
	public IObjectSafetyImpl<CLASSNAME, INTERFACESAFE_FOR_UNTRUSTED_CALLER>, \
	public ISupportErrorInfoImpl<&IID_BASE>

template < class T, const IID* IID_BASE, const GUID* LIBID >
class ATL_NO_VTABLE IVWUIToolImpl : 
	public IDispatchImpl<T, IID_BASE, LIBID>
{
public:
	IVWUIView * m_pUIView;	 
	IPropertyList * m_pSelectionList;  
	IVWControlManager * m_pControlManager;						
	HCURSOR m_hCursor;
	IWorld* m_pWorld;
	
	IVWUIToolImpl() 
	{
		m_pUIView = NULL;	 
		m_pSelectionList = NULL; 
		m_pControlManager = NULL; 
		m_pWorld = NULL;
		m_hCursor = LoadCursor(NULL, IDC_ARROW );
	}

	virtual ~IVWUIToolImpl() 
	{
		Terminate();
	}

	STDMETHOD(Terminate)(void)
	{
		SAFERELEASE( m_pWorld );
		SAFERELEASE( m_pUIView );	 
		SAFERELEASE( m_pSelectionList ); 
		SAFERELEASE( m_pControlManager ); 

		return S_OK;
	}

	STDMETHOD(OnUIEvent)(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient)
	{
		return S_OK;
	};

	STDMETHOD(OnToolChanging)( VARIANT varHint )
	{ 
		return S_OK; 
	};

	STDMETHOD(OnToolActivate)( VARIANT varHint )
	{ 
		return S_OK; 
	};

	STDMETHOD(Update)()
	{ 
		return S_OK; 
	};

	STDMETHOD(SetCursor)( IVWUIView* pVWUIView, HCURSOR hCursor)
	{
		HWND hwnd;
		RECT rect;
		HRESULT hr = S_OK;
		POINT pt;

		m_hCursor = hCursor;

		if (pVWUIView)
		{
			hr = pVWUIView->get_HWND(&hwnd);
			if (FAILED(hr)) return hr;

			GetClientRect(hwnd, &rect);
			GetCursorPos(&pt);
			ScreenToClient(hwnd, &pt);

			if (PtInRect(&rect, pt))
			{
				if (m_hCursor)
					::SetCursor(m_hCursor);
				else
					::SetCursor(LoadCursor(NULL, IDC_ARROW ));
			}
		}
		return S_OK;		
	}

	STDMETHOD(Initialize)( IVWUIView* pVw, IWorld* pWorld, IVWRenderRoot *pVWRenderRoot)
	{
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
		return S_OK;		
	}

	STDMETHOD(OnMessage)(IVWUIView *pVw,unsigned int nIDEvent,unsigned int wParam,long lParam,VARIANT_BOOL*pbHandled)
	{
		HRESULT hr = S_OK;
		BOOL bHandled = FALSE;
		POINT pt;
		pt.x = MAKEPOINTS(lParam).x;
		pt.y = MAKEPOINTS(lParam).y;

		if (!m_pControlManager)
		{
			if (FAILED(hr = pVw->get_ControlManager(&m_pControlManager)))
				goto EXIT_FAIL;
		}

		// A VWUIView doesn't need a ControlManager.
		if(!m_pSelectionList && m_pControlManager != NULL)
		{
			if (FAILED(hr = m_pControlManager->get_SelectionList(&m_pSelectionList)))
				goto EXIT_FAIL;
		}

		if (m_pUIView != pVw)
		{
			SAFERELEASE(m_pUIView);
			m_pUIView = pVw;
			SAFEADDREF(pVw);
		}
		switch(nIDEvent)
		{
		case WM_LBUTTONDOWN:
			bHandled = OnLButtonDown( wParam, pt );
		break;
		case WM_LBUTTONUP:
			bHandled = OnLButtonUp( wParam, pt);
		break;
		case WM_LBUTTONDBLCLK:
			bHandled = OnLButtonDblClk( wParam, pt );
		break;
		case WM_RBUTTONDOWN:
			bHandled = OnRButtonDown( wParam, pt );
		break;
		case WM_RBUTTONUP:
			bHandled = OnRButtonUp( wParam, pt );
		break;
		case WM_RBUTTONDBLCLK:
			bHandled = OnRButtonDblClk( wParam, pt );
		break;
		case WM_MOUSEMOVE:
			bHandled = OnMouseMove( wParam, pt );
		break;
		case WM_KEYUP:
			bHandled = OnKeyUp( wParam, lParam );
		break;
		case WM_KEYDOWN:
			bHandled = OnKeyDown( wParam, lParam );
		break;
		case WM_SETCURSOR:
			bHandled = OnSetCursor( wParam, lParam );
		break;
		case WM_TIMER:
			bHandled = OnTimer( wParam, (TIMERPROC *) lParam );
		break;
		default:
			bHandled = OnMessage( nIDEvent, wParam, lParam );
		}

	EXIT_FAIL:

		*pbHandled = (VARIANT_BOOL)bHandled;
		return hr;
	}

	STDMETHOD(IsValid)(IVWUIView* pVW,VARIANT_BOOL * pB)
	{
		if(NULL == pB)
			return E_POINTER;
		*pB = VARIANT_FALSE;

		return S_OK;
	}
	virtual BOOL OnLButtonDown( UINT nFlags, POINT point ){ return FALSE; };
	virtual BOOL OnLButtonUp( UINT nFlags, POINT point ){ return FALSE; };
	virtual BOOL OnLButtonDblClk( UINT nFlags, POINT point ){ return FALSE; };
	virtual BOOL OnRButtonDown( UINT nFlags, POINT point ){ return FALSE; };
	virtual BOOL OnRButtonUp( UINT nFlags, POINT point ){ return FALSE; };
	virtual BOOL OnRButtonDblClk( UINT nFlags, POINT point ){ return FALSE; };
	virtual BOOL OnMouseMove( UINT nFlags, POINT point ){ return FALSE; };
	virtual BOOL OnKeyDown( UINT nVirtKey, LPARAM lKeyData ){ return FALSE; };
	virtual BOOL OnKeyUp( UINT nVirtKey, LPARAM lKeyData ){ return FALSE; };
	virtual BOOL OnTimer( UINT nIDEvent , TIMERPROC * tmprc){ return FALSE; };
	virtual BOOL OnMessage( UINT nIDEvent, WPARAM wParam, LPARAM lParam ){ return FALSE; };

	virtual BOOL OnSetCursor( WPARAM wParam, LPARAM lParam )
	{ 
		if ( m_pUIView )
		{
			HWND hWnd;

			if (LOWORD(lParam) == HTCLIENT && m_hCursor)
			{
				if (FAILED(m_pUIView->get_HWND(&hWnd)))
					TRACE("Tool failed to get Hwnd in OnSetCursor()\n");

				if ( hWnd == (HWND) wParam )
					::SetCursor(m_hCursor);
				else
					::SetCursor(LoadCursor(NULL, IDC_ARROW ));

				return TRUE;
			}
			else
			{
				::SetCursor(LoadCursor(NULL, IDC_ARROW ));
				return TRUE;
			}
		}
		return FALSE;
	};
};

#endif
