// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// MExistDl.cpp : implementation file
//

#include "stdafx.h"
#include "objexplr.h"
#include "MExistDl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMethExistsDlog dialog


CMethExistsDlog::CMethExistsDlog(CWnd* pParent /*=NULL*/)
	: CDialog(CMethExistsDlog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMethExistsDlog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMethExistsDlog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMethExistsDlog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMethExistsDlog, CDialog)
	//{{AFX_MSG_MAP(CMethExistsDlog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMethExistsDlog message handlers
