// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _MMUtils_h
#define _MMUtils_h

// File:	MMUtils.h
// Author:	MM    (MM)
// Description:
//     This header contains miscellaneous utility functions.
// 
// History:
// -@- 8/4/95 (MM) created
// -@- 2/5/96 (MM) removed debug stuff

#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <limits.h>
#include <memory.h>

#if defined(_WINDOWS) || defined(WIN32)
	#ifndef _INC_WINDOWS
		#define WIN32_EXTRA_LEAN
		#define WIN32_LEAN_AND_MEAN
		#include <WINDOWS.H>
	#endif
#endif

// TMP_BUF_SIZE must be at least 1024
#define TMP_BUF_SIZE 1024
#define nTMPBUFSIZE TMP_BUF_SIZE
extern char g_rgchTmpBuf[nTMPBUFSIZE];

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif
#ifndef INOUT
#define INOUT
#endif

#ifndef BOOL
typedef int BOOL;
typedef BOOL far *LPBOOL;
#endif
#ifndef BYTE
typedef unsigned char BYTE;
typedef BYTE far *LPBYTE;
#endif
#ifndef LPVOID
typedef void far *LPVOID;
#endif
#ifndef DWORD
typedef unsigned long DWORD;
typedef DWORD far *LPDWORD;
#endif
#ifndef WORD
typedef unsigned short WORD;
typedef WORD far *LPWORD;
#endif

#define maskBYTE	0xFF
#define maskWORD	0xFFFF
#define maskDWORD	0xFFFFFFFF

#define chSPC ' '
#define chTAB '	'
#define chEOL '\0'
#define chNULL '\0'

#define MACSTART do {
#define MACEND } while(0)

#define MMINITSTRUCT(__s) memset(&(__s), 0, sizeof(__s))

#ifndef INIT_DXSTRUCT
#define INIT_DXSTRUCT(__dxstruct) MACSTART \
	MMINITSTRUCT(__dxstruct); (__dxstruct).dwSize = sizeof(__dxstruct); MACEND
#endif

// Macro: ISWAP
//    This macro swaps two integer registers using 3 xor's.
//  This is unsafe because a & b are not guaranteed to get 2 regs.
#define ISWAP(a,b) MACSTART (a)^=(b); (b)^=(a); (a)^=(b); MACEND

// Macro: PSWAP
//    This macro swaps two pointers in place using 3 xor's.
#define PSWAP(a,b,type) MACSTART \
	a = (type *)(int(a) ^ int(b)); \
	b = (type *)(int(a) ^ int(b)); \
	a = (type *)(int(a) ^ int(b)); \
MACEND

/*inline void *
operator new(size_t size, void *memAddress) {
	return memAddress;
}*/

#ifndef SQR
#define SQR(a) ((a)*(a))
#endif

#ifndef ABS
#define ABS(x) ((x) < 0 ? -(x) : (x))
#endif

// Macro: SGN
//    take binary sign of a, either -1, or 1 if >= 0
#define SGN(a) (((a)<0) ? -1 : 1)


#define INRANGE(x, xmin, xmax) 	(((x) >= (xmin)) && ((x) <= (xmax)))
#define INARRAY(x, xmax) 		(((x) >= 0) && ((x) < (xmax)))

#define SETABS(x)					MACSTART if ((x) < 0) (x) = -(x); MACEND
#define SETMAX(dst, src1, src2)		MACSTART if ((src1) < (src2)) (dst) = (src2); else (dst) = (src1); MACEND
#define SETMIN(dst, src1, src2)		MACSTART if ((src1) > (src2)) (dst) = (src2); else (dst) = (src1); MACEND
#define UPDATEMAX(dst, src)			MACSTART if ((src) > (dst)) (dst) = (src); MACEND
#define UPDATEMIN(dst, src)			MACSTART if ((src) < (dst)) (dst) = (src); MACEND
#define UPDATEMINMAX(__min, __max, __src1, __src2) MACSTART \
	if ((__src1) < (__src2)) { \
		UPDATEMIN(__min, __src1); UPDATEMAX(__max, __src2); \
	} else {\
		UPDATEMIN(__min, __src2); UPDATEMAX(__max, __src1); \
	} \
MACEND
#define CLAMP(x, xmin, xmax) MACSTART \
	if ((x) > (xmax)) (x) = (xmax); \
	else if ((x) < (xmin)) (x) = (xmin); \
MACEND

//
// BIT MANIPULATION: BitVector
//
// **Hungarian Prefix: bv
typedef unsigned int BitVector;

#define NUMBITS(Type)	(sizeof(Type) << 3)
#define MASKRANGE(low, hi) \
(((((BitVector) ~0) >> (low)) << ((low) + (NUMBITS(BitVector) - 1 - (hi)))) >> (NUMBITS(BitVector) - 1 - (hi)))
#define SETBIT(bv, i) ((bv) |= (((BitVector) 1) << (i)))
#define UNSETBIT(bv, i) ((bv) &= ~(((BitVector) 1) << (i)))
#define SETRANGE(bv, lo, hi) ((bv) |= MASKRANGE(lo, hi))
#define UNSETRANGE(bv, lo, hi) ((bv) &= ~MASKRANGE(lo, hi))

//
// DEBUG STUFF
//
void _MMStall(const char *szExp, const char *szFile, int nLine);
void _MMTrace(const char *szFmt, ...);

#ifndef __AFX_H__
	#if defined(_DEBUG) && defined(_WINDOWS)
		#define DBGMSG(__sz) MACSTART OutputDebugString(__sz); MACEND
		#define DBGMSG0(__sz) DBGMSG(__sz)
		#define DBGMSG1(__sz, __arg1) MACSTART sprintf(g_rgchTmpBuf, __sz, __arg1); DBGMSG(g_rgchTmpBuf); MACEND
		#define DBGMSG2(__sz, __arg1, __arg2) MACSTART sprintf(g_rgchTmpBuf, __sz, __arg1, __arg2); DBGMSG(rgchTmpBuf); MACEND
	#else
		#define DBGMSG(__sz) MACSTART fprintf(stderr, __sz); MACEND
		#define DBGMSG0(__sz) DBGMSG(__sz)
		#define DBGMSG1(__sz, __arg1) MACSTART fprintf(stderr, __sz, __arg1); MACEND
		#define DBGMSG2(__sz, __arg1, __arg2) MACSTART fprintf(stderr, __sz, __arg1, __arg2); MACEND
	#endif

	#ifdef _DEBUG
		#define MMASSERT(exp)		(void)((exp) || (_MMStall(#exp, __FILE__, __LINE__),0))
		#define MMASSERT_VALID(exp)	MMASSERT(exp)
		#define MMVERIFY(exp)		MMASSERT(exp)
		#define MMDEBUG_ONLY(exp)	(exp)
		#define MMTRACE				::_MMTrace
	#else
		#define MMASSERT(exp)		((void)0)
		#define MMASSERT_VALID(exp)	((void)0)
		#define MMVERIFY(exp)		((void)(exp))
		#define MMDEBUG_ONLY(exp)	((void)0)
		#define MMTRACE				1 ? (void)0 : ::_MMTrace
	#endif

#else
	#define MMASSERT(exp)		ASSERT(exp)
	#define MMASSERT_VALID(exp)	ASSERT_VALID(exp)
	#define MMVERIFY(exp)		VERIFY(exp)
	#define MMDEBUG_ONLY(exp)	DEBUG_ONLY(exp)
	#define MMTRACE				TRACE
#endif

// Macro: DBGWINMSG
//    Output debug information describing a particular windows message
#if defined(_WINDOWS)
	const char *WinMsgIDToString(unsigned int uMsgID);
	#define DBGWINMSG(wMsgID, wParam, lParam) MACSTART \
		sprintf(g_rgchTmpBuf,"%16s | %04X | %08lX | %8d | %8d\n\r", WinMsgIDToString(wMsgID), wParam, lParam, HIWORD(lParam), LOWORD(lParam));\
		DBGMSG(g_rgchTmpBuf); MACEND
#endif


// Macro: MMCERTIFY
//    Assert expression even outside of debug
#define MMCERTIFY(exp) (void)((exp) || (_MMStall(#exp, __FILE__, __LINE__),0))

// Macro: MMRELEASE
//    Safe release for COM objects
// ***this code should never change - there is stuff that relies on the pointer being
//    set to NULL after being released
#ifndef MMRELEASE
#define MMRELEASE(_p) MACSTART if ((_p) != NULL) {(_p)->Release(); (_p) = NULL;} MACEND
#endif

#define MMDELETE(__ptr) MACSTART delete (__ptr); (__ptr) = NULL; MACEND
#define MMRGDELETE(__ptr) MACSTART delete [] (__ptr); (__ptr) = NULL; MACEND

#ifdef _DEBUG
	#define MMCHECK_DEL(__ptrs) MACSTART if ((__ptrs) != NULL) \
		DBGMSG1("warning: ptr not freed - %s\n", #__ptrs) MACEND
#else
	#define MMCHECK_DEL(__ptrs)
#endif

// Function: FloatEquals
//     Peform a reasonable "fuzzy" compare of two floating point numbers.
//  This relies on the IEEE bit representation of floating point numbers.
int FloatEquals(float f1, float f2);


// Function: PrintBits
//     Print the binary representation of the number in ASCII ('0' & '1') to a FILE stream.
void PrintBits(FILE *pfs, BitVector uBits);

#endif
