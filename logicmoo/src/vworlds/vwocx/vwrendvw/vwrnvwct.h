// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWRnVwCt.h : Declaration of the CVWRenderViewCtrl OLE control class.

#include "d2d.h"
#include <vwobject.h>
#include <vwclient.h>
#include <vwrender.h>
#include <vwnvtool.h>
#include <viewctl.h>
#include "VWGCache.h"
#include <vwuiview.h>
#include <cmddel.h>
#include <vwmenu.h>
#include <afxcmn.h>
#include <afxext.h>

#include "geomsel.h"
#include "tooldlg.h"
#include "vwsgfxut.h"

#include <vwrnedit.h>

typedef struct _ToolEventInfoStruct
{
	int	 nEventNum;
	LPDISPATCH pObject;
	char szStr1[256];
	char szStr2[256];
	char szStr3[256];
} ToolEventInfoStruct;

/////////////////////////////////////////////////////////////////
// Boundary Object related stuff

typedef struct _BoundaryPoint
{
	float x;
	float y;
	VARIANT_BOOL bVertexSelected;

	POINT screenPnt;
} BoundaryPoint;

//Flag values for nStatus member of boundary object.
#define	BOUNDARYOBJECT_UPDATE_INTERNAL_INFO 1
#define	BOUNDARYOBJECT_UPDATE_SCREEN_INFO 2
#define BOUNDARYOBJECT_DELETE_ME 3

class CBoundaryDisplayObject : public CObject
{
	friend class CVWRenderViewCtrl;
public:
	//Pass it the control manager so it can determine selected status, etc.
	CBoundaryDisplayObject(IThing* pThing, IBoundary* pBoundary, int nBoundaryIndex, IVWControlManager* m_pControlManager, LPDIRECT3DRMVIEWPORT pViewport); 
	~CBoundaryDisplayObject();

	HRESULT DrawBoundary(HDC hDC, IVWControlManager* pControlManager, LPDIRECT3DRMVIEWPORT pViewport, BOOL bNeedUpdateBoundaryScreenCoordinates);
	HRESULT DrawBoundaryEdgeIndex(HDC hDC, int nPosX, int nPosY, int nLastPosX, int nLastPosY, int nIndex);

	HRESULT UpdateInternalInfo(IVWControlManager* pControlManager, LPDIRECT3DRMVIEWPORT pViewport);
	HRESULT UpdateScreenInfo(LPDIRECT3DRMVIEWPORT pViewport);

	void WorldToLocal(float* fX, float* fZ) { };
	void LocalToWorld(float* fX, float* fZ) { };

private:

	//Unref when destroy please
	IBoundary*				m_pBoundary;
	IThing*					m_pThing;
	IVWFrame*				m_pVWFrame;

	//These should be freed when destroyed
	BoundaryPoint*				m_pLocalPoints;

	int					m_nBoundaryIndex;
	int					m_nNumPoints;
	VARIANT_BOOL		m_bPassable;
	VARIANT_BOOL		m_bClosed;
	VARIANT_BOOL		m_bIsSelected;
	VARIANT_BOOL		m_bIsLocked;
	VARIANT_BOOL		m_bNoModify;

	int					m_nStatus;

	static CVWRenderViewCtrl* m_pRenderControl;
};

typedef CTypedPtrArray<CObArray, CBoundaryDisplayObject*> CBoundaryDisplayArray;

typedef struct _D3DAnimation 
{
	void * Animation;
	unsigned long StartTime;  //Time we started the animation
	float fDuration;
	long Options;
	CComBSTR Name;
	CComPtr<IThing> pThis;
	CComBSTR bstrAnimDoneMethod;
	long nFirstKeyTime;
	float fTimeScale;
	float fRangeStart;
	float fRangeEnd;
	float fIndex;

} D3DAnimation;

/////////////////////////////////////////////////////////////////////////////
// CVWRenderViewCtrl : See VWRnVwCt.cpp for implementation.
#define NAVTOOLNAME "Navigate"
#define PICKTOOLNAME "VWPickTool"
#define SELECTTOOLNAME "Select"
#define TRANSLATETOOLNAME "Translate"
#define ROTATETOOLNAME "Rotate"
#define SCALETOOLNAME "Scale"
#define ZOOMINTOOLNAME "ZoomIn3D"
#define ZOOMOUTTOOLNAME "ZoomIn3D"

#define CREATEBOUNDARYTOOLNAME "DrawBoundaryTool"
#define TRANSLATE2DTOOLNAME "Translate2D"
#define ROTATE2DTOOLNAME "Rotate2D"
#define SCALE2DTOOLNAME "Scale2D"

#define WM_CONTROLGETFOCUS (WM_USER + 1)
#define WM_FIRETOOLEVENT (WM_USER + 2)

class CVWRenderViewCtrl : public CVWUIView
{
	DECLARE_DYNCREATE(CVWRenderViewCtrl)

	friend UINT PaintThread(void *pvParam);
	friend class CBoundaryDisplayObject;

// Constructor
public:
	CVWRenderViewCtrl();
	//virtual BOOL HandleOnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	HACCEL m_hAccelerator;

	LPDISPATCH m_VWClient;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVWRenderViewCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual DWORD GetControlFlags();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam );
	virtual BOOL OnGetColorSet(DVTARGETDEVICE* ptd, HDC hicTargetDev, LPLOGPALETTE* ppColorSet);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CVWRenderViewCtrl();

	DECLARE_OLECREATE_EX(CVWRenderViewCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CVWRenderViewCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CVWRenderViewCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CVWRenderViewCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CVWRenderViewCtrl)
	 afx_msg void OnViewFront();
	 afx_msg void OnUpdateViewFront(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnControlGetFocus(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateToolsEditBoundaries(CCmdUI* pCmdUI);
	afx_msg void OnToolsEditBoundaries();
	afx_msg void OnUpdateToolsNavigate(CCmdUI* pCmdUI);
	afx_msg void OnToolsNavigate();
	afx_msg void OnUpdateToolsSelect(CCmdUI* pCmdUI);
	afx_msg void OnToolsSelect();
	afx_msg void OnUpdateToolsTranslate(CCmdUI* pCmdUI);
	afx_msg void OnToolsTranslate();
	afx_msg void OnUpdateToolsRotate(CCmdUI* pCmdUI);
	afx_msg void OnToolsRotate();
	afx_msg void OnUpdateToolsScale(CCmdUI* pCmdUI);
	afx_msg void OnToolsScale();
	afx_msg void OnUpdateToolsUndo(CCmdUI* pCmdUI);
	afx_msg void OnToolsUndo();
	afx_msg void OnUpdateToolsRedo(CCmdUI* pCmdUI);
	afx_msg void OnToolsRedo();
	afx_msg void OnToolsZoomIn();
	afx_msg void OnUpdateToolsZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnToolsZoomOut();
	afx_msg void OnUpdateToolsZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnToolsShowOptions();
	afx_msg void OnUpdateToolsShowOptions(CCmdUI* pCmdUI);
	afx_msg void OnToolsGravity();
	afx_msg void OnUpdateToolsGravity(CCmdUI* pCmdUI);
	afx_msg void OnToolsCollisions();
	afx_msg void OnUpdateToolsCollisions(CCmdUI* pCmdUI);
	afx_msg void OnViewPerspective();
	afx_msg void OnUpdateViewPerspective(CCmdUI* pCmdUI);
	afx_msg void OnViewTop();
	afx_msg void OnUpdateViewTop(CCmdUI* pCmdUI);
	afx_msg void OnViewRight();
	afx_msg void OnUpdateViewRight(CCmdUI* pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);	
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnToolsSelectContents();
	afx_msg void OnUpdateToolsSelectContents(CCmdUI* pCmdUI);
	afx_msg void OnToolsHelp();
	afx_msg void OnToolsZoomNormal();
	afx_msg void OnUpdateToolsCreatePolygonBoundary(CCmdUI* pCmdUI);
	afx_msg void OnToolsCreatePolygonBoundary();
	afx_msg void OnUpdateToolsZoomNormal(CCmdUI* pCmdUI);
	afx_msg void OnToolsDelete();
	afx_msg void OnUpdateToolsDelete(CCmdUI* pCmdUI);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnToolsDumpToScript();
	afx_msg void OnUpdateToolsDumpToScript(CCmdUI* pCmdUI);

	//}}AFX_MSG
	afx_msg void OnCommandRange(UINT nID);
	afx_msg void OnCommandRangeUI(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CVWRenderViewCtrl)
	afx_msg long GetEditMode();
	afx_msg void SetEditMode(long nNewValue);
	afx_msg long GetViewMode();
	afx_msg void SetViewMode( long nMode );
	afx_msg long GetViewFilter();
	afx_msg void SetViewFilter( long nFilter );
	afx_msg BOOL GetShowSelection();
	afx_msg void SetShowSelection(BOOL bNewValue);
	afx_msg LPDISPATCH GetTargetObjectProperty();
	afx_msg void SetTargetObjectProperty(LPDISPATCH newValue);
	afx_msg float GetZoomLevel();
	afx_msg void SetZoomLevel(float newValue);
	afx_msg LPDISPATCH GetControlManager();
	afx_msg void SetControlManager(LPDISPATCH newValue);
	afx_msg SCODE ToolEvent(long nEvent, LPDISPATCH pObject, LPCTSTR bstr1, LPCTSTR bstr2, LPCTSTR bstr3, BOOL bBubble);
	afx_msg SCODE ResetCameraPosition();

	afx_msg SCODE LoadUIToolByProgID(LPCTSTR lpctstrProgID, LPCTSTR lpctstrName);
	afx_msg LPDISPATCH GetUITool(LPCTSTR lpctstrName );
	afx_msg SCODE AddCurrentUITool(LPCTSTR lpctstrName);
	afx_msg BOOL IsUIToolCurrent(LPCTSTR lpctstrName);

	afx_msg SCODE CenterViewOnPosition(short x, short y);
	afx_msg SCODE CenterViewOnObject(LPDISPATCH pObject, BOOL bFollowingObject);
	afx_msg void SetCamera(LPDISPATCH pThing);

	afx_msg LPDISPATCH GetVWRenderRoot();
	afx_msg void SetVWRenderRoot(LPDISPATCH newValue);
	afx_msg void SetNavigationPath(LPDISPATCH pmapPositions, LPDISPATCH pmapDirections);

	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CVWRenderViewCtrl)
	void FireAlphaKeyPressed(long nChar)
		{ FireEvent(eventidAlphaKeyPressed,EVENT_PARAM(VTS_I4), nChar);}
	void FireToolEvent(long nEvent, LPDISPATCH pObject, LPCTSTR bstr1, LPCTSTR bstr2, LPCTSTR bstr3) 	
		{ FireEvent(eventidToolEvent, EVENT_PARAM(VTS_I4 VTS_DISPATCH VTS_BSTR VTS_BSTR VTS_BSTR), nEvent, pObject, bstr1, bstr2, bstr3 );}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()


	//
	// thread synchronization stuff
	//
public:
	HANDLE				m_hGraphicsEnabled;		// sync up threads on init

protected:
	CWinThread			*m_pPaintThread;
	HDC					m_hDC;
	int					m_cxPaintInfo, m_cyPaintInfo, m_xPaintInfo, m_yPaintInfo;
	WPARAM				m_wParamPaintInfo;
	IVWRenderRoot		*m_pVWRenderRoot;
	BOOL				m_bNavigationToolIsCurrentlySelected;
	IVWNavigationTool	*m_pVWNavigationTool;
	IVWPickTool			*m_pVWPickTool;
	IVWGeometryCache	*m_pVWGeometryCache;
	IDirectDraw			*m_pDD;
	IDirect3DRM			*m_pD3DRM;
	IDirect2DRM			*m_pD2DRM;
	IThing				*m_pRenderingContext;
	HWND				m_hwndFocus;
	HWND				m_hwndParent;
	DWORD				m_dwFinishRenderingPause;
	CToolTipCtrl		m_ToolTip;
	CToolTipCtrl		m_CursorToolTip;
	CPoint				m_ToolTipPos;
	BOOL				m_ToolTipShown;
	BOOL				m_CursorToolTipShown;
	BOOL				m_bRendererInitialized;
	BOOL				m_bQuit;
	BOOL				m_bHaltRendering;
	BOOL				m_bRenderingHasHalted;
	BOOL				m_bWaitOneFrame;

	// editing stuff
	long					m_nEditingMode;
	BOOL					m_bShowSelection;
	BOOL					m_bEditingToolsCreated;
	BOOL					m_bIgnoreGravity;
	BOOL					m_bIgnoreCollisions;
	CGeometrySelectionList	m_listSelected;
	CVWMenu					m_Menu;
	WPARAM					m_wpMenuItems;
	BOOL					m_bUsingDX5;
	D3DVECTOR				m_vecLastPerspectiveDirection;
	D3DVECTOR				m_vecLastPerspectivePosition;
	POINT					m_pLastClickedPoint;

	// orthographic camera stuff
	CameraMode	m_CameraMode;
	float		m_flPixelsToWorldUnit;
	float		m_flZoom, m_flPerspectiveZoom, m_flTopZoom, m_flFrontZoom;
	float		m_flCameraX, m_flCameraY, m_flCameraZ;  //Camera position variables (boundary edit mode only!)
	float		m_flCameraDirX, m_flCameraDirY, m_flCameraDirZ; //(boundary edit mode only!)
	float		m_flCameraAngle; //(boundary edit mode only!)

	// Boundary editing properties
	long					m_nViewFilter;
	VARIANT_BOOL			m_bWasUsingFastMode;
	long					m_nWasUsingCameraMode;	
	long					m_nOldEditMode;
	CBoundaryDisplayArray	m_boundaryArray;
	BOOL					m_bNeedUpdateBoundaryScreenCoordinates;

	//Interpolation variables
	CList <D3DAnimation,D3DAnimation> m_animList;
	CMap <CString, const char*, IThing*, IThing*> m_DoneMethodMap;
	HANDLE					m_hInterpMutex;
	float					m_fInterpTime;
	void OnTimer(UINT nProperty);
	//Interpolation helpers
	void ULongSubtract(unsigned long lNum1, unsigned long lNum2, unsigned long * lNum3);
	void ULongAdd(unsigned long lNum1, unsigned long lNum2, unsigned long * lNum3);
	long CurrentTime(IThing* pThing);

	HRESULT RenderRelease();
	HRESULT RenderDoProcessing();
	BOOL OnToolTipNeedText(UINT id, NMHDR * pNMHDR, LRESULT *pResult);
	void SetProjectionAndFOV();

	virtual HRESULT HandleOnUserConnect(IThing* pthing, BOOL bFromClient);
	virtual HRESULT HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient);
	virtual HRESULT HandleOnUserReconnect(IVWClient* newClient, IWorld* pWorld, IThing* pthing);
	virtual HRESULT HandleOnConnect(IDispatch* pWorld);
	virtual HRESULT HandleOnDisconnect(IDispatch* pWorld);

	HRESULT HandleOnEnterRoom(IThing* pWhere, IThing* pWhat);
	HRESULT HandleOnLeaveRoom(IThing* pWhere, IThing* pWhat);
	HRESULT HandleOnGeometryChanged(IThing* pthing);
	
	virtual HRESULT HandleHitTest( VARIANT varHitparams, long nFlags, VARIANT * pvarHitResult );
	virtual HRESULT HandleOptionalSelection( VARIANT varSelection );
	
	virtual HRESULT HandleOnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient);

	void	DoCreateTooltip();
	HRESULT CreateSelectionBox(VARIANT varSel );

	// editing handlers
	HRESULT CreateEditingTools();
	HRESULT InitializeEditingTools();
	virtual HRESULT HandleUIUpdate( enumVWUIEventTypes eTypes, IVWUIView*, VARIANT varHint );
	virtual HRESULT HandleAddSelection( VARIANT varSel );
	virtual HRESULT HandleRemoveSelection( VARIANT varSel );
	virtual HRESULT HandleClearSelections( void );

	// drag/drop handlers
	virtual BOOL HandleDropFile(CPoint pt, LPCTSTR pszFilename);

	// menu stuff
	void UpdateMenus(CMenu *pToolMenu);
	void DoMenu(CPoint pt);

	//Boundary editing methods
	HRESULT CreateBoundaryEditingTools();
	HRESULT DrawBoundaries();
	HRESULT PickVertex(CBoundaryDisplayObject* boundaryDisplayObj, int nX, int nY, int nThresh, long nFlags, EIndex *pIndex);
	HRESULT PickEdge(CBoundaryDisplayObject* boundaryDisplayObj, int nX, int nY, int nThresh, long nFlags, EIndex *pIndex);
	HRESULT DoBoundaryHitTest(CPoint& ptClick, CBoundaryDisplayObject* boundaryDisplayObj, long nFlags, VARIANT * pvarHitResult);
	HRESULT HandleBoundaryHitTest(CPoint& ptClick, long nFlags, VARIANT * pvarHitResult);

	HRESULT RebuildBoundaryArray();
	HRESULT AddThingToBoundaryDisplayArray(IThing* pThing);
	HRESULT UpdateBoundary(IBoundary* pBoundary, unsigned long nHint);

	HRESULT UpdateBoundaryByIndex(IThing* pThing, long nBoundaryIndex);
	HRESULT RemoveBoundaryByIndex(IThing* pThing, long nBoundaryIndex);
	HRESULT AddBoundaryByIndex(IThing* pThing, int nBoundaryIndex);


	// error handler
	void HrError(HRESULT hr);

// Dispatch and event IDs
public:
	BOOL m_bRenderWireframe;
	LPDISPATCH GetVWNavigationTool();
	void SetVWNavigationTool(LPDISPATCH newValue);
	void ConvertFromCameraToThingCoords(D3DVECTOR* pvecPosition);
	void ConvertFromThingToCameraCoords(D3DVECTOR* pvecPosition);
	POINT WorldToDP(float flX, float flY, float flZ);
	HRESULT HandleDeviceToWorld(float *pflX, float *pflY,  float *pflZ);
	float ComputeAngleFromVector(float flX, float flY, float flZ);


	enum {
	//{{AFX_DISP_ID(CVWRenderViewCtrl)
	dispidVWClient = 1L,
	dispidTargetObjectProperty = 2L,
	dispidControlManager = 3L,
	dispidVWRenderRoot = 4L,
	dispidShowSelection = 5L,
	dispidEditMode = 6L,
	dispidZoomLevel = 7L,
	dispidViewMode = 8L,
	dispidViewFilter = 9L,
	dispidLoadUIToolByProgID = 10L,
	dispidAddCurrentUITool = 11L,
	dispidResetCameraPosition = 12L,
	dispidSetNavigationPath = 13L,
	dispidSetCamera = 14L,
	dispidCenterViewOnPosition = 15L,
	dispidGetUITool = 16L,
	dispidIsUIToolCurrent = 17L,
	dispidCenterViewOnObject = 18L,
	dispidToolEvent = 19L,
	eventidAlphaKeyPressed = 1L,
	eventidToolEvent = 2L,
	//}}AFX_DISP_ID
	};
};
