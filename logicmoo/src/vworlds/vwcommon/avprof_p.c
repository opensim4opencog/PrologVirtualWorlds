/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:38 2000
 */
/* Compiler settings for avprof.idl:
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


#include "avprof.h"

#define TYPE_FORMAT_STRING_SIZE   1065                              
#define PROC_FORMAT_STRING_SIZE   795                               

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


/* Standard interface: __MIDL_itf_avprof_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IObjectProperty, ver. 0.0,
   GUID={0x590D9840,0xA417,0x11CF,{0x9B,0xF0,0x00,0x80,0xC7,0xA5,0x6A,0x8A}} */


/* Object interface: IAvatarProfile, ver. 0.0,
   GUID={0xD4E7472B,0x2953,0x11D1,{0xAF,0xB6,0x00,0xC0,0x4F,0xB9,0x15,0xAA}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IAvatarProfile_ServerInfo;

#pragma code_seg(".orpc")
/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_get_KeyValue_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ BSTR bstrKey,
    /* [retval][out] */ VARIANT __RPC_FAR *pvarKeyVal)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pvarKeyVal);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[136],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[136],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&bstrKey,
                  ( unsigned char __RPC_FAR * )&pvarKeyVal);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[136],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_put_KeyValue_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ BSTR bstrKey,
    /* [in] */ VARIANT varKeyVal)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,varKeyVal);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[170],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[170],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&bstrKey,
                  ( unsigned char __RPC_FAR * )&varKeyVal);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[170],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_get_KeyGraphic_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ BSTR bstrKey,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrGraphic)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pbstrGraphic);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[204],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[204],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&bstrKey,
                  ( unsigned char __RPC_FAR * )&pbstrGraphic);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[204],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_put_KeyGraphic_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ BSTR bstrKey,
    /* [in] */ BSTR bstrGraphic)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,bstrGraphic);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[238],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[238],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&bstrKey,
                  ( unsigned char __RPC_FAR * )&bstrGraphic);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[238],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_get_KeyOwner_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ BSTR bstrKey,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppOwner)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,ppOwner);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[272],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[272],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&bstrKey,
                  ( unsigned char __RPC_FAR * )&ppOwner);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[272],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_get_KeyAccess_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ BSTR bstrKey,
    /* [retval][out] */ short __RPC_FAR *sAccess)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,sAccess);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[306],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[306],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&bstrKey,
                  ( unsigned char __RPC_FAR * )&sAccess);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[306],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_put_KeyAccess_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ BSTR bstrKey,
    /* [in] */ short sAccess)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,sAccess);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[340],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[340],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&bstrKey,
                  ( unsigned char __RPC_FAR * )&sAccess);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[340],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_RemoveKey_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ BSTR bstrKey)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,bstrKey);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[374],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[374],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&bstrKey);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[374],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_BanObject_Proxy( 
    IAvatarProfile __RPC_FAR * This,
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
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[402],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[402],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pThing);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[402],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_UnbanObject_Proxy( 
    IAvatarProfile __RPC_FAR * This,
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
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[430],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[430],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pThing);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[430],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_get_Avatar_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *pThing)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pThing);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[458],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[458],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pThing);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[458],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id][propput][restricted] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_put_Avatar_Proxy( 
    IAvatarProfile __RPC_FAR * This,
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
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[486],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[486],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pThing);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[486],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id][restricted] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_WriteToDisk_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ BSTR bstrFilename)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,bstrFilename);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[514],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[514],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&bstrFilename);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[514],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id][restricted] */ HRESULT STDMETHODCALLTYPE IAvatarProfile_ReadFromDisk_Proxy( 
    IAvatarProfile __RPC_FAR * This,
    /* [in] */ BSTR bstrFilename)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,bstrFilename);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[542],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[542],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&bstrFilename);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[542],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

static const unsigned short IAvatarProfile_FormatStringOffsetTable[] = 
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
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
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
    22,
    50,
    84,
    136,
    170,
    204,
    238,
    272,
    306,
    340,
    374,
    402,
    430,
    458,
    486,
    514,
    542
    };

static const MIDL_SERVER_INFO IAvatarProfile_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IAvatarProfile_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IAvatarProfile_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IAvatarProfile_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(46) _IAvatarProfileProxyVtbl = 
{
    &IAvatarProfile_ProxyInfo,
    &IID_IAvatarProfile,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* (void *)-1 /* IObjectProperty::Terminate */ ,
    0 /* (void *)-1 /* IObjectProperty::get_RefCount */ ,
    0 /* (void *)-1 /* IObjectProperty::get_CLSID */ ,
    0 /* (void *)-1 /* IObjectProperty::get_IID */ ,
    0 /* (void *)-1 /* IObjectProperty::get_World */ ,
    0 /* (void *)-1 /* IObjectProperty::put_World */ ,
    0 /* (void *)-1 /* IObjectProperty::get_Parent */ ,
    0 /* (void *)-1 /* IObjectProperty::put_Parent */ ,
    0 /* (void *)-1 /* IObjectProperty::get_PropertyName */ ,
    0 /* (void *)-1 /* IObjectProperty::put_PropertyName */ ,
    0 /* (void *)-1 /* IObjectProperty::get_Type */ ,
    0 /* (void *)-1 /* IObjectProperty::put_Type */ ,
    0 /* (void *)-1 /* IObjectProperty::get_Security */ ,
    0 /* (void *)-1 /* IObjectProperty::put_Security */ ,
    0 /* (void *)-1 /* IObjectProperty::IsOfType */ ,
    0 /* (void *)-1 /* IObjectProperty::ToAscii */ ,
    0 /* (void *)-1 /* IObjectProperty::Dump */ ,
    0 /* (void *)-1 /* IObjectProperty::Clone */ ,
    0 /* (void *)-1 /* IObjectProperty::Marshall */ ,
    0 /* (void *)-1 /* IObjectProperty::UnMarshall */ ,
    0 /* (void *)-1 /* IObjectProperty::MarshallToScript */ ,
    (void *)-1 /* IAvatarProfile::Reset */ ,
    (void *)-1 /* IAvatarProfile::get_Keys */ ,
    (void *)-1 /* IAvatarProfile::IsValid */ ,
    (void *)-1 /* IAvatarProfile::AddKey */ ,
    IAvatarProfile_get_KeyValue_Proxy ,
    IAvatarProfile_put_KeyValue_Proxy ,
    IAvatarProfile_get_KeyGraphic_Proxy ,
    IAvatarProfile_put_KeyGraphic_Proxy ,
    IAvatarProfile_get_KeyOwner_Proxy ,
    IAvatarProfile_get_KeyAccess_Proxy ,
    IAvatarProfile_put_KeyAccess_Proxy ,
    IAvatarProfile_RemoveKey_Proxy ,
    IAvatarProfile_BanObject_Proxy ,
    IAvatarProfile_UnbanObject_Proxy ,
    IAvatarProfile_get_Avatar_Proxy ,
    IAvatarProfile_put_Avatar_Proxy ,
    IAvatarProfile_WriteToDisk_Proxy ,
    IAvatarProfile_ReadFromDisk_Proxy
};


static const PRPC_STUB_FUNCTION IAvatarProfile_table[] =
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
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
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

CInterfaceStubVtbl _IAvatarProfileStubVtbl =
{
    &IID_IAvatarProfile,
    &IAvatarProfile_ServerInfo,
    46,
    &IAvatarProfile_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_avprof_0244, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IAvatarProfileData, ver. 0.0,
   GUID={0xD4E7472D,0x2953,0x11D1,{0xAF,0xB6,0x00,0xC0,0x4F,0xB9,0x15,0xAA}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IAvatarProfileData_ServerInfo;

#pragma code_seg(".orpc")
/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAvatarProfileData_get_Owner_Proxy( 
    IAvatarProfileData __RPC_FAR * This,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppOwner)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,ppOwner);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[682],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[682],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&ppOwner);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[682],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IAvatarProfileData_put_Owner_Proxy( 
    IAvatarProfileData __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pOwner)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,pOwner);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[710],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[710],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&pOwner);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[710],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAvatarProfileData_get_Access_Proxy( 
    IAvatarProfileData __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *sAccess)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,sAccess);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[738],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[738],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&sAccess);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[738],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IAvatarProfileData_put_Access_Proxy( 
    IAvatarProfileData __RPC_FAR * This,
    /* [in] */ short sAccess)
{
CLIENT_CALL_RETURN _RetVal;


#if defined( _ALPHA_ )
    va_list vlist;
#endif
    
#if defined( _ALPHA_ )
    va_start(vlist,sAccess);
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[766],
                  vlist.a0);
#elif defined( _PPC_ ) || defined( _MIPS_ )

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[766],
                  ( unsigned char __RPC_FAR * )&This,
                  ( unsigned char __RPC_FAR * )&sAccess);
#else
    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&Object_StubDesc,
                  (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[766],
                  ( unsigned char __RPC_FAR * )&This);
#endif
    return ( HRESULT  )_RetVal.Simple;
    
}

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

static const unsigned short IAvatarProfileData_FormatStringOffsetTable[] = 
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
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    570,
    598,
    626,
    654,
    682,
    710,
    738,
    766
    };

static const MIDL_SERVER_INFO IAvatarProfileData_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IAvatarProfileData_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IAvatarProfileData_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IAvatarProfileData_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(36) _IAvatarProfileDataProxyVtbl = 
{
    &IAvatarProfileData_ProxyInfo,
    &IID_IAvatarProfileData,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* (void *)-1 /* IObjectProperty::Terminate */ ,
    0 /* (void *)-1 /* IObjectProperty::get_RefCount */ ,
    0 /* (void *)-1 /* IObjectProperty::get_CLSID */ ,
    0 /* (void *)-1 /* IObjectProperty::get_IID */ ,
    0 /* (void *)-1 /* IObjectProperty::get_World */ ,
    0 /* (void *)-1 /* IObjectProperty::put_World */ ,
    0 /* (void *)-1 /* IObjectProperty::get_Parent */ ,
    0 /* (void *)-1 /* IObjectProperty::put_Parent */ ,
    0 /* (void *)-1 /* IObjectProperty::get_PropertyName */ ,
    0 /* (void *)-1 /* IObjectProperty::put_PropertyName */ ,
    0 /* (void *)-1 /* IObjectProperty::get_Type */ ,
    0 /* (void *)-1 /* IObjectProperty::put_Type */ ,
    0 /* (void *)-1 /* IObjectProperty::get_Security */ ,
    0 /* (void *)-1 /* IObjectProperty::put_Security */ ,
    0 /* (void *)-1 /* IObjectProperty::IsOfType */ ,
    0 /* (void *)-1 /* IObjectProperty::ToAscii */ ,
    0 /* (void *)-1 /* IObjectProperty::Dump */ ,
    0 /* (void *)-1 /* IObjectProperty::Clone */ ,
    0 /* (void *)-1 /* IObjectProperty::Marshall */ ,
    0 /* (void *)-1 /* IObjectProperty::UnMarshall */ ,
    0 /* (void *)-1 /* IObjectProperty::MarshallToScript */ ,
    (void *)-1 /* IAvatarProfileData::get_Value */ ,
    (void *)-1 /* IAvatarProfileData::put_Value */ ,
    (void *)-1 /* IAvatarProfileData::get_Graphic */ ,
    (void *)-1 /* IAvatarProfileData::put_Graphic */ ,
    IAvatarProfileData_get_Owner_Proxy ,
    IAvatarProfileData_put_Owner_Proxy ,
    IAvatarProfileData_get_Access_Proxy ,
    IAvatarProfileData_put_Access_Proxy
};


static const PRPC_STUB_FUNCTION IAvatarProfileData_table[] =
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
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
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
    NdrStubCall2
};

CInterfaceStubVtbl _IAvatarProfileDataStubVtbl =
{
    &IID_IAvatarProfileData,
    &IAvatarProfileData_ServerInfo,
    36,
    &IAvatarProfileData_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

#pragma data_seg(".rdata")

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[2] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            },
            {
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
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

	/* Procedure Reset */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x1c ),	/* 28 */
#ifndef _ALPHA_
/*  8 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */

/* 16 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 18 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 20 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Keys */

/* 22 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 24 */	NdrFcLong( 0x0 ),	/* 0 */
/* 28 */	NdrFcShort( 0x1d ),	/* 29 */
#ifndef _ALPHA_
/* 30 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 32 */	NdrFcShort( 0x0 ),	/* 0 */
/* 34 */	NdrFcShort( 0x8 ),	/* 8 */
/* 36 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter ppKeyList */

/* 38 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 40 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 42 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 44 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 46 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 48 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure IsValid */

/* 50 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 52 */	NdrFcLong( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x1e ),	/* 30 */
#ifndef _ALPHA_
/* 58 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 60 */	NdrFcShort( 0x0 ),	/* 0 */
/* 62 */	NdrFcShort( 0xe ),	/* 14 */
/* 64 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrKey */

/* 66 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 68 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 70 */	NdrFcShort( 0x30 ),	/* Type Offset=48 */

	/* Parameter pbValid */

/* 72 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 74 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 76 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 78 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 80 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 82 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddKey */

/* 84 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 86 */	NdrFcLong( 0x0 ),	/* 0 */
/* 90 */	NdrFcShort( 0x1f ),	/* 31 */
#ifndef _ALPHA_
/* 92 */	NdrFcShort( 0x4c ),	/* x86, MIPS, PPC Stack size/offset = 76 */
#else
			NdrFcShort( 0x58 ),	/* Alpha Stack size/offset = 88 */
#endif
/* 94 */	NdrFcShort( 0x0 ),	/* 0 */
/* 96 */	NdrFcShort( 0x8 ),	/* 8 */
/* 98 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x6,		/* 6 */

	/* Parameter bstrKey */

/* 100 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 102 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 104 */	NdrFcShort( 0x30 ),	/* Type Offset=48 */

	/* Parameter varKeyVal */

/* 106 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 108 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 110 */	NdrFcShort( 0x3e4 ),	/* Type Offset=996 */

	/* Parameter varStrGraphic */

/* 112 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 114 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 116 */	NdrFcShort( 0x3e4 ),	/* Type Offset=996 */

	/* Parameter varAccess */

/* 118 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 120 */	NdrFcShort( 0x28 ),	/* x86, MIPS, PPC Stack size/offset = 40 */
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 122 */	NdrFcShort( 0x3e4 ),	/* Type Offset=996 */

	/* Parameter varOwner */

/* 124 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 126 */	NdrFcShort( 0x38 ),	/* x86, MIPS, PPC Stack size/offset = 56 */
#else
			NdrFcShort( 0x40 ),	/* Alpha Stack size/offset = 64 */
#endif
/* 128 */	NdrFcShort( 0x3e4 ),	/* Type Offset=996 */

	/* Return value */

/* 130 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 132 */	NdrFcShort( 0x48 ),	/* x86, MIPS, PPC Stack size/offset = 72 */
#else
			NdrFcShort( 0x50 ),	/* Alpha Stack size/offset = 80 */
#endif
/* 134 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_KeyValue */

/* 136 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 138 */	NdrFcLong( 0x0 ),	/* 0 */
/* 142 */	NdrFcShort( 0x20 ),	/* 32 */
#ifndef _ALPHA_
/* 144 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 146 */	NdrFcShort( 0x0 ),	/* 0 */
/* 148 */	NdrFcShort( 0x8 ),	/* 8 */
/* 150 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrKey */

/* 152 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 154 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 156 */	NdrFcShort( 0x30 ),	/* Type Offset=48 */

	/* Parameter pvarKeyVal */

/* 158 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
#ifndef _ALPHA_
/* 160 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 162 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Return value */

/* 164 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 166 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 168 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_KeyValue */

/* 170 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 172 */	NdrFcLong( 0x0 ),	/* 0 */
/* 176 */	NdrFcShort( 0x21 ),	/* 33 */
#ifndef _ALPHA_
/* 178 */	NdrFcShort( 0x1c ),	/* x86, MIPS, PPC Stack size/offset = 28 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 180 */	NdrFcShort( 0x0 ),	/* 0 */
/* 182 */	NdrFcShort( 0x8 ),	/* 8 */
/* 184 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrKey */

/* 186 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 188 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 190 */	NdrFcShort( 0x30 ),	/* Type Offset=48 */

	/* Parameter varKeyVal */

/* 192 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 194 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 196 */	NdrFcShort( 0x3e4 ),	/* Type Offset=996 */

	/* Return value */

/* 198 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 200 */	NdrFcShort( 0x18 ),	/* x86, MIPS, PPC Stack size/offset = 24 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 202 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_KeyGraphic */

/* 204 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 206 */	NdrFcLong( 0x0 ),	/* 0 */
/* 210 */	NdrFcShort( 0x22 ),	/* 34 */
#ifndef _ALPHA_
/* 212 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 214 */	NdrFcShort( 0x0 ),	/* 0 */
/* 216 */	NdrFcShort( 0x8 ),	/* 8 */
/* 218 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrKey */

/* 220 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 222 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 224 */	NdrFcShort( 0x30 ),	/* Type Offset=48 */

	/* Parameter pbstrGraphic */

/* 226 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 228 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 230 */	NdrFcShort( 0x408 ),	/* Type Offset=1032 */

	/* Return value */

/* 232 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 234 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 236 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_KeyGraphic */

/* 238 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 240 */	NdrFcLong( 0x0 ),	/* 0 */
/* 244 */	NdrFcShort( 0x23 ),	/* 35 */
#ifndef _ALPHA_
/* 246 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 248 */	NdrFcShort( 0x0 ),	/* 0 */
/* 250 */	NdrFcShort( 0x8 ),	/* 8 */
/* 252 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrKey */

/* 254 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 256 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 258 */	NdrFcShort( 0x30 ),	/* Type Offset=48 */

	/* Parameter bstrGraphic */

/* 260 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 262 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 264 */	NdrFcShort( 0x30 ),	/* Type Offset=48 */

	/* Return value */

/* 266 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 268 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 270 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_KeyOwner */

/* 272 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 274 */	NdrFcLong( 0x0 ),	/* 0 */
/* 278 */	NdrFcShort( 0x24 ),	/* 36 */
#ifndef _ALPHA_
/* 280 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 282 */	NdrFcShort( 0x0 ),	/* 0 */
/* 284 */	NdrFcShort( 0x8 ),	/* 8 */
/* 286 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrKey */

/* 288 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 290 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 292 */	NdrFcShort( 0x30 ),	/* Type Offset=48 */

	/* Parameter ppOwner */

/* 294 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 296 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 298 */	NdrFcShort( 0x412 ),	/* Type Offset=1042 */

	/* Return value */

/* 300 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 302 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 304 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_KeyAccess */

/* 306 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 308 */	NdrFcLong( 0x0 ),	/* 0 */
/* 312 */	NdrFcShort( 0x25 ),	/* 37 */
#ifndef _ALPHA_
/* 314 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 316 */	NdrFcShort( 0x0 ),	/* 0 */
/* 318 */	NdrFcShort( 0xe ),	/* 14 */
/* 320 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrKey */

/* 322 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 324 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 326 */	NdrFcShort( 0x30 ),	/* Type Offset=48 */

	/* Parameter sAccess */

/* 328 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 330 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 332 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 334 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 336 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 338 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_KeyAccess */

/* 340 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 342 */	NdrFcLong( 0x0 ),	/* 0 */
/* 346 */	NdrFcShort( 0x26 ),	/* 38 */
#ifndef _ALPHA_
/* 348 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 350 */	NdrFcShort( 0x6 ),	/* 6 */
/* 352 */	NdrFcShort( 0x8 ),	/* 8 */
/* 354 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrKey */

/* 356 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 358 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 360 */	NdrFcShort( 0x30 ),	/* Type Offset=48 */

	/* Parameter sAccess */

/* 362 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 364 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 366 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 368 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 370 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 372 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RemoveKey */

/* 374 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 376 */	NdrFcLong( 0x0 ),	/* 0 */
/* 380 */	NdrFcShort( 0x27 ),	/* 39 */
#ifndef _ALPHA_
/* 382 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 384 */	NdrFcShort( 0x0 ),	/* 0 */
/* 386 */	NdrFcShort( 0x8 ),	/* 8 */
/* 388 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter bstrKey */

/* 390 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 392 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 394 */	NdrFcShort( 0x30 ),	/* Type Offset=48 */

	/* Return value */

/* 396 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 398 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 400 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure BanObject */

/* 402 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 404 */	NdrFcLong( 0x0 ),	/* 0 */
/* 408 */	NdrFcShort( 0x28 ),	/* 40 */
#ifndef _ALPHA_
/* 410 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 412 */	NdrFcShort( 0x0 ),	/* 0 */
/* 414 */	NdrFcShort( 0x8 ),	/* 8 */
/* 416 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pThing */

/* 418 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 420 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 422 */	NdrFcShort( 0x416 ),	/* Type Offset=1046 */

	/* Return value */

/* 424 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 426 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 428 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure UnbanObject */

/* 430 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 432 */	NdrFcLong( 0x0 ),	/* 0 */
/* 436 */	NdrFcShort( 0x29 ),	/* 41 */
#ifndef _ALPHA_
/* 438 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 440 */	NdrFcShort( 0x0 ),	/* 0 */
/* 442 */	NdrFcShort( 0x8 ),	/* 8 */
/* 444 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pThing */

/* 446 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 448 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 450 */	NdrFcShort( 0x416 ),	/* Type Offset=1046 */

	/* Return value */

/* 452 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 454 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 456 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Avatar */

/* 458 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 460 */	NdrFcLong( 0x0 ),	/* 0 */
/* 464 */	NdrFcShort( 0x2a ),	/* 42 */
#ifndef _ALPHA_
/* 466 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 468 */	NdrFcShort( 0x0 ),	/* 0 */
/* 470 */	NdrFcShort( 0x8 ),	/* 8 */
/* 472 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter pThing */

/* 474 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 476 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 478 */	NdrFcShort( 0x412 ),	/* Type Offset=1042 */

	/* Return value */

/* 480 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 482 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 484 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Avatar */

/* 486 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 488 */	NdrFcLong( 0x0 ),	/* 0 */
/* 492 */	NdrFcShort( 0x2b ),	/* 43 */
#ifndef _ALPHA_
/* 494 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 496 */	NdrFcShort( 0x0 ),	/* 0 */
/* 498 */	NdrFcShort( 0x8 ),	/* 8 */
/* 500 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pThing */

/* 502 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 504 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 506 */	NdrFcShort( 0x416 ),	/* Type Offset=1046 */

	/* Return value */

/* 508 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 510 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 512 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure WriteToDisk */

/* 514 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 516 */	NdrFcLong( 0x0 ),	/* 0 */
/* 520 */	NdrFcShort( 0x2c ),	/* 44 */
#ifndef _ALPHA_
/* 522 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 524 */	NdrFcShort( 0x0 ),	/* 0 */
/* 526 */	NdrFcShort( 0x8 ),	/* 8 */
/* 528 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter bstrFilename */

/* 530 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 532 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 534 */	NdrFcShort( 0x30 ),	/* Type Offset=48 */

	/* Return value */

/* 536 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 538 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 540 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ReadFromDisk */

/* 542 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 544 */	NdrFcLong( 0x0 ),	/* 0 */
/* 548 */	NdrFcShort( 0x2d ),	/* 45 */
#ifndef _ALPHA_
/* 550 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 552 */	NdrFcShort( 0x0 ),	/* 0 */
/* 554 */	NdrFcShort( 0x8 ),	/* 8 */
/* 556 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter bstrFilename */

/* 558 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 560 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 562 */	NdrFcShort( 0x30 ),	/* Type Offset=48 */

	/* Return value */

/* 564 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 566 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 568 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Value */

/* 570 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 572 */	NdrFcLong( 0x0 ),	/* 0 */
/* 576 */	NdrFcShort( 0x1c ),	/* 28 */
#ifndef _ALPHA_
/* 578 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 580 */	NdrFcShort( 0x0 ),	/* 0 */
/* 582 */	NdrFcShort( 0x8 ),	/* 8 */
/* 584 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter pvarValue */

/* 586 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
#ifndef _ALPHA_
/* 588 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 590 */	NdrFcShort( 0x3f6 ),	/* Type Offset=1014 */

	/* Return value */

/* 592 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 594 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 596 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Value */

/* 598 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 600 */	NdrFcLong( 0x0 ),	/* 0 */
/* 604 */	NdrFcShort( 0x1d ),	/* 29 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 606 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
#else
			NdrFcShort( 0x1c ),	/* MIPS & PPC Stack size/offset = 28 */
#endif
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 608 */	NdrFcShort( 0x0 ),	/* 0 */
/* 610 */	NdrFcShort( 0x8 ),	/* 8 */
/* 612 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter varValue */

/* 614 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 616 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* MIPS & PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 618 */	NdrFcShort( 0x3e4 ),	/* Type Offset=996 */

	/* Return value */

/* 620 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 622 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
#else
			NdrFcShort( 0x18 ),	/* MIPS & PPC Stack size/offset = 24 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 624 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Graphic */

/* 626 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 628 */	NdrFcLong( 0x0 ),	/* 0 */
/* 632 */	NdrFcShort( 0x1e ),	/* 30 */
#ifndef _ALPHA_
/* 634 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 636 */	NdrFcShort( 0x0 ),	/* 0 */
/* 638 */	NdrFcShort( 0x8 ),	/* 8 */
/* 640 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter pbstrGraphic */

/* 642 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 644 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 646 */	NdrFcShort( 0x408 ),	/* Type Offset=1032 */

	/* Return value */

/* 648 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 650 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 652 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Graphic */

/* 654 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 656 */	NdrFcLong( 0x0 ),	/* 0 */
/* 660 */	NdrFcShort( 0x1f ),	/* 31 */
#ifndef _ALPHA_
/* 662 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 664 */	NdrFcShort( 0x0 ),	/* 0 */
/* 666 */	NdrFcShort( 0x8 ),	/* 8 */
/* 668 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter bstrGraphic */

/* 670 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 672 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 674 */	NdrFcShort( 0x30 ),	/* Type Offset=48 */

	/* Return value */

/* 676 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 678 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 680 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Owner */

/* 682 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 684 */	NdrFcLong( 0x0 ),	/* 0 */
/* 688 */	NdrFcShort( 0x20 ),	/* 32 */
#ifndef _ALPHA_
/* 690 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 692 */	NdrFcShort( 0x0 ),	/* 0 */
/* 694 */	NdrFcShort( 0x8 ),	/* 8 */
/* 696 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter ppOwner */

/* 698 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 700 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 702 */	NdrFcShort( 0x412 ),	/* Type Offset=1042 */

	/* Return value */

/* 704 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 706 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 708 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Owner */

/* 710 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 712 */	NdrFcLong( 0x0 ),	/* 0 */
/* 716 */	NdrFcShort( 0x21 ),	/* 33 */
#ifndef _ALPHA_
/* 718 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 720 */	NdrFcShort( 0x0 ),	/* 0 */
/* 722 */	NdrFcShort( 0x8 ),	/* 8 */
/* 724 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pOwner */

/* 726 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 728 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 730 */	NdrFcShort( 0x416 ),	/* Type Offset=1046 */

	/* Return value */

/* 732 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 734 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 736 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Access */

/* 738 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 740 */	NdrFcLong( 0x0 ),	/* 0 */
/* 744 */	NdrFcShort( 0x22 ),	/* 34 */
#ifndef _ALPHA_
/* 746 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 748 */	NdrFcShort( 0x0 ),	/* 0 */
/* 750 */	NdrFcShort( 0xe ),	/* 14 */
/* 752 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter sAccess */

/* 754 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 756 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 758 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 760 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 762 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 764 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Access */

/* 766 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 768 */	NdrFcLong( 0x0 ),	/* 0 */
/* 772 */	NdrFcShort( 0x23 ),	/* 35 */
#ifndef _ALPHA_
/* 774 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 776 */	NdrFcShort( 0x6 ),	/* 6 */
/* 778 */	NdrFcShort( 0x8 ),	/* 8 */
/* 780 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter sAccess */

/* 782 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 784 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 786 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 788 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 790 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 792 */	0x8,		/* FC_LONG */
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
			0x11, 0x10,	/* FC_RP */
/*  4 */	NdrFcShort( 0x2 ),	/* Offset= 2 (6) */
/*  6 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/*  8 */	NdrFcLong( 0x590d9846 ),	/* 1494063174 */
/* 12 */	NdrFcShort( 0xa417 ),	/* -23529 */
/* 14 */	NdrFcShort( 0x11cf ),	/* 4559 */
/* 16 */	0x9b,		/* 155 */
			0xf0,		/* 240 */
/* 18 */	0x0,		/* 0 */
			0x80,		/* 128 */
/* 20 */	0xc7,		/* 199 */
			0xa5,		/* 165 */
/* 22 */	0x6a,		/* 106 */
			0x8a,		/* 138 */
/* 24 */	
			0x12, 0x0,	/* FC_UP */
/* 26 */	NdrFcShort( 0xc ),	/* Offset= 12 (38) */
/* 28 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 30 */	NdrFcShort( 0x2 ),	/* 2 */
/* 32 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 34 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 36 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 38 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 40 */	NdrFcShort( 0x8 ),	/* 8 */
/* 42 */	NdrFcShort( 0xfffffff2 ),	/* Offset= -14 (28) */
/* 44 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 46 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 48 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 50 */	NdrFcShort( 0x0 ),	/* 0 */
/* 52 */	NdrFcShort( 0x4 ),	/* 4 */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (24) */
/* 58 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 60 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 62 */	
			0x12, 0x0,	/* FC_UP */
/* 64 */	NdrFcShort( 0x390 ),	/* Offset= 912 (976) */
/* 66 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 68 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 70 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 72 */	NdrFcShort( 0x2 ),	/* Offset= 2 (74) */
/* 74 */	NdrFcShort( 0x10 ),	/* 16 */
/* 76 */	NdrFcShort( 0x2b ),	/* 43 */
/* 78 */	NdrFcLong( 0x3 ),	/* 3 */
/* 82 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 84 */	NdrFcLong( 0x11 ),	/* 17 */
/* 88 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 90 */	NdrFcLong( 0x2 ),	/* 2 */
/* 94 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 96 */	NdrFcLong( 0x4 ),	/* 4 */
/* 100 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 102 */	NdrFcLong( 0x5 ),	/* 5 */
/* 106 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 108 */	NdrFcLong( 0xb ),	/* 11 */
/* 112 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 114 */	NdrFcLong( 0xa ),	/* 10 */
/* 118 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 120 */	NdrFcLong( 0x6 ),	/* 6 */
/* 124 */	NdrFcShort( 0xd6 ),	/* Offset= 214 (338) */
/* 126 */	NdrFcLong( 0x7 ),	/* 7 */
/* 130 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 132 */	NdrFcLong( 0x8 ),	/* 8 */
/* 136 */	NdrFcShort( 0xffffff90 ),	/* Offset= -112 (24) */
/* 138 */	NdrFcLong( 0xd ),	/* 13 */
/* 142 */	NdrFcShort( 0xca ),	/* Offset= 202 (344) */
/* 144 */	NdrFcLong( 0x9 ),	/* 9 */
/* 148 */	NdrFcShort( 0xd6 ),	/* Offset= 214 (362) */
/* 150 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 154 */	NdrFcShort( 0xe2 ),	/* Offset= 226 (380) */
/* 156 */	NdrFcLong( 0x24 ),	/* 36 */
/* 160 */	NdrFcShort( 0x2ec ),	/* Offset= 748 (908) */
/* 162 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 166 */	NdrFcShort( 0x2e6 ),	/* Offset= 742 (908) */
/* 168 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 172 */	NdrFcShort( 0x2e4 ),	/* Offset= 740 (912) */
/* 174 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 178 */	NdrFcShort( 0x2e2 ),	/* Offset= 738 (916) */
/* 180 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 184 */	NdrFcShort( 0x2e0 ),	/* Offset= 736 (920) */
/* 186 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 190 */	NdrFcShort( 0x2de ),	/* Offset= 734 (924) */
/* 192 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 196 */	NdrFcShort( 0x2dc ),	/* Offset= 732 (928) */
/* 198 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 202 */	NdrFcShort( 0x2ca ),	/* Offset= 714 (916) */
/* 204 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 208 */	NdrFcShort( 0x2c8 ),	/* Offset= 712 (920) */
/* 210 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 214 */	NdrFcShort( 0x2ce ),	/* Offset= 718 (932) */
/* 216 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 220 */	NdrFcShort( 0x2c4 ),	/* Offset= 708 (928) */
/* 222 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 226 */	NdrFcShort( 0x2c6 ),	/* Offset= 710 (936) */
/* 228 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 232 */	NdrFcShort( 0x2c4 ),	/* Offset= 708 (940) */
/* 234 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 238 */	NdrFcShort( 0x2c2 ),	/* Offset= 706 (944) */
/* 240 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 244 */	NdrFcShort( 0x2c0 ),	/* Offset= 704 (948) */
/* 246 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 250 */	NdrFcShort( 0x2be ),	/* Offset= 702 (952) */
/* 252 */	NdrFcLong( 0x10 ),	/* 16 */
/* 256 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 258 */	NdrFcLong( 0x12 ),	/* 18 */
/* 262 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 264 */	NdrFcLong( 0x13 ),	/* 19 */
/* 268 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 270 */	NdrFcLong( 0x16 ),	/* 22 */
/* 274 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 276 */	NdrFcLong( 0x17 ),	/* 23 */
/* 280 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 282 */	NdrFcLong( 0xe ),	/* 14 */
/* 286 */	NdrFcShort( 0x2a2 ),	/* Offset= 674 (960) */
/* 288 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 292 */	NdrFcShort( 0x2a8 ),	/* Offset= 680 (972) */
/* 294 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 298 */	NdrFcShort( 0x266 ),	/* Offset= 614 (912) */
/* 300 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 304 */	NdrFcShort( 0x264 ),	/* Offset= 612 (916) */
/* 306 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 310 */	NdrFcShort( 0x262 ),	/* Offset= 610 (920) */
/* 312 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 316 */	NdrFcShort( 0x25c ),	/* Offset= 604 (920) */
/* 318 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 322 */	NdrFcShort( 0x256 ),	/* Offset= 598 (920) */
/* 324 */	NdrFcLong( 0x0 ),	/* 0 */
/* 328 */	NdrFcShort( 0x0 ),	/* Offset= 0 (328) */
/* 330 */	NdrFcLong( 0x1 ),	/* 1 */
/* 334 */	NdrFcShort( 0x0 ),	/* Offset= 0 (334) */
/* 336 */	NdrFcShort( 0xffffffff ),	/* Offset= -1 (335) */
/* 338 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 340 */	NdrFcShort( 0x8 ),	/* 8 */
/* 342 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 344 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 346 */	NdrFcLong( 0x0 ),	/* 0 */
/* 350 */	NdrFcShort( 0x0 ),	/* 0 */
/* 352 */	NdrFcShort( 0x0 ),	/* 0 */
/* 354 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 356 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 358 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 360 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 362 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 364 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 368 */	NdrFcShort( 0x0 ),	/* 0 */
/* 370 */	NdrFcShort( 0x0 ),	/* 0 */
/* 372 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 374 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 376 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 378 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 380 */	
			0x12, 0x0,	/* FC_UP */
/* 382 */	NdrFcShort( 0x1fc ),	/* Offset= 508 (890) */
/* 384 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 386 */	NdrFcShort( 0x18 ),	/* 24 */
/* 388 */	NdrFcShort( 0xa ),	/* 10 */
/* 390 */	NdrFcLong( 0x8 ),	/* 8 */
/* 394 */	NdrFcShort( 0x58 ),	/* Offset= 88 (482) */
/* 396 */	NdrFcLong( 0xd ),	/* 13 */
/* 400 */	NdrFcShort( 0x78 ),	/* Offset= 120 (520) */
/* 402 */	NdrFcLong( 0x9 ),	/* 9 */
/* 406 */	NdrFcShort( 0x94 ),	/* Offset= 148 (554) */
/* 408 */	NdrFcLong( 0xc ),	/* 12 */
/* 412 */	NdrFcShort( 0xbc ),	/* Offset= 188 (600) */
/* 414 */	NdrFcLong( 0x24 ),	/* 36 */
/* 418 */	NdrFcShort( 0x114 ),	/* Offset= 276 (694) */
/* 420 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 424 */	NdrFcShort( 0x130 ),	/* Offset= 304 (728) */
/* 426 */	NdrFcLong( 0x10 ),	/* 16 */
/* 430 */	NdrFcShort( 0x148 ),	/* Offset= 328 (758) */
/* 432 */	NdrFcLong( 0x2 ),	/* 2 */
/* 436 */	NdrFcShort( 0x160 ),	/* Offset= 352 (788) */
/* 438 */	NdrFcLong( 0x3 ),	/* 3 */
/* 442 */	NdrFcShort( 0x178 ),	/* Offset= 376 (818) */
/* 444 */	NdrFcLong( 0x14 ),	/* 20 */
/* 448 */	NdrFcShort( 0x190 ),	/* Offset= 400 (848) */
/* 450 */	NdrFcShort( 0xffffffff ),	/* Offset= -1 (449) */
/* 452 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 454 */	NdrFcShort( 0x4 ),	/* 4 */
/* 456 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 458 */	NdrFcShort( 0x0 ),	/* 0 */
/* 460 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 462 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 464 */	NdrFcShort( 0x4 ),	/* 4 */
/* 466 */	NdrFcShort( 0x0 ),	/* 0 */
/* 468 */	NdrFcShort( 0x1 ),	/* 1 */
/* 470 */	NdrFcShort( 0x0 ),	/* 0 */
/* 472 */	NdrFcShort( 0x0 ),	/* 0 */
/* 474 */	0x12, 0x0,	/* FC_UP */
/* 476 */	NdrFcShort( 0xfffffe4a ),	/* Offset= -438 (38) */
/* 478 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 480 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 482 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 484 */	NdrFcShort( 0x8 ),	/* 8 */
/* 486 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 488 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 490 */	NdrFcShort( 0x4 ),	/* 4 */
/* 492 */	NdrFcShort( 0x4 ),	/* 4 */
/* 494 */	0x11, 0x0,	/* FC_RP */
/* 496 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (452) */
/* 498 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 500 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 502 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 504 */	NdrFcShort( 0x0 ),	/* 0 */
/* 506 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 508 */	NdrFcShort( 0x0 ),	/* 0 */
/* 510 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 514 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 516 */	NdrFcShort( 0xffffff54 ),	/* Offset= -172 (344) */
/* 518 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 520 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 522 */	NdrFcShort( 0x8 ),	/* 8 */
/* 524 */	NdrFcShort( 0x0 ),	/* 0 */
/* 526 */	NdrFcShort( 0x6 ),	/* Offset= 6 (532) */
/* 528 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 530 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 532 */	
			0x11, 0x0,	/* FC_RP */
/* 534 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (502) */
/* 536 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 538 */	NdrFcShort( 0x0 ),	/* 0 */
/* 540 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 542 */	NdrFcShort( 0x0 ),	/* 0 */
/* 544 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 548 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 550 */	NdrFcShort( 0xffffff44 ),	/* Offset= -188 (362) */
/* 552 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 554 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 556 */	NdrFcShort( 0x8 ),	/* 8 */
/* 558 */	NdrFcShort( 0x0 ),	/* 0 */
/* 560 */	NdrFcShort( 0x6 ),	/* Offset= 6 (566) */
/* 562 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 564 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 566 */	
			0x11, 0x0,	/* FC_RP */
/* 568 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (536) */
/* 570 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 572 */	NdrFcShort( 0x4 ),	/* 4 */
/* 574 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 576 */	NdrFcShort( 0x0 ),	/* 0 */
/* 578 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 580 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 582 */	NdrFcShort( 0x4 ),	/* 4 */
/* 584 */	NdrFcShort( 0x0 ),	/* 0 */
/* 586 */	NdrFcShort( 0x1 ),	/* 1 */
/* 588 */	NdrFcShort( 0x0 ),	/* 0 */
/* 590 */	NdrFcShort( 0x0 ),	/* 0 */
/* 592 */	0x12, 0x0,	/* FC_UP */
/* 594 */	NdrFcShort( 0x17e ),	/* Offset= 382 (976) */
/* 596 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 598 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 600 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 602 */	NdrFcShort( 0x8 ),	/* 8 */
/* 604 */	NdrFcShort( 0x0 ),	/* 0 */
/* 606 */	NdrFcShort( 0x6 ),	/* Offset= 6 (612) */
/* 608 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 610 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 612 */	
			0x11, 0x0,	/* FC_RP */
/* 614 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (570) */
/* 616 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 618 */	NdrFcLong( 0x2f ),	/* 47 */
/* 622 */	NdrFcShort( 0x0 ),	/* 0 */
/* 624 */	NdrFcShort( 0x0 ),	/* 0 */
/* 626 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 628 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 630 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 632 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 634 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 636 */	NdrFcShort( 0x1 ),	/* 1 */
/* 638 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 640 */	NdrFcShort( 0x4 ),	/* 4 */
/* 642 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 644 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 646 */	NdrFcShort( 0x10 ),	/* 16 */
/* 648 */	NdrFcShort( 0x0 ),	/* 0 */
/* 650 */	NdrFcShort( 0xa ),	/* Offset= 10 (660) */
/* 652 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 654 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 656 */	NdrFcShort( 0xffffffd8 ),	/* Offset= -40 (616) */
/* 658 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 660 */	
			0x12, 0x0,	/* FC_UP */
/* 662 */	NdrFcShort( 0xffffffe4 ),	/* Offset= -28 (634) */
/* 664 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 666 */	NdrFcShort( 0x4 ),	/* 4 */
/* 668 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 670 */	NdrFcShort( 0x0 ),	/* 0 */
/* 672 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 674 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 676 */	NdrFcShort( 0x4 ),	/* 4 */
/* 678 */	NdrFcShort( 0x0 ),	/* 0 */
/* 680 */	NdrFcShort( 0x1 ),	/* 1 */
/* 682 */	NdrFcShort( 0x0 ),	/* 0 */
/* 684 */	NdrFcShort( 0x0 ),	/* 0 */
/* 686 */	0x12, 0x0,	/* FC_UP */
/* 688 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (644) */
/* 690 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 692 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 694 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 696 */	NdrFcShort( 0x8 ),	/* 8 */
/* 698 */	NdrFcShort( 0x0 ),	/* 0 */
/* 700 */	NdrFcShort( 0x6 ),	/* Offset= 6 (706) */
/* 702 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 704 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 706 */	
			0x11, 0x0,	/* FC_RP */
/* 708 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (664) */
/* 710 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 712 */	NdrFcShort( 0x8 ),	/* 8 */
/* 714 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */
/* 716 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 718 */	NdrFcShort( 0x10 ),	/* 16 */
/* 720 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 722 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 724 */	0x0,		/* 0 */
			NdrFcShort( 0xfffffff1 ),	/* Offset= -15 (710) */
			0x5b,		/* FC_END */
/* 728 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 730 */	NdrFcShort( 0x18 ),	/* 24 */
/* 732 */	NdrFcShort( 0x0 ),	/* 0 */
/* 734 */	NdrFcShort( 0xa ),	/* Offset= 10 (744) */
/* 736 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 738 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 740 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (716) */
/* 742 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 744 */	
			0x11, 0x0,	/* FC_RP */
/* 746 */	NdrFcShort( 0xffffff0c ),	/* Offset= -244 (502) */
/* 748 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 750 */	NdrFcShort( 0x1 ),	/* 1 */
/* 752 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 754 */	NdrFcShort( 0x0 ),	/* 0 */
/* 756 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 758 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 760 */	NdrFcShort( 0x8 ),	/* 8 */
/* 762 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 764 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 766 */	NdrFcShort( 0x4 ),	/* 4 */
/* 768 */	NdrFcShort( 0x4 ),	/* 4 */
/* 770 */	0x12, 0x0,	/* FC_UP */
/* 772 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (748) */
/* 774 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 776 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 778 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 780 */	NdrFcShort( 0x2 ),	/* 2 */
/* 782 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 784 */	NdrFcShort( 0x0 ),	/* 0 */
/* 786 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 788 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 790 */	NdrFcShort( 0x8 ),	/* 8 */
/* 792 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 794 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 796 */	NdrFcShort( 0x4 ),	/* 4 */
/* 798 */	NdrFcShort( 0x4 ),	/* 4 */
/* 800 */	0x12, 0x0,	/* FC_UP */
/* 802 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (778) */
/* 804 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 806 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 808 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 810 */	NdrFcShort( 0x4 ),	/* 4 */
/* 812 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 814 */	NdrFcShort( 0x0 ),	/* 0 */
/* 816 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 818 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 820 */	NdrFcShort( 0x8 ),	/* 8 */
/* 822 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 824 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 826 */	NdrFcShort( 0x4 ),	/* 4 */
/* 828 */	NdrFcShort( 0x4 ),	/* 4 */
/* 830 */	0x12, 0x0,	/* FC_UP */
/* 832 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (808) */
/* 834 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 836 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 838 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 840 */	NdrFcShort( 0x8 ),	/* 8 */
/* 842 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 844 */	NdrFcShort( 0x0 ),	/* 0 */
/* 846 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 848 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 850 */	NdrFcShort( 0x8 ),	/* 8 */
/* 852 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 854 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 856 */	NdrFcShort( 0x4 ),	/* 4 */
/* 858 */	NdrFcShort( 0x4 ),	/* 4 */
/* 860 */	0x12, 0x0,	/* FC_UP */
/* 862 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (838) */
/* 864 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 866 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 868 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 870 */	NdrFcShort( 0x8 ),	/* 8 */
/* 872 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 874 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 876 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 878 */	NdrFcShort( 0x8 ),	/* 8 */
/* 880 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 882 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 884 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 886 */	NdrFcShort( 0xffffffee ),	/* Offset= -18 (868) */
/* 888 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 890 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 892 */	NdrFcShort( 0x28 ),	/* 40 */
/* 894 */	NdrFcShort( 0xffffffee ),	/* Offset= -18 (876) */
/* 896 */	NdrFcShort( 0x0 ),	/* Offset= 0 (896) */
/* 898 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 900 */	0x38,		/* FC_ALIGNM4 */
			0x8,		/* FC_LONG */
/* 902 */	0x8,		/* FC_LONG */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 904 */	0x0,		/* 0 */
			NdrFcShort( 0xfffffdf7 ),	/* Offset= -521 (384) */
			0x5b,		/* FC_END */
/* 908 */	
			0x12, 0x0,	/* FC_UP */
/* 910 */	NdrFcShort( 0xfffffef6 ),	/* Offset= -266 (644) */
/* 912 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 914 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 916 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 918 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 920 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 922 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 924 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 926 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 928 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 930 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 932 */	
			0x12, 0x0,	/* FC_UP */
/* 934 */	NdrFcShort( 0xfffffdac ),	/* Offset= -596 (338) */
/* 936 */	
			0x12, 0x10,	/* FC_UP */
/* 938 */	NdrFcShort( 0xfffffc6e ),	/* Offset= -914 (24) */
/* 940 */	
			0x12, 0x10,	/* FC_UP */
/* 942 */	NdrFcShort( 0xfffffdaa ),	/* Offset= -598 (344) */
/* 944 */	
			0x12, 0x10,	/* FC_UP */
/* 946 */	NdrFcShort( 0xfffffdb8 ),	/* Offset= -584 (362) */
/* 948 */	
			0x12, 0x10,	/* FC_UP */
/* 950 */	NdrFcShort( 0xfffffdc6 ),	/* Offset= -570 (380) */
/* 952 */	
			0x12, 0x10,	/* FC_UP */
/* 954 */	NdrFcShort( 0x2 ),	/* Offset= 2 (956) */
/* 956 */	
			0x12, 0x0,	/* FC_UP */
/* 958 */	NdrFcShort( 0xfffffc42 ),	/* Offset= -958 (0) */
/* 960 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 962 */	NdrFcShort( 0x10 ),	/* 16 */
/* 964 */	0x6,		/* FC_SHORT */
			0x2,		/* FC_CHAR */
/* 966 */	0x2,		/* FC_CHAR */
			0x38,		/* FC_ALIGNM4 */
/* 968 */	0x8,		/* FC_LONG */
			0x39,		/* FC_ALIGNM8 */
/* 970 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 972 */	
			0x12, 0x0,	/* FC_UP */
/* 974 */	NdrFcShort( 0xfffffff2 ),	/* Offset= -14 (960) */
/* 976 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 978 */	NdrFcShort( 0x20 ),	/* 32 */
/* 980 */	NdrFcShort( 0x0 ),	/* 0 */
/* 982 */	NdrFcShort( 0x0 ),	/* Offset= 0 (982) */
/* 984 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 986 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 988 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 990 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 992 */	NdrFcShort( 0xfffffc62 ),	/* Offset= -926 (66) */
/* 994 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 996 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 998 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1000 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1002 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1004 */	NdrFcShort( 0xfffffc52 ),	/* Offset= -942 (62) */
/* 1006 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 1008 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1014) */
/* 1010 */	
			0x13, 0x0,	/* FC_OP */
/* 1012 */	NdrFcShort( 0xffffffdc ),	/* Offset= -36 (976) */
/* 1014 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1016 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1018 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1020 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1022 */	NdrFcShort( 0xfffffff4 ),	/* Offset= -12 (1010) */
/* 1024 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 1026 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1032) */
/* 1028 */	
			0x13, 0x0,	/* FC_OP */
/* 1030 */	NdrFcShort( 0xfffffc20 ),	/* Offset= -992 (38) */
/* 1032 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1034 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1036 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1038 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1040 */	NdrFcShort( 0xfffffff4 ),	/* Offset= -12 (1028) */
/* 1042 */	
			0x11, 0x10,	/* FC_RP */
/* 1044 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1046) */
/* 1046 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1048 */	NdrFcLong( 0x590d9852 ),	/* 1494063186 */
/* 1052 */	NdrFcShort( 0xa417 ),	/* -23529 */
/* 1054 */	NdrFcShort( 0x11cf ),	/* 4559 */
/* 1056 */	0x9b,		/* 155 */
			0xf0,		/* 240 */
/* 1058 */	0x0,		/* 0 */
			0x80,		/* 128 */
/* 1060 */	0xc7,		/* 199 */
			0xa5,		/* 165 */
/* 1062 */	0x6a,		/* 106 */
			0x8a,		/* 138 */

			0x0
        }
    };

const CInterfaceProxyVtbl * _avprof_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IAvatarProfileProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IAvatarProfileDataProxyVtbl,
    0
};

const CInterfaceStubVtbl * _avprof_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IAvatarProfileStubVtbl,
    ( CInterfaceStubVtbl *) &_IAvatarProfileDataStubVtbl,
    0
};

PCInterfaceName const _avprof_InterfaceNamesList[] = 
{
    "IAvatarProfile",
    "IAvatarProfileData",
    0
};

const IID *  _avprof_BaseIIDList[] = 
{
    &IID_IObjectProperty,
    &IID_IObjectProperty,
    0
};


#define _avprof_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _avprof, pIID, n)

int __stdcall _avprof_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _avprof, 2, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _avprof, 2, *pIndex )
    
}

const ExtendedProxyFileInfo avprof_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _avprof_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _avprof_StubVtblList,
    (const PCInterfaceName * ) & _avprof_InterfaceNamesList,
    (const IID ** ) & _avprof_BaseIIDList,
    & _avprof_IID_Lookup, 
    2,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
