// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

/////////////////////////////////////////////////////////////////////////////////////////
// Project:		FastComm
// Module:		tools.cpp
// Description:	implements user classes: CTimer, SmartPtr.
// Author:		CH
// Create:		10/28/1999
/////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "workq.cpp"
#include "lockfree.cpp"

CString str(ULONG in)
{
	CString str;

	str.Format("%u", in);

	return str;
}

CString str(void *p)
{
	CString str;

	str.Format("%x", p);

	return str;
}

CString str(double in)
{
	CString str;

	str.Format("%.3f", in);

	return str;
}

