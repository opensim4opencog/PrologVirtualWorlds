// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxctl.h>         // MFC support for ActiveX Controls
#include <atlbase.h>
#include <vwobject.h>
#include <propbase.h>
#include <vwmenu.h>
#include <menuitem.h>

//#ifndef VWSYSTEM_BUILD
//#define IID_DEFINED
//#include "menuitem_i.c"
//#endif

HRESULT CVWMenu::CreatePopupFromPropList(IPropertyList* pListMenu, WPARAM* pwID)
{
	HRESULT			hr = S_OK;
	int				iItem;
	IMenuItem*		pItem = NULL;
	BOOL			bEmpty = TRUE;
	CVWMenu* pSubMenu;

	// build the menu
	VERIFY(CreatePopupMenu());

	// add items
	long cItem;
	hr = pListMenu->get_Count(&cItem);
	if (FAILED(hr))
		goto ERROR_CONDITION;

	// Calculate IDs
	m_wFirstID = *pwID;
	*pwID += cItem;
	m_wLastID = *pwID - 1;

	// Delete any sub-menus
	while (!m_subMenus.IsEmpty())
	{
		pSubMenu = m_subMenus.RemoveHead();
		pSubMenu->DestroyMenu();
		delete pSubMenu;
	}

	// Iterate through the menu items
	for (iItem=0; iItem<cItem; iItem++) {

		// get item from list
        hr = pListMenu->get_ObjectPropertyExt(iItem, IID_IMenuItem, (IObjectProperty**)&pItem);
		if (FAILED(hr))
			goto ERROR_CONDITION;

		// get info
		long lFlagsProp;
		hr = pItem->get_Flags(&lFlagsProp);
		if (FAILED(hr))
			goto ERROR_CONDITION;

		if (!(lFlagsProp & MI_INVISIBLE))
		{
			BSTR bstrText;
			CVWComPtr<IPropertyList, NULL,&IID_IPropertyList> pSubMenuList;

			hr = pItem->get_MenuText(&bstrText);
			if (FAILED(hr))
				goto ERROR_CONDITION;

			// munge data
			CString strMenuText(bstrText);
			::SysFreeString(bstrText);

			// truncate really long items...
			if (strMenuText.GetLength() > 40 )
			{
				try
				{
					strMenuText = strMenuText.Left(40);
				}
				catch ( CMemoryException e)
				{
				}

				int index = strMenuText.GetLength()-1;
				for(int i=0;i<3;i++,index--)
				{
					strMenuText.SetAt(index,'.');
				}
			}
			UINT nflags;
			if (lFlagsProp & MI_SEPARATOR)
				nflags =  MF_SEPARATOR;
			else {
				nflags  = MF_STRING;
				nflags |= ((lFlagsProp & MI_GRAYED) ? MF_GRAYED : 0);
				nflags |= ((lFlagsProp & MI_DISABLED) ? MF_DISABLED : 0);
				nflags |= ((lFlagsProp & MI_CHECKED) ? MF_CHECKED : 0);
			}

			// Either add a regular menu or create a pop-up sub-menu
			hr = pItem->get_SubMenu(&pSubMenuList.p);
			if (FAILED(hr))
				goto ERROR_CONDITION;
			if ((pSubMenuList != NULL) &&
			    (lFlagsProp & MI_POPUP))
			{
				// add a sub-menu
				CVWMenu* pSubMenu = new CVWMenu;
				if (pSubMenu == NULL)
				{
					hr = E_OUTOFMEMORY;
					goto ERROR_CONDITION;
				}
				// Create the menu
				if (SUCCEEDED(pSubMenu->CreatePopupFromPropList(pSubMenuList, pwID)))
				{
					// Add it into the sub-menu list
					try
					{
						m_subMenus.AddHead(pSubMenu);
					}
					catch(CException*)
					{
						hr = E_OUTOFMEMORY;
						goto ERROR_CONDITION;
					}

					// Get the HMENU
					HMENU hm = pSubMenu->GetSafeHmenu();
					if (hm == NULL)
					{
						hr = E_OUTOFMEMORY;
						goto ERROR_CONDITION;
					}
					// Add in the sub-menu
					VERIFY(AppendMenu(nflags | MF_POPUP, (UINT) hm, strMenuText));
				}
				else
				{
					// Add a disabled menu item since the sub-menu didn't create properly
					VERIFY(AppendMenu(MF_GRAYED, 0, strMenuText));
				}
			}
			else
			{
				// add a regular item
				VERIFY(AppendMenu(nflags, m_wFirstID+iItem, strMenuText));
			}

			// set flag
			bEmpty = FALSE;

		}

		// release stuff
		SAFERELEASE(pItem);
	}

	// Remember the item list
	SAFERELEASE( m_pListMenu );
	m_pListMenu = pListMenu;
	SAFEADDREF(m_pListMenu);

ERROR_CONDITION:

	SAFERELEASE(pItem);

	return bEmpty ? E_FAIL : hr;
}

BOOL CVWMenu::DestroyMenu()
{
	ReleaseAll();
	return CMenu::DestroyMenu();
}

void CVWMenu::ReleaseAll()
{
	CVWMenu* pSubMenu;

	m_wFirstID = m_wLastID = 0;

	// Delete any sub-menus
	while (!m_subMenus.IsEmpty())
	{
		pSubMenu = m_subMenus.RemoveHead();
		pSubMenu->DestroyMenu();
		delete pSubMenu;
	}

	// Release m_pListMenu
	SAFERELEASE(m_pListMenu);
}

BOOL CVWMenu::OnCommandRange(WPARAM wID, HRESULT* hr)
{
	BOOL bHandledIt = FALSE;
	long cItem;
 	IMenuItem *	pItem = NULL;

	// Assume we succeed
	*hr = S_OK;

	// Return FALSE immediately if menu not built.
	if (m_wFirstID == 0 && m_wLastID == 0)
		return FALSE;

	// assure command in range
	if ((wID < m_wFirstID) || (wID > m_wLastID))
	{
		// If it wasn't for us, try our sub-menus
		POSITION pos = m_subMenus.GetHeadPosition();
		while (pos != NULL)
		{
			// Get the next sub-menu
			CVWMenu* pMenu = m_subMenus.GetNext(pos);
			// If the command was handled OK, we're done
			if (pMenu->OnCommandRange(wID, hr))
				return TRUE;
		}

		// None of our sub-menus knew what to do with it
		return FALSE;
	}

	// normalize
	wID -= m_wFirstID;

	*hr = m_pListMenu->get_Count(&cItem);
	if (FAILED(*hr))
		goto ERROR_CONDITION;

	ASSERT((long)wID < cItem);

	// get item from list
	*hr = m_pListMenu->get_ObjectPropertyExt((long) wID, IID_IMenuItem, (IObjectProperty**)&pItem);
	if (FAILED(*hr))
		goto ERROR_CONDITION;

	// now tell the item to run itself
	*hr = pItem->Execute();

	// We handled it
	bHandledIt = TRUE;

ERROR_CONDITION:
	SAFERELEASE(pItem);

	return bHandledIt;
}


BOOL CVWMenu::OnCommandRangeUI(CCmdUI* pCmdUI, HRESULT* hr)
{
	BOOL bHandledIt = FALSE;
	long cItem, lFlagsProp, nflags = 0;
 	IMenuItem *	pItem = NULL;
	WPARAM wID = pCmdUI->m_nID;

	// Assume we succeed
	*hr = S_OK;

	// Return FALSE immediately if menu not built.
	if (m_wFirstID == 0 && m_wLastID == 0)
		return FALSE;

	// assure command in range
	if ((wID < m_wFirstID) || (wID > m_wLastID))
	{
		// If it wasn't for us, try our sub-menus
		POSITION pos = m_subMenus.GetHeadPosition();
		while (pos != NULL)
		{
			// Get the next sub-menu
			CVWMenu* pMenu = m_subMenus.GetNext(pos);
			// If the command was handled OK, we're done
			if (pMenu->OnCommandRangeUI(pCmdUI, hr))
				return TRUE;
		}

		// None of our sub-menus knew what to do with it
		return FALSE;
	}

	// normalize
	wID -= m_wFirstID;

	*hr = m_pListMenu->get_Count(&cItem);
	if (FAILED(*hr))
		goto ERROR_CONDITION;

	ASSERT((long)wID < cItem);

	// get item from list
	*hr = m_pListMenu->get_ObjectPropertyExt((long) wID, IID_IMenuItem, (IObjectProperty**)&pItem);
	if (FAILED(*hr))
		goto ERROR_CONDITION;

	// now set the menuitem flags
	*hr = pItem->get_Flags( &lFlagsProp );
	nflags |= ((lFlagsProp & MI_GRAYED) ? MF_GRAYED : 0);
	nflags |= ((lFlagsProp & MI_DISABLED) ? MF_DISABLED : MF_ENABLED);
	// Have to do this in case its a CTestCmdUI which overrides Enable() and will have
	// a NULL m_pMenu.
	if (pCmdUI->m_pMenu)
	{
		pCmdUI->m_pMenu->EnableMenuItem(pCmdUI->m_nIndex, MF_BYPOSITION | nflags);
		pCmdUI->m_bEnableChanged = TRUE;
	}
	else
		pCmdUI->Enable(!(nflags & MF_DISABLED));
	pCmdUI->SetCheck( ( lFlagsProp & MI_CHECKED ) ? 1 : 0 );

	// We handled it
	bHandledIt = TRUE;

ERROR_CONDITION:
	SAFERELEASE(pItem);

	return bHandledIt;
}
