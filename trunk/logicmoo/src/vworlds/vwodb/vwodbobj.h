// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWOdbObj.h : Declaration of the CVWOdb

#include <resource.h>
#include <afxtempl.h>

// V-Worlds optimized templates
#include <vwtempl.h>

#include <propbase.h>

EXTERN_C const CLSID CLSID_VWObjectRepository;

// NOTE: IWorld must now return good errors on ODB failures
#define ReportODBError(hr) SUCCEEDED(hr) ? hr : ReportError("VWSYSTEM.ObjectRepository", IID_IVWObjectRepository, hr, 0x600, 0x700, m_pWorld, VWT_ERROR)
//#define ReportODBError(hr) hr

#define COPYBUFFER_SIZE 32768

/////////////////////////////////////////////////////////////////////////////
// VWOdb

class ATL_NO_VTABLE CVWOdb :
	public IDispatchImpl<IVWObjectRepository, &IID_IVWObjectRepository, &LIBID_VWSYSTEMLib>,
	public ISupportErrorInfo,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVWOdb, &CLSID_VWObjectRepository>
{
DECLARE_AGGREGATABLE(CVWOdb)

public:
	CVWOdb();
	~CVWOdb();

BEGIN_COM_MAP(CVWOdb)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWObjectRepository)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

//DECLARE_NOT_AGGREGATABLE(CVWOdb) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

DECLARE_REGISTRY(CVWOdb, _T("VWSYSTEM.ObjectRepository.1"), _T("VWSYSTEM.ObjectRepository"), IDS_VWODB_DESC, THREADFLAGS_BOTH)

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IVWObjectRepository
	STDMETHOD(Initialize)( IWorld *pWorld, BSTR bstrConnection );

	STDMETHOD(OpenWorld)(VARIANT_BOOL bCreate = VARIANT_FALSE, VARIANT_BOOL bOverwrite = VARIANT_FALSE);
	STDMETHOD(CloseWorld)();

	STDMETHOD(CreateObject)(IThing *pThing);
	STDMETHOD(DeleteObject)(int id);

	STDMETHOD(GetObject)(int id, IThing **ppThing);
	STDMETHOD(GetObjectByName)(BSTR bstrName, IThing **pThing);
	STDMETHOD(PutObject)(IThing *pThing);

	STDMETHOD(WriteLogRecord)(int id, IMarshallBuffer *pbuffer);
//	STDMETHOD(WriteWorldLogRecord)(IMarshallBuffer *pbuffer);
	STDMETHOD(Commit)();
	STDMETHOD(CommitAs)(BSTR bstrFileName);
	STDMETHOD(RenameAs)(BSTR bstrFileName);
	STDMETHOD(DumpObjects)();

	STDMETHOD(get_Open)(VARIANT_BOOL* pboolVal)
	{
		ASSERT(pboolVal);
		if (pboolVal == NULL)
			return E_POINTER;

		*pboolVal = m_bOpen;

		return S_OK;
	};

	STDMETHOD(get_Persist)(VARIANT_BOOL* pboolVal)
	{
		ASSERT(pboolVal);
		if (pboolVal == NULL)
			return E_POINTER;

		*pboolVal = m_bPersist;

		return S_OK;
	};

	// helpers
	HRESULT WriteObject(IThing *pThing);

protected:
	// state
	VARIANT_BOOL m_bOpen;

	// Persist flag - if false, then we never write anything out
	// (used by client side)
	VARIANT_BOOL m_bPersist;

	// Temporary world flag - used during client-side creation of avatars
	// Note: if m_bTemporary==TRUE then m_bPersist==FALSE
	VARIANT_BOOL m_bTemporary;

	// connection string
	CString m_strDb;

	//Worldname;
	CString m_strName;

	// pointer back to world
	IWorld * m_pWorld;

	// map of id to thing
	CVWMap<long, long, IThing*, IThing*&>	m_mapIdToThing;

	// log/checkpoint file
	CFile m_fileDb;

	unsigned char m_pCopyBuf[COPYBUFFER_SIZE];
};
