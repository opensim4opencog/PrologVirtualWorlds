// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.


// VWNavT3D.cpp : Implementation of CVWNvToolApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>

#include <math.h>
//#include <afx.h>
#include <d3drmwin.h>
#include <VWNvTool.h>
#include <VWPkTool.h>
#include "VWNavT3D.h"
#include <menuitem.h>
#include <vwidata.h>

#define IID_DEFINED
#include <vwuiobjs_i.c>
#include <vwanim_i.c>
#include <vwutils.h>


// Navigation Constants
#define PI						3.1415927F
#define DEG2RAD(d)				(d*PI/180.0F)

// General Navigation Constants
#define fAccelScale				(1.0f/0.5f)		// it takes 0.5 seconds to accelerate to max velocity 
#define fSpeedScale				8.0f			// Scale used for all velocities.
#define fTerminalVelocity		5.0F			// Maximum velocity when falling.
#define nPixelsOfMouseArea		400
#define fUnitsPerAdjustedPixel	(1.f/(float(nPixelsOfMouseArea - nPixelsOfStillArea)))
#define dwMaxTimeDelta			250				// maximum of 250ms (4fps) of dead reckoning 
#define dwTimeAccelThreshold	250				// time in ms to accelerate to high gear 
#define IsPressed(key)			(GetAsyncKeyState(key) < 0)
#define CLAMP(x, xmin, xmax)	if ((x) < (xmin)) (x) = (xmin); else if ((x) > (xmax)) (x) = (xmax)
#define UPDATEMIN(dst, src)		if ((src) < (dst)) (dst) = (src)
#define INRANGE(x, xmin, xmax) 	(((x) >= (xmin)) && ((x) <= (xmax)))
#define fOffsetFromFloor		1.0F			// Allows walking up stairs etc. (leg height).
#define fOffsetFromWalls		0.01F			// Allows for numeric errors at intersections.

// Avatar Navigation Constants
#define fTurboScale				2.5f			// Maximum (running) acceleration multiplier.
#define fRotScale				DEG2RAD(105.f)	// it takes at least 1 second to turn 105 degrees 
#define fLowSpeedScale			0.36f			// speed factor in keyboard low gear 

// Camera Navigation Constants
#define fPanScale				DEG2RAD(90.f)	// It takes at least 1 second to turn the camera 90 degrees 
#define fTiltScale				DEG2RAD(90.f)	// It takes at least 1 second to look up or down 90 degrees
#define fRelativeZoomScale		1.0f			// Speed of Zoom
#define fCameraNavScale			12.0f			// General speed of camera navigation

// picking constants
#define cxHitTolerance			3
#define cyHitTolerance			3

D3DVECTOR	d3dvXAxis = {1.0F, 0.0F, 0.0F};
D3DVECTOR	d3dvYAxis = {0.0F, 1.0F, 0.0F};
D3DVECTOR	d3dvZAxis = {0.0F, 0.0F, 1.0F};


/////////////////////////////////////////////////////////////////////////////
CComBSTR    bstrEditingMode("EditingMode");
/////////////////////////// ViewMode enumeration ////////////////////////
//The view mode is set by one of four mutually exclusive properties:
//OrthogonalMode, TopOrthogonalMode, FrontOrthogonalMode, RightOrthogonalMode.
enum _ViewModeEnum
{
	PERSPECTIVE = 0,
	TOP,
	FRONT,
	RIGHT,
} ViewModeEnum;


/////////////////////////////////////////////////////////////////////////////
//Zoomlevel info
const IID DIID__RenderingControl = { 0x44fac781, 0xca4, 0x11d0, { 0x89, 0xa9, 0, 0xa0, 0xc9, 0x5, 0x41, 0x29 } };
#define didZoomLevelProperty 0x7
#define didResetCameraPosition 12L
/////////////////////////////////////////////////////////////////////////////
//
// Local helper functions.

// Function: ElapsedFrameTime
//    Return the time, in seconds, that has elasped since the last call to this function
float
ElapsedFrameTime()
{
	static DWORD dwTimeThisFrame = 0;
	static DWORD dwTimeLastFrame;

	// update the frame times
	dwTimeLastFrame = dwTimeThisFrame;
	dwTimeThisFrame = GetTickCount();

	DWORD dwElapsedTime = dwTimeThisFrame - dwTimeLastFrame;
	
	// make sure the elapsed time is always sensible
	CLAMP(dwElapsedTime, 0, dwMaxTimeDelta);

	return float(dwElapsedTime) * 0.001f;	// in seconds...
}

// Function: IsTimeToAccel
//    If the user has been moving the avatar with the keyboard without pause for a while,
//  kick into high-speed mode.  Slow mode lets the user make fine-tune adjustments.
static inline BOOL 
IsTimeToAccel(BOOL bMoving)
{
	static DWORD dwStartNavTime = 0;
	
	if (bMoving) {
		if (dwStartNavTime == 0) 
			dwStartNavTime = GetTickCount();
		else
			return ((GetTickCount() - dwStartNavTime) > dwTimeAccelThreshold);
	} else
		dwStartNavTime = 0;

	return FALSE;
}

// Function: KeyboardNavVector
//    Return a Vector2 in range: [(-1.f, -1.f), (1.f, 1.f)]
Vector2 
CVWNavigationTool3D::KeyboardNavVector()
{
	int		nDX, nDY;

	nDX	=	(IsPressed(m_KeyBindings[kbRight]) - IsPressed(m_KeyBindings[kbLeft]));
	nDY	=	(IsPressed(m_KeyBindings[kbUp])	- IsPressed(m_KeyBindings[kbDown]));

	Vector2 v2Nav = Vector2(float(nDX), float(nDY));
	
	return v2Nav;
}

// Function: MouseResponse
//    Pipe mouse displacement thru a piecewise linear response curve...
static float 
MouseResponse(int nDisp)
{
	BOOL bNeg;

	if (bNeg = (nDisp < 0)) 
		nDisp = -nDisp;

	// provide square around anchor point where velocity goes to zero
	nDisp -= nPixelsOfStillArea;

	CLAMP(nDisp, 0, nPixelsOfMouseArea);

	return float(bNeg ? -nDisp : nDisp) * fUnitsPerAdjustedPixel;
}

// Function: MouseNavVector
//    Return a Vector2 in range: [(-1.f, -1.f), (1.f, 1.f)]
static inline Vector2 
MouseNavVector(CSize &vnMouseDisp)
{
	// the "y" response is negative because the 0th pixel row is at the top
	return Vector2(MouseResponse(vnMouseDisp.cx), -MouseResponse(vnMouseDisp.cy));
}

// Function: AccelVelocity
//    Given an old velocity, a desired velocity, and maximum acceleration,
//  compute a new velocity.
static inline float
AccelVelocity(float fOldVel, float fDesiredVel, float fAccel)
{
	float fDelta = fDesiredVel - fOldVel;
	if (fDelta < 0.f) {
		fAccel = (-fAccel > fDelta ? -fAccel : fDelta);
	} else {
		UPDATEMIN(fAccel, fDelta);
	}
	return fOldVel + fAccel;
}

BOOL GetMapElementByTime(IPropertyMap *pMap, DWORD dwTimeElapsed, DWORD &dwElementTime, COleVariant &varElement)
{
	BSTR			bstrTime = NULL;
	long			lCount;
	DWORD			dwTime;
	VARIANT_BOOL	bLastItem;
	COleVariant		var;

	dwElementTime = 0;

	if (FAILED(pMap->get_Count(&lCount)))
		return FALSE;
	if (lCount > 0)
	{
		if (FAILED(pMap->FirstItem(&bstrTime, &var, &bLastItem)))
			return FALSE;
		do
		{
			dwTime = atol((const char *)CString(bstrTime));
			if (dwTimeElapsed < dwTime &&
				(dwElementTime == 0 || dwTime < dwElementTime))
			{
				varElement.Clear();
				dwElementTime = dwTime;
				varElement = var;
			}
			var.Clear();
			SAFEFREESTRING(bstrTime);
			if (!bLastItem)
			{
				if (FAILED(pMap->NextItem(&bstrTime, &var, &bLastItem)))
					return FALSE;
			}
		} 
		while (bLastItem == VARIANT_FALSE);
	}
	return dwElementTime == 0 ? FALSE : TRUE;
}

float InterpolatePos(DWORD dwSourceTime, DWORD dwDestinationTime, float fSource, float fDestination)
{
	return fSource + 
		(((float)(GetTickCount()-dwSourceTime)/(dwDestinationTime-dwSourceTime))*(fDestination-fSource));
}

float InterpolateDir(DWORD dwSourceTime, DWORD dwDestinationTime, float fSource, float fDestination)
{
	if (fDestination-fSource > PI)
	{
		return fSource -
			(((float)(GetTickCount()-dwSourceTime)/(dwDestinationTime-dwSourceTime))*(2*PI-(fDestination-fSource)));
	}
	else if (fSource-fDestination > PI)
	{
		return fSource +
			(((float)(GetTickCount()-dwSourceTime)/(dwDestinationTime-dwSourceTime))*(2*PI-(fSource-fDestination)));
	}
	else
		return fSource +
			(((float)(GetTickCount()-dwSourceTime)/(dwDestinationTime-dwSourceTime))*(fDestination-fSource));
}

static void ConvertRotationToDir(float fRotation, float &fDirX, float &fDirY, float &fDirZ,
								 D3DVECTOR	rlvPreDef, D3DVECTOR rlvAxis)
{
	D3DVECTOR rlvDir;

	// And finally rotate the vector;
	D3DRMVectorRotate(&rlvDir, &rlvPreDef, &rlvAxis, fRotation);

	fDirX = rlvDir.x;
	fDirY = rlvDir.y;
	fDirZ = rlvDir.z;
}

static void ConvertDirToRotation(float fDirX, float fDirY, float fDirZ, float &fRotation)
{
	D3DVECTOR rlvDir;
	D3DVALUE  valCosTheta;

	rlvDir.x = fDirX;
	rlvDir.y = fDirY;
	rlvDir.z = fDirZ;

	D3DRMVectorNormalize(&rlvDir);

	// First need angle between the pre-defined angle and rlvDir.
	valCosTheta = D3DRMVectorDotProduct(&d3dvZAxis, &rlvDir);
	fRotation = (D3DVALUE)acos(valCosTheta);

	// This will always be the acute angle. Since rotation will always be in positive direction we must
	// give correct angle (possibly obtuse) for that direction.
	if (rlvDir.x < 0.0F)
	{
		// Acute angle will not work, need obtuse angle.
		fRotation = (2*PI) - fRotation;
	}
}

/////////////////////////////////////////////////////////////////////////////
//

CVWNavigationTool3D::CVWNavigationTool3D()
{
	m_bTrackingMouse = VARIANT_FALSE;
	m_vnStoredMouseDisp = m_vnMouseDisp = CSize(0, 0);
	m_ptActualMousePos = CPoint(0,0);
	m_v2CurVel = Vector2(0.f, 0.f);
	m_v2CurCVel = Vector2(0.f, 0.f);
	m_bMoving = VARIANT_FALSE;
	m_pmapPositions = NULL;
	m_pmapDirections = NULL;
	m_bNavigationPath = VARIANT_FALSE;
	m_pvwfTransform = NULL;
	m_pvwfParent = NULL;
	m_pThing = NULL;
	m_pHBAnimation = NULL;
	m_pVWRenderRoot = NULL;
	m_bCameraMode = VARIANT_FALSE;

	m_nViewMode = PERSPECTIVE;
//	m_bOrthogonalMode = VARIANT_FALSE;
	m_bFalling = VARIANT_FALSE;
	m_bGravityOn = VARIANT_TRUE;
	m_bCollisionDetection = VARIANT_TRUE;
	m_bGotFocus = VARIANT_FALSE;
	m_bKeyboardNavEnabled = VARIANT_FALSE;
	m_bAllNavigationDisabled = VARIANT_FALSE;
	m_piRenderViewControl = NULL;

	InitializeCriticalSection(&m_CS);
	
	// init the keybindings
	m_KeyBindings[kbCamera] = NULL;
	m_KeyBindings[kbZoomIn] = VK_ADD;
	m_KeyBindings[kbZoomOut] = VK_SUBTRACT;
	
	m_KeyBindings[kbTiltUp]	= VK_PRIOR;
	m_KeyBindings[kbTiltDown] = VK_NEXT;
	m_KeyBindings[kbReset] = VK_HOME;

	m_KeyBindings[kbRight] = VK_RIGHT;
	m_KeyBindings[kbLeft] = VK_LEFT;
	m_KeyBindings[kbUp] = VK_UP;
	m_KeyBindings[kbDown] = VK_DOWN;

	SetCursor(m_pUIView, LoadCursor(NULL, IDC_ARROW) );
}

CVWNavigationTool3D::~CVWNavigationTool3D()
{
	SAFERELEASE(m_piRenderViewControl);
	SAFERELEASE(m_pvwfTransform);
	SAFERELEASE(m_pvwfParent);
	SAFERELEASE(m_pThing);
	SAFERELEASE(m_pHBAnimation);
	SAFERELEASE(m_pVWRenderRoot);
	DeleteCriticalSection(&m_CS);
}

BOOL CVWNavigationTool3D::OnLButtonDown(unsigned int nFlags, POINT point )
{
	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;
	VARIANT	varResult;
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};

	// Review: This is UITool order-dependent. Important?

//	if (!(*pbHandled)) {
	if (!m_bTrackingMouse && m_pUIView) {
			// start mouse navigation mode
			m_bTrackingMouse = VARIANT_TRUE;
			m_ptAnchor = point;
			m_ptActualMousePos = point;
			m_vnMouseDisp = CSize(0, 0);
			m_vnStoredMouseDisp = CSize(0, 0);

			// Ensure all future mouse input gets sent to this window.
			HWND hWnd;
			m_pUIView->get_HWND(&hWnd);
			SetCapture(hWnd);

			VariantInit(&varResult);

			//Get the control's ZoomLevel property 
			hr = m_piRenderViewControl->Invoke(didZoomLevelProperty, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &dispparamsNoArgs, &varResult, NULL, NULL);
			if (SUCCEEDED(hr))
			{
				m_fZoomLevel = varResult.fltVal;
			}

			VariantClear(&varResult);

			ShowCursor(FALSE);
	}
	

	return bHandled;
}

BOOL CVWNavigationTool3D::OnLButtonUp( unsigned int nFlags, POINT point )
{
	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;

	if (m_bTrackingMouse && m_pUIView)
	{
		// Set the mouse cursor to where it would be if we hadn't been mucking with it.
		HWND hWnd;
		m_pUIView->get_HWND(&hWnd);
		ClientToScreen(hWnd, &m_ptActualMousePos);
		SetCursorPos(m_ptActualMousePos.x, m_ptActualMousePos.y);
		m_bTrackingMouse = VARIANT_FALSE;

		ShowCursor(TRUE);

		// Stop all future mouse input getting sent to this window.
		ReleaseCapture();
	}
	return bHandled;
}

BOOL CVWNavigationTool3D::OnMouseMove(unsigned int nFlags, POINT point)
{
	BOOL bHandled = FALSE;

	if (m_bTrackingMouse && NULL != m_pUIView)
	{
		// Calculate total displacement
		CSize vnDisp(CPoint(point) - m_ptAnchor);

		CRect	rect;
		HWND	hWnd;
		
		// Keep track of the actual mouse position (where it would be if we weren't mucking
		// with it).
		m_ptActualMousePos += CPoint((m_vnStoredMouseDisp + vnDisp) - m_vnMouseDisp);
		m_pUIView->get_HWND(&hWnd);
		ClientToScreen(hWnd, &m_ptActualMousePos);
		GetWindowRect(GetDesktopWindow(), &rect);
		if (m_ptActualMousePos.x < 0)
			m_ptActualMousePos.x = 0;
		else if (m_ptActualMousePos.x > rect.right)
			m_ptActualMousePos.x = rect.right;
		if (m_ptActualMousePos.y < 0)
			m_ptActualMousePos.y = 0;
		else if (m_ptActualMousePos.y > rect.bottom)
			m_ptActualMousePos.y = rect.bottom;
		ScreenToClient(hWnd, &m_ptActualMousePos);

		// Keep track of the actual mouse displacement (the value needed to drive 
		// navigation).
		m_vnMouseDisp = m_vnStoredMouseDisp + vnDisp;

		// Make sure the hidden cursor stays in the client rect, so it is not
		// limited by the screen edge
		m_pUIView->get_HWND(&hWnd);
		GetClientRect(hWnd, &rect);
		if (!(INRANGE(point.x, 0, rect.right) && INRANGE(point.y, 0, rect.bottom)))
		{
			// Update the stored mouse displacement (to keep track of the displacement
			// lost when we keep moving the cursor back to the anchor point).
			m_vnStoredMouseDisp += vnDisp;

			// Set the cursor position back to the anchor point
			CPoint ptScreen(m_ptAnchor);
			ClientToScreen(hWnd, &ptScreen);
			SetCursorPos(ptScreen.x, ptScreen.y);
		}
	}
	return bHandled;
}

BOOL CVWNavigationTool3D::OnKeyUp( UINT nVirtKey, LPARAM lKeyData )
{
	BOOL	bHandled = FALSE;
	IWorld*						pWorld = NULL;
	IThing*						pGlobal = NULL;
	HRESULT						hr = S_OK;
	long						nEditingMode;

	if (nVirtKey == m_KeyBindings[kbCamera] )
	{
		// Get the world via the thing before we release the thing.
		if (FAILED(hr = m_pThing->get_World(&pWorld)))
			goto ERROR_ENCOUNTERED;

		if (FAILED(hr = pWorld->get_Global(&pGlobal)))
			goto ERROR_ENCOUNTERED;

		if (FAILED(hr = pGlobal->get_Long(bstrEditingMode, &nEditingMode)))
			goto ERROR_ENCOUNTERED;

		if (!nEditingMode)
			m_bCameraMode = !m_bCameraMode;
	}

ERROR_ENCOUNTERED:
	SAFERELEASE(pWorld);
	SAFERELEASE(pGlobal);

	return bHandled;
}

STDMETHODIMP CVWNavigationTool3D::Update()
{
	float	fTime;
	BOOL	bHandled = FALSE;
	HRESULT	hr = S_OK;

	if (m_bAllNavigationDisabled)
		return hr;
	
	EnterCriticalSection(&m_CS);

	fTime = ElapsedFrameTime();

	m_bMoving = FALSE;

	if (!m_bCameraMode)
	{
		if (FAILED(hr = NavigateTarget(fTime)))
			goto ERROR_ENCOUNTERED;
	}

	if (FAILED(hr = NavigateCamera(fTime)))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:

	LeaveCriticalSection(&m_CS);

	return hr;
}

BOOL CVWNavigationTool3D::OnMessage( UINT nIDEvent, WPARAM wParam, LPARAM lParam )
{
	HRESULT hr = S_OK;
	BOOL bHandled = FALSE;

	if ((nIDEvent == WM_KILLFOCUS || nIDEvent == WM_ENTERMENULOOP))
	{
		if (m_bTrackingMouse && m_pUIView)
		{
			// Set the mouse cursor to where it would be if we hadn't been mucking with it.
			HWND hWnd;
			m_pUIView->get_HWND(&hWnd);
			ClientToScreen(hWnd, &m_ptActualMousePos);
			SetCursorPos(m_ptActualMousePos.x, m_ptActualMousePos.y);

			m_bTrackingMouse = VARIANT_FALSE;
			ShowCursor(TRUE);

			// Stop all future mouse input getting sent to this window.
			ReleaseCapture();
		}
		if (nIDEvent == WM_ENTERMENULOOP && m_bKeyboardNavEnabled)
			m_bKeyboardNavEnabled = VARIANT_FALSE;
		m_bGotFocus = VARIANT_FALSE;
	}
	else if (nIDEvent == WM_EXITMENULOOP)
	{
		HWND hWnd, hWndCurrent;
		m_pUIView->get_HWND(&hWnd);
		hWndCurrent = GetFocus();
		if (hWnd == hWndCurrent)
			m_bGotFocus = VARIANT_TRUE;
	}
	else if (nIDEvent == WM_SETFOCUS)
		m_bGotFocus = VARIANT_TRUE;
	return bHandled;
}

STDMETHODIMP CVWNavigationTool3D::NavigateTarget(float fTime)
{
	IPropertyList		*pIDataList = NULL;
	IVWAnimator			*pVWAnimator = NULL;
	IVWAnimation		*pVWAnimation = NULL;
	IWorld				*pWorld = NULL;
	VARIANT_BOOL		bTranslating = VARIANT_FALSE, bRotating = VARIANT_FALSE,
						bNavigationPath;
	Vector3				vLastPos, vPos, vDir, vUp;//(0.0F, 1.0F, 0.0F);
	static CComBSTR		bstrAnimator("Animator");
	HRESULT				hr = S_OK;

	if (!m_pvwfTransform)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	bNavigationPath = m_bNavigationPath;
	if (!bNavigationPath)
	{
		if (FAILED(hr = CalcVelocityAndPosition(fTime, &vLastPos, &vPos, &vDir, &bTranslating, &bRotating)))
			goto ERROR_ENCOUNTERED;
	}
	else
	{
		if (FAILED(hr = CalculateLocationOnPath(&vPos, &vDir, &bTranslating, &bRotating)))
			goto ERROR_ENCOUNTERED;
	}

	// translate
	if (bTranslating)
	{
		if (m_bCollisionDetection && !bNavigationPath)
		{
			//Move and test for boundary collisions
			if (FAILED(hr = m_pvwfTransform->MoveToPosition(m_pvwfParent, vPos.x, vPos.y, vPos.z, VARIANT_FALSE, &pIDataList)))
				goto ERROR_ENCOUNTERED;

			if (pIDataList)
			{
				if (FAILED(hr = HandleIntersections(pIDataList, vLastPos, vPos)))
					goto ERROR_ENCOUNTERED;
			}
		}
		else //Ignore boundary collisions and just set the position.
		{
			if (FAILED(hr = m_pvwfTransform->SetPositionEx(m_pvwfParent, vPos.x, vPos.y, vPos.z)))
				goto ERROR_ENCOUNTERED;
			if (bNavigationPath && !m_bNavigationPath)
			{
				// We've just finished a navigation path. Make sure we've got our feet on 
				// the ground (if necessary).
				if (m_bGravityOn && FAILED(hr = Drop(vPos.x, vPos.y, vPos.z)))
					goto ERROR_ENCOUNTERED;
				// Invoke done event (if any).
				if (m_bstrDoneEvent.Length() != 0)
				{
					if (FAILED(m_pThing->get_World(&pWorld)))
						goto ERROR_ENCOUNTERED;
					if (FAILED(hr = pWorld->get_Tool(bstrAnimator, (IUnknown**)&pVWAnimator)) || !pVWAnimator)
						goto ERROR_ENCOUNTERED;
					// Invoke the method on the main thread (via the animator) 
					// so we don't hold up this thread!
					if (FAILED(hr = pVWAnimator->CreateExt(0, m_pThing, m_bstrDoneEvent, NULL, &pVWAnimation)))
						goto ERROR_ENCOUNTERED;
					m_bstrDoneEvent.Empty();
				}
			}
		}
	}

	// rotate
	if (bRotating)
	{
		//Review: Leave commented out until approach issue is resolved.  MM
		Vector3 vTmpDir;

		if (FAILED(hr = m_pvwfTransform->GetOrientationEx(m_pvwfParent, &vTmpDir.x, &vTmpDir.y, &vTmpDir.z, &vUp.x, &vUp.y, &vUp.z)))
			goto ERROR_ENCOUNTERED;

		if (FAILED(hr = m_pvwfTransform->SetOrientationEx(m_pvwfParent, vDir.x, vDir.y, vDir.z, vUp.x, vUp.y, vUp.z)))
			goto ERROR_ENCOUNTERED;
	}

	m_bMoving = bTranslating || bRotating;

ERROR_ENCOUNTERED:

	SAFERELEASE(pIDataList);
	SAFERELEASE(pVWAnimator);
	SAFERELEASE(pVWAnimation);
	SAFERELEASE(pWorld);

	return hr;
}

STDMETHODIMP CVWNavigationTool3D::FireIntersectEvent(float fP1X, float fP1Y, float fP1Z, float fP2X, float fP2Y, float fP2Z, IPropertyList *pIDataList)
{
	IVWAnimator			*pVWAnimator = NULL;
	IVWAnimation		*pVWAnimation = NULL;
	IWorld				*pWorld = NULL;
	DISPPARAMS			dispparams;
	static CComBSTR		bstrAnimator("Animator");
	static CComBSTR		bstrFireIntersectEvent("FireIntersectEvent");
	HRESULT hr = S_OK;

	// Initialize dispparams so TerminateDispparams won't fail if not used.
	dispparams.rgvarg = NULL;

	if (FAILED(m_pThing->get_World(&pWorld)))
		goto ERROR_ENCOUNTERED;

	// Get the global animator tool we need from the world.
	if (FAILED(hr = pWorld->get_Tool(bstrAnimator, (IUnknown**)&pVWAnimator)) || !pVWAnimator)
		goto ERROR_ENCOUNTERED;

	// Package up args into DISPPARAMS struct.
	if (FAILED(hr = InitializeDISPPARAMS(&dispparams, 8)))
		goto ERROR_ENCOUNTERED;
	dispparams.rgvarg[7].vt = VT_DISPATCH;
	dispparams.rgvarg[7].pdispVal =	m_pvwfParent;
	dispparams.rgvarg[6].vt = VT_R4;
	dispparams.rgvarg[6].fltVal = fP1X;
	dispparams.rgvarg[5].vt = VT_R4;
	dispparams.rgvarg[5].fltVal = fP1Y;
	dispparams.rgvarg[4].vt = VT_R4;
	dispparams.rgvarg[4].fltVal = fP1Z;
	dispparams.rgvarg[3].vt = VT_R4;
	dispparams.rgvarg[3].fltVal = fP2X;
	dispparams.rgvarg[2].vt = VT_R4;
	dispparams.rgvarg[2].fltVal = fP2Y;
	dispparams.rgvarg[1].vt = VT_R4;
	dispparams.rgvarg[1].fltVal = fP2Z;
	dispparams.rgvarg[0].vt = VT_DISPATCH;
	dispparams.rgvarg[0].pdispVal =	pIDataList;
	// Invoke the method on the intersecter which will deal with invoking all the 
	// necessary intersect event handlers. Invoke the method on the main thread
	// (via the animator) so we don't hold up this thread!
	if (FAILED(hr = pVWAnimator->CreateExt(0, m_pThing, bstrFireIntersectEvent, &dispparams, &pVWAnimation)))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:

	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	SAFERELEASE(pVWAnimator);
	SAFERELEASE(pVWAnimation);
	SAFERELEASE(pWorld);

	return hr;
}

STDMETHODIMP CVWNavigationTool3D::NavigateCamera(float fTime)
{
	IVWFrame			*pvwfCamera = NULL, *pvwfParent = NULL;
	IDirect3DRMFrame	*pd3drmfCamera = NULL;
	IVWRender3D			*pVWRender3D = NULL;
	IDirect3DRMViewport	*pd3drmViewport = NULL;
	int					nTilt =0, nZoom = 0;
	float				fTilt, fZoom, fForward, fRight, fUp, fPan, fTmp;
	VARIANT_BOOL		bReset = VARIANT_FALSE;
	Vector2				v2Nav(0.f, 0.f), v2NavC(0.f, 0.f);
	D3DVECTOR			vPos, vDir, vUp, vCross;
	HRESULT				hr = S_OK;
	float				fCurrentSpeedFactor = fCameraNavScale;

	fTilt = fZoom = fForward = fRight = fUp = fPan = fTmp = 0.0f;

	if (!m_pvwfTransform)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	if (!m_bGotFocus && !m_bKeyboardNavEnabled)
		goto ERROR_ENCOUNTERED;

	if (!m_nViewMode)
	{
		nTilt = IsPressed(m_KeyBindings[kbTiltUp]) - IsPressed(m_KeyBindings[kbTiltDown]);
		bReset = IsPressed(m_KeyBindings[kbReset]);
	}
	
	//nZoom = IsPressed(m_KeyBindings[kbZoomOut]) - IsPressed(m_KeyBindings[kbZoomIn]);

	if (!nTilt && !nZoom && !m_bCameraMode && !bReset)
		goto ERROR_ENCOUNTERED;
	
	if (FAILED(hr = m_pVWRenderRoot->get_Camera(&pvwfCamera)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pvwfCamera->GetParent(&pvwfParent)) || !pvwfParent)
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pvwfCamera->get_Frame3D(&pd3drmfCamera)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = m_pVWRenderRoot->get_VWRender3D(&pVWRender3D)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pVWRender3D->get_Viewport(&pd3drmViewport)))
		goto ERROR_ENCOUNTERED;

	if (bReset)
	{
		//if (FAILED(hr = pd3drmfCamera->AddRotation(D3DRMCOMBINE_REPLACE, 1.0F, 0.0F, 0.0F, 0.0F)))
		//	goto ERROR_ENCOUNTERED;

		DISPPARAMS dispparams;
		VARIANT varResult;

		VariantInit(&varResult);

		hr = InitializeDISPPARAMS(&dispparams, 0);
		if (FAILED(hr)) goto ERROR_ENCOUNTERED;

		hr = m_piRenderViewControl->Invoke(didResetCameraPosition, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dispparams, &varResult, NULL, NULL);
		if (FAILED(hr)) goto ERROR_ENCOUNTERED;

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		if (m_nViewMode != TOP)
		{
			if (FAILED(hr = pd3drmViewport->SetField((float)fDefaultField)))
				goto ERROR_ENCOUNTERED;
		}

		// Camera at rendering context's eye's looking forward.
		// REVIEW: This should NOT be hard coded. If nothing else it is dependent on the
		// avatar's size/scale.
		//if (FAILED(hr = pd3drmfCamera->AddTranslation(D3DRMCOMBINE_REPLACE, 0.0F, 0.7F, 0.0F)))
		//	goto ERROR_ENCOUNTERED;
		goto ERROR_ENCOUNTERED;
	}

	if (FAILED(hr = pvwfCamera->GetPositionEx(pvwfParent, &vPos.x, &vPos.y, &vPos.z)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pvwfCamera->GetOrientationEx(pvwfParent, &vDir.x, &vDir.y, &vDir.z, &vUp.x, &vUp.y, &vUp.z)))
		goto ERROR_ENCOUNTERED;

	if (nTilt)
	{
		fTilt = nTilt * fTiltScale * fTime;

		if (!D3DRMVectorCrossProduct(&vCross, &vDir, &vUp))
		{
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		
		if (FAILED(hr = pvwfCamera->SetPositionEx(pvwfParent, 0.0F, 0.0F, 0.0F)))
			goto ERROR_ENCOUNTERED;

		if (FAILED(hr = pd3drmfCamera->AddRotation(D3DRMCOMBINE_AFTER, vCross.x, vCross.y, vCross.z, fTilt)))
			goto ERROR_ENCOUNTERED;

		if (FAILED(hr = pvwfCamera->SetPositionEx(pvwfParent, vPos.x, vPos.y, vPos.z)))
			goto ERROR_ENCOUNTERED;

		m_bMoving = TRUE;
	}

	if (nZoom)
	{
		fZoom = nZoom * (float)fRelativeZoomScale * (float)fDefaultField * fTime;

		if (FAILED(hr = pd3drmViewport->SetField(pd3drmViewport->GetField() + fZoom)))
			goto ERROR_ENCOUNTERED;

		m_bMoving = TRUE;
	}

	//Do the camera-only (editing) navigation.
	if (m_bCameraMode)
	{
		v2Nav = KeyboardNavVector();

		if (m_bTrackingMouse)
			v2Nav += MouseNavVector(m_vnMouseDisp);

		if (IsPressed(VK_SHIFT)) 
			fCurrentSpeedFactor *= 2.5f;

		if (m_nViewMode == TOP)
		{
			fForward = -v2Nav.y * fCurrentSpeedFactor * fTime * (8.0f / m_fZoomLevel);
			fRight = v2Nav.x * fCurrentSpeedFactor * fTime * (8.0f / m_fZoomLevel);;
			fUp	= 0.0f;

			//Get position in WORLD coordinates.  (was in local from above)
			if (FAILED(hr = pvwfCamera->GetPositionEx(NULL, &vPos.x, &vPos.y, &vPos.z)))
				goto ERROR_ENCOUNTERED;

			if (FAILED(hr = pvwfCamera->SetPositionEx(NULL,
										vPos.x + fForward,
										vPos.y + fUp,
										vPos.z + fRight)))
				goto ERROR_ENCOUNTERED;
		}
		else
		{
			// if ctrl key pressed, the navigation vector deals with left/right, up/down motion
			if (IsPressed(VK_CONTROL)) 
			{
				v2NavC = v2Nav;
				v2Nav = Vector2(0.f, 0.f);
			}

			fForward = v2Nav.y * fCurrentSpeedFactor * fTime;
			fRight = v2NavC.x * fCurrentSpeedFactor * fTime;
			fUp	= v2NavC.y * fCurrentSpeedFactor * fTime;
			fPan = v2Nav.x * fPanScale * fTime;

			if (FAILED(hr = pvwfCamera->SetPositionEx(pvwfParent, 0.0F, 0.0F, 0.0F)))
				goto ERROR_ENCOUNTERED;

			if (FAILED(hr = pd3drmfCamera->AddRotation(D3DRMCOMBINE_AFTER, 0.0F, 1.0F, 0.0F, fPan)))
				goto ERROR_ENCOUNTERED;

			if (FAILED(hr = pvwfCamera->SetPositionEx(pvwfParent,
													vPos.x + fRight * vDir.z + fForward * vDir.x,
													vPos.y + fUp,
													vPos.z + fForward * vDir.z + -fRight * vDir.x)))
				goto ERROR_ENCOUNTERED;
			}

		if (fForward || fRight || fUp || fPan)
			m_bMoving = TRUE;
	}

ERROR_ENCOUNTERED:

	SAFERELEASE(pvwfCamera);
	SAFERELEASE(pvwfParent);
	SAFERELEASE(pd3drmfCamera);
	SAFERELEASE(pVWRender3D);
	SAFERELEASE(pd3drmViewport);

	return hr;
}

STDMETHODIMP CVWNavigationTool3D::IsValid(IVWUIView *pVw,VARIANT_BOOL * pfValid)
{
	return E_NOTIMPL;
}

STDMETHODIMP CVWNavigationTool3D::Initialize(IVWUIView* pVw, IWorld* pWorld, IVWRenderRoot *pVWRenderRoot)
{
	HRESULT hr = S_OK;

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

	if (m_pVWRenderRoot != pVWRenderRoot)
	{
		SAFERELEASE(m_pVWRenderRoot);
		m_pVWRenderRoot = pVWRenderRoot;
		SAFEADDREF(pVWRenderRoot);
	}

	SAFERELEASE(m_piRenderViewControl);

	//If used in the boundary editor, use orthogonal movement mode.
	hr = m_pUIView->QueryInterface(DIID__RenderingControl, (LPVOID*) &m_piRenderViewControl);
	if (FAILED(hr) || !m_piRenderViewControl)
	{
		OutputDebugString("NavTool: Failed to QI UIView for RenderingControl.\n");
		ASSERT(FALSE);
	}

	return S_OK;
}

STDMETHODIMP CVWNavigationTool3D::put_NavigationTarget(IThing *pThing)
{
	IWorld			*pWorld = NULL;
	IVWAnimator		*pVWAnimator = NULL;
	DISPPARAMS		dispparams;
	CComVariant		var;
	HRESULT			hr = S_OK;

	EnterCriticalSection(&m_CS);

	if (m_pThing)
	{
		// Get the world via the thing before we release the thing.
		if (FAILED(hr = m_pThing->get_World(&pWorld)))
			goto ERROR_ENCOUNTERED;
		if (!pWorld)
		{
			ASSERT(FALSE);
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		// Get the global animator tool we need from the world.
		if (FAILED(hr = pWorld->get_Tool(CComBSTR("Animator"), (IUnknown**)&pVWAnimator)))
			goto ERROR_ENCOUNTERED;
		if (!pVWAnimator || !m_pHBAnimation)
		{
			ASSERT(FALSE);
			hr = E_FAIL;
			//Clear out the thing pointer so we don't get stuck in this state.
			SAFERELEASE(m_pThing);   
			goto ERROR_ENCOUNTERED;
		}
		// Before we remove the HB animation, call it to ensure latest position/orientation
		// properties are sent if they have changed since the last HB.
		if (FAILED(hr = m_pHBAnimation->Invoke(GetTickCount())))
			goto ERROR_ENCOUNTERED;

		// Remove the HB animation for this thing.
		if (FAILED(hr = pVWAnimator->Remove(m_pHBAnimation)))
			goto ERROR_ENCOUNTERED;

		m_pThing->put_BOOL(CComBSTR("NavigationTarget"), VARIANT_FALSE);
		m_pThing->put_Short(CComBSTR("PositionDuration"), 0);

		SAFERELEASE(m_pHBAnimation);
		SAFERELEASE(pVWAnimator);
		SAFERELEASE(pWorld);
		SAFERELEASE(m_pvwfParent);
		SAFERELEASE(m_pvwfTransform);
		SAFERELEASE(m_pmapPositions);
		SAFERELEASE(m_pmapDirections);
		m_bNavigationPath = VARIANT_FALSE;
	}

	SAFERELEASE(m_pThing);
	m_pThing = pThing;
	SAFEADDREF(m_pThing);

	if (m_pThing)
	{
		// Get this thing's loaded geometry. This is the geometry we will be manipulating.
		if (FAILED(hr = m_pThing->InvokeMethodExt(CComBSTR("GetFrame"), NULL, &var)))
			goto ERROR_ENCOUNTERED;

		if (var.vt != VT_DISPATCH || !var.pdispVal || 
			FAILED(((IVWFrame*)var.pdispVal)->GetParent(&m_pvwfParent)) || !m_pvwfParent)
		{
			// A thing should not init the navigation target unless it has loaded geometry
			// and a parent!
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		m_pvwfTransform = (IVWFrame*)var.pdispVal;
		var.pdispVal->AddRef();

		// Get the world via the thing before we release the thing.
		if (FAILED(hr = m_pThing->get_World(&pWorld)))
			goto ERROR_ENCOUNTERED;
		if (!pWorld)
		{
			ASSERT(FALSE);
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}
		// Get the global animator tool we need from the world.
		if (FAILED(hr = pWorld->get_Tool(CComBSTR("Animator"), (IUnknown**)&pVWAnimator)))
			goto ERROR_ENCOUNTERED;

		if (!pVWAnimator)
		{
			ASSERT(FALSE);
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}

		// Add a HB animation for this thing (this will update the thing's position on the
		// server periodically).

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_DISPATCH;
		dispparams.rgvarg[0].pdispVal = m_pThing;

		hr = pVWAnimator->CreateExt(1000, m_pThing, CComBSTR("PositionOrientationHeartbeat"), &dispparams, &m_pHBAnimation);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);

		if (!m_pHBAnimation)
		{
			ASSERT(FALSE);
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}

		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		m_pThing->put_BOOL(CComBSTR("NavigationTarget"), VARIANT_TRUE);
		m_pThing->put_Short(CComBSTR("PositionDuration"), 1000);

		SAFERELEASE(pVWAnimator);
		SAFERELEASE(pWorld);
	}

ERROR_ENCOUNTERED:
	
	LeaveCriticalSection(&m_CS);

	SAFERELEASE(pVWAnimator);
	SAFERELEASE(pWorld);

	return hr;
}

STDMETHODIMP CVWNavigationTool3D::SetNavigationPath(IPropertyMap *pmapPositions, IPropertyMap *pmapDirections, BSTR bstrDoneEvent)
{
	EnterCriticalSection(&m_CS);

	// Store the navigation path.
	SAFERELEASE(m_pmapPositions);
	m_pmapPositions = pmapPositions;
	SAFEADDREF(pmapPositions);
	SAFERELEASE(m_pmapDirections);
	m_pmapDirections = pmapDirections;
	SAFEADDREF(pmapDirections);
	m_bNavigationPath = VARIANT_TRUE;
	m_bstrDoneEvent = bstrDoneEvent;

	LeaveCriticalSection(&m_CS);
	return S_OK;
}

STDMETHODIMP CVWNavigationTool3D::get_Moving(VARIANT_BOOL *bMoving)
{
	*bMoving = m_bMoving;
	return S_OK;
}

STDMETHODIMP CVWNavigationTool3D::put_TopOrthogonalMode(VARIANT_BOOL bOrthogonal)
{
	m_nViewMode = TOP;

	return S_OK;
}

STDMETHODIMP CVWNavigationTool3D::put_FrontOrthogonalMode(VARIANT_BOOL bOrthogonal)
{
	m_nViewMode = FRONT;

	return S_OK;
}

STDMETHODIMP CVWNavigationTool3D::put_RightOrthogonalMode(VARIANT_BOOL bOrthogonal)
{
	m_nViewMode = RIGHT;

	return S_OK;
}

STDMETHODIMP CVWNavigationTool3D::put_OrthogonalMode(VARIANT_BOOL bOrthogonal)
{
	if (bOrthogonal == VARIANT_TRUE && m_nViewMode == PERSPECTIVE )
	{
		//Default orthongonal mode is the top view
		m_nViewMode = TOP;
	}
	else if (bOrthogonal == VARIANT_FALSE)
	{
		m_nViewMode = PERSPECTIVE;
	}

	//MM: m_bOrthogonalMode = bOrthogonal;

	return S_OK;
}

STDMETHODIMP CVWNavigationTool3D::CalcVelocityAndPosition(float fTime, 
														  Vector3 *pvLastPos, Vector3 *pvPos,
														  Vector3 *pvDir, 
														  VARIANT_BOOL *pbTranslating,
														  VARIANT_BOOL *pbRotating)
{
	Vector3 vUp, vVel;
	float	fSpeedFactor = fSpeedScale * fTime, fRotFactor = fRotScale * fTime,
			fAccel, fForward, fRight, fUp = 0.0F, fRot;
	HRESULT	hr = S_OK;

	*pbTranslating = *pbRotating = VARIANT_FALSE;

	// combine keyboard and mouse navigation to generate a single navigation vector
	Vector2 v2Nav(0.f, 0.f), v2NavC(0.f, 0.f);
	if ((m_bGotFocus || m_bKeyboardNavEnabled) && !m_bCameraMode) 
		v2Nav += KeyboardNavVector();
	// check to see if we're still in low speed mode
	if (!IsTimeToAccel(v2Nav.x || v2Nav.y))
		v2Nav *= fLowSpeedScale;
	// if we're using the mouse to move, add the mouse vector
	if (m_bTrackingMouse)
		v2Nav += MouseNavVector(m_vnMouseDisp);

	// make sure that the combined vector is in range: [(-1.f, -1.f), (1.f, 1.f)]
	CLAMP(v2Nav.x, -1.f, 1.f);
	CLAMP(v2Nav.y, -1.f, 1.f);

	// accelerate to new velocities and store for next time...
	fAccel = fAccelScale * fTime;

	if (m_nViewMode == FRONT)
	{
		if (IsPressed(VK_CONTROL))
		{
			fForward = m_v2CurCVel.y = AccelVelocity(m_v2CurCVel.y, v2Nav.y, fAccel);
			fUp = 0.0f;
		}
		else
		{
			fUp	= m_v2CurCVel.y = AccelVelocity(m_v2CurCVel.y, v2Nav.y, fAccel);
			fForward = 0.0f;
		}

		fRight = m_v2CurVel.x = AccelVelocity(m_v2CurVel.x, v2Nav.x, fAccel);
		fRot = 0.0f;

	}
	else if (m_nViewMode == RIGHT)
	{
		if (IsPressed(VK_CONTROL))
		{
			fRight = m_v2CurCVel.y = AccelVelocity(m_v2CurCVel.y, v2Nav.y, fAccel);
			fUp = 0.0f;
		}
		else
		{
			fUp	= m_v2CurCVel.y = AccelVelocity(m_v2CurCVel.y, v2Nav.y, fAccel);
			fRight = 0.0f;
		}

		fForward = m_v2CurVel.x = AccelVelocity(m_v2CurVel.x, v2Nav.x, fAccel);
		fRot = 0.0f;
	}
	else if (m_nViewMode == TOP)
	{
		// don't rotate, scoot in the XZ plane
		fForward = m_v2CurVel.y  = AccelVelocity(m_v2CurVel.y, v2Nav.y, fAccel);
		fRight	= m_v2CurVel.x = AccelVelocity(m_v2CurVel.x, v2Nav.x, fAccel);
		fUp	= 0.0f; //m_v2CurCVel.y = AccelVelocity(m_v2CurCVel.y, v2Nav.y, fAccel);
		fRot = 0.0f;
	}
	else // m_nViewMode == PERSPECTIVE (default behavior)
	{
		// if ctrl key pressed, the navigation vector deals with left/right, up/down motion
		if ( (m_bGotFocus || m_bKeyboardNavEnabled) && IsPressed(VK_CONTROL))
		{
			v2NavC = v2Nav;
			v2Nav = Vector2(0.f, 0.0f);
		}

		fForward = m_v2CurVel.y  = AccelVelocity(m_v2CurVel.y, v2Nav.y, fAccel);
		fRight	= m_v2CurCVel.x = AccelVelocity(m_v2CurCVel.x, v2NavC.x, fAccel);

		// rotation is instantaneous
		fRot = m_v2CurVel.x = v2Nav.x;

		if (m_bFalling && m_bGravityOn)
			fUp	= m_v2CurCVel.y = AccelVelocity(m_v2CurCVel.y, -fTerminalVelocity, fAccel);
		else if (!m_bGravityOn)
			fUp	= m_v2CurCVel.y = AccelVelocity(m_v2CurCVel.y, v2NavC.y, fAccel);
	}

	// Get our last positon/orientation.
	if (FAILED(hr = m_pvwfTransform->GetPositionEx(m_pvwfParent, &pvLastPos->x, &pvLastPos->y, &pvLastPos->z)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pvwfTransform->GetOrientationEx(m_pvwfParent, &pvDir->x, &pvDir->y, &pvDir->z, &vUp.x, &vUp.y, &vUp.z)))
		goto ERROR_ENCOUNTERED;

	// if shift key is pressed, go into turbo mode - we don't want to accelerate
	// into turbo mode from normal mode
	if (m_bGotFocus || m_bKeyboardNavEnabled)
	{
		if (IsPressed(VK_SHIFT))
			fSpeedFactor *= fTurboScale;
	}

	if ((fForward != 0.f) || (fRight != 0.f) || (fUp != 0.f))
	{
		vVel = fForward * *pvDir + fRight * Cross(vUp, *pvDir) + fUp * vUp;
		*pvPos = *pvLastPos + (vVel * fSpeedFactor);
		*pbTranslating = VARIANT_TRUE;
	}

	if (fRot != 0.f)
	{
		pvDir->Rotate(vUp, -fRot * fRotFactor);
		*pbRotating = VARIANT_TRUE;
	}

ERROR_ENCOUNTERED:

	return hr;
}

STDMETHODIMP CVWNavigationTool3D::CalculateLocationOnPath(Vector3 *pvPos, Vector3 *pvDir,
														  VARIANT_BOOL *pbTranslating,
														  VARIANT_BOOL *pbRotating)
{
	COleVariant		varDestinationPos, varDestinationDir;
	DWORD			dwTimeElapsed;
	static DWORD	dwPathStartTime = 0, dwSourcePosTime, dwDestinationPosTime,
					dwSourceDirTime, dwDestinationDirTime;
	float			fSourceDirX, fSourceDirY, fSourceDirZ, fSourceUpX, fSourceUpY, fSourceUpZ,
					fDir;
	static float	fSourcePosX, fSourcePosY, fSourcePosZ, fSourceDir,
					fDestinationPosX, fDestinationPosY, fDestinationPosZ, fDestinationDir;
	HRESULT			hr = S_OK;

	*pbTranslating = *pbRotating = VARIANT_TRUE;

	if (dwPathStartTime == 0)
	{
		// This is the first time called on this navigation path. Initialize the source times, 
		// positions, directions and the path start time.
		dwPathStartTime = GetTickCount();
		dwSourcePosTime = dwDestinationPosTime = 0;
		dwTimeElapsed = 0;
		if (FAILED(hr = m_pvwfTransform->GetPositionEx(m_pvwfParent, &fSourcePosX, &fSourcePosY, &fSourcePosZ)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = m_pvwfTransform->GetOrientationEx(m_pvwfParent, &fSourceDirX, &fSourceDirY, &fSourceDirZ, &fSourceUpX, &fSourceUpY, &fSourceUpZ)))
			goto ERROR_ENCOUNTERED;
		fDestinationPosX = fSourcePosX;
		fDestinationPosY = fSourcePosY;
		fDestinationPosZ = fSourcePosZ;
		ConvertDirToRotation(fSourceDirX, fSourceDirY, fSourceDirZ, fSourceDir);
		fDestinationDir = fSourceDir;
	}
	else
	{
		// Update the time elapsed since this navigation path was started.
		dwTimeElapsed = GetTickCount() - dwPathStartTime;
	}

	// If necessary, figure out our next destination position based on the current time.
	if (dwTimeElapsed == 0 || dwTimeElapsed > dwDestinationPosTime)
	{
		dwSourcePosTime = dwDestinationPosTime;
		fSourcePosX = fDestinationPosX;
		fSourcePosY = fDestinationPosY;
		fSourcePosZ = fDestinationPosZ;
		if (!m_pmapPositions)
			*pbTranslating = VARIANT_FALSE;
		else if (!GetMapElementByTime(m_pmapPositions, dwTimeElapsed, dwDestinationPosTime, varDestinationPos))
		{
			// Finished position side of navigation path, release positions info and 
			// set position to final destination.
			SAFERELEASE(m_pmapPositions);
			pvPos->x = fDestinationPosX;
			pvPos->y = fDestinationPosY;
			pvPos->z = fDestinationPosZ;
		}
		else 
		{
			if (varDestinationPos.vt != VT_DISPATCH)
			{
				hr = E_FAIL;
				goto ERROR_ENCOUNTERED;
			}
			((IVector*)varDestinationPos.pdispVal)->get(&fDestinationPosX, &fDestinationPosY, &fDestinationPosZ);
		}
	}

	// If necessary, figure out our next destination direction based on the current time.
	if (dwTimeElapsed == 0 || dwTimeElapsed > dwDestinationDirTime)
	{
		dwSourceDirTime = dwDestinationDirTime;
		fSourceDir = fDestinationDir;
		if (!m_pmapDirections)
			*pbRotating = VARIANT_FALSE;
		else if (!GetMapElementByTime(m_pmapDirections, dwTimeElapsed, dwDestinationDirTime, varDestinationDir))
		{
			// Finished direction side of navigation path, release directions info and 
			// set direction to final destination.
			SAFERELEASE(m_pmapDirections);
			fDir = fDestinationDir;
			ConvertRotationToDir(fDir, pvDir->x, pvDir->y, pvDir->z, d3dvZAxis, d3dvYAxis);
		}
		else
		{
			if (varDestinationDir.vt != VT_R4)
			{
				hr = E_FAIL;
				goto ERROR_ENCOUNTERED;
			}
			fDestinationDir = varDestinationDir.fltVal;
		}
	}

	if (m_pmapPositions)
	{
		// Interpolate for desired postion at current time.
		pvPos->x = InterpolatePos(dwPathStartTime + dwSourcePosTime, 
								  dwPathStartTime + dwDestinationPosTime, 
							      fSourcePosX, 
							      fDestinationPosX);
		pvPos->y = InterpolatePos(dwPathStartTime + dwSourcePosTime, 
							      dwPathStartTime + dwDestinationPosTime, 
							      fSourcePosY, 
							      fDestinationPosY);
		pvPos->z = InterpolatePos(dwPathStartTime + dwSourcePosTime, 
							      dwPathStartTime + dwDestinationPosTime, 
							      fSourcePosZ, 
							      fDestinationPosZ);
	}

	if (m_pmapDirections)
	{
		// Interpolate for desired direction at current time.
		fDir = InterpolateDir(dwPathStartTime + dwSourceDirTime, 
						      dwPathStartTime + dwDestinationDirTime, 
						      fSourceDir, 
						      fDestinationDir);
		ConvertRotationToDir(fDir, pvDir->x, pvDir->y, pvDir->z, d3dvZAxis, d3dvYAxis);
	}

	if (!m_pmapPositions && !m_pmapDirections)
	{
		// Finished navigation path.
		dwPathStartTime = 0;
		m_bNavigationPath = VARIANT_FALSE;
	}

ERROR_ENCOUNTERED:

	return hr;
}

STDMETHODIMP CVWNavigationTool3D::HandleIntersections(IPropertyList *pIDataList, Vector3 &vLastPos, Vector3 &vPos)
{
	IPropertyList		*pIDataList2 = NULL;
	IVWIntersectData	*pVWIntersectData = NULL;
	IPropertyList		*pBoundaryList = NULL;
	IBoundary			*pBoundary = NULL;
	IVector				*pvProjection = NULL;
	int					iBoundaryIndex, iEdgeIndex;
	VARIANT_BOOL		bPassable;
	BYTE				bPassed = eHit;
	float				fDist;
	long				lCount;
	Vector3				vMovement;
	HRESULT hr = S_OK;

	pIDataList->get_Count(&lCount);
	if (lCount == 0)
	{
		// Should be at least one element in the list!
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	pIDataList->get_ObjectProperty(lCount-1, (IObjectProperty**)&pVWIntersectData);
	pVWIntersectData->Get(&pBoundaryList, &iBoundaryIndex, &iEdgeIndex, NULL, &bPassed, NULL, &pvProjection);
	pBoundaryList->get_ObjectProperty(iBoundaryIndex, (IObjectProperty**) &pBoundary);
	pBoundary->get_IsPassable(&bPassable);

	if (!bPassable && !bPassed)
	{
		// The last edge intersected is impassable. 
		// Don't consider edge index. Gets confusing to fire multiple intersects on
		// different edges of same boundary (these intersects can be almost instantaneous).
		if (m_LastHitEdge1.IsNewHit(pBoundaryList, iBoundaryIndex, 0, GetTickCount()))
		{
			// Fire intersect event.
			if (FAILED(hr = FireIntersectEvent(vLastPos.x, vLastPos.y, vLastPos.z, vPos.x, vPos.y, vPos.z, pIDataList)))
				goto ERROR_ENCOUNTERED;

			m_LastHitEdge1.Set(pBoundaryList, iBoundaryIndex, 0, GetTickCount());
		}
		else
			m_LastHitEdge1.dwTimeHit = GetTickCount();
		
		// Get the projection point along the wall (a unit vector).
		pvProjection->get(&vPos.x, &vPos.y, &vPos.z);

		// Move along parallel to the wall.
		if (FAILED(hr = m_pvwfTransform->MoveToPosition(m_pvwfParent, vPos.x, vPos.y, vPos.z, VARIANT_FALSE, &pIDataList2)))
			goto ERROR_ENCOUNTERED;

		if (pIDataList2)
		{
			pIDataList2->get_Count(&lCount);
			if (lCount == 0)
			{
				// Should be at least one element in the list!
				hr = E_FAIL;
				goto ERROR_ENCOUNTERED;
			}

			SAFERELEASE(pVWIntersectData);
			pIDataList2->get_ObjectProperty(lCount-1, (IObjectProperty**)&pVWIntersectData);
			SAFERELEASE(pBoundaryList);
			SAFERELEASE(pvProjection);
			pVWIntersectData->Get(&pBoundaryList, &iBoundaryIndex, &iEdgeIndex, &fDist, &bPassed, NULL, &pvProjection);
			SAFERELEASE(pBoundary);
			pBoundaryList->get_ObjectProperty(iBoundaryIndex, (IObjectProperty**) &pBoundary);
			pBoundary->get_IsPassable(&bPassable);

			if (!bPassable && !bPassed)
			{
				// Gets confusing to fire multiple intersects on different edges of same 
				// boundary (these intersects can be almost instantaneous).
				if (m_LastHitEdge2.IsNewHit(pBoundaryList, iBoundaryIndex, iEdgeIndex, GetTickCount()) &&
					m_LastHitEdge2.IsNewHit(m_LastHitEdge1.pBoundaryList, m_LastHitEdge1.iBoundaryIndex, m_LastHitEdge2.iEdgeIndex, GetTickCount()))
				{
					// Fire intersect event.
					if (FAILED(hr = FireIntersectEvent(vLastPos.x, vLastPos.y, vLastPos.z, vPos.x, vPos.y, vPos.z, pIDataList2)))
						goto ERROR_ENCOUNTERED;

					m_LastHitEdge2.Set(pBoundaryList, iBoundaryIndex, iEdgeIndex, GetTickCount());
				}
				else
					m_LastHitEdge2.dwTimeHit = GetTickCount();

				// Do not change position, only risks crossing other boundaries.
				vPos = vLastPos;
			}
			else
			{
				if (!bPassed)
				{
					// Don't need to check m_LastHitEdge. Just slid along non-passable edge
					// and hit a passable one. Passable state is boundary specific so this
					// MUST be a different boundary.
					// Fire intersect event.
					if (FAILED(hr = FireIntersectEvent(vLastPos.x, vLastPos.y, vLastPos.z, vPos.x, vPos.y, vPos.z, pIDataList2)))
						goto ERROR_ENCOUNTERED;
				}

				// Go to where we were going.
				if (FAILED(hr = m_pvwfTransform->SetPositionEx(m_pvwfParent, vPos.x, vPos.y, vPos.z)))
					goto ERROR_ENCOUNTERED;
			}
		}
	}
	else
	{
		if (!bPassed)
		{
			// Don't consider edge index. Gets confusing to fire multiple intersects on
			// different edges of same boundary (these intersects can be almost instantaneous).
			if (m_LastHitEdge1.IsNewHit(pBoundaryList, iBoundaryIndex, 0, GetTickCount()))
			{
				// Fire intersect event.
				if (FAILED(hr = FireIntersectEvent(vLastPos.x, vLastPos.y, vLastPos.z, vPos.x, vPos.y, vPos.z, pIDataList)))
					goto ERROR_ENCOUNTERED;

				m_LastHitEdge1.Set(pBoundaryList, iBoundaryIndex, 0, GetTickCount());
			}
			else
				m_LastHitEdge1.dwTimeHit = GetTickCount();
		}
		
		// Go to where we were going.
		if (FAILED(hr = m_pvwfTransform->SetPositionEx(m_pvwfParent, vPos.x, vPos.y, vPos.z)))
			goto ERROR_ENCOUNTERED;
	}

	// We passed at least one intersection and should find the new floor.
	if (m_bGravityOn && FAILED(hr = Drop(vPos.x, vPos.y, vPos.z)))
		goto ERROR_ENCOUNTERED;
	
ERROR_ENCOUNTERED:

	SAFERELEASE(pVWIntersectData);
	SAFERELEASE(pBoundaryList);
	SAFERELEASE(pBoundary);
	SAFERELEASE(pvProjection);
	SAFERELEASE(pIDataList2);

	return hr;
}

STDMETHODIMP CVWNavigationTool3D::put_KeyboardNavEnabled(VARIANT_BOOL bEnable)
{
	m_bKeyboardNavEnabled = bEnable;
	return S_OK;
}

STDMETHODIMP CVWNavigationTool3D::get_KeyboardNavEnabled(VARIANT_BOOL* pbEnable)
{
	if (!pbEnable)
		return E_POINTER;

	*pbEnable = m_bKeyboardNavEnabled;
	return S_OK;
}

STDMETHODIMP CVWNavigationTool3D::put_AllNavigationDisabled(VARIANT_BOOL bDisable)
{
	m_bAllNavigationDisabled = bDisable;
	return S_OK;
}

STDMETHODIMP CVWNavigationTool3D::get_AllNavigationDisabled(VARIANT_BOOL* pbDisable)
{
	if (!pbDisable)
		return E_POINTER;

	*pbDisable = m_bAllNavigationDisabled;
	return S_OK;
}

STDMETHODIMP CVWNavigationTool3D::get_KeyBinding(KeyConstant index, VARIANT* pvar) 
{
	if ( index >= kbMax ) return E_INVALIDARG;
	if ( index < 0 ) return E_INVALIDARG;
	if ( pvar == NULL ) return E_POINTER;
	return ::VariantCopy( pvar,  &(CComVariant((int)m_KeyBindings[index]))); 
}

STDMETHODIMP CVWNavigationTool3D::put_KeyBinding(KeyConstant index, VARIANT var)
{
	if ( index >= kbMax ) return E_INVALIDARG;
	if ( index < 0 ) return E_INVALIDARG;

	CComVariant val;
	HRESULT hr = ::VariantChangeType( &val,&var,0,VT_I4); 
	if ( FAILED(hr))
		return hr;
  
	m_KeyBindings[index] = val.lVal;

	return S_OK;
}


STDMETHODIMP CVWNavigationTool3D::get_CollisionDetection(VARIANT_BOOL *pVal)
{
	ASSERT(pVal);

	*pVal = m_bCollisionDetection;

	return S_OK;
}

STDMETHODIMP CVWNavigationTool3D::put_CollisionDetection(VARIANT_BOOL newVal)
{
	m_bCollisionDetection = newVal;

	return S_OK;
}

STDMETHODIMP CVWNavigationTool3D::get_Gravity(VARIANT_BOOL *pVal)
{
	ASSERT(pVal);

	*pVal = m_bGravityOn;

	return S_OK;
}

STDMETHODIMP CVWNavigationTool3D::put_Gravity(VARIANT_BOOL newVal)
{
	float	fPosX, fPosY, fPosZ;
	HRESULT hr = S_OK;

	m_bGravityOn = newVal;

	if (!m_bGravityOn)
		m_v2CurCVel.y = 0.0F;
	else
	{
		// When gravity is turned on start falling, if appropriate.
		if (m_pvwfTransform && m_pvwfParent)
		{
			// Get our current position.
			if (FAILED(hr = m_pvwfTransform->GetPositionEx(m_pvwfParent, &fPosX, &fPosY, &fPosZ)))
				goto ERROR_ENCOUNTERED;
			if (FAILED(hr = Drop(fPosX, fPosY, fPosZ)))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:

	return hr;
}

STDMETHODIMP CVWNavigationTool3D::Drop(float fCurrentPosX, float fCurrentPosY, float fCurrentPosZ)
{
	float fFloorHeight;
	HRESULT hr = S_OK;

	// Get the floor height.
	if (FAILED(hr = m_pvwfTransform->GetFloorHeight(m_pvwfParent, &fFloorHeight)))
		goto ERROR_ENCOUNTERED;

	// Is there even a floor at all, and are we already on it.
	if (fFloorHeight != fCurrentPosY && fFloorHeight != fCurrentPosY-fOffsetFromFloor)
	{
		// OK, we need to step up/down or fall.
		if (fFloorHeight >= fCurrentPosY-(fOffsetFromFloor*2.0F))
		{
			// Step up/down.
			if (FAILED(hr = m_pvwfTransform->SetPositionEx(m_pvwfParent, fCurrentPosX, fFloorHeight+fOffsetFromFloor, fCurrentPosZ)))
				goto ERROR_ENCOUNTERED;
			// Stop falling
			m_bFalling = VARIANT_FALSE;
			m_v2CurCVel.y = 0.0F;
		}
		else
		{
			// Step down.
			if (FAILED(hr = m_pvwfTransform->SetPositionEx(m_pvwfParent, fCurrentPosX, fCurrentPosY-fOffsetFromFloor, fCurrentPosZ)))
				goto ERROR_ENCOUNTERED;
			// Start falling!
			m_bFalling = VARIANT_TRUE;
			m_v2CurCVel.y = -fOffsetFromFloor;
		}
	}

ERROR_ENCOUNTERED:

	return hr;
}

STDMETHODIMP CVWNavigationTool3D::get_CameraOnlyNavigationMode(VARIANT_BOOL* pbEnable) 
{
	HRESULT hr = S_OK;

	if (!pbEnable)
		return E_POINTER;

	*pbEnable = m_bCameraMode;

	return hr;
}

STDMETHODIMP CVWNavigationTool3D::put_CameraOnlyNavigationMode(VARIANT_BOOL bEnable)
{
	HRESULT hr = S_OK;

	m_bCameraMode = bEnable;

	return hr;
}
