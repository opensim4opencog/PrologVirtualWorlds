// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _D2DTrans_h
#define _D2DTrans_h

// File:	D2DTrans.h
// Author:	MM    (MM)
//
// Description:
//    This class is used to represent transformations while traversing
//  the frame hierarchy.
// 
// History:
// -@- 11/18/96 (MM) created

struct D2DPoint;
struct D2DRect;
struct D2DVector;
struct D2DCovector;

class D2DTransform {
public:
	//
	// CONSTRUCTION
	//
						D2DTransform();
						D2DTransform(const D2DTransform &t2d);
						~D2DTransform() {}
	
	D2DTransform &		operator=(const D2DTransform &t2d);
	
	void				Translate(float fX, float fY);
	void				Scale(float fX, float fY);
//	void				Rotate(float fTheta);
	void				SetTranslation(float fX, float fY) { m_fXPos = fX; m_fYPos = fY; }
	void				SetScale(float fX, float fY) { m_fXScale = fX; m_fYScale = fY; }

	void				Init();
	void				Set(const D2DTransform &t2d);

	//
	// DATA ACCESS
	//
	float				GetXPos() const { return m_fXPos; }
	float				GetYPos() const { return m_fYPos; }
	float				GetXScale() const { return m_fXScale; }
	float				GetYScale() const { return m_fYScale; }
	void				GetInverse(D2DTransform *pt2d) const;
//	float				GetRotation() const { return m_fTheta; }

	//
	// APPLICATION
	//
	void				Transform(const D2DTransform &t2dSrc, D2DTransform *pt2dDst) const;
	void				Transform(const RECT &rSrc, LPRECT prDst) const;
	void				Transform(const D2DPoint &pt2Src, D2DPoint *ppt2Dst) const;
	void				Transform(const D2DVector &vSrc, D2DVector *pvDst) const;
	BOOL				TransformClip(const D2DRect &r2Clip, D2DRect *pr2Src, D2DRect *pr2Dst) const;
	void				InverseTransform(const D2DPoint &pt2Src, D2DPoint *ppt2Dst) const;
	void				InverseTransform(const D2DVector &vSrc, D2DVector *pvDst) const;
	void				InverseXTransform(float fSrc, float *pfDst) const;
	void				InverseYTransform(float fSrc, float *pfDst) const;

private:
	// when rotation is added, this is probably going to have to change to
	// a matrix representation
	float m_fXPos, m_fYPos;
	float m_fXScale, m_fYScale;
};

inline void
D2DTransform::Init()
{
	// default is the identity transformation
	m_fXPos = m_fYPos = 0.f;
	m_fXScale = m_fYScale = 1.f;
}

inline
D2DTransform::D2DTransform()
{
	// Init();
}

inline void
D2DTransform::Set(const D2DTransform &t2d)
{
	m_fXPos = t2d.m_fXPos;
	m_fYPos = t2d.m_fYPos;
	m_fXScale = t2d.m_fXScale;
	m_fYScale = t2d.m_fYScale;
}

inline
D2DTransform::D2DTransform(const D2DTransform &t2d)
{
	Set(t2d);
}

inline D2DTransform &
D2DTransform::operator=(const D2DTransform &t2d)
{
	Set(t2d);
	return *this;
}

inline void
D2DTransform::Translate(float fX, float fY)
{
	m_fXPos += fX;
	m_fYPos += fY;
}

inline void
D2DTransform::Scale(float fX, float fY)
{
	m_fXScale *= fX;
	m_fXPos *= fX;
	m_fYScale *= fY;
	m_fYPos *= fY;
}


// Function: InverseXTransform
//    This only works because we have no rotation...
inline void
D2DTransform::InverseXTransform(float fSrc, float *pfDst) const
{
	*pfDst = (fSrc - m_fXPos) / m_fXScale;
}

// Function: InverseYTransform
//    This only works because we have no rotation...
inline void
D2DTransform::InverseYTransform(float fSrc, float *pfDst) const
{
	*pfDst = (fSrc - m_fYPos) / m_fYScale;
}

#endif
