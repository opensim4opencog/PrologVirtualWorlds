// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// VWUIView.h : header file
//
#include <cmddel.h>
#include <vwuiobjs.h>
#include <..\vwocx\vwview\viewctl.h>

/////////////////////////////////////////////////////////////////////////////
// Helper class for drag/drop support

class CVWUIView;

class CVWUIDropTarget: public COleDropTarget
{
public:
	void Initialize(CVWUIView *pParent);

	// overrides
    DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
    DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
    BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);

protected:
	CVWUIView *m_pParent;
};


/////////////////////////////////////////////////////////////////////////////
// CVWUIView window

class CVWUIView : public CVWViewCtrl
{
	friend class CVWUIDropTarget;

	DECLARE_DYNCREATE(CVWUIView)
// Construction
public:
	CVWUIView();

	// Attributes
public:

// Operations

public:
//#ifdef _VIEW_STANDALONE_CLASS_
	DECLARE_OLECREATE_EX(CVWUIView)    // Class factory and guid
	DECLARE_OLETYPELIB(CVWUIView)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CVWUIView)     // Property page IDs
//#endif
	DECLARE_OLECTLTYPE(CVWUIView)		// Type name and misc status

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVWUIView)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVWUIView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CVWUIView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CVWUIView)
	afx_msg LPDISPATCH GetTargetObjectProperty();
	afx_msg void SetTargetObjectProperty(LPDISPATCH newValue);
	afx_msg LPDISPATCH GetControlManager();
	afx_msg void SetControlManager(LPDISPATCH newValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

BEGIN_INTERFACE_PART(VWUIView, IVWUIView)		 
		STDMETHOD(GetTypeInfoCount)(unsigned int *);				
		STDMETHOD(GetTypeInfo)(unsigned int,unsigned long,			
								struct ITypeInfo ** );				
		STDMETHOD(GetIDsOfNames)(const struct _GUID &,				
			unsigned short ** ,unsigned int,unsigned long,long *);  
		STDMETHOD(Invoke)(long,const struct _GUID &,unsigned long,	
				unsigned short,struct tagDISPPARAMS *,				
				struct tagVARIANT *,struct tagEXCEPINFO *,			
				unsigned int *); 									
		STDMETHOD(get_HWND)(HWND*);									
// UITool Management
		STDMETHOD(LoadUIToolByCLSID)(REFCLSID,BSTR);					
		STDMETHOD(LoadUIToolByProgID)(BSTR,BSTR);					
		STDMETHOD(AddCurrentUITool)(BSTR);					
		STDMETHOD(RemoveCurrentUITool)(BSTR);	
		STDMETHOD(get_CurrentUIToolList)(IPropertyList**);					
		STDMETHOD(get_LoadedUIToolMap)(IPropertyMap**);					
		STDMETHOD(get_LoadedUITool)(BSTR, IVWUITool**);					
		STDMETHOD(get_CurrentUIToolNameList)(IPropertyList**);					
		STDMETHOD(put_CurrentUIToolNameList)(IPropertyList*);	
		
		STDMETHOD(put_TargetObjectProperty)(struct IObjectProperty *pOP);	
		STDMETHOD(get_TargetObjectProperty)(struct IObjectProperty * * ppOP); 
		STDMETHOD(get_ControlManager)(struct IVWControlManager * * ppCM); 
		STDMETHOD(HitTest)(VARIANT varHitparam, long nFlags, VARIANT *varHitResult); 
		STDMETHOD(OptionalSelection)(VARIANT varSelection); 
		STDMETHOD(DeviceToWorld)( float *pflX, float *pflY, float *pflZ );

	END_INTERFACE_PART(VWUIView)

	BEGIN_INTERFACE_PART(VWUIEvents,IVWUIEvents )
		STDMETHOD (GetTypeInfoCount)(UINT * pctinfo);
		STDMETHOD (GetTypeInfo)(UINT iTInfo,LCID lcid,ITypeInfo ** ppTInfo );

		STDMETHOD (GetIDsOfNames)(REFIID riid,LPOLESTR * rgszNames,UINT cNames,LCID lcid,DISPID * rgDispId);
		STDMETHOD (Invoke ) (
				DISPID dispIdMember,
                REFIID riid,
                LCID lcid,
                WORD wFlags,
                DISPPARAMS * pDispParams,
                VARIANT * pVarResult,
                EXCEPINFO * pExcepInfo,
                UINT * puArgErr );

       	STDMETHOD(OnUIUpdate)(int, IVWUIView*,VARIANT, VARIANT_BOOL);
	END_INTERFACE_PART(VWUIEvents)

	BEGIN_INTERFACE_PART(VWExecuteUndo, IVWExecuteUndo)		 
		STDMETHOD(GetTypeInfoCount)(unsigned int *);				
		STDMETHOD(GetTypeInfo)(unsigned int,unsigned long,			
								struct ITypeInfo ** );				
		STDMETHOD(GetIDsOfNames)(const struct _GUID &,				
			unsigned short ** ,unsigned int,unsigned long,long *);  
		STDMETHOD(Invoke)(long,const struct _GUID &,unsigned long,	
				unsigned short,struct tagDISPPARAMS *,				
				struct tagVARIANT *,struct tagEXCEPINFO *,			
				unsigned int *); 									
		STDMETHOD(Undo)(IVWUndoItem*);									
		STDMETHOD(Redo)(IVWUndoItem*);								
	END_INTERFACE_PART(VWExecuteUndo)

	DECLARE_INTERFACE_MAP()											
	CCmdDelegate m_CDelegate;										
	IObjectProperty * m_pTargetObjectProperty;	
	IVWControlManager * m_pControlManager;
	IVWUIView * m_pVWUIView;
	IPropertyMap * m_pmapLoadedUITools;
	IVWExecuteUndo *m_pVWExecuteUndo;

	// dragdrop support
	CVWUIDropTarget	m_dropTarget;

//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam );
	virtual void SetVWClient(LPDISPATCH newValue);
	
#if 0
	HRESULT Advise( IUnknown *pUnk,REFIID riid, ULONG *pdwCoookie );
	HRESULT UnAdvise( IUnknown *pUnk,REFIID riid, ULONG dwCoookie );
#endif

	ULONG m_dwUIViewEventsCookie;
	// Setup ControlManager 
	virtual BOOL CreateHelperComponents(VARIANT_BOOL bCreateControlManager = VARIANT_TRUE);
	virtual void ReleaseHelperComponents();
	// Load all UITools needed by the UIView 
	virtual HRESULT LoadMyUITools(){ return TRUE; };
	BOOL m_fUIToolsLoaded;

	void Refresh( void );
	virtual void HandleRefresh( void );
	virtual BOOL HandleOnMessage(UINT message, WPARAM wParam, LPARAM lParam);

	// enables drop support
	void EnableDropFile(BOOL bEnable);

	// IVWExecuteUndo event handlers
	virtual HRESULT HandleUndo(IVWUndoItem*){ return E_NOTIMPL; };	
	virtual HRESULT HandleRedo(IVWUndoItem*){ return E_NOTIMPL; };	

	// drop handler
	virtual BOOL HandleDropFile(CPoint pt, LPCTSTR pszFilename){ return FALSE; };

protected:
	virtual HRESULT HandleUIUpdate( enumVWUIEventTypes, IVWUIView*,VARIANT varHint ){return S_OK;};							
	virtual HRESULT HandleHitTest( VARIANT varHit, long nFlags, VARIANT * pvarResult ){return S_OK;};							
	virtual HRESULT HandleOptionalSelection( VARIANT varSelection ) { return S_OK; };	
	virtual HRESULT HandleDeviceToWorld( float *pflX, float *pflY, float *pflZ ){return E_NOTIMPL;};
	virtual HRESULT HandleOnUIEvent(IThing* pthing, BSTR bstrEventName, VARIANT varArg, BOOL bFromClient);

	HRESULT UpdateCurrentUITools();

private:
	IPropertyList * m_plistCurrentUITools;
	CRITICAL_SECTION m_CS;
};

/////////////////////////////////////////////////////////////////////////////
