// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// helper functions...
HRESULT ParseURLHelper(BSTR bstrURL, BSTR* pbstrOut, BSTR* pbstrProtocol, BSTR* pbstrServer, int* pnPort);
BOOL IsFullURL(BSTR bstrURL);
BOOL ExtractFirstURL(CString& in, CString& out);
