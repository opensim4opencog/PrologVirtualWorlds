// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// Convert3DS.h : Declaration of the CConvert3DSGeometryGeometry

#ifndef __CONVERT3DS_H_
#define __CONVERT3DS_H_

#include "resource.h"       // main symbols
#include <ddraw.h>
#include <d3d.h>
#include <d3drm.h>

#include <d2d.h>

#include "ConvGeEr.h"

extern "C" 
{
#include "3DSFTK\INC\3DSFTK.H"
}

EXTERN_C const IID LIBID_VWMMLib;
EXTERN_C const CLSID CLSID_Convert3DSGeometry;

/////////////////////////////////////////////////////////////////////////////
// CConvert3DSGeometry
class ATL_NO_VTABLE CConvert3DSGeometry : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CConvert3DSGeometry, &CLSID_Convert3DSGeometry>,
	public ISupportErrorInfo,
	public IDispatchImpl<IConvert3DSGeometry, &IID_IConvert3DSGeometry, &LIBID_VWMMLib>
{

public:

DECLARE_REGISTRY_RESOURCEID(IDR_CONVERT3DS)

BEGIN_COM_MAP(CConvert3DSGeometry)
	COM_INTERFACE_ENTRY(IConvert3DSGeometry)
	COM_INTERFACE_ENTRY(IConvertGeometry)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()


	CConvert3DSGeometry();
	~CConvert3DSGeometry();

	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IConvert3DS
public:
	STDMETHOD(Init)(LPDIRECT3DRM piD3DRMParam, LPDIRECT2DRM piD2DRMParam, IInternetFileManager* piInternetFileManagerParam);

	STDMETHOD(get_KeepCachedTextureNames)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_KeepCachedTextureNames)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_LoadTextures)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_LoadTextures)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_ConvertAnimation)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_ConvertAnimation)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(Name)(BSTR* bstr);
	STDMETHOD(Extensions)(BSTR* bstr);
	STDMETHOD(get_ScaleFactor)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_ScaleFactor)(/*[in]*/ double newVal);

	//---------------------------------------------------------------
	// Contained Types for building geometry  (Node and NodeList)
	//---------------------------------------------------------------
	class Node;

	// The NodeList class /////////////////////////////////////
	class NodeList
	{
	public:
		~NodeList();
		NodeList();

		void		Add( Node *pnode ) { m_list.AddTail( (void *) pnode ); }
		int			GetCount() { return m_list.GetCount();}
		void		RemoveAt( POSITION pos ) { m_list.RemoveAt( pos ); }
		void		RemoveAll() { m_list.RemoveAll(); }
		Node*		GetAt( POSITION pos ) { return (Node *) m_list.GetAt( pos ); }
		POSITION	GetHeadPosition( ) { return m_list.GetHeadPosition( ); }

		POSITION 	Find( const char * pszName );

		HRESULT		Dump(LPDIRECT3DRMFRAME piD3DParentFrame, CConvert3DSGeometry* pConverterObject);

	protected:
		CPtrList	m_list;
	};

	///// The Node class /////////////////////////////////////////////
	class Node
	{
	friend class NodeList;
	public:
		Node( mesh3ds *p3dsMesh, ulong3ds imesh );
		~Node();

		void SetPivot( point3ds *pPnt3dsPivot );
		void AddChild( Node *pnode );
		HRESULT Dump(LPDIRECT3DRMFRAME piD3DParentFrame, CConvert3DSGeometry* pConverterObject);

	protected:
		char*		m_pszName;
		ulong3ds	m_imesh;
		point3ds	m_pnt3dsOffset;
		Node*		m_pnodeParent;
		mesh3ds*	m_p3dsMesh;
		NodeList	m_listChildren;

	};

	class MaterialNode
	{
	public:
		MaterialNode() { m_piD3DMaterial = NULL; m_p3dsMaterial = NULL; }
		~MaterialNode()
		{
			if (m_p3dsMaterial)
				ReleaseMaterial3ds( &m_p3dsMaterial );

			if (m_piD3DMaterial)
				m_piD3DMaterial->Release();
		}

		LPDIRECT3DRMMATERIAL	m_piD3DMaterial;
		material3ds*			m_p3dsMaterial;
	};

	//---------------------------------------------------------------
	// Member methods for CConvert3DSGeometry
	//---------------------------------------------------------------
	STDMETHOD(ReadScene)(BSTR bstr, 
						 ITextureCallback* pCGTCallback,
						 VARIANT* pvarFrame); //type will be IDirect3DRMFrame

	HRESULT ConvertFile(LPDIRECT3DRMFRAME piD3DParentFrame);

   	//---------------------------------------------------------------
	// Member variables for CConvert3DSGeometry
	//---------------------------------------------------------------


	//Necessary objects
	LPDIRECT3DRM			m_piD3DRM;			// Direct3DRM object
	IInternetFileManager*	m_piInternetFileManager;
	LPDIRECT2DRM			m_piD2DRM;			// Direct3DRM object
	LPDIRECTDRAW			m_piDDraw;			// DirectDraw object.  Not used unless we need to
	ITextureCallback*		m_pCGTCallback;		// Callback to load textures

	//Properties
	double m_flMasterScale;			// Global scaling value
	BOOL   m_bConvertAnimations;
	BOOL   m_bLoadTextures;
	BOOL   m_bKeepCachedTextureNames;

	// Maintains list of mesh nodes to build hierarchy with
	NodeList m_listNodes;
	// Stores list of created materials to be assigned 
	CMapStringToPtr m_mapMaterialList;

	//3DStudio file database
	database3ds*	m_p3dsDatabase;

	// Flags
	BOOL m_bInitCalled;				//Set when init() is called
};

#endif //__CONVERT3DS_H_
