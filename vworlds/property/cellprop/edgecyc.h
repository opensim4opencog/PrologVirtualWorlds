// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// EdgeCyc.h : Declaration of the CBoundary

#include <resource.h>

#include "darray.h"

#include "geom2d.h"

#include <propbase.h>
#include <objimpl.h>

EXTERN_C const IID LIBID_VWMMLib;
EXTERN_C const CLSID CLSID_Boundary;

#define NULL_EINDEX	-1
#define NULL_VINDEX	-1
typedef int EIndex;			// Edge Index
typedef int VIndex;			// Vertex Index
typedef int ECIndex;		// Boundary Index

// class Isect2;

/////////////////////////////////////////////////////////////////////////////
// CellProp

DECLARE_OBJECTPROPERTYEX(CBoundary, IBoundary, IID_IBoundary, CLSID_Boundary, LIBID_VWMMLib, IDS_BOUNDARY_TYPE)
{
DECLARE_AGGREGATABLE(CBoundary)

	CBoundary();
	~CBoundary();
public:
BEGIN_COM_MAP(CBoundary)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectProperty)
	COM_INTERFACE_ENTRY(IBoundary)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(CBoundary) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

DECLARE_REGISTRY(CBoundary, _T("VWSYSTEM.Boundary.1"), _T("VWSYSTEM.Boundary"), IDS_BOUNDARY_DESC, THREADFLAGS_BOTH)

// Added for Aggregation
DECLARE_GET_CONTROLLING_UNKNOWN()

public:
	// Override the base class
	STDMETHOD(Marshall)(DWORD dwOptions, IMarshallBuffer* pbuffer); 
	STDMETHOD(UnMarshall)(IUnMarshallBuffer* pbuffer); 
	STDMETHOD(MarshallToScript)(DWORD dwOptions, BSTR btrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut);
	STDMETHOD(Terminate)(void);
	STDMETHOD(get_RefCount)(DWORD *pdwRef);

// IBoundary
public:
	STDMETHOD(Intersect)(IPropertyList *pCell, int iBoundaryIndex, float x1, float y1, float z1, float x2, float y2, float z2, IPropertyList **pIDataList);
	STDMETHOD(IsWithinExt)(Point2 *pnt, VARIANT_BOOL *pbInside);
	STDMETHOD(IsWithin)(float fX, float fY, VARIANT_BOOL *pbInside);
	STDMETHOD(get_EdgeAttr)(int index, unsigned char *pEdgeAttr);
	STDMETHOD(get_Vertex)(int index, IVector **ppvertex);
	STDMETHOD(GetBoundingBox)(VARIANT* pvertUpperLeft, VARIANT* pvertLowerRight);
	STDMETHOD(GetBoundingBoxExt)(IVector **ppvertexUL, IVector **ppvertexLR);
	STDMETHOD(GetCenter)(VARIANT* varfCenterX, VARIANT* varfCenterY);
	STDMETHOD(GetCenterExt)(float* fCenterX, float* fCenterY);
	STDMETHOD(Set)(int nEdges, Point2 *trgpntVerts, unsigned char *trgEAttrs);
	STDMETHOD(SetIncomplete)(int nEdges, Point2 *trgpntVerts, unsigned char *trgEAttrs);
	STDMETHOD(put_HeightUpper)(float fHeightUpper);
	STDMETHOD(get_HeightUpper)(float* fHeightUpper);
	STDMETHOD(put_HeightLower)(float fHeightLower);
	STDMETHOD(get_HeightLower)(float* fHeightLower);
	STDMETHOD(put_IsPassable)(VARIANT_BOOL bPassable);
	STDMETHOD(get_IsPassable)(VARIANT_BOOL* pbPassable);
	STDMETHOD(put_IsProximity)(VARIANT_BOOL bProximity);
	STDMETHOD(get_IsProximity)(VARIANT_BOOL* pbProximity);
	STDMETHOD(get_VertexCount)(int* nEdges);
	STDMETHOD(put_IsInverse)(VARIANT_BOOL bInverse);
	STDMETHOD(get_IsInverse)(VARIANT_BOOL* pbInverse);
	STDMETHOD(GetVertexXY)(int nEdge, VARIANT* pvarfX, VARIANT* pvarfY);
	STDMETHOD(GetVertexXYExt)(int nEdge, float* pfX, float* pfY);
	STDMETHOD(SetVertexXY)(int nEdge, float fX, float fY);
	STDMETHOD(Translate)(float flX, float flY);
//	STDMETHOD(TranslateVertex)(int nIndex, float flX, float flY);
	STDMETHOD(Rotate)(float fAngle);
	STDMETHOD(Scale)(float fX, float fY);
	STDMETHOD(DeleteVertexSafe)(long nIndex, VARIANT_BOOL *pBool);
	STDMETHOD(DeleteVertex)(long nIndex);
	STDMETHOD(TranslateVertexSafe)(long nIndex, float fX, float fY, VARIANT_BOOL* pBool);
	STDMETHOD(TranslateVertex)(long nIndex, float fX,  float fY);
	STDMETHOD(InsertVertexSafe)(long nIndex, float fX, float fY,  VARIANT_BOOL* pBool);
	STDMETHOD(InsertVertex)(long nIndex, float fX, float fY);
	STDMETHOD(put_IsClosed)(VARIANT_BOOL vbClosed);
	STDMETHOD(get_IsClosed)(VARIANT_BOOL* pvbClosed);
	STDMETHOD(AutoGenerate)(IThing* pThing, int nSides = 4, float fExtraRadius = 0.0f);
	STDMETHOD(AutoGenerateFromBox)(float fMinX, float fMinY, float fMinZ, float fMaxX, float fMaxY, float fMaxZ, int nSides, float fExtraRadius);
	STDMETHOD(put_IsAutoSolidBoundary)(VARIANT_BOOL bAutoSolid);
	STDMETHOD(get_IsAutoSolidBoundary)(VARIANT_BOOL* pbAutoSolid);

// internal calls
	void				Init(int nEdges, Point2 *tprgVerts, unsigned char *tprgAttrs);
	BOOL				Sacrifice();
	HRESULT				GeneratePoints(IThing* pThing, int nSides, Point2* pnts, float fExtraRadius);

	//
	// DATA ACCESS & QUERY FUNCTIONS
	//
	unsigned int		DoIntersect(IPropertyList *pCell, int iBoundaryIndex, const Point3 &p1, const Point3 &p2, IPropertyList **pIDataList);
	VIndex				GetNEdges() const		{ return cEdges; }
	VIndex				GetLeftBottom() const	{ return CBoundary::GetLeftBottom(rgpntVerts, cEdges); }
	BOOL				IsCCW() const			{ return ((m_Attrs & ATTR_CCW) == ATTR_CCW); }
	BOOL				IsCW() const			{ return !IsCCW(); }
	BOOL				IsConvexPoly() const	{ return ((m_Attrs & ATTR_CONVEXPOLY) == ATTR_CONVEXPOLY); }
	BOOL				IsConvex() const		{ return (IsConvexPoly() && IsCCW()); }

private:
	//
	// I/O FUNCTIONS
	//

	BOOL				Compile();
	BOOL				CheckCrossing(IPropertyList* pCell, int iBoundaryIndex, VIndex nSrc, VIndex nDst, const Point3 &p1, const Point3 &p2, BOOL bTransition, IPropertyList **pIDataList);
	HRESULT				PutIDataInSortedList(IPropertyList *pIDataListSorted, IVWIntersectData *pVWIData);
	
public:
	static BOOL			IsValid(const Point2 *rgpntVerts, int cEdges, BOOL bClosed);
	static BOOL			IsSimplePoly(const Point2 *rgpntVerts, int cEdges, BOOL bClosed);
	static BOOL			IsConvexPoly(const Point2 *rgpntVerts, int cEdges, BOOL bCCW);
	static BOOL			IsConvex(const Point2 *rgpntVerts, int cEdges);
	static BOOL			IsCCW(const Point2 *rgpntVerts, int cEdges);
	static EIndex		GetLeftBottom(const Point2 *rgpntVerts, int cEdges);

private:
	VARIANT_BOOL		m_bClosed;
	BBox2				bbox;		// 2D bounding box for Boundary
	Point2 *			rgpntVerts;	// array of edge vertices
	unsigned char*		rgEAttrs;	// array of pointers to edge attrs
	int					cEdges;		// number of edges (and vertices) in cycle
	unsigned char		m_Attrs;	// bit flags associated with the Boundary
	float				m_fHeightUpper;
	float				m_fHeightLower;

	
public:
};

inline void
CBoundary::Init(int nEdges, Point2 *trgpntVerts, unsigned char *trgEAttrs)
{
	m_Attrs = 0;
	VERIFY(Set(nEdges, trgpntVerts, trgEAttrs));
}

inline BOOL
CBoundary::Sacrifice()
{
	if (rgpntVerts)
	{
		delete [] rgpntVerts;
		rgpntVerts = NULL;
	}

	if (rgEAttrs)
	{
		delete [] rgEAttrs;
		rgEAttrs = NULL;
	}

	return TRUE;
}
