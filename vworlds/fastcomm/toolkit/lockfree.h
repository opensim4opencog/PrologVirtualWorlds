// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _WaitFree_H
#define _WaitFree_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//disable warning that some template identifiers are more than 255 characters and are
//truncated by the compiler.
#pragma warning( disable : 4786 )  

/*
				A Correct and Efficient Threadsafe Lock Free Queue

CREDITS:
	Implemented/designed/tested by CH. 
	
	This work is based on research by Maged M. Michael 
	published in his doctoral thesis "Reducing the Overhead of Sharing on Shared
	Memory Multiprocessors", 1997, U of Rochester, under Prof. Michael Scott.

	The lock free stack used for memory allocation is based on an implementation by Joe Long.

PERFORMANCE:
	On a quad 450mhz intel xeon machine:
	i have n threads put stuff in, 1 thread taking req's out and checking the serialization order for
	correctness. LF=lock free queue. Normal=CList implementation with critsecs. Results vary from
	time to time due to, (I believe), cache hit/misses. These are pessimistic since I'm making
	the queue huge (100000 msgs by each thread) to measure the time accurately 
	(but making the unrealistic assumption that the q does not fit in the cache). 
	If I make the q small (100 msgs or less), the q is
	too fast to measure very well (millions of msgs/s even with tons of threads).
	Threads		LF			Normal
	1			588K		54Kmsgs/second
	2			475K		70K
	4			416K		61K All four procs are going. NT distributing load well.
	8			593K		73K
	16			530K		41K Context switches hurting.
	32			521K		51K

	With a smaller (but still large) q of each thread queueing 1000 msgs each:
	256			1200K		74K
	Less than this, the timer lacks resolution.

	On a dual proc machine:
	Threads		LF			Normal
	1			568K		54Kmsgs/second
	2			557K		55K
	4			656K		61K All four procs are going. NT distributing load well.
	8			897K		55K
	16			642K		41K Context switches hurting.
	32			612K		51K

	On a single proc 450mhz machine:
	Threads		LF			Normal
	1			49K			50K
	2			52K			52K
	4			105K		108K
	8			219K		223K
	16			1200K		937K 
	Here too, the LF's are faster or about the same as the crit sectioned q. The difference
	is much smaller though.

	Clearly, the LF q is much faster on both multiprocs and single procs, making this the
	q of choice on a range of machines. The perf advantage ranges from 10x to 130x faster. 

DEFINES:
	- You can define different memory allocators below.

EXAMPLE USAGE:
	void sample(void)
	{
		CWaitFreeQueue <int> q;
		int i;
		q.put(5);
		assert(true==q.get(i)); //it's full.
		assert(5==i);			//obviously.
		assert(false==q.get()); //it's empty now.
	}

	Simple enough? ;=)
*/

//allocator. returns ptr to space of correct size of T. uses appropriate system allocator.
//my tests show that heapfree is fastest on nt5. malloc is fastest on nt4/win9x.
//note: this does *not* call class's constructor/destructor. you need to 
//override new/delete for this.
class CSystemAllocator
{
public:
	CSystemAllocator()
	{
		//Under NT5, the heapalloc is must faster than new (especially in the multiproc case), 
		//but on NT4, the C++ new is more efficient.
		m_hHeap = GetProcessHeap();		
	}
	
	~CSystemAllocator()
	{
		m_hHeap = NULL;
	}

	volatile void * allocate(size_t size) 
	{
		ASSERT(m_hHeap);
		void *p = HeapAlloc(m_hHeap, 
			HEAP_GENERATE_EXCEPTIONS, size);
		//TRACE("HeapAllocating'ing: %x\n",p);
		return p;
	}

	//note that this does *not* call C++ destructor.
	void free(volatile void * pnode)
	{
		ASSERT(m_hHeap);
		//TRACE("HeapFree'ing: %x\n",pnode);
		HeapFree(m_hHeap, NULL, (void *)pnode);
	}

private:
	HANDLE m_hHeap;
};

/*
Avail on intel pentiums and above.
	Atomically performs:
	{
		__int64 result = *pDestination;
		if (result == comperand)
			*pDestination = exchange;
		return result;
	}
*/

#pragma warning (disable: 4035)		// function doesn't return value warning.
inline __int64 InterlockedCompareExchange64 (volatile __int64* pDestination, __int64 exchange, __int64 comperand)
{
	__asm
	{
		mov esi, pDestination

		mov eax, DWORD PTR comperand[0]
		mov edx, DWORD PTR comperand[4]

		mov ebx, DWORD PTR exchange[0]
		mov ecx, DWORD PTR exchange[4]

		_emit 0xF0		// lock
		_emit 0x0F		// cmpxchg8b [esi]
		_emit 0xC7
		_emit 0x0E

		// result is in DX,AX
	}
}
#pragma warning (default: 4035)		// function doesn't return value warning

////////////////////////////////////////////////////////////////////////////////////////////
//The basic lockfree stack, suitable for use as a free list-------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////

//A lock free stack used as a free list by the q to reuse nodes.
//Nodes you put into this thing has to derive from CStackPoolNode.
//This class does not allocate anything.
class CLockFreeStackPool
{
	class CStackPoolPtr;
public:
	class CStackPoolNode
	{
		friend CLockFreeStackPool;
		friend CStackPoolPtr;
	public:
		CStackPoolNode() {}
		
		~CStackPoolNode() {}

		void * operator new(size_t size)
		{
			return (void *)m_SystemAllocator.allocate(size);
		}

		void operator delete(void *p)
		{
			m_SystemAllocator.free(p);
		}	

		static void GlobalCleanup()
		{
			m_SystemAllocator.~CSystemAllocator();
		}
	private:	
		//32 bit ptr used by stack algorithm.
		CStackPoolNode volatile * m_pStackNext;

		static CSystemAllocator m_SystemAllocator;
	};

private:
	//This is the 64 bit Ptr used by the LF stack.
	class CStackPoolPtr
	{
	public:
		union {
			__int64  m_i64;  // ensure 8-byte alignment
			struct Element {
				CStackPoolNode volatile * pvFirst;			// ptr to user data of first free block;
				unsigned long volatile ulFlag;	// flag word, see below.
			} m_element;
		};

		// default constructor:
		CStackPoolPtr() 
		{		
			m_element.pvFirst = NULL;
			m_element.ulFlag = 0;
		}
	};

public:
	void push(CStackPoolNode volatile * pNode)
	{		
		CStackPoolPtr volatile ListComp;
		CStackPoolPtr volatile ListExch;
		for (;;) // make it thread safe
		{
			ListComp.m_element.pvFirst = sm_list.m_element.pvFirst;
			ListComp.m_element.ulFlag  = sm_list.m_element.ulFlag;           			
            //pNode -> data = (void *)NULL;
			pNode -> m_pStackNext = ListComp.m_element.pvFirst;
			ListExch.m_element.pvFirst = pNode;
			ListExch.m_element.ulFlag = ListComp.m_element.ulFlag + 1;
			if (ListComp.m_i64 == InterlockedCompareExchange64  (&sm_list.m_i64, 
				ListExch.m_i64, ListComp.m_i64))
			{				
				return;
			}
		}
	}

	CStackPoolNode volatile * pop()
	{
		CStackPoolNode volatile * pNode = NULL;
		CStackPoolPtr volatile ListComp;
		CStackPoolPtr volatile ListExch;
		for (;;)
		{
			ListComp.m_element.pvFirst = sm_list.m_element.pvFirst;
			ListComp.m_element.ulFlag  = sm_list.m_element.ulFlag;
			pNode = ListComp.m_element.pvFirst;
			if (pNode == NULL) // stack empty
			{
				return NULL;
			}
			
			ListExch.m_element.pvFirst = pNode -> m_pStackNext;
			ListExch.m_element.ulFlag = ListComp.m_element.ulFlag + 1;
			if (ListComp.m_i64 == InterlockedCompareExchange64  (&sm_list.m_i64, 
				ListExch.m_i64, ListComp.m_i64))
			{
				return  pNode;   				
			}	
		}
	}

	~CLockFreeStackPool()
	{
		//delete everything.
		CStackPoolNode volatile *p;
		p = pop();
		while(p)
		{
			delete p;		
			p = pop();			
		}
	}

private:
	CStackPoolPtr volatile sm_list;   	
};

//Same thing with extra features.
class CLockFreeStackPoolEx : public CLockFreeStackPool 
{
public:
	CLockFreeStackPoolEx()
	{
		m_Size = 0;
	}

	void push(CStackPoolNode volatile * pNode)
	{
		InterlockedIncrement(&m_Size);
		CLockFreeStackPool::push(pNode);
	}

	CStackPoolNode volatile * pop()
	{
		volatile CStackPoolNode * pNode = CLockFreeStackPool::pop();
		if(pNode)
			InterlockedDecrement(&m_Size);
		return pNode;
	}

	long getSize() {return m_Size;}

private:
	long m_Size;
};

////////////////////////////////////////////////////////////////////////////////////////////
//The basic templatized lockfree stack, using the above stack pool as a free list.
//This stack is the one you should use if you don't want to think about it too much.
//T's copy needs to be fast. (Should be a ptr, preferably).
////////////////////////////////////////////////////////////////////////////////////////////
template <class T> class CLockFreeStack
{
	class CLockFreeStackNode : public CLockFreeStackPool::CStackPoolNode
	{
	public:
		CLockFreeStackNode() {}
	
		//32 bit ptr used by stack algorithm.
		CLockFreeStackNode volatile * m_pStackNext;

		T m_Data;
	};

	//This is the 64 bit Ptr used by the LF stack.
	class CStackPtr
	{
	public:
		union {
			__int64  m_i64;  // ensure 8-byte alignment
			struct Element {
				CLockFreeStackNode volatile * pvFirst;			// ptr to user data of first free block;
				unsigned long volatile ulFlag;	// flag word, see below.
			} m_element;
		};

		// default constructor:
		CStackPtr() 
		{		
			m_element.pvFirst = NULL;
			m_element.ulFlag = 0;
		}
	};

public:
	void push(T data)
	{		
		CStackPtr volatile ListComp;
		CStackPtr volatile ListExch;

		//Allocate node.
		CLockFreeStackNode volatile *pNode = (CLockFreeStackNode volatile *)m_StackPool.pop();
		if(pNode==NULL)
		{
			pNode = new CLockFreeStackNode;
		}
		pNode->m_Data=data;

		for (;;) // make it thread safe
		{
			ListComp.m_element.pvFirst = sm_list.m_element.pvFirst;
			ListComp.m_element.ulFlag  = sm_list.m_element.ulFlag;           			
			pNode -> m_pStackNext = ListComp.m_element.pvFirst;
			ListExch.m_element.pvFirst = pNode;
			ListExch.m_element.ulFlag = ListComp.m_element.ulFlag + 1;
			if (ListComp.m_i64 == InterlockedCompareExchange64  (&sm_list.m_i64, 
				ListExch.m_i64, ListComp.m_i64))
			{				
				return;
			}
		}
	}

	bool pop(T &retData)
	{
		CLockFreeStackNode volatile * pNode = NULL;
		CStackPtr volatile ListComp;
		CStackPtr volatile ListExch;
		for (;;)
		{
			ListComp.m_element.pvFirst = sm_list.m_element.pvFirst;
			ListComp.m_element.ulFlag  = sm_list.m_element.ulFlag;
			pNode = ListComp.m_element.pvFirst;
			if (pNode == NULL) // stack empty
			{
				return FALSE;
			}
			
			ListExch.m_element.pvFirst = pNode -> m_pStackNext;
			ListExch.m_element.ulFlag = ListComp.m_element.ulFlag + 1;
			if (ListComp.m_i64 == InterlockedCompareExchange64  (&sm_list.m_i64, 
				ListExch.m_i64, ListComp.m_i64))
			{
				retData = pNode->m_Data;
				m_StackPool.push(pNode);
				return  TRUE;   				
			}	
		}
	}

	~CLockFreeStack()
	{
		//pop everything from stack to stackpool.
		//stack will delete properly.
		T data;
		bool b;
		b = pop(data);
		while(b)
		{
			b = pop(data);			
		}
	}

private:
	CStackPtr volatile sm_list;   	
	CLockFreeStackPool m_StackPool;
};

template <class T> class CLockFreeStackEx : public CLockFreeStack <T>
{
public:
	CLockFreeStackEx() : m_iSize(0) {}
	void push(T data)
	{
		InterlockedIncrement(&m_iSize);
		CLockFreeStack <T>::push(data);
	}
	bool pop(T &retData)
	{
		bool b = CLockFreeStack <T>::pop(retData);

		if(b)
			InterlockedDecrement(&m_iSize);

		return b;
	}

	long getSize() 
	{
		return m_iSize;
	}
private:
	long m_iSize;
};

////////////////////////////////////////////////////////////////////////////////////////////
// Lock Free Queue
//
//The pointers in this algorithm are 64 bits.
//The first 32 is the real pointer, the second a monotonically increasing
//count that assures the failure in the CAS in case of the ABA problem.
//Structure heading each list. This struct must be exactly 8 bytes long.
//I had many overrides here but I decided not to use much C++. It's better
//to be absolutely clear about what's going on in such a complex algorithm.
//
//Note that it's *not* optional to use the LF stack to reuse nodes. Removing it
//will cause access violations.
////////////////////////////////////////////////////////////////////////////////////////////

//forward declaration for ptr below.
template <class T> class CWaitFreeQueueNode; 

template <class T> struct PtrToWaitFreeQueueNode
{
	union {
		__int64  volatile m_i64;  // ensure 8-byte alignment
		struct PtrType {
			CWaitFreeQueueNode <T> volatile * m_Ptr;
			__int32 volatile m_iCount;
		} m_64BitCombo;
	};

	//The fcns that manipulate this struct use the __int64 extension datatype
	//to perform the 64bit compares and assignments necessary
	//in the q algorithm. These are *not* guarenteed to be atomic.
	//This is okay since we *are* guarenteed that all updates are atomic.

	//Fcns---------------------------------------
	PtrToWaitFreeQueueNode <T> () {}
	PtrToWaitFreeQueueNode <T> (CWaitFreeQueueNode <T> volatile * Ptr, __int32 volatile iCount)
	{
		m_64BitCombo.m_Ptr = Ptr;
		m_64BitCombo.m_iCount = iCount;
	}
};

//These are nodes of both the queue and the lock free stack used as a free list.
template <class T> class CWaitFreeQueueNode : public CLockFreeStackPool::CStackPoolNode
{
public:
	CWaitFreeQueueNode <T> () {}

	//User data.
	T m_Data;

	//64 bit pointer used by q algorithm.
	PtrToWaitFreeQueueNode <T> volatile m_pNext;		
};

//Wait free refers to the fact that in addition to the fact that the algorithm has no
//locks (thus making it lock free), deadlocks are guarenteed not to exist (a deadlock would be an
//infinite spin in the for loop). Of course, this doesn't mean it's efficient. But 
//my tests show that this algorithm is very efficient compared to the standard lock algorithm,
//with advantages increasing with the number of processors, as well as the # of threads per procr.
template <class T> class CWaitFreeQueue
{
public:
	CWaitFreeQueue <T> () 
	{
		PtrToWaitFreeQueueNode <T> volatile pNode;

		//Allocate a dummy node.
		pNode.m_64BitCombo.m_Ptr = new CWaitFreeQueueNode<T>;
		
		//IMPORTANT: Never write to the count 32 bits. 
		
		//Initialize next's ptr.
		pNode.m_64BitCombo.m_Ptr->m_pNext.m_64BitCombo.m_Ptr = (CWaitFreeQueueNode <T> *)NULL;
		m_pHead.m_i64 = m_pTail.m_i64 = pNode.m_i64;
	}

	~CWaitFreeQueue <T> () 
	{
		//empty queue.
		//note that destructors of T is not called. user is responsible for this.
		T data;
		bool ret = true;
		while(ret)
		{
			ret = get(data);
		}

		//assert that it should be empty.
		ASSERT(m_pHead.m_i64 == m_pTail.m_i64);
		ASSERT((m_pHead.m_64BitCombo.m_Ptr->m_pNext.m_64BitCombo.m_Ptr)==NULL);
		
		delete (CWaitFreeQueueNode <T> *)m_pHead.m_64BitCombo.m_Ptr;
	
		m_pHead.m_64BitCombo.m_Ptr = m_pTail.m_64BitCombo.m_Ptr = NULL;
		
		//The remaining nodes are freed when the stack is destructed.
	}

	void put(volatile T data) 
	{
		//I'm being very conservative by making every ptr volatile, but it doesn't hurt
		//and I'm worried about the correctness of the compiler output.

		PtrToWaitFreeQueueNode <T> volatile pNode; //The new node.
		PtrToWaitFreeQueueNode <T> volatile pTail; //The current tail.
		PtrToWaitFreeQueueNode <T> volatile pNext; //The current next.
		
		//Allocate a new node.
		pNode.m_64BitCombo.m_Ptr = (volatile CWaitFreeQueueNode<T> *)m_StackPool.pop();
		if(!pNode.m_64BitCombo.m_Ptr)
			pNode.m_64BitCombo.m_Ptr = new CWaitFreeQueueNode<T>;

		//Copy values into the node.
		//These are, of course, arbitrary.
		pNode.m_64BitCombo.m_Ptr->m_Data = data;

		//The node should point to NULL.
		pNode.m_64BitCombo.m_Ptr->m_pNext.m_64BitCombo.m_Ptr = NULL;

		//the q puts stuff on the tail, and gets stuff from the head.
		//head points down to the tail.
		for(;;)
		{
			//the following needs to be a 64 bit copy. The assignment operator
			//is implemented by the compiler. This need not be atomic.
			pTail.m_i64 = m_pTail.m_i64;

			//64 bit copy.
			pNext.m_i64 = pTail.m_64BitCombo.m_Ptr->m_pNext.m_i64;

			//64 bit compare.
			//This line is not really necessary and is a speed optimization.
			if(pTail.m_i64==m_pTail.m_i64)
			{
				//check consistency. This has to be the tail still.
				if(pNext.m_64BitCombo.m_Ptr==NULL)
				{
					//JL discovered the challenging case
					//where this thread sleeps here. The sleep tests this.
					//Sleep(0);

					//If we did not use our own stack that never deallocates nodes
					//until the q is destructed, we would need exception handling
					//to deal with access violations from the race condition above.
					//The CAS below should always fail if the node is freed by gets.

					//This tries to link the new node to the end of the linked list.
					if(pNext.m_i64==InterlockedCompareExchange64(
						&pTail.m_64BitCombo.m_Ptr->m_pNext.m_i64, 
						PtrToWaitFreeQueueNode <T> 
							(pNode.m_64BitCombo.m_Ptr, pNext.m_64BitCombo.m_iCount+1).m_i64, 
						pNext.m_i64))
					{
						//succeeded
						break;
					}
				}
				else
				{
					//The tail is no longer pointing to the last node.
					//Try to move m_pTail to the next node.
					//It's okay if this doesn't succeed. It means another
					//thread must have moved it already.
						InterlockedCompareExchange64(
						&m_pTail.m_i64, 
						PtrToWaitFreeQueueNode <T> 
							(pNext.m_64BitCombo.m_Ptr, pTail.m_64BitCombo.m_iCount+1).m_i64,
						pTail.m_i64);
				}
			}
		}
		
		//Try to move m_pTail to the next node.
		//It's okay if this doesn't succeed. It means another
		//thread must have moved it already.		
		InterlockedCompareExchange64(&m_pTail.m_i64, 
			PtrToWaitFreeQueueNode <T> 
				(pNode.m_64BitCombo.m_Ptr, pTail.m_64BitCombo.m_iCount+1).m_i64,
			pTail.m_i64);
	}

	bool get(volatile T & data) 
	{
		PtrToWaitFreeQueueNode <T> volatile pHead, pTail, pNext;
		
		for(;;)
		{
			//64 bit assigns.
			pHead.m_i64 = m_pHead.m_i64;
			pTail.m_i64 = m_pTail.m_i64;
			pNext.m_i64 = pHead.m_64BitCombo.m_Ptr->m_pNext.m_i64;

			//64 bit compare.
			if(pHead.m_i64==m_pHead.m_i64)
			{
				//check if q is empty or if tail is falling behind (someone else
				//called get here).
				if(pHead.m_64BitCombo.m_Ptr==pTail.m_64BitCombo.m_Ptr)
				{
					//is q empty?
					if(pNext.m_64BitCombo.m_Ptr==NULL)
						return false;
		
					//tail is falling behind. try to move it ahead one.
					//it's okay if this fails. someone else's succeeded.
					InterlockedCompareExchange64(&m_pTail.m_i64, 
						PtrToWaitFreeQueueNode <T>
							(pNext.m_64BitCombo.m_Ptr, pTail.m_64BitCombo.m_iCount+1).m_i64,
						pTail.m_i64);					
				}
				else
				{
					//Read the value before CAS.
					//If I do this after, the node could be freed by another
					//thread. Here, the info is discarded if the CAS fails.
					//If it succeeds, I'm certain that the values i'm reading
					//are valid.
					data = pNext.m_64BitCombo.m_Ptr->m_Data;
					
					//Try to swing m_pHead to next node.
					if	(pHead.m_i64==
							InterlockedCompareExchange64
							(
								&m_pHead.m_i64, 
								PtrToWaitFreeQueueNode <T>
									(pNext.m_64BitCombo.m_Ptr, pHead.m_64BitCombo.m_iCount+1).m_i64, 
								pHead.m_i64
							)
						)
					{
						//succeeded
						break;	
					}
				}
			}
		}

		//Free the old dummy node. (the q has a new one now).
		//Put it back in the stack.
		m_StackPool.push(pHead.m_64BitCombo.m_Ptr);		

		return true;
	}

	//peek. CAUTION. If you using this, you almost certainly need a lock.
	//Data is *not* guarenteed to be valid.
	//data is not guarenteed to still be there when you call get after this.
	bool peek(volatile T & data) 
	{
		PtrToWaitFreeQueueNode <T> volatile pHead, pTail, pNext;
		
		//64 bit assigns.
		pHead.m_i64 = m_pHead.m_i64;
		pNext.m_i64 = pHead.m_64BitCombo.m_Ptr->m_pNext.m_i64;

		//is q empty?
		if(pNext.m_64BitCombo.m_Ptr==NULL)
			return false;
		
		//Read the value before CAS.
		//If I do this after, the node could be freed by another
		//thread. Here, the info is discarded if the CAS fails.
		//If it succeeds, I'm certain that the values i'm reading
		//are valid.
		data = pNext.m_64BitCombo.m_Ptr->m_Data;		
		return true;
	}

protected:
	PtrToWaitFreeQueueNode <T> volatile m_pHead;
	PtrToWaitFreeQueueNode <T> volatile m_pTail;

	CLockFreeStackPool m_StackPool;
};

template <class T> class CWaitFreeQueueEx : public CWaitFreeQueue <T>
{
public:
	CWaitFreeQueueEx <T> () 
	{
		m_lSize = 0;
	}

	~CWaitFreeQueueEx <T> () 
	{
	}

	void put(volatile T data) 
	{
		CWaitFreeQueue<T>::put(data);
		InterlockedIncrement(&m_lSize);
	}

	bool get(volatile T & data) 
	{
		bool ret = CWaitFreeQueue<T>::get(data);
		if(ret) 
			InterlockedDecrement(&m_lSize);
		return ret;
	}

	bool peek(volatile T & data) 
	{
		return CWaitFreeQueue<T>::peek(data);
	}

	long getSize() {return m_lSize;}
protected:
	long m_lSize;
};

#endif _WaitFree_H
