// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _d2dprv_h
#define _d2dprv_h

// File:	d2dprv.h
// Author:	MM    (MM)
//
// Description:
//    This file describes several private COM interfaces for d2d that are
//  only used internal to d2d.
// 
// History:
// -@- 11/04/96 (MM) created

#if defined( _WIN32 )  && !defined( _NO_COM )
#define COM_NO_WINDOWS_H
#include <objbase.h>
#else
#define IUnknown            void
#define CO_E_NOTINITIALIZED 0x800401F0L
#endif

#if !defined(__cplusplus) || defined(CINTERFACE)
#error error: the private interface can only be used by C++
#endif

EXTERN_C const IID IID_ID2DRMPrivate;
EXTERN_C const IID IID_ID2DRMArrayPrivate;
EXTERN_C const IID IID_ID2DRMPickPrivate;
EXTERN_C const IID IID_ID2DRMDevicePrivate;
EXTERN_C const IID IID_ID2DRMFramePrivate;
EXTERN_C const IID IID_ID2DRMImagePrivate;
EXTERN_C const IID IID_ID2DRMViewportPrivate;
EXTERN_C const IID IID_ID2DRMVisualPrivate;

struct ID2DRMPrivate;
struct ID2DRMArrayPrivate;
struct ID2DRMPickPrivate;
struct ID2DRMDevicePrivate;
struct ID2DRMFramePrivate;
struct ID2DRMImagePrivate;
struct ID2DRMViewportPrivate;
struct ID2DRMVisualPrivate;

typedef struct ID2DRMPrivate FAR *				LPD2DRMPRIVATE;
typedef struct ID2DRMArrayPrivate FAR *			LPD2DRMARRAYPRIVATE;
typedef struct ID2DRMPickPrivate FAR *			LPD2DRMPICKPRIVATE;
typedef struct ID2DRMDevicePrivate FAR *		LPD2DRMDEVICEPRIVATE;
typedef struct ID2DRMFramePrivate FAR *			LPD2DRMFRAMEPRIVATE;
typedef struct ID2DRMImagePrivate FAR *			LPD2DRMIMAGEPRIVATE;
typedef struct ID2DRMViewportPrivate FAR *		LPD2DRMVIEWPORTPRIVATE;
typedef struct ID2DRMVisualPrivate FAR *		LPD2DRMVISUALPRIVATE;

///////////////////////////////////////////////////////////////////////////////
// ID2DRMPrivate
///////////////////////////////////////////////////////////////////////////////
#if defined( _WIN32 ) && !defined( _NO_COM )
#undef INTERFACE
#define INTERFACE ID2DRMPrivate
DECLARE_INTERFACE_( ID2DRMPrivate, IDirect2DRM )
{
	// REVIEW: later on, this should probably be changed to take an object
	//  and the object interface should implement Tick
	STDMETHOD_(POSITION, InsTicked)(THIS_ LPD2DRMIMAGEPRIVATE pImage) PURE;
	STDMETHOD_(void, DelTicked)(THIS_ POSITION pos) PURE;
};
#endif


///////////////////////////////////////////////////////////////////////////////
// ID2DRMArrayPrivate
///////////////////////////////////////////////////////////////////////////////
#if defined( _WIN32 ) && !defined( _NO_COM )
#undef INTERFACE
#define INTERFACE ID2DRMArrayPrivate
DECLARE_INTERFACE_( ID2DRMArrayPrivate, IDirect2DRMArray )
{
	STDMETHOD_(DWORD, InsElement)(THIS_ DWORD nType, LPDIRECT2DRMOBJECT pD2DRMObject) PURE;
	STDMETHOD_(void, DelElement)(THIS_ DWORD nIndex) PURE;
	STDMETHOD(Copy)(THIS_ LPD2DRMARRAYPRIVATE *ppArray) PURE;
	STDMETHOD_(void, Fill)(THIS_ LPVOID pvData) PURE;
};
#endif

///////////////////////////////////////////////////////////////////////////////
// ID2DRMPickPrivate
///////////////////////////////////////////////////////////////////////////////
#if defined( _WIN32 ) && !defined( _NO_COM )
#undef INTERFACE
#define INTERFACE ID2DRMPickPrivate
DECLARE_INTERFACE_( ID2DRMPickPrivate, IDirect2DRMPick )
{
	STDMETHOD(Init)(THIS_ LPD2DPOINT ppt2HitPos, LPD2DRMVISUALPRIVATE pVisual, 
				LPD2DRMARRAYPRIVATE pAncestory) PURE;
};
#endif

///////////////////////////////////////////////////////////////////////////////
// ID2DRMDevicePrivate
///////////////////////////////////////////////////////////////////////////////
#if defined( _WIN32 ) && !defined( _NO_COM )
#undef INTERFACE
#define INTERFACE ID2DRMDevicePrivate
DECLARE_INTERFACE_( ID2DRMDevicePrivate, IDirect2DRMDevice )
{
	STDMETHOD_(LPDIRECTDRAWSURFACE, GetBackBuffer)(THIS) PURE;
	STDMETHOD(AddViewport)(THIS_ LPDIRECT2DRMVIEWPORT pd2drmViewport) PURE;
	STDMETHOD_(DWORD, GetDepth)(THIS) PURE;
	STDMETHOD_(DWORD, GetColor)(THIS_ BYTE r, BYTE g, BYTE b) PURE;
};
#endif

///////////////////////////////////////////////////////////////////////////////
// ID2DRMFramePrivate
///////////////////////////////////////////////////////////////////////////////
#if defined( _WIN32 ) && !defined( _NO_COM )
#undef INTERFACE
#define INTERFACE ID2DRMFramePrivate
DECLARE_INTERFACE_( ID2DRMFramePrivate, IDirect2DRMFrame )
{
	STDMETHOD_(void, SetParent) (THIS_ LPD2DRMFRAMEPRIVATE pParent) PURE;
	STDMETHOD_(LPD2DRMFRAMEPRIVATE, GetParent) (THIS) PURE;
	STDMETHOD(GetFrameDisplayList) (THIS_ CDisplayList *pdlScene, D2DTransform *pt2dCurrent) PURE;
	STDMETHOD(RenderToSurface)(THIS_ LPDIRECTDRAWSURFACE pdds, LPD2DRECT prDst, 
				D2DTransform *pt2dCurrent, DWORD dwFlags) PURE;
	STDMETHOD(PickFrame)(THIS_ LPD2DPOINT ppt, LPD2DRMARRAYPRIVATE pAncestory, 
				LPD2DRMARRAYPRIVATE pPicks) PURE;
	STDMETHOD(GetInverseTransform)(THIS_ D2DTransform *pTransform) PURE;
};
#endif

///////////////////////////////////////////////////////////////////////////////
// ID2DRMImagePrivate
///////////////////////////////////////////////////////////////////////////////
#if defined( _WIN32 ) && !defined( _NO_COM )
#undef INTERFACE
#define INTERFACE ID2DRMImagePrivate
DECLARE_INTERFACE_( ID2DRMImagePrivate, IDirect2DRMImage )
{
	STDMETHOD(InitFromMFImage)(THIS_ LPD2DRMPRIVATE pD2DRMPriv, CMFImage *pmfi, DWORD dwFlags) PURE;
	STDMETHOD(Tick)(THIS_ DWORD nElapsedTime) PURE;
};
#endif

///////////////////////////////////////////////////////////////////////////////
// ID2DRMViewportPrivate
///////////////////////////////////////////////////////////////////////////////
#if defined( _WIN32 ) && !defined( _NO_COM )
#undef INTERFACE
#define INTERFACE ID2DRMViewportPrivate
DECLARE_INTERFACE_( ID2DRMViewportPrivate, IDirect2DRMViewport )
{
	STDMETHOD(DeviceResized)(THIS) PURE;
	STDMETHOD(UnsetParentDevice)(THIS) PURE;
};
#endif

///////////////////////////////////////////////////////////////////////////////
// ID2DRMVisualPrivate
///////////////////////////////////////////////////////////////////////////////
// Notes: The VisualPrivate interface is actually not derived from the Visual
//  interface.  This is because the Visual interface pretty much just functions
//  as a convenient generic pointer, and having this structure simplifies the
//  structural issues.  In short, we can get away with it here only.
#if defined( _WIN32 ) && !defined( _NO_COM )
#undef INTERFACE
#define INTERFACE ID2DRMVisualPrivate
DECLARE_INTERFACE_( ID2DRMVisualPrivate, IUnknown )
{
	STDMETHOD(GetDisplayList) (THIS_ CDisplayList *pdlScene, D2DTransform *pt2dCurrent) PURE;
	STDMETHOD(PickVisual)(THIS_ LPD2DPOINT ppt, LPD2DRMARRAYPRIVATE pAncestory, 
					LPD2DRMARRAYPRIVATE pPicks) PURE;
};
#endif

#endif
