// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

/////////////////////////////////////////////////////////////////////////////////////////
// Project:		FastComm
// Module:		tools.h
// Description:	implements user classes: CTimer, SmartPtr.
// Author:		CH
// Create:		10/28/1999
/////////////////////////////////////////////////////////////////////////////////////////
#include "lockfree.h"
#include "workq.h"
#include "locks.h"

#ifndef TOOLS_HEADER
#define TOOLS_HEADER

CString str(long in);
CString str(void *p);
CString str(ULONG in);
CString str(double in);

class CTimer
{
public:
	void start(void) 
	{
		VERIFY(QueryPerformanceCounter((LARGE_INTEGER *)&iStart));
	}
	int stop(void) 
	{
		__int64 iFrequency;
		VERIFY(QueryPerformanceCounter((LARGE_INTEGER *)&iStop));
		VERIFY(QueryPerformanceFrequency((LARGE_INTEGER *)&iFrequency));
		return (int)(((double)iStop-(double)iStart)/(double)iFrequency*1000.0);
	}

private:
	__int64 iStart;
	__int64 iStop;
};

template <class T>
class SmartPtr
{
public:
	SmartPtr() { p=NULL; }
	SmartPtr(T* lp) { if ((p = lp) != NULL) p->AddRef(); }
	SmartPtr(const SmartPtr<T>& lp) {	if ((p = lp.p) != NULL)	p->AddRef(); }
	~SmartPtr() { if (p) p->Release(); }
	bool operator!() const {return (p == NULL);}
	bool operator==(SmartPtr & pT) const {return p == pT.p;}
    SmartPtr& operator=(SmartPtr<T> &p_) {return operator=((T *) p_);}    
	SmartPtr& operator=(T* p_) {if(p) p->Release(); p = p_; if(p) p->AddRef(); return *this;}
    T* operator->(void) { return p; }
	T& operator*() const {ASSERT(p!=NULL); return *p; }
	operator T*() const	{return (T*)p;}

private:
	T* p;
};

#endif