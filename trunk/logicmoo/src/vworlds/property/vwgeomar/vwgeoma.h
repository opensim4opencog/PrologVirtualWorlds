// VWGeomA.h : Declaration of the CVWGeometryArticulated

#ifndef __VWGEOMETRYARTICULATED_H_
#define __VWGEOMETRYARTICULATED_H_

#include <resource.h>       // main symbols
#include <propbase.h>
#include <objimpl.h>
#include "jointobj.h"
#include <VWGeomAr.h>

EXTERN_C const IID LIBID_VWMMLib;
EXTERN_C const CLSID CLSID_VWGeometryArticulated;

EXTERN_C const CLSID CLSID_VWFrame;

/////////////////////////////////////////////////////////////////////////////
// CVWGeometryArticulated

//DECLARE_OBJECTPROPERTYEX(CVWGeometryArticulated, IVWGeometryArticulated, IID_IVWGeometryArticulated, CLSID_VWGeometryArticulated, LIBID_VWMMLib, IDS_VWGEOMETRYSP_TYPE)
DECLARE_OBJECTPROPERTYEX(CVWGeometryArticulated, IVWGeometryArticulated, IID_IVWGeometryArticulated, CLSID_VWGeometryArticulated, LIBID_VWMMLib, IDS_VWGEOMETRYARTICULATED_TYPE)
{
	DECLARE_AGGREGATABLE(CVWGeometryArticulated)

	STDMETHOD(Marshall)(DWORD dwOptions, IMarshallBuffer* pbuffer);
	STDMETHOD(UnMarshall)(IUnMarshallBuffer* pbuffer);
	STDMETHOD(MarshallToScript)(DWORD dwOptions, BSTR bstrContext, VARIANT_BOOL *pbAsVal, BSTR *pbstrOut);
	STDMETHOD(Terminate)(void);
public:
	CVWGeometryArticulated();
	~CVWGeometryArticulated();

DECLARE_REGISTRY(CVWGeometryArticulated, _T("VWSYSTEM.GeometryArticulated.1"), _T("VWSYSTEM.GeometryArticulated"), IDS_VWGEOMETRYARTICULATED_DESC, THREADFLAGS_BOTH)

BEGIN_COM_MAP(CVWGeometryArticulated)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectProperty)
	COM_INTERFACE_ENTRY(IVWGeometry)
	COM_INTERFACE_ENTRY(IVWGeometryArticulated)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

//Articulated-body-specific methods ...

	STDMETHOD(get_Character)(ICharacter **ch);
	STDMETHOD(put_Character)(ICharacter *ch);
	STDMETHOD(Load)(BSTR xfile);

//A random prototype that we need to throw in here...
	STDMETHOD(get_RefCount)(DWORD *pdwRef);

// IVWGeometry
	STDMETHOD(ComposeGeometry)(IVWFrame **ppvwf);
	STDMETHOD(ReleaseGeometry)();
	STDMETHOD(put_GeometryName)(BSTR bstr);
	STDMETHOD(get_GeometryName)(BSTR* pbstr);
	STDMETHOD(put_MotionFile)(BSTR bstr);
	STDMETHOD(get_MotionFile)(BSTR* pbstr);
	STDMETHOD(get_Frame)(IVWFrame **ppVWFrame);
	STDMETHOD(put_Frame)(IVWFrame *pVWFrame);
	STDMETHOD(get_GeometryFrame)(IVWFrame **ppVWFrame);
 	STDMETHOD(put_Occluded)(VARIANT_BOOL bOccluded);
	STDMETHOD(get_NamedGesture)(BSTR bstrGesture, int *pGestureNum);
	STDMETHOD(get_GestureText)(BSTR bstrGesture, BSTR *pbstrText);

	STDMETHOD(CreateBehavior)(BSTR progID, IBehavior **behavior);

	STDMETHOD(GetNamedGestures)(IPropertyList **ppGestureNameList);
	STDMETHOD(put_CurrentGesture)(int iGesture);
	STDMETHOD(get_CurrentGesture)(int* piGesture);

//	STDMETHOD(get_ThumbnailName)(BSTR* pbstrThumbnailName);

	static int CVWGeometryArticulated::UpdateCallBack(LPDIRECT3DRMUSERVISUAL obj, LPVOID arg,
                                   D3DRMUSERVISUALREASON reason,
                                   LPDIRECT3DRMDEVICE dev,
                                   LPDIRECT3DRMVIEWPORT view);

private:
	//Internal methods
	bool InitJoint(int id, int dof[], int numDOF, int parent, CComBSTR filename, double poX, 
																	double poY, double poZ);
	STDMETHOD(ParseJoint)(char *firstline, CStdioFile *xFile, int parent);

	CComBSTR						m_bstrGeometryName;
	CComBSTR						motionFile;
	IVWFrame						*top_frame;
	IVWFrame						*geom_frame;
	VARIANT_BOOL					m_bOccluded;
	ICharacter						*character;
	void							*appData;

	static CComBSTR					m_bstrVWFrame;
	IVWGeometryCache				*m_pVWGeometryCache;

	int								curID;

};

#endif //__VWGEOMETRYARTICULATED_H_
