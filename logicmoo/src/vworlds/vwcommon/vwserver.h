/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:51 2000
 */
/* Compiler settings for vwserver.idl:
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

#ifndef __vwserver_h__
#define __vwserver_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IVWServer_FWD_DEFINED__
#define __IVWServer_FWD_DEFINED__
typedef interface IVWServer IVWServer;
#endif 	/* __IVWServer_FWD_DEFINED__ */


#ifndef __DVWServerEvents_FWD_DEFINED__
#define __DVWServerEvents_FWD_DEFINED__
typedef interface DVWServerEvents DVWServerEvents;
#endif 	/* __DVWServerEvents_FWD_DEFINED__ */


/* header files for imported files */
#include "vwobject.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_vwserver_0000 */
/* [local] */ 

#define VWSERVER_E_THREADCREATION            MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0401)
#define VWSERVER_E_WORLDISOFFLINE            MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0402)
#define VWSERVER_E_WORLDPREVIOUSLYREGISTERED	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0403)
#define VWSERVER_E_ERRORCREATINGWORLD		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0404)
#define VWSERVER_E_INVALIDWORLDNAME			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0405)
#define VWSERVER_E_AUTHENTICATIONREQUIRED	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0406)
#define VWSERVER_E_GROUPAUTHENTICATIONFAILURE	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0407)
#define VWSERVER_E_TOOMANYCONNECTIONS		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0408)


extern RPC_IF_HANDLE __MIDL_itf_vwserver_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_vwserver_0000_v0_0_s_ifspec;

#ifndef __IVWServer_INTERFACE_DEFINED__
#define __IVWServer_INTERFACE_DEFINED__

/* interface IVWServer */
/* [unique][local][dual][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IVWServer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F85848FE-00F4-11D0-A516-00AA00B595DB")
    IVWServer : public IDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Accept( 
            /* [in] */ int iPort,
            /* [in] */ int iOptionalCallbackWindowHandle) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Disconnect( void) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE DisconnectWarning( void) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE DisconnectCancel( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ShutDown( void) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE TestQueue( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetStatusInfo( 
            /* [out] */ DWORD __RPC_FAR *queuesize,
            /* [out] */ DWORD __RPC_FAR *totalRequests,
            /* [out] */ BSTR __RPC_FAR *bstrWorldsInfo,
            /* [out] */ DWORD __RPC_FAR *totalConnections,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbIsDSConnected) = 0;
        
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
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE EnumerateWorlds( 
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppGUIDList) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SaveWorld( 
            /* [in] */ BSTR bstrWorldName,
            /* [out] */ BSTR __RPC_FAR *bstrHR) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SaveWorldAs( 
            /* [in] */ BSTR bstrWorldName,
            /* [in] */ BSTR bstrNewWorldName,
            /* [out] */ BSTR __RPC_FAR *bstrHR) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SaveWorldScript( 
            /* [in] */ BSTR bstrWorldName,
            /* [in] */ BSTR bstrScriptName,
            /* [out] */ BSTR __RPC_FAR *bstrHR) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetSimpleStatus( 
            /* [out] */ BSTR __RPC_FAR *bstrMsg) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DeleteWorldFromSystem( 
            /* [in] */ BSTR bstrWorldName,
            /* [out] */ BSTR __RPC_FAR *bstrHR) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE TakeWorldOffline( 
            /* [in] */ BSTR bstrWorldName,
            /* [out] */ BSTR __RPC_FAR *bstrHR) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE TakeWorldOnline( 
            /* [in] */ BSTR bstrWorldName,
            /* [out] */ BSTR __RPC_FAR *bstrHR) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ReconnectToDS( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RefreshDS( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OpenWorld( 
            /* [in] */ BSTR bstrWorldName,
            /* [out] */ BSTR __RPC_FAR *bstrHR) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE PerformanceTest( 
            /* [in] */ BSTR bstrWorldName,
            /* [in] */ long iType,
            /* [out] */ BSTR __RPC_FAR *bstrHR) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ClearPerformanceCounters( void) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_UseAuthentication( 
            /* [retval][out] */ BOOL __RPC_FAR *pVal) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_UseAuthentication( 
            /* [in] */ BOOL newVal) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_AuthenticationGroup( 
            /* [retval][out] */ BSTR __RPC_FAR *bstrGroup) = 0;
        
        virtual /* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE put_AuthenticationGroup( 
            /* [in] */ BSTR bstrGroup) = 0;
        
        virtual /* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE get_RunningOnWinNT( 
            /* [retval][out] */ BOOL __RPC_FAR *pbRunningOnWinNT) = 0;
        
        virtual /* [helpstring][restricted][propget] */ HRESULT STDMETHODCALLTYPE get_ConnectionLimit( 
            /* [retval][out] */ DWORD __RPC_FAR *dwLimit) = 0;
        
        virtual /* [helpstring][restricted][propput] */ HRESULT STDMETHODCALLTYPE put_ConnectionLimit( 
            /* [in] */ DWORD dwLimit) = 0;
        
        virtual /* [restricted][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ConnectionCount( 
            /* [retval][out] */ long __RPC_FAR *pCount) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVWServerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IVWServer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IVWServer __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IVWServer __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IVWServer __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IVWServer __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IVWServer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IVWServer __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Accept )( 
            IVWServer __RPC_FAR * This,
            /* [in] */ int iPort,
            /* [in] */ int iOptionalCallbackWindowHandle);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Disconnect )( 
            IVWServer __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DisconnectWarning )( 
            IVWServer __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DisconnectCancel )( 
            IVWServer __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ShutDown )( 
            IVWServer __RPC_FAR * This);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TestQueue )( 
            IVWServer __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetStatusInfo )( 
            IVWServer __RPC_FAR * This,
            /* [out] */ DWORD __RPC_FAR *queuesize,
            /* [out] */ DWORD __RPC_FAR *totalRequests,
            /* [out] */ BSTR __RPC_FAR *bstrWorldsInfo,
            /* [out] */ DWORD __RPC_FAR *totalConnections,
            /* [out] */ VARIANT_BOOL __RPC_FAR *pbIsDSConnected);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateToolFromProgID )( 
            IVWServer __RPC_FAR * This,
            /* [in] */ BSTR name,
            /* [in] */ BSTR progid,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveTool )( 
            IVWServer __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Tool )( 
            IVWServer __RPC_FAR * This,
            /* [in] */ BSTR name,
            /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Tools )( 
            IVWServer __RPC_FAR * This,
            /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropmap);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EnumerateWorlds )( 
            IVWServer __RPC_FAR * This,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppGUIDList);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveWorld )( 
            IVWServer __RPC_FAR * This,
            /* [in] */ BSTR bstrWorldName,
            /* [out] */ BSTR __RPC_FAR *bstrHR);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveWorldAs )( 
            IVWServer __RPC_FAR * This,
            /* [in] */ BSTR bstrWorldName,
            /* [in] */ BSTR bstrNewWorldName,
            /* [out] */ BSTR __RPC_FAR *bstrHR);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SaveWorldScript )( 
            IVWServer __RPC_FAR * This,
            /* [in] */ BSTR bstrWorldName,
            /* [in] */ BSTR bstrScriptName,
            /* [out] */ BSTR __RPC_FAR *bstrHR);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSimpleStatus )( 
            IVWServer __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *bstrMsg);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DeleteWorldFromSystem )( 
            IVWServer __RPC_FAR * This,
            /* [in] */ BSTR bstrWorldName,
            /* [out] */ BSTR __RPC_FAR *bstrHR);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TakeWorldOffline )( 
            IVWServer __RPC_FAR * This,
            /* [in] */ BSTR bstrWorldName,
            /* [out] */ BSTR __RPC_FAR *bstrHR);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TakeWorldOnline )( 
            IVWServer __RPC_FAR * This,
            /* [in] */ BSTR bstrWorldName,
            /* [out] */ BSTR __RPC_FAR *bstrHR);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReconnectToDS )( 
            IVWServer __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RefreshDS )( 
            IVWServer __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OpenWorld )( 
            IVWServer __RPC_FAR * This,
            /* [in] */ BSTR bstrWorldName,
            /* [out] */ BSTR __RPC_FAR *bstrHR);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PerformanceTest )( 
            IVWServer __RPC_FAR * This,
            /* [in] */ BSTR bstrWorldName,
            /* [in] */ long iType,
            /* [out] */ BSTR __RPC_FAR *bstrHR);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ClearPerformanceCounters )( 
            IVWServer __RPC_FAR * This);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_UseAuthentication )( 
            IVWServer __RPC_FAR * This,
            /* [retval][out] */ BOOL __RPC_FAR *pVal);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_UseAuthentication )( 
            IVWServer __RPC_FAR * This,
            /* [in] */ BOOL newVal);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AuthenticationGroup )( 
            IVWServer __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *bstrGroup);
        
        /* [helpstring][propput][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_AuthenticationGroup )( 
            IVWServer __RPC_FAR * This,
            /* [in] */ BSTR bstrGroup);
        
        /* [helpstring][propget][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_RunningOnWinNT )( 
            IVWServer __RPC_FAR * This,
            /* [retval][out] */ BOOL __RPC_FAR *pbRunningOnWinNT);
        
        /* [helpstring][restricted][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ConnectionLimit )( 
            IVWServer __RPC_FAR * This,
            /* [retval][out] */ DWORD __RPC_FAR *dwLimit);
        
        /* [helpstring][restricted][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ConnectionLimit )( 
            IVWServer __RPC_FAR * This,
            /* [in] */ DWORD dwLimit);
        
        /* [restricted][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ConnectionCount )( 
            IVWServer __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pCount);
        
        END_INTERFACE
    } IVWServerVtbl;

    interface IVWServer
    {
        CONST_VTBL struct IVWServerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVWServer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IVWServer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IVWServer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IVWServer_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IVWServer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IVWServer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IVWServer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IVWServer_Accept(This,iPort,iOptionalCallbackWindowHandle)	\
    (This)->lpVtbl -> Accept(This,iPort,iOptionalCallbackWindowHandle)

#define IVWServer_Disconnect(This)	\
    (This)->lpVtbl -> Disconnect(This)

#define IVWServer_DisconnectWarning(This)	\
    (This)->lpVtbl -> DisconnectWarning(This)

#define IVWServer_DisconnectCancel(This)	\
    (This)->lpVtbl -> DisconnectCancel(This)

#define IVWServer_ShutDown(This)	\
    (This)->lpVtbl -> ShutDown(This)

#define IVWServer_TestQueue(This)	\
    (This)->lpVtbl -> TestQueue(This)

#define IVWServer_GetStatusInfo(This,queuesize,totalRequests,bstrWorldsInfo,totalConnections,pbIsDSConnected)	\
    (This)->lpVtbl -> GetStatusInfo(This,queuesize,totalRequests,bstrWorldsInfo,totalConnections,pbIsDSConnected)

#define IVWServer_CreateToolFromProgID(This,name,progid,ppunk)	\
    (This)->lpVtbl -> CreateToolFromProgID(This,name,progid,ppunk)

#define IVWServer_RemoveTool(This,name)	\
    (This)->lpVtbl -> RemoveTool(This,name)

#define IVWServer_get_Tool(This,name,ppunk)	\
    (This)->lpVtbl -> get_Tool(This,name,ppunk)

#define IVWServer_get_Tools(This,pppropmap)	\
    (This)->lpVtbl -> get_Tools(This,pppropmap)

#define IVWServer_EnumerateWorlds(This,ppGUIDList)	\
    (This)->lpVtbl -> EnumerateWorlds(This,ppGUIDList)

#define IVWServer_SaveWorld(This,bstrWorldName,bstrHR)	\
    (This)->lpVtbl -> SaveWorld(This,bstrWorldName,bstrHR)

#define IVWServer_SaveWorldAs(This,bstrWorldName,bstrNewWorldName,bstrHR)	\
    (This)->lpVtbl -> SaveWorldAs(This,bstrWorldName,bstrNewWorldName,bstrHR)

#define IVWServer_SaveWorldScript(This,bstrWorldName,bstrScriptName,bstrHR)	\
    (This)->lpVtbl -> SaveWorldScript(This,bstrWorldName,bstrScriptName,bstrHR)

#define IVWServer_GetSimpleStatus(This,bstrMsg)	\
    (This)->lpVtbl -> GetSimpleStatus(This,bstrMsg)

#define IVWServer_DeleteWorldFromSystem(This,bstrWorldName,bstrHR)	\
    (This)->lpVtbl -> DeleteWorldFromSystem(This,bstrWorldName,bstrHR)

#define IVWServer_TakeWorldOffline(This,bstrWorldName,bstrHR)	\
    (This)->lpVtbl -> TakeWorldOffline(This,bstrWorldName,bstrHR)

#define IVWServer_TakeWorldOnline(This,bstrWorldName,bstrHR)	\
    (This)->lpVtbl -> TakeWorldOnline(This,bstrWorldName,bstrHR)

#define IVWServer_ReconnectToDS(This)	\
    (This)->lpVtbl -> ReconnectToDS(This)

#define IVWServer_RefreshDS(This)	\
    (This)->lpVtbl -> RefreshDS(This)

#define IVWServer_OpenWorld(This,bstrWorldName,bstrHR)	\
    (This)->lpVtbl -> OpenWorld(This,bstrWorldName,bstrHR)

#define IVWServer_PerformanceTest(This,bstrWorldName,iType,bstrHR)	\
    (This)->lpVtbl -> PerformanceTest(This,bstrWorldName,iType,bstrHR)

#define IVWServer_ClearPerformanceCounters(This)	\
    (This)->lpVtbl -> ClearPerformanceCounters(This)

#define IVWServer_get_UseAuthentication(This,pVal)	\
    (This)->lpVtbl -> get_UseAuthentication(This,pVal)

#define IVWServer_put_UseAuthentication(This,newVal)	\
    (This)->lpVtbl -> put_UseAuthentication(This,newVal)

#define IVWServer_get_AuthenticationGroup(This,bstrGroup)	\
    (This)->lpVtbl -> get_AuthenticationGroup(This,bstrGroup)

#define IVWServer_put_AuthenticationGroup(This,bstrGroup)	\
    (This)->lpVtbl -> put_AuthenticationGroup(This,bstrGroup)

#define IVWServer_get_RunningOnWinNT(This,pbRunningOnWinNT)	\
    (This)->lpVtbl -> get_RunningOnWinNT(This,pbRunningOnWinNT)

#define IVWServer_get_ConnectionLimit(This,dwLimit)	\
    (This)->lpVtbl -> get_ConnectionLimit(This,dwLimit)

#define IVWServer_put_ConnectionLimit(This,dwLimit)	\
    (This)->lpVtbl -> put_ConnectionLimit(This,dwLimit)

#define IVWServer_get_ConnectionCount(This,pCount)	\
    (This)->lpVtbl -> get_ConnectionCount(This,pCount)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWServer_Accept_Proxy( 
    IVWServer __RPC_FAR * This,
    /* [in] */ int iPort,
    /* [in] */ int iOptionalCallbackWindowHandle);


void __RPC_STUB IVWServer_Accept_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWServer_Disconnect_Proxy( 
    IVWServer __RPC_FAR * This);


void __RPC_STUB IVWServer_Disconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IVWServer_DisconnectWarning_Proxy( 
    IVWServer __RPC_FAR * This);


void __RPC_STUB IVWServer_DisconnectWarning_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IVWServer_DisconnectCancel_Proxy( 
    IVWServer __RPC_FAR * This);


void __RPC_STUB IVWServer_DisconnectCancel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWServer_ShutDown_Proxy( 
    IVWServer __RPC_FAR * This);


void __RPC_STUB IVWServer_ShutDown_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IVWServer_TestQueue_Proxy( 
    IVWServer __RPC_FAR * This);


void __RPC_STUB IVWServer_TestQueue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWServer_GetStatusInfo_Proxy( 
    IVWServer __RPC_FAR * This,
    /* [out] */ DWORD __RPC_FAR *queuesize,
    /* [out] */ DWORD __RPC_FAR *totalRequests,
    /* [out] */ BSTR __RPC_FAR *bstrWorldsInfo,
    /* [out] */ DWORD __RPC_FAR *totalConnections,
    /* [out] */ VARIANT_BOOL __RPC_FAR *pbIsDSConnected);


void __RPC_STUB IVWServer_GetStatusInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWServer_CreateToolFromProgID_Proxy( 
    IVWServer __RPC_FAR * This,
    /* [in] */ BSTR name,
    /* [in] */ BSTR progid,
    /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);


void __RPC_STUB IVWServer_CreateToolFromProgID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWServer_RemoveTool_Proxy( 
    IVWServer __RPC_FAR * This,
    /* [in] */ BSTR name);


void __RPC_STUB IVWServer_RemoveTool_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IVWServer_get_Tool_Proxy( 
    IVWServer __RPC_FAR * This,
    /* [in] */ BSTR name,
    /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk);


void __RPC_STUB IVWServer_get_Tool_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IVWServer_get_Tools_Proxy( 
    IVWServer __RPC_FAR * This,
    /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropmap);


void __RPC_STUB IVWServer_get_Tools_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWServer_EnumerateWorlds_Proxy( 
    IVWServer __RPC_FAR * This,
    /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppGUIDList);


void __RPC_STUB IVWServer_EnumerateWorlds_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWServer_SaveWorld_Proxy( 
    IVWServer __RPC_FAR * This,
    /* [in] */ BSTR bstrWorldName,
    /* [out] */ BSTR __RPC_FAR *bstrHR);


void __RPC_STUB IVWServer_SaveWorld_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWServer_SaveWorldAs_Proxy( 
    IVWServer __RPC_FAR * This,
    /* [in] */ BSTR bstrWorldName,
    /* [in] */ BSTR bstrNewWorldName,
    /* [out] */ BSTR __RPC_FAR *bstrHR);


void __RPC_STUB IVWServer_SaveWorldAs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWServer_SaveWorldScript_Proxy( 
    IVWServer __RPC_FAR * This,
    /* [in] */ BSTR bstrWorldName,
    /* [in] */ BSTR bstrScriptName,
    /* [out] */ BSTR __RPC_FAR *bstrHR);


void __RPC_STUB IVWServer_SaveWorldScript_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWServer_GetSimpleStatus_Proxy( 
    IVWServer __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *bstrMsg);


void __RPC_STUB IVWServer_GetSimpleStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWServer_DeleteWorldFromSystem_Proxy( 
    IVWServer __RPC_FAR * This,
    /* [in] */ BSTR bstrWorldName,
    /* [out] */ BSTR __RPC_FAR *bstrHR);


void __RPC_STUB IVWServer_DeleteWorldFromSystem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWServer_TakeWorldOffline_Proxy( 
    IVWServer __RPC_FAR * This,
    /* [in] */ BSTR bstrWorldName,
    /* [out] */ BSTR __RPC_FAR *bstrHR);


void __RPC_STUB IVWServer_TakeWorldOffline_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWServer_TakeWorldOnline_Proxy( 
    IVWServer __RPC_FAR * This,
    /* [in] */ BSTR bstrWorldName,
    /* [out] */ BSTR __RPC_FAR *bstrHR);


void __RPC_STUB IVWServer_TakeWorldOnline_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWServer_ReconnectToDS_Proxy( 
    IVWServer __RPC_FAR * This);


void __RPC_STUB IVWServer_ReconnectToDS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWServer_RefreshDS_Proxy( 
    IVWServer __RPC_FAR * This);


void __RPC_STUB IVWServer_RefreshDS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWServer_OpenWorld_Proxy( 
    IVWServer __RPC_FAR * This,
    /* [in] */ BSTR bstrWorldName,
    /* [out] */ BSTR __RPC_FAR *bstrHR);


void __RPC_STUB IVWServer_OpenWorld_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE IVWServer_PerformanceTest_Proxy( 
    IVWServer __RPC_FAR * This,
    /* [in] */ BSTR bstrWorldName,
    /* [in] */ long iType,
    /* [out] */ BSTR __RPC_FAR *bstrHR);


void __RPC_STUB IVWServer_PerformanceTest_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IVWServer_ClearPerformanceCounters_Proxy( 
    IVWServer __RPC_FAR * This);


void __RPC_STUB IVWServer_ClearPerformanceCounters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IVWServer_get_UseAuthentication_Proxy( 
    IVWServer __RPC_FAR * This,
    /* [retval][out] */ BOOL __RPC_FAR *pVal);


void __RPC_STUB IVWServer_get_UseAuthentication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IVWServer_put_UseAuthentication_Proxy( 
    IVWServer __RPC_FAR * This,
    /* [in] */ BOOL newVal);


void __RPC_STUB IVWServer_put_UseAuthentication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IVWServer_get_AuthenticationGroup_Proxy( 
    IVWServer __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *bstrGroup);


void __RPC_STUB IVWServer_get_AuthenticationGroup_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput][restricted] */ HRESULT STDMETHODCALLTYPE IVWServer_put_AuthenticationGroup_Proxy( 
    IVWServer __RPC_FAR * This,
    /* [in] */ BSTR bstrGroup);


void __RPC_STUB IVWServer_put_AuthenticationGroup_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget][restricted] */ HRESULT STDMETHODCALLTYPE IVWServer_get_RunningOnWinNT_Proxy( 
    IVWServer __RPC_FAR * This,
    /* [retval][out] */ BOOL __RPC_FAR *pbRunningOnWinNT);


void __RPC_STUB IVWServer_get_RunningOnWinNT_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted][propget] */ HRESULT STDMETHODCALLTYPE IVWServer_get_ConnectionLimit_Proxy( 
    IVWServer __RPC_FAR * This,
    /* [retval][out] */ DWORD __RPC_FAR *dwLimit);


void __RPC_STUB IVWServer_get_ConnectionLimit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted][propput] */ HRESULT STDMETHODCALLTYPE IVWServer_put_ConnectionLimit_Proxy( 
    IVWServer __RPC_FAR * This,
    /* [in] */ DWORD dwLimit);


void __RPC_STUB IVWServer_put_ConnectionLimit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IVWServer_get_ConnectionCount_Proxy( 
    IVWServer __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pCount);


void __RPC_STUB IVWServer_get_ConnectionCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IVWServer_INTERFACE_DEFINED__ */


#ifndef __DVWServerEvents_INTERFACE_DEFINED__
#define __DVWServerEvents_INTERFACE_DEFINED__

/* interface DVWServerEvents */
/* [unique][local][dual][helpstring][uuid][object] */ 


EXTERN_C const IID IID_DVWServerEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F8584900-00F4-11D0-A516-00AA00B595DB")
    DVWServerEvents : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnWorldLoaded( 
            /* [in] */ BSTR bstrWorld) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnWorldUnloaded( 
            /* [in] */ BSTR bstrWorld) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnConnect( 
            /* [in] */ DWORD hConnection) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnDisconnect( 
            /* [in] */ DWORD hConnection) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnEnterWorld( 
            /* [in] */ DWORD hConnection,
            /* [in] */ BSTR bstrWorld) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnLeaveWorld( 
            /* [in] */ DWORD hConnection,
            /* [in] */ BSTR bstrWorld) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnCommError( 
            /* [in] */ DWORD hConnection) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnCommMessage( 
            /* [in] */ DWORD hConnection,
            /* [in] */ BSTR bstrMessage) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnReport( 
            /* [in] */ BSTR bstr,
            /* [in] */ long lType) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnTrace( 
            /* [in] */ BSTR bstr) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnAvailableWorld( 
            /* [in] */ BSTR bstrName,
            /* [in] */ int iNoUsers) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct DVWServerEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            DVWServerEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            DVWServerEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            DVWServerEvents __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnWorldLoaded )( 
            DVWServerEvents __RPC_FAR * This,
            /* [in] */ BSTR bstrWorld);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnWorldUnloaded )( 
            DVWServerEvents __RPC_FAR * This,
            /* [in] */ BSTR bstrWorld);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnConnect )( 
            DVWServerEvents __RPC_FAR * This,
            /* [in] */ DWORD hConnection);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnDisconnect )( 
            DVWServerEvents __RPC_FAR * This,
            /* [in] */ DWORD hConnection);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnEnterWorld )( 
            DVWServerEvents __RPC_FAR * This,
            /* [in] */ DWORD hConnection,
            /* [in] */ BSTR bstrWorld);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnLeaveWorld )( 
            DVWServerEvents __RPC_FAR * This,
            /* [in] */ DWORD hConnection,
            /* [in] */ BSTR bstrWorld);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnCommError )( 
            DVWServerEvents __RPC_FAR * This,
            /* [in] */ DWORD hConnection);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnCommMessage )( 
            DVWServerEvents __RPC_FAR * This,
            /* [in] */ DWORD hConnection,
            /* [in] */ BSTR bstrMessage);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnReport )( 
            DVWServerEvents __RPC_FAR * This,
            /* [in] */ BSTR bstr,
            /* [in] */ long lType);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnTrace )( 
            DVWServerEvents __RPC_FAR * This,
            /* [in] */ BSTR bstr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnAvailableWorld )( 
            DVWServerEvents __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ int iNoUsers);
        
        END_INTERFACE
    } DVWServerEventsVtbl;

    interface DVWServerEvents
    {
        CONST_VTBL struct DVWServerEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define DVWServerEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define DVWServerEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define DVWServerEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define DVWServerEvents_OnWorldLoaded(This,bstrWorld)	\
    (This)->lpVtbl -> OnWorldLoaded(This,bstrWorld)

#define DVWServerEvents_OnWorldUnloaded(This,bstrWorld)	\
    (This)->lpVtbl -> OnWorldUnloaded(This,bstrWorld)

#define DVWServerEvents_OnConnect(This,hConnection)	\
    (This)->lpVtbl -> OnConnect(This,hConnection)

#define DVWServerEvents_OnDisconnect(This,hConnection)	\
    (This)->lpVtbl -> OnDisconnect(This,hConnection)

#define DVWServerEvents_OnEnterWorld(This,hConnection,bstrWorld)	\
    (This)->lpVtbl -> OnEnterWorld(This,hConnection,bstrWorld)

#define DVWServerEvents_OnLeaveWorld(This,hConnection,bstrWorld)	\
    (This)->lpVtbl -> OnLeaveWorld(This,hConnection,bstrWorld)

#define DVWServerEvents_OnCommError(This,hConnection)	\
    (This)->lpVtbl -> OnCommError(This,hConnection)

#define DVWServerEvents_OnCommMessage(This,hConnection,bstrMessage)	\
    (This)->lpVtbl -> OnCommMessage(This,hConnection,bstrMessage)

#define DVWServerEvents_OnReport(This,bstr,lType)	\
    (This)->lpVtbl -> OnReport(This,bstr,lType)

#define DVWServerEvents_OnTrace(This,bstr)	\
    (This)->lpVtbl -> OnTrace(This,bstr)

#define DVWServerEvents_OnAvailableWorld(This,bstrName,iNoUsers)	\
    (This)->lpVtbl -> OnAvailableWorld(This,bstrName,iNoUsers)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE DVWServerEvents_OnWorldLoaded_Proxy( 
    DVWServerEvents __RPC_FAR * This,
    /* [in] */ BSTR bstrWorld);


void __RPC_STUB DVWServerEvents_OnWorldLoaded_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE DVWServerEvents_OnWorldUnloaded_Proxy( 
    DVWServerEvents __RPC_FAR * This,
    /* [in] */ BSTR bstrWorld);


void __RPC_STUB DVWServerEvents_OnWorldUnloaded_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE DVWServerEvents_OnConnect_Proxy( 
    DVWServerEvents __RPC_FAR * This,
    /* [in] */ DWORD hConnection);


void __RPC_STUB DVWServerEvents_OnConnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE DVWServerEvents_OnDisconnect_Proxy( 
    DVWServerEvents __RPC_FAR * This,
    /* [in] */ DWORD hConnection);


void __RPC_STUB DVWServerEvents_OnDisconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE DVWServerEvents_OnEnterWorld_Proxy( 
    DVWServerEvents __RPC_FAR * This,
    /* [in] */ DWORD hConnection,
    /* [in] */ BSTR bstrWorld);


void __RPC_STUB DVWServerEvents_OnEnterWorld_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE DVWServerEvents_OnLeaveWorld_Proxy( 
    DVWServerEvents __RPC_FAR * This,
    /* [in] */ DWORD hConnection,
    /* [in] */ BSTR bstrWorld);


void __RPC_STUB DVWServerEvents_OnLeaveWorld_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE DVWServerEvents_OnCommError_Proxy( 
    DVWServerEvents __RPC_FAR * This,
    /* [in] */ DWORD hConnection);


void __RPC_STUB DVWServerEvents_OnCommError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE DVWServerEvents_OnCommMessage_Proxy( 
    DVWServerEvents __RPC_FAR * This,
    /* [in] */ DWORD hConnection,
    /* [in] */ BSTR bstrMessage);


void __RPC_STUB DVWServerEvents_OnCommMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE DVWServerEvents_OnReport_Proxy( 
    DVWServerEvents __RPC_FAR * This,
    /* [in] */ BSTR bstr,
    /* [in] */ long lType);


void __RPC_STUB DVWServerEvents_OnReport_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE DVWServerEvents_OnTrace_Proxy( 
    DVWServerEvents __RPC_FAR * This,
    /* [in] */ BSTR bstr);


void __RPC_STUB DVWServerEvents_OnTrace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE DVWServerEvents_OnAvailableWorld_Proxy( 
    DVWServerEvents __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [in] */ int iNoUsers);


void __RPC_STUB DVWServerEvents_OnAvailableWorld_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __DVWServerEvents_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
