// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include <d3drmwin.h>
#include "rlutil.h"
#include <math.h>

#include <propbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


////////////////////////////////////////////////////////////////////////


static unsigned long rod_faces[] =
{   8, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0, 0, /* end 1 */
    4, 0, 0, 1, 1, 9, 1, 8, 0,  /* side 0 */
    4, 1, 1, 2, 2, 10, 2, 9, 1, /* side 1 */
    4, 2, 2, 3, 3, 11, 3, 10, 2, /* side 2 */
    4, 3, 3, 4, 4, 12, 4, 11, 3, /* side 3 */
    4, 4, 4, 5, 5, 13, 5, 12, 4, /* side 4 */
    4, 5, 5, 6, 6, 14, 6, 13, 5, /* side 5 */
    4, 6, 6, 7, 7, 15, 7, 14, 6, /* side 6 */
    4, 7, 7, 0, 0, 8, 0, 15, 7,         /* side 7 */
    8, 8, 0, 9, 1, 10, 2, 11, 3, 12, 4, 13, 5, 14, 6, 15, 7, /* end 2 */
    0,
};
static unsigned long cone_faces[] =
{   8, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0, 0, /* end 1 */
    3, 0, 0, 1, 1, 8, 1,        /* side 0 */
    3, 1, 1, 2, 2, 8, 1,        /* side 1 */
    3, 2, 2, 3, 3, 8, 1, /* side 2 */
    3, 3, 3, 4, 4, 8, 1, /* side 3 */
    3, 4, 4, 5, 5, 8, 1, /* side 4 */
    3, 5, 5, 6, 6, 8, 1, /* side 5 */
    3, 6, 6, 7, 7, 8, 1, /* side 6 */
    3, 7, 7, 0, 0, 8, 1,                /* side 7 */
    0,
};

////////////////////////////////////////////////////////////////////////

D3DRMBOX GetSpriteBoundingBox(IDirect3DRMFrame *pFrame)
{
//	BoundingBox bbox = xGetBoundingBox(pFrame, TRUE);
//	return bbox.GetBox();

	D3DRMBOX bbox;

	bbox.min.x = -1.0f;
	bbox.min.y = -1.0f;
	bbox.min.z = -1.0f;
	bbox.max.x = 1.0f;
	bbox.max.y = 1.0f;
	bbox.max.z = 1.0f;

	return bbox;
}


D3DRMBOX GetBoundingBox(IDirect3DRMFrame *pFrame)
{
	BoundingBox bbox = xGetBoundingBox(pFrame, FALSE);

	return bbox.GetBox();
}

BoundingBox xGetBoundingBox(IDirect3DRMFrame *pFrame, BOOL bSprite)
{
	BoundingBox result;

	unsigned i;

	// Delete the children
	LPDIRECT3DRMFRAMEARRAY lpChildren = NULL;
	if (SUCCEEDED(pFrame->GetChildren(&lpChildren))) 
	{
		for (i=0; i < lpChildren->GetSize(); i++) 
		{
			IDirect3DRMFrame *pChild = NULL;

			if (SUCCEEDED(lpChildren->GetElement(i, &pChild))) 
			{
				BoundingBox childBox = xGetBoundingBox(pChild, bSprite);
				childBox = childBox.Transform(pChild);
				result = result.Combine(childBox);
				SAFERELEASE(pChild);
			}
		}
		SAFERELEASE(lpChildren);
	}

	LPDIRECT3DRMVISUALARRAY	lpVisuals = NULL;

	if (SUCCEEDED(pFrame->GetVisuals(&lpVisuals))) 
	{
		for ( i = 0; i < lpVisuals->GetSize(); i++) 
		{
			LPDIRECT3DRMVISUAL lpD3DRMVisual;

			if (SUCCEEDED(lpVisuals->GetElement(i, &lpD3DRMVisual))) 
			{
				IDirect3DRMMeshBuilder *prlmbVis = NULL;
				IDirect3DRMFrame *prlfVis = NULL;

				if (SUCCEEDED(lpD3DRMVisual->QueryInterface(IID_IDirect3DRMMeshBuilder, (void **) &prlmbVis)))
				{
					D3DRMBOX meshBox;
					
					if (bSprite)
					{
						meshBox.min.x = -1.0f;
						meshBox.min.y = -1.0f;
						meshBox.min.z = -1.0f;
						meshBox.max.x = 1.0f;
						meshBox.max.y = 1.0f;
						meshBox.max.z = 1.0f;
					}
					else
						prlmbVis->GetBox(&meshBox);

					result = result.Combine(BoundingBox(meshBox));
					SAFERELEASE(prlmbVis);
				}
				else if (SUCCEEDED(lpD3DRMVisual->QueryInterface(IID_IDirect3DRMFrame, (void **) &prlfVis)))
				{
					unsigned long len = 100;
					char name[100];

					prlfVis->GetName(&len, name);
					TRACE("calculating bbox for %s\n", name);

					BoundingBox bbox = xGetBoundingBox(prlfVis, bSprite);
					bbox = bbox.Transform(prlfVis);
					result = result.Combine(bbox);
					SAFERELEASE(prlfVis);
				}
				SAFERELEASE(lpD3DRMVisual);
			}
		}
		SAFERELEASE(lpVisuals);
	}

	return result;
}

void ExtendBox(D3DRMBOX& box, const D3DVECTOR& pt)
{
	if (pt.x < box.min.x)
		box.min.x = pt.x; 
	if (pt.y < box.min.y)
		box.min.y = pt.y; 
	if (pt.z < box.min.z)
		box.min.z = pt.z; 
	if (pt.x > box.max.x)
		box.max.x = pt.x; 
	if (pt.y > box.max.y)
		box.max.y = pt.y; 
	if (pt.z > box.max.z)
		box.max.z = pt.z; 
}

D3DRMBOX CombineBox(const D3DRMBOX& box1, const D3DRMBOX& box2)
{
	D3DRMBOX result = box1;

	ExtendBox(result, box2.min);
	ExtendBox(result, box2.max);

	return result;
}


D3DRMBOX TransformBox(IDirect3DRMFrame *pFrame, const D3DRMBOX& box)
{
	D3DRMMATRIX4D matrix;

	pFrame->GetTransform(matrix);

	D3DRMBOX result;
	InitializeBox(result);

	D3DVECTOR rgrlvVerts[8]; //, vecTmp;
	FillBoxVertices(box, rgrlvVerts);
	for (int i=0; i < 8; i++) 
	{
		// apply the local transform to every vertex
		rgrlvVerts[i] = TransformVertex(matrix, rgrlvVerts[i]);
//		pFrame->Transform(&vecTmp, &rgrlvVerts[i]);
		ExtendBox(result, rgrlvVerts[i]);
	}
	return result;
}

D3DRMBOX InverseTransformBox(IDirect3DRMFrame *pFrame, const D3DRMBOX& box)
{
	//D3DRMMATRIX4D matrix;

	//pFrame->GetTransform(matrix);

	D3DRMBOX result;
	InitializeBox(result);

	D3DVECTOR rgrlvVerts[8], vecTmp;
	FillBoxVertices(box, rgrlvVerts);
	for (int i=0; i < 8; i++) 
	{
		// apply the local transform to every vertex
		pFrame->InverseTransform(&vecTmp, &rgrlvVerts[i]);
		ExtendBox(result, vecTmp);
	}
	return result;
}

void InitializeBox(D3DRMBOX& box)
{
	box.min.x = +1.0e6f;
	box.min.y = +1.0e6f;
	box.min.z = +1.0e6f;
	box.max.x = -1.0e6f;
	box.max.y = -1.0e6f;
	box.max.z = -1.0e6f;
}

// multiply the vec by the matrix
D3DVECTOR TransformVertex(D3DRMMATRIX4D matrix, const D3DVECTOR& vec)
{
	// a4 = b4 * m;
	float a4[4], b4[4];
	D3DVECTOR result;

	b4[0] = vec.x;
	b4[1] = vec.y;
	b4[2] = vec.z;
	b4[3] = 1.0f;

	for (int i = 0; i < 4; i++) 
	{
		float temp = 0.0f;

		for (int j = 0; j < 4; j++)
			temp += b4[j] * matrix[j][i];
		a4[i] = temp;
	}

	result.x = a4[0] / a4[3];
	result.y = a4[1] / a4[3];
	result.z = a4[2] / a4[3];
	return result;
}

D3DVECTOR TransformNormal(D3DRMMATRIX4D matrix, const D3DVECTOR& vec)
{
	// a3 = b3 * m;
	D3DVECTOR result;
	float a3[3], b3[3];
	float temp = 0.0f;

	b3[0] = vec.x;
	b3[1] = vec.y;
	b3[2] = vec.z;

	for (int i=0; i < 3; i++) 
	{
		for (int j=0; j < 3; j++)
			temp += b3[j] * matrix[j][i];

		a3[i] = temp;
	}

	result.x = a3[0];
	result.y = a3[1];
	result.z = a3[2];

	return result;
}

D3DVECTOR GetCenter(const D3DRMBOX& box)
{
	D3DVECTOR center;

	D3DRMVectorAdd(&center, (D3DVECTOR *)&box.min, (D3DVECTOR *)&box.max);
	D3DRMVectorScale(&center, &center, 0.5f);

	return center;
}

void FillBoxVertices(const D3DRMBOX& box, D3DVECTOR* rgrlvVets)
{
	rgrlvVets[0].x = box.min.x;
	rgrlvVets[0].y = box.min.y;
	rgrlvVets[0].z = box.min.z;

	rgrlvVets[1].x = box.min.x;
	rgrlvVets[1].y = box.max.y;
	rgrlvVets[1].z = box.min.z;

	rgrlvVets[2].x = box.min.x;
	rgrlvVets[2].y = box.min.y;
	rgrlvVets[2].z = box.max.z;

	rgrlvVets[3].x = box.min.x;
	rgrlvVets[3].y = box.max.y;
	rgrlvVets[3].z = box.max.z;

	rgrlvVets[4].x = box.max.x;
	rgrlvVets[4].y = box.min.y;
	rgrlvVets[4].z = box.min.z;

	rgrlvVets[5].x = box.max.x;
	rgrlvVets[5].y = box.max.y;
	rgrlvVets[5].z = box.min.z;

	rgrlvVets[6].x = box.max.x;
	rgrlvVets[6].y = box.min.y;
	rgrlvVets[6].z = box.max.z;

	rgrlvVets[7].x = box.max.x;
	rgrlvVets[7].y = box.max.y;
	rgrlvVets[7].z = box.max.z;
}


LPDIRECT3DRMMESHBUILDER MakeBox(D3DRMBOX* box, IDirect3DRM *pD3Drm, float fRed, float fGreen, float fBlue)
{
    LPDIRECT3DRMMESHBUILDER mesh = NULL;
//    LPDIRECT3DRMFACEARRAY faceArray = NULL;
	LPDIRECT3DRMFACE d3drmFace = NULL;
    D3DVECTOR a, b;

    D3DVECTOR zero = {D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0)};
    D3DVECTOR dir = {D3DVAL(0.0), D3DVAL(0.0), D3DVAL(0.0)};

    pD3Drm->CreateMeshBuilder(&mesh);

	//Add the normal arrow (one rod, one cone)
	a.x = (float) fabs(box->max.x + box->min.x) / 2.0F;
	a.y = (float) fabs(box->max.y + box->min.y) / 2.0F;
	a.z = box->max.z;
	dir = a;
	dir.z += D3DVAL(0.2);
    AddRod(mesh, D3DVAL(0.01), a, dir, fRed, fGreen, fBlue);
	a = dir;
    a.z += D3DVAL(0.2);
    AddCone(mesh, D3DVAL(0.03), dir, a, fRed, fGreen, fBlue);

	//Now make the box
    a = box->min;
    b = a;
    b.y = box->max.y;
    AddRod(mesh, D3DVAL(0.01), a, b, fRed, fGreen, fBlue);

    a = b; b.x = box->max.x;
    AddRod(mesh, D3DVAL(0.01), a, b, fRed, fGreen, fBlue);

    a = b; b.y = box->min.y;
    AddRod(mesh, D3DVAL(0.01), a, b, fRed, fGreen, fBlue);

    a = b; b.x = box->min.x;
    AddRod(mesh, D3DVAL(0.01), a, b, fRed, fGreen, fBlue);

    a = b; b.z = box->max.z;
    AddRod(mesh, D3DVAL(0.01), a, b, fRed, fGreen, fBlue);

    a = b; b.x = box->max.x;
    AddRod(mesh, D3DVAL(0.01), a, b, fRed, fGreen, fBlue);

    a = b; b.y = box->max.y;
    AddRod(mesh, D3DVAL(0.01), a, b, fRed, fGreen, fBlue);

    a = b; b.x = box->min.x;
    AddRod(mesh, D3DVAL(0.01), a, b, fRed, fGreen, fBlue);

    a = b; b.y = box->min.y;
    AddRod(mesh, D3DVAL(0.01), a, b, fRed, fGreen, fBlue);

    b.y = box->max.y; a = b; b.z = box->min.z;
    AddRod(mesh, D3DVAL(0.01), a, b, fRed, fGreen, fBlue);

    a = b = box->max; b.z = box->min.z;
    AddRod(mesh, D3DVAL(0.01), a, b, fRed, fGreen, fBlue);

    a.y = box->min.y; b = a; b.z = box->min.z;
    AddRod(mesh, D3DVAL(0.01), a, b, fRed, fGreen, fBlue);

    mesh->SetColor(D3DRMCreateColorRGB(D3DVAL(fRed), D3DVAL(fGreen), D3DVAL(fBlue)));
	mesh->SetQuality( D3DRMSHADE_GOURAUD | D3DRMFILL_SOLID | D3DRMLIGHT_OFF );
/*
	mesh->GetFaces(&faceArray);
	if (faceArray)
	{
		for (int i = 0; i < (int) faceArray->GetSize(); i++)
		{
			d3drmFace = NULL;
			faceArray->GetElement(i, &d3drmFace);
			d3drmFace->SetColorRGB( D3DVAL(BBOX_COLOR_RED), D3DVAL(BBOX_COLOR_GREEN), D3DVAL(BBOX_COLOR_BLUE) );
			d3drmFace->Release();
		}
		faceArray->Release();
	}
*/
    return mesh;
}

// Creates a rod from point 'a' to point 'b' with radius 'radius'
void AddRod(LPDIRECT3DRMMESHBUILDER mesh, D3DVALUE radius, D3DVECTOR a, D3DVECTOR b, float fRed, float fGreen, float fBlue)
{
    D3DVECTOR d, u, r;
    D3DVECTOR v[16];
    D3DVECTOR n[8];
    D3DVALUE f;
    int i;

    /*
     * Find the unit vector along the rod.
     */
    d.x = b.x - a.x;
    d.y = b.y - a.y;
    d.z = b.z - a.z;
    D3DRMVectorNormalise(&d);

    /*
     * Pick a vector normal to d
     */
    if (d.y != D3DVAL(0.0) || d.z != D3DVAL(0.0))
    {   
		u.x = D3DVAL(0.0);
        if (d.y == D3DVAL(0.0))
        {   
			u.y = D3DVAL(1.0);
            u.z = D3DVAL(0.0);
        } 
		else
        {   D3DVALUE n_fix = D3DVAL(1.0) + D3DDivide(D3DMultiply(d.z, d.z), D3DMultiply(d.y, d.y));
            u.z = D3DVAL(sqrt(D3DDivide(D3DVAL(1.0), D3DVAL(n_fix))));
            u.y = -D3DMultiply(u.z, D3DDivide(d.z, d.y));
        }
    } 
	else
    { 
		u.x = D3DVAL(0.0);
        u.y = D3DVAL(0.0);
        u.z = D3DVAL(1.0);
    }

    /*
     * Now find a vector normal to them both, to give us a coordinate
     * system in the plane normal to the rod.
     */
    D3DRMVectorCrossProduct(&r, &d, &u);

    /*
     * Scale down the coordinates to the radius of the rod.
     */
    u.x = D3DMultiply(u.x, radius);
    u.y = D3DMultiply(u.y, radius);
    u.z = D3DMultiply(u.z, radius);
    r.x = D3DMultiply(r.x, radius);
    r.y = D3DMultiply(r.y, radius);
    r.z = D3DMultiply(r.z, radius);

    /*
     * Calculate the corners of an octagon.
     */
    f = D3DVAL((float)sqrt(2.0) / (2 * (1 + (float)sqrt(2.0) / 2)));
    v[0].x = u.x + D3DMultiply(r.x, f);
    v[0].y = u.y + D3DMultiply(r.y, f);
    v[0].z = u.z + D3DMultiply(r.z, f);

    v[1].x = D3DMultiply(u.x, f) + r.x;
    v[1].y = D3DMultiply(u.y, f) + r.y;
    v[1].z = D3DMultiply(u.z, f) + r.z;

    v[2].x = D3DMultiply(-u.x, f) + r.x;
    v[2].y = D3DMultiply(-u.y, f) + r.y;
    v[2].z = D3DMultiply(-u.z, f) + r.z;

    v[3].x = -u.x + D3DMultiply(r.x, f);
    v[3].y = -u.y + D3DMultiply(r.y, f);
    v[3].z = -u.z + D3DMultiply(r.z, f);

    v[4].x = -u.x - D3DMultiply(r.x, f);
    v[4].y = -u.y - D3DMultiply(r.y, f);
    v[4].z = -u.z - D3DMultiply(r.z, f);

    v[5].x = D3DMultiply(-u.x, f) - r.x;
    v[5].y = D3DMultiply(-u.y, f) - r.y;
    v[5].z = D3DMultiply(-u.z, f) - r.z;

    v[6].x = D3DMultiply(u.x, f) - r.x;
    v[6].y = D3DMultiply(u.y, f) - r.y;
    v[6].z = D3DMultiply(u.z, f) - r.z;

    v[7].x = u.x - D3DMultiply(r.x, f);
    v[7].y = u.y - D3DMultiply(r.y, f);
    v[7].z = u.z - D3DMultiply(r.z, f);

    /*
     * Add the rod endpoints and calculate the vertex normals.
     */
    for (i = 0; i < 8; i++)
    {   
		n[i] = v[i];
        D3DRMVectorNormalise(&n[i]);
        v[i + 8].x = v[i].x + b.x;
        v[i + 8].y = v[i].y + b.y;
        v[i + 8].z = v[i].z + b.z;
        v[i].x += a.x;
        v[i].y += a.y;
        v[i].z += a.z;
    }

    /*
     * Now add the faces.
     */
    mesh->AddFaces(16, v, 8, n, rod_faces, NULL);

    for (i = 0; i < 16; i++)
		mesh->SetVertexColorRGB(i, D3DVAL(fRed), D3DVAL(fGreen), D3DVAL(fBlue));

}

void AddCone(LPDIRECT3DRMMESHBUILDER mesh, D3DVALUE radius, D3DVECTOR a, D3DVECTOR b, float fRed, float fGreen, float fBlue)
{
    D3DVECTOR d, u, r;
    D3DVECTOR v[16];
    D3DVECTOR n[8];
    D3DVALUE f;
    int i;

    /*
     * Find the unit vector along the rod.
     */
    d.x = b.x - a.x;
    d.y = b.y - a.y;
    d.z = b.z - a.z;
    D3DRMVectorNormalise(&d);

    /*
     * Pick a vector normal to d
     */
    if (d.y != D3DVAL(0.0) || d.z != D3DVAL(0.0))
    {   
		u.x = D3DVAL(0.0);

        if (d.y == D3DVAL(0.0))
        {   u.y = D3DVAL(1.0);
            u.z = D3DVAL(0.0);
        } 
		else
        {   D3DVALUE n_fix = D3DVAL(1.0) + D3DDivide(D3DMultiply(d.z, d.z), D3DMultiply(d.y, d.y));
            u.z = D3DVAL(sqrt(D3DVAL(1.0) / D3DVAL(n_fix)));
            u.y = - D3DDivide(D3DMultiply(u.z, d.z), d.y);
        }
    } 
	else
    {   u.x = D3DVAL(0.0);
        u.y = D3DVAL(0.0);
        u.z = D3DVAL(1.0);
    }

    /*
     * Now find a vector normal to them both, to give us a coordinate
     * system in the plane normal to the rod.
     */
    
	D3DRMVectorCrossProduct(&r, &d, &u);

    /*
     * Scale down the coordinates to the radius of the rod.
     */
    u.x = D3DMultiply(u.x, radius);
    u.y = D3DMultiply(u.y, radius);
    u.z = D3DMultiply(u.z, radius);
    r.x = D3DMultiply(r.x, radius);
    r.y = D3DMultiply(r.y, radius);
    r.z = D3DMultiply(r.z, radius);

    /*
     * Calculate the corners of an octagon.
     */
    f = D3DVAL((float)sqrt(2.0) / (2 * (1 + (float)sqrt(2.0) / 2)));
    v[0].x = u.x + D3DMultiply(r.x, f);
    v[0].y = u.y + D3DMultiply(r.y, f);
    v[0].z = u.z + D3DMultiply(r.z, f);

    v[1].x = D3DMultiply(u.x, f) + r.x;
    v[1].y = D3DMultiply(u.y, f) + r.y;
    v[1].z = D3DMultiply(u.z, f) + r.z;

    v[2].x = D3DMultiply(-u.x, f) + r.x;
    v[2].y = D3DMultiply(-u.y, f) + r.y;
    v[2].z = D3DMultiply(-u.z, f) + r.z;

    v[3].x = -u.x + D3DMultiply(r.x, f);
    v[3].y = -u.y + D3DMultiply(r.y, f);
    v[3].z = -u.z + D3DMultiply(r.z, f);

    v[4].x = -u.x - D3DMultiply(r.x, f);
    v[4].y = -u.y - D3DMultiply(r.y, f);
    v[4].z = -u.z - D3DMultiply(r.z, f);

    v[5].x = D3DMultiply(-u.x, f) - r.x;
    v[5].y = D3DMultiply(-u.y, f) - r.y;
    v[5].z = D3DMultiply(-u.z, f) - r.z;

    v[6].x = D3DMultiply(u.x, f) - r.x;
    v[6].y = D3DMultiply(u.y, f) - r.y;
    v[6].z = D3DMultiply(u.z, f) - r.z;

    v[7].x = u.x - D3DMultiply(r.x, f);
    v[7].y = u.y - D3DMultiply(r.y, f);
    v[7].z = u.z - D3DMultiply(r.z, f);

    v[8] = b;

    /*
     * Calculate the vertex normals.
     */
    for (i = 0; i < 8; i++)
    {   
		n[i] = v[i];
        D3DRMVectorNormalise(&n[0]);
        v[i].x += a.x;
        v[i].y += a.y;
        v[i].z += a.z;
    }

    /*
     * Now add the faces.
     */
    mesh->AddFaces(9, v, 8, n, cone_faces, NULL);

    for (i = 0; i < 9; i++)
		mesh->SetVertexColorRGB(i, D3DVAL(fRed), D3DVAL(fGreen), D3DVAL(fBlue));

}
////////// End of useful code //////////////////////////////////////////////////////


LPDIRECT3DRMMESHBUILDER MakeWireBox(D3DRMBOX* box, IDirect3DRM *pD3Drm, float fRed, float fGreen, float fBlue)
{
	HRESULT hr = S_OK;
    LPDIRECT3DRMMESHBUILDER meshbuilder = NULL;
	LPDIRECT3DRMFACE d3drmFace = NULL;
	float fPointerLength, fPointerWidth, fPointerHeight;
	D3DVECTOR vec;

    hr = pD3Drm->CreateMeshBuilder(&meshbuilder);
	if (FAILED(hr)) goto EXIT_FAIL;

    //hr = mesh->AddGroup(24, 6, 4, (unsigned*)&fData, &returnId);
	//if (FAILED(hr)) goto EXIT_FAIL;
    //hr = mesh->AddGroup(16, 4, 4, (unsigned*)&fData, &returnId);
	//if (FAILED(hr)) goto EXIT_FAIL;

	meshbuilder->CreateFace(&d3drmFace);

	//XY - Plane Min
	d3drmFace->AddVertex(box->min.x, box->min.y, box->min.z);
	d3drmFace->AddVertex(box->max.x, box->min.y, box->min.z);
	d3drmFace->AddVertex(box->max.x, box->max.y, box->min.z);
	d3drmFace->AddVertex(box->min.x, box->max.y, box->min.z);

	d3drmFace->Release();
	meshbuilder->CreateFace(&d3drmFace);

	//XY - Plane Min - Reverse side
	d3drmFace->AddVertex(box->min.x, box->max.y, box->min.z);
	d3drmFace->AddVertex(box->max.x, box->max.y, box->min.z);
	d3drmFace->AddVertex(box->max.x, box->min.y, box->min.z);
	d3drmFace->AddVertex(box->min.x, box->min.y, box->min.z);

	d3drmFace->Release();
	meshbuilder->CreateFace(&d3drmFace);

	//XY - Plane Max
	d3drmFace->AddVertex(box->min.x, box->min.y, box->max.z);
	d3drmFace->AddVertex(box->max.x, box->min.y, box->max.z);
	d3drmFace->AddVertex(box->max.x, box->max.y, box->max.z);
	d3drmFace->AddVertex(box->min.x, box->max.y, box->max.z);

	d3drmFace->Release();
	meshbuilder->CreateFace(&d3drmFace);

	//XY - Plane Max - Reverse side
	d3drmFace->AddVertex(box->min.x, box->max.y, box->max.z);
	d3drmFace->AddVertex(box->max.x, box->max.y, box->max.z);
	d3drmFace->AddVertex(box->max.x, box->min.y, box->max.z);
	d3drmFace->AddVertex(box->min.x, box->min.y, box->max.z);

	d3drmFace->Release();
	meshbuilder->CreateFace(&d3drmFace);

	//XZ - Plane Min
	d3drmFace->AddVertex(box->min.x, box->min.y, box->min.z);
	d3drmFace->AddVertex(box->max.x, box->min.y, box->min.z);
	d3drmFace->AddVertex(box->max.x, box->min.y, box->max.z);
	d3drmFace->AddVertex(box->min.x, box->min.y, box->max.z);

	d3drmFace->Release();
	meshbuilder->CreateFace(&d3drmFace);

	//XZ - Plane Min - Reverse side
	d3drmFace->AddVertex(box->min.x, box->min.y, box->max.z);
	d3drmFace->AddVertex(box->max.x, box->min.y, box->max.z);
	d3drmFace->AddVertex(box->max.x, box->min.y, box->min.z);
	d3drmFace->AddVertex(box->min.x, box->min.y, box->min.z);

	d3drmFace->Release();
	meshbuilder->CreateFace(&d3drmFace);

	//XZ - Plane Max
	d3drmFace->AddVertex(box->min.x, box->max.y, box->min.z);
	d3drmFace->AddVertex(box->max.x, box->max.y, box->min.z);
	d3drmFace->AddVertex(box->max.x, box->max.y, box->max.z);
	d3drmFace->AddVertex(box->min.x, box->max.y, box->max.z);

	d3drmFace->Release();
	meshbuilder->CreateFace(&d3drmFace);

	//XZ - Plane Max - Reverse side
	d3drmFace->AddVertex(box->min.x, box->max.y, box->max.z);
	d3drmFace->AddVertex(box->max.x, box->max.y, box->max.z);
	d3drmFace->AddVertex(box->max.x, box->max.y, box->min.z);
	d3drmFace->AddVertex(box->min.x, box->max.y, box->min.z);

	d3drmFace->Release();
	meshbuilder->CreateFace(&d3drmFace);

	//YZ - Plane Min
	d3drmFace->AddVertex(box->min.x, box->min.y, box->min.z);
	d3drmFace->AddVertex(box->min.x, box->max.y, box->min.z);
	d3drmFace->AddVertex(box->min.x, box->max.y, box->max.z);
	d3drmFace->AddVertex(box->min.x, box->min.y, box->max.z);

	d3drmFace->Release();
	meshbuilder->CreateFace(&d3drmFace);

	//YZ - Plane Min - Reverse side
	d3drmFace->AddVertex(box->min.x, box->min.y, box->max.z);
	d3drmFace->AddVertex(box->min.x, box->max.y, box->max.z);
	d3drmFace->AddVertex(box->min.x, box->max.y, box->min.z);
	d3drmFace->AddVertex(box->min.x, box->min.y, box->min.z);

	d3drmFace->Release();
	meshbuilder->CreateFace(&d3drmFace);

	//YZ - Plane Max
	d3drmFace->AddVertex(box->max.x, box->min.y, box->min.z);
	d3drmFace->AddVertex(box->max.x, box->max.y, box->min.z);
	d3drmFace->AddVertex(box->max.x, box->max.y, box->max.z);
	d3drmFace->AddVertex(box->max.x, box->min.y, box->max.z);

	d3drmFace->Release();
	meshbuilder->CreateFace(&d3drmFace);

	//YZ - Plane Max - Reverse side
	d3drmFace->AddVertex(box->max.x, box->min.y, box->max.z);
	d3drmFace->AddVertex(box->max.x, box->max.y, box->max.z);
	d3drmFace->AddVertex(box->max.x, box->max.y, box->min.z);
	d3drmFace->AddVertex(box->max.x, box->min.y, box->min.z);

	d3drmFace->Release();
	meshbuilder->CreateFace(&d3drmFace);

	//Pointer stem
	vec.x = (float) fabs(box->max.x + box->min.x) / 2.0F;
	vec.y = (float) fabs(box->max.y + box->min.y) / 2.0F;
	vec.z = box->max.z;
	fPointerLength = (box->max.z-box->min.z)*0.25f;

	d3drmFace->AddVertex(vec.x, vec.y, vec.z);
	d3drmFace->AddVertex(vec.x, vec.y, vec.z + fPointerLength);
	d3drmFace->AddVertex(vec.x-0.005f, vec.y, vec.z);

	d3drmFace->Release();
	meshbuilder->CreateFace(&d3drmFace);

	//Pointer stem - Reverse side
	d3drmFace->AddVertex(vec.x-0.005f, vec.y, vec.z);
	d3drmFace->AddVertex(vec.x, vec.y, vec.z + fPointerLength);
	d3drmFace->AddVertex(vec.x, vec.y, vec.z);

	d3drmFace->Release();
	meshbuilder->CreateFace(&d3drmFace);

	//Pointer head, face 1
	vec.z += fPointerLength;
	fPointerWidth = fPointerLength*0.16f;
	fPointerHeight = fPointerLength*0.32f;
	d3drmFace->AddVertex(vec.x, vec.y, vec.z);
	d3drmFace->AddVertex(vec.x-fPointerWidth, vec.y-fPointerWidth, vec.z-fPointerHeight);
	d3drmFace->AddVertex(vec.x+fPointerWidth, vec.y-fPointerWidth, vec.z-fPointerHeight);

	d3drmFace->Release();
	meshbuilder->CreateFace(&d3drmFace);

	//Pointer head, face 1 - reverse side
	d3drmFace->AddVertex(vec.x+fPointerWidth, vec.y-fPointerWidth, vec.z-fPointerHeight);
	d3drmFace->AddVertex(vec.x-fPointerWidth, vec.y-fPointerWidth, vec.z-fPointerHeight);
	d3drmFace->AddVertex(vec.x, vec.y, vec.z);

	d3drmFace->Release();
	meshbuilder->CreateFace(&d3drmFace);

	//Pointer head, face 2
	d3drmFace->AddVertex(vec.x, vec.y, vec.z);
	d3drmFace->AddVertex(vec.x-fPointerWidth, vec.y-fPointerWidth, vec.z-fPointerHeight);
	d3drmFace->AddVertex(vec.x, vec.y+fPointerWidth, vec.z-fPointerHeight);

	d3drmFace->Release();
	meshbuilder->CreateFace(&d3drmFace);

	//Pointer head, face 2 - reverse side
	d3drmFace->AddVertex(vec.x, vec.y+fPointerWidth, vec.z-fPointerHeight);
	d3drmFace->AddVertex(vec.x-fPointerWidth, vec.y-fPointerWidth, vec.z-fPointerHeight);
	d3drmFace->AddVertex(vec.x, vec.y, vec.z);

	d3drmFace->Release();
	meshbuilder->CreateFace(&d3drmFace);

	//Pointer head, face 3
	d3drmFace->AddVertex(vec.x, vec.y, vec.z);
	d3drmFace->AddVertex(vec.x+fPointerWidth, vec.y-fPointerWidth, vec.z-fPointerHeight);
	d3drmFace->AddVertex(vec.x, vec.y+fPointerWidth, vec.z-fPointerHeight);

	d3drmFace->Release();
	meshbuilder->CreateFace(&d3drmFace);

	//Pointer head, face 3 - reverse side
	d3drmFace->AddVertex(vec.x, vec.y+fPointerWidth, vec.z-fPointerHeight);
	d3drmFace->AddVertex(vec.x+fPointerWidth, vec.y-fPointerWidth, vec.z-fPointerHeight);
	d3drmFace->AddVertex(vec.x, vec.y, vec.z);

	d3drmFace->Release();

    meshbuilder->SetColor(D3DRMCreateColorRGB(D3DVAL(fRed), D3DVAL(fGreen), D3DVAL(fBlue)));
	meshbuilder->SetQuality(D3DRMRENDER_WIREFRAME);

/*
	//XY plane min (line 2)
	d3drmFace->AddVertex(box->min.x, box->min.y, box->min.z);
	d3drmFace->AddVertex(box->max.x, box->min.y, box->min.z);
	d3drmFace->AddVertex(box->min.x  - 0.0001f, box->min.y, box->min.z);

	//XY plane min (line 3)
	d3drmFace->AddVertex(box->min.x, box->min.y, box->min.z);
	d3drmFace->AddVertex(box->min.x, box->min.y, box->max.z);
	d3drmFace->AddVertex(box->min.x, box->min.y, box->min.z - 0.0001f);
	for (i = 0; i < 24; i++)
	{
		ZeroMemory( &(verts[i].normal), sizeof D3DVECTOR);
		ZeroMemory( &(verts[i].tu), sizeof D3DVALUE);
		ZeroMemory( &(verts[i].tv), sizeof D3DVALUE);
		ZeroMemory( &(verts[i].color), sizeof D3DCOLOR);
	}

	//X plane min
	verts[0].position.x = box->min.x;
	verts[0].position.y = box->min.y;
	verts[0].position.z = box->min.z;
	verts[1].position.x = box->min.x;
	verts[1].position.y = box->max.y;
	verts[1].position.z = box->min.z;
	verts[2].position.x = box->min.x;
	verts[2].position.y = box->max.y;
	verts[2].position.z = box->max.z;
	verts[3].position.x = box->min.x;
	verts[3].position.y = box->min.y;
	verts[3].position.z = box->max.z;
	
	//X plane max
	verts[4].position.x = box->max.x;
	verts[4].position.y = box->max.y;
	verts[4].position.z = box->max.z;
	verts[5].position.x = box->max.x;
	verts[5].position.y = box->max.y;
	verts[5].position.z = box->min.z;
	verts[6].position.x = box->min.x;
	verts[6].position.y = box->min.y;
	verts[6].position.z = box->min.z;
	verts[7].position.x = box->min.x;
	verts[7].position.y = box->min.y;
	verts[7].position.z = box->max.z;
	

	//Y plane min
	verts[8].position.x = box->min.x;
	verts[8].position.y = box->min.y;
	verts[8].position.z = box->min.z;
	verts[9].position.x = box->max.x;
	verts[9].position.y = box->min.y;
	verts[9].position.z = box->min.z;
	verts[10].position.x = box->max.x;
	verts[10].position.y = box->min.y;
	verts[10].position.z = box->max.z;
	verts[11].position.x = box->min.x;
	verts[11].position.y = box->min.y;
	verts[11].position.z = box->max.z;
	
	//Y plane max
	verts[12].position.x = box->max.x;
	verts[12].position.y = box->max.y;
	verts[12].position.z = box->max.z;
	verts[13].position.x = box->max.x;
	verts[13].position.y = box->max.y;
	verts[13].position.z = box->min.z;
	verts[14].position.x = box->min.x;
	verts[14].position.y = box->max.y;
	verts[14].position.z = box->min.z;
	verts[15].position.x = box->min.x;
	verts[15].position.y = box->max.y;
	verts[15].position.z = box->max.z;
*/	
//    hr = mesh->SetVertices(returnId, 0, 16, (D3DRMVERTEX*) &verts);
//	if (FAILED(hr)) goto EXIT_FAIL;

//	hr = mesh->SetGroupQuality(returnId, D3DRMRENDER_WIREFRAME);
//	if (FAILED(hr)) goto EXIT_FAIL;

//	mesh->QueryInterface(IID_IDirect3DRMMeshBuilder, (LPVOID*) &meshbuilder);

	goto EXIT_GOOD;
EXIT_FAIL:
	ASSERT(0);

EXIT_GOOD:
	return meshbuilder;
}

/*

	//First the X min plane, 1st triangle
	hr = d3drmFace->AddVertex(box->min.x, box->min.y, box->min.z);
	if (FAILED(hr)) goto EXIT_FAIL;
	hr = d3drmFace->AddVertex(box->min.x, box->max.y, box->min.z);
	if (FAILED(hr)) goto EXIT_FAIL;
	hr = d3drmFace->AddVertex(box->min.x, box->max.y, box->max.z);
	if (FAILED(hr)) goto EXIT_FAIL;

	//Next the X min, plane, 2nd triangle
	hr = d3drmFace->AddVertex(box->min.x, box->min.y, box->min.z);
	if (FAILED(hr)) goto EXIT_FAIL;
	hr = d3drmFace->AddVertex(box->min.x, box->max.y, box->max.z);
	if (FAILED(hr)) goto EXIT_FAIL;
	hr = d3drmFace->AddVertex(box->min.x, box->min.y, box->max.z);
	if (FAILED(hr)) goto EXIT_FAIL;

	//First the Y min plane, 1st triangle
	hr = d3drmFace->AddVertex(box->min.x, box->min.y, box->min.z);
	if (FAILED(hr)) goto EXIT_FAIL;
	hr = d3drmFace->AddVertex(box->max.x, box->min.y, box->min.z);
	if (FAILED(hr)) goto EXIT_FAIL;
	hr = d3drmFace->AddVertex(box->max.x, box->min.y, box->max.z);
	if (FAILED(hr)) goto EXIT_FAIL;

	//Next the Y min, plane, 2nd triangle
	hr = d3drmFace->AddVertex(box->min.x, box->min.y, box->min.z);
	if (FAILED(hr)) goto EXIT_FAIL;
	hr = d3drmFace->AddVertex(box->max.x, box->min.y, box->max.z);
	if (FAILED(hr)) goto EXIT_FAIL;
	hr = d3drmFace->AddVertex(box->min.x, box->min.y, box->max.z);
	if (FAILED(hr)) goto EXIT_FAIL;

	//First the Z min plane, 1st triangle
	hr = d3drmFace->AddVertex(box->min.x, box->min.y, box->min.z);
	if (FAILED(hr)) goto EXIT_FAIL;
	hr = d3drmFace->AddVertex(box->max.x, box->min.y, box->min.z);
	if (FAILED(hr)) goto EXIT_FAIL;
	hr = d3drmFace->AddVertex(box->max.x, box->max.y, box->min.z);
	if (FAILED(hr)) goto EXIT_FAIL;

	//Next the Z min, plane, 2nd triangle
	hr = d3drmFace->AddVertex(box->min.x, box->min.y, box->min.z);
	if (FAILED(hr)) goto EXIT_FAIL;
	hr = d3drmFace->AddVertex(box->max.x, box->max.y, box->min.z);
	if (FAILED(hr)) goto EXIT_FAIL;
	hr = d3drmFace->AddVertex(box->min.x, box->max.y, box->min.z);
	if (FAILED(hr)) goto EXIT_FAIL;
*/