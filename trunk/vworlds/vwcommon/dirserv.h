/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:44 2000
 */
/* Compiler settings for dirserv.idl:
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

#ifndef __dirserv_h__
#define __dirserv_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IDServ_FWD_DEFINED__
#define __IDServ_FWD_DEFINED__
typedef interface IDServ IDServ;
#endif 	/* __IDServ_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IDServ_INTERFACE_DEFINED__
#define __IDServ_INTERFACE_DEFINED__

/* interface IDServ */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IDServ;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("40A94350-9AE4-11D0-8587-00A0C90F2911")
    IDServ : public IDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ConnectEx2( 
            HWND hNotificationWindow,
            BSTR bstrLDAP) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ConnectEx( 
            BSTR bstrLocation,
            BSTR bstrAdmin,
            BSTR bstrPasswd,
            HWND hNotificationWindow) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Disconnect( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetStatus( 
            /* [out] */ BSTR __RPC_FAR *bstrMsg,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbIsDSConnected) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddWorld( 
            /* [in] */ BSTR bstrWorldGUID,
            /* [in] */ BSTR bstrWorldURL,
            /* [in] */ BSTR bstrWorldNoUsers,
            /* [in] */ BSTR bstrInfoURL) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RemoveWorld( 
            /* [in] */ BSTR bstrWorldGUID) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddUser( 
            BSTR EmailName,
            BSTR InWorldAlias,
            BSTR ComputerName,
            BSTR Comment,
            BSTR bstrCurrentWorld) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RemoveUser( 
            /* [in] */ BSTR InWorldAlias) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE LogUserOn( 
            /* [in] */ BSTR InWorldAlias) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE LogUserOff( 
            /* [in] */ BSTR InWorldAlias) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE FindUser( 
            /* [in] */ BSTR inworldalias,
            /* [out] */ BSTR __RPC_FAR *emailname,
            /* [out] */ BSTR __RPC_FAR *computername,
            /* [out] */ BSTR __RPC_FAR *comment,
            /* [out] */ BSTR __RPC_FAR *bstrCurrentWorld) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE TestQueue( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE EnumerateUsers( 
            HWND hNotificationWindow) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetNextUser( 
            /* [out] */ BSTR __RPC_FAR *inworldalias,
            /* [out] */ BSTR __RPC_FAR *emailname,
            /* [out] */ BSTR __RPC_FAR *computername,
            /* [out] */ BSTR __RPC_FAR *comment,
            /* [out] */ BSTR __RPC_FAR *bstrCurrentWorld) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsEnumerating( 
            /* [out] */ VARIANT_BOOL __RPC_FAR *bIsEnum) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDServVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDServ __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDServ __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDServ __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IDServ __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IDServ __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IDServ __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IDServ __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ConnectEx2 )( 
            IDServ __RPC_FAR * This,
            HWND hNotificationWindow,
            BSTR bstrLDAP);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ConnectEx )( 
            IDServ __RPC_FAR * This,
            BSTR bstrLocation,
            BSTR bstrAdmin,
            BSTR bstrPasswd,
            HWND hNotificationWindow);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Disconnect )( 
            IDServ __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetStatus )( 
            IDServ __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *bstrMsg,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbIsDSConnected);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddWorld )( 
            IDServ __RPC_FAR * This,
            /* [in] */ BSTR bstrWorldGUID,
            /* [in] */ BSTR bstrWorldURL,
            /* [in] */ BSTR bstrWorldNoUsers,
            /* [in] */ BSTR bstrInfoURL);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveWorld )( 
            IDServ __RPC_FAR * This,
            /* [in] */ BSTR bstrWorldGUID);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddUser )( 
            IDServ __RPC_FAR * This,
            BSTR EmailName,
            BSTR InWorldAlias,
            BSTR ComputerName,
            BSTR Comment,
            BSTR bstrCurrentWorld);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveUser )( 
            IDServ __RPC_FAR * This,
            /* [in] */ BSTR InWorldAlias);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LogUserOn )( 
            IDServ __RPC_FAR * This,
            /* [in] */ BSTR InWorldAlias);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LogUserOff )( 
            IDServ __RPC_FAR * This,
            /* [in] */ BSTR InWorldAlias);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindUser )( 
            IDServ __RPC_FAR * This,
            /* [in] */ BSTR inworldalias,
            /* [out] */ BSTR __RPC_FAR *emailname,
            /* [out] */ BSTR __RPC_FAR *computername,
            /* [out] */ BSTR __RPC_FAR *comment,
            /* [out] */ BSTR __RPC_FAR *bstrCurrentWorld);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TestQueue )( 
            IDServ __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnumerateUsers )( 
            IDServ __RPC_FAR * This,
            HWND hNotificationWindow);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetNextUser )( 
            IDServ __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *inworldalias,
            /* [out] */ BSTR __RPC_FAR *emailname,
            /* [out] */ BSTR __RPC_FAR *computername,
            /* [out] */ BSTR __RPC_FAR *comment,
            /* [out] */ BSTR __RPC_FAR *bstrCurrentWorld);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsEnumerating )( 
            IDServ __RPC_FAR * This,
            /* [out] */ VARIANT_BOOL __RPC_FAR *bIsEnum);
        
        END_INTERFACE
    } IDServVtbl;

    interface IDServ
    {
        CONST_VTBL struct IDServVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDServ_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDServ_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDServ_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDServ_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDServ_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDServ_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDServ_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDServ_ConnectEx2(This,hNotificationWindow,bstrLDAP)	\
    (This)->lpVtbl -> ConnectEx2(This,hNotificationWindow,bstrLDAP)

#define IDServ_ConnectEx(This,bstrLocation,bstrAdmin,bstrPasswd,hNotificationWindow)	\
    (This)->lpVtbl -> ConnectEx(This,bstrLocation,bstrAdmin,bstrPasswd,hNotificationWindow)

#define IDServ_Disconnect(This)	\
    (This)->lpVtbl -> Disconnect(This)

#define IDServ_GetStatus(This,bstrMsg,pbIsDSConnected)	\
    (This)->lpVtbl -> GetStatus(This,bstrMsg,pbIsDSConnected)

#define IDServ_AddWorld(This,bstrWorldGUID,bstrWorldURL,bstrWorldNoUsers,bstrInfoURL)	\
    (This)->lpVtbl -> AddWorld(This,bstrWorldGUID,bstrWorldURL,bstrWorldNoUsers,bstrInfoURL)

#define IDServ_RemoveWorld(This,bstrWorldGUID)	\
    (This)->lpVtbl -> RemoveWorld(This,bstrWorldGUID)

#define IDServ_AddUser(This,EmailName,InWorldAlias,ComputerName,Comment,bstrCurrentWorld)	\
    (This)->lpVtbl -> AddUser(This,EmailName,InWorldAlias,ComputerName,Comment,bstrCurrentWorld)

#define IDServ_RemoveUser(This,InWorldAlias)	\
    (This)->lpVtbl -> RemoveUser(This,InWorldAlias)

#define IDServ_LogUserOn(This,InWorldAlias)	\
    (This)->lpVtbl -> LogUserOn(This,InWorldAlias)

#define IDServ_LogUserOff(This,InWorldAlias)	\
    (This)->lpVtbl -> LogUserOff(This,InWorldAlias)

#define IDServ_FindUser(This,inworldalias,emailname,computername,comment,bstrCurrentWorld)	\
    (This)->lpVtbl -> FindUser(This,inworldalias,emailname,computername,comment,bstrCurrentWorld)

#define IDServ_TestQueue(This)	\
    (This)->lpVtbl -> TestQueue(This)

#define IDServ_EnumerateUsers(This,hNotificationWindow)	\
    (This)->lpVtbl -> EnumerateUsers(This,hNotificationWindow)

#define IDServ_GetNextUser(This,inworldalias,emailname,computername,comment,bstrCurrentWorld)	\
    (This)->lpVtbl -> GetNextUser(This,inworldalias,emailname,computername,comment,bstrCurrentWorld)

#define IDServ_IsEnumerating(This,bIsEnum)	\
    (This)->lpVtbl -> IsEnumerating(This,bIsEnum)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDServ_ConnectEx2_Proxy( 
    IDServ __RPC_FAR * This,
    HWND hNotificationWindow,
    BSTR bstrLDAP);


void __RPC_STUB IDServ_ConnectEx2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDServ_ConnectEx_Proxy( 
    IDServ __RPC_FAR * This,
    BSTR bstrLocation,
    BSTR bstrAdmin,
    BSTR bstrPasswd,
    HWND hNotificationWindow);


void __RPC_STUB IDServ_ConnectEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDServ_Disconnect_Proxy( 
    IDServ __RPC_FAR * This);


void __RPC_STUB IDServ_Disconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDServ_GetStatus_Proxy( 
    IDServ __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *bstrMsg,
    /* [out] */ VARIANT_BOOL __RPC_FAR *pbIsDSConnected);


void __RPC_STUB IDServ_GetStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDServ_AddWorld_Proxy( 
    IDServ __RPC_FAR * This,
    /* [in] */ BSTR bstrWorldGUID,
    /* [in] */ BSTR bstrWorldURL,
    /* [in] */ BSTR bstrWorldNoUsers,
    /* [in] */ BSTR bstrInfoURL);


void __RPC_STUB IDServ_AddWorld_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDServ_RemoveWorld_Proxy( 
    IDServ __RPC_FAR * This,
    /* [in] */ BSTR bstrWorldGUID);


void __RPC_STUB IDServ_RemoveWorld_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDServ_AddUser_Proxy( 
    IDServ __RPC_FAR * This,
    BSTR EmailName,
    BSTR InWorldAlias,
    BSTR ComputerName,
    BSTR Comment,
    BSTR bstrCurrentWorld);


void __RPC_STUB IDServ_AddUser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDServ_RemoveUser_Proxy( 
    IDServ __RPC_FAR * This,
    /* [in] */ BSTR InWorldAlias);


void __RPC_STUB IDServ_RemoveUser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDServ_LogUserOn_Proxy( 
    IDServ __RPC_FAR * This,
    /* [in] */ BSTR InWorldAlias);


void __RPC_STUB IDServ_LogUserOn_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDServ_LogUserOff_Proxy( 
    IDServ __RPC_FAR * This,
    /* [in] */ BSTR InWorldAlias);


void __RPC_STUB IDServ_LogUserOff_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDServ_FindUser_Proxy( 
    IDServ __RPC_FAR * This,
    /* [in] */ BSTR inworldalias,
    /* [out] */ BSTR __RPC_FAR *emailname,
    /* [out] */ BSTR __RPC_FAR *computername,
    /* [out] */ BSTR __RPC_FAR *comment,
    /* [out] */ BSTR __RPC_FAR *bstrCurrentWorld);


void __RPC_STUB IDServ_FindUser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDServ_TestQueue_Proxy( 
    IDServ __RPC_FAR * This);


void __RPC_STUB IDServ_TestQueue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDServ_EnumerateUsers_Proxy( 
    IDServ __RPC_FAR * This,
    HWND hNotificationWindow);


void __RPC_STUB IDServ_EnumerateUsers_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDServ_GetNextUser_Proxy( 
    IDServ __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *inworldalias,
    /* [out] */ BSTR __RPC_FAR *emailname,
    /* [out] */ BSTR __RPC_FAR *computername,
    /* [out] */ BSTR __RPC_FAR *comment,
    /* [out] */ BSTR __RPC_FAR *bstrCurrentWorld);


void __RPC_STUB IDServ_GetNextUser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDServ_IsEnumerating_Proxy( 
    IDServ __RPC_FAR * This,
    /* [out] */ VARIANT_BOOL __RPC_FAR *bIsEnum);


void __RPC_STUB IDServ_IsEnumerating_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDServ_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  HWND_UserSize(     unsigned long __RPC_FAR *, unsigned long            , HWND __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  HWND_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, HWND __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  HWND_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, HWND __RPC_FAR * ); 
void                      __RPC_USER  HWND_UserFree(     unsigned long __RPC_FAR *, HWND __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
