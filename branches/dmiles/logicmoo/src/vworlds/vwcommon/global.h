// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _GLOBAL_HEADER_
#define _GLOBAL_HEADER_
//global.h
//This header file contains variables used by a number of projects. Changes
//here will force recompilation of all of them.

#include <COMDEF.H>

#define nRandom				0x011760		

// version for client/server protocol changes
#define ucSchema			1
#define ucSubSchema			1
#define PROTOCOL_VERSION	((DWORD)((nRandom<<16)+(ucSchema<<8)+ucSubSchema))

#define nDBRandom			0x082468		

// version for marshalling changes (to disk or network)
#define ucDBSchema			1
#define ucDBSubSchema		1
#define DATABASE_VERSION	((DWORD)((nDBRandom<<16)+(ucDBSchema<<8)+ucDBSubSchema))

//used by vwclient and vwserver projects.
//millisecond delay until logoff enforced.
const unsigned long LOGOFF_WAIT_TIME=3000;

//time server stays up to service sockets closing.
const unsigned long SHUTDOWN_WAIT_TIME=3000;

//Server async callbacks.
#define WM_ONAVAILABLEWORLD (WM_USER + 100)

#define WM_ONREPORT (WM_USER + 101)

#define C_CLOSEDOCUMENT 123 //anything but zero.

class REPORTSTRUCT
{
public:
	BSTR bstrWorldName;
	BSTR bstrMessage;
	long lType;

	
	~REPORTSTRUCT() 
	{	
		::SysFreeString(bstrWorldName);
		::SysFreeString(bstrMessage);
	}
};

//Queue code: Windows message for callback notification
#define WM_ASYNC_CALL_FINISHED (WM_USER+10)
#define WM_SYNC_CALL_FINISHED (WM_USER+11)

#endif
