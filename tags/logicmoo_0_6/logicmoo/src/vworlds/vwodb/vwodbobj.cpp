// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWOdbObj.cpp : Implementation of CVWOdbApp and DLL registration.

#include "stdafx.h"
#include <vwobject.h>
#include <io.h>
#include <vwodb.h>
#include "VWOdbObj.h"
#include "global.h"
#include "reghelp.h"
#include "syshelp.h"

EXTERN_C const CLSID CLSID_MarshallBuffer;
EXTERN_C const CLSID CLSID_UnMarshallBuffer;

// REVIEW: is this going to be pokey (to flush every time?)
//#define FLUSHEVERYTIME

// define this to print out every constructor/destructor
//#define TRACE_ALLOCATION VWT_NONE
#define TRACE_ALLOCATION VWT_METHOD

//#define TRACE_LOGGING VWT_NONE
#define TRACE_LOGGING VWT_METHOD

/////////////////////////////////////////////////////////////////////////////
// Parameters

// REVIEW: changed to use version number from SLM

// file delimiters
#define chObject	'O'
#define chLog		'L'
#define chWorld		'W'

/////////////////////////////////////////////////////////////////////////////
// construction/destruction

CVWOdb::CVWOdb() :
	m_bOpen(VARIANT_FALSE),
	m_bPersist(VARIANT_FALSE),
	m_bTemporary(VARIANT_FALSE),
	m_pWorld(NULL)
{
	VWTRACE(m_pWorld, "VWODB", TRACE_ALLOCATION, "CVWOdb::CVWOdb\n");
}

CVWOdb::~CVWOdb()
{
	VWTRACE(m_pWorld, "VWODB", TRACE_ALLOCATION, "CVWOdb::~CVWOdb\n");

	// complain if we're open
	if (m_bOpen==VARIANT_TRUE)
	{
		VWTRACE(m_pWorld, "VWODB", VWT_ERROR, "CVWOdb::~CVWOdb: database is still open.  Closing it.\n");
		CloseWorld();
	}

	// iterate through object map, release objects
	POSITION pos;
	long id;
	IThing *pThing;

	// iterate through objects, telling them
	// to terminate themselves (release back pointers)
	VWTRACE(m_pWorld, "VWODB", TRACE_ALLOCATION, "CVWOdb::~CVWOdb: terminating objects\n");

	for (pos=m_mapIdToThing.GetStartPosition(); pos!=NULL; )
	{
		// get the object pointer
		m_mapIdToThing.GetNextAssoc(pos, id, pThing);

		ASSERT(pThing != NULL);

		if (id != WORLD_OBJECT)
		{
			VWTRACE(m_pWorld, "VWODB", TRACE_ALLOCATION, "CVWOdb::~CVWOdb: calling IThing::Terminate, id %d\n", id);

			pThing->Terminate();
		}
	}

	VWTRACE(m_pWorld, "VWODB", TRACE_ALLOCATION, "CVWOdb::~CVWOdb: terminating global object\n");

	// terminate the global object last
	if (m_mapIdToThing.Lookup(WORLD_OBJECT, pThing))
	{
		ASSERT(pThing != NULL);

		VWTRACE(m_pWorld, "VWODB", TRACE_ALLOCATION, "CVWOdb::~CVWOdb: calling IThing::Terminate, id %d\n", WORLD_OBJECT);

		pThing->Terminate();
	}

	// now release stuff
	for (pos=m_mapIdToThing.GetStartPosition(); pos!=NULL; )
	{
		// get the object pointer
		m_mapIdToThing.GetNextAssoc(pos, id, pThing);

		ASSERT(pThing != NULL);

		// release it
		if (id != WORLD_OBJECT)
		{
			VWTRACE(m_pWorld, "VWODB", TRACE_ALLOCATION, "CVWOdb::~CVWOdb: calling IThing::Release, id %d\n", id);

			pThing->Release();
		}
	}

	// release the global object last
	if (m_mapIdToThing.Lookup(WORLD_OBJECT, pThing))
	{
		ASSERT(pThing != NULL);

		VWTRACE(m_pWorld, "VWODB", TRACE_ALLOCATION, "CVWOdb::~CVWOdb: calling IThing::Release, id %d\n", WORLD_OBJECT);

		pThing->Release();
	}

	// release world
	SAFERELEASE(m_pWorld);

	// empty the collection
	m_mapIdToThing.RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////
// ErrorInfo

STDMETHODIMP CVWOdb::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IVWObjectRepository
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// IVWObjectRepository

STDMETHODIMP CVWOdb::Initialize(IWorld *pWorld, BSTR bstrConnection)
{
	VWTRACE(m_pWorld, "VWODB", VWT_METHOD, "CVWOdb::Initialize\n");

	// set database and persist flags
	m_bPersist = VARIANT_FALSE;
	m_bTemporary = VARIANT_FALSE;

	// if we intend to be in persist mode
	if (bstrConnection != NULL)
	{
		m_strDb = bstrConnection;

		// if valid filename, set persist mode
		if (m_strDb.GetLength() == 0)
			return ReportODBError(VWODB_E_DBCREATEERROR);

		// see if temporary world name
		char szFName[_MAX_FNAME];
		_splitpath(m_strDb, NULL, NULL, szFName, NULL);
		if (szFName[0]=='~')
			m_bTemporary = VARIANT_TRUE;
		else
			m_bPersist = VARIANT_TRUE;
		
		m_strName = szFName;
	}

	// stow world
	m_pWorld = pWorld;
	SAFEADDREF(m_pWorld);

	return S_OK;
}

STDMETHODIMP CVWOdb::OpenWorld(VARIANT_BOOL bCreate, VARIANT_BOOL bOverwrite)
{
	HRESULT hr = S_OK;
	int		cb;
	char	chOp;
	VARIANT_BOOL bOpenWorld = VARIANT_FALSE;
	DWORD	dw;
	IUnMarshallBuffer *pbuffer=NULL;
	VARIANT_BOOL bExists;
#ifdef _DEBUG
	DWORD dwPos = 0;
#endif

	VWTRACE(m_pWorld, "VWODB", VWT_METHOD, "CVWOdb::OpenWorld\n");

	// if already open, then fail
	if (m_bOpen == VARIANT_TRUE)
		return ReportODBError(VWODB_E_ALREADYOPEN);

	// Must set status to open or things below will fail - set
	// back to closed if fail along the way
	m_bOpen = VARIANT_TRUE;

	// if we're not in "persist" mode, there's nothing to do.
	if (m_bPersist == VARIANT_FALSE)
	{
		// call OnCreateWorld for temporary worlds (need all the one-time initialization)
		if (m_bTemporary == VARIANT_TRUE)
			hr = m_pWorld->OnCreateWorld();
		else
			// callback to world for initialization
			hr = m_pWorld->OnOpenWorld(VARIANT_FALSE);

		return hr;
	}

	// check for empty filename (only if in persist mode)
	if (m_strDb.GetLength() == 0)
	{
		m_bOpen = VARIANT_FALSE;
		return ReportODBError(VWODB_E_INVALIDDBNAME);
	}

	// force lowercase
	m_strDb.MakeLower();

	// HACK: if it doesn't have file extension, add it
	if (m_strDb.Find(".vwc") == -1)
		m_strDb += ".vwc";

	// if length of filename plus 4 chars (for .bak) is greater 
	// than _MAX_PATH, then fail
	if (m_strDb.GetLength() + 4 > _MAX_PATH)
	{
		m_bOpen = VARIANT_FALSE;
		return ReportODBError(VWODB_E_INVALIDDBNAME);
	}

	VWTRACE(m_pWorld, "VWODB", VWT_METHOD, "CVWOdb::OpenWorld: opening log file: %s\n", m_strDb);

	CComBSTR bstrTemp;
	
	// don't bother with if exists on disk. 
	// bExists = (_access(m_strDb, 0) == 0) ? VARIANT_TRUE : VARIANT_FALSE;
	hr = FindWorldByPath(CComBSTR(m_strName), &bstrTemp.m_str);
	
	bExists = (bstrTemp) ? VARIANT_TRUE : VARIANT_FALSE;

	if (bCreate == VARIANT_TRUE)
	{
		if (bExists == VARIANT_TRUE && bOverwrite == VARIANT_FALSE)
		{
			m_bOpen = VARIANT_FALSE;
			return ReportODBError(VWODB_E_DBALREADYEXIST);
		}

		// get here if bExists == VARIANT_FALSE || (bExists == VARIANT_TRUE && bOverwrite == VARIANT_TRUE)

		//this deletes the old version.
		// open log file
		if (!m_fileDb.Open(	m_strDb, 
							CFile::modeCreate		|	
							CFile::modeReadWrite	|
							CFile::shareExclusive	|
							CFile::typeBinary ))
		{
			VWTRACE(m_pWorld, "VWODB", VWT_ERROR, "CVWOdb::OpenWorld: unable to create log file\n");
			m_bOpen = VARIANT_FALSE;
			return ReportODBError(VWODB_E_DBCREATEERROR);
		}

		// write stamp out to it
		dw = DATABASE_VERSION;
		m_fileDb.Write(&dw, sizeof(dw));

#ifdef FLUSHEVERYTIME
		m_fileDb.Flush();
#endif
		// callback to world for initialization
		hr = m_pWorld->OnCreateWorld();
		if (FAILED(hr))
		{
			VWTRACE(m_pWorld, "VWODB", VWT_ERROR, "CVWOdb::OpenWorld: OnCreateWorld failed\n");
			m_bOpen = VARIANT_FALSE;
			return hr;
		}

		return S_OK;
	}
	// regular open case
	else if (bCreate == VARIANT_FALSE)
	{
		if (bExists == VARIANT_FALSE)
		{
			// want to open file, but it doesn't exist in the registry.
			
			//check if exists on disk.
			VARIANT_BOOL bExistsOnDisk = (_access(m_strDb, 0) == 0) ? VARIANT_TRUE : VARIANT_FALSE;

			if (bExistsOnDisk == VARIANT_FALSE)
			{
				m_bOpen = VARIANT_FALSE;

				return ReportODBError(VWODB_E_DBNOTEXIST);
			}
			else if (bExistsOnDisk == VARIANT_TRUE)
			{
				//do nothing.
			}
			else
				ASSERT(0);
		}
		else if (bExists == VARIANT_TRUE)
		{
			// fail if attempting to open with overwrite
			if (bOverwrite == VARIANT_TRUE)		
			{
				//you're never supposed to do this.
				ASSERT(0);
				m_bOpen = VARIANT_FALSE;

				return ReportODBError(VWODB_E_DBNOTEXIST);
			}
			else if (bOverwrite == VARIANT_FALSE)		
			{
				//do nothing.
			}
			else 
				ASSERT(0);
		}
		else 
			ASSERT(0);
	}
	else 
		ASSERT(0);

	// file exists, open and process it

	// open log file
	if (!m_fileDb.Open(	m_strDb, 
						CFile::modeCreate		|	
						CFile::modeNoTruncate	|
						CFile::modeReadWrite	|
						CFile::shareExclusive	|
						CFile::typeBinary ))
	{
		VWTRACE(m_pWorld, "VWODB", VWT_ERROR, "CVWOdb::OpenWorld: unable to open log file\n");
		m_bOpen = VARIANT_FALSE;
		return ReportODBError(VWODB_E_DBOPENERROR);
	}

	// read the stamp
	cb = m_fileDb.Read(&dw, sizeof(dw));
	if (cb != sizeof(dw))
	{
		VWTRACE(m_pWorld, "VWODB", VWT_ERROR, "CVWOdb::OpenWorld: unable to read stamp\n");
		m_bOpen = VARIANT_FALSE;
		return ReportODBError(VWODB_E_DBREADERROR);
	}

	if (dw != DATABASE_VERSION)
	{
		VWTRACE(m_pWorld, "VWODB", VWT_ERROR, "CVWOdb::OpenWorld: incorrect log stamp\n");
		m_bOpen = VARIANT_FALSE;
		return ReportODBError(VWODB_E_INVALIDDBSIGNATURE);
	}

	// read worlds, objects and log records
	for(;;)
	{
#ifdef _DEBUG
		dwPos = m_fileDb.GetPosition();
		VWTRACE(m_pWorld, "VWODB", TRACE_LOGGING, "CVWOdb::OpenWorld: header byte position %d\n", dwPos);
#endif

		// read header byte (stop if eof)
		cb = m_fileDb.Read(&chOp, sizeof(chOp));
		if (cb == 0)
			break;

		if (cb != sizeof(chOp))
		{
			VWTRACE(m_pWorld, "VWODB", VWT_ERROR, "CVWOdb::OpenWorld: incorrect header byte size\n");
			m_bOpen = VARIANT_FALSE;
			return ReportODBError(VWODB_E_DBREADERROR);
		}

		// process
		if (chOp == chObject || chOp == chWorld)
		{
			// unmarshal object

#ifdef _DEBUG
			dwPos = m_fileDb.GetPosition();
			VWTRACE(m_pWorld, "VWODB", TRACE_LOGGING, "CVWOdb::OpenWorld: object record position %d\n", dwPos);
#endif

			// read the length 
			int cbObject;
			cb = m_fileDb.Read(&cbObject, sizeof(cbObject));

			if (cb != sizeof(cbObject))
			{
				VWTRACE(m_pWorld, "VWODB", VWT_ERROR, "CVWOdb::OpenWorld: incorrect header byte size\n");
				m_bOpen = VARIANT_FALSE;
				return ReportODBError(VWODB_E_DBREADERROR);
			}

			// read the record
			VWTRACE(m_pWorld, "VWODB", TRACE_LOGGING, "CVWOdb::OpenWorld: read object, %d bytes (plus %d bytes)\n", cbObject, sizeof(cbObject));

			// allocate memory
			BYTE *pbObject;
			pbObject = new BYTE[cbObject];
			if (pbObject == NULL)
			{
				VWTRACE(m_pWorld, "VWODB", VWT_ERROR, "CVWOdb::OpenWorld: failed to allocate object buffer\n");
				m_bOpen = VARIANT_FALSE;
				return ReportODBError(E_OUTOFMEMORY);
			}

			// read
			cb = m_fileDb.Read(pbObject, cbObject);
			if (cb != cbObject)
			{
				VWTRACE(m_pWorld, "VWODB", VWT_ERROR, "CVWOdb::OpenWorld: failed to read object\n");
				delete [] pbObject;
				m_bOpen = VARIANT_FALSE;
				return ReportODBError(VWODB_E_DBREADERROR);
			}

			// create an unmarshal buffer
			hr = CreateUnMarshallBuffer(&pbuffer);
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "VWODB", VWT_ERROR, "CVWOdb::OpenWorld: failed to create unmarshall buffer\n");
				delete [] pbObject;
				m_bOpen = VARIANT_FALSE;
				return ReportODBError(hr);
			}

			// attach the buffer to the Object memory
			pbuffer->Attach(pbObject, cbObject);

			// unmarshal the object
			IThing *pthing;
            hr = m_pWorld->UnMarshallThing(pbuffer, &pthing);
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "VWODB", VWT_ERROR, "CVWOdb::OpenWorld: failed to unmarshall thing\n");
				SAFERELEASE(pbuffer);
				delete [] pbObject;
				m_bOpen = VARIANT_FALSE;
				return hr;
			}

			// release the object. The unmarshal call returns an interface, but we
			// don't really need it - the important thing is the side of effect of
			// creating new objects that don't exist
			pthing->Release();

			// free the memory
			SAFERELEASE(pbuffer);
			delete [] pbObject;

			// should get called only once
			if (chOp == chWorld && bOpenWorld == VARIANT_FALSE)
			{
				// callback to world to tell it it's been loaded
				hr = m_pWorld->OnOpenWorld(VARIANT_TRUE);
				if (FAILED(hr))
				{
					VWTRACE(m_pWorld, "VWODB", VWT_ERROR, "CVWOdb::OpenWorld: OnOpenWorld failed\n");
					m_bOpen = VARIANT_FALSE;
					return hr;
				}

				bOpenWorld = VARIANT_TRUE;
			}
		}
		else if (chOp == chLog)
		{
			// unmarshal log record

#ifdef _DEBUG
			dwPos = m_fileDb.GetPosition();
			VWTRACE(m_pWorld, "VWODB", TRACE_LOGGING, "CVWOdb::OpenWorld: log record position %d\n", dwPos);
#endif

			// REVIEW: do we have to catch file exceptions for EOF?
			// REVIEW: handle file exceptions for other cases
			long id;
			cb = m_fileDb.Read(&id, sizeof(id));

			// eof?
			if (cb == 0)
				break;

			// bogus?
			if (cb != sizeof(id))
			{
				VWTRACE(m_pWorld, "VWODB", VWT_ERROR, "CVWOdb::OpenWorld: invalid log record object id\n");
				m_bOpen = VARIANT_FALSE;
				return ReportODBError(VWODB_E_DBREADERROR);
			}

			// read the record size
			int cbRecord;
			cb = m_fileDb.Read(&cbRecord, sizeof(cbRecord));
			if (cb != sizeof(cbRecord))
			{
				VWTRACE(m_pWorld, "VWODB", VWT_ERROR, "CVWOdb::OpenWorld: failed to read log record size\n");
				m_bOpen = VARIANT_FALSE;
				return ReportODBError(VWODB_E_DBREADERROR);
			}

			// read the record
			VWTRACE(m_pWorld, "VWODB", TRACE_LOGGING, "CVWOdb::OpenWorld: read log record, id %d, %d bytes (plus %d bytes)\n", id, cbRecord, sizeof(id) + sizeof(cbRecord));

			// allocate memory
			BYTE *pbRecord;
			pbRecord = new BYTE[cbRecord];
			if (pbRecord == NULL)
			{
				VWTRACE(m_pWorld, "VWODB", VWT_ERROR, "CVWOdb::OpenWorld: failed to allocate log record buffer\n");
				m_bOpen = VARIANT_FALSE;
				return ReportODBError(E_OUTOFMEMORY);
			}

			// read
			cb = m_fileDb.Read(pbRecord, cbRecord);
			if (cb != cbRecord)
			{
				VWTRACE(m_pWorld, "VWODB", VWT_ERROR, "CVWOdb::OpenWorld: failed to read log record\n");
				delete [] pbRecord;
				m_bOpen = VARIANT_FALSE;
				return ReportODBError(VWODB_E_DBREADERROR);
			}

			// create an unmarshal buffer 
			hr = CreateUnMarshallBuffer(&pbuffer);
			if (FAILED(hr))
			{
				VWTRACE(m_pWorld, "VWODB", VWT_ERROR, "CVWOdb::OpenWorld: failed to create unmarshall buffer\n");
				delete [] pbRecord;
				m_bOpen = VARIANT_FALSE;
				return ReportODBError(hr);
			}

			// attach the buffer to the record memory
			pbuffer->Attach(pbRecord, cbRecord);

			// REVIEW: need to bother with this check?
			// ProcessLogRecord will barf if object ID isn't correct
#ifdef _DEBUG
			if (id != WORLD_OBJECT)
			{
				// assure that the thing exists
				IThing *pThing;
				if (!m_mapIdToThing.Lookup(id, pThing))
				{
					SAFERELEASE(pbuffer);
					delete [] pbRecord;
					m_bOpen = VARIANT_FALSE;
					return ReportODBError(VWODB_E_OBJECTNOTEXIST);
				}
			}
#endif

			// process the record
			hr = m_pWorld->ProcessLogRecord(pbuffer);
			if (FAILED(hr))
			{
				VWREPORT(m_pWorld, VWR_ERROR, "CVWOdb::OpenWorld: ProcessLogRecord failed, id %d, hr %x\n", id, hr);

				SAFERELEASE(pbuffer);
				delete [] pbRecord;

//				m_bOpen = VARIANT_FALSE;
				return ReportODBError(VWODB_E_INVALIDDBRECORD);
			}	

			// free the memory
			SAFERELEASE(pbuffer);
			delete [] pbRecord;

		}
		else
		{
			VWTRACE(m_pWorld, "VWODB", VWT_ERROR, "CVWOdb::OpenWorld: unknown header byte\n");

//			m_bOpen = VARIANT_FALSE;
			return ReportODBError(VWODB_E_INVALIDDBRECORD);
		}

	}

	// done
	return S_OK;
}

STDMETHODIMP CVWOdb::CloseWorld()
{
	VWTRACE(m_pWorld, "VWODB", VWT_METHOD, "CVWOdb::CloseWorld\n");

	if (m_bPersist==VARIANT_TRUE)
	{
		// close the database file
		if (m_fileDb.m_hFile != (UINT)CFile::hFileNull)
			m_fileDb.Close();
	}

	// set status
	m_bOpen = VARIANT_FALSE;

	return S_OK;
}

STDMETHODIMP CVWOdb::CreateObject(IThing *pThing)
{
	HRESULT hr = S_OK;
	long id;

	VWTRACE(m_pWorld, "VWODB", VWT_METHOD, "CVWOdb::CreateObject\n");

	// must be open for business
	if (m_bOpen!=VARIANT_TRUE)
		return ReportODBError(VWODB_E_NOTOPEN);

	// get the object id
	/* hr = */ pThing->get_ID(&id);

	// verify that object doesn't already exist
	IThing *pThingTemp;
	if (m_mapIdToThing.Lookup(id, pThingTemp))
		return ReportODBError(VWODB_E_OBJECTNOTEXIST);

	VWTRACE(m_pWorld, "VWODB", VWT_METHOD, "CVWOdb::CreateObject (%d)\n", id);

	// increase the reference count
	pThing->AddRef();

	// add it to the map
	m_mapIdToThing.SetAt( id, pThing);
	
	return S_OK;
}

STDMETHODIMP CVWOdb::DeleteObject(int id)
{
	VWTRACE(m_pWorld, "VWODB", VWT_METHOD, "CVWOdb::DeleteObject (%d)\n", id);

	// must be open for business
	if (m_bOpen != VARIANT_TRUE)
		return ReportODBError(VWODB_E_NOTOPEN);

	// nuke from map, release object

	// look it up
	IThing *pThing;
	if (!m_mapIdToThing.Lookup(id, pThing))
		return ReportODBError(VWODB_E_OBJECTNOTEXIST);

	// delete the key
	m_mapIdToThing.RemoveKey(id);

#ifdef _DEBUG
	{
		DWORD dwRef = 0;
		CComBSTR bstrName("unknown");

		pThing->get_RefCount(&dwRef);
		pThing->get_Name(&bstrName.m_str);

		VWTRACE(m_pWorld, "VWODB", TRACE_ALLOCATION, "CVWOdb::DeleteObject (begin): this 0x%0x, id %d, name (%s), refcount %d\n",
			pThing, id, CString(bstrName), dwRef);
	}
#endif

	pThing->Terminate();

#ifdef _DEBUG
	{
		DWORD dwRef = 0;
		CComBSTR bstrName("unknown");

		pThing->get_RefCount(&dwRef);
		pThing->get_Name(&bstrName.m_str);

		VWTRACE(m_pWorld, "VWODB", TRACE_ALLOCATION, "CVWOdb::DeleteObject (terminated): this 0x%0x, id %d, name (%s), refcount %d\n",
			pThing, id, CString(bstrName), dwRef);
	}
#endif

	// release the object
	pThing->Release();

	return S_OK;
}

STDMETHODIMP CVWOdb::GetObject(int id, IThing **ppThing)
{
	VWTRACE(m_pWorld, "VWODB", VWT_METHOD, "CVWOdb::GetObject(%d)\n", id);

	// must be open for business
	if (m_bOpen!=VARIANT_TRUE)
		return ReportODBError(VWODB_E_NOTOPEN);

//	TRACE("CVWOdb::GetObject(%d)\n", id);

	// look it up
	if (m_mapIdToThing.Lookup(id, *ppThing))
	{
		(*ppThing)->AddRef();
		return S_OK;
	}
	else
		// NOTE: don't report since it happens a lot
		return VWODB_E_OBJECTNOTEXIST; // ReportODBError(VWODB_E_OBJECTNOTEXIST);
}

STDMETHODIMP CVWOdb::GetObjectByName(BSTR bstrName, IThing **ppThing)
{
	VWTRACE(m_pWorld, "VWODB", VWT_METHOD, "CVWOdb::GetObjectByName\n");

	// REVIEW: Shouldn't be called
	ASSERT(FALSE);

	// must be open for business
	if (m_bOpen!=VARIANT_TRUE)
		return ReportODBError(VWODB_E_NOTOPEN);

	return ReportODBError(E_NOTIMPL);
}

STDMETHODIMP CVWOdb::PutObject(IThing *pThing)
{
	VWTRACE(m_pWorld, "VWODB", VWT_METHOD, "CVWOdb::PutObject\n");

	// must be open for business
	if (m_bOpen!=VARIANT_TRUE)
		return ReportODBError(VWODB_E_NOTOPEN);

	// this is a nop
	return S_OK;
}

STDMETHODIMP CVWOdb::WriteLogRecord(int id, IMarshallBuffer *pbuffer)
{
	VWTRACE(m_pWorld, "VWODB", VWT_METHOD, "CVWOdb::WriteLogRecord(%d)\n", id);

	// must be open for business
	if (m_bOpen!=VARIANT_TRUE)
		return ReportODBError(VWODB_E_NOTOPEN);

	// if we're not in persist mode, we fail...
	if (m_bPersist!=VARIANT_TRUE)
	{
		// ...unless it's a temporary world, in which case we ignore
		if (m_bTemporary==VARIANT_TRUE)
			return S_OK;
		else
			return ReportODBError(VWODB_E_NOTPERSISTMODE);
	}

	// write out record
	// REVIEW: trap file exceptions

#ifdef _DEBUG
	// assure its in the active list
	IThing *pThing;
	if (id != 0 && !m_mapIdToThing.Lookup(id, pThing))
	{
		ASSERT(FALSE);
		return ReportODBError(VWODB_E_OBJECTNOTEXIST);
	}

#endif

	// write the op byte out the log file
	char chOp=chLog;
	m_fileDb.Write(&chOp, sizeof(char));

	// get the buffer pointer and length
	HRESULT hr = S_OK;
	unsigned long cch = 0;

	pbuffer->get_Length(&cch);

	// NOTE: using static buffer of 32kb

	ASSERT(cch <= COPYBUFFER_SIZE);
	if (cch > COPYBUFFER_SIZE)
	{
		hr = VWODB_E_LOGRECORDTOOLARGE;
		goto ERROR_ENCOUNTERED;
	}

	hr = pbuffer->CopyFrom(m_pCopyBuf, cch);
	if (FAILED(hr))
		goto ERROR_ENCOUNTERED;

	// write out id
	m_fileDb.Write(&id, sizeof(id));

	// write out length and record
	m_fileDb.Write(&cch, sizeof(cch));
	m_fileDb.Write(m_pCopyBuf, cch);

#ifdef FLUSHEVERYTIME
	m_fileDb.Flush();
#endif

ERROR_ENCOUNTERED:
	return hr;
}


STDMETHODIMP CVWOdb::Commit()
{
	HRESULT hr = S_OK;

	CString strBackup = m_strDb.Left(m_strDb.GetLength()-4);
	strBackup += ".bak.vwc";

	hr = CommitAs(CComBSTR(strBackup));
	
	return hr;
}

STDMETHODIMP CVWOdb::CommitAs(BSTR bstrFileName)
{
	HRESULT hr = S_OK;
	IThing *pThing = NULL;
	DWORD dw;

	VWTRACE(m_pWorld, "VWODB", VWT_METHOD, "CVWOdb::Commit\n");

	// must be open for business
	if (m_bOpen!=VARIANT_TRUE)
		return ReportODBError(VWODB_E_NOTOPEN);

	// if not in persist mode, we fail...
	if (m_bPersist!=VARIANT_TRUE)
	{
		// unless it's a temporary world, in which case we ignore
		if (m_bTemporary==VARIANT_TRUE)
			return S_OK;
		else
			return ReportODBError(VWODB_E_NOTPERSISTMODE);
	}

	CloseWorld();

	CString strBackup = CString(bstrFileName);

	VARIANT_BOOL bExists = (_access(strBackup, 0) == 0) ? VARIANT_TRUE : VARIANT_FALSE;

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
		CFile::Rename( m_strDb, strBackup );
	}
	CATCH( CFileException, e )
	{
		return ReportODBError(VWODB_E_BACKUPFAILED);
	}
	END_CATCH

	m_bOpen = VARIANT_TRUE;

	// create log file
	if (!m_fileDb.Open(	m_strDb, 
						CFile::modeCreate		|	
						CFile::modeReadWrite	|
						CFile::shareExclusive	|
						CFile::typeBinary ))
	{
		VWTRACE(m_pWorld, "VWODB", VWT_ERROR, "CVWOdb::Commit - unable to create log file\n");
		m_bOpen = VARIANT_FALSE;
		return ReportODBError(VWODB_E_DBCREATEERROR);
	}

	// write stamp out to it
	dw = DATABASE_VERSION;
	m_fileDb.Write(&dw, sizeof(dw));

#ifdef FLUSHEVERYTIME
	m_fileDb.Flush();
#endif

	// NOTE: need to differentiate world object so we can
	// callback world for notification of being loaded

	// write out the world object
	if (m_mapIdToThing.Lookup(WORLD_OBJECT, pThing))
	{
		unsigned long cb = 0;
		unsigned char* pb = NULL;
		char ch=chWorld;

		// create a marshal buffer
		IMarshallBuffer* pbuffer = NULL;

		hr = CreateMarshallBuffer(&pbuffer);
		if (FAILED(hr))
			return hr;

		// marshal the world object out
        hr = m_pWorld->MarshallThing(pThing, MARSHALL_BYID | MARSHALL_BYVALUE | MARSHALL_TODISK, pbuffer);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED_1;

		// detach marshbuf so we can write it to disk
		hr = pbuffer->Detach(&pb, &cb);

		// write out the world marker
		m_fileDb.Write(&ch, sizeof(char));

		// write out the length of the world and the world itself
		m_fileDb.Write(&cb, sizeof(cb));
		m_fileDb.Write(pb, cb);

		VWTRACE(m_pWorld, "VWODB", TRACE_LOGGING, "CVWOdb::Commit: writing WORLD_OBJECT, %d bytes\n", cb);

#ifdef FLUSHEVERYTIME
		m_fileDb.Flush();
#endif

		// NOTE: now IMarshallBuffer will free Detach-ed data
		// free stuff
//		free(pb);

ERROR_ENCOUNTERED_1:
		SAFERELEASE(pbuffer);

		if (FAILED(hr))
			return hr;
	}

	// iterate through all objects, writing them to the log file
	long id;
	POSITION pos;
	for (pos=m_mapIdToThing.GetStartPosition(); pos!=NULL; )
	{
		// get the object pointer
		m_mapIdToThing.GetNextAssoc(pos, id, pThing);

		// skip world object
		if (id != WORLD_OBJECT)
		{
			ASSERT(pThing != NULL);

			// write it
			hr = WriteObject(pThing);
			if (FAILED(hr))
				return hr;
		}
	}

	return S_OK;
}

STDMETHODIMP CVWOdb::RenameAs(BSTR bstrFileName)
{
	HRESULT hr = S_OK;
	IThing *pThing = NULL;
	DWORD dw;
	CString strDb = CString(bstrFileName);

	VWTRACE(m_pWorld, "VWODB", VWT_METHOD, "CVWOdb::Commit\n");

	// must be open for business
	if (m_bOpen!=VARIANT_TRUE)
		return ReportODBError(VWODB_E_NOTOPEN);

	// if not in persist mode, we fail...
	if (m_bPersist!=VARIANT_TRUE)
	{
		// unless it's a temporary world, in which case we ignore
		if (m_bTemporary==VARIANT_TRUE)
			return S_OK;
		else
			return ReportODBError(VWODB_E_NOTPERSISTMODE);
	}

	CloseWorld();

	CString strNewFile = strDb;
	VARIANT_BOOL bExists = (_access(strNewFile, 0) == 0) ? VARIANT_TRUE : VARIANT_FALSE;

	// kill off backup if it exists
	if (bExists == VARIANT_TRUE)
	{
		TRY
		{
			CFile::Remove( strNewFile );
		}
		CATCH( CFileException, e )
		{
			return ReportODBError(VWODB_E_BACKUPFAILED);
		}
		END_CATCH
	}

	m_bOpen = VARIANT_TRUE;

	// create log file
	if (!m_fileDb.Open(	strNewFile, 
						CFile::modeCreate		|	
						CFile::modeReadWrite	|
						CFile::shareExclusive	|
						CFile::typeBinary ))
	{
		VWTRACE(m_pWorld, "VWODB", VWT_ERROR, "CVWOdb::Commit - unable to create log file\n");
		m_bOpen = VARIANT_FALSE;
		return ReportODBError(VWODB_E_DBCREATEERROR);
	}

	m_strDb = strNewFile;

	// write stamp out to it
	dw = DATABASE_VERSION;
	m_fileDb.Write(&dw, sizeof(dw));

#ifdef FLUSHEVERYTIME
	m_fileDb.Flush();
#endif

	// NOTE: need to differentiate world object so we can
	// callback world for notification of being loaded

	// write out the world object
	if (m_mapIdToThing.Lookup(WORLD_OBJECT, pThing))
	{
		unsigned long cb = 0;
		unsigned char* pb = NULL;
		char ch=chWorld;

		// create a marshal buffer
		IMarshallBuffer* pbuffer = NULL;

		hr = CreateMarshallBuffer(&pbuffer);
		if (FAILED(hr))
			return hr;

		// marshal the world object out
        hr = m_pWorld->MarshallThing(pThing, MARSHALL_BYID | MARSHALL_BYVALUE | MARSHALL_TODISK, pbuffer);
		if (FAILED(hr))
			goto ERROR_ENCOUNTERED_1;

		// detach marshbuf so we can write it to disk
		hr = pbuffer->Detach(&pb, &cb);

		// write out the world marker
		m_fileDb.Write(&ch, sizeof(char));

		// write out the length of the world and the world itself
		m_fileDb.Write(&cb, sizeof(cb));
		m_fileDb.Write(pb, cb);

#ifdef FLUSHEVERYTIME
		m_fileDb.Flush();
#endif

		// NOTE: now IMarshallBuffer will free Detach-ed data
		// free stuff
//		free(pb);

ERROR_ENCOUNTERED_1:
		SAFERELEASE(pbuffer);

		if (FAILED(hr))
			return hr;
	}

	// iterate through all objects, writing them to the log file
	long id;
	POSITION pos;
	for (pos=m_mapIdToThing.GetStartPosition(); pos!=NULL; )
	{
		// get the object pointer
		m_mapIdToThing.GetNextAssoc(pos, id, pThing);

		// skip world object
		if (id != WORLD_OBJECT)
		{
			ASSERT(pThing != NULL);

			// write it
			hr = WriteObject(pThing);
			if (FAILED(hr))
				return hr;
		}
	}

	return S_OK;
}

STDMETHODIMP CVWOdb::DumpObjects()
{
	POSITION pos;
	long id;
	IThing *pThing = NULL;
	IPropertyMap* ppropmap = NULL;
	BSTR bstrName;
	ULONG cref;
	CString strName;
	VARIANT_BOOL bStub = VARIANT_FALSE;

	VWTRACE(m_pWorld, "VWODB", VWT_METHOD, "CVWOdb::DumpObjects\n");

	for (pos=m_mapIdToThing.GetStartPosition(); pos!=NULL; )
	{
		// get the object pointer
		m_mapIdToThing.GetNextAssoc(pos, id, pThing);

		ASSERT(pThing != NULL);

		// account for shutdown case where propmap would be NULL
		if (FAILED(pThing->get_InstanceProperties(&ppropmap)) || ppropmap == NULL)
		{
			strName = "(deleted)";
		}
		else
		{
			pThing->get_IsStub(&bStub);

			if (bStub == VARIANT_FALSE)
			{
				// display id, name, ref count
				pThing->get_Name(&bstrName);
				if (bstrName != NULL)
				{
					strName = (LPCWSTR) bstrName;
					::SysFreeString(bstrName);
				}
				else
					strName = "(No name prop)";
			}
			else
				strName = "(Stub)";
		}

		SAFERELEASE(ppropmap);

		pThing->get_ID(&id);
		pThing->get_RefCount(&cref);

		VWTRACE(m_pWorld, "VWODB", VWT_DUMP, "    Object (%d), name \"%s\", ref count %d\n", 
				id, (LPCSTR) strName, cref );
	}

	return S_OK;
}


/////////////////////////////////////////////////////////////////
// Helpers

HRESULT CVWOdb::WriteObject(IThing *pThing)
{
	// write object to the log file
	HRESULT hr;

	// marshal the object into a buffer

	// create a marshal buffer
	IMarshallBuffer* pbuffer = NULL;

	hr = CreateMarshallBuffer(&pbuffer);
	if (FAILED(hr))
		return hr;

	// now marshal the object into it
    hr = m_pWorld->MarshallThing(pThing, MARSHALL_BYID | MARSHALL_BYVALUE | MARSHALL_TODISK, pbuffer);
	if (FAILED(hr))
	{
		SAFERELEASE(pbuffer);
		return hr;
	}

	// get the pointer and length out
    unsigned long cbData;
    unsigned char* pbData;

    hr = pbuffer->Detach(&pbData, &cbData);
	if (FAILED(hr))
	{
		SAFERELEASE(pbuffer);
		return hr;
	}

	// write the op byte out the log file
	char chOp=chObject;
	m_fileDb.Write(&chOp, sizeof(chOp));

	// write the length out
	m_fileDb.Write(&cbData, sizeof(cbData));

	// write the data out
	m_fileDb.Write(pbData, cbData);

#ifdef _DEBUG
	long id = 0;
	/* hr = */ pThing->get_ID(&id);

	VWTRACE(m_pWorld, "VWODB", TRACE_LOGGING, "CVWOdb::WriteObject: writing object, id %d, %d bytes\n", id, cbData);
#endif

#ifdef FLUSHEVERYTIME
	m_fileDb.Flush();
#endif

	// NOTE: now IMarshallBuffer will free Detach-ed data
	// done
//	free(pbData);

 	SAFERELEASE(pbuffer);

	return S_OK;

}

#ifdef _DEBUG

void CrtSetBreakAlloc( long iAlloc )
{
	 _CrtSetBreakAlloc( iAlloc );
}

#endif

