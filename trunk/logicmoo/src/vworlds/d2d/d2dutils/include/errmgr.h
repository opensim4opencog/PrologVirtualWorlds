// Copyright © 2000 Microsoft Corporation.  All rights reserved.
// In installing/viewing this source code, you agree to the terms of the
// Microsoft Research Source License (MSRSL) included in the root of this source tree
// and available from http://www.vworlds.org/license.asp.

#ifndef _ErrMgr_H
#define _ErrMgr_H

// File:	ErrMgr.h
// Author:	MM    (MM)
//
// Description:
//    This module implements an error manager class that, in theory,
//  makes common error handling tasks easier.  There are basically
//  three main features: operator= is overloaded for convenience, 
//  practically all known errors can be turned into sensible strings
//  in english, and error handling can be accomplished thru a callback
//  mechanism.  Error codes: 
//		0x8004FF00 - 0x8004FFFF and 0x0004FF00-0x0004FFFF 
//  are reserved by the ErrMgr for special codes.
// 
// History:
// -@- 7/14/96 (MM) created
// -@- 9/11/96 (MM) modified
//    i changed the code so that it complies with OLE style error
//  handling -- now everything is based on HRESULTS
// -@- 9/12/96 (MM) modified
//    the source now contains an enormous database of error messages,
//  including the DirectDraw/Direct3D errors -- no more wondering what
//  that pesky error code really means
// -@- 9/12/96 (MM) modified
//    reserved certain HRESULTS for use by ErrMgr
// -@- 9/17/96 (MM) modified
//    fixed the variable length argument list bug

#ifndef _MMUtils_h
#include "MMUtils.h"
#endif

// the OLE error handling is good enough and prevalent...
#ifndef HRESULT
typedef long HRESULT;
#endif
#ifndef _WINERROR_
#include <winerror.h>
#endif

//
// 9/11/96 (MM) - snarfed from winerror.h:
//
//  HRESULTs are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +-+-+-+-+-+---------------------+-------------------------------+
//  |S|R|C|N|r|    Facility         |               Code            |
//  +-+-+-+-+-+---------------------+-------------------------------+
//
//  where
//
//      S - Severity - indicates success/fail
//
//          0 - Success
//          1 - Fail (COERROR)
//
//      R - reserved portion of the facility code, corresponds to NT's
//              second severity bit.
//
//      C - reserved portion of the facility code, corresponds to NT's
//              C field.
//
//      N - reserved portion of the facility code. Used to indicate a
//              mapped NT status value.
//
//      r - reserved portion of the facility code. Reserved for internal
//              use. Used to indicate HRESULT values that are not status
//              values, but are instead message ids for display strings.
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//

// From the HELP files:
//
// -    Status codes in facilities other than FACILITY_ITF can only be defined 
//      by Microsoft.
// -    Status codes in facility FACILITY_ITF are defined solely by the 
//      developer of the interface or API that returns the status code.
//      To avoid conflicting error codes, whoever defines the interface
//      is responsible for coordinating and publishing the FACILITY_ITF
//      status codes associated with that interface.
//  
// All the OLE-defined FACILITY_ITF codes have a code value in the range
// of 0x0000 - 0x01FF. While it is legal to use any codes in FACILITY_ITF,
// it is recommended that only code values in the range of 0x0200 - 0xFFFF
// be used. This recommendation is made as a means of reducing confusion
// with any OLE-defined errors.

class ErrMgr;

//
// TYPEDEFS
//
typedef const char *(*HRToStringFunc)(HRESULT);
typedef HRESULT (*FailureCallback)(HRESULT, const char *, va_list);
//
// MACROS
//
#define MAKE_USERERROR(code) MAKE_HRESULT(1,FACILITY_ITF,code)
#define MAKE_USERMSG(code) MAKE_HRESULT(0,FACILITY_ITF,code)
//
// RESERVED HRESULT's
// 
#define S_GENERIC				MAKE_USERMSG(0xFFFF)
#define S_MESSAGE				S_GENERIC

#define E_GENERIC				MAKE_USERERROR(0xFFFF)
#define E_NULLPOINTER			MAKE_USERERROR(0xFFFE)
#define E_INVALIDOBJECT			MAKE_USERERROR(0xFFFD)
#define E_NOTINITIALIZED		MAKE_USERERROR(0xFFFC)
#define E_ALREADYINITIALIZED	MAKE_USERERROR(0xFFFB)
#define E_NOTFOUND				MAKE_USERERROR(0xFFFA)
#define E_INSUFFICIENTDATA		MAKE_USERERROR(0xFFF9)


//
// OTHER MACROS
//
#ifndef ERRMGR_NAME
#define ERRMGR_NAME g_emErr
#endif

// Macro: MMCHECK_MEM
//    Verify memory was actually allocated to pointer
#define MMCHECK_MEM(p) MMCERTIFY(p)

#define MMCHECK_ERRMSG(__hr, __szMsg) MACSTART (ERRMGR_NAME).CheckError(__hr, (__szMsg)); MACEND

// Macro: _MEMERR
//    Check for a memory error, and report to error manager if necessary
#define MMCHECK_MEMERR(__p) MACSTART if ((__p) == NULL) (ERRMGR_NAME).ThrowError(E_OUTOFMEMORY, #__p); MACEND
#define MMPROP_MEMERR(__p) MACSTART if ((__p) == NULL) return E_OUTOFMEMORY; MACEND
#define MMCPROP_MEMERR(__p) MACSTART if ((__p) == NULL) return (ERRMGR_NAME).ThrowError(E_OUTOFMEMORY, #__p); MACEND

// Macro: MMPROP_FALSE
//    Checks if "exp" evaluates to FALSE, and if so, propagates FALSE 
//  up the execution stack.
#define MMPROP_FALSE(exp) MACSTART if ((exp) == FALSE) return FALSE; MACEND

// Macro: _ERR
//    Propagate errors up the program stack
#define MMPROP_ERR(__hr) MACSTART if (FAILED(g_hrErr = (__hr))) return g_hrErr; MACEND
#define MMCPROP_ERR(__p) MACSTART if (FAILED(g_hrErr = (__hr))) return (ERRMGR_NAME).ThrowError(g_hrErr); MACEND

// Macro: MMPROP_NULLERR
//    Checks if "exp" evaluates to NULL, and if so, reports an error, and
//  propagates ITF_E_NULLPOINTER up the execution stack.
#define MMCPROP_NULLERR(__exp) MACSTART \
	if ((__exp) == NULL) return (ERRMGR_NAME).ThrowError(ITF_E_NULLPOINTER, #__exp); MACEND


HRESULT DefaultFailureCallback(HRESULT hr, const char *szErrMsg, va_list pMsgArgs);

// Class: ErrMgr
// ***Hungarian Notation: em
class ErrMgr {
public:
				ErrMgr(FailureCallback pfnFailureCallback = DefaultFailureCallback);
				~ErrMgr() {}
	
				operator HRESULT() { return m_hrLast; }
	HRESULT			operator=(HRESULT hr);

	HRESULT			ErrToString(HRESULT hr, char *pMsgBuf, int nMsgBufSize, const char *szMsgFmt = NULL, ...);
	HRESULT			CheckError(HRESULT hr, const char *szMsgFmt = NULL, ...);
	HRESULT			GenericError(const char *szFmt = NULL, ...);
	HRESULT			ThrowError(HRESULT hr, const char *szMsgFmt = NULL, ...);

	//
	// Error String Generators
	//
	static int			ComposeGenericString(HRESULT hr, char *pErrBuf, int nBufSize, const char *szMsgFmt, va_list pArgs);
	static void			GetSubStrings(HRESULT hr, const char **pszFacName,
								const char **pszMsgType, const char **pszSysErr);
	static int			ComposeSubStrings(char *pErrBuf, int nBufSize, const char *szFacName,
								const char *szMsgType, const char *szSysErr,
								const char *szMsgFmt, va_list pArgs);
	static const char *	UnkErrNumToString(HRESULT hr);

	//
	// Data Manipulation Functions
	//
	void			SetFailureCallback(FailureCallback pfnFailureCallback) { MMVERIFY(m_pfnFailureCallback = pfnFailureCallback); }

private:
	static const char *FacWindowsErrToString(HRESULT hr);
	static const char *FacStorageErrToString(HRESULT hr);
	static const char *FacRPCErrToString(HRESULT hr);
	static const char *FacSSPIErrToString(HRESULT hr);
	static const char *FacWin32ErrToString(HRESULT hr);
	static const char *FacControlErrToString(HRESULT hr);
	static const char *FacNullErrToString(HRESULT hr);
	static const char *FacITFErrToString(HRESULT hr);
	static const char *FacDispatchErrToString(HRESULT hr);
	static const char *FacCertErrToString(HRESULT hr);
	static const char *FacUnknownErrToString(HRESULT hr);
	static const char *FacDDrawErrToString(HRESULT hr);
	static const char *FacITFOLEErrToString(HRESULT hr);
	static const char *FacITFResErrToString(HRESULT hr);

private:
	HRESULT			m_hrLast;
	FailureCallback	m_pfnFailureCallback;
};


//
// GLOBALS
//
extern ErrMgr g_emErr;
extern HRESULT g_hrErr;

//
// INLINE FUNCTIONS
//

// Function: ErrToString
//    This function can be used by the application to generate an error string,
//  without going indirectly thru the callback mechanism.
inline HRESULT
ErrMgr::ErrToString(HRESULT hr, char *pMsgBuf, int nMsgBufSize, const char *szMsgFmt, ...)
{
	ComposeGenericString(hr, pMsgBuf, nMsgBufSize, szMsgFmt, (va_list)(&szMsgFmt+1));
	return hr; 
}

inline HRESULT
ErrMgr::operator=(HRESULT hr)
{
	if (FAILED(hr))
		return (m_hrLast = m_pfnFailureCallback(hr, NULL, NULL));
	return (m_hrLast = hr);
}

// Function: ThrowError
//    directly cause an error to be thrown
inline HRESULT
ErrMgr::ThrowError(HRESULT hr, const char *szMsgFmt, ...)
{
	return (m_hrLast = m_pfnFailureCallback(hr, szMsgFmt, (va_list)(&szMsgFmt+1)));
}

inline HRESULT
ErrMgr::CheckError(HRESULT hr, const char *szMsgFmt, ...)
{
	if (FAILED(hr))
		return (m_hrLast = m_pfnFailureCallback(hr, szMsgFmt, (va_list)(&szMsgFmt+1)));
	return (m_hrLast = hr);
}

// Function: GenericError
//    Generate an output string, without error code information.
inline HRESULT
ErrMgr::GenericError(const char *szMsgFmt, ...)
{
	return (m_hrLast = m_pfnFailureCallback(E_GENERIC, szMsgFmt, (va_list)(&szMsgFmt+1)));
}

#endif
