// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWUIView.cpp : implementation file
//

#include <afxctl.h>         // MFC support for OLE Controls
#include "viewctl.h"
#include <atlbase.h>
#include "propbase.h"
#include "vwuiview.h"
#include "vwuiobjs.h"
#include <vwuiobjs_i.c>
#include "syshelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;


IMPLEMENT_DYNCREATE(CVWUIView, CVWViewCtrl)

#define IID_DEFINED
#ifdef USE_CONTROLMANAGER
#include "stdtools_i.c"
#include <vwstudio.h>
//#include "vwstudio_i.c"
#endif
//#include "vwsystem_i.c"

//typedef CVWComPtr<IObjectProperty,NULL,&IID_IObjectProperty> CObjectPropertyPtr;

// {ECD75340-8983-11D0-89A9-00A0C9054129}
static const IID IID_IUIVwBase =
{ 0xecd75340, 0x8983, 0x11d0, { 0x89, 0xa9, 0x0, 0xa0, 0xc9, 0x5, 0x41, 0x29 } };

#ifdef _VWSTANDALONE_CLASS_
Generate error
// IMPLEMENT_OLECTLTYPE(CVWUIView, IDS_VWVIEW, _dwVWViewOleMisc)
#else
UINT CVWUIView::GetUserTypeNameID() { ASSERT(FALSE); return 0; }
DWORD CVWUIView::GetMiscStatus() { ASSERT(FALSE); return 0; }
#endif

/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CVWUIView, 1)
#ifdef _VWSTANDALONE_CLASS_
	PROPPAGEID(CVWViewPropPage::guid)
#else
	//PROPPAGEID(NULL)
#endif
END_PROPPAGEIDS(CVWUIView)

/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CVWUIView, "VWUIVIEW.VWUIView.1",
	0x812e6303, 0xc8c, 0x11d0, 0x8a, 0x96, 0, 0xa0, 0xc9, 0x8, 0x25, 0x83)

/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CVWUIView, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// CVWViewCtrl::CVWViewCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CVWViewCtrl


BOOL CVWUIView::CVWUIViewFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegInsertable | afxRegApartmentThreading to afxRegInsertable.

#ifndef _VWSTANDALONE_CLASS_
	return TRUE;
#else

	if (bRegister)
	{
		HRESULT hr = CreateComponentCategory(CATID_SafeForScripting, 
			L"Controls that are safely scriptable");
		if (SUCCEEDED(hr))
			RegisterCLSIDInCategory(m_clsid, CATID_SafeForScripting);

		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_VWVIEW,
			IDB_VWVIEW,
			afxRegInsertable | afxRegApartmentThreading,
			_dwVWViewOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	}
	else
	{
		HRESULT hRes = S_OK;

		hRes = UnRegisterCLSIDInCategory(m_clsid, CATID_SafeForScripting);

		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CVWUIView

CVWUIView::CVWUIView()
{
	EnableAutomation();

	m_pControlManager = NULL;			
	m_pTargetObjectProperty = NULL;
	m_pVWUIView = NULL;
	m_pVWExecuteUndo = NULL;
	m_pmapLoadedUITools = NULL;
	m_plistCurrentUITools = NULL;
	m_pfnSuper = NULL;
	InitializeCriticalSection(&m_CS);
}

CVWUIView::~CVWUIView()
{
	DeleteCriticalSection(&m_CS);
}

void CVWUIView::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CVWViewCtrl::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CVWUIView, CVWViewCtrl)
	//{{AFX_MSG_MAP(CVWUIView)
//	ON_WM_LBUTTONDOWN()
//	ON_WM_LBUTTONDBLCLK()
//	ON_WM_MOUSEMOVE()
//	ON_WM_LBUTTONUP()
//	ON_WM_KEYDOWN()
//	ON_WM_KEYUP()
	ON_WM_CREATE()
//	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CVWUIView, CVWViewCtrl)
	//{{AFX_DISPATCH_MAP(CVWUIView)
	DISP_PROPERTY_EX(CVWUIView, "TargetObjectProperty", GetTargetObjectProperty, SetTargetObjectProperty, VT_DISPATCH)
	DISP_PROPERTY_EX(CVWUIView, "ControlManager", GetControlManager, SetControlManager, VT_DISPATCH)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


BEGIN_INTERFACE_MAP(CVWUIView, CVWViewCtrl)												
//	INTERFACE_PART(CVWUIView, IID_IUIVwBase, Dispatch)
	INTERFACE_PART(CVWUIView, IID_IVWUIView, VWUIView)
	INTERFACE_PART(CVWUIView, IID_IVWUIEvents, VWUIEvents)
	INTERFACE_PART(CVWUIView, IID_IVWExecuteUndo, VWExecuteUndo)
END_INTERFACE_MAP()																
////////////////////////////////////////////////////////////////////////////
// CVWUIView message handlers


STDMETHODIMP_(ULONG) CVWUIView::XVWUIView::AddRef()									
{ 
	METHOD_PROLOGUE_EX_(CVWUIView, VWUIView)											
	return (ULONG)pThis->ExternalAddRef();
}
											
STDMETHODIMP_(ULONG) CVWUIView::XVWUIView::Release()								
{	
	METHOD_PROLOGUE_EX_(CVWUIView, VWUIView)										
	return (ULONG)pThis->ExternalRelease();
}
										
STDMETHODIMP CVWUIView::XVWUIView::QueryInterface(									
	REFIID iid, LPVOID* ppvObj) 
{													
	METHOD_PROLOGUE_EX_(CVWUIView, VWUIView)										
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj); 
}
					
STDMETHODIMP CVWUIView::XVWUIView::GetTypeInfoCount(unsigned int * uC )				
{ 	
	METHOD_PROLOGUE_EX_(CVWUIView, VWUIView);										
	*uC = pThis->m_CDelegate.GetTypeInfoCount(  ); 
	return S_OK; 
}
					
STDMETHODIMP CVWUIView::XVWUIView::Invoke(DISPID	dispidMember,					
										REFIID	riid,								
										LCID	 lcid,								
										WORD	wFlags,								
										DISPPARAMS FAR* pdispparams,				
										VARIANT FAR* pvarResult,					
										EXCEPINFO FAR* pexcepinfo,					
										unsigned int FAR* puArgErr )				
{ 	
	METHOD_PROLOGUE_EX_(CVWUIView, VWUIView);										
	return pThis->m_CDelegate.GetIDispatch(FALSE)->Invoke(dispidMember, riid, lcid,	
							wFlags,pdispparams, pvarResult, pexcepinfo,puArgErr); 
}	

STDMETHODIMP CVWUIView::XVWUIView::GetIDsOfNames(REFIID riid, 						
								OLECHAR FAR* FAR* rgszNames,						
								unsigned int	cNames,								
								LCID	lcid,										
								DISPID FAR* rgdispid	)							
{ 	
	METHOD_PROLOGUE_EX_(CVWUIView, VWUIView);										
	return pThis->m_CDelegate.GetIDispatch(FALSE)->GetIDsOfNames(riid, 				
								rgszNames, cNames, lcid, rgdispid); 
}
				
STDMETHODIMP CVWUIView::XVWUIView::GetTypeInfo(unsigned int	 itinfo,				
												LCID	 lcid,						
												ITypeInfo FAR* FAR* pptinfo		)	
{ 	
	METHOD_PROLOGUE_EX_(CVWUIView, VWUIView);										
	return pThis->m_CDelegate.GetIDispatch(FALSE)->GetTypeInfo(itinfo, lcid,		
												pptinfo); 
}							

STDMETHODIMP CVWUIView::XVWUIView::get_TargetObjectProperty(struct IObjectProperty * * ppOP)	 
{ 	
	METHOD_PROLOGUE_EX_(CVWUIView, VWUIView);											 
	HRESULT hr = S_OK;																	 

	ASSERT( ppOP );																		 
	if( !ppOP )																			 
		return E_POINTER;																 
	*ppOP = (IObjectProperty*)pThis->GetTargetObjectProperty();												 
	SAFEADDREF( *ppOP );																 

	return hr; 																			 
}																						 
STDMETHODIMP CVWUIView::XVWUIView::put_TargetObjectProperty(struct IObjectProperty * pOP)		 
{ 	
	METHOD_PROLOGUE_EX_(CVWUIView, VWUIView);											 
	HRESULT hr = S_OK;	
	pThis->SetTargetObjectProperty( pOP );

	return hr; 																			 
}																						 

STDMETHODIMP CVWUIView::XVWUIView::get_ControlManager(						
				struct IVWControlManager * *ppCM)									
{ 	
	METHOD_PROLOGUE_EX_(CVWUIView, VWUIView);									
	HRESULT hr = S_OK;															
	ASSERT( ppCM );																
	if( !ppCM ) return E_POINTER;												
	*ppCM = pThis->m_pControlManager;											
	SAFEADDREF( *ppCM );														

	return hr; 																	
}

STDMETHODIMP CVWUIView::XVWUIView::get_HWND(HWND * pHWND)				
{ 	
	METHOD_PROLOGUE_EX_(CVWUIView, VWUIView);								
	*pHWND = pThis->GetSafeHwnd();											

	return S_OK; 															
}

STDMETHODIMP CVWUIView::XVWUIView::AddCurrentUITool(BSTR bstrName)
{
	METHOD_PROLOGUE(CVWUIView, VWUIView)
	CComVariant var;
	HRESULT hr = S_OK;
	VARIANT_BOOL fValid = FALSE;
	CComBSTR bstrEmpty = "";
	long nCount;
	int i;
	
	EnterCriticalSection(&pThis->m_CS);

	var.Clear();
	// Is control initialized?
	if ( ( NULL == pThis->m_plistCurrentUITools ) ||
		 ( NULL == pThis->m_pmapLoadedUITools ) )
	{
		hr = VWSTUDIO_E_BADCONTROLINITIALIZATION;
		goto EXIT_FAIL;
	}

	hr = pThis->m_plistCurrentUITools->get_Count(&nCount);
	if(FAILED(hr)) goto EXIT_FAIL;

	for (i = 0; i < nCount; i++)
	{
		CComVariant varUITool; 

		hr = pThis->m_plistCurrentUITools->get_Property(i, &varUITool);
		if( FAILED(hr) ) goto EXIT_FAIL; 

		hr = ((IVWUITool*)varUITool.pdispVal)->OnToolChanging(var);
		if( (hr != E_NOTIMPL ) && FAILED(hr))
			goto EXIT_FAIL;
	}

	// If bstrName == NULL or empty string, remove all CurrentUITools
	if( ( bstrName == NULL ) || ( CompareElements(&bstrName, &bstrEmpty.m_str) ) )
	{
		pThis->m_plistCurrentUITools->RemoveAll();
		goto EXIT_FAIL; // Not a failure.. but exit now
	}

	// Do we have a LoadedUITool of this name?
	hr = pThis->m_pmapLoadedUITools->IsValid(bstrName, &fValid);
	if(FAILED(hr))
	{
		hr = VWSTUDIO_E_INVALIDLOADEDUITOOLNAME;
		goto EXIT_FAIL;
	}
	
	if (VARIANT_TRUE != fValid)
	{ 
		// No LoadUITool of that name.
		hr = VWSTUDIO_E_INVALIDLOADEDUITOOLNAME;
		goto EXIT_FAIL;
	}

	// Grab the Tool from the LoadedUITools and add it to the front CurrentUITools
	hr = pThis->m_pmapLoadedUITools->get_Property(bstrName, &var);
	if(FAILED(hr)) goto EXIT_FAIL;

	if (var.vt != VT_DISPATCH)
	{
		hr = E_FAIL;	// Review: Error msg.
		goto EXIT_FAIL;
	}

	hr = pThis->m_plistCurrentUITools->get_Count(&nCount);
	if(FAILED(hr)) goto EXIT_FAIL;

	// Insert new tool to CurrentUIToolList
	hr = pThis->m_plistCurrentUITools->InsertAt( nCount, var );
	if(FAILED(hr)) goto EXIT_FAIL;

	hr = ((IVWUITool*)var.pdispVal)->OnToolActivate( CComVariant(0) );
	if( (hr != E_NOTIMPL ) && FAILED(hr))
		goto EXIT_FAIL;

EXIT_FAIL:

	LeaveCriticalSection(&pThis->m_CS);

	return hr;
}

STDMETHODIMP CVWUIView::XVWUIView::LoadUIToolByProgID(BSTR bstrProgID,BSTR bstrName)
{

	METHOD_PROLOGUE(CVWUIView, VWUIView)
	HRESULT hr = S_OK;
	IUnknown * punkUITool = NULL;
	IVWUITool * puitlTemp = NULL;
	VARIANT_BOOL	vBool = VARIANT_TRUE;

	ASSERT(pThis->m_pWorld);
	if( NULL == pThis->m_pmapLoadedUITools )
	{
		hr = VWSTUDIO_E_BADCONTROLINITIALIZATION;
		goto EXIT_FAIL;
	}

	hr = pThis->m_pWorld->IsValidTool(bstrName, &vBool);
	if (FAILED(hr))
		goto EXIT_FAIL;

	if (vBool == VARIANT_FALSE)
		hr = pThis->m_pWorld->CreateTool(bstrName, bstrProgID, &punkUITool);
	else
		hr = pThis->m_pWorld->get_Tool(bstrName, &punkUITool);

	if(FAILED(hr)) goto EXIT_FAIL;

	// Test to see that it is a UITool
	hr = punkUITool->QueryInterface(IID_IVWUITool,(LPVOID*)&puitlTemp);
	if(FAILED(hr))  goto EXIT_FAIL;

	// Add it to LoadedUITools map.
	hr = pThis->m_pmapLoadedUITools->put_Dispatch( bstrName, (IDispatch*)puitlTemp );
	if( FAILED(hr)) goto EXIT_FAIL;

EXIT_FAIL:

	SAFERELEASE( punkUITool );
	SAFERELEASE( puitlTemp );

	return hr;
}

STDMETHODIMP CVWUIView::XVWUIView::LoadUIToolByCLSID(REFCLSID clsid,BSTR bstrName)
{
	METHOD_PROLOGUE(CVWUIView, VWUIView)
	HRESULT hr = S_OK;
	IUnknown * punkUITool = NULL;
	IVWUITool * puitlTemp = NULL;
	VARIANT_BOOL vBool = VARIANT_FALSE;

	ASSERT(pThis->m_pWorld);
	if( NULL == pThis->m_pmapLoadedUITools )
	{
		hr = VWSTUDIO_E_BADCONTROLINITIALIZATION;
		goto EXIT_FAIL;
	}
	
	hr = pThis->m_pWorld->IsValidTool(bstrName, &vBool);
	if (FAILED(hr)) goto EXIT_FAIL;

	if (vBool == VARIANT_FALSE)
		hr = pThis->m_pWorld->CreateToolExt(bstrName, clsid, &punkUITool);
	else
		hr = pThis->m_pWorld->get_Tool(bstrName,&punkUITool);

	if(FAILED(hr)) goto EXIT_FAIL;

	// Test to see that it is a UITool
	hr = punkUITool->QueryInterface(IID_IVWUITool,(LPVOID*)&puitlTemp);
	if(FAILED(hr))  goto EXIT_FAIL;

	hr = pThis->m_pmapLoadedUITools->put_Dispatch( bstrName, (IDispatch*)puitlTemp );
	if( FAILED(hr)) goto EXIT_FAIL;

EXIT_FAIL:

	SAFERELEASE( punkUITool );
	SAFERELEASE( puitlTemp );
	return hr;
}


STDMETHODIMP CVWUIView::XVWUIView::get_CurrentUIToolList(IPropertyList * * ppuitl)
{
	METHOD_PROLOGUE(CVWUIView, VWUIView)

	if(NULL == ppuitl)
		return E_POINTER;

	*ppuitl = pThis->m_plistCurrentUITools;
	SAFEADDREF(*ppuitl);

	return S_OK;
}

STDMETHODIMP CVWUIView::XVWUIView::get_LoadedUIToolMap(IPropertyMap * * ppMap)
{
	METHOD_PROLOGUE(CVWUIView, VWUIView)

	if(NULL == ppMap)
		return E_POINTER;

	*ppMap = pThis->m_pmapLoadedUITools;
	SAFEADDREF(*ppMap);

	return S_OK;
}


STDMETHODIMP CVWUIView::XVWUIView::get_LoadedUITool(BSTR bstrName, IVWUITool * * ppUITool)
{
	METHOD_PROLOGUE(CVWUIView, VWUIView)
	CComVariant var;
	HRESULT hr = S_OK;
	VARIANT_BOOL fValid = FALSE;
	CComBSTR bstrEmpty = "";
	
	var.Clear();
	// Is control initialized?
	if ( ( NULL == pThis->m_plistCurrentUITools ) ||
		 ( NULL == pThis->m_pmapLoadedUITools ) )
	{
		hr = VWSTUDIO_E_BADCONTROLINITIALIZATION;
		goto EXIT_FAIL;
	}

	// Do we have a LoadedUITool of this name?
	hr = pThis->m_pmapLoadedUITools->IsValid(bstrName, &fValid);
	if(FAILED(hr))
	{
		hr = VWSTUDIO_E_INVALIDLOADEDUITOOLNAME;
		goto EXIT_FAIL;
	}
	
	if (VARIANT_TRUE != fValid)
	{ 
		// No LoadUITool of that name.
		hr = VWSTUDIO_E_INVALIDLOADEDUITOOLNAME;
		goto EXIT_FAIL;
	}

	// Grab the Tool from the LoadedUITools and QI for IVWUITool
	hr = pThis->m_pmapLoadedUITools->get_Property(bstrName, &var);
	if(FAILED(hr)) goto EXIT_FAIL;

	if ( (var.vt != VT_DISPATCH) || (NULL == var.pdispVal))
	{
		hr = E_FAIL;	// Review: Error msg.
		goto EXIT_FAIL;
	}

	hr = var.pdispVal->QueryInterface(IID_IVWUITool,(LPVOID*)ppUITool);
	if(FAILED(hr)) goto EXIT_FAIL;

EXIT_FAIL:

	return hr;
}

STDMETHODIMP CVWUIView::XVWUIView::get_CurrentUIToolNameList(IPropertyList * * ppNames)
{
	METHOD_PROLOGUE(CVWUIView, VWUIView)
	HRESULT hr = S_OK;
	VARIANT_BOOL  bLastItem; 
	BSTR bstrReturnValue = NULL; 
	CComVariant varUITool; 
	varUITool.Clear(); 
	IPropertyList *pPL = NULL;

	if ( ( NULL == pThis->m_plistCurrentUITools ) ||
		 ( NULL == pThis->m_pmapLoadedUITools ) )
	{
		hr = VWSTUDIO_E_BADCONTROLINITIALIZATION;
		goto EXIT_FAIL;
	}

	if( NULL == ppNames)
	{
		hr = E_POINTER;
		goto EXIT_FAIL;
	}

	// Iterate through all current tools.
	hr = pThis->m_plistCurrentUITools->FirstItem( &bstrReturnValue, &varUITool, &bLastItem ) ;
	if( FAILED ( hr) )goto EXIT_FAIL;

	// Build the property list
	hr = CreatePropertyList(pThis->m_pWorld, &pPL);
	if( FAILED(hr) ) goto EXIT_FAIL;

	while( VARIANT_FALSE == bLastItem )  
	{ 
		SAFEFREESTRING(bstrReturnValue); 
		if( VT_DISPATCH != varUITool.vt ) 
			goto EXIT_FAIL; 
		// Iterate through loaded tools and do a match.
		{
			VARIANT_BOOL  bLastMapItem; 
			BSTR bstrMapReturnValue; 
			CComVariant varMapUITool; 
			varMapUITool.Clear(); 

			bstrMapReturnValue = NULL;
			hr = pThis->m_pmapLoadedUITools->FirstItem( &bstrMapReturnValue, &varMapUITool, &bLastMapItem ) ;
			if( FAILED ( hr) )goto EXIT_FAIL;
			while( VARIANT_FALSE == bLastMapItem )  
			{ 
				if(varMapUITool.pdispVal == varUITool.pdispVal)
				{
					// Found a match so add name to the returning PropertyList
					pPL->AddString(bstrMapReturnValue);
				}
				varMapUITool.Clear();  
				hr = pThis->m_pmapLoadedUITools->NextItem( &bstrMapReturnValue, &varMapUITool, &bLastMapItem );  
				if( FAILED ( hr) )	goto EXIT_FAIL;  
			}

		}
		varUITool.Clear();  
		hr = pThis->m_plistCurrentUITools->NextItem( &bstrReturnValue, &varUITool, &bLastItem );  
		if( FAILED ( hr) )	goto EXIT_FAIL;  
	} 

	*ppNames = pPL;
	SAFEADDREF(pPL);

EXIT_FAIL:
	SAFEFREESTRING(bstrReturnValue);  
	SAFERELEASE(pPL);
	return hr;
}

STDMETHODIMP CVWUIView::XVWUIView::put_CurrentUIToolNameList(IPropertyList *pNames)
{
	return E_NOTIMPL;
}

STDMETHODIMP CVWUIView::XVWUIView::RemoveCurrentUITool(BSTR bstrName)
{
	METHOD_PROLOGUE(CVWUIView, VWUIView)
	CComVariant var;
	HRESULT hr = S_OK;
	VARIANT_BOOL fValid = FALSE;
	CComBSTR bstrEmpty = "";
	
	EnterCriticalSection(&pThis->m_CS);

	var.Clear();
	// Is control initialized?
	if ( ( NULL == pThis->m_plistCurrentUITools ) ||
		 ( NULL == pThis->m_pmapLoadedUITools ) )
	{
		hr = VWSTUDIO_E_BADCONTROLINITIALIZATION;
		goto EXIT_FAIL;
	}
	// Review. Is this the correct behavior?
	// If bstrName == NULL or empty string, remove all CurrentUITools
	if( ( bstrName == NULL ) || ( bstrName == bstrEmpty) )
	{
		pThis->m_plistCurrentUITools->RemoveAll();
		goto EXIT_FAIL; // done.. lets exit
	}

	// Do we have a LoadedUITool of this name?
	hr = pThis->m_pmapLoadedUITools->IsValid(bstrName, &fValid);
	if(FAILED(hr))
	{
		hr = VWSTUDIO_E_INVALIDLOADEDUITOOLNAME;
		goto EXIT_FAIL;
	}
	
	if (VARIANT_TRUE != fValid)
	{ 
		// No LoadUITool of that name.
		hr = VWSTUDIO_E_INVALIDLOADEDUITOOLNAME;
		goto EXIT_FAIL;
	}

	// Grab the Tool from the LoadedUITools 
	hr = pThis->m_pmapLoadedUITools->get_Property(bstrName, &var);
	if(FAILED(hr)) goto EXIT_FAIL;

	if (var.vt != VT_DISPATCH)
	{
		hr = E_FAIL;	// Review: Error msg.
		goto EXIT_FAIL;
	}
	// Remove tool from CurrentUIToolList
	hr = pThis->m_plistCurrentUITools->Remove( var );
	if(FAILED(hr)) goto EXIT_FAIL;

EXIT_FAIL:

	LeaveCriticalSection(&pThis->m_CS);

	return hr;
}


STDMETHODIMP_(ULONG) CVWUIView::XVWUIEvents::AddRef()
{
	METHOD_PROLOGUE(CVWUIView, VWUIEvents)
	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CVWUIView::XVWUIEvents::Release()
{
	METHOD_PROLOGUE(CVWUIView, VWUIEvents)
	return pThis->ExternalRelease();
}

STDMETHODIMP CVWUIView::XVWUIEvents::QueryInterface(
    REFIID iid, void FAR* FAR* ppvObj)
{
	METHOD_PROLOGUE(CVWUIView, VWUIEvents)
    return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CVWUIView::XVWUIEvents::GetIDsOfNames(REFIID riid,LPOLESTR * rgszNames,UINT cNames,LCID lcid,DISPID * rgDispId)
{
	METHOD_PROLOGUE(CVWUIView, VWUIEvents)
	IDispatch * pdisp = pThis->GetIDispatch( FALSE );
	ASSERT( pdisp );
	return pdisp->GetIDsOfNames(riid,rgszNames,cNames,lcid,rgDispId);
}
STDMETHODIMP CVWUIView::XVWUIEvents::Invoke(
		DISPID dispIdMember,
        REFIID riid,
        LCID lcid,
        WORD wFlags,
        DISPPARAMS * pDispParams,
        VARIANT * pVarResult,
        EXCEPINFO * pExcepInfo,
        UINT * puArgErr )
{
	METHOD_PROLOGUE(CVWUIView, VWUIEvents)
	IDispatch * pdisp = pThis->GetIDispatch( FALSE );
	ASSERT( pdisp );
	return pdisp->Invoke(dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr );
}
STDMETHODIMP CVWUIView::XVWUIEvents::GetTypeInfoCount(UINT * pctinfo)
{
	METHOD_PROLOGUE(CVWUIView, VWUIEvents)
	*pctinfo = 0;
	return S_OK;
}

STDMETHODIMP CVWUIView::XVWUIEvents::GetTypeInfo(UINT iTInfo,LCID lcid,ITypeInfo ** ppTInfo )
{
	METHOD_PROLOGUE(CVWUIView, VWUIEvents)

	return E_FAIL;
}

STDMETHODIMP CVWUIView::XVWUIEvents::OnUIUpdate(int enumUpdateType ,struct IVWUIView * pvwSrc ,VARIANT varHint, VARIANT_BOOL vbOnlyRefreshSource)
{
	HRESULT hr = S_OK;
	METHOD_PROLOGUE(CVWUIView, VWUIEvents)

	if (VARIANT_TRUE == vbOnlyRefreshSource) 
	{
		if (pvwSrc == pThis->m_pVWUIView)
			hr = pThis->HandleUIUpdate((enumVWUIEventTypes)enumUpdateType, pvwSrc, varHint);
	}
	else
		hr = pThis->HandleUIUpdate((enumVWUIEventTypes)enumUpdateType, pvwSrc, varHint);

	return hr;
}

STDMETHODIMP CVWUIView::XVWUIView::OptionalSelection(VARIANT varSelection)
{											
	METHOD_PROLOGUE_EX_(CVWUIView, VWUIView);								
	return pThis->HandleOptionalSelection(varSelection);
}

STDMETHODIMP CVWUIView::XVWUIView::HitTest(VARIANT varHit, long nFlags, VARIANT*pvarHitResult)
{											
	METHOD_PROLOGUE_EX_(CVWUIView, VWUIView);								
	return pThis->HandleHitTest(varHit, nFlags, pvarHitResult);
}

STDMETHODIMP CVWUIView::XVWUIView::DeviceToWorld( float *pflX, float *pflY, float *pflZ )
{
	METHOD_PROLOGUE_EX_(CVWUIView, VWUIView);								
	return pThis->HandleDeviceToWorld(pflX, pflY,pflZ);
}
	

STDMETHODIMP_(ULONG) CVWUIView::XVWExecuteUndo::AddRef()
{
	METHOD_PROLOGUE(CVWUIView, VWExecuteUndo)
	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CVWUIView::XVWExecuteUndo::Release()
{
	METHOD_PROLOGUE(CVWUIView, VWExecuteUndo)
	return pThis->ExternalRelease();
}

STDMETHODIMP CVWUIView::XVWExecuteUndo::QueryInterface(
    REFIID iid, void FAR* FAR* ppvObj)
{
	METHOD_PROLOGUE(CVWUIView, VWExecuteUndo)
    return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CVWUIView::XVWExecuteUndo::GetIDsOfNames(REFIID riid,LPOLESTR * rgszNames,UINT cNames,LCID lcid,DISPID * rgDispId)
{
	METHOD_PROLOGUE(CVWUIView, VWExecuteUndo)
	IDispatch * pdisp = pThis->GetIDispatch( FALSE );
	ASSERT( pdisp );
	return pdisp->GetIDsOfNames(riid,rgszNames,cNames,lcid,rgDispId);
}
STDMETHODIMP CVWUIView::XVWExecuteUndo::Invoke(
		DISPID dispIdMember,
        REFIID riid,
        LCID lcid,
        WORD wFlags,
        DISPPARAMS * pDispParams,
        VARIANT * pVarResult,
        EXCEPINFO * pExcepInfo,
        UINT * puArgErr )
{
	METHOD_PROLOGUE(CVWUIView, VWExecuteUndo)
	IDispatch * pdisp = pThis->GetIDispatch( FALSE );
	ASSERT( pdisp );
	return pdisp->Invoke(dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr );
}

STDMETHODIMP CVWUIView::XVWExecuteUndo::GetTypeInfoCount(UINT * pctinfo)
{
	METHOD_PROLOGUE(CVWUIView, VWExecuteUndo)
	*pctinfo = 0;
	return S_OK;
}

STDMETHODIMP CVWUIView::XVWExecuteUndo::GetTypeInfo(UINT iTInfo,LCID lcid,ITypeInfo ** ppTInfo )
{
	METHOD_PROLOGUE(CVWUIView, VWExecuteUndo)
	return E_FAIL;
}

STDMETHODIMP CVWUIView::XVWExecuteUndo::Undo(IVWUndoItem * pUItem)
{
	HRESULT hr;

	METHOD_PROLOGUE(CVWUIView, VWExecuteUndo)
	hr = pThis->HandleUndo(pUItem);
	return hr;
}

STDMETHODIMP CVWUIView::XVWExecuteUndo::Redo(IVWUndoItem * pUItem)
{
	HRESULT hr;

	METHOD_PROLOGUE(CVWUIView, VWExecuteUndo)
	hr = pThis->HandleRedo(pUItem);

	return hr;
}

void CVWUIView::EnableDropFile(BOOL bEnable)
{
	if (bEnable)
		// initialize drag/drop support
		m_dropTarget.Initialize(this);
	else
		m_dropTarget.Revoke();
}

void CVWUIView::Refresh( )
{
	HandleRefresh();

}

void CVWUIView::HandleRefresh( )
{
	CVWViewCtrl::Refresh();

}

// Review: Based on (R.E.)'s implementation in VWRenderVW
LRESULT CVWUIView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam )
{
	BOOL bHandled = HandleOnMessage( message, wParam, lParam );

	// pass on to default
	if (bHandled != TRUE)
		bHandled = CVWViewCtrl::WindowProc( message, wParam, lParam );

	return bHandled;

//	else
//		// REVIEW: What to do now? Return 0? Can't pass on
//		return CWnd::DefWindowProc( message, wParam, lParam);
}

BOOL CVWUIView::HandleOnMessage(UINT message, WPARAM wParam, LPARAM lParam )
{
	HRESULT hr = S_OK; 
	VARIANT_BOOL bHandled = VARIANT_FALSE;
	IPropertyList *pListCurrentUITools = NULL;
	BOOL bReturn = FALSE;
	BSTR bstrReturnValue = NULL; 
	long nCount, i;

//	varUITool.Clear(); 

	if(message == WM_LBUTTONDOWN)
		hr = hr;

	if(m_plistCurrentUITools) 
	{ 
		// Need to explicity QI for IPropertyList to get correct pointer for proxy/stub marshalling
		hr = m_plistCurrentUITools->QueryInterface(IID_IPropertyList, (LPVOID*) &pListCurrentUITools);

		if (FAILED(hr))
		{
			SAFERELEASE(pListCurrentUITools);
			return hr;
		}

		if (!pListCurrentUITools)
			goto EXIT;

		hr = pListCurrentUITools->get_Count( &nCount ); 
		if( FAILED(hr) ) goto EXIT; 

		for (i = 0; i < nCount; i++)
		{
			CComVariant varUITool; 

			hr = pListCurrentUITools->get_Property(i, &varUITool);
			if( FAILED(hr) ) goto EXIT; 

			hr = ((IVWUITool*)varUITool.pdispVal)->OnMessage(m_pVWUIView, message, wParam, lParam, &bHandled );
			if( (hr != E_NOTIMPL ) && FAILED(hr))
				goto EXIT;

			if(bHandled) 
				bReturn = TRUE; 
		}
/*		while( VARIANT_FALSE == bLastItem )  
		{ 
			SAFEFREESTRING(bstrReturnValue); 

			hr = ((IVWUITool*)varUITool.pdispVal)->OnMessage(m_pVWUIView, message, wParam, lParam, &bHandled );
			if( (hr != E_NOTIMPL ) && FAILED(hr))
				goto EXIT;

			varUITool.Clear();

			if(bHandled) 
				bReturn = TRUE; 

			hr = m_plistCurrentUITools->NextItem( &bstrReturnValue, &varUITool, &bLastItem ); 
			if( FAILED ( hr) )	goto EXIT;
		}
*/
	} 

	EXIT: 
	SAFEFREESTRING(bstrReturnValue); 
	SAFERELEASE(pListCurrentUITools);

	return bReturn;	
}

LPDISPATCH CVWUIView::GetTargetObjectProperty() 
{
	HRESULT hr = S_OK;																	 
	SAFEADDREF( m_pTargetObjectProperty );																  
	return m_pTargetObjectProperty; 																		 
}

void CVWUIView::SetTargetObjectProperty(LPDISPATCH newValue) 
{
	IObjectProperty * pOP = NULL;
	HRESULT hr = S_OK;
	IWorld* pWorld = NULL;

	CreateHelperComponents(); //Make sure we're init'ed.

	if( !m_pControlManager  )
	{
		if( FALSE == CreateHelperComponents() )
			goto EXIT_FAIL;
	}
	
	SAFERELEASE( m_pTargetObjectProperty );
	if( NULL != newValue )
	{
		hr = newValue->QueryInterface( IID_IObjectProperty, (LPVOID*)&pOP);
		if ( SUCCEEDED(hr) ) 
		{
				m_pTargetObjectProperty = pOP;													 
				SAFEADDREF( m_pTargetObjectProperty );

				m_pTargetObjectProperty->get_World(&pWorld);
				m_pControlManager->put_World(pWorld);																	 
		}
	
	}

	
	Refresh();																	 
	SetModifiedFlag();
/* Review: Right now don't do any selection setting on SetTargObj
	ASSERT(m_pVWUIView);
	if( m_pVWUIView )
	{
		hr = m_pControlManager->ClearSelections());
		if(FAILED( hr )) goto EXIT_FAIL;
		if( NULL != pOP )
		{
			VARIANT var;
			var.vt = VT_DISPATCH;
			var.pdispVal = (IDispatch *)pOP;
			hr = HandleAddSelection( var ); 
			if(FAILED( hr )) goto EXIT_FAIL;
		}
	}

	// Review: What if m_pVWUIView == NULL?

*/

EXIT_FAIL:
	SAFERELEASE( pWorld );
	SAFERELEASE( pOP );
}

int CVWUIView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	HRESULT hr = S_OK;				
	IDispatch *pdispVC = NULL;		

	if (CVWViewCtrl::OnCreate(lpCreateStruct) == -1){
			return -1;}
	
	pdispVC = GetIDispatch( TRUE ) ;
	ASSERT( pdispVC );				
	if( pdispVC )						  
	{								  
		hr = pdispVC->QueryInterface(IID_IVWUIView, (LPVOID*)&m_pVWUIView); 
		ASSERT( SUCCEEDED(hr ));												   
	}

	if( pdispVC )						  
	{								  
		hr = pdispVC->QueryInterface(IID_IVWExecuteUndo, (LPVOID*)&m_pVWExecuteUndo); 
		ASSERT( SUCCEEDED(hr ));												   
	}

	SAFERELEASE( pdispVC );

	return 0;
}

void CVWUIView::OnDestroy()
{
	// Release everything here, not in the destructor to avoid circular dependencies.
	HRESULT hr = S_OK;

	SAFERELEASE(m_pVWExecuteUndo);

	if (m_pControlManager)
	{
		hr = UnAdvise(m_pControlManager, IID_IVWUIEvents, m_dwUIViewEventsCookie);
		if (SUCCEEDED(hr))
		{
			TRACE("CVWUIView::OnDestroy: UnAdvise, control manager %x, dwUIViewEventsCookie %x\n", m_pControlManager, m_dwUIViewEventsCookie);
		}
		else
		{
			TRACE("CVWUIView::OnDestroy: failed during UnAdvise, control manager %x, hr %x\n", m_pControlManager, hr);
		}

		m_dwUIViewEventsCookie = 0;

		SAFERELEASE(m_pControlManager);
	}

	SAFERELEASE( m_pTargetObjectProperty );	

	if(m_pmapLoadedUITools)
	{
		// Need to QI for correct interface so proxy/stub marshalling works correctly
		IPropertyMap * pMapLoadedUITools;
		hr = m_pmapLoadedUITools->QueryInterface(IID_IPropertyMap, (LPVOID*) &pMapLoadedUITools);
		if (SUCCEEDED(hr) && pMapLoadedUITools)
		{
			pMapLoadedUITools->RemoveAll();
			SAFERELEASE(pMapLoadedUITools);
		}
		else
			ASSERT(FALSE);
	}

	SAFERELEASE( m_pmapLoadedUITools );	
	
	if(m_plistCurrentUITools)
	{	
		// Need to QI for correct interface so proxy/stub marshalling works correctly
		IPropertyList * pListCurrentUITools;
		hr = m_plistCurrentUITools->QueryInterface(IID_IPropertyList, (LPVOID*) &pListCurrentUITools);
		if (SUCCEEDED(hr) && pListCurrentUITools)
		{
			pListCurrentUITools->RemoveAll();
			SAFERELEASE(pListCurrentUITools);
		}
		else
			ASSERT(FALSE);
	}

	SAFERELEASE( m_plistCurrentUITools );	

	CVWViewCtrl::OnDestroy();

	SAFERELEASE( m_pVWUIView );
}

BOOL CVWUIView::CreateHelperComponents(VARIANT_BOOL bCreateControlManager)
{	HRESULT hr = S_OK;	 											
	BOOL fReturn = TRUE;											
	IUnknown *punk = NULL;									
	IVWUndoStacks * pUndoStack = NULL;
	static CComBSTR bstrControlManager("ControlManager");
	VARIANT_BOOL vBool = VARIANT_FALSE;

	//if( m_pControlManager ) goto SUCCEEDED;			
	if( !m_pWorld ) goto EXIT_FAIL;						

#ifdef USE_CONTROLMANAGER
	if (bCreateControlManager)
	{
		if( !m_pControlManager )											
		{	
			// check to see if the tool exists first
			if (FAILED(hr = m_pWorld->IsValidTool(bstrControlManager, &vBool)))
				goto EXIT_FAIL;

			if (vBool == VARIANT_TRUE)
			{
				// See if one already exists in the World's ToolMap
				hr = m_pWorld->get_Tool(bstrControlManager, &punk);
				if (FAILED(hr))
					goto EXIT_FAIL;
			}
			else
			{	// None has been created, so create it.
				hr = m_pWorld->CreateToolExt( bstrControlManager, CLSID_VWControlManager,&punk);			
				if( FAILED(hr) ) goto EXIT_FAIL;
										
				// Store a pointer to the World in the UndoStack.
				hr = punk->QueryInterface( IID_IVWUndoStacks, 				
									(LPVOID*)&pUndoStack );				
				if( FAILED(hr) ) goto EXIT_FAIL;

				hr = pUndoStack->put_World(m_pWorld);
				if( FAILED(hr) ) goto EXIT_FAIL;
			}
			// Cache a pointer to the ControlManager
			hr = punk->QueryInterface( IID_IVWControlManager, 				
								(LPVOID*)&m_pControlManager );				
			if( FAILED(hr) ) goto EXIT_FAIL;

			hr = Advise(m_pControlManager, IID_IVWUIEvents, &m_dwUIViewEventsCookie);
			if (SUCCEEDED(hr))
			{
				TRACE("CVWUIView::CreateHelperComponents: Advise, control manager %x, dwUIViewEventsCookie %x\n", m_pControlManager, m_dwUIViewEventsCookie);
			}
			else
			{
				TRACE("CVWUIView::CreateHelperComponents: failed during Advise, control manager %x, hr %x\n", m_pControlManager, hr);

				m_dwUIViewEventsCookie = 0;
				SAFERELEASE(m_pControlManager);
			}
			if( FAILED(hr) ) goto EXIT_FAIL;
		}
		else
		{
			hr = m_pWorld->get_Tool(bstrControlManager, &punk);
			if(FAILED(hr))
			{
				hr = m_pWorld->CreateToolFromInterface(bstrControlManager, m_pControlManager);
				if( FAILED(hr) ) goto EXIT_FAIL;
			}
		}
	}
#endif

	if(NULL == m_pmapLoadedUITools)
	{
		hr = CreatePropertyMap(m_pWorld, &m_pmapLoadedUITools);
		if( FAILED(hr) ) goto EXIT_FAIL;
								
	}																
	if(NULL == m_plistCurrentUITools)
	{
		hr = CreatePropertyList(m_pWorld, &m_plistCurrentUITools);
		if( FAILED(hr) ) goto EXIT_FAIL;
								
	}																
	goto SUCCEEDED;													
EXIT_FAIL:																
	fReturn = FALSE;												
SUCCEEDED:															
	SAFERELEASE(punk); 												
	SAFERELEASE(pUndoStack); 												
	return fReturn; 
}												

void CVWUIView::ReleaseHelperComponents()
{
	EnterCriticalSection(&m_CS);

	if(m_pmapLoadedUITools)
		m_pmapLoadedUITools->RemoveAll();
	SAFERELEASE( m_pmapLoadedUITools );	
	if(m_plistCurrentUITools)
		m_plistCurrentUITools->RemoveAll();
	SAFERELEASE( m_plistCurrentUITools );	

	LeaveCriticalSection(&m_CS);
}

LPDISPATCH CVWUIView::GetControlManager() 
{
	HRESULT hr = S_OK;			
	
	SAFEADDREF(m_pControlManager);

	return m_pControlManager; 																			 
}

void CVWUIView::SetControlManager(LPDISPATCH newValue) 
{
	HRESULT hr = S_OK;
	IVWControlManager* pCM = NULL;
	
	// UnAdvise old ControlManager
	if (m_pControlManager)
	{
		hr = UnAdvise(m_pControlManager, IID_IVWUIEvents, m_dwUIViewEventsCookie);
		if (SUCCEEDED(hr))
		{
			TRACE("CVWUIView::SetControlManager: UnAdvise, control manager %x, dwUIViewEventsCookie %x\n", m_pControlManager, m_dwUIViewEventsCookie);
		}
		else
		{
			TRACE("CVWUIView::SetControlManager: failed during UnAdvise, control manager %x, hr %x\n", m_pControlManager, hr);
		}

		m_dwUIViewEventsCookie = 0;

		SAFERELEASE(m_pControlManager);
	}

	if (NULL != newValue)
	{
		hr = newValue->QueryInterface(IID_IVWControlManager, (LPVOID*)&pCM);
		if (SUCCEEDED(hr)) 
		{
			m_pControlManager = pCM;													 
			SAFEADDREF(m_pControlManager);
			
			hr = Advise(m_pControlManager, IID_IVWUIEvents, &m_dwUIViewEventsCookie);
			if (SUCCEEDED(hr))
			{
				TRACE("CVWUIView::SetControlManager: Advise, control manager %x, dwUIViewEventsCookie %x\n", m_pControlManager, m_dwUIViewEventsCookie);
			}
			else
			{
				TRACE("CVWUIView::SetControlManager: failed during Advise, control manager %x, hr %x\n", m_pControlManager, hr);

				m_dwUIViewEventsCookie = 0;
				SAFERELEASE(m_pControlManager);
			}

			SAFERELEASE(pCM);
		}
	}

}

void CVWUIView::SetVWClient(LPDISPATCH newValue) 
{
	if (m_pControlManager)
	{
		HRESULT hr = S_OK;

		hr = UnAdvise(m_pControlManager, IID_IVWUIEvents, m_dwUIViewEventsCookie);
		if (SUCCEEDED(hr))
		{
			TRACE("CVWUIView::SetVWClient: UnAdvise, control manager %x, dwUIViewEventsCookie %x\n", m_pControlManager, m_dwUIViewEventsCookie);
		}
		else
		{
			TRACE("CVWUIView::SetVWClient: failed during UnAdvise, control manager %x, hr %x\n", m_pControlManager, hr);
		}

		m_dwUIViewEventsCookie = 0;

		SAFERELEASE(m_pControlManager);
	}

	CVWViewCtrl::SetVWClient(newValue);

	SetModifiedFlag();
}

HRESULT CVWUIView::HandleOnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient)
{
	//METHOD_PROLOGUE(CVWUIView, VWUIView)

	HRESULT hr = S_OK;
	long nCount;
	int i;
	
	// Is control initialized?
	if ( ( NULL == m_plistCurrentUITools ) ||
		 ( NULL == m_pmapLoadedUITools ) )
	{
		hr = VWSTUDIO_E_BADCONTROLINITIALIZATION;
		goto EXIT_FAIL;
	}

	hr = m_plistCurrentUITools->get_Count(&nCount);
	if(FAILED(hr)) goto EXIT_FAIL;

	for (i = 0; i < nCount; i++)
	{
		CComVariant varUITool; 

		hr = m_plistCurrentUITools->get_Property(i, &varUITool);
		if( FAILED(hr) ) goto EXIT_FAIL; 

		hr = ((IVWUITool*)varUITool.pdispVal)->OnUIEvent(pthing, bstrEventName, varArg, bFromClient);
		if( (hr != E_NOTIMPL ) && FAILED(hr))
			goto EXIT_FAIL;
	}

EXIT_FAIL:
	return hr;
}

HRESULT CVWUIView::UpdateCurrentUITools()
{
	CComPtr<IVWUITool> pVWUITool;
	long nCount;
	int i;
	HRESULT hr = S_OK;

	EnterCriticalSection(&m_CS);
	
	if(!m_plistCurrentUITools)
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = m_plistCurrentUITools->get_Count( &nCount )))
		goto ERROR_ENCOUNTERED;

	for (i = 0; i < nCount; i++)
	{
		if (FAILED(hr = m_plistCurrentUITools->get_Dispatch(i, (IDispatch**) &pVWUITool)) || !pVWUITool)
			continue;
		pVWUITool->Update();
		pVWUITool.Release();
	}

ERROR_ENCOUNTERED:
	
	LeaveCriticalSection(&m_CS);
	
	return hr;
}

/////////////////////////////////////////////////////////////////////////
// Drag/drop support

void CVWUIDropTarget::Initialize(CVWUIView *pParent)
{
	// stow backpointer
	m_pParent = pParent;

	// initialize drag/drop
	Register(pParent);

};

//MM: No drag/drop for Version1.
DROPEFFECT CVWUIDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	return DROPEFFECT_NONE; //DROPEFFECT_COPY|DROPEFFECT_MOVE;
}


DROPEFFECT CVWUIDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	return DROPEFFECT_NONE; //DROPEFFECT_COPY|DROPEFFECT_MOVE;
}

BOOL CVWUIDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	TCHAR filename[MAX_PATH];
	FORMATETC etc = {NULL, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
	etc.cfFormat = (CLIPFORMAT)CF_HDROP;

	if (pDataObject->IsDataAvailable(CF_HDROP))
	{
		HDROP hDropInfo = (HDROP)pDataObject->GetGlobalData(CF_HDROP, &etc);
		if (hDropInfo!=NULL)
		{
			UINT cch = DragQueryFile( hDropInfo, 0, filename, MAX_PATH );

			if (cch!=0)
			{
				ASSERT(m_pParent);
				return m_pParent->HandleDropFile(point, filename);
			}
		}
	}
	return FALSE;
}


