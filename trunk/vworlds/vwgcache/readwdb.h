// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include <d3drmwin.h>
#include <inetfile.h>
// #include <d2d.h>

class CWDBReader
{
public:
	static BOOL ReadWDB(const char* psz, 
						const char* szPath,
						IDirect3DRMFrame* prlfGeometry,
						CVWGeometryCacheObject *pGeometryCacheObject);

private:
	static void ReadFrame( CArchive& ar, IDirect3DRMFrame *prlFrame );
	static void ReadMesh( CArchive& ar, IDirect3DRMFrame *prlfParent, int cFrames, int nPeriod );
};

HRESULT LoadTextureCB(char *szName, void *pvArg, LPDIRECT3DRMTEXTURE *ppTexture);
