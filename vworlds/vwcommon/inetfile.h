/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:39 2000
 */
/* Compiler settings for inetfile.idl:
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

#ifndef __inetfile_h__
#define __inetfile_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IDLThread_FWD_DEFINED__
#define __IDLThread_FWD_DEFINED__
typedef interface IDLThread IDLThread;
#endif 	/* __IDLThread_FWD_DEFINED__ */


#ifndef __IInternetFileManager_FWD_DEFINED__
#define __IInternetFileManager_FWD_DEFINED__
typedef interface IInternetFileManager IInternetFileManager;
#endif 	/* __IInternetFileManager_FWD_DEFINED__ */


#ifndef __IIFMCallback_FWD_DEFINED__
#define __IIFMCallback_FWD_DEFINED__
typedef interface IIFMCallback IIFMCallback;
#endif 	/* __IIFMCallback_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_inetfile_0000 */
/* [local] */ 

#define INETFILE_E_FILENOTFOUND			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0d01)
#define INETFILE_E_INVALIDROOTURL		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0d02)




extern RPC_IF_HANDLE __MIDL_itf_inetfile_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_inetfile_0000_v0_0_s_ifspec;

#ifndef __IDLThread_INTERFACE_DEFINED__
#define __IDLThread_INTERFACE_DEFINED__

/* interface IDLThread */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IDLThread;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5E56BA8F-6B52-11D1-AFC7-00C04FB915AA")
    IDLThread : public IDispatch
    {
    public:
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE Download( 
            /* [in] */ BSTR filename,
            /* [in] */ BSTR RootURL) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE AddCallback( 
            /* [in] */ IInternetFileManager __RPC_FAR *pInetFile,
            /* [in] */ IIFMCallback __RPC_FAR *pCallback,
            /* [in] */ long lCookie) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE Terminate( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDLThreadVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDLThread __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDLThread __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDLThread __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IDLThread __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IDLThread __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IDLThread __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IDLThread __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Download )( 
            IDLThread __RPC_FAR * This,
            /* [in] */ BSTR filename,
            /* [in] */ BSTR RootURL);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddCallback )( 
            IDLThread __RPC_FAR * This,
            /* [in] */ IInternetFileManager __RPC_FAR *pInetFile,
            /* [in] */ IIFMCallback __RPC_FAR *pCallback,
            /* [in] */ long lCookie);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IDLThread __RPC_FAR * This);
        
        END_INTERFACE
    } IDLThreadVtbl;

    interface IDLThread
    {
        CONST_VTBL struct IDLThreadVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDLThread_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDLThread_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDLThread_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDLThread_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDLThread_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDLThread_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDLThread_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDLThread_Download(This,filename,RootURL)	\
    (This)->lpVtbl -> Download(This,filename,RootURL)

#define IDLThread_AddCallback(This,pInetFile,pCallback,lCookie)	\
    (This)->lpVtbl -> AddCallback(This,pInetFile,pCallback,lCookie)

#define IDLThread_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IDLThread_Download_Proxy( 
    IDLThread __RPC_FAR * This,
    /* [in] */ BSTR filename,
    /* [in] */ BSTR RootURL);


void __RPC_STUB IDLThread_Download_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IDLThread_AddCallback_Proxy( 
    IDLThread __RPC_FAR * This,
    /* [in] */ IInternetFileManager __RPC_FAR *pInetFile,
    /* [in] */ IIFMCallback __RPC_FAR *pCallback,
    /* [in] */ long lCookie);


void __RPC_STUB IDLThread_AddCallback_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IDLThread_Terminate_Proxy( 
    IDLThread __RPC_FAR * This);


void __RPC_STUB IDLThread_Terminate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDLThread_INTERFACE_DEFINED__ */


#ifndef __IInternetFileManager_INTERFACE_DEFINED__
#define __IInternetFileManager_INTERFACE_DEFINED__

/* interface IInternetFileManager */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IInternetFileManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("120113DC-E936-11CF-87BD-00AA00B4DF93")
    IInternetFileManager : public IDispatch
    {
    public:
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE Terminate( void) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_RootURL( 
            /* [retval][out] */ BSTR __RPC_FAR *bstrRootURL) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_RootURL( 
            /* [in] */ BSTR bstrRootURL) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE GetFileEx( 
            /* [in] */ BSTR bstrURL,
            /* [in] */ VARIANT_BOOL bCheckCache,
            /* [out] */ BSTR __RPC_FAR *pbstrFinalURL,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrFinalPath) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetFile( 
            /* [in] */ BSTR bstrURL,
            /* [in] */ VARIANT_BOOL bCheckCache,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrFinalPath) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE GetFileAsync( 
            /* [in] */ BSTR bstrURL,
            /* [in] */ long lCookie,
            /* [in] */ IIFMCallback __RPC_FAR *pIFMCallback,
            VARIANT_BOOL bCheckCache) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CancelAllDownloads( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE FindCachedFile( 
            /* [in] */ BSTR bstrURL,
            /* [out] */ BSTR __RPC_FAR *pbstrFinalURL,
            /* [out] */ BSTR __RPC_FAR *pbstrFinalPath,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE FileDLDone( 
            /* [in] */ BSTR bstrFilename) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE Test( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IInternetFileManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IInternetFileManager __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IInternetFileManager __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IInternetFileManager __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IInternetFileManager __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IInternetFileManager __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IInternetFileManager __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IInternetFileManager __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IInternetFileManager __RPC_FAR * This);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_RootURL )( 
            IInternetFileManager __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *bstrRootURL);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_RootURL )( 
            IInternetFileManager __RPC_FAR * This,
            /* [in] */ BSTR bstrRootURL);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFileEx )( 
            IInternetFileManager __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in] */ VARIANT_BOOL bCheckCache,
            /* [out] */ BSTR __RPC_FAR *pbstrFinalURL,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrFinalPath);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFile )( 
            IInternetFileManager __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in] */ VARIANT_BOOL bCheckCache,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrFinalPath);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetFileAsync )( 
            IInternetFileManager __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [in] */ long lCookie,
            /* [in] */ IIFMCallback __RPC_FAR *pIFMCallback,
            VARIANT_BOOL bCheckCache);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CancelAllDownloads )( 
            IInternetFileManager __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FindCachedFile )( 
            IInternetFileManager __RPC_FAR * This,
            /* [in] */ BSTR bstrURL,
            /* [out] */ BSTR __RPC_FAR *pbstrFinalURL,
            /* [out] */ BSTR __RPC_FAR *pbstrFinalPath,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FileDLDone )( 
            IInternetFileManager __RPC_FAR * This,
            /* [in] */ BSTR bstrFilename);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Test )( 
            IInternetFileManager __RPC_FAR * This);
        
        END_INTERFACE
    } IInternetFileManagerVtbl;

    interface IInternetFileManager
    {
        CONST_VTBL struct IInternetFileManagerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IInternetFileManager_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IInternetFileManager_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IInternetFileManager_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IInternetFileManager_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IInternetFileManager_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IInternetFileManager_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IInternetFileManager_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IInternetFileManager_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IInternetFileManager_get_RootURL(This,bstrRootURL)	\
    (This)->lpVtbl -> get_RootURL(This,bstrRootURL)

#define IInternetFileManager_put_RootURL(This,bstrRootURL)	\
    (This)->lpVtbl -> put_RootURL(This,bstrRootURL)

#define IInternetFileManager_GetFileEx(This,bstrURL,bCheckCache,pbstrFinalURL,pbstrFinalPath)	\
    (This)->lpVtbl -> GetFileEx(This,bstrURL,bCheckCache,pbstrFinalURL,pbstrFinalPath)

#define IInternetFileManager_GetFile(This,bstrURL,bCheckCache,pbstrFinalPath)	\
    (This)->lpVtbl -> GetFile(This,bstrURL,bCheckCache,pbstrFinalPath)

#define IInternetFileManager_GetFileAsync(This,bstrURL,lCookie,pIFMCallback,bCheckCache)	\
    (This)->lpVtbl -> GetFileAsync(This,bstrURL,lCookie,pIFMCallback,bCheckCache)

#define IInternetFileManager_CancelAllDownloads(This)	\
    (This)->lpVtbl -> CancelAllDownloads(This)

#define IInternetFileManager_FindCachedFile(This,bstrURL,pbstrFinalURL,pbstrFinalPath,pbool)	\
    (This)->lpVtbl -> FindCachedFile(This,bstrURL,pbstrFinalURL,pbstrFinalPath,pbool)

#define IInternetFileManager_FileDLDone(This,bstrFilename)	\
    (This)->lpVtbl -> FileDLDone(This,bstrFilename)

#define IInternetFileManager_Test(This)	\
    (This)->lpVtbl -> Test(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IInternetFileManager_Terminate_Proxy( 
    IInternetFileManager __RPC_FAR * This);


void __RPC_STUB IInternetFileManager_Terminate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IInternetFileManager_get_RootURL_Proxy( 
    IInternetFileManager __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *bstrRootURL);


void __RPC_STUB IInternetFileManager_get_RootURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE IInternetFileManager_put_RootURL_Proxy( 
    IInternetFileManager __RPC_FAR * This,
    /* [in] */ BSTR bstrRootURL);


void __RPC_STUB IInternetFileManager_put_RootURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IInternetFileManager_GetFileEx_Proxy( 
    IInternetFileManager __RPC_FAR * This,
    /* [in] */ BSTR bstrURL,
    /* [in] */ VARIANT_BOOL bCheckCache,
    /* [out] */ BSTR __RPC_FAR *pbstrFinalURL,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrFinalPath);


void __RPC_STUB IInternetFileManager_GetFileEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IInternetFileManager_GetFile_Proxy( 
    IInternetFileManager __RPC_FAR * This,
    /* [in] */ BSTR bstrURL,
    /* [in] */ VARIANT_BOOL bCheckCache,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrFinalPath);


void __RPC_STUB IInternetFileManager_GetFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IInternetFileManager_GetFileAsync_Proxy( 
    IInternetFileManager __RPC_FAR * This,
    /* [in] */ BSTR bstrURL,
    /* [in] */ long lCookie,
    /* [in] */ IIFMCallback __RPC_FAR *pIFMCallback,
    VARIANT_BOOL bCheckCache);


void __RPC_STUB IInternetFileManager_GetFileAsync_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IInternetFileManager_CancelAllDownloads_Proxy( 
    IInternetFileManager __RPC_FAR * This);


void __RPC_STUB IInternetFileManager_CancelAllDownloads_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IInternetFileManager_FindCachedFile_Proxy( 
    IInternetFileManager __RPC_FAR * This,
    /* [in] */ BSTR bstrURL,
    /* [out] */ BSTR __RPC_FAR *pbstrFinalURL,
    /* [out] */ BSTR __RPC_FAR *pbstrFinalPath,
    /* [out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IInternetFileManager_FindCachedFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IInternetFileManager_FileDLDone_Proxy( 
    IInternetFileManager __RPC_FAR * This,
    /* [in] */ BSTR bstrFilename);


void __RPC_STUB IInternetFileManager_FileDLDone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IInternetFileManager_Test_Proxy( 
    IInternetFileManager __RPC_FAR * This);


void __RPC_STUB IInternetFileManager_Test_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IInternetFileManager_INTERFACE_DEFINED__ */


#ifndef __IIFMCallback_INTERFACE_DEFINED__
#define __IIFMCallback_INTERFACE_DEFINED__

/* interface IIFMCallback */
/* [unique][helpstring][uuid][local][object] */ 


EXTERN_C const IID IID_IIFMCallback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("120113DE-E936-11CF-87BD-00AA00B4DF93")
    IIFMCallback : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnProgress_T2( 
            /* [in] */ ULONG ulProgress,
            /* [in] */ ULONG ulProgressMax,
            /* [in] */ long lCookie) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnDone_T2( 
            /* [in] */ HRESULT hresult,
            /* [in] */ BSTR bstrFinalPath,
            /* [in] */ long lCookie,
            /* [in] */ BSTR bstrFullURL) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IIFMCallbackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IIFMCallback __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IIFMCallback __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IIFMCallback __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnProgress_T2 )( 
            IIFMCallback __RPC_FAR * This,
            /* [in] */ ULONG ulProgress,
            /* [in] */ ULONG ulProgressMax,
            /* [in] */ long lCookie);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnDone_T2 )( 
            IIFMCallback __RPC_FAR * This,
            /* [in] */ HRESULT hresult,
            /* [in] */ BSTR bstrFinalPath,
            /* [in] */ long lCookie,
            /* [in] */ BSTR bstrFullURL);
        
        END_INTERFACE
    } IIFMCallbackVtbl;

    interface IIFMCallback
    {
        CONST_VTBL struct IIFMCallbackVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IIFMCallback_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IIFMCallback_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IIFMCallback_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IIFMCallback_OnProgress_T2(This,ulProgress,ulProgressMax,lCookie)	\
    (This)->lpVtbl -> OnProgress_T2(This,ulProgress,ulProgressMax,lCookie)

#define IIFMCallback_OnDone_T2(This,hresult,bstrFinalPath,lCookie,bstrFullURL)	\
    (This)->lpVtbl -> OnDone_T2(This,hresult,bstrFinalPath,lCookie,bstrFullURL)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IIFMCallback_OnProgress_T2_Proxy( 
    IIFMCallback __RPC_FAR * This,
    /* [in] */ ULONG ulProgress,
    /* [in] */ ULONG ulProgressMax,
    /* [in] */ long lCookie);


void __RPC_STUB IIFMCallback_OnProgress_T2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IIFMCallback_OnDone_T2_Proxy( 
    IIFMCallback __RPC_FAR * This,
    /* [in] */ HRESULT hresult,
    /* [in] */ BSTR bstrFinalPath,
    /* [in] */ long lCookie,
    /* [in] */ BSTR bstrFullURL);


void __RPC_STUB IIFMCallback_OnDone_T2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IIFMCallback_INTERFACE_DEFINED__ */


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
