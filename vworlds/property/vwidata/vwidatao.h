// VWIDataO.h : Declaration of the CVWIntersectDataObject

#include <resource.h>
#include <PropBase.h>
#include <objimpl.h>

EXTERN_C const IID LIBID_VWMMLib;
EXTERN_C const CLSID CLSID_VWIntersectData;

/////////////////////////////////////////////////////////////////////////////
// VWIntersectData

DECLARE_OBJECTPROPERTY(VWIntersectData, LIBID_VWMMLib, IDS_VWINTERSECTDATAOBJECT_TYPE)
{
DECLARE_AGGREGATABLE(CVWIntersectDataObject)

public:
	CVWIntersectDataObject();
	~CVWIntersectDataObject();

BEGIN_COM_MAP(CVWIntersectDataObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectProperty)
	COM_INTERFACE_ENTRY(IVWIntersectData)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

//DECLARE_NOT_AGGREGATABLE(CVWIntersectDataObject) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

	DECLARE_REGISTRY(CVWIntersectDataObject, _T("VWSYSTEM.IntersectData.1"), _T("VWSYSTEM.IntersectData"), IDS_VWINTERSECTDATAOBJECT_DESC, THREADFLAGS_BOTH)

	// Override the base class
	STDMETHOD(Marshall)(DWORD dwOptions, IMarshallBuffer* pbuffer); 
	STDMETHOD(UnMarshall)(IUnMarshallBuffer* pbuffer); 
	STDMETHOD(get_RefCount)(DWORD *pdwRef);
	STDMETHOD(Terminate)(void);

// IVWIntersectData

	STDMETHOD(get_BoundaryList)(IPropertyList **ppCell);
	STDMETHOD(put_BoundaryList)(IPropertyList *pCell);
	STDMETHOD(get_BoundaryIndex)(int *piBoundaryIndex);
	STDMETHOD(put_BoundaryIndex)(int iBoundaryIndex);
	STDMETHOD(get_EdgeIndex)(int *piEdgeIndex);
	STDMETHOD(put_EdgeIndex)(int iEdgeIndex);
	STDMETHOD(get_DistToIntersect)(float *pfDist);
	STDMETHOD(put_DistToIntersect)(float fDist);
	STDMETHOD(get_Passed)(BYTE *pbPassed);
	STDMETHOD(put_Passed)(BYTE bPassed);
	STDMETHOD(get_Transition)(BYTE *pbTransition);
	STDMETHOD(put_Transition)(BYTE bTransition);
	STDMETHOD(get_Projection)(IVector **ppvProjection);
	STDMETHOD(put_Projection)(IVector *pvProjection);
	STDMETHOD(Get)(IPropertyList **ppCell, int *piBoundaryIndex, int *piEdgeIndex, 
				   float *pfDist, BYTE *pbPassed, BYTE *pbTransition, IVector **ppvProjection);

private:
// Local helper data and funcs.

	IPropertyList	*m_pBoundaryList;
	int				m_iBoundaryIndex;
	int				m_iEdgeIndex;
	float			m_fDistToIntersect;
	BYTE			m_bPassed;
	BYTE			m_bTransition;
	IVector			*m_pvProjection;
};

	
	
	
