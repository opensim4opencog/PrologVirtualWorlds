/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:43 2000
 */
/* Compiler settings for vwodb.idl:
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

#ifndef __vwodb_h__
#define __vwodb_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IVWObjectRepository_FWD_DEFINED__
#define __IVWObjectRepository_FWD_DEFINED__
typedef interface IVWObjectRepository IVWObjectRepository;
#endif 	/* __IVWObjectRepository_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "vwobject.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_vwodb_0000 */
/* [local] */ 

#define VWODB_E_ALREADYOPEN			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0601)
#define VWODB_E_DBCREATEERROR		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0602)
#define VWODB_E_DBNOTEXIST			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0603)
#define VWODB_E_DBREADERROR			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0604)
#define VWODB_E_DBOPENERROR			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0605)
#define VWODB_E_INVALIDDBSIGNATURE	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0606)
#define VWODB_E_OBJECTNOTEXIST		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0607)
#define VWODB_E_INVALIDDBRECORD		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0608)
#define VWODB_E_NOTPERSISTMODE		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0609)
#define VWODB_E_BACKUPFAILED			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x060A)
#define VWODB_E_LOGRECORDTOOLARGE	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x060B)
#define VWODB_E_NOTOPEN				MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x060C)
#define VWODB_E_INVALIDDBNAME		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x060D)
#define VWODB_E_DBALREADYEXIST		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x060E)


extern RPC_IF_HANDLE __MIDL_itf_vwodb_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_vwodb_0000_v0_0_s_ifspec;

#ifndef __IVWObjectRepository_INTERFACE_DEFINED__
#define __IVWObjectRepository_INTERFACE_DEFINED__

/* interface IVWObjectRepository */
/* [unique][local][dual][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IVWObjectRepository;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8D20B9CA-E963-11CF-A516-00AA00B595DB")
    IVWObjectRepository : public IDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Initialize( 
            /* [in] */ IWorld __RPC_FAR *pWorld,
            /* [in] */ BSTR bstrConnection) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OpenWorld( 
            /* [in] */ VARIANT_BOOL bCreate,
            /* [in] */ VARIANT_BOOL bOverwrite) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CloseWorld( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateObject( 
            /* [in] */ IThing __RPC_FAR *pThing) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DeleteObject( 
            /* [in] */ int id) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetObject( 
            /* [in] */ int id,
            /* [out] */ IThing __RPC_FAR *__RPC_FAR *ppThing) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetObjectByName( 
            /* [in] */ BSTR bstrName,
            /* [out] */ IThing __RPC_FAR *__RPC_FAR *ppThing) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE PutObject( 
            /* [in] */ IThing __RPC_FAR *pThing) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE WriteLogRecord( 
            /* [in] */ int id,
            /* [in] */ IMarshallBuffer __RPC_FAR *pbuffer) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Commit( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CommitAs( 
            BSTR bstrFileName) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RenameAs( 
            BSTR bstrFileName) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Open( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Persist( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DumpObjects( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVWObjectRepositoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IVWObjectRepository __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IVWObjectRepository __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IVWObjectRepository __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IVWObjectRepository __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IVWObjectRepository __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IVWObjectRepository __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IVWObjectRepository __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            IVWObjectRepository __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pWorld,
            /* [in] */ BSTR bstrConnection);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OpenWorld )( 
            IVWObjectRepository __RPC_FAR * This,
            /* [in] */ VARIANT_BOOL bCreate,
            /* [in] */ VARIANT_BOOL bOverwrite);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CloseWorld )( 
            IVWObjectRepository __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateObject )( 
            IVWObjectRepository __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteObject )( 
            IVWObjectRepository __RPC_FAR * This,
            /* [in] */ int id);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetObject )( 
            IVWObjectRepository __RPC_FAR * This,
            /* [in] */ int id,
            /* [out] */ IThing __RPC_FAR *__RPC_FAR *ppThing);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetObjectByName )( 
            IVWObjectRepository __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [out] */ IThing __RPC_FAR *__RPC_FAR *ppThing);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PutObject )( 
            IVWObjectRepository __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WriteLogRecord )( 
            IVWObjectRepository __RPC_FAR * This,
            /* [in] */ int id,
            /* [in] */ IMarshallBuffer __RPC_FAR *pbuffer);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Commit )( 
            IVWObjectRepository __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CommitAs )( 
            IVWObjectRepository __RPC_FAR * This,
            BSTR bstrFileName);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RenameAs )( 
            IVWObjectRepository __RPC_FAR * This,
            BSTR bstrFileName);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Open )( 
            IVWObjectRepository __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Persist )( 
            IVWObjectRepository __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DumpObjects )( 
            IVWObjectRepository __RPC_FAR * This);
        
        END_INTERFACE
    } IVWObjectRepositoryVtbl;

    interface IVWObjectRepository
    {
        CONST_VTBL struct IVWObjectRepositoryVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVWObjectRepository_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IVWObjectRepository_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IVWObjectRepository_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IVWObjectRepository_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IVWObjectRepository_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IVWObjectRepository_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IVWObjectRepository_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IVWObjectRepository_Initialize(This,pWorld,bstrConnection)	\
    (This)->lpVtbl -> Initialize(This,pWorld,bstrConnection)

#define IVWObjectRepository_OpenWorld(This,bCreate,bOverwrite)	\
    (This)->lpVtbl -> OpenWorld(This,bCreate,bOverwrite)

#define IVWObjectRepository_CloseWorld(This)	\
    (This)->lpVtbl -> CloseWorld(This)

#define IVWObjectRepository_CreateObject(This,pThing)	\
    (This)->lpVtbl -> CreateObject(This,pThing)

#define IVWObjectRepository_DeleteObject(This,id)	\
    (This)->lpVtbl -> DeleteObject(This,id)

#define IVWObjectRepository_GetObject(This,id,ppThing)	\
    (This)->lpVtbl -> GetObject(This,id,ppThing)

#define IVWObjectRepository_GetObjectByName(This,bstrName,ppThing)	\
    (This)->lpVtbl -> GetObjectByName(This,bstrName,ppThing)

#define IVWObjectRepository_PutObject(This,pThing)	\
    (This)->lpVtbl -> PutObject(This,pThing)

#define IVWObjectRepository_WriteLogRecord(This,id,pbuffer)	\
    (This)->lpVtbl -> WriteLogRecord(This,id,pbuffer)

#define IVWObjectRepository_Commit(This)	\
    (This)->lpVtbl -> Commit(This)

#define IVWObjectRepository_CommitAs(This,bstrFileName)	\
    (This)->lpVtbl -> CommitAs(This,bstrFileName)

#define IVWObjectRepository_RenameAs(This,bstrFileName)	\
    (This)->lpVtbl -> RenameAs(This,bstrFileName)

#define IVWObjectRepository_get_Open(This,pboolVal)	\
    (This)->lpVtbl -> get_Open(This,pboolVal)

#define IVWObjectRepository_get_Persist(This,pboolVal)	\
    (This)->lpVtbl -> get_Persist(This,pboolVal)

#define IVWObjectRepository_DumpObjects(This)	\
    (This)->lpVtbl -> DumpObjects(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWObjectRepository_Initialize_Proxy( 
    IVWObjectRepository __RPC_FAR * This,
    /* [in] */ IWorld __RPC_FAR *pWorld,
    /* [in] */ BSTR bstrConnection);


void __RPC_STUB IVWObjectRepository_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWObjectRepository_OpenWorld_Proxy( 
    IVWObjectRepository __RPC_FAR * This,
    /* [in] */ VARIANT_BOOL bCreate,
    /* [in] */ VARIANT_BOOL bOverwrite);


void __RPC_STUB IVWObjectRepository_OpenWorld_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWObjectRepository_CloseWorld_Proxy( 
    IVWObjectRepository __RPC_FAR * This);


void __RPC_STUB IVWObjectRepository_CloseWorld_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWObjectRepository_CreateObject_Proxy( 
    IVWObjectRepository __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing);


void __RPC_STUB IVWObjectRepository_CreateObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWObjectRepository_DeleteObject_Proxy( 
    IVWObjectRepository __RPC_FAR * This,
    /* [in] */ int id);


void __RPC_STUB IVWObjectRepository_DeleteObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWObjectRepository_GetObject_Proxy( 
    IVWObjectRepository __RPC_FAR * This,
    /* [in] */ int id,
    /* [out] */ IThing __RPC_FAR *__RPC_FAR *ppThing);


void __RPC_STUB IVWObjectRepository_GetObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWObjectRepository_GetObjectByName_Proxy( 
    IVWObjectRepository __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [out] */ IThing __RPC_FAR *__RPC_FAR *ppThing);


void __RPC_STUB IVWObjectRepository_GetObjectByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWObjectRepository_PutObject_Proxy( 
    IVWObjectRepository __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing);


void __RPC_STUB IVWObjectRepository_PutObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWObjectRepository_WriteLogRecord_Proxy( 
    IVWObjectRepository __RPC_FAR * This,
    /* [in] */ int id,
    /* [in] */ IMarshallBuffer __RPC_FAR *pbuffer);


void __RPC_STUB IVWObjectRepository_WriteLogRecord_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWObjectRepository_Commit_Proxy( 
    IVWObjectRepository __RPC_FAR * This);


void __RPC_STUB IVWObjectRepository_Commit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWObjectRepository_CommitAs_Proxy( 
    IVWObjectRepository __RPC_FAR * This,
    BSTR bstrFileName);


void __RPC_STUB IVWObjectRepository_CommitAs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWObjectRepository_RenameAs_Proxy( 
    IVWObjectRepository __RPC_FAR * This,
    BSTR bstrFileName);


void __RPC_STUB IVWObjectRepository_RenameAs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWObjectRepository_get_Open_Proxy( 
    IVWObjectRepository __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);


void __RPC_STUB IVWObjectRepository_get_Open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWObjectRepository_get_Persist_Proxy( 
    IVWObjectRepository __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolVal);


void __RPC_STUB IVWObjectRepository_get_Persist_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWObjectRepository_DumpObjects_Proxy( 
    IVWObjectRepository __RPC_FAR * This);


void __RPC_STUB IVWObjectRepository_DumpObjects_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IVWObjectRepository_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
