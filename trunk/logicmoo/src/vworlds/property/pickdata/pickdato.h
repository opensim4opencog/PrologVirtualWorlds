// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWIDataO.h : Declaration of the CVWPickDataObject

#include <resource.h>
#include <PropBase.h>
#include <objimpl.h>
#include <afxtempl.h>

EXTERN_C const IID LIBID_VWMMLib;
EXTERN_C const CLSID CLSID_VWPickData;

typedef CArray<int,int> INTARRAY;

/////////////////////////////////////////////////////////////////////////////
// VWPickData

DECLARE_OBJECTPROPERTY(VWPickData, LIBID_VWMMLib, IDS_VWPICKDATAOBJECT_TYPE)
{
DECLARE_AGGREGATABLE(CVWPickDataObject)

public:
	CVWPickDataObject();
	~CVWPickDataObject();

BEGIN_COM_MAP(CVWPickDataObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectProperty)
	COM_INTERFACE_ENTRY(IVWPickData)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

//DECLARE_NOT_AGGREGATABLE(CVWPickDataObject) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

	DECLARE_REGISTRY(CVWPickDataObject, _T("VWSYSTEM.PickData.1"), _T("VWSYSTEM.PickData"), IDS_VWPICKDATAOBJECT_DESC, THREADFLAGS_BOTH)

	// Override the base class
	STDMETHOD(Marshall)(DWORD dwOptions, IMarshallBuffer* pbuffer); 
	STDMETHOD(UnMarshall)(IUnMarshallBuffer* pbuffer); 
	STDMETHOD(get_RefCount)(DWORD *pdwRef);
	STDMETHOD(Terminate)(void);

// IVWPickData

	STDMETHOD(get_BoundaryList)(IPropertyList **ppBoundaryList);
	STDMETHOD(put_BoundaryList)(IPropertyList *pBoundaryList);

	STDMETHOD(get_Thing)(IThing **ppThing);
	STDMETHOD(put_Thing)(IThing *pThing);
	STDMETHOD(get_Boundary)(IBoundary **ppBoundary);
	STDMETHOD(put_Boundary)(IBoundary *pBoundary);

	STDMETHOD(get_BoundaryIndex)(int *piBoundaryIndex);
	STDMETHOD(put_BoundaryIndex)(int iBoundaryIndex);
	STDMETHOD(get_EdgeIndex)(int *piEdgeIndex);
	STDMETHOD(put_EdgeIndex)(int iEdgeIndex);

	STDMETHOD(get_DataType)(int *piDataType);
	STDMETHOD(put_DataType)(int iDataType);

	STDMETHOD(get_VertexPicked)(int nIndex, VARIANT_BOOL* vbPicked);
	STDMETHOD(put_VertexPicked)(int nIndex, VARIANT_BOOL vbPicked);
	STDMETHOD(IsAnyVertexPicked)(VARIANT_BOOL* pvbPicked);
	STDMETHOD(get_Name)(BSTR* bstrName);


private:
// Local helper data and funcs.

	IPropertyList	*m_pBoundaryList;
	IThing			*m_pThing;
	IBoundary		*m_pBoundary;
	int				m_iBoundaryIndex;
	int				m_iEdgeIndex;
	int				m_iDataType;
	INTARRAY		m_pickedVertexArray;
};

	
	
	
