/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:41 2000
 */
/* Compiler settings for vwanim.idl:
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

#ifndef __vwanim_h__
#define __vwanim_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IVWAnimator_FWD_DEFINED__
#define __IVWAnimator_FWD_DEFINED__
typedef interface IVWAnimator IVWAnimator;
#endif 	/* __IVWAnimator_FWD_DEFINED__ */


#ifndef __IVWAnimation_FWD_DEFINED__
#define __IVWAnimation_FWD_DEFINED__
typedef interface IVWAnimation IVWAnimation;
#endif 	/* __IVWAnimation_FWD_DEFINED__ */


#ifndef __IVWAnimationCallback_FWD_DEFINED__
#define __IVWAnimationCallback_FWD_DEFINED__
typedef interface IVWAnimationCallback IVWAnimationCallback;
#endif 	/* __IVWAnimationCallback_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "vwobject.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_vwanim_0000 */
/* [local] */ 




#define VWANIM_E_ANIMATIONNOTEXIST			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0d81)


extern RPC_IF_HANDLE __MIDL_itf_vwanim_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_vwanim_0000_v0_0_s_ifspec;

#ifndef __IVWAnimator_INTERFACE_DEFINED__
#define __IVWAnimator_INTERFACE_DEFINED__

/* interface IVWAnimator */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IVWAnimator;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FF08F5A2-2E6B-11D0-89AB-00A0C9054129")
    IVWAnimator : public IDispatch
    {
    public:
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ IWorld __RPC_FAR *pWorld) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE Terminate( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateEvent( 
            /* [in] */ int msecPeriod,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrEvent,
            /* [optional][in] */ VARIANT varArgs,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE CreateEventExt( 
            /* [in] */ int msecPeriod,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrEvent,
            /* [in] */ DISPPARAMS __RPC_FAR *dispparams,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateUIEvent( 
            /* [in] */ int msecPeriod,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrEvent,
            /* [optional][in] */ VARIANT varArgs,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Create( 
            /* [in] */ int msecPeriod,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrMethod,
            /* [optional][in] */ VARIANT varArgs,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE CreateExt( 
            /* [in] */ int msecPeriod,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrMethod,
            /* [in] */ DISPPARAMS __RPC_FAR *dispparams,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE CreateClosedExt( 
            /* [in] */ int msecPeriod,
            /* [in] */ int iInvokeMax,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrMethod,
            /* [in] */ DISPPARAMS __RPC_FAR *dispparams,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateSynchronized( 
            /* [in] */ IVWAnimation __RPC_FAR *pAnimation,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrMethod,
            /* [optional][in] */ VARIANT varArgs,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE CreateSynchronizedExt( 
            /* [in] */ IVWAnimation __RPC_FAR *pAnimation,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrMethod,
            /* [in] */ DISPPARAMS __RPC_FAR *dispparams,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateSynchronizedEvent( 
            /* [in] */ IVWAnimation __RPC_FAR *pAnimation,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrEvent,
            /* [optional][in] */ VARIANT varArgs,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE CreateSynchronizedEventExt( 
            /* [in] */ IVWAnimation __RPC_FAR *pAnimation,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrEvent,
            /* [in] */ DISPPARAMS __RPC_FAR *dispparams,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateSynchronizedUIEvent( 
            /* [in] */ IVWAnimation __RPC_FAR *pAnimation,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrEvent,
            /* [optional][in] */ VARIANT varArgs,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE CreateCallbackExt( 
            /* [in] */ int msecPeriod,
            /* [in] */ IVWAnimationCallback __RPC_FAR *pacb,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ IVWAnimation __RPC_FAR *pAnimation) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RemoveThingAnimations( 
            /* [in] */ IThing __RPC_FAR *pThing) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RemoveAll( void) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentAnimation( 
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVWAnimatorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IVWAnimator __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IVWAnimator __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IVWAnimator __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IVWAnimator __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IVWAnimator __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IVWAnimator __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IVWAnimator __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            IVWAnimator __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pWorld);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IVWAnimator __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateEvent )( 
            IVWAnimator __RPC_FAR * This,
            /* [in] */ int msecPeriod,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrEvent,
            /* [optional][in] */ VARIANT varArgs,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateEventExt )( 
            IVWAnimator __RPC_FAR * This,
            /* [in] */ int msecPeriod,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrEvent,
            /* [in] */ DISPPARAMS __RPC_FAR *dispparams,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateUIEvent )( 
            IVWAnimator __RPC_FAR * This,
            /* [in] */ int msecPeriod,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrEvent,
            /* [optional][in] */ VARIANT varArgs,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Create )( 
            IVWAnimator __RPC_FAR * This,
            /* [in] */ int msecPeriod,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrMethod,
            /* [optional][in] */ VARIANT varArgs,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateExt )( 
            IVWAnimator __RPC_FAR * This,
            /* [in] */ int msecPeriod,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrMethod,
            /* [in] */ DISPPARAMS __RPC_FAR *dispparams,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateClosedExt )( 
            IVWAnimator __RPC_FAR * This,
            /* [in] */ int msecPeriod,
            /* [in] */ int iInvokeMax,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrMethod,
            /* [in] */ DISPPARAMS __RPC_FAR *dispparams,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateSynchronized )( 
            IVWAnimator __RPC_FAR * This,
            /* [in] */ IVWAnimation __RPC_FAR *pAnimation,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrMethod,
            /* [optional][in] */ VARIANT varArgs,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateSynchronizedExt )( 
            IVWAnimator __RPC_FAR * This,
            /* [in] */ IVWAnimation __RPC_FAR *pAnimation,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrMethod,
            /* [in] */ DISPPARAMS __RPC_FAR *dispparams,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateSynchronizedEvent )( 
            IVWAnimator __RPC_FAR * This,
            /* [in] */ IVWAnimation __RPC_FAR *pAnimation,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrEvent,
            /* [optional][in] */ VARIANT varArgs,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateSynchronizedEventExt )( 
            IVWAnimator __RPC_FAR * This,
            /* [in] */ IVWAnimation __RPC_FAR *pAnimation,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrEvent,
            /* [in] */ DISPPARAMS __RPC_FAR *dispparams,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateSynchronizedUIEvent )( 
            IVWAnimator __RPC_FAR * This,
            /* [in] */ IVWAnimation __RPC_FAR *pAnimation,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrEvent,
            /* [optional][in] */ VARIANT varArgs,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateCallbackExt )( 
            IVWAnimator __RPC_FAR * This,
            /* [in] */ int msecPeriod,
            /* [in] */ IVWAnimationCallback __RPC_FAR *pacb,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Remove )( 
            IVWAnimator __RPC_FAR * This,
            /* [in] */ IVWAnimation __RPC_FAR *pAnimation);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveThingAnimations )( 
            IVWAnimator __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveAll )( 
            IVWAnimator __RPC_FAR * This);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CurrentAnimation )( 
            IVWAnimator __RPC_FAR * This,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);
        
        END_INTERFACE
    } IVWAnimatorVtbl;

    interface IVWAnimator
    {
        CONST_VTBL struct IVWAnimatorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVWAnimator_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IVWAnimator_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IVWAnimator_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IVWAnimator_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IVWAnimator_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IVWAnimator_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IVWAnimator_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IVWAnimator_Initialize(This,pWorld)	\
    (This)->lpVtbl -> Initialize(This,pWorld)

#define IVWAnimator_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IVWAnimator_CreateEvent(This,msecPeriod,pThing,bstrEvent,varArgs,ppAnimation)	\
    (This)->lpVtbl -> CreateEvent(This,msecPeriod,pThing,bstrEvent,varArgs,ppAnimation)

#define IVWAnimator_CreateEventExt(This,msecPeriod,pThing,bstrEvent,dispparams,ppAnimation)	\
    (This)->lpVtbl -> CreateEventExt(This,msecPeriod,pThing,bstrEvent,dispparams,ppAnimation)

#define IVWAnimator_CreateUIEvent(This,msecPeriod,pThing,bstrEvent,varArgs,ppAnimation)	\
    (This)->lpVtbl -> CreateUIEvent(This,msecPeriod,pThing,bstrEvent,varArgs,ppAnimation)

#define IVWAnimator_Create(This,msecPeriod,pThing,bstrMethod,varArgs,ppAnimation)	\
    (This)->lpVtbl -> Create(This,msecPeriod,pThing,bstrMethod,varArgs,ppAnimation)

#define IVWAnimator_CreateExt(This,msecPeriod,pThing,bstrMethod,dispparams,ppAnimation)	\
    (This)->lpVtbl -> CreateExt(This,msecPeriod,pThing,bstrMethod,dispparams,ppAnimation)

#define IVWAnimator_CreateClosedExt(This,msecPeriod,iInvokeMax,pThing,bstrMethod,dispparams,ppAnimation)	\
    (This)->lpVtbl -> CreateClosedExt(This,msecPeriod,iInvokeMax,pThing,bstrMethod,dispparams,ppAnimation)

#define IVWAnimator_CreateSynchronized(This,pAnimation,pThing,bstrMethod,varArgs,ppAnimation)	\
    (This)->lpVtbl -> CreateSynchronized(This,pAnimation,pThing,bstrMethod,varArgs,ppAnimation)

#define IVWAnimator_CreateSynchronizedExt(This,pAnimation,pThing,bstrMethod,dispparams,ppAnimation)	\
    (This)->lpVtbl -> CreateSynchronizedExt(This,pAnimation,pThing,bstrMethod,dispparams,ppAnimation)

#define IVWAnimator_CreateSynchronizedEvent(This,pAnimation,pThing,bstrEvent,varArgs,ppAnimation)	\
    (This)->lpVtbl -> CreateSynchronizedEvent(This,pAnimation,pThing,bstrEvent,varArgs,ppAnimation)

#define IVWAnimator_CreateSynchronizedEventExt(This,pAnimation,pThing,bstrEvent,dispparams,ppAnimation)	\
    (This)->lpVtbl -> CreateSynchronizedEventExt(This,pAnimation,pThing,bstrEvent,dispparams,ppAnimation)

#define IVWAnimator_CreateSynchronizedUIEvent(This,pAnimation,pThing,bstrEvent,varArgs,ppAnimation)	\
    (This)->lpVtbl -> CreateSynchronizedUIEvent(This,pAnimation,pThing,bstrEvent,varArgs,ppAnimation)

#define IVWAnimator_CreateCallbackExt(This,msecPeriod,pacb,ppAnimation)	\
    (This)->lpVtbl -> CreateCallbackExt(This,msecPeriod,pacb,ppAnimation)

#define IVWAnimator_Remove(This,pAnimation)	\
    (This)->lpVtbl -> Remove(This,pAnimation)

#define IVWAnimator_RemoveThingAnimations(This,pThing)	\
    (This)->lpVtbl -> RemoveThingAnimations(This,pThing)

#define IVWAnimator_RemoveAll(This)	\
    (This)->lpVtbl -> RemoveAll(This)

#define IVWAnimator_get_CurrentAnimation(This,ppAnimation)	\
    (This)->lpVtbl -> get_CurrentAnimation(This,ppAnimation)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IVWAnimator_Initialize_Proxy( 
    IVWAnimator __RPC_FAR * This,
    /* [in] */ IWorld __RPC_FAR *pWorld);


void __RPC_STUB IVWAnimator_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IVWAnimator_Terminate_Proxy( 
    IVWAnimator __RPC_FAR * This);


void __RPC_STUB IVWAnimator_Terminate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWAnimator_CreateEvent_Proxy( 
    IVWAnimator __RPC_FAR * This,
    /* [in] */ int msecPeriod,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [in] */ BSTR bstrEvent,
    /* [optional][in] */ VARIANT varArgs,
    /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);


void __RPC_STUB IVWAnimator_CreateEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IVWAnimator_CreateEventExt_Proxy( 
    IVWAnimator __RPC_FAR * This,
    /* [in] */ int msecPeriod,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [in] */ BSTR bstrEvent,
    /* [in] */ DISPPARAMS __RPC_FAR *dispparams,
    /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);


void __RPC_STUB IVWAnimator_CreateEventExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWAnimator_CreateUIEvent_Proxy( 
    IVWAnimator __RPC_FAR * This,
    /* [in] */ int msecPeriod,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [in] */ BSTR bstrEvent,
    /* [optional][in] */ VARIANT varArgs,
    /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);


void __RPC_STUB IVWAnimator_CreateUIEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWAnimator_Create_Proxy( 
    IVWAnimator __RPC_FAR * This,
    /* [in] */ int msecPeriod,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [in] */ BSTR bstrMethod,
    /* [optional][in] */ VARIANT varArgs,
    /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);


void __RPC_STUB IVWAnimator_Create_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IVWAnimator_CreateExt_Proxy( 
    IVWAnimator __RPC_FAR * This,
    /* [in] */ int msecPeriod,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [in] */ BSTR bstrMethod,
    /* [in] */ DISPPARAMS __RPC_FAR *dispparams,
    /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);


void __RPC_STUB IVWAnimator_CreateExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IVWAnimator_CreateClosedExt_Proxy( 
    IVWAnimator __RPC_FAR * This,
    /* [in] */ int msecPeriod,
    /* [in] */ int iInvokeMax,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [in] */ BSTR bstrMethod,
    /* [in] */ DISPPARAMS __RPC_FAR *dispparams,
    /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);


void __RPC_STUB IVWAnimator_CreateClosedExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWAnimator_CreateSynchronized_Proxy( 
    IVWAnimator __RPC_FAR * This,
    /* [in] */ IVWAnimation __RPC_FAR *pAnimation,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [in] */ BSTR bstrMethod,
    /* [optional][in] */ VARIANT varArgs,
    /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);


void __RPC_STUB IVWAnimator_CreateSynchronized_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IVWAnimator_CreateSynchronizedExt_Proxy( 
    IVWAnimator __RPC_FAR * This,
    /* [in] */ IVWAnimation __RPC_FAR *pAnimation,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [in] */ BSTR bstrMethod,
    /* [in] */ DISPPARAMS __RPC_FAR *dispparams,
    /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);


void __RPC_STUB IVWAnimator_CreateSynchronizedExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWAnimator_CreateSynchronizedEvent_Proxy( 
    IVWAnimator __RPC_FAR * This,
    /* [in] */ IVWAnimation __RPC_FAR *pAnimation,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [in] */ BSTR bstrEvent,
    /* [optional][in] */ VARIANT varArgs,
    /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);


void __RPC_STUB IVWAnimator_CreateSynchronizedEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IVWAnimator_CreateSynchronizedEventExt_Proxy( 
    IVWAnimator __RPC_FAR * This,
    /* [in] */ IVWAnimation __RPC_FAR *pAnimation,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [in] */ BSTR bstrEvent,
    /* [in] */ DISPPARAMS __RPC_FAR *dispparams,
    /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);


void __RPC_STUB IVWAnimator_CreateSynchronizedEventExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWAnimator_CreateSynchronizedUIEvent_Proxy( 
    IVWAnimator __RPC_FAR * This,
    /* [in] */ IVWAnimation __RPC_FAR *pAnimation,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [in] */ BSTR bstrEvent,
    /* [optional][in] */ VARIANT varArgs,
    /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);


void __RPC_STUB IVWAnimator_CreateSynchronizedUIEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IVWAnimator_CreateCallbackExt_Proxy( 
    IVWAnimator __RPC_FAR * This,
    /* [in] */ int msecPeriod,
    /* [in] */ IVWAnimationCallback __RPC_FAR *pacb,
    /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);


void __RPC_STUB IVWAnimator_CreateCallbackExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWAnimator_Remove_Proxy( 
    IVWAnimator __RPC_FAR * This,
    /* [in] */ IVWAnimation __RPC_FAR *pAnimation);


void __RPC_STUB IVWAnimator_Remove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWAnimator_RemoveThingAnimations_Proxy( 
    IVWAnimator __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing);


void __RPC_STUB IVWAnimator_RemoveThingAnimations_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWAnimator_RemoveAll_Proxy( 
    IVWAnimator __RPC_FAR * This);


void __RPC_STUB IVWAnimator_RemoveAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWAnimator_get_CurrentAnimation_Proxy( 
    IVWAnimator __RPC_FAR * This,
    /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);


void __RPC_STUB IVWAnimator_get_CurrentAnimation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IVWAnimator_INTERFACE_DEFINED__ */


#ifndef __IVWAnimation_INTERFACE_DEFINED__
#define __IVWAnimation_INTERFACE_DEFINED__

/* interface IVWAnimation */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IVWAnimation;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FF08F5A4-2E6B-11D0-89AB-00A0C9054129")
    IVWAnimation : public IDispatch
    {
    public:
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ IWorld __RPC_FAR *pWorld) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE Terminate( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ int msecNow) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Clone( 
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_TimeOfLastInvocation( 
            /* [retval][out] */ int __RPC_FAR *pmsec) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_TimeOfCreation( 
            /* [retval][out] */ int __RPC_FAR *pmsec) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_CurrentTime( 
            /* [retval][out] */ int __RPC_FAR *pmsec) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_TimesInvoked( 
            /* [retval][out] */ int __RPC_FAR *pcInvoked) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Period( 
            /* [in] */ int msecPeriod) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Period( 
            /* [retval][out] */ int __RPC_FAR *pmsecPeriod) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_InvokeMax( 
            /* [in] */ int iInvokeMax) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_InvokeMax( 
            /* [retval][out] */ int __RPC_FAR *iInvokeMax) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Thing( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Thing( 
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_User( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_User( 
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Method( 
            /* [in] */ BSTR bstrMethod) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Method( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrMethod) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Event( 
            /* [in] */ BSTR bstrEvent) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Event( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrEvent) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_UIEvent( 
            /* [in] */ BSTR bstrEvent) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_UIEvent( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrEvent) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Callback( 
            /* [in] */ IVWAnimationCallback __RPC_FAR *pacb) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Callback( 
            /* [retval][out] */ IVWAnimationCallback __RPC_FAR *__RPC_FAR *ppacb) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_ArgumentsExt( 
            /* [retval][out] */ DISPPARAMS __RPC_FAR *__RPC_FAR *ppdispparams) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_ArgumentsExt( 
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_UIArguments( 
            /* [in] */ VARIANT var) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVWAnimationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IVWAnimation __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IVWAnimation __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IVWAnimation __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IVWAnimation __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IVWAnimation __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IVWAnimation __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IVWAnimation __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            IVWAnimation __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pWorld);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IVWAnimation __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IVWAnimation __RPC_FAR * This,
            /* [in] */ int msecNow);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IVWAnimation __RPC_FAR * This,
            /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TimeOfLastInvocation )( 
            IVWAnimation __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *pmsec);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TimeOfCreation )( 
            IVWAnimation __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *pmsec);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CurrentTime )( 
            IVWAnimation __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *pmsec);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TimesInvoked )( 
            IVWAnimation __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *pcInvoked);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Period )( 
            IVWAnimation __RPC_FAR * This,
            /* [in] */ int msecPeriod);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Period )( 
            IVWAnimation __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *pmsecPeriod);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_InvokeMax )( 
            IVWAnimation __RPC_FAR * This,
            /* [in] */ int iInvokeMax);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_InvokeMax )( 
            IVWAnimation __RPC_FAR * This,
            /* [retval][out] */ int __RPC_FAR *iInvokeMax);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Thing )( 
            IVWAnimation __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Thing )( 
            IVWAnimation __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_User )( 
            IVWAnimation __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_User )( 
            IVWAnimation __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Method )( 
            IVWAnimation __RPC_FAR * This,
            /* [in] */ BSTR bstrMethod);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Method )( 
            IVWAnimation __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrMethod);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Event )( 
            IVWAnimation __RPC_FAR * This,
            /* [in] */ BSTR bstrEvent);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Event )( 
            IVWAnimation __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrEvent);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_UIEvent )( 
            IVWAnimation __RPC_FAR * This,
            /* [in] */ BSTR bstrEvent);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_UIEvent )( 
            IVWAnimation __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrEvent);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Callback )( 
            IVWAnimation __RPC_FAR * This,
            /* [in] */ IVWAnimationCallback __RPC_FAR *pacb);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Callback )( 
            IVWAnimation __RPC_FAR * This,
            /* [retval][out] */ IVWAnimationCallback __RPC_FAR *__RPC_FAR *ppacb);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ArgumentsExt )( 
            IVWAnimation __RPC_FAR * This,
            /* [retval][out] */ DISPPARAMS __RPC_FAR *__RPC_FAR *ppdispparams);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ArgumentsExt )( 
            IVWAnimation __RPC_FAR * This,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_UIArguments )( 
            IVWAnimation __RPC_FAR * This,
            /* [in] */ VARIANT var);
        
        END_INTERFACE
    } IVWAnimationVtbl;

    interface IVWAnimation
    {
        CONST_VTBL struct IVWAnimationVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVWAnimation_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IVWAnimation_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IVWAnimation_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IVWAnimation_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IVWAnimation_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IVWAnimation_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IVWAnimation_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IVWAnimation_Initialize(This,pWorld)	\
    (This)->lpVtbl -> Initialize(This,pWorld)

#define IVWAnimation_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IVWAnimation_Invoke(This,msecNow)	\
    (This)->lpVtbl -> Invoke(This,msecNow)

#define IVWAnimation_Clone(This,ppAnimation)	\
    (This)->lpVtbl -> Clone(This,ppAnimation)

#define IVWAnimation_get_TimeOfLastInvocation(This,pmsec)	\
    (This)->lpVtbl -> get_TimeOfLastInvocation(This,pmsec)

#define IVWAnimation_get_TimeOfCreation(This,pmsec)	\
    (This)->lpVtbl -> get_TimeOfCreation(This,pmsec)

#define IVWAnimation_get_CurrentTime(This,pmsec)	\
    (This)->lpVtbl -> get_CurrentTime(This,pmsec)

#define IVWAnimation_get_TimesInvoked(This,pcInvoked)	\
    (This)->lpVtbl -> get_TimesInvoked(This,pcInvoked)

#define IVWAnimation_put_Period(This,msecPeriod)	\
    (This)->lpVtbl -> put_Period(This,msecPeriod)

#define IVWAnimation_get_Period(This,pmsecPeriod)	\
    (This)->lpVtbl -> get_Period(This,pmsecPeriod)

#define IVWAnimation_put_InvokeMax(This,iInvokeMax)	\
    (This)->lpVtbl -> put_InvokeMax(This,iInvokeMax)

#define IVWAnimation_get_InvokeMax(This,iInvokeMax)	\
    (This)->lpVtbl -> get_InvokeMax(This,iInvokeMax)

#define IVWAnimation_put_Thing(This,pthing)	\
    (This)->lpVtbl -> put_Thing(This,pthing)

#define IVWAnimation_get_Thing(This,ppthing)	\
    (This)->lpVtbl -> get_Thing(This,ppthing)

#define IVWAnimation_put_User(This,pthing)	\
    (This)->lpVtbl -> put_User(This,pthing)

#define IVWAnimation_get_User(This,ppthing)	\
    (This)->lpVtbl -> get_User(This,ppthing)

#define IVWAnimation_put_Method(This,bstrMethod)	\
    (This)->lpVtbl -> put_Method(This,bstrMethod)

#define IVWAnimation_get_Method(This,pbstrMethod)	\
    (This)->lpVtbl -> get_Method(This,pbstrMethod)

#define IVWAnimation_put_Event(This,bstrEvent)	\
    (This)->lpVtbl -> put_Event(This,bstrEvent)

#define IVWAnimation_get_Event(This,pbstrEvent)	\
    (This)->lpVtbl -> get_Event(This,pbstrEvent)

#define IVWAnimation_put_UIEvent(This,bstrEvent)	\
    (This)->lpVtbl -> put_UIEvent(This,bstrEvent)

#define IVWAnimation_get_UIEvent(This,pbstrEvent)	\
    (This)->lpVtbl -> get_UIEvent(This,pbstrEvent)

#define IVWAnimation_put_Callback(This,pacb)	\
    (This)->lpVtbl -> put_Callback(This,pacb)

#define IVWAnimation_get_Callback(This,ppacb)	\
    (This)->lpVtbl -> get_Callback(This,ppacb)

#define IVWAnimation_get_ArgumentsExt(This,ppdispparams)	\
    (This)->lpVtbl -> get_ArgumentsExt(This,ppdispparams)

#define IVWAnimation_put_ArgumentsExt(This,pdispparams)	\
    (This)->lpVtbl -> put_ArgumentsExt(This,pdispparams)

#define IVWAnimation_put_UIArguments(This,var)	\
    (This)->lpVtbl -> put_UIArguments(This,var)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IVWAnimation_Initialize_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [in] */ IWorld __RPC_FAR *pWorld);


void __RPC_STUB IVWAnimation_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IVWAnimation_Terminate_Proxy( 
    IVWAnimation __RPC_FAR * This);


void __RPC_STUB IVWAnimation_Terminate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWAnimation_Invoke_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [in] */ int msecNow);


void __RPC_STUB IVWAnimation_Invoke_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWAnimation_Clone_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [retval][out] */ IVWAnimation __RPC_FAR *__RPC_FAR *ppAnimation);


void __RPC_STUB IVWAnimation_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWAnimation_get_TimeOfLastInvocation_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *pmsec);


void __RPC_STUB IVWAnimation_get_TimeOfLastInvocation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWAnimation_get_TimeOfCreation_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *pmsec);


void __RPC_STUB IVWAnimation_get_TimeOfCreation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWAnimation_get_CurrentTime_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *pmsec);


void __RPC_STUB IVWAnimation_get_CurrentTime_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWAnimation_get_TimesInvoked_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *pcInvoked);


void __RPC_STUB IVWAnimation_get_TimesInvoked_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IVWAnimation_put_Period_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [in] */ int msecPeriod);


void __RPC_STUB IVWAnimation_put_Period_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWAnimation_get_Period_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *pmsecPeriod);


void __RPC_STUB IVWAnimation_get_Period_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IVWAnimation_put_InvokeMax_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [in] */ int iInvokeMax);


void __RPC_STUB IVWAnimation_put_InvokeMax_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWAnimation_get_InvokeMax_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [retval][out] */ int __RPC_FAR *iInvokeMax);


void __RPC_STUB IVWAnimation_get_InvokeMax_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IVWAnimation_put_Thing_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IVWAnimation_put_Thing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWAnimation_get_Thing_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IVWAnimation_get_Thing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IVWAnimation_put_User_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IVWAnimation_put_User_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWAnimation_get_User_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);


void __RPC_STUB IVWAnimation_get_User_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IVWAnimation_put_Method_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [in] */ BSTR bstrMethod);


void __RPC_STUB IVWAnimation_put_Method_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWAnimation_get_Method_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrMethod);


void __RPC_STUB IVWAnimation_get_Method_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IVWAnimation_put_Event_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [in] */ BSTR bstrEvent);


void __RPC_STUB IVWAnimation_put_Event_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWAnimation_get_Event_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrEvent);


void __RPC_STUB IVWAnimation_get_Event_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IVWAnimation_put_UIEvent_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [in] */ BSTR bstrEvent);


void __RPC_STUB IVWAnimation_put_UIEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWAnimation_get_UIEvent_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrEvent);


void __RPC_STUB IVWAnimation_get_UIEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IVWAnimation_put_Callback_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [in] */ IVWAnimationCallback __RPC_FAR *pacb);


void __RPC_STUB IVWAnimation_put_Callback_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWAnimation_get_Callback_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [retval][out] */ IVWAnimationCallback __RPC_FAR *__RPC_FAR *ppacb);


void __RPC_STUB IVWAnimation_get_Callback_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IVWAnimation_get_ArgumentsExt_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [retval][out] */ DISPPARAMS __RPC_FAR *__RPC_FAR *ppdispparams);


void __RPC_STUB IVWAnimation_get_ArgumentsExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IVWAnimation_put_ArgumentsExt_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams);


void __RPC_STUB IVWAnimation_put_ArgumentsExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE IVWAnimation_put_UIArguments_Proxy( 
    IVWAnimation __RPC_FAR * This,
    /* [in] */ VARIANT var);


void __RPC_STUB IVWAnimation_put_UIArguments_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IVWAnimation_INTERFACE_DEFINED__ */


#ifndef __IVWAnimationCallback_INTERFACE_DEFINED__
#define __IVWAnimationCallback_INTERFACE_DEFINED__

/* interface IVWAnimationCallback */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IVWAnimationCallback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FF08F5A6-2E6B-11D0-89AB-00A0C9054129")
    IVWAnimationCallback : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Invoke( 
            IVWAnimation __RPC_FAR *pani) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVWAnimationCallbackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IVWAnimationCallback __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IVWAnimationCallback __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IVWAnimationCallback __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IVWAnimationCallback __RPC_FAR * This,
            IVWAnimation __RPC_FAR *pani);
        
        END_INTERFACE
    } IVWAnimationCallbackVtbl;

    interface IVWAnimationCallback
    {
        CONST_VTBL struct IVWAnimationCallbackVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVWAnimationCallback_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IVWAnimationCallback_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IVWAnimationCallback_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IVWAnimationCallback_Invoke(This,pani)	\
    (This)->lpVtbl -> Invoke(This,pani)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWAnimationCallback_Invoke_Proxy( 
    IVWAnimationCallback __RPC_FAR * This,
    IVWAnimation __RPC_FAR *pani);


void __RPC_STUB IVWAnimationCallback_Invoke_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IVWAnimationCallback_INTERFACE_DEFINED__ */


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
