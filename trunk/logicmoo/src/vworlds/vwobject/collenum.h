// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _COLLENUM_H_
#define _COLLENUM_H_

#include <resource.h>
#include <propbase.h>

class CEnumCollection: public IEnumVARIANT
{
friend class CPropertyListObject;
friend class CPropertyMapObject;
public:

// IUnknown
	STDMETHOD(QueryInterface)(REFIID iid, void** ppunk);
	STDMETHOD_(ULONG, AddRef)(void);
	STDMETHOD_(ULONG, Release)(void);

// IEnumVARIANT
	STDMETHOD(Next)(unsigned long celt, 
					VARIANT* rgvar, 
					unsigned long* pceltFetched);
	STDMETHOD(Skip)(unsigned long celt);
	STDMETHOD(Reset)();
	STDMETHOD(Clone)(IEnumVARIANT**  ppenum);

protected:
	CEnumCollection();
	~CEnumCollection();
	HRESULT Initialize(IPropertyCollection* pcoll);

	ULONG m_dwRef;
	IPropertyCollection* m_pcoll;
	ULONG m_celt;
	ULONG m_ielt;
};

#endif