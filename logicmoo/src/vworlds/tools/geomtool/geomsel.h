// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef __GEOMSEL_H_
#define __GEOMSEL_H_

#include <afxtempl.h>
#include <propbase.h>

class CTranslate3DObject : public CObject
{
public:

	CTranslate3DObject(){m_pTrans = NULL; m_pThing = NULL; m_pVWFrame = NULL; };
	~CTranslate3DObject() {SAFERELEASE(m_pTrans); SAFERELEASE(m_pThing); SAFERELEASE(m_pVWFrame); };

	CTranslate3DObject( IVWTransform * pTrans);
	CTranslate3DObject( IThing * pThing, IVWTransform *pTrans );
	CTranslate3DObject( IThing * pThing, IVWTransform *pTrans, IVWFrame* pVWFrame );
	IVWTransform * m_pTrans;
	IThing * m_pThing;

	D3DVECTOR	firstLocation;
	D3DVECTOR	currentLocation;
	IVWFrame* m_pVWFrame;
};

typedef CTypedPtrList<CObList, CTranslate3DObject*> CTransform3DList;

//C helper to delete and remove the elements of the typed ptr list
void DeletePtrListElements(CTransform3DList* pTransList);

#endif