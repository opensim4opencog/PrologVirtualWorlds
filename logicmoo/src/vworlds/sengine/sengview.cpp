// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// SEngineView.cpp : implementation of the CSEngineView class
//

#include "stdafx.h"
#include "SEngine.h"

#include "SEngDoc.h"
#include "SEngView.h"
#include "GotoDlg.h"

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/
/////////////////////////////////////////////////////////////////////////////
// CSEngineView

IMPLEMENT_DYNCREATE(CSEngineView, CEditView)

BEGIN_MESSAGE_MAP(CSEngineView, CEditView)
	//{{AFX_MSG_MAP(CSEngineView)
	ON_COMMAND(ID_EDIT_GOTOLINE, OnEditGoto)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GOTOLINE, OnUpdateEditGoto)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_EDIT_DEBUGOFFBY1, OnEditDebugoffby1)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DEBUGOFFBY1, OnUpdateEditDebugoffby1)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSEngineView construction/destruction

CSEngineView::CSEngineView()
{
	// TODO: add construction code here
	m_cOffBy1 = 0;
}

CSEngineView::~CSEngineView()
{


}

BOOL CSEngineView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	//cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}

/////////////////////////////////////////////////////////////////////////////
// CSEngineView drawing

void CSEngineView::OnDraw(CDC* pDC)
{
	CSEngineDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CSEngineView printing

BOOL CSEngineView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CEditView preparation
	return CEditView::OnPreparePrinting(pInfo);
}

void CSEngineView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing.
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CSEngineView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CSEngineView diagnostics

#ifdef _DEBUG
void CSEngineView::AssertValid() const
{
	CEditView::AssertValid();
}

void CSEngineView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CSEngineDoc* CSEngineView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSEngineDoc)));
	return (CSEngineDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSEngineView message handlers





void CSEngineView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( !pHint ) return;
	switch ( lHint  ){
	case E_RUNTIME:
		{
			CScriptSite::RunTimeErrorInfo * prteInfo = (CScriptSite::RunTimeErrorInfo *) pHint;
			CEdit &edit = GetEditCtrl();
			int line = prteInfo->ulLine - m_cOffBy1; 
			int index = edit.LineIndex(line);
			int len = edit.LineLength( index );
			edit.SetSel( index , index + len, FALSE );
	
		}
	break;
	default:
	break;
	}
}

void CSEngineView::OnEditGoto() 
{
	// TODO: Add your command handler code here
	CGotoDialog Dlg;
	if ( Dlg.DoModal() == IDOK )
	{
			CEdit &edit = GetEditCtrl();
			int index = edit.LineIndex(Dlg.m_LineNumber);
			int len = edit.LineLength( index );
			edit.SetSel( index , index + len, FALSE );
	}
	
}

void CSEngineView::OnUpdateEditGoto(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable( TRUE );
}

void CSEngineView::UpdateRowColInfo(){
static lastline = -1;
static lastcol  = -1;

	if ( !m_hWnd ) return;

	CEdit &edit = GetEditCtrl();
	CPoint point =edit.GetCaretPos();

	int line = edit.CharFromPos( point);
	int index = edit.LineIndex( );
	int col  = LOWORD( line) - index;
	line = HIWORD( line );
	if (( line == lastline ) &&
		( col == lastcol))	return;
	lastline = line;
	lastcol = col;

	CString strLine;
	strLine.Format("Ln %d, Col %d", line, col );
	//GetDocument()->SetStatusText( strLine );
	CFrameWnd * pFrame = AfxGetMainWnd()->GetTopLevelFrame();
	CStatusBar* pStatusBar = ( CStatusBar *) pFrame->GetControlBar( AFX_IDW_STATUS_BAR);
	if ( pStatusBar ){
		int index = pStatusBar->CommandToIndex( ID_INDICATOR_ROWCOL );
		pStatusBar->SetPaneText(index, strLine);

	}

	
}

void CSEngineView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CEditView::OnKeyDown(nChar, nRepCnt, nFlags);
	UpdateRowColInfo();
}

void CSEngineView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CEditView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	UpdateRowColInfo();

}

void CSEngineView::OnEditDebugoffby1() 
{
	// TODO: Add your command handler code here
	m_cOffBy1 = m_cOffBy1 ? 0: 1;
}

void CSEngineView::OnUpdateEditDebugoffby1(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_cOffBy1 );
}

void CSEngineView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CEditView::OnLButtonDown(nFlags, point);
		UpdateRowColInfo();
}

void CSEngineView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CEditView::OnRButtonDown(nFlags, point);
	UpdateRowColInfo();
}
