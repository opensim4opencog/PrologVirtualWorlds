// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VectorOb.cpp : Implementation of CVectorApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include <math.h>
#include <d3drmwin.h>
#include <vector.h>
#include "VectorOb.h"

#define PI					3.1415926F
#define PIOVER180			0.0174532F
#define PIOVER2				1.5707963F
#define PITIMES2			6.2831853F

IMPLEMENT_OBJECTPROPERTY(CVectorObject)

/////////////////////////////////////////////////////////////////////////////
//

CVectorObject::CVectorObject()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVectorObject::CVectorObject, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif

	m_x = 0.0F;
	m_y = 0.0F;
	m_z = 0.0F;
}

CVectorObject::~CVectorObject()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVectorObject::~CVectorObject, this 0x%0x\n", this);
#endif
}

STDMETHODIMP CVectorObject::Terminate()
{
#ifdef _DEBUG
	CComBSTR bstrName("n/a");

	if (m_pParent)
	{
		m_pParent->get_Name(&bstrName.m_str);

		VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVectorObject::Terminate, %s.%s, this 0x%0x\n", CString(bstrName), CString(m_bstrPropertyName), this);
	}
#endif

	CVectorObjectBase::Terminate();

	return S_OK;
}

STDMETHODIMP CVectorObject::get_x(float *pfx)
{
	*pfx = m_x;

	return S_OK;
}

STDMETHODIMP CVectorObject::put_x(float fx)
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CVectorObject::put_x\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CVectorObject::put_x", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CVectorObject::put_x: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		m_x = fx;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_R4;
		dispparams.rgvarg[0].fltVal = fx;

		hr = SendObjectPropertyCommand(psbits, DISPID_VECTOR_X, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, veX, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CVectorObject::get_y(float *pfy)
{
	*pfy = m_y;

	return S_OK;
}

STDMETHODIMP CVectorObject::put_y(float fy)
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CVectorObject::put_y\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CVectorObject::put_y", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CVectorObject::put_y: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		m_y = fy;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_R4;
		dispparams.rgvarg[0].fltVal = fy;

		hr = SendObjectPropertyCommand(psbits, DISPID_VECTOR_Y, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, veY, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CVectorObject::get_z(float *pfz)
{
	*pfz = m_z;

	return S_OK;
}

STDMETHODIMP CVectorObject::put_z(float fz)
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CVectorObject::put_z\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CVectorObject::put_z", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CVectorObject::put_z: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		m_z = fz;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_R4;
		dispparams.rgvarg[0].fltVal = fz;

		hr = SendObjectPropertyCommand(psbits, DISPID_VECTOR_Z, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, veZ, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CVectorObject::set(float fx, float fy, float fz)
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWPROPERTY", VWT_METHOD, "CVectorObject::set\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CVectorObject::set", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CVectorObject::set: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		m_x = fx;
		m_y = fy;
		m_z = fz;
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 3);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[2].vt = VT_R4;
		dispparams.rgvarg[2].fltVal = fx;

		dispparams.rgvarg[1].vt = VT_R4;
		dispparams.rgvarg[1].fltVal = fy;

		dispparams.rgvarg[0].vt = VT_R4;
		dispparams.rgvarg[0].fltVal = fz;

		hr = SendObjectPropertyCommand(psbits, DISPID_VECTOR_SET, DISPATCH_METHOD, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (bShouldNotify == VARIANT_TRUE)
	{
		// notify client/server and world of property change
		if (m_pParent)
		{
			hr = m_pParent->NotifyObjectPropertyChanged(this, veSet, CComVariant());
			if (FAILED(hr))
				goto ERROR_ENCOUNTERED;
		}
	}

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CVectorObject::get(float *pfx, float *pfy, float *pfz)
{
	*pfx = m_x;
	*pfy = m_y;
	*pfz = m_z;

	return S_OK;
}

STDMETHODIMP CVectorObject::Add(IVector *pv, IVector *pvres)
{
	HRESULT hr;
	float val, sumx, sumy, sumz;

	if (FAILED(hr = pv->get_x(&val))) 
		goto ERROR_ENCOUNTERED;
	sumx = m_x + val;
		
	if (FAILED(hr = pv->get_y(&val))) 
		goto ERROR_ENCOUNTERED;
	sumy = m_y + val;
	
	if (FAILED(hr = pv->get_z(&val))) 
		goto ERROR_ENCOUNTERED;
	sumz = m_z + val;

	if (FAILED(hr = pvres->set(sumx, sumy, sumz))) 
		goto ERROR_ENCOUNTERED;
	return S_OK;

ERROR_ENCOUNTERED:
	return E_FAIL;
}

STDMETHODIMP CVectorObject::Subtract(IVector *pv, IVector *pvres)
{
	HRESULT hr;
	float val, sumx, sumy, sumz;

	if (FAILED(hr = pv->get_x(&val))) 
		goto ERROR_ENCOUNTERED;
	sumx = m_x - val;
		
	if (FAILED(hr = pv->get_y(&val))) 
		goto ERROR_ENCOUNTERED;
	sumy = m_y - val;
	
	if (FAILED(hr = pv->get_z(&val))) 
		goto ERROR_ENCOUNTERED;
	sumz = m_z - val;

	if (FAILED(hr = pvres->set(sumx, sumy, sumz))) 
		goto ERROR_ENCOUNTERED;
	return S_OK;

ERROR_ENCOUNTERED:
	return E_FAIL;
}

STDMETHODIMP CVectorObject::Normalize(IVector *pvres)
{
	HRESULT hr;
	float norm;

	norm = (float)sqrt((m_x*m_x) + (m_y*m_y) + (m_z*m_z));

	// if 0 length vector, just return - should do this with a float_compare
	if (norm == 0.0) 
		return S_OK;

	if (FAILED(hr = pvres->set(m_x/norm, m_y/norm, m_z/norm))) 
		goto ERROR_ENCOUNTERED;

	return S_OK;

ERROR_ENCOUNTERED:
	return E_FAIL;
}

STDMETHODIMP CVectorObject::Scale(float scale, IVector *pvres)
{
	HRESULT hr;
	
	if (FAILED(hr = pvres->set(m_x*scale, m_y*scale, m_z*scale))) 
		goto ERROR_ENCOUNTERED;

	return S_OK;

ERROR_ENCOUNTERED:
	return E_FAIL;
}

STDMETHODIMP CVectorObject::Length(float *lpf)
{
	*lpf = (float)sqrt((m_x*m_x) + (m_y*m_y) + (m_z*m_z));
	return S_OK;
}

STDMETHODIMP CVectorObject::get_Rotation(float *pfTheta)
{
	D3DVECTOR rlvPreDef, rlvDir;
	D3DVALUE  valCosTheta;

	// The pre-defined angle.
	rlvPreDef.x = 0.0F;
	rlvPreDef.y	= 0.0F;
	rlvPreDef.z	= 1.0F;

	// Make a D3DVECTOR from this vector.
	rlvDir.x = m_x;
	rlvDir.y = m_y;
	rlvDir.z = m_z;

	D3DRMVectorNormalize(&rlvDir);

	// First need angle between the pre-defined angle and rlvDir.
	valCosTheta = D3DRMVectorDotProduct(&rlvPreDef, &rlvDir);
	*pfTheta = (D3DVALUE)acos(valCosTheta);

	// This will always be the acute angle. Since rotation will always be in positive direction we must
	// give correct angle (possibly obtuse) for that direction.
	if (rlvDir.x < 0.0F)
	{
		// Acute angle will not work, need obtuse angle.
		*pfTheta = (2*PI) - *pfTheta;
	}

	return S_OK;
}

STDMETHODIMP CVectorObject::Rotate(float fTheta, IVector *pvres)
{
	D3DVECTOR	rlvThis, rlvRotated, rlvAxis;
	D3DVALUE	valModulus;

	if (!pvres)
		return E_POINTER;

	rlvThis.x = m_x;
	rlvThis.y = m_y;
	rlvThis.z = m_z;
	rlvAxis.x = 0.0F;
	rlvAxis.y = 1.0F;
	rlvAxis.z = 0.0F;

	valModulus = D3DRMVectorModulus(&rlvThis);
	D3DRMVectorRotate(&rlvRotated, &rlvThis, &rlvAxis, fTheta);

	// D3DRMVectorRotate will return a unit vector. We need the original size vector.
	D3DRMVectorScale(&rlvRotated, &rlvRotated, valModulus);

	pvres->set(rlvRotated.x, rlvRotated.y, rlvRotated.z);
	
	return S_OK;
}

STDMETHODIMP CVectorObject::GetEulerAngles(VARIANT* fAngleX, VARIANT* fAngleY, VARIANT* fAngleZ)
{
	HRESULT hr = E_NOTIMPL;
	return hr;
}

STDMETHODIMP CVectorObject::RotateAboutAxis(float fTheta, float fAxisX, float fAxisY, float fAxisZ)
{
	D3DVECTOR	rlvThis, rlvRotated, rlvAxis;
	D3DVALUE	valModulus;
	HRESULT hr = S_OK;

	if (fAxisX == 0.0f && fAxisY == 0.0f && fAxisZ == 0.0f)
		return E_INVALIDARG;

	rlvThis.x = m_x;
	rlvThis.y = m_y;
	rlvThis.z = m_z;
	rlvAxis.x = fAxisX;
	rlvAxis.y = fAxisY;
	rlvAxis.z = fAxisZ;

	valModulus = D3DRMVectorModulus(&rlvThis);
	D3DRMVectorRotate(&rlvRotated, &rlvThis, &rlvAxis, fTheta);

	// D3DRMVectorRotate will return a unit vector. We need the original size vector.
	D3DRMVectorScale(&rlvRotated, &rlvRotated, valModulus);

	hr = set(rlvRotated.x, rlvRotated.y, rlvRotated.z);
	
	return hr;
}

STDMETHODIMP CVectorObject::ToAscii(BSTR* pbstr)
{
	ASSERT(pbstr);

	CString strValue;
	strValue.Format("(%f, %f, %f)", m_x, m_y, m_z);
	*pbstr = strValue.AllocSysString();

	return S_OK;
}

STDMETHODIMP CVectorObject::Marshall(DWORD dwOptions, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = CVectorObjectBase::Marshall( dwOptions, pbuffer )))
		return hr;

	ASSERT(pbuffer);

	hr = pbuffer->put_Float(m_x);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->put_Float(m_y);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->put_Float(m_z);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CVectorObject::MarshallToScript(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut)
{
	CString strOut;

	if (bstrContext==NULL || pbstrOut==NULL || pbAsVal==NULL)
		return E_INVALIDARG;

	strOut.Format(	"\tSet %s = World.CreateVector(%g, %g, %g)\n",
					(LPCTSTR) CString(bstrContext),
					m_x, m_y, m_z);
	
	*pbstrOut = strOut.AllocSysString();
	*pbAsVal = VARIANT_FALSE;

	return S_OK;
}


STDMETHODIMP CVectorObject::UnMarshall(IUnMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = CVectorObjectBase::UnMarshall( pbuffer )))
		return hr;

	ASSERT(pbuffer);

	float fx, fy, fz;

	hr = pbuffer->get_Float(&fx);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->get_Float(&fy);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	hr = pbuffer->get_Float(&fz);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	m_x = fx;
	m_y = fy;
	m_z = fz;

ERROR_ENCOUNTERED:
	return hr;
}
