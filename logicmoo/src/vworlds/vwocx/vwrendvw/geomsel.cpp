// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include <d3drmwin.h>
#include "stdtools.h"
#include "geomsel.h"
#include <vwsutils.h>
#include <vwrender.h>

#include <propbase.h>
/*
CGeometrySelection::CGeometrySelection(IThing *pThing, IVWFrame *pVWFrame, IVWFrame *pBoxFrame)
{
	m_pThing = pThing;
	m_pVWFrame = pVWFrame;
	m_pBoxFrame = pBoxFrame;
}
*/

CGeometrySelection::CGeometrySelection(IThing *pThing, IDirect3DRMFrame* pD3DFrame, IDirect3DRMMeshBuilder *pBoxBuilder)
{
	m_pThing = pThing;
	m_pBoxBuilder = pBoxBuilder;
	m_pD3DFrame = pD3DFrame;

	m_pThing->AddRef();
	m_pBoxBuilder->AddRef();
	m_pD3DFrame->AddRef();
}

CGeometrySelection::~CGeometrySelection()
{
	SAFERELEASE(m_pThing);
	SAFERELEASE(m_pBoxBuilder);
	SAFERELEASE(m_pD3DFrame);
	//SAFERELEASE(m_pVWFrame);
	//SAFERELEASE(m_pBoxFrame);
}


//
// constructor
CGeometrySelectionList::CGeometrySelectionList()
{
}

CGeometrySelectionList::~CGeometrySelectionList()
{
	ClearList();
}

void CGeometrySelectionList::AddSelection(CGeometrySelection *pSel)
{
	m_list.AddTail(pSel);
}


void CGeometrySelectionList::RemoveSelection(IThing *pThing)
{
	POSITION pos;
	POSITION oldpos;

	// iterate through material list, freeing up contents
	for( pos=m_list.GetHeadPosition(); pos!=0; )
	{
		CGeometrySelection *psel;
		
		oldpos = pos;

		psel = (CGeometrySelection *) m_list.GetNext(pos);
		ASSERT( psel != NULL );

		if (psel->m_pThing == pThing)
		{
			m_list.RemoveAt(oldpos);
			if (psel != NULL)
				delete psel;
			return;
		}
	}
}

CGeometrySelection *CGeometrySelectionList::GetSelection(IThing *pThing)
{
	POSITION pos;

	// iterate through material list, freeing up contents
	for( pos=m_list.GetHeadPosition(); pos!=0; )
	{
		CGeometrySelection *psel;
		
		psel = (CGeometrySelection *) m_list.GetNext(pos);
		ASSERT( psel != NULL );

		if (psel->m_pThing == pThing)
		{
			return psel;
		}
	}

	return NULL;
}

void CGeometrySelectionList::ClearList()
{
	POSITION pos;

	// iterate through material list, freeing up contents
	for( pos=m_list.GetHeadPosition(); pos!=0; )
	{
		CGeometrySelection *psel;
		
		psel = (CGeometrySelection *) m_list.GetNext(pos);
		ASSERT( psel != NULL );
		if (psel != NULL)
			delete psel;
	}

	m_list.RemoveAll();
}
