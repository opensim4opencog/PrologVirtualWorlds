// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include <d2d.h>
#include <d3drm.h>
#include <vwobject.h>
#include <propbase.h>
#include "vwidata.h"
#include "mmhelp.h"
#include "syshelp.h"
#include "vwsystem.h"

// NOTE: can use faster CComObject construction if building VWMM
// otherwise, use CoCreateInstance by CLSID

#ifdef VWMM_BUILD
#include "vwgeomob.h"
#include "vwgeoms.h"
#include "vectorob.h"
#include "vwadatao.h"
#include "sound.h"
#include "PickDatO.h"
#include "edgecyc.h"
#include "vwframeo.h"

typedef CComObject<CVWGeometryObject> CComVWGeometry;
typedef CComObject<CVWGeometrySprite> CComVWGeometrySprite;
typedef CComObject<CVectorObject> CComVector;
typedef CComObject<CVWAccessoryDataObject> CComVWAccessoryData;
typedef CComObject<CSoundObject> CComSound;
typedef CComObject<CVWGeometryObject> CComVWGeometry;
typedef CComObject<CBoundary> CComBoundary;
typedef CComObject<CVWPickDataObject> CComPickData;
typedef CComObject<CVWFrameObject> CComVWFrame;

#else
EXTERN_C const CLSID CLSID_VWGeometry;
EXTERN_C const CLSID CLSID_VWFrame;
EXTERN_C const CLSID CLSID_VWGeometrySprite;
EXTERN_C const CLSID CLSID_Vector;
EXTERN_C const CLSID CLSID_VWAccessoryData;
EXTERN_C const CLSID CLSID_Sound;
EXTERN_C const CLSID CLSID_Vector;
EXTERN_C const CLSID CLSID_Boundary;
EXTERN_C const CLSID CLSID_VWPickData;
#endif

#ifdef VWMM_BUILD
HRESULT CreateGeometry(IWorld* pWorld, BSTR bstrGeometryName, IVWGeometry **ppgeometry)
{
	HRESULT hr = S_OK;

	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "CreateGeometry\n");

	ASSERT(ppgeometry);
	if (ppgeometry == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateGeometry: outgoing pointer (IVWGeometry**) is NULL\n");
		return E_POINTER;
	}

	*ppgeometry = new CComVWGeometry;

	if (*ppgeometry == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateGeometry: failed to allocate CComVWGeometry\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(*ppgeometry);

	(*ppgeometry)->AddRef();

	hr = (*ppgeometry)->put_World(pWorld);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppgeometry)->put_GeometryName(bstrGeometryName);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppgeometry);

	return hr;
};

HRESULT CreateFrame(IWorld* pWorld, IVWFrame **ppframe)
{
	HRESULT hr = S_OK;

	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "CreateFrame\n");

	ASSERT(ppframe);
	if (ppframe == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateFrame: outgoing pointer (IVWFrame**) is NULL\n");
		return E_POINTER;
	}

	*ppframe = new CComVWFrame;

	if (*ppframe == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateFrame: failed to allocate CComVWFrame\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(*ppframe);

	(*ppframe)->AddRef();

	hr = (*ppframe)->put_World(pWorld);

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppframe);

	return hr;
};

HRESULT CreateSpritedGeometry(IWorld* pWorld, BSTR bstrGeometryName, IVWGeometrySprite **ppgeometry)
{
	HRESULT hr = S_OK;

	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "CreateSpritedGeometry\n");

	ASSERT(ppgeometry);
	if (ppgeometry == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateSpritedGeometry: outgoing pointer (IVWGeometrySprite**) is NULL\n");
		return E_POINTER;
	}

	*ppgeometry = new CComVWGeometrySprite;

	if (*ppgeometry == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateSpritedGeometry: failed to allocate CComVWGeometrySprite\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(*ppgeometry);

	(*ppgeometry)->AddRef();

	hr = (*ppgeometry)->put_World(pWorld);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppgeometry)->put_GeometryName(bstrGeometryName);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppgeometry);

	return hr;
};

HRESULT CreateVector(IWorld* pWorld, float x, float y, float z, IVector** ppvector)
{
	HRESULT hr = S_OK;

	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "CreateVector\n");

	ASSERT(ppvector);
	if (ppvector == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateVector: outgoing pointer (IVector**) is NULL\n");
		return E_POINTER;
	}

	*ppvector = new CComVector;

	if (*ppvector == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateVector: failed to allocate CComVector\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(*ppvector);

//	VWTRACE(pWorld, "VWOBJECT", TRACE_ALLOCATION, "CreateVector: allocated CComVector, this 0x%0x\n", *ppvector);

	(*ppvector)->AddRef();

	hr = (*ppvector)->put_World(pWorld);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppvector)->set(x, y, z);

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppvector);

	return hr;
}

HRESULT CreateAccessoryData(IWorld* pWorld, BSTR bstrAccessoryGraphic, float fPosX, float fPosY, IVWAccessoryData **ppaccessorydata)
{
	HRESULT hr = S_OK;

	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "CreateAccessoryData\n");

	ASSERT(ppaccessorydata);
	if (ppaccessorydata == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateAccessoryData: outgoing pointer (IVWAccessoryData**) is NULL\n");
		return E_POINTER;
	}

	*ppaccessorydata = new CComVWAccessoryData;

	if (*ppaccessorydata == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateAccessoryData: failed to allocate CComVWAccessoryData\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(*ppaccessorydata);

	(*ppaccessorydata)->AddRef();

	hr = (*ppaccessorydata)->put_World(pWorld);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppaccessorydata)->put_AccessoryGraphic(bstrAccessoryGraphic);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppaccessorydata)->PutPos(fPosX, fPosY);

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppaccessorydata);

	return hr;
}

HRESULT CreateSound(IWorld* pWorld,BSTR SoundURL, VARIANT_BOOL bIs3D,VARIANT_BOOL bIsLooping, ISound **ppSound)
{
	HRESULT hr = S_OK;

	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "CreateSound\n");

	ASSERT(ppSound);
	if (ppSound == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateSound: outgoing pointer (ISound**) is NULL\n");
		return E_POINTER;
	}

	*ppSound = new CComSound;

	if (*ppSound == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateSound: failed to allocate CComSound\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(*ppSound);

	(*ppSound)->AddRef();

	hr = (*ppSound)->put_World(pWorld);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppSound)->put_SoundURL(SoundURL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppSound)->put_Is3D(bIs3D);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppSound)->put_IsLooping(bIsLooping);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;


ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppSound);

	return hr;
}

HRESULT CreatePickData(IWorld* pWorld, IVWPickData** ppPickData)
{
	HRESULT hr = S_OK;

	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "CreatePickData\n");

	ASSERT(ppPickData);
	if (ppPickData == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreatePickData: outgoing pointer (IVWPickData**) is NULL\n");
		return E_POINTER;
	}

	*ppPickData = new CComPickData;

	if (*ppPickData == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreatePickData: failed to allocate CComPickData\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(*ppPickData);

	(*ppPickData)->AddRef();

	hr = (*ppPickData)->put_World(pWorld);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppPickData);

	return hr;

}

HRESULT CreateBoundary(IWorld* pWorld, IBoundary** ppBoundary)
{
	HRESULT hr = S_OK;

	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "CreateBoundary\n");

	ASSERT(ppBoundary);
	if (ppBoundary == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateBoundary: outgoing pointer (IBoundary**) is NULL\n");
		return E_POINTER;
	}

	*ppBoundary = new CComBoundary;

	if (*ppBoundary == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateBoundary: failed to allocate IBoundary\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT(*ppBoundary);

	(*ppBoundary)->AddRef();

	hr = (*ppBoundary)->put_World(pWorld);

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppBoundary);

	return hr;
}

HRESULT CreateBoundaryList(IWorld* pWorld, IPropertyList** pplist)
{
	HRESULT hr = S_OK;
	static CComBSTR bstrBoundaryList("Boundary List");

	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "CreateBoundaryList\n");

	ASSERT(pplist);
	if (pplist == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateBoundaryList: outgoing pointer (IPropertyList**) is NULL\n");
		return E_POINTER;
	}

	hr = ::CreatePropertyList(pWorld, pplist);
	if (FAILED(hr)) 
		goto ERROR_ENCOUNTERED;

	ASSERT(*pplist);

	hr = (*pplist)->put_Type(bstrBoundaryList);
	if (FAILED(hr)) 
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*pplist);

	return hr;
};
#else
HRESULT CreateGeometry(IWorld* pWorld, BSTR bstrGeometryName, IVWGeometry **ppgeometry)
{
	HRESULT hr = S_OK;

	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "CreateGeometry\n");

	ASSERT(ppgeometry);
	if (ppgeometry == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateGeometry: outgoing pointer (IVWGeometry**) is NULL\n");
		return E_POINTER;
	}

	*ppgeometry = NULL;

	hr = pWorld->CreateObjectPropertyExt(CLSID_VWGeometry, NULL, (IObjectProperty**)ppgeometry);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (*ppgeometry == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateGeometry: failed to create object property\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	hr = (*ppgeometry)->put_GeometryName(bstrGeometryName);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppgeometry);

	return hr;
};

HRESULT CreateFrame(IWorld* pWorld, IVWFrame **ppframe)
{
	HRESULT hr = S_OK;

	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "CreateFrame\n");

	ASSERT(ppframe);
	if (ppframe == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateFrame: outgoing pointer (IVWFrame**) is NULL\n");
		return E_POINTER;
	}

	*ppframe = NULL;

	hr = pWorld->CreateObjectPropertyExt(CLSID_VWFrame, NULL, (IObjectProperty**)ppframe);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (*ppframe == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateFrame: failed to create object property\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppframe);

	return hr;
};


HRESULT CreateSpritedGeometry(IWorld* pWorld, BSTR bstrGeometryName, IVWGeometrySprite **ppgeometry)
{
	HRESULT hr = S_OK;

	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "CreateSpritedGeometry\n");

	ASSERT(ppgeometry);
	if (ppgeometry == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateSpritedGeometry: outgoing pointer (IVWGeometrySprite**) is NULL\n");
		return E_POINTER;
	}

	*ppgeometry = NULL;

	hr = pWorld->CreateObjectPropertyExt(CLSID_VWGeometrySprite, NULL, (IObjectProperty**)ppgeometry);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (*ppgeometry == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateSpritedGeometry: failed to create object property\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	hr = (*ppgeometry)->put_GeometryName(bstrGeometryName);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppgeometry);

	return hr;
};

HRESULT CreateVector(IWorld* pWorld, float x, float y, float z, IVector** ppvector)
{
	HRESULT hr = S_OK;

	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "CreateVector\n");

	ASSERT(ppvector);
	if (ppvector == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateVector: outgoing pointer (IVector**) is NULL\n");
		return E_POINTER;
	}

	*ppvector = NULL;

	hr = pWorld->CreateObjectPropertyExt(CLSID_Vector, NULL, (IObjectProperty**)ppvector);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (*ppvector == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateVector: failed to create object property\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	hr = (*ppvector)->set(x, y, z);

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppvector);

	return hr;
}

HRESULT CreateAccessoryData(IWorld* pWorld, BSTR bstrAccessoryGraphic, float fPosX, float fPosY, IVWAccessoryData **ppaccessorydata)
{
	HRESULT hr = S_OK;

	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "CreateAccessoryData\n");

	ASSERT(ppaccessorydata);
	if (ppaccessorydata == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateAccessoryData: outgoing pointer (IVWAccessoryData**) is NULL\n");
		return E_POINTER;
	}

	*ppaccessorydata = NULL;

	hr = pWorld->CreateObjectPropertyExt(CLSID_VWAccessoryData, NULL, (IObjectProperty**)ppaccessorydata);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (*ppaccessorydata == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateAccessoryData: failed to create object property\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	hr = (*ppaccessorydata)->put_AccessoryGraphic(bstrAccessoryGraphic);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppaccessorydata)->PutPos(fPosX, fPosY);

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppaccessorydata);

	return hr;
}


HRESULT CreateSound(IWorld* pWorld,BSTR SoundURL, VARIANT_BOOL bIs3D,VARIANT_BOOL bIsLooping, ISound **ppSound)
{
	HRESULT hr = S_OK;

	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "CreateSound\n");

	ASSERT(ppSound);
	if (ppSound == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateSound: outgoing pointer (ISound**) is NULL\n");
		return E_POINTER;
	}

	hr = pWorld->CreateObjectPropertyExt(CLSID_Sound, NULL, (IObjectProperty**)ppSound);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (*ppSound == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateSound: failed to create object property\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	hr = (*ppSound)->put_SoundURL(SoundURL);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppSound)->put_Is3D(bIs3D);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = (*ppSound)->put_IsLooping(bIsLooping);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;


ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppSound);

	return hr;
}

HRESULT CreateBoundary(IWorld* pWorld, IBoundary** ppBoundary)
{
	HRESULT hr = S_OK;

	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "CreateBoundary\n");

	ASSERT(ppBoundary);
	if (ppBoundary == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateBoundary: outgoing pointer (IBoundary**) is NULL\n");
		return E_POINTER;
	}

	hr = pWorld->CreateObjectPropertyExt(CLSID_Boundary, NULL, (IObjectProperty**)ppBoundary);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (*ppBoundary == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateBoundary: failed to create IBoundary\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppBoundary);

	return hr;
}

HRESULT CreatePickData(IWorld* pWorld, IVWPickData** ppPickData)
{
	HRESULT hr = S_OK;

	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "CreatePickData\n");

	ASSERT(ppPickData);
	if (ppPickData == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreatePickData: outgoing pointer (IVWPickData**) is NULL\n");
		return E_POINTER;
	}

	hr = pWorld->CreateObjectPropertyExt(CLSID_VWPickData, NULL, (IObjectProperty**)ppPickData);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (*ppPickData == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreatePickData: failed to create IVWPickData\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*ppPickData);

	return hr;
}


HRESULT CreateBoundaryList(IWorld* pWorld, IPropertyList** pplist)
{
	HRESULT hr = S_OK;
	static CComBSTR bstrBoundaryList("Boundary List");

	VWTRACE(pWorld, "VWOBJECT", VWT_METHOD, "CreateBoundaryList\n");

	ASSERT(pplist);
	if (pplist == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateBoundaryList: outgoing pointer (IPropertyList**) is NULL\n");
		return E_POINTER;
	}

	hr = pWorld->CreateObjectPropertyExt(CLSID_PropertyList, NULL, (IObjectProperty**)pplist);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (*pplist == NULL)
	{
		VWTRACE(pWorld, "VWOBJECT", VWT_ERROR, "CreateBoundaryList: failed to create objectproperty\n");
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	hr = (*pplist)->put_Type(bstrBoundaryList);


ERROR_ENCOUNTERED:
	if (FAILED(hr))
		SAFERELEASE(*pplist);

	return hr;
};
#endif

//******************************************************
//Helper function to get a number from a variant.
//Used by CreateBoundaryFromArray
HRESULT GetNumericFromVariant(VARIANT& varNumber, float* fDest)
{
	HRESULT hr = S_OK;

	if (!fDest)
		return E_POINTER;

	switch (varNumber.vt)
	{
	case VT_I2:
		*fDest = varNumber.iVal;
		break;
	case VT_I4:
		*fDest = (float) varNumber.lVal;
		break;
	case VT_R4:
		*fDest = varNumber.fltVal;
		break;
	case VT_R8:
		*fDest = (float) varNumber.dblVal;
		break;
	default:
		*fDest = 0;
		hr = E_INVALIDARG;
	}

	return hr;
}
