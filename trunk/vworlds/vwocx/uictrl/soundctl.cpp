// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VwsoundCtl.cpp : Implementation of the CVwsoundCtrl ActiveX Control class.

#include "stdafx.h"

#include <objbase.h>
#include <initguid.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <dsound.h>

#include <vwobject.h>
#include <afxtempl.h>
#include <propbase.h>
#include <inetfile.h>
#include <vector.h>
#include <vwrender.h>
#include <isound.h>
#include "uictrl.h"

#include "soundCtl.h"

#include "vwevents.h"

#define IID_DEFINED
#include "VWRender_i.c"
#include "vwgeom_i.c"
#include "isound_i.c"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Interface IDs

const IID BASED_CODE IID_DVwsound =
		{ 0x5769b8a, 0xa180, 0x11d1, { 0x83, 0xe9, 0, 0xc0, 0x4f, 0xb6, 0xfa, 0x46 } };
const IID BASED_CODE IID_DVwsoundEvents =
		{ 0x5769b8b, 0xa180, 0x11d1, { 0x83, 0xe9, 0, 0xc0, 0x4f, 0xb6, 0xfa, 0x46 } };


IMPLEMENT_DYNCREATE(CVwsoundCtrl, CVWViewCtrl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CVwsoundCtrl, CVWViewCtrl)
	//{{AFX_MSG_MAP(CVwsoundCtrl)
	ON_WM_CREATE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CVwsoundCtrl, CVWViewCtrl)
	//{{AFX_DISPATCH_MAP(CVwsoundCtrl)
	DISP_PROPERTY_EX(CVwsoundCtrl, "VWClient", GetVWClient, SetVWClient, VT_DISPATCH)
	DISP_PROPERTY_EX(CVwsoundCtrl, "RenderRoot", GetRenderRoot, SetRenderRoot, VT_DISPATCH)
	DISP_PROPERTY_EX(CVwsoundCtrl, "Enable", GetEnable, SetEnable, VT_BOOL)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CVwsoundCtrl, CVWViewCtrl)
	//{{AFX_EVENT_MAP(CVwsoundCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT_MAP
END_EVENT_MAP()

BEGIN_INTERFACE_MAP(CVwsoundCtrl, CVWViewCtrl)
	INTERFACE_PART(CVwsoundCtrl, IID_DVwsound, Dispatch)
END_INTERFACE_MAP()

BEGIN_EVENTSINK_MAP(CVwsoundCtrl, CVWViewCtrl)
    //{{AFX_EVENTSINK_MAP(CVwsoundCtrl)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CVwsoundCtrl, 1)
	   PROPPAGEID(CLSID_CColorPropPage)
END_PROPPAGEIDS(CVwsoundCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CVwsoundCtrl, "VWSOUND.VwsoundCtrl.1",
	0x5769b8c, 0xa180, 0x11d1, 0x83, 0xe9, 0, 0xc0, 0x4f, 0xb6, 0xfa, 0x46)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CVwsoundCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs
/*
const IID BASED_CODE IID_DVwsound =
		{ 0x5769b8a, 0xa180, 0x11d1, { 0x83, 0xe9, 0, 0xc0, 0x4f, 0xb6, 0xfa, 0x46 } };
const IID BASED_CODE IID_DVwsoundEvents =
		{ 0x5769b8b, 0xa180, 0x11d1, { 0x83, 0xe9, 0, 0xc0, 0x4f, 0xb6, 0xfa, 0x46 } };

*/
/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwVwsoundOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE|
	OLEMISC_ALWAYSRUN ;
	
IMPLEMENT_OLECTLTYPE(CVwsoundCtrl, IDS_SOUND, _dwVwsoundOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CVwsoundCtrl::CVwsoundCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CVwsoundCtrl

BOOL CVwsoundCtrl::CVwsoundCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegInsertable | afxRegApartmentThreading to afxRegInsertable.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_SOUND,
			IDB_VWCLIOCX,
			afxRegInsertable | afxRegApartmentThreading,
			_dwVwsoundOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);


}


/////////////////////////////////////////////////////////////////////////////
// CVwsoundCtrl::CVwsoundCtrl - Constructor

CVwsoundCtrl::CVwsoundCtrl()
{
	InitializeIIDs(&IID_DVwsound, &IID_DVwsoundEvents);
	m_ulFlags = ADVISE_CLIENT | ADVISE_WORLD;
	m_bEnable = VARIANT_TRUE;
	m_pDSound = NULL;
	m_pDSListener = NULL;
	m_pRenderViewOCX = NULL;

	// TODO: Initialize your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CVwsoundCtrl::~CVwsoundCtrl - Destructor

CVwsoundCtrl::~CVwsoundCtrl()
{
	// TODO: Cleanup your control's instance data here.
	ClearAll();

	SAFERELEASE(m_pDSListener);
	SAFERELEASE(m_pDSound);
	SAFERELEASE(m_pRenderViewOCX);
	//KillTimer(1);

}


/////////////////////////////////////////////////////////////////////////////
// CVwsoundCtrl::OnDraw - Drawing function

void CVwsoundCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
}


/////////////////////////////////////////////////////////////////////////////
// CVwsoundCtrl::DoPropExchange - Persistence support

void CVwsoundCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));

	CVWViewCtrl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}


/////////////////////////////////////////////////////////////////////////////
// CVwsoundCtrl::GetControlFlags -
// Flags to customize MFC's implementation of ActiveX controls.
//
// For information on using these flags, please see MFC technical note
// #nnn, "Optimizing an ActiveX Control".
DWORD CVwsoundCtrl::GetControlFlags()
{
	DWORD dwFlags = CVWViewCtrl::GetControlFlags();


	// The control can activate without creating a window.
	// TODO: when writing the control's message handlers, avoid using
	//		the m_hWnd member variable without first checking that its
	//		value is non-NULL.
	dwFlags &= ~windowlessActivate;
	return dwFlags;
}


/////////////////////////////////////////////////////////////////////////////
// CVwsoundCtrl::OnResetState - Reset control to default state

void CVwsoundCtrl::OnResetState()
{
	CVWViewCtrl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



BOOL CVwsoundCtrl::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	
	BOOL retval =  CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	// No longer directly supporting IMA since it requires a separate download,
	// but leaving in incase we want to do this later.
	//if (retval)
	//{
	//	retval = m_IMA.Create(lpszClassName,lpszWindowName, dwStyle, rect,this,255);
	//}
	return retval;
}

int CVwsoundCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CVWViewCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	static CLSID const clsid
			= { 0x300673c3, 0x8be3, 0x11cf, { 0xb4, 0xb7, 0x0, 0xaa, 0x0, 0xc0, 0x18, 0x2 } };
	
	/*  
	// No longer directly supporting IMA since it requires a separate download,
	// leaving in incase we want to this later..
	if (!m_IMA.CreateDispatch(clsid))
	{
		return -1;
	}
	CString strPer("c:/program files/ima25/documentation/music/mindor.per");
	CString strStyle("c:/program files/ima25/documentation/music/jazz.sty");
	CString strTemplate("c:/program files/ima25/documentation/music/funk.tpl");

	m_IMA.EZPlaySection(strStyle, strPer, 65535, 0, "Default", 0, 1, TRUE, TRUE);
	*/

	HRESULT hr = S_OK;
	DSBUFFERDESC dsbd = {0};
	IDirectSoundBuffer *pbuf = NULL;

	
	hr = DirectSoundCreate(NULL, &m_pDSound, NULL);
	if (FAILED(hr))
		return 0;

	HWND hWnd = m_hWnd;

	CWnd *pWnd = GetTopLevelParent();
	if (pWnd)
		hWnd = pWnd->GetSafeHwnd();

	hr = m_pDSound->SetCooperativeLevel(hWnd, DSSCL_NORMAL);
	if (FAILED(hr))
		return 0;

	// create listener

	// first, create a primary buffer
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D;

	hr = m_pDSound->CreateSoundBuffer(&dsbd, &pbuf, NULL);
	if (FAILED(hr))
		return 0;

	// query for the listener interface
	hr = pbuf->QueryInterface(IID_IDirectSound3DListener, (void **) &m_pDSListener);
	SAFERELEASE(pbuf);

	if (FAILED(hr))
		return 0;

	// Set the position way out there, so that the initial sounds are quite
	m_pDSListener->SetPosition(1000,1000,1000, DS3D_DEFERRED);

	m_pDSListener->CommitDeferredSettings();

	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CVwsoundCtrl message handlers

LPDISPATCH CVwsoundCtrl::GetVWClient() 
{
	// TODO: Add your property handler here

	return CVWViewCtrl::GetVWClient();
}

void CVwsoundCtrl::SetVWClient(LPDISPATCH newValue) 
{
	// TODO: Add your property handler here
	ClearAll();	
	CVWViewCtrl::SetVWClient(newValue);
	if (newValue)
	{
		if (m_hWnd)
			SetTimer(1, 1000,NULL);
	}
	else
	{
		if (m_hWnd)
			KillTimer(1);
	}

}

HRESULT CVwsoundCtrl::HandleOnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient)
{ 
	HRESULT hr = S_OK;
	static CComBSTR bstrPlaySound("PlaySound");
	static CComBSTR bstrSoundEvent("SoundEvent");
	static CComBSTR bstrEnterRoom(VW_ENTERROOM_EVENT_STR);
	static CComBSTR bstrLeaveRoom(VW_LEAVEROOM_EVENT_STR);
	
	CVWViewCtrl::HandleOnUIEvent(pthing, bstrEventName, varArg, bFromClient);

	if (m_bEnable == VARIANT_FALSE)
		return S_OK;

	if (m_pDSound == NULL || bFromClient == FALSE)
		return S_OK;

	if (CompareElements(&bstrPlaySound.m_str, &bstrEventName))
	{
		CComVariant varArray;
		long propcnt = 0;
		long lLBound = 0, lUBound = 0;
		long i = 0, l = 0;
		SAFEARRAY* psaArguments = NULL;
		CComVariant DataArray[4];
		CComBSTR bstrSoundURL;
		VARIANT_BOOL bIsLooping;
		long id,lVolume;
		CSoundBufferPtr	SoundBufferPtr;

		if(FAILED(::VariantCopyInd(&varArray, (VARIANT *)&varArg)))
			goto ERROR_ENCOUNTERED;
		
		// we have a SAFEARRAY
		if (varArray.vt == (VT_VARIANT | VT_ARRAY))
		{
			psaArguments = (SAFEARRAY*)varArray.parray;

			// Verify that the SafeArray is the proper shape.
			if (SafeArrayGetDim(psaArguments) != 1)
			{
				hr = E_INVALIDARG;
				goto ERROR_ENCOUNTERED;
			}

			// Get array bounds    
			hr = SafeArrayGetLBound(psaArguments, 1, &lLBound);     
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			hr = SafeArrayGetUBound(psaArguments, 1, &lUBound);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED; 

			propcnt = lUBound - lLBound + 1;
		}
	
		//extract the data
		if ((psaArguments != NULL) &&
			(propcnt == 4))
		{
			// Loop through the array retrieving parameters
			for (i = 0, l = lLBound; l <= lUBound; i++, l++)
			{
				CComVariant v;

				hr = SafeArrayGetElement(psaArguments, &l, &v);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;   
        
				// Check if optional/variable parameter was passed. 
				// VT_ERROR/DISP_E_PARAMNOTFOUND indicates that the parameter was not passed by the controller.
				if (V_VT(&v) == VT_ERROR && V_ERROR(&v) == DISP_E_PARAMNOTFOUND)
				{
					hr = ResultFromScode(DISP_E_PARAMNOTFOUND);   // No parameter passed
					goto ERROR_ENCOUNTERED;
				}

				// If a parameter was passed by reference, get the value after de-reference.  
				// VBA will pass variables by reference.
				hr = VariantCopyInd(&DataArray[i], &v);   
				if (FAILED(hr))
				{    
					// Parameter not passed. This corresponds to an example like Obj.Add(1, 2, , 4), 
					// where the third parameter is not passed. Ignore and continue.
					if (GetScode(hr) == DISP_E_PARAMNOTFOUND)   
						continue;
					else 
						goto ERROR_ENCOUNTERED;  // Other error
				}
			}

			//Convert to the right types
			if(FAILED(::VariantChangeType(&DataArray[0],&DataArray[0],0,VT_I4)))
				goto ERROR_ENCOUNTERED;
			id = DataArray[0].lVal;
			
			if(FAILED(::VariantChangeType(&DataArray[1],&DataArray[1],0,VT_BSTR)))
				goto ERROR_ENCOUNTERED;
			bstrSoundURL = DataArray[1].bstrVal;

			if(FAILED(::VariantChangeType(&DataArray[2],&DataArray[2],0,VT_BOOL)))
				goto ERROR_ENCOUNTERED;
			bIsLooping = DataArray[2].boolVal;

			if(FAILED(::VariantChangeType(&DataArray[3],&DataArray[3],0,VT_I4)))
				goto ERROR_ENCOUNTERED;
			lVolume = DataArray[3].lVal;

			//Remove any old sound
			if (m_bufferMap[UIEVENTMAP].Lookup(id, SoundBufferPtr) != 0)
			{
				if (SoundBufferPtr != NULL)
				{
					SoundBufferPtr->Stop();
				}
				m_bufferMap[UIEVENTMAP].RemoveKey(id);
				SoundBufferPtr = (IDispatch *) NULL;
			}
			
			// start up the new sound
		
			if (FAILED(LoadSoundBuffer(CString(bstrSoundURL), FALSE,  &SoundBufferPtr.p)))
				goto ERROR_ENCOUNTERED;
			
			SoundBufferPtr->SetVolume(lVolume);	
			
			// stick it in the map and start playing
			m_bufferMap[UIEVENTMAP][id] = SoundBufferPtr;
			Play(SoundBufferPtr, bIsLooping);
			SoundBufferPtr = (IDispatch *) NULL;	

		}
	}
	else if	(CompareElements(&bstrSoundEvent.m_str, &bstrEventName))
	{
		CComVariant varArray;
		long propcnt = 0;
		long lLBound = 0, lUBound = 0;
		long i = 0, l = 0;
		SAFEARRAY* psaArguments = NULL;
		CComVariant DataArray[3]; DataArray[2] = CComVariant((bool)VARIANT_FALSE);
		CComBSTR bstrCommand;
		long id;
		VARIANT_BOOL bIsLooping;

		CSoundBufferPtr	SoundBufferPtr;

		if(FAILED(::VariantCopyInd(&varArray, (VARIANT *)&varArg)))
			goto ERROR_ENCOUNTERED;
		
		// we have a SAFEARRAY
		if (varArray.vt == (VT_VARIANT | VT_ARRAY))
		{
			psaArguments = (SAFEARRAY*)varArray.parray;

			// Verify that the SafeArray is the proper shape.
			if (SafeArrayGetDim(psaArguments) != 1)
			{
				hr = E_INVALIDARG;
				goto ERROR_ENCOUNTERED;
			}

			// Get array bounds    
			hr = SafeArrayGetLBound(psaArguments, 1, &lLBound);     
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			hr = SafeArrayGetUBound(psaArguments, 1, &lUBound);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED; 

			propcnt = lUBound - lLBound + 1;
		}
	
		//extract the data
		if ((psaArguments != NULL) &&
			(propcnt <= 3))
		{
			// Loop through the array retrieving parameters
			for (i = 0, l = lLBound; l <= lUBound; i++, l++)
			{
				CComVariant v;

				hr = SafeArrayGetElement(psaArguments, &l, &v);
				if (FAILED(hr))
					goto ERROR_ENCOUNTERED;   
        
				// Check if optional/variable parameter was passed. 
				// VT_ERROR/DISP_E_PARAMNOTFOUND indicates that the parameter was not passed by the controller.
				if (V_VT(&v) == VT_ERROR && V_ERROR(&v) == DISP_E_PARAMNOTFOUND)
				{
					hr = ResultFromScode(DISP_E_PARAMNOTFOUND);   // No parameter passed
					goto ERROR_ENCOUNTERED;
				}

				// If a parameter was passed by reference, get the value after de-reference.  
				// VBA will pass variables by reference.
				hr = VariantCopyInd(&DataArray[i], &v);   
				if (FAILED(hr))
				{    
					// Parameter not passed. This corresponds to an example like Obj.Add(1, 2, , 4), 
					// where the third parameter is not passed. Ignore and continue.
					if (GetScode(hr) == DISP_E_PARAMNOTFOUND)   
						continue;
					else 
						goto ERROR_ENCOUNTERED;  // Other error
				}
			}

			//Convert to the right types
			if(FAILED(::VariantChangeType(&DataArray[0],&DataArray[0],0,VT_I4)))
				goto ERROR_ENCOUNTERED;
			id = DataArray[0].lVal;
			
			if(FAILED(::VariantChangeType(&DataArray[1],&DataArray[1],0,VT_BSTR)))
				goto ERROR_ENCOUNTERED;
			bstrCommand = DataArray[1].bstrVal;

			if (FAILED(::VariantChangeType(&DataArray[2],&DataArray[2],0,VT_BOOL)))
				goto ERROR_ENCOUNTERED;
			bIsLooping = DataArray[2].boolVal;	
			

			//lookup the sound
			if (m_bufferMap[UIEVENTMAP].Lookup(id, SoundBufferPtr) != 0)
			{
				if (SoundBufferPtr != NULL)
				{
					static CComBSTR bstrStop("Stop");
					static CComBSTR bstrPlay("Play");

					if (CompareElements(&bstrStop.m_str, &bstrCommand.m_str))
						SoundBufferPtr->Stop();
					else
					if (CompareElements(&bstrPlay.m_str, &bstrCommand.m_str))
						Play(SoundBufferPtr, bIsLooping);
				}
			}
		}
	}
	else if	(CompareElements(&bstrEnterRoom.m_str, &bstrEventName))
	{
		ASSERT(varArg.vt == VT_DISPATCH);
		CThingPtr pWhat(varArg.pdispVal);

		hr = HandleOnEnterRoom(pthing, pWhat);
	}
	else if	(CompareElements(&bstrLeaveRoom.m_str, &bstrEventName))
	{
		ASSERT(varArg.vt == VT_DISPATCH);
		CThingPtr pWhat(varArg.pdispVal);

		hr = HandleOnLeaveRoom(pthing, pWhat);
	}

ERROR_ENCOUNTERED:
	return hr; 
};

HRESULT CVwsoundCtrl::HandleOnPropertyChanged(IThing* pthing, BSTR bstr, long lHint, VARIANT varHintData)
{
	HRESULT hr = CVWViewCtrl::HandleOnPropertyChanged(pthing, bstr, lHint, varHintData);
	static CComBSTR bstrPosition("Position");
	static CComBSTR bstrDirection("Direction");
	static CComBSTR bstrSound("Sound");
	static CComBSTR bstrClientSound("ClientSound");

	// don't ignore add/remove events
	//if (lHint == thAdd || lHint == thRemove)
	//	return S_OK;

	if (m_pDSListener == NULL)
		return S_OK;

	CThingPtr pUser;
	CVectorPtr pVector;

	if (m_pWorld == NULL)
		return S_OK;

	m_pWorld->get_User(&pUser.p);

	if (CompareElements(&bstrPosition.m_str, &bstr))
	{
		if (SUCCEEDED(pthing->get_ObjectPropertyExt(bstrPosition, IID_IVector, (IObjectProperty**)&pVector.p)) &&
			(pVector != NULL))
		{
			D3DVALUE V[12];
			pVector->get_x(&(V[0]));
			pVector->get_y(&(V[1]));
			pVector->get_z(&(V[2]));

			long id = 0;
			pthing->get_ID(&id);

			CSoundBufferPtr	SoundBufferPtr;
			for(int i = 0; i< NBUFFERMAPS; i++)
			{
				if (m_bufferMap[i].Lookup(id, SoundBufferPtr) != 0)
				{
					CSound3DBufferPtr Sound3DBufferPtr = SoundBufferPtr;

					if (Sound3DBufferPtr != NULL)
					{
						Sound3DBufferPtr->SetMode(DS3DMODE_NORMAL, DS3D_DEFERRED);
						Sound3DBufferPtr->SetPosition(V[0],V[1],V[2], DS3D_DEFERRED);
					}
				}

			}				

			if (pUser == pthing)
			{
				if (SUCCEEDED(GetListenerPosition(&V[3])))
				{
					m_pDSListener->SetPosition(V[3],V[4],V[5], DS3D_DEFERRED);
					m_pDSListener->SetOrientation(V[6],V[7],V[8], 
												  V[9],V[10],V[11], DS3D_DEFERRED);
				}
				else
				{
					m_pDSListener->SetPosition(V[0],V[1],V[2], DS3D_DEFERRED);
				}
			}
		}
	}
	else if (CompareElements(&bstrDirection.m_str, &bstr))
	{
		if (SUCCEEDED(pthing->get_ObjectPropertyExt(bstrDirection, IID_IVector, (IObjectProperty**)&pVector.p)) &&
			(pVector != NULL))
		{
			D3DVALUE V[12];
			pVector->get_x(&(V[0]));
			pVector->get_y(&(V[1]));
			pVector->get_z(&(V[2]));

			long id= 0;
			pthing->get_ID(&id);

			CSoundBufferPtr	SoundBufferPtr;
			for(int i=0;i<NBUFFERMAPS;i++)
			{
				if (m_bufferMap[i].Lookup(id, SoundBufferPtr) != 0)
				{
					CSound3DBufferPtr Sound3DBufferPtr = SoundBufferPtr;
					if (Sound3DBufferPtr != NULL)
					{
						Sound3DBufferPtr->SetMode(DS3DMODE_NORMAL  ,DS3D_DEFERRED);
						Sound3DBufferPtr->SetConeOrientation(V[0],V[1],V[2], DS3D_DEFERRED);
					}
				}
			}				

			if (pUser == pthing)
			{
				if (SUCCEEDED(GetListenerPosition(&V[3])))
				{
					m_pDSListener->SetPosition(V[3],V[4],V[5], DS3D_DEFERRED);
					m_pDSListener->SetOrientation(V[6],V[7],V[8], 
												  V[9],V[10],V[11],DS3D_DEFERRED);
				}
				else
				{
					m_pDSListener->SetOrientation(V[0],V[1],V[2],0,1,0,DS3D_DEFERRED);
				}
			}

		}
	}
	else if (CompareElements(&bstrSound.m_str, &bstr))
	{
		CComVariant var;

		if (SUCCEEDED(pthing->get_Property(bstrSound, &var)))
		{
			RenderSound(pthing, STANDARDMAP, var, lHint, &varHintData);
		}
	}
	else if (CompareElements(&bstrClientSound.m_str, &bstr))
	{
		CComVariant var;

		if (SUCCEEDED(pthing->get_Property(bstrClientSound, &var)))
		{
			RenderSound(pthing, CLIENTONLYMAP, var, lHint, &varHintData);
		}
	}

	return S_OK;
}

HRESULT CVwsoundCtrl::HandleOnUserConnect(IThing* pthing, BOOL bFromClient)
{
	CVWViewCtrl::HandleOnUserConnect(pthing, bFromClient);

	if (!bFromClient)
		return S_OK;

	HRESULT	hr;
	CThingPtr pUser;

	if (m_pWorld)
	{
		hr = m_pWorld->get_User(&pUser.p);
		if (FAILED(hr))
			goto CLEAN_UP;
		
		if (pUser == pthing)
			RefreshList();
		else if (pthing)
			CheckThingForSound(pthing);
	}

CLEAN_UP:
	return S_OK;
}

HRESULT CVwsoundCtrl::HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient)
{
	CVWViewCtrl::HandleOnUserDisconnect(pthing, bFromClient);

	if (!bFromClient)
		return S_OK;

	HRESULT	hr;
	CThingPtr pUser;

	if (m_pWorld)
	{
		hr = m_pWorld->get_User(&pUser.p);
		if (FAILED(hr))
			goto CLEAN_UP;
		
		if (pUser == pthing)
			ClearAll();
		else if (pthing)
			RemoveThing(pthing);
	}

CLEAN_UP:
	return S_OK;
}

HRESULT CVwsoundCtrl::HandleOnEnterRoom(IThing* pWhere, IThing* pWhat)
{
//	CVWViewCtrl::HandleOnEnterRoom(pWhere, pWhat);

	HRESULT	hr;
	CThingPtr pUser;

	if (m_pWorld)
	{
		hr = m_pWorld->get_User(&pUser.p);
		if (FAILED(hr))
			goto CLEAN_UP;
		
		if (pWhat == pUser)
			RefreshList();
		else if (pWhat)
			CheckThingForSound(pWhat);
	}

CLEAN_UP:
	return hr;
}


HRESULT CVwsoundCtrl::HandleOnLeaveRoom(IThing* pWhere, IThing* pWhat)
{
//	CVWViewCtrl::HandleOnLeaveRoom(pWhere, pWhat);

	HRESULT	hr;
	CThingPtr pUser;

	if (m_pWorld)
	{
		hr = m_pWorld->get_User(&pUser.p);
		if (FAILED(hr))
			goto CLEAN_UP;
		
		if (pWhat == pUser)
			RefreshList();
		else if (pWhat)
			RemoveThing(pWhat);
	}

CLEAN_UP:
	return S_OK;
}

void CVwsoundCtrl::ClearAll()
{
	for(int i = 0; i< NBUFFERMAPS;i++)
	{
		POSITION pos = m_bufferMap[i].GetStartPosition();
		while (pos != NULL)
		{
			long id;
			CSoundBufferPtr SoundBufferPtr;

			m_bufferMap[i].GetNextAssoc(pos, id, SoundBufferPtr);

			if (SoundBufferPtr != NULL)
				SoundBufferPtr->Stop();
		}

		m_bufferMap[i].RemoveAll();
	}
}


void CVwsoundCtrl::RemoveThing(IThing *pThing)
{
	long id = 0;

	// stop the old sound
	// id doesn't fail
	pThing->get_ID(&id);

	CSoundBufferPtr	SoundBufferPtr;
	if (m_bufferMap[STANDARDMAP].Lookup(id, SoundBufferPtr) != 0)
	{
		if (SoundBufferPtr != NULL)
		{
			SoundBufferPtr->Stop();
		}
		m_bufferMap[STANDARDMAP].RemoveKey(id);
	}
}


void CVwsoundCtrl::RefreshList()
{
	ClearAll();

	HRESULT	hr = S_OK;
	CListPtr pContents;
	CThingPtr pContainer,pUser;

	long lItems;
	int	i;

	if (m_pWorld == NULL)
		goto CLEAN_UP;

	hr = m_pWorld->get_User(&pUser.p);
	if (FAILED(hr) || (!pUser))
		goto CLEAN_UP;

	hr = pUser->get_Container(&pContainer.p);
	if (FAILED(hr) || (!pContainer))
		goto CLEAN_UP;

	CheckThingForSound(pContainer);

	hr = pContainer->get_Contents(&pContents.p);
	if (FAILED(hr) || (!pContents))
		goto CLEAN_UP;

	hr = pContents->get_Count(&lItems);
	if (FAILED(hr))
		goto CLEAN_UP;

	for (i = 0; i < lItems; i++)
	{
		VARIANT_BOOL bShow = VARIANT_FALSE;
		CThingPtr pThing;
		hr = pContents->get_Thing(i, &pThing.p);
		if (FAILED(hr))
			goto CLEAN_UP;
		
		if (pThing)
		{
			CheckThingForSound(pThing);
		}
	}
	
CLEAN_UP:;

}

BOOL IsIgnoring(IThing *pUser, IThing *pThing)
{

	CComVariant varResult;
	VARIANT rgvarg[] = {{0,0,0,0,0}, {0,0,0,0,0}};
	DISPPARAMS	dispparams = {rgvarg,NULL,2,0}; 	
	static CComBSTR bstrIgnoring("Ignoring");

	HRESULT hr = S_OK;

	::VariantInit(&(rgvarg[0]));
	::VariantInit(&(rgvarg[1]));

	dispparams.rgvarg[1].vt = VT_DISPATCH;
	dispparams.rgvarg[1].pdispVal = pThing;

	//Are we ignoring it?
	dispparams.rgvarg[0].vt = VT_BSTR;
	dispparams.rgvarg[0].bstrVal = bstrIgnoring.m_str;

	hr = pUser->InvokeMethodExt(CComBSTR("IsInUserProfileList"), &dispparams, &varResult);
	if (SUCCEEDED(hr) &&
		(varResult.vt == VT_BOOL) &&
		(varResult.boolVal == VARIANT_TRUE))
	{
		return TRUE;
	}
	
	return FALSE;

}

void  CVwsoundCtrl::CheckThingForSound(IThing *pThing)
{
	static CComBSTR bstrSound("Sound");
	static CComBSTR bstrClientSound("ClientSound");
	CComVariant var;
	CThingPtr pUser;
	
	if (m_pWorld)
	{
		m_pWorld->get_User(&pUser.p);
		
		if (!IsIgnoring(pUser,pThing))
		{
			if (SUCCEEDED(pThing->get_Property(bstrSound, &var)))
				RenderSound(pThing, STANDARDMAP,var , -1, NULL);
			
			var.Clear();

			if (SUCCEEDED(pThing->get_Property(bstrClientSound, &var)))
				RenderSound(pThing, CLIENTONLYMAP,var , -1, NULL);

		}
	}
}

void  CVwsoundCtrl::RenderSound(IThing *pThing, int iMap, VARIANT var, long lHint, const VARIANT *varHintData)
{
	//static CComBSTR bstrSound("Sound");
	// general data
	
	long	id			= 0;
	CVWSoundPtr pSound = var;
	CSoundBufferPtr	SoundBufferPtr;
	CSound3DBufferPtr Sound3DBufferPtr;

	if (m_bEnable == VARIANT_FALSE)
		return;

	// id doesn't fail
	pThing->get_ID(&id);

	// what do we do?  change the sound, or modify it.
	// change... hint == -1;
	// 2D <--> 3D
	// URL changes

	if ((lHint == -1) ||
		(lHint == thAdd) ||
		(lHint == thPut) ||
		(lHint == thRemove) ||
		(lHint == soundURL) ||	
		(lHint == soundIs3D))
	{	
		// stop the old sound
		if (m_bufferMap[iMap].Lookup(id, SoundBufferPtr) != 0)
		{
			if (SoundBufferPtr != NULL)
			{
				SoundBufferPtr->Stop();
			}
			m_bufferMap[iMap].RemoveKey(id);
			SoundBufferPtr = (IDispatch *) NULL;
		}
		
		// start up the new sound
		if ((m_pDSound != NULL) &&
			(pSound != NULL))
		{	
			CComBSTR bstrSoundURL;
			VARIANT_BOOL bIsLooping,bIs3D;
			long lVolume, lFrequency, lPan;

			if (FAILED(pSound->get_SoundURL(&bstrSoundURL.m_str)))
				return;
			if (FAILED(pSound->get_Is3D(&bIs3D)))
				return;
			if (FAILED(pSound->get_IsLooping (&bIsLooping)))
				return;
			// don't bother to play one shots on enter, only from setSound
			if ((lHint == -1) && (bIsLooping == VARIANT_FALSE))
				return;

			if (FAILED(LoadSoundBuffer(CString(bstrSoundURL), bIs3D == VARIANT_TRUE?TRUE:FALSE,  &SoundBufferPtr.p)))
				return;
			
			if (FAILED(pSound->Get2DDataExt(&lVolume, &lFrequency, &lPan)))
				return;
			// Set the default 2D properties
			SoundBufferPtr->SetPan(lPan);
			SoundBufferPtr->SetVolume(lVolume);
			SoundBufferPtr->SetFrequency(lFrequency);	
			
			// set up the hint to set the 3D data if we need to
			if (bIs3D == VARIANT_TRUE)
			{		
				lHint = sound3DData;
			}
			
			// stick it in the map and start playing
			m_bufferMap[iMap][id] = SoundBufferPtr;
			Play(SoundBufferPtr, bIsLooping);
			SoundBufferPtr = (IDispatch *) NULL;	
		}
	}

	// 
	if (m_bufferMap[iMap].Lookup(id, SoundBufferPtr) == 0)
		return;

	// Change the looping construct
	if ((lHint == soundIsLooping) &&
		(SoundBufferPtr != NULL))
	{
		VARIANT_BOOL bIsLooping;
		pSound->get_IsLooping(&bIsLooping);
		SoundBufferPtr->Stop();
		Play(SoundBufferPtr, bIsLooping);

	}

		// set the 2D parameters;
	if ((lHint == sound2DData) &&
		(SoundBufferPtr != NULL))
	{
		long lVolume, lFrequency, lPan;
		if (FAILED(pSound->Get2DDataExt(&lVolume, &lFrequency, &lPan)))
			return;
		SoundBufferPtr->SetPan(lPan);
		SoundBufferPtr->SetVolume(lVolume);
		SoundBufferPtr->SetFrequency(lFrequency);
	}

	if ((lHint == sound3DData) &&
		(Sound3DBufferPtr = SoundBufferPtr))
	{
		// Set the Positional sound
		static CComBSTR  bstrProps[] = {"Position","Direction"};
		//3D data
		long dwInsideConeAngle;
		long dwOutsideConeAngle;
		long lConeOutsideVolume;
		float flMinDistance;
		float flMaxDistance;

		D3DVALUE V[6];
		int index = 0;

		for(int i = 0;i<2;i++)
		{
			CVectorPtr pVector;
			pThing->get_ObjectProperty(bstrProps[i],(IObjectProperty**)&pVector.p);

			pVector->get_x(&(V[index])); index++;
			pVector->get_y(&(V[index])); index++;
			pVector->get_z(&(V[index])); index++;
		}

		if (FAILED(pSound->Get3DDataExt(&dwInsideConeAngle, &dwOutsideConeAngle,&lConeOutsideVolume,&flMinDistance,&flMaxDistance)))
			return;

		Sound3DBufferPtr->SetMode(DS3DMODE_NORMAL ,DS3D_DEFERRED);
		Sound3DBufferPtr->SetPosition(V[0],V[1],V[2], DS3D_DEFERRED);
		Sound3DBufferPtr->SetConeOrientation(V[3],V[4],V[5],DS3D_DEFERRED);
		Sound3DBufferPtr->SetConeAngles(dwInsideConeAngle,dwOutsideConeAngle,DS3D_DEFERRED); 
		Sound3DBufferPtr->SetConeOutsideVolume(lConeOutsideVolume,DS3D_DEFERRED);
		Sound3DBufferPtr->SetMinDistance(flMinDistance, DS3D_DEFERRED);
		Sound3DBufferPtr->SetMaxDistance(flMaxDistance, DS3D_DEFERRED);

		//REVIEW [ac]:		
		//Only do this on the timer
		//m_pDSListener->CommitDeferredSettings();
		
	}

}

HRESULT CVwsoundCtrl::Play(IDirectSoundBuffer *pBuffer ,VARIANT_BOOL bLoop)
{
	HRESULT hr = S_OK;

	hr = pBuffer->Play(0, 0, (bLoop == VARIANT_TRUE) ? DSBPLAY_LOOPING : 0);

	if (FAILED(hr) && hr == DSERR_BUFFERLOST)
	{
		hr = pBuffer->Restore();
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		//hr = ReloadSoundBuffer(m_strFile, pBuffer);
		//if (FAILED(hr))
		//	goto ERROR_ENCOUNTERED;

		//hr = pBuffer->Play(0, 0, (bLoop == VARIANT_TRUE) ? DSBPLAY_LOOPING : 0);
	}

ERROR_ENCOUNTERED:

	return hr;
}



HRESULT CVwsoundCtrl::Create3D(BSTR bstrFile, IDirectSound3DBuffer **pp3DBuffer)
{
	HRESULT hr = S_OK;
	IDirectSoundBuffer *pdsb = NULL;
	IDirectSound3DBuffer *pdsb3D = NULL;
	CString strFile(bstrFile);

	// init
	*pp3DBuffer = NULL;

	// load the file
	hr = LoadSoundBuffer(strFile, TRUE, &pdsb);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// get 3D interface
	hr = pdsb->QueryInterface(IID_IDirectSound3DBuffer, (void **) &pdsb3D);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// create a 3D sound buffer object


	// return
	*pp3DBuffer = pdsb3D;
	pdsb3D->AddRef();

ERROR_ENCOUNTERED:

	SAFERELEASE(pdsb);
	SAFERELEASE(pdsb3D);

	return hr;

}



HRESULT CVwsoundCtrl::SetPosition(float fPosX, float fPosY, float fPosZ)
{
	HRESULT hr = S_OK;

	if (m_pDSListener == NULL)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	// set pos
	hr = m_pDSListener->SetPosition((D3DVALUE)fPosX, (D3DVALUE)fPosY, (D3DVALUE)fPosZ, DS3D_IMMEDIATE);
	if (FAILED(hr))
	{
		TRACE("CVwsoundCtrl::SetPosition: IDirectSound3DListener::SetPosition failed, hr %x\n", hr);
	}

ERROR_ENCOUNTERED:
	return hr;
}

HRESULT CVwsoundCtrl::SetOrientation(float fDirX, float fDirY, float fDirZ, float fUpX, float fUpY, float fUpZ)
{
	HRESULT hr = S_OK;

	if (m_pDSListener == NULL)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	hr = m_pDSListener->SetOrientation((D3DVALUE)fDirX, (D3DVALUE)fDirY, (D3DVALUE)fDirZ,
									     (D3DVALUE)fUpX, (D3DVALUE)fUpY, (D3DVALUE)fUpZ,
										 DS3D_IMMEDIATE);
	if (FAILED(hr))
	{
		TRACE("CVwsoundCtrl::SetOrientation: IDirectSound3DListener::SetOrientation failed, hr %x\n", hr);
	}

	// set pos
ERROR_ENCOUNTERED:
	return hr;
}

HRESULT CVwsoundCtrl::SetDistanceFactor(float flFactor)
{
	HRESULT hr = S_OK;

	if (m_pDSListener == NULL)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	hr = m_pDSListener->SetDistanceFactor((D3DVALUE) flFactor, DS3D_IMMEDIATE);
	if (FAILED(hr))
	{
		TRACE("CVwsoundCtrl::SetDistanceFactor: IDirectSound3DListener::SetDistanceFactor failed, hr %x\n", hr);
	}

ERROR_ENCOUNTERED:
	return hr;
}

HRESULT CVwsoundCtrl::SetRolloffFactor(float flFactor)
{
	HRESULT hr = S_OK;

	if (m_pDSListener == NULL)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	hr = m_pDSListener->SetRolloffFactor((D3DVALUE) flFactor, DS3D_IMMEDIATE);
	if (FAILED(hr))
	{
		TRACE("CVwsoundCtrl::SetRolloffFactor: IDirectSound3DListener::SetRolloffFactor failed, hr %x\n", hr);
	}

ERROR_ENCOUNTERED:
	return hr;
}

/////////////////////////////////////////////////////////////////////////////
// Internal helper functions

HRESULT CVwsoundCtrl::CleanUp()
{
	
	// iterate through Playing list, seeing if any sounds are done
	for(int i = 0;i<NBUFFERMAPS;i++)
	{
		POSITION pos = m_bufferMap[i].GetStartPosition();

		while (pos != NULL)
		{
			long id;
			DWORD nStatus = 0;
			CSoundBufferPtr SoundBufferPtr;

			m_bufferMap[i].GetNextAssoc(pos, id, SoundBufferPtr);

			if ((SoundBufferPtr != NULL) &&
				SUCCEEDED(SoundBufferPtr->GetStatus(&nStatus)) &&
				!(nStatus & DSBSTATUS_PLAYING))
			{
				m_bufferMap[STANDARDMAP][id] = CSoundBufferPtr((IDirectSoundBuffer*)NULL);
			}
		}
	}

	
	return S_OK;
}

HRESULT CVwsoundCtrl::LoadSoundBuffer(CString& strFile, BOOL b3D, IDirectSoundBuffer **ppbuf)
{
	HRESULT hr = S_OK;
	void *pvMapping = NULL;
	DSBUFFERDESC dsbd = {0};
	BYTE *pbWaveData;

	if (m_pDSound == NULL)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	// open the file
	hr = CreateMapping(strFile, &pvMapping);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// parse it to make sure it's ok
	hr = ParseWAV(	pvMapping, 
					&dsbd.lpwfxFormat, 
					&pbWaveData, 
					&dsbd.dwBufferBytes);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// create a sound buffer
	dsbd.dwSize = sizeof(dsbd);
	//dsbd.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLVOLUME | DSBCAPS_LOCSOFTWARE;
	//dsbd.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLDEFAULT ;
	dsbd.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
	if (b3D)
		dsbd.dwFlags |= DSBCAPS_CTRL3D;

	try
	{
		hr = m_pDSound->CreateSoundBuffer(&dsbd, ppbuf, NULL);
		if (FAILED(hr))
		{
			TRACE("CVwsoundCtrl::LoadSoundBuffer: IDirectSound::CreateSoundBuffer failed, hr %x\n", hr);
			goto ERROR_ENCOUNTERED;
		}

	}
	catch(CException* e)
	{
		ASSERT(FALSE);

		// display error message
		char szError[256];
		if (e->GetErrorMessage(szError, sizeof(szError)))
			OutputDebugString(szError);

		e->Delete();

		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}
	catch(...)
	{
		ASSERT(FALSE);

		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	hr = FillBuffer(*ppbuf, pbWaveData, dsbd.dwBufferBytes);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// bump ref count of buffer since we want to keep it
	(*ppbuf)->AddRef();

ERROR_ENCOUNTERED:

	if (pvMapping != NULL)
		CloseMapping(pvMapping);

	if (*ppbuf != NULL)
		(*ppbuf)->Release();

	return hr;
}

HRESULT CVwsoundCtrl::ReloadSoundBuffer(CString& strFile, IDirectSoundBuffer *pbuf)
{
	HRESULT hr = S_OK;
	void *pvMapping = NULL;
	BYTE *pbWaveData;
	DWORD cbWaveSize;

	hr = CreateMapping(strFile, &pvMapping);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// parse it to make sure it's ok
	hr = ParseWAV(	pvMapping, 
					NULL, 
					&pbWaveData, 
					&cbWaveSize);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuf->Restore();
	if (FAILED(hr))
	{
		//TRACE("CVwsoundCtrl::ReloadSoundBuffer: IDirectSoundBuffer::Restore failed, hr %x\n", hr);
		goto ERROR_ENCOUNTERED;
	}

	hr = FillBuffer(pbuf, pbWaveData, cbWaveSize);

ERROR_ENCOUNTERED:

	if (pvMapping != NULL)
		CloseMapping(pvMapping);

	return hr;
}

HRESULT CVwsoundCtrl::FillBuffer(IDirectSoundBuffer *pbuf, BYTE *pbWaveData, DWORD cbWaveSize)
{
	HRESULT hr = S_OK;
	void *pvMem1;
	DWORD cbMem1;
	void *pvMem2;
	DWORD cbMem2;

	if (pbuf == NULL || pbWaveData == NULL || cbWaveSize == 0)
	{
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}
 
	// lock the buffer
	hr = pbuf->Lock(0,
					cbWaveSize,
					&pvMem1,
					&cbMem1,
					&pvMem2,
					&cbMem2,
					0);
	if (FAILED(hr))
	{
		TRACE("CVwsoundCtrl::FillBuffer: IDirectSoundBuffer::Lock failed, hr %x\n", hr);
		goto ERROR_ENCOUNTERED;
	}

	// copy first part of buffer
	::CopyMemory(pvMem1, pbWaveData, cbMem1);

	// if necessary, copy second part
	if (cbMem2 != 0)
		::CopyMemory(pvMem2, pbWaveData + cbMem1, cbMem2);

	hr = pbuf->Unlock(pvMem1, cbMem1, pvMem2, cbMem2);
	if (FAILED(hr))
	{
		TRACE("CVwsoundCtrl::FillBuffer: IDirectSoundBuffer::Unlock failed, hr %x\n", hr);
	}

ERROR_ENCOUNTERED:

	return hr;
}

HRESULT CVwsoundCtrl::CreateMapping(CString& strFile, void **ppvMapping)
{
	HRESULT hr = S_OK;
	HANDLE hFile;
	HANDLE hMapping;
	IInternetFileManager *pInetfile = NULL;
	VARIANT_BOOL bFound;
	static CComBSTR bstrInetfile("Inetfile");

	if (ppvMapping == NULL)
	{
		hr = E_POINTER;
		goto ERROR_ENCOUNTERED;
	}

	// initialize
	*ppvMapping = NULL;

	// open the file
	if (m_pWorld)
	{
		hr = m_pWorld->get_Tool(bstrInetfile, (IUnknown**)&pInetfile);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;
		
		CComBSTR bstrURLPath, bstrFilePath;
		CComBSTR bstrFile = strFile;

		if (FAILED(hr = pInetfile->FindCachedFile(bstrFile, &bstrURLPath.m_str, &bstrFilePath.m_str, &bFound)) || !bFound)
		{
			if (FAILED(hr = pInetfile->GetFileEx(bstrFile, VARIANT_FALSE, &bstrURLPath.m_str, &bstrFilePath.m_str))) 
			{
				m_pVWClient->Report(CComBSTR(CString("Failed to download file: ") + CString(bstrFile)), -1);
				goto ERROR_ENCOUNTERED;
			}
		}

		strFile = bstrFilePath;
	}

	// open the file
	hFile = CreateFile(	strFile, 
						GENERIC_READ, 
						FILE_SHARE_READ, 
						NULL, 
						OPEN_EXISTING, 
						0, 
						NULL);
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if (hMapping == INVALID_HANDLE_VALUE) 
	{
		CloseHandle(hFile);
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	CloseHandle (hFile);

	*ppvMapping = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
	if (*ppvMapping == NULL) 
	{
		CloseHandle(hMapping);
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	CloseHandle (hMapping);

ERROR_ENCOUNTERED:

	SAFERELEASE(pInetfile);

	return hr;
}

HRESULT CVwsoundCtrl::ParseWAV(	void *			pvMapping,
							WAVEFORMATEX **	ppWaveFormat,
							BYTE **			ppbWaveData,
							DWORD *			pcbWaveSize)
{
	HRESULT hr = S_OK;
	DWORD *pdw;
	DWORD *pdwEnd;
	DWORD dwRiff;
	DWORD dwType;
	DWORD dwLength;

	if (ppWaveFormat)
		*ppWaveFormat = NULL;

	if (ppbWaveData)
		*ppbWaveData = NULL;

	if (pcbWaveSize)
		*pcbWaveSize = 0;

	pdw = (DWORD *)pvMapping;
	dwRiff = *pdw++;
	dwLength = *pdw++;
	dwType = *pdw++;

	if (dwRiff != mmioFOURCC('R', 'I', 'F', 'F'))
	{
		// not even RIFF
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	if (dwType != mmioFOURCC('W', 'A', 'V', 'E'))
	{
		// not a WAV
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	pdwEnd = (DWORD *)((BYTE *)pdw + dwLength-4);

	while (pdw < pdwEnd)
	{
		dwType = *pdw++;
		dwLength = *pdw++;

		switch (dwType)
		{
			case mmioFOURCC('f', 'm', 't', ' '):
				if (ppWaveFormat && !*ppWaveFormat)
				{
					if (dwLength < sizeof(WAVEFORMAT))
					{
						// not a proper WAV
						hr = E_FAIL;
						goto ERROR_ENCOUNTERED;
					}

					*ppWaveFormat = (WAVEFORMATEX *)pdw;

					if ((!ppbWaveData || *ppbWaveData) &&
						(!pcbWaveSize || *pcbWaveSize))
					{
						// done (no error)
						goto ERROR_ENCOUNTERED;
					}
				}
				break;

			case mmioFOURCC('d', 'a', 't', 'a'):
				if ((ppbWaveData && !*ppbWaveData) ||
					(pcbWaveSize && !*pcbWaveSize))
				{
					if (ppbWaveData)
						*ppbWaveData = (LPBYTE)pdw;

					if (pcbWaveSize)
						*pcbWaveSize = dwLength;

					if (!ppWaveFormat || *ppWaveFormat)
					{
						// done (no error)
						goto ERROR_ENCOUNTERED;
					}
				}
				break;
		}

		// advance to next 
		pdw = (DWORD *)((BYTE *)pdw + ((dwLength+1)&~1));
	}

ERROR_ENCOUNTERED:

	return hr;
}

HRESULT CVwsoundCtrl::CloseMapping(void *pvMapping)
{
	HRESULT hr = S_OK;

	if (!UnmapViewOfFile(pvMapping))
		hr = E_FAIL;

	return hr;
}


void CVwsoundCtrl::OnTimer(UINT nIDEvent) 
{
	CVWViewCtrl::OnTimer(nIDEvent);
	switch (nIDEvent)
	{
	case 1:
		if (m_pDSListener)
		{
			float v[9] = {0,0,0,0,0,0,0,0,0};
			if (SUCCEEDED(GetListenerPosition(v)))
			{
				m_pDSListener->SetPosition(v[0],v[1],v[2], DS3D_DEFERRED);
				m_pDSListener->SetOrientation(v[3],v[4],v[5], 
											  v[6],v[7],v[8],DS3D_DEFERRED);
			}
			m_pDSListener->CommitDeferredSettings();	
		}
	break;
	default:
	break;
	}
}

HRESULT CVwsoundCtrl::GetRoomFrame(IVWFrame** ppRoomFrame)
{
	HRESULT hr = E_FAIL;
	CThingPtr pUser, pRoom;
	IObjectProperty *pProp = NULL;
	CVWGeometryPtr GeomPtr;
	static CComBSTR bstrGeometry("Geometry");

	if (!m_pWorld)
		goto ERROR_ENCOUNTERED;

	hr = m_pWorld->get_User(&pUser.p);
	if (FAILED(hr) || pUser == NULL)
		goto ERROR_ENCOUNTERED;

	hr = pUser->get_Container(&pRoom.p);
	if (FAILED(hr) || pRoom == NULL)
		goto ERROR_ENCOUNTERED;

	hr =pRoom->get_ObjectProperty(bstrGeometry, &pProp);
	if (FAILED(hr) || pProp == NULL)
		goto ERROR_ENCOUNTERED;

	GeomPtr = pProp;
	if (GeomPtr == NULL)
		hr = E_FAIL;

	hr = GeomPtr->get_Frame(ppRoomFrame);
	if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	SAFERELEASE(pProp);
	return hr;
}
HRESULT CVwsoundCtrl::GetListenerPosition(float *rgfl)
{
	HRESULT hr = E_FAIL;
	if (m_pRenderViewOCX)
	{
		CVWFramePtr pCamera, pRoomFrame;

		if (m_RenderRootPtr == NULL)
		{
			DISPID dispid;
			static CComBSTR bstrName("VWRenderRoot");
			DISPPARAMS dp = { NULL,NULL, 0,0 };
			EXCEPINFO ei;
			UINT	uerr;
			CComVariant varResult;

			hr = m_pRenderViewOCX->GetIDsOfNames(IID_NULL,&bstrName.m_str,1, LOCALE_SYSTEM_DEFAULT, &dispid);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			hr = m_pRenderViewOCX->Invoke(dispid, IID_NULL,
					LOCALE_SYSTEM_DEFAULT,
					DISPATCH_PROPERTYGET,
					&dp,
					&varResult,
					&ei,&uerr);
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;

			m_RenderRootPtr = varResult;
			if (m_RenderRootPtr == NULL)
				goto ERROR_ENCOUNTERED;
		}

		hr = GetRoomFrame(&pRoomFrame.p);
		if (FAILED(hr) || pRoomFrame == NULL)
			goto ERROR_ENCOUNTERED;

		hr = m_RenderRootPtr->get_Camera(&pCamera.p);
		if (FAILED(hr) || pCamera.p == NULL)
			goto ERROR_ENCOUNTERED;

		hr = pCamera->GetPositionEx(pRoomFrame, &rgfl[0],&rgfl[1],&rgfl[2]);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		hr = pCamera->GetOrientationEx(pRoomFrame, &rgfl[3],&rgfl[4],&rgfl[5],
													&rgfl[6],&rgfl[7],&rgfl[8]);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

	}
ERROR_ENCOUNTERED:

	return hr;
}


LPDISPATCH CVwsoundCtrl::GetRenderRoot() 
{
	SAFEADDREF(m_pRenderViewOCX);
	
	return m_pRenderViewOCX;
}

void CVwsoundCtrl::SetRenderRoot(LPDISPATCH newValue) 
{
	SAFERELEASE(m_pRenderViewOCX);
	m_pRenderViewOCX = newValue;
	SAFEADDREF(m_pRenderViewOCX);
	m_RenderRootPtr = (IDispatch *)NULL;
}

BOOL CVwsoundCtrl::GetEnable() 
{
	return m_bEnable == VARIANT_TRUE? TRUE:FALSE;
}

void CVwsoundCtrl::SetEnable(BOOL bNewValue) 
{
	m_bEnable = bNewValue? VARIANT_TRUE: VARIANT_FALSE;

	if (m_bEnable == VARIANT_TRUE)
	{
		RefreshList();
	}
	else
	{
		ClearAll();
	}
	SetModifiedFlag();
}

