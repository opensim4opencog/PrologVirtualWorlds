// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#include <afxwin.h>
#include <afxdisp.h>

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

#ifndef ERRMGR_NAME
#define ERRMGR_NAME g_emD2D
#endif
#include <D2DUtils.h>
#include <MMImage.h>
#include "d2d.h"
#include "D2DTrans.h"
#include "DispList.h"
#include "d2dprv.h"

extern ErrMgr g_emD2D;
