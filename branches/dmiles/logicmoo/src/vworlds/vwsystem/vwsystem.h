/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jun 25 18:44:24 2000
 */
/* Compiler settings for vwsystem.idl:
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

#ifndef __vwsystem_h__
#define __vwsystem_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __MarshallBuffer_FWD_DEFINED__
#define __MarshallBuffer_FWD_DEFINED__

#ifdef __cplusplus
typedef class MarshallBuffer MarshallBuffer;
#else
typedef struct MarshallBuffer MarshallBuffer;
#endif /* __cplusplus */

#endif 	/* __MarshallBuffer_FWD_DEFINED__ */


#ifndef __UnMarshallBuffer_FWD_DEFINED__
#define __UnMarshallBuffer_FWD_DEFINED__

#ifdef __cplusplus
typedef class UnMarshallBuffer UnMarshallBuffer;
#else
typedef struct UnMarshallBuffer UnMarshallBuffer;
#endif /* __cplusplus */

#endif 	/* __UnMarshallBuffer_FWD_DEFINED__ */


#ifndef __PropertyCollection_FWD_DEFINED__
#define __PropertyCollection_FWD_DEFINED__

#ifdef __cplusplus
typedef class PropertyCollection PropertyCollection;
#else
typedef struct PropertyCollection PropertyCollection;
#endif /* __cplusplus */

#endif 	/* __PropertyCollection_FWD_DEFINED__ */


#ifndef __PropertySecurity_FWD_DEFINED__
#define __PropertySecurity_FWD_DEFINED__

#ifdef __cplusplus
typedef class PropertySecurity PropertySecurity;
#else
typedef struct PropertySecurity PropertySecurity;
#endif /* __cplusplus */

#endif 	/* __PropertySecurity_FWD_DEFINED__ */


#ifndef __PropertyList_FWD_DEFINED__
#define __PropertyList_FWD_DEFINED__

#ifdef __cplusplus
typedef class PropertyList PropertyList;
#else
typedef struct PropertyList PropertyList;
#endif /* __cplusplus */

#endif 	/* __PropertyList_FWD_DEFINED__ */


#ifndef __PropertyMap_FWD_DEFINED__
#define __PropertyMap_FWD_DEFINED__

#ifdef __cplusplus
typedef class PropertyMap PropertyMap;
#else
typedef struct PropertyMap PropertyMap;
#endif /* __cplusplus */

#endif 	/* __PropertyMap_FWD_DEFINED__ */


#ifndef __COMModule_FWD_DEFINED__
#define __COMModule_FWD_DEFINED__

#ifdef __cplusplus
typedef class COMModule COMModule;
#else
typedef struct COMModule COMModule;
#endif /* __cplusplus */

#endif 	/* __COMModule_FWD_DEFINED__ */


#ifndef __ScriptModule_FWD_DEFINED__
#define __ScriptModule_FWD_DEFINED__

#ifdef __cplusplus
typedef class ScriptModule ScriptModule;
#else
typedef struct ScriptModule ScriptModule;
#endif /* __cplusplus */

#endif 	/* __ScriptModule_FWD_DEFINED__ */


#ifndef __Method_FWD_DEFINED__
#define __Method_FWD_DEFINED__

#ifdef __cplusplus
typedef class Method Method;
#else
typedef struct Method Method;
#endif /* __cplusplus */

#endif 	/* __Method_FWD_DEFINED__ */


#ifndef __Thing_FWD_DEFINED__
#define __Thing_FWD_DEFINED__

#ifdef __cplusplus
typedef class Thing Thing;
#else
typedef struct Thing Thing;
#endif /* __cplusplus */

#endif 	/* __Thing_FWD_DEFINED__ */


#ifndef __World_FWD_DEFINED__
#define __World_FWD_DEFINED__

#ifdef __cplusplus
typedef class World World;
#else
typedef struct World World;
#endif /* __cplusplus */

#endif 	/* __World_FWD_DEFINED__ */


#ifndef __VWComm_FWD_DEFINED__
#define __VWComm_FWD_DEFINED__

#ifdef __cplusplus
typedef class VWComm VWComm;
#else
typedef struct VWComm VWComm;
#endif /* __cplusplus */

#endif 	/* __VWComm_FWD_DEFINED__ */


#ifndef __VWCommConnection_FWD_DEFINED__
#define __VWCommConnection_FWD_DEFINED__

#ifdef __cplusplus
typedef class VWCommConnection VWCommConnection;
#else
typedef struct VWCommConnection VWCommConnection;
#endif /* __cplusplus */

#endif 	/* __VWCommConnection_FWD_DEFINED__ */


#ifndef __VWCommMessage_FWD_DEFINED__
#define __VWCommMessage_FWD_DEFINED__

#ifdef __cplusplus
typedef class VWCommMessage VWCommMessage;
#else
typedef struct VWCommMessage VWCommMessage;
#endif /* __cplusplus */

#endif 	/* __VWCommMessage_FWD_DEFINED__ */


#ifndef __FastComm_FWD_DEFINED__
#define __FastComm_FWD_DEFINED__

#ifdef __cplusplus
typedef class FastComm FastComm;
#else
typedef struct FastComm FastComm;
#endif /* __cplusplus */

#endif 	/* __FastComm_FWD_DEFINED__ */


#ifndef __FastCommConnection_FWD_DEFINED__
#define __FastCommConnection_FWD_DEFINED__

#ifdef __cplusplus
typedef class FastCommConnection FastCommConnection;
#else
typedef struct FastCommConnection FastCommConnection;
#endif /* __cplusplus */

#endif 	/* __FastCommConnection_FWD_DEFINED__ */


#ifndef __FastCommMessage_FWD_DEFINED__
#define __FastCommMessage_FWD_DEFINED__

#ifdef __cplusplus
typedef class FastCommMessage FastCommMessage;
#else
typedef struct FastCommMessage FastCommMessage;
#endif /* __cplusplus */

#endif 	/* __FastCommMessage_FWD_DEFINED__ */


#ifndef __VWClient_FWD_DEFINED__
#define __VWClient_FWD_DEFINED__

#ifdef __cplusplus
typedef class VWClient VWClient;
#else
typedef struct VWClient VWClient;
#endif /* __cplusplus */

#endif 	/* __VWClient_FWD_DEFINED__ */


#ifndef ___VWClientOCX_FWD_DEFINED__
#define ___VWClientOCX_FWD_DEFINED__
typedef interface _VWClientOCX _VWClientOCX;
#endif 	/* ___VWClientOCX_FWD_DEFINED__ */


#ifndef __VWClientOCX_FWD_DEFINED__
#define __VWClientOCX_FWD_DEFINED__

#ifdef __cplusplus
typedef class VWClientOCX VWClientOCX;
#else
typedef struct VWClientOCX VWClientOCX;
#endif /* __cplusplus */

#endif 	/* __VWClientOCX_FWD_DEFINED__ */


#ifndef __VWServer_FWD_DEFINED__
#define __VWServer_FWD_DEFINED__

#ifdef __cplusplus
typedef class VWServer VWServer;
#else
typedef struct VWServer VWServer;
#endif /* __cplusplus */

#endif 	/* __VWServer_FWD_DEFINED__ */


#ifndef __VWObjectRepository_FWD_DEFINED__
#define __VWObjectRepository_FWD_DEFINED__

#ifdef __cplusplus
typedef class VWObjectRepository VWObjectRepository;
#else
typedef struct VWObjectRepository VWObjectRepository;
#endif /* __cplusplus */

#endif 	/* __VWObjectRepository_FWD_DEFINED__ */


#ifndef __VWAdmin_FWD_DEFINED__
#define __VWAdmin_FWD_DEFINED__

#ifdef __cplusplus
typedef class VWAdmin VWAdmin;
#else
typedef struct VWAdmin VWAdmin;
#endif /* __cplusplus */

#endif 	/* __VWAdmin_FWD_DEFINED__ */


#ifndef __InternetFileManager_FWD_DEFINED__
#define __InternetFileManager_FWD_DEFINED__

#ifdef __cplusplus
typedef class InternetFileManager InternetFileManager;
#else
typedef struct InternetFileManager InternetFileManager;
#endif /* __cplusplus */

#endif 	/* __InternetFileManager_FWD_DEFINED__ */


#ifndef __DServ_FWD_DEFINED__
#define __DServ_FWD_DEFINED__

#ifdef __cplusplus
typedef class DServ DServ;
#else
typedef struct DServ DServ;
#endif /* __cplusplus */

#endif 	/* __DServ_FWD_DEFINED__ */


#ifndef __VWAnimator_FWD_DEFINED__
#define __VWAnimator_FWD_DEFINED__

#ifdef __cplusplus
typedef class VWAnimator VWAnimator;
#else
typedef struct VWAnimator VWAnimator;
#endif /* __cplusplus */

#endif 	/* __VWAnimator_FWD_DEFINED__ */


#ifndef __VWAnimation_FWD_DEFINED__
#define __VWAnimation_FWD_DEFINED__

#ifdef __cplusplus
typedef class VWAnimation VWAnimation;
#else
typedef struct VWAnimation VWAnimation;
#endif /* __cplusplus */

#endif 	/* __VWAnimation_FWD_DEFINED__ */


#ifndef __VWAnimationCallback_FWD_DEFINED__
#define __VWAnimationCallback_FWD_DEFINED__

#ifdef __cplusplus
typedef class VWAnimationCallback VWAnimationCallback;
#else
typedef struct VWAnimationCallback VWAnimationCallback;
#endif /* __cplusplus */

#endif 	/* __VWAnimationCallback_FWD_DEFINED__ */


#ifndef __VWHtml_FWD_DEFINED__
#define __VWHtml_FWD_DEFINED__

#ifdef __cplusplus
typedef class VWHtml VWHtml;
#else
typedef struct VWHtml VWHtml;
#endif /* __cplusplus */

#endif 	/* __VWHtml_FWD_DEFINED__ */


#ifndef __VWHtmlHelper_FWD_DEFINED__
#define __VWHtmlHelper_FWD_DEFINED__

#ifdef __cplusplus
typedef class VWHtmlHelper VWHtmlHelper;
#else
typedef struct VWHtmlHelper VWHtmlHelper;
#endif /* __cplusplus */

#endif 	/* __VWHtmlHelper_FWD_DEFINED__ */


#ifndef __MenuItem_FWD_DEFINED__
#define __MenuItem_FWD_DEFINED__

#ifdef __cplusplus
typedef class MenuItem MenuItem;
#else
typedef struct MenuItem MenuItem;
#endif /* __cplusplus */

#endif 	/* __MenuItem_FWD_DEFINED__ */


#ifndef __OCXProxy_FWD_DEFINED__
#define __OCXProxy_FWD_DEFINED__

#ifdef __cplusplus
typedef class OCXProxy OCXProxy;
#else
typedef struct OCXProxy OCXProxy;
#endif /* __cplusplus */

#endif 	/* __OCXProxy_FWD_DEFINED__ */


#ifndef __AvatarProfile_FWD_DEFINED__
#define __AvatarProfile_FWD_DEFINED__

#ifdef __cplusplus
typedef class AvatarProfile AvatarProfile;
#else
typedef struct AvatarProfile AvatarProfile;
#endif /* __cplusplus */

#endif 	/* __AvatarProfile_FWD_DEFINED__ */


#ifndef __AvatarProfileData_FWD_DEFINED__
#define __AvatarProfileData_FWD_DEFINED__

#ifdef __cplusplus
typedef class AvatarProfileData AvatarProfileData;
#else
typedef struct AvatarProfileData AvatarProfileData;
#endif /* __cplusplus */

#endif 	/* __AvatarProfileData_FWD_DEFINED__ */


#ifndef __Authenticate_FWD_DEFINED__
#define __Authenticate_FWD_DEFINED__

#ifdef __cplusplus
typedef class Authenticate Authenticate;
#else
typedef struct Authenticate Authenticate;
#endif /* __cplusplus */

#endif 	/* __Authenticate_FWD_DEFINED__ */


#ifndef __Authentication_FWD_DEFINED__
#define __Authentication_FWD_DEFINED__

#ifdef __cplusplus
typedef class Authentication Authentication;
#else
typedef struct Authentication Authentication;
#endif /* __cplusplus */

#endif 	/* __Authentication_FWD_DEFINED__ */


#ifndef __SystemExemplar_FWD_DEFINED__
#define __SystemExemplar_FWD_DEFINED__

#ifdef __cplusplus
typedef class SystemExemplar SystemExemplar;
#else
typedef struct SystemExemplar SystemExemplar;
#endif /* __cplusplus */

#endif 	/* __SystemExemplar_FWD_DEFINED__ */


#ifndef __ThingExemplar_FWD_DEFINED__
#define __ThingExemplar_FWD_DEFINED__

#ifdef __cplusplus
typedef class ThingExemplar ThingExemplar;
#else
typedef struct ThingExemplar ThingExemplar;
#endif /* __cplusplus */

#endif 	/* __ThingExemplar_FWD_DEFINED__ */


#ifndef __ArtifactExemplar_FWD_DEFINED__
#define __ArtifactExemplar_FWD_DEFINED__

#ifdef __cplusplus
typedef class ArtifactExemplar ArtifactExemplar;
#else
typedef struct ArtifactExemplar ArtifactExemplar;
#endif /* __cplusplus */

#endif 	/* __ArtifactExemplar_FWD_DEFINED__ */


#ifndef __AvatarExemplar_FWD_DEFINED__
#define __AvatarExemplar_FWD_DEFINED__

#ifdef __cplusplus
typedef class AvatarExemplar AvatarExemplar;
#else
typedef struct AvatarExemplar AvatarExemplar;
#endif /* __cplusplus */

#endif 	/* __AvatarExemplar_FWD_DEFINED__ */


#ifndef __HostExemplar_FWD_DEFINED__
#define __HostExemplar_FWD_DEFINED__

#ifdef __cplusplus
typedef class HostExemplar HostExemplar;
#else
typedef struct HostExemplar HostExemplar;
#endif /* __cplusplus */

#endif 	/* __HostExemplar_FWD_DEFINED__ */


#ifndef __PortalExemplar_FWD_DEFINED__
#define __PortalExemplar_FWD_DEFINED__

#ifdef __cplusplus
typedef class PortalExemplar PortalExemplar;
#else
typedef struct PortalExemplar PortalExemplar;
#endif /* __cplusplus */

#endif 	/* __PortalExemplar_FWD_DEFINED__ */


#ifndef __IWPortalExemplar_FWD_DEFINED__
#define __IWPortalExemplar_FWD_DEFINED__

#ifdef __cplusplus
typedef class IWPortalExemplar IWPortalExemplar;
#else
typedef struct IWPortalExemplar IWPortalExemplar;
#endif /* __cplusplus */

#endif 	/* __IWPortalExemplar_FWD_DEFINED__ */


#ifndef __RoomExemplar_FWD_DEFINED__
#define __RoomExemplar_FWD_DEFINED__

#ifdef __cplusplus
typedef class RoomExemplar RoomExemplar;
#else
typedef struct RoomExemplar RoomExemplar;
#endif /* __cplusplus */

#endif 	/* __RoomExemplar_FWD_DEFINED__ */


#ifndef __StateMachineExemplar_FWD_DEFINED__
#define __StateMachineExemplar_FWD_DEFINED__

#ifdef __cplusplus
typedef class StateMachineExemplar StateMachineExemplar;
#else
typedef struct StateMachineExemplar StateMachineExemplar;
#endif /* __cplusplus */

#endif 	/* __StateMachineExemplar_FWD_DEFINED__ */


#ifndef __WebPageExemplar_FWD_DEFINED__
#define __WebPageExemplar_FWD_DEFINED__

#ifdef __cplusplus
typedef class WebPageExemplar WebPageExemplar;
#else
typedef struct WebPageExemplar WebPageExemplar;
#endif /* __cplusplus */

#endif 	/* __WebPageExemplar_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "vwobject.h"
#include "vwclient.h"
#include "vwserver.h"
#include "vwodb.h"
#include "vwanim.h"
#include "vwhtml.h"
#include "inetfile.h"
#include "dirserv.h"
#include "vwsysex.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __VWSYSTEMLib_LIBRARY_DEFINED__
#define __VWSYSTEMLib_LIBRARY_DEFINED__

/* library VWSYSTEMLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_VWSYSTEMLib;

EXTERN_C const CLSID CLSID_MarshallBuffer;

#ifdef __cplusplus

class DECLSPEC_UUID("590D9881-A417-11CF-9BF0-0080C7A56A8A")
MarshallBuffer;
#endif

EXTERN_C const CLSID CLSID_UnMarshallBuffer;

#ifdef __cplusplus

class DECLSPEC_UUID("590D9883-A417-11CF-9BF0-0080C7A56A8A")
UnMarshallBuffer;
#endif

EXTERN_C const CLSID CLSID_PropertyCollection;

#ifdef __cplusplus

class DECLSPEC_UUID("590D986F-A417-11CF-9BF0-0080C7A56A8A")
PropertyCollection;
#endif

EXTERN_C const CLSID CLSID_PropertySecurity;

#ifdef __cplusplus

class DECLSPEC_UUID("590D9871-A417-11CF-9BF0-0080C7A56A8A")
PropertySecurity;
#endif

EXTERN_C const CLSID CLSID_PropertyList;

#ifdef __cplusplus

class DECLSPEC_UUID("590D9873-A417-11CF-9BF0-0080C7A56A8A")
PropertyList;
#endif

EXTERN_C const CLSID CLSID_PropertyMap;

#ifdef __cplusplus

class DECLSPEC_UUID("590D9875-A417-11CF-9BF0-0080C7A56A8A")
PropertyMap;
#endif

EXTERN_C const CLSID CLSID_COMModule;

#ifdef __cplusplus

class DECLSPEC_UUID("590D9877-A417-11CF-9BF0-0080C7A56A8A")
COMModule;
#endif

EXTERN_C const CLSID CLSID_ScriptModule;

#ifdef __cplusplus

class DECLSPEC_UUID("590D9879-A417-11CF-9BF0-0080C7A56A8A")
ScriptModule;
#endif

EXTERN_C const CLSID CLSID_Method;

#ifdef __cplusplus

class DECLSPEC_UUID("590D987B-A417-11CF-9BF0-0080C7A56A8A")
Method;
#endif

EXTERN_C const CLSID CLSID_Thing;

#ifdef __cplusplus

class DECLSPEC_UUID("590D987D-A417-11CF-9BF0-0080C7A56A8A")
Thing;
#endif

EXTERN_C const CLSID CLSID_World;

#ifdef __cplusplus

class DECLSPEC_UUID("590D987F-A417-11CF-9BF0-0080C7A56A8A")
World;
#endif

EXTERN_C const CLSID CLSID_VWComm;

#ifdef __cplusplus

class DECLSPEC_UUID("7736D441-DBD6-11CF-A516-00AA00B595DB")
VWComm;
#endif

EXTERN_C const CLSID CLSID_VWCommConnection;

#ifdef __cplusplus

class DECLSPEC_UUID("7736D443-DBD6-11CF-A516-00AA00B595DB")
VWCommConnection;
#endif

EXTERN_C const CLSID CLSID_VWCommMessage;

#ifdef __cplusplus

class DECLSPEC_UUID("7736D445-DBD6-11CF-A516-00AA00B595DB")
VWCommMessage;
#endif

EXTERN_C const CLSID CLSID_FastComm;

#ifdef __cplusplus

class DECLSPEC_UUID("00F5F3D4-95A0-4e8e-88BA-C7DB1F836CCB")
FastComm;
#endif

EXTERN_C const CLSID CLSID_FastCommConnection;

#ifdef __cplusplus

class DECLSPEC_UUID("00F5F3D6-95A0-4e8e-88BA-C7DB1F836CCB")
FastCommConnection;
#endif

EXTERN_C const CLSID CLSID_FastCommMessage;

#ifdef __cplusplus

class DECLSPEC_UUID("00F5F3D8-95A0-4e8e-88BA-C7DB1F836CCB")
FastCommMessage;
#endif

EXTERN_C const CLSID CLSID_VWClient;

#ifdef __cplusplus

class DECLSPEC_UUID("590D9823-A417-11CF-9BF0-0080C7A56A8A")
VWClient;
#endif

#ifndef ___VWClientOCX_DISPINTERFACE_DEFINED__
#define ___VWClientOCX_DISPINTERFACE_DEFINED__

/* dispinterface _VWClientOCX */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__VWClientOCX;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("D4898802-20EF-11D0-8A96-00A0C9082583")
    _VWClientOCX : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _VWClientOCXVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _VWClientOCX __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _VWClientOCX __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _VWClientOCX __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _VWClientOCX __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _VWClientOCX __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _VWClientOCX __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _VWClientOCX __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } _VWClientOCXVtbl;

    interface _VWClientOCX
    {
        CONST_VTBL struct _VWClientOCXVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _VWClientOCX_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _VWClientOCX_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _VWClientOCX_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _VWClientOCX_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _VWClientOCX_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _VWClientOCX_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _VWClientOCX_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___VWClientOCX_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_VWClientOCX;

#ifdef __cplusplus

class DECLSPEC_UUID("D4898803-20EF-11D0-8A96-00A0C9082583")
VWClientOCX;
#endif

EXTERN_C const CLSID CLSID_VWServer;

#ifdef __cplusplus

class DECLSPEC_UUID("F8584902-00F4-11D0-A516-00AA00B595DB")
VWServer;
#endif

EXTERN_C const CLSID CLSID_VWObjectRepository;

#ifdef __cplusplus

class DECLSPEC_UUID("8D20B9CE-E963-11CF-A516-00AA00B595DB")
VWObjectRepository;
#endif

EXTERN_C const CLSID CLSID_VWAdmin;

#ifdef __cplusplus

class DECLSPEC_UUID("02043F43-02B3-11d2-80F5-00C04FAD7D6D")
VWAdmin;
#endif

EXTERN_C const CLSID CLSID_InternetFileManager;

#ifdef __cplusplus

class DECLSPEC_UUID("120113E0-E936-11CF-87BD-00AA00B4DF93")
InternetFileManager;
#endif

EXTERN_C const CLSID CLSID_DServ;

#ifdef __cplusplus

class DECLSPEC_UUID("40A94351-9AE4-11D0-8587-00A0C90F2911")
DServ;
#endif

EXTERN_C const CLSID CLSID_VWAnimator;

#ifdef __cplusplus

class DECLSPEC_UUID("FF08F5A8-2E6B-11D0-89AB-00A0C9054129")
VWAnimator;
#endif

EXTERN_C const CLSID CLSID_VWAnimation;

#ifdef __cplusplus

class DECLSPEC_UUID("FF08F5AA-2E6B-11D0-89AB-00A0C9054129")
VWAnimation;
#endif

EXTERN_C const CLSID CLSID_VWAnimationCallback;

#ifdef __cplusplus

class DECLSPEC_UUID("FF08F5AC-2E6B-11D0-89AB-00A0C9054129")
VWAnimationCallback;
#endif

EXTERN_C const CLSID CLSID_VWHtml;

#ifdef __cplusplus

class DECLSPEC_UUID("7E3832CD-4F4E-11D0-89AB-00A0C9054129")
VWHtml;
#endif

EXTERN_C const CLSID CLSID_VWHtmlHelper;

#ifdef __cplusplus

class DECLSPEC_UUID("7E3832CF-4F4E-11D0-89AB-00A0C9054129")
VWHtmlHelper;
#endif

EXTERN_C const CLSID CLSID_MenuItem;

#ifdef __cplusplus

class DECLSPEC_UUID("D4916C26-00C2-11D0-8A96-00A0C9082583")
MenuItem;
#endif

EXTERN_C const CLSID CLSID_OCXProxy;

#ifdef __cplusplus

class DECLSPEC_UUID("09362219-2570-11D1-83BF-00C04FB6FA46")
OCXProxy;
#endif

EXTERN_C const CLSID CLSID_AvatarProfile;

#ifdef __cplusplus

class DECLSPEC_UUID("D4E7472C-2953-11D1-AFB6-00C04FB915AA")
AvatarProfile;
#endif

EXTERN_C const CLSID CLSID_AvatarProfileData;

#ifdef __cplusplus

class DECLSPEC_UUID("D4E7472E-2953-11D1-AFB6-00C04FB915AA")
AvatarProfileData;
#endif

EXTERN_C const CLSID CLSID_Authenticate;

#ifdef __cplusplus

class DECLSPEC_UUID("955A131A-731F-11D1-9082-00C04FD91EFE")
Authenticate;
#endif

EXTERN_C const CLSID CLSID_Authentication;

#ifdef __cplusplus

class DECLSPEC_UUID("955A131C-731F-11D1-9082-00C04FD91EFE")
Authentication;
#endif

EXTERN_C const CLSID CLSID_SystemExemplar;

#ifdef __cplusplus

class DECLSPEC_UUID("717B5A84-7267-11d1-959F-00C04FD91F4D")
SystemExemplar;
#endif

EXTERN_C const CLSID CLSID_ThingExemplar;

#ifdef __cplusplus

class DECLSPEC_UUID("99AD3619-AAB5-11D0-BAE3-00C04FC2CAB1")
ThingExemplar;
#endif

EXTERN_C const CLSID CLSID_ArtifactExemplar;

#ifdef __cplusplus

class DECLSPEC_UUID("99AD361D-AAB5-11D0-BAE3-00C04FC2CAB1")
ArtifactExemplar;
#endif

EXTERN_C const CLSID CLSID_AvatarExemplar;

#ifdef __cplusplus

class DECLSPEC_UUID("99AD361F-AAB5-11D0-BAE3-00C04FC2CAB1")
AvatarExemplar;
#endif

EXTERN_C const CLSID CLSID_HostExemplar;

#ifdef __cplusplus

class DECLSPEC_UUID("4874AC90-1B87-11d2-AC71-00104B26A373")
HostExemplar;
#endif

EXTERN_C const CLSID CLSID_PortalExemplar;

#ifdef __cplusplus

class DECLSPEC_UUID("99AD3625-AAB5-11D0-BAE3-00C04FC2CAB1")
PortalExemplar;
#endif

EXTERN_C const CLSID CLSID_IWPortalExemplar;

#ifdef __cplusplus

class DECLSPEC_UUID("FFA01B40-9F4C-11d1-80E8-00C04FC2A129")
IWPortalExemplar;
#endif

EXTERN_C const CLSID CLSID_RoomExemplar;

#ifdef __cplusplus

class DECLSPEC_UUID("99AD3627-AAB5-11D0-BAE3-00C04FC2CAB1")
RoomExemplar;
#endif

EXTERN_C const CLSID CLSID_StateMachineExemplar;

#ifdef __cplusplus

class DECLSPEC_UUID("4D83E7E0-F47B-11d1-A392-0000F807558D")
StateMachineExemplar;
#endif

EXTERN_C const CLSID CLSID_WebPageExemplar;

#ifdef __cplusplus

class DECLSPEC_UUID("2D0F3A43-5CC3-11d2-82F0-00104B26A373")
WebPageExemplar;
#endif
#endif /* __VWSYSTEMLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
