/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:49 2000
 */
/* Compiler settings for vwhtml.idl:
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

#ifndef __vwhtml_h__
#define __vwhtml_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IVWHtml_FWD_DEFINED__
#define __IVWHtml_FWD_DEFINED__
typedef interface IVWHtml IVWHtml;
#endif 	/* __IVWHtml_FWD_DEFINED__ */


#ifndef __IVWHtmlHelper_FWD_DEFINED__
#define __IVWHtmlHelper_FWD_DEFINED__
typedef interface IVWHtmlHelper IVWHtmlHelper;
#endif 	/* __IVWHtmlHelper_FWD_DEFINED__ */


/* header files for imported files */
#include "vwclient.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IVWHtml_INTERFACE_DEFINED__
#define __IVWHtml_INTERFACE_DEFINED__

/* interface IVWHtml */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IVWHtml;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7E3832C9-4F4E-11D0-89AB-00A0C9054129")
    IVWHtml : public IDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ IVWClient __RPC_FAR *pclient) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_RootURL( 
            /* [retval][out] */ BSTR __RPC_FAR *bstrRootURL) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_RootURL( 
            /* [in] */ BSTR bstrRootURL) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CloseAll( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Close( 
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ShowHTML( 
            /* [in] */ int nhwnd,
            /* [in] */ BSTR bstrHtml,
            /* [in] */ BSTR bstrTitle,
            /* [in] */ VARIANT_BOOL bSafeMode,
            /* [in] */ IDispatch __RPC_FAR *pdispArg,
            /* [in] */ int x,
            /* [in] */ int y,
            /* [in] */ int cx,
            /* [in] */ int cy) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ShowHTMLEx( 
            /* [in] */ BSTR bstrName,
            /* [in] */ int nhwnd,
            /* [in] */ BSTR bstrHtml,
            /* [in] */ BSTR bstrTitle,
            /* [in] */ VARIANT_BOOL bSafeMode,
            /* [in] */ IDispatch __RPC_FAR *pdispArg,
            /* [in] */ int x,
            /* [in] */ int y,
            /* [in] */ int cx,
            /* [in] */ int cy) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ShowURL( 
            /* [in] */ int nhwnd,
            /* [in] */ BSTR bstrURL,
            /* [in] */ BSTR bstrTitle,
            /* [in] */ VARIANT_BOOL bSafeMode,
            /* [in] */ IDispatch __RPC_FAR *pdispArg,
            /* [in] */ int x,
            /* [in] */ int y,
            /* [in] */ int cx,
            /* [in] */ int cy) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ShowURLEx( 
            /* [in] */ BSTR bstrName,
            /* [in] */ int nhwnd,
            /* [in] */ BSTR bstrURL,
            /* [in] */ BSTR bstrTitle,
            /* [in] */ VARIANT_BOOL bSafeMode,
            /* [in] */ IDispatch __RPC_FAR *pdispArg,
            /* [in] */ int x,
            /* [in] */ int y,
            /* [in] */ int cx,
            /* [in] */ int cy) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVWHtmlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IVWHtml __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IVWHtml __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IVWHtml __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IVWHtml __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IVWHtml __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IVWHtml __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IVWHtml __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            IVWHtml __RPC_FAR * This,
            /* [in] */ IVWClient __RPC_FAR *pclient);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_RootURL )( 
            IVWHtml __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *bstrRootURL);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_RootURL )( 
            IVWHtml __RPC_FAR * This,
            /* [in] */ BSTR bstrRootURL);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CloseAll )( 
            IVWHtml __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Close )( 
            IVWHtml __RPC_FAR * This,
            /* [in] */ BSTR bstrName);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ShowHTML )( 
            IVWHtml __RPC_FAR * This,
            /* [in] */ int nhwnd,
            /* [in] */ BSTR bstrHtml,
            /* [in] */ BSTR bstrTitle,
            /* [in] */ VARIANT_BOOL bSafeMode,
            /* [in] */ IDispatch __RPC_FAR *pdispArg,
            /* [in] */ int x,
            /* [in] */ int y,
            /* [in] */ int cx,
            /* [in] */ int cy);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ShowHTMLEx )( 
            IVWHtml __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ int nhwnd,
            /* [in] */ BSTR bstrHtml,
            /* [in] */ BSTR bstrTitle,
            /* [in] */ VARIANT_BOOL bSafeMode,
            /* [in] */ IDispatch __RPC_FAR *pdispArg,
            /* [in] */ int x,
            /* [in] */ int y,
            /* [in] */ int cx,
            /* [in] */ int cy);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ShowURL )( 
            IVWHtml __RPC_FAR * This,
            /* [in] */ int nhwnd,
            /* [in] */ BSTR bstrURL,
            /* [in] */ BSTR bstrTitle,
            /* [in] */ VARIANT_BOOL bSafeMode,
            /* [in] */ IDispatch __RPC_FAR *pdispArg,
            /* [in] */ int x,
            /* [in] */ int y,
            /* [in] */ int cx,
            /* [in] */ int cy);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ShowURLEx )( 
            IVWHtml __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ int nhwnd,
            /* [in] */ BSTR bstrURL,
            /* [in] */ BSTR bstrTitle,
            /* [in] */ VARIANT_BOOL bSafeMode,
            /* [in] */ IDispatch __RPC_FAR *pdispArg,
            /* [in] */ int x,
            /* [in] */ int y,
            /* [in] */ int cx,
            /* [in] */ int cy);
        
        END_INTERFACE
    } IVWHtmlVtbl;

    interface IVWHtml
    {
        CONST_VTBL struct IVWHtmlVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVWHtml_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IVWHtml_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IVWHtml_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IVWHtml_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IVWHtml_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IVWHtml_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IVWHtml_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IVWHtml_Initialize(This,pclient)	\
    (This)->lpVtbl -> Initialize(This,pclient)

#define IVWHtml_get_RootURL(This,bstrRootURL)	\
    (This)->lpVtbl -> get_RootURL(This,bstrRootURL)

#define IVWHtml_put_RootURL(This,bstrRootURL)	\
    (This)->lpVtbl -> put_RootURL(This,bstrRootURL)

#define IVWHtml_CloseAll(This)	\
    (This)->lpVtbl -> CloseAll(This)

#define IVWHtml_Close(This,bstrName)	\
    (This)->lpVtbl -> Close(This,bstrName)

#define IVWHtml_ShowHTML(This,nhwnd,bstrHtml,bstrTitle,bSafeMode,pdispArg,x,y,cx,cy)	\
    (This)->lpVtbl -> ShowHTML(This,nhwnd,bstrHtml,bstrTitle,bSafeMode,pdispArg,x,y,cx,cy)

#define IVWHtml_ShowHTMLEx(This,bstrName,nhwnd,bstrHtml,bstrTitle,bSafeMode,pdispArg,x,y,cx,cy)	\
    (This)->lpVtbl -> ShowHTMLEx(This,bstrName,nhwnd,bstrHtml,bstrTitle,bSafeMode,pdispArg,x,y,cx,cy)

#define IVWHtml_ShowURL(This,nhwnd,bstrURL,bstrTitle,bSafeMode,pdispArg,x,y,cx,cy)	\
    (This)->lpVtbl -> ShowURL(This,nhwnd,bstrURL,bstrTitle,bSafeMode,pdispArg,x,y,cx,cy)

#define IVWHtml_ShowURLEx(This,bstrName,nhwnd,bstrURL,bstrTitle,bSafeMode,pdispArg,x,y,cx,cy)	\
    (This)->lpVtbl -> ShowURLEx(This,bstrName,nhwnd,bstrURL,bstrTitle,bSafeMode,pdispArg,x,y,cx,cy)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWHtml_Initialize_Proxy( 
    IVWHtml __RPC_FAR * This,
    /* [in] */ IVWClient __RPC_FAR *pclient);


void __RPC_STUB IVWHtml_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWHtml_get_RootURL_Proxy( 
    IVWHtml __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *bstrRootURL);


void __RPC_STUB IVWHtml_get_RootURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE IVWHtml_put_RootURL_Proxy( 
    IVWHtml __RPC_FAR * This,
    /* [in] */ BSTR bstrRootURL);


void __RPC_STUB IVWHtml_put_RootURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWHtml_CloseAll_Proxy( 
    IVWHtml __RPC_FAR * This);


void __RPC_STUB IVWHtml_CloseAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWHtml_Close_Proxy( 
    IVWHtml __RPC_FAR * This,
    /* [in] */ BSTR bstrName);


void __RPC_STUB IVWHtml_Close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWHtml_ShowHTML_Proxy( 
    IVWHtml __RPC_FAR * This,
    /* [in] */ int nhwnd,
    /* [in] */ BSTR bstrHtml,
    /* [in] */ BSTR bstrTitle,
    /* [in] */ VARIANT_BOOL bSafeMode,
    /* [in] */ IDispatch __RPC_FAR *pdispArg,
    /* [in] */ int x,
    /* [in] */ int y,
    /* [in] */ int cx,
    /* [in] */ int cy);


void __RPC_STUB IVWHtml_ShowHTML_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWHtml_ShowHTMLEx_Proxy( 
    IVWHtml __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ int nhwnd,
    /* [in] */ BSTR bstrHtml,
    /* [in] */ BSTR bstrTitle,
    /* [in] */ VARIANT_BOOL bSafeMode,
    /* [in] */ IDispatch __RPC_FAR *pdispArg,
    /* [in] */ int x,
    /* [in] */ int y,
    /* [in] */ int cx,
    /* [in] */ int cy);


void __RPC_STUB IVWHtml_ShowHTMLEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWHtml_ShowURL_Proxy( 
    IVWHtml __RPC_FAR * This,
    /* [in] */ int nhwnd,
    /* [in] */ BSTR bstrURL,
    /* [in] */ BSTR bstrTitle,
    /* [in] */ VARIANT_BOOL bSafeMode,
    /* [in] */ IDispatch __RPC_FAR *pdispArg,
    /* [in] */ int x,
    /* [in] */ int y,
    /* [in] */ int cx,
    /* [in] */ int cy);


void __RPC_STUB IVWHtml_ShowURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWHtml_ShowURLEx_Proxy( 
    IVWHtml __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ int nhwnd,
    /* [in] */ BSTR bstrURL,
    /* [in] */ BSTR bstrTitle,
    /* [in] */ VARIANT_BOOL bSafeMode,
    /* [in] */ IDispatch __RPC_FAR *pdispArg,
    /* [in] */ int x,
    /* [in] */ int y,
    /* [in] */ int cx,
    /* [in] */ int cy);


void __RPC_STUB IVWHtml_ShowURLEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IVWHtml_INTERFACE_DEFINED__ */


#ifndef __IVWHtmlHelper_INTERFACE_DEFINED__
#define __IVWHtmlHelper_INTERFACE_DEFINED__

/* interface IVWHtmlHelper */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IVWHtmlHelper;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7E3832CB-4F4E-11D0-89AB-00A0C9054129")
    IVWHtmlHelper : public IDispatch
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Client( 
            /* [retval][out] */ IVWClient __RPC_FAR *__RPC_FAR *ppclient) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Arg( 
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispArg) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CloseWindow( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetShortPathName( 
            /* [in] */ BSTR bstrLongName,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrShortName) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVWHtmlHelperVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IVWHtmlHelper __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IVWHtmlHelper __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IVWHtmlHelper __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IVWHtmlHelper __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IVWHtmlHelper __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IVWHtmlHelper __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IVWHtmlHelper __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Client )( 
            IVWHtmlHelper __RPC_FAR * This,
            /* [retval][out] */ IVWClient __RPC_FAR *__RPC_FAR *ppclient);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Arg )( 
            IVWHtmlHelper __RPC_FAR * This,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispArg);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CloseWindow )( 
            IVWHtmlHelper __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetShortPathName )( 
            IVWHtmlHelper __RPC_FAR * This,
            /* [in] */ BSTR bstrLongName,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrShortName);
        
        END_INTERFACE
    } IVWHtmlHelperVtbl;

    interface IVWHtmlHelper
    {
        CONST_VTBL struct IVWHtmlHelperVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVWHtmlHelper_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IVWHtmlHelper_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IVWHtmlHelper_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IVWHtmlHelper_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IVWHtmlHelper_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IVWHtmlHelper_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IVWHtmlHelper_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IVWHtmlHelper_get_Client(This,ppclient)	\
    (This)->lpVtbl -> get_Client(This,ppclient)

#define IVWHtmlHelper_get_Arg(This,ppdispArg)	\
    (This)->lpVtbl -> get_Arg(This,ppdispArg)

#define IVWHtmlHelper_CloseWindow(This)	\
    (This)->lpVtbl -> CloseWindow(This)

#define IVWHtmlHelper_GetShortPathName(This,bstrLongName,pbstrShortName)	\
    (This)->lpVtbl -> GetShortPathName(This,bstrLongName,pbstrShortName)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWHtmlHelper_get_Client_Proxy( 
    IVWHtmlHelper __RPC_FAR * This,
    /* [retval][out] */ IVWClient __RPC_FAR *__RPC_FAR *ppclient);


void __RPC_STUB IVWHtmlHelper_get_Client_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWHtmlHelper_get_Arg_Proxy( 
    IVWHtmlHelper __RPC_FAR * This,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppdispArg);


void __RPC_STUB IVWHtmlHelper_get_Arg_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWHtmlHelper_CloseWindow_Proxy( 
    IVWHtmlHelper __RPC_FAR * This);


void __RPC_STUB IVWHtmlHelper_CloseWindow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWHtmlHelper_GetShortPathName_Proxy( 
    IVWHtmlHelper __RPC_FAR * This,
    /* [in] */ BSTR bstrLongName,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrShortName);


void __RPC_STUB IVWHtmlHelper_GetShortPathName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IVWHtmlHelper_INTERFACE_DEFINED__ */


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
