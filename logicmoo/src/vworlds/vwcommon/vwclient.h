/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:47 2000
 */
/* Compiler settings for vwclient.idl:
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

#ifndef __vwclient_h__
#define __vwclient_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IVWClient_FWD_DEFINED__
#define __IVWClient_FWD_DEFINED__
typedef interface IVWClient IVWClient;
#endif 	/* __IVWClient_FWD_DEFINED__ */


#ifndef __IVWClientSite_FWD_DEFINED__
#define __IVWClientSite_FWD_DEFINED__
typedef interface IVWClientSite IVWClientSite;
#endif 	/* __IVWClientSite_FWD_DEFINED__ */


#ifndef __IVWClientOCX_FWD_DEFINED__
#define __IVWClientOCX_FWD_DEFINED__
typedef interface IVWClientOCX IVWClientOCX;
#endif 	/* __IVWClientOCX_FWD_DEFINED__ */


/* header files for imported files */
#include "vwobject.h"
#include "vwadmin.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_vwclient_0000 */
/* [local] */ 

#define VWCLIENT_E_ALREADYCONNECTED			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0301)
#define VWCLIENT_E_WINDOWCREATIONFAILURE		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0302)
#define VWCLIENT_E_POSTMESSAGEFAILURE		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0303)
#define VWCLIENT_E_INVALIDREPLYFROMSERVER	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0304)
#define VWCLIENT_E_INVALIDDATABASENAME		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0305)
#define VWCLIENT_E_CONNECTFAILED				MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0306)




extern RPC_IF_HANDLE __MIDL_itf_vwclient_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_vwclient_0000_v0_0_s_ifspec;

#ifndef __IVWClient_INTERFACE_DEFINED__
#define __IVWClient_INTERFACE_DEFINED__

/* interface IVWClient */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IVWClient;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("590D9820-A417-11CF-9BF0-0080C7A56A8A")
    IVWClient : public IDispatch
    {
    public:
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE Initialize( void) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE Terminate( void) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ConnectLocalEx( 
            /* [in] */ BSTR bstr,
            /* [in] */ VARIANT_BOOL bOverwrite,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ConnectLocal( 
            /* [in] */ BSTR bstr,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Connect( 
            /* [in] */ BSTR bstrURL,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ConnectToServer( 
            /* [in] */ BSTR bstrURL,
            /* [in] */ VARIANT_BOOL bAuthenticated,
            /* [retval][out] */ IVWAdmin __RPC_FAR *__RPC_FAR *ppadmin) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateWorld( 
            /* [in] */ BSTR bstrWorldName,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OpenWorld( 
            /* [in] */ BSTR bstrWorldName,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Disconnect( void) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_IsConnected( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_World( 
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Trace( 
            /* [in] */ BSTR bstr) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Report( 
            /* [in] */ BSTR bstr,
            /* [in] */ long lType) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateTool( 
            /* [in] */ BSTR name,
            /* [in] */ REFCLSID clsid,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateToolFromProgID( 
            /* [in] */ BSTR name,
            /* [in] */ BSTR progid,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RemoveTool( 
            /* [in] */ BSTR name) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Tool( 
            /* [in] */ BSTR name,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Tools( 
            /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropmap) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVWClientVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IVWClient __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IVWClient __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IVWClient __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IVWClient __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IVWClient __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IVWClient __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IVWClient __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            IVWClient __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IVWClient __RPC_FAR * This);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ConnectLocalEx )( 
            IVWClient __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ VARIANT_BOOL bOverwrite,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ConnectLocal )( 
            IVWClient __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Connect )( 
            IVWClient __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ConnectToServer )( 
            IVWClient __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in] */ VARIANT_BOOL bAuthenticated,
            /* [retval][out] */ IVWAdmin __RPC_FAR *__RPC_FAR *ppadmin);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateWorld )( 
            IVWClient __RPC_FAR * This,
            /* [in] */ BSTR bstrWorldName,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OpenWorld )( 
            IVWClient __RPC_FAR * This,
            /* [in] */ BSTR bstrWorldName,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Disconnect )( 
            IVWClient __RPC_FAR * This);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsConnected )( 
            IVWClient __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_World )( 
            IVWClient __RPC_FAR * This,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Trace )( 
            IVWClient __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Report )( 
            IVWClient __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ long lType);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateTool )( 
            IVWClient __RPC_FAR * This,
            /* [in] */ BSTR name,
            /* [in] */ REFCLSID clsid,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateToolFromProgID )( 
            IVWClient __RPC_FAR * This,
            /* [in] */ BSTR name,
            /* [in] */ BSTR progid,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveTool )( 
            IVWClient __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Tool )( 
            IVWClient __RPC_FAR * This,
            /* [in] */ BSTR name,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Tools )( 
            IVWClient __RPC_FAR * This,
            /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropmap);
        
        END_INTERFACE
    } IVWClientVtbl;

    interface IVWClient
    {
        CONST_VTBL struct IVWClientVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVWClient_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IVWClient_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IVWClient_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IVWClient_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IVWClient_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IVWClient_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IVWClient_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IVWClient_Initialize(This)	\
    (This)->lpVtbl -> Initialize(This)

#define IVWClient_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IVWClient_ConnectLocalEx(This,bstr,bOverwrite,ppworld)	\
    (This)->lpVtbl -> ConnectLocalEx(This,bstr,bOverwrite,ppworld)

#define IVWClient_ConnectLocal(This,bstr,ppworld)	\
    (This)->lpVtbl -> ConnectLocal(This,bstr,ppworld)

#define IVWClient_Connect(This,bstrURL,ppworld)	\
    (This)->lpVtbl -> Connect(This,bstrURL,ppworld)

#define IVWClient_ConnectToServer(This,bstrURL,bAuthenticated,ppadmin)	\
    (This)->lpVtbl -> ConnectToServer(This,bstrURL,bAuthenticated,ppadmin)

#define IVWClient_CreateWorld(This,bstrWorldName,ppworld)	\
    (This)->lpVtbl -> CreateWorld(This,bstrWorldName,ppworld)

#define IVWClient_OpenWorld(This,bstrWorldName,ppworld)	\
    (This)->lpVtbl -> OpenWorld(This,bstrWorldName,ppworld)

#define IVWClient_Disconnect(This)	\
    (This)->lpVtbl -> Disconnect(This)

#define IVWClient_get_IsConnected(This,pbool)	\
    (This)->lpVtbl -> get_IsConnected(This,pbool)

#define IVWClient_get_World(This,ppworld)	\
    (This)->lpVtbl -> get_World(This,ppworld)

#define IVWClient_Trace(This,bstr)	\
    (This)->lpVtbl -> Trace(This,bstr)

#define IVWClient_Report(This,bstr,lType)	\
    (This)->lpVtbl -> Report(This,bstr,lType)

#define IVWClient_CreateTool(This,name,clsid,ppunk)	\
    (This)->lpVtbl -> CreateTool(This,name,clsid,ppunk)

#define IVWClient_CreateToolFromProgID(This,name,progid,ppunk)	\
    (This)->lpVtbl -> CreateToolFromProgID(This,name,progid,ppunk)

#define IVWClient_RemoveTool(This,name)	\
    (This)->lpVtbl -> RemoveTool(This,name)

#define IVWClient_get_Tool(This,name,ppunk)	\
    (This)->lpVtbl -> get_Tool(This,name,ppunk)

#define IVWClient_get_Tools(This,pppropmap)	\
    (This)->lpVtbl -> get_Tools(This,pppropmap)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IVWClient_Initialize_Proxy( 
    IVWClient __RPC_FAR * This);


void __RPC_STUB IVWClient_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IVWClient_Terminate_Proxy( 
    IVWClient __RPC_FAR * This);


void __RPC_STUB IVWClient_Terminate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IVWClient_ConnectLocalEx_Proxy( 
    IVWClient __RPC_FAR * This,
    /* [in] */ BSTR bstr,
    /* [in] */ VARIANT_BOOL bOverwrite,
    /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);


void __RPC_STUB IVWClient_ConnectLocalEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWClient_ConnectLocal_Proxy( 
    IVWClient __RPC_FAR * This,
    /* [in] */ BSTR bstr,
    /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);


void __RPC_STUB IVWClient_ConnectLocal_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWClient_Connect_Proxy( 
    IVWClient __RPC_FAR * This,
    /* [in] */ BSTR bstrURL,
    /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);


void __RPC_STUB IVWClient_Connect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWClient_ConnectToServer_Proxy( 
    IVWClient __RPC_FAR * This,
    /* [in] */ BSTR bstrURL,
    /* [in] */ VARIANT_BOOL bAuthenticated,
    /* [retval][out] */ IVWAdmin __RPC_FAR *__RPC_FAR *ppadmin);


void __RPC_STUB IVWClient_ConnectToServer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWClient_CreateWorld_Proxy( 
    IVWClient __RPC_FAR * This,
    /* [in] */ BSTR bstrWorldName,
    /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);


void __RPC_STUB IVWClient_CreateWorld_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWClient_OpenWorld_Proxy( 
    IVWClient __RPC_FAR * This,
    /* [in] */ BSTR bstrWorldName,
    /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);


void __RPC_STUB IVWClient_OpenWorld_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWClient_Disconnect_Proxy( 
    IVWClient __RPC_FAR * This);


void __RPC_STUB IVWClient_Disconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWClient_get_IsConnected_Proxy( 
    IVWClient __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IVWClient_get_IsConnected_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWClient_get_World_Proxy( 
    IVWClient __RPC_FAR * This,
    /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);


void __RPC_STUB IVWClient_get_World_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWClient_Trace_Proxy( 
    IVWClient __RPC_FAR * This,
    /* [in] */ BSTR bstr);


void __RPC_STUB IVWClient_Trace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWClient_Report_Proxy( 
    IVWClient __RPC_FAR * This,
    /* [in] */ BSTR bstr,
    /* [in] */ long lType);


void __RPC_STUB IVWClient_Report_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWClient_CreateTool_Proxy( 
    IVWClient __RPC_FAR * This,
    /* [in] */ BSTR name,
    /* [in] */ REFCLSID clsid,
    /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);


void __RPC_STUB IVWClient_CreateTool_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWClient_CreateToolFromProgID_Proxy( 
    IVWClient __RPC_FAR * This,
    /* [in] */ BSTR name,
    /* [in] */ BSTR progid,
    /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);


void __RPC_STUB IVWClient_CreateToolFromProgID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWClient_RemoveTool_Proxy( 
    IVWClient __RPC_FAR * This,
    /* [in] */ BSTR name);


void __RPC_STUB IVWClient_RemoveTool_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWClient_get_Tool_Proxy( 
    IVWClient __RPC_FAR * This,
    /* [in] */ BSTR name,
    /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);


void __RPC_STUB IVWClient_get_Tool_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IVWClient_get_Tools_Proxy( 
    IVWClient __RPC_FAR * This,
    /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropmap);


void __RPC_STUB IVWClient_get_Tools_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IVWClient_INTERFACE_DEFINED__ */


#ifndef __IVWClientSite_INTERFACE_DEFINED__
#define __IVWClientSite_INTERFACE_DEFINED__

/* interface IVWClientSite */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IVWClientSite;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("590D9822-A417-11CF-9BF0-0080C7A56A8A")
    IVWClientSite : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnConnect( 
            /* [in] */ IWorld __RPC_FAR *pWorld) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnDisconnect( 
            /* [in] */ IWorld __RPC_FAR *pWorld) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnUserConnect( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnUserDisconnect( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnUserReconnect( 
            /* [in] */ IVWClient __RPC_FAR *pNewClient,
            /* [in] */ IWorld __RPC_FAR *pNewWorld,
            /* [in] */ IThing __RPC_FAR *pNewThing) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnTrace( 
            /* [in] */ BSTR bstr) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnReport( 
            /* [in] */ BSTR bstr,
            /* [in] */ long lVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnUIEvent( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrEventName,
            /* [in] */ VARIANT varArg) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVWClientSiteVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IVWClientSite __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IVWClientSite __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IVWClientSite __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnConnect )( 
            IVWClientSite __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pWorld);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnDisconnect )( 
            IVWClientSite __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pWorld);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUserConnect )( 
            IVWClientSite __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUserDisconnect )( 
            IVWClientSite __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUserReconnect )( 
            IVWClientSite __RPC_FAR * This,
            /* [in] */ IVWClient __RPC_FAR *pNewClient,
            /* [in] */ IWorld __RPC_FAR *pNewWorld,
            /* [in] */ IThing __RPC_FAR *pNewThing);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnTrace )( 
            IVWClientSite __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnReport )( 
            IVWClientSite __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ long lVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUIEvent )( 
            IVWClientSite __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrEventName,
            /* [in] */ VARIANT varArg);
        
        END_INTERFACE
    } IVWClientSiteVtbl;

    interface IVWClientSite
    {
        CONST_VTBL struct IVWClientSiteVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVWClientSite_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IVWClientSite_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IVWClientSite_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IVWClientSite_OnConnect(This,pWorld)	\
    (This)->lpVtbl -> OnConnect(This,pWorld)

#define IVWClientSite_OnDisconnect(This,pWorld)	\
    (This)->lpVtbl -> OnDisconnect(This,pWorld)

#define IVWClientSite_OnUserConnect(This,pthing)	\
    (This)->lpVtbl -> OnUserConnect(This,pthing)

#define IVWClientSite_OnUserDisconnect(This,pthing)	\
    (This)->lpVtbl -> OnUserDisconnect(This,pthing)

#define IVWClientSite_OnUserReconnect(This,pNewClient,pNewWorld,pNewThing)	\
    (This)->lpVtbl -> OnUserReconnect(This,pNewClient,pNewWorld,pNewThing)

#define IVWClientSite_OnTrace(This,bstr)	\
    (This)->lpVtbl -> OnTrace(This,bstr)

#define IVWClientSite_OnReport(This,bstr,lVal)	\
    (This)->lpVtbl -> OnReport(This,bstr,lVal)

#define IVWClientSite_OnUIEvent(This,pthing,bstrEventName,varArg)	\
    (This)->lpVtbl -> OnUIEvent(This,pthing,bstrEventName,varArg)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWClientSite_OnConnect_Proxy( 
    IVWClientSite __RPC_FAR * This,
    /* [in] */ IWorld __RPC_FAR *pWorld);


void __RPC_STUB IVWClientSite_OnConnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWClientSite_OnDisconnect_Proxy( 
    IVWClientSite __RPC_FAR * This,
    /* [in] */ IWorld __RPC_FAR *pWorld);


void __RPC_STUB IVWClientSite_OnDisconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWClientSite_OnUserConnect_Proxy( 
    IVWClientSite __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IVWClientSite_OnUserConnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWClientSite_OnUserDisconnect_Proxy( 
    IVWClientSite __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IVWClientSite_OnUserDisconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWClientSite_OnUserReconnect_Proxy( 
    IVWClientSite __RPC_FAR * This,
    /* [in] */ IVWClient __RPC_FAR *pNewClient,
    /* [in] */ IWorld __RPC_FAR *pNewWorld,
    /* [in] */ IThing __RPC_FAR *pNewThing);


void __RPC_STUB IVWClientSite_OnUserReconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWClientSite_OnTrace_Proxy( 
    IVWClientSite __RPC_FAR * This,
    /* [in] */ BSTR bstr);


void __RPC_STUB IVWClientSite_OnTrace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWClientSite_OnReport_Proxy( 
    IVWClientSite __RPC_FAR * This,
    /* [in] */ BSTR bstr,
    /* [in] */ long lVal);


void __RPC_STUB IVWClientSite_OnReport_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWClientSite_OnUIEvent_Proxy( 
    IVWClientSite __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrEventName,
    /* [in] */ VARIANT varArg);


void __RPC_STUB IVWClientSite_OnUIEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IVWClientSite_INTERFACE_DEFINED__ */


#ifndef __IVWClientOCX_INTERFACE_DEFINED__
#define __IVWClientOCX_INTERFACE_DEFINED__

/* interface IVWClientOCX */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IVWClientOCX;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D4898801-20EF-11D0-8A96-00A0C9082583")
    IVWClientOCX : public IDispatch
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_VWClient( 
            /* [retval][out] */ IVWClient __RPC_FAR *__RPC_FAR *pVWClient) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Version( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrVersion) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVWClientOCXVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IVWClientOCX __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IVWClientOCX __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IVWClientOCX __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IVWClientOCX __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IVWClientOCX __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IVWClientOCX __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IVWClientOCX __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_VWClient )( 
            IVWClientOCX __RPC_FAR * This,
            /* [retval][out] */ IVWClient __RPC_FAR *__RPC_FAR *pVWClient);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Version )( 
            IVWClientOCX __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrVersion);
        
        END_INTERFACE
    } IVWClientOCXVtbl;

    interface IVWClientOCX
    {
        CONST_VTBL struct IVWClientOCXVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVWClientOCX_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IVWClientOCX_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IVWClientOCX_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IVWClientOCX_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IVWClientOCX_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IVWClientOCX_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IVWClientOCX_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IVWClientOCX_get_VWClient(This,pVWClient)	\
    (This)->lpVtbl -> get_VWClient(This,pVWClient)

#define IVWClientOCX_get_Version(This,pbstrVersion)	\
    (This)->lpVtbl -> get_Version(This,pbstrVersion)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWClientOCX_get_VWClient_Proxy( 
    IVWClientOCX __RPC_FAR * This,
    /* [retval][out] */ IVWClient __RPC_FAR *__RPC_FAR *pVWClient);


void __RPC_STUB IVWClientOCX_get_VWClient_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWClientOCX_get_Version_Proxy( 
    IVWClientOCX __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrVersion);


void __RPC_STUB IVWClientOCX_get_Version_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IVWClientOCX_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
