// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_VWSOUNDCTL_H__05769B9A_A180_11D1_83E9_00C04FB6FA46__INCLUDED_)
#define AFX_VWSOUNDCTL_H__05769B9A_A180_11D1_83E9_00C04FB6FA46__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include <vwsysex.h>
#include <afxcmn.h>
#include "..\vwview\viewctl.h"


typedef CVWComPtr<IThing, NULL, &IID_IThing> CThingPtr;
typedef CVWComPtr<IPropertyList, NULL, &IID_IPropertyList> CListPtr;
typedef CVWComPtr<IDirectSoundBuffer, NULL, &IID_IDirectSoundBuffer> CSoundBufferPtr;
typedef CVWComPtr<IVector, NULL, &IID_IVector> CVectorPtr;
typedef CVWComPtr<IDirectSound3DBuffer, NULL, &IID_IDirectSound3DBuffer> CSound3DBufferPtr;
typedef CVWComPtr<IVWRenderRoot,NULL,&IID_IVWRenderRoot> CVWRenderRootPtr;
typedef CVWComPtr<IVWFrame,NULL,&IID_IVWFrame> CVWFramePtr;
typedef CVWComPtr<IVWGeometry,NULL,&IID_IVWGeometry> CVWGeometryPtr;
typedef CVWComPtr<ISound, NULL, &IID_ISound> CVWSoundPtr;


// VwsoundCtl.h : Declaration of the CVwsoundCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CVwsoundCtrl : See VwsoundCtl.cpp for implementation.

typedef CMap <long, long, CSoundBufferPtr, CSoundBufferPtr&> CSoundMap;
#define NBUFFERMAPS 3
#define STANDARDMAP 0
#define CLIENTONLYMAP 1
#define UIEVENTMAP 2

class CVwsoundCtrl : public CVWViewCtrl
{
	DECLARE_DYNCREATE(CVwsoundCtrl)

// Constructor
public:
	CVwsoundCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVwsoundCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual DWORD GetControlFlags();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CVwsoundCtrl();

	DECLARE_OLECREATE_EX(CVwsoundCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CVwsoundCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CVwsoundCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CVwsoundCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CVwsoundCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CVwsoundCtrl)
	afx_msg LPDISPATCH GetVWClient();
	afx_msg void SetVWClient(LPDISPATCH newValue);
	afx_msg LPDISPATCH GetRenderRoot();
	afx_msg void SetRenderRoot(LPDISPATCH newValue);
	afx_msg BOOL GetEnable();
	afx_msg void SetEnable(BOOL bNewValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	DECLARE_EVENTSINK_MAP()

	virtual HRESULT HandleOnPropertyChanged(IThing* pthing, BSTR bstr, long lHint, VARIANT varHintData);

	virtual HRESULT HandleOnUserConnect(IThing* pthing, BOOL bFromClient);
	virtual HRESULT HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient);

	virtual HRESULT HandleOnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient);

	HRESULT HandleOnEnterRoom(IThing* pWhere, IThing* pWhat);
	HRESULT HandleOnLeaveRoom(IThing* pWhere, IThing* pWhat);

	void ClearAll();
	void RemoveThing(IThing * pThing);
	void RefreshList();
	void CheckThingForSound(IThing *pThing);
	void RenderSound(IThing *pThing,int iMap, VARIANT var, long hint, const VARIANT *varHintData);
	HRESULT Create3D(BSTR bstrFile, IDirectSound3DBuffer **pp3DBuffer );
	HRESULT Play(IDirectSoundBuffer *pBuffer ,VARIANT_BOOL bLoop);
	HRESULT SetPosition(float fPosX, float fPosY, float fPosZ);
	HRESULT SetOrientation(float fDirX, float fDirY, float fDirZ, float fUpX, float fUpY, float fUpZ);
	HRESULT SetDistanceFactor( float flFactor );
	HRESULT SetRolloffFactor( float flFactor );
	HRESULT CleanUp();
	HRESULT LoadSoundBuffer( CString& strFile, BOOL b3D, IDirectSoundBuffer **ppbuf );
	HRESULT ReloadSoundBuffer( CString& strFile, IDirectSoundBuffer *pbuf );
	HRESULT FillBuffer( IDirectSoundBuffer *pbuf, BYTE *pbWaveData, DWORD cbWaveSize );
	HRESULT CreateMapping( CString& strFile, void **ppvMapping );
	HRESULT ParseWAV(	void *			pvMapping,
						WAVEFORMATEX **	ppWaveFormat,
						BYTE **			ppbWaveData,
						DWORD *			pcbWaveSize );
	HRESULT CloseMapping( void *pvMapping );

	HRESULT GetListenerPosition(float *rgfl);
	HRESULT GetRoomFrame( IVWFrame** ppRoomFrame );



// Event maps
	//{{AFX_EVENT(CVwsoundCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()
	DECLARE_INTERFACE_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CVwsoundCtrl)
	dispidRenderRoot = 2L,
	dispidEnable = 3L,
	//}}AFX_DISP_ID
	};

private:
	VARIANT_BOOL m_bEnable;
	LPDIRECTSOUND				m_pDSound;
	IDirectSound3DListener *	m_pDSListener;
	IDispatch	* m_pRenderViewOCX;
	CVWRenderRootPtr m_RenderRootPtr;
	//_DAACtlMethods				m_IMA;
	// 2 sound buffers, standard and clientonly
	CSoundMap m_bufferMap[NBUFFERMAPS];

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VWSOUNDCTL_H__05769B9A_A180_11D1_83E9_00C04FB6FA46__INCLUDED)
