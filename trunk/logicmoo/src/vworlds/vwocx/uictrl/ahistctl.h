// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#if !defined(AFX_HISTCTL_H__F5029F79_AF74_11D0_A78C_00A0C90AAA10__INCLUDED_)
#define AFX_HISTCTL_H__F5029F79_AF74_11D0_A78C_00A0C90AAA10__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <vwsysex.h>
#include <afxcmn.h>
#include "..\vwview\viewctl.h"
#include "cutils.h"
#include <afxtempl.h>

#include <comdef.h>
#include <afxrich.h>

#define cxIndentation	200
#define cxLeftMargin	35

// HistCtl.h : Declaration of the CHistoryCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CHistoryCtrl : See HistCtl.cpp for implementation.

#define TCTRACE 0x00FF
#define TCERROR 0x0100


class CHistoryCtrl : public CVWViewCtrl
{
	DECLARE_DYNCREATE(CHistoryCtrl)

// Constructor
public:
	CRichEditCtrl m_Edit;
	LPBITMAPINFOHEADER m_pDIB;

#define _USE_RICHEDIT_
	CHistoryCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistoryCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void PerformAutosave(void);
	~CHistoryCtrl();

	DECLARE_OLECREATE_EX(CHistoryCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CHistoryCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CHistoryCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CHistoryCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CHistoryCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSaveHistory();
	afx_msg void OnUpdateSaveHistory(CCmdUI* pCmdUI);
	afx_msg void OnClearHistory();
	afx_msg void OnUpdateClearHistory(CCmdUI* pCmdUI);
	afx_msg void OnRefreshHistory();
	afx_msg void OnUpdateRefreshHistory(CCmdUI* pCmdUI);
	afx_msg void OnFilterGestures();
	afx_msg void OnUpdateFilterGestures(CCmdUI* pCmdUI);
	afx_msg void OnFilterEvents();
	afx_msg void OnUpdateFilterEvents(CCmdUI* pCmdUI);
	afx_msg void OnFilterGeneral();
	afx_msg void OnUpdateFilterGeneral(CCmdUI* pCmdUI);
	afx_msg void OnFilterTrace();
	afx_msg void OnUpdateFilterTrace(CCmdUI* pCmdUI);
	afx_msg void OnFilterReport();
	afx_msg void OnUpdateFilterReport(CCmdUI* pCmdUI);
	afx_msg void OnCopy();
	afx_msg void OnUpdateCopy(CCmdUI* pCmdUI);
	afx_msg void OnFontChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CHistoryCtrl)
	BOOL m_bAppendOnSave;
	afx_msg void OnAppendOnSaveChanged();
	BOOL m_bAutoSave;
	afx_msg void OnAutoSaveChanged();
	afx_msg LPDISPATCH GetVWClient();
	afx_msg void SetVWClient(LPDISPATCH newValue);
	afx_msg BOOL GetFilterEvents();
	afx_msg void SetFilterEvents(BOOL bNewValue);
	afx_msg BOOL GetFilterGestures();
	afx_msg void SetFilterGestures(BOOL bNewValue);
	afx_msg BOOL GetFilterGeneral();
	afx_msg void SetFilterGeneral(BOOL bNewValue);
	afx_msg BOOL GetFilterTrace();
	afx_msg void SetFilterTrace(BOOL bNewValue);
	afx_msg BOOL GetFilterList();
	afx_msg void SetFilterList(BOOL bNewValue);
	afx_msg BOOL GetFilterReport();
	afx_msg void SetFilterReport(BOOL bNewValue);
	afx_msg BSTR GetAutoSaveFileName();
	afx_msg void SetAutoSaveFileName(LPCTSTR lpszNewValue);
	afx_msg long GetMaxAutoSaveFileSize();
	afx_msg void SetMaxAutoSaveFileSize(long nNewValue);
	afx_msg OLE_COLOR GetSayColor();
	afx_msg void SetSayColor(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetEmoteColor();
	afx_msg void SetEmoteColor(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetThinkColor();
	afx_msg void SetThinkColor(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetWhisperColor();
	afx_msg void SetWhisperColor(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetAnnounceColor();
	afx_msg void SetAnnounceColor(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetGestureColor();
	afx_msg void SetGestureColor(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetShoutColor();
	afx_msg void SetShoutColor(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetReportColor();
	afx_msg void SetReportColor(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetEventColor();
	afx_msg void SetEventColor(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetDirectColor();
	afx_msg void SetDirectColor(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetTraceColor();
	afx_msg void SetTraceColor(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetFriendColor();
	afx_msg void SetFriendColor(OLE_COLOR nNewValue);
	afx_msg void RefreshText();
	afx_msg void OnTrace(LPCTSTR bstr);
	afx_msg void ViewAsHistory();
	afx_msg void ViewAsScript();
	afx_msg SCODE SetWhisperList(const VARIANT FAR& List);
	afx_msg SCODE SetPageNameList(const VARIANT FAR& List);
	afx_msg void AutoSaveNow();
	//}}AFX_DISPATCH
	afx_msg BOOL OnLink(UINT ID,NMHDR* pNMHDR, LRESULT* pResult);

	virtual void OnBackColorChanged();
	virtual void OnFontChanged();
	virtual void OnAmbientPropertyChange(DISPID dispid);



	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CHistoryCtrl)
	void FireOnLink(LPCTSTR URL)
		{FireEvent(eventidOnLink,EVENT_PARAM(VTS_BSTR), URL);}
	void FireOnPrefsChanged()
		{FireEvent(eventidOnPrefsChanged,EVENT_PARAM(VTS_NONE));}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
	BOOL			m_bWasSuper;
	int				m_cyFont;
	int				m_cyCharOld;
	int				m_cyWindow;
	HMODULE			m_hLibModule;

	BOOL CRichEditCtrl_Create(CRichEditCtrl *pCtrl, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	void NewParagraph();
	void CharacterFormat(BOOL bBold, BOOL bItalic, COLORREF crTextColor);
	void Indent(BOOL bIndent);
	void SuperScript(BOOL bSuperscript);
	BOOL SaveAndMoveSel( CHARRANGE *pcr );
	BOOL IsEndInView();
	BOOL IsSelectionEmpty();
public:
enum
	{
		eLNewPara=0x01,
		eLSuperScript=0x02,
		eLBold=0x04,
		eLItalic = 0x08,
		eLIndent=0x10,
	};

	void Log(bstr_t bstr, int eLogOptions, COLORREF crTextColor, BOOL bTrim = FALSE);
	void SetFont( CFont *pfont, BOOL bRedraw );


	void RemoveAll();
	void DisplayText();
	long CHistoryCtrl::TestForHints( IThing *pFrom, IPropertyList *pTo = NULL );

	virtual HRESULT HandleOnTrace( BSTR bstr,BOOL bFromClient);
	virtual HRESULT HandleOnReport( BSTR bstr, long lType, BOOL bFromClient);
	virtual HRESULT HandleOnTell(IThing* pthing, IPropertyList* pToList, BSTR bstr, long lval);
	virtual HRESULT HandleOnEnterInventory(IThing* pWhere, IThing* pWhat);
	virtual HRESULT HandleOnLeaveInventory(IThing* pWhere, IThing* pWhat);
	virtual HRESULT HandleOnConnect(IDispatch* pWorld);
	virtual HRESULT HandleOnUserConnect(IThing* pthing, BOOL bFromClient);
	virtual HRESULT HandleOnUserDisconnect(IThing* pthing, BOOL bFromClient);
	virtual HRESULT HandleOnWield(IThing* pWho, IThing* pWhat);
	virtual HRESULT HandleOnUnWield(IThing* pWho, IThing* pWhat);
	virtual HRESULT HandleOnEnterRoom(IThing* pWhere, IThing* pWhat);
	virtual HRESULT HandleOnLeaveRoom(IThing* pWhere, IThing* pWhat);


public:
	enum {
	//{{AFX_DISP_ID(CHistoryCtrl)
	dispidVWClient = 3L,
	dispidFilterEvents = 4L,
	dispidFilterGestures = 5L,
	dispidFilterGeneral = 6L,
	dispidFilterTrace = 7L,
	dispidFilterList = 8L,
	dispidFilterReport = 9L,
	dispidAutoSaveFileName = 10L,
	dispidMaxAutoSaveFileSize = 11L,
	dispidSayColor = 12L,
	dispidEmoteColor = 13L,
	dispidThinkColor = 14L,
	dispidWhisperColor = 15L,
	dispidAnnounceColor = 16L,
	dispidGestureColor = 17L,
	dispidShoutColor = 18L,
	dispidReportColor = 19L,
	dispidEventColor = 20L,
	dispidDirectColor = 21L,
	dispidTraceColor = 22L,
	dispidFriendColor = 23L,
	dispidAppendOnSave = 1L,
	dispidAutoSave = 2L,
	dispidRefreshText = 24L,
	dispidOnTrace = 25L,
	dispidViewAsHistory = 26L,
	dispidViewAsScript = 27L,
	dispidSetWhisperList = 28L,
	dispidSetPageNameList = 29L,
	dispidAutoSaveNow = 30L,
	eventidOnLink = 1L,
	eventidOnPrefsChanged = 2L,
	//}}AFX_DISP_ID
	};

	CString m_AutoSaveFileName;
	FILE*	m_pAutoSaveFile;
	long	m_AutoSaveFileSize;
	CArray< CTextData, CTextData& > m_HistoryData;
	long m_CurrentIndex;
	CArray<long, long&> m_WhisperList, m_IgnoreList;
	CStringList	m_PageNameList;
	CThingPtr m_UserPtr;
	long m_FilterFlags;

	OLE_COLOR m_rgbSay;
	OLE_COLOR m_rgbEmote;	
	OLE_COLOR m_rgbThink;
	OLE_COLOR m_rgbWhisper;	
	OLE_COLOR m_rgbAnnounce;
	OLE_COLOR m_rgbGesture;
	OLE_COLOR m_rgbShout;
	OLE_COLOR m_rgbReport;
	OLE_COLOR m_rgbEvent;
	OLE_COLOR m_rgbDirect;
	OLE_COLOR m_rgbBackColor;
	OLE_COLOR m_rgbTrace;
	OLE_COLOR m_rgbFriend;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTCTL_H__F5029F79_AF74_11D0_A78C_00A0C90AAA10__INCLUDED)
