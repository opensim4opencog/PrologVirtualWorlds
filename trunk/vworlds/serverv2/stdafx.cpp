// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// stdafx.cpp : source file that includes just the standard includes
//	ServerV2.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


#include <atlimpl.cpp>
#include "..\vwcommon\vwimpl.cpp"

CComModule	_Module;

CString str(long in)
{
	CString str;

	str.Format("%d", in);

	return str;
}