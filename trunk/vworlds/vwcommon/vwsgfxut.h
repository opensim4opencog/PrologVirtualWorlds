// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// File:	VWSGfxUt.h
// Author:	D.S.
// Updated:	M.M.
//
// Description: 
//   Math utility functions for Geometry and boundary editors.
//

#ifndef __vwsgraphicsutils_h__
#define __vwsgraphicsutils_h__


//#include <d3drmwin.h>
//#include <vwgeom.h>

typedef struct _fPoint2D
{
	float x;
	float y;
} fPoint2D;

typedef D3DVALUE D3DRMMATRIX4D[4][4];

//Helpful C declarated functions
void IdentityMatrix(D3DRMMATRIX4D* mat);
void PostMultiplyMatrix(D3DRMMATRIX4D* mat, D3DRMMATRIX4D* postmat);
void ScaleMatrix(D3DRMMATRIX4D* mat, float fX, float fY, float fZ);
void MakeYRotatationMatrix(D3DRMMATRIX4D* mat, float angle);
float GetYRotation(float fX, float fY, float fZ);
void TransposeMatrix(D3DRMMATRIX4D* mat);
void RotateMatrix(D3DRMMATRIX4D* mat, float fX, float fY, float fZ);
void InverseRotateMatrix(D3DRMMATRIX4D* mat, float fX, float fY, float fZ);
void TranslateMatrix(D3DRMMATRIX4D* mat, float fX, float fY, float fZ);



// The C2DThingCoordTransformer class helps you move points in and out of 
// a thing's local coordinate system into world coords.
class C2DThingCoordTransformer
{
public:
	C2DThingCoordTransformer();
	virtual ~C2DThingCoordTransformer();

	HRESULT StoreThingCoords(IThing * pThing);
	HRESULT TransformFromThingCoords(float * pflX, float * pflY, float * pflZ);
	HRESULT TransformToThingCoords(float * pflX, float * pflY, float * pflZ);
	HRESULT TransformToThingCoordsNoTranslation(float * pflX, float * pflY, float * pflZ);

protected:
	HRESULT BuildTransformation(IThing * pThing, IThing * pParentThing);
	void Transform(D3DVECTOR* pd3dVDst, D3DVECTOR* pd3dVSrc);
	void InverseTransform(D3DVECTOR* pd3dVDst, D3DVECTOR* pd3dVSrc);

	IThing * m_pThing;
	BOOL m_bDoTransform;
	D3DRMMATRIX4D m_d3dMatrix;
	D3DRMMATRIX4D m_d3dInverseMatrix;

};



#endif