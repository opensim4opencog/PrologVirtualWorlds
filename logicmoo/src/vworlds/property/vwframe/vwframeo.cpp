// VWFrameO.cpp : Implementation of CVWFrameApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include <d2d.h>
#include <math.h>
#include <VWFrame.h>
#include <vwidata.h>
#include "VWFrameO.h"
#include "BBox.h"
#include "syshelp.h"
#include "mmhelp.h"
#include <inetfile.h>
#include <d3drmwin.h>

#include "vwutils.h"

#ifdef _DXMEDIA
#include <ddraw.h>
#include <mmsystem.h>
#include <mmstream.h>
#include <amstream.h>
#include <ddstream.h>
#include <dxtrans.h>
#include <dxtmsft.h>
#include <dxtmsftguid.h>
#include <dxtguid.c>
#endif//VWORLDS_1_5

#define PI 3.1415926F
#define OLERETURN(hRet) if (FAILED(hRet)) { ASSERT(FALSE); return hRet; }

// Create the 3D frame for the object.
#define CHECKANDCREATEFRAME  if (!m_pd3drmf)										\
							 {														\
								 hr = m_pDirect3DRM->CreateFrame(NULL, &m_pd3drmf); \
								 if (FAILED(hr) || !m_pd3drmf) goto EXIT_FAIL;		\
								 m_pd3drmf->SetAppData((DWORD)this);				\
							 }

#define CHECKFRAME  if (!m_pd3drmf)																		\
					{																					\
						VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CVWFrameObject: No D3DRMFrame.\n");	\
						hr = E_FAIL;																	\
						goto EXIT_FAIL;																	\
					}

#define CHECK_IN_POINTER(xxx)  do { ASSERT(xxx); if (!xxx) { hr = E_INVALIDARG; goto EXIT_FAIL; } } while(FALSE);
#define CHECK_OUT_POINTER(xxx)  do { ASSERT(xxx); if (!xxx) { hr = E_POINTER; goto EXIT_FAIL; } } while(FALSE);

#define MACSTART do {
#define MACEND } while(0)
#define MMINITSTRUCT(__s) memset(&(__s), 0, sizeof(__s))
#ifndef INIT_DXSTRUCT
#define INIT_DXSTRUCT(__dxstruct) MACSTART \
	MMINITSTRUCT(__dxstruct); (__dxstruct).dwSize = sizeof(__dxstruct); MACEND
#endif


HRESULT D3DFrameGetVisual(IDirect3DRMFrame *pd3drmf, REFIID ref, void** visualObj, int cWhich);
HRESULT D3DFrameSetMeshQuality(IDirect3DRMFrame *pd3drmf, long nMeshIndex, int nQuality);
HRESULT D3DFrameCopy(IDirect3DRM *pd3drm, LPDIRECT3DRMFRAME lpSrcFrame, LPDIRECT3DRMFRAME *ppDstFrame, LPDIRECT3DRMANIMATIONSET2 pSrcAnimSet, LPDIRECT3DRMANIMATIONSET2 pDestAnimSet, IVWFrame* lpVWRootFrame);
HRESULT D3DFrameSetMeshColor(IDirect3DRMFrame *pd3drmf, long nMeshIndex, float fRed, float fGreen, float fBlue);
HRESULT D3DFrameSetProgressiveMeshDetail(IDirect3DRMFrame *pd3drmf, long nMeshIndex, float detail);
HRESULT D3DFrameSetProgressiveMeshQuality(IDirect3DRMFrame *pd3drmf, long nMeshIndex, int nQuality);
HRESULT D3DFrameSetProgressiveMeshFaceDetail(IDirect3DRMFrame *pd3drmf, long nMeshIndex, DWORD detail);
HRESULT D3DFrameSetProgressiveMeshVertexDetail(IDirect3DRMFrame *pd3drmf, long nMeshIndex, DWORD detail);
HRESULT D3DFrameSetMeshTexture(IDirect3DRMFrame *pd3drmf, long nMeshIndex, LPDIRECT3DRMTEXTURE pTexture);
HRESULT D3DFrameSetDecalSize(IDirect3DRMFrame *pd3drmf, long nMeshIndex, float fHeight, float fWidth, VARIANT_BOOL bScale);
HRESULT D3DFrameSetDecalOrigin(IDirect3DRMFrame *pd3drmf, long nMeshIndex, long lx, long ly);
HRESULT D3DFrameSetMeshPerspective(IDirect3DRMFrame *pd3drmf, long nMeshIndex, VARIANT_BOOL vbPersp);
HRESULT D3DFrameSetLightAttenuation(IDirect3DRMFrame *pd3drmf, long nMeshIndex, float fConst, float fLinear, float fQuadratic);
HRESULT D3DFrameSetLightColor(IDirect3DRMFrame *pd3drmf, long nMeshIndex, float fRed, float fGreen, float fBlue);
HRESULT D3DFrameSetLightType(IDirect3DRMFrame *pd3drmf, long nMeshIndex, D3DRMLIGHTTYPE d3drmtType);
HRESULT D3DFrameSetLightRange(IDirect3DRMFrame *pd3drmf, long nMeshIndex, float fRange);
HRESULT D3DFrameSetLightRadii(IDirect3DRMFrame *pd3drmf, long nMeshIndex, float fUmbra,  float fPenumbra);

HRESULT GetLight(LPDIRECT3DRMFRAME pd3drmf, long nIndex, LPDIRECT3DRMLIGHT* ppLight);

typedef CVWComPtr<IVector, NULL, &IID_IVector> CVectorPtr;

IMPLEMENT_OBJECTPROPERTY(CVWFrameObject)

/////////////////////////////////////////////////////////////////////////////
//

CVWFrameObject::CVWFrameObject()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVWFrameObject::CVWFrameObject, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif

	m_pd3drmf = NULL;
	m_pd3drmfClone = NULL;
	m_pd2drmf = NULL;
	m_pBoundaryList = NULL;
	m_pVWGeometryCache = NULL;
	m_pVWGeometryObject = NULL;
	m_pThing = NULL;
	m_pDirect3DRM = NULL;
	m_pDirect2DRM = NULL;
	m_pVWAnimator = NULL;
	m_fScaleX = m_fScaleY = m_fScaleZ = 1.0F;
#ifdef _DXMEDIA
	m_pMMStream = NULL;
	m_pSample = NULL;
	m_pVidTexture = NULL;
	m_pVidSurface = NULL;
	m_hVidMutex = CreateMutex(NULL,FALSE,NULL);	
	m_hExpMutex = CreateMutex(NULL,FALSE,NULL);
	m_pEffect = NULL;
	m_pExploder = NULL;
	m_pExpMeshBuilder = NULL;
	m_pExpMeshBuilderOrig = NULL;
#endif
	m_pAnimationSet = NULL;
	InitializeCriticalSection(&m_CSChildren);
}

CVWFrameObject::~CVWFrameObject()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVWFrameObject::~CVWFrameObject, this 0x%0x\n", this);
#endif
	
	Terminate();
}

STDMETHODIMP CVWFrameObject::Terminate(void)
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVWFrameObject::Terminate, this 0x%0x\n", this);
#endif

	CVWFrameObjectBase::Terminate();

	// This object is always responsible for any 3D/2D frames it creates or that have
	// been passed to it.
	SAFERELEASE(m_pd3drmf);
	SAFERELEASE(m_pd3drmfClone);
	SAFERELEASE(m_pd2drmf);
	SAFERELEASE(m_pBoundaryList);
	SAFERELEASE(m_pThing);
	SAFERELEASE(m_pDirect3DRM);
	SAFERELEASE(m_pDirect2DRM);
	SAFERELEASE(m_pVWAnimator);
#ifdef _DXMEDIA
	SAFERELEASE(m_pMMStream);
	SAFERELEASE(m_pSample);
	SAFERELEASE(m_pVidTexture);
	SAFERELEASE(m_pVidSurface);
	CloseHandle(m_hVidMutex);
	CloseHandle(m_hExpMutex);
	SAFERELEASE(m_pEffect);
	SAFERELEASE(m_pExploder);
	SAFERELEASE(m_pExpMeshBuilder);
	SAFERELEASE(m_pExpMeshBuilderOrig);
#endif
	DeleteCriticalSection(&m_CSChildren);
	return S_OK;
}

// IVWTransform
STDMETHODIMP CVWFrameObject::SetPosition(IVWFrame *pvwfRef, float flX, float flY, float flZ)
{
	HRESULT		hr = S_OK;
	CHECKANDCREATEFRAME;
	hr = SetPositionEx(pvwfRef, flX, flY, flZ);
EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::SetPosition(float flX, float flY, float flZ)
{
	IVWFrame	*pvwfParent = NULL;
	HRESULT		hr = S_OK;

	CHECKANDCREATEFRAME;

	GetParent(&pvwfParent);
	if (!pvwfParent)
	{
		hr = E_FAIL;
		goto EXIT_FAIL;
	}
	hr = SetPositionEx(pvwfParent, flX, flY, flZ);

EXIT_FAIL:

	SAFERELEASE(pvwfParent);
	
	return hr;
}

STDMETHODIMP CVWFrameObject::GetPosition(float * pflX, float * pflY, float * pflZ)
{
	IVWFrame	*pvwfParent = NULL;
	HRESULT		hr = S_OK;

	CHECK_OUT_POINTER(pflX);
	CHECK_OUT_POINTER(pflY);
	CHECK_OUT_POINTER(pflZ);

	CHECKANDCREATEFRAME;

	GetParent(&pvwfParent);
	if (!pvwfParent)
	{
		hr = E_FAIL;
		goto EXIT_FAIL;
	}
	hr = GetPositionEx(pvwfParent, pflX, pflY, pflZ);

EXIT_FAIL:
	SAFERELEASE(pvwfParent);
	
	return hr;
}

STDMETHODIMP CVWFrameObject::SetScale(IVWFrame *pvwfRef, float flX, float flY, float flZ)
{
	HRESULT		hr = S_OK;
	CHECKANDCREATEFRAME;
	hr = SetScaleEx(pvwfRef, flX, flY, flZ);
EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::SetScale(float flX, float flY, float flZ)
{
	IVWFrame	*pvwfParent = NULL;
	HRESULT		hr = S_OK;

	CHECKANDCREATEFRAME;

	GetParent(&pvwfParent);
	if (!pvwfParent)
	{
		hr = E_FAIL;
		goto EXIT_FAIL;
	}
	hr = SetScaleEx(pvwfParent, flX, flY, flZ);

EXIT_FAIL:
	SAFERELEASE(pvwfParent);
	
	return hr;
}

STDMETHODIMP CVWFrameObject::GetScale(float * pflX, float * pflY, float * pflZ)
{
	return E_NOTIMPL;
}

STDMETHODIMP CVWFrameObject::SetOrientation(IVWFrame *pvwfRef, float flXDir, float flYDir, float flZDir,
											 float flXUp, float flYUp, float flZUp)
{
	HRESULT		hr = S_OK;
	CHECKANDCREATEFRAME;
	hr = SetOrientationEx(pvwfRef, flXDir, flYDir, flZDir, flXUp, flYUp, flZUp);
EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::SetOrientation(float flXDir, float flYDir, float flZDir,
											 float flXUp, float flYUp, float flZUp)
{
	IVWFrame	*pvwfParent = NULL;
	HRESULT		hr = S_OK;

	CHECKANDCREATEFRAME;

	GetParent(&pvwfParent);
	if (!pvwfParent)
	{
		hr = E_FAIL;
		goto EXIT_FAIL;
	}
	hr = SetOrientationEx(pvwfParent, flXDir, flYDir, flZDir, flXUp, flYUp, flZUp);

EXIT_FAIL:

	SAFERELEASE(pvwfParent);
	
	return hr;
}

STDMETHODIMP CVWFrameObject::GetOrientation(float * pflXDir, float * pflYDir, float * pflZDir,
											 float * pflXUp, float * pflYUp, float * pflZUp)
{
	IVWFrame	*pvwfParent = NULL;
	HRESULT		hr = S_OK;

	CHECK_OUT_POINTER(pflXDir);
	CHECK_OUT_POINTER(pflYDir);
	CHECK_OUT_POINTER(pflZDir);
	CHECK_OUT_POINTER(pflXUp);
	CHECK_OUT_POINTER(pflYUp);
	CHECK_OUT_POINTER(pflZUp);

	CHECKANDCREATEFRAME;

	GetParent(&pvwfParent);
	if (!pvwfParent)
	{
		hr = E_FAIL;
		goto EXIT_FAIL;
	}

	hr = GetOrientationEx(pvwfParent, pflXDir, pflYDir, pflZDir, pflXUp, pflYUp, pflZUp);

EXIT_FAIL:

	SAFERELEASE(pvwfParent);
	
	return hr;
}

// IVWTransform
STDMETHODIMP CVWFrameObject::AddChild(IVWFrame *pvwfChild)
{
	IDirect3DRMFrame *pd3drmfChild = NULL, *pd3drmfParent = NULL;
	IDirect2DRMFrame *pd2drmfChild = NULL, *pd2drmfParent = NULL;
	HRESULT hr = S_OK;

	CHECK_IN_POINTER(pvwfChild);

	// Note: Can only have 2D -> 3D link and must be at root scene.
	CHECKANDCREATEFRAME;

	if(FAILED(hr = pvwfChild->get_Frame3D(&pd3drmfChild)))
		goto EXIT_FAIL;
	if(FAILED(hr = pvwfChild->get_Frame2D(&pd2drmfChild)))
		goto EXIT_FAIL;

	if ((pd3drmfChild || m_pd3drmf) || 
		(!pd3drmfChild && !pd2drmfChild && !m_pd3drmf && !m_pd2drmf))
	{
		if (!m_pd3drmf)
		{
			// Create the 3D frame for the object.
			if(FAILED(hr = m_pDirect3DRM->CreateFrame(NULL, &m_pd3drmf)))
				goto EXIT_FAIL;
			m_pd3drmf->SetAppData((DWORD)this);
		}
		if (!pd3drmfChild)
		{
			// Create the 3D frame for the child object.
			if(FAILED(hr = m_pDirect3DRM->CreateFrame(NULL, &pd3drmfChild)))
				goto EXIT_FAIL;
			if(FAILED(hr = pvwfChild->put_Frame3D(pd3drmfChild)))
				goto EXIT_FAIL;
		}
		else
		{
			if (FAILED(hr = pd3drmfChild->GetParent(&pd3drmfParent)))
				goto EXIT_FAIL;
			if (pd3drmfParent)
			{
				// Need to explicitly Release child VWFrame since it is indirectly referenced 
				// by parent VWFrame.
				pvwfChild->Release();
			}
		}
		// And finally do the actual AddChild.
		if(FAILED(hr = m_pd3drmf->AddChild(pd3drmfChild)))
			goto EXIT_FAIL;

		if (m_pd2drmf)
		{
			if(FAILED(hr = m_pd2drmf->get_Parent(&pd2drmfParent)))
				goto EXIT_FAIL;
			if (pd2drmfParent)
			{
				// Error! - 2D/3D link must be at root scene.
				hr = E_FAIL;
				goto EXIT_FAIL;
			}
		}
	}
	else if (pd2drmfChild || m_pd2drmf)
	{
		if (!m_pd2drmf)
		{
			// Create the 2D frame for the object.
			if(FAILED(hr = m_pDirect2DRM->CreateFrame(NULL, &m_pd2drmf)))
				goto EXIT_FAIL;
		}
		else if (!pd2drmfChild)
		{
			// Create the 2D frame for the child object.
			if(FAILED(hr = m_pDirect2DRM->CreateFrame(NULL, &pd2drmfChild)))
				goto EXIT_FAIL;
			if(FAILED(hr = pvwfChild->put_Frame2D(pd2drmfChild)))
				goto EXIT_FAIL;
		}
		// And finally do the actual AddChild.
		if(FAILED(hr = m_pd2drmf->AddChild(pd2drmfChild)))
			goto EXIT_FAIL;
	}

	// Need to explicitly AddRef child VWFrame since it is indirectly referenced by 
	// this VWFrame.
	pvwfChild->AddRef();

EXIT_FAIL:

	SAFERELEASE(pd3drmfChild);
	SAFERELEASE(pd3drmfParent);
	SAFERELEASE(pd2drmfChild);
	SAFERELEASE(pd2drmfParent);

	return hr;
}

STDMETHODIMP CVWFrameObject::DeleteChild(IVWFrame *pvwfChild)
{
	IDirect3DRMFrame	*pd3drmfChild = NULL;
	IDirect2DRMFrame	*pd2drmfChild = NULL;
	HRESULT				hr = S_OK;

	CHECK_IN_POINTER(pvwfChild);

	CHECKANDCREATEFRAME;

	if(FAILED(hr = pvwfChild->get_Frame3D(&pd3drmfChild)))
		goto EXIT_FAIL;
	if(FAILED(hr = pvwfChild->get_Frame2D(&pd2drmfChild)))
		goto EXIT_FAIL;

	// Cannot let children be removed while running intersetion code.
	EnterCriticalSection(&m_CSChildren);

	if (pd3drmfChild)
	{
		if (!m_pd3drmf)
		{
			// Error! - Child must not have been created via VWFrame::AddChild.
			hr = E_FAIL;
			goto EXIT_FAIL;
		}
		if (FAILED(hr = m_pd3drmf->DeleteChild(pd3drmfChild)))
			goto EXIT_FAIL;
	}
	else if (pd2drmfChild)
	{
		if (!m_pd2drmf)
		{
			// Error! - Child must not have been created via VWFrame::AddChild.
			hr = E_FAIL;
			goto EXIT_FAIL;
		}
		if (FAILED(hr = m_pd2drmf->DeleteChild(pd2drmfChild)))
			goto EXIT_FAIL;
	}
//	else
//	{
		// Error! - Child is neither 2D or 3D.
		// Actually, could happen if child only contains BoundaryList.
//	}

	// Need to explicitly Release child VWFrame since it is indirectly referenced by 
	// this VWFrame.
	pvwfChild->Release();

EXIT_FAIL:

	LeaveCriticalSection(&m_CSChildren);

	SAFERELEASE(pd3drmfChild);
	SAFERELEASE(pd2drmfChild);

	return hr;
}


STDMETHODIMP CVWFrameObject::GetFrameByName(BSTR bstrName, IVWFrame **ppvwfDst)
{
	HRESULT					hr = E_FAIL;
	LPDIRECT3DRMFRAME		pd3drmFrame = NULL;

	CHECK_OUT_POINTER(ppvwfDst);
	
	CHECKANDCREATEFRAME;

	if (m_pd3drmf)
	{
		hr = FindFrame(bstrName, m_pd3drmf, &pd3drmFrame);
		if (pd3drmFrame)
		{
			// we found a frame with a matching name
			// now make sure that we create the vwframe correctly and do the proper
			// addrefs to it and potentially to its children
			*ppvwfDst = (IVWFrame *)(pd3drmFrame->GetAppData());
			if (*ppvwfDst) {
				(*ppvwfDst)->AddRef();
				goto EXIT_FAIL;
			}
			
			m_pWorld->CreateObjectPropertyExt(CLSID_VWFrame, NULL, (IObjectProperty**)(ppvwfDst));
			(*ppvwfDst)->put_Frame3D(pd3drmFrame);
		}
	}

EXIT_FAIL:

	SAFERELEASE(pd3drmFrame);

	return hr;
}

STDMETHODIMP CVWFrameObject::SetName(BSTR bstrName)
{
	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;

	hr = m_pd3drmf->SetName((const char *)(CString(bstrName)));

EXIT_FAIL:
	return hr;
}
		
STDMETHODIMP CVWFrameObject::GetName(BSTR* bstrName)
{
	HRESULT hr = S_OK;
	char szTmp[256];
	DWORD dwSize = 256;

	CHECK_OUT_POINTER(bstrName);

	CHECKANDCREATEFRAME;

	hr = m_pd3drmf->GetName(&dwSize, szTmp);

	*bstrName = CComBSTR(szTmp).Copy();

EXIT_FAIL:
	return hr;
}
		

STDMETHODIMP CVWFrameObject::SetPositionEx(IVWFrame *pvwfRef, float fPosX, float fPosY, float fPosZ)
{
	IDirect3DRMFrame	*pd3drmfRef = NULL;
	IDirect2DRMFrame	*pd2drmfRef = NULL;
	HRESULT hr = S_OK;

	// pvwfRef can be NULL
//	CHECK_IN_POINTER(pvwfRef);

	CHECKANDCREATEFRAME;

	if (pvwfRef)
	{
		if(FAILED(hr = pvwfRef->get_Frame3D(&pd3drmfRef)))
			goto EXIT_FAIL;
		if(FAILED(hr = pvwfRef->get_Frame2D(&pd2drmfRef)))
			goto EXIT_FAIL;
	}

	if (m_pd3drmf)
	{
		// Simple 3D pos relative to 3D frame.
		if(FAILED(hr = m_pd3drmf->SetPosition(pd3drmfRef, fPosX, fPosY, fPosZ)))
			goto EXIT_FAIL;
	}
	else if (m_pd2drmf)
	{
		// Simple 2D pos relative to 2D frame.
		//if(FAILED(hr = m_pd2drmf->SetPosition(pd2drmfRef, fPosX, fPosY)))
		//goto EXIT_FAIL;
	}
 
EXIT_FAIL:

	SAFERELEASE(pd3drmfRef);
	SAFERELEASE(pd2drmfRef);

	return hr;
}

STDMETHODIMP CVWFrameObject::GetPosition(IVWFrame *pvwfRef, VARIANT *pfX, VARIANT *pfY, VARIANT *pfZ)
{
	HRESULT hr = S_OK;

	// pvwfRef can be NULL
//	CHECK_IN_POINTER(pvwfRef);

	CHECK_OUT_POINTER(pfX);
	CHECK_OUT_POINTER(pfY);
	CHECK_OUT_POINTER(pfZ);
	
	CHECKANDCREATEFRAME;

	VariantClear(pfX);
	VariantClear(pfY);
	VariantClear(pfZ);

	pfX->vt = pfY->vt = pfZ->vt = VT_R4;
	hr = GetPositionEx(pvwfRef, &pfX->fltVal, &pfY->fltVal, &pfZ->fltVal);

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::GetPositionEx(IVWFrame *pvwfRef, float *pfX, float *pfY, float *pfZ)
{
	IDirect3DRMFrame	*pd3drmfRef = NULL;
	IDirect2DRMFrame	*pd2drmfRef = NULL;
	D3DVECTOR			d3dvPos;
	HRESULT hr = S_OK;

	// pvwfRef can be NULL
//	CHECK_IN_POINTER(pvwfRef);

	CHECK_OUT_POINTER(pfX);
	CHECK_OUT_POINTER(pfY);
	CHECK_OUT_POINTER(pfZ);

	CHECKANDCREATEFRAME;

	if (pvwfRef)
	{
		if(FAILED(hr = pvwfRef->get_Frame3D(&pd3drmfRef)))
			goto EXIT_FAIL;
		if(FAILED(hr = pvwfRef->get_Frame2D(&pd2drmfRef)))
			goto EXIT_FAIL;
	}

	if (m_pd3drmf)
	{
		// Simple 3D pos relative to 3D frame.

		// Get our position.
		if(FAILED(hr = m_pd3drmf->GetPosition(pd3drmfRef, &d3dvPos)))
			goto EXIT_FAIL;

		// And return it.
		*pfX = d3dvPos.x;
		*pfY = d3dvPos.y;
		*pfZ = d3dvPos.z;
	}
	else if (m_pd2drmf)
	{
		// Simple 2D pos relative to 2D frame.
		// Get our position.
		//		if(FAILED(hr = m_pd2drmf->GetPosition(pd2drmfRef, &d2dvPos)))
		//			goto EXIT_FAIL;
		// And return it.
		//		*pfX = d2dvPos.x;
		//		*pfY = d2dvPos.y;
	}

EXIT_FAIL:

	SAFERELEASE(pd3drmfRef);
	SAFERELEASE(pd2drmfRef);

	return hr;
}

STDMETHODIMP CVWFrameObject::GetOrientation(IVWFrame *pvwfRef, VARIANT *pfDirX, VARIANT *pfDirY, VARIANT *pfDirZ, VARIANT *pfUpX, VARIANT *pfUpY, VARIANT *pfUpZ)
{
	HRESULT hr = S_OK;

	// pvwfRef can be NULL
//	CHECK_IN_POINTER(pvwfRef);

	CHECK_OUT_POINTER(pfDirX);
	CHECK_OUT_POINTER(pfDirY);
	CHECK_OUT_POINTER(pfDirZ);

	CHECK_OUT_POINTER(pfUpX);
	CHECK_OUT_POINTER(pfUpY);
	CHECK_OUT_POINTER(pfUpZ);
	
	CHECKANDCREATEFRAME;

	VariantClear(pfDirX);
	VariantClear(pfDirY);
	VariantClear(pfDirZ);
	VariantClear(pfUpX);
	VariantClear(pfUpY);
	VariantClear(pfUpZ);

	pfDirX->vt = pfDirY->vt = pfDirZ->vt = pfUpX->vt = pfUpY->vt = pfUpZ->vt = VT_R4;
	hr = GetOrientationEx(pvwfRef, &pfDirX->fltVal, &pfDirY->fltVal, &pfDirZ->fltVal, &pfUpX->fltVal, &pfUpY->fltVal, &pfUpZ->fltVal);

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::GetOrientationEx(IVWFrame *pvwfRef, float *pfDirX, float *pfDirY, float *pfDirZ, float *pfUpX, float *pfUpY, float *pfUpZ)
{
	IDirect3DRMFrame	*pd3drmfRef = NULL;
	IDirect2DRMFrame	*pd2drmfRef = NULL;
	D3DVECTOR			d3dvDir, d3dvUp;
	HRESULT hr = S_OK;

	// pvwfRef can be NULL
//	CHECK_IN_POINTER(pvwfRef);

	CHECK_OUT_POINTER(pfDirX);
	CHECK_OUT_POINTER(pfDirY);
	CHECK_OUT_POINTER(pfDirZ);
	CHECK_OUT_POINTER(pfUpX);
	CHECK_OUT_POINTER(pfUpY);
	CHECK_OUT_POINTER(pfUpZ);

	CHECKANDCREATEFRAME;

	if (pvwfRef)
	{
		if(FAILED(hr = pvwfRef->get_Frame3D(&pd3drmfRef)))
			goto EXIT_FAIL;
		if(FAILED(hr = pvwfRef->get_Frame2D(&pd2drmfRef)))
			goto EXIT_FAIL;
	}

	if (m_pd3drmf)
	{
		// Simple 3D pos relative to 3D frame.

		// Get our orientation.
		if(FAILED(hr = m_pd3drmf->GetOrientation(pd3drmfRef, &d3dvDir, &d3dvUp)))
			goto EXIT_FAIL;

		// And return it.
		*pfDirX = d3dvDir.x;
		*pfDirY = d3dvDir.y;
		*pfDirZ = d3dvDir.z;
		*pfUpX = d3dvUp.x;
		*pfUpY = d3dvUp.y;
		*pfUpZ = d3dvUp.z;
	}
	else if (m_pd2drmf)
	{
		// Simple 2D pos relative to 2D frame.
		// REVIEW: Figure out 2D orientation.
	}

EXIT_FAIL:

	SAFERELEASE(pd3drmfRef);
	SAFERELEASE(pd2drmfRef);

	return hr;
}

STDMETHODIMP CVWFrameObject::SetOrientationEx(IVWFrame *pvwfRef, float fDirX, float fDirY, float fDirZ, float fUpX, float fUpY, float fUpZ)
{
	IDirect3DRMFrame	*pd3drmfRef = NULL;
	IDirect2DRMFrame	*pd2drmfRef = NULL;
	HRESULT hr = S_OK;

	// pvwfRef can be NULL
//	CHECK_IN_POINTER(pvwfRef);

	CHECKANDCREATEFRAME;

	if (pvwfRef)
	{
		if(FAILED(hr = pvwfRef->get_Frame3D(&pd3drmfRef)))
			goto EXIT_FAIL;
		if(FAILED(hr = pvwfRef->get_Frame2D(&pd2drmfRef)))
			goto EXIT_FAIL;
	}

	if (m_pd3drmf)
	{
		// Add scale 'cause D3D SetOrientation blasts Scale away in matrix.
		if(FAILED(hr = m_pd3drmf->SetOrientation(pd3drmfRef, fDirX, fDirY, fDirZ, fUpX, fUpY, fUpZ)))
			goto EXIT_FAIL;
		if(FAILED(hr = m_pd3drmf->AddScale(D3DRMCOMBINE_BEFORE, m_fScaleX, m_fScaleY, m_fScaleZ)))
			goto EXIT_FAIL;
	}
	else if (m_pd2drmf)
	{
		// Simple 2D pos relative to 2D frame.
		//		if(FAILED(hr = m_pd2drmf->SetOrientation(pd3drmfRef, fDirX, fDirY, fUpX, fUpY)))
		//			goto EXIT_FAIL;
	}

EXIT_FAIL:

	SAFERELEASE(pd3drmfRef);
	SAFERELEASE(pd2drmfRef);

	return hr;
}

STDMETHODIMP CVWFrameObject::GetScale(IVWFrame *pvwfRef, VARIANT *pfScaleX, VARIANT *pfScaleY, VARIANT *pfScaleZ)
{
	HRESULT hr = S_OK;

	// pvwfRef can be NULL
//	CHECK_IN_POINTER(pvwfRef);

	CHECK_OUT_POINTER(pfScaleX);
	CHECK_OUT_POINTER(pfScaleY);
	CHECK_OUT_POINTER(pfScaleZ);
	
	CHECKANDCREATEFRAME;

	VariantClear(pfScaleX);
	VariantClear(pfScaleY);
	VariantClear(pfScaleZ);

	pfScaleX->vt = pfScaleY->vt = pfScaleZ->vt = VT_R4;
	hr = GetScaleEx(pvwfRef, &pfScaleX->fltVal, &pfScaleY->fltVal, &pfScaleZ->fltVal);

EXIT_FAIL:
	return hr;

}

STDMETHODIMP CVWFrameObject::GetScaleEx(IVWFrame *pvwfRef, float *pfScaleX, float *pfScaleY, float *pfScaleZ)
{
	HRESULT hr = S_OK;

	// pvwfRef can be NULL
//	CHECK_IN_POINTER(pvwfRef);

	CHECK_OUT_POINTER(pfScaleX);
	CHECK_OUT_POINTER(pfScaleY);
	CHECK_OUT_POINTER(pfScaleZ);
	
	CHECKANDCREATEFRAME;

	*pfScaleX = m_fScaleX;
	*pfScaleY = m_fScaleY;
	*pfScaleZ = m_fScaleZ;

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::SetScaleEx(IVWFrame *pvwfRef, float fScaleX, float fScaleY, float fScaleZ)
{
	IDirect3DRMFrame	*pd3drmfRef = NULL;
	IDirect2DRMFrame	*pd2drmfRef = NULL;
	D3DVECTOR			d3dvDir, d3dvUp;
	HRESULT hr = S_OK;

	// pvwfRef can be NULL
//	CHECK_IN_POINTER(pvwfRef);

	CHECKANDCREATEFRAME;

	if (fScaleX == m_fScaleX && fScaleY == m_fScaleY && fScaleZ == m_fScaleZ)
		goto EXIT_FAIL;

	if (pvwfRef)
	{
		if(FAILED(hr = pvwfRef->get_Frame3D(&pd3drmfRef)))
			goto EXIT_FAIL;
		if(FAILED(hr = pvwfRef->get_Frame2D(&pd2drmfRef)))
			goto EXIT_FAIL;
	}

	if (m_pd3drmf)
	{
		// Need this in case someone set scale to 0. This will wipe out the dir.
		if (m_fScaleX == 0.0F || m_fScaleY == 0.0F || m_fScaleZ == 0.0F)
		{
			d3dvDir.x = d3dvDir.y = d3dvUp.x = d3dvUp.z = 0.0F;
			d3dvDir.z = d3dvUp.y = 1.0F;
		}
		else
		{
			if(FAILED(hr = m_pd3drmf->GetOrientation(pd3drmfRef, &d3dvDir, &d3dvUp)))
				goto EXIT_FAIL;
		}
		// D3D SetOrientation removes Scale from matrix.
		if(FAILED(hr = m_pd3drmf->SetOrientation(pd3drmfRef, d3dvDir.x, d3dvDir.y, d3dvDir.z, d3dvUp.x, d3dvUp.y, d3dvUp.z)))
			goto EXIT_FAIL;
		if (FAILED(hr = m_pd3drmf->AddScale(D3DRMCOMBINE_BEFORE, fScaleX, fScaleY, fScaleZ)))
			goto EXIT_FAIL;
	}
	else if (m_pd2drmf)
	{
		// Simple 2D pos relative to 2D frame.
		//		if(FAILED(hr = m_pd2drmf->AddScale(pd3drmfRef, rctCombine, fScaleX, fScaleY)))
		//			goto EXIT_FAIL;
	}

	m_fScaleX = fScaleX;
	m_fScaleY = fScaleY;
	m_fScaleZ = fScaleZ;

EXIT_FAIL:

	SAFERELEASE(pd3drmfRef);
	SAFERELEASE(pd2drmfRef);

	return hr;
}

STDMETHODIMP CVWFrameObject::LookAt(float fX, float fY, float fZ)
{
	IDirect3DRMFrame	*pd3drmfRef = NULL;
	D3DVECTOR vec, vecLeft, vecUp;
	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;

	if (m_pd3drmf)
	{
		if(FAILED(hr = m_pd3drmf->GetScene(&pd3drmfRef)))
			goto EXIT_FAIL;

		if(FAILED(hr = m_pd3drmf->GetPosition(pd3drmfRef, &vec)))
			goto EXIT_FAIL;

		vec.x = fX - vec.x;
		vec.y = fY - vec.y;
		vec.z = fZ - vec.z;

		D3DRMVectorNormalize(&vec);

		if (vec.x != 0.0F || vec.z != 0.0F)
		{
			vecLeft.x = vec.z;
			vecLeft.y = vec.y;
			vecLeft.z = vec.x;
		}
		else
		{
			vecLeft.x = vec.y;
			vecLeft.y = vec.x;
			vecLeft.z = vec.z;
		}
		D3DRMVectorCrossProduct(&vecUp, &vec, &vecLeft);

		// Simple 3D pos relative to 3D frame.
		if (FAILED(hr = m_pd3drmf->SetOrientation(pd3drmfRef, vec.x, vec.y, vec.z, vecUp.x, vecUp.y, vecUp.z)))
			goto EXIT_FAIL;
	}

EXIT_FAIL:
	SAFERELEASE(pd3drmfRef);
	return hr;

}

STDMETHODIMP CVWFrameObject::SetAppString(BSTR bstr)
{
	m_strApp = CString(bstr);
	return S_OK;
}

STDMETHODIMP CVWFrameObject::GetAppString(BSTR *pbstr)
{
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(pbstr);

	*pbstr = m_strApp.AllocSysString();

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::IsChildOf(IVWFrame *pvwf, VARIANT_BOOL *pbIsChild)
{
	IDirect3DRMFrame *pd3drmfParent = NULL;
	IDirect2DRMFrame *pd2drmfParent = NULL;
	IDirect3DRMFrame *pd3drmf = NULL;
	IDirect2DRMFrame *pd2drmf = NULL;
	HRESULT hr = S_OK;

	CHECK_IN_POINTER(pvwf);
	CHECK_OUT_POINTER(pbIsChild);

	CHECKANDCREATEFRAME;

	*pbIsChild = VARIANT_FALSE;

	if(FAILED(hr = pvwf->get_Frame3D(&pd3drmf)))
		goto EXIT_FAIL;
	if(FAILED(hr = pvwf->get_Frame2D(&pd2drmf)))
		goto EXIT_FAIL;

	if (m_pd3drmf)
	{
		if(FAILED(hr = m_pd3drmf->GetParent(&pd3drmfParent)))
			goto EXIT_FAIL;
		if (pd3drmfParent != NULL && pd3drmf != NULL && pd3drmfParent == pd3drmf)
		{
			*pbIsChild = VARIANT_TRUE;
		}

	}
	else if (m_pd2drmf)
	{
		if(FAILED(hr = m_pd2drmf->get_Parent(&pd2drmfParent)))
			goto EXIT_FAIL;
		if (pd2drmfParent != NULL && pd2drmf != NULL && pd2drmfParent == pd2drmf)
		{
			*pbIsChild = VARIANT_TRUE;
		}
	}

EXIT_FAIL:

	SAFERELEASE(pd3drmf);
	SAFERELEASE(pd2drmf);
	SAFERELEASE(pd3drmfParent);
	SAFERELEASE(pd2drmfParent);

	return hr;
}

STDMETHODIMP CVWFrameObject::StartAnimation(float fDuration, float fRangeStart, float fRangeEnd, BSTR bstrAnimationDoneMethod)
{
	HRESULT hr = S_OK;
	char szAnimName[256];
	DWORD dwSize = 256;
	long nRealDuration, nOptions;
	static CComBSTR	bstrAddAnimation("AddAnimation");
	CComPtr <IThing> pThing;
	CComPtr <IVWFrame> pParentFrame;
	

	if (m_pAnimationSet != NULL)
	{
		CComPtr <IPropertyList> pPropList;

		::CreatePropertyList(m_pWorld, &pPropList);

		m_pAnimationSet->GetName(&dwSize, szAnimName);

		nOptions = D3DRMANIMATION_LINEARPOSITION | D3DRMANIMATION_SCALEANDROTATION;
		
		if (fDuration < 0.001f && fDuration > -0.001f)
		{
			nRealDuration = -1;
			nOptions |= D3DRMANIMATION_CLOSED;
		}
		else if (fDuration < 0.0f)  //If it's negative, it's a loop
		{
			nRealDuration = (long) -(fDuration * 1000.0f );
			nOptions |= D3DRMANIMATION_CLOSED;
		}
		else
		{
			nRealDuration = (long) (fDuration * 1000.0f );
			nOptions |= D3DRMANIMATION_OPEN;
		}


		GetParent(&pParentFrame.p);
		pParentFrame.p->get_Thing(&pThing.p);

		pPropList->AddLong( (long)(void*) m_pAnimationSet);
		pPropList->AddLong( GetTickCount() );
		pPropList->AddLong(nOptions);
		pPropList->AddLong( nRealDuration ); 
		pPropList->AddString( CComBSTR(szAnimName) ); //bstrAnimationName ); 

		pPropList->AddThing( pThing ); 
		pPropList->AddString( CComBSTR("") ); 
		pPropList->AddString( bstrAnimationDoneMethod ); //CComBSTR("OnXAnimationDone") ); 

		pPropList->Add( CComVariant(fRangeStart) ); 
		//If the end range isn't specified, set it to the maximum
		if (fRangeEnd < 0.001f && fRangeEnd > -0.001f)
			pPropList->Add( CComVariant(FLT_MAX) ); 
		else
			pPropList->Add( CComVariant(fRangeEnd) ); 

		//Send it all off to the render thread.
		m_pWorld->FireUIEvent(NULL, bstrAddAnimation, CComVariant((IUnknown*)pPropList));
	}

	return hr;
}

STDMETHODIMP CVWFrameObject::AnimationSetTime(float fTime)
{
	HRESULT hr = S_OK;

	if (m_pAnimationSet != NULL)
	{
		hr = m_pAnimationSet->SetTime(fTime);
	}

	return hr;
}

STDMETHODIMP CVWFrameObject::StopAnimation()
{
	HRESULT hr = S_OK;
	char szAnimName[256];
	DWORD dwSize = 256;
	static CComBSTR	bstrDeleteAnimation("DeleteAnimation");

	if (m_pAnimationSet != NULL)
	{
		m_pAnimationSet->GetName(&dwSize, szAnimName);
		m_pWorld->FireUIEvent(NULL, bstrDeleteAnimation, CComVariant( CComBSTR(szAnimName) ) );
	}

	return hr;
}

/*STDMETHODIMP CVWFrameObject::get_Animation(LPDIRECT3DRMANIMATION2 *ppd3drmanim)
{
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(ppd3drmanim);

	*ppd3drmanim = m_pAnimation;
	SAFEADDREF(m_pAnimation);

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::put_Animation(LPDIRECT3DRMANIMATION2 ppd3drmanim)
{
	HRESULT hr = S_OK;

	CHECK_IN_POINTER(ppd3drmanim);

	SAFERELEASE(m_pAnimation);
	m_pAnimation = ppd3drmanim;
	SAFEADDREF(m_pAnimation);

EXIT_FAIL:
	return hr;
}
*/

STDMETHODIMP CVWFrameObject::get_AnimationSet(LPDIRECT3DRMANIMATIONSET2 *ppd3drmanimset)
{
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(ppd3drmanimset);

	*ppd3drmanimset = m_pAnimationSet;
	SAFEADDREF(m_pAnimationSet);

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::put_AnimationSet(LPDIRECT3DRMANIMATIONSET2 ppd3drmanimset)
{
	HRESULT hr = S_OK;

	CHECK_IN_POINTER(ppd3drmanimset);

	SAFERELEASE(m_pAnimationSet);
	m_pAnimationSet = ppd3drmanimset;
	SAFEADDREF(m_pAnimationSet);

EXIT_FAIL:
	return hr;
}


STDMETHODIMP CVWFrameObject::Copy(IVWFrame **ppvwfDst)
{
	LPDIRECT3DRMFRAME lpDstFrame = NULL;
	LPDIRECT3DRMANIMATIONSET pd3drmanimset = NULL;
	LPDIRECT3DRMANIMATIONSET2 pd3drmanimset2 = NULL;
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(ppvwfDst);

	CHECKANDCREATEFRAME;

	// Create destination VWFrame.
	m_pWorld->CreateObjectPropertyExt(CLSID_VWFrame, NULL, (IObjectProperty**)ppvwfDst);

	//Is there an animation set to copy?
	if (m_pAnimationSet)
	{
		char szTmp[256];
		DWORD dwSize;

		m_pDirect3DRM->CreateAnimationSet(&pd3drmanimset);
		pd3drmanimset->QueryInterface( IID_IDirect3DRMAnimationSet2, (LPVOID*) &pd3drmanimset2);

		(*ppvwfDst)->put_AnimationSet(pd3drmanimset2);

		//Clear the string
		szTmp[0] = 0;
		dwSize = 256;
		m_pAnimationSet->GetName(&dwSize, szTmp);
		pd3drmanimset2->SetName(szTmp);
		//pd3drmanimset2->SetTime(0);
	}

	// Copy the 3D frame hierarchy from this VWFrame.
	D3DFrameCopy(m_pDirect3DRM, m_pd3drmf, &lpDstFrame, m_pAnimationSet, pd3drmanimset2, *ppvwfDst);

	// Set up destination VWFrame.
	(*ppvwfDst)->put_Frame3D(lpDstFrame);

	//If there is an animation set, set the time to zero
	if (pd3drmanimset2)
		pd3drmanimset2->SetTime(0);

EXIT_FAIL:
	SAFERELEASE(pd3drmanimset2);
	SAFERELEASE(pd3drmanimset);
	SAFERELEASE(lpDstFrame);

	return hr;
}

STDMETHODIMP CVWFrameObject::MoveToPosition(IVWFrame *pvwfRef, float fPosX, float fPosY, float fPosZ, VARIANT_BOOL bFireEvents, IPropertyList **ppIDataList)
{
	IDirect3DRMFrame	*pd3drmfRef = NULL;
	IDirect2DRMFrame	*pd2drmfRef = NULL;
	IVWAnimation		*pVWAnimation = NULL;
	IThing				*pIntersecter = NULL;
	DISPPARAMS			dispparams;
	D3DVECTOR			d3dvPos;
	IVWFrame			*pVWRefFrame = NULL;
	HRESULT hr = S_OK;
	static CComBSTR bstrFireIntersectEvent("FireIntersectEvent");

	// pvwfRef can be NULL
//	CHECK_IN_POINTER(pvwfRef);
	CHECK_OUT_POINTER(ppIDataList);

	CHECKANDCREATEFRAME;

	// Initialize dispparams so TerminateDispparams won't fail if not used.
	dispparams.rgvarg = NULL;

	// Make sure caller doesn't think we've returned something unless we have.
	*ppIDataList = NULL;

	if (!pvwfRef)
	{
		hr = GetScene(&pVWRefFrame);
		if (FAILED(hr) || !pVWRefFrame) goto EXIT_FAIL;
	}
	else
	{
		pVWRefFrame = pvwfRef;
		pVWRefFrame->AddRef();
	}

	if(FAILED(hr = pVWRefFrame->get_Frame3D(&pd3drmfRef)))
		goto EXIT_FAIL;
	if(FAILED(hr = pVWRefFrame->get_Frame2D(&pd2drmfRef)))
		goto EXIT_FAIL;

	if (m_pd3drmf)
	{
		// Simple 3D pos relative to 3D frame.
		// Does this position change intersect with the reference BoundaryList?

		// Get our last position.
		if(FAILED(hr = m_pd3drmf->GetPosition(pd3drmfRef, &d3dvPos)))
			goto EXIT_FAIL;

		// Test for intersection between this frame's movement vector and the cells
		// of the reference frame and all the reference frame's children.
		if (((CVWFrameObject*)pVWRefFrame)->Intersect(pVWRefFrame, this, d3dvPos.x, d3dvPos.y, d3dvPos.z, fPosX, fPosY, fPosZ, ppIDataList))
		{
			// We hit something!
			// Fire the intersect event (if necessary).
			if (!bFireEvents)
				goto EXIT_FAIL;

			// We are the intersecter, the intersecter will deal with invoking all the 
			// necessary intersect event handlers.
			if (FAILED(get_Thing(&pIntersecter)))
				goto EXIT_FAIL;
			
			// Package up args into DISPPARAMS struct.
			if (FAILED(hr = InitializeDISPPARAMS(&dispparams, 8)))
				goto EXIT_FAIL;
			dispparams.rgvarg[7].vt = VT_DISPATCH;
			dispparams.rgvarg[7].pdispVal =	pvwfRef;
			dispparams.rgvarg[6].vt = VT_R4;
			dispparams.rgvarg[6].fltVal = d3dvPos.x;
			dispparams.rgvarg[5].vt = VT_R4;
			dispparams.rgvarg[5].fltVal = d3dvPos.y;
			dispparams.rgvarg[4].vt = VT_R4;
			dispparams.rgvarg[4].fltVal = d3dvPos.z;
			dispparams.rgvarg[3].vt = VT_R4;
			dispparams.rgvarg[3].fltVal = fPosX;
			dispparams.rgvarg[2].vt = VT_R4;
			dispparams.rgvarg[2].fltVal = fPosY;
			dispparams.rgvarg[1].vt = VT_R4;
			dispparams.rgvarg[1].fltVal = fPosZ;
			dispparams.rgvarg[0].vt = VT_DISPATCH;
			dispparams.rgvarg[0].pdispVal =	*ppIDataList;
			// Invoke the method on the intersecter which will deal with invoking all the 
			// necessary intersect event handlers. Invoke the method on the main thread
			// (via the animator) so we don't hold up this thread!
			if (FAILED(hr = m_pVWAnimator->CreateExt(0, pIntersecter, bstrFireIntersectEvent, &dispparams, &pVWAnimation)))
				goto EXIT_FAIL;

			// And return.
			goto EXIT_FAIL;
		}
		else
		{
			// Translate the 3D frame.
			if(FAILED(hr = m_pd3drmf->SetPosition(pd3drmfRef, fPosX, fPosY, fPosZ)))
				goto EXIT_FAIL;
		}
	}
	else if (m_pd2drmf)
	{
		// Simple 2D pos relative to 2D frame.
		//		if(FAILED(hr = m_pd2drmf->SetPosition(pd2drmfRef, fPosX, fPosY)))
		//			goto EXIT_FAIL;
	}

EXIT_FAIL:

	SAFERELEASE(pVWRefFrame);
	SAFERELEASE(pd3drmfRef);
	SAFERELEASE(pd2drmfRef);
	TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	SAFERELEASE(pVWAnimation);
	SAFERELEASE(pIntersecter);

	return hr;
}

STDMETHODIMP CVWFrameObject::FindIntersections(float fStartPosX, float fStartPosY, float fStartPosZ, float fEndPosX, float fEndPosY, float fEndPosZ, IPropertyList **ppIDataList)
{
	Intersect(this, NULL, fStartPosX, fStartPosY, fStartPosZ, fEndPosX, fEndPosY, fEndPosZ, ppIDataList);

	return S_OK;
}

BOOL CVWFrameObject::Intersect(IVWFrame *pvwfRef, IVWFrame *pvwfSkip, float x1, float y1, float z1, float x2, float y2, float z2, IPropertyList **ppIDataList)
{
	IDirect3DRMFrameArray	*p3DChildren = NULL;
	IDirect3DRMFrame		*p3DChild = NULL;
	IVWFrame				*pvwfChild = NULL, *pVWFrame = NULL, *pvwfScene = NULL;
	IVWIntersectData		*pVWIntersectData = NULL, *pPrevVWIData = NULL;
	IBoundary				*pBoundary = NULL;
	IVector					*pvProjection = NULL;
	int						iBoundaryIndex;
	VARIANT_BOOL			bPassable, bRet = VARIANT_FALSE;
	BYTE					bPassed;
	D3DVECTOR				d3dvProj, rlv1, rlv2;
	long					lChildren = 0, lCount = 0, i;
	CComVariant				var;
	HRESULT					hr = S_OK;

	CHECK_OUT_POINTER(ppIDataList);

	CHECKANDCREATEFRAME;

	if (this == pvwfSkip)
		goto EXIT_FAIL;

	if (m_pBoundaryList)
	{
		if (this != pvwfRef)
		{
			// OK, this isn't the reference frame and therefore the frame (and hence the 
			// BoundaryList) may be transformed relative to the reference frame.
			// We need to apply this transform inversly to the movement vector to
			// allow us to use the BoundaryList's intersect routine without actually transforming
			// all vertices in the BoundaryList.
			rlv1.x = x1;
			rlv1.y = y1;
			rlv1.z = z1;
			rlv2.x = x2;
			rlv2.y = y2;
			rlv2.z = z2;
			// If the reference frame is not the scene, must first transform movement vector
			// to be relative to scene, since InverseTransform will work relative to scene.
			GetScene(&pvwfScene);
			if (pvwfRef != pvwfScene)
			{
				pvwfRef->Transform(AXIS_Y, &rlv1.x, &rlv1.y, &rlv1.z, rlv1.x, rlv1.y, rlv1.z);
				pvwfRef->Transform(AXIS_Y, &rlv2.x, &rlv2.y, &rlv2.z, rlv2.x, rlv2.y, rlv2.z);
			}
			InverseTransform(AXIS_Y, &rlv1.x, &rlv1.y, &rlv1.z, rlv1.x, rlv1.y, rlv1.z);
			InverseTransform(AXIS_Y, &rlv2.x, &rlv2.y, &rlv2.z, rlv2.x, rlv2.y, rlv2.z);

			// There are already elements in the intersect data list. We need to analyze the
			// last element to check if the next operation adds an element to the end of
			// the list.
			if (*ppIDataList)
			{
				(*ppIDataList)->get_Count(&lCount);
				if (lCount != 0)
					(*ppIDataList)->get_ObjectProperty(lCount-1, (IObjectProperty**)&pPrevVWIData);
			}
			
			if ( (hr = DoIntersections(m_pBoundaryList, rlv1.x, rlv1.y, rlv1.z, rlv2.x, rlv2.y, rlv2.z, ppIDataList))
					== S_INTERSECTION)
			{
				// OK, got some intersections.
				bRet = VARIANT_TRUE;

				(*ppIDataList)->get_Count(&lCount);
				if (lCount == 0)
				{
					// Should be at least one element in the list!
					hr = E_FAIL;
					goto EXIT_FAIL;
				}
				(*ppIDataList)->get_ObjectProperty(lCount-1, (IObjectProperty**)&pVWIntersectData);
				if (pVWIntersectData != pPrevVWIData)
				{
					pVWIntersectData->get_Passed(&bPassed);
					pVWIntersectData->get_BoundaryIndex(&iBoundaryIndex);
					m_pBoundaryList->get_ObjectProperty(iBoundaryIndex, (IObjectProperty**) &pBoundary);
					pBoundary->get_IsPassable(&bPassable);

					if (!bPassable && !bPassed)
					{
						// The last edge intersected is impassable. We will need to transform
						// it's projection vector.
						pVWIntersectData->get_Projection(&pvProjection);
						if (!pvProjection)
						{
							// Should have calculated projection for every such intersection.
							hr = E_FAIL;
							goto EXIT_FAIL;
						}
						pvProjection->get(&d3dvProj.x, &d3dvProj.y, &d3dvProj.z);
						Transform(AXIS_Y, &d3dvProj.x, &d3dvProj.y, &d3dvProj.z, d3dvProj.x, d3dvProj.y, d3dvProj.z);
						if (pvwfRef != pvwfScene)
							pvwfRef->InverseTransform(AXIS_Y, &d3dvProj.x, &d3dvProj.y, &d3dvProj.z, d3dvProj.x, d3dvProj.y, d3dvProj.z);
						pvProjection->set(d3dvProj.x, d3dvProj.y, d3dvProj.z);
					}
				}
			}
		}
		else
		{
			if ((hr = DoIntersections(m_pBoundaryList, x1, y1, z1, x2, y2, z2, ppIDataList)) == S_INTERSECTION)
			{
				// OK, got some intersections.
				bRet = VARIANT_TRUE;
			}
		}
	}

	// Only traverse to reference frame's children, no deeper.
	if (this != pvwfRef)
		goto EXIT_FAIL;

	// Work through the children (if any).
	if (m_pd3drmf)
	{
		// Cannot let children be removed while running this code.
		EnterCriticalSection(&m_CSChildren);

		m_pd3drmf->GetChildren(&p3DChildren);
		lChildren = p3DChildren->GetSize();
		for (i = 0; i < lChildren; i++)
		{
			if (SUCCEEDED(p3DChildren->GetElement(i, &p3DChild)) && p3DChild && p3DChild->GetAppData())
			{
				if (((CVWFrameObject*)(p3DChild->GetAppData()))->Intersect(pvwfRef, pvwfSkip, x1, y1, z1, x2, y2, z2, ppIDataList))
				{
					// OK, child got an intersect!
					bRet = VARIANT_TRUE;
				}
			}
			SAFERELEASE(p3DChild);
		}
		LeaveCriticalSection(&m_CSChildren);
	}

EXIT_FAIL:

	SAFERELEASE(pvwfChild);
	SAFERELEASE(pVWFrame);
	SAFERELEASE(p3DChildren);
	SAFERELEASE(p3DChild);
	SAFERELEASE(pvwfScene);
	SAFERELEASE(pVWIntersectData);
	SAFERELEASE(pPrevVWIData);
	SAFERELEASE(pBoundary);
	SAFERELEASE(pvProjection);

	return bRet;
}

STDMETHODIMP CVWFrameObject::GetBoundingBox( VARIANT* fminX,  VARIANT* fminY,  VARIANT* fminZ, VARIANT* fmaxX,  VARIANT* fmaxY,  VARIANT* fmaxZ )
{
	HRESULT hr = S_OK;
	D3DRMBOX d3dBox;
	IVWGeometrySprite* pGeomSprite = NULL;

	CHECK_OUT_POINTER(fminX);
	CHECK_OUT_POINTER(fminY);
	CHECK_OUT_POINTER(fminZ);
	CHECK_OUT_POINTER(fmaxX);
	CHECK_OUT_POINTER(fmaxY);
	CHECK_OUT_POINTER(fmaxZ);

	CHECKANDCREATEFRAME;

	VariantClear(fminX);
	VariantClear(fminY);
	VariantClear(fminZ);
	VariantClear(fmaxX);
	VariantClear(fmaxY);
	VariantClear(fmaxZ);

	fminX->vt = VT_R4;
	fminY->vt = VT_R4;
	fminZ->vt = VT_R4;
	fmaxX->vt = VT_R4;
	fmaxY->vt = VT_R4;
	fmaxZ->vt = VT_R4;
	
	// OK for QI to fail, it just means the geometry isn't a sprite, which is the case most of the time
	if (m_pVWGeometryObject && (SUCCEEDED(m_pVWGeometryObject->QueryInterface(IID_IVWGeometrySprite, (LPVOID*)&pGeomSprite))) && pGeomSprite)
	{
		fminX->fltVal = -1.0f;
		fminY->fltVal = -1.0f;
		fminZ->fltVal = -1.0f;
		fmaxX->fltVal = 1.0f;
		fmaxY->fltVal = 1.0f;
		fmaxZ->fltVal = 1.0f;
	}
	else
	{
		d3dBox = GetFrameBoundingBox(m_pd3drmf);
		fminX->fltVal = d3dBox.min.x;
		fminY->fltVal = d3dBox.min.y;
		fminZ->fltVal = d3dBox.min.z;
		fmaxX->fltVal = d3dBox.max.x;
		fmaxY->fltVal = d3dBox.max.y;
		fmaxZ->fltVal = d3dBox.max.z;
	}


EXIT_FAIL:
	SAFERELEASE(pGeomSprite);

	return hr;
}

STDMETHODIMP CVWFrameObject::GetFloorHeight(IVWFrame *pvwfRef, float *pfFloorHeight)
{
	float fX, fY, fZ;
	HRESULT hr = S_OK;

	// pvwfRef can be NULL
//	CHECK_IN_POINTER(pvwfRef);
	CHECK_OUT_POINTER(pfFloorHeight);

	CHECKANDCREATEFRAME;

	GetPosition(&fX, &fY, &fZ);

	if (pvwfRef)
		hr = ((CVWFrameObject*)pvwfRef)->GetFloorOrCeiling(pvwfRef, this, fX, fY, fZ, VARIANT_TRUE, pfFloorHeight);
	else
		hr = E_INVALIDARG;

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::GetCeilingHeight(IVWFrame *pvwfRef, float *pfCeilingHeight)
{
	float fX, fY, fZ;
	HRESULT hr = S_OK;

	// pvwfRef can be NULL
//	CHECK_IN_POINTER(pvwfRef);
	CHECK_OUT_POINTER(pfCeilingHeight);

	CHECKANDCREATEFRAME;

	GetPosition(&fX, &fY, &fZ);

	if (pvwfRef)
		hr = ((CVWFrameObject*)pvwfRef)->GetFloorOrCeiling(pvwfRef, this, fX, fY, fZ, VARIANT_FALSE, pfCeilingHeight);
	else
		hr = E_INVALIDARG;

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::GetFloorHeightAtPosition(float fX, float fY, float fZ, float *pfFloorHeight)
{
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(pfFloorHeight);

	CHECKANDCREATEFRAME;

	hr = GetFloorOrCeiling(this, NULL, fX, fY, fZ, VARIANT_TRUE, pfFloorHeight);

EXIT_FAIL:
	return hr;

}

STDMETHODIMP CVWFrameObject::GetCeilingHeightAtPosition(float fX, float fY, float fZ, float *pfCeilingHeight)
{
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(pfCeilingHeight);

	CHECKANDCREATEFRAME;

	hr = GetFloorOrCeiling(this, NULL, fX, fY, fZ, VARIANT_FALSE, pfCeilingHeight);

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::GetFloorOrCeiling(IVWFrame *pvwfRef, IVWFrame *pvwfSkip, float fX, float fY, float fZ, VARIANT_BOOL bFloor, float *pfHeight)
{
	IDirect3DRMFrameArray	*p3DChildren = NULL;
	IDirect3DRMFrame		*p3DChild = NULL;
	IVWFrame				*pvwfScene = NULL;
	float					fHeight, fTempY;
	D3DVECTOR				rlv1;
	long					lChildren = 0, lCount = 0, i;
	CComVariant				var;
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(pfHeight);

	CHECKANDCREATEFRAME;

	if (pvwfRef == this)
		*pfHeight = fY;
	
	if (this == pvwfSkip)
		goto EXIT_FAIL;

	if (m_pBoundaryList)
	{
		if (this != pvwfRef)
		{
			// OK, this isn't the reference frame and therefore the frame (and hence the 
			// BoundaryList) may be transformed relative to the reference frame.
			// We need to apply this transform inversly to the movement vector to
			// allow us to use the BoundaryList's intersect routine without actually transforming
			// all vertices in the BoundaryList.
			rlv1.x = fX;
			rlv1.y = fY;
			rlv1.z = fZ;
			// If the reference frame is not the scene, must first transform movement vector
			// to be relative to scene, since InverseTransform will work relative to scene.
			GetScene(&pvwfScene);
			if (pvwfRef != pvwfScene)
				pvwfRef->Transform(AXIS_Y, &rlv1.x, &rlv1.y, &rlv1.z, rlv1.x, rlv1.y, rlv1.z);
			InverseTransform(AXIS_Y, &rlv1.x, &rlv1.y, &rlv1.z, rlv1.x, rlv1.y, rlv1.z);

			if (bFloor)
			{
				fTempY = rlv1.y;
				DoGetFloorHeight(m_pBoundaryList, rlv1.x, rlv1.y, rlv1.z, &rlv1.y);
				if (rlv1.y != fTempY)
				{
					Transform(AXIS_Y, &rlv1.x, &rlv1.y, &rlv1.z, rlv1.x, rlv1.y, rlv1.z);
					if (rlv1.y > *pfHeight || *pfHeight == fY)
						*pfHeight = rlv1.y;
				}
			}
			else
			{
				fTempY = rlv1.y;
				DoGetCeilingHeight(m_pBoundaryList, rlv1.x, rlv1.y, rlv1.z, &rlv1.y);
				if (rlv1.y != fTempY)
				{
					Transform(AXIS_Y, &rlv1.x, &rlv1.y, &rlv1.z, rlv1.x, rlv1.y, rlv1.z);
					if (rlv1.y < *pfHeight || *pfHeight == fY)
						*pfHeight = rlv1.y;
				}
			}
		}
		else
		{
			if (bFloor)
			{
				DoGetFloorHeight(m_pBoundaryList, fX, fY, fZ, &fHeight);
				if (fHeight != fY && (fHeight > *pfHeight || *pfHeight == fY))
					*pfHeight = fHeight;
			}
			else
			{
				DoGetCeilingHeight(m_pBoundaryList, fX, fY, fZ, &fHeight);
				if (fHeight != fY && (fHeight < *pfHeight || *pfHeight == fY))
					*pfHeight = fHeight;
			}
		}
	}

	// Only traverse to reference frame's children, no deeper.
	if (this != pvwfRef)
		goto EXIT_FAIL;

	// Work through the children (if any).
	if (m_pd3drmf)
	{
		// Cannot let children be removed while running this code.
		EnterCriticalSection(&m_CSChildren);

		m_pd3drmf->GetChildren(&p3DChildren);
		lChildren = p3DChildren->GetSize();
		for (i = 0; i < lChildren; i++)
		{
			p3DChildren->GetElement(i, &p3DChild);
			if (p3DChild->GetAppData())
			{
				((CVWFrameObject*)(p3DChild->GetAppData()))->GetFloorOrCeiling(pvwfRef, pvwfSkip, fX, fY, fZ, bFloor, pfHeight);
			}
			SAFERELEASE(p3DChild);
		}
		LeaveCriticalSection(&m_CSChildren);
	}

EXIT_FAIL:

	SAFERELEASE(p3DChildren);
	SAFERELEASE(p3DChild);
	SAFERELEASE(pvwfScene);

	return S_OK;
}

STDMETHODIMP CVWFrameObject::get_Frame3D(LPDIRECT3DRMFRAME *ppd3drmf)
{
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(ppd3drmf);

	if (m_pd3drmf)
	{
		m_pd3drmf->AddRef();
	}
	*ppd3drmf = m_pd3drmf;

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::put_Frame3D(LPDIRECT3DRMFRAME pd3drmf)
{
	IVWFrame			*pvwfParent = NULL;
	IDirect3DRMFrame	*pd3drmfParent = NULL;
	HRESULT hr = S_OK;
	
	CHECK_IN_POINTER(pd3drmf);

	if (m_pd3drmf)
	{
		// Only allowed to do this once for initialization purposes. 
		// After that all access should be via VWFrame.
		// This avoids confusing the references between parent and child frames.
		ASSERT(FALSE);
		hr = E_FAIL;
		goto EXIT_FAIL;
	}

	// Actually store the frame.
	m_pd3drmf = pd3drmf;
	SAFEADDREF(m_pd3drmf);
	pd3drmf->SetAppData((DWORD)this);

	// May need to create a VWFrame for all parents up to the next VWFrame to ensure
	// ref counting is properly maintained.
	pd3drmf->GetParent(&pd3drmfParent);
	if (pd3drmfParent)
	{
		// Need to explicitly AddRef this VWFrame since it is indirectly referenced by 
		// the parent VWFrame.
		((IVWFrame*)this)->AddRef();
		if (!pd3drmfParent->GetAppData())
		{
			m_pWorld->CreateObjectPropertyExt(CLSID_VWFrame, NULL, (IObjectProperty**)(&pvwfParent));
			pvwfParent->put_Frame3D(pd3drmfParent);
		}
	}

EXIT_FAIL:

	SAFERELEASE(pvwfParent);
	SAFERELEASE(pd3drmfParent);

	return hr;
}

STDMETHODIMP CVWFrameObject::get_Frame2D(LPDIRECT2DRMFRAME *ppd2drmf)
{
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(ppd2drmf);

	if (m_pd2drmf)
	{
		m_pd2drmf->AddRef();
	}
	*ppd2drmf = m_pd2drmf;

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::put_Frame2D(LPDIRECT2DRMFRAME pd2drmf)
{
	SAFERELEASE(m_pd2drmf);
	m_pd2drmf = pd2drmf;
	SAFEADDREF(m_pd2drmf);
	return S_OK;
}

STDMETHODIMP CVWFrameObject::put_BoundaryList(IPropertyList *pBoundaryList)
{
	SAFERELEASE(m_pBoundaryList);
	m_pBoundaryList = pBoundaryList;
	SAFEADDREF(m_pBoundaryList);
	return S_OK;
}

STDMETHODIMP CVWFrameObject::get_BoundaryList(IPropertyList **ppBoundaryList)
{
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(ppBoundaryList);

	if (m_pBoundaryList)
	{
		m_pBoundaryList->AddRef();
	}
	*ppBoundaryList = m_pBoundaryList;

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::GetChildren(IPropertyList **ppChildren)
{
	IDirect3DRMFrameArray	*p3DChildren = NULL;
	IDirect3DRMFrame		*p3DChild = NULL;
	IVWFrame				*pvwfChild = NULL;
	long					lChildren, i;
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(ppChildren);

	CHECKANDCREATEFRAME;

	m_pd3drmf->GetChildren(&p3DChildren);
	lChildren = p3DChildren->GetSize();
	CreatePropertyList(m_pWorld, ppChildren);
	for (i = 0; i < lChildren; i++)
	{
		p3DChildren->GetElement(i, &p3DChild);
		if (p3DChild->GetAppData())
		{
			(*ppChildren)->AddObjectProperty((IObjectProperty*)(p3DChild->GetAppData()));
		}
		else
		{
			m_pWorld->CreateObjectPropertyExt(CLSID_VWFrame, NULL, (IObjectProperty**)&pvwfChild);
			pvwfChild->put_Frame3D(p3DChild);
			(*ppChildren)->AddObjectProperty((IObjectProperty*)pvwfChild);
			SAFERELEASE(pvwfChild);
		}
		SAFERELEASE(p3DChild);
	}

	SAFERELEASE(p3DChildren);
	SAFERELEASE(p3DChild);
	SAFERELEASE(pvwfChild);

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::GetVWFChildren(IPropertyList **ppChildren)
{
	IDirect3DRMFrameArray	*p3DChildren = NULL;
	IDirect3DRMFrame		*p3DChild = NULL;
	long					lChildren, i;
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(ppChildren);

	CHECKANDCREATEFRAME;

	if (m_pd3drmf)
	{
		m_pd3drmf->GetChildren(&p3DChildren);
		lChildren = p3DChildren->GetSize();
		CreatePropertyList(m_pWorld, ppChildren);
		for (i = 0; i < lChildren; i++)
		{
			p3DChildren->GetElement(i, &p3DChild);
			if (p3DChild->GetAppData())
			{
				(*ppChildren)->AddObjectProperty((IObjectProperty*)(p3DChild->GetAppData()));
			}
			SAFERELEASE(p3DChild);
		}
	}
	else if (m_pd2drmf)
	{
	}

	SAFERELEASE(p3DChildren);
	SAFERELEASE(p3DChild);

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::GetParent(IVWFrame **ppvwfParent)
{
	IDirect3DRMFrame *p3DParent = NULL;
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(ppvwfParent);

	CHECKANDCREATEFRAME;

	*ppvwfParent = NULL;
	
	if (m_pd3drmf)
	{
		m_pd3drmf->GetParent(&p3DParent);
		if (p3DParent)
		{
			*ppvwfParent = (IVWFrame*)(p3DParent->GetAppData());
			SAFEADDREF(*ppvwfParent);
		}
			
	}
	else if (m_pd2drmf)
	{
	}
	
	SAFERELEASE(p3DParent);
	
EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::GetScene(IVWFrame **ppvwfScene)
{
	IVWFrame *pvwfScene = NULL, *pvwfParent = NULL;
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(ppvwfScene);

	CHECKANDCREATEFRAME;

	GetParent(&pvwfParent);
	if (!pvwfParent)
	{
		*ppvwfScene = this;
		((IVWFrame*)this)->AddRef();
		return S_OK;
	}
	while (TRUE)
	{
		pvwfScene = pvwfParent;
		pvwfParent->GetParent(&pvwfParent);
		if (pvwfParent == NULL)
		{
			break;
		}
		SAFERELEASE(pvwfScene);
	}
	*ppvwfScene = pvwfScene;

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::get_Thing(IThing **ppThing)
{
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(ppThing);

	*ppThing = m_pThing;
	SAFEADDREF(m_pThing);

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::put_Thing(IThing *pThing)
{
	SAFERELEASE(m_pThing);
	m_pThing = pThing;
	SAFEADDREF(pThing);
	return S_OK;
}

ULONG CVWFrameObject::InternalRelease()
{
	IDirect3DRMFrameArray	*p3DChildren = NULL;
	IDirect3DRMFrame		*p3DChild = NULL;
	IVWFrame				*pvwfChild;
	long					lChildren, i;
	DWORD					dwRef;
	IVWGeometryCache		*pVWGeometryCache;

	CComObjectRootEx<CComMultiThreadModel>::InternalRelease();
	// Use stack vars since call to geometry object may delete this object.
	dwRef = m_dwRef;
	pVWGeometryCache = m_pVWGeometryCache;
	if (m_dwRef == 0)
	{
		// Need to explicitly delete all children VWFrames since they are indirectly
		// referenced by this VWFrame.
		if (m_pd3drmf)
		{
			m_pd3drmf->GetChildren(&p3DChildren);
			lChildren = p3DChildren->GetSize();
			for (i = 0; i < lChildren; i++)
			{
				p3DChildren->GetElement(i, &p3DChild);
				pvwfChild = (IVWFrame*)(p3DChild->GetAppData());
				if (pvwfChild)
				{
					DeleteChild(pvwfChild);
				}
				SAFERELEASE(p3DChild);
			}
		}
	}
	if (m_dwRef == 1 && m_pVWGeometryObject)
	{
		// Only the GeometryObject now has a reference to this object.
		// The GeometryObject will need to know this so the object can be flagged 
		// as unused.
		m_pVWGeometryObject->ReleaseGeometry();
	}
	if (dwRef == 0 && pVWGeometryCache)
	{
		// Only the GeometryCache now has a reference to this object. The GeometryCache 
		// will need to know this so the object can be flagged as unused.
		pVWGeometryCache->ReleaseGeometry((IVWFrame*)this);
	}

	SAFERELEASE(p3DChildren);
	SAFERELEASE(p3DChild);

	return dwRef;
}
 
STDMETHODIMP CVWFrameObject::put_GeometryCache(IVWGeometryCache *pVWGeometryCache)
{
	SAFERELEASE(m_pVWGeometryCache);
	m_pVWGeometryCache = pVWGeometryCache;
	SAFEADDREF(pVWGeometryCache);
	return S_OK;
}

STDMETHODIMP CVWFrameObject::put_GeometryObject(IVWGeometry *pVWGeometry)
{
	SAFERELEASE(m_pVWGeometryObject);
	m_pVWGeometryObject = pVWGeometry;
	SAFEADDREF(pVWGeometry);
	return S_OK;
}

STDMETHODIMP CVWFrameObject::get_GeometryObject(IVWGeometry **ppVWGeometry)
{
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(ppVWGeometry);

	*ppVWGeometry = m_pVWGeometryObject;
	SAFEADDREF(m_pVWGeometryObject);

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::put_World(IWorld* pworld)
{
	HRESULT hr = S_OK;
	static CComBSTR bstrDirect3DRM("Direct3DRM");
	static CComBSTR bstrDirect2DRM("Direct2DRM");
	static CComBSTR bstrAnimator("Animator");

	CVWFrameObjectBase::put_World(pworld);

	// REVIEW: creating static weak references to tools here
	// This should be OK since IWorld actually manages lifetime of tools;
	// it also prevents needing to have first VWFrame remember to free the tools
	if (!m_pDirect3DRM)
	{
		if (FAILED(m_pWorld->get_Tool(bstrDirect3DRM, (IUnknown**)&m_pDirect3DRM)))
			goto EXIT_FAIL;

	}

	if (!m_pDirect2DRM)
	{
		if (FAILED(m_pWorld->get_Tool(bstrDirect2DRM, (IUnknown**)&m_pDirect2DRM)))
			goto EXIT_FAIL;

	}

	if (!m_pVWAnimator)
	{
		if (FAILED(hr = m_pWorld->get_Tool(bstrAnimator, (IUnknown**)&m_pVWAnimator)))
			goto EXIT_FAIL;

	}

EXIT_FAIL:

	return S_OK;
}

HRESULT
D3DFrameCopy(IDirect3DRM *pd3drm, LPDIRECT3DRMFRAME lpSrcFrame, LPDIRECT3DRMFRAME *ppDstFrame, LPDIRECT3DRMANIMATIONSET2 pSrcAnimSet, LPDIRECT3DRMANIMATIONSET2 pDestAnimSet, IVWFrame* lpVWRootFrame)
{
	int nVisuals, cVisuals, nLights, cLights, nChildren, cChildren;
	HRESULT hr;
	LPDIRECT3DRMVISUALARRAY lpVisualArray = NULL;
	LPDIRECT3DRMVISUAL lpVisual = NULL, lpCloneVisual = NULL;
 	LPDIRECT3DRMLIGHTARRAY	lpLightArray = NULL;
 	LPDIRECT3DRMLIGHT		lpLight = NULL, lpCloneLight = NULL;
	LPDIRECT3DRMFRAMEARRAY  lpChildArray = NULL;
	LPDIRECT3DRMFRAME lpFrame = NULL;
	LPDIRECT3DRMFRAME lpSubFrame = NULL;
	DWORD dwSize = 256;
	char szbuf[256];

	hr = lpSrcFrame->Clone(0, IID_IDirect3DRMFrame, (void**)ppDstFrame);
	lpSrcFrame->GetName(&dwSize, szbuf);
	(*ppDstFrame)->SetName(szbuf);

	// now loop through all the visuals at this level, clone and add them
	hr = lpSrcFrame->GetVisuals(&lpVisualArray);
	nVisuals = lpVisualArray->GetSize();
	for (cVisuals = 0; cVisuals<nVisuals; cVisuals++) 
	{
		hr = lpVisualArray->GetElement(cVisuals, &lpVisual);
        lpVisual->Clone(0, IID_IDirect3DRMVisual, (void **) &lpCloneVisual);
		dwSize = 256;
		lpVisual->GetName(&dwSize, szbuf);
		lpCloneVisual->SetName(szbuf);
		(*ppDstFrame)->AddVisual(lpCloneVisual);
		SAFERELEASE(lpVisual);
		SAFERELEASE(lpCloneVisual);
	}
	SAFERELEASE(lpVisualArray);

	// now loop through all the lights at this level, clone and add them
	hr = lpSrcFrame->GetLights(&lpLightArray);
	nLights = lpLightArray->GetSize();
	for (cLights = 0; cLights<nLights; cLights++) 
	{
		hr = lpLightArray->GetElement(cLights, &lpLight);
        lpLight->Clone(0, IID_IDirect3DRMLight, (void **) &lpCloneLight);
		lpCloneLight->SetType(lpLight->GetType());
		lpCloneLight->SetColor(lpLight->GetColor());
 		lpCloneLight->SetConstantAttenuation(lpLight->GetConstantAttenuation());
 		lpCloneLight->SetLinearAttenuation(lpLight->GetLinearAttenuation());
 		lpCloneLight->SetQuadraticAttenuation(lpLight->GetQuadraticAttenuation());
 		lpCloneLight->SetPenumbra(lpLight->GetPenumbra());
 		lpCloneLight->SetUmbra(lpLight->GetUmbra());
 		lpCloneLight->SetRange(lpLight->GetRange());
		(*ppDstFrame)->AddLight(lpCloneLight);
		SAFERELEASE(lpLight);
		SAFERELEASE(lpCloneLight);
	}
	SAFERELEASE(lpLightArray);

	// now recursively loop through frames and add them to current frame
	hr = lpSrcFrame->GetChildren(&lpChildArray);
	nChildren = lpChildArray->GetSize();
	for (cChildren = 0; cChildren<nChildren; cChildren++) {
		lpChildArray->GetElement(cChildren, &lpFrame);
		D3DFrameCopy(pd3drm, lpFrame, &lpSubFrame, pSrcAnimSet, pDestAnimSet, lpVWRootFrame);
		(*ppDstFrame)->AddChild(lpSubFrame);
		SAFERELEASE(lpSubFrame);
		SAFERELEASE(lpFrame);
	}
	SAFERELEASE(lpChildArray);

	//Now copy animations
	CComPtr <IDirect3DRMAnimation> pNewAnimation;
	CComPtr <IDirect3DRMAnimation2> pSrcAnimation2, pNewAnimation2;

	if (pSrcAnimSet)
	{
		CComPtr <IDirect3DRMAnimationArray> pD3DAnimArray;

		//char szAnimName[256];
		//DWORD dwSize = 256;
		int nAnimCount, i;

		pSrcAnimSet->GetAnimations(&pD3DAnimArray);
		nAnimCount = pD3DAnimArray->GetSize();

		for (i = 0; i < nAnimCount; i++)
		{
			CComPtr <IDirect3DRMFrame3> pFrame, pNewFrame3;
			CComPtr <IDirect3DRMAnimation2> pD3DAnim;

			hr = pD3DAnimArray->GetElement(i, &pD3DAnim);
			ASSERT(hr == S_OK);

			hr = pD3DAnim->GetFrame(&pFrame.p);
			ASSERT(hr == S_OK);

			lpSrcFrame->QueryInterface( IID_IDirect3DRMFrame3, (LPVOID*) &pNewFrame3.p);

			if (pFrame.p == pNewFrame3.p)
			{
				pSrcAnimation2 = pD3DAnim;
				break;
			}
		}

		//Did we find an animation for this frame?
		if (pSrcAnimation2.p)
		{
			D3DRMANIMATIONOPTIONS d3dAnimOptions;
			DWORD dwNumKeys = 0;
			unsigned int j;
			D3DRMANIMATIONKEY* pKeys = NULL;
			char szTmp[256];

			pd3drm->CreateAnimation(&pNewAnimation.p);

			pNewAnimation->QueryInterface( IID_IDirect3DRMAnimation2, (LPVOID*) &pNewAnimation2.p);

			//Get number of keys
			pSrcAnimation2->GetKeys(0, 9999999, &dwNumKeys, NULL);

			pKeys = new D3DRMANIMATIONKEY[dwNumKeys];

			//Actually get the keys
			pSrcAnimation2->GetKeys(0, 9999999, &dwNumKeys, pKeys);

			//Add to the new animation
			for (j = 0; j < dwNumKeys; j++)
			{
				pKeys[j].dwID = 0;
				pNewAnimation2->AddKey(&(pKeys[j]));
			}

			delete [] pKeys;

			//Set the options on the new animation
			d3dAnimOptions = pSrcAnimation2->GetOptions();
			pNewAnimation2->SetOptions(d3dAnimOptions);

			//Clear the string
			szTmp[0] = 0;
			dwSize = 256;
			pSrcAnimation2->GetName(&dwSize, szTmp);
			pNewAnimation->SetName(szTmp);

			//Set frame for animation
			pNewAnimation->SetFrame(*ppDstFrame);

	 		pDestAnimSet->AddAnimation(pNewAnimation2);

		}
	}
	return S_OK;
}

// this routine returns the cWhich mesh builder associated with a vwframe

STDMETHODIMP CVWFrameObject::GetVisual(REFIID ref, void** visualObj, int cWhich)
{
	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;

	hr = D3DFrameGetVisual(m_pd3drmf, ref, visualObj, cWhich);

EXIT_FAIL:

	return hr;
}

HRESULT D3DFrameGetVisual(IDirect3DRMFrame *pd3drmf, REFIID ref, void** visualObj, int cWhich)
{
	CComPtr<IDirect3DRMVisualArray> lpVisualArray;
	CComPtr<IDirect3DRMVisual>		lpVisual, lpRequiredVisual;
	int								nCount, i, nTemp = 0;
	HRESULT hr = S_OK;

	if (FAILED(hr = pd3drmf->GetVisuals(&lpVisualArray)))
		return hr;

	nCount = lpVisualArray->GetSize();

	for (i = 0; i < nCount; i++)
	{
		if (FAILED(hr = lpVisualArray->GetElement(i, &lpVisual)))
			return hr;

		hr = lpVisual->QueryInterface(ref, (void**)&lpRequiredVisual);
		if (hr == E_NOINTERFACE || hr == CLASS_E_CLASSNOTAVAILABLE) hr = S_OK;
		if (FAILED(hr)) return hr;

		if (lpRequiredVisual)
		{
			if (nTemp == cWhich)
			{
				*visualObj = (void*)lpRequiredVisual;
				lpRequiredVisual.p->AddRef();
				return hr;
			}
			lpRequiredVisual.Release();
			nTemp++;
		}
		lpVisual.Release();
	}
	return E_INVALIDARG;
}

//Gets the number of visuals under the frame
STDMETHODIMP CVWFrameObject::GetMeshCount(VARIANT *pvarCount)
{
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(pvarCount);

	CHECKANDCREATEFRAME;

	VariantClear(pvarCount);

	if (m_pd3drmf)																		
		hr = GetVisualCountByIID(IID_IDirect3DRMMeshBuilder, pvarCount);
	else
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CVWFrameObject::GetMeshCount: No D3DRMFrame.\n");	



EXIT_FAIL:
	return hr;
}

//Gets the number of visuals under the frame
STDMETHODIMP CVWFrameObject::GetProgressiveMeshCount(VARIANT *pvarCount)
{
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(pvarCount);

	CHECKANDCREATEFRAME;
	
	VariantClear(pvarCount);

	hr = GetVisualCountByIID(IID_IDirect3DRMProgressiveMesh, pvarCount);

EXIT_FAIL:
	return hr;
}

//Gets the number of visuals under the frame
STDMETHODIMP CVWFrameObject::GetVisualCount(VARIANT *pvarCount)
{
	LPDIRECT3DRMVISUALARRAY lpVisualArray = NULL;
	LPDIRECT3DRMVISUAL lpVisual = NULL;
	LPDIRECT3DRMMESH lpMesh = NULL;
	HRESULT hr = S_OK;
	int nCount, i;

	CHECK_OUT_POINTER(pvarCount);

	CHECKANDCREATEFRAME;

	VariantClear(pvarCount);
	pvarCount->vt = VT_I4;
	pvarCount->lVal = 0;

	if (SUCCEEDED(hr = m_pd3drmf->GetVisuals(&lpVisualArray) ))
	{
		nCount = lpVisualArray->GetSize();
		for (i = 0; i < nCount; i++)
		{
			hr = lpVisualArray->GetElement(i, &lpVisual);
			if (FAILED(hr)) goto EXIT_FAIL;

			pvarCount->lVal++;

			SAFERELEASE(lpVisual);
		}
	}

EXIT_FAIL:
	SAFERELEASE(lpVisualArray);
	SAFERELEASE(lpVisual);
	SAFERELEASE(lpMesh);

	return hr;
}

//Gets the number of visuals under the frame
STDMETHODIMP CVWFrameObject::GetVisualCountByIID(REFIID pref, VARIANT *pvarCount)
{
	LPDIRECT3DRMVISUALARRAY lpVisualArray = NULL;
	LPDIRECT3DRMVISUAL lpVisual = NULL;
	LPDIRECT3DRMMESH lpMesh = NULL;
	HRESULT hr = S_OK;
	int nCount, i;

	CHECK_OUT_POINTER(pvarCount);

	CHECKANDCREATEFRAME;

	VariantClear(pvarCount);
	pvarCount->vt = VT_I4;
	pvarCount->lVal = 0;

	if (SUCCEEDED(hr = m_pd3drmf->GetVisuals(&lpVisualArray) ))
	{
		nCount = lpVisualArray->GetSize();
		for (i = 0; i < nCount; i++)
		{
			hr = lpVisualArray->GetElement(i, &lpVisual);
			if (FAILED(hr)) goto EXIT_FAIL;

			lpMesh = NULL;
			hr = lpVisual->QueryInterface(pref, (void**) &lpMesh);
			if (hr == E_NOINTERFACE || hr == CLASS_E_CLASSNOTAVAILABLE) hr = S_OK;
			if (FAILED(hr)) goto EXIT_FAIL;

			if (lpMesh)
				pvarCount->lVal++;

			SAFERELEASE(lpMesh);
			SAFERELEASE(lpVisual);
		}
	}

EXIT_FAIL:
	SAFERELEASE(lpVisualArray);
	SAFERELEASE(lpVisual);
	SAFERELEASE(lpMesh);

	return hr;
}

// for now we'll default most of these to operating on the first mesh in the frame
// later we can write other routines that will index or operate on all the meshes
STDMETHODIMP CVWFrameObject::SetMeshColor(long nMeshIndex, float fRed, float fGreen, float fBlue)
{
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;

	D3DFrameSetMeshColor(m_pd3drmf, nMeshIndex, fRed, fGreen, fBlue);

EXIT_FAIL:
	SAFERELEASE(lpMeshBuilder);
	return hr;
}


HRESULT D3DFrameSetMeshColor(IDirect3DRMFrame *pd3drmf, long nMeshIndex, float fRed, float fGreen, float fBlue)
{
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	LPDIRECT3DRMFRAME		lpChildFrame = NULL;
	HRESULT hr = S_OK;

	CHECK_IN_POINTER(pd3drmf);

	if (nMeshIndex == VWFRAME_ALLFRAME || nMeshIndex == VWFRAME_ALLHIERARCHY)
	{																						
		int	i, nCount;
		LPDIRECT3DRMVISUALARRAY lpVisualArray = NULL;
		if (SUCCEEDED(hr = pd3drmf->GetVisuals(&lpVisualArray) && lpVisualArray))
		{
			nCount = lpVisualArray->GetSize();
			for (i = 0; i < nCount; i++)
				D3DFrameSetMeshColor(pd3drmf, i, fRed, fGreen, fBlue );
		}
		SAFERELEASE(lpVisualArray);
		if (nMeshIndex == VWFRAME_ALLHIERARCHY)												
		{																					
			LPDIRECT3DRMFRAMEARRAY lpFrameArray = NULL;
			if (SUCCEEDED(hr = pd3drmf->GetChildren(&lpFrameArray)) && lpFrameArray)
			{																				
				nCount = lpFrameArray->GetSize();											
				for (i = 0; i < nCount; i++)
				{
					if (SUCCEEDED(lpFrameArray->GetElement(i, &lpChildFrame)) && lpChildFrame)
					{
						D3DFrameSetMeshColor(lpChildFrame, VWFRAME_ALLHIERARCHY, fRed, fGreen, fBlue);
					}
					SAFERELEASE(lpChildFrame);
				}
			}														
			SAFERELEASE(lpFrameArray);
		}				
	}
	else
	{
		if (SUCCEEDED(hr = D3DFrameGetVisual(pd3drmf, IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex))) 
			hr = lpMeshBuilder->SetColor( D3DRMCreateColorRGB(fRed, fGreen, fBlue) );
	}

EXIT_FAIL:
	SAFERELEASE(lpMeshBuilder);
	SAFERELEASE(lpChildFrame);

	return hr;
}


STDMETHODIMP CVWFrameObject::GetMeshColor(long nMeshIndex, VARIANT *pfRed, VARIANT *pfGreen, VARIANT *pfBlue)
{
	D3DCOLOR rgbColor;
	CComPtr<IDirect3DRMMeshBuilder> lpMeshBuilder;
	CComPtr<IDirect3DRMFaceArray>	lpFaceArray;
	CComPtr<IDirect3DRMFace>		lpFace;
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(pfRed);
	CHECK_OUT_POINTER(pfGreen);
	CHECK_OUT_POINTER(pfBlue);

	CHECKANDCREATEFRAME;

	VariantClear(pfRed);
	VariantClear(pfGreen);
	VariantClear(pfBlue);

	rgbColor = D3DRMCreateColorRGB(0.0f, 0.0f, 0.0f);
	if (SUCCEEDED(hr = GetVisual(IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex))) 
	{
		if (SUCCEEDED(hr = lpMeshBuilder->GetFaces(&lpFaceArray)) )
		{
			if (lpFaceArray->GetSize() == 0)
				return ReportFrameError(VWFRAME_E_MESHHASNOFACES);
			if (SUCCEEDED(hr = lpFaceArray->GetElement(0, &lpFace)) )
			{
				rgbColor = lpFace->GetColor();
			}
		}
	}

	pfRed->vt = VT_R4;
	pfGreen->vt = VT_R4;
	pfBlue->vt = VT_R4;

	pfRed->fltVal = D3DRMColorGetRed(rgbColor);
	pfGreen->fltVal = D3DRMColorGetGreen(rgbColor);
	pfBlue->fltVal = D3DRMColorGetBlue(rgbColor);

EXIT_FAIL:
	return hr;
}

// since we don't want to have to worry about shademode, lightmode, etc.
// we'll just do a simple switch statement and the 5 renderquality modes
// discussed in the dx documentation
STDMETHODIMP CVWFrameObject::SetMeshQuality(long nMeshIndex, int nQuality)
{
	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;

	hr = D3DFrameSetMeshQuality(m_pd3drmf, nMeshIndex, nQuality);

EXIT_FAIL:
	return hr;
}

HRESULT D3DFrameSetMeshQuality(IDirect3DRMFrame *pd3drmf, long nMeshIndex, int nQuality)
{
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	LPDIRECT3DRMFRAME		lpChildFrame = NULL;
	D3DRMRENDERQUALITY		quality = 0;
	HRESULT hr = S_OK;

	CHECK_IN_POINTER(pd3drmf);

	switch (nQuality) 
	{
		case 0:
			quality = D3DRMRENDER_WIREFRAME;
			break;
		case 1:
			quality = D3DRMRENDER_UNLITFLAT;
			break;
		case 2:
			quality = D3DRMRENDER_FLAT;
			break;
		case 3:
			quality = D3DRMRENDER_GOURAUD;
			break;
		case 4:
			quality = D3DRMRENDER_PHONG;
			break;
		default:
			quality = D3DRMRENDER_UNLITFLAT;
			break;

	}

	if (nMeshIndex == VWFRAME_ALLFRAME || nMeshIndex == VWFRAME_ALLHIERARCHY)
	{																						
		int	i, nCount;
		LPDIRECT3DRMVISUALARRAY lpVisualArray = NULL;
		if (SUCCEEDED(hr = pd3drmf->GetVisuals(&lpVisualArray) && lpVisualArray))
		{
			nCount = lpVisualArray->GetSize();
			for (i = 0; i < nCount; i++)
				D3DFrameSetMeshQuality(pd3drmf, i, nQuality);
		}
		SAFERELEASE(lpVisualArray);
		if (nMeshIndex == VWFRAME_ALLHIERARCHY)												
		{																					
			LPDIRECT3DRMFRAMEARRAY lpFrameArray = NULL;
			if (SUCCEEDED(hr = pd3drmf->GetChildren(&lpFrameArray)) && lpFrameArray)
			{																				
				nCount = lpFrameArray->GetSize();											
				for (i = 0; i < nCount; i++)
				{
					if (SUCCEEDED(lpFrameArray->GetElement(i, &lpChildFrame)) && lpChildFrame)
					{
						D3DFrameSetMeshQuality(lpChildFrame, VWFRAME_ALLHIERARCHY, nQuality);
					}
					SAFERELEASE(lpChildFrame);
				}
			}														
			SAFERELEASE(lpFrameArray);
		}				
	}
	else
	{
		if (SUCCEEDED(hr = D3DFrameGetVisual(pd3drmf, IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex))) 
		{
			DWORD	dwSize = 100;
			char	szName[100];
			lpMeshBuilder->GetName(&dwSize, szName);
			if (strcmp(szName, LEAVEMESHQUALITY))
				hr = lpMeshBuilder->SetQuality(quality);
		}
	}

EXIT_FAIL:
	SAFERELEASE(lpMeshBuilder);
	SAFERELEASE(lpChildFrame);

	return hr;
}

STDMETHODIMP CVWFrameObject::GetMeshQuality(long nMeshIndex, int *nQuality )
{
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	D3DRMRENDERQUALITY quality = 0;
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(nQuality);

	CHECKANDCREATEFRAME;

	//Initialize to zero.
	*nQuality = 0;

	if (SUCCEEDED(hr = GetVisual(IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex))) 
	{
		quality = lpMeshBuilder->GetQuality();
	}
	switch (quality) 
	{
		case D3DRMRENDER_WIREFRAME:
			*nQuality = 0;
			break;
		case D3DRMRENDER_UNLITFLAT:
			*nQuality = 1;
			break;
		case D3DRMRENDER_FLAT:
			*nQuality = 2;
			break;
		case D3DRMRENDER_GOURAUD:
			*nQuality = 3;
			break;
		case D3DRMRENDER_PHONG:
			*nQuality = 4;
			break;
		default:
			*nQuality = 0;
			break;
	}

EXIT_FAIL:
	SAFERELEASE(lpMeshBuilder);
	return hr;
}

STDMETHODIMP CVWFrameObject::SetProgressiveMeshQuality(long nMeshIndex, int nQuality)
{
#ifdef IDirect3D2_CreateDevice 
	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;

	hr = D3DFrameSetProgressiveMeshQuality(m_pd3drmf, nMeshIndex, nQuality);

EXIT_FAIL:
	return hr;

#else
	return E_NOTIMPL;
#endif
}

HRESULT D3DFrameSetProgressiveMeshQuality(IDirect3DRMFrame *pd3drmf, long nMeshIndex, int nQuality)
{
	HRESULT hr = S_OK;
#ifdef IDirect3D2_CreateDevice 
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	LPDIRECT3DRMFRAME		lpChildFrame = NULL;
	D3DRMRENDERQUALITY		quality = 0;

	CHECK_IN_POINTER(pd3drmf);

	switch (nQuality) 
	{
		case 0:
			quality = D3DRMRENDER_WIREFRAME;
			break;
		case 1:
			quality = D3DRMRENDER_UNLITFLAT;
			break;
		case 2:
			quality = D3DRMRENDER_FLAT;
			break;
		case 3:
			quality = D3DRMRENDER_GOURAUD;
			break;
		case 4:
			quality = D3DRMRENDER_PHONG;
			break;
		default:
			quality = D3DRMRENDER_UNLITFLAT;
			break;

	}

	if (nMeshIndex == VWFRAME_ALLFRAME || nMeshIndex == VWFRAME_ALLHIERARCHY)
	{																						
		int	i, nCount;
		LPDIRECT3DRMVISUALARRAY lpVisualArray = NULL;
		if (SUCCEEDED(hr = pd3drmf->GetVisuals(&lpVisualArray) && lpVisualArray))
		{
			nCount = lpVisualArray->GetSize();
			for (i = 0; i < nCount; i++)
				D3DFrameSetProgressiveMeshQuality(pd3drmf, i, nQuality);
		}
		SAFERELEASE(lpVisualArray);
		if (nMeshIndex == VWFRAME_ALLHIERARCHY)												
		{																					
			LPDIRECT3DRMFRAMEARRAY lpFrameArray = NULL;
			if (SUCCEEDED(hr = pd3drmf->GetChildren(&lpFrameArray)) && lpFrameArray)
			{																				
				nCount = lpFrameArray->GetSize();											
				for (i = 0; i < nCount; i++)
				{
					if (SUCCEEDED(lpFrameArray->GetElement(i, &lpChildFrame)) && lpChildFrame)
					{
						D3DFrameSetProgressiveMeshQuality(lpChildFrame, VWFRAME_ALLHIERARCHY, nQuality);
					}
					SAFERELEASE(lpChildFrame);
				}
			}														
			SAFERELEASE(lpFrameArray);
		}				
	}
	else
	{
		if (SUCCEEDED(hr = D3DFrameGetVisual(pd3drmf, IID_IDirect3DRMProgressiveMesh, (void**)&lpMeshBuilder, nMeshIndex))) 
			hr = lpMeshBuilder->SetQuality(quality);
	}

EXIT_FAIL:
	SAFERELEASE(lpMeshBuilder);
	SAFERELEASE(lpChildFrame);
#endif
	return hr;
}


STDMETHODIMP CVWFrameObject::GetProgressiveMeshQuality(long nMeshIndex, int *nQuality) 
{
#ifdef IDirect3D2_CreateDevice 
	IDirect3DRMProgressiveMesh *pm = NULL;
	HRESULT hr = S_OK;
	D3DRMRENDERQUALITY d3dqQuality = 0;

	CHECK_OUT_POINTER(nQuality);

	CHECKANDCREATEFRAME;

	hr = GetVisual(IID_IDirect3DRMProgressiveMesh, (void**)&pm, nMeshIndex);
	if (SUCCEEDED(hr)) 
	{
		//We found the progmesh. Let's return the quality
		pm->GetQuality(&d3dqQuality);
		switch (d3dqQuality) 
		{
			case D3DRMRENDER_WIREFRAME:
				*nQuality = 0;
				break;
			case D3DRMRENDER_UNLITFLAT:
				*nQuality = 1;
				break;
			case D3DRMRENDER_FLAT:
				*nQuality = 2;
				break;
			case D3DRMRENDER_GOURAUD:
				*nQuality = 3;
				break;
			case D3DRMRENDER_PHONG:
				*nQuality = 4;
				break;
			default:
				*nQuality = 0;
				break;
		}
	}

EXIT_FAIL:
	SAFERELEASE(pm);
	return hr;

#else
	return E_NOTIMPL;
#endif
}


STDMETHODIMP CVWFrameObject::GetProgressiveMeshDetail(long nMeshIndex, float *detail) 
{
#ifdef IDirect3D2_CreateDevice 
	IDirect3DRMProgressiveMesh *pm = NULL;
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(detail);

	CHECKANDCREATEFRAME;

	hr = GetVisual(IID_IDirect3DRMProgressiveMesh, (void**)&pm, nMeshIndex);
	if (SUCCEEDED(hr)) 
	{
		//We found the progmesh. Let's return the detail.
		hr = pm->GetDetail(detail);
	}

EXIT_FAIL:
	SAFERELEASE(pm);
	return hr;
#else
	return E_NOTIMPL;
#endif
}


HRESULT D3DFrameSetProgressiveMeshDetail(IDirect3DRMFrame *pd3drmf, long nMeshIndex, float detail)
{
	IDirect3DRMProgressiveMesh *pm = NULL;
	LPDIRECT3DRMFRAME		lpChildFrame = NULL;
	HRESULT hr = S_OK;

	CHECK_IN_POINTER(pd3drmf);

	if (nMeshIndex == VWFRAME_ALLFRAME || nMeshIndex == VWFRAME_ALLHIERARCHY)
	{																						
		int	i, nCount;
		LPDIRECT3DRMVISUALARRAY lpVisualArray = NULL;
		if (SUCCEEDED(hr = pd3drmf->GetVisuals(&lpVisualArray) && lpVisualArray))
		{
			nCount = lpVisualArray->GetSize();
			for (i = 0; i < nCount; i++)
				D3DFrameSetProgressiveMeshDetail(pd3drmf, i, detail);
		}
		SAFERELEASE(lpVisualArray);
		if (nMeshIndex == VWFRAME_ALLHIERARCHY)												
		{																					
			LPDIRECT3DRMFRAMEARRAY lpFrameArray = NULL;
			if (SUCCEEDED(hr = pd3drmf->GetChildren(&lpFrameArray)) && lpFrameArray)
			{																				
				nCount = lpFrameArray->GetSize();											
				for (i = 0; i < nCount; i++)
				{
					if (SUCCEEDED(lpFrameArray->GetElement(i, &lpChildFrame)) && lpChildFrame)
					{
						D3DFrameSetProgressiveMeshDetail(lpChildFrame, VWFRAME_ALLHIERARCHY, detail);
					}
					SAFERELEASE(lpChildFrame);
				}
			}														
			SAFERELEASE(lpFrameArray);
		}				
	}
	else
	{
		if (SUCCEEDED(hr = D3DFrameGetVisual(pd3drmf, IID_IDirect3DRMProgressiveMesh, (void**)&pm, nMeshIndex))) 
			hr = pm->SetDetail(detail);
	}

EXIT_FAIL:
	SAFERELEASE(pm);
	SAFERELEASE(lpChildFrame);

	return hr;
}

//New for progressive mesh detail control
STDMETHODIMP CVWFrameObject::SetProgressiveMeshDetail(long nMeshIndex, float detail) 
{
#ifdef IDirect3D2_CreateDevice 

	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;

	if (detail > 1.0) 
		detail = 1.0;
	else if (detail < 0.0) 
		detail = 0.0;

	hr = D3DFrameSetProgressiveMeshDetail(m_pd3drmf, nMeshIndex, detail);

EXIT_FAIL:
	return hr;
#else
	return E_NOTIMPL;
#endif
}


STDMETHODIMP CVWFrameObject::GetProgressiveMeshFaceDetail(long nMeshIndex, DWORD *detail) 
{
#ifdef IDirect3D2_CreateDevice 
	IDirect3DRMProgressiveMesh *pm = NULL;
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(detail);

	CHECKANDCREATEFRAME;

	hr = GetVisual(IID_IDirect3DRMProgressiveMesh, (void**)&pm, nMeshIndex);
	if (SUCCEEDED(hr)) 
	{
		//We found the progmesh. Let's return the detail.
		hr = pm->GetFaceDetail(detail);
	}

EXIT_FAIL:
	SAFERELEASE(pm);
	return hr;
#else
	return E_NOTIMPL;
#endif
}

STDMETHODIMP CVWFrameObject::SetProgressiveMeshFaceDetail(long nMeshIndex, DWORD detail)
{
#ifdef IDirect3D2_CreateDevice 
	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;

	D3DFrameSetProgressiveMeshFaceDetail(m_pd3drmf, nMeshIndex, detail);

EXIT_FAIL:
	return hr;
#else
	return E_NOTIMPL;
#endif
}

HRESULT D3DFrameSetProgressiveMeshFaceDetail(IDirect3DRMFrame *pd3drmf, long nMeshIndex, DWORD detail)
{
	IDirect3DRMProgressiveMesh *pm = NULL;
	LPDIRECT3DRMFRAME		lpChildFrame = NULL;
	HRESULT hr = S_OK;

	CHECK_IN_POINTER(pd3drmf);

	if (nMeshIndex == VWFRAME_ALLFRAME || nMeshIndex == VWFRAME_ALLHIERARCHY)
	{																						
		int	i, nCount;
		LPDIRECT3DRMVISUALARRAY lpVisualArray = NULL;
		if (SUCCEEDED(hr = pd3drmf->GetVisuals(&lpVisualArray) && lpVisualArray))
		{
			nCount = lpVisualArray->GetSize();
			for (i = 0; i < nCount; i++)
				D3DFrameSetProgressiveMeshFaceDetail(pd3drmf, i, detail);
		}
		SAFERELEASE(lpVisualArray);
		if (nMeshIndex == VWFRAME_ALLHIERARCHY)												
		{																					
			LPDIRECT3DRMFRAMEARRAY lpFrameArray = NULL;
			if (SUCCEEDED(hr = pd3drmf->GetChildren(&lpFrameArray)) && lpFrameArray)
			{																				
				nCount = lpFrameArray->GetSize();											
				for (i = 0; i < nCount; i++)
				{
					if (SUCCEEDED(lpFrameArray->GetElement(i, &lpChildFrame)) && lpChildFrame)
					{
						D3DFrameSetProgressiveMeshFaceDetail(lpChildFrame, VWFRAME_ALLHIERARCHY, detail);
					}
					SAFERELEASE(lpChildFrame);
				}
			}														
			SAFERELEASE(lpFrameArray);
		}				
	}
	else
	{
		if (SUCCEEDED(hr = D3DFrameGetVisual(pd3drmf, IID_IDirect3DRMProgressiveMesh, (void**)&pm, nMeshIndex))) 
			hr = pm->SetFaceDetail(detail);
	}

EXIT_FAIL:
	SAFERELEASE(pm);
	SAFERELEASE(lpChildFrame);

	return hr;
}


STDMETHODIMP CVWFrameObject::GetProgressiveMeshVertexDetail(long nMeshIndex, DWORD *detail) 
{
#ifdef IDirect3D2_CreateDevice 
	IDirect3DRMProgressiveMesh *pm = NULL;
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(detail);

	CHECKANDCREATEFRAME;

	hr = GetVisual(IID_IDirect3DRMProgressiveMesh, (void**)&pm, nMeshIndex);
	if (SUCCEEDED(hr)) 
	{
		//We found the progmesh. Let's return the detail.
		hr = pm->GetVertexDetail(detail);
	}

EXIT_FAIL:
	SAFERELEASE(pm);
	return hr;
#else
	return E_NOTIMPL;
#endif
}

STDMETHODIMP CVWFrameObject::SetProgressiveMeshVertexDetail(long nMeshIndex, DWORD detail) 
{
#ifdef IDirect3D2_CreateDevice 
	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;

	D3DFrameSetProgressiveMeshVertexDetail(m_pd3drmf, nMeshIndex, detail);

EXIT_FAIL:
	return hr;
#else
	return E_NOTIMPL;
#endif
}

HRESULT D3DFrameSetProgressiveMeshVertexDetail(IDirect3DRMFrame *pd3drmf, long nMeshIndex, DWORD detail)
{
	IDirect3DRMProgressiveMesh *pm = NULL;
	LPDIRECT3DRMFRAME		lpChildFrame = NULL;
	HRESULT hr = S_OK;

	CHECK_IN_POINTER(pd3drmf);

	if (nMeshIndex == VWFRAME_ALLFRAME || nMeshIndex == VWFRAME_ALLHIERARCHY)
	{																						
		int	i, nCount;
		LPDIRECT3DRMVISUALARRAY lpVisualArray = NULL;
		if (SUCCEEDED(hr = pd3drmf->GetVisuals(&lpVisualArray) && lpVisualArray))
		{
			nCount = lpVisualArray->GetSize();
			for (i = 0; i < nCount; i++)
				D3DFrameSetProgressiveMeshVertexDetail(pd3drmf, i, detail);
		}
		SAFERELEASE(lpVisualArray);
		if (nMeshIndex == VWFRAME_ALLHIERARCHY)												
		{																					
			LPDIRECT3DRMFRAMEARRAY lpFrameArray = NULL;
			if (SUCCEEDED(hr = pd3drmf->GetChildren(&lpFrameArray)) && lpFrameArray)
			{																				
				nCount = lpFrameArray->GetSize();											
				for (i = 0; i < nCount; i++)
				{
					if (SUCCEEDED(lpFrameArray->GetElement(i, &lpChildFrame)) && lpChildFrame)
					{
						D3DFrameSetProgressiveMeshVertexDetail(lpChildFrame, VWFRAME_ALLHIERARCHY, detail);
					}
					SAFERELEASE(lpChildFrame);
				}
			}														
			SAFERELEASE(lpFrameArray);
		}				
	}
	else
	{
		if (SUCCEEDED(hr = D3DFrameGetVisual(pd3drmf, IID_IDirect3DRMProgressiveMesh, (void**)&pm, nMeshIndex))) 
			hr = pm->SetVertexDetail(detail);
	}

EXIT_FAIL:
	SAFERELEASE(pm);
	SAFERELEASE(lpChildFrame);

	return hr;
}


STDMETHODIMP CVWFrameObject::GetProgressiveMeshFaceDetailRange(long nMeshIndex, VARIANT *min, VARIANT *max) 
{
#ifdef IDirect3D2_CreateDevice 
	IDirect3DRMProgressiveMesh *pm = NULL;
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(min);
	CHECK_OUT_POINTER(max);

	CHECKANDCREATEFRAME;

	min->vt = max->vt = VT_I4;

	hr = GetVisual(IID_IDirect3DRMProgressiveMesh, (void**)&pm, nMeshIndex);
	if (SUCCEEDED(hr)) 
	{
		//let's set the detail of the mesh
		hr = pm->GetFaceDetailRange((ULONG *)&min->lVal, (ULONG*)&max->lVal);
	}

EXIT_FAIL:
	SAFERELEASE(pm);
	return hr;
#else
	return E_NOTIMPL;
#endif
}

STDMETHODIMP CVWFrameObject::GetProgressiveMeshVertexDetailRange(long nMeshIndex, VARIANT *min, VARIANT *max) 
{
#ifdef IDirect3D2_CreateDevice 
	IDirect3DRMProgressiveMesh *pm = NULL;
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(min);
	CHECK_OUT_POINTER(max);

	CHECKANDCREATEFRAME;

	min->vt = max->vt = VT_I4;

	hr = GetVisual(IID_IDirect3DRMProgressiveMesh, (void**)&pm, nMeshIndex);
	if (SUCCEEDED(hr)) 
	{
		//let's set the detail of the mesh
		hr = pm->GetVertexDetailRange((ULONG*)&min->lVal, (ULONG*)&max->lVal);
	}

EXIT_FAIL:
	SAFERELEASE(pm);
	return hr;
#else
	return E_NOTIMPL;
#endif
}

STDMETHODIMP CVWFrameObject::GetProgressiveMeshIndexByName(BSTR bstrMeshName, long *plIndex)
{
	return GetVisualIndexByNameAndIID(bstrMeshName, IID_IDirect3DRMProgressiveMesh, plIndex);
}

STDMETHODIMP CVWFrameObject::GetMeshTextureName(long nMeshIndex, BSTR* bstrTexture)
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	LPDIRECT3DRMTEXTURE	pTexture = NULL;
	LPDIRECT3DRMFACE	pFace = NULL;
	LPDIRECT3DRMFACEARRAY pFaceArray = NULL;
	char szTmp[256];
	DWORD dwSize = 256;

	CHECK_OUT_POINTER(bstrTexture);

	CHECKANDCREATEFRAME;

	if (SUCCEEDED(hr = GetVisual(IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex))) 
	{
		hr = lpMeshBuilder->GetFaces(&pFaceArray);
		if (FAILED(hr) || !pFaceArray) goto EXIT_FAIL;

		DWORD dwSize = pFaceArray->GetSize();

		if (dwSize > 0)
		{
			pFaceArray->GetElement(0, &pFace);
			if (FAILED(hr) || !pFace) goto EXIT_FAIL;
			
			pFace->GetTexture(&pTexture);
			if (FAILED(hr) || !pTexture) goto EXIT_FAIL;

			dwSize = 256;
			hr = pTexture->GetName(&dwSize, szTmp);
			if (FAILED(hr)) goto EXIT_FAIL;

			*bstrTexture = CComBSTR(szTmp).Copy();
		}
		else
			hr = E_FAIL;
	}

EXIT_FAIL:
	SAFERELEASE(lpMeshBuilder);
	SAFERELEASE(pTexture);
	SAFERELEASE(pFaceArray);
	SAFERELEASE(pFace);

	return hr;
}


STDMETHODIMP CVWFrameObject::SetMeshTextureFromURL(long nMeshIndex, BSTR bstrTexture)
{
	CComPtr<IInternetFileManager> pINetFile;
	CComPtr<IDirect2DRMImage>	  pImage;
	CComPtr<IDirect3DRMTexture>	  pTexture;
	static CComBSTR bstrInetfile("Inetfile");
	CComBSTR	bstrFinalURL , bstrFinalPath;
	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;

	if (SysStringLen(bstrTexture) != 0)
	{
		if (FAILED(hr = m_pWorld->get_Tool(bstrInetfile, (IUnknown**)&pINetFile))) 
			return hr;
		if (FAILED(hr = pINetFile->GetFileEx(bstrTexture, VARIANT_TRUE, &bstrFinalURL.m_str, &bstrFinalPath.m_str)))
			return hr;
		if (FAILED(hr = m_pDirect2DRM->CreateImageFromImageFile(bstrFinalPath, 0, &pImage)))
			return hr;
		if (FAILED(hr = pImage->GetD3DTexture(m_pDirect3DRM, &pTexture)))
			return hr;
	}

	if (pTexture)
	{
		pTexture->SetName( CString(bstrTexture) );
	}

	D3DFrameSetMeshTexture(m_pd3drmf, nMeshIndex, pTexture);

EXIT_FAIL:
	return hr;
}

HRESULT D3DFrameSetMeshTexture(IDirect3DRMFrame *pd3drmf, long nMeshIndex, LPDIRECT3DRMTEXTURE pTexture)
{
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	LPDIRECT3DRMFRAME		lpChildFrame = NULL;
	HRESULT hr = S_OK;

	CHECK_IN_POINTER(pd3drmf);

	if (nMeshIndex == VWFRAME_ALLFRAME || nMeshIndex == VWFRAME_ALLHIERARCHY)
	{																						
		int	i, nCount;
		LPDIRECT3DRMVISUALARRAY lpVisualArray = NULL;
		if (SUCCEEDED(hr = pd3drmf->GetVisuals(&lpVisualArray) && lpVisualArray))
		{
			nCount = lpVisualArray->GetSize();
			for (i = 0; i < nCount; i++)
				D3DFrameSetMeshTexture(pd3drmf, i, pTexture );
		}
		SAFERELEASE(lpVisualArray);
		if (nMeshIndex == VWFRAME_ALLHIERARCHY)												
		{																					
			LPDIRECT3DRMFRAMEARRAY lpFrameArray = NULL;
			if (SUCCEEDED(hr = pd3drmf->GetChildren(&lpFrameArray)) && lpFrameArray)
			{																				
				nCount = lpFrameArray->GetSize();											
				for (i = 0; i < nCount; i++)
				{
					if (SUCCEEDED(lpFrameArray->GetElement(i, &lpChildFrame)) && lpChildFrame)
					{
						D3DFrameSetMeshTexture(lpChildFrame, VWFRAME_ALLHIERARCHY, pTexture );
					}
					SAFERELEASE(lpChildFrame);
				}
			}														
			SAFERELEASE(lpFrameArray);
		}				
	}
	else
	{
		if (SUCCEEDED(hr = D3DFrameGetVisual(pd3drmf, IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex)))
			lpMeshBuilder->SetTexture(pTexture);
	}

EXIT_FAIL:
	SAFERELEASE(lpMeshBuilder);
	SAFERELEASE(lpChildFrame);

	return hr;
}


STDMETHODIMP CVWFrameObject::SetMeshTextureFromText(long nMeshIndex, BSTR bstrText, VARIANT_BOOL bShadowed, DWORD nWidth, DWORD nHeight, DWORD nPointHeight)
{	
	HRESULT hr = S_OK;
	LPDIRECT2DRMIMAGE	pTextImage = NULL;
	IDirect3DRMTexture	*pd3drmTextTexture = NULL;
	IDirectDrawSurface	*pddsTextTexture = NULL;

	CHECKANDCREATEFRAME;

	if (m_pDirect2DRM) 
	{
		if (FAILED(m_pDirect2DRM->CreateImageFromWrappedTextExt(bstrText, NULL, (BOOL)bShadowed, nWidth, nHeight, nPointHeight, &pTextImage)))
			goto EXIT_FAIL;
	
		//if (FAILED(hr = pTextImage->ExportD3DTexture(m_pDirect3DRM, &pddsTextTexture, &pd3drmTextTexture)))
		if (FAILED(hr = pTextImage->ExportD3DTextureEx(m_pDirect3DRM, &pddsTextTexture, flagTEXTURE_STRETCHNEAREST, &pd3drmTextTexture)))
			goto EXIT_FAIL;

		if (pd3drmTextTexture)
		{
			pd3drmTextTexture->SetName((const char *)CString(bstrText));
			D3DFrameSetMeshTexture(m_pd3drmf, nMeshIndex, pd3drmTextTexture);
		}		

	}

EXIT_FAIL:
	SAFERELEASE(pTextImage);
	SAFERELEASE(pddsTextTexture);
	SAFERELEASE(pd3drmTextTexture);

	return hr;
}

STDMETHODIMP CVWFrameObject::CreateDecalFromURL(BSTR bstrTexture)
{
	CComPtr<IInternetFileManager> pINetFile;
	CComPtr<IDirect2DRMImage>	  pImage = NULL;
	CComPtr<IDirect3DRMTexture>	  pTexture = NULL;
	static CComBSTR bstrInetfile("Inetfile");
	CComBSTR	bstrFinalURL , bstrFinalPath;
	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;

	if (FAILED(hr = m_pWorld->get_Tool(bstrInetfile, (IUnknown**)&pINetFile))) 
		return hr;
	if (FAILED(hr = pINetFile->GetFileEx(bstrTexture, VARIANT_TRUE, &bstrFinalURL.m_str, &bstrFinalPath.m_str)))
		return hr;
	if (FAILED(hr = m_pDirect2DRM->CreateImageFromImageFile(bstrFinalPath, 0, &pImage)))
		return hr;
	if (FAILED(hr = pImage->GetD3DTexture(m_pDirect3DRM, &pTexture)))
		return hr;

	if (pTexture) 
	{
		pTexture->SetName((const char *)CString(bstrTexture));
		m_pd3drmf->AddVisual(pTexture);
	}

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::DeleteDecal(long nMeshIndex)
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMTEXTURE lpTexture = NULL;

	CHECKANDCREATEFRAME;

	hr = GetVisual(IID_IDirect3DRMTexture, (void**)&lpTexture, nMeshIndex);
	if (FAILED(hr) || !lpTexture) goto EXIT_FAIL;

	hr = m_pd3drmf->DeleteVisual(lpTexture);

EXIT_FAIL:
	SAFERELEASE(lpTexture );
	return hr;
}

STDMETHODIMP CVWFrameObject::GetDecalCount(long* nCount)
{
	HRESULT hr = S_OK;
	VARIANT vCount;

	CHECK_OUT_POINTER(nCount);

	CHECKANDCREATEFRAME;

	hr = GetVisualCountByIID(IID_IDirect3DRMTexture, &vCount);
	if (FAILED(hr)) goto EXIT_FAIL;

	*nCount = vCount.lVal;

EXIT_FAIL:
	return S_OK;
}


STDMETHODIMP CVWFrameObject::GetDecalSize(long nMeshIndex, VARIANT* fWidth, VARIANT* fHeight, VARIANT* bScale)
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMTEXTURE lpTexture = NULL;
	DWORD dwSize;

	CHECK_OUT_POINTER(fWidth);
	CHECK_OUT_POINTER(fHeight);
	CHECK_OUT_POINTER(bScale);

	CHECKANDCREATEFRAME;

	VariantClear(fWidth);
	VariantClear(fHeight);
	VariantClear(bScale);

	hr = GetVisual(IID_IDirect3DRMTexture, (void**)&lpTexture, nMeshIndex);
	if (FAILED(hr) || !lpTexture) goto EXIT_FAIL;

	fWidth->vt = VT_R4;
	fHeight->vt = VT_R4;
	bScale->vt = VT_BOOL;

	lpTexture->GetDecalSize(&fWidth->fltVal, &fHeight->fltVal);
	dwSize = lpTexture->GetDecalScale();
	bScale->boolVal = (dwSize ? VARIANT_TRUE : VARIANT_FALSE);

EXIT_FAIL:
	SAFERELEASE(lpTexture );

	return hr;
}

HRESULT D3DFrameSetDecalSize(IDirect3DRMFrame *pd3drmf, long nMeshIndex, float fHeight, float fWidth, VARIANT_BOOL bScale)
{
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	LPDIRECT3DRMFRAME		lpChildFrame = NULL;
	LPDIRECT3DRMTEXTURE		lpTexture = NULL;
	HRESULT hr = S_OK;

	CHECK_IN_POINTER(pd3drmf);

	if (nMeshIndex == VWFRAME_ALLFRAME || nMeshIndex == VWFRAME_ALLHIERARCHY)
	{																						
		int	i, nCount;
		LPDIRECT3DRMVISUALARRAY lpVisualArray = NULL;
		if (SUCCEEDED(hr = pd3drmf->GetVisuals(&lpVisualArray) && lpVisualArray))
		{
			nCount = lpVisualArray->GetSize();
			for (i = 0; i < nCount; i++)
				D3DFrameSetDecalSize(pd3drmf, i, fHeight, fWidth, bScale );
		}
		SAFERELEASE(lpVisualArray);
		if (nMeshIndex == VWFRAME_ALLHIERARCHY)												
		{																					
			LPDIRECT3DRMFRAMEARRAY lpFrameArray = NULL;
			if (SUCCEEDED(hr = pd3drmf->GetChildren(&lpFrameArray)) && lpFrameArray)
			{																				
				nCount = lpFrameArray->GetSize();											
				for (i = 0; i < nCount; i++)
				{
					if (SUCCEEDED(lpFrameArray->GetElement(i, &lpChildFrame)) && lpChildFrame)
					{
						D3DFrameSetDecalSize(lpChildFrame, VWFRAME_ALLHIERARCHY, fHeight, fWidth, bScale);
					}
					SAFERELEASE(lpChildFrame);
				}
			}														
			SAFERELEASE(lpFrameArray);
		}				
	}
	else
	{
		if (SUCCEEDED(hr = D3DFrameGetVisual(pd3drmf, IID_IDirect3DRMTexture, (void**)&lpTexture, nMeshIndex))) 
		{
			lpTexture->SetDecalSize(fHeight, fWidth);
			lpTexture->SetDecalScale((DWORD)bScale);
		}
	}

EXIT_FAIL:
	SAFERELEASE(lpMeshBuilder);
	SAFERELEASE(lpChildFrame);
	SAFERELEASE(lpTexture);

	return hr;
}


STDMETHODIMP CVWFrameObject::SetDecalSize(long nMeshIndex, float fHeight, float fWidth, VARIANT_BOOL bScale)
{
	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;

	hr = D3DFrameSetDecalSize(m_pd3drmf, nMeshIndex, fHeight, fWidth, bScale);

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::GetDecalOrigin(long nMeshIndex, VARIANT* lx, VARIANT* ly)
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMTEXTURE lpTexture = NULL;

	CHECK_OUT_POINTER(lx);
	CHECK_OUT_POINTER(ly);

	CHECKANDCREATEFRAME;

	VariantClear(lx);
	VariantClear(ly);

	hr = GetVisual(IID_IDirect3DRMTexture, (void**)&lpTexture, nMeshIndex);
	if (FAILED(hr) || !lpTexture) goto EXIT_FAIL;

	lx->vt = VT_I4;
	ly->vt = VT_I4;
	lpTexture->GetDecalOrigin(&lx->lVal, &ly->lVal);

EXIT_FAIL:
	SAFERELEASE(lpTexture );

	return hr;
}

STDMETHODIMP CVWFrameObject::SetDecalOrigin(long nMeshIndex, long lx, long ly)
{
	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;

	hr = D3DFrameSetDecalOrigin(m_pd3drmf, nMeshIndex, lx, ly);

EXIT_FAIL:
	return hr;

}

HRESULT D3DFrameSetDecalOrigin(IDirect3DRMFrame *pd3drmf, long nMeshIndex, long lx, long ly)
{
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	LPDIRECT3DRMFRAME		lpChildFrame = NULL;
	LPDIRECT3DRMTEXTURE		lpTexture = NULL;
	HRESULT hr = S_OK;

	CHECK_IN_POINTER(pd3drmf);

	if (nMeshIndex == VWFRAME_ALLFRAME || nMeshIndex == VWFRAME_ALLHIERARCHY)
	{																						
		int	i, nCount;
		LPDIRECT3DRMVISUALARRAY lpVisualArray = NULL;
		if (SUCCEEDED(hr = pd3drmf->GetVisuals(&lpVisualArray) && lpVisualArray))
		{
			nCount = lpVisualArray->GetSize();
			for (i = 0; i < nCount; i++)
				D3DFrameSetDecalOrigin(pd3drmf, i, lx, ly );
		}
		SAFERELEASE(lpVisualArray);
		if (nMeshIndex == VWFRAME_ALLHIERARCHY)												
		{																					
			LPDIRECT3DRMFRAMEARRAY lpFrameArray = NULL;
			if (SUCCEEDED(hr = pd3drmf->GetChildren(&lpFrameArray)) && lpFrameArray)
			{																				
				nCount = lpFrameArray->GetSize();											
				for (i = 0; i < nCount; i++)
				{
					if (SUCCEEDED(lpFrameArray->GetElement(i, &lpChildFrame)) && lpChildFrame)
					{
						D3DFrameSetDecalOrigin(lpChildFrame, VWFRAME_ALLHIERARCHY, lx, ly);
					}
					SAFERELEASE(lpChildFrame);
				}
			}														
			SAFERELEASE(lpFrameArray);
		}				
	}
	else
	{
		if (SUCCEEDED(hr = D3DFrameGetVisual(pd3drmf, IID_IDirect3DRMTexture, (void**)&lpTexture, nMeshIndex))) 
		{
			hr = lpTexture->SetDecalOrigin(lx, ly);
		}
	}

EXIT_FAIL:
	SAFERELEASE(lpMeshBuilder);
	SAFERELEASE(lpChildFrame);
	SAFERELEASE(lpTexture);

	return hr;
}

STDMETHODIMP CVWFrameObject::CreateDecalFromText(BSTR bstrText, VARIANT bstrName)
{
	return CreateDecalFromTextExt(bstrText, 0, CComBSTR("Arial"), 24, 0, NULL, NULL, bstrName);
}

STDMETHODIMP CVWFrameObject::CreateDecalFromTextExt(BSTR bstrText, long lPlacement,
													BSTR bstrFontName, long lFontSize,
													long lLabelStyle, IVector *pvFontColor, IVector *pvShadowColor,
													VARIANT bstrName)
{
	HRESULT hr = S_OK;
	CComPtr<IDirect2DRMImage>	pTextImage;
	CComPtr<IDirect3DRMTexture>	pd3drmTextTexture;
	CComPtr<IDirectDrawSurface>	pddsTextTexture;
	CComPtr<IFont>				pFont;
	CComPtr<IVWFrame>			pvwfParent;
	long						lWidth, lHeight, lOriginX, lOriginY;
	CComVariant					varMinX, varMinY, varMinZ, varMaxX, varMaxY, varMaxZ;
	OLE_COLOR					ocFontColor, ocShadowColor;
	float						fRed, fGreen, fBlue, fWidth,
								fScaleX = 1.0F, fScaleY = 1.0F, fScaleZ = 1.0F;
	static float				fDecalScale = 0.01F;
	FONTDESC fontdesc  = {sizeof(FONTDESC), bstrFontName, {lFontSize*10000, 0},
						  lLabelStyle & BOLD ? TRUE : FALSE,
						  ANSI_CHARSET,
						  lLabelStyle & ITALIC ? TRUE : FALSE,
						  lLabelStyle & UNDERLINE ? TRUE : FALSE,
						  lLabelStyle & STRIKETHROUGH ? TRUE : FALSE };

	CHECKANDCREATEFRAME;

	if (m_pDirect2DRM) 
	{
		if (FAILED(hr = OleCreateFontIndirect(&fontdesc, IID_IFont, (LPVOID*)&pFont)))
			return hr;

		if (!pvFontColor)
			fRed = fGreen = fBlue = 1.0F;
		else
		{
			pvFontColor->get(&fRed, &fGreen, &fBlue);
			if (fRed > 1.0F || fRed < 0.0F ||
				fGreen > 1.0F || fGreen < 0.0F ||
				fBlue > 1.0F || fBlue < 0.0F)
			{
				hr = E_INVALIDARG;
				return hr;
			}
		}
		ocFontColor = (int)(fRed*255) + (((int)(fGreen*255))<<8) + (((int)(fBlue*255))<<16);
		if (pvShadowColor)
		{
			pvShadowColor->get(&fRed, &fGreen, &fBlue);
			if (fRed > 1.0F || fRed < 0.0F ||
				fGreen > 1.0F || fGreen < 0.0F ||
				fBlue > 1.0F || fBlue < 0.0F)
			{
				hr = E_INVALIDARG;
				return hr;
			}
			ocShadowColor = (int)(fRed*255) + (((int)(fGreen*255))<<8) + (((int)(fBlue*255))<<16);
		}

		if (FAILED(hr = m_pDirect2DRM->CreateImageFromTextExt(bstrText, pFont, ocFontColor, pvShadowColor == NULL ? FALSE : TRUE, ocShadowColor, &pTextImage))) 
			return hr;
	
		//if (FAILED(hr = pTextImage->ExportD3DTexture(m_pDirect3DRM, &pddsTextTexture, &pd3drmTextTexture)))
		if (FAILED(hr = pTextImage->ExportD3DTextureEx(m_pDirect3DRM, &pddsTextTexture, flagTEXTURE_STRETCHNEAREST, &pd3drmTextTexture)))
			return hr;

		if (!pd3drmTextTexture)
		{
			hr = E_FAIL;
			return hr;
		}

		m_pd3drmf->AddVisual(pd3drmTextTexture);
		pd3drmTextTexture->Changed(TRUE, FALSE);

		DDSURFACEDESC ddsd;
		memset(&ddsd, 0, sizeof(DDSURFACEDESC));
		ddsd.dwSize = sizeof(DDSURFACEDESC);
		if (FAILED(hr = pddsTextTexture->GetSurfaceDesc(&ddsd))) 
			return hr;
		lWidth = ddsd.dwWidth;
		lHeight = ddsd.dwHeight;

		if (FAILED(hr = GetBoundingBox( &varMinX, &varMinY, &varMinZ, &varMaxX, &varMaxY, &varMaxZ )))
			return hr;

		// Scale should be based upon parent frame also.
		// REVIEW: Not bothering with parent's parent etc. for now.
		GetParent(&pvwfParent);
		if (pvwfParent)
			pvwfParent->GetScaleEx(NULL, &fScaleX, &fScaleY, &fScaleZ);
		fScaleX *= m_fScaleX;
		fScaleY *= m_fScaleY;
		fScaleZ *= m_fScaleZ;

		// Need width (for left/right labels) to be max width viewed from any angle.
		fWidth = max((float)sqrt((pow(fScaleX*varMaxX.fltVal, 2) + pow(fScaleX*varMaxZ.fltVal, 2))),
					 (float)sqrt((pow(fScaleX*varMinX.fltVal, 2) + pow(fScaleX*varMinZ.fltVal, 2))));

		switch(lPlacement)
		{
		case LABEL_CENTER:
			lOriginX = lWidth/2;
			lOriginY = (lHeight + (long)(((varMaxY.fltVal-varMinY.fltVal)*fScaleY)/fDecalScale))/2;
			break;
		case LABEL_ABOVE:
			lOriginX = lWidth/2;
			lOriginY = lHeight + (long)((varMaxY.fltVal*fScaleY)/fDecalScale);
			break;
		case LABEL_BELOW:
			lOriginX = lWidth/2;
			lOriginY = (long)((varMinY.fltVal*fScaleY)/fDecalScale);
			break;
		case LABEL_RIGHT:
			lOriginX = -(long)(fWidth/fDecalScale);
			lOriginY = (lHeight + (long)(((varMaxY.fltVal-varMinY.fltVal)*fScaleY)/fDecalScale))/2;
			break;
		case LABEL_LEFT:
			lOriginX = lWidth + (long)(fWidth/fDecalScale);
			lOriginY = (lHeight + (long)(((varMaxY.fltVal-varMinY.fltVal)*fScaleY)/fDecalScale))/2;
			break;
		}
		pd3drmTextTexture->SetDecalSize((float)lWidth*fDecalScale, (float)lHeight*fDecalScale);
		pd3drmTextTexture->SetDecalOrigin(lOriginX, lOriginY);

		if (bstrName.vt == VT_BSTR)
			pd3drmTextTexture->SetName((const char *)CString(bstrName.bstrVal));
	}

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::GetDecalIndexByName(BSTR bstrDecalName, long *plIndex)
{
	return GetVisualIndexByNameAndIID(bstrDecalName, IID_IDirect3DRMTexture, plIndex);
}

STDMETHODIMP CVWFrameObject::SetMeshPerspective(long nMeshIndex, VARIANT_BOOL vbPersp)
{
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;

	hr = D3DFrameSetMeshPerspective(m_pd3drmf, nMeshIndex, vbPersp);

	//if (SUCCEEDED(hr = GetVisual(IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex))) 
	//{
	//	SAFERELEASE(lpMeshBuilder);
	//}

EXIT_FAIL:
	return hr;
}

HRESULT D3DFrameSetMeshPerspective(IDirect3DRMFrame *pd3drmf, long nMeshIndex, VARIANT_BOOL vbPersp)
{
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	LPDIRECT3DRMFRAME		lpChildFrame = NULL;
	HRESULT hr = S_OK;

	CHECK_IN_POINTER(pd3drmf);

	if (nMeshIndex == VWFRAME_ALLFRAME || nMeshIndex == VWFRAME_ALLHIERARCHY)
	{																						
		int	i, nCount;
		LPDIRECT3DRMVISUALARRAY lpVisualArray = NULL;
		if (SUCCEEDED(hr = pd3drmf->GetVisuals(&lpVisualArray) && lpVisualArray))
		{
			nCount = lpVisualArray->GetSize();
			for (i = 0; i < nCount; i++)
				D3DFrameSetMeshPerspective(pd3drmf, i, vbPersp);
		}
		SAFERELEASE(lpVisualArray);
		if (nMeshIndex == VWFRAME_ALLHIERARCHY)												
		{																					
			LPDIRECT3DRMFRAMEARRAY lpFrameArray = NULL;
			if (SUCCEEDED(hr = pd3drmf->GetChildren(&lpFrameArray)) && lpFrameArray)
			{																				
				nCount = lpFrameArray->GetSize();											
				for (i = 0; i < nCount; i++)
				{
					if (SUCCEEDED(lpFrameArray->GetElement(i, &lpChildFrame)) && lpChildFrame)
					{
						D3DFrameSetMeshPerspective(lpChildFrame, VWFRAME_ALLHIERARCHY, vbPersp);
					}
					SAFERELEASE(lpChildFrame);
				}
			}														
			SAFERELEASE(lpFrameArray);
		}				
	}
	else
	{
		if (SUCCEEDED(hr = D3DFrameGetVisual(pd3drmf, IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex))) 
			lpMeshBuilder->SetPerspective( vbPersp == VARIANT_TRUE ? TRUE : FALSE );
	}

EXIT_FAIL:
	SAFERELEASE(lpMeshBuilder);
	SAFERELEASE(lpChildFrame);

	return hr;
}

STDMETHODIMP CVWFrameObject::GetMeshPerspective(long nMeshIndex, VARIANT_BOOL *vbPersp)
{
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(vbPersp);

	CHECKANDCREATEFRAME;

	//Initialize
	*vbPersp = VARIANT_FALSE;

	if (SUCCEEDED(hr = GetVisual(IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex))) 
	{
		BOOL bPersp = lpMeshBuilder->GetPerspective();
		if (bPersp)
			*vbPersp = VARIANT_TRUE;
		else 
			*vbPersp = VARIANT_FALSE;

		SAFERELEASE(lpMeshBuilder);
	}

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::GetMeshName(long nMeshIndex, BSTR *pbstrMeshName)
{
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	HRESULT hr = S_OK;
	DWORD	dwSize = 256;
	char strbuf[256];

	CHECK_OUT_POINTER(pbstrMeshName);

	CHECKANDCREATEFRAME;

	if (SUCCEEDED(hr = GetVisual(IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex))) 
	{
		dwSize = 256;
		hr = lpMeshBuilder->GetName(&dwSize, strbuf);
		if (FAILED(hr)) goto EXIT_FAIL;

		*pbstrMeshName = CComBSTR(strbuf).Copy();
	}

EXIT_FAIL:
	SAFERELEASE(lpMeshBuilder);
	return hr;
}

STDMETHODIMP CVWFrameObject::SetMeshName(long nMeshIndex, BSTR bstrMeshName)
{
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;

	if (SUCCEEDED(hr = GetVisual(IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex))) 
	{
		hr = lpMeshBuilder->SetName(CString(bstrMeshName) );
		if (FAILED(hr)) goto EXIT_FAIL;

	}

EXIT_FAIL:
	SAFERELEASE(lpMeshBuilder);
	return hr;
}

STDMETHODIMP CVWFrameObject::GetMeshIndexByName(BSTR bstrMeshName, long *plIndex)
{
	return GetVisualIndexByNameAndIID(bstrMeshName, IID_IDirect3DRMMeshBuilder, plIndex);
}


STDMETHODIMP CVWFrameObject::SetMeshMaterial(long nMeshIndex, float fDiffuse, float fSpecular, float fPower)
{
	return E_NOTIMPL;
}

STDMETHODIMP CVWFrameObject::GetMeshMaterial(long nMeshIndex, VARIANT *fDiffuse, VARIANT *fSpecular, VARIANT *fPower)
{
	return E_NOTIMPL;
}

STDMETHODIMP CVWFrameObject::AddMeshFaceVertex(long nMeshIndex, int nFaceIndex, float fX, float fY, float fZ)
{
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	LPDIRECT3DRMFACEARRAY lpFaceArray = NULL;
	LPDIRECT3DRMFACE lpFace = NULL;
	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;

	if (SUCCEEDED(hr = GetVisual(IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex))) 
	{
		hr = lpMeshBuilder->GetFaces(&lpFaceArray);
		if (FAILED(hr) || !lpFaceArray) goto EXIT_FAIL;

		hr = lpFaceArray->GetElement(nFaceIndex, &lpFace);
		if (FAILED(hr) || !lpFace) goto EXIT_FAIL;

		hr = lpFace->AddVertex(fX, fY, fZ);
	}

EXIT_FAIL:
	SAFERELEASE(lpMeshBuilder);
	SAFERELEASE(lpFaceArray);
	SAFERELEASE(lpFace);
	return hr;
}

	
STDMETHODIMP CVWFrameObject::SetMeshFaceVertex(long nMeshIndex, int nFaceIndex, int index, float fX, float fY, float fZ)
{
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	LPDIRECT3DRMFACEARRAY lpFaceArray = NULL;
	LPDIRECT3DRMFACE lpFace = NULL;
	DWORD dwIndex;
	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;

	if (nMeshIndex != VWFRAME_ALLFRAME && nMeshIndex != VWFRAME_ALLHIERARCHY)
	{
		if (SUCCEEDED(hr = GetVisual(IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex))) 
		{
			hr = lpMeshBuilder->GetFaces(&lpFaceArray);
			if (FAILED(hr) || !lpFaceArray) goto EXIT_FAIL;

			hr = lpFaceArray->GetElement(nFaceIndex, &lpFace);
			if (FAILED(hr) || !lpFace) goto EXIT_FAIL;

			dwIndex = lpFace->GetVertexIndex (index);
			if (FAILED(hr)) goto EXIT_FAIL;

			hr = lpMeshBuilder->SetVertex(dwIndex, fX, fY, fZ);
			if (FAILED(hr)) goto EXIT_FAIL;
		}
	}
	else
		hr = E_INVALIDARG;

EXIT_FAIL:
	SAFERELEASE(lpMeshBuilder);
	SAFERELEASE(lpFaceArray);
	SAFERELEASE(lpFace);
	return hr;
}

STDMETHODIMP CVWFrameObject::GetMeshFaceVertexCount(long nMeshIndex, long nFaceIndex, long *nCount)
{
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	LPDIRECT3DRMFACEARRAY lpFaceArray = NULL;
	LPDIRECT3DRMFACE lpFace = NULL;
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(nCount);

	CHECKANDCREATEFRAME;

	*nCount = 0;

	if (SUCCEEDED(hr = GetVisual(IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex))) 
	{
		if (SUCCEEDED(hr = lpMeshBuilder->GetFaces(&lpFaceArray)) )
		{
			if (SUCCEEDED(hr = lpFaceArray->GetElement(nFaceIndex, &lpFace)) )
			{
				*nCount = lpFace->GetVertexCount();
			}
		}
	}
	
EXIT_FAIL:
	SAFERELEASE(lpFace);
	SAFERELEASE(lpFaceArray);
	SAFERELEASE(lpMeshBuilder);
	return hr;
}

STDMETHODIMP CVWFrameObject::GetMeshFaceVertex(long nMeshIndex, int nFaceIndex, int index, VARIANT *fX, VARIANT *fY, VARIANT *fZ)
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	LPDIRECT3DRMFACEARRAY lpFaceArray = NULL;
	LPDIRECT3DRMFACE lpFace = NULL;
	DWORD dwIndex;

	CHECK_OUT_POINTER(fX);
	CHECK_OUT_POINTER(fY);
	CHECK_OUT_POINTER(fZ);

	CHECKANDCREATEFRAME;

	VariantClear(fX);
	VariantClear(fY);
	VariantClear(fZ);

	fX->vt = VT_R4;
	fY->vt = VT_R4;
	fZ->vt = VT_R4;

	if (SUCCEEDED(hr = GetVisual(IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex))) 
	{
		DWORD vcount, ncount, facedatasize;
		D3DVECTOR *vertices = NULL;

		hr = lpMeshBuilder->GetFaces(&lpFaceArray);
		if (FAILED(hr) || !lpFaceArray) goto EXIT_FAIL;

		hr = lpFaceArray->GetElement(nFaceIndex, &lpFace);
		if (FAILED(hr) || !lpFace) goto EXIT_FAIL;

		dwIndex = lpFace->GetVertexIndex (index);
		if (FAILED(hr)) goto EXIT_FAIL;

		hr = lpMeshBuilder->GetVertices(&vcount, NULL, &ncount, NULL, &facedatasize, NULL);

		if (index < (int) vcount)
		{
			vertices = new D3DVECTOR[vcount];

			hr = lpMeshBuilder->GetVertices(&vcount, vertices, &ncount, NULL, &facedatasize, NULL);

			fX->fltVal = vertices[dwIndex].x;
			fY->fltVal = vertices[dwIndex].y;
			fZ->fltVal = vertices[dwIndex].z;

			delete [] vertices;
		}
		else
		{
			hr = E_INVALIDARG;
		}

	}

EXIT_FAIL:	
	SAFERELEASE(lpMeshBuilder);
	SAFERELEASE(lpFaceArray);
	SAFERELEASE(lpFace);

	return hr;
}


STDMETHODIMP CVWFrameObject::SetMeshFaceVertexColor(long nMeshIndex, int nFaceIndex, int index, float fRed, float fGreen, float fBlue)
{
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	LPDIRECT3DRMFACEARRAY lpFaceArray = NULL;
	LPDIRECT3DRMFACE lpFace = NULL;
	DWORD dwIndex;
	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;

	if (nMeshIndex != VWFRAME_ALLFRAME && nMeshIndex != VWFRAME_ALLHIERARCHY)
	{
		if (SUCCEEDED(hr = GetVisual(IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex))) 
		{
			hr = lpMeshBuilder->GetFaces(&lpFaceArray);
			if (FAILED(hr) || !lpFaceArray) goto EXIT_FAIL;

			hr = lpFaceArray->GetElement(nFaceIndex, &lpFace);
			if (FAILED(hr) || !lpFace) goto EXIT_FAIL;

			dwIndex = lpFace->GetVertexIndex (index);
			if (FAILED(hr)) goto EXIT_FAIL;

			// since we're setting a vertex color, we must color from vertices
			hr = lpMeshBuilder->SetColorSource(D3DRMCOLOR_FROMVERTEX);

			hr = lpMeshBuilder->SetVertexColorRGB(dwIndex, fRed, fGreen, fBlue);
			SAFERELEASE(lpMeshBuilder);
		}
	}
	else
		hr = E_INVALIDARG;

EXIT_FAIL:
	SAFERELEASE(lpMeshBuilder);
	SAFERELEASE(lpFaceArray);
	SAFERELEASE(lpFace);

	return hr;
}

STDMETHODIMP CVWFrameObject::GetMeshFaceVertexColor(long nMeshIndex, int nFaceIndex, int index, VARIANT *pfRed, VARIANT *pfGreen, VARIANT *pfBlue)
{
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	D3DCOLOR rgbColor;
	LPDIRECT3DRMFACEARRAY lpFaceArray = NULL;
	LPDIRECT3DRMFACE lpFace = NULL;
	DWORD dwIndex;
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(pfRed);
	CHECK_OUT_POINTER(pfGreen);
	CHECK_OUT_POINTER(pfBlue);

	CHECKANDCREATEFRAME;

	VariantClear(pfRed);
	VariantClear(pfGreen);
	VariantClear(pfBlue);

	rgbColor = D3DRMCreateColorRGB(0.0f, 0.0f, 0.0f);

	if (SUCCEEDED(hr = GetVisual(IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex))) 
	{
		hr = lpMeshBuilder->GetFaces(&lpFaceArray);
		if (FAILED(hr) || !lpFaceArray) goto EXIT_FAIL;

		hr = lpFaceArray->GetElement(nFaceIndex, &lpFace);
		if (FAILED(hr) || !lpFace) goto EXIT_FAIL;

		dwIndex = lpFace->GetVertexIndex (index);
		if (FAILED(hr)) goto EXIT_FAIL;

		rgbColor = lpMeshBuilder->GetVertexColor(dwIndex);
	}
	
	pfRed->vt = VT_R4;
	pfGreen->vt = VT_R4;
	pfBlue->vt = VT_R4;

	pfRed->fltVal = D3DRMColorGetRed(rgbColor);
	pfGreen->fltVal = D3DRMColorGetGreen(rgbColor);
	pfBlue->fltVal = D3DRMColorGetBlue(rgbColor);

EXIT_FAIL:
	SAFERELEASE(lpMeshBuilder);
	SAFERELEASE(lpFaceArray);
	SAFERELEASE(lpFace);
	return hr;
}

STDMETHODIMP CVWFrameObject::SetMeshFaceColor(long nMeshIndex, int index, float fRed, float fGreen, float fBlue)
{
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	LPDIRECT3DRMFACEARRAY lpFaceArray = NULL;
	LPDIRECT3DRMFACE lpFace = NULL;
	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;

	if (nMeshIndex != VWFRAME_ALLFRAME && nMeshIndex != VWFRAME_ALLHIERARCHY)
	{
		if (SUCCEEDED(hr = GetVisual(IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex))) 
		{
			// since we're setting a vertex color, we must color from vertices
			hr = lpMeshBuilder->SetColorSource(D3DRMCOLOR_FROMFACE);

			if (SUCCEEDED(hr = GetVisual(IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex))) 
			{
				if (SUCCEEDED(hr = lpMeshBuilder->GetFaces(&lpFaceArray)) )
				{
					if (SUCCEEDED(hr = lpFaceArray->GetElement(index, &lpFace)) )
					{
						hr = lpFace->SetColor(D3DRMCreateColorRGB(fRed, fGreen, fBlue) );
					}
				}
			}
		}
	}
	else
		hr = E_INVALIDARG;

EXIT_FAIL:
	SAFERELEASE(lpFace);
	SAFERELEASE(lpFaceArray);
	SAFERELEASE(lpMeshBuilder);
	
	return hr;
}

STDMETHODIMP CVWFrameObject::GetMeshFaceColor(long nMeshIndex, int index, VARIANT *pfRed, VARIANT *pfGreen, VARIANT *pfBlue)
{
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	LPDIRECT3DRMFACEARRAY lpFaceArray = NULL;
	LPDIRECT3DRMFACE lpFace = NULL;
	D3DCOLOR rgbColor;
	HRESULT hr = S_OK;

	CHECK_OUT_POINTER(pfRed);
	CHECK_OUT_POINTER(pfGreen);
	CHECK_OUT_POINTER(pfGreen);

	CHECKANDCREATEFRAME;

	VariantClear(pfRed);
	VariantClear(pfGreen);
	VariantClear(pfGreen);

	if (SUCCEEDED(hr = GetVisual(IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex))) 
	{
		// since we're setting a vertex color, we must color from vertices
		hr = lpMeshBuilder->SetColorSource(D3DRMCOLOR_FROMFACE);

		if (SUCCEEDED(hr = GetVisual(IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex))) 
		{
			if (SUCCEEDED(hr = lpMeshBuilder->GetFaces(&lpFaceArray)) )
			{
				if (SUCCEEDED(hr = lpFaceArray->GetElement(index, &lpFace)) )
				{
					rgbColor = lpFace->GetColor();
				}
			}
		}
	}

	pfRed->vt = VT_R4;
	pfGreen->vt = VT_R4;
	pfBlue->vt = VT_R4;

	pfRed->fltVal = D3DRMColorGetRed(rgbColor);
	pfGreen->fltVal = D3DRMColorGetGreen(rgbColor);
	pfBlue->fltVal = D3DRMColorGetBlue(rgbColor);

EXIT_FAIL:
	SAFERELEASE(lpFace);
	SAFERELEASE(lpFaceArray);
	SAFERELEASE(lpMeshBuilder);

	return hr;
}

STDMETHODIMP CVWFrameObject::SetMeshFaceUV(long nMeshIndex, int nFaceIndex, int index, float fU, float fV)
{
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	LPDIRECT3DRMFACEARRAY lpFaceArray = NULL;
	LPDIRECT3DRMFACE lpFace = NULL;
	DWORD dwIndex;
	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;

	if (nMeshIndex != VWFRAME_ALLFRAME && nMeshIndex != VWFRAME_ALLHIERARCHY)
	{
		if (SUCCEEDED(hr = GetVisual(IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex)))
		{
			hr = lpMeshBuilder->GetFaces(&lpFaceArray);
			if (FAILED(hr) || !lpFaceArray) goto EXIT_FAIL;

			hr = lpFaceArray->GetElement(nFaceIndex, &lpFace);
			if (FAILED(hr) || !lpFace) goto EXIT_FAIL;

			dwIndex = lpFace->GetVertexIndex (index);
			if (FAILED(hr)) goto EXIT_FAIL;

			hr = lpMeshBuilder->SetTextureCoordinates(dwIndex , fU, fV);
			SAFERELEASE(lpMeshBuilder);
		}
	}
	else
		hr = E_INVALIDARG;

EXIT_FAIL:
	SAFERELEASE(lpFaceArray);
	SAFERELEASE(lpFace);
	SAFERELEASE(lpMeshBuilder);
	return hr;
}

STDMETHODIMP CVWFrameObject::GetMeshFaceUV(long nMeshIndex, int nFaceIndex, int index, VARIANT *pfU, VARIANT *pfV)
{
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	LPDIRECT3DRMFACEARRAY lpFaceArray = NULL;
	LPDIRECT3DRMFACE lpFace = NULL;
	DWORD dwIndex;
	HRESULT hr = S_OK;
	D3DVALUE fU, fV;

	CHECK_OUT_POINTER(pfU);
	CHECK_OUT_POINTER(pfV);

	CHECKANDCREATEFRAME;

	VariantClear(pfU);
	VariantClear(pfV);

	if (SUCCEEDED(hr = GetVisual(IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex))) 
	{
		hr = lpMeshBuilder->GetFaces(&lpFaceArray);
		if (FAILED(hr) || !lpFaceArray) goto EXIT_FAIL;

		hr = lpFaceArray->GetElement(nFaceIndex, &lpFace);
		if (FAILED(hr) || !lpFace) goto EXIT_FAIL;

		dwIndex = lpFace->GetVertexIndex (index);
		if (FAILED(hr)) goto EXIT_FAIL;

		hr = lpMeshBuilder->GetTextureCoordinates(dwIndex , &fU, &fV);
		SAFERELEASE(lpMeshBuilder);
	}
	
	pfU->vt = VT_R4;
	pfV->vt = VT_R4;

	pfU->fltVal = fU;
	pfV->fltVal = fV;

EXIT_FAIL:
	SAFERELEASE(lpFaceArray);
	SAFERELEASE(lpFace);
	SAFERELEASE(lpMeshBuilder);

	return hr;
}

STDMETHODIMP CVWFrameObject::WrapMesh(long nMeshIndex, int type, float scaleu, float scalev)
{
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	LPDIRECT3DRMWRAP lpD3DRMWrap = NULL;
	D3DRMWRAPTYPE wrapType;
	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;

	switch(type) 
	{
	case 0:
		wrapType = D3DRMWRAP_FLAT;
		break;
	case 1:
		wrapType = D3DRMWRAP_CYLINDER;
		break;
	case 2:
		wrapType = D3DRMWRAP_SPHERE;
		break;
	case 3:
		wrapType = D3DRMWRAP_CHROME;
		break;
	default:
		wrapType = D3DRMWRAP_SPHERE;
		break;
	}
	if (SUCCEEDED(hr = GetVisual(IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex))) 
	{
		m_pDirect3DRM->CreateWrap(wrapType, m_pd3drmf,
			0.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0, 
			0.0, 0.0, scaleu, scalev, &lpD3DRMWrap);
		lpD3DRMWrap->Apply(lpMeshBuilder);
		SAFERELEASE(lpD3DRMWrap);
		SAFERELEASE(lpMeshBuilder);
	}

EXIT_FAIL:
	return hr;

}

// return in pFrame the d3dframe that has bstrName
STDMETHODIMP CVWFrameObject::FindFrame(BSTR bstrName, LPDIRECT3DRMFRAME pStartFrame, LPDIRECT3DRMFRAME *ppFrame)
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMFRAMEARRAY lpFrames = NULL;
	LPDIRECT3DRMFRAME pFrameCurrent = NULL;
	int cFrames;
	DWORD dwSize = 256;
	char lpName[256];
	CString csTargetName(bstrName);
	CString csName;
	int i;

	CHECK_IN_POINTER(pStartFrame);
	CHECK_OUT_POINTER(ppFrame);

	CHECKANDCREATEFRAME;

	if (pStartFrame) 
	{
		*ppFrame = NULL;

		// make sure that the initial frame is not the right name 
		pStartFrame->GetName(&dwSize, lpName);
		csName = CString(lpName);
		if (csTargetName == csName)
		{
			*ppFrame = pStartFrame;
			SAFEADDREF(pStartFrame);
			return hr;
		}

		// now need to loop through the children of pStartFrame
		pStartFrame->GetChildren(&lpFrames);
		cFrames = lpFrames->GetSize();
		for (i = 0; i< cFrames; i++) 
		{
			dwSize = 256;
			lpFrames->GetElement(i, &pFrameCurrent);
			pFrameCurrent->GetName(&dwSize, lpName);
			csName = CString(lpName);

			if (csTargetName == csName)
			{
				*ppFrame = pFrameCurrent;
				SAFERELEASE(lpFrames);
				return hr;
			}
			FindFrame(bstrName, pFrameCurrent, ppFrame);
			if (*ppFrame != NULL) 
			{
				SAFERELEASE(pFrameCurrent);
				SAFERELEASE(lpFrames);
				return hr;
			}
			SAFERELEASE(pFrameCurrent);
		}
		SAFERELEASE(lpFrames);
	}	

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::RecurseQuality(LPDIRECT3DRMFRAME lpFrame, REFIID refid, D3DRMRENDERQUALITY quality)
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMFRAMEARRAY lpFrameArray = NULL;
	LPDIRECT3DRMVISUALARRAY lpVisuals = NULL;
	LPDIRECT3DRMVISUAL lpD3DRMVisual = NULL;
	LPDIRECT3DRMMESHBUILDER lpMesh = NULL;
	LPDIRECT3DRMFRAME lpChildFrame = NULL;
#ifdef IDirect3D2_CreateDevice 
	IDirect3DRMProgressiveMesh *lpProgMesh = NULL;
#endif
	int cVisual, cChild;

	CHECK_IN_POINTER(lpFrame);

	if (lpFrame != NULL)
	{
		if (FAILED(hr = lpFrame->GetVisuals(&lpVisuals))) goto EXIT_FAIL;
		for (cVisual = 0; cVisual < (int)(lpVisuals->GetSize()); cVisual++) 
		{
			if (SUCCEEDED(hr = lpVisuals->GetElement(cVisual, &lpD3DRMVisual))) 
			{
#ifdef IDirect3D2_CreateDevice 
				if (refid == IID_IDirect3DRMProgressiveMesh)
				{
					if (SUCCEEDED(hr = lpD3DRMVisual->QueryInterface(IID_IDirect3DRMProgressiveMesh, (void**) &lpProgMesh)))
					{
						lpProgMesh->SetQuality(quality);
						SAFERELEASE(lpProgMesh);
					}
				}
#endif
				if (refid == IID_IDirect3DRMMeshBuilder)
				{
					if (SUCCEEDED(hr = lpD3DRMVisual->QueryInterface(IID_IDirect3DRMMeshBuilder, (void**) &lpMesh)))
					{
						lpMesh->SetQuality(quality);
						SAFERELEASE(lpMesh);
					}
				}

				SAFERELEASE(lpD3DRMVisual);
			}
		}
		SAFERELEASE(lpVisuals);

		// now need to recurse down a level if necessary
		if (FAILED(hr = lpFrame->GetChildren(&lpFrameArray))) goto EXIT_FAIL;
		for (cChild = 0; cChild < (int)(lpFrameArray->GetSize()); cChild++) 
		{
			if (SUCCEEDED(hr = lpFrameArray->GetElement(cChild, &lpChildFrame))) 
			{
				RecurseQuality(lpChildFrame, refid, quality);
				SAFERELEASE(lpChildFrame);
			}
		}
		SAFERELEASE(lpFrameArray);
	}
	return hr;

EXIT_FAIL:

#ifdef IDirect3D2_CreateDevice 
	SAFERELEASE(lpProgMesh);
#endif
	SAFERELEASE(lpFrameArray);
	SAFERELEASE(lpVisuals);
	return hr;
}

// note: sound will not be updated!
STDMETHODIMP CVWFrameObject::SetRotationRate(IVWFrame *pvwfRef,  float rvX,  float rvY,  float rvZ,  float rvTheta)
{
	IDirect3DRMFrame	*pd3drmfRef = NULL;
	HRESULT hr = S_OK;

	// pvwfRef can be NULL
//	CHECK_IN_POINTER(pvwfRef);

	CHECKANDCREATEFRAME;

	if (pvwfRef)
	{
		if(FAILED(hr = pvwfRef->get_Frame3D(&pd3drmfRef)))
			goto EXIT_FAIL;
	}
	
	if (m_pd3drmf)
	{
		// Simple 3D pos relative to 3D frame.
		if (FAILED(hr = m_pd3drmf->SetRotation(pd3drmfRef, rvX, rvY, rvZ, rvTheta)))
			goto EXIT_FAIL;
	}

EXIT_FAIL:
	SAFERELEASE(pd3drmfRef);
	return hr;
}

STDMETHODIMP CVWFrameObject::GetRotationRate(IVWFrame *pvwfRef, VARIANT *prvX, VARIANT *prvY, VARIANT *prvZ, VARIANT *prvTheta)
{
	HRESULT hr = S_OK;

	// pvwfRef can be NULL
//	CHECK_IN_POINTER(pvwfRef);
	CHECK_OUT_POINTER(prvX);
	CHECK_OUT_POINTER(prvY);
	CHECK_OUT_POINTER(prvZ);
	CHECK_OUT_POINTER(prvTheta);

	CHECKANDCREATEFRAME;

	prvX->vt = prvY->vt = prvZ->vt = prvTheta->vt = VT_R4;
	hr = GetRotationRateEx(pvwfRef, &prvX->fltVal, &prvY->fltVal, &prvZ->fltVal, &prvTheta->fltVal);

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::GetRotationRateEx(IVWFrame *pvwfRef,  float* prvX,  float* prvY,  float* prvZ,  float* prvTheta)
{
	IDirect3DRMFrame	*pd3drmfRef = NULL;
	D3DVECTOR vec;
	HRESULT hr = S_OK;

	// pvwfRef can be NULL
//	CHECK_IN_POINTER(pvwfRef);
	CHECK_OUT_POINTER(prvX);
	CHECK_OUT_POINTER(prvY);
	CHECK_OUT_POINTER(prvZ);
	CHECK_OUT_POINTER(prvTheta);

	CHECKANDCREATEFRAME;

	*prvX = 0.0F;
	*prvY = 0.0F;
	*prvZ = 0.0F;
	*prvTheta = 0.0F;

	if (pvwfRef)
	{
		if(FAILED(hr = pvwfRef->get_Frame3D(&pd3drmfRef)))
			goto EXIT_FAIL;
	}
	
	if (m_pd3drmf)
	{
		// Simple 3D pos relative to 3D frame.
		if (FAILED(hr = m_pd3drmf->GetRotation(pd3drmfRef, &vec, prvTheta)))
			goto EXIT_FAIL;

		*prvX = vec.x;
		*prvY = vec.y;
		*prvZ = vec.z;
	}

EXIT_FAIL:
	SAFERELEASE(pd3drmfRef);
	return hr;
}


STDMETHODIMP CVWFrameObject::SetVelocity(IVWFrame *pvwfRef,  float vX,  float vY,  float vZ)
{
	IDirect3DRMFrame	*pd3drmfRef = NULL;
	HRESULT hr = S_OK;

	// pvwfRef can be NULL
//	CHECK_IN_POINTER(pvwfRef);

	CHECKANDCREATEFRAME;

	if (pvwfRef)
	{
		if(FAILED(hr = pvwfRef->get_Frame3D(&pd3drmfRef)))
			goto EXIT_FAIL;
	}
	
	if (m_pd3drmf)
	{
		// Simple 3D pos relative to 3D frame.
		if (FAILED(hr = m_pd3drmf->SetVelocity(pd3drmfRef, vX, vY, vZ, FALSE)))
			goto EXIT_FAIL;
	}

EXIT_FAIL:
	SAFERELEASE(pd3drmfRef);
	return hr;
}

STDMETHODIMP CVWFrameObject::GetVelocity(IVWFrame *pvwfRef, VARIANT *pvX, VARIANT *pvY, VARIANT *pvZ)
{
	HRESULT hr = S_OK;
	CHECKANDCREATEFRAME;

	// pvwfRef can be NULL
//	CHECK_IN_POINTER(pvwfRef);
	CHECK_OUT_POINTER(pvX);
	CHECK_OUT_POINTER(pvY);
	CHECK_OUT_POINTER(pvZ);

	pvX->vt = pvY->vt = pvZ->vt = VT_R4;
	hr = GetVelocityEx(pvwfRef, &pvX->fltVal, &pvY->fltVal, &pvZ->fltVal);

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::GetVelocityEx(IVWFrame *pvwfRef,  float* pvX,  float* pvY,  float* pvZ)
{
	IDirect3DRMFrame	*pd3drmfRef = NULL;
	HRESULT hr = S_OK;
	D3DVECTOR vec;

	// pvwfRef can be NULL
//	CHECK_IN_POINTER(pvwfRef);
	CHECK_OUT_POINTER(pvX);
	CHECK_OUT_POINTER(pvY);
	CHECK_OUT_POINTER(pvZ);

	CHECKANDCREATEFRAME;

	*pvX = 0.0F;
	*pvY = 0.0F;
	*pvZ = 0.0F;

	if (pvwfRef)
	{
		if(FAILED(hr = pvwfRef->get_Frame3D(&pd3drmfRef)))
			goto EXIT_FAIL;
	}
	
	if (m_pd3drmf)
	{
		// Simple 3D pos relative to 3D frame.
		if (FAILED(hr = m_pd3drmf->GetVelocity(pd3drmfRef, &vec, FALSE)))
			goto EXIT_FAIL;

		*pvX = vec.x;
		*pvY = vec.y;
		*pvZ = vec.z;
	}

EXIT_FAIL:
	SAFERELEASE(pd3drmfRef);
	return hr;
}

		
STDMETHODIMP CVWFrameObject::CreateLight(VARIANT bstrName)
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMLIGHT lpLight = NULL;

	CHECKANDCREATEFRAME;

	if (FAILED(hr = m_pDirect3DRM->CreateLightRGB(D3DRMLIGHT_POINT, 1.0f, 1.0f, 1.0f,  &lpLight))) 
		goto EXIT_FAIL;

	if (FAILED(hr = m_pd3drmf->AddLight(lpLight))) 
		goto EXIT_FAIL;

	if (bstrName.vt == VT_BSTR)
		lpLight->SetName((const char *)(CString(bstrName.bstrVal)));

EXIT_FAIL:
	SAFERELEASE(lpLight);
	return hr;
}

STDMETHODIMP CVWFrameObject::GetLightName(long nIndex, BSTR* bstrName)
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMLIGHT lpLight = NULL;
	DWORD	dwSize = 256;
	char strbuf[256];

	CHECK_OUT_POINTER(bstrName);

	CHECKANDCREATEFRAME;

	if (nIndex < 0)
	{
		hr = E_INVALIDARG;
		goto EXIT_FAIL;
	}

	hr = GetLight(m_pd3drmf, nIndex, &lpLight);
	if (FAILED(hr)) goto EXIT_FAIL;

	dwSize = 256;
	hr = lpLight->GetName(&dwSize, strbuf);
	if (FAILED(hr)) goto EXIT_FAIL;

	*bstrName = CComBSTR(strbuf).Copy();

EXIT_FAIL:
	SAFERELEASE(lpLight);
	return hr;
}

STDMETHODIMP CVWFrameObject::GetLightIndexByName(BSTR bstrName, long* nIndex)
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMLIGHTARRAY lpLightArray = NULL;
	LPDIRECT3DRMLIGHT lpLight = NULL;
	long nLights;
	DWORD dwSize = 256;
	char szTmp[256];
	int i;

	CHECK_OUT_POINTER(nIndex);

	CHECKANDCREATEFRAME;

	*nIndex = -1;

	if (m_pd3drmf) 
	{
		hr = m_pd3drmf->GetLights(&lpLightArray);
		nLights = lpLightArray->GetSize();

		for (i = 0; i < nLights; i++)
		{
			hr = lpLightArray->GetElement(i, &lpLight);
			if (FAILED(hr) || !lpLight) goto EXIT_FAIL;

			dwSize = 256;
			hr = lpLight->GetName(&dwSize, szTmp);
			if (FAILED(hr)) goto EXIT_FAIL;

			SAFERELEASE(lpLight);

			if ( CString(bstrName).Compare(szTmp) == 0)
			{
				*nIndex = i;
				break;
			}
		}

		if (*nIndex == -1)
		{
			hr = E_INVALIDARG;
			VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CVWFrameObject::GetLightIndexByName: can't find named light.\n");
		}

	} 
	else
		hr = E_FAIL;


EXIT_FAIL:
	SAFERELEASE(lpLight);
	SAFERELEASE(lpLightArray);
	return hr;
}

HRESULT GetLight(LPDIRECT3DRMFRAME pd3drmf, long nIndex, LPDIRECT3DRMLIGHT* ppLight)
{
	HRESULT hr = S_OK;
	int nLights;
	LPDIRECT3DRMLIGHTARRAY lpLightArray = NULL;

	CHECK_IN_POINTER(pd3drmf);
	CHECK_OUT_POINTER(ppLight);

	if (nIndex < 0)
	{
		hr = E_INVALIDARG;
		goto EXIT_FAIL;
	}

	hr = pd3drmf->GetLights(&lpLightArray);
	nLights = lpLightArray->GetSize();

	if (nIndex >= nLights)
	{
		OutputDebugString("CVWFrameObject::GetLightName: index out of range.\n");
		hr = E_INVALIDARG;
		goto EXIT_FAIL;
	}

	hr = lpLightArray->GetElement(nIndex, ppLight);
	if (FAILED(hr)) goto EXIT_FAIL;

EXIT_FAIL:
	SAFERELEASE(lpLightArray);
	return hr;
}

STDMETHODIMP CVWFrameObject::SetLightName(long nIndex, BSTR bstrName)
{
	HRESULT hr = S_OK;
	int nLights;
	LPDIRECT3DRMLIGHTARRAY lpLightArray = NULL;
	LPDIRECT3DRMLIGHT lpLight = NULL;

	CHECKANDCREATEFRAME;

	if (nIndex < 0)
	{
		hr = E_INVALIDARG;
		goto EXIT_FAIL;
	}

	hr = m_pd3drmf->GetLights(&lpLightArray);
	nLights = lpLightArray->GetSize();

	if (nIndex >= nLights)
	{
		VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CVWFrameObject::GetLightName: index out of range.\n");
		hr = E_INVALIDARG;
		goto EXIT_FAIL;
	}

	hr = lpLightArray->GetElement(nIndex, &lpLight);
	if (FAILED(hr)) goto EXIT_FAIL;

	hr = lpLight->SetName(CString(bstrName));

EXIT_FAIL:
	SAFERELEASE(lpLight);
	SAFERELEASE(lpLightArray);
	return hr;
}

STDMETHODIMP CVWFrameObject::DeleteLight( long nIndex)
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMLIGHT lpLight = NULL;
	
	CHECKANDCREATEFRAME;

	if (SUCCEEDED(hr = GetLight(m_pd3drmf, nIndex, &lpLight))) 
	{
		m_pd3drmf->DeleteLight(lpLight);
	}

EXIT_FAIL:
	SAFERELEASE(lpLight);
	return hr;
}


STDMETHODIMP CVWFrameObject::SetLightColor( long nIndex,  float fRed,  float fGreen,  float fBlue)
{
	HRESULT hr = S_OK;
	
	CHECKANDCREATEFRAME;

	D3DFrameSetLightColor(m_pd3drmf, nIndex, fRed, fGreen, fBlue);

EXIT_FAIL:
	return hr;
}

HRESULT D3DFrameSetLightColor(IDirect3DRMFrame *pd3drmf, long nMeshIndex, float fRed, float fGreen, float fBlue)
{
	LPDIRECT3DRMLIGHT lpLight = NULL;
	LPDIRECT3DRMFRAME lpChildFrame = NULL;
	HRESULT hr = S_OK;

	CHECK_IN_POINTER(pd3drmf);

	if (nMeshIndex == VWFRAME_ALLFRAME || nMeshIndex == VWFRAME_ALLHIERARCHY)
	{																						
		LPDIRECT3DRMLIGHTARRAY lpLightArray = NULL;
		int i, nCount;

		if (SUCCEEDED(hr = pd3drmf->GetLights(&lpLightArray) ))
		{
			nCount = lpLightArray->GetSize();
			for (i = 0; i < nCount; i++)
				D3DFrameSetLightColor(pd3drmf, i, fRed, fGreen, fBlue );
		}
		
		SAFERELEASE(lpLightArray);															
		if (nMeshIndex == VWFRAME_ALLHIERARCHY)												
		{																					
			LPDIRECT3DRMFRAMEARRAY lpFrameArray = NULL;										
			if (SUCCEEDED(hr = pd3drmf->GetChildren(&lpFrameArray) ))						
			{																				
				nCount = lpFrameArray->GetSize();											
				for (i = 0; i < nCount; i++)
				{
					if (SUCCEEDED(lpFrameArray->GetElement(i, &lpChildFrame)) && lpChildFrame)
					{
						D3DFrameSetLightColor(lpChildFrame, VWFRAME_ALLHIERARCHY, fRed, fGreen, fBlue );
					}
					SAFERELEASE(lpChildFrame);
				}
			}														
			SAFERELEASE(lpFrameArray);
		}				
	}
	else
	{
		if (SUCCEEDED(hr = GetLight(pd3drmf, nMeshIndex, &lpLight))) 
		{
			lpLight->SetColorRGB(fRed, fGreen, fBlue);
		}
	}

EXIT_FAIL:
	SAFERELEASE(lpLight);
	SAFERELEASE(lpChildFrame);

	return hr;
}


STDMETHODIMP CVWFrameObject::GetLightColor( long nIndex,  VARIANT* fRed,  VARIANT* fGreen,  VARIANT* fBlue)
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMLIGHT lpLight = NULL;

	CHECK_OUT_POINTER(fRed);
	CHECK_OUT_POINTER(fGreen);
	CHECK_OUT_POINTER(fBlue);

	CHECKANDCREATEFRAME;

	VariantClear(fRed);
	VariantClear(fGreen);
	VariantClear(fBlue);

	if (SUCCEEDED(hr = GetLight(m_pd3drmf, nIndex, &lpLight))) 
	{
		D3DCOLOR rgb; 

		fRed->vt = VT_R4;
		fGreen->vt = VT_R4;
		fBlue->vt = VT_R4;

		rgb = lpLight->GetColor();
		fRed->fltVal = D3DRMColorGetRed(rgb);
		fGreen->fltVal = D3DRMColorGetGreen(rgb);
		fBlue->fltVal = D3DRMColorGetBlue(rgb);
	}

EXIT_FAIL:
	SAFERELEASE(lpLight);
	return hr;
}

STDMETHODIMP CVWFrameObject::SetLightAttenuation( long nIndex,  float fConst,  float fLinear,  float fQuadratic)
{
	HRESULT hr = S_OK;
	
	CHECKANDCREATEFRAME;

	D3DFrameSetLightAttenuation(m_pd3drmf, nIndex, fConst, fLinear, fQuadratic);

EXIT_FAIL:
	return hr;
}

HRESULT D3DFrameSetLightAttenuation(IDirect3DRMFrame *pd3drmf, long nMeshIndex, float fConst, float fLinear, float fQuadratic)
{
	LPDIRECT3DRMLIGHT lpLight = NULL;
	LPDIRECT3DRMFRAME lpChildFrame = NULL;
	HRESULT hr = S_OK;

	CHECK_IN_POINTER(pd3drmf);

	if (nMeshIndex == VWFRAME_ALLFRAME || nMeshIndex == VWFRAME_ALLHIERARCHY)
	{																						
		LPDIRECT3DRMLIGHTARRAY lpLightArray = NULL;
		int i, nCount;

		if (SUCCEEDED(hr = pd3drmf->GetLights(&lpLightArray) ))
		{
			nCount = lpLightArray->GetSize();
			for (i = 0; i < nCount; i++)
				D3DFrameSetLightAttenuation(pd3drmf, i, fConst, fLinear, fQuadratic );
		}
		
		SAFERELEASE(lpLightArray);															
		if (nMeshIndex == VWFRAME_ALLHIERARCHY)												
		{																					
			LPDIRECT3DRMFRAMEARRAY lpFrameArray = NULL;										
			if (SUCCEEDED(hr = pd3drmf->GetChildren(&lpFrameArray) ))						
			{																				
				nCount = lpFrameArray->GetSize();											
				for (i = 0; i < nCount; i++)
				{
					if (SUCCEEDED(lpFrameArray->GetElement(i, &lpChildFrame)) && lpChildFrame)
					{
						D3DFrameSetLightAttenuation(lpChildFrame, VWFRAME_ALLHIERARCHY, fConst, fLinear, fQuadratic );
					}
					SAFERELEASE(lpChildFrame);
				}
			}														
			SAFERELEASE(lpFrameArray);
		}				
	}
	else
	{
		if (SUCCEEDED(hr = GetLight(pd3drmf, nMeshIndex, &lpLight))) 
		{
			lpLight->SetConstantAttenuation(fConst);
			lpLight->SetLinearAttenuation(fLinear);
			lpLight->SetQuadraticAttenuation(fQuadratic);
		}
	}

EXIT_FAIL:
	SAFERELEASE(lpLight);
	SAFERELEASE(lpChildFrame);

	return hr;
}


STDMETHODIMP CVWFrameObject::GetLightAttenuation( long nIndex,  VARIANT* fConst,  VARIANT* fLinear,  VARIANT* fQuadratic)
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMLIGHT lpLight = NULL;

	CHECK_OUT_POINTER(fConst);
	CHECK_OUT_POINTER(fLinear);
	CHECK_OUT_POINTER(fQuadratic);

	CHECKANDCREATEFRAME;

	VariantClear(fConst);
	VariantClear(fLinear);
	VariantClear(fQuadratic);

	if (SUCCEEDED(hr = GetLight(m_pd3drmf, nIndex, &lpLight))) 
	{
		fConst->vt = VT_R4;
		fLinear->vt = VT_R4;
		fQuadratic->vt = VT_R4;

		fConst->fltVal = lpLight->GetConstantAttenuation( );
		fLinear->fltVal = lpLight->GetLinearAttenuation();
		fQuadratic->fltVal = lpLight->GetQuadraticAttenuation();
	}

EXIT_FAIL:
	SAFERELEASE(lpLight);
	return hr;
}

STDMETHODIMP CVWFrameObject::SetLightType( long nIndex,  int iType)
{
	HRESULT hr = S_OK;
	D3DRMLIGHTTYPE d3drmtType = D3DRMLIGHT_POINT;
	
	CHECKANDCREATEFRAME;

	switch(iType) 
	{
	case 0: 
		d3drmtType = D3DRMLIGHT_AMBIENT;
		break;
	case 1: 
		d3drmtType = D3DRMLIGHT_POINT;
		break;
	case 2: 
		d3drmtType = D3DRMLIGHT_SPOT;
		break;
	case 3: 
		d3drmtType = D3DRMLIGHT_DIRECTIONAL;
		break;
	case 4: 
		d3drmtType = D3DRMLIGHT_PARALLELPOINT;
		break;
	}

	D3DFrameSetLightType(m_pd3drmf, nIndex, d3drmtType);

EXIT_FAIL:
	return hr;
}

HRESULT D3DFrameSetLightType(IDirect3DRMFrame *pd3drmf, long nMeshIndex, D3DRMLIGHTTYPE d3drmtType)
{
	LPDIRECT3DRMLIGHT lpLight = NULL;
	LPDIRECT3DRMFRAME lpChildFrame = NULL;
	HRESULT hr = S_OK;

	CHECK_IN_POINTER(pd3drmf);

	if (nMeshIndex == VWFRAME_ALLFRAME || nMeshIndex == VWFRAME_ALLHIERARCHY)
	{																						
		LPDIRECT3DRMLIGHTARRAY lpLightArray = NULL;
		int i, nCount;

		if (SUCCEEDED(hr = pd3drmf->GetLights(&lpLightArray) ))
		{
			nCount = lpLightArray->GetSize();
			for (i = 0; i < nCount; i++)
				D3DFrameSetLightType(pd3drmf, i, d3drmtType );
		}
		
		SAFERELEASE(lpLightArray);															
		if (nMeshIndex == VWFRAME_ALLHIERARCHY)												
		{																					
			LPDIRECT3DRMFRAMEARRAY lpFrameArray = NULL;										
			if (SUCCEEDED(hr = pd3drmf->GetChildren(&lpFrameArray) ))						
			{																				
				nCount = lpFrameArray->GetSize();											
				for (i = 0; i < nCount; i++)
				{
					if (SUCCEEDED(lpFrameArray->GetElement(i, &lpChildFrame)) && lpChildFrame)
					{
						D3DFrameSetLightType(lpChildFrame, VWFRAME_ALLHIERARCHY, d3drmtType );
					}
					SAFERELEASE(lpChildFrame);
				}
			}														
			SAFERELEASE(lpFrameArray);
		}				
	}
	else
	{
		if (SUCCEEDED(hr = GetLight(pd3drmf, nMeshIndex, &lpLight))) 
			lpLight->SetType(d3drmtType);
	}

EXIT_FAIL:
	SAFERELEASE(lpLight);
	SAFERELEASE(lpChildFrame);

	return hr;
}


STDMETHODIMP CVWFrameObject::GetLightType( long nIndex,  int* iType)
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMLIGHT lpLight = NULL;
	D3DRMLIGHTTYPE d3drmtType = D3DRMLIGHT_POINT;
	
	CHECK_OUT_POINTER(iType);

	CHECKANDCREATEFRAME;

	if (SUCCEEDED(hr = GetLight(m_pd3drmf, nIndex, &lpLight))) 
	{
		d3drmtType = lpLight->GetType();

		switch(d3drmtType) 
		{
		case D3DRMLIGHT_AMBIENT: 
			*iType = 0;
			break;
		case D3DRMLIGHT_POINT: 
			*iType = 1;
			break;
		case D3DRMLIGHT_SPOT: 
			*iType = 2;
			break;
		case D3DRMLIGHT_DIRECTIONAL: 
			*iType = 3;
			break;
		case D3DRMLIGHT_PARALLELPOINT: 
			*iType = 4;
			break;
		default:
			*iType = -1;
			break;
		}
	}

EXIT_FAIL:
	SAFERELEASE(lpLight);
	return hr;
}


STDMETHODIMP CVWFrameObject::SetLightRange( long nIndex,  float fRange)
{
	HRESULT hr = S_OK;
	
	CHECKANDCREATEFRAME;

	D3DFrameSetLightRange(m_pd3drmf, nIndex, fRange);

EXIT_FAIL:
	return hr;
}

HRESULT D3DFrameSetLightRange(IDirect3DRMFrame *pd3drmf, long nMeshIndex, float fRange)
{
	LPDIRECT3DRMLIGHT lpLight = NULL;
	LPDIRECT3DRMFRAME lpChildFrame = NULL;
	HRESULT hr = S_OK;

	CHECK_IN_POINTER(pd3drmf);

	if (nMeshIndex == VWFRAME_ALLFRAME || nMeshIndex == VWFRAME_ALLHIERARCHY)
	{																						
		LPDIRECT3DRMLIGHTARRAY lpLightArray = NULL;
		int i, nCount;

		if (SUCCEEDED(hr = pd3drmf->GetLights(&lpLightArray) ))
		{
			nCount = lpLightArray->GetSize();
			for (i = 0; i < nCount; i++)
				D3DFrameSetLightRange(pd3drmf, i, fRange );
		}
		
		SAFERELEASE(lpLightArray);															
		if (nMeshIndex == VWFRAME_ALLHIERARCHY)												
		{																					
			LPDIRECT3DRMFRAMEARRAY lpFrameArray = NULL;										
			if (SUCCEEDED(hr = pd3drmf->GetChildren(&lpFrameArray) ))						
			{																				
				nCount = lpFrameArray->GetSize();											
				for (i = 0; i < nCount; i++)
				{
					if (SUCCEEDED(lpFrameArray->GetElement(i, &lpChildFrame)) && lpChildFrame)
					{
						D3DFrameSetLightRange(lpChildFrame, VWFRAME_ALLHIERARCHY, fRange );
					}
					SAFERELEASE(lpChildFrame);
				}
			}														
			SAFERELEASE(lpFrameArray);
		}				
	}
	else
	{
		if (SUCCEEDED(hr = GetLight(pd3drmf, nMeshIndex, &lpLight))) 
			lpLight->SetRange(fRange);
	}

EXIT_FAIL:
	SAFERELEASE(lpLight);
	SAFERELEASE(lpChildFrame);

	return hr;
}


STDMETHODIMP CVWFrameObject::GetLightCount(long* nCount)
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMLIGHTARRAY lpLightArray = NULL;

	CHECK_OUT_POINTER(nCount);

	CHECKANDCREATEFRAME;

	hr = m_pd3drmf->GetLights(&lpLightArray);
	*nCount = lpLightArray->GetSize();

EXIT_FAIL:
	SAFERELEASE(lpLightArray);
	return hr;
}

STDMETHODIMP CVWFrameObject::GetLightRange( long nIndex,  float* fRange)
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMLIGHT lpLight = NULL;

	CHECK_OUT_POINTER(fRange);

	CHECKANDCREATEFRAME;

	if (SUCCEEDED(hr = GetLight(m_pd3drmf, nIndex, &lpLight))) 
	{
		*fRange = lpLight->GetRange();
	}

EXIT_FAIL:
	SAFERELEASE(lpLight);
	return hr;
}

STDMETHODIMP CVWFrameObject::SetSpotLightRadii( long nIndex,  float fUmbra,  float fPenumbra)
{
	HRESULT hr = S_OK;
	
	CHECKANDCREATEFRAME;

	D3DFrameSetLightRadii(m_pd3drmf, nIndex, fUmbra,  fPenumbra);

EXIT_FAIL:
	return hr;
}

HRESULT D3DFrameSetLightRadii(IDirect3DRMFrame *pd3drmf, long nMeshIndex, float fUmbra,  float fPenumbra)
{
	LPDIRECT3DRMLIGHT lpLight = NULL;
	LPDIRECT3DRMFRAME lpChildFrame = NULL;
	HRESULT hr = S_OK;

	CHECK_IN_POINTER(pd3drmf);

	if (nMeshIndex == VWFRAME_ALLFRAME || nMeshIndex == VWFRAME_ALLHIERARCHY)
	{																						
		LPDIRECT3DRMLIGHTARRAY lpLightArray = NULL;
		int i, nCount;

		if (SUCCEEDED(hr = pd3drmf->GetLights(&lpLightArray) ))
		{
			nCount = lpLightArray->GetSize();
			for (i = 0; i < nCount; i++)
				D3DFrameSetLightRadii(pd3drmf, i, fUmbra, fPenumbra );
		}
		
		SAFERELEASE(lpLightArray);															
		if (nMeshIndex == VWFRAME_ALLHIERARCHY)												
		{																					
			LPDIRECT3DRMFRAMEARRAY lpFrameArray = NULL;										
			if (SUCCEEDED(hr = pd3drmf->GetChildren(&lpFrameArray) ))						
			{																				
				nCount = lpFrameArray->GetSize();											
				for (i = 0; i < nCount; i++)
				{
					if (SUCCEEDED(lpFrameArray->GetElement(i, &lpChildFrame)) && lpChildFrame)
					{
						D3DFrameSetLightRadii(lpChildFrame, VWFRAME_ALLHIERARCHY, fUmbra, fPenumbra);
					}
					SAFERELEASE(lpChildFrame);
				}
			}														
			SAFERELEASE(lpFrameArray);
		}				
	}
	else
	{
		if (SUCCEEDED(hr = GetLight(pd3drmf, nMeshIndex, &lpLight))) 
		{
			lpLight->SetUmbra(fUmbra);
			lpLight->SetPenumbra(fPenumbra);
		}
	}

EXIT_FAIL:
	SAFERELEASE(lpLight);
	SAFERELEASE(lpChildFrame);

	return hr;
}


STDMETHODIMP CVWFrameObject::GetSpotLightRadii( long nIndex,  VARIANT* fUmbra,  VARIANT* fPenumbra)
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMLIGHT lpLight = NULL;

	CHECK_OUT_POINTER(fUmbra);
	CHECK_OUT_POINTER(fPenumbra);

	CHECKANDCREATEFRAME;

	VariantClear(fUmbra);
	VariantClear(fPenumbra);

	if (SUCCEEDED(hr = GetLight(m_pd3drmf, nIndex, &lpLight))) 
	{
		fUmbra->vt = VT_R4;
		fPenumbra->vt = VT_R4;

		fUmbra->fltVal = lpLight->GetUmbra( );
		fPenumbra->fltVal = lpLight->GetPenumbra();
	}

EXIT_FAIL:
	SAFERELEASE(lpLight);
	return hr;
}

#define UNITWIDTH	1.0
#define UNITHEIGHT	1.0

STDMETHODIMP CVWFrameObject::CreateUnitMesh()
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMFACE lpD3DRMFace = NULL;
	LPDIRECT3DRMMESHBUILDER lpd3drmMeshBuilder = NULL;

	CHECKANDCREATEFRAME;

	hr = m_pDirect3DRM->CreateMeshBuilder(&lpd3drmMeshBuilder);
	if (SUCCEEDED(hr) && lpd3drmMeshBuilder)
	{
		lpd3drmMeshBuilder->AddVertex(D3DVALUE(-(UNITWIDTH/2.0)), D3DVALUE(-(UNITHEIGHT/2.0)), D3DVALUE( 0.0));
		lpd3drmMeshBuilder->AddVertex(D3DVALUE( (UNITWIDTH/2.0)), D3DVALUE(-(UNITHEIGHT/2.0)), D3DVALUE( 0.0));
		lpd3drmMeshBuilder->AddVertex(D3DVALUE( (UNITWIDTH/2.0)), D3DVALUE( (UNITHEIGHT/2.0)), D3DVALUE( 0.0));
		lpd3drmMeshBuilder->AddVertex(D3DVALUE(-(UNITWIDTH/2.0)), D3DVALUE( (UNITHEIGHT/2.0)), D3DVALUE( 0.0));
		lpd3drmMeshBuilder->AddNormal(D3DVALUE( 0.0), D3DVALUE( 0.0), D3DVALUE( -1.0));
		lpd3drmMeshBuilder->SetTextureCoordinates(0,  D3DVALUE( 1.0), D3DVALUE( 1.0));
		lpd3drmMeshBuilder->SetTextureCoordinates(1,  D3DVALUE( 0.0), D3DVALUE( 1.0));
		lpd3drmMeshBuilder->SetTextureCoordinates(2,  D3DVALUE( 0.0), D3DVALUE( 0.0));
		lpd3drmMeshBuilder->SetTextureCoordinates(3,  D3DVALUE( 1.0), D3DVALUE( 0.0));
		
		hr = lpd3drmMeshBuilder->CreateFace(&lpD3DRMFace);

		if (SUCCEEDED(hr) && lpD3DRMFace)
		{
			lpD3DRMFace->AddVertexAndNormalIndexed(0, 0);
			lpD3DRMFace->AddVertexAndNormalIndexed(1, 0);
			lpD3DRMFace->AddVertexAndNormalIndexed(2, 0);
			lpD3DRMFace->AddVertexAndNormalIndexed(3, 0);
		}

		lpd3drmMeshBuilder->SetQuality(D3DRMRENDER_UNLITFLAT);

		m_pd3drmf->AddVisual(lpd3drmMeshBuilder);
	}

EXIT_FAIL:
	SAFERELEASE(lpd3drmMeshBuilder);
	SAFERELEASE(lpD3DRMFace);
	return hr;
}

STDMETHODIMP CVWFrameObject::CreateMesh()
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMFACE lpD3DRMFace = NULL;
	LPDIRECT3DRMMESHBUILDER lpd3drmMeshBuilder = NULL;

	CHECKANDCREATEFRAME;

	hr = m_pDirect3DRM->CreateMeshBuilder(&lpd3drmMeshBuilder);

	if (SUCCEEDED(hr) && lpd3drmMeshBuilder)
	{
		lpd3drmMeshBuilder->SetQuality(D3DRMRENDER_UNLITFLAT);

		hr = m_pd3drmf->AddVisual(lpd3drmMeshBuilder);
	}

EXIT_FAIL:
	SAFERELEASE(lpd3drmMeshBuilder);
	SAFERELEASE(lpD3DRMFace);
	return hr;
}

//******************************************************
// AddFace
// Add an N-sided face to your mesh

STDMETHODIMP CVWFrameObject::CreateMeshFace(long nMeshIndex, VARIANT varArgs)
{
	HRESULT hr = S_OK;
	DISPPARAMS dispparams;
	CComVariant varTemp;
	int i;
	float fX, fY, fZ;
	LPDIRECT3DRMFACE lpD3DRMFace = NULL;
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;

	InitializeDISPPARAMS(&dispparams, 0);

	// VT_ERROR means optional arg not passed in
	if (varArgs.vt != VT_ERROR)
	{
		hr = VariantCopyInd(&varTemp, &varArgs);
		if (FAILED(hr))
			goto EXIT_FAIL;

		hr = ConvertJSArrayToDISPPARAMS(&varTemp, &dispparams);
		if (FAILED(hr))
		{
			hr = ConvertSAFEARRAYToDISPPARAMS(&varTemp, &dispparams);
			if (FAILED(hr))
				goto EXIT_FAIL;
		}
	}

	CHECKANDCREATEFRAME;

	if (FAILED(hr = GetVisual(IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex)) || !lpMeshBuilder) 
		goto EXIT_FAIL;

	hr = lpMeshBuilder->CreateFace(&lpD3DRMFace);
	if (FAILED(hr)) goto EXIT_FAIL;

	//If the array is an array of float's
	if (SUCCEEDED(GetNumericFromVariant(dispparams.rgvarg[0], &fX)))
	{
		//This is an array of floats, so make sure it's sets of 3
		//and there is at least 9 elements. (minimum to make a triangle)
		if (dispparams.cArgs % 3 != 0 || dispparams.cArgs < 9)
		{
			hr = E_INVALIDARG;
			goto EXIT_FAIL;
		}

		//Go through floats in the array 3 at a time
		for (i = dispparams.cArgs-1; i >= 0; i = i - 3)
		{
			//Make sure everything in the array is a float.
			if ( FAILED(GetNumericFromVariant(dispparams.rgvarg[i], &fX)) || 
				 FAILED(GetNumericFromVariant(dispparams.rgvarg[i-1], &fY)) ||
				 FAILED(GetNumericFromVariant(dispparams.rgvarg[i-2], &fZ)) )
			{
				hr = E_INVALIDARG;
				goto EXIT_FAIL;
			}
			hr = lpD3DRMFace->AddVertex(fX, fY, fZ);
			if (FAILED(hr)) goto EXIT_FAIL;
		}

	}
	else if (dispparams.rgvarg[0].vt == VT_DISPATCH)
	{   //Maybe this is an array of vectors?

		//Iterate through the safearray.  We'll ditch out of here if we
		//find anything that's not a vector.
		for (i = dispparams.cArgs-1; i >= 0; i--)
		{
			CVectorPtr vecPtr;

			if (dispparams.rgvarg[i].vt == VT_DISPATCH)
			{
				IDispatch* pdisp = dispparams.rgvarg[i].pdispVal;

				vecPtr = pdisp;
				if (!vecPtr.p)
				{
					hr = E_INVALIDARG;
					goto EXIT_FAIL;
				}
				vecPtr->get(&fX, &fY, &fZ);

				hr = lpD3DRMFace->AddVertex(fX, fY, fZ);
				if (FAILED(hr)) goto EXIT_FAIL;
			}
		}
	}

EXIT_FAIL:
	SAFERELEASE(lpD3DRMFace);
	SAFERELEASE(lpMeshBuilder);

	TerminateDISPPARAMS(VARIANT_TRUE, &dispparams);

	return hr;
}

STDMETHODIMP CVWFrameObject::GetMeshFaceCount(long nMeshIndex, VARIANT* varFaceCount)
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;

	CHECK_OUT_POINTER(varFaceCount);

	CHECKANDCREATEFRAME;

	VariantClear(varFaceCount);
	
	varFaceCount->vt = VT_I4;

	if (SUCCEEDED(hr = GetVisual(IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder, nMeshIndex))) 
	{
		varFaceCount->lVal = lpMeshBuilder->GetFaceCount();
	}

	SAFERELEASE(lpMeshBuilder);

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::DoIntersections(  IPropertyList* pBoundaryList,
											   float x1, float y1, float z1, 
											   float x2, float y2, float z2, 
											   IPropertyList **ppIDataList)
{
	IBoundary			*pec = NULL;
	long				propcnt;
	int					i;
	//Point3				p1(x1,y1,z1), p2(x2,y2,z2);
	IVector				*pVec1 = NULL, *pVec2 = NULL;
	HRESULT				hr = S_OK;
	HRESULT				hitResult = S_OK;

	CHECK_IN_POINTER(pBoundaryList);
	CHECK_OUT_POINTER(ppIDataList);

	if (FAILED(hr = pBoundaryList->get_Count(&propcnt)))
		goto EXIT_FAIL;

	if (propcnt < 1)
	{
		hr = E_FAIL;
		goto EXIT_FAIL;
	}

	// Need to loop over all the edge cycles and find the closest intersection with an
	// impassable edge and all intersections with explicitly passable edges (those which
	// are intersected before the impassable edge is intersected).
	for (i = 0; i < propcnt; i++)
	{
		if (FAILED(hr = pBoundaryList->get_ObjectProperty(i, (IObjectProperty **)&pec)))
			goto EXIT_FAIL;

		if (FAILED(hr = pec->Intersect(pBoundaryList, i, x1, y1, z1, x2, y2, z2, ppIDataList)))
			goto EXIT_FAIL;

		if (S_INTERSECTION == hr)
			hitResult = hr;

		SAFERELEASE(pec);
	}

	if (S_INTERSECTION == hitResult)
		hr = S_INTERSECTION;

EXIT_FAIL:
	SAFERELEASE(pec);
	SAFERELEASE(pVec1);
	SAFERELEASE(pVec2);

	return(hr);
}

STDMETHODIMP CVWFrameObject::DoGetFloorHeight(IPropertyList* pBoundaryList, float x1, float y1, float z1, float *pFloorHeight)
{
	return DoGetFloorOrCeiling(pBoundaryList, x1, y1, z1, VARIANT_TRUE, pFloorHeight);
}

STDMETHODIMP CVWFrameObject::DoGetCeilingHeight(IPropertyList* pBoundaryList, float x1, float y1, float z1, float *pCeilingHeight)
{
	return DoGetFloorOrCeiling(pBoundaryList, x1, y1, z1, VARIANT_FALSE, pCeilingHeight);
}

HRESULT CVWFrameObject::DoGetFloorOrCeiling(IPropertyList* pBoundaryList, float x1, float y1, float z1, VARIANT_BOOL bFloor, float *pHeight)
{
	IBoundary			*pec = NULL;
	long				propcnt;
	int					i;
	float				fHeightLower, fHeightUpper;
	VARIANT_BOOL		bWithin, bPassable;
	Point2				p1( x1, z1 );
	HRESULT				hr = S_OK;

	CHECK_IN_POINTER(pBoundaryList);
	CHECK_OUT_POINTER(pHeight);

	*pHeight = y1;
	
	if (FAILED(hr = pBoundaryList->get_Count(&propcnt)))
		goto EXIT_FAIL;

	if (propcnt < 1)
	{
		hr = E_FAIL;
		goto EXIT_FAIL;
	}

	// Need to loop over all the edge cycles and find the heighest impassable floor below 
	// the point or the lowest impassable ceiling above the point.
	for (i=0;i<propcnt;i++)
	{
		if (FAILED(hr = pBoundaryList->get_ObjectProperty(i, (IObjectProperty **)&pec)))
			goto EXIT_FAIL;
		if (FAILED(hr = pec->IsWithinExt(&p1, &bWithin)))
			goto EXIT_FAIL;
		if (!bWithin)
		{
			SAFERELEASE(pec);
			continue;
		}
		if (FAILED(hr = pec->get_IsPassable(&bPassable)))
			goto EXIT_FAIL;
		if (bPassable)
		{
			SAFERELEASE(pec);
			continue;
		}
		if (FAILED(hr = pec->get_HeightUpper(&fHeightUpper)))
			goto EXIT_FAIL;
		if (FAILED(hr = pec->get_HeightLower(&fHeightLower)))
			goto EXIT_FAIL;
		if (fHeightUpper > fHeightLower)
		{
			if (bFloor)
			{
				if (y1 >= fHeightUpper && (fHeightUpper > *pHeight || *pHeight == y1))
					*pHeight = fHeightUpper;
				else if (y1 >= fHeightLower && (fHeightLower > *pHeight || *pHeight == y1))
					*pHeight = fHeightLower;
			}
			else
			{
				if (y1 <= fHeightLower && (fHeightLower < *pHeight || *pHeight == y1))
					*pHeight = fHeightLower;
				else if (y1 <= fHeightUpper && (fHeightUpper < *pHeight || *pHeight == y1))
					*pHeight = fHeightUpper;
			}
		}
		SAFERELEASE(pec);
	}

EXIT_FAIL:
	SAFERELEASE(pec);

	return hr;
}

STDMETHODIMP CVWFrameObject::GetVisualIndexByName(BSTR bstrName, long* nIndex)
{
	return GetVisualIndexByNameAndIID(bstrName, IID_IDirect3DRMVisual, nIndex);
}

STDMETHODIMP CVWFrameObject::GetVisualIndexByNameAndIID(BSTR bstrName, REFIID ref, long* plIndex)
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMVISUAL lpVisual = NULL;
	CComVariant	varVisuals;
	DWORD dwSize = 256;
	char szTmp[256];
	int i;

	CHECK_OUT_POINTER(plIndex);

	CHECKANDCREATEFRAME;

	*plIndex = -1;

	if (m_pd3drmf) 
	{
		if (FAILED(hr = GetVisualCountByIID(ref, &varVisuals)))
			goto EXIT_FAIL;

		for (i = 0; i < varVisuals.lVal; i++)
		{
			if (SUCCEEDED(hr = GetVisual(ref, (void**) &lpVisual, i)) && lpVisual)
			{
				dwSize = 256;
				hr = lpVisual->GetName(&dwSize, szTmp);
				if (FAILED(hr)) goto EXIT_FAIL;

				if ( CString(bstrName).Compare(szTmp) == 0)
				{
					*plIndex = i;
					break;
				}
			}
			SAFERELEASE(lpVisual);
		}

		if (*plIndex == -1)
		{
			hr = E_INVALIDARG;
			VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CVWFrameObject::GetVisualIndexByName: can't find named visual.\n");
		}

	} 
	else
		hr = E_FAIL;


EXIT_FAIL:
	SAFERELEASE(lpVisual);
	return hr;
}

STDMETHODIMP CVWFrameObject::Transform(long nRotationAxis, float *pfDstX, float *pfDstY, float *pfDstZ, float fSrcX, float fSrcY, float fSrcZ)
{
	D3DVECTOR d3dvSrc = {fSrcX, fSrcY, fSrcZ}, d3dvDst;
	HRESULT hr = S_OK;

	if (nRotationAxis != AXIS_Y)
		return E_INVALIDARG;

	CHECK_OUT_POINTER(pfDstX);
	CHECK_OUT_POINTER(pfDstY);
	CHECK_OUT_POINTER(pfDstZ);
	CHECKANDCREATEFRAME;

	if (FAILED(hr = CloneRemoveXZRotation()))
		return hr;

	if(FAILED(hr = m_pd3drmfClone->Transform(&d3dvDst, &d3dvSrc)))
		return hr;

	*pfDstX = d3dvDst.x;
	*pfDstY = d3dvDst.y;
	*pfDstZ = d3dvDst.z;

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::InverseTransform(long nRotationAxis, float *pfDstX, float *pfDstY, float *pfDstZ, float fSrcX, float fSrcY, float fSrcZ)
{
	D3DVECTOR d3dvSrc = {fSrcX, fSrcY, fSrcZ}, d3dvDst;
	HRESULT hr = S_OK;

	if (nRotationAxis != AXIS_Y)
		return E_INVALIDARG;

	CHECK_OUT_POINTER(pfDstX);
	CHECK_OUT_POINTER(pfDstY);
	CHECK_OUT_POINTER(pfDstZ);
	CHECKANDCREATEFRAME;

	if (FAILED(hr = CloneRemoveXZRotation()))
		return hr;

	if(FAILED(hr = m_pd3drmfClone->InverseTransform(&d3dvDst, &d3dvSrc)))
		return hr;

	*pfDstX = d3dvDst.x;
	*pfDstY = d3dvDst.y;
	*pfDstZ = d3dvDst.z;

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWFrameObject::CloneRemoveXZRotation()
{
	D3DVECTOR d3dvPos, d3dvDir, d3dvUp;
	HRESULT hr = S_OK;

	if (!m_pd3drmfClone)
	{
		if (FAILED(hr = m_pd3drmf->Clone(0, IID_IDirect3DRMFrame, (void**)&m_pd3drmfClone)) || !m_pd3drmfClone)
			return hr;
	}

	if (FAILED(hr = m_pd3drmf->GetPosition(NULL, &d3dvPos)))
		return hr;
	if (FAILED(hr = m_pd3drmf->GetOrientation(NULL, &d3dvDir, &d3dvUp)))
		return hr;

	// If pointing straight up or down just use up vector to determine direction.
	if (d3dvDir.x == 0.0F && d3dvDir.z == 0.0F)
	{
		d3dvDir.x = -d3dvUp.x;
		d3dvDir.z = -d3dvUp.z;
	}
	// If laying on side just sit up.
	if (d3dvUp.y == 0.0F)
		d3dvUp.y = 1.0F;
	
	if (FAILED(hr = m_pd3drmfClone->SetOrientation(NULL, d3dvDir.x, 0.0F, d3dvDir.z, 0.0F, d3dvUp.y, 0.0F)))
		return hr;
	if (FAILED(hr = m_pd3drmfClone->AddScale(D3DRMCOMBINE_BEFORE, m_fScaleX, m_fScaleY, m_fScaleZ)))
		return hr;
	if (FAILED(hr = m_pd3drmfClone->SetPosition(NULL, d3dvPos.x, d3dvPos.y, d3dvPos.z)))
		return hr;

	return hr;
}

STDMETHODIMP CVWFrameObject::SetSceneFogColor(float red, float green, float blue)
{
	IDirect3DRMFrame3	*pd3drmfRef3 = NULL;
	DWORD color = 0;
	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;
	
	if ((red<0) || (red>1) || (blue<0) || (blue>1) || (green<0) || (green>1))
	{
		hr = E_FAIL;
		goto EXIT_FAIL;
	}

	if(FAILED(hr = m_pd3drmf->QueryInterface(IID_IDirect3DRMFrame3,(void**)&pd3drmfRef3)))
		goto EXIT_FAIL;
		
	if (pd3drmfRef3)
	{
		color += (int)(blue*255) + (((int)(green*255))<<8) + (((int)(red*255))<<16);
		pd3drmfRef3->SetSceneFogColor(color);
	}

EXIT_FAIL:
	SAFERELEASE(pd3drmfRef3);

	return hr;
}

STDMETHODIMP CVWFrameObject::SetSceneFogEnable(VARIANT_BOOL fogEnable) 
{
	IDirect3DRMFrame3	*pd3drmfRef3 = NULL;
	
	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;

	if(FAILED(hr = m_pd3drmf->QueryInterface(IID_IDirect3DRMFrame3,(void**)&pd3drmfRef3)))
		goto EXIT_FAIL;
		
	if (pd3drmfRef3)
	{
		if (fogEnable == VARIANT_TRUE) {
			pd3drmfRef3->SetSceneFogEnable(TRUE);
		}
		else{
			pd3drmfRef3->SetSceneFogEnable(FALSE);
		}
	}

EXIT_FAIL:
	SAFERELEASE(pd3drmfRef3);

	return hr;
	
}

STDMETHODIMP CVWFrameObject::SetSceneFogParams(float start, float end)
{
	IDirect3DRMFrame3	*pd3drmfRef3 = NULL;
	DWORD color = 0;
	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;

	if(FAILED(hr = m_pd3drmf->QueryInterface(IID_IDirect3DRMFrame3,(void**)&pd3drmfRef3)))
		goto EXIT_FAIL;
		
	if (pd3drmfRef3)
	{
		pd3drmfRef3->SetSceneFogParams(start,end,0.0f);
	}

EXIT_FAIL:
	SAFERELEASE(pd3drmfRef3);

	return hr;
}

HRESULT D3DFrameDeleteShadow(IDirect3DRMFrame *pd3drmfThis, int nMeshIndex, IVWFrame *pvwfLightSrc, int nIndex) 
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMVISUALARRAY lpVisualArray = NULL;
	LPDIRECT3DRMVISUAL lpVisual = NULL;
	LPDIRECT3DRMVISUAL lpShadowVisual = NULL;
	LPDIRECT3DRMMESHBUILDER lpMeshBuilder = NULL;
	LPDIRECT3DRMSHADOW2 lpShadow2 = NULL;
	LPDIRECT3DRMFRAMEARRAY lpFrameArray = NULL;	
	LPDIRECT3DRMFRAME lpChildFrame = NULL;
	IDirect3DRMFrame	*pd3drmfLight  = NULL;
	LPDIRECT3DRMLIGHT lpLight = NULL;
	LPDIRECT3DRMLIGHT lpShadowLight = NULL;
	int i,nCount,nMCount = -1;

	CHECK_IN_POINTER(pd3drmfThis);
	CHECK_IN_POINTER(pvwfLightSrc);

	if (FAILED(hr = pd3drmfThis->GetVisuals(&lpVisualArray)) && lpVisualArray)
		goto EXIT_FAIL;

	if(FAILED(hr = pvwfLightSrc->get_Frame3D(&pd3drmfLight)))
		goto EXIT_FAIL;

	if (FAILED(hr = GetLight(pd3drmfLight, nIndex, &lpLight))) 
		goto EXIT_FAIL;


	if (nMeshIndex == VWFRAME_ALLFRAME || nMeshIndex == VWFRAME_ALLHIERARCHY)
	{																						
		nCount = lpVisualArray->GetSize();
		for (i = 0; i < nCount; i++)
			{
				if (SUCCEEDED(hr = lpVisualArray->GetElement(i,&lpVisual))) 
				{
					if (SUCCEEDED(hr = lpVisual->QueryInterface(IID_IDirect3DRMShadow2, (void**)&lpShadow2))) 
					{
						lpShadow2->GetLight(&lpShadowLight);
						if (lpShadowLight==lpLight)
							hr = pd3drmfThis->DeleteVisual(lpShadow2);
					}
				}
			}

		if (nMeshIndex == VWFRAME_ALLHIERARCHY)												
		{																					
			if (SUCCEEDED(hr = pd3drmfThis->GetChildren(&lpFrameArray)) && lpFrameArray)
			{																				
				nCount = lpFrameArray->GetSize();											
				for (i = 0; i < nCount; i++)
				{
					if (SUCCEEDED(lpFrameArray->GetElement(i, &lpChildFrame)) && lpChildFrame)
					{
						D3DFrameDeleteShadow(lpChildFrame,nMeshIndex,pvwfLightSrc,nIndex);
					}
				}
			}														
		}				
	}
	else
	{
		if (SUCCEEDED(hr = lpVisualArray->GetElement(nMeshIndex,&lpVisual))) 
		{
			if (SUCCEEDED(hr = lpVisual->QueryInterface(IID_IDirect3DRMMeshBuilder, (void**)&lpMeshBuilder))) 
			{
				nCount = lpVisualArray->GetSize();
				for (i = 0; i < nCount; i++)
				{
					if (SUCCEEDED(hr = lpVisualArray->GetElement(i,&lpVisual))) 
					{
						if (SUCCEEDED(hr = lpVisual->QueryInterface(IID_IDirect3DRMShadow2, (void**)&lpShadow2))) 
						{
							lpShadow2->GetVisual(&lpShadowVisual);
							lpShadow2->GetLight(&lpShadowLight);
							if ((lpShadowLight==lpLight)&&(lpShadowVisual==lpMeshBuilder))
								hr = pd3drmfThis->DeleteVisual(lpShadow2);
						}
					}
				}
			}
		}
	}

EXIT_FAIL:
	SAFERELEASE(lpVisual);
	SAFERELEASE(lpVisualArray);
	SAFERELEASE(lpShadowVisual);
	SAFERELEASE(lpMeshBuilder);
	SAFERELEASE(pd3drmfLight);
	SAFERELEASE(lpShadowLight);
	SAFERELEASE(lpLight);
	SAFERELEASE(lpShadow2);
	SAFERELEASE(lpFrameArray);
	SAFERELEASE(lpChildFrame);

	return hr;
}

STDMETHODIMP CVWFrameObject::DeleteShadow(int nMeshIndex, IVWFrame *pvwfLightSrc, int nIndex) 
{
	HRESULT hr = S_OK;

	CHECKANDCREATEFRAME;
	
	CHECK_IN_POINTER(pvwfLightSrc);
	
	if ((nIndex < 0) || ((nMeshIndex < 0) && (nMeshIndex!=VWFRAME_ALLFRAME) && 
						 (nMeshIndex!=VWFRAME_ALLHIERARCHY))) 
		{ hr = E_FAIL; goto EXIT_FAIL; }

	hr = D3DFrameDeleteShadow(m_pd3drmf,nMeshIndex,pvwfLightSrc,nIndex);


EXIT_FAIL:

	return hr;
}

HRESULT D3DFrameCreateShadow(IDirect3DRMFrame *pd3drmf, IDirect3DRM3 *pDirect3DRM3, int nMeshIndex, IVWFrame *pvwfLightSrc, int nIndex, float ylevel) 
{
	HRESULT hr = S_OK;
	IDirect3DRMFrame	*pd3drmfLight  = NULL;
	LPDIRECT3DRMLIGHT lpLight = NULL;
	LPDIRECT3DRMSHADOW2 lpShadow2 = NULL;
	LPDIRECT3DRMVISUALARRAY lpVisualArray = NULL;
	LPDIRECT3DRMVISUAL lpVisual = NULL;
	LPDIRECT3DRMFRAMEARRAY lpFrameArray = NULL;	
	LPDIRECT3DRMFRAME lpChildFrame = NULL;
	int	i, nCount,nMeshCount = -1;
	
	CHECK_IN_POINTER(pd3drmf);
	CHECK_IN_POINTER(pvwfLightSrc);

	if(FAILED(hr = pvwfLightSrc->get_Frame3D(&pd3drmfLight)))
		goto EXIT_FAIL;

	if (FAILED(hr = GetLight(pd3drmfLight, nIndex, &lpLight))) 
		goto EXIT_FAIL;

	if (FAILED(hr = pd3drmf->GetVisuals(&lpVisualArray) && lpVisualArray))
		goto EXIT_FAIL;

	if (nMeshIndex == VWFRAME_ALLFRAME || nMeshIndex == VWFRAME_ALLHIERARCHY)
	{																						
		nCount = lpVisualArray->GetSize();
		for (i = 0; i < nCount; i++)
		{
			if (SUCCEEDED(hr = lpVisualArray->GetElement(i,&lpVisual))) 
			{
				if (SUCCEEDED(hr = lpVisual->QueryInterface(IID_IDirect3DRMMeshBuilder,(void**)&lpVisual)))
					{
							if (SUCCEEDED(pDirect3DRM3->CreateShadow( lpVisual,lpLight,0,ylevel,0,0,1,0,&lpShadow2 )))
								pd3drmf->AddVisual( lpShadow2 );
					}
			}
		}
		if (nMeshIndex == VWFRAME_ALLHIERARCHY)												
		{																					
			if (SUCCEEDED(hr = pd3drmf->GetChildren(&lpFrameArray)) && lpFrameArray)
			{																				
				nCount = lpFrameArray->GetSize();											
				for (i = 0; i < nCount; i++)
				{
					if (SUCCEEDED(lpFrameArray->GetElement(i, &lpChildFrame)) && lpChildFrame)
					{
						D3DFrameCreateShadow(lpChildFrame,pDirect3DRM3,nMeshIndex,pvwfLightSrc,nIndex,ylevel);
					}
				}
			}														
		}				
	}
	else
	{
		if (SUCCEEDED(hr = lpVisualArray->GetElement(nMeshIndex,&lpVisual))) 
		{
			if (SUCCEEDED(hr = lpVisual->QueryInterface(IID_IDirect3DRMMeshBuilder,(void**)&lpVisual)))
			{
				if (SUCCEEDED(pDirect3DRM3->CreateShadow( lpVisual,lpLight,0,ylevel,0,0,1,0,&lpShadow2 )))
					pd3drmf->AddVisual( lpShadow2 );
			}
		}
	}

EXIT_FAIL:
	SAFERELEASE(lpLight);	
	SAFERELEASE(lpVisualArray);	
	SAFERELEASE(pd3drmfLight);		
	SAFERELEASE(lpVisual);	
	SAFERELEASE(lpShadow2);		
	SAFERELEASE(lpFrameArray);
	SAFERELEASE(lpChildFrame);
	
	return hr;
}

STDMETHODIMP CVWFrameObject::CreateShadow(int nMeshIndex, IVWFrame *pvwfLightSrc, int nIndex, float ylevel) 
{
	IDirect3DRM3        *pDirect3DRM3 = NULL;
	HRESULT hr = S_OK;

	CHECK_IN_POINTER(pvwfLightSrc);

	CHECKANDCREATEFRAME;
	
	if ((nIndex < 0) || ((nMeshIndex!=VWFRAME_ALLHIERARCHY)&&(nMeshIndex!=VWFRAME_ALLFRAME)&&
						 (nMeshIndex <0)) ) 
	{ hr = E_FAIL; goto EXIT_FAIL; }

	if(FAILED(hr = m_pDirect3DRM->QueryInterface(IID_IDirect3DRM3,(void**)&pDirect3DRM3)))
		goto EXIT_FAIL;

	hr = D3DFrameCreateShadow(m_pd3drmf,pDirect3DRM3,nMeshIndex,pvwfLightSrc,nIndex,ylevel);

EXIT_FAIL:
	SAFERELEASE(pDirect3DRM3);

	return hr;	
}

#ifdef _DXMEDIA
void RenderFileToMMStream(BSTR szFileName, IMultiMediaStream **ppMMStream, IDirectDraw *pDD)
{		
	IAMMultiMediaStream *pAMStream = NULL;

 	if (FAILED(CoCreateInstance(CLSID_AMMultiMediaStream, NULL, CLSCTX_INPROC_SERVER, 
				IID_IAMMultiMediaStream, (void **)&pAMStream)))
			goto EXIT_FAIL;
 	
	if(FAILED(pAMStream->Initialize(STREAMTYPE_READ, AMMSF_NOGRAPHTHREAD, NULL)))
		goto EXIT_FAIL;
 	
	if(FAILED(pAMStream->AddMediaStream(pDD, &MSPID_PrimaryVideo, 0, NULL)))
		goto EXIT_FAIL;
 	
	if(FAILED(pAMStream->AddMediaStream(NULL, &MSPID_PrimaryAudio, AMMSF_ADDDEFAULTRENDERER, NULL)))
		goto EXIT_FAIL;
 	
	if(FAILED(pAMStream->OpenFile(szFileName, 0)))
		goto EXIT_FAIL;

EXIT_FAIL:
	*ppMMStream = pAMStream;

}
#endif

DWORD GetClosestPowerOf2(DWORD n, BOOL bGreater)
{
	ASSERT(((bGreater == 1) || (bGreater == 0)) && (sizeof(DWORD) == 4));

	DWORD i = 0;
	for (n >>= 1; n != 0; i++) {
		n >>= 1;
	}
	i += (bGreater && ((n & ~(1 << i)) != 0));

	return (1 << i);
}

int CVWFrameObject::VideoCallBack(LPDIRECT3DRMUSERVISUAL uvis,
									  void* arg,
									  D3DRMUSERVISUALREASON reason,
									  LPDIRECT3DRMDEVICE dev,
									  LPDIRECT3DRMVIEWPORT view) 
{
#ifdef _DXMEDIA
	IDirectDrawSurface* pSurface = NULL;

    if (reason == D3DRMUSERVISUAL_RENDER)
	{
		CVWFrameObject * pthisInst = (CVWFrameObject*)arg;
		
		if (WaitForSingleObject(pthisInst->m_hVidMutex,0)==WAIT_OBJECT_0) {
			if (pthisInst->m_pSample->Update(0, NULL, NULL, NULL) == S_OK)
			{
				if (SUCCEEDED(pthisInst->m_pVidTexture->GetSurface(0,&pSurface)))
				{
					if (pSurface!=pthisInst->m_pVidSurface)
					{	
						if (SUCCEEDED(pSurface->Blt(NULL,pthisInst->m_pVidSurface,NULL,0,NULL)))
							pthisInst->m_pVidTexture->Changed(D3DRMTEXTURE_CHANGEDPIXELS,0,NULL);
					}
					else pthisInst->m_pVidTexture->Changed(D3DRMTEXTURE_CHANGEDPIXELS,0,NULL);
				}
			}
			else {
				IDirect3DRMFrame* pd3drmf = NULL;
				pthisInst->get_Frame3D(&pd3drmf);
				pd3drmf->DeleteVisual(uvis);
				pthisInst->m_pMMStream->SetState(STREAMSTATE_STOP);
				SAFERELEASE(pd3drmf);
			}
			ReleaseMutex(pthisInst->m_hVidMutex);
		}
	}

	SAFERELEASE(pSurface);
#endif
	return TRUE;
}

STDMETHODIMP CVWFrameObject::PlayVideo(int nMeshIndex,BSTR sfilename) 
{
	HRESULT hr = S_OK;

#ifdef _DXMEDIA

	IDirectDraw* lpDD = NULL;
	IMediaStream* pPrimaryVidStream = NULL;
	IDirectDrawMediaStream* pDDStream = NULL;
	IDirectDrawSurface* pSurface = NULL;
	LPDIRECT3DRMUSERVISUAL video = NULL;
	IDirect3DRM3* pDirect3DRM3;
	DDSURFACEDESC ddsd,ddsd2;
	RECT rect;

	WaitForSingleObject(m_hVidMutex,INFINITE);
	if (m_pMMStream) m_pMMStream->SetState(STREAMSTATE_STOP);
	SAFERELEASE(m_pMMStream);
	SAFERELEASE(m_pSample);
	SAFERELEASE(m_pVidTexture);
	SAFERELEASE(m_pVidSurface);
	

	if (FAILED(hr = m_pDirect3DRM->QueryInterface(IID_IDirect3DRM3,(void **)&pDirect3DRM3)))
		goto EXIT_FAIL;

	CHECKANDCREATEFRAME;

	if(FAILED(m_pWorld->get_Tool(CComBSTR("DirectDraw"),(IUnknown**)&lpDD)))
		goto EXIT_FAIL;

	RenderFileToMMStream(sfilename, &m_pMMStream, lpDD);

 	if (FAILED(m_pMMStream->GetMediaStream(MSPID_PrimaryVideo, &pPrimaryVidStream)))
		goto EXIT_FAIL;
 
	if (FAILED(pPrimaryVidStream->QueryInterface(IID_IDirectDrawMediaStream, (void **)&pDDStream)))
		goto EXIT_FAIL;

	ddsd.dwSize = sizeof(ddsd);
	ddsd2.dwSize = sizeof(ddsd2);
	pDDStream->GetFormat(&ddsd, NULL, NULL, NULL);
	rect.top = rect.left = 0;
	rect.bottom = ddsd.dwHeight;
 	rect.right = ddsd.dwWidth;
	ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY;
	memcpy(&ddsd2,&ddsd,sizeof(ddsd));
	ddsd2.dwHeight = GetClosestPowerOf2(ddsd.dwHeight,TRUE);
	ddsd2.dwWidth = GetClosestPowerOf2(ddsd.dwWidth,TRUE);

	if (FAILED(lpDD->CreateSurface(&ddsd,&m_pVidSurface,NULL)))
		goto EXIT_FAIL;
	
	if (!((ddsd.dwWidth == ddsd2.dwWidth) && (ddsd.dwHeight == ddsd2.dwHeight))) {
		if (FAILED(lpDD->CreateSurface(&ddsd2,&pSurface,NULL)))
			goto EXIT_FAIL;
	}
	else  {
		pSurface = m_pVidSurface;
		pSurface->AddRef();
	}

 	if (FAILED(hr = pDDStream->CreateSample(m_pVidSurface, &rect, 0, &m_pSample)))
		goto EXIT_FAIL;

	if (FAILED(pSurface->Blt(NULL,m_pVidSurface,NULL,0,NULL)))
		goto EXIT_FAIL;

	if (FAILED(pDirect3DRM3->CreateTextureFromSurface(pSurface,&m_pVidTexture)))
		goto EXIT_FAIL;

	if (m_pVidTexture) 
	{
		if (FAILED(D3DFrameSetMeshTexture(m_pd3drmf,nMeshIndex,((IDirect3DRMTexture*)(m_pVidTexture)))))
			goto EXIT_FAIL;
	}

	if (FAILED(m_pMMStream->SetState(STREAMSTATE_RUN)))
		goto EXIT_FAIL;

	m_pDirect3DRM->CreateUserVisual(CVWFrameObject::VideoCallBack, (void*)this, &video);
	m_pd3drmf->AddVisual(video);

	ReleaseMutex(m_hVidMutex);

EXIT_FAIL:
	SAFERELEASE(lpDD);
	SAFERELEASE(pPrimaryVidStream);
	SAFERELEASE(pDDStream);
	SAFERELEASE(pSurface);
	SAFERELEASE(video);
	SAFERELEASE(pDirect3DRM3);
#endif

	return hr;
}
HRESULT
GetSurfaceDimensions(LPDIRECTDRAWSURFACE pdds, LPRECT prDim)
{

	HRESULT hr;
	DDSURFACEDESC ddsd;
	INIT_DXSTRUCT(ddsd);
	if (FAILED(hr = pdds->GetSurfaceDesc(&ddsd))) {
		return hr;
	}
	prDim->left = 0;
	prDim->top = 0;
	prDim->right = ddsd.dwWidth;
	prDim->bottom = ddsd.dwHeight;

	return S_OK;
}

HRESULT
ClearToColor(DWORD dwColor, LPRECT prDst, LPDIRECTDRAWSURFACE pdds)
{
	
	DDBLTFX ddbfx;
	INIT_DXSTRUCT(ddbfx);
	ddbfx.dwFillColor = dwColor;
	
	RECT rDst;
	if (prDst == NULL) {
		::GetSurfaceDimensions(pdds, &rDst);
		prDst = &rDst;
	}
	
	return pdds->Blt(prDst, NULL, NULL, DDBLT_COLORFILL | DDBLT_ASYNC, &ddbfx);
}

HRESULT FindMesh(IDirect3DRMFrame* pRefFrame,IDirect3DRMFrame** pFrame,IDirect3DRMMeshBuilder** pMesh) {
	HRESULT hr = E_FAIL;

	int	i, nCount;
	LPDIRECT3DRMVISUALARRAY lpVisualArray = NULL;
	if (SUCCEEDED(hr = pRefFrame->GetVisuals(&lpVisualArray) && lpVisualArray))
	{
		nCount = lpVisualArray->GetSize();
		for (i = 0; i < nCount; i++) {
			if (SUCCEEDED(hr = D3DFrameGetVisual(pRefFrame, IID_IDirect3DRMMeshBuilder, (void**)pMesh, i)))
			{
				*pFrame = pRefFrame;
				(*pFrame)->AddRef();
				SAFERELEASE(lpVisualArray);
				return S_OK;
			}
		}
	}
	SAFERELEASE(lpVisualArray);


	LPDIRECT3DRMFRAMEARRAY lpFrameArray = NULL;
	IDirect3DRMFrame* lpChildFrame = NULL;
	if (SUCCEEDED(hr = pRefFrame->GetChildren(&lpFrameArray)) && lpFrameArray)
	{																				
		nCount = lpFrameArray->GetSize();											
		for (i = 0; i < nCount; i++)
		{
			if (SUCCEEDED(lpFrameArray->GetElement(i, &lpChildFrame)) && lpChildFrame)
			{
				if(SUCCEEDED(FindMesh(lpChildFrame,pFrame,pMesh))) {
					SAFERELEASE(lpChildFrame);
					SAFERELEASE(lpFrameArray);
					return S_OK;
				}
			}
			SAFERELEASE(lpChildFrame);
		}
	}														
	SAFERELEASE(lpFrameArray);
			
	return E_FAIL;
}

STDMETHODIMP CVWFrameObject::Explode(unsigned long dwTime) 
{
	HRESULT hr = S_OK;

#ifdef _DXMEDIA
	IDXTransformFactory*	pTransFact = NULL;
	LPDIRECT3DRMMESHBUILDER3 lpMeshBuilder3 = NULL;
	LPDIRECT3DRMUSERVISUAL explosion = NULL;
	LPDIRECT3DRMMESHBUILDER3 lpMeshBuilder3In = NULL;
	LPDIRECT3DRMMESHBUILDER lpMeshBuilderIn = NULL;
	IDirect3DRMFrame*	pFrame = NULL;
	IDirectDraw* lpDD = NULL;
	IDXEffect* pEffect = NULL;
	IDirect3DRM3* pDirect3DRM3 = NULL;
	IDXTransform *pExploder = NULL;
	IExplode* pExplode = NULL;
	
	CHECKANDCREATEFRAME

	//get Direct3DRM3 and directdraw interfaces 
	if (FAILED(hr = m_pDirect3DRM->QueryInterface(IID_IDirect3DRM3,(void **)&pDirect3DRM3)))
		goto EXIT_FAIL;	
	if (FAILED(hr = m_pWorld->get_Tool(CComBSTR("DirectDraw"),(IUnknown**)&lpDD)))
		goto EXIT_FAIL;

	//get transform factory and create explode transform
	if (FAILED(hr = CoCreateInstance( CLSID_DXTransformFactory, NULL,
				 CLSCTX_INPROC, IID_IDXTransformFactory,
				 (void **)&pTransFact ))) 
				 goto EXIT_FAIL;
	if (FAILED(hr = pTransFact->CreateTransform( NULL, 0, NULL, 0,
                 NULL, NULL,CLSID_Explode,
                 IID_IDXTransform, (void **)&pExploder ))) 
				 goto EXIT_FAIL;
	if (FAILED(hr = pExploder->QueryInterface(IID_IExplode,(void **)&pExplode)))
		goto EXIT_FAIL;
	
	//create output mesh, get input mesh
	if (FAILED(hr = pDirect3DRM3->CreateMeshBuilder(&lpMeshBuilder3)))
		goto EXIT_FAIL;

	if (FAILED(hr = FindMesh(m_pd3drmf,&pFrame,&lpMeshBuilderIn)))
			goto EXIT_FAIL;
	
	
	if (FAILED(hr = lpMeshBuilderIn->QueryInterface(IID_IDirect3DRMMeshBuilder3,(void **)&lpMeshBuilder3In)))
		goto EXIT_FAIL;
	
	//setup transform input, ouput, and options
    if (FAILED(hr = pExploder->Setup( (IUnknown**)&lpMeshBuilder3In, 1, (IUnknown**)&lpMeshBuilder3, 1, 0 ))) 
		goto EXIT_FAIL;
	if (FAILED(hr = pExploder->QueryInterface(IID_IDXEffect, (void **)&pEffect ))) 
		goto EXIT_FAIL;
	if (FAILED(hr = ((IExplode*)(pExplode))->put_FinalVelocity(5)))
		goto EXIT_FAIL;
	if (FAILED(hr = ((IExplode*)(pExplode))->put_MaxRotations(4)))
		goto EXIT_FAIL;

	//delete mesh, add transform mesh
	if (FAILED(hr = pFrame->AddVisual((IDirect3DRMVisual *)lpMeshBuilder3)))
		goto EXIT_FAIL;
	if (FAILED(hr = pFrame->DeleteVisual((IDirect3DRMVisual *)lpMeshBuilder3In)))
		goto EXIT_FAIL;
	
	WaitForSingleObject(m_hExpMutex,INFINITE);
	
	SAFERELEASE(m_pEffect);
	SAFERELEASE(m_pExploder);
	SAFERELEASE(m_pExpMeshBuilder);
	SAFERELEASE(m_pExpMeshBuilderOrig);
	m_time = 0;
	m_pEffect = pEffect;
	m_pExploder = pExploder;
	m_pExpMeshBuilder = lpMeshBuilder3;
	m_pExpMeshBuilderOrig = lpMeshBuilder3In;

	m_pDirect3DRM->CreateUserVisual(CVWFrameObject::ExplodeCallBack, (void*)this, &explosion);
	m_pd3drmf->AddVisual(explosion);
	ReleaseMutex(m_hVidMutex);

EXIT_FAIL:
	SAFERELEASE(pTransFact);
	SAFERELEASE(lpMeshBuilderIn);
	SAFERELEASE(lpDD);
	SAFERELEASE(pDirect3DRM3);
	SAFERELEASE(pExplode);
	SAFERELEASE(pFrame);
#endif
	return hr;
}
int CVWFrameObject::ExplodeCallBack(LPDIRECT3DRMUSERVISUAL uvis,
									  void* arg,
									  D3DRMUSERVISUALREASON reason,
									  LPDIRECT3DRMDEVICE dev,
									  LPDIRECT3DRMVIEWPORT view) 
{
#ifdef _DXMEDIA

	long dif = 0;
	float durout = 2000,durin = 5000;

    if (reason == D3DRMUSERVISUAL_RENDER)
	{
		CVWFrameObject * pthisInst = (CVWFrameObject*)arg;

		if (!pthisInst->m_time) pthisInst->m_time = timeGetTime();
		dif = timeGetTime() - pthisInst->m_time - 5000;
		HRESULT hr = S_OK;

		if (dif > 0) 
		{
			if (dif < durout) 
			{
				//explode object out
				if (FAILED(hr = pthisInst->m_pEffect->put_Progress( (float)(dif) / durout ))) 
					goto EXIT_FAIL;
				if (FAILED(hr = pthisInst->m_pExploder->Execute( NULL, NULL, NULL ))) 
					goto EXIT_FAIL;
			}
			else if (dif < (durout+durin)) 
			{
				//suck object back in
				if (FAILED(hr = pthisInst->m_pEffect->put_Progress( 1- ((float)(dif-durout) / durin) ))) 
					goto EXIT_FAIL;
				if (FAILED(hr = pthisInst->m_pExploder->Execute( NULL, NULL, NULL ))) 
					goto EXIT_FAIL;
			}
			else  
			{
				pthisInst->m_time = 0;
				//execute last frame of transformation
				if (FAILED(hr = pthisInst->m_pEffect->put_Progress(0))) 
					goto EXIT_FAIL;
				if (FAILED(hr = pthisInst->m_pExploder->Execute( NULL, NULL, NULL ))) 
					goto EXIT_FAIL;
				//delete transform mesh, replace original mesh
				IDirect3DRMFrame* pd3drmf = NULL;
				pthisInst->get_Frame3D(&pd3drmf);
				pd3drmf->DeleteVisual(uvis);
				if (FAILED(hr = pd3drmf->DeleteVisual((IDirect3DRMVisual *)pthisInst->m_pExpMeshBuilder)))
					goto EXIT_FAIL;
				if (FAILED(hr = pd3drmf->AddVisual((IDirect3DRMVisual *)pthisInst->m_pExpMeshBuilderOrig)))
					goto EXIT_FAIL;
				SAFERELEASE(pd3drmf);
			}
		}
	}
EXIT_FAIL:
#endif
	return TRUE;
}	

STDMETHODIMP CVWFrameObject::SetInternalScale(float fScaleX,float fScaleY,float fScaleZ) {

	m_fScaleX = fScaleX;
	m_fScaleY = fScaleY;
	m_fScaleZ = fScaleZ;

	return S_OK;
}
