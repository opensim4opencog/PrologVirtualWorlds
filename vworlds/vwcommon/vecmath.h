// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

//
//	Classes to support 2D and 3D linear vector space and their duals
//	D.P.M.  95/06/02.
//
//  History:
//	-@- 7/6/95 (mm) added Print and Read functions
//  -@- 8/1/95 (mm) added fuzzy compare for floats
//  -@- 4/15/96 (mm) changed stdio stuff to (dm)'s stream stuff
//  -@- 4/18/96 (mm) added vector.inl to this file
//  -@- 6/21/96 (mm) added +=, etc. operators

#ifndef VECTORMATH_H
#define VECTORMATH_H

class CoVector2;
class CoVector3;
class Vector3;

class  Vector2 {
public:
						Vector2()								{}
						Vector2(float a, float b)				: x(a), y(b) {}

	// ALGEBRAIC OPERATORS
	friend Vector2		 operator +(const Vector2&, const Vector2&);
	friend Vector2		 operator -(const Vector2&, const Vector2&);
	friend Vector2		 operator -(const Vector2&);
	friend Vector2		 operator *(const Vector2&, float);
	friend Vector2		 operator *(float, const Vector2&);
	friend Vector2		 operator /(const Vector2&, float);
	friend int			 operator ==(const Vector2&, const Vector2&);
	friend int			 operator !=(const Vector2&, const Vector2&);

	friend Vector2		 operator +=(Vector2 &v1, const Vector2 &v2);
	friend Vector2		 operator -=(Vector2 &v1, const Vector2 &v2);
	friend Vector2		 operator *=(Vector2 &v1, float a);
	friend Vector2		 operator /=(Vector2 &v1, float a);

	friend float		 Cross(const Vector2&, const Vector2&);
	friend float		 Dot(const Vector2&, const Vector2&);
	friend CoVector2	 Transpose(const Vector2&);
	float				Norm() const;
	Vector2 			Unit() const;

	// I/O interface
	BOOL				PrintASCII(FILE *pf) const;
	BOOL				ParseASCII(FILE *pf);

public:
	float				x, y;
};

class  CoVector2 {
public:
						CoVector2()								{}
						CoVector2(float a, float b)				: x(a), y(b) {}

	// ALGEBRAIC OPERATORS
	friend CoVector2	 operator +(const CoVector2&, const CoVector2&);
	friend CoVector2	 operator -(const CoVector2&, const CoVector2&);
	friend CoVector2	 operator -(const CoVector2&);
	friend CoVector2	 operator *(const CoVector2&, float);
	friend CoVector2	 operator *(float, const CoVector2&);
	friend CoVector2	 operator /(const CoVector2&, float);
	friend int			 operator ==(const CoVector2&, const CoVector2&);
	friend int			 operator !=(const CoVector2&, const CoVector2&);

	friend CoVector2	 operator +=(CoVector2 &cov1, const CoVector2 &cov2);
	friend CoVector2	 operator -=(CoVector2 &cov1, const CoVector2 &cov2);
	friend CoVector2	 operator *=(CoVector2 &cov1, float a);
	friend CoVector2	 operator /=(CoVector2 &cov1, float a);

	friend float		 Cross(const CoVector2&, const CoVector2&);
	friend float		 Dot(const CoVector2&, const CoVector2&);
	friend Vector2		 Transpose(const CoVector2&);
	float				Norm() const;
	CoVector2 			Unit() const;
	friend float		 operator * (const CoVector2&, const Vector2&);

	// I/O interface
	BOOL				PrintASCII(FILE *pf) const;
	BOOL				ParseASCII(FILE *pf);

public:
	float				x, y;
};


class  Vector3 {
public:
						Vector3()								{}
						Vector3(float a, float b, float c)		: x(a), y(b), z(c) {}

	// ALGEBRAIC OPERATORS
	friend Vector3		 operator +(const Vector3&, const Vector3&);
	friend Vector3		 operator -(const Vector3&, const Vector3&);
	friend Vector3		 operator -(const Vector3&);
	friend Vector3		 operator *(const Vector3&, float);
	friend Vector3		 operator *(float, const Vector3&);
	friend Vector3		 operator /(const Vector3&, float);
	friend int			 operator ==(const Vector3&, const Vector3&);
	friend int			 operator !=(const Vector3&, const Vector3&);

	friend Vector3		 operator +=(Vector3 &v1, const Vector3 &v2);
	friend Vector3		 operator -=(Vector3 &v1, const Vector3 &v2);
	friend Vector3		 operator *=(Vector3 &v1, float a);
	friend Vector3		 operator /=(Vector3 &v1, float a);

	friend Vector3		 Cross(const Vector3&, const Vector3&);
	friend float		 Dot(const Vector3&, const Vector3&);
	friend CoVector3	 Transpose(const Vector3&);
	float				Norm() const;
	Vector3				Unit() const;
	Vector2				Project(int) const;
	void				Rotate(const Vector3 &vAxis, float fTheta);

	// I/O interface
	BOOL				PrintASCII(FILE *pf) const;
	BOOL				ParseASCII(FILE *pf);

public:
	float				x, y, z;							// column vector
};


class  CoVector3 {
public:
						CoVector3()								{}
						CoVector3(float a, float b, float c)	: x(a), y(b), z(c) {}

	friend CoVector3	 operator +(const CoVector3&, const CoVector3&);
	friend CoVector3	 operator -(const CoVector3&, const CoVector3&);
	friend CoVector3	 operator -(const CoVector3&);
	friend CoVector3	 operator *(const CoVector3&, float);
	friend CoVector3	 operator *(float, const CoVector3&);
	friend CoVector3	 operator /(const CoVector3&, float);
	friend int			 operator ==(const CoVector3&, const CoVector3&);
	friend int			 operator !=(const CoVector3&, const CoVector3&);

	friend CoVector3	 operator +=(CoVector3 &cov1, const CoVector3 &cov2);
	friend CoVector3	 operator -=(CoVector3 &cov1, const CoVector3 &cov2);
	friend CoVector3	 operator *=(CoVector3 &cov1, float a);
	friend CoVector3	 operator /=(CoVector3 &cov1, float a);

	friend CoVector3	 Cross(const CoVector3&, const CoVector3&);
	friend float		 Dot(const CoVector3&, const CoVector3&);
	friend Vector3		 Transpose(const CoVector3&);
	float				Norm() const;
	CoVector3			Unit() const;
	friend float		 operator *(const CoVector3&, const Vector3&);	// linear form

	// I/O interface
	BOOL				PrintASCII(FILE *pf) const;
	BOOL				ParseASCII(FILE *pf);

public:
	float				x, y, z;						// row vector
};



///////////////////
// INLINE FUNCTIONS
///////////////////

//
// History:
// -@- 7/6/95 (mm) added Print and Read functions
// -@- 7/10/95 (mm) added some iostream interface
//		moved to .inl file
// -@- 8/1/95 (mm)
//     clean up and add stdio I/O Interface
// -@- 8/7/95 (dm) add CoVector2 class for HalfSpace2
// -@- 4/18/96 (mm) added to vector.h

//////////
// Vector2
//////////

inline Vector2
operator +(const Vector2 &u, const Vector2 &v)
{
	return Vector2(u.x+v.x, u.y+v.y);
}

inline Vector2
operator -(const Vector2 &u, const Vector2 &v)
{
	return Vector2(u.x-v.x, u.y-v.y);
}

inline Vector2
operator -(const Vector2 &v)
{
	return Vector2(-v.x, -v.y);
}

inline Vector2
operator *(float a, const Vector2 &v)
{
	return Vector2(a*v.x, a*v.y);
}

inline Vector2
operator *(const Vector2 &v, float a)
{
	return Vector2(a*v.x, a*v.y);
}

inline Vector2
operator /(const Vector2 &v, float a)
{
	return Vector2(v.x/a, v.y/a);
}

inline int
operator !=(const Vector2 &v1, const Vector2 &v2)
{
	return !(v1 == v2);
}

inline Vector2
operator +=(Vector2 &v1, const Vector2 &v2)
{
	v1.x += v2.x; 
	v1.y += v2.y;
	return v1;
}

inline Vector2
operator -=(Vector2 &v1, const Vector2 &v2)
{
	v1.x -= v2.x; 
	v1.y -= v2.y;
	return v1;
}

inline Vector2
operator *=(Vector2 &v1, float a)
{
	v1.x *= a;
	v1.y *= a;
	return v1;
}

inline Vector2
operator /=(Vector2 &v1, float a)
{
	a = 1.f/a;
	v1.x *= a;
	v1.y *= a;
	return v1;
}

inline float
Cross(const Vector2 &u, const Vector2 &v)
{
	return u.x*v.y - u.y*v.x;
}

inline float
Dot(const Vector2 &u, const Vector2 &v)
{
	return u.x*v.x + u.y*v.y;
}

inline CoVector2
Transpose(const Vector2 &v)
{
	return CoVector2(v.x, v.y);
}

inline Vector2
Vector2::Unit() const
{
	return *this/this->Norm();
}

//////////
// CoVector2
//////////

inline CoVector2
operator +(const CoVector2 &u, const CoVector2 &v)
{
	return CoVector2(u.x+v.x, u.y+v.y);
}

inline CoVector2
operator -(const CoVector2 &u, const CoVector2 &v)
{
	return CoVector2(u.x-v.x, u.y-v.y);
}

inline CoVector2
operator -(const CoVector2 &v)
{
	return CoVector2(-v.x, -v.y);
}

inline CoVector2
operator *(float a, const CoVector2 &v)
{
	return CoVector2(a*v.x, a*v.y);
}

inline CoVector2
operator *(const CoVector2 &v, float a)
{
	return CoVector2(a*v.x, a*v.y);
}

inline CoVector2
operator /(const CoVector2 &v, float a)
{
	return CoVector2(v.x/a, v.y/a);
}

inline int
operator !=(const CoVector2 &v1, const CoVector2 &v2) {
	return !(v1 == v2);
}

inline CoVector2
operator +=(CoVector2 &v1, const CoVector2 &v2)
{
	v1.x += v2.x; 
	v1.y += v2.y;
	return v1;
}

inline CoVector2
operator -=(CoVector2 &v1, const CoVector2 &v2)
{
	v1.x -= v2.x; 
	v1.y -= v2.y;
	return v1;
}

inline CoVector2
operator *=(CoVector2 &v1, float a)
{
	v1.x *= a;
	v1.y *= a;
	return v1;
}

inline CoVector2
operator /=(CoVector2 &v1, float a)
{
	a = 1.f/a;
	v1.x *= a;
	v1.y *= a;
	return v1;
}

inline float
Cross(const CoVector2 &u, const CoVector2 &v)
{
	return u.x*v.y - u.y*v.x;
}

inline float
Dot(const CoVector2 &u, const CoVector2 &v)
{
	return u.x*v.x + u.y*v.y;
}

inline Vector2
Transpose(const CoVector2 &c)
{
	return Vector2(c.x, c.y);
}

inline CoVector2
CoVector2::Unit() const
{
	return *this/this->Norm();
}

inline float
operator *(const CoVector2 &c, const Vector2 &v)
{
	return c.x*v.x + c.y*v.y;
}


//////////
// Vector3
//////////

inline Vector3
operator +(const Vector3 &u, const Vector3 &v)
{
	return Vector3(u.x+v.x, u.y+v.y, u.z+v.z);
}

inline Vector3
operator -(const Vector3 &u, const Vector3 &v)
{
	return Vector3(u.x-v.x, u.y-v.y, u.z-v.z);
}

inline Vector3
operator -(const Vector3 &v)
{
	return Vector3(-v.x, -v.y, -v.z);
}

inline Vector3
operator *(float a, const Vector3 &v)
{
	return Vector3(a*v.x, a*v.y, a*v.z);
}

inline Vector3
operator *(const Vector3 &v, float a)
{
	return Vector3(a*v.x, a*v.y, a*v.z);
}

inline Vector3
operator /(const Vector3 &v, float a)
{
	return Vector3(v.x/a, v.y/a, v.z/a);
}

inline int
operator !=(const Vector3 &v1, const Vector3 &v2) {
	return !(v1 == v2);
}

inline Vector3
operator +=(Vector3 &v1, const Vector3 &v2)
{
	v1.x += v2.x; 
	v1.y += v2.y;
	v1.z += v2.z;
	return v1;
}

inline Vector3
operator -=(Vector3 &v1, const Vector3 &v2)
{
	v1.x -= v2.x; 
	v1.y -= v2.y;
	v1.z -= v2.z;
	return v1;
}

inline Vector3
operator *=(Vector3 &v1, float a)
{
	v1.x *= a;
	v1.y *= a;
	v1.z *= a;
	return v1;
}

inline Vector3
operator /=(Vector3 &v1, float a)
{
	a = 1.f/a;
	v1.x *= a;
	v1.y *= a;
	v1.z *= a;
	return v1;
}

inline Vector3
Cross(const Vector3 &u, const Vector3 &v)
{
	return Vector3(u.y*v.z-u.z*v.y, u.z*v.x-u.x*v.z, u.x*v.y-u.y*v.x);
}

inline float
Dot(const Vector3 &u, const Vector3 &v)
{
	return u.x*v.x + u.y*v.y + u.z*v.z;
}

inline float
operator *(const CoVector3 &c, const Vector3 &v)
{
	return c.x*v.x + c.y*v.y + c.z*v.z;
}

inline Vector3
Transpose(const CoVector3 &c)
{
	return Vector3(c.x, c.y, c.z);
}

inline Vector3
Vector3::Unit() const
{
	return *this/this->Norm();
}

////////////
// CoVector3
////////////

inline CoVector3
operator +(const CoVector3 &u, const CoVector3 &v)
{
	return CoVector3(u.x+v.x, u.y+v.y, u.z+v.z);
}

inline CoVector3
operator -(const CoVector3 &u, const CoVector3 &v)
{
	return CoVector3(u.x-v.x, u.y-v.y, u.z-v.z);
}

inline CoVector3
operator -(const CoVector3 &v)
{
	return CoVector3(-v.x, -v.y, -v.z);
}

inline CoVector3
operator *(float a, const CoVector3 &v)
{
	return CoVector3(a*v.x, a*v.y, a*v.z);
}

inline CoVector3
operator *(const CoVector3 &v, float a)
{
	return CoVector3(a*v.x, a*v.y, a*v.z);
}

inline CoVector3
operator /(const CoVector3 &v, float a)
{
	return CoVector3(v.x/a, v.y/a, v.z/a);
}

inline int
operator !=(const CoVector3 &v1, const CoVector3 &v2) {
	return !(v1 == v2);
}

inline CoVector3
operator +=(CoVector3 &v1, const CoVector3 &v2)
{
	v1.x += v2.x; 
	v1.y += v2.y;
	v1.z += v2.z;
	return v1;
}

inline CoVector3
operator -=(CoVector3 &v1, const CoVector3 &v2)
{
	v1.x -= v2.x; 
	v1.y -= v2.y;
	v1.z -= v2.z;
	return v1;
}

inline CoVector3
operator *=(CoVector3 &v1, float a)
{
	v1.x *= a;
	v1.y *= a;
	v1.z *= a;
	return v1;
}

inline CoVector3
operator /=(CoVector3 &v1, float a)
{
	a = 1.f/a;
	v1.x *= a;
	v1.y *= a;
	v1.z *= a;
	return v1;
}

inline CoVector3
Cross(const CoVector3 &u, const CoVector3 &v)
{
	return CoVector3(u.y*v.z-u.z*v.y, u.z*v.x-u.x*v.z, u.x*v.y-u.y*v.x);
}

inline float
Dot(const CoVector3 &u, const CoVector3 &v)
{
	return u.x*v.x + u.y*v.y + u.z*v.z;
}

inline CoVector3
Transpose(const Vector3 &v)
{
	return CoVector3(v.x, v.y, v.z);
}

inline CoVector3
CoVector3::Unit() const
{
	return *this/this->Norm();
}


#endif
