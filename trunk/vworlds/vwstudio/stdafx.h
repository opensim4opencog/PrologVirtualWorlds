// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__06E3A0F8_958F_11D0_9543_00C04FD91F4D__INCLUDED_)
#define AFX_STDAFX_H__06E3A0F8_958F_11D0_9543_00C04FD91F4D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define STRICT

#include <afxwin.h>
#include <afxdisp.h>

#define _WIN32_WINNT 0x0400
#define _ATL_APARTMENT_THREADED
#define VC_EXTRALEAN

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#include <d3drm.h>
#include <d2d.h>
#include <stdtools.h>
#include <geomtool.h>
//#include <popup.h>
//#include <popupmgr.h>
#include <webtools.h>

#endif // !defined(AFX_STDAFX_H__06E3A0F8_958F_11D0_9543_00C04FD91F4D__INCLUDED)
