// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include "vecmath.h"


BOOL 
Vector2::PrintASCII(FILE *fd) const 
{
	return (fprintf(fd, "%f %f", x, y) < 0);
}

BOOL
Vector2::ParseASCII(FILE *fd) 
{
	return (fscanf(fd, "%f %f", &x, &y) == EOF);
}

BOOL
Vector3::PrintASCII(FILE *fd) const 
{
	return (fprintf(fd, "%f %f %f", x, y, z) < 0);
}

BOOL
Vector3::ParseASCII(FILE *fd) 
{
	return (fscanf(fd, "%f %f %f", &x, &y, &z) == EOF);
}


BOOL
CoVector2::PrintASCII(FILE *fd) const 
{
	return (fprintf(fd, "%f %f", x, y) < 0);
}

BOOL
CoVector2::ParseASCII(FILE *fd) 
{
	return (fscanf(fd, "%f %f", &x, &y) == EOF);
}


BOOL
CoVector3::PrintASCII(FILE *fd) const 
{
	return (fprintf(fd, "%f %f %f", x, y, z) < 0);
}

BOOL
CoVector3::ParseASCII(FILE *fd) 
{
	return (fscanf(fd, "%f %f %f", &x, &y, &z) == EOF);
}


Vector2
Vector3::Project(int nDim) const
{
	switch (nDim) {

	case 0:		return Vector2(y, z);
	case 1:		return Vector2(x, z);
	case 2:		return Vector2(x, y);
	default:	return Vector2(0.0f, 0.0f);
	}
}

// Function: FloatEquals
//    Peform a "fuzzy" compare of two floating point numbers.  This relies
//  on the IEEE bit representation of floating point numbers.
inline int 
FloatEquals(float x1, float x2) {
	#define EXPMASK 		0x7f800000
	#define BITSOFPRECISION 19
	#define MANTBITS 		23
	#define EXPOFFSET		(BITSOFPRECISION<<MANTBITS)
	#define ZEROEPS 		3.8e-6F
	#define TINYEPS         1.E-35F

	if (x1 == x2) return 1;		// quick out on exact match
	
	float flEps;

	if ((x1 == 0.0f) || (x2 == 0.0f)) {
		flEps = ZEROEPS;
	} else {
		float maxX;

		if (x1 > x2) 
			maxX = x1;
		else 
			maxX = x2;

		// grab the exponent of the larger number
		unsigned int uExp = (*((unsigned int *) &maxX) & EXPMASK);
		if (uExp < EXPOFFSET)
			flEps = TINYEPS;
		else {
			uExp -= EXPOFFSET;
			flEps = *((float *) &uExp);
		}
	}
	return (((x1 + flEps) >= x2) && ((x1 - flEps) <= x2));
}

int
operator ==(const Vector2 &v1, const Vector2 &v2) {
	return (FloatEquals(v1.x, v2.x) && FloatEquals(v1.y, v2.y));
}

int
operator ==(const CoVector2 &v1, const CoVector2 &v2) {
	return (FloatEquals(v1.x, v2.x) && FloatEquals(v1.y, v2.y));
}

int
operator ==(const Vector3 &v1, const Vector3 &v2) {
	return (FloatEquals(v1.x, v2.x) && FloatEquals(v1.y, v2.y) && FloatEquals(v1.z, v2.z));
}

int
operator ==(const CoVector3 &v1, const CoVector3 &v2) {
	return (FloatEquals(v1.x, v2.x) && FloatEquals(v1.y, v2.y) && FloatEquals(v1.z, v2.z));
}


#include <math.h>

float 
Vector2::Norm() const {
	return (float)sqrt(Dot(*this, *this));
}

float 
CoVector2::Norm() const {
	return (float)sqrt(Dot(*this, *this));
}

float 
Vector3::Norm() const {
	return (float)sqrt(Dot(*this, *this));
}

float
CoVector3::Norm() const {
	return (float)sqrt(Dot(*this, *this));
}

#include <math.h>

// Function: Rotate
//    rotate the vector counterclockwise around the given axis by theta radians
// Preconditions:
//    axis must be UNIT LENGTH
void Vector3::Rotate(const Vector3 &vAxis, float fTheta)
{
	float fCosTheta = float(cos(fTheta)), fSinTheta = float(sin(fTheta));
	
	*this *= fCosTheta;
	*this += (vAxis * (Dot(*this, vAxis) * (1.f - fCosTheta)));
	*this += (Cross(*this, vAxis) * fSinTheta);
}

