// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// Convert3DS.cpp : Implementation of CConvert3DS
#include "stdafx.h"
#include <3dsToD3D.h>
#include <inetfile.h>
#include "Conv3DS.h"
#include "comdef.h"

#ifdef VWMM
#include "d2d_i.c"
#include "inetfile_i.c"
#endif
#include "vwsystem_i.c"

/////////////////////////////////////////////////////////////////////////////
// Constants

#define fDefBackgroundRed	0.0F
#define fDefBackgroundGreen	0.0F
#define fDefBackgroundBlue	0.5F

#define fTransparentRed		0.0F
#define fTransparentGreen	1.0F
#define fTransparentBlue	0.0F

#define iTransparentRed		0
#define iTransparentGreen	255
#define iTransparentBlue	0

// Right now in 3D Studio it is difficult to set UV coords to exactly 1 or 0, so we will define a tolerance within
// which we will "snap" the UV coords to 1 or 0.!
#define fUVTolerance		0.05F


/////////////////////////////////////////////////////////////////////////////
// Convenience macros
/////////////////////////////////////////////////////////////////////////////

#define NO_MATERIAL_DATA	-1
#define INCHESPERMETER		(1.0F / .0254F)
#define PI					3.1415926F
#define DEG2RAD(d)			(float)(d*PI/180.0F)
#define ERROR_IN_3DSFTK		(ftkerr3ds != FALSE3DS)

//Convenient macro for debugging.  // _asm {int 3}
//#ifdef _DEBUG
//#define CONVERSION3DS_ERROR ASSERT(0)  
//#else
#define CONVERSION3DS_ERROR
//#endif

#define SAFERELEASE(x) if (TRUE) { if (x) x->Release(); x = NULL; }
#define SAFEADDREF(x) if (TRUE) { if (x) x->AddRef(); }
/////////////////////////////////////////////////////////////////////////////
// Extern defs
/////////////////////////////////////////////////////////////////////////////
extern CComModule _Module;


/////////////////////////////////////////////////////////////////////////////
// Declarations
/////////////////////////////////////////////////////////////////////////////
void CleanNameString(char* szNameString);

BOOL ConvertFile(file3ds*			fiIn, 
				 LPDIRECT3DRMFRAME  piD3DFrame,
				 CString&			strPath);

/////////////////////////////////////////////////////////////////////////////
//Error detection... temporary
#if 0 // def _DEBUG
void _cdecl SetMemBreak(int nBreakNum)
{
	_CrtSetBreakAlloc(nBreakNum);
}
#endif
/////////////////////////////////////////////////////////////////////////////
// CConvert3DSGeometry
CConvert3DSGeometry::CConvert3DSGeometry()
{
	m_piDDraw = NULL;			
	m_piD3DRM = NULL;			
	m_piD2DRM = NULL;			
	m_piInternetFileManager = NULL;

	m_pCGTCallback = NULL;
	m_flMasterScale = 1.0;

	m_bConvertAnimations = TRUE;
	m_bKeepCachedTextureNames = FALSE;
	m_bLoadTextures = TRUE;
	m_bInitCalled = FALSE;
}

STDMETHODIMP CConvert3DSGeometry::Init(LPDIRECT3DRM piD3DRMParam = NULL, LPDIRECT2DRM piD2DRMParam = NULL, IInternetFileManager* piInternetFileManagerParam = NULL)
{
	BOOL bHaveAllObjects = TRUE;
	BOOL hr;
	_bstr_t bstrPath;

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	//If we've already inited, release old pointers
	if (m_bInitCalled)
	{
		SAFERELEASE(m_piD3DRM);
		SAFERELEASE(m_piD2DRM);
		SAFERELEASE(m_piDDraw);
		SAFERELEASE(m_piInternetFileManager);

		m_piDDraw = NULL;			
		m_piD3DRM = NULL;			
		m_piD2DRM = NULL;			
		m_piInternetFileManager = NULL;
	}

	//Set up the piD3DRM object
	if (NULL == (m_piD3DRM = piD3DRMParam) )
	{
		hr = Direct3DRMCreate(&m_piD3DRM);
		if ( FAILED(hr) || NULL == m_piD3DRM)
			bHaveAllObjects = FALSE;
	}
	else
		m_piD3DRM->AddRef();

	//Set up the piDDraw object
	if (bHaveAllObjects)
	{
		if (NULL == (m_piD2DRM = piD2DRMParam))
		{
			m_piDDraw = NULL;
			hr = DirectDrawCreate(NULL, &m_piDDraw, NULL);
			if ( FAILED(hr) || NULL == m_piDDraw)
				bHaveAllObjects = FALSE;
			else
			{
				hr = m_piDDraw->SetCooperativeLevel(NULL, DDSCL_NORMAL);
				if ( FAILED(hr) )
					bHaveAllObjects = FALSE;
			}

			if (bHaveAllObjects)
			{
				hr = CoCreateInstance(CLSID_CDirect2DRM, 
										  NULL, 
										  CLSCTX_INPROC_SERVER,
										  IID_IDirect2DRM,
										  (void**) &m_piD2DRM);
				if ( FAILED(hr) || NULL == m_piD2DRM)
					bHaveAllObjects = FALSE;
				else
					if ( FAILED(hr = m_piD2DRM->Init(m_piDDraw)) )
						bHaveAllObjects = FALSE;
			}
		}
		else
			m_piD2DRM->AddRef();

	}

	//Set up the piInternetFileManager object
	if (bHaveAllObjects)
	{
		if (NULL == (m_piInternetFileManager = piInternetFileManagerParam))
		{
			hr = CoCreateInstance(CLSID_InternetFileManager, 
									  NULL, 
									  CLSCTX_INPROC_SERVER,
									  IID_IInternetFileManager,
									  (void**) &m_piInternetFileManager);
			if (FAILED(hr) || NULL == m_piInternetFileManager)
				bHaveAllObjects = FALSE;
			else 
			{
			}
		}
		else
			m_piInternetFileManager->AddRef();
	}

	if (!bHaveAllObjects)
	{
		SAFERELEASE(m_piD3DRM);
		SAFERELEASE(m_piD2DRM);
		SAFERELEASE(m_piDDraw);
		SAFERELEASE(m_piInternetFileManager);

		return VWCONVGEOM_E_BADEXTERNOBJECTS;
	}

	m_bInitCalled = TRUE;

	return S_OK;
}


CConvert3DSGeometry::~CConvert3DSGeometry()
{
	SAFERELEASE(m_piD3DRM);
	SAFERELEASE(m_piD2DRM);
	SAFERELEASE(m_piDDraw);
	SAFERELEASE(m_piInternetFileManager);

}
STDMETHODIMP CConvert3DSGeometry::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IConvert3DSGeometry,
	};
	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CConvert3DSGeometry::ReadScene(BSTR	 bstr, 
										    ITextureCallback* pCGTCallback,
											VARIANT* pvarFrame)  //IDirect3DRMFrame
{
	CString					strFile, strkey;
	file3ds*				fiIn = NULL;
	HRESULT					hr = S_OK;
   	LPDIRECT3DRMFRAME		piD3DFrame = NULL;	// Parent frame object
	LPDIRECT3DRMMATERIAL	piD3DMaterial = NULL;
	POSITION				pos;
	MaterialNode*			matNode = NULL;
	Node*					pNode = NULL;
//	int						nTimeDelta;

//	nTimeDelta = GetTickCount();

	m_listNodes.RemoveAll();

	if (!m_bInitCalled)
	{
		OutputDebugString("CConvert3DSGeometry::ReadScene: Init() has not been called.\n");
		hr = VWCONVGEOM_E_INITNOTCALLED;
		goto EXIT_FAIL;
	}

	m_pCGTCallback = pCGTCallback;
	SAFEADDREF(m_pCGTCallback);

	pvarFrame->vt = VT_EMPTY;
	pvarFrame->punkVal = (IUnknown*) NULL;

	// find the file
	if( bstr == NULL)
	{
		hr = E_INVALIDARG;
		goto EXIT_FAIL;
	}

	strFile = bstr;

	// open input file, or return if we can't open it.
	fiIn = OpenFile3ds(strFile, "rw");
	if (ERROR_IN_3DSFTK)
	{
		CONVERSION3DS_ERROR;
		hr = VWCONVGEOM_E_FILENOTFOUND;
		goto EXIT_FAIL;
	}

	m_p3dsDatabase = NULL;
	InitDatabase3ds( &m_p3dsDatabase );
	if (ERROR_IN_3DSFTK)
	{	
		CONVERSION3DS_ERROR;
		hr = VWCONVGEOM_E_OUTOFMEMORY;
		goto EXIT_FAIL;
	}

	CreateDatabase3ds( fiIn, m_p3dsDatabase );
	if (ERROR_IN_3DSFTK)
	{
		CONVERSION3DS_ERROR;
		hr = VWCONVGEOM_E_BADFILEFORMAT;
		goto EXIT_FAIL;
	}

	hr = m_piD3DRM->CreateFrame(NULL, &piD3DFrame);
	if (FAILED(hr) || !piD3DFrame) goto EXIT_FAIL;

	// do actual conversion
	hr = ConvertFile( piD3DFrame);
	if (FAILED(hr)) goto EXIT_FAIL;

	pvarFrame->vt = VT_UNKNOWN;
	pvarFrame->punkVal = (IUnknown*) piD3DFrame;

	ReleaseDatabase3ds(&m_p3dsDatabase);
	if (ERROR_IN_3DSFTK)
	{
		CONVERSION3DS_ERROR;
		hr = VWCONVGEOM_E_ERRORMEMFREE;
	}

	// close file
	CloseFile3ds(fiIn);
	if (ERROR_IN_3DSFTK)
	{
		CONVERSION3DS_ERROR;
		hr = VWCONVGEOM_E_FILENOTCLOSED;
		goto EXIT_FAIL;
	}


EXIT_FAIL:
	SAFERELEASE(m_pCGTCallback);

	CloseAllFiles3ds();

	//Clear out material list!
	pos = m_mapMaterialList.GetStartPosition();

	while (pos)
	{
		matNode = NULL;
		m_mapMaterialList.GetNextAssoc(pos, strkey, (void*&) matNode);
		delete matNode;
	}
	m_mapMaterialList.RemoveAll();

	pos = m_listNodes.GetHeadPosition();
	while ( pos )
	{
		pNode = (Node*) m_listNodes.GetAt(pos);
		delete pNode;
		m_listNodes.RemoveAt(pos);
		pos = m_listNodes.GetHeadPosition();
	}
	m_listNodes.RemoveAll();

//	nTimeDelta = GetTickCount() - nTimeDelta;
//	TRACE("Converted 3DStudio file in %d ms.\n", nTimeDelta);

	return hr;
}


HRESULT CConvert3DSGeometry::ConvertFile(LPDIRECT3DRMFRAME	piD3DParentFrame)
{
	meshset3ds*		p3dsMeshSet = NULL;
	mesh3ds*		p3dsMesh = NULL;
	kfmesh3ds*		p3dsKeyFrameObject = NULL;
	camera3ds*		p3dsCamera = NULL ;
	atmosphere3ds*  p3dsAtmosphere = NULL;
	background3ds*	p3dsBackground = NULL;
	light3ds*		p3dsLight = NULL;
	material3ds*	p3dsMaterial = NULL;

	ulong3ds		nMeshCount, nMeshIter; //, nObjCount;
	ulong3ds		nLightIter, nLightCount;
	objmat3ds*		p3dsObjectMat;
	MaterialNode*	matNode = NULL;

	HRESULT hr S_OK;
	LPDIRECT3DRMFRAME piD3DLightFrame = NULL;
	LPDIRECT3DRMLIGHT piD3DLight = NULL;
	LPDIRECT3DRMMATERIAL piD3DMaterial = NULL;

	p3dsMeshSet = NULL;
	GetMeshSet3ds( m_p3dsDatabase, &p3dsMeshSet );
	if (ERROR_IN_3DSFTK)
	{
		CONVERSION3DS_ERROR;
		hr = VWCONVGEOM_E_BADFILEFORMAT;
		goto EXIT_FAIL;
	}

	// scale
	m_flMasterScale = m_flMasterScale / p3dsMeshSet->masterscale;

	// ambient lighting
	piD3DLight = NULL;
	hr = m_piD3DRM->CreateLightRGB(D3DRMLIGHT_AMBIENT, 
									   p3dsMeshSet->ambientlight.r,
									   p3dsMeshSet->ambientlight.g,
									   p3dsMeshSet->ambientlight.b,
									   &piD3DLight);
	if (FAILED(hr) || NULL == piD3DLight)
	{
		CONVERSION3DS_ERROR;
		goto EXIT_FAIL;
	}

	piD3DParentFrame->AddLight( piD3DLight );
	SAFERELEASE(piD3DLight);

	// get camera named "AVATAR" to set initial avatar positions
	p3dsCamera = NULL;

	//Set up fog if present
	p3dsAtmosphere = NULL;
	GetAtmosphere3ds(m_p3dsDatabase, &p3dsAtmosphere);  
	if (ERROR_IN_3DSFTK)
	{
		CONVERSION3DS_ERROR;
		hr = VWCONVGEOM_E_BADREAD;
		goto EXIT_FAIL;
	}

	if (UseFog == p3dsAtmosphere->activeatmo)
	{
		D3DCOLOR d3dColor;

		if (p3dsAtmosphere->fog.fardensity > p3dsAtmosphere->fog.neardensity)
			piD3DParentFrame->SetSceneFogParams(p3dsAtmosphere->fog.nearplane,
												p3dsAtmosphere->fog.farplane, 
												p3dsAtmosphere->fog.neardensity / p3dsAtmosphere->fog.fardensity);
		else
			piD3DParentFrame->SetSceneFogParams(p3dsAtmosphere->fog.nearplane,
												p3dsAtmosphere->fog.farplane, 
												p3dsAtmosphere->fog.fardensity / p3dsAtmosphere->fog.neardensity);

		d3dColor = RGB_MAKE((unsigned short) p3dsAtmosphere->fog.fogcolor.r, 
							(unsigned short) p3dsAtmosphere->fog.fogcolor.g,
							(unsigned short) p3dsAtmosphere->fog.fogcolor.b);

		piD3DParentFrame->SetSceneFogColor(d3dColor);
		piD3DParentFrame->SetSceneFogMode(D3DRMFOG_LINEAR);
		piD3DParentFrame->SetSceneFogEnable(TRUE);
	}

	ReleaseAtmosphere3ds(&p3dsAtmosphere);
	if (ERROR_IN_3DSFTK)
	{
		CONVERSION3DS_ERROR;
		hr = VWCONVGEOM_E_ERRORMEMFREE;
		goto EXIT_FAIL;
	}
	p3dsAtmosphere = NULL;

	// set background color
	p3dsBackground = NULL;
	GetBackground3ds( m_p3dsDatabase, &p3dsBackground  );
	if (ERROR_IN_3DSFTK)
	{
		CONVERSION3DS_ERROR;
		hr = VWCONVGEOM_E_BADREAD;
		goto EXIT_FAIL;
	}

	if( UseSolidBgnd == p3dsBackground->bgndused)
	{
		piD3DParentFrame->SetSceneBackgroundRGB(p3dsBackground->solid.color.r,
												p3dsBackground->solid.color.g,
												p3dsBackground->solid.color.b );
	}
	else
	{
		piD3DParentFrame->SetSceneBackgroundRGB(fDefBackgroundRed,
												fDefBackgroundGreen,
												fDefBackgroundBlue);
	}

	ReleaseBackground3ds(&p3dsBackground );
	if (ERROR_IN_3DSFTK)
	{
		CONVERSION3DS_ERROR;
		hr = VWCONVGEOM_E_ERRORMEMFREE;
		goto EXIT_FAIL;
	}
	p3dsBackground = NULL;

	// get lights
	// handle omni lights
	nLightCount = GetOmnilightCount3ds( m_p3dsDatabase );
	for( nLightIter = 0; nLightIter < nLightCount; nLightIter++ )
	{
		p3dsLight = NULL;
		GetOmnilightByIndex3ds( m_p3dsDatabase, nLightIter, &p3dsLight );
		if (ERROR_IN_3DSFTK)
		{
			CONVERSION3DS_ERROR;
			hr = VWCONVGEOM_E_BADREAD;
			goto EXIT_FAIL;
		}

		if( !p3dsLight->dloff )
		{
			hr = m_piD3DRM->CreateFrame( piD3DParentFrame, &piD3DLightFrame);
			if (D3DRM_OK != hr || NULL == piD3DLightFrame)
			{
				CONVERSION3DS_ERROR;
				goto EXIT_FAIL;
			}

			piD3DLightFrame->SetPosition( piD3DParentFrame, 
										  (D3DVALUE) (p3dsLight->pos.x / m_flMasterScale), 
										  (D3DVALUE) (p3dsLight->pos.z / m_flMasterScale),
										  (D3DVALUE) (p3dsLight->pos.y / m_flMasterScale) );

			hr = m_piD3DRM->CreateLightRGB(D3DRMLIGHT_PARALLELPOINT, 
										  p3dsLight->color.r,
									      p3dsLight->color.g,
									      p3dsLight->color.b,
										  &piD3DLight);
			if (FAILED(hr) || NULL == piD3DLight)
			{
				CONVERSION3DS_ERROR;
				goto EXIT_FAIL;
			}

			hr = piD3DLightFrame->AddLight(piD3DLight);
			if (D3DRM_OK != hr)
			{
				CONVERSION3DS_ERROR;
				goto EXIT_FAIL;
			}
			SAFERELEASE(piD3DLightFrame);
			SAFERELEASE(piD3DLight);
		}
		ReleaseLight3ds( &p3dsLight );
		if (ERROR_IN_3DSFTK)
		{
			CONVERSION3DS_ERROR;
			goto EXIT_FAIL;
		}
		p3dsLight = NULL;

	}

	// handle omni lights
	nLightCount = GetSpotlightCount3ds( m_p3dsDatabase );
	for( nLightIter = 0; nLightIter < nLightCount; nLightIter++ )
	{
		InitLight3ds( &p3dsLight );

		p3dsLight = NULL;
		GetSpotlightByIndex3ds( m_p3dsDatabase, nLightIter, &p3dsLight );
		if (ERROR_IN_3DSFTK)
		{
			CONVERSION3DS_ERROR;
			hr = VWCONVGEOM_E_BADREAD;
			goto EXIT_FAIL;
		}

		if( !p3dsLight->dloff )
		{
			hr = m_piD3DRM->CreateFrame( piD3DParentFrame, &piD3DLightFrame);
			if (D3DRM_OK != hr || NULL == piD3DLightFrame)
			{
				CONVERSION3DS_ERROR;
				ReleaseLight3ds( &p3dsLight );
				goto EXIT_FAIL;
			}


			piD3DLightFrame->SetPosition( piD3DParentFrame, 
										  (D3DVALUE) (p3dsLight->pos.x / m_flMasterScale), 
										  (D3DVALUE) (p3dsLight->pos.z / m_flMasterScale),
										  (D3DVALUE) (p3dsLight->pos.y / m_flMasterScale) );

			piD3DLightFrame->SetOrientation(piD3DParentFrame,
											0.0F,
											-1.0F,
											0.0F,
											0.0F,
											0.0F,
											1.0F );

			hr = m_piD3DRM->CreateLightRGB(D3DRMLIGHT_DIRECTIONAL, 
										     p3dsLight->color.r,
									         p3dsLight->color.g,
											 p3dsLight->color.b,
											 &piD3DLight);
			if (FAILED(hr) || NULL == piD3DLight)
			{
				CONVERSION3DS_ERROR;
				goto EXIT_FAIL;
			}

			piD3DLight->SetPenumbra( p3dsLight->spot->falloff );
			piD3DLight->SetUmbra( p3dsLight->spot->hotspot );
			hr = piD3DLightFrame->AddLight(piD3DLight);
			if (D3DRM_OK != hr)
			{
				CONVERSION3DS_ERROR;
				goto EXIT_FAIL;
			}
		}

		ReleaseLight3ds( &p3dsLight );
		if (ERROR_IN_3DSFTK)
		{
			CONVERSION3DS_ERROR;
			hr = VWCONVGEOM_E_ERRORMEMFREE ;
			goto EXIT_FAIL;
		}
		p3dsLight = NULL;
	}

	// iterate through meshes, first get how many.
	nMeshCount = GetMeshCount3ds(m_p3dsDatabase);
	if (ERROR_IN_3DSFTK)
	{
		CONVERSION3DS_ERROR;
		hr = VWCONVGEOM_E_BADREAD;
		goto EXIT_FAIL;
	}

	//Loop for each mesh in the file
	for( nMeshIter = 0; nMeshIter < nMeshCount; nMeshIter++ )
	{
		Node *pnode;
		int nMat;

		// alloc and initialize mesh struction
		p3dsMesh = NULL;
		GetMeshByIndex3ds( m_p3dsDatabase, nMeshIter, &p3dsMesh );
		if (ERROR_IN_3DSFTK)
		{
			CONVERSION3DS_ERROR;
			hr = VWCONVGEOM_E_BADREAD;
			goto EXIT_FAIL;
		}

		// add to node list
		pnode = new Node( p3dsMesh, nMeshIter );
		if (!pnode)
		{
			CONVERSION3DS_ERROR;
			hr = VWCONVGEOM_E_OUTOFMEMORY;
			goto EXIT_FAIL;
		}

		m_listNodes.Add( pnode );

		//3DSObjects can have multiple materials.  We'll search for each material
		//in our created material list.  If we can't find it, we'll create it.
		//We don't actually assign the material until stage two though.
		for( nMat = 0; nMat < p3dsMesh->nmats; nMat++ )
		{
			p3dsObjectMat = &p3dsMesh->matarray[nMat];

			if ( !(m_mapMaterialList.Lookup(p3dsObjectMat->name, (void*&) matNode )) )
			{
				//We didn't find the material, so let's create it!

				matNode = new MaterialNode();

				if (!matNode)
				{
					CONVERSION3DS_ERROR;
					hr = VWCONVGEOM_E_OUTOFMEMORY;
					goto EXIT_FAIL;
				}

				piD3DMaterial = NULL;
				hr = m_piD3DRM->CreateMaterial(1.0, &piD3DMaterial);
				if (FAILED(hr) || !piD3DMaterial)
				{
					CONVERSION3DS_ERROR;
					goto EXIT_FAIL;
				}

				GetMaterialByName3ds(m_p3dsDatabase, p3dsObjectMat->name, &p3dsMaterial);
				if (ERROR_IN_3DSFTK)
				{
					OutputDebugString("Error, named material is not in file.\n");
					CONVERSION3DS_ERROR;
					goto EXIT_FAIL;
				}

				piD3DMaterial->SetSpecular(p3dsMaterial->specular.r, p3dsMaterial->specular.g, p3dsMaterial->specular.b);
				piD3DMaterial->SetPower(p3dsMaterial->shinstrength);
				piD3DMaterial->SetEmissive(p3dsMaterial->selfillumpct, p3dsMaterial->selfillumpct, p3dsMaterial->selfillumpct);

				matNode->m_piD3DMaterial = piD3DMaterial;
				matNode->m_p3dsMaterial = p3dsMaterial;

				m_mapMaterialList.SetAt(p3dsObjectMat->name, matNode);

				p3dsMaterial = NULL;
			}
		}
	}

	// now dump object list
	hr = m_listNodes.Dump(piD3DParentFrame, this);


EXIT_FAIL:

	SAFERELEASE(piD3DLight);
	SAFERELEASE(piD3DLightFrame);

	if (p3dsMaterial)
	{
		ReleaseMaterial3ds( &p3dsMaterial );
		if (ERROR_IN_3DSFTK)
		{
			CONVERSION3DS_ERROR;
			hr = VWCONVGEOM_E_ERRORMEMFREE;
		}
	}

	if (p3dsBackground)
	{
		ReleaseBackground3ds(&p3dsBackground );
		if (ERROR_IN_3DSFTK)
		{
			CONVERSION3DS_ERROR;
			hr = VWCONVGEOM_E_ERRORMEMFREE;
		}
	}


	if (p3dsAtmosphere)
	{
		ReleaseAtmosphere3ds(&p3dsAtmosphere);
		if (ERROR_IN_3DSFTK)
		{
			CONVERSION3DS_ERROR;
			hr = VWCONVGEOM_E_ERRORMEMFREE;
		}
	}

	if (p3dsLight)
	{
		ReleaseLight3ds( &p3dsLight );
		if (ERROR_IN_3DSFTK)
		{
			CONVERSION3DS_ERROR;
			hr = VWCONVGEOM_E_ERRORMEMFREE;
		}
	}

	if (p3dsMeshSet)
	{
		ReleaseMeshSet3ds( &p3dsMeshSet );
		if (ERROR_IN_3DSFTK)
		{
			CONVERSION3DS_ERROR;
			hr = VWCONVGEOM_E_ERRORMEMFREE;
		}
	}

	return hr;
}


// NodeList methods ////////////////////////////////////////////
CConvert3DSGeometry::NodeList::NodeList() : m_list()
{
}

CConvert3DSGeometry::NodeList::~NodeList()
{
	Node* pNode;

	while ( !m_list.IsEmpty() )
	{
		pNode = (Node*) m_list.RemoveHead();
		delete pNode;
	}
}

POSITION CConvert3DSGeometry::NodeList::Find( const char * pszName )
{
	POSITION pos, posRet;

	// iterate, looking for pszName
	for( pos = m_list.GetHeadPosition(); pos != NULL; )
	{
		Node *pnode;

		posRet = pos;
		pnode = (Node *) m_list.GetNext( pos );
		ASSERT( pnode != NULL );

		if( strcmp( pnode->m_pszName, pszName ) == 0 )
			return posRet;
		else
		{
			// if have children, check there
			if( pnode->m_listChildren.GetCount() != 0 )
			{
				posRet = pnode->m_listChildren.Find( pszName );
				if( posRet != NULL )
					return posRet;
			}
		}		
	}

	return NULL;
}

HRESULT CConvert3DSGeometry::NodeList::Dump(LPDIRECT3DRMFRAME piD3DParentFrame, CConvert3DSGeometry* pConverterObject)
{
	POSITION	pos;
	Node*		pnode;
	HRESULT hr;

	for( pos = m_list.GetHeadPosition(); pos!=NULL; )
	{
		pnode = (Node *) m_list.GetNext(pos);
		ASSERT( pnode != NULL );
		
		hr = pnode->Dump(piD3DParentFrame, pConverterObject);
		if (FAILED(hr) )
			break;
	}

	return hr;
}

// Node methods ////////////////////////////////////////////////


CConvert3DSGeometry::Node::Node( mesh3ds *p3dsMesh, ulong3ds nMeshIter) : m_listChildren()
{
	m_pszName = strdup(p3dsMesh->name);

	m_imesh = nMeshIter;
	m_p3dsMesh = p3dsMesh;
	m_pnodeParent = NULL;

	m_pnt3dsOffset.x = p3dsMesh->locmatrix[ 9];
	m_pnt3dsOffset.y = p3dsMesh->locmatrix[10];
	m_pnt3dsOffset.z = p3dsMesh->locmatrix[11];
}

void CConvert3DSGeometry::Node::AddChild( Node *pnode )
{
	pnode->m_pnodeParent = this;
	m_listChildren.Add( pnode );
}


CConvert3DSGeometry::Node::~Node() 
{ 
	free(m_pszName); 

	RelMeshObj3ds( &m_p3dsMesh );
}


void CConvert3DSGeometry::Node::SetPivot( point3ds *pPnt3dsPivot )
{
	m_pnt3dsOffset.x += pPnt3dsPivot->x;
	m_pnt3dsOffset.y += pPnt3dsPivot->y;
	m_pnt3dsOffset.z += pPnt3dsPivot->z;
}

HRESULT CConvert3DSGeometry::Node::Dump( LPDIRECT3DRMFRAME piD3DParentFrame, CConvert3DSGeometry* pConverterObject)
{
	HRESULT hr = S_OK;

	//D3DRM Objects
	LPDIRECT3DRMMESHBUILDER piD3DRMMeshBuilder = NULL;
	LPDIRECT3DRMTEXTURE piD3DRMTexture = NULL;
	LPDIRECT3DRMFRAME piD3DDummyFrame = NULL;
	LPDIRECT3DRMFACE piD3DRMFace = NULL;
	LPDIRECT2DRMIMAGE piD2DImage = NULL;
	LPDIRECT3DRMFACEARRAY piD3DRMFaceArray = NULL;

	// Flags
	BOOL bAllMaterialsWireframe = TRUE;
	BOOL bHaveTexture = FALSE;

	// Temporary 3ds tool kit objects
	point3ds pnt3dsOffset;
	objmat3ds *p3dsObjectMat  = NULL;

	// Indexing
	int nFace;
	int nVertex;
	int nMat; 
	MaterialNode*			matNode = NULL;

	// pointers for allocation face info.
	D3DVECTOR*	pvecVertexs = NULL;
	D3DVECTOR*	pvecNormals = NULL;
	BOOL*		pbNormDone = NULL;
	DWORD		*pnFaceData = NULL;
	DWORD		*pnTemp = NULL;   // Temp pointer, never malloc'ed

	// Strings
	char szBuffer[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_bstr_t bstrRemoteFilename;
	CComBSTR bstrFullURL;
	BSTR bstrFilename;
	_bstr_t bstrTmp;

	if( m_p3dsMesh->ntextverts && m_p3dsMesh->ntextverts != m_p3dsMesh->nvertices )
	{
		CONVERSION3DS_ERROR;
		hr = VWCONVGEOM_E_BADREAD;
		goto EXIT_FAIL;
	}

	// calculate relative offset
	if( m_pnodeParent != NULL )
	{
		pnt3dsOffset.x = m_pnt3dsOffset.x - m_pnodeParent->m_pnt3dsOffset.x;
		pnt3dsOffset.y = m_pnt3dsOffset.y - m_pnodeParent->m_pnt3dsOffset.y;
		pnt3dsOffset.z = m_pnt3dsOffset.z - m_pnodeParent->m_pnt3dsOffset.z;
	}
	else
	{
		pnt3dsOffset = m_pnt3dsOffset;
	}

	//create a frame for the geometry
	hr = pConverterObject->m_piD3DRM->CreateFrame(piD3DParentFrame, &piD3DDummyFrame);
	if (hr != D3DRM_OK || NULL == piD3DDummyFrame)
	{
		CONVERSION3DS_ERROR;
		goto EXIT_FAIL;
	}

	//Set it's name from the 3dstudio name
	strcpy(szBuffer, m_p3dsMesh->name);
	CleanNameString(szBuffer);
	piD3DDummyFrame->SetName(szBuffer);

	// add translation
	piD3DDummyFrame->AddTranslation(D3DRMCOMBINE_AFTER,
									(D3DVALUE)(pnt3dsOffset.x / pConverterObject->m_flMasterScale),
									(D3DVALUE)(pnt3dsOffset.z / pConverterObject->m_flMasterScale),
									(D3DVALUE)(pnt3dsOffset.y / pConverterObject->m_flMasterScale) );

	// setup for adding faces to mesh
	// create vertex, normal and flag list
	pvecVertexs = (D3DVECTOR*) malloc( sizeof(D3DVECTOR) * m_p3dsMesh->nvertices );
	if( pvecVertexs == NULL )
	{
		CONVERSION3DS_ERROR;
		hr = VWCONVGEOM_E_OUTOFMEMORY;
		goto EXIT_FAIL;
	}

	// copy and scale vertices - zero flag
	for( nVertex = 0; nVertex < m_p3dsMesh->nvertices; nVertex++ )
	{
		if (pConverterObject->m_flMasterScale != 1.0f)
		{
			pvecVertexs[nVertex].x = (float)((m_p3dsMesh->vertexarray[nVertex].x - m_pnt3dsOffset.x) / pConverterObject->m_flMasterScale);
			pvecVertexs[nVertex].y = (float)((m_p3dsMesh->vertexarray[nVertex].z - m_pnt3dsOffset.z) / pConverterObject->m_flMasterScale);
			pvecVertexs[nVertex].z = (float)((m_p3dsMesh->vertexarray[nVertex].y - m_pnt3dsOffset.y) / pConverterObject->m_flMasterScale);
		}
		else
		{
			pvecVertexs[nVertex].x = m_p3dsMesh->vertexarray[nVertex].x - m_pnt3dsOffset.x;
			pvecVertexs[nVertex].y = m_p3dsMesh->vertexarray[nVertex].z - m_pnt3dsOffset.z;
			pvecVertexs[nVertex].z = m_p3dsMesh->vertexarray[nVertex].y - m_pnt3dsOffset.y;
		}
	}

	// create and add visual
	hr = pConverterObject->m_piD3DRM->CreateMeshBuilder(&piD3DRMMeshBuilder);
	if (FAILED(hr) || NULL == piD3DRMMeshBuilder)
	{
		CONVERSION3DS_ERROR;
		hr = VWCONVGEOM_E_OUTOFMEMORY;
		goto EXIT_FAIL;
	}

	// add visual object to scene
	hr = piD3DDummyFrame->AddVisual( piD3DRMMeshBuilder );
	if (FAILED(hr))	
	{
		CONVERSION3DS_ERROR;
		hr = VWCONVGEOM_E_OUTOFMEMORY;
		goto EXIT_FAIL;
	}

	// allocate face info.
	// for each face, we need one int for the vertex count (which will be 3), then 
	// 6 ints for the vertex and normal indices (3*2).  Finally, we need a
	// one int terminator
	pnFaceData = (DWORD*) malloc( m_p3dsMesh->nfaces * sizeof(DWORD) * 4 + sizeof(DWORD));
	if( pnFaceData == NULL )
	{
		CONVERSION3DS_ERROR;
		hr = VWCONVGEOM_E_OUTOFMEMORY;
		goto EXIT_FAIL;
	}

	pnTemp = pnFaceData;
	for( nFace = 0; nFace < m_p3dsMesh->nfaces; nFace++ )
	{
		if (m_p3dsMesh->facearray[nFace].v3 >= m_p3dsMesh->nvertices ||
			m_p3dsMesh->facearray[nFace].v2 >= m_p3dsMesh->nvertices ||
			m_p3dsMesh->facearray[nFace].v1 >= m_p3dsMesh->nvertices)
		{
			// Attempting to use this as an index will result in an exception.
			hr = E_FAIL;
			goto EXIT_FAIL;
		}
		*pnTemp++ = 3;
		*pnTemp++ = m_p3dsMesh->facearray[nFace].v3;
		*pnTemp++ = m_p3dsMesh->facearray[nFace].v2;
		*pnTemp++ = m_p3dsMesh->facearray[nFace].v1;
	}
	// terminate
	*pnTemp = 0;

	hr = piD3DRMMeshBuilder->AddFaces(m_p3dsMesh->nvertices,
									  pvecVertexs,
									  0, 
									  NULL, 
								      pnFaceData,
									  &piD3DRMFaceArray);
	if (FAILED(hr))	goto EXIT_FAIL;

	// set texture coordinates
	for( nVertex = 0; nVertex < m_p3dsMesh->ntextverts; nVertex++ )
	{
		piD3DRMMeshBuilder->SetTextureCoordinates(nVertex, m_p3dsMesh->textarray[nVertex].u, -m_p3dsMesh->textarray[nVertex].v );
	}

	for( nMat = 0; nMat < m_p3dsMesh->nmats; nMat++ )
	{
		p3dsObjectMat = &m_p3dsMesh->matarray[nMat];
		matNode = NULL;
		pConverterObject->m_mapMaterialList.Lookup(p3dsObjectMat->name, (void*&) matNode );
		if (!matNode)
		{
			CONVERSION3DS_ERROR;
			hr = VWCONVGEOM_E_BADMATERIAL;
			goto EXIT_FAIL;
		}

		//For each face in the object's material list
		for( nFace = 0; nFace < p3dsObjectMat->nfaces; nFace++ )
		{
			//Get ahold of the face we're going to assign stuff to.
			piD3DRMFace = NULL;
			hr = piD3DRMFaceArray->GetElement(p3dsObjectMat->faceindex[nFace], &piD3DRMFace );
			if (FAILED(hr) || NULL == piD3DRMFace)
			{
				CONVERSION3DS_ERROR;
				goto EXIT_FAIL;
			}

			if (!matNode->m_p3dsMaterial->usewire)
				bAllMaterialsWireframe = FALSE;
			
			//Assign it a material if it has one
			if (matNode->m_piD3DMaterial)
				hr = piD3DRMFace->SetMaterial(matNode->m_piD3DMaterial);

			// If LoadTexture property is true and there's a texture to load, do so.
			if( pConverterObject->m_bLoadTextures && 
				matNode->m_p3dsMaterial && 
				matNode->m_p3dsMaterial->texture.map.name[0] != '\0' )
			{
				bstrRemoteFilename = matNode->m_p3dsMaterial->texture.map.name;

				//If they've passed us a valid Callback, we should use it above all else
				if (pConverterObject->m_pCGTCallback)
				{
					hr = pConverterObject->m_pCGTCallback->LoadTextureCallback(bstrRemoteFilename, &piD3DRMTexture );
					if (FAILED(hr))
					{
						//Can't load the file.  It's ok, just assign a NULL texture.
						piD3DRMTexture = NULL;
						hr = S_OK;
						//CONVERSION3DS_ERROR;
						//goto EXIT_FAIL;
					}

					hr = piD3DRMFace->SetTexture( piD3DRMTexture );
					if (FAILED(hr))
					{
						CONVERSION3DS_ERROR;
						goto EXIT_FAIL;
					}

					SAFERELEASE(piD3DRMTexture);

					piD3DRMFace->SetColorRGB((D3DVALUE)1.0f, (D3DVALUE)1.0f, (D3DVALUE)1.0f );	
//Review:			piD3DRMFace->SetColorRGB((D3DVALUE)matNode->m_p3dsMaterial->diffuse.r, (D3DVALUE)matNode->m_p3dsMaterial->diffuse.g, (D3DVALUE)matNode->m_p3dsMaterial->diffuse.b );	
					bHaveTexture = TRUE;

				}
				else //No texture callback, try and load it ourselves.
				{
					//First check if the texture has already been loaded
					piD3DRMTexture = NULL;
					hr = pConverterObject->m_piD3DRM->GetNamedObject(matNode->m_p3dsMaterial->texture.map.name, (LPDIRECT3DRMOBJECT*) &piD3DRMTexture);
					if (SUCCEEDED(hr) && piD3DRMTexture )
					{
						hr = piD3DRMFace->SetTexture( piD3DRMTexture );
						SAFERELEASE(piD3DRMTexture);
					}
					else //
					{
						//bstrRemoteFilename = p3dsMaterial->texture.map.name;
						//Try to find the file with INetFile.  If not found, try to open locally.
						hr = pConverterObject->m_piInternetFileManager->GetFileEx(bstrRemoteFilename, VARIANT_FALSE, &bstrFullURL.m_str, &bstrFilename );
						if (FAILED(hr) )
						{
							hr = S_OK;
							bstrFilename = bstrRemoteFilename;
						}
						
						piD2DImage = NULL;
						piD3DRMTexture = NULL;

						if (!FAILED(hr) &&
							!FAILED(hr = pConverterObject->m_piD2DRM->CreateImageFromImageFile(bstrFilename, 0, &piD2DImage)))
						{
							if (!FAILED(hr = piD2DImage->GetD3DTexture(pConverterObject->m_piD3DRM, &piD3DRMTexture)) )
							{
								//Set the correct file name for the texture
								if (pConverterObject->m_bKeepCachedTextureNames)  //Use new name
								{
									bstrTmp = bstrFilename;
									_splitpath( bstrTmp, drive, dir, fname, ext );
									strcpy(szBuffer, fname);
									strcat(szBuffer, ".");
									strcat(szBuffer, ext);
								}
								else							//Keep old name
									strcpy(szBuffer, matNode->m_p3dsMaterial->texture.map.name);

								//CleanNameString(szBuffer);
								piD3DRMTexture->SetName(szBuffer);

								hr = piD3DRMFace->SetTexture( piD3DRMTexture );
								if (FAILED(hr))
								{
									CONVERSION3DS_ERROR;
									hr = VWCONVGEOM_E_BADREAD;
									goto EXIT_FAIL;
								}

								SAFERELEASE(piD3DRMTexture);

								piD3DRMFace->SetColorRGB((D3DVALUE)1.0f, (D3DVALUE)1.0f, (D3DVALUE)1.0f );	
//REVIEW:						piD3DRMFace->SetColorRGB((D3DVALUE)matNode->m_p3dsMaterial->diffuse.r, (D3DVALUE)matNode->m_p3dsMaterial->diffuse.g, (D3DVALUE)matNode->m_p3dsMaterial->diffuse.b );	
								bHaveTexture = TRUE;
							}
							SAFERELEASE(piD2DImage);
						}
						else
						{
							char sz[_MAX_PATH+64];

							sprintf( sz, "Could not load texture file: %s\n", matNode->m_p3dsMaterial->texture.map.name );
							OutputDebugString(sz);

							piD3DRMFace->SetColorRGB((D3DVALUE)matNode->m_p3dsMaterial->diffuse.r, (D3DVALUE)matNode->m_p3dsMaterial->diffuse.g, (D3DVALUE)matNode->m_p3dsMaterial->diffuse.b );	
						}
					}
				}
			}
			else //There's no texture or we're no supposed to load it.
			{
				piD3DRMFace->SetColorRGB((D3DVALUE)matNode->m_p3dsMaterial->diffuse.r, (D3DVALUE)matNode->m_p3dsMaterial->diffuse.g, (D3DVALUE)matNode->m_p3dsMaterial->diffuse.b );	
			}

			// release current face
			SAFERELEASE(piD3DRMFace);
		}
	}

	if( bHaveTexture )
	{
		piD3DRMMeshBuilder->SetPerspective( TRUE );
	}

	if (bAllMaterialsWireframe)
		piD3DRMMeshBuilder->SetQuality( D3DRMRENDER_GOURAUD | D3DRMLIGHT_ON | D3DRMFILL_WIREFRAME );
	else
		piD3DRMMeshBuilder->SetQuality( D3DRMRENDER_PHONG | D3DRMLIGHT_ON | D3DRMFILL_SOLID );

	// now dump out children
	if( m_listChildren.GetCount() > 0 )
		hr = m_listChildren.Dump(piD3DDummyFrame, pConverterObject);

EXIT_FAIL:

	// free stuff
	if (pvecVertexs)
		free( pvecVertexs );
	if (pvecNormals)
		free( pvecNormals );
	if (pbNormDone)
		free( pbNormDone );
	if (pnFaceData)
		free( pnFaceData );

	SAFERELEASE(piD3DRMMeshBuilder);
	SAFERELEASE(piD3DRMTexture);
	SAFERELEASE(piD3DDummyFrame);
	SAFERELEASE(piD3DRMFace);
	SAFERELEASE(piD2DImage);
	SAFERELEASE(piD3DRMFaceArray);
	return hr;
}


STDMETHODIMP CConvert3DSGeometry::get_ScaleFactor(double * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (NULL != pVal)
	{
		*pVal = m_flMasterScale;
		return S_OK;
	}
	else
		return E_INVALIDARG;

}

STDMETHODIMP CConvert3DSGeometry::put_ScaleFactor(double newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_flMasterScale = newVal;

	return S_OK;
}


STDMETHODIMP CConvert3DSGeometry::get_ConvertAnimation(VARIANT_BOOL* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_bConvertAnimations;

	return S_OK;
}

STDMETHODIMP CConvert3DSGeometry::put_ConvertAnimation(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_bConvertAnimations = newVal;

	return S_OK;
}

STDMETHODIMP CConvert3DSGeometry::Name(BSTR* bstr)
{
	char charResString[_MAX_PATH];

	LoadString(_Module.GetResourceInstance(), 
		   IDS_NAME, 
		   charResString, 
		   _MAX_PATH);

	_bstr_t bstrTemp(charResString); 

	*bstr = bstrTemp.copy();
	return S_OK;
}

STDMETHODIMP CConvert3DSGeometry::Extensions(BSTR* bstr)
{
	char charResString[_MAX_PATH];

	LoadString(_Module.GetResourceInstance(), 
		   IDS_EXTENSION, 
		   charResString, 
		   _MAX_PATH);

	_bstr_t bstrTemp(charResString); 

	*bstr = bstrTemp.copy();
	return S_OK;
}


STDMETHODIMP CConvert3DSGeometry::get_LoadTextures(VARIANT_BOOL * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = (m_bLoadTextures ? VARIANT_TRUE : VARIANT_FALSE);

	return S_OK;
}

STDMETHODIMP CConvert3DSGeometry::put_LoadTextures(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_bLoadTextures = (VARIANT_FALSE == newVal ? FALSE : TRUE);

	return S_OK;
}

STDMETHODIMP CConvert3DSGeometry::get_KeepCachedTextureNames(VARIANT_BOOL * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = (m_bKeepCachedTextureNames ? VARIANT_TRUE : VARIANT_FALSE);

	return S_OK;
}

STDMETHODIMP CConvert3DSGeometry::put_KeepCachedTextureNames(VARIANT_BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_bKeepCachedTextureNames = (VARIANT_FALSE == newVal ? FALSE : TRUE);

	return S_OK;
}

void CleanNameString(char* szNameString)
{
	int nLen = strlen(szNameString);
	int nPos = 0;
	char* szTmp = new char[nLen + 1];

	for (int i = 0; i < nLen; i++)
	{
		if (isalnum(szNameString[i]) )
		{
			szTmp[nPos++] = szNameString[i];
		}
	}
	szTmp[nPos] = 0;
	strcpy(szNameString, szTmp);
	delete [] szTmp;
}


/*
	// now iterate through keyframer object data
	nObjCount = GetObjectNodeCount3ds(m_p3dsDatabase);
	if (ERROR_IN_3DSFTK)
	{
		CONVERSION3DS_ERROR;
		hr = VWCONVGEOM_E_BADREAD;
		goto EXIT_FAIL;
	}
	ulong3ds nObjIter;
	for( nObjIter = 0; nObjIter < nObjCount; nObjIter++ )
	{
		POSITION posThis;
		Node *pnodeThis;

		// alloc and initialize kf object structure
		p3dsKeyFrameObject = NULL;
		GetObjectMotionByIndex3ds( p3dsDatabase, nObjIter, &p3dsKeyFrameObject);
		if (ERROR_IN_3DSFTK)
		{
			CONVERSION3DS_ERROR;
			ReleaseDatabase3ds( &p3dsDatabase );
			return VWCONVGEOM_E_BADREAD;
		}

		// find this node in the main list
		posThis = m_listNodes.Find( p3dsKeyFrameObject->name );
		if( posThis == NULL )
		{
			CONVERSION3DS_ERROR;
			ReleaseObjectMotion3ds( &p3dsKeyFrameObject );
			continue;
		}
		pnodeThis = m_listNodes.GetAt( posThis );

		// if the parent is non-null, delete the node from the main list and add it to the parent
		if( p3dsKeyFrameObject->parent[0] != '\0' )
		{
			POSITION posParent;
			Node *pnodeParent;

			// find the parent node
			posParent = m_listNodes.Find( p3dsKeyFrameObject->parent );
			if( posParent == NULL )
			{
				CONVERSION3DS_ERROR;
				ReleaseObjectMotion3ds( &p3dsKeyFrameObject );
				continue;
			}

			// delete child
			m_listNodes.RemoveAt( posThis );

			// add to parent node
			pnodeParent = m_listNodes.GetAt( posParent );
			pnodeParent->AddChild( pnodeThis );
	
		}

		// set pivot point
		pnodeThis->SetPivot( &p3dsKeyFrameObject->pivot );

		// release 
		ReleaseObjectMotion3ds( &p3dsKeyFrameObject );
		if (ERROR_IN_3DSFTK)
		{
			CONVERSION3DS_ERROR;
			ReleaseDatabase3ds( &p3dsDatabase );
			return VWCONVGEOM_E_ERRORMEMFREE;
		}
	}
*/


		/*
	if (convertAnimations)
	{
		//Do the animation conversion, if there's any to convert
		kfmesh3ds *p3dsKeyFrameObject;
		LPDIRECT3DRMANIMATION piD3DRMAnim;
		D3DRMQUATERNION quatRotation;
		D3DVECTOR vecRotation;
		unsigned int nFrameNum;

		p3dsKeyFrameObject = NULL;
		GetObjectMotionByName3ds(p3dsDatabase, p3dsMesh->name, &p3dsKeyFrameObject);
		if (ERROR_IN_3DSFTK)
		{
			CONVERSION3DS_ERROR;
			free( pnMapF2M );
			free( pvecVertexs );
			free( pvecNormals );
			free( pbNormDone );
			free( pnFaceData );
			RelMeshObj3ds( &p3dsMesh );
			return VWCONVGEOM_E_BADREAD;
		}

		//If we have either position, scale, or rotation animation
		if (p3dsKeyFrameObject->npkeys || p3dsKeyFrameObject->nskeys || p3dsKeyFrameObject->nrkeys)
		{
			hr = piD3DRM->CreateAnimation(&piD3DRMAnim);
			if (FAILED(hr) || NULL == piD3DRMAnim)
			{
				CONVERSION3DS_ERROR;
				ReleaseObjectMotion3ds( &p3dsKeyFrameObject );
				RelMeshObj3ds( &p3dsMesh );
				free( pnMapF2M );
				free( pvecVertexs );
				free( pvecNormals );
				free( pbNormDone );
				free( pnFaceData );
				return hr;
			}
			piD3DRMAnim->SetFrame(piD3DDummyFrame);
			if ( (p3dsKeyFrameObject->npflag & TrackLoops3ds == TrackLoops3ds) ||
				 (p3dsKeyFrameObject->npflag & TrackRepeats3ds == TrackRepeats3ds))
				piD3DRMAnim->SetOptions(D3DRMANIMATION_CLOSED);
			else
				piD3DRMAnim->SetOptions(D3DRMANIMATION_OPEN);

			//Set up position frames
			for (nFrameNum = 0; nFrameNum < p3dsKeyFrameObject->npkeys; nFrameNum++)
			{
				piD3DRMAnim->AddPositionKey((D3DVALUE)p3dsKeyFrameObject->pkeys[nFrameNum].time,
											p3dsKeyFrameObject->pos[nFrameNum].x, 
											p3dsKeyFrameObject->pos[nFrameNum].y,
											p3dsKeyFrameObject->pos[nFrameNum].z );
			}
			//Set up rotation frames
			for (nFrameNum = 0; nFrameNum < p3dsKeyFrameObject->nrkeys; nFrameNum++)
			{
				vecRotation.x = p3dsKeyFrameObject->rot[nFrameNum].x;
				vecRotation.y = p3dsKeyFrameObject->rot[nFrameNum].y;
				vecRotation.z = p3dsKeyFrameObject->rot[nFrameNum].z;
				D3DRMQuaternionFromRotation(&quatRotation, 
											&vecRotation, 
											p3dsKeyFrameObject->rot[nFrameNum].angle);
				
				piD3DRMAnim->AddRotateKey((D3DVALUE)p3dsKeyFrameObject->rkeys[nFrameNum].time,
											&quatRotation);
			}
			//Set up scaling frames
			for (nFrameNum = 0; nFrameNum < p3dsKeyFrameObject->nskeys; nFrameNum++)
			{
				piD3DRMAnim->AddScaleKey((D3DVALUE)p3dsKeyFrameObject->skeys[nFrameNum].time,
										 p3dsKeyFrameObject->scale[nFrameNum].x, 
										 p3dsKeyFrameObject->scale[nFrameNum].y,
										 p3dsKeyFrameObject->scale[nFrameNum].z );
			}


		}
		ReleaseObjectMotion3ds( &p3dsKeyFrameObject );
		if (ERROR_IN_3DSFTK)
		{
			CONVERSION3DS_ERROR;
			free( pnMapF2M );
			free( pvecVertexs );
			free( pvecNormals );
			free( pbNormDone );
			free( pnFaceData );
			return VWCONVGEOM_E_ERRORMEMFREE;
		}
	}
*/

/*
	D3DRMIMAGE  d3dImage;

					else  //Load a stub for the texture with the correct name
					{
						//REVIEW: Not tested and probably uses too much memory for practical use
						//Suggest adding texture to material node

						ZeroMemory(&d3dImage, sizeof D3DRMIMAGE);
						d3dImage.width = d3dImage.height = 1;
						d3dImage.aspectx = d3dImage.aspecty = 1;
						d3dImage.depth = 16;
						d3dImage.rgb = TRUE;

						pConverterObject->m_piD3DRM->CreateTexture(&d3dImage, &piD3DRMTexture);
						if (FAILED(hr))
						{
							CONVERSION3DS_ERROR;
							hr = VWCONVGEOM_E_BADREAD;
							goto EXIT_FAIL;
						}

						strcpy(szBuffer, p3dsMaterial->texture.map.name);
						//CleanNameString(szBuffer);
						piD3DRMTexture->SetName(szBuffer);
						hr = piD3DRMFace->SetTexture( piD3DRMTexture );
						if (FAILED(hr))
						{
							CONVERSION3DS_ERROR;
							hr = VWCONVGEOM_E_BADREAD;
							goto EXIT_FAIL;
						}
						SAFERELEASE(piD3DRMTexture);
					}
*/
