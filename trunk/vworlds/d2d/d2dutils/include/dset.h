// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _DSet_h
#define _DSet_h

// File:	DSet.h
// Author:	MM	(MM)
//
// Description:
//     This class defines a template for dynamically sizable sets of
//  items.
//
// **Hungarian Prefix: ds
// 
// History:
// -@- 11/06/96 (MM) created

#ifndef _DArray_h
#include "DArray.h"
#endif

typedef unsigned int DSetHandle;

template <class Type>
class DSet : public DArray<Type> {
public:
					DSet();
					DSet(const DSet &ds);
					~DSet();

	// DATA ACCESS FUNCTIONS
	unsigned int	Items() const				{ return m_cItems; }
	HRESULT			Find(const Type &item, DSetHandle *pIndex) const;
	Type &			operator[](DSetHandle index);
	const Type &	operator[](DSetHandle index) const;

	// CONSTRUCTION FUNCTIONS
	DSet &			operator=(const DSet &ds);
	DSetHandle		Ins(Type &item);
	void			Del(DSetHandle i);			// preserves order
	void			DelFast(DSetHandle i);		// does NOT preserve order
	void			Compactify()				{ Shrink(m_cItems ? m_cItems : 1); }
	void			Reset();

protected:
	unsigned int	m_cItems;
};

#define MMTHISCLASS DSet
#define MMTHIS MMTHISCLASS<Type>
#define MMTEMPLATE template <class Type>

MMTEMPLATE inline void
MMTHIS::Reset()
{
	m_cItems = 0;
}

MMTEMPLATE inline
MMTHIS::MMTHISCLASS()
: DArray<Type>(1, NULL)
{
	m_cItems = 0;
}

MMTEMPLATE inline 
MMTHIS::~MMTHISCLASS()
{
}

MMTEMPLATE inline
MMTHIS::MMTHISCLASS(const DSet &ds)
: DArray<Type>(ds.cLength, ds.prgData)
{
	m_cItems = ds.m_cItems;
}

MMTEMPLATE inline HRESULT
MMTHIS::Find(const Type &item, DSetHandle *pIndex) const
{
	for (unsigned int i = 0; i < m_cItems; i++)
		if (item == prgData[i]) {
			*pIndex = i;
			return S_OK;
		}
	return E_FAIL;
}

// CONSTRUCTION FUNCTIONS
MMTEMPLATE inline DSetHandle
MMTHIS::Ins(Type &item)
{
	if (m_cItems == (unsigned int) cLength) {
		Grow(cLength << 1);
	}
	prgData[m_cItems++] = item;

	return m_cItems - 1;
}

MMTEMPLATE inline void
MMTHIS::Del(DSetHandle i)
{
	MMASSERT(INARRAY(i, m_cItems));

	m_cItems--;
	
	// preserve order on delete
	while (i < m_cItems) {
		prgData[i] = prgData[i+1];
		i++;
	}
}

MMTEMPLATE inline void
MMTHIS::DelFast(DSetHandle i)
{
	MMASSERT(INARRAY(i, m_cItems));
	m_cItems--;
	// swap last item into deleted item's slot
	if (i != m_cItems)
		prgData[i] = prgData[m_cItems];
}


MMTEMPLATE inline Type &
MMTHIS::operator[](DSetHandle i)
{
	MMASSERT(i < m_cItems);

	return prgData[i];
}

MMTEMPLATE inline const Type &
MMTHIS::operator[](DSetHandle i) const
{
	MMASSERT(i < m_cItems);

	return prgData[i];
}

MMTEMPLATE inline MMTHIS &
MMTHIS::operator=(const MMTHIS &ds)
{
	if (this != &ds) {
		MMASSERT(ds.cLength > 0);
		DArray<Type>::Set(ds.cLength, ds.prgData);
		m_cItems = ds.m_cItems;
	}
	return *this;
}

#undef MMTHISCLASS
#undef MMTHIS
#undef MMTEMPLATE

#endif