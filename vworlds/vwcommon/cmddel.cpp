// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// CmdDel.cpp : implementation file
//


#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxctl.h>         // MFC support for OLE Controls
#include "CmdDel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdDelegate

IMPLEMENT_DYNCREATE(CCmdDelegate, CCmdTarget)

CCmdDelegate::CCmdDelegate()
{
	EnableAutomation();
}

CCmdDelegate::~CCmdDelegate()
{
}


void CCmdDelegate::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CCmdDelegate, CCmdTarget)
	//{{AFX_MSG_MAP(CCmdDelegate)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CCmdDelegate, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CCmdDelegate)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ICmdDelegate to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {9BF8A471-1543-11D0-89A9-00A0C9054129}
static const IID IID_ICmdDelegate =
{ 0x9bf8a471, 0x1543, 0x11d0, { 0x89, 0xa9, 0x0, 0xa0, 0xc9, 0x5, 0x41, 0x29 } };

BEGIN_INTERFACE_MAP(CCmdDelegate, CCmdTarget)
	INTERFACE_PART(CCmdDelegate, IID_ICmdDelegate, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdDelegate message handlers
