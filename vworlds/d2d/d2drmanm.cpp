// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// D2DRMAnm.cpp : Implementation of Cd2dApp and DLL registration.

#include "stdafx.h"
#include "D2DRMAnm.h"

/////////////////////////////////////////////////////////////////////////////
//
CDirect2DRMAnimation::CDirect2DRMAnimation()
{
	D2DOBJINIT();
}

CDirect2DRMAnimation::~CDirect2DRMAnimation()
{
	D2DOBJDESTROY();
}
