/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:26 2000
 */
/* Compiler settings for vwcomm.idl:
    Oicf (OptLev=i2), W0, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __vwcomm_h__
#define __vwcomm_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IVWComm_FWD_DEFINED__
#define __IVWComm_FWD_DEFINED__
typedef interface IVWComm IVWComm;
#endif 	/* __IVWComm_FWD_DEFINED__ */


#ifndef __IVWCommSite_FWD_DEFINED__
#define __IVWCommSite_FWD_DEFINED__
typedef interface IVWCommSite IVWCommSite;
#endif 	/* __IVWCommSite_FWD_DEFINED__ */


#ifndef __IVWCommConnection_FWD_DEFINED__
#define __IVWCommConnection_FWD_DEFINED__
typedef interface IVWCommConnection IVWCommConnection;
#endif 	/* __IVWCommConnection_FWD_DEFINED__ */


#ifndef __IVWCommMessage_FWD_DEFINED__
#define __IVWCommMessage_FWD_DEFINED__
typedef interface IVWCommMessage IVWCommMessage;
#endif 	/* __IVWCommMessage_FWD_DEFINED__ */


/* header files for imported files */
#include "vwbuffer.h"
#include "authntic.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_vwcomm_0000 */
/* [local] */ 

#define VWCOMM_E_COULDNTCONNECT			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0501)
#define VWCOMM_E_COULDNTACCEPT			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0502)
#define VWCOMM_E_NOTINITIALIZED			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0503)
#define VWCOMM_E_INVALIDCONFIGURATION	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0504)
#define VWCOMM_E_UNKNOWNERROR			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0505)
#define VWCOMM_E_INVALIDPARAMETER		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0506)
#define VWCOMM_E_INTERNALTIMEOUT			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0507)
#define VWCOMM_E_ERRORDURINGSYNCHWRITE	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0508)
#define VWCOMM_E_NOMESSAGES				MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0509)
#define VWCOMM_E_CONNECTIONNOTFOUND		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x050A)
#define VWCOMM_E_INVALIDSOCKET			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x050B)
#define VWCOMM_E_IMPROPERSTATE			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x050C)
#define VWCOMM_E_WINSOCKERROR			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x050D)
#define VWCOMM_E_COULDNTINITIALIZE		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x050E)
#define VWCOMM_E_ALREADYINWRITESYNCH		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x050F)





extern RPC_IF_HANDLE __MIDL_itf_vwcomm_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_vwcomm_0000_v0_0_s_ifspec;

#ifndef __IVWComm_INTERFACE_DEFINED__
#define __IVWComm_INTERFACE_DEFINED__

/* interface IVWComm */
/* [unique][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IVWComm;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("c1918680-dbe4-11cf-a516-00aa00b595db")
    IVWComm : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ IVWCommSite __RPC_FAR *pICommSite) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Terminate( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Connect( 
            /* [in] */ LPSTR psz,
            /* [in] */ int iPort,
            /* [in] */ VARIANT_BOOL bTunnel,
            /* [out] */ IVWCommConnection __RPC_FAR *__RPC_FAR *ppICommConnection) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DisconnectAll( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Accept( 
            /* [in] */ int iPort) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetMessage( 
            /* [out] */ IVWCommMessage __RPC_FAR *__RPC_FAR *pICommMessage) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_HostName( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE KeepAlive( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetUsageStatistics( 
            /* [out] */ int __RPC_FAR *pcPacketsSent,
            /* [out] */ int __RPC_FAR *pcPacketsReceived,
            /* [out] */ int __RPC_FAR *pcBytesSent,
            /* [out] */ int __RPC_FAR *pcBytesReceived,
            /* [out] */ int __RPC_FAR *pcPacketsReceivedPerSecond,
            /* [out] */ int __RPC_FAR *pcBytesReceivedPerSecond,
            /* [out] */ int __RPC_FAR *pcPacketsSentPerSecond,
            /* [out] */ int __RPC_FAR *pcBytesSentPerSecond,
            /* [out] */ int __RPC_FAR *pcMeanPacketsInWriteQueue,
            /* [out] */ int __RPC_FAR *pcMeanMessagesInReadQueue,
            /* [out] */ int __RPC_FAR *pcMeanTimeInWriteQueue,
            /* [out] */ int __RPC_FAR *pcMeanTimeInReadQueue) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVWCommVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IVWComm __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IVWComm __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IVWComm __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            IVWComm __RPC_FAR * This,
            /* [in] */ IVWCommSite __RPC_FAR *pICommSite);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IVWComm __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Connect )( 
            IVWComm __RPC_FAR * This,
            /* [in] */ LPSTR psz,
            /* [in] */ int iPort,
            /* [in] */ VARIANT_BOOL bTunnel,
            /* [out] */ IVWCommConnection __RPC_FAR *__RPC_FAR *ppICommConnection);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DisconnectAll )( 
            IVWComm __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Accept )( 
            IVWComm __RPC_FAR * This,
            /* [in] */ int iPort);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMessage )( 
            IVWComm __RPC_FAR * This,
            /* [out] */ IVWCommMessage __RPC_FAR *__RPC_FAR *pICommMessage);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_HostName )( 
            IVWComm __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *KeepAlive )( 
            IVWComm __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetUsageStatistics )( 
            IVWComm __RPC_FAR * This,
            /* [out] */ int __RPC_FAR *pcPacketsSent,
            /* [out] */ int __RPC_FAR *pcPacketsReceived,
            /* [out] */ int __RPC_FAR *pcBytesSent,
            /* [out] */ int __RPC_FAR *pcBytesReceived,
            /* [out] */ int __RPC_FAR *pcPacketsReceivedPerSecond,
            /* [out] */ int __RPC_FAR *pcBytesReceivedPerSecond,
            /* [out] */ int __RPC_FAR *pcPacketsSentPerSecond,
            /* [out] */ int __RPC_FAR *pcBytesSentPerSecond,
            /* [out] */ int __RPC_FAR *pcMeanPacketsInWriteQueue,
            /* [out] */ int __RPC_FAR *pcMeanMessagesInReadQueue,
            /* [out] */ int __RPC_FAR *pcMeanTimeInWriteQueue,
            /* [out] */ int __RPC_FAR *pcMeanTimeInReadQueue);
        
        END_INTERFACE
    } IVWCommVtbl;

    interface IVWComm
    {
        CONST_VTBL struct IVWCommVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVWComm_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IVWComm_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IVWComm_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IVWComm_Initialize(This,pICommSite)	\
    (This)->lpVtbl -> Initialize(This,pICommSite)

#define IVWComm_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IVWComm_Connect(This,psz,iPort,bTunnel,ppICommConnection)	\
    (This)->lpVtbl -> Connect(This,psz,iPort,bTunnel,ppICommConnection)

#define IVWComm_DisconnectAll(This)	\
    (This)->lpVtbl -> DisconnectAll(This)

#define IVWComm_Accept(This,iPort)	\
    (This)->lpVtbl -> Accept(This,iPort)

#define IVWComm_GetMessage(This,pICommMessage)	\
    (This)->lpVtbl -> GetMessage(This,pICommMessage)

#define IVWComm_get_HostName(This,pbstrVal)	\
    (This)->lpVtbl -> get_HostName(This,pbstrVal)

#define IVWComm_KeepAlive(This)	\
    (This)->lpVtbl -> KeepAlive(This)

#define IVWComm_GetUsageStatistics(This,pcPacketsSent,pcPacketsReceived,pcBytesSent,pcBytesReceived,pcPacketsReceivedPerSecond,pcBytesReceivedPerSecond,pcPacketsSentPerSecond,pcBytesSentPerSecond,pcMeanPacketsInWriteQueue,pcMeanMessagesInReadQueue,pcMeanTimeInWriteQueue,pcMeanTimeInReadQueue)	\
    (This)->lpVtbl -> GetUsageStatistics(This,pcPacketsSent,pcPacketsReceived,pcBytesSent,pcBytesReceived,pcPacketsReceivedPerSecond,pcBytesReceivedPerSecond,pcPacketsSentPerSecond,pcBytesSentPerSecond,pcMeanPacketsInWriteQueue,pcMeanMessagesInReadQueue,pcMeanTimeInWriteQueue,pcMeanTimeInReadQueue)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWComm_Initialize_Proxy( 
    IVWComm __RPC_FAR * This,
    /* [in] */ IVWCommSite __RPC_FAR *pICommSite);


void __RPC_STUB IVWComm_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWComm_Terminate_Proxy( 
    IVWComm __RPC_FAR * This);


void __RPC_STUB IVWComm_Terminate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWComm_Connect_Proxy( 
    IVWComm __RPC_FAR * This,
    /* [in] */ LPSTR psz,
    /* [in] */ int iPort,
    /* [in] */ VARIANT_BOOL bTunnel,
    /* [out] */ IVWCommConnection __RPC_FAR *__RPC_FAR *ppICommConnection);


void __RPC_STUB IVWComm_Connect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWComm_DisconnectAll_Proxy( 
    IVWComm __RPC_FAR * This);


void __RPC_STUB IVWComm_DisconnectAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWComm_Accept_Proxy( 
    IVWComm __RPC_FAR * This,
    /* [in] */ int iPort);


void __RPC_STUB IVWComm_Accept_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWComm_GetMessage_Proxy( 
    IVWComm __RPC_FAR * This,
    /* [out] */ IVWCommMessage __RPC_FAR *__RPC_FAR *pICommMessage);


void __RPC_STUB IVWComm_GetMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWComm_get_HostName_Proxy( 
    IVWComm __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrVal);


void __RPC_STUB IVWComm_get_HostName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWComm_KeepAlive_Proxy( 
    IVWComm __RPC_FAR * This);


void __RPC_STUB IVWComm_KeepAlive_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWComm_GetUsageStatistics_Proxy( 
    IVWComm __RPC_FAR * This,
    /* [out] */ int __RPC_FAR *pcPacketsSent,
    /* [out] */ int __RPC_FAR *pcPacketsReceived,
    /* [out] */ int __RPC_FAR *pcBytesSent,
    /* [out] */ int __RPC_FAR *pcBytesReceived,
    /* [out] */ int __RPC_FAR *pcPacketsReceivedPerSecond,
    /* [out] */ int __RPC_FAR *pcBytesReceivedPerSecond,
    /* [out] */ int __RPC_FAR *pcPacketsSentPerSecond,
    /* [out] */ int __RPC_FAR *pcBytesSentPerSecond,
    /* [out] */ int __RPC_FAR *pcMeanPacketsInWriteQueue,
    /* [out] */ int __RPC_FAR *pcMeanMessagesInReadQueue,
    /* [out] */ int __RPC_FAR *pcMeanTimeInWriteQueue,
    /* [out] */ int __RPC_FAR *pcMeanTimeInReadQueue);


void __RPC_STUB IVWComm_GetUsageStatistics_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IVWComm_INTERFACE_DEFINED__ */


#ifndef __IVWCommSite_INTERFACE_DEFINED__
#define __IVWCommSite_INTERFACE_DEFINED__

/* interface IVWCommSite */
/* [unique][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IVWCommSite;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("c1918682-dbe4-11cf-a516-00aa00b595db")
    IVWCommSite : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnMessageAvailable( 
            IVWComm __RPC_FAR *__MIDL_0015) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnMessageAvailable_T2( 
            IVWComm __RPC_FAR *__MIDL_0016) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVWCommSiteVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IVWCommSite __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IVWCommSite __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IVWCommSite __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnMessageAvailable )( 
            IVWCommSite __RPC_FAR * This,
            IVWComm __RPC_FAR *__MIDL_0015);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnMessageAvailable_T2 )( 
            IVWCommSite __RPC_FAR * This,
            IVWComm __RPC_FAR *__MIDL_0016);
        
        END_INTERFACE
    } IVWCommSiteVtbl;

    interface IVWCommSite
    {
        CONST_VTBL struct IVWCommSiteVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVWCommSite_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IVWCommSite_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IVWCommSite_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IVWCommSite_OnMessageAvailable(This,__MIDL_0015)	\
    (This)->lpVtbl -> OnMessageAvailable(This,__MIDL_0015)

#define IVWCommSite_OnMessageAvailable_T2(This,__MIDL_0016)	\
    (This)->lpVtbl -> OnMessageAvailable_T2(This,__MIDL_0016)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWCommSite_OnMessageAvailable_Proxy( 
    IVWCommSite __RPC_FAR * This,
    IVWComm __RPC_FAR *__MIDL_0015);


void __RPC_STUB IVWCommSite_OnMessageAvailable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWCommSite_OnMessageAvailable_T2_Proxy( 
    IVWCommSite __RPC_FAR * This,
    IVWComm __RPC_FAR *__MIDL_0016);


void __RPC_STUB IVWCommSite_OnMessageAvailable_T2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IVWCommSite_INTERFACE_DEFINED__ */


#ifndef __IVWCommConnection_INTERFACE_DEFINED__
#define __IVWCommConnection_INTERFACE_DEFINED__

/* interface IVWCommConnection */
/* [unique][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IVWCommConnection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("c1918686-dbe4-11cf-a516-00aa00b595db")
    IVWCommConnection : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SendAsync( 
            /* [in] */ IMarshallBuffer __RPC_FAR *pcb) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE SendAsyncExt( 
            /* [in] */ IMarshallBuffer __RPC_FAR *pcb,
            /* [in] */ int cConnections,
            /* [size_is][in] */ IVWCommConnection __RPC_FAR *__RPC_FAR rgpconn[  ]) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SendSync( 
            /* [in] */ IMarshallBuffer __RPC_FAR *pcb,
            /* [out] */ IUnMarshallBuffer __RPC_FAR *__RPC_FAR *ppcbReply) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Disconnect( void) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_SocketAddress( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrVal) = 0;
        
        virtual /* [helpstring][restricted][propput] */ HRESULT STDMETHODCALLTYPE put_Authentication( 
            /* [in] */ IAuthentication __RPC_FAR *pauth) = 0;
        
        virtual /* [helpstring][restricted][propget] */ HRESULT STDMETHODCALLTYPE get_Authentication( 
            /* [retval][out] */ IAuthentication __RPC_FAR *__RPC_FAR *ppauth) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_IsAuthenticated( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAuthenticated) = 0;
        
        virtual /* [helpstring][restricted][propput] */ HRESULT STDMETHODCALLTYPE put_UserData( 
            /* [in] */ void __RPC_FAR *pvData) = 0;
        
        virtual /* [helpstring][restricted][propget] */ HRESULT STDMETHODCALLTYPE get_UserData( 
            /* [retval][out] */ void __RPC_FAR *__RPC_FAR *ppvData) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVWCommConnectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IVWCommConnection __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IVWCommConnection __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IVWCommConnection __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendAsync )( 
            IVWCommConnection __RPC_FAR * This,
            /* [in] */ IMarshallBuffer __RPC_FAR *pcb);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendAsyncExt )( 
            IVWCommConnection __RPC_FAR * This,
            /* [in] */ IMarshallBuffer __RPC_FAR *pcb,
            /* [in] */ int cConnections,
            /* [size_is][in] */ IVWCommConnection __RPC_FAR *__RPC_FAR rgpconn[  ]);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendSync )( 
            IVWCommConnection __RPC_FAR * This,
            /* [in] */ IMarshallBuffer __RPC_FAR *pcb,
            /* [out] */ IUnMarshallBuffer __RPC_FAR *__RPC_FAR *ppcbReply);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Disconnect )( 
            IVWCommConnection __RPC_FAR * This);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SocketAddress )( 
            IVWCommConnection __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrVal);
        
        /* [helpstring][restricted][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Authentication )( 
            IVWCommConnection __RPC_FAR * This,
            /* [in] */ IAuthentication __RPC_FAR *pauth);
        
        /* [helpstring][restricted][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Authentication )( 
            IVWCommConnection __RPC_FAR * This,
            /* [retval][out] */ IAuthentication __RPC_FAR *__RPC_FAR *ppauth);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsAuthenticated )( 
            IVWCommConnection __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAuthenticated);
        
        /* [helpstring][restricted][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_UserData )( 
            IVWCommConnection __RPC_FAR * This,
            /* [in] */ void __RPC_FAR *pvData);
        
        /* [helpstring][restricted][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_UserData )( 
            IVWCommConnection __RPC_FAR * This,
            /* [retval][out] */ void __RPC_FAR *__RPC_FAR *ppvData);
        
        END_INTERFACE
    } IVWCommConnectionVtbl;

    interface IVWCommConnection
    {
        CONST_VTBL struct IVWCommConnectionVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVWCommConnection_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IVWCommConnection_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IVWCommConnection_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IVWCommConnection_SendAsync(This,pcb)	\
    (This)->lpVtbl -> SendAsync(This,pcb)

#define IVWCommConnection_SendAsyncExt(This,pcb,cConnections,rgpconn)	\
    (This)->lpVtbl -> SendAsyncExt(This,pcb,cConnections,rgpconn)

#define IVWCommConnection_SendSync(This,pcb,ppcbReply)	\
    (This)->lpVtbl -> SendSync(This,pcb,ppcbReply)

#define IVWCommConnection_Disconnect(This)	\
    (This)->lpVtbl -> Disconnect(This)

#define IVWCommConnection_get_SocketAddress(This,pbstrVal)	\
    (This)->lpVtbl -> get_SocketAddress(This,pbstrVal)

#define IVWCommConnection_put_Authentication(This,pauth)	\
    (This)->lpVtbl -> put_Authentication(This,pauth)

#define IVWCommConnection_get_Authentication(This,ppauth)	\
    (This)->lpVtbl -> get_Authentication(This,ppauth)

#define IVWCommConnection_get_IsAuthenticated(This,pbAuthenticated)	\
    (This)->lpVtbl -> get_IsAuthenticated(This,pbAuthenticated)

#define IVWCommConnection_put_UserData(This,pvData)	\
    (This)->lpVtbl -> put_UserData(This,pvData)

#define IVWCommConnection_get_UserData(This,ppvData)	\
    (This)->lpVtbl -> get_UserData(This,ppvData)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWCommConnection_SendAsync_Proxy( 
    IVWCommConnection __RPC_FAR * This,
    /* [in] */ IMarshallBuffer __RPC_FAR *pcb);


void __RPC_STUB IVWCommConnection_SendAsync_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IVWCommConnection_SendAsyncExt_Proxy( 
    IVWCommConnection __RPC_FAR * This,
    /* [in] */ IMarshallBuffer __RPC_FAR *pcb,
    /* [in] */ int cConnections,
    /* [size_is][in] */ IVWCommConnection __RPC_FAR *__RPC_FAR rgpconn[  ]);


void __RPC_STUB IVWCommConnection_SendAsyncExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWCommConnection_SendSync_Proxy( 
    IVWCommConnection __RPC_FAR * This,
    /* [in] */ IMarshallBuffer __RPC_FAR *pcb,
    /* [out] */ IUnMarshallBuffer __RPC_FAR *__RPC_FAR *ppcbReply);


void __RPC_STUB IVWCommConnection_SendSync_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWCommConnection_Disconnect_Proxy( 
    IVWCommConnection __RPC_FAR * This);


void __RPC_STUB IVWCommConnection_Disconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWCommConnection_get_SocketAddress_Proxy( 
    IVWCommConnection __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrVal);


void __RPC_STUB IVWCommConnection_get_SocketAddress_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted][propput] */ HRESULT STDMETHODCALLTYPE IVWCommConnection_put_Authentication_Proxy( 
    IVWCommConnection __RPC_FAR * This,
    /* [in] */ IAuthentication __RPC_FAR *pauth);


void __RPC_STUB IVWCommConnection_put_Authentication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted][propget] */ HRESULT STDMETHODCALLTYPE IVWCommConnection_get_Authentication_Proxy( 
    IVWCommConnection __RPC_FAR * This,
    /* [retval][out] */ IAuthentication __RPC_FAR *__RPC_FAR *ppauth);


void __RPC_STUB IVWCommConnection_get_Authentication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWCommConnection_get_IsAuthenticated_Proxy( 
    IVWCommConnection __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbAuthenticated);


void __RPC_STUB IVWCommConnection_get_IsAuthenticated_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted][propput] */ HRESULT STDMETHODCALLTYPE IVWCommConnection_put_UserData_Proxy( 
    IVWCommConnection __RPC_FAR * This,
    /* [in] */ void __RPC_FAR *pvData);


void __RPC_STUB IVWCommConnection_put_UserData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted][propget] */ HRESULT STDMETHODCALLTYPE IVWCommConnection_get_UserData_Proxy( 
    IVWCommConnection __RPC_FAR * This,
    /* [retval][out] */ void __RPC_FAR *__RPC_FAR *ppvData);


void __RPC_STUB IVWCommConnection_get_UserData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IVWCommConnection_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_vwcomm_0219 */
/* [local] */ 

typedef /* [helpstring][uuid] */ 
enum MsgStatusConstant
    {	mscOk	= 0,
	mscReadError	= 1,
	mscWriteError	= 2,
	mscAcceptError	= 3,
	mscConnect	= 4,
	mscDisconnect	= 5,
	mscDirtyDisconnect	= 6,
	mscOutOfMemory	= 7,
	mscEvilUser	= 8
    }	MsgStatusConstant;



extern RPC_IF_HANDLE __MIDL_itf_vwcomm_0219_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_vwcomm_0219_v0_0_s_ifspec;

#ifndef __IVWCommMessage_INTERFACE_DEFINED__
#define __IVWCommMessage_INTERFACE_DEFINED__

/* interface IVWCommMessage */
/* [unique][local][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IVWCommMessage;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("c1918688-dbe4-11cf-a516-00aa00b595db")
    IVWCommMessage : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetStatus( 
            /* [out] */ int __RPC_FAR *pnStatus) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetConnection( 
            /* [out] */ IVWCommConnection __RPC_FAR *__RPC_FAR *ppIVWCommConnection) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetData( 
            /* [out] */ IUnMarshallBuffer __RPC_FAR *__RPC_FAR *ppbuffer) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetSynchronous( 
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbSync) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Reply( 
            /* [in] */ IMarshallBuffer __RPC_FAR *pbuffer) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVWCommMessageVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IVWCommMessage __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IVWCommMessage __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IVWCommMessage __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetStatus )( 
            IVWCommMessage __RPC_FAR * This,
            /* [out] */ int __RPC_FAR *pnStatus);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetConnection )( 
            IVWCommMessage __RPC_FAR * This,
            /* [out] */ IVWCommConnection __RPC_FAR *__RPC_FAR *ppIVWCommConnection);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetData )( 
            IVWCommMessage __RPC_FAR * This,
            /* [out] */ IUnMarshallBuffer __RPC_FAR *__RPC_FAR *ppbuffer);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSynchronous )( 
            IVWCommMessage __RPC_FAR * This,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbSync);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Reply )( 
            IVWCommMessage __RPC_FAR * This,
            /* [in] */ IMarshallBuffer __RPC_FAR *pbuffer);
        
        END_INTERFACE
    } IVWCommMessageVtbl;

    interface IVWCommMessage
    {
        CONST_VTBL struct IVWCommMessageVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVWCommMessage_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IVWCommMessage_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IVWCommMessage_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IVWCommMessage_GetStatus(This,pnStatus)	\
    (This)->lpVtbl -> GetStatus(This,pnStatus)

#define IVWCommMessage_GetConnection(This,ppIVWCommConnection)	\
    (This)->lpVtbl -> GetConnection(This,ppIVWCommConnection)

#define IVWCommMessage_GetData(This,ppbuffer)	\
    (This)->lpVtbl -> GetData(This,ppbuffer)

#define IVWCommMessage_GetSynchronous(This,pbSync)	\
    (This)->lpVtbl -> GetSynchronous(This,pbSync)

#define IVWCommMessage_Reply(This,pbuffer)	\
    (This)->lpVtbl -> Reply(This,pbuffer)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWCommMessage_GetStatus_Proxy( 
    IVWCommMessage __RPC_FAR * This,
    /* [out] */ int __RPC_FAR *pnStatus);


void __RPC_STUB IVWCommMessage_GetStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWCommMessage_GetConnection_Proxy( 
    IVWCommMessage __RPC_FAR * This,
    /* [out] */ IVWCommConnection __RPC_FAR *__RPC_FAR *ppIVWCommConnection);


void __RPC_STUB IVWCommMessage_GetConnection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWCommMessage_GetData_Proxy( 
    IVWCommMessage __RPC_FAR * This,
    /* [out] */ IUnMarshallBuffer __RPC_FAR *__RPC_FAR *ppbuffer);


void __RPC_STUB IVWCommMessage_GetData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWCommMessage_GetSynchronous_Proxy( 
    IVWCommMessage __RPC_FAR * This,
    /* [out] */ VARIANT_BOOL __RPC_FAR *pbSync);


void __RPC_STUB IVWCommMessage_GetSynchronous_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWCommMessage_Reply_Proxy( 
    IVWCommMessage __RPC_FAR * This,
    /* [in] */ IMarshallBuffer __RPC_FAR *pbuffer);


void __RPC_STUB IVWCommMessage_Reply_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IVWCommMessage_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
