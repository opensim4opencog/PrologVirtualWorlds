// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWGCachO.cpp : Implementation of CVWGCacheApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include <d2d.h>
#include <d3drmwin.h>
#include <io.h>
#include <VWGCache.h>
#include "VWGCachO.h"
#include "readwdb.h"
#include "syshelp.h"

#define szDefaultTextureName "VWGCDEFAULT"
#define iDefaultTextureName 11
#define szDefaultTextureFailed "FAILED"
#define iDefaultTextureFailed 6

void EnumObjectsCallback(LPDIRECT3DRMOBJECT lpD3DRMobj, LPVOID lpArg)
{
	CComPtr<IDirect3DRMMeshBuilder>	pMeshBuilder;
	DWORD	dwSize;
	char	*szName;

	if (!lpArg || !lpD3DRMobj)
		goto ERROR_ENCOUNTERED;

	if (FAILED(lpD3DRMobj->QueryInterface(IID_IDirect3DRMMeshBuilder, (void**)&pMeshBuilder)) || !pMeshBuilder)
		goto ERROR_ENCOUNTERED;

	// First get the object's name.
	if (FAILED(lpD3DRMobj->GetName(&dwSize, NULL)))
	{
		ASSERT(FALSE);
		goto ERROR_ENCOUNTERED;
	}

	if (dwSize == 0)
		goto ERROR_ENCOUNTERED;

	szName = (char*)malloc(dwSize);
	if (FAILED(lpD3DRMobj->GetName(&dwSize, szName)))
	{
		ASSERT(FALSE);
		goto ERROR_ENCOUNTERED;
	}

	// Store the objects name in its app data.
	lpD3DRMobj->SetAppData((DWORD)szName);
	((CPtrList*)lpArg)->AddHead(lpD3DRMobj);
	lpD3DRMobj->SetName(NULL);
	return;

ERROR_ENCOUNTERED:
	lpD3DRMobj->Release();
}

/////////////////////////////////////////////////////////////////////////////
//

CVWGeometryCacheObject::CVWGeometryCacheObject()
{
#ifdef TRACE_ALLOCATION
	TRACE("CVWGeometryCacheObject::CVWGeometryCacheObject, this 0x%0x\n", this);
#endif

	m_pInternetFileManager = NULL;
	m_pD2DRM = NULL;
	m_pD3DRM = NULL;
#ifndef NO_3DS_CONVERTER
	m_p3dsConverter = NULL;
#endif
#ifndef NO_VRML_CONVERTER
	m_pVRMLConverter = NULL;
#endif
	m_pDD = NULL;
	m_iGeomCacheSize = 0;
	m_pWorld = NULL;
	m_pDefaultImage = NULL;
	InitializeCriticalSection(&m_CS);
}

HRESULT
CVWGeometryCacheObject::Init(IWorld *pWorld)
{
	// Get the cache and image manager COM objects.
//	CLSID	clsidImageMgr;
	HRESULT hr=S_OK;
	static CComBSTR bstrDirect3DRM("Direct3DRM");
	static CComBSTR bstrDirect2DRM("Direct2DRM");
	static CComBSTR bstrDirectDraw("DirectDraw");
	static CComBSTR bstrInetfile("Inetfile");

	// REVIEW: We need to check if we are reinitializing because of Reconnect
	if (m_pInternetFileManager)
		m_pInternetFileManager->Release();
	if (m_pD2DRM)
		m_pD2DRM->Release();
	if (m_pD3DRM)
		m_pD3DRM->Release();
	if (m_pDD)
		m_pDD->Release();
	
	//Get InternetFileManager.
	hr=pWorld->get_Tool(bstrInetfile,(IUnknown**) &m_pInternetFileManager);
	if(FAILED(hr))
	{
		goto ERROR_ENCOUNTERED;
	}
	if (FAILED(hr = pWorld->get_Tool(bstrDirect2DRM, (IUnknown**) &m_pD2DRM)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pWorld->get_Tool(bstrDirect3DRM, (IUnknown**) &m_pD3DRM)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pWorld->get_Tool(bstrDirectDraw, (IUnknown**) &m_pDD)))
		goto ERROR_ENCOUNTERED;

	SAFERELEASE(m_pWorld);
	m_pWorld = pWorld;
	SAFEADDREF(pWorld);

ERROR_ENCOUNTERED:
	return hr;
}

CVWGeometryCacheObject::~CVWGeometryCacheObject()
{
#ifdef TRACE_ALLOCATION
	TRACE("CVWGeometryCacheObject::~CVWGeometryCacheObject, this 0x%0x\n", this);
#endif

	IVWFrame	*pVWFrame;

	// By now all frames should not be referenced.
	ASSERT(m_mapFileToVWFrame.GetCount() == m_listNotReferenced.GetCount());
	
	// Release all cached geometry.
	while (!m_listNotReferenced.IsEmpty())
	{
		pVWFrame = (IVWFrame*)m_listNotReferenced.GetTail();
		m_listNotReferenced.RemoveTail();
		DeleteFrame(pVWFrame);
	}

	// Now all maps should be empty.
	ASSERT(m_mapFileToVWFrame.IsEmpty() && m_mapVWFrameToFaces.IsEmpty());

	SAFERELEASE(m_pDefaultImage);

	SAFERELEASE(m_pInternetFileManager);
	SAFERELEASE(m_pD2DRM);
	SAFERELEASE(m_pD3DRM);
#ifndef NO_3DS_CONVERTER
	SAFERELEASE(m_p3dsConverter);
#endif
#ifndef NO_VRML_CONVERTER
	SAFERELEASE(m_pVRMLConverter);
#endif
	SAFERELEASE(m_pDD);
	SAFERELEASE(m_pWorld);

	DeleteCriticalSection(&m_CS);
}

STDMETHODIMP CVWGeometryCacheObject::LoadGeometry(BSTR bstrGeometry, IVWFrame **ppvwf)
{
	// REVIEW: Always assume 3D WDB for now.
	IVWFrame			*pvwfCached = NULL;
	IDirect3DRMFrame	*prlFrame = NULL;
	char				szExt[_MAX_EXT];
	BSTR				bstrFinalURL = NULL, bstrFinalPath = NULL;
	CString				strGeometry, strFinalURL, strFinalPath;
	CPtrList			*plistFaces;
	int					iFwdSlash, iBackSlash;
	VARIANT_BOOL		bFound;
	HRESULT				hr = S_OK;
	
	// First, download the file.
	// Retrieve URL by lower case and search in cache by lower case since cache search
	// is case sensitive.
	strGeometry = CString(bstrGeometry);
	strGeometry.MakeLower();
	if (FAILED(hr = m_pInternetFileManager->FindCachedFile(CComBSTR(strGeometry), &bstrFinalURL, &bstrFinalPath, &bFound)) || !bFound)
	{
		if (FAILED(hr = m_pInternetFileManager->GetFileEx(CComBSTR(strGeometry), VARIANT_FALSE, &bstrFinalURL, &bstrFinalPath)))
		{
			m_pWorld->Report(CComBSTR(CString("VWGeometryCache: Failed to download file: ") + CString(bstrGeometry)), -1);
			goto ERROR_ENCOUNTERED;
		}
	}
	strFinalURL = CString(bstrFinalURL);
	strFinalPath = CString(bstrFinalPath);
	
	// Firstly, have we already loaded this?
	if (m_mapFileToVWFrame.Lookup(strFinalURL, (void*&)pvwfCached))
	{
		SAFEADDREF(pvwfCached);
#ifdef _DEBUG
		VWTRACE(m_pWorld, "CVWGeometryCacheObject", VWT_ERROR, "CVWGeometryCacheObject::LoadGeometry. Found cached geometry %s\n", strFinalURL);
#endif			
		// Is it on the NotReferenced list?
		POSITION pos = m_listNotReferenced.Find(pvwfCached);
		if (pos)
		{
			// Remove this VWFrame from the NotReferenced list now that it is being used
			// again and attach its textures.
			m_listNotReferenced.RemoveAt(pos);
			if (!m_mapVWFrameToFaces.Lookup(pvwfCached, plistFaces))
			{
				ASSERT(FALSE);
				hr = E_FAIL;
				goto ERROR_ENCOUNTERED;
			}
			AttachTexturesToFaces(plistFaces);
		}
		// Never use original frame, copy it. Must do the recurse persp
		// texture to deal with any textures not yet downloaded for the original
		// frame (these textures must know that the new downloaded texture will now
		// need attaching to another face).
		EnterCriticalSection(&m_CS);
		pvwfCached->Copy(ppvwf);
		(*ppvwf)->get_Frame3D(&prlFrame);
		RecursePerspTexture(prlFrame);
		LeaveCriticalSection(&m_CS);
	}
	else
	{
#ifdef _DEBUG
		VWTRACE(m_pWorld, "CVWGeometryCacheObject", VWT_ERROR, "CVWGeometryCacheObject::LoadGeometry. Creating cached geometry %s\n", strFinalURL);
#endif			
		// Create the 3D frame for the object.
		if (FAILED(hr = m_pD3DRM->CreateFrame(NULL, &prlFrame)))
		{
			ASSERT(FALSE);
			goto ERROR_ENCOUNTERED;
		}
		
		// get extension
		_splitpath(strFinalPath, NULL, NULL, NULL, szExt );
		
		// Now, create the VWFrame for this geometry which will be returned to the caller.
		if (FAILED(hr = m_pWorld->CreateObjectPropertyExt(CLSID_VWFrame, NULL, (IObjectProperty**)&pvwfCached)))
			goto ERROR_ENCOUNTERED;

		if (FAILED(hr = pvwfCached->put_Frame3D(prlFrame)))
		{
			ASSERT(FALSE);
			goto ERROR_ENCOUNTERED;
		}

		// Store globals for loader callbacks.
		pParentFrameForLoading = prlFrame;
		iFwdSlash = strFinalURL.ReverseFind('/');
		iBackSlash = strFinalURL.ReverseFind('\\');
		if (iBackSlash > iFwdSlash)
			strParentURLForLoading = strFinalURL.Left(iBackSlash+1);
		else
			strParentURLForLoading = strFinalURL.Left(iFwdSlash+1);
		d3drmRenderQuality = -1;

		// The VWFrame created is the geometry cache's private copy (the original) and needs
		// to be referenced. It will never be passed out, only copied.
		m_mapFileToVWFrame.SetAt(strFinalURL, pvwfCached);
		SAFEADDREF(pvwfCached);
		plistFaces = new CPtrList;
		m_mapVWFrameToFaces.SetAt(pvwfCached, plistFaces);
		pvwfCached->SetAppString(bstrFinalURL);

		// call appropriate loader 
		if( _stricmp(szExt, ".WDB") == 0)
		{
			if (!CWDBReader::ReadWDB(strFinalPath, "", prlFrame, this))
			{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "CVWGeometryCacheObject", VWT_ERROR, "CVWGeometryCacheObject::LoadGeometry: Couldn't read wdb file (local)%s internet:%s\n", strFinalPath, CString(bstrGeometry));
#endif
				m_pWorld->Report(CComBSTR(CString("VWGeometryCache: Failed to load file: ") + strFinalPath), -1);
				DeleteFrame(pvwfCached);
				hr = E_FAIL;
				goto ERROR_ENCOUNTERED;
			}
		}
		else if ( _stricmp(szExt, ".X") == 0 )
		{
			// Want X files meshes to default to unlit flat for now since X file format
			// allows no way to set mesh quality and no X files writer is available which
			// can add lights to the X file.
			d3drmRenderQuality = D3DRMRENDER_UNLITFLAT;
			if (FAILED(hr = ReadXExt(strFinalPath)))
			{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "CVWGeometryCacheObject", VWT_ERROR, "CVWGeometryCacheObject::LoadGeometry: Couldn't read x file (local)%s internet:%s\n", strFinalPath, CString(bstrGeometry));
#endif
				m_pWorld->Report(CComBSTR(CString("VWGeometryCache: Failed to load file: ") + strFinalPath), -1);
				DeleteFrame(pvwfCached);
				goto ERROR_ENCOUNTERED;
			}
		} 
#ifndef NO_3DS_CONVERTER
		else if ( _stricmp(szExt, ".3ds") == 0 )
		{
			if (FAILED(hr = Read3DS(bstrFinalPath, prlFrame)))
			{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "CVWGeometryCacheObject", VWT_ERROR, "CVWGeometryCacheObject::LoadGeometry: Couldn't read 3ds file (local)%s internet:%s\n", strFinalPath, CString(bstrGeometry));
#endif
				m_pWorld->Report(CComBSTR(CString("VWGeometryCache: Failed to load file: ") + strFinalPath), -1);
				DeleteFrame(pvwfCached);
				goto ERROR_ENCOUNTERED;
			}
		} 
#endif
#ifndef NO_VRML_CONVERTER
		else if ( _stricmp(szExt, ".wrl") == 0 )
		{
			if (FAILED(hr = ReadVRML(bstrFinalPath, prlFrame)))
			{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "CVWGeometryCacheObject", VWT_ERROR, "CVWGeometryCacheObject::LoadGeometry: Couldn't read vrml file (local)%s internet:%s\n", strFinalPath, CString(bstrGeometry));
#endif
				m_pWorld->Report(CComBSTR(CString("VWGeometryCache: Failed to load file: ") + strFinalPath), -1);
				DeleteFrame(pvwfCached);
				goto ERROR_ENCOUNTERED;
			}
		} 
#endif
		else 
		{
			m_pWorld->Report(CComBSTR(CString("VWGeometryCache: Unknown extension on file: ") + strFinalPath), -1);
			DeleteFrame(pvwfCached);
			hr = E_FAIL;
			goto ERROR_ENCOUNTERED;
		}

		// Ensure all textures set to perspective correct and associated with faces.
		EnterCriticalSection(&m_CS);
		pvwfParentFrameForLoading = pvwfCached;
		RecursePerspTexture(prlFrame);
		pvwfParentFrameForLoading = NULL;

		// Never use original frame, copy it. Must do the recurse persp
		// texture to deal with any textures not yet downloaded for the original
		// frame (these textures must know that the new downloaded texture will now
		// need attaching to another face).
		pvwfCached->Copy(ppvwf);
		SAFERELEASE(prlFrame);
		(*ppvwf)->get_Frame3D(&prlFrame);
		RecursePerspTexture(prlFrame);
		LeaveCriticalSection(&m_CS);
	}

	// Set up back pointers and data for the frame we're returning.
	(*ppvwf)->put_GeometryCache((IVWGeometryCache*)this);
	(*ppvwf)->SetAppString(bstrFinalURL);

ERROR_ENCOUNTERED:
	
	pParentFrameForLoading = NULL;
	strParentURLForLoading.Empty();
	SAFEFREESTRING(bstrFinalURL);
	SAFEFREESTRING(bstrFinalPath);
	SAFERELEASE(prlFrame);

	return hr;
}

STDMETHODIMP CVWGeometryCacheObject::ReadXExt(const char* psz)
{
	CPtrList listNamedMeshes;
	IDirect3DRMObject *pObject;
	char *szName;
	POSITION pos;
	HRESULT hr = S_OK;

	// Before we read the X file, we will NULL out the name of ALL other D3D objects
	// to get around the D3D problem whereby objects named similarly to already loaded
	// objects will be loaded nameless.
	m_pD3DRM->EnumerateObjects(EnumObjectsCallback,  &listNamedMeshes);
	
#ifdef IDirect3D2_CreateDevice 
	IID	*apIID[4];

	IDirect3DRM2 *d3drm2 = NULL;

	// Specify IID's of object types to be loaded from this X file.

	apIID[0] = (IID*)&IID_IDirect3DRMMeshBuilder;
	apIID[1] = (IID*)&IID_IDirect3DRMFrame;
	apIID[2] = (IID*)&IID_IDirect3DRMAnimationSet;
	apIID[3] = (IID*)&IID_IDirect3DRMProgressiveMesh;

	// Load the object.
	// Ignore failures, best thing for user is just to leave frame without visuals.

	//Query the m_pD3DRM for the d3drm2 interface
	bLoaded=false;
	hr = m_pD3DRM->QueryInterface(IID_IDirect3DRM2, (void**)&d3drm2);
	if (SUCCEEDED(hr)) {
		hr = d3drm2->Load((void*)psz,
							NULL,
							(IID**)apIID,
							3,
							D3DRMLOAD_FROMFILE,
							loadObjects,
							this,
							LoadTextureCB,
							this,
							pParentFrameForLoading);

		if (!bLoaded) {	
			//We load it as a progressive mesh at this point.
			IDirect3DRMProgressiveMesh *pm;
			hr = d3drm2->CreateProgressiveMesh(&pm);
			if (FAILED(hr)) ASSERT(false);
			hr = pm->Load((void*)psz,
						NULL,
						D3DRMLOAD_FIRST | D3DRMLOAD_FROMFILE,
						NULL,			//was LoadTextureCB
						NULL);			//was this

			if (SUCCEEDED(hr)) {
				pParentFrameForLoading->AddVisual(pm);
			}
			//if hr != D3DRM_OK ... well, don't do anything.
		}
	}
	else {
		hr = m_pD3DRM->Load((void*)psz,
							NULL,
							(IID**)apIID,
							2,
							D3DRMLOAD_FROMFILE,
							loadObjects,
							this,
							LoadTextureCB,
							this,
							pParentFrameForLoading);
	}
#else
	IID	*apIID[3];

	// Specify IID's of object types to be loaded from this X file.

	apIID[0] = (IID*)&IID_IDirect3DRMMeshBuilder;
	apIID[1] = (IID*)&IID_IDirect3DRMFrame;
	apIID[2] = (IID*)&IID_IDirect3DRMAnimationSet;

	// Load the object.
	// Ignore failures, best thing for user is just to leave frame without visuals.

	//Query the m_pD3DRM for the d3drm2 interface
	hr = m_pD3DRM->Load((void*)psz,
						NULL,
						(IID**)apIID,
						2,
						D3DRMLOAD_FROMFILE,
						loadObjects,
						this,
						LoadTextureCB,
						this,
						pParentFrameForLoading);
#endif

	// Now we've read the X file, replace all the names.
	for (pos = listNamedMeshes.GetHeadPosition(); pos != NULL;)
	{
		if (!(pObject = (IDirect3DRMObject*)(listNamedMeshes.GetNext(pos))) ||
			!(szName = (char*)pObject->GetAppData()))
		{
			ASSERT(FALSE);
			continue;
		}
		pObject->SetName(szName);
		pObject->Release();
		free((void*)szName);
	}
	return hr;
}

#ifndef NO_3DS_CONVERTER
STDMETHODIMP CVWGeometryCacheObject::Read3DS(BSTR bstrFile, LPDIRECT3DRMFRAME lpFrame)
{
	HRESULT hr = S_OK;
	CComVariant varFrame;
	LPDIRECT3DRMFRAME pLocalFrame = NULL;

	if (m_p3dsConverter == NULL) 
	{
		if (FAILED(hr = CoCreateInstance(CLSID_Convert3DSGeometry, 
							  NULL, 
							  CLSCTX_INPROC_SERVER,
							  IID_IConvert3DSGeometry,
							  (void**) &m_p3dsConverter)))
		{
			m_pWorld->Report(CComBSTR("VWGeometryCache: Could not instantiate Convert3DS class/interface."), -1);
			goto ERROR_DONE;
		}

		if (FAILED(hr = m_p3dsConverter->Init(m_pD3DRM, m_pD2DRM, m_pInternetFileManager)))
		{
			m_pWorld->Report(CComBSTR("VWGeometryCache: Could not initialize Convert3DS class/interface."), -1);
			goto ERROR_DONE;
		}
	}
	// these should really be qi'ed but we know it implements the ITextureCallback interface
	if (FAILED(hr = m_p3dsConverter->ReadScene(bstrFile, (ITextureCallback *)this, &varFrame))) goto ERROR_DONE;
	pLocalFrame = (LPDIRECT3DRMFRAME) varFrame.punkVal;
	lpFrame->AddChild(pLocalFrame);

ERROR_DONE:
	return hr;
}
#endif

#ifndef NO_VRML_CONVERTER
STDMETHODIMP CVWGeometryCacheObject::ReadVRML(BSTR bstrFile, LPDIRECT3DRMFRAME lpFrame)
{
	HRESULT hr = S_OK;
	CComVariant varFrame;
	LPDIRECT3DRMFRAME pLocalFrame = NULL;

	if (m_pVRMLConverter == NULL) 
	{
		if (FAILED(hr = CoCreateInstance(CLSID_ConvertVRMLGeometry, 
							  NULL, 
							  CLSCTX_INPROC_SERVER,
							  IID_IConvertVRMLGeometry,
							  (void**) &m_pVRMLConverter))) 
		{
			m_pWorld->Report(CComBSTR("VWGeometryCache: Could not instantiate Convert3DS class/interface."), -1);
			goto ERROR_DONE;
		}

		if (FAILED(hr = m_pVRMLConverter->Init(m_pD3DRM, m_pD2DRM, m_pInternetFileManager)))
		{
			m_pWorld->Report(CComBSTR("VWGeometryCache: Could not initialize Convert3DS class/interface."), -1);
			goto ERROR_DONE;
		}
	}
	// these should really be qi'ed but we know it implements the ITextureCallback interface
	if (FAILED(hr = m_pVRMLConverter->ReadScene(bstrFile, (ITextureCallback *)this, &varFrame))) goto ERROR_DONE;
	pLocalFrame = (LPDIRECT3DRMFRAME) varFrame.punkVal;
	lpFrame->AddChild(pLocalFrame);

ERROR_DONE:

	return hr;
}
#endif

STDMETHODIMP CVWGeometryCacheObject::ReleaseGeometry(IVWFrame *pvwf)
{
	BSTR				bstr;
	DWORD				dwRef;
	CPtrList			*plistFaces;
	IVWFrame			*pvwfOriginal;
	
	// Putting the geometry cache to NULL in the VWFrame could delete this. Add a temporary
	// reference to ourself.
	((IVWGeometryCache*)this)->AddRef();
	pvwf->put_GeometryCache(NULL);
	pvwf->GetAppString(&bstr);

#ifdef _DEBUG
	VWTRACE(m_pWorld, "CVWGeometryCacheObject", VWT_ERROR, "CVWGeometryCacheObject::ReleaseGeometry: %s\n", CString(bstr));
#endif			

	// Find the original frame this frame was copied from.
	VERIFY(m_mapFileToVWFrame.Lookup(CString(bstr), (void*&)pvwfOriginal));
	SysFreeString(bstr);
	
	pvwfOriginal->Release();
	pvwfOriginal->get_RefCount(&dwRef);
	if (dwRef == 1)
	{
		// No copies of original are in use any more.

#ifdef _DEBUG
		VWTRACE(m_pWorld, "CVWGeometryCacheObject", VWT_ERROR, "CVWGeometryCacheObject::ReleaseGeometry. Marking %s as un-referenced\n", CString(bstr));
#endif			

		EnterCriticalSection(&m_CS);

		// Don't delete, just mark as unused.
		m_listNotReferenced.AddHead(pvwfOriginal);

		// Remove the textures for all faces of this frame.
		VERIFY(m_mapVWFrameToFaces.Lookup(pvwfOriginal, plistFaces));
		RemoveTexturesFromFaces(plistFaces);

		// Right, now how's the cache doing for size?
		if (m_listNotReferenced.GetCount() > m_iGeomCacheSize)
		{
			IVWFrame *pvwfLRU;

			// Get the LRU'd VWFrame and remove it from the list.
			pvwfLRU = (IVWFrame*)m_listNotReferenced.GetTail();
			m_listNotReferenced.RemoveTail();
			
			// Delete the frame and all associated references.
			DeleteFrame(pvwfLRU);
		}
		LeaveCriticalSection(&m_CS);
	}

	// Remove temporary reference from ourself.
	((IVWGeometryCache*)this)->Release();

	return S_OK;
}

void CVWGeometryCacheObject::DeleteFrame(IVWFrame *pvwFrame)
{
	IDirect3DRMFace		*pFace, *pFace2;
	IDirect3DRMTexture	*pTexture;
	CPtrList			*plistFacesForTexture, *plistFaces, listFreed;
	POSITION			pos, pos2;
	BSTR				bstrFile, bstrImageFile;
	CString				strFile;

	// Remove entry in the frame-faces map and release associated face/texture data.
	VERIFY(m_mapVWFrameToFaces.Lookup(pvwFrame, plistFaces));
	for (pos = plistFaces->GetHeadPosition(); pos != NULL;)
	{
		pFace = (IDirect3DRMFace*)(plistFaces->GetNext(pos));
		pFace->GetTexture(&pTexture);
		if (!(pFace->GetAppData()))
		{
			// Texture not yet downloaded.
			// Make sure haven't already dealt with this texture's faces.
			if (!(plistFacesForTexture = (CPtrList*)(pTexture->GetAppData())))
				continue;
			// Cancel download.
			char buff[20];
			ltoa((DWORD)pTexture,buff,10);
			m_pInternetFileManager->FileDLDone(CComBSTR(CString(buff)));
			// Cleanup the FacesForTexture list since it will now never be used.
			for (pos2 = plistFacesForTexture->GetHeadPosition(); pos2 != NULL;)
			{
				pFace2 = (IDirect3DRMFace*)(plistFacesForTexture->GetNext(pos2));
				ASSERT(pFace2);
				SAFERELEASE(pFace2);
			}
			delete plistFacesForTexture;
			// This face must still have a default texture.
			// Set the texture's AppData to NULL since list is no longer valid
			// and will get to the texture again in this loop via other faces.
			pTexture->SetAppData(NULL);
			// Release the AddRef done on GetFileAsync().
			pTexture->Release();
		}
		// Free the BSTR names in the faces AppData's.
		bstrImageFile = (BSTR)(pFace->GetAppData());
		if (!listFreed.Find(bstrImageFile))
		{
			SysFreeString(bstrImageFile);
			listFreed.AddHead(bstrImageFile);
		}
		SAFERELEASE(pTexture);
		SAFERELEASE(pFace);
	}
	delete plistFaces;
	m_mapVWFrameToFaces.RemoveKey(pvwFrame);

	// Get it's assoc string and look that up in the map, then remove it from the map.
	pvwFrame->GetAppString(&bstrFile);
	strFile = CString(bstrFile);
	VERIFY(m_mapFileToVWFrame.RemoveKey(strFile));
	SysFreeString(bstrFile);

	// And finally release the LRU'd VWFrame.
	pvwFrame->Release();
}

STDMETHODIMP CVWGeometryCacheObject::SetGeomCacheSize(long iCacheSize)
{
	m_iGeomCacheSize = iCacheSize;
	return S_OK;
}

STDMETHODIMP CVWGeometryCacheObject::OnProgress_T2(ULONG ulProgress, ULONG ulProgressMax, long lCookie)
{
	return S_OK;
}

STDMETHODIMP CVWGeometryCacheObject::OnDone_T2(HRESULT hresult, BSTR bstrFinalPath, long lCookie, BSTR bstrFullURL)
{	
	IDirect2DRMImage *pImage = NULL;
	IDirect3DRMTexture	*pTexture = NULL;
	IDirect3DRMFace		*pFace = NULL;
	CString				strFinalPath(bstrFinalPath);
	CPtrList			*plistFacesForTexture;
	POSITION			pos;
	HRESULT	hr = S_OK;

	EnterCriticalSection(&m_CS);

	// If hresult isn't OK then bstrFinalPath is probably empty. We need to note the
	// failure so other areas of the code are aware of it.
	if (hresult != S_OK)
	{
		m_pWorld->Report(CComBSTR(CString("VWGeometryCache: Asynchronous file download failed: ") + CString(bstrFullURL)), -1);

		strFinalPath = szDefaultTextureFailed;
	}
	
	plistFacesForTexture = (CPtrList*)(((IDirect3DRMTexture*)lCookie)->GetAppData());
	if (plistFacesForTexture)
	{
		// OK the texture-faces relationship has been created. Attach the new texture to
		// the faces.
		// If we failed to download the file, delete the default texture's FacesForTexture list,
		// but leave the faces app data alone so other methods know its still the default texture.

		// Create the texture from the image filename and apply it to all the faces.
		if (hresult == S_OK && FAILED(m_pD2DRM->CreateImageFromImageFile(bstrFinalPath, 0, &pImage)))
			m_pWorld->Report(CComBSTR(CString("VWGeometryCache: Could not create image from file: ") + strFinalPath), -1);

		if (hresult == S_OK && pImage && FAILED(pImage->GetD3DTexture(m_pD3DRM, &pTexture)))
			m_pWorld->Report(CComBSTR(CString("VWGeometryCache: Could not get texture for file: ") + strFinalPath), -1);

		if (pTexture)
			pTexture->SetName(strFinalPath);

		for (pos = plistFacesForTexture->GetHeadPosition(); pos != NULL;)
		{
			pFace = (IDirect3DRMFace*)(plistFacesForTexture->GetNext(pos));

			// Create the face-filename relationship, just for original frames.
			if (hresult == S_OK && !pFace->GetAppData())
				pFace->SetAppData((DWORD)(SysAllocString(bstrFinalPath)));

			if (pTexture)
				VERIFY(SUCCEEDED(pFace->SetTexture(pTexture)));

			SAFERELEASE(pFace);
		}

		// The faces for texture list was only created so that default textures could
		// be replaced with final textures on all the faces.
		delete plistFacesForTexture;
		((IDirect3DRMTexture*)lCookie)->SetAppData(NULL);
	}

	// Incase the texture-faces relationship has not been created, store this new texture 
	// name in the default texture so it can be attached to the face when the texture-face
	// relationship is figured out.
	((IDirect3DRMTexture*)lCookie)->SetName(CString(szDefaultTextureName) + strFinalPath);

	// This was AddRef'd on GetFileAsync().
	((IDirect3DRMTexture*)lCookie)->Release();

	SAFERELEASE(pImage);
	SAFERELEASE(pTexture);

	LeaveCriticalSection(&m_CS);

	return hr;
}

void loadObjects(LPDIRECT3DRMOBJECT pObject, REFIID ObjectGuid, void *pvArg)
{
	DWORD	dwSize = 100;
	char	szName[100];

	if (!pvArg)
	{
		ASSERT(FALSE);
		return;
	}

	// First get the object's name.
	if (FAILED(pObject->GetName(&dwSize, szName)))
		ASSERT(FALSE);

	//It can be added as a visual if the object is a mesh or a progressiveMesh
	if (ObjectGuid == IID_IDirect3DRMMeshBuilder)
	{
		// Load the mesh builder object.
		//IDirect3DRMVisual was IDirect3DRMMeshBuilder
		if (FAILED(((CVWGeometryCacheObject*)pvArg)->pParentFrameForLoading->AddVisual((IDirect3DRMVisual*)pObject)))
			ASSERT(FALSE);
	}
//check for the progressive mesh only if we're using Direct3D version 5 or higher
#ifdef IDirect3D2_CreateDevice 
	else if (ObjectGuid == IID_IDirect3DRMProgressiveMesh)
	{
		if (FAILED(((CVWGeometryCacheObject*)pvArg)->pParentFrameForLoading->AddVisual((IDirect3DRMVisual*)pObject)))
			ASSERT(FALSE);
	}
#endif
	else if (ObjectGuid == IID_IDirect3DRMFrame)
	{
		if (FAILED(((CVWGeometryCacheObject*)pvArg)->pParentFrameForLoading->AddChild((IDirect3DRMFrame*)pObject)))
			ASSERT(FALSE);
	}
	else if (ObjectGuid == IID_IDirect3DRMAnimationSet)
	{
		IDirect3DRMAnimationSet2* pAnimSet = NULL;
		IVWFrame* pSceneFrame = NULL;

		pObject->QueryInterface(IID_IDirect3DRMAnimationSet2,(void**)&pAnimSet);

		//Assume there's only one animation set per scene and poke in there.
		pSceneFrame = (IVWFrame*) ((CVWGeometryCacheObject*)pvArg)->pParentFrameForLoading->GetAppData();
		pSceneFrame->put_AnimationSet(pAnimSet);

		SAFERELEASE(pAnimSet);
	}
	else
	{
		ASSERT(FALSE);
	}
	SAFERELEASE(pObject);
	((CVWGeometryCacheObject*)pvArg)->bLoaded = true;
}

// Function: LoadTextureCB
//    We will assume that textures of the same name will share the same D2DRMImage.
HRESULT
LoadTextureCB(char *szName, void *pvArg, LPDIRECT3DRMTEXTURE *ppd3dTexture)
{
	// Ignore return value since don't want to stop geometry file loading.
	((CVWGeometryCacheObject*)pvArg)->LoadTextureInt(CComBSTR(szName), NULL, ppd3dTexture);
	return S_OK;
}


// call function recursively on all meshbuilders for a frame hierarchy
void CVWGeometryCacheObject::RecursePerspTexture(LPDIRECT3DRMFRAME lpFrame)
{
	HRESULT hr = S_OK;
	LPDIRECT3DRMFRAMEARRAY lpFrameArray = NULL;
	LPDIRECT3DRMVISUALARRAY lpVisuals = NULL;
	LPDIRECT3DRMVISUAL lpD3DRMVisual = NULL;
	LPDIRECT3DRMMESHBUILDER lpMesh = NULL;
	LPDIRECT3DRMFRAME lpChildFrame = NULL;
	int cVisual, cChild;

	if (FAILED(hr = lpFrame->GetVisuals(&lpVisuals))) goto ERROR_ENCOUNTERED;
	for (cVisual = 0; cVisual < (int)(lpVisuals->GetSize()); cVisual++) {
		if (SUCCEEDED(hr = lpVisuals->GetElement(cVisual, &lpD3DRMVisual))) {
			if (SUCCEEDED(hr = lpD3DRMVisual->QueryInterface(IID_IDirect3DRMMeshBuilder, (void**) &lpMesh)))
			{
				lpMesh->SetPerspective(TRUE);
				if (d3drmRenderQuality != -1)
					lpMesh->SetQuality(d3drmRenderQuality);
				RecurseFaceTextures(lpMesh);
				SAFERELEASE(lpMesh);
			}
			SAFERELEASE(lpD3DRMVisual);
		}
	}
	SAFERELEASE(lpVisuals);
	// now need to recurse down a level if necessary
	if (FAILED(hr = lpFrame->GetChildren(&lpFrameArray))) goto ERROR_ENCOUNTERED;
	for (cChild = 0; cChild < (int)(lpFrameArray->GetSize()); cChild++) {
		if (SUCCEEDED(hr = lpFrameArray->GetElement(cChild, &lpChildFrame))) {
			RecursePerspTexture(lpChildFrame);
			SAFERELEASE(lpChildFrame);
		}
	}
	SAFERELEASE(lpFrameArray);
	return;

ERROR_ENCOUNTERED:
	SAFERELEASE(lpVisuals);
	SAFERELEASE(lpFrameArray);
	SAFERELEASE(lpVisuals);
}

void CVWGeometryCacheObject::RecurseFaceTextures(LPDIRECT3DRMMESHBUILDER lpMeshBuilder)
{
	IDirect2DRMImage		*pImage = NULL;
	IDirect3DRMFaceArray	*pFaces = NULL;
	IDirect3DRMFace			*pFace = NULL;
	IDirect3DRMTexture		*pTexture = NULL, *pTextureTemp = NULL;
	CPtrList				*plistFaces, *plistFacesForTexture;
	BSTR					bstrFinalPath = NULL;
	int						iFace;
	char					szFinalPath[_MAX_PATH];
	DWORD					dwMAX_PATH = _MAX_PATH;
	CString					strFinalPath;
	HRESULT hr = S_OK;

	if (FAILED(hr = lpMeshBuilder->GetFaces(&pFaces)))
		goto ERROR_ENCOUNTERED;
	for (iFace = 0; iFace < (int)(pFaces->GetSize()); iFace++) {
		if (SUCCEEDED(hr = pFaces->GetElement(iFace, &pFace)))
		{
			if (SUCCEEDED(pFace->GetTexture(&pTexture)) && pTexture)
			{
				pTexture->GetName((DWORD*)&dwMAX_PATH, szFinalPath);
				strFinalPath = CString(szFinalPath);
				if (strFinalPath.GetLength() > iDefaultTextureName &&
					strFinalPath.Left(iDefaultTextureName) == szDefaultTextureName)
				{
					// This face has the default texture and the final texture file has been
					// downloaded. The default texture needs replacing with the final texture.
					strFinalPath = strFinalPath.Mid(iDefaultTextureName);
					bstrFinalPath = strFinalPath.AllocSysString();

					if (strFinalPath.GetLength() == iDefaultTextureFailed &&
						strFinalPath == szDefaultTextureFailed)
					{
						// Final texture download HAS finished, but it failed.
						SAFERELEASE(pFace);
						continue;
					}

					// Create the face-filename relationship.
					pFace->SetAppData((DWORD)bstrFinalPath);

					// Release local var to re-use.
					SAFERELEASE(pTexture);

					if (FAILED(m_pD2DRM->CreateImageFromImageFile(bstrFinalPath,
																  0,
																  &pImage)))
					{
						m_pWorld->Report(CComBSTR(CString("VWGeometryCache: Could not create image from file: ") + strFinalPath), -1);
						SAFERELEASE(pFace);
						SAFERELEASE(pImage);
						continue;
					}
					if (FAILED(pImage->GetD3DTexture(m_pD3DRM, &pTexture)))
					{
						m_pWorld->Report(CComBSTR(CString("VWGeometryCache: Could not get texture for file: ") + strFinalPath), -1);
						SAFERELEASE(pFace);
						SAFERELEASE(pImage);
						continue;
					}
					VERIFY(SUCCEEDED(pFace->SetTexture(pTexture)));
					pTexture->SetName(strFinalPath);
				}
				else
				{
					// Either this face has the default texture, or the final texture.
					// If its the default texture we will need to create the texture-faces 
					// relationship for when file download finishes.
					plistFacesForTexture = (CPtrList*)pTexture->GetAppData();
					if (pvwfParentFrameForLoading)
					{
						// This is the original frame being loaded, and must be the 
						// default texture.
						if (!plistFacesForTexture)
						{
							plistFacesForTexture = new CPtrList;
							pTexture->SetAppData((DWORD)plistFacesForTexture);
						}
						plistFacesForTexture->AddHead(pFace);
						SAFEADDREF(pFace);
						// Set AppData on face to NULL since filename is needed on 
						// face when file is downloaded.
						pFace->SetAppData(NULL);
					}
					else
					{
						// This is the original frame being copied.
						if (plistFacesForTexture)
						{
							// This must be the default texture.
							plistFacesForTexture->AddHead(pFace);
							SAFEADDREF(pFace);
						}
						// Set AppData on face to not NULL since filename is not needed on 
						// face when file is downloaded.
						pFace->SetAppData(0xffffffff);
					}
				}
				// Create the frame-faces relationship (only needed for original frames).
				if (pvwfParentFrameForLoading)
				{
					VERIFY(m_mapVWFrameToFaces.Lookup(pvwfParentFrameForLoading, plistFaces));
					plistFaces->AddHead(pFace);
					SAFEADDREF(pFace);
				}
				SAFERELEASE(pTexture);
			}
			SAFERELEASE(pFace);
			SAFERELEASE(pImage);
		}
	}
	SAFERELEASE(pFaces);

ERROR_ENCOUNTERED:

	SAFERELEASE(pTexture);
	SAFERELEASE(pFace);
	SAFERELEASE(pFaces);
	SAFERELEASE(pImage);

	return;
}

HRESULT CVWGeometryCacheObject::CreateDefaultImage()
{
	static CComBSTR bstrDefaultImage("DefaultImage");
	IDirectDrawSurface	*pdds = NULL;
	LPDIRECTDRAWPALETTE pPalette = NULL;
	DDSURFACEDESC		ddsd;
	DDBLTFX				ddbltfx;
	PALETTEENTRY		rgPE[256];
	HDC					hDC;
	int					i;
	HRESULT				hr = S_OK;
	
	// Create an offscreen surface. 4*4 is minimum size and make it 8 bit too.
    memset(&ddsd,0,sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
	ddsd.dwWidth = 4;
	ddsd.dwHeight = 4;
	memset(&ddsd.ddpfPixelFormat,0,sizeof(DDPIXELFORMAT));
    ddsd.ddpfPixelFormat.dwSize = sizeof( ddsd.ddpfPixelFormat );
	ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_PALETTEINDEXED8;
	ddsd.ddpfPixelFormat.dwRGBBitCount = 8;

	// Place surface in system memory.
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	if (FAILED(hr = m_pDD->CreateSurface( &ddsd, &pdds, NULL )))
	{
		m_pWorld->Report(CComBSTR("VWGeometryCache: Could not create surface for default image."), -1);
		goto ERROR_ENCOUNTERED;
	}

	hDC = GetDC(NULL);
	GetSystemPaletteEntries(hDC, 0, 256, rgPE);
	ReleaseDC(NULL, hDC);
	// lock down the palette entries so that D3D does not try to modify them...
	for (i = 0; i < 256; i++) rgPE[i].peFlags = D3DPAL_READONLY;

	if (FAILED(hr = m_pDD->CreatePalette(DDPCAPS_8BIT | DDPCAPS_INITIALIZE, rgPE, &pPalette, NULL)))
	{
		m_pWorld->Report(CComBSTR("VWGeometryCache: Could not create palette for default image."), -1);
		goto ERROR_ENCOUNTERED;
	}

	if (FAILED(hr = pdds->SetPalette(pPalette)))
	{
		m_pWorld->Report(CComBSTR("VWGeometryCache: Could not set palette for default image."), -1);
		goto ERROR_ENCOUNTERED;
	}

	// Make the surface just plain white.
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwFillColor = RGB_MAKE(255, 255, 255);
	pdds->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx);

	// Create the default image.
	if (FAILED(hr = m_pD2DRM->CreateImageFromSurface(pdds, bstrDefaultImage, &m_pDefaultImage)))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:

	SAFERELEASE(pdds);
	SAFERELEASE(pPalette);

	return hr;
}

STDMETHODIMP CVWGeometryCacheObject::AttachTexturesToFaces(CPtrList *plistFaces)
{
	IDirect3DRMTexture	*pTexture = NULL;
	IDirect3DRMFace		*pFace;
	IDirect2DRMImage	*pImage = NULL;
	BSTR				bstrFinalPath;
	POSITION			pos;

	for (pos = plistFaces->GetHeadPosition(); pos != NULL;)
	{
		pFace = (IDirect3DRMFace*)(plistFaces->GetNext(pos));
		if (!(bstrFinalPath = (BSTR)(pFace->GetAppData())))
		{
			// No face-filename relationship yet - this face must still have a default texture.
			continue;
		}
		// REVIEW: If this file doesn't exist, it has been swapped out of our temporary 
		// internet files area since it was last used. We will have to re-download it.
		if (FAILED(m_pD2DRM->CreateImageFromImageFile(bstrFinalPath, 0, &pImage)))
		{
			m_pWorld->Report(CComBSTR(CString("VWGeometryCache: Could not create image from file: ") + CString(bstrFinalPath)), -1);
			continue;
		}
		if (FAILED(pImage->GetD3DTexture(m_pD3DRM, &pTexture)))
		{
			m_pWorld->Report(CComBSTR(CString("VWGeometryCache: Could not get texture for file: ") + CString(bstrFinalPath)), -1);
			continue;
		}
		pFace->SetTexture(pTexture);
		pTexture->SetName(CString(bstrFinalPath));
		SAFERELEASE(pTexture);
		SAFERELEASE(pImage);
	}

	SAFERELEASE(pTexture);
	SAFERELEASE(pImage);

	return S_OK;
}

STDMETHODIMP CVWGeometryCacheObject::RemoveTexturesFromFaces(CPtrList *plistFaces)
{
	IDirect3DRMFace		*pFace;
	IDirect3DRMTexture  *pTexture;
	POSITION			pos;

	for (pos = plistFaces->GetHeadPosition(); pos != NULL;)
	{
		pFace = (IDirect3DRMFace*)(plistFaces->GetNext(pos));
		if (!(pFace->GetAppData()))
		{
			// Texture not yet downloaded.
			continue;
		}
		pFace->GetTexture(&pTexture);
		pFace->SetTexture(NULL);
		if (pTexture)
			m_pD2DRM->AddTextureForRelease(pTexture);
	}

	return S_OK;
}

STDMETHODIMP CVWGeometryCacheObject::LoadTextureInt(BSTR bstrName, IDirect3DRMFace *pFace, LPDIRECT3DRMTEXTURE *ppd3dTexture)
{
	IDirectDrawSurface	*pddsDefault = NULL;
	CComBSTR			bstrFinalURL, bstrFinalPath;
	CString				strName(bstrName), strURLToLoad;
	VARIANT_BOOL		bFound;
	HRESULT hr = S_OK;

	if (!m_pDefaultImage)
	{
		if (FAILED(hr = CreateDefaultImage()))
			goto Cleanup_LoadTexture;
	}

	// Create new texture each time since default image is tiny and can use AppData to
	// pass on face/final texture.
	if (FAILED(hr = m_pDefaultImage->ExportD3DTexture(m_pD3DRM, &pddsDefault, ppd3dTexture)))
		goto Cleanup_LoadTexture;

	// Make sure AppData and Name are NULL.
	(*ppd3dTexture)->SetAppData(NULL);
	(*ppd3dTexture)->SetName(szDefaultTextureName);
	(*ppd3dTexture)->AddRef();
	(*ppd3dTexture)->Release();
	
	// Want to load textures relative to parent root URL unless given a full URL.
	if (strName.Find("://") != -1)
		strURLToLoad = strName;
	else
		strURLToLoad = strParentURLForLoading + strName;

	// Retrieve URL by lower case and search in cache by lower case since cache search
	// is case sensitive.
	strURLToLoad.MakeLower();
	// Get remote files asyncronously (unless we already have them locally).
	if (FAILED(hr = m_pInternetFileManager->FindCachedFile(CComBSTR(strURLToLoad),
														   &bstrFinalURL.m_str,
														   &bstrFinalPath.m_str,
														   &bFound)) || !bFound)
	{
		// AddRef texture until OnDone().
		(*ppd3dTexture)->AddRef();
		if (FAILED
			(hr = m_pInternetFileManager->GetFileAsync(CComBSTR(strURLToLoad),
															 (long)(*ppd3dTexture),
															 (IIFMCallback*)this,
															 VARIANT_FALSE)
															 )
			)
		{
			m_pWorld->Report(CComBSTR(CString("VWGeometryCache: Failed to set up file download for file: ") + strURLToLoad), -1);
			(*ppd3dTexture)->Release();
		}
		goto Cleanup_LoadTexture;
	}

	// Store this new texture name in the default texture so it can be attached to the
	// face when the texture-face relationship is figured out.
	(*ppd3dTexture)->SetName(CString(szDefaultTextureName) + CString(bstrFinalPath));

Cleanup_LoadTexture:

	SAFERELEASE(pddsDefault);

	return hr;
}

// this is for passing to the 3dstudio and vrml converteres
STDMETHODIMP CVWGeometryCacheObject::LoadTextureCallback(BSTR bstr, LPDIRECT3DRMTEXTURE* ppD3DTexture)
{
	return LoadTextureInt(bstr, NULL, ppD3DTexture);
}
