/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:29 2000
 */
/* Compiler settings for ocxproxy.idl:
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

#ifndef __ocxproxy_h__
#define __ocxproxy_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IOCXProxy_FWD_DEFINED__
#define __IOCXProxy_FWD_DEFINED__
typedef interface IOCXProxy IOCXProxy;
#endif 	/* __IOCXProxy_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "vwprop.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IOCXProxy_INTERFACE_DEFINED__
#define __IOCXProxy_INTERFACE_DEFINED__

/* interface IOCXProxy */
/* [unique][helpstring][dual][uuid][object] */ 

#define	DISPID_OCXPROXY_OCX	( 101 )


EXTERN_C const IID IID_IOCXProxy;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("09362218-2570-11D1-83BF-00C04FB6FA46")
    IOCXProxy : public IObjectProperty
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_OCX( 
            /* [retval][out] */ LPDISPATCH __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_OCX( 
            /* [in] */ LPDISPATCH newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOCXProxyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IOCXProxy __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IOCXProxy __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IOCXProxy __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IOCXProxy __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IOCXProxy __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IOCXProxy __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IOCXProxy __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IOCXProxy __RPC_FAR * This);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_RefCount )( 
            IOCXProxy __RPC_FAR * This,
            /* [retval][out] */ DWORD __RPC_FAR *pdwRef);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_CLSID )( 
            IOCXProxy __RPC_FAR * This,
            /* [retval][out] */ CLSID __RPC_FAR *pclsid);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IID )( 
            IOCXProxy __RPC_FAR * This,
            /* [retval][out] */ IID __RPC_FAR *piid);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_World )( 
            IOCXProxy __RPC_FAR * This,
            /* [retval][out] */ IWorld __RPC_FAR *__RPC_FAR *ppworld);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_World )( 
            IOCXProxy __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Parent )( 
            IOCXProxy __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppthing);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Parent )( 
            IOCXProxy __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PropertyName )( 
            IOCXProxy __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PropertyName )( 
            IOCXProxy __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [id][helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Type )( 
            IOCXProxy __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [id][helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Type )( 
            IOCXProxy __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring][propget][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Security )( 
            IOCXProxy __RPC_FAR * This,
            /* [retval][out] */ IPropertySecurity __RPC_FAR *__RPC_FAR *ppSecurity);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Security )( 
            IOCXProxy __RPC_FAR * This,
            /* [in] */ IPropertySecurity __RPC_FAR *pSecurity);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsOfType )( 
            IOCXProxy __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ToAscii )( 
            IOCXProxy __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Dump )( 
            IOCXProxy __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IOCXProxy __RPC_FAR * This,
            /* [in] */ IObjectProperty __RPC_FAR *pobjectproperty);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Marshall )( 
            IOCXProxy __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ IMarshallBuffer __RPC_FAR *pmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnMarshall )( 
            IOCXProxy __RPC_FAR * This,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *punmarshallbuffer);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MarshallToScript )( 
            IOCXProxy __RPC_FAR * This,
            /* [in] */ DWORD dwOptions,
            /* [in] */ BSTR bstrContext,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbAsVal,
            /* [out] */ BSTR __RPC_FAR *pbstrOut);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_OCX )( 
            IOCXProxy __RPC_FAR * This,
            /* [retval][out] */ LPDISPATCH __RPC_FAR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_OCX )( 
            IOCXProxy __RPC_FAR * This,
            /* [in] */ LPDISPATCH newVal);
        
        END_INTERFACE
    } IOCXProxyVtbl;

    interface IOCXProxy
    {
        CONST_VTBL struct IOCXProxyVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOCXProxy_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IOCXProxy_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IOCXProxy_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IOCXProxy_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IOCXProxy_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IOCXProxy_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IOCXProxy_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IOCXProxy_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IOCXProxy_get_RefCount(This,pdwRef)	\
    (This)->lpVtbl -> get_RefCount(This,pdwRef)

#define IOCXProxy_get_CLSID(This,pclsid)	\
    (This)->lpVtbl -> get_CLSID(This,pclsid)

#define IOCXProxy_get_IID(This,piid)	\
    (This)->lpVtbl -> get_IID(This,piid)

#define IOCXProxy_get_World(This,ppworld)	\
    (This)->lpVtbl -> get_World(This,ppworld)

#define IOCXProxy_put_World(This,pworld)	\
    (This)->lpVtbl -> put_World(This,pworld)

#define IOCXProxy_get_Parent(This,ppthing)	\
    (This)->lpVtbl -> get_Parent(This,ppthing)

#define IOCXProxy_put_Parent(This,pthing)	\
    (This)->lpVtbl -> put_Parent(This,pthing)

#define IOCXProxy_get_PropertyName(This,pbstr)	\
    (This)->lpVtbl -> get_PropertyName(This,pbstr)

#define IOCXProxy_put_PropertyName(This,bstr)	\
    (This)->lpVtbl -> put_PropertyName(This,bstr)

#define IOCXProxy_get_Type(This,pbstr)	\
    (This)->lpVtbl -> get_Type(This,pbstr)

#define IOCXProxy_put_Type(This,bstr)	\
    (This)->lpVtbl -> put_Type(This,bstr)

#define IOCXProxy_get_Security(This,ppSecurity)	\
    (This)->lpVtbl -> get_Security(This,ppSecurity)

#define IOCXProxy_put_Security(This,pSecurity)	\
    (This)->lpVtbl -> put_Security(This,pSecurity)

#define IOCXProxy_IsOfType(This,bstr,pbool)	\
    (This)->lpVtbl -> IsOfType(This,bstr,pbool)

#define IOCXProxy_ToAscii(This,pbstr)	\
    (This)->lpVtbl -> ToAscii(This,pbstr)

#define IOCXProxy_Dump(This)	\
    (This)->lpVtbl -> Dump(This)

#define IOCXProxy_Clone(This,pobjectproperty)	\
    (This)->lpVtbl -> Clone(This,pobjectproperty)

#define IOCXProxy_Marshall(This,dwOptions,pmarshallbuffer)	\
    (This)->lpVtbl -> Marshall(This,dwOptions,pmarshallbuffer)

#define IOCXProxy_UnMarshall(This,punmarshallbuffer)	\
    (This)->lpVtbl -> UnMarshall(This,punmarshallbuffer)

#define IOCXProxy_MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)	\
    (This)->lpVtbl -> MarshallToScript(This,dwOptions,bstrContext,pbAsVal,pbstrOut)


#define IOCXProxy_get_OCX(This,pVal)	\
    (This)->lpVtbl -> get_OCX(This,pVal)

#define IOCXProxy_put_OCX(This,newVal)	\
    (This)->lpVtbl -> put_OCX(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IOCXProxy_get_OCX_Proxy( 
    IOCXProxy __RPC_FAR * This,
    /* [retval][out] */ LPDISPATCH __RPC_FAR *pVal);


void __RPC_STUB IOCXProxy_get_OCX_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IOCXProxy_put_OCX_Proxy( 
    IOCXProxy __RPC_FAR * This,
    /* [in] */ LPDISPATCH newVal);


void __RPC_STUB IOCXProxy_put_OCX_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IOCXProxy_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
