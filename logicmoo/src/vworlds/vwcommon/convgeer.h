// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.




#define VWCONVGEOM_E_BADEXTERNOBJECTS	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c00)
//An error occured allocting one or more of IDirect3DRM, IDirect2DRM, or IInternetFileManager

#define VWCONVGEOM_E_INITNOTCALLED	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c01)
//The Init() function has not been called or it failed to initialize external objects;

#define VWCONVGEOM_E_FILENOTFOUND	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c02)
//Could not find given filename to convert.

#define VWCONVGEOM_E_FILENOTCLOSED	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c03)
//Converted file could not be closed.

#define VWCONVGEOM_E_OUTOFMEMORY	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c05)
//Out of memory

#define VWCONVGEOM_E_BADFILEFORMAT	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c06)
//The file specificed to convert is not the correct format or is corrupt.

#define VWCONVGEOM_E_BADREAD	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c07)
//Couldn't read a necessary element of file, file is corrupt, or failed to allocate memory for element.

#define VWCONVGEOM_E_ERRORMEMFREE	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c08)
//Could not free specified pointer, ie.  delete or free failed.

#define VWCONVGEOM_E_BADMATERIAL	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c09)
//Couldn't create the specified material


/////////////////////////////////////////////////////////////////////////////
