// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef RL_UTIL_H
#define RL_UTIL_H

/////////////// External functions //////////////////////////
D3DRMBOX GetBoundingBox(IDirect3DRMFrame *pFrame);
D3DRMBOX GetSpriteBoundingBox(IDirect3DRMFrame *pFrame);

LPDIRECT3DRMMESHBUILDER MakeBox(D3DRMBOX* box, IDirect3DRM *pD3Drm, float fRed, float fGreen, float fBlue);
LPDIRECT3DRMMESHBUILDER MakeWireBox(D3DRMBOX* box, IDirect3DRM *pD3Drm, float fRed, float fGreen, float fBlue);



//////////////Internal helper functions and definitions//////////////
D3DVECTOR GetCenter(const D3DRMBOX& box);
void FillBoxVertices(const D3DRMBOX& box, D3DVECTOR*);
D3DRMBOX CombineBox(const D3DRMBOX& box1, const D3DRMBOX& box2);
D3DRMBOX TransformBox(IDirect3DRMFrame *pFrame, const D3DRMBOX& box);
D3DRMBOX InverseTransformBox(IDirect3DRMFrame *pFrame, const D3DRMBOX& box);
D3DVECTOR TransformVertex(D3DRMMATRIX4D matrix, const D3DVECTOR& vec);
D3DVECTOR TransformNormal(D3DRMMATRIX4D matrix, const D3DVECTOR& vec);
void InitializeBox(D3DRMBOX& box);
void AddRod(LPDIRECT3DRMMESHBUILDER mesh, D3DVALUE radius, D3DVECTOR a, D3DVECTOR b, float fRed, float fGreen, float fBlue);
void AddCone(LPDIRECT3DRMMESHBUILDER mesh, D3DVALUE radius, D3DVECTOR a, D3DVECTOR b, float fRed, float fGreen, float fBlue);

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
	BoundingBox InverseTransform(IDirect3DRMFrame *pFrame) {
		if (IsEmpty())
			return *this;

		return BoundingBox(InverseTransformBox(pFrame, GetBox()));
	}

protected:
	BOOL IsEmpty() const { return m_bIsEmpty; }
	
	BOOL	m_bIsEmpty;
	D3DRMBOX	m_rlbBox;
};

////////////////////////////////////////////////////////////////////////
BoundingBox xGetBoundingBox(IDirect3DRMFrame *pFrame, BOOL bSprite);

#endif