// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _DispList_h
#define _DispList_h

// File:	DispList.h
// Author:	MM    (MM)
//
// Description:
//    This is the 2D display list structure which temporarily stores
//  information for image blts.
// 
// History:
// -@- 11/18/96 (MM) created

class CDisplayDesc {
public:
							CDisplayDesc();
							~CDisplayDesc();

	LPDIRECT2DRMIMAGE		m_pImage;
	D2DRect					m_rSrcExtent;
	D2DRect					m_rDstExtent;
	D2DTransform			m_t2d;

	CDisplayDesc *			m_pNext;
};
typedef CDisplayDesc *LPDISPLAYDESC;

inline 
CDisplayDesc::CDisplayDesc()
{
	memset(this, 0, sizeof(*this));
}

inline
CDisplayDesc::~CDisplayDesc()
{
	MMASSERT(m_pImage);
	m_pImage->Release();
}

class CDisplayList {
public:
					CDisplayList();
					~CDisplayList();

	void			Ins(LPDISPLAYDESC pDisplayItem);
	HRESULT			Clip(const D2DRect &rClip);
	HRESULT			BltToSurface(LPDIRECTDRAWSURFACE pdds, DWORD dwFlags) const;
					
	LPDISPLAYDESC	m_pFirst;
};

inline
CDisplayList::CDisplayList()
{
	m_pFirst = NULL;
}

inline
CDisplayList::~CDisplayList()
{
	while (m_pFirst) {
		LPDISPLAYDESC pTmp = m_pFirst->m_pNext;
		delete m_pFirst;
		m_pFirst = pTmp;
	}
}

inline void
CDisplayList::Ins(LPDISPLAYDESC pDisplayItem)
{
	MMASSERT(pDisplayItem);

	pDisplayItem->m_pNext = m_pFirst;
	m_pFirst = pDisplayItem;
}


#endif
