/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:36 2000
 */
/* Compiler settings for authntic.idl:
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


#include "authntic.h"

#define TYPE_FORMAT_STRING_SIZE   123                               
#define PROC_FORMAT_STRING_SIZE   749                               

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


/* Standard interface: __MIDL_itf_authntic_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IVWAuthenticate, ver. 0.0,
   GUID={0x955A1319,0x731F,0x11D1,{0x90,0x82,0x00,0xC0,0x4F,0xD9,0x1E,0xFE}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IVWAuthenticate_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short IVWAuthenticate_FormatStringOffsetTable[] = 
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    40,
    74,
    102,
    130,
    152,
    180,
    208,
    236,
    276
    };

static const MIDL_SERVER_INFO IVWAuthenticate_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IVWAuthenticate_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IVWAuthenticate_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IVWAuthenticate_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(17) _IVWAuthenticateProxyVtbl = 
{
    &IVWAuthenticate_ProxyInfo,
    &IID_IVWAuthenticate,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *)-1 /* IVWAuthenticate::GenerateClientContext */ ,
    (void *)-1 /* IVWAuthenticate::GenerateServerContext */ ,
    (void *)-1 /* IVWAuthenticate::get_Package */ ,
    (void *)-1 /* IVWAuthenticate::put_Package */ ,
    (void *)-1 /* IVWAuthenticate::Initialize */ ,
    (void *)-1 /* IVWAuthenticate::Terminate */ ,
    (void *)-1 /* IVWAuthenticate::Impersonate */ ,
    (void *)-1 /* IVWAuthenticate::RevertContext */ ,
    (void *)-1 /* IVWAuthenticate::IsGroupMember */ ,
    (void *)-1 /* IVWAuthenticate::GetUserInfo */
};


static const PRPC_STUB_FUNCTION IVWAuthenticate_table[] =
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
    NdrStubCall2
};

CInterfaceStubVtbl _IVWAuthenticateStubVtbl =
{
    &IID_IVWAuthenticate,
    &IVWAuthenticate_ServerInfo,
    17,
    &IVWAuthenticate_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IAuthentication, ver. 0.0,
   GUID={0x3D3CE78F,0x7323,0x11D1,{0x90,0x82,0x00,0xC0,0x4F,0xD9,0x1E,0xFE}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IAuthentication_ServerInfo;

#pragma code_seg(".orpc")
extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[1];

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

static const unsigned short IAuthentication_FormatStringOffsetTable[] = 
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    310,
    338,
    366,
    394,
    422,
    450,
    478,
    506,
    534,
    568,
    596,
    624,
    664,
    692,
    720
    };

static const MIDL_SERVER_INFO IAuthentication_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IAuthentication_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IAuthentication_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IAuthentication_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };

CINTERFACE_PROXY_VTABLE(22) _IAuthenticationProxyVtbl = 
{
    &IAuthentication_ProxyInfo,
    &IID_IAuthentication,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *)-1 /* IAuthentication::get_Initialized */ ,
    (void *)-1 /* IAuthentication::put_Initialized */ ,
    (void *)-1 /* IAuthentication::get_Authenticated */ ,
    (void *)-1 /* IAuthentication::put_Authenticated */ ,
    (void *)-1 /* IAuthentication::get_Credentials */ ,
    (void *)-1 /* IAuthentication::put_Credentials */ ,
    (void *)-1 /* IAuthentication::get_Context */ ,
    (void *)-1 /* IAuthentication::put_Context */ ,
    (void *)-1 /* IAuthentication::SetBlob */ ,
    (void *)-1 /* IAuthentication::GetBlobLength */ ,
    (void *)-1 /* IAuthentication::CopyBlob */ ,
    (void *)-1 /* IAuthentication::SetUserInfo */ ,
    (void *)-1 /* IAuthentication::get_AuthenticationData */ ,
    (void *)-1 /* IAuthentication::put_Authenticate */ ,
    (void *)-1 /* IAuthentication::get_AuthenticationInfo */
};


static const PRPC_STUB_FUNCTION IAuthentication_table[] =
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
    NdrStubCall2
};

CInterfaceStubVtbl _IAuthenticationStubVtbl =
{
    &IID_IAuthentication,
    &IAuthentication_ServerInfo,
    22,
    &IAuthentication_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

#pragma data_seg(".rdata")

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[1] = 
        {
            
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
#error   -Oif or -Oicf, [wire_marshal] or [user_marshal] attribute, more than 32 methods in the interface.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure GenerateClientContext */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
#ifndef _ALPHA_
/*  8 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 10 */	NdrFcShort( 0x6 ),	/* 6 */
/* 12 */	NdrFcShort( 0xe ),	/* 14 */
/* 14 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter pauth */

/* 16 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 18 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 20 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter bShowDialog */

/* 22 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 24 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 26 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter pbDone */

/* 28 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 30 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 32 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 34 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 36 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 38 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GenerateServerContext */

/* 40 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 42 */	NdrFcLong( 0x0 ),	/* 0 */
/* 46 */	NdrFcShort( 0x8 ),	/* 8 */
#ifndef _ALPHA_
/* 48 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 50 */	NdrFcShort( 0x0 ),	/* 0 */
/* 52 */	NdrFcShort( 0xe ),	/* 14 */
/* 54 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pauth */

/* 56 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 58 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 60 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter pbDone */

/* 62 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 64 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 66 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 68 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 70 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 72 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Package */

/* 74 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 76 */	NdrFcLong( 0x0 ),	/* 0 */
/* 80 */	NdrFcShort( 0x9 ),	/* 9 */
#ifndef _ALPHA_
/* 82 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 84 */	NdrFcShort( 0x0 ),	/* 0 */
/* 86 */	NdrFcShort( 0x8 ),	/* 8 */
/* 88 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter pbstrVal */

/* 90 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 92 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 94 */	NdrFcShort( 0x34 ),	/* Type Offset=52 */

	/* Return value */

/* 96 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 98 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 100 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Package */

/* 102 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 104 */	NdrFcLong( 0x0 ),	/* 0 */
/* 108 */	NdrFcShort( 0xa ),	/* 10 */
#ifndef _ALPHA_
/* 110 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 112 */	NdrFcShort( 0x0 ),	/* 0 */
/* 114 */	NdrFcShort( 0x8 ),	/* 8 */
/* 116 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter newVal */

/* 118 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 120 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 122 */	NdrFcShort( 0x42 ),	/* Type Offset=66 */

	/* Return value */

/* 124 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 126 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 128 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Initialize */

/* 130 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 132 */	NdrFcLong( 0x0 ),	/* 0 */
/* 136 */	NdrFcShort( 0xb ),	/* 11 */
#ifndef _ALPHA_
/* 138 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 140 */	NdrFcShort( 0x0 ),	/* 0 */
/* 142 */	NdrFcShort( 0x8 ),	/* 8 */
/* 144 */	0x4,		/* Oi2 Flags:  has return, */
			0x1,		/* 1 */

	/* Return value */

/* 146 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 148 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 150 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Terminate */

/* 152 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 154 */	NdrFcLong( 0x0 ),	/* 0 */
/* 158 */	NdrFcShort( 0xc ),	/* 12 */
#ifndef _ALPHA_
/* 160 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 162 */	NdrFcShort( 0x0 ),	/* 0 */
/* 164 */	NdrFcShort( 0x8 ),	/* 8 */
/* 166 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pauth */

/* 168 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 170 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 172 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 174 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 176 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 178 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Impersonate */

/* 180 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 182 */	NdrFcLong( 0x0 ),	/* 0 */
/* 186 */	NdrFcShort( 0xd ),	/* 13 */
#ifndef _ALPHA_
/* 188 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 190 */	NdrFcShort( 0x0 ),	/* 0 */
/* 192 */	NdrFcShort( 0x8 ),	/* 8 */
/* 194 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pauth */

/* 196 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 198 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 200 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 202 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 204 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 206 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RevertContext */

/* 208 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 210 */	NdrFcLong( 0x0 ),	/* 0 */
/* 214 */	NdrFcShort( 0xe ),	/* 14 */
#ifndef _ALPHA_
/* 216 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 218 */	NdrFcShort( 0x0 ),	/* 0 */
/* 220 */	NdrFcShort( 0x8 ),	/* 8 */
/* 222 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter pauth */

/* 224 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 226 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 228 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 230 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 232 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 234 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure IsGroupMember */

/* 236 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 238 */	NdrFcLong( 0x0 ),	/* 0 */
/* 242 */	NdrFcShort( 0xf ),	/* 15 */
#ifndef _ALPHA_
/* 244 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 246 */	NdrFcShort( 0x0 ),	/* 0 */
/* 248 */	NdrFcShort( 0xe ),	/* 14 */
/* 250 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter pauth */

/* 252 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 254 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 256 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter bstrGroup */

/* 258 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 260 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 262 */	NdrFcShort( 0x42 ),	/* Type Offset=66 */

	/* Parameter pbIsMember */

/* 264 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 266 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 268 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 270 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 272 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 274 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetUserInfo */

/* 276 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 278 */	NdrFcLong( 0x0 ),	/* 0 */
/* 282 */	NdrFcShort( 0x10 ),	/* 16 */
#ifndef _ALPHA_
/* 284 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 286 */	NdrFcShort( 0x0 ),	/* 0 */
/* 288 */	NdrFcShort( 0x8 ),	/* 8 */
/* 290 */	0x7,		/* Oi2 Flags:  srv must size, clt must size, has return, */
			0x3,		/* 3 */

	/* Parameter pauth */

/* 292 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 294 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 296 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Parameter pstrDomainAndName */

/* 298 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 300 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 302 */	NdrFcShort( 0x34 ),	/* Type Offset=52 */

	/* Return value */

/* 304 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 306 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 308 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Initialized */

/* 310 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 312 */	NdrFcLong( 0x0 ),	/* 0 */
/* 316 */	NdrFcShort( 0x7 ),	/* 7 */
#ifndef _ALPHA_
/* 318 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 320 */	NdrFcShort( 0x0 ),	/* 0 */
/* 322 */	NdrFcShort( 0xe ),	/* 14 */
/* 324 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter pVal */

/* 326 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 328 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 330 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 332 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 334 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 336 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Initialized */

/* 338 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 340 */	NdrFcLong( 0x0 ),	/* 0 */
/* 344 */	NdrFcShort( 0x8 ),	/* 8 */
#ifndef _ALPHA_
/* 346 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 348 */	NdrFcShort( 0x6 ),	/* 6 */
/* 350 */	NdrFcShort( 0x8 ),	/* 8 */
/* 352 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter newVal */

/* 354 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 356 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 358 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 360 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 362 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 364 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Authenticated */

/* 366 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 368 */	NdrFcLong( 0x0 ),	/* 0 */
/* 372 */	NdrFcShort( 0x9 ),	/* 9 */
#ifndef _ALPHA_
/* 374 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 376 */	NdrFcShort( 0x0 ),	/* 0 */
/* 378 */	NdrFcShort( 0xe ),	/* 14 */
/* 380 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter pVal */

/* 382 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 384 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 386 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 388 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 390 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 392 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Authenticated */

/* 394 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 396 */	NdrFcLong( 0x0 ),	/* 0 */
/* 400 */	NdrFcShort( 0xa ),	/* 10 */
#ifndef _ALPHA_
/* 402 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 404 */	NdrFcShort( 0x6 ),	/* 6 */
/* 406 */	NdrFcShort( 0x8 ),	/* 8 */
/* 408 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter newVal */

/* 410 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 412 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 414 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 416 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 418 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 420 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Credentials */

/* 422 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 424 */	NdrFcLong( 0x0 ),	/* 0 */
/* 428 */	NdrFcShort( 0xb ),	/* 11 */
#ifndef _ALPHA_
/* 430 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 432 */	NdrFcShort( 0x0 ),	/* 0 */
/* 434 */	NdrFcShort( 0x18 ),	/* 24 */
/* 436 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter pVal */

/* 438 */	NdrFcShort( 0x2112 ),	/* Flags:  must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 440 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 442 */	NdrFcShort( 0x50 ),	/* Type Offset=80 */

	/* Return value */

/* 444 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 446 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 448 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Credentials */

/* 450 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 452 */	NdrFcLong( 0x0 ),	/* 0 */
/* 456 */	NdrFcShort( 0xc ),	/* 12 */
#ifndef _ALPHA_
#if !defined(_MIPS_)
#ifndef _PPC_
/* 458 */	NdrFcShort( 0x10 ),	/* x86, MIPS Stack size/offset = 16 */
#else
			NdrFcShort( 0x14 ),	/* PPC Stack size/offset = 20 */
#endif
#else
			NdrFcShort( 0x10 ),	/* MIPS Stack size/offset = 16 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 460 */	NdrFcShort( 0x10 ),	/* 16 */
/* 462 */	NdrFcShort( 0x8 ),	/* 8 */
/* 464 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter newVal */

/* 466 */	NdrFcShort( 0x8a ),	/* Flags:  must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_)
#ifndef _PPC_
/* 468 */	NdrFcShort( 0x4 ),	/* x86, MIPS Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x4 ),	/* MIPS Stack size/offset = 4 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 470 */	NdrFcShort( 0x50 ),	/* Type Offset=80 */

	/* Return value */

/* 472 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_)
#ifndef _PPC_
/* 474 */	NdrFcShort( 0xc ),	/* x86, MIPS Stack size/offset = 12 */
#else
			NdrFcShort( 0x10 ),	/* PPC Stack size/offset = 16 */
#endif
#else
			NdrFcShort( 0xc ),	/* MIPS Stack size/offset = 12 */
#endif
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 476 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Context */

/* 478 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 480 */	NdrFcLong( 0x0 ),	/* 0 */
/* 484 */	NdrFcShort( 0xd ),	/* 13 */
#ifndef _ALPHA_
/* 486 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 488 */	NdrFcShort( 0x0 ),	/* 0 */
/* 490 */	NdrFcShort( 0x18 ),	/* 24 */
/* 492 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter pVal */

/* 494 */	NdrFcShort( 0x2112 ),	/* Flags:  must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 496 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 498 */	NdrFcShort( 0x50 ),	/* Type Offset=80 */

	/* Return value */

/* 500 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 502 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 504 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Context */

/* 506 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 508 */	NdrFcLong( 0x0 ),	/* 0 */
/* 512 */	NdrFcShort( 0xe ),	/* 14 */
#ifndef _ALPHA_
#if !defined(_MIPS_)
#ifndef _PPC_
/* 514 */	NdrFcShort( 0x10 ),	/* x86, MIPS Stack size/offset = 16 */
#else
			NdrFcShort( 0x14 ),	/* PPC Stack size/offset = 20 */
#endif
#else
			NdrFcShort( 0x10 ),	/* MIPS Stack size/offset = 16 */
#endif
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 516 */	NdrFcShort( 0x10 ),	/* 16 */
/* 518 */	NdrFcShort( 0x8 ),	/* 8 */
/* 520 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter newVal */

/* 522 */	NdrFcShort( 0x8a ),	/* Flags:  must free, in, by val, */
#ifndef _ALPHA_
#if !defined(_MIPS_)
#ifndef _PPC_
/* 524 */	NdrFcShort( 0x4 ),	/* x86, MIPS Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* PPC Stack size/offset = 8 */
#endif
#else
			NdrFcShort( 0x4 ),	/* MIPS Stack size/offset = 4 */
#endif
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 526 */	NdrFcShort( 0x50 ),	/* Type Offset=80 */

	/* Return value */

/* 528 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
#if !defined(_MIPS_)
#ifndef _PPC_
/* 530 */	NdrFcShort( 0xc ),	/* x86, MIPS Stack size/offset = 12 */
#else
			NdrFcShort( 0x10 ),	/* PPC Stack size/offset = 16 */
#endif
#else
			NdrFcShort( 0xc ),	/* MIPS Stack size/offset = 12 */
#endif
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 532 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetBlob */

/* 534 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 536 */	NdrFcLong( 0x0 ),	/* 0 */
/* 540 */	NdrFcShort( 0xf ),	/* 15 */
#ifndef _ALPHA_
/* 542 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 544 */	NdrFcShort( 0xd ),	/* 13 */
/* 546 */	NdrFcShort( 0x8 ),	/* 8 */
/* 548 */	0x4,		/* Oi2 Flags:  has return, */
			0x3,		/* 3 */

	/* Parameter pvBlob */

/* 550 */	NdrFcShort( 0x148 ),	/* Flags:  in, base type, simple ref, */
#ifndef _ALPHA_
/* 552 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 554 */	0x2,		/* FC_CHAR */
			0x0,		/* 0 */

	/* Parameter cbBlob */

/* 556 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
#ifndef _ALPHA_
/* 558 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 560 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 562 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 564 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 566 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetBlobLength */

/* 568 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 570 */	NdrFcLong( 0x0 ),	/* 0 */
/* 574 */	NdrFcShort( 0x10 ),	/* 16 */
#ifndef _ALPHA_
/* 576 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 578 */	NdrFcShort( 0x0 ),	/* 0 */
/* 580 */	NdrFcShort( 0x10 ),	/* 16 */
/* 582 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter pcbBlob */

/* 584 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 586 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 588 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 590 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 592 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 594 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CopyBlob */

/* 596 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 598 */	NdrFcLong( 0x0 ),	/* 0 */
/* 602 */	NdrFcShort( 0x11 ),	/* 17 */
#ifndef _ALPHA_
/* 604 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 606 */	NdrFcShort( 0x5 ),	/* 5 */
/* 608 */	NdrFcShort( 0x8 ),	/* 8 */
/* 610 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter pbDest */

/* 612 */	NdrFcShort( 0x148 ),	/* Flags:  in, base type, simple ref, */
#ifndef _ALPHA_
/* 614 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 616 */	0x2,		/* FC_CHAR */
			0x0,		/* 0 */

	/* Return value */

/* 618 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 620 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 622 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetUserInfo */

/* 624 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 626 */	NdrFcLong( 0x0 ),	/* 0 */
/* 630 */	NdrFcShort( 0x12 ),	/* 18 */
#ifndef _ALPHA_
/* 632 */	NdrFcShort( 0x14 ),	/* x86, MIPS, PPC Stack size/offset = 20 */
#else
			NdrFcShort( 0x28 ),	/* Alpha Stack size/offset = 40 */
#endif
/* 634 */	NdrFcShort( 0x0 ),	/* 0 */
/* 636 */	NdrFcShort( 0x8 ),	/* 8 */
/* 638 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x4,		/* 4 */

	/* Parameter bstrUser */

/* 640 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 642 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 644 */	NdrFcShort( 0x42 ),	/* Type Offset=66 */

	/* Parameter bstrPassword */

/* 646 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 648 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 650 */	NdrFcShort( 0x42 ),	/* Type Offset=66 */

	/* Parameter bstrDomain */

/* 652 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
/* 654 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 656 */	NdrFcShort( 0x42 ),	/* Type Offset=66 */

	/* Return value */

/* 658 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 660 */	NdrFcShort( 0x10 ),	/* x86, MIPS, PPC Stack size/offset = 16 */
#else
			NdrFcShort( 0x20 ),	/* Alpha Stack size/offset = 32 */
#endif
/* 662 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_AuthenticationData */

/* 664 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 666 */	NdrFcLong( 0x0 ),	/* 0 */
/* 670 */	NdrFcShort( 0x13 ),	/* 19 */
#ifndef _ALPHA_
/* 672 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 674 */	NdrFcShort( 0x0 ),	/* 0 */
/* 676 */	NdrFcShort( 0x15 ),	/* 21 */
/* 678 */	0x4,		/* Oi2 Flags:  has return, */
			0x2,		/* 2 */

	/* Parameter ppVal */

/* 680 */	NdrFcShort( 0x2012 ),	/* Flags:  must free, out, srv alloc size=8 */
#ifndef _ALPHA_
/* 682 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 684 */	NdrFcShort( 0x60 ),	/* Type Offset=96 */

	/* Return value */

/* 686 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 688 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 690 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Authenticate */

/* 692 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 694 */	NdrFcLong( 0x0 ),	/* 0 */
/* 698 */	NdrFcShort( 0x14 ),	/* 20 */
#ifndef _ALPHA_
/* 700 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 702 */	NdrFcShort( 0x0 ),	/* 0 */
/* 704 */	NdrFcShort( 0x8 ),	/* 8 */
/* 706 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter newVal */

/* 708 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
#ifndef _ALPHA_
/* 710 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 712 */	NdrFcShort( 0x68 ),	/* Type Offset=104 */

	/* Return value */

/* 714 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 716 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 718 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_AuthenticationInfo */

/* 720 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 722 */	NdrFcLong( 0x0 ),	/* 0 */
/* 726 */	NdrFcShort( 0x15 ),	/* 21 */
#ifndef _ALPHA_
/* 728 */	NdrFcShort( 0xc ),	/* x86, MIPS, PPC Stack size/offset = 12 */
#else
			NdrFcShort( 0x18 ),	/* Alpha Stack size/offset = 24 */
#endif
/* 730 */	NdrFcShort( 0x0 ),	/* 0 */
/* 732 */	NdrFcShort( 0x8 ),	/* 8 */
/* 734 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter pbstrVal */

/* 736 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
/* 738 */	NdrFcShort( 0x4 ),	/* x86, MIPS, PPC Stack size/offset = 4 */
#else
			NdrFcShort( 0x8 ),	/* Alpha Stack size/offset = 8 */
#endif
/* 740 */	NdrFcShort( 0x34 ),	/* Type Offset=52 */

	/* Return value */

/* 742 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
#ifndef _ALPHA_
/* 744 */	NdrFcShort( 0x8 ),	/* x86, MIPS, PPC Stack size/offset = 8 */
#else
			NdrFcShort( 0x10 ),	/* Alpha Stack size/offset = 16 */
#endif
/* 746 */	0x8,		/* FC_LONG */
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
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/*  4 */	NdrFcLong( 0x3d3ce78f ),	/* 1027401615 */
/*  8 */	NdrFcShort( 0x7323 ),	/* 29475 */
/* 10 */	NdrFcShort( 0x11d1 ),	/* 4561 */
/* 12 */	0x90,		/* 144 */
			0x82,		/* 130 */
/* 14 */	0x0,		/* 0 */
			0xc0,		/* 192 */
/* 16 */	0x4f,		/* 79 */
			0xd9,		/* 217 */
/* 18 */	0x1e,		/* 30 */
			0xfe,		/* 254 */
/* 20 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 22 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 24 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 26 */	NdrFcShort( 0x1a ),	/* Offset= 26 (52) */
/* 28 */	
			0x13, 0x0,	/* FC_OP */
/* 30 */	NdrFcShort( 0xc ),	/* Offset= 12 (42) */
/* 32 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 34 */	NdrFcShort( 0x2 ),	/* 2 */
/* 36 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 38 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 40 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 42 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 44 */	NdrFcShort( 0x8 ),	/* 8 */
/* 46 */	NdrFcShort( 0xfffffff2 ),	/* Offset= -14 (32) */
/* 48 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 50 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 52 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x4 ),	/* 4 */
/* 58 */	NdrFcShort( 0x0 ),	/* 0 */
/* 60 */	NdrFcShort( 0xffffffe0 ),	/* Offset= -32 (28) */
/* 62 */	
			0x12, 0x0,	/* FC_UP */
/* 64 */	NdrFcShort( 0xffffffea ),	/* Offset= -22 (42) */
/* 66 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 68 */	NdrFcShort( 0x0 ),	/* 0 */
/* 70 */	NdrFcShort( 0x4 ),	/* 4 */
/* 72 */	NdrFcShort( 0x0 ),	/* 0 */
/* 74 */	NdrFcShort( 0xfffffff4 ),	/* Offset= -12 (62) */
/* 76 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 78 */	NdrFcShort( 0x2 ),	/* Offset= 2 (80) */
/* 80 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 82 */	NdrFcShort( 0x8 ),	/* 8 */
/* 84 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 86 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 88 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/* 90 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 92 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 94 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 96 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] */
/* 98 */	NdrFcShort( 0x2 ),	/* Offset= 2 (100) */
/* 100 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 102 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 104 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 106 */	NdrFcLong( 0x955a1319 ),	/* -1789258983 */
/* 110 */	NdrFcShort( 0x731f ),	/* 29471 */
/* 112 */	NdrFcShort( 0x11d1 ),	/* 4561 */
/* 114 */	0x90,		/* 144 */
			0x82,		/* 130 */
/* 116 */	0x0,		/* 0 */
			0xc0,		/* 192 */
/* 118 */	0x4f,		/* 79 */
			0xd9,		/* 217 */
/* 120 */	0x1e,		/* 30 */
			0xfe,		/* 254 */

			0x0
        }
    };

const CInterfaceProxyVtbl * _authntic_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IVWAuthenticateProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IAuthenticationProxyVtbl,
    0
};

const CInterfaceStubVtbl * _authntic_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IVWAuthenticateStubVtbl,
    ( CInterfaceStubVtbl *) &_IAuthenticationStubVtbl,
    0
};

PCInterfaceName const _authntic_InterfaceNamesList[] = 
{
    "IVWAuthenticate",
    "IAuthentication",
    0
};

const IID *  _authntic_BaseIIDList[] = 
{
    &IID_IDispatch,
    &IID_IDispatch,
    0
};


#define _authntic_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _authntic, pIID, n)

int __stdcall _authntic_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _authntic, 2, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _authntic, 2, *pIndex )
    
}

const ExtendedProxyFileInfo authntic_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _authntic_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _authntic_StubVtblList,
    (const PCInterfaceName * ) & _authntic_InterfaceNamesList,
    (const IID ** ) & _authntic_BaseIIDList,
    & _authntic_IID_Lookup, 
    2,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
