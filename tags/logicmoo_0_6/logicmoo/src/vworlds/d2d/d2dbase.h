// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _d2dbase_h
#define _d2dbase_h

// File:	d2dbase.h
// Author:	MM    (MM)
//
// Description:
//     provide some useful macros for d2d
// 
// History:
// -@- 10/2/96 (MM) created

#define D2D_GENERIC_BASE(x, y, z) \
	public CComDualImpl<I##x, &IID_I##x, &LIBID_##y>, \
	public ISupportErrorInfo, \
	public CComObjectRoot, \
	public CComCoClass<C##z, &CLSID_C##z>

#define D2DOBJECT_BASE(x) D2D_GENERIC_BASE(Direct2DRM##x, D2DLib, Direct2DRM##x)

#define D2DIERRORINFO_DECL(__class) \
DECLARE_AGGREGATABLE(C##__class) \
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid) \
	{ \
		static const IID* arr[] = { &IID_I##__class }; \
		for (int i = 0; i < sizeof(arr)/sizeof(arr[0]); i++) { \
			if (InlineIsEqualGUID(*arr[i],riid)) \
				return S_OK; \
		} \
		return S_FALSE; \
	}

#define D2DOBJMETHODS_DECL() \
	DWORD m_dwAppData; \
	void *m_pvDestroyCallbackArg; \
	D2DRMOBJECTCALLBACK m_pDestroyCallback; \
	STDMETHOD(put_AppData)(DWORD dwData) \
	{\
		m_dwAppData = dwData; \
		return S_OK; \
	}\
	STDMETHOD(get_AppData)(DWORD *pdwData) \
	{\
		*pdwData = m_dwAppData; \
		return S_OK; \
	}\
	STDMETHOD(SetDestroyCallback)(int *pCallback, int *pvArg) \
	{\
		m_pvDestroyCallbackArg = (void *) pvArg; \
		m_pDestroyCallback = (D2DRMOBJECTCALLBACK) pCallback; \
		return S_OK; \
	}\
	STDMETHOD(GetDestroyCallback)(int **pCallback, int **ppvArg) \
	{\
		return E_NOTIMPL; \
	}\


#define D2DOBJINIT() \
do { \
	m_pvDestroyCallbackArg = NULL; \
	m_pDestroyCallback = NULL; \
	m_dwAppData = 0; \
} while (0)

#define D2DOBJDESTROY() \
do { \
	if (m_pDestroyCallback) \
		m_pDestroyCallback(this, m_pvDestroyCallbackArg); \
} while (0)



#define D2DOBJECT_BASE_CLASS(__class_suffix) \
public: \
	D2DIERRORINFO_DECL(Direct2DRM##__class_suffix) \
	D2DOBJMETHODS_DECL()


#define D2DREG_DECL(__class, __filename) DECLARE_REGISTRY(C##__class, _T("D2D."###__class##".1"), _T("D2D."###__class), IDS_##__filename##_DESC, THREADFLAGS_BOTH)
#define D2DOBJREG_DECL(__class_suffix, __filename) D2DREG_DECL(Direct2DRM##__class_suffix, __filename)

#endif