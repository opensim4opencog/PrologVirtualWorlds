/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:35 2000
 */
/* Compiler settings for vwsysex.idl:
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

#ifndef __vwsysex_h__
#define __vwsysex_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ISystemExemplar_FWD_DEFINED__
#define __ISystemExemplar_FWD_DEFINED__
typedef interface ISystemExemplar ISystemExemplar;
#endif 	/* __ISystemExemplar_FWD_DEFINED__ */


#ifndef __IThingExemplar_FWD_DEFINED__
#define __IThingExemplar_FWD_DEFINED__
typedef interface IThingExemplar IThingExemplar;
#endif 	/* __IThingExemplar_FWD_DEFINED__ */


#ifndef __IArtifactExemplar_FWD_DEFINED__
#define __IArtifactExemplar_FWD_DEFINED__
typedef interface IArtifactExemplar IArtifactExemplar;
#endif 	/* __IArtifactExemplar_FWD_DEFINED__ */


#ifndef __IAvatarExemplar_FWD_DEFINED__
#define __IAvatarExemplar_FWD_DEFINED__
typedef interface IAvatarExemplar IAvatarExemplar;
#endif 	/* __IAvatarExemplar_FWD_DEFINED__ */


#ifndef __IHostExemplar_FWD_DEFINED__
#define __IHostExemplar_FWD_DEFINED__
typedef interface IHostExemplar IHostExemplar;
#endif 	/* __IHostExemplar_FWD_DEFINED__ */


#ifndef __IPortalExemplar_FWD_DEFINED__
#define __IPortalExemplar_FWD_DEFINED__
typedef interface IPortalExemplar IPortalExemplar;
#endif 	/* __IPortalExemplar_FWD_DEFINED__ */


#ifndef __IIWPortalExemplar_FWD_DEFINED__
#define __IIWPortalExemplar_FWD_DEFINED__
typedef interface IIWPortalExemplar IIWPortalExemplar;
#endif 	/* __IIWPortalExemplar_FWD_DEFINED__ */


#ifndef __IRoomExemplar_FWD_DEFINED__
#define __IRoomExemplar_FWD_DEFINED__
typedef interface IRoomExemplar IRoomExemplar;
#endif 	/* __IRoomExemplar_FWD_DEFINED__ */


#ifndef __IStateMachineExemplar_FWD_DEFINED__
#define __IStateMachineExemplar_FWD_DEFINED__
typedef interface IStateMachineExemplar IStateMachineExemplar;
#endif 	/* __IStateMachineExemplar_FWD_DEFINED__ */


#ifndef __IWebPageExemplar_FWD_DEFINED__
#define __IWebPageExemplar_FWD_DEFINED__
typedef interface IWebPageExemplar IWebPageExemplar;
#endif 	/* __IWebPageExemplar_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "vwobject.h"
#include "menuitem.h"
#include "ocxproxy.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_vwsysex_0000 */
/* [local] */ 

#define VWPORTAL_E_NOTAVALIDINDEX			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c00)
#define VWPORTAL_E_NODESTINATIONS			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c01)
#define VWPORTAL_E_INVALIDDESTINATION		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c02)
#define VWPORTAL_E_UNKNOWN					MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c03)
#define VWAVATAR_E_NODEFAULTROOMORPORTAL		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c10)
#define VWAVATAR_E_NOPROFILE					MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c11)
#define VWAVATAR_E_WHISPERNOTALLOWED			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c12)
#define VWHOST_E_NOTAHOST					MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c20)
#define VWHOST_E_CANTLOGOFFWIZARD			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c21)
#define VWTHING_E_MOVEINTOPOLICYDENIED		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c30)
#define VWTHING_E_ACCEPTLEAVEDENIED			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c31)
#define VWTHING_E_ALLOWLEAVEDENIED			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c32)
#define VWTHING_E_ACCEPTENTERDENIED			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c33)
#define VWTHING_E_ALLOWENTERDENIED			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c34)
#define VWTHING_E_TAKENOTINCONTAINER			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c35)
#define VWTHING_E_DROPNOTINCONTENTS			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c36)
#define VWTHING_E_TAKEPERMISSIONDENIED		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c37)
#define VWTHING_E_UNKNOWN					MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c38)
#define VWTHING_E_NOSECURITYONSERVER			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c39)
#define VWTHING_E_NOSECURITYFORUNKNOWNUSER	MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c3a)
#define VWTHING_E_NOTACLIENTPROPERTY			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c3b)
#define VWIWPORTAL_E_NODESTINATION			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c50)
#define VWIWPORTAL_E_INVALIDDESTINATION		MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c51)
#define VWIWPORTAL_E_RECONNECTFAILED			MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c52)
#define VWCORE_E_FILEUPLOADERROR				MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c60)
#define VWCORE_E_NOTAWIZARD					MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c61)
#define VWCORE_E_NOTANAVATAR					MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c62)
#define VWCORE_E_NOTAHOST					MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0c63)


extern RPC_IF_HANDLE __MIDL_itf_vwsysex_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_vwsysex_0000_v0_0_s_ifspec;

#ifndef __ISystemExemplar_INTERFACE_DEFINED__
#define __ISystemExemplar_INTERFACE_DEFINED__

/* interface ISystemExemplar */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISystemExemplar;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("717B5A86-7267-11d1-959F-00C04FD91F4D")
    ISystemExemplar : public IVWDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreatePropertyList( 
            /* [optional][in] */ VARIANT varInitialValue,
            /* [optional][in] */ VARIANT varOrphan,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *pppropertylist) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreatePropertyMap( 
            /* [optional][in] */ VARIANT varOrphan,
            /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropertymap) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateMenu( 
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenu) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateMenuItem( 
            /* [in] */ BSTR bstrMenuText,
            /* [in] */ long lFlags,
            /* [in] */ IThing __RPC_FAR *pOwner,
            /* [in] */ BSTR bstrUpdateMethod,
            /* [in] */ BSTR bstrMethod,
            /* [optional][in] */ VARIANT var,
            /* [retval][out] */ IMenuItem __RPC_FAR *__RPC_FAR *ppMenuItem) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateMenuSeparator( 
            /* [in] */ IThing __RPC_FAR *pOwner,
            /* [retval][out] */ IMenuItem __RPC_FAR *__RPC_FAR *ppMenuItem) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateOCXProxy( 
            /* [in] */ IDispatch __RPC_FAR *pOCX,
            /* [retval][out] */ IOCXProxy __RPC_FAR *__RPC_FAR *ppOCXProxy) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QueryAvatarNames( 
            /* [in] */ long lFalgs,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *pNameList) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE InvokeRemoteClientMethod( 
            /* [in] */ IThing __RPC_FAR *pDispTarget,
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ VARIANT varUserOrList,
            /* [in] */ VARIANT varParams) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE HandleRemoteClientMethod( 
            /* [in] */ IThing __RPC_FAR *pDispTarget,
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ VARIANT varUserOrList,
            /* [in] */ VARIANT varParams) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE FireUIEvent( 
            /* [in] */ IPropertyList __RPC_FAR *pClientList,
            /* [in] */ IThing __RPC_FAR *pSource,
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ VARIANT varParams) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE FireRemoteUIEvent( 
            /* [in] */ VARIANT varClientList,
            /* [in] */ IThing __RPC_FAR *pSource,
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ VARIANT varParams) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnRootURLChanged( 
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE UploadFile( 
            /* [in] */ BSTR bstrLocalName,
            /* [in] */ BSTR bstrRemoteName,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrURL) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE MakeHost( 
            /* [in] */ IThing __RPC_FAR *pTarget) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE UnMakeHost( 
            /* [in] */ IThing __RPC_FAR *pTarget) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateTimer( 
            /* [in] */ int msecPeriod,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrMethod,
            /* [optional][in] */ VARIANT varArgs,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppTimer) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DestroyTimer( 
            /* [in] */ IDispatch __RPC_FAR *pTimer) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnCreateRoomClient( 
            /* [in] */ IThing __RPC_FAR *pThing) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ShowWelcomeMessage( void) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnWorldOwnerAccess( 
            /* [in] */ long lPerms,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnGlobalConstantAccess( 
            /* [in] */ long lPerms,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TraceExemplarInfo( void) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimeGetProperty( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrPropertyName,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimePutProperty( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrPropertyName,
            /* [in] */ VARIANT varData,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimeInvokeMethod( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ VARIANT varArgs,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimeGetMethod( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimeGetPropertySecurity( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrPropertyName,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimeCheckPropertySecurity( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrPropertyName,
            /* [in] */ unsigned long ulFlags,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimeCheckMethodSecurity( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ unsigned long ulFlags,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimeCheckPropertiesSecurity( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ unsigned long ulFlags,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimeCheckMethodsSecurity( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ unsigned long ulFlags,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimeVTableInvokeMethod( 
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimeAutomationInvokeMethod( 
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimeScriptInvokeMethod( 
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimeInterfaceInvokeMethod( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE TimeNotifyInvokeMethod( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE TimeNotifyPropertyChanged( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrPropertyName,
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE TimeSendThingCommand( 
            /* [in] */ long lFlags,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE TimeSendObjectPropertyCommand( 
            /* [in] */ long lFlags,
            /* [in] */ IObjectProperty __RPC_FAR *pprop,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE TimeSendWorldCommand( 
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE TimeProcessMessage( 
            /* [in] */ unsigned char ch,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *pUbuffer,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimeCloneProperty( 
            /* [in] */ VARIANT varData,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimeCopyProperty( 
            /* [in] */ VARIANT varData,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimePutPropertySecurity( 
            /* [in] */ IPropertySecurity __RPC_FAR *pSecurity,
            /* [in] */ VARIANT varData,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimeGetPropertyAll( 
            /* [in] */ VARIANT varData,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimePutPropertyAll( 
            /* [in] */ VARIANT varData,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimeInvokeMethodAll( 
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimeGetMethodAll( 
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimeGetPropertySecurityAll( 
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimeCheckPropertySecurityAll( 
            /* [in] */ unsigned long ulFlags,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimeCheckMethodSecurityAll( 
            /* [in] */ unsigned long ulFlags,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimeCheckPropertiesSecurityAll( 
            /* [in] */ unsigned long ulFlags,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimeCheckMethodsSecurityAll( 
            /* [in] */ unsigned long ulFlags,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE TimeNotifyAll( 
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE TimeSendCommandAll( 
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE TimeProcessMessageAll( 
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE TimeAll( 
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE YYY( void) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnXXXChanged( 
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnXXXAccess( 
            /* [in] */ long lPerms,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISystemExemplarVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISystemExemplar __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISystemExemplar __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ISystemExemplar __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            ISystemExemplar __RPC_FAR * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Install )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ IModule __RPC_FAR *pmodule);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnLoad )( 
            ISystemExemplar __RPC_FAR * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUnload )( 
            ISystemExemplar __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreatePropertyList )( 
            ISystemExemplar __RPC_FAR * This,
            /* [optional][in] */ VARIANT varInitialValue,
            /* [optional][in] */ VARIANT varOrphan,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *pppropertylist);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreatePropertyMap )( 
            ISystemExemplar __RPC_FAR * This,
            /* [optional][in] */ VARIANT varOrphan,
            /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropertymap);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateMenu )( 
            ISystemExemplar __RPC_FAR * This,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenu);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateMenuItem )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrMenuText,
            /* [in] */ long lFlags,
            /* [in] */ IThing __RPC_FAR *pOwner,
            /* [in] */ BSTR bstrUpdateMethod,
            /* [in] */ BSTR bstrMethod,
            /* [optional][in] */ VARIANT var,
            /* [retval][out] */ IMenuItem __RPC_FAR *__RPC_FAR *ppMenuItem);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateMenuSeparator )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pOwner,
            /* [retval][out] */ IMenuItem __RPC_FAR *__RPC_FAR *ppMenuItem);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateOCXProxy )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *pOCX,
            /* [retval][out] */ IOCXProxy __RPC_FAR *__RPC_FAR *ppOCXProxy);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryAvatarNames )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ long lFalgs,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *pNameList);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InvokeRemoteClientMethod )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pDispTarget,
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ VARIANT varUserOrList,
            /* [in] */ VARIANT varParams);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *HandleRemoteClientMethod )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pDispTarget,
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ VARIANT varUserOrList,
            /* [in] */ VARIANT varParams);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FireUIEvent )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ IPropertyList __RPC_FAR *pClientList,
            /* [in] */ IThing __RPC_FAR *pSource,
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ VARIANT varParams);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FireRemoteUIEvent )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ VARIANT varClientList,
            /* [in] */ IThing __RPC_FAR *pSource,
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ VARIANT varParams);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnRootURLChanged )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UploadFile )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrLocalName,
            /* [in] */ BSTR bstrRemoteName,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrURL);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MakeHost )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pTarget);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnMakeHost )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pTarget);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateTimer )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ int msecPeriod,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrMethod,
            /* [optional][in] */ VARIANT varArgs,
            /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppTimer);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DestroyTimer )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ IDispatch __RPC_FAR *pTimer);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnCreateRoomClient )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ShowWelcomeMessage )( 
            ISystemExemplar __RPC_FAR * This);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnWorldOwnerAccess )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ long lPerms,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnGlobalConstantAccess )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ long lPerms,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TraceExemplarInfo )( 
            ISystemExemplar __RPC_FAR * This);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeGetProperty )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrPropertyName,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimePutProperty )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrPropertyName,
            /* [in] */ VARIANT varData,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeInvokeMethod )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ VARIANT varArgs,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeGetMethod )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeGetPropertySecurity )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrPropertyName,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeCheckPropertySecurity )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrPropertyName,
            /* [in] */ unsigned long ulFlags,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeCheckMethodSecurity )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ unsigned long ulFlags,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeCheckPropertiesSecurity )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ unsigned long ulFlags,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeCheckMethodsSecurity )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ unsigned long ulFlags,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeVTableInvokeMethod )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeAutomationInvokeMethod )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeScriptInvokeMethod )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeInterfaceInvokeMethod )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeNotifyInvokeMethod )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeNotifyPropertyChanged )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ BSTR bstrPropertyName,
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeSendThingCommand )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ long lFlags,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeSendObjectPropertyCommand )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ long lFlags,
            /* [in] */ IObjectProperty __RPC_FAR *pprop,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeSendWorldCommand )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ DISPID dispid,
            /* [in] */ WORD wFlags,
            /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeProcessMessage )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ unsigned char ch,
            /* [in] */ IUnMarshallBuffer __RPC_FAR *pUbuffer,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeCloneProperty )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ VARIANT varData,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeCopyProperty )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ VARIANT varData,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimePutPropertySecurity )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ IPropertySecurity __RPC_FAR *pSecurity,
            /* [in] */ VARIANT varData,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeGetPropertyAll )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ VARIANT varData,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimePutPropertyAll )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ VARIANT varData,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeInvokeMethodAll )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeGetMethodAll )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeGetPropertySecurityAll )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeCheckPropertySecurityAll )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ unsigned long ulFlags,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeCheckMethodSecurityAll )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ unsigned long ulFlags,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeCheckPropertiesSecurityAll )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ unsigned long ulFlags,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeCheckMethodsSecurityAll )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ unsigned long ulFlags,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeNotifyAll )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeSendCommandAll )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeProcessMessageAll )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TimeAll )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ long lRepeat,
            /* [retval][out] */ long __RPC_FAR *plElapsed);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *YYY )( 
            ISystemExemplar __RPC_FAR * This);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnXXXChanged )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnXXXAccess )( 
            ISystemExemplar __RPC_FAR * This,
            /* [in] */ long lPerms,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue);
        
        END_INTERFACE
    } ISystemExemplarVtbl;

    interface ISystemExemplar
    {
        CONST_VTBL struct ISystemExemplarVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISystemExemplar_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISystemExemplar_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISystemExemplar_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISystemExemplar_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISystemExemplar_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISystemExemplar_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISystemExemplar_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISystemExemplar_Initialize(This,pworld)	\
    (This)->lpVtbl -> Initialize(This,pworld)

#define ISystemExemplar_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define ISystemExemplar_Install(This,pmodule)	\
    (This)->lpVtbl -> Install(This,pmodule)

#define ISystemExemplar_OnLoad(This)	\
    (This)->lpVtbl -> OnLoad(This)

#define ISystemExemplar_OnUnload(This)	\
    (This)->lpVtbl -> OnUnload(This)


#define ISystemExemplar_CreatePropertyList(This,varInitialValue,varOrphan,pppropertylist)	\
    (This)->lpVtbl -> CreatePropertyList(This,varInitialValue,varOrphan,pppropertylist)

#define ISystemExemplar_CreatePropertyMap(This,varOrphan,pppropertymap)	\
    (This)->lpVtbl -> CreatePropertyMap(This,varOrphan,pppropertymap)

#define ISystemExemplar_CreateMenu(This,ppMenu)	\
    (This)->lpVtbl -> CreateMenu(This,ppMenu)

#define ISystemExemplar_CreateMenuItem(This,bstrMenuText,lFlags,pOwner,bstrUpdateMethod,bstrMethod,var,ppMenuItem)	\
    (This)->lpVtbl -> CreateMenuItem(This,bstrMenuText,lFlags,pOwner,bstrUpdateMethod,bstrMethod,var,ppMenuItem)

#define ISystemExemplar_CreateMenuSeparator(This,pOwner,ppMenuItem)	\
    (This)->lpVtbl -> CreateMenuSeparator(This,pOwner,ppMenuItem)

#define ISystemExemplar_CreateOCXProxy(This,pOCX,ppOCXProxy)	\
    (This)->lpVtbl -> CreateOCXProxy(This,pOCX,ppOCXProxy)

#define ISystemExemplar_QueryAvatarNames(This,lFalgs,pNameList)	\
    (This)->lpVtbl -> QueryAvatarNames(This,lFalgs,pNameList)

#define ISystemExemplar_InvokeRemoteClientMethod(This,pDispTarget,bstrMethodName,varUserOrList,varParams)	\
    (This)->lpVtbl -> InvokeRemoteClientMethod(This,pDispTarget,bstrMethodName,varUserOrList,varParams)

#define ISystemExemplar_HandleRemoteClientMethod(This,pDispTarget,bstrMethodName,varUserOrList,varParams)	\
    (This)->lpVtbl -> HandleRemoteClientMethod(This,pDispTarget,bstrMethodName,varUserOrList,varParams)

#define ISystemExemplar_FireUIEvent(This,pClientList,pSource,bstrMethodName,varParams)	\
    (This)->lpVtbl -> FireUIEvent(This,pClientList,pSource,bstrMethodName,varParams)

#define ISystemExemplar_FireRemoteUIEvent(This,varClientList,pSource,bstrMethodName,varParams)	\
    (This)->lpVtbl -> FireRemoteUIEvent(This,varClientList,pSource,bstrMethodName,varParams)

#define ISystemExemplar_OnRootURLChanged(This,lHint,varHintData)	\
    (This)->lpVtbl -> OnRootURLChanged(This,lHint,varHintData)

#define ISystemExemplar_UploadFile(This,bstrLocalName,bstrRemoteName,pbstrURL)	\
    (This)->lpVtbl -> UploadFile(This,bstrLocalName,bstrRemoteName,pbstrURL)

#define ISystemExemplar_MakeHost(This,pTarget)	\
    (This)->lpVtbl -> MakeHost(This,pTarget)

#define ISystemExemplar_UnMakeHost(This,pTarget)	\
    (This)->lpVtbl -> UnMakeHost(This,pTarget)

#define ISystemExemplar_CreateTimer(This,msecPeriod,pThing,bstrMethod,varArgs,ppTimer)	\
    (This)->lpVtbl -> CreateTimer(This,msecPeriod,pThing,bstrMethod,varArgs,ppTimer)

#define ISystemExemplar_DestroyTimer(This,pTimer)	\
    (This)->lpVtbl -> DestroyTimer(This,pTimer)

#define ISystemExemplar_OnCreateRoomClient(This,pThing)	\
    (This)->lpVtbl -> OnCreateRoomClient(This,pThing)

#define ISystemExemplar_ShowWelcomeMessage(This)	\
    (This)->lpVtbl -> ShowWelcomeMessage(This)

#define ISystemExemplar_OnWorldOwnerAccess(This,lPerms,bReturnValue)	\
    (This)->lpVtbl -> OnWorldOwnerAccess(This,lPerms,bReturnValue)

#define ISystemExemplar_OnGlobalConstantAccess(This,lPerms,bReturnValue)	\
    (This)->lpVtbl -> OnGlobalConstantAccess(This,lPerms,bReturnValue)

#define ISystemExemplar_TraceExemplarInfo(This)	\
    (This)->lpVtbl -> TraceExemplarInfo(This)

#define ISystemExemplar_TimeGetProperty(This,pthing,bstrPropertyName,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeGetProperty(This,pthing,bstrPropertyName,lRepeat,plElapsed)

#define ISystemExemplar_TimePutProperty(This,pthing,bstrPropertyName,varData,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimePutProperty(This,pthing,bstrPropertyName,varData,lRepeat,plElapsed)

#define ISystemExemplar_TimeInvokeMethod(This,pthing,bstrMethodName,varArgs,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeInvokeMethod(This,pthing,bstrMethodName,varArgs,lRepeat,plElapsed)

#define ISystemExemplar_TimeGetMethod(This,pthing,bstrMethodName,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeGetMethod(This,pthing,bstrMethodName,lRepeat,plElapsed)

#define ISystemExemplar_TimeGetPropertySecurity(This,pthing,bstrPropertyName,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeGetPropertySecurity(This,pthing,bstrPropertyName,lRepeat,plElapsed)

#define ISystemExemplar_TimeCheckPropertySecurity(This,pthing,bstrPropertyName,ulFlags,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeCheckPropertySecurity(This,pthing,bstrPropertyName,ulFlags,lRepeat,plElapsed)

#define ISystemExemplar_TimeCheckMethodSecurity(This,pthing,bstrMethodName,ulFlags,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeCheckMethodSecurity(This,pthing,bstrMethodName,ulFlags,lRepeat,plElapsed)

#define ISystemExemplar_TimeCheckPropertiesSecurity(This,pthing,ulFlags,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeCheckPropertiesSecurity(This,pthing,ulFlags,lRepeat,plElapsed)

#define ISystemExemplar_TimeCheckMethodsSecurity(This,pthing,ulFlags,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeCheckMethodsSecurity(This,pthing,ulFlags,lRepeat,plElapsed)

#define ISystemExemplar_TimeVTableInvokeMethod(This,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeVTableInvokeMethod(This,lRepeat,plElapsed)

#define ISystemExemplar_TimeAutomationInvokeMethod(This,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeAutomationInvokeMethod(This,lRepeat,plElapsed)

#define ISystemExemplar_TimeScriptInvokeMethod(This,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeScriptInvokeMethod(This,lRepeat,plElapsed)

#define ISystemExemplar_TimeInterfaceInvokeMethod(This,pthing,bstrMethodName,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeInterfaceInvokeMethod(This,pthing,bstrMethodName,lRepeat,plElapsed)

#define ISystemExemplar_TimeNotifyInvokeMethod(This,pthing,bstrMethodName,pdispparams,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeNotifyInvokeMethod(This,pthing,bstrMethodName,pdispparams,lRepeat,plElapsed)

#define ISystemExemplar_TimeNotifyPropertyChanged(This,pthing,bstrPropertyName,lHint,varHintData,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeNotifyPropertyChanged(This,pthing,bstrPropertyName,lHint,varHintData,lRepeat,plElapsed)

#define ISystemExemplar_TimeSendThingCommand(This,lFlags,pthing,dispid,wFlags,pdispparams,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeSendThingCommand(This,lFlags,pthing,dispid,wFlags,pdispparams,lRepeat,plElapsed)

#define ISystemExemplar_TimeSendObjectPropertyCommand(This,lFlags,pprop,dispid,wFlags,pdispparams,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeSendObjectPropertyCommand(This,lFlags,pprop,dispid,wFlags,pdispparams,lRepeat,plElapsed)

#define ISystemExemplar_TimeSendWorldCommand(This,dispid,wFlags,pdispparams,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeSendWorldCommand(This,dispid,wFlags,pdispparams,lRepeat,plElapsed)

#define ISystemExemplar_TimeProcessMessage(This,ch,pUbuffer,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeProcessMessage(This,ch,pUbuffer,lRepeat,plElapsed)

#define ISystemExemplar_TimeCloneProperty(This,varData,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeCloneProperty(This,varData,lRepeat,plElapsed)

#define ISystemExemplar_TimeCopyProperty(This,varData,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeCopyProperty(This,varData,lRepeat,plElapsed)

#define ISystemExemplar_TimePutPropertySecurity(This,pSecurity,varData,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimePutPropertySecurity(This,pSecurity,varData,lRepeat,plElapsed)

#define ISystemExemplar_TimeGetPropertyAll(This,varData,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeGetPropertyAll(This,varData,lRepeat,plElapsed)

#define ISystemExemplar_TimePutPropertyAll(This,varData,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimePutPropertyAll(This,varData,lRepeat,plElapsed)

#define ISystemExemplar_TimeInvokeMethodAll(This,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeInvokeMethodAll(This,lRepeat,plElapsed)

#define ISystemExemplar_TimeGetMethodAll(This,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeGetMethodAll(This,lRepeat,plElapsed)

#define ISystemExemplar_TimeGetPropertySecurityAll(This,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeGetPropertySecurityAll(This,lRepeat,plElapsed)

#define ISystemExemplar_TimeCheckPropertySecurityAll(This,ulFlags,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeCheckPropertySecurityAll(This,ulFlags,lRepeat,plElapsed)

#define ISystemExemplar_TimeCheckMethodSecurityAll(This,ulFlags,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeCheckMethodSecurityAll(This,ulFlags,lRepeat,plElapsed)

#define ISystemExemplar_TimeCheckPropertiesSecurityAll(This,ulFlags,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeCheckPropertiesSecurityAll(This,ulFlags,lRepeat,plElapsed)

#define ISystemExemplar_TimeCheckMethodsSecurityAll(This,ulFlags,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeCheckMethodsSecurityAll(This,ulFlags,lRepeat,plElapsed)

#define ISystemExemplar_TimeNotifyAll(This,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeNotifyAll(This,lRepeat,plElapsed)

#define ISystemExemplar_TimeSendCommandAll(This,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeSendCommandAll(This,lRepeat,plElapsed)

#define ISystemExemplar_TimeProcessMessageAll(This,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeProcessMessageAll(This,lRepeat,plElapsed)

#define ISystemExemplar_TimeAll(This,lRepeat,plElapsed)	\
    (This)->lpVtbl -> TimeAll(This,lRepeat,plElapsed)

#define ISystemExemplar_YYY(This)	\
    (This)->lpVtbl -> YYY(This)

#define ISystemExemplar_OnXXXChanged(This,lHint,varHintData)	\
    (This)->lpVtbl -> OnXXXChanged(This,lHint,varHintData)

#define ISystemExemplar_OnXXXAccess(This,lPerms,bReturnValue)	\
    (This)->lpVtbl -> OnXXXAccess(This,lPerms,bReturnValue)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_CreatePropertyList_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [optional][in] */ VARIANT varInitialValue,
    /* [optional][in] */ VARIANT varOrphan,
    /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *pppropertylist);


void __RPC_STUB ISystemExemplar_CreatePropertyList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_CreatePropertyMap_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [optional][in] */ VARIANT varOrphan,
    /* [retval][out] */ IPropertyMap __RPC_FAR *__RPC_FAR *pppropertymap);


void __RPC_STUB ISystemExemplar_CreatePropertyMap_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_CreateMenu_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenu);


void __RPC_STUB ISystemExemplar_CreateMenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_CreateMenuItem_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrMenuText,
    /* [in] */ long lFlags,
    /* [in] */ IThing __RPC_FAR *pOwner,
    /* [in] */ BSTR bstrUpdateMethod,
    /* [in] */ BSTR bstrMethod,
    /* [optional][in] */ VARIANT var,
    /* [retval][out] */ IMenuItem __RPC_FAR *__RPC_FAR *ppMenuItem);


void __RPC_STUB ISystemExemplar_CreateMenuItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_CreateMenuSeparator_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pOwner,
    /* [retval][out] */ IMenuItem __RPC_FAR *__RPC_FAR *ppMenuItem);


void __RPC_STUB ISystemExemplar_CreateMenuSeparator_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_CreateOCXProxy_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IDispatch __RPC_FAR *pOCX,
    /* [retval][out] */ IOCXProxy __RPC_FAR *__RPC_FAR *ppOCXProxy);


void __RPC_STUB ISystemExemplar_CreateOCXProxy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_QueryAvatarNames_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lFalgs,
    /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *pNameList);


void __RPC_STUB ISystemExemplar_QueryAvatarNames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_InvokeRemoteClientMethod_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pDispTarget,
    /* [in] */ BSTR bstrMethodName,
    /* [in] */ VARIANT varUserOrList,
    /* [in] */ VARIANT varParams);


void __RPC_STUB ISystemExemplar_InvokeRemoteClientMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_HandleRemoteClientMethod_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pDispTarget,
    /* [in] */ BSTR bstrMethodName,
    /* [in] */ VARIANT varUserOrList,
    /* [in] */ VARIANT varParams);


void __RPC_STUB ISystemExemplar_HandleRemoteClientMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_FireUIEvent_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IPropertyList __RPC_FAR *pClientList,
    /* [in] */ IThing __RPC_FAR *pSource,
    /* [in] */ BSTR bstrMethodName,
    /* [in] */ VARIANT varParams);


void __RPC_STUB ISystemExemplar_FireUIEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_FireRemoteUIEvent_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ VARIANT varClientList,
    /* [in] */ IThing __RPC_FAR *pSource,
    /* [in] */ BSTR bstrMethodName,
    /* [in] */ VARIANT varParams);


void __RPC_STUB ISystemExemplar_FireRemoteUIEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_OnRootURLChanged_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lHint,
    /* [in] */ VARIANT varHintData);


void __RPC_STUB ISystemExemplar_OnRootURLChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_UploadFile_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrLocalName,
    /* [in] */ BSTR bstrRemoteName,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrURL);


void __RPC_STUB ISystemExemplar_UploadFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_MakeHost_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pTarget);


void __RPC_STUB ISystemExemplar_MakeHost_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_UnMakeHost_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pTarget);


void __RPC_STUB ISystemExemplar_UnMakeHost_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_CreateTimer_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ int msecPeriod,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [in] */ BSTR bstrMethod,
    /* [optional][in] */ VARIANT varArgs,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppTimer);


void __RPC_STUB ISystemExemplar_CreateTimer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_DestroyTimer_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IDispatch __RPC_FAR *pTimer);


void __RPC_STUB ISystemExemplar_DestroyTimer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_OnCreateRoomClient_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing);


void __RPC_STUB ISystemExemplar_OnCreateRoomClient_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_ShowWelcomeMessage_Proxy( 
    ISystemExemplar __RPC_FAR * This);


void __RPC_STUB ISystemExemplar_ShowWelcomeMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_OnWorldOwnerAccess_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lPerms,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue);


void __RPC_STUB ISystemExemplar_OnWorldOwnerAccess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_OnGlobalConstantAccess_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lPerms,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue);


void __RPC_STUB ISystemExemplar_OnGlobalConstantAccess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TraceExemplarInfo_Proxy( 
    ISystemExemplar __RPC_FAR * This);


void __RPC_STUB ISystemExemplar_TraceExemplarInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeGetProperty_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrPropertyName,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeGetProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimePutProperty_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrPropertyName,
    /* [in] */ VARIANT varData,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimePutProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeInvokeMethod_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrMethodName,
    /* [in] */ VARIANT varArgs,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeInvokeMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeGetMethod_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrMethodName,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeGetMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeGetPropertySecurity_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrPropertyName,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeGetPropertySecurity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeCheckPropertySecurity_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrPropertyName,
    /* [in] */ unsigned long ulFlags,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeCheckPropertySecurity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeCheckMethodSecurity_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrMethodName,
    /* [in] */ unsigned long ulFlags,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeCheckMethodSecurity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeCheckPropertiesSecurity_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ unsigned long ulFlags,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeCheckPropertiesSecurity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeCheckMethodsSecurity_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ unsigned long ulFlags,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeCheckMethodsSecurity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeVTableInvokeMethod_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeVTableInvokeMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeAutomationInvokeMethod_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeAutomationInvokeMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeScriptInvokeMethod_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeScriptInvokeMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeInterfaceInvokeMethod_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrMethodName,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeInterfaceInvokeMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeNotifyInvokeMethod_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrMethodName,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeNotifyInvokeMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeNotifyPropertyChanged_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ BSTR bstrPropertyName,
    /* [in] */ long lHint,
    /* [in] */ VARIANT varHintData,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeNotifyPropertyChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeSendThingCommand_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lFlags,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [in] */ DISPID dispid,
    /* [in] */ WORD wFlags,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeSendThingCommand_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeSendObjectPropertyCommand_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lFlags,
    /* [in] */ IObjectProperty __RPC_FAR *pprop,
    /* [in] */ DISPID dispid,
    /* [in] */ WORD wFlags,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeSendObjectPropertyCommand_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeSendWorldCommand_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ DISPID dispid,
    /* [in] */ WORD wFlags,
    /* [in] */ DISPPARAMS __RPC_FAR *pdispparams,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeSendWorldCommand_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeProcessMessage_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ unsigned char ch,
    /* [in] */ IUnMarshallBuffer __RPC_FAR *pUbuffer,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeProcessMessage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeCloneProperty_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ VARIANT varData,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeCloneProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeCopyProperty_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ VARIANT varData,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeCopyProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimePutPropertySecurity_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ IPropertySecurity __RPC_FAR *pSecurity,
    /* [in] */ VARIANT varData,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimePutPropertySecurity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeGetPropertyAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ VARIANT varData,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeGetPropertyAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimePutPropertyAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ VARIANT varData,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimePutPropertyAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeInvokeMethodAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeInvokeMethodAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeGetMethodAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeGetMethodAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeGetPropertySecurityAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeGetPropertySecurityAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeCheckPropertySecurityAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ unsigned long ulFlags,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeCheckPropertySecurityAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeCheckMethodSecurityAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ unsigned long ulFlags,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeCheckMethodSecurityAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeCheckPropertiesSecurityAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ unsigned long ulFlags,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeCheckPropertiesSecurityAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeCheckMethodsSecurityAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ unsigned long ulFlags,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeCheckMethodsSecurityAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeNotifyAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeNotifyAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeSendCommandAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeSendCommandAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][restricted] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeProcessMessageAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeProcessMessageAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_TimeAll_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lRepeat,
    /* [retval][out] */ long __RPC_FAR *plElapsed);


void __RPC_STUB ISystemExemplar_TimeAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_YYY_Proxy( 
    ISystemExemplar __RPC_FAR * This);


void __RPC_STUB ISystemExemplar_YYY_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_OnXXXChanged_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lHint,
    /* [in] */ VARIANT varHintData);


void __RPC_STUB ISystemExemplar_OnXXXChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE ISystemExemplar_OnXXXAccess_Proxy( 
    ISystemExemplar __RPC_FAR * This,
    /* [in] */ long lPerms,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue);


void __RPC_STUB ISystemExemplar_OnXXXAccess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISystemExemplar_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_vwsysex_0257 */
/* [local] */ 

typedef /* [helpstring][uuid] */ 
enum QueryAvatarNamesFlags
    {	qanFlagAll	= 0,
	qanFlagConnected	= 1,
	qanFlagIgnored	= 2,
	qanFlagIgnoring	= 4,
	qanFlagFriend	= 8
    }	QueryAvatarNamesFlags;



extern RPC_IF_HANDLE __MIDL_itf_vwsysex_0257_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_vwsysex_0257_v0_0_s_ifspec;

#ifndef __IThingExemplar_INTERFACE_DEFINED__
#define __IThingExemplar_INTERFACE_DEFINED__

/* interface IThingExemplar */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IThingExemplar;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("99AD3618-AAB5-11D0-BAE3-00C04FC2CAB1")
    IThingExemplar : public IVWDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Report( 
            /* [in] */ BSTR bstrReport) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Announce( 
            /* [in] */ BSTR bstrAnnounce) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnSystemDestroyClient( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnContentSystemEnter( 
            /* [in] */ IThing __RPC_FAR *pWhere,
            /* [in] */ IThing __RPC_FAR *pWhat) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnContentSystemLeave( 
            /* [in] */ IThing __RPC_FAR *pOldWhere,
            /* [in] */ IThing __RPC_FAR *pWhat) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE LookAt( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetMenuList( 
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *pplistMenu) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetMenuItems( 
            /* [in] */ IThing __RPC_FAR *pTarget,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenuList) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetPeerMenuItems( 
            /* [in] */ IThing __RPC_FAR *pTarget,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenuList) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetContainerMenuItems( 
            /* [in] */ IThing __RPC_FAR *pTarget,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenuList) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetContentMenuItems( 
            /* [in] */ IThing __RPC_FAR *pTarget,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenuList) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetSelfMenuItems( 
            /* [in] */ IThing __RPC_FAR *pTarget,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenuList) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetInventoryMenuItems( 
            /* [in] */ IThing __RPC_FAR *pTarget,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenuList) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetEditMenuItems( 
            /* [in] */ IThing __RPC_FAR *pTarget,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenuList) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetMenuItemsFromProperty( 
            /* [in] */ BSTR bstrMenuListProperty,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenuList) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE UpdateIsArtifact( 
            /* [in] */ IMenuItem __RPC_FAR *pmi) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE UpdateIsAvatar( 
            /* [in] */ IMenuItem __RPC_FAR *pmi) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE UpdateIsAvatarAndNotWizard( 
            /* [in] */ IMenuItem __RPC_FAR *pmi) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE UpdateIsArtifactOrAvatar( 
            /* [in] */ IMenuItem __RPC_FAR *pmi) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE UpdateIsTakeable( 
            /* [in] */ IMenuItem __RPC_FAR *pmi) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE UpdatePropertiesMenuItem( 
            /* [in] */ IMenuItem __RPC_FAR *pmi) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE UpdateMenuItemName( 
            /* [in] */ IMenuItem __RPC_FAR *pmi) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE UpdatePropertyListMenu( 
            /* [in] */ IMenuItem __RPC_FAR *pmi,
            /* [in] */ IPropertyList __RPC_FAR *pList) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE UpdateInventoryMenu( 
            /* [in] */ IMenuItem __RPC_FAR *pmi) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE UpdateContentsMenu( 
            /* [in] */ IMenuItem __RPC_FAR *pmi) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE UpdateRoomAvatarsMenu( 
            /* [in] */ IMenuItem __RPC_FAR *pmi) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DoGesture( 
            /* [in] */ int iGesture) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DoNamedGesture( 
            /* [in] */ BSTR bstrGestureName) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DoGestureServer( 
            /* [in] */ IThing __RPC_FAR *pThingOriginator,
            /* [in] */ int iGesture) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DoGestureClient( 
            /* [in] */ IThing __RPC_FAR *pThingOriginator,
            /* [in] */ int iGesture) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AllowEnter( 
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AllowLeave( 
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AcceptEnter( 
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AcceptLeave( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Clone( 
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppNewThing) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Emote( 
            /* [in] */ VARIANT varGesture,
            /* [optional][in] */ VARIANT varText) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GestureText( 
            /* [in] */ BSTR bstrGesture,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrText) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE MoveInto( 
            /* [in] */ IThing __RPC_FAR *pNewContainer) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Say( 
            /* [in] */ BSTR strText,
            /* [optional][in] */ VARIANT userOrList) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Shout( 
            /* [in] */ BSTR strText) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SayGesture( 
            /* [in] */ BSTR strText) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Whisper( 
            /* [in] */ BSTR strText,
            /* [optional][in] */ VARIANT userOrList) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Take( 
            /* [in] */ IThing __RPC_FAR *pThing) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Drop( 
            /* [in] */ IThing __RPC_FAR *pThing) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnIsOpenChanged( 
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE EditObject( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetEditTabMap( 
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *pMap) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetSortedEditTabMap( 
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *pMap) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddClientProperty( 
            /* [in] */ BSTR bstrName,
            /* [optional][in] */ VARIANT var) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RemoveClientProperty( 
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnIsNoticeableAccess( 
            /* [in] */ long lPerms,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnNameAccess( 
            /* [in] */ long lPerms,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnOwnerOrWizardAccess( 
            /* [in] */ long lPerms,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IThingExemplarVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IThingExemplar __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IThingExemplar __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IThingExemplar __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IThingExemplar __RPC_FAR * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Install )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IModule __RPC_FAR *pmodule);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnLoad )( 
            IThingExemplar __RPC_FAR * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUnload )( 
            IThingExemplar __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Report )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrReport);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Announce )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrAnnounce);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnSystemDestroyClient )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnContentSystemEnter )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pWhere,
            /* [in] */ IThing __RPC_FAR *pWhat);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnContentSystemLeave )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pOldWhere,
            /* [in] */ IThing __RPC_FAR *pWhat);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LookAt )( 
            IThingExemplar __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMenuList )( 
            IThingExemplar __RPC_FAR * This,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *pplistMenu);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMenuItems )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pTarget,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenuList);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetPeerMenuItems )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pTarget,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenuList);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetContainerMenuItems )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pTarget,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenuList);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetContentMenuItems )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pTarget,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenuList);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSelfMenuItems )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pTarget,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenuList);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetInventoryMenuItems )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pTarget,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenuList);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetEditMenuItems )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pTarget,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenuList);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMenuItemsFromProperty )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrMenuListProperty,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenuList);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UpdateIsArtifact )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IMenuItem __RPC_FAR *pmi);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UpdateIsAvatar )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IMenuItem __RPC_FAR *pmi);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UpdateIsAvatarAndNotWizard )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IMenuItem __RPC_FAR *pmi);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UpdateIsArtifactOrAvatar )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IMenuItem __RPC_FAR *pmi);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UpdateIsTakeable )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IMenuItem __RPC_FAR *pmi);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UpdatePropertiesMenuItem )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IMenuItem __RPC_FAR *pmi);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UpdateMenuItemName )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IMenuItem __RPC_FAR *pmi);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UpdatePropertyListMenu )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IMenuItem __RPC_FAR *pmi,
            /* [in] */ IPropertyList __RPC_FAR *pList);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UpdateInventoryMenu )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IMenuItem __RPC_FAR *pmi);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UpdateContentsMenu )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IMenuItem __RPC_FAR *pmi);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UpdateRoomAvatarsMenu )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IMenuItem __RPC_FAR *pmi);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DoGesture )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ int iGesture);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DoNamedGesture )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrGestureName);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DoGestureServer )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThingOriginator,
            /* [in] */ int iGesture);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DoGestureClient )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThingOriginator,
            /* [in] */ int iGesture);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AllowEnter )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AllowLeave )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AcceptEnter )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AcceptLeave )( 
            IThingExemplar __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IThingExemplar __RPC_FAR * This,
            /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppNewThing);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Emote )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ VARIANT varGesture,
            /* [optional][in] */ VARIANT varText);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GestureText )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrGesture,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrText);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MoveInto )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pNewContainer);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Say )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ BSTR strText,
            /* [optional][in] */ VARIANT userOrList);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Shout )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ BSTR strText);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SayGesture )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ BSTR strText);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Whisper )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ BSTR strText,
            /* [optional][in] */ VARIANT userOrList);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Take )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Drop )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnIsOpenChanged )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EditObject )( 
            IThingExemplar __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetEditTabMap )( 
            IThingExemplar __RPC_FAR * This,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *pMap);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSortedEditTabMap )( 
            IThingExemplar __RPC_FAR * This,
            /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *pMap);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddClientProperty )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrName,
            /* [optional][in] */ VARIANT var);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveClientProperty )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrName);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnIsNoticeableAccess )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ long lPerms,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnNameAccess )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ long lPerms,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnOwnerOrWizardAccess )( 
            IThingExemplar __RPC_FAR * This,
            /* [in] */ long lPerms,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue);
        
        END_INTERFACE
    } IThingExemplarVtbl;

    interface IThingExemplar
    {
        CONST_VTBL struct IThingExemplarVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IThingExemplar_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IThingExemplar_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IThingExemplar_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IThingExemplar_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IThingExemplar_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IThingExemplar_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IThingExemplar_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IThingExemplar_Initialize(This,pworld)	\
    (This)->lpVtbl -> Initialize(This,pworld)

#define IThingExemplar_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IThingExemplar_Install(This,pmodule)	\
    (This)->lpVtbl -> Install(This,pmodule)

#define IThingExemplar_OnLoad(This)	\
    (This)->lpVtbl -> OnLoad(This)

#define IThingExemplar_OnUnload(This)	\
    (This)->lpVtbl -> OnUnload(This)


#define IThingExemplar_Report(This,bstrReport)	\
    (This)->lpVtbl -> Report(This,bstrReport)

#define IThingExemplar_Announce(This,bstrAnnounce)	\
    (This)->lpVtbl -> Announce(This,bstrAnnounce)

#define IThingExemplar_OnSystemDestroyClient(This,pthing)	\
    (This)->lpVtbl -> OnSystemDestroyClient(This,pthing)

#define IThingExemplar_OnContentSystemEnter(This,pWhere,pWhat)	\
    (This)->lpVtbl -> OnContentSystemEnter(This,pWhere,pWhat)

#define IThingExemplar_OnContentSystemLeave(This,pOldWhere,pWhat)	\
    (This)->lpVtbl -> OnContentSystemLeave(This,pOldWhere,pWhat)

#define IThingExemplar_LookAt(This)	\
    (This)->lpVtbl -> LookAt(This)

#define IThingExemplar_GetMenuList(This,pplistMenu)	\
    (This)->lpVtbl -> GetMenuList(This,pplistMenu)

#define IThingExemplar_GetMenuItems(This,pTarget,ppMenuList)	\
    (This)->lpVtbl -> GetMenuItems(This,pTarget,ppMenuList)

#define IThingExemplar_GetPeerMenuItems(This,pTarget,ppMenuList)	\
    (This)->lpVtbl -> GetPeerMenuItems(This,pTarget,ppMenuList)

#define IThingExemplar_GetContainerMenuItems(This,pTarget,ppMenuList)	\
    (This)->lpVtbl -> GetContainerMenuItems(This,pTarget,ppMenuList)

#define IThingExemplar_GetContentMenuItems(This,pTarget,ppMenuList)	\
    (This)->lpVtbl -> GetContentMenuItems(This,pTarget,ppMenuList)

#define IThingExemplar_GetSelfMenuItems(This,pTarget,ppMenuList)	\
    (This)->lpVtbl -> GetSelfMenuItems(This,pTarget,ppMenuList)

#define IThingExemplar_GetInventoryMenuItems(This,pTarget,ppMenuList)	\
    (This)->lpVtbl -> GetInventoryMenuItems(This,pTarget,ppMenuList)

#define IThingExemplar_GetEditMenuItems(This,pTarget,ppMenuList)	\
    (This)->lpVtbl -> GetEditMenuItems(This,pTarget,ppMenuList)

#define IThingExemplar_GetMenuItemsFromProperty(This,bstrMenuListProperty,ppMenuList)	\
    (This)->lpVtbl -> GetMenuItemsFromProperty(This,bstrMenuListProperty,ppMenuList)

#define IThingExemplar_UpdateIsArtifact(This,pmi)	\
    (This)->lpVtbl -> UpdateIsArtifact(This,pmi)

#define IThingExemplar_UpdateIsAvatar(This,pmi)	\
    (This)->lpVtbl -> UpdateIsAvatar(This,pmi)

#define IThingExemplar_UpdateIsAvatarAndNotWizard(This,pmi)	\
    (This)->lpVtbl -> UpdateIsAvatarAndNotWizard(This,pmi)

#define IThingExemplar_UpdateIsArtifactOrAvatar(This,pmi)	\
    (This)->lpVtbl -> UpdateIsArtifactOrAvatar(This,pmi)

#define IThingExemplar_UpdateIsTakeable(This,pmi)	\
    (This)->lpVtbl -> UpdateIsTakeable(This,pmi)

#define IThingExemplar_UpdatePropertiesMenuItem(This,pmi)	\
    (This)->lpVtbl -> UpdatePropertiesMenuItem(This,pmi)

#define IThingExemplar_UpdateMenuItemName(This,pmi)	\
    (This)->lpVtbl -> UpdateMenuItemName(This,pmi)

#define IThingExemplar_UpdatePropertyListMenu(This,pmi,pList)	\
    (This)->lpVtbl -> UpdatePropertyListMenu(This,pmi,pList)

#define IThingExemplar_UpdateInventoryMenu(This,pmi)	\
    (This)->lpVtbl -> UpdateInventoryMenu(This,pmi)

#define IThingExemplar_UpdateContentsMenu(This,pmi)	\
    (This)->lpVtbl -> UpdateContentsMenu(This,pmi)

#define IThingExemplar_UpdateRoomAvatarsMenu(This,pmi)	\
    (This)->lpVtbl -> UpdateRoomAvatarsMenu(This,pmi)

#define IThingExemplar_DoGesture(This,iGesture)	\
    (This)->lpVtbl -> DoGesture(This,iGesture)

#define IThingExemplar_DoNamedGesture(This,bstrGestureName)	\
    (This)->lpVtbl -> DoNamedGesture(This,bstrGestureName)

#define IThingExemplar_DoGestureServer(This,pThingOriginator,iGesture)	\
    (This)->lpVtbl -> DoGestureServer(This,pThingOriginator,iGesture)

#define IThingExemplar_DoGestureClient(This,pThingOriginator,iGesture)	\
    (This)->lpVtbl -> DoGestureClient(This,pThingOriginator,iGesture)

#define IThingExemplar_AllowEnter(This,pThing,pbool)	\
    (This)->lpVtbl -> AllowEnter(This,pThing,pbool)

#define IThingExemplar_AllowLeave(This,pThing,pbool)	\
    (This)->lpVtbl -> AllowLeave(This,pThing,pbool)

#define IThingExemplar_AcceptEnter(This,pThing,pbool)	\
    (This)->lpVtbl -> AcceptEnter(This,pThing,pbool)

#define IThingExemplar_AcceptLeave(This,pbool)	\
    (This)->lpVtbl -> AcceptLeave(This,pbool)

#define IThingExemplar_Clone(This,ppNewThing)	\
    (This)->lpVtbl -> Clone(This,ppNewThing)

#define IThingExemplar_Emote(This,varGesture,varText)	\
    (This)->lpVtbl -> Emote(This,varGesture,varText)

#define IThingExemplar_GestureText(This,bstrGesture,pbstrText)	\
    (This)->lpVtbl -> GestureText(This,bstrGesture,pbstrText)

#define IThingExemplar_MoveInto(This,pNewContainer)	\
    (This)->lpVtbl -> MoveInto(This,pNewContainer)

#define IThingExemplar_Say(This,strText,userOrList)	\
    (This)->lpVtbl -> Say(This,strText,userOrList)

#define IThingExemplar_Shout(This,strText)	\
    (This)->lpVtbl -> Shout(This,strText)

#define IThingExemplar_SayGesture(This,strText)	\
    (This)->lpVtbl -> SayGesture(This,strText)

#define IThingExemplar_Whisper(This,strText,userOrList)	\
    (This)->lpVtbl -> Whisper(This,strText,userOrList)

#define IThingExemplar_Take(This,pThing)	\
    (This)->lpVtbl -> Take(This,pThing)

#define IThingExemplar_Drop(This,pThing)	\
    (This)->lpVtbl -> Drop(This,pThing)

#define IThingExemplar_OnIsOpenChanged(This,lHint,varHintData)	\
    (This)->lpVtbl -> OnIsOpenChanged(This,lHint,varHintData)

#define IThingExemplar_EditObject(This)	\
    (This)->lpVtbl -> EditObject(This)

#define IThingExemplar_GetEditTabMap(This,pMap)	\
    (This)->lpVtbl -> GetEditTabMap(This,pMap)

#define IThingExemplar_GetSortedEditTabMap(This,pMap)	\
    (This)->lpVtbl -> GetSortedEditTabMap(This,pMap)

#define IThingExemplar_AddClientProperty(This,bstrName,var)	\
    (This)->lpVtbl -> AddClientProperty(This,bstrName,var)

#define IThingExemplar_RemoveClientProperty(This,bstrName)	\
    (This)->lpVtbl -> RemoveClientProperty(This,bstrName)

#define IThingExemplar_OnIsNoticeableAccess(This,lPerms,bReturnValue)	\
    (This)->lpVtbl -> OnIsNoticeableAccess(This,lPerms,bReturnValue)

#define IThingExemplar_OnNameAccess(This,lPerms,bReturnValue)	\
    (This)->lpVtbl -> OnNameAccess(This,lPerms,bReturnValue)

#define IThingExemplar_OnOwnerOrWizardAccess(This,lPerms,bReturnValue)	\
    (This)->lpVtbl -> OnOwnerOrWizardAccess(This,lPerms,bReturnValue)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_Report_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrReport);


void __RPC_STUB IThingExemplar_Report_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_Announce_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrAnnounce);


void __RPC_STUB IThingExemplar_Announce_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IThingExemplar_OnSystemDestroyClient_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IThingExemplar_OnSystemDestroyClient_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IThingExemplar_OnContentSystemEnter_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pWhere,
    /* [in] */ IThing __RPC_FAR *pWhat);


void __RPC_STUB IThingExemplar_OnContentSystemEnter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IThingExemplar_OnContentSystemLeave_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pOldWhere,
    /* [in] */ IThing __RPC_FAR *pWhat);


void __RPC_STUB IThingExemplar_OnContentSystemLeave_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_LookAt_Proxy( 
    IThingExemplar __RPC_FAR * This);


void __RPC_STUB IThingExemplar_LookAt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_GetMenuList_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *pplistMenu);


void __RPC_STUB IThingExemplar_GetMenuList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_GetMenuItems_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pTarget,
    /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenuList);


void __RPC_STUB IThingExemplar_GetMenuItems_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_GetPeerMenuItems_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pTarget,
    /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenuList);


void __RPC_STUB IThingExemplar_GetPeerMenuItems_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_GetContainerMenuItems_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pTarget,
    /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenuList);


void __RPC_STUB IThingExemplar_GetContainerMenuItems_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_GetContentMenuItems_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pTarget,
    /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenuList);


void __RPC_STUB IThingExemplar_GetContentMenuItems_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_GetSelfMenuItems_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pTarget,
    /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenuList);


void __RPC_STUB IThingExemplar_GetSelfMenuItems_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_GetInventoryMenuItems_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pTarget,
    /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenuList);


void __RPC_STUB IThingExemplar_GetInventoryMenuItems_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_GetEditMenuItems_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pTarget,
    /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenuList);


void __RPC_STUB IThingExemplar_GetEditMenuItems_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_GetMenuItemsFromProperty_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrMenuListProperty,
    /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *ppMenuList);


void __RPC_STUB IThingExemplar_GetMenuItemsFromProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_UpdateIsArtifact_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IMenuItem __RPC_FAR *pmi);


void __RPC_STUB IThingExemplar_UpdateIsArtifact_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_UpdateIsAvatar_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IMenuItem __RPC_FAR *pmi);


void __RPC_STUB IThingExemplar_UpdateIsAvatar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_UpdateIsAvatarAndNotWizard_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IMenuItem __RPC_FAR *pmi);


void __RPC_STUB IThingExemplar_UpdateIsAvatarAndNotWizard_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_UpdateIsArtifactOrAvatar_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IMenuItem __RPC_FAR *pmi);


void __RPC_STUB IThingExemplar_UpdateIsArtifactOrAvatar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_UpdateIsTakeable_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IMenuItem __RPC_FAR *pmi);


void __RPC_STUB IThingExemplar_UpdateIsTakeable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_UpdatePropertiesMenuItem_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IMenuItem __RPC_FAR *pmi);


void __RPC_STUB IThingExemplar_UpdatePropertiesMenuItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_UpdateMenuItemName_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IMenuItem __RPC_FAR *pmi);


void __RPC_STUB IThingExemplar_UpdateMenuItemName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_UpdatePropertyListMenu_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IMenuItem __RPC_FAR *pmi,
    /* [in] */ IPropertyList __RPC_FAR *pList);


void __RPC_STUB IThingExemplar_UpdatePropertyListMenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_UpdateInventoryMenu_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IMenuItem __RPC_FAR *pmi);


void __RPC_STUB IThingExemplar_UpdateInventoryMenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_UpdateContentsMenu_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IMenuItem __RPC_FAR *pmi);


void __RPC_STUB IThingExemplar_UpdateContentsMenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_UpdateRoomAvatarsMenu_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IMenuItem __RPC_FAR *pmi);


void __RPC_STUB IThingExemplar_UpdateRoomAvatarsMenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_DoGesture_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ int iGesture);


void __RPC_STUB IThingExemplar_DoGesture_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_DoNamedGesture_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrGestureName);


void __RPC_STUB IThingExemplar_DoNamedGesture_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_DoGestureServer_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThingOriginator,
    /* [in] */ int iGesture);


void __RPC_STUB IThingExemplar_DoGestureServer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_DoGestureClient_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThingOriginator,
    /* [in] */ int iGesture);


void __RPC_STUB IThingExemplar_DoGestureClient_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_AllowEnter_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IThingExemplar_AllowEnter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_AllowLeave_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IThingExemplar_AllowLeave_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_AcceptEnter_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IThingExemplar_AcceptEnter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_AcceptLeave_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IThingExemplar_AcceptLeave_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_Clone_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [retval][out] */ IThing __RPC_FAR *__RPC_FAR *ppNewThing);


void __RPC_STUB IThingExemplar_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_Emote_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ VARIANT varGesture,
    /* [optional][in] */ VARIANT varText);


void __RPC_STUB IThingExemplar_Emote_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_GestureText_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrGesture,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrText);


void __RPC_STUB IThingExemplar_GestureText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_MoveInto_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pNewContainer);


void __RPC_STUB IThingExemplar_MoveInto_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_Say_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ BSTR strText,
    /* [optional][in] */ VARIANT userOrList);


void __RPC_STUB IThingExemplar_Say_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_Shout_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ BSTR strText);


void __RPC_STUB IThingExemplar_Shout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_SayGesture_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ BSTR strText);


void __RPC_STUB IThingExemplar_SayGesture_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_Whisper_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ BSTR strText,
    /* [optional][in] */ VARIANT userOrList);


void __RPC_STUB IThingExemplar_Whisper_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_Take_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing);


void __RPC_STUB IThingExemplar_Take_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_Drop_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing);


void __RPC_STUB IThingExemplar_Drop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_OnIsOpenChanged_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ long lHint,
    /* [in] */ VARIANT varHintData);


void __RPC_STUB IThingExemplar_OnIsOpenChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_EditObject_Proxy( 
    IThingExemplar __RPC_FAR * This);


void __RPC_STUB IThingExemplar_EditObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_GetEditTabMap_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *pMap);


void __RPC_STUB IThingExemplar_GetEditTabMap_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_GetSortedEditTabMap_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [retval][out] */ IPropertyList __RPC_FAR *__RPC_FAR *pMap);


void __RPC_STUB IThingExemplar_GetSortedEditTabMap_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_AddClientProperty_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrName,
    /* [optional][in] */ VARIANT var);


void __RPC_STUB IThingExemplar_AddClientProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_RemoveClientProperty_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrName);


void __RPC_STUB IThingExemplar_RemoveClientProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_OnIsNoticeableAccess_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ long lPerms,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue);


void __RPC_STUB IThingExemplar_OnIsNoticeableAccess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_OnNameAccess_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ long lPerms,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue);


void __RPC_STUB IThingExemplar_OnNameAccess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IThingExemplar_OnOwnerOrWizardAccess_Proxy( 
    IThingExemplar __RPC_FAR * This,
    /* [in] */ long lPerms,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue);


void __RPC_STUB IThingExemplar_OnOwnerOrWizardAccess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IThingExemplar_INTERFACE_DEFINED__ */


#ifndef __IArtifactExemplar_INTERFACE_DEFINED__
#define __IArtifactExemplar_INTERFACE_DEFINED__

/* interface IArtifactExemplar */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IArtifactExemplar;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("99AD361C-AAB5-11D0-BAE3-00C04FC2CAB1")
    IArtifactExemplar : public IVWDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE WieldItem( void) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE UpdateWield( 
            /* [in] */ IMenuItem __RPC_FAR *pmi) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE UnWieldItem( void) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE UpdateUnWield( 
            /* [in] */ IMenuItem __RPC_FAR *pmi) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AcceptLeave( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AcceptEnter( 
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IArtifactExemplarVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IArtifactExemplar __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IArtifactExemplar __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IArtifactExemplar __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IArtifactExemplar __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IArtifactExemplar __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IArtifactExemplar __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IArtifactExemplar __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            IArtifactExemplar __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IArtifactExemplar __RPC_FAR * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Install )( 
            IArtifactExemplar __RPC_FAR * This,
            /* [in] */ IModule __RPC_FAR *pmodule);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnLoad )( 
            IArtifactExemplar __RPC_FAR * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUnload )( 
            IArtifactExemplar __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WieldItem )( 
            IArtifactExemplar __RPC_FAR * This);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UpdateWield )( 
            IArtifactExemplar __RPC_FAR * This,
            /* [in] */ IMenuItem __RPC_FAR *pmi);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnWieldItem )( 
            IArtifactExemplar __RPC_FAR * This);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UpdateUnWield )( 
            IArtifactExemplar __RPC_FAR * This,
            /* [in] */ IMenuItem __RPC_FAR *pmi);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AcceptLeave )( 
            IArtifactExemplar __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AcceptEnter )( 
            IArtifactExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        END_INTERFACE
    } IArtifactExemplarVtbl;

    interface IArtifactExemplar
    {
        CONST_VTBL struct IArtifactExemplarVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IArtifactExemplar_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IArtifactExemplar_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IArtifactExemplar_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IArtifactExemplar_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IArtifactExemplar_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IArtifactExemplar_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IArtifactExemplar_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IArtifactExemplar_Initialize(This,pworld)	\
    (This)->lpVtbl -> Initialize(This,pworld)

#define IArtifactExemplar_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IArtifactExemplar_Install(This,pmodule)	\
    (This)->lpVtbl -> Install(This,pmodule)

#define IArtifactExemplar_OnLoad(This)	\
    (This)->lpVtbl -> OnLoad(This)

#define IArtifactExemplar_OnUnload(This)	\
    (This)->lpVtbl -> OnUnload(This)


#define IArtifactExemplar_WieldItem(This)	\
    (This)->lpVtbl -> WieldItem(This)

#define IArtifactExemplar_UpdateWield(This,pmi)	\
    (This)->lpVtbl -> UpdateWield(This,pmi)

#define IArtifactExemplar_UnWieldItem(This)	\
    (This)->lpVtbl -> UnWieldItem(This)

#define IArtifactExemplar_UpdateUnWield(This,pmi)	\
    (This)->lpVtbl -> UpdateUnWield(This,pmi)

#define IArtifactExemplar_AcceptLeave(This,pbool)	\
    (This)->lpVtbl -> AcceptLeave(This,pbool)

#define IArtifactExemplar_AcceptEnter(This,pThing,pbool)	\
    (This)->lpVtbl -> AcceptEnter(This,pThing,pbool)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IArtifactExemplar_WieldItem_Proxy( 
    IArtifactExemplar __RPC_FAR * This);


void __RPC_STUB IArtifactExemplar_WieldItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IArtifactExemplar_UpdateWield_Proxy( 
    IArtifactExemplar __RPC_FAR * This,
    /* [in] */ IMenuItem __RPC_FAR *pmi);


void __RPC_STUB IArtifactExemplar_UpdateWield_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IArtifactExemplar_UnWieldItem_Proxy( 
    IArtifactExemplar __RPC_FAR * This);


void __RPC_STUB IArtifactExemplar_UnWieldItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IArtifactExemplar_UpdateUnWield_Proxy( 
    IArtifactExemplar __RPC_FAR * This,
    /* [in] */ IMenuItem __RPC_FAR *pmi);


void __RPC_STUB IArtifactExemplar_UpdateUnWield_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IArtifactExemplar_AcceptLeave_Proxy( 
    IArtifactExemplar __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IArtifactExemplar_AcceptLeave_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IArtifactExemplar_AcceptEnter_Proxy( 
    IArtifactExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IArtifactExemplar_AcceptEnter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IArtifactExemplar_INTERFACE_DEFINED__ */


#ifndef __IAvatarExemplar_INTERFACE_DEFINED__
#define __IAvatarExemplar_INTERFACE_DEFINED__

/* interface IAvatarExemplar */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IAvatarExemplar;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("99AD361E-AAB5-11D0-BAE3-00C04FC2CAB1")
    IAvatarExemplar : public IVWDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DoNamedGesture( 
            /* [in] */ BSTR bstrGestureName) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GestureText( 
            /* [in] */ BSTR bstrGesture,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrText) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE LookAt( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE WhisperTo( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE UpdateUserProfileList( 
            /* [in] */ IMenuItem __RPC_FAR *pmi) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE MenuUserProfileList( 
            /* [in] */ BSTR bstrKey) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddToUserProfileList( 
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrKey) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE RemoveFromUserProfileList( 
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrKey) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE IsInUserProfileList( 
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrKey,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pBool) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE UpdateMenuItemName( 
            /* [in] */ IMenuItem __RPC_FAR *pmi) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE UpdateWhisperMenuItem( 
            /* [in] */ IMenuItem __RPC_FAR *pmi) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE EditProfile( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Gesture( 
            /* [in] */ BSTR strGestureName) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Wield( 
            /* [in] */ IThing __RPC_FAR *pThing) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE UnWield( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Teleport( 
            /* [in] */ IThing __RPC_FAR *pNewRoom) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Query( 
            /* [in] */ BSTR bstrTitle,
            /* [in] */ BSTR bstrHTML,
            /* [in] */ BSTR bstrPrompt,
            /* [in] */ BSTR bstrDefault,
            /* [defaultvalue][optional][in] */ VARIANT varNotifyObject,
            /* [defaultvalue][optional][in] */ VARIANT varNotifyMethod,
            /* [defaultvalue][optional][in] */ VARIANT varNotifyParam) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Alert( 
            /* [in] */ BSTR bstrTitle,
            /* [in] */ BSTR bstrText,
            /* [defaultvalue][optional][in] */ VARIANT varNotifyObject,
            /* [defaultvalue][optional][in] */ VARIANT varNotifyMethod,
            /* [defaultvalue][optional][in] */ VARIANT varNotifyParam) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Confirm( 
            /* [in] */ BSTR bstrTitle,
            /* [in] */ BSTR bstrText,
            /* [defaultvalue][optional][in] */ VARIANT varNotifyObject,
            /* [defaultvalue][optional][in] */ VARIANT varNotifyMethod,
            /* [defaultvalue][optional][in] */ VARIANT varNotifyParam) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Prompt( 
            /* [in] */ BSTR bstrTitle,
            /* [in] */ BSTR bstrPrompt,
            /* [in] */ BSTR bstrDefault,
            /* [defaultvalue][optional][in] */ VARIANT varNotifyObject,
            /* [defaultvalue][optional][in] */ VARIANT varNotifyMethod,
            /* [defaultvalue][optional][in] */ VARIANT varNotifyParam) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ClientQuery( 
            /* [in] */ BSTR bstrTitle,
            /* [in] */ BSTR bstrHTML,
            /* [in] */ BSTR bstrPrompt,
            /* [in] */ BSTR bstrDefault,
            /* [in] */ IThing __RPC_FAR *pNotifyObject,
            /* [in] */ BSTR bstrNotifyMethod,
            /* [in] */ IDispatch __RPC_FAR *pNotifyParam) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE QueryReturn( 
            /* [in] */ IPropertyList __RPC_FAR *pArg,
            /* [in] */ VARIANT_BOOL boolOK,
            /* [in] */ BSTR bstrResult) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnSystemCreate( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnSystemConnect( 
            /* [in] */ IThing __RPC_FAR *pWho) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnSystemDisconnect( 
            /* [in] */ IThing __RPC_FAR *pWho) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ThumbnailFileName( 
            /* [retval][out] */ BSTR __RPC_FAR *pbstrFileName) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnContainerSystemTell( 
            /* [in] */ IThing __RPC_FAR *pfrom,
            /* [in] */ IPropertyList __RPC_FAR *pToList,
            /* [in] */ BSTR bstrSay,
            /* [in] */ long lval,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrOut) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnSystemTell( 
            /* [in] */ IThing __RPC_FAR *pfrom,
            /* [in] */ IPropertyList __RPC_FAR *pToList,
            /* [in] */ BSTR bstrSay,
            /* [in] */ long lval,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrOut) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnPeerSystemTell( 
            /* [in] */ IThing __RPC_FAR *pfrom,
            /* [in] */ IPropertyList __RPC_FAR *pToList,
            /* [in] */ BSTR bstrSay,
            /* [in] */ long lval) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnSystemPreCreate( 
            /* [in] */ IThing __RPC_FAR *pExemplar,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbOkToCreate) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnSystemPostCreate( 
            /* [in] */ IThing __RPC_FAR *pThing) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnSystemPreDestroy( 
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbOkToDestroy) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnSystemPostDestroy( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetObjectQuota( 
            /* [in] */ long lQuota) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AddDefaultKeys( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE OnAction( 
            /* [in] */ BSTR bstrAction,
            /* [in] */ BSTR bstrDescription,
            /* [in] */ IPropertyList __RPC_FAR *pParticipants) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnIsAuthorChanged( 
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnIsWizardOrIsAuthorAccess( 
            /* [in] */ long lPerms,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnWizardAccess( 
            /* [in] */ long lPerms,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE FireConnectAvatar( 
            /* [in] */ IThing __RPC_FAR *pthing) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAvatarExemplarVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAvatarExemplar __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAvatarExemplar __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IAvatarExemplar __RPC_FAR * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Install )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ IModule __RPC_FAR *pmodule);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnLoad )( 
            IAvatarExemplar __RPC_FAR * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUnload )( 
            IAvatarExemplar __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DoNamedGesture )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrGestureName);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GestureText )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrGesture,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrText);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LookAt )( 
            IAvatarExemplar __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WhisperTo )( 
            IAvatarExemplar __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UpdateUserProfileList )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ IMenuItem __RPC_FAR *pmi);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MenuUserProfileList )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrKey);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddToUserProfileList )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrKey);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *RemoveFromUserProfileList )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrKey);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *IsInUserProfileList )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ BSTR bstrKey,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pBool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UpdateMenuItemName )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ IMenuItem __RPC_FAR *pmi);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UpdateWhisperMenuItem )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ IMenuItem __RPC_FAR *pmi);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EditProfile )( 
            IAvatarExemplar __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Gesture )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ BSTR strGestureName);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Wield )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *UnWield )( 
            IAvatarExemplar __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Teleport )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pNewRoom);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Query )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrTitle,
            /* [in] */ BSTR bstrHTML,
            /* [in] */ BSTR bstrPrompt,
            /* [in] */ BSTR bstrDefault,
            /* [defaultvalue][optional][in] */ VARIANT varNotifyObject,
            /* [defaultvalue][optional][in] */ VARIANT varNotifyMethod,
            /* [defaultvalue][optional][in] */ VARIANT varNotifyParam);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Alert )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrTitle,
            /* [in] */ BSTR bstrText,
            /* [defaultvalue][optional][in] */ VARIANT varNotifyObject,
            /* [defaultvalue][optional][in] */ VARIANT varNotifyMethod,
            /* [defaultvalue][optional][in] */ VARIANT varNotifyParam);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Confirm )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrTitle,
            /* [in] */ BSTR bstrText,
            /* [defaultvalue][optional][in] */ VARIANT varNotifyObject,
            /* [defaultvalue][optional][in] */ VARIANT varNotifyMethod,
            /* [defaultvalue][optional][in] */ VARIANT varNotifyParam);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Prompt )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrTitle,
            /* [in] */ BSTR bstrPrompt,
            /* [in] */ BSTR bstrDefault,
            /* [defaultvalue][optional][in] */ VARIANT varNotifyObject,
            /* [defaultvalue][optional][in] */ VARIANT varNotifyMethod,
            /* [defaultvalue][optional][in] */ VARIANT varNotifyParam);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ClientQuery )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrTitle,
            /* [in] */ BSTR bstrHTML,
            /* [in] */ BSTR bstrPrompt,
            /* [in] */ BSTR bstrDefault,
            /* [in] */ IThing __RPC_FAR *pNotifyObject,
            /* [in] */ BSTR bstrNotifyMethod,
            /* [in] */ IDispatch __RPC_FAR *pNotifyParam);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryReturn )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ IPropertyList __RPC_FAR *pArg,
            /* [in] */ VARIANT_BOOL boolOK,
            /* [in] */ BSTR bstrResult);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnSystemCreate )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnSystemConnect )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pWho);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnSystemDisconnect )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pWho);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ThumbnailFileName )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrFileName);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnContainerSystemTell )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pfrom,
            /* [in] */ IPropertyList __RPC_FAR *pToList,
            /* [in] */ BSTR bstrSay,
            /* [in] */ long lval,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrOut);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnSystemTell )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pfrom,
            /* [in] */ IPropertyList __RPC_FAR *pToList,
            /* [in] */ BSTR bstrSay,
            /* [in] */ long lval,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrOut);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnPeerSystemTell )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pfrom,
            /* [in] */ IPropertyList __RPC_FAR *pToList,
            /* [in] */ BSTR bstrSay,
            /* [in] */ long lval);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnSystemPreCreate )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pExemplar,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbOkToCreate);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnSystemPostCreate )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnSystemPreDestroy )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbOkToDestroy);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnSystemPostDestroy )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetObjectQuota )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ long lQuota);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddDefaultKeys )( 
            IAvatarExemplar __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnAction )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrAction,
            /* [in] */ BSTR bstrDescription,
            /* [in] */ IPropertyList __RPC_FAR *pParticipants);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnIsAuthorChanged )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ long lHint,
            /* [in] */ VARIANT varHintData);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnIsWizardOrIsAuthorAccess )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ long lPerms,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnWizardAccess )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ long lPerms,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *FireConnectAvatar )( 
            IAvatarExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pthing);
        
        END_INTERFACE
    } IAvatarExemplarVtbl;

    interface IAvatarExemplar
    {
        CONST_VTBL struct IAvatarExemplarVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAvatarExemplar_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAvatarExemplar_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAvatarExemplar_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAvatarExemplar_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAvatarExemplar_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAvatarExemplar_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAvatarExemplar_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IAvatarExemplar_Initialize(This,pworld)	\
    (This)->lpVtbl -> Initialize(This,pworld)

#define IAvatarExemplar_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IAvatarExemplar_Install(This,pmodule)	\
    (This)->lpVtbl -> Install(This,pmodule)

#define IAvatarExemplar_OnLoad(This)	\
    (This)->lpVtbl -> OnLoad(This)

#define IAvatarExemplar_OnUnload(This)	\
    (This)->lpVtbl -> OnUnload(This)


#define IAvatarExemplar_DoNamedGesture(This,bstrGestureName)	\
    (This)->lpVtbl -> DoNamedGesture(This,bstrGestureName)

#define IAvatarExemplar_GestureText(This,bstrGesture,pbstrText)	\
    (This)->lpVtbl -> GestureText(This,bstrGesture,pbstrText)

#define IAvatarExemplar_LookAt(This)	\
    (This)->lpVtbl -> LookAt(This)

#define IAvatarExemplar_WhisperTo(This)	\
    (This)->lpVtbl -> WhisperTo(This)

#define IAvatarExemplar_UpdateUserProfileList(This,pmi)	\
    (This)->lpVtbl -> UpdateUserProfileList(This,pmi)

#define IAvatarExemplar_MenuUserProfileList(This,bstrKey)	\
    (This)->lpVtbl -> MenuUserProfileList(This,bstrKey)

#define IAvatarExemplar_AddToUserProfileList(This,pThing,bstrKey)	\
    (This)->lpVtbl -> AddToUserProfileList(This,pThing,bstrKey)

#define IAvatarExemplar_RemoveFromUserProfileList(This,pThing,bstrKey)	\
    (This)->lpVtbl -> RemoveFromUserProfileList(This,pThing,bstrKey)

#define IAvatarExemplar_IsInUserProfileList(This,pThing,bstrKey,pBool)	\
    (This)->lpVtbl -> IsInUserProfileList(This,pThing,bstrKey,pBool)

#define IAvatarExemplar_UpdateMenuItemName(This,pmi)	\
    (This)->lpVtbl -> UpdateMenuItemName(This,pmi)

#define IAvatarExemplar_UpdateWhisperMenuItem(This,pmi)	\
    (This)->lpVtbl -> UpdateWhisperMenuItem(This,pmi)

#define IAvatarExemplar_EditProfile(This)	\
    (This)->lpVtbl -> EditProfile(This)

#define IAvatarExemplar_Gesture(This,strGestureName)	\
    (This)->lpVtbl -> Gesture(This,strGestureName)

#define IAvatarExemplar_Wield(This,pThing)	\
    (This)->lpVtbl -> Wield(This,pThing)

#define IAvatarExemplar_UnWield(This)	\
    (This)->lpVtbl -> UnWield(This)

#define IAvatarExemplar_Teleport(This,pNewRoom)	\
    (This)->lpVtbl -> Teleport(This,pNewRoom)

#define IAvatarExemplar_Query(This,bstrTitle,bstrHTML,bstrPrompt,bstrDefault,varNotifyObject,varNotifyMethod,varNotifyParam)	\
    (This)->lpVtbl -> Query(This,bstrTitle,bstrHTML,bstrPrompt,bstrDefault,varNotifyObject,varNotifyMethod,varNotifyParam)

#define IAvatarExemplar_Alert(This,bstrTitle,bstrText,varNotifyObject,varNotifyMethod,varNotifyParam)	\
    (This)->lpVtbl -> Alert(This,bstrTitle,bstrText,varNotifyObject,varNotifyMethod,varNotifyParam)

#define IAvatarExemplar_Confirm(This,bstrTitle,bstrText,varNotifyObject,varNotifyMethod,varNotifyParam)	\
    (This)->lpVtbl -> Confirm(This,bstrTitle,bstrText,varNotifyObject,varNotifyMethod,varNotifyParam)

#define IAvatarExemplar_Prompt(This,bstrTitle,bstrPrompt,bstrDefault,varNotifyObject,varNotifyMethod,varNotifyParam)	\
    (This)->lpVtbl -> Prompt(This,bstrTitle,bstrPrompt,bstrDefault,varNotifyObject,varNotifyMethod,varNotifyParam)

#define IAvatarExemplar_ClientQuery(This,bstrTitle,bstrHTML,bstrPrompt,bstrDefault,pNotifyObject,bstrNotifyMethod,pNotifyParam)	\
    (This)->lpVtbl -> ClientQuery(This,bstrTitle,bstrHTML,bstrPrompt,bstrDefault,pNotifyObject,bstrNotifyMethod,pNotifyParam)

#define IAvatarExemplar_QueryReturn(This,pArg,boolOK,bstrResult)	\
    (This)->lpVtbl -> QueryReturn(This,pArg,boolOK,bstrResult)

#define IAvatarExemplar_OnSystemCreate(This,pthing)	\
    (This)->lpVtbl -> OnSystemCreate(This,pthing)

#define IAvatarExemplar_OnSystemConnect(This,pWho)	\
    (This)->lpVtbl -> OnSystemConnect(This,pWho)

#define IAvatarExemplar_OnSystemDisconnect(This,pWho)	\
    (This)->lpVtbl -> OnSystemDisconnect(This,pWho)

#define IAvatarExemplar_ThumbnailFileName(This,pbstrFileName)	\
    (This)->lpVtbl -> ThumbnailFileName(This,pbstrFileName)

#define IAvatarExemplar_OnContainerSystemTell(This,pfrom,pToList,bstrSay,lval,pbstrOut)	\
    (This)->lpVtbl -> OnContainerSystemTell(This,pfrom,pToList,bstrSay,lval,pbstrOut)

#define IAvatarExemplar_OnSystemTell(This,pfrom,pToList,bstrSay,lval,pbstrOut)	\
    (This)->lpVtbl -> OnSystemTell(This,pfrom,pToList,bstrSay,lval,pbstrOut)

#define IAvatarExemplar_OnPeerSystemTell(This,pfrom,pToList,bstrSay,lval)	\
    (This)->lpVtbl -> OnPeerSystemTell(This,pfrom,pToList,bstrSay,lval)

#define IAvatarExemplar_OnSystemPreCreate(This,pExemplar,pbOkToCreate)	\
    (This)->lpVtbl -> OnSystemPreCreate(This,pExemplar,pbOkToCreate)

#define IAvatarExemplar_OnSystemPostCreate(This,pThing)	\
    (This)->lpVtbl -> OnSystemPostCreate(This,pThing)

#define IAvatarExemplar_OnSystemPreDestroy(This,pthing,pbOkToDestroy)	\
    (This)->lpVtbl -> OnSystemPreDestroy(This,pthing,pbOkToDestroy)

#define IAvatarExemplar_OnSystemPostDestroy(This,pthing)	\
    (This)->lpVtbl -> OnSystemPostDestroy(This,pthing)

#define IAvatarExemplar_SetObjectQuota(This,lQuota)	\
    (This)->lpVtbl -> SetObjectQuota(This,lQuota)

#define IAvatarExemplar_AddDefaultKeys(This)	\
    (This)->lpVtbl -> AddDefaultKeys(This)

#define IAvatarExemplar_OnAction(This,bstrAction,bstrDescription,pParticipants)	\
    (This)->lpVtbl -> OnAction(This,bstrAction,bstrDescription,pParticipants)

#define IAvatarExemplar_OnIsAuthorChanged(This,lHint,varHintData)	\
    (This)->lpVtbl -> OnIsAuthorChanged(This,lHint,varHintData)

#define IAvatarExemplar_OnIsWizardOrIsAuthorAccess(This,lPerms,bReturnValue)	\
    (This)->lpVtbl -> OnIsWizardOrIsAuthorAccess(This,lPerms,bReturnValue)

#define IAvatarExemplar_OnWizardAccess(This,lPerms,bReturnValue)	\
    (This)->lpVtbl -> OnWizardAccess(This,lPerms,bReturnValue)

#define IAvatarExemplar_FireConnectAvatar(This,pthing)	\
    (This)->lpVtbl -> FireConnectAvatar(This,pthing)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_DoNamedGesture_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrGestureName);


void __RPC_STUB IAvatarExemplar_DoNamedGesture_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_GestureText_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrGesture,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrText);


void __RPC_STUB IAvatarExemplar_GestureText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_LookAt_Proxy( 
    IAvatarExemplar __RPC_FAR * This);


void __RPC_STUB IAvatarExemplar_LookAt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_WhisperTo_Proxy( 
    IAvatarExemplar __RPC_FAR * This);


void __RPC_STUB IAvatarExemplar_WhisperTo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_UpdateUserProfileList_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IMenuItem __RPC_FAR *pmi);


void __RPC_STUB IAvatarExemplar_UpdateUserProfileList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_MenuUserProfileList_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrKey);


void __RPC_STUB IAvatarExemplar_MenuUserProfileList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_AddToUserProfileList_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [in] */ BSTR bstrKey);


void __RPC_STUB IAvatarExemplar_AddToUserProfileList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_RemoveFromUserProfileList_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [in] */ BSTR bstrKey);


void __RPC_STUB IAvatarExemplar_RemoveFromUserProfileList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_IsInUserProfileList_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [in] */ BSTR bstrKey,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pBool);


void __RPC_STUB IAvatarExemplar_IsInUserProfileList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_UpdateMenuItemName_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IMenuItem __RPC_FAR *pmi);


void __RPC_STUB IAvatarExemplar_UpdateMenuItemName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_UpdateWhisperMenuItem_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IMenuItem __RPC_FAR *pmi);


void __RPC_STUB IAvatarExemplar_UpdateWhisperMenuItem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_EditProfile_Proxy( 
    IAvatarExemplar __RPC_FAR * This);


void __RPC_STUB IAvatarExemplar_EditProfile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_Gesture_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ BSTR strGestureName);


void __RPC_STUB IAvatarExemplar_Gesture_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_Wield_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing);


void __RPC_STUB IAvatarExemplar_Wield_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_UnWield_Proxy( 
    IAvatarExemplar __RPC_FAR * This);


void __RPC_STUB IAvatarExemplar_UnWield_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_Teleport_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pNewRoom);


void __RPC_STUB IAvatarExemplar_Teleport_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_Query_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrTitle,
    /* [in] */ BSTR bstrHTML,
    /* [in] */ BSTR bstrPrompt,
    /* [in] */ BSTR bstrDefault,
    /* [defaultvalue][optional][in] */ VARIANT varNotifyObject,
    /* [defaultvalue][optional][in] */ VARIANT varNotifyMethod,
    /* [defaultvalue][optional][in] */ VARIANT varNotifyParam);


void __RPC_STUB IAvatarExemplar_Query_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_Alert_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrTitle,
    /* [in] */ BSTR bstrText,
    /* [defaultvalue][optional][in] */ VARIANT varNotifyObject,
    /* [defaultvalue][optional][in] */ VARIANT varNotifyMethod,
    /* [defaultvalue][optional][in] */ VARIANT varNotifyParam);


void __RPC_STUB IAvatarExemplar_Alert_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_Confirm_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrTitle,
    /* [in] */ BSTR bstrText,
    /* [defaultvalue][optional][in] */ VARIANT varNotifyObject,
    /* [defaultvalue][optional][in] */ VARIANT varNotifyMethod,
    /* [defaultvalue][optional][in] */ VARIANT varNotifyParam);


void __RPC_STUB IAvatarExemplar_Confirm_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_Prompt_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrTitle,
    /* [in] */ BSTR bstrPrompt,
    /* [in] */ BSTR bstrDefault,
    /* [defaultvalue][optional][in] */ VARIANT varNotifyObject,
    /* [defaultvalue][optional][in] */ VARIANT varNotifyMethod,
    /* [defaultvalue][optional][in] */ VARIANT varNotifyParam);


void __RPC_STUB IAvatarExemplar_Prompt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_ClientQuery_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrTitle,
    /* [in] */ BSTR bstrHTML,
    /* [in] */ BSTR bstrPrompt,
    /* [in] */ BSTR bstrDefault,
    /* [in] */ IThing __RPC_FAR *pNotifyObject,
    /* [in] */ BSTR bstrNotifyMethod,
    /* [in] */ IDispatch __RPC_FAR *pNotifyParam);


void __RPC_STUB IAvatarExemplar_ClientQuery_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_QueryReturn_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IPropertyList __RPC_FAR *pArg,
    /* [in] */ VARIANT_BOOL boolOK,
    /* [in] */ BSTR bstrResult);


void __RPC_STUB IAvatarExemplar_QueryReturn_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnSystemCreate_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IAvatarExemplar_OnSystemCreate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnSystemConnect_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pWho);


void __RPC_STUB IAvatarExemplar_OnSystemConnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnSystemDisconnect_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pWho);


void __RPC_STUB IAvatarExemplar_OnSystemDisconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_ThumbnailFileName_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrFileName);


void __RPC_STUB IAvatarExemplar_ThumbnailFileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnContainerSystemTell_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pfrom,
    /* [in] */ IPropertyList __RPC_FAR *pToList,
    /* [in] */ BSTR bstrSay,
    /* [in] */ long lval,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrOut);


void __RPC_STUB IAvatarExemplar_OnContainerSystemTell_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnSystemTell_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pfrom,
    /* [in] */ IPropertyList __RPC_FAR *pToList,
    /* [in] */ BSTR bstrSay,
    /* [in] */ long lval,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrOut);


void __RPC_STUB IAvatarExemplar_OnSystemTell_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnPeerSystemTell_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pfrom,
    /* [in] */ IPropertyList __RPC_FAR *pToList,
    /* [in] */ BSTR bstrSay,
    /* [in] */ long lval);


void __RPC_STUB IAvatarExemplar_OnPeerSystemTell_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnSystemPreCreate_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pExemplar,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbOkToCreate);


void __RPC_STUB IAvatarExemplar_OnSystemPreCreate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnSystemPostCreate_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing);


void __RPC_STUB IAvatarExemplar_OnSystemPostCreate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnSystemPreDestroy_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbOkToDestroy);


void __RPC_STUB IAvatarExemplar_OnSystemPreDestroy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnSystemPostDestroy_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IAvatarExemplar_OnSystemPostDestroy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_SetObjectQuota_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ long lQuota);


void __RPC_STUB IAvatarExemplar_SetObjectQuota_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_AddDefaultKeys_Proxy( 
    IAvatarExemplar __RPC_FAR * This);


void __RPC_STUB IAvatarExemplar_AddDefaultKeys_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnAction_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrAction,
    /* [in] */ BSTR bstrDescription,
    /* [in] */ IPropertyList __RPC_FAR *pParticipants);


void __RPC_STUB IAvatarExemplar_OnAction_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnIsAuthorChanged_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ long lHint,
    /* [in] */ VARIANT varHintData);


void __RPC_STUB IAvatarExemplar_OnIsAuthorChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnIsWizardOrIsAuthorAccess_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ long lPerms,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue);


void __RPC_STUB IAvatarExemplar_OnIsWizardOrIsAuthorAccess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_OnWizardAccess_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ long lPerms,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *bReturnValue);


void __RPC_STUB IAvatarExemplar_OnWizardAccess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IAvatarExemplar_FireConnectAvatar_Proxy( 
    IAvatarExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pthing);


void __RPC_STUB IAvatarExemplar_FireConnectAvatar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAvatarExemplar_INTERFACE_DEFINED__ */


#ifndef __IHostExemplar_INTERFACE_DEFINED__
#define __IHostExemplar_INTERFACE_DEFINED__

/* interface IHostExemplar */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IHostExemplar;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("44001330-1B82-11d2-AC71-00104B26A373")
    IHostExemplar : public IVWDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ShowLogOffDialog( 
            /* [in] */ IThing __RPC_FAR *pTarget) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE LogOff( 
            /* [in] */ IThing __RPC_FAR *pTarget,
            /* [in] */ BSTR bstrReason) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IHostExemplarVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IHostExemplar __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IHostExemplar __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IHostExemplar __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IHostExemplar __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IHostExemplar __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IHostExemplar __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IHostExemplar __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            IHostExemplar __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IHostExemplar __RPC_FAR * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Install )( 
            IHostExemplar __RPC_FAR * This,
            /* [in] */ IModule __RPC_FAR *pmodule);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnLoad )( 
            IHostExemplar __RPC_FAR * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUnload )( 
            IHostExemplar __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ShowLogOffDialog )( 
            IHostExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pTarget);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *LogOff )( 
            IHostExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pTarget,
            /* [in] */ BSTR bstrReason);
        
        END_INTERFACE
    } IHostExemplarVtbl;

    interface IHostExemplar
    {
        CONST_VTBL struct IHostExemplarVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHostExemplar_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IHostExemplar_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IHostExemplar_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IHostExemplar_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IHostExemplar_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IHostExemplar_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IHostExemplar_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IHostExemplar_Initialize(This,pworld)	\
    (This)->lpVtbl -> Initialize(This,pworld)

#define IHostExemplar_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IHostExemplar_Install(This,pmodule)	\
    (This)->lpVtbl -> Install(This,pmodule)

#define IHostExemplar_OnLoad(This)	\
    (This)->lpVtbl -> OnLoad(This)

#define IHostExemplar_OnUnload(This)	\
    (This)->lpVtbl -> OnUnload(This)


#define IHostExemplar_ShowLogOffDialog(This,pTarget)	\
    (This)->lpVtbl -> ShowLogOffDialog(This,pTarget)

#define IHostExemplar_LogOff(This,pTarget,bstrReason)	\
    (This)->lpVtbl -> LogOff(This,pTarget,bstrReason)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IHostExemplar_ShowLogOffDialog_Proxy( 
    IHostExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pTarget);


void __RPC_STUB IHostExemplar_ShowLogOffDialog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IHostExemplar_LogOff_Proxy( 
    IHostExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pTarget,
    /* [in] */ BSTR bstrReason);


void __RPC_STUB IHostExemplar_LogOff_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IHostExemplar_INTERFACE_DEFINED__ */


#ifndef __IPortalExemplar_INTERFACE_DEFINED__
#define __IPortalExemplar_INTERFACE_DEFINED__

/* interface IPortalExemplar */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPortalExemplar;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("99AD3624-AAB5-11D0-BAE3-00C04FC2CAB1")
    IPortalExemplar : public IVWDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ShowTransportDialog( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE TransportMenu( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Transport( 
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [defaultvalue][optional][in] */ int iIndex = 0) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE TransportTo( 
            /* [in] */ IThing __RPC_FAR *pThing) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AllowTransport( 
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AcceptTransport( 
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetDestinationByID( 
            /* [in] */ long lNewID) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPortalExemplarVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPortalExemplar __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPortalExemplar __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPortalExemplar __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IPortalExemplar __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IPortalExemplar __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IPortalExemplar __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IPortalExemplar __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            IPortalExemplar __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IPortalExemplar __RPC_FAR * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Install )( 
            IPortalExemplar __RPC_FAR * This,
            /* [in] */ IModule __RPC_FAR *pmodule);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnLoad )( 
            IPortalExemplar __RPC_FAR * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUnload )( 
            IPortalExemplar __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ShowTransportDialog )( 
            IPortalExemplar __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TransportMenu )( 
            IPortalExemplar __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Transport )( 
            IPortalExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [defaultvalue][optional][in] */ int iIndex);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TransportTo )( 
            IPortalExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AllowTransport )( 
            IPortalExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AcceptTransport )( 
            IPortalExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDestinationByID )( 
            IPortalExemplar __RPC_FAR * This,
            /* [in] */ long lNewID);
        
        END_INTERFACE
    } IPortalExemplarVtbl;

    interface IPortalExemplar
    {
        CONST_VTBL struct IPortalExemplarVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPortalExemplar_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPortalExemplar_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPortalExemplar_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPortalExemplar_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPortalExemplar_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPortalExemplar_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPortalExemplar_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPortalExemplar_Initialize(This,pworld)	\
    (This)->lpVtbl -> Initialize(This,pworld)

#define IPortalExemplar_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IPortalExemplar_Install(This,pmodule)	\
    (This)->lpVtbl -> Install(This,pmodule)

#define IPortalExemplar_OnLoad(This)	\
    (This)->lpVtbl -> OnLoad(This)

#define IPortalExemplar_OnUnload(This)	\
    (This)->lpVtbl -> OnUnload(This)


#define IPortalExemplar_ShowTransportDialog(This)	\
    (This)->lpVtbl -> ShowTransportDialog(This)

#define IPortalExemplar_TransportMenu(This)	\
    (This)->lpVtbl -> TransportMenu(This)

#define IPortalExemplar_Transport(This,pThing,iIndex)	\
    (This)->lpVtbl -> Transport(This,pThing,iIndex)

#define IPortalExemplar_TransportTo(This,pThing)	\
    (This)->lpVtbl -> TransportTo(This,pThing)

#define IPortalExemplar_AllowTransport(This,pThing,pbool)	\
    (This)->lpVtbl -> AllowTransport(This,pThing,pbool)

#define IPortalExemplar_AcceptTransport(This,pThing,pbool)	\
    (This)->lpVtbl -> AcceptTransport(This,pThing,pbool)

#define IPortalExemplar_SetDestinationByID(This,lNewID)	\
    (This)->lpVtbl -> SetDestinationByID(This,lNewID)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IPortalExemplar_ShowTransportDialog_Proxy( 
    IPortalExemplar __RPC_FAR * This);


void __RPC_STUB IPortalExemplar_ShowTransportDialog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IPortalExemplar_TransportMenu_Proxy( 
    IPortalExemplar __RPC_FAR * This);


void __RPC_STUB IPortalExemplar_TransportMenu_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IPortalExemplar_Transport_Proxy( 
    IPortalExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [defaultvalue][optional][in] */ int iIndex);


void __RPC_STUB IPortalExemplar_Transport_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IPortalExemplar_TransportTo_Proxy( 
    IPortalExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing);


void __RPC_STUB IPortalExemplar_TransportTo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IPortalExemplar_AllowTransport_Proxy( 
    IPortalExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IPortalExemplar_AllowTransport_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IPortalExemplar_AcceptTransport_Proxy( 
    IPortalExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IPortalExemplar_AcceptTransport_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IPortalExemplar_SetDestinationByID_Proxy( 
    IPortalExemplar __RPC_FAR * This,
    /* [in] */ long lNewID);


void __RPC_STUB IPortalExemplar_SetDestinationByID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPortalExemplar_INTERFACE_DEFINED__ */


#ifndef __IIWPortalExemplar_INTERFACE_DEFINED__
#define __IIWPortalExemplar_INTERFACE_DEFINED__

/* interface IIWPortalExemplar */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IIWPortalExemplar;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FFA01B4F-9F4C-11d1-80E8-00C04FC2A129")
    IIWPortalExemplar : public IVWDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Transport( 
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [defaultvalue][optional][in] */ int iIndex = 0) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IIWPortalExemplarVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IIWPortalExemplar __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IIWPortalExemplar __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IIWPortalExemplar __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IIWPortalExemplar __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IIWPortalExemplar __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IIWPortalExemplar __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IIWPortalExemplar __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            IIWPortalExemplar __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IIWPortalExemplar __RPC_FAR * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Install )( 
            IIWPortalExemplar __RPC_FAR * This,
            /* [in] */ IModule __RPC_FAR *pmodule);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnLoad )( 
            IIWPortalExemplar __RPC_FAR * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUnload )( 
            IIWPortalExemplar __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Transport )( 
            IIWPortalExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [defaultvalue][optional][in] */ int iIndex);
        
        END_INTERFACE
    } IIWPortalExemplarVtbl;

    interface IIWPortalExemplar
    {
        CONST_VTBL struct IIWPortalExemplarVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IIWPortalExemplar_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IIWPortalExemplar_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IIWPortalExemplar_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IIWPortalExemplar_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IIWPortalExemplar_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IIWPortalExemplar_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IIWPortalExemplar_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IIWPortalExemplar_Initialize(This,pworld)	\
    (This)->lpVtbl -> Initialize(This,pworld)

#define IIWPortalExemplar_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IIWPortalExemplar_Install(This,pmodule)	\
    (This)->lpVtbl -> Install(This,pmodule)

#define IIWPortalExemplar_OnLoad(This)	\
    (This)->lpVtbl -> OnLoad(This)

#define IIWPortalExemplar_OnUnload(This)	\
    (This)->lpVtbl -> OnUnload(This)


#define IIWPortalExemplar_Transport(This,pThing,iIndex)	\
    (This)->lpVtbl -> Transport(This,pThing,iIndex)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IIWPortalExemplar_Transport_Proxy( 
    IIWPortalExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [defaultvalue][optional][in] */ int iIndex);


void __RPC_STUB IIWPortalExemplar_Transport_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IIWPortalExemplar_INTERFACE_DEFINED__ */


#ifndef __IRoomExemplar_INTERFACE_DEFINED__
#define __IRoomExemplar_INTERFACE_DEFINED__

/* interface IRoomExemplar */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IRoomExemplar;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("99AD3626-AAB5-11D0-BAE3-00C04FC2CAB1")
    IRoomExemplar : public IVWDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Expel( 
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ IThing __RPC_FAR *pDoor,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pBool) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE TransmitWhisper( 
            /* [in] */ BSTR bstrMsg,
            /* [in] */ VARIANT varNameList) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ReceiveWhisper( 
            /* [in] */ BSTR bstrMsg,
            /* [in] */ IPropertyList __RPC_FAR *pTargetList) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AllowEnter( 
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE AllowLeave( 
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnSystemCreate( 
            /* [in] */ IThing __RPC_FAR *pThing) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnSystemDestroy( 
            /* [in] */ IThing __RPC_FAR *pThing) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnContentSystemConnect( 
            /* [in] */ IThing __RPC_FAR *pWho) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnContentSystemDisconnect( 
            /* [in] */ IThing __RPC_FAR *pWho) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnContentSystemTell( 
            /* [in] */ IThing __RPC_FAR *pfrom,
            /* [in] */ IPropertyList __RPC_FAR *pToList,
            /* [in] */ BSTR bstrSay,
            /* [in] */ long lval,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrOut) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnContentEnter( 
            /* [in] */ IThing __RPC_FAR *pWhere,
            /* [in] */ IThing __RPC_FAR *pWhat) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnContentLeave( 
            /* [in] */ IThing __RPC_FAR *pOldWhere,
            /* [in] */ IThing __RPC_FAR *pWhat) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRoomExemplarVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IRoomExemplar __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IRoomExemplar __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IRoomExemplar __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IRoomExemplar __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IRoomExemplar __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IRoomExemplar __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IRoomExemplar __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            IRoomExemplar __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IRoomExemplar __RPC_FAR * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Install )( 
            IRoomExemplar __RPC_FAR * This,
            /* [in] */ IModule __RPC_FAR *pmodule);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnLoad )( 
            IRoomExemplar __RPC_FAR * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUnload )( 
            IRoomExemplar __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Expel )( 
            IRoomExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [in] */ IThing __RPC_FAR *pDoor,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pBool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *TransmitWhisper )( 
            IRoomExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrMsg,
            /* [in] */ VARIANT varNameList);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReceiveWhisper )( 
            IRoomExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrMsg,
            /* [in] */ IPropertyList __RPC_FAR *pTargetList);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AllowEnter )( 
            IRoomExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AllowLeave )( 
            IRoomExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnSystemCreate )( 
            IRoomExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnSystemDestroy )( 
            IRoomExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pThing);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnContentSystemConnect )( 
            IRoomExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pWho);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnContentSystemDisconnect )( 
            IRoomExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pWho);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnContentSystemTell )( 
            IRoomExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pfrom,
            /* [in] */ IPropertyList __RPC_FAR *pToList,
            /* [in] */ BSTR bstrSay,
            /* [in] */ long lval,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrOut);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnContentEnter )( 
            IRoomExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pWhere,
            /* [in] */ IThing __RPC_FAR *pWhat);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnContentLeave )( 
            IRoomExemplar __RPC_FAR * This,
            /* [in] */ IThing __RPC_FAR *pOldWhere,
            /* [in] */ IThing __RPC_FAR *pWhat);
        
        END_INTERFACE
    } IRoomExemplarVtbl;

    interface IRoomExemplar
    {
        CONST_VTBL struct IRoomExemplarVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRoomExemplar_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRoomExemplar_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRoomExemplar_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRoomExemplar_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IRoomExemplar_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IRoomExemplar_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IRoomExemplar_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IRoomExemplar_Initialize(This,pworld)	\
    (This)->lpVtbl -> Initialize(This,pworld)

#define IRoomExemplar_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IRoomExemplar_Install(This,pmodule)	\
    (This)->lpVtbl -> Install(This,pmodule)

#define IRoomExemplar_OnLoad(This)	\
    (This)->lpVtbl -> OnLoad(This)

#define IRoomExemplar_OnUnload(This)	\
    (This)->lpVtbl -> OnUnload(This)


#define IRoomExemplar_Expel(This,pThing,pDoor,pBool)	\
    (This)->lpVtbl -> Expel(This,pThing,pDoor,pBool)

#define IRoomExemplar_TransmitWhisper(This,bstrMsg,varNameList)	\
    (This)->lpVtbl -> TransmitWhisper(This,bstrMsg,varNameList)

#define IRoomExemplar_ReceiveWhisper(This,bstrMsg,pTargetList)	\
    (This)->lpVtbl -> ReceiveWhisper(This,bstrMsg,pTargetList)

#define IRoomExemplar_AllowEnter(This,pThing,pbool)	\
    (This)->lpVtbl -> AllowEnter(This,pThing,pbool)

#define IRoomExemplar_AllowLeave(This,pThing,pbool)	\
    (This)->lpVtbl -> AllowLeave(This,pThing,pbool)

#define IRoomExemplar_OnSystemCreate(This,pThing)	\
    (This)->lpVtbl -> OnSystemCreate(This,pThing)

#define IRoomExemplar_OnSystemDestroy(This,pThing)	\
    (This)->lpVtbl -> OnSystemDestroy(This,pThing)

#define IRoomExemplar_OnContentSystemConnect(This,pWho)	\
    (This)->lpVtbl -> OnContentSystemConnect(This,pWho)

#define IRoomExemplar_OnContentSystemDisconnect(This,pWho)	\
    (This)->lpVtbl -> OnContentSystemDisconnect(This,pWho)

#define IRoomExemplar_OnContentSystemTell(This,pfrom,pToList,bstrSay,lval,pbstrOut)	\
    (This)->lpVtbl -> OnContentSystemTell(This,pfrom,pToList,bstrSay,lval,pbstrOut)

#define IRoomExemplar_OnContentEnter(This,pWhere,pWhat)	\
    (This)->lpVtbl -> OnContentEnter(This,pWhere,pWhat)

#define IRoomExemplar_OnContentLeave(This,pOldWhere,pWhat)	\
    (This)->lpVtbl -> OnContentLeave(This,pOldWhere,pWhat)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IRoomExemplar_Expel_Proxy( 
    IRoomExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [in] */ IThing __RPC_FAR *pDoor,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pBool);


void __RPC_STUB IRoomExemplar_Expel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IRoomExemplar_TransmitWhisper_Proxy( 
    IRoomExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrMsg,
    /* [in] */ VARIANT varNameList);


void __RPC_STUB IRoomExemplar_TransmitWhisper_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IRoomExemplar_ReceiveWhisper_Proxy( 
    IRoomExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrMsg,
    /* [in] */ IPropertyList __RPC_FAR *pTargetList);


void __RPC_STUB IRoomExemplar_ReceiveWhisper_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IRoomExemplar_AllowEnter_Proxy( 
    IRoomExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IRoomExemplar_AllowEnter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IRoomExemplar_AllowLeave_Proxy( 
    IRoomExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IRoomExemplar_AllowLeave_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IRoomExemplar_OnSystemCreate_Proxy( 
    IRoomExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing);


void __RPC_STUB IRoomExemplar_OnSystemCreate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IRoomExemplar_OnSystemDestroy_Proxy( 
    IRoomExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pThing);


void __RPC_STUB IRoomExemplar_OnSystemDestroy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IRoomExemplar_OnContentSystemConnect_Proxy( 
    IRoomExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pWho);


void __RPC_STUB IRoomExemplar_OnContentSystemConnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IRoomExemplar_OnContentSystemDisconnect_Proxy( 
    IRoomExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pWho);


void __RPC_STUB IRoomExemplar_OnContentSystemDisconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IRoomExemplar_OnContentSystemTell_Proxy( 
    IRoomExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pfrom,
    /* [in] */ IPropertyList __RPC_FAR *pToList,
    /* [in] */ BSTR bstrSay,
    /* [in] */ long lval,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrOut);


void __RPC_STUB IRoomExemplar_OnContentSystemTell_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IRoomExemplar_OnContentEnter_Proxy( 
    IRoomExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pWhere,
    /* [in] */ IThing __RPC_FAR *pWhat);


void __RPC_STUB IRoomExemplar_OnContentEnter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IRoomExemplar_OnContentLeave_Proxy( 
    IRoomExemplar __RPC_FAR * This,
    /* [in] */ IThing __RPC_FAR *pOldWhere,
    /* [in] */ IThing __RPC_FAR *pWhat);


void __RPC_STUB IRoomExemplar_OnContentLeave_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRoomExemplar_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_vwsysex_0264 */
/* [local] */ 

typedef /* [helpstring][uuid] */ 
enum TellConstant
    {	tcSay	= 0,
	tcEmote	= 1,
	tcThink	= 2,
	tcWhisper	= 3,
	tcAnnounce	= 4,
	tcGesture	= 5,
	tcShout	= 6,
	tcReport	= 7,
	tcAction	= 8
    }	TellConstant;



extern RPC_IF_HANDLE __MIDL_itf_vwsysex_0264_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_vwsysex_0264_v0_0_s_ifspec;

#ifndef __IStateMachineExemplar_INTERFACE_DEFINED__
#define __IStateMachineExemplar_INTERFACE_DEFINED__

/* interface IStateMachineExemplar */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IStateMachineExemplar;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("69A2A200-F187-11d1-A392-0000F807558D")
    IStateMachineExemplar : public IVWDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SMchAddStateMachine( 
            /* [in] */ BSTR bstrStateMachineName,
            /* [in] */ BSTR bstrInitialValue) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SMchSetState( 
            /* [in] */ BSTR bstrState) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SMchSetStateMachineState( 
            /* [in] */ BSTR bstrStateMachineName,
            /* [in] */ BSTR bstrState) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SMchIsInState( 
            /* [in] */ BSTR bstrState,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolResult) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SMchIsStateMachineInState( 
            /* [in] */ BSTR bstrStateMachineName,
            /* [in] */ BSTR bstrState,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolResult) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SMchSetIfState( 
            /* [in] */ BSTR bstrOldState,
            /* [in] */ BSTR bstrNewState) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SMchSetStateMachineIfState( 
            /* [in] */ BSTR bstrStateMachineName,
            /* [in] */ BSTR bstrOldState,
            /* [in] */ BSTR bstrNewState) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE SMchGetStateConcatName( 
            /* [in] */ BSTR bstrStateMachineName,
            /* [in] */ BSTR bstrState,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrOut) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE SMchGetCurrentStateConcatName( 
            /* [in] */ BSTR bstrStateMachineName,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrOut) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SMchGetStateProperty( 
            /* [in] */ BSTR bstrStateMachineName,
            /* [in] */ BSTR bstrState,
            /* [in] */ BSTR bstrPropertyName,
            /* [retval][out] */ VARIANT __RPC_FAR *var) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SMchPutStateProperty( 
            /* [in] */ BSTR bstrState,
            /* [in] */ BSTR bstrPropertyName,
            /* [in] */ VARIANT var) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SMchPutStateMachineStateProperty( 
            /* [in] */ BSTR bstrStateMachineName,
            /* [in] */ BSTR bstrState,
            /* [in] */ BSTR bstrPropertyName,
            /* [in] */ VARIANT var) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SMchPutInlineStateMethod( 
            /* [in] */ long lFlags,
            /* [in] */ BSTR bstrState,
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ BSTR bstrCode,
            /* [defaultvalue][optional][in] */ VARIANT varParams) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SMchPutStateMachineInlineStateMethod( 
            /* [in] */ long lFlags,
            /* [in] */ BSTR bstrStateMachineName,
            /* [in] */ BSTR bstrState,
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ BSTR bstrCode,
            /* [defaultvalue][optional][in] */ VARIANT varParams) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SMchGetInlineStateMethod( 
            /* [in] */ BSTR bstrStateMachineName,
            /* [in] */ BSTR bstrState,
            /* [in] */ BSTR bstrMethodName,
            /* [retval][out] */ BSTR __RPC_FAR *bstrCode) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SMchAddStateMethod( 
            /* [in] */ long lFlags,
            /* [in] */ IModule __RPC_FAR *pModule,
            /* [in] */ BSTR bstrModuleMethodName,
            /* [in] */ BSTR bstrStateMachineName,
            /* [in] */ BSTR bstrState,
            /* [in] */ BSTR bstrMethodName) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SMchGetStateMethodFlags( 
            /* [in] */ BSTR bstrStateMachineName,
            /* [in] */ BSTR bstrState,
            /* [in] */ BSTR bstrMethodName,
            /* [retval][out] */ long __RPC_FAR *plFlags) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE SMchOnInvokeMethodFailed( 
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ IPropertyList __RPC_FAR *plistParams) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IStateMachineExemplarVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IStateMachineExemplar __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IStateMachineExemplar __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IStateMachineExemplar __RPC_FAR * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Install )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [in] */ IModule __RPC_FAR *pmodule);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnLoad )( 
            IStateMachineExemplar __RPC_FAR * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUnload )( 
            IStateMachineExemplar __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SMchAddStateMachine )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrStateMachineName,
            /* [in] */ BSTR bstrInitialValue);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SMchSetState )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrState);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SMchSetStateMachineState )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrStateMachineName,
            /* [in] */ BSTR bstrState);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SMchIsInState )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrState,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolResult);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SMchIsStateMachineInState )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrStateMachineName,
            /* [in] */ BSTR bstrState,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolResult);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SMchSetIfState )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrOldState,
            /* [in] */ BSTR bstrNewState);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SMchSetStateMachineIfState )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrStateMachineName,
            /* [in] */ BSTR bstrOldState,
            /* [in] */ BSTR bstrNewState);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SMchGetStateConcatName )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrStateMachineName,
            /* [in] */ BSTR bstrState,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrOut);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SMchGetCurrentStateConcatName )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrStateMachineName,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ BSTR __RPC_FAR *pbstrOut);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SMchGetStateProperty )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrStateMachineName,
            /* [in] */ BSTR bstrState,
            /* [in] */ BSTR bstrPropertyName,
            /* [retval][out] */ VARIANT __RPC_FAR *var);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SMchPutStateProperty )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrState,
            /* [in] */ BSTR bstrPropertyName,
            /* [in] */ VARIANT var);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SMchPutStateMachineStateProperty )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrStateMachineName,
            /* [in] */ BSTR bstrState,
            /* [in] */ BSTR bstrPropertyName,
            /* [in] */ VARIANT var);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SMchPutInlineStateMethod )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [in] */ long lFlags,
            /* [in] */ BSTR bstrState,
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ BSTR bstrCode,
            /* [defaultvalue][optional][in] */ VARIANT varParams);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SMchPutStateMachineInlineStateMethod )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [in] */ long lFlags,
            /* [in] */ BSTR bstrStateMachineName,
            /* [in] */ BSTR bstrState,
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ BSTR bstrCode,
            /* [defaultvalue][optional][in] */ VARIANT varParams);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SMchGetInlineStateMethod )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrStateMachineName,
            /* [in] */ BSTR bstrState,
            /* [in] */ BSTR bstrMethodName,
            /* [retval][out] */ BSTR __RPC_FAR *bstrCode);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SMchAddStateMethod )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [in] */ long lFlags,
            /* [in] */ IModule __RPC_FAR *pModule,
            /* [in] */ BSTR bstrModuleMethodName,
            /* [in] */ BSTR bstrStateMachineName,
            /* [in] */ BSTR bstrState,
            /* [in] */ BSTR bstrMethodName);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SMchGetStateMethodFlags )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrStateMachineName,
            /* [in] */ BSTR bstrState,
            /* [in] */ BSTR bstrMethodName,
            /* [retval][out] */ long __RPC_FAR *plFlags);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SMchOnInvokeMethodFailed )( 
            IStateMachineExemplar __RPC_FAR * This,
            /* [in] */ BSTR bstrMethodName,
            /* [in] */ IPropertyList __RPC_FAR *plistParams);
        
        END_INTERFACE
    } IStateMachineExemplarVtbl;

    interface IStateMachineExemplar
    {
        CONST_VTBL struct IStateMachineExemplarVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IStateMachineExemplar_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IStateMachineExemplar_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IStateMachineExemplar_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IStateMachineExemplar_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IStateMachineExemplar_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IStateMachineExemplar_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IStateMachineExemplar_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IStateMachineExemplar_Initialize(This,pworld)	\
    (This)->lpVtbl -> Initialize(This,pworld)

#define IStateMachineExemplar_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IStateMachineExemplar_Install(This,pmodule)	\
    (This)->lpVtbl -> Install(This,pmodule)

#define IStateMachineExemplar_OnLoad(This)	\
    (This)->lpVtbl -> OnLoad(This)

#define IStateMachineExemplar_OnUnload(This)	\
    (This)->lpVtbl -> OnUnload(This)


#define IStateMachineExemplar_SMchAddStateMachine(This,bstrStateMachineName,bstrInitialValue)	\
    (This)->lpVtbl -> SMchAddStateMachine(This,bstrStateMachineName,bstrInitialValue)

#define IStateMachineExemplar_SMchSetState(This,bstrState)	\
    (This)->lpVtbl -> SMchSetState(This,bstrState)

#define IStateMachineExemplar_SMchSetStateMachineState(This,bstrStateMachineName,bstrState)	\
    (This)->lpVtbl -> SMchSetStateMachineState(This,bstrStateMachineName,bstrState)

#define IStateMachineExemplar_SMchIsInState(This,bstrState,pboolResult)	\
    (This)->lpVtbl -> SMchIsInState(This,bstrState,pboolResult)

#define IStateMachineExemplar_SMchIsStateMachineInState(This,bstrStateMachineName,bstrState,pboolResult)	\
    (This)->lpVtbl -> SMchIsStateMachineInState(This,bstrStateMachineName,bstrState,pboolResult)

#define IStateMachineExemplar_SMchSetIfState(This,bstrOldState,bstrNewState)	\
    (This)->lpVtbl -> SMchSetIfState(This,bstrOldState,bstrNewState)

#define IStateMachineExemplar_SMchSetStateMachineIfState(This,bstrStateMachineName,bstrOldState,bstrNewState)	\
    (This)->lpVtbl -> SMchSetStateMachineIfState(This,bstrStateMachineName,bstrOldState,bstrNewState)

#define IStateMachineExemplar_SMchGetStateConcatName(This,bstrStateMachineName,bstrState,bstrName,pbstrOut)	\
    (This)->lpVtbl -> SMchGetStateConcatName(This,bstrStateMachineName,bstrState,bstrName,pbstrOut)

#define IStateMachineExemplar_SMchGetCurrentStateConcatName(This,bstrStateMachineName,bstrName,pbstrOut)	\
    (This)->lpVtbl -> SMchGetCurrentStateConcatName(This,bstrStateMachineName,bstrName,pbstrOut)

#define IStateMachineExemplar_SMchGetStateProperty(This,bstrStateMachineName,bstrState,bstrPropertyName,var)	\
    (This)->lpVtbl -> SMchGetStateProperty(This,bstrStateMachineName,bstrState,bstrPropertyName,var)

#define IStateMachineExemplar_SMchPutStateProperty(This,bstrState,bstrPropertyName,var)	\
    (This)->lpVtbl -> SMchPutStateProperty(This,bstrState,bstrPropertyName,var)

#define IStateMachineExemplar_SMchPutStateMachineStateProperty(This,bstrStateMachineName,bstrState,bstrPropertyName,var)	\
    (This)->lpVtbl -> SMchPutStateMachineStateProperty(This,bstrStateMachineName,bstrState,bstrPropertyName,var)

#define IStateMachineExemplar_SMchPutInlineStateMethod(This,lFlags,bstrState,bstrMethodName,bstrCode,varParams)	\
    (This)->lpVtbl -> SMchPutInlineStateMethod(This,lFlags,bstrState,bstrMethodName,bstrCode,varParams)

#define IStateMachineExemplar_SMchPutStateMachineInlineStateMethod(This,lFlags,bstrStateMachineName,bstrState,bstrMethodName,bstrCode,varParams)	\
    (This)->lpVtbl -> SMchPutStateMachineInlineStateMethod(This,lFlags,bstrStateMachineName,bstrState,bstrMethodName,bstrCode,varParams)

#define IStateMachineExemplar_SMchGetInlineStateMethod(This,bstrStateMachineName,bstrState,bstrMethodName,bstrCode)	\
    (This)->lpVtbl -> SMchGetInlineStateMethod(This,bstrStateMachineName,bstrState,bstrMethodName,bstrCode)

#define IStateMachineExemplar_SMchAddStateMethod(This,lFlags,pModule,bstrModuleMethodName,bstrStateMachineName,bstrState,bstrMethodName)	\
    (This)->lpVtbl -> SMchAddStateMethod(This,lFlags,pModule,bstrModuleMethodName,bstrStateMachineName,bstrState,bstrMethodName)

#define IStateMachineExemplar_SMchGetStateMethodFlags(This,bstrStateMachineName,bstrState,bstrMethodName,plFlags)	\
    (This)->lpVtbl -> SMchGetStateMethodFlags(This,bstrStateMachineName,bstrState,bstrMethodName,plFlags)

#define IStateMachineExemplar_SMchOnInvokeMethodFailed(This,bstrMethodName,plistParams)	\
    (This)->lpVtbl -> SMchOnInvokeMethodFailed(This,bstrMethodName,plistParams)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStateMachineExemplar_SMchAddStateMachine_Proxy( 
    IStateMachineExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrStateMachineName,
    /* [in] */ BSTR bstrInitialValue);


void __RPC_STUB IStateMachineExemplar_SMchAddStateMachine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStateMachineExemplar_SMchSetState_Proxy( 
    IStateMachineExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrState);


void __RPC_STUB IStateMachineExemplar_SMchSetState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStateMachineExemplar_SMchSetStateMachineState_Proxy( 
    IStateMachineExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrStateMachineName,
    /* [in] */ BSTR bstrState);


void __RPC_STUB IStateMachineExemplar_SMchSetStateMachineState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStateMachineExemplar_SMchIsInState_Proxy( 
    IStateMachineExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrState,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolResult);


void __RPC_STUB IStateMachineExemplar_SMchIsInState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStateMachineExemplar_SMchIsStateMachineInState_Proxy( 
    IStateMachineExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrStateMachineName,
    /* [in] */ BSTR bstrState,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pboolResult);


void __RPC_STUB IStateMachineExemplar_SMchIsStateMachineInState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStateMachineExemplar_SMchSetIfState_Proxy( 
    IStateMachineExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrOldState,
    /* [in] */ BSTR bstrNewState);


void __RPC_STUB IStateMachineExemplar_SMchSetIfState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStateMachineExemplar_SMchSetStateMachineIfState_Proxy( 
    IStateMachineExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrStateMachineName,
    /* [in] */ BSTR bstrOldState,
    /* [in] */ BSTR bstrNewState);


void __RPC_STUB IStateMachineExemplar_SMchSetStateMachineIfState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IStateMachineExemplar_SMchGetStateConcatName_Proxy( 
    IStateMachineExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrStateMachineName,
    /* [in] */ BSTR bstrState,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrOut);


void __RPC_STUB IStateMachineExemplar_SMchGetStateConcatName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IStateMachineExemplar_SMchGetCurrentStateConcatName_Proxy( 
    IStateMachineExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrStateMachineName,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrOut);


void __RPC_STUB IStateMachineExemplar_SMchGetCurrentStateConcatName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStateMachineExemplar_SMchGetStateProperty_Proxy( 
    IStateMachineExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrStateMachineName,
    /* [in] */ BSTR bstrState,
    /* [in] */ BSTR bstrPropertyName,
    /* [retval][out] */ VARIANT __RPC_FAR *var);


void __RPC_STUB IStateMachineExemplar_SMchGetStateProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStateMachineExemplar_SMchPutStateProperty_Proxy( 
    IStateMachineExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrState,
    /* [in] */ BSTR bstrPropertyName,
    /* [in] */ VARIANT var);


void __RPC_STUB IStateMachineExemplar_SMchPutStateProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStateMachineExemplar_SMchPutStateMachineStateProperty_Proxy( 
    IStateMachineExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrStateMachineName,
    /* [in] */ BSTR bstrState,
    /* [in] */ BSTR bstrPropertyName,
    /* [in] */ VARIANT var);


void __RPC_STUB IStateMachineExemplar_SMchPutStateMachineStateProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStateMachineExemplar_SMchPutInlineStateMethod_Proxy( 
    IStateMachineExemplar __RPC_FAR * This,
    /* [in] */ long lFlags,
    /* [in] */ BSTR bstrState,
    /* [in] */ BSTR bstrMethodName,
    /* [in] */ BSTR bstrCode,
    /* [defaultvalue][optional][in] */ VARIANT varParams);


void __RPC_STUB IStateMachineExemplar_SMchPutInlineStateMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStateMachineExemplar_SMchPutStateMachineInlineStateMethod_Proxy( 
    IStateMachineExemplar __RPC_FAR * This,
    /* [in] */ long lFlags,
    /* [in] */ BSTR bstrStateMachineName,
    /* [in] */ BSTR bstrState,
    /* [in] */ BSTR bstrMethodName,
    /* [in] */ BSTR bstrCode,
    /* [defaultvalue][optional][in] */ VARIANT varParams);


void __RPC_STUB IStateMachineExemplar_SMchPutStateMachineInlineStateMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStateMachineExemplar_SMchGetInlineStateMethod_Proxy( 
    IStateMachineExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrStateMachineName,
    /* [in] */ BSTR bstrState,
    /* [in] */ BSTR bstrMethodName,
    /* [retval][out] */ BSTR __RPC_FAR *bstrCode);


void __RPC_STUB IStateMachineExemplar_SMchGetInlineStateMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStateMachineExemplar_SMchAddStateMethod_Proxy( 
    IStateMachineExemplar __RPC_FAR * This,
    /* [in] */ long lFlags,
    /* [in] */ IModule __RPC_FAR *pModule,
    /* [in] */ BSTR bstrModuleMethodName,
    /* [in] */ BSTR bstrStateMachineName,
    /* [in] */ BSTR bstrState,
    /* [in] */ BSTR bstrMethodName);


void __RPC_STUB IStateMachineExemplar_SMchAddStateMethod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IStateMachineExemplar_SMchGetStateMethodFlags_Proxy( 
    IStateMachineExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrStateMachineName,
    /* [in] */ BSTR bstrState,
    /* [in] */ BSTR bstrMethodName,
    /* [retval][out] */ long __RPC_FAR *plFlags);


void __RPC_STUB IStateMachineExemplar_SMchGetStateMethodFlags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IStateMachineExemplar_SMchOnInvokeMethodFailed_Proxy( 
    IStateMachineExemplar __RPC_FAR * This,
    /* [in] */ BSTR bstrMethodName,
    /* [in] */ IPropertyList __RPC_FAR *plistParams);


void __RPC_STUB IStateMachineExemplar_SMchOnInvokeMethodFailed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IStateMachineExemplar_INTERFACE_DEFINED__ */


#ifndef __IWebPageExemplar_INTERFACE_DEFINED__
#define __IWebPageExemplar_INTERFACE_DEFINED__

/* interface IWebPageExemplar */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IWebPageExemplar;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("63CA06F6-5CC2-11d2-82F0-00104B26A373")
    IWebPageExemplar : public IVWDispatch
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Show( void) = 0;
        
        virtual /* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE OnLButtonUp( 
            /* [in] */ int hWnd,
            /* [in] */ int x,
            /* [in] */ int y,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWebPageExemplarVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IWebPageExemplar __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IWebPageExemplar __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IWebPageExemplar __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IWebPageExemplar __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IWebPageExemplar __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IWebPageExemplar __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IWebPageExemplar __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            IWebPageExemplar __RPC_FAR * This,
            /* [in] */ IWorld __RPC_FAR *pworld);
        
        /* [helpstring][restricted] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            IWebPageExemplar __RPC_FAR * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Install )( 
            IWebPageExemplar __RPC_FAR * This,
            /* [in] */ IModule __RPC_FAR *pmodule);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnLoad )( 
            IWebPageExemplar __RPC_FAR * This);
        
        /* [id][helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnUnload )( 
            IWebPageExemplar __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Show )( 
            IWebPageExemplar __RPC_FAR * This);
        
        /* [helpstring][hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnLButtonUp )( 
            IWebPageExemplar __RPC_FAR * This,
            /* [in] */ int hWnd,
            /* [in] */ int x,
            /* [in] */ int y,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);
        
        END_INTERFACE
    } IWebPageExemplarVtbl;

    interface IWebPageExemplar
    {
        CONST_VTBL struct IWebPageExemplarVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWebPageExemplar_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWebPageExemplar_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWebPageExemplar_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWebPageExemplar_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IWebPageExemplar_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IWebPageExemplar_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IWebPageExemplar_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IWebPageExemplar_Initialize(This,pworld)	\
    (This)->lpVtbl -> Initialize(This,pworld)

#define IWebPageExemplar_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define IWebPageExemplar_Install(This,pmodule)	\
    (This)->lpVtbl -> Install(This,pmodule)

#define IWebPageExemplar_OnLoad(This)	\
    (This)->lpVtbl -> OnLoad(This)

#define IWebPageExemplar_OnUnload(This)	\
    (This)->lpVtbl -> OnUnload(This)


#define IWebPageExemplar_Show(This)	\
    (This)->lpVtbl -> Show(This)

#define IWebPageExemplar_OnLButtonUp(This,hWnd,x,y,pbool)	\
    (This)->lpVtbl -> OnLButtonUp(This,hWnd,x,y,pbool)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IWebPageExemplar_Show_Proxy( 
    IWebPageExemplar __RPC_FAR * This);


void __RPC_STUB IWebPageExemplar_Show_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][hidden] */ HRESULT STDMETHODCALLTYPE IWebPageExemplar_OnLButtonUp_Proxy( 
    IWebPageExemplar __RPC_FAR * This,
    /* [in] */ int hWnd,
    /* [in] */ int x,
    /* [in] */ int y,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbool);


void __RPC_STUB IWebPageExemplar_OnLButtonUp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWebPageExemplar_INTERFACE_DEFINED__ */


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
