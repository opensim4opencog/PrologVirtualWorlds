// VWGeomOb.h : Declaration of the CVWGeometryObject

#include <resource.h>
#include <propbase.h>
#include <objimpl.h>

#include <VWGCache.h>

EXTERN_C const IID LIBID_VWMMLib;
EXTERN_C const CLSID CLSID_VWGeometry;

/////////////////////////////////////////////////////////////////////////////
// VWGeom

DECLARE_OBJECTPROPERTY(VWGeometry, LIBID_VWMMLib, IDS_VWGEOMETRY_TYPE)
{
DECLARE_AGGREGATABLE(CVWGeometryObject)

public:
	CVWGeometryObject();
	~CVWGeometryObject();

	STDMETHOD(Marshall)(DWORD dwOptions, IMarshallBuffer* pbuffer);
	STDMETHOD(UnMarshall)(IUnMarshallBuffer* pbuffer);
	STDMETHOD(MarshallToScript)(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut);
	STDMETHOD(Terminate)(void);
	STDMETHOD(get_RefCount)(DWORD *pdwRef);

BEGIN_COM_MAP(CVWGeometryObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectProperty)
	COM_INTERFACE_ENTRY(IVWGeometry)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

//DECLARE_NOT_AGGREGATABLE(CVWGeometryObject) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

	DECLARE_REGISTRY(CVWGeometryObject, _T("VWSYSTEM.Geometry.1"), _T("VWSYSTEM.Geometry"), IDS_VWGEOMETRYOBJECT_DESC, THREADFLAGS_BOTH)

// IVWGeometry
	STDMETHOD(ComposeGeometry)(IVWFrame **ppvwf);
	STDMETHOD(ReleaseGeometry)();
	STDMETHOD(put_GeometryName)(BSTR bstr);
	STDMETHOD(get_GeometryName)(BSTR* pbstr);
	STDMETHOD(get_Frame)(IVWFrame **ppVWFrame);
	STDMETHOD(put_Frame)(IVWFrame *pVWFrame);
	STDMETHOD(get_GeometryFrame)(IVWFrame **ppVWFrame);
 	STDMETHOD(put_Occluded)(VARIANT_BOOL bOccluded);
	STDMETHOD(get_NamedGesture)(BSTR bstrGesture, int *pGestureNum);
	STDMETHOD(get_GestureText)(BSTR bstrGesture, BSTR *pbstrText);
	STDMETHOD(GetNamedGestures)(IPropertyList **ppGestureNameList);
	STDMETHOD(put_CurrentGesture)(int iGesture);
	STDMETHOD(get_CurrentGesture)(int* piGesture);


private:
	CComBSTR		m_bstrGeometryName;
	IVWFrame		*m_pvwfTransform;
	IVWFrame		*m_pvwfCachedGeometry;
	VARIANT_BOOL	m_bOccluded;

	static CComBSTR m_bstrVWFrame;

	IVWGeometryCache	*m_pVWGeometryCache;
};
