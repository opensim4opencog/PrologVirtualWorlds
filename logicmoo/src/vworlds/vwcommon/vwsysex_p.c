/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:35 2000
 */
/* Compiler settings for vwsysex.idl:
    Oicf (OptLev=i2), W0, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 440
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "vwsysex.h"

#define TYPE_FORMAT_STRING_SIZE   1265                              
#define PROC_FORMAT_STRING_SIZE   6871                              

typedef struct _MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MIDL_PROC_FORMAT_STRING;


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


/* Standard interface: __MIDL_itf_vwsysex_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IVWDispatch, ver. 0.0,
   GUID={0x590D9858,0xA417,0x11CF,{0x9B,0xF0,0x00,0x80,0xC7,0xA5,0x6A,0x8A}} */


/* Object interface: ISystemExemplar, ver. 0.0,
   GUID={0x717B5A86,0x7267,0x11d1,{0x95,0x9F,0x00,0xC0,0x4F,0xD9,0x1F,0x4D}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ISystemExemplar_ServerInfo;

#pragma code_seg(".orpc")
/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_OnGlobalConstantAccess_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lPerms,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,bReturnValue);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[746],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[746],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&lPerms,
                  ( unsigned char __RPC_FAR * )&bReturnValue);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[746],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TraceExemplarInfo_Proxy( 
    ISystemExemplar __RPC_FAR * This)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,This);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[780],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[780],
                  ( unsigned char __RPC_FAR * )&This);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[780],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeGetProperty_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrPropertyName,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[802],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[802],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pthing,
                  ( unsigned char __RPC_FAR * )&bstrPropertyName,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[802],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimePutProperty_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrPropertyName,
    /* [in] */ VARIANT varData,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[848],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[848],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pthing,
                  ( unsigned char __RPC_FAR * )&bstrPropertyName,
                  ( unsigned char __RPC_FAR * )&varData,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[848],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeInvokeMethod_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrMethodName,
    /* [in] */ VARIANT varArgs,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[900],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[900],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pthing,
                  ( unsigned char __RPC_FAR * )&bstrMethodName,
                  ( unsigned char __RPC_FAR * )&varArgs,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[900],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeGetMethod_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrMethodName,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[952],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[952],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pthing,
                  ( unsigned char __RPC_FAR * )&bstrMethodName,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[952],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeGetPropertySecurity_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrPropertyName,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[998],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[998],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pthing,
                  ( unsigned char __RPC_FAR * )&bstrPropertyName,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[998],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeCheckPropertySecurity_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrPropertyName,
    /* [in] */ unsigned long ulFlags,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1044],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1044],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pthing,
                  ( unsigned char __RPC_FAR * )&bstrPropertyName,
                  ( unsigned char __RPC_FAR * )&ulFlags,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1044],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeCheckMethodSecurity_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrMethodName,
    /* [in] */ unsigned long ulFlags,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1096],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1096],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pthing,
                  ( unsigned char __RPC_FAR * )&bstrMethodName,
                  ( unsigned char __RPC_FAR * )&ulFlags,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1096],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeCheckPropertiesSecurity_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ unsigned long ulFlags,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1148],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1148],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pthing,
                  ( unsigned char __RPC_FAR * )&ulFlags,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1148],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeCheckMethodsSecurity_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ unsigned long ulFlags,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1194],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1194],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pthing,
                  ( unsigned char __RPC_FAR * )&ulFlags,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1194],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeVTableInvokeMethod_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1240],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1240],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1240],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeAutomationInvokeMethod_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1274],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1274],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1274],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeScriptInvokeMethod_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1308],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1308],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1308],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeInterfaceInvokeMethod_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrMethodName,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1342],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1342],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pthing,
                  ( unsigned char __RPC_FAR * )&bstrMethodName,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1342],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeNotifyInvokeMethod_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrMethodName,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1388],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1388],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pthing,
                  ( unsigned char __RPC_FAR * )&bstrMethodName,
                  ( unsigned char __RPC_FAR * )&pdispparams,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1388],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeNotifyPropertyChanged_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrPropertyName,
    /* [in] */ long lHint,
    /* [in] */ VARIANT varHintData,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1440],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1440],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pthing,
                  ( unsigned char __RPC_FAR * )&bstrPropertyName,
                  ( unsigned char __RPC_FAR * )&lHint,
                  ( unsigned char __RPC_FAR * )&varHintData,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1440],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeSendThingCommand_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lFlags,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ DISPID dispid,
    /* [in] */ WORD wFlags,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1498],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1498],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&lFlags,
                  ( unsigned char __RPC_FAR * )&pthing,
                  ( unsigned char __RPC_FAR * )&dispid,
                  ( unsigned char __RPC_FAR * )&wFlags,
                  ( unsigned char __RPC_FAR * )&pdispparams,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1498],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeSendObjectPropertyCommand_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lFlags,
    /* [in] */ IObjectProperty __RPC_FAR *pprop,
    /* [in] */ DISPID dispid,
    /* [in] */ WORD wFlags,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1562],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1562],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&lFlags,
                  ( unsigned char __RPC_FAR * )&pprop,
                  ( unsigned char __RPC_FAR * )&dispid,
                  ( unsigned char __RPC_FAR * )&wFlags,
                  ( unsigned char __RPC_FAR * )&pdispparams,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1562],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeSendWorldCommand_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ DISPID dispid,
    /* [in] */ WORD wFlags,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1626],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1626],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&dispid,
                  ( unsigned char __RPC_FAR * )&wFlags,
                  ( unsigned char __RPC_FAR * )&pdispparams,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1626],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeProcessMessage_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ unsigned char ch,
    /* [in] */ IUnMarshallBuffer __RPC_FAR *pUbuffer,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1678],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1678],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&ch,
                  ( unsigned char __RPC_FAR * )&pUbuffer,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1678],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeCloneProperty_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ VARIANT varData,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1724],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1724],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&varData,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1724],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeCopyProperty_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ VARIANT varData,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1764],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1764],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&varData,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1764],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimePutPropertySecurity_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IPropertySecurity __RPC_FAR *pSecurity,
    /* [in] */ VARIANT varData,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1804],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1804],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pSecurity,
                  ( unsigned char __RPC_FAR * )&varData,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1804],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeGetPropertyAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ VARIANT varData,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1850],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1850],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&varData,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1850],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimePutPropertyAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ VARIANT varData,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1890],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1890],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&varData,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1890],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeInvokeMethodAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1930],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1930],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1930],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeGetMethodAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1964],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1964],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1964],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeGetPropertySecurityAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1998],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1998],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[1998],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeCheckPropertySecurityAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ unsigned long ulFlags,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2032],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2032],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&ulFlags,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2032],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeCheckMethodSecurityAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ unsigned long ulFlags,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2072],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2072],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&ulFlags,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2072],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeCheckPropertiesSecurityAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ unsigned long ulFlags,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2112],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2112],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&ulFlags,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2112],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeCheckMethodsSecurityAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ unsigned long ulFlags,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2152],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2152],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&ulFlags,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2152],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeNotifyAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2192],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2192],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2192],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeSendCommandAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2226],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2226],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2226],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeProcessMessageAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2260],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2260],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2260],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,plElapsed);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2294],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2294],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&lRepeat,
                  ( unsigned char __RPC_FAR * )&plElapsed);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2294],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_YYY_Proxy( 
    ISystemExemplar __RPC_FAR * This)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,This);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2328],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2328],
                  ( unsigned char __RPC_FAR * )&This);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2328],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_OnXXXChanged_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lHint,
    /* [in] */ VARIANT varHintData)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,varHintData);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2350],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2350],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&lHint,
                  ( unsigned char __RPC_FAR * )&varHintData);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2350],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_OnXXXAccess_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lPerms,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,bReturnValue);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2384],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2384],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&lPerms,
                  ( unsigned char __RPC_FAR * )&bReturnValue);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[2384],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

static const unsigned short ISystemExemplar_FormatStringOffsetTable[] = 
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    40,
    74,
    102,
    166,
    200,
    234,
    268,
    314,
    360,
    406,
    452,
    486,
    526,
    554,
    582,
    634,
    662,
    690,
    712,
    746,
    780,
    802,
    848,
    900,
    952,
    998,
    1044,
    1096,
    1148,
    1194,
    1240,
    1274,
    1308,
    1342,
    1388,
    1440,
    1498,
    1562,
    1626,
    1678,
    1724,
    1764,
    1804,
    1850,
    1890,
    1930,
    1964,
    1998,
    2032,
    2072,
    2112,
    2152,
    2192,
    2226,
    2260,
    2294,
    2328,
    2350,
    2384
    };

static const MIDL_SERVER_INFO ISystemExemplar_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &ISystemExemplar_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO ISystemExemplar_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &ISystemExemplar_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(72) _ISystemExemplarProxyVtbl = 
{
    &ISystemExemplar_ProxyInfo,
    &IID_ISystemExemplar,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* (void *)-1 /* IVWDispatch::Initialize */ ,
    0 /* (void *)-1 /* IVWDispatch::Terminate */ ,
    0 /* (void *)-1 /* IVWDispatch::Install */ ,
    0 /* (void *)-1 /* IVWDispatch::OnLoad */ ,
    0 /* (void *)-1 /* IVWDispatch::OnUnload */ ,
    (void *)-1 /* ISystemExemplar::CreatePropertyList */ ,
    (void *)-1 /* ISystemExemplar::CreatePropertyMap */ ,
    (void *)-1 /* ISystemExemplar::CreateMenu */ ,
    (void *)-1 /* ISystemExemplar::CreateMenuItem */ ,
    (void *)-1 /* ISystemExemplar::CreateMenuSeparator */ ,
    (void *)-1 /* ISystemExemplar::CreateOCXProxy */ ,
    (void *)-1 /* ISystemExemplar::QueryAvatarNames */ ,
    (void *)-1 /* ISystemExemplar::InvokeRemoteClientMethod */ ,
    (void *)-1 /* ISystemExemplar::HandleRemoteClientMethod */ ,
    (void *)-1 /* ISystemExemplar::FireUIEvent */ ,
    (void *)-1 /* ISystemExemplar::FireRemoteUIEvent */ ,
    (void *)-1 /* ISystemExemplar::OnRootURLChanged */ ,
    (void *)-1 /* ISystemExemplar::UploadFile */ ,
    (void *)-1 /* ISystemExemplar::MakeHost */ ,
    (void *)-1 /* ISystemExemplar::UnMakeHost */ ,
    (void *)-1 /* ISystemExemplar::CreateTimer */ ,
    (void *)-1 /* ISystemExemplar::DestroyTimer */ ,
    (void *)-1 /* ISystemExemplar::OnCreateRoomClient */ ,
    (void *)-1 /* ISystemExemplar::ShowWelcomeMessage */ ,
    (void *)-1 /* ISystemExemplar::OnWorldOwnerAccess */ ,
    ISystemExemplar_OnGlobalConstantAccess_Proxy ,
    ISystemExemplar_TraceExemplarInfo_Proxy ,
    ISystemExemplar_TimeGetProperty_Proxy ,
    ISystemExemplar_TimePutProperty_Proxy ,
    ISystemExemplar_TimeInvokeMethod_Proxy ,
    ISystemExemplar_TimeGetMethod_Proxy ,
    ISystemExemplar_TimeGetPropertySecurity_Proxy ,
    ISystemExemplar_TimeCheckPropertySecurity_Proxy ,
    ISystemExemplar_TimeCheckMethodSecurity_Proxy ,
    ISystemExemplar_TimeCheckPropertiesSecurity_Proxy ,
    ISystemExemplar_TimeCheckMethodsSecurity_Proxy ,
    ISystemExemplar_TimeVTableInvokeMethod_Proxy ,
    ISystemExemplar_TimeAutomationInvokeMethod_Proxy ,
    ISystemExemplar_TimeScriptInvokeMethod_Proxy ,
    ISystemExemplar_TimeInterfaceInvokeMethod_Proxy ,
    ISystemExemplar_TimeNotifyInvokeMethod_Proxy ,
    ISystemExemplar_TimeNotifyPropertyChanged_Proxy ,
    ISystemExemplar_TimeSendThingCommand_Proxy ,
    ISystemExemplar_TimeSendObjectPropertyCommand_Proxy ,
    ISystemExemplar_TimeSendWorldCommand_Proxy ,
    ISystemExemplar_TimeProcessMessage_Proxy ,
    ISystemExemplar_TimeCloneProperty_Proxy ,
    ISystemExemplar_TimeCopyProperty_Proxy ,
    ISystemExemplar_TimePutPropertySecurity_Proxy ,
    ISystemExemplar_TimeGetPropertyAll_Proxy ,
    ISystemExemplar_TimePutPropertyAll_Proxy ,
    ISystemExemplar_TimeInvokeMethodAll_Proxy ,
    ISystemExemplar_TimeGetMethodAll_Proxy ,
    ISystemExemplar_TimeGetPropertySecurityAll_Proxy ,
    ISystemExemplar_TimeCheckPropertySecurityAll_Proxy ,
    ISystemExemplar_TimeCheckMethodSecurityAll_Proxy ,
    ISystemExemplar_TimeCheckPropertiesSecurityAll_Proxy ,
    ISystemExemplar_TimeCheckMethodsSecurityAll_Proxy ,
    ISystemExemplar_TimeNotifyAll_Proxy ,
    ISystemExemplar_TimeSendCommandAll_Proxy ,
    ISystemExemplar_TimeProcessMessageAll_Proxy ,
    ISystemExemplar_TimeAll_Proxy ,
    ISystemExemplar_YYY_Proxy ,
    ISystemExemplar_OnXXXChanged_Proxy ,
    ISystemExemplar_OnXXXAccess_Proxy
};


static const PRPC_STUB_FUNCTION ISystemExemplar_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _ISystemExemplarStubVtbl =
{
    &IID_ISystemExemplar,
    &ISystemExemplar_ServerInfo,
    72,
    &ISystemExemplar_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_vwsysex_0257, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IThingExemplar, ver. 0.0,
   GUID={0x99AD3618,0xAAB5,0x11D0,{0xBA,0xE3,0x00,0xC0,0x4F,0xC2,0xCA,0xB1}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IThingExemplar_ServerInfo;

#pragma code_seg(".orpc")
/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_UpdatePropertiesMenuItem_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IMenuItem __RPC_FAR *pmi)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pmi);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3032],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3032],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pmi);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3032],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_UpdateMenuItemName_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IMenuItem __RPC_FAR *pmi)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pmi);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3060],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3060],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pmi);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3060],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_UpdatePropertyListMenu_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IMenuItem __RPC_FAR *pmi,
    /* [in] */ IPropertyList __RPC_FAR *pList)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pList);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3088],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3088],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pmi,
                  ( unsigned char __RPC_FAR * )&pList);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3088],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_UpdateInventoryMenu_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IMenuItem __RPC_FAR *pmi)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pmi);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3122],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3122],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pmi);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3122],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_UpdateContentsMenu_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IMenuItem __RPC_FAR *pmi)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pmi);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3150],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3150],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pmi);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3150],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_UpdateRoomAvatarsMenu_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IMenuItem __RPC_FAR *pmi)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pmi);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3178],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3178],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pmi);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3178],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_DoGesture_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ int iGesture)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,iGesture);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3206],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3206],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&iGesture);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3206],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_DoNamedGesture_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrGestureName)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,bstrGestureName);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3234],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3234],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&bstrGestureName);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3234],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_DoGestureServer_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThingOriginator,
    /* [in] */ int iGesture)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,iGesture);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3262],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3262],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pThingOriginator,
                  ( unsigned char __RPC_FAR * )&iGesture);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3262],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_DoGestureClient_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThingOriginator,
    /* [in] */ int iGesture)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,iGesture);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3296],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3296],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pThingOriginator,
                  ( unsigned char __RPC_FAR * )&iGesture);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3296],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_AllowEnter_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pbool);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3330],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3330],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pThing,
                  ( unsigned char __RPC_FAR * )&pbool);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3330],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_AllowLeave_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pbool);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3364],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3364],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pThing,
                  ( unsigned char __RPC_FAR * )&pbool);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3364],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_AcceptEnter_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pbool);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3398],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3398],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pThing,
                  ( unsigned char __RPC_FAR * )&pbool);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3398],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_AcceptLeave_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pbool);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3432],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3432],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pbool);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3432],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_Clone_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppNewThing)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,ppNewThing);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3460],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3460],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&ppNewThing);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3460],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_Emote_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ VARIANT varGesture,
    /* [optional][in] */ VARIANT varText)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,varText);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3488],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3488],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&varGesture,
                  ( unsigned char __RPC_FAR * )&varText);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3488],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_GestureText_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrGesture,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrText)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pbstrText);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3522],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3522],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&bstrGesture,
                  ( unsigned char __RPC_FAR * )&pbstrText);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3522],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_MoveInto_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pNewContainer)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pNewContainer);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3556],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3556],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pNewContainer);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3556],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_Say_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ BSTR strText,
    /* [optional][in] */ VARIANT userOrList)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,userOrList);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3584],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3584],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&strText,
                  ( unsigned char __RPC_FAR * )&userOrList);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3584],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_Shout_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ BSTR strText)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,strText);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3618],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3618],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&strText);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3618],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_SayGesture_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ BSTR strText)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,strText);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3646],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3646],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&strText);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3646],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_Whisper_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ BSTR strText,
    /* [optional][in] */ VARIANT userOrList)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,userOrList);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3674],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3674],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&strText,
                  ( unsigned char __RPC_FAR * )&userOrList);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3674],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_Take_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pThing);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3708],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3708],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pThing);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3708],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_Drop_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pThing);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3736],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3736],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pThing);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3736],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_OnIsOpenChanged_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ long lHint,
    /* [in] */ VARIANT varHintData)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,varHintData);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3764],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3764],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&lHint,
                  ( unsigned char __RPC_FAR * )&varHintData);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3764],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_EditObject_Proxy( 
    IThingExemplar __RPC_FAR * This)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,This);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3798],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3798],
                  ( unsigned char __RPC_FAR * )&This);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3798],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_GetEditTabMap_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *pMap)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pMap);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3820],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3820],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pMap);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3820],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_GetSortedEditTabMap_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *pMap)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pMap);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3848],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3848],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pMap);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3848],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_AddClientProperty_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [optional][in] */ VARIANT var)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,var);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3876],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3876],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&bstrName,
                  ( unsigned char __RPC_FAR * )&var);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3876],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_RemoveClientProperty_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrName)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,bstrName);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3910],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3910],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&bstrName);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3910],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_OnIsNoticeableAccess_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ long lPerms,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,bReturnValue);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3938],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3938],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&lPerms,
                  ( unsigned char __RPC_FAR * )&bReturnValue);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3938],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_OnNameAccess_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ long lPerms,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,bReturnValue);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3972],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3972],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&lPerms,
                  ( unsigned char __RPC_FAR * )&bReturnValue);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3972],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_OnOwnerOrWizardAccess_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ long lPerms,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,bReturnValue);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[4006],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[4006],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&lPerms,
                  ( unsigned char __RPC_FAR * )&bReturnValue);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[4006],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

static const unsigned short IThingExemplar_FormatStringOffsetTable[] = 
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    2418,
    2446,
    2474,
    2502,
    2536,
    2570,
    2592,
    2620,
    2654,
    2688,
    2722,
    2756,
    2790,
    2824,
    2858,
    2892,
    2920,
    2948,
    2976,
    3004,
    3032,
    3060,
    3088,
    3122,
    3150,
    3178,
    3206,
    3234,
    3262,
    3296,
    3330,
    3364,
    3398,
    3432,
    3460,
    3488,
    3522,
    3556,
    3584,
    3618,
    3646,
    3674,
    3708,
    3736,
    3764,
    3798,
    3820,
    3848,
    3876,
    3910,
    3938,
    3972,
    4006
    };

static const MIDL_SERVER_INFO IThingExemplar_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IThingExemplar_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IThingExemplar_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IThingExemplar_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(65) _IThingExemplarProxyVtbl = 
{
    &IThingExemplar_ProxyInfo,
    &IID_IThingExemplar,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* (void *)-1 /* IVWDispatch::Initialize */ ,
    0 /* (void *)-1 /* IVWDispatch::Terminate */ ,
    0 /* (void *)-1 /* IVWDispatch::Install */ ,
    0 /* (void *)-1 /* IVWDispatch::OnLoad */ ,
    0 /* (void *)-1 /* IVWDispatch::OnUnload */ ,
    (void *)-1 /* IThingExemplar::Report */ ,
    (void *)-1 /* IThingExemplar::Announce */ ,
    (void *)-1 /* IThingExemplar::OnSystemDestroyClient */ ,
    (void *)-1 /* IThingExemplar::OnContentSystemEnter */ ,
    (void *)-1 /* IThingExemplar::OnContentSystemLeave */ ,
    (void *)-1 /* IThingExemplar::LookAt */ ,
    (void *)-1 /* IThingExemplar::GetMenuList */ ,
    (void *)-1 /* IThingExemplar::GetMenuItems */ ,
    (void *)-1 /* IThingExemplar::GetPeerMenuItems */ ,
    (void *)-1 /* IThingExemplar::GetContainerMenuItems */ ,
    (void *)-1 /* IThingExemplar::GetContentMenuItems */ ,
    (void *)-1 /* IThingExemplar::GetSelfMenuItems */ ,
    (void *)-1 /* IThingExemplar::GetInventoryMenuItems */ ,
    (void *)-1 /* IThingExemplar::GetEditMenuItems */ ,
    (void *)-1 /* IThingExemplar::GetMenuItemsFromProperty */ ,
    (void *)-1 /* IThingExemplar::UpdateIsArtifact */ ,
    (void *)-1 /* IThingExemplar::UpdateIsAvatar */ ,
    (void *)-1 /* IThingExemplar::UpdateIsAvatarAndNotWizard */ ,
    (void *)-1 /* IThingExemplar::UpdateIsArtifactOrAvatar */ ,
    (void *)-1 /* IThingExemplar::UpdateIsTakeable */ ,
    IThingExemplar_UpdatePropertiesMenuItem_Proxy ,
    IThingExemplar_UpdateMenuItemName_Proxy ,
    IThingExemplar_UpdatePropertyListMenu_Proxy ,
    IThingExemplar_UpdateInventoryMenu_Proxy ,
    IThingExemplar_UpdateContentsMenu_Proxy ,
    IThingExemplar_UpdateRoomAvatarsMenu_Proxy ,
    IThingExemplar_DoGesture_Proxy ,
    IThingExemplar_DoNamedGesture_Proxy ,
    IThingExemplar_DoGestureServer_Proxy ,
    IThingExemplar_DoGestureClient_Proxy ,
    IThingExemplar_AllowEnter_Proxy ,
    IThingExemplar_AllowLeave_Proxy ,
    IThingExemplar_AcceptEnter_Proxy ,
    IThingExemplar_AcceptLeave_Proxy ,
    IThingExemplar_Clone_Proxy ,
    IThingExemplar_Emote_Proxy ,
    IThingExemplar_GestureText_Proxy ,
    IThingExemplar_MoveInto_Proxy ,
    IThingExemplar_Say_Proxy ,
    IThingExemplar_Shout_Proxy ,
    IThingExemplar_SayGesture_Proxy ,
    IThingExemplar_Whisper_Proxy ,
    IThingExemplar_Take_Proxy ,
    IThingExemplar_Drop_Proxy ,
    IThingExemplar_OnIsOpenChanged_Proxy ,
    IThingExemplar_EditObject_Proxy ,
    IThingExemplar_GetEditTabMap_Proxy ,
    IThingExemplar_GetSortedEditTabMap_Proxy ,
    IThingExemplar_AddClientProperty_Proxy ,
    IThingExemplar_RemoveClientProperty_Proxy ,
    IThingExemplar_OnIsNoticeableAccess_Proxy ,
    IThingExemplar_OnNameAccess_Proxy ,
    IThingExemplar_OnOwnerOrWizardAccess_Proxy
};


static const PRPC_STUB_FUNCTION IThingExemplar_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IThingExemplarStubVtbl =
{
    &IID_IThingExemplar,
    &IThingExemplar_ServerInfo,
    65,
    &IThingExemplar_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IArtifactExemplar, ver. 0.0,
   GUID={0x99AD361C,0xAAB5,0x11D0,{0xBA,0xE3,0x00,0xC0,0x4F,0xC2,0xCA,0xB1}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IArtifactExemplar_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short IArtifactExemplar_FormatStringOffsetTable[] = 
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    4040,
    4062,
    4090,
    4112,
    4140,
    4168
    };

static const MIDL_SERVER_INFO IArtifactExemplar_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IArtifactExemplar_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IArtifactExemplar_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IArtifactExemplar_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(18) _IArtifactExemplarProxyVtbl = 
{
    &IArtifactExemplar_ProxyInfo,
    &IID_IArtifactExemplar,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* (void *)-1 /* IVWDispatch::Initialize */ ,
    0 /* (void *)-1 /* IVWDispatch::Terminate */ ,
    0 /* (void *)-1 /* IVWDispatch::Install */ ,
    0 /* (void *)-1 /* IVWDispatch::OnLoad */ ,
    0 /* (void *)-1 /* IVWDispatch::OnUnload */ ,
    (void *)-1 /* IArtifactExemplar::WieldItem */ ,
    (void *)-1 /* IArtifactExemplar::UpdateWield */ ,
    (void *)-1 /* IArtifactExemplar::UnWieldItem */ ,
    (void *)-1 /* IArtifactExemplar::UpdateUnWield */ ,
    (void *)-1 /* IArtifactExemplar::AcceptLeave */ ,
    (void *)-1 /* IArtifactExemplar::AcceptEnter */
};


static const PRPC_STUB_FUNCTION IArtifactExemplar_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IArtifactExemplarStubVtbl =
{
    &IID_IArtifactExemplar,
    &IArtifactExemplar_ServerInfo,
    18,
    &IArtifactExemplar_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IAvatarExemplar, ver. 0.0,
   GUID={0x99AD361E,0xAAB5,0x11D0,{0xBA,0xE3,0x00,0xC0,0x4F,0xC2,0xCA,0xB1}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IAvatarExemplar_ServerInfo;

#pragma code_seg(".orpc")
/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_ClientQuery_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrTitle,
    /* [in] */ BSTR bstrHTML,
    /* [in] */ BSTR bstrPrompt,
    /* [in] */ BSTR bstrDefault,
    /* [in] */ IThing __RPC_FAR *pNotifyObject,
    /* [in] */ BSTR bstrNotifyMethod,
    /* [in] */ IDispatch __RPC_FAR *pNotifyParam)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pNotifyParam);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[4804],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[4804],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&bstrTitle,
                  ( unsigned char __RPC_FAR * )&bstrHTML,
                  ( unsigned char __RPC_FAR * )&bstrPrompt,
                  ( unsigned char __RPC_FAR * )&bstrDefault,
                  ( unsigned char __RPC_FAR * )&pNotifyObject,
                  ( unsigned char __RPC_FAR * )&bstrNotifyMethod,
                  ( unsigned char __RPC_FAR * )&pNotifyParam);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[4804],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_QueryReturn_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IPropertyList __RPC_FAR *pArg,
    /* [in] */ VARIANT_BOOL boolOK,
    /* [in] */ BSTR bstrResult)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,bstrResult);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[4868],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[4868],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pArg,
                  ( unsigned char __RPC_FAR * )&boolOK,
                  ( unsigned char __RPC_FAR * )&bstrResult);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[4868],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnSystemCreate_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pthing);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[4908],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[4908],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pthing);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[4908],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnSystemConnect_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pWho)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pWho);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[4936],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[4936],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pWho);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[4936],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnSystemDisconnect_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pWho)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pWho);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[4964],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[4964],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pWho);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[4964],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_ThumbnailFileName_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrFileName)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pbstrFileName);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[4992],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[4992],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pbstrFileName);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[4992],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnContainerSystemTell_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pfrom,
    /* [in] */ IPropertyList __RPC_FAR *pToList,
    /* [in] */ BSTR bstrSay,
    /* [in] */ long lval,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrOut)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pbstrOut);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5020],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5020],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pfrom,
                  ( unsigned char __RPC_FAR * )&pToList,
                  ( unsigned char __RPC_FAR * )&bstrSay,
                  ( unsigned char __RPC_FAR * )&lval,
                  ( unsigned char __RPC_FAR * )&pbstrOut);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5020],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnSystemTell_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pfrom,
    /* [in] */ IPropertyList __RPC_FAR *pToList,
    /* [in] */ BSTR bstrSay,
    /* [in] */ long lval,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrOut)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pbstrOut);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5072],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5072],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pfrom,
                  ( unsigned char __RPC_FAR * )&pToList,
                  ( unsigned char __RPC_FAR * )&bstrSay,
                  ( unsigned char __RPC_FAR * )&lval,
                  ( unsigned char __RPC_FAR * )&pbstrOut);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5072],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnPeerSystemTell_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pfrom,
    /* [in] */ IPropertyList __RPC_FAR *pToList,
    /* [in] */ BSTR bstrSay,
    /* [in] */ long lval)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,lval);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5124],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5124],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pfrom,
                  ( unsigned char __RPC_FAR * )&pToList,
                  ( unsigned char __RPC_FAR * )&bstrSay,
                  ( unsigned char __RPC_FAR * )&lval);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5124],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnSystemPreCreate_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pExemplar,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbOkToCreate)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pbOkToCreate);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5170],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5170],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pExemplar,
                  ( unsigned char __RPC_FAR * )&pbOkToCreate);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5170],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnSystemPostCreate_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pThing);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5204],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5204],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pThing);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5204],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnSystemPreDestroy_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbOkToDestroy)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pbOkToDestroy);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3364],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3364],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pthing,
                  ( unsigned char __RPC_FAR * )&pbOkToDestroy);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[3364],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnSystemPostDestroy_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pthing);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5232],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5232],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pthing);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5232],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_SetObjectQuota_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ long lQuota)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,lQuota);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5260],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5260],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&lQuota);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5260],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_AddDefaultKeys_Proxy( 
    IAvatarExemplar __RPC_FAR * This)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,This);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5288],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5288],
                  ( unsigned char __RPC_FAR * )&This);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5288],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnAction_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrAction,
    /* [in] */ BSTR bstrDescription,
    /* [in] */ IPropertyList __RPC_FAR *pParticipants)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pParticipants);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5310],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5310],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&bstrAction,
                  ( unsigned char __RPC_FAR * )&bstrDescription,
                  ( unsigned char __RPC_FAR * )&pParticipants);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5310],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnIsAuthorChanged_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ long lHint,
    /* [in] */ VARIANT varHintData)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,varHintData);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5350],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5350],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&lHint,
                  ( unsigned char __RPC_FAR * )&varHintData);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5350],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnIsWizardOrIsAuthorAccess_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ long lPerms,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,bReturnValue);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5384],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5384],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&lPerms,
                  ( unsigned char __RPC_FAR * )&bReturnValue);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5384],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnWizardAccess_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ long lPerms,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,bReturnValue);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5418],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5418],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&lPerms,
                  ( unsigned char __RPC_FAR * )&bReturnValue);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5418],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_FireConnectAvatar_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pthing);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5452],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5452],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pthing);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[5452],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

static const unsigned short IAvatarExemplar_FormatStringOffsetTable[] = 
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    2418,
    4202,
    4090,
    4236,
    4258,
    4286,
    4314,
    4348,
    4382,
    4422,
    4450,
    4478,
    4500,
    526,
    4528,
    4550,
    4578,
    4642,
    4694,
    4746,
    4804,
    4868,
    4908,
    4936,
    4964,
    4992,
    5020,
    5072,
    5124,
    5170,
    5204,
    3364,
    5232,
    5260,
    5288,
    5310,
    5350,
    5384,
    5418,
    5452
    };

static const MIDL_SERVER_INFO IAvatarExemplar_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IAvatarExemplar_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IAvatarExemplar_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IAvatarExemplar_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(52) _IAvatarExemplarProxyVtbl = 
{
    &IAvatarExemplar_ProxyInfo,
    &IID_IAvatarExemplar,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* (void *)-1 /* IVWDispatch::Initialize */ ,
    0 /* (void *)-1 /* IVWDispatch::Terminate */ ,
    0 /* (void *)-1 /* IVWDispatch::Install */ ,
    0 /* (void *)-1 /* IVWDispatch::OnLoad */ ,
    0 /* (void *)-1 /* IVWDispatch::OnUnload */ ,
    (void *)-1 /* IAvatarExemplar::DoNamedGesture */ ,
    (void *)-1 /* IAvatarExemplar::GestureText */ ,
    (void *)-1 /* IAvatarExemplar::LookAt */ ,
    (void *)-1 /* IAvatarExemplar::WhisperTo */ ,
    (void *)-1 /* IAvatarExemplar::UpdateUserProfileList */ ,
    (void *)-1 /* IAvatarExemplar::MenuUserProfileList */ ,
    (void *)-1 /* IAvatarExemplar::AddToUserProfileList */ ,
    (void *)-1 /* IAvatarExemplar::RemoveFromUserProfileList */ ,
    (void *)-1 /* IAvatarExemplar::IsInUserProfileList */ ,
    (void *)-1 /* IAvatarExemplar::UpdateMenuItemName */ ,
    (void *)-1 /* IAvatarExemplar::UpdateWhisperMenuItem */ ,
    (void *)-1 /* IAvatarExemplar::EditProfile */ ,
    (void *)-1 /* IAvatarExemplar::Gesture */ ,
    (void *)-1 /* IAvatarExemplar::Wield */ ,
    (void *)-1 /* IAvatarExemplar::UnWield */ ,
    (void *)-1 /* IAvatarExemplar::Teleport */ ,
    (void *)-1 /* IAvatarExemplar::Query */ ,
    (void *)-1 /* IAvatarExemplar::Alert */ ,
    (void *)-1 /* IAvatarExemplar::Confirm */ ,
    (void *)-1 /* IAvatarExemplar::Prompt */ ,
    IAvatarExemplar_ClientQuery_Proxy ,
    IAvatarExemplar_QueryReturn_Proxy ,
    IAvatarExemplar_OnSystemCreate_Proxy ,
    IAvatarExemplar_OnSystemConnect_Proxy ,
    IAvatarExemplar_OnSystemDisconnect_Proxy ,
    IAvatarExemplar_ThumbnailFileName_Proxy ,
    IAvatarExemplar_OnContainerSystemTell_Proxy ,
    IAvatarExemplar_OnSystemTell_Proxy ,
    IAvatarExemplar_OnPeerSystemTell_Proxy ,
    IAvatarExemplar_OnSystemPreCreate_Proxy ,
    IAvatarExemplar_OnSystemPostCreate_Proxy ,
    IAvatarExemplar_OnSystemPreDestroy_Proxy ,
    IAvatarExemplar_OnSystemPostDestroy_Proxy ,
    IAvatarExemplar_SetObjectQuota_Proxy ,
    IAvatarExemplar_AddDefaultKeys_Proxy ,
    IAvatarExemplar_OnAction_Proxy ,
    IAvatarExemplar_OnIsAuthorChanged_Proxy ,
    IAvatarExemplar_OnIsWizardOrIsAuthorAccess_Proxy ,
    IAvatarExemplar_OnWizardAccess_Proxy ,
    IAvatarExemplar_FireConnectAvatar_Proxy
};


static const PRPC_STUB_FUNCTION IAvatarExemplar_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IAvatarExemplarStubVtbl =
{
    &IID_IAvatarExemplar,
    &IAvatarExemplar_ServerInfo,
    52,
    &IAvatarExemplar_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IHostExemplar, ver. 0.0,
   GUID={0x44001330,0x1B82,0x11d2,{0xAC,0x71,0x00,0x10,0x4B,0x26,0xA3,0x73}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IHostExemplar_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short IHostExemplar_FormatStringOffsetTable[] = 
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    5480,
    5508
    };

static const MIDL_SERVER_INFO IHostExemplar_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IHostExemplar_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IHostExemplar_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IHostExemplar_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(14) _IHostExemplarProxyVtbl = 
{
    &IHostExemplar_ProxyInfo,
    &IID_IHostExemplar,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* (void *)-1 /* IVWDispatch::Initialize */ ,
    0 /* (void *)-1 /* IVWDispatch::Terminate */ ,
    0 /* (void *)-1 /* IVWDispatch::Install */ ,
    0 /* (void *)-1 /* IVWDispatch::OnLoad */ ,
    0 /* (void *)-1 /* IVWDispatch::OnUnload */ ,
    (void *)-1 /* IHostExemplar::ShowLogOffDialog */ ,
    (void *)-1 /* IHostExemplar::LogOff */
};


static const PRPC_STUB_FUNCTION IHostExemplar_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IHostExemplarStubVtbl =
{
    &IID_IHostExemplar,
    &IHostExemplar_ServerInfo,
    14,
    &IHostExemplar_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IPortalExemplar, ver. 0.0,
   GUID={0x99AD3624,0xAAB5,0x11D0,{0xBA,0xE3,0x00,0xC0,0x4F,0xC2,0xCA,0xB1}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPortalExemplar_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short IPortalExemplar_FormatStringOffsetTable[] = 
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    4040,
    5542,
    5564,
    5598,
    5626,
    4168,
    5660
    };

static const MIDL_SERVER_INFO IPortalExemplar_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPortalExemplar_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IPortalExemplar_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPortalExemplar_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(19) _IPortalExemplarProxyVtbl = 
{
    &IPortalExemplar_ProxyInfo,
    &IID_IPortalExemplar,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* (void *)-1 /* IVWDispatch::Initialize */ ,
    0 /* (void *)-1 /* IVWDispatch::Terminate */ ,
    0 /* (void *)-1 /* IVWDispatch::Install */ ,
    0 /* (void *)-1 /* IVWDispatch::OnLoad */ ,
    0 /* (void *)-1 /* IVWDispatch::OnUnload */ ,
    (void *)-1 /* IPortalExemplar::ShowTransportDialog */ ,
    (void *)-1 /* IPortalExemplar::TransportMenu */ ,
    (void *)-1 /* IPortalExemplar::Transport */ ,
    (void *)-1 /* IPortalExemplar::TransportTo */ ,
    (void *)-1 /* IPortalExemplar::AllowTransport */ ,
    (void *)-1 /* IPortalExemplar::AcceptTransport */ ,
    (void *)-1 /* IPortalExemplar::SetDestinationByID */
};


static const PRPC_STUB_FUNCTION IPortalExemplar_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IPortalExemplarStubVtbl =
{
    &IID_IPortalExemplar,
    &IPortalExemplar_ServerInfo,
    19,
    &IPortalExemplar_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IIWPortalExemplar, ver. 0.0,
   GUID={0xFFA01B4F,0x9F4C,0x11d1,{0x80,0xE8,0x00,0xC0,0x4F,0xC2,0xA1,0x29}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IIWPortalExemplar_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short IIWPortalExemplar_FormatStringOffsetTable[] = 
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    5688
    };

static const MIDL_SERVER_INFO IIWPortalExemplar_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IIWPortalExemplar_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IIWPortalExemplar_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IIWPortalExemplar_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(13) _IIWPortalExemplarProxyVtbl = 
{
    &IIWPortalExemplar_ProxyInfo,
    &IID_IIWPortalExemplar,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* (void *)-1 /* IVWDispatch::Initialize */ ,
    0 /* (void *)-1 /* IVWDispatch::Terminate */ ,
    0 /* (void *)-1 /* IVWDispatch::Install */ ,
    0 /* (void *)-1 /* IVWDispatch::OnLoad */ ,
    0 /* (void *)-1 /* IVWDispatch::OnUnload */ ,
    (void *)-1 /* IIWPortalExemplar::Transport */
};


static const PRPC_STUB_FUNCTION IIWPortalExemplar_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2
};

CInterfaceStubVtbl _IIWPortalExemplarStubVtbl =
{
    &IID_IIWPortalExemplar,
    &IIWPortalExemplar_ServerInfo,
    13,
    &IIWPortalExemplar_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IRoomExemplar, ver. 0.0,
   GUID={0x99AD3626,0xAAB5,0x11D0,{0xBA,0xE3,0x00,0xC0,0x4F,0xC2,0xCA,0xB1}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IRoomExemplar_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short IRoomExemplar_FormatStringOffsetTable[] = 
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    5722,
    5762,
    5796,
    5830,
    5626,
    5864,
    5892,
    5920,
    5948,
    5976,
    6028,
    6062
    };

static const MIDL_SERVER_INFO IRoomExemplar_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IRoomExemplar_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IRoomExemplar_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IRoomExemplar_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(24) _IRoomExemplarProxyVtbl = 
{
    &IRoomExemplar_ProxyInfo,
    &IID_IRoomExemplar,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* (void *)-1 /* IVWDispatch::Initialize */ ,
    0 /* (void *)-1 /* IVWDispatch::Terminate */ ,
    0 /* (void *)-1 /* IVWDispatch::Install */ ,
    0 /* (void *)-1 /* IVWDispatch::OnLoad */ ,
    0 /* (void *)-1 /* IVWDispatch::OnUnload */ ,
    (void *)-1 /* IRoomExemplar::Expel */ ,
    (void *)-1 /* IRoomExemplar::TransmitWhisper */ ,
    (void *)-1 /* IRoomExemplar::ReceiveWhisper */ ,
    (void *)-1 /* IRoomExemplar::AllowEnter */ ,
    (void *)-1 /* IRoomExemplar::AllowLeave */ ,
    (void *)-1 /* IRoomExemplar::OnSystemCreate */ ,
    (void *)-1 /* IRoomExemplar::OnSystemDestroy */ ,
    (void *)-1 /* IRoomExemplar::OnContentSystemConnect */ ,
    (void *)-1 /* IRoomExemplar::OnContentSystemDisconnect */ ,
    (void *)-1 /* IRoomExemplar::OnContentSystemTell */ ,
    (void *)-1 /* IRoomExemplar::OnContentEnter */ ,
    (void *)-1 /* IRoomExemplar::OnContentLeave */
};


static const PRPC_STUB_FUNCTION IRoomExemplar_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IRoomExemplarStubVtbl =
{
    &IID_IRoomExemplar,
    &IRoomExemplar_ServerInfo,
    24,
    &IRoomExemplar_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_vwsysex_0264, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IStateMachineExemplar, ver. 0.0,
   GUID={0x69A2A200,0xF187,0x11d1,{0xA3,0x92,0x00,0x00,0xF8,0x07,0x55,0x8D}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IStateMachineExemplar_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short IStateMachineExemplar_FormatStringOffsetTable[] = 
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    6096,
    2446,
    6130,
    6164,
    6198,
    6238,
    6272,
    6312,
    6358,
    6398,
    6444,
    6484,
    6530,
    6582,
    6640,
    6686,
    6744,
    6790
    };

static const MIDL_SERVER_INFO IStateMachineExemplar_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IStateMachineExemplar_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IStateMachineExemplar_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IStateMachineExemplar_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(30) _IStateMachineExemplarProxyVtbl = 
{
    &IStateMachineExemplar_ProxyInfo,
    &IID_IStateMachineExemplar,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* (void *)-1 /* IVWDispatch::Initialize */ ,
    0 /* (void *)-1 /* IVWDispatch::Terminate */ ,
    0 /* (void *)-1 /* IVWDispatch::Install */ ,
    0 /* (void *)-1 /* IVWDispatch::OnLoad */ ,
    0 /* (void *)-1 /* IVWDispatch::OnUnload */ ,
    (void *)-1 /* IStateMachineExemplar::SMchAddStateMachine */ ,
    (void *)-1 /* IStateMachineExemplar::SMchSetState */ ,
    (void *)-1 /* IStateMachineExemplar::SMchSetStateMachineState */ ,
    (void *)-1 /* IStateMachineExemplar::SMchIsInState */ ,
    (void *)-1 /* IStateMachineExemplar::SMchIsStateMachineInState */ ,
    (void *)-1 /* IStateMachineExemplar::SMchSetIfState */ ,
    (void *)-1 /* IStateMachineExemplar::SMchSetStateMachineIfState */ ,
    (void *)-1 /* IStateMachineExemplar::SMchGetStateConcatName */ ,
    (void *)-1 /* IStateMachineExemplar::SMchGetCurrentStateConcatName */ ,
    (void *)-1 /* IStateMachineExemplar::SMchGetStateProperty */ ,
    (void *)-1 /* IStateMachineExemplar::SMchPutStateProperty */ ,
    (void *)-1 /* IStateMachineExemplar::SMchPutStateMachineStateProperty */ ,
    (void *)-1 /* IStateMachineExemplar::SMchPutInlineStateMethod */ ,
    (void *)-1 /* IStateMachineExemplar::SMchPutStateMachineInlineStateMethod */ ,
    (void *)-1 /* IStateMachineExemplar::SMchGetInlineStateMethod */ ,
    (void *)-1 /* IStateMachineExemplar::SMchAddStateMethod */ ,
    (void *)-1 /* IStateMachineExemplar::SMchGetStateMethodFlags */ ,
    (void *)-1 /* IStateMachineExemplar::SMchOnInvokeMethodFailed */
};


static const PRPC_STUB_FUNCTION IStateMachineExemplar_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IStateMachineExemplarStubVtbl =
{
    &IID_IStateMachineExemplar,
    &IStateMachineExemplar_ServerInfo,
    30,
    &IStateMachineExemplar_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IWebPageExemplar, ver. 0.0,
   GUID={0x63CA06F6,0x5CC2,0x11d2,{0x82,0xF0,0x00,0x10,0x4B,0x26,0xA3,0x73}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IWebPageExemplar_ServerInfo;

#pragma code_seg(".orpc")
extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[2];

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x20000, /* Ndr library version */
    0,
    0x50100a4, /* MIDL Version 5.1.164 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    1,  /* Flags */
    0,  /* Reserved3 */
    0,  /* Reserved4 */
    0   /* Reserved5 */
    };

static const unsigned short IWebPageExemplar_FormatStringOffsetTable[] = 
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    4040,
    6824
    };

static const MIDL_SERVER_INFO IWebPageExemplar_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IWebPageExemplar_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IWebPageExemplar_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IWebPageExemplar_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(14) _IWebPageExemplarProxyVtbl = 
{
    &IWebPageExemplar_ProxyInfo,
    &IID_IWebPageExemplar,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* (void *)-1 /* IVWDispatch::Initialize */ ,
    0 /* (void *)-1 /* IVWDispatch::Terminate */ ,
    0 /* (void *)-1 /* IVWDispatch::Install */ ,
    0 /* (void *)-1 /* IVWDispatch::OnLoad */ ,
    0 /* (void *)-1 /* IVWDispatch::OnUnload */ ,
    (void *)-1 /* IWebPageExemplar::Show */ ,
    (void *)-1 /* IWebPageExemplar::OnLButtonUp */
};


static const PRPC_STUB_FUNCTION IWebPageExemplar_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IWebPageExemplarStubVtbl =
{
    &IID_IWebPageExemplar,
    &IWebPageExemplar_ServerInfo,
    14,
    &IWebPageExemplar_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

#pragma data_seg(".rdata")

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[2] = 
        {
            
            {
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
            },
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            }

        };


#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT40_OR_LATER)
#error You need a Windows NT 4.0 or later to run this stub because it uses these features:
#error   -Oif or -Oicf, [wire_marshal] or [user_marshal] attribute, more than 64 delegated procs, float, double or hyper in -Oif or -Oicf, more than 32 methods in the interface.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure CreatePropertyList */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0xc ),	/* 12 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/*  8 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
#else
			NdrFcShort( 0x30 ),	/* MIPS & PPC Stack size/offset = 48 */
#endif
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter varInitialValue */

/* 16 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 18 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 20 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter varOrphan */

/* 22 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 24 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 26 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter pppropertylist */

/* 28 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 30 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
#else
			NdrFcShort( 0x28 ),	/* MIPS & PPC Stack size/offset = 40 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 32 */	NdrFcShort( 0x3ca ),	/* Type Offset=970 */

	/* Return value */

/* 34 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 36 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
#else
			NdrFcShort( 0x2c ),	/* MIPS & PPC Stack size/offset = 44 */
#endif
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 38 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreatePropertyMap */

/* 40 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 42 */	NdrFcLong( 0x0 ),	/* 0 */
/* 46 */	NdrFcShort( 0xd ),	/* 13 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 48 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 50 */	NdrFcShort( 0x0 ),	/* 0 */
/* 52 */	NdrFcShort( 0x8 ),	/* 8 */
/* 54 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter varOrphan */

/* 56 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 58 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 60 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter pppropertymap */

/* 62 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 64 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 66 */	NdrFcShort( 0x3e0 ),	/* Type Offset=992 */

	/* Return value */

/* 68 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 70 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 72 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateMenu */

/* 74 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 76 */	NdrFcLong( 0x0 ),	/* 0 */
/* 80 */	NdrFcShort( 0xe ),	/* 14 */
#ifndef _ALPHA_
/* 82 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 84 */	NdrFcShort( 0x0 ),	/* 0 */
/* 86 */	NdrFcShort( 0x8 ),	/* 8 */
/* 88 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter ppMenu */

/* 90 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 92 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 94 */	NdrFcShort( 0x3ca ),	/* Type Offset=970 */

	/* Return value */

/* 96 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 98 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 100 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateMenuItem */

/* 102 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 104 */	NdrFcLong( 0x0 ),	/* 0 */
/* 108 */	NdrFcShort( 0xf ),	/* 15 */
#ifndef _ALPHA_
/* 110 */	NdrFcShort( 0x30 ),	/* x86, MIPS, PPC Stack size/offset = 48 */
#else
			NdrFcShort( 0x50 ),	/* Alpha Stack size/offset = 80 */
#endif
/* 112 */	NdrFcShort( 0x8 ),	/* 8 */
/* 114 */	NdrFcShort( 0x8 ),	/* 8 */
/* 116 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x8,		/* 8 */

	/* Parameter bstrMenuText */

/* 118 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 120 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 122 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter lFlags */

/* 124 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 126 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 128 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pOwner */

/* 130 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 132 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 134 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter bstrUpdateMethod */

/* 136 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 138 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 140 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrMethod */

/* 142 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 144 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 146 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter var */

/* 148 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 150 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 152 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter ppMenuItem */

/* 154 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 156 */	NdrFcShort( 0x28 ),	/* x86, MIPS, PPC Stack size/offset = 40 */
#else
			NdrFcShort( 0x40 ),	/* Alpha Stack size/offset = 64 */
#endif
/* 158 */	NdrFcShort( 0x412 ),	/* Type Offset=1042 */

	/* Return value */

/* 160 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 162 */	NdrFcShort( 0x2c ),	/* x86, MIPS, PPC Stack size/offset = 44 */
#else
			NdrFcShort( 0x48 ),	/* Alpha Stack size/offset = 72 */
#endif
/* 164 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateMenuSeparator */

/* 166 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 168 */	NdrFcLong( 0x0 ),	/* 0 */
/* 172 */	NdrFcShort( 0x10 ),	/* 16 */
#ifndef _ALPHA_
/* 174 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 176 */	NdrFcShort( 0x0 ),	/* 0 */
/* 178 */	NdrFcShort( 0x8 ),	/* 8 */
/* 180 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pOwner */

/* 182 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 184 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 186 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter ppMenuItem */

/* 188 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 190 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 192 */	NdrFcShort( 0x412 ),	/* Type Offset=1042 */

	/* Return value */

/* 194 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 196 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 198 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateOCXProxy */

/* 200 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 202 */	NdrFcLong( 0x0 ),	/* 0 */
/* 206 */	NdrFcShort( 0x11 ),	/* 17 */
#ifndef _ALPHA_
/* 208 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 210 */	NdrFcShort( 0x0 ),	/* 0 */
/* 212 */	NdrFcShort( 0x8 ),	/* 8 */
/* 214 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pOCX */

/* 216 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 218 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 220 */	NdrFcShort( 0x146 ),	/* Type Offset=326 */

	/* Parameter ppOCXProxy */

/* 222 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 224 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 226 */	NdrFcShort( 0x428 ),	/* Type Offset=1064 */

	/* Return value */

/* 228 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 230 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 232 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure QueryAvatarNames */

/* 234 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 236 */	NdrFcLong( 0x0 ),	/* 0 */
/* 240 */	NdrFcShort( 0x12 ),	/* 18 */
#ifndef _ALPHA_
/* 242 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 244 */	NdrFcShort( 0x8 ),	/* 8 */
/* 246 */	NdrFcShort( 0x8 ),	/* 8 */
/* 248 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x3,		/* 3 */

	/* Parameter lFalgs */

/* 250 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 252 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 254 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pNameList */

/* 256 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 258 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 260 */	NdrFcShort( 0x3ca ),	/* Type Offset=970 */

	/* Return value */

/* 262 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 264 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 266 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure InvokeRemoteClientMethod */

/* 268 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 270 */	NdrFcLong( 0x0 ),	/* 0 */
/* 274 */	NdrFcShort( 0x13 ),	/* 19 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 276 */	NdrFcShort( 0x30 ),	/* x86 Stack size/offset = 48 */
#else
			NdrFcShort( 0x34 ),	/* MIPS & PPC Stack size/offset = 52 */
#endif
#else
			NdrFcShort( 0x40 ),	/* Alpha Stack size/offset = 64 */
#endif
/* 278 */	NdrFcShort( 0x0 ),	/* 0 */
/* 280 */	NdrFcShort( 0x8 ),	/* 8 */
/* 282 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x5,		/* 5 */

	/* Parameter pDispTarget */

/* 284 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 286 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 288 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter bstrMethodName */

/* 290 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 292 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 294 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter varUserOrList */

/* 296 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 298 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
#else
			NdrFcShort( 0x10 ),	/* MIPS & PPC Stack size/offset = 16 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 300 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter varParams */

/* 302 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 304 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 306 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 308 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 310 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
#else
			NdrFcShort( 0x30 ),	/* MIPS & PPC Stack size/offset = 48 */
#endif
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 312 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure HandleRemoteClientMethod */

/* 314 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 316 */	NdrFcLong( 0x0 ),	/* 0 */
/* 320 */	NdrFcShort( 0x14 ),	/* 20 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 322 */	NdrFcShort( 0x30 ),	/* x86 Stack size/offset = 48 */
#else
			NdrFcShort( 0x34 ),	/* MIPS & PPC Stack size/offset = 52 */
#endif
#else
			NdrFcShort( 0x40 ),	/* Alpha Stack size/offset = 64 */
#endif
/* 324 */	NdrFcShort( 0x0 ),	/* 0 */
/* 326 */	NdrFcShort( 0x8 ),	/* 8 */
/* 328 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x5,		/* 5 */

	/* Parameter pDispTarget */

/* 330 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 332 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 334 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter bstrMethodName */

/* 336 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 338 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 340 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter varUserOrList */

/* 342 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 344 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
#else
			NdrFcShort( 0x10 ),	/* MIPS & PPC Stack size/offset = 16 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 346 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter varParams */

/* 348 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 350 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 352 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 354 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 356 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
#else
			NdrFcShort( 0x30 ),	/* MIPS & PPC Stack size/offset = 48 */
#endif
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 358 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure FireUIEvent */

/* 360 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 362 */	NdrFcLong( 0x0 ),	/* 0 */
/* 366 */	NdrFcShort( 0x15 ),	/* 21 */
#ifndef _ALPHA_
/* 368 */	NdrFcShort( 0x24 ),	/* x86, MIPS, PPC Stack size/offset = 36 */
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 370 */	NdrFcShort( 0x0 ),	/* 0 */
/* 372 */	NdrFcShort( 0x8 ),	/* 8 */
/* 374 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x5,		/* 5 */

	/* Parameter pClientList */

/* 376 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 378 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 380 */	NdrFcShort( 0x3ce ),	/* Type Offset=974 */

	/* Parameter pSource */

/* 382 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 384 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 386 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter bstrMethodName */

/* 388 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 390 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 392 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter varParams */

/* 394 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 396 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 398 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 400 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 402 */	NdrFcShort( 0x20 ),	/* x86, MIPS, PPC Stack size/offset = 32 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 404 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure FireRemoteUIEvent */

/* 406 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 408 */	NdrFcLong( 0x0 ),	/* 0 */
/* 412 */	NdrFcShort( 0x16 ),	/* 22 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 414 */	NdrFcShort( 0x30 ),	/* x86 Stack size/offset = 48 */
#else
			NdrFcShort( 0x34 ),	/* MIPS & PPC Stack size/offset = 52 */
#endif
#else
			NdrFcShort( 0x40 ),	/* Alpha Stack size/offset = 64 */
#endif
/* 416 */	NdrFcShort( 0x0 ),	/* 0 */
/* 418 */	NdrFcShort( 0x8 ),	/* 8 */
/* 420 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x5,		/* 5 */

	/* Parameter varClientList */

/* 422 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 424 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 426 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter pSource */

/* 428 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 430 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 432 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter bstrMethodName */

/* 434 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 436 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 438 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter varParams */

/* 440 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 442 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 444 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 446 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 448 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
#else
			NdrFcShort( 0x30 ),	/* MIPS & PPC Stack size/offset = 48 */
#endif
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 450 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnRootURLChanged */

/* 452 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 454 */	NdrFcLong( 0x0 ),	/* 0 */
/* 458 */	NdrFcShort( 0x17 ),	/* 23 */
#ifndef _ALPHA_
/* 460 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 462 */	NdrFcShort( 0x8 ),	/* 8 */
/* 464 */	NdrFcShort( 0x8 ),	/* 8 */
/* 466 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter lHint */

/* 468 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 470 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 472 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter varHintData */

/* 474 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 476 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 478 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 480 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 482 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 484 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure UploadFile */

/* 486 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 488 */	NdrFcLong( 0x0 ),	/* 0 */
/* 492 */	NdrFcShort( 0x18 ),	/* 24 */
#ifndef _ALPHA_
/* 494 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 496 */	NdrFcShort( 0x0 ),	/* 0 */
/* 498 */	NdrFcShort( 0x8 ),	/* 8 */
/* 500 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter bstrLocalName */

/* 502 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 504 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 506 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrRemoteName */

/* 508 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 510 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 512 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter pbstrURL */

/* 514 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 516 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 518 */	NdrFcShort( 0x446 ),	/* Type Offset=1094 */

	/* Return value */

/* 520 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 522 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 524 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Wield */


	/* Procedure MakeHost */

/* 526 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 528 */	NdrFcLong( 0x0 ),	/* 0 */
/* 532 */	NdrFcShort( 0x19 ),	/* 25 */
#ifndef _ALPHA_
/* 534 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 536 */	NdrFcShort( 0x0 ),	/* 0 */
/* 538 */	NdrFcShort( 0x8 ),	/* 8 */
/* 540 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pThing */


	/* Parameter pTarget */

/* 542 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 544 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 546 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Return value */


	/* Return value */

/* 548 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 550 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 552 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure UnMakeHost */

/* 554 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 556 */	NdrFcLong( 0x0 ),	/* 0 */
/* 560 */	NdrFcShort( 0x1a ),	/* 26 */
#ifndef _ALPHA_
/* 562 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 564 */	NdrFcShort( 0x0 ),	/* 0 */
/* 566 */	NdrFcShort( 0x8 ),	/* 8 */
/* 568 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pTarget */

/* 570 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 572 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 574 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Return value */

/* 576 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 578 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 580 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateTimer */

/* 582 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 584 */	NdrFcLong( 0x0 ),	/* 0 */
/* 588 */	NdrFcShort( 0x1b ),	/* 27 */
#ifndef _ALPHA_
/* 590 */	NdrFcShort( 0x28 ),	/* x86, MIPS, PPC Stack size/offset = 40 */
#else
			NdrFcShort( 0x40 ),	/* Alpha Stack size/offset = 64 */
#endif
/* 592 */	NdrFcShort( 0x8 ),	/* 8 */
/* 594 */	NdrFcShort( 0x8 ),	/* 8 */
/* 596 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x6,		/* 6 */

	/* Parameter msecPeriod */

/* 598 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 600 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 602 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pThing */

/* 604 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 606 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 608 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter bstrMethod */

/* 610 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 612 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 614 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter varArgs */

/* 616 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 618 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 620 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter ppTimer */

/* 622 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 624 */	NdrFcShort( 0x20 ),	/* x86, MIPS, PPC Stack size/offset = 32 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 626 */	NdrFcShort( 0x450 ),	/* Type Offset=1104 */

	/* Return value */

/* 628 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 630 */	NdrFcShort( 0x24 ),	/* x86, MIPS, PPC Stack size/offset = 36 */
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 632 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DestroyTimer */

/* 634 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 636 */	NdrFcLong( 0x0 ),	/* 0 */
/* 640 */	NdrFcShort( 0x1c ),	/* 28 */
#ifndef _ALPHA_
/* 642 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 644 */	NdrFcShort( 0x0 ),	/* 0 */
/* 646 */	NdrFcShort( 0x8 ),	/* 8 */
/* 648 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pTimer */

/* 650 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 652 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 654 */	NdrFcShort( 0x146 ),	/* Type Offset=326 */

	/* Return value */

/* 656 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 658 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 660 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnCreateRoomClient */

/* 662 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 664 */	NdrFcLong( 0x0 ),	/* 0 */
/* 668 */	NdrFcShort( 0x1d ),	/* 29 */
#ifndef _ALPHA_
/* 670 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 672 */	NdrFcShort( 0x0 ),	/* 0 */
/* 674 */	NdrFcShort( 0x8 ),	/* 8 */
/* 676 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pThing */

/* 678 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 680 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 682 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Return value */

/* 684 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 686 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 688 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ShowWelcomeMessage */

/* 690 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 692 */	NdrFcLong( 0x0 ),	/* 0 */
/* 696 */	NdrFcShort( 0x1e ),	/* 30 */
#ifndef _ALPHA_
/* 698 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 700 */	NdrFcShort( 0x0 ),	/* 0 */
/* 702 */	NdrFcShort( 0x8 ),	/* 8 */
/* 704 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */

/* 706 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 708 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 710 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnWorldOwnerAccess */

/* 712 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 714 */	NdrFcLong( 0x0 ),	/* 0 */
/* 718 */	NdrFcShort( 0x1f ),	/* 31 */
#ifndef _ALPHA_
/* 720 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 722 */	NdrFcShort( 0x8 ),	/* 8 */
/* 724 */	NdrFcShort( 0xe ),	/* 14 */
/* 726 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter lPerms */

/* 728 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 730 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 732 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter bReturnValue */

/* 734 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 736 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 738 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 740 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 742 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 744 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnGlobalConstantAccess */

/* 746 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 748 */	NdrFcLong( 0x0 ),	/* 0 */
/* 752 */	NdrFcShort( 0x20 ),	/* 32 */
#ifndef _ALPHA_
/* 754 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 756 */	NdrFcShort( 0x8 ),	/* 8 */
/* 758 */	NdrFcShort( 0xe ),	/* 14 */
/* 760 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter lPerms */

/* 762 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 764 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 766 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter bReturnValue */

/* 768 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 770 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 772 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 774 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 776 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 778 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TraceExemplarInfo */

/* 780 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 782 */	NdrFcLong( 0x0 ),	/* 0 */
/* 786 */	NdrFcShort( 0x21 ),	/* 33 */
#ifndef _ALPHA_
/* 788 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 790 */	NdrFcShort( 0x0 ),	/* 0 */
/* 792 */	NdrFcShort( 0x8 ),	/* 8 */
/* 794 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */

/* 796 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 798 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 800 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeGetProperty */

/* 802 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 804 */	NdrFcLong( 0x0 ),	/* 0 */
/* 808 */	NdrFcShort( 0x22 ),	/* 34 */
#ifndef _ALPHA_
/* 810 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 812 */	NdrFcShort( 0x8 ),	/* 8 */
/* 814 */	NdrFcShort( 0x10 ),	/* 16 */
/* 816 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x5,		/* 5 */

	/* Parameter pthing */

/* 818 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 820 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 822 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter bstrPropertyName */

/* 824 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 826 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 828 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter lRepeat */

/* 830 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 832 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 834 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 836 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 838 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 840 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 842 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 844 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 846 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimePutProperty */

/* 848 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 850 */	NdrFcLong( 0x0 ),	/* 0 */
/* 854 */	NdrFcShort( 0x23 ),	/* 35 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 856 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
#else
			NdrFcShort( 0x2c ),	/* MIPS & PPC Stack size/offset = 44 */
#endif
#else
			NdrFcShort( 0x40 ),	/* Alpha Stack size/offset = 64 */
#endif
/* 858 */	NdrFcShort( 0x8 ),	/* 8 */
/* 860 */	NdrFcShort( 0x10 ),	/* 16 */
/* 862 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x6,		/* 6 */

	/* Parameter pthing */

/* 864 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 866 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 868 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter bstrPropertyName */

/* 870 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 872 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 874 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter varData */

/* 876 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 878 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
#else
			NdrFcShort( 0x10 ),	/* MIPS & PPC Stack size/offset = 16 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 880 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter lRepeat */

/* 882 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 884 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 886 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 888 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 890 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
#else
			NdrFcShort( 0x24 ),	/* MIPS & PPC Stack size/offset = 36 */
#endif
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 892 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 894 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 896 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
#else
			NdrFcShort( 0x28 ),	/* MIPS & PPC Stack size/offset = 40 */
#endif
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 898 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeInvokeMethod */

/* 900 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 902 */	NdrFcLong( 0x0 ),	/* 0 */
/* 906 */	NdrFcShort( 0x24 ),	/* 36 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 908 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
#else
			NdrFcShort( 0x2c ),	/* MIPS & PPC Stack size/offset = 44 */
#endif
#else
			NdrFcShort( 0x40 ),	/* Alpha Stack size/offset = 64 */
#endif
/* 910 */	NdrFcShort( 0x8 ),	/* 8 */
/* 912 */	NdrFcShort( 0x10 ),	/* 16 */
/* 914 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x6,		/* 6 */

	/* Parameter pthing */

/* 916 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 918 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 920 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter bstrMethodName */

/* 922 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 924 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 926 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter varArgs */

/* 928 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 930 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
#else
			NdrFcShort( 0x10 ),	/* MIPS & PPC Stack size/offset = 16 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 932 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter lRepeat */

/* 934 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 936 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 938 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 940 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 942 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
#else
			NdrFcShort( 0x24 ),	/* MIPS & PPC Stack size/offset = 36 */
#endif
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 944 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 946 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 948 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
#else
			NdrFcShort( 0x28 ),	/* MIPS & PPC Stack size/offset = 40 */
#endif
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 950 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeGetMethod */

/* 952 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 954 */	NdrFcLong( 0x0 ),	/* 0 */
/* 958 */	NdrFcShort( 0x25 ),	/* 37 */
#ifndef _ALPHA_
/* 960 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 962 */	NdrFcShort( 0x8 ),	/* 8 */
/* 964 */	NdrFcShort( 0x10 ),	/* 16 */
/* 966 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x5,		/* 5 */

	/* Parameter pthing */

/* 968 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 970 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 972 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter bstrMethodName */

/* 974 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 976 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 978 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter lRepeat */

/* 980 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 982 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 984 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 986 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 988 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 990 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 992 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 994 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 996 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeGetPropertySecurity */

/* 998 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1000 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1004 */	NdrFcShort( 0x26 ),	/* 38 */
#ifndef _ALPHA_
/* 1006 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 1008 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1010 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1012 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x5,		/* 5 */

	/* Parameter pthing */

/* 1014 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 1016 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1018 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter bstrPropertyName */

/* 1020 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 1022 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1024 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter lRepeat */

/* 1026 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1028 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1030 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 1032 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 1034 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1036 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1038 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1040 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 1042 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeCheckPropertySecurity */

/* 1044 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1046 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1050 */	NdrFcShort( 0x27 ),	/* 39 */
#ifndef _ALPHA_
/* 1052 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 1054 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1056 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1058 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x6,		/* 6 */

	/* Parameter pthing */

/* 1060 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 1062 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1064 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter bstrPropertyName */

/* 1066 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 1068 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1070 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter ulFlags */

/* 1072 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1074 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1076 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter lRepeat */

/* 1078 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1080 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1082 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 1084 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 1086 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 1088 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1090 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1092 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 1094 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeCheckMethodSecurity */

/* 1096 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1098 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1102 */	NdrFcShort( 0x28 ),	/* 40 */
#ifndef _ALPHA_
/* 1104 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 1106 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1108 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1110 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x6,		/* 6 */

	/* Parameter pthing */

/* 1112 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 1114 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1116 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter bstrMethodName */

/* 1118 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 1120 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1122 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter ulFlags */

/* 1124 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1126 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1128 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter lRepeat */

/* 1130 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1132 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1134 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 1136 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 1138 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 1140 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1142 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1144 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 1146 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeCheckPropertiesSecurity */

/* 1148 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1150 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1154 */	NdrFcShort( 0x29 ),	/* 41 */
#ifndef _ALPHA_
/* 1156 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 1158 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1160 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1162 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x5,		/* 5 */

	/* Parameter pthing */

/* 1164 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 1166 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1168 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter ulFlags */

/* 1170 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1172 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1174 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter lRepeat */

/* 1176 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1178 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1180 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 1182 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 1184 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1186 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1188 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1190 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 1192 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeCheckMethodsSecurity */

/* 1194 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1196 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1200 */	NdrFcShort( 0x2a ),	/* 42 */
#ifndef _ALPHA_
/* 1202 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 1204 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1206 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1208 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x5,		/* 5 */

	/* Parameter pthing */

/* 1210 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 1212 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1214 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter ulFlags */

/* 1216 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1218 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1220 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter lRepeat */

/* 1222 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1224 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1226 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 1228 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 1230 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1232 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1234 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1236 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 1238 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeVTableInvokeMethod */

/* 1240 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1242 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1246 */	NdrFcShort( 0x2b ),	/* 43 */
#ifndef _ALPHA_
/* 1248 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1250 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1252 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1254 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter lRepeat */

/* 1256 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1258 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1260 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 1262 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 1264 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1266 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1268 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1270 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1272 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeAutomationInvokeMethod */

/* 1274 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1276 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1280 */	NdrFcShort( 0x2c ),	/* 44 */
#ifndef _ALPHA_
/* 1282 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1284 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1286 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1288 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter lRepeat */

/* 1290 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1292 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1294 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 1296 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 1298 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1300 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1302 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1304 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1306 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeScriptInvokeMethod */

/* 1308 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1310 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1314 */	NdrFcShort( 0x2d ),	/* 45 */
#ifndef _ALPHA_
/* 1316 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1318 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1320 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1322 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter lRepeat */

/* 1324 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1326 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1328 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 1330 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 1332 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1334 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1336 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1338 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1340 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeInterfaceInvokeMethod */

/* 1342 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1344 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1348 */	NdrFcShort( 0x2e ),	/* 46 */
#ifndef _ALPHA_
/* 1350 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 1352 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1354 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1356 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x5,		/* 5 */

	/* Parameter pthing */

/* 1358 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 1360 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1362 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter bstrMethodName */

/* 1364 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 1366 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1368 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter lRepeat */

/* 1370 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1372 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1374 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 1376 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 1378 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1380 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1382 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1384 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 1386 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeNotifyInvokeMethod */

/* 1388 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1390 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1394 */	NdrFcShort( 0x2f ),	/* 47 */
#ifndef _ALPHA_
/* 1396 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 1398 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1400 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1402 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x6,		/* 6 */

	/* Parameter pthing */

/* 1404 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 1406 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1408 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter bstrMethodName */

/* 1410 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 1412 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1414 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter pdispparams */

/* 1416 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 1418 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1420 */	NdrFcShort( 0x47c ),	/* Type Offset=1148 */

	/* Parameter lRepeat */

/* 1422 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1424 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1426 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 1428 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 1430 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 1432 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1434 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1436 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 1438 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeNotifyPropertyChanged */

/* 1440 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1442 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1446 */	NdrFcShort( 0x30 ),	/* 48 */
#ifndef _ALPHA_
/* 1448 */	NdrFcShort( 0x2c ),	/* x86, MIPS, PPC Stack size/offset = 44 */
#else
			NdrFcShort( 0x48 ),	/* Alpha Stack size/offset = 72 */
#endif
/* 1450 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1452 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1454 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x7,		/* 7 */

	/* Parameter pthing */

/* 1456 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 1458 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1460 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter bstrPropertyName */

/* 1462 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 1464 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1466 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter lHint */

/* 1468 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1470 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1472 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter varHintData */

/* 1474 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 1476 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1478 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter lRepeat */

/* 1480 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1482 */	NdrFcShort( 0x20 ),	/* x86, MIPS, PPC Stack size/offset = 32 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 1484 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 1486 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 1488 */	NdrFcShort( 0x24 ),	/* x86, MIPS, PPC Stack size/offset = 36 */
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 1490 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1492 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1494 */	NdrFcShort( 0x28 ),	/* x86, MIPS, PPC Stack size/offset = 40 */
#else
			NdrFcShort( 0x40 ),	/* Alpha Stack size/offset = 64 */
#endif
/* 1496 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeSendThingCommand */

/* 1498 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1500 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1504 */	NdrFcShort( 0x31 ),	/* 49 */
#ifndef _ALPHA_
/* 1506 */	NdrFcShort( 0x24 ),	/* x86, MIPS, PPC Stack size/offset = 36 */
#else
			NdrFcShort( 0x48 ),	/* Alpha Stack size/offset = 72 */
#endif
/* 1508 */	NdrFcShort( 0x1e ),	/* 30 */
/* 1510 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1512 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x8,		/* 8 */

	/* Parameter lFlags */

/* 1514 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1516 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1518 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pthing */

/* 1520 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 1522 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1524 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter dispid */

/* 1526 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1528 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1530 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter wFlags */

/* 1532 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1534 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1536 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter pdispparams */

/* 1538 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 1540 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 1542 */	NdrFcShort( 0x47c ),	/* Type Offset=1148 */

	/* Parameter lRepeat */

/* 1544 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1546 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 1548 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 1550 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 1552 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 1554 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1556 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1558 */	NdrFcShort( 0x20 ),	/* x86, MIPS, PPC Stack size/offset = 32 */
#else
			NdrFcShort( 0x40 ),	/* Alpha Stack size/offset = 64 */
#endif
/* 1560 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeSendObjectPropertyCommand */

/* 1562 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1564 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1568 */	NdrFcShort( 0x32 ),	/* 50 */
#ifndef _ALPHA_
/* 1570 */	NdrFcShort( 0x24 ),	/* x86, MIPS, PPC Stack size/offset = 36 */
#else
			NdrFcShort( 0x48 ),	/* Alpha Stack size/offset = 72 */
#endif
/* 1572 */	NdrFcShort( 0x1e ),	/* 30 */
/* 1574 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1576 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x8,		/* 8 */

	/* Parameter lFlags */

/* 1578 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1580 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1582 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pprop */

/* 1584 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 1586 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1588 */	NdrFcShort( 0x492 ),	/* Type Offset=1170 */

	/* Parameter dispid */

/* 1590 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1592 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1594 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter wFlags */

/* 1596 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1598 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1600 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter pdispparams */

/* 1602 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 1604 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 1606 */	NdrFcShort( 0x47c ),	/* Type Offset=1148 */

	/* Parameter lRepeat */

/* 1608 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1610 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 1612 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 1614 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 1616 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 1618 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1620 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1622 */	NdrFcShort( 0x20 ),	/* x86, MIPS, PPC Stack size/offset = 32 */
#else
			NdrFcShort( 0x40 ),	/* Alpha Stack size/offset = 64 */
#endif
/* 1624 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeSendWorldCommand */

/* 1626 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1628 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1632 */	NdrFcShort( 0x33 ),	/* 51 */
#ifndef _ALPHA_
/* 1634 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 1636 */	NdrFcShort( 0x16 ),	/* 22 */
/* 1638 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1640 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x6,		/* 6 */

	/* Parameter dispid */

/* 1642 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1644 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1646 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter wFlags */

/* 1648 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1650 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1652 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter pdispparams */

/* 1654 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
#ifndef _ALPHA_
/* 1656 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1658 */	NdrFcShort( 0x47c ),	/* Type Offset=1148 */

	/* Parameter lRepeat */

/* 1660 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1662 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1664 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 1666 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 1668 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 1670 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1672 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1674 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 1676 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeProcessMessage */

/* 1678 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1680 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1684 */	NdrFcShort( 0x34 ),	/* 52 */
#ifndef _ALPHA_
/* 1686 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 1688 */	NdrFcShort( 0xd ),	/* 13 */
/* 1690 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1692 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x5,		/* 5 */

	/* Parameter ch */

/* 1694 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1696 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1698 */	0x2,		/* FC_CHAR */
			0x0,		/* 0 */

	/* Parameter pUbuffer */

/* 1700 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 1702 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1704 */	NdrFcShort( 0x4a4 ),	/* Type Offset=1188 */

	/* Parameter lRepeat */

/* 1706 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1708 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1710 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 1712 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 1714 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1716 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1718 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1720 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 1722 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeCloneProperty */

/* 1724 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1726 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1730 */	NdrFcShort( 0x35 ),	/* 53 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1732 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
#else
			NdrFcShort( 0x24 ),	/* MIPS & PPC Stack size/offset = 36 */
#endif
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 1734 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1736 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1738 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter varData */

/* 1740 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1742 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1744 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter lRepeat */

/* 1746 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1748 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1750 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 1752 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1754 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1756 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1758 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1760 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 1762 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeCopyProperty */

/* 1764 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1766 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1770 */	NdrFcShort( 0x36 ),	/* 54 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1772 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
#else
			NdrFcShort( 0x24 ),	/* MIPS & PPC Stack size/offset = 36 */
#endif
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 1774 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1776 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1778 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter varData */

/* 1780 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1782 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1784 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter lRepeat */

/* 1786 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1788 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1790 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 1792 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1794 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1796 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1798 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1800 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 1802 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimePutPropertySecurity */

/* 1804 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1806 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1810 */	NdrFcShort( 0x37 ),	/* 55 */
#ifndef _ALPHA_
/* 1812 */	NdrFcShort( 0x24 ),	/* x86, MIPS, PPC Stack size/offset = 36 */
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 1814 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1816 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1818 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x5,		/* 5 */

	/* Parameter pSecurity */

/* 1820 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 1822 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1824 */	NdrFcShort( 0x4b6 ),	/* Type Offset=1206 */

	/* Parameter varData */

/* 1826 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 1828 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1830 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter lRepeat */

/* 1832 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1834 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1836 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 1838 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 1840 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 1842 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1844 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1846 */	NdrFcShort( 0x20 ),	/* x86, MIPS, PPC Stack size/offset = 32 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 1848 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeGetPropertyAll */

/* 1850 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1852 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1856 */	NdrFcShort( 0x38 ),	/* 56 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1858 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
#else
			NdrFcShort( 0x24 ),	/* MIPS & PPC Stack size/offset = 36 */
#endif
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 1860 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1862 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1864 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter varData */

/* 1866 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1868 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1870 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter lRepeat */

/* 1872 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1874 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1876 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 1878 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1880 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1882 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1884 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1886 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 1888 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimePutPropertyAll */

/* 1890 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1892 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1896 */	NdrFcShort( 0x39 ),	/* 57 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1898 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
#else
			NdrFcShort( 0x24 ),	/* MIPS & PPC Stack size/offset = 36 */
#endif
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 1900 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1902 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1904 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter varData */

/* 1906 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1908 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1910 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter lRepeat */

/* 1912 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1914 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1916 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 1918 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1920 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1922 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1924 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 1926 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 1928 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeInvokeMethodAll */

/* 1930 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1932 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1936 */	NdrFcShort( 0x3a ),	/* 58 */
#ifndef _ALPHA_
/* 1938 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1940 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1942 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1944 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter lRepeat */

/* 1946 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1948 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1950 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 1952 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 1954 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1956 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1958 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1960 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1962 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeGetMethodAll */

/* 1964 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1966 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1970 */	NdrFcShort( 0x3b ),	/* 59 */
#ifndef _ALPHA_
/* 1972 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 1974 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1976 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1978 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter lRepeat */

/* 1980 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 1982 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 1984 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 1986 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 1988 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 1990 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1992 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 1994 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 1996 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeGetPropertySecurityAll */

/* 1998 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2000 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2004 */	NdrFcShort( 0x3c ),	/* 60 */
#ifndef _ALPHA_
/* 2006 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 2008 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2010 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2012 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter lRepeat */

/* 2014 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 2016 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2018 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 2020 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 2022 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2024 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2026 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2028 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2030 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeCheckPropertySecurityAll */

/* 2032 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2034 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2038 */	NdrFcShort( 0x3d ),	/* 61 */
#ifndef _ALPHA_
/* 2040 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 2042 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2044 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2046 */	0x4,		/* Oi2 Flags:  has return, */
			0x4,		/* 4 */

	/* Parameter ulFlags */

/* 2048 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 2050 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2052 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter lRepeat */

/* 2054 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 2056 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2058 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 2060 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 2062 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2064 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2066 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2068 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 2070 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeCheckMethodSecurityAll */

/* 2072 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2074 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2078 */	NdrFcShort( 0x3e ),	/* 62 */
#ifndef _ALPHA_
/* 2080 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 2082 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2084 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2086 */	0x4,		/* Oi2 Flags:  has return, */
			0x4,		/* 4 */

	/* Parameter ulFlags */

/* 2088 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 2090 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2092 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter lRepeat */

/* 2094 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 2096 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2098 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 2100 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 2102 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2104 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2106 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2108 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 2110 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeCheckPropertiesSecurityAll */

/* 2112 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2114 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2118 */	NdrFcShort( 0x3f ),	/* 63 */
#ifndef _ALPHA_
/* 2120 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 2122 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2124 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2126 */	0x4,		/* Oi2 Flags:  has return, */
			0x4,		/* 4 */

	/* Parameter ulFlags */

/* 2128 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 2130 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2132 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter lRepeat */

/* 2134 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 2136 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2138 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 2140 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 2142 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2144 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2146 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2148 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 2150 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeCheckMethodsSecurityAll */

/* 2152 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2154 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2158 */	NdrFcShort( 0x40 ),	/* 64 */
#ifndef _ALPHA_
/* 2160 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 2162 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2164 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2166 */	0x4,		/* Oi2 Flags:  has return, */
			0x4,		/* 4 */

	/* Parameter ulFlags */

/* 2168 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 2170 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2172 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter lRepeat */

/* 2174 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 2176 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2178 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 2180 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 2182 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2184 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2186 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2188 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 2190 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeNotifyAll */

/* 2192 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2194 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2198 */	NdrFcShort( 0x41 ),	/* 65 */
#ifndef _ALPHA_
/* 2200 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 2202 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2204 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2206 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter lRepeat */

/* 2208 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 2210 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2212 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 2214 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 2216 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2218 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2220 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2222 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2224 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeSendCommandAll */

/* 2226 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2228 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2232 */	NdrFcShort( 0x42 ),	/* 66 */
#ifndef _ALPHA_
/* 2234 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 2236 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2238 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2240 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter lRepeat */

/* 2242 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 2244 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2246 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 2248 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 2250 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2252 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2254 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2256 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2258 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeProcessMessageAll */

/* 2260 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2262 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2266 */	NdrFcShort( 0x43 ),	/* 67 */
#ifndef _ALPHA_
/* 2268 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 2270 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2272 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2274 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter lRepeat */

/* 2276 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 2278 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2280 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 2282 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 2284 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2286 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2288 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2290 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2292 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TimeAll */

/* 2294 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2296 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2300 */	NdrFcShort( 0x44 ),	/* 68 */
#ifndef _ALPHA_
/* 2302 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 2304 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2306 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2308 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter lRepeat */

/* 2310 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 2312 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2314 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plElapsed */

/* 2316 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 2318 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2320 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2322 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2324 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2326 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure YYY */

/* 2328 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2330 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2334 */	NdrFcShort( 0x45 ),	/* 69 */
#ifndef _ALPHA_
/* 2336 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2338 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2340 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2342 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */

/* 2344 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2346 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2348 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnXXXChanged */

/* 2350 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2352 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2356 */	NdrFcShort( 0x46 ),	/* 70 */
#ifndef _ALPHA_
/* 2358 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 2360 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2362 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2364 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter lHint */

/* 2366 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 2368 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2370 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter varHintData */

/* 2372 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 2374 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2376 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 2378 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2380 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 2382 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnXXXAccess */

/* 2384 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2386 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2390 */	NdrFcShort( 0x47 ),	/* 71 */
#ifndef _ALPHA_
/* 2392 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 2394 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2396 */	NdrFcShort( 0xe ),	/* 14 */
/* 2398 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter lPerms */

/* 2400 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 2402 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2404 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter bReturnValue */

/* 2406 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 2408 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2410 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2412 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2414 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2416 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DoNamedGesture */


	/* Procedure Report */

/* 2418 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2420 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2424 */	NdrFcShort( 0xc ),	/* 12 */
#ifndef _ALPHA_
/* 2426 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2428 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2430 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2432 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter bstrGestureName */


	/* Parameter bstrReport */

/* 2434 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 2436 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2438 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Return value */


	/* Return value */

/* 2440 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2442 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2444 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SMchSetState */


	/* Procedure Announce */

/* 2446 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2448 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2452 */	NdrFcShort( 0xd ),	/* 13 */
#ifndef _ALPHA_
/* 2454 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2456 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2458 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2460 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter bstrState */


	/* Parameter bstrAnnounce */

/* 2462 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 2464 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2466 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Return value */


	/* Return value */

/* 2468 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2470 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2472 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnSystemDestroyClient */

/* 2474 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2476 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2480 */	NdrFcShort( 0xe ),	/* 14 */
#ifndef _ALPHA_
/* 2482 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2484 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2486 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2488 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pthing */

/* 2490 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 2492 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2494 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Return value */

/* 2496 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2498 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2500 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnContentSystemEnter */

/* 2502 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2504 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2508 */	NdrFcShort( 0xf ),	/* 15 */
#ifndef _ALPHA_
/* 2510 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 2512 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2514 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2516 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pWhere */

/* 2518 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 2520 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2522 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter pWhat */

/* 2524 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 2526 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2528 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Return value */

/* 2530 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2532 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2534 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnContentSystemLeave */

/* 2536 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2538 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2542 */	NdrFcShort( 0x10 ),	/* 16 */
#ifndef _ALPHA_
/* 2544 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 2546 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2548 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2550 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pOldWhere */

/* 2552 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 2554 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2556 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter pWhat */

/* 2558 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 2560 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2562 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Return value */

/* 2564 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2566 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2568 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure LookAt */

/* 2570 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2572 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2576 */	NdrFcShort( 0x11 ),	/* 17 */
#ifndef _ALPHA_
/* 2578 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2580 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2582 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2584 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */

/* 2586 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2588 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2590 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetMenuList */

/* 2592 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2594 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2598 */	NdrFcShort( 0x12 ),	/* 18 */
#ifndef _ALPHA_
/* 2600 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2602 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2604 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2606 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter pplistMenu */

/* 2608 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 2610 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2612 */	NdrFcShort( 0x3ca ),	/* Type Offset=970 */

	/* Return value */

/* 2614 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2616 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2618 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetMenuItems */

/* 2620 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2622 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2626 */	NdrFcShort( 0x13 ),	/* 19 */
#ifndef _ALPHA_
/* 2628 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 2630 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2632 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2634 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pTarget */

/* 2636 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 2638 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2640 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter ppMenuList */

/* 2642 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 2644 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2646 */	NdrFcShort( 0x3ca ),	/* Type Offset=970 */

	/* Return value */

/* 2648 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2650 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2652 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetPeerMenuItems */

/* 2654 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2656 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2660 */	NdrFcShort( 0x14 ),	/* 20 */
#ifndef _ALPHA_
/* 2662 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 2664 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2666 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2668 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pTarget */

/* 2670 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 2672 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2674 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter ppMenuList */

/* 2676 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 2678 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2680 */	NdrFcShort( 0x3ca ),	/* Type Offset=970 */

	/* Return value */

/* 2682 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2684 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2686 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetContainerMenuItems */

/* 2688 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2690 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2694 */	NdrFcShort( 0x15 ),	/* 21 */
#ifndef _ALPHA_
/* 2696 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 2698 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2700 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2702 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pTarget */

/* 2704 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 2706 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2708 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter ppMenuList */

/* 2710 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 2712 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2714 */	NdrFcShort( 0x3ca ),	/* Type Offset=970 */

	/* Return value */

/* 2716 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2718 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2720 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetContentMenuItems */

/* 2722 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2724 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2728 */	NdrFcShort( 0x16 ),	/* 22 */
#ifndef _ALPHA_
/* 2730 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 2732 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2734 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2736 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pTarget */

/* 2738 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 2740 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2742 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter ppMenuList */

/* 2744 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 2746 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2748 */	NdrFcShort( 0x3ca ),	/* Type Offset=970 */

	/* Return value */

/* 2750 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2752 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2754 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetSelfMenuItems */

/* 2756 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2758 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2762 */	NdrFcShort( 0x17 ),	/* 23 */
#ifndef _ALPHA_
/* 2764 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 2766 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2768 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2770 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pTarget */

/* 2772 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 2774 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2776 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter ppMenuList */

/* 2778 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 2780 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2782 */	NdrFcShort( 0x3ca ),	/* Type Offset=970 */

	/* Return value */

/* 2784 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2786 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2788 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetInventoryMenuItems */

/* 2790 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2792 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2796 */	NdrFcShort( 0x18 ),	/* 24 */
#ifndef _ALPHA_
/* 2798 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 2800 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2802 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2804 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pTarget */

/* 2806 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 2808 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2810 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter ppMenuList */

/* 2812 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 2814 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2816 */	NdrFcShort( 0x3ca ),	/* Type Offset=970 */

	/* Return value */

/* 2818 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2820 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2822 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetEditMenuItems */

/* 2824 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2826 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2830 */	NdrFcShort( 0x19 ),	/* 25 */
#ifndef _ALPHA_
/* 2832 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 2834 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2836 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2838 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pTarget */

/* 2840 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 2842 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2844 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter ppMenuList */

/* 2846 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 2848 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2850 */	NdrFcShort( 0x3ca ),	/* Type Offset=970 */

	/* Return value */

/* 2852 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2854 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2856 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetMenuItemsFromProperty */

/* 2858 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2860 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2864 */	NdrFcShort( 0x1a ),	/* 26 */
#ifndef _ALPHA_
/* 2866 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 2868 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2870 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2872 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrMenuListProperty */

/* 2874 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 2876 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2878 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter ppMenuList */

/* 2880 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 2882 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2884 */	NdrFcShort( 0x3ca ),	/* Type Offset=970 */

	/* Return value */

/* 2886 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2888 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2890 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure UpdateIsArtifact */

/* 2892 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2894 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2898 */	NdrFcShort( 0x1b ),	/* 27 */
#ifndef _ALPHA_
/* 2900 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2902 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2904 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2906 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pmi */

/* 2908 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 2910 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2912 */	NdrFcShort( 0x416 ),	/* Type Offset=1046 */

	/* Return value */

/* 2914 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2916 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2918 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure UpdateIsAvatar */

/* 2920 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2922 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2926 */	NdrFcShort( 0x1c ),	/* 28 */
#ifndef _ALPHA_
/* 2928 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2930 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2932 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2934 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pmi */

/* 2936 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 2938 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2940 */	NdrFcShort( 0x416 ),	/* Type Offset=1046 */

	/* Return value */

/* 2942 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2944 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2946 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure UpdateIsAvatarAndNotWizard */

/* 2948 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2950 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2954 */	NdrFcShort( 0x1d ),	/* 29 */
#ifndef _ALPHA_
/* 2956 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2958 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2960 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2962 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pmi */

/* 2964 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 2966 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2968 */	NdrFcShort( 0x416 ),	/* Type Offset=1046 */

	/* Return value */

/* 2970 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 2972 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 2974 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure UpdateIsArtifactOrAvatar */

/* 2976 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2978 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2982 */	NdrFcShort( 0x1e ),	/* 30 */
#ifndef _ALPHA_
/* 2984 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 2986 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2988 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2990 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pmi */

/* 2992 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 2994 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 2996 */	NdrFcShort( 0x416 ),	/* Type Offset=1046 */

	/* Return value */

/* 2998 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3000 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3002 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure UpdateIsTakeable */

/* 3004 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3006 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3010 */	NdrFcShort( 0x1f ),	/* 31 */
#ifndef _ALPHA_
/* 3012 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3014 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3016 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3018 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pmi */

/* 3020 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 3022 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3024 */	NdrFcShort( 0x416 ),	/* Type Offset=1046 */

	/* Return value */

/* 3026 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3028 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3030 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure UpdatePropertiesMenuItem */

/* 3032 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3034 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3038 */	NdrFcShort( 0x20 ),	/* 32 */
#ifndef _ALPHA_
/* 3040 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3042 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3044 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3046 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pmi */

/* 3048 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 3050 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3052 */	NdrFcShort( 0x416 ),	/* Type Offset=1046 */

	/* Return value */

/* 3054 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3056 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3058 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure UpdateMenuItemName */

/* 3060 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3062 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3066 */	NdrFcShort( 0x21 ),	/* 33 */
#ifndef _ALPHA_
/* 3068 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3070 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3072 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3074 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pmi */

/* 3076 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 3078 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3080 */	NdrFcShort( 0x416 ),	/* Type Offset=1046 */

	/* Return value */

/* 3082 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3084 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3086 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure UpdatePropertyListMenu */

/* 3088 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3090 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3094 */	NdrFcShort( 0x22 ),	/* 34 */
#ifndef _ALPHA_
/* 3096 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 3098 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3100 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3102 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pmi */

/* 3104 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 3106 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3108 */	NdrFcShort( 0x416 ),	/* Type Offset=1046 */

	/* Parameter pList */

/* 3110 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 3112 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3114 */	NdrFcShort( 0x3ce ),	/* Type Offset=974 */

	/* Return value */

/* 3116 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3118 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3120 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure UpdateInventoryMenu */

/* 3122 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3124 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3128 */	NdrFcShort( 0x23 ),	/* 35 */
#ifndef _ALPHA_
/* 3130 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3132 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3134 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3136 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pmi */

/* 3138 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 3140 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3142 */	NdrFcShort( 0x416 ),	/* Type Offset=1046 */

	/* Return value */

/* 3144 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3146 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3148 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure UpdateContentsMenu */

/* 3150 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3152 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3156 */	NdrFcShort( 0x24 ),	/* 36 */
#ifndef _ALPHA_
/* 3158 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3160 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3162 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3164 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pmi */

/* 3166 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 3168 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3170 */	NdrFcShort( 0x416 ),	/* Type Offset=1046 */

	/* Return value */

/* 3172 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3174 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3176 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure UpdateRoomAvatarsMenu */

/* 3178 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3180 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3184 */	NdrFcShort( 0x25 ),	/* 37 */
#ifndef _ALPHA_
/* 3186 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3188 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3190 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3192 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pmi */

/* 3194 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 3196 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3198 */	NdrFcShort( 0x416 ),	/* Type Offset=1046 */

	/* Return value */

/* 3200 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3202 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3204 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DoGesture */

/* 3206 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3208 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3212 */	NdrFcShort( 0x26 ),	/* 38 */
#ifndef _ALPHA_
/* 3214 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3216 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3218 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3220 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter iGesture */

/* 3222 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 3224 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3226 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3228 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3230 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3232 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DoNamedGesture */

/* 3234 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3236 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3240 */	NdrFcShort( 0x27 ),	/* 39 */
#ifndef _ALPHA_
/* 3242 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3244 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3246 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3248 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter bstrGestureName */

/* 3250 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 3252 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3254 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Return value */

/* 3256 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3258 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3260 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DoGestureServer */

/* 3262 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3264 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3268 */	NdrFcShort( 0x28 ),	/* 40 */
#ifndef _ALPHA_
/* 3270 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 3272 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3274 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3276 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pThingOriginator */

/* 3278 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 3280 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3282 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter iGesture */

/* 3284 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 3286 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3288 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3290 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3292 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3294 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DoGestureClient */

/* 3296 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3298 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3302 */	NdrFcShort( 0x29 ),	/* 41 */
#ifndef _ALPHA_
/* 3304 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 3306 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3308 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3310 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pThingOriginator */

/* 3312 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 3314 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3316 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter iGesture */

/* 3318 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 3320 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3322 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3324 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3326 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3328 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AllowEnter */

/* 3330 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3332 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3336 */	NdrFcShort( 0x2a ),	/* 42 */
#ifndef _ALPHA_
/* 3338 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 3340 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3342 */	NdrFcShort( 0xe ),	/* 14 */
/* 3344 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pThing */

/* 3346 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 3348 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3350 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter pbool */

/* 3352 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 3354 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3356 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 3358 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3360 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3362 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnSystemPreDestroy */


	/* Procedure AllowLeave */

/* 3364 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3366 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3370 */	NdrFcShort( 0x2b ),	/* 43 */
#ifndef _ALPHA_
/* 3372 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 3374 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3376 */	NdrFcShort( 0xe ),	/* 14 */
/* 3378 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pthing */


	/* Parameter pThing */

/* 3380 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 3382 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3384 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter pbOkToDestroy */


	/* Parameter pbool */

/* 3386 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 3388 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3390 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 3392 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3394 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3396 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AcceptEnter */

/* 3398 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3400 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3404 */	NdrFcShort( 0x2c ),	/* 44 */
#ifndef _ALPHA_
/* 3406 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 3408 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3410 */	NdrFcShort( 0xe ),	/* 14 */
/* 3412 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pThing */

/* 3414 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 3416 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3418 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter pbool */

/* 3420 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 3422 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3424 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 3426 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3428 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3430 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AcceptLeave */

/* 3432 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3434 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3438 */	NdrFcShort( 0x2d ),	/* 45 */
#ifndef _ALPHA_
/* 3440 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3442 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3444 */	NdrFcShort( 0xe ),	/* 14 */
/* 3446 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter pbool */

/* 3448 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 3450 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3452 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 3454 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3456 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3458 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Clone */

/* 3460 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3462 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3466 */	NdrFcShort( 0x2e ),	/* 46 */
#ifndef _ALPHA_
/* 3468 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3470 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3472 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3474 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter ppNewThing */

/* 3476 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 3478 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3480 */	NdrFcShort( 0x4c8 ),	/* Type Offset=1224 */

	/* Return value */

/* 3482 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3484 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3486 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Emote */

/* 3488 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3490 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3494 */	NdrFcShort( 0x2f ),	/* 47 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 3496 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
#else
			NdrFcShort( 0x2c ),	/* MIPS & PPC Stack size/offset = 44 */
#endif
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 3498 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3500 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3502 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter varGesture */

/* 3504 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 3506 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3508 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter varText */

/* 3510 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 3512 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3514 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 3516 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 3518 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
#else
			NdrFcShort( 0x28 ),	/* MIPS & PPC Stack size/offset = 40 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 3520 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GestureText */

/* 3522 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3524 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3528 */	NdrFcShort( 0x30 ),	/* 48 */
#ifndef _ALPHA_
/* 3530 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 3532 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3534 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3536 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrGesture */

/* 3538 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 3540 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3542 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter pbstrText */

/* 3544 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 3546 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3548 */	NdrFcShort( 0x446 ),	/* Type Offset=1094 */

	/* Return value */

/* 3550 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3552 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3554 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure MoveInto */

/* 3556 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3558 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3562 */	NdrFcShort( 0x31 ),	/* 49 */
#ifndef _ALPHA_
/* 3564 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3566 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3568 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3570 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pNewContainer */

/* 3572 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 3574 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3576 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Return value */

/* 3578 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3580 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3582 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Say */

/* 3584 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3586 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3590 */	NdrFcShort( 0x32 ),	/* 50 */
#ifndef _ALPHA_
/* 3592 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 3594 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3596 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3598 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter strText */

/* 3600 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 3602 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3604 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter userOrList */

/* 3606 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 3608 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3610 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 3612 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3614 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 3616 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Shout */

/* 3618 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3620 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3624 */	NdrFcShort( 0x33 ),	/* 51 */
#ifndef _ALPHA_
/* 3626 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3628 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3630 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3632 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter strText */

/* 3634 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 3636 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3638 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Return value */

/* 3640 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3642 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3644 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SayGesture */

/* 3646 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3648 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3652 */	NdrFcShort( 0x34 ),	/* 52 */
#ifndef _ALPHA_
/* 3654 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3656 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3658 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3660 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter strText */

/* 3662 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 3664 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3666 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Return value */

/* 3668 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3670 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3672 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Whisper */

/* 3674 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3676 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3680 */	NdrFcShort( 0x35 ),	/* 53 */
#ifndef _ALPHA_
/* 3682 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 3684 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3686 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3688 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter strText */

/* 3690 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 3692 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3694 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter userOrList */

/* 3696 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 3698 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3700 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 3702 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3704 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 3706 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Take */

/* 3708 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3710 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3714 */	NdrFcShort( 0x36 ),	/* 54 */
#ifndef _ALPHA_
/* 3716 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3718 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3720 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3722 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pThing */

/* 3724 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 3726 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3728 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Return value */

/* 3730 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3732 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3734 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Drop */

/* 3736 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3738 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3742 */	NdrFcShort( 0x37 ),	/* 55 */
#ifndef _ALPHA_
/* 3744 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3746 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3748 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3750 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pThing */

/* 3752 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 3754 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3756 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Return value */

/* 3758 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3760 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3762 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnIsOpenChanged */

/* 3764 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3766 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3770 */	NdrFcShort( 0x38 ),	/* 56 */
#ifndef _ALPHA_
/* 3772 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 3774 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3776 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3778 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter lHint */

/* 3780 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 3782 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3784 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter varHintData */

/* 3786 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 3788 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3790 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 3792 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3794 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 3796 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure EditObject */

/* 3798 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3800 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3804 */	NdrFcShort( 0x39 ),	/* 57 */
#ifndef _ALPHA_
/* 3806 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3808 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3810 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3812 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */

/* 3814 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3816 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3818 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetEditTabMap */

/* 3820 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3822 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3826 */	NdrFcShort( 0x3a ),	/* 58 */
#ifndef _ALPHA_
/* 3828 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3830 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3832 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3834 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter pMap */

/* 3836 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 3838 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3840 */	NdrFcShort( 0x3ca ),	/* Type Offset=970 */

	/* Return value */

/* 3842 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3844 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3846 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetSortedEditTabMap */

/* 3848 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3850 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3854 */	NdrFcShort( 0x3b ),	/* 59 */
#ifndef _ALPHA_
/* 3856 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3858 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3860 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3862 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter pMap */

/* 3864 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 3866 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3868 */	NdrFcShort( 0x3ca ),	/* Type Offset=970 */

	/* Return value */

/* 3870 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3872 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3874 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddClientProperty */

/* 3876 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3878 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3882 */	NdrFcShort( 0x3c ),	/* 60 */
#ifndef _ALPHA_
/* 3884 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 3886 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3888 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3890 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrName */

/* 3892 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 3894 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3896 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter var */

/* 3898 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 3900 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3902 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 3904 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3906 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 3908 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RemoveClientProperty */

/* 3910 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3912 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3916 */	NdrFcShort( 0x3d ),	/* 61 */
#ifndef _ALPHA_
/* 3918 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3920 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3922 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3924 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter bstrName */

/* 3926 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 3928 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3930 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Return value */

/* 3932 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3934 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3936 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnIsNoticeableAccess */

/* 3938 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3940 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3944 */	NdrFcShort( 0x3e ),	/* 62 */
#ifndef _ALPHA_
/* 3946 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 3948 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3950 */	NdrFcShort( 0xe ),	/* 14 */
/* 3952 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter lPerms */

/* 3954 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 3956 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3958 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter bReturnValue */

/* 3960 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 3962 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3964 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 3966 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 3968 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 3970 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnNameAccess */

/* 3972 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3974 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3978 */	NdrFcShort( 0x3f ),	/* 63 */
#ifndef _ALPHA_
/* 3980 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 3982 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3984 */	NdrFcShort( 0xe ),	/* 14 */
/* 3986 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter lPerms */

/* 3988 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 3990 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 3992 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter bReturnValue */

/* 3994 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 3996 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 3998 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 4000 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4002 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4004 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnOwnerOrWizardAccess */

/* 4006 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4008 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4012 */	NdrFcShort( 0x40 ),	/* 64 */
#ifndef _ALPHA_
/* 4014 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 4016 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4018 */	NdrFcShort( 0xe ),	/* 14 */
/* 4020 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter lPerms */

/* 4022 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 4024 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4026 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter bReturnValue */

/* 4028 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 4030 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4032 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 4034 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4036 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4038 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Show */


	/* Procedure ShowTransportDialog */


	/* Procedure WieldItem */

/* 4040 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4042 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4046 */	NdrFcShort( 0xc ),	/* 12 */
#ifndef _ALPHA_
/* 4048 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4050 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4052 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4054 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */


	/* Return value */


	/* Return value */

/* 4056 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4058 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4060 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure UpdateWield */

/* 4062 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4064 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4068 */	NdrFcShort( 0xd ),	/* 13 */
#ifndef _ALPHA_
/* 4070 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4072 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4074 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4076 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pmi */

/* 4078 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 4080 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4082 */	NdrFcShort( 0x416 ),	/* Type Offset=1046 */

	/* Return value */

/* 4084 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4086 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4088 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure LookAt */


	/* Procedure UnWieldItem */

/* 4090 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4092 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4096 */	NdrFcShort( 0xe ),	/* 14 */
#ifndef _ALPHA_
/* 4098 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4100 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4102 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4104 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */


	/* Return value */

/* 4106 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4108 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4110 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure UpdateUnWield */

/* 4112 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4114 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4118 */	NdrFcShort( 0xf ),	/* 15 */
#ifndef _ALPHA_
/* 4120 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4122 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4124 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4126 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pmi */

/* 4128 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 4130 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4132 */	NdrFcShort( 0x416 ),	/* Type Offset=1046 */

	/* Return value */

/* 4134 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4136 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4138 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AcceptLeave */

/* 4140 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4142 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4146 */	NdrFcShort( 0x10 ),	/* 16 */
#ifndef _ALPHA_
/* 4148 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4150 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4152 */	NdrFcShort( 0xe ),	/* 14 */
/* 4154 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter pbool */

/* 4156 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 4158 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4160 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 4162 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4164 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4166 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AcceptTransport */


	/* Procedure AcceptEnter */

/* 4168 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4170 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4174 */	NdrFcShort( 0x11 ),	/* 17 */
#ifndef _ALPHA_
/* 4176 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 4178 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4180 */	NdrFcShort( 0xe ),	/* 14 */
/* 4182 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pThing */


	/* Parameter pThing */

/* 4184 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 4186 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4188 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter pbool */


	/* Parameter pbool */

/* 4190 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 4192 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4194 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 4196 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4198 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4200 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GestureText */

/* 4202 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4204 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4208 */	NdrFcShort( 0xd ),	/* 13 */
#ifndef _ALPHA_
/* 4210 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 4212 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4214 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4216 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrGesture */

/* 4218 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4220 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4222 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter pbstrText */

/* 4224 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 4226 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4228 */	NdrFcShort( 0x446 ),	/* Type Offset=1094 */

	/* Return value */

/* 4230 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4232 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4234 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure WhisperTo */

/* 4236 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4238 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4242 */	NdrFcShort( 0xf ),	/* 15 */
#ifndef _ALPHA_
/* 4244 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4246 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4248 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4250 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */

/* 4252 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4254 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4256 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure UpdateUserProfileList */

/* 4258 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4260 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4264 */	NdrFcShort( 0x10 ),	/* 16 */
#ifndef _ALPHA_
/* 4266 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4268 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4270 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4272 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pmi */

/* 4274 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 4276 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4278 */	NdrFcShort( 0x416 ),	/* Type Offset=1046 */

	/* Return value */

/* 4280 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4282 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4284 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure MenuUserProfileList */

/* 4286 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4288 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4292 */	NdrFcShort( 0x11 ),	/* 17 */
#ifndef _ALPHA_
/* 4294 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4296 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4298 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4300 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter bstrKey */

/* 4302 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4304 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4306 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Return value */

/* 4308 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4310 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4312 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddToUserProfileList */

/* 4314 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4316 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4320 */	NdrFcShort( 0x12 ),	/* 18 */
#ifndef _ALPHA_
/* 4322 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 4324 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4326 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4328 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pThing */

/* 4330 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 4332 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4334 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter bstrKey */

/* 4336 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4338 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4340 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Return value */

/* 4342 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4344 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4346 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RemoveFromUserProfileList */

/* 4348 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4350 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4354 */	NdrFcShort( 0x13 ),	/* 19 */
#ifndef _ALPHA_
/* 4356 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 4358 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4360 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4362 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pThing */

/* 4364 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 4366 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4368 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter bstrKey */

/* 4370 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4372 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4374 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Return value */

/* 4376 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4378 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4380 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure IsInUserProfileList */

/* 4382 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4384 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4388 */	NdrFcShort( 0x14 ),	/* 20 */
#ifndef _ALPHA_
/* 4390 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 4392 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4394 */	NdrFcShort( 0xe ),	/* 14 */
/* 4396 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter pThing */

/* 4398 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 4400 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4402 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter bstrKey */

/* 4404 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4406 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4408 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter pBool */

/* 4410 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 4412 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4414 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 4416 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4418 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 4420 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure UpdateMenuItemName */

/* 4422 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4424 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4428 */	NdrFcShort( 0x15 ),	/* 21 */
#ifndef _ALPHA_
/* 4430 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4432 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4434 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4436 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pmi */

/* 4438 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 4440 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4442 */	NdrFcShort( 0x416 ),	/* Type Offset=1046 */

	/* Return value */

/* 4444 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4446 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4448 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure UpdateWhisperMenuItem */

/* 4450 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4452 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4456 */	NdrFcShort( 0x16 ),	/* 22 */
#ifndef _ALPHA_
/* 4458 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4460 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4462 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4464 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pmi */

/* 4466 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 4468 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4470 */	NdrFcShort( 0x416 ),	/* Type Offset=1046 */

	/* Return value */

/* 4472 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4474 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4476 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure EditProfile */

/* 4478 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4480 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4484 */	NdrFcShort( 0x17 ),	/* 23 */
#ifndef _ALPHA_
/* 4486 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4488 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4490 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4492 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */

/* 4494 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4496 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4498 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Gesture */

/* 4500 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4502 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4506 */	NdrFcShort( 0x18 ),	/* 24 */
#ifndef _ALPHA_
/* 4508 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4510 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4512 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4514 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter strGestureName */

/* 4516 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4518 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4520 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Return value */

/* 4522 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4524 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4526 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure UnWield */

/* 4528 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4530 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4534 */	NdrFcShort( 0x1a ),	/* 26 */
#ifndef _ALPHA_
/* 4536 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4538 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4540 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4542 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */

/* 4544 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4546 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4548 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Teleport */

/* 4550 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4552 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4556 */	NdrFcShort( 0x1b ),	/* 27 */
#ifndef _ALPHA_
/* 4558 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4560 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4562 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4564 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pNewRoom */

/* 4566 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 4568 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4570 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Return value */

/* 4572 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4574 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4576 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Query */

/* 4578 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4580 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4584 */	NdrFcShort( 0x1c ),	/* 28 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 4586 */	NdrFcShort( 0x48 ),	/* x86 Stack size/offset = 72 */
#else
			NdrFcShort( 0x4c ),	/* MIPS & PPC Stack size/offset = 76 */
#endif
#else
			NdrFcShort( 0x60 ),	/* Alpha Stack size/offset = 96 */
#endif
/* 4588 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4590 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4592 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x8,		/* 8 */

	/* Parameter bstrTitle */

/* 4594 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4596 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4598 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrHTML */

/* 4600 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4602 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4604 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrPrompt */

/* 4606 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4608 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4610 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrDefault */

/* 4612 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4614 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 4616 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter varNotifyObject */

/* 4618 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 4620 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 4622 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter varNotifyMethod */

/* 4624 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 4626 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
#else
			NdrFcShort( 0x28 ),	/* MIPS & PPC Stack size/offset = 40 */
#endif
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 4628 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter varNotifyParam */

/* 4630 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 4632 */	NdrFcShort( 0x34 ),	/* x86 Stack size/offset = 52 */
#else
			NdrFcShort( 0x38 ),	/* MIPS & PPC Stack size/offset = 56 */
#endif
#else
			NdrFcShort( 0x48 ),	/* Alpha Stack size/offset = 72 */
#endif
/* 4634 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 4636 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 4638 */	NdrFcShort( 0x44 ),	/* x86 Stack size/offset = 68 */
#else
			NdrFcShort( 0x48 ),	/* MIPS & PPC Stack size/offset = 72 */
#endif
#else
			NdrFcShort( 0x58 ),	/* Alpha Stack size/offset = 88 */
#endif
/* 4640 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Alert */

/* 4642 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4644 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4648 */	NdrFcShort( 0x1d ),	/* 29 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 4650 */	NdrFcShort( 0x40 ),	/* x86 Stack size/offset = 64 */
#else
			NdrFcShort( 0x44 ),	/* MIPS & PPC Stack size/offset = 68 */
#endif
#else
			NdrFcShort( 0x50 ),	/* Alpha Stack size/offset = 80 */
#endif
/* 4652 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4654 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4656 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x6,		/* 6 */

	/* Parameter bstrTitle */

/* 4658 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4660 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4662 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrText */

/* 4664 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4666 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4668 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter varNotifyObject */

/* 4670 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 4672 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
#else
			NdrFcShort( 0x10 ),	/* MIPS & PPC Stack size/offset = 16 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4674 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter varNotifyMethod */

/* 4676 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 4678 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 4680 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter varNotifyParam */

/* 4682 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 4684 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
#else
			NdrFcShort( 0x30 ),	/* MIPS & PPC Stack size/offset = 48 */
#endif
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 4686 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 4688 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 4690 */	NdrFcShort( 0x3c ),	/* x86 Stack size/offset = 60 */
#else
			NdrFcShort( 0x40 ),	/* MIPS & PPC Stack size/offset = 64 */
#endif
#else
			NdrFcShort( 0x48 ),	/* Alpha Stack size/offset = 72 */
#endif
/* 4692 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Confirm */

/* 4694 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4696 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4700 */	NdrFcShort( 0x1e ),	/* 30 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 4702 */	NdrFcShort( 0x40 ),	/* x86 Stack size/offset = 64 */
#else
			NdrFcShort( 0x44 ),	/* MIPS & PPC Stack size/offset = 68 */
#endif
#else
			NdrFcShort( 0x50 ),	/* Alpha Stack size/offset = 80 */
#endif
/* 4704 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4706 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4708 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x6,		/* 6 */

	/* Parameter bstrTitle */

/* 4710 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4712 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4714 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrText */

/* 4716 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4718 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4720 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter varNotifyObject */

/* 4722 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 4724 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
#else
			NdrFcShort( 0x10 ),	/* MIPS & PPC Stack size/offset = 16 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4726 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter varNotifyMethod */

/* 4728 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 4730 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 4732 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter varNotifyParam */

/* 4734 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 4736 */	NdrFcShort( 0x2c ),	/* x86 Stack size/offset = 44 */
#else
			NdrFcShort( 0x30 ),	/* MIPS & PPC Stack size/offset = 48 */
#endif
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 4738 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 4740 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 4742 */	NdrFcShort( 0x3c ),	/* x86 Stack size/offset = 60 */
#else
			NdrFcShort( 0x40 ),	/* MIPS & PPC Stack size/offset = 64 */
#endif
#else
			NdrFcShort( 0x48 ),	/* Alpha Stack size/offset = 72 */
#endif
/* 4744 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Prompt */

/* 4746 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4748 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4752 */	NdrFcShort( 0x1f ),	/* 31 */
#ifndef _ALPHA_
/* 4754 */	NdrFcShort( 0x44 ),	/* x86, MIPS, PPC Stack size/offset = 68 */
#else
			NdrFcShort( 0x58 ),	/* Alpha Stack size/offset = 88 */
#endif
/* 4756 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4758 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4760 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x7,		/* 7 */

	/* Parameter bstrTitle */

/* 4762 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4764 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4766 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrPrompt */

/* 4768 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4770 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4772 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrDefault */

/* 4774 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4776 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4778 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter varNotifyObject */

/* 4780 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4782 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 4784 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter varNotifyMethod */

/* 4786 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4788 */	NdrFcShort( 0x20 ),	/* x86, MIPS, PPC Stack size/offset = 32 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 4790 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Parameter varNotifyParam */

/* 4792 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4794 */	NdrFcShort( 0x30 ),	/* x86, MIPS, PPC Stack size/offset = 48 */
#else
			NdrFcShort( 0x40 ),	/* Alpha Stack size/offset = 64 */
#endif
/* 4796 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 4798 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4800 */	NdrFcShort( 0x40 ),	/* x86, MIPS, PPC Stack size/offset = 64 */
#else
			NdrFcShort( 0x50 ),	/* Alpha Stack size/offset = 80 */
#endif
/* 4802 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ClientQuery */

/* 4804 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4806 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4810 */	NdrFcShort( 0x20 ),	/* 32 */
#ifndef _ALPHA_
/* 4812 */	NdrFcShort( 0x24 ),	/* x86, MIPS, PPC Stack size/offset = 36 */
#else
			NdrFcShort( 0x48 ),	/* Alpha Stack size/offset = 72 */
#endif
/* 4814 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4816 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4818 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x8,		/* 8 */

	/* Parameter bstrTitle */

/* 4820 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4822 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4824 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrHTML */

/* 4826 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4828 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4830 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrPrompt */

/* 4832 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4834 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4836 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrDefault */

/* 4838 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4840 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 4842 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter pNotifyObject */

/* 4844 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 4846 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 4848 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter bstrNotifyMethod */

/* 4850 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4852 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 4854 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter pNotifyParam */

/* 4856 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 4858 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 4860 */	NdrFcShort( 0x146 ),	/* Type Offset=326 */

	/* Return value */

/* 4862 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4864 */	NdrFcShort( 0x20 ),	/* x86, MIPS, PPC Stack size/offset = 32 */
#else
			NdrFcShort( 0x40 ),	/* Alpha Stack size/offset = 64 */
#endif
/* 4866 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure QueryReturn */

/* 4868 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4870 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4874 */	NdrFcShort( 0x21 ),	/* 33 */
#ifndef _ALPHA_
/* 4876 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 4878 */	NdrFcShort( 0x6 ),	/* 6 */
/* 4880 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4882 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter pArg */

/* 4884 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 4886 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4888 */	NdrFcShort( 0x3ce ),	/* Type Offset=974 */

	/* Parameter boolOK */

/* 4890 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 4892 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4894 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter bstrResult */

/* 4896 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 4898 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4900 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Return value */

/* 4902 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4904 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 4906 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnSystemCreate */

/* 4908 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4910 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4914 */	NdrFcShort( 0x22 ),	/* 34 */
#ifndef _ALPHA_
/* 4916 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4918 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4920 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4922 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pthing */

/* 4924 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 4926 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4928 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Return value */

/* 4930 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4932 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4934 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnSystemConnect */

/* 4936 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4938 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4942 */	NdrFcShort( 0x23 ),	/* 35 */
#ifndef _ALPHA_
/* 4944 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4946 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4948 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4950 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pWho */

/* 4952 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 4954 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4956 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Return value */

/* 4958 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4960 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4962 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnSystemDisconnect */

/* 4964 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4966 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4970 */	NdrFcShort( 0x24 ),	/* 36 */
#ifndef _ALPHA_
/* 4972 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 4974 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4976 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4978 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pWho */

/* 4980 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 4982 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 4984 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Return value */

/* 4986 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 4988 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 4990 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ThumbnailFileName */

/* 4992 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4994 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4998 */	NdrFcShort( 0x25 ),	/* 37 */
#ifndef _ALPHA_
/* 5000 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5002 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5004 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5006 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter pbstrFileName */

/* 5008 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 5010 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5012 */	NdrFcShort( 0x446 ),	/* Type Offset=1094 */

	/* Return value */

/* 5014 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5016 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5018 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnContainerSystemTell */

/* 5020 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5022 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5026 */	NdrFcShort( 0x26 ),	/* 38 */
#ifndef _ALPHA_
/* 5028 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 5030 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5032 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5034 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x6,		/* 6 */

	/* Parameter pfrom */

/* 5036 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5038 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5040 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter pToList */

/* 5042 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5044 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5046 */	NdrFcShort( 0x3ce ),	/* Type Offset=974 */

	/* Parameter bstrSay */

/* 5048 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 5050 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5052 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter lval */

/* 5054 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 5056 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 5058 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pbstrOut */

/* 5060 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 5062 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 5064 */	NdrFcShort( 0x446 ),	/* Type Offset=1094 */

	/* Return value */

/* 5066 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5068 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 5070 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnSystemTell */

/* 5072 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5074 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5078 */	NdrFcShort( 0x27 ),	/* 39 */
#ifndef _ALPHA_
/* 5080 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 5082 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5084 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5086 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x6,		/* 6 */

	/* Parameter pfrom */

/* 5088 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5090 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5092 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter pToList */

/* 5094 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5096 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5098 */	NdrFcShort( 0x3ce ),	/* Type Offset=974 */

	/* Parameter bstrSay */

/* 5100 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 5102 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5104 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter lval */

/* 5106 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 5108 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 5110 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pbstrOut */

/* 5112 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 5114 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 5116 */	NdrFcShort( 0x446 ),	/* Type Offset=1094 */

	/* Return value */

/* 5118 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5120 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 5122 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnPeerSystemTell */

/* 5124 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5126 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5130 */	NdrFcShort( 0x28 ),	/* 40 */
#ifndef _ALPHA_
/* 5132 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 5134 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5136 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5138 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x5,		/* 5 */

	/* Parameter pfrom */

/* 5140 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5142 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5144 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter pToList */

/* 5146 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5148 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5150 */	NdrFcShort( 0x3ce ),	/* Type Offset=974 */

	/* Parameter bstrSay */

/* 5152 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 5154 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5156 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter lval */

/* 5158 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 5160 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 5162 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 5164 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5166 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 5168 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnSystemPreCreate */

/* 5170 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5172 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5176 */	NdrFcShort( 0x29 ),	/* 41 */
#ifndef _ALPHA_
/* 5178 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 5180 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5182 */	NdrFcShort( 0xe ),	/* 14 */
/* 5184 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pExemplar */

/* 5186 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5188 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5190 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter pbOkToCreate */

/* 5192 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 5194 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5196 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 5198 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5200 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5202 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnSystemPostCreate */

/* 5204 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5206 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5210 */	NdrFcShort( 0x2a ),	/* 42 */
#ifndef _ALPHA_
/* 5212 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5214 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5216 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5218 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pThing */

/* 5220 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5222 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5224 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Return value */

/* 5226 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5228 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5230 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnSystemPostDestroy */

/* 5232 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5234 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5238 */	NdrFcShort( 0x2c ),	/* 44 */
#ifndef _ALPHA_
/* 5240 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5242 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5244 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5246 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pthing */

/* 5248 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5250 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5252 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Return value */

/* 5254 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5256 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5258 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetObjectQuota */

/* 5260 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5262 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5266 */	NdrFcShort( 0x2d ),	/* 45 */
#ifndef _ALPHA_
/* 5268 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5270 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5272 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5274 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter lQuota */

/* 5276 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 5278 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5280 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 5282 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5284 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5286 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddDefaultKeys */

/* 5288 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5290 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5294 */	NdrFcShort( 0x2e ),	/* 46 */
#ifndef _ALPHA_
/* 5296 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5298 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5300 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5302 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */

/* 5304 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5306 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5308 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnAction */

/* 5310 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5312 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5316 */	NdrFcShort( 0x2f ),	/* 47 */
#ifndef _ALPHA_
/* 5318 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 5320 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5322 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5324 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter bstrAction */

/* 5326 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 5328 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5330 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrDescription */

/* 5332 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 5334 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5336 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter pParticipants */

/* 5338 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5340 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5342 */	NdrFcShort( 0x3ce ),	/* Type Offset=974 */

	/* Return value */

/* 5344 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5346 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 5348 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnIsAuthorChanged */

/* 5350 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5352 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5356 */	NdrFcShort( 0x30 ),	/* 48 */
#ifndef _ALPHA_
/* 5358 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 5360 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5362 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5364 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter lHint */

/* 5366 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 5368 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5370 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter varHintData */

/* 5372 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 5374 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5376 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 5378 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5380 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 5382 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnIsWizardOrIsAuthorAccess */

/* 5384 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5386 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5390 */	NdrFcShort( 0x31 ),	/* 49 */
#ifndef _ALPHA_
/* 5392 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 5394 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5396 */	NdrFcShort( 0xe ),	/* 14 */
/* 5398 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter lPerms */

/* 5400 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 5402 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5404 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter bReturnValue */

/* 5406 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 5408 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5410 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 5412 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5414 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5416 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnWizardAccess */

/* 5418 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5420 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5424 */	NdrFcShort( 0x32 ),	/* 50 */
#ifndef _ALPHA_
/* 5426 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 5428 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5430 */	NdrFcShort( 0xe ),	/* 14 */
/* 5432 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter lPerms */

/* 5434 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 5436 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5438 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter bReturnValue */

/* 5440 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 5442 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5444 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 5446 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5448 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5450 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure FireConnectAvatar */

/* 5452 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5454 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5458 */	NdrFcShort( 0x33 ),	/* 51 */
#ifndef _ALPHA_
/* 5460 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5462 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5464 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5466 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pthing */

/* 5468 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5470 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5472 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Return value */

/* 5474 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5476 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5478 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ShowLogOffDialog */

/* 5480 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5482 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5486 */	NdrFcShort( 0xc ),	/* 12 */
#ifndef _ALPHA_
/* 5488 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5490 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5492 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5494 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pTarget */

/* 5496 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5498 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5500 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Return value */

/* 5502 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5504 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5506 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure LogOff */

/* 5508 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5510 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5514 */	NdrFcShort( 0xd ),	/* 13 */
#ifndef _ALPHA_
/* 5516 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 5518 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5520 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5522 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pTarget */

/* 5524 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5526 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5528 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter bstrReason */

/* 5530 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 5532 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5534 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Return value */

/* 5536 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5538 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5540 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TransportMenu */

/* 5542 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5544 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5548 */	NdrFcShort( 0xd ),	/* 13 */
#ifndef _ALPHA_
/* 5550 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5552 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5554 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5556 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */

/* 5558 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5560 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5562 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Transport */

/* 5564 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5566 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5570 */	NdrFcShort( 0xe ),	/* 14 */
#ifndef _ALPHA_
/* 5572 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 5574 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5576 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5578 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pThing */

/* 5580 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5582 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5584 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter iIndex */

/* 5586 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 5588 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5590 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 5592 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5594 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5596 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TransportTo */

/* 5598 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5600 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5604 */	NdrFcShort( 0xf ),	/* 15 */
#ifndef _ALPHA_
/* 5606 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5608 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5610 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5612 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pThing */

/* 5614 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5616 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5618 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Return value */

/* 5620 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5622 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5624 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AllowLeave */


	/* Procedure AllowTransport */

/* 5626 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5628 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5632 */	NdrFcShort( 0x10 ),	/* 16 */
#ifndef _ALPHA_
/* 5634 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 5636 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5638 */	NdrFcShort( 0xe ),	/* 14 */
/* 5640 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pThing */


	/* Parameter pThing */

/* 5642 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5644 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5646 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter pbool */


	/* Parameter pbool */

/* 5648 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 5650 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5652 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 5654 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5656 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5658 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetDestinationByID */

/* 5660 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5662 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5666 */	NdrFcShort( 0x12 ),	/* 18 */
#ifndef _ALPHA_
/* 5668 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5670 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5672 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5674 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter lNewID */

/* 5676 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 5678 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5680 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 5682 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5684 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5686 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Transport */

/* 5688 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5690 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5694 */	NdrFcShort( 0xc ),	/* 12 */
#ifndef _ALPHA_
/* 5696 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 5698 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5700 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5702 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pThing */

/* 5704 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5706 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5708 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter iIndex */

/* 5710 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 5712 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5714 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 5716 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5718 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5720 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Expel */

/* 5722 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5724 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5728 */	NdrFcShort( 0xc ),	/* 12 */
#ifndef _ALPHA_
/* 5730 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 5732 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5734 */	NdrFcShort( 0xe ),	/* 14 */
/* 5736 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter pThing */

/* 5738 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5740 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5742 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter pDoor */

/* 5744 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5746 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5748 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter pBool */

/* 5750 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 5752 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5754 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 5756 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5758 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 5760 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TransmitWhisper */

/* 5762 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5764 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5768 */	NdrFcShort( 0xd ),	/* 13 */
#ifndef _ALPHA_
/* 5770 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 5772 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5774 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5776 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrMsg */

/* 5778 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 5780 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5782 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter varNameList */

/* 5784 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 5786 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5788 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 5790 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5792 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 5794 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ReceiveWhisper */

/* 5796 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5798 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5802 */	NdrFcShort( 0xe ),	/* 14 */
#ifndef _ALPHA_
/* 5804 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 5806 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5808 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5810 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrMsg */

/* 5812 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 5814 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5816 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter pTargetList */

/* 5818 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5820 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5822 */	NdrFcShort( 0x3ce ),	/* Type Offset=974 */

	/* Return value */

/* 5824 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5826 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5828 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AllowEnter */

/* 5830 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5832 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5836 */	NdrFcShort( 0xf ),	/* 15 */
#ifndef _ALPHA_
/* 5838 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 5840 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5842 */	NdrFcShort( 0xe ),	/* 14 */
/* 5844 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pThing */

/* 5846 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5848 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5850 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter pbool */

/* 5852 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 5854 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5856 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 5858 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5860 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5862 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnSystemCreate */

/* 5864 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5866 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5870 */	NdrFcShort( 0x11 ),	/* 17 */
#ifndef _ALPHA_
/* 5872 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5874 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5876 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5878 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pThing */

/* 5880 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5882 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5884 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Return value */

/* 5886 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5888 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5890 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnSystemDestroy */

/* 5892 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5894 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5898 */	NdrFcShort( 0x12 ),	/* 18 */
#ifndef _ALPHA_
/* 5900 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5902 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5904 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5906 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pThing */

/* 5908 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5910 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5912 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Return value */

/* 5914 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5916 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5918 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnContentSystemConnect */

/* 5920 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5922 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5926 */	NdrFcShort( 0x13 ),	/* 19 */
#ifndef _ALPHA_
/* 5928 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5930 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5932 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5934 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pWho */

/* 5936 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5938 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5940 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Return value */

/* 5942 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5944 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5946 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnContentSystemDisconnect */

/* 5948 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5950 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5954 */	NdrFcShort( 0x14 ),	/* 20 */
#ifndef _ALPHA_
/* 5956 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 5958 */	NdrFcShort( 0x0 ),	/* 0 */
/* 5960 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5962 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pWho */

/* 5964 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5966 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5968 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Return value */

/* 5970 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 5972 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 5974 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnContentSystemTell */

/* 5976 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 5978 */	NdrFcLong( 0x0 ),	/* 0 */
/* 5982 */	NdrFcShort( 0x15 ),	/* 21 */
#ifndef _ALPHA_
/* 5984 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 5986 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5988 */	NdrFcShort( 0x8 ),	/* 8 */
/* 5990 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x6,		/* 6 */

	/* Parameter pfrom */

/* 5992 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 5994 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 5996 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter pToList */

/* 5998 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 6000 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 6002 */	NdrFcShort( 0x3ce ),	/* Type Offset=974 */

	/* Parameter bstrSay */

/* 6004 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6006 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 6008 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter lval */

/* 6010 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 6012 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 6014 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pbstrOut */

/* 6016 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 6018 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 6020 */	NdrFcShort( 0x446 ),	/* Type Offset=1094 */

	/* Return value */

/* 6022 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 6024 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 6026 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnContentEnter */

/* 6028 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6030 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6034 */	NdrFcShort( 0x16 ),	/* 22 */
#ifndef _ALPHA_
/* 6036 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 6038 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6040 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6042 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pWhere */

/* 6044 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 6046 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 6048 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter pWhat */

/* 6050 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 6052 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 6054 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Return value */

/* 6056 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 6058 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 6060 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnContentLeave */

/* 6062 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6064 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6068 */	NdrFcShort( 0x17 ),	/* 23 */
#ifndef _ALPHA_
/* 6070 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 6072 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6074 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6076 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pOldWhere */

/* 6078 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 6080 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 6082 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Parameter pWhat */

/* 6084 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 6086 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 6088 */	NdrFcShort( 0x400 ),	/* Type Offset=1024 */

	/* Return value */

/* 6090 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 6092 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 6094 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SMchAddStateMachine */

/* 6096 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6098 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6102 */	NdrFcShort( 0xc ),	/* 12 */
#ifndef _ALPHA_
/* 6104 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 6106 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6108 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6110 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrStateMachineName */

/* 6112 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6114 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 6116 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrInitialValue */

/* 6118 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6120 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 6122 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Return value */

/* 6124 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 6126 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 6128 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SMchSetStateMachineState */

/* 6130 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6132 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6136 */	NdrFcShort( 0xe ),	/* 14 */
#ifndef _ALPHA_
/* 6138 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 6140 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6142 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6144 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrStateMachineName */

/* 6146 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6148 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 6150 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrState */

/* 6152 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6154 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 6156 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Return value */

/* 6158 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 6160 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 6162 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SMchIsInState */

/* 6164 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6166 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6170 */	NdrFcShort( 0xf ),	/* 15 */
#ifndef _ALPHA_
/* 6172 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 6174 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6176 */	NdrFcShort( 0xe ),	/* 14 */
/* 6178 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrState */

/* 6180 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6182 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 6184 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter pboolResult */

/* 6186 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 6188 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 6190 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 6192 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 6194 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 6196 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SMchIsStateMachineInState */

/* 6198 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6200 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6204 */	NdrFcShort( 0x10 ),	/* 16 */
#ifndef _ALPHA_
/* 6206 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 6208 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6210 */	NdrFcShort( 0xe ),	/* 14 */
/* 6212 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter bstrStateMachineName */

/* 6214 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6216 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 6218 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrState */

/* 6220 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6222 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 6224 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter pboolResult */

/* 6226 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 6228 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 6230 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 6232 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 6234 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 6236 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SMchSetIfState */

/* 6238 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6240 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6244 */	NdrFcShort( 0x11 ),	/* 17 */
#ifndef _ALPHA_
/* 6246 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 6248 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6250 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6252 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrOldState */

/* 6254 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6256 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 6258 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrNewState */

/* 6260 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6262 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 6264 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Return value */

/* 6266 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 6268 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 6270 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SMchSetStateMachineIfState */

/* 6272 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6274 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6278 */	NdrFcShort( 0x12 ),	/* 18 */
#ifndef _ALPHA_
/* 6280 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 6282 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6284 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6286 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter bstrStateMachineName */

/* 6288 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6290 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 6292 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrOldState */

/* 6294 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6296 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 6298 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrNewState */

/* 6300 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6302 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 6304 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Return value */

/* 6306 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 6308 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 6310 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SMchGetStateConcatName */

/* 6312 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6314 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6318 */	NdrFcShort( 0x13 ),	/* 19 */
#ifndef _ALPHA_
/* 6320 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 6322 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6324 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6326 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x5,		/* 5 */

	/* Parameter bstrStateMachineName */

/* 6328 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6330 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 6332 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrState */

/* 6334 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6336 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 6338 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrName */

/* 6340 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6342 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 6344 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter pbstrOut */

/* 6346 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 6348 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 6350 */	NdrFcShort( 0x446 ),	/* Type Offset=1094 */

	/* Return value */

/* 6352 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 6354 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 6356 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SMchGetCurrentStateConcatName */

/* 6358 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6360 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6364 */	NdrFcShort( 0x14 ),	/* 20 */
#ifndef _ALPHA_
/* 6366 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 6368 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6370 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6372 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter bstrStateMachineName */

/* 6374 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6376 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 6378 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrName */

/* 6380 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6382 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 6384 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter pbstrOut */

/* 6386 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 6388 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 6390 */	NdrFcShort( 0x446 ),	/* Type Offset=1094 */

	/* Return value */

/* 6392 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 6394 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 6396 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SMchGetStateProperty */

/* 6398 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6400 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6404 */	NdrFcShort( 0x15 ),	/* 21 */
#ifndef _ALPHA_
/* 6406 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 6408 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6410 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6412 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x5,		/* 5 */

	/* Parameter bstrStateMachineName */

/* 6414 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6416 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 6418 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrState */

/* 6420 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6422 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 6424 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrPropertyName */

/* 6426 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6428 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 6430 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter var */

/* 6432 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
#ifndef _ALPHA_
/* 6434 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 6436 */	NdrFcShort( 0x4d4 ),	/* Type Offset=1236 */

	/* Return value */

/* 6438 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 6440 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 6442 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SMchPutStateProperty */

/* 6444 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6446 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6450 */	NdrFcShort( 0x16 ),	/* 22 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 6452 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
#else
			NdrFcShort( 0x24 ),	/* MIPS & PPC Stack size/offset = 36 */
#endif
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 6454 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6456 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6458 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter bstrState */

/* 6460 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6462 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 6464 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrPropertyName */

/* 6466 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6468 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 6470 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter var */

/* 6472 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 6474 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
#else
			NdrFcShort( 0x10 ),	/* MIPS & PPC Stack size/offset = 16 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 6476 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 6478 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 6480 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 6482 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SMchPutStateMachineStateProperty */

/* 6484 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6486 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6490 */	NdrFcShort( 0x17 ),	/* 23 */
#ifndef _ALPHA_
/* 6492 */	NdrFcShort( 0x24 ),	/* x86, MIPS, PPC Stack size/offset = 36 */
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 6494 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6496 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6498 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x5,		/* 5 */

	/* Parameter bstrStateMachineName */

/* 6500 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6502 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 6504 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrState */

/* 6506 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6508 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 6510 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrPropertyName */

/* 6512 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6514 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 6516 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter var */

/* 6518 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6520 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 6522 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 6524 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 6526 */	NdrFcShort( 0x20 ),	/* x86, MIPS, PPC Stack size/offset = 32 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 6528 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SMchPutInlineStateMethod */

/* 6530 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6532 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6536 */	NdrFcShort( 0x18 ),	/* 24 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 6538 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
#else
			NdrFcShort( 0x2c ),	/* MIPS & PPC Stack size/offset = 44 */
#endif
#else
			NdrFcShort( 0x40 ),	/* Alpha Stack size/offset = 64 */
#endif
/* 6540 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6542 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6544 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x6,		/* 6 */

	/* Parameter lFlags */

/* 6546 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 6548 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 6550 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter bstrState */

/* 6552 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6554 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 6556 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrMethodName */

/* 6558 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6560 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 6562 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrCode */

/* 6564 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6566 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 6568 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter varParams */

/* 6570 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 6572 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 6574 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 6576 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 6578 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
#else
			NdrFcShort( 0x28 ),	/* MIPS & PPC Stack size/offset = 40 */
#endif
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 6580 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SMchPutStateMachineInlineStateMethod */

/* 6582 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6584 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6588 */	NdrFcShort( 0x19 ),	/* 25 */
#ifndef _ALPHA_
/* 6590 */	NdrFcShort( 0x2c ),	/* x86, MIPS, PPC Stack size/offset = 44 */
#else
			NdrFcShort( 0x48 ),	/* Alpha Stack size/offset = 72 */
#endif
/* 6592 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6594 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6596 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x7,		/* 7 */

	/* Parameter lFlags */

/* 6598 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 6600 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 6602 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter bstrStateMachineName */

/* 6604 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6606 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 6608 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrState */

/* 6610 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6612 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 6614 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrMethodName */

/* 6616 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6618 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 6620 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrCode */

/* 6622 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6624 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 6626 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter varParams */

/* 6628 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6630 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 6632 */	NdrFcShort( 0x3c0 ),	/* Type Offset=960 */

	/* Return value */

/* 6634 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 6636 */	NdrFcShort( 0x28 ),	/* x86, MIPS, PPC Stack size/offset = 40 */
#else
			NdrFcShort( 0x40 ),	/* Alpha Stack size/offset = 64 */
#endif
/* 6638 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SMchGetInlineStateMethod */

/* 6640 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6642 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6646 */	NdrFcShort( 0x1a ),	/* 26 */
#ifndef _ALPHA_
/* 6648 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 6650 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6652 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6654 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x5,		/* 5 */

	/* Parameter bstrStateMachineName */

/* 6656 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6658 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 6660 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrState */

/* 6662 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6664 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 6666 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrMethodName */

/* 6668 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6670 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 6672 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrCode */

/* 6674 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 6676 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 6678 */	NdrFcShort( 0x446 ),	/* Type Offset=1094 */

	/* Return value */

/* 6680 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 6682 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 6684 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SMchAddStateMethod */

/* 6686 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6688 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6692 */	NdrFcShort( 0x1b ),	/* 27 */
#ifndef _ALPHA_
/* 6694 */	NdrFcShort( 0x20 ),	/* x86, MIPS, PPC Stack size/offset = 32 */
#else
			NdrFcShort( 0x40 ),	/* Alpha Stack size/offset = 64 */
#endif
/* 6696 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6698 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6700 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x7,		/* 7 */

	/* Parameter lFlags */

/* 6702 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 6704 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 6706 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pModule */

/* 6708 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 6710 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 6712 */	NdrFcShort( 0x4de ),	/* Type Offset=1246 */

	/* Parameter bstrModuleMethodName */

/* 6714 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6716 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 6718 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrStateMachineName */

/* 6720 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6722 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 6724 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrState */

/* 6726 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6728 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 6730 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrMethodName */

/* 6732 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6734 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 6736 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Return value */

/* 6738 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 6740 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x38 ),	/* Alpha Stack size/offset = 56 */
#endif
/* 6742 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SMchGetStateMethodFlags */

/* 6744 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6746 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6750 */	NdrFcShort( 0x1c ),	/* 28 */
#ifndef _ALPHA_
/* 6752 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 6754 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6756 */	NdrFcShort( 0x10 ),	/* 16 */
/* 6758 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x5,		/* 5 */

	/* Parameter bstrStateMachineName */

/* 6760 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6762 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 6764 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrState */

/* 6766 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6768 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 6770 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter bstrMethodName */

/* 6772 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6774 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 6776 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter plFlags */

/* 6778 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 6780 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 6782 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 6784 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 6786 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 6788 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SMchOnInvokeMethodFailed */

/* 6790 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6792 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6796 */	NdrFcShort( 0x1d ),	/* 29 */
#ifndef _ALPHA_
/* 6798 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 6800 */	NdrFcShort( 0x0 ),	/* 0 */
/* 6802 */	NdrFcShort( 0x8 ),	/* 8 */
/* 6804 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrMethodName */

/* 6806 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 6808 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 6810 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Parameter plistParams */

/* 6812 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 6814 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 6816 */	NdrFcShort( 0x3ce ),	/* Type Offset=974 */

	/* Return value */

/* 6818 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 6820 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 6822 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnLButtonUp */

/* 6824 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 6826 */	NdrFcLong( 0x0 ),	/* 0 */
/* 6830 */	NdrFcShort( 0xd ),	/* 13 */
#ifndef _ALPHA_
/* 6832 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 6834 */	NdrFcShort( 0x18 ),	/* 24 */
/* 6836 */	NdrFcShort( 0xe ),	/* 14 */
/* 6838 */	0x4,		/* Oi2 Flags:  has return, */
			0x5,		/* 5 */

	/* Parameter hWnd */

/* 6840 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 6842 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 6844 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter x */

/* 6846 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 6848 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 6850 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter y */

/* 6852 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 6854 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 6856 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pbool */

/* 6858 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 6860 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 6862 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 6864 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 6866 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 6868 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x12, 0x0,	/* FC_UP */
/*  4 */	NdrFcShort( 0x3a8 ),	/* Offset= 936 (940) */
/*  6 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/*  8 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 10 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 12 */	NdrFcShort( 0x2 ),	/* Offset= 2 (14) */
/* 14 */	NdrFcShort( 0x10 ),	/* 16 */
/* 16 */	NdrFcShort( 0x2b ),	/* 43 */
/* 18 */	NdrFcLong( 0x3 ),	/* 3 */
/* 22 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 24 */	NdrFcLong( 0x11 ),	/* 17 */
/* 28 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 30 */	NdrFcLong( 0x2 ),	/* 2 */
/* 34 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 36 */	NdrFcLong( 0x4 ),	/* 4 */
/* 40 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 42 */	NdrFcLong( 0x5 ),	/* 5 */
/* 46 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 48 */	NdrFcLong( 0xb ),	/* 11 */
/* 52 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 54 */	NdrFcLong( 0xa ),	/* 10 */
/* 58 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 60 */	NdrFcLong( 0x6 ),	/* 6 */
/* 64 */	NdrFcShort( 0xd6 ),	/* Offset= 214 (278) */
/* 66 */	NdrFcLong( 0x7 ),	/* 7 */
/* 70 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 72 */	NdrFcLong( 0x8 ),	/* 8 */
/* 76 */	NdrFcShort( 0xd0 ),	/* Offset= 208 (284) */
/* 78 */	NdrFcLong( 0xd ),	/* 13 */
/* 82 */	NdrFcShort( 0xe2 ),	/* Offset= 226 (308) */
/* 84 */	NdrFcLong( 0x9 ),	/* 9 */
/* 88 */	NdrFcShort( 0xee ),	/* Offset= 238 (326) */
/* 90 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 94 */	NdrFcShort( 0xfa ),	/* Offset= 250 (344) */
/* 96 */	NdrFcLong( 0x24 ),	/* 36 */
/* 100 */	NdrFcShort( 0x304 ),	/* Offset= 772 (872) */
/* 102 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 106 */	NdrFcShort( 0x2fe ),	/* Offset= 766 (872) */
/* 108 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 112 */	NdrFcShort( 0x2fc ),	/* Offset= 764 (876) */
/* 114 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 118 */	NdrFcShort( 0x2fa ),	/* Offset= 762 (880) */
/* 120 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 124 */	NdrFcShort( 0x2f8 ),	/* Offset= 760 (884) */
/* 126 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 130 */	NdrFcShort( 0x2f6 ),	/* Offset= 758 (888) */
/* 132 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 136 */	NdrFcShort( 0x2f4 ),	/* Offset= 756 (892) */
/* 138 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 142 */	NdrFcShort( 0x2e2 ),	/* Offset= 738 (880) */
/* 144 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 148 */	NdrFcShort( 0x2e0 ),	/* Offset= 736 (884) */
/* 150 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 154 */	NdrFcShort( 0x2e6 ),	/* Offset= 742 (896) */
/* 156 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 160 */	NdrFcShort( 0x2dc ),	/* Offset= 732 (892) */
/* 162 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 166 */	NdrFcShort( 0x2de ),	/* Offset= 734 (900) */
/* 168 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 172 */	NdrFcShort( 0x2dc ),	/* Offset= 732 (904) */
/* 174 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 178 */	NdrFcShort( 0x2da ),	/* Offset= 730 (908) */
/* 180 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 184 */	NdrFcShort( 0x2d8 ),	/* Offset= 728 (912) */
/* 186 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 190 */	NdrFcShort( 0x2d6 ),	/* Offset= 726 (916) */
/* 192 */	NdrFcLong( 0x10 ),	/* 16 */
/* 196 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 198 */	NdrFcLong( 0x12 ),	/* 18 */
/* 202 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 204 */	NdrFcLong( 0x13 ),	/* 19 */
/* 208 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 210 */	NdrFcLong( 0x16 ),	/* 22 */
/* 214 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 216 */	NdrFcLong( 0x17 ),	/* 23 */
/* 220 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 222 */	NdrFcLong( 0xe ),	/* 14 */
/* 226 */	NdrFcShort( 0x2ba ),	/* Offset= 698 (924) */
/* 228 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 232 */	NdrFcShort( 0x2c0 ),	/* Offset= 704 (936) */
/* 234 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 238 */	NdrFcShort( 0x27e ),	/* Offset= 638 (876) */
/* 240 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 244 */	NdrFcShort( 0x27c ),	/* Offset= 636 (880) */
/* 246 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 250 */	NdrFcShort( 0x27a ),	/* Offset= 634 (884) */
/* 252 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 256 */	NdrFcShort( 0x274 ),	/* Offset= 628 (884) */
/* 258 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 262 */	NdrFcShort( 0x26e ),	/* Offset= 622 (884) */
/* 264 */	NdrFcLong( 0x0 ),	/* 0 */
/* 268 */	NdrFcShort( 0x0 ),	/* Offset= 0 (268) */
/* 270 */	NdrFcLong( 0x1 ),	/* 1 */
/* 274 */	NdrFcShort( 0x0 ),	/* Offset= 0 (274) */
/* 276 */	NdrFcShort( 0xffffffff ),	/* Offset= -1 (275) */
/* 278 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 280 */	NdrFcShort( 0x8 ),	/* 8 */
/* 282 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 284 */	
			0x12, 0x0,	/* FC_UP */
/* 286 */	NdrFcShort( 0xc ),	/* Offset= 12 (298) */
/* 288 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 290 */	NdrFcShort( 0x2 ),	/* 2 */
/* 292 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 294 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 296 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 298 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 300 */	NdrFcShort( 0x8 ),	/* 8 */
/* 302 */	NdrFcShort( 0xfffffff2 ),	/* Offset= -14 (288) */
/* 304 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 306 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 308 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 310 */	NdrFcLong( 0x0 ),	/* 0 */
/* 314 */	NdrFcShort( 0x0 ),	/* 0 */
/* 316 */	NdrFcShort( 0x0 ),	/* 0 */
/* 318 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 320 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 322 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 324 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 326 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 328 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 332 */	NdrFcShort( 0x0 ),	/* 0 */
/* 334 */	NdrFcShort( 0x0 ),	/* 0 */
/* 336 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 338 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 340 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 342 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 344 */	
			0x12, 0x0,	/* FC_UP */
/* 346 */	NdrFcShort( 0x1fc ),	/* Offset= 508 (854) */
/* 348 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 350 */	NdrFcShort( 0x18 ),	/* 24 */
/* 352 */	NdrFcShort( 0xa ),	/* 10 */
/* 354 */	NdrFcLong( 0x8 ),	/* 8 */
/* 358 */	NdrFcShort( 0x58 ),	/* Offset= 88 (446) */
/* 360 */	NdrFcLong( 0xd ),	/* 13 */
/* 364 */	NdrFcShort( 0x78 ),	/* Offset= 120 (484) */
/* 366 */	NdrFcLong( 0x9 ),	/* 9 */
/* 370 */	NdrFcShort( 0x94 ),	/* Offset= 148 (518) */
/* 372 */	NdrFcLong( 0xc ),	/* 12 */
/* 376 */	NdrFcShort( 0xbc ),	/* Offset= 188 (564) */
/* 378 */	NdrFcLong( 0x24 ),	/* 36 */
/* 382 */	NdrFcShort( 0x114 ),	/* Offset= 276 (658) */
/* 384 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 388 */	NdrFcShort( 0x130 ),	/* Offset= 304 (692) */
/* 390 */	NdrFcLong( 0x10 ),	/* 16 */
/* 394 */	NdrFcShort( 0x148 ),	/* Offset= 328 (722) */
/* 396 */	NdrFcLong( 0x2 ),	/* 2 */
/* 400 */	NdrFcShort( 0x160 ),	/* Offset= 352 (752) */
/* 402 */	NdrFcLong( 0x3 ),	/* 3 */
/* 406 */	NdrFcShort( 0x178 ),	/* Offset= 376 (782) */
/* 408 */	NdrFcLong( 0x14 ),	/* 20 */
/* 412 */	NdrFcShort( 0x190 ),	/* Offset= 400 (812) */
/* 414 */	NdrFcShort( 0xffffffff ),	/* Offset= -1 (413) */
/* 416 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 418 */	NdrFcShort( 0x4 ),	/* 4 */
/* 420 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 422 */	NdrFcShort( 0x0 ),	/* 0 */
/* 424 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 426 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 428 */	NdrFcShort( 0x4 ),	/* 4 */
/* 430 */	NdrFcShort( 0x0 ),	/* 0 */
/* 432 */	NdrFcShort( 0x1 ),	/* 1 */
/* 434 */	NdrFcShort( 0x0 ),	/* 0 */
/* 436 */	NdrFcShort( 0x0 ),	/* 0 */
/* 438 */	0x12, 0x0,	/* FC_UP */
/* 440 */	NdrFcShort( 0xffffff72 ),	/* Offset= -142 (298) */
/* 442 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 444 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 446 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 448 */	NdrFcShort( 0x8 ),	/* 8 */
/* 450 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 452 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 454 */	NdrFcShort( 0x4 ),	/* 4 */
/* 456 */	NdrFcShort( 0x4 ),	/* 4 */
/* 458 */	0x11, 0x0,	/* FC_RP */
/* 460 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (416) */
/* 462 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 464 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 466 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 468 */	NdrFcShort( 0x0 ),	/* 0 */
/* 470 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 472 */	NdrFcShort( 0x0 ),	/* 0 */
/* 474 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 478 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 480 */	NdrFcShort( 0xffffff54 ),	/* Offset= -172 (308) */
/* 482 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 484 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 486 */	NdrFcShort( 0x8 ),	/* 8 */
/* 488 */	NdrFcShort( 0x0 ),	/* 0 */
/* 490 */	NdrFcShort( 0x6 ),	/* Offset= 6 (496) */
/* 492 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 494 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 496 */	
			0x11, 0x0,	/* FC_RP */
/* 498 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (466) */
/* 500 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 502 */	NdrFcShort( 0x0 ),	/* 0 */
/* 504 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 506 */	NdrFcShort( 0x0 ),	/* 0 */
/* 508 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 512 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 514 */	NdrFcShort( 0xffffff44 ),	/* Offset= -188 (326) */
/* 516 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 518 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 520 */	NdrFcShort( 0x8 ),	/* 8 */
/* 522 */	NdrFcShort( 0x0 ),	/* 0 */
/* 524 */	NdrFcShort( 0x6 ),	/* Offset= 6 (530) */
/* 526 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 528 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 530 */	
			0x11, 0x0,	/* FC_RP */
/* 532 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (500) */
/* 534 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 536 */	NdrFcShort( 0x4 ),	/* 4 */
/* 538 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 540 */	NdrFcShort( 0x0 ),	/* 0 */
/* 542 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 544 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 546 */	NdrFcShort( 0x4 ),	/* 4 */
/* 548 */	NdrFcShort( 0x0 ),	/* 0 */
/* 550 */	NdrFcShort( 0x1 ),	/* 1 */
/* 552 */	NdrFcShort( 0x0 ),	/* 0 */
/* 554 */	NdrFcShort( 0x0 ),	/* 0 */
/* 556 */	0x12, 0x0,	/* FC_UP */
/* 558 */	NdrFcShort( 0x17e ),	/* Offset= 382 (940) */
/* 560 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 562 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 564 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 566 */	NdrFcShort( 0x8 ),	/* 8 */
/* 568 */	NdrFcShort( 0x0 ),	/* 0 */
/* 570 */	NdrFcShort( 0x6 ),	/* Offset= 6 (576) */
/* 572 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 574 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 576 */	
			0x11, 0x0,	/* FC_RP */
/* 578 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (534) */
/* 580 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 582 */	NdrFcLong( 0x2f ),	/* 47 */
/* 586 */	NdrFcShort( 0x0 ),	/* 0 */
/* 588 */	NdrFcShort( 0x0 ),	/* 0 */
/* 590 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 592 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 594 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 596 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 598 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 600 */	NdrFcShort( 0x1 ),	/* 1 */
/* 602 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 604 */	NdrFcShort( 0x4 ),	/* 4 */
/* 606 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 608 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 610 */	NdrFcShort( 0x10 ),	/* 16 */
/* 612 */	NdrFcShort( 0x0 ),	/* 0 */
/* 614 */	NdrFcShort( 0xa ),	/* Offset= 10 (624) */
/* 616 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 618 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 620 */	NdrFcShort( 0xffffffd8 ),	/* Offset= -40 (580) */
/* 622 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 624 */	
			0x12, 0x0,	/* FC_UP */
/* 626 */	NdrFcShort( 0xffffffe4 ),	/* Offset= -28 (598) */
/* 628 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 630 */	NdrFcShort( 0x4 ),	/* 4 */
/* 632 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 634 */	NdrFcShort( 0x0 ),	/* 0 */
/* 636 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 638 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 640 */	NdrFcShort( 0x4 ),	/* 4 */
/* 642 */	NdrFcShort( 0x0 ),	/* 0 */
/* 644 */	NdrFcShort( 0x1 ),	/* 1 */
/* 646 */	NdrFcShort( 0x0 ),	/* 0 */
/* 648 */	NdrFcShort( 0x0 ),	/* 0 */
/* 650 */	0x12, 0x0,	/* FC_UP */
/* 652 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (608) */
/* 654 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 656 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 658 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 660 */	NdrFcShort( 0x8 ),	/* 8 */
/* 662 */	NdrFcShort( 0x0 ),	/* 0 */
/* 664 */	NdrFcShort( 0x6 ),	/* Offset= 6 (670) */
/* 666 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 668 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 670 */	
			0x11, 0x0,	/* FC_RP */
/* 672 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (628) */
/* 674 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 676 */	NdrFcShort( 0x8 ),	/* 8 */
/* 678 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */
/* 680 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 682 */	NdrFcShort( 0x10 ),	/* 16 */
/* 684 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 686 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 688 */	0x0,		/* 0 */
			NdrFcShort( 0xfffffff1 ),	/* Offset= -15 (674) */
			0x5b,		/* FC_END */
/* 692 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 694 */	NdrFcShort( 0x18 ),	/* 24 */
/* 696 */	NdrFcShort( 0x0 ),	/* 0 */
/* 698 */	NdrFcShort( 0xa ),	/* Offset= 10 (708) */
/* 700 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 702 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 704 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (680) */
/* 706 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 708 */	
			0x11, 0x0,	/* FC_RP */
/* 710 */	NdrFcShort( 0xffffff0c ),	/* Offset= -244 (466) */
/* 712 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 714 */	NdrFcShort( 0x1 ),	/* 1 */
/* 716 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 718 */	NdrFcShort( 0x0 ),	/* 0 */
/* 720 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 722 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 724 */	NdrFcShort( 0x8 ),	/* 8 */
/* 726 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 728 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 730 */	NdrFcShort( 0x4 ),	/* 4 */
/* 732 */	NdrFcShort( 0x4 ),	/* 4 */
/* 734 */	0x12, 0x0,	/* FC_UP */
/* 736 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (712) */
/* 738 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 740 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 742 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 744 */	NdrFcShort( 0x2 ),	/* 2 */
/* 746 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 748 */	NdrFcShort( 0x0 ),	/* 0 */
/* 750 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 752 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 754 */	NdrFcShort( 0x8 ),	/* 8 */
/* 756 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 758 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 760 */	NdrFcShort( 0x4 ),	/* 4 */
/* 762 */	NdrFcShort( 0x4 ),	/* 4 */
/* 764 */	0x12, 0x0,	/* FC_UP */
/* 766 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (742) */
/* 768 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 770 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 772 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 774 */	NdrFcShort( 0x4 ),	/* 4 */
/* 776 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 778 */	NdrFcShort( 0x0 ),	/* 0 */
/* 780 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 782 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 784 */	NdrFcShort( 0x8 ),	/* 8 */
/* 786 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 788 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 790 */	NdrFcShort( 0x4 ),	/* 4 */
/* 792 */	NdrFcShort( 0x4 ),	/* 4 */
/* 794 */	0x12, 0x0,	/* FC_UP */
/* 796 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (772) */
/* 798 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 800 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 802 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 804 */	NdrFcShort( 0x8 ),	/* 8 */
/* 806 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 808 */	NdrFcShort( 0x0 ),	/* 0 */
/* 810 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 812 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 814 */	NdrFcShort( 0x8 ),	/* 8 */
/* 816 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 818 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 820 */	NdrFcShort( 0x4 ),	/* 4 */
/* 822 */	NdrFcShort( 0x4 ),	/* 4 */
/* 824 */	0x12, 0x0,	/* FC_UP */
/* 826 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (802) */
/* 828 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 830 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 832 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 834 */	NdrFcShort( 0x8 ),	/* 8 */
/* 836 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 838 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 840 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 842 */	NdrFcShort( 0x8 ),	/* 8 */
/* 844 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 846 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 848 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 850 */	NdrFcShort( 0xffffffee ),	/* Offset= -18 (832) */
/* 852 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 854 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 856 */	NdrFcShort( 0x28 ),	/* 40 */
/* 858 */	NdrFcShort( 0xffffffee ),	/* Offset= -18 (840) */
/* 860 */	NdrFcShort( 0x0 ),	/* Offset= 0 (860) */
/* 862 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 864 */	0x38,		/* FC_ALIGNM4 */
			0x8,		/* FC_LONG */
/* 866 */	0x8,		/* FC_LONG */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 868 */	0x0,		/* 0 */
			NdrFcShort( 0xfffffdf7 ),	/* Offset= -521 (348) */
			0x5b,		/* FC_END */
/* 872 */	
			0x12, 0x0,	/* FC_UP */
/* 874 */	NdrFcShort( 0xfffffef6 ),	/* Offset= -266 (608) */
/* 876 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 878 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 880 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 882 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 884 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 886 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 888 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 890 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 892 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 894 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 896 */	
			0x12, 0x0,	/* FC_UP */
/* 898 */	NdrFcShort( 0xfffffd94 ),	/* Offset= -620 (278) */
/* 900 */	
			0x12, 0x10,	/* FC_UP */
/* 902 */	NdrFcShort( 0xfffffd96 ),	/* Offset= -618 (284) */
/* 904 */	
			0x12, 0x10,	/* FC_UP */
/* 906 */	NdrFcShort( 0xfffffdaa ),	/* Offset= -598 (308) */
/* 908 */	
			0x12, 0x10,	/* FC_UP */
/* 910 */	NdrFcShort( 0xfffffdb8 ),	/* Offset= -584 (326) */
/* 912 */	
			0x12, 0x10,	/* FC_UP */
/* 914 */	NdrFcShort( 0xfffffdc6 ),	/* Offset= -570 (344) */
/* 916 */	
			0x12, 0x10,	/* FC_UP */
/* 918 */	NdrFcShort( 0x2 ),	/* Offset= 2 (920) */
/* 920 */	
			0x12, 0x0,	/* FC_UP */
/* 922 */	NdrFcShort( 0xfffffc66 ),	/* Offset= -922 (0) */
/* 924 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 926 */	NdrFcShort( 0x10 ),	/* 16 */
/* 928 */	0x6,		/* FC_SHORT */
			0x2,		/* FC_CHAR */
/* 930 */	0x2,		/* FC_CHAR */
			0x38,		/* FC_ALIGNM4 */
/* 932 */	0x8,		/* FC_LONG */
			0x39,		/* FC_ALIGNM8 */
/* 934 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 936 */	
			0x12, 0x0,	/* FC_UP */
/* 938 */	NdrFcShort( 0xfffffff2 ),	/* Offset= -14 (924) */
/* 940 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 942 */	NdrFcShort( 0x20 ),	/* 32 */
/* 944 */	NdrFcShort( 0x0 ),	/* 0 */
/* 946 */	NdrFcShort( 0x0 ),	/* Offset= 0 (946) */
/* 948 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 950 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 952 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 954 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 956 */	NdrFcShort( 0xfffffc4a ),	/* Offset= -950 (6) */
/* 958 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 960 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 962 */	NdrFcShort( 0x0 ),	/* 0 */
/* 964 */	NdrFcShort( 0x10 ),	/* 16 */
/* 966 */	NdrFcShort( 0x0 ),	/* 0 */
/* 968 */	NdrFcShort( 0xfffffc3a ),	/* Offset= -966 (2) */
/* 970 */	
			0x11, 0x10,	/* FC_RP */
/* 972 */	NdrFcShort( 0x2 ),	/* Offset= 2 (974) */
/* 974 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 976 */	NdrFcLong( 0x590d9846 ),	/* 1494063174 */
/* 980 */	NdrFcShort( 0xa417 ),	/* -23529 */
/* 982 */	NdrFcShort( 0x11cf ),	/* 4559 */
/* 984 */	0x9b,		/* 155 */
			0xf0,		/* 240 */
/* 986 */	0x0,		/* 0 */
			0x80,		/* 128 */
/* 988 */	0xc7,		/* 199 */
			0xa5,		/* 165 */
/* 990 */	0x6a,		/* 106 */
			0x8a,		/* 138 */
/* 992 */	
			0x11, 0x10,	/* FC_RP */
/* 994 */	NdrFcShort( 0x2 ),	/* Offset= 2 (996) */
/* 996 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 998 */	NdrFcLong( 0x590d9848 ),	/* 1494063176 */
/* 1002 */	NdrFcShort( 0xa417 ),	/* -23529 */
/* 1004 */	NdrFcShort( 0x11cf ),	/* 4559 */
/* 1006 */	0x9b,		/* 155 */
			0xf0,		/* 240 */
/* 1008 */	0x0,		/* 0 */
			0x80,		/* 128 */
/* 1010 */	0xc7,		/* 199 */
			0xa5,		/* 165 */
/* 1012 */	0x6a,		/* 106 */
			0x8a,		/* 138 */
/* 1014 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1016 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1018 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1020 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1022 */	NdrFcShort( 0xfffffd1e ),	/* Offset= -738 (284) */
/* 1024 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1026 */	NdrFcLong( 0x590d9852 ),	/* 1494063186 */
/* 1030 */	NdrFcShort( 0xa417 ),	/* -23529 */
/* 1032 */	NdrFcShort( 0x11cf ),	/* 4559 */
/* 1034 */	0x9b,		/* 155 */
			0xf0,		/* 240 */
/* 1036 */	0x0,		/* 0 */
			0x80,		/* 128 */
/* 1038 */	0xc7,		/* 199 */
			0xa5,		/* 165 */
/* 1040 */	0x6a,		/* 106 */
			0x8a,		/* 138 */
/* 1042 */	
			0x11, 0x10,	/* FC_RP */
/* 1044 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1046) */
/* 1046 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1048 */	NdrFcLong( 0xd4916c22 ),	/* -728667102 */
/* 1052 */	NdrFcShort( 0xc2 ),	/* 194 */
/* 1054 */	NdrFcShort( 0x11d0 ),	/* 4560 */
/* 1056 */	0x8a,		/* 138 */
			0x96,		/* 150 */
/* 1058 */	0x0,		/* 0 */
			0xa0,		/* 160 */
/* 1060 */	0xc9,		/* 201 */
			0x8,		/* 8 */
/* 1062 */	0x25,		/* 37 */
			0x83,		/* 131 */
/* 1064 */	
			0x11, 0x10,	/* FC_RP */
/* 1066 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1068) */
/* 1068 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1070 */	NdrFcLong( 0x9362218 ),	/* 154542616 */
/* 1074 */	NdrFcShort( 0x2570 ),	/* 9584 */
/* 1076 */	NdrFcShort( 0x11d1 ),	/* 4561 */
/* 1078 */	0x83,		/* 131 */
			0xbf,		/* 191 */
/* 1080 */	0x0,		/* 0 */
			0xc0,		/* 192 */
/* 1082 */	0x4f,		/* 79 */
			0xb6,		/* 182 */
/* 1084 */	0xfa,		/* 250 */
			0x46,		/* 70 */
/* 1086 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 1088 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1094) */
/* 1090 */	
			0x13, 0x0,	/* FC_OP */
/* 1092 */	NdrFcShort( 0xfffffce6 ),	/* Offset= -794 (298) */
/* 1094 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1096 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1098 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1100 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1102 */	NdrFcShort( 0xfffffff4 ),	/* Offset= -12 (1090) */
/* 1104 */	
			0x11, 0x10,	/* FC_RP */
/* 1106 */	NdrFcShort( 0xfffffcf4 ),	/* Offset= -780 (326) */
/* 1108 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 1110 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 1112 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 1114 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 1116 */	
			0x11, 0x0,	/* FC_RP */
/* 1118 */	NdrFcShort( 0x1e ),	/* Offset= 30 (1148) */
/* 1120 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 1122 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1124 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1126 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1128 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 1132 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1134 */	NdrFcShort( 0xffffff52 ),	/* Offset= -174 (960) */
/* 1136 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1138 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1140 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1142 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1144 */	NdrFcShort( 0xc ),	/* 12 */
/* 1146 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1148 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1150 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1152 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1154 */	NdrFcShort( 0x8 ),	/* Offset= 8 (1162) */
/* 1156 */	0x36,		/* FC_POINTER */
			0x36,		/* FC_POINTER */
/* 1158 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1160 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1162 */	
			0x12, 0x0,	/* FC_UP */
/* 1164 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (1120) */
/* 1166 */	
			0x12, 0x0,	/* FC_UP */
/* 1168 */	NdrFcShort( 0xffffffe2 ),	/* Offset= -30 (1138) */
/* 1170 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1172 */	NdrFcLong( 0x590d9840 ),	/* 1494063168 */
/* 1176 */	NdrFcShort( 0xa417 ),	/* -23529 */
/* 1178 */	NdrFcShort( 0x11cf ),	/* 4559 */
/* 1180 */	0x9b,		/* 155 */
			0xf0,		/* 240 */
/* 1182 */	0x0,		/* 0 */
			0x80,		/* 128 */
/* 1184 */	0xc7,		/* 199 */
			0xa5,		/* 165 */
/* 1186 */	0x6a,		/* 106 */
			0x8a,		/* 138 */
/* 1188 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1190 */	NdrFcLong( 0x590d9830 ),	/* 1494063152 */
/* 1194 */	NdrFcShort( 0xa417 ),	/* -23529 */
/* 1196 */	NdrFcShort( 0x11cf ),	/* 4559 */
/* 1198 */	0x9b,		/* 155 */
			0xf0,		/* 240 */
/* 1200 */	0x0,		/* 0 */
			0x80,		/* 128 */
/* 1202 */	0xc7,		/* 199 */
			0xa5,		/* 165 */
/* 1204 */	0x6a,		/* 106 */
			0x8a,		/* 138 */
/* 1206 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1208 */	NdrFcLong( 0x590d9842 ),	/* 1494063170 */
/* 1212 */	NdrFcShort( 0xa417 ),	/* -23529 */
/* 1214 */	NdrFcShort( 0x11cf ),	/* 4559 */
/* 1216 */	0x9b,		/* 155 */
			0xf0,		/* 240 */
/* 1218 */	0x0,		/* 0 */
			0x80,		/* 128 */
/* 1220 */	0xc7,		/* 199 */
			0xa5,		/* 165 */
/* 1222 */	0x6a,		/* 106 */
			0x8a,		/* 138 */
/* 1224 */	
			0x11, 0x10,	/* FC_RP */
/* 1226 */	NdrFcShort( 0xffffff36 ),	/* Offset= -202 (1024) */
/* 1228 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 1230 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1236) */
/* 1232 */	
			0x13, 0x0,	/* FC_OP */
/* 1234 */	NdrFcShort( 0xfffffeda ),	/* Offset= -294 (940) */
/* 1236 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1238 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1240 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1242 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1244 */	NdrFcShort( 0xfffffff4 ),	/* Offset= -12 (1232) */
/* 1246 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1248 */	NdrFcLong( 0x590d984a ),	/* 1494063178 */
/* 1252 */	NdrFcShort( 0xa417 ),	/* -23529 */
/* 1254 */	NdrFcShort( 0x11cf ),	/* 4559 */
/* 1256 */	0x9b,		/* 155 */
			0xf0,		/* 240 */
/* 1258 */	0x0,		/* 0 */
			0x80,		/* 128 */
/* 1260 */	0xc7,		/* 199 */
			0xa5,		/* 165 */
/* 1262 */	0x6a,		/* 106 */
			0x8a,		/* 138 */

			0x0
        }
    };

const CInterfaceProxyVtbl * _vwsysex_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IStateMachineExemplarProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IThingExemplarProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IArtifactExemplarProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IAvatarExemplarProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IPortalExemplarProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IRoomExemplarProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IHostExemplarProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IIWPortalExemplarProxyVtbl,
    ( CInterfaceProxyVtbl *) &_ISystemExemplarProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IWebPageExemplarProxyVtbl,
    0
};

const CInterfaceStubVtbl * _vwsysex_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IStateMachineExemplarStubVtbl,
    ( CInterfaceStubVtbl *) &_IThingExemplarStubVtbl,
    ( CInterfaceStubVtbl *) &_IArtifactExemplarStubVtbl,
    ( CInterfaceStubVtbl *) &_IAvatarExemplarStubVtbl,
    ( CInterfaceStubVtbl *) &_IPortalExemplarStubVtbl,
    ( CInterfaceStubVtbl *) &_IRoomExemplarStubVtbl,
    ( CInterfaceStubVtbl *) &_IHostExemplarStubVtbl,
    ( CInterfaceStubVtbl *) &_IIWPortalExemplarStubVtbl,
    ( CInterfaceStubVtbl *) &_ISystemExemplarStubVtbl,
    ( CInterfaceStubVtbl *) &_IWebPageExemplarStubVtbl,
    0
};

PCInterfaceName const _vwsysex_InterfaceNamesList[] = 
{
    "IStateMachineExemplar",
    "IThingExemplar",
    "IArtifactExemplar",
    "IAvatarExemplar",
    "IPortalExemplar",
    "IRoomExemplar",
    "IHostExemplar",
    "IIWPortalExemplar",
    "ISystemExemplar",
    "IWebPageExemplar",
    0
};

const IID *  _vwsysex_BaseIIDList[] = 
{
    &IID_IVWDispatch,
    &IID_IVWDispatch,
    &IID_IVWDispatch,
    &IID_IVWDispatch,
    &IID_IVWDispatch,
    &IID_IVWDispatch,
    &IID_IVWDispatch,
    &IID_IVWDispatch,
    &IID_IVWDispatch,
    &IID_IVWDispatch,
    0
};


#define _vwsysex_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _vwsysex, pIID, n)

int __stdcall _vwsysex_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _vwsysex, 10, 8 )
    IID_BS_LOOKUP_NEXT_TEST( _vwsysex, 4 )
    IID_BS_LOOKUP_NEXT_TEST( _vwsysex, 2 )
    IID_BS_LOOKUP_NEXT_TEST( _vwsysex, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _vwsysex, 10, *pIndex )
    
}

const ExtendedProxyFileInfo vwsysex_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _vwsysex_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _vwsysex_StubVtblList,
    (const PCInterfaceName * ) & _vwsysex_InterfaceNamesList,
    (const IID ** ) & _vwsysex_BaseIIDList,
    & _vwsysex_IID_Lookup, 
    10,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
