// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// Artifact.h : Declaration of the CArtifact

#ifndef __ARTIFACT_H_
#define __ARTIFACT_H_

#include "resource.h"       // main symbols
#include "objimpl.h"

/////////////////////////////////////////////////////////////////////////////
// CArtifact

EXTERN_C const CLSID CLSID_ArtifactExemplar;

#define ReportArtifactError(hr) SUCCEEDED(hr) ? hr : ReportError("VWSYSTEM.Artifact.1", hr, 0xc40, 0xc50, "VWARTIFACT")

DECLARE_VWDISPATCH(ArtifactExemplar, LIBID_VWSYSTEMLib )
{
public:

DECLARE_REGISTRY_RESOURCEID(IDR_ARTIFACTEXEMPLAR)

BEGIN_COM_MAP(CArtifactExemplarObject)
	COM_INTERFACE_ENTRY(IArtifactExemplar)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

// IArtifact
public:
	STDMETHOD(AcceptLeave)(VARIANT_BOOL *pbool);
	STDMETHOD(AcceptEnter)(IThing *pThing, VARIANT_BOOL *pbool);
	STDMETHOD(Install)(IModule* pModule);
	STDMETHOD(WieldItem)();
	STDMETHOD(UpdateWield)(IMenuItem *pmi);
	STDMETHOD(UnWieldItem)();
	STDMETHOD(UpdateUnWield)(IMenuItem *pmi);
};

#endif //__ARTIFACT_H_
