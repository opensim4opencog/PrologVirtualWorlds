// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _MARSHBUF_H_
#define _MARSHBUF_H_

// MarshBuf.h : Declaration of the CMarshallBufferObject

#include <resource.h>

#include <propbase.h>

#include <afxtempl.h>

// V-Worlds optimized templates
#include <vwtempl.h>

EXTERN_C const CLSID CLSID_MarshallBuffer;
EXTERN_C const CLSID CLSID_UnMarshallBuffer;

extern CVWMap<CBSTRAdapt<CComBSTR>, BSTR, int, int&> gAtomMap;

//#define TRACE_MARSHBUF_ALLOCATION

/////////////////////////////////////////////////////////////////////////////
// VWOBJECT

//REVIEW -- using pointers to ID's is necessary because some compilers don't like
//references as template arguments.

class ATL_NO_VTABLE CMarshallBufferObject :
	public IDispatchImpl<IMarshallBuffer, &IID_IMarshallBuffer, &LIBID_VWSYSTEMLib>,
	public ISupportErrorInfo,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMarshallBufferObject, &CLSID_MarshallBuffer>
{
DECLARE_AGGREGATABLE(CMarshallBufferObject)

public:
BEGIN_COM_MAP(CMarshallBufferObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IMarshallBuffer)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

DECLARE_REGISTRY(CLSID_MarshallBuffer, "VWSYSTEM.MarshallBuffer.1", "VWSYSTEM.MarshallBuffer", IDS_MARSHBUF_DESC, THREADFLAGS_BOTH)

public:
	CMarshallBufferObject() : m_pbytes(NULL)
	{ 
#ifdef TRACE_MARSHBUF_ALLOCATION
		TRACE("CMarshallBufferObject::CMarshallBufferObject, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif
	};

	~CMarshallBufferObject() 
	{
#ifdef TRACE_MARSHBUF_ALLOCATION
		TRACE("CMarshallBufferObject::~CMarshallBufferObject, this 0x%0x\n", this);
#endif

		if (m_pbytes)
		{
			free (m_pbytes);
			m_pbytes = NULL;
		}
	};

public:
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IMarshallBuffer
public:
	STDMETHOD(put_ClassID)(REFCLSID clsid);
	STDMETHOD(put_Byte)(unsigned char bval);
	STDMETHOD(put_Bytes)(unsigned char* pbytes, long lval);
	STDMETHOD(put_Boolean)(VARIANT_BOOL boolval);
	STDMETHOD(put_String)(BSTR bstr, VARIANT_BOOL bCompress = VARIANT_TRUE);
	STDMETHOD(put_Float)(float fval);
	STDMETHOD(put_Double)(double dval);
	STDMETHOD(put_UShort)(unsigned short usval);
	STDMETHOD(put_Short)(short sval);
	STDMETHOD(put_UInt)(unsigned int uival);
	STDMETHOD(put_Int)(int ival);
	STDMETHOD(put_ULong)(unsigned long ulval);
	STDMETHOD(put_Long)(long lval);
	STDMETHOD(put_UChar)(unsigned char ucval);
	STDMETHOD(put_BLOB)(BSTR blobval);

	STDMETHOD(get_Length)(DWORD* plval)
	{
		ASSERT(plval);

		*plval = m_marshFile.GetLength();

		return S_OK;
	};

	STDMETHOD(SeekToBegin)(void)
	{
		m_marshFile.SeekToBegin();

		return S_OK;
	};

	STDMETHOD(Rewind)(void)
	{
		m_marshFile.SetLength(0);

		return S_OK;
	};

	STDMETHOD(CopyFrom)(unsigned char* pbytes, DWORD lval);

	STDMETHOD(Detach)(unsigned char** pbytes, DWORD* plval)
	{
		ASSERT(pbytes);
		ASSERT(plval);

		// if we've already detached the buffer, fail
		if (m_pbytes)
			return VWOBJECT_E_FILEEXCEPTION;

		*plval = m_marshFile.GetLength();
		// throws CFileException

		// save off buffer so we can destroy it
		// on CMarshallBuffer destruction
		m_pbytes = m_marshFile.Detach();

		*pbytes = m_pbytes;

		return S_OK;
	};

	STDMETHOD(WriteToDisk)(BSTR strFilename);

// IMarshallBuffer	
private:
	CMemFile m_marshFile;
	unsigned char* m_pbytes;
};

class ATL_NO_VTABLE CUnMarshallBufferObject :
	public IDispatchImpl<IUnMarshallBuffer, &IID_IUnMarshallBuffer, &LIBID_VWSYSTEMLib>,
	public ISupportErrorInfo,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CUnMarshallBufferObject, &CLSID_UnMarshallBuffer>
{
DECLARE_AGGREGATABLE(CUnMarshallBufferObject)

public:
BEGIN_COM_MAP(CUnMarshallBufferObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IUnMarshallBuffer)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

DECLARE_REGISTRY(CLSID_UnMarshallBuffer, "VWSYSTEM.UnMarshallBuffer.1", "VWSYSTEM.UnMarshallBuffer", IDS_UNMARSHBUF_DESC, THREADFLAGS_BOTH)

public:
	CUnMarshallBufferObject() 
	{ 
#ifdef TRACE_MARSHBUF_ALLOCATION
		TRACE("CUnMarshallBufferObject::CUnMarshallBufferObject, sizeof %d, this 0x%0x\n", sizeof *this, this);
#endif
	};

	~CUnMarshallBufferObject() 
	{
#ifdef TRACE_MARSHBUF_ALLOCATION
		TRACE("CUnMarshallBufferObject::~CUnMarshallBufferObject, this 0x%0x\n", this);
#endif
	};

public:
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IUnMarshallBuffer
public:
	STDMETHOD(get_ClassID)(CLSID* pclsid);
	STDMETHOD(get_Byte)(unsigned char* bval);
	STDMETHOD(get_Bytes)(unsigned char* pbytes, long lval);
	STDMETHOD(get_Boolean)(VARIANT_BOOL* boolval);
	STDMETHOD(get_String)(BSTR* pbstr);
	STDMETHOD(get_UShort)(unsigned short* pusval);
	STDMETHOD(get_Short)(short* psval);
	STDMETHOD(get_Float)(float* pfval);
	STDMETHOD(get_Double)(double* pdval);
	STDMETHOD(get_UInt)(unsigned int* puival);
	STDMETHOD(get_Int)(int* pival);
	STDMETHOD(get_ULong)(unsigned long* pulval);
	STDMETHOD(get_Long)(long* plval);
	STDMETHOD(get_UChar)(unsigned char* pucval);
	STDMETHOD(get_BLOB)(BSTR* pblobval);

	STDMETHOD(get_Length)(DWORD* plval)
	{
		ASSERT(plval);

		*plval = m_marshFile.GetLength();

		return S_OK;
	};

	STDMETHOD(SeekToBegin)(void)
	{
		m_marshFile.SeekToBegin();

		return S_OK;
	};

	STDMETHOD(Rewind)(void)
	{
		m_marshFile.SetLength(0);

		return S_OK;
	};

	// used for initializing the buffer
	STDMETHOD(CopyTo)(unsigned char* pbytes, DWORD lval);

	STDMETHOD(Attach)(unsigned char* pbytes, DWORD lval)
	{
		ASSERT(pbytes);

		m_marshFile.Attach(pbytes, lval);

		return S_OK;
	};

	STDMETHOD(ReadFromDisk)(BSTR strFilename);

// IUnMarshallBuffer	
private:
	CMemFile m_marshFile;
};

#endif // _MARSHBUF_H_