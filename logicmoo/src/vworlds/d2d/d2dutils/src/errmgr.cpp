// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

// File:	ErrMgr.cpp
// Author:	MM    (MM)

#include "D2DUtils.h"

// load directdraw and direct3d so that we can deal with those error messages
#include <d3drm.h>

// get a facility code for DirectDraw
#ifndef _FACDD
	#ifdef _FAC
		#define _FACDD _FAC
	#endif
#endif
#ifndef _FACDD
	#define _FACDD 0x876
#endif
#define FACILITY_DDRAW _FACDD

//
// GLOBALS
//
ErrMgr g_emErr;
HRESULT g_hrErr;

#define nERRBUFSIZE 384

static char gs_szUnkNumBuf[32] = "unknown error - 0x";
static const int gs_nUnkNumBufOffset = 18;
static char gs_szUnkFacBuf[64] = "unknown error (invalid facility code) - 0x";
static const int gs_nUnkFacBufOffset = 42;

HRESULT 
DefaultFailureCallback(HRESULT hr, const char *szMsgFmt, va_list pMsgArgs)
{
	char szErrMgr[nERRBUFSIZE];
	ErrMgr::ComposeGenericString(hr, szErrMgr, nERRBUFSIZE, szMsgFmt, pMsgArgs);

	// print a message to debug output based on HRESULT
	DBGMSG0(szErrMgr);
	return hr;
}


ErrMgr::ErrMgr(FailureCallback pfnFailureCallback)
{
	m_hrLast = S_OK;
	SetFailureCallback(pfnFailureCallback);
}

// Function: ComposeGenericString
//    This function writes a null terminated error string to the 
//  specified buffer and returns the number of characters written, not
//  counting the terminating null character.  The function is guaranteed
//  not to write more than nBufSize characters.
int
ErrMgr::ComposeGenericString(HRESULT hr, char *pErrBuf, int nBufSize, 
					   const char *szMsgFmt, va_list pArgs)
{
	const char *szFacName, *szMsgType, *szSysErr;

	GetSubStrings(hr, &szFacName, &szMsgType, &szSysErr);

	if (szMsgFmt && ((hr == S_MESSAGE) || (hr == E_GENERIC) || (hr == E_FAIL)))
		szSysErr = "";

	return ComposeSubStrings(pErrBuf, nBufSize, szFacName, szMsgType, szSysErr, szMsgFmt, pArgs);
}

// Function: GetSubStrings
//    This function assigns strings to each of the error message substrings.
//  All substrings are guaranteed to be non-NULL, tho possibly empty ("").
void
ErrMgr::GetSubStrings(HRESULT hr, const char **pszFacName, const char **pszMsgType,
					  const char **pszSysErr)
{
	static const char *rgszMsgType[2] = {"message", "error"};
	static const char *rgszFacNames[] = {
		"", "rpc ", "dispatch ", "storage ", "user ", "unknown ", "unknown ",
		"win32 ", "windows ", "sspi ", "control ", "cert "
	};
	static HRToStringFunc rgpfnFacErrToString[] = {
		ErrMgr::FacNullErrToString, ErrMgr::FacRPCErrToString,
		ErrMgr::FacDispatchErrToString, ErrMgr::FacStorageErrToString,
		ErrMgr::FacITFErrToString, ErrMgr::FacUnknownErrToString,
		ErrMgr::FacUnknownErrToString, ErrMgr::FacWin32ErrToString,
		ErrMgr::FacWindowsErrToString, ErrMgr::FacSSPIErrToString,
		ErrMgr::FacControlErrToString, ErrMgr::FacCertErrToString
	};
	int nFac = HRESULT_FACILITY(hr);
	MMASSERT(nFac >= 0);

	*pszMsgType = rgszMsgType[FAILED(hr)];

	if (nFac <= FACILITY_CERT) {
		// this is a facility defined in winerror.h as of VC4.2
		*pszFacName = rgszFacNames[nFac];
		*pszSysErr = rgpfnFacErrToString[nFac](hr);
	} else {
		switch(nFac) { 
		case FACILITY_DDRAW:
			// we also know about DirectX HRESULTS (as of DX2)
			*pszFacName = "directx "; *pszSysErr = FacDDrawErrToString(hr); break;
		default:
			*pszFacName = "unknown "; *pszSysErr = FacUnknownErrToString(hr); break;
		}
	}
	MMASSERT(*pszFacName && *pszMsgType && *pszSysErr);
}

int
ErrMgr::ComposeSubStrings(char *pErrBuf, int nBufSize, const char *szFacName,
						  const char *szMsgType, const char *szSysErr,
						  const char *szMsgFmt, va_list pArgs)
{
	// prepare to write to the error message buffer
	const int nReservedBytes = 8;	// "\n\r\t- " + "\n\r\0"
	int nBytesTotal = 0, nBytesLast, nBytesLeft = nBufSize - nReservedBytes;
	
	if (nBytesLeft <= 0) {
		pErrBuf[0] = chNULL;
		return 0;
	}

	nBytesLast = _snprintf(pErrBuf, nBytesLeft, "%s%s: %s", szFacName, szMsgType, szSysErr);

	if (nBytesLast >= 0) {
		nBytesLeft -= nBytesLast;
		nBytesTotal += nBytesLast;

		// append the user's message to the error, if it exists
		if (szMsgFmt && szMsgFmt[0]) {
			if (szSysErr[0]) {
				strcpy(pErrBuf + nBytesTotal, "\n\r\t- ");
				nBytesTotal += 5;
			}
			nBytesLast =_vsnprintf(pErrBuf + nBytesTotal, nBytesLeft, szMsgFmt, pArgs);
			nBytesTotal += (nBytesLast < 0 ? nBytesLeft : nBytesLast);
		}
	} else {
		nBytesTotal += nBytesLeft;
	}
	// fill reserved slots with bytes we always want at the end of the message
	pErrBuf[nBytesTotal++] = '\n';
	pErrBuf[nBytesTotal++] = '\r';
	pErrBuf[nBytesTotal] = chNULL;

	return nBytesTotal;
}

const char *
ErrMgr::UnkErrNumToString(HRESULT hr)
{
	sprintf(gs_szUnkNumBuf + gs_nUnkNumBufOffset, "%08lx", hr);
	return gs_szUnkNumBuf;
}

const char *
ErrMgr::FacUnknownErrToString(HRESULT hr)
{
	sprintf(gs_szUnkFacBuf + gs_nUnkFacBufOffset, "%08lx", hr);
	return gs_szUnkFacBuf;
}

#define mmm(__hrMMM,__szMMM) case __hrMMM: return #__hrMMM##" - "##__szMMM
#define xxx(__hrMMM) case __hrMMM: return #__hrMMM

// Function: FacWindowsErrToString
//    Convert all the FACILITY_WINDOWS errors to strings.
const char *
ErrMgr::FacWindowsErrToString(HRESULT hr)
{
	//
	// Codes 0x0-0x01ff are reserved for the OLE group of
	// interfaces.
	//
	switch (hr) {
	mmm(CO_E_CLASS_CREATE_FAILED,"Attempt to create a class object failed");
	mmm(CO_E_SCM_ERROR,"OLE service could not bind object");
	mmm(CO_E_SCM_RPC_FAILURE,"RPC communication failed with OLE service");
	mmm(CO_E_BAD_PATH,"Bad path to object");
	mmm(CO_E_SERVER_EXEC_FAILURE,"Server execution failed");
	mmm(CO_E_OBJSRV_RPC_FAILURE,"OLE service could not communicate with the object server");
	mmm(MK_E_NO_NORMALIZED,"Moniker path could not be normalized");
	mmm(CO_E_SERVER_STOPPING,"Object server is stopping when OLE service contacts it");
	mmm(MEM_E_INVALID_ROOT,"An invalid root block pointer was specified");
	mmm(MEM_E_INVALID_LINK,"An allocation chain contained an invalid link pointer");
	mmm(MEM_E_INVALID_SIZE,"The requested allocation size was too large");
	mmm(CO_S_NOTALLINTERFACES,"Not all the requested interfaces were available");
	default:
		return UnkErrNumToString(hr);
	}
}

// Function: FacStorageErrToString
//    Convert all the FACILITY_STORAGE errors to strings.
const char *
ErrMgr::FacStorageErrToString(HRESULT hr)
{
	switch (hr) {
	mmm(STG_E_INVALIDFUNCTION,"Unable to perform requested operation.");
	mmm(STG_E_FILENOTFOUND,"%1 could not be found.");
	mmm(STG_E_PATHNOTFOUND,"The path %1 could not be found.");
	mmm(STG_E_TOOMANYOPENFILES,"There are insufficient resources to open another file.");
	mmm(STG_E_ACCESSDENIED,"Access Denied.");
	mmm(STG_E_INVALIDHANDLE,"Attempted an operation on an invalid object.");
	mmm(STG_E_INSUFFICIENTMEMORY,"There is insufficient memory available to complete operation.");
	mmm(STG_E_INVALIDPOINTER,"Invalid pointer error.");
	mmm(STG_E_NOMOREFILES,"There are no more entries to return.");
	mmm(STG_E_DISKISWRITEPROTECTED,"Disk is write-protected.");
	mmm(STG_E_SEEKERROR,"An error occurred during a seek operation.");
	mmm(STG_E_WRITEFAULT,"A disk error occurred during a write operation.");
	mmm(STG_E_READFAULT,"A disk error occurred during a read operation.");
	mmm(STG_E_SHAREVIOLATION,"A share violation has occurred.");
	mmm(STG_E_LOCKVIOLATION,"A lock violation has occurred.");
	mmm(STG_E_FILEALREADYEXISTS,"%1 already exists.");
	mmm(STG_E_INVALIDPARAMETER,"Invalid parameter error.");
	mmm(STG_E_MEDIUMFULL,"There is insufficient disk space to complete operation.");
	mmm(STG_E_PROPSETMISMATCHED,"Illegal write of non-simple property to simple property set.");
	mmm(STG_E_ABNORMALAPIEXIT,"An API call exited abnormally.");
	mmm(STG_E_INVALIDHEADER,"The file %1 is not a valid compound file.");
	mmm(STG_E_INVALIDNAME,"The name %1 is not valid.");
	mmm(STG_E_UNKNOWN,"An unexpected error occurred.");
	mmm(STG_E_UNIMPLEMENTEDFUNCTION,"That function is not implemented.");
	mmm(STG_E_INVALIDFLAG,"Invalid flag error.");
	mmm(STG_E_INUSE,"Attempted to use an object that is busy.");
	mmm(STG_E_NOTCURRENT,"The storage has been changed since the last commit.");
	mmm(STG_E_REVERTED,"Attempted to use an object that has ceased to exist.");
	mmm(STG_E_CANTSAVE,"Can't save.");
	mmm(STG_E_OLDFORMAT,"The compound file %1 was produced with an incompatible version of storage.");
	mmm(STG_E_OLDDLL,"The compound file %1 was produced with a newer version of storage.");
	mmm(STG_E_SHAREREQUIRED,"Share.exe or equivalent is required for operation.");
	mmm(STG_E_NOTFILEBASEDSTORAGE,"Illegal operation called on non-file based storage.");
	mmm(STG_E_EXTANTMARSHALLINGS,"Illegal operation called on object with extant marshallings.");
	mmm(STG_E_DOCFILECORRUPT,"The docfile has been corrupted.");
	mmm(STG_E_BADBASEADDRESS,"OLE32.DLL has been loaded at the wrong address.");
	mmm(STG_E_INCOMPLETE,"The file download was aborted abnormally.  The file is incomplete.");
	mmm(STG_E_TERMINATED,"The file download has been terminated.");
	mmm(STG_S_CONVERTED,"The underlying file was converted to compound file format.");
	mmm(STG_S_BLOCK,"The storage operation should block until more data is available.");
	mmm(STG_S_RETRYNOW,"The storage operation should retry immediately.");
	mmm(STG_S_MONITORING,"The notified event sink will not influence the storage operation.");
	default:
		return UnkErrNumToString(hr);
	}
}

// Function: FacRPCErrToString
//    Convert all the FACILITY_RPC errors to strings.
const char *
ErrMgr::FacRPCErrToString(HRESULT hr)
{
	switch (hr) {
		//
		// Codes 0x0-0x11 are propogated from 16 bit OLE.
		//
	mmm(RPC_E_CALL_REJECTED,"Call was rejected by callee.");
	mmm(RPC_E_CALL_CANCELED,"Call was canceled by the message filter.");
	mmm(RPC_E_CANTPOST_INSENDCALL,"The caller is dispatching an intertask SendMessage call and");
	mmm(RPC_E_CANTCALLOUT_INASYNCCALL,"The caller is dispatching an asynchronous call and cannot make an outgoing call on behalf of this call.");
	mmm(RPC_E_CANTCALLOUT_INEXTERNALCALL,"It is illegal to call out while inside message filter.");
	mmm(RPC_E_CONNECTION_TERMINATED,"The connection terminated or is in a bogus state and cannot be used any more. Other connections are still valid.");
	mmm(RPC_E_SERVER_DIED,"The callee (server [not server application]) is not available and disappeared; all connections are invalid.  The call may have executed.");
	mmm(RPC_E_CLIENT_DIED,"The caller (client) disappeared while the callee (server) was processing a call.");
	mmm(RPC_E_INVALID_DATAPACKET,"The data packet with the marshalled parameter data is incorrect.");
	mmm(RPC_E_CANTTRANSMIT_CALL,"The call was not transmitted properly; the message queue was full and was not emptied after yielding.");
	mmm(RPC_E_CLIENT_CANTMARSHAL_DATA,"The client (caller) cannot marshall the parameter data - low memory, etc.");
	mmm(RPC_E_CLIENT_CANTUNMARSHAL_DATA,"The client (caller) cannot unmarshall the return data - low memory, etc.");
	mmm(RPC_E_SERVER_CANTMARSHAL_DATA,"The server (callee) cannot marshall the return data - low memory, etc.");
	mmm(RPC_E_SERVER_CANTUNMARSHAL_DATA,"The server (callee) cannot unmarshall the parameter data - low memory, etc.");
	mmm(RPC_E_INVALID_DATA,"Received data is invalid; could be server or client data.");
	mmm(RPC_E_INVALID_PARAMETER,"A particular parameter is invalid and cannot be (un)marshalled.");
	mmm(RPC_E_CANTCALLOUT_AGAIN,"There is no second outgoing call on same channel in DDE conversation.");
	mmm(RPC_E_SERVER_DIED_DNE,"The callee (server [not server application]) is not available and disappeared; all connections are invalid.  The call did not execute.");
	mmm(RPC_E_SYS_CALL_FAILED,"System call failed.");
	mmm(RPC_E_OUT_OF_RESOURCES,"Could not allocate some required resource (memory, events, ...)");
	mmm(RPC_E_ATTEMPTED_MULTITHREAD,"Attempted to make calls on more than one thread in single threaded mode.");
	mmm(RPC_E_NOT_REGISTERED,"The requested interface is not registered on the server object.");
	mmm(RPC_E_FAULT,"RPC could not call the server or could not return the results of calling the server.");
	mmm(RPC_E_SERVERFAULT,"The server threw an exception.");
	mmm(RPC_E_CHANGED_MODE,"Cannot change thread mode after it is set.");
	mmm(RPC_E_INVALIDMETHOD,"The method called does not exist on the server.");
	mmm(RPC_E_DISCONNECTED,"The object invoked has disconnected from its clients.");
	mmm(RPC_E_RETRY,"The object invoked chose not to process the call now.  Try again later.");
	mmm(RPC_E_SERVERCALL_RETRYLATER,"The message filter indicated that the application is busy.");
	mmm(RPC_E_SERVERCALL_REJECTED,"The message filter rejected the call.");
	mmm(RPC_E_INVALID_CALLDATA,"A call control interfaces was called with invalid data.");
	mmm(RPC_E_CANTCALLOUT_ININPUTSYNCCALL,"An outgoing call cannot be made since the application is dispatching an input-synchronous call.");
	mmm(RPC_E_WRONG_THREAD,"The application called an interface that was marshalled for a different thread.");
	mmm(RPC_E_THREAD_NOT_INIT,"CoInitialize has not been called on the current thread.");
	mmm(RPC_E_VERSION_MISMATCH,"The version of OLE on the client and server machines does not match.");
	mmm(RPC_E_INVALID_HEADER,"OLE received a packet with an invalid header.");
	mmm(RPC_E_INVALID_EXTENSION,"OLE received a packet with an invalid extension.");
	mmm(RPC_E_INVALID_IPID,"The requested object or interface does not exist.");
	mmm(RPC_E_INVALID_OBJECT,"The requested object does not exist.");
	mmm(RPC_S_CALLPENDING,"OLE has sent a request and is waiting for a reply.");
	mmm(RPC_S_WAITONTIMER,"OLE is waiting before retrying a request.");
	mmm(RPC_E_CALL_COMPLETE,"Call context cannot be accessed after call completed.");
	mmm(RPC_E_UNSECURE_CALL,"Impersonate on unsecure calls is not supported.");
	mmm(RPC_E_TOO_LATE,"Security must be initialized before any interfaces are marshalled or unmarshalled.  It cannot be changed once initialized.");
	mmm(RPC_E_NO_GOOD_SECURITY_PACKAGES,"No security packages are installed on this machine or the user is not logged on or there are no compatible security packages between the client and server.");
	mmm(RPC_E_ACCESS_DENIED,"Access is denied.");
	mmm(RPC_E_REMOTE_DISABLED,"Remote calls are not allowed for this process.");
	mmm(RPC_E_UNEXPECTED,"An internal error occurred.");
	default:
		return UnkErrNumToString(hr);
	}
}

// Function: FacSSPIErrToString
//    Convert all the FACILITY_SSPI errors to strings.
const char *
ErrMgr::FacSSPIErrToString(HRESULT hr)
{
	switch (hr) {
	mmm(NTE_BAD_UID,"Bad UID.");
	mmm(NTE_BAD_HASH,"Bad Hash.");
	mmm(NTE_BAD_KEY,"Bad Key.");
	mmm(NTE_BAD_LEN,"Bad Length.");
	mmm(NTE_BAD_DATA,"Bad Data.");
	mmm(NTE_BAD_SIGNATURE,"Invalid Signature.");
	mmm(NTE_BAD_VER,"Bad Version of provider.");
	mmm(NTE_BAD_ALGID,"Invalid algorithm specified.");
	mmm(NTE_BAD_FLAGS,"Invalid flags specified.");
	mmm(NTE_BAD_TYPE,"Invalid type specified.");
	mmm(NTE_BAD_KEY_STATE,"Key not valid for use in specified state.");
	mmm(NTE_BAD_HASH_STATE,"Hash not valid for use in specified state.");
	mmm(NTE_NO_KEY,"Key does not exist.");
	mmm(NTE_NO_MEMORY,"Insufficient memory available for the operation.");
	mmm(NTE_EXISTS,"Object already exists.");
	mmm(NTE_PERM,"Access denied.");
	mmm(NTE_NOT_FOUND,"Object was not found.");
	mmm(NTE_DOUBLE_ENCRYPT,"Data already encrypted.");
	mmm(NTE_BAD_PROVIDER,"Invalid provider specified.");
	mmm(NTE_BAD_PROV_TYPE,"Invalid provider type specified.");
	mmm(NTE_BAD_PUBLIC_KEY,"Provider's public key is invalid.");
	mmm(NTE_BAD_KEYSET,"Keyset does not exist");
	mmm(NTE_PROV_TYPE_NOT_DEF,"Provider type not defined.");
	mmm(NTE_PROV_TYPE_ENTRY_BAD,"Provider type as registered is invalid.");
	mmm(NTE_KEYSET_NOT_DEF,"The keyset is not defined.");
	mmm(NTE_KEYSET_ENTRY_BAD,"Keyset as registered is invalid.");
	mmm(NTE_PROV_TYPE_NO_MATCH,"Provider type does not match registered value.");
	mmm(NTE_SIGNATURE_FILE_BAD,"The digital signature file is corrupt.");
	mmm(NTE_PROVIDER_DLL_FAIL,"Provider DLL failed to initialize correctly.");
	mmm(NTE_PROV_DLL_NOT_FOUND,"Provider DLL could not be found.");
	mmm(NTE_BAD_KEYSET_PARAM,"The Keyset parameter is invalid.");
	mmm(NTE_FAIL,"An internal error occurred.");
	mmm(NTE_SYS_ERR,"A base error occurred.");
	default:
		return UnkErrNumToString(hr);
	}
}

// Function: FacWin32ErrToString
//    Convert all the FACILITY_WIN32 errors to strings.
const char *
ErrMgr::FacWin32ErrToString(HRESULT hr)
{
	switch (hr) {
	mmm(E_OUTOFMEMORY,"Ran out of memory");
	mmm(E_INVALIDARG,"One or more arguments are invalid");
	mmm(E_HANDLE,"Invalid handle");
	mmm(E_ACCESSDENIED,"General access denied error");
	default:
		return UnkErrNumToString(hr);
	}
}

// Function: FacControlErrToString
//    Convert all the FACILITY_CONTROL errors to strings.
const char *
ErrMgr::FacControlErrToString(HRESULT hr)
{
	return UnkErrNumToString(hr);
}

// Function: FacNullErrToString
//    Convert all the FACILITY_NULL errors to strings.
const char *
ErrMgr::FacNullErrToString(HRESULT hr)
{
	//
	// Codes 0x4000-0x40ff are reserved for OLE
	//
	switch (hr) {
//	case S_OK: 
//	case ERROR_SUCCESS: 
//	case NO_ERROR: 
	mmm(NOERROR,"No Error");
	mmm(S_FALSE,"False");
	mmm(E_UNEXPECTED,"Catastrophic failure");
	mmm(E_NOTIMPL,"Not implemented");
	mmm(E_NOINTERFACE,"No such interface supported");
	mmm(E_POINTER,"Invalid pointer");
	mmm(E_ABORT,"Operation aborted");
	mmm(E_FAIL,"Unspecified error");
	mmm(E_PENDING,"The data necessary to complete this operation is not yet available. ");
	mmm(CO_E_INIT_TLS,"Thread local storage failure");
	mmm(CO_E_INIT_SHARED_ALLOCATOR,"Get shared memory allocator failure");
	mmm(CO_E_INIT_MEMORY_ALLOCATOR,"Get memory allocator failure");
	mmm(CO_E_INIT_CLASS_CACHE,"Unable to initialize class cache");
	mmm(CO_E_INIT_RPC_CHANNEL,"Unable to initialize RPC services");
	mmm(CO_E_INIT_TLS_SET_CHANNEL_CONTROL,"Cannot set thread local storage channel control");
	mmm(CO_E_INIT_TLS_CHANNEL_CONTROL,"Could not allocate thread local storage channel control");
	mmm(CO_E_INIT_UNACCEPTED_USER_ALLOCATOR,"The user supplied memory allocator is unacceptable");
	mmm(CO_E_INIT_SCM_MUTEX_EXISTS,"The OLE service mutex already exists");
	mmm(CO_E_INIT_SCM_FILE_MAPPING_EXISTS,"The OLE service file mapping already exists");
	mmm(CO_E_INIT_SCM_MAP_VIEW_OF_FILE,"Unable to map view of file for OLE service");
	mmm(CO_E_INIT_SCM_EXEC_FAILURE,"Failure attempting to launch OLE service");
	mmm(CO_E_INIT_ONLY_SINGLE_THREADED,"There was an attempt to call CoInitialize a second time while single threaded");
	mmm(CO_E_CANT_REMOTE,"A Remote activation was necessary but was not allowed");
	mmm(CO_E_BAD_SERVER_NAME,"A Remote activation was necessary but the server name provided was invalid");
//	mmm(CO_E_RUNAS_INCOMPATIBLE,"The class is configured to run as a security id different from the caller");
	mmm(CO_E_WRONG_SERVER_IDENTITY,"The class is configured to run as a security id different from the caller");
	mmm(CO_E_OLE1DDE_DISABLED,"Use of Ole1 services requiring DDE windows is disabled");
	mmm(CO_E_RUNAS_SYNTAX,"A RunAs specification must be <domain name> <user name> or simply <user name>");
	mmm(CO_E_CREATEPROCESS_FAILURE,"The server process could not be started.  The pathname may be incorrect.");
	mmm(CO_E_RUNAS_CREATEPROCESS_FAILURE,"The server process could not be started as the configured identity.  The pathname may be incorrect or unavailable.");
	mmm(CO_E_RUNAS_LOGON_FAILURE,"The server process could not be started because the configured identity is incorrect.  Check the username and password.");
	mmm(CO_E_LAUNCH_PERMSSION_DENIED,"The client is not allowed to launch this server.");
	mmm(CO_E_START_SERVICE_FAILURE,"The service providing this server could not be started.");
	mmm(CO_E_REMOTE_COMMUNICATION_FAILURE,"This computer was unable to communicate with the computer providing the server.");
	mmm(CO_E_SERVER_START_TIMEOUT,"The server did not respond after being launched.");
	mmm(CO_E_CLSREG_INCONSISTENT,"The registration information for this server is inconsistent or incomplete.");
	mmm(CO_E_IIDREG_INCONSISTENT,"The registration information for this interface is inconsistent or incomplete.");//
	default:
		return UnkErrNumToString(hr);
	}
}

// Function: FacITFErrToString
//    Convert errors in the FACILITY_ITF to strings.  These are the
//  errors that are generally defined by the user, except for a number
//  that are reserved, by convention, for OLE.
const char *
ErrMgr::FacITFErrToString(HRESULT hr)
{
	int nCode = HRESULT_CODE(hr);
	//
	// Codes 0x0-0x01ff are reserved for the OLE group of interfaces.
	//
	if (nCode <= 0x01FF)
		return FacITFOLEErrToString(hr);

	// Codes 0xFF00 - 0xFFFF are reserved for the ErrMgr codes
	if (nCode >= 0xFF00)
		return FacITFResErrToString(hr);
	
	// the error is user defined...
	return "unspecified error";
}

const char *
ErrMgr::FacITFResErrToString(HRESULT hr)
{
	switch(hr) {
	mmm(E_GENERIC,"generic error");
	mmm(E_NULLPOINTER,"unexpected null pointer");
	mmm(S_GENERIC,"generic message");
	mmm(E_INVALIDOBJECT,"a pointer that was received is an invalid object");
	mmm(E_NOTINITIALIZED,"object not intialized");
	mmm(E_ALREADYINITIALIZED,"object already intialized");
	mmm(E_NOTFOUND,"object not found in specified place");
	mmm(E_INSUFFICIENTDATA,"there is insufficient data to complete the specified action");
	default: 
		return UnkErrNumToString(hr);
	}
}

// Function: FacDispatchErrToString
//    Convert all the FACILITY_DISPATCH errors to strings.
const char *
ErrMgr::FacDispatchErrToString(HRESULT hr)
{
	switch (hr) {
	mmm(DISP_E_UNKNOWNINTERFACE,"Unknown interface.");
	mmm(DISP_E_MEMBERNOTFOUND,"Member not found.");
	mmm(DISP_E_PARAMNOTFOUND,"Parameter not found.");
	mmm(DISP_E_TYPEMISMATCH,"Type mismatch.");
	mmm(DISP_E_UNKNOWNNAME,"Unknown name.");
	mmm(DISP_E_NONAMEDARGS,"No named arguments.");
	mmm(DISP_E_BADVARTYPE,"Bad variable type.");
	mmm(DISP_E_EXCEPTION,"Exception occurred.");
	mmm(DISP_E_OVERFLOW,"Out of present range.");
	mmm(DISP_E_BADINDEX,"Invalid index.");
	mmm(DISP_E_UNKNOWNLCID,"Unknown language.");
	mmm(DISP_E_ARRAYISLOCKED,"Memory is locked.");
	mmm(DISP_E_BADPARAMCOUNT,"Invalid number of parameters.");
	mmm(DISP_E_PARAMNOTOPTIONAL,"Parameter not optional.");
	mmm(DISP_E_BADCALLEE,"Invalid callee.");
	mmm(DISP_E_NOTACOLLECTION,"Does not support a collection.");
	mmm(TYPE_E_BUFFERTOOSMALL,"Buffer too small.");
	mmm(TYPE_E_INVDATAREAD,"Old format or invalid type library.");
	mmm(TYPE_E_UNSUPFORMAT,"Old format or invalid type library.");
	mmm(TYPE_E_REGISTRYACCESS,"Error accessing the OLE registry.");
	mmm(TYPE_E_LIBNOTREGISTERED,"Library not registered.");
	mmm(TYPE_E_UNDEFINEDTYPE,"Bound to unknown type.");
	mmm(TYPE_E_QUALIFIEDNAMEDISALLOWED,"Qualified name disallowed.");
	mmm(TYPE_E_INVALIDSTATE,"Invalid forward reference, or reference to uncompiled type.");
	mmm(TYPE_E_WRONGTYPEKIND,"Type mismatch.");
	mmm(TYPE_E_ELEMENTNOTFOUND,"Element not found.");
	mmm(TYPE_E_AMBIGUOUSNAME,"Ambiguous name.");
	mmm(TYPE_E_NAMECONFLICT,"Name already exists in the library.");
	mmm(TYPE_E_UNKNOWNLCID,"Unknown LCID.");
	mmm(TYPE_E_DLLFUNCTIONNOTFOUND,"Function not defined in specified DLL.");
	mmm(TYPE_E_BADMODULEKIND,"Wrong module kind for the operation.");
	mmm(TYPE_E_SIZETOOBIG,"Size may not exceed 64K.");
	mmm(TYPE_E_DUPLICATEID,"Duplicate ID in inheritance hierarchy.");
	mmm(TYPE_E_INVALIDID,"Incorrect inheritance depth in standard OLE hmember.");
	mmm(TYPE_E_TYPEMISMATCH,"Type mismatch.");
	mmm(TYPE_E_OUTOFBOUNDS,"Invalid number of arguments.");
	mmm(TYPE_E_IOERROR,"I/O Error.");
	mmm(TYPE_E_CANTCREATETMPFILE,"Error creating unique tmp file.");
	mmm(TYPE_E_CANTLOADLIBRARY,"Error loading type library/DLL.");
	mmm(TYPE_E_INCONSISTENTPROPFUNCS,"Inconsistent property functions.");
	mmm(TYPE_E_CIRCULARTYPE,"Circular dependency between types/modules.");
	default:
		return UnkErrNumToString(hr);
	}
}

// Function: FacCertErrToString
//    Convert all the FACILITY_CERT errors to strings.
const char *
ErrMgr::FacCertErrToString(HRESULT hr)
{
	switch (hr) {
	mmm(TRUST_E_PROVIDER_UNKNOWN,"The specified trust provider is not known on this system.");
	mmm(TRUST_E_ACTION_UNKNOWN,"The trust verification action specified is not supported by the specified trust provider.");
	mmm(TRUST_E_SUBJECT_FORM_UNKNOWN,"The form specified for the subject is not one supported or known by the specified trust provider.");
	mmm(TRUST_E_SUBJECT_NOT_TRUSTED,"The subject is not trusted for the specified action.");
	default:
		return UnkErrNumToString(hr);
	}
}

// Function: FacITFOLEErrToString
//    Convert all the FACILITY_ITF that are by convention used by OLE, to strings.
const char *
ErrMgr::FacITFOLEErrToString(HRESULT hr)
{
	switch (hr) {
		//
		// Generic OLE errors that may be returned by many inerfaces
		//
//		xxx(OLE_E_FIRST);
	mmm(OLE_E_OLEVERB,"Invalid OLEVERB structure");
		xxx(OLE_E_LAST);
//		xxx(OLE_S_FIRST);
	mmm(OLE_S_USEREG,"Use the registry database to provide the requested information");
		xxx(OLE_S_LAST);
		//
		// Old OLE errors
		//
	mmm(OLE_E_ADVF,"Invalid advise flags");
	mmm(OLE_E_ENUM_NOMORE,"Can't enumerate any more, because the associated data is missing");
	mmm(OLE_E_ADVISENOTSUPPORTED,"This implementation doesn't take advises");
	mmm(OLE_E_NOCONNECTION,"There is no connection for this connection ID");
	mmm(OLE_E_NOTRUNNING,"Need to run the object to perform this operation");
	mmm(OLE_E_NOCACHE,"There is no cache to operate on");
	mmm(OLE_E_BLANK,"Uninitialized object");
	mmm(OLE_E_CLASSDIFF,"Linked object's source class has changed");
	mmm(OLE_E_CANT_GETMONIKER,"Not able to get the moniker of the object");
	mmm(OLE_E_CANT_BINDTOSOURCE,"Not able to bind to the source");
	mmm(OLE_E_STATIC,"Object is static; operation not allowed");
	mmm(OLE_E_PROMPTSAVECANCELLED,"User cancelled out of save dialog");
	mmm(OLE_E_INVALIDRECT,"Invalid rectangle");
	mmm(OLE_E_WRONGCOMPOBJ,"compobj.dll is too old for the ole2.dll initialized");
	mmm(OLE_E_INVALIDHWND,"Invalid window handle");
	mmm(OLE_E_NOT_INPLACEACTIVE,"Object is not in any of the inplace active states");
	mmm(OLE_E_CANTCONVERT,"Not able to convert object");
	mmm(OLE_E_NOSTORAGE,"Not able to perform the operation because object is not given storage yet");
	mmm(DV_E_FORMATETC,"Invalid FORMATETC structure");
	mmm(DV_E_DVTARGETDEVICE,"Invalid DVTARGETDEVICE structure");
	mmm(DV_E_STGMEDIUM,"Invalid STDGMEDIUM structure");
	mmm(DV_E_STATDATA,"Invalid STATDATA structure");
	mmm(DV_E_LINDEX,"Invalid lindex");
	mmm(DV_E_TYMED,"Invalid tymed");
	mmm(DV_E_CLIPFORMAT,"Invalid clipboard format");
	mmm(DV_E_DVASPECT,"Invalid aspect(s)");
	mmm(DV_E_DVTARGETDEVICE_SIZE,"tdSize parameter of the DVTARGETDEVICE structure is invalid");
	mmm(DV_E_NOIVIEWOBJECT,"Object doesn't support IViewObject interface");
		xxx(DRAGDROP_E_LAST);
//		xxx(DRAGDROP_S_FIRST);
	mmm(DRAGDROP_S_DROP,"Successful drop took place");
		xxx(DRAGDROP_S_LAST);
	mmm(DRAGDROP_E_NOTREGISTERED,"Trying to revoke a drop target that has not been registered");
	mmm(DRAGDROP_E_ALREADYREGISTERED,"This window has already been registered as a drop target");
	mmm(DRAGDROP_E_INVALIDHWND,"Invalid window handle");
		xxx(CLASSFACTORY_E_LAST);
		xxx(CLASSFACTORY_S_FIRST);
		xxx(CLASSFACTORY_S_LAST);
	mmm(CLASS_E_NOAGGREGATION,"Class does not support aggregation (or class object is remote)");
	mmm(CLASS_E_CLASSNOTAVAILABLE,"ClassFactory cannot supply requested class");
		xxx(MARSHAL_E_LAST);
		xxx(MARSHAL_S_FIRST);	// success
		xxx(MARSHAL_S_LAST);	// success
		xxx(DATA_E_FIRST);
		xxx(DATA_E_LAST);
//		xxx(DATA_S_FIRST);	// success
	mmm(DATA_S_SAMEFORMATETC,"Data has same FORMATETC");
		xxx(DATA_S_LAST);	// success
//		xxx(VIEW_E_FIRST);
	mmm(VIEW_E_DRAW,"Error drawing view");
		xxx(VIEW_E_LAST);
//		xxx(VIEW_S_FIRST);	// success
	mmm(VIEW_S_ALREADY_FROZEN,"View is already frozen");
		xxx(VIEW_S_LAST);	// success
		xxx(REGDB_E_LAST);
		xxx(REGDB_S_FIRST);
		xxx(REGDB_S_LAST);
	mmm(REGDB_E_READREGDB,"Could not read key from registry");
	mmm(REGDB_E_WRITEREGDB,"Could not write key to registry");
	mmm(REGDB_E_KEYMISSING,"Could not find the key in the registry");
	mmm(REGDB_E_INVALIDVALUE,"Invalid value for registry");
	mmm(REGDB_E_CLASSNOTREG,"Class not registered");
	mmm(REGDB_E_IIDNOTREG,"Interface not registered");
		xxx(CACHE_E_LAST);
//		xxx(CACHE_S_FIRST);
	mmm(CACHE_S_FORMATETC_NOTSUPPORTED,"FORMATETC not supported");
		xxx(CACHE_S_LAST);
	mmm(CACHE_E_NOCACHE_UPDATED,"Cache not updated");
		xxx(OLEOBJ_E_LAST);
//		xxx(OLEOBJ_S_FIRST);
	mmm(OLEOBJ_S_INVALIDVERB,"Invalid verb for OLE object");
		xxx(OLEOBJ_S_LAST);
	mmm(OLEOBJ_E_NOVERBS,"No verbs for OLE object");
	mmm(OLEOBJ_E_INVALIDVERB,"Invalid verb for OLE object");
		xxx(CLIENTSITE_E_LAST);
		xxx(CLIENTSITE_S_FIRST);
		xxx(CLIENTSITE_S_LAST);
	mmm(INPLACE_E_NOTUNDOABLE,"Undo is not available");
	mmm(INPLACE_E_NOTOOLSPACE,"Space for tools is not available");
		xxx(INPLACE_E_LAST);
//		xxx(INPLACE_S_FIRST);
	mmm(INPLACE_S_TRUNCATED,"Message is too long; some of it had to be truncated before displaying");
		xxx(INPLACE_S_LAST);
		xxx(ENUM_E_FIRST);
		xxx(ENUM_E_LAST);
		xxx(ENUM_S_FIRST);
		xxx(ENUM_S_LAST);
//		xxx(CONVERT10_E_FIRST);
	mmm(CONVERT10_E_OLESTREAM_GET,"OLESTREAM Get method failed");
		xxx(CONVERT10_E_LAST);
//		xxx(CONVERT10_S_FIRST);
	mmm(CONVERT10_S_NO_PRESENTATION,"Unable to convert OLESTREAM to IStorage");
		xxx(CONVERT10_S_LAST);
	mmm(CONVERT10_E_OLESTREAM_PUT,"OLESTREAM Put method failed");
	mmm(CONVERT10_E_OLESTREAM_FMT,"Contents of the OLESTREAM not in correct format");
	mmm(CONVERT10_E_OLESTREAM_BITMAP_TO_DIB,"There was an error in a Windows GDI call while converting the bitmap to a DIB");
	mmm(CONVERT10_E_STG_FMT,"Contents of the IStorage not in correct format");
	mmm(CONVERT10_E_STG_NO_STD_STREAM,"Contents of IStorage is missing one of the standard streams");
	mmm(CONVERT10_E_STG_DIB_TO_BITMAP,"There was an error in a Windows GDI call while converting the DIB to a bitmap.");
//		xxx(CLIPBRD_E_FIRST);
	mmm(CLIPBRD_E_CANT_OPEN,"OpenClipboard Failed");
		xxx(CLIPBRD_E_LAST);
		xxx(CLIPBRD_S_FIRST);
		xxx(CLIPBRD_S_LAST);
	mmm(CLIPBRD_E_CANT_EMPTY,"EmptyClipboard Failed");
	mmm(CLIPBRD_E_CANT_SET,"SetClipboard Failed");
	mmm(CLIPBRD_E_BAD_DATA,"Data on clipboard is invalid");
	mmm(CLIPBRD_E_CANT_CLOSE,"CloseClipboard Failed");
//		xxx(MK_E_LAST);
	mmm(MK_E_ENUMERATION_FAILED,"Moniker could not be enumerated");
		xxx(MK_S_FIRST);
		xxx(MK_S_LAST);
	mmm(MK_E_CONNECTMANUALLY,"Moniker needs to be connected manually");
	mmm(MK_E_EXCEEDEDDEADLINE,"Operation exceeded deadline");
	mmm(MK_E_NEEDGENERIC,"Moniker needs to be generic");
	mmm(MK_E_UNAVAILABLE,"Operation unavailable");
	mmm(MK_E_SYNTAX,"Invalid syntax");
	mmm(MK_E_NOOBJECT,"No object for moniker");
	mmm(MK_E_INVALIDEXTENSION,"Bad extension for file");
	mmm(MK_E_INTERMEDIATEINTERFACENOTSUPPORTED,"Intermediate operation failed");
	mmm(MK_E_NOTBINDABLE,"Moniker is not bindable");
	mmm(MK_E_NOTBOUND,"Moniker is not bound");
	mmm(MK_E_CANTOPENFILE,"Moniker cannot open file");
	mmm(MK_E_MUSTBOTHERUSER,"User input required for operation to succeed");
	mmm(MK_E_NOINVERSE,"Moniker class has no inverse");
	mmm(MK_E_NOSTORAGE,"Moniker does not refer to storage");
	mmm(MK_E_NOPREFIX,"No common prefix");
//		xxx(CO_E_LAST);
	mmm(CO_E_RELEASED,"Object has been released");
		xxx(CO_S_FIRST);
		xxx(CO_S_LAST);
	mmm(CO_E_NOTINITIALIZED,"CoInitialize has not been called.");
	mmm(CO_E_ALREADYINITIALIZED,"CoInitialize has already been called.");
	mmm(CO_E_CANTDETERMINECLASS,"Class of object cannot be determined");
	mmm(CO_E_CLASSSTRING,"Invalid class string");
	mmm(CO_E_IIDSTRING,"Invalid interface string");
	mmm(CO_E_APPNOTFOUND,"Application not found");
	mmm(CO_E_APPSINGLEUSE,"Application cannot be run more than once");
	mmm(CO_E_ERRORINAPP,"Some error in application program");
	mmm(CO_E_DLLNOTFOUND,"DLL for class not found");
	mmm(CO_E_ERRORINDLL,"Error in the DLL");
	mmm(CO_E_WRONGOSFORAPP,"Wrong OS or OS version for application");
	mmm(CO_E_OBJNOTREG,"Object is not registered");
	mmm(CO_E_OBJISREG,"Object is already registered");
	mmm(CO_E_OBJNOTCONNECTED,"Object is not connected to server");
	mmm(CO_E_APPDIDNTREG,"Application was launched but it didn't register a class factory");
		//
		// Old OLE Success Codes
		//
	mmm(OLE_S_STATIC,"Success, but static");
	mmm(OLE_S_MAC_CLIPFORMAT,"Macintosh clipboard format");
	mmm(DRAGDROP_S_CANCEL,"Drag-drop operation canceled");
	mmm(DRAGDROP_S_USEDEFAULTCURSORS,"Use the default cursor");
	mmm(CACHE_S_SAMECACHE,"Same cache");
	mmm(CACHE_S_SOMECACHES_NOTUPDATED,"Some cache(s) not updated");
	mmm(OLEOBJ_S_CANNOT_DOVERB_NOW,"Verb number is valid but verb cannot be done now");
	mmm(OLEOBJ_S_INVALIDHWND,"Invalid window handle passed");
	mmm(MK_S_REDUCED_TO_SELF,"Moniker reduced to itself");
	mmm(MK_S_ME,"Common prefix is this moniker");
	mmm(MK_S_HIM,"Common prefix is input moniker");
	mmm(MK_S_US,"Common prefix is both monikers");
	mmm(MK_S_MONIKERALREADYREGISTERED,"Moniker is already registered in running object table");
	default:
		return UnkErrNumToString(hr);
	}
}

// Function: FacDDrawErrToString
//    Generates a string for a DirectDraw/Direct3D error message.
const char *
ErrMgr::FacDDrawErrToString(HRESULT hr)
{
    switch(hr) {
	case DD_OK:
//	mmm(D3D_OK,"No error.\0");
	mmm(DDERR_ALREADYINITIALIZED,"This object is already initialized.\0");
	mmm(DDERR_BLTFASTCANTCLIP,"Return if a clipper object is attached to the source surface passed into a BltFast call.\0");
	mmm(DDERR_CANNOTATTACHSURFACE,"This surface can not be attached to the requested surface.\0");
	mmm(DDERR_CANNOTDETACHSURFACE,"This surface can not be detached from the requested surface.\0");
	mmm(DDERR_CANTCREATEDC,"Windows can not create any more DCs.\0");
	mmm(DDERR_CANTDUPLICATE,"Can't duplicate primary & 3D surfaces, or surfaces that are implicitly created.\0");
	mmm(DDERR_CLIPPERISUSINGHWND,"An attempt was made to set a cliplist for a clipper object that is already monitoring an hwnd.\0");
	mmm(DDERR_COLORKEYNOTSET,"No src color key specified for this operation.\0");
	mmm(DDERR_CURRENTLYNOTAVAIL,"Support is currently not available.\0");
	mmm(DDERR_DIRECTDRAWALREADYCREATED,"A DirectDraw object representing this driver has already been created for this process.\0");
	mmm(DDERR_EXCEPTION,"An exception was encountered while performing the requested operation.\0");
	mmm(DDERR_EXCLUSIVEMODEALREADYSET,"An attempt was made to set the cooperative level when it was already set to exclusive.\0");
	mmm(DDERR_GENERIC,"Generic failure.\0");
	mmm(DDERR_HEIGHTALIGN,"Height of rectangle provided is not a multiple of reqd alignment.\0");
	mmm(DDERR_HWNDALREADYSET,"The CooperativeLevel HWND has already been set. It can not be reset while the process has surfaces or palettes created.\0");
	mmm(DDERR_HWNDSUBCLASSED,"HWND used by DirectDraw CooperativeLevel has been subclassed, this prevents DirectDraw from restoring state.\0");
	mmm(DDERR_IMPLICITLYCREATED,"This surface can not be restored because it is an implicitly created surface.\0");
	mmm(DDERR_INCOMPATIBLEPRIMARY,"Unable to match primary surface creation request with existing primary surface.\0");
	mmm(DDERR_INVALIDCAPS,"One or more of the caps bits passed to the callback are incorrect.\0");
	mmm(DDERR_INVALIDCLIPLIST,"DirectDraw does not support the provided cliplist.\0");
	mmm(DDERR_INVALIDDIRECTDRAWGUID,"The GUID passed to DirectDrawCreate is not a valid DirectDraw driver identifier.\0");
	mmm(DDERR_INVALIDMODE,"DirectDraw does not support the requested mode.\0");
	mmm(DDERR_INVALIDOBJECT,"DirectDraw received a pointer that was an invalid DIRECTDRAW object.\0");
	mmm(DDERR_INVALIDPARAMS,"One or more of the parameters passed to the function are incorrect.\0");
	mmm(DDERR_INVALIDPIXELFORMAT,"The pixel format was invalid as specified.\0");
	mmm(DDERR_INVALIDPOSITION,"Returned when the position of the overlay on the destination is no longer legal for that destination.\0");
	mmm(DDERR_INVALIDRECT,"Rectangle provided was invalid.\0");
	mmm(DDERR_LOCKEDSURFACES,"Operation could not be carried out because one or more surfaces are locked.\0");
	mmm(DDERR_NO3D,"There is no 3D present.\0");
	mmm(DDERR_NOALPHAHW,"Operation could not be carried out because there is no alpha accleration hardware present or available.\0");
	mmm(DDERR_NOBLTHW,"No blitter hardware present.\0");
	mmm(DDERR_NOCLIPLIST,"No cliplist available.\0");
	mmm(DDERR_NOCLIPPERATTACHED,"No clipper object attached to surface object.\0");
	mmm(DDERR_NOCOLORCONVHW,"Operation could not be carried out because there is no color conversion hardware present or available.\0");
	mmm(DDERR_NOCOLORKEY,"Surface doesn't currently have a color key\0");
	mmm(DDERR_NOCOLORKEYHW,"Operation could not be carried out because there is no hardware support of the destination color key.\0");
	mmm(DDERR_NOCOOPERATIVELEVELSET,"Create function called without DirectDraw object method SetCooperativeLevel being called.\0");
	mmm(DDERR_NODC,"No DC was ever created for this surface.\0");
	mmm(DDERR_NODDROPSHW,"No DirectDraw ROP hardware.\0");
	mmm(DDERR_NODIRECTDRAWHW,"A hardware-only DirectDraw object creation was attempted but the driver did not support any hardware.\0");
	mmm(DDERR_NOEMULATION,"Software emulation not available.\0");
	mmm(DDERR_NOEXCLUSIVEMODE,"Operation requires the application to have exclusive mode but the application does not have exclusive mode.\0");
	mmm(DDERR_NOFLIPHW,"Flipping visible surfaces is not supported.\0");
	mmm(DDERR_NOGDI,"There is no GDI present.\0");
	mmm(DDERR_NOHWND,"Clipper notification requires an HWND or no HWND has previously been set as the CooperativeLevel HWND.\0");
	mmm(DDERR_NOMIRRORHW,"Operation could not be carried out because there is no hardware present or available.\0");
	mmm(DDERR_NOOVERLAYDEST,"Returned when GetOverlayPosition is called on an overlay that UpdateOverlay has never been called on to establish a destination.\0");
	mmm(DDERR_NOOVERLAYHW,"Operation could not be carried out because there is no overlay hardware present or available.\0");
	mmm(DDERR_NOPALETTEATTACHED,"No palette object attached to this surface.\0");
	mmm(DDERR_NOPALETTEHW,"No hardware support for 16 or 256 color palettes.\0");
	mmm(DDERR_NORASTEROPHW,"Operation could not be carried out because there is no appropriate raster op hardware present or available.\0");
	mmm(DDERR_NOROTATIONHW,"Operation could not be carried out because there is no rotation hardware present or available.\0");
	mmm(DDERR_NOSTRETCHHW,"Operation could not be carried out because there is no hardware support for stretching.\0");
	mmm(DDERR_NOT4BITCOLOR,"DirectDrawSurface is not in 4 bit color palette and the requested operation requires 4 bit color palette.\0");
	mmm(DDERR_NOT4BITCOLORINDEX,"DirectDrawSurface is not in 4 bit color index palette and the requested operation requires 4 bit color index palette.\0");
	mmm(DDERR_NOT8BITCOLOR,"DirectDrawSurface is not in 8 bit color mode and the requested operation requires 8 bit color.\0");
	mmm(DDERR_NOTAOVERLAYSURFACE,"Returned when an overlay member is called for a non-overlay surface.\0");
	mmm(DDERR_NOTEXTUREHW,"Operation could not be carried out because there is no texture mapping hardware present or available.\0");
	mmm(DDERR_NOTFLIPPABLE,"An attempt has been made to flip a surface that is not flippable.\0");
	mmm(DDERR_NOTFOUND,"Requested item was not found.\0");
	mmm(DDERR_NOTLOCKED,"Surface was not locked.  An attempt to unlock a surface that was not locked at all, or by this process, has been attempted.\0");
	mmm(DDERR_NOTPALETTIZED,"The surface being used is not a palette-based surface.\0");
	mmm(DDERR_NOVSYNCHW,"Operation could not be carried out because there is no hardware support for vertical blank synchronized operations.\0");
	mmm(DDERR_NOZBUFFERHW,"Operation could not be carried out because there is no hardware support for zbuffer blitting.\0");
	mmm(DDERR_NOZOVERLAYHW,"Overlay surfaces could not be z layered based on their BltOrder because the hardware does not support z layering of overlays.\0");
	mmm(DDERR_OUTOFCAPS,"The hardware needed for the requested operation has already been allocated.\0");
	mmm(DDERR_OUTOFMEMORY,"DirectDraw does not have enough memory to perform the operation.\0");
	mmm(DDERR_OUTOFVIDEOMEMORY,"DirectDraw does not have enough memory to perform the operation.\0");
	mmm(DDERR_OVERLAYCANTCLIP,"The hardware does not support clipped overlays.\0");
	mmm(DDERR_OVERLAYCOLORKEYONLYONEACTIVE,"Can only have ony color key active at one time for overlays.\0");
	mmm(DDERR_OVERLAYNOTVISIBLE,"Returned when GetOverlayPosition is called on a hidden overlay.\0");
	mmm(DDERR_PALETTEBUSY,"Access to this palette is being refused because the palette is already locked by another thread.\0");
	mmm(DDERR_PRIMARYSURFACEALREADYEXISTS,"This process already has created a primary surface.\0");
	mmm(DDERR_REGIONTOOSMALL,"Region passed to Clipper::GetClipList is too small.\0");
	mmm(DDERR_SURFACEALREADYATTACHED,"This surface is already attached to the surface it is being attached to.\0");
	mmm(DDERR_SURFACEALREADYDEPENDENT,"This surface is already a dependency of the surface it is being made a dependency of.\0");
	mmm(DDERR_SURFACEBUSY,"Access to this surface is being refused because the surface is already locked by another thread.\0");
	mmm(DDERR_SURFACEISOBSCURED,"Access to surface refused because the surface is obscured.\0");
	mmm(DDERR_SURFACELOST,"Access to this surface is being refused because the surface memory is gone. The DirectDrawSurface object representing this surface should have Restore called on it.\0");
	mmm(DDERR_SURFACENOTATTACHED,"The requested surface is not attached.\0");
	mmm(DDERR_TOOBIGHEIGHT,"Height requested by DirectDraw is too large.\0");
	mmm(DDERR_TOOBIGSIZE,"Size requested by DirectDraw is too large, but the individual height and width are OK.\0");
	mmm(DDERR_TOOBIGWIDTH,"Width requested by DirectDraw is too large.\0");
	mmm(DDERR_UNSUPPORTED,"Action not supported.\0");
	mmm(DDERR_UNSUPPORTEDFORMAT,"FOURCC format requested is unsupported by DirectDraw.\0");
	mmm(DDERR_UNSUPPORTEDMASK,"Bitmask in the pixel format requested is unsupported by DirectDraw.\0");
	mmm(DDERR_VERTICALBLANKINPROGRESS,"Vertical blank is in progress.\0");
	mmm(DDERR_WASSTILLDRAWING,"Informs DirectDraw that the previous Blt which is transfering information to or from this Surface is incomplete.\0");
	mmm(DDERR_WRONGMODE,"This surface can not be restored because it was created in a different mode.\0");
	mmm(DDERR_XALIGN,"Rectangle provided was not horizontally aligned on required boundary.\0");
	mmm(D3DERR_BADMAJORVERSION,"bad DLL major version\0");
	mmm(D3DERR_BADMINORVERSION,"bad DLL minor version\0");
		xxx(D3DERR_EXECUTE_CREATE_FAILED);
		xxx(D3DERR_EXECUTE_DESTROY_FAILED);
		xxx(D3DERR_EXECUTE_LOCK_FAILED);
		xxx(D3DERR_EXECUTE_UNLOCK_FAILED);
		xxx(D3DERR_EXECUTE_LOCKED);
		xxx(D3DERR_EXECUTE_NOT_LOCKED);
		xxx(D3DERR_EXECUTE_FAILED);
		xxx(D3DERR_EXECUTE_CLIPPED_FAILED);
		xxx(D3DERR_TEXTURE_NO_SUPPORT);
		xxx(D3DERR_TEXTURE_CREATE_FAILED);
		xxx(D3DERR_TEXTURE_DESTROY_FAILED);
		xxx(D3DERR_TEXTURE_LOCK_FAILED);
		xxx(D3DERR_TEXTURE_UNLOCK_FAILED);
		xxx(D3DERR_TEXTURE_LOAD_FAILED);
		xxx(D3DERR_TEXTURE_SWAP_FAILED);
		xxx(D3DERR_TEXTURE_LOCKED);
		xxx(D3DERR_TEXTURE_NOT_LOCKED);
		xxx(D3DERR_TEXTURE_GETSURF_FAILED);
		xxx(D3DERR_MATRIX_CREATE_FAILED);
		xxx(D3DERR_MATRIX_DESTROY_FAILED);
		xxx(D3DERR_MATRIX_SETDATA_FAILED);
		xxx(D3DERR_MATRIX_GETDATA_FAILED);
		xxx(D3DERR_SETVIEWPORTDATA_FAILED);
		xxx(D3DERR_MATERIAL_CREATE_FAILED);
		xxx(D3DERR_MATERIAL_DESTROY_FAILED);
		xxx(D3DERR_MATERIAL_SETDATA_FAILED);
		xxx(D3DERR_MATERIAL_GETDATA_FAILED);
		xxx(D3DERR_LIGHT_SET_FAILED);
		xxx(D3DERR_SCENE_IN_SCENE);
		xxx(D3DERR_SCENE_NOT_IN_SCENE);
		xxx(D3DERR_SCENE_BEGIN_FAILED);
		xxx(D3DERR_SCENE_END_FAILED);
	mmm(D3DRMERR_BADOBJECT,"object expected in argument\0");
	mmm(D3DRMERR_BADTYPE,"bad argument type passed\0");
	mmm(D3DRMERR_BADALLOC,"out of memory\0");
	mmm(D3DRMERR_FACEUSED,"face already used in a mesh\0");
	mmm(D3DRMERR_NOTFOUND,"object not found in specified place\0");
	mmm(D3DRMERR_NOTDONEYET,"unimplemented\0");
	mmm(D3DRMERR_FILENOTFOUND,"file cannot be opened\0");
	mmm(D3DRMERR_BADFILE,"data file is corrupt\0");
	mmm(D3DRMERR_BADDEVICE,"device is not compatible with renderer\0");
	mmm(D3DRMERR_BADVALUE,"bad argument value\0");
	mmm(D3DRMERR_BADMAJORVERSION,"bad DLL major version\0");
	mmm(D3DRMERR_BADMINORVERSION,"bad DLL minor version\0");
	mmm(D3DRMERR_UNABLETOEXECUTE,"unable to execute\0");
	default:
		return UnkErrNumToString(hr);
	}	
}

#undef xxx
#undef mmm