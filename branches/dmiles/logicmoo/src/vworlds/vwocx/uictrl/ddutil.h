// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef __DDUTIL__
#define __DDUTIL__


HRESULT GetBitmapFromThing(IWorld *pWorld, IThing *pThing, COLORREF rgbBackColor, LPBITMAPINFOHEADER *ppDIB);
HBITMAP DIBToBitmap(int cx, int cy, int depth, LPBITMAPINFOHEADER pDIB, int nFlags = 0);


#define WIDTHBYTES(i)           ((unsigned)((i+31)&(~31))/8)  /* ULONG aligned ! */
#define DibWidth(lpbi)          (UINT)(((LPBITMAPINFOHEADER)(lpbi))->biWidth)
#define DibHeight(lpbi)         (INT)(((LPBITMAPINFOHEADER)(lpbi))->biHeight >0?((LPBITMAPINFOHEADER)(lpbi))->biHeight:-((LPBITMAPINFOHEADER)(lpbi))->biHeight)

#define DibColors(lpbi) ((RGBQUAD FAR *)((LPBYTE)(lpbi) + (int)(lpbi)->biSize))

#define DibPtr(lpbi)  ((lpbi)->biCompression == BI_BITFIELDS \
                            ? (LPVOID)(DibColors(lpbi) + 3) \
                              : (LPVOID)(DibColors(lpbi) + (UINT)(lpbi)->biClrUsed))

#define DibWidthBytesN(lpbi, n) (UINT)WIDTHBYTES((UINT)(lpbi)->biWidth * (UINT)(n))
#define DibWidthBytes(lpbi)     DibWidthBytesN(lpbi, (lpbi)->biBitCount)

#define DibXYN(lpbi,pb,x,y,n)   (LPVOID)(                                     \
                                (BYTE *)(pb) +                          \
                                (UINT)((UINT)(x) * (UINT)(n) / 8u) +          \
                                ((DWORD)DibWidthBytesN(lpbi,n) * (DWORD)(UINT)(y)))

#define DibXY(lpbi,x,y)         DibXYN(lpbi,DibPtr(lpbi),x,y,(lpbi)->biBitCount)
#define DibInfo(pDIB) ((BITMAPINFO far *)(pDIB))

#define DibColors(lpbi)         ((RGBQUAD FAR *)((LPBYTE)(lpbi) + (int)(lpbi)->biSize))

#define DibNumColors(lpbi)      ((lpbi)->biClrUsed == 0 && (lpbi)->biBitCount <= 8 \
                                    ? (int)(1 << (int)(lpbi)->biBitCount)          \
                                    : (int)(lpbi)->biClrUsed)


#endif
