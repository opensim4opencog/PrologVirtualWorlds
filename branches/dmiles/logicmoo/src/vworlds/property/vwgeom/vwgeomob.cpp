// VWGeomOb.cpp : Implementation of CVWGeomApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include <d3drm.h>
#include <vwgeom.h>
#include <vwgcache.h>
#include "VWGeomOb.h"

IMPLEMENT_OBJECTPROPERTY(CVWGeometryObject)

#define INIT_SAFERETURN HRESULT hr = S_OK;

#define SAFERETURN(hRes) \
if FAILED(hr = (hRes)) \
{ \
	goto ERROR_ENCOUNTERED; \
}

#define FORCE_SAFERETURN goto ERROR_ENCOUNTERED;

#define BEGIN_SAFERETURN ERROR_ENCOUNTERED:

#define END_SAFERETURN return hr;

CComBSTR CVWGeometryObject::m_bstrVWFrame("VWSYSTEM.Frame.1");

/////////////////////////////////////////////////////////////////////////////
//

CVWGeometryObject::CVWGeometryObject()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVWGeometryObject::CVWGeometryObject, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif

	m_pvwfTransform = NULL;
	m_pvwfCachedGeometry = NULL;
	m_pVWGeometryCache = NULL;
	m_bOccluded = FALSE;
}

CVWGeometryObject::~CVWGeometryObject()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVWGeometryObject::~CVWGeometryObject, this 0x%0x\n", this);
#endif

	Terminate();
}

STDMETHODIMP CVWGeometryObject::Terminate(void)
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVWGeometryObject::Terminate, this 0x%0x\n", this);
#endif

	CVWGeometryObjectBase::Terminate();

	SAFERELEASE(m_pVWGeometryCache);

	return S_OK;
}

STDMETHODIMP CVWGeometryObject::ComposeGeometry(IVWFrame **ppvwf)
{
	// REVIEW: For now assume we always return 3D geometry.
	INIT_SAFERETURN
	IThing	*pThing = NULL;
	static CComBSTR bstrGeometryCache("VWGeometryCache");

	if (!m_pVWGeometryCache)
	{
		// One time obtain of static GeometryCache object.
		// Release after obtaining 'cause we know it'll be around for as long as any 
		// geom object and 'cause it's static we'd have no decent place to release 
		// it anyway.
		SAFERETURN(m_pWorld->get_Tool(bstrGeometryCache, (IUnknown**)&m_pVWGeometryCache));
	}

	if (m_pvwfTransform)
	{
//		TRACE("Returning another reference to TNode %s\n", CString(m_bstrGeometryName));

		// Use existing 'T' node.
		*ppvwf = m_pvwfTransform;
		m_pvwfTransform->AddRef();
	}
	else
	{
		// Create an empty 'T' node.
		SAFERETURN(m_pWorld->CreateObjectProperty(m_bstrVWFrame, (IObjectProperty**)&m_pvwfTransform));
//		TRACE("Storing and returning TNode %s\n", CString(m_bstrGeometryName));
		m_pvwfTransform->put_GeometryObject((IVWGeometry*)this);
		if (m_pSecurity)
		{
			m_pSecurity->get_Instance(&pThing);
			m_pvwfTransform->put_Thing(pThing);
		}
		m_pvwfTransform->AddRef();
		*ppvwf = m_pvwfTransform;

		if ((BSTR)m_bstrGeometryName != NULL && m_bstrGeometryName.Length() != 0 && !m_bOccluded)
		{
			// Load the named geometry and attach it to the 'T' node.
			if (FAILED(hr = m_pVWGeometryCache->LoadGeometry(m_bstrGeometryName, &m_pvwfCachedGeometry)) ||
				FAILED(hr = m_pvwfTransform->AddChild(m_pvwfCachedGeometry)) ||
				FAILED(hr = m_pvwfCachedGeometry->SetPositionEx(m_pvwfTransform, 0.0F, 0.0F, 0.0F)) ||
				FAILED(hr = m_pvwfCachedGeometry->SetOrientationEx(m_pvwfTransform, 0.0F, 0.0F, 1.0F, 0.0F, 1.0F, 0.0F)))
			{
				// Hmmm, failed loading the geometry. We have a good T node to pass back
				// so don't error out, just report it.
				m_pWorld->Report(CComBSTR(CString("VWGeometryObject: Failed to load geometry: ") + CString(m_bstrGeometryName.m_str)), -1);
				hr = S_OK;
				goto ERROR_ENCOUNTERED;
			}
		}
	}

ERROR_ENCOUNTERED:

	SAFERELEASE(pThing);

	return hr;
}

STDMETHODIMP CVWGeometryObject::ReleaseGeometry()
{
	if (!m_pvwfTransform)
		return E_FAIL;

	// Use stack variables since this object may be deleted when we remove the VWFrame's
	// reference to it.
	IVWFrame *pvwfTransform = m_pvwfTransform, *pvwfCachedGeometry = m_pvwfCachedGeometry;
	m_pvwfTransform = NULL;
	m_pvwfCachedGeometry = NULL;

	pvwfTransform->put_GeometryObject(NULL);
	SAFERELEASE(pvwfTransform);
	SAFERELEASE(pvwfCachedGeometry);

	return S_OK;
}

STDMETHODIMP CVWGeometryObject::put_GeometryName(BSTR bstr)
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CVWGeometryObject::put_GeometryName\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CVWGeometryObject::put_GeometryName", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CVWGeometryObject::put_GeometryName: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		// Actually set the geomtry
		m_bstrGeometryName = bstr;

		if (m_pvwfTransform && !m_bOccluded)
		{
			if (m_pvwfCachedGeometry)
			{
				m_pvwfTransform->DeleteChild(m_pvwfCachedGeometry);
				SAFERELEASE(m_pvwfCachedGeometry);
			}
			if ((BSTR)m_bstrGeometryName != NULL && m_bstrGeometryName.Length() != 0)
			{
				// Load the named geometry.
				if (SUCCEEDED(m_pVWGeometryCache->LoadGeometry(m_bstrGeometryName, &m_pvwfCachedGeometry)))
				{
					m_pvwfTransform->AddChild(m_pvwfCachedGeometry);
					m_pvwfCachedGeometry->SetPositionEx(m_pvwfTransform, 0.0F, 0.0F, 0.0F);
					m_pvwfCachedGeometry->SetOrientationEx(m_pvwfTransform, 0.0F, 0.0F, 1.0F, 0.0F, 1.0F, 0.0F);
				}
			}
		}
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		DISPPARAMS dispparams;

		// package up args into DISPPARAMS struct
		hr = InitializeDISPPARAMS(&dispparams, 1);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED;

		dispparams.rgvarg[0].vt = VT_BSTR;
		dispparams.rgvarg[0].bstrVal = bstr;

		hr = SendObjectPropertyCommand(psbits, DISPID_GEOMETRY_GEOMETRYNAME, DISPATCH_PROPERTYPUT, &dispparams);

		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (bShouldNotify == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			// notify client/server and world of property change
			if (FAILED(hr = m_pParent->NotifyObjectPropertyChanged(this, glPutGeometryName, CComVariant(bstr))))
				goto ERROR_ENCOUNTERED;
		}
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CVWGeometryObject::get_GeometryName(BSTR* pbstr)
{
	*pbstr = m_bstrGeometryName.Copy();
	return S_OK;
}

STDMETHODIMP CVWGeometryObject::get_Frame(IVWFrame **ppVWFrame)
{
	*ppVWFrame = m_pvwfTransform;
	SAFEADDREF(*ppVWFrame);
	return S_OK;
}

STDMETHODIMP CVWGeometryObject::get_GeometryFrame(IVWFrame **ppVWFrame)
{
	*ppVWFrame = m_pvwfCachedGeometry;
	SAFEADDREF(*ppVWFrame);
	return S_OK;
}

STDMETHODIMP CVWGeometryObject::get_NamedGesture(BSTR bstrGesture, int *pGestureNum)
{
	*pGestureNum = 0;
	return E_NOTIMPL;
}

STDMETHODIMP CVWGeometryObject::get_GestureText(BSTR bstrGesture, BSTR *pbstrText)
{
	*pbstrText = NULL;
	return E_NOTIMPL;
}


STDMETHODIMP CVWGeometryObject::put_Frame(IVWFrame *pVWFrame)
{
	IThing *pThing = NULL;
	m_pvwfTransform->put_GeometryObject(NULL);
	SAFERELEASE(m_pvwfTransform);
	m_pvwfTransform = pVWFrame;
	SAFEADDREF(pVWFrame);
	m_pvwfTransform->put_GeometryObject((IVWGeometry*)this);
	if (m_pSecurity)
	{
		m_pSecurity->get_Instance(&pThing);
		m_pvwfTransform->put_Thing(pThing);
	}
	SAFERELEASE(pThing);
	return S_OK;
}

STDMETHODIMP CVWGeometryObject::put_Occluded(VARIANT_BOOL bOccluded)
{
	HRESULT hr = S_OK;

	if (bOccluded != m_bOccluded && m_pvwfTransform)
	{
		if (!bOccluded)
		{
			if ((BSTR)m_bstrGeometryName != NULL && m_bstrGeometryName.Length() != 0)
			{
				// Load the named geometry.
				SAFERETURN(m_pVWGeometryCache->LoadGeometry(m_bstrGeometryName, &m_pvwfCachedGeometry));
				SAFERETURN(m_pvwfTransform->AddChild(m_pvwfCachedGeometry));
				if (FAILED(hr = m_pvwfCachedGeometry->SetPositionEx(m_pvwfTransform, 0.0F, 0.0F, 0.0F)))
					goto ERROR_ENCOUNTERED;
				if (FAILED(hr = m_pvwfCachedGeometry->SetOrientationEx(m_pvwfTransform, 0.0F, 0.0F, 1.0F, 0.0F, 1.0F, 0.0F)))
					goto ERROR_ENCOUNTERED;
			}
		}
		else if (m_pvwfCachedGeometry)
		{
			SAFERETURN(m_pvwfTransform->DeleteChild(m_pvwfCachedGeometry));
			SAFERELEASE(m_pvwfCachedGeometry);
		}
	}
	m_bOccluded = bOccluded;

ERROR_ENCOUNTERED:

	return hr;
}

STDMETHODIMP CVWGeometryObject::Marshall(DWORD dwOptions, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	ASSERT(pbuffer);

	if (FAILED(hr = CVWGeometryObjectBase::Marshall(dwOptions, pbuffer )))
		return hr;

	hr = pbuffer->put_String(m_bstrGeometryName);

	return hr;
}

STDMETHODIMP CVWGeometryObject::UnMarshall(IUnMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	// free any alloced data
	m_bstrGeometryName.Empty();
	if (m_pvwfTransform)
		m_pvwfTransform->put_GeometryObject(NULL);
	SAFERELEASE(m_pvwfTransform);
	SAFERELEASE(m_pvwfCachedGeometry);

	ASSERT(pbuffer);

	if (FAILED(hr = CVWGeometryObjectBase::UnMarshall( pbuffer )))
		return hr;

	hr = pbuffer->get_String(&m_bstrGeometryName.m_str);
	if (FAILED(hr))
		return hr;

	return hr;
}
 
STDMETHODIMP CVWGeometryObject::MarshallToScript(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut)
{
	CString strOut;

	if (bstrContext==NULL || pbstrOut==NULL || pbAsVal==NULL)
		return E_INVALIDARG;
											
	strOut.Format(	"\tSet %s = World.CreateGeometry(\"%s\")\n",
					(LPCTSTR) CString(bstrContext),
					(LPCTSTR) CString(m_bstrGeometryName));

	*pbstrOut = strOut.AllocSysString();
	*pbAsVal = VARIANT_FALSE;

	return S_OK;
}

/*
NOTE: None of the following methods are implemented for 3d geometry...
  */

STDMETHODIMP CVWGeometryObject::GetNamedGestures(IPropertyList **ppGestureNameList) {
	return E_NOTIMPL;
}

STDMETHODIMP CVWGeometryObject::put_CurrentGesture(int iGesture) {
	return E_NOTIMPL;
}

STDMETHODIMP CVWGeometryObject::get_CurrentGesture(int* piGesture) {
	return E_NOTIMPL;
}

