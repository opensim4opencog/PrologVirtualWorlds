// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// MarshBuf.cpp : Implementation of CfooApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include "..\vwcomm\commobj.h"
#include "marshbuf.h"
#include <io.h>

#define USE_ATOM_COMPRESSION

/////////////////////////////////////////////////////////////////////////////
// ErrorInfo stuff

STDMETHODIMP CMarshallBufferObject::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IMarshallBuffer,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CUnMarshallBufferObject::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IUnMarshallBuffer,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CUnMarshallBufferObject::CopyTo(unsigned char* pbytes, DWORD lval)
{
	try
	{
		// NOTE: begin writing at start
		m_marshFile.SeekToBegin();

		m_marshFile.Write(pbytes, (UINT)lval);

		// NOTE: want to begin reading from buffer at start
		m_marshFile.SeekToBegin();
	}
	catch (CFileException e)
	{
		TRACE("CUnMarshallBufferObject::CopyTo: caught CFileException\n");
		return VWOBJECT_E_FILEEXCEPTION;
	}

	return S_OK;
}

STDMETHODIMP CUnMarshallBufferObject::get_ClassID(CLSID* pclsid)
{
	HRESULT hr = S_OK;

	// REVIEW: marshalling actual values should give better compression
	//    unsigned long Data1;
	//    unsigned short Data2;
	//    unsigned short Data3;
	//    unsigned char Data4[8];

	hr = get_ULong(&pclsid->Data1);
	if (FAILED(hr))
		return hr;

	hr = get_UShort(&pclsid->Data2);
	if (FAILED(hr))
		return hr;

	hr = get_UShort(&pclsid->Data3);
	if (FAILED(hr))
		return hr;

	hr = get_Bytes(pclsid->Data4, 8);

	return hr;
}

STDMETHODIMP CUnMarshallBufferObject::get_Byte(unsigned char* bval)
{
	return get_Bytes(bval, sizeof(unsigned char));
}

STDMETHODIMP CUnMarshallBufferObject::get_Bytes(unsigned char* pbytes, long lval)
{
	if (lval == 0)
		return S_OK;

	try
	{
		UINT nActual = 0;
		nActual = m_marshFile.Read(pbytes, (UINT)lval);

		ASSERT((int)nActual == lval);

		if ((int)nActual != lval)
		{
			TRACE("CUnMarshallBufferObject::get_Bytes: failed to read correct number of bytes, actual %d, requested %d\n", nActual, lval);
			return VWOBJECT_E_FILEREADERROR;
		}
	}
	catch (CFileException e)
	{
		TRACE("CUnMarshallBufferObject::get_Bytes: caught CFileException\n");
		return VWOBJECT_E_FILEEXCEPTION;
	}

	return S_OK;
}

STDMETHODIMP CUnMarshallBufferObject::get_Boolean(VARIANT_BOOL* pboolval)
{
	HRESULT hr = S_OK;
	unsigned char bval;

	ASSERT(pboolval);
	if (pboolval == NULL)
	{
		TRACE("CUnMarshallBufferObject::get_Boolean: outgoing pointer (VARIANT_BOOL*) is NULL\n");
		return E_POINTER;
	}

	*pboolval = VARIANT_FALSE;

	hr = get_UChar(&bval);
	if (FAILED(hr))
		return hr;

	*pboolval = (bval) ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CUnMarshallBufferObject::get_String(BSTR* pbstr)
{
	HRESULT hr = S_OK;
	int len;

	ASSERT(pbstr);
	if (pbstr == NULL)
	{
		TRACE("CUnMarshallBufferObject::get_String: outgoing pointer (BSTR*) is NULL\n");
		return E_POINTER;
	}

	*pbstr = NULL;

	hr = get_Int(&len);
	if (FAILED(hr))
		return hr;

	// NULL string or empty string BLOB
	if (len == 0)
	{
		CComBSTR bstr("");

		// return empty string
		*pbstr = bstr.Detach();
	}
#ifdef USE_ATOM_COMPRESSION
	// well-known string - lookup in string map
	else if (len < 0)
	{
		int iAtom = -len;
		CComBSTR bstr;

		if (!CVWCommObject::m_stringMap.Lookup(iAtom, (CBSTRAdapt<CComBSTR>&)bstr))
		{
			hr = E_FAIL; // VWOBJECT_E_INVALIDATOM;
			ASSERT(FALSE);
			return hr;
		}

//		TRACE("CUnMarshallBufferObject::get_String: atom %d, string %s\n", iAtom, CString(bstr));

		*pbstr = bstr.Detach();
	}
#endif
	else
	{
		BSTR bstr = ::SysAllocStringLen(NULL, len);  // the allocation null terminates this puppy

		if (!bstr)
			return E_OUTOFMEMORY; // mem allocation error

		*pbstr = bstr;

		for (int i = 0; i< len && SUCCEEDED(hr); i++)
			 hr = get_UShort(&bstr[i]);
	}

	return hr;
}

STDMETHODIMP CUnMarshallBufferObject::get_BLOB(BSTR* pblob)
{
	HRESULT hr = S_OK;
	unsigned int len;

	ASSERT(pblob);
	if (pblob == NULL)
	{
		TRACE("CUnMarshallBufferObject::get_BLOB: outgoing pointer (BSTR*) is NULL\n");
		return E_POINTER;
	}

	hr = get_UInt(&len);
	if (FAILED(hr))
		return hr;
	else if (len == 0)
		*pblob = NULL;
	else
	{
		*pblob = ::SysAllocStringByteLen(NULL, len); 
	
		if (!(*pblob))
			return E_FAIL;
		
		hr = get_Bytes((unsigned char *)*pblob, len);
		if (FAILED(hr))
			return hr;
	}

	return S_OK;
}

STDMETHODIMP CUnMarshallBufferObject::get_UChar(unsigned char* pucval)
{
	HRESULT hr = S_OK;
	unsigned int uival;

	ASSERT(pucval);
	if (pucval == NULL)
	{
		TRACE("CUnMarshallBufferObject::get_UChar: outgoing pointer (unsigned char*) is NULL\n");
		return E_POINTER;
	}

	*pucval = 0;

	hr = get_UInt(&uival);
	if (FAILED(hr))
		return hr;

	*pucval = uival;

	return hr;
}

STDMETHODIMP CUnMarshallBufferObject::get_UShort(unsigned short* pusval)
{
	HRESULT hr = S_OK;
	unsigned int uival;

	ASSERT(pusval);
	if (pusval == NULL)
	{
		TRACE("CUnMarshallBufferObject::get_UShort: outgoing pointer (unsigned short*) is NULL\n");
		return E_POINTER;
	}

	*pusval = 0;

	hr = get_UInt(&uival);
	if (FAILED(hr))
		return hr;

	*pusval = uival;

	return hr;
}

STDMETHODIMP CUnMarshallBufferObject::get_Short(short* psval)
{
	HRESULT hr = S_OK;
	int ival;

	ASSERT(psval);
	if (psval == NULL)
	{
		TRACE("CUnMarshallBufferObject::get_Short: outgoing pointer (short*) is NULL\n");
		return E_POINTER;
	}

	*psval = 0;

	hr = get_Int(&ival);
	if (FAILED(hr))
		return hr;

	*psval = ival;

	return hr;
}

STDMETHODIMP CUnMarshallBufferObject::get_Float(float* pfval)
{
	return get_Bytes((unsigned char*)pfval, sizeof(float));
}

STDMETHODIMP CUnMarshallBufferObject::get_Double(double* pdval)
{
	return get_Bytes((unsigned char*)pdval, sizeof(double));
}

STDMETHODIMP CUnMarshallBufferObject::get_UInt(unsigned int* puival)
{
	HRESULT hr = S_OK;
	unsigned char c;

	ASSERT(puival);
	if (puival == NULL)
	{
		TRACE("CUnMarshallBufferObject::get_UInt: outgoing pointer (unsigned int*) is NULL\n");
		return E_POINTER;
	}

	*puival = 0;

	do {
		hr = get_Byte(&c);
		*puival = (*puival << 7) + (c & 0x7F);
	} while (c < 128 && SUCCEEDED(hr));

	return hr;
}

STDMETHODIMP CUnMarshallBufferObject::get_Int(int* pival)
{
	HRESULT hr = S_OK;
	unsigned int uival;

	ASSERT(pival);
	if (pival == NULL)
	{
		TRACE("CUnMarshallBufferObject::get_Int: outgoing pointer (int*) is NULL\n");
		return E_POINTER;
	}

	*pival = 0;

	hr = get_UInt(&uival);
	if (FAILED(hr))
		return hr;

	if (uival & 1) {
		*pival = -(int)(uival >> 1);
		if (*pival == 0)				// handle the extra, smallest negative number
			*pival = 0x10000000;
	} else
		*pival = uival >> 1;

	return hr;
}

STDMETHODIMP CUnMarshallBufferObject::get_ULong(unsigned long* pulval)
{
	ASSERT(sizeof(unsigned int) == sizeof(unsigned long));
	
	return get_UInt((unsigned int*)pulval);
}

STDMETHODIMP CUnMarshallBufferObject::get_Long(long* plval)
{
	ASSERT(sizeof(int) == sizeof(long));

	return get_Int((int*)plval);
}

STDMETHODIMP CMarshallBufferObject::put_ClassID(REFCLSID clsid)
{
	HRESULT hr = S_OK;

	// REVIEW: marshalling actual values should give better compression
	//    DWORD Data1;
	//    WORD Data2;
	//    WORD Data3;
	//    BYTE Data4[8];

	hr = put_ULong(clsid.Data1);
	if (FAILED(hr))
		return hr;

	hr = put_UShort(clsid.Data2);
	if (FAILED(hr))
		return hr;

	hr = put_UShort(clsid.Data3);
	if (FAILED(hr))
		return hr;

	hr = put_Bytes((unsigned char *)clsid.Data4, 8);

	return hr;
}

STDMETHODIMP CMarshallBufferObject::put_Byte(unsigned char bval)
{
	// NOTE: inlining for performance

	try
	{
		m_marshFile.Write(&bval, sizeof(unsigned char));
	}
	catch (CFileException e)
	{
		TRACE("CMarshallBufferObject::put_Byte: caught CFileException\n");
		return VWOBJECT_E_FILEEXCEPTION;
	}

	return S_OK;
}

STDMETHODIMP CMarshallBufferObject::put_Bytes(unsigned char* pbytes, long lval)
{
	ASSERT(pbytes);
	if (pbytes == NULL)
	{
		TRACE("CUnMarshallBufferObject::put_Bytes: incoming argument (unsigned char*) is NULL\n");
		return E_INVALIDARG;
	}

	if (lval == 0)
		return S_OK;

	try
	{
		m_marshFile.Write(pbytes, (UINT)lval);
	}
	catch (CFileException e)
	{
		TRACE("CMarshallBufferObject::put_Bytes: caught CFileException\n");
		return VWOBJECT_E_FILEEXCEPTION;
	}

	return S_OK;
}

STDMETHODIMP CMarshallBufferObject::put_Boolean(VARIANT_BOOL boolval)
{
	return put_UChar((boolval == VARIANT_TRUE) ? 0x1 : 0x0);
}

STDMETHODIMP CMarshallBufferObject::put_String(BSTR bstr, VARIANT_BOOL bCompress)
{
	HRESULT hr = S_OK;
	int iAtom = 0;
	int len = (bstr != NULL) ? ::SysStringLen(bstr) : 0;

	// NULL string or empty string
	if (len == 0)
	{
		hr = put_Int(0);
		if (FAILED(hr))
			return hr;
	}
#ifdef USE_ATOM_COMPRESSION
	// well-known string - lookup in atom map
	else if (bCompress == VARIANT_TRUE && CVWCommObject::m_atomMap.Lookup(bstr, iAtom))
	{
		ASSERT(iAtom != 0);

//		TRACE("CMarshallBufferObject::put_String: atom %d, string %s\n", iAtom, CString(bstr));

		hr = put_Int(-iAtom);
		if (FAILED(hr))
			return hr;
	}
#endif
	else
	{
#ifdef _DEBUG
//		TRACE("CMarshallBufferObject::put_String: unknown string %s\n", CString(bstr));
#endif

		hr = put_Int(len);
		if (FAILED(hr))
			return hr;

		for (int i = 0; (i<len) && SUCCEEDED(hr); i++)
			hr = put_UShort(bstr[i]);
	}

	return hr;
}

STDMETHODIMP CMarshallBufferObject::put_Float(float fval)
{
	return put_Bytes((unsigned char*)&fval, sizeof(float));
}

STDMETHODIMP CMarshallBufferObject::put_Double(double dval)
{
	return put_Bytes((unsigned char*)&dval, sizeof(double));
}

STDMETHODIMP CMarshallBufferObject::put_UChar(unsigned char ucval)
{
	return put_UInt(ucval);
}

STDMETHODIMP CMarshallBufferObject::put_UShort(unsigned short usval)
{
	return put_UInt(usval);
}

STDMETHODIMP CMarshallBufferObject::put_Short(short sval)
{
	return put_Int(sval);
}

STDMETHODIMP CMarshallBufferObject::put_UInt(unsigned int uival)
{
	HRESULT hr = S_OK;
	unsigned char bval;

	if (uival >= 128*128*128*128)
	{
		bval = (uival >> 28) & 0x0F;
		hr = put_Byte(bval);
		if (FAILED(hr))
			return hr;
	}

	if (uival >= 128*128*128)		
	{
		bval = (uival >> 21) & 0x7F;
		hr = put_Byte(bval);
		if (FAILED(hr))
			return hr;
	}

	if (uival >= 128*128)			
	{
		bval = (uival >> 14) & 0x7F;
		hr = put_Byte(bval);
		if (FAILED(hr))
			return hr;
	}

	if (uival >= 128)				
	{
		bval = (uival >> 7 ) & 0x7F;
		hr = put_Byte(bval);
		if (FAILED(hr))
			return hr;
	}
								
	bval = (uival & 0x7F) | 128;
	hr = put_Byte(bval);

	return hr;
}

STDMETHODIMP CMarshallBufferObject::put_Int(int ival)
{
	unsigned int uival;

	if (ival < 0)
		uival = ((unsigned)(-ival) << 1) | 1;
	else
		uival = (unsigned)ival << 1;

	return put_UInt(uival);
}

STDMETHODIMP CMarshallBufferObject::put_ULong(unsigned long ulval)
{
	ASSERT(sizeof(unsigned int) == sizeof(unsigned long));

	return put_UInt(ulval);
}

STDMETHODIMP CMarshallBufferObject::put_Long(long lval)
{
	ASSERT(sizeof(int) == sizeof(long));

	return put_Int(lval);
}

STDMETHODIMP CMarshallBufferObject::put_BLOB(BSTR blobval)
{
	HRESULT hr;

	// length
	DWORD	len = ::SysStringByteLen(blobval);
	hr = put_UInt(len);
	if (FAILED(hr))
		return hr;

	if (blobval)
		return put_Bytes((unsigned char *)blobval, len);
	else
		return S_OK;
}

STDMETHODIMP CMarshallBufferObject::CopyFrom(unsigned char* pbytes, DWORD lval)
{
	try
	{
		// NOTE: start copying at start
		m_marshFile.SeekToBegin();

		UINT nActual = 0;
		nActual = m_marshFile.Read(pbytes, (UINT)lval);

//		if (nActual != (UINT)lval)
//			return VWOBJECT_E_FILEREADERROR;

		// NOTE: reset file ptr to beginning
		m_marshFile.SeekToBegin();
	}
	catch (CFileException e)
	{
		TRACE("CMarshallBufferObject::CopyFrom: caught CFileException\n");
		return VWOBJECT_E_FILEEXCEPTION;
	}

	return S_OK;
}

STDMETHODIMP CUnMarshallBufferObject::ReadFromDisk(BSTR bstrFilename)
{	
	CString strFilename(bstrFilename);
	DWORD lSize = 0;
	HRESULT hr = S_OK;
	DWORD lReadSize;
	unsigned char* pbuffer = NULL;
	VARIANT_BOOL bExists;
	CFile file;

	// force lowercase
	strFilename.MakeLower();

	// if it doesn't have file extension, add it
	if (strFilename.Find(".vwc") == -1)
		strFilename += ".vwc";

	bExists = (_access(strFilename, 0) == 0) ? VARIANT_TRUE : VARIANT_FALSE;

	// if the file doesn't exist or overwrite is VARIANT_TRUE, create it and call OnCreateWorld
	if (bExists == VARIANT_FALSE)
	{
#ifdef _DEBUG
		TRACE("CMarshallBufferObject::ReadFromDisk: file (%s) doesn't exist\n", CString(bstrFilename));
#endif
		hr = VWOBJECT_E_FILEREADERROR;
		goto ERROR_ENCOUNTERED;
	}

	// file exists, open and process it

	// open log file
	if (!file.Open(	strFilename, 
					CFile::modeCreate		|	
					CFile::modeNoTruncate	|
					CFile::modeReadWrite	|
					CFile::shareExclusive	|
					CFile::typeBinary ))
	{
#ifdef _DEBUG
		TRACE("CMarshallBufferObject::ReadFromDisk: file (%s) doesn't exist\n", CString(bstrFilename));
#endif
		hr = VWOBJECT_E_FILEREADERROR;
		goto ERROR_ENCOUNTERED;
	}

	// read header byte (stop if eof)
	lReadSize = file.Read(&lSize, sizeof(lSize));
	if (lReadSize == 0)
	{
#ifdef _DEBUG
		TRACE("CMarshallBufferObject::ReadFromDisk: failed to read header\n");
#endif
		hr = VWOBJECT_E_FILEREADERROR;
		goto ERROR_ENCOUNTERED;
	}

	if (lReadSize != sizeof(lSize))
	{
#ifdef _DEBUG
		TRACE("CMarshallBufferObject::ReadFromDisk: incorrect header\n");
#endif
		hr = VWOBJECT_E_FILEREADERROR;
		goto ERROR_ENCOUNTERED;
	}

	// allocate memory
	pbuffer = new BYTE[lSize];
	if (pbuffer == NULL)
	{
		hr = E_OUTOFMEMORY;
		goto ERROR_ENCOUNTERED;
	}

	// read
	lReadSize = file.Read(pbuffer, lSize);
	if (lReadSize != lSize)
	{	
#ifdef _DEBUG
		TRACE("CMarshallBufferObject::ReadFromDisk: failed to read data\n");
#endif
		hr = VWOBJECT_E_FILEREADERROR;
		goto ERROR_ENCOUNTERED;
	}

	// attach the buffer to the Object memory
	hr = CopyTo(pbuffer, lSize);

ERROR_ENCOUNTERED:
	if (pbuffer)
		delete [] pbuffer;

	return hr;
}

STDMETHODIMP CMarshallBufferObject::WriteToDisk(BSTR bstrFilename)
{
	HRESULT hr = S_OK;
	CString strFilename(bstrFilename);
	CString strBackup = strFilename + ".bak";
	VARIANT_BOOL bExists = (_access(strBackup, 0) == 0) ? VARIANT_TRUE : VARIANT_FALSE;
	DWORD lSize = 0;
	unsigned char* pbuffer = NULL;
	CFile file;

	// kill off backup if it exists
	if (bExists == VARIANT_TRUE)
	{
		TRY
		{
			CFile::Remove( strBackup );
		}
		CATCH( CFileException, e )
		{
			// no-op
		}
		END_CATCH
	}

	// rename current db to backup
	TRY
	{
		CFile::Rename( strFilename, strBackup );
	}
	CATCH( CFileException, e )
	{
	}
	END_CATCH

	// create log file
	if (!file.Open(	strFilename, 
					CFile::modeCreate		|	
					CFile::modeReadWrite	|
					CFile::shareExclusive	|
					CFile::typeBinary ))
	{
		hr = VWOBJECT_E_FILEEXCEPTION;
		goto ERROR_ENCOUNTERED;
	}

	lSize = m_marshFile.GetLength();

	// allocate memory
	pbuffer = new BYTE[lSize];
	if (pbuffer == NULL)
	{
		hr = VWOBJECT_E_FILEEXCEPTION;
		goto ERROR_ENCOUNTERED;
	}

	// detach marshbuf so we can write it to disk
	hr = CopyFrom(pbuffer, lSize);

	// write out the length of the buffer and the buffer itself
	file.Write(&lSize, sizeof(lSize));
	file.Write(pbuffer, lSize);

ERROR_ENCOUNTERED:
	if (pbuffer)
		delete [] pbuffer;

	return hr;
}
