// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// File:	D2DTrans.cpp
// Author:	MM    (MM)

#include "stdafx.h"
#include "D2DTrans.h"


// Function: Transform
//    The transform matrix for scale & translation only looks like:
// [sx   0    tx]
// [0    sy   ty]
// [0    0    1 ]
void
D2DTransform::Transform(const D2DTransform &t2dSrc, D2DTransform *pt2dDst) const
{
	// this should work even if this, src, and/or dst are the same D2DTransform
	pt2dDst->m_fXPos = t2dSrc.m_fXPos * m_fXScale + m_fXPos;
	pt2dDst->m_fXScale = t2dSrc.m_fXScale * m_fXScale;
	pt2dDst->m_fYPos = t2dSrc.m_fYPos * m_fYScale + m_fYPos;
	pt2dDst->m_fYScale = t2dSrc.m_fYScale * m_fYScale;
}

void
D2DTransform::Transform(const RECT &rSrc, LPRECT prDst) const
{
	// this should work even if src and dst are the same RECT
	prDst->left = (long) (((float) rSrc.left) * m_fXScale + m_fXPos);
	prDst->right = (long) (((float) rSrc.right) * m_fXScale + m_fXPos);
	prDst->top = (long) (((float) rSrc.top) * m_fYScale + m_fYPos);
	prDst->bottom = (long) (((float) rSrc.bottom) * m_fYScale + m_fYPos);
}

// Function: Transform
void
D2DTransform::Transform(const D2DPoint &pt2Src, LPD2DPOINT ppt2Dst) const
{
	ppt2Dst->x = (pt2Src.x + m_fXPos) * m_fXScale;
	ppt2Dst->y = (pt2Src.y + m_fYPos) * m_fYScale;
}

void
D2DTransform::Transform(const D2DVector &vSrc, D2DVector *pvDst) const
{
	pvDst->x = vSrc.x * m_fXScale;
	pvDst->y = vSrc.y * m_fYScale;
}



// The inverse transform matrix for scale & translation only looks like:
// [1/sx 0    -tx/sx]
// [0    1/sy -ty/sy]
// [0    0    1     ]

// Function: GetInverse
void
D2DTransform::GetInverse(D2DTransform *pt2d) const
{
	pt2d->m_fXScale = 1.f/m_fXScale;
	pt2d->m_fYScale = 1.f/m_fYScale;
	pt2d->m_fXPos = -m_fXPos * pt2d->m_fXScale;
	pt2d->m_fYPos = -m_fYPos * pt2d->m_fYScale;
}

// Function: InverseTransform
void
D2DTransform::InverseTransform(const D2DPoint &pt2Src, LPD2DPOINT ppt2Dst) const
{
	ppt2Dst->x = (pt2Src.x - m_fXPos) / m_fXScale;
	ppt2Dst->y = (pt2Src.y - m_fYPos) / m_fYScale;
}

void
D2DTransform::InverseTransform(const D2DVector &vSrc, D2DVector *pvDst) const
{
	pvDst->x = vSrc.x / m_fXScale;
	pvDst->y = vSrc.y / m_fYScale;
}



// Function: InverseTransform
//    The inverse transform matrix for scale & translation only looks like:
// [1/sx 0    -tx/sx]
// [0    1/sy -ty/sy]
// [0    0    1     ]
/*void
D2DTransform::InverseTransform(const RECT &rSrc, LPRECT prDst) const
{
	// this should work even if src and dst are the same RECT
	float fInvScale;
	fInvScale = 1.f/m_fXScale;
	prDst->left = (long) ((((float) rSrc.left) - m_fXPos) * fInvScale);
	prDst->right = (long) ((((float) rSrc.right) - m_fXPos) * fInvScale);
	fInvScale = 1.f/m_fYScale;
	prDst->top = (long) ((((float) rSrc.top) - m_fYPos) * fInvScale);
	prDst->bottom = (long) ((((float) rSrc.bottom) - m_fYPos) * fInvScale);
}
*/

BOOL
D2DTransform::TransformClip(const D2DRect &r2Clip, LPD2DRECT pr2Src, LPD2DRECT pr2Dst) const
{
	//
	// transform
	//
	D2DRect r2Dst;
	r2Dst.left = pr2Src->left * m_fXScale + m_fXPos;
	r2Dst.right = pr2Src->right * m_fXScale + m_fXPos;
	r2Dst.top = pr2Src->top * m_fYScale + m_fYPos;
	r2Dst.bottom = pr2Src->bottom * m_fYScale + m_fYPos;

	//
	// clip
	//
	if ((r2Clip.left >= r2Dst.right) ||
		(r2Clip.right <= r2Dst.left) ||
		(r2Clip.top >= r2Dst.bottom) ||
		(r2Clip.bottom <= r2Dst.top)) {
		// no intersection
		return TRUE;
	} else {
		// clip dst and src RECT's
		if (r2Clip.left > r2Dst.left) {
			r2Dst.left = r2Clip.left;
			InverseXTransform(r2Dst.left, &(pr2Src->left));
		}
		if (r2Clip.right < r2Dst.right) {
			r2Dst.right = r2Clip.right;
			InverseXTransform(r2Dst.right, &(pr2Src->right));
		}
		if (r2Clip.top > r2Dst.top) {
			r2Dst.top = r2Clip.top;
			InverseYTransform(r2Dst.top, &(pr2Src->top));
		}
		if (r2Clip.bottom < r2Dst.bottom) {
			r2Dst.bottom = r2Clip.bottom;
			InverseYTransform(r2Dst.bottom, &(pr2Src->bottom));
		}
	}
	*pr2Dst = r2Dst;
	return FALSE;
}
