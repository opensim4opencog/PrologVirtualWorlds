// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

//registry.h

HRESULT WriteStuffToRegistry
	(CString defaultalias,CString alias,CString email,CString IsInWorld,
					  CString comment, CString exemplar, CString graphics, 
					  CString room);

HRESULT LoadStuffFromRegistry(CString &alias,CString &email,CString &IsInWorld,
					  CString &comment, CString &exemplar, CString &graphics, CString &room);


HRESULT DeleteAlias(CString alias);
