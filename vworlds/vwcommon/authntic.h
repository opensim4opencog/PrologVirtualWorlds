/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:36 2000
 */
/* Compiler settings for authntic.idl:
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

#ifndef __authntic_h__
#define __authntic_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IVWAuthenticate_FWD_DEFINED__
#define __IVWAuthenticate_FWD_DEFINED__
typedef interface IVWAuthenticate IVWAuthenticate;
#endif 	/* __IVWAuthenticate_FWD_DEFINED__ */


#ifndef __IAuthentication_FWD_DEFINED__
#define __IAuthentication_FWD_DEFINED__
typedef interface IAuthentication IAuthentication;
#endif 	/* __IAuthentication_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_authntic_0000 */
/* [local] */ 


#define SECURITY_WIN32
#include "sspi.h"
#include "issperr.h"
#ifndef SECHANDLEDEFINED
#define SECHANDLEDEFINED
typedef struct  _SECHANDLE
    {
    unsigned long dwLower;
    unsigned long dwUpper;
    }	SECHANDLE;

#endif


extern RPC_IF_HANDLE __MIDL_itf_authntic_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_authntic_0000_v0_0_s_ifspec;

#ifndef __IVWAuthenticate_INTERFACE_DEFINED__
#define __IVWAuthenticate_INTERFACE_DEFINED__

/* interface IVWAuthenticate */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IVWAuthenticate;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("955A1319-731F-11D1-9082-00C04FD91EFE")
    IVWAuthenticate : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GenerateClientContext( 
            /* [in] */ IAuthentication __RPC_FAR *pauth,
            /* [in] */ VARIANT_BOOL bShowDialog,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbDone) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GenerateServerContext( 
            /* [in] */ IAuthentication __RPC_FAR *pauth,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbDone) = 0;
        
        virtual /* [restricted][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Package( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrVal) = 0;
        
        virtual /* [restricted][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Package( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Initialize( void) = 0;
        
        virtual /* [restricted][helpstring][id] */ HRESULT STDMETHODCALLTYPE Terminate( 
            /* [in] */ IAuthentication __RPC_FAR *pauth) = 0;
        
        virtual /* [restricted][helpstring][id] */ HRESULT STDMETHODCALLTYPE Impersonate( 
            /* [in] */ IAuthentication __RPC_FAR *pauth) = 0;
        
        virtual /* [restricted][helpstring][id] */ HRESULT STDMETHODCALLTYPE RevertContext( 
            /* [in] */ IAuthentication __RPC_FAR *pauth) = 0;
        
        virtual /* [restricted][helpstring][id] */ HRESULT STDMETHODCALLTYPE IsGroupMember( 
            /* [in] */ IAuthentication __RPC_FAR *pauth,
            /* [in] */ BSTR bstrGroup,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbIsMember) = 0;
        
        virtual /* [restricted][helpstring][id] */ HRESULT STDMETHODCALLTYPE GetUserInfo( 
            /* [in] */ IAuthentication __RPC_FAR *pauth,
            /* [out] */ BSTR __RPC_FAR *pstrDomainAndName) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVWAuthenticateVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IVWAuthenticate __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IVWAuthenticate __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IVWAuthenticate __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IVWAuthenticate __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IVWAuthenticate __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IVWAuthenticate __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IVWAuthenticate __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GenerateClientContext )( 
            IVWAuthenticate __RPC_FAR * This,
            /* [in] */ IAuthentication __RPC_FAR *pauth,
            /* [in] */ VARIANT_BOOL bShowDialog,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbDone);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GenerateServerContext )( 
            IVWAuthenticate __RPC_FAR * This,
            /* [in] */ IAuthentication __RPC_FAR *pauth,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbDone);
        
        /* [restricted][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Package )( 
            IVWAuthenticate __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrVal);
        
        /* [restricted][helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Package )( 
            IVWAuthenticate __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            IVWAuthenticate __RPC_FAR * This);
        
        /* [restricted][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IVWAuthenticate __RPC_FAR * This,
            /* [in] */ IAuthentication __RPC_FAR *pauth);
        
        /* [restricted][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Impersonate )( 
            IVWAuthenticate __RPC_FAR * This,
            /* [in] */ IAuthentication __RPC_FAR *pauth);
        
        /* [restricted][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RevertContext )( 
            IVWAuthenticate __RPC_FAR * This,
            /* [in] */ IAuthentication __RPC_FAR *pauth);
        
        /* [restricted][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsGroupMember )( 
            IVWAuthenticate __RPC_FAR * This,
            /* [in] */ IAuthentication __RPC_FAR *pauth,
            /* [in] */ BSTR bstrGroup,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbIsMember);
        
        /* [restricted][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetUserInfo )( 
            IVWAuthenticate __RPC_FAR * This,
            /* [in] */ IAuthentication __RPC_FAR *pauth,
            /* [out] */ BSTR __RPC_FAR *pstrDomainAndName);
        
        END_INTERFACE
    } IVWAuthenticateVtbl;

    interface IVWAuthenticate
    {
        CONST_VTBL struct IVWAuthenticateVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVWAuthenticate_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IVWAuthenticate_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IVWAuthenticate_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IVWAuthenticate_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IVWAuthenticate_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IVWAuthenticate_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IVWAuthenticate_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IVWAuthenticate_GenerateClientContext(This,pauth,bShowDialog,pbDone)	\
    (This)->lpVtbl -> GenerateClientContext(This,pauth,bShowDialog,pbDone)

#define IVWAuthenticate_GenerateServerContext(This,pauth,pbDone)	\
    (This)->lpVtbl -> GenerateServerContext(This,pauth,pbDone)

#define IVWAuthenticate_get_Package(This,pbstrVal)	\
    (This)->lpVtbl -> get_Package(This,pbstrVal)

#define IVWAuthenticate_put_Package(This,newVal)	\
    (This)->lpVtbl -> put_Package(This,newVal)

#define IVWAuthenticate_Initialize(This)	\
    (This)->lpVtbl -> Initialize(This)

#define IVWAuthenticate_Terminate(This,pauth)	\
    (This)->lpVtbl -> Terminate(This,pauth)

#define IVWAuthenticate_Impersonate(This,pauth)	\
    (This)->lpVtbl -> Impersonate(This,pauth)

#define IVWAuthenticate_RevertContext(This,pauth)	\
    (This)->lpVtbl -> RevertContext(This,pauth)

#define IVWAuthenticate_IsGroupMember(This,pauth,bstrGroup,pbIsMember)	\
    (This)->lpVtbl -> IsGroupMember(This,pauth,bstrGroup,pbIsMember)

#define IVWAuthenticate_GetUserInfo(This,pauth,pstrDomainAndName)	\
    (This)->lpVtbl -> GetUserInfo(This,pauth,pstrDomainAndName)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVWAuthenticate_GenerateClientContext_Proxy( 
    IVWAuthenticate __RPC_FAR * This,
    /* [in] */ IAuthentication __RPC_FAR *pauth,
    /* [in] */ VARIANT_BOOL bShowDialog,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbDone);


void __RPC_STUB IVWAuthenticate_GenerateClientContext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVWAuthenticate_GenerateServerContext_Proxy( 
    IVWAuthenticate __RPC_FAR * This,
    /* [in] */ IAuthentication __RPC_FAR *pauth,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbDone);


void __RPC_STUB IVWAuthenticate_GenerateServerContext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVWAuthenticate_get_Package_Proxy( 
    IVWAuthenticate __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrVal);


void __RPC_STUB IVWAuthenticate_get_Package_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IVWAuthenticate_put_Package_Proxy( 
    IVWAuthenticate __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IVWAuthenticate_put_Package_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IVWAuthenticate_Initialize_Proxy( 
    IVWAuthenticate __RPC_FAR * This);


void __RPC_STUB IVWAuthenticate_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][helpstring][id] */ HRESULT STDMETHODCALLTYPE IVWAuthenticate_Terminate_Proxy( 
    IVWAuthenticate __RPC_FAR * This,
    /* [in] */ IAuthentication __RPC_FAR *pauth);


void __RPC_STUB IVWAuthenticate_Terminate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][helpstring][id] */ HRESULT STDMETHODCALLTYPE IVWAuthenticate_Impersonate_Proxy( 
    IVWAuthenticate __RPC_FAR * This,
    /* [in] */ IAuthentication __RPC_FAR *pauth);


void __RPC_STUB IVWAuthenticate_Impersonate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][helpstring][id] */ HRESULT STDMETHODCALLTYPE IVWAuthenticate_RevertContext_Proxy( 
    IVWAuthenticate __RPC_FAR * This,
    /* [in] */ IAuthentication __RPC_FAR *pauth);


void __RPC_STUB IVWAuthenticate_RevertContext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][helpstring][id] */ HRESULT STDMETHODCALLTYPE IVWAuthenticate_IsGroupMember_Proxy( 
    IVWAuthenticate __RPC_FAR * This,
    /* [in] */ IAuthentication __RPC_FAR *pauth,
    /* [in] */ BSTR bstrGroup,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbIsMember);


void __RPC_STUB IVWAuthenticate_IsGroupMember_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][helpstring][id] */ HRESULT STDMETHODCALLTYPE IVWAuthenticate_GetUserInfo_Proxy( 
    IVWAuthenticate __RPC_FAR * This,
    /* [in] */ IAuthentication __RPC_FAR *pauth,
    /* [out] */ BSTR __RPC_FAR *pstrDomainAndName);


void __RPC_STUB IVWAuthenticate_GetUserInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IVWAuthenticate_INTERFACE_DEFINED__ */


#ifndef __IAuthentication_INTERFACE_DEFINED__
#define __IAuthentication_INTERFACE_DEFINED__

/* interface IAuthentication */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IAuthentication;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3D3CE78F-7323-11D1-9082-00C04FD91EFE")
    IAuthentication : public IDispatch
    {
    public:
        virtual /* [restricted][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Initialized( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [restricted][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Initialized( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Authenticated( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [restricted][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Authenticated( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [restricted][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Credentials( 
            /* [retval][out] */ SECHANDLE __RPC_FAR *pVal) = 0;
        
        virtual /* [restricted][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Credentials( 
            /* [in] */ SECHANDLE newVal) = 0;
        
        virtual /* [restricted][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Context( 
            /* [retval][out] */ SECHANDLE __RPC_FAR *pVal) = 0;
        
        virtual /* [restricted][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Context( 
            /* [in] */ SECHANDLE newVal) = 0;
        
        virtual /* [restricted][helpstring][id] */ HRESULT STDMETHODCALLTYPE SetBlob( 
            /* [in] */ BYTE __RPC_FAR *pvBlob,
            /* [in] */ int cbBlob) = 0;
        
        virtual /* [restricted][helpstring][id] */ HRESULT STDMETHODCALLTYPE GetBlobLength( 
            /* [out] */ int __RPC_FAR *pcbBlob) = 0;
        
        virtual /* [restricted][helpstring][id] */ HRESULT STDMETHODCALLTYPE CopyBlob( 
            /* [in] */ BYTE __RPC_FAR *pbDest) = 0;
        
        virtual /* [restricted][helpstring][id] */ HRESULT STDMETHODCALLTYPE SetUserInfo( 
            /* [in] */ BSTR bstrUser,
            /* [in] */ BSTR bstrPassword,
            /* [in] */ BSTR bstrDomain) = 0;
        
        virtual /* [restricted][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AuthenticationData( 
            /* [retval][out] */ BYTE __RPC_FAR *__RPC_FAR *ppVal) = 0;
        
        virtual /* [restricted][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Authenticate( 
            /* [in] */ IVWAuthenticate __RPC_FAR *newVal) = 0;
        
        virtual /* [restricted][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AuthenticationInfo( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAuthenticationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAuthentication __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAuthentication __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAuthentication __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IAuthentication __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IAuthentication __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IAuthentication __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IAuthentication __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Initialized )( 
            IAuthentication __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [restricted][helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Initialized )( 
            IAuthentication __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Authenticated )( 
            IAuthentication __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);
        
        /* [restricted][helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Authenticated )( 
            IAuthentication __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [restricted][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Credentials )( 
            IAuthentication __RPC_FAR * This,
            /* [retval][out] */ SECHANDLE __RPC_FAR *pVal);
        
        /* [restricted][helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Credentials )( 
            IAuthentication __RPC_FAR * This,
            /* [in] */ SECHANDLE newVal);
        
        /* [restricted][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Context )( 
            IAuthentication __RPC_FAR * This,
            /* [retval][out] */ SECHANDLE __RPC_FAR *pVal);
        
        /* [restricted][helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Context )( 
            IAuthentication __RPC_FAR * This,
            /* [in] */ SECHANDLE newVal);
        
        /* [restricted][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetBlob )( 
            IAuthentication __RPC_FAR * This,
            /* [in] */ BYTE __RPC_FAR *pvBlob,
            /* [in] */ int cbBlob);
        
        /* [restricted][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetBlobLength )( 
            IAuthentication __RPC_FAR * This,
            /* [out] */ int __RPC_FAR *pcbBlob);
        
        /* [restricted][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CopyBlob )( 
            IAuthentication __RPC_FAR * This,
            /* [in] */ BYTE __RPC_FAR *pbDest);
        
        /* [restricted][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetUserInfo )( 
            IAuthentication __RPC_FAR * This,
            /* [in] */ BSTR bstrUser,
            /* [in] */ BSTR bstrPassword,
            /* [in] */ BSTR bstrDomain);
        
        /* [restricted][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AuthenticationData )( 
            IAuthentication __RPC_FAR * This,
            /* [retval][out] */ BYTE __RPC_FAR *__RPC_FAR *ppVal);
        
        /* [restricted][helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Authenticate )( 
            IAuthentication __RPC_FAR * This,
            /* [in] */ IVWAuthenticate __RPC_FAR *newVal);
        
        /* [restricted][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AuthenticationInfo )( 
            IAuthentication __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrVal);
        
        END_INTERFACE
    } IAuthenticationVtbl;

    interface IAuthentication
    {
        CONST_VTBL struct IAuthenticationVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAuthentication_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAuthentication_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAuthentication_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAuthentication_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAuthentication_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAuthentication_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAuthentication_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IAuthentication_get_Initialized(This,pVal)	\
    (This)->lpVtbl -> get_Initialized(This,pVal)

#define IAuthentication_put_Initialized(This,newVal)	\
    (This)->lpVtbl -> put_Initialized(This,newVal)

#define IAuthentication_get_Authenticated(This,pVal)	\
    (This)->lpVtbl -> get_Authenticated(This,pVal)

#define IAuthentication_put_Authenticated(This,newVal)	\
    (This)->lpVtbl -> put_Authenticated(This,newVal)

#define IAuthentication_get_Credentials(This,pVal)	\
    (This)->lpVtbl -> get_Credentials(This,pVal)

#define IAuthentication_put_Credentials(This,newVal)	\
    (This)->lpVtbl -> put_Credentials(This,newVal)

#define IAuthentication_get_Context(This,pVal)	\
    (This)->lpVtbl -> get_Context(This,pVal)

#define IAuthentication_put_Context(This,newVal)	\
    (This)->lpVtbl -> put_Context(This,newVal)

#define IAuthentication_SetBlob(This,pvBlob,cbBlob)	\
    (This)->lpVtbl -> SetBlob(This,pvBlob,cbBlob)

#define IAuthentication_GetBlobLength(This,pcbBlob)	\
    (This)->lpVtbl -> GetBlobLength(This,pcbBlob)

#define IAuthentication_CopyBlob(This,pbDest)	\
    (This)->lpVtbl -> CopyBlob(This,pbDest)

#define IAuthentication_SetUserInfo(This,bstrUser,bstrPassword,bstrDomain)	\
    (This)->lpVtbl -> SetUserInfo(This,bstrUser,bstrPassword,bstrDomain)

#define IAuthentication_get_AuthenticationData(This,ppVal)	\
    (This)->lpVtbl -> get_AuthenticationData(This,ppVal)

#define IAuthentication_put_Authenticate(This,newVal)	\
    (This)->lpVtbl -> put_Authenticate(This,newVal)

#define IAuthentication_get_AuthenticationInfo(This,pbstrVal)	\
    (This)->lpVtbl -> get_AuthenticationInfo(This,pbstrVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [restricted][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAuthentication_get_Initialized_Proxy( 
    IAuthentication __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB IAuthentication_get_Initialized_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IAuthentication_put_Initialized_Proxy( 
    IAuthentication __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IAuthentication_put_Initialized_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAuthentication_get_Authenticated_Proxy( 
    IAuthentication __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal);


void __RPC_STUB IAuthentication_get_Authenticated_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IAuthentication_put_Authenticated_Proxy( 
    IAuthentication __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IAuthentication_put_Authenticated_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAuthentication_get_Credentials_Proxy( 
    IAuthentication __RPC_FAR * This,
    /* [retval][out] */ SECHANDLE __RPC_FAR *pVal);


void __RPC_STUB IAuthentication_get_Credentials_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IAuthentication_put_Credentials_Proxy( 
    IAuthentication __RPC_FAR * This,
    /* [in] */ SECHANDLE newVal);


void __RPC_STUB IAuthentication_put_Credentials_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAuthentication_get_Context_Proxy( 
    IAuthentication __RPC_FAR * This,
    /* [retval][out] */ SECHANDLE __RPC_FAR *pVal);


void __RPC_STUB IAuthentication_get_Context_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IAuthentication_put_Context_Proxy( 
    IAuthentication __RPC_FAR * This,
    /* [in] */ SECHANDLE newVal);


void __RPC_STUB IAuthentication_put_Context_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][helpstring][id] */ HRESULT STDMETHODCALLTYPE IAuthentication_SetBlob_Proxy( 
    IAuthentication __RPC_FAR * This,
    /* [in] */ BYTE __RPC_FAR *pvBlob,
    /* [in] */ int cbBlob);


void __RPC_STUB IAuthentication_SetBlob_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][helpstring][id] */ HRESULT STDMETHODCALLTYPE IAuthentication_GetBlobLength_Proxy( 
    IAuthentication __RPC_FAR * This,
    /* [out] */ int __RPC_FAR *pcbBlob);


void __RPC_STUB IAuthentication_GetBlobLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][helpstring][id] */ HRESULT STDMETHODCALLTYPE IAuthentication_CopyBlob_Proxy( 
    IAuthentication __RPC_FAR * This,
    /* [in] */ BYTE __RPC_FAR *pbDest);


void __RPC_STUB IAuthentication_CopyBlob_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][helpstring][id] */ HRESULT STDMETHODCALLTYPE IAuthentication_SetUserInfo_Proxy( 
    IAuthentication __RPC_FAR * This,
    /* [in] */ BSTR bstrUser,
    /* [in] */ BSTR bstrPassword,
    /* [in] */ BSTR bstrDomain);


void __RPC_STUB IAuthentication_SetUserInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAuthentication_get_AuthenticationData_Proxy( 
    IAuthentication __RPC_FAR * This,
    /* [retval][out] */ BYTE __RPC_FAR *__RPC_FAR *ppVal);


void __RPC_STUB IAuthentication_get_AuthenticationData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IAuthentication_put_Authenticate_Proxy( 
    IAuthentication __RPC_FAR * This,
    /* [in] */ IVWAuthenticate __RPC_FAR *newVal);


void __RPC_STUB IAuthentication_put_Authenticate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAuthentication_get_AuthenticationInfo_Proxy( 
    IAuthentication __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrVal);


void __RPC_STUB IAuthentication_get_AuthenticationInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAuthentication_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
