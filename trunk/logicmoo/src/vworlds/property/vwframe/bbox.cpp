#include "stdafx.h"
#include <d3drmwin.h>
#include "BBox.h"
#include <math.h>

#include <propbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class BoundingBox {
public:
	BoundingBox() {
		m_bIsEmpty = TRUE;
	}
	BoundingBox(const D3DRMBOX& box) {
		m_bIsEmpty = FALSE;
		m_rlbBox   = box;
	}
	D3DRMBOX& GetBox() { return m_rlbBox; }
	const D3DRMBOX& GetBox() const { return m_rlbBox; }

	// operations
	BoundingBox Combine(const BoundingBox& that) {
		if (IsEmpty())
			return that;
		if (that.IsEmpty())
			return *this;
		return BoundingBox(CombineBox(GetBox(), that.GetBox()));
	}
	BoundingBox Transform(IDirect3DRMFrame *pFrame) {
		if (IsEmpty())
			return *this;

		return BoundingBox(TransformBox(pFrame, GetBox()));
	}

protected:
	BOOL IsEmpty() const { return m_bIsEmpty; }
	
	BOOL	m_bIsEmpty;
	D3DRMBOX	m_rlbBox;
};

BoundingBox xGetBoundingBox(IDirect3DRMFrame *pFrame);

D3DRMBOX GetFrameBoundingBox(IDirect3DRMFrame *pFrame)
{
	BoundingBox bbox = xGetBoundingBox(pFrame);
	//TRACE("bbox width is %7.2f\n", bbox.GetBox().max.x - bbox.GetBox().min.x);
	return bbox.GetBox();
}

BoundingBox xGetBoundingBox(IDirect3DRMFrame *pFrame)
{
	BoundingBox result;

	unsigned i;

	// Delete the children
	LPDIRECT3DRMFRAMEARRAY lpChildren = NULL;
	if (SUCCEEDED(pFrame->GetChildren(&lpChildren))) {
		for (i=0; i < lpChildren->GetSize(); i++) {
			IDirect3DRMFrame *pChild = NULL;
			if (SUCCEEDED(lpChildren->GetElement(i, &pChild))) {
				BoundingBox childBox = xGetBoundingBox(pChild);
				childBox = childBox.Transform(pChild);
				result = result.Combine(childBox);
				SAFERELEASE(pChild);
			}
		}
		SAFERELEASE(lpChildren);
	}

	LPDIRECT3DRMVISUALARRAY	lpVisuals;
	if (SUCCEEDED(pFrame->GetVisuals(&lpVisuals))) {
		for (i=0; i < lpVisuals->GetSize(); i++) {
			LPDIRECT3DRMVISUAL lpD3DRMVisual;
			if (SUCCEEDED(lpVisuals->GetElement(i, &lpD3DRMVisual))) {
				IDirect3DRMMeshBuilder *prlmbVis = NULL;
				IDirect3DRMFrame *prlfVis = NULL;

				if (SUCCEEDED(lpD3DRMVisual->QueryInterface(
					IID_IDirect3DRMMeshBuilder, (void **) &prlmbVis))) 
				{
					D3DRMBOX meshBox;
					prlmbVis->GetBox(&meshBox);
					result = result.Combine(BoundingBox(meshBox));
					SAFERELEASE(prlmbVis);
				}
				else if (SUCCEEDED(lpD3DRMVisual->QueryInterface(
					IID_IDirect3DRMFrame, (void **) &prlfVis))) 
				{
					unsigned long len = 100;
					char name[100];
					prlfVis->GetName(&len, name);
					TRACE("calculating bbox for %s\n", name);

					BoundingBox bbox = xGetBoundingBox(prlfVis);
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

	D3DVECTOR rgrlvVerts[8];
	FillBoxVertices(box, rgrlvVerts);
	for (int i=0; i < 8; i++) {
		// apply the local transform to every vertex
		rgrlvVerts[i] = TransformVertex(matrix, rgrlvVerts[i]);
		ExtendBox(result, rgrlvVerts[i]);
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
	b4[0] = vec.x;
	b4[1] = vec.y;
	b4[2] = vec.z;
	b4[3] = 1.0f;

	for (int i=0; i < 4; i++) {
		float temp = 0.0f;
		for (int j=0; j < 4; j++)
			temp += b4[j] * matrix[j][i];
		a4[i] = temp;
	}

	D3DVECTOR result;
	result.x = a4[0] / a4[3];
	result.y = a4[1] / a4[3];
	result.z = a4[2] / a4[3];
	return result;
}

D3DVECTOR TransformNormal(D3DRMMATRIX4D matrix, const D3DVECTOR& vec)
{
	// a3 = b3 * m;
	float a3[3], b3[3];
	b3[0] = vec.x;
	b3[1] = vec.y;
	b3[2] = vec.z;

	for (int i=0; i < 3; i++) {
		float temp = 0.0f;
		for (int j=0; j < 3; j++)
			temp += b3[j] * matrix[j][i];
		a3[i] = temp;
	}

	D3DVECTOR result;
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

