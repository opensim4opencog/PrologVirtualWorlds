// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

//
//	Classes to support 2D and 3D affine space
//	D. P. M.  95/06/02.
//
// Author: MM
//
// History:
// -@- 8/1/95 (mm)
//    - unified & cleaned up interface
//    - added Print and Parse
// -@- 8/1/95 (mm)
//    define all inlined functions with macros 
//
#ifndef POINT_H
#define POINT_H

#include "vecmath.h"

class Point2 {
public:
					Point2()						{}
					Point2(float x, float y)		: vecCoord(x, y) {}
					Point2(const Vector2& v)		: vecCoord(v) {}

	// ALGEBRAIC OPERATORS
	friend Point2	operator +(const Point2 &p, const Vector2 &v);
	friend Point2	operator +(const Vector2 &v, const Point2 &p);
	friend Point2	operator -(const Point2 &p, const Vector2 &v);
	friend Point2	operator -(const Vector2 &v, const Point2 &p);
	friend Vector2	operator -(const Point2 &p1, const Point2 &p2);
	friend Point2	operator +=(const Point2 &p, const Vector2 &v);
	friend Point2	operator -=(const Point2 &p, const Vector2 &v);
	friend int		operator ==(const Point2 &p1, const Point2 &p2);
	friend int		operator !=(const Point2 &p1, const Point2 &p2);

	friend Point2	Lerp(const Point2 &p1, const Point2 &p2, float a);
	friend BOOL		IsCCW(const Point2 &p0, const Point2 &p1, const Point2 &p2);

	// DATA ACCESS
	float			X() const { return vecCoord.x; }
	float			Y() const { return vecCoord.y; }

	float			&X() { return vecCoord.x; }
	float			&Y() { return vecCoord.y; }

	// I/O INTERFACE
	BOOL			PrintASCII(FILE *pf) const;
	BOOL			ParseASCII(FILE *pf);

public:
	Vector2			vecCoord;
};


class Point3 {
public:
	Point3()										{}
	Point3(float x, float y, float z)				: vecCoord(x, y, z) {}
	Point3(const Vector3& v)						: vecCoord(v) {}
	
	// ALGEBRAIC OPERATORS
	friend Point3	operator +(const Point3 &p, const Vector3 &v);
	friend Point3	operator +(const Vector3 &v, const Point3 &p);
	friend Point3	operator -(const Point3 &p, const Vector3 &v);
	friend Point3	operator -(const Vector3 &v, const Point3 &p);
	friend Vector3	operator -(const Point3 &p1, const Point3 &p2);
	friend Point3	operator +=(const Point3 &p, const Vector3 &v);
	friend Point3	operator -=(const Point3 &p, const Vector3 &v);
	friend int		operator ==(const Point3 &p1, const Point3 &p2);
	friend int		operator !=(const Point3 &p1, const Point3 &p2);

	friend Point3	Lerp(const Point3 &p1, const Point3 &p2, float a);
	Point2			Project(int axis) const;

	// DATA ACCESS
	float			X() const { return vecCoord.x; }
	float			Y() const { return vecCoord.y; }
	float			Z() const { return vecCoord.z; }

	float			&X() { return vecCoord.x; }
	float			&Y() { return vecCoord.y; }
	float			&Z() { return vecCoord.z; }

	// I/O INTERFACE
	BOOL			PrintASCII(FILE *pf) const;
	BOOL			ParseASCII(FILE *pf);

public:
	Vector3			vecCoord;
};



#define PRINTASCII(n) \
inline BOOL \
Point##n::PrintASCII(FILE *pf) const {\
	return vecCoord.PrintASCII(pf);\
}

#define PARSEASCII(n) \
inline BOOL \
Point##n::ParseASCII(FILE *pf) {\
	return vecCoord.ParseASCII(pf);\
}

#define LERP(n) \
inline Point##n \
Lerp(const Point##n &p1, const Point##n &p2, float a) {\
	return Point##n((1.0f-a)*p1.vecCoord + a*p2.vecCoord);\
}

// Macro: PP_OP
//    point-point operation - define an operator that takes two point operands
#define PP_OP(ret_type,op,n) \
inline ret_type \
operator op(const Point##n &p1, const Point##n &p2) { \
	return ret_type(p1.vecCoord op p2.vecCoord); \
}

// Macro: PV_OP
//    point-vector operation - define an operator that takes a point & vector operand
#define PV_OP(op,n) \
inline Point##n \
operator op(const Point##n &p, const Vector##n &v) { \
	return Point##n(p.vecCoord op v); \
}

// Macro: VP_OP
//    vector-point operation - define an operator that takes a vector & point operand
#define VP_OP(op,n) \
inline Point##n \
operator op(const Vector##n &v, const Point##n &p) { \
	return (p op v); \
}

#define DEFINE_FUNCS(n) PRINTASCII(n) PARSEASCII(n) LERP(n) \
						PP_OP(int,==,n) PP_OP(int,!=,n) PP_OP(Vector##n,-,n) \
						PV_OP(+,n) PV_OP(-,n) PV_OP(+=,n) PV_OP(-=,n) \
						VP_OP(+,n) VP_OP(-,n)

DEFINE_FUNCS(2)		// Point2
DEFINE_FUNCS(3)		// Point3

inline Point2
Point3::Project(int dim) const {
	return Point2(vecCoord.Project(dim));
}

inline BOOL
IsCCW(const Point2 &p0, const Point2 &p1, const Point2 &p2)
{
	return ((p0.vecCoord.y - p1.vecCoord.y) * (p2.vecCoord.x - p1.vecCoord.x) >=
		-(p1.vecCoord.x - p0.vecCoord.x) * (p2.vecCoord.y - p1.vecCoord.y));
}

#endif
