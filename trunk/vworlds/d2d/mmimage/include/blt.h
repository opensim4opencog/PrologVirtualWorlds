// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _Blt_h
#define _Blt_h

// File:	Blt.h
// Author:	MM    (MM)
//
// Description:
//    These are all of the blt routines that are available.  It is assumed
//  that clipping and parameter checking has already occurred.


typedef HRESULT (*HasPixelFn)(const BYTE *pSrcPixels, DWORD nSrcPitch, DWORD dwPixel, 
									DWORD nSrcWidth, DWORD nHeight, BOOL *pb);

HRESULT HasPixel8(		const BYTE *pSrcPixels, DWORD nSrcPitch, DWORD dwPixel,
						DWORD nSrcWidth, DWORD nHeight, BOOL *pb);
HRESULT HasPixel16(		const BYTE *pSrcPixels, DWORD nSrcPitch, DWORD dwPixel,
						DWORD nSrcWidth, DWORD nHeight, BOOL *pb);
HRESULT HasPixel24(		const BYTE *pSrcPixels, DWORD nSrcPitch, DWORD dwPixel,
						DWORD nSrcWidth, DWORD nHeight, BOOL *pb);
HRESULT HasPixel32(		const BYTE *pSrcPixels, DWORD nSrcPitch, DWORD dwPixel,
						DWORD nSrcWidth, DWORD nHeight, BOOL *pb);

//
// Regular Image Bltting
//
// Notes:
//    Notice we can do subrectangle bltting by adjusting the src & dst
//  pixel pointers before calling these routines.



// straight Blts
HRESULT BltFast(		const BYTE *pSrcPixels, DWORD nSrcPitch,
						BYTE *pDstPixels, DWORD nDstPitch, 
						DWORD nSrcWidth, DWORD nHeight);

HRESULT BltFastMirrorY(	const BYTE *pSrcPixels, DWORD nSrcPitch,
						BYTE *pDstPixels, DWORD nDstPitch, 
						DWORD nSrcWidth, DWORD nHeight);

HRESULT BltFastRGBToRGB(const BYTE *pSrcPixels, DWORD nSrcPitch,
						BYTE *pDstPixels, DWORD nDstPitch, 
						DWORD nWidth, DWORD nHeight,
						const CPixelInfo &pixiSrc, const CPixelInfo &pixiDst);

HRESULT BltFast24To16(	const BYTE *pSrcPixels, DWORD nSrcPitch,
						BYTE *pDstPixels, DWORD nDstPitch, 
						DWORD nWidth, DWORD nHeight,
						const CPixelInfo &pixiSrc, const CPixelInfo &pixiDst);

HRESULT BltFast16To16(	const BYTE *pSrcPixels, DWORD nSrcPitch,
						BYTE *pDstPixels, DWORD nDstPitch, 
						DWORD nWidth, DWORD nHeight,
						const CPixelInfo &pixiSrc, const CPixelInfo &pixiDst);

HRESULT BltFast32To32(	const BYTE *pSrcPixels, DWORD nSrcPitch,
						BYTE *pDstPixels, DWORD nDstPitch, 
						DWORD nWidth, DWORD nHeight,
						const CPixelInfo &pixiSrc, const CPixelInfo &pixiDst);

HRESULT BltFast24To32(	const BYTE *pSrcPixels, DWORD nSrcPitch,
						BYTE *pDstPixels, DWORD nDstPitch, 
						DWORD nWidth, DWORD nHeight,
						const CPixelInfo &pixiSrc, const CPixelInfo &pixiDst);

HRESULT BltFast8To4(	const BYTE *pSrcPixels, DWORD nSrcPitch,
						BYTE *pDstPixels, DWORD nDstPitch,
						DWORD nWidth, DWORD nHeight, DWORD nOffset);

HRESULT BltFast8To2(	const BYTE *pSrcPixels, DWORD nSrcPitch,
						BYTE *pDstPixels, DWORD nDstPitch,
						DWORD nWidth, DWORD nHeight, DWORD nOffset);

HRESULT BltFast8To1(	const BYTE *pSrcPixels, long nSrcPitch,
						BYTE *pDstPixels, long nDstPitch,
						DWORD nWidth, DWORD nHeight, DWORD nOffset);

// translation Blts
HRESULT BltFast8To8T(	const BYTE *pSrcPixels, long nSrcPitch,
						BYTE *pDstPixels, long nDstPitch,
						DWORD nWidth, DWORD nHeight,
						const BYTE *pIndexMap, const BOOL bTransparent, const DWORD dwTransSrc);
HRESULT BltFast8To16T(	const BYTE *pSrcPixels, long nSrcPitch,
						BYTE *pDstPixels, long nDstPitch,
						DWORD nWidth, DWORD nHeight,
						const BYTE *pIndexMap, const BOOL bTransparent, const DWORD dwTransSrc);
HRESULT BltFast8To24T(	const BYTE *pSrcPixels, long nSrcPitch,
						BYTE *pDstPixels, long nDstPitch,
						DWORD nWidth, DWORD nHeight,
						const BYTE *pIndexMap, const BOOL bTransparent, const DWORD dwTransSrc);
HRESULT BltFast8To32T(	const BYTE *pSrcPixels, long nSrcPitch,
						BYTE *pDstPixels, long nDstPitch,
						DWORD nWidth, DWORD nHeight,
						const BYTE *pIndexMap, const BOOL bTransparent, const DWORD dwTransSrc);

//
// RLE Bltting
// Notes:
//    RLE is assumed to encode transparency as the zeroth index.
//
// straight Blts
HRESULT BltFastRLE8(DWORD nXPos, DWORD nYPos,
					const BYTE *pSrcPixels, long nSrcPitch,
					BYTE *pDstPixels, long nDstPitch,
					const LPRECT prSrcRect);

// translation Blts
HRESULT BltFastRLE8To8T(DWORD nXPos, DWORD nYPos,
						const BYTE *pSrcPixels, long nSrcPitch,
						BYTE *pDstPixels, long nDstPitch,
						const LPRECT prSrcRect, const BYTE *pIndexMap);
HRESULT BltFastRLE8To16T(DWORD nXPos, DWORD nYPos,
						const BYTE *pSrcPixels, long nSrcPitch,
						BYTE *pDstPixels, long nDstPitch,
						const LPRECT prSrcRect, const BYTE *pIndexMap);
HRESULT BltFastRLE8To24T(DWORD nXPos, DWORD nYPos,
						const BYTE *pSrcPixels, long nSrcPitch,
						BYTE *pDstPixels, long nDstPitch,
						const LPRECT prSrcRect, const BYTE *pIndexMap);
HRESULT BltFastRLE8To32T(DWORD nXPos, DWORD nYPos,
						const BYTE *pSrcPixels, long nSrcPitch,
						BYTE *pDstPixels, long nDstPitch,
						const LPRECT prSrcRect, const BYTE *pIndexMap);

// Function: WriteXBitRow
//    These functions pack bytes into bit streams.  Buffers with
//  a bit count <= sizeof(unsigned char) are passed as a buffer 
//  of unsigned char's, buffers with a bit count <= sizeof(unsigned
//  short) are passed as unsigned short, etc.
//void *Write4BitRow(void *pDst, const void *pSrc, unsigned int cCount);
//void *Write2BitRow(void *pDst, const void *pSrc, unsigned int cCount);
//void *Write1BitRow(void *pDst, const void *pSrc, unsigned int cCount);

#endif
