/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:26 2000
 */
/* Compiler settings for vwcomm.idl:
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


#include "vwcomm.h"

#define TYPE_FORMAT_STRING_SIZE   3                                 
#define PROC_FORMAT_STRING_SIZE   1                                 

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


/* Standard interface: __MIDL_itf_vwcomm_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IVWComm, ver. 0.0,
   GUID={0xc1918680,0xdbe4,0x11cf,{0xa5,0x16,0x00,0xaa,0x00,0xb5,0x95,0xdb}} */


/* Object interface: IVWCommSite, ver. 0.0,
   GUID={0xc1918682,0xdbe4,0x11cf,{0xa5,0x16,0x00,0xaa,0x00,0xb5,0x95,0xdb}} */


/* Object interface: IVWCommConnection, ver. 0.0,
   GUID={0xc1918686,0xdbe4,0x11cf,{0xa5,0x16,0x00,0xaa,0x00,0xb5,0x95,0xdb}} */


/* Standard interface: __MIDL_itf_vwcomm_0219, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IVWCommMessage, ver. 0.0,
   GUID={0xc1918688,0xdbe4,0x11cf,{0xa5,0x16,0x00,0xaa,0x00,0xb5,0x95,0xdb}} */

#pragma data_seg(".rdata")

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT40_OR_LATER)
#error You need a Windows NT 4.0 or later to run this stub because it uses these features:
#error   -Oif or -Oicf, float, double or hyper in -Oif or -Oicf, more than 32 methods in the interface.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */

			0x0
        }
    };

const CInterfaceProxyVtbl * _vwcomm_ProxyVtblList[] = 
{
    0
};

const CInterfaceStubVtbl * _vwcomm_StubVtblList[] = 
{
    0
};

PCInterfaceName const _vwcomm_InterfaceNamesList[] = 
{
    0
};


#define _vwcomm_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _vwcomm, pIID, n)

int __stdcall _vwcomm_IID_Lookup( const IID * pIID, int * pIndex )
{
    return 0;
}

const ExtendedProxyFileInfo vwcomm_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _vwcomm_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _vwcomm_StubVtblList,
    (const PCInterfaceName * ) & _vwcomm_InterfaceNamesList,
    0, // no delegation
    & _vwcomm_IID_Lookup, 
    0,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
