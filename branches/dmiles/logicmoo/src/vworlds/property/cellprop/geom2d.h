// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _Geom2D_h
#define _Geom2D_h

// File:	Geom2D.h
// Author:	MM
//
// Description:
//     This file contains basic 2D geometry structures.
// 
// History:
// -@- 9/5/95 (mm) created
//    cannibalized from EdgeCycle.h 
// -@- 4/30/96 (mm)
//    changed Segment2 to record 2 points, rather than pointers
//    to points

#include "MMUtils.h"
#include "point23.h"

// Class:		BBox2
// Description:
//    BBox2 defines a class for axis aligned 2D bounding boxes.  The bounding box
//  is constructed by inserting points or other bounding boxes into the bound.
//  CAUTION: using the NULL constructor is dangerous.  You must be sure to call
//  InsFirst before inserting any points if the NULL constructor is used.
//
class BBox2 {
public:
			BBox2() {}
			BBox2(const Point2 &tPnt);
			BBox2(const Point2 &segSrc, const Point2 &segDst);		// common case: line segment
			BBox2(const BBox2 &box);
			~BBox2() {}
	//
	// CONSTRUCTION FUNCTIONS
	//
	void 	InsFirst(const Point2 &pnt);
	void 	InsFirst(const BBox2 &box);
	void	InsFirst(const Point2 &segSrc, const Point2 &segDst);
	void	InsNext(const Point2 &pnt);
	void	InsNext(const BBox2 &box);

	//
	// QUERY FUNCTIONS
	//
	BOOL 	Intersect(const BBox2 &box) const;
	BOOL	IsInside(const Point2 &pnt) const;

	// I/O INTERFACE
//	void	Print(FILE *fd) const;

	// Access functions added by Dougsha for Bootstrapping Cell editor.
	Point2 Get_minPnt(){ return minPnt; };
	Point2 Get_maxPnt(){ return maxPnt; };
private:
	Point2 	minPnt;
	Point2 	maxPnt;
};


// Class:		Segment2
// Description:
//    Defines a 2D line Segment class.  The segment normal points to the left.
//
class Segment2 {
public:
						Segment2(const Point2 &src, const Point2 &dst);
						~Segment2() {}

	// DATA ACCESS FUNCTIONS
	const CoVector2 &	GetNorm() const		{ return N; }
	const BBox2 &		GetBBox() const		{ return bbox; }
	const Point2 &		GetSrc() const		{ return rgPnt[0]; }
	const Point2 &		GetDst() const		{ return rgPnt[1]; }

	// QUERY FUNCTIONS
	BOOL				FullIntersect(const Segment2 &seg) const;
	BOOL				Intersect(const Segment2 &seg) const;
	BOOL				IsCCW(const Point2 &pnt) const;
	BOOL				IsCW(const Point2 &pnt) const;

	// I/O INTERFACE
//	void				Print(FILE *fd) const;

private:
	Point2			rgPnt[2];			// segment end points
	CoVector2		N;					// line normal
	BBox2			bbox;				// segment bounding box
};


//////////////////
// BBox2 FUNCTIONS
//////////////////

// Function: InsFirst
//    Use this to insert what is known to be the first point into the bounding
//  box. It can also be used to reset the box to contain one point.
//
inline void 
BBox2::InsFirst(const Point2 &pnt)
{
	minPnt = maxPnt = pnt;
}

inline void
BBox2::InsFirst(const BBox2 &box)
{
	minPnt = box.minPnt;
	maxPnt = box.maxPnt;
}

inline void
BBox2::InsFirst(const Point2 &segSrc, const Point2 &segDst)
{
	if (segSrc.vecCoord.x < segDst.vecCoord.x) {
		minPnt.vecCoord.x = segSrc.vecCoord.x; maxPnt.vecCoord.x = segDst.vecCoord.x;
	} else {
		minPnt.vecCoord.x = segDst.vecCoord.x; maxPnt.vecCoord.x = segSrc.vecCoord.x;
	}
	if (segSrc.vecCoord.y < segDst.vecCoord.y) {
		minPnt.vecCoord.y = segSrc.vecCoord.y; maxPnt.vecCoord.y = segDst.vecCoord.y;
	} else {
		minPnt.vecCoord.y = segDst.vecCoord.y; maxPnt.vecCoord.y = segSrc.vecCoord.y;
	}
}

inline 
BBox2::BBox2(const Point2 &tPnt) :
	minPnt(tPnt),
	maxPnt(tPnt)
{
}

// Function: BBox2
//    Inserting a line segment is a special case that can be executed faster than
//  initializing with one point and inserting a second.  This case should be
//  fast since it corresponds to finding the bounding box of a line segment.
inline 
BBox2::BBox2(const Point2 &segSrc, const Point2 &segDst)
{
	// REVIEW: OPT - this might call the default constructors on the two points before
	// 		calling InsFirst
	InsFirst(segSrc, segDst);
}

inline
BBox2::BBox2(const BBox2 &box) :
	minPnt(box.minPnt),
	maxPnt(box.maxPnt) 
{
}

// Function InsNext
//    Use this to insert points after at least one point has been inserted into
//  the bounding box.  This is incorrect if applied to an empty box.
//
inline void 
BBox2::InsNext(const Point2 &pnt)
{
	float x = pnt.vecCoord.x;
	float y = pnt.vecCoord.y;

	if (x < minPnt.vecCoord.x) minPnt.vecCoord.x = x;
	else if (x > maxPnt.vecCoord.x) maxPnt.vecCoord.x = x;
	if (y < minPnt.vecCoord.y) minPnt.vecCoord.y = y;
	else if (y > maxPnt.vecCoord.y) maxPnt.vecCoord.y = y;
}

// Function: InsNext
//    Insert a bounding box into an initialized, existing bounding box.
inline void 
BBox2::InsNext(const BBox2 &box)
{
	if (minPnt.vecCoord.x > box.minPnt.vecCoord.x) minPnt.vecCoord.x = box.minPnt.vecCoord.x;
	if (minPnt.vecCoord.y > box.minPnt.vecCoord.y) minPnt.vecCoord.y = box.minPnt.vecCoord.y;
	if (maxPnt.vecCoord.x > box.maxPnt.vecCoord.x) maxPnt.vecCoord.x = box.maxPnt.vecCoord.x;
	if (maxPnt.vecCoord.y > box.maxPnt.vecCoord.y) maxPnt.vecCoord.y = box.maxPnt.vecCoord.y;
}

// Function: Intersect
//    Return true if two bounding boxes overlap, false otherwise.
inline BOOL 
BBox2::Intersect(const BBox2 &box) const
{
	return ((maxPnt.vecCoord.x >= box.minPnt.vecCoord.x) && (box.maxPnt.vecCoord.x >= minPnt.vecCoord.x) &&
		(maxPnt.vecCoord.y >= box.minPnt.vecCoord.y) && (box.maxPnt.vecCoord.y >= minPnt.vecCoord.y));
}

// Function: IsInside
//    Return FALSE if pnt is outside bounding box, TRUE if inside
inline BOOL
BBox2::IsInside(const Point2 &pnt) const
{
	return (INRANGE(pnt.vecCoord.x, minPnt.vecCoord.x, maxPnt.vecCoord.x) &&
		INRANGE(pnt.vecCoord.y, minPnt.vecCoord.y, maxPnt.vecCoord.y));
}


///////////
// Segment2
///////////

inline
Segment2::Segment2(const Point2 &src, const Point2 &dst) 
: N(src.vecCoord.y - dst.vecCoord.y, dst.vecCoord.x - src.vecCoord.x),
  bbox(src, dst) 
{
	rgPnt[0] = src;
	rgPnt[1] = dst;
}

// Function: FullIntersect
//    This function decides if two undirected segments intersect (i.e. no
//  backface culling).
//
// Hacked From:
// Faster Line Segment Intersection by Franklin Antonio, Graphics Gems III
inline BOOL 
Segment2::FullIntersect(const Segment2 &seg) const 
{
	if (!bbox.Intersect(seg.bbox)) return FALSE;

	float Ax, Ay, Cx, Cy, d, aNum, bNum;
	
	Ax = N.y;
	Ay = N.x;

	float Bx = seg.N.y;
	float By = seg.N.x;
	
	float x1 = rgPnt[0].X();
	float y1 = rgPnt[0].Y();
	float x3 = seg.rgPnt[0].X();
	float y3 = seg.rgPnt[0].Y();

	Cx = x3-x1;
	Cy = y1-y3;
	
	d = Ay*Bx - Ax*By;				// both denominator
	aNum = Bx*Cy - By*Cx;			// alpha numerator
	
	// alpha tests
	if (d > 0.f) {
		if (aNum < 0.f || aNum > d) return FALSE;
	} else {
		if (aNum > 0.f || aNum < d) return FALSE;
	}
	
	bNum = Ax*Cy - Ay*Cx;			// beta numerator
	
	// beta tests
	if (d > 0.f) {
		if (bNum < 0.f || bNum > d) return FALSE;
	} else {
		if (bNum > 0.f || bNum < d) return FALSE;
	}
	return TRUE;           
}

// Function: CCW
//    This function returns TRUE if the given point is to the LEFT OF or ON 
//  the line segment, modulo numerical error.  Notice this is NOT simply !CW,
//  since both return true when the pnt is on the line thru the segment.
inline BOOL
Segment2::IsCCW(const Point2 &pnt) const
{
//	return ((N * (pnt - GetDst())) >= 0.f);

	// it is more numerically stable to compute the two halves of the dot product
	// and compare them, than to do the final add and compare to zero
	return (N.x * (pnt.X() - rgPnt[1].X()) >= -N.y * (pnt.Y() - rgPnt[1].Y()));
}

// Function: CW
//    This function returns TRUE if the given point is to the RIGHT OF or ON 
//  the line segment, modulo numerical error.  Notice this is NOT simply !CCW,
//  since both return true when the pnt is on the line thru the segment.
inline BOOL
Segment2::IsCW(const Point2 &pnt) const
{
	return (N.x * (pnt.X() - rgPnt[1].X()) <= -N.y * (pnt.Y() - rgPnt[1].Y()));
}


// Function: Intersect
//    This function performs a directed intersection of two line segments.  That
//  is, an intersection exists if the incumbent segment passes from the left
//  to the right side of the this segment.  Notice that if you swap the roles of
//  the two segments, you get a different solution, because of the directedness
//  of the calculation.
//  The convention being used here is that the src vertex represents a closed
//  interval endpoint, and the dst vertex represents an open interval endpoint.
//  Modulo numerical error, there will be no intersection between two segments 
//  where one segment's src is the same as the other dst.
//
inline BOOL
Segment2::Intersect(const Segment2 &seg) const 
{
	// ALGORITHM: use 4 dot products and back face culling

	// if segment src is to the right of this, we can backface cull ==> no intersection
	// NB: if the segment dst lands exactly on the line, discard because open
	return (bbox.Intersect(seg.bbox) && 
			IsCCW(seg.GetSrc()) && !IsCCW(seg.GetDst()) && seg.IsCW(GetSrc()) && !seg.IsCW(GetDst()));
}

#endif
