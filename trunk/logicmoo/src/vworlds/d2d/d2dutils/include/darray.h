// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _DArray_h
#define _DArray_h

// File:	DArray.h
// Author:	MM	(MM)
//
// Description:
//     This class defines a template for a dynamically sizable array with
//  bounds checking.
//
// **Hungarian Prefix: drg
// 
// History:
// -@- 4/16/96 (MM) created


#ifndef _MMUtils_h
#include "MMUtils.h"
#endif

typedef int ArrayIndex;
typedef int ArrayLength;


template <class Type>
class DArray {
public:
					DArray(const DArray &drgType);
					DArray(ArrayLength nLength = 0, const Type *prgType = NULL);
					~DArray();

	// DATA ACCESS FUNCTIONS
	ArrayLength		Length() const;
	Type &			operator[](ArrayIndex i);
	const Type &	operator[](ArrayIndex i) const;

	Type *			operator+(ArrayIndex i);
	const Type *	operator+(ArrayIndex i) const;

	DArray &		operator=(const DArray &drgType);
//	DArray &		operator=(const Type *prgType);		// use Fill instead

	Type *			Base();
	const Type *	Base() const;

	// CONSTRUCTION FUNCTIONS
	void			Set(ArrayLength nLength, const Type *prgType);
	void			Grow(ArrayLength nLength);
	void			SafeGrow(ArrayLength nLength, const Type &clearVal);
	void			Shrink(ArrayLength nLength);
	void			AssureIndex(ArrayIndex i);
	void			SafeAssureIndex(ArrayIndex i, const Type &clearVal);

	void			Fill(const Type *prgType);
protected:
	void			Create(ArrayLength nLength, const Type *prgType);
	void			ChangeSize(ArrayLength nLength, ArrayIndex nLimit);
	void			SafeChangeSize(ArrayLength nLength, ArrayIndex nLimit, const Type &clearVal);

protected:
	Type *			prgData;
	ArrayLength		cLength;
};


#define MMTHISCLASS DArray
#define MMTHIS MMTHISCLASS<Type>
#define MMTEMPLATE template <class Type>


MMTEMPLATE inline 
MMTHIS::~MMTHISCLASS()
{
//	cout << "Delete DArray: " << this << ' ' << prgData << endl;
	delete [] prgData;
	MMDEBUG_ONLY(prgData = NULL);
}

MMTEMPLATE inline void
MMTHIS::Fill(const Type *prgType)
{
//	cout << "Fill DArray: " << this << ' ' << prgData << endl;
	if (prgType)
		for (int i = 0; i < cLength; i++)
			prgData[i] = prgType[i];
}

MMTEMPLATE inline void
MMTHIS::Create(ArrayLength nLength, const Type *prgType)
{
//	cout << "Create DArray: " << this << ' ' << prgData << endl;
	MMASSERT(nLength >= 0);

	cLength = nLength;

	if (cLength) {
		// calls default constructors if Type is non-trivial
		MMASSERT(prgData == NULL);
		prgData = new Type[nLength];
		MMCERTIFY(prgData);

		Fill(prgType);
	} else {
		prgData = NULL;
	}
}

MMTEMPLATE inline void
MMTHIS::ChangeSize(ArrayLength nNewLength, ArrayIndex nLimit)
{
//	cout << "ChangeSize DArray: " << this << ' ' << prgData << endl;
	Type *pOldData = prgData;

	MMDEBUG_ONLY(prgData = NULL);
	MMTHIS::Create(nNewLength, NULL);

	for (int i = 0; i < nLimit; i++) {
		prgData[i] = pOldData[i];		// copy data
	}
	delete [] pOldData;					// free old memory
}

MMTEMPLATE inline void
MMTHIS::SafeChangeSize(ArrayLength nNewLength, ArrayIndex nLimit, const Type &clearVal) 
{
//	cout << "SafeChangeSize DArray: " << this << ' ' << prgData << endl;
	Type *pOldData = prgData;

	MMDEBUG_ONLY(prgData = NULL);
	MMTHIS::Create(nNewLength, NULL);

	for (int i = 0; i < nLimit; i++) {
		prgData[i] = pOldData[i];		// copy data
	}
	for (; i < nNewLength; i++) {		// initialize untouched slots
		prgData[i] = clearVal;
	}
	delete [] pOldData;					// free old memory
}

MMTEMPLATE inline void
MMTHIS::AssureIndex(ArrayIndex i) 
{
//	cout << "AssureIndex DArray: " << this << ' ' << prgData << endl;
	MMASSERT(i >= 0);

	// perform array doubling if the index is too small
	if (i >= cLength) {
		int nNewLength;
		if (cLength == 0)
			nNewLength = 1;
		else
			nNewLength = cLength << 1;
		ChangeSize(nNewLength, cLength);
	}
}

MMTEMPLATE inline void
MMTHIS::SafeAssureIndex(ArrayIndex i, const Type &clearVal) 
{
//	cout << "SafeAssureIndex DArray: " << this << ' ' << prgData << endl;
	MMASSERT(i >= 0);

	// perform array doubling if the index is too small
	if (i >= cLength) {
		int nNewLength;
		if (cLength == 0)
			nNewLength = 1;
		else
			nNewLength = cLength << 1;
		SafeChangeSize(nNewLength, cLength, clearVal);
	}
}

MMTEMPLATE inline void
MMTHIS::Set(ArrayLength nLength, const Type *prgType)
{
	MMTHIS::~MMTHISCLASS();		// delete previous contents
	Create(nLength, prgType);
}

MMTEMPLATE inline
MMTHIS::MMTHISCLASS(ArrayLength nLength, const Type *prgType)
{
	MMDEBUG_ONLY(prgData = NULL);
	Create(nLength, prgType);
}

MMTEMPLATE inline
MMTHIS::MMTHISCLASS(const DArray &array)
{
	MMDEBUG_ONLY(prgData = NULL);
	Create(array.Length(), array.Base());
}

MMTEMPLATE inline ArrayLength
MMTHIS::Length() const
{
	return cLength;
}

MMTEMPLATE inline Type &
MMTHIS::operator[](ArrayIndex i)
{
	MMASSERT(INARRAY(i, cLength));

	return prgData[i];
}

MMTEMPLATE inline const Type &
MMTHIS::operator[](ArrayIndex i) const
{
	MMASSERT(INARRAY(i, cLength));

	return prgData[i];
}

MMTEMPLATE inline Type *
MMTHIS::operator+(ArrayIndex i)
{
	return prgData + i;
}

MMTEMPLATE inline const Type *
MMTHIS::operator+(ArrayIndex i) const
{
	return prgData + i;
}

MMTEMPLATE inline MMTHIS &
MMTHIS::operator=(const MMTHIS &array)
{
	if (this != &array) 
		Set(array.Length(), array.Base());

	return *this;
}

// Function: operator=
//     This version of the function can be called if the array has
//  already been initialized with a length, and the previous elements
//  do not need to be deleted.
//MMTEMPLATE inline MMTHIS &
//MMTHIS::operator=(const Type *prgType) 
//{
//	Fill(prgType);
//
//	return *this;
//}

MMTEMPLATE inline Type *
MMTHIS::Base() 
{
	return prgData;
}

MMTEMPLATE inline const Type *
MMTHIS::Base() const 
{
	return prgData;
}


MMTEMPLATE inline void
MMTHIS::Grow(ArrayLength nLength) 
{
	if (nLength > cLength) 
		ChangeSize(nLength, cLength);
}

// Function: SafeGrow
//     Increase the size of the array and fill new slots with given value.
MMTEMPLATE inline void
MMTHIS::SafeGrow(ArrayLength nLength, const Type &clearVal) 
{
	if (nLength > cLength) 
		SafeChangeSize(nLength, cLength, clearVal);
}

MMTEMPLATE inline void
MMTHIS::Shrink(ArrayLength nLength) 
{
	if (nLength < cLength) 
		ChangeSize(nLength, nLength);
}


#undef MMTHISCLASS
#undef MMTHIS
#undef MMTEMPLATE

#endif
