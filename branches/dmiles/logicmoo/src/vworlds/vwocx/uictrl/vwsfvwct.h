// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWSfVwCt.h : Declaration of the CVWSelfVwCtrl OLE control class.

/////////////////////////////////////////////////////////////////////////////
// CVWSelfVwCtrl : See VWSfVwCt.cpp for implementation.

#include "resource.h"

#include <mmimage.h>
#include <d2d.h>
#include <vwobject.h>
#include <vwmenu.h>
#include "..\vwview\viewctl.h"
#include <spritemn.h>
#include <inetfile.h>

typedef CVWComPtr<IPropertyList, NULL,&IID_IPropertyList> CPropertyListPtr;

class CVWSelfVwCtrl : public CVWViewCtrl
{
	DECLARE_DYNCREATE(CVWSelfVwCtrl)

// Constructor
public:
	CVWSelfVwCtrl();

	LPDISPATCH m_VWClient;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVWSelfVwCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CVWSelfVwCtrl();

	virtual HRESULT HandleOnUserConnect(IThing* pthing, BOOL bFromClient);
	virtual HRESULT HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient);
	virtual HRESULT HandleOnUserReconnect(IVWClient* newClient, IWorld* pWorld, IThing* pthing);

	virtual HRESULT HandleOnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient);

	HRESULT HandleOnGeometryChanged(IThing* pthing);
	HRESULT HandleOnWear(IThing* pWho, IThing* pWhat);
	HRESULT HandleOnUnWear(IThing* pWho, IThing* pWhat);
	HRESULT HandleOnDoGesture(int iGesture);

	DECLARE_OLECREATE_EX(CVWSelfVwCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CVWSelfVwCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CVWSelfVwCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CVWSelfVwCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CVWSelfVwCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnLButtonUp( UINT nFlags, CPoint point );
	afx_msg void OnMouseMove( UINT nFlags, CPoint point );
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnCommandRange(UINT nID);
	afx_msg void OnCommandRangeUI(CCmdUI* pCmdUI);
	afx_msg void OnTimer( UINT nIDEvent );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CVWSelfVwCtrl)
	afx_msg void SetGeometryName(LPCTSTR lpszNewValue);
	afx_msg BSTR GetGeometryName();
	afx_msg void SetCurrentGesture(int iGesture);
	afx_msg int GetCurrentGesture();
	afx_msg void AddAccessory(BSTR bstrName, long lID, float fPosX, float fPosY);
	afx_msg void RemoveAccessory(long lID);
	afx_msg void SetBackgroundColor(short r, short g, short b);
	afx_msg int GetNamedGesture(LPCSTR szGesture);
	afx_msg void SetHandleWorldEvents(BOOL bHandleWorldEvents);
	afx_msg BOOL GetHandleWorldEvents();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CVWSelfVwCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CVWSelfVwCtrl)
	dispidVWClient = 1L,
	dispidGeometryName = 2L,
	dispidCurrentGesture = 3L,
	dispidHandleWorldEvents = 4L,
	dispidAddAccessory = 5L,
	dispidRemoveAccessory = 6L,
	dispidSetBackgroundColor = 7L,
	dispidGetNamedGesture = 8L
	//}}AFX_DISP_ID
	};

private:
	//
	// D2D helper functions
	//
	HRESULT					ReleaseAll();
	HRESULT					UpdateSelfView();
	HRESULT					SetSelfViewBackground(BSTR bstrNewImageFilename);
	void					DoPaint(const RECT &rInvalid);
	HRESULT					GeometryNameChanged();
	HRESULT					AccessoryFrameFromID(long lID, LPDIRECT2DRMFRAME *ppFrame);

private:
	float					m_fXWinScale, m_fYWinScale;
	LPDIRECTDRAWSURFACE		m_pBackBuffer;
	LPDIRECT2DRMIMAGE		m_pd2diBackground;
	LPDIRECT2DRMIMAGE		m_pd2diSelfView;
	DWORD					m_nSVWidth, m_nSVHeight;
	ISpriteManager			*m_pSpriteManager;
	CComBSTR				m_bstrGeometryName;
	DWORD					m_dwTimerID;
	CComBSTR				m_bstrSelfViewName;
	COLORREF				m_crBackgroundColor;
	BOOL					m_bHandleWorldEvents;
	CComBSTR				m_bstrDefaultSpriteFile;

	//
	// D2D variables
	//
	ErrMgr					m_em;

	// DirectDraw
	LPDIRECTDRAW			m_pDD;
	LPDIRECTDRAWSURFACE		m_pFrontBuffer;
	LPDIRECTDRAWPALETTE		m_pPalette;
	DDPIXELFORMAT			m_ddpfTarget;
	LPDIRECTDRAWCLIPPER		m_pClipper;

	LPDIRECT2DRM			m_pD2DRM;
	LPDIRECT2DRMDEVICE		m_pDevice;
	LPDIRECT2DRMVIEWPORT	m_pViewport;
	LPDIRECT2DRMFRAME		m_pCamera;
	LPDIRECT2DRMFRAME		m_pAccessoryScene;
	VARIANT_BOOL			m_bShowAccessories;
	VARIANT_BOOL			m_bTrackingMouse;
	LPDIRECT2DRMPICK		m_pPick;
	CPoint					m_ptAnchor;
	CVWMenu					m_Menu;

	IInternetFileManager	*m_pINetFileManager;

	// hardware info
	int						m_nZDepth;
};
