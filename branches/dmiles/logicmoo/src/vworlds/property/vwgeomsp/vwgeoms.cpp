/////////////////////////////////////////////////////////////////////////////
//
// VWGeomS.cpp : Implementation of CVWGeomApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include "d2d.h"
#include <math.h>
#include <vwgeomsp.h>
#include <vwgcache.h>
#include "VWGeomS.h"
#include "d3dmacs.h"
#include "d3dmath.h"
#include "spritemo.h"
#include <vwadata.h>
#include <syshelp.h>

#define nDEFFRAMES 24
#define SPRITEHEIGHT 2.0
#define SPRITEWIDTH	2.0
#define	szDefaultThumbNailName "tdefault.gif"
#define	szThumbnailBackground "default.gif"
#define RADTODEG(Rad) Rad*=57.29577951F


IMPLEMENT_OBJECTPROPERTY(CVWGeometrySprite)

typedef CComObject<CSpriteManagerObject> CComSpriteManager;

/////////////////////////////////////////////////////////////////////////////
//

CVWGeometrySprite::CVWGeometrySprite()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVWGeometrySprite::CVWGeometrySprite, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif

	m_pvwfTransform = NULL;
	m_pvwfGeometry = NULL;
	m_bThumbnailMode = VARIANT_FALSE;
	m_lpd3drmMeshBuilder = NULL;
	m_pd3drmTexture = NULL;
	m_pddsTexture = NULL;
	m_pd2drmfAccessoryScene = NULL;
	m_bAccessoriesDirty = VARIANT_TRUE;
	m_pSpriteFrame = NULL;
	m_pSpriteImage = NULL;
	m_pThumbnailImage = NULL;
	m_pINetFileManager = NULL;
	m_pDirect3DRM = NULL;
	m_pDirect2DRM = NULL;
	m_bOccluded = FALSE;
	//m_pSpriteManager = NULL;
	m_pSpriteManager = new CComSpriteManager;
	SAFEADDREF(m_pSpriteManager);
	m_iSpriteImageFrame = -1;
	m_iAxis = AXIS_Y;
	m_bAccessoriesShown = VARIANT_FALSE;
}

CVWGeometrySprite::~CVWGeometrySprite()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVWGeometrySprite::~CVWGeometrySprite, this 0x%0x\n", this);
#endif

	Terminate();
}

STDMETHODIMP CVWGeometrySprite::Terminate(void)
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CVWGeometrySprite::Terminate, this 0x%0x\n", this);
#endif

	CVWGeometrySpriteBase::Terminate();

	SAFERELEASE(m_pd2drmfAccessoryScene);
	SAFERELEASE(m_pINetFileManager);
	SAFERELEASE(m_pDirect3DRM);
	SAFERELEASE(m_pDirect2DRM);
	SAFERELEASE(m_pSpriteManager);

	return S_OK;
}

// use this for creating user visuals
HRESULT CVWGeometrySprite::CreateSprite()
{
	IDirect3DRMFrame		*prlFrame = NULL;
    LPDIRECT3DRMUSERVISUAL	prlUvis = NULL;
	LPDIRECT3DRMFACE		lpD3DRMFace = NULL;
	IPropertyMap			*pWornAccessories = NULL;
	IThing			*pThing = NULL;
	BSTR			bstrFinalPath = NULL;
	static CComBSTR	bstrWornAccessories("WornAccessories");
	VARIANT_BOOL	bIsValid, bLastItem;
	CComBSTR		bstrFullURL, bstrAccessoryGraphic, bstrID;
	COleVariant		var;
	float			fPosX, fPosY;
	long			lWornAccessories;
	HRESULT hr = S_OK;

	// Create the 3D frame for the object.
	m_pDirect3DRM->CreateFrame(NULL, &prlFrame);
	
	// now create the user visual itself
	m_pDirect3DRM->CreateUserVisual(CVWGeometrySprite::UpdateCallBack, (void*)this, &prlUvis);
	prlFrame->AddVisual( prlUvis );
	m_pSpriteFrame = prlFrame;
	SAFEADDREF(prlFrame);
	m_pDirect3DRM->CreateMeshBuilder(&m_lpd3drmMeshBuilder);
	m_lpd3drmMeshBuilder->AddVertex(D3DVALUE(-(SPRITEWIDTH/2.0)), D3DVALUE(-(SPRITEHEIGHT/2.0)), D3DVALUE( 0.0));
	m_lpd3drmMeshBuilder->AddVertex(D3DVALUE( (SPRITEWIDTH/2.0)), D3DVALUE(-(SPRITEHEIGHT/2.0)), D3DVALUE( 0.0));
	m_lpd3drmMeshBuilder->AddVertex(D3DVALUE( (SPRITEWIDTH/2.0)), D3DVALUE( (SPRITEHEIGHT/2.0)), D3DVALUE( 0.0));
	m_lpd3drmMeshBuilder->AddVertex(D3DVALUE(-(SPRITEWIDTH/2.0)), D3DVALUE( (SPRITEHEIGHT/2.0)), D3DVALUE( 0.0));
	m_lpd3drmMeshBuilder->AddNormal(D3DVALUE( 0.0), D3DVALUE( 0.0), D3DVALUE( -1.0));
	m_lpd3drmMeshBuilder->SetTextureCoordinates(0,  D3DVALUE( 1.0), D3DVALUE( 1.0));
	m_lpd3drmMeshBuilder->SetTextureCoordinates(1,  D3DVALUE( 0.0), D3DVALUE( 1.0));
	m_lpd3drmMeshBuilder->SetTextureCoordinates(2,  D3DVALUE( 0.0), D3DVALUE( 0.0));
	m_lpd3drmMeshBuilder->SetTextureCoordinates(3,  D3DVALUE( 1.0), D3DVALUE( 0.0));
	m_lpd3drmMeshBuilder->CreateFace(&lpD3DRMFace);
	lpD3DRMFace->AddVertexAndNormalIndexed(0, 0);
	lpD3DRMFace->AddVertexAndNormalIndexed(1, 0);
	lpD3DRMFace->AddVertexAndNormalIndexed(2, 0);
	lpD3DRMFace->AddVertexAndNormalIndexed(3, 0);
	m_lpd3drmMeshBuilder->SetQuality(D3DRMRENDER_UNLITFLAT);
	prlFrame->AddVisual(m_lpd3drmMeshBuilder);

	// Now, create the VWFrame for this geometry which will be returned to the caller.
	// After this point, we will not return a failure since the geometry frame has been
	// created and can be used.
	if (FAILED(hr = m_pWorld->CreateObjectPropertyExt(CLSID_VWFrame, NULL, (IObjectProperty**)&m_pvwfGeometry)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pvwfGeometry->put_Frame3D(prlFrame)))
		goto ERROR_ENCOUNTERED;
		
	// now see whether to use name tag: sdrucker
	m_pSecurity->get_Instance(&pThing);

	// Add accessories to the sprite (if any).
	// Get the WornAccessories property list and cycle through it adding the accessories
	// to this sprite.
	if (FAILED(hr = pThing->IsValidProperty(bstrWornAccessories, &bIsValid)))
		goto ERROR_ENCOUNTERED;
	if (bIsValid)
	{
		if (FAILED(hr = pThing->get_ObjectProperty(bstrWornAccessories, (IObjectProperty**)&pWornAccessories)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = pWornAccessories->get_Count(&lWornAccessories)))
			goto ERROR_ENCOUNTERED;
		if (lWornAccessories > 0)
		{
			if (FAILED(hr = pWornAccessories->FirstItem(&bstrID.m_str, &var, &bLastItem)))
				goto ERROR_ENCOUNTERED;
			do
			{
				((IVWAccessoryData*)var.pdispVal)->get_AccessoryGraphic(&bstrAccessoryGraphic.m_str);
				((IVWAccessoryData*)var.pdispVal)->GetPos(&fPosX, &fPosY);
				AddAccessory(bstrAccessoryGraphic, atol(CString(bstrID)), fPosX, fPosY);
				if (!bLastItem)
				{
					if (FAILED(hr = pWornAccessories->NextItem(&bstrID.m_str, &var, &bLastItem)))
						goto ERROR_ENCOUNTERED;
				}
			} 
			while (bLastItem == VARIANT_FALSE);
		}
	}

	if (FAILED(hr = InitializeSprite()))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:

	SAFERELEASE(prlUvis);
	SAFERELEASE(prlFrame);
	SAFERELEASE(lpD3DRMFace);
	SAFERELEASE(pThing);
	SAFERELEASE(pWornAccessories);
	SAFEFREESTRING(bstrFinalPath);

	return hr;
}

STDMETHODIMP CVWGeometrySprite::InitializeSprite(VARIANT_BOOL bForceDownload)
{
	CComPtr<IThing>				pThing, pUser, pGlobal;
	CComPtr<IDirect3DRMFrame>	pd3drmfGeometry;
	CComBSTR					bstrImageFileURL;
	VARIANT_BOOL				bIsAvatar = VARIANT_FALSE, bFound, bAutoDownload = VARIANT_TRUE;
	char						szExt[_MAX_EXT];
	CString						strImageURLToLoad;
	HRESULT						hr = S_OK;

	// First, uninitialize the sprite.
	m_pvwfGeometry->get_Frame3D(&pd3drmfGeometry);
	pd3drmfGeometry->DeleteVisual(m_lpd3drmMeshBuilder);
	SAFERELEASE(m_pSpriteImage);
	SAFERELEASE(m_pddsTexture);
	m_lpd3drmMeshBuilder->SetTexture(NULL);
	if (m_pd3drmTexture)
	{
		m_pDirect2DRM->AddTextureForRelease(m_pd3drmTexture);
		m_pd3drmTexture = NULL;
	}
	m_bstrImageFileURL.Empty();
	m_bstrImageFilePath.Empty();
	m_bThumbnailMode = VARIANT_FALSE;
	m_bstrThumbnailName = szDefaultThumbNailName;

	// Create the sprite manager if we haven't already.
	if (!m_pSpriteManager)
	{
		m_pSpriteManager = new CComSpriteManager;
		SAFEADDREF(m_pSpriteManager);
	}

	// Is this thing an avatar?
	if (m_pSecurity)
		m_pSecurity->get_Instance(&pThing);
	if (pThing)
		pThing->IsOfType(CComBSTR("Avatar"), &bIsAvatar);

	m_pWorld->get_Global(&pGlobal);
	if (pGlobal)
		pGlobal->get_BOOL(CComBSTR("EnableAutoDownload"), &bAutoDownload);

	// Always download the user's avatar.
	m_pWorld->get_User(&pUser);
	if (pThing == pUser)
		bForceDownload = VARIANT_TRUE;

	// See if we need to read a sprite file.
	_splitpath(CString(m_bstrGeometryName), NULL, NULL, NULL, szExt );
	strImageURLToLoad = m_bstrGeometryName;

	// If its an SPR file read it. If not and this is an avatar, or if we fail to read it 
	// and this is an avtar then read the default sprite file.
	if ((stricmp(szExt, ".SPR") == 0 && 
		 FAILED(hr = ReadSpriteFile(m_bstrGeometryName, VARIANT_TRUE))) || 
		(stricmp(szExt, ".SPR") != 0 && bIsAvatar))
	{
		if (!bIsAvatar || FAILED(hr = ReadSpriteFile(m_bstrDefaultSpriteFile, FAILED(hr))))
			return hr;
	}
	if ((stricmp(szExt, ".SPR") == 0) && SUCCEEDED(hr))
		strImageURLToLoad = m_bstrImageFileURL;

	// Now try to download the image file.
	strImageURLToLoad.MakeLower();
	if (FAILED(m_pINetFileManager->FindCachedFile(CComBSTR(strImageURLToLoad), &bstrImageFileURL.m_str, &m_bstrImageFilePath.m_str, &bFound)) || !bFound)
	{
		if (bForceDownload || bAutoDownload)
		{
			// Get remote image file asyncronously.
			// Cancel previous download (if any).
			char buff[20];
			ltoa((DWORD)this,buff,10);
			m_pINetFileManager->FileDLDone(CComBSTR(CString(buff)));
			if (FAILED(hr = m_pINetFileManager->GetFileAsync(CComBSTR(strImageURLToLoad),
															 (long)this,
															 (IIFMCallback*)this, 
															 VARIANT_FALSE)))
			m_pWorld->Report(CComBSTR(CString("VWGeometrySprite: Failed to set up file download for file: ") + strImageURLToLoad), -1);
			
			// OnDone will do the rest after the file is downloaded.
			return hr;
		}
		else
		{
			// Do not have image file locally, go to thumbnail mode.
			m_bThumbnailMode = VARIANT_TRUE;
			if (FAILED(hr = m_pINetFileManager->FindCachedFile(CComBSTR(szThumbnailBackground), &bstrImageFileURL.m_str, &m_bstrImageFilePath.m_str, &bFound)) || !bFound)
				return hr;
		}
	}
	
	// Store the URL for reloading purposes.
	m_bstrImageFileURL = bstrImageFileURL;

	// Load the image
	if (FAILED(hr = m_pDirect2DRM->CreateImageFromImageFile(m_bstrImageFilePath, 
						flagCREATE_IGNOREANIMATION, &(m_pSpriteImage))) ||
		FAILED(hr = m_pSpriteImage->ExportD3DTexture(m_pDirect3DRM, &m_pddsTexture, &m_pd3drmTexture)))
	{
		m_pWorld->Report(CComBSTR(CString("CVWGeometrySprite::InitializeSprite: Failed to create texture from image file: ") + CString(m_bstrImageFilePath)), -1);
		return hr;
	}

	// If we didn't load a sprite file we will need to create the data for one.
	if (!bIsAvatar && stricmp(szExt, ".SPR") != 0)
	{
		// We need to create sprite file data.
		if (FAILED(hr = m_pSpriteManager->CreateSpriteFileData(m_pSpriteImage, -1)))
			return hr;
	}

	// Finally once we know nothing has failed, set the texture.
	m_lpd3drmMeshBuilder->SetTexture(m_pd3drmTexture);
	m_lpd3drmMeshBuilder->SetPerspective(TRUE);
	m_iSpriteImageFrame = -1;
	pd3drmfGeometry->AddVisual(m_lpd3drmMeshBuilder);

	if (m_bThumbnailMode)
		InitializeThumbnailImage();

	return hr;
}

STDMETHODIMP CVWGeometrySprite::ReadSpriteFile(BSTR bstrSpriteFile, VARIANT_BOOL bGetImageFile)
{
	CComBSTR		bstrSpriteFileURL, bstrSpriteFilePath, bstrError, bstrThumbnailName;
	VARIANT_BOOL	bFound;
	int				iFwdSlash, iBackSlash;
	HRESULT			hr = S_OK;

	// Download the necessary sprite file syncrhonously.
	if ((FAILED(hr = m_pINetFileManager->FindCachedFile(bstrSpriteFile, &bstrSpriteFileURL.m_str, &bstrSpriteFilePath.m_str, &bFound)) || !bFound) &&
		FAILED(hr = m_pINetFileManager->GetFileEx(bstrSpriteFile, VARIANT_FALSE, &bstrSpriteFileURL.m_str, &bstrSpriteFilePath.m_str)))
	{
		m_pWorld->Report(CComBSTR(CString("VWGeometrySprite: Failed to download file: ") + CString(bstrSpriteFile)), -1);
		goto ERROR_ENCOUNTERED;
	}

	// Read the sprite file.
	if (FAILED(hr = m_pSpriteManager->ReadSpriteFile(bstrSpriteFilePath, &bstrError.m_str)))
	{
		m_pWorld->Report(bstrError, -1);
		goto ERROR_ENCOUNTERED;
	}

	if (!bGetImageFile)
		goto ERROR_ENCOUNTERED;

	// Extract the thumbnail image (if there is one) from the sprite file.
	m_pSpriteManager->get_ThumbNailImage(&bstrThumbnailName.m_str);

	// The thumbnail image file should be in the same URL as the sprite file, unless it is a 
	// full URL.
	if (bstrThumbnailName.Length() != 0)
	{
		m_bstrThumbnailName = bstrThumbnailName;
		iFwdSlash = CString(bstrSpriteFileURL).ReverseFind('/');
		iBackSlash = CString(bstrSpriteFileURL).ReverseFind('\\');
		if (iBackSlash > iFwdSlash)
			m_bstrThumbnailName = CString(bstrSpriteFileURL).Left(iBackSlash+1) + CString(m_bstrThumbnailName);
		else
			m_bstrThumbnailName = CString(bstrSpriteFileURL).Left(iFwdSlash+1) + CString(m_bstrThumbnailName);
	}

	
	// Extract the image file name from the sprite file.
	m_pSpriteManager->get_ImageFile(&m_bstrImageFileURL.m_str);

	// The image file should be in the same URL as the sprite file, unless it is a 
	// full URL.
	if (m_bstrImageFileURL.Length() != 0 && CString(m_bstrImageFileURL).Find("://") == -1)
	{
		iFwdSlash = CString(bstrSpriteFileURL).ReverseFind('/');
		iBackSlash = CString(bstrSpriteFileURL).ReverseFind('\\');
		if (iBackSlash > iFwdSlash)
			m_bstrImageFileURL = CString(bstrSpriteFileURL).Left(iBackSlash+1) + CString(m_bstrImageFileURL);
		else
			m_bstrImageFileURL = CString(bstrSpriteFileURL).Left(iFwdSlash+1) + CString(m_bstrImageFileURL);
	}

ERROR_ENCOUNTERED:

	return hr;
}
		
STDMETHODIMP CVWGeometrySprite::InitializeThumbnailImage()
{
	IDirect2DRMImage	*pThumbnailImage = NULL;
	BSTR	bstrFinalPath = NULL;
	DWORD	dwThumbnailWidth, dwThumbnailHeight, dwSpriteWidth, dwSpriteHeight;
	CComBSTR	bstrFullURL;
	VARIANT_BOOL	bFound;
	HRESULT	hr = S_OK;
	
	SAFERELEASE(m_pThumbnailImage);
	if ((BSTR)m_bstrGeometryName == NULL || m_bstrGeometryName.Length() == 0)
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = m_pINetFileManager->FindCachedFile(m_bstrThumbnailName, &bstrFullURL.m_str, &bstrFinalPath, &bFound)) || !bFound)
	{
		if (FAILED(hr = m_pINetFileManager->GetFileEx(m_bstrThumbnailName, VARIANT_FALSE, &bstrFullURL.m_str, &bstrFinalPath)))
		{
			m_pWorld->Report(CComBSTR(CString("CVWGeometrySprite::InitializeThumbnailImage: Failed to download file: ") + CString(m_bstrThumbnailName)), -1);
			goto ERROR_ENCOUNTERED;
		}
	}

	if (FAILED(hr = m_pDirect2DRM->CreateImageFromImageFile(bstrFinalPath, 
						flagCREATE_IGNOREANIMATION, &pThumbnailImage)))
		goto ERROR_ENCOUNTERED;

	pThumbnailImage->get_Width(&dwThumbnailWidth);
	pThumbnailImage->get_Height(&dwThumbnailHeight);
	m_pSpriteImage->get_Width(&dwSpriteWidth);
	m_pSpriteImage->get_Height(&dwSpriteHeight);
	m_rectThumbnailSrc.left = 0.0;
	m_rectThumbnailSrc.top = 0.0;
	m_rectThumbnailSrc.right = (float)dwThumbnailWidth;
	m_rectThumbnailSrc.bottom = (float)dwThumbnailHeight;
	m_rectThumbnailDst.left = (float)((float)dwSpriteWidth/2.0 - (float)dwThumbnailWidth/2.0);
	m_rectThumbnailDst.top = (float)((float)dwSpriteHeight/2.0 - (float)dwThumbnailHeight/2.0);
	m_rectThumbnailDst.right = (float)((float)dwSpriteWidth/2.0 + (float)dwThumbnailWidth/2.0);
	m_rectThumbnailDst.bottom = (float)((float)dwSpriteHeight/2.0 + (float)dwThumbnailHeight/2.0);
	m_pThumbnailImage = pThumbnailImage;
	m_iSpriteImageFrame = -1;

ERROR_ENCOUNTERED:

	SAFEFREESTRING(bstrFinalPath);

	return hr;
}

STDMETHODIMP CVWGeometrySprite::OnDone_T2(HRESULT hresult, BSTR bstrFinalPath, long lCookie, BSTR bstrFullURL)
{
	CComPtr<IDirect3DRMFrame>	pd3drmfGeometry;
	CComPtr<IThing>				pThing;
	VARIANT_BOOL				bIsAvatar = VARIANT_FALSE;
	CComBSTR					bstrGeometryName;
	char						szExt[_MAX_EXT];
	HRESULT hr = S_OK;
	
	// Is this thing an avatar?
	if (m_pSecurity)
		m_pSecurity->get_Instance(&pThing);
	if (pThing)
		pThing->IsOfType(CComBSTR("Avatar"), &bIsAvatar);

	if (FAILED(hresult))
	{
		hresult = S_OK;
		if (bIsAvatar)
		{
			// Avatars should always default back to the default sprite file.
			if (m_bstrDefaultSpriteFile.Length() != 0 &&
				!(CString(m_bstrGeometryName) == CString(m_bstrDefaultSpriteFile)))
			{
				bstrGeometryName = m_bstrGeometryName;
				m_bstrGeometryName = m_bstrDefaultSpriteFile;
				InitializeSprite(VARIANT_TRUE);
				m_bstrGeometryName = bstrGeometryName;
			}
		}
		goto ERROR_ENCOUNTERED;
	}
	
	if (!m_lpd3drmMeshBuilder)
		goto ERROR_ENCOUNTERED;  // We must have released the sprite.

	// Store the Final Path and URL for reloading purposes.
	m_bstrImageFilePath = bstrFinalPath;
	m_bstrImageFileURL = bstrFullURL;

	// Load the image
	if (FAILED(hr = m_pDirect2DRM->CreateImageFromImageFile(m_bstrImageFilePath, 
						flagCREATE_IGNOREANIMATION, &(m_pSpriteImage))) ||
		FAILED(hr = m_pSpriteImage->ExportD3DTexture(m_pDirect3DRM, &m_pddsTexture, &m_pd3drmTexture)))
	{
		m_pWorld->Report(CComBSTR(CString("CVWGeometrySprite::OnDone_T2: Failed to create texture from image file: ") + CString(m_bstrImageFilePath)), -1);
		goto ERROR_ENCOUNTERED;
	}

	// If we didn't load a sprite file we will need to create the data for one.
	_splitpath(CString(bstrFinalPath), NULL, NULL, NULL, szExt);
	if (!bIsAvatar && stricmp(szExt, ".SPR") != 0)
	{
		// We need to create sprite file data.
		if (FAILED(hr = m_pSpriteManager->CreateSpriteFileData(m_pSpriteImage, -1)))
			goto ERROR_ENCOUNTERED;
	}

	// Finally once we know nothing has failed, set the texture.
	m_lpd3drmMeshBuilder->SetTexture(m_pd3drmTexture);
	m_lpd3drmMeshBuilder->SetPerspective(TRUE);
	m_iSpriteImageFrame = -1;
	m_pvwfGeometry->get_Frame3D(&pd3drmfGeometry);
	pd3drmfGeometry->AddVisual(m_lpd3drmMeshBuilder);

ERROR_ENCOUNTERED:

	return hr;
}

STDMETHODIMP CVWGeometrySprite::ComposeGeometry(IVWFrame **ppvwfTransform)
{
	HRESULT			hr = S_OK;
	CComPtr<IThing>	pGlobal;
	CComPtr<IThing>	pThing;
	
	// REVIEW: For now assume we always return 3D geometry.

	if (!m_pDirect3DRM)
	{
		if (FAILED(hr = m_pWorld->get_Tool(CComBSTR("Direct3DRM"), (IUnknown**)&m_pDirect3DRM)))
			goto ERROR_ENCOUNTERED;
	}

	if (!m_pDirect2DRM)
	{
		if (FAILED(hr = m_pWorld->get_Tool(CComBSTR("Direct2DRM"), (IUnknown**)&m_pDirect2DRM)))
			goto ERROR_ENCOUNTERED;
	}

	if (!m_pINetFileManager)
	{
		if (FAILED(hr = m_pWorld->get_Tool(CComBSTR("Inetfile"),(IUnknown**) &m_pINetFileManager)))
			goto ERROR_ENCOUNTERED;
	}

	if (m_bstrDefaultSpriteFile.Length() == 0)
	{
		if (FAILED(hr = m_pWorld->get_Global(&pGlobal)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = pGlobal->get_String(CComBSTR("DefaultSpriteFile"), &m_bstrDefaultSpriteFile.m_str)))
			goto ERROR_ENCOUNTERED;
	}

	if (m_pvwfTransform)
	{
		*ppvwfTransform = m_pvwfTransform;
		m_pvwfTransform->AddRef();
		goto ERROR_ENCOUNTERED;
	}

	// Create an empty 'T' node.
	if (FAILED(hr = m_pWorld->CreateObjectPropertyExt(CLSID_VWFrame, NULL, (IObjectProperty**)&m_pvwfTransform)))
		goto ERROR_ENCOUNTERED;
	m_pvwfTransform->put_GeometryObject((IVWGeometry*)this);
	if (m_pSecurity)
	{
		m_pSecurity->get_Instance(&pThing);
		m_pvwfTransform->put_Thing(pThing);
	}
	m_pvwfTransform->AddRef();
	*ppvwfTransform = m_pvwfTransform;

	if (!m_bOccluded)
	{
		if (FAILED(hr = CreateSprite()))
		{
			// Hmmm, failed loading the geometry. We have a good T node to pass back
			// so don't error out, just report it.
			m_pWorld->Report(CComBSTR(CString("CVWGeometrySprite::ComposeGeometry: Failed to load geometry: ") + CString(m_bstrGeometryName.m_str)), -1);
			hr = S_OK;
		}
		if (m_pvwfGeometry && FAILED(hr = m_pvwfTransform->AddChild(m_pvwfGeometry)))
			goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:

	return hr;
}

STDMETHODIMP CVWGeometrySprite::ReleaseGeometry()
{
	ReleaseSprite();

	if (!m_pvwfTransform)
		return E_FAIL;

	// Use stack variables since this object may be deleted when we remove the VWFrame's
	// reference to it.
	IVWFrame *pvwfTransform = m_pvwfTransform;
	m_pvwfTransform = NULL;

	pvwfTransform->put_GeometryObject(NULL);
	SAFERELEASE(pvwfTransform);

	return S_OK;
}

STDMETHODIMP CVWGeometrySprite::ReleaseSprite()
{
	//SAFERELEASE(m_pSpriteManager);
	SAFERELEASE(m_pddsTexture);
	if (m_lpd3drmMeshBuilder)
		m_lpd3drmMeshBuilder->SetTexture(NULL);
	SAFERELEASE(m_lpd3drmMeshBuilder);
	if (m_pd3drmTexture)
	{
		m_pDirect2DRM->AddTextureForRelease(m_pd3drmTexture);
		m_pd3drmTexture = NULL;
	}
	SAFERELEASE(m_pSpriteFrame);
	SAFERELEASE(m_pSpriteImage);
	SAFERELEASE(m_pThumbnailImage);
	SAFERELEASE(m_pvwfGeometry);
	m_bstrImageFileURL.Empty();
	m_bstrImageFilePath.Empty();

	return S_OK;
}

STDMETHODIMP CVWGeometrySprite::put_GeometryName(BSTR bstr)
{
	HRESULT hr = S_OK;
	IVWFrame *pvwfGeometry = NULL;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CVWGeometrySprite::put_GeometryName\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
		ValidatePropertyName(psbits, CString(m_bstrPropertyName), "CVWGeometrySprite::put_GeometryName", "VWPROPERTY");
#endif	
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CVWGeometrySprite::put_GeometryName: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		// Actually set the new name.
		m_bstrGeometryName = bstr;

		if (m_pvwfTransform && !m_bOccluded)
			InitializeSprite();
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
		// notify client/server and world of property change
		if (m_pParent)
		{
			if (FAILED(hr = m_pParent->NotifyObjectPropertyChanged(this, glPutGeometryName, CComVariant(bstr))))
				goto ERROR_ENCOUNTERED;
		}
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	SAFERELEASE(pvwfGeometry);

	return hr;
}

STDMETHODIMP CVWGeometrySprite::get_GeometryName(BSTR* pbstr)
{
	*pbstr = m_bstrGeometryName.Copy();
	return S_OK;
}

STDMETHODIMP CVWGeometrySprite::get_NamedGesture(BSTR bstrGesture, int *pGestureNum)
{
	HRESULT	hr = S_OK;
	*pGestureNum = 0;

	if (m_pSpriteManager)
		hr = m_pSpriteManager->get_NamedGesture(bstrGesture, pGestureNum);

	return hr;
}

STDMETHODIMP CVWGeometrySprite::get_GestureText(BSTR bstrGesture, BSTR *pbstrText)
{
	HRESULT	hr = S_OK;
	*pbstrText = NULL;

	if (m_pSpriteManager)
		hr = m_pSpriteManager->get_GestureText(bstrGesture, pbstrText);

	return hr;
}


STDMETHODIMP CVWGeometrySprite::GetNamedGestures(IPropertyList **ppGestureNameList)
{
	HRESULT	hr = S_OK;
	*ppGestureNameList = NULL;

	if (m_pSpriteManager)
	{
		if (FAILED(hr = CreatePropertyList(m_pWorld, ppGestureNameList)))
			return hr;
		hr = m_pSpriteManager->GetNamedGestures(*ppGestureNameList);
	}

	return hr;
}


STDMETHODIMP CVWGeometrySprite::get_Frame(IVWFrame **ppVWFrame)
{
	*ppVWFrame = m_pvwfTransform;
	SAFEADDREF(*ppVWFrame);
	return S_OK;
}

STDMETHODIMP CVWGeometrySprite::put_Frame(IVWFrame *pVWFrame)
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

STDMETHODIMP CVWGeometrySprite::get_GeometryFrame(IVWFrame **ppVWFrame)
{
	*ppVWFrame = m_pvwfGeometry;
	SAFEADDREF(*ppVWFrame);
	return S_OK;
}

STDMETHODIMP CVWGeometrySprite::put_Occluded(VARIANT_BOOL bOccluded)
{
	HRESULT hr = S_OK, hr2 = S_OK;

	if (bOccluded != m_bOccluded && m_pvwfTransform)
	{
		if (!bOccluded)
		{
			hr = CreateSprite();
			if (!m_pvwfGeometry)
				goto ERROR_ENCOUNTERED;
			if (FAILED(hr2 = m_pvwfTransform->AddChild(m_pvwfGeometry)))
				goto ERROR_ENCOUNTERED;
			// Seem to need to ensure position is reset here.
			if (FAILED(hr2 = m_pvwfGeometry->SetPositionEx(m_pvwfTransform, 0.0, 0.0, 0.0)))
				goto ERROR_ENCOUNTERED;
		}
		else
		{
			if (FAILED(hr = m_pvwfTransform->DeleteChild(m_pvwfGeometry)))
				goto ERROR_ENCOUNTERED;
			ReleaseSprite();
		}
	}
	m_bOccluded = bOccluded;

ERROR_ENCOUNTERED:

	if (SUCCEEDED(hr) && FAILED(hr2))
		hr = hr2;
	return hr;
}

STDMETHODIMP CVWGeometrySprite::put_CurrentGesture(int iGesture)
{
	if (m_pSpriteManager)
		return m_pSpriteManager->put_CurrentGesture(iGesture);
	return E_FAIL;
}

STDMETHODIMP CVWGeometrySprite::get_CurrentGesture(int* piGesture)
{
	if (m_pSpriteManager)
		return m_pSpriteManager->get_CurrentGesture(piGesture);
	return E_FAIL;
}

STDMETHODIMP CVWGeometrySprite::get_ImageFileName(BSTR* pbstrImageFileName)
{
	*pbstrImageFileName = m_bstrImageFilePath.Copy();
	return S_OK;
}

STDMETHODIMP CVWGeometrySprite::get_ThumbnailName(BSTR* pbstr)
{
	HRESULT hr = S_OK;
	if (!SAFE_CHECKSECURITY( m_pWorld, m_pSecurity, PS_READ))
	{
		hr = VWOBJECT_E_PERMISSION_DENIED;
		goto ERROR_ENCOUNTERED;
	}

	ASSERT( pbstr );
	*pbstr = m_bstrThumbnailName.Copy();

ERROR_ENCOUNTERED:

	return hr;
}

STDMETHODIMP CVWGeometrySprite::DownloadFullImage()
{
	InitializeSprite(VARIANT_TRUE);
	return S_OK;
}

STDMETHODIMP CVWGeometrySprite::ReloadImage()
{
	HRESULT hr = S_OK;
	long psbits = 0;
	VARIANT_BOOL bShouldExecute = VARIANT_TRUE;
	VARIANT_BOOL bShouldRemote = VARIANT_FALSE;
	VARIANT_BOOL bShouldNotify = VARIANT_FALSE;
	CComBSTR	bstrFullURL;
	CComPtr<IDirect3DRMFrame>	pd3drmfGeometry;
	DISPPARAMS	dispparams;

	GetMarshallingFlags(&bShouldExecute, &bShouldRemote, &bShouldNotify);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", VWT_METHOD, "CVWGeometrySprite::ReloadImage\n");
#endif

	if (bShouldRemote == VARIANT_TRUE)
	{
		GetMarshallingBits(&psbits, &bShouldExecute, &bShouldRemote, &bShouldNotify);
	}

	if (bShouldExecute == VARIANT_TRUE)
	{
		if (m_pParent)
		{
			if (FAILED(m_pParent->CheckPropertySecurityExt(m_bstrPropertyName, PS_WRITE)))
			{
				VWTRACE(m_pWorld, "VWOBJECT", VWT_ERROR, "CVWGeometrySprite::ReloadImage: security violation (PS_WRITE)\n");
				hr = VWOBJECT_E_PERMISSION_DENIED;
				goto ERROR_ENCOUNTERED;
			}
		}

		if (m_pvwfTransform && !m_bOccluded)
		{
			// Tell d2d to release the image file from its cache
			m_pvwfGeometry->get_Frame3D(&pd3drmfGeometry);
			pd3drmfGeometry->DeleteVisual(m_lpd3drmMeshBuilder);
			SAFERELEASE(m_pddsTexture);
			if (FAILED(m_lpd3drmMeshBuilder->SetTexture(NULL)))
				goto ERROR_ENCOUNTERED;
			if (m_pd3drmTexture)
			{
				m_pDirect2DRM->AddTextureForRelease(m_pd3drmTexture);
				m_pd3drmTexture = NULL;
			}
			SAFERELEASE(m_pSpriteImage);
			if (FAILED(m_pDirect2DRM->ReleaseImage(m_bstrImageFilePath)))
				goto ERROR_ENCOUNTERED;

			// Get the image file.
			// REVIEW: we need to assure that we don't get a cached copy!
			// Works fine for FILE:// based URLs
			if (FAILED(m_pINetFileManager->GetFileEx(m_bstrImageFileURL, VARIANT_FALSE, &bstrFullURL.m_str, &m_bstrImageFilePath.m_str)))
			{
				m_pWorld->Report(CComBSTR(CString("CVWGeometrySprite::ReloadImage: Failed to re-download file: ") + CString(m_bstrImageFileURL)), -1);
				goto ERROR_ENCOUNTERED;
			}

			// (re)-load the image
			if (FAILED(m_pDirect2DRM->CreateImageFromImageFile(m_bstrImageFilePath, 
					   flagCREATE_IGNOREANIMATION, &(m_pSpriteImage))) ||
				FAILED(m_pSpriteImage->ExportD3DTexture(m_pDirect3DRM, &m_pddsTexture, &m_pd3drmTexture)))
			{
				m_pWorld->Report(CComBSTR(CString("CVWGeometrySprite::ReloadImage: Failed to create texture from image file: ") + CString(m_bstrImageFilePath)), -1);
				goto ERROR_ENCOUNTERED;
			}
			m_lpd3drmMeshBuilder->SetTexture(m_pd3drmTexture);
			m_lpd3drmMeshBuilder->SetPerspective(TRUE);
			pd3drmfGeometry->AddVisual(m_lpd3drmMeshBuilder);
		}
	}

	if (bShouldRemote == VARIANT_TRUE)
	{
		InitializeDISPPARAMS(&dispparams, 0);
		hr = SendObjectPropertyCommand(psbits, DISPID_GEOMETRYSPRITE_RELOADIMAGE, DISPATCH_METHOD, &dispparams);
		TerminateDISPPARAMS(VARIANT_FALSE, &dispparams);
	}

	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:

	return hr;
}

STDMETHODIMP CVWGeometrySprite::get_AccessoryScene(LPDIRECT2DRMFRAME *ppFrame)
{
	*ppFrame = m_pd2drmfAccessoryScene;
	SAFEADDREF(m_pd2drmfAccessoryScene);
	return S_OK;
}

STDMETHODIMP CVWGeometrySprite::AddAccessory(BSTR bstrImageFileName, long lID, float fPosX, float fPosY)
{
	IDirect2DRMFrame	*pFrame = NULL;
	IDirect2DRMImage	*pImage = NULL;
	BSTR				bstrLocalName = NULL;
	CComBSTR	bstrFullURL;
	VARIANT_BOOL		bFound;
	HRESULT hr = S_OK;
	static CComBSTR bstrClientGeometry("ClientGeometry");

	if (!m_pDirect2DRM)
	{
		// Can't have called ComposeGeometry yet!
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	// Don't do accessories for client geometries.
	if (m_bstrPropertyName == bstrClientGeometry)
		goto ERROR_ENCOUNTERED;

	if (!m_pd2drmfAccessoryScene)
	{
		if (FAILED(hr = m_pDirect2DRM->CreateFrame(NULL, &m_pd2drmfAccessoryScene)))
			goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = m_pDirect2DRM->CreateFrame(m_pd2drmfAccessoryScene, &pFrame)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pFrame->SetTranslation(fPosX, fPosY)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = m_pINetFileManager->FindCachedFile(bstrImageFileName, &bstrFullURL.m_str, &bstrLocalName, &bFound)) || !bFound)
	{
		if (FAILED(hr = m_pINetFileManager->GetFileEx(bstrImageFileName, VARIANT_FALSE, &bstrFullURL.m_str, &bstrLocalName)))
		{
			m_pWorld->Report(CComBSTR(CString("CVWGeometrySprite::AddAccessory: Failed to download file: ") + CString(bstrImageFileName)), -1);
			goto ERROR_ENCOUNTERED;
		}
	}
	if (FAILED(hr = m_pDirect2DRM->CreateImageFromImageFile(bstrLocalName, 
						flagCREATE_IGNOREANIMATION, &pImage)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pFrame->AddVisual(pImage)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pFrame->put_AppData(lID)))
		goto ERROR_ENCOUNTERED;
		
	// We need to ensure that the accessories are marked as dirty.
	OnAccessoriesChanged();

ERROR_ENCOUNTERED:

	SAFEFREESTRING(bstrLocalName);
	SAFERELEASE(pFrame);
	SAFERELEASE(pImage);

	return hr;
}

STDMETHODIMP CVWGeometrySprite::RemoveAccessory(long lID)
{
	IDirect2DRMFrame	*pFrame = NULL;
	IDirect2DRMArray	*pChildren = NULL;
	DWORD				dwChildren;
	HRESULT hr = S_OK;
	static CComBSTR bstrClientGeometry("ClientGeometry");

	if (!m_pDirect2DRM || !m_pd2drmfAccessoryScene)
	{
		hr = E_FAIL;
		goto ERROR_ENCOUNTERED;
	}

	// Don't do accessories for client geometries.
	if (m_bstrPropertyName == bstrClientGeometry)
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = AccessoryFrameFromID(lID, &pFrame)))
		goto ERROR_ENCOUNTERED;
	
	if (FAILED(hr = m_pd2drmfAccessoryScene->DeleteChild(pFrame)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = m_pd2drmfAccessoryScene->get_Children(&pChildren)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pChildren->get_Size(&dwChildren)))
		goto ERROR_ENCOUNTERED;
	if (dwChildren == 0)
		SAFERELEASE(m_pd2drmfAccessoryScene);

	// We need to ensure that the accessories are marked as dirty.
	OnAccessoriesChanged();

ERROR_ENCOUNTERED:

	SAFERELEASE(pFrame);
	SAFERELEASE(pChildren);

	return hr;
}

STDMETHODIMP CVWGeometrySprite::OnAccessoriesChanged()
{
	m_bAccessoriesDirty = TRUE;
	return S_OK;
}

STDMETHODIMP CVWGeometrySprite::AccessoryIDFromFrame(LPDIRECT2DRMFRAME pFrame, long *plID)
{
	pFrame->get_AppData((unsigned long*)plID);
	return S_OK;
}

STDMETHODIMP CVWGeometrySprite::AccessoryFrameFromID(long lID, LPDIRECT2DRMFRAME *ppFrame)
{
	IDirect2DRMArray	*pChildren = NULL;
	DWORD				dwChildren, dwType, dwAppData;
	int					iChild;
	HRESULT hr = S_OK;

	*ppFrame = NULL;

	if (!m_pd2drmfAccessoryScene)
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = m_pd2drmfAccessoryScene->get_Children(&pChildren)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pChildren->get_Size(&dwChildren)))
		goto ERROR_ENCOUNTERED;

	for (iChild = 0; iChild < (long)dwChildren; iChild++)
	{
		if (FAILED(hr = pChildren->GetElement(iChild, &dwType, (IDirect2DRMObject**)ppFrame)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = (*ppFrame)->get_AppData(&dwAppData)))
			goto ERROR_ENCOUNTERED;
		if ((long)dwAppData == lID)
		{
			break;
		}
		SAFERELEASE(*ppFrame);
	}

ERROR_ENCOUNTERED:

	SAFERELEASE(pChildren);

	return hr;
}

STDMETHODIMP CVWGeometrySprite::Marshall(DWORD dwOptions, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = CVWGeometrySpriteBase::Marshall( dwOptions, pbuffer )))
		return hr;

	ASSERT(pbuffer);

	hr = pbuffer->put_String(m_bstrGeometryName);

	return hr;
}

STDMETHODIMP CVWGeometrySprite::UnMarshall(IUnMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	// free alloced data
	m_bstrGeometryName.Empty();
	ReleaseSprite();
	if (m_pvwfTransform)
		m_pvwfTransform->put_GeometryObject(NULL);
	SAFERELEASE(m_pvwfTransform);

	if (FAILED(hr = CVWGeometrySpriteBase::UnMarshall( pbuffer )))
		return hr;

	ASSERT(pbuffer);

	hr = pbuffer->get_String(&m_bstrGeometryName.m_str);

	return hr;
}

STDMETHODIMP CVWGeometrySprite::MarshallToScript(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut)
{
	CString strOut;

	if (bstrContext==NULL || pbstrOut==NULL || pbAsVal==NULL)
		return E_INVALIDARG;

	strOut.Format(	"\tSet %s = World.CreateSpritedGeometry(\"%s\")\n",
					(LPCTSTR) CString(bstrContext),
					(LPCTSTR) CString(m_bstrGeometryName));

	*pbstrOut = strOut.AllocSysString();
	*pbAsVal = VARIANT_FALSE;

	return S_OK;
}


/*
 * A sprite is turned to always face the viewer
 *
 */
int CVWGeometrySprite::UpdateCallBack(LPDIRECT3DRMUSERVISUAL uvis,
									  void* arg,
									  D3DRMUSERVISUALREASON reason,
									  LPDIRECT3DRMDEVICE dev,
									  LPDIRECT3DRMVIEWPORT view)
{
    if (reason == D3DRMUSERVISUAL_RENDER)
	{
		LPDIRECT3DRMFRAME lpCamera = NULL, lpTransform = NULL;
		D3DVECTOR	dir, up, d3dvCameraPos;
		D3DVALUE	fTheta;
		int			iImageFrame;
		VARIANT_BOOL	bShowAccessories;

		CVWGeometrySprite *pcvwspInst = (CVWGeometrySprite*) arg;

		// If the basic geometry isn't created we can't do anything at all.
		if (!pcvwspInst->m_pvwfTransform || !pcvwspInst->m_pSpriteFrame)
			return TRUE;

		// Get the root frame of the sprite
		pcvwspInst->m_pvwfTransform->get_Frame3D(&lpTransform);

		//Check to see if this is an auto-facing sprite
		if (AXIS_NONE != pcvwspInst->m_iAxis)
		{
			BOOL bOrthgraphic = (view->GetProjection() == D3DRMPROJECT_ORTHOGRAPHIC);

			// get the camera
			if (FAILED(view->GetCamera(&lpCamera)))
				return DDERR_GENERIC;
 			
			// Turn the frame toward the camera
			lpCamera->GetPosition(lpTransform, &d3dvCameraPos);
			if (d3dvCameraPos.x == 0.0F && d3dvCameraPos.y == 0.0F && d3dvCameraPos.z == 0.0F)
			{
				d3dvCameraPos.x = 0.0F;
				d3dvCameraPos.y = 0.0F;
				d3dvCameraPos.z = 1.0F;
			}

			//Apply the constraint if needed
			if ( (pcvwspInst->m_iAxis != AXIS_ALL) && !bOrthgraphic)
			{
				if (pcvwspInst->m_iAxis & AXIS_X)
					d3dvCameraPos.x = 0.0F;
				if (pcvwspInst->m_iAxis & AXIS_Y )
					d3dvCameraPos.y = 0.0F;
				if (pcvwspInst->m_iAxis & AXIS_Z)
					d3dvCameraPos.z = 0.0F;
			}

			if (!d3dvCameraPos.x && !d3dvCameraPos.y && !d3dvCameraPos.z)
			{
				d3dvCameraPos.x = 1.0F;
				d3dvCameraPos.y = 0.0F;
				d3dvCameraPos.z = 0.0F;
			}
				up.x = 0.0F;
				up.y = 0.0F;
				up.z = 0.0F;

				//If our auto-facing axis is unconstrained or y axis
				if ( (AXIS_Y & pcvwspInst->m_iAxis) || (pcvwspInst->m_iAxis == AXIS_ALL) || bOrthgraphic )
					up.y = 1.0F;
				else if (AXIS_X & pcvwspInst->m_iAxis)
					up.x = 1.0F;
				else if (AXIS_Z & pcvwspInst->m_iAxis)
					up.z = 1.0F;

				D3DRMVectorNormalize(&d3dvCameraPos);

				pcvwspInst->m_pSpriteFrame->SetOrientation(lpTransform,
														   d3dvCameraPos.x,
														   d3dvCameraPos.y,
														   d3dvCameraPos.z,
														   up.x, up.y, up.z);
			
		}
		
		// compute which sprite->frame
		pcvwspInst->m_pSpriteFrame->GetOrientation(lpTransform, &dir, &up);

		// If theta = angle between orientation and Z axis, then:-
		// X component of orientation = sin theta and
		// Z component of orientation = cos theta.
		fTheta = (float)acos(dir.z); // (dir dot spritedir)

		// cross product of sprite_dir (1, 0, 0) and dir
		if (dir.x < 0.0) {
			fTheta = 6.283185307F - fTheta;
		}
		RADTODEG(fTheta);

		if (!pcvwspInst->m_pSpriteManager)
			return TRUE;

		// Find out which image frame should be displayed based on the current gesture frame
		// and theta.
		if (FAILED(pcvwspInst->m_pSpriteManager->CalcCurrentGestureFrame()) ||
			FAILED(pcvwspInst->m_pSpriteManager->GetImageFrame(fTheta, &iImageFrame, &bShowAccessories)))
		{
			// Couldn't work out the required image frame, just leave things as they were
			// and DON'T return an error to D3D.
			SAFERELEASE(lpCamera);
			SAFERELEASE(lpTransform);
			return D3D_OK;
		}

		if (!pcvwspInst->m_pSpriteImage ||
			!pcvwspInst->m_pddsTexture)
		{
			// Image/Texture creation must have failed, just leave things as they were
			// and DON'T return an error to D3D.
			SAFERELEASE(lpCamera);
			SAFERELEASE(lpTransform);
			return D3D_OK;
		}
		
		// Pick texture maps based on that!
		if (iImageFrame != pcvwspInst->m_iSpriteImageFrame ||
			(!bShowAccessories && pcvwspInst->m_bAccessoriesShown) ||
			(bShowAccessories && !pcvwspInst->m_bAccessoriesShown) ||
			pcvwspInst->m_bAccessoriesDirty)
		{
			pcvwspInst->m_iSpriteImageFrame = iImageFrame;
			pcvwspInst->m_pSpriteImage->put_VisibleImage(iImageFrame);
			pcvwspInst->m_pSpriteImage->BltToSurface(NULL, pcvwspInst->m_pddsTexture, 
				NULL, flagBLT_IGNORETRANSPARENCY);
			if (pcvwspInst->m_bThumbnailMode && pcvwspInst->m_pThumbnailImage)
			{
				pcvwspInst->m_pThumbnailImage->BltToSurface(&(pcvwspInst->m_rectThumbnailSrc),
															pcvwspInst->m_pddsTexture,
															&(pcvwspInst->m_rectThumbnailDst),
															0);
			}

			if (pcvwspInst->m_pd2drmfAccessoryScene && bShowAccessories)
				pcvwspInst->m_pd2drmfAccessoryScene->RenderToSurface(pcvwspInst->m_pddsTexture, NULL, 0);

			pcvwspInst->m_pd3drmTexture->Changed(TRUE, FALSE);

			if (bShowAccessories)
				pcvwspInst->m_bAccessoriesShown = VARIANT_TRUE;
			else
				pcvwspInst->m_bAccessoriesShown = VARIANT_FALSE;
			pcvwspInst->m_bAccessoriesDirty = VARIANT_FALSE;
		}
		SAFERELEASE(lpCamera);
		SAFERELEASE(lpTransform);

		return D3D_OK;
    }
    return TRUE;
}

STDMETHODIMP CVWGeometrySprite::get_RotationAxis(int* piAxis)
{
	HRESULT hr = S_OK;

	ASSERT(piAxis);

	if (NULL == piAxis)
	{
		hr = E_POINTER;
		goto EXIT_FAIL;
	}

	*piAxis = m_iAxis;

EXIT_FAIL:
	return hr;
}

STDMETHODIMP CVWGeometrySprite::put_RotationAxis(int iAxis)
{
	HRESULT hr = S_OK;

	//Make sure value is 0, 1,2, or 4
	if (iAxis & 0xFFFFFFF8)
	{
		hr = E_INVALIDARG;
		goto EXIT_FAIL;
	}

	m_iAxis = iAxis;

EXIT_FAIL:
	return hr;

}

