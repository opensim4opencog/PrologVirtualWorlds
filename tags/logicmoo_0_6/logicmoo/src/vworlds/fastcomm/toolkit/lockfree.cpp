// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

//**************************************************************************************
// Project:		FastComm
// Module:		lockfree.cpp
// Description:	defines system allocator.
// Author:		CH
// Create:		10/28/99              
//--------------------------------------------------------------------------------------

//#include "stdafx.h"
#include "lockfree.h"

CSystemAllocator CLockFreeStackPool::CStackPoolNode::m_SystemAllocator;
