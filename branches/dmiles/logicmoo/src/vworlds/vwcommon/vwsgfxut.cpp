// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// File:	VWSGfxUt.cpp
// Author:	D.S.
// Updated:	M.M.
//
// Description: 
//   Math utility functions for Geometry and boundary editors.
//

#include <stdafx.h>
#include <d3drm.h>
#include <stdtools.h>
#include <propbase.h>
#include <vwsgfxut.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////////
static CComBSTR bstrPosition = "Position";
static CComBSTR bstrDirection = "Direction";
static CComBSTR bstrUp = "Up";
static CComBSTR bstrScale = "Scale";

#define PI 3.14159265359f
//////////////////////////////////////////////////////////////////////////


void IdentityMatrix(D3DRMMATRIX4D* mat)
{
	(*mat)[0][0] = 1; (*mat)[0][1] = 0; (*mat)[0][2] = 0; (*mat)[0][3] = 0;
	(*mat)[1][0] = 0; (*mat)[1][1] = 1; (*mat)[1][2] = 0; (*mat)[1][3] = 0;
	(*mat)[2][0] = 0; (*mat)[2][1] = 0; (*mat)[2][2] = 1; (*mat)[2][3] = 0;
	(*mat)[3][0] = 0; (*mat)[3][1] = 0; (*mat)[3][2] = 0; (*mat)[3][3] = 1;
}

void PostMultiplyMatrix(D3DRMMATRIX4D* mat, D3DRMMATRIX4D* postmat)
{
	D3DRMMATRIX4D srcmat;

	CopyMemory((LPVOID) &srcmat, (LPVOID) mat, sizeof D3DRMMATRIX4D);

	int i, j, k;

	for ( i = 0; i < 4; i++) 
	{
		for (j = 0; j < 4; j++) 
		{
			(*mat)[i][j] = 0;
			for ( k = 0; k < 4; k++)
				(*mat)[i][j] += (srcmat[i][k] * (*postmat)[k][j]);
		}
	}
}

void ScaleMatrix(D3DRMMATRIX4D* mat, float fX, float fY, float fZ)
{
	D3DRMMATRIX4D scalemat;

	IdentityMatrix(&scalemat);

	scalemat[0][0] = fX; 
	scalemat[1][1] = fY; 
	scalemat[2][2] = fZ;

	PostMultiplyMatrix(mat, &scalemat);

}

//The the rotation elements in an identity matrix
void MakeYRotatationMatrix(D3DRMMATRIX4D* mat, float angle)
{
	float c, s;

	c = (float) cos(angle);	
	s = (float) sin(angle);

	IdentityMatrix(mat);

	(*mat)[0][0] = c; (*mat)[0][2] = -s;
	(*mat)[2][0] = s; (*mat)[2][2] = c;
}

//Given a direction vector, get the rotation around the y axis
float GetYRotation(float fX, float fY, float fZ)
{
	D3DVECTOR rlvPreDef, rlvDir;
	D3DVALUE  valCosTheta, fTheta;

	// The pre-defined angle.
	rlvPreDef.x = 0.0F;
	rlvPreDef.y	= 0.0F;
	rlvPreDef.z	= 1.0F;

	// Make a D3DVECTOR from this vector.
	rlvDir.x = fX;
	rlvDir.y = fY;
	rlvDir.z = fZ;

	// First need angle between the pre-defined angle and rlvDir.
	valCosTheta = D3DRMVectorDotProduct(&rlvPreDef, &rlvDir);
	fTheta = (D3DVALUE)acos(valCosTheta);

	// This will always be the acute angle. Since rotation will always be in positive direction we must
	// give correct angle (possibly obtuse) for that direction.
	if (rlvDir.x < 0.0F)
	{
		// Acute angle will not work, need obtuse angle.
		fTheta = (2*PI) - fTheta;
	}
	return fTheta;
}

void TransposeMatrix(D3DRMMATRIX4D* mat)
{
	D3DRMMATRIX4D srcmat;

	CopyMemory((LPVOID) &srcmat, (LPVOID) mat, sizeof D3DRMMATRIX4D);

	int i, j;

	for ( i = 0; i < 4; i++) 
	{
		for (j=0; j<4; j++) 
		{
			(*mat)[i][j] = srcmat[j][i];
		}
	}
}

void RotateMatrix(D3DRMMATRIX4D* mat, float fX, float fY, float fZ)
{
	float frot;
	D3DRMMATRIX4D rotmat;

	frot = GetYRotation(fX, fY, fZ);

	MakeYRotatationMatrix(&rotmat, frot);

	PostMultiplyMatrix(mat, &rotmat);
}

void InverseRotateMatrix(D3DRMMATRIX4D* mat, float fX, float fY, float fZ)
{
	float frot;
	D3DRMMATRIX4D rotmat;

	frot = GetYRotation(fX, fY, fZ);

	MakeYRotatationMatrix(&rotmat, 2*PI - frot);

	PostMultiplyMatrix(mat, &rotmat);
}


void TranslateMatrix(D3DRMMATRIX4D* mat, float fX, float fY, float fZ)
{
	D3DRMMATRIX4D transmat;

	IdentityMatrix(&transmat);
	transmat[3][0] = fX; transmat[3][1] = fY; transmat[3][2] = fZ;

	PostMultiplyMatrix(mat, &transmat);
}

C2DThingCoordTransformer::C2DThingCoordTransformer()
{

	m_pThing = NULL;
	m_bDoTransform = FALSE;
}

C2DThingCoordTransformer::~C2DThingCoordTransformer()
{
	SAFERELEASE(m_pThing);
}

//The function recursively builds up the transformation of the pThing 
//until it's in room-space coordinates
HRESULT C2DThingCoordTransformer::BuildTransformation(IThing * pThing, IThing * pParentThing)
{
	IVector * pVector = NULL;
	HRESULT hr = S_OK;
	IThing* pTmpParentThing = NULL;
	D3DRMMATRIX4D tmpMatrix;
	float flScaleX, flScaleY, flScaleZ;
	float flThingX, flThingY, flThingZ, // Position of current Thing having cells edited
	      flThingDirX, flThingDirY, flThingDirZ; // Orientation of current Thing having cells edited

	if (!pThing || !pParentThing)
		goto EXIT_FAIL;

	// Store the position Vector of the Thing
	hr = pThing->get_ObjectProperty(bstrPosition, (IObjectProperty **) &pVector);
	if( FAILED(hr) || !pVector) goto EXIT_FAIL;
	hr = pVector->get(&flThingX, &flThingY, &flThingZ);
	if( FAILED(hr) ) goto EXIT_FAIL;
	SAFERELEASE(pVector);

	// Get the Scale Vector of the Thing
	hr = pThing->get_ObjectProperty(bstrScale, (IObjectProperty **) &pVector);
	if( FAILED(hr) || !pVector) goto EXIT_FAIL;
	hr = pVector->get(&flScaleX, &flScaleY, &flScaleZ);
	if( FAILED(hr) ) goto EXIT_FAIL;
	SAFERELEASE(pVector);

	// Store the Direction Vector of the Thing
	hr = pThing->get_ObjectProperty(bstrDirection, (IObjectProperty **) &pVector);
	if( FAILED(hr) ) goto EXIT_FAIL;
	hr = pVector->get(&flThingDirX, &flThingDirY, &flThingDirZ);
	if( FAILED(hr) ) goto EXIT_FAIL;
	SAFERELEASE(pVector);

	IdentityMatrix(&tmpMatrix);
	ScaleMatrix(&tmpMatrix, flScaleX, flScaleY, flScaleZ);
	RotateMatrix(&tmpMatrix, flThingDirX, flThingDirY, flThingDirZ);
	TranslateMatrix(&tmpMatrix, flThingX, flThingY, flThingZ);
	PostMultiplyMatrix(&m_d3dMatrix, &tmpMatrix);

	IdentityMatrix(&tmpMatrix);
	TranslateMatrix(&tmpMatrix, -flThingX, -flThingY, -flThingZ);
	InverseRotateMatrix(&tmpMatrix, flThingDirX, flThingDirY, flThingDirZ);
	ScaleMatrix(&tmpMatrix, 1.0f / flScaleX, 1.0f / flScaleY, 1.0f / flScaleZ);
	PostMultiplyMatrix(&m_d3dInverseMatrix, &tmpMatrix);

	hr = pParentThing->get_Container(&pTmpParentThing);
	if ( SUCCEEDED(hr) && pTmpParentThing)
	{
		BuildTransformation(pParentThing, pTmpParentThing);
	}


EXIT_FAIL:
	SAFERELEASE(pTmpParentThing);
	SAFERELEASE(pVector);
	return hr;
}

HRESULT C2DThingCoordTransformer::StoreThingCoords(IThing * pThing)
{
	HRESULT hr = S_OK;
	IThing* pTmpParentThing = NULL;

	m_bDoTransform = TRUE;

	if(NULL == pThing)
	{
		m_bDoTransform = FALSE;
		hr = E_INVALIDARG;
		goto EXIT_FAIL;
	}

	SAFERELEASE(m_pThing);
	m_pThing = pThing;
	SAFEADDREF(m_pThing);

	IdentityMatrix(&m_d3dMatrix);
	IdentityMatrix(&m_d3dInverseMatrix);

	hr = m_pThing->get_Container(&pTmpParentThing);
	if ( SUCCEEDED(hr) && pTmpParentThing)
	{
		BuildTransformation(pThing, pTmpParentThing);
	}

	goto EXIT_SUCCESS2;

EXIT_FAIL:
	ASSERT(0); 
EXIT_SUCCESS2:
	SAFERELEASE(pTmpParentThing);
	return hr;
}


HRESULT C2DThingCoordTransformer::TransformFromThingCoords(float * pflX, float * pflY, float * pflZ)
{
	HRESULT hr = S_OK;
	D3DVECTOR d3dVDst, d3dVSrc;

	if(NULL == pflX || NULL == pflY	|| NULL == pflZ)
	{
		hr = E_POINTER;
		goto EXIT_FAIL;
	}

	if (m_bDoTransform)
	{
		d3dVSrc.x = *pflX;
		d3dVSrc.y = *pflY;
		d3dVSrc.z = *pflZ;

		Transform(&d3dVDst, &d3dVSrc);

		*pflX = d3dVDst.x;
		*pflY = d3dVDst.y;
		*pflZ = d3dVDst.z;
	}

EXIT_FAIL:

	return hr;
}

//Transform a point from world to thing coords
HRESULT C2DThingCoordTransformer::TransformToThingCoords(float * pflX, float * pflY, float * pflZ)
{

	HRESULT hr = S_OK;
	D3DVECTOR d3dVDst, d3dVSrc;

	if(NULL == pflX || NULL == pflY	|| NULL == pflZ)
	{
		hr = E_POINTER;
		goto EXIT_FAIL;
	}

	if (m_bDoTransform)
	{
		d3dVSrc.x = *pflX;
		d3dVSrc.y = *pflY;
		d3dVSrc.z = *pflZ;

		InverseTransform(&d3dVDst, &d3dVSrc);

		*pflX = d3dVDst.x;
		*pflY = d3dVDst.y;
		*pflZ = d3dVDst.z;
	}

EXIT_FAIL:

	return hr;
}

//Transform a vector from world to thing coords
HRESULT C2DThingCoordTransformer::TransformToThingCoordsNoTranslation(float * pflX, float * pflY, float * pflZ)
{
	HRESULT hr = S_OK;
	D3DVECTOR d3dVDst, d3dVSrc;
	IVector * pVector = NULL;


	if(NULL == pflX || NULL == pflY	|| NULL == pflZ)
	{
		hr = E_POINTER;
		goto EXIT_FAIL;
	}

	if (m_bDoTransform)
	{
		hr = m_pThing->get_ObjectProperty(bstrPosition, (IObjectProperty **) &pVector);
		if( FAILED(hr) ) goto EXIT_FAIL;

		hr = pVector->get(&d3dVSrc.x, &d3dVSrc.y, &d3dVSrc.z);
		if( FAILED(hr) ) goto EXIT_FAIL;

		d3dVSrc.x = *pflX + d3dVSrc.x;
		d3dVSrc.y = *pflY + d3dVSrc.y;
		d3dVSrc.z = *pflZ + d3dVSrc.z;

		InverseTransform(&d3dVDst, &d3dVSrc);

		*pflX = d3dVDst.x;
		*pflY = d3dVDst.y;
		*pflZ = d3dVDst.z;
	}

EXIT_FAIL:
	SAFERELEASE(pVector);

	return hr;
}

void C2DThingCoordTransformer::Transform(D3DVECTOR* pd3dVDst, D3DVECTOR* pd3dVSrc)
{
	pd3dVDst->x = m_d3dMatrix[0][0] * pd3dVSrc->x + m_d3dMatrix[1][0] * pd3dVSrc->y  + m_d3dMatrix[2][0] * pd3dVSrc->z + m_d3dMatrix[3][0];
	pd3dVDst->y = m_d3dMatrix[0][1] * pd3dVSrc->x + m_d3dMatrix[1][1] * pd3dVSrc->y  + m_d3dMatrix[2][1] * pd3dVSrc->z + m_d3dMatrix[3][1];
	pd3dVDst->z = m_d3dMatrix[0][2] * pd3dVSrc->x + m_d3dMatrix[1][2] * pd3dVSrc->y  + m_d3dMatrix[2][2] * pd3dVSrc->z + m_d3dMatrix[3][2];

	return;
}

void C2DThingCoordTransformer::InverseTransform(D3DVECTOR* pd3dVDst, D3DVECTOR* pd3dVSrc)
{
	pd3dVDst->x = m_d3dInverseMatrix[0][0] * pd3dVSrc->x + m_d3dInverseMatrix[1][0] * pd3dVSrc->y  + m_d3dInverseMatrix[2][0] * pd3dVSrc->z + m_d3dInverseMatrix[3][0];
	pd3dVDst->y = m_d3dInverseMatrix[0][1] * pd3dVSrc->x + m_d3dInverseMatrix[1][1] * pd3dVSrc->y  + m_d3dInverseMatrix[2][1] * pd3dVSrc->z + m_d3dInverseMatrix[3][1];
	pd3dVDst->z = m_d3dInverseMatrix[0][2] * pd3dVSrc->x + m_d3dInverseMatrix[1][2] * pd3dVSrc->y  + m_d3dInverseMatrix[2][2] * pd3dVSrc->z + m_d3dInverseMatrix[3][2];

	return;
}


