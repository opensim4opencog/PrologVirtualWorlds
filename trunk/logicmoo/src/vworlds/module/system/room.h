// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// Room.h : Declaration of the CRoom

#ifndef __ROOM_H_
#define __ROOM_H_

#include "resource.h"       // main symbols
#include "objimpl.h"

/////////////////////////////////////////////////////////////////////////////
// CRoom

EXTERN_C const CLSID CLSID_RoomExemplar;

#define ReportRoomError(hr) SUCCEEDED(hr) ? hr : ReportError("VWSYSTEM.Room.1", hr, 0xc70, 0xc80, "VWROOM")

DECLARE_VWDISPATCH( RoomExemplar, LIBID_VWSYSTEMLib )
{
public:
DECLARE_REGISTRY_RESOURCEID(IDR_ROOMEXEMPLAR)

BEGIN_COM_MAP(CRoomExemplarObject)
	COM_INTERFACE_ENTRY(IRoomExemplar)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IVWDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

// IRoom 

public:
	STDMETHOD(Install)(IModule* pModule);
	STDMETHOD(Expel)(IThing *pThing, IThing *pDoor, VARIANT_BOOL *pBool);

	STDMETHOD(TransmitWhisper)(BSTR bstrMsg, VARIANT varNameList);
	STDMETHOD(ReceiveWhisper)(BSTR bstrMsg, IPropertyList *pTargetList);

	STDMETHOD(AllowLeave)(IThing *pThing, VARIANT_BOOL *pbool);
	STDMETHOD(AllowEnter)(IThing *pThing, VARIANT_BOOL *pbool);

	STDMETHOD(OnContentEnter)(IThing *pWhere, IThing *pWhat );
	STDMETHOD(OnContentLeave)(IThing *pOldWhere, IThing *pWhat );

	STDMETHOD(OnSystemCreate)(IThing *pThing);
	STDMETHOD(OnSystemDestroy)(IThing *pThing);
	
	STDMETHOD(OnContentSystemConnect)(IThing *pWho);
	STDMETHOD(OnContentSystemDisconnect)(IThing *pWho);
	STDMETHOD(OnContentSystemTell)(IThing* pfrom, IPropertyList* pto, BSTR bstr, long lval, BSTR *pbstrOut);
};

#endif //__ROOM_H_
