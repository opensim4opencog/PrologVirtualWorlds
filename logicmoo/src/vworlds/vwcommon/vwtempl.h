// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef __VWTEMPL_H__
#define __VWTEMPL_H__

#ifndef __AFXPLEX_H__
	#include <afxplex_.h>
#endif

#ifdef _AFX_MINREBUILD
#pragma component(minrebuild, off)
#endif
#ifndef _AFX_FULLTYPEINFO
#pragma component(mintypeinfo, on)
#endif

#ifdef _AFX_PACKING
#pragma pack(push, _AFX_PACKING)
#endif

#ifndef ALL_WARNINGS
#pragma warning(disable: 4114)
#endif

/////////////////////////////////////////////////////////////
// Class to Adapt CComBSTR which have overloaded & operator.

template <class T>
class CBSTRAdapt
{
public:
	CBSTRAdapt()
	{
	}
	CBSTRAdapt(const T& rSrc)
	{
		m_T = rSrc;
	}

	CBSTRAdapt& operator=(const T& rSrc)
	{
		m_T = rSrc;
		return *this;
	}
	operator BSTR() const
	{
		return m_T.m_str;
	}

	T m_T;
};

/////////////////////////////////////////////////////////////////////////////
// CVWArray<TYPE, ARG_TYPE>

template<class TYPE, class ARG_TYPE>
class CVWArray : public CObject
{
public:
// Construction
	CVWArray();

// Attributes
	int GetSize() const;
	int GetUpperBound() const;
	void SetSize(int nNewSize, int nGrowBy = -1);

// Operations
	// Clean up
	void FreeExtra();
	void RemoveAll();

	// Accessing elements
	TYPE GetAt(int nIndex) const;
	void SetAt(int nIndex, ARG_TYPE newElement);
	TYPE& ElementAt(int nIndex);

	// Direct Access to the element data (may return NULL)
	const TYPE* GetData() const;
	TYPE* GetData();

	// Potentially growing the array
	void SetAtGrow(int nIndex, ARG_TYPE newElement);
	int Add(ARG_TYPE newElement);
	int Append(const CVWArray& src);
	void Copy(const CVWArray& src);

	// overloaded operator helpers
	TYPE operator[](int nIndex) const;
	TYPE& operator[](int nIndex);

	// Operations that move elements around
	void InsertAt(int nIndex, ARG_TYPE newElement, int nCount = 1);
	void RemoveAt(int nIndex, int nCount = 1);
	void InsertAt(int nStartIndex, CVWArray* pNewArray);

// Implementation
protected:
	TYPE* m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount

public:
	~CVWArray();
	void Serialize(CArchive&);
#ifdef _DEBUG
	void Dump(CDumpContext&) const;
	void AssertValid() const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CVWArray<TYPE, ARG_TYPE> inline functions

template<class TYPE, class ARG_TYPE>
inline int CVWArray<TYPE, ARG_TYPE>::GetSize() const
	{ return m_nSize; }
template<class TYPE, class ARG_TYPE>
inline int CVWArray<TYPE, ARG_TYPE>::GetUpperBound() const
	{ return m_nSize-1; }
template<class TYPE, class ARG_TYPE>
inline void CVWArray<TYPE, ARG_TYPE>::RemoveAll()
	{ SetSize(0, -1); }
template<class TYPE, class ARG_TYPE>
inline TYPE CVWArray<TYPE, ARG_TYPE>::GetAt(int nIndex) const
	{ ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
template<class TYPE, class ARG_TYPE>
inline void CVWArray<TYPE, ARG_TYPE>::SetAt(int nIndex, ARG_TYPE newElement)
	{ ASSERT(nIndex >= 0 && nIndex < m_nSize);
		m_pData[nIndex] = newElement; }
template<class TYPE, class ARG_TYPE>
inline TYPE& CVWArray<TYPE, ARG_TYPE>::ElementAt(int nIndex)
	{ ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
template<class TYPE, class ARG_TYPE>
inline const TYPE* CVWArray<TYPE, ARG_TYPE>::GetData() const
	{ return (const TYPE*)m_pData; }
template<class TYPE, class ARG_TYPE>
inline TYPE* CVWArray<TYPE, ARG_TYPE>::GetData()
	{ return (TYPE*)m_pData; }
template<class TYPE, class ARG_TYPE>
inline int CVWArray<TYPE, ARG_TYPE>::Add(ARG_TYPE newElement)
	{ int nIndex = m_nSize;
		SetAtGrow(nIndex, newElement);
		return nIndex; }
template<class TYPE, class ARG_TYPE>
inline TYPE CVWArray<TYPE, ARG_TYPE>::operator[](int nIndex) const
	{ return GetAt(nIndex); }
template<class TYPE, class ARG_TYPE>
inline TYPE& CVWArray<TYPE, ARG_TYPE>::operator[](int nIndex)
	{ return ElementAt(nIndex); }

/////////////////////////////////////////////////////////////////////////////
// CVWArray<TYPE, ARG_TYPE> out-of-line functions

template<class TYPE, class ARG_TYPE>
CVWArray<TYPE, ARG_TYPE>::CVWArray()
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

template<class TYPE, class ARG_TYPE>
CVWArray<TYPE, ARG_TYPE>::~CVWArray()
{
	ASSERT_VALID(this);

	if (m_pData != NULL)
	{
		DestructElements<TYPE>(m_pData, m_nSize);
		delete[] (BYTE*)m_pData;
	}
}

template<class TYPE, class ARG_TYPE>
void CVWArray<TYPE, ARG_TYPE>::SetSize(int nNewSize, int nGrowBy)
{
	ASSERT_VALID(this);
	ASSERT(nNewSize >= 0);

	if (nGrowBy != -1)
		m_nGrowBy = nGrowBy;  // set new size

	if (nNewSize == 0)
	{
		// shrink to nothing
		if (m_pData != NULL)
		{
			DestructElements<TYPE>(m_pData, m_nSize);
			delete[] (BYTE*)m_pData;
			m_pData = NULL;
		}
		m_nSize = m_nMaxSize = 0;
	}
	else if (m_pData == NULL)
	{
		// create one with exact size
#ifdef SIZE_T_MAX
		ASSERT(nNewSize <= SIZE_T_MAX/sizeof(TYPE));    // no overflow
#endif
		m_pData = (TYPE*) new BYTE[nNewSize * sizeof(TYPE)];
		ConstructElements<TYPE>(m_pData, nNewSize);
		m_nSize = m_nMaxSize = nNewSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
		{
			// initialize the new elements
			ConstructElements<TYPE>(&m_pData[m_nSize], nNewSize-m_nSize);
		}
		else if (m_nSize > nNewSize)
		{
			// destroy the old elements
			DestructElements<TYPE>(&m_pData[nNewSize], m_nSize-nNewSize);
		}
		m_nSize = nNewSize;
	}
	else
	{
		// otherwise, grow array
		int nGrowBy = m_nGrowBy;
		if (nGrowBy == 0)
		{
			// heuristically determine growth when nGrowBy == 0
			//  (this avoids heap fragmentation in many situations)
			nGrowBy = m_nSize / 8;
			nGrowBy = (nGrowBy < 4) ? 4 : ((nGrowBy > 1024) ? 1024 : nGrowBy);
		}
		int nNewMax;
		if (nNewSize < m_nMaxSize + nGrowBy)
			nNewMax = m_nMaxSize + nGrowBy;  // granularity
		else
			nNewMax = nNewSize;  // no slush

		ASSERT(nNewMax >= m_nMaxSize);  // no wrap around
#ifdef SIZE_T_MAX
		ASSERT(nNewMax <= SIZE_T_MAX/sizeof(TYPE)); // no overflow
#endif
		TYPE* pNewData = (TYPE*) new BYTE[nNewMax * sizeof(TYPE)];

		// copy new data from old
		memcpy(pNewData, m_pData, m_nSize * sizeof(TYPE));

		// construct remaining elements
		ASSERT(nNewSize > m_nSize);
		ConstructElements<TYPE>(&pNewData[m_nSize], nNewSize-m_nSize);

		// get rid of old stuff (note: no destructors called)
		delete[] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}

template<class TYPE, class ARG_TYPE>
int CVWArray<TYPE, ARG_TYPE>::Append(const CVWArray& src)
{
	ASSERT_VALID(this);
	ASSERT(this != &src);   // cannot append to itself

	int nOldSize = m_nSize;
	SetSize(m_nSize + src.m_nSize);
	CopyElements<TYPE>(m_pData + nOldSize, src.m_pData, src.m_nSize);
	return nOldSize;
}

template<class TYPE, class ARG_TYPE>
void CVWArray<TYPE, ARG_TYPE>::Copy(const CVWArray& src)
{
	ASSERT_VALID(this);
	ASSERT(this != &src);   // cannot append to itself

	SetSize(src.m_nSize);
	CopyElements<TYPE>(m_pData, src.m_pData, src.m_nSize);
}

template<class TYPE, class ARG_TYPE>
void CVWArray<TYPE, ARG_TYPE>::FreeExtra()
{
	ASSERT_VALID(this);

	if (m_nSize != m_nMaxSize)
	{
		// shrink to desired size
#ifdef SIZE_T_MAX
		ASSERT(m_nSize <= SIZE_T_MAX/sizeof(TYPE)); // no overflow
#endif
		TYPE* pNewData = NULL;
		if (m_nSize != 0)
		{
			pNewData = (TYPE*) new BYTE[m_nSize * sizeof(TYPE)];
			// copy new data from old
			memcpy(pNewData, m_pData, m_nSize * sizeof(TYPE));
		}

		// get rid of old stuff (note: no destructors called)
		delete[] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nMaxSize = m_nSize;
	}
}

template<class TYPE, class ARG_TYPE>
void CVWArray<TYPE, ARG_TYPE>::SetAtGrow(int nIndex, ARG_TYPE newElement)
{
	ASSERT_VALID(this);
	ASSERT(nIndex >= 0);

	if (nIndex >= m_nSize)
		SetSize(nIndex+1, -1);
	m_pData[nIndex] = newElement;
}

template<class TYPE, class ARG_TYPE>
void CVWArray<TYPE, ARG_TYPE>::InsertAt(int nIndex, ARG_TYPE newElement, int nCount /*=1*/)
{
	ASSERT_VALID(this);
	ASSERT(nIndex >= 0);    // will expand to meet need
	ASSERT(nCount > 0);     // zero or negative size not allowed

	if (nIndex >= m_nSize)
	{
		// adding after the end of the array
		SetSize(nIndex + nCount, -1);   // grow so nIndex is valid
	}
	else
	{
		// inserting in the middle of the array
		int nOldSize = m_nSize;
		SetSize(m_nSize + nCount, -1);  // grow it to new size
		// destroy intial data before copying over it
		DestructElements<TYPE>(&m_pData[nOldSize], nCount);
		// shift old data up to fill gap
		memmove(&m_pData[nIndex+nCount], &m_pData[nIndex],
			(nOldSize-nIndex) * sizeof(TYPE));

		// re-init slots we copied from
		ConstructElements<TYPE>(&m_pData[nIndex], nCount);
	}

	// insert new value in the gap
	ASSERT(nIndex + nCount <= m_nSize);
	while (nCount--)
		m_pData[nIndex++] = newElement;
}

template<class TYPE, class ARG_TYPE>
void CVWArray<TYPE, ARG_TYPE>::RemoveAt(int nIndex, int nCount)
{
	ASSERT_VALID(this);
	ASSERT(nIndex >= 0);
	ASSERT(nCount >= 0);
	ASSERT(nIndex + nCount <= m_nSize);

	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);
	DestructElements<TYPE>(&m_pData[nIndex], nCount);
	if (nMoveCount)
		memcpy(&m_pData[nIndex], &m_pData[nIndex + nCount],
			nMoveCount * sizeof(TYPE));
	m_nSize -= nCount;
}

template<class TYPE, class ARG_TYPE>
void CVWArray<TYPE, ARG_TYPE>::InsertAt(int nStartIndex, CVWArray* pNewArray)
{
	ASSERT_VALID(this);
	ASSERT(pNewArray != NULL);
	ASSERT_VALID(pNewArray);
	ASSERT(nStartIndex >= 0);

	if (pNewArray->GetSize() > 0)
	{
		InsertAt(nStartIndex, pNewArray->GetAt(0), pNewArray->GetSize());
		for (int i = 0; i < pNewArray->GetSize(); i++)
			SetAt(nStartIndex + i, pNewArray->GetAt(i));
	}
}

template<class TYPE, class ARG_TYPE>
void CVWArray<TYPE, ARG_TYPE>::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	CObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar.WriteCount(m_nSize);
	}
	else
	{
		DWORD nOldSize = ar.ReadCount();
		SetSize(nOldSize, -1);
	}
	SerializeElements<TYPE>(ar, m_pData, m_nSize);
}

#ifdef _DEBUG
template<class TYPE, class ARG_TYPE>
void CVWArray<TYPE, ARG_TYPE>::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);

	dc << "with " << m_nSize << " elements";
	if (dc.GetDepth() > 0)
	{
		dc << "\n";
		DumpElements<TYPE>(dc, m_pData, m_nSize);
	}

	dc << "\n";
}

template<class TYPE, class ARG_TYPE>
void CVWArray<TYPE, ARG_TYPE>::AssertValid() const
{
	CObject::AssertValid();

	if (m_pData == NULL)
	{
		ASSERT(m_nSize == 0);
		ASSERT(m_nMaxSize == 0);
	}
	else
	{
		ASSERT(m_nSize >= 0);
		ASSERT(m_nMaxSize >= 0);
		ASSERT(m_nSize <= m_nMaxSize);
		ASSERT(AfxIsValidAddress(m_pData, m_nMaxSize * sizeof(TYPE)));
	}
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVWList<TYPE, ARG_TYPE>

template<class TYPE, class ARG_TYPE>
class CVWList : public CObject
{
protected:
	struct CVWNode
	{
		CVWNode* pNext;
		CVWNode* pPrev;
		TYPE data;
	};
public:
// Construction
	CVWList(int nBlockSize = 10);

// Attributes (head and tail)
	// count of elements
	int GetCount() const;
	BOOL IsEmpty() const;

	// peek at head or tail
	TYPE& GetHead();
	TYPE GetHead() const;
	TYPE& GetTail();
	TYPE GetTail() const;

// Operations
	// get head or tail (and remove it) - don't call on empty list !
	TYPE RemoveHead();
	TYPE RemoveTail();

	// add before head or after tail
	POSITION AddHead(ARG_TYPE newElement);
	POSITION AddTail(ARG_TYPE newElement);

	// add another list of elements before head or after tail
	void AddHead(CVWList* pNewList);
	void AddTail(CVWList* pNewList);

	// remove all elements
	void RemoveAll();

	// iteration
	POSITION GetHeadPosition() const;
	POSITION GetTailPosition() const;
	TYPE& GetNext(POSITION& rPosition); // return *Position++
	TYPE GetNext(POSITION& rPosition) const; // return *Position++
	TYPE& GetPrev(POSITION& rPosition); // return *Position--
	TYPE GetPrev(POSITION& rPosition) const; // return *Position--

	// getting/modifying an element at a given position
	TYPE& GetAt(POSITION position);
	TYPE GetAt(POSITION position) const;
	void SetAt(POSITION pos, ARG_TYPE newElement);
	void RemoveAt(POSITION position);

	// inserting before or after a given position
	POSITION InsertBefore(POSITION position, ARG_TYPE newElement);
	POSITION InsertAfter(POSITION position, ARG_TYPE newElement);

	// helper functions (note: O(n) speed)
	POSITION Find(ARG_TYPE searchValue, POSITION startAfter = NULL) const;
		// defaults to starting at the HEAD, return NULL if not found
	POSITION FindIndex(int nIndex) const;
		// get the 'nIndex'th element (may return NULL)

// Implementation
protected:
	CVWNode* m_pNodeHead;
	CVWNode* m_pNodeTail;
	int m_nCount;
	CVWNode* m_pNodeFree;
	struct CPlex* m_pBlocks;
	int m_nBlockSize;

	CVWNode* NewNode(CVWNode*, CVWNode*);
	void FreeNode(CVWNode*);

public:
	~CVWList();
	void Serialize(CArchive&);
#ifdef _DEBUG
	void Dump(CDumpContext&) const;
	void AssertValid() const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CVWList<TYPE, ARG_TYPE> inline functions

template<class TYPE, class ARG_TYPE>
inline int CVWList<TYPE, ARG_TYPE>::GetCount() const
	{ return m_nCount; }
template<class TYPE, class ARG_TYPE>
inline BOOL CVWList<TYPE, ARG_TYPE>::IsEmpty() const
	{ return m_nCount == 0; }
template<class TYPE, class ARG_TYPE>
inline TYPE& CVWList<TYPE, ARG_TYPE>::GetHead()
	{ ASSERT(m_pNodeHead != NULL);
		return m_pNodeHead->data; }
template<class TYPE, class ARG_TYPE>
inline TYPE CVWList<TYPE, ARG_TYPE>::GetHead() const
	{ ASSERT(m_pNodeHead != NULL);
		return m_pNodeHead->data; }
template<class TYPE, class ARG_TYPE>
inline TYPE& CVWList<TYPE, ARG_TYPE>::GetTail()
	{ ASSERT(m_pNodeTail != NULL);
		return m_pNodeTail->data; }
template<class TYPE, class ARG_TYPE>
inline TYPE CVWList<TYPE, ARG_TYPE>::GetTail() const
	{ ASSERT(m_pNodeTail != NULL);
		return m_pNodeTail->data; }
template<class TYPE, class ARG_TYPE>
inline POSITION CVWList<TYPE, ARG_TYPE>::GetHeadPosition() const
	{ return (POSITION) m_pNodeHead; }
template<class TYPE, class ARG_TYPE>
inline POSITION CVWList<TYPE, ARG_TYPE>::GetTailPosition() const
	{ return (POSITION) m_pNodeTail; }
template<class TYPE, class ARG_TYPE>
inline TYPE& CVWList<TYPE, ARG_TYPE>::GetNext(POSITION& rPosition) // return *Position++
	{ CVWNode* pNode = (CVWNode*) rPosition;
		ASSERT(AfxIsValidAddress(pNode, sizeof(CVWNode)));
		rPosition = (POSITION) pNode->pNext;
		return pNode->data; }
template<class TYPE, class ARG_TYPE>
inline TYPE CVWList<TYPE, ARG_TYPE>::GetNext(POSITION& rPosition) const // return *Position++
	{ CVWNode* pNode = (CVWNode*) rPosition;
		ASSERT(AfxIsValidAddress(pNode, sizeof(CVWNode)));
		rPosition = (POSITION) pNode->pNext;
		return pNode->data; }
template<class TYPE, class ARG_TYPE>
inline TYPE& CVWList<TYPE, ARG_TYPE>::GetPrev(POSITION& rPosition) // return *Position--
	{ CVWNode* pNode = (CVWNode*) rPosition;
		ASSERT(AfxIsValidAddress(pNode, sizeof(CVWNode)));
		rPosition = (POSITION) pNode->pPrev;
		return pNode->data; }
template<class TYPE, class ARG_TYPE>
inline TYPE CVWList<TYPE, ARG_TYPE>::GetPrev(POSITION& rPosition) const // return *Position--
	{ CVWNode* pNode = (CVWNode*) rPosition;
		ASSERT(AfxIsValidAddress(pNode, sizeof(CVWNode)));
		rPosition = (POSITION) pNode->pPrev;
		return pNode->data; }
template<class TYPE, class ARG_TYPE>
inline TYPE& CVWList<TYPE, ARG_TYPE>::GetAt(POSITION position)
	{ CVWNode* pNode = (CVWNode*) position;
		ASSERT(AfxIsValidAddress(pNode, sizeof(CVWNode)));
		return pNode->data; }
template<class TYPE, class ARG_TYPE>
inline TYPE CVWList<TYPE, ARG_TYPE>::GetAt(POSITION position) const
	{ CVWNode* pNode = (CVWNode*) position;
		ASSERT(AfxIsValidAddress(pNode, sizeof(CVWNode)));
		return pNode->data; }
template<class TYPE, class ARG_TYPE>
inline void CVWList<TYPE, ARG_TYPE>::SetAt(POSITION pos, ARG_TYPE newElement)
	{ CVWNode* pNode = (CVWNode*) pos;
		ASSERT(AfxIsValidAddress(pNode, sizeof(CVWNode)));
		pNode->data = newElement; }

template<class TYPE, class ARG_TYPE>
CVWList<TYPE, ARG_TYPE>::CVWList(int nBlockSize)
{
	ASSERT(nBlockSize > 0);

	m_nCount = 0;
	m_pNodeHead = m_pNodeTail = m_pNodeFree = NULL;
	m_pBlocks = NULL;
	m_nBlockSize = nBlockSize;
}

template<class TYPE, class ARG_TYPE>
void CVWList<TYPE, ARG_TYPE>::RemoveAll()
{
	ASSERT_VALID(this);

	// destroy elements
	CVWNode* pNode;
	for (pNode = m_pNodeHead; pNode != NULL; pNode = pNode->pNext)
		DestructElements<TYPE>(&pNode->data, 1);

	m_nCount = 0;
	m_pNodeHead = m_pNodeTail = m_pNodeFree = NULL;
	m_pBlocks->FreeDataChain();
	m_pBlocks = NULL;
}

template<class TYPE, class ARG_TYPE>
CVWList<TYPE, ARG_TYPE>::~CVWList()
{
	RemoveAll();
	ASSERT(m_nCount == 0);
}

/////////////////////////////////////////////////////////////////////////////
// Node helpers
//
// Implementation note: CVWNode's are stored in CPlex blocks and
//  chained together. Free blocks are maintained in a singly linked list
//  using the 'pNext' member of CVWNode with 'm_pNodeFree' as the head.
//  Used blocks are maintained in a doubly linked list using both 'pNext'
//  and 'pPrev' as links and 'm_pNodeHead' and 'm_pNodeTail'
//   as the head/tail.
//
// We never free a CPlex block unless the List is destroyed or RemoveAll()
//  is used - so the total number of CPlex blocks may grow large depending
//  on the maximum past size of the list.
//

template<class TYPE, class ARG_TYPE>
CVWList<TYPE, ARG_TYPE>::CVWNode*
CVWList<TYPE, ARG_TYPE>::NewNode(CVWList::CVWNode* pPrev, CVWList::CVWNode* pNext)
{
	if (m_pNodeFree == NULL)
	{
		// add another block
		CPlex* pNewBlock = CPlex::Create(m_pBlocks, m_nBlockSize,
				 sizeof(CVWNode));

		// chain them into free list
		CVWNode* pNode = (CVWNode*) pNewBlock->data();
		// free in reverse order to make it easier to debug
		pNode += m_nBlockSize - 1;
		for (int i = m_nBlockSize-1; i >= 0; i--, pNode--)
		{
			pNode->pNext = m_pNodeFree;
			m_pNodeFree = pNode;
		}
	}
	ASSERT(m_pNodeFree != NULL);  // we must have something

	CVWList::CVWNode* pNode = m_pNodeFree;
	m_pNodeFree = m_pNodeFree->pNext;
	pNode->pPrev = pPrev;
	pNode->pNext = pNext;
	m_nCount++;
	ASSERT(m_nCount > 0);  // make sure we don't overflow

	ConstructElements<TYPE>(&pNode->data, 1);
	return pNode;
}

template<class TYPE, class ARG_TYPE>
void CVWList<TYPE, ARG_TYPE>::FreeNode(CVWList::CVWNode* pNode)
{
	DestructElements<TYPE>(&pNode->data, 1);
	pNode->pNext = m_pNodeFree;
	m_pNodeFree = pNode;
	m_nCount--;
	ASSERT(m_nCount >= 0);  // make sure we don't underflow

	// if no more elements, cleanup completely
	if (m_nCount == 0)
		RemoveAll();
}

template<class TYPE, class ARG_TYPE>
POSITION CVWList<TYPE, ARG_TYPE>::AddHead(ARG_TYPE newElement)
{
	ASSERT_VALID(this);

	CVWNode* pNewNode = NewNode(NULL, m_pNodeHead);
	pNewNode->data = newElement;
	if (m_pNodeHead != NULL)
		m_pNodeHead->pPrev = pNewNode;
	else
		m_pNodeTail = pNewNode;
	m_pNodeHead = pNewNode;
	return (POSITION) pNewNode;
}

template<class TYPE, class ARG_TYPE>
POSITION CVWList<TYPE, ARG_TYPE>::AddTail(ARG_TYPE newElement)
{
	ASSERT_VALID(this);

	CVWNode* pNewNode = NewNode(m_pNodeTail, NULL);
	pNewNode->data = newElement;
	if (m_pNodeTail != NULL)
		m_pNodeTail->pNext = pNewNode;
	else
		m_pNodeHead = pNewNode;
	m_pNodeTail = pNewNode;
	return (POSITION) pNewNode;
}

template<class TYPE, class ARG_TYPE>
void CVWList<TYPE, ARG_TYPE>::AddHead(CVWList* pNewList)
{
	ASSERT_VALID(this);

	ASSERT(pNewList != NULL);
	ASSERT_VALID(pNewList);

	// add a list of same elements to head (maintain order)
	POSITION pos = pNewList->GetTailPosition();
	while (pos != NULL)
		AddHead(pNewList->GetPrev(pos));
}

template<class TYPE, class ARG_TYPE>
void CVWList<TYPE, ARG_TYPE>::AddTail(CVWList* pNewList)
{
	ASSERT_VALID(this);
	ASSERT(pNewList != NULL);
	ASSERT_VALID(pNewList);

	// add a list of same elements
	POSITION pos = pNewList->GetHeadPosition();
	while (pos != NULL)
		AddTail(pNewList->GetNext(pos));
}

template<class TYPE, class ARG_TYPE>
TYPE CVWList<TYPE, ARG_TYPE>::RemoveHead()
{
	ASSERT_VALID(this);
	ASSERT(m_pNodeHead != NULL);  // don't call on empty list !!!
	ASSERT(AfxIsValidAddress(m_pNodeHead, sizeof(CVWNode)));

	CVWNode* pOldNode = m_pNodeHead;
	TYPE returnValue = pOldNode->data;

	m_pNodeHead = pOldNode->pNext;
	if (m_pNodeHead != NULL)
		m_pNodeHead->pPrev = NULL;
	else
		m_pNodeTail = NULL;
	FreeNode(pOldNode);
	return returnValue;
}

template<class TYPE, class ARG_TYPE>
TYPE CVWList<TYPE, ARG_TYPE>::RemoveTail()
{
	ASSERT_VALID(this);
	ASSERT(m_pNodeTail != NULL);  // don't call on empty list !!!
	ASSERT(AfxIsValidAddress(m_pNodeTail, sizeof(CVWNode)));

	CVWNode* pOldNode = m_pNodeTail;
	TYPE returnValue = pOldNode->data;

	m_pNodeTail = pOldNode->pPrev;
	if (m_pNodeTail != NULL)
		m_pNodeTail->pNext = NULL;
	else
		m_pNodeHead = NULL;
	FreeNode(pOldNode);
	return returnValue;
}

template<class TYPE, class ARG_TYPE>
POSITION CVWList<TYPE, ARG_TYPE>::InsertBefore(POSITION position, ARG_TYPE newElement)
{
	ASSERT_VALID(this);

	if (position == NULL)
		return AddHead(newElement); // insert before nothing -> head of the list

	// Insert it before position
	CVWNode* pOldNode = (CVWNode*) position;
	CVWNode* pNewNode = NewNode(pOldNode->pPrev, pOldNode);
	pNewNode->data = newElement;

	if (pOldNode->pPrev != NULL)
	{
		ASSERT(AfxIsValidAddress(pOldNode->pPrev, sizeof(CVWNode)));
		pOldNode->pPrev->pNext = pNewNode;
	}
	else
	{
		ASSERT(pOldNode == m_pNodeHead);
		m_pNodeHead = pNewNode;
	}
	pOldNode->pPrev = pNewNode;
	return (POSITION) pNewNode;
}

template<class TYPE, class ARG_TYPE>
POSITION CVWList<TYPE, ARG_TYPE>::InsertAfter(POSITION position, ARG_TYPE newElement)
{
	ASSERT_VALID(this);

	if (position == NULL)
		return AddTail(newElement); // insert after nothing -> tail of the list

	// Insert it before position
	CVWNode* pOldNode = (CVWNode*) position;
	ASSERT(AfxIsValidAddress(pOldNode, sizeof(CVWNode)));
	CVWNode* pNewNode = NewNode(pOldNode, pOldNode->pNext);
	pNewNode->data = newElement;

	if (pOldNode->pNext != NULL)
	{
		ASSERT(AfxIsValidAddress(pOldNode->pNext, sizeof(CVWNode)));
		pOldNode->pNext->pPrev = pNewNode;
	}
	else
	{
		ASSERT(pOldNode == m_pNodeTail);
		m_pNodeTail = pNewNode;
	}
	pOldNode->pNext = pNewNode;
	return (POSITION) pNewNode;
}

template<class TYPE, class ARG_TYPE>
void CVWList<TYPE, ARG_TYPE>::RemoveAt(POSITION position)
{
	ASSERT_VALID(this);

	CVWNode* pOldNode = (CVWNode*) position;
	ASSERT(AfxIsValidAddress(pOldNode, sizeof(CVWNode)));

	// remove pOldNode from list
	if (pOldNode == m_pNodeHead)
	{
		m_pNodeHead = pOldNode->pNext;
	}
	else
	{
		ASSERT(AfxIsValidAddress(pOldNode->pPrev, sizeof(CVWNode)));
		pOldNode->pPrev->pNext = pOldNode->pNext;
	}
	if (pOldNode == m_pNodeTail)
	{
		m_pNodeTail = pOldNode->pPrev;
	}
	else
	{
		ASSERT(AfxIsValidAddress(pOldNode->pNext, sizeof(CVWNode)));
		pOldNode->pNext->pPrev = pOldNode->pPrev;
	}
	FreeNode(pOldNode);
}

template<class TYPE, class ARG_TYPE>
POSITION CVWList<TYPE, ARG_TYPE>::FindIndex(int nIndex) const
{
	ASSERT_VALID(this);
	ASSERT(nIndex >= 0);

	if (nIndex >= m_nCount)
		return NULL;  // went too far

	CVWNode* pNode = m_pNodeHead;
	while (nIndex--)
	{
		ASSERT(AfxIsValidAddress(pNode, sizeof(CVWNode)));
		pNode = pNode->pNext;
	}
	return (POSITION) pNode;
}

template<class TYPE, class ARG_TYPE>
POSITION CVWList<TYPE, ARG_TYPE>::Find(ARG_TYPE searchValue, POSITION startAfter) const
{
	ASSERT_VALID(this);

	CVWNode* pNode = (CVWNode*) startAfter;
	if (pNode == NULL)
	{
		pNode = m_pNodeHead;  // start at head
	}
	else
	{
		ASSERT(AfxIsValidAddress(pNode, sizeof(CVWNode)));
		pNode = pNode->pNext;  // start after the one specified
	}

	for (; pNode != NULL; pNode = pNode->pNext)
		if (CompareElements<TYPE>(&pNode->data, &searchValue))
			return (POSITION)pNode;
	return NULL;
}

template<class TYPE, class ARG_TYPE>
void CVWList<TYPE, ARG_TYPE>::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	CObject::Serialize(ar);

	if (ar.IsStoring())
	{
		ar.WriteCount(m_nCount);
		for (CVWNode* pNode = m_pNodeHead; pNode != NULL; pNode = pNode->pNext)
		{
			ASSERT(AfxIsValidAddress(pNode, sizeof(CVWNode)));
			SerializeElements<TYPE>(ar, &pNode->data, 1);
		}
	}
	else
	{
		DWORD nNewCount = ar.ReadCount();
		while (nNewCount--)
		{
			TYPE newData;
			SerializeElements<TYPE>(ar, &newData, 1);
			AddTail(newData);
		}
	}
}

#ifdef _DEBUG
template<class TYPE, class ARG_TYPE>
void CVWList<TYPE, ARG_TYPE>::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);

	dc << "with " << m_nCount << " elements";
	if (dc.GetDepth() > 0)
	{
		POSITION pos = GetHeadPosition();
		while (pos != NULL)
		{
			dc << "\n";
			DumpElements<TYPE>(dc, &((CVWList*)this)->GetNext(pos), 1);
		}
	}

	dc << "\n";
}

template<class TYPE, class ARG_TYPE>
void CVWList<TYPE, ARG_TYPE>::AssertValid() const
{
	CObject::AssertValid();

	if (m_nCount == 0)
	{
		// empty list
		ASSERT(m_pNodeHead == NULL);
		ASSERT(m_pNodeTail == NULL);
	}
	else
	{
		// non-empty list
		ASSERT(AfxIsValidAddress(m_pNodeHead, sizeof(CVWNode)));
		ASSERT(AfxIsValidAddress(m_pNodeTail, sizeof(CVWNode)));
	}
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
class CVWMap : public CObject
{
protected:
	// Association
	struct CVWAssoc
	{
		CVWAssoc* pNext;
		UINT nHashValueOrig;  // un-modulo hash key
		KEY key;
		VALUE value;
	};
public:
// Construction
	CVWMap(int nBlockSize = 10);

// Attributes
	// number of elements
	int GetCount() const;
	BOOL IsEmpty() const;

	// Lookup
	BOOL Lookup(ARG_KEY key, VALUE& rValue) const;
	BOOL LookupExt(ARG_KEY key, UINT nHashOrig, VALUE& rValue) const;

// Operations
	// Lookup and add if not there
	VALUE& operator[](ARG_KEY key);

	// add a new (key, value) pair
	void SetAt(ARG_KEY key, ARG_VALUE newValue);

	// removing existing (key, ?) pair
	BOOL RemoveKey(ARG_KEY key);
	void RemoveAll();

	// iterating all (key, value) pairs
	POSITION GetStartPosition() const;
	void GetNextAssoc(POSITION& rNextPosition, KEY& rKey, VALUE& rValue) const;

	// advanced features for derived classes
	UINT GetHashTableSize() const;
	void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);

// Implementation
protected:
	CVWAssoc** m_pHashTable;
	UINT m_nHashTableSize;
	int m_nCount;
	CVWAssoc* m_pFreeList;
	struct CPlex* m_pBlocks;
	int m_nBlockSize;

	CVWAssoc* NewAssoc();
	void FreeAssoc(CVWAssoc*);
	CVWAssoc* GetAssocAt(ARG_KEY, UINT&) const;
	CVWAssoc* GetAssocAtExt(ARG_KEY, UINT) const;

    // methods to aid with large maps where 17 buckets just ain't enough
   	UINT RoundUpToPrime(UINT nValue) const;
	void GrowHashTable();

public:
	~CVWMap();
	void Serialize(CArchive&);
#ifdef _DEBUG
	void Dump(CDumpContext&) const;
	void AssertValid() const;
	void TraceMap() const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE> inline functions

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline int CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetCount() const
	{ return m_nCount; }
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline BOOL CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::IsEmpty() const
	{ return m_nCount == 0; }
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline void CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::SetAt(ARG_KEY key, ARG_VALUE newValue)
	{ (*this)[key] = newValue; }
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline POSITION CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetStartPosition() const
	{ return (m_nCount == 0) ? NULL : BEFORE_START_POSITION; }
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
inline UINT CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetHashTableSize() const
	{ return m_nHashTableSize; }

/////////////////////////////////////////////////////////////////////////////
// CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE> out-of-line functions

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::CVWMap(int nBlockSize)
{
	ASSERT(nBlockSize > 0);

	m_pHashTable = NULL;
	m_nHashTableSize = 17;  // default size
	m_nCount = 0;
	m_pFreeList = NULL;
	m_pBlocks = NULL;
	m_nBlockSize = nBlockSize;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::InitHashTable(
	UINT nHashSize, BOOL bAllocNow)
//
// Used to force allocation of a hash table or to override the default
//   hash table size of (which is fairly small)
{
	ASSERT_VALID(this);
	ASSERT(m_nCount == 0);
	ASSERT(nHashSize > 0);

   	nHashSize = RoundUpToPrime (nHashSize);

	if (m_pHashTable != NULL)
	{
		// free hash table
		delete[] m_pHashTable;
		m_pHashTable = NULL;
	}

	if (bAllocNow)
	{
		m_pHashTable = new CVWAssoc* [nHashSize];
		memset(m_pHashTable, 0, sizeof(CVWAssoc*) * nHashSize);
	}
	m_nHashTableSize = nHashSize;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::RemoveAll()
{
	ASSERT_VALID(this);

	if (m_pHashTable != NULL)
	{
		// destroy elements (values and keys)
		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)
		{
			CVWAssoc* pAssoc;
			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;
			  pAssoc = pAssoc->pNext)
			{
				DestructElements<VALUE>(&pAssoc->value, 1);
				DestructElements<KEY>(&pAssoc->key, 1);
			}
		}
	}

	// free hash table
	delete[] m_pHashTable;
	m_pHashTable = NULL;

	m_nCount = 0;
	m_pFreeList = NULL;
	m_pBlocks->FreeDataChain();
	m_pBlocks = NULL;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::~CVWMap()
{
	RemoveAll();
	ASSERT(m_nCount == 0);
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::CVWAssoc*
CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::NewAssoc()
{
	if (m_pFreeList == NULL)
	{
		// add another block
		CPlex* newBlock = CPlex::Create(m_pBlocks, m_nBlockSize, sizeof(CVWMap::CVWAssoc));
		// chain them into free list
		CVWMap::CVWAssoc* pAssoc = (CVWMap::CVWAssoc*) newBlock->data();
		// free in reverse order to make it easier to debug
		pAssoc += m_nBlockSize - 1;
		for (int i = m_nBlockSize-1; i >= 0; i--, pAssoc--)
		{
			pAssoc->pNext = m_pFreeList;
			m_pFreeList = pAssoc;
		}
	}
	ASSERT(m_pFreeList != NULL);  // we must have something

	CVWMap::CVWAssoc* pAssoc = m_pFreeList;
	m_pFreeList = m_pFreeList->pNext;
	m_nCount++;
	ASSERT(m_nCount > 0);  // make sure we don't overflow
	ConstructElements<KEY>(&pAssoc->key, 1);
	ConstructElements<VALUE>(&pAssoc->value, 1);   // special construct values
	return pAssoc;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::FreeAssoc(CVWMap::CVWAssoc* pAssoc)
{
	DestructElements<VALUE>(&pAssoc->value, 1);
	DestructElements<KEY>(&pAssoc->key, 1);
	pAssoc->pNext = m_pFreeList;
	m_pFreeList = pAssoc;
	m_nCount--;
	ASSERT(m_nCount >= 0);  // make sure we don't underflow

    // this is REALLY expensive when map goes from 1->0->1->0, etc.  Not necessary
	//  // if no more elements, cleanup completely
	//  if (m_nCount == 0)
	//	    RemoveAll();
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::CVWAssoc*
CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetAssocAt(ARG_KEY key, UINT& nHashOrig) const
// find association (or return NULL)
{
	// return un-modulo hash key
	nHashOrig = HashKey<ARG_KEY>(key);
	UINT nHash = nHashOrig % m_nHashTableSize;

	if (m_pHashTable == NULL)
		return NULL;

	// see if it exists
	CVWAssoc* pAssoc;
	for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext)
	{
		// check that hash keys are identical before comparing elements
		if (nHashOrig == pAssoc->nHashValueOrig)
		{
			if (CompareElements(&pAssoc->key, &key))
				return pAssoc;
		}
	}
	return NULL;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::CVWAssoc*
CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetAssocAtExt(ARG_KEY key, UINT nHashOrig) const
// find association (or return NULL)
{
	if (m_pHashTable == NULL)
		return NULL;

	UINT nHash = nHashOrig % m_nHashTableSize;

	// see if it exists
	CVWAssoc* pAssoc;
	for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext)
	{
		// check that hash keys are identical before comparing elements
		if (nHashOrig == pAssoc->nHashValueOrig)
		{
			if (CompareElements(&pAssoc->key, &key))
				return pAssoc;
		}
	}
	return NULL;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
BOOL CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::Lookup(ARG_KEY key, VALUE& rValue) const
{
	ASSERT_VALID(this);

	UINT nHashOrig;
	CVWAssoc* pAssoc = GetAssocAt(key, nHashOrig);
	if (pAssoc == NULL)
		return FALSE;  // not in map

	rValue = pAssoc->value;
	return TRUE;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
BOOL CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::LookupExt(ARG_KEY key, UINT nHashOrig, VALUE& rValue) const
{
	ASSERT_VALID(this);

	// pass in pre-hashed value
	CVWAssoc* pAssoc = GetAssocAtExt(key, nHashOrig);
	if (pAssoc == NULL)
		return FALSE;  // not in map

	rValue = pAssoc->value;
	return TRUE;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
VALUE& CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::operator[](ARG_KEY key)
{
	ASSERT_VALID(this);

	UINT nHashOrig;
	CVWAssoc* pAssoc;
	if ((pAssoc = GetAssocAt(key, nHashOrig)) == NULL)
	{
		if (m_pHashTable == NULL)
			InitHashTable(m_nHashTableSize);
   		else 
        {
            // check if it is time to grow the table
            if ((unsigned)m_nCount >= m_nHashTableSize)
			    GrowHashTable();
		}

		// it doesn't exist, add a new Association
		pAssoc = NewAssoc();
		pAssoc->nHashValueOrig = nHashOrig;
		pAssoc->key = key;
		// 'pAssoc->value' is a constructed object, nothing more

		// put into hash table
		UINT nHash = nHashOrig % m_nHashTableSize;
		pAssoc->pNext = m_pHashTable[nHash];
		m_pHashTable[nHash] = pAssoc;
	}
	return pAssoc->value;  // return new reference
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
BOOL CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::RemoveKey(ARG_KEY key)
// remove key - return TRUE if removed
{
	ASSERT_VALID(this);

	if (m_pHashTable == NULL)
		return FALSE;  // nothing in the table

	CVWAssoc** ppAssocPrev;
	UINT nHash, nHashOrig;

	nHashOrig = HashKey<ARG_KEY>(key);
	nHash = nHashOrig % m_nHashTableSize;

	ppAssocPrev = &m_pHashTable[nHash];

	CVWAssoc* pAssoc;
	for (pAssoc = *ppAssocPrev; pAssoc != NULL; pAssoc = pAssoc->pNext)
	{
		if (nHashOrig == pAssoc->nHashValueOrig)
		{
			if (CompareElements(&pAssoc->key, &key))
			{
				// remove it
				*ppAssocPrev = pAssoc->pNext;  // remove from list
				FreeAssoc(pAssoc);
				return TRUE;
			}
		}

		ppAssocPrev = &pAssoc->pNext;
	}
	return FALSE;  // not found
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GetNextAssoc(POSITION& rNextPosition,
	KEY& rKey, VALUE& rValue) const
{
	ASSERT_VALID(this);
	ASSERT(m_pHashTable != NULL);  // never call on empty map

	CVWAssoc* pAssocRet = (CVWAssoc*)rNextPosition;
	ASSERT(pAssocRet != NULL);

	if (pAssocRet == (CVWAssoc*) BEFORE_START_POSITION)
	{
		// find the first association
		for (UINT nBucket = 0; nBucket < m_nHashTableSize; nBucket++)
			if ((pAssocRet = m_pHashTable[nBucket]) != NULL)
				break;
		ASSERT(pAssocRet != NULL);  // must find something
	}

	// find next association
	ASSERT(AfxIsValidAddress(pAssocRet, sizeof(CVWAssoc)));
	CVWAssoc* pAssocNext;
	if ((pAssocNext = pAssocRet->pNext) == NULL)
	{
		UINT nHash = pAssocRet->nHashValueOrig % m_nHashTableSize;

		// go to next bucket
		for (UINT nBucket = nHash + 1; nBucket < m_nHashTableSize; nBucket++)
			if ((pAssocNext = m_pHashTable[nBucket]) != NULL)
				break;
	}

	rNextPosition = (POSITION) pAssocNext;

	// fill in return data
	rKey = pAssocRet->key;
	rValue = pAssocRet->value;
}

// Round a value up to a prime number as given in our table
// Each entry in the table is the lowest prime number which
// exceeds twice the previous entry.
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
UINT CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::RoundUpToPrime(UINT nValue) const
{
	static unsigned int primes[] = { 17, 37, 79, 163, 331, 673, 1361, 2729, 5471, 10949,
		21911, 43853, 87719, 175447, 350899, 701819, 1403641, 2807303, 5614657, 11229331,
		22458671, 44917381, 89834777, 179669557, 359339171, 718678369, 1437356741, 2874713497 };

	int i = 0;

	while (primes[i] < nValue)
		i++;
	// if we ever hash more than 2 billion items, we might have to worry about falling off the end of the table.

	return primes[i];
}

// Grow the hash table by at least a factor of 2.
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::GrowHashTable()
{
	UINT nNewSize;
	CVWAssoc** ppNewTable;
	UINT i;
	CVWAssoc* pAssoc;
	UINT nHash;

	nNewSize = RoundUpToPrime(m_nHashTableSize * 2);
	ppNewTable = new CVWAssoc*[nNewSize];
	if (ppNewTable == NULL)
		return;			// can't resize it right now

	memset(ppNewTable, 0, sizeof(CVWAssoc*) * nNewSize);

	for (i = 0; i < m_nHashTableSize; i++)
	{
		while (m_pHashTable[i] != NULL)
		{
			pAssoc = m_pHashTable[i];
			m_pHashTable[i] = pAssoc->pNext;	// remove one from old table
			nHash = pAssoc->nHashValueOrig % nNewSize;
			pAssoc->pNext = ppNewTable[nHash];
			ppNewTable[nHash] = pAssoc;	// add one to new table
		}
	}

	delete[] m_pHashTable;
	m_pHashTable = ppNewTable;
	m_nHashTableSize = nNewSize;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	CObject::Serialize(ar);

	if (ar.IsStoring())
	{
		ar.WriteCount(m_nCount);
		if (m_nCount == 0)
			return;  // nothing more to do

		ASSERT(m_pHashTable != NULL);
		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)
		{
			CVWAssoc* pAssoc;
			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;
			  pAssoc = pAssoc->pNext)
			{
				SerializeElements<KEY>(ar, &pAssoc->key, 1);
				SerializeElements<VALUE>(ar, &pAssoc->value, 1);
			}
		}
	}
	else
	{
		DWORD nNewCount = ar.ReadCount();
		while (nNewCount--)
		{
			KEY newKey;
			VALUE newValue;
			SerializeElements<KEY>(ar, &newKey, 1);
			SerializeElements<VALUE>(ar, &newValue, 1);
			SetAt(newKey, newValue);
		}
	}
}

#ifdef _DEBUG
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::TraceMap() const
{
	if (m_pHashTable == NULL)
	{
		TRACE("CVWMap::TraceMap: empty hash table\n");
		return;
	}

	TRACE("CVWMap::TraceMap: begin\n");

	int k = 0;
	for (int i = 0; i < (int)m_nHashTableSize; i++)
	{
		// see if it exists
		CVWAssoc* pAssoc;
		int j = 0;

		for (j = 0, pAssoc = m_pHashTable[i]; pAssoc != NULL; j++, k++, pAssoc = pAssoc->pNext)
		{
			TRACE("\tbucket[%d], entry[%d], value %s\n", i, j, CString(pAssoc->key));
		}

		TRACE("bucket[%d] has %d entries\n", i, j);
	}

	TRACE("hash table has %d entries\n", k);
	TRACE("CVWMap::TraceMap: end\n");
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);

	dc << "with " << m_nCount << " elements";
	if (dc.GetDepth() > 0)
	{
		// Dump in format "[key] -> value"
		KEY key;
		VALUE val;

		POSITION pos = GetStartPosition();
		while (pos != NULL)
		{
			GetNextAssoc(pos, key, val);
			dc << "\n\t[";
			DumpElements<KEY>(dc, &key, 1);
			dc << "] = ";
			DumpElements<VALUE>(dc, &val, 1);
		}
	}

	dc << "\n";
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE>
void CVWMap<KEY, ARG_KEY, VALUE, ARG_VALUE>::AssertValid() const
{
	CObject::AssertValid();

	ASSERT(m_nHashTableSize > 0);
	ASSERT(m_nCount == 0 || m_pHashTable != NULL);
		// non-empty map should have hash table
}
#endif //_DEBUG

#ifdef _AFX_PACKING
#pragma pack(pop)
#endif

#ifdef _AFX_MINREBUILD
#pragma component(minrebuild, on)
#endif
#ifndef _AFX_FULLTYPEINFO
#pragma component(mintypeinfo, off)
#endif

#endif //__VWTEMPL_H__

/////////////////////////////////////////////////////////////////////////////
