// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#include <vwprop.h>
#include <menuitem.h>
#include <ocxproxy.h>

HRESULT CreatePropertyList(IWorld* pWorld, IPropertyList** pppropertylist);
HRESULT CreatePropertyMap(IWorld* pWorld, IPropertyMap** pppropertymap);

HRESULT CreateMenu(IWorld* pWorld, IPropertyList** pppropertylist);
HRESULT CreateMenuItem(IWorld* pWorld, BSTR bstrName, long lFlags, IThing *pOwner, BSTR bstrUpdateMethod, BSTR bstrMethod, VARIANT var, IMenuItem **ppMenuItem);
HRESULT CreateMenuItemExt(IWorld* pWorld, BSTR bstrName, long lFlags, IThing *pOwner, BSTR bstrUpdateMethod, BSTR bstrMethod, DISPPARAMS *params, IMenuItem **ppMenuItem);
HRESULT CreateMenuSeparator(IWorld* pWorld, IThing *pOwner, IMenuItem **ppMenuItem);

HRESULT CreateOCXProxy(IWorld* pWorld, IDispatch *pOCX, IOCXProxy **ppOCXProxy);

HRESULT CreateMarshallBuffer(IMarshallBuffer** ppMarshBuf);
HRESULT CreateUnMarshallBuffer(IUnMarshallBuffer** ppMarshBuf);

HRESULT ShowDialog(IWorld* pWorld, VARIANT_BOOL bIsModal, BSTR bstrURL, IDispatch* pArguments, BSTR bstrWindowFeatures);
