// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// File:	DispList.cpp
// Author:	MM    (MM)

#include "stdafx.h"
#include "DispList.h"


// Function: Clip
//    Take the raw display list and clip it to a rectangle.
HRESULT
CDisplayList::Clip(const D2DRect &rClip)
{
	LPDISPLAYDESC pItem;
	LPDISPLAYDESC *ppPointerToItem;
	
	//
	// examine each item in display list
	//
	pItem = *(ppPointerToItem = &m_pFirst);
	for (; pItem; ) {
		if (pItem->m_t2d.TransformClip(rClip, &(pItem->m_rSrcExtent), &(pItem->m_rDstExtent))) {
			// no intersection -- remove item
			*ppPointerToItem = pItem->m_pNext;
			delete pItem;
			pItem = *ppPointerToItem;
		} else {
			pItem = *(ppPointerToItem = &(pItem->m_pNext));
		}
	}

	return S_OK;
}

// Function: BltToSurface
//    Blt the display list to a DirectDrawSurface.
HRESULT
CDisplayList::BltToSurface(LPDIRECTDRAWSURFACE pdds, DWORD dwFlags) const
{
	HRESULT hr;
	MMASSERT(pdds);

	for (LPDISPLAYDESC pItem = m_pFirst; pItem; pItem = pItem->m_pNext) {
		// REVIEW: this should do something sensible on failure
		//   -- like try again on a busy bltter
		if (FAILED(hr = pItem->m_pImage->BltToSurface(
			&(pItem->m_rSrcExtent), pdds, &(pItem->m_rDstExtent), dwFlags)))
			return hr;
	}
	return S_OK;
}
