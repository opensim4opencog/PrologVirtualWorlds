// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _MMUtils_h
#define _MMUtils_h

// File:	MMUtils.h
// Author:	MM
// Description:
//     This header contains general utility functions.
// 
// History:
// -@- 8/4/95 (mm) created
// -@- 2/5/96 (mm) removed debug stuff
// -@- 4/12/96 (mm) munged this up so it is pretty small
// -@- 6/20/96 (mm) added CLAMP

#define INRANGE(x, xmin, xmax) 	(((x) >= (xmin)) && ((x) <= (xmax)))
#define INARRAY(x, xmax) 		(((x) >= 0) && ((x) < (xmax)))

// "ASSEMBLY" MACROS
#define SETABS(x)					if ((x) < 0) (x) = -(x)
#define SETMAX(dst, src1, src2)		if ((src1) < (src2)) (dst) = (src2); else (dst) = (src1)
#define SETMIN(dst, src1, src2)		if ((src1) > (src2)) (dst) = (src2); else (dst) = (src1)
#define UPDATEMAX(dst, src)			if ((src) > (dst)) (dst) = (src)
#define UPDATEMIN(dst, src)			if ((src) < (dst)) (dst) = (src)
#define CLAMP(x, xmin, xmax)		if ((x) < (xmin)) (x) = (xmin); else if ((x) > (xmax)) (x) = (xmax)

#endif
