// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWADataO.h : Declaration of the CVWAccessoryDataObject

#include <resource.h>
#include <PropBase.h>
#include <objimpl.h>

EXTERN_C const CLSID CLSID_VWAccessoryData;
EXTERN_C const IID LIBID_VWMMLib;

/////////////////////////////////////////////////////////////////////////////
// VWAccessoryData

DECLARE_OBJECTPROPERTY(VWAccessoryData, LIBID_VWMMLib, IDS_VWACCESSORYDATAOBJECT_TYPE)
{
	DECLARE_AGGREGATABLE(CVWAccessoryDataObject)

public:
	CVWAccessoryDataObject();
	~CVWAccessoryDataObject();

BEGIN_COM_MAP(CVWAccessoryDataObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectProperty)
	COM_INTERFACE_ENTRY(IVWAccessoryData)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

//DECLARE_NOT_AGGREGATABLE(CVWAccessoryDataObject) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

	DECLARE_REGISTRY(CVWAccessoryDataObject, _T("VWSYSTEM.AccessoryData.1"), _T("VWSYSTEM.AccessoryData"), IDS_VWACCESSORYDATAOBJECT_DESC, THREADFLAGS_BOTH)

	// Override the base class
	STDMETHOD(get_RefCount)(DWORD *pdwRef);
	STDMETHOD(Marshall)(DWORD dwOptions, IMarshallBuffer* pbuffer); 
	STDMETHOD(UnMarshall)(IUnMarshallBuffer* pbuffer); 

// IVWAccessoryData

	STDMETHOD(get_AccessoryGraphic)(BSTR *pbstrAccessoryGraphic);
	STDMETHOD(put_AccessoryGraphic)(BSTR bstrAccessoryGraphic);
	STDMETHOD(GetPos)(float *pfPosX, float *pfPosY);
	STDMETHOD(PutPos)(float fPosX, float fPosY);

private:
// Local helper data and funcs.

	CComBSTR	m_bstrAccessoryGraphic;
	float		m_fPosX;
	float		m_fPosY;
};

	
	
	
