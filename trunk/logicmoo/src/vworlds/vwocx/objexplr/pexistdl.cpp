// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// PExistDl.cpp : implementation file
//

#include "stdafx.h"
#include "objexplr.h"
#include "PExistDl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropExistsDlog dialog


CPropExistsDlog::CPropExistsDlog(CWnd* pParent /*=NULL*/)
	: CDialog(CPropExistsDlog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropExistsDlog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropExistsDlog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropExistsDlog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropExistsDlog, CDialog)
	//{{AFX_MSG_MAP(CPropExistsDlog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropExistsDlog message handlers
