// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _D2DRMArr_h
#define _D2DRMArr_h
// File:	D2DRMArr.h
// Author:	MM    (MM)
//
// Description:
//    This file contains all of the array class definitions.
// 
// History:
// -@- 11/12/96 (MM) created

#include "resource.h"       // main symbols
#include "d2dbase.h"

/////////////////////////////////////////////////////////////////////////////
// d2d

typedef struct ArrayElement {
	LPDIRECT2DRMOBJECT	pObject;
	DWORD				nType;
} ArrayElement;

class ATL_NO_VTABLE CDirect2DRMArray :
	public CComDualImpl<ID2DRMArrayPrivate, &IID_IDirect2DRMArray, &LIBID_D2DLib>,
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<CDirect2DRMArray, &CLSID_CDirect2DRMArray>
{
public:
	CDirect2DRMArray();
	~CDirect2DRMArray();

	BEGIN_COM_MAP(CDirect2DRMArray)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IDirect2DRMObject)
		COM_INTERFACE_ENTRY(IDirect2DRMArray)
		COM_INTERFACE_ENTRY(ID2DRMArrayPrivate)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
	END_COM_MAP()
	
	D2DOBJECT_BASE_CLASS(Array)

	D2DOBJREG_DECL(Array, D2DRMARR)
	
public:
	//
	// IDirect2DRMArray Interface
	//
	STDMETHOD(get_Size(DWORD *pnSize));
	STDMETHOD(GetElement(DWORD nIndex, DWORD *pnType, LPDIRECT2DRMOBJECT *ppD2DRMObject));
	STDMETHOD(GetDevice(DWORD nIndex, LPDIRECT2DRMDEVICE *ppD2DRMDevice)) {
		DWORD nType;
		HRESULT hr;
		hr = GetElement(nIndex, &nType, (LPDIRECT2DRMOBJECT *) ppD2DRMDevice);
		MMASSERT(nType == d2dotDevice);
		return hr;
	}
	STDMETHOD(GetViewport(DWORD nIndex, LPDIRECT2DRMVIEWPORT *ppD2DRMViewport)) {
		DWORD nType;
		HRESULT hr;
		hr = GetElement(nIndex, &nType, (LPDIRECT2DRMOBJECT *) ppD2DRMViewport);
		MMASSERT(nType == d2dotViewport);
		return hr;
	}
	STDMETHOD(GetVisual(DWORD nIndex, LPDIRECT2DRMVISUAL *ppD2DRMVisual)) {
		DWORD nType;
		HRESULT hr;
		hr = GetElement(nIndex, &nType, (LPDIRECT2DRMOBJECT *) ppD2DRMVisual);
		MMASSERT(nType == d2dotVisual);
		return hr;
	}
	STDMETHOD(GetFrame(DWORD nIndex, LPDIRECT2DRMFRAME *ppD2DRMFrame)) {
		DWORD nType;
		HRESULT hr;
		hr = GetElement(nIndex, &nType, (LPDIRECT2DRMOBJECT *) ppD2DRMFrame);
		MMASSERT(nType == d2dotFrame);
		return hr;
	}
	STDMETHOD(GetPick(DWORD nIndex, LPDIRECT2DRMPICK *ppD2DRMPick)) {
		DWORD nType;
		HRESULT hr;
		hr = GetElement(nIndex, &nType, (LPDIRECT2DRMOBJECT *) ppD2DRMPick);
		MMASSERT(nType == d2dotPick);
		return hr;
	}

	//
	// ID2DRMArrayPrivate Interface
	//
	STDMETHOD_(DWORD, InsElement)(DWORD nType, LPDIRECT2DRMOBJECT pD2DRMObject)
	{
		ArrayElement ae;
		ae.pObject = pD2DRMObject;
		ae.nType = nType;
		pD2DRMObject->AddRef();		// create a reference to the object
		return m_dsData.Ins(ae);
	}

	STDMETHOD_(void, DelElement)(DWORD nIndex)
	{
		LPDIRECT2DRMOBJECT pObject = m_dsData[nIndex].pObject;
		MMASSERT(pObject);
		pObject->Release();
		m_dsData.Del(nIndex);
	}

	STDMETHOD(Copy)(LPD2DRMARRAYPRIVATE *ppArray);
	STDMETHOD_(void, Fill)(LPVOID pvData)
	{
		m_dsData = *((DSet<ArrayElement> *) pvData);
		// AddRef all of the objects that were copied
		for (int i = m_dsData.Items() - 1; i >= 0; i--)
			m_dsData[i].pObject->AddRef();
	}

	/*
	STDMETHOD(FindElement(LPDIRECT2DRMOBJECT pD2DRMObject, DWORD *pdwIndex)) {
		int nIndex = m_dsData.Find(pD2DRMObject);
		if (nIndex > 0) {
			*pdwIndex = nIndex;
			return S_OK;
		} else
			return D2DRMERR_ITEMNOTFOUND;
	}
	*/
private:
	DSet<ArrayElement>		m_dsData;
};

#endif
