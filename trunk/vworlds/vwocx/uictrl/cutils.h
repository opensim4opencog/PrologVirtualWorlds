// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _VWCONT_UTILS_
#define _VWCONT_UTILS_

#include <vwobject.h>
#include <afxtempl.h>
#include <propbase.h>

typedef CVWComPtr<IThing, NULL, &IID_IThing> CThingPtr;
typedef CVWComPtr<IPropertyList, NULL,&IID_IPropertyList> CPropertyListPtr;

typedef IThing* PTHING;

#define JUMPONFAIL( hr, label ) \
	if (FAILED(hr)) goto label;


HRESULT AppendSubMenu( IWorld * pWorld, BSTR bstrName, IPropertyList *pMenu, IPropertyList *pSubMenu );
HRESULT AppendSeparator(IWorld * pWorld, IPropertyList * pList );



#endif