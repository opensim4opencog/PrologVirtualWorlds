/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:39 2000
 */
/* Compiler settings for vwbuffer.idl:
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

#ifndef __vwbuffer_h__
#define __vwbuffer_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IUnMarshallBuffer_FWD_DEFINED__
#define __IUnMarshallBuffer_FWD_DEFINED__
typedef interface IUnMarshallBuffer IUnMarshallBuffer;
#endif 	/* __IUnMarshallBuffer_FWD_DEFINED__ */


#ifndef __IMarshallBuffer_FWD_DEFINED__
#define __IMarshallBuffer_FWD_DEFINED__
typedef interface IMarshallBuffer IMarshallBuffer;
#endif 	/* __IMarshallBuffer_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IUnMarshallBuffer_INTERFACE_DEFINED__
#define __IUnMarshallBuffer_INTERFACE_DEFINED__

/* interface IUnMarshallBuffer */
/* [unique][helpstring][local][dual][uuid][object] */ 


EXTERN_C const IID IID_IUnMarshallBuffer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("590D9830-A417-11CF-9BF0-0080C7A56A8A")
    IUnMarshallBuffer : public IDispatch
    {
    public:
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_ClassID( 
            /* [out] */ CLSID __RPC_FAR *pclsid) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Byte( 
            /* [out] */ unsigned char __RPC_FAR *bval) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Bytes( 
            /* [out] */ unsigned char __RPC_FAR *pbytes,
            /* [in] */ long lval) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Boolean( 
            /* [out] */ VARIANT_BOOL __RPC_FAR *boolval) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_String( 
            /* [out] */ BSTR __RPC_FAR *pbstr) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_UShort( 
            /* [out] */ unsigned short __RPC_FAR *pusval) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Short( 
            /* [out] */ short __RPC_FAR *psval) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Float( 
            /* [out] */ float __RPC_FAR *pfval) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Double( 
            /* [out] */ double __RPC_FAR *pdval) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_UInt( 
            /* [out] */ unsigned int __RPC_FAR *puival) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Int( 
            /* [out] */ int __RPC_FAR *pival) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_ULong( 
            /* [out] */ unsigned long __RPC_FAR *pulval) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_Long( 
            /* [out] */ long __RPC_FAR *plval) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_BLOB( 
            /* [out] */ BSTR __RPC_FAR *pblobval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CopyTo( 
            /* [in] */ unsigned char __RPC_FAR *pbytes,
            /* [in] */ DWORD lval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ReadFromDisk( 
            /* [in] */ BSTR strFilename) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Attach( 
            /* [in] */ unsigned char __RPC_FAR *pbytes,
            /* [in] */ DWORD lval) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Length( 
            /* [in] */ DWORD __RPC_FAR *lval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SeekToBegin( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Rewind( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUnMarshallBufferVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IUnMarshallBuffer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IUnMarshallBuffer __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IUnMarshallBuffer __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IUnMarshallBuffer __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IUnMarshallBuffer __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IUnMarshallBuffer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IUnMarshallBuffer __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ClassID )( 
            IUnMarshallBuffer __RPC_FAR * This,
            /* [out] */ CLSID __RPC_FAR *pclsid);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Byte )( 
            IUnMarshallBuffer __RPC_FAR * This,
            /* [out] */ unsigned char __RPC_FAR *bval);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Bytes )( 
            IUnMarshallBuffer __RPC_FAR * This,
            /* [out] */ unsigned char __RPC_FAR *pbytes,
            /* [in] */ long lval);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Boolean )( 
            IUnMarshallBuffer __RPC_FAR * This,
            /* [out] */ VARIANT_BOOL __RPC_FAR *boolval);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_String )( 
            IUnMarshallBuffer __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *pbstr);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_UShort )( 
            IUnMarshallBuffer __RPC_FAR * This,
            /* [out] */ unsigned short __RPC_FAR *pusval);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Short )( 
            IUnMarshallBuffer __RPC_FAR * This,
            /* [out] */ short __RPC_FAR *psval);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Float )( 
            IUnMarshallBuffer __RPC_FAR * This,
            /* [out] */ float __RPC_FAR *pfval);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Double )( 
            IUnMarshallBuffer __RPC_FAR * This,
            /* [out] */ double __RPC_FAR *pdval);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_UInt )( 
            IUnMarshallBuffer __RPC_FAR * This,
            /* [out] */ unsigned int __RPC_FAR *puival);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Int )( 
            IUnMarshallBuffer __RPC_FAR * This,
            /* [out] */ int __RPC_FAR *pival);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ULong )( 
            IUnMarshallBuffer __RPC_FAR * This,
            /* [out] */ unsigned long __RPC_FAR *pulval);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Long )( 
            IUnMarshallBuffer __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *plval);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_BLOB )( 
            IUnMarshallBuffer __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *pblobval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CopyTo )( 
            IUnMarshallBuffer __RPC_FAR * This,
            /* [in] */ unsigned char __RPC_FAR *pbytes,
            /* [in] */ DWORD lval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadFromDisk )( 
            IUnMarshallBuffer __RPC_FAR * This,
            /* [in] */ BSTR strFilename);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Attach )( 
            IUnMarshallBuffer __RPC_FAR * This,
            /* [in] */ unsigned char __RPC_FAR *pbytes,
            /* [in] */ DWORD lval);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Length )( 
            IUnMarshallBuffer __RPC_FAR * This,
            /* [in] */ DWORD __RPC_FAR *lval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SeekToBegin )( 
            IUnMarshallBuffer __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Rewind )( 
            IUnMarshallBuffer __RPC_FAR * This);
        
        END_INTERFACE
    } IUnMarshallBufferVtbl;

    interface IUnMarshallBuffer
    {
        CONST_VTBL struct IUnMarshallBufferVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUnMarshallBuffer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IUnMarshallBuffer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IUnMarshallBuffer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IUnMarshallBuffer_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IUnMarshallBuffer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IUnMarshallBuffer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IUnMarshallBuffer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IUnMarshallBuffer_get_ClassID(This,pclsid)	\
    (This)->lpVtbl -> get_ClassID(This,pclsid)

#define IUnMarshallBuffer_get_Byte(This,bval)	\
    (This)->lpVtbl -> get_Byte(This,bval)

#define IUnMarshallBuffer_get_Bytes(This,pbytes,lval)	\
    (This)->lpVtbl -> get_Bytes(This,pbytes,lval)

#define IUnMarshallBuffer_get_Boolean(This,boolval)	\
    (This)->lpVtbl -> get_Boolean(This,boolval)

#define IUnMarshallBuffer_get_String(This,pbstr)	\
    (This)->lpVtbl -> get_String(This,pbstr)

#define IUnMarshallBuffer_get_UShort(This,pusval)	\
    (This)->lpVtbl -> get_UShort(This,pusval)

#define IUnMarshallBuffer_get_Short(This,psval)	\
    (This)->lpVtbl -> get_Short(This,psval)

#define IUnMarshallBuffer_get_Float(This,pfval)	\
    (This)->lpVtbl -> get_Float(This,pfval)

#define IUnMarshallBuffer_get_Double(This,pdval)	\
    (This)->lpVtbl -> get_Double(This,pdval)

#define IUnMarshallBuffer_get_UInt(This,puival)	\
    (This)->lpVtbl -> get_UInt(This,puival)

#define IUnMarshallBuffer_get_Int(This,pival)	\
    (This)->lpVtbl -> get_Int(This,pival)

#define IUnMarshallBuffer_get_ULong(This,pulval)	\
    (This)->lpVtbl -> get_ULong(This,pulval)

#define IUnMarshallBuffer_get_Long(This,plval)	\
    (This)->lpVtbl -> get_Long(This,plval)

#define IUnMarshallBuffer_get_BLOB(This,pblobval)	\
    (This)->lpVtbl -> get_BLOB(This,pblobval)

#define IUnMarshallBuffer_CopyTo(This,pbytes,lval)	\
    (This)->lpVtbl -> CopyTo(This,pbytes,lval)

#define IUnMarshallBuffer_ReadFromDisk(This,strFilename)	\
    (This)->lpVtbl -> ReadFromDisk(This,strFilename)

#define IUnMarshallBuffer_Attach(This,pbytes,lval)	\
    (This)->lpVtbl -> Attach(This,pbytes,lval)

#define IUnMarshallBuffer_get_Length(This,lval)	\
    (This)->lpVtbl -> get_Length(This,lval)

#define IUnMarshallBuffer_SeekToBegin(This)	\
    (This)->lpVtbl -> SeekToBegin(This)

#define IUnMarshallBuffer_Rewind(This)	\
    (This)->lpVtbl -> Rewind(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IUnMarshallBuffer_get_ClassID_Proxy( 
    IUnMarshallBuffer __RPC_FAR * This,
    /* [out] */ CLSID __RPC_FAR *pclsid);


void __RPC_STUB IUnMarshallBuffer_get_ClassID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IUnMarshallBuffer_get_Byte_Proxy( 
    IUnMarshallBuffer __RPC_FAR * This,
    /* [out] */ unsigned char __RPC_FAR *bval);


void __RPC_STUB IUnMarshallBuffer_get_Byte_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IUnMarshallBuffer_get_Bytes_Proxy( 
    IUnMarshallBuffer __RPC_FAR * This,
    /* [out] */ unsigned char __RPC_FAR *pbytes,
    /* [in] */ long lval);


void __RPC_STUB IUnMarshallBuffer_get_Bytes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IUnMarshallBuffer_get_Boolean_Proxy( 
    IUnMarshallBuffer __RPC_FAR * This,
    /* [out] */ VARIANT_BOOL __RPC_FAR *boolval);


void __RPC_STUB IUnMarshallBuffer_get_Boolean_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IUnMarshallBuffer_get_String_Proxy( 
    IUnMarshallBuffer __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *pbstr);


void __RPC_STUB IUnMarshallBuffer_get_String_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IUnMarshallBuffer_get_UShort_Proxy( 
    IUnMarshallBuffer __RPC_FAR * This,
    /* [out] */ unsigned short __RPC_FAR *pusval);


void __RPC_STUB IUnMarshallBuffer_get_UShort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IUnMarshallBuffer_get_Short_Proxy( 
    IUnMarshallBuffer __RPC_FAR * This,
    /* [out] */ short __RPC_FAR *psval);


void __RPC_STUB IUnMarshallBuffer_get_Short_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IUnMarshallBuffer_get_Float_Proxy( 
    IUnMarshallBuffer __RPC_FAR * This,
    /* [out] */ float __RPC_FAR *pfval);


void __RPC_STUB IUnMarshallBuffer_get_Float_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IUnMarshallBuffer_get_Double_Proxy( 
    IUnMarshallBuffer __RPC_FAR * This,
    /* [out] */ double __RPC_FAR *pdval);


void __RPC_STUB IUnMarshallBuffer_get_Double_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IUnMarshallBuffer_get_UInt_Proxy( 
    IUnMarshallBuffer __RPC_FAR * This,
    /* [out] */ unsigned int __RPC_FAR *puival);


void __RPC_STUB IUnMarshallBuffer_get_UInt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IUnMarshallBuffer_get_Int_Proxy( 
    IUnMarshallBuffer __RPC_FAR * This,
    /* [out] */ int __RPC_FAR *pival);


void __RPC_STUB IUnMarshallBuffer_get_Int_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IUnMarshallBuffer_get_ULong_Proxy( 
    IUnMarshallBuffer __RPC_FAR * This,
    /* [out] */ unsigned long __RPC_FAR *pulval);


void __RPC_STUB IUnMarshallBuffer_get_ULong_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IUnMarshallBuffer_get_Long_Proxy( 
    IUnMarshallBuffer __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *plval);


void __RPC_STUB IUnMarshallBuffer_get_Long_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IUnMarshallBuffer_get_BLOB_Proxy( 
    IUnMarshallBuffer __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *pblobval);


void __RPC_STUB IUnMarshallBuffer_get_BLOB_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUnMarshallBuffer_CopyTo_Proxy( 
    IUnMarshallBuffer __RPC_FAR * This,
    /* [in] */ unsigned char __RPC_FAR *pbytes,
    /* [in] */ DWORD lval);


void __RPC_STUB IUnMarshallBuffer_CopyTo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUnMarshallBuffer_ReadFromDisk_Proxy( 
    IUnMarshallBuffer __RPC_FAR * This,
    /* [in] */ BSTR strFilename);


void __RPC_STUB IUnMarshallBuffer_ReadFromDisk_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUnMarshallBuffer_Attach_Proxy( 
    IUnMarshallBuffer __RPC_FAR * This,
    /* [in] */ unsigned char __RPC_FAR *pbytes,
    /* [in] */ DWORD lval);


void __RPC_STUB IUnMarshallBuffer_Attach_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IUnMarshallBuffer_get_Length_Proxy( 
    IUnMarshallBuffer __RPC_FAR * This,
    /* [in] */ DWORD __RPC_FAR *lval);


void __RPC_STUB IUnMarshallBuffer_get_Length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUnMarshallBuffer_SeekToBegin_Proxy( 
    IUnMarshallBuffer __RPC_FAR * This);


void __RPC_STUB IUnMarshallBuffer_SeekToBegin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IUnMarshallBuffer_Rewind_Proxy( 
    IUnMarshallBuffer __RPC_FAR * This);


void __RPC_STUB IUnMarshallBuffer_Rewind_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IUnMarshallBuffer_INTERFACE_DEFINED__ */


#ifndef __IMarshallBuffer_INTERFACE_DEFINED__
#define __IMarshallBuffer_INTERFACE_DEFINED__

/* interface IMarshallBuffer */
/* [unique][helpstring][local][dual][uuid][object] */ 


EXTERN_C const IID IID_IMarshallBuffer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("590D9832-A417-11CF-9BF0-0080C7A56A8A")
    IMarshallBuffer : public IDispatch
    {
    public:
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_ClassID( 
            /* [in] */ REFCLSID clsid) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Byte( 
            /* [in] */ unsigned char bval) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Bytes( 
            /* [in] */ unsigned char __RPC_FAR *pbytes,
            /* [in] */ long lval) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Boolean( 
            /* [in] */ VARIANT_BOOL boolval) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_String( 
            /* [in] */ BSTR bstr,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL bCompress = -1) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Float( 
            /* [in] */ float fval) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Double( 
            /* [in] */ double dval) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_UShort( 
            /* [in] */ unsigned short usval) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Short( 
            /* [in] */ short sval) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_UInt( 
            /* [in] */ unsigned int uival) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Int( 
            /* [in] */ int ival) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_ULong( 
            /* [in] */ unsigned long ulval) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_Long( 
            /* [in] */ long lval) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_BLOB( 
            /* [in] */ BSTR blobval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SeekToBegin( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Rewind( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CopyFrom( 
            /* [in] */ unsigned char __RPC_FAR *pbytes,
            /* [in] */ DWORD lval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Detach( 
            /* [out] */ unsigned char __RPC_FAR *__RPC_FAR *ppbytes,
            /* [out] */ DWORD __RPC_FAR *plval) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Length( 
            /* [in] */ DWORD __RPC_FAR *lval) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE WriteToDisk( 
            /* [in] */ BSTR strFilename) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMarshallBufferVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMarshallBuffer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMarshallBuffer __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMarshallBuffer __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IMarshallBuffer __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IMarshallBuffer __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IMarshallBuffer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IMarshallBuffer __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ClassID )( 
            IMarshallBuffer __RPC_FAR * This,
            /* [in] */ REFCLSID clsid);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Byte )( 
            IMarshallBuffer __RPC_FAR * This,
            /* [in] */ unsigned char bval);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Bytes )( 
            IMarshallBuffer __RPC_FAR * This,
            /* [in] */ unsigned char __RPC_FAR *pbytes,
            /* [in] */ long lval);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Boolean )( 
            IMarshallBuffer __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL boolval);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_String )( 
            IMarshallBuffer __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [defaultvalue][optional][in] */ VARIANT_BOOL bCompress);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Float )( 
            IMarshallBuffer __RPC_FAR * This,
            /* [in] */ float fval);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Double )( 
            IMarshallBuffer __RPC_FAR * This,
            /* [in] */ double dval);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_UShort )( 
            IMarshallBuffer __RPC_FAR * This,
            /* [in] */ unsigned short usval);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Short )( 
            IMarshallBuffer __RPC_FAR * This,
            /* [in] */ short sval);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_UInt )( 
            IMarshallBuffer __RPC_FAR * This,
            /* [in] */ unsigned int uival);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Int )( 
            IMarshallBuffer __RPC_FAR * This,
            /* [in] */ int ival);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ULong )( 
            IMarshallBuffer __RPC_FAR * This,
            /* [in] */ unsigned long ulval);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Long )( 
            IMarshallBuffer __RPC_FAR * This,
            /* [in] */ long lval);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_BLOB )( 
            IMarshallBuffer __RPC_FAR * This,
            /* [in] */ BSTR blobval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SeekToBegin )( 
            IMarshallBuffer __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Rewind )( 
            IMarshallBuffer __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CopyFrom )( 
            IMarshallBuffer __RPC_FAR * This,
            /* [in] */ unsigned char __RPC_FAR *pbytes,
            /* [in] */ DWORD lval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Detach )( 
            IMarshallBuffer __RPC_FAR * This,
            /* [out] */ unsigned char __RPC_FAR *__RPC_FAR *ppbytes,
            /* [out] */ DWORD __RPC_FAR *plval);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Length )( 
            IMarshallBuffer __RPC_FAR * This,
            /* [in] */ DWORD __RPC_FAR *lval);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WriteToDisk )( 
            IMarshallBuffer __RPC_FAR * This,
            /* [in] */ BSTR strFilename);
        
        END_INTERFACE
    } IMarshallBufferVtbl;

    interface IMarshallBuffer
    {
        CONST_VTBL struct IMarshallBufferVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMarshallBuffer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMarshallBuffer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMarshallBuffer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMarshallBuffer_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMarshallBuffer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMarshallBuffer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMarshallBuffer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMarshallBuffer_put_ClassID(This,clsid)	\
    (This)->lpVtbl -> put_ClassID(This,clsid)

#define IMarshallBuffer_put_Byte(This,bval)	\
    (This)->lpVtbl -> put_Byte(This,bval)

#define IMarshallBuffer_put_Bytes(This,pbytes,lval)	\
    (This)->lpVtbl -> put_Bytes(This,pbytes,lval)

#define IMarshallBuffer_put_Boolean(This,boolval)	\
    (This)->lpVtbl -> put_Boolean(This,boolval)

#define IMarshallBuffer_put_String(This,bstr,bCompress)	\
    (This)->lpVtbl -> put_String(This,bstr,bCompress)

#define IMarshallBuffer_put_Float(This,fval)	\
    (This)->lpVtbl -> put_Float(This,fval)

#define IMarshallBuffer_put_Double(This,dval)	\
    (This)->lpVtbl -> put_Double(This,dval)

#define IMarshallBuffer_put_UShort(This,usval)	\
    (This)->lpVtbl -> put_UShort(This,usval)

#define IMarshallBuffer_put_Short(This,sval)	\
    (This)->lpVtbl -> put_Short(This,sval)

#define IMarshallBuffer_put_UInt(This,uival)	\
    (This)->lpVtbl -> put_UInt(This,uival)

#define IMarshallBuffer_put_Int(This,ival)	\
    (This)->lpVtbl -> put_Int(This,ival)

#define IMarshallBuffer_put_ULong(This,ulval)	\
    (This)->lpVtbl -> put_ULong(This,ulval)

#define IMarshallBuffer_put_Long(This,lval)	\
    (This)->lpVtbl -> put_Long(This,lval)

#define IMarshallBuffer_put_BLOB(This,blobval)	\
    (This)->lpVtbl -> put_BLOB(This,blobval)

#define IMarshallBuffer_SeekToBegin(This)	\
    (This)->lpVtbl -> SeekToBegin(This)

#define IMarshallBuffer_Rewind(This)	\
    (This)->lpVtbl -> Rewind(This)

#define IMarshallBuffer_CopyFrom(This,pbytes,lval)	\
    (This)->lpVtbl -> CopyFrom(This,pbytes,lval)

#define IMarshallBuffer_Detach(This,ppbytes,plval)	\
    (This)->lpVtbl -> Detach(This,ppbytes,plval)

#define IMarshallBuffer_get_Length(This,lval)	\
    (This)->lpVtbl -> get_Length(This,lval)

#define IMarshallBuffer_WriteToDisk(This,strFilename)	\
    (This)->lpVtbl -> WriteToDisk(This,strFilename)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IMarshallBuffer_put_ClassID_Proxy( 
    IMarshallBuffer __RPC_FAR * This,
    /* [in] */ REFCLSID clsid);


void __RPC_STUB IMarshallBuffer_put_ClassID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IMarshallBuffer_put_Byte_Proxy( 
    IMarshallBuffer __RPC_FAR * This,
    /* [in] */ unsigned char bval);


void __RPC_STUB IMarshallBuffer_put_Byte_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IMarshallBuffer_put_Bytes_Proxy( 
    IMarshallBuffer __RPC_FAR * This,
    /* [in] */ unsigned char __RPC_FAR *pbytes,
    /* [in] */ long lval);


void __RPC_STUB IMarshallBuffer_put_Bytes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IMarshallBuffer_put_Boolean_Proxy( 
    IMarshallBuffer __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL boolval);


void __RPC_STUB IMarshallBuffer_put_Boolean_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IMarshallBuffer_put_String_Proxy( 
    IMarshallBuffer __RPC_FAR * This,
    /* [in] */ BSTR bstr,
    /* [defaultvalue][optional][in] */ VARIANT_BOOL bCompress);


void __RPC_STUB IMarshallBuffer_put_String_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IMarshallBuffer_put_Float_Proxy( 
    IMarshallBuffer __RPC_FAR * This,
    /* [in] */ float fval);


void __RPC_STUB IMarshallBuffer_put_Float_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IMarshallBuffer_put_Double_Proxy( 
    IMarshallBuffer __RPC_FAR * This,
    /* [in] */ double dval);


void __RPC_STUB IMarshallBuffer_put_Double_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IMarshallBuffer_put_UShort_Proxy( 
    IMarshallBuffer __RPC_FAR * This,
    /* [in] */ unsigned short usval);


void __RPC_STUB IMarshallBuffer_put_UShort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IMarshallBuffer_put_Short_Proxy( 
    IMarshallBuffer __RPC_FAR * This,
    /* [in] */ short sval);


void __RPC_STUB IMarshallBuffer_put_Short_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IMarshallBuffer_put_UInt_Proxy( 
    IMarshallBuffer __RPC_FAR * This,
    /* [in] */ unsigned int uival);


void __RPC_STUB IMarshallBuffer_put_UInt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IMarshallBuffer_put_Int_Proxy( 
    IMarshallBuffer __RPC_FAR * This,
    /* [in] */ int ival);


void __RPC_STUB IMarshallBuffer_put_Int_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IMarshallBuffer_put_ULong_Proxy( 
    IMarshallBuffer __RPC_FAR * This,
    /* [in] */ unsigned long ulval);


void __RPC_STUB IMarshallBuffer_put_ULong_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IMarshallBuffer_put_Long_Proxy( 
    IMarshallBuffer __RPC_FAR * This,
    /* [in] */ long lval);


void __RPC_STUB IMarshallBuffer_put_Long_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IMarshallBuffer_put_BLOB_Proxy( 
    IMarshallBuffer __RPC_FAR * This,
    /* [in] */ BSTR blobval);


void __RPC_STUB IMarshallBuffer_put_BLOB_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMarshallBuffer_SeekToBegin_Proxy( 
    IMarshallBuffer __RPC_FAR * This);


void __RPC_STUB IMarshallBuffer_SeekToBegin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMarshallBuffer_Rewind_Proxy( 
    IMarshallBuffer __RPC_FAR * This);


void __RPC_STUB IMarshallBuffer_Rewind_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMarshallBuffer_CopyFrom_Proxy( 
    IMarshallBuffer __RPC_FAR * This,
    /* [in] */ unsigned char __RPC_FAR *pbytes,
    /* [in] */ DWORD lval);


void __RPC_STUB IMarshallBuffer_CopyFrom_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMarshallBuffer_Detach_Proxy( 
    IMarshallBuffer __RPC_FAR * This,
    /* [out] */ unsigned char __RPC_FAR *__RPC_FAR *ppbytes,
    /* [out] */ DWORD __RPC_FAR *plval);


void __RPC_STUB IMarshallBuffer_Detach_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IMarshallBuffer_get_Length_Proxy( 
    IMarshallBuffer __RPC_FAR * This,
    /* [in] */ DWORD __RPC_FAR *lval);


void __RPC_STUB IMarshallBuffer_get_Length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMarshallBuffer_WriteToDisk_Proxy( 
    IMarshallBuffer __RPC_FAR * This,
    /* [in] */ BSTR strFilename);


void __RPC_STUB IMarshallBuffer_WriteToDisk_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMarshallBuffer_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
