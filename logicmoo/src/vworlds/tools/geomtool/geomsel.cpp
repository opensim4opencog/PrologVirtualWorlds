// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include "stdafx.h"
#include <d3drm.h>
#include <geomtool.h>
#include "geomsel.h"

CTranslate3DObject::CTranslate3DObject( IVWTransform * pTrans )

{
	HRESULT hr = S_OK;

	ASSERT( pTrans );
	m_pTrans = pTrans;
	pTrans->AddRef();
	m_pThing = NULL;
	m_pVWFrame = NULL;
}

CTranslate3DObject::CTranslate3DObject( IThing *pThing, IVWTransform * pTrans )

{
	HRESULT hr = S_OK;

	ASSERT( pTrans );
	ASSERT( pThing );
	m_pTrans = pTrans;
	pTrans->AddRef();
	m_pThing = pThing;
	pThing->AddRef();
	m_pVWFrame = NULL;
}

CTranslate3DObject::CTranslate3DObject( IThing *pThing, IVWTransform * pTrans, IVWFrame* pVWFrame  )

{
	HRESULT hr = S_OK;

	ASSERT( pTrans );
	ASSERT( pThing );
	ASSERT( pVWFrame );

	m_pTrans = pTrans;
	m_pTrans->AddRef();

	m_pThing = pThing;
	m_pThing->AddRef();

	m_pVWFrame = pVWFrame;
	m_pVWFrame->AddRef();
}

void DeletePtrListElements(CTransform3DList* pTransList)
{
	CTranslate3DObject *pCTrans;

	if (!pTransList->IsEmpty())
	{
		for( POSITION pos = pTransList->GetHeadPosition(); pos != NULL; )
		{
			pCTrans = pTransList->GetNext( pos );
			if(NULL != pCTrans)
				delete pCTrans;
		}

		pTransList->RemoveAll();
	}
}