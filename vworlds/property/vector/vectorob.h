// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VectorOb.h : Declaration of the CVectorObject

#include <resource.h>
#include <PropBase.h>
#include <objimpl.h>

EXTERN_C const IID LIBID_VWMMLib;
EXTERN_C const CLSID CLSID_Vector;

/////////////////////////////////////////////////////////////////////////////
// Vector

DECLARE_OBJECTPROPERTY(Vector, LIBID_VWMMLib, IDS_VECTOR_TYPE)
{
DECLARE_AGGREGATABLE(CVectorObject)

public:
	CVectorObject();
	~CVectorObject();

BEGIN_COM_MAP(CVectorObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectProperty)
	COM_INTERFACE_ENTRY(IVector)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

//DECLARE_NOT_AGGREGATABLE(CVectorObject) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

DECLARE_REGISTRY(CVectorObject, _T("VWSYSTEM.Vector.1"), _T("VWSYSTEM.Vector"), IDS_VECTOR_DESC, THREADFLAGS_BOTH)

	// Override the base class
	STDMETHOD(ToAscii)(BSTR* pbstr); 
	STDMETHOD(Marshall)(DWORD dwOptions, IMarshallBuffer* pbuffer); 
	STDMETHOD(UnMarshall)(IUnMarshallBuffer* pbuffer); 
	STDMETHOD(MarshallToScript)(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut);
	STDMETHOD(get_RefCount)(DWORD *pdwRef);
	STDMETHOD(Terminate)(void);
	
// IVector
	STDMETHOD(get_x)(float *pfx);
	STDMETHOD(put_x)(float fx);
	STDMETHOD(get_y)(float *pfy);
	STDMETHOD(put_y)(float fy);
	STDMETHOD(get_z)(float *pfz);
	STDMETHOD(put_z)(float fz);
	STDMETHOD(set)(float fx, float fy, float fz);
	STDMETHOD(get)(float *pfx, float *pfy, float *pfz);
	STDMETHOD(Add)(IVector *pv, IVector *pvres);
	STDMETHOD(Subtract)(IVector *pv, IVector *pvres);
	STDMETHOD(Normalize)(IVector *pvres);
	STDMETHOD(Scale)(float scale, IVector *pvres);
	STDMETHOD(Length)(float *pfl);
	STDMETHOD(get_Rotation)(float *pfTheta);
	STDMETHOD(Rotate)(float fTheta, IVector *pvres);
	STDMETHOD(RotateAboutAxis)(float fTheta, float fAxisX, float fAxisY, float fAxisZ);
	STDMETHOD(GetEulerAngles)(VARIANT* fAngleX, VARIANT* fAngleY, VARIANT* fAngleZ);


private:
	float	m_x;
	float	m_y;
	float	m_z;
};
