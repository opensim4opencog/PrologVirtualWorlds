#ifndef BBOX_H
#define BBOX_H

D3DRMBOX GetFrameBoundingBox(IDirect3DRMFrame *pFrame);
D3DVECTOR GetCenter(const D3DRMBOX& box);
void FillBoxVertices(const D3DRMBOX& box, D3DVECTOR*);
D3DRMBOX CombineBox(const D3DRMBOX& box1, const D3DRMBOX& box2);
D3DRMBOX TransformBox(IDirect3DRMFrame *pFrame, const D3DRMBOX& box);
D3DVECTOR TransformVertex(D3DRMMATRIX4D matrix, const D3DVECTOR& vec);
D3DVECTOR TransformNormal(D3DRMMATRIX4D matrix, const D3DVECTOR& vec);
void InitializeBox(D3DRMBOX& box);

#endif