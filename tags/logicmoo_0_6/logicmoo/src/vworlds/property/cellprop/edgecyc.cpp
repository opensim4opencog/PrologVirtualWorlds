// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// EdgeCyc.cpp : Implementation for IBoundary

#include "stdafx.h"
#include <vwobject.h>
#include <d3drmwin.h>
#include <math.h>
#include "point23.h"
#include <vwidata.h>
#include "vwidatao.h"
#include "EdgeCyc.h"
#include "syshelp.h"
#include "mmhelp.h"
#include "vwmmex.h"

#define CHECKHR(x)  { hr = (x);  if (hr) { goto ERROR_ENCOUNTERED; } }
#define PI			3.1415926F
#define fSafeDist	0.01F

#if 0
// force debugging of IThing creation/destruction
#undef TRACE_ALLOCATION

#define TRACE_ALLOCATION VWT_NONE
//#define TRACE_ALLOCATION VWT_METHOD
#endif


typedef CComObject<CVWIntersectDataObject> CComIntersectData;
typedef CVWComPtr<IBoundary,NULL,&IID_IBoundary> CBoundaryPtr;

IMPLEMENT_OBJECTPROPERTY(CBoundary)

EXTERN_C const CLSID CLSID_Vector;

/////////////////////////////////////////////////////////////////////////////
//
CBoundary::CBoundary()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CBoundary::CBoundary, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif

	cEdges = 0; 
	rgpntVerts = NULL; 
	rgEAttrs = NULL; 
	m_Attrs = 0;
	m_fHeightUpper = m_fHeightLower = 0.0F;
	m_bClosed = VARIANT_TRUE;
}

CBoundary::~CBoundary()
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CBoundary::~CBoundary, this 0x%0x\n", this);
#endif
	Terminate();
}

STDMETHODIMP CBoundary::Terminate(void)
{
#ifdef _DEBUG
	VWTRACE(m_pWorld, "VWOBJECT", TRACE_ALLOCATION, "CBoundary::Terminate, this 0x%0x\n", this);
#endif

	CBoundaryBase::Terminate();

	Sacrifice();

	return S_OK;
}

STDMETHODIMP CBoundary::Marshall(DWORD dwOptions, IMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = CBoundaryBase::Marshall(dwOptions, pbuffer)))
		return hr;

	EIndex i;

	ASSERT(pbuffer);

	// Marshall number of edges.
	if (FAILED(hr = pbuffer->put_Int(cEdges)))
		goto ERROR_ENCOUNTERED;

	// Marshall the vertices and edge attributes.
	for (i = 0; i < cEdges; i++) {
		if (FAILED(hr = pbuffer->put_Float(rgpntVerts[i].vecCoord.x)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = pbuffer->put_Float(rgpntVerts[i].vecCoord.y)))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = pbuffer->put_Byte(rgEAttrs[i])))
			goto ERROR_ENCOUNTERED;
	}

	// Marshall the upper/lower heights.
	if (FAILED(hr = pbuffer->put_Float(m_fHeightUpper)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pbuffer->put_Float(m_fHeightLower)))
		goto ERROR_ENCOUNTERED;
	
	// Marshall the edge cycle attributes.
	if (FAILED(hr = pbuffer->put_Byte(m_Attrs)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pbuffer->put_Boolean(m_bClosed)))
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CBoundary::UnMarshall(IUnMarshallBuffer* pbuffer)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = CBoundaryBase::UnMarshall(pbuffer)))
		return hr;

	EIndex i;

	ASSERT(pbuffer);

	// free old data
	Sacrifice();

	// How many edges does this edge cycle have?
	if (FAILED(hr = pbuffer->get_Int(&cEdges)))
		goto ERROR_ENCOUNTERED;

	// Allocate space for vertices.
	rgpntVerts = new Point2[cEdges];
	rgEAttrs = new unsigned char[cEdges];

	// Unmarshall the vertices and edge attributes.
	for (i = 0; i < cEdges; i++) {
		if (FAILED(hr = pbuffer->get_Float(&(rgpntVerts[i].vecCoord.x))))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = pbuffer->get_Float(&(rgpntVerts[i].vecCoord.y))))
			goto ERROR_ENCOUNTERED;
		if (FAILED(hr = pbuffer->get_Byte(&(rgEAttrs[i]))))
			goto ERROR_ENCOUNTERED;
	}

	// ignore HRESULT because we don't want UnMarshall to fail
	Compile();

	// Unmarshall the upper/lower heights.
	if (FAILED(hr = pbuffer->get_Float(&m_fHeightUpper)))
		goto ERROR_ENCOUNTERED;
	if (FAILED(hr = pbuffer->get_Float(&m_fHeightLower)))
		goto ERROR_ENCOUNTERED;

	// Unmarshall the edge cycle attributes.
	if (FAILED(hr = pbuffer->get_Byte(&m_Attrs)))
		goto ERROR_ENCOUNTERED;

	if (FAILED(hr = pbuffer->get_Boolean(&m_bClosed))) 
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CBoundary::MarshallToScript(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut)
{
	HRESULT hr = S_OK;
	CString strOut;
	CString strOne;
	CString strContext;
	VARIANT_BOOL b;

	int i;

	if (bstrContext==NULL || pbAsVal==NULL || pbstrOut==NULL)
		return E_INVALIDARG;

	strContext = bstrContext;

	// initialize
	*pbstrOut = NULL;
	*pbAsVal = VARIANT_FALSE;

	strOne.Format("\tset %s = World.CreateBoundary(Array(_\n\t\t", (LPCTSTR) CString(strContext) );
	strOut += strOne;

	// insert the vertices into the builder
	for (i = 0; i < cEdges; i++)
	{
		strOne.Format("%f, %f", rgpntVerts[i].X(), rgpntVerts[i].Y());
		strOut += strOne;

		if (i != cEdges - 1)
		{
			strOne.Format(", ");
			strOut += strOne;
		}

		// insert line break every 4 vertex pairs
		if ((i+1) % 4 == 0)
		{
			strOne.Format("_\n\t\t");
			strOut += strOne;
		}
	}

	strOne = "))\n";
	strOut += strOne;

	// set the upper and lower stuff
	strOne.Format(	"\t%s.HeightUpper = %g\n"
					"\t%s.HeightLower = %g\n",
					(LPCTSTR) CString(strContext),
					m_fHeightUpper,
					(LPCTSTR) CString(strContext),
					m_fHeightLower );
	strOut += strOne;

	// set the the proximity and passable information if different from default
	hr = get_IsProximity(&b);
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;

	if (b==VARIANT_TRUE)
	{
		strOne.Format(	"\t%s.IsProximity = True\n", (LPCTSTR) CString(strContext) );
		strOut += strOne;
	}

	hr = get_IsPassable(&b);
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;

	if (b==VARIANT_TRUE)
	{
		strOne.Format(	"\t%s.IsPassable = True\n", (LPCTSTR) CString(strContext) );
		strOut += strOne;
	}

	hr = get_IsClosed(&b);
	if (FAILED(hr)) goto ERROR_ENCOUNTERED;

	if (b==VARIANT_FALSE)
	{
		strOne.Format(	"\t%s.IsClosed = False\n", (LPCTSTR) CString(strContext));
		strOut += strOne;
	}

	*pbstrOut = strOut.AllocSysString();

ERROR_ENCOUNTERED:

	return hr;
}

STDMETHODIMP CBoundary::get_EdgeAttr(int nEdge, unsigned char *pEdgeAttr)
{
	HRESULT hr = S_OK;

	// Fail if asking for an Edge too high or under 0
	if( (nEdge >= cEdges )
		|| (nEdge < 0 ) )
	{
		return E_INVALIDARG;
	}

	*pEdgeAttr = rgEAttrs[nEdge];
	return S_OK;
}

// Method added by DS for bootstrapping the Boundary Editor.
STDMETHODIMP CBoundary::get_Vertex(int nEdge, IVector **ppvector)
{
	HRESULT hr = S_OK;
	// Fail if asking for an Edge too high or under 0
	if( (nEdge >= cEdges )
		|| (nEdge < 0 ) )
	{
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	//	Stuff Vertice's x and y into Vector.
	hr = CreateVector(m_pWorld, rgpntVerts[nEdge].vecCoord.x, rgpntVerts[nEdge].vecCoord.y, 0.0, ppvector);
	if( FAILED(hr) ) goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	// Don't leave dangling Vector if error occurred.
	if(FAILED(hr))
		SAFERELEASE(*ppvector);
	return hr;
}

//Method added by DS for bootstrapping the Boundary Editor.
STDMETHODIMP CBoundary::GetBoundingBox( VARIANT* pvertUpperLeft, VARIANT* pvertLowerRight)
{
	HRESULT hr = S_OK;
	Point2 pt2;

	ASSERT(pvertUpperLeft);
	ASSERT(pvertLowerRight);

	if (!pvertUpperLeft || !pvertLowerRight)
	{
		hr = E_POINTER;
	}
	else
	{
		VariantClear(pvertUpperLeft);
		VariantClear(pvertLowerRight);

		pvertUpperLeft->vt = VT_DISPATCH;
		pvertLowerRight->vt = VT_DISPATCH;

		hr = GetBoundingBoxExt( (IVector**) &(pvertUpperLeft->pdispVal), (IVector**) &(pvertLowerRight->pdispVal));
	}

	if (FAILED(hr))
	{
		VariantClear(pvertUpperLeft);
		VariantClear(pvertLowerRight);
	}

	return hr;
}

STDMETHODIMP CBoundary::GetBoundingBoxExt( IVector **ppvectorUL,  IVector **ppvectorLR)
{
	HRESULT hr = S_OK;
	Point2 pt2;

	if (ppvectorUL == NULL || ppvectorLR == NULL)
		return E_POINTER;

	//	Stuff UpperLeft Vertice's x and y into Vector.
	pt2 = bbox.Get_minPnt();

	hr = CreateVector(m_pWorld, pt2.vecCoord.x, pt2.vecCoord.y, 0.0, ppvectorUL);
	if( FAILED(hr) ) goto ERROR_ENCOUNTERED;

		//	Stuff LowerRight Vertice's x and y into Vector.
	pt2 = bbox.Get_maxPnt();

	hr = CreateVector(m_pWorld, pt2.vecCoord.x, pt2.vecCoord.y, 0.0, ppvectorLR);
	if( FAILED(hr) ) goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:
	// Don't leave dangling Vector if error occurred.
	if(FAILED(hr))
	{
		SAFERELEASE(*ppvectorUL);
		SAFERELEASE(*ppvectorLR);
	}
	return hr;
}

//Gets center for rotation purposes in boundary Editor
STDMETHODIMP CBoundary::GetCenter(VARIANT* varfCenterX, VARIANT* varfCenterY)
{
	HRESULT hr = S_OK;

	if (!varfCenterX || !varfCenterX)
	{
		hr = E_POINTER;
	}
	else
	{
		VariantClear(varfCenterX);
		VariantClear(varfCenterY);

		varfCenterX->vt = VT_R4;
		varfCenterY->vt = VT_R4;

		hr = GetCenterExt(&varfCenterX->fltVal, &varfCenterY->fltVal); 
	}

	if (FAILED(hr))
	{
		VariantClear(varfCenterX);
		VariantClear(varfCenterY);
	}

	return hr;
}

//Gets center for rotation purposes in boundary editor
STDMETHODIMP CBoundary::GetCenterExt(float* fCenterX, float* fCenterY)
{
	HRESULT hr = S_OK;
	Point2 pt1, pt2;

	ASSERT(fCenterX);
	ASSERT(fCenterY);

	if (fCenterX == NULL || fCenterY == NULL)
		return E_POINTER;

	pt1 = bbox.Get_minPnt();
	pt2 = bbox.Get_maxPnt();

	*fCenterX = (float) ((pt2.vecCoord.x - pt1.vecCoord.x) / 2.0 + pt1.vecCoord.x);
	*fCenterY = (float) ((pt2.vecCoord.y - pt1.vecCoord.y) / 2.0 + pt1.vecCoord.y);

	return hr;
}

STDMETHODIMP CBoundary::Intersect(IPropertyList* pCell, int iBoundaryIndex, float x1, float y1, float z1, float x2, float y2, float z2, IPropertyList **ppIDataList)
{
	HRESULT hr = S_OK;
	int hit;
	Point3	p1(x1,y1,z1), p2(x2,y2,z2);

	hit = DoIntersect(pCell, iBoundaryIndex, p1, p2, ppIDataList);
	if (hit)
	{
		return S_INTERSECTION;
	} 

	return hr;
}

////////////////////////////
// Boundary FUNCTIONS
////////////////////////////

// Function: DoIntersect
//  Returns: Number of times segment intersects with Boundary - notice that
//     because the Boundarys are directed, we still cull out backfacing
//     intersections.
//
//     Intersect an edge cycle with a line segment.  This is done by
//  classifying vertices as being on one side of the segment or the other and
//  checking segment intersection for consectutive vertices that change sides.
//  We get back-face culling for free because edge cycles are ordered and one-
//  sided, so we only check for itersections when the dot products change sign in
//  the proper direction.  The algorithm finds all intersections that pierce the
//  cycle, even when there is numerical error.  This is true because we
//  calculate dot products only once per vertex, so a vertex of a closed cycle
//  will find that one segment or the other causes the crossing.  Segments that
//  are coincident with an edge or just a vertex may or may not cause a
//  collision depending on numerical error.  In particular, if the origin of the
//  intersecting segment is on the edge or vertex of a cycle, a collision may
//  (or may not) be found approximately at the origin.  Right now, there is no
//  threshholding to discard such intersections (assuming that is the right
//  thing to do, as is often the case in a ray tracer).
//
// Notes:
//  Edges point from src to dst.  Edge & Segment normals point to the left
//  looking from src to dst.  The computation for the normal is:
//  	N = (src.y - dst.y, dst.x - src.x)
//  To decide if a point, P, is to the left of an edge, compute if the
//  projection along N of a directed vector from either src or dst to P
//  is positive (since N points to the left).  Thus:
//  	CCW > 0 ==> P on left
//		CCW == 0 ==> P on segment
//		CCW < 0 ==> P on right
//  where CCW = (P - dst) * N = (P - src) * N.  This computation is 
//  numerically inaccurate if P is "far" from the segment endpoint being
//  used.  CCW is probably almost never numerically zero even if logically
//  that is the case, since this code uses floating point.
//
//  Computing all numbers from scratch:
//		CCW = (P.x - dst.x)(src.y - dst.y) + (P.y - dst.y)(dst.x - src.x)
//  requires 6+$ loads, 5 adds, 2 muls.  $ = 0 for a machine with f.p.
//  registers and no thrashing.  $ = 2 for a stack based f.p.
//
//  Storing edge normals:
//		CCW = (P.x - dst.x)(N.x) + (P.y - dst.y)(N.y)
//  requires 6 loads, 3 adds, 2 muls.
//
//  We can also compute CCW with the line equation:
//		CCW = P.x * N.x + P.y * N.y + C
//  where C = dst.y * src.x - dst.x * src.y.  Assuming N and C are
//  precomputed, we have: 5 loads, 2 adds, 2 muls.
//
//  Option 1 requires the least amount of memory, since we must store
//  the vertices no matter what (to do CheckCrossing).  On a RISC machine,
//  you might take advantage of having already loaded up the registers in
//  order to do the CheckCrossing.
//  
//  The size of the edge information structure essentially dictates the
//  overall memory consumption of the data structure.  Storing precomputed
//  results at the edge level can significantly inflate memory usage and
//  might cause cache concerns.
//
//  This code uses Option 1.
//
unsigned int
CBoundary::DoIntersect(IPropertyList* pList, int iBoundaryIndex, const Point3 &p1, const Point3 &p2, IPropertyList **ppIDataList)
{
	float	x1, y1, z1, x2, y2, z2, fLocalSafeDist = fSafeDist;
	IVWIntersectData	*pVWID = NULL;
	IVector				*pvProjection = NULL;
	unsigned int		uHits = 0;
	VARIANT_BOOL		bIsWithin;
	float				fDistToIntersect;
	Point2				pIntersect2D;

	// Need at least 3 vertices/edges to be a valid boundary (protects against "bad" boundaries created in script)
	if (cEdges < 3)
		return 0;

	// Need to think in just X/Z plane for most of this stuff.
	Point2 p12D(p1.X(), p1.Z());
	Point2 p22D(p2.X(), p2.Z());
	Segment2 seg2D(p12D, p22D);

	// Only do XZ stuff if segment bounding box intersects (overlaps) cycle bounding box
	if (bbox.Intersect(seg2D.GetBBox()))
	{
		// REVIEW: should we check if segment intersects cycle bounding box?
	//	if ((cVerts > CYCLE_THRESH) && (!bBox.Intersect(p1, p2))) return FALSE;
		
		// ALGORITHM:  Intersect each of the segments in the edge cycle,
		// using a finite state machine to keep track of the current sign.
		
		// FINITE STATE MACHINE
		// traverse the cycle BACKWARDS:
		// - only transitions from pos to neg are front facing
		// - current edge index and the current vertex index are the same 
		//   ==> use one index: iEdge
		EIndex iVert = 0;
		EIndex maxVert = cEdges;

		const CoVector2 &seg2DNormal = seg2D.GetNorm();
		const Point2 &seg2DVert = seg2D.GetDst();

		// save the first dot product so we can use it on the last compare
		float flE0 = seg2DNormal * (rgpntVerts[0] - seg2DVert);
		
		// start in the appropriate state
		if (flE0 < 0.0f) goto neg_state;

		// only transition from neg to pos state causes intersect, o.w. backface cull
		// NOTE: we are chucking the sidedness - so check the backfacing edges
pos_state:
		if (++iVert == maxVert) {
			// reuse computed dot producted
			if (flE0 < 0.0f) {
				uHits += CheckCrossing(pList, iBoundaryIndex, iVert-1, 0, p1, p2, 1, ppIDataList);
			}
			goto FINISHED_XZ;
		}

		if (seg2DNormal * (rgpntVerts[iVert] - seg2DVert) >= 0.0f) 
			goto pos_state;

		// fall thru check
		uHits += CheckCrossing(pList, iBoundaryIndex, iVert-1, iVert, p1, p2, 1, ppIDataList);

neg_state:
		if (++iVert == maxVert) {
			// reuse computed dot producted
			if (flE0 >= 0.0f) 
				uHits += CheckCrossing(pList, iBoundaryIndex, iVert-1, 0, p1, p2, 0, ppIDataList);
		} else {
			if (seg2DNormal * (rgpntVerts[iVert] - seg2DVert) >= 0.0f) {
				uHits += CheckCrossing(pList, iBoundaryIndex, iVert-1, iVert, p1, p2, 0, ppIDataList);
				goto pos_state;
			}
			goto neg_state;
		}
	}
	else
	{
		if (!IsCW())
		{
			// If the segment bounding box does not overlap the cycle bounding
			// box then the cycle has to be clockwise else we will not intersect
			// its heights.
			goto ERROR_ENCOUNTERED;
		}
	}

FINISHED_XZ:

	// OK we've finished finding all the intersections with the edges of this edge cycle.
	// Now how about the heights of this edge cycle.
	x1 = p1.X();
	y1 = p1.Y();
	z1 = p1.Z();
	x2 = p2.X();
	y2 = p2.Y();
	z2 = p2.Z();

	if (m_fHeightUpper == m_fHeightLower)
		goto ERROR_ENCOUNTERED;

	if ((y1 > m_fHeightUpper && y2 <= m_fHeightUpper) ||
		(y1 < m_fHeightUpper && y2 >= m_fHeightUpper))
	{
		// We've moved down or up onto this edge cycle's upper height.

		fDistToIntersect = (float)fabs((m_fHeightUpper-y1)/(y2-y1));

		// First check if point of intersection is within the edge cycle.
		pIntersect2D = p12D+((p22D-p12D)*fDistToIntersect);
		IsWithinExt(&pIntersect2D, &bIsWithin);
		if (!bIsWithin)
		    goto ERROR_ENCOUNTERED;

		// Create the necessary intersect data for this intersect.
		pVWID = new CComIntersectData;
	    if (!pVWID)
		    goto ERROR_ENCOUNTERED;
	    pVWID->AddRef();
		pVWID->put_BoundaryList(pList);
		pVWID->put_BoundaryIndex(iBoundaryIndex);
		pVWID->put_DistToIntersect(fDistToIntersect);

		//Reverse transition variable if this is a clock-wise boundary or an inverse
		//boundary, but not both.
		int nTrans;

		if (y1 > m_fHeightUpper)
			nTrans = 1;
		else
		{
			nTrans = 0;
			fLocalSafeDist = -fLocalSafeDist;
		}

		if ( ( (m_Attrs & ATTR_CCW) && (m_Attrs & ATTR_INVERSE)) ||
			 ( !(m_Attrs & ATTR_CCW) && !(m_Attrs & ATTR_INVERSE)) )
		{
			nTrans = (nTrans ? 0 : 1);
		}

		pVWID->put_Transition(nTrans);

		if (!(m_Attrs & ATTR_PASSABLE))
		{
			if (FAILED(CreateVector(m_pWorld, x2, m_fHeightUpper+fLocalSafeDist, z2, &pvProjection)))
				goto ERROR_ENCOUNTERED;
			pVWID->put_Projection(pvProjection);
		}
		if (!(*ppIDataList))
		{
			// We need to create the list.
			if(FAILED(CreatePropertyList(m_pWorld, ppIDataList)))
				goto ERROR_ENCOUNTERED;
		}
		// Save the intersect data for this intersect in the appropriate list position.
		if (FAILED(PutIDataInSortedList(*ppIDataList, pVWID)))
			goto ERROR_ENCOUNTERED;
		uHits++;
	}
	else if ((y1 > m_fHeightLower && y2 <= m_fHeightLower) ||
			 (y1 < m_fHeightLower && y2 >= m_fHeightLower))
	{
		// We've moved down or up onto this edge cycle's lower height.

		fDistToIntersect = (float)fabs((m_fHeightLower-y1)/(y2-y1));

		// First check if point of intersection is within the edge cycle.
		pIntersect2D = p12D+((p22D-p12D)*fDistToIntersect);
		IsWithinExt(&pIntersect2D, &bIsWithin);
		if (!bIsWithin)
		    goto ERROR_ENCOUNTERED;

		// Create the necessary intersect data for this intersect.
	    pVWID = new CComIntersectData;
	    if (!pVWID)
		    goto ERROR_ENCOUNTERED;
	    pVWID->AddRef();
		pVWID->put_BoundaryList(pList);
		pVWID->put_BoundaryIndex(iBoundaryIndex);
		pVWID->put_DistToIntersect(fDistToIntersect);

		//Reverse transition variable if this is a clock-wise boundary or an inverse
		//boundary, but not both.
		int nTrans;

		if (y1 < m_fHeightLower)
		{
			nTrans = 1;
			fLocalSafeDist = -fLocalSafeDist;
		}
		else
			nTrans = 0;

		if ( ( (m_Attrs & ATTR_CCW) && (m_Attrs & ATTR_INVERSE)) ||
			 ( !(m_Attrs & ATTR_CCW) && !(m_Attrs & ATTR_INVERSE)) )
		{
			nTrans = (nTrans ? 0 : 1);
		}

		pVWID->put_Transition(nTrans);

		if (!(m_Attrs & ATTR_PASSABLE))
		{
			if (FAILED(CreateVector(m_pWorld, x2, m_fHeightLower+fLocalSafeDist, z2, &pvProjection)))
				goto ERROR_ENCOUNTERED;
			pVWID->put_Projection(pvProjection);
		}
		if (!(*ppIDataList))
		{
			// We need to create the list.
			if(FAILED(CreatePropertyList(m_pWorld, ppIDataList)))
				goto ERROR_ENCOUNTERED;
		}
		// Save the intersect data for this intersect in the appropriate list position.
		if (FAILED(PutIDataInSortedList(*ppIDataList, pVWID)))
			goto ERROR_ENCOUNTERED;
		uHits++;
	}
	else
		goto ERROR_ENCOUNTERED;

ERROR_ENCOUNTERED:

	SAFERELEASE(pVWID);
	SAFERELEASE(pvProjection);

	return uHits;
}


// Function: CheckCrossing
//    This function does the other requisite dot products and compares to
//  see if two line segments intersect.  It is called by Intersect.  Currently,
//  it stores all intersections in ppIDataList. Returns TRUE if an intersection is found.
//
BOOL 
CBoundary::CheckCrossing(IPropertyList *pList, int iBoundaryIndex, VIndex nSrc, VIndex nDst, const Point3 &p1, const Point3 &p2, BOOL bTransition, IPropertyList **ppIDataList) 
{
	ASSERT(INARRAY(nSrc, cEdges));
	ASSERT(INARRAY(nDst, cEdges));
	// REVIEW: OPT - we might check for seg-seg bounding box intersection

	IVWIntersectData	*pVWID = NULL;
	IVector				*pvProjection = NULL;
	Point2				p2D1(p1.X(),p1.Z()), p2D2(p2.X(),p2.Z());
	Segment2			seg(p2D1, p2D2);
	BYTE				bPassed = eHit;
	BOOL				bNewTrans;

	const Point2 &srcVert = rgpntVerts[nSrc];		// since we are traversing in reverse order
	const Point2 &dstVert = rgpntVerts[nDst];

	// compute normal:  N = (src.y - dst.y, dst.x - src.x)
	const CoVector2 N(srcVert.Y() - dstVert.Y(), dstVert.X() - srcVert.X());
	
	float fY, fFraction;
	float E1, E2;
	E1 = N * (seg.GetSrc() - srcVert);
	E2 = N * (seg.GetDst() - srcVert);

	// if points are on same side - no intersection, o.w. fall thru 
	if (E1 < 0.0f) {
		if (E2 < 0.0f) return FALSE;
		E1 = -E1;		// moved absolute value stuff up here
	} else {
		if (E2 >= 0.0f) return FALSE;
		E2 = -E2;		// moved absolute value stuff up here
	}

	// Create the intersect data list if necessary.
	if (!(*ppIDataList))
	{
		if(FAILED(CreatePropertyList(m_pWorld, ppIDataList)))
			goto ERROR_ENCOUNTERED;
	}

	// Create the necessary intersect data for this intersect.
	pVWID = new CComIntersectData;
	if (!pVWID)
		goto ERROR_ENCOUNTERED;
	pVWID->AddRef();
	pVWID->put_BoundaryList(pList);
	pVWID->put_BoundaryIndex(iBoundaryIndex);
	pVWID->put_EdgeIndex(nSrc);

	if ( ( (m_Attrs & ATTR_CCW) && (m_Attrs & ATTR_INVERSE)) ||
		 ( !(m_Attrs & ATTR_CCW) && !(m_Attrs & ATTR_INVERSE)) )
		bNewTrans = (bTransition ? FALSE : TRUE);
	else
		bNewTrans = (bTransition ? TRUE : FALSE);

	pVWID->put_Transition(bNewTrans);

	fFraction = E1/*numerator*/ / (E1+E2)/*denomenator*/;
	fY = (p2.Y()-p1.Y())*fFraction;
	pVWID->put_DistToIntersect(fFraction);
	if ((m_fHeightUpper != m_fHeightLower))
	{
		if ((p1.Y()+fY) > m_fHeightUpper)
			bPassed = ePassedOver;
		else if ((p1.Y()+fY) < m_fHeightLower)
			bPassed = ePassedUnder;
	}
	pVWID->put_Passed(bPassed);

	// If we have hit a hard surface, we may need a projected segment vector.
	if (bPassed == eHit && !(m_Attrs & ATTR_PASSABLE))
	{
		D3DVECTOR	d3dvSeg = {p2.X() - p1.X(), 0.0F, p2.Z() - p1.Z()},
					d3dvVerts = {dstVert.X() - srcVert.X(), 0.0F, dstVert.Y() - srcVert.Y()},
					d3dvIntersection, d3dvProj, d3dvPerp, d3dvAxis = {0.0F, 1.0F, 0.0F}, d3dvCross;
		float		fXHyp, fYHyp, fHyp, fCosTheta, fAdj;

		// We want to pass back the projection point.
		// This point is defined as the point on the edge (Src-Dst) at which 
		// a perpendicular line to that boundary would intersect the end point (p2) of the 
		// intersecting vector (p1-p2). All is considered in X-Z plane first.

		// First need the distance from point of intersection to p2.
		fXHyp = (p2.X()-p1.X()) * (1.0F-fFraction);
		fYHyp = (p2.Z()-p1.Z()) * (1.0F-fFraction);
		fHyp = (float)sqrt(pow(fXHyp, 2) + pow(fYHyp, 2));
		
		// Next need the angle between the intersecting vector and the collision boundary.
		// We will use the dot product.
		D3DRMVectorNormalize(&d3dvSeg);
		D3DRMVectorNormalize(&d3dvVerts);
		fCosTheta = D3DRMVectorDotProduct(&d3dvSeg, &d3dvVerts);

		// Then the distance between the point of intersection and the point we want.
		fAdj = fHyp * fCosTheta;

		// Finally we must add this distance to the  point of intersection in the 
		// correct direction along the edge to get our point.
		d3dvIntersection.x = p1.X() + (p2.X()-p1.X()) * fFraction;
		d3dvIntersection.z = p1.Z() + (p2.Z()-p1.Z()) * fFraction;
		d3dvProj.x = d3dvIntersection.x + fAdj*d3dvVerts.x;
		d3dvProj.z = d3dvIntersection.z + fAdj*d3dvVerts.z;

		// Ah, but want to keep you away from the wall.
		// Need a vector perpendicular to Verts.
		D3DRMVectorRotate(&d3dvPerp, &d3dvVerts, &d3dvAxis, PI/2.0F);
		D3DRMVectorCrossProduct(&d3dvCross, &d3dvSeg, &d3dvVerts);
		if (d3dvCross.y < 0.0F)
		{
			d3dvProj.x -= d3dvPerp.x*fSafeDist;
			d3dvProj.z -= d3dvPerp.z*fSafeDist;
		}
		else
		{
			d3dvProj.x += d3dvPerp.x*fSafeDist;
			d3dvProj.z += d3dvPerp.z*fSafeDist;
		}

		// Put in the Y component.
		d3dvProj.y = p2.Y();

		// Finally return the vector.
		if (FAILED(CreateVector(m_pWorld, d3dvProj.x, d3dvProj.y, d3dvProj.z, &pvProjection)))
			goto ERROR_ENCOUNTERED;
		pVWID->put_Projection(pvProjection);
	}

	// Save the intersect data for this intersect in the appropriate list position.
	if (FAILED(PutIDataInSortedList(*ppIDataList, pVWID)))
		goto ERROR_ENCOUNTERED;
		
	// CONDITIONAL STORE INTERSECTION
	// get absolute values of dot products
//	SETABS(E1);
//	SETABS(E2);
//	float Esum = E1 + E2;

	// - check if new intersection is closer than old one using "no division" trick
	// - notice that WITHOUT the divide, this is more numerically sound (no singularities, etc.)
//	if (E1 * pHit->den < Esum * pHit->num)
//		pHit->Set(E1, Esum, this, nSrc);

ERROR_ENCOUNTERED:
	
	SAFERELEASE(pVWID);
	SAFERELEASE(pvProjection);

	return TRUE;
}

HRESULT CBoundary::PutIDataInSortedList(IPropertyList *pIDataListSorted, IVWIntersectData *pVWIData)
{
	long				lIDataCount;
	int					i, j;
	float				fDist, fPrevDist;
	IVWIntersectData	*pPrev = NULL;
	IPropertyList		*pPrevList = NULL;
	IBoundary			*pPrevBoundary = NULL;
	int					iPrevBoundaryIndex, iPrevEdgeIndex;
	VARIANT_BOOL		bPrevPassable;
	BYTE				bPassed, bPrevPassed;
	HRESULT				hr = S_OK;

	// Extract the intersect data.
	pVWIData->get_DistToIntersect(&fDist);
	pVWIData->get_Passed(&bPassed);

	if (FAILED(hr = pIDataListSorted->get_Count(&lIDataCount)))
		goto ERROR_ENCOUNTERED;

	// Place the intersect data correctly in the list.
	if (lIDataCount > 0)
	{	
		for (i = 0; i < lIDataCount; i++)
		{
			// Need to sort this edge by comparing distances to intersections.
			if (FAILED(hr = pIDataListSorted->get_ObjectProperty(i, (IObjectProperty**)&pPrev)))
				goto ERROR_ENCOUNTERED;
			pPrev->get_DistToIntersect(&fPrevDist);
			pPrev->get_BoundaryList(&pPrevList);
			pPrev->get_BoundaryIndex(&iPrevBoundaryIndex);
			pPrevList->get_ObjectProperty(iPrevBoundaryIndex, (IObjectProperty**) &pPrevBoundary);
			pPrev->get_EdgeIndex(&iPrevEdgeIndex);
			pPrev->get_Passed(&bPrevPassed);
			pPrevBoundary->get_IsPassable(&bPrevPassable);

			if (fPrevDist > fDist)
			{
				// Place intersect data in list in order of increasing distance to 
				// intersections.
				if (FAILED(hr = pIDataListSorted->InsertObjectPropertyAt(i, pVWIData)))
					goto ERROR_ENCOUNTERED;

				if (!(m_Attrs & ATTR_PASSABLE) && !bPassed)
				{
					// I have placed an impassable intersection in the list, all intersections
					// after this point should now be removed.
					for (j = lIDataCount; j > i; j--)
					{
						if (FAILED(hr = pIDataListSorted->RemoveAt(j)))
							goto ERROR_ENCOUNTERED;
					}
				}
				// Don't search list any further!
				goto ERROR_ENCOUNTERED;
			}
			else
			{
				if (i == lIDataCount-1)
				{
					// Place intersect data at the end of the list, but only if it's not
					// after an impassable intersection.
					if (bPrevPassable || bPrevPassed)
					{
						if (FAILED(hr = pIDataListSorted->AddObjectProperty(pVWIData)))
							goto ERROR_ENCOUNTERED;
					}
				}
					// Don't store passable edges in list after an impasssable edge.
			}
			SAFERELEASE(pPrev);
			SAFERELEASE(pPrevList);
			SAFERELEASE(pPrevBoundary);
		}
	}
	else
	{
		// List is empty.
		if (FAILED(hr = pIDataListSorted->AddObjectProperty(pVWIData)))
			goto ERROR_ENCOUNTERED;
	}

ERROR_ENCOUNTERED:

	SAFERELEASE(pPrev);
	SAFERELEASE(pPrevList);
	SAFERELEASE(pPrevBoundary);

	return hr;
}

#define STATE_I			2
#define STATE_II		3
#define STATE_III		1
#define STATE_IV		0
#define STATE_I_III		((STATE_I << 2) | STATE_III)
#define STATE_I_IV		((STATE_I << 2) | STATE_IV)
#define STATE_II_IV		((STATE_II << 2) | STATE_IV)
#define STATE_III_I		((STATE_III << 2) | STATE_I)
#define STATE_IV_II		((STATE_IV << 2) | STATE_II)
#define STATE_IV_I		((STATE_IV << 2) | STATE_I)


// Function: IsWithinEx
//     Return if the given point is inside the Boundary, taking into account the directedness
//  of the Boundary.  For example, if the point is outside the polygon defined by the Boundary
//  vertices, but the cycle is clockwise, the point is considered INSIDE.
//
// ALGORITHM:
//     We can compute if the point is inside by checking the number of crossings the Boundary
//  edges have with a horizontal line extending to the right of the point to the outside of
//  the Boundary.  The crossings are considered directed, so that if the horizontal line
//  approaches the segment the left, it is a positive crossing, and a negative crossing
//  if from the left.  If the Crossing Number is positive, the point is inside.  If the
//  Crossing Number is negative, the point is actually within the bounds of the polygon, but
//  considered outside because the Boundary is CLOCKWISE.  If the point is found to be
//  outside the polygon (that is, its Crossing Number is 0), it is INSIDE if the Boundary 
//  is CLOCKWISE, otherwise OUTSIDE.
//     The Crossing Number is computed by categorizing each of the points of the Boundary 
//  w.r.t. the four quadrants associated with the pnt (as the origin of a coordinate
//  system).  That is, a point above and to the left of the pnt is in Quadrant II.  Now
//  there are only a handful of interesting cases where the edges can intersect the
//  horizontal segment.  Edges joining points in: Quadrants I & IV always cause a crossing,
//  and Quadrants II & IV, or I & III may cause a crossing if the edge is to the right of
//  the point (which can be computed with a CCW kind of dot product).
//     The point categorization is computed incrementally as the program steps thru the
//  Boundary vertices, and is implicitly stored in the states of a finite state machine.
//  Crossings are computed at appropriate transitions of the FSM.
//
STDMETHODIMP CBoundary::IsWithinExt(Point2 *pnt, VARIANT_BOOL *pbInside)
{
	// Need at least 3 vertices/edges to be a valid boundary (protects against "bad" boundaries created in script)
	if (cEdges < 3)
		return VWMM_E_INVALIDBOUNDARY;

	int nHits = 0;

	if (bbox.IsInside(*pnt)) 
	{
		// we are in the bounding box of the Boundary

		EIndex iVert = 0;
		
		const float flPntX = pnt->vecCoord.x;
		const float flPntY = pnt->vecCoord.y;
		const Point2 *ppntCurVert = rgpntVerts;
		float flVertX = ppntCurVert->vecCoord.x;
		float flVertY = ppntCurVert->vecCoord.y;
		
#define SET_PNTS() ppntCurVert = rgpntVerts + iVert; flVertX = ppntCurVert->vecCoord.x; flVertY = ppntCurVert->vecCoord.y;
#define SET_STATENUM() (uLastState = (((flVertY >= flPntY) << 1) | (flVertX < flPntX)))
		
		unsigned int uLastState;
		unsigned int uFirstState = (((flVertY >= flPntY) << 1) | (flVertX < flPntX));
		
		switch (uFirstState) {
		case STATE_II: goto state_II;
		case STATE_III: goto state_III;
		case STATE_IV: goto state_IV;
		}
		// otherwise fall thru to state_I
		
state_I:
		if (++iVert == cEdges) goto state_end;
		SET_PNTS();
		
		switch (SET_STATENUM()) {
		case STATE_I:		goto state_I;
		case STATE_II:		goto state_II;
		case STATE_IV:
			nHits--;
			goto state_IV;
		}
		// goto state_III
		nHits -= (((rgpntVerts[iVert-1].vecCoord.y - flVertY) * (flPntX - flVertX)) <  
			-((flVertX - rgpntVerts[iVert-1].vecCoord.x) * (flPntY - flVertY)));
		
state_III:
		if (++iVert == cEdges) goto state_end;
		SET_PNTS();
		
		switch (SET_STATENUM()) {
		case STATE_III:		goto state_III;
		case STATE_II:		goto state_II;
		case STATE_I:
			nHits += (((rgpntVerts[iVert-1].vecCoord.y - flVertY) * (flPntX - flVertX)) >=
				-((flVertX - rgpntVerts[iVert-1].vecCoord.x) * (flPntY - flVertY)));
			goto state_I;
		}
		// goto state_IV
		
state_IV:
		if (++iVert == cEdges) goto state_end;
		SET_PNTS();
		
		switch (SET_STATENUM()) {
		case STATE_IV:		goto state_IV;
		case STATE_III:		goto state_III;
		case STATE_I:
			nHits++;
			goto state_I;
		}
		// goto state_II
		nHits += (((rgpntVerts[iVert-1].vecCoord.y - flVertY) * (flPntX - flVertX)) >=
			-((flVertX - rgpntVerts[iVert-1].vecCoord.x) * (flPntY - flVertY)));
		
state_II:
		if (++iVert == cEdges) goto state_end;
		SET_PNTS();
		
		switch (SET_STATENUM()) {
		case STATE_II:		goto state_II;
		case STATE_III:		goto state_III;
		case STATE_I:		goto state_I;
		}
		// goto state_IV
		nHits -= (((rgpntVerts[iVert-1].vecCoord.y - flVertY) * (flPntX - flVertX)) <  
			-((flVertX - rgpntVerts[iVert-1].vecCoord.x) * (flPntY - flVertY)));
		goto state_IV;
		
state_end:
		switch ((uLastState << 2) | uFirstState) {
		case STATE_I_IV:	nHits--; break;
		case STATE_IV_I:	nHits++; break;
		case STATE_I_III:	
		case STATE_II_IV:
			nHits -= (((flVertY - rgpntVerts[0].vecCoord.y) * (flPntX - rgpntVerts[0].vecCoord.x)) <
				-((rgpntVerts[0].vecCoord.x - flVertX) * (flPntY - rgpntVerts[0].vecCoord.y)));
			break;
		case STATE_III_I:
		case STATE_IV_II:
			nHits += (((flVertY - rgpntVerts[0].vecCoord.y) * (flPntX - rgpntVerts[0].vecCoord.x)) >=
				-((rgpntVerts[0].vecCoord.x - flVertX) * (flPntY - rgpntVerts[0].vecCoord.y)));
			break;
		}
	}

//	*pbInside = ((nHits > 0) || ((nHits == 0) && IsCW()));

	*pbInside = (nHits != 0);
	*pbInside = ( (m_Attrs & ATTR_INVERSE) ? !(*pbInside) : *pbInside );

	return S_OK;
}

// Function: IsWithin
//   A script callable version of IsWithinEx.
STDMETHODIMP CBoundary::IsWithin(float fX, float fY, VARIANT_BOOL *pbInside)
{
	Point2 pnt(fX, fY);

	ASSERT(pbInside);
	if (!pbInside)
	{
		return E_POINTER;		
	}

	return IsWithinExt(&pnt, pbInside);
}

#pragma optimize( "", off )

// Function: Set
//    This function is used by the constructors.  It is careful to verify the structure
//  imposed by the specification for Boundarys.  Namely, it checks to make sure the
//  vertices form a SIMPLE polygon.
//
STDMETHODIMP CBoundary::Set(int nEdges, Point2 *trgpntVerts, unsigned char *trgEAttrs)
{
	HRESULT hr = S_OK;
	EIndex i;
	
	// Boundarys are guaranteed to have at least 1 vert
	ASSERT(nEdges > 0);
	ASSERT(trgpntVerts);
	ASSERT(trgEAttrs);

	cEdges = nEdges;

	if (rgEAttrs) delete [] rgEAttrs;
	if (rgpntVerts) delete [] rgpntVerts;

	// allocate space for vertices
	MMCHECK_MEM(rgpntVerts = new Point2[cEdges]);
	MMCHECK_MEM(rgEAttrs = new unsigned char[cEdges]);

	// copy the vertices and edge attributes
	for (i = 0; i < cEdges; i++) 
	{
		rgEAttrs[i] = trgEAttrs[i];
		rgpntVerts[i] = trgpntVerts[i];
	}

	if (!Compile()) 
		hr = E_FAIL;

	return hr;
}
#pragma optimize( "", on )

STDMETHODIMP CBoundary::put_HeightUpper(float fHeightUpper)
{
	if ((fHeightUpper <= m_fHeightLower) && (m_fHeightUpper != m_fHeightLower))
		return E_INVALIDARG;

	m_fHeightUpper = fHeightUpper;

	// ok, if we are replacing the initialized values, we can have just set them to be
	// invalid, fix that up
	if (m_fHeightUpper <= m_fHeightLower)
		m_fHeightLower = m_fHeightUpper - 0.0001F;

	return S_OK;
}

STDMETHODIMP CBoundary::get_HeightUpper(float* fHeightUpper)
{
	*fHeightUpper = m_fHeightUpper;
	return S_OK;
}

STDMETHODIMP CBoundary::put_HeightLower(float fHeightLower)
{
	if ((m_fHeightUpper <= fHeightLower) && (m_fHeightUpper != m_fHeightLower))
		return E_INVALIDARG;

	m_fHeightLower = fHeightLower;

	// ok, if we are replacing the initialized values, we can have just set them to be
	// invalid, fix that up
	if (m_fHeightUpper <= m_fHeightLower)
		m_fHeightUpper = m_fHeightLower + 0.0001F;

	return S_OK;
}

STDMETHODIMP CBoundary::get_HeightLower(float* fHeightLower)
{
	*fHeightLower = m_fHeightLower;
	return S_OK;
}

STDMETHODIMP CBoundary::put_IsPassable(VARIANT_BOOL bPassable)
{
	if (bPassable)
		m_Attrs |= ATTR_PASSABLE;
	else
		m_Attrs &= ~ATTR_PASSABLE;
	return S_OK;
}

STDMETHODIMP CBoundary::get_IsPassable(VARIANT_BOOL* pbPassable)
{
	*pbPassable = (m_Attrs & ATTR_PASSABLE) ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CBoundary::put_IsProximity(VARIANT_BOOL bProximity)
{
	if (bProximity)
		m_Attrs |= ATTR_PROXIMITY;
	else
		m_Attrs &= ~ATTR_PROXIMITY;
	return S_OK;
}

STDMETHODIMP CBoundary::get_IsProximity(VARIANT_BOOL* pbProximity)
{
	*pbProximity = (m_Attrs & ATTR_PROXIMITY) ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CBoundary::put_IsInverse(VARIANT_BOOL bInverse)
{
	if (bInverse)
		m_Attrs |= ATTR_INVERSE;
	else
		m_Attrs &= ~ATTR_INVERSE;
	return S_OK;
}


STDMETHODIMP CBoundary::get_IsInverse(VARIANT_BOOL* pbInverse)
{
	*pbInverse = (m_Attrs & ATTR_INVERSE) ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

// Function: GetLeftBottom
//    Find the leftmost vertex, preferring the bottommost vertex to break ties.
EIndex
CBoundary::GetLeftBottom(const Point2 *rgpntVerts, int cEdges)
{
	EIndex nLeftmost = 0;

	for (EIndex i = 1; i < cEdges; i++) {
		if (rgpntVerts[i].X() < rgpntVerts[nLeftmost].X()) {
			nLeftmost = i;
		} else if (rgpntVerts[i].X() == rgpntVerts[nLeftmost].X()) {
			if (rgpntVerts[i].Y() < rgpntVerts[nLeftmost].Y()) {
				nLeftmost = i;
			}
		}
	}
	return nLeftmost;
}

// Function: IsSimplePoly
//    Check if the given Boundary forms a simple polygon (i.e. is non-self-
//  intersecting).
// ALGORITHM: obvious O(n^2) all pairs checking.
//    There is a divide and conquer algorithm that would behave heuristically
//  better than this one, tho it is also O(n^2).  However, n is almost always
//  very small, so it is largely irrelevent.
BOOL CBoundary::IsSimplePoly(const Point2 *rgpntVerts, int cEdges, BOOL bClosed)
{
	if (!bClosed)
	{
		if (cEdges > 3) 
		{
			VIndex i, iPrev, j;

			for (i = 3, iPrev = 2; i < cEdges; i++, iPrev++) 
			{
				j = 1;

				for (; j < iPrev; j++)
					if (Segment2(rgpntVerts[iPrev], rgpntVerts[i]).FullIntersect(Segment2(rgpntVerts[j-1], rgpntVerts[j])))
						return FALSE;
			}
		}
	}
	else if (cEdges > 3) 
	{
		VIndex i, iPrev, j;
		for (i = 3, iPrev = 2; iPrev < cEdges; i++, iPrev++) 
		{
			if (i == cEdges) 
			{
				i = 0;
				j = 2;
			}
			else j = 1;

			for (; j < iPrev; j++)
			{
				if (Segment2(rgpntVerts[iPrev], rgpntVerts[i]).FullIntersect(Segment2(rgpntVerts[j-1], rgpntVerts[j])))
					return FALSE;
			}
		}
	}
	else if (cEdges < 3)
		return FALSE;

	//If we get through everything above we're simple.
	return TRUE;
}

BOOL
CBoundary::IsValid(const Point2 *rgpntVerts, int cEdges, BOOL bClosed)
{
	return IsSimplePoly(rgpntVerts, cEdges, bClosed);
}

// Function: IsConvex
//    Determine if the given cycle of edges is "convex" - that is, given
//  any two points that are considered inside the Boundary, is there a
//  straight line connecting them that does not intersect any edges.  Notice
//  this means that no finite CLOCKWISE Boundary can be considered convex.
BOOL
CBoundary::IsConvex(const Point2 *rgpntVerts, int cEdges)
{
	if (cEdges > 2) 
	{
		BOOL bConvex = ::IsCCW(rgpntVerts[cEdges-1], rgpntVerts[0], rgpntVerts[1]);

		for (EIndex i = 1; bConvex && (i < cEdges - 1); i++) {
			bConvex = ::IsCCW(rgpntVerts[i-1], rgpntVerts[i], rgpntVerts[i+1]);
		}
		return bConvex && ::IsCCW(rgpntVerts[i-1], rgpntVerts[i], rgpntVerts[0]);
	}
	// just so i'm not dependent on 2 vertex cycle polarity (CW or CCW)...
	return IsCCW(rgpntVerts, cEdges);
}

// Function: IsConvexPoly
//    Determine if the given Boundary is in the shape of a convex polygon.
//  Two vertex degenerate polygons are considered convex.
BOOL
CBoundary::IsConvexPoly(const Point2 *rgpntVerts, int cEdges, BOOL bCCW) {
	if (cEdges > 2)
	{
		BOOL bConvex = !(::IsCCW(rgpntVerts[cEdges-1], rgpntVerts[0], rgpntVerts[1]) ^ bCCW);

		for (EIndex i = 1; bConvex && (i < cEdges - 1); i++) {
			bConvex = !(::IsCCW(rgpntVerts[i-1], rgpntVerts[i], rgpntVerts[i+1]) ^ bCCW);
		}

		return bConvex && !(::IsCCW(rgpntVerts[i-1], rgpntVerts[i], rgpntVerts[0]) ^ bCCW);
	} 
	return TRUE;
}

// Function: IsCCW
//    Compute if the cycle is counterclockwise.
// ALGORITHM: Find the leftmost (bottommost) vertex and its two 
//     adjacent vertices and compute if these three points are CCW.
// Note: 2 vertex Boundarys are assumed CLOCKWISE.
// REVIEW:  what happens when the vertices of the Boundary are linear?
BOOL
CBoundary::IsCCW(const Point2 *rgpntVerts, int cEdges)
{
	if (cEdges > 2)
	{
		EIndex nLeftmost = GetLeftBottom(rgpntVerts, cEdges);

		EIndex nNext = (nLeftmost == (cEdges - 1) ? 0 : nLeftmost + 1);
		EIndex nPrev = (nLeftmost == 0 ? cEdges - 1 : nLeftmost - 1);

		return ::IsCCW(rgpntVerts[nPrev], rgpntVerts[nLeftmost], rgpntVerts[nNext]);
	}
	return FALSE;
}

// Function: Compile
//    This function updates optimization structures (such as bounding box)
//  after data has been changed in the Boundary.  We also verify the correctness
//  of the data.
//
#pragma optimize( "", off )
BOOL CBoundary::Compile() 
{
	ASSERT(rgpntVerts);

	if (cEdges < 2)
		return FALSE;

	//  verify that the polygon is valid
	BOOL bValid = IsValid(rgpntVerts, cEdges, VARIANT_FALSE);  //Could be building.

	// compute the 2D bounding box
	bbox.InsFirst(rgpntVerts[0]);

	for (EIndex i = 1; i < cEdges; i++) 
		bbox.InsNext(rgpntVerts[i]);

	// fill in the compile bit vector flags
	if (IsCCW(rgpntVerts, cEdges)) 
		m_Attrs |= ATTR_CCW;
	else
		m_Attrs &= ~ATTR_CCW;

	if (IsConvexPoly(rgpntVerts, cEdges, IsCCW()))
		m_Attrs |= ATTR_CONVEXPOLY;
	else
		m_Attrs &= ~ATTR_CONVEXPOLY;

	return bValid;
}
#pragma optimize( "", on )

// Function: SetIncomple
//    This function is used while building the Boundary before it is finished.
//
STDMETHODIMP CBoundary::SetIncomplete(int nEdges, Point2 *trgpntVerts, unsigned char *trgEAttrs)
{
	HRESULT hr = S_OK;
	EIndex i;
	
	// Boundarys are guaranteed to have at least 2 verts
	ASSERT(nEdges > 1);
	ASSERT(trgpntVerts);
	ASSERT(trgEAttrs);
	cEdges = nEdges;
	if (rgEAttrs) delete [] rgEAttrs;
	if (rgpntVerts) delete [] rgpntVerts;

	// allocate space for vertices
	MMCHECK_MEM(rgpntVerts = new Point2[cEdges]);
	MMCHECK_MEM(rgEAttrs = new unsigned char[cEdges]);

	// copy the vertices and edge attributes
	for (i = 0; i < cEdges; i++) {
		rgEAttrs[i] = trgEAttrs[i];
		rgpntVerts[i] = trgpntVerts[i];
	}
		// compute the 2D bounding box
	bbox.InsFirst(rgpntVerts[0]);
	for (i = 1; i < cEdges; i++) {
		bbox.InsNext(rgpntVerts[i]);
	}

	return hr;
}

STDMETHODIMP CBoundary::get_VertexCount(int* nEdges)
{
	if (nEdges == NULL)
		return E_POINTER;

	*nEdges = GetNEdges();

	return S_OK;
}

STDMETHODIMP CBoundary::GetVertexXY(int nEdge, VARIANT* pvarfX, VARIANT* pvarfY)
{
	HRESULT hr = S_OK;
	
	float fX, fY;

	ASSERT(pvarfX);
	ASSERT(pvarfY);
	if (!pvarfX || !pvarfY)
	{
		hr = E_POINTER;
		goto ERROR_ENCOUNTERED;
	}

	VariantClear(pvarfX);
	VariantClear(pvarfY);

	hr = GetVertexXYExt(nEdge, &fX, &fY );
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	pvarfX->vt = VT_R4;
	pvarfX->fltVal = fX;
	pvarfY->vt = VT_R4;
	pvarfY->fltVal = fY;

ERROR_ENCOUNTERED:
	return hr;
}

STDMETHODIMP CBoundary::GetVertexXYExt(int nEdge, float* pfX, float* pfY)
{
	HRESULT hr = S_OK;

	ASSERT(pfX);
	ASSERT(pfY);

	if ((nEdge >= cEdges) || (nEdge < 0 ))
	{
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	*pfX = rgpntVerts[nEdge].X();
	*pfY = rgpntVerts[nEdge].Y();

ERROR_ENCOUNTERED:
	return hr;
}


/////////////////////////////
// Boundary builder FUNCTIONS
/////////////////////////////


STDMETHODIMP CBoundary::SetVertexXY(int nEdge, float fX, float fY)
{
	HRESULT hr = S_OK;

	if ((nEdge >= cEdges) || (nEdge < 0 ))
	{
		hr = E_INVALIDARG;
		goto ERROR_ENCOUNTERED;
	}

	rgpntVerts[nEdge].vecCoord.x = fX;
	rgpntVerts[nEdge].vecCoord.y = fY;

ERROR_ENCOUNTERED:
	return hr;
}

// Translate every vertex by the delta between the incoming X and Y
// and the 0th X and Y.
STDMETHODIMP CBoundary::Translate(float flX, float flY)
{
	HRESULT hr = S_OK;
	VIndex i;

	if (cEdges < 1)
	{
		hr = E_FAIL;
		goto EXIT_FAIL;
	}

	for (i = 0; i < cEdges; i++) 
	{
		rgpntVerts[i].vecCoord.x += flX;
		rgpntVerts[i].vecCoord.y += flY;
	}

	if (!Compile( ))
		hr = E_FAIL;

EXIT_FAIL:
	return hr;
}

/*
// Translate every vertex by the delta between the incoming X and Y
// and the 0th X and Y.
STDMETHODIMP CBoundary::TranslateVertex(int nIndex, float flX, float flY)
{
	HRESULT hr = S_OK;

	if (nIndex > cEdges )
	{
		hr = E_FAIL;
		goto EXIT_FAIL;
	}

	rgpntVerts[nIndex].vecCoord.x += flX;
	rgpntVerts[nIndex].vecCoord.y += flY;

	if (!Compile( ))
		hr = E_FAIL;

EXIT_FAIL:
	return hr;
}
*/

// Translate every vertex by the delta between the incoming X and Y
// and the 0th X and Y.
STDMETHODIMP CBoundary::Rotate(float fAngle)
{
	HRESULT hr = S_OK;
	float fTmp, fCenterX, fCenterY;
	int i;
	IVector *pVectorStart = NULL, *pVectorRotated = NULL;

	if (cEdges < 1)
	{
		hr = E_FAIL;
		goto EXIT_FAIL;
	}

	hr = CoCreateInstance(CLSID_Vector, NULL, CLSCTX_INPROC_SERVER, IID_IVector, (LPVOID*)&pVectorStart);
	if (FAILED(hr) || !pVectorStart) 
		goto EXIT_FAIL;

	hr = CoCreateInstance(CLSID_Vector, NULL, CLSCTX_INPROC_SERVER, IID_IVector, (LPVOID*)&pVectorRotated);
	if (FAILED(hr) || !pVectorRotated) 
		goto EXIT_FAIL;

	GetCenterExt(&fCenterX, &fCenterY);

	for (i = 0; i < cEdges; i++) 
	{
		rgpntVerts[i].vecCoord.x -= fCenterX;
		rgpntVerts[i].vecCoord.y -= fCenterY;

		hr = pVectorStart->set(rgpntVerts[i].vecCoord.x, 0.0, rgpntVerts[i].vecCoord.y);
		if (FAILED(hr)) goto EXIT_FAIL;

		hr = pVectorStart->Rotate(fAngle, pVectorRotated);
		if (FAILED(hr)) goto EXIT_FAIL;
	
		pVectorRotated->get(&rgpntVerts[i].vecCoord.x, &fTmp, &rgpntVerts[i].vecCoord.y);

		rgpntVerts[i].vecCoord.x += fCenterX;
		rgpntVerts[i].vecCoord.y += fCenterY;
	}

	if (!Compile( ))
		hr = E_FAIL;

EXIT_FAIL:
	SAFERELEASE(pVectorStart);
	SAFERELEASE(pVectorRotated);

	return hr;
}

// Translate every vertex by the delta between the incoming X and Y
// and the 0th X and Y.
STDMETHODIMP CBoundary::Scale(float flX, float flY)
{
	HRESULT hr = S_OK;
	VIndex i;
	float fCenterX, fCenterY;

	if (cEdges < 1)
	{
		hr = E_FAIL;
		goto EXIT_FAIL;
	}
	
	GetCenterExt(&fCenterX, &fCenterY);

	for (i = 0; i < cEdges; i++) 
	{
		rgpntVerts[i].vecCoord.x = (rgpntVerts[i].vecCoord.x - fCenterX) *  flX + fCenterX;
		rgpntVerts[i].vecCoord.y = (rgpntVerts[i].vecCoord.y - fCenterY) *  flY + fCenterY;
		//rgpntVerts[i].vecCoord.y *= flY;
	}

	if (!Compile( ))
		hr = E_FAIL;

EXIT_FAIL:
	return hr;
}

// Function: InsVert
//    Insert a vertex at the given index position so that the vertex
//  currently at that position and all later vertices are bumped up 1.
//  The edge attribute information associated with the split edge is
//  now associated with the edge formed by the new vertex and the
//  vertex formerly at that position.
//  Returns FALSE is boundary is not a simple polygon

STDMETHODIMP CBoundary::InsertVertex(long nIndex, float fX, float fY)
{
	Point2 *			tmpPntVerts;	// array of edge vertices
	unsigned char*		tmpEAttrs;	// array of pointers to edge attrs
	//BOOL bRetVal;
	int i;

	// if no index was specified, add to the end
	if (nIndex == NULL_VINDEX)
		nIndex = cEdges;

	// allocate space for vertices
	MMCHECK_MEM(tmpPntVerts = new Point2[cEdges + 1]);
	MMCHECK_MEM(tmpEAttrs = new unsigned char[cEdges + 1]);

	ASSERT(INRANGE(nIndex, 0, cEdges));

	// Setup the vertex and edge info
	for (i = cEdges; i != nIndex; i--) 
	{
		tmpPntVerts[i] = rgpntVerts[i-1];
		tmpEAttrs[i] = rgEAttrs[i-1];
	}

	tmpPntVerts[nIndex].X() = fX;
	tmpPntVerts[nIndex].Y() = fY;

	// Setup the vertex and edge info
	for (i = nIndex - 1; i >= 0; i--) 
	{
		tmpPntVerts[i] = rgpntVerts[i];
		tmpEAttrs[i] = rgEAttrs[i];
	}

	if (nIndex == 0) 
	{
		if (cEdges > 0) 
		{
			tmpEAttrs[0] = tmpEAttrs[cEdges-1];	// copy last to first
			tmpEAttrs[cEdges-1] = 0;				// set attributes to 0
		} 
		else 
			tmpEAttrs[0] = 0;
	} 
	else 
	{
		tmpEAttrs[nIndex] = tmpEAttrs[nIndex-1];
		tmpEAttrs[nIndex-1] = 0;
	}

	//bRetVal = IsSimplePoly(tmpPntVerts, cEdges + 1, m_bClosed);

	Set(cEdges + 1, tmpPntVerts, tmpEAttrs);

	delete [] tmpPntVerts;
	delete [] tmpEAttrs;

	return S_OK;
}

STDMETHODIMP CBoundary::InsertVertexSafe(long nIndex, float fX, float fY, VARIANT_BOOL* pBool)
{
	Point2 *			tmpPntVerts;	// array of edge vertices
	unsigned char*		tmpEAttrs;	// array of pointers to edge attrs
	BOOL bRetVal;
	int i;

	// if no index was specified, add to the end
	if (nIndex == NULL_VINDEX)
		nIndex = cEdges;

	// allocate space for vertices
	MMCHECK_MEM(tmpPntVerts = new Point2[cEdges + 1]);
	MMCHECK_MEM(tmpEAttrs = new unsigned char[cEdges + 1]);

	ASSERT(INRANGE(nIndex, 0, cEdges));

	// Setup the vertex and edge info
	for (i = cEdges; i != nIndex; i--) 
	{
		tmpPntVerts[i] = rgpntVerts[i-1];
		tmpEAttrs[i] = rgEAttrs[i-1];
	}

	tmpPntVerts[nIndex].X() = fX;
	tmpPntVerts[nIndex].Y() = fY;

	// Setup the vertex and edge info
	for (i = nIndex - 1; i >= 0; i--) 
	{
		tmpPntVerts[i] = rgpntVerts[i];
		tmpEAttrs[i] = rgEAttrs[i];
	}

	if (nIndex == 0) 
	{
		if (cEdges > 0) 
		{
			tmpEAttrs[0] = tmpEAttrs[cEdges-1];	// copy last to first
			tmpEAttrs[cEdges-1] = 0;				// set attributes to 0
		} 
		else 
			tmpEAttrs[0] = 0;
	} 
	else 
	{
		tmpEAttrs[nIndex] = tmpEAttrs[nIndex-1];
		tmpEAttrs[nIndex-1] = 0;
	}

	bRetVal = IsSimplePoly(tmpPntVerts, cEdges + 1, VARIANT_FALSE);  //False cause we might be building.

	if (bRetVal)
	{
		Set(cEdges + 1, tmpPntVerts, tmpEAttrs);
		*pBool = VARIANT_TRUE;
	}
	else 
	{
		*pBool = VARIANT_FALSE;
	}

	delete [] tmpPntVerts;
	delete [] tmpEAttrs;

	return S_OK;
}

// Function: MoveVertSafe
//    Tests to see if moving a vertex in the Boundary immediately violates
//  any Boundary restrictions.  If not, the vertex is moved and TRUE
//  is returned, otherwise the vertex is left unmoved and FALSE is returned.
STDMETHODIMP CBoundary::TranslateVertexSafe(long nIndex, float fX, float fY, VARIANT_BOOL* pBool)
{
	ASSERT(INARRAY(nIndex, cEdges));
	
	Point2 pntTemp = rgpntVerts[nIndex];

	rgpntVerts[nIndex].X() += fX;
	rgpntVerts[nIndex].Y() += fY;

	if (IsSimplePoly(rgpntVerts, cEdges, m_bClosed) )  //False cause we might be building.
	{
		*pBool = VARIANT_TRUE;
	}
	else 
	{
		rgpntVerts[nIndex] = pntTemp;
		*pBool = VARIANT_FALSE;

	}

	if (!Compile( ))
		return S_FALSE;

	return S_OK;
}

STDMETHODIMP CBoundary::TranslateVertex(long nIndex, float fX,  float fY)
{
	ASSERT(INARRAY(nIndex, cEdges));

	rgpntVerts[nIndex].X() += fX;
	rgpntVerts[nIndex].Y() += fY;

	if (!Compile( ))
		return E_FAIL;

	return S_OK;
}

#pragma optimize( "", off )

// Function: DelVert
//    Remove a vertex from the Boundary.  This should behave as the exact inverse
//  of InsVert.
//Turned optimization off because of release build compiler error.
STDMETHODIMP CBoundary::DeleteVertex(long nIndex)
{

	Point2 *			tmpPntVerts;	// array of edge vertices
	unsigned char*		tmpEAttrs;	// array of pointers to edge attrs
	int i;
	int nNewEdgeCount;

	if (nIndex == NULL_VINDEX)
		nIndex = cEdges - 1;

	ASSERT(INARRAY(nIndex, cEdges));

	//cEdges--;

	// allocate space for vertices
	nNewEdgeCount = cEdges - 1;
	MMCHECK_MEM(tmpPntVerts = new Point2[nNewEdgeCount]);
	MMCHECK_MEM(tmpEAttrs = new unsigned char[nNewEdgeCount ]);

	if (nIndex < nNewEdgeCount)
		tmpEAttrs[nIndex] = 0;

	// Copy the old points
	for (i = 0; i < nIndex; i++)
	{
		tmpPntVerts[i] = rgpntVerts[i];
	}

	// Except the new point
	for (i = nIndex; i < nNewEdgeCount; i++)
	{
		tmpPntVerts[i] = rgpntVerts[i+1];
		tmpEAttrs[i] = rgEAttrs[i+1];
	}

	Set(nNewEdgeCount, tmpPntVerts, tmpEAttrs);

	delete [] tmpPntVerts;
	delete [] tmpEAttrs;

	return S_OK;

}

STDMETHODIMP CBoundary::DeleteVertexSafe(long nIndex, VARIANT_BOOL *pBool)
{
	Point2 *			tmpPntVerts;	// array of edge vertices
	unsigned char*		tmpEAttrs;	// array of pointers to edge attrs
	BOOL bRetVal;
	int i;
	int nNewEdgeCount;

	if (!pBool)
		return E_POINTER;

	*pBool = VARIANT_FALSE;

	if (cEdges <= 0)
	{
		return E_FAIL;
	}

	if (nIndex == NULL_VINDEX)
		nIndex = cEdges - 1;

	ASSERT(INARRAY(nIndex, cEdges));

	//cEdges--;
	//MMCHECK_MEM(tmpPntVerts = new Point2[cEdges - 1]);
	//MMCHECK_MEM(tmpEAttrs = new unsigned char[cEdges - 1 ]);

	// allocate space for vertices
	nNewEdgeCount = cEdges - 1;
	tmpPntVerts = new Point2[nNewEdgeCount];
	tmpEAttrs = new unsigned char[nNewEdgeCount];

	if (nIndex < nNewEdgeCount)
		tmpEAttrs[nIndex] = 0;

	// Copy the old points
	for (i = 0; i < nIndex; i++)
	{
		tmpPntVerts[i] = rgpntVerts[i];
	}

	// Except the new point
	for (i = nIndex; i < nNewEdgeCount; i++)
	{
		tmpPntVerts[i] = rgpntVerts[i+1];
		tmpEAttrs[i] = rgEAttrs[i+1];
	}

	bRetVal = IsSimplePoly(tmpPntVerts, nNewEdgeCount, VARIANT_TRUE);  //False cause we might be building.

	if (bRetVal)
	{
		Set(nNewEdgeCount, tmpPntVerts, tmpEAttrs);
		*pBool = VARIANT_TRUE;
	}
	else 
	{
		*pBool = VARIANT_FALSE;
	}

	delete [] tmpPntVerts;
	delete [] tmpEAttrs;

	return S_OK;
}
#pragma optimize( "", on )

STDMETHODIMP CBoundary::get_IsClosed(VARIANT_BOOL* pvbClosed)
{
	HRESULT hr = S_OK;

	ASSERT(pvbClosed);
	if (NULL == pvbClosed)
		return E_POINTER;

	*pvbClosed = m_bClosed;

	return S_OK;
}


STDMETHODIMP CBoundary::put_IsClosed(VARIANT_BOOL vbClosed)
{
	HRESULT hr = S_OK;

	m_bClosed = vbClosed;

	return S_OK;
}

STDMETHODIMP CBoundary::AutoGenerate(IThing* pThing, int nSides, float fExtraRadius)
{
	HRESULT hr = S_OK;
	Point2 *			tmpPntVerts;	// array of edge vertices
	unsigned char*		tmpEAttrs;	// array of pointers to edge attrs
	int i;

		// allocate space for vertices
	MMCHECK_MEM(tmpPntVerts = new Point2[nSides]);
	MMCHECK_MEM(tmpEAttrs = new unsigned char[nSides ]);

	// Except the new point
	for (i = 0; i < nSides; i++)
		tmpEAttrs[i] = 0;

	hr = GeneratePoints(pThing, nSides, tmpPntVerts, fExtraRadius);
	if (FAILED(hr)) goto EXIT_FAIL;

	Set(nSides, tmpPntVerts, tmpEAttrs);

EXIT_FAIL:
	delete [] tmpPntVerts;
	delete [] tmpEAttrs;

	return hr;

}

HRESULT CBoundary::AutoGenerateFromBox(float fMinX, float fMinY, float fMinZ, float fMaxX, float fMaxY, float fMaxZ, int nSides, float fExtraRadius)
{
	HRESULT hr = S_OK;
	Point2 *			tmpPntVerts;	// array of edge vertices
	unsigned char*		tmpEAttrs;	// array of pointers to edge attrs
	int i;
	float fInc =  6.28318530718F / nSides;
	float fLenX, fLenZ;
	D3DVECTOR vecVector, vecAxis;

		// allocate space for vertices
	MMCHECK_MEM(tmpPntVerts = new Point2[nSides]);
	MMCHECK_MEM(tmpEAttrs = new unsigned char[nSides ]);

	// Except the new point
	for (i = 0; i < nSides; i++)
		tmpEAttrs[i] = 0;

	vecAxis.x = 0.0F;
	vecAxis.y = 1.0F;
	vecAxis.z = 0.0F;

	vecVector.x = 1.0F;
	vecVector.y = 0.0F;
	vecVector.z = 0.0F;

	//Calculate the radius.  Add an extra const for the clipping plane.
	fLenX = (float) fabs(fMaxX - fMinX) *  0.7071067811865F + 0.2f + fExtraRadius;
	fLenZ = (float) fabs(fMaxZ - fMinZ) *  0.7071067811865F + 0.2f + fExtraRadius;

	for (i = 0; i < nSides; i++)
	{
		//TRACE( "AutoGenerated vertex: %f, %f\n",vecVector.x * fLenX, vecVector.z * fLenZ );

		tmpPntVerts[i].vecCoord.x = vecVector.x * fLenX;
		tmpPntVerts[i].vecCoord.y = vecVector.z * fLenZ;
		
		D3DRMVectorRotate(&vecVector, &vecVector, &vecAxis, fInc);
	}

	Set(nSides, tmpPntVerts, tmpEAttrs);

	hr = put_HeightLower( (fMinY < fMaxY ? fMinY : fMaxY ) );
	if( FAILED(hr) ) goto EXIT_FAIL;

	hr = put_HeightUpper( (fMinY > fMaxY ? fMinY : fMaxY ) );
	if( FAILED(hr) ) goto EXIT_FAIL;

EXIT_FAIL:
	delete [] tmpPntVerts;
	delete [] tmpEAttrs;

	return hr;
}


HRESULT CBoundary::GeneratePoints(IThing* pThing, int nSides, Point2* pnts, float fExtraRadius)
{
	IVWFrame* pVWFrame = NULL;
	D3DRMBOX d3dBox;
	IVWGeometry* pGeometry = NULL;
	IVWGeometrySprite* pSprite = NULL;
	HRESULT hr = S_OK;
	int i;
	D3DVECTOR vecVector, vecAxis;
	float fInc =  6.28318530718F / nSides;
	float fLenX, fLenZ, fFloor, fCeiling;
	static CComBSTR cbstrGeometry("Geometry");
	VARIANT v1, v2, v3, v4, v5, v6;

	VariantInit(&v1); VariantInit(&v2); VariantInit(&v3); 
	VariantInit(&v4); VariantInit(&v5); VariantInit(&v6); 

	hr = pThing->get_ObjectProperty(cbstrGeometry, (IObjectProperty**) &pGeometry);
	if( FAILED(hr) || !pGeometry) goto EXIT_FAIL;

	hr = pGeometry->get_Frame(&pVWFrame);
	if (!pVWFrame)
		hr = VWMM_E_INVALIDBOUNDARY;
	if( FAILED(hr)) goto EXIT_FAIL;

	v1.vt = v2.vt = v3.vt = v4.vt = v5.vt = v6.vt = VT_R4;

	pVWFrame->GetBoundingBox(&v1, &v2, &v3, &v4, &v5, &v6);

	d3dBox.min.x = v1.fltVal;
	d3dBox.min.y = v2.fltVal;
	d3dBox.min.z = v3.fltVal;
	d3dBox.max.x = v4.fltVal;
	d3dBox.max.y = v5.fltVal;
	d3dBox.max.z = v6.fltVal;

	fFloor = d3dBox.min.y;
	fCeiling = (d3dBox.max.y == d3dBox.min.y ? d3dBox.max.y + 0.001f : d3dBox.max.y);

	vecAxis.x = 0.0F;
	vecAxis.y = 1.0F;
	vecAxis.z = 0.0F;

	vecVector.x = 1.0F;
	vecVector.y = 0.0F;
	vecVector.z = 0.0F;

	//Calculate the radius.  Add an extra const for the clipping plane.
	fLenX = (float) fabs(d3dBox.max.x - d3dBox.min.x) *  0.7071067811865F + 0.2f + fExtraRadius;
	fLenZ = (float) fabs(d3dBox.max.z - d3dBox.min.z) *  0.7071067811865F + 0.2f + fExtraRadius;

	for (i = 0; i < nSides; i++)
	{
		//TRACE( "AutoGenerated vertex: %f, %f\n",vecVector.x * fLenX, vecVector.z * fLenZ );

		pnts[i].vecCoord.x = vecVector.x * fLenX;
		pnts[i].vecCoord.y = vecVector.z * fLenZ;
		
		D3DRMVectorRotate(&vecVector, &vecVector, &vecAxis, fInc);
	}

	hr = put_HeightLower(fFloor);
	if( FAILED(hr) ) goto EXIT_FAIL;

	hr = put_HeightUpper(fCeiling);
	if( FAILED(hr) ) goto EXIT_FAIL;

EXIT_FAIL:
	SAFERELEASE(pSprite);
	SAFERELEASE(pGeometry);
	SAFERELEASE(pVWFrame);

	return hr;
}

STDMETHODIMP CBoundary::put_IsAutoSolidBoundary(VARIANT_BOOL bAutoSolid)
{
	if (bAutoSolid)
		m_Attrs |= ATTR_AUTOSOLIDBOUNDARY;
	else
		m_Attrs &= ~ATTR_AUTOSOLIDBOUNDARY;
	return S_OK;
}


STDMETHODIMP CBoundary::get_IsAutoSolidBoundary(VARIANT_BOOL* pbAutoSolid)
{
	ASSERT(pbAutoSolid);

	*pbAutoSolid = (m_Attrs & ATTR_AUTOSOLIDBOUNDARY) ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}
