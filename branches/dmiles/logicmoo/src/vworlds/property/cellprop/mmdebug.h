// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _MMDebug_h
#define _MMDebug_h

// File:	MMDebug.h
// Author:	MM
//
// History:
// -@- 1/25/96 (mm) created
// -@- 1/25/96 (mm)
//     extracted some MMDebug code from Utils.h in the InterVRS project
// -@- 4/16/96 (mm)
//     afx'ified - removed _stall && MMCERTIFY

#include "MMUtils.h"


// afx version
#define MMASSERT(exp) ASSERT(exp)
#define MMVERIFY(exp) VERIFY(exp)
#define MMDEBUG(exp) DEBUG_ONLY(exp)

#define DBGMSG0(parm1) TRACE0(parm1)
#define DBGMSG1(parm1, parm2) TRACE1(parm1, parm2)
#define DBGMSG2(parm1, parm2, parm3) TRACE2(parm1, parm2, parm3)


// Macro: CHECK_MEM
//    Verify memory was actually allocated to pointer
//#define CHECK_MEM(p) MMCERTIFY(p)
#ifdef _DEBUG
#define MMCHECK_MEM(p) do { if ((p) == NULL) { DBGMSG0("error: out of memory"); exit(1); }} while (0)
#else
#define MMCHECK_MEM(p) do { if ((p) == NULL) { fprintf(stderr, "error: out of memory"); exit(1); }} while (0)
#endif

// Macro: CHECK_ERROR
//    Checks if "exp" evaluates to FALSE, and if so, propagates FALSE 
//  up the execution stack.
#define MMCHECK_ERROR(exp) do {\
	if ((exp) == FALSE) return FALSE; } while (0)

// Macro: CHECK_NULL
//    Checks if "exp" evaluates to NULL, and if so, propagates FALSE
//  up the execution stack after printing error msg.
#define MMCHECK_NULL(__exp, __szErrorMsg) do {\
	if ((__exp) == NULL) {\
		DBGMSG1("warning: %s\n", __szErrorMsg);\
		return FALSE;}} while (0)

#ifdef _DEBUG
#define MMDELETE(__ptr) do { delete (__ptr); (__ptr) = NULL; } while (0)
#define MMCHECK_DEL(__ptrs) do { if ((__ptrs) != NULL) \
	DBGMSG1("warning: ptr not freed - %s\n", #__ptrs) } while (0)
#else
#define MMDELETE(__ptr) do { delete __ptr; } while (0)
#define MMCHECK_DEL(__ptrs)
#endif

#endif
