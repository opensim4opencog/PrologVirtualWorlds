// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include <stdafx.h>
#include <vwobject.h>
#include <vwmenu.h>
#include "cutils.h"
#include <menuitem.h>
#include "syshelp.h"

HRESULT AppendSeparator(IWorld * pWorld, IPropertyList * pList )
{
HRESULT hr = S_OK;
IMenuItem *pSubMI = NULL;
	
	// Add The Separator
	hr = CreateMenuSeparator(pWorld, NULL, &pSubMI);
	JUMPONFAIL(hr,ERROR_ENCOUNTERED);

	// Add it to the list
	hr = pList->AddObjectProperty(pSubMI);

ERROR_ENCOUNTERED:

	SAFERELEASE( pSubMI );
	return hr;

}


HRESULT AppendSubMenu( IWorld * pWorld, BSTR bstrName, IPropertyList *pMenu, IPropertyList *pSubMenu )
{
	HRESULT hr = S_OK;
	IMenuItem *pSubMI = NULL;
	
	// Create the menu item
	hr = CreateMenuItemExt(pWorld, bstrName, MI_POPUP, NULL, NULL, NULL, NULL, &pSubMI);
	JUMPONFAIL(hr,ERROR_ENCOUNTERED);

	// Fill in the subMenu ptr
	hr = pSubMI->put_SubMenu(pSubMenu);
	JUMPONFAIL(hr,ERROR_ENCOUNTERED);

	hr = pMenu->AddObjectProperty(pSubMI);

ERROR_ENCOUNTERED:

	SAFERELEASE(pSubMI);
	return hr;
}

