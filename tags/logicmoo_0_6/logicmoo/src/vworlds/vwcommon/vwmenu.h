// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.


#ifndef _VWMENU_
#define _VWMENU_

#include <vwprop.h>
#include <afxtempl.h>
#include <propbase.h>


/////////////////////////////////////////////////////////////////////////////
// CVWMenu -- utility class to track and handle VW menus and sub-menus

class CVWMenu : public CMenu
{
public:
	CVWMenu() : m_pListMenu(NULL), m_wFirstID(0), m_wLastID(0) {};
	~CVWMenu() {ReleaseAll();};

	// Create a pop-up menu from a property list; start assigning IDs from *pwID,
	// and return the last ID + 1; return S_OK if all went well, error otherwise
	// (including if there were no visible items in the menu)
	HRESULT CreatePopupFromPropList(IPropertyList* pListMenu, WPARAM* pwID);
	BOOL DestroyMenu();
	// Process a command; invoke the right menu item method if it's within this
	// menu's ID range and return true
	BOOL OnCommandRange(WPARAM wID, HRESULT* hr);
	BOOL OnCommandRangeUI(CCmdUI* pCmdUI, HRESULT* hr);

	WPARAM m_wFirstID;								// ID for first menu item
	WPARAM m_wLastID;								// ID for last menu item

private:
	void ReleaseAll();

	IPropertyList* m_pListMenu;						// The menu item list itself
	CTypedPtrList<CObList, CVWMenu*> m_subMenus;	// Any sub-menus
};


#endif //_VWMENU_