/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:47 2000
 */
/* Compiler settings for vwclient.idl:
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


#include "vwclient.h"

#define TYPE_FORMAT_STRING_SIZE   1117                              
#define PROC_FORMAT_STRING_SIZE   881                               

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


/* Standard interface: __MIDL_itf_vwclient_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IVWClient, ver. 0.0,
   GUID={0x590D9820,0xA417,0x11CF,{0x9B,0xF0,0x00,0x80,0xC7,0xA5,0x6A,0x8A}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IVWClient_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short IVWClient_FormatStringOffsetTable[] = 
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    22,
    44,
    84,
    118,
    152,
    192,
    226,
    260,
    282,
    310,
    338,
    366,
    400,
    440,
    480,
    508,
    542
    };

static const MIDL_SERVER_INFO IVWClient_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IVWClient_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IVWClient_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IVWClient_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(25) _IVWClientProxyVtbl = 
{
    &IVWClient_ProxyInfo,
    &IID_IVWClient,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *)-1 /* IVWClient::Initialize */ ,
    (void *)-1 /* IVWClient::Terminate */ ,
    (void *)-1 /* IVWClient::ConnectLocalEx */ ,
    (void *)-1 /* IVWClient::ConnectLocal */ ,
    (void *)-1 /* IVWClient::Connect */ ,
    (void *)-1 /* IVWClient::ConnectToServer */ ,
    (void *)-1 /* IVWClient::CreateWorld */ ,
    (void *)-1 /* IVWClient::OpenWorld */ ,
    (void *)-1 /* IVWClient::Disconnect */ ,
    (void *)-1 /* IVWClient::get_IsConnected */ ,
    (void *)-1 /* IVWClient::get_World */ ,
    (void *)-1 /* IVWClient::Trace */ ,
    (void *)-1 /* IVWClient::Report */ ,
    (void *)-1 /* IVWClient::CreateTool */ ,
    (void *)-1 /* IVWClient::CreateToolFromProgID */ ,
    (void *)-1 /* IVWClient::RemoveTool */ ,
    (void *)-1 /* IVWClient::get_Tool */ ,
    (void *)-1 /* IVWClient::get_Tools */
};


static const PRPC_STUB_FUNCTION IVWClient_table[] =
{
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

CInterfaceStubVtbl _IVWClientStubVtbl =
{
    &IID_IVWClient,
    &IVWClient_ServerInfo,
    25,
    &IVWClient_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IVWClientSite, ver. 0.0,
   GUID={0x590D9822,0xA417,0x11CF,{0x9B,0xF0,0x00,0x80,0xC7,0xA5,0x6A,0x8A}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IVWClientSite_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short IVWClientSite_FormatStringOffsetTable[] = 
    {
    570,
    598,
    626,
    654,
    682,
    722,
    750,
    784
    };

static const MIDL_SERVER_INFO IVWClientSite_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IVWClientSite_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IVWClientSite_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IVWClientSite_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(11) _IVWClientSiteProxyVtbl = 
{
    &IVWClientSite_ProxyInfo,
    &IID_IVWClientSite,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *)-1 /* IVWClientSite::OnConnect */ ,
    (void *)-1 /* IVWClientSite::OnDisconnect */ ,
    (void *)-1 /* IVWClientSite::OnUserConnect */ ,
    (void *)-1 /* IVWClientSite::OnUserDisconnect */ ,
    (void *)-1 /* IVWClientSite::OnUserReconnect */ ,
    (void *)-1 /* IVWClientSite::OnTrace */ ,
    (void *)-1 /* IVWClientSite::OnReport */ ,
    (void *)-1 /* IVWClientSite::OnUIEvent */
};

const CInterfaceStubVtbl _IVWClientSiteStubVtbl =
{
    &IID_IVWClientSite,
    &IVWClientSite_ServerInfo,
    11,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IVWClientOCX, ver. 0.0,
   GUID={0xD4898801,0x20EF,0x11D0,{0x8A,0x96,0x00,0xA0,0xC9,0x08,0x25,0x83}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IVWClientOCX_ServerInfo;

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

static const unsigned short IVWClientOCX_FormatStringOffsetTable[] = 
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    824,
    852
    };

static const MIDL_SERVER_INFO IVWClientOCX_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IVWClientOCX_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IVWClientOCX_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IVWClientOCX_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(9) _IVWClientOCXProxyVtbl = 
{
    &IVWClientOCX_ProxyInfo,
    &IID_IVWClientOCX,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *)-1 /* IVWClientOCX::get_VWClient */ ,
    (void *)-1 /* IVWClientOCX::get_Version */
};


static const PRPC_STUB_FUNCTION IVWClientOCX_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IVWClientOCXStubVtbl =
{
    &IID_IVWClientOCX,
    &IVWClientOCX_ServerInfo,
    9,
    &IVWClientOCX_table[-3],
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

	/* Procedure Initialize */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
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

	/* Procedure Terminate */

/* 22 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 24 */	NdrFcLong( 0x0 ),	/* 0 */
/* 28 */	NdrFcShort( 0x8 ),	/* 8 */
#ifndef _ALPHA_
/* 30 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 32 */	NdrFcShort( 0x0 ),	/* 0 */
/* 34 */	NdrFcShort( 0x8 ),	/* 8 */
/* 36 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */

/* 38 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 40 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 42 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ConnectLocalEx */

/* 44 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 46 */	NdrFcLong( 0x0 ),	/* 0 */
/* 50 */	NdrFcShort( 0x9 ),	/* 9 */
#ifndef _ALPHA_
/* 52 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 54 */	NdrFcShort( 0x6 ),	/* 6 */
/* 56 */	NdrFcShort( 0x8 ),	/* 8 */
/* 58 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter bstr */

/* 60 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 62 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 64 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter bOverwrite */

/* 66 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 68 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 70 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter ppworld */

/* 72 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 74 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 76 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */

	/* Return value */

/* 78 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 80 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 82 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ConnectLocal */

/* 84 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 86 */	NdrFcLong( 0x0 ),	/* 0 */
/* 90 */	NdrFcShort( 0xa ),	/* 10 */
#ifndef _ALPHA_
/* 92 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 94 */	NdrFcShort( 0x0 ),	/* 0 */
/* 96 */	NdrFcShort( 0x8 ),	/* 8 */
/* 98 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstr */

/* 100 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 102 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 104 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter ppworld */

/* 106 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 108 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 110 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */

	/* Return value */

/* 112 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 114 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 116 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Connect */

/* 118 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 120 */	NdrFcLong( 0x0 ),	/* 0 */
/* 124 */	NdrFcShort( 0xb ),	/* 11 */
#ifndef _ALPHA_
/* 126 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 128 */	NdrFcShort( 0x0 ),	/* 0 */
/* 130 */	NdrFcShort( 0x8 ),	/* 8 */
/* 132 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrURL */

/* 134 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 136 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 138 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter ppworld */

/* 140 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 142 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 144 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */

	/* Return value */

/* 146 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 148 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 150 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ConnectToServer */

/* 152 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 154 */	NdrFcLong( 0x0 ),	/* 0 */
/* 158 */	NdrFcShort( 0xc ),	/* 12 */
#ifndef _ALPHA_
/* 160 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 162 */	NdrFcShort( 0x6 ),	/* 6 */
/* 164 */	NdrFcShort( 0x8 ),	/* 8 */
/* 166 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter bstrURL */

/* 168 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 170 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 172 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter bAuthenticated */

/* 174 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 176 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 178 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter ppadmin */

/* 180 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 182 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 184 */	NdrFcShort( 0x3a ),	/* Type Offset=58 */

	/* Return value */

/* 186 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 188 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 190 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateWorld */

/* 192 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 194 */	NdrFcLong( 0x0 ),	/* 0 */
/* 198 */	NdrFcShort( 0xd ),	/* 13 */
#ifndef _ALPHA_
/* 200 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 202 */	NdrFcShort( 0x0 ),	/* 0 */
/* 204 */	NdrFcShort( 0x8 ),	/* 8 */
/* 206 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrWorldName */

/* 208 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 210 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 212 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter ppworld */

/* 214 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 216 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 218 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */

	/* Return value */

/* 220 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 222 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 224 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OpenWorld */

/* 226 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 228 */	NdrFcLong( 0x0 ),	/* 0 */
/* 232 */	NdrFcShort( 0xe ),	/* 14 */
#ifndef _ALPHA_
/* 234 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 236 */	NdrFcShort( 0x0 ),	/* 0 */
/* 238 */	NdrFcShort( 0x8 ),	/* 8 */
/* 240 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstrWorldName */

/* 242 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 244 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 246 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter ppworld */

/* 248 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 250 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 252 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */

	/* Return value */

/* 254 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 256 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 258 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Disconnect */

/* 260 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 262 */	NdrFcLong( 0x0 ),	/* 0 */
/* 266 */	NdrFcShort( 0xf ),	/* 15 */
#ifndef _ALPHA_
/* 268 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 270 */	NdrFcShort( 0x0 ),	/* 0 */
/* 272 */	NdrFcShort( 0x8 ),	/* 8 */
/* 274 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */

/* 276 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 278 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 280 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_IsConnected */

/* 282 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 284 */	NdrFcLong( 0x0 ),	/* 0 */
/* 288 */	NdrFcShort( 0x10 ),	/* 16 */
#ifndef _ALPHA_
/* 290 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 292 */	NdrFcShort( 0x0 ),	/* 0 */
/* 294 */	NdrFcShort( 0xe ),	/* 14 */
/* 296 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter pbool */

/* 298 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 300 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 302 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 304 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 306 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 308 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_World */

/* 310 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 312 */	NdrFcLong( 0x0 ),	/* 0 */
/* 316 */	NdrFcShort( 0x11 ),	/* 17 */
#ifndef _ALPHA_
/* 318 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 320 */	NdrFcShort( 0x0 ),	/* 0 */
/* 322 */	NdrFcShort( 0x8 ),	/* 8 */
/* 324 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter ppworld */

/* 326 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 328 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 330 */	NdrFcShort( 0x24 ),	/* Type Offset=36 */

	/* Return value */

/* 332 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 334 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 336 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Trace */

/* 338 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 340 */	NdrFcLong( 0x0 ),	/* 0 */
/* 344 */	NdrFcShort( 0x12 ),	/* 18 */
#ifndef _ALPHA_
/* 346 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 348 */	NdrFcShort( 0x0 ),	/* 0 */
/* 350 */	NdrFcShort( 0x8 ),	/* 8 */
/* 352 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter bstr */

/* 354 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 356 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 358 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Return value */

/* 360 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 362 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 364 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Report */

/* 366 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 368 */	NdrFcLong( 0x0 ),	/* 0 */
/* 372 */	NdrFcShort( 0x13 ),	/* 19 */
#ifndef _ALPHA_
/* 374 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 376 */	NdrFcShort( 0x8 ),	/* 8 */
/* 378 */	NdrFcShort( 0x8 ),	/* 8 */
/* 380 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstr */

/* 382 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 384 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 386 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter lType */

/* 388 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 390 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 392 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 394 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 396 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 398 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateTool */

/* 400 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 402 */	NdrFcLong( 0x0 ),	/* 0 */
/* 406 */	NdrFcShort( 0x14 ),	/* 20 */
#ifndef _ALPHA_
/* 408 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 410 */	NdrFcShort( 0x20 ),	/* 32 */
/* 412 */	NdrFcShort( 0x8 ),	/* 8 */
/* 414 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter name */

/* 416 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 418 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 420 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter clsid */

/* 422 */	NdrFcShort( 0x10a ),	/* Flags:  must free, in, simple ref, */
#ifndef _ALPHA_
/* 424 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 426 */	NdrFcShort( 0x5e ),	/* Type Offset=94 */

	/* Parameter ppunk */

/* 428 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 430 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 432 */	NdrFcShort( 0x6a ),	/* Type Offset=106 */

	/* Return value */

/* 434 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 436 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 438 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateToolFromProgID */

/* 440 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 442 */	NdrFcLong( 0x0 ),	/* 0 */
/* 446 */	NdrFcShort( 0x15 ),	/* 21 */
#ifndef _ALPHA_
/* 448 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 450 */	NdrFcShort( 0x0 ),	/* 0 */
/* 452 */	NdrFcShort( 0x8 ),	/* 8 */
/* 454 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter name */

/* 456 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 458 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 460 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter progid */

/* 462 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 464 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 466 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter ppunk */

/* 468 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 470 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 472 */	NdrFcShort( 0x6a ),	/* Type Offset=106 */

	/* Return value */

/* 474 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 476 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 478 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RemoveTool */

/* 480 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 482 */	NdrFcLong( 0x0 ),	/* 0 */
/* 486 */	NdrFcShort( 0x16 ),	/* 22 */
#ifndef _ALPHA_
/* 488 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 490 */	NdrFcShort( 0x0 ),	/* 0 */
/* 492 */	NdrFcShort( 0x8 ),	/* 8 */
/* 494 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter name */

/* 496 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 498 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 500 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Return value */

/* 502 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 504 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 506 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Tool */

/* 508 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 510 */	NdrFcLong( 0x0 ),	/* 0 */
/* 514 */	NdrFcShort( 0x17 ),	/* 23 */
#ifndef _ALPHA_
/* 516 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 518 */	NdrFcShort( 0x0 ),	/* 0 */
/* 520 */	NdrFcShort( 0x8 ),	/* 8 */
/* 522 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter name */

/* 524 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 526 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 528 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter ppunk */

/* 530 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 532 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 534 */	NdrFcShort( 0x6a ),	/* Type Offset=106 */

	/* Return value */

/* 536 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 538 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 540 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Tools */

/* 542 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 544 */	NdrFcLong( 0x0 ),	/* 0 */
/* 548 */	NdrFcShort( 0x18 ),	/* 24 */
#ifndef _ALPHA_
/* 550 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 552 */	NdrFcShort( 0x0 ),	/* 0 */
/* 554 */	NdrFcShort( 0x8 ),	/* 8 */
/* 556 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter pppropmap */

/* 558 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 560 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 562 */	NdrFcShort( 0x80 ),	/* Type Offset=128 */

	/* Return value */

/* 564 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 566 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 568 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnConnect */

/* 570 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 572 */	NdrFcLong( 0x0 ),	/* 0 */
/* 576 */	NdrFcShort( 0x3 ),	/* 3 */
#ifndef _ALPHA_
/* 578 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 580 */	NdrFcShort( 0x0 ),	/* 0 */
/* 582 */	NdrFcShort( 0x8 ),	/* 8 */
/* 584 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pWorld */

/* 586 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 588 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 590 */	NdrFcShort( 0x28 ),	/* Type Offset=40 */

	/* Return value */

/* 592 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 594 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 596 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnDisconnect */

/* 598 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 600 */	NdrFcLong( 0x0 ),	/* 0 */
/* 604 */	NdrFcShort( 0x4 ),	/* 4 */
#ifndef _ALPHA_
/* 606 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 608 */	NdrFcShort( 0x0 ),	/* 0 */
/* 610 */	NdrFcShort( 0x8 ),	/* 8 */
/* 612 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pWorld */

/* 614 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 616 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 618 */	NdrFcShort( 0x28 ),	/* Type Offset=40 */

	/* Return value */

/* 620 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 622 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 624 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnUserConnect */

/* 626 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 628 */	NdrFcLong( 0x0 ),	/* 0 */
/* 632 */	NdrFcShort( 0x5 ),	/* 5 */
#ifndef _ALPHA_
/* 634 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 636 */	NdrFcShort( 0x0 ),	/* 0 */
/* 638 */	NdrFcShort( 0x8 ),	/* 8 */
/* 640 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pthing */

/* 642 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 644 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 646 */	NdrFcShort( 0x96 ),	/* Type Offset=150 */

	/* Return value */

/* 648 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 650 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 652 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnUserDisconnect */

/* 654 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 656 */	NdrFcLong( 0x0 ),	/* 0 */
/* 660 */	NdrFcShort( 0x6 ),	/* 6 */
#ifndef _ALPHA_
/* 662 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 664 */	NdrFcShort( 0x0 ),	/* 0 */
/* 666 */	NdrFcShort( 0x8 ),	/* 8 */
/* 668 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pthing */

/* 670 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 672 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 674 */	NdrFcShort( 0x96 ),	/* Type Offset=150 */

	/* Return value */

/* 676 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 678 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 680 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnUserReconnect */

/* 682 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 684 */	NdrFcLong( 0x0 ),	/* 0 */
/* 688 */	NdrFcShort( 0x7 ),	/* 7 */
#ifndef _ALPHA_
/* 690 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 692 */	NdrFcShort( 0x0 ),	/* 0 */
/* 694 */	NdrFcShort( 0x8 ),	/* 8 */
/* 696 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter pNewClient */

/* 698 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 700 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 702 */	NdrFcShort( 0xa8 ),	/* Type Offset=168 */

	/* Parameter pNewWorld */

/* 704 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 706 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 708 */	NdrFcShort( 0x28 ),	/* Type Offset=40 */

	/* Parameter pNewThing */

/* 710 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 712 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 714 */	NdrFcShort( 0x96 ),	/* Type Offset=150 */

	/* Return value */

/* 716 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 718 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 720 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnTrace */

/* 722 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 724 */	NdrFcLong( 0x0 ),	/* 0 */
/* 728 */	NdrFcShort( 0x8 ),	/* 8 */
#ifndef _ALPHA_
/* 730 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 732 */	NdrFcShort( 0x0 ),	/* 0 */
/* 734 */	NdrFcShort( 0x8 ),	/* 8 */
/* 736 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter bstr */

/* 738 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 740 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 742 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Return value */

/* 744 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 746 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 748 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnReport */

/* 750 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 752 */	NdrFcLong( 0x0 ),	/* 0 */
/* 756 */	NdrFcShort( 0x9 ),	/* 9 */
#ifndef _ALPHA_
/* 758 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 760 */	NdrFcShort( 0x8 ),	/* 8 */
/* 762 */	NdrFcShort( 0x8 ),	/* 8 */
/* 764 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter bstr */

/* 766 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 768 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 770 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter lVal */

/* 772 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 774 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 776 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 778 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 780 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 782 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnUIEvent */

/* 784 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 786 */	NdrFcLong( 0x0 ),	/* 0 */
/* 790 */	NdrFcShort( 0xa ),	/* 10 */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 792 */	NdrFcShort( 0x20 ),	/* x86 Stack size/offset = 32 */
#else
			NdrFcShort( 0x24 ),	/* MIPS & PPC Stack size/offset = 36 */
#endif
#else
			NdrFcShort( 0x30 ),	/* Alpha Stack size/offset = 48 */
#endif
/* 794 */	NdrFcShort( 0x0 ),	/* 0 */
/* 796 */	NdrFcShort( 0x8 ),	/* 8 */
/* 798 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter pthing */

/* 800 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 802 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 804 */	NdrFcShort( 0x96 ),	/* Type Offset=150 */

	/* Parameter bstrEventName */

/* 806 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 808 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 810 */	NdrFcShort( 0x1a ),	/* Type Offset=26 */

	/* Parameter varArg */

/* 812 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 814 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
#else
			NdrFcShort( 0x10 ),	/* MIPS & PPC Stack size/offset = 16 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 816 */	NdrFcShort( 0x43c ),	/* Type Offset=1084 */

	/* Return value */

/* 818 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_) && !defined(_PPC_)
/* 820 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
#else
			NdrFcShort( 0x20 ),	/* MIPS & PPC Stack size/offset = 32 */
#endif
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 822 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_VWClient */

/* 824 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 826 */	NdrFcLong( 0x0 ),	/* 0 */
/* 830 */	NdrFcShort( 0x7 ),	/* 7 */
#ifndef _ALPHA_
/* 832 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 834 */	NdrFcShort( 0x0 ),	/* 0 */
/* 836 */	NdrFcShort( 0x8 ),	/* 8 */
/* 838 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter pVWClient */

/* 840 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
#ifndef _ALPHA_
/* 842 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 844 */	NdrFcShort( 0x446 ),	/* Type Offset=1094 */

	/* Return value */

/* 846 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 848 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 850 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Version */

/* 852 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 854 */	NdrFcLong( 0x0 ),	/* 0 */
/* 858 */	NdrFcShort( 0x8 ),	/* 8 */
#ifndef _ALPHA_
/* 860 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 862 */	NdrFcShort( 0x0 ),	/* 0 */
/* 864 */	NdrFcShort( 0x8 ),	/* 8 */
/* 866 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter pbstrVersion */

/* 868 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 870 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 872 */	NdrFcShort( 0x452 ),	/* Type Offset=1106 */

	/* Return value */

/* 874 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 876 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 878 */	0x8,		/* FC_LONG */
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
/*  4 */	NdrFcShort( 0xc ),	/* Offset= 12 (16) */
/*  6 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/*  8 */	NdrFcShort( 0x2 ),	/* 2 */
/* 10 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 12 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 14 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 16 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 18 */	NdrFcShort( 0x8 ),	/* 8 */
/* 20 */	NdrFcShort( 0xfffffff2 ),	/* Offset= -14 (6) */
/* 22 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 24 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 26 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 28 */	NdrFcShort( 0x0 ),	/* 0 */
/* 30 */	NdrFcShort( 0x4 ),	/* 4 */
/* 32 */	NdrFcShort( 0x0 ),	/* 0 */
/* 34 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (2) */
/* 36 */	
			0x11, 0x10,	/* FC_RP */
/* 38 */	NdrFcShort( 0x2 ),	/* Offset= 2 (40) */
/* 40 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 42 */	NdrFcLong( 0x590d9854 ),	/* 1494063188 */
/* 46 */	NdrFcShort( 0xa417 ),	/* -23529 */
/* 48 */	NdrFcShort( 0x11cf ),	/* 4559 */
/* 50 */	0x9b,		/* 155 */
			0xf0,		/* 240 */
/* 52 */	0x0,		/* 0 */
			0x80,		/* 128 */
/* 54 */	0xc7,		/* 199 */
			0xa5,		/* 165 */
/* 56 */	0x6a,		/* 106 */
			0x8a,		/* 138 */
/* 58 */	
			0x11, 0x10,	/* FC_RP */
/* 60 */	NdrFcShort( 0x2 ),	/* Offset= 2 (62) */
/* 62 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 64 */	NdrFcLong( 0x2043f40 ),	/* 33832768 */
/* 68 */	NdrFcShort( 0x2b3 ),	/* 691 */
/* 70 */	NdrFcShort( 0x11d2 ),	/* 4562 */
/* 72 */	0x80,		/* 128 */
			0xf5,		/* 245 */
/* 74 */	0x0,		/* 0 */
			0xc0,		/* 192 */
/* 76 */	0x4f,		/* 79 */
			0xad,		/* 173 */
/* 78 */	0x7d,		/* 125 */
			0x6d,		/* 109 */
/* 80 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 82 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 84 */	
			0x11, 0x0,	/* FC_RP */
/* 86 */	NdrFcShort( 0x8 ),	/* Offset= 8 (94) */
/* 88 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 90 */	NdrFcShort( 0x8 ),	/* 8 */
/* 92 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */
/* 94 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 96 */	NdrFcShort( 0x10 ),	/* 16 */
/* 98 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 100 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 102 */	0x0,		/* 0 */
			NdrFcShort( 0xfffffff1 ),	/* Offset= -15 (88) */
			0x5b,		/* FC_END */
/* 106 */	
			0x11, 0x10,	/* FC_RP */
/* 108 */	NdrFcShort( 0x2 ),	/* Offset= 2 (110) */
/* 110 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 112 */	NdrFcLong( 0x0 ),	/* 0 */
/* 116 */	NdrFcShort( 0x0 ),	/* 0 */
/* 118 */	NdrFcShort( 0x0 ),	/* 0 */
/* 120 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 122 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 124 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 126 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 128 */	
			0x11, 0x10,	/* FC_RP */
/* 130 */	NdrFcShort( 0x2 ),	/* Offset= 2 (132) */
/* 132 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 134 */	NdrFcLong( 0x590d9848 ),	/* 1494063176 */
/* 138 */	NdrFcShort( 0xa417 ),	/* -23529 */
/* 140 */	NdrFcShort( 0x11cf ),	/* 4559 */
/* 142 */	0x9b,		/* 155 */
			0xf0,		/* 240 */
/* 144 */	0x0,		/* 0 */
			0x80,		/* 128 */
/* 146 */	0xc7,		/* 199 */
			0xa5,		/* 165 */
/* 148 */	0x6a,		/* 106 */
			0x8a,		/* 138 */
/* 150 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 152 */	NdrFcLong( 0x590d9852 ),	/* 1494063186 */
/* 156 */	NdrFcShort( 0xa417 ),	/* -23529 */
/* 158 */	NdrFcShort( 0x11cf ),	/* 4559 */
/* 160 */	0x9b,		/* 155 */
			0xf0,		/* 240 */
/* 162 */	0x0,		/* 0 */
			0x80,		/* 128 */
/* 164 */	0xc7,		/* 199 */
			0xa5,		/* 165 */
/* 166 */	0x6a,		/* 106 */
			0x8a,		/* 138 */
/* 168 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 170 */	NdrFcLong( 0x590d9820 ),	/* 1494063136 */
/* 174 */	NdrFcShort( 0xa417 ),	/* -23529 */
/* 176 */	NdrFcShort( 0x11cf ),	/* 4559 */
/* 178 */	0x9b,		/* 155 */
			0xf0,		/* 240 */
/* 180 */	0x0,		/* 0 */
			0x80,		/* 128 */
/* 182 */	0xc7,		/* 199 */
			0xa5,		/* 165 */
/* 184 */	0x6a,		/* 106 */
			0x8a,		/* 138 */
/* 186 */	
			0x12, 0x0,	/* FC_UP */
/* 188 */	NdrFcShort( 0x36c ),	/* Offset= 876 (1064) */
/* 190 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 192 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 194 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 196 */	NdrFcShort( 0x2 ),	/* Offset= 2 (198) */
/* 198 */	NdrFcShort( 0x10 ),	/* 16 */
/* 200 */	NdrFcShort( 0x2b ),	/* 43 */
/* 202 */	NdrFcLong( 0x3 ),	/* 3 */
/* 206 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 208 */	NdrFcLong( 0x11 ),	/* 17 */
/* 212 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 214 */	NdrFcLong( 0x2 ),	/* 2 */
/* 218 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 220 */	NdrFcLong( 0x4 ),	/* 4 */
/* 224 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 226 */	NdrFcLong( 0x5 ),	/* 5 */
/* 230 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 232 */	NdrFcLong( 0xb ),	/* 11 */
/* 236 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 238 */	NdrFcLong( 0xa ),	/* 10 */
/* 242 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 244 */	NdrFcLong( 0x6 ),	/* 6 */
/* 248 */	NdrFcShort( 0xd6 ),	/* Offset= 214 (462) */
/* 250 */	NdrFcLong( 0x7 ),	/* 7 */
/* 254 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 256 */	NdrFcLong( 0x8 ),	/* 8 */
/* 260 */	NdrFcShort( 0xfffffefe ),	/* Offset= -258 (2) */
/* 262 */	NdrFcLong( 0xd ),	/* 13 */
/* 266 */	NdrFcShort( 0xffffff64 ),	/* Offset= -156 (110) */
/* 268 */	NdrFcLong( 0x9 ),	/* 9 */
/* 272 */	NdrFcShort( 0xc4 ),	/* Offset= 196 (468) */
/* 274 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 278 */	NdrFcShort( 0xd0 ),	/* Offset= 208 (486) */
/* 280 */	NdrFcLong( 0x24 ),	/* 36 */
/* 284 */	NdrFcShort( 0x2c8 ),	/* Offset= 712 (996) */
/* 286 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 290 */	NdrFcShort( 0x2c2 ),	/* Offset= 706 (996) */
/* 292 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 296 */	NdrFcShort( 0x2c0 ),	/* Offset= 704 (1000) */
/* 298 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 302 */	NdrFcShort( 0x2be ),	/* Offset= 702 (1004) */
/* 304 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 308 */	NdrFcShort( 0x2bc ),	/* Offset= 700 (1008) */
/* 310 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 314 */	NdrFcShort( 0x2ba ),	/* Offset= 698 (1012) */
/* 316 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 320 */	NdrFcShort( 0x2b8 ),	/* Offset= 696 (1016) */
/* 322 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 326 */	NdrFcShort( 0x2a6 ),	/* Offset= 678 (1004) */
/* 328 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 332 */	NdrFcShort( 0x2a4 ),	/* Offset= 676 (1008) */
/* 334 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 338 */	NdrFcShort( 0x2aa ),	/* Offset= 682 (1020) */
/* 340 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 344 */	NdrFcShort( 0x2a0 ),	/* Offset= 672 (1016) */
/* 346 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 350 */	NdrFcShort( 0x2a2 ),	/* Offset= 674 (1024) */
/* 352 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 356 */	NdrFcShort( 0x2a0 ),	/* Offset= 672 (1028) */
/* 358 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 362 */	NdrFcShort( 0x29e ),	/* Offset= 670 (1032) */
/* 364 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 368 */	NdrFcShort( 0x29c ),	/* Offset= 668 (1036) */
/* 370 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 374 */	NdrFcShort( 0x29a ),	/* Offset= 666 (1040) */
/* 376 */	NdrFcLong( 0x10 ),	/* 16 */
/* 380 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 382 */	NdrFcLong( 0x12 ),	/* 18 */
/* 386 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 388 */	NdrFcLong( 0x13 ),	/* 19 */
/* 392 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 394 */	NdrFcLong( 0x16 ),	/* 22 */
/* 398 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 400 */	NdrFcLong( 0x17 ),	/* 23 */
/* 404 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 406 */	NdrFcLong( 0xe ),	/* 14 */
/* 410 */	NdrFcShort( 0x27e ),	/* Offset= 638 (1048) */
/* 412 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 416 */	NdrFcShort( 0x284 ),	/* Offset= 644 (1060) */
/* 418 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 422 */	NdrFcShort( 0x242 ),	/* Offset= 578 (1000) */
/* 424 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 428 */	NdrFcShort( 0x240 ),	/* Offset= 576 (1004) */
/* 430 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 434 */	NdrFcShort( 0x23e ),	/* Offset= 574 (1008) */
/* 436 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 440 */	NdrFcShort( 0x238 ),	/* Offset= 568 (1008) */
/* 442 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 446 */	NdrFcShort( 0x232 ),	/* Offset= 562 (1008) */
/* 448 */	NdrFcLong( 0x0 ),	/* 0 */
/* 452 */	NdrFcShort( 0x0 ),	/* Offset= 0 (452) */
/* 454 */	NdrFcLong( 0x1 ),	/* 1 */
/* 458 */	NdrFcShort( 0x0 ),	/* Offset= 0 (458) */
/* 460 */	NdrFcShort( 0xffffffff ),	/* Offset= -1 (459) */
/* 462 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 464 */	NdrFcShort( 0x8 ),	/* 8 */
/* 466 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 468 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 470 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 474 */	NdrFcShort( 0x0 ),	/* 0 */
/* 476 */	NdrFcShort( 0x0 ),	/* 0 */
/* 478 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 480 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 482 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 484 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 486 */	
			0x12, 0x0,	/* FC_UP */
/* 488 */	NdrFcShort( 0x1ea ),	/* Offset= 490 (978) */
/* 490 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 492 */	NdrFcShort( 0x18 ),	/* 24 */
/* 494 */	NdrFcShort( 0xa ),	/* 10 */
/* 496 */	NdrFcLong( 0x8 ),	/* 8 */
/* 500 */	NdrFcShort( 0x58 ),	/* Offset= 88 (588) */
/* 502 */	NdrFcLong( 0xd ),	/* 13 */
/* 506 */	NdrFcShort( 0x78 ),	/* Offset= 120 (626) */
/* 508 */	NdrFcLong( 0x9 ),	/* 9 */
/* 512 */	NdrFcShort( 0x94 ),	/* Offset= 148 (660) */
/* 514 */	NdrFcLong( 0xc ),	/* 12 */
/* 518 */	NdrFcShort( 0xbc ),	/* Offset= 188 (706) */
/* 520 */	NdrFcLong( 0x24 ),	/* 36 */
/* 524 */	NdrFcShort( 0x114 ),	/* Offset= 276 (800) */
/* 526 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 530 */	NdrFcShort( 0x11e ),	/* Offset= 286 (816) */
/* 532 */	NdrFcLong( 0x10 ),	/* 16 */
/* 536 */	NdrFcShort( 0x136 ),	/* Offset= 310 (846) */
/* 538 */	NdrFcLong( 0x2 ),	/* 2 */
/* 542 */	NdrFcShort( 0x14e ),	/* Offset= 334 (876) */
/* 544 */	NdrFcLong( 0x3 ),	/* 3 */
/* 548 */	NdrFcShort( 0x166 ),	/* Offset= 358 (906) */
/* 550 */	NdrFcLong( 0x14 ),	/* 20 */
/* 554 */	NdrFcShort( 0x17e ),	/* Offset= 382 (936) */
/* 556 */	NdrFcShort( 0xffffffff ),	/* Offset= -1 (555) */
/* 558 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 560 */	NdrFcShort( 0x4 ),	/* 4 */
/* 562 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 564 */	NdrFcShort( 0x0 ),	/* 0 */
/* 566 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 568 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 570 */	NdrFcShort( 0x4 ),	/* 4 */
/* 572 */	NdrFcShort( 0x0 ),	/* 0 */
/* 574 */	NdrFcShort( 0x1 ),	/* 1 */
/* 576 */	NdrFcShort( 0x0 ),	/* 0 */
/* 578 */	NdrFcShort( 0x0 ),	/* 0 */
/* 580 */	0x12, 0x0,	/* FC_UP */
/* 582 */	NdrFcShort( 0xfffffdca ),	/* Offset= -566 (16) */
/* 584 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 586 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 588 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 590 */	NdrFcShort( 0x8 ),	/* 8 */
/* 592 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 594 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 596 */	NdrFcShort( 0x4 ),	/* 4 */
/* 598 */	NdrFcShort( 0x4 ),	/* 4 */
/* 600 */	0x11, 0x0,	/* FC_RP */
/* 602 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (558) */
/* 604 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 606 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 608 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 610 */	NdrFcShort( 0x0 ),	/* 0 */
/* 612 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 614 */	NdrFcShort( 0x0 ),	/* 0 */
/* 616 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 620 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 622 */	NdrFcShort( 0xfffffe00 ),	/* Offset= -512 (110) */
/* 624 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 626 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 628 */	NdrFcShort( 0x8 ),	/* 8 */
/* 630 */	NdrFcShort( 0x0 ),	/* 0 */
/* 632 */	NdrFcShort( 0x6 ),	/* Offset= 6 (638) */
/* 634 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 636 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 638 */	
			0x11, 0x0,	/* FC_RP */
/* 640 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (608) */
/* 642 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 644 */	NdrFcShort( 0x0 ),	/* 0 */
/* 646 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 648 */	NdrFcShort( 0x0 ),	/* 0 */
/* 650 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 654 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 656 */	NdrFcShort( 0xffffff44 ),	/* Offset= -188 (468) */
/* 658 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 660 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 662 */	NdrFcShort( 0x8 ),	/* 8 */
/* 664 */	NdrFcShort( 0x0 ),	/* 0 */
/* 666 */	NdrFcShort( 0x6 ),	/* Offset= 6 (672) */
/* 668 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 670 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 672 */	
			0x11, 0x0,	/* FC_RP */
/* 674 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (642) */
/* 676 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 678 */	NdrFcShort( 0x4 ),	/* 4 */
/* 680 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 682 */	NdrFcShort( 0x0 ),	/* 0 */
/* 684 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 686 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 688 */	NdrFcShort( 0x4 ),	/* 4 */
/* 690 */	NdrFcShort( 0x0 ),	/* 0 */
/* 692 */	NdrFcShort( 0x1 ),	/* 1 */
/* 694 */	NdrFcShort( 0x0 ),	/* 0 */
/* 696 */	NdrFcShort( 0x0 ),	/* 0 */
/* 698 */	0x12, 0x0,	/* FC_UP */
/* 700 */	NdrFcShort( 0x16c ),	/* Offset= 364 (1064) */
/* 702 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 704 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 706 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 708 */	NdrFcShort( 0x8 ),	/* 8 */
/* 710 */	NdrFcShort( 0x0 ),	/* 0 */
/* 712 */	NdrFcShort( 0x6 ),	/* Offset= 6 (718) */
/* 714 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 716 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 718 */	
			0x11, 0x0,	/* FC_RP */
/* 720 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (676) */
/* 722 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 724 */	NdrFcLong( 0x2f ),	/* 47 */
/* 728 */	NdrFcShort( 0x0 ),	/* 0 */
/* 730 */	NdrFcShort( 0x0 ),	/* 0 */
/* 732 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 734 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 736 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 738 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 740 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 742 */	NdrFcShort( 0x1 ),	/* 1 */
/* 744 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 746 */	NdrFcShort( 0x4 ),	/* 4 */
/* 748 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 750 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 752 */	NdrFcShort( 0x10 ),	/* 16 */
/* 754 */	NdrFcShort( 0x0 ),	/* 0 */
/* 756 */	NdrFcShort( 0xa ),	/* Offset= 10 (766) */
/* 758 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 760 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 762 */	NdrFcShort( 0xffffffd8 ),	/* Offset= -40 (722) */
/* 764 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 766 */	
			0x12, 0x0,	/* FC_UP */
/* 768 */	NdrFcShort( 0xffffffe4 ),	/* Offset= -28 (740) */
/* 770 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 772 */	NdrFcShort( 0x4 ),	/* 4 */
/* 774 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 776 */	NdrFcShort( 0x0 ),	/* 0 */
/* 778 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 780 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 782 */	NdrFcShort( 0x4 ),	/* 4 */
/* 784 */	NdrFcShort( 0x0 ),	/* 0 */
/* 786 */	NdrFcShort( 0x1 ),	/* 1 */
/* 788 */	NdrFcShort( 0x0 ),	/* 0 */
/* 790 */	NdrFcShort( 0x0 ),	/* 0 */
/* 792 */	0x12, 0x0,	/* FC_UP */
/* 794 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (750) */
/* 796 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 798 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 800 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 802 */	NdrFcShort( 0x8 ),	/* 8 */
/* 804 */	NdrFcShort( 0x0 ),	/* 0 */
/* 806 */	NdrFcShort( 0x6 ),	/* Offset= 6 (812) */
/* 808 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 810 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 812 */	
			0x11, 0x0,	/* FC_RP */
/* 814 */	NdrFcShort( 0xffffffd4 ),	/* Offset= -44 (770) */
/* 816 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 818 */	NdrFcShort( 0x18 ),	/* 24 */
/* 820 */	NdrFcShort( 0x0 ),	/* 0 */
/* 822 */	NdrFcShort( 0xa ),	/* Offset= 10 (832) */
/* 824 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 826 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 828 */	NdrFcShort( 0xfffffd22 ),	/* Offset= -734 (94) */
/* 830 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 832 */	
			0x11, 0x0,	/* FC_RP */
/* 834 */	NdrFcShort( 0xffffff1e ),	/* Offset= -226 (608) */
/* 836 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 838 */	NdrFcShort( 0x1 ),	/* 1 */
/* 840 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 842 */	NdrFcShort( 0x0 ),	/* 0 */
/* 844 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 846 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 848 */	NdrFcShort( 0x8 ),	/* 8 */
/* 850 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 852 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 854 */	NdrFcShort( 0x4 ),	/* 4 */
/* 856 */	NdrFcShort( 0x4 ),	/* 4 */
/* 858 */	0x12, 0x0,	/* FC_UP */
/* 860 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (836) */
/* 862 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 864 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 866 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 868 */	NdrFcShort( 0x2 ),	/* 2 */
/* 870 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 872 */	NdrFcShort( 0x0 ),	/* 0 */
/* 874 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 876 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 878 */	NdrFcShort( 0x8 ),	/* 8 */
/* 880 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 882 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 884 */	NdrFcShort( 0x4 ),	/* 4 */
/* 886 */	NdrFcShort( 0x4 ),	/* 4 */
/* 888 */	0x12, 0x0,	/* FC_UP */
/* 890 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (866) */
/* 892 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 894 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 896 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 898 */	NdrFcShort( 0x4 ),	/* 4 */
/* 900 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 902 */	NdrFcShort( 0x0 ),	/* 0 */
/* 904 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 906 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 908 */	NdrFcShort( 0x8 ),	/* 8 */
/* 910 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 912 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 914 */	NdrFcShort( 0x4 ),	/* 4 */
/* 916 */	NdrFcShort( 0x4 ),	/* 4 */
/* 918 */	0x12, 0x0,	/* FC_UP */
/* 920 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (896) */
/* 922 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 924 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 926 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 928 */	NdrFcShort( 0x8 ),	/* 8 */
/* 930 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 932 */	NdrFcShort( 0x0 ),	/* 0 */
/* 934 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 936 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 938 */	NdrFcShort( 0x8 ),	/* 8 */
/* 940 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 942 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 944 */	NdrFcShort( 0x4 ),	/* 4 */
/* 946 */	NdrFcShort( 0x4 ),	/* 4 */
/* 948 */	0x12, 0x0,	/* FC_UP */
/* 950 */	NdrFcShort( 0xffffffe8 ),	/* Offset= -24 (926) */
/* 952 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 954 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 956 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 958 */	NdrFcShort( 0x8 ),	/* 8 */
/* 960 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 962 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 964 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 966 */	NdrFcShort( 0x8 ),	/* 8 */
/* 968 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 970 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 972 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 974 */	NdrFcShort( 0xffffffee ),	/* Offset= -18 (956) */
/* 976 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 978 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 980 */	NdrFcShort( 0x28 ),	/* 40 */
/* 982 */	NdrFcShort( 0xffffffee ),	/* Offset= -18 (964) */
/* 984 */	NdrFcShort( 0x0 ),	/* Offset= 0 (984) */
/* 986 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 988 */	0x38,		/* FC_ALIGNM4 */
			0x8,		/* FC_LONG */
/* 990 */	0x8,		/* FC_LONG */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 992 */	0x0,		/* 0 */
			NdrFcShort( 0xfffffe09 ),	/* Offset= -503 (490) */
			0x5b,		/* FC_END */
/* 996 */	
			0x12, 0x0,	/* FC_UP */
/* 998 */	NdrFcShort( 0xffffff08 ),	/* Offset= -248 (750) */
/* 1000 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1002 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 1004 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1006 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 1008 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1010 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 1012 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1014 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 1016 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1018 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 1020 */	
			0x12, 0x0,	/* FC_UP */
/* 1022 */	NdrFcShort( 0xfffffdd0 ),	/* Offset= -560 (462) */
/* 1024 */	
			0x12, 0x10,	/* FC_UP */
/* 1026 */	NdrFcShort( 0xfffffc00 ),	/* Offset= -1024 (2) */
/* 1028 */	
			0x12, 0x10,	/* FC_UP */
/* 1030 */	NdrFcShort( 0xfffffc68 ),	/* Offset= -920 (110) */
/* 1032 */	
			0x12, 0x10,	/* FC_UP */
/* 1034 */	NdrFcShort( 0xfffffdca ),	/* Offset= -566 (468) */
/* 1036 */	
			0x12, 0x10,	/* FC_UP */
/* 1038 */	NdrFcShort( 0xfffffdd8 ),	/* Offset= -552 (486) */
/* 1040 */	
			0x12, 0x10,	/* FC_UP */
/* 1042 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1044) */
/* 1044 */	
			0x12, 0x0,	/* FC_UP */
/* 1046 */	NdrFcShort( 0xfffffbea ),	/* Offset= -1046 (0) */
/* 1048 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 1050 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1052 */	0x6,		/* FC_SHORT */
			0x2,		/* FC_CHAR */
/* 1054 */	0x2,		/* FC_CHAR */
			0x38,		/* FC_ALIGNM4 */
/* 1056 */	0x8,		/* FC_LONG */
			0x39,		/* FC_ALIGNM8 */
/* 1058 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1060 */	
			0x12, 0x0,	/* FC_UP */
/* 1062 */	NdrFcShort( 0xfffffff2 ),	/* Offset= -14 (1048) */
/* 1064 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 1066 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1068 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1070 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1070) */
/* 1072 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1074 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1076 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1078 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1080 */	NdrFcShort( 0xfffffc86 ),	/* Offset= -890 (190) */
/* 1082 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1084 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1086 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1088 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1090 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1092 */	NdrFcShort( 0xfffffc76 ),	/* Offset= -906 (186) */
/* 1094 */	
			0x11, 0x10,	/* FC_RP */
/* 1096 */	NdrFcShort( 0xfffffc60 ),	/* Offset= -928 (168) */
/* 1098 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 1100 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1106) */
/* 1102 */	
			0x13, 0x0,	/* FC_OP */
/* 1104 */	NdrFcShort( 0xfffffbc0 ),	/* Offset= -1088 (16) */
/* 1106 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1108 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1110 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1112 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1114 */	NdrFcShort( 0xfffffff4 ),	/* Offset= -12 (1102) */

			0x0
        }
    };

const CInterfaceProxyVtbl * _vwclient_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IVWClientOCXProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IVWClientProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IVWClientSiteProxyVtbl,
    0
};

const CInterfaceStubVtbl * _vwclient_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IVWClientOCXStubVtbl,
    ( CInterfaceStubVtbl *) &_IVWClientStubVtbl,
    ( CInterfaceStubVtbl *) &_IVWClientSiteStubVtbl,
    0
};

PCInterfaceName const _vwclient_InterfaceNamesList[] = 
{
    "IVWClientOCX",
    "IVWClient",
    "IVWClientSite",
    0
};

const IID *  _vwclient_BaseIIDList[] = 
{
    &IID_IDispatch,
    &IID_IDispatch,
    0,
    0
};


#define _vwclient_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _vwclient, pIID, n)

int __stdcall _vwclient_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _vwclient, 3, 2 )
    IID_BS_LOOKUP_NEXT_TEST( _vwclient, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _vwclient, 3, *pIndex )
    
}

const ExtendedProxyFileInfo vwclient_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _vwclient_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _vwclient_StubVtblList,
    (const PCInterfaceName * ) & _vwclient_InterfaceNamesList,
    (const IID ** ) & _vwclient_BaseIIDList,
    & _vwclient_IID_Lookup, 
    3,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
