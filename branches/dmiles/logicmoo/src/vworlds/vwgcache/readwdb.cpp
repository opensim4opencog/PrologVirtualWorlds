// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// readwdb.cpp : Read 3D studio .3ds files into scene
//
#include "stdafx.h"
#include <string.h>
#include <math.h>
#include <d2d.h>
#include <VWGCache.h>
#include "vwgcacho.h"
#include "readwdb.h"

//---------------------------------------------------------------
// Parameters
//---------------------------------------------------------------

#define cchNameMax			255

#define dwArchiveStamp		('W' + ('D'<<8) + ('B'<<16) + ('V'<<24) )
#define dwArchiveVersion	0x00000004

//---------------------------------------------------------------
// Types
//---------------------------------------------------------------

enum
{
	ePropWrapU=1,
	ePropWrapV=2
};

struct FaceInfo
{
	BYTE		m_iMaterial;
	BYTE		m_chProperties;
};

struct MatEntry
{
	D3DCOLOR 			m_rlc;
	IDirect3DRMTexture	*m_prlt;
	IDirect3DRMMaterial	*m_prlm;
};

class CMatList
{
protected:
	CPtrList	m_listMat;
public:
	CMatList();
	~CMatList();
	BOOL GetMaterial( int iMaterial, MatEntry *pmat );
	BOOL Read(CArchive& ar,
			  int cFrames,
			  int nPeriod,
			  IInternetFileManager *pInternetFileManager,
			  LPDIRECT2DRM pD2DRM);
};

struct LightInfo
{
	WORD	wLightType;
	DWORD	dwColour;
	float	fConstantAttenuation;
	float	fLinearAttenuation;
	float	fQuadraticAttenuation;
	float	fPenumbra;
	float	fUmbra;
}; 

//---------------------------------------------------------------
// Static data.
//---------------------------------------------------------------

IDirect3DRM		*pd3drm = NULL;
IInternetFileManager	*pInternetFileManager;
LPDIRECT2DRM	pD2DRM;
CVWGeometryCacheObject *pGeometryCacheObject;

/////////////////////////////////////////////////////////////////////////////
// CMatList support class

CMatList::CMatList()
{
}

CMatList::~CMatList()
{
	POSITION pos;

	// iterate through material list, freeing up contents
	for( pos=m_listMat.GetHeadPosition(); pos!=0; )
	{
		MatEntry *pmat;
		int iRet;
		
		pmat = (MatEntry *) m_listMat.GetNext(pos);
		ASSERT( pmat != NULL );

		// Release interfaces.
		if (pmat->m_prlt != NULL)
		{
			iRet = pmat->m_prlt->Release();
		}
		if (pmat->m_prlm != NULL)
		{
			iRet = pmat->m_prlm->Release();
		}

		delete pmat;
	}
}

BOOL CMatList::GetMaterial( int iMaterial, MatEntry *pmat )
{
	POSITION pos;

	pos = m_listMat.FindIndex( iMaterial );
	if( pos == NULL )
		return FALSE;

	// copy
	*pmat = *((MatEntry *) m_listMat.GetAt( pos ) );

	return TRUE;
}

BOOL CMatList::Read(CArchive& ar,
					int cFrames,
					int nPeriod,
					IInternetFileManager* pInternetFileManager,
					LPDIRECT2DRM pD2DRM)
{
	HRESULT hr;
	BYTE cMat;
	int iMat;

	// get count
	ar >> (BYTE) cMat;

	for( iMat=0; iMat < cMat; iMat++ )
	{
		D3DCOLOR	rlc;
		IDirect3DRMTexture	*prlt;
		IDirect3DRMMaterial *prlm;
		float		fKs, fPower;
		BYTE		cch;
		MatEntry	*pmat;
		BYTE		bMatFlag;

		// read color
		ar >> (DWORD) rlc;

		// read texture filename size
		ar >> (BYTE) cch;
		prlt = NULL;

		// if have filename, read it and reconstruct it
		if( cch > 0 )
		{
			char sz[256];
			ar.Read( sz, cch );
			sz[cch] = '\0';

//			TRACE("Reading Texture: %s\n", sz);

			if (FAILED(hr = LoadTextureCB(sz, (void *)pGeometryCacheObject, &prlt))) {
				// REVIEW: (mdm) we need to do some clean up and sensible error recovery...
				ASSERT(0);
				return FALSE;
			}
		}


		// read material flag
		ar >> (BYTE) bMatFlag;

		if( bMatFlag )
		{
			// read material info
			ar >> (float) fKs;
			ar >> (float) fPower;

			// create material
			pd3drm->CreateMaterial( fPower, &prlm);
			prlm->SetEmissive(fKs, fKs, fKs);
		}
		else
		{
			prlm = NULL;
		}

		// add material to list
		pmat = new MatEntry;
		if( pmat==NULL )
			return FALSE;

		pmat->m_rlc = rlc;
		pmat->m_prlt = prlt;
		pmat->m_prlm = prlm;

		m_listMat.AddTail( pmat );
	}

 	return TRUE;
}
 
/////////////////////////////////////////////////////////////////
// CWDBReader

BOOL CWDBReader::ReadWDB(const char* psz, 
						 const char* szPath,
						 IDirect3DRMFrame* prlfGeometry,
						 CVWGeometryCacheObject *pGeometryCacheObject)
{
	CString 	strFile, strPath;
	CFile		fileIn;
	DWORD		dw;

	// Store the locally required stuff in static data.
	::pd3drm = pGeometryCacheObject->m_pD3DRM;
	::pInternetFileManager = pGeometryCacheObject->m_pInternetFileManager;
	::pD2DRM = pGeometryCacheObject->m_pD2DRM;
	::pGeometryCacheObject = pGeometryCacheObject;

	strPath = CString(szPath);
	if (strPath.GetLength() > 0)
	{
		// make the full path filename from the fullname and path.
		strFile = (CString)szPath + "\\" + (CString)psz;
	}
	else
	{
		strFile = (CString)psz;
	}

	// open it
	if( !fileIn.Open( strFile, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary ) )
	{
		return FALSE;
	}

	// create an archive with the file
	CArchive ar( &fileIn, CArchive::load, 16*1024 );

	// get and check the stamp
	ar >> (DWORD) dw;
	if (dw != dwArchiveStamp)
	{
		return FALSE;
	}

	// read version # and check
	ar >> (DWORD) dw;
	if (dw != dwArchiveVersion )
	{
		return FALSE;
	}

	// read background color
	D3DCOLOR rlc;
	ar >> (DWORD) rlc;
	// REVIEW: Don't use scene background color.

	// REVIEW: read past collision detection stuff
	WORD w;
	int iccd, ccd;
	double d;
	ar >> (WORD) w;
	ccd = w;
	for( iccd=0; iccd<ccd; iccd++ )
	{
		double x1, y1, x2, y2;

		ar >> (double) x1;
		ar >> (double) y1;
		ar >> (double) x2;
		ar >> (double) y2;
	}
	ar >> (double) d;
	float fYMax = (float) d;
	ar >> (double) d;
	float fYMin = (float) d;

	// read in user-added stuff
	ReadFrame( ar, prlfGeometry );

	return TRUE;
}

void CWDBReader::ReadFrame( CArchive& ar, IDirect3DRMFrame* prlFrame )
{
	WORD		c;
	static int	iDepth=0;
	IDirect3DRMFrame	*prlfParent;
	BOOL		bAnimated;
	int			cFrames = 1;
	int			nPeriod = 0;

	// initialize
	bAnimated = FALSE;

	// bump depth
	iDepth++;
	// TRACE("\nReadFrame: iDepth=%d\n", iDepth );

	// get this frame's parent for a reference
	prlFrame->GetParent(&prlfParent);

	// read object name length
	BYTE cch;
	char szName[cchNameMax+1];

	ar >> (BYTE) cch;
	if( cch > 0 )
	{
		ar.Read(szName, cch);
		szName[cch] = '\0';
		prlFrame->SetName( szName );

		// see if we have an animated object
		if( _strnicmp( szName, "anim", 4 ) == 0 )
		{
			// parse out number of images and animation speed
			CString str(szName);
			cFrames = atoi(str.Mid(4, 2));
			nPeriod = atoi(str.Mid(6, 2));
			ASSERT( cFrames > 0 );
			ASSERT( nPeriod > 0 );
			ASSERT( str.GetLength() > 7 );
		}
	}

	// read the transform
	D3DRMMATRIX4D rlm;
	ar.Read( rlm, sizeof(D3DRMMATRIX4D) );
	prlFrame->AddTransform( D3DRMCOMBINE_REPLACE, rlm );

	// read the velocity and rotation
	D3DVECTOR rlvVelocity;
	D3DVECTOR rlvAxis;
	D3DVALUE	fTheta;
	ar.Read( &rlvVelocity, sizeof(D3DVECTOR) ) ;
	ar.Read( &rlvAxis, sizeof(D3DVECTOR) );
	ar >> (float) fTheta;
	prlFrame->SetVelocity( prlfParent, rlvVelocity.x, rlvVelocity.y, rlvVelocity.z, FALSE ); 
	prlFrame->SetRotation( prlfParent, rlvAxis.x, rlvAxis.y, rlvAxis.z, fTheta );

	// read out the visuals
	int iVisual;
	int cVisual;
	ar >> (WORD) c;
	cVisual = c;
	// TRACE("ReadFrame: cVisual=%d\n", cVisual );
	for( iVisual=0; iVisual<cVisual; iVisual++ )
	{
		ReadMesh( ar, prlFrame, cFrames, nPeriod );
	}


	// collect light information
	int iLight;
	int cLight=0;
	LightInfo *prgLI;
	ar >> (WORD) c;
	// TRACE("ReadFrame: cLight=%d\n", c );
	if( c != 0 )
	{
		cLight = c;
		prgLI = (LightInfo*)malloc( sizeof(LightInfo) * cLight );
		ar.Read( prgLI, sizeof(LightInfo) * cLight );
		for( iLight=0; iLight<cLight; iLight++ )
		{
			IDirect3DRMLight *prll;

			pd3drm->CreateLight((D3DRMLIGHTTYPE) prgLI[iLight].wLightType, 
							  (D3DCOLOR) prgLI[iLight].dwColour,
							  &prll);

			prll->SetConstantAttenuation( prgLI[iLight].fConstantAttenuation );
			prll->SetLinearAttenuation( prgLI[iLight].fLinearAttenuation );
			prll->SetQuadraticAttenuation( prgLI[iLight].fQuadraticAttenuation );
			prll->SetPenumbra( prgLI[iLight].fPenumbra );
			prll->SetUmbra( prgLI[iLight].fUmbra );

			// add to scene
			prlFrame->AddLight( prll );

			// Release light interface.
			VERIFY(prll->Release() == 1);
		}
		free( prgLI );
	}

	// read the children
	int iChild;
	int cChildren;
	ar >> (WORD) c;
	cChildren = c;
	// TRACE("ReadFrame: cChildren=%d\n", cChildren );
	for( iChild=0; iChild<cChildren; iChild++ )
	{
		IDirect3DRMFrame *prlfChild;

		// Create the frame for the child.
		pd3drm->CreateFrame(prlFrame, &prlfChild);
		VERIFY(prlfChild->Release() == 1);
		ReadFrame( ar, prlfChild );
	}

	// Release parent interface.
	if (prlfParent != NULL)
	{
//		VERIFY(prlfParent->Release() == 1); 
		prlfParent->Release();
	}

	// unbump
	iDepth--;
}

void CWDBReader::ReadMesh( CArchive& ar, IDirect3DRMFrame* prlfParent, int cFrames, int nPeriod )
{
	int				iVertex, cVertex;
	D3DVECTOR		*prgVertex;
	int				cNormal;
	D3DVECTOR		*prgNormal=NULL;
	int				iFace, cFace;
	WORD			c;
	BOOL			bHaveTexture;
	IDirect3DRMMeshBuilder	*prlMeshBuilder;

	// Create mesh builder object
	pd3drm->CreateMeshBuilder( &prlMeshBuilder );

	// read vertex count and array
	ar >> (WORD) c;
	// TRACE("ReadMesh: cVertex=%d\n", c );
	if( c != 0 )
	{
		cVertex = c;
		prgVertex = (D3DVECTOR*)malloc( sizeof(D3DVECTOR)*cVertex );
		ar.Read( prgVertex, sizeof(D3DVECTOR)*cVertex );
	}

	// read normals
	ar >> (WORD) c;
	// TRACE("ReadMesh: cNormal=%d\n", c );
	if( c != 0 )
	{
		cNormal = c;
		prgNormal = (D3DVECTOR*)malloc( sizeof(D3DVECTOR)*cNormal );
		ar.Read( prgNormal, sizeof(D3DVECTOR)*cNormal );
	}

	// read face count
	ar >> (WORD) c;
	// TRACE("ReadMesh: cFace=%d\n", c );
	cFace = c;

	// read face data array size
	WORD *pwFaceData;
	ar >> (WORD) c;

	// allocate array
	pwFaceData = (WORD*)malloc( c );

	// read array
	ar.Read( pwFaceData, c );

	// expand
	WORD	*pw;
	ULONG	*pnFaceData;
	ULONG	*pn;

	pnFaceData = (ULONG*)malloc( c * 2);
	pw = pwFaceData;
	pn = pnFaceData;
	for(;;)
	{
		WORD cFaceVertex;

		// get count of vertices in face
		cFaceVertex = *pw++;

		// copy
		*pn++ = cFaceVertex;

		// if zero, done
		if( cFaceVertex == 0 )
			break;

		// copy data, expanding
		for( iVertex=0; iVertex<cFaceVertex; iVertex++ )
		{
			// copy vertex index
			*pn++ = *pw++;

			// if normals present, copy that too
			if( cNormal > 0 )
				*pn++ = *pw++;
		}
	}

	// create faces
	IDirect3DRMFaceArray *prlFaceArray;
	prlMeshBuilder->AddFaces(cVertex,
							  prgVertex,
							  cNormal,
							  prgNormal,
							  pnFaceData,
							  &prlFaceArray);

	// read color source flag
	BYTE ch;
	D3DRMCOLORSOURCE rlcs;

	ar >> (BYTE) ch;
	if( ch==1 )
		rlcs = D3DRMCOLOR_FROMVERTEX;
	else
		rlcs = D3DRMCOLOR_FROMFACE;
	prlMeshBuilder->SetColorSource( rlcs );

		// read render quality flag
	D3DRMRENDERQUALITY rq;
				
	ar >> (BYTE) ch;
	switch (ch) {
		case 0:
			rq = D3DRMRENDER_WIREFRAME;
			break;
		case 1:
			rq = D3DRMRENDER_UNLITFLAT;
			break;
		case 2:
			rq = D3DRMRENDER_FLAT;
			break;
		case 3:
			rq = D3DRMRENDER_GOURAUD;
			break;
		case 4:
			rq = D3DRMRENDER_PHONG;
			break;
	}
	prlMeshBuilder->SetQuality(rq);

 	// allocate UV and colour arrays
	D3DVALUE *prgUV;
	D3DCOLOR *prgColour;
	prgUV = (D3DVALUE*)malloc( sizeof(D3DVALUE) * cVertex * 2 );
	prgColour = (D3DCOLOR*)malloc( sizeof(D3DCOLOR) * cVertex );

	// read
	ar.Read( prgUV, sizeof(D3DVALUE) * cVertex * 2 );
	ar.Read( prgColour, sizeof(D3DCOLOR) * cVertex );

	// set vertex texel coords and colors
	D3DVALUE *pUV;
	D3DCOLOR *pColour;
	pUV = prgUV;
	pColour = prgColour;
	for( iVertex=0; iVertex<cVertex; iVertex++ )
	{
		D3DVALUE u, v;

		u = *pUV++;
		v = *pUV++;
		// NOTE: (mdm) we must mirror (negate) the v texture coordinates because
		//  the wdb files write out texture coordinates bottom up, and
		//  d3d wants them top down.
		prlMeshBuilder->SetTextureCoordinates( iVertex, u, -v );
		prlMeshBuilder->SetVertexColor( iVertex, *pColour++ );
	}

	// read the material list
	CMatList listMat;
	if( !listMat.Read(ar, cFrames, nPeriod, pInternetFileManager, pD2DRM) )
	{
		ASSERT(FALSE);
		return;
	}

	// alloc face info
	FaceInfo *prgFaceInfo;
	prgFaceInfo = (FaceInfo*)malloc( sizeof(FaceInfo) * cFace );

	// read
	ar.Read( prgFaceInfo, sizeof(FaceInfo) * cFace );

	// go through faces assigning colors, materials, topology and texture
	bHaveTexture = FALSE;
	for( iFace=0; iFace<cFace; iFace++ )
	{
		MatEntry mat;

		if( !listMat.GetMaterial( prgFaceInfo[iFace].m_iMaterial, &mat ) )
		{
			ASSERT(FALSE);
			return;
		}

		IDirect3DRMFace *prlFace;
		prlFaceArray->GetElement(iFace, &prlFace);
		if( mat.m_prlt != NULL )
		{
			prlFace->SetTexture( mat.m_prlt );
			bHaveTexture = TRUE;
		}
		prlFace->SetColor( mat.m_rlc );

		if( mat.m_prlm != NULL )
			prlFace->SetMaterial( mat.m_prlm );

		prlFace->SetTextureTopology(!!(prgFaceInfo[iFace].m_chProperties & ePropWrapU),
									!!(prgFaceInfo[iFace].m_chProperties & ePropWrapV));
		VERIFY(prlFace->Release() == 1);
	}

	// if any texture, then turn on perspective correct
	if( bHaveTexture )
	{
		prlMeshBuilder->SetPerspective( TRUE );
	}

	// add the visual
	prlfParent->AddVisual( prlMeshBuilder );

	// Release face array interface and all it's faces.
	VERIFY(prlFaceArray->Release() == 0);
	// Release mesh builder interface.
	VERIFY(prlMeshBuilder->Release() == 1);

	// free memory
	free( prgVertex );
	free( prgNormal );
	free( pwFaceData );
	free( pnFaceData );
	free( prgUV );
	free( prgColour );
	free( prgFaceInfo );
}
