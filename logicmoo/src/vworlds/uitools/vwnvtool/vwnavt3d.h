// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWNavT3D.h : Declaration of the CVWNavigationTool3D

#include <resource.h>

#include <vecmath.h>

#include <propbase.h>
#include <uitlimpl.h>
#include <vwanim.h>

EXTERN_C const IID LIBID_VWMMLib;
EXTERN_C const CLSID CLSID_VWNavigationTool3D;

// Minimum time between firing intersection events on the same boundary.
#define dwTimeThreshold 1000

/////////////////////////////////////////////////////////////////////////////
// VWNvTool

typedef IVWUIToolImpl<IVWNavigationTool, &IID_IVWNavigationTool, &LIBID_VWMMLib> CVWNavigationTool3DBase;
class ATL_NO_VTABLE CVWNavigationTool3D :
	public CVWNavigationTool3DBase,
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVWNavigationTool3D, &CLSID_VWNavigationTool3D>,
	public IObjectSafetyImpl<CVWNavigationTool3D, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
	public ISupportErrorInfoImpl<&IID_IVWNavigationTool>
{
public:
	CVWNavigationTool3D();
	~CVWNavigationTool3D();

BEGIN_COM_MAP(CVWNavigationTool3D)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWUITool)
	COM_INTERFACE_ENTRY(IVWNavigationTool)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

//DECLARE_NOT_AGGREGATABLE(CVWNavigationTool3D) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation.  The default is to support it

	DECLARE_REGISTRY(CVWNavigationTool3D, _T("VWSYSTEM.NavigationTool3D.1"), _T("VWSYSTEM.NavigationTool3D"), IDS_VWNAVIGATIONTOOL3D_DESC, THREADFLAGS_BOTH)

// IVWUITool
	virtual BOOL OnLButtonDown( UINT nFlags, POINT point );
	virtual BOOL OnLButtonUp( UINT nFlags, POINT point );
	virtual BOOL OnMouseMove( UINT nFlags, POINT point );
	virtual BOOL OnKeyUp( UINT nVirtKey, LPARAM lKeyData );
	virtual BOOL OnMessage( UINT nIDEvent, WPARAM wParam, LPARAM lParam );
	STDMETHOD (IsValid)(IVWUIView *pVw, VARIANT_BOOL* pbool);

// IVWNavigationTool
	STDMETHOD (Initialize)(IVWUIView* pVw, IWorld* pWorld, IVWRenderRoot *pVWRenderRoot);
	STDMETHOD (Update)();
	STDMETHOD (put_NavigationTarget)(IThing *pThing);
	STDMETHOD (SetNavigationPath)(IPropertyMap *pmapPositions, IPropertyMap *pmapDirections, BSTR bstrDoneEvent);
	STDMETHOD (get_Moving)(VARIANT_BOOL *bMoving);
	STDMETHOD (put_OrthogonalMode)(VARIANT_BOOL bOrthogonal);
	STDMETHOD (put_TopOrthogonalMode)(VARIANT_BOOL bOrthogonal);
	STDMETHOD (put_FrontOrthogonalMode)(VARIANT_BOOL bOrthogonal);
	STDMETHOD (put_RightOrthogonalMode)(VARIANT_BOOL bOrthogonal);
	STDMETHOD (put_KeyboardNavEnabled)(VARIANT_BOOL bEnable);
	STDMETHOD (get_KeyboardNavEnabled)(VARIANT_BOOL* pbEnable);
	STDMETHOD (put_AllNavigationDisabled)(VARIANT_BOOL bDisable);
	STDMETHOD (get_AllNavigationDisabled)(VARIANT_BOOL* pbDisable);
	STDMETHOD (get_KeyBinding)(KeyConstant index, VARIANT* pvar); 
	STDMETHOD (put_KeyBinding)(KeyConstant index, VARIANT var);
	STDMETHOD (get_CollisionDetection)(VARIANT_BOOL *pVal); 
	STDMETHOD (put_CollisionDetection)(VARIANT_BOOL newVal);
	STDMETHOD (get_Gravity)(VARIANT_BOOL *pVal); 
	STDMETHOD (put_Gravity)(VARIANT_BOOL newVal);
	STDMETHOD (get_CameraOnlyNavigationMode)(VARIANT_BOOL* pbEnable);
	STDMETHOD (put_CameraOnlyNavigationMode)(VARIANT_BOOL bEnable);
	
private:
	// Helper funcs.
	STDMETHOD (CalcVelocityAndPosition)(float fTime, Vector3 *pvLastPos, 
										Vector3 *pvPos, Vector3 *pvDir, 
										VARIANT_BOOL *pbTranslating, VARIANT_BOOL *pbRotating);
	STDMETHOD (CalculateLocationOnPath)(Vector3 *pvPos, Vector3 *pvDir, VARIANT_BOOL *pbTranslating, VARIANT_BOOL *pbRotating);
	STDMETHOD (HandleIntersections)(IPropertyList *pIDataList, Vector3 &vLastPos, Vector3 &vPos);
	STDMETHOD (NavigateTarget)(float fTime);
	STDMETHOD (NavigateCamera)(float fTime);
	STDMETHOD (FireIntersectEvent)(float fP1X, float fP1Y, float fP1Z, float fP2X, float fP2Y, float fP2Z, IPropertyList *pIDataList);
	STDMETHOD (Drop)(float fCurrentPosX, float fCurrentPosY, float fCurrentPosZ);

	// Helper data.
	CPoint				m_ptAnchor;				// anchor point for mouse navigation
	CPoint				m_ptActualMousePos;		// actual mouse position (if we didn't muck with it)
	CSize				m_vnMouseDisp;			// integer vector representing total mouse displacement
	CSize				m_vnStoredMouseDisp;	// int vec of "stored" disp from resetting cursor pos
	VARIANT_BOOL		m_bTrackingMouse;
	Vector2				m_v2CurVel;				// standard velocity
	Vector2				m_v2CurCVel;			// "ctrl" velocity
	VARIANT_BOOL		m_bMoving;
	IPropertyMap		*m_pmapPositions;
	IPropertyMap		*m_pmapDirections;
	VARIANT_BOOL		m_bNavigationPath;
	IVWFrame			*m_pvwfTransform;
	IVWFrame			*m_pvwfParent;
	IThing				*m_pThing;
	IVWAnimation		*m_pHBAnimation;
	IVWRenderRoot		*m_pVWRenderRoot;
	VARIANT_BOOL		m_bCameraMode;
	CRITICAL_SECTION	m_CS;
	UINT 				m_nViewMode;
	VARIANT_BOOL		m_bFalling;
	VARIANT_BOOL		m_bGravityOn;
	VARIANT_BOOL		m_bCollisionDetection;
	VARIANT_BOOL		m_bGotFocus;
	VARIANT_BOOL		m_bKeyboardNavEnabled;
	VARIANT_BOOL		m_bAllNavigationDisabled;
	UINT				m_KeyBindings[kbMax];
//	IDispatch* m_piBoundaryViewControl;			//Boundary control pointer, for use between LButtonDown/Up.
	CComBSTR			m_bstrDoneEvent;
	LPDISPATCH			m_piRenderViewControl;
	float				m_fZoomLevel;

	//utils
	Vector2 KeyboardNavVector();
	struct LASTHITEDGE
	{
		LASTHITEDGE::LASTHITEDGE() {pBoundaryList = NULL;};

		IObjectProperty	*pBoundaryList;
		int				iBoundaryIndex;
		int				iEdgeIndex;
		DWORD			dwTimeHit;

		BOOL IsNewHit(IObjectProperty* p1, int i1, int i2, DWORD dwTime)
		{
			return (pBoundaryList != p1 || iBoundaryIndex != i1 || iEdgeIndex != i2) ||
				   dwTime-dwTimeHit > dwTimeThreshold;
		};

		void Set(IPropertyList *p1, int i1, int i2, DWORD dwTime) 
		{
			pBoundaryList = p1;
			iBoundaryIndex = i1;
			iEdgeIndex = i2;
			dwTimeHit = dwTime;
		};
	};
	LASTHITEDGE			m_LastHitEdge1;
	LASTHITEDGE			m_LastHitEdge2;
};
